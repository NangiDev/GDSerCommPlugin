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

RootPath = path.dirname(__file__)

# Git Clone godot-cpp
if not path.exists("{}/godot-cpp".format(RootPath)):
    GodotVersion = "4.1"
    process = Popen("git clone -b {} https://github.com/godotengine/godot-cpp".format(GodotVersion), shell=True, cwd=RootPath)
    process.wait()
else:
    print("godot-cpp already cloned")

# Git Clone libserialport
DependenciesPath = "{}/deps".format(RootPath)
try:
    mkdir(DependenciesPath)
except FileExistsError as e:
    print("{} already exists.".format(DependenciesPath))
if not path.exists("{}/libserialport".format(DependenciesPath)):
    process = Popen("git clone https://github.com/sigrokproject/libserialport.git", shell=True, cwd=DependenciesPath)
    process.wait()
else:
    print("libserialport already cloned")

#Create python environment
VEnvDir = "{}/pvenv".format(RootPath)
if not path.exists(VEnvDir):
    print("Creating Python virtual environment. Path: {}".format(VEnvDir))
    create(VEnvDir, with_pip=True)
else:
    print("Python virtual environment already exists. Path: {}".format(VEnvDir))

# Python venv executables
PythonExec = "{}/Scripts/python".format(VEnvDir)
PipExec = "{}/Scripts/pip".format(VEnvDir)

# Update pip
process = Popen("{} -m pip install --upgrade pip".format(PythonExec), shell=True, cwd=RootPath)
process.wait()

# Install SCons through pip
process = Popen("{} install -r requirements.txt".format(PipExec), shell=True, cwd=RootPath)
process.wait()

# Scons arguments
ThreadCount = "16"
Target = "template_release"
Target = "template_debug"
Platform = "macos"
Platform = "linux"
Platform = "windows"

# Python venv Scons executables
SconsExec = "{}/Scripts/scons".format(VEnvDir)
SconsCommand = "{} platform={} target={} -j{}".format(SconsExec, Platform, Target, ThreadCount)

# Compile Godot bindings
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

exit()

# Compile our plugin
process = Popen(SconsCommand, shell=True, cwd=RootPath)
process.wait()

if (process.returncode != 0):
    print("Compiling our plugin failed!")
    exit(1)
