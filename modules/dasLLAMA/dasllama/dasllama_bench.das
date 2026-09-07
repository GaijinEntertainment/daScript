options gen2
options indenting = 4
options _dasllama_internal = true

module dasllama_bench shared public

require dasllama/dasllama_lint public
require dasllama/dasllama_common   // Model, Session, forward_prefill, forward, piece_
require math

// The llama-bench protocol, one rep at a time: an untimed full-size warmup, then timed reps of
// either row - pp: one batched prefill of N synthetic ids from an empty cache; tg: N single-token
// forwards at pos 0.. with no logit read (llama-bench never samples). lcpp_bench drives the rows
// from its own loop; dasllama-server runs one rep per tick so its HTTP stays live between them.

let public BENCH_PP_TOKENS = 512l   // llama-bench -p
let public BENCH_TG_TOKENS = 128l   // llama-bench -n
let public BENCH_WARM_SEED = 999l   // the warmup's prompt seed; the timed reps take 0, 1, 2, ...

def public bench_synth_id(i, seed : int64) : int64 => 1000l + ((i * 17l + seed * 131l) % 5000l)

def public bench_prompt(n, seed : int64) : array<int64> {
    return <- [for (i in range64(n)); bench_synth_id(i, seed)]
}

struct public BenchRow {
    test : string
    mean : double
    sd : double
}

//! mean and sample standard deviation of the per-rep tok/s, under the row's tag
def public bench_row(tag : string; tps : array<double>) : BenchRow {
    var sum = 0.0lf
    for (v in tps) {
        sum += v
    }
    let n = length(tps)
    let mean = n > 0 ? sum / double(n) : 0.0lf
    var vsum = 0.0lf
    for (v in tps) {
        vsum += (v - mean) * (v - mean)
    }
    let sd = n > 1 ? double(sqrt(float(vsum / double(n - 1)))) : 0.0lf
    return BenchRow(test = tag, mean = mean, sd = sd)
}

//! The output sanity check a row's warmup pays before its timed reps read no output at all: the
//! logits must be finite with a non-degenerate argmax, or the run refuses rather than post
//! clean-looking numbers from a broken pipeline. `ok` false carries the reason in `detail`;
//! true carries the argmax piece and its logit.
def public bench_warm_sanity(t : Model; s : Session) : tuple<ok : bool; detail : string> {
    let vocab = t.config.vocab_size
    var bi = 0l
    var bv = -3.0e38
    var mn = 3.0e38
    var bad = 0l
    for (v in range64(vocab)) {
        let x = s.logits[v]
        if (is_nan(x) || abs(x) > 3.0e38) {
            bad++
        } else {
            if (x > bv) {
                bv = x
                bi = v
            }
            if (x < mn) {
                mn = x
            }
        }
    }
    if (bad > 0l || bv <= mn) {
        return (ok = false, detail = "warmup logits unusable - {bad} non-finite of {vocab}, max {bv} min {mn}")
    }
    return (ok = true, detail = "warmup argmax {bi} \"{piece_(t, bi)}\" (logit {bv})")
}

//! a fresh session for the rows: an f16 KV cache over the model's whole context, llama-bench's
//! default, so a server's rows and lcpp_bench's read the same cache
def public bench_session(t : Model) : Session {
    return <- make_run_state(t.config, KVDtype.f16, KVDtype.f16)
}

//! the pp row's untimed warmup: one full-size prefill, as llama-bench does before timing
def public bench_pp_warm(t : Model; var s : Session; plen : int64) {
    let warm <- bench_prompt(plen, BENCH_WARM_SEED)
    forward_prefill(t, s, warm, plen, 0l)
}

//! one timed pp rep: `plen` synthetic ids prefilled from an empty cache; returns tok/s
def public bench_pp_rep(t : Model; var s : Session; plen, seed : int64) : double {
    let prompt <- bench_prompt(plen, seed)
    let t0 = ref_time_ticks()
    forward_prefill(t, s, prompt, plen, 0l)
    let us = get_time_usec(t0)
    return double(plen) * 1.0e6lf / double(us)
}

//! the tg row's untimed warmup: one token, llama-bench's test_gen(ctx, 1)
def public bench_tg_warm(t : Model; var s : Session) {
    forward(t, s, bench_synth_id(0l, BENCH_WARM_SEED), 0l)
}

//! one timed tg rep: `ngen` single-token forwards at pos 0.., feeding synthetic ids; returns tok/s
def public bench_tg_rep(t : Model; var s : Session; ngen, seed : int64) : double {
    let t0 = ref_time_ticks()
    for (i in range64(ngen)) {
        forward(t, s, bench_synth_id(i, seed), i)
    }
    let us = get_time_usec(t0)
    return double(ngen) * 1.0e6lf / double(us)
}
