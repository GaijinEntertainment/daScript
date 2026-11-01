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
require daslib/fio
require daslib/strings_boost
require strings
require math
require dasllama/dasllama_env
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
    @clarg_doc = "Suite: decode | mtp | prefill | matrix | kernels | image | image-vulkan | coverage | model-free | stocked | all (default all; all excludes image-vulkan, model-free and stocked)"
    suite : string = "all"

    @clarg_short = "x"
    @clarg_doc = "Exclude — comma-separated substrings; a suite file whose name contains one is dropped, each named on an EXCLUDED line (e.g. --suite stocked --exclude test_ple_modes)"
    exclude : string

    @clarg_doc = "Shard i/n — this box runs every n-th file of the run's file list from the i-th (1-based), so n boxes on the same command with --shard 1/n .. n/n cover the run once; the REPORT line names the shard and the NEXT line the shards still owed"
    shard : string
    @clarg_doc = "Only — comma-separated substrings; a file of the run whose name contains none is dropped, each kept file named on an ONLY line and the REPORT line carrying the tokens, so a narrowed run cannot read as whole (e.g. --changed --only cuda,segments under DASLLAMA_CUDA=1: the CUDA home's files of the run)"
    only : string

    @clarg_doc = "Area — comma-separated: audio | vision | tts | translate | llm | infra; runs every model-free and stocked file of the area plus the image suite's arms of the area (no --suite, no --arm)"
    area : string

    @clarg_doc = "Run what the changed files reach: git diff against --base plus untracked files under modules/dasLLAMA/, each path mapped to its areas (printed), then the areas' tests"
    changed : bool

    @clarg_doc = "Diff base for --changed (default origin/master)"
    base : string = "origin/master"

    @clarg_doc = "Family filter — comma-separated family tokens (the full list lives in tests/CLAUDE.md, EXACT match); only blocks of these model families run (model-free blocks always run); sets DASLLAMA_TEST_FAMILY"
    family : string

    @clarg_short = "n"
    @clarg_doc = "Repeat count — explicit only (default 1, never best-of-N)"
    nreps : int = 1
    @clarg_name = "no-tune"
    @clarg_doc = "Short-circuit the [tune] framework in every child (DAS_TUNE_POLICY=reference): the reference bodies run — the portable tier's arm"
    no_tune : bool

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
                    "modules/dasLLAMA/tests/test_mtp_gemma_drafter.das" ]
    } elif (name == "prefill") {
        return <- [ "modules/dasLLAMA/tests/test_metal_prefill_parity.das" ]
    } elif (name == "matrix") {
        return <- [ "modules/dasLLAMA/tests/test_metal_support_matrix.das" ]
    } elif (name == "kernels") {
        return <- [ "modules/dasLLAMA/tests/test_metal_prefill_kernels.das",
                    "modules/dasLLAMA/tests/test_metal_decode_kernels.das",
                    "modules/dasLLAMA/tests/test_metal_rope_kernels.das",
                    "modules/dasLLAMA/tests/test_metal_gemv_kernels.das",
                    "modules/dasLLAMA/tests/test_metal_misc_kernels.das",
                    "modules/dasLLAMA/tests/test_metal_attn_kernels.das",
                    "modules/dasLLAMA/tests/test_metal_gemm_kernels.das" ]
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
                    "modules/dasLLAMA/tests/test_accel_backend.das",
                    "modules/dasLLAMA/tests/test_affinity_predicates.das",
                    "modules/dasLLAMA/tests/test_arch_registry.das",
                    "modules/dasLLAMA/tests/test_asr_verbs.das",
                    "modules/dasLLAMA/tests/test_batch_grid.das",
                    "modules/dasLLAMA/tests/test_bench_records_schema.das",
                    "modules/dasLLAMA/tests/test_box_ident.das",
                    "modules/dasLLAMA/tests/test_box_profile.das",
                    "modules/dasLLAMA/tests/test_compares.das",
                    "modules/dasLLAMA/tests/test_mtp_snapshot.das",
                    "modules/dasLLAMA/tests/test_mtp_sampled_walk.das",
                    "modules/dasLLAMA/tests/test_mtp_accept_walks.das",
                    "modules/dasLLAMA/tests/test_sampling_landed.das",
                    "modules/dasLLAMA/tests/test_dasllama_core.das",
                    "modules/dasLLAMA/tests/test_dasllama_lib.das",
                    "modules/dasLLAMA/tests/test_dasllama_lint_contracts.das",
                    "modules/dasLLAMA/tests/test_metal_float_a_gate.das",
                    "modules/dasLLAMA/tests/test_dasllama_lint_escape.das",
                    "modules/dasLLAMA/tests/test_dispatch_shaping.das",
                    "modules/dasLLAMA/tests/test_driver_knob_getters.das",
                    "modules/dasLLAMA/tests/test_dlim_wipe.das",
                    "modules/dasLLAMA/tests/test_env_registry.das",
                    "modules/dasLLAMA/tests/test_exchange_schema.das",
                    "modules/dasLLAMA/tests/test_f32_batch_tile_perms.das",
                    "modules/dasLLAMA/tests/test_facade_docs.das",
                    "modules/dasLLAMA/tests/test_fat_hook_umbrella.das",
                    "modules/dasLLAMA/tests/test_fat_start.das",
                    "modules/dasLLAMA/tests/test_gen_records_args.das",
                    "modules/dasLLAMA/tests/test_from_template.das",
                    "modules/dasLLAMA/tests/test_gguf_quant.das",
                    "modules/dasLLAMA/tests/test_gguf_shards.das",
                    "modules/dasLLAMA/tests/test_gpu_serving_declines.das",
                    "modules/dasLLAMA/tests/test_gpu_home.das",
                    "modules/dasLLAMA/tests/test_gpu_shared_units.das",
                    "modules/dasLLAMA/tests/test_gpu_tier.das",
                    "modules/dasLLAMA/tests/test_groupn.das",
                    "modules/dasLLAMA/tests/test_kernel_backend.das",
                    "modules/dasLLAMA/tests/test_kgroup_repack.das",
                    "modules/dasLLAMA/tests/test_kqformat.das",
                    "modules/dasLLAMA/tests/test_math_activations.das",
                    "modules/dasLLAMA/tests/test_math_elementwise.das",
                    "modules/dasLLAMA/tests/test_matmul.das",
                    "modules/dasLLAMA/tests/test_matmul_batch.das",
                    "modules/dasLLAMA/tests/test_metal_gemm.das",
                    "modules/dasLLAMA/tests/test_metal_role_infer.das",
                    "modules/dasLLAMA/tests/test_model_specs.das",
                    "modules/dasLLAMA/tests/test_mxfp4.das",
                    "modules/dasLLAMA/tests/test_par_indexed.das",
                    "modules/dasLLAMA/tests/test_parity_cache.das",
                    "modules/dasLLAMA/tests/test_jobque_tripwire.das",
                    "modules/dasLLAMA/tests/test_plane.das",
                    "modules/dasLLAMA/tests/test_ple_check.das",
                    "modules/dasLLAMA/tests/test_pod_fleet.das",
                    "modules/dasLLAMA/tests/test_prefill_cpu_kernels.das",
                    "modules/dasLLAMA/tests/test_program_roots.das",
                    "modules/dasLLAMA/tests/test_q8q8_family.das",
                    "modules/dasLLAMA/tests/test_quant.das",
                    "modules/dasLLAMA/tests/test_qwen4exp.das",
                    "modules/dasLLAMA/tests/test_repack.das",
                    "modules/dasLLAMA/tests/test_repack_lane_context.das",
                    "modules/dasLLAMA/tests/test_review_howto.das",
                    "modules/dasLLAMA/tests/test_rmsnorm.das",
                    "modules/dasLLAMA/tests/test_rnnt.das",
                    "modules/dasLLAMA/tests/test_rope.das",
                    "modules/dasLLAMA/tests/test_rope_apply.das",
                    "modules/dasLLAMA/tests/test_run_suites.das",
                    "modules/dasLLAMA/tests/test_run_summary.das",
                    "modules/dasLLAMA/tests/test_silu.das",
                    "modules/dasLLAMA/tests/test_site_records.das",
                    "modules/dasLLAMA/tests/test_sizing_helpers.das",
                    "modules/dasLLAMA/tests/test_softmax.das",
                    "modules/dasLLAMA/tests/test_think_split.das",
                    "modules/dasLLAMA/tests/test_tok_seed.das",
                    "modules/dasLLAMA/tests/test_tts_textnorm.das",
                    "modules/dasLLAMA/tests/test_tts_blocks.das",
                    "modules/dasLLAMA/tests/test_tts_voice_prompts.das",
                    "modules/dasLLAMA/tests/test_tool_formats.das",
                    "modules/dasLLAMA/tests/test_tower_asr_kernels.das",
                    "modules/dasLLAMA/tests/test_tower_helpers.das",
                    "modules/dasLLAMA/tests/test_tune_interrupt.das",
                    "modules/dasLLAMA/tests/test_served_bench.das",
                    "modules/dasLLAMA/tests/test_vk_spv_diff.das",
                    "modules/dasLLAMA/tests/test_unicode.das",
                    "modules/dasLLAMA/tests/test_vision.das",
                    "modules/dasLLAMA/tests/test_vulkan_dec_tail.das",
                    "modules/dasLLAMA/tests/test_vulkan_kv_codec_kernels.das",
                    "modules/dasLLAMA/tests/test_vulkan_tower_kernels.das",
                    "modules/dasLLAMA/tests/test_vulkan_tts_conv_kernels.das",
                    "modules/dasLLAMA/tests/test_vulkan_tts_kernels.das",
                    "modules/dasLLAMA/tests/test_vulkan_tts_pocket_kernels.das",
                    "modules/dasLLAMA/tests/test_vulkan_tts_source_kernels.das",
                    "modules/dasLLAMA/tests/test_vulkan_kernels.das",
                    "modules/dasLLAMA/tests/test_vulkan_moe_cm2.das",
                    "modules/dasLLAMA/tests/test_vulkan_host_twin.das",
                    "modules/dasLLAMA/tests/test_vulkan_tier.das",
                    "modules/dasLLAMA/tests/test_vulkan_shared_device.das",
                    "modules/dasLLAMA/tests/test_cuda_kernels.das" ]
    } elif (name == "stocked") {
        return <- [
                    "modules/dasLLAMA/tests/test_attn_span.das",
                    "modules/dasLLAMA/tests/test_audio.das",
                    "modules/dasLLAMA/tests/test_audio_embedder.das",
                    "modules/dasLLAMA/tests/test_batch_decode.das",
                    "modules/dasLLAMA/tests/test_bench_rows.das",
                    "modules/dasLLAMA/tests/test_chat.das",
                    "modules/dasLLAMA/tests/test_deltanet.das",
                    "modules/dasLLAMA/tests/test_embeddings.das",
                    "modules/dasLLAMA/tests/test_exe_smoke.das",
                    "modules/dasLLAMA/tests/test_facade.das",
                    "modules/dasLLAMA/tests/test_flash.das",
                    "modules/dasLLAMA/tests/test_forward.das",
                    "modules/dasLLAMA/tests/test_fused_decode.das",
                    "modules/dasLLAMA/tests/test_gemma3v.das",
                    "modules/dasLLAMA/tests/test_gemma4uv.das",
                    "modules/dasLLAMA/tests/test_gemma4v.das",
                    "modules/dasLLAMA/tests/test_gigaam.das",
                    "modules/dasLLAMA/tests/test_nmt.das",
                    "modules/dasLLAMA/tests/test_gpu_model_swap.das",
                    "modules/dasLLAMA/tests/test_gpu_moe_shexp.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_gemma2.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_gemma3_1b.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_gemma3_4b.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_gemma4_12b_k.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_gemma4_12b_q8.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_gemma4_26b.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_gemma4_26b_k.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_gemma4_26b_mx4.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_gemma4_26b_host.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_gemma4_31b.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_gemma4_e2b.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_gemma4_e4b.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_gptoss.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_hc.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_hc_micro.das",
                    "modules/dasLLAMA/tests/test_parity_dump.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_continuation.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_long_window.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_hybrid.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_llama.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_cuda_llama.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_cuda_codec.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_cuda_batch.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_cuda_qwen3.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_cuda_phi3.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_cuda_qwen35.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_cuda_mtp.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_cuda_moe_host.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_cuda_moe.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_cuda_gemma4.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_cuda_gemma.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_cuda_qwen2.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_segments.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_moe.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_moe_host_pinned.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_moe_host_rows.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_moe_formats.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_format_census.das",
                    "modules/dasLLAMA/tests/test_gpu_perop_kv_codec.das",
                    "modules/dasLLAMA/tests/test_gpu_perop_kv_codec_iq.das",
                    "modules/dasLLAMA/tests/test_gpu_perop_kv_codec_k.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_codec_llama_iq.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_codec_llama_k.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_qwen2.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_regions.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_regions_e2b.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_regions_gemma2.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_regions_gemma3.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_regions_gemma4.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_regions_gptoss.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_regions_qwen3moe.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_regions_qwen35moe.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_regions_qwen2moe.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_regions_gemma4moe.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_regions_hybrid.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_regions_hybrid_k.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_regions_hybrid_iq3s.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_regions_hybrid_q3kxl.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_regions_llama_k.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_regions_llama3b_ctx32k.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_regions_gemma3_4b_ctx128k.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_regions_gemma4_e2b_ctx128k.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_regions_phi3.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_regions_qwen3.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_regions_q8kv.das",
                    "modules/dasLLAMA/tests/test_gpu_resident_regions_tq4kv.das",
                    "modules/dasLLAMA/tests/test_gpu_slot_swap.das",
                    "modules/dasLLAMA/tests/test_kquant.das",
                    "modules/dasLLAMA/tests/test_kv_codec.das",
                    "modules/dasLLAMA/tests/test_kv_paged.das",
                    "modules/dasLLAMA/tests/test_kv_prefix.das",
                    "modules/dasLLAMA/tests/test_metal_batched_row.das",
                    "modules/dasLLAMA/tests/test_metal_format_census.das",
                    "modules/dasLLAMA/tests/test_mtp.das",
                    "modules/dasLLAMA/tests/test_mtp_gemma_drafter.das",
                    "modules/dasLLAMA/tests/test_parity.das",
                    "modules/dasLLAMA/tests/test_interpreter.das",
                    "modules/dasLLAMA/tests/test_parity_pregate.das",
                    "modules/dasLLAMA/tests/test_parrot.das",
                    "modules/dasLLAMA/tests/test_ple_modes.das",
                    "modules/dasLLAMA/tests/test_prefill.das",
                    "modules/dasLLAMA/tests/test_qwen3v.das",
                    "modules/dasLLAMA/tests/test_qwen25v.das",
                    "modules/dasLLAMA/tests/test_sampling.das",
                    "modules/dasLLAMA/tests/test_constraint.das",
                    "modules/dasLLAMA/tests/test_scheduler.das",
                    "modules/dasLLAMA/tests/test_storyteller_restart.das",
                    "modules/dasLLAMA/tests/test_storywish.das",
                    "modules/dasLLAMA/tests/test_tokenizer.das",
                    "modules/dasLLAMA/tests/test_tts_postag.das",
                    "modules/dasLLAMA/tests/test_tts_g2p.das",
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
    } elif (name == "all") {
        return <- [ "modules/dasLLAMA/tests/test_metal_decode_parity.das",
                    "modules/dasLLAMA/tests/test_metal_mtp_parity.das",
                    "modules/dasLLAMA/tests/test_metal_prefill_parity.das",
                    "modules/dasLLAMA/tests/test_metal_support_matrix.das",
                    "modules/dasLLAMA/tests/test_metal_prefill_kernels.das",
                    "modules/dasLLAMA/tests/test_metal_decode_kernels.das",
                    "modules/dasLLAMA/tests/test_metal_rope_kernels.das",
                    "modules/dasLLAMA/tests/test_metal_gemv_kernels.das",
                    "modules/dasLLAMA/tests/test_metal_misc_kernels.das",
                    "modules/dasLLAMA/tests/test_metal_attn_kernels.das",
                    "modules/dasLLAMA/tests/test_metal_gemm_kernels.das",
                    "modules/dasLLAMA/tests/test_model_image.das" ]
    }
    var none : array<string>
    return <- none
}

