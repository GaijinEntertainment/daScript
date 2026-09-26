// The parakeet tower row needs ggml-parakeet-tdt-0.6b-v2-f32.bin under whisper_dir() and skips without it; the
// StyleTTS2 tower row needs kitten-nano.gguf (and kokoro-82m.gguf for the torch-law stamps) under models_dir() and the kitten
// family on, and skips without them; the Pocket row needs pocket-tts-en.gguf (and pocket-tts-en-kq.gguf for the q8 head stamps) and the pocket family on.
options gen2
options persistent_heap   // + explicit deletes: the models free before the process exits
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama   // nolint:STYLE029,LINT019 — the public facade (engine + arch registrations + the metal overrides); only the metal half uses more than dasllama_common
require dasllama/dasllama_math   // set_wscale_f16 — the s16 axis
require dasllama/dasllama_layout   // convert_model_to_metal_blob — the blob-flavor transform
require ?das_metal dasllama/dasllama_metal_common  // the coverage census rail
require ?das_metal dasllama/dasllama_metal_decode  // decode stats + shutdown rails
require ?das_metal metal/das_metal_boost  // device probe
require ?vulkan dasllama/dasllama_vulkan_common  // the vulkan census rail (spec-only here)
require dasllama/dasllama_gpu_resident  // moe_gpu_drop_model_ — the census's model-switch drops
require dasllama/dasllama_whisper   // the cov_tower whisper row
require dasllama/dasllama_audio     // load_audio_tower — the cov_tower erf-GELU carrier
require dasllama/dasllama_gemma4uv  // the cov_tower embedder row
require dasllama/dasllama_gemma4v   // the cov_tower ViT row
require dasllama/dasllama_gemma3v   // the cov_tower SigLIP row (the head-restride kernel)
require dasllama/dasllama_qwen3a    // the cov_tower conv2d + GPU-mel carrier
require dasllama/dasllama_gemma4a   // the cov_tower Conformer whole-chunk row
require dasllama/dasllama_canary    // the cov_tower FastConformer row
require dasllama/dasllama_parakeet  // the cov_tower parakeet row
require dasllama/dasllama_qwen25v   // the cov_tower window-ViT row
require dasllama/dasllama_qwen3v    // the cov_tower GPU-stem row
require dasllama/dasllama_styletts2
require dasllama/dasllama_pocket   // nolint:STYLE030,LINT019 — the Pocket census leg sits in the das_metal static_if half
require dasllama/dasllama_tts_types   // nolint:STYLE030,LINT019 — read only inside the Apple half
require ?das_metal dasllama/dasllama_metal_tower   // set_metal_tower — the census pins the rail ON
require ?das_metal dasllama/dasllama_metal_prefill // metal_prefill_shutdown — the crowned twin-W leg re-arms PSOs
require ?das_metal dasllama/dasllama_metal_asr_dec // set_metal_wdec — the cov_tower mixed chunk's decoder half
require daslib/jobque_boost
require daslib/fio  // the zoo paths
require math  // nolint:STYLE030,LINT019 — used only inside the das_metal static_if half
require strings
require _model_tier
require dasllama/dasllama_env   // nolint:STYLE030,LINT019 — the read sites sit in the apple static_if half; the linux lint flavor sees the require as unused

// The KERNEL COVERAGE census (REVIEW_GPU_KERNEL_CLASSES.md: a Metal kernel class is dispatched by a census row here or named in
// CENSUS_NEVER_DISPATCHED; a Vulkan kernel class by a census row or VK_CENSUS_NEVER_DISPATCHED): drive the small-model zoo across the
// format / graph / batch-shape / KV-codec axes and report which compiled kernels were never
// dispatched — LOUD WARNINGS, never an auto-dead verdict. A zero means "nothing this zoo runs
// dispatched it", never "unreachable" — deleting a kernel additionally needs a reachability
// audit of its dispatch predicate (models/quants/shapes not stocked here still count). This
// suite is the gate a kernel deletion must run before ("was it ever dispatched by anything
// we run?") and the home a NEW kernel's small-model run joins. Expected warnings on the small tier: MoE / mx4 / suppress and other
// kernels whose carriers sit above the tier (their census rows serve under
// DASLLAMA_PARITY_FULL=1); the report stays honest about what THIS run covered. The vulkan
// half: arm `coverage` = the device-free rail unit (spec parse + zero table); arm `coverage-vk`
// = the SERVING census (needs a vulkan device + DASLLAMA_GPU=1 + the local model zoo).

