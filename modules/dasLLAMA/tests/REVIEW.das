options gen2

require dastest/review_gate
require dastest/test_shape
require daslib/fio
require strings
require daslib/strings_boost
require _model_categories
require ../../../utils/common/text_lines.das   // split_trimmed: a table cell's comma list

// The mechanical half of modules/dasLLAMA/tests/REVIEW_FILES.md
// (contract: REVIEW_COMMON.md at the repo root). Run from the repo
// root: bin/daslang modules/dasLLAMA/tests/REVIEW.das - exit 0
// clean, 1 with findings. The root is judged tests-only: run.das,
// the runner, is a script beside the suites it runs.

//! the programs a child process of this folder runs as a program of its own, each for a claim a verb of the test
//! child cannot carry
let private OWN_ROOTS = fixed_array("_child_root.das", "_tuned_fallbacks_root.das", "_exe_smoke_root.das", "_box_runtime_root.das",
    "_audio_lane_whisper_root.das", "_mx4_mint_root.das", "_fake_chat_root.das", "run.das",
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

let private CATEGORIES_DOC = "modules/dasLLAMA/TEST_CATEGORIES.md"

let private ONE_CATEGORY_ROW = ("a model category is one row of `model_categories` (`_model_categories.das`) and one table row "
    + "under `## Categories` of TEST_CATEGORIES.md holding the category's name, each stem of its model files and its topics word "
    + "for word - add, change or drop the two in the same change (ARCHITECTURE_ENGINE_SUPPORT.md#test-model-filters)")

//! the categories document and the categories table name the same categories, each with its stems and topics
[arch(at = "../ARCHITECTURE_ENGINE_SUPPORT.md#test-model-filters")]
def private check_test_categories() {
    var inscope rows <- markdown_table_rows(fread(CATEGORIES_DOC), "Categories")
    var known : table<string>
    for (cat in model_categories()) {
        known |> insert(cat.name)
        var row_agrees = false
        for (row in rows) {
            continue if (length(row) < 3 || row[0] != cat.name)
            var inscope stems <- split_trimmed(row[1], ",")
            row_agrees = row[2] == cat.topics
            for (stem in split_trimmed(cat.stems, ",")) {
                row_agrees &&= has_value(stems, stem)
            }
        }
        if (!row_agrees) {
            gate_finding(CATEGORIES_DOC, "{ONE_CATEGORY_ROW} (the category {cat.name})")
        }
    }
    for (row, at in rows, count()) {
        if (at > 0 && !empty(row) && !key_exists(known, row[0])) {
            gate_finding(CATEGORIES_DOC, "{ONE_CATEGORY_ROW} (the row {row[0]})")
        }
    }
}

let private MODEL_ONCE = ("a model file loads once a form in an area's run: `with_cpu_and_gpu_model` is called once an area and model "
    + "file across this folder, its forms the call's list - fold the cells of this call, and the forms they need, into the first "
    + "call (ARCHITECTURE_ENGINE_SUPPORT.md#test-load-once)")

let private MODEL_LITERAL = ("`with_cpu_and_gpu_model` takes its run area and its model file as string literals - `t, \"llm\", "
    + "\"<file>.gguf\", ...`: this gate reads them to hold one call an area and model across the folder "
    + "(ARCHITECTURE_ENGINE_SUPPORT.md#test-load-once)")

//! one call of `with_cpu_and_gpu_model` an area and model file across the folder's test files: a second call naming the same
//! area and file, in any file and whatever its other arguments, is a second full load of a form; every call names its area and
//! file by literals on the call's first line
[arch(at = "../ARCHITECTURE_ENGINE_SUPPORT.md#test-load-once")]
def private check_models_load_once() {
    var seen : table<string; string>
    for (f in tracked_files(["modules/dasLLAMA/tests/test_*.das"], "the model blocks")) {
        let text = fread(f)
        var inscope calls <- split(text, "with_cpu_and_gpu_model(t, ")
        calls |> erase(0)
        for (tail in calls) {
            let eol = find(tail, "\n")
            let head = eol >= 0 ? slice(tail, 0, eol) : tail
            let at = find_line(text, "with_cpu_and_gpu_model(t, {head}")
            var inscope lits <- split(head, "\"")   //! `"area", "file", rest`: empty, the area, the comma, the file, the rest
            if (!(head |> starts_with("\"")) || length(lits) < 5 || strip(lits[2]) != ",") {
                gate_finding(f, at, MODEL_LITERAL)
                continue
            }
            let key = "{lits[1]}, {lits[3]}"
            if (key_exists(seen, key)) {
                gate_finding(f, at, "{MODEL_ONCE} - the first call is in {seen?[key] ?? ""}")
            } else {
                seen |> insert(key, base_name(f))
            }
        }
    }
}

[export]
def main() : int {
    check_test_shape("modules/dasLLAMA/tests", 100, true)
    check_one_test_child()
    check_test_categories()
    check_models_load_once()
    return gate_verdict("modules/dasLLAMA/tests")
}
