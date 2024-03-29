#!/usr/bin/env python
import os
import sys
import platform

env = SConscript("godot-cpp/SConstruct")

# For reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags

# tweak this if you want to use different folders, or more folders, to store your source code in.
env.Tool('compilation_db')
env.Append(CPPPATH=['src/', 'deps/libserialport/'])
env.Append(LIBPATH=['deps/libserialport/x64/Debug/'])
env.Append(LIBS=['libserialport'])

# Define FMT_HEADER_ONLY
env.Append(CPPDEFINES=['FMT_HEADER_ONLY'])

if platform.system() == 'Windows':
    env['CXXFLAGS'].remove('/std:c++17')
    env.Append(CXXFLAGS=['/std:c++20'])
else:
    env['CXXFLAGS'].remove('-std=c++17')
    env.Append(CXXFLAGS=['-std=c++20'])

sources = Glob("src/*.cpp")

if env["platform"] == "macos":
    library = env.SharedLibrary(
        "demo/bin/libsercomm.{}.{}.framework/libsercomm.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources,
    )
else:
    library = env.SharedLibrary(
        "demo/bin/libsercomm{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

cc = env.CompilationDatabase('compile_commands.json')

Default(cc, library)
