options gen2
options _dasllama_internal = true

module _batch_decode_cells shared public

//! the batched-decode cells against the per-session sequential forward, on a model its caller loaded; no entry loads one

require dastest/testing_boost public
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration
require dasllama/dasllama_math   // with_dasllama_jobque_ + the runtime knob setters (engine-level, not re-exported)
require dasllama/dasllama_bench   // bench_tg_batched_rep: the batched row's refusal on a per-row step
require daslib/f16_cvt
require daslib/jobque_boost
require math
require _model_tier   // cut_within, logits_maxdiff, nan_wide_diff
require _cpu_gpu_model   // with_ctx_cut: the cells' 64-position context

// Batched decode vs sequential: eval_batch_ must reproduce the per-session forward() stream —
// token-for-token greedy, per-step logits, and the final KV caches (`batch_vs_ref`, the harness
// `test_batch_decode.das`'s stories15M cells run too). The reference pins the fused decode chain
// OFF (the std path is what the batch worker mirrors). The cells here take a loaded model: the
// q8_0 and tq4 KV codec cells and the batched bench row's refusal on SmolLM2-135M, the deltanet
// hybrid's cells on Qwen3.5-0.8B and on the MoE hybrid's four-block cut.

// ragged prompts (raw stories15M token ids, BOS-first — no tokenizer.bin dependency)
def prompt_a() : array<int64> => [1l, 9038l, 2501l, 263l, 931l]
def prompt_b() : array<int64> => [1l, 306l, 763l]
def prompt_c() : array<int64> => [1l, 3439l, 278l, 1781l, 367l, 263l, 2217l]

// one session's sequential greedy stream: continuation tokens, the logits after every step, and
// clones of the final caches — the oracle a batched twin must reproduce
struct RefRun {
    toks : array<int64>   // greedy continuation token per step
    lg : array<float>     // logits after each step (nsteps x vocab)
    kc : array<uint8>     // final key_cache
    vc : array<uint8>     // final value_cache
    dn : array<float>     // final deltanet state (a hybrid), else empty
    conv : array<float>   // final deltanet conv history (a hybrid), else empty
    npast : int64
}

def run_ref(tr : Model; kdt : KVDtype; prompt : array<int64>; nsteps, vocab : int64) : RefRun {
    var s <- make_run_state(tr.config, kdt, kdt)
    eval_(tr, s, prompt)
    var r : RefRun
    r.lg |> reserve(nsteps * vocab)
    r.toks |> reserve(nsteps)
    var cur = argmax_of(s.logits, vocab)
    for (_st in range64(nsteps)) {
        eval_(tr, s, [cur])
        r.lg |> push_from(s.logits)
        cur = argmax_of(s.logits, vocab)
        r.toks |> push(cur)
    }
    r.kc := s.key_cache
    r.vc := s.value_cache
    r.dn := s.dn_state
    r.conv := s.dn_conv_state
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
                        maxd = max(maxd, nan_wide_diff(f16_to_f32(uint(hb[e])), f16_to_f32(uint(hr[e]))))
                    }
                } elif (dt == KVDtype.q8_0) {
                    let qb = bp + base + row * rowb
                    let qr = rp + base + row * rowb
                    for (bi in range64(kvd / 32l)) {
                        let bb = bi * 34l
                        let db = f16_to_f32(uint(qb[bb]) | (uint(qb[bb + 1l]) << 8u))
                        let dr = f16_to_f32(uint(qr[bb]) | (uint(qr[bb + 1l]) << 8u))
                        for (k in range64(32l)) {
                            maxd = max(maxd, nan_wide_diff(float(int8(qb[bb + 2l + k])) * db, float(int8(qr[bb + 2l + k])) * dr))
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
                            maxd = max(maxd, nan_wide_diff(float((byb & 15) - 8) * db, float((byr & 15) - 8) * dr))
                            maxd = max(maxd, nan_wide_diff(float((byb >> 4) - 8) * db, float((byr >> 4) - 8) * dr))
                        }
                    }
                } else {
                    let fb = reinterpret<float const?>(bp + base + row * rowb)
                    let fr = reinterpret<float const?>(rp + base + row * rowb)
                    for (e in range64(kvd)) {
                        maxd = max(maxd, nan_wide_diff(fb[e], fr[e]))
                    }
                }
            }
        }
    }
    return maxd
}

def cache_feq(t : T?; s : Session; ref : RefRun; c : Config; tol : float; tag : string) {
    t |> equal(s.n_past, ref.npast)
    let kd = kv_blob_max_diff(s.key_cache, ref.kc, s.kv_bprefix_k, s.kv_dtype_k, c, s.n_past)
    let vd = kv_blob_max_diff(s.value_cache, ref.vc, s.kv_bprefix_v, s.kv_dtype_v, c, s.n_past)
    t |> success(kd <= tol, "{tag}: final key cache within {tol} (max diff {kd})")
    t |> success(vd <= tol, "{tag}: final value cache within {tol} (max diff {vd})")
    if (!empty(ref.dn)) {
        t |> equal(s.n_past, s.dn_pos, "{tag}: the deltanet position advanced with the rows")
        t |> equal(length(s.dn_state), length(ref.dn), "{tag}: deltanet state length")
        t |> equal(length(s.dn_conv_state), length(ref.conv), "{tag}: conv history length")
        let dnd = logits_maxdiff(s.dn_state, ref.dn)
        let cvd = logits_maxdiff(s.dn_conv_state, ref.conv)
        t |> success(dnd <= tol, "{tag}: final deltanet state within {tol} (max diff {dnd})")
        t |> success(cvd <= tol, "{tag}: final conv history within {tol} (max diff {cvd})")
    }
}

