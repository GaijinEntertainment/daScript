options gen2
options indenting = 4
options persistent_heap
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_global_variables = false

require fio
require strings

require lockfile
require utils
require index
require commands

// daspkg — daslang package manager

def print_usage() {
    print("daspkg — daslang package manager\n\n")
    print("Usage: daspkg <command> [args...]\n\n")
    print("Commands:\n")
    print("  install <url|name|path>[@version]  Install a package\n")
    print("  remove <name>                      Remove a package\n")
    print("  update [name]                      Update one or all packages\n")
    print("  list                               List installed packages\n")
    print("  search <query>                     Search the package index\n")
    print("  introduce [url]                    Add a package to the index via PR\n")
    print("  withdraw <name>                    Remove a package from the index via PR\n")
    print("  build                              Build C/C++ packages\n")
    print("  check                              Verify installed packages\n")
    print("  doctor                             Check environment (git, cmake, gh)\n")
    print("\nOptions:\n")
    print("  --root <path>   Project root (default: current directory)\n")
    print("  --force         Force reinstall\n")
}

[export]
def main() {
    var args := get_command_line_arguments()
    let parsed = parse_args(args)

    if (empty(parsed.command)) {
        print_usage()
        return
    }

    let root = get_full_file_name(parsed.root)
    let command = parsed.command

    if (command == "list") {
        cmd_list(root)
    } elif (command == "install") {
        if (empty(parsed.command_arg)) {
            cmd_install_all(root, parsed.force)
        } else {
            cmd_install(root, parsed.command_arg, parsed.force)
        }
    } elif (command == "remove") {
        if (empty(parsed.command_arg)) {
            print("Error: remove requires a package name\n")
        } else {
            cmd_remove(root, parsed.command_arg)
        }
    } elif (command == "update") {
        if (empty(parsed.command_arg)) {
            cmd_update_all(root)
        } else {
            cmd_update(root, parsed.command_arg)
        }
    } elif (command == "build") {
        cmd_build(root)
    } elif (command == "check") {
        cmd_check(root)
    } elif (command == "doctor") {
        cmd_doctor()
    } elif (command == "search") {
        if (empty(parsed.command_arg)) {
            print("Error: search requires a query\n")
        } else {
            cmd_search(root, parsed.command_arg)
        }
    } elif (command == "introduce") {
        cmd_introduce(root, parsed.command_arg)
    } elif (command == "withdraw") {
        if (empty(parsed.command_arg)) {
            print("Error: withdraw requires a package name\n")
        } else {
            cmd_withdraw(root, parsed.command_arg)
        }
    } else {
        print("Unknown command: {command}\n\n")
        print_usage()
    }
}
