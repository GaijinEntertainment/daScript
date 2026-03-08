options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_global_variables = false

module index shared public

require fio
require strings
require daslib/json
require daslib/json_boost

require lockfile
require utils

let INDEX_REPO = "github.com/borisbat/daspkg-index"

struct IndexEntry {
    url : string
    description : string
}

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
        run_cmd("git -C \"{cache_dir}\" checkout main 2>&1", output)
        run_cmd("git -C \"{cache_dir}\" fetch origin 2>&1", output)
        run_cmd("git -C \"{cache_dir}\" reset --hard origin/main 2>&1", output)
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
    if (!(parsed.value is _array)) {
        print("Error: index is not a JSON array\n")
        return false
    }
    unsafe {
        for (val in parsed.value as _array) {
            let name = val?.name ?? ""
            if (!empty(name)) {
                var entry : IndexEntry
                entry.url = val?.url ?? ""
                entry.description = val?.description ?? ""
                index[name] := entry
            }
        }
    }
    return true
}

def resolve_from_index(root, name : string) : string {
    var index : table<string; IndexEntry>
    if (!fetch_index(root, index)) {
        return ""
    }
    if (!(index |> key_exists(name))) {
        return ""
    }
    unsafe {
        return index[name].url
    }
}

def is_index_name(spec : string) : bool {
    return find(spec, "/") < 0 && find(spec, "\\") < 0 && find(spec, ".") < 0
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
    let dependencies = parsed?.dependencies
    if (dependencies == null) {
        return false
    }
    if (!(dependencies.value is _object)) {
        return false
    }
    unsafe {
        for (name, val in keys(dependencies.value as _object), values(dependencies.value as _object)) {
            if (val != null && val.value is _string) {
                deps[name] := val.value as _string
            }
        }
    }
    return true
}

def cmd_search(root, query : string) {
    var index : table<string; IndexEntry>
    if (!fetch_index(root, index)) {
        return
    }
    let q = to_lower(query)
    var count = 0
    for (name, entry in keys(index), values(index)) {
        if (find(to_lower(name), q) >= 0 || find(to_lower(entry.description), q) >= 0) {
            print("  {name} — {entry.description}\n")
            print("    {entry.url}\n")
            count ++
        }
    }
    print("\n{count} package(s) found\n")
}

// --- introduce / withdraw ---

struct PackageManifest {
    name : string
    description : string
}

def read_manifest(pkg_dir : string; var manifest : PackageManifest) : bool {
    let path = "{pkg_dir}/daspkg.json"
    if (!fexist(path)) {
        print("Error: no daspkg.json found in {pkg_dir}\n")
        return false
    }
    let text = fread(path)
    if (empty(text)) {
        print("Error: cannot read {path}\n")
        return false
    }
    var error : string
    var parsed = read_json(text, error)
    if (parsed == null) {
        print("Error parsing daspkg.json: {error}\n")
        return false
    }
    manifest.name = parsed?.name ?? ""
    manifest.description = parsed?.description ?? ""
    if (empty(manifest.name)) {
        print("Error: daspkg.json missing 'name' field\n")
        return false
    }
    if (empty(manifest.description)) {
        print("Error: daspkg.json missing 'description' field\n")
        return false
    }
    return true
}

def get_git_remote_url(pkg_dir : string) : string {
    var output : string
    let exit_code = run_cmd("git -C \"{pkg_dir}\" remote get-url origin 2>&1", output)
    if (exit_code != 0) {
        return ""
    }
    // trim trailing newline
    var url = strip(output)
    // convert git@github.com:user/repo.git or https://github.com/user/repo.git
    // to github.com/user/repo
    if (starts_with(url, "git@")) {
        // git@github.com:user/repo.git -> github.com/user/repo
        url = replace(url, "git@", "")
        url = replace(url, ":", "/")
    }
    if (starts_with(url, "https://")) {
        url = slice(url, 8)
    }
    if (starts_with(url, "http://")) {
        url = slice(url, 7)
    }
    if (ends_with(url, ".git")) {
        url = slice(url, 0, length(url) - 4)
    }
    return url
}

