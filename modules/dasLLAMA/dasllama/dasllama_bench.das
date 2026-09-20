options gen2
options indenting = 4
options _dasllama_internal = true

module dasllama_bench shared public

require dasllama/dasllama_lint public
require dasllama/dasllama_common   // Model, Session, Config, make_run_state, forward_prefill, forward, piece_
require dasllama/dasllama   // gpu_device_sessions: the device homes the batched row's streams
require dasllama/dasllama_scheduler   // the batched row runs the served shape: device-home streams through the scheduler
require dasllama/dasllama_gpu_tier   // gpu_cpu_passes_: a step the driver handed to the CPU refuses the batched row
require math

// The llama-bench protocol, one rep at a time: an untimed full-size warmup, then timed reps of
// either row - pp: one batched prefill of N synthetic ids from an empty cache; tg: N single-token
// forwards at pos 0.. with no logit read (llama-bench never samples). lcpp_bench drives the rows
// from its own loop; dasllama-server runs one pp rep or one tg token per tick so its HTTP stays
// live between them.

let public BENCH_PP_TOKENS = 512l   // llama-bench -p
let public BENCH_TG_TOKENS = 128l   // llama-bench -n
let public BENCH_WARM_SEED = 999l   // the warmup's prompt seed; the timed reps take 0, 1, 2, ...

//! deterministic synthetic ids in the vocab-safe low range (the decode_prof convention): llama-bench
//! feeds std::rand(), and token content does not move the speed, so determinism keeps runs comparable
def public bench_synth_id(i, seed : int64) : int64 => 1000l + ((i * 17l + seed * 131l) % 5000l)

//! the prompt one pp rep prefills: `n` synthetic ids under one seed
def public bench_prompt(n, seed : int64) : array<int64> {
    return <- [for (i in range64(n)); bench_synth_id(i, seed)]
}

struct public BenchRow {
    test : string
    mean : double
    sd : double
}

//! mean and sample standard deviation of the per-rep rates (tok/s, MB/s - whatever the row measures),
//! under the row's tag; an empty rep list reads 0, and a driver that reached it measured nothing
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

//! A flat session for the rows, sized the way llama-bench sizes its context - `plen + ngen`
//! positions, never more than the model serves - under the KV codec the caller names, so a
//! server's rows read the codec its slot serves and lcpp_bench's read f16, llama-bench's default.
//! The size is written into the model's own config, not a copy: the engine keys every KV layer
//! offset on `t.config.seq_len` (`kv_cache_off`), so a session built from a smaller copy is
//! written past its end at the second layer. A caller that serves the model after the rows
//! saves `t.config.seq_len` before this call and restores it when the rows' session is deleted.
//! A flat session has no capacity check on its prefill path: the size here is what keeps the pp
//! row's writes inside the cache, so a caller never asks it for fewer rows than its `plen`.
def public bench_session(var t : Model; plen, ngen : int64; kdt, vdt : KVDtype) : Session {
    t.config.seq_len = min(t.config.seq_len, plen + ngen)
    return <- make_run_state(t.config, kdt, vdt)
}

//! the pp row's untimed warmup: one full-size prefill, as llama-bench does before timing
[cold_path]   // a driver over the annotated forward_* entries; it allocates its prompt per call
def public bench_pp_warm(t : Model; var s : Session; plen : int64) {
    let warm <- bench_prompt(plen, BENCH_WARM_SEED)
    forward_prefill(t, s, warm, plen, 0l)
}

//! one timed pp rep: `plen` synthetic ids prefilled from an empty cache; returns tok/s
[cold_path]
def public bench_pp_rep(t : Model; var s : Session; plen, seed : int64) : double {
    let prompt <- bench_prompt(plen, seed)
    let t0 = ref_time_ticks()
    forward_prefill(t, s, prompt, plen, 0l)
    return double(plen) * 1.0e9lf / double(get_time_nsec(t0))   // the 64-bit clock: a rep on a slow box outlives the 32-bit microsecond one
}

//! the tg row's untimed warmup: one token, llama-bench's test_gen(ctx, 1)
[cold_path]
def public bench_tg_warm(t : Model; var s : Session) {
    forward(t, s, bench_synth_id(0l, BENCH_WARM_SEED), 0l)
}

