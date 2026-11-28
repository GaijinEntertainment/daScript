options gen2
options _dasllama_internal = true   // the large-model gate reads the size tier's engine module
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

require dastest/testing_boost public
require run
require dastest/kill_switch   // KILL_SWITCH_EXIT_CODE: the exit of a child its test wall ended
require _model_categories
require dasllama/dasllama_gguf   // model_tier_skip_reason: a file past the tier is a category's
require dasllama/dasllama_env   // models_dir_resolved: the models directory the runner reads
require _child_run   // disable_module_flags, child_cmd_root_leaving: what a spawned child leaves out
require _kernel_census   // census_read_of, with_kernel_census
require daslib/fio
require daslib/strings_boost
require strings
require math

// The runner's per-PR suite split, model-free: `model-free` holds only files that reach no
// machine-local fixture root, `stocked` holds every file that does, the two never share a file,
// and every listed file exists. Read off the files' own text, so a misfiled test reds here.
// On Windows a cell that starts the runner over a POSIX no-op child stops after its pure asserts.

//! the calls through which a test reaches a machine-local fixture root (_model_tier.das)
let FIXTURE_ROOTS <- ["models_dir()", "model_available(", "llama2c_dir()", "whisper_dir()", "corpus_dir()", "asr_clip(", "asr_oracle_dir("]

def private text_holds(text : string; needles : array<string>) : bool {
    for (n in needles) {
        return true if (find(text, n) >= 0)
    }
    return false
}

//! whether `file`, or a same-folder fixture it requires (`require _<fixture>`) down to `depth` levels (-1: any depth),
//! holds one of `needles`; `seen` holds the files read, each read once
def private reaches_text(file : string; needles : array<string>; depth : int; var seen : table<string>) : bool {
    return false if (key_exists(seen, file))
    seen |> insert(file)
    let text = fread(file)
    return true if (text_holds(text, needles))
    return false if (depth == 0)
    for (line in split(text, "\n")) {
        continue if (!(line |> starts_with("require _")))
        var inscope name <- split_by_chars(slice(line, length("require ")), " \t/\r")
        let fixture = path_join(dir_name(file), "{name[0]}.das")
        return true if (fexist(fixture) && reaches_text(fixture, needles, depth - 1, seen))
    }
    return false
}

//! a test reaches a fixture root through its own text or through a shared fixture module beside it, one level down - a
//! same-folder `require _<fixture>` (test_gpu_resident_gemma4_26b_host.das reaches `model_available` through
//! `_gemma_resident` alone)
def private reaches_fixture_root(file : string) : bool {
    var inscope seen : table<string>
    return reaches_text(file, FIXTURE_ROOTS, 1, seen)
}

[test(description = "`reaches_text` finds a needle in the file's own text, in a same-folder fixture it requires down to the depth asked and no deeper, at any depth for -1, and reads a require cycle once")]
def test_reaches_text(t : T?) {
    let dir = test_temp_dir()
    t |> success(fwrite(path_join(dir, "top.das"), "require _mid public\n")
        && fwrite(path_join(dir, "_mid.das"), "require _leaf\nrequire _top_again\n")
        && fwrite(path_join(dir, "_top_again.das"), "require _mid\n")
        && fwrite(path_join(dir, "_leaf.das"), "the NEEDLE text\n"), "the fixture files are written")
    let top = path_join(dir, "top.das")
    var inscope s0, s1, s2, sa, sl : table<string>
    t |> success(!reaches_text(top, ["NEEDLE"], 0, s0), "depth 0 reads the file's own text alone")
    t |> success(!reaches_text(top, ["NEEDLE"], 1, s1), "depth 1 stops at the fixture it requires")
    t |> success(reaches_text(top, ["NEEDLE"], 2, s2), "depth 2 reaches the fixture's fixture")
    t |> success(reaches_text(top, ["absent", "NEEDLE"], -1, sa), "-1 reaches any depth, any needle of the list")
    t |> success(!reaches_text(top, ["absent"], -1, sl), "a needle no file holds is found nowhere, the cycle read once")
}

[test(description = "Every model-free suite file reaches no machine-local fixture root by its text or a same-folder fixture, every stocked file reaches one and declares persistent_heap, and no file sits in both lists")]
def test_suite_split_matches_the_files(t : T?) {
    var seen : table<string; void?>
    for (file in free_files()) {
        continue if (base_name(file) == "test_run_suites.das")   //! this file names the roots as strings and reaches none
        t |> success(!empty(fread(file)), "model-free file exists and reads: {file}")
        t |> success(!reaches_fixture_root(file), "a model-free file reaches no fixture root - move it to `stocked`: {file}")
        seen |> insert(file, null)
    }
    for (file in suite_files("stocked")) {
        let text = fread(file)
        t |> success(!empty(text), "stocked file exists and reads: {file}")
        t |> success(reaches_fixture_root(file), "a stocked file reaches a fixture root - move it to `model-free`: {file}")
        t |> success(!key_exists(seen, file), "a file sits in one per-PR list only: {file}")
        t |> success(find(text, "options persistent_heap") >= 0, "a stocked file declares `options persistent_heap`, so the carriers its cells delete are freed: {file}")
    }
}

//! every test file in the folder sits in some run.das suite, the
//! census verdict the runner appends to a run aside
[test(description = "Every test_, failed_ and uncategorized file in the folder but the census verdict sits in some run.das suite, over more than 50 files walked, and run.das carries no [init]")]
def test_every_file_sits_in_a_suite(t : T?) {
    var listed : table<string; void?>   // nolint:STYLE027 - a nested walk over the suite tables, not one comprehension
    for (suite in ["mtp", "kernels", "gpu-kernels", "image", "image-vulkan", "model-free", "stocked", "examples"]) {
        for (file in suite_files(suite)) {
            listed |> insert(base_name(file), null)
        }
    }
    var count = 0
    fio::dir("modules/dasLLAMA/tests") $(name) {
        if ((name |> starts_with("test_") || name |> starts_with("failed_") || name |> starts_with("uncategorized")) && name |> ends_with(".das")) {
            count++
            let verdict = "{TESTS_DIR}/{name}" == CENSUS_VERDICT
            t |> success(key_exists(listed, name) || verdict, "a test file sits in a run.das suite: {name}")
        }
    }
    t |> success(count > 50, "the folder listing walked the test files ({count})")
    // the runner is required by this file and test_run_summary.das: nothing may fire on require
    t |> success(find(fread("{TESTS_DIR}/run.das"), "[init]") < 0, "run.das carries no [init] - it runs nothing on require")
}

//! the area tables cover the per-PR census exactly: every standing
//! area's file is a model-free, stocked or image file,
//! every such file has at least one area, an on-demand area's files are the examples suite's and no per-PR file,
//! every image-suite arm an area names exists, and every image-suite
//! arm is one area's
[test(description = "The runner's standing area tables cover the model-free, stocked and image census exactly, an on-demand area holds the examples suite's files and none of the census, every image arm an area names exists in test_model_image, and every image arm literal is reached by the arms of exactly one area")]
def test_area_tables_match_the_census(t : T?) {
    var census <- [for (f in free_files()); base_name(f)]
    census |> push_from([for (f in suite_files("stocked")); base_name(f)], [for (f in suite_files("image")); base_name(f)],
        [for (f in suite_files("image-vulkan")); base_name(f)])
    let on_demand <- [for (f in suite_files("examples")); base_name(f)]
    var covered : table<string; void?>
    for (a in standing_areas()) {
        for (name in area_tests(a)) {
            t |> success(find_index(census, name) >= 0, "an area file sits in the model-free or stocked census: {a}/{name}")
            covered |> insert(name, null)
        }
    }
    t |> equal(join(ON_DEMAND_AREAS, ","), "examples", "the examples area is the one a run must name")
    t |> success(find_index(standing_areas(), "examples") < 0 && length(standing_areas()) == length(AREAS) - 1, "a standing area is every area but the on-demand one")
    for (a in ON_DEMAND_AREAS) {
        t |> equal(join(area_tests(a), ","), join(on_demand, ","), "an on-demand area's files are the examples suite's: {a}")
        for (name in area_tests(a)) {
            t |> success(find_index(census, name) < 0 && !key_exists(covered, name), "an on-demand file sits in no per-PR list and no standing area: {name}")
        }
    }
    for (name in census) {
        t |> success(key_exists(covered, name), "a per-PR file has an area - add it to `area_tests`: {name}")
    }
    let image = fread("{TESTS_DIR}/test_model_image.das")
    for (a in AREAS) {
        for (tok in split(area_image_arms(a), ",")) {
            continue if (empty(tok))
            t |> success(find(image, "arm_on(t, \"{tok}") >= 0, "the image suite has an arm the area names: {a}/{tok}")
        }
    }
    let marker = "arm_on(t, \""
    var at = find(image, marker)
    var n_arms = 0
    while (at >= 0) {
        let start = at + length(marker)
        let stop = find(image, "\"", start)
        break if (stop < 0)
        let arm = slice(image, start, stop)   // nolint:PERF031 - a few dozen short arm names, read once
        n_arms++
        var inscope owners : array<string>
        for (a in AREAS) {
            for (tok in split(area_image_arms(a), ",")) {
                if (!empty(tok) && find(arm, tok) >= 0 && !has_value(owners, a)) {
                    owners |> push(a)
                }
            }
        }
        t |> equal(length(owners), 1, "an image-suite arm is one area's, so a whole run runs it once: {arm} ({join(owners, ",")})")
        at = find(image, marker, stop)
    }
    t |> success(n_arms > 10, "the image suite's arm literals were walked ({n_arms})")
}

//! the change-to-area map behind --changed
[test(description = "`areas_for_path` maps a pinned table of module, fixture, test and tree paths to their expected areas, unmapped module paths to every area, outside paths to none, and `skipped_cells` reads the summary's skip count")]
def test_areas_for_path(t : T?) {
    let every = join(standing_areas(), ",")
    t |> success(find(every, "examples") < 0, "no change reaches the on-demand area through an engine module: {every}")
    t |> equal(join(areas_for_path("modules/dasLLAMA/tests/test_parrot.das"), ","), "examples", "an example's smoke test is its own area's file")
    t |> equal(join(areas_for_path("modules/dasLLAMA/dasllama/dasllama_audio.das"), ","), "audio", "an audio module reaches audio")
    t |> equal(join(areas_for_path("modules/dasLLAMA/dasllama/dasllama_tower.das"), ","), "audio,vision,tts,translate", "the shared tower reaches both towers, the TTS blocks calling its row kernels and the Bergamot chain its GEMMs")
    t |> equal(join(areas_for_path("modules/dasLLAMA/dasllama/dasllama_vulkan_tower.das"), ","), "audio,vision,tts,translate", "the Vulkan tower reaches both towers and the TTS and translation drivers riding its classes")
    for (stem in ["dasllama_nmt", "dasllama_metal_nmt", "dasllama_vulkan_nmt", "dasllama_translate"]) {
        t |> equal(join(areas_for_path("modules/dasLLAMA/dasllama/{stem}.das"), ","), "translate", "a translation module reaches translate: {stem}")
    }
    t |> equal(join(areas_for_path("modules/dasLLAMA/dasllama/dasllama_metal_prefill.das"), ","), "llm,tts,audio,vision,translate", "the Metal prefill reaches the Bergamot chain riding its q8 GEMM")
    t |> equal(join(areas_for_path("modules/dasLLAMA/dasllama/dasllama_gpu_kernels_common.das"), ","), "audio,vision,tts,translate,llm", "the shared GPU kernels reach the Bergamot step's stamps")
    t |> equal(join(areas_for_path("modules/dasLLAMA/dasllama/dasllama_spm.das"), ","), "llm,tts,translate", "the SentencePiece module reaches the Bergamot model-file reader")
    t |> equal(join(areas_for_path("modules/dasLLAMA/tests/_nmt_oracle.das"), ","), "translate,llm", "the Bergamot oracle fixture reaches translate and the kernel cells")
    t |> equal(join(areas_for_path("modules/dasLLAMA/tests/_npy.das"), ","), "audio,translate", "the npy reader reaches the GigaAM and Bergamot dumps")
    t |> equal(join(areas_for_path("modules/dasLLAMA/tests/test_nmt.das"), ","), "translate", "the translation file reaches its own area")
    t |> equal(join(areas_for_path("modules/dasLLAMA/dasllama/dasllama_tts_slab.das"), ","), "tts", "the TTS slab writer reaches the TTS seats alone: the driver is its one requirer")
    t |> equal(join(areas_for_path("modules/dasLLAMA/dasllama/dasllama_vulkan_tts.das"), ","), "tts", "the Vulkan TTS driver reaches tts")
    t |> equal(join(areas_for_path("modules/dasLLAMA/dasllama/dasllama_rope.das"), ","),
               "llm,tts,vision,audio",
               "the rope tables reach the engine, the TTS slab, and every decoder load")
    t |> equal(join(areas_for_path("modules/dasLLAMA/dasllama/dasllama_vulkan_asr_dec.das"), ","), "audio", "the Vulkan ASR decoder reaches audio")
    t |> equal(join(areas_for_path("modules/dasLLAMA/dasllama/dasllama_math.das"), ","), every, "a core module reaches every area")
    t |> equal(join(areas_for_path("modules/dasLLAMA/dasllama/dasllama_brand_new.das"), ","), every, "a module without a row reaches every area - the safe default")
    t |> equal(join(areas_for_path("modules/dasLLAMA/dasllama/dasllama_bench.das"), ","), "llm,infra", "the bench rows reach the engine area and the tooling that drives them")
    t |> equal(join(areas_for_path("modules/dasLLAMA/tests/test_gemma3v.das"), ","), "vision", "a test file reaches its own area")
    t |> equal(join(areas_for_path("modules/dasLLAMA/tests/test_tower_helpers.das"), ","), "audio,vision", "a two-area test reaches both")
    t |> equal(join(areas_for_path("modules/dasLLAMA/tests/test_vulkan_tower_kernels.das"), ","), "audio,vision", "the Vulkan tower kernel cells reach both towers")
    t |> equal(join(areas_for_path("modules/dasLLAMA/tests/test_vulkan_tts_conv_kernels.das"), ","), "tts", "the Vulkan TTS sequence-class cells reach the tts area")
    t |> equal(join(areas_for_path("modules/dasLLAMA/tests/test_vulkan_tts_kernels.das"), ","), "tts", "the Vulkan TTS kernel cells reach the tts area")
    t |> equal(join(areas_for_path("modules/dasLLAMA/tests/test_cuda_tts_kernels.das"), ","), "tts", "the CUDA TTS kernel cells reach the tts area")
    t |> equal(join(areas_for_path("modules/dasLLAMA/dasllama/dasllama_tower_tts.das"), ","), "tts", "the discrete TTS driver reaches tts")
    t |> equal(join(areas_for_path("modules/dasLLAMA/dasllama/dasllama_cuda_common.das"), ","),
               "llm,audio,vision,tts",
               "the CUDA home's common module reaches the TTS kernel cells that hold its carveout scope")
    t |> equal(join(areas_for_path("modules/dasLLAMA/tests/test_vulkan_tts_pocket_kernels.das"), ","), "tts", "the Vulkan Pocket TTS cells reach the tts area")
    t |> equal(join(areas_for_path("modules/dasLLAMA/tests/test_vulkan_tts_source_kernels.das"), ","), "tts", "the Vulkan TTS source and STFT cells reach the tts area")
    t |> equal(join(areas_for_path("modules/dasLLAMA/tests/_tower_twin.das"), ","), "audio,vision", "the tower twin fixture reaches both towers")
    t |> equal(length(areas_for_path("modules/dasLLAMA/tests/test_metal_decode_kernels.das")), 0, "a kernels-suite file reaches no area - it takes --arm")
    t |> equal(join(areas_for_path("modules/dasLLAMA/tests/_tts_oracle.das"), ","), "tts", "a TTS fixture reaches tts")
    t |> equal(join(areas_for_path("modules/dasLLAMA/tests/_audio_asr_cells.das"), ","), "audio", "an audio cell module reaches audio")
    t |> equal(join(areas_for_path("modules/dasLLAMA/tests/_model_tier.das"), ","), every, "the shared tier helper reaches every area")
    t |> equal(join(areas_for_path("modules/dasLLAMA/performance/model_specs.das"), ","), "infra", "the performance tree reaches infra")
    t |> equal(join(areas_for_path("modules/dasLLAMA/tests/REVIEW.md"), ","), "infra", "a document under tests/ reaches infra, not a model suite")
    t |> equal(join(areas_for_path("modules/dasLLAMA/tests/run.das"), ","), "infra", "the runner itself reaches infra (its own tests live there)")
    t |> equal(join(areas_for_path("modules/dasLLAMA/REVIEW.das"), ","), "infra", "a folder's lint gate reaches infra")
    t |> equal(join(areas_for_path("modules/dasLLAMA/models/silero_vad.bin"), ","), "audio", "the checked-in VAD weights reach audio")
    t |> equal(join(areas_for_path("modules\\dasLLAMA\\dasllama\\dasllama_tts.das"), ","), "tts", "a backslash path maps the same")
    t |> equal(length(areas_for_path("src/ast/ast_parse.cpp")), 0, "a path outside the module reaches nothing")
    t |> equal(join(areas_for_path("modules/dasLLAMA/tests/_vision_oracle.das"), ","), "vision", "the vision oracle fixture reaches vision")
    t |> equal(join(areas_for_path("modules/dasLLAMA/tests/_metal_kernel_common.das"), ","), "llm", "the metal kernel fixture reaches llm")
    t |> equal(join(areas_for_path("modules/dasLLAMA/tests/_vk_kq_fixtures.das"), ","), "llm", "a vulkan fixture reaches llm")
    t |> equal(join(areas_for_path("modules/dasLLAMA/tests/_vkd_oracles.das"), ","), "llm,tts,audio,vision", "the cell-plane oracles reach every area with Vulkan cells")
    t |> equal(join(areas_for_path("modules/dasLLAMA/harness/parity.sh"), ","), "infra", "the harness tree reaches infra")
    t |> equal(join(areas_for_path("modules/dasLLAMA/benchmarks/lcpp_bench.das"), ","), "llm", "the benchmarks tree reaches llm")
    t |> equal(join(areas_for_path("modules/dasLLAMA/some_new_tree/x.das"), ","), every, "an unmapped tree reaches every area - the safe default")
    t |> equal(skipped_cells("37 tests, 23 passed, 0 failed, 0 errors, 14 skipped"), 14, "the summary's skipped count is read")
    t |> equal(skipped_cells(""), 0, "no summary reads as no skips")
}