// The census's DECLARED blind spots: kernel classes no zoo row below can dispatch, each with
// the reason. A new [metal_kernel] belongs in a census row, not here; a class lands here only
// when no stocked carrier can reach it, and it leaves the moment a row can. Reconcile the list
// against the report's NEVER DISPATCHED warnings — a name here that the report shows dispatched
// is stale, and a warning with no entry here is a coverage gap to close with a row.
let private CENSUS_NEVER_DISPATCHED <- [
    // the MoE expert mul_mm tall M-tile pairs (128-row floor + remainder), ALL formats: the
    // tall stamps engage at npos * n_expert_used >= 128 * n_expert, and the stocked MoE
    // carriers (128/8, 60/4, 32/4 experts) put that floor at 1024-2048 rows - past the
    // ~590-token census prompt under the 1024-row session cap. Their coverage is the
    // tall-pair kernel-unit gate in test_metal_gemm_kernels.das
    "MetalMoeMulMmK4TH128", "MetalMoeMulMmK4THR",
    "MetalMoeMulMmK5TH128", "MetalMoeMulMmK5THR",
    "MetalMoeMulMmK6TH128", "MetalMoeMulMmK6THR",
    "MetalMoeMulMmQ8TH128", "MetalMoeMulMmQ8THR",
    "MetalMoeMulMmMx4TH128", "MetalMoeMulMmMx4THR",
    // the q5_1 MoE tensor twins, ALL stamps: no stocked carrier holds a Q5_1 expert plane
    // (the zoo's MoE carriers are Q8_0 / Q5_K_M / Q4_K_M / mxfp4; Q5_1 appears only when
    // expert_ffn % 256 != 0 forces the converter off K-quants). Their coverage is the
    // bit-exact twin + tall-pair gate in test_metal_prefill_kernels.das (moe_mulmm_q51_gate)
    "MetalMoeMulMmQ51T", "MetalMoeMulMmQ51TH",
    "MetalMoeMulMmQ51TH128", "MetalMoeMulMmQ51THR",
    // the dense tall in-kernel-dequant kq stamps: they engage only when a site's f16 panel
    // would reach TALLKQ_MIN_PANEL (96 MiB) - the zoo's small carriers top out far below it,
    // and the iq3 pair additionally wants a plane in a format the zoo does not stock.
    // Their coverage is the tall kernel-unit leg in test_metal_gemm_kernels.das (kq_mulmm_gate)
    "MetalKqMulMmK4TH128", "MetalKqMulMmK5TH128", "MetalKqMulMmK6TH128",
    "MetalKqMulMmIq3sTH128", "MetalKqMulMmIq3xxsTH128",
    // the nine formats the iquant arc added (Q2_K, Q3_K, IQ4_XS, IQ4_NL, IQ3_S, IQ3_XXS, IQ2_S,
    // IQ2_XS, IQ2_XXS): no stocked census carrier holds a plane in any of them - the zoo quants
    // are Q8_0 / Q5_K_M / Q4_K_M / mxfp4. Their coverage is the kernel-unit gates in
    // test_metal_gemv_kernels.das / test_metal_gemm_kernels.das and the per-format e2e parity
    // rows recorded in HOW_TO_ADD_A_FORMAT.md. A class naming its own kernel records the bare
    // shader entry name, so those stamps sit here under it, not under the class
    "metal_kq_gemv_k2", "MetalKqMvB2K2", "MetalKqMvB4K2", "metal_kq_mvb8_k2", "MetalKqMulMmK2",
    "metal_kq_gemv_k3", "MetalKqMvB2K3", "MetalKqMvB4K3", "metal_kq_mvb8_k3", "MetalKqMulMmK3",
    "MetalKqGemvIq4xs", "MetalKqMvB2Iq4xs", "MetalKqMvB4Iq4xs", "MetalKqMvB8Iq4xs", "MetalKqMulMmIq4xs",
    "MetalKqGemvIq4nl", "MetalKqMvB2Iq4nl", "MetalKqMvB4Iq4nl", "MetalKqMvB8Iq4nl", "MetalKqMulMmIq4nl",
    "metal_kq_gemv_iq3s", "MetalKqMvB2Iq3s", "MetalKqMvB4Iq3s", "metal_kq_mvb8_iq3s", "MetalKqMulMmIq3s",
    "metal_kq_gemv_iq3xxs", "MetalKqMvB2Iq3xxs", "MetalKqMvB4Iq3xxs", "metal_kq_mvb8_iq3xxs", "MetalKqMulMmIq3xxs",
    "metal_kq_gemv_iq2s", "MetalKqMvB2Iq2s", "MetalKqMvB4Iq2s", "metal_kq_mvb8_iq2s", "MetalKqMulMmIq2s",
    "metal_kq_gemv_iq2xs", "MetalKqMvB2Iq2xs", "MetalKqMvB4Iq2xs", "metal_kq_mvb8_iq2xs", "MetalKqMulMmIq2xs",
    "MetalKqGemvIq2xxs", "MetalKqMvB2Iq2xxs", "MetalKqMvB4Iq2xxs", "metal_kq_mvb8_iq2xxs", "MetalKqMulMmIq2xxs",
    // the same nine formats' crown-gated forms (the metal-kq-race arc): the f4-slab GEMV twins
    // and the tensor mul_mm T/TH stamps compile only behind a box crown and dispatch only on
    // planes the zoo does not stock. Their coverage is the same kernel-unit gates plus the
    // tensor-twins/iq2xxs matrix row and the mint's METAL_TWIN race rows
    "metal_kq_gemv_iq3s_f4", "metal_kq_gemv_iq3xxs_f4", "MetalKqGemvIq2xxsF4",
    "MetalKqMulMmIq4xsT", "MetalKqMulMmIq4xsTH", "MetalKqMulMmIq4nlT", "MetalKqMulMmIq4nlTH",
    "MetalKqMulMmK3T", "MetalKqMulMmK3TH", "MetalKqMulMmK2T", "MetalKqMulMmK2TH",
    "MetalKqMulMmIq3sT", "MetalKqMulMmIq3sTH", "MetalKqMulMmIq3xxsT", "MetalKqMulMmIq3xxsTH",
    "MetalKqMulMmIq2sT", "MetalKqMulMmIq2sTH", "MetalKqMulMmIq2xsT", "MetalKqMulMmIq2xsTH",
    "MetalKqMulMmIq2xxsT", "MetalKqMulMmIq2xxsTH",
    // the MTP round's kernels: the same-slab verify's per-row argmax dispatches only inside a
    // speculative round (the census prompt never speculates), and the k4 two-row tile only under
    // the kq_mvb2_k4_r2 crown. Their coverage is the argmax_rows gate in test_metal_misc_kernels.das,
    // the r2 arm of kq_mvb_gate plus the pair-walk gate in test_metal_gemv_kernels.das, and the
    // mtp suite's parity arms
    "metal_argmax_rows", "MetalKqMvB2K4R2",
    // the Q4_0 small-batch twins: the gemma4moe row reaches every other q40 dense stamp, but its
    // decode steps are single rows, so the B2/B4/B8 tiles need a batched carrier no zoo row runs.
    // The simdgroup dense mul_mm loses the box race to its own tensor stamp, so it never serves
    // either - its entry carries the method suffix, since the bare class name prefix-matches the
    // T/TH keys the row DOES dispatch. Their coverage is kq_mvb_gate(fmt 40) in
    // test_metal_gemv_kernels.das and kq_mulmm_gate(fmt 40) in test_metal_gemm_kernels.das
    "MetalKqMvB2Q40", "MetalKqMvB4Q40", "MetalKqMvB8Q40", "MetalKqMulMmQ40_metal_kq_mulmm",
    // the dev-W dequant kernels of the split-scale and iquant formats: the image mint's dev-W bake
    // runs one per format, and no stocked census carrier holds a plane in any of them (same zoo
    // quants as above). Their coverage is kq_devw_gate in test_metal_gemm_kernels.das, whose
    // kq_dq_kernel picks the stamp per format
    "MetalKqDequantIq4xs", "MetalKqDequantIq4nl", "MetalKqDequantK3",
    "MetalKqDequantIq3s", "MetalKqDequantIq3xxs", "MetalKqDequantK2", "MetalKqDequantIq2s",
    "MetalKqDequantIq2xs", "MetalKqDequantIq2xxs",
    // the split-format MoE expert mul_mm stamps, all four forms (f32-X base, half-X twin, and the
    // tall pair): no stocked MoE carrier holds an expert plane in these formats - the zoo's MoE
    // carriers put Q4_K_M (k4/k6), Q8_0 and mxfp4 expert planes on the schedule. The half-X twin
    // and the tall pair are covered by moe_mulmm_split_gate in test_metal_gemm_kernels.das; the
    // f32-X base of every one of these formats is NOT - moe_split_twin has no MoeSplitForm.t arm,
    // and the only path to that stamp is a served routed model whose prefill minted no half panel
    // (pf_enc_moe_mm's last arm). followup_metal.md sec.14 owes that arm
    "MetalMoeMulMmK3T", "MetalMoeMulMmK3TH", "MetalMoeMulMmK3TH128", "MetalMoeMulMmK3THR",
    "MetalMoeMulMmIq4xsT", "MetalMoeMulMmIq4xsTH", "MetalMoeMulMmIq4xsTH128", "MetalMoeMulMmIq4xsTHR",
    "MetalMoeMulMmIq3sT", "MetalMoeMulMmIq3sTH", "MetalMoeMulMmIq3sTH128", "MetalMoeMulMmIq3sTHR",
    "MetalMoeMulMmIq3xxsT", "MetalMoeMulMmIq3xxsTH", "MetalMoeMulMmIq3xxsTH128", "MetalMoeMulMmIq3xxsTHR",
    "MetalMoeMulMmIq2sT", "MetalMoeMulMmIq2sTH", "MetalMoeMulMmIq2sTH128", "MetalMoeMulMmIq2sTHR",
    "MetalMoeMulMmIq2xsT", "MetalMoeMulMmIq2xsTH", "MetalMoeMulMmIq2xsTH128", "MetalMoeMulMmIq2xsTHR",
    "MetalMoeMulMmIq2xxsT", "MetalMoeMulMmIq2xxsTH", "MetalMoeMulMmIq2xxsTH128", "MetalMoeMulMmIq2xxsTHR",
    // the q40 expert twins the gemma4moe row cannot reach: the tall pair engages at
    // npos * n_expert_used >= 128 * n_expert = 16384 rows, and its coverage is
    // moe_mulmm_split_gate(fmt 40) in test_metal_gemm_kernels.das. The f32-X base serves only
    // where the prefill minted no half panel (pf_enc_moe_mm's last arm), which no served routed
    // model on this box takes, and moe_split_twin has no MoeSplitForm.t arm for any format - so
    // the T stamp has NO model-less cell, in q40 or in the seven formats above. followup_metal.md
    // sec.14 owes that arm
    "MetalMoeMulMmQ40T_metal_moe_mulmm_iq4xs_t",
    "MetalMoeMulMmQ40TH128_metal_moe_mulmm_iq4xs_t", "MetalMoeMulMmQ40THR_metal_moe_mulmm_iq4xs_t",
    // the expert-indexed GEMVs of the same formats, on the same expert planes nothing stocks.
    // Their coverage is moe_kq_gemv_gate in test_metal_gemv_kernels.das
    "metal_moe_gemv_k3", "metal_moe_gemv_iq4xs", "metal_moe_gemv_iq3s", "metal_moe_gemv_iq3xxs",
    "metal_moe_gemv_iq2s", "metal_moe_gemv_iq2xs", "metal_moe_gemv_iq2xxs",
    // the 16-deep router select stamp: it serves past 256 experts, and the stocked MoE carriers top
    // out at 128 (Qwen3-30B-A3B), 60 (Qwen1.5-MoE) and 32 (gpt-oss-20b). Its coverage is
    // moe_select_gate's ne=512 and ne=300 cases in test_metal_prefill_kernels.das
    "MetalMoeSelect512",
    // the PLE token-table gather twins: the census's only E-series carrier is the E4B Q8_0, whose
    // table sits on the q8 plane and takes the q8 stamp. A k4/k5/q40 table needs an E-series file
    // whose per_layer_token_embd is K-quant or Q4_0, and the zoo stocks none. Their coverage is
    // ple_gather_gate in test_metal_prefill_kernels.das
    "MetalPleGatherK4_metal_ple_gather_kq", "MetalPleGatherK5_metal_ple_gather_kq",
    "MetalPleGatherQ40_metal_ple_gather_kq",
    // the unconverted geglu arm of the bound-total MoE activation: the gemma4moe row picks the
    // geglu family on moe_dense_shexp, and its panel always clears CVT_MIN_ROWS, so the HX twin
    // serves and this one never does. Its coverage is ew2_hx_gate(geglu = true, bounded = true) in
    // test_metal_misc_kernels.das
    "MetalGegluB",
    // the transposed im2col: only a transposed conv whose channel count is off the GEMM's 8-run
    // lattice takes it, and every stocked StyleTTS2 and Pocket transposed conv sits on the lattice,
    // so the gathering CONV stamp serves. Its coverage is st2_im2col_gate(transposed = true) in
    // test_metal_prefill_kernels.das
    "metal_st2_im2col_tr"
]

def private counting_prompt(start, last : int) : string {
    return build_string() $(var w) {
        for (i in range(start, last + 1)) {
            w |> write(i)
            w |> write(i < last ? ", " : ",")
        }
    }
}

def private greedy_top(lg : array<float>; vocab : int64) : int64 {
    var best = 0l
    for (v in range64(1l, vocab)) {
        if (lg[v] > lg[best]) {
            best = v
        }
    }
    return best
}

