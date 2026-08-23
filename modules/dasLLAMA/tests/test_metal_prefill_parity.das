options gen2
options persistent_heap   // + explicit deletes: the model frees before the process exits
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama   // the public facade — pulls the engine + arch registrations + (on Apple) the metal prefill override
require dasllama/dasllama_math   // set_wscale_f16 — the resident path reads the f32 scale plane
require dasllama/dasllama_layout   // convert_model_to_metal_blob — the blob-flavor transform
require ?das_metal dasllama/dasllama_metal_prefill  // min-npos setter + stats + the shutdown/leak rail
require ?das_metal dasllama/dasllama_metal_decode  // decode shutdown (the kq/quantized-KV arms touch the decode driver)
require ?das_metal dasllama/dasllama_metal_common   // g_dev + residency_* — the pin witness probes the API directly
require ?das_metal das_metal   // metal_new_residency_set/metal_release — the availability probe
require dasllama/dasllama_env   // nolint:STYLE030,LINT019 — g_env_metal.residency, read only inside the das_metal static_if half
require ?das_metal metal/das_metal_boost  // metal_live_object_count for the leak gate
require daslib/jobque_boost
require daslib/fio   // nolint:STYLE030,LINT019 — used only inside the Apple static_if half
require strings
require math
require _model_tier

// End-to-end gate for the dasMetal whole-prefill override (Phase 6): greedy continuation of a
// confident counting prompt, GPU-resident prefill vs the CPU control ON THE SAME BINARY — decode
// is the identical CPU path in both runs, so any prefill-induced logit shift beyond the shared
// tolerance envelope surfaces as a token flip. The counting prompt is the kernel-robust fixture
// family test_parity.das established (high-entropy prompts flip on genuine near-ties, which are
// not bugs). Skips cleanly without the model, off-Apple, or with no Metal device.

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

// "1, 2, …, 120," — long enough to engage the resident path with margin (~300 llama-3 BPE tokens)
def private counting_prompt() : string {
    return build_string() $(var w) {
        for (i in range(1, 121)) {
            w |> write(i)
            w |> write(i < 120 ? ", " : ",")
        }
    }
}

// chunked prompt-to-continuation: eval chunk 1, eval chunk 2 (start_pos > 0 on the second),
// then n greedy steps — the M2.5 continuation arm's control/GPU shape (probe-proven)
def private chunked_greedy(trp, trd : Model; c1, c2 : array<int64>; kdt : KVDtype; nsteps : int) : array<int64> {
    var ids : array<int64>
    with_job_que() {
        setup_dasllama_jobque()
        var s = make_run_state(trp.config, kdt, kdt)
        eval_(trp, s, c1)
        eval_(trp, s, c2)
        let vocab = trp.config.vocab_size
        var cur = 0l
        for (v in range64(1l, vocab)) {
            if (s.logits[v] > s.logits[cur]) {
                cur = v
            }
        }
        for (_i in range(nsteps)) {
            ids |> push(cur)
            var one <- [cur]
            eval_(trd, s, one)
            delete one
            cur = 0l
            for (v in range64(1l, vocab)) {
                if (s.logits[v] > s.logits[cur]) {
                    cur = v
                }
            }
        }
        delete s
    }
    return <- ids
}

// the media turn's eval shape: a causal head chunk, then a NON-CAUSAL embd span at
// start_pos > 0 (rows = the tail tokens' own embedding rows, so the greedy continuation
// stays meaningful), then n greedy steps on trd — CPU control and GPU run share it verbatim
def private span_greedy(trp, trd : Model; head, tail : array<int64>; kdt : KVDtype; nsteps : int) : array<int64> {
    var ids : array<int64>
    with_job_que() {
        setup_dasllama_jobque()
        var s = make_run_state(trp.config, kdt, kdt)
        eval_(trp, s, head)
        let ntail = long_length(tail)
        var rows : array<float>
        rows |> reserve(ntail * trp.config.dim)
        rows |> resize(ntail * trp.config.dim)
        embed_text_rows(trp, tail, rows, 0l)
        eval_embd(trp, s, rows, ntail, true)
        let vocab = trp.config.vocab_size
        var cur = 0l
        for (v in range64(1l, vocab)) {
            if (s.logits[v] > s.logits[cur]) {
                cur = v
            }
        }
        for (_i in range(nsteps)) {
            ids |> push(cur)
            var one <- [cur]
            eval_(trd, s, one)
            delete one
            cur = 0l
            for (v in range64(1l, vocab)) {
                if (s.logits[v] > s.logits[cur]) {
                    cur = v
                }
            }
        }
        delete rows
        delete s
    }
    return <- ids
}

