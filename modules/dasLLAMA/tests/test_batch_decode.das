options gen2
options persistent_heap   // several by-value Sessions + a BatchWorkspace per subtest; explicit deletes below
options stack = 131072   // batch harness holds 3 Sessions + a workspace (its own Session) in one frame

require dastest/testing_boost public
require dasllama/dasllama_transformer   // nolint:STYLE029 — umbrella fires each arch [init] registration
require dasllama/dasllama_math   // setup_dasllama_jobque_ + the runtime knob setters (engine-level, not re-exported)
require llvm/daslib/f16_cvt
require daslib/jobque_boost
require daslib/fio
require math

// Batched decode vs sequential: eval_batch_ must reproduce the per-session forward() stream —
// token-for-token greedy, per-step logits, and the final KV caches. Needs llama2.c's
// stories15M.bin (gitignored); skips cleanly when absent, same contract as test_forward.
// The reference pins the fused decode chain OFF (the std path is what the batch worker mirrors).
def private llama2c_dir() : string {
    let env = get_env_variable("DASLLAMA_LLAMA2C_DIR")
    return empty(env) ? "/Users/borisbatkin/Work/llama2.c" : env
}
let MODEL_PATH = path_join(llama2c_dir(), "stories15M.bin")

// ragged prompts (raw stories15M token ids, BOS-first — no tokenizer.bin dependency)
def private prompt_a() : array<int64> => [1l, 9038l, 2501l, 263l, 931l]
def private prompt_b() : array<int64> => [1l, 306l, 763l]
def private prompt_c() : array<int64> => [1l, 3439l, 278l, 1781l, 367l, 263l, 2217l]

def private greedy_top(lg : array<float>; vocab : int64) : int64 {
    var best = 0l
    var bestv = lg[0]
    for (i in range64(vocab)) {
        if (lg[i] > bestv) {
            bestv = lg[i]
            best = i
        }
    }
    return best
}

// one session's sequential greedy stream: continuation tokens, the logits after every step, and
// clones of the final caches — the oracle a batched twin must reproduce
struct private RefRun {
    toks : array<int64>   // greedy continuation token per step
    lg : array<float>     // logits after each step (nsteps x vocab)
    kc : array<uint8>     // final key_cache
    vc : array<uint8>     // final value_cache
    npast : int64
}

def private run_ref(tr : Model; kdt : KVDtype; prompt : array<int64>; nsteps, vocab : int64) : RefRun {
    var s <- make_run_state(tr.config, kdt, kdt)
    eval_(tr, s, prompt)
    var r : RefRun
    r.lg |> reserve(int(nsteps * vocab))
    r.toks |> reserve(int(nsteps))
    var cur = greedy_top(s.logits, vocab)
    for (_st in range64(nsteps)) {
        eval_(tr, s, [cur])
        r.lg |> push_from(s.logits)
        cur = greedy_top(s.logits, vocab)
        r.toks |> push(cur)
    }
    r.kc := s.key_cache
    r.vc := s.value_cache
    r.npast = s.n_past
    delete s
    return <- r
}