//! the pure planners behind an area run: the file set is the areas' census files, deduplicated and
//! sorted, with the image suite's file appended once when any area owns image arms
[test(description = "`area_plan` returns an area's census files sorted and deduplicated with the image file appended once and last when an area owns image arms, plans the image-vulkan suite's file for the llm area, and `plan_files` lets an area override --suite")]
def test_area_plan(t : T?) {
    let tts <- area_plan(["tts"])
    t |> equal(length(tts.files), 19, "tts plans its eighteen files and the image file its kitten arm rides")
    t |> equal(tts.image_arms, "kitten,pocket", "tts owns the kitten and pocket image arms")
    t |> equal(tts.files[length(tts.files) - 1], "{TESTS_DIR}/test_model_image.das", "the image file rides last")
    for (i in range(1, length(tts.files) - 1)) {
        t |> success(tts.files[i - 1] < tts.files[i], "the plan is sorted: {tts.files[i - 1]} < {tts.files[i]}")
    }
    let translate <- area_plan(["translate"])
    t |> equal(join(translate.files, ","), "{TESTS_DIR}/test_nmt.das,{TESTS_DIR}/test_model_image.das", "translate plans its one file and the image file its bergamot arm rides")
    let examples <- area_plan(["examples"])
    t |> equal(join(examples.files, ","), "{TESTS_DIR}/test_interpreter.das,{TESTS_DIR}/test_parrot.das,{TESTS_DIR}/test_storyteller_restart.das,{TESTS_DIR}/test_storywish.das", "a run that names examples plans the four smoke tests")
    t |> equal(examples.image_arms, "", "examples owns no image arm")
    t |> equal(translate.image_arms, "bergamot", "translate owns the bergamot image arm")
    let infra <- area_plan(["infra"])
    t |> equal(infra.image_arms, "", "infra owns no image arms, so no image file")
    t |> success(find_index(infra.files, "{TESTS_DIR}/test_model_image.das") < 0, "no image file without arms")
    let both <- area_plan(["audio", "vision"])
    let n_image = length([for (f in both.files); f; where f == "{TESTS_DIR}/test_model_image.das"])
    t |> equal(n_image, 1, "two arm-owning areas append the image file once")
    t |> equal(both.files[length(both.files) - 1], "{TESTS_DIR}/test_model_image.das", "the image file rides last")
    t |> success(find(both.image_arms, "whisper") >= 0 && find(both.image_arms, "gemma3v") >= 0, "the arm string is the union: {both.image_arms}")
    let n_tower = length([for (f in both.files); f; where f == "{TESTS_DIR}/test_tower_helpers.das"])
    t |> equal(n_tower, 1, "a file in both areas plans once")
    let cfg = RunArgs(suite = "image", arm = "arm12")
    let none : array<string>
    let suite <- plan_files(cfg, none)
    t |> equal(length(suite.files), 1, "a suite run plans the suite table")
    t |> equal(suite.image_arms, "arm12", "a suite run passes --arm through")
    let area <- plan_files(cfg, ["tts"])
    t |> equal(length(area.files), 19, "an area run plans the area, whatever --suite says")
    let llm <- area_plan(["llm"])
    t |> success(has_value(llm.files, "{TESTS_DIR}/test_model_image_vulkan.das"),
        "llm plans the image-vulkan suite's file")
}

//! the argument contracts: an area run refuses --full/--suite and an
//! unknown area, takes --arm, and a changed run refuses --arm; a
//! per-PR suite refuses --arm and --full; a model suite wants exactly
//! one of --arm and --full, and --full is refused
[test(description = "The runner refuses unknown areas, --area beside --suite and --changed beside --arm, runs --area beside --arm, a per-PR suite refuses --arm, a model suite needs --arm and refuses --full, and child timeouts are 3600 for stocked and 1200 for arm runs")]
def test_argument_contracts(t : T?) {
    let plain = RunArgs()
    let none <- select_areas(plain)
    t |> equal(none.status, -1, "no --area and no --changed: a suite run")
    t |> equal(length(none.areas), 0, "and no areas")
    let tts = RunArgs(area = "tts,audio")
    let two <- select_areas(tts)
    t |> equal(two.status, -1, "named areas run")
    t |> equal(join(two.areas, ","), "audio,tts", "areas come back in canonical order")
    let bogus = RunArgs(area = "bogus")
    t |> equal(select_areas(bogus).status, 2, "an unknown area is refused")
    let mixed = RunArgs(area = "tts", suite = "image")
    t |> equal(select_areas(mixed).status, 2, "--area beside --suite is refused")
    let armed = RunArgs(area = "llm", arm = "arm12")
    let narrowed <- select_areas(armed)
    t |> equal(narrowed.status, -1, "--area beside --arm runs: the arms narrow the area's children")
    t |> equal(join(narrowed.areas, ","), "llm", "... over the named area")
    t |> equal(select_areas(RunArgs(changed = true, arm = "x")).status, 2, "--changed beside --arm is refused")
    t |> equal(check_suite_args(RunArgs(suite = "stocked")), 0, "a per-PR suite runs bare")
    t |> equal(check_suite_args(RunArgs(suite = "stocked", arm = "x")), 2, "a per-PR suite refuses --arm")
    t |> equal(check_suite_args(RunArgs(suite = "wasm")), 0, "the wasm suite runs bare")
    t |> equal(check_suite_args(RunArgs(suite = "wasm", arm = "x")), 2, "the wasm suite refuses --arm")
    t |> equal(check_suite_args(RunArgs(suite = "mtp")), 2, "a model suite with neither --arm nor --full is refused")
    t |> equal(check_suite_args(RunArgs(suite = "mtp", arm = "arm12")), 0, "a model suite with --arm runs")
    t |> equal(check_suite_args(RunArgs(suite = "mtp", full = true)), 2, "--full is refused while the Metal build-out is in progress")
    t |> equal(child_timeout(RunArgs(suite = "stocked")), 3600, "the stocked gate's child holds the large tier's tails")
    t |> equal(child_timeout(RunArgs(changed = true)), 3600, "--changed runs the stocked files, so its child holds the same tails")
    t |> equal(child_timeout(RunArgs(suite = "mtp", full = true)), 3600, "the full gate's child too")
    t |> equal(child_timeout(RunArgs(suite = "mtp", arm = "arm12")), 1200, "an arm run keeps the arm-mode budget")
    t |> equal(child_timeout(RunArgs(area = "tts")), 1200, "... and so does an area run")
}

//! the runner on `args`; `env` is a POSIX prefix of `NAME=value ` tokens its children inherit
def private run_runner(args : string; var output : string&; env : string = "") : int {
    //! the runner's child logs land in the test's own folder; cmd.exe has no `NAME=value` prefix, and `temp_dir` reads TEMP there
    let scratch = get_platform_name() == "windows" ? "" : "TMPDIR='{test_temp_dir()}' "
    let cache = "-module-cache {path_join(test_temp_dir(), "run_suites.dascache")}"   //! and its compile record
    return unsafe(popen_timeout("{scratch}{env}{das_exe()} -dasroot {get_das_root()} {cache} {TESTS_DIR}/run.das -- {args}", 120.0, $(f) {
        if (f != null) {
            output = fread(f)
        }
    }))
}

//! the runner end to end with a no-op child binary: the AREAS line, the image file's arm env, the
//! REPORT and NEXT lines, and the refusals main prints
[test(description = "A dry runner run with a no-op child prints the AREAS, START, REPORT and NEXT lines, gives the image child alone the arms union, batches other files, and refuses an all-excluding filter or unresolvable base")]
def test_runner_dry_run(t : T?) {
    if (get_platform_name() == "windows") {
        t |> skip("the dry run stands in a POSIX no-op binary for the child")
        return
    }
    var out : string
    let area_rc = run_runner("--area audio,vision --das /usr/bin/true", out)
    t |> equal(area_rc, 0, "a dry area run exits clean:\n{out}")
    t |> success(find(out, "AREAS audio,vision") >= 0, "the AREAS line names the areas")
    t |> success(find(out, "START test_model_image.das: ") >= 0, "the image file runs")
    let image_start = find(out, "START test_model_image.das: ")
    let image_line_end = find(out, "\n", image_start)
    let image_line = slice(out, image_start, image_line_end)
    t |> success(find(image_line, "--run-env \"DASLLAMA_TEST_ARMS=whisper,") >= 0 && find(image_line, "gemma3v") >= 0, "the image child gets the union of the areas' arms:\n{image_line}")
    t |> success(find(image_line, " --test \"") >= 0 && find(image_line, "--together") < 0, "the image child runs alone, under --test:\n{image_line}")
    let audio_cmd = find(out, "--together \"{TESTS_DIR}/test_audio.das\"")
    t |> success(audio_cmd >= 0, "the audio file rides a --together batch:\n{out}")
    let audio_start = rfind(slice(out, 0, max(audio_cmd, 0)), "START ")
    let audio_line = slice(out, audio_start, find(out, "\n", audio_start))
    t |> success(find(audio_line, "--run-env \"DASLLAMA_TEST_ARMS=\"") >= 0, "every other child gets no arm filter:\n{audio_line}")
    var single : string
    t |> equal(run_runner("--area audio --das /usr/bin/true --batch 1", single), 0, "a dry area run at --batch 1 exits clean:\n{single}")
    t |> success(find(single, "START test_audio.das: ") >= 0 && find(single, "--together") < 0, "at --batch 1 every file is its own --test child:\n{single}")
    t |> success(find(out, "REPORT areas audio,vision: ") >= 0, "the REPORT line names the scope")
    t |> success(find(out, "NEXT before the PR") >= 0, "the NEXT line names the per-PR suites")
    var llm : string
    t |> equal(run_runner("--area infra --das /usr/bin/true --exclude test_facade.das", llm), 0, "an infra dry run with an exclusion exits clean")
    t |> success(find(llm, "EXCLUDED test_facade.das") >= 0 && find(llm, "1 file(s) excluded") >= 0, "the exclusion is loud and counted:\n{llm}")
    var refused : string
    t |> equal(run_runner("--suite stocked --exclude test", refused), 2, "an exclusion that drops every file is refused")
    t |> success(find(refused, "dropped every file") >= 0, "and says so:\n{refused}")
    var changed : string
    let changed_rc = run_runner("--changed --base HEAD --das /usr/bin/true", changed)
    t |> equal(changed_rc, 0, "a --changed run against HEAD exits clean:\n{changed}")
    t |> success(find(changed, "CHANGED") >= 0 || find(changed, "nothing") >= 0, "it either maps a change or says nothing reached an area:\n{changed}")
    var no_base : string
    let no_base_rc = run_runner("--changed --base no-such-ref-here --das /usr/bin/true", no_base)
    t |> equal(no_base_rc, 2, "a base git cannot resolve is refused, never read as nothing changed:\n{no_base}")
}

//! the `--timeout` value a START line's command carries
def start_timeout(out : string; tag : string) : int {
    let at = find(out, "START {tag}: ")
    return -1 if (at < 0)
    let line = slice(out, at, find(out, "\n", at))
    let t = find(line, "--timeout ")
    return -1 if (t < 0)
    let v = slice(line, t + length("--timeout "))
    return to_int(slice(v, 0, find(v, " ")))
}

[test(description = "A batch child that exits red before any verdict stops the run as LOST on a run with no wall, no file of the batch started again alone, the batch timeout is file count times a one-file child's, and a green child stops nothing")]
def test_runner_stops_at_a_verdictless_batch(t : T?) {
    return if (get_platform_name() == "windows")   //! the child binary stands in a POSIX one, as test_runner_dry_run's
    var out : string
    let rc = run_runner("--area infra --only test_box --das /usr/bin/false", out)
    t |> success(rc != 0, "a run whose child exits red before any verdict exits red:\n{out}")
    t |> success(find(out, "START test_box_ident.das+1: ") >= 0, "the two files ride one batch:\n{out}")
    t |> success(find(out, "LOST test_box_ident.das+1: the batch ended with exit 1: 0 of 2 files have a verdict") >= 0,
        "with no wall a batch red before any verdict is lost by that name:\n{out}")
    t |> success(find(out, "START test_box_ident.das: ") < 0 && find(out, "START test_box_profile.das: ") < 0,
        "no file of the lost batch is started again on its own:\n{out}")
    t |> success(find(out, "RESUME once it is fixed: this command with --skip 0\n") >= 0 && find(out, "STOPPED, the files after it not run") >= 0,
        "the run stops there and says how to resume:\n{out}")
    var lone : string
    t |> equal(run_runner("--area infra --only test_box --das /usr/bin/true --batch 1", lone), 0, "a dry run one file a child exits clean:\n{lone}")
    let single = start_timeout(lone, "test_box_ident.das")
    t |> success(single > 0, "a one-file child carries a timeout:\n{lone}")
    t |> equal(start_timeout(out, "test_box_ident.das+1"), 2 * single, "the batch's timeout is its file count times the single's")
    var clean : string
    t |> equal(run_runner("--area infra --only test_box --das /usr/bin/true", clean), 0, "a green child with no verdict stops nothing:\n{clean}")
    t |> success(find(clean, "LOST ") < 0, "the control: nothing lost:\n{clean}")
}

[test(description = "The model-free suite's wall is 30 s outside the full suite and the run's own under --test-wall, its batch is the whole suite off Windows and --batch where given, the kill switch's line is read out of a child's log, and the device kernel files sit beside the model-free ones as files that load no model")]
def test_wall_and_batch_defaults(t : T?) {
    let plain = RunArgs(suite = "model-free")
    t |> equal(wall_seconds_in(plain, false), TEST_WALL_SECONDS, "the model-free suite's own wall")
    t |> equal(wall_seconds_in(plain, true), 0, "no wall in the full suite, whose cells are the ones no wall admits")
    t |> equal(wall_seconds_in(RunArgs(suite = "model-free", test_wall = 0), false), 0, "--test-wall 0 is no wall")
    t |> equal(wall_seconds_in(RunArgs(suite = "stocked", test_wall = 45), true), 45, "--test-wall walls any suite, the full one too")
    t |> equal(wall_seconds_in(RunArgs(suite = "stocked"), false), 0, "no other suite has a wall of its own")
    t |> equal(wall_seconds_in(RunArgs(suite = "gpu-kernels"), false), 0, "the device kernel files run with no wall")
    t |> equal(batch_files(plain), get_platform_name() == "windows" ? MODEL_FREE_BATCH_WINDOWS : MODEL_FREE_BATCH, "the model-free suite's own batch")
    t |> success(MODEL_FREE_BATCH >= length(suite_files("model-free")), "off Windows the whole model-free suite is one batch")
    t |> equal(batch_files(RunArgs(suite = "model-free", batch = 3)), 3, "--batch wins")
    t |> equal(batch_files(RunArgs(suite = "stocked")), MODEL_BATCH, "any other run batches by MODEL_BATCH")
    t |> equal(scope_relaxed_in(plain, true), " full suite (DASLLAMA_PARITY_FULL=1): no test wall", "the full suite's lifted wall is said in the REPORT")
    t |> equal(scope_relaxed_in(plain, false), "", "the plain walled run says nothing")
    t |> equal(scope_relaxed_in(RunArgs(suite = "model-free", test_wall = 0, skip = 2), true), " no test wall resumed at child 2", "a wall given by hand is the one said")
    t |> equal(scope_relaxed_in(RunArgs(suite = "stocked"), true), "", "a suite with no wall of its own has none to lift")
    let said = "kill switch: test 'test_x' of a.das (--test-wall) ran past 30s; the process ends here with exit code 125"
    t |> equal(log_wall_line("=== RUN 'test_x'\n[E] {said}\n"), said, "the kill switch's line, from its own first word")
    t |> equal(log_wall_line("=== RUN 'test_x'\n--- PASS 'test_x' (0.1s)\n"), "", "no line where the switch said nothing")
    let free <- free_files()
    for (f in suite_files("gpu-kernels")) {
        t |> success(find_index(free, f) >= 0, "a device kernel file loads no model: {f}")
        t |> success(find_index(suite_files("model-free"), f) < 0, "... and sits outside the walled suite: {f}")
    }
    t |> equal(length(free), length(suite_files("model-free")) + length(suite_files("gpu-kernels")), "the two lists share no file")
}

[test(description = "A child the kill switch ended (exit 125) has no summary - a log line carrying the summary's marker is a cell's text - and its DONE verdict is the switch's own line, or says the switch ended it; any other exit reads its dastest summary")]
def test_child_verdict_of_the_kill_switch(t : T?) {
    let usage = "[E] --area / --changed take neither --arm, --full nor --suite: an area is the whole set - its tests, its arms"
    let said = "kill switch: test 'test_x' of a.das (--test-wall) ran past 30s; the process ends here with exit code 125"
    let killed = "=== RUN 'test_y'\n{usage}\n=== RUN 'test_x'\n{said}\n"
    t |> equal(child_summary(KILL_SWITCH_EXIT_CODE, killed), "", "a killed child has no summary")
    t |> equal(child_verdict(KILL_SWITCH_EXIT_CODE, child_summary(KILL_SWITCH_EXIT_CODE, killed), killed), said,
        "a killed child's verdict is the switch's line, not a cell's usage sentence")
    let mute = "=== RUN 'test_y'\n{usage}\n"
    let quiet = child_verdict(KILL_SWITCH_EXIT_CODE, child_summary(KILL_SWITCH_EXIT_CODE, mute), mute)
    t |> success(find(quiet, "kill switch ended the child, exit 125") >= 0 && find(quiet, "--area") < 0,
        "a killed child whose log lost the switch's line still reads as the switch's: {quiet}")
    let clean = "PASS a.das (1s)\n7 tests, 7 passed, 0 failed, 0 errors, 0 skipped\n"
    t |> equal(child_verdict(0, child_summary(0, clean), clean), "7 tests, 7 passed, 0 failed, 0 errors, 0 skipped",
        "any other exit reads its summary")
    t |> equal(child_verdict(1, child_summary(1, "crash\n"), "crash\n"), "no dastest summary - the run died early (read the log)",
        "no summary on another exit is an early death")
}

//! the 1-based-free index `K` of the child whose `START` line names `tag`, as its `child K/N` says ("" where none does)
def private child_index(out, tag : string) : string {
    let head = "START {tag}: child "
    let at = find(out, head)
    return "" if (at < 0)
    let from = at + length(head)
    return slice(out, from, find(out, "/", from))
}

[test(description = "Under a test wall a batch child that exits red before any verdict stops the run with a LOST line and the --skip of that child to resume on, retrying nothing, while a lone red file is a plain failure and the run goes on; the stop writes the run's stop file and a run that finds it starts no child; a resumed run starts no child before its --skip, says so in its REPORT, and reruns a later batch's files from the first")]
def test_walled_run_stops_at_a_lost_batch(t : T?) {
    if (get_platform_name() == "windows") {
        t |> skip("the dry run stands in a POSIX no-op binary for the child")
        return
    }
    let stop = path_join(test_temp_dir(), "dasllama_run_stop_{get_time_usec(0l)}")
    var out : string
    let rc = run_runner("--suite model-free --test-wall 30 --das /usr/bin/false --stop-file {stop}", out)
    t |> success(rc != 0, "a run stopped at a lost batch exits red:\n{out}")
    t |> success(find(out, " --test-wall 30 ") >= 0, "a walled run hands its children the wall:\n{out}")
    t |> success(find(out, "LOST failed_dasllama_lint_require.das") < 0 && find(out, "START failed_dasllama_lint_sidedoor.das: ") >= 0,
        "a lone red file is a failure of its own, and the run goes on:\n{out}")
    let lost_at = find(out, "LOST ")
    t |> success(lost_at >= 0 && find(out, "files have a verdict") >= 0, "the batch with no verdict is named lost:\n{out}")
    let tag = lost_at < 0 ? "" : slice(out, lost_at + length("LOST "), find(out, ": ", lost_at))
    let at = child_index(out, tag)
    t |> success(!empty(at) && find(out, "RESUME once it is fixed: remove {stop}, then this command with --skip {at}\n") >= 0,
        "the line to resume on names the lost child ({tag}, child {at}):\n{out}")
    t |> success(find(out, "STOPPED, the files after it not run") >= 0, "the run says it stopped short:\n{out}")
    t |> success(find(fread(stop), tag) >= 0, "the stop wrote the run's stop file, naming the child: {stop}")
    var marked : string
    t |> success(run_runner("--suite model-free --das /usr/bin/true --stop-file {stop}", marked) != 0, "a run that finds the stop file exits red:\n{marked}")
    t |> success(find(marked, "remove {stop}, or drop --stop-file, to run") >= 0 && find(marked, "START ") < 0,
        "... starts no child, and says how to clear it:\n{marked}")
    remove(stop)
    var resumed : string
    t |> equal(run_runner("--suite model-free --test-wall 30 --das /usr/bin/true --skip 1", resumed), 0, "a resumed dry run exits clean:\n{resumed}")
    t |> success(find(resumed, "SKIP failed_dasllama_lint_require.das: child 0/") >= 0, "the child before --skip is not started:\n{resumed}")
    t |> success(find(resumed, "START failed_dasllama_lint_require.das: ") < 0, "... and has no START line:\n{resumed}")
    t |> success(find(resumed, "test wall 30 s resumed at child 1") >= 0, "the REPORT carries the wall and the resume:\n{resumed}")
    var open_run : string
    t |> success(run_runner("--suite model-free --test-wall 0 --das /usr/bin/false --skip {at}", open_run) != 0, "with no wall the red batch stops the run all the same:\n{open_run}")
    t |> success(find(open_run, " --test-wall ") < 0 && find(open_run, " no test wall resumed at child {at}") >= 0,
        "--test-wall 0 hands the children no wall and says so in the REPORT:\n{open_run}")
    let first = slice(tag, 0, find(tag, "+"))
    t |> success(find(open_run, "LOST {tag}: ") >= 0, "the batch is lost by its name, wall or none:\n{open_run}")
    t |> success(find(open_run, "START {first}: ") < 0, "and no file of it is started again on its own:\n{open_run}")
}