let AREAS <- ["audio", "vision", "tts", "translate", "llm", "infra"]
let TESTS_DIR = "modules/dasLLAMA/tests"

//! the suite-less test files by AREA - what someone working on audio, vision, TTS, the LLM engine or the
//! tooling runs after an edit. A file serving two areas sits in both (a shared tower helper's test).
//! Non-private: test_run_suites.das holds the union to the model-free + stocked census
def area_tests(area : string) : array<string> {
    if (area == "audio") {
        return <- [ "test_asr_verbs.das", "test_audio.das", "test_audio_embedder.das", "test_dasllama_lint_contracts.das", "test_gigaam.das",
                    "test_interpreter.das", "test_parrot.das", "test_rnnt.das", "test_tower_asr_kernels.das", "test_tower_helpers.das", "test_vad.das",
                    "test_vulkan_tower_kernels.das", "test_whisper.das" ]
    } elif (area == "vision") {
        return <- [ "test_attn_span.das", "test_gemma3v.das", "test_gemma4uv.das", "test_gemma4v.das", "test_qwen25v.das",
                    "test_qwen3v.das", "test_tower_helpers.das", "test_vision.das", "test_vision_chat.das",
                    "test_vision_embedder.das", "test_vulkan_tower_kernels.das" ]
    } elif (area == "tts") {
        return <- [ "test_f32_batch_tile_perms.das", "test_interpreter.das", "test_parrot.das", "test_storyteller_restart.das", "test_storywish.das", "test_tts_blocks.das", "test_tts_facade.das",
                    "test_tts_g2p.das", "test_tts_kitten.das", "test_tts_kokoro.das", "test_tts_pocket.das",
                    "test_tts_postag.das", "test_tts_textnorm.das", "test_tts_voice_prompts.das", "test_vulkan_tts_conv_kernels.das",
                    "test_vulkan_tts_kernels.das", "test_vulkan_tts_pocket_kernels.das", "test_vulkan_tts_source_kernels.das" ]
    } elif (area == "translate") {
        return <- [ "test_interpreter.das", "test_nmt.das" ]
    } elif (area == "infra") {
        return <- [ "failed_dasllama_lint_require.das", "failed_dasllama_lint_sidedoor.das", "test_bench_records_schema.das", "test_bench_rows.das",
                    "test_box_ident.das", "test_box_profile.das", "test_compares.das", "test_dasllama_lint_contracts.das",
                    "test_dasllama_core.das", "test_dasllama_lib.das",
                    "test_dasllama_lint_escape.das", "test_dlim_wipe.das",
                    "test_env_registry.das",
                    "test_exchange_schema.das", "test_exe_smoke.das", "test_facade_docs.das", "test_fat_hook_umbrella.das", "test_fat_start.das", "test_gen_records_args.das",
                    "test_model_specs.das", "test_pod_fleet.das", "test_program_roots.das", "test_review_howto.das", "test_run_suites.das", "test_run_summary.das", "test_served_bench.das",
                    "test_site_records.das", "test_sizing_helpers.das", "test_tok_seed.das", "test_tune_interrupt.das", "test_vk_spv_diff.das" ]
    } elif (area == "llm") {
        return <- [ "test_accel_backend.das", "test_affinity_predicates.das", "test_arch_registry.das", "test_batch_decode.das", "test_bench_rows.das",
                    "test_batch_grid.das", "test_chat.das", "test_deltanet.das", "test_dispatch_shaping.das", "test_driver_knob_getters.das", "test_facade.das",
                    "test_flash.das", "test_forward.das", "test_from_template.das", "test_fused_decode.das", "test_gguf_quant.das", "test_gguf_shards.das",
                    "test_gpu_home.das", "test_gpu_model_swap.das", "test_gpu_moe_shexp.das", "test_gpu_perop_kv_codec.das", "test_gpu_perop_kv_codec_iq.das", "test_gpu_perop_kv_codec_k.das", "test_gpu_resident_codec_llama_iq.das", "test_gpu_resident_codec_llama_k.das", "test_gpu_resident_gemma2.das", "test_gpu_resident_gemma3_1b.das", "test_gpu_resident_gemma3_4b.das", "test_gpu_resident_gemma4_12b_k.das", "test_gpu_resident_gemma4_12b_q8.das", "test_gpu_resident_gemma4_26b.das", "test_gpu_resident_gemma4_26b_k.das", "test_gpu_resident_gemma4_26b_mx4.das", "test_gpu_resident_gemma4_26b_host.das", "test_gpu_resident_moe_host_pinned.das", "test_gpu_resident_gemma4_31b.das", "test_gpu_resident_gemma4_e2b.das", "test_gpu_resident_gemma4_e4b.das", "test_gpu_resident_gptoss.das", "test_gpu_resident_hc.das", "test_gpu_resident_hc_micro.das", "test_parity_dump.das", "test_gpu_resident_continuation.das", "test_gpu_resident_long_window.das", "test_gpu_resident_hybrid.das", "test_gpu_resident_llama.das", "test_gpu_resident_cuda_llama.das", "test_gpu_resident_cuda_gemma.das", "test_gpu_resident_cuda_qwen2.das", "test_gpu_resident_cuda_codec.das", "test_gpu_resident_cuda_batch.das", "test_gpu_resident_cuda_qwen3.das", "test_gpu_resident_cuda_phi3.das", "test_gpu_resident_cuda_qwen35.das", "test_gpu_resident_cuda_mtp.das", "test_gpu_resident_cuda_moe.das", "test_gpu_resident_cuda_moe_host.das", "test_gpu_resident_cuda_gemma4.das", "test_gpu_resident_segments.das", "test_gpu_resident_moe.das", "test_gpu_resident_moe_host_rows.das", "test_gpu_resident_moe_formats.das", "test_gpu_resident_format_census.das", "test_gpu_resident_qwen2.das", "test_gpu_resident_regions.das", "test_gpu_resident_regions_e2b.das", "test_gpu_resident_regions_gptoss.das", "test_gpu_resident_regions_qwen3moe.das", "test_gpu_resident_regions_qwen35moe.das", "test_gpu_resident_regions_qwen2moe.das", "test_gpu_resident_regions_gemma4moe.das", "test_gpu_resident_regions_gemma2.das", "test_gpu_resident_regions_gemma3.das", "test_gpu_resident_regions_gemma4.das", "test_gpu_resident_regions_hybrid.das", "test_gpu_resident_regions_hybrid_k.das", "test_gpu_resident_regions_hybrid_iq3s.das", "test_gpu_resident_regions_hybrid_q3kxl.das", "test_gpu_resident_regions_llama_k.das", "test_gpu_resident_regions_llama3b_ctx32k.das", "test_gpu_resident_regions_gemma3_4b_ctx128k.das", "test_gpu_resident_regions_gemma4_e2b_ctx128k.das", "test_gpu_resident_regions_phi3.das", "test_gpu_resident_regions_qwen3.das", "test_gpu_resident_regions_q8kv.das", "test_gpu_resident_regions_tq4kv.das", "test_gpu_serving_declines.das", "test_gpu_shared_units.das", "test_gpu_slot_swap.das", "test_gpu_tier.das", "test_groupn.das",
                    "test_embeddings.das", "test_jobque_tripwire.das", "test_kernel_backend.das", "test_kgroup_repack.das", "test_kqformat.das", "test_kquant.das",
                    "test_kv_codec.das", "test_kv_paged.das", "test_kv_prefix.das", "test_metal_batched_row.das", "test_metal_format_census.das", "test_math_activations.das",
                    "test_math_elementwise.das", "test_matmul.das", "test_matmul_batch.das", "test_f32_batch_tile_perms.das",
                    "test_metal_float_a_gate.das", "test_metal_gemm.das", "test_metal_role_infer.das", "test_mtp.das",
                    "test_mtp_gemma_drafter.das", "test_mtp_sampled_walk.das", "test_mtp_snapshot.das", "test_mtp_accept_walks.das", "test_sampling_landed.das", "test_mxfp4.das",
                    "test_par_indexed.das", "test_parity.das", "test_parity_cache.das", "test_parity_pregate.das",
                    "test_plane.das", "test_ple_check.das", "test_ple_modes.das", "test_prefill.das", "test_prefill_cpu_kernels.das",
                    "test_q8q8_family.das", "test_quant.das", "test_qwen4exp.das",
                    "test_repack.das", "test_repack_lane_context.das", "test_rmsnorm.das", "test_rope.das", "test_rope_apply.das",
                    "test_sampling.das", "test_constraint.das", "test_cuda_kernels.das",
                    "test_scheduler.das", "test_silu.das", "test_softmax.das", "test_storyteller_restart.das", "test_storywish.das", "test_think_split.das", "test_tokenizer.das",
                    "test_tool_formats.das", "test_unicode.das", "test_vulkan_dec_tail.das", "test_vulkan_host_twin.das", "test_vulkan_kernels.das", "test_vulkan_kv_codec_kernels.das",
                    "test_vulkan_mint.das", "test_cuda_mint.das", "test_vulkan_moe_cm2.das", "test_vulkan_tier.das", "test_vulkan_shared_device.das" ]
    }
    var none : array<string>
    return <- none
}

