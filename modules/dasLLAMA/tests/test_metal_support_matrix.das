options gen2
options _function_length = 0   // one flat per-family row per function: needs pin, per-path cells, generate + batch parity
options persistent_heap   // + explicit deletes: the models free before the process exits
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

require dastest/testing_boost public
require dasllama/dasllama   // the public facade (engine + arch registrations + the metal overrides); off-Apple the static_if halves compile out and only its dasllama_common re-export stays referenced
require dasllama/dasllama_math   // set_wscale_f16 — the wscale matrix axis
require dasllama/dasllama_layout   // convert_model_to_metal_blob — the blob-flavor transform
require dasllama/dasllama_parity   // nolint:STYLE030,LINT019 — cached_ids/cached_vals, used only inside the Apple static_if half
require ?das_metal dasllama/dasllama_metal_decode  // stats/declines/shutdown rails
require ?das_metal dasllama/dasllama_metal_prefill  // prefill stats/declines rails
require ?das_metal dasllama/dasllama_metal_common  // the kernel coverage census (tensor-twin cell)
require ?das_metal metal/das_metal_boost  // device probe + metal_live_object_count
require daslib/jobque_boost
require daslib/fio   // nolint:STYLE030,LINT019 — used only inside the Apple static_if half
require math  // used only inside the Apple static_if half
require strings
require _model_tier

// The (weights x wscale x kv-dtype x path) SUPPORT MATRIX for the Metal drivers (format-matrix
// arc W0). Each cell asserts ENGAGE (steps served on the GPU) or DECLINE with the EXPECTED
// first-failing-clause reason from the declines-by-reason tables — a coverage regression (a gate
// declining a config that used to engage) turns into a red cell here instead of a silent CPU
// fallback (the wscale16-prefill inertness went unnoticed for months exactly that way). This
// expectation table IS the support documentation; each later wave flips its cells to ENGAGE as
// coverage widens (W2: s16 prefill; W3: q8_0/tq4 caches; W4: K-quant weights).
//
// Expected matrix TODAY (llama family, portable backend, Llama-3.2-1B):
//   weights   wscale  kv     prefill              decode               batch decode
//   q8        f32     f16    ENGAGE               ENGAGE               ENGAGE
//   q8        f32     f32    ENGAGE               ENGAGE               ENGAGE
//   q8        f32     q8_0   ENGAGE               ENGAGE (W3)          ENGAGE (W3)
//   q8        f32     tq4    ENGAGE               ENGAGE (W3)          ENGAGE (W3)
//   q8        f16     f16    ENGAGE (W2)          ENGAGE               ENGAGE
//   Q4_K_M    f32     f16    ENGAGE (M2.3)        ENGAGE (W4)          ENGAGE (M2.1/W4)
//   Q4_K_M    f32     q8_0   ENGAGE (M2.3)        ENGAGE (W4)          ENGAGE (M2.1/W4)
// (W4: single-stream decode serves K-quant weights via the per-site kq plane GEMVs; M2.1: the
// batched step serves kq at B <= 8 via nrows per-stream plane GEMVs, B >= 9 via the kq mul_mm
// twins (%64 shape gates); M2.3: prefill serves kq via the mul_mm twins, same gates.)
// The dim ladder (M2.6) rides two extra models when present: an 8B (dim 4096, q8) engages at
// every B — past B=4 the GEMM's fixed K-tiles take any width — and a 70B-class kq model
// (dim 8192) engages all three paths with the batched add+rms running as the unfused pair.
// The MetalMode knob (M1) rides the same matrix: off = overrides deselected, when_available =
// engage/fallback per the cells above, required = a CAPABILITY decline panics (policy declines
// — the prefill npos window — stay quiet CPU fallbacks). Batch kq: B <= 8 = per-stream GEMVs,
// B >= 9 = the kq mul_mm twins (%64 shapes gated).
// plus the prefill-only window clause: npos < min_npos declines npos_window. A continuation
// chunk (start_pos > 0) SERVES since M2.5 (existing rows gather into the K/V panels, the
// causal diagonal shifts).
//
// The FAMILY matrix (family waves A, test_metal_family_matrix below): the gate is no longer
// arch-named — the block set declares the graph (`graph` decline) and metal_needs derives the
// feature set (`feature` + need_* declines) against per-path implemented masks. Served today:
// llama + phi3 (neox) + qwen2 (neox+qkv_bias) + qwen3 (neox+qk_norm+qd_neq_dim) + gemma2
// (neox+softcap+sliding+pre_post_norm+geglu, hs 256 — wave B stage 1) + gemma3 (dual-theta
// per-layer rope — stage 2), each row token-parity-proven on all three paths. gemma4-12B
// (stage 3: two attention classes, hs 512, V-from-K, v_norm, out_scale, suppress) serves
// ALL THREE paths token-exact (stage 3d: per-class batch uniforms + prefix `lay`). mistral3
// (empty config = the llama shape) serves by construction; no <= 35B mistral3-arch GGUF
// exists to validate against, so its real-model row waits on a fixture. qwen3moe +
// gemma4moe-26B + gptoss (wave C): GPU MoE routing — expert-indexed GEMV twins (decode/batch)
// + CSR-gathered mul_mm prefill; gemma4's parallel-dense form; gpt-oss mx4 experts + sinks +
// softmax_weight + biases + swiglu_oai. qwen2moe (sigmoid shexp) parked. deltanet = wave D.

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

// greedy over one stream's slice of a concatenated multi-stream logits array (the B=4
// forced cells' eyeball rail)
def private greedy_top_at(lg : array<float>; base, vocab : int64) : int64 {
    var best = 0l
    for (v in range64(1l, vocab)) {
        if (lg[base + v] > lg[base + best]) {
            best = v
        }
    }
    return best
}

def private same(a, b : array<int64>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (x, y in a, b) {
        if (x != y) {
            return false
        }
    }
    return true
}

[unused_argument(t, tr, kdt, tag, exp_pf, exp_dec, exp_bat)]   // off-Apple the gated body is empty
def private matrix_cell(t : T?; tr : Model; kdt : KVDtype; tag : string;
                        exp_pf, exp_dec, exp_bat : string) {
    // one prefill + two decode steps + one B=2 batch step under the metal overrides; each path
    // asserts its cell: "" = ENGAGE (served > 0), else DECLINE with that reason recorded
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let c = tr.config
        let vocab = c.vocab_size
        let pf0 = metal_prefill_stats().prefills
        let de0 = metal_decode_stats().decodes
        let ba0 = metal_batch_decode_stats().steps
        var pfd0 <- metal_prefill_declines()
        var ded0 <- metal_decode_declines()
        var bad0 <- metal_batch_decode_declines()
        with_job_que() {
            setup_dasllama_jobque()
            var sa = make_run_state(c, kdt, kdt)
            var sb = make_run_state(c, kdt, kdt)
            let pa <- encode(tr, counting_prompt(1, 40), true, false)
            let pb <- encode(tr, counting_prompt(5, 44), true, false)
            t |> success(select_prefill_override("metal"), "{tag}: prefill override registered")
            t |> success(select_decode_override("metal"), "{tag}: decode override registered")
            set_metal_prefill_min_npos(1l)
            eval_(tr, sa, pa)   // the prefill cell
            eval_(tr, sb, pb)
            var cura = greedy_top(sa.logits, vocab)
            let curb = greedy_top(sb.logits, vocab)
            for (_i in range(2)) {   // the single-stream decode cell
                var one <- [cura]
                eval_(tr, sa, one)
                delete one
                cura = greedy_top(sa.logits, vocab)
            }
            var ws <- create_batch_workspace_(tr)   // the batched decode cell
            var sess : array<Session?>
            unsafe {
                sess |> push(addr(sa))
                sess |> push(addr(sb))
            }
            var cur <- [cura, curb]
            eval_batch_(tr, ws, sess, cur)
            select_decode_override("")
            select_prefill_override("")
            set_metal_prefill_min_npos(64l)
            sess |> clear()   // drop the borrowed handles: delete of array<T?> cascades to the pointees
            unsafe {
                delete ws
                delete sess
            }
            delete cur
            delete sa
            delete sb
        }
        var pfd <- metal_prefill_declines()
        var ded <- metal_decode_declines()
        var bad <- metal_batch_decode_declines()
        check_cell(t, "{tag} prefill", exp_pf, metal_prefill_stats().prefills - pf0, pfd0, pfd)
        check_cell(t, "{tag} decode", exp_dec, metal_decode_stats().decodes - de0, ded0, ded)
        check_cell(t, "{tag} batch", exp_bat, metal_batch_decode_stats().steps - ba0, bad0, bad)
        delete pfd0
        delete ded0
        delete bad0
        delete pfd
        delete ded
        delete bad
    }
}

[unused_argument(t, tr, nb, tag)]   // off-Apple the gated body is empty
def private dim_ladder_batch_cell(t : T?; tr : Model; nb : int; tag : string) {
    // one B=nb batched step under the metal decode override — the M2.6 coverage past B=4
    // (GPU prefill fills the sessions so the wide models stay cheap)
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let c = tr.config
        let vocab = c.vocab_size
        var served = 0l
        with_job_que() {
            setup_dasllama_jobque()
            var ss : array<Session>
            ss |> reserve(nb)
            var sess : array<Session?>
            var cur : array<int64>
            select_prefill_override("metal")
            set_metal_prefill_min_npos(1l)
            for (i in range(nb)) {
                ss |> emplace <| make_run_state(c, KVDtype.f16, KVDtype.f16)
                let p <- encode(tr, counting_prompt(1 + i, 14 + i), true, false)
                eval_(tr, ss[i], p)
                unsafe {
                    sess |> push(addr(ss[i]))
                }
                cur |> push(greedy_top(ss[i].logits, vocab))
            }
            set_metal_prefill_min_npos(64l)
            select_prefill_override("")
            var ws <- create_batch_workspace_(tr)
            let ba0 = metal_batch_decode_stats().steps
            select_decode_override("metal")
            eval_batch_(tr, ws, sess, cur)
            select_decode_override("")
            served = metal_batch_decode_stats().steps - ba0
            sess |> clear()   // borrowed handles — delete of array<T?> cascades to the pointees
            unsafe {
                delete ws
                delete sess
            }
            delete cur
            delete ss
        }
        t |> success(served > 0l, "{tag}: B={nb} batched step ENGAGES on the GPU ({served} served)")
    }
}

// "" = the path must ENGAGE (served > 0); otherwise it must sit out (served == 0) with `want`
// recorded as a first-failing clause in the window
def private check_cell(t : T?; cell : string; want : string; served : int64;
                       before, after : table<string; int64>) {
    if (want == "") {
        t |> success(served > 0l, "{cell}: ENGAGES on the GPU ({served} served)")
    } else {
        let hits = (after?[want] ?? 0l) - (before?[want] ?? 0l)
        t |> success(served == 0l, "{cell}: stays on the CPU (expected decline {want})")
        t |> success(hits > 0l, "{cell}: declines with reason '{want}' ({hits} recorded)")
    }
}

// the blob twin of a planar fixture: an OWNED engine load transformed to the GPU form — the
// drivers are blob-only. ENGAGE cells run the twin; DECLINE cells keep the planar model (its
// CPU fallback serves quietly, and the capability reasons out-rank `planar` in the gates).
def private blob_twin(t : T?; path : string; seq_cap : int64) : Model {
    var tg <- load_model_(path, QuantMode.q8)
    t |> success(convert_model_to_metal_blob(tg), "metal-blob transform")
    tg.config.seq_len = min(tg.config.seq_len, seq_cap)
    return <- tg
}

