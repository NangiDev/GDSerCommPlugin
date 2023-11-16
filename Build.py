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
from os import path
from platform import system

RootPath = path.dirname(__file__)

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
VEnvDir = "{}/pvenv".format(RootPath)

# On Linux the folder is called bin and on Windows it's called Scripts
VEnvDirBin = "{}/bin".format(VEnvDir)
if not path.exists(VEnvDirBin):
    VEnvDirBin = "{}/Scripts".format(VEnvDir)

SconsExec = "{}/scons".format(VEnvDirBin)
SconsCommand = "{} platform={} target={} -j{}".format(SconsExec, Platform, Target, ThreadCount)

# Compile sercomm plugin
print("\n===== Compile sercomm plugin =====")
process = Popen(SconsCommand, shell=True, cwd=RootPath)
process.wait()

if (process.returncode != 0):
    print("Compiling sercomm plugin failed!")
    exit(1)