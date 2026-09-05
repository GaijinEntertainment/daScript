options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

require dastest/testing_boost public
require run
require daslib/fio
require daslib/strings_boost
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
        continue if (base_name(file) == "test_run_suites.das")   //! this file names the roots as strings and reaches none
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
    // the runner is required by this file and test_run_summary.das: nothing may fire on require
    t |> success(find(file_text("{TESTS_DIR}/run.das"), "[init]") < 0, "run.das carries no [init] - it runs nothing on require")
}

//! the area tables cover the per-PR census exactly: every area file is a model-free or stocked file,
//! every such file has at least one area, and every image-suite arm an area names exists
[test]
def test_area_tables_match_the_census(t : T?) {
    var census <- [for (f in suite_files("model-free")); base_name(f)]
    census |> push_from([for (f in suite_files("stocked")); base_name(f)])
    var covered : table<string; void?>
    for (a in AREAS) {
        for (name in area_tests(a)) {
            t |> success(find_index(census, name) >= 0, "an area file sits in the model-free or stocked census: {a}/{name}")
            covered |> insert(name, null)
        }
    }
    for (name in census) {
        t |> success(key_exists(covered, name), "a per-PR file has an area - add it to `area_tests`: {name}")
    }
    let image = file_text("{TESTS_DIR}/test_model_image.das")
    var claimed : array<string>
    for (a in AREAS) {
        for (tok in split(area_image_arms(a), ",")) {
            continue if (empty(tok))
            t |> success(find(image, "arm_on(t, \"{tok}") >= 0, "the image suite has an arm the area names: {a}/{tok}")
            claimed |> push(tok)
        }
    }
    // the reverse: every arm the image suite declares is reachable from some area, or is named here as
    // deliberately unclaimed - `gemma` is an --arm substring of five vision arms (CLAUDE.md says how to reach it)
    let unclaimed <- ["gemma"]
    let marker = "arm_on(t, \""
    var at = find(image, marker)
    var n_arms = 0
    while (at >= 0) {
        let start = at + length(marker)
        let stop = find(image, "\"", start)
        break if (stop < 0)
        let arm = slice(image, start, stop)   // nolint:PERF031 - a few dozen short arm names, read once
        n_arms++
        var hit = find_index(unclaimed, arm) >= 0
        for (tok in claimed) {
            hit ||= find(arm, tok) >= 0
        }
        t |> success(hit, "an image-suite arm is reachable from an area, or listed as unclaimed: {arm}")
        at = find(image, marker, stop)
    }
    t |> success(n_arms > 10, "the image suite's arm literals were walked ({n_arms})")
}

//! the change-to-area map behind --changed
[test]
def test_areas_for_path(t : T?) {
    let every = join(AREAS, ",")
    t |> equal(join(areas_for_path("modules/dasLLAMA/dasllama/dasllama_audio.das"), ","), "audio", "an audio module reaches audio")
    t |> equal(join(areas_for_path("modules/dasLLAMA/dasllama/dasllama_tower.das"), ","), "audio,vision", "the shared tower reaches both towers")
    t |> equal(join(areas_for_path("modules/dasLLAMA/dasllama/dasllama_math.das"), ","), every, "a core module reaches every area")
    t |> equal(join(areas_for_path("modules/dasLLAMA/dasllama/dasllama_brand_new.das"), ","), every, "a module without a row reaches every area - the safe default")
    t |> equal(join(areas_for_path("modules/dasLLAMA/tests/test_gemma3v.das"), ","), "vision", "a test file reaches its own area")
    t |> equal(join(areas_for_path("modules/dasLLAMA/tests/test_tower_helpers.das"), ","), "audio,vision", "a two-area test reaches both")
    t |> equal(length(areas_for_path("modules/dasLLAMA/tests/test_metal_decode_parity.das")), 0, "a model-suite file reaches no area - it takes --arm")
    t |> equal(join(areas_for_path("modules/dasLLAMA/tests/_tts_oracle.das"), ","), "tts", "a TTS fixture reaches tts")
    t |> equal(join(areas_for_path("modules/dasLLAMA/tests/_model_tier.das"), ","), every, "the shared tier helper reaches every area")
    t |> equal(join(areas_for_path("modules/dasLLAMA/performance/model_specs.das"), ","), "infra", "the performance tree reaches infra")
    t |> equal(join(areas_for_path("modules/dasLLAMA/tests/REVIEW.md"), ","), "infra", "a document under tests/ reaches infra, not a model suite")
    t |> equal(join(areas_for_path("modules/dasLLAMA/tests/run.das"), ","), "infra", "the runner itself reaches infra (its own tests live there)")
    t |> equal(join(areas_for_path("modules/dasLLAMA/models/silero_vad.bin"), ","), "audio", "the checked-in VAD weights reach audio")
    t |> equal(join(areas_for_path("modules\\dasLLAMA\\dasllama\\dasllama_tts.das"), ","), "tts", "a backslash path maps the same")
    t |> equal(length(areas_for_path("src/ast/ast_parse.cpp")), 0, "a path outside the module reaches nothing")
    t |> equal(skipped_cells("37 tests, 23 passed, 0 failed, 0 errors, 14 skipped"), 14, "the summary's skipped count is read")
    t |> equal(skipped_cells(""), 0, "no summary reads as no skips")
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
