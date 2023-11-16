from Shared import IsValidPythonVersion, TryGetLoc, GetLoc, Loc
from Build import GetPlatform, SconsCommand, Compile
from subprocess import Popen
from os import mkdir, path
from venv import create
from platform import system
from shutil import copyfile

IsValidPythonVersion()

# Git Clone godot-cpp
print("\n===== Cloning godot-cpp =====")
(_, PathExists) = TryGetLoc(Loc.godotcpp)
if not PathExists:
    GodotVersion = "4.1"
    process = Popen(f"git clone -b {GodotVersion} https://github.com/godotengine/godot-cpp", shell=True, cwd=GetLoc(Loc.root))
    process.wait()
else:
    print("godot-cpp already cloned")

# Create deps directory if its missing
(DepsPath, PathExists) = TryGetLoc(Loc.deps)
try: mkdir(DepsPath)
except FileExistsError as e: pass

print("\n===== Cloning libserialport =====")
# Git Clone libserialport
(_, PathExists) = TryGetLoc(Loc.libserialport)
if not PathExists:
    process = Popen("git clone https://github.com/sigrokproject/libserialport.git", shell=True, cwd=DepsPath)
    process.wait()
else:
    print("libserialport already cloned")

#Create python environment
print("\n===== Creating Python virtual environment =====")
(VEnvDir, PathExists) = TryGetLoc(Loc.pvenv)
if not PathExists:
    print(f"Creating Python virtual environment. Path: {VEnvDir}")
    create(VEnvDir, with_pip=True)
else:
    print(f"Python virtual environment already exists. Path: {VEnvDir}")

# Update pip
print("\n===== Updating pip =====")
process = Popen(f"{GetLoc(Loc.python)} -m pip install --upgrade pip", shell=True, cwd=GetLoc(Loc.root))
process.wait()

# Install SCons through pip
print("\n===== Installing Python requirements =====")
process = Popen(f"{GetLoc(Loc.pip)} install -r requirements.txt", shell=True, cwd=GetLoc(Loc.root))
process.wait()

# Compile Godot bindings
(GodotCppPath, PathExists) = TryGetLoc(Loc.godotcpp)
if not PathExists:
    CustomApiFilePath = f"{GodotCppPath}/gdextension/extension_api.json"
    Compile(GodotCppPath, CustomApiFilePath, "Compile Godot bindings")
else:
    print("\n===== Compile Godot bindings =====")
    print("godot-cpp already compiled")

# Compile sercomm plugin
Compile(GetLoc(Loc.root), Message="Compile Sercom")

# Copy sercomm.gdextension file to demo project
print("\n===== Copy gdextension tempplate to demo directory =====")
GdExtTemplFileName = "sercomm.gdextension.template"
GdExtTemplFilePath = f"{GetLoc(Loc.root)}/src/template/{GdExtTemplFileName}"
GdExtFileName = "sercomm.gdextension"
GdExtFilePath = f"{GetLoc(Loc.root)}/demo/bin/{GdExtFileName}"
copyfile(GdExtTemplFilePath, GdExtFilePath)