//! the image suite's arms an area owns (substring tokens, `arm_on` semantics); "" = the area has none
def area_image_arms(area : string) : string {
    if (area == "audio") {
        return "whisper,voxtral,parakeet,gigaam,qwen3a,canary,gemma4a,mtower"
    } elif (area == "vision") {
        return "gemma4uv,gemma4v,gemma3v,gemma4e,tower"
    } elif (area == "llm") {
        return "mechanics,smol,untied,metal,mx4,b32"
    } elif (area == "tts") {
        return "kitten"
    } elif (area == "translate") {
        return "bergamot"
    }
    return ""
}

//! engine module stem -> the areas a change to it reaches. A module not in the table is CORE and
//! reaches every area (the safe default for a new module until it gets a row)
let MODULE_AREAS <- {
    "dasllama_tower" => "audio,vision,tts,translate",   // the TTS blocks, Pocket and StyleTTS2 call its row kernels; the Bergamot chain its GEMMs
    "dasllama_metal_tower" => "audio,vision,tts",
    "dasllama_vulkan_tower" => "audio,vision,tts,translate",   // the Vulkan TTS and translation drivers ride the tower's classes
    "dasllama_tts_slab" => "audio,vision,tts",   // the Metal tower requires it for every seat
    "dasllama_vulkan_tts" => "tts",
    "dasllama_gpu_kernels_common" => "audio,vision,tts,translate,llm",   // the kernels both GPU homes stamp: the TTS chains', the depthwise rows and rope the audio and vision towers stamp, the Bergamot step's
    "dasllama_asr" => "audio", "dasllama_asr_types" => "audio", "dasllama_audio" => "audio",
    "dasllama_audio_embedder" => "audio", "dasllama_audio_io" => "audio", "dasllama_canary" => "audio",
    "dasllama_gemma4a" => "audio", "dasllama_metal_asr_dec" => "audio", "dasllama_vulkan_asr_dec" => "audio", "dasllama_parakeet" => "audio",
    "dasllama_nmt" => "translate", "dasllama_metal_nmt" => "translate", "dasllama_vulkan_nmt" => "translate", "dasllama_translate" => "translate",
    "dasllama_gigaam" => "audio", "dasllama_rnnt" => "audio", "dasllama_qwen3a" => "audio", "dasllama_vad" => "audio", "dasllama_whisper" => "audio",
    "dasllama_gemma3v" => "vision", "dasllama_gemma4uv" => "vision", "dasllama_gemma4v" => "vision",
    "dasllama_qwen25v" => "vision", "dasllama_qwen3v" => "vision", "dasllama_vision" => "vision",
    "dasllama_vision_embedder" => "vision", "dasllama_vision_io" => "vision",
    "dasllama_g2p" => "tts", "dasllama_kitten" => "tts", "dasllama_kokoro" => "tts", "dasllama_pocket" => "tts", "dasllama_postag" => "tts",
    "dasllama_styletts2" => "tts", "dasllama_textnorm" => "tts", "dasllama_tts" => "tts",
    "dasllama_tts_blocks" => "tts", "dasllama_tts_types" => "tts",
    "dasllama_exchange" => "infra", "dasllama_exchange_schema" => "infra", "dasllama_lint" => "infra",
    "dasllama_version" => "infra", "dasllama_fat_start" => "infra",
    "dasllama_chat" => "llm,vision,audio", "dasllama_tools" => "llm,vision,audio",
    "dasllama_arch_gemma2" => "llm", "dasllama_arch_gemma3" => "llm", "dasllama_arch_gemma4" => "llm",
    "dasllama_arch_glm4moe" => "llm", "dasllama_arch_gptoss" => "llm", "dasllama_arch_llama" => "llm",
    "dasllama_arch_mistral3" => "llm", "dasllama_arch_phi3" => "llm", "dasllama_arch_qwen2" => "llm",
    "dasllama_arch_qwen2moe" => "llm", "dasllama_arch_qwen3" => "llm", "dasllama_arch_qwen35" => "llm", "dasllama_arch_qwen4exp" => "llm",
    "dasllama_arch_qwen3moe" => "llm", "dasllama_attn_prefill" => "llm", "dasllama_batch" => "llm",
    "dasllama_bpe" => "llm", "dasllama_gpu_resident" => "llm,audio,vision", "dasllama_gpu_tier" => "llm",
    "dasllama_kv_dtype" => "llm", "dasllama_metal_decode" => "llm", "dasllama_metal_mtp_gemma" => "llm",
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
    "dasllama_cuda_common" => "llm", "dasllama_cuda_dispatch" => "llm", "dasllama_cuda_classes" => "llm",
    "dasllama_cuda_decode" => "llm", "dasllama_cuda_stream" => "llm", "dasllama_math_cuda" => "llm"
}

