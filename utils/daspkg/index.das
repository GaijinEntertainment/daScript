options gen2
options rtti
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_global_variables = false

module index shared public

require fio
require strings
require daslib/json
require daslib/json_boost
require daslib/strings_boost
require daslib/ansi_colors

require lockfile
require utils
require package_runner

let INDEX_REPO = "github.com/borisbat/daspkg-index"

struct IndexEntry {
    url : string
    description : string
    author : string
    license : string
    tags : array<string>
    min_sdk : string
    modules : array<string>
    latest_version : string
    last_updated : string
    has_native : bool
    dependencies : array<string>
}

def get_index_cache_dir(root : string) : string {
    return "{root}/modules/.daspkg_cache/index"
}

def private parse_string_array(val : JsonValue?; field : string) : array<string> {
    var result : array<string>
    let arr = val?[field]
    if (arr != null) {
        if (arr.value is _array) {
            for (item in arr.value as _array) {
                if (item.value is _string) {
                    result |> push_clone(item.value as _string)
                }
            }
        }
    }
    return <- result
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

    return parse_index_file(packages_file, index)
}

def parse_index_file(packages_file : string; var index : table<string; IndexEntry>) : bool {
    let text = fread(packages_file)
    if (empty(text)) {
        print("Error: cannot read {packages_file}\n")
        return false
    }
    return parse_index_json(text, index)
}

