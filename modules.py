import os
import sys
import subprocess
import argparse
from typing import List, Dict


options_table = {
    "dasBGFX":        "DAS_BGFX_DISABLED",
    "dasClangBind":   "DAS_CLANG_BIND_DISABLED",
    "dasGlfw":        "DAS_GLFW_DISABLED",
    "dasGlsl":        "",
    "dasHV":          "DAS_HV_DISABLED",
    "dasImgui":       "DAS_IMGUI_DISABLED",
    "dasLLVM":        "DAS_LLVM_DISABLED",
    "dasMinfft":      "DAS_MINFFT_DISABLED",
    "dasOpenGL":      "",
    "dasQuirrel":     "DAS_QUIRREL_DISABLED",
    "dasSFML":        "DAS_SFML_DISABLED",
    "dasAudio":       "DAS_AUDIO_DISABLED",
    "dasStbImage":    "DAS_STBIMAGE_DISABLED",
    "dasStbTrueType": "DAS_STBTRUETYPE_DISABLED",
    "dasStdDlg":      "DAS_STDDLG_DISABLED",
    "dasXbyak":       "DAS_XBYAK_DISABLED"
}

cmake_options_cache: Dict[str, str] = {}


def goto_build_directory() -> str:
    oldpath = os.getcwd()

    try:
        os.chdir(cmake_path)
    except FileNotFoundError:
        print("ERROR: cmake build directory cannot be found, maybe it was deleted or <cmake build path> was entered incorrectly.")
        exit(1)

    try:
        if not os.path.exists("CMakeCache.txt"):
            raise FileNotFoundError
    except FileNotFoundError:
        print("ERROR: CMakeCache.txt cannot be found, maybe it was moved or deleted.")
        exit(1)

    return oldpath


def goback_to_directory(oldpath: str):
    try:
        os.chdir(oldpath)
    except FileNotFoundError:
        print("ERROR: can't go back to %s, maybe project directory was renamed" % oldpath)
        exit(1)


def get_all_submodule_names() -> List[str]:
    all_submodules_path = os.path.join(os.getcwd(), "modules")
    dir_elems = os.listdir(all_submodules_path)

    submodules_list = []
    for elem in dir_elems:
        if os.path.isdir(os.path.join(all_submodules_path, elem)):
            submodules_list.append(elem)

    return submodules_list


def fill_cmake_options_cache(all_modules: List[str]):
    oldpath = goto_build_directory()

    cmake_output = subprocess.run(["cmake", "-N", "-LA", "."], stdout=subprocess.PIPE, encoding='utf-8')
    if cmake_output.stderr is not None:
        print("ERROR: there was an error executing 'cmake -N -LA .', stderr:")
        print(cmake_output.stderr)
        exit(1)

    cache_list = cmake_output.stdout
    goback_to_directory(oldpath)

    for module in all_modules:

        option_name = options_table.setdefault(module, "")

        if option_name == "":
            cmake_options_cache[module] = " "
            continue

        for cache_str in cache_list.split("\n"):
            if cache_str.find(option_name) != -1:
                status = cache_str[cache_str.find("=") + 1:]
                if status == "ON":
                    cmake_options_cache[module] = "-"
                elif status == "OFF":
                    cmake_options_cache[module] = "+"
                break


def print_all_option_values(all_modules: List[str], selected_module: str):
    if not cmake_options_cache or len(all_modules) != len(cmake_options_cache):
        fill_cmake_options_cache(all_modules)

    for module, sign in cmake_options_cache.items():
        output = ""

        if module == selected_module:
            output += "-> "
        else:
            output += "   "

        output += sign + " : " + module

        if sign == " ":
            output += " - option for this module is unknown"

        print(output)


def able_selected_submodule(submodule_name: str):
    option_name = options_table[submodule_name]

    submodule_path = os.path.join(os.getcwd(), "modules", submodule_name)

    if not os.listdir(submodule_path):
        if subprocess.call(["git", "submodule", "update", "--init", "--recursive", "modules/" + submodule_name]) == 0:
            print("%s cloned successfully!" % submodule_name)
        else:
            print("ERROR: there was an error executing 'git submodule update --init --recursive' for %s submodule." % submodule_name)
            return

    oldpath = goto_build_directory()

    if subprocess.call(["cmake", "-D" + option_name + ":BOOL=OFF ", oldpath]) == 0:
        print("%s turned on successfully (see log above)!" % submodule_name)
    else:
        print("ERROR: there was an error executing 'cmake -D%s:BOOL=OFF %s' " % (option_name, oldpath))

    goback_to_directory(oldpath)


