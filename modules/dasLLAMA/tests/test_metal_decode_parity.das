options gen2
options persistent_heap   // + explicit deletes: the model frees before the process exits
options stack = 131072   // several by-value Sessions per arm in one frame (test_batch_decode sizing)

require dastest/testing_boost public
require dasllama/dasllama   // the public facade — engine + arch registrations + (on Apple) the metal overrides
require dasllama/dasllama_math   // set_wscale_f16 — the resident path reads the f32 scale plane
require dasllama/dasllama_kv_codec   // cvt_q8kv_to_f32/cvt_tq4kv_to_f32 — the cache-row dequant oracles
require dasllama/dasllama_layout   // convert_model_to_metal_blob — the blob-flavor transform
require ?das_metal dasllama/dasllama_metal_decode  // nolint:STYLE030 — stats/mirror/shutdown rails
require ?das_metal dasllama/dasllama_metal_prefill  // nolint:STYLE030 — the Metal-prefill→GPU-decode hybrid arm
require ?das_metal metal/das_metal_boost  // nolint:STYLE030 — metal_live_object_count for the leak gate
require llvm/daslib/f16_cvt  // nolint:STYLE030 — f16 KV row decode in the tolerance gate (Apple half only)
require daslib/jobque_boost
require daslib/fio  // nolint:STYLE030 — used only inside the Apple static_if half
require math
require strings
require _model_tier

// End-to-end gates for the dasMetal whole-decode override (emission arc P2): greedy continuation
// of a confident counting prompt, GPU-resident decode vs the CPU control ON THE SAME BINARY — the
// prefill is the identical CPU path in both runs (except the hybrid arm), so any decode-induced
// logit shift beyond the shared tolerance envelope surfaces as a token flip. Arms: both hybrid
// modes (CPU-prefill→GPU-decode, Metal-prefill→GPU-decode), the f16/f32 single-step logits + KV
// row tolerance gates (GPU decode is NOT bit-exact: f32-X GEMVs vs the CPU's q8 activations,
// online GPU softmax), the paged multi-run gather arm, session churn under the LRU cap, the
// rewind (n_past = 0 reuse) arm, and the leak gate. Skips cleanly without the model, off-Apple,
// or with no Metal device.

def private same(a, b : array<int64>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (i in range(length(a))) {
        if (a[i] != b[i]) {
            return false
        }
    }
    return true
}

// "start, start+1, …, last," — the kernel-robust confident fixture family (test_parity.das)
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

// n greedy steps from the session's current state (the prompt is already evaluated); the decode
// path is whatever override is active when called
def private greedy_steps(tr : Model; var s : Session; n : int) : array<int64> {
    let vocab = tr.config.vocab_size
    var ids : array<int64>
    ids |> reserve(n)
    var cur = greedy_top(s.logits, vocab)
    for (_i in range(n)) {
        ids |> push(cur)
        var one <- [cur]
        eval_(tr, s, one)
        delete one
        cur = greedy_top(s.logits, vocab)
    }
    return <- ids
}

// The blob twin of a planar fixture: an OWNED engine load (no image cache) transformed to the
// GPU form — the drivers are blob-only, and CPU inference on a blob model panics, so every
// CPU/GPU arm pair runs a planar model for CPU stages and its twin for GPU stages (configs are
// identical, sessions are geometry-bound — one session spans both).
def private blob_twin(t : T?; path : string; seq_cap : int64) : Model {
    var tg <- load_model_(path, QuantMode.q8)
    t |> success(convert_model_to_metal_blob(tg), "metal-blob transform")
    tg.config.seq_len = min(tg.config.seq_len, seq_cap)
    return <- tg
}

// prompt-to-continuation: prefill on trp, then n_gen greedy decode steps on trd (forward(), where
// the decode override hooks). The drivers are blob-only, so callers pass the planar model for CPU
// stages and the blob twin for GPU stages — sessions are geometry-bound, one session spans both.
def private continuation(trp, trd : Model; prompt : array<int64>; n_gen : int; kdt : KVDtype) : array<int64> {
    var got : array<int64>
    with_job_que() {
        setup_dasllama_jobque()
        var s = make_run_state(trp.config, kdt, kdt)
        eval_(trp, s, prompt)
        got <- greedy_steps(trd, s, n_gen)
        delete s
    }
    return <- got
}

// max |Δ| between two cache slabs' row `pos`, decoded per the (f16/f32/q8_0/tq4) codec
def private kv_row_max_diff_one(cache_a, cache_b : array<uint8>; base_a, base_b : int64;
                                dt : KVDtype; kv_dim, pos : int64) : float {
    var maxd = 0.0
    let rowb = kv_row_bytes(dt, kv_dim)
    unsafe {
        let pa = addr < uint8 const? >(cache_a[0]) + (base_a + pos * rowb)
        let pb = addr < uint8 const? >(cache_b[0]) + (base_b + pos * rowb)
        if (dt == KVDtype.f16) {
            var ha = reinterpret<uint16 const?>(pa)
            var hb = reinterpret<uint16 const?>(pb)
            for (e in range64(kv_dim)) {
                maxd = max(maxd, abs(f16_to_f32(uint(ha[e])) - f16_to_f32(uint(hb[e]))))
            }
        } elif (dt == KVDtype.q8_0 || dt == KVDtype.tq4) {
            // block codecs compare dequantized (tq4: in the rotated basis both rows share)
            var da : array<float>
            var db : array<float>
            da |> resize(int(kv_dim))
            db |> resize(int(kv_dim))
            if (dt == KVDtype.q8_0) {
                cvt_q8kv_to_f32(addr(da[0]), pa, kv_dim)
                cvt_q8kv_to_f32(addr(db[0]), pb, kv_dim)
            } else {
                cvt_tq4kv_to_f32(addr(da[0]), pa, kv_dim)
                cvt_tq4kv_to_f32(addr(db[0]), pb, kv_dim)
            }
            for (e in range64(kv_dim)) {
                maxd = max(maxd, abs(da[e] - db[e]))
            }
            delete da
            delete db
        } else {
            var fa = reinterpret<float const?>(pa)
            var fb = reinterpret<float const?>(pb)
            for (e in range64(kv_dim)) {
                maxd = max(maxd, abs(fa[e] - fb[e]))
            }
        }
    }
    return maxd
}

// max |Δ| between the two sessions' cache rows at position `pos` across all layers — the
// kv_blob_max_diff pattern (test_batch_decode) narrowed to one row
def private kv_row_max_diff(sa, sb : Session; c : Config; pos : int64) : float {
    var maxd = 0.0
    for (l in range64(c.n_layers)) {
        let kv_dim = layer_kv_dim(c, l)
        maxd = max(maxd, kv_row_max_diff_one(sa.key_cache, sb.key_cache,
            kv_base_k(sa, l, c.seq_len), kv_base_k(sb, l, c.seq_len), sa.kv_dtype_k, kv_dim, pos))
        maxd = max(maxd, kv_row_max_diff_one(sa.value_cache, sb.value_cache,
            kv_base_v(sa, l, c.seq_len), kv_base_v(sb, l, c.seq_len), sa.kv_dtype_v, kv_dim, pos))
    }
    return maxd
}

