options gen2
options rtti
options no_unused_function_arguments = false
options no_unused_block_arguments = false
options no_global_variables = false

require common public
require daslib/strings_boost

// Read packages.json from the local daspkg cache and return matching entries as JSON.
// If refresh="true", fetch fresh from GitHub raw URL first.

let PACKAGES_JSON_URL = "https://raw.githubusercontent.com/borisbat/daspkg-index/main/packages.json"

def private get_packages_json(refresh : bool) : string {
    let cache_path = "{get_das_root()}/modules/.daspkg_cache/index/packages.json"
    if (!refresh && fexist(cache_path)) {
        return fread(cache_path)
    }
    // try fetching from GitHub via curl
    var output : string
    let exit_code = run_and_capture("curl -sL \"{PACKAGES_JSON_URL}\"", output)
    if (exit_code == 0 && !empty(output) && starts_with(strip(output), "[")) {
        return output
    }
    // fall back to cache
    if (fexist(cache_path)) {
        return fread(cache_path)
    }
    return ""
}

def private filter_packages(packages_json, query : string) : string {
    var error : string
    var parsed = read_json(packages_json, error)
    if (parsed == null || !(parsed.value is _array)) {
        return "[]"
    }
    if (empty(query)) {
        // return all
        return packages_json
    }
    let q = to_lower(query)
    var results : array<JsonValue?>
    unsafe {
        for (val in parsed.value as _array) {
            let name = to_lower(val?.name ?? "")
            let desc = to_lower(val?.description ?? "")
            let author = to_lower(val?.author ?? "")
            var tag_match = false
            let tags = val?["tags"]
            if (tags != null && tags.value is _array) {
                for (t in tags.value as _array) {
                    if (t.value is _string && find(to_lower(t.value as _string), q) >= 0) {
                        tag_match = true
                        break
                    }
                }
            }
            var mod_match = false
            let mods = val?["modules"]
            if (mods != null && mods.value is _array) {
                for (m in mods.value as _array) {
                    if (m.value is _string && find(to_lower(m.value as _string), q) >= 0) {
                        mod_match = true
                        break
                    }
                }
            }
            if (find(name, q) >= 0 || find(desc, q) >= 0 || find(author, q) >= 0 || tag_match || mod_match) {
                results |> push_clone(val)
            }
        }
    }
    return write_json(JV(results))
}

def do_package_search(query, refresh_str : string) : string {
    let refresh = refresh_str == "true"
    let json = get_packages_json(refresh)
    if (empty(json)) {
        return make_tool_result("Package index not available. Run 'daspkg search' first to populate the cache, or set refresh=true to fetch from GitHub.", true)
    }
    let filtered = filter_packages(json, query)
    return make_tool_result(filtered)
}

def do_package_info(name, refresh_str : string) : string {
    if (empty(name)) {
        return make_tool_result("missing 'name' argument", true)
    }
    let refresh = refresh_str == "true"
    let json = get_packages_json(refresh)
    if (empty(json)) {
        return make_tool_result("Package index not available", true)
    }
    var error : string
    var parsed = read_json(json, error)
    if (parsed == null || !(parsed.value is _array)) {
        return make_tool_result("Failed to parse index", true)
    }
    unsafe {
        for (val in parsed.value as _array) {
            if ((val?.name ?? "") == name) {
                return make_tool_result(write_json(val))
            }
        }
    }
    return make_tool_result("Package '{name}' not found in index", true)
}