// generate()'s exact output shape (prompt echo minus BOS, continuation to `steps`, BOS stop)
// with the prefill on trp and the decode steps on trd — the decode-isolated GPU pass (the
// planar model CPU-prefills, the blob twin GPU-decodes; whole-GPU passes just call generate
// on the twin with both overrides selected). Call inside with_job_que.
def private split_generate(trp, trd : Model; prompt : array<int64>; steps : int) : array<int64> {
    var ids : array<int64>
    ids |> reserve(steps)
    let n = long_length(prompt)
    for (q in range64(n - 1l)) {
        return <- ids if (long_length(ids) >= int64(steps) || prompt[q + 1l] == 1l)
        ids |> push(prompt[q + 1l])
    }
    var s = make_run_state(trp.config, KVDtype.f16, KVDtype.f16)
    eval_(trp, s, prompt)
    var token = greedy_top(s.logits, trp.config.vocab_size)
    while (long_length(ids) < int64(steps)) {
        break if (token == 1l)   // BOS delimits sequences (generate's stop)
        ids |> push(token)
        break if (long_length(ids) >= int64(steps))
        var one <- [token]
        eval_(trd, s, one)
        delete one
        token = greedy_top(s.logits, trp.config.vocab_size)
    }
    delete s
    return <- ids
}

[test]
def test_metal_support_matrix(t : T?) {
    t |> run("Metal support matrix: weights x wscale x kv x path engage/decline (Llama-3.2-1B)") <| @(t : T?) {
        if (!family_on(t, "llama")) return
        static_if (typeinfo builtin_module_exists(das_metal)) {
            if (!jit_enabled()) {
                t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
                return
            }
            let path = path_join(models_dir(), "Llama-3.2-1B-Instruct-Q8_0.gguf")
            if (!model_available(t, path)) {
                return
            }
            var dev = metal_create_system_default_device()
            if (dev == null) {
                feint("no Metal device; support matrix skipped\n")
                return
            }
            metal_release(dev)
            let prior_wscale = get_wscale_f16()             // process-wide knobs — restored below
            let prior_backend = active_kernel_backend()
            pin_kernel_backend("portable")   // row-major weights (the resident drivers' donor contract)

            // ===== q8 weights, f32 scale plane =====
            set_wscale_f16(false)
            var tr <- load_model_(path, QuantMode.q8)
            tr.config.seq_len = min(tr.config.seq_len, 512l)
            var trg <- blob_twin(t, path, 512l)
            if (arm_on(t, "cells-q8")) {
                matrix_cell(t, trg, KVDtype.f16, "q8/s32/f16", "", "", "")
                matrix_cell(t, trg, KVDtype.f32, "q8/s32/f32", "", "", "")
                matrix_cell(t, trg, KVDtype.q8_0, "q8/s32/q8_0", "", "", "")
                matrix_cell(t, trg, KVDtype.tq4, "q8/s32/tq4", "", "", "")
            }

            // the prefill window clause is PLANAR-ONLY now: a blob model has no CPU fallback,
            // so the policy window collapses (any npos serves); a planar model still records
            // npos_window first. A continuation chunk (start_pos > 0) SERVES since M2.5.
            if (arm_on(t, "window")) {
                var pfd0 <- metal_prefill_declines()
                let pf0 = metal_prefill_stats().prefills
                with_job_que() {
                    setup_dasllama_jobque()
                    var sp = make_run_state(tr.config, KVDtype.f16, KVDtype.f16)
                    var s = make_run_state(trg.config, KVDtype.f16, KVDtype.f16)
                    let short_p <- encode(tr, counting_prompt(1, 12), true, false)
                    let cont_p <- encode(tr, counting_prompt(13, 26), false, false)
                    select_prefill_override("metal")   // min_npos stays at the 64 default
                    eval_(tr, sp, short_p)             // planar + npos < 64 -> npos_window (CPU serves)
                    eval_(trg, s, short_p)             // blob + npos < 64 -> SERVES (window collapsed)
                    set_metal_prefill_min_npos(1l)
                    eval_(trg, s, cont_p)              // start_pos = n_past > 0 -> SERVES (M2.5)
                    set_metal_prefill_min_npos(64l)
                    select_prefill_override("")
                    delete s
                    delete sp
                }
                var pfd <- metal_prefill_declines()
                t |> success((pfd?["npos_window"] ?? 0l) > (pfd0?["npos_window"] ?? 0l),
                    "planar short prompt below min_npos declines npos_window")
                t |> success(metal_prefill_stats().prefills - pf0 == 2l,
                    "blob short prompt + continuation chunk both ENGAGE (the npos window is planar-only)")
                t |> success((pfd?["start_pos"] ?? 0l) == (pfd0?["start_pos"] ?? 0l),
                    "the retired start_pos clause records nothing")
                delete pfd0
                delete pfd
            }

            // ===== q8 weights, f16 scale plane (the load DEFAULT since wscale16) =====
            if (arm_on(t, "cells-s16")) {
                set_wscale_f16(true)   // the s16 planar LOAD path into the blob transform
                var tr16 <- blob_twin(t, path, 512l)
                matrix_cell(t, tr16, KVDtype.f16, "q8/s16/f16", "", "", "")
                set_wscale_f16(false)
                delete tr16
            }

            // ===== the MetalMode knob (M1) on a fully-servable q8 config =====
            if (arm_on(t, "mode")) {
                let de_off = metal_decode_stats().decodes
                set_metal_mode(MetalMode.off)
                t |> success(active_decode_override() == "", "mode off: overrides deselected")
                with_job_que() {
                    setup_dasllama_jobque()
                    var s = make_run_state(tr.config, KVDtype.f16, KVDtype.f16)
                    let pr <- encode(tr, counting_prompt(1, 12), true, false)
                    eval_(tr, s, pr)
                    var one <- [greedy_top(s.logits, tr.config.vocab_size)]
                    eval_(tr, s, one)
                    delete one
                    delete s
                }
                t |> success(metal_decode_stats().decodes == de_off, "mode off: nothing served on the GPU")
                set_metal_mode(MetalMode.when_available)
                t |> success(active_decode_override() == "metal", "mode when_available: metal overrides selected")
                let de_avail = metal_decode_stats().decodes
                with_job_que() {
                    setup_dasllama_jobque()
                    var s = make_run_state(trg.config, KVDtype.f16, KVDtype.f16)
                    let pr <- encode(trg, counting_prompt(1, 12), true, false)
                    eval_(trg, s, pr)
                    var one <- [greedy_top(s.logits, trg.config.vocab_size)]
                    eval_(trg, s, one)
                    delete one
                    delete s
                }
                t |> success(metal_decode_stats().decodes > de_avail, "mode when_available: decode serves on the GPU")
                // required on a servable blob config: everything serves — the short prompt
                // included (the npos POLICY window is planar-only; blob has no CPU fallback)
                set_metal_mode(MetalMode.required)
                let de_req = metal_decode_stats().decodes
                let pf_req = metal_prefill_stats().prefills
                with_job_que() {
                    setup_dasllama_jobque()
                    var s = make_run_state(trg.config, KVDtype.f16, KVDtype.f16)
                    let pr <- encode(trg, counting_prompt(1, 12), true, false)
                    eval_(trg, s, pr)
                    var one <- [greedy_top(s.logits, trg.config.vocab_size)]
                    eval_(trg, s, one)
                    delete one
                    delete s
                }
                t |> success(metal_decode_stats().decodes > de_req, "mode required: decode serves on the GPU")
                t |> success(metal_prefill_stats().prefills > pf_req,
                    "mode required: the short prefill serves too (the npos window collapsed on blob)")
                set_metal_mode(MetalMode.off)
            }

            // ===== native K-quant weights (Q4_K_M) =====
            // All three paths ENGAGE (W4 decode, M2.1 batch B<=8 + W4 mul_mm twins past that,
            // M2.3 prefill mul_mm twins) — Q4_K_M mixes k4 and k6 per tensor, incl. the tied
            // Q6_K classifier on the GPU-logits paths
            let kpath = path_join(models_dir(), "Llama-3.2-1B-Instruct-Q4_K_M.gguf")
            if (!arm_on(t, "kq")) {
                // filtered — the skip is already registered
            } elif (stat(kpath).is_valid) {
                var trk <- load_model_(kpath, QuantMode.q8)
                trk.config.seq_len = min(trk.config.seq_len, 512l)
                t |> success(trk.kquant_native, "Q4_K_M under a q8-mode load keeps native K-quant planes")
                var trkg <- blob_twin(t, kpath, 512l)
                matrix_cell(t, trkg, KVDtype.f16, "k4/s32/f16", "", "", "")
                matrix_cell(t, trkg, KVDtype.q8_0, "k4/s32/q8_0", "", "", "")
                // the output half: GPU kq decode tokens must be the CPU truth
                let kprompt <- encode(trk, counting_prompt(1, 30), true, false)
                var cpu_ids : array<int64>
                var gpu_ids : array<int64>
                with_job_que() {
                    setup_dasllama_jobque()
                    var s = make_run_state(trk.config, KVDtype.f16, KVDtype.f16)
                    let ids <- generate(trk, s, kprompt, length(kprompt) + 8)
                    cpu_ids := ids
                    delete s
                    select_decode_override("metal")
                    select_prefill_override("metal")
                    set_metal_prefill_min_npos(1l)
                    var s2 = make_run_state(trkg.config, KVDtype.f16, KVDtype.f16)
                    let ids2 <- generate(trkg, s2, kprompt, length(kprompt) + 8)
                    set_metal_prefill_min_npos(64l)
                    gpu_ids := ids2
                    delete s2
                    select_decode_override("")
                    select_prefill_override("")
                }
                t |> success(same(cpu_ids, gpu_ids),
                    "K-quant continuation with metal overrides ON matches the CPU truth (guard verified)")
                delete cpu_ids
                delete gpu_ids

                // ===== MetalMode.required on the kq model =====
                // every implemented path serves (prefill mul_mm, single decode, batched steps);
                // the remaining capability gap — a continuation prefill (start_pos > 0, M2.5) —
                // fails hard instead of falling back
                with_job_que() {
                    setup_dasllama_jobque()
                    var sa = make_run_state(trkg.config, KVDtype.f16, KVDtype.f16)
                    var sb = make_run_state(trkg.config, KVDtype.f16, KVDtype.f16)
                    set_metal_mode(MetalMode.required)
                    let pf0 = metal_prefill_stats().prefills
                    set_metal_prefill_min_npos(1l)
                    eval_(trkg, sa, kprompt)   // required: the kq prefill must serve on the GPU
                    eval_(trkg, sb, kprompt)
                    set_metal_prefill_min_npos(64l)
                    t |> success(metal_prefill_stats().prefills - pf0 > 0l,
                        "required: kq prefill serves on the GPU (M2.3 mul_mm twins)")
                    let de0 = metal_decode_stats().decodes
                    var one <- [greedy_top(sa.logits, trkg.config.vocab_size)]
                    eval_(trkg, sa, one)
                    t |> success(metal_decode_stats().decodes - de0 > 0l,
                        "required: kq single-stream decode serves on the GPU")
                    var ws <- create_batch_workspace_(trkg)
                    var sess : array<Session?>
                    unsafe {
                        sess |> push(addr(sa))
                        sess |> push(addr(sb))
                    }
                    var cur <- [greedy_top(sa.logits, trkg.config.vocab_size), greedy_top(sb.logits, trkg.config.vocab_size)]
                    let ba0 = metal_batch_decode_stats().steps
                    eval_batch_(trkg, ws, sess, cur)
                    t |> success(metal_batch_decode_stats().steps - ba0 > 0l,
                        "required: kq batched step serves on the GPU (M2.1 per-stream form)")
                    // a continuation chunk under required SERVES since M2.5...
                    var sc = make_run_state(trkg.config, KVDtype.f16, KVDtype.f16)
                    set_metal_prefill_min_npos(1l)
                    let pfc0 = metal_prefill_stats().prefills
                    eval_(trkg, sc, kprompt)     // chunk 1: fresh-start, GPU-serves
                    eval_(trkg, sc, kprompt)     // chunk 2: start_pos = n_past > 0 — GPU-serves too
                    t |> success(metal_prefill_stats().prefills - pfc0 == 2l,
                        "required: continuation prefill (start_pos > 0) serves on the GPU (M2.5)")
                    // ... and a CAPABILITY decline still panics — the legacy-GEMM pin forces
                    // the `planar` decline (the blob drivers are mul_mm-only; the quantized-X
                    // rail is dead)
                    set_metal_prefill_mulmm_legacy(true)
                    var sd = make_run_state(trkg.config, KVDtype.f16, KVDtype.f16)
                    var pf2_panicked = false
                    try {
                        eval_(trkg, sd, kprompt)
                    } recover {
                        pf2_panicked = true
                    }
                    set_metal_prefill_mulmm_legacy(false)
                    set_metal_prefill_min_npos(64l)
                    t |> success(pf2_panicked, "required: the legacy-GEMM pin (a `planar` capability decline) panics instead of CPU fallback")
                    delete sd
                    set_metal_mode(MetalMode.off)
                    sess |> clear()   // borrowed handles — delete of array<T?> cascades to the pointees
                    unsafe {
                        delete ws
                        delete sess
                    }
                    delete cur
                    delete one
                    delete sa
                    delete sb
                    delete sc
                }
                delete trk
                delete trkg
            } else {
                feint("Llama-3.2-1B Q4_K_M not present (llama-quantize the Q8 source); K-quant cells skipped\n")
            }

            // ===== the dim ladder (M2.6): batch dim caps retired =====
            // past ADD_RMS_MAX_DIM the batched add+rms runs as the unfused pair, and past the
            // mv8 probe's dim 3072 the B > 4 GEMM tier serves any width — an 8B (dim 4096)
            // batches at every B and a 70B-class kq model (dim 8192) serves all three paths.
            // Both are large-tier: they run only under DASLLAMA_PARITY_FULL (the pre-commit gate).
            if (arm_on(t, "dim8b")) {
                let path8 = path_join(models_dir(), "Meta-Llama-3.1-8B-Instruct-Q8_0.gguf")
                if (model_available(t, path8)) {
                    var tr8 <- blob_twin(t, path8, 512l)
                    matrix_cell(t, tr8, KVDtype.f16, "8B-q8/s32/f16", "", "", "")
                    dim_ladder_batch_cell(t, tr8, 8, "8B dim4096")   // the q8 GEMM tier past B=4
                    delete tr8
                }
            }
            if (arm_on(t, "dim70b")) {
                let path70 = path_join(models_dir(), "Llama-3.3-70B-Instruct-Q4_K_M.gguf")
                if (model_available(t, path70)) {
                    var tr70 <- blob_twin(t, path70, 128l)
                    matrix_cell(t, tr70, KVDtype.f16, "70B-kq/s32/f16", "", "", "")
                    dim_ladder_batch_cell(t, tr70, 12, "70B dim8192")   // the kq mul_mm twin at dim 8192
                    delete tr70
                }
            }

            // the decline tables surface in the human report too, and the leak gate holds
            metal_decode_stage_report()
            metal_decode_shutdown()
            metal_prefill_shutdown()
            t |> equal(metal_log_leaks_if_any(), 0l)
            clear_kernel_backend_pin()
            select_kernel_backend(prior_backend)
            set_wscale_f16(prior_wscale)
            delete tr
            delete trg
        } else {
            feint("das_metal is not built on this platform; nothing to assert\n")
        }
    }
}

