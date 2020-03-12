# GDSerCommPlugin
A Godot plugin to read Arduino serial input

# Dependencies
* https://github.com/ingeniamc/sercomm
* https://github.com/GodotNativeTools/godot_headers
* https://github.com/Superwaitsum/GDSercomm

# Installation
## Build binaries
### Windows 10 64-bit
* In Powershell run these commands
```
git clone --depth=1 git@github.com:Superwaitsum/GDSercomm.git
cd .\GDSercomm\
git clone --depth=1 git@github.com:GodotNativeTools/godot_headers.git
git clone --depth=1 git@github.com:ingeniamc/sercomm.git
cd .\sercomm\
cmake -S. -Bbuild
cmake --build build
cd ..
cp .\sercomm\build\config.h .\sercomm\include\public\sercomm\
mkdir lib
cp .\sercomm\build\Debug\sercomm.lib .\lib\
cp .\sercomm\build\Debug\sercomm.dll .\lib\
scons p=platform
```
* Now you should have your .dll in the GDSercomm/bin folder

## Plugin

* Git clone repo into your addons folder in the root of your project.
* Inside the editor got to Project->Projects Settings->Plugins and activate "Serial Communication"

# Screenshot

![Screenshot](Screenshot.png)