// the FUSED media turn: causal head + non-causal media rows + causal tail in ONE
// eval_embd_span_ call (the per-query mask — AttnArgs.ulo on the GPU leg), then n greedy
// steps on trd — CPU control and GPU run share it verbatim
def private span_fused_greedy(trp, trd : Model; head, media, tail : array<int64>; kdt : KVDtype; nsteps : int) : array<int64> {
    var ids : array<int64>
    with_job_que() {
        setup_dasllama_jobque()
        var s = make_run_state(trp.config, kdt, kdt)
        let nh = long_length(head)
        let nm = long_length(media)
        let npos = nh + nm + long_length(tail)
        var rows : array<float>
        rows |> reserve(npos * trp.config.dim)
        rows |> resize(npos * trp.config.dim)
        embed_text_rows(trp, head, rows, 0l)
        embed_text_rows(trp, media, rows, nh)
        embed_text_rows(trp, tail, rows, nh + nm)
        eval_embd_span_(trp, s, rows, npos, nh, nh + nm)
        let vocab = trp.config.vocab_size
        var cur = 0l
        for (v in range64(1l, vocab)) {
            if (s.logits[v] > s.logits[cur]) {
                cur = v
            }
        }
        for (_i in range(nsteps)) {
            ids |> push(cur)
            var one <- [cur]
            eval_(trd, s, one)
            delete one
            cur = 0l
            for (v in range64(1l, vocab)) {
                if (s.logits[v] > s.logits[cur]) {
                    cur = v
                }
            }
        }
        delete rows
        delete s
    }
    return <- ids
}

// the fused turn's prefill logits alone (no decode) — the fused-vs-splice witness reads these
// on ONE backend, where the per-row math is identical and any mask defect is the whole diff
def private span_fused_prefill_logits(trp : Model; head, media, tail : array<int64>; kdt : KVDtype) : array<float> {
    var lg : array<float>
    with_job_que() {
        setup_dasllama_jobque()
        var s = make_run_state(trp.config, kdt, kdt)
        let nh = long_length(head)
        let nm = long_length(media)
        let npos = nh + nm + long_length(tail)
        var rows : array<float>
        rows |> reserve(npos * trp.config.dim)
        rows |> resize(npos * trp.config.dim)
        embed_text_rows(trp, head, rows, 0l)
        embed_text_rows(trp, media, rows, nh)
        embed_text_rows(trp, tail, rows, nh + nm)
        eval_embd_span_(trp, s, rows, npos, nh, nh + nm)
        lg |> resize(long_length(s.logits))
        for (i in range64(long_length(s.logits))) {
            lg[i] = s.logits[i]
        }
        delete rows
        delete s
    }
    return <- lg
}

// span_fused_greedy's mrope twin: the media rows rope as a grid through
// eval_embd_span_mrope_ (the per-row table map), then n greedy steps on trd — the CPU
// control and the GPU run share it verbatim, delta'd decode included
def private span_mrope_greedy(trp, trd : Model; head, media, tail : array<int64>; grid : int2;
                              kdt : KVDtype; nsteps : int) : array<int64> {
    var ids : array<int64>
    with_job_que() {
        setup_dasllama_jobque()
        var s = make_run_state(trp.config, kdt, kdt)
        let nh = long_length(head)
        let nm = long_length(media)
        let npos = nh + nm + long_length(tail)
        var rows : array<float>
        rows |> reserve(npos * trp.config.dim)
        rows |> resize(npos * trp.config.dim)
        embed_text_rows(trp, head, rows, 0l)
        embed_text_rows(trp, media, rows, nh)
        embed_text_rows(trp, tail, rows, nh + nm)
        eval_embd_span_mrope_(trp, s, rows, npos, nh, nh + nm, int64(grid.x), int64(grid.y))
        let vocab = trp.config.vocab_size
        var cur = 0l
        for (v in range64(1l, vocab)) {
            if (s.logits[v] > s.logits[cur]) {
                cur = v
            }
        }
        for (_i in range(nsteps)) {
            ids |> push(cur)
            var one <- [cur]
            eval_(trd, s, one)
            delete one
            cur = 0l
            for (v in range64(1l, vocab)) {
                if (s.logits[v] > s.logits[cur]) {
                    cur = v
                }
            }
        }
        delete rows
        delete s
    }
    return <- ids
}

// span_fused_prefill_logits' mrope twin — the grid-vs-sequential witness reads these on the
// SAME backend, where the table upload is the only difference
def private span_mrope_prefill_logits(trp : Model; head, media, tail : array<int64>; grid : int2;
                                      kdt : KVDtype) : array<float> {
    var lg : array<float>
    with_job_que() {
        setup_dasllama_jobque()
        var s = make_run_state(trp.config, kdt, kdt)
        let nh = long_length(head)
        let nm = long_length(media)
        let npos = nh + nm + long_length(tail)
        var rows : array<float>
        rows |> reserve(npos * trp.config.dim)
        rows |> resize(npos * trp.config.dim)
        embed_text_rows(trp, head, rows, 0l)
        embed_text_rows(trp, media, rows, nh)
        embed_text_rows(trp, tail, rows, nh + nm)
        eval_embd_span_mrope_(trp, s, rows, npos, nh, nh + nm, int64(grid.x), int64(grid.y))
        lg |> resize(long_length(s.logits))
        for (i in range64(long_length(s.logits))) {
            lg[i] = s.logits[i]
        }
        delete rows
        delete s
    }
    return <- lg
}

