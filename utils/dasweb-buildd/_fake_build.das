options gen2
options indenting = 4

require daslib/fio
require strings

// Test fixture, not a [test] suite: a cross-platform stand-in for run_build.sh
// with the same contract — `daslang _fake_build.das -- <src> <out> <mode> <entry>`.
// Produces sample.wasm (plus a page pair in page mode) from the entry source;
// a source containing FAIL_ME exits nonzero with an error on stdout, the way
// a real compile failure would.

[export]
def main : int {
    var args <- get_user_args()
    if (length(args) < 4) {
        print("fake build: expected <src> <out> <mode> <entry>\n")
        return 2
    }
    let src = args[0]
    let out = args[1]
    let mode = args[2]
    let entry = args[3]
    let text = fread(path_join(src, entry))
    if (empty(text)) {
        print("fake build: cannot read entry {entry}\n")
        return 3
    }
    if (find(text, "FAIL_ME") >= 0) {
        print("synthetic compile error: FAIL_ME at {entry}:1:1\n")
        return 1
    }
    fwrite(path_join(out, "sample.wasm"), "WASM:{text}")
    if (mode == "page") {
        fwrite(path_join(out, "index.html"), "<html>{entry}</html>")
        fwrite(path_join(out, "game.js"), "// generated js")
    }
    fwrite(path_join(out, "build.log"), "not an artifact — must be skipped")
    return 0
}
