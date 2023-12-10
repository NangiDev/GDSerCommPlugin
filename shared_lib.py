'''
Shared functionality between install and build steps.
'''

import sys
import enum
import os
import platform


def is_valid_python_version():
    '''
    Exit early, with return code 1, if python version is invalid.
    '''
    exp_major_ver = 3
    exp_minor_ver = 8
    cur_major_ver = sys.version_info.major
    cur_minor_ver = sys.version_info.minor

    major_is_lower = cur_major_ver < exp_major_ver
    major_is_greater_but_minor_is_lower = not major_is_lower and cur_minor_ver < exp_minor_ver

    if major_is_lower or major_is_greater_but_minor_is_lower:
        print(
            f"    Python {exp_major_ver}.{exp_minor_ver} or higher is required.")
        print(
            f"    You are using Python {cur_major_ver}.{cur_minor_ver}.")
        sys.exit(1)


def name_of_bin_file():
    '''
    Returns the name of the Python venv bin directory. It differs from system to system.
    Bin folder in Windows is called 'Scripts' instead of 'bin'

    Returns:
    string: Name of Python venv bin directory
    '''
    return "Scripts" if platform.system() == 'Windows' else "bin"


class Loc(enum.Enum):
    '''
    Enums used as keys for local dictionary (LocDict).
    '''
    ROOT = 1
    PVENV = 2
    GODOTCPP = 3
    DEPS = 4
    LIBSERIALPORT = 5
    BIN = 6
    SCONS = 7
    PYTHON = 8
    PIP = 9
    GTEST = 10


_root_path = os.path.dirname(__file__)
_pvenv_path = os.path.join(_root_path, "pvenv")
_godot_cpp_path = os.path.join(_root_path, "godot-cpp")
_deps_path = os.path.join(_root_path, "deps")
_lib_ser_path = os.path.join(_deps_path, "libserialport")
_g_test_path = os.path.join(_deps_path, "googletest")
_bin_path = os.path.join(_pvenv_path, name_of_bin_file())

LocDict = {
    Loc.ROOT:             _root_path,
    Loc.PVENV:            _pvenv_path,
    Loc.GODOTCPP:         _godot_cpp_path,
    Loc.DEPS:             _deps_path,
    Loc.LIBSERIALPORT:    _lib_ser_path,
    Loc.GTEST:            _g_test_path,
    Loc.BIN:              _bin_path,
    Loc.SCONS:            os.path.join(_bin_path, "scons"),
    Loc.PYTHON:           os.path.join(_bin_path, "python"),
    Loc.PIP:              os.path.join(_bin_path, "pip"),
}


def get_loc(loc):
    '''
    Get the path of Loc enum.

    Parameters:
    loc (Loc(Enum)): Loc enum. Ex Loc.ROOT

    Returns:
    string: Path to wanted directory/file
    '''
    return LocDict[loc]


def try_get_loc(loc):
    '''
    Will check if path exists as well as getting the path of parameter enum.

    Parameters:
    loc (Loc(Enum)): Loc enum. Ex Loc.ROOT

    Returns:
    (string, bool): Return pair (path, path_exists) with path requested and if it exists or not
    '''
    p = LocDict[loc]
    return (p, os.path.exists(p))