//! a stand-in child binary under the test's temp folder: it prints the file FAKE_CHILD_LOG names and exits with FAKE_CHILD_RC
def private fake_child() : string {
    let path = path_join(test_temp_dir(), "dasllama_fake_child.sh")
    fwrite(path, "#!/bin/sh\nif [ -n \"$FAKE_CHILD_LOG\" ]; then cat \"$FAKE_CHILD_LOG\"; fi\nexit $FAKE_CHILD_RC\n")
    var out : string
    run_and_capture(["chmod", "+x", path], out)
    return path
}

[test(description = "A child that ends with the kill switch's exit code stops the run with a WALL line carrying the switch's own line where the log holds one, a walled batch whose root failed is lost by that name, and a walled batch that is red with every file's verdict counts its failures and stops nothing")]
def test_runner_reads_a_red_child(t : T?) {
    if (get_platform_name() == "windows") {
        t |> skip("the stand-in child is a POSIX shell script")
        return
    }
    let child = fake_child()
    let log = path_join(test_temp_dir(), "dasllama_fake_child.log")
    var out : string
    t |> success(run_runner("--suite model-free --das {child}", out, "FAKE_CHILD_RC=125 FAKE_CHILD_LOG= ") != 0, "a child at the wall's exit code reds the run:\n{out}")
    t |> success(find(out, "WALL failed_dasllama_lint_require.das: a child ended with the kill switch's exit code 125") >= 0,
        "a wall exit with no line of the switch's is still a wall:\n{out}")
    t |> success(find(out, "this command with --skip 0\n") >= 0 && find(out, "START failed_dasllama_lint_sidedoor.das: ") < 0,
        "the run stops at that child, lone file or not:\n{out}")
    let said = "kill switch: test 'test_x' of a.das (--test-wall) ran past 30s; the process ends here with exit code 125"
    fwrite(log, "=== RUN 'test_x'\n{said}\n")
    run_runner("--suite model-free --das {child}", out, "FAKE_CHILD_RC=125 FAKE_CHILD_LOG={log} ")
    t |> success(find(out, "WALL failed_dasllama_lint_require.das: {said}") >= 0, "the WALL line is the switch's own:\n{out}")
    fwrite(log, "error[30341]: no matching functions or generics: f(bool)\n[E] Failed to compile the together root x.das\nFAIL modules/dasLLAMA/tests/test_box_ident.das (0.0s)\nFAIL modules/dasLLAMA/tests/test_box_profile.das (0.0s)\n")
    run_runner("--area infra --only test_box --test-wall 30 --das {child}", out, "FAKE_CHILD_RC=2 FAKE_CHILD_LOG={log} ")
    t |> success(find(out, "LOST test_box_ident.das+1: the batch ended with exit 2: its root failed, no file ran: error[30341]: no matching functions or generics: f(bool) (read its log)") >= 0,
        "a batch whose root failed is lost by that name, its first compiler error on the line:\n{out}")
    run_runner("--area infra --only test_box --das {child}", out, "FAKE_CHILD_RC=2 FAKE_CHILD_LOG={log} ")
    t |> success(find(out, "LOST test_box_ident.das+1: ") >= 0 && find(out, "START test_box_ident.das: ") < 0,
        "with no wall the same batch is lost the same way, no file of it run alone:\n{out}")
    t |> equal(log_error_line("no error here\n"), "", "a log with no compiler error names none")
    let long_error = "error[30341]: {repeat("x", 300)}"
    t |> equal(length(log_error_line("[E] {long_error}\nnext\n")), 240, "an error line past 240 characters is cut there")
    t |> equal(log_error_line("[E] error[1]: the first\nerror[2]: a second"), "error[1]: the first",
        "the first compiler error is named, from its `error[`")
    t |> equal(log_error_line("a warning\nerror[2]: the last line"), "error[2]: the last line",
        "an error on a last line with no newline is that line whole")
    fwrite(log, "PASS modules/dasLLAMA/tests/test_box_ident.das (0.1s)\nFAIL modules/dasLLAMA/tests/test_box_profile.das (0.1s)\n")
    run_runner("--area infra --only test_box --test-wall 30 --das {child}", out, "FAKE_CHILD_RC=1 FAKE_CHILD_LOG={log} ")
    t |> success(find(out, "LOST ") < 0 && find(out, "2 file(s), 1 failed (test_box_profile.das)") >= 0,
        "a walled batch with every verdict counts its failures and stops nothing:\n{out}")
    remove(log)
    remove(child)
}

[test(description = "The device kernel suite runs whole with no arm under the stocked child budget, a path of one of its files reaches that file, every one of them sits in some area's plan, and the stocked plan starts with them; a run that reaps no image walks no models folder")]
def test_gpu_kernels_suite(t : T?) {
    t |> equal(child_timeout(RunArgs(suite = "gpu-kernels")), child_timeout(RunArgs(suite = "stocked")), "the device kernel files take the stocked budget")
    t |> success(child_timeout(RunArgs(suite = "gpu-kernels")) > child_timeout(RunArgs(suite = "model-free")), "... past the model-free one")
    t |> equal(check_suite_args(RunArgs(suite = "gpu-kernels")), 0, "the suite runs whole, with no --arm")
    t |> success(check_suite_args(RunArgs(suite = "gpu-kernels", arm = "x")) != 0, "... and takes none")
    let planned <- area_plan([for (a in AREAS); a])
    for (f in suite_files("gpu-kernels")) {
        let reach <- reach_for_path(f)
        t |> success(find_index(reach.files, base_name(f)) >= 0 && empty(reach.note), "a changed device kernel file runs itself: {f} ({reach.note})")
        t |> success(find_index(planned.files, f) >= 0, "an area plans the file: {f}")
    }
    let stocked <- plan_files(RunArgs(suite = "stocked"), [for (a in AREAS); a; where false])
    let kernels <- suite_files("gpu-kernels")
    t |> equal(join([for (i in range(length(kernels))); stocked.files[i]], ","), join(kernels, ","), "the stocked plan starts with the device kernel files")
    t |> equal(length(stocked.files), length(kernels) + length(suite_files("stocked")), "... ahead of every stocked file")
    let dir = path_join(test_temp_dir(), "dasllama_images_at_start_{get_time_usec(0l)}")
    mkdir_rec(dir)
    fwrite(path_join(dir, "a.dlim"), "x")
    t |> success(empty(images_at_start(RunArgs(), dir)), "a run that reaps nothing walks no models folder")
    t |> equal(length(images_at_start(RunArgs(reap_images = true), dir)), 1, "a reaping run keeps the images there as it starts")
    rmdir_rec(dir)
}

[test(description = "A model category holds the files whose names carry one of its stems: a run that names no category opens none of them, a run that names one opens its files past the size tier and no other model, and a name that is no category's is reported")]
def test_model_categories(t : T?) {
    t |> equal(model_category("/models/gemma-4-E4B-it-Q8_0"), "gemma4-e4b", "the decoder, by its path")
    t |> equal(model_category("mmproj-gemma-4-E4B-it-BF16"), "gemma4-e4b", "the mmproj")
    t |> equal(model_category("gemma-4-E2B-it-Q8_0"), "", "the family's small carrier is in none")
    t |> equal(model_category("gemma-4-12b-it-Q6_K"), "gemma4-dense", "a stem in either casing")
    t |> equal(model_category("mmproj-gemma-4-12B-it-BF16"), "", "a stem opens the name: the 12B's vision projector is no dense decoder")
    t |> equal(model_category("mtp-gemma-4-26B-A4B-it-Q8_0"), "gemma4-moe", "a draft head rides its target's category")
    for (cut in fixed_array("gpt-oss-4L-mxfp4-local.gguf", "gemma-4-12B-12L-Q4_K_M-local.gguf",
            "gemma-4-12B-12L-Q8_0-local.gguf", "gemma-4-26B-A4B-6L-Q4_K_M-local.gguf",
            "gemma-4-26B-A4B-6L-Q8_0-local.gguf", "gemma-4-26B-A4B-6L-Q4_0-local.gguf",
            "gemma-4-26B-A4B-6L-UD-IQ3_XXS-local.gguf", "gemma-4-26B-A4B-6L-MXFP4_MOE-local.gguf",
            "Qwen3.8-27B-8L-i1-IQ3_S-local.gguf", "Qwen3.8-27B-8L-UD-Q3_K_XL-local.gguf",
            "Qwen1.5-MoE-A2.7B-4L-Q8_0-local.gguf", "Qwen1.5-MoE-A2.7B-4L-Q4_K_M-local.gguf",
            "Qwen1.5-MoE-A2.7B-4L-Q5_1-local.gguf", "Qwen1.5-MoE-A2.7B-4L-Q5_0-pure-local.gguf",
            "Qwen1.5-MoE-A2.7B-4L-Q4_0-local.gguf", "Qwen3-30B-A3B-7L-Q4_K_M-local.gguf",
            "Qwen3-30B-A3B-7L-UD-IQ2_XXS-local.gguf", "Qwen3-30B-A3B-7L-UD-Q3_K_XL-local.gguf",
            "Qwen3-30B-A3B-7L-NVFP4-pure-local.gguf", "Qwen3.6-35B-A3B-4L-UD-Q4_K_M-local.gguf",
            "Qwen3.6-35B-A3B-4L-MTP-UD-Q4_K_M-local.gguf", "Qwen3.6-35B-A3B-4L-MTP-IQ4_XS-pure-local.gguf",
            "Qwen3.6-35B-A3B-4L-UD-IQ3_S-local.gguf", "Qwen3.6-35B-A3B-4L-UD-IQ2_XXS-local.gguf")) {
        t |> equal(model_category(cut), "", "{cut}, a cut of a category's file, is no category's: every run opens it")
    }
    t |> equal(model_category("gpt-oss-20b-mxfp4.gguf"), "gptoss-20b", "the file a cut is made from stays its category's")
    t |> equal(model_category("gemma-4-26B_q4_0-it.gguf"), "gemma4-moe", "the 26B's Q4_0 file is the routed category's")
    for (whole in fixed_array("Qwen1.5-MoE-A2.7B-Chat.Q8_0.gguf", "Qwen1.5-MoE-A2.7B-Chat-Q4_K_M-local.gguf",
            "moe-formats/Qwen1.5-MoE-A2.7B-Chat-exps-Q4_K.gguf")) {
        t |> equal(model_category(whole), "qwen15-moe", "{whole}, a whole Qwen1.5-MoE file or one minted off it, is the category's")
    }
    for (whole in fixed_array("Qwen3-30B-A3B-Instruct-2507-Q4_K_M.gguf", "Qwen3-30B-A3B-Instruct-2507-UD-IQ2_XXS.gguf")) {
        t |> equal(model_category(whole), "qwen3moe-30b", "{whole}, a whole 30B file, is the category's")
    }
    for (whole in fixed_array("Qwen3.6-35B-A3B-UD-IQ2_XXS.gguf", "Qwen3.6-35B-A3B-UD-Q4_K_M.gguf",
            "Qwen3.6-35B-A3B-MTP-UD-Q4_K_M.gguf", "Qwen3.6-35B-A3B-MTP-IQ4_XS-pure.gguf", "Qwen3.6-35B-A3B-NVFP4.gguf")) {
        t |> equal(model_category(whole), "qwen35moe-35b", "{whole}, a whole 35B file, is the category's")
    }
    t |> equal(model_family("Qwen3.6-35B-A3B-4L-MTP-IQ4_XS-pure-local.gguf"), "qwen35moe", "the 35B's cut keeps its family")
    t |> equal(model_category("Qwen3.5-4B-Q8_0.gguf"), "qwen35-4b", "the hybrid's 4B shape is its category's")
    t |> equal(model_category("Qwen3.5-0.8B-MTP-Q8_0.gguf"), "", "... and the hybrid's 0.8B carrier is in none")
    t |> equal(model_category("Qwen3.5-9B-MTP-UD-Q5_K_XL.gguf"), "qwen35-9b", "the 9B's K-quant hybrid is its category's")
    t |> equal(model_category("Qwen3.8-27B.i1-IQ3_S.gguf"), "qwen35-27b", "a 27B hybrid is the 27B category's")
    t |> equal(model_category("mtp-Qwen3.8-27B-Q8_0.gguf"), "qwen35-27b", "... and so is its NextN head file")
    for (whole in fixed_array("Qwen3.8-27B.i1-IQ3_S.gguf", "Qwen3.8-27B-UD-Q3_K_XL.gguf", "Qwen3.8-27B-Q4_K_M.gguf",
            "Qwen3.8-27B-UD-IQ4_XS.gguf", "Qwen3.8-27B-4bit", "Qwen3.8-27B-DFlash2-Q8_0.gguf", "Qwen3.6-27B-MTP-Q4_K_M.gguf")) {
        t |> equal(model_category(whole), "qwen35-27b", "{whole}, a whole 27B file, stays the 27B category's")
    }
    t |> equal(model_category("Qwen3.8-Flash-Next-UD-IQ4_XS-00001-of-00003.gguf"), "flash-next", "the Flash-Next trunk, under the same 3.8 prefix, is its own category's")
    t |> success(model_category("Qwen3.8-Flash-Next-GGUF/MTP/mtp-Qwen3.8-Flash-Next-shared-Q8_0.gguf") == "flash-next" && !empty(category_skip_reason("Qwen3.8-Flash-Next-IQ4_XS-pure.gguf", "")), "... as its split head is, and a run naming no category opens none of them")
    t |> equal(model_category("Qwen3-4B-Instruct-2507-Q8_0.gguf"), "qwen3-4b", "the dense qwen3's 4B shape is its category's")
    //! a format carrier is no category's: its cells claim a format the family's small carrier lacks, so every per-PR run
    //! opens it - a category holds a larger model on the small carrier's own path
    t |> equal(model_category("Qwen3-4B-Instruct-2507-Q4_K_M.gguf"), "", "the 4B's K-quant files are format carriers, in no category")
    t |> equal(model_category("Qwen3-4B-Instruct-2507-Q5_K_M.gguf"), "", "... the Q5_K_M as the Q4_K_M")
    t |> equal(model_category("Qwen3-4B-Instruct-2507-Q6_K.gguf"), "", "... and every other 4B format past the Q8_0")
    t |> equal(model_category("Qwen3.5-0.8B-Q4_K_M.gguf"), "", "the hybrid's K-quant twin is in none")
    t |> equal(model_category("Qwen3-0.6B-Q8_0.gguf"), "", "... and the dense qwen3's 0.6B carrier is in none")
    for (carrier in fixed_array("Qwen1.5-MoE-A2.7B-4L-Q5_1-local.gguf", "Qwen1.5-MoE-A2.7B-4L-Q5_0-pure-local.gguf",
            "Qwen1.5-MoE-A2.7B-4L-Q4_0-local.gguf")) {
        t |> equal(model_category(carrier), "", "{carrier}, a per-32 format carrier's cut, is in none")
    }
    for (carrier in fixed_array("Qwen3-30B-A3B-7L-NVFP4-pure-local.gguf", "Qwen3-30B-A3B-7L-UD-IQ2_XXS-local.gguf",
            "Qwen3-30B-A3B-7L-UD-Q3_K_XL-local.gguf")) {
        t |> equal(model_category(carrier), "", "{carrier}, the 30B's format carrier cut, is in none")
    }
    for (carrier in fixed_array("Qwen3.6-35B-A3B-4L-MTP-IQ4_XS-pure-local.gguf", "Qwen3.6-35B-A3B-4L-UD-IQ3_S-local.gguf",
            "Qwen3.6-35B-A3B-4L-UD-IQ2_XXS-local.gguf")) {
        t |> equal(model_category(carrier), "", "{carrier}, the 35B's format carrier cut, is in none")
    }
    t |> equal(model_family("gemma-4-26B-A4B-6L-Q4_K_M-local.gguf"), "gemma4moe", "a cut keeps its family")
    t |> success(find(category_skip_reason("gemma-4-E4B-it-Q8_0", ""), "--category gemma4-e4b") >= 0, "a run that names none leaves a category's file, the reason naming the flag")
    t |> equal(category_skip_reason("gemma-4-E2B-it-Q8_0", ""), "", "... and opens a model of no category")
    t |> equal(category_skip_reason("gemma-4-E4B-it-Q8_0", "other, gemma4-e4b"), "", "a named category's file opens")
    t |> success(find(category_skip_reason("gemma-4-E2B-it-Q8_0", "gemma4-e4b"), "outside the named categories") >= 0, "... and no other model does")
    t |> success(list_names("a, gemma4-e4b", "gemma4-e4b") && !list_names("gemma4-e4bx", "gemma4-e4b") && !list_names("gemma4", "gemma4-e4b"), "a category is named whole")
    t |> equal(join(unknown_categories("gemma4-e4b, nope,,other"), ","), "nope,other", "the names that are no category's")
    t |> success(category_gate("gemma-4-E4B-it-Q8_0", "gemma4-e4b").lifts_tier, "a named category's file is past the size tier")
    t |> success(!category_gate("gemma-4-E2B-it-Q8_0", "").lifts_tier && !category_gate("gemma-4-E2B-it-Q8_0", "gemma4-e4b").lifts_tier, "... and no other file is")
    for (cat in model_categories()) {
        t |> success(!empty(cat.name) && !empty(cat.stems) && !empty(cat.topics), "the category {cat.name} names its files and its topics")
        t |> success(!empty(category_files(cat.name)), "... and a test file names one of its models")
    }
}

[test(description = "A category run opens the towers its row names beside its own files - the gemma4-dense decoders' image chat towers - which a run naming no category opens too and a run naming another leaves, and a tower a row opens is no category's own file")]
def test_category_opens(t : T?) {
    let mm12 = "mmproj-gemma-4-12B-it-BF16.gguf"
    t |> equal(category_skip_reason(mm12, "gemma4-dense"), "",
        "a tower the category's decoder pairs with opens")
    t |> equal(category_skip_reason("mmproj-gemma-3-4b-it-F16.gguf", "other, gemma4-dense"), "",
        "... the gemma-3-4b's too")
    t |> equal(category_skip_reason(mm12, ""), "", "... which a run naming no category opens still")
    let other = category_skip_reason(mm12, "gemma4-e4b")
    t |> success(find(other, "outside the named categories") >= 0,
        "... and a run naming another category leaves")
    for (cat in model_categories()) {
        for (stem in split(cat.opens, ",")) {
            continue if (empty(strip(stem)))
            t |> equal(model_category("{strip(stem)}-x.gguf"), "",
                "{cat.name} opens {stem}, a file of no category")
        }
    }
}