[unused_argument(t, tr, trg, tag, tol)]   // off-Apple the gated body is empty
def private cont_logits_cell(t : T?; tr, trg : Model; tag : string; tol : float) {
    // continuation-prefill NUMERIC parity: CPU-prefill a head, GPU-prefill the tail chunk
    // (start_pos > 0), compare last-position logits against the all-CPU truth within `tol`.
    // Pins the panel-overrun class (the K/V GEMMs write mp FULL M-tile rows at the start_pos
    // row offset; short-sized panels silently corrupted pool neighbors — 16-31 logits off on
    // the qd != dim families, allocation-order-lucky zero on llama). Tolerance, not tokens:
    // near-tie greedy picks legitimately flip under GPU reduction-order noise.
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let c = tr.config
        let vocab = c.vocab_size
        let full <- encode(tr, "The old lighthouse keeper watched the storm roll in from the", true, false)
        let n = long_length(full)
        var head : array<int64>
        var tail : array<int64>
        head |> reserve(n - 2l)
        for (i in range64(n - 2l)) {
            head |> push(full[i])
        }
        tail |> push(full[n - 2l])
        tail |> push(full[n - 1l])
        var served = 0l
        var d = 0.0
        var cpk = 0l
        var gpk = 0l
        with_job_que() {
            setup_dasllama_jobque()
            var sc = make_run_state(c, KVDtype.f16, KVDtype.f16)
            eval_(tr, sc, full)
            var sb = make_run_state(c, KVDtype.f16, KVDtype.f16)
            eval_(tr, sb, head)
            let pf0 = metal_prefill_stats().prefills
            select_prefill_override("metal")
            set_metal_prefill_min_npos(1l)
            eval_(trg, sb, tail)   // the GPU tail chunk rides the blob twin
            set_metal_prefill_min_npos(64l)
            select_prefill_override("")
            served = metal_prefill_stats().prefills - pf0
            for (i in range64(vocab)) {
                d = max(d, abs(sb.logits[i] - sc.logits[i]))
            }
            cpk = greedy_top(sc.logits, vocab)
            gpk = greedy_top(sb.logits, vocab)
            delete sb
            delete sc
        }
        to_log(LOG_INFO, "{tag} continuation eyeball: CPU next '{eyeball_str(piece_(tr, cpk))}' vs GPU next '{eyeball_str(piece_(tr, gpk))}' (maxd {d})\n")
        t |> success(served > 0l, "{tag}: continuation prefill ENGAGED ({served} served)")
        t |> success(d < tol, "{tag}: continuation-prefill logits within {tol} of CPU (measured {d})")
        delete head
        delete tail
    }
}

[unused_argument(t, path, family, tag, expect_needs, want, kv2, cont_tol)]   // off-Apple the gated body is empty
def private family_cells(t : T?; path, family, tag : string; expect_needs : uint; want : string;
                         kv2 : KVDtype = KVDtype.f16; cont_tol : float = 0.0) {
    // one family's row: pin the metal_needs derivation bits (uint in the signature — MetalNeed
    // does not exist off-Apple, and signatures compile on every platform), then the three path
    // cells — each asserts ENGAGE ("") or DECLINE with the named need_* reason. Wave stages flip
    // these. An ENGAGE row ("" cells) ALSO proves token parity: a metal-overrides generate must
    // equal the CPU truth. kv2 != f16 adds a second cell row on that KV codec (its rope-store
    // twin); cont_tol > 0 adds the continuation-prefill numeric cell (cont_logits_cell).
    static_if (typeinfo builtin_module_exists(das_metal)) {
        if (!family_on(t, family) || !arm_on(t, "fam-{family}") || !model_available(t, path)) {
            return
        }
        var tr <- load_model_(path, QuantMode.q8)
        tr.config.seq_len = min(tr.config.seq_len, 512l)
        let derived = metal_needs(tr)
        let expected = MetalNeed(expect_needs)   // uint→named-bitfield relabel so the message prints bit NAMES
        t |> success((uint(derived) & expect_needs) == expect_needs,
            "{tag}: metal_needs derives {expected} (got {derived})")
        if (want == "") {
            // ENGAGE row: the cells + GPU generate ride the blob twin; the planar model keeps
            // the CPU truths (the ref-cache rail keys them by path)
            var trg <- blob_twin(t, path, 512l)
            matrix_cell(t, trg, KVDtype.f16, tag, want, want, want)
            if (kv2 != KVDtype.f16) {
                matrix_cell(t, trg, kv2, "{tag}+{kv2}", want, want, want)
            }
            let prompt <- encode(tr, counting_prompt(1, 30), true, false)
            var cpu_ids : array<int64>
            var gpu_ids : array<int64>
            with_job_que() {
                setup_dasllama_jobque()
                var s = make_run_state(tr.config, KVDtype.f16, KVDtype.f16)
                let ids <- generate(tr, s, prompt, length(prompt) + 8)
                cpu_ids := ids
                delete s
                select_decode_override("metal")
                select_prefill_override("metal")
                set_metal_prefill_min_npos(1l)
                var s2 = make_run_state(trg.config, KVDtype.f16, KVDtype.f16)
                let ids2 <- generate(trg, s2, prompt, length(prompt) + 8)
                gpu_ids := ids2
                delete s2
                set_metal_prefill_min_npos(64l)
                select_decode_override("")
                select_prefill_override("")
            }
            log_gen_texts(tr, tag, cpu_ids, gpu_ids)
            t |> success(same(cpu_ids, gpu_ids),
                "{tag}: metal-overrides generate matches the CPU truth token-for-token")
            delete cpu_ids
            delete gpu_ids
            if (cont_tol > 0.0) {
                cont_logits_cell(t, tr, trg, tag, cont_tol)
            }
            delete trg
        } else {
            // DECLINE row: the planar model records the capability reason (it out-ranks
            // `planar` in the gates) and its CPU fallback serves quietly
            matrix_cell(t, tr, KVDtype.f16, tag, want, want, want)
            if (kv2 != KVDtype.f16) {
                matrix_cell(t, tr, kv2, "{tag}+{kv2}", want, want, want)
            }
        }
        delete tr
    }
}

