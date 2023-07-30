import hashlib
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
engine_filename = f"{godot_name}_{godot_edition}.zip"
engine_url = f"{base_url}/{engine_filename}"
template_filename = f"{godot_name}_export_templates.tpz"
template_url = f"{base_url}/{template_filename}"
sums_url = f"{base_url}/SHA512-SUMS.txt"


def download_file(source: str, target: str):
	print(f"{source} => {target}")
	if os.path.isfile(target):
		return
	with requests.get(source, stream=True) as response:
		with open(target, "wb") as file:
			for chunk in response.iter_content(chunk_size=8192):
				file.write(chunk)


def download_str(source: str):
	with requests.get(source) as response:
		return response.text


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


hashes: dict[str, str] = {}
for line in download_str(sums_url).splitlines():
	parts = line.split("  ")
	hashes[parts[1]] = parts[0]


def verify(filename: str, name: str):
	print(f"{name} ({filename})")
	sha512 = hashlib.sha512()
	with open(filename, "rb") as file:
		while True:
			data = file.read(65536)  # 64 kb
			if not data:
				break
			sha512.update(data)
	file_hash = sha512.hexdigest()
	if not file_hash == hashes[name]:
		sys.exit(f"error: invalid checksum: {name} ({file_hash})")


download_file(engine_url, "godot.zip")
extract("godot.zip", "godot")
verify("godot.zip", engine_filename)

download_file(template_url, "templates.zip")
extract("templates.zip", "templates")
verify("templates.zip", template_filename)

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
	"--headless"
	"--path", "game",
	"--export-release", godot_preset, "."
])

for line in godot_build:
	print(line)