// one CPU step and one GPU step from identical prefilled states: logits + stored-KV-row envelope
[unused_argument(t, tr, trg, prompt, kdt, tol_logits, tol_kv, tag)]   // off-Apple the gated body is empty
def private single_step_gate(t : T?; tr, trg : Model; prompt : array<int64>; kdt : KVDtype;
                             tol_logits, tol_kv : float; tag : string) {
    // whole-body gate: the stats rails only exist with das_metal, and every caller is already
    // inside the Apple half — off-Apple this helper just needs to compile
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let c = tr.config
        let vocab = c.vocab_size
        var sa = make_run_state(c, kdt, kdt)
        var sb = make_run_state(c, kdt, kdt)
        var served = 0l
        with_job_que() {
            setup_dasllama_jobque()
            eval_(tr, sa, prompt)
            eval_(tr, sb, prompt)
            let tok = greedy_top(sa.logits, vocab)
            var one <- [tok]
            select_decode_override("")
            eval_(tr, sa, one)          // the CPU control step
            t |> success(select_decode_override("metal"), "{tag}: override registered")
            let before = metal_decode_stats().decodes
            eval_(trg, sb, one)         // the GPU step (the blob twin — the drivers are blob-only)
            served = metal_decode_stats().decodes - before
            select_decode_override("")
            delete one
        }
        if (served == 0l) {
            feint("{tag}: metal decode declined; single-step gate skipped\n")
        } else {
            var maxd = 0.0
            for (v in range64(vocab)) {
                maxd = max(maxd, abs(sa.logits[v] - sb.logits[v]))
            }
            t |> success(maxd <= tol_logits, "{tag}: one-step logits within {tol_logits} (max diff {maxd})")
            let kvd = kv_row_max_diff(sa, sb, c, sa.n_past - 1l)
            t |> success(kvd <= tol_kv, "{tag}: stored KV row within {tol_kv} (max diff {kvd})")
            // envelope visibility on green runs too — a diff near the gate means a flaky tolerance
            to_log(LOG_INFO, "metal decode {tag} single-step: logits maxd={maxd} (tol {tol_logits}), kv row maxd={kvd} (tol {tol_kv})\n")
        }
        delete sa
        delete sb
    }
}

// interleaved-chunk prefill of two paged sessions (their page groups interleave, so the second
// session's logical rows gather through MULTIPLE physical runs), then n greedy steps on it
def private paged_greedy(tr, trg : Model; prompt : array<int64>; page_rows : int64; n : int; gpu : bool) : array<int64> {
    var got : array<int64>
    with_job_que() {
        setup_dasllama_jobque()
        var pool <- create_kv_pool_(tr.config, page_rows, KVDtype.f16, KVDtype.f16)
        var sa <- make_run_state_paged(tr.config, pool)
        var sb <- make_run_state_paged(tr.config, pool)
        let np = long_length(prompt)
        var at = 0l
        while (at < np) {   // alternate page-sized chunks -> physically interleaved block tables
            let hi = min(at + page_rows, np)
            var chunk <- [for (i in range64(at, hi)); prompt[i]]
            eval_(tr, sa, chunk)
            eval_(tr, sb, chunk)
            delete chunk
            at = hi
        }
        select_decode_override(gpu ? "metal" : "")
        got <- greedy_steps(gpu ? trg : tr, sb, n)
        select_decode_override("")
        release_kv_pages_(sa)
        release_kv_pages_(sb)
        delete sa
        delete sb
        delete pool
    }
    return <- got
}

// GPU decode, rewind (n_past = 0 session reuse — the ASR pattern), CPU re-prefill of a SHORTER
// different prompt, GPU decode again: stale mirror rows past the rewind would poison the tokens
def private rewind_greedy(tr, trg : Model; prompt, prompt2 : array<int64>; n : int) : array<int64> {
    var got : array<int64>
    with_job_que() {
        setup_dasllama_jobque()
        var s = make_run_state(tr.config, KVDtype.f16, KVDtype.f16)
        select_decode_override("metal")
        eval_(tr, s, prompt)                    // CPU prefill (the planar model)
        var warm <- greedy_steps(trg, s, 8)     // GPU decode advances the mirror watermark
        delete warm
        s.n_past = 0l                           // rewind: reuse the session from scratch
        eval_(tr, s, prompt2)                   // shorter prompt ⇒ first GPU pos < watermark
        got <- greedy_steps(trg, s, n)
        select_decode_override("")
        delete s
    }
    return <- got
}

// ===== P4: batched-decode arms =====

// per-row greedy streams through eval_batch_ (the batch override hooks inside), with optional
// mid-run shrink (retire row 1) and optional interleaved per-row forward() steps (the
// single-stream override serves those — batch <-> single mirror-consistency coverage)
def private batch_greedy(tr, trg : Model; prompts : array<array<int64>>; nsteps : int; kdt : KVDtype;
                         gpu : bool; shrink_at : int; single_every : int) : array<array<int64>> {
    let c = tr.config
    let vocab = c.vocab_size
    let nb = length(prompts)
    var got : array<array<int64>>
    got |> resize(nb)
    with_job_que() {
        setup_dasllama_jobque()
        var ss : array<Session>
        ss |> reserve(nb)
        for (i in range(nb)) {
            ss |> emplace <| make_run_state(c, kdt, kdt)
            eval_(tr, ss[i], prompts[i])
        }
        var ws <- create_batch_workspace_(tr)
        var sess : array<Session?>
        var cur : array<int64>
        var live : array<int>
        for (i in range(nb)) {
            unsafe {
                sess |> push(addr(ss[i]))
            }
            cur |> push(greedy_top(ss[i].logits, vocab))
            live |> push(i)
        }
        select_decode_override(gpu ? "metal" : "")
        for (st in range(nsteps)) {
            if (single_every > 0 && (st + 1) % single_every == 0) {
                for (k in range(length(sess))) {
                    var sp = sess[k]
                    forward(gpu ? trg : tr, *sp, cur[k], sp.n_past)
                    sp.n_past++
                    cur[k] = greedy_top(sp.logits, vocab)
                }
            } else {
                eval_batch_(gpu ? trg : tr, ws, sess, cur)
                for (k in range(length(sess))) {
                    cur[k] = greedy_top(sess[k].logits, vocab)
                }
            }
            for (k in range(length(sess))) {
                got[live[k]] |> push(cur[k])
            }
            if (st == shrink_at && length(sess) > 1) {
                sess |> erase(1)
                cur |> erase(1)
                live |> erase(1)
            }
        }
        select_decode_override("")
        sess |> clear()   // drop the borrowed handles: delete of array<T?> cascades to the pointees
        unsafe {
            delete ws
            delete sess
        }
        delete cur
        delete live
        delete ss
    }
    return <- got
}

def private streams_match(a, b : array<array<int64>>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (i in range(length(a))) {
        if (!same(a[i], b[i])) {
            return false
        }
    }
    return true
}

