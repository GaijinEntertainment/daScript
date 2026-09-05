options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

require dastest/testing_boost public
require run   // the suite runner's own tables and filter - bare same-dir require; run.das is [export] main only, no [init]
require daslib/fio
require strings

// The runner's per-PR suite split, model-free: `model-free` holds only files that reach no
// machine-local fixture root, `stocked` holds every file that does, the two never share a file,
// and every listed file exists. Read off the files' own text, so a misfiled test reds here.

//! the calls through which a test reaches a machine-local fixture root (_model_tier.das)
let FIXTURE_ROOTS <- ["models_dir()", "model_available(", "llama2c_dir()", "whisper_dir()"]

def private file_text(path : string) : string {
    var text = ""
    fopen(path, "rb") $(f) {
        if (f != null) {
            text = fread(f)
        }
    }
    return text
}

def private reaches_fixture_root(text : string) : bool {
    for (root in FIXTURE_ROOTS) {
        if (find(text, root) >= 0) {
            return true
        }
    }
    return false
}

[test]
def test_suite_split_matches_the_files(t : T?) {
    var seen : table<string; void?>
    for (file in suite_files("model-free")) {
        continue if (base_name(file) == "test_run_suites.das")   // this file names the roots as strings and reaches none
        let text = file_text(file)
        t |> success(!empty(text), "model-free file exists and reads: {file}")
        t |> success(!reaches_fixture_root(text), "a model-free file reaches no fixture root - move it to `stocked`: {file}")
        seen |> insert(file, null)
    }
    for (file in suite_files("stocked")) {
        let text = file_text(file)
        t |> success(!empty(text), "stocked file exists and reads: {file}")
        t |> success(reaches_fixture_root(text), "a stocked file reaches a fixture root - move it to `model-free`: {file}")
        t |> success(!key_exists(seen, file), "a file sits in one per-PR list only: {file}")
    }
}

//! every test file in the folder sits in some run.das suite; the one exemption is the CPU-prefill
//! tripwire, whose header says why (it asserts the guard the runner disarms for every child)
[test]
def test_every_file_sits_in_a_suite(t : T?) {
    var listed : table<string; void?>   // nolint:STYLE027 - a nested walk over the suite tables, not one comprehension
    for (suite in ["decode", "mtp", "prefill", "matrix", "kernels", "image", "image-vulkan", "coverage", "model-free", "stocked"]) {
        for (file in suite_files(suite)) {
            listed |> insert(base_name(file), null)
        }
    }
    let exempt <- ["test_cpu_prefill_tripwire.das"]
    var count = 0
    fio::dir("modules/dasLLAMA/tests") $(name) {
        if ((name |> starts_with("test_") || name |> starts_with("failed_")) && name |> ends_with(".das")) {
            count++
            t |> success(key_exists(listed, name) || find_index(exempt, name) >= 0, "a test file sits in a run.das suite, or is the declared tripwire exemption: {name}")
        }
    }
    t |> success(count > 50, "the folder listing walked the test files ({count})")
}

[test]
def test_exclude_filter(t : T?) {
    let files <- ["modules/dasLLAMA/tests/test_ple_modes.das", "modules/dasLLAMA/tests/test_parity.das", "modules/dasLLAMA/tests/test_flash.das"]
    let none <- exclude_files(files, "")
    t |> equal(length(none.kept), 3, "an empty exclude keeps every file")
    t |> equal(length(none.dropped), 0, "an empty exclude drops nothing")
    let one <- exclude_files(files, "ple_modes")
    t |> equal(length(one.kept), 2, "a token drops the files whose base name contains it")
    t |> equal(length(one.dropped), 1, "the dropped list carries the match")
    t |> equal(one.dropped[0], files[0], "the dropped file is the matching one")
    let two <- exclude_files(files, " ple_modes, ,flash ")
    t |> equal(length(two.kept), 1, "a comma list drops every match, blanks ignored")
    t |> equal(two.kept[0], files[1], "the survivor is the unmatched file")
    let dir <- exclude_files(files, "dasLLAMA")
    t |> equal(length(dir.kept), 3, "a token matches the base name, not the directory")
}
