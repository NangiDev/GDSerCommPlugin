'''
Install script.
'''

from subprocess import Popen
from os import mkdir
from venv import create
from shutil import copyfile
from shared_lib import is_valid_python_version, try_get_loc, get_loc, Loc
from build import scons_compile, get_platform

is_valid_python_version()

# Git Clone godot-cpp
print("\n===== Cloning godot-cpp =====")
(_, path_exists) = try_get_loc(Loc.GODOTCPP)
if not path_exists:
    GODOT_VER = "4.1"
    GODOT_REPO = "https://github.com/godotengine/godot-cpp"
    CMD = f"git clone -b {GODOT_VER} {GODOT_REPO}"
    with Popen(CMD, shell=True, cwd=get_loc(Loc.ROOT)) as process:
        process.wait()
else:
    print("godot-cpp already cloned")

# Create deps directory if its missing
(deps_path, path_exists) = try_get_loc(Loc.DEPS)
try:
    mkdir(deps_path)
except FileExistsError as e:
    pass

# Git Clone libserialport
print("\n===== Cloning libserialport =====")
(_, path_exists) = try_get_loc(Loc.LIBSERIALPORT)
if not path_exists:
    LIBSER_REPO = "https://github.com/sigrokproject/libserialport.git"
    CMD = f"git clone {LIBSER_REPO}"
    with Popen(CMD, shell=True, cwd=deps_path) as process:
        process.wait()
else:
    print("libserialport already cloned")

# Create python environment
print("\n===== Creating Python virtual environment =====")
(v_env_dir, path_exists) = try_get_loc(Loc.PVENV)
if not path_exists:
    print(f"Creating Python virtual environment. Path: {v_env_dir}")
    create(v_env_dir, with_pip=True)
else:
    print(f"Python virtual environment already exists. Path: {v_env_dir}")

# Update pip
print("\n===== Updating pip =====")
CMD = f"{get_loc(Loc.PYTHON)} -m pip install --upgrade pip"
with Popen(CMD,  shell=True, cwd=get_loc(Loc.ROOT)) as process:
    process.wait()

# Install SCons through pip
print("\n===== Installing Python requirements =====")
CMD = f"{get_loc(Loc.PIP)} install -r requirements.txt"
with Popen(CMD, shell=True, cwd=get_loc(Loc.ROOT)) as process:
    process.wait()

# Compile libserialport library
print("\n===== Compile libserialport =====")
(lib_ser_path, path_exists) = try_get_loc(Loc.LIBSERIALPORT)
MS_BUILD = '"C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Professional\\MSBuild\\Current\\Bin\\msbuild"'
CMD = f"{MS_BUILD} -m libserialport.sln /p:Configuration=Debug /p:Platform=x64"
if not get_platform() == 'windows':
    CMD = f"./autogen.sh && ./configure && make && sudo make install"
with Popen(CMD,  shell=True, cwd=lib_ser_path) as process:
    process.wait()

# Compile Godot bindings
(godot_cpp_path, path_exists) = try_get_loc(Loc.GODOTCPP)
MESSAGE = "Compile Godot bindings"
if not path_exists:
    custom_api_file_path = f"{godot_cpp_path}/gdextension/extension_api.json"
    scons_compile(godot_cpp_path, custom_api_file_path, MESSAGE)
else:
    print(f"\n===== {MESSAGE} =====")
    print("godot-cpp already compiled")

# Compile sercomm plugin
scons_compile(get_loc(Loc.ROOT), message="Compile Sercom")

# Copy sercomm.gdextension file to demo project
print("\n===== Copy gdextension tempplate to demo directory =====")
GDEXTTEMPLFILENAME = "sercomm.gdextension.template"
GDEXTTEMPLFILEPATH = f"{get_loc(Loc.ROOT)}/src/template/{GDEXTTEMPLFILENAME}"
GDEXTFILENAME = "sercomm.gdextension"
GDEXTFILEPATH = f"{get_loc(Loc.ROOT)}/demo/bin/{GDEXTFILENAME}"
copyfile(GDEXTTEMPLFILEPATH, GDEXTFILEPATH)
