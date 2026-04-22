options gen2
options indenting = 4
options persistent_heap
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_global_variables = false

require daslib/fio
require strings

require daslib/ansi_colors

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
    print("  update [name]                      Re-install at pinned version\n")
    print("  upgrade [name]                     Upgrade to latest version\n")
    print("  list                               List installed packages\n")
    print("  search [query]                     Search the package index\n")
    print("  introduce [url]                    Add a package to the index via PR\n")
    print("  withdraw <name>                    Remove a package from the index via PR\n")
    print("  update-index                       Refresh all index metadata from repos\n")
    print("  build                              Build C/C++ packages\n")
    print("  check                              Verify installed packages\n")
    print("  cleanup                            Remove modules/ and daspkg.lock for a fresh install\n")
    print("  doctor                             Check environment (git, cmake, gh)\n")
    print("\nOptions:\n")
    print("  --root <path>   Project root (default: current directory)\n")
    print("  --force         Force reinstall\n")
    print("  --global, -g    Operate on global modules (das_root/modules/)\n")
    print("  --color         Enable colored output\n")
    print("  --no-color      Disable colored output\n")
    print("  --verbose, -v   Print detailed progress\n")
    print("  --json          Machine-readable JSON output (list, check, search)\n")
    print("  --branch <name>, -b <name>  Install from a branch (e.g. master) instead of a tag\n")
}

[export]
def main() {
    let args = get_command_line_arguments()
    let parsed = parse_args(args)

    if (empty(parsed.command)) {
        print_usage()
        return 0
    }

    let root = get_full_file_name(parsed.root)
    log_init(root)
    let command = parsed.command
    let is_global = parsed.global_flag

    var rc = 0
    if (command == "list") {
        rc = cmd_list(root, parsed.json, is_global)
    } elif (command == "install") {
        if (empty(parsed.command_arg)) {
            rc = cmd_install_all(root, parsed.force, is_global)
        } else {
            rc = cmd_install(root, parsed.command_arg, parsed.force, is_global, "", parsed.branch)
        }
    } elif (command == "remove") {
        if (empty(parsed.command_arg)) {
            to_log(LOG_ERROR, "Error: remove requires a package name\n")
            rc = 1
        } else {
            rc = cmd_remove(root, parsed.command_arg, is_global)
        }
    } elif (command == "update") {
        if (empty(parsed.command_arg)) {
            rc = cmd_update_all(root, is_global)
        } else {
            rc = cmd_update(root, parsed.command_arg, is_global)
        }
    } elif (command == "upgrade") {
        if (empty(parsed.command_arg)) {
            rc = cmd_upgrade_all(root, is_global)
        } else {
            rc = cmd_upgrade(root, parsed.command_arg, is_global)
        }
    } elif (command == "build") {
        rc = cmd_build(root, is_global)
    } elif (command == "check") {
        rc = cmd_check(root, parsed.json, is_global)
    } elif (command == "cleanup") {
        rc = cmd_cleanup(root, parsed.force, is_global)
    } elif (command == "doctor") {
        rc = cmd_doctor()
    } elif (command == "search") {
        if (empty(parsed.command_arg)) {
            rc = cmd_search_all(root, parsed.json)
        } else {
            rc = cmd_search(root, parsed.command_arg, parsed.json)
        }
    } elif (command == "update-index") {
        rc = cmd_update_index(root)
    } elif (command == "introduce") {
        rc = cmd_introduce(root, parsed.command_arg)
    } elif (command == "withdraw") {
        if (empty(parsed.command_arg)) {
            to_log(LOG_ERROR, "Error: withdraw requires a package name\n")
            rc = 1
        } else {
            rc = cmd_withdraw(root, parsed.command_arg)
        }
    } else {
        to_log(LOG_ERROR, "Unknown command: {command}\n\n")
        print_usage()
        rc = 1
    }
    return rc
}