[unused_argument(t, path, family, kdts, nbs)]   // off-Apple the gated body is empty
def private cov_model(t : T?; path, family : string; kdts : array<KVDtype>; nbs : array<int>) {
    // one model's coverage sweep: per KV codec — GPU prefill (the ~120-id counting prompt
    // clears the npos gate), two greedy decode steps, then one batched step per requested B.
    // No parity asserts here — the census wants DISPATCH DIVERSITY, the parity suites own truth.
    static_if (typeinfo builtin_module_exists(das_metal)) {
        if (!family_on(t, family) || !arm_on(t, "coverage") || !model_available(t, path)) {
            return
        }
        var tr <- load_model_(path, QuantMode.q8)
        if (!convert_model_to_metal_blob(tr)) {
            t |> failure("{base_name(path)}: metal-blob transform failed")
            delete tr
            return
        }
        // 1024, not 512: the digit-splitting tokenizers (gemma, qwen) encode the 140-number
        // counting prompt to ~590 tokens, and the eval capacity tripwire panics past seq_len
        tr.config.seq_len = min(tr.config.seq_len, 1024l)
        let vocab = tr.config.vocab_size
        select_prefill_override("metal")
        select_decode_override("metal")
        with_job_que() {
            setup_dasllama_jobque()
            for (kdt in kdts) {
                var s = make_run_state(tr.config, kdt, kdt)
                // >= 256 rows: the half-X convert (CVT_MIN_ROWS), the tall M-tile stamps and
                // the MoE gather engage only past their row floors — a short prompt leaves
                // every one of those kernels undispatched and the census blind to them
                let p <- encode(tr, counting_prompt(1, 140), true, false)
                eval_(tr, s, p)
                var cur = greedy_top(s.logits, vocab)
                for (_i in range(2)) {
                    var one <- [cur]
                    eval_(tr, s, one)
                    delete one
                    cur = greedy_top(s.logits, vocab)
                }
                // continuation prefill (start_pos > 0) — the pf window-landing utilities only
                // serve when a second window lands on an already-warm cache
                var p2 <- encode(tr, counting_prompt(50, 90), false, false)
                eval_(tr, s, p2)
                delete p2
                delete s
                for (nb in nbs) {
                    var ss : array<Session>
                    ss |> reserve(nb)
                    var sess : array<Session?>
                    var curs : array<int64>
                    for (i in range(nb)) {
                        ss |> emplace <| make_run_state(tr.config, kdt, kdt)
                        let pb <- encode(tr, counting_prompt(3 + i, 44 + i), true, false)
                        eval_(tr, ss[i], pb)
                        sess |> push(unsafe(addr(ss[i])))
                        curs |> push(greedy_top(ss[i].logits, vocab))
                    }
                    var ws <- create_batch_workspace_(tr)
                    eval_batch_(tr, ws, sess, curs)
                    sess |> clear()   // borrowed handles — delete of array<T?> cascades to the pointees
                    unsafe {
                        delete ws
                        delete sess
                    }
                    delete curs
                    delete ss
                }
            }
        }
        select_prefill_override("")
        select_decode_override("")
        delete tr
    }
}

def private census_zoo(t : T?) {
    // llama: the dim-ladder base — all three KV codecs, the B=2/4/8 batch shapes
    cov_model(t, path_join(models_dir(), "Llama-3.2-1B-Instruct-Q8_0.gguf"), "llama",
        [KVDtype.f16, KVDtype.q8_0, KVDtype.tq4], [2, 4, 8])
    // the s16 axis (f16 weight-scale plane) on the same carrier
    set_wscale_f16(true)
    cov_model(t, path_join(models_dir(), "Llama-3.2-1B-Instruct-Q8_0.gguf"), "llama",
        [KVDtype.f16], [2])
    set_wscale_f16(false)
    // the q5_K tail on the same carrier — Q5_K_M rows drive the k5 GEMV/mulmm/batch tiles
    cov_model(t, path_join(models_dir(), "Llama-3.2-1B-Instruct-Q5_K_M.gguf"), "llama",
        [KVDtype.f16], [2, 4, 8])
    // gemma2: K-quant weights x softcap/sliding/geglu/wide heads; B=4/8 drive the
    // k4/k6 batch tiles
    cov_model(t, path_join(models_dir(), "gemma-2-2b-it-Q4_K_M.gguf"), "gemma2",
        [KVDtype.f16, KVDtype.q8_0], [2, 4, 8])
    // qwen3: qk_norm + qd != dim
    cov_model(t, path_join(models_dir(), "Qwen3-0.6B-Q8_0.gguf"), "qwen3",
        [KVDtype.f16, KVDtype.tq4], [2])
    // qwen2: qkv_bias
    cov_model(t, path_join(models_dir(), "Qwen2.5-0.5B-Instruct-Q8_0.gguf"), "qwen2",
        [KVDtype.f16, KVDtype.q8_0], [2])
    // qwen3.5: the DeltaNet hybrid (batch = per-row fallback by design — still coverage)
    cov_model(t, path_join(models_dir(), "Qwen3.5-0.8B-Q8_0.gguf"), "qwen35",
        [KVDtype.f16], [2])
    // gemma4e: PLE + V-from-K (no PLE batch arm — a blob-twin CPU batch fallback
    // would trip the blob-only panic, so no batch shapes here); large tier
    let none : array<int>
    cov_model(t, path_join(models_dir(), "gemma-4-E4B-it-Q8_0.gguf"), "gemma4e",
        [KVDtype.f16], none)
    // gemma4: the checkpoint suppress list (suppress_tokens metadata rides the 12B
    // family, NOT the E-series) + K-quant at 12B shapes; large tier
    cov_model(t, path_join(models_dir(), "gemma-4-12B-it-Q4_K_M.gguf"), "gemma4",
        [KVDtype.f16], [2])
    // qwen3moe: the whole-graph GPU MoE family (router/bucket/select/combine +
    // expert GEMV/GEMMs); no batch shapes — MoE batch declines are graph-level and a
    // blob-twin CPU batch fallback would trip the blob-only panic; large tier
    cov_model(t, path_join(models_dir(), "Qwen3-30B-A3B-Instruct-2507-Q4_K_M.gguf"), "qwen3moe",
        [KVDtype.f16], none)
    // gptoss: the MXFP4 expert stacks + swiglu_oai + attention sinks — the only carrier for the
    // mx4 expert kernels; no batch shapes for the same reason as the other MoE rows; large tier
    cov_model(t, path_join(models_dir(), "gpt-oss-20b-mxfp4.gguf"), "gptoss",
        [KVDtype.f16], none)
    // qwen2moe: q8 expert stacks + the sigmoid-gated shared expert on std attention; large tier
    cov_model(t, path_join(models_dir(), "Qwen1.5-MoE-A2.7B-Chat.Q8_0.gguf"), "qwen2moe",
        [KVDtype.f16], none)
    // gemma4moe: the only carrier of a Q4_0 routed expert plane - the gemma-4 dense-shared MoE
    // whose fused gate_up stacks are Q4_0 (the 704-wide down stacks demote to q8: no multiple of
    // 256). No batch shapes, same reason as the other MoE rows; large tier
    cov_model(t, path_join(models_dir(), "gemma-4-26B-A4B-it-Q4_0.gguf"), "gemma4moe",
        [KVDtype.f16], none)
    cov_tower(t)
}

// the gemma3v row of cov_tower: the fixed 896 canvas through the gemma-3-4b SigLIP tower on
// the f32 planes, BOTH attention routes — the 4096-token %64 canvas serves the tower flash
// (flash + kv_hc), then a flash-off encode drives the slab route (head_restride + padded QK/AV)
[unused_argument(t)]   // off-Apple the gated body is empty
def private cov_tower_gemma3v(t : T?) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let vpath = path_join(models_dir(), "mmproj-gemma-3-4b-it-F16.gguf")
        if (family_on(t, "gemma3v") && model_available(t, vpath)) {
            set_gemma3v_q8(false)   // the driver serves f32 planes (the lane policy picks this with the tower on; pinned for the census)
            var inscope g3stg <- stage_gemma3v_tower(vpath)   // stage+mint: no .dlim written, no sibling-lane GC
            var inscope tw = Gemma3vTower()
            mint_gemma3v_tower(g3stg, tw)
            reset_gemma3v_q8()
            var st = Gemma3vState()
            var planes : array<float>
            planes |> reserve_resize(896l * 896l * 3l)
            for (i in range64(896l * 896l * 3l)) {
                planes[i] = sin(float(i) * 0.017) * 0.5 + 0.5
            }
            var out : array<float>
            let prev_flash = get_metal_tower_flash()
            with_job_que() {
                setup_dasllama_jobque()
                gemma3v_encode(tw, st, planes, 896l, 896l, out)
                set_metal_tower_flash(false)
                var out2 : array<float>
                gemma3v_encode(tw, st, planes, 896l, 896l, out2)
                set_metal_tower_flash(prev_flash)
                delete out2
            }
            delete out
            delete st
            // the crowned twin-W leg: a fresh load under pinned crowns bakes the halfword
            // twin, so the f16-route kernels (cvt_half + the half GEMMs) dispatch - the PIN
            // form, since the load re-applies the box profile's crowns over a bare setter
            let prev_crowns = get_metal_tensor_crowns()
            let prev_tower_f16 = get_metal_tower_f16()
            pin_metal_tensor_crowns("mulmm_q8")
            metal_prefill_shutdown()
            set_metal_tower_f16(true)
            set_gemma3v_q8(false)   // the exact lane is the one that bakes the twin
            var inscope g3stg2 <- stage_gemma3v_tower(vpath)   // in-memory: the crowned bake must not purge the serving flavor
            var twt = Gemma3vTower()
            mint_gemma3v_tower(g3stg2, twt)
            reset_gemma3v_q8()
            var stt = Gemma3vState()
            var outt : array<float>
            with_job_que() {
                setup_dasllama_jobque()
                gemma3v_encode(twt, stt, planes, 896l, 896l, outt)
            }
            unpin_metal_tensor_crowns()
            set_metal_tensor_crowns(prev_crowns)
            set_metal_tower_f16(prev_tower_f16)
            metal_prefill_shutdown()
            delete outt
            delete stt
            delete twt
            delete planes
        }
    }
}

