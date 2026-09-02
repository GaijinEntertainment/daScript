options gen2

require strings
require daslib/fio
require daslib/strings_boost
require dastest/review_gate

// The mechanical half of web/REVIEW.md (contract: REVIEW_COMMON.md at the repo root).
// Run from the repo root: bin/daslang web/REVIEW.das - exit 0 clean, 1 with findings, 2 when
// not run from the repo root.

// Every authored tree under this folder that the deploy or `daspkg release wasm` copies
// into a page a visitor loads. Everything else here is emsdk, a build tree, or a generated
// output that no diff hand-edits.
let private DEPLOYED_ROOTS <- ["web/examples/ui/src", "web/examples/ui/samples", "web/templates"]

let private PAGES_YML = ".github/workflows/pages.yml"
let private WEB_CMAKE = "web/CMakeLists.txt"

//! A staged source under this prefix is a build output and is skipped: the tree it is built
//! from is gated where that tree lives.
let private GENERATED_PREFIX = "web/output"

// Empty on purpose: neither shell fetches anything off-site, and a browser opening a page
// built from them contacts only the host that served it.
let private ALLOWED_HOSTS : table<string>

//! The authored `web/` tree a staged source names, truncated at the first shell variable,
//! glob or quote; "" when the source is outside this folder or only a build variable names it.
def private staged_web_tree(source : string) : string {
    var path = source
    if (path |> starts_with("$\{CMAKE_CURRENT_SOURCE_DIR\}/")) {
        path = "web/" + slice(path, length("$\{CMAKE_CURRENT_SOURCE_DIR\}/"))
    }
    return "" if (!(path |> starts_with("web/")) || find(path, "/../") >= 0)
    var tree = ""
    peek_data(path) $(d) {
        var cut = length(d)
        for (i in range(length(d))) {
            let c = int(d[i])
            if (c == '$' || c == '*' || c == '"' || c == '\'') {
                cut = i
                break
            }
        }
        while (cut > 0 && int(d[cut - 1]) == '/') {
            cut--
        }
        tree = slice(d, 0, cut)
    }
    // a step that copies one file names that file; the tree the gate walks is its folder
    if (fexist(tree) && !stat(tree).is_dir) {
        tree = dir_name(tree)
    }
    return tree
}

def private is_under_deployed_root(tree : string) : bool {
    for (root in DEPLOYED_ROOTS) {
        return true if (tree == root || tree |> starts_with(root + "/"))
    }
    return false
}

def private check_staged_sources(file : string; sources : array<string>) {
    let text = fread(file)
    if (empty(text)) {
        gate_finding(file, "missing or unreadable - the staged-tree check read nothing, which is not a pass")
        return
    }
    for (src in sources) {
        let tree = staged_web_tree(src)
        continue if (empty(tree) || tree |> starts_with(GENERATED_PREFIX))
        if (!is_under_deployed_root(tree)) {
            gate_finding(file, find_line(text, src),
                "stages {tree} into a served page, and no DEPLOYED_ROOTS entry in web/REVIEW.das covers it - add the tree to the gate's root list so its files are read")
        }
    }
}

//! The deploy's cp steps land in `_site/`; the CMake stage target names its copy
//! destinations `SITE_*`.
//! The three staging shapes the gate parses: a `cp` whose destination starts with `_site/`
//! in the deploy, and a `-E copy` / `copy_directory` whose destination contains `SITE_` or
//! `/site/` in the CMake stage target. A step of another shape is one the gate does not read.
def private check_staged_trees_gated {
    check_staged_sources(PAGES_YML, shell_cp_sources(fread(PAGES_YML), "_site/"))
    let cmake = fread(WEB_CMAKE)
    check_staged_sources(WEB_CMAKE, cmake_copy_sources(cmake, "SITE_"))
    check_staged_sources(WEB_CMAKE, cmake_copy_sources(cmake, "/site/"))
}

[export]
def main() : int {
    for (root in DEPLOYED_ROOTS) {
        if (!fexist(root)) {
            to_log(LOG_ERROR, "web/REVIEW.das: run from the repo root\n")
            return 2
        }
    }
    check_staged_trees_gated()
    let no_skipped_dirs : table<string>
    gate_web_third_party(DEPLOYED_ROOTS, no_skipped_dirs, ALLOWED_HOSTS)
    return gate_verdict("web")
}
