options gen2

require dastest/review_gate
require dastest/test_shape
require daslib/fio
require strings
require daslib/strings_boost

// The mechanical half of modules/dasLLAMA/tests/REVIEW_FILES.md
// (contract: REVIEW_COMMON.md at the repo root). Run from the repo
// root: bin/daslang modules/dasLLAMA/tests/REVIEW.das - exit 0
// clean, 1 with findings. The root is judged tests-only: run.das,
// the runner, is a script beside the suites it runs.

//! the programs a child process of this folder runs as a program of its own, each for a claim a verb of the test
//! child cannot carry
let private OWN_ROOTS = fixed_array("_child_root.das", "_tuned_fallbacks_root.das", "_exe_smoke_root.das", "_box_runtime_root.das",
    "_resident_load_root.das", "_audio_lane_whisper_root.das", "_mx4_mint_root.das", "_fake_chat_root.das", "run.das",
    "dastest.das")

//! the test files that build a child's command line on a root they name themselves (`child_cmd_root`): the
//! module-cache test, whose root is a copy it writes
let private ROOT_CMD_CALLERS = fixed_array("test_f32_batch_tile_perms.das")

let private ONE_CHILD = ("a child process of this folder is a verb of the test child (`_child_root.das` dispatching a `_child_<verb>.das` "
    + "module, spawned through `child_verb_cmd`) - a program of its own is licensed only for a claim a verb cannot carry: "
    + "the program's own compile, the process it runs in, a model a stocked test loads, or a server or runner started by "
    + "its command line (ARCHITECTURE_ENGINE_SUPPORT.md#test-child)")

//! the literal first argument of every `call("` in `text`, with the 1-based line of each
def private literal_roots(text, call : string) : array<tuple<root : string; line : int>> {
    var found : array<tuple<root : string; line : int>>
    var inscope calls <- split(text, "{call}(\"")
    calls |> erase(0)
    for (tail in calls) {
        let close = find(tail, "\"")
        if (close > 0) {
            let root = slice(tail, 0, close)
            found |> push((root = root, line = find_line(text, "{call}(\"{root}")))
        }
    }
    return <- found
}

//! every child process a test of this folder spawns is the one test child, or a licensed program: no root file, no
//! `child_cmd` / `child_cmd_at` naming a root, and no `child_cmd_root` caller outside the licensed sets
[arch(at = "../ARCHITECTURE_ENGINE_SUPPORT.md#test-child")]
def private check_one_test_child() {
    for (f in tracked_files(["modules/dasLLAMA/tests/_*_root.das"], "the test child's roots")) {
        if (!has_value(OWN_ROOTS, base_name(f))) {
            gate_finding(f, ONE_CHILD)
        }
    }
    for (f in tracked_files(["modules/dasLLAMA/tests/*.das"], "the test child's spawns")) {
        continue if (base_name(f) == "REVIEW.das" || base_name(f) == "_child_run.das")   //! the gate's needles, the helpers' own bodies
        let text = fread(f)
        for (call in ["child_cmd", "child_cmd_at"]) {
            for (r in literal_roots(text, call)) {
                if (!has_value(OWN_ROOTS, base_name(r.root))) {
                    gate_finding(f, r.line, ONE_CHILD)
                }
            }
        }
        if (find(text, "child_cmd_root(") >= 0 && !has_value(ROOT_CMD_CALLERS, base_name(f))) {
            gate_finding(f, find_line(text, "child_cmd_root("), ONE_CHILD)
        }
    }
}

[export]
def main() : int {
    check_test_shape("modules/dasLLAMA/tests", 100, true)
    check_one_test_child()
    return gate_verdict("modules/dasLLAMA/tests")
}
