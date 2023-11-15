# This branch does not work yet!

# SerCommPlugin
A Godot 4.1.3+ plugin to read/write Arduino serial input. \
Read
[GDExtension](https://docs.godotengine.org/en/stable/tutorials/scripting/gdextension/index.html)
for more

# Dependencies
* [Godot 4.1 or above](https://github.com/godotengine/godot/releases/tag/4.1.3-stable)
* [godot-cpp](https://github.com/godotengine/godot-cpp)
* [libserialport](https://github.com/sigrokproject/libserialport)

# Build system
* [SCons](https://scons.org/)
* [Python 3.8 or above](https://www.python.org/downloads/)
* [Git](https://git-scm.com/)

# Repo structure
    src - Plugin source code
    deps - Dependencies, like libserialport. Will be created on install\
    godot-cpp - Godot bindings. Will be created on install\

# Installation
### Windows
```
python Install.py
```

### Ubuntu
```
python Install.py
```
### OSX
```
python Install.py
```

## Plugin