[test(description = "A large-tier model's row holds its whole file and leaves its small siblings and its towers out, and a category run takes the test files that walk the spec table to a category's row")]
def test_large_model_rows(t : T?) {
    for (whole, cat in fixed_array("Meta-Llama-3.1-8B-Instruct-Q8_0.gguf",
            "Llama-3.3-70B-Instruct-Q4_K_M.gguf", "gemma-3-12b-it-Q4_K_M.gguf",
            "Qwen3-Omni-30B-A3B-Instruct-Q8_0.gguf", "qwen2audio-7b-q8_0.gguf",
            "Mistral-7B-Instruct-v0.3-Q8_0.gguf", "Mistral-Small-3.1-24B-Instruct-2503-Q4_K_M.gguf",
            "Qwen3VL-8B-Instruct-Q8_0.gguf", "Qwen3-Coder-Next-Q4_K_M.gguf",
            "Qwen3-Coder-30B-A3B-Instruct-Q8_0.gguf", "Qwen3-8B-BF16.gguf",
            "Qwen3-30B-A3B-Q8_0.gguf", "Qwen3-30B-A3B-NVFP4-pure-local.gguf",
            "Qwen3.6-35B-A3B-Q8_0.gguf", "Qwen3.5-35B-A3B-UD-IQ3_S.gguf"),
        fixed_array("llama31-8b", "llama33-70b", "gemma3-12b", "qwen3omni-30b", "qwen2audio-7b",
            "mistral-7b", "mistral3-24b", "qwen3vl-8b", "qwen3coder-next", "qwen3coder-30b",
            "qwen3-8b", "qwen3moe-30b", "qwen3moe-30b", "qwen35moe-35b", "qwen35moe-35b")) {
        t |> equal(model_category(whole), cat, "{whole} is the {cat} category's")
    }
    for (small in fixed_array("Meta-Llama-3.1-8B-Instruct-Q4_K_M.gguf",
            "Meta-Llama-3.1-8B-Instruct-Q4_0-local.gguf",
            "Meta-Llama-3.1-8B-Instruct-NVFP4-local.gguf",
            "Mistral-7B-Instruct-v0.3-Q4_K_M.gguf", "mmproj-gemma-3-12b-it-F16.gguf",
            "mmproj-Qwen3-Omni-30B-A3B-Instruct-bf16.gguf", "qwen2audio-mmproj-f32.gguf",
            "mmproj-Qwen3VL-8B-Instruct-F16.gguf", "Llama-3.2-1B-Instruct-Q8_0.gguf")) {
        t |> equal(model_category(small), "",
            "{small}, a small sibling or a tower, is in no category: every run opens it")
    }
    var inscope walker <- category_files("mistral3-24b")
    t |> success(has_value(walker, "{TESTS_DIR}/test_parity.das"),
        "the 24B, which no model test names, runs in the file walking the spec table: {walker}")
    t |> success(!has_value(walker, "{TESTS_DIR}/test_chat.das"),
        "... and in no file that names none of its models")
}

[test(description = "Every model file this folder's sources or the spec table's name that the models directory holds past `MODEL_TIER_CEILING_BYTES` - its split shards and NextN head summed, as the size tier sums them - is a model category's file, so no run opens it unless the run names its category")]
def test_large_stocked_models_have_a_category(t : T?) {
    var names : table<string>
    for (folder in fixed_array(TESTS_DIR, dir_name(SPEC_TABLE))) {
        dir(folder) $(name) {
            return if (!(name |> ends_with(".das")))
            for (model in model_paths(fread("{folder}/{name}"))) {
                names |> insert(model)
            }
        }
    }
    t |> success(key_exists(names, "Qwen3-8B-BF16.gguf"),
        "the spec table's rows are among the names")
    let models = models_dir_resolved()
    for (model in keys(names)) {
        let path = path_join(models, model)
        // stocked whole - every shard present - and past the line
        let whole = empty(model_tier_skip_reason(path, true))
        continue if (!whole || empty(model_tier_skip_reason(path, false)))
        t |> success(!empty(model_category(model)),
            ("{model} is past MODEL_TIER_CEILING_BYTES in {models} and no category holds it - add "
                + "it to a model_categories() row (_model_categories.das) and that row of "
                + "../TEST_CATEGORIES.md"))
    }
}

[test(description = "A family run opens the model files of the families it names and of no row, by the file's name: the longest stem that opens a name is its family's")]
def test_model_families(t : T?) {
    t |> equal(model_family("/models/gemma-4-E2B-it-Q8_0.gguf"), "gemma4e", "the E-series carrier, by its path")
    t |> equal(model_family("drafters/x/gemma-4-E2B-it-dflash2-Q8_0.gguf"), "gemma4e", "a drafter rides its trunk's family")
    t |> equal(model_family("Qwen3-30B-A3B-Instruct-2507-Q4_K_M.gguf"), "qwen3moe", "the longest stem wins: the routed model under `Qwen3-`")
    t |> equal(model_family("Qwen3-0.6B-Q8_0.gguf"), "qwen3", "... and the dense one keeps the short stem")
    t |> equal(model_family("mmproj-gemma-4-E2B-it-bf16.gguf"), "", "a tower is in no row")
    t |> equal(family_skip_reason("Llama-3.2-1B-Instruct-Q8_0.gguf", ""), "", "a run that names no family opens every file")
    t |> equal(family_skip_reason("gemma-4-E2B-it-Q8_0.gguf", "gemma2, gemma4e"), "", "a named family's file opens")
    t |> success(find(family_skip_reason("Llama-3.2-1B-Instruct-Q8_0.gguf", "gemma4e"), "the llama family's") >= 0, "another family's file is left, the reason naming its family")
    t |> equal(family_skip_reason("mmproj-gemma-4-E2B-it-bf16.gguf", "llama"), "", "a file of no row is open to any family run")
    t |> equal(model_family("Qwen2.5-Omni-3B-Q8_0.gguf"), "qwen2", "the Omni decoder the qwen25v tower's chat cells read")
    t |> equal(model_family("Qwen3VL-4B-Instruct-Q8_0.gguf"), "qwen3", "the Qwen3-VL decoder the qwen3v tower's chat cells read")
    t |> equal(model_family("mmproj-Qwen2.5-Omni-3B-f16.gguf"), "", "... whose towers stay in no row")
    t |> equal(model_family("kitten-nano-q8.gguf"), "kitten", "a voice file is its family's")
    t |> equal(model_family("ggml-parakeet-tdt-0.6b-v3-f32.bin"), "parakeet", "the longest stem wins among the ggml carriers")
    t |> equal(model_family("ggml-tiny-q8_0.bin"), "whisper")
    t |> equal(model_family("bergamot/ru-en-tiny/model.ruen.intgemm.alphas.bin"), "bergamot", "a student, by its file under its folder")
    t |> equal(model_family("canary-qwen-2.5b-decoder-f16.gguf"), "", "the canary decoder is in no row: the qwen3 census reads it too")
    for (pack in fixed_array("tts_g2p.bin", "tts_postag.bin", "tts_g2p_ru.bin", "tts_ctx_ru.gguf")) {
        t |> equal(model_family(pack), "", "{pack}, which a voice reads beside it, is in no row")
    }
    t |> success(find(family_skip_reason("kokoro-82m.gguf", "kitten"), "the kokoro family's") >= 0, "a kitten run leaves a kokoro file")
    var seen : table<string>
    for (fam in model_families()) {
        t |> success(!empty(fam.name) && !empty(fam.stems) && !key_exists(seen, fam.name), "the family {fam.name} has one row with stems")
        seen |> insert(fam.name)
    }
}

//! the families of the tts area's voice and stress model files
let private TTS_FAMILIES = fixed_array("kitten", "kokoro", "pocket", "rustress")

[test(description = "A run narrowed to one TTS family opens, of the model files the tts area's tests name, every file of that family and no file of another family's row - kitten, kokoro, pocket and the Russian stress models in turn - and still opens a front-end file no row holds; a run that names no family opens all four families' files")]
def test_tts_family_run(t : T?) {
    var inscope files <- area_tests("tts")
    let dir = stocked_models_dir("dasllama_tts_family", files)
    var inscope whole <- opened_models(files, dir, "", "", false)
    for (fam in TTS_FAMILIES) {
        var inscope want <- [for (p in whole); p; where model_family(p) == fam]
        t |> success(!empty(want), "the area's tests name files of the {fam} family")
        var inscope opened <- opened_models(files, dir, "", fam, false)
        var own = 0
        for (p in opened) {
            let of = model_family(p)
            t |> success(empty(of) || of == fam, "--family {fam} opens no other family's file: {p} is {of}'s")
            own += of == fam ? 1 : 0
        }
        t |> equal(own, length(want), "--family {fam} opens every file of its family")
        t |> success(has_value(opened, "tts_ctx_ru.gguf"),
            "--family {fam} opens the context model a voice reads beside it: no row holds it")
    }
    var why : string
    rmdir_rec(dir, why)
}

[test(description = "`--category` takes known names and no flag that picks files or cells another way, plans the test files naming the categories' models, and hands every child the names", timeout = 1200)]
def test_category_run(t : T?) {
    t |> equal(check_filter_args(RunArgs(arm = "fam_gemma4e,g4e.q8:x+y-z", family = "gemma4e", category = "gemma4-e4b")), 0,
        "names and the marks between them stand")
    for (bad in [RunArgs(arm = "x$(id)"), RunArgs(family = "a`b"), RunArgs(category = "a\"b"), RunArgs(arm = "a b")]) {
        t |> success(check_filter_args(bad) != 0, "a filter a shell would read as its own is refused: '{bad.arm}{bad.family}{bad.category}'")
    }
    t |> equal(check_category_args(RunArgs(category = "gemma4-e4b")), 0, "a known category stands alone")
    t |> success(check_category_args(RunArgs(category = "gemma4-e4b,nope")) != 0, "a name that is no category's is refused")
    t |> success(check_category_args(RunArgs(category = "gemma4-e4b,")) != 0 && check_category_args(RunArgs(category = ",gemma4-e4b")) != 0,
        "an empty name between the commas is refused")
    for (clash in [RunArgs(category = "gemma4-e4b", suite = "stocked"), RunArgs(category = "gemma4-e4b", area = "audio"),
                   RunArgs(category = "gemma4-e4b", changed = true), RunArgs(category = "gemma4-e4b", arm = "g4e"),
                   RunArgs(category = "gemma4-e4b", full = true), RunArgs(category = "gemma4-e4b", family = "gemma4a")]) {
        t |> success(check_category_args(clash) != 0, "refused beside --suite {clash.suite} --area '{clash.area}' --changed {clash.changed} --arm '{clash.arm}' --full {clash.full} --family '{clash.family}'")
    }
    let files <- category_files("gemma4-e4b")
    t |> success(find_index(files, "{TESTS_DIR}/test_gemma4v.das") >= 0 && find_index(files, "{TESTS_DIR}/test_audio.das") >= 0, "the files naming an E4B model: {join(files, " ")}")
    t |> success(find_index(files, "{TESTS_DIR}/test_chat.das") < 0, "a file that names none stays out")
    t |> success(empty(category_files("nope")), "a name that is no category's plans nothing")
    t |> equal(length(category_files("gemma4-e4b,")), length(files), "an empty name plans no file of a model in no category")
    t |> success(empty([for (f in files); f; where base_name(f) |> starts_with("_")]), "a helper file that names a model is no test file: {join(files, " ")}")
    let by_path <- category_files("qwen15-moe")
    t |> success(find_index(by_path, "{TESTS_DIR}/test_gpu_resident_moe_formats.das") >= 0, "a file naming a category's model by its path under the models directory (`moe-formats/...`) is the category's: {join(by_path, " ")}")
    let named = RunArgs(category = "gemma4-e4b")
    let kernels = "{TESTS_DIR}/test_metal_decode_kernels.das"
    let image = "{TESTS_DIR}/test_model_image.das"
    let plain <- [for (i in range(12)); "{TESTS_DIR}/test_plain_{i}.das"]
    var mixed <- [for (f in plain); f]
    mixed |> push(image)
    mixed |> push(kernels)
    let chunks <- category_chunks(mixed, batch_files(named))
    t |> equal(length(chunks), 4, "a process suite's files ride a child of their own, the others ten a child: {chunks}")
    t |> success(length(chunks[0]) == 1 && chunks[0][0] == kernels && length(chunks[1]) == 1 && chunks[1][0] == image, "the kernels file and the image file each alone, in the process suites' order")
    t |> success(length(chunks[2]) == 10 && length(chunks[3]) == 2, "the twelve other files in two children")
    t |> equal(length(run_chunks(named, mixed, false)), 4, "a category run's children are the category's chunks")
    t |> equal(batch_files(named), batch_files(RunArgs(suite = "stocked")), "a category run batches as a model suite does")
    let mtp <- suite_files("mtp")
    t |> success(length(mtp) > 1 && length(category_chunks(mtp, 10)) == 1, "the files of one process suite share its child")
    t |> equal(length(category_chunks(mtp, 1)), length(mtp), "at one file a child - a failed root's rerun - each file is its own child")
    t |> success(find(child_run_env(named, ""), " --run-env \"DASLLAMA_TEST_CATEGORIES=gemma4-e4b\"") >= 0, "a run-time pair of the child: {child_run_env(named, "")}")
    t |> success(find(child_envs(named, false), "DASLLAMA_TEST_") < 0 && find(child_envs(named, true), "DASLLAMA_TEST_") < 0, "no filter rides the child's environment prefix: it would name another compile record")
    t |> success(find(child_run_env(RunArgs(), ""), "DASLLAMA_TEST_CATEGORIES") < 0, "no flag, no names")
    var fam = RunArgs()
    fam.family = "gemma4e"
    fam.arm = "g4e"
    t |> equal(child_run_env(fam, fam.arm), " --run-env \"DASLLAMA_TEST_ARMS=g4e\" --run-env \"DASLLAMA_TEST_FAMILY=gemma4e\"", "the arms and the families a run names")
    t |> success(find(child_envs(fam, false), "DASLLAMA_TEST_") < 0 && find(child_envs(fam, true), "DASLLAMA_TEST_") < 0, "the arms and families ride no environment prefix either")
    fam.full = true
    t |> equal(child_run_env(fam, fam.arm), " --run-env \"DASLLAMA_TEST_FAMILY=gemma4e\"", "the full suite names no arm")
    if (get_platform_name() == "windows") {
        t |> skip("the dry run stands in a POSIX no-op binary for the child")
        return
    }
    var out : string
    t |> equal(run_runner("--category gemma4-e4b --das /usr/bin/true", out), 0, "a dry category run exits clean:\n{out}")
    t |> success(find(out, "REPORT category gemma4-e4b: {length(files)} file(s)") >= 0, "the REPORT names the category and its files:\n{out}")
    t |> success(run_runner("--category nope --das /usr/bin/true", out) != 0 && find(out, "unknown --category 'nope'") >= 0, "an unknown category stops the run:\n{out}")
    t |> success(run_runner("--suite model-free --arm a@b --das /usr/bin/true", out) != 0 && find(out, "--arm 'a@b' holds a character") >= 0,
        "a filter value a child's command line cannot carry stops the run:\n{out}")
}

[test(description = "The tune variables a run names for its children: the box's file as the manifest where the caller named none, the forced file, and under --no-tune the reference policy alone, with the image rail off")]
def test_tune_vars(t : T?) {
    let boxed <- tune_vars_of(false, "", "/box.json", "/box.json")
    t |> equal(length(boxed), 2, "a box with a tune file and a caller that names none: the manifest and the forced file")
    t |> success(boxed[0].name == "DAS_TUNE_MANIFEST" && boxed[0].value == "/box.json", "the box's file is the manifest")
    t |> success(boxed[1].name == "DAS_TUNE_FORCE" && boxed[1].value == "/box.json", "and the forced file")
    let named <- tune_vars_of(false, "/caller.json", "/box.json", "/caller.json")
    t |> success(length(named) == 1 && named[0].name == "DAS_TUNE_FORCE", "a manifest the caller named inherits: it is not named again")
    let bare <- tune_vars_of(false, "", "", "")
    t |> equal(length(bare), 0, "a box with no tune file names nothing")
    let reference <- tune_vars_of(true, "", "/box.json", "")
    t |> success(length(reference) == 1 && reference[0].name == "DAS_TUNE_POLICY" && reference[0].value == "reference",
        "--no-tune names the reference policy and no manifest")
    var cfg = RunArgs()
    t |> success(find(child_envs(cfg, false), "DAS_TUNE_POLICY") < 0 && find(child_envs(cfg, false), "DASLLAMA_IMAGE") < 0,
        "the control: a tuned run's children carry neither: {child_envs(cfg, false)}")
    cfg.no_tune = true
    let posix = child_envs(cfg, false)
    t |> success(find(posix, "DAS_TUNE_POLICY='reference' ") >= 0 && find(posix, "DASLLAMA_IMAGE='0' ") >= 0,
        "a --no-tune run's children: the reference policy, the image rail off: {posix}")
    let cmd = child_envs(cfg, true)
    t |> success(find(cmd, "set \"DAS_TUNE_POLICY=reference\"&& ") >= 0 && find(cmd, "set \"DASLLAMA_IMAGE=0\"&& ") >= 0,
        "cmd.exe's form: {cmd}")
}

//! the test child's exe key a dry run of `args` builds under: its `CHILD key` line, "" where it prints none
def private runner_child_key(t : T?; args : string) : string {
    var out : string
    t |> equal(run_runner("--suite model-free --only test_par_indexed --das /usr/bin/true {args}", out, "DAS_LOG_LEVEL=info "), 0,
        "the dry run exits clean:\n{out}")
    for (line in split(out, "\n")) {
        let at = find(line, "CHILD key ")
        return slice(line, at + length("CHILD key ")) if (at >= 0)
    }
    return ""
}

[test(description = "The runner builds the test child under the key a test computes: it sets on itself each tune variable it names for its children, so its own key under --no-tune is a child's under the reference policy, and no other run's")]
def test_runner_builds_under_a_childs_key(t : T?) {
    if (get_platform_name() == "windows") {
        t |> skip("the dry run stands in a POSIX no-op binary for the child")
        return
    }
    let reference = runner_child_key(t, "--no-tune")
    var out = ""
    let envs = [(name = "DAS_TUNE_POLICY", value = "reference")]
    let rc = run_verb_child(child_verb_cmd("disable-flags", envs), out)
    t |> success(rc == 0 && find(out, "ROOT_DONE") >= 0, "the child reports its key under the reference policy:\n{out}")
    var child = ""
    for (line in split(out, "\n")) {
        if (line |> starts_with("KEY ")) {
            child = slice(line, length("KEY "))
        }
    }
    t |> success(!empty(reference) && reference == child, "the runner's key ({reference}) is the child's ({child})")
    t |> success(reference != runner_child_key(t, ""), "the control: a tuned run builds under another key")
}

[test(description = "A run forces its children onto the tune file in effect, whatever the file's age: the caller's manifest where it exists, nothing under --no-tune, for an absent file, or where the caller forced a file already", timeout = 1200)]
def test_forced_tune_file(t : T?) {
    let box = path_join(test_temp_dir(), "forced_box_{get_time_usec(0l)}.tune.json")
    t |> success(fwrite(box, "\{}"), "a box tune file is written")
    t |> equal(forced_tune_file_of(false, "", "", box), box, "with no manifest named the box's own file is forced")
    t |> equal(forced_tune_file_of(false, "", "{box}.other", box), "", "a named manifest that is not there forces nothing, the box's file or no")
    t |> equal(forced_tune_file_of(true, "", "", box), "", "--no-tune forces nothing")
    t |> equal(forced_tune_file_of(false, "theirs.json", "", box), "", "a file the caller forced stands")
    remove(box)
    t |> equal(env_ahead("A=1 ", "DAS_TUNE_FORCE", "x.json", false), "DAS_TUNE_FORCE='x.json' A=1 ", "the POSIX prefix names the forced file")
    t |> equal(env_ahead("set \"A=1\"&& ", "DAS_TUNE_FORCE", "x.json", true), "set \"A=1\"&& set \"DAS_TUNE_FORCE=x.json\"&& ", "cmd.exe's form sets it")
    if (get_platform_name() == "windows") {
        t |> skip("the dry run stands in a POSIX no-op binary for the child")
        return
    }
    let file = path_join(test_temp_dir(), "forced_{get_time_usec(0l)}.tune.json")
    t |> success(fwrite(file, "\{}"), "a tune file is written")
    let run = "--suite model-free --only test_run_suites --das /usr/bin/true"
    var out : string
    t |> equal(run_runner(run, out, "DAS_TUNE_FORCE= DAS_TUNE_MANIFEST='{file}' "), 0, "the dry run exits clean:\n{out}")
    t |> success(find(out, "DAS_TUNE_FORCE='{file}' ") >= 0, "the child is forced onto the caller's manifest:\n{out}")
    t |> equal(run_runner("{run} --no-tune", out, "DAS_TUNE_FORCE= DAS_TUNE_MANIFEST='{file}' "), 0, "the --no-tune dry run exits clean:\n{out}")
    t |> success(find(out, "DAS_TUNE_FORCE=") < 0, "--no-tune forces no file:\n{out}")
    t |> equal(run_runner(run, out, "DAS_TUNE_FORCE= DAS_TUNE_MANIFEST='{file}.absent' "), 0, "the absent-file dry run exits clean:\n{out}")
    t |> success(find(out, "DAS_TUNE_FORCE=") < 0, "a manifest that is not there forces nothing:\n{out}")
    t |> equal(run_runner(run, out, "DAS_TUNE_FORCE='{file}.theirs' DAS_TUNE_MANIFEST='{file}' "), 0, "the caller-forced dry run exits clean:\n{out}")
    t |> success(find(out, "DAS_TUNE_FORCE='{file}' ") < 0, "a file the caller forced stands: the run adds none of its own:\n{out}")
    remove(file)
}

