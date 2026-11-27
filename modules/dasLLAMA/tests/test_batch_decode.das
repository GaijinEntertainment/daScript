options gen2
options persistent_heap   // several by-value Sessions + a BatchWorkspace per subtest; explicit deletes below
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration
require dasllama/dasllama_math   // the runtime knob setters (engine-level, not re-exported)
require dasllama/dasllama_load   // load_model_: the engine load the batched row's scheduler streams need
require daslib/jobque_boost
require daslib/fio
require math
require _model_tier   // model_available: the stocked-carrier gate (shards, the large tier)
require _override_probes   // decode_declines: the fake override's step
require _batch_decode_cells   // batch_vs_ref, run_ref, cache_feq, the prompts: the harness
require _fused_decode_cells   // fused_attn_chain_cell

// Batched decode vs sequential: eval_batch_ must reproduce the per-session forward() stream —
// token-for-token greedy, per-step logits, and the final KV caches. Needs llama2.c's
// stories15M.bin (gitignored); skips cleanly when absent, same contract as test_forward.
// The reference pins the fused decode chain OFF (the std path is what the batch worker mirrors).
// The SmolLM2 and deltanet hybrid cells (`_batch_decode_cells.das`) run on their models' blocks,
// test_gpu_resident_llama.das, test_gpu_resident_hybrid.das and test_gpu_resident_qwen_moe.das.
let MODEL_PATH = path_join(llama2c_dir(), "stories15M.bin")

def private model_missing(t : T?) : bool {
    return true if (!jit_only(t))
    if (!stat(MODEL_PATH).is_valid) {
        t |> skip("stories15M.bin not present")
        return true
    }
    return false
}

[test(description = "eval_batch_ over three ragged stories15M prompts with fp32 weights and f32 KV reproduces each sequential stream logits and final caches within 1e-6, while another row reference lands outside the bar")]
def test_batch_decode_fp32(t : T?) {
    t |> run("batched decode == sequential, fp32 weights + f32 KV (needs stories15M.bin)") @(t : T?) {
        if (model_missing(t)) {
            return
        }
        var tr <- load_checkpoint(MODEL_PATH)
        with_unfused_jobque() {
            batch_vs_ref(t, tr, KVDtype.f32, 8l, 1.0e-6, "fp32")
        }
        delete tr
    }
}

[test(description = "With Q4_0 weights every batched step runs on the CPU batched stack and reproduces the sequential greedy tokens, logits and caches bit-exact, since the batched GEMM is the GEMV dot over rows")]
def test_batch_decode_q4(t : T?) {
    t |> run("batched decode == sequential, Q4_0 weights on the CPU batched stack (needs stories15M.bin)") @(t : T?) {
        if (model_missing(t)) {
            return
        }
        var tr <- load_checkpoint(MODEL_PATH)
        quantize_weights_q4(tr)  // all 2D weights -> Q4_0, tr.quant = q4_0
        //! one code path: the batched Q4_0 GEMM is the GEMV's dot_q4 run over the rows, so the bar is exact
        with_unfused_jobque() {
            let c0 = batch_step_census()
            batch_vs_ref(t, tr, KVDtype.f32, 6l, 0.0, "q4_0")
            let c1 = batch_step_census()
            t |> equal(c1.cpu_stack - c0.cpu_stack, 6l, "q4_0: every step ran on the CPU batched stack")
            t |> equal(c1.per_row - c0.per_row, 0l, "q4_0: ... none of them a row at a time")
        }
        delete tr
    }
}