// one CPU batch step and one GPU batch step from identical prefilled states: per-row logits +
// stored-KV-row envelope (the batch twin of single_step_gate)
[unused_argument(t, tr, trg, nb, kdt, tol_logits, tol_kv, tag, plen)]   // off-Apple the gated body is empty
def private batch_step_gate(t : T?; tr, trg : Model; nb : int; kdt : KVDtype; tol_logits, tol_kv : float; tag : string;
                            plen : int = 40) {
    // whole-body gate: see single_step_gate — off-Apple this helper just needs to compile
    // (plen scales the per-session prompts DOWN for the big dim-ladder models, whose CPU
    // control prefills dominate the arm's wall time)
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let c = tr.config
        let vocab = c.vocab_size
        var sa : array<Session>
        var sb : array<Session>
        var served = 0l
        with_job_que() {
            setup_dasllama_jobque()
            sa |> reserve(nb)
            sb |> reserve(nb)
            for (i in range(nb)) {
                let prompt <- encode(tr, counting_prompt(1 + i, plen + 7 * i), true, false)
                sa |> emplace <| make_run_state(c, kdt, kdt)
                sb |> emplace <| make_run_state(c, kdt, kdt)
                eval_(tr, sa[i], prompt)
                eval_(tr, sb[i], prompt)
            }
            var wsa <- create_batch_workspace_(tr)
            var wsb <- create_batch_workspace_(tr)
            var sessa : array<Session?>
            var sessb : array<Session?>
            var cur : array<int64>
            for (i in range(nb)) {
                unsafe {
                    sessa |> push(addr(sa[i]))
                    sessb |> push(addr(sb[i]))
                }
                cur |> push(greedy_top(sa[i].logits, vocab))
            }
            select_decode_override("")
            eval_batch_(tr, wsa, sessa, cur)          // the CPU control step
            let before = metal_batch_decode_stats().steps
            select_decode_override("metal")
            eval_batch_(trg, wsb, sessb, cur)         // the GPU step (the blob twin)
            served = metal_batch_decode_stats().steps - before
            select_decode_override("")
            sessa |> clear()
            sessb |> clear()
            unsafe {
                delete wsa
                delete wsb
                delete sessa
                delete sessb
            }
            delete cur
        }
        if (served == 0l) {
            feint("{tag}: metal batch decode declined; batch step gate skipped\n")
        } else {
            var maxl = 0.0
            var maxkv = 0.0
            for (i in range(nb)) {
                for (v in range64(vocab)) {
                    maxl = max(maxl, abs(sa[i].logits[v] - sb[i].logits[v]))
                }
                maxkv = max(maxkv, kv_row_max_diff(sa[i], sb[i], c, sa[i].n_past - 1l))
            }
            t |> success(maxl <= tol_logits, "{tag}: one-batch-step logits within {tol_logits} (max diff {maxl})")
            t |> success(maxkv <= tol_kv, "{tag}: stored KV rows within {tol_kv} (max diff {maxkv})")
            to_log(LOG_INFO, "metal batch decode {tag}: logits maxd={maxl} (tol {tol_logits}), kv rows maxd={maxkv} (tol {tol_kv})\n")
        }
        delete sa
        delete sb
    }
}

// interleaved-chunk prefill of paged sessions (multi-run mirror gathers), then batched greedy
def private paged_batch_greedy(tr, trg : Model; prompts : array<array<int64>>; page_rows : int64; nsteps : int; gpu : bool) : array<array<int64>> {
    let c = tr.config
    let vocab = c.vocab_size
    let nb = length(prompts)
    var got : array<array<int64>>
    got |> resize(nb)
    with_job_que() {
        setup_dasllama_jobque()
        var pool <- create_kv_pool_(c, page_rows, KVDtype.f16, KVDtype.f16)
        var ss : array<Session>
        ss |> reserve(nb)
        for (_i in range(nb)) {
            ss |> emplace <| make_run_state_paged(c, pool)
        }
        var longest = 0l
        for (p in prompts) {
            longest = max(longest, long_length(p))
        }
        var at = 0l
        while (at < longest) {   // alternate page-sized chunks -> physically interleaved block tables
            for (i in range(nb)) {
                let np = long_length(prompts[i])
                let hi = min(at + page_rows, np)
                if (at < hi) {
                    var chunk <- [for (j in range64(at, hi)); prompts[i][j]]
                    eval_(tr, ss[i], chunk)
                    delete chunk
                }
            }
            at += page_rows
        }
        var ws <- create_batch_workspace_(tr)
        var sess : array<Session?>
        var cur : array<int64>
        for (i in range(nb)) {
            unsafe {
                sess |> push(addr(ss[i]))
            }
            cur |> push(greedy_top(ss[i].logits, vocab))
        }
        select_decode_override(gpu ? "metal" : "")
        for (_st in range(nsteps)) {
            eval_batch_(gpu ? trg : tr, ws, sess, cur)
            for (k in range(nb)) {
                cur[k] = greedy_top(ss[k].logits, vocab)
                got[k] |> push(cur[k])
            }
        }
        select_decode_override("")
        for (i in range(nb)) {
            release_kv_pages_(ss[i])
        }
        sess |> clear()   // drop the borrowed handles: delete of array<T?> cascades to the pointees
        unsafe {
            delete ws
            delete sess
        }
        delete cur
        delete ss
        delete pool
    }
    return <- got
}

