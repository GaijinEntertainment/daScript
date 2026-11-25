options gen2
options indenting = 4
options _dasllama_internal = true

// The ONLY sanctioned way to run the dasLLAMA Metal suites (see modules/dasLLAMA/tests/CLAUDE.md).
// Enforces the iteration discipline that keeps suite walls honest: --arm (filtered iteration,
// minutes) scoped to the arms a change can actually affect. --full is REFUSED while the Metal
// build-out is in progress — a whole-zoo pass buys soak time, not coverage; narrow the scope.
// Every run's COMPLETE output lands in a log file whose path is printed — never pipe suite
// output through a grep at capture time.

require daslib/clargs
require daslib/cmp
require daslib/fio
require daslib/strings_boost
require dastest/fs
require strings
require math
require dasllama/dasllama_env
require dasllama/dasllama_gguf   // model_tier_skip_reason, gguf_shard_paths: the model files a run opens
require llvm/daslib/llvm_env   // g_env_tune.tune_force: a tune file the caller forced already
require _child_run   // child_exe_ensure: the test child's exe, built before the run's first child
require _model_categories   // the model categories a run names with --category
require dastest/kill_switch   // KILL_SWITCH_EXIT_CODE: the exit of a child its test wall ended
require ../performance/box_ident.das   // derive_box_name — hardware-keyed, engine-free
require ../../../utils/common/text_lines.das   // split_trimmed: the comma lists

let MODULE_CACHE = ".cache/daslang/module_cache/dastest.dascache"
//! the children's per-test AST records (dastest's --module-cache-dir), one folder per daslang binary: a record is
//! keyed by the binary, so a rebuild opens a new folder and the run removes the folders of the binaries before it
let CHILD_CACHE_ROOT = ".cache/daslang/module_cache/dastest_tests"

[CommandLineArgs]
struct RunArgs {
    @clarg_short = "a"
    @clarg_doc = "Arm filter — comma-separated substrings matched against arm names (e.g. arm12,batchB8); sets DASLLAMA_TEST_ARMS"
    arm : string

    @clarg_short = "f"
    @clarg_doc = "REFUSED while the Metal build-out is in progress — narrow the scope with --arm instead"
    full : bool

    @clarg_short = "s"
    @clarg_doc = "Suite: decode | mtp | prefill | matrix | kernels | gpu-kernels | image | image-vulkan | coverage | model-free | stocked | wasm | all (default all; all excludes image-vulkan, gpu-kernels, model-free, stocked and wasm). gpu-kernels is the Vulkan and CUDA device kernel files: no model, and no test wall - a device's first compile of a kernel is the driver's time, which no test wall admits on a box that never ran it; the stocked suite runs them first"
    suite : string = "all"

    @clarg_short = "x"
    @clarg_doc = "Exclude — comma-separated substrings; a suite file whose name contains one is dropped, each named on an EXCLUDED line (e.g. --suite stocked --exclude test_gpu_resident_gemma4_e2b)"
    exclude : string

    @clarg_doc = "Only — comma-separated substrings; a file of the run whose name contains none is dropped, each kept file named on an ONLY line and the REPORT line carrying the tokens, so a narrowed run cannot read as whole (e.g. --changed --only cuda,regions under DASLLAMA_CUDA=1: the CUDA home's files of the run)"
    only : string

    @clarg_doc = "Area — comma-separated: audio | vision | tts | translate | llm | infra | examples; examples is the browser examples' smoke tests, each spawning its example under a window, and runs only when named here - its files sit in no per-PR suite and no engine change reaches them; a standing area runs every model-free and stocked file of the area plus the image suite's arms of the area (no --suite, no --arm)"
    area : string

    @clarg_doc = "Run what the changed files reach: git diff against --base plus untracked files under modules/dasLLAMA/, each path mapped to its areas (printed), then the areas' tests"
    changed : bool

    @clarg_doc = "Diff base for --changed (default origin/master)"
    base : string = "origin/master"

    @clarg_doc = "Category - comma-separated model categories (../TEST_CATEGORIES.md): runs the test files that name a category's model files, each cell on another model skipped. No other run opens a category's files. Takes --only and --exclude; no --suite, --area, --changed, --arm or --family"
    category : string

    @clarg_doc = "Family filter — comma-separated family tokens (the full list lives in tests/CLAUDE.md, EXACT match); only blocks of these model families run (model-free blocks always run); sets DASLLAMA_TEST_FAMILY"
    family : string

    @clarg_short = "n"
    @clarg_doc = "Repeat count — explicit only (default 1, never best-of-N)"
    nreps : int = 1

    @clarg_short = "b"
    @clarg_doc = "Files per child (default the whole suite in one child on the model-free suite, 10 on any other run): past 1 the child compiles its files as ONE program (dastest --together), so the engine's parse and JIT are paid once a batch and each file still reports as its own; a batch that dies before every file's verdict reruns the files without one, one child each - under a test wall it stops the run instead. The image suite's child always runs alone; 1 = one child a file"
    batch : int = 0
    @clarg_doc = "Run every file. Without it a file whose program loaded whole from its compile record and whose last run on this box - over the same model files and the same environment - passed is reported from that pass (dastest --skip-fully-cached). The run that signs an arc off takes it; the full suite (DASLLAMA_PARITY_FULL=1) runs every file whatever is given"
    rerun : bool
    @clarg_name = "no-tune"
    @clarg_doc = "Short-circuit the [tune] framework in every child (DAS_TUNE_POLICY=reference): the reference bodies run — the portable tier's arm"
    no_tune : bool

    @clarg_name = "reap-images"
    @clarg_doc = "After every child, delete the prepared images (.dlim) and save temp files (.tmp) under the models directory that were not there when the run started - for a box whose run alone writes that directory (a fleet pod's own folder of links), so a suite's mints never fill its disk"
    reap_images : bool
    @clarg_name = "test-wall"
    @clarg_doc = "One [test] function's wall in seconds for this run, in place of the suite's own (30 in the model-free suite, none elsewhere); 0 runs with no wall. The REPORT line carries a wall given here, so a relaxed run cannot read as a plain one"
    test_wall : int = -1
    @clarg_doc = "Start no child before the run's Nth (counted from 0): the resume of a run a wall stopped, on the N its WALL line names. The REPORT line carries it, so a resumed run cannot read as a whole one"
    skip : int = 0
    @clarg_name = "stop-file"
    @clarg_doc = "A marker shared by the boxes of one run (the fleet's two homes, a pod each): the run that sees a test past the wall writes it, and a run that finds it starts no further child, so one box at the wall ends every box's run"
    stop_file : string
    @clarg_name = "poison-kv"
    @clarg_doc = "Start every child's K/V slabs as NaN (DASLLAMA_KV_INIT=poison): a read of a row no store wrote reaches the logits as NaN and reds its cell - the run a change to a K/V store or reader cites. Every session then touches its whole slab, so the run's memory is not the product's"
    poison_kv : bool
    @clarg_name = "list-models"
    @clarg_doc = "Start no child: print the model files this run opens, a `# child <n>: <files>` line ahead of each child's in start order, then one path a line under the models directory (subfolders and split shards and NextN heads included), a file an earlier child named not repeated - the files a fleet pod copies onto its own disk ahead of the run. The list is the GGUF names the runner can read in the files' text: a name built at run time is not on it, and is read where the models directory has it"
    list_models : bool
    @clarg_name = "await-models"
    @clarg_doc = "The copier's state folder (`done` when it ended well, `failed` naming the file it failed on): before each child starts, wait until every model file the child opens is a regular file under the models directory, not a link - or the copier is done (a file still a link is one the list could not name and resolves where it links) - and stop the run red when the copier failed or no file of the child landed in 30 minutes. WAIT lines name each wait, and the REPORT line its total"
    await_models : string

