options gen2
options _dasllama_internal = true

// The CPU ASR line's races in one process: the arms timed in turn, one warmup round each arm drops, then --reps
// rounds; a report row is one arm's best round, with its compare and the tune manifest the run compiled under.
//   --arm bf16   baseline: the f32 plane through matmul_batch (the tile walk) over the bf16 weights widened;
//                compared arms: the bf16 plane through matmul_bf16_batch on the stamped bf16w family (where the
//                sidecar names a perm), on the widen walk and on the row nest it replaced; the compare region is
//                SAMPLE outputs of the ntok x d image, the baseline held to a double dot over them, the widen and
//                nest arms to the f32 bound, the family (its activations rounded to bf16) to the bf16 bound
//   --arm gelu   baseline: the double erfc body gelu_erf replaced, held to the erf series; compared arm: the
//                float kernel; the compare region is SAMPLE elements of the plane
//   --arm kpass  baseline: the decode attention's K pass over (kv head, slice) units on one lane (28 layers x 3000
//                keys x 8 kv heads x 128, f16, two q heads a kv head), held to a double dot; compared arms: the
//                same units on the lanes, one slice over every kv head on one lane and on the lanes, each
//                bit-exact with the baseline (dot_f16 a score, the same call on every route); the region is
//                SAMPLE scores
//   ATTRIBUTION SWEEP --arm mel: log_mel_qwen3a over a 60 s synthetic clip, one lane against the lanes,
//                timing-only - the lanes' image against the one-lane image is tests/test_whisper.das's mel cell
//
//   DAS_TUNE_MANIFEST=<sidecar> bin/daslang -jit modules/dasLLAMA/harness/cpu_asr_race.das -- [--arm all] [--reps N]

require dasllama/dasllama_math
require dasllama/dasllama_math_default
require dasllama/dasllama_math_gen
require dasllama/dasllama_kv_codec
require dasllama/dasllama_qwen3a
require dasllama/dasllama_audio
require dasllama/dasllama_par
require _race_fixture
require ../tests/_compares.das
require daslib/env_registry
require daslib/jobque_boost
require daslib/f16_cvt
require daslib/clargs
require math