def generate_index_readme(packages_json : string) : string {
    var error : string
    var parsed = read_json(packages_json, error)
    if (parsed == null || !(parsed.value is _array)) {
        return ""
    }
    return build_string() <| $(var writer) {
        writer |> write("# daspkg package index\n\n")
        writer |> write("Package registry for [daspkg](https://github.com/GaijinEntertainment/daScript), the daslang package manager.\n\n")
        writer |> write("## Packages\n\n")
        writer |> write("| Package | Description | URL |\n")
        writer |> write("|---------|-------------|-----|\n")
        unsafe {
            for (val in parsed.value as _array) {
                let pkg_name = val?.name ?? ""
                let pkg_url = val?.url ?? ""
                let pkg_desc = val?.description ?? ""
                writer |> write("| {pkg_name} | {pkg_desc} | [{pkg_url}](https://{pkg_url}) |\n")
            }
        }
        unsafe {
            writer |> write("\n_{length(parsed.value as _array)} package(s) registered._\n")
        }
        writer |> write("\n## Adding a package\n\n")
        writer |> write("```\ndaspkg introduce github.com/user/repo\n```\n\n")
        writer |> write("This validates the package manifest (`daspkg.json`) and creates a PR.\n")
    }
}

def add_to_index_json(index_text, name, url, description : string) : string {
    // parse existing JSON array, add entry in sorted position, re-serialize
    var error : string
    var parsed = read_json(index_text, error)
    if (parsed == null || !(parsed.value is _array)) {
        return ""
    }
    unsafe {
        var idx = 0
        for (val in parsed.value as _array) {
            if ((val?.name ?? "") > name) {
                break
            }
            idx ++
        }
        var entry = JV({"name" => JV(name), "url" => JV(url), "description" => JV(description)})
        (parsed.value as _array) |> push(entry, idx)
    }
    return write_json(parsed)
}

def remove_from_index_json(index_text, name : string) : string {
    var error : string
    var parsed = read_json(index_text, error)
    if (parsed == null || !(parsed.value is _array)) {
        return ""
    }
    unsafe {
        var idx = -1
        for (val, ii in parsed.value as _array, range(length(parsed.value as _array))) {
            if ((val?.name ?? "") == name) {
                idx = ii
                break
            }
        }
        if (idx >= 0) {
            (parsed.value as _array) |> erase(idx)
        }
    }
    return write_json(parsed)
}

def cmd_introduce(root, source : string) {
    // step 1: determine package directory and URL
    var pkg_dir : string
    var pkg_url : string
    if (empty(source)) {
        // use current directory
        pkg_dir = root
        pkg_url = get_git_remote_url(root)
        if (empty(pkg_url)) {
            print("Error: could not determine git remote URL. Pass a URL explicitly or run from a git repo.\n")
            return
        }
    } else {
        // clone the repo to validate it
        let tmp_dir = "{root}/_daspkg_introduce_tmp"
        mkdir(tmp_dir)
        let git_url = "https://{source}.git"
        var output : string
        let exit_code = run_cmd("git clone --depth 1 \"{git_url}\" \"{tmp_dir}\" 2>&1", output)
        if (exit_code != 0) {
            print("Error: failed to clone {git_url}:\n{output}\n")
            rmdir_rec(tmp_dir)
            return
        }
        pkg_dir = tmp_dir
        pkg_url = source
    }

    // step 2: validate manifest
    var manifest : PackageManifest
    if (!read_manifest(pkg_dir, manifest)) {
        if (!empty(source)) {
            rmdir_rec(pkg_dir)
        }
        return
    }

    // step 3: check not already in index
    var idx : table<string; IndexEntry>
    if (!fetch_index(root, idx)) {
        if (!empty(source)) {
            rmdir_rec(pkg_dir)
        }
        print("Error: failed to fetch package index\n")
        return
    }
    if (idx |> key_exists(manifest.name)) {
        print("Error: package '{manifest.name}' already exists in the index\n")
        if (!empty(source)) {
            rmdir_rec(pkg_dir)
        }
        return
    }

    // cleanup temp clone
    if (!empty(source)) {
        rmdir_rec(pkg_dir)
    }

    // step 4: modify index and create PR via gh
    let cache_dir = get_index_cache_dir(root)
    let branch_name = "introduce-{manifest.name}"

    var output : string
    // ensure we're on main and up to date
    run_cmd("git -C \"{cache_dir}\" checkout main 2>&1", output)
    run_cmd("git -C \"{cache_dir}\" fetch origin 2>&1", output)
    run_cmd("git -C \"{cache_dir}\" reset --hard origin/main 2>&1", output)
    // create branch (force-reset if it already exists)
    run_cmd("git -C \"{cache_dir}\" checkout -B \"{branch_name}\" 2>&1", output)

    // read, modify, write packages.json
    let packages_file = "{cache_dir}/packages.json"
    let old_text = fread(packages_file)
    let new_text = add_to_index_json(old_text, manifest.name, pkg_url, manifest.description)
    if (empty(new_text)) {
        print("Error: failed to modify packages.json\n")
        run_cmd("git -C \"{cache_dir}\" checkout main 2>&1", output)
        return
    }
    fwrite(packages_file, "{new_text}\n")

    // regenerate README
    let readme = generate_index_readme(new_text)
    if (!empty(readme)) {
        fwrite("{cache_dir}/README.md", readme)
    }

    // commit and push
    run_cmd("git -C \"{cache_dir}\" add packages.json README.md 2>&1", output)
    run_cmd("git -C \"{cache_dir}\" commit -m \"Add {manifest.name}\" 2>&1", output)
    var exit_code = run_cmd("git -C \"{cache_dir}\" push --force origin \"{branch_name}\" 2>&1", output)
    if (exit_code != 0) {
        print("Error: failed to push branch: {output}\n")
        run_cmd("git -C \"{cache_dir}\" checkout main 2>&1", output)
        return
    }

    // create PR
    exit_code = run_cmd("gh pr create --repo borisbat/daspkg-index --base main --head \"{branch_name}\" --title \"Add package: {manifest.name}\" --body \"Introduce **{manifest.name}** — {manifest.description}\n\nURL: `{pkg_url}`\" 2>&1", output)
    if (exit_code != 0) {
        print("Error: failed to create PR: {output}\n")
    } else {
        print("PR created: {output}")
    }

    // switch back to main
    run_cmd("git -C \"{cache_dir}\" checkout main 2>&1", output)
}

