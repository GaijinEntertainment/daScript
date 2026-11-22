options gen2

module check_cache shared public

require daslib/fio

//! The passes of external checks a test asks for - a validator run
//! over generated output - remembered across runs, so a check that
//! already passed for the same input and the same tool starts no
//! child. A pass is one empty file under the tree's cache folder; a
//! failure is never recorded.

struct ExternalTool {
    exe : string        //! "" when nothing answers to the name
    stamp : string      //! what a check's key carries for the tool
}

var private cache_dir_override : string
var private known_tools : table<string; ExternalTool>

//! the cache's own test points it at its own folder; "" undoes it
def set_check_cache_dir(dir : string) {
    cache_dir_override = dir
}

def private check_cache_dir() : string {
    return cache_dir_override if (!empty(cache_dir_override))
    return path_join(get_das_root(), ".cache/daslang/checks")
}

def passed_marker_path(kind : string; key : uint64) : string =>
    path_join(check_cache_dir(), "{kind}_{key}.passed")

def check_passed(kind : string; key : uint64) : bool => fexist(passed_marker_path(kind, key))

def record_check_passed(kind : string; key : uint64) {
    mkdir_rec(check_cache_dir())
    fopen(passed_marker_path(kind, key), "wb") $(_f) {}
}

//! The SDK's own file is stamped by path, size and time without a
//! child; a name PATH resolves by what its `--version` printed.
def private find_vulkan_sdk_tool(name : string) : ExternalTool {
    if (has_env_variable("VULKAN_SDK")) {
        let sdk = get_env_variable("VULKAN_SDK")
        for (p in ["{sdk}/Bin/{name}.exe", "{sdk}/bin/{name}"]) {
            let st = stat(p)
            if (st.is_valid) return ExternalTool(exe = p, stamp = "{p}|{st.size}|{st.mtime}")
        }
    }
    var version : string
    for (p in [name, "{name}.exe"]) {
        if (run_and_capture([p, "--version"], version, 10.0) == 0) {
            return ExternalTool(exe = p, stamp = "{p}|{version}")
        }
    }
    return ExternalTool()
}

//! A Vulkan SDK tool, looked for once a program.
def vulkan_sdk_tool(name : string) : ExternalTool {
    if (!key_exists(known_tools, name)) {
        known_tools |> insert(name, find_vulkan_sdk_tool(name))
    }
    return known_tools?[name] ?? ExternalTool()
}
