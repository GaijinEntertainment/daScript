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

let MODULE_CACHE = ".jitted_scripts/module_cache/dastest.dascache"

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

    @clarg_doc = "Area — comma-separated: audio | vision | tts | llm | infra; runs every model-free and stocked file of the area plus the image suite's arms of the area (no --suite, no --arm)"
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
                    "modules/dasLLAMA/tests/test_mtp_snapshot.das",
                    "modules/dasLLAMA/tests/test_mtp_sampled_walk.das",
                    "modules/dasLLAMA/tests/test_dasllama_lint_contracts.das",
                    "modules/dasLLAMA/tests/test_metal_float_a_gate.das",
                    "modules/dasLLAMA/tests/test_dasllama_lint_escape.das",
                    "modules/dasLLAMA/tests/test_dispatch_shaping.das",
                    "modules/dasLLAMA/tests/test_dlim_wipe.das",
                    "modules/dasLLAMA/tests/test_env_registry.das",
                    "modules/dasLLAMA/tests/test_exchange_schema.das",
                    "modules/dasLLAMA/tests/test_facade_docs.das",
                    "modules/dasLLAMA/tests/test_gen_records_args.das",
                    "modules/dasLLAMA/tests/test_gguf_quant.das",
                    "modules/dasLLAMA/tests/test_gguf_shards.das",
                    "modules/dasLLAMA/tests/test_gpu_tier.das",
                    "modules/dasLLAMA/tests/test_groupn.das",
                    "modules/dasLLAMA/tests/test_kernel_backend.das",
                    "modules/dasLLAMA/tests/test_kgroup_repack.das",
                    "modules/dasLLAMA/tests/test_kqformat.das",
                    "modules/dasLLAMA/tests/test_matmul.das",
                    "modules/dasLLAMA/tests/test_matmul_batch.das",
                    "modules/dasLLAMA/tests/test_metal_gemm.das",
                    "modules/dasLLAMA/tests/test_metal_role_infer.das",
                    "modules/dasLLAMA/tests/test_model_specs.das",
                    "modules/dasLLAMA/tests/test_mxfp4.das",
                    "modules/dasLLAMA/tests/test_par_indexed.das",
                    "modules/dasLLAMA/tests/test_parity_cache.das",
                    "modules/dasLLAMA/tests/test_plane.das",
                    "modules/dasLLAMA/tests/test_ple_check.das",
                    "modules/dasLLAMA/tests/test_program_roots.das",
                    "modules/dasLLAMA/tests/test_quant.das",
                    "modules/dasLLAMA/tests/test_repack.das",
                    "modules/dasLLAMA/tests/test_repack_lane_context.das",
                    "modules/dasLLAMA/tests/test_rmsnorm.das",
                    "modules/dasLLAMA/tests/test_rope.das",
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
                    "modules/dasLLAMA/tests/test_tool_formats.das",
                    "modules/dasLLAMA/tests/test_tower_helpers.das",
                    "modules/dasLLAMA/tests/test_tune_interrupt.das",
                    "modules/dasLLAMA/tests/test_unicode.das",
                    "modules/dasLLAMA/tests/test_vad.das",
                    "modules/dasLLAMA/tests/test_vision.das",
                    "modules/dasLLAMA/tests/test_vulkan_dec_tail.das",
                    "modules/dasLLAMA/tests/test_vulkan_kernels.das",
                    "modules/dasLLAMA/tests/test_vulkan_moe_cm2.das",
                    "modules/dasLLAMA/tests/test_vulkan_tier.das" ]
    } elif (name == "stocked") {
        return <- [
                    "modules/dasLLAMA/tests/test_attn_span.das",
                    "modules/dasLLAMA/tests/test_audio.das",
                    "modules/dasLLAMA/tests/test_audio_embedder.das",
                    "modules/dasLLAMA/tests/test_batch_decode.das",
                    "modules/dasLLAMA/tests/test_chat.das",
                    "modules/dasLLAMA/tests/test_deltanet.das",
                    "modules/dasLLAMA/tests/test_exe_smoke.das",
                    "modules/dasLLAMA/tests/test_facade.das",
                    "modules/dasLLAMA/tests/test_flash.das",
                    "modules/dasLLAMA/tests/test_forward.das",
                    "modules/dasLLAMA/tests/test_fused_decode.das",
                    "modules/dasLLAMA/tests/test_gemma3v.das",
                    "modules/dasLLAMA/tests/test_gemma4uv.das",
                    "modules/dasLLAMA/tests/test_gemma4v.das",
                    "modules/dasLLAMA/tests/test_gpu_model_swap.das",
                    "modules/dasLLAMA/tests/test_gpu_slot_swap.das",
                    "modules/dasLLAMA/tests/test_kquant.das",
                    "modules/dasLLAMA/tests/test_kv_codec.das",
                    "modules/dasLLAMA/tests/test_kv_paged.das",
                    "modules/dasLLAMA/tests/test_kv_prefix.das",
                    "modules/dasLLAMA/tests/test_mtp.das",
                    "modules/dasLLAMA/tests/test_mtp_gemma_drafter.das",
                    "modules/dasLLAMA/tests/test_parity.das",
                    "modules/dasLLAMA/tests/test_parity_pregate.das",
                    "modules/dasLLAMA/tests/test_ple_modes.das",
                    "modules/dasLLAMA/tests/test_prefill.das",
                    "modules/dasLLAMA/tests/test_qwen3v.das",
                    "modules/dasLLAMA/tests/test_qwen25v.das",
                    "modules/dasLLAMA/tests/test_sampling.das",
                    "modules/dasLLAMA/tests/test_scheduler.das",
                    "modules/dasLLAMA/tests/test_tokenizer.das",
                    "modules/dasLLAMA/tests/test_tts_postag.das",
                    "modules/dasLLAMA/tests/test_tts_g2p.das",
                    "modules/dasLLAMA/tests/test_tts_kitten.das",
                    "modules/dasLLAMA/tests/test_tts_kokoro.das",
                    "modules/dasLLAMA/tests/test_tts_facade.das",
                    "modules/dasLLAMA/tests/test_vision_chat.das",
                    "modules/dasLLAMA/tests/test_vision_embedder.das",
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

let AREAS <- ["audio", "vision", "tts", "llm", "infra"]
let TESTS_DIR = "modules/dasLLAMA/tests"

//! the suite-less test files by AREA - what someone working on audio, vision, TTS, the LLM engine or the
//! tooling runs after an edit. A file serving two areas sits in both (a shared tower helper's test).
//! Non-private: test_run_suites.das holds the union to the model-free + stocked census
def area_tests(area : string) : array<string> {
    if (area == "audio") {
        return <- [ "test_asr_verbs.das", "test_audio.das", "test_audio_embedder.das", "test_dasllama_lint_contracts.das",
                    "test_tower_helpers.das", "test_vad.das", "test_whisper.das" ]
    } elif (area == "vision") {
        return <- [ "test_attn_span.das", "test_gemma3v.das", "test_gemma4uv.das", "test_gemma4v.das", "test_qwen25v.das",
                    "test_qwen3v.das", "test_tower_helpers.das", "test_vision.das", "test_vision_chat.das",
                    "test_vision_embedder.das" ]
    } elif (area == "tts") {
        return <- [ "test_tts_blocks.das", "test_tts_facade.das", "test_tts_g2p.das", "test_tts_kitten.das",
                    "test_tts_kokoro.das", "test_tts_postag.das", "test_tts_textnorm.das" ]
    } elif (area == "infra") {
        return <- [ "failed_dasllama_lint_require.das", "failed_dasllama_lint_sidedoor.das", "test_bench_records_schema.das",
                    "test_box_ident.das", "test_box_profile.das", "test_dasllama_lint_contracts.das",
                    "test_dasllama_lint_escape.das", "test_dlim_wipe.das", "test_env_registry.das",
                    "test_exchange_schema.das", "test_exe_smoke.das", "test_facade_docs.das", "test_gen_records_args.das",
                    "test_model_specs.das", "test_program_roots.das", "test_run_suites.das", "test_run_summary.das",
                    "test_site_records.das", "test_sizing_helpers.das", "test_tok_seed.das", "test_tune_interrupt.das" ]
    } elif (area == "llm") {
        return <- [ "test_accel_backend.das", "test_affinity_predicates.das", "test_arch_registry.das", "test_batch_decode.das",
                    "test_batch_grid.das", "test_chat.das", "test_deltanet.das", "test_dispatch_shaping.das", "test_facade.das",
                    "test_flash.das", "test_forward.das", "test_fused_decode.das", "test_gguf_quant.das", "test_gguf_shards.das",
                    "test_gpu_model_swap.das", "test_gpu_slot_swap.das", "test_gpu_tier.das", "test_groupn.das",
                    "test_kernel_backend.das", "test_kgroup_repack.das", "test_kqformat.das", "test_kquant.das",
                    "test_kv_codec.das", "test_kv_paged.das", "test_kv_prefix.das", "test_matmul.das", "test_matmul_batch.das",
                    "test_metal_float_a_gate.das", "test_metal_gemm.das", "test_metal_role_infer.das", "test_mtp.das",
                    "test_mtp_gemma_drafter.das", "test_mtp_sampled_walk.das", "test_mtp_snapshot.das", "test_mxfp4.das",
                    "test_par_indexed.das", "test_parity.das", "test_parity_cache.das", "test_parity_pregate.das",
                    "test_plane.das", "test_ple_check.das", "test_ple_modes.das", "test_prefill.das", "test_quant.das",
                    "test_repack.das", "test_repack_lane_context.das", "test_rmsnorm.das", "test_rope.das", "test_sampling.das",
                    "test_scheduler.das", "test_silu.das", "test_softmax.das", "test_think_split.das", "test_tokenizer.das",
                    "test_tool_formats.das", "test_unicode.das", "test_vulkan_dec_tail.das", "test_vulkan_kernels.das",
                    "test_vulkan_moe_cm2.das", "test_vulkan_tier.das" ]
    }
    var none : array<string>
    return <- none
}

//! the image suite's arms an area owns (substring tokens, `arm_on` semantics); "" = the area has none
def area_image_arms(area : string) : string {
    if (area == "audio") {
        return "whisper,voxtral,parakeet,qwen3a,canary,gemma4a,mtower"
    } elif (area == "vision") {
        return "gemma4uv,gemma4v,gemma3v,gemma4e,tower"
    } elif (area == "llm") {
        return "mechanics,smol,metal"
    }
    return ""
}

//! engine module stem -> the areas a change to it reaches. A module not in the table is CORE and
//! reaches every area (the safe default for a new module until it gets a row)
let MODULE_AREAS <- {
    "dasllama_tower" => "audio,vision", "dasllama_metal_tower" => "audio,vision",
    "dasllama_asr" => "audio", "dasllama_asr_types" => "audio", "dasllama_audio" => "audio",
    "dasllama_audio_embedder" => "audio", "dasllama_audio_io" => "audio", "dasllama_canary" => "audio",
    "dasllama_gemma4a" => "audio", "dasllama_metal_asr_dec" => "audio", "dasllama_parakeet" => "audio",
    "dasllama_qwen3a" => "audio", "dasllama_vad" => "audio", "dasllama_whisper" => "audio",
    "dasllama_gemma3v" => "vision", "dasllama_gemma4uv" => "vision", "dasllama_gemma4v" => "vision",
    "dasllama_qwen25v" => "vision", "dasllama_qwen3v" => "vision", "dasllama_vision" => "vision",
    "dasllama_vision_embedder" => "vision", "dasllama_vision_io" => "vision",
    "dasllama_g2p" => "tts", "dasllama_kitten" => "tts", "dasllama_kokoro" => "tts", "dasllama_postag" => "tts",
    "dasllama_styletts2" => "tts", "dasllama_textnorm" => "tts", "dasllama_tts" => "tts",
    "dasllama_tts_blocks" => "tts", "dasllama_tts_types" => "tts",
    "dasllama_exchange" => "infra", "dasllama_exchange_schema" => "infra", "dasllama_lint" => "infra",
    "dasllama_version" => "infra",
    "dasllama_chat" => "llm,vision,audio", "dasllama_tools" => "llm,vision,audio",
    "dasllama_arch_gemma2" => "llm", "dasllama_arch_gemma3" => "llm", "dasllama_arch_gemma4" => "llm",
    "dasllama_arch_glm4moe" => "llm", "dasllama_arch_gptoss" => "llm", "dasllama_arch_llama" => "llm",
    "dasllama_arch_mistral3" => "llm", "dasllama_arch_phi3" => "llm", "dasllama_arch_qwen2" => "llm",
    "dasllama_arch_qwen2moe" => "llm", "dasllama_arch_qwen3" => "llm", "dasllama_arch_qwen35" => "llm",
    "dasllama_arch_qwen3moe" => "llm", "dasllama_attn_prefill" => "llm", "dasllama_batch" => "llm",
    "dasllama_bpe" => "llm", "dasllama_gpu_resident" => "llm", "dasllama_gpu_tier" => "llm",
    "dasllama_kv_codec" => "llm", "dasllama_metal_decode" => "llm", "dasllama_metal_mtp_gemma" => "llm",
    "dasllama_metal_prefill" => "llm", "dasllama_moe" => "llm", "dasllama_mtp_gemma" => "llm",
    "dasllama_parity" => "llm", "dasllama_ple" => "llm", "dasllama_prefix" => "llm", "dasllama_pretok" => "llm",
    "dasllama_rope" => "llm", "dasllama_sampling" => "llm", "dasllama_scheduler" => "llm", "dasllama_spm" => "llm",
    "dasllama_tokenizer" => "llm", "dasllama_transformer" => "llm", "dasllama_unicode" => "llm",
    "dasllama_vulkan_decode" => "llm", "dasllama_vulkan_prefill" => "llm", "dasllama_vulkan_seams" => "llm"
}

def private csv_list(csv : string) : array<string> {
    return <- [for (tok in split(csv, ",")); strip(tok); where !empty(strip(tok))]
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
    if (!(name |> ends_with(".das")) || name == "run.das") {
        return <- ["infra"]
    }
    if (rel |> starts_with("tests/")) {
        if (name |> starts_with("_")) {
            if (name |> starts_with("_tts_")) {
                return <- ["tts"]
            } elif (name == "_vision_oracle.das") {
                return <- ["vision"]
            } elif (name |> starts_with("_vk") || name == "_metal_kernel_common.das" || name == "_mtl_toy.das") {
                return <- ["llm"]
            }
            return <- clone_to_move(AREAS)
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

//! --exclude: a file whose base name contains any comma-separated token is dropped. Non-private:
//! test_run_suites.das gates it
def exclude_files(files : array<string>; exclude : string) : tuple<kept : array<string>; dropped : array<string>> {
    var kept : array<string>
    var dropped : array<string>
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
    // DAS_TUNE_MANIFEST inherits; a box without a minted manifest keeps sidecar behavior.
    if (empty(g_env_core.tune_manifest)) {
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
    unsafe {
        popen("git {args}") $(f) {
            if (f != null) {
                out = fread(f)
            }
        }
    }
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
    if (cfg.changed) {
        if (empty(git_lines("rev-parse --verify --quiet {cfg.base}"))) {   // a base git cannot resolve would read as "nothing changed"
            to_log(LOG_ERROR, "--base '{cfg.base}' is not a commit git knows - fetch origin, or name another base\n")
            return <- (status = 2, areas <- none)
        }
        for (p in changed_paths(cfg.base)) {
            let hit <- areas_for_path(p)
            if (empty(hit)) {
                if ((p |> replace("\\", "/")) |> starts_with("modules/dasLLAMA/")) {
                    to_log(LOG_INFO, "CHANGED {p} -> a model suite file: run its suite with --arm scoped to the change\n")
                }
                continue
            }
            to_log(LOG_INFO, "CHANGED {p} -> {join(hit, ",")}\n")
            for (a in hit) {
                want |> insert(a, null)
            }
        }
        if (empty(want)) {
            to_log(LOG_INFO, "CHANGED nothing vs {cfg.base} reaches an area - nothing to run\n")
            return <- (status = 0, areas <- none)
        }
    }
    var areas <- [for (a in AREAS); a; where key_exists(want, a)]
    to_log(LOG_INFO, "AREAS {join(areas, ",")}\n")
    return <- (status = -1, areas <- areas)
}

//! the suite forms' argument contract: 0 = run, else main's exit code
def check_suite_args(cfg : RunArgs) : int {
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

//! one dastest child per file, `--nreps` times. On an area run the image suite's child gets the
//! areas' arms and every other child none; a suite run passes --arm through to every child
def private run_files(cfg : RunArgs; files : array<string>; image_arms : string; area_run : bool) : RunTally {
    let tmp = temp_dir()
    let win = get_platform_name() == "windows"
    let envs = child_envs(cfg, win, cfg.arm)
    let image_envs = child_envs(cfg, win, image_arms)
    let tmo = cfg.full ? 3600 : 1200   //! the full gate holds the 8B/70B tails; 1200 s is the arm-mode budget
    let reps = max(cfg.nreps, 1)
    let das_exe = empty(cfg.das) ? get_command_line_arguments()[0] : cfg.das
    mkdir(dir_name(MODULE_CACHE))
    var tally : RunTally
    for (rep in range(reps)) {
        for (file in files) {
            let tag = base_name(file)
            let log_path = path_join(tmp, "dasllama_run_{tag}_{rep}_{ref_time_ticks()}.log")
            let file_envs = (area_run && tag == "test_model_image.das") ? image_envs : envs
            let cmd = "{file_envs}\"{das_exe}\" -jit -module-cache {MODULE_CACHE} dastest/dastest.das -- --test \"{file}\" --timeout {tmo} > \"{log_path}\" 2>&1"
            let rep_tag = reps > 1 ? " rep {rep + 1}/{reps}" : ""
            to_log(LOG_INFO, "START {tag}{rep_tag}: {cmd}\n")
            let t0 = ref_time_ticks()
            var rc = 0
            unsafe {
                rc = popen(cmd) $(f) {
                    let _drain = fread(f)   // output is redirected; drain keeps the pipe contract
                }
            }
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
def private print_report(cfg : RunArgs; areas : array<string>; n_files, n_excluded : int; tally : RunTally) {
    let scope = !empty(areas) ? "areas {join(areas, ",")}" : "suite {cfg.suite}"
    let failed_list = empty(tally.failed_tags) ? "" : " ({join(tally.failed_tags, ", ")})"
    let excluded = n_excluded == 0 ? "" : ", {n_excluded} file(s) excluded"
    to_log(tally.failures == 0 ? LOG_INFO : LOG_ERROR, "REPORT {scope}: {n_files} file(s), {tally.failures} failed{failed_list}, {tally.skipped} cell(s) skipped (a filtered arm, an absent device, or a missing model or fixture - each skips loudly){excluded}\n")
    if (!empty(areas)) {
        if (find_index(areas, "llm") >= 0) {
            to_log(LOG_INFO, "NEXT the model suites (decode | prefill | matrix) take --arm scoped to the change: run.das -- --arm <arms> --suite <name>\n")
        }
        to_log(LOG_INFO, "NEXT before the PR: run.das -- --suite model-free, then --suite stocked on a box with models (preflight --full runs both when the module changed)\n")
    } elif (cfg.suite == "model-free") {
        to_log(LOG_INFO, "NEXT before the PR: run.das -- --suite stocked on a box with models\n")
    }
}

[export]
def main() : int {
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
    let suite_rc = empty(areas) ? check_suite_args(cfg) : 0
    if (suite_rc != 0) {
        return suite_rc
    }
    let plan <- plan_files(cfg, areas)
    if (empty(plan.files)) {
        to_log(LOG_ERROR, "unknown --suite '{cfg.suite}' (decode | mtp | prefill | matrix | kernels | image | image-vulkan | coverage | model-free | stocked | all)\n")
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
    let tally <- run_files(cfg, split.kept, plan.image_arms, !empty(areas))
    print_report(cfg, areas, length(split.kept), length(split.dropped), tally)
    return tally.failures
}