// max |decoded difference| over the VALID cache region (rows 0..npast-1 of every own-KV layer);
// both blobs share one layout, so offsets line up by construction
def private kv_blob_max_diff(blob : array<uint8>; refb : array<uint8>; pref : array<int64>;
                             dt : KVDtype; c : Config; npast : int64) : float {
    var maxd = 0.0
    unsafe {
        let bp = addr<uint8 const?>(blob[0])
        let rp = addr<uint8 const?>(refb[0])
        for (l in range64(c.n_layers)) {
            if (layer_kv_source(c, l) != l) {
                continue
            }
            let kvd = layer_kv_dim(c, l)
            let rowb = kv_row_bytes(dt, kvd)
            let base = pref[l] * c.seq_len
            for (row in range64(npast)) {
                if (dt == KVDtype.f16) {
                    let hb = reinterpret<uint16 const?>(bp + base + row * rowb)
                    let hr = reinterpret<uint16 const?>(rp + base + row * rowb)
                    for (e in range64(kvd)) {
                        maxd = max(maxd, abs(f16_to_f32(uint(hb[e])) - f16_to_f32(uint(hr[e]))))
                    }
                } elif (dt == KVDtype.q8_0) {
                    let qb = bp + base + row * rowb
                    let qr = rp + base + row * rowb
                    for (bi in range64(kvd / 32l)) {
                        let bb = bi * 34l
                        let db = f16_to_f32(uint(qb[bb]) | (uint(qb[bb + 1l]) << 8u))
                        let dr = f16_to_f32(uint(qr[bb]) | (uint(qr[bb + 1l]) << 8u))
                        for (k in range64(32l)) {
                            maxd = max(maxd, abs(float(int8(qb[bb + 2l + k])) * db - float(int8(qr[bb + 2l + k])) * dr))
                        }
                    }
                } elif (dt == KVDtype.tq4) {
                    let qb = bp + base + row * rowb
                    let qr = rp + base + row * rowb
                    for (bi in range64(kvd / 32l)) {
                        let bb = bi * 18l
                        let db = f16_to_f32(uint(qb[bb]) | (uint(qb[bb + 1l]) << 8u))
                        let dr = f16_to_f32(uint(qr[bb]) | (uint(qr[bb + 1l]) << 8u))
                        for (k in range64(16l)) {
                            let byb = int(qb[bb + 2l + k])
                            let byr = int(qr[bb + 2l + k])
                            maxd = max(maxd, abs(float((byb & 15) - 8) * db - float((byr & 15) - 8) * dr))
                            maxd = max(maxd, abs(float((byb >> 4) - 8) * db - float((byr >> 4) - 8) * dr))
                        }
                    }
                } else {
                    let fb = reinterpret<float const?>(bp + base + row * rowb)
                    let fr = reinterpret<float const?>(rp + base + row * rowb)
                    for (e in range64(kvd)) {
                        maxd = max(maxd, abs(fb[e] - fr[e]))
                    }
                }
            }
        }
    }
    return maxd
}

def private cache_feq(t : T?; s : Session; ref : RefRun; c : Config; tol : float; tag : string) {
    t |> equal(s.n_past, ref.npast)
    let kd = kv_blob_max_diff(s.key_cache, ref.kc, s.kv_bprefix_k, s.kv_dtype_k, c, s.n_past)
    let vd = kv_blob_max_diff(s.value_cache, ref.vc, s.kv_bprefix_v, s.kv_dtype_v, c, s.n_past)
    t |> success(kd <= tol, "{tag}: final key cache within {tol} (max diff {kd})")
    t |> success(vd <= tol, "{tag}: final value cache within {tol} (max diff {vd})")
}

// the main harness: 3 ragged prompts, sequential reference vs one batched-decode run
def private batch_vs_ref(t : T?; tr : Model; kdt : KVDtype; nsteps : int64; tol : float; tag : string) {
    let c = tr.config
    let vocab = c.vocab_size
    var p0 <- prompt_a()
    var p1 <- prompt_b()
    var p2 <- prompt_c()
    var refs <- [ <- run_ref(tr, kdt, p0, nsteps, vocab), <- run_ref(tr, kdt, p1, nsteps, vocab),
                 <- run_ref(tr, kdt, p2, nsteps, vocab)]
    var s0 <- make_run_state(c, kdt, kdt)
    var s1 <- make_run_state(c, kdt, kdt)
    var s2 <- make_run_state(c, kdt, kdt)
    eval_(tr, s0, p0)
    eval_(tr, s1, p1)
    eval_(tr, s2, p2)
    var ws <- create_batch_workspace_(tr)
    var sess <- [unsafe(addr(s0)), unsafe(addr(s1)), unsafe(addr(s2))]
    var cur <- [greedy_top(s0.logits, vocab), greedy_top(s1.logits, vocab), greedy_top(s2.logits, vocab)]
    var tokens_ok = true
    var maxd = 0.0
    for (st in range64(nsteps)) {
        eval_batch_(tr, ws, sess, cur)
        for (i in range64(3l)) {
            let sp = sess[i]
            for (v in range64(vocab)) {
                maxd = max(maxd, abs(sp.logits[v] - refs[i].lg[st * vocab + v]))
            }
            cur[i] = greedy_top(sp.logits, vocab)
            if (cur[i] != refs[i].toks[st]) {
                tokens_ok = false
            }
        }
    }
    t |> success(tokens_ok, "{tag}: batched greedy tokens match sequential token-for-token")
    t |> success(maxd <= tol, "{tag}: per-step logits within {tol} (max diff {maxd})")
    cache_feq(t, s0, refs[0], c, tol, "{tag} row0")
    cache_feq(t, s1, refs[1], c, tol, "{tag} row1")
    cache_feq(t, s2, refs[2], c, tol, "{tag} row2")
    sess |> clear()   // drop the borrowed handles: delete of array<T?> cascades to the pointees
    unsafe {
        delete ws
        delete sess
    }
    delete cur
    delete refs
    delete s0
    delete s1
    delete s2
    delete p0
    delete p1
    delete p2
}

