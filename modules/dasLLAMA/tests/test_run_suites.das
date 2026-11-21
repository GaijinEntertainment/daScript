options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

require dastest/testing_boost public
require run
require _model_categories
require daslib/fio
require daslib/strings_boost
require strings
require math

// The runner's per-PR suite split, model-free: `model-free` holds only files that reach no
// machine-local fixture root, `stocked` holds every file that does, the two never share a file,
// and every listed file exists. Read off the files' own text, so a misfiled test reds here.

//! the calls through which a test reaches a machine-local fixture root (_model_tier.das)
let FIXTURE_ROOTS <- ["models_dir()", "model_available(", "llama2c_dir()", "whisper_dir()", "corpus_dir()", "asr_clip(", "asr_oracle_dir("]

def private file_text(path : string) : string {
    var text = ""
    fopen(path, "rb") $(f) {
        if (f != null) {
            text = fread(f)
        }
    }
    return text
}

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
    let text = file_text(file)
    if (text_reaches_fixture_root(text)) {
        return true
    }
    for (line in split(text, "\n")) {
        continue if (!(line |> starts_with("require _")))
        var inscope name <- split_by_chars(slice(line, length("require ")), " \t/\r")
        if (text_reaches_fixture_root(file_text(path_join(dir_name(file), "{name[0]}.das")))) {
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
        t |> success(!empty(file_text(file)), "model-free file exists and reads: {file}")
        t |> success(!reaches_fixture_root(file), "a model-free file reaches no fixture root - move it to `stocked`: {file}")
        seen |> insert(file, null)
    }
    for (file in suite_files("stocked")) {
        let text = file_text(file)
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
    for (suite in ["decode", "mtp", "prefill", "matrix", "kernels", "gpu-kernels", "image", "image-vulkan", "coverage", "model-free", "stocked"]) {
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
    t |> success(find(file_text("{TESTS_DIR}/run.das"), "[init]") < 0, "run.das carries no [init] - it runs nothing on require")
}

//! the area tables cover the per-PR census exactly: every area file is a model-free or stocked file,
//! every such file has at least one area, and every image-suite arm an area names exists
[test(description = "The runner's area tables cover the model-free plus stocked census exactly, every image arm an area names exists in test_model_image, and every image arm is reachable from an area or listed unclaimed")]
def test_area_tables_match_the_census(t : T?) {
    var census <- [for (f in free_files()); base_name(f)]
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
    let every = join(AREAS, ",")
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
    t |> equal(length(tts.files), 22, "tts plans its twenty-one files and the image file its kitten arm rides")
    t |> equal(tts.image_arms, "kitten,pocket", "tts owns the kitten and pocket image arms")
    t |> equal(tts.files[length(tts.files) - 1], "{TESTS_DIR}/test_model_image.das", "the image file rides last")
    for (i in range(1, length(tts.files) - 1)) {
        t |> success(tts.files[i - 1] < tts.files[i], "the plan is sorted: {tts.files[i - 1]} < {tts.files[i]}")
    }
    let translate <- area_plan(["translate"])
    t |> equal(join(translate.files, ","), "{TESTS_DIR}/test_interpreter.das,{TESTS_DIR}/test_nmt.das,{TESTS_DIR}/test_model_image.das", "translate plans its two files and the image file its bergamot arm rides")
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
    t |> equal(length(area.files), 22, "an area run plans the area, whatever --suite says")
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
            "gemma-4-26B-A4B-6L-UD-IQ3_XXS-local.gguf", "gemma-4-26B-A4B-6L-MXFP4_MOE-local.gguf")) {
        t |> equal(model_category(cut), "", "{cut}, a cut of a category's file, is no category's: every run opens it")
    }
    t |> equal(model_category("gpt-oss-20b-mxfp4.gguf"), "gptoss-20b", "the file a cut is made from stays its category's")
    t |> equal(model_category("gemma-4-26B_q4_0-it.gguf"), "gemma4-moe", "the 26B's Q4_0 file is the routed category's")
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
    t |> success(find_index(files, "{TESTS_DIR}/test_gemma4v.das") >= 0 && find_index(files, "{TESTS_DIR}/test_whisper.das") >= 0, "the files naming an E4B model: {join(files, " ")}")
    t |> success(find_index(files, "{TESTS_DIR}/test_chat.das") < 0, "a file that names none stays out")
    t |> success(empty(category_files("nope")), "a name that is no category's plans nothing")
    t |> equal(length(category_files("gemma4-e4b,")), length(files), "an empty name plans no file of a model in no category")
    t |> success(empty([for (f in files); f; where base_name(f) |> starts_with("_")]), "a helper file that names a model is no test file: {join(files, " ")}")
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
    let doc = file_text("{TESTS_DIR}/CLAUDE.md")
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
        continue if (no_cell || !text_reaches_cuda(file_text(file)))
        reach++
        t |> success(find_index(kept, file) >= 0,
            "a file reaching the CUDA home sits in the CUDA pass - add a token: {file}")
    }
    t |> success(reach > 10, "the census walk found the CUDA files ({reach})")
}

//! the reach plan behind --changed: a test file itself, a fixture the tests requiring it through the fixtures between, an
//! engine module its witness row or the core set beside its other areas whole, a benchmark the bench rows' files; every
//! witness row and the core set name census files, and every row's module exists and reaches llm
[test(description = "`reach_for_path` maps a test to itself, a fixture to its requiring census tests, a leaf module to its witness row, a core module to the core plus every other area, and every witness names census files")]
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
    let qwen35 <- reach_for_path("modules/dasLLAMA/dasllama/dasllama_arch_qwen35.das")
    t |> success(!qwen35.core && empty(qwen35.areas) && find_index(qwen35.files, "test_gpu_resident_hybrid.das") >= 0, "a leaf module reaches its witness row, not the core")
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

[test(description = "`model_names` reads every whole quoted .gguf name out of a file's text once and skips interpolated names, paths and patterns, and `missing_models` lists the names a run's files and the fixtures they require ask that the directory lacks, each with its askers, while a directory holding them lists none")]
def test_missing_models(t : T?) {
    let names <- model_names("let a = \"B-model.gguf\"\nlet b = \"\{stem\}.gguf\"\nlet c = \"dir/C.gguf\"\nlet d = \"*.gguf\"\nrun(\"A-model.gguf\", \"B-model.gguf\", \"note.txt\")\n")
    t |> equal(join(names, ","), "A-model.gguf,B-model.gguf", "whole quoted names once each, in order; an interpolated name, a path and a pattern are not names")
    let dir = path_join(test_temp_dir(), "dasllama_missing_models")
    mkdir_rec(dir)
    let run <- ["{TESTS_DIR}/test_kv_paged.das"]
    let missing <- missing_models(run, dir)
    t |> success(!empty(missing), "an empty directory misses what the file names")
    let smol = find_index_if(missing) $(m) => m.name == "SmolLM2-135M-Instruct-Q8_0.gguf"
    t |> success(smol >= 0, "the file's own model is named: {missing}")
    t |> success(smol >= 0 && find(missing[smol].asked, "test_kv_paged.das") >= 0, "with the file that names it")
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