//! `blk` with the fused decode chain pinned off (the std path is what the batch worker mirrors) inside the engine's job queue,
//! the fused knob put back after
def with_unfused_jobque(blk : block<void>) {
    let prev_fused = get_fused_decode()
    set_fused_decode(false)
    with_dasllama_jobque_() {
        invoke(blk)
    }
    set_fused_decode(prev_fused)
}

//! the run `title`: batched decode on `tr` under the `kdt` KV codec against the sequential stream (`batch_vs_ref`, eight steps)
//! within `bar`, `tag` naming its logs, the fused chain pinned off
def batch_vs_ref_cell(t : T?; tr : Model; title : string; kdt : KVDtype; bar : float; tag : string) {
    unsafe {
        t |> run(title) @capture(& tr)(t : T ?) {
            with_unfused_jobque() {
                batch_vs_ref(t, tr, kdt, 8l, bar, tag)
            }
        }
    }
}

// the main harness: 3 ragged prompts, sequential reference vs one batched-decode run
def batch_vs_ref(t : T?; tr : Model; kdt : KVDtype; nsteps : int64; tol : float; tag : string) {
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
    var cur <- [argmax_of(s0.logits, vocab), argmax_of(s1.logits, vocab), argmax_of(s2.logits, vocab)]
    //! an exact bar (0) is one code path, so the batched side free-runs its own greedy and the tokens
    //! must match; a bar above 0 concedes rounding, so the batched side is FED the reference's tokens
    //! and only the logits are held to the bar - a near-tie flip cannot red a correct engine
    let forced = tol > 0.0
    var tokens_ok = true
    var maxd = 0.0
    var got : array<array<int64>>
    got |> resize(3)
    for (g in got) {
        g |> resize(nsteps)
    }
    for (st in range64(nsteps)) {
        eval_batch_(tr, ws, sess, cur)
        for (i in range64(3l)) {
            let sp = sess[i]
            for (v in range64(vocab)) {
                maxd = max(maxd, nan_wide_diff(sp.logits[v], refs[i].lg[st * vocab + v]))
            }
            let top = argmax_of(sp.logits, vocab)
            got[i][st] = top
            if (top != refs[i].toks[st]) {
                tokens_ok = false
            }
            cur[i] = forced ? refs[i].toks[st] : top
        }
    }
    for (i in range64(3l)) {
        to_log(LOG_INFO, "{tag} row{i} reference ids: {refs[i].toks}\n{tag} row{i} batched argmax ids: {got[i]}\n")
    }
    if (!forced) {
        t |> success(tokens_ok, "{tag}: batched greedy tokens match sequential token-for-token")
    }
    t |> success(maxd <= tol, "{tag}: per-step logits within {tol} (max diff {maxd})")
    cache_feq(t, s0, refs[0], c, tol, "{tag} row0")
    cache_feq(t, s1, refs[1], c, tol, "{tag} row1")
    cache_feq(t, s2, refs[2], c, tol, "{tag} row2")
    // the control: row 0 against row 1's reference lands outside the bar - the bar discriminates here
    var xlg = 0.0
    for (v in range64(vocab)) {
        xlg = max(xlg, nan_wide_diff(s0.logits[v], refs[1].lg[(nsteps - 1l) * vocab + v]))
    }
    t |> success(xlg > tol, "{tag}: control - row 0's logits against row 1's reference exceed {tol} ({xlg})")
    if (!empty(refs[1].dn)) {
        let xdn = logits_maxdiff(s0.dn_state, refs[1].dn)
        t |> success(xdn > tol, "{tag}: control - row 0's deltanet state against row 1's reference exceeds {tol} ({xdn})")
    }
    delete got
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

//! the deltanet hybrid: batched == sequential on the CPU stack - the arch's own batched layer,
//! recurrent rows against per-session state, gated partial-rope attention rows; `ba_f32` names
//! the beta/alpha arm the carrier takes (f32 rows on disk, or q8 planes); `blocks` past 0 the
//! trunk blocks a cut's name claims, asserted on the load
def private hybrid_batch_cell(t : T?; tr : Model; kdt : KVDtype; tag : string; ba_f32 : bool; blocks : int64 = 0l) {
    t |> equal(tr.dn_ba_f32, ba_f32, "{tag}: the carrier takes the {ba_f32 ? "f32-on-disk" : "q8-plane"} beta/alpha arm")
    if (blocks > 0l) {
        cut_within(t, "{tag}: ", tr, blocks)
    }
    with_unfused_jobque() {
        let c0 = batch_step_census()
        batch_vs_ref(t, tr, kdt, 8l, 1.0e-6, tag)
        let c1 = batch_step_census()
        t |> equal(c1.cpu_stack - c0.cpu_stack, 8l, "{tag}: every step ran on the CPU batched stack")
        t |> equal(c1.per_row - c0.per_row, 0l, "{tag}: ... none of them a row at a time")
        // the state is forward-only: a step below the deltanet position panics instead of rewinding
        var sr <- make_run_state(tr.config, kdt, kdt)
        let pb <- prompt_b()
        eval_(tr, sr, pb)
        sr.n_past = 1l
        var tripped = false
        try {
            eval_(tr, sr, [1l])
        } recover {
            tripped = true
        }
        t |> success(tripped, "{tag}: a decode below the deltanet position panics - the state is forward-only")
        delete sr
    }
}

//! On Qwen3.5-0.8B, batched decode reproduces sequential on the CPU batched stack under f32 and f16 KV, the q8-plane beta/alpha arm on the load
def batch_decode_hybrid_cells(t : T?; tr : Model) {
    unsafe {
        t |> run("batched decode == sequential on the deltanet hybrid, f32 KV (needs Qwen3.5-0.8B-Q8_0)") @capture(& tr)(t : T ?) {
            hybrid_batch_cell(t, tr, KVDtype.f32, "qwen35", false)
        }
        t |> run("batched decode == sequential on the deltanet hybrid, f16 KV (needs Qwen3.5-0.8B-Q8_0)") @capture(& tr)(t : T ?) {
            hybrid_batch_cell(t, tr, KVDtype.f16, "qwen35 f16kv", false)
        }
    }
}

//! the MoE hybrid's beta/alpha rows come as f32 on disk - the other arm of the batched layer's
//! projection prologue - on the four-block cut: batched == sequential under f16 KV
def batch_decode_hybrid_moe_cell(t : T?; tr : Model) {
    unsafe {
        t |> run("batched decode == sequential on the deltanet hybrid MoE, f32-on-disk beta/alpha, f16 KV (needs Qwen3.6-35B-A3B-4L-UD-Q4_K_M)") @capture(& tr)(t : T ?) {
            hybrid_batch_cell(t, tr, KVDtype.f16, "qwen35moe", true, 4l)
        }
    }
}

//! SmolLM2-135M's q8_0 and tq4 KV codec cells in turn, `m`'s context cut to 64 for them: batched decode reproduces the
//! sequential per-step logits and dequantized final caches within 1e-6, a cross-row control outside the bar
def batch_decode_codec_cells(t : T?; var m : Model) {
    with_ctx_cut(m, 64l) {
        //! stories15M's head_size 48 is rejected by the q8_0 32-block check and fails the tq4 pow2 check (by design): the codec
        //! cells run SmolLM2 (head 64). Parity, not quality: both paths rotate + quantize through the same kernels
        batch_vs_ref_cell(t, m, "batched decode == sequential under the q8_0 KV codec (needs SmolLM2-135M)", KVDtype.q8_0, 1.0e-6, "q8_0kv")
        batch_vs_ref_cell(t, m, "batched decode == sequential under the tq4 KV codec (needs SmolLM2-135M)", KVDtype.tq4, 1.0e-6, "tq4kv")
    }
}

//! The batched bench row serves at a positive rate on the CPU batched stack, and with the classic rope path forcing per-row steps it refuses by name, reads 0 and the census counts the per-row steps; `tr` an engine load (the scheduler's streams detokenize through the model's own tokenizer) with no device state, its context cut to 64
def batch_decode_row_refuses_cell(t : T?; var tr : Model) {
    with_ctx_cut(tr, 64l) {
        unsafe {
            t |> run("the batched bench row refuses by name when a timed step ran its rows one at a time (needs SmolLM2-135M)") @capture(& tr)(t : T ?) {
                let prev_rope = get_rope_table_mode()
                with_unfused_jobque() {
                    let served = bench_tg_batched_rep(tr, 2l, 8l, 4l, 1l)
                    t |> equal(served.why, "", "the CPU batched stack serves the row: {served.why}")
                    t |> success(served.tps > 0.0lf, "... at a rate")
                    set_rope_table_mode(false)   //! the classic rope path has no per-row positions: every step runs a row at a time
                    let census0 = batch_step_census()
                    let refused = bench_tg_batched_rep(tr, 2l, 8l, 4l, 1l)
                    t |> equal(refused.why, "4 of the 4 timed steps ran their rows one at a time", "the row names the per-row steps")
                    t |> equal(refused.tps, 0.0lf, "... and reads 0")
                    let census1 = batch_step_census()
                    t |> success(census1.per_row - census0.per_row >= 4l, "the census counted the per-row steps ({census1.per_row - census0.per_row})")
                }
                set_rope_table_mode(prev_rope)
            }
        }
    }
}