// the DEEPSTACK turn: wide (1+n_ds)·dim rows — slice 0 = the tokens' own embedding rows,
// tails = small deterministic pseudo-random values (nonzero, so a skipped per-layer add
// diverges) — one eval_embd_span_ call, then n greedy steps on trd; CPU control and GPU run
// share it verbatim
def private span_ds_greedy(trp, trd : Model; head, media, tail : array<int64>; kdt : KVDtype; nsteps : int) : array<int64> {
    var ids : array<int64>
    with_job_que() {
        setup_dasllama_jobque()
        var s = make_run_state(trp.config, kdt, kdt)
        let dim = trp.config.dim
        let nds = trp.config.n_deepstack
        let rw = (1l + nds) * dim
        let nh = long_length(head)
        let nm = long_length(media)
        let npos = nh + nm + long_length(tail)
        var rows : array<float>
        rows |> reserve(npos * rw)
        rows |> resize(npos * rw)
        embed_text_rows_strided(trp, head, rows, 0l, rw)
        embed_text_rows_strided(trp, media, rows, nh, rw)
        embed_text_rows_strided(trp, tail, rows, nh + nm, rw)
        var state = 0xBEEFu
        for (r in range64(npos)) {
            for (i in range64(nds * dim)) {
                state = state * 1664525u + 1013904223u
                rows[r * rw + dim + i] = (float(state >> 8u) / float(1 << 24) - 0.5) * 0.05
            }
        }
        eval_embd_span_(trp, s, rows, npos, nh, nh + nm)
        let vocab = trp.config.vocab_size
        var cur = 0l
        for (v in range64(1l, vocab)) {
            if (s.logits[v] > s.logits[cur]) {
                cur = v
            }
        }
        for (_i in range(nsteps)) {
            ids |> push(cur)
            var one <- [cur]
            eval_(trd, s, one)
            delete one
            cur = 0l
            for (v in range64(1l, vocab)) {
                if (s.logits[v] > s.logits[cur]) {
                    cur = v
                }
            }
        }
        delete rows
        delete s
    }
    return <- ids
}

// the deepstack turn's prefill logits alone: same assembly as span_ds_greedy, tails present
// or zeroed — the witness reads the DIFFERENCE of the two on one backend (the add's own
// contribution), where backend noise cancels
def private span_ds_prefill_logits(trp : Model; head, media, tail : array<int64>; kdt : KVDtype;
                                   with_tails : bool) : array<float> {
    var lg : array<float>
    with_job_que() {
        setup_dasllama_jobque()
        var s = make_run_state(trp.config, kdt, kdt)
        let dim = trp.config.dim
        let nds = trp.config.n_deepstack
        let rw = (1l + nds) * dim
        let nh = long_length(head)
        let nm = long_length(media)
        let npos = nh + nm + long_length(tail)
        var rows : array<float>
        rows |> reserve(npos * rw)
        rows |> resize(npos * rw)
        embed_text_rows_strided(trp, head, rows, 0l, rw)
        embed_text_rows_strided(trp, media, rows, nh, rw)
        embed_text_rows_strided(trp, tail, rows, nh + nm, rw)
        if (with_tails) {
            var state = 0xBEEFu
            for (r in range64(npos)) {
                for (i in range64(nds * dim)) {
                    state = state * 1664525u + 1013904223u
                    rows[r * rw + dim + i] = (float(state >> 8u) / float(1 << 24) - 0.5) * 0.05
                }
            }
        }
        eval_embd_span_(trp, s, rows, npos, nh, nh + nm)
        lg |> resize(long_length(s.logits))
        for (i in range64(long_length(s.logits))) {
            lg[i] = s.logits[i]
        }
        delete rows
        delete s
    }
    return <- lg
}

// prompt-to-continuation: prefill on trp (the arm's subject — GPU when the override is
// selected and trp is the blob twin), then n_gen greedy decode steps on trd (the planar
// model = the shared CPU decode both runs ride, preserving the prefill-isolation property).
def private continuation(trp, trd : Model; prompt : array<int64>; n_gen : int) : array<int64> {
    var got : array<int64>
    with_job_que() {
        setup_dasllama_jobque()
        var s = make_run_state(trp.config)
        eval_(trp, s, prompt)
        let vocab = trp.config.vocab_size
        var cur = 0l
        for (v in range64(1l, vocab)) {
            if (s.logits[v] > s.logits[cur]) {
                cur = v
            }
        }
        for (_i in range(n_gen)) {
            got |> push(cur)
            var one <- [cur]
            eval_(trd, s, one)
            delete one
            cur = 0l
            for (v in range64(1l, vocab)) {
                if (s.logits[v] > s.logits[cur]) {
                    cur = v
                }
            }
        }
        delete s
    }
    return <- got
}

// the blob twin of a planar fixture (see test_metal_decode_parity's twin): an OWNED engine
// load transformed to the GPU form — the drivers are blob-only
def private blob_twin(t : T?; path : string; seq_cap : int64) : Model {
    var tg <- load_model_(path, QuantMode.q8)
    t |> success(convert_model_to_metal_blob(tg), "metal-blob transform")
    tg.config.seq_len = min(tg.config.seq_len, seq_cap)
    return <- tg
}