def parse_index_json(text : string; var index : table<string; IndexEntry>) : bool {
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
                var entry = IndexEntry(
                    url = val?.url ?? "",
                    description = val?.description ?? "",
                    author = val?.author ?? "",
                    license = val?.license ?? "",
                    min_sdk = val?.min_sdk ?? "",
                    latest_version = val?.latest_version ?? "",
                    last_updated = val?.last_updated ?? "",
                    tags <- parse_string_array(val, "tags"),
                    modules <- parse_string_array(val, "modules"),
                    dependencies <- parse_string_array(val, "dependencies")
                )
                let native_val = val?["has_native"]
                if (native_val != null && native_val.value is _bool) {
                    entry.has_native = native_val.value as _bool
                }
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

// ── Search ──────────────────────────────────────────────────────────

struct SearchResult {
    name : string
    score : int       // lower = better match
}

struct PackageInfo {
    name : string
    url : string
    description : string
    author : string
    license : string
    tags : array<string>
    modules : array<string>
    min_sdk : string
    latest_version : string
    last_updated : string
    has_native : bool
    dependencies : array<string>
}

def private make_package_info(name : string; entry : IndexEntry) : PackageInfo {
    return PackageInfo(
        name = name,
        url = entry.url,
        description = entry.description,
        author = entry.author,
        license = entry.license,
        tags := entry.tags,
        modules := entry.modules,
        min_sdk = entry.min_sdk,
        latest_version = entry.latest_version,
        last_updated = entry.last_updated,
        has_native = entry.has_native,
        dependencies := entry.dependencies
    )
}

def private fuzzy_match(haystack, needle : string; max_distance : int = 2) : bool {
    if (find(to_lower(haystack), needle) >= 0) {
        return true
    }
    // try levenshtein on short strings (word-level)
    if (length(haystack) < 30 && length(needle) < 15) {
        return levenshtein_distance_fast(to_lower(haystack), needle) <= max_distance
    }
    return false
}

def private match_score(name, query : string) : int {
    // exact prefix match = best
    let ln = to_lower(name)
    let q = query
    if (starts_with(ln, q)) {
        return 0
    }
    // substring match
    if (find(ln, q) >= 0) {
        return 1
    }
    // fuzzy
    if (length(name) < 30 && length(query) < 15) {
        let dist = levenshtein_distance_fast(ln, q)
        if (dist <= 2) {
            return 2 + dist
        }
    }
    return 100  // no match
}

def matches_entry(name : string; entry : IndexEntry; query : string) : int {
    // check for tag: and module: filters
    if (starts_with(query, "tag:")) {
        let tag_q = to_lower(slice(query, 4))
        for (t in entry.tags) {
            if (fuzzy_match(t, tag_q)) {
                return 0
            }
        }
        return 100
    }
    if (starts_with(query, "module:")) {
        let mod_q = to_lower(slice(query, 7))
        for (m in entry.modules) {
            if (fuzzy_match(m, mod_q)) {
                return 0
            }
        }
        return 100
    }
    // general search across all fields
    var best = match_score(name, query)
    if (best == 0) {
        return 0
    }
    let desc_score = match_score(entry.description, query)
    if (desc_score < best) {
        best = desc_score
    }
    for (t in entry.tags) {
        let s = match_score(t, query)
        if (s < best) {
            best = s
        }
    }
    for (m in entry.modules) {
        let s = match_score(m, query)
        if (s < best) {
            best = s
        }
    }
    let author_score = match_score(entry.author, query)
    if (author_score < best) {
        best = author_score
    }
    return best
}

def private format_entry(name : string; entry : IndexEntry) {
    // line 1: name — description
    print("  {bold_str(name)} — {entry.description}\n")
    // line 2: author | license | version | updated
    var meta_parts : array<string>
    if (!empty(entry.author)) {
        meta_parts |> push_clone(entry.author)
    }
    if (!empty(entry.license)) {
        meta_parts |> push_clone(entry.license)
    }
    if (!empty(entry.latest_version)) {
        meta_parts |> push_clone(entry.latest_version)
    }
    if (!empty(entry.last_updated)) {
        meta_parts |> push_clone(entry.last_updated)
    }
    if (entry.has_native) {
        meta_parts |> push_clone(yellow_str("native"))
    }
    if (length(meta_parts) > 0) {
        print("    {join(meta_parts, " | ")}\n")
    }
    // line 3: modules | tags | min-sdk
    var cat_parts : array<string>
    if (length(entry.modules) > 0) {
        cat_parts |> push_clone("modules: {cyan_str(join(entry.modules, ", "))}")
    }
    if (length(entry.tags) > 0) {
        cat_parts |> push_clone("tags: {green_str(join(entry.tags, ", "))}")
    }
    if (!empty(entry.min_sdk)) {
        cat_parts |> push_clone("min-sdk: {entry.min_sdk}")
    }
    if (length(entry.dependencies) > 0) {
        cat_parts |> push_clone("deps: {join(entry.dependencies, ", ")}")
    }
    if (length(cat_parts) > 0) {
        print("    {join(cat_parts, " | ")}\n")
    }
    // line 4: install command
    let install_cmd = "daspkg install {entry.url}"
    print("    {dim_str(install_cmd)}\n")
}

def cmd_search(root, query : string; json : bool = false) {
    var index : table<string; IndexEntry>
    if (!fetch_index(root, index)) {
        if (json) {
            print("[]\n")
        }
        return
    }
    let q = to_lower(query)
    var results : array<SearchResult>
    for (name, entry in keys(index), values(index)) {
        let score = matches_entry(name, entry, q)
        if (score < 100) {
            results |> emplace(SearchResult(name = name, score = score))
        }
    }
    // sort by score (lower = better)
    sort(results) <| $(a, b : SearchResult) : bool {
        if (a.score != b.score) {
            return a.score < b.score
        }
        return a.name < b.name
    }
    if (json) {
        var infos : array<PackageInfo>
        for (r in results) {
            if (index |> key_exists(r.name)) {
                unsafe {
                    infos |> emplace(make_package_info(r.name, index[r.name]))
                }
            }
        }
        print(sprint_json(infos, false))
        print("\n")
        return
    }
    for (r in results) {
        if (index |> key_exists(r.name)) {
            unsafe {
                format_entry(r.name, index[r.name])
            }
            print("\n")
        }
    }
    print("{length(results)} package(s) found\n")
}

def cmd_search_all(root : string; json : bool = false) {
    var index : table<string; IndexEntry>
    if (!fetch_index(root, index)) {
        if (json) {
            print("[]\n")
        }
        return
    }
    // collect and sort by name
    var names : array<string>
    for (name in keys(index)) {
        names |> push_clone(name)
    }
    sort(names) <| $(a, b : string) : bool {
        return a < b
    }
    if (json) {
        var infos : array<PackageInfo>
        for (name in names) {
            if (index |> key_exists(name)) {
                unsafe {
                    infos |> emplace(make_package_info(name, index[name]))
                }
            }
        }
        print(sprint_json(infos, false))
        print("\n")
        return
    }
    for (name in names) {
        if (index |> key_exists(name)) {
            unsafe {
                format_entry(name, index[name])
            }
            print("\n")
        }
    }
    print("{length(names)} package(s) in index\n")
}

// ── introduce / withdraw ────────────────────────────────────────────

struct PackageManifest {
    name : string
    description : string
    author : string
    license : string
    tags : array<string>
    min_sdk : string
    modules : array<string>
    has_native : bool
    dependencies : array<string>
}

def read_manifest(pkg_dir : string; var manifest : PackageManifest) : bool {
    let das_package = "{pkg_dir}/.das_package"
    if (!fexist(das_package)) {
        print("Error: no .das_package found in {pkg_dir}\n")
        return false
    }
    var meta : PackageMeta
    if (!run_das_package_meta(das_package, meta)) {
        print("Error: failed to run .das_package in {pkg_dir}\n")
        return false
    }
    manifest.name = meta.pkg_name
    manifest.description = meta.description
    manifest.author = meta.author
    manifest.license = meta.license
    manifest.min_sdk = meta.min_sdk
    for (tag in meta.tags) {
        manifest.tags |> push_clone(tag)
    }
    if (empty(manifest.name)) {
        print("Error: .das_package missing package_name()\n")
        return false
    }
    if (empty(manifest.description)) {
        print("Error: .das_package missing package_description()\n")
        return false
    }
    // extract modules from .das_module
    extract_modules(pkg_dir, manifest)
    // check build info
    var build_info : PackageBuildInfo
    if (run_das_package_build(das_package, build_info)) {
        manifest.has_native = build_info.is_cmake || build_info.is_custom
    }
    // extract dependencies
    var deps : array<PackageDependency>
    if (run_das_package_deps(das_package, "", deps)) {
        for (d in deps) {
            manifest.dependencies |> push_clone(package_name_from_source(d.source))
        }
    }
    return true
}

def private extract_modules(pkg_dir : string; var manifest : PackageManifest) {
    let das_module = "{pkg_dir}/.das_module"
    if (!fexist(das_module)) {
        return
    }
    // parse register_native_path calls from .das_module
    let text = fread(das_module)
    if (empty(text)) {
        return
    }
    // simple text parsing: find register_native_path("name", ...) lines
    var rest = text
    while (true) {
        let pos = find(rest, "register_native_path(\"")
        if (pos < 0) {
            break
        }
        let start = pos + length("register_native_path(\"")
        let end_quote = find(rest, "\"", start)
        if (end_quote < 0) {
            break
        }
        let mod_name = slice(rest, start, end_quote)
        if (!empty(mod_name)) {
            manifest.modules |> push_clone(mod_name)
        }
        rest = slice(rest, end_quote + 1)
    }
}

def get_git_remote_url(pkg_dir : string) : string {
    var output : string
    let exit_code = run_cmd("git -C \"{pkg_dir}\" remote get-url origin 2>&1", output)
    if (exit_code != 0) {
        return ""
    }
    var url = strip(output)
    if (starts_with(url, "git@")) {
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

// ── JSON serialization ──────────────────────────────────────────────

def private entry_to_jv(name : string; entry : IndexEntry) : JsonValue? {
    var obj <- JV({"name" => JV(name), "url" => JV(entry.url), "description" => JV(entry.description)})
    if (!empty(entry.author)) {
        unsafe {
            (obj.value as _object)["author"] = JV(entry.author)
        }
    }
    if (!empty(entry.license)) {
        unsafe {
            (obj.value as _object)["license"] = JV(entry.license)
        }
    }
    if (length(entry.tags) > 0) {
        var arr : array<JsonValue?>
        for (t in entry.tags) {
            arr |> push(JV(t))
        }
        unsafe {
            (obj.value as _object)["tags"] = JV(arr)
        }
    }
    if (!empty(entry.min_sdk)) {
        unsafe {
            (obj.value as _object)["min_sdk"] = JV(entry.min_sdk)
        }
    }
    if (length(entry.modules) > 0) {
        var arr : array<JsonValue?>
        for (m in entry.modules) {
            arr |> push(JV(m))
        }
        unsafe {
            (obj.value as _object)["modules"] = JV(arr)
        }
    }
    if (!empty(entry.latest_version)) {
        unsafe {
            (obj.value as _object)["latest_version"] = JV(entry.latest_version)
        }
    }
    if (!empty(entry.last_updated)) {
        unsafe {
            (obj.value as _object)["last_updated"] = JV(entry.last_updated)
        }
    }
    if (entry.has_native) {
        unsafe {
            (obj.value as _object)["has_native"] = JV(true)
        }
    }
    if (length(entry.dependencies) > 0) {
        var arr : array<JsonValue?>
        for (d in entry.dependencies) {
            arr |> push(JV(d))
        }
        unsafe {
            (obj.value as _object)["dependencies"] = JV(arr)
        }
    }
    return <- obj
}

def private manifest_to_entry(manifest : PackageManifest; url : string) : IndexEntry {
    var entry : IndexEntry
    entry.url = url
    entry.description = manifest.description
    entry.author = manifest.author
    entry.license = manifest.license
    entry.min_sdk = manifest.min_sdk
    entry.has_native = manifest.has_native
    for (t in manifest.tags) {
        entry.tags |> push_clone(t)
    }
    for (m in manifest.modules) {
        entry.modules |> push_clone(m)
    }
    for (d in manifest.dependencies) {
        entry.dependencies |> push_clone(d)
    }
    return <- entry
}

def serialize_index(index : table<string; IndexEntry>) : string {
    // collect names, sort, then look up via values iteration
    var name_to_jv : array<tuple<string; JsonValue?>>
    for (name, entry in keys(index), values(index)) {
        var jv <- entry_to_jv(name, entry)
        name_to_jv |> emplace(name => jv)
    }
    sort(name_to_jv) <| $(a, b : tuple<string; JsonValue?>) : bool {
        return a._0 < b._0
    }
    var arr : array<JsonValue?>
    for (p in name_to_jv) {
        arr |> emplace(p._1)
    }
    return write_json(JV(arr))
}

// ── README generation ───────────────────────────────────────────────

def generate_index_readme(packages_json : string) : string {
    var index : table<string; IndexEntry>
    if (!parse_index_json(packages_json, index)) {
        return ""
    }
    return generate_index_readme_from_index(index)
}

def generate_index_readme_from_index(index : table<string; IndexEntry>) : string {
    // build sorted list of (name, entry) pairs for use inside build_string block
    var sorted_names : array<string>
    var sorted_entries : array<IndexEntry>
    for (name, entry in keys(index), values(index)) {
        sorted_names |> push_clone(name)
        sorted_entries |> push_clone(entry)
    }
    // sort both arrays by name
    var order : array<int>
    for (i in range(length(sorted_names))) {
        order |> push(i)
    }
    sort(order) <| $(a, b : int) : bool {
        return sorted_names[a] < sorted_names[b]
    }
    var names : array<string>
    var entries : array<IndexEntry>
    for (i in order) {
        names |> push_clone(sorted_names[i])
        entries |> push_clone(sorted_entries[i])
    }

    // collect all tags -> package names
    var tag_to_pkgs : array<string>   // "tag\tpkg1, pkg2"
    {
        var all_tags : table<string; array<string>>
        for (name, entry in names, entries) {
            for (t in entry.tags) {
                unsafe {
                    if (!(all_tags |> key_exists(t))) {
                        var arr : array<string>
                        arr |> push_clone(name)
                        all_tags[t] <- arr
                    } else {
                        all_tags[t] |> push_clone(name)
                    }
                }
            }
        }
        var tag_names : array<string>
        for (t in keys(all_tags)) {
            tag_names |> push_clone(t)
        }
        sort(tag_names) <| $(a, b : string) : bool {
            return a < b
        }
        for (t in tag_names) {
            if (all_tags |> key_exists(t)) {
                unsafe {
                    tag_to_pkgs |> push_clone("{t}\t{join(all_tags[t], ", ")}")
                }
            }
        }
    }

    return build_string() <| $(var writer) {
        writer |> write("# daspkg package index\n\n")
        writer |> write("Package registry for [daspkg](https://github.com/GaijinEntertainment/daScript), the daslang package manager.\n\n")

        // module index
        var has_modules = false
        for (entry in entries) {
            if (length(entry.modules) > 0) {
                has_modules = true
                break
            }
        }
        if (has_modules) {
            writer |> write("## Module Index\n\n")
            writer |> write("| Module | Package | Install |\n")
            writer |> write("|--------|---------|--------|\n")
            for (name, entry in names, entries) {
                for (m in entry.modules) {
                    writer |> write("| `{m}` | {name} | `daspkg install {entry.url}` |\n")
                }
            }
            writer |> write("\n")
        }

        // packages by tag
        if (length(tag_to_pkgs) > 0) {
            writer |> write("## Packages by Tag\n\n")
            for (tp in tag_to_pkgs) {
                let tab_pos = find(tp, "\t")
                if (tab_pos > 0) {
                    let tag_name = slice(tp, 0, tab_pos)
                    let pkg_list = slice(tp, tab_pos + 1)
                    writer |> write("**{tag_name}**: {pkg_list}\n\n")
                }
            }
        }

        // per-package details
        writer |> write("## Packages\n\n")
        for (name, entry in names, entries) {
            writer |> write("### {name}\n\n")
            writer |> write("{entry.description}\n\n")
            var details : array<string>
            if (!empty(entry.author)) {
                details |> push_clone("**Author:** {entry.author}")
            }
            if (!empty(entry.license)) {
                details |> push_clone("**License:** {entry.license}")
            }
            if (length(entry.modules) > 0) {
                let mods = join(entry.modules, "`, `")
                details |> push_clone("**Modules:** `{mods}`")
            }
            if (length(entry.tags) > 0) {
                details |> push_clone("**Tags:** {join(entry.tags, ", ")}")
            }
            if (!empty(entry.min_sdk)) {
                details |> push_clone("**Min SDK:** {entry.min_sdk}")
            }
            if (!empty(entry.latest_version)) {
                details |> push_clone("**Version:** {entry.latest_version}")
            }
            if (!empty(entry.last_updated)) {
                details |> push_clone("**Updated:** {entry.last_updated}")
            }
            if (entry.has_native) {
                details |> push_clone("**Native:** yes (requires C/C++ toolchain)")
            }
            if (length(entry.dependencies) > 0) {
                details |> push_clone("**Dependencies:** {join(entry.dependencies, ", ")}")
            }
            for (d in details) {
                writer |> write("- {d}\n")
            }
            writer |> write("- **Install:** `daspkg install {entry.url}`\n")
            writer |> write("- **URL:** [{entry.url}](https://{entry.url})\n")
            writer |> write("\n---\n\n")
        }

        writer |> write("_{length(names)} package(s) registered._\n\n")
        writer |> write("## Adding a package\n\n")
        writer |> write("```\ndaspkg introduce github.com/user/repo\n```\n\n")
        writer |> write("This validates the package manifest (`.das_package`) and creates a PR.\n")
    }
}

// ── Index manipulation ──────────────────────────────────────────────

def add_to_index_json(index_text, name, url, description : string) : string {
    var index : table<string; IndexEntry>
    if (!parse_index_json(index_text, index)) {
        return ""
    }
    var entry : IndexEntry
    entry.url = url
    entry.description = description
    unsafe {
        index[name] := entry
    }
    return serialize_index(index)
}

def add_to_index_json_rich(index_text : string; name : string; manifest : PackageManifest; url : string) : string {
    var index : table<string; IndexEntry>
    if (!parse_index_json(index_text, index)) {
        return ""
    }
    unsafe {
        index[name] := manifest_to_entry(manifest, url)
    }
    return serialize_index(index)
}

def remove_from_index_json(index_text, name : string) : string {
    var index : table<string; IndexEntry>
    if (!parse_index_json(index_text, index)) {
        return ""
    }
    if (index |> key_exists(name)) {
        index |> erase(name)
    }
    return serialize_index(index)
}

// ── introduce / withdraw ────────────────────────────────────────────

def cmd_introduce(root, source : string) {
    // step 1: determine package directory and URL
    var pkg_dir : string
    var pkg_url : string
    if (empty(source)) {
        pkg_dir = root
        pkg_url = get_git_remote_url(root)
        if (empty(pkg_url)) {
            print("Error: could not determine git remote URL. Pass a URL explicitly or run from a git repo.\n")
            return
        }
    } else {
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

    // step 2: validate manifest (now reads full metadata)
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
    run_cmd("git -C \"{cache_dir}\" checkout main 2>&1", output)
    run_cmd("git -C \"{cache_dir}\" fetch origin 2>&1", output)
    run_cmd("git -C \"{cache_dir}\" reset --hard origin/main 2>&1", output)
    run_cmd("git -C \"{cache_dir}\" checkout -B \"{branch_name}\" 2>&1", output)

    let packages_file = "{cache_dir}/packages.json"
    let old_text = fread(packages_file)
    let new_text = add_to_index_json_rich(old_text, manifest.name, manifest, pkg_url)
    if (empty(new_text)) {
        print("Error: failed to modify packages.json\n")
        run_cmd("git -C \"{cache_dir}\" checkout main 2>&1", output)
        return
    }
    fwrite(packages_file, "{new_text}\n")

    let readme = generate_index_readme(new_text)
    if (!empty(readme)) {
        fwrite("{cache_dir}/README.md", readme)
    }

    run_cmd("git -C \"{cache_dir}\" add packages.json README.md 2>&1", output)
    run_cmd("git -C \"{cache_dir}\" commit -m \"Add {manifest.name}\" 2>&1", output)
    var exit_code = run_cmd("git -C \"{cache_dir}\" push --force origin \"{branch_name}\" 2>&1", output)
    if (exit_code != 0) {
        print("Error: failed to push branch: {output}\n")
        run_cmd("git -C \"{cache_dir}\" checkout main 2>&1", output)
        return
    }

    let pr_out_file = "{cache_dir}/_pr_output.txt"
    exit_code = run_cmd("gh pr create --repo borisbat/daspkg-index --base main --head \"{branch_name}\" --title \"Add package: {manifest.name}\" --body \"Introduce **{manifest.name}** — {manifest.description}\n\nURL: `{pkg_url}`\" > \"{pr_out_file}\" 2>&1", output)
    var pr_output : string
    fopen(pr_out_file, "rb") <| $(f) {
        if (f != null) {
            pr_output = fread(f)
        }
    }
    remove(pr_out_file)
    if (exit_code != 0) {
        print("Error: failed to create PR: {pr_output}{output}\n")
    } else {
        print("PR created: {strip(pr_output)}\n")
    }

    run_cmd("git -C \"{cache_dir}\" checkout main 2>&1", output)
}

def cmd_withdraw(root, name : string) {
    var idx : table<string; IndexEntry>
    if (!fetch_index(root, idx)) {
        print("Error: failed to fetch package index\n")
        return
    }
    if (!(idx |> key_exists(name))) {
        print("Error: package '{name}' is not in the index\n")
        return
    }

    let cache_dir = get_index_cache_dir(root)
    let branch_name = "withdraw-{name}"

    var output : string
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

    let pr_out_file = "{cache_dir}/_pr_output.txt"
    exit_code = run_cmd("gh pr create --repo borisbat/daspkg-index --base main --head \"{branch_name}\" --title \"Remove package: {name}\" --body \"Withdraw **{name}** from the package index.\" > \"{pr_out_file}\" 2>&1", output)
    var pr_output : string
    fopen(pr_out_file, "rb") <| $(f) {
        if (f != null) {
            pr_output = fread(f)
        }
    }
    remove(pr_out_file)
    if (exit_code != 0) {
        print("Error: failed to create PR: {pr_output}{output}\n")
    } else {
        print("PR created: {strip(pr_output)}\n")
    }

    run_cmd("git -C \"{cache_dir}\" checkout main 2>&1", output)
}

// ── update-index ────────────────────────────────────────────────────

def cmd_update_index(root : string) {
    var index : table<string; IndexEntry>
    if (!fetch_index(root, index)) {
        print("Error: failed to fetch package index\n")
        return
    }

    let tmp_base = "{root}/modules/.daspkg_cache/_update_tmp"
    mkdir(tmp_base)

    // collect names first to avoid iterating locked table
    var pkg_names : array<string>
    var pkg_urls : array<string>
    for (name, entry in keys(index), values(index)) {
        pkg_names |> push_clone(name)
        pkg_urls |> push_clone(entry.url)
    }

    var updated = 0
    for (ni in range(length(pkg_names))) {
        let name = pkg_names[ni]
        let url = pkg_urls[ni]
        unsafe {
            print("Updating {name}...\n")
            let tmp_dir = "{tmp_base}/{name}"
            if (fexist(tmp_dir)) {
                rmdir_rec(tmp_dir)
            }
            let git_url = "https://{url}.git"
            var output : string
            let exit_code = run_cmd("git clone --depth 1 \"{git_url}\" \"{tmp_dir}\" 2>&1", output)
            if (exit_code != 0) {
                print("  Warning: failed to clone {url}: {output}\n")
                continue
            }

            // read manifest
            var manifest : PackageManifest
            if (read_manifest(tmp_dir, manifest)) {
                // preserve fields from manifest
                index[name].description = manifest.description
                if (!empty(manifest.author)) {
                    index[name].author = manifest.author
                }
                if (!empty(manifest.license)) {
                    index[name].license = manifest.license
                }
                if (!empty(manifest.min_sdk)) {
                    index[name].min_sdk = manifest.min_sdk
                }
                if (length(manifest.tags) > 0) {
                    delete index[name].tags
                    for (t in manifest.tags) {
                        index[name].tags |> push_clone(t)
                    }
                }
                if (length(manifest.modules) > 0) {
                    delete index[name].modules
                    for (m in manifest.modules) {
                        index[name].modules |> push_clone(m)
                    }
                }
                index[name].has_native = manifest.has_native
                if (length(manifest.dependencies) > 0) {
                    delete index[name].dependencies
                    for (d in manifest.dependencies) {
                        index[name].dependencies |> push_clone(d)
                    }
                }
            }

            // get latest tag from git
            var tag_output : string
            let tag_exit = run_cmd("git -C \"{tmp_dir}\" describe --tags --abbrev=0 2>&1", tag_output)
            if (tag_exit == 0 && !empty(strip(tag_output))) {
                index[name].latest_version = strip(tag_output)
            }

            // get last commit date
            var date_output : string
            let date_exit = run_cmd("git -C \"{tmp_dir}\" log -1 --format=%cs 2>&1", date_output)
            if (date_exit == 0 && !empty(strip(date_output))) {
                index[name].last_updated = strip(date_output)
            }

            // GitHub API fallback for license if not specified in manifest
            if (empty(index[name].license)) {
                let gh_license = github_get_license(url)
                if (!empty(gh_license)) {
                    index[name].license = gh_license
                }
            }

            // GitHub API fallback for author if not specified
            if (empty(index[name].author)) {
                let gh_author = github_get_owner(url)
                if (!empty(gh_author)) {
                    index[name].author = gh_author
                }
            }

            rmdir_rec(tmp_dir)
            updated ++
        }
    }

    rmdir_rec(tmp_base)

    // write updated index
    let cache_dir = get_index_cache_dir(root)
    let branch_name = "update-index"

    var output : string
    run_cmd("git -C \"{cache_dir}\" checkout main 2>&1", output)
    run_cmd("git -C \"{cache_dir}\" fetch origin 2>&1", output)
    run_cmd("git -C \"{cache_dir}\" reset --hard origin/main 2>&1", output)
    run_cmd("git -C \"{cache_dir}\" checkout -B \"{branch_name}\" 2>&1", output)

    let new_text = serialize_index(index)
    let packages_file = "{cache_dir}/packages.json"
    fwrite(packages_file, "{new_text}\n")

    let readme = generate_index_readme_from_index(index)
    if (!empty(readme)) {
        fwrite("{cache_dir}/README.md", readme)
    }

    run_cmd("git -C \"{cache_dir}\" add packages.json README.md 2>&1", output)
    run_cmd("git -C \"{cache_dir}\" commit -m \"Update index metadata\" 2>&1", output)
    var exit_code = run_cmd("git -C \"{cache_dir}\" push --force origin \"{branch_name}\" 2>&1", output)
    if (exit_code != 0) {
        print("Error: failed to push branch: {output}\n")
        run_cmd("git -C \"{cache_dir}\" checkout main 2>&1", output)
        return
    }

    let pr_out_file = "{cache_dir}/_pr_output.txt"
    exit_code = run_cmd("gh pr create --repo borisbat/daspkg-index --base main --head \"{branch_name}\" --title \"Update index metadata\" --body \"Refreshed metadata for {updated} package(s).\" > \"{pr_out_file}\" 2>&1", output)
    var pr_output : string
    fopen(pr_out_file, "rb") <| $(f) {
        if (f != null) {
            pr_output = fread(f)
        }
    }
    remove(pr_out_file)
    if (exit_code != 0) {
        print("Error: failed to create PR: {pr_output}{output}\n")
    } else {
        print("PR created: {strip(pr_output)}\n")
    }

    run_cmd("git -C \"{cache_dir}\" checkout main 2>&1", output)
    print("Updated {updated} package(s)\n")
}

// ── GitHub API helpers ──────────────────────────────────────────────

def private github_get_license(url : string) : string {
    // url is like "github.com/user/repo"
    // use gh api to get license
    var output : string
    let exit_code = run_cmd("gh api repos/{slice(url, length("github.com/"))} --jq .license.spdx_id 2>&1", output)
    if (exit_code == 0) {
        let result = strip(output)
        if (result != "null" && result != "NOASSERTION" && !empty(result)) {
            return result
        }
    }
    return ""
}

def private github_get_owner(url : string) : string {
    // extract owner from "github.com/user/repo"
    let path = slice(url, length("github.com/"))
    let slash = find(path, "/")
    if (slash > 0) {
        return slice(path, 0, slash)
    }
    return ""
}