def cmd_withdraw(root, name : string) {
    // step 1: check package exists in index
    var idx : table<string; IndexEntry>
    if (!fetch_index(root, idx)) {
        print("Error: failed to fetch package index\n")
        return
    }
    if (!(idx |> key_exists(name))) {
        print("Error: package '{name}' is not in the index\n")
        return
    }

    // step 2: modify index and create PR
    let cache_dir = get_index_cache_dir(root)
    let branch_name = "withdraw-{name}"

    var output : string
    // ensure we're on main and up to date
    run_cmd("git -C \"{cache_dir}\" checkout main 2>&1", output)
    run_cmd("git -C \"{cache_dir}\" fetch origin 2>&1", output)
    run_cmd("git -C \"{cache_dir}\" reset --hard origin/main 2>&1", output)
    run_cmd("git -C \"{cache_dir}\" checkout -B \"{branch_name}\" 2>&1", output)

    let packages_file = "{cache_dir}/packages.json"
    let old_text = fread(packages_file)
    let new_text = remove_from_index_json(old_text, name)
    if (empty(new_text)) {
        print("Error: failed to modify packages.json\n")
        run_cmd("git -C \"{cache_dir}\" checkout main 2>&1", output)
        return
    }
    fwrite(packages_file, "{new_text}\n")

    // regenerate README
    let readme = generate_index_readme(new_text)
    if (!empty(readme)) {
        fwrite("{cache_dir}/README.md", readme)
    }

    run_cmd("git -C \"{cache_dir}\" add packages.json README.md 2>&1", output)
    run_cmd("git -C \"{cache_dir}\" commit -m \"Remove {name}\" 2>&1", output)
    var exit_code = run_cmd("git -C \"{cache_dir}\" push --force origin \"{branch_name}\" 2>&1", output)
    if (exit_code != 0) {
        print("Error: failed to push branch: {output}\n")
        run_cmd("git -C \"{cache_dir}\" checkout main 2>&1", output)
        return
    }

    exit_code = run_cmd("gh pr create --repo borisbat/daspkg-index --base main --head \"{branch_name}\" --title \"Remove package: {name}\" --body \"Withdraw **{name}** from the package index.\" 2>&1", output)
    if (exit_code != 0) {
        print("Error: failed to create PR: {output}\n")
    } else {
        print("PR created: {output}")
    }

    run_cmd("git -C \"{cache_dir}\" checkout main 2>&1", output)
}