def private model_missing(t : T?) : bool {
    if (!jit_enabled()) {
        t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
        return true
    }
    if (!stat(MODEL_PATH).is_valid) {
        t |> skip("stories15M.bin not present")
        return true
    }
    return false
}

[test]
def test_batch_decode_fp32(t : T?) {
    t |> run("batched decode == sequential, fp32 weights + f32 KV (needs stories15M.bin)") @(t : T?) {
        if (model_missing(t)) {
            return
        }
        var tr <- load_checkpoint(MODEL_PATH)
        let prev_fused = get_fused_decode()
        set_fused_decode(false)
        with_job_que() {
            batch_vs_ref(t, tr, KVDtype.f32, 8l, 1.0e-6, "fp32")
        }
        set_fused_decode(prev_fused)
        delete tr
    }
}

[test]
def test_batch_decode_q8(t : T?) {
    t |> run("batched decode == sequential, Q8 weights (needs stories15M.bin)") @(t : T?) {
        if (model_missing(t)) {
            return
        }
        // strict parity needs same-dot kernels on both paths: portable's batch and GEMV kernels
        // share dot_q8q8, so batch == sequential holds at 1e-6 under any tune stamp; a tiled
        // backend's batch GEMM accumulates in a different order (covered by test_batch_decode_tuned)
        pin_kernel_backend("portable")
        var tr <- load_checkpoint(MODEL_PATH)
        quantize_weights(tr)  // all 2D weights -> Q8_0, tr.quant = q8
        let prev_fused = get_fused_decode()
        set_fused_decode(false)
        with_job_que() {
            batch_vs_ref(t, tr, KVDtype.f32, 8l, 1.0e-6, "q8")
        }
        set_fused_decode(prev_fused)
        clear_kernel_backend_pin()
        delete tr
    }
}

[test]
def test_batch_decode_f16_kv(t : T?) {
    t |> run("batched decode == sequential under the f16 KV codec (needs stories15M.bin)") @(t : T?) {
        if (model_missing(t)) {
            return
        }
        pin_kernel_backend("portable")   // same-dot parity — see test_batch_decode_q8
        var tr <- load_checkpoint(MODEL_PATH)
        quantize_weights(tr)
        let prev_fused = get_fused_decode()
        set_fused_decode(false)
        with_job_que() {
            batch_vs_ref(t, tr, KVDtype.f16, 8l, 1.0e-6, "f16kv")
        }
        set_fused_decode(prev_fused)
        clear_kernel_backend_pin()
        delete tr
    }
}

[test]
def test_batch_decode_q8_kv(t : T?) {
    t |> run("batched decode == sequential under the q8_0 KV codec (needs SmolLM2-135M)") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        // stories15M's head_size 48 is rejected by the q8_0 32-block check (by design) — this arm
        // runs the SmolLM2 GGUF carrier instead, same rail as test_fused_decode
        let env = get_env_variable("DASLLAMA_MODELS_DIR")
        let mdir = empty(env) ? "/Users/borisbatkin/Work/llama.cpp/models/" : env
        let path = path_join(mdir, "SmolLM2-135M-Instruct-Q8_0.gguf")
        if (!stat(path).is_valid) {
            t |> skip("{path} not present")
            return
        }
        var tr <- load_gguf(path, QuantMode.q8)
        tr.config.seq_len = min(tr.config.seq_len, 64l)
        let prev_fused = get_fused_decode()
        set_fused_decode(false)
        with_job_que() {
            batch_vs_ref(t, tr, KVDtype.q8_0, 8l, 1.0e-6, "q8_0kv")
        }
        set_fused_decode(prev_fused)
        delete tr
    }
}