[unused_argument(t, path, family, tag, expect_needs, cont_tol)]   // off-Apple the gated body is empty
def private qwen35_row(t : T?; path, family, tag : string; expect_needs : uint; cont_tol : float) {
    // qwen3.5/3.6 hybrid (Wave D): 3-in-4 Gated-DeltaNet layers + gated attention. Prefill +
    // decode ENGAGE on the blob twin; batch has NO metal arm by design — eval_batch CPU-declines
    // hybrids (attn_is_std false) into per-row forward, which rides the metal DECODE override.
    // The batch cell therefore asserts the FALLBACK shape: metal batch steps stay 0 while the
    // single-decode counter serves both rows.
    static_if (typeinfo builtin_module_exists(das_metal)) {
        if (!family_on(t, family) || !arm_on(t, "fam-{family}") || !model_available(t, path)) {
            return
        }
        var tr <- load_model_(path, QuantMode.q8)
        tr.config.seq_len = min(tr.config.seq_len, 512l)
        let derived = metal_needs(tr)
        let expected = MetalNeed(expect_needs)   // uint→named-bitfield relabel so the message prints bit NAMES
        t |> success((uint(derived) & expect_needs) == expect_needs,
            "{tag}: metal_needs derives {expected} (got {derived})")
        var trg <- blob_twin(t, path, 512l)
        let c = trg.config
        let vocab = c.vocab_size
        let pf0 = metal_prefill_stats().prefills
        let de0 = metal_decode_stats().decodes
        let ba0 = metal_batch_decode_stats().steps
        with_job_que() {
            setup_dasllama_jobque()
            var sa = make_run_state(c, KVDtype.f16, KVDtype.f16)
            var sb = make_run_state(c, KVDtype.f16, KVDtype.f16)
            let pa <- encode(trg, counting_prompt(1, 40), true, false)
            let pb <- encode(trg, counting_prompt(5, 44), true, false)
            t |> success(select_prefill_override("metal"), "{tag}: prefill override registered")
            t |> success(select_decode_override("metal"), "{tag}: decode override registered")
            set_metal_prefill_min_npos(1l)
            eval_(trg, sa, pa)
            eval_(trg, sb, pb)
            var cura = greedy_top(sa.logits, vocab)
            let curb = greedy_top(sb.logits, vocab)
            for (_i in range(2)) {
                var one <- [cura]
                eval_(trg, sa, one)
                delete one
                cura = greedy_top(sa.logits, vocab)
            }
            let de1 = metal_decode_stats().decodes
            var ws <- create_batch_workspace_(trg)
            var sess : array<Session?>
            unsafe {
                sess |> push(addr(sa))
                sess |> push(addr(sb))
            }
            var cur <- [cura, curb]
            eval_batch_(trg, ws, sess, cur)
            t |> success(metal_decode_stats().decodes - de1 >= 2l,
                "{tag} batch: per-row fallback serves each row on the GPU decode path")
            select_decode_override("")
            select_prefill_override("")
            set_metal_prefill_min_npos(64l)
            sess |> clear()   // drop the borrowed handles: delete of array<T?> cascades to the pointees
            unsafe {
                delete ws
                delete sess
            }
            delete cur
            delete sa
            delete sb
        }
        t |> success(metal_prefill_stats().prefills - pf0 > 0l, "{tag} prefill: ENGAGES on the GPU")
        t |> success(metal_decode_stats().decodes - de0 > 0l, "{tag} decode: ENGAGES on the GPU")
        t |> equal(metal_batch_decode_stats().steps - ba0, 0l)
        let prompt <- encode(tr, counting_prompt(1, 30), true, false)
        var cpu_ids : array<int64>
        var gpu_ids : array<int64>
        with_job_que() {
            setup_dasllama_jobque()
            var s = make_run_state(tr.config, KVDtype.f16, KVDtype.f16)
            let ids <- generate(tr, s, prompt, length(prompt) + 8)
            cpu_ids := ids
            delete s
            select_decode_override("metal")
            select_prefill_override("metal")
            set_metal_prefill_min_npos(1l)
            var s2 = make_run_state(trg.config, KVDtype.f16, KVDtype.f16)
            let ids2 <- generate(trg, s2, prompt, length(prompt) + 8)
            gpu_ids := ids2
            delete s2
            set_metal_prefill_min_npos(64l)
            select_decode_override("")
            select_prefill_override("")
        }
        log_gen_texts(tr, tag, cpu_ids, gpu_ids)
        t |> success(same(cpu_ids, gpu_ids),
            "{tag}: metal-overrides generate matches the CPU truth token-for-token")
        delete cpu_ids
        delete gpu_ids
        if (cont_tol > 0.0) {
            cont_logits_cell(t, tr, trg, tag, cont_tol)
        }
        delete trg
        delete tr
    }
}

// A Metal-4 tensor (tmm2d) twin SERVES only when the box's tune sidecar crowns its family, so a
// box whose sidecar is stale or absent crowns nothing and every twin ships unexercised. Force the
// crowns, recompile the PSOs, and assert the model still produces the CPU's tokens with twins in
// the graph. A new tensor family joins CROWNED below.
[unused_argument(t, path)]   // off-Apple the gated body is empty
def private tensor_twin_row(t : T?; path, tag : string) {   // nolint:LINT012,LINT019 — tag is read by the das_metal arm
    static_if (typeinfo builtin_module_exists(das_metal)) {
        if (!family_on(t, "llama") || !arm_on(t, "tensor")) {
            return
        }
        // the carriers are provisioned (fetch_models.das), so absence is a RED, never a skip -
        // a box that skips here ships every tensor twin unexercised
        if (!stat(path).is_valid) {
            t |> success(false, "tensor twin carrier missing: {path} - bin/daslang modules/dasLLAMA/performance/fetch_models.das -- --fetch")
            return
        }
        let CROWNED = ("attn_avmm,attn_qkmm,gemm64b_q8,gemmb_q8,gemmb_sk_q8,kq_mulmm_k4," +
            "kq_mulmm_k5,kq_mulmm_k6,moe_mulmm_mx4,moe_mulmm_q8,mulmm_bf16,mulmm_q8")
        var tr <- load_model_(path, QuantMode.q8)
        tr.config.seq_len = min(tr.config.seq_len, 512l)
        var trg <- blob_twin(t, path, 512l)
        let prompt <- encode(tr, counting_prompt(1, 30), true, false)
        var cpu_ids : array<int64>
        var gpu_ids : array<int64>
        with_job_que() {
            setup_dasllama_jobque()
            var s = make_run_state(tr.config, KVDtype.f16, KVDtype.f16)
            let ids <- generate(tr, s, prompt, length(prompt) + 8)
            cpu_ids := ids
            delete s
            // crowns are read when the PSOs compile, so they must be set before the rebuild
            set_metal_tensor_crowns(CROWNED)
            metal_decode_shutdown()
            metal_prefill_shutdown()
            metal_kernel_coverage_reset()
            select_decode_override("metal")
            select_prefill_override("metal")
            set_metal_prefill_min_npos(1l)
            var s2 = make_run_state(trg.config, KVDtype.f16, KVDtype.f16)
            let ids2 <- generate(trg, s2, prompt, length(prompt) + 8)
            gpu_ids := ids2
            delete s2
            set_metal_prefill_min_npos(64l)
            select_decode_override("")
            select_prefill_override("")
        }
        var twins = 0
        var cov <- metal_kernel_coverage()
        for (name in keys(cov)) {
            // cov[name] would INSERT into a table keys() has locked — read through the safe form
            let n = cov?[name] ?? 0l
            if (n > 0l && ends_with(name, "_t")) {
                twins++
                to_log(LOG_INFO, "tensor twin served: {name} = {n}\n")
            }
        }
        delete cov
        // without this the cell passes vacuously the day crowning stops taking effect
        t |> success(twins > 0, "{tag}: a crowned tmm2d twin actually served ({twins} of them)")
        log_gen_texts(tr, tag, cpu_ids, gpu_ids)
        t |> success(same(cpu_ids, gpu_ids),
            "{tag}: crowned tmm2d generate matches the CPU truth token-for-token")
        delete cpu_ids
        delete gpu_ids
        set_metal_tensor_crowns("")
        metal_decode_shutdown()
        metal_prefill_shutdown()
        delete trg
        delete tr
    }
}

[unused_argument(t, path)]   // off-Apple the gated body is empty
def private qwen2moe_row(t : T?; path : string) {
    // qwen2moe (Qwen1.5-MoE-A2.7B): std attention + softmax-no-renorm select (gmode 2) + the
    // sigmoid-gated shared expert. Decode + prefill ENGAGE; batch DECLINES by design (shexp
    // has no batch arm) — the decline cell runs on the PLANAR model: the blob twin's CPU
    // batch fallback would trip the blob-only panic (std attention runs the CPU batch stack).
    static_if (typeinfo builtin_module_exists(das_metal)) {
        if (!family_on(t, "qwen2moe") || !arm_on(t, "fam-qwen2moe") || !model_available(t, path)) {
            return
        }
        var tr <- load_model_(path, QuantMode.q8)
        tr.config.seq_len = min(tr.config.seq_len, 512l)
        let derived = metal_needs(tr)
        t |> success((uint(derived) & uint(MetalNeed.neox | MetalNeed.qkv_bias)) == uint(MetalNeed.neox | MetalNeed.qkv_bias),
            "qwen2moe: metal_needs derives neox|qkv_bias (got {derived})")
        var trg <- blob_twin(t, path, 512l)
        let c = trg.config
        let vocab = c.vocab_size
        let pf0 = metal_prefill_stats().prefills
        let de0 = metal_decode_stats().decodes
        let ba0 = metal_batch_decode_stats().steps
        var bad0 <- metal_batch_decode_declines()
        with_job_que() {
            setup_dasllama_jobque()
            var sa = make_run_state(c, KVDtype.f16, KVDtype.f16)
            var sb = make_run_state(c, KVDtype.f16, KVDtype.f16)
            let pa <- encode(trg, counting_prompt(1, 40), true, false)
            let pb <- encode(trg, counting_prompt(5, 44), true, false)
            t |> success(select_prefill_override("metal"), "qwen2moe: prefill override registered")
            t |> success(select_decode_override("metal"), "qwen2moe: decode override registered")
            set_metal_prefill_min_npos(1l)
            eval_(trg, sa, pa)
            eval_(trg, sb, pb)
            var cura = greedy_top(sa.logits, vocab)
            for (_i in range(2)) {
                var one <- [cura]
                eval_(trg, sa, one)
                delete one
                cura = greedy_top(sa.logits, vocab)
            }
            // batch decline cell on the PLANAR model: the gate records `graph`, CPU serves
            var sc = make_run_state(tr.config, KVDtype.f16, KVDtype.f16)
            var sd = make_run_state(tr.config, KVDtype.f16, KVDtype.f16)
            eval_(tr, sc, pa)
            eval_(tr, sd, pb)
            var ws <- create_batch_workspace_(tr)
            var sess : array<Session?>
            unsafe {
                sess |> push(addr(sc))
                sess |> push(addr(sd))
            }
            var cur <- [greedy_top(sc.logits, vocab), greedy_top(sd.logits, vocab)]
            eval_batch_(tr, ws, sess, cur)
            select_decode_override("")
            select_prefill_override("")
            set_metal_prefill_min_npos(64l)
            sess |> clear()   // drop the borrowed handles: delete of array<T?> cascades to the pointees
            unsafe {
                delete ws
                delete sess
            }
            delete cur
            delete sa
            delete sb
            delete sc
            delete sd
        }
        t |> success(metal_prefill_stats().prefills - pf0 > 0l, "qwen2moe prefill: ENGAGES on the GPU")
        t |> success(metal_decode_stats().decodes - de0 > 0l, "qwen2moe decode: ENGAGES on the GPU")
        var bad <- metal_batch_decode_declines()
        t |> equal(metal_batch_decode_stats().steps - ba0, 0l)
        t |> success((bad?["graph"] ?? 0l) - (bad0?["graph"] ?? 0l) > 0l,
            "qwen2moe batch: declines with reason 'graph' (no shexp batch arm)")
        delete bad0
        delete bad
        let prompt <- encode(tr, counting_prompt(1, 30), true, false)
        var cpu_ids : array<int64>
        var gpu_ids : array<int64>
        with_job_que() {
            setup_dasllama_jobque()
            var s = make_run_state(tr.config, KVDtype.f16, KVDtype.f16)
            let ids <- generate(tr, s, prompt, length(prompt) + 8)
            cpu_ids := ids
            delete s
            select_decode_override("metal")
            select_prefill_override("metal")
            set_metal_prefill_min_npos(1l)
            var s2 = make_run_state(trg.config, KVDtype.f16, KVDtype.f16)
            let ids2 <- generate(trg, s2, prompt, length(prompt) + 8)
            gpu_ids := ids2
            delete s2
            set_metal_prefill_min_npos(64l)
            select_decode_override("")
            select_prefill_override("")
        }
        log_gen_texts(tr, "qwen2moe", cpu_ids, gpu_ids)
        t |> success(same(cpu_ids, gpu_ids),
            "qwen2moe: metal-overrides generate matches the CPU truth token-for-token")
        delete cpu_ids
        delete gpu_ids
        cont_logits_cell(t, tr, trg, "qwen2moe-a2.7b/q8/f16", 4.0)
        delete trg
        delete tr
    }
}