//! batch_step_census counts each two-row eval_batch_ step on the CPU batched stack with none per-row or on a device, and a one-row step as per-row only
def test_batch_decode_step_census(t : T?; tr : Model) {
    unsafe {
        t |> run("the step census: a B-row step counts on the CPU batched stack, a one-row step as per-row (needs stories15M.bin)") @capture(& tr)(t : T ?) {
            let c = tr.config
            let vocab = c.vocab_size
            with_unfused_jobque() {
                var pa <- prompt_a()
                var pb <- prompt_b()
                var sa <- make_run_state(c)
                var sb <- make_run_state(c)
                eval_(tr, sa, pa)
                eval_(tr, sb, pb)
                var ws <- create_batch_workspace_(tr)
                var both <- [unsafe(addr(sa)), unsafe(addr(sb))]
                var one <- [unsafe(addr(sa))]
                let toks <- [argmax_of(sa.logits, vocab), argmax_of(sb.logits, vocab)]
                let tok <- [argmax_of(sa.logits, vocab)]
                let c0 = batch_step_census()
                eval_batch_(tr, ws, both, toks)
                eval_batch_(tr, ws, both, toks)
                let c1 = batch_step_census()
                t |> equal(c1.cpu_stack - c0.cpu_stack, 2l, "two B=2 steps ran on the CPU batched stack")
                t |> equal(c1.per_row - c0.per_row, 0l, "... none of them a row at a time")
                t |> equal(c1.device - c0.device, 0l, "... none on a device driver")
                eval_batch_(tr, ws, one, tok)
                let c2 = batch_step_census()
                t |> equal(c2.per_row - c1.per_row, 1l, "a B=1 step counts as per-row")
                t |> equal(c2.cpu_stack - c1.cpu_stack, 0l, "... not on the batched stack")
                delete ws
                delete sa
                delete sb
            }
        }
    }
}