[test]
def test_batch_decode_tq4_kv(t : T?) {
    t |> run("batched decode == sequential under the tq4 KV codec (needs SmolLM2-135M)") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        // stories15M's head_size 48 fails the tq4 pow2 check (by design) — SmolLM2 (head 64) carrier,
        // same rail as test_batch_decode_q8_kv. Parity, not quality: both paths rotate + quantize
        // through the same kernels, so batch == sequential holds at fp32 tolerance
        let env = get_env_variable("DASLLAMA_MODELS_DIR")
        let mdir = empty(env) ? "/Users/borisbatkin/Work/llama.cpp/models/" : env
        let path = path_join(mdir, "SmolLM2-135M-Instruct-Q8_0.gguf")
        if (!stat(path).is_valid) {
            t |> skip("{path} not present")
            return
        }
        var tr <- load_gguf(path, QuantMode.q8)
        tr.config.seq_len = min(tr.config.seq_len, 64l)
        let prev_fused = get_fused_decode()
        set_fused_decode(false)
        with_job_que() {
            batch_vs_ref(t, tr, KVDtype.tq4, 8l, 1.0e-6, "tq4kv")
        }
        set_fused_decode(prev_fused)
        delete tr
    }
}

[test]
def test_batch_decode_q4_fallback(t : T?) {
    t |> run("q4 weights take the per-row fallback and still match (needs stories15M.bin)") @(t : T?) {
        if (model_missing(t)) {
            return
        }
        var tr <- load_checkpoint(MODEL_PATH)
        quantize_weights_q4(tr)  // all 2D weights -> Q4_0, tr.quant = q4_0 => eval_batch_ per-row path
        let prev_fused = get_fused_decode()
        set_fused_decode(false)
        with_job_que() {
            batch_vs_ref(t, tr, KVDtype.f32, 6l, 0.0, "q4_0")   // fallback IS forward(): bit-exact
        }
        set_fused_decode(prev_fused)
        delete tr
    }
}

[test]
def test_batch_decode_b1_delegation(t : T?) {
    t |> run("B == 1 delegates to forward() bit-exactly (needs stories15M.bin)") @(t : T?) {
        if (model_missing(t)) {
            return
        }
        var tr <- load_checkpoint(MODEL_PATH)
        quantize_weights(tr)
        let c = tr.config
        let vocab = c.vocab_size
        let prev_fused = get_fused_decode()
        set_fused_decode(false)
        with_job_que() {
            var p0 <- prompt_a()
            var ref <- run_ref(tr, KVDtype.f32, p0, 6l, vocab)
            var s <- make_run_state(c)
            eval_(tr, s, p0)
            var ws <- create_batch_workspace_(tr)
            var sess <- [unsafe(addr(s))]
            var cur <- [greedy_top(s.logits, vocab)]
            var maxd = 0.0
            var tokens_ok = true
            for (st in range64(6l)) {
                eval_batch_(tr, ws, sess, cur)
                for (v in range64(vocab)) {
                    maxd = max(maxd, abs(s.logits[v] - ref.lg[st * vocab + v]))
                }
                cur[0] = greedy_top(s.logits, vocab)
                if (cur[0] != ref.toks[st]) {
                    tokens_ok = false
                }
            }
            t |> success(tokens_ok, "B==1: tokens match")
            t |> success(maxd == 0.0, "B==1: logits BIT-exact — the same forward() path (max diff {maxd})")
            cache_feq(t, s, ref, c, 0.0, "B==1")
            sess |> clear()   // drop the borrowed handles: delete of array<T?> cascades to the pointees
            unsafe {
                delete ws
                delete sess
            }
            delete cur
            delete ref
            delete s
            delete p0
        }
        set_fused_decode(prev_fused)
        delete tr
    }
}

