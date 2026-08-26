options gen2

require strings
require daslib/fio
require dastest/review_gate

// The mechanical half of nano/REVIEW.md (contract: REVIEW_COMMON.md at the repo root).
// Run from the repo root: bin/daslang nano/REVIEW.das - exit 0 clean, 1 with findings.

let NANO_CMAKE = "nano/CMakeLists.txt"

// Directories a checkout carries but git does not: build output, tool caches, and the
// nested worktrees agents run in. Walking them costs minutes and finds nothing.
let UNTRACKED_DIRS = {
    ".git", ".claude", ".codex", ".vs", "build", "bin", "lib",
    "node_modules", "out", "__pycache__", "_build"
}

def private walk_cmakelists(root : string; var out : array<string>) {
    dir(root) $(name) {
        return if (name == "." || name == "..")
        let p = root == "." ? name : path_join(root, name)
        let st = stat(p)
        return if (!st.is_valid)
        if (st.is_dir) {
            return if (key_exists(UNTRACKED_DIRS, name))
            walk_cmakelists(p, out)
        } elif (name == "CMakeLists.txt" || name == "CMakeLists.standalone.cmake") {
            // the .standalone.cmake variants are installed AS CMakeLists.txt, so
            // they carry the same obligation an SDK user will inherit
            out |> push(to_generic_path(p))
        }
    }
}

// Sources the nano library compiles out of the shared tree, as CMake lists them.
def private nano_shared_sources(text : string) : array<string> {
    var inscope entries <- cmake_list_entries(text, "NANO_SHARED_SRC")
    let marker = "}/"
    return <- [for (e in entries); find(e, marker) >= 0 ? slice(e, find(e, marker) + length(marker), length(e)) : e]
}

// Every shared source nano names is a file that is still there. A rename upstream would
// otherwise surface only when somebody builds nano.
def private check_shared_sources(text : string) {
    for (src in nano_shared_sources(text)) {
        continue if (fexist(src))
        gate_finding(NANO_CMAKE, "NANO_SHARED_SRC names {src}, which does not exist - the file moved or was renamed upstream")
    }
}

// A header under nano/include shadows an upstream header or is nano's own. A third case
// is a header nothing in the build will ever reach.
def private check_shadow_headers(folder : string; owned : array<string>) {
    dir(folder) $(name) {
        return if (name == "." || name == "..")
        let path = path_join(folder, name)
        let st = stat(path)
        return if (!st.is_valid)
        if (st.is_dir) {
            check_shadow_headers(path, owned)
            return
        }
        return if (!(name |> ends_with(".h")))
        let generic = to_generic_path(path)
        let upstream = "include/{slice(generic, length("nano/include/"), length(generic))}"
        return if (fexist(upstream) || find_index(owned, name) >= 0)
        gate_finding(generic, "shadows nothing: there is no {upstream}, and it is not one of nano's own headers - either the upstream header moved or this file is unreachable")
    }
}

// Paths the root CMakeLists installs into src/, as install(FILES ...) lists them.
def private installed_src_files(text : string) : array<string> {
    var out : array<string>
    for (blk in cmake_command_blocks(text, "install")) {
        var inscope args <- cmake_args(blk)
        continue if (find_index(args, "FILES") != 0)
        var dest = ""
        for (i in range(1, length(args))) {
            if (args[i] == "DESTINATION" && i + 1 < length(args)) {
                dest = args[i + 1]
            }
        }
        continue if (!(dest |> starts_with("src/")))
        let marker = "}/"
        out |> reserve(length(out) + length(args))
        for (a in args) {
            let at = find(a, marker)
            out |> push(at >= 0 ? slice(a, at + length(marker), length(a)) : a)
        }
    }
    return <- out
}

// An SDK carries no src/ except the sources nano compiles, so a source added to
// NANO_SHARED_SRC and not to the root install list builds here and not from an SDK.
def private check_shared_sources_ship(text : string) {
    let root_cmake = "CMakeLists.txt"
    var inscope installed <- installed_src_files(fread(root_cmake))
    for (src in nano_shared_sources(text)) {
        continue if (find_index(installed, src) >= 0)
        gate_finding(root_cmake, "{src} is in NANO_SHARED_SRC but no install(FILES ...) puts it under src/ - an SDK build of nano would not find it")
    }
}

// The include-order trap: CMake searches a directory's include_directories() before any
// target's own, so a nano-linked target in a directory that inherited the repo's would
// compile against the full runtime's headers while linking nano's library.
def private links_nano(text : string) : bool {
    for (blk in cmake_command_blocks(text, "target_link_libraries")) {
        var inscope args <- cmake_args(blk)
        return true if (find_index(args, "libDaScriptNano") >= 0)
    }
    return false
}

def private check_include_order_guard {
    var files : array<string>
    walk_cmakelists(".", files)
    files |> sort
    for (f in files) {
        let text = fread(f)
        continue if (!links_nano(text) || find(text, "INCLUDE_DIRECTORIES \"\"") >= 0)
        gate_finding(f, "links libDaScriptNano without clearing inherited include directories - add set_property(DIRECTORY PROPERTY INCLUDE_DIRECTORIES \"\"), or the target compiles against the full runtime's headers")
    }
}

[export]
def main() : int {
    if (!fexist(NANO_CMAKE)) {
        to_log(LOG_ERROR, "nano/REVIEW.das: run from the repo root\n")
        return 2
    }
    let cmake_text = fread(NANO_CMAKE)
    check_shared_sources(cmake_text)
    check_shared_sources_ship(cmake_text)
    check_shadow_headers("nano/include", ["nano_print.h"])
    check_include_order_guard()
    return gate_verdict("nano")
}