[test(description = "`wasm_summary` reads the last summary line of a wasm test release's log and its FAIL names, and returns no line for a log that died before one")]
def test_wasm_summary(t : T?) {
    let log = "PASS test_a\nFAIL test_b\n=== 1/2 passed, 1 failed ===\nSKIP test_c\nFAIL test_d\n=== 2/4 passed, 2 failed ===\n"
    let sum <- wasm_summary(log)
    t |> equal(sum.line, "=== 2/4 passed, 2 failed ===", "the last summary line is the run's")
    t |> equal(join(sum.failed, ","), "test_b,test_d", "every FAIL line's test is named, in order")
    t |> equal(sum.skipped, 1, "every SKIP line is counted")
    let died <- wasm_summary("PASS test_a\nAborted(OOM)\n")
    t |> equal(died.line, "", "a log with no summary line reads as none")
    t |> equal(length(died.failed), 0, "and names no failure of its own")
    let quoted <- wasm_summary("    === 3 passed, 0 failed === is what a cell printed\n")
    t |> equal(quoted.line, "", "a line that only holds the words inside it is not the summary")
}

[test(description = "Every wasm suite file is a model-free suite file, and `wasm_package` writes a manifest, a descriptor naming each file and the wasm testing_boost, and a root that requires each file and returns the last file's exit code")]
def test_wasm_package(t : T?) {
    let files <- suite_files("wasm")
    t |> success(length(files) >= 3, "the wasm suite holds files ({length(files)})")
    let free <- suite_files("model-free")
    for (f in files) {
        t |> success(find_index(free, f) >= 0, "{base_name(f)} is a model-free file: a wasm run has no models")
    }
    let two <- ["modules/dasLLAMA/tests/test_a.das", "modules/dasLLAMA/tests/test_b.das"]
    let package <- wasm_package(two, "/das")
    t |> equal(length(package), 3, "three files: the manifest, the descriptor, the root")
    let manifest = package?[".das_package"] ?? ""
    t |> success(find(manifest, "release_main(\"main.das\")") >= 0, "the manifest releases the root")
    t |> success(find(manifest, "release_wasm_disable_host_gpu_modules()") >= 0, "and drops the host GPU modules")
    let descriptor = package?["modules/wasm_tests/.das_module"] ?? ""
    t |> success(find(descriptor, "register_native_path(\"dastest\", \"testing_boost\", \"/das/dastest_wasm/testing_boost.das\")") >= 0,
        "the descriptor gives the files the wasm testing_boost")
    t |> success(find(descriptor, "register_native_path(\"wasm_tests\", \"test_b\", \"/das/modules/dasLLAMA/tests/test_b.das\")") >= 0,
        "and names each file by its path under the root")
    let root = package?["main.das"] ?? ""
    t |> success(find(root, "require wasm_tests/test_a\nrequire wasm_tests/test_b\n") >= 0, "the root requires each file, in order")
    t |> success(find(root, "    rc = test_a::main()\n    rc = test_b::main()\n    return rc\n") >= 0,
        "and returns the last file's exit code - the count of failures so far")
    t |> success(find(root, "options persistent_heap\n") >= 0 && find(root, "options _dasllama_internal = true\n") >= 0,
        "under the options a dasLLAMA test root takes")
}

[test(description = "`exclude_files` drops the files whose base name contains any comma-listed token, ignores blank tokens, matches base names not directories, and keeps everything on an empty list")]
def test_exclude_filter(t : T?) {
    let files <- ["modules/dasLLAMA/tests/test_gpu_resident_gemma4_e2b.das", "modules/dasLLAMA/tests/test_parity.das", "modules/dasLLAMA/tests/test_prefill.das"]
    let none <- exclude_files(files, "")
    t |> equal(length(none.kept), 3, "an empty exclude keeps every file")
    t |> equal(length(none.dropped), 0, "an empty exclude drops nothing")
    let one <- exclude_files(files, "gemma4_e2b")
    t |> equal(length(one.kept), 2, "a token drops the files whose base name contains it")
    t |> equal(length(one.dropped), 1, "the dropped list carries the match")
    t |> equal(one.dropped[0], files[0], "the dropped file is the matching one")
    let two <- exclude_files(files, " gemma4_e2b, ,prefill ")
    t |> equal(length(two.kept), 1, "a comma list drops every match, blanks ignored")
    t |> equal(two.kept[0], files[1], "the survivor is the unmatched file")
    let dir <- exclude_files(files, "dasLLAMA")
    t |> equal(length(dir.kept), 3, "a token matches the base name, not the directory")
}

[test(description = "`run_exclusion` drops the CUDA-only files (`CUDA_ONLY_EXCLUDE`) where the children leave dasCUDA out and none where they leave out another module or none, each on an EXCLUDED line naming the knob, beside a caller's `--exclude` on its own lines, a file both drop on one line; a run it drops whole is refused naming both causes; a dry run under DASLLAMA_DISABLE_MODULES=dasCUDA prints the line and counts it, one without the knob neither")]
def test_run_exclusion(t : T?) {
    let files <- ["tests/test_cuda_tts_kernels.das", "tests/test_cuda_kernels.das",
        "tests/test_parity.das", "tests/test_prefill.das"]
    for (disabled in fixed_array("", "dasVulkan")) {
        var inscope kept <- run_exclusion(files, "", disabled)
        t |> success(length(kept.kept) == 4 && empty(kept.excluded) && empty(kept.refusal),
            "'{disabled}' leaves out no CUDA-only file")
    }
    var inscope off <- run_exclusion(files, "", "dasVulkan, dasCUDA")
    t |> equal(join(off.kept, ","), "tests/test_parity.das,tests/test_prefill.das",
        "dasCUDA left out drops the CUDA-only files")
    let why = ("dasCUDA left out (DASLLAMA_DISABLE_MODULES=dasVulkan, dasCUDA): "
        + "it requires the CUDA kernel classes)")
    t |> equal(join(off.excluded, "|"),
        "EXCLUDED test_cuda_tts_kernels.das ({why}|EXCLUDED test_cuda_kernels.das ({why}",
        "each on a line naming the knob")
    var inscope both <- run_exclusion(files, "parity,cuda_tts", "dasCUDA")
    t |> equal(join(both.kept, ","), "tests/test_prefill.das",
        "the caller's --exclude and the module drop together")
    t |> equal(length(both.excluded), 3, "a file both drop is one line: {both.excluded}")
    let caller_first = (length(both.excluded) == 3
        && find(both.excluded[0], "EXCLUDED test_cuda_tts_kernels.das (--exclude parity,cuda_tts)") == 0
        && find(both.excluded[1], "EXCLUDED test_parity.das (--exclude") == 0
        && find(both.excluded[2], "EXCLUDED test_cuda_kernels.das (dasCUDA left out") == 0)
    t |> success(caller_first, "the caller's reason first: {both.excluded}")
    var inscope all <- run_exclusion([for (f in files); f; where find(f, "cuda") >= 0], "", "dasCUDA")
    t |> equal(all.refusal, "with dasCUDA left out dropped every file of the run",
        "a run the module drops whole is refused")
    var inscope all_both <- run_exclusion(files, "p", "dasCUDA")
    t |> equal(all_both.refusal, "--exclude 'p' with dasCUDA left out dropped every file of the run",
        "naming both causes")
    if (get_platform_name() == "windows") {
        t |> skip("the dry run stands in a POSIX no-op binary for the child")
        return
    }
    let dry = "--suite model-free --only test_cuda_tts_kernels,test_run_suites --das /usr/bin/true"
    var on : string
    t |> equal(run_runner(dry, on, "DASLLAMA_DISABLE_MODULES=dasCUDA "), 0,
        "a dry run under the knob is clean:\n{on}")
    let line = "EXCLUDED test_cuda_tts_kernels.das (dasCUDA left out (DASLLAMA_DISABLE_MODULES=dasCUDA)"
    t |> success(find(on, line) >= 0 && find(on, "ONLY test_run_suites.das") >= 0
        && find(on, "ONLY test_cuda_tts_kernels.das") < 0 && find(on, " file(s) excluded") >= 0,
        "the runner drops the file, runs the other and counts the drop:\n{on}")
    var plain : string
    t |> equal(run_runner(dry, plain, "DASLLAMA_DISABLE_MODULES= "), 0,
        "a dry run without the knob is clean:\n{plain}")
    t |> success(find(plain, "EXCLUDED") < 0 && find(plain, "ONLY test_cuda_tts_kernels.das") >= 0
        && find(plain, " file(s) excluded") < 0, "the control: without the knob the file runs:\n{plain}")
}

//! --only keeps the files whose name contains a token and nothing else, "" the whole list; the dry run names each kept
//! file on an ONLY line and the REPORT line carries the tokens, and a --only that keeps nothing is refused
[test(description = "`only_files` keeps in order the files whose name holds a token, a dry run prints an ONLY line per kept file and the tokens on the REPORT line, and a --only keeping nothing is refused")]
def test_only_filter(t : T?) {
    let files <- ["a/test_cuda_kernels.das", "a/test_gpu_resident_cuda_llama.das", "a/test_gpu_resident_segments.das", "a/test_kv_codec.das"]
    let whole <- only_files(files, "")
    t |> equal(length(whole), 4, "no --only keeps every file")
    let cuda <- only_files(files, "cuda,segments")
    t |> equal(join(cuda, ","), "a/test_cuda_kernels.das,a/test_gpu_resident_cuda_llama.das,a/test_gpu_resident_segments.das", "a token keeps the files whose name holds it, in the list's order")
    let none <- only_files(files, "metal")
    t |> equal(length(none), 0, "a token no name holds keeps nothing")
    return if (get_platform_name() == "windows")   //! the dry run's no-op child binary is a POSIX one, as test_runner_dry_run's
    var out : string
    t |> equal(run_runner("--area infra --das /usr/bin/true --only exchange,facade", out), 0, "an infra dry run under --only exits clean:\n{out}")
    t |> success(find(out, "ONLY test_exchange_schema.das (--only exchange,facade)") >= 0 && find(out, "ONLY test_facade.das") >= 0 && find(out, "ONLY test_facade_docs.das") >= 0, "each kept file sits on an ONLY line:\n{out}")
    t |> success(find(out, "REPORT areas infra only exchange,facade: 3 file(s)") >= 0, "the REPORT line carries the tokens, so the run cannot read as whole:\n{out}")
    var refused : string
    t |> equal(run_runner("--area infra --das /usr/bin/true --only no-such-file", refused), 2, "a --only that keeps nothing is refused")
    t |> success(find(refused, "keeps no file") >= 0, "and says so:\n{refused}")
}

//! the text by which a test file reaches the CUDA home: the home ask, a CUDA fixture, a fixture
//! seating a tower twin on the CUDA tower under the ask, the voices' rail, whose `@dev` forms
//! serve the speech seats on the armed home, or a CUDA tower module
let private CUDA_REACH <- [
    "gpu_want_cuda()", "require _cuda_", "require ?cuda _cuda_", "cuda_tower_asked",
    "require _tower_twin", "require _audio_twin", "require _vision_forms", "require _tts_parity",
    "require ?cuda dasllama/dasllama_cuda_tower", "require ?cuda dasllama/dasllama_cuda_dflash"]

//! the files whose text names the CUDA home and that hold no CUDA cell
let private CUDA_REACH_NO_CELL <- [
    "test_fat_start.das",   // its Vulkan race cells skip on the CUDA home
    "test_gpu_home.das",   // a unit of the home pick itself, on no device
    "test_run_suites.das",   // this file, which names the reach text
    "test_vulkan_moe_cm2.das",   // its CUDA branch only skips
    "uncategorized__dasllama_internal_persistent_heap_stack_524288.das" ]   // a twin's verdict on synthetic rows

//! the `--only` tokens of the CUDA home's pass, read off its command
//! line in `CLAUDE.md`
def private cuda_pass_tokens(doc : string) : string {
    let only = "-- --changed --only "
    for (line in split(doc, "\n")) {
        let at = find(line, only)
        continue if (at < 0 || find(line, "# the CUDA home's pass") < 0)
        let rest = slice(line, at + length(only))
        let end = find(rest, " ")
        return end < 0 ? rest : slice(rest, 0, end)
    }
    return ""
}

[test(description = "The CUDA home's pass (`CUDA_PASS_ONLY`) keeps, by its `--only` tokens, every census file whose text reaches the CUDA home but the declared no-cell files, and CLAUDE.md's command and prose carry the same tokens")]
def test_cuda_pass_keeps_every_cuda_file(t : T?) {
    let doc = fread("{TESTS_DIR}/CLAUDE.md")
    let tokens = CUDA_PASS_ONLY
    t |> equal(cuda_pass_tokens(doc), tokens,
        "CLAUDE.md's CUDA pass command carries CUDA_PASS_ONLY")
    let prose = find(doc, "`--changed --only {tokens}`") >= 0
    t |> success(prose, "the prose names the same tokens: {tokens}")
    var census <- free_files()
    census |> push_from(suite_files("stocked"))
    let kept <- only_files(census, tokens)
    var reach = 0
    for (file in census) {
        let no_cell = find_index(CUDA_REACH_NO_CELL, base_name(file)) >= 0
        continue if (no_cell || !text_holds(fread(file), CUDA_REACH))
        reach++
        t |> success(find_index(kept, file) >= 0,
            "a file reaching the CUDA home sits in the CUDA pass - add a token: {file}")
    }
    t |> success(reach > 10, "the census walk found the CUDA files ({reach})")
}

//! the reach plan behind --changed: a test file itself, a fixture the tests requiring it through the fixtures between, an
//! engine module its witness row or the core set beside its other areas whole, a benchmark the bench rows' files; every
//! witness row and the core set name census files, and every row's module exists and reaches llm
[test(description = "`reach_for_path` maps a test to itself - an on-demand one too -, a fixture to its requiring census tests and to no on-demand one, a leaf module to its witness row, a media decoder's arch module to that file's area, a core module to the core plus every other area, and every witness names census files")]
def test_reach_for_path(t : T?) {
    let own <- reach_for_path("modules/dasLLAMA/tests/test_gemma3v.das")
    t |> equal(join(own.files, ","), "test_gemma3v.das", "a test file reaches itself")
    t |> success(empty(own.areas) && !own.core, "and nothing else")
    let suite <- reach_for_path("modules/dasLLAMA/tests/test_metal_decode_kernels.das")
    t |> success(empty(suite.files) && !empty(suite.note), "a model-suite file reaches nothing and says why")
    let feed <- reach_for_path("modules/dasLLAMA/tests/_resident_feed.das")
    t |> success(find_index(feed.files, "test_gpu_resident_llama.das") >= 0, "a fixture reaches the tests requiring it ({length(feed.files)})")
    t |> success(find_index(feed.files, "test_tts_kitten.das") < 0, "and not the tests that do not")
    let oracle <- reach_for_path("modules/dasLLAMA/tests/_attn_oracle.das")
    t |> success(find_index(oracle.files, "test_vulkan_kernels.das") >= 0 && find_index(oracle.files, "test_metal_decode_kernels.das") < 0, "a fixture's reach stops at the census: a model-suite file takes --arm ({join(oracle.files, ", ")})")
    let tier <- reach_for_path("modules/dasLLAMA/tests/_model_tier.das")
    t |> success(length(tier.files) > length(feed.files), "the shared tier fixture reaches more, through the fixtures between ({length(tier.files)})")
    let example_own <- reach_for_path("modules/dasLLAMA/tests/test_parrot.das")
    t |> equal(join(example_own.files, ","), "test_parrot.das", "an on-demand test file reaches itself")
    t |> success(empty(example_own.note), "and is no model-suite file: it takes no --arm ({example_own.note})")
    let rail <- reach_for_path("modules/dasLLAMA/tests/_example_rail.das")
    t |> success(find_index(rail.files, "test_gigaam.das") >= 0, "the examples' fixture reaches the per-PR test that requires it ({join(rail.files, ", ")})")
    for (example in ["test_interpreter.das", "test_parrot.das", "test_storyteller_restart.das", "test_storywish.das"]) {
        t |> success(find_index(rail.files, example) < 0 && find_index(tier.files, example) < 0, "a fixture's change plans no on-demand file: {example}")
    }
    let qwen35 <- reach_for_path("modules/dasLLAMA/dasllama/dasllama_arch_qwen35.das")
    t |> success(!qwen35.core && empty(qwen35.areas) && find_index(qwen35.files, "test_gpu_resident_hybrid.das") >= 0, "a leaf module reaches its witness row, not the core")
    for (arch, file in fixed_array("gemma3", "gemma3", "gemma4", "gemma4", "qwen2", "qwen3", "qwen3", "qwen3moe", "qwen3moe", "llama"),
            fixed_array("test_gemma3v.das", "test_vision_chat.das", "test_gemma4v.das", "test_audio.das", "test_qwen25v.das", "test_qwen3v.das",
                "test_audio.das", "test_qwen3v.das", "test_whisper.das", "test_audio_embedder.das")) {
        let decoder <- reach_for_path("modules/dasLLAMA/dasllama/dasllama_arch_{arch}.das")
        var inscope file_areas <- test_file_areas(file)
        var reached = false
        for (a in decoder.areas) {
            reached ||= find_index(file_areas, a) >= 0
        }
        t |> success(reached, "an arch module reaches the area of a media file loading its decoder: {arch} -> {file} ({join(decoder.areas, ",")})")
    }
    let load <- reach_for_path("modules/dasLLAMA/dasllama/dasllama_load.das")
    t |> success(load.core, "a core module reaches the core")
    t |> success(find_index(load.files, "test_vulkan_kernels.das") >= 0 && find_index(load.files, "test_gpu_resident_llama.das") >= 0, "every model-free llm file and the core witnesses")
    t |> success(find_index(load.files, "test_gpu_resident_moe_formats.das") < 0, "not the whole stocked set")
    t |> equal(join(load.areas, ","), "audio,vision,tts,translate,infra", "and every other area whole")
    let audio <- reach_for_path("modules/dasLLAMA/dasllama/dasllama_audio.das")
    t |> success(empty(audio.files) && join(audio.areas, ",") == "audio" && !audio.core, "an audio module reaches audio whole and no llm file")
    let codec <- reach_for_path("modules/dasLLAMA/dasllama/dasllama_kv_codec.das")
    t |> success(join(codec.areas, ",") == "audio" && find_index(codec.files, "test_kv_codec.das") >= 0 && !codec.core, "a two-area module reaches its other area whole and its llm witnesses")
    let bench <- reach_for_path("modules/dasLLAMA/benchmarks/lcpp_bench.das")
    t |> equal(join(bench.files, ","), join(BENCH_WITNESSES, ","), "a benchmark reaches the bench rows' files")
    let outside <- reach_for_path("src/ast/ast_parse.cpp")
    t |> success(empty(outside.files) && empty(outside.areas) && empty(outside.note), "a path outside the module reaches nothing")
    var listed <- free_files()
    listed |> push_from(suite_files("stocked"))
    let census <- {for (f in listed); base_name(f) => null}
    for (name in LLM_CORE_WITNESSES) {
        t |> success(key_exists(census, name), "a core witness is a census file: {name}")
    }
    for (stem, row in keys(MODULE_WITNESSES), values(MODULE_WITNESSES)) {
        var st : FStat
        t |> success(stat("modules/dasLLAMA/dasllama/{stem}.das", st), "a witness row names a module that exists: {stem}")
        for (name in split(row, ",")) {
            t |> success(key_exists(census, name), "a witness row names census files: {stem} -> {name}")
        }
        let areas = MODULE_AREAS?[stem] ?? ""
        t |> success(find(areas, "llm") >= 0, "a witness row's module has a MODULE_AREAS row holding llm - a module with no row runs every area whole: {stem}")
    }
}