// the qwen25v row of cov_tower: one 112x112 window (16 tokens) through the Qwen2.5-Omni
// window ViT — the per-window attention kernel + the row-gather stem sort
[unused_argument(t)]   // off-Apple the gated body is empty
def private cov_tower_qwen25v(t : T?) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let vpath = path_join(models_dir(), "mmproj-Qwen2.5-Omni-3B-f16.gguf")
        if (family_on(t, "qwen25v") && model_available(t, vpath)) {
            var inscope q25stg <- stage_qwen25v_tower(vpath)   // stage+mint: no .dlim written, no sibling-lane GC
            var tw = Qwen25vTower()
            mint_qwen25v_tower(q25stg, tw)
            var st = Qwen25vState()
            var planes : array<float>
            planes |> reserve_resize(112l * 112l * 3l)
            for (i in range64(112l * 112l * 3l)) {
                planes[i] = sin(float(i) * 0.019) * 0.5 + 0.5
            }
            var out : array<float>
            with_job_que() {
                setup_dasllama_jobque()
                qwen25v_encode(tw, st, planes, 112l, 112l, out)
            }
            delete out
            delete planes
            delete st
            delete tw
        }
    }
}

// the qwen3v row of cov_tower: a 96x96 canvas through the Omni-family merger tower — the
// fused GPU stem (patch GEMM + merge-walk assemble) ahead of the block slab
[unused_argument(t)]   // off-Apple the gated body is empty
def private cov_tower_qwen3v(t : T?) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let vpath = path_join(models_dir(), "mmproj-Qwen3VL-4B-Instruct-F16.gguf")
        if (family_on(t, "qwen3v") && model_available(t, vpath)) {
            set_qwen3v_q8(false)   // pinned for the census — the box's lane policy must not pick the row's kernel set
            var inscope q3stg <- stage_qwen3v_tower(vpath)   // stage+mint: no .dlim written, no sibling-lane GC
            var tw = Qwen3vTower()
            mint_qwen3v_tower(q3stg, tw)
            reset_qwen3v_q8()
            var st = Qwen3vState()
            var planes : array<float>
            planes |> reserve_resize(96l * 96l * 3l)
            for (i in range64(96l * 96l * 3l)) {
                planes[i] = sin(float(i) * 0.023) * 0.5 + 0.5
            }
            var out : array<float>
            with_job_que() {
                setup_dasllama_jobque()
                qwen3v_encode(tw, st, planes, 96l, 96l, out)
            }
            delete out
            delete planes
            delete st
            delete tw
        }
    }
}

// the gemma4a row of cov_tower: one whole chunk from raw samples — GPU mel (twiddle-T DFT
// GEMM + mag + filter + log), both subsample convs, and the 12-block Conformer chain
[unused_argument(t, samples)]   // off-Apple the gated body is empty
def private cov_tower_gemma4a(t : T?; samples : array<float>) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let gpath = path_join(models_dir(), "mmproj-gemma-4-E2B-it-bf16.gguf")
        if (family_on(t, "gemma4a") && model_available(t, gpath) && !empty(samples)) {
            set_gemma4a_q8(false)   // pinned for the census — the box's lane policy must not pick the row's kernel set
            var inscope gstg <- stage_gemma4a_encoder(gpath)   // stage+mint: no .dlim written, no sibling-lane GC
            var inscope genc = Gemma4aEncoder()
            mint_gemma4a_encoder(gstg, genc)
            reset_gemma4a_q8()
            var gst = Gemma4aState()
            var grows : array<float>
            with_job_que() {
                setup_dasllama_jobque()
                gemma4a_encode_all(genc, gst, samples, grows)
            }
            delete grows
            delete gst
        }
    }
}

// the canary row of cov_tower: CPU mel, then the 32-block Metal FastConformer chain — the
// bidirectional rel-pos XL attention and the centered-k9 depthwise kernels
[unused_argument(t, samples)]   // off-Apple the gated body is empty
def private cov_tower_canary(t : T?; samples : array<float>) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let cnpath = path_join(models_dir(), "canary-qwen-2.5b-encoder-f32.bin")
        if (family_on(t, "canary") && model_available(t, cnpath) && !empty(samples)) {
            var inscope cstg <- stage_canary_encoder(cnpath)   // stage+mint: no .dlim written, no sibling-lane GC
            var inscope cenc = CanaryEncoder()
            mint_canary_encoder(cstg, cenc)
            var cst = CanaryState()
            with_job_que() {
                setup_dasllama_jobque()
                canary_log_mel(cenc, samples, cst)
                canary_encode(cenc, cst)
            }
            delete cst
        }
    }
}

//! reaches the tap-major depthwise stamp (enc_pk_dw), which the canary row does not
[unused_argument(t, samples)]   // off-Apple the gated body is empty
def private cov_tower_parakeet(t : T?; samples : array<float>) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let pkpath = path_join(whisper_dir(), "ggml-parakeet-tdt-0.6b-v2-f32.bin")
        if (family_on(t, "parakeet") && model_available(t, pkpath) && !empty(samples)) {
            var inscope pstg <- stage_parakeet_model(pkpath, false)
            var inscope pm = ParakeetModel()
            mint_parakeet_model(pstg, pm, false)
            var pst = ParakeetState()
            with_job_que() {
                setup_dasllama_jobque()
                parakeet_log_mel(samples, pm, pst)
                parakeet_encode(pm, pst)
            }
            delete pst
        }
    }
}

// the gemma4v row of cov_tower: a 96x96 canvas through the E2B ViT tower on the file's planes
[unused_argument(t)]   // off-Apple the gated body is empty
def private cov_tower_gemma4v(t : T?) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let vpath = path_join(models_dir(), "mmproj-gemma-4-E2B-it-bf16.gguf")
        if (model_available(t, vpath)) {
            set_gemma4v_q8(false)   // the driver serves the file's planes (the lane policy picks this with the tower on; pinned for the census)
            var inscope g4stg <- stage_gemma4v_tower(vpath)   // stage+mint: no .dlim written, no sibling-lane GC
            var inscope tw = Gemma4vTower()
            mint_gemma4v_tower(g4stg, tw)
            reset_gemma4v_q8()
            var st = Gemma4vState()
            var planes : array<float>
            planes |> resize(96l * 96l * 3l)
            for (i in range64(96l * 96l * 3l)) {
                planes[i] = sin(float(i) * 0.017) * 0.5 + 0.5
            }
            var out : array<float>
            with_job_que() {
                setup_dasllama_jobque()
                gemma4v_encode(tw, st, planes, 96l, 96l, out)
            }
            delete out
            delete planes
            delete st
        }
    }
}

// cov_tower: the tower-driver kernels (LN / f32 mul_mm / the gelu flavors / posadd / the
// borrowed trio / the gemma4v clamp, rope2d, GEGLU-quick) AND the decoder-side cross-KV kernels
// go non-zero — a whisper tiny MIXED chunk (f32 enc = the tower LUT flavor, q8 dec = the wdec
// driver) plus one row per tower family, each row's own comment naming the kernels it reaches
[unused_argument(t)]   // off-Apple the gated body is empty
def private cov_tower(t : T?) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        set_metal_tower(true)
        set_metal_wdec(true)
        set_metal_wdec_step(true)
        set_metal_wdec_step_min_d(0l)   // the census carrier is tiny — under the serving floor
        let wpath = path_join(whisper_dir(), "ggml-tiny.bin")
        var samples <- read_wav_pcm16_mono(path_join(models_dir(), "jfk.wav"))
        if (model_available(t, wpath) && !empty(samples)) {
            var inscope w = WhisperModel()
            var inscope stg <- stage_whisper_model(wpath, false, true)
            mint_whisper_model(stg, w, false, true)
            with_job_que() {
                setup_dasllama_jobque()
                var ws <- create_session(w)
                transcribe(w, ws, samples) $(_seg) {
                    pass
                }
                delete ws
            }
        }
        // the erf-GELU carrier (whisper is the LUT flavor): one qwen2audio chunk on varied mel
        let apath = path_join(models_dir(), "qwen2audio-mmproj-f32.gguf")
        if (model_available(t, apath)) {
            var atw <- load_audio_tower(apath)
            var mel = MelChunk(n_mel = atw.n_mel, n_len = AUDIO_CHUNK_FRAMES)
            mel.data |> resize(atw.n_mel * AUDIO_CHUNK_FRAMES)
            for (i in range(length(mel.data))) {
                mel.data[i] = sin(float(i) * 0.0137) * 0.8
            }
            var aenc <- make_encoder_state(atw)
            with_job_que() {
                setup_dasllama_jobque()
                audio_encode(atw, aenc, mel)
            }
            delete aenc
            delete atw
        }
        // the qwen3a conv2d + mel carrier: the mel is built from raw samples so the GPU mel hook
        // (twiddle-T DFT GEMM + power + mel-major log store) dispatches ahead of the im2col2d
        // window and its padded tile GEMMs
        let qpath = path_join(models_dir(), "mmproj-Qwen3-ASR-0.6B-bf16.gguf")
        if (model_available(t, qpath) && !empty(samples)) {
            var inscope qtw <- load_qwen3a_tower(qpath)
            var qenc <- make_encoder_state(qtw.core)
            var qcs = Qwen3aConvState()
            var qrows : array<float>
            with_job_que() {
                setup_dasllama_jobque()
                var qmel <- log_mel_qwen3a(samples, qtw.core.n_mel)
                qwen3a_encode_all(qtw, qenc, qcs, qmel, qrows)
                delete qmel
            }
            delete qrows
            delete qcs
            delete qenc
        }
        cov_tower_gemma4a(t, samples)
        cov_tower_canary(t, samples)
        cov_tower_parakeet(t, samples)
        delete samples
        cov_tower_gemma4uv(t)
        cov_tower_gemma4v(t)
        cov_tower_gemma3v(t)
        cov_tower_qwen25v(t)
        cov_tower_qwen3v(t)
        cov_tower_styletts2(t)
        cov_tower_pocket(t)
    }
}

