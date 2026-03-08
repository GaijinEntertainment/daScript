options gen2
options rtti
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
require package_runner

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
    try_build_package(target, pkg_name)
}

def install_dependencies(root, pkg_name : string; var lf : LockFile) {
    let pkg_dir = "{root}/modules/{pkg_name}"
    let das_package = "{pkg_dir}/.das_package"
    if (!fexist(das_package)) {
        return
    }
    var pkg_deps : array<PackageDependency>
    if (run_das_package_deps(das_package, "", pkg_deps)) {
        for (d in pkg_deps) {
            let dep_name = package_name_from_source(d.source)
            if (lf.packages |> key_exists(dep_name)) {
                continue
            }
            print("  Installing dependency: {dep_name}...\n")
            install_git(root, dep_name, d.source, "", lf, [is_root = false])
        }
    }
}

def private git_checkout_tag(dir, tag : string) : bool {
    var output : string
    var exit_code = run_cmd("git -C \"{dir}\" fetch --depth 1 origin tag \"{tag}\" 2>&1", output)
    if (exit_code != 0) {
        print("Error: git fetch tag failed (exit {exit_code}):\n{output}\n")
        return false
    }
    exit_code = run_cmd("git -C \"{dir}\" checkout \"{tag}\" 2>&1", output)
    if (exit_code != 0) {
        print("Error: git checkout failed (exit {exit_code}):\n{output}\n")
        return false
    }
    return true
}

def private git_checkout_branch(dir, branch : string) : bool {
    var output : string
    var exit_code = run_cmd("git -C \"{dir}\" fetch --depth 1 origin \"{branch}\" 2>&1", output)
    if (exit_code != 0) {
        print("Error: git fetch branch failed (exit {exit_code}):\n{output}\n")
        return false
    }
    exit_code = run_cmd("git -C \"{dir}\" checkout \"{branch}\" 2>&1", output)
    if (exit_code != 0) {
        print("Error: git checkout failed (exit {exit_code}):\n{output}\n")
        return false
    }
    return true
}

def private git_clone(url, dir : string) : bool {
    var output : string
    let exit_code = run_cmd("git clone --depth 1 \"{url}\" \"{dir}\" 2>&1", output)
    if (exit_code != 0) {
        print("Error: git clone failed (exit {exit_code}):\n{output}\n")
        return false
    }
    return true
}