//! one token of a tg rep - the forward at position `i` under the rep's seed - returning the
//! nanoseconds it took, for a driver that steps the row between other work; a rep's tok/s is
//! `ngen * 1e9 / (the sum over its tokens)`
[cold_path]
def public bench_tg_step(t : Model; var s : Session; i, seed : int64) : int64 {
    let t0 = ref_time_ticks()
    forward(t, s, bench_synth_id(i, seed), i)
    return get_time_nsec(t0)
}

//! one timed tg rep: `ngen` single-token forwards at pos 0.., feeding synthetic ids, one clock
//! window over the whole loop as llama-bench times it; returns tok/s
[cold_path]
def public bench_tg_rep(t : Model; var s : Session; ngen, seed : int64) : double {
    let t0 = ref_time_ticks()
    for (i in range64(ngen)) {
        forward(t, s, bench_synth_id(i, seed), i)
    }
    return double(ngen) * 1.0e9lf / double(get_time_nsec(t0))
}

//! the calls the GPU driver handed to the CPU chain so far, every reason summed
def public cpu_pass_total() : int64 {
    var n = 0l
    for (p in gpu_cpu_passes_()) {
        n += p.count
    }
    return n
}

//! every one of the row's `npl` streams admitted and past its prefill
def private all_streams_decoding(sch : Scheduler; npl : int64) : bool {
    if (long_length(sch.streams) != npl) return false
    for (st in sch.streams) {
        if (st.state != StreamState.decoding) return false
    }
    return true
}

//! one timed batched tg rep in the served shape - what the batched reference exe's parallel row reads at that PL:
//! `npl` device-home streams through the scheduler's device mode over distinct `plen`-token prompts, prefilled
//! untimed, then `ngen` batched steps timed, the rate `npl x ngen` over their wall. `why` names a refusal and the
//! rate then reads 0: the device homes fewer streams than `npl` (the region count is asked before the load), the
//! streams never all reached decode within `npl x 8` steps, a stream that ended inside the timed steps, or a step
//! the driver handed to the CPU chain. Every path past `submit` parks the streams' regions for the next rep, a
//! refusal included
[cold_path, arch(at="../ARCHITECTURE_MEASUREMENT.md#one-benchmark-rig")]
def public bench_tg_batched_rep(t : Model; npl, plen, ngen, seed : int64) : tuple<tps : double; why : string> {
    let homes = gpu_device_sessions()
    if (homes < npl) {
        return (tps = 0.0lf, why = "the device homes {homes} stream(s), the row asks {npl}")
    }
    let passes0 = cpu_pass_total()
    var sch <- create_scheduler(t, npl, npl, plen)   //! a prompt is one prefill quantum
    verify(set_device_kv(sch, true), "bench_tg_batched_rep: an idle scheduler takes the device mode")
    var events : array<SchedEvent>
    for (i in range64(npl)) {
        var req <- PendingReq(id = i + 1l, prompt <- bench_prompt(plen, seed * 64l + i + 1l), max_new = ngen + npl + 8l)
        verify(submit(t, sch, req), "bench_tg_batched_rep: the queue holds the row's streams")
    }
    var why = ""
    var steps = 0l
    while (!all_streams_decoding(sch, npl)) {
        if (steps >= npl * 8l) {
            why = "the streams never all reached decode after {steps} steps"
            break
        }
        scheduler_step(t, sch, events)
        delete events
        steps++
    }
    var ns = 0l
    if (empty(why)) {
        scheduler_step(t, sch, events)   //! the untimed warm step: the batched command's first record
        delete events
        let t0 = ref_time_ticks()
        for (_i in range64(ngen)) {
            scheduler_step(t, sch, events)
            delete events
        }
        ns = get_time_nsec(t0)
    }
    let alive = n_active(sch)
    let ids <- [for (st in sch.streams); st.id]
    for (id in ids) {   //! evicted, then reaped by one more step: the streams park their regions for the next rep
        evict(sch, id, events)
    }
    scheduler_step(t, sch, events)
    delete events
    delete sch
    if (empty(why) && alive != npl) {
        why = "{npl - alive} of the {npl} streams ended inside the timed steps"
    }
    let passed = cpu_pass_total() - passes0
    if (empty(why) && passed > 0l) {
        why = "{passed} call(s) passed to the CPU chain inside the row"
    }
    return (tps = empty(why) ? double(npl * ngen) * 1.0e9lf / double(ns) : 0.0lf, why = why)
}