[unused_argument(t)]   // without a GPU module the gated body is empty
def private cov_tower_styletts2(t : T?) {
    static_if (typeinfo builtin_module_exists(das_metal) || typeinfo builtin_module_exists(vulkan)) {
        for (name in ["kitten-nano.gguf", "kokoro-82m.gguf"]) {   //! the ONNX law's stamps and the torch law's
            let path = path_join(models_dir(), name)
            continue if (!family_on(t, name == "kokoro-82m.gguf" ? "kokoro" : "kitten") || !model_available(t, path))
            var inscope stg <- stage_styletts2(path, false)   // stage+mint: no .dlim written, no sibling-lane GC
            var inscope m = StyleTts2Model()
            mint_styletts2(stg, m)
            var inscope ids : array<int>
            ids |> resize(48)
            for (i in range(48)) {
                ids[i] = 1 + i % (int(m.n_symbols) - 1)
            }
            var inscope style : array<float>
            styletts2_style(m, m.voice_names[0], 0, style)
            var inscope noise = TtsNoise()
            var tm = TtsTimings()
            var inscope sc = St2Scratch()
            with_job_que() {
                setup_dasllama_jobque()
                styletts2_synthesize(m, ids, style, 1.0, noise, 0x5eedul, null, tm, sc)
            }
        }
    }
}

[unused_argument(t)]   //! without a GPU module the gated body is empty
def private cov_tower_pocket(t : T?) {
    static_if (typeinfo builtin_module_exists(das_metal) || typeinfo builtin_module_exists(vulkan)) {
        let path = path_join(models_dir(), "pocket-tts-en.gguf")
        if (family_on(t, "pocket") && model_available(t, path)) {
            var inscope m <- load_pocket(path)   //! the Pocket loader stages in memory: no .dlim
            var inscope sc = PocketScratch()
            let frames = 24l
            var inscope lat : array<float>
            lat |> resize(frames * m.latent_dim)
            for (i in range(length(lat))) {
                lat[i] = 0.1 * float(i % 17) - 0.8
            }
            var inscope wave : array<float>
            var tm = TtsTimings()
            with_job_que() {
                setup_dasllama_jobque()
                pocket_decode_latents(m, lat, frames, sc, wave)
                pocket_speak(m, "The census row speaks one short line.", m.default_voice, m.default_temperature, 1ul, tm, sc)
                let kq = path_join(models_dir(), "pocket-tts-en-kq.gguf")   //! the small form: its head's q8 linears take the q8 stamps
                if (model_available(t, kq)) {
                    var inscope mk <- load_pocket(kq)
                    var inscope sck = PocketScratch()
                    pocket_speak(mk, "The small form speaks the same line.", mk.default_voice, mk.default_temperature, 1ul, tm, sck)
                }
            }
        }
    }
}

// the gemma4uv row of cov_tower: a small canvas through the 12B dense embedder chain
[unused_argument(t)]   // off-Apple the gated body is empty
def private cov_tower_gemma4uv(t : T?) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let mpath = path_join(models_dir(), "mmproj-gemma-4-12B-it-BF16.gguf")
        if (model_available(t, mpath)) {
            var inscope emb <- load_gemma4uv_embedder(mpath)
            var st = Gemma4uvState()
            var planes : array<float>
            planes |> resize(96l * 96l * 3l)
            for (i in range64(96l * 96l * 3l)) {
                planes[i] = sin(float(i) * 0.013) * 0.7
            }
            var out : array<float>
            with_job_que() {
                setup_dasllama_jobque()
                gemma4uv_encode(emb, st, planes, 96l, 96l, out)
            }
            delete out
            delete planes
            delete st
        }
    }
}

//! Blind entries whose stamp compiles only behind a box crown, and the crown that reaches them:
//! where the crown IS pinned a stocked carrier dispatches the stamp, so the drain assert would red
//! on a correct box. `CENSUS_NEVER_DISPATCHED` has no vocabulary for "blind only off this crown",
//! so the pair is named here.
let private CENSUS_CROWN_CONDITIONAL : table<string; string> <- {"MetalKqMvB2K4R2" => "kq_mvb2_k4_r2"}

//! Did this run compile the GATED stamp families the blind list names? Two gates decide it, both
//! read from the run itself rather than re-probed: the Metal-4 tensor toolchain - the prefill's PSO
//! init puts every tensor stamp behind one probe, so the dense q8 tensor family's presence in the
//! census answers for all of them - and the box's tensor crowns, since every crown-gated twin
//! compiles only behind `metal_tensor_crowned` and an empty crown list means none of them exist.
def private census_gated_stamps_compiled(cov : table<string; int64>) : bool {
    if (empty(get_metal_tensor_crowns())) {
        return false
    }
    for (k in keys(cov)) {
        if (starts_with(k, "MetalQ8MulMmT")) {
            return true
        }
    }
    return false
}

//! The blind-spot list self-drains: census keys are the compiled kernels' MSL entry names, so a
//! declared entry whose key counted dispatches is a STALE entry, and one matching no key at all
//! names nothing and would sit green forever. A box that compiled none of the gated stamp families
//! cannot tell that second case from a misspelt key, so it SKIPS the entry loudly, never passes it.
def private metal_census_blind_drain(t : T?; cov : table<string; int64>) {
    let judge_unmatched = census_gated_stamps_compiled(cov)
    for (blind in CENSUS_NEVER_DISPATCHED) {
        var hits = 0l
        var named = false
        for (k, n in keys(cov), values(cov)) {
            if (starts_with(k, blind)) {
                hits += n
                named = true
            }
        }
        if (!named && !judge_unmatched) {
            t |> skip("{blind}: this box compiled no tensor or crowned stamp, so an entry matching no census key cannot be told from a misspelt one here")
            continue
        }
        let crown = CENSUS_CROWN_CONDITIONAL?[blind] ?? ""
        if (hits > 0l && !empty(crown) && metal_tensor_crowned(crown)) {
            t |> skip("{blind}: the {crown} crown is pinned on this box, so a stocked carrier reaches it ({hits}) - the entry is blind only off that crown")
            continue
        }
        t |> success(named,
            "{blind} names no compiled kernel - spell the census key (the class's own shader entry name, or <Class>_<kernel method>)")
        t |> success(hits == 0l,
            "{blind} is a DECLARED blind spot - a stocked carrier now dispatches it ({hits}): drop the entry, add its census row")
    }
}

[test]
def test_metal_kernel_coverage(tst : T?) {
    tst |> run("Metal kernel coverage census: the small-model zoo across format/graph/batch/KV axes") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            if (!arm_on(t, "coverage")) {
                return
            }
            if (!jit_enabled()) {
                t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
                return
            }
            var dev = metal_create_system_default_device()
            if (dev == null) {
                feint("no Metal device; kernel coverage census skipped\n")
                return
            }
            metal_release(dev)
            let prior_wscale = get_wscale_f16()
            let prior_backend = active_kernel_backend()
            set_wscale_f16(false)
            pin_kernel_backend("portable")   // row-major weights — the blob transform's donor contract
            metal_kernel_coverage_reset()
            census_zoo(t)
            // the census verdict: the report prints counts + LOUD WARNINGS for the zeros;
            // the assert pins only that the RAIL counts — coverage gaps are human calls
            metal_kernel_coverage_report()
            var cov <- metal_kernel_coverage()
            metal_census_blind_drain(t, cov)
            var served = 0
            for (n in values(cov)) {
                if (n > 0l) {
                    served++
                }
            }
            let famfilter = g_env_test.test_family
            if (empty(famfilter)) {
                t |> success(served >= 40,
                    "the census counted {served} distinct dispatched kernels (rail floor 40)")
            } else {
                // a family filter narrows the zoo by design — the floor only holds unfiltered
                to_log(LOG_INFO, "census floor not asserted (family filter '{famfilter}'): {served} served\n")
            }
            delete cov
            metal_tower_shutdown()
            metal_decode_shutdown()
            clear_kernel_backend_pin()
            select_kernel_backend(prior_backend)
            set_wscale_f16(prior_wscale)
        } else {
            feint("das_metal is not built on this platform; nothing to census\n")
        }
    }
}