def disable_selected_submodule(submodule_name: str):
    option_name = options_table[submodule_name]

    oldpath = goto_build_directory()

    if subprocess.call(["cmake", "-D" + option_name + ":BOOL=ON ", oldpath]) == 0:
        print("%s turned off successfully (see log above)!" % submodule_name)
    else:
        print("ERROR: there was an error executing 'cmake -D%s:BOOL=ON %s' " % (option_name, oldpath))

    goback_to_directory(oldpath)


def disable_all_submodules():
    for module, sign in cmake_options_cache.items():
        if sign == "+":
            disable_selected_submodule(module)

    all_submodules = get_all_submodule_names()
    fill_cmake_options_cache(all_submodules)


def change_submodule_availability(submodule: str):
    status = cmake_options_cache[submodule]

    if status == "+":
        disable_selected_submodule(submodule)
    elif status == "-":
        able_selected_submodule(submodule)
    else:
        print("Option with this name (%s) doesn't exist." % submodule)

    all_submodules = get_all_submodule_names()
    fill_cmake_options_cache(all_submodules)


def cmd_submodule_configuration(on_list: List[str], off_list: List[str], all_modules: List[str]):
    if on_list is not None:
        for on_sub in on_list:
            if on_sub not in all_modules:
                print("Submodule with this name (%s) doesn't exist." % on_sub)
                continue
            able_selected_submodule(on_sub)

    if off_list is not None:
        for off_sub in off_list:
            if off_sub not in all_modules:
                print("Submodule with this name (%s) doesn't exist." % off_sub)
                continue
            disable_selected_submodule(off_sub)


class _Getch:
    """Gets a single character from standard input.  Does not echo to the
screen."""
    def __init__(self):
        try:
            self.impl = _GetchWindows()
        except ImportError:
            self.impl = _GetchUnix()

    def __call__(self): return self.impl()


class _GetchUnix:
    def __init__(self):
        import tty, sys

    def __call__(self):
        import sys, tty, termios
        fd = sys.stdin.fileno()
        old_settings = termios.tcgetattr(fd)
        try:
            tty.setraw(sys.stdin.fileno())
            ch = sys.stdin.read(1)
        finally:
            termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
        return ch


class _GetchWindows:
    def __init__(self):
        import msvcrt

    def __call__(self):
        import msvcrt
        ch = msvcrt.getch()
        if ch == '\000' or ch == '\xe0':
            return msvcrt.getch()
        return ch


getch = _Getch()


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description='''This is cmake/git configuration tool.
        It allows to manage cmake submodules options and turn them on (in case they were off)
        or turn off (in case they were on). Tool also clones the submodule while it turns it on,
        if it hasn't been cloned yet.
        example: D:\daScript> python3 modules.py --path <cmake_build_folder> --on <submodule_name> --off <submodule_name>''')

    parser.add_argument(
        "--path", required=True,
        # metavar="<cmake build path>" ,
        help='''relative path to build directory processed by cmake''')

    parser.add_argument (
        "--on", nargs='+',
        help='''list of submodules for turning on''')

    parser.add_argument (
        "--off", nargs='+',
        help='''list of submodules for turning off''')

    if len(sys.argv) == 1:
        parser.print_help()
        exit(0)

    args = parser.parse_args()
    cmake_path = args.path

    submodules_list = get_all_submodule_names()
    selected_submodule_ind = 0

    if args.on is not None or args.off is not None:
        cmd_submodule_configuration(args.on, args.off, submodules_list)
        print("\nPress any key to continue ...")
        getch()

    is_break_out = False
    while not is_break_out:
        os.system('cls' if os.name=='nt' else 'clear')

        print("Q - exit, U - update list, D - disable all modules, arrows/J/K - navigation, enter - toggle module state")
        print_all_option_values(submodules_list, submodules_list[selected_submodule_ind])

        c = getch()

        is_up = ord(c) == 72 if os.name=='nt' else ord(c) == 65
        is_down = ord(c) == 80 if os.name=='nt' else ord(c) == 66

        if ord(c) == 113: # q
            is_break_out = True
            break

        elif ord(c) == 117: # u
            submodules_list = get_all_submodule_names()
            fill_cmake_options_cache(submodules_list)

        elif ord(c) == 100: # d
            disable_all_submodules()
            print("\nPress any key to continue ...")
            getch()

        elif selected_submodule_ind > 0 and (is_up or ord(c) == 107): # up: pgup or k
            selected_submodule_ind -= 1

        elif selected_submodule_ind < len(submodules_list) - 1 and (is_down or ord(c) == 106): # down: pgdn or j
            selected_submodule_ind += 1

        elif ord(c) == 13: # enter
            change_submodule_availability(submodules_list[selected_submodule_ind])
            print("\nPress any key to continue ...")
            getch()