[unused_argument(t, path)]   // off-Apple the gated body is empty
def private gemma4e_row(t : T?; path : string) {
    // gemma-4 E-series (E4B): PLE (per-layer embeddings — MetalNeed.ple) + cross-layer KV
    // sharing (the last 18 layers are Q-only and alias layer 22/23's slab/panels). Decode +
    // prefill ENGAGE on the blob twin; batch has NO ple arm — the decline cell runs on the
    // PLANAR model (the blob twin's CPU batch fallback would trip the blob-only panic).
    static_if (typeinfo builtin_module_exists(das_metal)) {
        if (!family_on(t, "gemma4e") || !arm_on(t, "fam-gemma4e") || !model_available(t, path)) {
            return
        }
        var tr <- load_model_(path, QuantMode.q8)
        tr.config.seq_len = min(tr.config.seq_len, 512l)
        let want_bits = (MetalNeed.neox | MetalNeed.qk_norm | MetalNeed.v_norm | MetalNeed.attn_scale |
            MetalNeed.sliding | MetalNeed.pre_post_norm | MetalNeed.out_scale | MetalNeed.geglu |
            MetalNeed.ple | MetalNeed.qd_neq_dim)
        let derived = metal_needs(tr)
        t |> success((uint(derived) & uint(want_bits)) == uint(want_bits),
            "gemma4e-e4b: metal_needs derives {want_bits} (got {derived})")
        var nshared = 0l
        for (l in range64(tr.config.n_layers)) {
            if (tr.kv_src[l] != l) {
                nshared++
            }
        }
        t |> success(nshared > 0l, "gemma4e-e4b: fixture carries cross-layer KV sharing ({nshared} shared layers)")
        var trg <- blob_twin(t, path, 512l)
        let c = trg.config
        let vocab = c.vocab_size
        let pf0 = metal_prefill_stats().prefills
        let de0 = metal_decode_stats().decodes
        let ba0 = metal_batch_decode_stats().steps
        var bad0 <- metal_batch_decode_declines()
        with_job_que() {
            setup_dasllama_jobque()
            var sa = make_run_state(c, KVDtype.f16, KVDtype.f16)
            let pa <- encode(trg, counting_prompt(1, 40), true, false)
            let pb <- encode(trg, counting_prompt(5, 44), true, false)
            t |> success(select_prefill_override("metal"), "gemma4e-e4b: prefill override registered")
            t |> success(select_decode_override("metal"), "gemma4e-e4b: decode override registered")
            set_metal_prefill_min_npos(1l)
            eval_(trg, sa, pa)
            var cura = greedy_top(sa.logits, vocab)
            for (_i in range(2)) {
                var one <- [cura]
                eval_(trg, sa, one)
                delete one
                cura = greedy_top(sa.logits, vocab)
            }
            // batch decline cell on the PLANAR model: the gate records need_ple, CPU serves
            var sc = make_run_state(tr.config, KVDtype.f16, KVDtype.f16)
            var sd = make_run_state(tr.config, KVDtype.f16, KVDtype.f16)
            eval_(tr, sc, pa)
            eval_(tr, sd, pb)
            var ws <- create_batch_workspace_(tr)
            var sess : array<Session?>
            unsafe {
                sess |> push(addr(sc))
                sess |> push(addr(sd))
            }
            var cur <- [greedy_top(sc.logits, vocab), greedy_top(sd.logits, vocab)]
            eval_batch_(tr, ws, sess, cur)
            select_decode_override("")
            select_prefill_override("")
            set_metal_prefill_min_npos(64l)
            sess |> clear()   // drop the borrowed handles: delete of array<T?> cascades to the pointees
            unsafe {
                delete ws
                delete sess
            }
            delete cur
            delete sa
            delete sc
            delete sd
        }
        t |> success(metal_prefill_stats().prefills - pf0 > 0l, "gemma4e-e4b prefill: ENGAGES on the GPU")
        t |> success(metal_decode_stats().decodes - de0 > 0l, "gemma4e-e4b decode: ENGAGES on the GPU")
        var bad <- metal_batch_decode_declines()
        t |> equal(metal_batch_decode_stats().steps - ba0, 0l)
        t |> success((bad?["need_ple"] ?? 0l) - (bad0?["need_ple"] ?? 0l) > 0l,
            "gemma4e-e4b batch: declines with need_ple (no PLE batch arm)")
        delete bad0
        delete bad
        let prompt <- encode(tr, counting_prompt(1, 30), true, false)
        var cpu_ids : array<int64>
        var gpu_ids : array<int64>
        with_job_que() {
            setup_dasllama_jobque()
            var s = make_run_state(tr.config, KVDtype.f16, KVDtype.f16)
            let ids <- generate(tr, s, prompt, length(prompt) + 8)
            cpu_ids := ids
            delete s
            select_decode_override("metal")
            select_prefill_override("metal")
            set_metal_prefill_min_npos(1l)
            var s2 = make_run_state(trg.config, KVDtype.f16, KVDtype.f16)
            let ids2 <- generate(trg, s2, prompt, length(prompt) + 8)
            gpu_ids := ids2
            delete s2
            set_metal_prefill_min_npos(64l)
            select_decode_override("")
            select_prefill_override("")
        }
        log_gen_texts(tr, "gemma4e-e4b", cpu_ids, gpu_ids)
        t |> success(same(cpu_ids, gpu_ids),
            "gemma4e-e4b: metal-overrides generate matches the CPU truth token-for-token")
        delete cpu_ids
        delete gpu_ids
        cont_logits_cell(t, tr, trg, "gemma4e-e4b/q8/f16", 8.0)
        delete trg
        delete tr
    }
}

[unused_argument(t, path, tag, tol_pf, tol_step)]   // off-Apple the gated body is empty
def private gemma4moe_row(t : T?; path : string; tag : string; tol_pf : float = 8.0; tol_step : float = 16.0) {
    // gemma4-26B-A4B (Wave C stage 3): the parallel dense-shared + routed MoE graph. TOKEN
    // parity is NOT a valid instrument here (wave ruling): the CPU router's double norm is an
    // inverse TEMPERATURE on the expert softmax and the model amplifies ANY float-class
    // difference to ~2.5 logits/step (the CPU's own f32-router run differs identically) — so
    // the cells are ENGAGE + shallow logits TOLERANCES (measured noise 2.1 / 5.7; bars ~3x).
    // Serial two-phase: CPU truths on the planar load, then the blob twin (27GB x2 resident
    // would flirt with the box) — 26.9GB > the 6GiB tier, DASLLAMA_PARITY_FULL-gated.
    static_if (typeinfo builtin_module_exists(das_metal)) {
        if (!family_on(t, "gemma4moe") || !arm_on(t, "fam-gemma4moe") || !model_available(t, path)) {
            return
        }
        var tr <- load_model_(path, QuantMode.q8)
        tr.config.seq_len = min(tr.config.seq_len, 512l)
        let c = tr.config
        t |> success(c.moe && c.moe_dense_shexp && c.n_expert == 128l && c.n_expert_used == 8l,
            "{tag}: MoE geometry loads (ne={c.n_expert} k={c.n_expert_used} nfe={c.n_ff_exp})")
        let derived = metal_needs(tr)
        let expected = MetalNeed.neox | MetalNeed.qk_norm | MetalNeed.sliding | MetalNeed.v_norm
        t |> success((derived & expected) == expected,
            "{tag}: metal_needs derives {expected} (got {derived})")
        let vocab = c.vocab_size
        var prompt <- encode(tr, counting_prompt(1, 20), true, false)
        var lg0 : array<float>
        var lg1 : array<float>
        var t0 = 0l
        with_job_que() {
            setup_dasllama_jobque()
            var s = make_run_state(c, KVDtype.f16, KVDtype.f16)
            eval_(tr, s, prompt)
            lg0 := s.logits
            t0 = greedy_top(s.logits, vocab)
            var one <- [t0]
            eval_(tr, s, one)
            delete one
            lg1 := s.logits
            delete s
        }
        delete tr
        var trg <- blob_twin(t, path, 512l)
        matrix_cell(t, trg, KVDtype.f16, "{tag}/q8/f16", "", "", "")
        var mpf = 0.0
        var m1 = 0.0
        var served = 0l
        var gpk0 = 0l
        var gpk1 = 0l
        with_job_que() {
            setup_dasllama_jobque()
            select_prefill_override("metal")
            select_decode_override("metal")
            set_metal_prefill_min_npos(1l)
            let pf0 = metal_prefill_stats().prefills
            let d0 = metal_decode_stats().decodes
            var s = make_run_state(trg.config, KVDtype.f16, KVDtype.f16)
            eval_(trg, s, prompt)
            for (v in range64(vocab)) {
                mpf = max(mpf, abs(lg0[v] - s.logits[v]))
            }
            gpk0 = greedy_top(s.logits, vocab)
            var one <- [t0]
            eval_(trg, s, one)
            delete one
            for (v in range64(vocab)) {
                m1 = max(m1, abs(lg1[v] - s.logits[v]))
            }
            gpk1 = greedy_top(s.logits, vocab)
            served = (metal_prefill_stats().prefills - pf0) + (metal_decode_stats().decodes - d0)
            set_metal_prefill_min_npos(64l)
            select_prefill_override("")
            select_decode_override("")
            delete s
        }
        t |> success(served == 2l, "{tag}: GPU prefill + forced step both served ({served}/2)")
        t |> success(mpf <= tol_pf, "{tag}: post-prefill logits within {tol_pf} of the CPU truth (measured {mpf})")
        t |> success(m1 <= tol_step, "{tag}: one forced step within {tol_step} of the CPU truth (measured {m1})")
        to_log(LOG_INFO, "{tag} tolerance cells: post-prefill maxd={mpf} (tol {tol_pf}), forced step maxd={m1} (tol {tol_step}); eyeball CPU '{eyeball_str(piece_(trg, t0))}'/'{eyeball_str(piece_(trg, greedy_top(lg1, vocab)))}' vs GPU '{eyeball_str(piece_(trg, gpk0))}'/'{eyeball_str(piece_(trg, gpk1))}'\n")
        delete lg0
        delete lg1
        delete prompt
        delete trg
    }
}