// The Vulkan census's DECLARED blind spots, by census name (`<vk_dispatch name>_spv`): kernel
// classes no zoo row below can dispatch, each with the reason and the kernel-unit cell that does.
let private VK_CENSUS_NEVER_DISPATCHED <- [
    // the q51 expert rail (the 32-block down planes of the 26B-A4B's Q4_K_M: a carrier above the zoo's tier) -
    // its s and e stamps and its decode GEMV; their coverage is test_vkd_q51_cm2_batch and test_vkd_q51_gemv
    "q51_batch_cm2s_cls_spv", "q51_batch_cm2e_cls_spv", "q51_gemv_cls_spv",
    // the mx4 expert rail (gpt-oss's native-MXFP4 routed stacks: a carrier above the zoo's tier) - its s and e stamps and
    // its decode GEMV; their coverage is test_vkd_mx4_cm2_batch and test_vkd_mx4_gemv
    "mx4_batch_cm2s_cls_spv", "mx4_batch_cm2e_cls_spv", "mx4_gemv_cls_spv",
    // the per-32 expert rails' KHR arms (the expert schedule on a KHR-mode card: the 26B-A4B Q4_K_M's q51 down stacks, gpt-oss's
    // mx4 stacks - carriers above the zoo's tier); their coverage is the KHR arm of test_vkd_q51_cm2_batch and test_vkd_mx4_cm2_batch
    "q51_batch_khr_cls_spv", "mx4_batch_khr_cls_spv",
    // the mx4 experts' fused gate+up+act+requant GEMV (gpt-oss's token command, above the zoo's tier); its coverage is
    // test_vkd_mx4_gemv_gu in test_vulkan_kernels.das
    "mx4_gemv_gu_cls_spv",
    // the biased act twins (gpt-oss's expert stacks carry gate and up bias rows: a carrier above the zoo's tier) - the
    // Q8_0 requant form the token command runs and the f16 form the window chain runs; their coverage is test_vkd_act_family
    "q8_actrq_b_cls_spv", "actf16_b_cls_spv",
    // the attention-sink twins (gpt-oss's per-head sink logits, a carrier above the zoo's tier): the h64 flash tiles on both
    // arms with their f16-out twins, the token command's pass; their coverage is the sink arms of
    // test_vkd_fa_cm2, test_vkd_fa_khr and test_vkd_da_attn in test_vulkan_kernels.das
    "fa_cm2_h64_sink_cls_spv", "fa_cm2_h64_sink_f16_cls_spv", "fa_khr_h64_sink_cls_spv", "fa_khr_h64_sink_f16_cls_spv",
    "da_attn_sink_cls_spv", "da_attn_f16_sink_cls_spv",
    // the token command's f32-mirror attention stamps (the four-head slab and the two-head one): the census steps every
    // session on the f16 mirror the tier arms, the f32 twin serves under DASLLAMA_VK_KV32=1 (gemma-2-2b Q8_0's f32 parity
    // run dispatches both); their coverage is test_vkd_da_attn and test_vkd_da_attn_rqk in test_vulkan_kernels.das
    "da_attn_cls_spv", "da_attn_g2_cls_spv",
    // the E-series per-layer-embedding tail (the act, its requant and the proj GEMV in one): its carriers are the
    // gemma-4 E2B / E4B, no zoo row; its coverage is test_vkd_q8_gemv_pleact in test_vulkan_kernels.das
    "q8_gemv_pleact_cls_spv",
    // the KHR flash-attention tile's 256 and 512 heads: the mode sweep's mm row reaches the 64 and 128 heads through the
    // zoo's qwens, and no zoo row carries a wider head (the gemmas sit above the tier); their coverage is test_vkd_fa_khr
    // and test_vkd_fa_khr_h256_softcap in test_vulkan_kernels.das
    "fa_khr_h256_cls_spv", "fa_khr_h256_f16_cls_spv", "fa_khr_h256_cap_cls_spv", "fa_khr_h256_cap_f16_cls_spv",
    "fa_khr_h512_cls_spv", "fa_khr_h512_f16_cls_spv",
    // the bidirectional flash tiles (the vision towers' encoder attention): the 64 head is the gemma4v tower's (the stocked
    // mmproj-gemma-4-E2B-it-bf16.gguf, dispatched by test_gemma4v_vulkan_twin in test_gemma4v.das), the padded 128 the
    // gemma3v / qwen3v towers' (mmproj-gemma-3-4b-it-F16.gguf via test_gemma3v_vulkan_twin, mmproj-Qwen3VL-4B-Instruct-F16.gguf
    // via test_qwen3v_vulkan_twin) - no zoo row; their kernel coverage is test_vkt_fa_bidir in test_vulkan_tower_kernels.das
    "fa_cm2_h64_bidir_cls_spv", "fa_cm2_h128_bidir_cls_spv", "fa_khr_h64_bidir_cls_spv", "fa_khr_h128_bidir_cls_spv",
    // the audio fronts' classes (the Vulkan tower driver's mel and conv seats - no zoo row, no audio carrier in the census);
    // the carriers are the stocked mmproj-Qwen3-ASR-0.6B-bf16.gguf (test_qwen3a_vulkan_front in test_audio.das),
    // mmproj-gemma-4-E2B-it-bf16.gguf (test_gemma4a_vulkan_twin) and canary-qwen-2.5b-encoder-f32.bin (test_canary_vulkan_twin);
    // every covering cell below is in test_vulkan_tower_kernels.das
    // the spectrum pair (Qwen3-ASR's mel); its coverage is test_vkt_tower_front_mel
    "tower_spec_dft_cls_spv",
    // the mel-major log store (Qwen3-ASR's mel); its coverage is test_vkt_tower_front_mel
    "tower_mel_log_cls_spv",
    // the qwen3a front's k3 s2 p1 im2col; its coverage is test_vkt_tower_front_movers
    "tower_im2col_cls_spv",
    // the qwen3a front's conv-out shuffle; its coverage is test_vkt_tower_front_movers
    "tower_q3a_shuffle_cls_spv",
    // the LayerNorm + ReLU rows of the gemma4a subsample convs; its coverage is test_vkt_tower_front_movers
    "tower_ln_relu_cls_spv",
    // canary's per-feature mel normalization; its coverage is test_vkt_tower_front_mel
    "tower_mel_norm_cls_spv",
    // canary's front depthwise conv; its coverage is test_vkt_tower_front_mel
    "tower_dw_conv_cls_spv",
    // canary's feature rows (the feature shuffle); its coverage is test_vkt_tower_front_movers
    "tower_feat_shuffle_cls_spv",
    // canary's sinusoid rel table; its coverage is test_vkt_tower_front_mel
    "tower_rel_sincos_cls_spv",
    // canary's rel-plane attention (the tiled online softmax at both head widths; the head panels ahead of the rel GEMM are the
    // restride stamps at one head): the canary twin dispatches the 128-wide stamp (canary's head width); the 64-wide stamp
    // serves no stocked carrier; their kernel coverage is test_vkt_tower_cn_attn_rel_plane (both stamps) in
    // test_vulkan_tower_kernels.das
    "tower_cn_attn_r_cls_spv", "tower_cn_attn_r64_cls_spv",
    // the whisper decoder's classes (the Vulkan ASR-decoder driver - no zoo row, no whisper carrier in the census): the K/V
    // stores and the chunked attention's two halves, the stocked ggml-tiny.bin and ggml-large-v3-turbo.bin via
    // test_whisper_vulkan_wdec in test_whisper.das; their kernel coverage is test_vkt_wdec_classes in
    // test_vulkan_tower_kernels.das
    "tower_wdec_kv_cls_spv", "tower_wdec_attn_part_cls_spv", "tower_wdec_attn_comb_cls_spv",
    // the decode step's fused row passes (the layernorm, the post-add with its next norm, the bias + tanh GELU and the attention
    // combine, each landing its Q8_0 feed): the same two stocked files via test_whisper_vulkan_wdec; their kernel coverage is
    // test_vkt_wdec_fused_rq in test_vulkan_tower_kernels.das
    "tower_ln_rq_cls_spv", "tower_post_add_ln_rq_cls_spv", "tower_bias_act_rq_cls_spv", "tower_wdec_attn_comb_rq_cls_spv",
    // the whisper-class conv stem's classes (the tower driver's conv seat - no zoo row): the k3 im2col and the position rows'
    // add, the same two stocked whisper files via test_whisper_vulkan_twin in test_whisper.das; their kernel coverage is
    // test_vkt_tower_front_movers in test_vulkan_tower_kernels.das
    "tower_im2col1d_cls_spv",
    // the pre-LN chain's f16-feed stamps (the cm2 arm: the norms and seams storing the feed, the biased restrides, the unpad and
    // the bias + GELU storing it): the whisper twin and the gemma3v twin (mmproj-gemma-3-4b-it-F16.gguf) dispatch them; their
    // kernel coverage is test_vkt_tower_f16_feeds in test_vulkan_tower_kernels.das
    "tower_ln16_cls_spv", "tower_post_add_ln16_cls_spv", "tower_hpad_b_cls_spv", "tower_hpad16_b_cls_spv", "tower_hunpad16_cls_spv",
    "tower_bias_act_cvt16_cls_spv",
    // the gemma4v tower's row classes (the norms, clamps, feeds, the two-axis rope, the post-add and the GEGLU): the stocked
    // mmproj-gemma-4-E2B-it-bf16.gguf, dispatched by test_gemma4v_vulkan_twin in test_gemma4v.das - no zoo row; their kernel
    // coverage is test_vkt_tower_rows in test_vulkan_tower_kernels.das (the clamp + halfword feed also serves every audio
    // chain's cm2 feed, dispatched by the audio twins; test_vkt_tower_front_movers covers it too)
    "tower_rms_cls_spv", "tower_clamp_cls_spv", "tower_clamp_cvt_cls_spv", "tower_clamp_rq_cls_spv", "tower_rope2d_cls_spv",
    "tower_post_add_cls_spv", "tower_geglu_cls_spv",
    // the biased-block (SigLIP) row classes: the bias with its GELU, the head restrides to the flash tile's width (the
    // stocked mmproj-gemma-3-4b-it-F16.gguf, dispatched by test_gemma3v_vulkan_twin in test_gemma3v.das; the rope on a
    // fused row's slot is the qwen3v tower's - mmproj-Qwen3VL-4B-Instruct-F16.gguf via test_qwen3v_vulkan_twin in
    // test_qwen3v.das); their kernel coverage is test_vkt_tower_ln_rows. The layernorm and the seam with its next
    // layernorm serve the TTS rows' PL-BERT seat too, so the census reaches them on a box with a StyleTTS2 carrier
    "tower_bias_act_cls_spv", "tower_hpad_cls_spv", "tower_hpad16_cls_spv",
    "tower_hunpad_cls_spv", "tower_rope_tab_cls_spv",
    // the window-attention ViT's classes (qwen25v): the f32 per-window attention over the compact rows and the rms seam (the
    // gated hidden is the LLM's biased f16 act stamp at a zero row map; the stocked mmproj-Qwen2.5-Omni-3B-f16.gguf,
    // dispatched by test_qwen25v_vulkan_twin in test_qwen25v.das); their kernel coverage is test_vkt_tower_win_rows
    "tower_win_attn_cls_spv", "tower_post_add_rms_cls_spv",
    // the audio towers' classes - no zoo row runs an audio tower: the Conformer's silu rows (the bias class's unbiased stamp),
    // GLU, causal depthwise taps, windowed RPE attention (the 128-wide stamp serves the stocked mmproj-gemma-4-E2B-it-bf16.gguf's
    // audio encoder through test_gemma4a_vulkan_twin in test_audio.das) and the plain residual seam; the FastConformer's
    // centered depthwise conv (the stocked canary-qwen-2.5b-encoder-f32.bin through test_canary_vulkan_twin); the 64-wide
    // attention stamp serves no stocked carrier; their kernel coverage is test_vkt_tower_conformer,
    // test_vkt_tower_fastconformer and test_vkt_tower_rows
    "tower_act_cls_spv", "tower_glu_sig_cls_spv", "tower_dw_conv5_cls_spv", "tower_g4a_attn_cls_spv", "tower_g4a_attn64_cls_spv",
    "tower_post_add_plain_cls_spv", "tower_cn_dw_cls_spv",
    // the KHR f16 GEMM (the E-series projection off cm2): no zoo row carries a per-layer embedding; its coverage is
    // test_vkd_f16_gemm_khr
    "f16_gemm_khr_cls_spv",
    // the gemma-4 MoE combine (the two normed branches' residual step, f32 and f16 normed-row forms):
    // its carrier is the 26B-A4B, above the zoo's tier; its coverage is test_vkd_moe_routing's gemma-4
    // combine arm in test_vulkan_kernels.das
    "cls_ar_comb_g4_spv", "cls_ar_comb_g4_f16_b_spv", "cls_ar_comb_g4_rq_spv",
    // the gemma-2 softcap flash tile (the attention logits through cap * tanh(s / cap)) and the h512 flash tile
    // (gemma-4's global heads), each with its f16 twin: their carriers are gemma-2-2b and the gemma-4 dense files,
    // no zoo row; their coverage is test_vkd_fa_cm2_h256_softcap and test_vkd_fa_cm2's h512 arm in test_vulkan_kernels.das
    "fa_cm2_h256_cap_cls_spv", "fa_cm2_h256_cap_f16_cls_spv", "fa_cm2_h512_cls_spv", "fa_cm2_h512_f16_cls_spv",
    // the batched windowed decode attention and its f16 twin (a sliding window over the batch rows: the gemma
    // carriers, no zoo row); its coverage is test_vkd_da_attn_bw in test_vulkan_kernels.das
    "da_attn_bw_cls_spv", "da_attn_bw_f16_cls_spv",
    // the q51 routed down GEMV's summing twin (the 26B-A4B Q4_K_M, above the zoo's tier); its coverage is test_vkd_q51_gemv
    "q51_gemv_sum_cls_spv",
    // the scalar router GEMM: on a cm2 box every MoE carrier whose dim the small f16 GEMM's eight
    // 64-deep chunks divide takes the f16 route, so the scalar tile runs only off it (a dim off
    // 512, the KHR route). Its coverage is test_vkd_moe_routing's router arm in test_vulkan_kernels.das
    "router_gemm_cls_spv",
    // the TTS classes (the f32-exact tile, the StyleTTS2 and Pocket row, sequence, source and STFT classes) are dispatched by the
    // TTS census rows cov_tower_styletts2 and cov_tower_pocket on their carriers; their kernel cells are the test_vulkan_tts_*
    // files. A box without the TTS carriers reads them at zero - a warning, never an entry here
    // the scalar beta/alpha GEMM: the same route split for a hybrid's f32 beta/alpha rows on the f16
    // feed; off the f16 feed (DASLLAMA_COOPMAT=sdot4) it runs. Its coverage is the dn_ba run of
    // test_vkd_dn_family in test_vulkan_kernels.das
    "dn_ba_cls_spv",
    // the N-column decode GEMV leaves serve only the N-row token command, which the zoo's one-region loads never
    // engage (a batched step here rides f32 sessions on the per-op rails); the q8 row-pair stamps serve only past
    // `g_q8_n2_min_n`, off until a box's token command proves the pair. Their coverage is test_vkd_gemv_ncol in
    // test_vulkan_kernels.das; the stocked carriers are the regions files - test_gpu_resident_regions_qwen3.das and
    // the gemma files for q8, test_gpu_resident_regions_llama.das for k4 and k6, test_gpu_resident_regions_hybrid_iq3s.das
    // for iq3s, test_gpu_resident_regions_hybrid_q3kxl.das for the eight low-bit leaves
    "q8_gemv_n2_cls_spv", "q8_gemv_n4_cls_spv", "q8_gemv_n8_cls_spv",
    "q8_gemv_n2p_cls_spv", "q8_gemv_n4p_cls_spv", "q8_gemv_n8p_cls_spv",
    "kq_gemv_k4_n_cls_spv", "kq_gemv_k5_n_cls_spv", "kq_gemv_k6_n_cls_spv", "kq_gemv_q40_n_cls_spv", "kq_gemv_iq4xs_n_cls_spv",
    "kq_gemv_k3_n_cls_spv", "kq_gemv_k2_n_cls_spv", "kq_gemv_iq3s_n_cls_spv", "kq_gemv_iq3xxs_n_cls_spv", "kq_gemv_iq4nl_n_cls_spv",
    "kq_gemv_iq2s_n_cls_spv", "kq_gemv_iq2xs_n_cls_spv", "kq_gemv_iq2xxs_n_cls_spv",
    // the fused gate-up GEMV's N stamps serve only the N-row token command, which the zoo's one-region load never engages
    // (the batched rows need a region a row; the stocked regions files - test_gpu_resident_regions_qwen3.das and the
    // gemma files - reach them at two regions); their coverage is test_vkd_q8_gemv_gu_n in test_vulkan_kernels.das
    "q8_gemv_gu_n2_cls_spv", "q8_gemv_gu_n4_cls_spv", "q8_gemv_gu_n8_cls_spv",
    // the N-row command's per-row top-k: the census steps every session a row at a time (the batched step needs two sessions
    // over two regions - the gpt-oss and Qwen3-30B regions files serve it); its coverage is test_vkd_topk_n in test_vulkan_kernels.das
    "topk_n_cls_spv"]

