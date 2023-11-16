from Shared import Loc, GetLoc, IsValidPythonVersion
from subprocess import Popen
from platform import system

IsValidPythonVersion()

def GetPlatform():
    match system():
        case 'Linux':
            return "linux"
        case 'Darwin':
            return "macos"
        case 'Windows':
            return "windows"
        case other:
            print("Unsupported platform: {}".format(system()))
            exit(1)

# Scons arguments
ThreadCount = "16"
Target = "template_release"
Target = "template_debug"
Platform = GetPlatform()

#Scons command
SconsCommand = f"{GetLoc(Loc.scons)} platform={Platform} target={Target} -j{ThreadCount}"

def Compile(WorkingDir, CustomApiFilePath = None, Message = "Compile plugin"):
    print(f"\n===== {Message} =====")

    cmd = SconsCommand + (f" custom_api_file={CustomApiFilePath}" if CustomApiFilePath else "")
    process = Popen(cmd, shell=True, cwd=WorkingDir)
    process.wait()

    if (process.returncode != 0):
        print("Compiling failed!")
        exit(1)

if __name__ == "__main__":
    Compile(GetLoc(Loc.root))