//! the `require` lines of the fixtures whose cells run the Metal
//! drivers' code: every `_metal_*_cells` fixture and `_mlx_cells`
def private metal_cells_requires() : array<string> {
    var out : array<string>
    fio::dir(TESTS_DIR) $(name) {
        let cells = name |> starts_with("_metal_") && name |> ends_with("_cells.das")
        if (cells || name == "_mlx_cells.das") {
            out |> push("require {slice(name, 0, length(name) - 4)}")
        }
    }
    return <- out
}

[test(description = "Every llm census file that requires a Metal cells fixture (`_metal_*_cells`, `_mlx_cells`), in its own text or through the fixtures it requires, is in the witness row of each Metal driver module - dasllama_metal_decode, dasllama_metal_prefill, dasllama_metal_mtp_gemma - over at least ten such files and five such fixtures")]
def test_metal_witnesses_hold_metal_cells_files(t : T?) {
    var inscope needles <- metal_cells_requires()
    t |> success(length(needles) >= 5, "the Metal cells fixtures were found ({length(needles)})")
    var listed <- free_files()
    listed |> push_from(suite_files("stocked"))
    var holders = 0
    for (f in listed) {
        let name = base_name(f)
        var inscope areas <- test_file_areas(name)
        continue if (find_index(areas, "llm") < 0)
        var inscope seen : table<string>
        continue if (!reaches_text(f, needles, -1, seen))
        holders++
        for (stem in fixed_array("dasllama_metal_decode", "dasllama_metal_prefill",
                "dasllama_metal_mtp_gemma")) {
            var inscope row <- split(MODULE_WITNESSES?[stem] ?? "", ",")
            t |> success(find_index(row, name) >= 0,
                "{name} requires a Metal cells fixture and the {stem} witness row lacks it")
        }
    }
    t |> success(holders >= 10, "the llm files holding Metal cells were found ({holders})")
}

//! the children's per-test module cache: a folder keyed by the binary, the folders of other binaries removed on
//! the runner's own binary and kept on a --das dry run, no folder for a binary that cannot be stat'd, and the
//! child command carrying --module-cache-dir
[test(description = "`child_cache_dir` keys the children's module cache by binary size and mtime, removes other binaries' folders only for the runner's own binary, gives none to an unstattable one, and children get --module-cache-dir")]
def test_child_cache_dir(t : T?) {
    t |> equal(child_cache_dir("no/such/binary", true), "", "a binary that cannot be stat'd gets no cache folder")
    let own = das_exe()
    var st : FStat
    t |> success(stat(own, st), "the running binary stats: {own}")
    let want = "{CHILD_CACHE_ROOT}/{int64(st.size)}-{int64(st.mtime)}"
    let stale = "{CHILD_CACHE_ROOT}/0-0"
    mkdir_rec(stale)
    t |> equal(child_cache_dir(own, false), want, "the folder is keyed by the binary's size and mtime")
    var kept : FStat
    t |> success(stat(stale, kept) && kept.is_dir, "a --das run keeps the other binaries' folders")
    t |> equal(child_cache_dir(own, true), want, "the runner's own binary gets the same folder")
    var gone : FStat
    t |> success(!stat(stale, gone), "and removes the other binaries' folders")
    var mine : FStat
    t |> success(stat(want, mine) && mine.is_dir, "its own folder stands")
    return if (get_platform_name() == "windows")   //! the dry run's no-op child binary is a POSIX one, as test_runner_dry_run's
    var out : string
    t |> equal(run_runner("--area infra --das /usr/bin/true", out), 0, "an infra dry run exits clean:\n{out}")
    t |> success(find(out, "CACHE ") >= 0, "the CACHE line says where the children's records go, or that they get none:\n{out}")
    t |> success(find(out, "CACHE removed") < 0, "a --das dry run removes no folder:\n{out}")
    if (find(out, "CACHE none") < 0) {
        t |> success(find(out, " --module-cache-dir {CHILD_CACHE_ROOT}/") >= 0, "every child command carries the cache folder:\n{out}")
    }
}

[test(description = "`pass_stamp` changes with the box's CPU, with the environment the child runs under and with the size, the time or the presence of a model file the child opens, and holds while they hold; a run hands every child the skip flag with its stamp, and a --rerun hands none")]
def test_pass_stamp(t : T?) {
    let dir = path_join(test_temp_dir(), "pass_stamp_models")
    mkdir_rec(dir)
    let model = path_join(dir, "a.gguf")
    fwrite(model, "one")
    let names <- ["a.gguf", "absent.gguf"]
    let base = pass_stamp(dir, names, "cpu one", "DASLLAMA_GPU=1")
    t |> equal(pass_stamp(dir, names, "cpu one", "DASLLAMA_GPU=1"), base, "the same box, environment and files give the same stamp")
    t |> success(pass_stamp(dir, names, "cpu two", "DASLLAMA_GPU=1") != base, "another CPU is another stamp")
    t |> success(pass_stamp(dir, names, "cpu one", "DASLLAMA_GPU=1\nDASLLAMA_CUDA=1") != base, "another GPU home is another stamp")
    t |> success(pass_stamp(dir, names, "cpu one", "") != base, "and so is no GPU home")
    fwrite(path_join(dir, "absent.gguf"), "x")
    let landed = pass_stamp(dir, names, "cpu one", "DASLLAMA_GPU=1")
    t |> success(landed != base, "a model file that lands is another stamp")
    fwrite(model, "longer")
    let resized = pass_stamp(dir, names, "cpu one", "DASLLAMA_GPU=1")
    t |> success(resized != landed, "a model file of another size is another stamp")
    var built : FStat
    var why : string
    let retimed = stat(das_exe(), built) && set_mtime(model, built.mtime, why)
    t |> success(retimed, "the model file takes the binary's time: {why}")
    t |> success(pass_stamp(dir, names, "cpu one", "DASLLAMA_GPU=1") != resized, "a model file of another time is another stamp")
    t |> equal(cached_files("CACHED a.das\n\x1b[32mCACHED b.das\x1b[0m\n[I] a line that says CACHED later\nPASS c.das\n"), 2, "a child's CACHED lines count, colored or not, and no other line does")
    var cfg = RunArgs()
    t |> success(skips_cached(cfg), "a run reports a file from its pass on record, the full suite's as any other")
    cfg.rerun = true
    t |> success(!skips_cached(cfg), "a --rerun runs every file")
    t |> equal(cached_report(0), "", "a run that ran every file says nothing of files not run")
    t |> success(find(cached_report(3), " 3 of them not run") == 0 && find(cached_report(3), "--rerun") > 0, "the REPORT's count of files not run names --rerun: {cached_report(3)}")
    return if (get_platform_name() == "windows")   //! the dry run's no-op child binary is a POSIX one, as test_runner_dry_run's
    var out : string
    t |> equal(run_runner("--area infra --das /usr/bin/true", out), 0, "an infra dry run exits clean:\n{out}")
    t |> success(find(out, " --skip-fully-cached --pass-stamp 0x") >= 0, "a child command carries the flag and its stamp:\n{out}")
    t |> equal(run_runner("--area infra --das /usr/bin/true --rerun", out), 0, "a --rerun infra dry run exits clean:\n{out}")
    t |> success(find(out, "--skip-fully-cached") < 0, "a --rerun's children get no flag:\n{out}")
}

[test(description = "`child_chunks` batches files in order by the batch size, and gives the image file on area runs, an expected-failure compile test, and a tier contract file a child of its own that leaves the files around it in one batch")]
def test_child_chunks(t : T?) {
    let files <- [for (i in range(7)); "{TESTS_DIR}/test_{i}.das"]
    let threes <- child_chunks(files, 3, false)
    t |> equal(length(threes), 3, "seven files three a child: three children")
    t |> equal(join(threes[0], ","), "{TESTS_DIR}/test_0.das,{TESTS_DIR}/test_1.das,{TESTS_DIR}/test_2.das", "the first child takes the first three in order")
    t |> equal(length(threes[2]), 1, "the tail child takes the one left")
    let ones <- child_chunks(files, 1, false)
    t |> equal(length(ones), 7, "one a child is one child a file")
    var with_image <- [for (i in range(2)); "{TESTS_DIR}/test_{i}.das"]
    with_image |> push("{TESTS_DIR}/test_model_image.das")
    with_image |> push("{TESTS_DIR}/test_9.das")
    let area <- child_chunks(with_image, 10, true)
    t |> equal(length(area), 2, "an area run takes the image file out: the image file, and the files around it as one batch")
    t |> equal(join(area[0], ","), "{TESTS_DIR}/test_model_image.das", "the image file's child carries it alone")
    t |> equal(join(area[1], ","), "{TESTS_DIR}/test_0.das,{TESTS_DIR}/test_1.das,{TESTS_DIR}/test_9.das", "the files before and after it ride one child")
    let suite <- child_chunks(with_image, 10, false)
    t |> equal(length(suite), 1, "a suite run batches the image file like any other")
    var with_failed <- [for (i in range(2)); "{TESTS_DIR}/test_{i}.das"]
    with_failed |> push("{TESTS_DIR}/failed_lint.das")
    with_failed |> push("{TESTS_DIR}/test_9.das")
    let expected_failure <- child_chunks(with_failed, 10, false)
    t |> equal(length(expected_failure), 2, "an expected-failure compile test leaves the batch whole: itself, and the files around it")
    t |> equal(join(expected_failure[0], ","), "{TESTS_DIR}/failed_lint.das", "it runs alone, where dastest reads its expected errors")
    var with_tier <- [for (i in range(2)); "{TESTS_DIR}/test_{i}.das"]
    with_tier |> push("{TESTS_DIR}/test_gpu_tier.das")
    with_tier |> push("{TESTS_DIR}/test_9.das")
    let tier <- child_chunks(with_tier, 10, false)
    t |> equal(length(tier), 2, "a tier contract file leaves the batch whole: itself, and the files around it")
    t |> equal(join(tier[0], ","), "{TESTS_DIR}/test_gpu_tier.das", "it runs alone, where no batch-mate's entry arms the tier at [init]")
    t |> equal(length(tier[1]), 3, "the files before and after it ride one child")
}

[test(description = "`model_names` reads every whole quoted .gguf name out of a file's text once and skips interpolated names, paths and patterns, `model_paths` keeps a path under the models folder and drops a climb, an absolute path, a drive path, a backslash path and the bare suffix, and `missing_models` lists the names a run's files and the fixtures they require ask that the directory lacks, each with its askers, while a directory holding them lists none")]
def test_missing_models(t : T?) {
    let names <- model_names("let a = \"B-model.gguf\"\nlet b = \"\{stem\}.gguf\"\nlet c = \"dir/C.gguf\"\nlet d = \"*.gguf\"\nrun(\"A-model.gguf\", \"B-model.gguf\", \"note.txt\")\n")
    t |> equal(join(names, ","), "A-model.gguf,B-model.gguf", "whole quoted names once each, in order; an interpolated name, a path and a pattern are not names")
    let paths <- model_paths("\"../x.gguf\" \"/a/b.gguf\" \"D:/m/g.gguf\" \"d\\e.gguf\" \"drafters/d/f.gguf\" \".gguf\"")
    t |> equal(join(paths, ","), "drafters/d/f.gguf", "`model_paths` keeps a path under the models folder and drops a climb, an absolute path, a drive path, a backslash path and the suffix alone (no file's name)")
    let dir = path_join(test_temp_dir(), "dasllama_missing_models")
    mkdir_rec(dir)
    let run <- ["{TESTS_DIR}/test_gpu_model_swap.das"]
    let missing <- missing_models(run, dir)
    t |> success(!empty(missing), "an empty directory misses what the file names")
    let smol = find_index_if(missing) $(m) => m.name == "SmolLM2-135M-Instruct-Q8_0.gguf"
    t |> success(smol >= 0, "the file's own model is named: {missing}")
    t |> success(smol >= 0 && find(missing[smol].asked, "test_gpu_model_swap.das") >= 0, "with the file that names it")
    let via <- missing_models(["{TESTS_DIR}/test_gpu_resident_hybrid.das"], dir)
    let fixture = find_index_if(via) $(m) => find(m.asked, "_mtp_resident_cells.das") >= 0
    t |> success(fixture >= 0, "a fixture the file requires is read too: {via}")
    for (m in missing) {
        fopen(path_join(dir, m.name), "wb") $(f) {
            f |> fwrite("x")
        }
    }
    t |> equal(length(missing_models(run, dir)), 0, "a directory that holds every named file misses none")
    var why : string
    rmdir_rec(dir, why)
}

[test(description = "`missing_models` reports a stocked model a file names and no file the test writes: a name every line hands to a scratch call - `test_temp_dir()`, a function of the scratch fixture, or a function of the file that calls one - is synthesized, one plain line makes it a model, and a model-free suite's file names no model at all")]
def test_missing_models_leave_scratch_files(t : T?) {
    let fixture = "def scratch_path(name : string) : string => name\ndef arch_gguf(name, arch : string) : string => name\n"
    let text = ("def private synth_file(name : string) : string \{\n    return scratch_path(name)\n\}\n"
        + "def plain(name : string) : string => name\n"
        + "let a = arch_gguf(\"made.gguf\", \"x\")\nlet b = synth_file(\"wrapped.gguf\")\n"
        + "let c = path_join(test_temp_dir(), \"temp.gguf\")\nlet d = plain(\"stocked.gguf\")\n"
        + "let e = scratch_path(\"both.gguf\")\nlet f = path_join(models_dir(), \"both.gguf\")\n")
    var inscope calls <- scratch_calls(text, fixture)
    t |> success(has_value(calls, "arch_gguf(") && has_value(calls, "synth_file(") && !has_value(calls, "plain("),
        "the fixture's functions and the file's own wrapper are scratch calls, a plain function is none: {calls}")
    for (made in fixed_array("made.gguf", "wrapped.gguf", "temp.gguf")) {
        t |> success(names_only_written(text, made, calls), "{made} is a file the test writes")
    }
    t |> success(!names_only_written(text, "stocked.gguf", calls), "a name no scratch call takes is a model (the control)")
    t |> success(!names_only_written(text, "both.gguf", calls), "a name one line reads from the models directory is a model")
    t |> success(!names_only_written(text, "unnamed.gguf", calls), "a name the text does not hold is not synthesized")
    let dir = path_join(test_temp_dir(), "dasllama_missing_scratch")
    mkdir_rec(dir)
    let missing <- missing_models(["{TESTS_DIR}/test_tts_g2p_ru.das"], dir)
    let names <- [for (m in missing); m.name]
    t |> success(has_value(names, "ru-stress-rubert-base.gguf"), "the file's stocked model is missing: {names}")
    t |> success(!has_value(names, "kokoro_arch.gguf"), "a GGUF the scratch fixture writes is not: {names}")
    t |> success(!has_value(names, "ru_stress_head_12.gguf"), "nor one the file's own writer writes: {names}")
    let free <- missing_models(["{TESTS_DIR}/test_gguf_shards.das"], dir)
    t |> equal(length(free), 0, "a model-free suite's file names no stocked model: {free}")
    var why : string
    rmdir_rec(dir, why)
}

[test(description = "`reap_images` removes the .dlim and .tmp files a directory gained since the kept set was taken, counts them and their bytes, and leaves the kept images and every other file; a second reap removes nothing")]
def test_reap_images(t : T?) {
    let dir = path_join(test_temp_dir(), "dasllama_reap_images")
    mkdir_rec(dir)
    let put = @(name : string; text : string) {
        fopen(path_join(dir, name), "wb") $(f) {
            f |> fwrite(text)
        }
    }
    put("old.gguf.q8.dlim", "kept")
    put("model.gguf", "model")
    let kept <- image_set(dir)
    t |> equal(length(kept), 1, "the kept set is the images there at the start")
    put("new.gguf.q8.dlim", "12345")
    put("killed.dlim.77.tmp", "123")
    put("notes.txt", "n")
    let reaped = reap_images(dir, kept)
    t |> equal(reaped.count, 2, "the new image and the temp file go")
    t |> equal(reaped.bytes, 8l, "their bytes are counted")
    t |> success(stat(path_join(dir, "old.gguf.q8.dlim")).is_valid && stat(path_join(dir, "model.gguf")).is_valid && stat(path_join(dir, "notes.txt")).is_valid, "the kept image, the model and every other file stay")
    t |> success(!stat(path_join(dir, "new.gguf.q8.dlim")).is_valid && !stat(path_join(dir, "killed.dlim.77.tmp")).is_valid, "the reaped files are gone")
    t |> equal(reap_images(dir, kept).count, 0, "a second reap finds nothing")
    mkdir_rec(path_join(dir, "drafters"))
    put("drafters/sub.gguf.q8.dlim", "1234")
    put("drafters/sub.gguf", "model")
    var cfg = RunArgs()
    t |> equal(reap_after(cfg, dir, kept, "a child"), 0, "a run without --reap-images removes nothing")
    t |> success(stat(path_join(dir, "drafters/sub.gguf.q8.dlim")).is_valid, "... the image a subfolder gained still there")
    cfg.reap_images = true
    t |> equal(reap_after(cfg, dir, kept, "a child"), 1, "with the flag the image a subfolder gained goes")
    t |> success(!stat(path_join(dir, "drafters/sub.gguf.q8.dlim")).is_valid && stat(path_join(dir, "drafters/sub.gguf")).is_valid, "... and the model beside it stays")
    var why : string
    rmdir_rec(dir, why)
}

[test(description = "A `--poison-kv` run hands every child DASLLAMA_KV_INIT=poison on both shells' forms, and a run without the flag names no K/V start")]
def test_poison_kv_envs(t : T?) {
    var cfg = RunArgs()
    t |> success(find(child_envs(cfg, false), "DASLLAMA_KV_INIT") < 0 && find(child_envs(cfg, true), "DASLLAMA_KV_INIT") < 0, "no flag, no K/V start named: the children take the product's")
    cfg.poison_kv = true
    t |> success(find(child_envs(cfg, false), "DASLLAMA_KV_INIT=poison ") >= 0, "the posix prefix: {child_envs(cfg, false)}")
    t |> success(find(child_envs(cfg, true), "set \"DASLLAMA_KV_INIT=poison\"&& ") >= 0, "the cmd.exe form: {child_envs(cfg, true)}")
}

[test(description = "A run of media-area files alone is a media run and only its Linux children's heap keeps a freed model's pages; a run holding one other file, or none, is no media run and names no heap setting")]
def test_media_run_heap(t : T?) {
    let cfg = RunArgs()
    t |> success(media_run(["{TESTS_DIR}/test_whisper.das", "{TESTS_DIR}/test_nmt.das", "{TESTS_DIR}/test_model_image.das"]), "audio, translate and the image suite: a media run")
    t |> success(!media_run(["{TESTS_DIR}/test_whisper.das", "{TESTS_DIR}/test_run_suites.das"]), "one file no media area lists: no media run")
    var inscope none : array<string>
    t |> success(!media_run(none), "no file: no media run")
    let linux = get_platform_name() == "linux"
    t |> equal(find(child_envs(cfg, false, true), "glibc.malloc.mmap_max=0:glibc.malloc.trim_threshold=") >= 0, linux, "a media run's children keep the heap on Linux alone: {child_envs(cfg, false, true)}")
    t |> success(find(child_envs(cfg, false, false), "glibc.malloc") < 0, "any other run names no heap setting: {child_envs(cfg, false, false)}")
    t |> success(find(child_envs(cfg, true, true), "glibc.malloc") < 0, "nor does cmd.exe's form")
}

