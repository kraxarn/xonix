import argparse
import hashlib
import os
import pathlib
import shutil
import sys
import typing
import zipfile

import requests

parser = argparse.ArgumentParser()

parser.add_argument("type", choices=["editor", "export-templates"])
parser.add_argument("--godot-version", required=True)
parser.add_argument("--godot-edition")
parser.add_argument("--verify", type=bool, default=True)
parser.add_argument("--extract", type=bool, default=True)
parser.add_argument("--install", type=bool, default=True)

args = parser.parse_args()
godot_version: typing.Optional[str] = args.godot_version
godot_edition: typing.Optional[str] = args.godot_edition

if args.type == "editor" and godot_edition is None:
	sys.exit("argument --godot-edition required when type is editor")

base_url = f"https://downloads.tuxfamily.org/godotengine/{godot_version}"
godot_name = f"Godot_v{godot_version}-stable"
engine_filename = f"{godot_name}_{godot_edition}.zip"
engine_url = f"{base_url}/{engine_filename}"
template_filename = f"{godot_name}_export_templates.tpz"
template_url = f"{base_url}/{template_filename}"


def download_file(url: str, filename: str):
	full_url = f"{url}/{filename}"
	print(f"download: {full_url}")
	if os.path.isfile(filename):
		return
	with requests.get(full_url, stream=True) as response:
		with open(filename, "wb") as file:
			for chunk in response.iter_content(chunk_size=8192):
				file.write(chunk)


def download_str(source: str):
	with requests.get(source) as response:
		return response.text


def extract(filename: str):
	print(f"extract: {filename}")
	with zipfile.ZipFile(filename, "r") as zip_file:
		zip_file.extractall()


def get_hash(filename: str):
	sums_url = f"{base_url}/SHA512-SUMS.txt"
	for line in download_str(sums_url).splitlines():
		parts = line.split("  ")
		if parts[1] == filename:
			return parts[0]
	return ""


def verify(filename: str):
	print(f"verify: {filename}")
	sha512 = hashlib.sha512()
	with open(filename, "rb") as file:
		while True:
			data = file.read(65536)  # 64 kb
			if not data:
				break
			sha512.update(data)
	file_hash = sha512.hexdigest()
	if not file_hash == get_hash(filename):
		sys.exit(f"incorrect checksum: {filename} ({file_hash})")


download_filename: str

match args.type:
	case "editor":
		download_filename = f"{godot_name}_{godot_edition}.zip"
	case "export-templates":
		download_filename = f"{godot_name}_export_templates.tpz"
	case _:
		sys.exit(f"unknown type: {args.type}")

download_file(base_url, download_filename)

if args.verify:
	verify(download_filename)

if args.extract:
	extract(download_filename)

if args.install:
	if args.type == "editor":
		godot_path: pathlib.Path
		if sys.platform == "darwin":
			godot_path = pathlib.Path().joinpath("Godot.app", "Contents", "MacOS")
		else:
			godot_path = pathlib.Path()
		print(f"install: {godot_path}")
		os.environ["PATH"] += f"{os.pathsep}{godot_path}"
	elif args.type == "export-templates":
		template_path: pathlib.Path
		match sys.platform:
			case "linux":
				template_path = pathlib.Path.home().joinpath(".local", "share", "godot", "export_templates")
			case "darwin":
				template_path = pathlib.Path.home().joinpath("Library", "Application Support", "Godot", "export_templates")
			case "win32":
				template_path = pathlib.Path.home().joinpath("AppData", "Local", "Godot", "export_templates")
			case _:
				sys.exit(f"unknown platform: {sys.platform}")
		print(f"install: {template_path}")
		os.makedirs(template_path)
		shutil.move("templates", template_path.joinpath(f"{godot_version}.stable"))
