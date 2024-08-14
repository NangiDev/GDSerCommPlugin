'''
Build script.
'''

import sys
import subprocess
import platform
from shared_lib import get_loc, Loc, is_valid_python_version

is_valid_python_version()


def get_platform():
    '''
    Returns the name the current platform, translated into SCons terms.
    Ex. 'Linux = linux', 'Darwin = macos', 'Windows = windows'

    Will exit, with return code 1, if unsupported platform

    Returns:
    string: Return SCons terms for current platform
    '''
    match platform.system():
        case 'Linux':
            return "linux"
        case 'Darwin':
            return "macos"
        case 'Windows':
            return "windows"
        case other:
            print(f"Unsupported platform: {other}")
            sys.exit(1)


_THREADCOUNT = "16"
_TARGET = "template_release"
_TARGET = "template_debug"
_scons_cmd = f"\"{get_loc(Loc.SCONS)}\" platform={get_platform()} target={_TARGET} -j{_THREADCOUNT}"


def scons_compile(working_dir, custom_api_file_path=None, message="Compile plugin"):
    '''
    Will try compile SCons project.
    Will exit, with return code 1, if compile fails

    Parameters:
    working_dir (string): Working directory of where to run the compilation from
    custom_api_file_path (string): Path to custom api file if needed. Optional
    message (string): Message to output before comilation. For logging purposes. Optional
    '''
    print(f"\n===== {message} =====")

    cmd = _scons_cmd + \
        (f" custom_api_file={custom_api_file_path}" if custom_api_file_path else "")
    with subprocess.Popen(cmd, shell=True, cwd=working_dir) as process:
        process.wait()

        if process.returncode != 0:
            print("Compiling failed!")
            sys.exit(1)


if __name__ == "__main__":
    scons_compile(get_loc(Loc.ROOT))