//! the `--pass-stamp` values of a dry run's output, in order: every line's, or the START lines' alone where `start_only`
def private pass_stamps(out : string; start_only : bool) : array<string> {
    var stamps : array<string>
    for (line in split(out, "\n")) {
        let at = find(line, " --pass-stamp ")
        continue if (at < 0 || (start_only && find(line, "START ") < 0))
        let rest = slice(line, at + length(" --pass-stamp "))
        let end = find(rest, " ")
        stamps |> push(end < 0 ? rest : slice(rest, 0, end))
    }
    return <- stamps
}

//! the first `--pass-stamp` value a dry run's output carries, "" where none
def private first_stamp(out : string) : string {
    var inscope stamps <- pass_stamps(out, start_only = false)
    return empty(stamps) ? "" : stamps[0]
}

[test(description = "`pass_stamps` reads each line's `--pass-stamp` value to the next space or the line's end, the START lines' alone where asked, and `first_stamp` is the first of every line's, \"\" where none")]
def test_pass_stamps(t : T?) {
    let out = "PLAN x --pass-stamp p0 y\nSTART a --pass-stamp s1 -jit\nSTART b --pass-stamp s2\nSTART c no stamp"
    t |> equal(join(pass_stamps(out, start_only = true), ","), "s1,s2", "the START lines' stamps, the last read to its line's end")
    t |> equal(join(pass_stamps(out, start_only = false), ","), "p0,s1,s2", "every line's stamp")
    t |> equal(first_stamp(out), "p0", "the first stamp of any line")
    t |> equal(first_stamp("START c no stamp"), "", "no stamp, no value")
}

[test(description = "A dry run's children carry a pass stamp that changes with the GPU home and holds across the scratch folder, a named scratch folder is the children's TMPDIR and is announced, the full suite's children record their passes too, keyed apart, only a media run's Linux children keep the heap, the whisper-class tower's image arm is audio's alone, and a media file's name outside the tests folder is no media file")]
def test_run_wiring(t : T?) {
    t |> success(find_index(split(area_image_arms("audio"), ","), "tower") >= 0, "the tower image arm is the audio area's")
    t |> success(find_index(split(area_image_arms("vision"), ","), "tower") < 0, "and not the vision area's")
    t |> success(!media_run(["elsewhere/test_whisper.das"]), "a media file's name outside the tests folder is no media file")
    return if (get_platform_name() == "windows")   //! a POSIX no-op child
    let dry = "--area infra --das /usr/bin/true"
    var vulkan, cuda, moved, full, audio : string
    let scratch_unset_env = "DASLLAMA_SCRATCH_DIR= "   //! a box that names a scratch folder for its own runs names none for these
    let one_home = "DASLLAMA_GPU=1 DASLLAMA_CUDA= "   //! a box on the CUDA home names it for its own runs
    t |> equal(run_runner(dry, vulkan, "{scratch_unset_env}{one_home}"), 0, "a dry run on one GPU home exits clean:\n{vulkan}")
    t |> equal(run_runner(dry, cuda, "{scratch_unset_env}DASLLAMA_GPU=1 DASLLAMA_CUDA=1 "), 0, "and on another:\n{cuda}")
    let scratch = path_join(test_temp_dir(), "wiring_scratch")
    t |> equal(run_runner(dry, moved, "{one_home}DASLLAMA_SCRATCH_DIR='{scratch}' "), 0, "and with a scratch folder named:\n{moved}")
    t |> success(!empty(first_stamp(vulkan)), "a child carries a pass stamp:\n{vulkan}")
    t |> success(first_stamp(cuda) != first_stamp(vulkan), "another GPU home is another stamp ({first_stamp(vulkan)} / {first_stamp(cuda)})")
    t |> equal(first_stamp(moved), first_stamp(vulkan), "the scratch folder is no part of the stamp")
    t |> success(find(moved, "TMPDIR=\"{scratch}\" ") >= 0, "a child's command line carries the scratch folder as its TMPDIR:\n{moved}")
    t |> success(find(moved, "SCRATCH {scratch}") >= 0 && stat(scratch).is_dir, "the run announces the folder and makes it:\n{moved}")
    t |> success(find(vulkan, "TMPDIR=\"") < 0 && find(vulkan, "SCRATCH ") < 0, "a run that names none hands its children none:\n{vulkan}")
    t |> equal(run_runner(dry, full, "{scratch_unset_env}DASLLAMA_PARITY_FULL=1 "), 0, "a dry run of the full suite exits clean:\n{full}")
    var plain : string
    t |> equal(run_runner(dry, plain, "{scratch_unset_env}DASLLAMA_PARITY_FULL= "), 0, "a dry run outside the full suite exits clean:\n{plain}")
    t |> success(!empty(first_stamp(full)) && first_stamp(full) != first_stamp(plain), "the full suite's children record their passes too, keyed apart from a default run's ({first_stamp(plain)} / {first_stamp(full)})")
    t |> equal(run_runner("--area audio --das /usr/bin/true", audio, scratch_unset_env), 0, "a dry audio run exits clean:\n{audio}")
    let linux = get_platform_name() == "linux"
    t |> equal(find(audio, "glibc.malloc.mmap_max=0") >= 0, linux, "a media run's children keep the heap on Linux alone:\n{audio}")
    t |> success(find(vulkan, "glibc.malloc") < 0, "a run of another area's files names no heap setting:\n{vulkan}")
}

[test(description = "A scratch folder named for the run reaches every child as its TMPDIR on the posix form and as TEMP and TMP on cmd.exe's, and a run that names none hands its children no scratch folder")]
def test_child_scratch_envs(t : T?) {
    let cfg = RunArgs()
    t |> success(find(child_envs(cfg, false, false, "/disk/scratch"), "TMPDIR=\"/disk/scratch\" ") >= 0, "the posix prefix: {child_envs(cfg, false, false, "/disk/scratch")}")
    let cmd = child_envs(cfg, true, false, "D:\\scratch")
    t |> success(find(cmd, "set \"TEMP=D:\\scratch\"&& ") >= 0 && find(cmd, "set \"TMP=D:\\scratch\"&& ") >= 0, "the cmd.exe form: {cmd}")
    t |> success(find(child_envs(cfg, false), "TMPDIR") < 0 && find(child_envs(cfg, true), "TEMP") < 0, "no scratch folder named: the children make theirs under the runner's")
}

//! a models directory under the test's temp folder holding a one-byte file at every model path `files` and the fixtures
//! they reach name, subfolders made
def private stocked_models_dir(name : string; files : array<string>) : string {
    let dir = path_join(test_temp_dir(), name)
    var why : string
    rmdir_rec(dir, why)
    for (stem in keys(reached_names(files))) {
        for (model in model_paths(fread("{TESTS_DIR}/{stem}"))) {
            let path = path_join(dir, model)
            mkdir_rec(dir_name(path))
            fopen(path, "wb") $(f) {
                f |> fwrite("x")
            }
        }
    }
    return dir
}

//! the path lines of a `--list-models` text, its child lines left out
def private listed_paths(text : string) : array<string> =>
    <- [for (ln in split(text, "\n")); ln; where !empty(ln) && !(ln |> starts_with("#"))]

[test(description = "`--list-models` prints the model files a run opens child by child in start order - a `# child <n>` line ahead of each, a file an earlier child listed not repeated, subfolder paths and a NextN head beside its trunk included, no category's file on a run that names none, only the named category's files on a category run - and the runner prints it alone and starts no child")]
def test_list_models(t : T?) {
    var cfg = RunArgs()
    cfg.suite = "stocked"
    cfg.batch = 1
    var inscope files <- only_files(suite_files("stocked"), "resident_llama,gemma4_e2b")
    t |> equal(length(files), 2, "the --only set: {files}")
    let dir = stocked_models_dir("dasllama_list_models", files)
    fopen(path_join(dir, "mtp-gemma-4-E2B-it-Q8_0.gguf"), "wb") $(f) {
        f |> fwrite("x")
    }
    var inscope chunks <- run_chunks(cfg, files, false)
    let text = model_list_text(cfg, chunks, dir, false)
    var inscope paths <- listed_paths(text)
    t |> success(find(text, "# child 0: {child_tag(chunks[0])}\n") == 0 && find(text, "# child 1: {child_tag(chunks[1])}\n") > 0, "a child line ahead of each child, in start order:\n{text}")
    t |> success(has_value(paths, "Llama-3.2-1B-Instruct-Q8_0.gguf") && has_value(paths, "gemma-4-E2B-it-Q8_0.gguf"), "each file's own models are listed:\n{text}")
    t |> success(has_value(paths, "drafters/gemma-4-E2B-it-dflash2-GGUF/gemma-4-E2B-it-dflash2-Q8_0.gguf"), "a model in a subfolder by its path under the directory:\n{text}")
    t |> success(has_value(paths, "mtp-gemma-4-E2B-it-Q8_0.gguf"), "the NextN head the trunk's load reads beside it:\n{text}")
    let unique <- {for (p in paths); p}
    t |> equal(length(unique), length(paths), "no file is listed twice:\n{text}")
    var inscope twice <- [[files[0]], [files[0]]]
    let again = model_list_text(cfg, twice, dir, false)
    let second = slice(again, find(again, "# child 1: "))
    t |> success(!empty(listed_paths(again)) && second == "# child 1: {base_name(files[0])}\n", "a model an earlier child listed is not listed again:\n{again}")
    var named_category = false
    for (stem in keys(reached_names(files))) {
        for (model in model_paths(fread("{TESTS_DIR}/{stem}"))) {
            named_category = named_category || !empty(model_category(model))
        }
    }
    t |> success(named_category, "the files name a category's model (the control the category filter needs)")
    t |> success(!has_value([for (p in paths); !empty(model_category(p))], true), "a run that names no category lists no category's file:\n{text}")
    var cat_cfg = RunArgs()
    cat_cfg.category = "gptoss-20b"
    var inscope cat_files <- category_files(cat_cfg.category)
    let cat_dir = stocked_models_dir("dasllama_list_models_category", cat_files)
    var inscope cat_paths <- listed_paths(model_list_text(cat_cfg, run_chunks(cat_cfg, cat_files, false), cat_dir, false))
    t |> success(!empty(cat_paths) && !has_value([for (p in cat_paths); model_category(p) == "gptoss-20b"], false), "a category run lists its category's files and no other model: {cat_paths}")
    if (get_platform_name() != "windows") {
        var out : string
        let rc = run_runner("--suite stocked --only resident_llama,gemma4_e2b --batch 1 --list-models --das /usr/bin/false", out, "DASLLAMA_MODELS_DIR={dir} ")
        t |> equal(rc, 0, "the list exits clean:\n{out}")
        let cache_line = "deser: clean - all modules from cache\n"   //! the test's own `-module-cache` flag prints it, ahead of the runner
        out = (out |> starts_with(cache_line)) ? slice(out, length(cache_line)) : out
        t |> success(find(out, "# child 0: ") == 0 && find(out, "START ") < 0 && find(out, "REPORT") < 0 && find(out, "ONLY ") < 0, "the runner prints the list alone and starts no child:\n{out}")
        t |> success(has_value(listed_paths(out), "gemma-4-E2B-it-Q8_0.gguf"), "the runner's list is the run's:\n{out}")
    } else {
        t |> skip("the end-to-end list runs the runner under a POSIX environment prefix")
    }
    var why : string
    rmdir_rec(dir, why)
    rmdir_rec(cat_dir, why)
}

[test(description = "`copy_gate` starts a child whose model files are all local, stops the run when the copier failed while a file is still copying, starts a child over the links once the copier is done, waits while files copy, and stops the run once no file landed for the bound; `pending_models` counts an absent file and a link, never a regular file")]
def test_copy_gate(t : T?) {
    t |> success(copy_gate(0, false, "", 0.0, AWAIT_IDLE_SECONDS) is start, "every file local: the child starts")
    t |> success(copy_gate(0, false, "a.gguf", 0.0, AWAIT_IDLE_SECONDS) is start, "... even after a failed copy - the child needs nothing more of the copier")
    let failed = copy_gate(2, false, "drafters/x/a.gguf", 0.0, AWAIT_IDLE_SECONDS)
    t |> success(failed is stop && find(failed as stop, "drafters/x/a.gguf") >= 0, "a failed copier stops the run, naming the file: {failed}")
    let done = copy_gate(2, true, "", 0.0, AWAIT_IDLE_SECONDS)
    t |> success(done is start && find(done as start, "2 file(s)") >= 0, "a done copier starts the child over the links it was not given: {done}")
    let wait = copy_gate(3, false, "", AWAIT_IDLE_SECONDS - 1.0, AWAIT_IDLE_SECONDS)
    t |> success(wait is wait && (wait as wait) == 3, "files copying under the bound: the child waits on them: {wait}")
    let idle = copy_gate(3, false, "", AWAIT_IDLE_SECONDS, AWAIT_IDLE_SECONDS)
    t |> success(idle is stop && find(idle as stop, "1800 s") >= 0, "no file landed for the bound: the run stops red: {idle}")
    t |> equal(AWAIT_IDLE_SECONDS, 1800.0, "the bound is 30 minutes")
    let dir = path_join(test_temp_dir(), "dasllama_pending_models")
    var why : string
    rmdir_rec(dir, why)
    mkdir_rec(path_join(dir, "sub"))
    fopen(path_join(dir, "sub/real.gguf"), "wb") $(f) {
        f |> fwrite("x")
    }
    t |> equal(pending_models(dir, ["sub/real.gguf"]), 0, "a regular file is local")
    t |> equal(pending_models(dir, ["sub/real.gguf", "absent.gguf"]), 1, "an absent file is pending")
    if (get_platform_name() != "windows") {
        unsafe(popen("ln -s {path_join(dir, "sub/real.gguf")} {path_join(dir, "link.gguf")}") $(f) {
            let _drain = fread(f)
        })
        t |> success(stat(path_join(dir, "link.gguf")).is_reg, "the link resolves to the regular file (the control)")
        t |> equal(pending_models(dir, ["link.gguf", "sub/real.gguf"]), 1, "a link to a regular file is pending: it is not a copy")
    } else {
        t |> skip("the link half makes a POSIX symlink")
    }
    rmdir_rec(dir, why)
}

[test(description = "A dry runner run under `--await-models` stops red naming the file when the copier failed and a child's model is not local, starts the child over the links once the copier is done, and puts the seconds waited on the REPORT line")]
def test_runner_await_models(t : T?) {
    if (get_platform_name() == "windows") {
        t |> skip("the dry run stands in a POSIX no-op binary for the child")
        return
    }
    let dir = path_join(test_temp_dir(), "dasllama_await_models")
    let state = path_join(dir, "copy")
    var why : string
    rmdir_rec(dir, why)
    mkdir_rec(state)
    fopen(path_join(state, "volume.gguf"), "wb") $(f) {
        f |> fwrite("x")
    }
    for (name in ["Qwen3-0.6B-Q8_0.gguf", "SmolLM2-135M-Instruct-Q8_0.gguf", "tinyllama-1.1b-chat-v1.0.Q8_0.gguf"]) {   //! the child file's models
        unsafe(popen("ln -s {path_join(state, "volume.gguf")} {path_join(dir, name)}") $(f) {
            let _drain = fread(f)
        })
    }
    fopen(path_join(state, "failed"), "wb") $(f) {
        f |> fwrite("SmolLM2-135M-Instruct-Q8_0.gguf\n")
    }
    let env = "DASLLAMA_MODELS_DIR={dir} "
    let args = "--suite stocked --only test_gpu_model_swap --das /usr/bin/true --await-models {state}"
    var red : string
    t |> success(run_runner(args, red, env) != 0, "a failed copy with the child's model not local is red:\n{red}")
    t |> success(find(red, "COPY test_gpu_model_swap.das: the model copy failed: SmolLM2-135M-Instruct-Q8_0.gguf") >= 0 && find(red, "START test_gpu_model_swap.das") < 0, "the stop names the file and starts no child:\n{red}")
    remove(path_join(state, "failed"))
    fopen(path_join(state, "done"), "wb") $(f) {
        f |> fwrite("ok\n")
    }
    var green : string
    t |> equal(run_runner(args, green, env), 0, "a done copier starts the child:\n{green}")
    t |> success(find(green, "START test_gpu_model_swap.das") >= 0 && find(green, " s waited on the model copy") >= 0, "the child starts and the REPORT carries the wait:\n{green}")
    t |> success(find(green, "WAIT child 0/1 test_gpu_model_swap.das: started after ") >= 0 && find(green, "3 file(s) it was not given read where they link") >= 0, "a child started over links says so:\n{green}")
    rmdir_rec(dir, why)
}

[test(description = "Under DASLLAMA_DISABLE_MODULES every child of a run starts with `--disable-module <name>` for each name, right after the binary and ahead of -jit, and its pass stamp is another than the same run's without the knob, whose children carry no such flag; a test's spawned child takes the same flags after the binary (`child_cmd_root_leaving`), and the comma list maps to one flag pair a name")]
def test_runner_disabled_modules(t : T?) {
    let pair = " --disable-module dasVulkan --disable-module dasCUDA"
    t |> equal(disable_module_flags("dasVulkan, dasCUDA"), pair, "one flag pair a name, trimmed")
    t |> equal(disable_module_flags(""), "", "no name, no flag")
    let none : array<tuple<name : string; value : string>>
    let spawned = child_cmd_root_leaving("/x/root.das", none, "", "-jit", "dasCUDA")
    t |> success(find(spawned, "\"{das_exe()}\" --disable-module dasCUDA -jit -dasroot") >= 0,
        "a spawned child leaves the named module out, right after the binary: {spawned}")
    if (get_platform_name() == "windows") {
        t |> skip("the dry run stands in a POSIX no-op binary for the child")
        return
    }
    let dry = "--area translate --das /usr/bin/true"
    var plain : string
    t |> equal(run_runner(dry, plain, "DASLLAMA_DISABLE_MODULES= "), 0, "a dry run is clean:\n{plain}")
    t |> success(find(plain, "--disable-module") < 0 && find(plain, "\"/usr/bin/true\" -jit ") >= 0,
        "without the knob no child leaves a module out:\n{plain}")
    var off : string
    t |> equal(run_runner(dry, off, "DASLLAMA_DISABLE_MODULES=dasVulkan,dasCUDA "), 0,
        "a dry run under the knob exits clean:\n{off}")
    t |> success(find(off, "\"/usr/bin/true\"{pair} -jit ") >= 0,
        "under the knob every child leaves the modules out, ahead of -jit:\n{off}")
    let announced = find(off, "MODULES dasVulkan,dasCUDA (DASLLAMA_DISABLE_MODULES): every child")
    t |> success(announced >= 0 && announced < find(off, "START "),
        "the run announces the modules its children leave out, ahead of its first child:\n{off}")
    t |> success(find(plain, "(DASLLAMA_DISABLE_MODULES)") < 0,
        "the control: a run without the knob announces none:\n{plain}")
    var inscope plain_stamps <- pass_stamps(plain, start_only = true)
    var inscope off_stamps <- pass_stamps(off, start_only = true)
    let stamped = !empty(plain_stamps) && length(plain_stamps) == length(off_stamps)
    t |> success(stamped, "both runs stamp their children")
    for (a, b in plain_stamps, off_stamps) {
        t |> success(a != b, "a pass with a module left out is keyed apart ({a} vs {b})")
    }
}

//! the test child's `disable-flags` report under DASLLAMA_DISABLE_MODULES=`modules` ("" unsets it):
//! its lines by tag
def private disable_flags_report(t : T?; modules : string) : table<string; string> {
    var out = ""
    let envs = [(name = "DASLLAMA_DISABLE_MODULES", value = modules)]
    let rc = run_verb_child(child_verb_cmd("disable-flags", envs), out)
    t |> success(rc == 0 && find(out, "ROOT_DONE") >= 0,
        "the disable-flags verb ran under '{modules}':\n{out}")
    var lines : table<string; string>
    for (line in split(out, "\n")) {
        let sp = find(line, " ")
        if (sp > 0) {
            lines |> insert(slice(line, 0, sp), slice(line, sp + 1))
        }
    }
    return <- lines
}