[test]
def test_metal_prefill_parity(t : T?) {   // nolint:STYLE038 — flat arm list, one t |> run cell per arm
    t |> run("GPU-resident prefill == CPU control, 40-token greedy continuation (Llama-3.2-1B Q8)") <| @(t : T?) {
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
            let prior_wscale = get_wscale_f16()             // process-wide knobs — restored below
            let prior_backend = active_kernel_backend()     // (both exit paths run through the restore)
            set_wscale_f16(false)       // the resident path reads the f32 scale plane (declines on s16)
            pin_kernel_backend("portable")  // ...and ROW-MAJOR weights — a repack backend's layout is unreadable (5c donor contract)
            var tr <- load_model_(path, QuantMode.q8)
            if (tr.config.seq_len > 2048l) {
                tr.config.seq_len = 2048l
            }
            let prompt <- encode(tr, counting_prompt(), true, false)
            var trg <- blob_twin(t, path, 2048l)
            if (arm_on(t, "base")) {
                // the legacy quantized-GEMM arm is GONE: the drivers are blob-only mul_mm
                // (a blob model carries no planar scale planes for the quantized-X rail)
                let cpu <- continuation(tr, tr, prompt, 40)
                t |> equal(length(cpu), 40)
                t |> success(select_prefill_override("metal"), "override registered")
                set_metal_prefill_min_npos(1l)
                let before = metal_prefill_stats().prefills
                let gpu <- continuation(trg, tr, prompt, 40)    // GPU prefill (blob), CPU decode (planar)
                let served = metal_prefill_stats().prefills - before
                select_prefill_override("")
                set_metal_prefill_min_npos(64l)
                clear_kernel_backend_pin()
                select_kernel_backend(prior_backend)
                set_wscale_f16(prior_wscale)
                if (served == 0l) {
                    metal_prefill_shutdown()
                    delete tr
                    delete trg
                    feint("metal prefill declined (no Metal device?); GPU parity skipped\n")
                    return
                }
                t |> success(same(cpu, gpu), "40-token continuation matches the CPU control (blob mul_mm, f32 activations)")
                // the residency witness: a served prefill under the default knob pins the working
                // set. Availability probes the API DIRECTLY, so a dead machinery cannot hide
                // behind the macOS-<15 skip on a box whose OS serves the API.
                var api_probe = metal_new_residency_set(dasllama_metal_common::g_dev)
                if (api_probe != null) {
                    metal_release(api_probe)
                }
                if (g_env_metal.residency) {
                    if (api_probe != null) {
                        t |> success(residency_active(), "the residency set is live on an API-serving OS")
                        t |> success(residency_pinned() > 0l, "the served prefill pinned {residency_pinned()} buffers in the residency set")
                        if (g_env_metal.heartbeat_s > 0) {
                            t |> success(metal_residency_heartbeat_live(), "the residency heartbeat is armed after a served step")
                        } else {
                            t |> success(!metal_residency_heartbeat_live(), "DASLLAMA_METAL_HEARTBEAT_S=0 leaves the heartbeat unarmed")
                        }
                    } else {
                        t |> skip("MTLResidencySet unavailable (macOS < 15) - the pin witness has no API to observe")
                    }
                } else {
                    t |> equal(residency_pinned(), 0l, "DASLLAMA_METAL_RESIDENCY=0 pins nothing")
                }
            }

            // s16 arm (W2): the DEFAULT-load scale plane — blobs read raw f16 scales, the
            // classifier GEMV takes its halfword twin; a fresh load owns fresh weight regions.
            // The restores above un-pinned the backend — re-pin row-major (the donor contract)
            if (arm_on(t, "s16")) {
                set_wscale_f16(true)
                pin_kernel_backend("portable")
                var tr16 <- load_model_(path, QuantMode.q8)
                if (tr16.config.seq_len > 2048l) {
                    tr16.config.seq_len = 2048l
                }
                var tr16g <- blob_twin(t, path, 2048l)
                let cpu16 <- continuation(tr16, tr16, prompt, 40)
                t |> success(select_prefill_override("metal"), "override registered (s16)")
                set_metal_prefill_min_npos(1l)
                let before16 = metal_prefill_stats().prefills
                let gpu16 <- continuation(tr16g, tr16, prompt, 40)
                t |> success(metal_prefill_stats().prefills - before16 > 0l, "wscale16 prefill served on the GPU")
                select_prefill_override("")
                set_metal_prefill_min_npos(64l)
                clear_kernel_backend_pin()
                select_kernel_backend(prior_backend)
                set_wscale_f16(prior_wscale)
                t |> success(same(cpu16, gpu16), "40-token wscale16 continuation matches the CPU control (s16 mul_mm + s16 classifier)")
                delete tr16
                delete tr16g
            }

            // kq arm (M2.3): K-quant prefill — the kq mul_mm twins (mixed k4/k6 tensors per
            // layer) + the kq classifier GEMV. The CPU kq prefill is the control; the envelope
            // is the kq one (f32-x GPU vs Q8_K CPU activations), tokens exact on the fixture.
            let kpath = path_join(models_dir(), "Llama-3.2-1B-Instruct-Q4_K_M.gguf")
            if (!arm_on(t, "kq")) {
                // filtered — the skip is already registered
            } elif (stat(kpath).is_valid) {
                set_wscale_f16(false)
                pin_kernel_backend("portable")
                var trk <- load_model_(kpath, QuantMode.q8)
                if (trk.config.seq_len > 2048l) {
                    trk.config.seq_len = 2048l
                }
                var trkg <- blob_twin(t, kpath, 2048l)
                let cpuk <- continuation(trk, trk, prompt, 40)
                t |> success(select_prefill_override("metal"), "override registered (kq)")
                set_metal_prefill_min_npos(1l)
                let beforek = metal_prefill_stats().prefills
                let gpuk <- continuation(trkg, trk, prompt, 40)
                t |> success(metal_prefill_stats().prefills - beforek > 0l, "kq prefill served on the GPU")
                select_prefill_override("")
                set_metal_prefill_min_npos(64l)
                clear_kernel_backend_pin()
                select_kernel_backend(prior_backend)
                set_wscale_f16(prior_wscale)
                t |> success(same(cpuk, gpuk), "40-token kq continuation matches the CPU control (kq mul_mm + kq classifier)")
                delete trk
                delete trkg
            } else {
                feint("Llama-3.2-1B Q4_K_M not present (llama-quantize the Q8 source); kq prefill arm skipped\n")
            }

            // continuation arm (M2.5): a chunked prompt — chunk 1 is a fresh GPU prefill,
            // chunk 2 rides the NEW start_pos > 0 path (existing rows gathered into the K/V
            // panels, causal diagonal shifted by qoff). All three session codecs; the q8_0/tq4
            // gathers dequant the codec rows (tq4 un-rotates to the raw basis). Token-exact
            // vs the all-CPU chunked control.
            if (arm_on(t, "cont")) {
                set_wscale_f16(false)
                pin_kernel_backend("portable")
                var trc <- load_model_(path, QuantMode.q8)
                if (trc.config.seq_len > 2048l) {
                    trc.config.seq_len = 2048l
                }
                var trcg <- blob_twin(t, path, 2048l)
                let nfull = long_length(prompt)
                let cut = int(nfull * 3l / 5l)
                var ch1 <- [for (i in range(cut)); prompt[i]]
                var ch2 <- [for (i in range(cut, int(nfull))); prompt[i]]
                for (kdt in [KVDtype.f16, KVDtype.q8_0, KVDtype.tq4]) {
                    var cpu_c <- chunked_greedy(trc, trc, ch1, ch2, kdt, 24)
                    select_prefill_override("metal")
                    set_metal_prefill_min_npos(1l)
                    let pfc0 = metal_prefill_stats().prefills
                    var gpu_c <- chunked_greedy(trcg, trc, ch1, ch2, kdt, 24)
                    select_prefill_override("")
                    set_metal_prefill_min_npos(64l)
                    t |> success(metal_prefill_stats().prefills - pfc0 == 2l,
                        "{kdt} chunks served on the GPU (fresh + start_pos > 0 continuation)")
                    t |> success(same(cpu_c, gpu_c), "chunked {kdt} continuation matches the all-CPU control (M2.5)")
                    delete cpu_c
                    delete gpu_c
                }
                clear_kernel_backend_pin()
                select_kernel_backend(prior_backend)
                set_wscale_f16(prior_wscale)
                delete ch1
                delete ch2
                delete trc
                delete trcg
            }

            // span arm (metal-media): the media eval shape — causal head, non-causal embd span
            // at start_pos > 0, greedy decode. Both prefill calls must serve on the GPU (the
            // span through AttnArgs.uend), token-exact vs the all-CPU control, per KV codec.
            if (arm_on(t, "span")) {
                set_wscale_f16(false)
                pin_kernel_backend("portable")
                var trs <- load_model_(path, QuantMode.q8)
                if (trs.config.seq_len > 2048l) {
                    trs.config.seq_len = 2048l
                }
                var trsg <- blob_twin(t, path, 2048l)
                let nfull = long_length(prompt)
                let scut = int(nfull * 3l / 5l)
                var shead <- [for (i in range(scut)); prompt[i]]
                var stail <- [for (i in range(scut, int(nfull))); prompt[i]]
                for (kdt in [KVDtype.f16, KVDtype.q8_0, KVDtype.tq4]) {
                    var cpu_s <- span_greedy(trs, trs, shead, stail, kdt, 24)
                    select_prefill_override("metal")
                    set_metal_prefill_min_npos(1l)
                    let pfs0 = metal_prefill_stats().prefills
                    var gpu_s <- span_greedy(trsg, trs, shead, stail, kdt, 24)
                    select_prefill_override("")
                    set_metal_prefill_min_npos(64l)
                    t |> success(metal_prefill_stats().prefills - pfs0 == 2l,
                        "{kdt} head + span both served on the GPU")
                    log_gen_texts(trs, "span/{kdt}", cpu_s, gpu_s)
                    t |> success(same(cpu_s, gpu_s), "{kdt} span turn matches the all-CPU control")
                    delete cpu_s
                    delete gpu_s
                }
                clear_kernel_backend_pin()
                select_kernel_backend(prior_backend)
                set_wscale_f16(prior_wscale)
                delete shead
                delete stail
                delete trs
                delete trsg
            }

            // span-fused arm: the image turn's ONE-eval shape — causal head, non-causal media
            // rows, causal tail, all in a single prefill (the per-query mask through
            // AttnArgs.ulo). Exactly ONE GPU prefill must serve it, token-exact vs the all-CPU
            // control, per KV codec.
            if (arm_on(t, "span-fused")) {
                set_wscale_f16(false)
                pin_kernel_backend("portable")
                var trf <- load_model_(path, QuantMode.q8)
                if (trf.config.seq_len > 2048l) {
                    trf.config.seq_len = 2048l
                }
                var trfg <- blob_twin(t, path, 2048l)
                // the turn length pins npos % 64 == 32 — the ceil32 M-pad's tail block (the
                // shape the old ceil64 pad billed as a dead extra GEMM block) is exercised by
                // every fused eval below, and the residue is asserted, not assumed
                let nfull = long_length(prompt)
                var ntarget = (nfull / 64l) * 64l + 32l
                if (ntarget > nfull) {
                    ntarget -= 64l
                }
                t |> success(ntarget % 64l == 32l && ntarget >= 96l,
                    "fused-turn shape pins the ceil32 tail block (npos = {ntarget})")
                let fc1 = int(ntarget * 2l / 5l)
                let fc2 = int(ntarget * 4l / 5l)
                var fhead <- [for (i in range(fc1)); prompt[i]]
                var fmedia <- [for (i in range(fc1, fc2)); prompt[i]]
                var ftail <- [for (i in range(fc2, int(ntarget))); prompt[i]]
                // fused-vs-splice logits bars, per KV codec: the splice re-reads its head rows
                // from the QUANTIZED cache while the fused eval keeps its own rows in the f32
                // panels, so the codecs carry real quantization spread (measured clean: f16 = 0,
                // q8 0.073, tq4 0.708) — the bars sit above that and well under the poisoned-mask
                // magnitude (negative control: ~5.1 on every codec)
                var fbars <- { KVDtype.f16 => 1e-4, KVDtype.q8_0 => 0.5, KVDtype.tq4 => 2.0 }
                for (kdt in [KVDtype.f16, KVDtype.q8_0, KVDtype.tq4]) {
                    var cpu_f <- span_fused_greedy(trf, trf, fhead, fmedia, ftail, kdt, 24)
                    select_prefill_override("metal")
                    let pff0 = metal_prefill_stats().prefills
                    var gpu_f <- span_fused_greedy(trfg, trf, fhead, fmedia, ftail, kdt, 24)
                    t |> success(metal_prefill_stats().prefills - pff0 == 1l,
                        "{kdt} fused turn served as ONE GPU prefill")
                    // same-backend mask witness: fused vs the three-eval splice on the GPU run
                    // row-identical math, so the prefill logits must agree at the bit-noise
                    // floor — a mask defect on ANY row moves its KV and lands here
                    var lg_fused <- span_fused_prefill_logits(trfg, fhead, fmedia, ftail, kdt)
                    set_span_fuse(false)
                    set_metal_prefill_min_npos(1l)   // the splice's short head/tail ride the GPU too
                    let pfsp0 = metal_prefill_stats().prefills
                    var lg_splice <- span_fused_prefill_logits(trfg, fhead, fmedia, ftail, kdt)
                    // the reference must actually BE the splice: three GPU prefills, or the
                    // witness collapses onto its own subject (an always-fused mutant passes)
                    t |> success(metal_prefill_stats().prefills - pfsp0 == 3l,
                        "{kdt} splice leg served as THREE GPU prefills")
                    set_metal_prefill_min_npos(64l)
                    set_span_fuse(true)
                    select_prefill_override("")
                    var lmax = 0.0
                    var amax_f = 0l
                    var amax_s = 0l
                    for (i in range64(min(long_length(lg_fused), long_length(lg_splice)))) {
                        lmax = max(lmax, abs(lg_fused[i] - lg_splice[i]))
                        if (lg_fused[i] > lg_fused[amax_f]) {
                            amax_f = i
                        }
                        if (lg_splice[i] > lg_splice[amax_s]) {
                            amax_s = i
                        }
                    }
                    to_log(LOG_INFO, "span-fused/{kdt} GPU fused-vs-splice prefill-logits maxdiff = {lmax} (argmax fused {amax_f} @{lg_fused[amax_f]}, splice {amax_s} @{lg_splice[amax_s]})\n")
                    var pick_f <- [amax_f]
                    var pick_s <- [amax_s]
                    log_gen_texts(trf, "span-fused/{kdt} pick", pick_f, pick_s)
                    delete pick_f
                    delete pick_s
                    t |> success(lmax < (fbars?[kdt] ?? 0.0), "{kdt} fused == splice on the GPU ({lmax})")
                    log_gen_texts(trf, "span-fused/{kdt}", cpu_f, gpu_f)
                    t |> success(same(cpu_f, gpu_f), "{kdt} fused turn matches the all-CPU control")
                    delete cpu_f
                    delete gpu_f
                    delete lg_fused
                    delete lg_splice
                }
                clear_kernel_backend_pin()
                select_kernel_backend(prior_backend)
                set_wscale_f16(prior_wscale)
                delete fbars
                delete fhead
                delete fmedia
                delete ftail
                delete trf
                delete trfg
            }

            // span-mrope arm: the qwen image turn's grid-roped ONE-eval shape. The metal
            // override consumes the SAME per-row tables prefill_rope_tables builds from the
            // grid map, so the quantum serves as ONE GPU prefill (the mrope decline is gone),
            // token-exact vs the all-CPU control — and the grid must MOVE the GPU stream vs
            // the sequential span, or the map never reached the uploaded angles.
            if (arm_on(t, "span-mrope")) {
                set_wscale_f16(false)
                pin_kernel_backend("portable")
                var trm <- load_model_(path, QuantMode.q8)
                if (trm.config.seq_len > 2048l) {
                    trm.config.seq_len = 2048l
                }
                trm.config.rope_sections = int4(12, 10, 10, 0)   // stamp: the 1B is not an mrope arch (rotary 64 -> half 32)
                trm.config.mrope_interleaved = true
                var trmg <- blob_twin(t, path, 2048l)
                trmg.config.rope_sections = int4(12, 10, 10, 0)
                trmg.config.mrope_interleaved = true
                let mh = 20l
                let nm = 24l   // a 6x4 merged grid; position advance 6, delta -18
                let mgrid = int2(6, 4)
                var mhead <- [for (i in range64(mh)); prompt[i]]
                var mmedia <- [for (i in range64(mh, mh + nm)); prompt[i]]
                var mtail <- [for (i in range64(mh + nm, mh + nm + 20l)); prompt[i]]
                for (kdt in [KVDtype.f16, KVDtype.q8_0]) {
                    var cpu_m <- span_mrope_greedy(trm, trm, mhead, mmedia, mtail, mgrid, kdt, 24)
                    select_prefill_override("metal")
                    let pfm0 = metal_prefill_stats().prefills
                    var gpu_m <- span_mrope_greedy(trmg, trm, mhead, mmedia, mtail, mgrid, kdt, 24)
                    t |> success(metal_prefill_stats().prefills - pfm0 == 1l,
                        "{kdt} mrope turn served as ONE GPU prefill (no decline)")
                    // the map must reach the uploaded angles: the same GPU eval at sequential
                    // span positions moves the prefill logits (greedy tokens can tie on the
                    // counting fixture, so logits are the discriminator; E1's CPU shift ~0.14)
                    var lg_m <- span_mrope_prefill_logits(trmg, mhead, mmedia, mtail, mgrid, kdt)
                    var lg_s <- span_fused_prefill_logits(trmg, mhead, mmedia, mtail, kdt)
                    select_prefill_override("")
                    var gdiff = 0.0
                    for (i in range64(min(long_length(lg_m), long_length(lg_s)))) {
                        gdiff = max(gdiff, abs(lg_m[i] - lg_s[i]))
                    }
                    to_log(LOG_INFO, "span-mrope/{kdt} grid-vs-sequential GPU prefill-logits maxdiff = {gdiff}\n")
                    t |> success(gdiff > 1e-3, "{kdt} the grid moves the GPU prefill logits ({gdiff})")
                    log_gen_texts(trm, "span-mrope/{kdt}", cpu_m, gpu_m)
                    t |> success(same(cpu_m, gpu_m), "{kdt} mrope turn matches the all-CPU control")
                    delete cpu_m
                    delete gpu_m
                    delete lg_m
                    delete lg_s
                }
                clear_kernel_backend_pin()
                select_kernel_backend(prior_backend)
                set_wscale_f16(prior_wscale)
                delete mhead
                delete mmedia
                delete mtail
                delete trm
                delete trmg
            }

            // span-ds arm: the deepstack quantum on the GPU — the per-layer slice adds encode
            // via enc_add over uploaded slice-major planes. ONE GPU prefill (the decline is
            // gone), token-exact vs the all-CPU control; nonzero tails make a skipped or
            // misaligned add diverge immediately.
            if (arm_on(t, "span-ds")) {
                set_wscale_f16(false)
                pin_kernel_backend("portable")
                var trd_ <- load_model_(path, QuantMode.q8)
                if (trd_.config.seq_len > 2048l) {
                    trd_.config.seq_len = 2048l
                }
                trd_.config.n_deepstack = 3l   // stamp: the 1B is not a deepstack arch
                var trdg <- blob_twin(t, path, 2048l)
                trdg.config.n_deepstack = 3l
                let dh = 20l
                let dm = 24l
                var dhead <- [for (i in range64(dh)); prompt[i]]
                var dmedia <- [for (i in range64(dh, dh + dm)); prompt[i]]
                var dtail <- [for (i in range64(dh + dm, dh + dm + 20l)); prompt[i]]
                for (kdt in [KVDtype.f16, KVDtype.q8_0]) {
                    var cpu_d <- span_ds_greedy(trd_, trd_, dhead, dmedia, dtail, kdt, 24)
                    select_prefill_override("metal")
                    let pfd0 = metal_prefill_stats().prefills
                    var gpu_d <- span_ds_greedy(trdg, trd_, dhead, dmedia, dtail, kdt, 24)
                    t |> success(metal_prefill_stats().prefills - pfd0 == 1l,
                        "{kdt} deepstack turn served as ONE GPU prefill (no decline)")
                    log_gen_texts(trd_, "span-ds/{kdt}", cpu_d, gpu_d)
                    t |> success(same(cpu_d, gpu_d), "{kdt} deepstack turn matches the all-CPU control")
                    // the add's own contribution, per backend: lg(tails) − lg(zero-tails) on ONE
                    // backend cancels its noise (greedy tokens TIE on the counting fixture, so a
                    // skipped or misaligned GPU add needs this witness, not token equality)
                    var g_t <- span_ds_prefill_logits(trdg, dhead, dmedia, dtail, kdt, true)
                    var g_z <- span_ds_prefill_logits(trdg, dhead, dmedia, dtail, kdt, false)
                    select_prefill_override("")
                    var c_t <- span_ds_prefill_logits(trd_, dhead, dmedia, dtail, kdt, true)
                    var c_z <- span_ds_prefill_logits(trd_, dhead, dmedia, dtail, kdt, false)
                    var a_gpu = 0.0
                    var dd = 0.0
                    for (i in range64(min(long_length(g_t), long_length(c_t)))) {
                        let ag = g_t[i] - g_z[i]
                        let ac = c_t[i] - c_z[i]
                        a_gpu = max(a_gpu, abs(ag))
                        dd = max(dd, abs(ag - ac))
                    }
                    to_log(LOG_INFO, "span-ds/{kdt} GPU add-contribution max = {a_gpu}, GPU-vs-CPU contribution maxdiff = {dd}\n")
                    // clean dd measures 10-13% of a_gpu (the layer-0..2 add propagates 30+ layers,
                    // so cross-backend noise rides the Jacobian); a skipped add is a_gpu = 0 and a
                    // misaligned one lands dd at a_gpu's own scale - 0.5x separates both ways
                    t |> success(a_gpu > 0.01, "{kdt} the slice adds reach the GPU prefill logits ({a_gpu})")
                    t |> success(dd < 0.5 * a_gpu, "{kdt} the GPU add contribution matches the CPU's ({dd} vs {a_gpu})")
                    delete g_t
                    delete g_z
                    delete c_t
                    delete c_z
                    delete cpu_d
                    delete gpu_d
                }
                clear_kernel_backend_pin()
                select_kernel_backend(prior_backend)
                set_wscale_f16(prior_wscale)
                delete dhead
                delete dmedia
                delete dtail
                delete trd_
                delete trdg
            }

            // dim-4096 arm (M2.6): past the fused add+rms+quant slab (dim <= 3072) the prefill
            // driver runs the unfused dispatch set — this is that rail's first real-model
            // parity. The 8B is large-tier: it runs only under DASLLAMA_PARITY_FULL.
            if (arm_on(t, "dim")) {
                let path8 = path_join(models_dir(), "Meta-Llama-3.1-8B-Instruct-Q8_0.gguf")
                if (model_available(t, path8)) {
                    set_wscale_f16(false)
                    pin_kernel_backend("portable")
                    var tr8 <- load_model_(path8, QuantMode.q8)
                    if (tr8.config.seq_len > 2048l) {
                        tr8.config.seq_len = 2048l
                    }
                    var tr8g <- blob_twin(t, path8, 2048l)
                    let cpu8 <- continuation(tr8, tr8, prompt, 40)
                    t |> success(select_prefill_override("metal"), "override registered (dim4096)")
                    set_metal_prefill_min_npos(1l)
                    let before8 = metal_prefill_stats().prefills
                    let gpu8 <- continuation(tr8g, tr8, prompt, 40)
                    t |> success(metal_prefill_stats().prefills - before8 > 0l, "dim-4096 prefill served on the GPU")
                    select_prefill_override("")
                    set_metal_prefill_min_npos(64l)
                    clear_kernel_backend_pin()
                    select_kernel_backend(prior_backend)
                    set_wscale_f16(prior_wscale)
                    t |> success(same(cpu8, gpu8), "40-token dim-4096 continuation matches the CPU control (unfused prefill set)")
                    delete tr8
                    delete tr8g
                }
            }

            // GPU-prefill x quantized-KV arm: the prefill KV writeback into q8_0/tq4 SESSION
            // codecs — this combo engaged since W3 but its output was never explicitly verified
            // (the decode parity arms use CPU prefill). GPU prefill + GPU decode vs the all-CPU
            // control on the same codec, token-exact.
            if (arm_on(t, "qkv")) {
                set_wscale_f16(false)
                pin_kernel_backend("portable")
                var trq <- load_model_(path, QuantMode.q8)
                if (trq.config.seq_len > 2048l) {
                    trq.config.seq_len = 2048l
                }
                var trqg <- blob_twin(t, path, 2048l)
                for (kdt in [KVDtype.q8_0, KVDtype.tq4]) {
                    var cpuq : array<int64>
                    var gpuq : array<int64>
                    with_job_que() {
                        setup_dasllama_jobque()
                        var sa = make_run_state(trq.config, kdt, kdt)
                        let ids <- generate(trq, sa, prompt, length(prompt) + 24)
                        cpuq := ids
                        delete sa
                        select_prefill_override("metal")
                        select_decode_override("metal")
                        set_metal_prefill_min_npos(1l)
                        var sb = make_run_state(trq.config, kdt, kdt)
                        let ids2 <- generate(trqg, sb, prompt, length(prompt) + 24)
                        gpuq := ids2
                        delete sb
                        select_prefill_override("")
                        select_decode_override("")
                        set_metal_prefill_min_npos(64l)
                    }
                    t |> success(same(cpuq, gpuq), "GPU prefill x {kdt} KV: 24-token continuation matches the all-CPU control")
                    delete cpuq
                    delete gpuq
                }
                clear_kernel_backend_pin()
                select_kernel_backend(prior_backend)
                set_wscale_f16(prior_wscale)
                delete trq
                delete trqg
            }

            metal_prefill_shutdown()
            metal_decode_shutdown()   // the kq uploads + the quantized-KV arm's decode steps live in the decode driver's caches
            t |> equal(metal_log_leaks_if_any(), 0l)
            delete tr
            delete trg
        } else {
            feint("das_metal is not built on this platform; nothing to compare\n")
        }
    }
}