[unused_argument(t, path, family, nbs)]   // without vulkan the gated body is empty
def private vk_cov_model(t : T?; path, family : string; nbs : array<int>) {
    // one model's vulkan coverage sweep: GPU prefill (the counting prompt clears the npos gate),
    // two greedy decode steps, a continuation prefill, then one batched step per requested B.
    // No parity asserts — the census wants DISPATCH DIVERSITY, the parity suites own truth.
    static_if (typeinfo builtin_module_exists(vulkan)) {
        if (!family_on(t, family) || !arm_on(t, "coverage-vk") || !model_available(t, path)) {
            return
        }
        var tr <- load_model_(path, QuantMode.q8)
        tr.config.seq_len = min(tr.config.seq_len, 512l)
        let vocab = tr.config.vocab_size
        with_job_que() {
            setup_dasllama_jobque()
            // an f32 session rides the per-op rails; an f16 one (the armed mirror codec) the whole-model driver
            for (kdt in [KVDtype.f32, KVDtype.f16]) {
                var s = make_run_state(tr.config, kdt, kdt)
                let p <- encode(tr, counting_prompt(1, 40), true, false)
                eval_(tr, s, p)
                var cur = greedy_top(s.logits, vocab)
                for (_i in range(2)) {
                    var one <- [cur]
                    eval_(tr, s, one)
                    delete one
                    cur = greedy_top(s.logits, vocab)
                }
                var p2 <- encode(tr, counting_prompt(50, 90), false, false)
                eval_(tr, s, p2)
                delete p2
                delete s
            }
            for (nb in nbs) {
                var ss : array<Session>
                ss |> reserve(nb)
                var sess : array<Session?>
                var curs : array<int64>
                for (i in range(nb)) {
                    ss |> emplace <| make_run_state(tr.config, KVDtype.f32, KVDtype.f32)
                    let pb <- encode(tr, counting_prompt(3 + i, 44 + i), true, false)
                    eval_(tr, ss[i], pb)
                    sess |> push(unsafe(addr(ss[i])))
                    curs |> push(greedy_top(ss[i].logits, vocab))
                }
                var ws <- create_batch_workspace_(tr)
                eval_batch_(tr, ws, sess, curs)
                sess |> clear()   // borrowed handles — delete of array<T?> cascades to the pointees
                unsafe {
                    delete ws
                    delete sess
                }
                delete curs
                delete ss
            }
        }
        // the model-switch contract: whoever puts a DIFFERENT model on the GPU drops the old one
        // first (the slot machinery / server do) — without this the stack table accumulates
        // across the zoo until the MAX_STACKS decline
        moe_gpu_drop_model_()
        delete tr
    }
}