[test]
def test_metal_batch_decode_parity(t : T?) {
    t |> run("GPU batched decode == CPU batch control, greedy streams (Llama-3.2-1B Q8)") <| @(t : T?) {
        if (!family_on(t, "llama")) return
        static_if (typeinfo builtin_module_exists(das_metal)) {
            if (!jit_enabled()) {
                t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
                return
            }
            if (!arm_on(t, "batch")) {
                return
            }
            let path = path_join(models_dir(), "Llama-3.2-1B-Instruct-Q8_0.gguf")
            if (!model_available(t, path)) {
                return
            }
            let prior_wscale = get_wscale_f16()             // process-wide knobs — restored below
            let prior_backend = active_kernel_backend()
            set_wscale_f16(false)
            pin_kernel_backend("portable")   // row-major weights (the resident drivers' donor contract)
            var tr <- load_model(path, QuantMode.q8)
            tr.config.seq_len = min(tr.config.seq_len, 512l)   // batch arms hold up to 12 sessions
            var trg <- blob_twin(t, path, 512l)
            var prompts3 <- [ <- encode(tr, counting_prompt(1, 60), true, false),
                             <- encode(tr, counting_prompt(7, 45), true, false),
                             <- encode(tr, counting_prompt(3, 70), true, false)]
            var prompts6 <- [ <- encode(tr, counting_prompt(1, 40), true, false),
                             <- encode(tr, counting_prompt(5, 52), true, false),
                             <- encode(tr, counting_prompt(2, 33), true, false),
                             <- encode(tr, counting_prompt(9, 61), true, false),
                             <- encode(tr, counting_prompt(4, 47), true, false),
                             <- encode(tr, counting_prompt(6, 55), true, false)]

            // arm B1: B=3 ragged + mid-run shrink — the fixed-B GEMV path (B4 form), f16 mirrors
            let cpu3 <- batch_greedy(tr, trg, prompts3, 24, KVDtype.f16, false, 8, 0)
            var before = metal_batch_decode_stats().steps
            let gpu3 <- batch_greedy(tr, trg, prompts3, 24, KVDtype.f16, true, 8, 0)
            if (metal_batch_decode_stats().steps - before == 0l) {
                metal_decode_shutdown()
                clear_kernel_backend_pin()
                select_kernel_backend(prior_backend)
                set_wscale_f16(prior_wscale)
                delete prompts3
                delete prompts6
                delete tr
                delete trg
                feint("metal batch decode declined (no Metal device?); batch parity skipped\n")
                return
            }
            t |> success(streams_match(cpu3, gpu3), "B=3 ragged + shrink: batched greedy streams match the CPU control (GEMV path)")

            // arm B1b: B=2 — the fixed-B GEMV B2 form (covers the fused w13sw_b2 kernel)
            var prompts2 <- [ <- encode(tr, counting_prompt(2, 50), true, false),
                             <- encode(tr, counting_prompt(8, 38), true, false)]
            let cpu2 <- batch_greedy(tr, trg, prompts2, 20, KVDtype.f16, false, -1, 0)
            let gpu2 <- batch_greedy(tr, trg, prompts2, 20, KVDtype.f16, true, -1, 0)
            t |> success(streams_match(cpu2, gpu2), "B=2: batched greedy streams match the CPU control (B2 GEMV path)")
            delete prompts2

            // arm B2: B=6 — the M-pad-32 GEMM path
            let cpu6 <- batch_greedy(tr, trg, prompts6, 16, KVDtype.f16, false, -1, 0)
            before = metal_batch_decode_stats().steps
            let gpu6 <- batch_greedy(tr, trg, prompts6, 16, KVDtype.f16, true, -1, 0)
            t |> success(metal_batch_decode_stats().steps - before > 0l, "B=6 arm served on the GPU")
            t |> success(streams_match(cpu6, gpu6), "B=6: batched greedy streams match the CPU control (GEMM path)")

            // arm B3: interleaved per-row forward() steps — batch <-> single mirror consistency
            // (the single-stream override serves the interleaved steps, same arena slices)
            let cpum <- batch_greedy(tr, trg, prompts3, 18, KVDtype.f16, false, -1, 3)
            let gpum <- batch_greedy(tr, trg, prompts3, 18, KVDtype.f16, true, -1, 3)
            t |> success(streams_match(cpum, gpum), "mixed batch/single schedule matches the CPU control")

            // arm B4: one-step logits + stored-KV-row tolerance, both paths x all mirror dtypes
            batch_step_gate(t, tr, trg, 3, KVDtype.f16, 0.5, 0.25, "B=3 f16")
            batch_step_gate(t, tr, trg, 6, KVDtype.f16, 0.5, 0.25, "B=6 f16")
            batch_step_gate(t, tr, trg, 3, KVDtype.f32, 0.5, 0.25, "B=3 f32")
            batch_step_gate(t, tr, trg, 6, KVDtype.f32, 0.5, 0.25, "B=6 f32")
            batch_step_gate(t, tr, trg, 3, KVDtype.q8_0, 0.5, 0.25, "B=3 q8_0")
            batch_step_gate(t, tr, trg, 6, KVDtype.q8_0, 0.5, 0.25, "B=6 q8_0")
            batch_step_gate(t, tr, trg, 3, KVDtype.tq4, 1.0, 2.0, "B=3 tq4")   // quant-step-scale envelopes
            batch_step_gate(t, tr, trg, 6, KVDtype.tq4, 1.0, 2.0, "B=6 tq4")   // (see the single tq4 arm)

            // arm B4b: q8_0 greedy streams — the block-codec batch kernels end to end (format-
            // matrix W3): B=3 covers the GEMV path + the chunked partB attention as depth crosses
            // the single-tg ceiling, B=6 the GEMM path; the CPU q8_0 batch path is the oracle
            let cpu8b <- batch_greedy(tr, trg, prompts3, 24, KVDtype.q8_0, false, -1, 0)
            before = metal_batch_decode_stats().steps
            let gpu8b <- batch_greedy(tr, trg, prompts3, 24, KVDtype.q8_0, true, -1, 0)
            t |> success(metal_batch_decode_stats().steps - before > 0l, "B=3 q8_0 arm served on the GPU")
            t |> success(streams_match(cpu8b, gpu8b), "B=3 q8_0 batched greedy streams match the CPU control")
            let cpu8b6 <- batch_greedy(tr, trg, prompts6, 12, KVDtype.q8_0, false, -1, 0)
            let gpu8b6 <- batch_greedy(tr, trg, prompts6, 12, KVDtype.q8_0, true, -1, 0)
            t |> success(streams_match(cpu8b6, gpu8b6), "B=6 q8_0 batched greedy streams match the CPU control (GEMM path)")

            // arm B4c: tq4 greedy streams — FWHT store + nibble attention + unsign, batch shape
            let cpu4b <- batch_greedy(tr, trg, prompts3, 24, KVDtype.tq4, false, -1, 0)
            before = metal_batch_decode_stats().steps
            let gpu4b <- batch_greedy(tr, trg, prompts3, 24, KVDtype.tq4, true, -1, 0)
            t |> success(metal_batch_decode_stats().steps - before > 0l, "B=3 tq4 arm served on the GPU")
            t |> success(streams_match(cpu4b, gpu4b), "B=3 tq4 batched greedy streams match the CPU control")

            // arm B5: paged sessions — interleaved page tables force multi-run mirror gathers
            let paged_cpu <- paged_batch_greedy(tr, trg, prompts3, 64l, 8, false)
            before = metal_batch_decode_stats().steps
            let paged_gpu <- paged_batch_greedy(tr, trg, prompts3, 64l, 8, true)
            t |> success(metal_batch_decode_stats().steps - before > 0l, "paged batch arm served on the GPU")
            t |> success(streams_match(paged_cpu, paged_gpu), "paged batched streams match the CPU control")
            delete prompts6

            // arm B6: wscale_f16 model — the dual-view s16 scale reads on both batch paths
            set_wscale_f16(true)
            var tr16 <- load_model(path, QuantMode.q8)
            tr16.config.seq_len = min(tr16.config.seq_len, 512l)
            var tr16g <- blob_twin(t, path, 512l)
            var prompts16 <- [ <- encode(tr16, counting_prompt(1, 44), true, false),
                              <- encode(tr16, counting_prompt(6, 58), true, false),
                              <- encode(tr16, counting_prompt(3, 36), true, false),
                              <- encode(tr16, counting_prompt(8, 50), true, false),
                              <- encode(tr16, counting_prompt(2, 63), true, false)]
            let cpu16b <- batch_greedy(tr16, tr16g, prompts16, 12, KVDtype.f16, false, -1, 0)
            before = metal_batch_decode_stats().steps
            let gpu16b <- batch_greedy(tr16, tr16g, prompts16, 12, KVDtype.f16, true, -1, 0)
            t |> success(metal_batch_decode_stats().steps - before > 0l, "wscale16 batch arm served on the GPU")
            t |> success(streams_match(cpu16b, gpu16b), "B=5 wscale16 batched streams match the CPU control (s16 GEMM)")
            var prompts16v <- [ <- clone_to_move(prompts16[0]), <- clone_to_move(prompts16[1]), <- clone_to_move(prompts16[2])]
            let cpu16v <- batch_greedy(tr16, tr16g, prompts16v, 12, KVDtype.f16, false, -1, 0)
            let gpu16v <- batch_greedy(tr16, tr16g, prompts16v, 12, KVDtype.f16, true, -1, 0)
            t |> success(streams_match(cpu16v, gpu16v), "B=3 wscale16 batched streams match the CPU control (s16 fixed-B GEMV)")
            set_wscale_f16(false)
            delete prompts16
            delete prompts16v
            delete tr16
            delete tr16g

            // arm B7: hs=128 model — the fused single-pass partD attention path (the 1B's heads
            // are hs=64 and keep the chunked pair, so the 3B is partD's only parity coverage;
            // plain stat check — a mid-test model_available skip would flip the whole test)
            let path3b = path_join(models_dir(), "Llama-3.2-3B-Instruct-Q8_0.gguf")
            if (arm_on(t, "batchB7-partd")) {
                if (stat(path3b).is_valid) {
                    var tr3 <- load_model(path3b, QuantMode.q8)
                    tr3.config.seq_len = min(tr3.config.seq_len, 512l)
                    var tr3g <- blob_twin(t, path3b, 512l)
                    var promptsd <- [ <- encode(tr3, counting_prompt(1, 48), true, false),
                                     <- encode(tr3, counting_prompt(5, 62), true, false),
                                     <- encode(tr3, counting_prompt(3, 39), true, false)]
                    let cpud <- batch_greedy(tr3, tr3g, promptsd, 16, KVDtype.f16, false, -1, 0)
                    before = metal_batch_decode_stats().steps
                    let gpud <- batch_greedy(tr3, tr3g, promptsd, 16, KVDtype.f16, true, -1, 0)
                    t |> success(metal_batch_decode_stats().steps - before > 0l, "hs=128 partD arm served on the GPU")
                    t |> success(streams_match(cpud, gpud), "B=3 hs=128 batched greedy streams match the CPU control (partD attention)")
                    batch_step_gate(t, tr3, tr3g, 3, KVDtype.f16, 0.5, 0.25, "B=3 hs128 f16")
                    batch_step_gate(t, tr3, tr3g, 3, KVDtype.f32, 0.5, 0.25, "B=3 hs128 f32")
                    batch_step_gate(t, tr3, tr3g, 3, KVDtype.q8_0, 0.5, 0.25, "B=3 hs128 q8_0")   // the partD q8 twin
                    batch_step_gate(t, tr3, tr3g, 3, KVDtype.tq4, 1.0, 2.0, "B=3 hs128 tq4")      // ... and the partD tq4 twin
                    delete promptsd
                    delete tr3
                    delete tr3g
                } else {
                    feint("Llama-3.2-3B model not present; partD hs=128 arm skipped\n")
                }
            }

            // arm B8: K-quant weights (Q4_K_M — M2.1): the batched step serves B <= 8 via nrows
            // per-stream kq plane GEMVs (mixed k4/k6 tensors, split w13 + ew2 swiglu, kq batch
            // classifier); the CPU kq batch path is the oracle. B=3 f16 + B=2 q8_0-KV combos.
            let kpath_b = path_join(models_dir(), "Llama-3.2-1B-Instruct-Q4_K_M.gguf")
            if (arm_on(t, "batchB8-kq")) {
                if (stat(kpath_b).is_valid) {
                    var trkb <- load_model(kpath_b, QuantMode.q8)
                    trkb.config.seq_len = min(trkb.config.seq_len, 512l)
                    var trkbg <- blob_twin(t, kpath_b, 512l)
                    var promptsk <- [ <- encode(trkb, counting_prompt(1, 52), true, false),
                                     <- encode(trkb, counting_prompt(6, 41), true, false),
                                     <- encode(trkb, counting_prompt(3, 64), true, false)]
                    let cpuk <- batch_greedy(trkb, trkbg, promptsk, 16, KVDtype.f16, false, -1, 0)
                    before = metal_batch_decode_stats().steps
                    let gpuk <- batch_greedy(trkb, trkbg, promptsk, 16, KVDtype.f16, true, -1, 0)
                    t |> success(metal_batch_decode_stats().steps - before > 0l, "kq batch arm served on the GPU")
                    t |> success(streams_match(cpuk, gpuk), "B=3 kq batched greedy streams match the CPU control")
                    var promptsk2 <- [ <- clone_to_move(promptsk[0]), <- clone_to_move(promptsk[1])]
                    let cpuk2 <- batch_greedy(trkb, trkbg, promptsk2, 14, KVDtype.q8_0, false, -1, 0)
                    let gpuk2 <- batch_greedy(trkb, trkbg, promptsk2, 14, KVDtype.q8_0, true, -1, 0)
                    t |> success(streams_match(cpuk2, gpuk2), "B=2 kq q8_0-KV batched greedy streams match the CPU control")
                    // B=12: the kq mul_mm batch form (B >= 9 leaves the per-stream GEMVs)
                    var promptsk12 : array<array<int64>>
                    for (i in range(12)) {
                        promptsk12 |> emplace <| encode(trkb, counting_prompt(1 + i, 34 + 2 * i), true, false)
                    }
                    let cpuk12 <- batch_greedy(trkb, trkbg, promptsk12, 10, KVDtype.f16, false, -1, 0)
                    before = metal_batch_decode_stats().steps
                    let gpuk12 <- batch_greedy(trkb, trkbg, promptsk12, 10, KVDtype.f16, true, -1, 0)
                    t |> success(metal_batch_decode_stats().steps - before > 0l, "B=12 kq arm served on the GPU")
                    t |> success(streams_match(cpuk12, gpuk12), "B=12 kq batched greedy streams match the CPU control (kq mul_mm)")
                    delete promptsk12
                    delete promptsk
                    delete promptsk2
                    delete trkb
                    delete trkbg
                } else {
                    feint("Llama-3.2-1B Q4_K_M not present; kq batch arm skipped\n")
                }
            }

            // the k5 half of the same arm (Q5_K_M — k5-dominant planes, k6 mixed in): B <= 8
            // rides the kq ext mv twins. B=3 = the ext4 form with nr=3 gating the pad column;
            // B=2 = the ext2 form. Same greedy CPU oracle.
            let kpath_5 = path_join(models_dir(), "Llama-3.2-1B-Instruct-Q5_K_M.gguf")
            if (arm_on(t, "batchB8-kq")) {
                if (stat(kpath_5).is_valid) {
                    var trk5 <- load_model(kpath_5, QuantMode.q8)
                    trk5.config.seq_len = min(trk5.config.seq_len, 512l)
                    var trk5g <- blob_twin(t, kpath_5, 512l)
                    var prompts5 <- [ <- encode(trk5, counting_prompt(2, 47), true, false),
                                     <- encode(trk5, counting_prompt(4, 58), true, false),
                                     <- encode(trk5, counting_prompt(7, 36), true, false)]
                    let cpu5 <- batch_greedy(trk5, trk5g, prompts5, 16, KVDtype.f16, false, -1, 0)
                    before = metal_batch_decode_stats().steps
                    let gpu5 <- batch_greedy(trk5, trk5g, prompts5, 16, KVDtype.f16, true, -1, 0)
                    t |> success(metal_batch_decode_stats().steps - before > 0l, "k5 batch arm served on the GPU")
                    t |> success(streams_match(cpu5, gpu5), "B=3 k5 batched greedy streams match the CPU control (kq ext mv twins)")
                    var prompts52 <- [ <- clone_to_move(prompts5[0]), <- clone_to_move(prompts5[1])]
                    let cpu52 <- batch_greedy(trk5, trk5g, prompts52, 14, KVDtype.f16, false, -1, 0)
                    let gpu52 <- batch_greedy(trk5, trk5g, prompts52, 14, KVDtype.f16, true, -1, 0)
                    t |> success(streams_match(cpu52, gpu52), "B=2 k5 batched greedy streams match the CPU control (ext2 form)")
                    delete prompts5
                    delete prompts52
                    delete trk5
                    delete trk5g
                } else {
                    feint("Llama-3.2-1B Q5_K_M not present; k5 batch arm skipped\n")
                }
            }

            // the leak gate: every GPU object the driver holds releases
            metal_decode_stage_report()
            metal_decode_shutdown()
            t |> equal(metal_live_object_count(), 0l)
            clear_kernel_backend_pin()
            select_kernel_backend(prior_backend)
            set_wscale_f16(prior_wscale)
            delete prompts3
            delete tr
            delete trg
        } else {
            feint("das_metal is not built on this platform; nothing to compare\n")
        }
    }
}

