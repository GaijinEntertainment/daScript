options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

require dastest/testing_boost public
require run
require dastest/kill_switch   // KILL_SWITCH_EXIT_CODE: the exit of a child its test wall ended
require _model_categories
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

def private text_reaches_fixture_root(text : string) : bool {
    for (root in FIXTURE_ROOTS) {
        if (find(text, root) >= 0) {
            return true
        }
    }
    return false
}

//! a test reaches a fixture root through its own text or through a shared fixture module beside it - a same-folder
//! `require _<fixture>` (test_gpu_resident_gemma4_26b_host.das reaches `model_available` through `_gemma_resident` alone)
def private reaches_fixture_root(file : string) : bool {
    let text = fread(file)
    if (text_reaches_fixture_root(text)) {
        return true
    }
    for (line in split(text, "\n")) {
        continue if (!(line |> starts_with("require _")))
        var inscope name <- split_by_chars(slice(line, length("require ")), " \t/\r")
        if (text_reaches_fixture_root(fread(path_join(dir_name(file), "{name[0]}.das")))) {
            return true
        }
    }
    return false
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

//! every test file in the folder sits in some run.das suite; the one exemption is the CPU-prefill
//! tripwire, whose header says why (it asserts the guard the runner disarms for every child)
[test(description = "Every test_, failed_ and uncategorized file in the folder sits in some run.das suite except the declared CPU-prefill tripwire, over more than 50 files walked, and run.das carries no [init]")]
def test_every_file_sits_in_a_suite(t : T?) {
    var listed : table<string; void?>   // nolint:STYLE027 - a nested walk over the suite tables, not one comprehension
    for (suite in ["decode", "mtp", "prefill", "matrix", "kernels", "gpu-kernels", "image", "image-vulkan", "coverage", "model-free", "stocked", "examples"]) {
        for (file in suite_files(suite)) {
            listed |> insert(base_name(file), null)
        }
    }
    let exempt <- ["test_cpu_prefill_tripwire.das"]
    var count = 0
    fio::dir("modules/dasLLAMA/tests") $(name) {
        if ((name |> starts_with("test_") || name |> starts_with("failed_") || name |> starts_with("uncategorized")) && name |> ends_with(".das")) {
            count++
            t |> success(key_exists(listed, name) || find_index(exempt, name) >= 0, "a test file sits in a run.das suite, or is the declared tripwire exemption: {name}")
        }
    }
    t |> success(count > 50, "the folder listing walked the test files ({count})")
    // the runner is required by this file and test_run_summary.das: nothing may fire on require
    t |> success(find(fread("{TESTS_DIR}/run.das"), "[init]") < 0, "run.das carries no [init] - it runs nothing on require")
}

//! the area tables cover the per-PR census exactly: every standing area's file is a model-free or stocked file,
//! every such file has at least one area, an on-demand area's files are the examples suite's and no per-PR file,
//! and every image-suite arm an area names exists
[test(description = "The runner's standing area tables cover the model-free plus stocked census exactly, an on-demand area holds the examples suite's files and none of the census, every image arm an area names exists in test_model_image, and every image arm is reachable from an area or listed unclaimed")]
def test_area_tables_match_the_census(t : T?) {
    var census <- [for (f in free_files()); base_name(f)]
    census |> push_from([for (f in suite_files("stocked")); base_name(f)])
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
    var claimed : array<string>
    for (a in AREAS) {
        for (tok in split(area_image_arms(a), ",")) {
            continue if (empty(tok))
            t |> success(find(image, "arm_on(t, \"{tok}") >= 0, "the image suite has an arm the area names: {a}/{tok}")
            claimed |> push(tok)
        }
    }
    // the reverse: every arm the image suite declares is reachable from some area, or is named here as
    // deliberately unclaimed - `gemma` is an --arm substring of every other gemma* arm (CLAUDE.md says how to reach it)
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
    t |> equal(join(areas_for_path("modules/dasLLAMA/dasllama/dasllama_tts_slab.das"), ","), "audio,vision,tts", "the shared TTS slab writer reaches every tower seat")
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
    t |> equal(join(areas_for_path("modules/dasLLAMA/tests/test_vulkan_tts_pocket_kernels.das"), ","), "tts", "the Vulkan Pocket TTS cells reach the tts area")
    t |> equal(join(areas_for_path("modules/dasLLAMA/tests/test_vulkan_tts_source_kernels.das"), ","), "tts", "the Vulkan TTS source and STFT cells reach the tts area")
    t |> equal(join(areas_for_path("modules/dasLLAMA/tests/_tower_twin.das"), ","), "audio,vision", "the tower twin fixture reaches both towers")
    t |> equal(length(areas_for_path("modules/dasLLAMA/tests/test_metal_decode_parity.das")), 0, "a model-suite file reaches no area - it takes --arm")
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
[test(description = "`area_plan` returns an area's census files sorted and deduplicated with the image file appended once and last when an area owns image arms, and `plan_files` lets an area override --suite")]
def test_area_plan(t : T?) {
    let tts <- area_plan(["tts"])
    t |> equal(length(tts.files), 18, "tts plans its seventeen files and the image file its kitten arm rides")
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
    let cfg = RunArgs(suite = "decode", arm = "arm12")
    let none : array<string>
    let suite <- plan_files(cfg, none)
    t |> equal(length(suite.files), 1, "a suite run plans the suite table")
    t |> equal(suite.image_arms, "arm12", "a suite run passes --arm through")
    let area <- plan_files(cfg, ["tts"])
    t |> equal(length(area.files), 18, "an area run plans the area, whatever --suite says")
}

//! the argument contracts: an area run refuses --arm/--full/--suite and an unknown area; a per-PR
//! suite refuses --arm and --full; a model suite wants exactly one of --arm and --full, and --full is refused
[test(description = "The runner refuses unknown areas and --area beside --suite or --arm, a per-PR suite refuses --arm, a model suite needs --arm and refuses --full, and child timeouts are 3600 for stocked and 1200 for arm runs")]
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
    let mixed = RunArgs(area = "tts", suite = "decode")
    t |> equal(select_areas(mixed).status, 2, "--area beside --suite is refused")
    let armed = RunArgs(area = "tts", arm = "x")
    t |> equal(select_areas(armed).status, 2, "--area beside --arm is refused")
    t |> equal(check_suite_args(RunArgs(suite = "stocked")), 0, "a per-PR suite runs bare")
    t |> equal(check_suite_args(RunArgs(suite = "stocked", arm = "x")), 2, "a per-PR suite refuses --arm")
    t |> equal(check_suite_args(RunArgs(suite = "wasm")), 0, "the wasm suite runs bare")
    t |> equal(check_suite_args(RunArgs(suite = "wasm", arm = "x")), 2, "the wasm suite refuses --arm")
    t |> equal(check_suite_args(RunArgs(suite = "decode")), 2, "a model suite with neither --arm nor --full is refused")
    t |> equal(check_suite_args(RunArgs(suite = "decode", arm = "arm12")), 0, "a model suite with --arm runs")
    t |> equal(check_suite_args(RunArgs(suite = "decode", full = true)), 2, "--full is refused while the Metal build-out is in progress")
    t |> equal(child_timeout(RunArgs(suite = "stocked")), 3600, "the stocked gate's child holds the large tier's tails")
    t |> equal(child_timeout(RunArgs(changed = true)), 3600, "--changed runs the stocked files, so its child holds the same tails")
    t |> equal(child_timeout(RunArgs(suite = "decode", full = true)), 3600, "the full gate's child too")
    t |> equal(child_timeout(RunArgs(suite = "decode", arm = "arm12")), 1200, "an arm run keeps the arm-mode budget")
    t |> equal(child_timeout(RunArgs(area = "tts")), 1200, "... and so does an area run")
}

def private das_exe() : string {
    let args <- get_command_line_arguments()
    return empty(args) ? "" : args[0]
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

//! a batch whose child ends before any verdict reruns its files one child each, each red single child a failure of its
//! own; a batch's timeout is its file count times the single's
[test(description = "A batch child that exits red before any verdict reruns each file alone and counts each red single as a failure, the batch timeout is file count times the single's, and a green child retries nothing")]
def test_runner_retries_a_verdictless_batch(t : T?) {
    return if (get_platform_name() == "windows")   //! the child binary stands in a POSIX one, as test_runner_dry_run's
    var out : string
    let rc = run_runner("--area infra --only test_box --das /usr/bin/false", out)
    t |> success(rc != 0, "a run whose every child exits red exits red:\n{out}")
    t |> success(find(out, "START test_box_ident.das+1: ") >= 0, "the two files ride one batch:\n{out}")
    t |> success(find(out, "RETRY test_box_ident.das: ") >= 0 && find(out, "RETRY test_box_profile.das: ") >= 0, "a child red before any verdict retries every file:\n{out}")
    t |> success(find(out, "START test_box_ident.das: ") >= 0 && find(out, "START test_box_profile.das: ") >= 0, "each retried file runs alone:\n{out}")
    t |> success(find(out, "2 file(s), 2 failed") >= 0, "a red single child is a failure of its own:\n{out}")
    let single = start_timeout(out, "test_box_ident.das")
    t |> success(single > 0, "the single child carries a timeout:\n{out}")
    t |> equal(start_timeout(out, "test_box_ident.das+1"), 2 * single, "the batch's timeout is its file count times the single's")
    var clean : string
    t |> equal(run_runner("--area infra --only test_box --das /usr/bin/true", clean), 0, "a green child with no verdict is no retry:\n{clean}")
    t |> success(find(clean, "RETRY") < 0, "nothing retried:\n{clean}")
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
    t |> success(find(out, "RETRY") < 0, "no file of the batch reruns alone:\n{out}")
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
    t |> success(run_runner("--suite model-free --test-wall 0 --das /usr/bin/false --skip {at}", open_run) != 0, "with no wall the red batch is retried:\n{open_run}")
    t |> success(find(open_run, " --test-wall ") < 0 && find(open_run, " no test wall resumed at child {at}") >= 0,
        "--test-wall 0 hands the children no wall and says so in the REPORT:\n{open_run}")
    let first = slice(tag, 0, find(tag, "+"))
    t |> success(find(open_run, "RETRY {tag}: ") >= 0 || find(open_run, "RETRY {first}: ") >= 0, "the batch's files are retried:\n{open_run}")
    t |> success(find(open_run, "START {first}: ") >= 0 && find(open_run, "SKIP {first}: ") < 0, "the retry starts at its own first file, whatever --skip the run took:\n{open_run}")
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
    fwrite(log, "[E] Failed to compile the together root x.das\nFAIL modules/dasLLAMA/tests/test_box_ident.das (0.0s)\nFAIL modules/dasLLAMA/tests/test_box_profile.das (0.0s)\n")
    run_runner("--area infra --only test_box --test-wall 30 --das {child}", out, "FAKE_CHILD_RC=2 FAKE_CHILD_LOG={log} ")
    t |> success(find(out, "LOST test_box_ident.das+1: the batch ended with exit 2: its root failed, no file ran") >= 0,
        "a walled batch whose root failed is lost by that name:\n{out}")
    fwrite(log, "PASS modules/dasLLAMA/tests/test_box_ident.das (0.1s)\nFAIL modules/dasLLAMA/tests/test_box_profile.das (0.1s)\n")
    run_runner("--area infra --only test_box --test-wall 30 --das {child}", out, "FAKE_CHILD_RC=1 FAKE_CHILD_LOG={log} ")
    t |> success(find(out, "LOST ") < 0 && find(out, "RETRY") < 0 && find(out, "2 file(s), 1 failed (test_box_profile.das)") >= 0,
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
    t |> equal(model_category("Qwen3.8-Flash-Next-UD-IQ4_XS-00001-of-00003.gguf"), "", "the Flash-Next trunk, under the same 3.8 prefix, is no category's")
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
    var seen : table<string>
    for (fam in model_families()) {
        t |> success(!empty(fam.name) && !empty(fam.stems) && !key_exists(seen, fam.name), "the family {fam.name} has one row with stems")
        seen |> insert(fam.name)
    }
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
    let matrix = "{TESTS_DIR}/test_metal_support_matrix.das"
    let coverage = "{TESTS_DIR}/test_kernel_coverage.das"
    let plain <- [for (i in range(12)); "{TESTS_DIR}/test_plain_{i}.das"]
    var mixed <- [for (f in plain); f]
    mixed |> push(matrix)
    mixed |> push(coverage)
    let chunks <- category_chunks(mixed, batch_files(named))
    t |> equal(length(chunks), 4, "a process suite's files ride a child of their own, the others ten a child: {chunks}")
    t |> success(length(chunks[0]) == 1 && chunks[0][0] == matrix && length(chunks[1]) == 1 && chunks[1][0] == coverage, "the matrix file and the coverage file each alone")
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

[test(description = "A run forces its children onto the tune file in effect, whatever the file's age: the caller's manifest where it exists, nothing under --no-tune, for an absent file, or where the caller forced a file already", timeout = 1200)]
def test_forced_tune_file(t : T?) {
    let box = path_join(test_temp_dir(), "forced_box_{get_time_usec(0l)}.tune.json")
    t |> success(fwrite(box, "\{}"), "a box tune file is written")
    t |> equal(forced_tune_file_of(false, "", "", box), box, "with no manifest named the box's own file is forced")
    t |> equal(forced_tune_file_of(false, "", "{box}.other", box), "", "a named manifest that is not there forces nothing, the box's file or no")
    t |> equal(forced_tune_file_of(true, "", "", box), "", "--no-tune forces nothing")
    t |> equal(forced_tune_file_of(false, "theirs.json", "", box), "", "a file the caller forced stands")
    remove(box)
    t |> equal(forced_env("A=1 ", "x.json", false), "DAS_TUNE_FORCE='x.json' A=1 ", "the POSIX prefix names the forced file")
    t |> equal(forced_env("set \"A=1\"&& ", "x.json", true), "set \"A=1\"&& set \"DAS_TUNE_FORCE=x.json\"&& ", "cmd.exe's form sets it")
    t |> equal(forced_env("A=1 ", "", false), "A=1 ", "no forced file, no variable")
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

//! the text by which a test file reaches the CUDA home: the home
//! ask, or a CUDA fixture
let private CUDA_REACH <- [
    "gpu_want_cuda()", "require _cuda_", "require ?cuda _cuda_", "cuda_tower_asked"]

//! the files whose text names the CUDA home and that hold no CUDA cell
let private CUDA_REACH_NO_CELL <- [
    "test_gpu_home.das",   // a unit of the home pick itself, on no device
    "test_run_suites.das",   // this file, which names the reach text
    "test_vulkan_moe_cm2.das" ]   // its CUDA branch only skips

def private text_reaches_cuda(text : string) : bool {
    for (r in CUDA_REACH) {
        return true if (find(text, r) >= 0)
    }
    return false
}

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
        continue if (no_cell || !text_reaches_cuda(fread(file)))
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
    let suite <- reach_for_path("modules/dasLLAMA/tests/test_metal_decode_parity.das")
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
    let full_suite = !skips_cached(cfg)   //! this very run under DASLLAMA_PARITY_FULL=1 skips nothing either
    t |> success(skips_cached_in(cfg, false), "a run reports a file from its pass on record")
    t |> success(!skips_cached_in(cfg, true), "the full suite runs every file")
    cfg.rerun = true
    t |> success(!skips_cached_in(cfg, false) && !skips_cached(cfg), "a --rerun runs every file")
    t |> equal(cached_report(0), "", "a run that ran every file says nothing of files not run")
    t |> success(find(cached_report(3), " 3 of them not run") == 0 && find(cached_report(3), "--rerun") > 0, "the REPORT's count of files not run names --rerun: {cached_report(3)}")
    return if (get_platform_name() == "windows")   //! the dry run's no-op child binary is a POSIX one, as test_runner_dry_run's
    var out : string
    t |> equal(run_runner("--area infra --das /usr/bin/true", out), 0, "an infra dry run exits clean:\n{out}")
    t |> equal(find(out, " --skip-fully-cached --pass-stamp 0x") >= 0, !full_suite, "a child command carries the flag and its stamp, and under the full suite (DASLLAMA_PARITY_FULL=1) none:\n{out}")
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

[test(description = "`model_names` reads every whole quoted .gguf name out of a file's text once and skips interpolated names, paths and patterns, `model_paths` keeps a path under the models folder and drops a climb, an absolute path and a backslash path, and `missing_models` lists the names a run's files and the fixtures they require ask that the directory lacks, each with its askers, while a directory holding them lists none")]
def test_missing_models(t : T?) {
    let names <- model_names("let a = \"B-model.gguf\"\nlet b = \"\{stem\}.gguf\"\nlet c = \"dir/C.gguf\"\nlet d = \"*.gguf\"\nrun(\"A-model.gguf\", \"B-model.gguf\", \"note.txt\")\n")
    t |> equal(join(names, ","), "A-model.gguf,B-model.gguf", "whole quoted names once each, in order; an interpolated name, a path and a pattern are not names")
    let paths <- model_paths("\"../x.gguf\" \"/a/b.gguf\" \"d\\e.gguf\" \"drafters/d/f.gguf\"")
    t |> equal(join(paths, ","), "drafters/d/f.gguf", "`model_paths` keeps a path under the models folder and drops a climb, an absolute path and a backslash path")
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

//! the first `--pass-stamp` value a dry run's output carries, "" where none
def private first_stamp(out : string) : string {
    let key = " --pass-stamp "
    let at = find(out, key)
    return "" if (at < 0)
    let from = at + length(key)
    let stop = find(out, " ", from)
    return slice(out, from, stop < 0 ? length(out) : stop)
}

[test(description = "A dry run's children carry a pass stamp that changes with the GPU home and holds across the scratch folder, a named scratch folder is the children's TMPDIR and is announced, the full suite's children carry no skip flag, only a media run's Linux children keep the heap, the whisper-class tower's image arm is audio's alone, and a media file's name outside the tests folder is no media file")]
def test_run_wiring(t : T?) {
    t |> success(find_index(split(area_image_arms("audio"), ","), "tower") >= 0, "the tower image arm is the audio area's")
    t |> success(find_index(split(area_image_arms("vision"), ","), "tower") < 0, "and not the vision area's")
    t |> success(!media_run(["elsewhere/test_whisper.das"]), "a media file's name outside the tests folder is no media file")
    return if (get_platform_name() == "windows" || !skips_cached(RunArgs()))   //! a POSIX no-op child; the full suite stamps nothing
    let dry = "--area infra --das /usr/bin/true"
    var vulkan, cuda, moved, full, audio : string
    t |> equal(run_runner(dry, vulkan, "DASLLAMA_GPU=1 "), 0, "a dry run on one GPU home exits clean:\n{vulkan}")
    t |> equal(run_runner(dry, cuda, "DASLLAMA_GPU=1 DASLLAMA_CUDA=1 "), 0, "and on another:\n{cuda}")
    let scratch = path_join(test_temp_dir(), "wiring_scratch")
    t |> equal(run_runner(dry, moved, "DASLLAMA_GPU=1 DASLLAMA_SCRATCH_DIR='{scratch}' "), 0, "and with a scratch folder named:\n{moved}")
    t |> success(!empty(first_stamp(vulkan)), "a child carries a pass stamp:\n{vulkan}")
    t |> success(first_stamp(cuda) != first_stamp(vulkan), "another GPU home is another stamp ({first_stamp(vulkan)} / {first_stamp(cuda)})")
    t |> equal(first_stamp(moved), first_stamp(vulkan), "the scratch folder is no part of the stamp")
    t |> success(find(moved, "TMPDIR=\"{scratch}\" ") >= 0, "a child's command line carries the scratch folder as its TMPDIR:\n{moved}")
    t |> success(find(moved, "SCRATCH {scratch}") >= 0 && stat(scratch).is_dir, "the run announces the folder and makes it:\n{moved}")
    t |> success(find(vulkan, "TMPDIR=\"") < 0 && find(vulkan, "SCRATCH ") < 0, "a run that names none hands its children none:\n{vulkan}")
    t |> equal(run_runner(dry, full, "DASLLAMA_PARITY_FULL=1 "), 0, "a dry run of the full suite exits clean:\n{full}")
    t |> success(find(full, "--skip-fully-cached") < 0, "the full suite's children carry no skip flag:\n{full}")
    t |> equal(run_runner("--area audio --das /usr/bin/true", audio), 0, "a dry audio run exits clean:\n{audio}")
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
