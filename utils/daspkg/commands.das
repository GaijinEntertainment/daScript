options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_global_variables = false

module commands shared public

require fio
require strings

require lockfile

def cmd_list(root : string) {
    var lf : LockFile
    if (!read_lock_file(root, lf)) {
        print("No packages installed (no {LOCK_FILE_NAME} found)\n")
        return
    }
    if (length(lf.packages) == 0) {
        print("No packages installed\n")
        return
    }
    if (!empty(lf.sdk_version)) {
        print("SDK version: {lf.sdk_version}\n")
    }
    print("Installed packages:\n")
    for (name, entry in keys(lf.packages), values(lf.packages)) {
        var info = "  {name}"
        if (!empty(entry.version)) {
            info = "{info}@{entry.version}"
        }
        if (!empty(entry.tag)) {
            info = "{info} ({entry.tag})"
        }
        if (entry.local) {
            info = "{info} [local: {entry.source}]"
        } else {
            info = "{info} <- {entry.source}"
        }
        if (!entry.root) {
            info = "{info} (dependency)"
        }
        print("{info}\n")
    }
}

def cmd_install(root, spec : string; force : bool) {
    // parse name@version
    var source = spec
    var version = ""
    let at_pos = find(spec, "@")
    if (at_pos >= 0) {
        source = slice(spec, 0, at_pos)
        version = slice(spec, at_pos + 1)
    }

    // check if already installed
    var lf : LockFile
    read_lock_file(root, lf)

    // derive package name from source (last path component)
    let pkg_name = package_name_from_source(source)

    if (lf.packages |> key_exists(pkg_name) && !force) {
        print("Package '{pkg_name}' is already installed. Use --force to reinstall.\n")
        return
    }

    print("Installing {pkg_name} from {source}")
    if (!empty(version)) {
        print("@{version}")
    }
    print("...\n")

    // determine if local path or git URL
    if (is_local_path(source)) {
        install_local(root, pkg_name, source, lf)
    } else {
        install_git(root, pkg_name, source, version, lf)
    }
}

def cmd_install_all(root : string; force : bool) {
    var lf : LockFile
    if (!read_lock_file(root, lf)) {
        print("No {LOCK_FILE_NAME} found — nothing to install\n")
        return
    }
    print("Restoring {length(lf.packages)} package(s) from lock file...\n")
    for (name, entry in keys(lf.packages), values(lf.packages)) {
        if (entry.local) {
            print("  {name}: local path {entry.source} — skipping (verify manually)\n")
        } else {
            print("  {name}: TODO clone {entry.source} at {entry.tag}\n")
        }
    }
}

def cmd_remove(root, name : string) {
    var lf : LockFile
    if (!read_lock_file(root, lf)) {
        print("No {LOCK_FILE_NAME} found\n")
        return
    }
    if (!(lf.packages |> key_exists(name))) {
        print("Package '{name}' is not installed\n")
        return
    }
    let modules_dir = "{root}/modules/{name}"
    if (fexist(modules_dir)) {
        print("Removing {modules_dir}...\n")
        rmdir_rec(modules_dir)
    }
    lf.packages |> erase(name)
    write_lock_file(root, lf)
    print("Removed '{name}'\n")
}

def cmd_update(root, name : string) {
    print("TODO: update package '{name}'\n")
}

def cmd_update_all(root : string) {
    print("TODO: update all packages\n")
}

def cmd_build(root : string) {
    print("TODO: build C++ packages\n")
}

def cmd_check(root : string) {
    print("TODO: check SDK compatibility\n")
}

def cmd_search(query : string) {
    print("TODO: search package index for '{query}'\n")
}

// --- helpers ---

def package_name_from_source(source : string) : string {
    // "github.com/user/dasImgui" -> "dasImgui"
    // "/path/to/mymod" -> "mymod"
    let last_slash = find_back(source, "/")
    if (last_slash >= 0) {
        return slice(source, last_slash + 1)
    }
    return source
}

def is_local_path(source : string) : bool {
    if (starts_with(source, "/") || starts_with(source, "./") || starts_with(source, "../")) {
        return true
    }
    if (length(source) > 2 && character_at(source, 1) == ':') {
        return true
    }
    return false
}

def copy_dir_rec(src, dst : string) : bool {
    mkdir(dst)
    var ok = true
    dir(src) <| $(name) {
        if (name == "." || name == "..") {
            return
        }
        let src_child = "{src}/{name}"
        let dst_child = "{dst}/{name}"
        var st = stat(src_child)
        if (st.is_dir) {
            if (!copy_dir_rec(src_child, dst_child)) {
                ok = false
            }
        } elif (st.is_reg) {
            fopen(src_child, "rb") <| $(fin) {
                if (fin != null) {
                    let data = fread(fin)
                    fopen(dst_child, "wb") <| $(fout) {
                        if (fout != null) {
                            fwrite(fout, data)
                        } else {
                            ok = false
                        }
                    }
                } else {
                    ok = false
                }
            }
        }
    }
    return ok
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

def run_cmd(cmd : string; var output : string) : int {
    var exit_code = -1
    unsafe {
        exit_code = popen(cmd) <| $(f) {
            if (f != null) {
                output := fread(f)
            }
        }
    }
    return exit_code
}

def install_git(root, pkg_name, source, version : string; var lf : LockFile) {
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
    entry.root = true
    lf.packages |> insert(pkg_name, entry)
    write_lock_file(root, lf)
    print("Installed '{pkg_name}'\n")
}

def find_back(s, sub : string) : int {
    var last = -1
    var pos = 0
    while (true) {
        let found = find(s, sub, pos)
        if (found < 0) {
            break
        }
        last = found
        pos = found + 1
    }
    return last
}