[unused_argument(t, path)]   // off-Apple the gated body is empty
def private gptoss_row(t : T?; path : string) {
    // gpt-oss-20b (Wave C stage 4): MXFP4 expert stacks + attention sinks + softmax_weight
    // routing + router/expert biases + swiglu_oai + wo out-bias. ENGAGE cells + shallow
    // logits tolerances (measured noise 0.23/0.29 — the tightest MoE envelope; bars ~8x).
    // 12GB > the 6GiB tier — DASLLAMA_PARITY_FULL-gated; serial two-phase like the 26B row.
    static_if (typeinfo builtin_module_exists(das_metal)) {
        if (!family_on(t, "gptoss") || !arm_on(t, "fam-gptoss") || !model_available(t, path)) {
            return
        }
        var tr <- load_model_(path, QuantMode.q8)
        tr.config.seq_len = min(tr.config.seq_len, 512l)
        let c = tr.config
        t |> success(c.moe && tr.experts_mx4 && c.attn_sinks && c.n_expert == 32l && c.n_expert_used == 4l,
            "gptoss-20b: MoE geometry loads (mx4={tr.experts_mx4} sinks={c.attn_sinks} ne={c.n_expert} k={c.n_expert_used})")
        let derived = metal_needs(tr)
        let expected = MetalNeed.neox | MetalNeed.sliding | MetalNeed.sinks | MetalNeed.out_bias
        t |> success((derived & expected) == expected,
            "gptoss-20b: metal_needs derives {expected} (got {derived})")
        let vocab = c.vocab_size
        var prompt <- encode(tr, counting_prompt(1, 20), true, false)
        var lg0 : array<float>
        var lg1 : array<float>
        var t0 = 0l
        with_job_que() {
            setup_dasllama_jobque()
            var s = make_run_state(c, KVDtype.f16, KVDtype.f16)
            eval_(tr, s, prompt)
            lg0 := s.logits
            t0 = greedy_top(s.logits, vocab)
            var one <- [t0]
            eval_(tr, s, one)
            delete one
            lg1 := s.logits
            delete s
        }
        delete tr
        var trg <- blob_twin(t, path, 512l)
        matrix_cell(t, trg, KVDtype.f16, "gptoss-20b/mx4/f16", "", "", "")
        var mpf = 0.0
        var m1 = 0.0
        var served = 0l
        var gpk0 = 0l
        var gpk1 = 0l
        with_job_que() {
            setup_dasllama_jobque()
            select_prefill_override("metal")
            select_decode_override("metal")
            set_metal_prefill_min_npos(1l)
            let pf0 = metal_prefill_stats().prefills
            let d0 = metal_decode_stats().decodes
            var s = make_run_state(trg.config, KVDtype.f16, KVDtype.f16)
            eval_(trg, s, prompt)
            for (v in range64(vocab)) {
                mpf = max(mpf, abs(lg0[v] - s.logits[v]))
            }
            gpk0 = greedy_top(s.logits, vocab)
            var one <- [t0]
            eval_(trg, s, one)
            delete one
            for (v in range64(vocab)) {
                m1 = max(m1, abs(lg1[v] - s.logits[v]))
            }
            gpk1 = greedy_top(s.logits, vocab)
            served = (metal_prefill_stats().prefills - pf0) + (metal_decode_stats().decodes - d0)
            set_metal_prefill_min_npos(64l)
            select_prefill_override("")
            select_decode_override("")
            delete s
        }
        t |> success(served == 2l, "gptoss-20b: GPU prefill + forced step both served ({served}/2)")
        t |> success(mpf <= 2.0, "gptoss-20b: post-prefill logits within 2.0 of the CPU truth (measured {mpf})")
        t |> success(m1 <= 4.0, "gptoss-20b: one forced step within 4.0 of the CPU truth (measured {m1})")
        to_log(LOG_INFO, "gptoss-20b tolerance cells: post-prefill maxd={mpf} (tol 2), forced step maxd={m1} (tol 4); eyeball CPU '{eyeball_str(piece_(trg, t0))}'/'{eyeball_str(piece_(trg, greedy_top(lg1, vocab)))}' vs GPU '{eyeball_str(piece_(trg, gpk0))}'/'{eyeball_str(piece_(trg, gpk1))}'\n")
        delete lg0
        delete lg1
        delete prompt
        delete trg
    }
}

[unused_argument(t, path)]   // off-Apple the gated body is empty
def private gemma2_window_row(t : T?; path : string) {
    // sliding-window masking parity: shrink the window post-load (config-driven everywhere —
    // gemma2 is single-θ, so rope is unaffected) and generate PAST it. The prompt-depth assert
    // guarantees decode crosses chunk 224, so whole below-window chunks are skipped (chlo >= 1)
    // and the comb's chlo skip is on the proven path, not just the boundary mask.
    static_if (typeinfo builtin_module_exists(das_metal)) {
        if (!family_on(t, "gemma2") || !arm_on(t, "fam-gemma2") || !model_available(t, path)) {
            return
        }
        var tr <- load_model_(path, QuantMode.q8)
        tr.config.seq_len = min(tr.config.seq_len, 512l)
        tr.config.sliding_window = 160l
        var trg <- blob_twin(t, path, 512l)
        trg.config.sliding_window = 160l   // the twin must ride the same shrunk window
        let prompt <- encode(tr, counting_prompt(1, 120), true, false)
        t |> success(long_length(prompt) > 224l,
            "gemma2-2b/window: prompt depth {long_length(prompt)} crosses chunk 224")
        var cpu_ids : array<int64>
        var gpu_ids : array<int64>
        var served_de = 0l
        var served_pf = 0l
        with_job_que() {
            setup_dasllama_jobque()
            var s = make_run_state(tr.config, KVDtype.f16, KVDtype.f16)
            let ids <- generate(tr, s, prompt, length(prompt) + 16)
            cpu_ids := ids
            delete s
            let de0 = metal_decode_stats().decodes
            let pf0 = metal_prefill_stats().prefills
            select_decode_override("metal")
            select_prefill_override("metal")
            set_metal_prefill_min_npos(1l)
            var s2 = make_run_state(trg.config, KVDtype.f16, KVDtype.f16)
            let ids2 <- generate(trg, s2, prompt, length(prompt) + 16)
            gpu_ids := ids2
            delete s2
            set_metal_prefill_min_npos(64l)
            select_decode_override("")
            select_prefill_override("")
            served_de = metal_decode_stats().decodes - de0
            served_pf = metal_prefill_stats().prefills - pf0
        }
        t |> success(served_de > 0l && served_pf > 0l,
            "gemma2-2b/window: both GPU paths engaged (decode {served_de}, prefill {served_pf})")
        log_gen_texts(tr, "gemma2-2b/window", cpu_ids, gpu_ids)
        t |> success(same(cpu_ids, gpu_ids),
            "gemma2-2b/window: window-crossing generate matches the CPU truth token-for-token")
        delete cpu_ids
        delete gpu_ids
        delete tr
        delete trg
    }
}

// two CPU-prefilled streams through `nsteps` greedy eval_batch steps (whatever decode override
// is selected) — returns the interleaved sampled tokens, the B=2 parity arm's shape
def private batch_pair_steps(trp, trd : Model; pa, pb : array<int64>; nsteps : int) : array<int64> {
    let c = trp.config
    let vocab = c.vocab_size
    var sa = make_run_state(c, KVDtype.f16, KVDtype.f16)
    var sb = make_run_state(c, KVDtype.f16, KVDtype.f16)
    eval_(trp, sa, pa)
    eval_(trp, sb, pb)
    var out : array<int64>
    out |> reserve(2 * (nsteps + 1))
    var ws <- create_batch_workspace_(trp)
    var sess : array<Session?>
    unsafe {
        sess |> push(addr(sa))
        sess |> push(addr(sb))
    }
    var cur <- [greedy_top(sa.logits, vocab), greedy_top(sb.logits, vocab)]
    for (_i in range(nsteps)) {
        out |> push(cur[0])
        out |> push(cur[1])
        eval_batch_(trd, ws, sess, cur)
        cur[0] = greedy_top(sa.logits, vocab)
        cur[1] = greedy_top(sb.logits, vocab)
    }
    out |> push(cur[0])
    out |> push(cur[1])
    sess |> clear()   // borrowed handles — delete of array<T?> cascades to the pointees
    unsafe {
        delete ws
        delete sess
    }
    delete cur
    delete sa
    delete sb
    return <- out
}

// the B=2 cells' eyeball rail: de-interleave the pair streams and log each as text
def private log_pair_texts(m : Model; tag : string; cpu, gpu : array<int64>) {
    for (ph in range(2)) {
        var c <- [for (i in range(length(cpu))); cpu[i]; where i % 2 == ph]
        var g <- [for (i in range(length(gpu))); gpu[i]; where i % 2 == ph]
        to_log(LOG_INFO, "{tag} stream {ph} CPU text: {eyeball_text(m, c)}\n")
        to_log(LOG_INFO, "{tag} stream {ph} GPU text: {eyeball_text(m, g)}\n")
        delete c
        delete g
    }
}

// B=4 batched steps under TEACHER-FORCED tokens (deterministic synthetic feed, identical on
// the CPU and GPU passes — no greedy feedback, so a near-tie flip cannot compound), returning
// the four streams' final-step logits concatenated for a tolerance compare.
def private batch_quad_forced_logits(trp, trd : Model; pa, pb, pc, pd : array<int64>; nsteps : int) : array<float> {
    let c = trp.config
    let vocab = c.vocab_size
    var sa = make_run_state(c, KVDtype.f16, KVDtype.f16)
    var sb = make_run_state(c, KVDtype.f16, KVDtype.f16)
    var sc = make_run_state(c, KVDtype.f16, KVDtype.f16)
    var sd = make_run_state(c, KVDtype.f16, KVDtype.f16)
    eval_(trp, sa, pa)
    eval_(trp, sb, pb)
    eval_(trp, sc, pc)
    eval_(trp, sd, pd)
    var ws <- create_batch_workspace_(trp)
    var sess : array<Session?>
    unsafe {
        sess |> push(addr(sa))
        sess |> push(addr(sb))
        sess |> push(addr(sc))
        sess |> push(addr(sd))
    }
    var cur <- [0l, 0l, 0l, 0l]
    for (st in range(nsteps)) {
        for (i in range(4)) {
            cur[i] = 1000l + int64(st) * 7l + int64(i)
        }
        eval_batch_(trd, ws, sess, cur)
    }
    var out : array<float>
    out |> reserve(vocab * 4l)
    out |> push_from(sa.logits, sb.logits, sc.logits, sd.logits)
    sess |> clear()   // borrowed handles — delete of array<T?> cascades to the pointees
    unsafe {
        delete ws
        delete sess
    }
    delete cur
    delete sa
    delete sb
    delete sc
    delete sd
    return <- out
}

