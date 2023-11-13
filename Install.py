from subprocess import Popen
import os

RootPath = os.path.dirname(__file__)

# Git Clone godot-cpp
GodotVersion = "4.1"
process = Popen("git clone -b " + GodotVersion + " https://github.com/godotengine/godot-cpp", shell=True, cwd=RootPath)
process.wait()

# Git Clone libserialport
DependenciesPath = RootPath + "\deps"
process = Popen("git clone git://sigrok.org/libserialport", shell=True, cwd=DependenciesPath)
process.wait()

# Install SCons through pip
process = Popen("pip install SCons", shell=True)
process.wait()

# Compile Godot bindings
GodotCppPath = RootPath + "\godot-cpp"
ThreadCount = "16"
CustomApiFilePath = GodotCppPath + "\gdextension\extension_api.json"
Target = "template_release"
Target = "template_debug"
Platform = "macos"
Platform = "linux"
Platform = "windows"
process = Popen("scons platform=" + Platform + " target=" + Target + " -j" + ThreadCount + " custom_api_file=" + CustomApiFilePath, shell=True, cwd=GodotCppPath)
process.wait()

if (process.returncode != 0):
    print("Compiling Godot bindings failed!")
    exit()

# Compile our plugin
process = Popen("scons platform=" + Platform + " target=" + Target, shell=True, cwd=RootPath)
process.wait()

if (process.returncode != 0):
    print("Compiling our plugin failed!")
    exit()