    @clarg_doc = "daslang binary (default: the binary running this script — a hardcoded ./bin/daslang picked up a STALE top-level exe on the MSVC layout)"
    das : string = ""

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

//! the suite tables. Non-private: test_run_suites.das reads them against the files' own text
def suite_files(name : string) : array<string> {   // nolint:STYLE038 - a flat suite table, one file per line
    if (name == "decode") {
        return <- [ "modules/dasLLAMA/tests/test_metal_decode_parity.das" ]
    } elif (name == "mtp") {
        return <- [ "modules/dasLLAMA/tests/test_metal_mtp_parity.das",
                    "modules/dasLLAMA/tests/test_mtp_gemma_drafter.das",
                    "modules/dasLLAMA/tests/test_dflash_drafter.das" ]
    } elif (name == "prefill") {
        return <- [ "modules/dasLLAMA/tests/test_metal_prefill_parity.das" ]
    } elif (name == "matrix") {
        return <- [ "modules/dasLLAMA/tests/test_metal_support_matrix.das" ]
    } elif (name == "kernels") {
        return <- [ "modules/dasLLAMA/tests/test_metal_prefill_kernels.das",
                    "modules/dasLLAMA/tests/test_metal_decode_kernels.das",
                    "modules/dasLLAMA/tests/test_metal_gemv_kernels.das",
                    "modules/dasLLAMA/tests/test_metal_misc_kernels.das",
                    "modules/dasLLAMA/tests/test_metal_tts_kernels.das",
                    "modules/dasLLAMA/tests/test_metal_gemm_kernels.das" ]
    } elif (name == "gpu-kernels") {
        return <- [ "modules/dasLLAMA/tests/test_vulkan_kernels.das",
                    "modules/dasLLAMA/tests/test_vulkan_kernels_attn.das",
                    "modules/dasLLAMA/tests/test_vulkan_kernels_gemm.das",
                    "modules/dasLLAMA/tests/test_vulkan_moe_cm2.das",
                    "modules/dasLLAMA/tests/test_cuda_kernels.das" ]
    } elif (name == "image") {
        return <- [ "modules/dasLLAMA/tests/test_model_image.das" ]
    } elif (name == "image-vulkan") {
        return <- [ "modules/dasLLAMA/tests/test_model_image_vulkan.das" ]
    } elif (name == "coverage") {
        return <- [ "modules/dasLLAMA/tests/test_kernel_coverage.das" ]
    } elif (name == "model-free") {
        return <- [
                    "modules/dasLLAMA/tests/failed_dasllama_lint_require.das",
                    "modules/dasLLAMA/tests/failed_dasllama_lint_sidedoor.das",
                    "modules/dasLLAMA/tests/test_arch_registry.das",
                    "modules/dasLLAMA/tests/test_asr_verbs.das",
                    "modules/dasLLAMA/tests/test_bench_records_schema.das",
                    "modules/dasLLAMA/tests/test_box_ident.das",
                    "modules/dasLLAMA/tests/test_box_profile.das",
                    "modules/dasLLAMA/tests/test_compares.das",
                    "modules/dasLLAMA/tests/test_mtp_snapshot.das",
                    "modules/dasLLAMA/tests/test_mtp_sampled_walk.das",
                    "modules/dasLLAMA/tests/test_mtp_accept_walks.das",
                    "modules/dasLLAMA/tests/test_dasllama_lint_contracts.das",
                    "modules/dasLLAMA/tests/test_dispatch_shaping.das",
                    "modules/dasLLAMA/tests/test_driver_knob_getters.das",
                    "modules/dasLLAMA/tests/test_env_registry.das",
                    "modules/dasLLAMA/tests/test_exchange_schema.das",
                    "modules/dasLLAMA/tests/test_f32_batch_tile_perms.das",
                    "modules/dasLLAMA/tests/test_facade_docs.das",
                    "modules/dasLLAMA/tests/test_fat_hook_umbrella.das",
                    "modules/dasLLAMA/tests/test_fat_start.das",
                    "modules/dasLLAMA/tests/test_gen_records_args.das",
                    "modules/dasLLAMA/tests/test_gguf_quant.das",
                    "modules/dasLLAMA/tests/test_gguf_shards.das",
                    "modules/dasLLAMA/tests/test_gpu_serving_declines.das",
                    "modules/dasLLAMA/tests/test_gpu_home.das",
                    "modules/dasLLAMA/tests/test_gpu_tier.das",
                    "modules/dasLLAMA/tests/test_groupn.das",
                    "modules/dasLLAMA/tests/test_kernel_backend.das",
                    "modules/dasLLAMA/tests/test_kqformat.das",
                    "modules/dasLLAMA/tests/test_math_activations.das",
                    "modules/dasLLAMA/tests/test_math_elementwise.das",
                    "modules/dasLLAMA/tests/test_matmul_batch.das",
                    "modules/dasLLAMA/tests/test_model_forms.das",
                    "modules/dasLLAMA/tests/test_model_specs.das",
                    "modules/dasLLAMA/tests/test_mxfp4.das",
                    "modules/dasLLAMA/tests/test_par_indexed.das",
                    "modules/dasLLAMA/tests/test_jobque_tripwire.das",
                    "modules/dasLLAMA/tests/test_plane.das",
                    "modules/dasLLAMA/tests/test_ple_check.das",
                    "modules/dasLLAMA/tests/test_pod_fleet.das",
                    "modules/dasLLAMA/tests/test_prefill_cpu_kernels.das",
                    "modules/dasLLAMA/tests/test_q8q8_family.das",
                    "modules/dasLLAMA/tests/test_quant.das",
                    "modules/dasLLAMA/tests/test_qwen4exp.das",
                    "modules/dasLLAMA/tests/test_repack.das",
                    "modules/dasLLAMA/tests/test_rnnt.das",
                    "modules/dasLLAMA/tests/test_rope.das",
                    "modules/dasLLAMA/tests/test_rope_apply.das",
                    "modules/dasLLAMA/tests/test_run_suites.das",
                    "modules/dasLLAMA/tests/test_run_summary.das",
                    "modules/dasLLAMA/tests/test_sampling.das",
                    "modules/dasLLAMA/tests/test_site_records.das",
                    "modules/dasLLAMA/tests/test_sizing_helpers.das",
                    "modules/dasLLAMA/tests/test_think_split.das",
                    "modules/dasLLAMA/tests/test_tok_seed.das",
                    "modules/dasLLAMA/tests/test_tts_textnorm.das",
                    "modules/dasLLAMA/tests/test_tts_textnorm_ru.das",
                    "modules/dasLLAMA/tests/test_tts_g2p_ru_pack.das",
                    "modules/dasLLAMA/tests/test_tts_blocks.das",
                    "modules/dasLLAMA/tests/test_tts_voice_prompts.das",
                    "modules/dasLLAMA/tests/test_tool_formats.das",
                    "modules/dasLLAMA/tests/test_tower_asr_kernels.das",
                    "modules/dasLLAMA/tests/test_tower_helpers.das",
                    "modules/dasLLAMA/tests/test_served_bench.das",
                    "modules/dasLLAMA/tests/test_vision.das",
                    "modules/dasLLAMA/tests/test_vulkan_dec_tail.das",
                    "modules/dasLLAMA/tests/test_vulkan_kv_codec_kernels.das",
                    "modules/dasLLAMA/tests/test_vulkan_tower_kernels.das",
                    "modules/dasLLAMA/tests/test_vulkan_tts_conv_kernels.das",
                    "modules/dasLLAMA/tests/test_vulkan_tts_kernels.das",
                    "modules/dasLLAMA/tests/test_vulkan_tts_pocket_kernels.das",
                    "modules/dasLLAMA/tests/test_vulkan_tts_source_kernels.das",
                    "modules/dasLLAMA/tests/test_vulkan_host_twin.das",
                    "modules/dasLLAMA/tests/test_vulkan_tier.das",
                    "modules/dasLLAMA/tests/test_vulkan_shared_device.das",
                    "modules/dasLLAMA/tests/test_cuda_tower_front.das",
                    "modules/dasLLAMA/tests/test_cuda_tower_blocks.das",
                    "modules/dasLLAMA/tests/test_cuda_tower_asr.das",
                    "modules/dasLLAMA/tests/test_cuda_tower_whisper_chain.das",
                    "modules/dasLLAMA/tests/test_cuda_tts_kernels.das",
                    "modules/dasLLAMA/tests/uncategorized__dasllama_internal_persistent_heap_stack_524288.das",
                    "modules/dasLLAMA/tests/uncategorized_persistent_heap_stack_524288.das",
                    "modules/dasLLAMA/tests/uncategorized_no_aot_persistent_heap.das" ]
    } elif (name == "examples") {   //! the files of the on-demand `examples` area, which `--area examples` runs: each spawns a browser example under a window
        return <- [
                    "modules/dasLLAMA/tests/test_interpreter.das",
                    "modules/dasLLAMA/tests/test_parrot.das",
                    "modules/dasLLAMA/tests/test_storyteller_restart.das",
                    "modules/dasLLAMA/tests/test_storywish.das" ]
    } elif (name == "stocked") {
        return <- [
                    "modules/dasLLAMA/tests/test_attn_span.das",
                    "modules/dasLLAMA/tests/test_audio.das",
                    "modules/dasLLAMA/tests/test_audio_embedder.das",
                    "modules/dasLLAMA/tests/test_batch_decode.das",
                    "modules/dasLLAMA/tests/test_chat.das",
                    "modules/dasLLAMA/tests/test_deltanet.das",
                    "modules/dasLLAMA/tests/test_embeddings.das",
                    "modules/dasLLAMA/tests/test_facade.das",
                    "modules/dasLLAMA/tests/test_forward.das",
                    "modules/dasLLAMA/tests/test_fused_decode.das",
                    "modules/dasLLAMA/tests/test_gemma3v.das",
                    "modules/dasLLAMA/tests/test_gemma4uv.das",
                    "modules/dasLLAMA/tests/test_gemma4v.das",
                    "modules/dasLLAMA/tests/test_gigaam.das",
                    "modules/dasLLAMA/tests/test_mlx_snapshot.das",
                    "modules/dasLLAMA/tests/test_mlx_qwen4exp.das",
                    "modules/dasLLAMA/tests/test_nmt.das",
                    "modules/dasLLAMA/tests/test_gpu_model_swap.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_gemma2_3.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_gemma4_dense.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_gemma4_moe.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_gemma4_26b_host.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_gemma4_e2b.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_gptoss.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_hc.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_hc_micro.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_hybrid.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_llama.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_qwen_moe.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_moe_formats.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_format_census.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_qwen2.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_regions_hybrid_formats.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_qwen3.das",
                    "modules/dasLLAMA/tests/test_kquant.das",
                    "modules/dasLLAMA/tests/test_kv_codec.das",
                    "modules/dasLLAMA/tests/test_kv_paged.das",
                    "modules/dasLLAMA/tests/test_kv_prefix.das",
                    "modules/dasLLAMA/tests/test_metal_format_census.das",
                    "modules/dasLLAMA/tests/test_mtp_gemma_drafter.das",
                    "modules/dasLLAMA/tests/test_dflash_drafter.das",
                    "modules/dasLLAMA/tests/test_parity.das",
                    "modules/dasLLAMA/tests/test_parity_pregate.das",
                    "modules/dasLLAMA/tests/test_prefill.das",
                    "modules/dasLLAMA/tests/test_qwen3v.das",
                    "modules/dasLLAMA/tests/test_qwen25v.das",
                    "modules/dasLLAMA/tests/test_scheduler.das",
                    "modules/dasLLAMA/tests/test_tokenizer.das",
                    "modules/dasLLAMA/tests/test_tts_postag.das",
                    "modules/dasLLAMA/tests/test_tts_g2p.das",
                    "modules/dasLLAMA/tests/test_tts_g2p_ru.das",
                    "modules/dasLLAMA/tests/test_tts_kitten.das",
                    "modules/dasLLAMA/tests/test_tts_kokoro.das",
                    "modules/dasLLAMA/tests/test_tts_pocket.das",
                    "modules/dasLLAMA/tests/test_tts_facade.das",
                    "modules/dasLLAMA/tests/test_vad.das",
                    "modules/dasLLAMA/tests/test_vision_chat.das",
                    "modules/dasLLAMA/tests/test_vision_embedder.das",
                    "modules/dasLLAMA/tests/test_vulkan_mint.das",
                    "modules/dasLLAMA/tests/test_cuda_mint.das",
                    "modules/dasLLAMA/tests/test_whisper.das" ]
    } elif (name == "wasm") {
        //! model-free kernel files a wasm64 release runs under node: the cells of the kernels with a wasm seat
        return <- [ "modules/dasLLAMA/tests/test_q8q8_family.das",
                    "modules/dasLLAMA/tests/test_math_activations.das",
                    "modules/dasLLAMA/tests/test_math_elementwise.das",
                    "modules/dasLLAMA/tests/test_matmul_batch.das",
                    "modules/dasLLAMA/tests/test_groupn.das",
                    "modules/dasLLAMA/tests/test_quant.das",
                    "modules/dasLLAMA/tests/test_kqformat.das",
                    "modules/dasLLAMA/tests/test_mxfp4.das",
                    "modules/dasLLAMA/tests/test_repack.das",
                    "modules/dasLLAMA/tests/test_rope.das",
                    "modules/dasLLAMA/tests/test_rope_apply.das",
                    "modules/dasLLAMA/tests/test_prefill_cpu_kernels.das",
                    "modules/dasLLAMA/tests/test_f32_batch_tile_perms.das",
                    "modules/dasLLAMA/tests/test_par_indexed.das",
                    "modules/dasLLAMA/tests/test_plane.das",
                    "modules/dasLLAMA/tests/test_tower_asr_kernels.das",
                    "modules/dasLLAMA/tests/test_tower_helpers.das",
                    "modules/dasLLAMA/tests/test_gguf_quant.das",
                    "modules/dasLLAMA/tests/test_rnnt.das",
                    "modules/dasLLAMA/tests/test_tts_blocks.das" ]
    } elif (name == "all") {
        return <- [ "modules/dasLLAMA/tests/test_metal_decode_parity.das",
                    "modules/dasLLAMA/tests/test_metal_mtp_parity.das",
                    "modules/dasLLAMA/tests/test_metal_prefill_parity.das",
                    "modules/dasLLAMA/tests/test_metal_support_matrix.das",
                    "modules/dasLLAMA/tests/test_metal_prefill_kernels.das",
                    "modules/dasLLAMA/tests/test_metal_decode_kernels.das",
                    "modules/dasLLAMA/tests/test_metal_gemv_kernels.das",
                    "modules/dasLLAMA/tests/test_metal_misc_kernels.das",
                    "modules/dasLLAMA/tests/test_metal_tts_kernels.das",
                    "modules/dasLLAMA/tests/test_metal_gemm_kernels.das",
                    "modules/dasLLAMA/tests/test_model_image.das" ]
    }
    var none : array<string>
    return <- none
}

let AREAS <- ["audio", "vision", "tts", "translate", "llm", "infra", "examples"]
//! the areas a run takes only when its --area names them: no engine change reaches one, and no per-PR suite holds its files
let ON_DEMAND_AREAS <- ["examples"]

//! every area a change reaches when nothing narrows it: all but the on-demand ones
def standing_areas() : array<string> => <- [for (a in AREAS); a; where find_index(ON_DEMAND_AREAS, a) < 0]
let TESTS_DIR = "modules/dasLLAMA/tests"

//! the suite-less test files by AREA - what someone working on audio, vision, TTS, the LLM engine or the
//! tooling runs after an edit. A file serving two areas sits in both (a shared tower helper's test).
//! Non-private: test_run_suites.das holds the union to the model-free + stocked census
def area_tests(area : string) : array<string> {
    if (area == "audio") {
        return <- [ "test_asr_verbs.das", "test_audio.das", "test_audio_embedder.das", "test_cuda_tower_asr.das", "test_cuda_tower_blocks.das",
                    "test_cuda_tower_front.das", "test_cuda_tower_whisper_chain.das", "test_dasllama_lint_contracts.das", "test_gigaam.das",
                    "test_rnnt.das", "test_tower_asr_kernels.das", "test_tower_helpers.das", "test_vad.das",
                    "test_vulkan_tower_kernels.das", "test_whisper.das" ]
    } elif (area == "vision") {
        return <- [ "test_attn_span.das", "test_cuda_tower_blocks.das", "test_gemma3v.das", "test_gemma4uv.das", "test_gemma4v.das", "test_qwen25v.das",
                    "test_qwen3v.das", "test_tower_helpers.das", "test_vision.das", "test_vision_chat.das",
                    "test_vision_embedder.das", "test_vulkan_tower_kernels.das" ]
    } elif (area == "tts") {
        return <- [ "test_cuda_tts_kernels.das", "test_f32_batch_tile_perms.das", "test_tts_blocks.das", "test_tts_facade.das",
                    "test_tts_g2p.das", "test_tts_g2p_ru.das", "test_tts_g2p_ru_pack.das", "test_tts_kitten.das", "test_tts_kokoro.das", "test_tts_pocket.das",
                    "test_tts_postag.das", "test_tts_textnorm.das", "test_tts_textnorm_ru.das", "test_tts_voice_prompts.das", "test_vulkan_tts_conv_kernels.das",
                    "test_vulkan_tts_kernels.das", "test_vulkan_tts_pocket_kernels.das", "test_vulkan_tts_source_kernels.das" ]
    } elif (area == "translate") {
        return <- [ "test_nmt.das" ]
    } elif (area == "examples") {
        return <- [ "test_interpreter.das", "test_parrot.das", "test_storyteller_restart.das", "test_storywish.das" ]
    } elif (area == "infra") {
        return <- [ "failed_dasllama_lint_require.das", "failed_dasllama_lint_sidedoor.das", "test_bench_records_schema.das",
                    "test_box_ident.das", "test_box_profile.das", "test_compares.das", "test_dasllama_lint_contracts.das",
                    "test_env_registry.das",
                    "test_exchange_schema.das", "test_facade.das", "test_facade_docs.das", "test_fat_hook_umbrella.das", "test_fat_start.das", "test_gen_records_args.das",
                    "test_model_forms.das", "test_model_specs.das", "test_pod_fleet.das", "test_run_suites.das", "test_run_summary.das", "test_served_bench.das",
                    "test_site_records.das", "test_sizing_helpers.das", "test_tok_seed.das",
                    "uncategorized__dasllama_internal_persistent_heap_stack_524288.das", "uncategorized_persistent_heap_stack_524288.das" ]
    } elif (area == "llm") {
        return <- [ "test_arch_registry.das", "test_batch_decode.das", "test_bench_records_schema.das",
                    "test_chat.das", "test_deltanet.das", "test_dispatch_shaping.das", "test_driver_knob_getters.das", "test_facade.das",
                    "test_forward.das", "test_fused_decode.das", "test_gguf_quant.das", "test_gguf_shards.das", "test_mlx_snapshot.das", "test_mlx_qwen4exp.das",
                    "test_gpu_home.das", "test_gpu_model_swap.das", "test_gpu_resident_gemma2_3.das", "test_gpu_resident_gemma4_dense.das", "test_gpu_resident_gemma4_moe.das", "test_gpu_resident_gemma4_26b_host.das", "test_gpu_resident_gemma4_e2b.das", "test_gpu_resident_gptoss.das", "test_gpu_resident_hc.das", "test_gpu_resident_hc_micro.das", "test_gpu_resident_hybrid.das", "test_gpu_resident_llama.das", "test_gpu_resident_qwen_moe.das", "test_gpu_resident_moe_formats.das", "test_gpu_resident_format_census.das", "test_gpu_resident_qwen2.das", "test_gpu_resident_regions_hybrid_formats.das", "test_gpu_resident_qwen3.das", "test_gpu_serving_declines.das", "test_gpu_tier.das", "test_groupn.das",
                    "test_embeddings.das", "test_jobque_tripwire.das", "test_kernel_backend.das", "test_kqformat.das", "test_kquant.das",
                    "test_kv_codec.das", "test_kv_paged.das", "test_kv_prefix.das", "test_metal_format_census.das", "test_math_activations.das",
                    "test_math_elementwise.das", "test_matmul_batch.das", "test_f32_batch_tile_perms.das",
                    "test_mtp_gemma_drafter.das", "test_dflash_drafter.das", "test_mtp_sampled_walk.das", "test_mtp_snapshot.das", "test_mtp_accept_walks.das", "test_model_forms.das", "test_mxfp4.das",
                    "test_par_indexed.das", "test_parity.das", "test_parity_pregate.das",
                    "test_plane.das", "test_ple_check.das", "test_prefill.das", "test_prefill_cpu_kernels.das",
                    "test_q8q8_family.das", "test_quant.das", "test_qwen4exp.das",
                    "test_repack.das", "test_rope.das", "test_rope_apply.das",
                    "test_sampling.das", "test_cuda_kernels.das",
                    "test_scheduler.das", "test_think_split.das", "test_tokenizer.das",
                    "test_tool_formats.das", "test_tts_textnorm.das", "test_vulkan_dec_tail.das", "test_vulkan_host_twin.das", "test_vulkan_kernels.das", "test_vulkan_kernels_attn.das", "test_vulkan_kernels_gemm.das", "test_vulkan_kv_codec_kernels.das",
                    "test_vulkan_mint.das", "test_cuda_mint.das", "test_vulkan_moe_cm2.das", "test_vulkan_tier.das", "test_vulkan_shared_device.das",
                    "uncategorized__dasllama_internal_persistent_heap_stack_524288.das", "uncategorized_persistent_heap_stack_524288.das",
                    "uncategorized_no_aot_persistent_heap.das" ]
    }
    var none : array<string>
    return <- none
}

//! the areas whose file lists name the test file `name`, a base name
def test_file_areas(name : string) : array<string> => [for (a in AREAS); a; where find_index(area_tests(a), name) >= 0]

//! the image suite's arms an area owns (substring tokens, `arm_on` semantics); "" = the area has none
def area_image_arms(area : string) : string {
    if (area == "audio") {
        return "whisper,voxtral,parakeet,gigaam,qwen3a,canary,gemma4a,tower"
    } elif (area == "vision") {
        return "gemma4uv,gemma4v,gemma3v,gemma4e"
    } elif (area == "llm") {
        return "mechanics,smol,untied,metal,mx4,b32,mlx"
    } elif (area == "tts") {
        return "kitten,pocket"
    } elif (area == "translate") {
        return "bergamot"
    }
    return ""
}

//! engine module stem -> the areas a change to it reaches. A module not in the table is CORE and
//! reaches every area (the safe default for a new module until it gets a row)
let MODULE_AREAS <- {
    "dasllama_tower" => "audio,vision,tts,translate",   // the TTS blocks, Pocket and StyleTTS2 call its row kernels; the Bergamot chain its GEMMs
    "dasllama_tower_image" => "audio,vision,tts",   // the audio, vision and speech families mint and pick their GPU flavors through it
    "dasllama_metal_tower" => "audio,vision,tts",
    "dasllama_vulkan_tower" => "audio,vision,tts,translate",   // the Vulkan TTS and translation drivers ride the tower's classes
    "dasllama_cuda_tower" => "audio,vision,tts",   // the GPU knobs fixture the audio and TTS cells require reads its knob
    "dasllama_cuda_tower_seat" => "audio,vision,tts",   // every CUDA tower seat's plumbing: the knob, the counters, the resident
    "dasllama_cuda_tower_audio" => "audio",   // the audio seats under the CUDA ask, a family's row the home it serves on
    "dasllama_tower_flavor" => "audio,vision",   // the families' flavor records and registrants
    "dasllama_gpu_rail" => "audio,vision,tts",   // the tower rail's roles and the rail modules: every chain body both discrete homes serve dispatches through them
    "dasllama_vulkan_rail" => "audio,vision,tts", "dasllama_cuda_rail" => "audio,vision,tts", "dasllama_tower_rail" => "audio,vision,tts",
    "dasllama_tower_chains" => "audio,vision,tts",
    "dasllama_tower_chains_g4a" => "audio", "dasllama_cuda_tower_g4a" => "audio",   // the spectral fronts', gemma4a's, qwen3a's front and the VAD's chains, and their CUDA seats
    "dasllama_tower_chains_whisper" => "audio", "dasllama_cuda_tower_whisper" => "audio",   // the whisper class's stem, blocks and chat tail, and their CUDA seats
    "dasllama_asr_dec_chains" => "audio", "dasllama_cuda_asr_dec" => "audio",   // the whisper decoder's chains and its CUDA seat
    "dasllama_tower_chains_fc" => "audio", "dasllama_cuda_tower_fc" => "audio", "dasllama_cuda_asr_dec_fc" => "audio",   // the FastConformer trio's chains, fronts and decoders, and their CUDA seats
    "dasllama_cuda_classes_front" => "audio", "dasllama_cuda_classes_blocks" => "audio,vision", "dasllama_cuda_classes_asr" => "audio",
    "dasllama_tts_slab" => "tts",
    "dasllama_vulkan_tts" => "tts", "dasllama_tower_tts" => "tts", "dasllama_cuda_tts" => "tts", "dasllama_cuda_classes_tts" => "tts",   // the TTS driver, each home's seats and stamps
    "dasllama_metal_tts" => "tts", "dasllama_metal_sites" => "tts", "dasllama_metal_classes_tts" => "tts", "dasllama_metal_rail" => "tts", "dasllama_metal_dispatch" => "tts",
    "dasllama_gpu_kernels_common" => "audio,vision,tts,translate,llm",   // the kernels both GPU homes stamp: the TTS chains', the depthwise rows and rope the audio and vision towers stamp, the Bergamot step's
    "dasllama_asr" => "audio", "dasllama_asr_types" => "audio", "dasllama_audio" => "audio",
    "dasllama_audio_embedder" => "audio", "dasllama_audio_io" => "audio", "dasllama_canary" => "audio",
    "dasllama_gemma4a" => "audio", "dasllama_metal_asr_dec" => "audio", "dasllama_vulkan_asr_dec" => "audio", "dasllama_parakeet" => "audio",
    "dasllama_nmt" => "translate", "dasllama_metal_nmt" => "translate", "dasllama_vulkan_nmt" => "translate", "dasllama_translate" => "translate",
    "dasllama_gigaam" => "audio", "dasllama_rnnt" => "audio", "dasllama_qwen3a" => "audio", "dasllama_vad" => "audio", "dasllama_whisper" => "audio",
    "dasllama_gemma3v" => "vision", "dasllama_gemma4uv" => "vision", "dasllama_gemma4v" => "vision",
    "dasllama_qwen25v" => "vision", "dasllama_qwen3v" => "vision", "dasllama_vision" => "vision",
    "dasllama_vision_embedder" => "vision", "dasllama_vision_io" => "vision",
    "dasllama_g2p" => "tts", "dasllama_g2p_pack" => "tts", "dasllama_g2p_ru" => "tts", "dasllama_g2p_ru_stress" => "tts", "dasllama_wordpiece" => "tts", "dasllama_kitten" => "tts", "dasllama_kokoro" => "tts", "dasllama_pocket" => "tts", "dasllama_postag" => "tts",
    "dasllama_styletts2" => "tts", "dasllama_textnorm" => "tts", "dasllama_textnorm_ru" => "tts", "dasllama_textnorm_ru_nouns" => "tts", "dasllama_textscan" => "tts", "dasllama_tts" => "tts",
    "dasllama_tts_blocks" => "tts", "dasllama_tts_types" => "tts",
    "dasllama_exchange" => "infra", "dasllama_exchange_schema" => "infra", "dasllama_lint" => "infra",
    "dasllama_version" => "infra", "dasllama_fat_start" => "infra",
    "dasllama_chat" => "llm,vision,audio", "dasllama_tools" => "llm,vision,audio",
    // an arch a media file's chat decoder loads through reaches that file's area: the vision and audio blocks load
    // gemma-3/4, Qwen2.5-Omni, Qwen3-VL, Qwen3-Omni, the canary (qwen3) and SmolLM2 (llama) decoders
    "dasllama_arch_gemma2" => "llm", "dasllama_arch_gemma3" => "llm,vision", "dasllama_arch_gemma4" => "llm,vision,audio",
    "dasllama_arch_glm4moe" => "llm", "dasllama_arch_gptoss" => "llm", "dasllama_arch_llama" => "llm,audio",
    "dasllama_arch_mistral3" => "llm", "dasllama_arch_phi3" => "llm", "dasllama_arch_qwen2" => "llm,vision",
    "dasllama_arch_qwen2moe" => "llm", "dasllama_arch_qwen3" => "llm,vision,audio", "dasllama_arch_qwen35" => "llm", "dasllama_arch_qwen4exp" => "llm",
    "dasllama_arch_qwen3moe" => "llm,vision,audio", "dasllama_attn_prefill" => "llm", "dasllama_batch" => "llm",
    "dasllama_bpe" => "llm", "dasllama_safetensors" => "llm", "dasllama_gpu_resident" => "llm,audio,vision", "dasllama_gpu_tier" => "llm",
    "dasllama_kv_dtype" => "llm", "dasllama_metal_decode" => "llm", "dasllama_metal_mtp_gemma" => "llm",
    "dasllama_dflash" => "llm", "dasllama_metal_dflash" => "llm", "dasllama_vulkan_dflash" => "llm", "dasllama_cuda_dflash" => "llm",
    "dasllama_vulkan_classes_dflash" => "llm", "dasllama_cuda_classes_dflash" => "llm",
    "dasllama_kv_codec" => "llm,audio",   // its f16 dot and axpy serve the parakeet decode
    "dasllama_metal_prefill" => "llm,tts,audio,vision,translate",   // its rope serves the Pocket codec transformer, its attention pair and FastConformer kernels the audio towers, its half convert the vision tower, its q8 GEMM the Bergamot chain
    "dasllama_moe" => "llm", "dasllama_mtp_gemma" => "llm", "dasllama_vulkan_mtp_gemma" => "llm", "dasllama_cuda_mtp_gemma" => "llm",
    "dasllama_parity" => "llm", "dasllama_ple" => "llm", "dasllama_prefix" => "llm", "dasllama_pretok" => "llm",
    // the TTS slab builds its rope tables through it; every gguf load
    // picks its factor tensor through it, and the vision and audio
    // tests load decoders
    "dasllama_rope" => "llm,tts,vision,audio",
    "dasllama_sampling" => "llm,vision,audio", "dasllama_scheduler" => "llm",
    "dasllama_spm" => "llm,tts,translate",   // the unigram arm is the Pocket tokenizer (test_tts_pocket); the model-file reader is Bergamot's
    "dasllama_bench" => "llm,infra",   // the rows serve lcpp_bench (llm) and the server's /bench (infra's tooling)
    "dasllama_tokenizer" => "llm", "dasllama_transformer" => "llm",
    "dasllama_unicode" => "llm,tts",   // cpt_count serves the TTS front end and its chunker
    "dasllama_vulkan_decode" => "llm", "dasllama_vulkan_prefill" => "llm", "dasllama_vulkan_seams" => "llm",
    "dasllama_cuda_common" => "llm,audio,vision,tts", "dasllama_cuda_dispatch" => "llm,audio,vision", "dasllama_cuda_classes" => "llm,audio,vision",
    "dasllama_cuda_decode" => "llm", "dasllama_cuda_stream" => "llm", "dasllama_math_cuda" => "llm", "dasllama_cuda_classes_rows" => "llm"
}

//! the stocked llm files a change to the engine core reaches - a witness a family and home, the codec rig, the regions
//! form, the segmented window, the CPU chains - beside every model-free llm file: the per-edit gate. The whole stocked
//! set stays the pre-merge gate (`--suite stocked` over the fleet, a pod a GPU home). Non-private: test_run_suites.das pins it
let LLM_CORE_WITNESSES <- [
    "test_batch_decode.das", "test_deltanet.das", "test_forward.das",
    "test_gpu_resident_gemma2_3.das", "test_gpu_resident_gemma4_e2b.das",
    "test_gpu_resident_gemma4_moe.das", "test_gpu_resident_gptoss.das", "test_gpu_resident_hybrid.das",
    "test_gpu_resident_llama.das", "test_gpu_resident_qwen_moe.das", "test_gpu_resident_qwen2.das",
    "test_gpu_resident_qwen3.das",
    "test_kv_codec.das", "test_prefill.das", "test_scheduler.das" ]

let CUDA_WITNESSES = "test_cuda_kernels.das,test_gpu_resident_llama.das,test_gpu_resident_hybrid.das,test_gpu_resident_qwen3.das,test_gpu_resident_qwen2.das,test_gpu_resident_qwen_moe.das,test_mtp_gemma_drafter.das,test_cuda_mint.das,test_gpu_resident_gptoss.das,test_gpu_resident_gemma4_dense.das,test_gpu_resident_gemma4_moe.das"
let KV_CODEC_WITNESSES = "test_kv_codec.das,test_vulkan_kv_codec_kernels.das,test_gpu_resident_llama.das,test_gpu_resident_qwen3.das,test_gpu_resident_qwen2.das"
let TOKENIZER_WITNESSES = "test_tokenizer.das,test_tts_textnorm.das,test_chat.das,test_tok_seed.das"
let METAL_WITNESSES = "test_gpu_resident_llama.das,test_gpu_resident_hybrid.das,test_gpu_resident_qwen_moe.das,test_gpu_resident_gemma4_e2b.das,test_metal_format_census.das,test_matmul_batch.das,uncategorized_no_aot_persistent_heap.das,uncategorized_persistent_heap_stack_524288.das"

//! engine module stem -> the llm files a change to it reaches (base names, comma-separated) in place of the core
//! witnesses: a leaf module's own carriers and kernel cells. A module with no row here and an llm reach in
//! MODULE_AREAS (or no row there) is core. Its other areas still come from MODULE_AREAS. Non-private:
//! test_run_suites.das pins every row to the census
let MODULE_WITNESSES <- {
    "dasllama_cuda_common" => CUDA_WITNESSES, "dasllama_cuda_dispatch" => CUDA_WITNESSES, "dasllama_cuda_classes" => CUDA_WITNESSES,
    "dasllama_cuda_decode" => CUDA_WITNESSES, "dasllama_cuda_stream" => CUDA_WITNESSES, "dasllama_math_cuda" => CUDA_WITNESSES,
    "dasllama_cuda_classes_rows" => CUDA_WITNESSES,
    "dasllama_arch_llama" => "test_gpu_resident_llama.das,test_forward.das,test_prefill.das",
    "dasllama_arch_qwen2" => "test_gpu_resident_qwen2.das",
    "dasllama_arch_qwen3" => "test_gpu_resident_qwen3.das",
    "dasllama_arch_qwen35" => "test_gpu_resident_hybrid.das,test_gpu_resident_regions_hybrid_formats.das,test_gpu_resident_moe_formats.das,test_deltanet.das,test_dflash_drafter.das,test_vulkan_mint.das",
    "dasllama_arch_qwen3moe" => "test_gpu_resident_qwen_moe.das,test_gpu_resident_moe_formats.das,test_gpu_resident_format_census.das",
    "dasllama_arch_qwen2moe" => "test_gpu_resident_qwen_moe.das",
    "dasllama_arch_qwen4exp" => "test_qwen4exp.das,test_gpu_resident_hc.das,test_gpu_resident_hc_micro.das",
    "dasllama_arch_gptoss" => "test_gpu_resident_gptoss.das",
    "dasllama_arch_gemma2" => "test_gpu_resident_gemma2_3.das",
    "dasllama_arch_gemma3" => "test_gpu_resident_gemma2_3.das",
    "dasllama_arch_gemma4" => "test_gpu_resident_gemma4_e2b.das,test_gpu_resident_gemma4_dense.das,test_gpu_resident_gemma4_moe.das,test_mtp_gemma_drafter.das",
    "dasllama_arch_phi3" => "test_gpu_resident_llama.das",
    "dasllama_arch_glm4moe" => "test_dflash_drafter.das", "dasllama_arch_mistral3" => "test_chat.das,test_model_specs.das",
    "dasllama_moe" => "test_gpu_resident_qwen_moe.das,test_gpu_resident_moe_formats.das,test_gpu_resident_gptoss.das,test_gpu_resident_gemma4_moe.das,test_vulkan_dec_tail.das,test_vulkan_moe_cm2.das,test_vulkan_host_twin.das",
    "dasllama_mtp_gemma" => "test_mtp_gemma_drafter.das,test_gpu_resident_gemma4_e2b.das", "dasllama_vulkan_mtp_gemma" => "test_mtp_gemma_drafter.das",
    "dasllama_cuda_mtp_gemma" => "test_mtp_gemma_drafter.das",
    "dasllama_dflash" => "test_dflash_drafter.das,test_mtp_accept_walks.das,test_cuda_tower_asr.das,test_vulkan_tower_kernels.das,test_gpu_resident_gemma4_e2b.das,test_gpu_resident_qwen3.das", "dasllama_metal_dflash" => "test_dflash_drafter.das",
    "dasllama_vulkan_dflash" => "test_dflash_drafter.das,test_gpu_resident_gemma4_e2b.das,test_gpu_resident_qwen3.das",
    "dasllama_cuda_dflash" => "test_dflash_drafter.das,test_gpu_resident_gemma4_e2b.das,test_gpu_resident_qwen3.das",
    "dasllama_vulkan_classes_dflash" => "test_vulkan_tower_kernels.das,test_dflash_drafter.das,test_gpu_resident_gemma4_e2b.das,test_gpu_resident_qwen3.das",
    "dasllama_cuda_classes_dflash" => "test_cuda_tower_asr.das,test_dflash_drafter.das,test_gpu_resident_gemma4_e2b.das,test_gpu_resident_qwen3.das",
    "dasllama_metal_decode" => METAL_WITNESSES, "dasllama_metal_prefill" => METAL_WITNESSES, "dasllama_metal_mtp_gemma" => METAL_WITNESSES,
    "dasllama_scheduler" => "test_scheduler.das",
    "dasllama_sampling" => "test_sampling.das,test_mtp_sampled_walk.das,test_mtp_accept_walks.das,test_scheduler.das,test_dflash_drafter.das,test_gpu_resident_gemma4_e2b.das",
    "dasllama_batch" => "test_batch_decode.das,test_matmul_batch.das,test_gpu_resident_llama.das",
    "dasllama_kv_codec" => KV_CODEC_WITNESSES, "dasllama_kv_dtype" => KV_CODEC_WITNESSES,
    "dasllama_prefix" => "test_kv_prefix.das,test_kv_paged.das",
    "dasllama_ple" => "test_ple_check.das,test_gpu_resident_gemma4_e2b.das",
    "dasllama_parity" => "test_parity.das,test_parity_pregate.das,uncategorized__dasllama_internal_persistent_heap_stack_524288.das",
    "dasllama_bench" => "test_facade.das,test_served_bench.das",
    "dasllama_tokenizer" => TOKENIZER_WITNESSES, "dasllama_bpe" => TOKENIZER_WITNESSES, "dasllama_spm" => TOKENIZER_WITNESSES,
    "dasllama_safetensors" => "test_gguf_shards.das,test_mlx_snapshot.das,test_mlx_qwen4exp.das,test_tokenizer.das",
    "dasllama_pretok" => TOKENIZER_WITNESSES, "dasllama_unicode" => TOKENIZER_WITNESSES,
    "dasllama_chat" => "test_chat.das,test_tool_formats.das,test_think_split.das,test_scheduler.das,test_dflash_drafter.das,test_gpu_resident_qwen3.das",
    "dasllama_tools" => "test_chat.das,test_tool_formats.das,test_think_split.das,test_scheduler.das",
    "dasllama_attn_prefill" => "test_prefill.das,test_prefill_cpu_kernels.das,test_attn_span.das"
}

let BENCH_WITNESSES <- ["test_facade.das", "test_served_bench.das", "test_bench_records_schema.das"]

//! the files whose cells read the GPU tier as no home's entry armed it: a batch-mate requiring the engine core brings the
//! Vulkan home's entry, which arms the tier at its init under the run's DASLLAMA_GPU=1, so each runs in a child of its own
let TIER_CONTRACT_FILES <- ["test_gpu_tier.das", "test_gpu_serving_declines.das"]

def private csv_list(csv : string) : array<string> => <- split_trimmed(csv, ",")

//! a `tests/_*.das` fixture -> the areas a change to it reaches when no test requires it by name
def private fixture_areas(name : string) : array<string> {
    if (name |> starts_with("_tts_")) {
        return <- ["tts"]
    } elif (name |> starts_with("_audio_")) {
        return <- ["audio"]   //! the audio area's cell modules and its twins' legs
    } elif (name == "_vision_oracle.das") {
        return <- ["vision"]
    } elif (name == "_nmt_oracle.das") {
        return <- ["translate", "llm"]   //! the Bergamot step's kernel cells ride the kernels files
    } elif (name == "_npy.das") {
        return <- ["audio", "translate"]   //! the GigaAM and Bergamot oracle dumps
    } elif (name == "_tower_twin.das") {
        return <- ["audio", "vision"]
    } elif (name == "_vkd_oracles.das") {
        return <- ["llm", "tts", "audio", "vision"]   //! CellPlanes is the harness of the TTS and tower Vulkan cells too
    } elif (name |> starts_with("_vk") || name == "_metal_kernel_common.das" || name == "_mtl_toy.das" || name == "_kq_dot.das" || name == "_kernel_oracles.das" || name == "_kv_codec_oracles.das"
            || name == "_fp4_feed.das" || name == "_mmq_ref.das") {
        return <- ["llm"]
    } elif (name == "_kq_fixtures.das" || name == "_f32_tile_pin.das") {
        return <- ["llm", "tts"]   //! test_tts_blocks' K-quant lane cell reads the planes; the TTS parity rail runs under the tile pin
    }
    return <- standing_areas()
}

//! a changed path -> the areas it reaches (empty for a path outside the module, and for a model-suite
//! test file - those take --arm). Non-private: test_run_suites.das pins the table
def areas_for_path(path : string) : array<string> {
    let p = path |> replace("\\", "/")
    var none : array<string>
    if (!(p |> starts_with("modules/dasLLAMA/"))) {
        return <- none
    }
    let rel = slice(p, length("modules/dasLLAMA/"))
    let name = base_name(p)
    if (rel |> starts_with("models/")) {
        return <- ["audio"]   //! the checked-in silero VAD weights
    }
    if (!(name |> ends_with(".das")) || name == "run.das" || name == "REVIEW.das") {   //! a folder's lint gate is tooling
        return <- ["infra"]
    }
    if (rel |> starts_with("tests/")) {
        if (name |> starts_with("_")) {
            return <- fixture_areas(name)
        }
        return <- test_file_areas(name)
    }
    if (rel |> starts_with("dasllama/")) {
        let stem = name |> ends_with(".das") ? slice(name, 0, length(name) - 4) : name
        let row = MODULE_AREAS?[stem] ?? ""
        return <- (empty(row) ? standing_areas() : csv_list(row))
    }
    if (rel |> starts_with("performance/") || rel |> starts_with("harness/")) {
        return <- ["infra"]
    } elif (rel |> starts_with("benchmarks/")) {
        return <- ["llm"]
    }
    return <- standing_areas()
}

//! what --changed runs for one changed path
struct Reach {
    files : array<string>   //! the test files it runs, base names
    areas : array<string>   //! the areas it runs whole - never llm, whose files are named
    core : bool             //! the engine core changed: the whole stocked set is owed before the PR
    note : string           //! why a path runs nothing, "" when it runs something or lies outside the module
}

//! the files that load no model: the model-free suite, and the device kernel files (`gpu-kernels`) the test wall keeps
//! out of it. Non-private: test_run_suites.das pins the census
def free_files() : array<string> {
    var files <- suite_files("model-free")
    files |> push_from(suite_files("gpu-kernels"))
    return <- files
}

//! the llm area's model-free files - the kernel cells and the units every engine change reaches
def private llm_model_free() : array<string> {
    let free <- free_files()
    return <- [for (name in area_tests("llm")); name; where find_index(free, "{TESTS_DIR}/{name}") >= 0]
}

def private any_reached(stems : array<string>; reached : table<string>) : bool {
    for (s in stems) {
        return true if (key_exists(reached, s))
    }
    return false
}

//! every test file and fixture of the folder by base name, each with the fixture stems its `require _<stem>` lines name
def private folder_requires() : table<string; array<string>> {
    var requires : table<string; array<string>>
    fio::dir(TESTS_DIR) $(name) {
        if (name |> ends_with(".das") && (name |> starts_with("test_") || name |> starts_with("failed_") || name |> starts_with("uncategorized") || name |> starts_with("_"))) {
            var stems : array<string>
            fopen("{TESTS_DIR}/{name}", "rb") $(f) {
                if (f != null) {
                    for (line in split(fread(f), "\n")) {
                        continue if (!(line |> starts_with("require _")))
                        var inscope parts <- split_by_chars(slice(line, length("require ")), " \t/\r")
                        stems |> push(parts[0])
                    }
                }
            }
            requires[name] <- stems
        }
    }
    return <- requires
}

//! the test files of the folder whose `require _<stem>` reaches `stem`, through the fixtures between: a fixture's
//! change runs the per-PR tests that read it, and no file of an on-demand area. Non-private: test_run_suites.das pins it
def tests_requiring(stem : string) : array<string> {
    var requires <- folder_requires()
    var reached <- { stem }
    var grew = true
    while (grew) {
        grew = false
        for (name, stems in keys(requires), values(requires)) {
            continue if (!(name |> starts_with("_")))
            let fstem = slice(name, 0, length(name) - 4)
            if (!key_exists(reached, fstem) && any_reached(stems, reached)) {
                reached |> insert(fstem)
                grew = true
            }
        }
    }
    var listed <- free_files()
    listed |> push_from(suite_files("stocked"))
    let census <- {for (f in listed); base_name(f) => null}   //! a model-suite file the fixture reaches takes --arm, not this run
    var files <- [for (name, stems in keys(requires), values(requires)); name; where key_exists(census, name) && any_reached(stems, reached)]
    sort(files)
    return <- files
}

//! the llm reach of a change that lands in `areas`: nothing without llm among them, a module's own witness row, else
//! the core - every model-free llm file and the core witnesses
def private llm_reach(var r : Reach; areas : array<string>; witness : string) {
    return if (find_index(areas, "llm") < 0)
    if (!empty(witness)) {
        r.files |> push_from(csv_list(witness))
        return
    }
    r.core = true
    r.files |> push_from(llm_model_free(), clone_to_move(LLM_CORE_WITNESSES))
}

//! a changed path -> what --changed runs for it: a test file itself, a fixture the tests requiring it, an engine
//! module its MODULE_AREAS areas whole but llm, where it runs its MODULE_WITNESSES row or the core set, a
//! benchmark the bench rows' files, any other path its areas the same way. Non-private: test_run_suites.das pins it
def reach_for_path(path : string) : Reach {
    var r : Reach
    let p = path |> replace("\\", "/")
    return <- r if (!(p |> starts_with("modules/dasLLAMA/")))
    let rel = slice(p, length("modules/dasLLAMA/"))
    let name = base_name(p)
    if (rel |> starts_with("tests/") && name |> ends_with(".das") && name != "run.das") {
        if (name |> starts_with("_")) {
            r.files <- tests_requiring(slice(name, 0, length(name) - 4))
            if (empty(r.files)) {   //! a fixture no test requires by name (a root a child spawns): its areas, the llm one by the core
                let areas <- areas_for_path(p)
                r.areas <- [for (a in areas); a; where a != "llm"]
                llm_reach(r, areas, "")
            }
            return <- r
        }
        if (find_index(free_files(), p) >= 0 || find_index(suite_files("stocked"), p) >= 0 || find_index(suite_files("examples"), p) >= 0) {
            r.files |> push(name)
        } else {
            r.note = "a model suite file: run its suite with --arm scoped to the change"
        }
        return <- r
    }
    if (rel |> starts_with("benchmarks/") && name |> ends_with(".das")) {
        r.files := BENCH_WITNESSES
        return <- r
    }
    let areas <- areas_for_path(p)
    r.areas <- [for (a in areas); a; where a != "llm"]
    let stem = (rel |> starts_with("dasllama/")) && name |> ends_with(".das") ? slice(name, 0, length(name) - 4) : ""
    llm_reach(r, areas, empty(stem) ? "" : (MODULE_WITNESSES?[stem] ?? ""))
    return <- r
}

//! --exclude: a file whose base name contains any comma-separated token is dropped. Non-private:
//! test_run_suites.das gates it
def exclude_files(files : array<string>; exclude : string) : tuple<kept : array<string>; dropped : array<string>> {
    var kept, dropped : array<string>
    let tokens <- [for (tok in split(exclude, ",")); strip(tok); where !empty(strip(tok))]
    for (file in files) {
        let name = base_name(file)
        var hit = false
        for (tok in tokens) {
            hit ||= find(name, tok) >= 0
        }
        if (hit) {
            dropped |> push(file)
        } else {
            kept |> push(file)
        }
    }
    return <- (kept <- kept, dropped <- dropped)
}

//! the files a run drops ahead of its first child, `excluded` one EXCLUDED line a file with the reason, and where none
//! is kept `refusal`, the error that ends the run
struct RunExclusion {
    kept : array<string>
    excluded : array<string>
    refusal : string
}

def run_exclusion(files : array<string>; exclude, disabled : string) : RunExclusion {
    //! the caller's `--exclude` (`exclude_files`), then - where the children leave dasCUDA out
    //! (`disabled`, the comma list DASLLAMA_DISABLE_MODULES names) - the CUDA-only files
    //! (`CUDA_ONLY_EXCLUDE`), which do not compile without it. Non-private: test_run_suites.das
    //! gates it
    var r = RunExclusion()
    var inscope by_caller <- exclude_files(files, exclude)
    r.excluded <- [for (file in by_caller.dropped); "EXCLUDED {base_name(file)} (--exclude {exclude})"]
    let no_cuda = has_value(split_trimmed(disabled, ","), "dasCUDA")
    var inscope by_module <- exclude_files(by_caller.kept, no_cuda ? CUDA_ONLY_EXCLUDE : "")
    let why = "dasCUDA left out (DASLLAMA_DISABLE_MODULES={disabled}): it requires the CUDA kernel classes"
    r.excluded |> push_from([for (file in by_module.dropped); "EXCLUDED {base_name(file)} ({why})"])
    r.kept <- by_module.kept
    if (empty(r.kept)) {
        let by_exclude = empty(exclude) ? "" : "--exclude '{exclude}' "
        r.refusal = "{by_exclude}{no_cuda ? "with dasCUDA left out " : ""}dropped every file of the run"
    }
    return <- r
}

//! --only: the files whose base name contains any comma-separated token, the rest dropped; "" keeps the whole list.
//! Non-private: test_run_suites.das gates it
def only_files(files : array<string>; only : string) : array<string> {
    if (empty(only)) {
        return <- clone_to_move(files)
    }
    let tokens <- csv_list(only)
    var kept : array<string>
    for (file in files) {
        let name = base_name(file)
        var hit = false
        for (tok in tokens) {
            hit ||= find(name, tok) >= 0
        }
        if (hit) {
            kept |> push(file)
        }
    }
    return <- kept
}

// the trailing "N tests, ..." summary line of a dastest log ("" when the run died before
// printing one — crash, kill, or compile error). A final line with no trailing newline counts:
// a killed run's last bytes are exactly the ones worth reporting.
// Non-private: test_run_summary.das gates it.
def log_summary(log_path : string) : string => line_with(fread(log_path), " tests, ", last = true).line

//! a child's summary line by its exit code and log text: none where the kill switch ended it - the switch writes no
//! summary, so a line there carrying the marker is a cell's own text. Non-private: test_run_suites.das gates it
def child_summary(rc : int; log_text : string) : string
    => rc == KILL_SWITCH_EXIT_CODE ? "" : line_with(log_text, " tests, ", last = true).line

//! the DONE line's account of a child: its summary, else the kill switch's line where the switch ended it, else that
//! it died early. Non-private: test_run_suites.das gates it
def child_verdict(rc : int; summary, log_text : string) : string {
    return summary if (!empty(summary))
    if (rc == KILL_SWITCH_EXIT_CODE) {
        let said = log_wall_line(log_text)
        return empty(said) ? "the test wall's kill switch ended the child, exit {rc} (read the log)" : said
    }
    return "no dastest summary - the run died early (read the log)"
}

//! the non-empty line of `text` that holds `needle` - its last such line where `last`, else its first - and where in it
//! the needle starts ("" and -1 where none does)
def private line_with(text, needle : string; last : bool) : tuple<line : string; at : int> {
    var found = (line = "", at = -1)
    for (line in non_empty_lines(text)) {
        let at = find(line, needle)
        if (at >= 0) {
            found = (line = clone_string(line), at = at)
            break if (!last)
        }
    }
    return found
}

//! `word` opens `line`, or follows a tty color code within its first 8 bytes
def private verdict_word_at(line, word : string) : bool {
    let p = find(line, word)
    return p == 0 || (p > 0 && p <= 8 && starts_with(line, "\x1b"))
}

//! the PASS and FAIL file sets of a dastest log, by base name - a tty color code may sit ahead of the word - and
//! whether the together root itself failed (its compile, its simulate, its refusal): then every FAIL is the root's,
//! not a file's
def log_verdicts(log_path : string) : tuple<passed : array<string>; failed : array<string>; root_failed : bool> {
    var passed, failed : array<string>
    var root_failed = false
    for (line in non_empty_lines(fread(log_path))) {
        root_failed ||= find(line, "the together root") >= 0 || find(line, "] together: ") >= 0
        for (word in ["PASS ", "FAIL "]) {
            // a per-test line reads `--- PASS 'name'`: only a line start or a color code may sit ahead of a verdict
            continue if (!verdict_word_at(line, word))
            var inscope words <- split(line, " ")
            continue if (length(words) < 2)
            if (word == "PASS ") {
                passed |> push(base_name(words[1]))
            } else {
                failed |> push(base_name(words[1]))
            }
        }
    }
    return <- (passed <- passed, failed <- failed, root_failed = root_failed)
}

//! the filters a child's tests read at run time - the arms (none under the full suite), the families, the categories - as
//! dastest `--run-env` pairs: dastest sets them once the test program has compiled, so a filter is no part of the
//! program's compile record and a run under another filter reads the same record. Non-private: test_run_suites.das pins it
def child_run_env(cfg : RunArgs; arm : string) : string {
    var pairs : array<string>
    if (!cfg.full) {
        pairs |> push("DASLLAMA_TEST_ARMS={arm}")
    }
    if (!empty(cfg.family)) {   // composes with either mode: a family-scoped full gate is the profiling-cadence default
        pairs |> push("DASLLAMA_TEST_FAMILY={cfg.family}")
    }
    if (!empty(cfg.category)) {
        pairs |> push("DASLLAMA_TEST_CATEGORIES={cfg.category}")
    }
    return join([for (pair in pairs); " --run-env \"{pair}\""], "")
}

//! the tune manifest a run's children read: the caller's `caller`, else the per-box file `box`
//! where `box_valid`, else none
def box_manifest_pick(caller, box : string; box_valid : bool) : string
    => !empty(caller) ? caller : (box_valid ? box : "")

//! the tune manifest this run's children read (`box_manifest_pick` on the caller's DAS_TUNE_MANIFEST and the box's file)
def private children_manifest() : string {
    let box = box_tune_manifest(g_env_prof.box)
    return box_manifest_pick(g_env_core.tune_manifest, box, stat(box).is_valid)
}

// CPU_PREFILL: the suites' CPU truth halves and parity controls prefill on the CPU by intent.
// HAZARD_STRICT: the scheduling-unification ratchet's runtime half — an undeclared dispatch
// on an armed hazard oracle PANICS here, so declaration gaps can never ship quietly.
// cmd.exe has no VAR=val prefix — `set "VAR=val"&&` is the safe form (llvm_tune's tuner_cmd)
// Non-private: test_run_suites.das pins the K/V start a `--poison-kv` run hands its children
def child_envs(cfg : RunArgs; win : bool; media : bool = false; scratch : string = "") : string {
    var envs = ""
    if (win) {
        envs = ((cfg.full ? "set \"DASLLAMA_PARITY_FULL=1\"&& " : "")
            + "set \"DASLLAMA_CPU_PREFILL=1\"&& set \"DASLLAMA_METAL_HAZARD_STRICT=1\"&& ")
    } else {
        envs = (cfg.full ? "DASLLAMA_PARITY_FULL=1 " : "") + "DASLLAMA_CPU_PREFILL=1 DASLLAMA_METAL_HAZARD_STRICT=1 "
        if (get_platform_name() == "linux") {
            // glibc parks each late-loaded module's thread-local block in a 512-byte
            // surplus; libLLVM, a shared module and the NVIDIA Vulkan driver's three
            // libraries overrun it together, and vkCreateInstance then fails for good.
            // glibc hands a freed block of a model's size back to the kernel, so every load after a
            // child's first pays the first touch of its pages again - about a second a gigabyte;
            // with no mmap'd blocks and no trim the heap keeps the pages for the next load. A media
            // run's alone: its models are a few GB, and a child that kept a chat model's pages
            // could stand two of them against the box's memory
            let heap = media ? ":glibc.malloc.mmap_max=0:glibc.malloc.trim_threshold=68719476736" : ""
            envs = "GLIBC_TUNABLES=glibc.rtld.optional_static_tls=16384{heap} {envs}"
        }
    }
    if (cfg.poison_kv) {
        envs = win ? "{envs}set \"DASLLAMA_KV_INIT=poison\"&& " : "DASLLAMA_KV_INIT=poison {envs}"
    }
    if (!empty(scratch)) {
        envs = win ? "{envs}set \"TEMP={scratch}\"&& set \"TMP={scratch}\"&& " : "TMPDIR=\"{scratch}\" {envs}"
    }
    if (cfg.no_tune) {
        envs = (win ? "{envs}set \"DAS_TUNE_POLICY=reference\"&& set \"DASLLAMA_IMAGE=0\"&& "
            : "DAS_TUNE_POLICY=reference DASLLAMA_IMAGE=0 {envs}")
    }
    if (cfg.suite == "image-vulkan") {
        // the offline-bake suite: arm the tier so the probed config carries a vulkan section —
        // the bake itself is DRY (no device calls), so this is safe on GPU-less boxes too
        envs = (win ? "{envs}set \"DASLLAMA_GPU=1\"&& set \"DASLLAMA_GPU_VRAM_MB=2000\"&& "
            : "DASLLAMA_GPU=1 DASLLAMA_GPU_VRAM_MB=2000 {envs}")
    }
    // ONE tune per box (rig_tune_manifest's contract, inlined — profile_common pulls the whole
    // engine): pin the children to the box manifest so their .dlim image identities agree with
    // the perf rig's. Without this every child auto-mints its own sidecar tune, computes a
    // FOREIGN image identity, and its save GC-purges the rig's images as dead siblings
    // (observed 2026-07-30: fam-gemma4 freed 34 GB of bench flavors). A caller-set
    // DAS_TUNE_MANIFEST inherits; a box without a minted manifest keeps sidecar behavior. The
    // --no-tune arm pins nothing: an explicit manifest disables the policy rail it runs on.
    let manifest = children_manifest()
    if (empty(g_env_core.tune_manifest) && !cfg.no_tune && !empty(manifest)) {
        envs = (win ? "{envs}set \"DAS_TUNE_MANIFEST={manifest}\"&& "
            : "DAS_TUNE_MANIFEST='{manifest}' {envs}")
    }
    return forced_env(envs, forced_tune_file(cfg), win)
}

//! `envs` with the tune file `forced` named ahead of a child (`DAS_TUNE_FORCE`), in cmd.exe's form under `win`; `envs`
//! where no file is forced. Non-private: test_run_suites.das pins both forms
def forced_env(envs, forced : string; win : bool) : string {
    return envs if (empty(forced))
    return win ? "{envs}set \"DAS_TUNE_FORCE={forced}\"&& " : "DAS_TUNE_FORCE='{forced}' {envs}"
}

//! The tune file a run's children are forced onto (`DAS_TUNE_FORCE`), "" for none: the manifest in effect - the caller's
//! `DAS_TUNE_MANIFEST`, else the box's - so its kernels stamp whatever the file's age against the binary; a test run reads
//! the box's winners, and a rebuild does not drop every kernel to its fallback. None under `--no-tune`, where the file
//! is absent, or where the caller forced a file already
def forced_tune_file(cfg : RunArgs) : string
    => forced_tune_file_of(cfg.no_tune, g_env_tune.tune_force, g_env_core.tune_manifest, box_tune_manifest(g_env_prof.box))

//! `forced_tune_file` on what the run read: `caller_forced` a file the caller forced, `manifest` the caller's
//! `DAS_TUNE_MANIFEST`, `box_manifest` the box's own. Non-private: test_run_suites.das pins each answer
def forced_tune_file_of(no_tune : bool; caller_forced, manifest, box_manifest : string) : string {
    return "" if (no_tune || !empty(caller_forced))
    let file = box_manifest_pick(manifest, box_manifest, stat(box_manifest).is_valid)
    return stat(file).is_valid ? file : ""
}

//! stdout of a git command, "" when git is absent or the command fails
def private git_lines(args : string) : array<string> {
    var out = ""
    unsafe(popen("git {args}") $(f) {
        if (f != null) {
            out = fread(f)
        }
    })
    return <- [for (ln in split(out, "\n")); strip(ln); where !empty(strip(ln))]
}

//! the changed set for --changed: the working tree against --base, plus untracked files of the module
def private changed_paths(base : string) : array<string> {
    var paths <- git_lines("diff --name-only {base}")
    paths |> push_from(git_lines("ls-files --others --exclude-standard modules/dasLLAMA"))
    return <- paths
}

//! the files an area list runs, as runner paths: every area file (deduplicated, sorted) that the
//! model-free or stocked census or the examples suite lists; returns the image-suite arm tokens the areas own too
def area_plan(areas : array<string>) : tuple<files : array<string>; image_arms : string> {
    let free <- free_files()
    var stocked <- suite_files("stocked")
    stocked |> push_from(suite_files("examples"))
    var picked : table<string; void?>
    var arms : array<string>
    for (a in areas) {
        for (name in area_tests(a)) {
            picked |> insert("{TESTS_DIR}/{name}", null)
        }
        let armcsv = area_image_arms(a)
        if (!empty(armcsv)) {
            arms |> push_from(csv_list(armcsv))
        }
    }
    var files <- [for (f in keys(picked)); f; where find_index(free, f) >= 0 || find_index(stocked, f) >= 0]
    sort(files)
    if (!empty(arms)) {
        files |> push("{TESTS_DIR}/test_model_image.das")
    }
    return <- (files <- files, image_arms = join(arms, ","))
}

//! the "N tests, P passed, F failed, E errors, S skipped" summary's skipped count, 0 when absent
def skipped_cells(summary : string) : int {
    for (part in split(summary, ",")) {
        let s = strip(part)
        if (s |> ends_with(" skipped")) {
            return to_int(slice(s, 0, length(s) - length(" skipped")))
        }
    }
    return 0
}

//! the files a child reported from their passes on record and did not run: its log's `CACHED <file>` lines, a color
//! code ahead of the word or none
def cached_files(log_text : string) : int {
    var n = 0
    for (line in split(log_text, "\n")) {
        n++ if (verdict_word_at(line, "CACHED "))
    }
    return n
}

//! the REPORT line's count of the `n` files a run reported from their passes on record: "" where every file ran
def cached_report(n : int) : string => n == 0 ? "" : " {n} of them not run - passed before and unchanged (--rerun runs them),"

//! the area set of an --area / --changed run, canonical order. `status` >= 0 is main's exit code
//! (an argument error; a --changed run that reaches nothing); -1 means run `areas` (empty = a
//! suite run)
def select_areas(cfg : RunArgs) : tuple<status : int; areas : array<string>> {
    var none : array<string>
    if (!cfg.changed && empty(cfg.area)) {
        return <- (status = -1, areas <- none)
    }
    if (cfg.full || !empty(cfg.arm) || cfg.suite != "all") {
        to_log(LOG_ERROR, "--area / --changed take neither --arm, --full nor --suite: an area is the whole set - its tests, its arms\n")
        return <- (status = 2, areas <- none)
    }
    var want : table<string; void?>
    for (a in csv_list(cfg.area)) {
        if (find_index(AREAS, a) < 0) {
            to_log(LOG_ERROR, "unknown --area '{a}' ({join(AREAS, " | ")})\n")
            return <- (status = 2, areas <- none)
        }
        want |> insert(a, null)
    }
    if (cfg.changed) {   //! the files are the reach plan's (`changed_plan`)
        return <- (status = -1, areas <- none)
    }
    var areas <- [for (a in AREAS); a; where key_exists(want, a)]
    to_log(LOG_INFO, "AREAS {join(areas, ",")}\n")
    return <- (status = -1, areas <- areas)
}

//! the --changed run: each changed path's reach (`reach_for_path`) on a CHANGED line, the small areas whole through
//! `area_plan`, the llm files by name, the llm image arms when the core changed; `core` says the whole stocked set is
//! owed before the PR. `status` >= 0 is main's exit code (a base git cannot resolve; a change that reaches nothing)
def private changed_plan(cfg : RunArgs) : tuple<status : int; files : array<string>; image_arms : string; core : bool> {
    var none : array<string>
    if (empty(git_lines("rev-parse --verify --quiet {cfg.base}"))) {   // a base git cannot resolve would read as "nothing changed"
        to_log(LOG_ERROR, "--base '{cfg.base}' is not a commit git knows - fetch origin, or name another base\n")
        return <- (status = 2, files <- none, image_arms = "", core = false)
    }
    var picked, want : table<string; void?>
    var core = false
    for (p in changed_paths(cfg.base)) {
        var inscope r <- reach_for_path(p)
        to_log(LOG_INFO, "CHANGED {p} -> {describe_reach(r)}\n") if (!empty(r.files) || !empty(r.areas) || !empty(r.note))
        for (f in r.files) {
            picked |> insert("{TESTS_DIR}/{f}", null)
        }
        for (a in r.areas) {
            want |> insert(a, null)
        }
        core ||= r.core
    }
    if (empty(picked) && empty(want)) {
        to_log(LOG_INFO, "CHANGED nothing vs {cfg.base} reaches a test - nothing to run\n")
        return <- (status = 0, files <- none, image_arms = "", core = false)
    }
    let areas <- [for (a in AREAS); a; where key_exists(want, a)]
    let plan <- area_plan(areas)
    for (f in plan.files) {
        picked |> insert(f, null)
    }
    var arms <- csv_list(plan.image_arms)
    if (core) {
        arms |> push_from(csv_list(area_image_arms("llm")))
    }
    var files <- [for (f in keys(picked)); f; where base_name(f) != "test_model_image.das"]
    sort(files)
    if (!empty(arms)) {
        files |> push("{TESTS_DIR}/test_model_image.das")
    }
    if (!empty(areas)) {
        to_log(LOG_INFO, "AREAS {join(areas, ",")}\n")
    }
    to_log(LOG_INFO, "PLAN {length(files)} file(s){core ? " - the engine core changed, the whole stocked set is owed before the PR" : ""}\n")
    return <- (status = -1, files <- files, image_arms = join(arms, ","), core = core)
}

//! a CHANGED line's tail: the files by name or the core, the areas run whole, or why nothing runs
def private describe_reach(r : Reach) : string {
    return r.note if (empty(r.files) && empty(r.areas))
    let named = empty(r.files) ? "" : "{length(r.files)} file(s): {join(r.files, ", ")}"
    let what = r.core ? "the engine core: every model-free llm file and the {length(LLM_CORE_WITNESSES)} core witnesses" : named
    let sep = empty(what) ? "" : "; "
    return empty(r.areas) ? what : "{what}{sep}areas {join(r.areas, ",")} whole"
}

//! a `[test]` function's wall in seconds in the suites the wall is armed for: past it the test is a red that ends the run
let MODEL_FREE_BATCH = 1000   //! the whole suite one program: no file loads a model, and a file costs a context's clone, so one parse and one JIT serve them all
//! on Windows a batch's JIT DLL exports over two symbols a function and a DLL holds 65,535 exports: the suite whole
//! does not link as one DLL, nor does a batch of twenty of its files; sixteen do
let MODEL_FREE_BATCH_WINDOWS = 16
let MODEL_BATCH = 10
//! the suites whose files run as a process of their own kind: their device leak gates count the whole process
let PROCESS_SUITES <- ["decode", "mtp", "prefill", "matrix", "kernels", "image", "image-vulkan", "coverage"]
let TEST_WALL_SECONDS = 30
//! the exit code of a child its kill switch ended: a test past `--test-wall` (dastest's `kill_switch.das`)
//! files a child: `--batch`, else `MODEL_FREE_BATCH` on the model-free suite (`MODEL_FREE_BATCH_WINDOWS` on Windows) and
//! `MODEL_BATCH` on any other run
def batch_files(cfg : RunArgs) : int {
    return cfg.batch if (cfg.batch > 0)
    return cfg.suite != "model-free" ? MODEL_BATCH : (get_platform_name() == "windows" ? MODEL_FREE_BATCH_WINDOWS : MODEL_FREE_BATCH)
}

//! a child's per-test wall in seconds, 0 = none: the run's own (`--test-wall`), else the model-free suite's outside the
//! full suite (DASLLAMA_PARITY_FULL=1), whose cells are the ones no wall admits
def wall_seconds(cfg : RunArgs) : int => wall_seconds_in(cfg, parity_full_mode())

//! `wall_seconds` where `full` says whether the run is the full suite. Non-private: test_run_suites.das pins both
def wall_seconds_in(cfg : RunArgs; full : bool) : int
    => cfg.test_wall >= 0 ? cfg.test_wall : ((cfg.suite == "model-free" && !full) ? TEST_WALL_SECONDS : 0)

//! the line a child's kill switch printed as it ended the process ("" where the log holds none). Non-private:
//! test_run_suites.das gates it
def log_wall_line(log_text : string) : string {
    let said = line_with(log_text, "kill switch: ", last = true)
    return said.at < 0 ? "" : slice(said.line, said.at)
}

//! the first compiler error a child's log holds, from its `error[` and cut at 240 characters ("" where it holds none):
//! what a batch whose root failed stopped on. Non-private: test_run_suites.das gates it
def log_error_line(log_text : string) : string {
    let said = line_with(log_text, "error[", last = false)
    return "" if (said.at < 0)
    let line = slice(said.line, said.at)
    return length(line) > 240 ? slice(line, 0, 240) : line
}

//! a child's dastest timeout in seconds: the full gate, the stocked gate and --changed (the stocked files a change
//! reaches) hold the large tier's tails - the parity file alone runs past 20 min there, the llama resident file's
//! CPU chains past 1200 s on a cold JIT cache; 1200 s is the arm-mode budget
def child_timeout(cfg : RunArgs) : int
    => (cfg.full || cfg.suite == "stocked" || cfg.suite == "gpu-kernels" || cfg.changed) ? 3600 : 1200

//! the suite forms' argument contract: 0 = run, else main's exit code
def check_suite_args(cfg : RunArgs) : int {
    if (cfg.no_tune && (cfg.suite == "image" || cfg.suite == "image-vulkan")) {   //! the arm runs with the image rail off, and the image suites ARE the rail
        to_log(LOG_ERROR, "--no-tune sets DASLLAMA_IMAGE=0 in every child, so it cannot drive the image suites - drop --no-tune or pick another suite\n")
        return 2
    }
    if (cfg.suite == "model-free" || cfg.suite == "gpu-kernels" || cfg.suite == "stocked" || cfg.suite == "wasm") {
        if (cfg.full || !empty(cfg.arm)) {   //! the per-PR gates have no arms to scope: a filter would silently shrink them
            to_log(LOG_ERROR, "--suite {cfg.suite} takes neither --arm nor --full: it is a whole per-PR gate (--exclude names what it drops)\n")
            return 2
        }
    } elif (cfg.full == !empty(cfg.arm)) {   // both set, or neither
        to_log(LOG_ERROR, "pass --arm <filter> scoped to what the change can affect (--full is disabled while the Metal build-out is in progress; a large model's cells = --category <name>)\n")
        return 2
    }
    if (cfg.full) {
        to_log(LOG_ERROR, "please narrow the scope to the correct set of gates, metal is not fully implemented yet\n")
        return 2
    }
    return 0
}

//! the model spec table: a test file that walks it (`model_specs()` in its text) reaches every
//! row's file
let SPEC_TABLE = "modules/dasLLAMA/performance/model_specs.das"

//! whether a model of `models` is a file of one of the categories `named` lists; a model of no
//! category is no category's, whatever `named` holds
def private has_category_model(models : array<string>; named : string) : bool {
    for (model in models) {
        let cat = model_category(model)
        return true if (!empty(cat) && list_names(named, cat))
    }
    return false
}

//! the test files of this folder that name a model file of one of the categories `named` lists - by its name or by its path
//! under the models directory, or through a row of the spec table they walk (`SPEC_TABLE`)
def category_files(named : string) : array<string> {
    var files : array<string>
    let spec_named = has_category_model(model_paths(fread(SPEC_TABLE)), named)
    dir(TESTS_DIR) $(name) {
        return if (!(name |> ends_with(".das")) || !(name |> starts_with("test_") || name |> starts_with("uncategorized")))
        let text = fread("{TESTS_DIR}/{name}")
        let walks_specs = find(text, "model_specs()") >= 0
        if (has_category_model(model_paths(text), named) || (spec_named && walks_specs)) {
            files |> push("{TESTS_DIR}/{name}")
        }
    }
    sort(files)
    return <- files
}

//! true when the filter value `v` is names and the marks between them - letters, digits and `_ , . : + -` - so it reads the
//! same inside a child's command line under every shell
def filter_value_stands(v : string) : bool {
    for (c in v) {
        let name_char = is_alpha(c) || is_number(c)
        return false if (!name_char && c != '_' && c != ',' && c != '.' && c != ':' && c != '+' && c != '-')
    }
    return true
}

//! 0 when --arm, --family and --category stand as the text a child's command line carries (`child_run_env`); a value with
//! any other character ends the run, the flag named
def check_filter_args(cfg : RunArgs) : int {
    for (flag, v in fixed_array("--arm", "--family", "--category"), fixed_array(cfg.arm, cfg.family, cfg.category)) {
        if (!filter_value_stands(v)) {
            to_log(LOG_ERROR, "{flag} '{v}' holds a character a filter cannot carry: names, and , . : + - between them\n")
            return 2
        }
    }
    return 0
}

//! 0 when --category stands: every name a category, and no flag beside it that picks files or cells another way
def check_category_args(cfg : RunArgs) : int {
    if (length(split(cfg.category, ",")) != length(split_trimmed(cfg.category, ","))) {
        to_log(LOG_ERROR, "--category '{cfg.category}' holds an empty name between its commas\n")
        return 2
    }
    let unknown <- unknown_categories(cfg.category)
    if (!empty(unknown)) {
        let known <- [for (cat in model_categories()); cat.name]
        to_log(LOG_ERROR, "unknown --category '{join(unknown, ",")}' ({join(known, " | ")}; ../TEST_CATEGORIES.md)\n")
        return 2
    }
    if (cfg.suite != "all" || !empty(cfg.area) || cfg.changed || !empty(cfg.arm) || cfg.full || !empty(cfg.family)) {
        to_log(LOG_ERROR, "--category runs its categories' files whole: it takes no --suite, --area, --changed, --arm, --full or --family\n")
        return 2
    }
    return 0
}

//! the run's files and the arm filter its image-suite child gets: an area plan, or the suite table
//! under the plain --arm
def plan_files(cfg : RunArgs; areas : array<string>) : tuple<files : array<string>; image_arms : string> {
    if (!empty(areas)) {
        return <- area_plan(areas)
    }
    var files <- suite_files(cfg.suite == "stocked" ? "gpu-kernels" : cfg.suite)
    if (cfg.suite == "stocked") {   //! the device kernel files first: the box's kernels compiled before a model's cell asks for one
        files |> push_from(suite_files("stocked"))
    }
    return <- (files <- files, image_arms = cfg.arm)
}

def private temp_dir() : string {
    var tmp = g_env_test.tmpdir
    if (empty(tmp)) {
        tmp = g_env_test.temp_dir   // the Windows spelling; TMPDIR is the POSIX one
    }
    return empty(tmp) ? "/tmp" : tmp
}

struct RunTally {
    failures : int
    skipped : int
    cached : int   //! the files reported from their passes on record, not run (`skips_cached`)
    failed_tags : array<string>
    walled : string   //! why the run stopped short - a test past the wall, a batch without its verdicts ("" = it ran to its end)
    waited : double   //! the seconds the children waited on the model copy (`--await-models`)
}

//! what a child's pass on record rests on and its compile record does not see: the box's `cpu`, the environment the
//! child runs under (`env`: a pass on one GPU home is none on another), and the size and time of every model file under
//! `dir` the child opens (`opened`, by path under `dir`; an absent one counts as such).
//! Non-private: test_run_suites.das pins it
def pass_stamp(dir : string; opened : array<string>; cpu, env : string) : string {
    let text = build_string() $(var w) {
        w |> write(cpu)
        w |> write("\n")
        w |> write(env)
        for (model in opened) {
            w |> write("\n{model} {file_stamp(path_join(dir, model))}")
        }
    }
    return "{hash(text)}"
}

//! whether a run reports a file from its pass on record, and records the passes it makes: every run but a `--rerun`,
//! the full suite's included - its passes are keyed apart by the knob that asks for it (`pass_stamp`)
def skips_cached(cfg : RunArgs) : bool => !cfg.rerun

//! the dastest flags that report the child over `chunk` from its passes on record (`skips_cached`): the flag and the
//! child's `pass_stamp`, over `envs` - the prefix its command line carries - and the knobs the environment sets
def private skip_flags(cfg : RunArgs; models, envs : string; chunk : array<string>) : string {
    return "" if (!skips_cached(cfg))
    var inscope opened <- opened_models(chunk, models, cfg.category, cfg.family, parity_full_mode())
    let env = "{envs}\n{env_set_knobs()}"
    return " --skip-fully-cached --pass-stamp {pass_stamp(models, opened, child_cpu(), env)}"
}

//! the per-test cache folder of the run's binary, and the folders of earlier binaries removed when the run is
//! on its own binary (a --das dry run keeps them); "" when the binary cannot be stat'd, and the children get
//! no cache. A record serves a child's unchanged module prefix: the engine's compile (~90 s a file) becomes a
//! read of a few seconds, so a re-run pays the test, not the parse. Non-private: test_run_suites.das gates it
def child_cache_dir(das_exe : string; own_binary : bool) : string {
    return "" if (!fexist(das_exe))
    let id = file_stamp(das_exe, "-")
    let dir = "{CHILD_CACHE_ROOT}/{id}"
    mkdir_rec(dir)
    if (own_binary) {
        var dead : array<string>
        fio::dir(CHILD_CACHE_ROOT) $(name) {
            if (name != id && name != "." && name != "..") {
                dead |> push(name)
            }
        }
        for (name in dead) {
            var why : string
            rmdir_rec("{CHILD_CACHE_ROOT}/{name}", why)
            let note = empty(why) ? "" : " - {why}"
            to_log(LOG_INFO, "CACHE removed {CHILD_CACHE_ROOT}/{name} (an earlier binary's records){note}\n")
        }
    }
    return dir
}

//! the model files `text` names under the models directory: every quoted name ending `.gguf` that is one file name or a
//! relative path to one (`drafters/<dir>/<file>`); a name built by interpolation, an absolute path (a drive path too), a climb out (`..`)
//! and a pattern are not. Non-private: test_run_suites.das pins it
def model_paths(text : string) : array<string> {
    var names : table<string>
    for (part in split(text, "\"")) {
        if (part |> ends_with(".gguf") && part != ".gguf" && find(part, "\{") < 0 && !(part |> starts_with("/")) && find(part, "..") < 0
                && find(part, "\\") < 0 && find(part, " ") < 0 && find(part, "*") < 0 && find(part, ":") < 0) {
            names |> insert(part)
        }
    }
    var out <- [for (n in keys(names)); n]
    sort(out)
    return <- out
}

//! `model_paths` that are one file name, no folder. Non-private: test_run_suites.das pins it
def model_names(text : string) : array<string> => <- [for (p in model_paths(text)); p; where find(p, "/") < 0]

//! the names of `files` and of every fixture of this folder they require, at any depth
def reached_names(files : array<string>) : table<string> {
    var requires <- folder_requires()
    var reached : table<string>
    var queue <- [for (f in files); base_name(f)]
    while (!empty(queue)) {
        let name = queue[length(queue) - 1]
        queue |> pop()
        continue if (key_exists(reached, name))
        reached |> insert(name)
        requires |> get(name) $(stems) {
            for (stem in stems) {
                queue |> push("{stem}.das")
            }
        }
    }
    return <- reached
}

let SCRATCH_FIXTURE = "_scratch_dir.das"   //! the fixture whose functions put a name under a test's scratch directory

//! where a function's text ends, `head` its text from the `def` on: the first later line that opens at column 0 with
//! anything but the brace closing it
def private fn_text_end(head : string) : int {
    var end = length(head)
    peek_data(head) $(d) {
        for (i in range(length(d) - 1)) {
            let c = int(d[i + 1])
            if (int(d[i]) == '\n' && c != ' ' && c != '\t' && c != '\n' && c != '\r' && c != '}') {
                end = i
                break
            }
        }
    }
    return end
}

//! each function `text` defines - a `def` at column 0: its call `name(`, and its text from the name to its end
def private defined_fns(text : string) : array<tuple<call : string; body : string>> {
    var out : array<tuple<call : string; body : string>>
    var inscope heads <- split("\n{text}", "\ndef ")
    for (i in range(1, length(heads))) {
        let head = heads[i]
        let from = head |> starts_with("private ") ? 8 : 0
        let paren = find(head, "(", from)
        if (paren > from) {
            out |> push((call = slice(head, from, paren + 1), body = slice(head, from, fn_text_end(head))))
        }
    }
    return <- out
}

def scratch_calls(text, fixture : string) : array<string> {
    //! The calls by which a test file's `text` puts a name under its scratch directory:
    //! `test_temp_dir()`, each function the scratch fixture defines (`fixture`, its text), and each
    //! function of the file whose own body makes one of those. Non-private: test_run_suites.das
    //! pins it
    var calls <- [for (f in defined_fns(fixture)); f.call]
    calls |> push("test_temp_dir()")
    var inscope own : array<string>
    for (f in defined_fns(text)) {
        let at = length(f.call)
        for (c in calls) {
            if (find(f.body, c, at) >= 0) {
                own |> push(f.call)
                break
            }
        }
    }
    calls |> push_from(own)
    return <- calls
}

def names_only_written(text, model : string; calls : array<string>) : bool {
    //! Whether `text` names `model` only as a file it writes: every line holding the quoted name
    //! hands it to one of `calls`, a test's scratch calls. Non-private: test_run_suites.das pins it
    let quoted = "\"{model}\""
    var named = 0
    for (line in split(text, "\n")) {
        continue if (find(line, quoted) < 0)
        named++
        var scratch = false
        for (c in calls) {
            scratch ||= find(line, c) >= 0
        }
        return false if (!scratch)
    }
    return named > 0
}

def missing_models(files : array<string>; dir : string) : array<tuple<name : string; asked : string>> {
    //! The model files the run's `files`, and the fixtures they reach, name that `dir` does not
    //! hold, each with the files that name it: the cells that would skip, said before the run. A
    //! file of the model-free suite names no stocked model, and a name a file hands only to its
    //! scratch calls (`names_only_written`) is a file the test writes: neither is missing.
    //! Non-private: test_run_suites.das pins it
    var reached <- reached_names(files)
    let free <- {for (f in free_files()); base_name(f)}
    let fixture = fread("{TESTS_DIR}/{SCRATCH_FIXTURE}")
    var asked : table<string; array<string>>
    for (name in keys(reached)) {
        continue if (key_exists(free, name))
        let text = fread("{TESTS_DIR}/{name}")
        var inscope calls <- scratch_calls(text, fixture)
        for (model in model_names(text)) {
            if (!stat(path_join(dir, model)).is_valid && !names_only_written(text, model, calls)) {
                asked[model] |> push(name)
            }
        }
    }
    var out : array<tuple<name : string; asked : string>>
    for (model, by in keys(asked), values(asked)) {
        sort(by)
        out |> push((name = model, asked = join(by, ", ")))
    }
    sort(out) <| cmp_by(_.name)
    return <- out
}

//! the model files under `dir` a child over `files` opens, by their path under it, sorted: each model the files and the
//! fixtures they reach name (`model_paths`) that `dir` holds and a cell may open - its category named by `categories`
//! or none (--category), its family by `family` or none (--family), under the size tier unless `full`
//! (DASLLAMA_PARITY_FULL=1) or its category lifts it - with the split shards and NextN head its load reads beside it.
//! Non-private: test_run_suites.das pins it
def opened_models(files : array<string>; dir, categories, family : string; full : bool) : array<string> {
    var reached <- reached_names(files)
    var out : table<string>
    let root = (dir |> ends_with("/")) ? dir : "{dir}/"
    for (name in keys(reached)) {
        for (model in model_paths(fread("{TESTS_DIR}/{name}"))) {
            let path = "{root}{model}"
            let gate = category_gate(path, categories)
            continue if (!empty(gate.why) || !empty(family_skip_reason(path, family)) || !empty(model_tier_skip_reason(path, full || gate.lifts_tier)))
            let folder = slice(model, 0, length(model) - length(base_name(model)))
            var inscope shards <- gguf_shard_paths(path, [missing_ok = true])
            for (shard in shards) {
                if (dir_name(shard) == dir_name(path)) {
                    out |> insert("{folder}{base_name(shard)}")
                }
            }
        }
    }
    var listed <- [for (p in keys(out)); p]
    sort(listed)
    return <- listed
}

//! a child's name in the run's lines: its file, or its first file and how many ride with it
def child_tag(chunk : array<string>) : string {
    let n = length(chunk)
    return n == 1 ? base_name(chunk[0]) : "{base_name(chunk[0])}+{n - 1}"
}

//! the `--list-models` text of the children `chunks` over the models directory `dir`: a `# child <n>: <tag>` line a
//! child in start order, then the model files it opens (`opened_models` under the run's --category, --family and
//! `full`) that no earlier child opened, one path a line. Non-private: test_run_suites.das pins it
def model_list_text(cfg : RunArgs; chunks : array<array<string>>; dir : string; full : bool) : string {
    var seen : table<string>
    return build_string() $(w) {
        for (chunk, at in chunks, count()) {
            w |> write("# child {at}: {child_tag(chunk)}\n")
            for (model in opened_models(chunk, dir, cfg.category, cfg.family, full)) {
                continue if (key_exists(seen, model))
                seen |> insert(model)
                w |> write("{model}\n")
            }
        }
    }
}

//! the seconds `--await-models` waits for a file of a child to land before the run stops red
let AWAIT_IDLE_SECONDS = 1800.0

//! what `--await-models` does ahead of a child: start it (and why), wait on the files still copying, or stop the run red
variant CopyGate {
    start : string
    wait : int
    stop : string
}

//! the `--await-models` decision ahead of a child with `pending` of its model files not yet local: a child with every
//! file local starts, the copier's `failed` marker ("" where none) stops the run, the copier `done` starts the child over
//! the links the list could not name, and a child none of whose files landed for `idle` seconds past `bound` stops the
//! run; else it waits. Non-private: test_run_suites.das pins it
def copy_gate(pending : int; done : bool; failed : string; idle, bound : float) : CopyGate {
    return CopyGate(start = "every model file local") if (pending == 0)
    return CopyGate(stop = "the model copy failed: {failed}") if (!empty(failed))
    return CopyGate(start = "the copier is done; {pending} file(s) it was not given read where they link") if (done)
    return CopyGate(stop = "no model file of the child landed in {int(bound)} s, {pending} still copying") if (idle >= bound)
    return CopyGate(wait = pending)
}

//! how many of the model files `models` (paths under `dir`) are not yet local: absent, or a link and not a regular file.
//! Non-private: test_run_suites.das pins it
def pending_models(dir : string; models : array<string>) : int {
    var pending = 0
    for (model in models) {
        let path = path_join(dir, model)
        let link = is_symlink_result(path)
        if (!stat(path).is_reg || !(link is value) || (link as value)) {
            pending++
        }
    }
    return pending
}

//! the copier's `failed` marker under the state folder `state`: the file it failed on, "" where it has not failed
def copy_failure(state : string) : string {
    let marker = path_join(state, "failed")
    return "" if (!fexist(marker))
    var text = ""
    fopen(marker, "rb") $(f) {
        if (f != null) {
            text = strip(fread(f))
        }
    }
    return empty(text) ? "(the marker names no file)" : text
}

//! the prepared images and save temp files under `dir` at any depth, by their path under it
def image_files(dir : string) : array<string> {
    var names : array<string>
    dir_rec(dir) $(name, is_dir) {
        if (!is_dir && (name |> ends_with(".dlim") || name |> ends_with(".tmp"))) {
            names |> push(to_generic_path(name))
        }
    }
    return <- names
}

//! `image_files` as a set: the names a run's reap leaves alone
def image_set(dir : string) : table<string> {
    return <- {for (name in image_files(dir)); name}
}

//! the images a reaping run (`--reap-images`) leaves alone: the ones there as it starts. A run that reaps nothing walks
//! no models directory - the walk is every file under it, and on a pod that is the network volume. Non-private:
//! test_run_suites.das pins it
def images_at_start(cfg : RunArgs; models : string) : table<string> {
    var kept : table<string>
    if (cfg.reap_images) {
        kept <- image_set(models)
    }
    return <- kept
}

//! every image and save temp file under `dir` that `kept` - the names there when the run started - does not hold, removed:
//! how many, and their bytes. Non-private: test_run_suites.das pins it
def reap_images(dir : string; kept : table<string>) : tuple<count : int; bytes : int64> {
    var count = 0
    var bytes = 0l
    for (name in image_files(dir)) {
        continue if (key_exists(kept, name))
        let path = path_join(dir, name)
        let size = int64(stat(path).size)
        if (remove(path)) {
            count++
            bytes += size
        }
    }
    return (count = count, bytes = bytes)
}

//! `--reap-images` after the child `tag`: what it left under `models` goes, with a REAPED line where anything did; a run
//! without the flag removes nothing. Returns the files removed. Non-private: test_run_suites.das pins it
def reap_after(cfg : RunArgs; models : string; kept : table<string>; tag : string) : int {
    return 0 if (!cfg.reap_images)
    let reaped = reap_images(models, kept)
    if (reaped.count > 0) {
        to_log(LOG_INFO, "REAPED {reaped.count} image file(s) {tag} left under {models} ({reaped.bytes >> 20l} MB)\n")
    }
    return reaped.count
}

//! a MISSING line a model file the run's `files` name that the models directory does not hold
def private log_missing_models(files : array<string>) {
    let dir = models_dir_resolved()
    for (m in missing_models(files, dir)) {
        to_log(LOG_WARNING, "MISSING {m.name}: {dir} does not hold it, and its cells skip (named by {m.asked})\n")
    }
}

//! one dastest child per file, `--nreps` times. On an area run the image suite's child gets the
//! areas' arms and every other child none; a suite run passes --arm through to every child
//! A child that ended red, by its log - true where it ends the run, no later file started and none run again: a test
//! past the wall, or a batch without every file's verdict - its root failed, or it died midway; `--batch 1` is the run
//! that finds the file. Else a lone file is the red, and a batch's verdicts are counted (`count_batch`)
def private red_child(cfg : RunArgs; tag : string; chunk : array<string>; log_path : string; rc, at : int;
                      var tally : RunTally) : bool {
    if (rc == KILL_SWITCH_EXIT_CODE) {
        let said = log_wall_line(fread(log_path))
        stop_run(cfg, tag, "WALL", empty(said) ? "a child ended with the kill switch's exit code {rc}" : said, at, tally)
        return true
    }
    let n = length(chunk)
    if (n == 1) {
        tally.failures++
        tally.failed_tags |> push(tag)
        return false
    }
    let verdicts <- log_verdicts(log_path)
    let have = length(verdicts.passed) + length(verdicts.failed)
    if (verdicts.root_failed || have != n) {
        tally.failures += length(verdicts.failed)
        tally.failed_tags |> push_from(verdicts.failed)
        let err = log_error_line(fread(log_path))
        let said = empty(err) ? "" : ": {err}"
        let got = verdicts.root_failed ? "its root failed, no file ran{said}" : "{have} of {n} files have a verdict"
        stop_run(cfg, tag, "LOST", "the batch ended with exit {rc}: {got} (read its log)", at, tally)
        return true
    }
    count_batch(tag, n, verdicts, rc, tally)
    return false
}

//! a red batch's verdicts, every file of its `n` having one: each failed file's own count, and - every file passed, the
//! exit still red - the batch is its own red
def private count_batch(tag : string; n : int; verdicts : tuple<passed : array<string>; failed : array<string>; root_failed : bool>;
                        rc : int; var tally : RunTally) {
    tally.failures += length(verdicts.failed)
    tally.failed_tags |> push_from(verdicts.failed)
    if (empty(verdicts.failed) && length(verdicts.passed) == n) {
        // every file passed and the child still exited red - a crash at its shutdown: the batch's own red
        tally.failures++
        tally.failed_tags |> push("{tag} (exit {rc} after every verdict)")
    }
}

//! true where a run already stopped under the run's stop file (`--stop-file`) - another box's, or an earlier run's on
//! the same file: `tag` is not run and the tally says why and how to clear it
def private stopped_by_marker(cfg : RunArgs; tag : string; var tally : RunTally) : bool {
    return false if (empty(cfg.stop_file) || !fexist(cfg.stop_file))
    tally.walled = "stopped before {tag}: a run stopped under this stop file - remove {cfg.stop_file}, or drop --stop-file, to run"
    tally.failures++
    tally.failed_tags |> push("{tag} (not run: the run was stopped)")
    to_log(LOG_ERROR, "WALL {tally.walled}\n")
    return true
}

//! the run ends at the child `tag`, the run's `at`th, for `why`: the tally takes it, the run's stop marker (`--stop-file`)
//! is written for the run's other boxes where `mark`, and the line to resume on is said. `kind` names the stop in the
//! lines
def private stop_run(cfg : RunArgs; tag, kind, why : string; at : int; var tally : RunTally; mark : bool = true) {
    tally.walled = why
    to_log(LOG_ERROR, "{kind} {tag}: {why} - the run stops here, no later file runs\n")
    let marked = !empty(cfg.stop_file)
    let clear = marked ? "remove {cfg.stop_file}, then " : ""
    to_log(LOG_ERROR, "RESUME once it is fixed: {clear}this command with --skip {at}\n")
    if (mark && marked && !fexist(cfg.stop_file)) {
        fwrite(cfg.stop_file, "{tag}: {why}\n")
    }
    tally.failures++
    tally.failed_tags |> push("{tag} ({kind})")
}

//! true where the test child's exe and the entry program's compile record are current for a run over `files` - built
//! here, on the run's own binary, where a file takes either (`child_verb_cmd`, `child_compile_cache`) and none current
//! stands; false, said, where a build failed
def private child_exe_ready(cfg : RunArgs; files : array<string>) : bool {
    return true if (!empty(cfg.das))   //! a dry run on another binary runs no test of this tree: nothing to build
    var spawns = false
    for (name in keys(reached_names(files))) {   //! a test spawns the child through a fixture's cell as well as by its own call
        let text = fread("{TESTS_DIR}/{name}")
        spawns ||= name != "_child_run.das" && (find(text, "child_verb_cmd(") >= 0 || find(text, "child_compile_cache(") >= 0)
    }
    return true if (!spawns)
    let t0 = ref_time_ticks()
    let why = child_exe_ensure()
    let secs = double(get_time_nsec(t0)) / 1000000000.0lf
    if (!empty(why)) {
        to_log(LOG_ERROR, "CHILD {why}\n")
        return false
    }
    to_log(LOG_INFO, "CHILD the test child's exe and the entry program's compile record are current ({secs}s)\n")
    return true
}

//! `--await-models` ahead of the child `tag` over `chunk`, the run's `at`th: waits while `copy_gate` says so, a WAIT line
//! as the wait starts and one as it ends, its seconds in the tally; true where the run stops. A copy that failed on this
//! box stops this box's run alone: the stop file is the wall's
def private await_child_models(cfg : RunArgs; models : string; chunk : array<string>; seat, tag : string; at : int;
                               var tally : RunTally) : bool {
    return false if (empty(cfg.await_models))
    var inscope opened <- opened_models(chunk, models, cfg.category, cfg.family, parity_full_mode())
    let t0 = ref_time_ticks()
    var landed_at = t0
    var last = -1
    while (true) {
        let pending = pending_models(models, opened)
        if (pending < last) {
            landed_at = ref_time_ticks()
        }
        let idle = float(double(get_time_nsec(landed_at)) / 1000000000.0lf)
        let done = fexist(path_join(cfg.await_models, "done"))
        let gate = copy_gate(pending, done, copy_failure(cfg.await_models), idle, AWAIT_IDLE_SECONDS)
        if (gate is wait) {
            if (last < 0) {
                to_log(LOG_INFO, "WAIT {seat} {tag}: {pending} model file(s) still copying\n")
            }
            last = pending
            sleep(1000u)
            continue
        }
        let secs = double(get_time_nsec(t0)) / 1000000000.0lf
        tally.waited += secs
        if (gate is stop) {
            stop_run(cfg, tag, "COPY", gate as stop, at, tally, false)
            return true
        }
        if (last >= 0 || pending > 0) {
            to_log(LOG_INFO, "WAIT {seat} {tag}: {pending == 0 ? "landed" : "started"} after {fmt(":.1f", secs)} s ({gate as start})\n")
        }
        return false
    }
    return false
}

//! a run of the media areas' files alone: each one a file audio, vision, tts or translate lists, or the image suite
def media_run(files : array<string>) : bool {
    for (f in files) {
        let name = base_name(f)
        return false if (f != "{TESTS_DIR}/{name}")
        var media = name == "test_model_image.das"
        for (a in test_file_areas(name)) {
            media ||= a == "audio" || a == "vision" || a == "tts" || a == "translate"
        }
        return false if (!media)
    }
    return !empty(files)
}

//! `envs` the children's environment (`child_envs` with the scratch folder), `media` whether `files` are a media run alone
def private run_files(cfg : RunArgs; files : array<string>; image_arms : string; area_run : bool; envs : string;
                      media : bool) : RunTally {
    let tmp = temp_dir()
    let win = get_platform_name() == "windows"
    let stamp_envs = child_envs(cfg, win, media)   //! the scratch folder is where a child writes, no part of what it proves
    if (!empty(g_env_test.scratch_dir)) {
        mkdir_rec(g_env_test.scratch_dir)
        to_log(LOG_WARNING, "SCRATCH {g_env_test.scratch_dir} (DASLLAMA_SCRATCH_DIR): the children write their temp files there\n")
    }
    let modules_line = disabled_modules_line(child_disabled_modules())
    if (!empty(modules_line)) {
        to_log(LOG_WARNING, modules_line)
    }
    let tmo = child_timeout(cfg)
    let wall = wall_seconds(cfg)
    let wall_flag = wall > 0 ? " --test-wall {wall}" : ""
    let reps = max(cfg.nreps, 1)
    let exe = empty(cfg.das) ? das_exe() : cfg.das
    mkdir(dir_name(MODULE_CACHE))
    let cache_dir = child_cache_dir(exe, empty(cfg.das))
    let cache_flag = empty(cache_dir) ? "" : " --module-cache-dir {cache_dir}"
    let disabled = child_disable_flags()   // a GPU home's run compiles only its own module
    to_log(LOG_INFO, empty(cache_dir) ? "CACHE none - the binary could not be stat'd, every child parses the engine\n" : "CACHE {cache_dir}\n")
    let models = models_dir_resolved()
    let kept_images <- images_at_start(cfg, models)
    var tally : RunTally
    for (rep in range(reps)) {
        let rep_tag = reps > 1 ? " rep {rep + 1}/{reps}" : ""
        var inscope chunks <- run_chunks(cfg, files, area_run)
        for (chunk, at in chunks, count()) {
            let n = length(chunk)
            let seat = "child {at}/{length(chunks)}"
            let tag = child_tag(chunk)
            if (at < cfg.skip) {
                to_log(LOG_INFO, "SKIP {tag}: {seat}, before --skip {cfg.skip}\n")
                continue
            }
            let log_path = path_join(tmp, "dasllama_run_{tag}_{rep}_{ref_time_ticks()}.log")
            let file_args = n == 1 ? " --test \"{chunk[0]}\"" : join([for (f in chunk); " --together \"{f}\""], "")
            return <- tally if (stopped_by_marker(cfg, tag, tally) || await_child_models(cfg, models, chunk, seat, tag, at, tally))
            let run_env = child_run_env(cfg, (area_run && tag == "test_model_image.das") ? image_arms : cfg.arm)
            let skip = skip_flags(cfg, models, stamp_envs, chunk)
            let cmd = "{envs}\"{exe}\"{disabled} -jit -module-cache {MODULE_CACHE} dastest/dastest.das --{file_args} --timeout {tmo * n}{wall_flag}{cache_flag}{skip}{run_env} > \"{log_path}\" 2>&1"
            to_log(LOG_INFO, "START {tag}{rep_tag}: {seat}: {cmd}\n")
            let t0 = ref_time_ticks()
            let rc = unsafe(popen(cmd) $(f) {
                let _drain = fread(f)   // output is redirected; drain keeps the pipe contract
            })
            let secs = double(get_time_nsec(t0)) / 1000000000.0lf
            let log_text = fread(log_path)
            let summary = child_summary(rc, log_text)
            let verdict = child_verdict(rc, summary, log_text)
            to_log(rc == 0 ? LOG_INFO : LOG_ERROR, "DONE {tag}: {seat} exit={rc} {verdict} elapsed={secs}s log={log_path}\n")
            tally.skipped += skipped_cells(summary)
            tally.cached += cached_files(log_text)
            reap_after(cfg, models, kept_images, tag)
            continue if (rc == 0)
            return <- tally if (red_child(cfg, tag, chunk, log_path, rc, at, tally))
        }
    }
    return <- tally
}

let private WASM_RUN_JS = "modules/dasLLAMA/harness/wasm_mint/run.cjs"
let private WASM_PACKAGE = "wasm_tests"

//! the last `=== a/b passed, f failed ===` line of a wasm test release's log, its FAIL lines and its count of
//! SKIP lines; "" when the release never printed a summary. Non-private: test_run_suites.das gates it
def wasm_summary(log_text : string) : tuple<line : string; failed : array<string>; skipped : int> {
    var line = ""
    var failed : array<string>
    var skipped = 0
    for (ln in non_empty_lines(log_text)) {
        if (starts_with(ln, "=== ") && ends_with(ln, " failed ===")) {
            line = clone_string(ln)
        } elif (starts_with(ln, "FAIL ")) {
            failed |> push(clone_string(slice(ln, 5)))
        } elif (starts_with(ln, "SKIP ")) {
            skipped++
        }
    }
    return <- (line = line, failed <- failed, skipped = skipped)
}

//! the three files of the package a wasm run releases, by path under its folder: the manifest, the
//! descriptor that names every test file and gives it `dastest_wasm`'s `testing_boost` (whose `[test]`
//! writes the file's `main`), and the root that runs each file's `main`. Non-private:
//! test_run_suites.das gates it
def wasm_package(files : array<string>; das_root : string) : table<string; string> {
    let stems <- [for (f in files); stem(f)]
    let manifest = (
        "options gen2\n\nrequire daslib/daspkg\n\n[export]\ndef package() \{\n"
        + "    package_name(\"{WASM_PACKAGE}\")\n"
        + "    package_description(\"dasLLAMA test files as one wasm64 release\")\n\}\n\n"
        + "[export]\ndef release() \{\n    release_main(\"main.das\")\n"
        + "    release_wasm_disable_host_gpu_modules()\n    release_emcc_arg(\"-lnodefs.js\")\n\}\n")
    let descriptor = build_string() $(var w) {
        w |> write("options gen2\nrequire daslib/fio\n\n[export]\ndef initialize(project_path : string) \{\n")
        w |> write("    register_native_path(\"dastest\", \"testing_boost\", ")
        w |> write("\"{das_root}/dastest_wasm/testing_boost.das\")\n")
        for (f, st in files, stems) {
            w |> write("    register_native_path(\"{WASM_PACKAGE}\", \"{st}\", \"{das_root}/{f}\")\n")
        }
        w |> write("\}\n")
    }
    let root = build_string() $(var w) {
        w |> write("options gen2\noptions stack = 524288\noptions _dasllama_internal = true\n")
        w |> write("options persistent_heap\n\n")
        for (st in stems) {
            w |> write("require {WASM_PACKAGE}/{st}\n")
        }
        w |> write("\n[export]\ndef main : int \{\n    var rc = 0\n")
        for (st in stems) {
            w |> write("    rc = {st}::main()\n")
        }
        w |> write("    return rc\n\}\n")
    }
    return <- {
        ".das_package" => manifest,
        "modules/{WASM_PACKAGE}/.das_module" => descriptor,
        "main.das" => root
    }
}

//! one logged step of the wasm suite; its exit code
def private wasm_step(tag, cmd, log_path : string) : int {
    to_log(LOG_INFO, "START {tag}: {cmd}\n")
    let t0 = ref_time_ticks()
    let rc = unsafe(popen("{cmd} > \"{log_path}\" 2>&1") $(f) {
        let _drain = fread(f)   // output is redirected; drain keeps the pipe contract
    })
    let secs = double(get_time_nsec(t0)) / 1000000000.0lf
    to_log(rc == 0 ? LOG_INFO : LOG_ERROR, "DONE {tag}: exit={rc} elapsed={secs}s log={log_path}\n")
    return rc
}

//! --suite wasm: `files` built as one wasm64 release by `daspkg release wasm` and run under node; the
//! package and the release tree are removed, the two logs stay
def private run_wasm(cfg : RunArgs; files : array<string>) : RunTally {
    let exe = empty(cfg.das) ? das_exe() : cfg.das
    let stamp = ref_time_ticks()
    let work = path_join(temp_dir(), "dasllama_wasm_tests_{stamp}")
    let pkg = path_join(work, "package")
    let out = path_join(work, "out")
    let build_log = path_join(temp_dir(), "dasllama_wasm_release_{stamp}.log")
    let run_log = path_join(temp_dir(), "dasllama_wasm_run_{stamp}.log")
    mkdir_rec(path_join(pkg, "modules/{WASM_PACKAGE}"))
    mkdir_rec(path_join(out, "io"))
    let package <- wasm_package(files, replace(get_das_root(), "\\", "/"))
    for (name, text in keys(package), values(package)) {
        fwrite(path_join(pkg, name), text)
    }
    var tally : RunTally
    let build = "\"{exe}\" utils/daspkg/main.das -- release wasm --root \"{pkg}\" --out \"{out}\""
    if (wasm_step("wasm release", build, build_log) != 0) {
        to_log(LOG_ERROR, "the wasm release did not build: it takes emcc on PATH and the wasm64 archives (`daspkg build --wasm`)\n")
        tally.failures = 1
        tally.failed_tags |> push("the wasm release")
    } else {
        let app = path_join(out, "{WASM_PACKAGE}/{WASM_PACKAGE}.js")
        let rc = wasm_step("wasm tests", "node {WASM_RUN_JS} \"{app}\" \"{path_join(out, "io")}\"", run_log)
        var sum <- wasm_summary(fread(run_log))
        if (empty(sum.failed) && (rc != 0 || empty(sum.line))) {
            sum.failed |> push(empty(sum.line) ? "no summary line - the run died early (read the log)" : "exit {rc}")
        }
        to_log(empty(sum.failed) ? LOG_INFO : LOG_ERROR, "DONE wasm tests: {sum.line}, {sum.skipped} skipped\n")
        tally.failures = length(sum.failed)
        tally.skipped = sum.skipped
        tally.failed_tags <- sum.failed
    }
    rmdir_rec(work)
    return <- tally
}

//! the per-box runtime file's program (`dasllama_box_runtime_file`): it races the Metal crowns where the file is not
//! valid for this tree's kernels, and says where the file is
let BOX_RUNTIME_ROOT = "modules/dasLLAMA/tests/_box_runtime_root.das"
//! that program's compile record
let private BOX_RUNTIME_CACHE = ".cache/daslang/module_cache/box_runtime.dascache"

//! Whether a run's children may race the Metal crowns at their first Metal load, inside a test's wall: a build with the
//! Metal module, children reading no tune manifest (`manifest`, the caller's or the box's), a kernel policy that keeps
//! the crowns (not `--no-tune`'s reference, nor `fallback`). Non-private: test_run_suites.das pins each answer
def box_race_possible(metal_build, no_tune : bool; manifest, policy : string) : bool
    => metal_build && !no_tune && empty(manifest) && tune_policy_keeps_crowns(policy)

//! whether the runner makes the per-box runtime file ready ahead of its first child: a run on its
//! own binary (no `--das` stand-in) whose children may race the crowns (`box_race_possible`).
//! Non-private: test_run_suites.das pins it
def box_runtime_owed(das_standin : string; metal_build, no_tune : bool; manifest, policy : string) : bool
    => empty(das_standin) && box_race_possible(metal_build, no_tune, manifest, policy)

//! the BOXRACE line the per-box runtime program's exit `rc` and output `out` make, `secs` its
//! seconds: `warn` where it failed - a nonzero exit, or no file named - the race's line where it
//! raced, no line where a valid file held. Non-private: test_run_suites.das pins each line
def box_race_line(rc : int; out : string; secs : double) : tuple<warn : bool; text : string> {
    if (rc != 0 || find(out, "BOX_FILE=[]") >= 0) {
        return (warn = true,
            text = "BOXRACE failed (exit {rc}): the children race themselves\n{out}\n")
    } elif (find(out, "metal crowns raced") >= 0) {
        return (warn = false,
            text = "BOXRACE Metal crowns raced for the box file, no wall ({secs}s)\n")
    }
    return (warn = false, text = "")
}

//! the line that announces the modules every child of the run leaves out (`names`,
//! DASLLAMA_DISABLE_MODULES); none for an empty list. Non-private: test_run_suites.das pins it
def disabled_modules_line(names : string) : string {
    return "" if (empty(names))
    return "MODULES {names} (DASLLAMA_DISABLE_MODULES): every child leaves these modules out\n"
}

//! the per-box runtime file made ready ahead of the first child, outside any wall, where the children may race the Metal
//! crowns (`box_runtime_owed`): its program (`BOX_RUNTIME_ROOT`) under the children's environment `envs` races only where no
//! file valid for this tree's kernels exists, logs `box_race_line`'s line, and a failure of its own stops no child
def private box_runtime_ready(cfg : RunArgs; envs : string) {
    let metal = typeinfo builtin_module_exists(das_metal)
    return if (!box_runtime_owed(cfg.das, metal, cfg.no_tune, children_manifest(), g_env_tune.tune_policy))
    let tail = "-jit -module-cache {BOX_RUNTIME_CACHE} {BOX_RUNTIME_ROOT} 2>&1"
    let cmd = "{envs}\"{das_exe()}\"{child_disable_flags()} {tail}"
    let t0 = ref_time_ticks()
    var out = ""
    let rc = unsafe(popen(cmd) $(f) {
        out = fread(f)
    })
    let line = box_race_line(rc, out, double(get_time_nsec(t0)) / 1000000000.0lf)
    if (!empty(line.text)) {
        to_log(line.warn ? LOG_WARNING : LOG_INFO, line.text)
    }
}

//! the run's executor: the wasm suite's one release, else the dastest children
def private run_plan(cfg : RunArgs; files : array<string>; image_arms : string; area_run : bool) : RunTally {
    return <- run_wasm(cfg, files) if (cfg.suite == "wasm" && !area_run)
    if (!child_exe_ready(cfg, files)) {
        var tally : RunTally
        tally.failures = 1
        tally.failed_tags |> push("the test child's exe")
        return <- tally
    }
    let media = media_run(files)
    let envs = child_envs(cfg, get_platform_name() == "windows", media, g_env_test.scratch_dir)
    box_runtime_ready(cfg, envs)
    return <- run_files(cfg, files, image_arms, area_run, envs, media)
}

def operator +=(var a : RunTally; b : RunTally) {
    a.failures += b.failures
    a.skipped += b.skipped
    a.cached += b.cached
    a.failed_tags |> push_from(b.failed_tags)
    a.waited += b.waited
    if (empty(a.walled)) {
        a.walled = b.walled
    }
}

//! `per` files a child, in order; alone: an area run's image file, a cant_/failed_/invalid_ file (a root requiring it fails
//! whole) and a tier contract file (`TIER_CONTRACT_FILES`). A file that runs alone leaves the batch around it whole: its
//! neighbours ride one child, not one each
def child_chunks(files : array<string>; per : int; area_run : bool) : array<array<string>> {
    var chunks : array<array<string>>
    chunks |> reserve(length(files) / max(per, 1) + 1)
    var cur : array<string>
    for (f in files) {
        let b = base_name(f)
        if ((area_run && b == "test_model_image.das") || is_expected_failure_test(f) || has_value(TIER_CONTRACT_FILES, b)) {
            chunks |> emplace([f])
            continue
        }
        if (empty(cur)) {
            cur |> reserve(per)
        }
        cur |> push(f)
        if (length(cur) >= per) {
            chunks |> emplace(cur)
        }
    }
    if (!empty(cur)) {
        chunks |> emplace(cur)
    }
    return <- chunks
}

//! a category run's children: the files of each process suite (`PROCESS_SUITES`) one child a suite, as that suite runs
//! them, and the other files `per` a child; at `per` 1 - the rerun of a batch whose root failed - a file a child
def category_chunks(files : array<string>; per : int) : array<array<string>> {
    return <- child_chunks(files, 1, false) if (per == 1)
    var chunks : array<array<string>>
    var taken : table<string>
    for (suite in PROCESS_SUITES) {
        var inscope members <- suite_files(suite)
        var own <- [for (f in files); f; where !key_exists(taken, f) && has_value(members, f)]
        for (f in own) {
            taken |> insert(f)
        }
        if (!empty(own)) {
            chunks |> emplace(own)
        }
    }
    var rest <- child_chunks([for (f in files); f; where !key_exists(taken, f)], per, false)
    chunks |> push_clone_from(rest)
    return <- chunks
}

//! the run's children: a category run's by suite, any other run's `batch_files` a child
def run_chunks(cfg : RunArgs; files : array<string>; area_run : bool) : array<array<string>> {
    return <- category_chunks(files, batch_files(cfg)) if (!empty(cfg.category))
    return <- child_chunks(files, batch_files(cfg), area_run)
}

//! what makes the run less than the plain run of its scope: a wall given by hand, the model-free suite's own wall lifted
//! by the full suite (`full`), a resume
def private scope_relaxed(cfg : RunArgs) : string => scope_relaxed_in(cfg, parity_full_mode())

//! `scope_relaxed` where `full` says whether the run is the full suite. Non-private: test_run_suites.das pins it
def scope_relaxed_in(cfg : RunArgs; full : bool) : string {
    let lifted = cfg.test_wall < 0 && cfg.suite == "model-free" && full
    return ((cfg.test_wall < 0 ? "" : (cfg.test_wall == 0 ? " no test wall" : " test wall {cfg.test_wall} s"))
        + (lifted ? " full suite (DASLLAMA_PARITY_FULL=1): no test wall" : "")
        + (cfg.skip > 0 ? " resumed at child {cfg.skip}" : ""))
}

//! the run tells the next step itself: what ran, what a missing model skipped, what is still owed
def private print_report(cfg : RunArgs; areas : array<string>; n_files, n_excluded : int; tally : RunTally; core : bool) {
    let picked = !empty(cfg.category) ? "category {cfg.category}" : "suite {cfg.suite}"
    let scope = ((cfg.changed ? "changed vs {cfg.base}" : (!empty(areas) ? "areas {join(areas, ",")}" : picked))
        + (empty(cfg.only) ? "" : " only {cfg.only}") + scope_relaxed(cfg))
    let failed_list = empty(tally.failed_tags) ? "" : " ({join(tally.failed_tags, ", ")})"
    let excluded = n_excluded == 0 ? "" : ", {n_excluded} file(s) excluded"
    let waited = empty(cfg.await_models) ? "" : ", {fmt(":.0f", tally.waited)} s waited on the model copy"
    if (!empty(tally.walled)) {
        to_log(LOG_ERROR, "STOPPED, the files after it not run: {tally.walled}\n")
    }
    to_log(tally.failures == 0 ? LOG_INFO : LOG_ERROR, "REPORT {scope}: {n_files} file(s),{cached_report(tally.cached)} {tally.failures} failed{failed_list}, {tally.skipped} cell(s) skipped (a filtered arm, an absent device, or a missing model or fixture - each skips loudly){excluded}{waited}\n")
    if (cfg.changed) {
        if (core) {
            to_log(LOG_INFO, "NEXT the engine core changed: the whole stocked set before the PR - run.das -- --suite stocked over the fleet, a pod a GPU home (harness/pod_fleet.das)\n")
            to_log(LOG_INFO, "NEXT the model suites (decode | prefill | matrix) take --arm scoped to the change: run.das -- --arm <arms> --suite <name>\n")
        }
    } elif (!empty(areas)) {
        if (find_index(areas, "llm") >= 0) {
            to_log(LOG_INFO, "NEXT the model suites (decode | prefill | matrix) take --arm scoped to the change: run.das -- --arm <arms> --suite <name>\n")
        }
        to_log(LOG_INFO, "NEXT before the PR: run.das -- --suite model-free, then this --changed run on a box with models\n")
    } elif (cfg.suite == "model-free") {
        to_log(LOG_INFO, "NEXT before the PR: run.das -- --changed on a box with models (the files the changed paths reach)\n")
    }
}

//! what a run covers, read off its flags: the areas an area or changed run names, the files, the image child's arms,
//! and whether a changed run reached the engine core. `status` is the exit code of a run its flags end, -1 otherwise
struct RunScope {
    status : int = -1
    areas : array<string>
    files : array<string>
    image_arms : string
    core : bool
}

def private run_scope(cfg : RunArgs) : RunScope {
    var scope = RunScope()
    let by_category = !empty(cfg.category)
    if (check_filter_args(cfg) != 0 || (by_category && check_category_args(cfg) != 0)) {
        scope.status = 2
        return <- scope
    }
    var sel <- select_areas(cfg)
    scope.status = sel.status
    return <- scope if (scope.status >= 0)
    scope.areas <- sel.areas
    scope.status = (empty(scope.areas) && !cfg.changed && !by_category) ? check_suite_args(cfg) : 0
    return <- scope if (scope.status != 0)
    scope.status = -1
    if (by_category) {
        scope.files <- category_files(cfg.category)
    } elif (cfg.changed) {
        var changed <- changed_plan(cfg)
        scope.status = changed.status
        scope.files <- changed.files
        scope.image_arms = changed.image_arms
        scope.core = changed.core
    } else {
        var plan <- plan_files(cfg, scope.areas)
        scope.files <- plan.files
        scope.image_arms = plan.image_arms
    }
    return <- scope
}

[export]
def main() : int {
    //! the runner's lines (AREAS, START, CHANGED, REPORT, NEXT) are its output: the to_log floor is info
    //! unless the environment set one, and the children it spawns inherit it
    if (empty(g_env_core.log_level)) {
        set_env_variable("DAS_LOG_LEVEL", "info")
    }
    var r <- parse_args(type<RunArgs>)
    if (r |> is_err) {
        to_log(LOG_ERROR, "error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<RunArgs>), "modules/dasLLAMA/tests/run.das")
        return 2
    }
    let cfg <- r |> move_unwrap
    if (cfg.list_models) {   //! the list is the output: the run's own lines stay out of it
        set_env_variable("DAS_LOG_LEVEL", "error")
    }
    if (cfg.help) {
        print_help(get_command_info(type<RunArgs>), "modules/dasLLAMA/tests/run.das")
        return 0
    }
    var plan <- run_scope(cfg)
    if (plan.status >= 0) {
        return plan.status
    }
    let forced = forced_tune_file(cfg)
    if (!empty(forced)) {   //! the test child's build reads the runner's own environment
        set_env_variable("DAS_TUNE_FORCE", forced)
    }
    let areas <- plan.areas
    let core = plan.core
    if (empty(plan.files)) {
        to_log(LOG_ERROR, "unknown --suite '{cfg.suite}' (decode | mtp | prefill | matrix | kernels | image | image-vulkan | coverage | model-free | stocked | wasm | all)\n")
        return 2
    }
    if (cfg.no_tune && !empty(plan.image_arms) && (!empty(areas) || cfg.changed)) {   //! an area's image arms ride the image rail --no-tune turns off
        to_log(LOG_ERROR, "--no-tune sets DASLLAMA_IMAGE=0 in every child, and the run owns the image arms {plan.image_arms} - drop --no-tune, or run those arms through --suite image --arm\n")
        return 2
    }
    var split <- run_exclusion(plan.files, cfg.exclude, child_disabled_modules())
    for (line in split.excluded) {
        to_log(LOG_INFO, "{line}\n")
    }
    if (empty(split.kept)) {
        to_log(LOG_ERROR, "{split.refusal}\n")
        return 2
    }
    let only <- only_files(split.kept, cfg.only)
    if (!empty(cfg.only)) {
        for (file in only) {
            to_log(LOG_INFO, "ONLY {base_name(file)} (--only {cfg.only})\n")
        }
        if (empty(only)) {
            to_log(LOG_ERROR, "--only '{cfg.only}' keeps no file of the run's {length(split.kept)}\n")
            return 2
        }
    }
    if (cfg.list_models) {
        var inscope chunks <- run_chunks(cfg, only, !empty(areas) || cfg.changed)
        fwrite(fstdout(), model_list_text(cfg, chunks, models_dir_resolved(), parity_full_mode()))
        return 0
    }
    log_missing_models(only)
    let tally <- run_plan(cfg, only, plan.image_arms, !empty(areas) || cfg.changed)
    print_report(cfg, areas, length(only), length(split.excluded), tally, core)
    return tally.failures
}
