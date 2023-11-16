from sys import version_info

ExpectedMajorVersion = 3
ExpectedMinorVersion = 8
CurrentMajorVersion = version_info.major
CurrentMinorVersion = version_info.minor

MajorIsLower = CurrentMajorVersion < ExpectedMajorVersion
MajorIsGreaterButMinorIsLower = not MajorIsLower and CurrentMinorVersion < ExpectedMinorVersion

if MajorIsLower or MajorIsGreaterButMinorIsLower:
    print("    Python {}.{} or higher is required.".format(ExpectedMajorVersion, ExpectedMinorVersion))
    print("    You are using Python {}.{}.".format(CurrentMajorVersion, CurrentMinorVersion))
    exit(1)

from subprocess import Popen
from os import path, mkdir
from venv import create
from platform import system
from shutil import copyfile

RootPath = path.dirname(__file__)

# Git Clone godot-cpp
print("\n===== Cloning godot-cpp =====")
if not path.exists("{}/godot-cpp".format(RootPath)):
    GodotVersion = "4.1"
    process = Popen("git clone -b {} https://github.com/godotengine/godot-cpp".format(GodotVersion), shell=True, cwd=RootPath)
    process.wait()
else:
    print("godot-cpp already cloned")

# Git Clone libserialport
print("\n===== Cloning libserialport =====")
DependenciesPath = "{}/deps".format(RootPath)
try:
    mkdir(DependenciesPath)
except FileExistsError as e:
    pass
if not path.exists("{}/libserialport".format(DependenciesPath)):
    process = Popen("git clone https://github.com/sigrokproject/libserialport.git", shell=True, cwd=DependenciesPath)
    process.wait()
else:
    print("libserialport already cloned")

#Create python environment
print("\n===== Creating Python virtual environment =====")
VEnvDir = "{}/pvenv".format(RootPath)
if not path.exists(VEnvDir):
    print("Creating Python virtual environment. Path: {}".format(VEnvDir))
    create(VEnvDir, with_pip=True)
else:
    print("Python virtual environment already exists. Path: {}".format(VEnvDir))

# Python venv executables

# On Linux the folder is called bin and on Windows it's called Scripts
VEnvDirBin = "{}/bin".format(VEnvDir)
if not path.exists(VEnvDirBin):
    VEnvDirBin = "{}/Scripts".format(VEnvDir)

PythonExec = "{}/python".format(VEnvDirBin)
PipExec = "{}/pip".format(VEnvDirBin)

# Update pip
print("\n===== Updating pip =====")
process = Popen("{} -m pip install --upgrade pip".format(PythonExec), shell=True, cwd=RootPath)
process.wait()

# Install SCons through pip
print("\n===== Installing Python requirements =====")
process = Popen("{} install -r requirements.txt".format(PipExec), shell=True, cwd=RootPath)
process.wait()

# Scons arguments
ThreadCount = "16"
Target = "template_release"
Target = "template_debug"

Platform = "windows"
match system():
    case 'Linux':
        Platform = "linux"
    case 'Darwin':
        Platform = "macos"
    case 'Windows':
        Platform = "windows"
    case other:
        print("Unsupported platform: {}".format(system()))
        exit(1)

# Python venv Scons executables
SconsExec = "{}/scons".format(VEnvDirBin)
SconsCommand = "{} platform={} target={} -j{}".format(SconsExec, Platform, Target, ThreadCount)

# Compile Godot bindings
print("\n===== Compile Godot bindings =====")
GodotCppPath = "{}/godot-cpp".format(RootPath)
if not path.exists("{}/bin".format(GodotCppPath)):
    CustomApiFilePath = "{}/gdextension/extension_api.json".format(GodotCppPath)
    process = Popen("{} custom_api_file={}".format(SconsCommand, CustomApiFilePath), shell=True, cwd=GodotCppPath)
    process.wait()

    if (process.returncode != 0):
        print("Compiling Godot bindings failed!")
        exit(1)
else:
    print("godot-cpp already compiled")

# Compile sercomm plugin
print("\n===== Compile sercomm plugin =====")
process = Popen(SconsCommand, shell=True, cwd=RootPath)
process.wait()

if (process.returncode != 0):
    print("Compiling sercomm plugin failed!")
    exit(1)

# Copy sercomm.gdextension file to demo project
print("\n===== Copy gdextension tempplate to demo directory =====")
GdExtTemplFileName = "sercomm.gdextension.template"
GdExtTemplFilePath = "{}/src/template/{}".format(RootPath, GdExtTemplFileName)
GdExtFileName = "sercomm.gdextension"
GdExtFilePath = "{}/demo/bin/{}".format(RootPath, GdExtFileName)
copyfile(GdExtTemplFilePath, GdExtFilePath)