[test]
def test_metal_decode_parity(t : T?) {
    t |> run("GPU-resident decode == CPU control, greedy continuations (Llama-3.2-1B Q8)") <| @(t : T?) {
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
            let prior_backend = active_kernel_backend()     // (every exit path runs the restore)
            set_wscale_f16(false)       // the resident path reads the f32 scale plane (declines on s16)
            pin_kernel_backend("portable")  // ...and ROW-MAJOR weights (same donor contract as prefill)
            var tr <- load_model(path, QuantMode.q8)
            if (tr.config.seq_len > 2048l) {
                tr.config.seq_len = 2048l
            }
            var trg <- blob_twin(t, path, 2048l)
            let prompt <- encode(tr, counting_prompt(1, 120), true, false)
            let prompt2 <- encode(tr, counting_prompt(7, 60), true, false)

            // CPU control (f16 KV — the mirror dtype the decode arms run)
            let cpu16 <- continuation(tr, tr, prompt, 40, KVDtype.f16)
            t |> equal(length(cpu16), 40)

            var before = 0l
            // arm 1: CPU-prefill → GPU-decode, 40-token token-exact parity
            if (arm_on(t, "arm1-basic")) {
                t |> success(select_decode_override("metal"), "override registered")
                before = metal_decode_stats().decodes
                let gpu16 <- continuation(tr, trg, prompt, 40, KVDtype.f16)
                let served = metal_decode_stats().decodes - before
                select_decode_override("")
                if (served == 0l) {
                    metal_decode_shutdown()
                    clear_kernel_backend_pin()
                    select_kernel_backend(prior_backend)
                    set_wscale_f16(prior_wscale)
                    delete tr
                    delete trg
                    feint("metal decode declined (no Metal device?); GPU parity skipped\n")
                    return
                }
                t |> success(same(cpu16, gpu16), "40-token continuation matches the CPU control (CPU prefill → GPU decode)")
            }

            // arm 2: Metal-prefill → GPU-decode (both residents live, one queue each)
            if (arm_on(t, "arm2-hybrid")) {
                t |> success(select_prefill_override("metal"), "prefill override registered")
                set_metal_prefill_min_npos(1l)
                select_decode_override("metal")
                before = metal_decode_stats().decodes
                let pf_before = metal_prefill_stats().prefills
                let hyb <- continuation(trg, trg, prompt, 40, KVDtype.f16)
                let hyb_decodes = metal_decode_stats().decodes - before
                let hyb_prefills = metal_prefill_stats().prefills - pf_before
                select_decode_override("")
                select_prefill_override("")
                set_metal_prefill_min_npos(64l)
                t |> success(hyb_decodes > 0l && hyb_prefills > 0l, "hybrid arm served on both devices")
                t |> success(same(cpu16, hyb), "40-token continuation matches the CPU control (Metal prefill → GPU decode)")
            }

            // arm 13: the concurrent encoder + hazard tracker. Paranoid window isolates the
            // encoder flip (barrier every dispatch); STRICT window proves declaration coverage
            // (an undeclared dispatch panics) with elision live — the migration's standing gate
            if (arm_on(t, "arm13-conc")) {
                set_metal_decode_concurrent(1)
                set_metal_decode_hazard(true, false)
                select_decode_override("metal")
                let par <- continuation(tr, trg, prompt, 40, KVDtype.f16)
                t |> success(same(cpu16, par), "40-token continuation matches the CPU control (PARANOID concurrent encode)")
                set_metal_decode_hazard(false, true)
                let h0 = metal_hazard_stats()
                let cnc <- continuation(tr, trg, prompt, 40, KVDtype.f16)
                let hz = metal_hazard_stats()
                select_decode_override("")
                set_metal_decode_hazard(false, false)
                set_metal_decode_concurrent(-1)
                t |> success(same(cpu16, cnc), "40-token continuation matches the CPU control (STRICT concurrent encode)")
                t |> success(hz.gated > h0.gated, "hz_gate saw the step's dispatches")
                t |> success(hz.barriers - h0.barriers < hz.gated - h0.gated, "barrier elision engaged (barriers < gated dispatches)")
                t |> equal(hz.undeclared, h0.undeclared)
            }

            // arm 3: single-step logits + stored-KV-row tolerance, both mirror dtypes (NOT
            // byte-equality — f32-X GEMVs vs CPU q8 activations, online GPU softmax)
            if (arm_on(t, "arm3-step")) {
                single_step_gate(t, tr, trg, prompt, KVDtype.f16, 0.5, 0.25, "f16")
                single_step_gate(t, tr, trg, prompt, KVDtype.f32, 0.5, 0.25, "f32")
            }

            // arm 4: paged sessions — interleaved page tables force multi-run mirror gathers
            if (arm_on(t, "arm4-paged")) {
                let paged_cpu <- paged_greedy(tr, trg, prompt, 64l, 16, false)
                before = metal_decode_stats().decodes
                let paged_gpu <- paged_greedy(tr, trg, prompt, 64l, 16, true)
                t |> success(metal_decode_stats().decodes - before > 0l, "paged arm served on the GPU")
                t |> success(same(paged_cpu, paged_gpu), "16-token paged continuation matches the CPU control")
            }

            // arm 5: rewind — n_past = 0 reuse re-mirrors from scratch (watermark contract)
            if (arm_on(t, "arm5-rewind")) {
                var sc = make_run_state(tr.config, KVDtype.f16, KVDtype.f16)
                var rewind_cpu : array<int64>
                with_job_que() {
                    setup_dasllama_jobque()
                    eval_(tr, sc, prompt2)
                    rewind_cpu <- greedy_steps(tr, sc, 16)
                }
                delete sc
                before = metal_decode_stats().decodes
                let rewind_gpu <- rewind_greedy(tr, trg, prompt, prompt2, 16)
                t |> success(metal_decode_stats().decodes - before > 0l, "rewind arm served on the GPU")
                t |> success(same(rewind_cpu, rewind_gpu), "16-token post-rewind continuation matches the CPU control")
            }

            // arm 6: session churn under a tiny LRU cap — retired sessions' mirrors evict
            if (arm_on(t, "arm6-churn")) {
                set_metal_decode_mirror_cap_mb(1)
                with_job_que() {
                    setup_dasllama_jobque()
                    select_decode_override("metal")
                    for (_i in range(3)) {
                        var s = make_run_state(tr.config, KVDtype.f16, KVDtype.f16)
                        eval_(tr, s, prompt2)
                        var ids <- greedy_steps(trg, s, 4)
                        delete ids
                        delete s
                    }
                    select_decode_override("")
                }
                t |> equal(metal_decode_mirror_stats().mirrors, 1l)
                set_metal_decode_mirror_cap_mb(-1)
            }

            // arm 7: q8_0 KV mirror — the block-codec decode path (format-matrix W3): GPU
            // rope+quantize store + dual-view block-dequant attention; the CPU q8_0 decode
            // path is the oracle (both arms consume the same codec)
            if (arm_on(t, "arm7-q8kv")) {
                let cpu8 <- continuation(tr, tr, prompt, 40, KVDtype.q8_0)
                select_decode_override("metal")
                before = metal_decode_stats().decodes
                let gpu8 <- continuation(tr, trg, prompt, 40, KVDtype.q8_0)
                select_decode_override("")
                t |> success(metal_decode_stats().decodes - before > 0l, "q8_0 arm served on the GPU")
                t |> success(same(cpu8, gpu8), "40-token q8_0-cache continuation matches the CPU control")
                single_step_gate(t, tr, trg, prompt, KVDtype.q8_0, 0.5, 0.25, "q8_0")
            }

            // arm 7b: tq4 KV mirror — the rotated 4-bit codec (W3): threadgroup-per-head FWHT
            // store, nibble attention, fwht-unsign epilogue; the CPU tq4 path is the oracle.
            // Envelopes are quant-step scale (d = amax/8 ⇒ one flipped cell moves a stored row
            // ~1.3 and logits ~0.65 — measured on the 1B; tokens stay exact on the fixture), so
            // both tolerances sit at ~2x the q8_0 arm's.
            if (arm_on(t, "arm7b-tq4kv")) {
                let cpu4 <- continuation(tr, tr, prompt, 40, KVDtype.tq4)
                select_decode_override("metal")
                before = metal_decode_stats().decodes
                let gpu4 <- continuation(tr, trg, prompt, 40, KVDtype.tq4)
                select_decode_override("")
                t |> success(metal_decode_stats().decodes - before > 0l, "tq4 arm served on the GPU")
                t |> success(same(cpu4, gpu4), "40-token tq4-cache continuation matches the CPU control")
                single_step_gate(t, tr, trg, prompt, KVDtype.tq4, 1.0, 2.0, "tq4")
            }

            // arm 8: wscale_f16 model — the f16-scale GEMV twin reads the qscales16 halfword
            // plane (llama.cpp q8_0 byte parity); a fresh load owns fresh weight regions
            if (arm_on(t, "arm8-s16")) {
                set_wscale_f16(true)
                var tr16 <- load_model(path, QuantMode.q8)
                if (tr16.config.seq_len > 2048l) {
                    tr16.config.seq_len = 2048l
                }
                var tr16g <- blob_twin(t, path, 2048l)
                let cpu16s <- continuation(tr16, tr16, prompt, 40, KVDtype.f16)
                select_decode_override("metal")
                before = metal_decode_stats().decodes
                let gpu16s <- continuation(tr16, tr16g, prompt, 40, KVDtype.f16)
                select_decode_override("")
                t |> success(metal_decode_stats().decodes - before > 0l, "wscale16 arm served on the GPU")
                t |> success(same(cpu16s, gpu16s), "40-token wscale16 continuation matches the CPU control")
                set_wscale_f16(false)
                delete tr16
                delete tr16g
            }

            // arm 9: model reload — the weights-epoch guard. The weight caches key by HOST
            // ADDRESS; deleting a model and loading another can land the new arrays at recycled
            // addresses, and a stale cache hit serves the DEAD model's buffers (pre-W4 this
            // produced silent garbage continuations; the load-epoch flush is the fix)
            if (arm_on(t, "arm9-reload")) {
                var trr <- load_model(path, QuantMode.q8)
                if (trr.config.seq_len > 2048l) {
                    trr.config.seq_len = 2048l
                }
                var trrg <- blob_twin(t, path, 2048l)
                let cpu_r <- continuation(trr, trr, prompt, 40, KVDtype.f16)
                select_decode_override("metal")
                before = metal_decode_stats().decodes
                let gpu_r <- continuation(trr, trrg, prompt, 40, KVDtype.f16)
                select_decode_override("")
                t |> success(metal_decode_stats().decodes - before > 0l, "reload arm served on the GPU")
                t |> success(same(cpu_r, gpu_r), "40-token post-reload continuation matches the CPU control (weights-epoch guard)")
                delete trr
                delete trrg
            }

            // arm 10: K-quant weights (Q4_K_M — format-matrix W4): per-site kq plane GEMVs,
            // mixed k4/k6 tensors incl. the tied Q6_K classifier on the GPU-logits path. The
            // envelope is wider than q8's: the CPU kq dots run Q8_K-quantized activations while
            // the GPU kernels stream f32 x, so the two paths' WEIGHT math differs (measured
            // one-step logits maxd ~0.42 vs ~0.25 on the q8 arm); tokens stay exact on the fixture.
            let kpath = path_join(models_dir(), "Llama-3.2-1B-Instruct-Q4_K_M.gguf")
            if (arm_on(t, "arm10-kq")) {
                if (stat(kpath).is_valid) {
                    var trk <- load_model(kpath, QuantMode.q8)
                    if (trk.config.seq_len > 2048l) {
                        trk.config.seq_len = 2048l
                    }
                    var trkg <- blob_twin(t, kpath, 2048l)
                    let cpu_k <- continuation(trk, trk, prompt, 40, KVDtype.f16)
                    select_decode_override("metal")
                    before = metal_decode_stats().decodes
                    let gpu_k <- continuation(trk, trkg, prompt, 40, KVDtype.f16)
                    select_decode_override("")
                    t |> success(metal_decode_stats().decodes - before > 0l, "kq arm served on the GPU")
                    t |> success(same(cpu_k, gpu_k), "40-token K-quant continuation matches the CPU control")
                    single_step_gate(t, trk, trkg, prompt, KVDtype.f16, 0.8, 0.5, "kq/f16")
                    single_step_gate(t, trk, trkg, prompt, KVDtype.q8_0, 0.8, 0.5, "kq/q8_0")
                    delete trk
                    delete trkg
                } else {
                    feint("Llama-3.2-1B Q4_K_M not present (llama-quantize the Q8 source); kq arm skipped\n")
                }
            }

            // arm 11: depth past 2048 (M2.4 — the retired MAX_ATTN_ROWS gate): a ~2030-token
            // prompt + 40 decode steps crosses the old cap mid-continuation; chunked attention
            // (1B, hs=64) scans the full depth. f16 + q8_0 mirrors (block-codec row offsets at
            // depth). The CPU control is depth-unbounded by construction.
            if (arm_on(t, "arm11-depth")) {
                var trd <- load_model(path, QuantMode.q8)
                trd.config.seq_len = min(trd.config.seq_len, 2176l)
                var trdg <- blob_twin(t, path, 2176l)
                var deep_p <- encode(trd, counting_prompt(1, 700), true, false)
                // the guard is load-bearing: a shorter prompt makes every "past 2048" claim below
                // vacuous (counting to 620 tokenized to only 1860 — the arm silently tested nothing)
                t |> success(long_length(deep_p) >= 2030l, "deep prompt reaches 2030 tokens")
                deep_p |> resize(2030)
                let dcpu16 <- continuation(trd, trd, deep_p, 40, KVDtype.f16)
                select_decode_override("metal")
                before = metal_decode_stats().decodes
                let dgpu16 <- continuation(trd, trdg, deep_p, 40, KVDtype.f16)
                select_decode_override("")
                t |> success(metal_decode_stats().decodes - before > 30l, "deep arm: decode steps past 2048 served on the GPU")
                t |> success(same(dcpu16, dgpu16), "40-token continuation crossing depth 2048 matches the CPU control")
                let dcpu8 <- continuation(trd, trd, deep_p, 40, KVDtype.q8_0)
                select_decode_override("metal")
                let dgpu8 <- continuation(trd, trdg, deep_p, 40, KVDtype.q8_0)
                select_decode_override("")
                t |> success(same(dcpu8, dgpu8), "40-token q8_0-KV continuation crossing depth 2048 matches the CPU control")
                delete deep_p
                delete trd
                delete trdg
                // ... and the partD form (hs=128 — the 3B): its 16-simdgroup slice loop is the
                // other attention shape that never ran past the old cap
                let path3d = path_join(models_dir(), "Llama-3.2-3B-Instruct-Q8_0.gguf")
                if (stat(path3d).is_valid) {
                    var trd3 <- load_model(path3d, QuantMode.q8)
                    // seq 4224 makes the mirror slice ~470MB: the fresh 256MB arena MUST grow
                    // (M2.7 grow-on-demand exercised under parity — the rebuild re-mirrors)
                    trd3.config.seq_len = min(trd3.config.seq_len, 4224l)
                    var trd3g <- blob_twin(t, path3d, 4224l)
                    var deep_p3 <- encode(trd3, counting_prompt(1, 700), true, false)
                    t |> success(long_length(deep_p3) >= 2030l, "deep partD prompt reaches 2030 tokens")
                    deep_p3 |> resize(2030)
                    let d3cpu <- continuation(trd3, trd3, deep_p3, 40, KVDtype.f16)
                    select_decode_override("metal")
                    before = metal_decode_stats().decodes
                    let d3gpu <- continuation(trd3, trd3g, deep_p3, 40, KVDtype.f16)
                    select_decode_override("")
                    t |> success(metal_decode_stats().decodes - before > 30l, "deep partD arm served on the GPU")
                    t |> success(same(d3cpu, d3gpu), "40-token hs=128 continuation crossing depth 2048 matches the CPU control (partD)")
                    delete deep_p3
                    delete trd3
                    delete trd3g
                } else {
                    feint("Llama-3.2-3B model not present; deep partD arm skipped\n")
                }
            }

            // arm 12: the dim ladder (M2.6 — the retired batch_dim_cap gate): dim 4096 (8B)
            // past B=4 rides the q8 GEMM tier; the batched unfused add+rms pair (what
            // dim > 4096 selects) parity-rides the 1B via its pin (a 70B batch CPU control
            // streams 40GB against the GPU's wired copy — it OOM-killed this box; the 70B
            // batch cells stay ENGAGE-gated in the support matrix, GPU-only). The 8B/70B are
            // large-tier: they run only under DASLLAMA_PARITY_FULL (the pre-commit gate).
            if (arm_on(t, "arm12-dim")) {
                let path8 = path_join(models_dir(), "Meta-Llama-3.1-8B-Instruct-Q8_0.gguf")
                if (model_available(t, path8)) {
                    var tr8 <- load_model(path8, QuantMode.q8)
                    tr8.config.seq_len = min(tr8.config.seq_len, 512l)
                    var tr8g <- blob_twin(t, path8, 512l)
                    batch_step_gate(t, tr8, tr8g, 8, KVDtype.f16, 0.5, 0.25, "B=8 dim4096 f16", [plen = 12])
                    delete tr8
                    delete tr8g
                }
                set_metal_batch_addrms_unfused(true)
                batch_step_gate(t, tr, trg, 3, KVDtype.f16, 0.5, 0.25, "B=3 unfused add+rms pair", [plen = 12])
                set_metal_batch_addrms_unfused(false)
                let path70 = path_join(models_dir(), "Llama-3.3-70B-Instruct-Q4_K_M.gguf")
                if (model_available(t, path70)) {
                    // ENGAGE-only: a planar + blob 70B pair is ~80GB resident — same policy as
                    // the support matrix's 70B batch cells (no CPU control against the 70B)
                    var tr70 <- blob_twin(t, path70, 256l)
                    var p70 <- encode(tr70, counting_prompt(1, 12), true, false)
                    var s70 = make_run_state(tr70.config, KVDtype.f16, KVDtype.f16)
                    with_job_que() {
                        setup_dasllama_jobque()
                        select_prefill_override("metal")   // blob models have no CPU prefill
                        eval_(tr70, s70, p70)
                        select_prefill_override("")
                        select_decode_override("metal")
                        before = metal_decode_stats().decodes
                        var ids70 <- greedy_steps(tr70, s70, 4)
                        select_decode_override("")
                        delete ids70
                    }
                    t |> success(metal_decode_stats().decodes - before > 0l, "70B dim8192 decode served on the GPU")
                    delete s70
                    delete p70
                    delete tr70
                }
            }

            // the leak gate: every GPU object both drivers hold releases
            metal_decode_stage_report()
            metal_decode_shutdown()
            metal_prefill_shutdown()
            t |> equal(metal_live_object_count(), 0l)
            clear_kernel_backend_pin()
            select_kernel_backend(prior_backend)
            set_wscale_f16(prior_wscale)
            delete tr
            delete trg
        } else {
            feint("das_metal is not built on this platform; nothing to compare\n")
        }
    }
}
