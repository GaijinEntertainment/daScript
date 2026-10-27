options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

require dastest/testing_boost public
require run
require daslib/fio
require daslib/strings_boost
require strings

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
//! `require _<fixture>` (the one-model gemma files reach `model_available` through `_gemma_resident` alone)
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

[test]
def test_suite_split_matches_the_files(t : T?) {
    var seen : table<string; void?>
    for (file in suite_files("model-free")) {
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
[test]
def test_areas_for_path(t : T?) {
    let every = join(AREAS, ",")
    t |> equal(join(areas_for_path("modules/dasLLAMA/dasllama/dasllama_audio.das"), ","), "audio", "an audio module reaches audio")
    t |> equal(join(areas_for_path("modules/dasLLAMA/dasllama/dasllama_tower.das"), ","), "audio,vision,tts", "the shared tower reaches both towers and the TTS blocks calling its row kernels")
    t |> equal(join(areas_for_path("modules/dasLLAMA/dasllama/dasllama_vulkan_tower.das"), ","), "audio,vision,tts", "the Vulkan tower reaches both towers and the TTS driver riding its classes")
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
[test]
def test_area_plan(t : T?) {
    let tts <- area_plan(["tts"])
    t |> equal(length(tts.files), 18, "tts plans its seventeen files and the image file its kitten arm rides")
    t |> equal(tts.image_arms, "kitten", "tts owns the kitten image arm")
    t |> equal(tts.files[length(tts.files) - 1], "{TESTS_DIR}/test_model_image.das", "the image file rides last")
    for (i in range(1, length(tts.files) - 1)) {
        t |> success(tts.files[i - 1] < tts.files[i], "the plan is sorted: {tts.files[i - 1]} < {tts.files[i]}")
    }
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
[test]
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

def private run_runner(args : string; var output : string&) : int {
    return unsafe(popen_timeout("{das_exe()} -dasroot {get_das_root()} {TESTS_DIR}/run.das -- {args}", 120.0, $(f) {
        if (f != null) {
            output = fread(f)
        }
    }))
}

//! the runner end to end with a no-op child binary: the AREAS line, the image file's arm env, the
//! REPORT and NEXT lines, and the refusals main prints
[test]
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
    t |> success(find(image_line, "DASLLAMA_TEST_ARMS='whisper,") >= 0 && find(image_line, "gemma3v") >= 0, "the image child gets the union of the areas' arms:\n{image_line}")
    let audio_start = find(out, "START test_audio.das: ")
    let audio_line = slice(out, audio_start, find(out, "\n", audio_start))
    t |> success(find(audio_line, "DASLLAMA_TEST_ARMS=''") >= 0, "every other child gets no arm filter:\n{audio_line}")
    t |> success(find(out, "REPORT areas audio,vision: ") >= 0, "the REPORT line names the scope")
    t |> success(find(out, "NEXT before the PR") >= 0, "the NEXT line names the per-PR suites")
    var llm : string
    t |> equal(run_runner("--area infra --das /usr/bin/true --exclude test_exe_smoke", llm), 0, "an infra dry run with an exclusion exits clean")
    t |> success(find(llm, "EXCLUDED test_exe_smoke.das") >= 0 && find(llm, "1 file(s) excluded") >= 0, "the exclusion is loud and counted:\n{llm}")
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

[test]
def test_shard_filter(t : T?) {
    let files <- [for (k in range(7)); "modules/dasLLAMA/tests/test_{k}.das"]
    let whole <- shard_files(files, "")
    t |> equal(length(whole.kept), 7, "no --shard keeps every file")
    t |> success(empty(whole.why), "and refuses nothing")
    let one <- shard_files(files, "1/1")
    t |> equal(length(one.kept), 7, "1/1 is the whole run")
    var seen : table<string>
    for (k in range(3)) {
        let part <- shard_files(files, "{k + 1}/3")
        t |> success(empty(part.why), "{k + 1}/3 parses")
        t |> success(length(part.kept) == (k == 0 ? 3 : 2), "{k + 1}/3 holds every third file from its own: {length(part.kept)}")
        for (f in part.kept) {
            t |> success(!key_exists(seen, f), "{f} sits in one shard alone")
            seen |> insert(f)
        }
    }
    t |> equal(length(seen), 7, "the three shards cover the run once")
    let second <- shard_files(files, "2/3")
    t |> equal(second.kept[0], files[1], "shard i starts at the i-th file")
    t |> equal(second.kept[1], files[4], "and steps by n")
    for (bad in ["0/2", "3/2", "2/0", "a/b", "1", "1/2/3", "01/2", " 1/2"]) {
        let r <- shard_files(files, bad)
        t |> success(!empty(r.why) && empty(r.kept), "'{bad}' is refused: {r.why}")
    }
    return if (get_platform_name() == "windows")   //! the dry run's no-op child binary is a POSIX one, as test_runner_dry_run's
    var out : string
    t |> equal(run_runner("--area infra --das /usr/bin/true --shard 2/2", out), 0, "an infra dry run under a shard exits clean:\n{out}")
    t |> success(find(out, "SHARD 2/2: ") >= 0, "the SHARD line names the shard and its count:\n{out}")
    t |> success(find(out, "REPORT areas infra shard 2/2:") >= 0, "the REPORT line carries the shard, so it cannot read as the whole run:\n{out}")
    t |> success(find(out, "NEXT the run's other shard(s)") >= 0 && find(out, "--shard 1/2") >= 0, "the NEXT line owes the other shard:\n{out}")
    var refused : string
    t |> equal(run_runner("--area infra --das /usr/bin/true --shard 5/4", refused), 2, "a shard past n is refused")
    t |> success(find(refused, "not i/n") >= 0, "and says so:\n{refused}")
}

//! --only keeps the files whose name contains a token and nothing else, "" the whole list; the dry run names each kept
//! file on an ONLY line and the REPORT line carries the tokens, and a --only that keeps nothing is refused
[test]
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
    t |> success(find(out, "ONLY test_exchange_schema.das (--only exchange,facade)") >= 0 && find(out, "ONLY test_facade_docs.das") >= 0, "each kept file sits on an ONLY line:\n{out}")
    t |> success(find(out, "REPORT areas infra only exchange,facade: 2 file(s)") >= 0, "the REPORT line carries the tokens, so the run cannot read as whole:\n{out}")
    var refused : string
    t |> equal(run_runner("--area infra --das /usr/bin/true --only no-such-file", refused), 2, "a --only that keeps nothing is refused")
    t |> success(find(refused, "keeps no file") >= 0, "and says so:\n{refused}")
}

//! the reach plan behind --changed: a test file itself, a fixture the tests requiring it through the fixtures between, an
//! engine module its witness row or the core set beside its other areas whole, a benchmark the bench rows' files; every
//! witness row and the core set name census files, and every row's module exists and reaches llm
[test]
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
    t |> success(find_index(oracle.files, "test_vulkan_kernels.das") >= 0 && find_index(oracle.files, "test_metal_attn_kernels.das") < 0, "a fixture's reach stops at the census: a model-suite file takes --arm ({join(oracle.files, ", ")})")
    let tier <- reach_for_path("modules/dasLLAMA/tests/_model_tier.das")
    t |> success(length(tier.files) > length(feed.files), "the shared tier fixture reaches more, through the fixtures between ({length(tier.files)})")
    let qwen35 <- reach_for_path("modules/dasLLAMA/dasllama/dasllama_arch_qwen35.das")
    t |> success(!qwen35.core && empty(qwen35.areas) && find_index(qwen35.files, "test_gpu_resident_hybrid.das") >= 0, "a leaf module reaches its witness row, not the core")
    let load <- reach_for_path("modules/dasLLAMA/dasllama/dasllama_load.das")
    t |> success(load.core, "a core module reaches the core")
    t |> success(find_index(load.files, "test_vulkan_kernels.das") >= 0 && find_index(load.files, "test_gpu_resident_llama.das") >= 0, "every model-free llm file and the core witnesses")
    t |> success(find_index(load.files, "test_gpu_resident_moe_formats.das") < 0, "not the whole stocked set")
    t |> equal(join(load.areas, ","), "audio,vision,tts,infra", "and every other area whole")
    let audio <- reach_for_path("modules/dasLLAMA/dasllama/dasllama_audio.das")
    t |> success(empty(audio.files) && join(audio.areas, ",") == "audio" && !audio.core, "an audio module reaches audio whole and no llm file")
    let codec <- reach_for_path("modules/dasLLAMA/dasllama/dasllama_kv_codec.das")
    t |> success(join(codec.areas, ",") == "audio" && find_index(codec.files, "test_kv_codec.das") >= 0 && !codec.core, "a two-area module reaches its other area whole and its llm witnesses")
    let bench <- reach_for_path("modules/dasLLAMA/benchmarks/lcpp_bench.das")
    t |> equal(join(bench.files, ","), join(BENCH_WITNESSES, ","), "a benchmark reaches the bench rows' files")
    let outside <- reach_for_path("src/ast/ast_parse.cpp")
    t |> success(empty(outside.files) && empty(outside.areas) && empty(outside.note), "a path outside the module reaches nothing")
    var listed <- suite_files("model-free")
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
[test]
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
