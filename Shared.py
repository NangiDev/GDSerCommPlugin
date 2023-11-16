from sys import version_info
from enum import Enum
from os import path
from platform import system

# Exits if python version is invalid
def IsValidPythonVersion():
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

# Bin folder in Windows is called Scripts instead of bin
def NameOfBinFile(): return "Scripts" if system() == 'Windows' else "bin"
RootPath = path.dirname(__file__)
PvenvPath = path.join(RootPath, "pvenv")
GodotCppPath = path.join(RootPath, "godot-cpp")
DepsPath = path.join(RootPath, "deps")
LibSerPath = path.join(DepsPath, "libserialport")
GTestPath = path.join(DepsPath, "googletest")
BinPath = path.join(PvenvPath, NameOfBinFile())

class Loc(Enum):
    root = 1
    pvenv = 2
    godotcpp = 3
    deps = 4
    libserialport = 5
    bin = 6
    scons = 7
    python = 8
    pip = 9
    gtest = 10

LocDict = {
  Loc.root:             RootPath,
  Loc.pvenv:            PvenvPath,
  Loc.godotcpp:         GodotCppPath,
  Loc.deps:             DepsPath,
  Loc.libserialport:    LibSerPath,
  Loc.gtest:            GTestPath,
  Loc.bin:              BinPath,
  Loc.scons:            path.join(BinPath, "scons"),
  Loc.python:           path.join(BinPath, "python"),
  Loc.pip:              path.join(BinPath, "pip"),
}

def GetLoc(loc):
    return LocDict[loc]
    
def TryGetLoc(loc):
    p = LocDict[loc]
    return (p, path.exists(p))