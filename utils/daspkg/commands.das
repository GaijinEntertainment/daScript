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

let INDEX_REPO = "github.com/borisbat/daspkg-index"

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

    // determine if local path, index name, or git URL
    if (is_local_path(source)) {
        install_local(root, pkg_name, source, lf)
    } elif (is_index_name(source)) {
        let url = resolve_from_index(root, source)
        if (empty(url)) {
            return
        }
        print("Resolved '{source}' -> {url}\n")
        install_git(root, pkg_name, url, version, lf)
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

struct IndexEntry {
    url : string
    description : string
}

def cmd_search(root, query : string) {
    var index : table<string; IndexEntry>
    if (!fetch_index(root, index)) {
        print("Error: failed to fetch package index\n")
        return
    }
    var found = 0
    let q = to_lower(query)
    for (name, entry in keys(index), values(index)) {
        if (find(to_lower(name), q) >= 0 || find(to_lower(entry.description), q) >= 0) {
            print("  {name} — {entry.description}\n")
            print("    {entry.url}\n")
            found++
        }
    }
    if (found == 0) {
        print("No packages matching '{query}'\n")
    } else {
        print("\n{found} package(s) found\n")
    }
}

// --- index and dependencies ---

def get_index_cache_dir(root : string) : string {
    return "{root}/modules/.daspkg_cache/index"
}

def fetch_index(root : string; var index : table<string; IndexEntry>) : bool {
    let cache_dir = get_index_cache_dir(root)
    let packages_file = "{cache_dir}/packages.json"
    let modules_dir = "{root}/modules"
    mkdir(modules_dir)
    mkdir("{modules_dir}/.daspkg_cache")

    if (fexist(packages_file)) {
        // update existing cache
        var output : string
        run_cmd("git -C \"{cache_dir}\" pull --ff-only 2>&1", output)
    } else {
        // fresh clone
        let git_url = "https://{INDEX_REPO}.git"
        var output : string
        let exit_code = run_cmd("git clone --depth 1 \"{git_url}\" \"{cache_dir}\" 2>&1", output)
        if (exit_code != 0) {
            print("Error: failed to clone index: {output}\n")
            return false
        }
    }

    let text = fread(packages_file)
    if (empty(text)) {
        print("Error: cannot read {packages_file}\n")
        return false
    }
    var error : string
    var parsed = read_json(text, error)
    if (parsed == null) {
        print("Error parsing index: {error}\n")
        return false
    }
    if (!(parsed.value is _object)) {
        print("Error: index is not a JSON object\n")
        return false
    }
    unsafe {
        for (name, val in keys(parsed.value as _object), values(parsed.value as _object)) {
            var entry : IndexEntry
            entry.url = val?.url ?? ""
            entry.description = val?.description ?? ""
            index[name] := entry
        }
    }
    return true
}

def resolve_from_index(root, name : string) : string {
    var index : table<string; IndexEntry>
    if (!fetch_index(root, index)) {
        return ""
    }
    if (index |> key_exists(name)) {
        var result : string
        unsafe {
            result := index[name].url
        }
        return result
    }
    print("Error: package '{name}' not found in index\n")
    return ""
}

def is_index_name(spec : string) : bool {
    // no slashes, no dots with slashes, no drive letters = just a package name
    return find(spec, "/") < 0 && find(spec, "\\") < 0 && !is_local_path(spec)
}

def read_pkg_dependencies(pkg_dir : string; var deps : table<string; string>) : bool {
    let manifest = "{pkg_dir}/daspkg.json"
    if (!fexist(manifest)) {
        return false
    }
    let text = fread(manifest)
    if (empty(text)) {
        return false
    }
    var error : string
    var parsed = read_json(text, error)
    if (parsed == null) {
        return false
    }
    let deps_val = parsed?.dependencies
    if (deps_val == null || !(deps_val.value is _object)) {
        return false
    }
    unsafe {
        for (dep_name, dep_val in keys(deps_val.value as _object), values(deps_val.value as _object)) {
            deps[dep_name] = dep_val ?? ""
        }
    }
    return true
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
        install_git(root, dep_name, dep_source, "", lf, false)
        // recurse
        install_dependencies(root, dep_name, lf)
    }
}

// --- helpers ---

def package_name_from_source(source : string) : string {
    // "github.com/user/dasImgui" -> "dasImgui"
    // "/path/to/mymod" -> "mymod"
    // "C:\path\to\mymod" -> "mymod"
    var last_sep = rfind(source, "/")
    let last_back = rfind(source, "\\")
    if (last_back > last_sep) {
        last_sep = last_back
    }
    if (last_sep >= 0) {
        return slice(source, last_sep + 1)
    }
    return source
}

def is_local_path(source : string) : bool {
    if (starts_with(source, "/") || starts_with(source, "./") || starts_with(source, "../")) {
        return true
    }
    if (starts_with(source, ".\\") || starts_with(source, "..\\")) {
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

def run_cmd(cmd : string; var output : string&) : int {
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

