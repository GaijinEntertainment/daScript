options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_global_variables = false

module commands shared public

require fio
require strings
require daslib/json
require daslib/json_boost

require lockfile
require utils
require index

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
            i ++
            break
        }
        i ++
    }
    while (i < length(args)) {
        let arg = args[i]
        if (arg == "--force") {
            result.force = true
        } elif (arg == "--root" && i + 1 < length(args)) {
            i ++
            result.root = args[i]
        } elif (empty(result.command)) {
            result.command = arg
        } elif (empty(result.command_arg)) {
            result.command_arg = arg
        }
        i ++
    }
    return result
}

def cmd_list(root : string) {
    var lf : LockFile
    if (!read_lock_file(root, lf)) {
        print("No packages installed (no lock file found)\n")
        return
    }
    if (length(lf.packages) == 0) {
        print("No packages installed\n")
        return
    }
    print("Installed packages:\n")
    for (name, entry in keys(lf.packages), values(lf.packages)) {
        var flags : string
        if (entry.local) {
            flags = " (local)"
        }
        if (!entry.root) {
            flags = "{flags} (dependency)"
        }
        if (!empty(entry.tag)) {
            print("  {name} @ {entry.tag}{flags}\n")
            print("    {entry.source}\n")
        } else {
            print("  {name}{flags}\n")
            print("    {entry.source}\n")
        }
    }
}

def install_local(root, pkg_name, source : string; var lf : LockFile) {
    let modules_dir = "{root}/modules"
    let target = "{modules_dir}/{pkg_name}"
    let full_source = get_full_file_name(source)

    if (!fexist(full_source)) {
        print("Error: local path '{full_source}' does not exist\n")
        return
    }

    mkdir(modules_dir)

    if (fexist(target)) {
        print("Removing existing {target}...\n")
        rmdir_rec(target)
    }

    print("Copying {full_source} -> {target}\n")
    if (!copy_dir_rec(full_source, target)) {
        print("Error: copy failed\n")
        return
    }

    var entry = PackageEntry()
    entry.source = full_source
    entry.local = true
    entry.root = true
    lf.packages |> insert(pkg_name, entry)
    write_lock_file(root, lf)
    print("Installed '{pkg_name}' (local)\n")
}

def install_dependencies(root, pkg_name : string; var lf : LockFile) {
    let pkg_dir = "{root}/modules/{pkg_name}"
    var deps : table<string; string>
    if (!read_pkg_dependencies(pkg_dir, deps)) {
        return
    }
    for (dep_name, dep_source in keys(deps), values(deps)) {
        if (lf.packages |> key_exists(dep_name)) {
            continue
        }
        print("  Installing dependency: {dep_name}...\n")
        install_git(root, dep_name, dep_source, "", lf, [is_root = false])
    }
}

def install_git(root, pkg_name, source, version : string; var lf : LockFile; is_root : bool = true) {
    let modules_dir = "{root}/modules"
    let tmp_dir = "{modules_dir}/.daspkg_tmp/{pkg_name}"
    let target_dir = "{modules_dir}/{pkg_name}"

    mkdir(modules_dir)
    mkdir("{modules_dir}/.daspkg_tmp")

    let git_url = "https://{source}.git"

    // step 1: shallow clone
    print("Cloning {git_url}...\n")
    var output : string
    var exit_code = run_cmd("git clone --depth 1 \"{git_url}\" \"{tmp_dir}\" 2>&1", output)
    if (exit_code != 0) {
        print("Error: git clone failed (exit {exit_code}):\n{output}\n")
        return
    }

    // step 2: checkout specific tag/version if requested
    if (!empty(version)) {
        print("Fetching tag {version}...\n")
        exit_code = run_cmd("git -C \"{tmp_dir}\" fetch --depth 1 origin tag \"{version}\" 2>&1", output)
        if (exit_code != 0) {
            print("Error: git fetch tag failed (exit {exit_code}):\n{output}\n")
            rmdir_rec(tmp_dir)
            return
        }
        exit_code = run_cmd("git -C \"{tmp_dir}\" checkout \"{version}\" 2>&1", output)
        if (exit_code != 0) {
            print("Error: git checkout failed (exit {exit_code}):\n{output}\n")
            rmdir_rec(tmp_dir)
            return
        }
    }

    // step 3: move to final location
    if (fexist(target_dir)) {
        print("Removing existing {target_dir}...\n")
        rmdir_rec(target_dir)
    }
    if (!rename(tmp_dir, target_dir)) {
        print("Error: failed to move {tmp_dir} to {target_dir}\n")
        return
    }

    // step 4: record in lock file
    var entry = PackageEntry()
    entry.source = source
    entry.version = version
    entry.tag = version
    entry.root = is_root
    lf.packages |> insert(pkg_name, entry)
    write_lock_file(root, lf)
    print("Installed '{pkg_name}'\n")

    // step 5: install dependencies
    install_dependencies(root, pkg_name, lf)
}