//! the stocked llm files a change to the engine core reaches - a witness a family and home, the codec rig, the regions
//! form, the segmented window, the CPU chains - beside every model-free llm file: the per-edit gate. The whole stocked
//! set stays the pre-merge gate (`--suite stocked`, sharded over the fleet). Non-private: test_run_suites.das pins it
let LLM_CORE_WITNESSES <- [
    "test_batch_decode.das", "test_deltanet.das", "test_forward.das", "test_gpu_perop_kv_codec.das",
    "test_gpu_resident_codec_llama_k.das", "test_gpu_resident_cuda_llama.das", "test_gpu_resident_cuda_moe.das",
    "test_gpu_resident_cuda_qwen35.das", "test_gpu_resident_gemma3_1b.das", "test_gpu_resident_gemma4_26b.das",
    "test_gpu_resident_gemma4_e2b.das", "test_gpu_resident_gptoss.das", "test_gpu_resident_hybrid.das",
    "test_gpu_resident_llama.das", "test_gpu_resident_moe.das", "test_gpu_resident_qwen2.das",
    "test_gpu_resident_regions.das", "test_gpu_resident_regions_phi3.das", "test_gpu_resident_regions_qwen3.das",
    "test_gpu_resident_segments.das", "test_kv_codec.das", "test_mtp.das", "test_prefill.das", "test_scheduler.das" ]