//! every prefill tile family under the coopmat modes its planes have twins in - the box's resolved mode, mm,
//! sdot4, and for the q8 and q40 planes the f16 and int8 coopmat twins too; the carriers are the qwen3 files
//! (the Q8_0 also carries the resident fused qk_norm rail; the Q4_K_M k4/k6 with the Q8_K activation quant
//! forms and qk_norm on the split rail) and the 1B requants the format arc used
[unused_argument(t)]   // without vulkan the gated body is empty
def private vk_cov_mode_sweep(t : T?) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let vehicles = fixed_array(
            (file = "Qwen3-0.6B-Q8_0.gguf", family = "qwen3", nmodes = 5),
            (file = "Llama-3.2-1B-Instruct-Q4_0-local.gguf", family = "llama", nmodes = 5),
            (file = "Qwen3-4B-Instruct-2507-Q4_K_M.gguf", family = "qwen3", nmodes = 3),          // k4 + k6
            (file = "Llama-3.2-1B-Instruct-IQ4_XS.gguf", family = "llama", nmodes = 3),
            (file = "Llama-3.2-1B-Instruct-Q3_K_L.gguf", family = "llama", nmodes = 3),           // k3 + k5
            (file = "Llama-3.2-1B-Instruct-IQ3_M.gguf", family = "llama", nmodes = 3),            // iq3s
            (file = "Llama-3.2-1B-Instruct-IQ3_XXS-local.gguf", family = "llama", nmodes = 3),
            (file = "Llama-3.2-1B-Instruct-IQ4_NL-local.gguf", family = "llama", nmodes = 3),
            (file = "Llama-3.2-1B-Instruct-Q2_K-local.gguf", family = "llama", nmodes = 3),
            (file = "Llama-3.2-1B-Instruct-IQ2_XS-local.gguf", family = "llama", nmodes = 3),
            (file = "Llama-3.2-1B-Instruct-IQ2_XXS-local.gguf", family = "llama", nmodes = 3),
            (file = "Llama-3.2-1B-Instruct.i1-IQ3_XXS.gguf", family = "llama", nmodes = 3))       // iq2s on the attention planes
        if (!vk_moe_init()) {   //! the sweep reads the device's resolved mode before its first row loads (a family filter may have skipped every earlier row)
            t |> skip("no Vulkan device for the coopmat mode sweep")
            return
        }
        let none : array<int>
        let box_mode = g_gpu.coopmat_mode
        let modes = fixed_array(box_mode, COOPMAT_MM, COOPMAT_SDOT4, COOPMAT_F16, COOPMAT_INT8)
        for (v in vehicles) {
            let vpath = path_join(models_dir(), v.file)
            for (mi in range(v.nmodes)) {
                g_gpu.coopmat_mode = modes[mi]
                vk_cov_model(t, vpath, v.family, none)
            }
            g_gpu.coopmat_mode = box_mode
        }
    }
}

[test]
def test_vulkan_kernel_coverage_serving(tst : T?) {
    tst |> run("Vulkan serving census: the local zoo across resident/arena/kq/deltanet/MoE") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!arm_on(t, "coverage-vk")) {
                return
            }
            vk_kernel_coverage_reset()
            let none : array<int>
            // llama q8: the resident dense ladder (device prefill + decode) + the per-op rails' batched steps at 2, 4
            // and 8 f32 sessions (the resident N-row command needs a region a row: the regions files carry the N stamps)
            vk_cov_model(t, path_join(models_dir(), "tinyllama-1.1b-chat-v1.0.Q8_0.gguf"), "llama", [2, 4, 8])
            // gemma3 q8: the per-op arena rails + qk_norm
            vk_cov_model(t, path_join(models_dir(), "gemma-3-1b-it-Q8_0.gguf"), "gemma3", none)
            vk_cov_mode_sweep(t)
            // qwen3.5 q8: the DeltaNet chains (conv/step/scan + the decode step)
            vk_cov_model(t, path_join(models_dir(), "Qwen3.5-0.8B-Q8_0.gguf"), "qwen35", none)
            // qwen3.5 9B (large tier): head-256 gated attention on the cm2 flash stamps, the
            // K-quant deltanet planes on the resident driver
            vk_cov_model(t, path_join(models_dir(), "Qwen3.5-9B-MTP-UD-Q5_K_XL.gguf"), "qwen35", none)
            // the MoE rows (expert FFN/qkv chains + heat cache) sit above the small tier
            vk_cov_model(t, path_join(models_dir(), "Qwen1.5-MoE-A2.7B-Chat.Q8_0.gguf"), "qwen2moe", none)
            vk_cov_model(t, path_join(models_dir(), "Qwen3-30B-A3B-Instruct-2507-Q4_K_M.gguf"), "qwen3moe", none)
            // the Q4_K_M twin fits a 16 GB card whole: the whole-model driver's MoE block (the router on the f16
            // cm2 GEMM, the per-row select, the bucket schedule, the combine-folded residual step, the token command's routed block)
            vk_cov_model(t, path_join(models_dir(), "Qwen1.5-MoE-A2.7B-Chat-Q4_K_M-local.gguf"), "qwen2moe", none)
            // the UD-IQ2_XXS 30B: the resident MoE block's s and e stamps over iq2xxs expert planes, whole on a 16 GB card
            vk_cov_model(t, path_join(models_dir(), "Qwen3-30B-A3B-Instruct-2507-UD-IQ2_XXS.gguf"), "qwen3moe", none)
            // the TTS rows: the StyleTTS2 and Pocket seats on the Vulkan tier - the f32 tile, the row, sequence,
            // source and STFT classes - on the TTS carriers a box stocks
            cov_tower_styletts2(t)
            cov_tower_pocket(t)
            vk_kernel_coverage_report()
            var cov <- vk_kernel_coverage()
            var served = 0
            for (n in values(cov)) {
                if (n > 0l) {
                    served++
                }
            }
            if (served == 0) {
                delete cov
                feint("no vulkan dispatches - arm DASLLAMA_GPU=1 on a device box (the census needs the serving tier)\n")
                return
            }
            for (blind in VK_CENSUS_NEVER_DISPATCHED) {
                var named = false
                for (k in keys(cov)) {
                    if (starts_with(k, blind)) {
                        named = true
                    }
                }
                // the entry must reach a COMPILED key: cov carries every seeded kernel at zero,
                // so an entry matching none names nothing and would sit green forever
                t |> success(named,
                    "{blind} names no seeded kernel - spell the census key (<vk_dispatch name>_spv)")
                let hits = named ? vk_kernel_coverage_of(blind) : 0l   //! the accessor panics on an unseeded name; the assert above is the legible red
                t |> success(hits == 0l,
                    "{blind} is a DECLARED blind spot - a stocked carrier now dispatches it ({hits}): drop the entry, its census row exists")
            }
            delete cov
            let famfilter = g_env_test.test_family
            if (empty(famfilter)) {
                // floor calibration: 25 measured on the default 5-model zoo (MoE rows skip without
                // PARITY_FULL; coopmat-mode variants and mode-gated twins legitimately idle here);
                // 24 catches a gross wiring regression without tripping on one near-boundary row
                t |> success(served >= 24, "the serving census dispatched {served} distinct kernels (rail floor 24)")
            } else {
                to_log(LOG_INFO, "census floor not asserted (family filter '{famfilter}'): {served} served\n")
            }
        } else {
            feint("vulkan is not built on this platform; nothing to census\n")
        }
    }
}

[test]
def test_vulkan_kernel_coverage_rail(tst : T?) {
    tst |> run("Vulkan kernel coverage rail: the lens spec seeds the census (device-free)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!arm_on(t, "coverage")) {
                return
            }
            // no device, no dispatches — the unit pins that the compile-time lens spec parses
            // into the census universe and every kernel starts at zero. The serving census runs
            // on the PC box beside the vulkan tier suites.
            var cov <- vk_kernel_coverage()
            t |> success(length(cov) >= 40, "the lens spec seeds {length(cov)} kernels (floor 40)")
            var zeros = 0
            for (n in values(cov)) {
                if (n == 0l) {
                    zeros++
                }
            }
            t |> equal(zeros, length(cov))
            delete cov
        } else {
            feint("vulkan is not built on this platform; nothing to census\n")
        }
    }
}