[unused_argument(t, path)]   // off-Apple the gated body is empty
def private gemma4_hetero_q8_row(t : T?; path : string) {
    // the q8-format twin of the hetero row: batched q8 sites ride the fixed-B mul_mv rail
    // (MetalQ8MvB2/B4 + the no-wv [wq|wk] cat3 stream + the wo xs4 sliding twin) — a kernel
    // class the k4 row's kq planes never dispatch. Counting generates only (the q8 file's
    // near-tie landscape differs from k4's, so a freeform arm would ride sub-noise ties):
    // the single generate covers the single-stream q8 hetero sites, B=2 token parity covers
    // MvB2, and the B=4 cell is a TEACHER-FORCED LOGITS compare — on this file the counting
    // continuations themselves hit a step-3 near-tie (the portable and arm64-gen CPU truths
    // pick different tokens there), so B=4 token equality is not a valid bar; forced feeds +
    // tolerance still pin the real corruption class. The cont cell gates the q8 prefill panels.
    static_if (typeinfo builtin_module_exists(das_metal)) {
        if (!family_on(t, "gemma4") || !arm_on(t, "fam-gemma4") || !model_available(t, path)) {
            return
        }
        var tr <- load_model_(path, QuantMode.q8)
        tr.config.seq_len = min(tr.config.seq_len, 512l)
        var trg <- blob_twin(t, path, 512l)
        let count_prompt <- encode(tr, counting_prompt(1, 30), true, false)
        let count_prompt2 <- encode(tr, counting_prompt(7, 36), true, false)
        let count_prompt3 <- encode(tr, counting_prompt(3, 41), true, false)
        let count_prompt4 <- encode(tr, counting_prompt(11, 29), true, false)
        var cpu_ids : array<int64>
        var gpu_ids : array<int64>
        var cpu_b2 : array<int64>
        var gpu_b2 : array<int64>
        var served_bat = 0l
        var d4 = 0.0
        var b4c : array<int64>
        var b4g : array<int64>
        with_job_que() {
            setup_dasllama_jobque()
            cpu_ids <- cached_ids(path, "gen_c1_30_n8") $ {   // nolint:PERF030,LINT019 — fresh empty target, single move; fires only on the das_metal arm
                var s = make_run_state(tr.config, KVDtype.f16, KVDtype.f16)
                var ids <- generate(tr, s, count_prompt, length(count_prompt) + 8)
                delete s
                return <- ids
            }
            select_decode_override("metal")
            var g2 <- split_generate(tr, trg, count_prompt, length(count_prompt) + 8)
            gpu_ids := g2
            delete g2
            select_decode_override("")
            cpu_b2 <- cached_ids(path, "b2pair_c1c7_n4") $ {   // nolint:PERF030,LINT019 — fresh empty target, single move; fires only on the das_metal arm
                return <- batch_pair_steps(tr, tr, count_prompt, count_prompt2, 4)
            }
            var cpu_bl <- cached_vals(path, "b4forced_n4") $ {
                return <- batch_quad_forced_logits(tr, tr, count_prompt, count_prompt2, count_prompt3, count_prompt4, 4)
            }
            let ba0 = metal_batch_decode_stats().steps
            select_decode_override("metal")
            gpu_b2 <- batch_pair_steps(tr, trg, count_prompt, count_prompt2, 4)   // nolint:PERF030,LINT019 — fresh empty target, single move; fires only on the das_metal arm
            var gpu_bl <- batch_quad_forced_logits(tr, trg, count_prompt, count_prompt2, count_prompt3, count_prompt4, 4)
            select_decode_override("")
            served_bat = metal_batch_decode_stats().steps - ba0
            for (i in range(length(cpu_bl))) {
                d4 = max(d4, abs(cpu_bl[i] - gpu_bl[i]))
            }
            let vocab = tr.config.vocab_size
            for (s4 in range64(4l)) {
                b4c |> push(greedy_top_at(cpu_bl, s4 * vocab, vocab))
                b4g |> push(greedy_top_at(gpu_bl, s4 * vocab, vocab))
            }
            delete cpu_bl
            delete gpu_bl
        }
        log_gen_texts(tr, "gemma4-12b/q8", cpu_ids, gpu_ids)
        log_pair_texts(tr, "gemma4-12b/q8 B=2", cpu_b2, gpu_b2)
        to_log(LOG_INFO, "gemma4-12b/q8 B=4 eyeball: CPU forced-final picks '{eyeball_text(tr, b4c)}' vs GPU '{eyeball_text(tr, b4g)}' (maxd {d4})\n")
        t |> success(same(cpu_ids, gpu_ids),
            "gemma4-12b/q8: metal-decode generate matches the CPU truth token-for-token")
        t |> success(served_bat > 0l, "gemma4-12b/q8: the hetero q8 batch ENGAGED ({served_bat} served)")
        t |> success(same(cpu_b2, gpu_b2),
            "gemma4-12b/q8: B=2 batched decode (MvB2) matches the CPU truth token-for-token")
        t |> success(d4 < 8.0,
            "gemma4-12b/q8: B=4 forced-feed batched logits (MvB4) within 8 of CPU (measured {d4})")
        cont_logits_cell(t, tr, trg, "gemma4-12b/q8", 8.0)
        delete cpu_ids
        delete gpu_ids
        delete cpu_b2
        delete gpu_b2
        delete b4c
        delete b4g
        delete tr
        delete trg
    }
}

[unused_argument(t, path)]   // off-Apple the gated body is empty
def private gemma4_hetero_row(t : T?; path : string) {
    // the stage-3 heterogeneous row: needs-derivation pin, per-path cells (all three ENGAGE),
    // a FREEFORM decode-only forced-feed tolerance cell, a counting both-paths generate, a B=2
    // batched-decode parity, and the continuation-prefill numeric cell —
    // the per-class geometry, V-from-K, v_norm, out_scale, dual-theta p-RoPE and the
    // suppress+softcap gpu-logits tail all sit on the proven paths. The both-paths generate is
    // COUNTING on purpose: gemma4's 48-layer big-magnitude stack amplifies GPU reduction-order
    // noise to ~1-5 logits (CPU truth is thread-count-stable; the proven decode driver shows
    // the same scale), so a freeform first token can sit on a sub-noise near-tie and flip
    // without any driver bug — counting margins dwarf the noise, and real corruption (the
    // 16-31-logit panel-overrun class) still breaks it. The freeform arm is FORCED-FEED
    // TOLERANCE over a cached 128-token stream: token equality on a freeform prompt reds on
    // legitimate CPU-trajectory near-tie moves with the GPU side CORRECT — both decoders force
    // through the same cached tokens, so real corruption still reds, near-tie flips can't, and
    // the cache is a FEED, not a truth (staleness harmless). Both sides land in the log as
    // text for the eyeball rail.
    static_if (typeinfo builtin_module_exists(das_metal)) {
        if (!family_on(t, "gemma4") || !arm_on(t, "fam-gemma4") || !model_available(t, path)) {
            return
        }
        var tr <- load_model_(path, QuantMode.q8)
        tr.config.seq_len = min(tr.config.seq_len, 512l)
        let derived = metal_needs(tr)
        let want_bits = (MetalNeed.neox | MetalNeed.qk_norm | MetalNeed.sliding | MetalNeed.pre_post_norm |
            MetalNeed.geglu | MetalNeed.v_norm | MetalNeed.attn_scale | MetalNeed.out_scale |
            MetalNeed.qd_neq_dim)
        t |> success((uint(derived) & uint(want_bits)) == uint(want_bits),
            "gemma4-12b/k4: metal_needs derives {want_bits} (got {derived})")
        var trg <- blob_twin(t, path, 512l)
        matrix_cell(t, trg, KVDtype.f16, "gemma4-12b/k4/f16", "", "", "")
        let prompt <- encode(tr, "The old lighthouse keeper watched the storm roll in from the", true, false)
        let count_prompt <- encode(tr, counting_prompt(1, 30), true, false)
        let count_prompt2 <- encode(tr, counting_prompt(7, 36), true, false)
        var cpu_ids : array<int64>
        var gpu_picks : array<int64>
        var cpu_cnt_ids : array<int64>
        var both_ids : array<int64>
        var cpu_bat : array<int64>
        var gpu_bat : array<int64>
        var served_pf = 0l
        var served_bat = 0l
        var served_free = 0l
        var fmaxd = 0.0
        with_job_que() {
            setup_dasllama_jobque()
            cpu_ids <- cached_ids(path, "gen_free_n128") $ {   // nolint:PERF030,LINT019 — fresh empty target, single move; fires only on the das_metal arm
                var s = make_run_state(tr.config, KVDtype.f16, KVDtype.f16)
                var ids <- generate(tr, s, prompt, length(prompt) + 128)
                delete s
                return <- ids
            }
            cpu_cnt_ids <- cached_ids(path, "gen_c1_30_n8") $ {   // nolint:PERF030,LINT019 — fresh empty target, single move; fires only on the das_metal arm
                var sc = make_run_state(tr.config, KVDtype.f16, KVDtype.f16)
                var idsc <- generate(tr, sc, count_prompt, length(count_prompt) + 8)
                delete sc
                return <- idsc
            }
            // forced-feed freeform decode parity: CPU-prefill BOTH states on the planar model
            // (blob CPU prefill panics), then force each cached continuation token through both
            // decoders — under the override the planar model quietly CPU-serves (planar decline)
            let vocab = tr.config.vocab_size
            var sa = make_run_state(tr.config, KVDtype.f16, KVDtype.f16)
            var sb = make_run_state(trg.config, KVDtype.f16, KVDtype.f16)
            eval_(tr, sa, prompt)
            eval_(tr, sb, prompt)
            let df0 = metal_decode_stats().decodes
            select_decode_override("metal")
            for (q in range64(long_length(prompt) - 1l, long_length(cpu_ids))) {
                gpu_picks |> push(greedy_top(sb.logits, vocab))
                for (a, b in sa.logits, sb.logits) {
                    fmaxd = max(fmaxd, abs(a - b))
                }
                var one <- [cpu_ids[q]]
                eval_(tr, sa, one)
                eval_(trg, sb, one)
                delete one
            }
            for (a, b in sa.logits, sb.logits) {
                fmaxd = max(fmaxd, abs(a - b))
            }
            served_free = metal_decode_stats().decodes - df0
            delete sa
            delete sb
            // the decode override stays selected — the both-paths cell below decodes on the twin
            let pf0 = metal_prefill_stats().prefills
            select_prefill_override("metal")
            set_metal_prefill_min_npos(1l)
            var s3 = make_run_state(trg.config, KVDtype.f16, KVDtype.f16)
            let ids3 <- generate(trg, s3, count_prompt, length(count_prompt) + 8)
            both_ids := ids3
            delete s3
            set_metal_prefill_min_npos(64l)
            select_prefill_override("")
            select_decode_override("")
            served_pf = metal_prefill_stats().prefills - pf0
            // B=2 batched-decode parity (counting margins — see the header note): CPU truth
            // then the batched metal override, 4 steps per stream over CPU-prefilled state
            cpu_bat <- cached_ids(path, "b2pair_c1c7_n4") $ {   // nolint:PERF030,LINT019 — fresh empty target, single move; fires only on the das_metal arm
                return <- batch_pair_steps(tr, tr, count_prompt, count_prompt2, 4)
            }
            let ba0 = metal_batch_decode_stats().steps
            select_decode_override("metal")
            gpu_bat <- batch_pair_steps(tr, trg, count_prompt, count_prompt2, 4)   // nolint:PERF030,LINT019 — fresh empty target, single move; fires only on the das_metal arm
            select_decode_override("")
            served_bat = metal_batch_decode_stats().steps - ba0
        }
        to_log(LOG_INFO, "gemma4-12b/k4 freeform CPU text (prompt echo + cont): {eyeball_text(tr, cpu_ids)}\n")
        to_log(LOG_INFO, "gemma4-12b/k4 freeform GPU greedy-would-be cont (fmaxd {fmaxd}): {eyeball_text(tr, gpu_picks)}\n")
        t |> success(fmaxd < 8.0,
            "gemma4-12b/k4: forced-feed freeform decode logits within 8 of CPU over the 128-token stream (measured {fmaxd})")
        t |> success(served_free > 0l, "gemma4-12b/k4: the forced-feed freeform decode ENGAGED ({served_free} served)")
        t |> success(served_pf > 0l, "gemma4-12b/k4: the hetero prefill ENGAGED ({served_pf} served)")
        log_gen_texts(tr, "gemma4-12b/k4 both-paths", cpu_cnt_ids, both_ids)
        t |> success(same(cpu_cnt_ids, both_ids),
            "gemma4-12b/k4: metal prefill+decode generate matches the CPU truth token-for-token")
        t |> success(served_bat > 0l, "gemma4-12b/k4: the hetero batch ENGAGED ({served_bat} served)")
        log_pair_texts(tr, "gemma4-12b/k4 B=2", cpu_bat, gpu_bat)
        t |> success(same(cpu_bat, gpu_bat),
            "gemma4-12b/k4: B=2 batched decode matches the CPU truth token-for-token")
        cont_logits_cell(t, tr, trg, "gemma4-12b/k4", 8.0)
        delete cpu_ids
        delete gpu_picks
        delete cpu_cnt_ids
        delete both_ids
        delete cpu_bat
        delete gpu_bat
        delete tr
        delete trg
    }
}