let CUDA_WITNESSES = "test_cuda_kernels.das,test_gpu_resident_cuda_llama.das,test_gpu_resident_cuda_codec.das,test_gpu_resident_cuda_batch.das,test_gpu_resident_cuda_qwen3.das,test_gpu_resident_cuda_phi3.das,test_gpu_resident_cuda_qwen35.das,test_gpu_resident_cuda_mtp.das,test_gpu_resident_cuda_moe.das,test_gpu_resident_cuda_moe_host.das,test_gpu_resident_cuda_gemma4.das,test_gpu_resident_cuda_gemma.das,test_gpu_resident_cuda_qwen2.das,test_gpu_resident_segments.das"
let KV_CODEC_WITNESSES = "test_kv_codec.das,test_vulkan_kv_codec_kernels.das,test_gpu_resident_codec_llama_k.das,test_gpu_resident_codec_llama_iq.das,test_gpu_perop_kv_codec.das,test_gpu_perop_kv_codec_k.das,test_gpu_perop_kv_codec_iq.das,test_gpu_resident_cuda_codec.das,test_gpu_resident_regions_q8kv.das,test_gpu_resident_regions_tq4kv.das"
let TOKENIZER_WITNESSES = "test_tokenizer.das,test_unicode.das,test_chat.das,test_tok_seed.das"
let METAL_WITNESSES = "test_metal_batched_row.das,test_metal_format_census.das,test_metal_gemm.das,test_metal_role_infer.das,test_metal_float_a_gate.das"

