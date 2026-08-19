options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration

[init]
def private allow_cpu_prefill_intent() {
    allow_cpu_prefill()   // CPU-correctness test — prefills on the CPU by design
}
require daslib/jobque_boost
require daslib/fio
require math
require dasllama/dasllama_math   // setup_dasllama_jobque_ + set_prefill_attn_mode
require dasllama/dasllama_env

// The non-causal image span (eval_embd_/forward_prefill_embd non_causal=true): every query of
// the span attends [the sliding-window floor, span_end) — the mtmd_decode_use_non_causal semantics gemma
// vision decodes under. The reference here is the MASK DIRECTION itself: under causal
// attention position 0's residual cannot depend on later rows; under the span it must.
// Needs llama2.c's stories15M.bin (test_flash's fixture); skips cleanly when absent.

def private llama2c_dir() : string {
    let env = g_env_test.llama2c_dir
    return empty(env) ? "/Users/borisbatkin/Work/llama2.c" : env
}
let MODEL_PATH = path_join(llama2c_dir(), "stories15M.bin")

let NPOS = 48l

// deterministic pseudo-random embd rows in a tame range; `perturb_last` perturbs ONLY the final row
def private make_embd(dim : int64; perturb_last : int) : array<float> {
    var e : array<float>
    e |> resize(NPOS * dim)
    var state = 0x12345u
    for (i in range64(NPOS * dim)) {
        state = state * 1664525u + 1013904223u
        e[i] = (float(state >> 8u) / float(1 << 24) - 0.5) * 0.1
    }
    if (perturb_last != 0) {
        for (i in range64(dim)) {
            e[(NPOS - 1l) * dim + i] += 0.05 * float(perturb_last)
        }
    }
    return <- e
}

def private row_maxdiff(a, b : array<float>; row, dim : int64) : float {
    var m = 0.0
    for (i in range64(dim)) {
        m = max(m, abs(a[row * dim + i] - b[row * dim + i]))
    }
    return m
}

// one prefill: returns the full residual plane x_b (npos × dim), caller owns
def private span_prefill(t : Model; embd : array<float>; non_causal : bool) : array<float> {
    var s <- make_run_state(t.config)
    forward_prefill_embd(t, s, embd, NPOS, 0l, non_causal)
    var plane : array<float>
    plane |> resize(NPOS * t.config.dim)
    for (i in range64(NPOS * t.config.dim)) {
        plane[i] = s.x_b[i]
    }
    delete s
    return <- plane
}

[test]
def test_attn_span(t : T?) {
    t |> run("the non-causal span: direction, propagation, mode agreement, flag reset") @(tt : T?) {
        if (!stat(MODEL_PATH).is_valid) {
            tt |> skip("stories15M.bin not present at {MODEL_PATH} (set DASLLAMA_LLAMA2C_DIR)")
            return
        }
        var tr <- load_checkpoint(MODEL_PATH)
        let dim = tr.config.dim
        let embd_a <- make_embd(dim, 0)
        let embd_b <- make_embd(dim, 1)   // differs ONLY in the last row
        with_job_que() {
            setup_dasllama_jobque_()
            set_prefill_attn_mode(AttnPrefillMode.classic)
            // the mask direction: causal position 0 is blind to the last row; the span sees it
            let causal_a <- span_prefill(tr, embd_a, false)
            let causal_b <- span_prefill(tr, embd_b, false)
            let span_a <- span_prefill(tr, embd_a, true)
            let span_b <- span_prefill(tr, embd_b, true)
            tt |> equal(row_maxdiff(causal_a, causal_b, 0l, dim), 0.0, "causal: row 0 blind to a last-row perturbation")
            let d0 = row_maxdiff(span_a, span_b, 0l, dim)
            to_log(LOG_INFO, "attn_span: span row-0 sensitivity to last-row perturbation = {d0}\n")
            tt |> success(d0 > 1e-4, "span: row 0 sees the last row (d0 = {d0})")
            // the span rewrites the whole plane, not just row 0
            let dlast = row_maxdiff(causal_a, span_a, NPOS - 1l, dim)
            tt |> success(dlast > 1e-4, "span vs causal differ at the last row too ({dlast})")
            // three independent kernel implementations agree on the span semantics
            set_prefill_attn_mode(AttnPrefillMode.blocked)
            let span_blocked <- span_prefill(tr, embd_a, true)
            set_prefill_attn_mode(AttnPrefillMode.flash)
            let span_flash <- span_prefill(tr, embd_a, true)
            set_prefill_attn_mode(AttnPrefillMode.classic)
            var mode_diff = 0.0
            for (r in range64(NPOS)) {
                mode_diff = max(mode_diff, row_maxdiff(span_a, span_blocked, r, dim))
            }
            tt |> success(mode_diff < 1e-3, "blocked agrees with classic on the span ({mode_diff})")
            var flash_diff = 0.0
            for (r in range64(NPOS)) {
                flash_diff = max(flash_diff, row_maxdiff(span_a, span_flash, r, dim))
            }
            tt |> success(flash_diff < 1e-2, "flash agrees with classic on the span ({flash_diff})")
            // the flag clears: a causal prefill AFTER a span prefill is bit-identical to fresh causal
            var s2 <- make_run_state(tr.config)
            forward_prefill_embd(tr, s2, embd_a, NPOS, 0l, true)
            tt |> equal(s2.attn_uniform_end, 0l, "attn_uniform_end cleared after the span call")
            delete s2
            let causal_after <- span_prefill(tr, embd_a, false)
            var reset_diff = 0.0
            for (r in range64(NPOS)) {
                reset_diff = max(reset_diff, row_maxdiff(causal_a, causal_after, r, dim))
            }
            tt |> equal(reset_diff, 0.0, "causal after span == fresh causal, bit-exact")
        }
        delete tr
    }
}