def install_git(root, pkg_name, source, version : string; var lf : LockFile; is_root : bool = true) {
    let modules_dir = "{root}/modules"
    let tmp_dir = "{modules_dir}/.daspkg_tmp/{pkg_name}"
    let target_dir = "{modules_dir}/{pkg_name}"

    mkdir(modules_dir)
    mkdir("{modules_dir}/.daspkg_tmp")

    // step 1: shallow clone default branch
    let git_url = "https://{source}.git"
    print("Cloning {git_url}...\n")
    if (!git_clone(git_url, tmp_dir)) {
        return
    }

    // step 2: resolve version via .das_package (if present)
    var actual_source = source
    var actual_tag = ""
    var actual_branch = ""
    let das_package = "{tmp_dir}/.das_package"
    if (fexist(das_package)) {
        var res : ResolveResult
        if (run_das_package_resolve(das_package, "", version, res)) {
            if (!empty(res.source)) {
                // redirect — re-clone from a different repo
                print("Redirecting to {res.source}...\n")
                rmdir_rec(tmp_dir)
                actual_source = res.source
                let redirect_url = "https://{res.source}.git"
                if (!git_clone(redirect_url, tmp_dir)) {
                    return
                }
                // checkout the ref from the redirect
                if (!empty(res.tag)) {
                    print("Fetching tag {res.tag}...\n")
                    if (!git_checkout_tag(tmp_dir, res.tag)) {
                        rmdir_rec(tmp_dir)
                        return
                    }
                    actual_tag = res.tag
                } elif (!empty(res.branch)) {
                    print("Switching to branch {res.branch}...\n")
                    if (!git_checkout_branch(tmp_dir, res.branch)) {
                        rmdir_rec(tmp_dir)
                        return
                    }
                    actual_branch = res.branch
                }
            } elif (!empty(res.tag)) {
                // tag checkout
                print("Fetching tag {res.tag}...\n")
                if (!git_checkout_tag(tmp_dir, res.tag)) {
                    rmdir_rec(tmp_dir)
                    return
                }
                actual_tag = res.tag
            } elif (!empty(res.branch)) {
                // branch checkout
                print("Switching to branch {res.branch}...\n")
                if (!git_checkout_branch(tmp_dir, res.branch)) {
                    rmdir_rec(tmp_dir)
                    return
                }
                actual_branch = res.branch
            } elif (!empty(version)) {
                // resolve returned nothing — fall back to version as tag
                print("Fetching tag {version}...\n")
                if (!git_checkout_tag(tmp_dir, version)) {
                    rmdir_rec(tmp_dir)
                    return
                }
                actual_tag = version
            }
        }
    }
    if (empty(actual_tag) && empty(actual_branch) && !empty(version)) {
        // no .das_package or resolve failed — treat version as tag directly
        print("Fetching tag {version}...\n")
        if (!git_checkout_tag(tmp_dir, version)) {
            rmdir_rec(tmp_dir)
            return
        }
        actual_tag = version
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
    entry.source = actual_source
    entry.version = version
    entry.tag = actual_tag
    entry.branch = actual_branch
    entry.root = is_root
    lf.packages |> insert(pkg_name, entry)
    write_lock_file(root, lf)
    print("Installed '{pkg_name}'\n")

    // step 5: install dependencies
    install_dependencies(root, pkg_name, lf)

    // step 6: build
    try_build_package(target_dir, pkg_name)
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
    let das_package = "{root}/.das_package"
    if (!fexist(das_package)) {
        print("No .das_package found\n")
        return
    }
    var pkg_deps : array<PackageDependency>
    if (!run_das_package_deps(das_package, "", pkg_deps) || length(pkg_deps) == 0) {
        print("No dependencies listed in .das_package\n")
        return
    }
    for (d in pkg_deps) {
        cmd_install(root, d.source, force)
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
        // re-copy from original source path
        let source = entry.source
        if (empty(source) || !fexist(source)) {
            print("Error: local source '{source}' not found\n")
            return
        }
        let target = "{root}/modules/{name}"
        print("Updating '{name}' from {source}...\n")
        if (fexist(target)) {
            rmdir_rec(target)
        }
        if (!copy_dir_rec(source, target)) {
            print("Error: copy failed\n")
            return
        }
        print("Updated '{name}'\n")
        try_build_package(target, name)
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
    try_build_package(pkg_dir, name)
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

def build_package(pkg_dir : string) : bool {
    let build_dir = "{pkg_dir}/_build"
    let das_root = get_das_root()

    // Configure — pass DASLANG_DIR so the package can find headers and libs
    var output : string
    var exit_code = run_cmd("cmake -B \"{build_dir}\" -S \"{pkg_dir}\" -DDASLANG_DIR=\"{das_root}\" 2>&1", output)
    if (exit_code != 0) {
        print("  CMake configure FAILED:\n{output}\n")
        return false
    }

    // Build
    exit_code = run_cmd("cmake --build \"{build_dir}\" --config Release 2>&1", output)
    if (exit_code != 0) {
        print("  CMake build FAILED:\n{output}\n")
        return false
    }
    return true
}

def cmd_build(root : string) {
    let modules_dir = "{root}/modules"
    if (!fexist(modules_dir)) {
        print("No modules directory found\n")
        return
    }
    var built = 0
    var failed = 0
    dir(modules_dir) <| $(name) {
        if (name == "." || name == ".." || starts_with(name, ".")) {
            return
        }
        let pkg_dir = "{modules_dir}/{name}"
        if (fexist("{pkg_dir}/CMakeLists.txt")) {
            print("Building '{name}'...\n")
            if (build_package(pkg_dir)) {
                built ++
            } else {
                failed ++
            }
        }
    }
    if (built == 0 && failed == 0) {
        print("No packages with CMakeLists.txt found\n")
    } else {
        print("Build complete: {built} succeeded, {failed} failed\n")
    }
}

def check_tool(name, cmd : string) : bool {
    var output : string
    let exit_code = run_cmd("{cmd} 2>&1", output)
    if (exit_code == 0) {
        let trimmed = strip(output)
        if (!empty(trimmed)) {
            let nl = find(trimmed, "\n")
            let version_line = nl >= 0 ? slice(trimmed, 0, nl) : trimmed
            print("  OK: {name} — {version_line}\n")
        } else {
            print("  OK: {name}\n")
        }
        return true
    } else {
        print("  MISSING: {name} — not found in PATH\n")
        return false
    }
}

def cmd_doctor() {
    print("Checking environment...\n")
    var ok = true
    if (!check_tool("git", "git --version")) {
        ok = false
    }
    if (!check_tool("cmake", "cmake --version")) {
        ok = false
    }
    if (!check_tool("gh", "gh --version")) {
        // gh is optional — only needed for introduce/withdraw
        print("    (optional — only needed for introduce/withdraw)\n")
    }
    if (ok) {
        print("Environment OK\n")
    } else {
        print("Some required tools are missing\n")
    }
}

def try_build_package(pkg_dir, name : string) {
    let das_package = "{pkg_dir}/.das_package"
    if (fexist(das_package)) {
        var info : PackageBuildInfo
        if (run_das_package_build(das_package, info)) {
            if (info.is_cmake) {
                print("Building '{name}'...\n")
                if (!build_package(pkg_dir)) {
                    print("  Warning: build failed for '{name}'\n")
                }
            } elif (info.is_custom && !empty(info.command)) {
                print("Building '{name}' (custom)...\n")
                var output : string
                let exit_code = run_cmd("cd \"{pkg_dir}\" && {info.command} 2>&1", output)
                if (exit_code != 0) {
                    print("  Warning: custom build failed for '{name}':\n{output}\n")
                }
            }
        }
        return
    }
    // fallback: check for CMakeLists.txt directly
    if (fexist("{pkg_dir}/CMakeLists.txt")) {
        print("Building '{name}'...\n")
        if (!build_package(pkg_dir)) {
            print("  Warning: build failed for '{name}'\n")
        }
    }
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
