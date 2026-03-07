options gen2
options indenting = 4
options persistent_heap
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_global_variables = false

require fio
require strings

require lockfile
require commands

// daspkg — daslang package manager
//
// Usage: daslang utils/daspkg/main.das -- <command> [args...]

def print_usage() {
    print("daspkg — daslang package manager\n\n")
    print("Usage: daslang utils/daspkg/main.das -- <command> [args...]\n\n")
    print("Commands:\n")
    print("  install <url|name|path>[@version]  Install a package\n")
    print("  remove <name>                      Remove a package\n")
    print("  update [name]                      Update one or all packages\n")
    print("  list                               List installed packages\n")
    print("  search <query>                     Search the package index\n")
    print("  build                              Build C++ packages\n")
    print("  check                              Verify SDK compatibility\n")
    print("\nOptions:\n")
    print("  --root <path>   Project root (default: current directory)\n")
    print("  --force         Force reinstall\n")
}

struct ParsedArgs {
    root : string
    command : string
    command_arg : string
    force : bool
}

def parse_args(args : array<string>) : ParsedArgs {
    var result = ParsedArgs(root = ".")
    // args: [exe, script.das, --, command, ...]
    var i = 0
    while (i < length(args)) {
        if (args[i] == "--") {
            i++
            break
        }
        i++
    }
    while (i < length(args)) {
        if (args[i] == "--root" && i + 1 < length(args)) {
            i++
            result.root := args[i]
        } elif (args[i] == "--force") {
            result.force = true
        } elif (empty(result.command)) {
            result.command := args[i]
        } elif (empty(result.command_arg)) {
            result.command_arg := args[i]
        }
        i++
    }
    return result
}

[export]
def main() {
    let args = get_command_line_arguments()
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
    } elif (command == "search") {
        if (empty(parsed.command_arg)) {
            print("Error: search requires a query\n")
        } else {
            cmd_search(parsed.command_arg)
        }
    } else {
        print("Unknown command: {command}\n\n")
        print_usage()
    }
}
