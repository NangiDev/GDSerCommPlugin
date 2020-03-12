# GDSerCommPlugin
A Godot plugin to read Arduino serial input

## It is heavily based and inspired by https://github.com/Superwaitsum/GDSercomm

# Installation
## Build binaries
### Windows 10 64-bit
* In Powershell run these commands

```
git clone --depth=1 git@github.com:ingeniamc/sercomm.git
cd .\sercomm\
cmake -S. -Bbuild
cmake --build build
```

## Plugin

* Git clone repo into your addons folder in the root of your project.
* Inside the editor got to Project->Projects Settings->Plugins and activate "Serial Communication"

# Screenshot

![Screenshot](Screenshot.png)