// The family axis (family waves A): each non-llama dense family loads its small carrier and
// asserts per-path ENGAGE/DECLINE with the exact need_* reason — the descriptor refactor's
// living expectation table. mistral3 (empty config = the llama shape) has no <= 35B fixture;
// its coverage is the derivation pins on the families above plus the llama cells.
[test]
def test_metal_family_matrix(t : T?) {
    t |> run("Metal family matrix: per-family needs derivation + path cells") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            if (!jit_enabled()) {
                t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
                return
            }
            var dev = metal_create_system_default_device()
            if (dev == null) {
                feint("no Metal device; family matrix skipped\n")
                return
            }
            metal_release(dev)
            let prior_wscale = get_wscale_f16()
            let prior_backend = active_kernel_backend()
            pin_kernel_backend("portable")
            set_wscale_f16(false)
            // qwen3 = NEOX + QK-norm + qd != dim (0.6B: dim 1024, qd 2048) — ENGAGE since the
            // wave A norm prepass; the tq4 row proves the tq4 rope-store's NEOX partner (pow2 hs)
            family_cells(t, path_join(models_dir(), "Qwen3-0.6B-Q8_0.gguf"), "qwen3",
                "qwen3-0.6b/q8/f16", uint(MetalNeed.neox | MetalNeed.qk_norm | MetalNeed.qd_neq_dim),
                "", KVDtype.tq4, 4.0)
            // the wave's showcase model: k4/k6 mixed sites x qk_norm x qd != dim at 4B scale
            family_cells(t, path_join(models_dir(), "Qwen3-4B-Instruct-2507-Q4_K_M.gguf"), "qwen3",
                "qwen3-4b/k4/f16", uint(MetalNeed.neox | MetalNeed.qk_norm | MetalNeed.qd_neq_dim),
                "")
            // qwen2 = NEOX + QKV bias — ENGAGE since the wave A bias epilogue; two carriers
            // cover the hs-64 and hs-128 bias geometries (the 0.5B also proves q8_0-KV bias)
            family_cells(t, path_join(models_dir(), "Qwen2.5-0.5B-Instruct-Q8_0.gguf"), "qwen2",
                "qwen2.5-0.5b/q8/f16", uint(MetalNeed.neox | MetalNeed.qkv_bias),
                "", KVDtype.q8_0)
            family_cells(t, path_join(models_dir(), "Qwen2.5-1.5B-Instruct-Q8_0.gguf"), "qwen2",
                "qwen2.5-1.5b/q8/f16", uint(MetalNeed.neox | MetalNeed.qkv_bias),
                "")
            set_wscale_f16(true)   // the s16 row drives the fused qkv_rs NEOX+bias epilogue
            family_cells(t, path_join(models_dir(), "Qwen2.5-0.5B-Instruct-Q8_0.gguf"), "qwen2",
                "qwen2.5-0.5b/q8/s16", uint(MetalNeed.neox | MetalNeed.qkv_bias),
                "")
            set_wscale_f16(false)
            // phi3 = NEOX only — ENGAGE since the wave A rope twins; the Q4 row also proves the
            // q8_0-KV rope-store's NEOX block loop, the Q8/s16 row the fused qkv_rs NEOX mapping
            family_cells(t, path_join(models_dir(), "Phi-3.5-mini-instruct-Q4_K_M.gguf"), "phi3",
                "phi3.5-mini/k4/f16", uint(MetalNeed.neox),
                "", KVDtype.q8_0)
            set_wscale_f16(true)
            family_cells(t, path_join(models_dir(), "Phi-3.5-mini-instruct-Q8_0.gguf"), "phi3",
                "phi3.5-mini/q8/s16", uint(MetalNeed.neox),
                "")
            set_wscale_f16(false)
            // gemma2 = NEOX + attn/final softcaps + sliding (even layers) + 4-norms + GeGLU +
            // hs 256 (the wave B widening) — ENGAGE on all three paths. The Q4_K_M row proves the
            // K-quant sites x wide heads x q8_0-KV block attention; the Q8/s16 row proves the s16
            // GEMVs with the swiglu-fused w13 form standing down to the geglu ew
            family_cells(t, path_join(models_dir(), "gemma-2-2b-it-Q4_K_M.gguf"), "gemma2",
                "gemma2-2b/k4/f16", uint(MetalNeed.neox | MetalNeed.softcap | MetalNeed.sliding |
                    MetalNeed.pre_post_norm | MetalNeed.geglu),
                "", KVDtype.q8_0, 4.0)
            set_wscale_f16(true)
            family_cells(t, path_join(models_dir(), "gemma-2-2b-it-Q8_0.gguf"), "gemma2",
                "gemma2-2b/q8/s16", uint(MetalNeed.neox | MetalNeed.softcap | MetalNeed.sliding |
                    MetalNeed.pre_post_norm | MetalNeed.geglu),
                "")
            set_wscale_f16(false)
            // the sliding-window MASKING row: gemma2's stock window (4096) never triggers at test
            // depths, so shrink it post-load to 160 (>= 128, the gate's single-form floor) and
            // cross it — prefill masks the below-window prefix, decode's chunked attention skips
            // whole below-window chunks (chlo > 0 by the depth assert). CPU truth slides the same
            // window, so generate parity proves the masking math end-to-end.
            gemma2_window_row(t, path_join(models_dir(), "gemma-2-2b-it-Q4_K_M.gguf"))
            // gemma3 = gemma2's shape minus the softcaps, plus QK-norm and the 5:1 SWA pattern
            // with DUAL-THETA per-layer rope (sliding θ=10k vs global θ=1M) — the stage-2 swa
            // table pair; qd != dim on both carriers. 5 of 6 layers slide, so ANY generate
            // proves the per-layer table pick (a wrong θ breaks parity immediately). The 1b's
            // q8_0-KV row drives the hs-256 QK-norm PREPASS, its f16 row the widened H-form.
            family_cells(t, path_join(models_dir(), "gemma-3-1b-it-Q8_0.gguf"), "gemma3",
                "gemma3-1b/q8/f16", uint(MetalNeed.neox | MetalNeed.qk_norm | MetalNeed.sliding |
                    MetalNeed.pre_post_norm | MetalNeed.geglu | MetalNeed.qd_neq_dim),
                "", KVDtype.q8_0, 4.0)
            family_cells(t, path_join(models_dir(), "gemma-3-4b-it-Q8_0.gguf"), "gemma3",
                "gemma3-4b/q8/f16", uint(MetalNeed.neox | MetalNeed.qk_norm | MetalNeed.sliding |
                    MetalNeed.pre_post_norm | MetalNeed.geglu | MetalNeed.qd_neq_dim),
                "")
            // gemma4-12B (stage 3): the heterogeneous line — two attention classes (sliding
            // 16q/8kv/256, global 16q/1kv/512 MQA), V-from-K on global layers, v_norm,
            // out_scale on the whole layer output, unit attn scale, final softcap + suppress.
            // All three paths serve (stage 3d: per-class batch uniforms + the prefix `lay`).
            // 7.4GB > the 6GiB tier — runs under DASLLAMA_PARITY_FULL=1 (like the 8B rows).
            gemma4_hetero_row(t, path_join(models_dir(), "gemma-4-12B-it-Q4_K_M.gguf"))
            // the q8 twin drives the hetero fixed-B mul_mv rail (a class the k4 row never
            // dispatches); 12.5GB — DASLLAMA_PARITY_FULL-gated like the row above
            gemma4_hetero_q8_row(t, path_join(models_dir(), "gemma-4-12B-it-Q8_0.gguf"))
            // qwen3moe (Wave C): qwen3 attention + GPU MoE routing — router + top-k select +
            // expert-indexed GEMVs (decode/batch) + gathered mul_mm prefill (k4 gate/up, k6
            // down). All three paths ENGAGE; 18.5GB > the 6GiB tier — DASLLAMA_PARITY_FULL-gated.
            family_cells(t, path_join(models_dir(), "Qwen3-30B-A3B-Instruct-2507-Q4_K_M.gguf"), "qwen3moe",
                "qwen3moe-30b/k4/f16", uint(MetalNeed.neox | MetalNeed.qk_norm | MetalNeed.qd_neq_dim),
                "", KVDtype.f16, 6.0)
            // gemma4-26B-A4B (Wave C stage 3): the parallel dense-shared + routed graph —
            // ENGAGE cells + shallow logits tolerances (see the row's ruling comment)
            gemma4moe_row(t, path_join(models_dir(), "gemma-4-26B-A4B-it-Q8_0.gguf"), "gemma4moe-26b")
            // ... and the community Q4_K_M file: k4 gate_up + NATIVE q51 down expert stacks —
            // the q51 Metal GEMV/mul_mm twins' model-level pin (15.9GB, PARITY_FULL-gated).
            // Wider step bar: the k4/q51 f16-tile paths measure 13.8 under the same double-router
            // amplifier (post-prefill stays 2.5-class); 24 keeps the ~2x margin discipline
            gemma4moe_row(t, path_join(models_dir(), "gemma-4-26B-A4B-it-Q4_K_M.gguf"), "gemma4moe-26b-k", 8.0, 24.0)
            // gemma-4 E-series (E4B): PLE (MetalNeed.ple) + cross-layer KV sharing — decode +
            // prefill serve, batch declines need_ple. 7.2GB — DASLLAMA_PARITY_FULL-gated.
            // (E2B stays on the `layers` decline: per-layer FFN width, no per-layer u_hidden yet.)
            gemma4e_row(t, path_join(models_dir(), "gemma-4-E4B-it-Q8_0.gguf"))
            // gpt-oss-20b (Wave C stage 4): mx4 experts + sinks + softmax_weight + biases
            gptoss_row(t, path_join(models_dir(), "gpt-oss-20b-mxfp4.gguf"))
            // qwen35 (Wave D): the Gated-DeltaNet hybrid — 3-in-4 recurrent layers (state
            // mirror, fused sequential scan) + gated attention (2x-wide Q, partial rope 64/256,
            // σ out-gate). Decode + prefill serve; batch = per-row fallback (see qwen35_row).
            qwen35_row(t, path_join(models_dir(), "Qwen3.5-0.8B-Q8_0.gguf"), "qwen35",
                "qwen35-0.8b/q8/f16", uint(MetalNeed.neox | MetalNeed.qk_norm | MetalNeed.qd_neq_dim |
                    MetalNeed.partial_rope | MetalNeed.q_gated), 4.0)
            // qwen35moe (Wave D stage 3): deltanet + grouped MoE + the sigmoid-gated shared
            // expert + f32-on-disk β/α — the whole-surface hybrid MoE. 22GB > the 6GiB tier —
            // DASLLAMA_PARITY_FULL-gated.
            qwen35_row(t, path_join(models_dir(), "Qwen3.6-35B-A3B-UD-Q4_K_M.gguf"), "qwen35moe",
                "qwen35moe-35b/k4/f16", uint(MetalNeed.neox | MetalNeed.qk_norm | MetalNeed.qd_neq_dim |
                    MetalNeed.partial_rope | MetalNeed.q_gated), 6.0)
            // qwen2moe (Wave D ride-along): softmax-no-renorm select + sigmoid shexp on std
            // attention; batch declines by design. 15GB — DASLLAMA_PARITY_FULL-gated.
            qwen2moe_row(t, path_join(models_dir(), "Qwen1.5-MoE-A2.7B-Chat.Q8_0.gguf"))
            // the Metal-4 tensor twins, forced into service on small carriers (--arm tensor):
            // q8 drives the mulmm/attn twins, the K-quant row drives the kq mulmm twins
            tensor_twin_row(t, path_join(models_dir(), "Llama-3.2-1B-Instruct-Q8_0.gguf"), "tensor-twins/q8")
            tensor_twin_row(t, path_join(models_dir(), "Llama-3.2-1B-Instruct-Q5_K_M.gguf"), "tensor-twins/k5")
            metal_decode_shutdown()
            metal_prefill_shutdown()
            t |> equal(metal_log_leaks_if_any(), 0l)
            clear_kernel_backend_pin()
            select_kernel_backend(prior_backend)
            set_wscale_f16(prior_wscale)
        } else {
            feint("das_metal is not built on this platform; nothing to assert\n")
        }
    }
}