//! engine module stem -> the llm files a change to it reaches (base names, comma-separated) in place of the core
//! witnesses: a leaf module's own carriers and kernel cells. A module with no row here and an llm reach in
//! MODULE_AREAS (or no row there) is core. Its other areas still come from MODULE_AREAS. Non-private:
//! test_run_suites.das pins every row to the census
let MODULE_WITNESSES <- {
    "dasllama_cuda_common" => CUDA_WITNESSES, "dasllama_cuda_dispatch" => CUDA_WITNESSES, "dasllama_cuda_classes" => CUDA_WITNESSES,
    "dasllama_cuda_decode" => CUDA_WITNESSES, "dasllama_cuda_stream" => CUDA_WITNESSES, "dasllama_math_cuda" => CUDA_WITNESSES,
    "dasllama_arch_llama" => "test_gpu_resident_llama.das,test_gpu_resident_cuda_llama.das,test_gpu_resident_regions_llama_k.das,test_gpu_resident_codec_llama_k.das,test_forward.das,test_prefill.das",
    "dasllama_arch_qwen2" => "test_gpu_resident_qwen2.das,test_gpu_resident_cuda_qwen2.das,test_gpu_perop_kv_codec.das,test_gpu_perop_kv_codec_iq.das,test_gpu_resident_regions.das",
    "dasllama_arch_qwen3" => "test_gpu_resident_cuda_qwen3.das,test_gpu_resident_regions_qwen3.das,test_gpu_perop_kv_codec_k.das",
    "dasllama_arch_qwen35" => "test_gpu_resident_hybrid.das,test_gpu_resident_continuation.das,test_gpu_resident_cuda_qwen35.das,test_gpu_resident_cuda_mtp.das,test_gpu_resident_regions_hybrid.das,test_gpu_resident_long_window.das,test_gpu_resident_moe_formats.das,test_deltanet.das,test_mtp.das,test_vulkan_mint.das",
    "dasllama_arch_qwen3moe" => "test_gpu_resident_regions_qwen3moe.das,test_gpu_resident_moe_formats.das,test_gpu_resident_format_census.das,test_gpu_resident_cuda_moe.das",
    "dasllama_arch_qwen2moe" => "test_gpu_resident_moe.das,test_gpu_resident_cuda_moe.das,test_gpu_resident_cuda_moe_host.das,test_gpu_resident_regions_qwen2moe.das,test_gpu_moe_shexp.das,test_gpu_resident_moe_host_rows.das,test_gpu_resident_moe_host_pinned.das",
    "dasllama_arch_qwen4exp" => "test_qwen4exp.das,test_gpu_resident_hc.das,test_gpu_resident_hc_micro.das",
    "dasllama_arch_gptoss" => "test_gpu_resident_gptoss.das,test_gpu_resident_regions_gptoss.das,test_gpu_resident_cuda_moe.das",
    "dasllama_arch_gemma2" => "test_gpu_resident_gemma2.das,test_gpu_resident_cuda_gemma.das,test_gpu_resident_regions_gemma2.das",
    "dasllama_arch_gemma3" => "test_gpu_resident_gemma3_1b.das,test_gpu_resident_gemma3_4b.das,test_gpu_resident_cuda_gemma.das,test_gpu_resident_regions_gemma3.das",
    "dasllama_arch_gemma4" => "test_gpu_resident_cuda_gemma4.das,test_gpu_resident_gemma4_e2b.das,test_gpu_resident_gemma4_e4b.das,test_gpu_resident_gemma4_12b_k.das,test_gpu_resident_gemma4_26b.das,test_gpu_resident_gemma4_31b.das,test_gpu_resident_regions_gemma4.das,test_gpu_resident_regions_gemma4moe.das,test_ple_modes.das,test_mtp_gemma_drafter.das",
    "dasllama_arch_phi3" => "test_gpu_resident_cuda_phi3.das,test_gpu_resident_regions_phi3.das",
    "dasllama_arch_glm4moe" => "test_mtp.das", "dasllama_arch_mistral3" => "test_chat.das,test_model_specs.das",
    "dasllama_moe" => "test_gpu_resident_moe.das,test_gpu_resident_moe_formats.das,test_gpu_resident_cuda_moe.das,test_gpu_resident_gptoss.das,test_gpu_resident_gemma4_26b.das,test_gpu_moe_shexp.das,test_vulkan_dec_tail.das,test_vulkan_moe_cm2.das,test_vulkan_host_twin.das",
    "dasllama_mtp_gemma" => "test_mtp_gemma_drafter.das", "dasllama_vulkan_mtp_gemma" => "test_mtp_gemma_drafter.das",
    "dasllama_cuda_mtp_gemma" => "test_mtp_gemma_drafter.das",
    "dasllama_metal_decode" => METAL_WITNESSES, "dasllama_metal_prefill" => METAL_WITNESSES, "dasllama_metal_mtp_gemma" => METAL_WITNESSES,
    "dasllama_scheduler" => "test_scheduler.das",
    "dasllama_sampling" => "test_sampling.das,test_sampling_landed.das,test_constraint.das,test_mtp_sampled_walk.das,test_mtp_accept_walks.das,test_scheduler.das",
    "dasllama_batch" => "test_batch_decode.das,test_batch_grid.das,test_gpu_resident_cuda_batch.das",
    "dasllama_kv_codec" => KV_CODEC_WITNESSES, "dasllama_kv_dtype" => KV_CODEC_WITNESSES,
    "dasllama_prefix" => "test_kv_prefix.das,test_kv_paged.das",
    "dasllama_ple" => "test_ple_check.das,test_ple_modes.das,test_gpu_resident_gemma4_e2b.das,test_gpu_resident_regions_e2b.das",
    "dasllama_parity" => "test_parity.das,test_parity_pregate.das,test_parity_cache.das,test_parity_dump.das",
    "dasllama_bench" => "test_bench_rows.das,test_served_bench.das",
    "dasllama_tokenizer" => TOKENIZER_WITNESSES, "dasllama_bpe" => TOKENIZER_WITNESSES, "dasllama_spm" => TOKENIZER_WITNESSES,
    "dasllama_pretok" => TOKENIZER_WITNESSES, "dasllama_unicode" => TOKENIZER_WITNESSES,
    "dasllama_chat" => "test_chat.das,test_tool_formats.das,test_think_split.das,test_scheduler.das",
    "dasllama_tools" => "test_chat.das,test_tool_formats.das,test_think_split.das,test_scheduler.das",
    "dasllama_attn_prefill" => "test_prefill.das,test_prefill_cpu_kernels.das,test_flash.das,test_attn_span.das"
}

let BENCH_WITNESSES <- ["test_bench_rows.das", "test_served_bench.das", "test_bench_records_schema.das"]

def private csv_list(csv : string) : array<string> => <- split_trimmed(csv, ",")

//! a `tests/_*.das` fixture -> the areas a change to it reaches when no test requires it by name
def private fixture_areas(name : string) : array<string> {
    if (name |> starts_with("_tts_")) {
        return <- ["tts"]
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
    return <- clone_to_move(AREAS)
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
        return <- [for (a in AREAS); a; where find_index(area_tests(a), name) >= 0]
    }
    if (rel |> starts_with("dasllama/")) {
        let stem = name |> ends_with(".das") ? slice(name, 0, length(name) - 4) : name
        let row = MODULE_AREAS?[stem] ?? ""
        return <- (empty(row) ? clone_to_move(AREAS) : csv_list(row))
    }
    if (rel |> starts_with("performance/") || rel |> starts_with("harness/")) {
        return <- ["infra"]
    } elif (rel |> starts_with("benchmarks/")) {
        return <- ["llm"]
    }
    return <- clone_to_move(AREAS)
}

//! what --changed runs for one changed path
struct Reach {
    files : array<string>   //! the test files it runs, base names
    areas : array<string>   //! the areas it runs whole - never llm, whose files are named
    core : bool             //! the engine core changed: the whole stocked set is owed before the PR
    note : string           //! why a path runs nothing, "" when it runs something or lies outside the module
}

//! the llm area's model-free files - the kernel cells and the units every engine change reaches
def private llm_model_free() : array<string> {
    let free <- suite_files("model-free")
    return <- [for (name in area_tests("llm")); name; where find_index(free, "{TESTS_DIR}/{name}") >= 0]
}

def private any_reached(stems : array<string>; reached : table<string>) : bool {
    for (s in stems) {
        return true if (key_exists(reached, s))
    }
    return false
}

