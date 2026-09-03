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
require dasllama/dasllama_qwen25v   // the cov_tower window-ViT row
require dasllama/dasllama_qwen3v    // the cov_tower GPU-stem row
require ?das_metal dasllama/dasllama_metal_tower   // set_metal_tower — the census pins the rail ON
require ?das_metal dasllama/dasllama_metal_prefill // metal_prefill_shutdown — the crowned twin-W leg re-arms PSOs
require ?das_metal dasllama/dasllama_metal_asr_dec // set_metal_wdec — the cov_tower mixed chunk's decoder half
require daslib/jobque_boost
require daslib/fio  // the zoo paths
require math  // nolint:STYLE030,LINT019 — used only inside the das_metal static_if half
require strings
require _model_tier
require dasllama/dasllama_env   // nolint:STYLE030,LINT019 — the read sites sit in the apple static_if half; the linux lint flavor sees the require as unused

// The KERNEL COVERAGE census (REVIEW: "A diff that adds a [metal_kernel] under ../dasllama/ either has that kernel dispatched by a census row in test_kernel_coverage.das ... or names its kernel class in that file's CENSUS_NEVER_DISPATCHED"): drive the small-model zoo across the
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
    // would reach TALLKQ_MIN_PANEL (96 MiB) - the zoo's small carriers top out far below it.
    // Their coverage is the tall kernel-unit leg in test_metal_gemm_kernels.das (kq_mulmm_gate)
    "MetalKqMulMmK4TH128", "MetalKqMulMmK5TH128", "MetalKqMulMmK6TH128",
    // the nine formats the iquant arc added (Q2_K, Q3_K, IQ4_XS, IQ4_NL, IQ3_S, IQ3_XXS, IQ2_S,
    // IQ2_XS, IQ2_XXS): no stocked census carrier holds a plane in any of them - the zoo quants
    // are Q8_0 / Q5_K_M / Q4_K_M / mxfp4. Their coverage is the kernel-unit gates in
    // test_metal_gemv_kernels.das / test_metal_gemm_kernels.das and the per-format e2e parity
    // rows recorded in HOW_TO_ADD_A_FORMAT.md
    "MetalKqGemvK2", "MetalKqMvB2K2", "MetalKqMvB4K2", "MetalKqMvB8K2", "MetalKqMulMmK2",
    "MetalKqGemvK3", "MetalKqMvB2K3", "MetalKqMvB4K3", "MetalKqMvB8K3", "MetalKqMulMmK3",
    "MetalKqGemvIq4xs", "MetalKqMvB2Iq4xs", "MetalKqMvB4Iq4xs", "MetalKqMvB8Iq4xs", "MetalKqMulMmIq4xs",
    "MetalKqGemvIq4nl", "MetalKqMvB2Iq4nl", "MetalKqMvB4Iq4nl", "MetalKqMvB8Iq4nl", "MetalKqMulMmIq4nl",
    "MetalKqGemvIq3s", "MetalKqMvB2Iq3s", "MetalKqMvB4Iq3s", "MetalKqMvB8Iq3s", "MetalKqMulMmIq3s",
    "MetalKqGemvIq3xxs", "MetalKqMvB2Iq3xxs", "MetalKqMvB4Iq3xxs", "MetalKqMvB8Iq3xxs", "MetalKqMulMmIq3xxs",
    "MetalKqGemvIq2s", "MetalKqMvB2Iq2s", "MetalKqMvB4Iq2s", "MetalKqMvB8Iq2s", "MetalKqMulMmIq2s",
    "MetalKqGemvIq2xs", "MetalKqMvB2Iq2xs", "MetalKqMvB4Iq2xs", "MetalKqMvB8Iq2xs", "MetalKqMulMmIq2xs",
    "MetalKqGemvIq2xxs", "MetalKqMvB2Iq2xxs", "MetalKqMvB4Iq2xxs", "MetalKqMvB8Iq2xxs", "MetalKqMulMmIq2xxs",
    // the same nine formats' crown-gated forms (the metal-kq-race arc): the f4-slab GEMV twins
    // and the tensor mul_mm T/TH stamps compile only behind a box crown and dispatch only on
    // planes the zoo does not stock. Their coverage is the same kernel-unit gates plus the
    // tensor-twins/iq2xxs matrix row and the mint's METAL_TWIN race rows
    "MetalKqGemvIq3sF4", "MetalKqGemvIq3xxsF4", "MetalKqGemvIq2xxsF4",
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
    "MetalArgmaxRows", "MetalKqMvB2K4R2"
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
                        unsafe {
                            sess |> push(addr(ss[i]))
                        }
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
        delete samples
        cov_tower_gemma4uv(t)
        cov_tower_gemma4v(t)
        cov_tower_gemma3v(t)
        cov_tower_qwen25v(t)
        cov_tower_qwen3v(t)
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
            // the blind-spot list self-drains: census keys are class-qualified MSL entry
            // names, so a declared class whose entry counted dispatches is a STALE entry
            for (blind in CENSUS_NEVER_DISPATCHED) {
                var hits = 0l
                for (k, n in keys(cov), values(cov)) {
                    if (starts_with(k, blind)) {
                        hits += n
                    }
                }
                t |> success(hits == 0l,
                    "{blind} is a DECLARED blind spot - a stocked carrier now dispatches it ({hits}): drop the entry, add its census row")
            }
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
            var s = make_run_state(tr.config, KVDtype.f32, KVDtype.f32)
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
            for (nb in nbs) {
                var ss : array<Session>
                ss |> reserve(nb)
                var sess : array<Session?>
                var curs : array<int64>
                for (i in range(nb)) {
                    ss |> emplace <| make_run_state(tr.config, KVDtype.f32, KVDtype.f32)
                    let pb <- encode(tr, counting_prompt(3 + i, 44 + i), true, false)
                    eval_(tr, ss[i], pb)
                    unsafe {
                        sess |> push(addr(ss[i]))
                    }
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

[test]
def test_vulkan_kernel_coverage_serving(tst : T?) {
    tst |> run("Vulkan serving census: the local zoo across resident/arena/kq/deltanet/MoE") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!arm_on(t, "coverage-vk")) {
                return
            }
            vk_kernel_coverage_reset()
            let none : array<int>
            // llama q8: the resident dense ladder (device prefill + decode) + a B=2 batch step
            vk_cov_model(t, path_join(models_dir(), "tinyllama-1.1b-chat-v1.0.Q8_0.gguf"), "llama", [2])
            // gemma3 q8: the per-op arena rails + qk_norm
            vk_cov_model(t, path_join(models_dir(), "gemma-3-1b-it-Q8_0.gguf"), "gemma3", none)
            // qwen3 q8: the resident fused qk_norm rail (qkn_rope) — the kq row below rides the
            // split rail by construction, so this is the fused twin's only carrier
            vk_cov_model(t, path_join(models_dir(), "Qwen3-0.6B-Q8_0.gguf"), "qwen3", none)
            // qwen3 Q4_K_M: the native K-quant formats (k4/k6 GEMV + batch tiles) + the Q8_K
            // activation quant forms + qk_norm on the split rail
            vk_cov_model(t, path_join(models_dir(), "Qwen3-4B-Instruct-2507-Q4_K_M.gguf"), "qwen3", none)
            // qwen3.5 q8: the DeltaNet chains (conv/step/scan + the decode step)
            vk_cov_model(t, path_join(models_dir(), "Qwen3.5-0.8B-Q8_0.gguf"), "qwen35", none)
            // the MoE rows (expert FFN/qkv chains + heat cache) sit above the small tier
            vk_cov_model(t, path_join(models_dir(), "Qwen1.5-MoE-A2.7B-Chat.Q8_0.gguf"), "qwen2moe", none)
            vk_cov_model(t, path_join(models_dir(), "Qwen3-30B-A3B-Instruct-2507-Q4_K_M.gguf"), "qwen3moe", none)
            vk_kernel_coverage_report()
            var cov <- vk_kernel_coverage()
            var served = 0
            for (n in values(cov)) {
                if (n > 0l) {
                    served++
                }
            }
            delete cov
            if (served == 0) {
                feint("no vulkan dispatches - arm DASLLAMA_GPU=1 on a device box (the census needs the serving tier)\n")
                return
            }
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