//! A one-row eval_batch_ delegates to forward, so its six greedy tokens, logits and final caches match the sequential stream bit for bit
def test_batch_decode_b1_delegation(t : T?; tr : Model) {
    unsafe {
        t |> run("B == 1 delegates to forward() bit-exactly (needs stories15M.bin)") @capture(& tr)(t : T ?) {
            let c = tr.config
            let vocab = c.vocab_size
            with_unfused_jobque() {
                var p0 <- prompt_a()
                var ref <- run_ref(tr, KVDtype.f32, p0, 6l, vocab)
                var s <- make_run_state(c)
                eval_(tr, s, p0)
                var ws <- create_batch_workspace_(tr)
                var sess <- [unsafe(addr(s))]
                var cur <- [argmax_of(s.logits, vocab)]
                var maxd = 0.0
                var tokens_ok = true
                for (st in range64(6l)) {
                    eval_batch_(tr, ws, sess, cur)
                    for (v in range64(vocab)) {
                        maxd = max(maxd, nan_wide_diff(s.logits[v], ref.lg[st * vocab + v]))
                    }
                    cur[0] = argmax_of(s.logits, vocab)
                    if (cur[0] != ref.toks[st]) {
                        tokens_ok = false
                    }
                }
                t |> success(tokens_ok, "B==1: tokens match")
                t |> success(maxd == 0.0, "B==1: logits BIT-exact - the same forward() path (max diff {maxd})")
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
        }
    }
}

//! After the middle row retires mid-run, the two surviving batched rows keep their sequential greedy tokens, logits within 1e-6 and final caches within 1e-6
def test_batch_decode_shrink(t : T?; tr : Model) {
    unsafe {
        t |> run("mid-run shrink: survivors keep matching after a row retires (needs stories15M.bin)") @capture(& tr)(t : T ?) {
            let c = tr.config
            let vocab = c.vocab_size
            let nsteps = 6l   // 3 steps at B=3, then the middle row retires, 3 more at B=2
            with_unfused_jobque() {
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
                var cur <- [argmax_of(s0.logits, vocab), argmax_of(s1.logits, vocab), argmax_of(s2.logits, vocab)]
                var live <- [0l, 1l, 2l]   // live[k] = which ref stream sess[k] follows
                var tokens_ok = true
                var maxd = 0.0
                for (st in range64(nsteps)) {
                    eval_batch_(tr, ws, sess, cur)
                    for (k in range64(long_length(sess))) {
                        let ri = live[k]
                        let sp = sess[k]
                        for (v in range64(vocab)) {
                            maxd = max(maxd, nan_wide_diff(sp.logits[v], refs[ri].lg[st * vocab + v]))
                        }
                        cur[k] = argmax_of(sp.logits, vocab)
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
        }
    }
}

let private DECLINE_PROBE = "batch-decline-probe"   //! a fake decode override whose batched step declines every call

var private g_probe_batch_calls = 0l
var private g_probe_gather = false   //! the probe's embed gate answer: the step gathers its rows itself

[unused_argument(t, ws, sessions, nrows)]
def private probe_batch_decline(t : Model; var ws : BatchWorkspace; var sessions : array<Session?>; nrows : int64) : bool {
    g_probe_batch_calls++
    return false
}

[unused_argument(t, s, npos, start_pos)]
def private probe_gather_gate(t : Model; s : Session; npos, start_pos : int64) : bool => g_probe_gather

//! the probe override registered and selected for `body`, then the selection put back as found and the probe unregistered
def private with_decline_probe(body : block<() : void>) {
    let prev = active_decode_override()
    register_decode_override(DECLINE_PROBE, @@decode_declines)
    register_batch_decode_override(DECLINE_PROBE, @@probe_batch_decline)
    register_embed_batch_gate(DECLINE_PROBE, @@probe_gather_gate)
    select_decode_override(DECLINE_PROBE)
    invoke(body)
    select_decode_override(prev)
    unregister_decode_override(DECLINE_PROBE)
}

//! two sessions over prompts a and b, then `steps` batched greedy steps: the last step's logits, both rows
def private two_row_steps(tr : Model; steps : int64) : array<float> {
    let c = tr.config
    let vocab = c.vocab_size
    var pa <- prompt_a()
    var pb <- prompt_b()
    var sa <- make_run_state(c)
    var sb <- make_run_state(c)
    eval_(tr, sa, pa)
    eval_(tr, sb, pb)
    var ws <- create_batch_workspace_(tr)
    var both <- [unsafe(addr(sa)), unsafe(addr(sb))]
    var toks <- [argmax_of(sa.logits, vocab), argmax_of(sb.logits, vocab)]
    for (_st in range64(steps)) {
        eval_batch_(tr, ws, both, toks)
        toks[0] = argmax_of(sa.logits, vocab)
        toks[1] = argmax_of(sb.logits, vocab)
    }
    var out <- [for (i in range64(2l * vocab)); i < vocab ? sa.logits[i] : sb.logits[i - vocab]]
    both |> clear()   // borrowed handles
    unsafe {
        delete both
    }
    delete toks
    delete ws
    delete sa
    delete sb
    delete pa
    delete pb
    return <- out
}

//! one `verify_batch_groups` call over `groups` under the probe: it serves nothing, and reaches the override only where `reached`
def private verify_decline(t : T?; tr : Model; var ws : BatchWorkspace; var groups : array<Session?>; pos, nr : array<int64>;
                           reached : bool; what : string) {
    var toks <- [5l, 6l, 7l, 8l]
    let calls0 = g_probe_batch_calls
    t |> success(!verify_batch_groups(tr, ws, groups, toks, pos, nr), "{what}: no verify served")
    t |> equal(g_probe_batch_calls - calls0, reached ? 1l : 0l,
        reached ? "{what}: the groups reach the override - the control" : "{what}: declined before the override")
    groups |> clear()   // borrowed handles
    delete toks
}

//! A batch override that declines every step, with its gather gate on or off, leaves the host embedding each row once and the CPU stack reading the plain batched step logits bit for bit
def test_batch_decode_late_embed(t : T?; tr : Model) {
    unsafe {
        t |> run("a batch override that declines after its gather gate took the rows: the rows embed late, the CPU stack reads the plain step's logits (needs stories15M.bin)") @capture(& tr)(t : T ?) {
            with_unfused_jobque() {
                var plain <- two_row_steps(tr, 3l)
                with_decline_probe() {
                    for (gather in [true, false]) {
                        g_probe_gather = gather
                        let calls0 = g_probe_batch_calls
                        let emb0 = batch_rows_host_embedded()
                        var got <- two_row_steps(tr, 3l)
                        t |> equal(g_probe_batch_calls - calls0, 3l, "gate {gather}: the override was asked every step and declined")
                        t |> equal(batch_rows_host_embedded() - emb0, 6l, "gate {gather}: the host embedded every row once - late where the gate took them")
                        t |> equal(logits_maxdiff(got, plain), 0.0, "gate {gather}: the CPU stack after the decline reads the plain batched step's logits bit for bit")
                        delete got
                    }
                }
                g_probe_gather = false
                delete plain
            }
        }
    }
}

//! verify_batch_groups declines an empty group, a group past the context, mixed K/V codecs and a flat beside a paged group before reaching the override, while two valid groups reach it
def test_batch_decode_verify_declines(t : T?; tr : Model) {
    unsafe {
        t |> run("verify_batch_groups declines a group of no rows, a group past the context and groups on two codecs or pools before the override (needs stories15M.bin)") @capture(& tr)(t : T ?) {
            let c = tr.config
            with_unfused_jobque() {
                var pa <- prompt_a()
                var pb <- prompt_b()
                var sa <- make_run_state(c)
                var sb <- make_run_state(c)
                var s16 <- make_run_state(c, KVDtype.f16, KVDtype.f16)
                var pool <- create_kv_pool_(c, 16l, KVDtype.f32, KVDtype.f32)
                var sp <- make_run_state_paged(c, pool)
                eval_(tr, sa, pa)
                eval_(tr, sb, pb)
                eval_(tr, s16, pb)
                eval_(tr, sp, pb)
                var ws <- create_batch_workspace_(tr)
                with_decline_probe() {
                    g_probe_gather = false
                    var g1 <- [unsafe(addr(sa)), unsafe(addr(sb))]
                    verify_decline(t, tr, ws, g1, [sa.n_past, sb.n_past], [2l, 2l], true, "two groups of two rows each")
                    var g2 <- [unsafe(addr(sa)), unsafe(addr(sb))]
                    verify_decline(t, tr, ws, g2, [sa.n_past, sb.n_past], [2l, 0l], false, "a group of no rows")
                    var g3 <- [unsafe(addr(sa)), unsafe(addr(sb))]
                    verify_decline(t, tr, ws, g3, [sa.n_past, c.seq_len - 1l], [2l, 2l], false, "a group past the context")
                    var g4 <- [unsafe(addr(sa)), unsafe(addr(s16))]
                    verify_decline(t, tr, ws, g4, [sa.n_past, s16.n_past], [2l, 2l], false, "groups on two K/V codecs")
                    var g5 <- [unsafe(addr(sa)), unsafe(addr(sp))]
                    verify_decline(t, tr, ws, g5, [sa.n_past, sp.n_past], [2l, 2l], false, "a flat group beside a paged one")
                    unsafe {
                        delete g1
                        delete g2
                        delete g3
                        delete g4
                        delete g5
                    }
                }
                delete ws
                release_kv_pages_(sp)
                delete sp
                delete pool
                delete s16
                delete sa
                delete sb
                delete pa
                delete pb
            }
        }
    }
}

// ===== the forms: each model file loads once a form, its cells in turn on that load =====

[test(description = "stories15M on one load quantized to Q8_0 on the portable backend runs the Q8 weights, f16 KV codec and mid-run shrink cells in turn, each on its own sessions")]
def test_batch_decode_portable_cells(t : T?) {
    //! strict parity needs same-dot kernels on both paths: portable's batch and GEMV kernels share dot_q8q8, so batch ==
    //! sequential holds at 1e-6 under any tune stamp (a tiled backend's batch GEMM accumulates in another order: the tuned cell)
    with_stories15m(t, "portable") $(var tr) {
        batch_vs_ref_cell(t, tr, "batched decode == sequential, Q8 weights (needs stories15M.bin)", KVDtype.f32, 1.0e-6, "q8")
        batch_vs_ref_cell(t, tr, "batched decode == sequential under the f16 KV codec (needs stories15M.bin)", KVDtype.f16, 1.0e-6, "f16kv")
        test_batch_decode_shrink(t, tr)
    }
}

[test(description = "stories15M on one load quantized to Q8_0 on the box's own backend runs the step census, B=1 delegation, tuned-kernel, late-embed, verify-decline and fused-chain cells in turn, each on its own sessions")]
def test_batch_decode_q8_cells(t : T?) {
    with_stories15m(t) $(var tr) {
        test_batch_decode_step_census(t, tr)
        test_batch_decode_b1_delegation(t, tr)
        //! no pin: the load picks the box's best backend, whose batch GEMM tiles may accumulate in another order than the GEMV
        //! path - tokens must still match, numerics within budget
        batch_vs_ref_cell(t, tr, "active-backend batch kernels: tokens match sequential within the FP budget (needs stories15M.bin)", KVDtype.f32, 1.0e-3, "tuned")
        test_batch_decode_late_embed(t, tr)
        test_batch_decode_verify_declines(t, tr)
        fused_attn_chain_cell(t, tr, "fused chains == per-op dispatches, bit for bit")
    }
}