//! the test files of the folder whose `require _<stem>` reaches `stem`, through the fixtures between: a fixture's
//! change runs the tests that read it. Non-private: test_run_suites.das pins it
def tests_requiring(stem : string) : array<string> {
    var requires : table<string; array<string>>   // a file's base name -> the fixture stems it requires
    fio::dir(TESTS_DIR) $(name) {
        if (name |> ends_with(".das") && (name |> starts_with("test_") || name |> starts_with("failed_") || name |> starts_with("_"))) {
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
    var listed <- suite_files("model-free")
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
        if (find_index(suite_files("model-free"), p) >= 0 || find_index(suite_files("stocked"), p) >= 0) {
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

//! --shard i/n: the files at every n-th index from the (i - 1)-th of the list's order, which the
//! planners sort, so the shards of one command partition its run and spread the costly
//! neighbours. `why` names a form the flag refuses; "" takes the whole list. Non-private:
//! test_run_suites.das gates it
def shard_files(files : array<string>; shard : string) : tuple<kept : array<string>; why : string> {
    if (empty(shard)) {
        var whole := files
        return <- (kept <- whole, why = "")
    }
    let parts <- split(shard, "/")
    let i = length(parts) == 2 ? to_int(parts[0]) : 0
    let n = length(parts) == 2 ? to_int(parts[1]) : 0
    if (length(parts) != 2 || i < 1 || n < 1 || i > n || "{i}/{n}" != shard) {
        var none : array<string>
        return <- (kept <- none, why = "--shard '{shard}' is not i/n with 1 <= i <= n")
    }
    var kept <- [for (k in range(length(files))); files[k]; where k % n == i - 1]
    return <- (kept <- kept, why = "")
}

// the trailing "N tests, ..." summary line of a dastest log ("" when the run died before
// printing one — crash, kill, or compile error). A final line with no trailing newline counts:
// a killed run's last bytes are exactly the ones worth reporting.
// Non-private: test_run_summary.das gates it.
def log_summary(log_path : string) : string {
    var summary = ""
    fopen(log_path, "rb") $(f) {
        if (f != null) {
            let text = fread(f)
            peek_data(text) $(bytes) {
                let n = length(bytes)
                var at = 0
                var line_start = 0
                while (at <= n) {   // at == n closes the final line even when no newline does
                    if (at == n || bytes[at] == uint8('\n')) {
                        let line = slice(bytes, line_start, at)
                        if (find(line, " tests, ") >= 0) {
                            summary = clone_string(line)
                        }
                        line_start = at + 1
                    }
                    at++
                }
            }
        }
    }
    return summary
}

// CPU_PREFILL: the suites' CPU truth halves and parity controls prefill on the CPU by intent.
// HAZARD_STRICT: the scheduling-unification ratchet's runtime half — an undeclared dispatch
// on an armed hazard oracle PANICS here, so declaration gaps can never ship quietly.
// cmd.exe has no VAR=val prefix — `set "VAR=val"&&` is the safe form (llvm_tune's tuner_cmd)
def private child_envs(cfg : RunArgs; win : bool; arm : string) : string {
    var envs = ""
    if (win) {
        envs = ((cfg.full ? "set \"DASLLAMA_PARITY_FULL=1\"&& " : "set \"DASLLAMA_TEST_ARMS={arm}\"&& ")
            + "set \"DASLLAMA_CPU_PREFILL=1\"&& set \"DASLLAMA_METAL_HAZARD_STRICT=1\"&& ")
        if (!empty(cfg.family)) {   // composes with either mode: a family-scoped full gate is the profiling-cadence default
            envs = "{envs}set \"DASLLAMA_TEST_FAMILY={cfg.family}\"&& "
        }
    } else {
        envs = (cfg.full ? "DASLLAMA_PARITY_FULL=1" : "DASLLAMA_TEST_ARMS='{arm}'") + " DASLLAMA_CPU_PREFILL=1 DASLLAMA_METAL_HAZARD_STRICT=1 "
        if (!empty(cfg.family)) {
            envs = "DASLLAMA_TEST_FAMILY='{cfg.family}' {envs}"
        }
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
    if (empty(g_env_core.tune_manifest) && !cfg.no_tune) {
        let manifest = "modules/dasLLAMA/performance/{derive_box_name(g_env_prof.box)}.tune.json"
        var mst : FStat
        if (stat(manifest, mst)) {
            envs = (win ? "{envs}set \"DAS_TUNE_MANIFEST={manifest}\"&& "
                : "DAS_TUNE_MANIFEST='{manifest}' {envs}")
        }
    }
    return envs
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
//! model-free or stocked census lists; returns the image-suite arm tokens the areas own too
def area_plan(areas : array<string>) : tuple<files : array<string>; image_arms : string> {
    let free <- suite_files("model-free")
    let stocked <- suite_files("stocked")
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

//! a child's dastest timeout in seconds: the full gate, the stocked gate and --changed (the stocked files a change
//! reaches) hold the large tier's tails - the parity file alone runs past 20 min there, the llama resident file's
//! CPU chains past 1200 s on a cold JIT cache; 1200 s is the arm-mode budget
def child_timeout(cfg : RunArgs) : int => (cfg.full || cfg.suite == "stocked" || cfg.changed) ? 3600 : 1200

//! the suite forms' argument contract: 0 = run, else main's exit code
def check_suite_args(cfg : RunArgs) : int {
    if (cfg.no_tune && (cfg.suite == "image" || cfg.suite == "image-vulkan")) {   //! the arm runs with the image rail off, and the image suites ARE the rail
        to_log(LOG_ERROR, "--no-tune sets DASLLAMA_IMAGE=0 in every child, so it cannot drive the image suites - drop --no-tune or pick another suite\n")
        return 2
    }
    if (cfg.suite == "model-free" || cfg.suite == "stocked") {
        if (cfg.full || !empty(cfg.arm)) {   //! the per-PR gates have no arms to scope: a filter would silently shrink them
            to_log(LOG_ERROR, "--suite {cfg.suite} takes neither --arm nor --full: it is a whole per-PR gate (--exclude names what it drops)\n")
            return 2
        }
    } elif (cfg.full == !empty(cfg.arm)) {   // both set, or neither
        to_log(LOG_ERROR, "pass --arm <filter> scoped to what the change can affect (--full is disabled while the Metal build-out is in progress; large tier = DASLLAMA_PARITY_FULL=1 with --arm)\n")
        return 2
    }
    if (cfg.full) {
        to_log(LOG_ERROR, "please narrow the scope to the correct set of gates, metal is not fully implemented yet\n")
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
    return <- (files <- suite_files(cfg.suite), image_arms = cfg.arm)
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
    failed_tags : array<string>
}

//! the per-test cache folder of the run's binary, and the folders of earlier binaries removed when the run is
//! on its own binary (a --das dry run keeps them); "" when the binary cannot be stat'd, and the children get
//! no cache. A record serves a child's unchanged module prefix: the engine's compile (~90 s a file) becomes a
//! read of a few seconds, so a re-run pays the test, not the parse. Non-private: test_run_suites.das gates it
def child_cache_dir(das_exe : string; own_binary : bool) : string {
    var st : FStat
    if (!stat(das_exe, st)) {
        return ""
    }
    let id = "{int64(st.size)}-{int64(st.mtime)}"
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

//! one dastest child per file, `--nreps` times. On an area run the image suite's child gets the
//! areas' arms and every other child none; a suite run passes --arm through to every child
def private run_files(cfg : RunArgs; files : array<string>; image_arms : string; area_run : bool) : RunTally {
    let tmp = temp_dir()
    let win = get_platform_name() == "windows"
    let envs = child_envs(cfg, win, cfg.arm)
    let image_envs = child_envs(cfg, win, image_arms)
    let tmo = child_timeout(cfg)
    let reps = max(cfg.nreps, 1)
    let das_exe = empty(cfg.das) ? get_command_line_arguments()[0] : cfg.das
    mkdir(dir_name(MODULE_CACHE))
    let cache_dir = child_cache_dir(das_exe, empty(cfg.das))
    let cache_flag = empty(cache_dir) ? "" : " --module-cache-dir {cache_dir}"
    to_log(LOG_INFO, empty(cache_dir) ? "CACHE none - the binary could not be stat'd, every child parses the engine\n" : "CACHE {cache_dir}\n")
    var tally : RunTally
    for (rep in range(reps)) {
        for (file in files) {
            let tag = base_name(file)
            let log_path = path_join(tmp, "dasllama_run_{tag}_{rep}_{ref_time_ticks()}.log")
            let file_envs = (area_run && tag == "test_model_image.das") ? image_envs : envs
            let cmd = "{file_envs}\"{das_exe}\" -jit -module-cache {MODULE_CACHE} dastest/dastest.das -- --test \"{file}\" --timeout {tmo}{cache_flag} > \"{log_path}\" 2>&1"
            let rep_tag = reps > 1 ? " rep {rep + 1}/{reps}" : ""
            to_log(LOG_INFO, "START {tag}{rep_tag}: {cmd}\n")
            let t0 = ref_time_ticks()
            let rc = unsafe(popen(cmd) $(f) {
                let _drain = fread(f)   // output is redirected; drain keeps the pipe contract
            })
            let secs = double(get_time_usec(t0)) / 1000000.0lf
            let summary = log_summary(log_path)
            let verdict = !empty(summary) ? summary : "no dastest summary - the run died early (read the log)"
            to_log(rc == 0 ? LOG_INFO : LOG_ERROR, "DONE {tag}: exit={rc} {verdict} elapsed={secs}s log={log_path}\n")
            tally.skipped += skipped_cells(summary)
            if (rc != 0) {
                tally.failures++
                tally.failed_tags |> push(tag)
            }
        }
    }
    return <- tally
}

//! the run tells the next step itself: what ran, what a missing model skipped, what is still owed
def private print_report(cfg : RunArgs; areas : array<string>; n_files, n_excluded : int; tally : RunTally; core : bool) {
    let scope = ((cfg.changed ? "changed vs {cfg.base}" : (!empty(areas) ? "areas {join(areas, ",")}" : "suite {cfg.suite}"))
        + (empty(cfg.only) ? "" : " only {cfg.only}") + (empty(cfg.shard) ? "" : " shard {cfg.shard}"))
    let failed_list = empty(tally.failed_tags) ? "" : " ({join(tally.failed_tags, ", ")})"
    let excluded = n_excluded == 0 ? "" : ", {n_excluded} file(s) excluded"
    to_log(tally.failures == 0 ? LOG_INFO : LOG_ERROR, "REPORT {scope}: {n_files} file(s), {tally.failures} failed{failed_list}, {tally.skipped} cell(s) skipped (a filtered arm, an absent device, or a missing model or fixture - each skips loudly){excluded}\n")
    if (!empty(cfg.shard)) {
        let n = to_int(split(cfg.shard, "/")[1])
        let others <- [for (k in range(n)); "{k + 1}/{n}"; where "{k + 1}/{n}" != cfg.shard]
        if (!empty(others)) {
            to_log(LOG_INFO, "NEXT the run's other shard(s), each on its own box, with the same command: --shard {join(others, ", --shard ")}\n")
        }
    }
    if (cfg.changed) {
        if (core) {
            to_log(LOG_INFO, "NEXT the engine core changed: the whole stocked set before the PR - run.das -- --suite stocked, sharded over the fleet (harness/pod_fleet.das)\n")
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
    if (cfg.help) {
        print_help(get_command_info(type<RunArgs>), "modules/dasLLAMA/tests/run.das")
        return 0
    }
    var sel <- select_areas(cfg)
    if (sel.status >= 0) {
        return sel.status
    }
    let areas <- sel.areas
    let suite_rc = (empty(areas) && !cfg.changed) ? check_suite_args(cfg) : 0
    if (suite_rc != 0) {
        return suite_rc
    }
    var plan <- plan_files(cfg, areas)
    var core = false
    if (cfg.changed) {
        var changed <- changed_plan(cfg)
        if (changed.status >= 0) {
            return changed.status
        }
        plan.files <- changed.files
        plan.image_arms = changed.image_arms
        core = changed.core
    }
    if (empty(plan.files)) {
        to_log(LOG_ERROR, "unknown --suite '{cfg.suite}' (decode | mtp | prefill | matrix | kernels | image | image-vulkan | coverage | model-free | stocked | all)\n")
        return 2
    }
    if (cfg.no_tune && !empty(plan.image_arms) && (!empty(areas) || cfg.changed)) {   //! an area's image arms ride the image rail --no-tune turns off
        to_log(LOG_ERROR, "--no-tune sets DASLLAMA_IMAGE=0 in every child, and the run owns the image arms {plan.image_arms} - drop --no-tune, or run those arms through --suite image --arm\n")
        return 2
    }
    var split <- exclude_files(plan.files, cfg.exclude)
    for (file in split.dropped) {
        to_log(LOG_INFO, "EXCLUDED {base_name(file)} (--exclude {cfg.exclude})\n")
    }
    if (empty(split.kept)) {
        to_log(LOG_ERROR, "--exclude '{cfg.exclude}' dropped every file of the run\n")
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
    let shard <- shard_files(only, cfg.shard)
    if (!empty(shard.why)) {
        to_log(LOG_ERROR, "{shard.why}\n")
        return 2
    }
    if (!empty(cfg.shard)) {
        to_log(LOG_INFO, "SHARD {cfg.shard}: {length(shard.kept)} of {length(only)} file(s)\n")
    }
    if (empty(shard.kept)) {
        to_log(LOG_ERROR, "--shard {cfg.shard} holds no file of this run's {length(only)}\n")
        return 2
    }
    let tally <- run_files(cfg, shard.kept, plan.image_arms, !empty(areas) || cfg.changed)
    print_report(cfg, areas, length(shard.kept), length(split.dropped), tally, core)
    return tally.failures
}
