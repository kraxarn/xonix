import os
import subprocess
import sys
import typing
import zipfile

import requests

if ("BUILD_TYPE" not in os.environ
	or "GODOT_VERSION" not in os.environ
	or "GODOT_EDITION" not in os.environ
	or "GODOT_PRESET" not in os.environ):
	sys.exit("error: required environmental variables not set")

build_type = os.environ["BUILD_TYPE"]
godot_version = os.environ["GODOT_VERSION"]
godot_edition = os.environ["GODOT_EDITION"]
godot_preset = os.environ["GODOT_PRESET"]

base_url = f"https://downloads.tuxfamily.org/godotengine/{godot_version}"
godot_name = f"Godot_v{godot_version}-stable"
engine_url = f"{base_url}/{godot_name}_{godot_edition}.zip"
template_url = f"{base_url}/{godot_name}_export_templates.tpz"


def download(source: str, target: str):
	print(f"{source} => {target}")
	if os.path.isfile(target):
		return
	with requests.get(source, stream=True) as response:
		with open(target, "wb") as file:
			for chunk in response.iter_content(chunk_size=8192):
				file.write(chunk)


def extract(source: str, target: str):
	print(f"{source} => {target}")
	if os.path.isdir(target):
		return
	with zipfile.ZipFile(source, "r") as zip_file:
		zip_file.extractall(target)


def run(args: typing.Sequence[str]) -> typing.Iterator[str]:
	with subprocess.Popen(args, stdout=subprocess.PIPE) as process:
		for line in process.stdout:
			yield line.decode()


# TODO: Verify hashes

download(engine_url, "godot.zip")
extract("godot.zip", "godot")

download(template_url, "templates.zip")
extract("templates.zip", "templates")

godot_path: str
godot_os = godot_edition[:godot_edition.index(".")]
match godot_os:
	case "linux":
		godot_path = f"godot/{godot_name}_linux.x86_64"
		subprocess.run(["chmod", "+x", godot_path])
	case "macos":
		godot_path = "godot/Godot.app/Contents/MacOS/Godot"
		subprocess.run(["chmod", "+x", godot_path])
	case "win64":
		godot_path = f"/godot/{godot_name}_win64.exe/{godot_name}_win64.exe"
	case _:
		sys.exit(f"error: unknown os: {godot_os}")

godot_exec_version = run([godot_path, "--version"])
print(f"Godot {next(godot_exec_version)}")

cmake_configure = run([
	"cmake", ".",
	f"-DCMAKE_BUILD_TYPE={build_type}",
	"-DCMAKE_INSTALL_PREFIX=/usr",
])

for line in cmake_configure:
	print(line)

cmake_build = run([
	"cmake",
	"--build", ".",
	"--config", build_type,
])

for line in cmake_build:
	print(line)

godot_build = run([
	godot_path,
	"--debug",
	"--export-release", godot_preset, "game"
])