def cmd_install(root, spec : string; force : bool) {
    // parse spec: name[@version]
    var source = spec
    var version = ""
    let at_pos = find(spec, "@")
    if (at_pos >= 0) {
        source = slice(spec, 0, at_pos)
        version = slice(spec, at_pos + 1)
    }

    let pkg_name = package_name_from_source(source)
    if (empty(pkg_name)) {
        print("Error: cannot determine package name from '{source}'\n")
        return
    }

    // check if already installed
    var lf : LockFile
    read_lock_file(root, lf)
    if (!force && lf.packages |> key_exists(pkg_name)) {
        print("'{pkg_name}' is already installed (use --force to reinstall)\n")
        return
    }

    if (is_local_path(source)) {
        install_local(root, pkg_name, source, lf)
    } elif (is_index_name(source)) {
        // resolve from index
        print("Installing {source} from {source}...\n")
        let url = resolve_from_index(root, source)
        if (empty(url)) {
            print("Error: package '{source}' not found in the index\n")
            return
        }
        print("Resolved '{source}' -> {url}\n")
        install_git(root, pkg_name, url, version, lf)
    } else {
        install_git(root, pkg_name, source, version, lf)
    }
}

def cmd_install_all(root : string; force : bool) {
    var deps : table<string; string>
    if (!read_pkg_dependencies(root, deps)) {
        print("No daspkg.json found or no dependencies listed\n")
        return
    }
    for (name, source in keys(deps), values(deps)) {
        cmd_install(root, source, force)
    }
}

def cmd_remove(root, name : string) {
    var lf : LockFile
    if (!read_lock_file(root, lf)) {
        print("Error: no lock file found\n")
        return
    }
    if (!(lf.packages |> key_exists(name))) {
        print("Error: package '{name}' is not installed\n")
        return
    }

    let pkg_dir = "{root}/modules/{name}"
    if (fexist(pkg_dir)) {
        print("Removing {pkg_dir}...\n")
        rmdir_rec(pkg_dir)
    }

    lf.packages |> erase(name)
    write_lock_file(root, lf)
    print("Removed '{name}'\n")
}

def cmd_update(root, name : string) {
    var lf : LockFile
    if (!read_lock_file(root, lf)) {
        print("Error: no lock file found\n")
        return
    }
    if (!(lf.packages |> key_exists(name))) {
        print("Error: package '{name}' is not installed\n")
        return
    }
    let entry = lf.packages[name]
    if (entry.local) {
        print("Skipping '{name}' (local package)\n")
        return
    }
    let pkg_dir = "{root}/modules/{name}"
    print("Updating '{name}'...\n")
    var output : string
    let exit_code = run_cmd("git -C \"{pkg_dir}\" pull --ff-only 2>&1", output)
    if (exit_code != 0) {
        // fall back to fetch + reset
        let exit_code2 = run_cmd("git -C \"{pkg_dir}\" fetch origin 2>&1 && git -C \"{pkg_dir}\" reset --hard origin/HEAD 2>&1", output)
        if (exit_code2 != 0) {
            print("Error: update failed:\n{output}\n")
            return
        }
    }
    print("Updated '{name}'\n")
}

def cmd_update_all(root : string) {
    var lf : LockFile
    if (!read_lock_file(root, lf)) {
        print("No packages installed (no lock file found)\n")
        return
    }
    for (name in keys(lf.packages)) {
        cmd_update(root, name)
    }
}

def cmd_build(root : string) {
    print("Build not yet implemented — reserved for C++ plugin packages\n")
}

def cmd_check(root : string) {
    var lf : LockFile
    if (!read_lock_file(root, lf)) {
        print("No packages installed\n")
        return
    }
    var ok = true
    for (name, entry in keys(lf.packages), values(lf.packages)) {
        let pkg_dir = "{root}/modules/{name}"
        if (!fexist(pkg_dir)) {
            print("  MISSING: {name} (directory not found)\n")
            ok = false
            continue
        }
        let has_module = fexist("{pkg_dir}/.das_module")
        if (!has_module) {
            print("  WARNING: {name} has no .das_module\n")
        } else {
            print("  OK: {name}\n")
        }
    }
    if (ok) {
        print("All packages present\n")
    } else {
        print("Some packages are missing — run 'daspkg install' to fix\n")
    }
}
