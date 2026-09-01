options gen2

require strings
require daslib/fio
require daslib/strings_boost
require dastest/review_gate

// The mechanical half of web/REVIEW.md (contract: REVIEW_COMMON.md at the repo root).
// Run from the repo root: bin/daslang web/REVIEW.das - exit 0 clean, 1 with findings, 2 when
// not run from the repo root.

// Every authored tree under this folder that the deploy or `daspkg release --web` copies
// into a page a visitor loads. Everything else here is emsdk, a build tree, or a generated
// output that no diff hand-edits.
let private DEPLOYED_ROOTS <- ["web/examples/ui/src", "web/examples/ui/samples", "web/templates"]

// The two routes that stage a tree under this folder into a served page: the deploy's
// shell steps copy into _site/, and the CMake stage target copies into site/ (its
// destinations name SITE_*). A build output is not an authored tree - what it is built
// from is gated where it lives - so a source under an output dir is skipped.
let private PAGES_YML = ".github/workflows/pages.yml"
let private WEB_CMAKE = "web/CMakeLists.txt"
let private GENERATED_PREFIX = "web/output"

// Empty on purpose: neither shell fetches anything off-site, and a browser opening a page
// built from them contacts only the host that served it.
let private ALLOWED_HOSTS : table<string>

// A staged source, reduced to the authored tree it names: `${CMAKE_CURRENT_SOURCE_DIR}/x`
// is `web/x`, a shell variable, glob or quote ends the path, a trailing separator goes. "" for a
// source outside this folder or one only a build variable names.
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
    for (src in sources) {
        let tree = staged_web_tree(src)
        continue if (empty(tree) || tree |> starts_with(GENERATED_PREFIX))
        if (!is_under_deployed_root(tree)) {
            gate_finding(file, find_line(text, src),
                "stages {tree} into a served page, and no DEPLOYED_ROOTS entry in web/REVIEW.das covers it - add the tree to the gate's root list so its files are read")
        }
    }
}

def private check_staged_trees_gated {
    check_staged_sources(PAGES_YML, shell_cp_sources(fread(PAGES_YML), "_site/"))
    check_staged_sources(WEB_CMAKE, cmake_copy_sources(fread(WEB_CMAKE), "SITE_"))
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