[test]
def test_batch_decode_shrink(t : T?) {
    t |> run("mid-run shrink: survivors keep matching after a row retires (needs stories15M.bin)") @(t : T?) {
        if (model_missing(t)) {
            return
        }
        pin_kernel_backend("portable")   // same-dot parity — see test_batch_decode_q8
        var tr <- load_checkpoint(MODEL_PATH)
        quantize_weights(tr)
        let c = tr.config
        let vocab = c.vocab_size
        let nsteps = 6l   // 3 steps at B=3, then the middle row retires, 3 more at B=2
        let prev_fused = get_fused_decode()
        set_fused_decode(false)
        with_job_que() {
            var p0 <- prompt_a()
            var p1 <- prompt_b()
            var p2 <- prompt_c()
            var refs <- [ <- run_ref(tr, KVDtype.f32, p0, nsteps, vocab), <- run_ref(tr, KVDtype.f32, p1, nsteps, vocab),
                         <- run_ref(tr, KVDtype.f32, p2, nsteps, vocab)]
            var s0 <- make_run_state(c)
            var s1 <- make_run_state(c)
            var s2 <- make_run_state(c)
            eval_(tr, s0, p0)
            eval_(tr, s1, p1)
            eval_(tr, s2, p2)
            var ws <- create_batch_workspace_(tr)
            var sess <- [unsafe(addr(s0)), unsafe(addr(s1)), unsafe(addr(s2))]
            var cur <- [greedy_top(s0.logits, vocab), greedy_top(s1.logits, vocab), greedy_top(s2.logits, vocab)]
            var live <- [0l, 1l, 2l]   // live[k] = which ref stream sess[k] follows
            var tokens_ok = true
            var maxd = 0.0
            for (st in range64(nsteps)) {
                eval_batch_(tr, ws, sess, cur)
                for (k in range64(long_length(sess))) {
                    let ri = live[k]
                    let sp = sess[k]
                    for (v in range64(vocab)) {
                        maxd = max(maxd, abs(sp.logits[v] - refs[ri].lg[st * vocab + v]))
                    }
                    cur[k] = greedy_top(sp.logits, vocab)
                    if (cur[k] != refs[ri].toks[st]) {
                        tokens_ok = false
                    }
                }
                if (st == 2l) {   // the middle conversation "finishes" — drop its row
                    sess |> erase(1)
                    cur |> erase(1)
                    live |> erase(1)
                }
            }
            t |> success(tokens_ok, "shrink: survivor tokens match their reference streams")
            t |> success(maxd <= 1.0e-6, "shrink: survivor logits within 1e-6 (max diff {maxd})")
            cache_feq(t, s0, refs[0], c, 1.0e-6, "shrink row0")
            cache_feq(t, s2, refs[2], c, 1.0e-6, "shrink row2")
            sess |> clear()   // drop the borrowed handles: delete of array<T?> cascades to the pointees
            unsafe {
                delete ws
                delete sess
            }
            delete cur
            delete live
            delete refs
            delete s0
            delete s1
            delete s2
            delete p0
            delete p1
            delete p2
        }
        set_fused_decode(prev_fused)
        clear_kernel_backend_pin()
        delete tr
    }
}

[test]
def test_batch_decode_tuned(t : T?) {
    t |> run("active-backend batch kernels: tokens match sequential within the FP budget (needs stories15M.bin)") @(t : T?) {
        if (model_missing(t)) {
            return
        }
        // no pin: the load picks the box's best backend, whose batch GEMM tiles may accumulate in
        // a different order than the GEMV path — tokens must still match, numerics within budget
        var tr <- load_checkpoint(MODEL_PATH)
        quantize_weights(tr)
        let prev_fused = get_fused_decode()
        set_fused_decode(false)
        with_job_que() {
            batch_vs_ref(t, tr, KVDtype.f32, 8l, 1.0e-3, "tuned")
        }
        set_fused_decode(prev_fused)
        delete tr
    }
}
