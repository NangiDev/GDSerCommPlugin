from subprocess import Popen
from os import path

RootPath = path.dirname(__file__)

# Git Clone godot-cpp
if not path.exists(RootPath + "/godot-cpp"):
    GodotVersion = "4.1"
    process = Popen("git clone -b " + GodotVersion + " https://github.com/godotengine/godot-cpp", shell=True, cwd=RootPath)
    process.wait()
else:
    print("godot-cpp already cloned")

# Git Clone libserialport
DependenciesPath = RootPath + "\deps"
if not path.exists(DependenciesPath + "/libserialport"):
    process = Popen("git clone git://sigrok.org/libserialport", shell=True, cwd=DependenciesPath)
    process.wait()
else:
    print("libserialport already cloned")

# Install SCons through pip
process = Popen("pip install SCons", shell=True)
process.wait()

# Scons arguments
ThreadCount = "16"
Target = "template_release"
Target = "template_debug"
Platform = "macos"
Platform = "linux"
Platform = "windows"

SconsCom = "scons platform=" + Platform + " target=" + Target + " -j" + ThreadCount

# Compile Godot bindings
GodotCppPath = RootPath + "\godot-cpp"
if not path.exists(GodotCppPath + "/bin"):
    CustomApiFilePath = GodotCppPath + "\gdextension\extension_api.json"
    process = Popen(SconsCom + " custom_api_file=" + CustomApiFilePath, shell=True, cwd=GodotCppPath)
    process.wait()

    if (process.returncode != 0):
        print("Compiling Godot bindings failed!")
        exit()
else:
    print("godot-cpp already compiled")

# Compile our plugin
process = Popen(SconsCom, shell=True, cwd=RootPath)
process.wait()

if (process.returncode != 0):
    print("Compiling our plugin failed!")
    exit()