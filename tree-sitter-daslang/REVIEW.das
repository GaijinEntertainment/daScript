options gen2

require strings
require daslib/strings_boost
require daslib/fio
require dastest/review_gate

// The mechanical half of tree-sitter-daslang/REVIEW.md (contract: REVIEW_COMMON.md at the repo
// root). Run from the repo root: bin/daslang tree-sitter-daslang/REVIEW.das - exit 0 clean, 1 with
// findings. Regenerates the grammar in a scratch copy and diffs the tracked artifacts byte for
// byte, then runs the corpus; both need the tree-sitter CLI on PATH (`npm i -g tree-sitter-cli`).

let FOLDER = "tree-sitter-daslang"
let GENERATED <- [ "src/parser.c", "src/grammar.json", "src/node-types.json" ]

def private shell(cmd : string) : tuple<rc : int; out : string> {
    var output : string
    let rc = unsafe(popen(cmd) $(f) {
        if (f != null) {
            output = fread(f)
        }
    })
    return (rc = rc, out = output)
}

def private read_all(path : string) : string {
    var text = ""
    fopen(path, "rb") $(f) {
        if (f != null) {
            text = fread(f)
        }
    }
    return text
}

def private copy_tree(src : string; dst : string) : bool {
    // grammar.js, package.json, src/, test/ - everything `generate` and `test` read; the built
    // grammar dylib and node bindings stay behind
    var ok = mkdir_rec(dst)
    dir(src) $(name) {
        if (name == "." || name == ".." || !ok) return
        let from = path_join(src, name)
        let to = path_join(dst, name)
        var st : FStat
        if (!stat(from, st)) return
        if (st.is_dir) {
            if (name != "node_modules" && name != "bindings" && name != "build" && name != "zed-daslang") {
                ok = copy_tree(from, to) && ok
            }
        } elif (name |> ends_with(".js") || name |> ends_with(".json") || name |> ends_with(".c") || name |> ends_with(".h") || name |> ends_with(".txt") || name |> ends_with(".scm")) {
            var err : string
            if (!copy_file(from, to, true, err)) {
                ok = false
            }
        }
    }
    return ok
}

def private cd_and(dir_path : string; cmd : string) : string {
    let win = get_platform_name() == "windows"
    return win ? "cd /d \"{dir_path}\" && {cmd} 2>&1" : "cd \"{dir_path}\" && {cmd} 2>&1"
}

[export]
def main() : int {
    let probe = shell("tree-sitter --version 2>&1")
    if (probe.rc != 0) {
        gate_finding("{FOLDER}/grammar.js", "tree-sitter CLI not on PATH - `npm i -g tree-sitter-cli`; the gate regenerates the grammar and runs test/corpus")
        return gate_verdict("tree-sitter-daslang")
    }
    var err : string
    let scratch = create_temp_directory("tsdaslang", err)
    if (empty(scratch)) {
        gate_finding("{FOLDER}", "cannot create a scratch directory: {err}")
        return gate_verdict("tree-sitter-daslang")
    }
    if (!copy_tree(FOLDER, scratch)) {
        gate_finding("{FOLDER}", "cannot copy the grammar folder to {scratch}")
        return gate_verdict("tree-sitter-daslang")
    }
    let gen = shell(cd_and(scratch, "tree-sitter generate"))
    if (gen.rc != 0) {
        gate_finding("{FOLDER}/grammar.js", "tree-sitter generate failed: {gen.out}")
        return gate_verdict("tree-sitter-daslang")
    }
    for (rel in GENERATED) {
        let tracked = read_all(path_join(FOLDER, rel))
        let fresh = read_all(path_join(scratch, rel))
        if (tracked != fresh) {
            gate_finding("{FOLDER}/{rel}", "differs from what `tree-sitter generate` writes for grammar.js - regenerate and commit the artifact")
        }
    }
    let tst = shell(cd_and(scratch, "tree-sitter test"))
    if (tst.rc != 0) {
        var shown = 0
        for (line in split(tst.out, "\n")) {
            if (find(line, "✗") >= 0 || find(line, "failed") >= 0 || find(line, "Error") >= 0) {
                if (shown < 12) {
                    gate_finding("{FOLDER}/test/corpus", strip(line))
                }
                shown++
            }
        }
        if (shown == 0) {
            gate_finding("{FOLDER}/test/corpus", "tree-sitter test failed (exit {tst.rc})")
        }
    }
    return gate_verdict("tree-sitter-daslang")
}