[CommandLineArgs]
struct Args {
    @clarg_doc = "Which race: bf16 | gelu | mel | kpass | all (default: all)"
    arm : string = "all"
    @clarg_doc = "Timed rounds a candidate after its warmup round, interleaved, the best kept (default: 5)"
    reps : int = 5
    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

let SAMPLE = 512
let F32_BOUND = 1.0e-5
let BF16_BOUND = 1.0e-2
let GELU_BOUND = 1.0e-5
let REF_BOUND = 1.0e-6
let WARMUP = 1

//! the compare region: SAMPLE indices into a `total`-element image off an LCG
def private sample_idx(total : int) : array<int> {
    var idx : array<int>
    idx |> resize(SAMPLE)
    var st = 0x9E3779B9u
    for (i in range(SAMPLE)) {
        st = st * 1664525u + 1013904223u
        idx[i] = int((st >> 4u) % uint(total))
    }
    return <- idx
}

def private gather(src : array<float>; idx : array<int>) : array<float> => <- [for (i in idx); src[i]]

def private manifest_of() : string {
    let m = env_value_of("DAS_TUNE_MANIFEST")
    return empty(m) ? "manifest=unset" : "manifest={m}"
}

//! one candidate's report row: the best round in ms, the median and cv beside it, `rate` the figure the best
//! makes, the compare's words; a miss prints the miss and the bound alone
def private row(race, who : string; us : array<int>; h : Held; against : string; rate : block<(ms : float) : string>; var t : RaceTally) {
    let st = stats(us)
    let words = compare_words(h, against, "{SAMPLE} sampled elements")
    if (!h.ok) {
        print("{race} {who}: {words} {manifest_of()}\n")
        tally(t, -1)
        return
    }
    print("{race} {who}: {st.best} ms ({invoke(rate, st.best)}; median {st.median} ms, cv {st.cv}) {words} {manifest_of()}\n")
    tally(t, 1)
}

//! a timing-only row of an attribution sweep
def private row_timing_only(race, who : string; us : array<int>; rate : block<(ms : float) : string>; var t : RaceTally) {
    let st = stats(us)
    print("{race} {who}: {st.best} ms ({invoke(rate, st.best)}; median {st.median} ms, cv {st.cv}) timing-only {manifest_of()}\n")
    tally(t, 1)
}

//! past every dot length: the row nest serves every output
let private NEST_ONLY_MIN_N = 1l << 30

//! every arm once - `before(i)`, `arm(i)`, then `sample(i)` reading its output - and then the timed rounds
def private sampled_rounds(narms, reps : int; before : block<(i : int) : void>; arm : block<(i : int) : void>; sample : block<(i : int) : void>) : array<array<int>> {
    for (i in range(narms)) {
        invoke(before, i)
        invoke(arm, i)
        invoke(sample, i)
    }
    return <- time_rounds(narms, reps, WARMUP, before, arm)
}

def private widen(h : uint16) : float => unsafe(reinterpret<float>(uint(h) << 16u))

def private race_bf16(n, d, ntok : int64; reps : int; var t : RaceTally) {
    let w <- race_fill(int(n * d), 1, 1.2, 0.0)
    var w16 : array<uint16>
    w16 |> reserve_resize(n * d)
    var wwide : array<float>
    wwide |> reserve_resize(n * d)
    for (h, wide, v in w16, wwide, w) {
        h = bf16_bits(v)
        wide = widen(h)
    }
    let x <- race_fill(int(ntok * n), 2, 1.4, 0.0)
    var y : array<float>
    y |> reserve_resize(ntok * d)
    let idx <- sample_idx(int(ntok * d))
    var want : array<float>
    want |> resize(SAMPLE)
    for (s, wv in idx, want) {
        let tk = int64(s) / d
        let j = int64(s) % d
        var acc = 0.0lf
        for (k in range64(n)) {
            acc += double(x[tk * n + k]) * double(wwide[j * n + k])
        }
        wv = float(acc)
    }
    let family = get_bf16_batch_gen()
    let min_n = get_f32_batch_tile_min_n()
    var labels <- ["f32 plane, the tile walk"]
    var arms : array<int>   // 0 the baseline, 1 the family, 2 the widen walk, 3 the row nest
    arms |> push(0)
    if (bf16w_batch_serves(n, d, ntok)) {
        labels |> push("bf16 plane, the stamped family")
        arms |> push(1)
    }
    labels |> push("bf16 plane, the widen walk")
    arms |> push(2)
    labels |> push("bf16 plane, the row nest")
    arms |> push(3)
    let narms = length(arms)
    var got : array<array<float>>
    got |> resize(narms)
    let times <- sampled_rounds(narms, reps, $(i : int) {
        let a = arms[i]
        if (a == 1) {
            set_bf16_batch_gen(family)
        } else {
            let none : Bf16BatchGenFn
            set_bf16_batch_gen(none)
        }
        set_f32_batch_tile_min_n(a == 3 ? NEST_ONLY_MIN_N : min_n)
        poison(y)
    }, $(i : int) {
        if (arms[i] == 0) {
            matmul_batch(y, wwide, x, n, d, ntok)
        } else {
            matmul_bf16_batch(y, w16, 0l, x, n, d, ntok)
        }
    }, $(i : int) {
        got[i] <- gather(y, idx)
    })
    set_bf16_batch_gen(family)
    set_f32_batch_tile_min_n(min_n)
    let fl = 2.0lf * double(n * d * ntok) / 1.0e6lf
    for (i in range(narms)) {
        let h = held(got[i], want, arms[i] == 1 ? BF16_BOUND : F32_BOUND)
        row("bf16 n={n} d={d} ntok={ntok}", labels[i], times[i], h, "the double dot", $(ms : float) => "{fl / double(ms)} GFLOP/s", t)
    }
}

//! the GELU body before the float kernel: erfc as the NR rational fit over libm's exp, every op in double
def private gelu_erf_double(var x : array<float>) {
    for (v in x) {
        let dv = double(v)
        let z = abs(dv) * 0.7071067811865476lf
        let tt = 1.0lf / (1.0lf + 0.5lf * z)
        let erfc = tt * exp(-z * z - 1.26551223lf + tt * (1.00002368lf + tt * (0.37409196lf + tt * (0.09678418lf + tt * (-0.18628806lf
            + tt * (0.27886807lf + tt * (-1.13520398lf + tt * (1.48851587lf + tt * (-0.82215223lf + tt * 0.17087277lf)))))))))
        v = float(dv >= 0.0lf ? 0.5lf * dv * (2.0lf - erfc) : 0.5lf * dv * erfc)
    }
}

def private race_gelu(reps : int; var t : RaceTally) {
    let n = 1024 * 4096
    let x0 <- race_fill(n, 3, 8.0, 0.0)
    var x : array<float>
    x |> reserve_resize(n)
    let idx <- sample_idx(n)
    var want : array<float>
    want |> resize(SAMPLE)
    for (s, wv in idx, want) {
        let dv = double(x0[s])
        wv = float(0.5lf * dv * (1.0lf + erf_series(dv * 0.7071067811865476lf)))
    }
    let labels <- ["the double body", "the float kernel"]
    var got : array<array<float>>
    got |> resize(2)
    let times <- sampled_rounds(2, reps, $(_i : int) {
        for (dst, src in x, x0) {
            dst = src
        }
    }, $(i : int) {
        if (i == 0) {
            gelu_erf_double(x)
        } else {
            gelu_erf(unsafe(addr(x[0])), int64(n))
        }
    }, $(i : int) {
        got[i] <- gather(x, idx)
    })
    for (i in range(2)) {
        let h = held(got[i], want, i == 0 ? REF_BOUND : GELU_BOUND)
        row("gelu_erf {n} elements", labels[i], times[i], h, "the erf series", $(ms : float) => "{ms * 1.0e6 / float(n)} ns an element", t)
    }
}

def private race_mel(reps : int; var t : RaceTally) {
    var samples : array<float>
    samples |> reserve_resize(16000 * 60)
    for (i in range(16000 * 60)) {
        samples[i] = 0.3 * sin(float(i) * 0.01) + 0.1 * sin(float(i) * 0.37) + 0.05 * float((i * 7919) % 101) / 101.0
    }
    if (get_dispatch_lanes() <= 1) {
        print("mel: WARNING one dispatch lane on this box - the single-thread switch changes nothing\n")
    }
    let labels <- ["one lane", "the lanes"]
    let times <- time_rounds(2, reps, WARMUP, $(i : int) {
        set_single_thread_(i == 0)
    }, $(_i : int) {
        var inscope m <- log_mel_qwen3a(samples, 128l)
    })
    set_single_thread_(false)
    for (i in range(2)) {
        row_timing_only("mel 60 s x 128", labels[i], times[i], $(_ms : float) => "{get_dispatch_lanes()} lanes on the box", t)
    }
}

let KP_L = 28l
let KP_KEYS = 3000l
let KP_HS = 128l
let KP_KVH = 8l
let KP_SEQ = 4096l
let KP_NSL = 32l

//! the K pass as the chain dispatched it before fcfb1dacd8: a unit a (kv head, slice)
def private kpass_heads(kall : array<uint16>; q : array<float>; var att : array<float>; lanes : int) {
    let kvdim = KP_KVH * KP_HS
    unsafe {
        let qp = addr<float const?>(q[0])
        var ap = addr(att[0])
        for (l in range64(KP_L)) {
            let kp = addr<uint16 const?>(kall[l * KP_KEYS * kvdim])
            let slice_len = (KP_KEYS + KP_NSL - 1l) / KP_NSL
            maybe_parallel_for(0, int(KP_KVH * KP_NSL), lanes) $(ub, ue) {
                unsafe {
                    for (u in range(ub, ue)) {
                        let kvh = int64(u) / KP_NSL
                        let sl = int64(u) % KP_NSL
                        let j0 = sl * slice_len
                        let j1 = min(j0 + slice_len, KP_KEYS)
                        for (j in range64(j0, j1)) {
                            let krow = kp + j * kvdim + kvh * KP_HS
                            for (g in range64(2l)) {
                                ap[(kvh * 2l + g) * KP_SEQ + j] = dot_f16(qp + (kvh * 2l + g) * KP_HS, krow, KP_HS)
                            }
                        }
                    }
                }
            }
        }
    }
}

//! the K pass as the chain dispatches it: a unit one slice over every kv head, each row read once
def private kpass_rows(kall : array<uint16>; q : array<float>; var att : array<float>; lanes : int) {
    let kvdim = KP_KVH * KP_HS
    unsafe {
        let qp = addr<float const?>(q[0])
        var ap = addr(att[0])
        for (l in range64(KP_L)) {
            let kp = addr<uint16 const?>(kall[l * KP_KEYS * kvdim])
            let slice_len = (KP_KEYS + KP_NSL - 1l) / KP_NSL
            maybe_parallel_for(0, int(KP_NSL), lanes) $(ub, ue) {
                unsafe {
                    for (u in range(ub, ue)) {
                        let j0 = int64(u) * slice_len
                        let j1 = min(j0 + slice_len, KP_KEYS)
                        for (j in range64(j0, j1)) {
                            let krow = kp + j * kvdim
                            for (hh in range64(2l * KP_KVH)) {
                                ap[hh * KP_SEQ + j] = dot_f16(qp + hh * KP_HS, krow + (hh / 2l) * KP_HS, KP_HS)
                            }
                        }
                    }
                }
            }
        }
    }
}

def private race_kpass(reps : int; var t : RaceTally) {
    let kvdim = KP_KVH * KP_HS
    var kall : array<uint16>
    kall |> reserve_resize(int(KP_L * KP_KEYS * kvdim))
    for (i in range64(KP_L * KP_KEYS * kvdim)) {
        kall[i] = uint16(f32_to_f16(0.5 * sin(float(i % 977l) * 0.01)))
    }
    var q : array<float>
    q |> resize(2l * KP_KVH * KP_HS)
    for (i in range64(2l * KP_KVH * KP_HS)) {
        q[i] = cos(float(i) * 0.03)
    }
    var att : array<float>
    att |> resize(2l * KP_KVH * KP_SEQ)
    // the region: SAMPLE scores of the last layer's image, keys under KP_KEYS only
    var idx : array<int>
    idx |> resize(SAMPLE)
    var st = 0x9E3779B9u
    for (i in range(SAMPLE)) {
        st = st * 1664525u + 1013904223u
        let hh = int((st >> 4u) % uint(2l * KP_KVH))
        st = st * 1664525u + 1013904223u
        let j = int((st >> 4u) % uint(KP_KEYS))
        idx[i] = hh * int(KP_SEQ) + j
    }
    var want : array<float>
    want |> resize(SAMPLE)
    let last = (KP_L - 1l) * KP_KEYS * kvdim
    for (s, wv in idx, want) {
        let hh = int64(s) / KP_SEQ
        let j = int64(s) % KP_SEQ
        var acc = 0.0lf
        for (c in range64(KP_HS)) {
            acc += double(q[hh * KP_HS + c]) * double(f16_to_f32(uint(kall[last + j * kvdim + (hh / 2l) * KP_HS + c])))
        }
        wv = float(acc)
    }
    let labels <- ["(kv head, slice) units, one lane", "(kv head, slice) units, {KP_KVH * KP_NSL} units on the lanes",
                   "a slice over every kv head, one lane", "a slice over every kv head, {KP_NSL} units on the lanes"]
    var got : array<array<float>>
    got |> resize(4)
    let times <- sampled_rounds(4, reps, $(_i : int) {
        poison(att)
    }, $(i : int) {
        if (i < 2) {
            kpass_heads(kall, q, att, i == 0 ? 1 : int(KP_KVH * KP_NSL))
        } else {
            kpass_rows(kall, q, att, i == 2 ? 1 : int(KP_NSL))
        }
    }, $(i : int) {
        got[i] <- gather(att, idx)
    })
    let mb = double(KP_L * KP_KEYS * kvdim * 2l) / 1.0e6lf
    for (i in range(4)) {
        let h = i == 0 ? held(got[0], want, F32_BOUND) : held(got[i], got[0], 0.0)
        row("kpass {mb} MB of f16 keys a step", labels[i], times[i], h, i == 0 ? "the double dot" : labels[0], $(ms : float) => "{mb / double(ms)} GB/s, {ms / float(KP_L)} ms a layer", t)
    }
}

[export]
def main() : int {
    var r <- parse_args(type<Args>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<Args>), "cpu_asr_race")
        return 1
    }
    let cfg <- r |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<Args>), "cpu_asr_race")
        return 0
    }
    var t : RaceTally
    with_dasllama_jobque_() {
        if (cfg.arm == "all" || cfg.arm == "bf16") {
            race_bf16(2560l, 10752l, 512l, cfg.reps, t)
            race_bf16(4320l, 480l, 800l, cfg.reps, t)
            race_bf16(1152l, 1152l, 256l, cfg.reps, t)
        }
        if (cfg.arm == "all" || cfg.arm == "gelu") {
            race_gelu(cfg.reps, t)
        }
        if (cfg.arm == "all" || cfg.arm == "mel") {
            race_mel(cfg.reps, t)
        }
        if (cfg.arm == "all" || cfg.arm == "kpass") {
            race_kpass(cfg.reps, t)
        }
    }
    return race_exit(t, "cpu_asr_race")
}