[test(description = "In a process under DASLLAMA_DISABLE_MODULES each child builder leaves the modules out right after the binary - `daslang_argv`, `child_cmd` through `child_cmd_root`, the compile child's argv - and the test child's exe key moves with the knob; with the knob unset none carries the flag", timeout = 1200)]
def test_child_builders_read_the_knob(t : T?) {
    var inscope on <- disable_flags_report(t, "dasVulkan,dasCUDA")
    var inscope off <- disable_flags_report(t, "")
    let pair = " --disable-module dasVulkan --disable-module dasCUDA"
    let argv = on?["ARGV"] ?? ""
    let cmd = on?["CMD"] ?? ""
    let compile = on?["COMPILE"] ?? ""
    t |> success(find(argv, "das{pair} root.das") >= 0, "a daslang argv leaves them out: {argv}")
    t |> success(find(cmd, "\"{pair} -dasroot") >= 0, "a test's child command too: {cmd}")
    t |> success(find(compile, "{pair} -dasroot") >= 0, "and the compile child: {compile}")
    for (tag in fixed_array("ARGV", "CMD", "COMPILE")) {
        let line = off?[tag] ?? ""
        t |> success(!empty(line) && find(line, "--disable-module") < 0,
            "the control: unset, the {tag} line carries none: {line}")
    }
    let key_on = on?["KEY"] ?? ""
    t |> success(!empty(key_on) && key_on != (off?["KEY"] ?? ""),
        "the exe key moves with the knob ({key_on})")
}

//! the text by which a file or a fixture it requires reaches the Vulkan home's module
let private VULKAN_REACH <- ["require ?vulkan ", "builtin_module_exists(vulkan)"]

//! the model-free files that reach Vulkan and hold no Vulkan cell: the CUDA stamps over
//! `_discrete_cells`' and `_discrete_tts_cells`' shared case bodies, this file, which names the
//! reach text, and the file whose twin verdict reads synthetic rows
let private VULKAN_REACH_NO_CELL <- ["test_cuda_tower_front.das", "test_cuda_tower_blocks.das",
    "test_cuda_tower_asr.das", "test_cuda_tts_kernels.das", "test_run_suites.das",
    "uncategorized__dasllama_internal_persistent_heap_stack_524288.das"]

[test(description = "Every model-free file whose text, or a fixture it requires, reaches the Vulkan module sits in the two-pod fleet Vulkan home's slice of the model-free suite (`VULKAN_FREE_ONLY`) but the declared case-body files - the CUDA home runs the suite without that module - and the slice keeps no file that reaches none")]
def test_vulkan_free_slice(t : T?) {
    var inscope free <- suite_files("model-free")
    let kept <- only_files(free, VULKAN_FREE_ONLY)
    var reach = 0
    for (file in free) {
        var inscope seen : table<string>
        let vulkan = reaches_text(file, VULKAN_REACH, -1, seen)
        let declared = find_index(VULKAN_REACH_NO_CELL, base_name(file)) >= 0
        reach += vulkan ? 1 : 0
        if (vulkan && !declared) {
            t |> success(find_index(kept, file) >= 0,
                "a model-free file reaching Vulkan sits in VULKAN_FREE_ONLY - add a token: {file}")
        }
        if (find_index(kept, file) >= 0) {
            t |> success(vulkan, "VULKAN_FREE_ONLY keeps a file that reaches no Vulkan: {file}")
        }
    }
    t |> success(reach > 10, "the walk found the Vulkan files ({reach})")
}

//! the require of a CUDA kernel-class module with no `?cuda` guard: what makes a test file the CUDA home's alone
let private CUDA_CLASSES_REQUIRE <- ["require dasllama/dasllama_cuda_classes"]

[test(description = "`CUDA_ONLY_EXCLUDE` leaves out exactly the test files that require a CUDA kernel-class module with no `?cuda` guard, themselves or through a fixture, the files a two-pod fleet's Vulkan home does not run")]
def test_cuda_only_exclude(t : T?) {
    var files : array<string>
    dir(TESTS_DIR) $(name) {
        if (name |> ends_with(".das") && (name |> starts_with("test_") || name |> starts_with("uncategorized"))) {
            files |> push("{TESTS_DIR}/{name}")
        }
    }
    var inscope dropped <- exclude_files(files, CUDA_ONLY_EXCLUDE).dropped
    var reach = 0
    for (file in files) {
        continue if (base_name(file) == "test_run_suites.das")   //! this file names the require as a string
        var inscope seen : table<string>
        let cuda_only = reaches_text(file, CUDA_CLASSES_REQUIRE, -1, seen)
        reach += cuda_only ? 1 : 0
        t |> equal(find_index(dropped, file) >= 0, cuda_only,
            "CUDA_ONLY_EXCLUDE leaves out a file exactly where it requires the CUDA kernel classes unguarded: {file}")
    }
    t |> success(reach >= 5, "the walk found the CUDA-only files ({reach})")
}

[test(description = "`box_race_possible` owes the per-box runtime file's race ahead of the first child only on a build with the Metal module whose children read no tune manifest and keep the crowns: not under --no-tune, a manifest, or the reference or fallback policy")]
def test_box_race_possible(t : T?) {
    t |> success(box_race_possible(metal_build = true, no_tune = false, manifest = "", policy = ""), "a Metal build with no manifest races")
    t |> success(box_race_possible(metal_build = true, no_tune = false, manifest = "", policy = "default"), "a crown-keeping policy too")
    t |> success(!box_race_possible(metal_build = false, no_tune = false, manifest = "", policy = ""), "no Metal module, no crowns")
    t |> success(!box_race_possible(metal_build = true, no_tune = true, manifest = "", policy = ""), "--no-tune runs the reference bodies")
    t |> success(!box_race_possible(metal_build = true, no_tune = false, manifest = "/m5.tune.json", policy = ""), "a manifest holds them")
    t |> success(!box_race_possible(metal_build = true, no_tune = false, manifest = "", policy = "reference"), "reference is crownless")
    t |> success(!box_race_possible(metal_build = true, no_tune = false, manifest = "", policy = "fallback"), "and so is the fallback policy")
}

[test(description = "The per-box runtime file is made ready only for a run on its own binary whose children may race the crowns, its manifest the caller's, else the box file where valid, else none; the race program's exit and output make the BOXRACE line: a failed line on a nonzero exit or no file named, the raced line with its seconds, none where a valid file held")]
def test_box_runtime_lines(t : T?) {
    t |> equal(box_manifest_pick("/caller.json", "/box.json", true), "/caller.json",
        "the caller's manifest wins")
    t |> equal(box_manifest_pick("", "/box.json", true), "/box.json",
        "else the box file where valid")
    t |> equal(box_manifest_pick("", "/box.json", false), "", "and none where it is not")
    t |> success(box_runtime_owed(das_standin = "", metal_build = true, no_tune = false, manifest = "", policy = ""),
        "a run on its own binary owes the file where the crowns may race")
    t |> success(!box_runtime_owed(das_standin = "/usr/bin/true", metal_build = true, no_tune = false, manifest = "", policy = ""),
        "a --das stand-in owes none")
    t |> success(!box_runtime_owed(das_standin = "", metal_build = true, no_tune = false, manifest = "/m5.tune.json", policy = ""),
        "nor a run whose children read a manifest")
    let failed = box_race_line(3, "the program's output", 1.0lf)
    let failed_line = find(failed.text, "BOXRACE failed (exit 3)") >= 0
    t |> success(failed.warn && failed_line && find(failed.text, "the program's output") >= 0,
        "a nonzero exit is a failed line carrying the output: {failed.text}")
    let unnamed = box_race_line(0, "BOX_FILE=[]", 1.0lf)
    t |> success(unnamed.warn && find(unnamed.text, "BOXRACE failed (exit 0)") >= 0,
        "a clean exit naming no file fails too: {unnamed.text}")
    let raced = box_race_line(0, "BOX_FILE=[/box.json] metal crowns raced", 42.5lf)
    let raced_line = find(raced.text, "BOXRACE Metal crowns raced") >= 0
    t |> success(!raced.warn && raced_line && find(raced.text, "(42.5s)") >= 0,
        "a race is its line with its seconds: {raced.text}")
    let held = box_race_line(0, "BOX_FILE=[/box.json]", 1.0lf)
    t |> success(!held.warn && empty(held.text),
        "the control: a valid file held says nothing: '{held.text}'")
    t |> equal(disabled_modules_line(""), "", "no module left out, no MODULES line")
}

[test(description = "A run is whole only over every standing area with no narrowing flag and no test filter inherited from the runner's environment, and its verdict child gets DASLLAMA_KERNEL_CENSUS_WHOLE=1 only where the whole run reported no file from an earlier pass and failed none")]
def test_census_whole_run(t : T?) {
    var inscope standing <- standing_areas()
    t |> success(census_whole(RunArgs(), standing, ""),
        "every standing area, nothing narrowing it: whole")
    t |> success(!census_whole(RunArgs(family = "llama"), standing, ""),
        "a --family run is not whole")
    t |> success(!census_whole(RunArgs(only = "qwen"), standing, ""), "an --only run is not whole")
    t |> success(!census_whole(RunArgs(), standing, "gemma4e"),
        "a family filter the runner inherits from its environment narrows the run")
    var inscope one <- [standing[0]]
    t |> success(!census_whole(RunArgs(), one, ""), "one standing area alone is not whole")
    let knob = " --run-env \"DASLLAMA_KERNEL_CENSUS_WHOLE=1\""
    let clean = RunTally()
    t |> equal(census_child_run_env(census_ran_whole(true, clean)), knob,
        "a whole run with every file run and green hands its verdict child the knob")
    t |> equal(census_child_run_env(census_ran_whole(true, RunTally(cached = 1))), "",
        "a file reported from an earlier pass leaves the knob off")
    t |> equal(census_child_run_env(census_ran_whole(true, RunTally(failures = 1))), "",
        "a failed file leaves the knob off")
    t |> equal(census_child_run_env(census_ran_whole(false, clean)), "",
        "a narrowed run's verdict child gets no knob")
}

[test(description = "A dry run of every standing area runs the census verdict child last, hands it DASLLAMA_KERNEL_CENSUS_WHOLE=1 and says so on a CENSUS line, while the same run under an inherited DASLLAMA_TEST_FAMILY, _ARMS or _CATEGORIES and a one-area run hand it none")]
def test_census_whole_dry_run(t : T?) {
    if (get_platform_name() == "windows") {
        t |> skip("the dry run stands in a POSIX no-op binary for the child")
        return
    }
    var inscope standing <- standing_areas()
    let all = join(standing, ",")
    let knob = "DASLLAMA_KERNEL_CENSUS_WHOLE=1"
    let verdict = "START test_kernel_coverage.das"
    //! a run under a filter hands it to its test processes; these runs name their own
    let unfiltered = "DASLLAMA_TEST_FAMILY= DASLLAMA_TEST_ARMS= DASLLAMA_TEST_CATEGORIES= "
    var whole : string
    t |> equal(run_runner("--area {all} --das /usr/bin/true", whole, unfiltered), 0,
        "a dry run of every standing area exits clean:\n{whole}")
    let at = find(whole, verdict)
    t |> success(at >= 0, "the census verdict runs as a child:\n{whole}")
    let line = at >= 0 ? slice(whole, at, find(whole, "\n", at)) : ""
    t |> success(find(line, knob) >= 0, "its command carries the knob:\n{line}")
    t |> success(find(whole, "CENSUS whole:") >= 0, "the CENSUS whole line is printed")
    t |> equal(rfind(whole, "START "), at, "the verdict is the run's last child:\n{whole}")
    for (env in fixed_array("FAMILY=llama", "ARMS=fxarm", "CATEGORIES=fxcat")) {
        var inherited : string
        t |> equal(run_runner("--area {all} --das /usr/bin/true", inherited, "{unfiltered}DASLLAMA_TEST_{env} "), 0,
            "the run under an inherited {env} exits clean:\n{inherited}")
        t |> success(find(inherited, verdict) >= 0 && find(inherited, knob) < 0,
            "an inherited {env} leaves the knob off:\n{inherited}")
    }
    var narrowed : string
    t |> equal(run_runner("--area audio --das /usr/bin/true", narrowed, unfiltered), 0,
        "a one-area dry run exits clean")
    t |> success(find(narrowed, verdict) >= 0 && find(narrowed, knob) < 0,
        "a one-area run leaves the knob off:\n{narrowed}")
}

var private g_census_verdict_ran = false   //! whether a verdict block below ran

[test(description = "The census verdict skips where no census folder is named, fails where a named folder holds no census file of the home, and reads the files where they stand: the merged table and the run's whole flag reach the verdict")]
def test_census_verdict_reads(t : T?) {
    t |> equal(census_read_of("", 0, true), CensusRead.skip_unset, "no folder named: the verdict skips")
    t |> equal(census_read_of("/a/folder", 0, true), CensusRead.fail_empty,
        "a whole run's folder with no file fails")
    t |> equal(census_read_of("/a/folder", 0, false), CensusRead.skip_empty,
        "a narrowed run's folder with no file skips")
    for (whole in [false, true]) {
        t |> equal(census_read_of("/a/folder", 2, whole), CensusRead.read, "files: the verdict reads them")
    }
    let folder = path_join(test_temp_dir(), "census_reads")
    mkdir_rec(folder)
    fopen(path_join(folder, "metal_1.json"), "wb") $(f) {
        fwrite(f, sprint_json({ "k_a" => 3l }, false))
    }
    var inscope none <- kernel_census_merged(folder, CENSUS_VULKAN)
    t |> equal(census_read_of(folder, none.files, true), CensusRead.fail_empty,
        "the folder holds no file of the Vulkan home: that verdict fails on a whole run")
    t |> run("an unset census folder skips") @(t : T?) {
        g_census_verdict_ran = with_kernel_census(t, "", CENSUS_METAL, true) $(_cov, _whole) {
            t |> failure("the verdict ran with no census folder")
        }
    }
    t |> success(!g_census_verdict_ran, "the skip answers false")
    for (whole in [false, true]) {
        g_census_verdict_ran = false
        let read = with_kernel_census(t, folder, CENSUS_METAL, whole) $(cov, got) {
            t |> equal(cov?["k_a"] ?? -1l, 3l, "the verdict reads the merged table")
            t |> equal(got, whole, "the verdict gets the run's whole flag")
            g_census_verdict_ran = true
        }
        t |> success(read && g_census_verdict_ran, "a folder with the home's file runs the verdict")
    }
}

[test(description = "A run is not whole under --changed, --arm, --category, --skip or an --exclude past the CUDA-only files; the census verdict is owed on every run but a category run and the model-free, gpu-kernels and wasm suites'; and the verdict rides a child of its own after every other, a category run's too")]
def test_census_scope(t : T?) {
    var inscope standing <- standing_areas()
    var inscope narrowing <- [(why = "--changed", cfg = RunArgs(changed = true)),
        (why = "--arm", cfg = RunArgs(arm = "x")), (why = "--category", cfg = RunArgs(category = "x")),
        (why = "--exclude", cfg = RunArgs(exclude = "x")), (why = "--skip", cfg = RunArgs(skip = 1))]
    for (n in narrowing) {
        t |> success(!census_whole(n.cfg, standing, ""), "a {n.why} run is not whole")
    }
    t |> success(census_whole(RunArgs(exclude = CUDA_ONLY_EXCLUDE), standing, ""),
        "a fleet Vulkan home's run, the CUDA-only files left out, is whole")
    t |> success(!census_whole(RunArgs(exclude = "{CUDA_ONLY_EXCLUDE},test_audio"), standing, ""),
        "one more file left out and it is not")
    t |> success(census_verdict_owed(RunArgs()), "a plain run owes the verdict")
    t |> success(census_verdict_owed(RunArgs(suite = "stocked")), "the stocked suite owes it")
    t |> success(!census_verdict_owed(RunArgs(category = "x")), "a category run owes none")
    for (suite in fixed_array("model-free", "gpu-kernels", "wasm")) {
        t |> success(!census_verdict_owed(RunArgs(suite = suite)), "the {suite} suite owes none")
    }
    let files <- [CENSUS_VERDICT, "{TESTS_DIR}/test_a.das", "{TESTS_DIR}/test_b.das"]
    for (cfg in [RunArgs(), RunArgs(category = "x")]) {
        var inscope chunks <- run_chunks(cfg, files, false)
        t |> equal(join(chunks[length(chunks) - 1], ","), CENSUS_VERDICT,
            "the verdict is the last child, alone (category '{cfg.category}')")
        t |> equal(length([for (c in chunks); c; where has_value(c, CENSUS_VERDICT)]), 1,
            "and rides no other child")
    }
}

[test(description = "A run's census folder reaches every child as DASLLAMA_KERNEL_CENSUS_DIR on the posix form and cmd.exe's, a run with none names none, and each repetition starts on the folder emptied")]
def test_child_census_envs(t : T?) {
    let cfg = RunArgs()
    let posix = child_envs(cfg, false, false, "", "/disk/census")
    t |> success(find(posix, "DASLLAMA_KERNEL_CENSUS_DIR=\"/disk/census\" ") >= 0, "the posix prefix: {posix}")
    let cmd = child_envs(cfg, true, false, "", "D:\\census")
    t |> success(find(cmd, "set \"DASLLAMA_KERNEL_CENSUS_DIR=D:\\census\"&& ") >= 0, "the cmd.exe form: {cmd}")
    t |> success(find(child_envs(cfg, false), "KERNEL_CENSUS") < 0 && find(child_envs(cfg, true), "KERNEL_CENSUS") < 0,
        "no census folder, none named")
    let folder = path_join(test_temp_dir(), "census_rep")
    mkdir_rec(folder)
    t |> success(fwrite(path_join(folder, "metal_1.json"), "\{\}"), "a past repetition's file is planted")
    census_rep_open(folder)
    var left = 0
    fio::dir(folder) $(name) {
        if (name != "." && name != "..") {
            left++
        }
    }
    t |> success(stat(folder).is_dir && left == 0, "the folder stands, emptied ({left} left)")
}

[test(description = "A dry area run under --arm makes its census folder under the run's scratch folder, hands it to the verdict child and removes it after the run, and hands the image child the asked arm, not the area's union")]
def test_census_folder_dry_run(t : T?) {
    if (get_platform_name() == "windows") {
        t |> skip("the dry run stands in a POSIX no-op binary for the child")
        return
    }
    let scratch = path_join(test_temp_dir(), "census_scratch")
    var out : string
    t |> equal(run_runner("--area audio --arm fxarm --das /usr/bin/true", out, "DASLLAMA_SCRATCH_DIR='{scratch}' "), 0,
        "a dry audio run under --arm exits clean:\n{out}")
    let lead = "CENSUS {path_join(scratch, "dasllama_census_")}"
    let at = find(out, lead)
    t |> success(at >= 0, "the census folder sits under the scratch folder:\n{out}")
    return if (at < 0)
    let folder = slice(out, at + length("CENSUS "), find(out, " (DASLLAMA_KERNEL_CENSUS_DIR)", at))
    let verdict = find(out, "START test_kernel_coverage.das")
    let line = verdict >= 0 ? slice(out, verdict, find(out, "\n", verdict)) : ""
    t |> success(find(line, "DASLLAMA_KERNEL_CENSUS_DIR=\"{folder}\"") >= 0, "the verdict child reads it:\n{line}")
    t |> success(!stat(folder).is_valid, "the run removes it: {folder}")
    let image = find(out, "START test_model_image.das")
    let image_line = image >= 0 ? slice(out, image, find(out, "\n", image)) : ""
    t |> success(find(image_line, "DASLLAMA_TEST_ARMS=fxarm\"") >= 0, "the image child takes the asked arm:\n{image_line}")
}
