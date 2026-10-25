options gen2
options _dasllama_internal = true

// The CPU ASR line's four in-process races, one line a candidate, best of --reps interleaved rounds:
//   --arm bf16   the bf16 weight plane's routes against the f32 plane through matmul_batch at the gemma-E
//                projection's pp512 shape, the Qwen3-ASR conv's and a vision patch class: the stamped
//                bf16w family where the sidecar names a perm, the widen walk, the row nest it replaced
//   --arm gelu   gelu_erf on the float lattice against the double erfc body it replaced, in place over a
//                tower's FFN plane
//   --arm mel    log_mel_qwen3a over a 60 s synthetic clip, one lane against the lanes, the two images
//                compared element for element
//   --arm kpass  the decode attention's K pass over a model-sized f16 cache (28 layers x 3000 keys x
//                8 kv heads x 128, two q heads a kv head): the (kv head, slice) unit order against one
//                slice over every kv head, one lane and all lanes
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
require daslib/jobque_boost
require daslib/f16_cvt
require daslib/clargs
require math

[CommandLineArgs]
struct Args {
    @clarg_doc = "Which race: bf16 | gelu | mel | kpass | all (default: all)"
    arm : string = "all"
    @clarg_doc = "Timed rounds a candidate, interleaved, the best kept (default: 5)"
    reps : int = 5
    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

//! one candidate's line: its best round in ms, the median and the cv beside it, `rate` the figure the best makes
def private line(race, who : string; us : array<int>; rate : block<(ms : float) : string>) {
    let st = stats(us)
    print("{race} {who}: {st.best} ms ({invoke(rate, st.best)}; median {st.median} ms, cv {st.cv})\n")
}

//! past every dot length: the row nest serves every output
let private NEST_ONLY_MIN_N = 1l << 30

def private race_bf16(n, d, ntok : int64; reps : int) {
    let w <- race_fill(int(n * d), 1, 1.2, 0.0)
    var w16 : array<uint16>
    w16 |> resize(n * d)
    for (h, v in w16, w) {
        h = bf16_bits(v)
    }
    let x <- race_fill(int(ntok * n), 2, 1.4, 0.0)
    var y : array<float>
    y |> resize(ntok * d)
    let family = get_bf16_batch_gen()
    let min_n = get_f32_batch_tile_min_n()
    let labels <- [family != null ? "bf16 plane, the stamped family" : "bf16 plane, the widen walk (no family stamped)",
                   "f32 plane, the tile walk", "bf16 plane, the widen walk", "bf16 plane, the row nest"]
    var times <- time_rounds(length(labels), reps, 1, $(i : int) {
        if (i == 0) {
            set_bf16_batch_gen(family)
        } else {
            let none : Bf16BatchGenFn
            set_bf16_batch_gen(none)
        }
        set_f32_batch_tile_min_n(i == 3 ? NEST_ONLY_MIN_N : min_n)
    }, $(i : int) {
        if (i == 1) {
            matmul_batch(y, w, x, n, d, ntok)
        } else {
            matmul_bf16_batch(y, w16, 0l, x, n, d, ntok)
        }
    })
    set_bf16_batch_gen(family)
    set_f32_batch_tile_min_n(min_n)
    let fl = 2.0lf * double(n * d * ntok) / 1.0e6lf
    for (lbl, us in labels, times) {
        line("bf16 n={n} d={d} ntok={ntok}", lbl, us) $(ms : float) => "{fl / double(ms)} GFLOP/s"
    }
}

//! the GELU body before the float kernel: erfc as the NR rational fit over libm's exp, every op in double
def private gelu_erf_double(var x : array<float>) {
    for (v in x) {
        let d = double(v)
        let z = abs(d) * 0.7071067811865476lf
        let t = 1.0lf / (1.0lf + 0.5lf * z)
        let erfc = t * exp(-z * z - 1.26551223lf + t * (1.00002368lf + t * (0.37409196lf + t * (0.09678418lf + t * (-0.18628806lf
            + t * (0.27886807lf + t * (-1.13520398lf + t * (1.48851587lf + t * (-0.82215223lf + t * 0.17087277lf)))))))))
        v = float(d >= 0.0lf ? 0.5lf * d * (2.0lf - erfc) : 0.5lf * d * erfc)
    }
}

def private race_gelu(reps : int) {
    let n = 1024 * 4096
    let x0 <- race_fill(n, 3, 8.0, 0.0)
    var x : array<float>
    x |> resize(n)
    let labels <- ["the float kernel", "the double body"]
    var times <- time_rounds(length(labels), reps, 1, $(_i : int) {
        for (d, s in x, x0) {
            d = s
        }
    }, $(i : int) {
        if (i == 0) {
            gelu_erf(unsafe(addr(x[0])), int64(n))
        } else {
            gelu_erf_double(x)
        }
    })
    for (lbl, us in labels, times) {
        line("gelu_erf {n} elements", lbl, us) $(ms : float) => "{ms * 1.0e6 / float(n)} ns an element"
    }
}

def private race_mel(reps : int) {
    var samples : array<float>
    samples |> reserve_resize(16000 * 60)
    for (i in range(16000 * 60)) {
        samples[i] = 0.3 * sin(float(i) * 0.01) + 0.1 * sin(float(i) * 0.37) + 0.05 * float((i * 7919) % 101) / 101.0
    }
    set_single_thread_(true)
    var inscope a <- log_mel_qwen3a(samples, 128l)
    set_single_thread_(false)
    var inscope b <- log_mel_qwen3a(samples, 128l)
    let labels <- ["one lane", "the lanes"]
    var times <- time_rounds(length(labels), reps, 1, $(i : int) {
        set_single_thread_(i == 0)
    }, $(_i : int) {
        var inscope m <- log_mel_qwen3a(samples, 128l)
    })
    set_single_thread_(false)
    var diff = 0
    for (p, q in a.data, b.data) {
        if (p != q) {
            diff++
        }
    }
    for (lbl, us in labels, times) {
        line("mel 60 s x 128", lbl, us) $(_ms : float) => "elements apart {diff} of {length(a.data)}"
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

def private race_kpass(reps : int) {
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
    let mb = double(KP_L * KP_KEYS * kvdim * 2l) / 1.0e6lf
    let labels <- ["(kv head, slice) units, one lane", "(kv head, slice) units, {KP_KVH * KP_NSL} units on the lanes",
                   "a slice over every kv head, one lane", "a slice over every kv head, {KP_NSL} units on the lanes"]
    var times <- time_rounds(length(labels), reps, 1) $(i : int) {
        if (i < 2) {
            kpass_heads(kall, q, att, i == 0 ? 1 : int(KP_KVH * KP_NSL))
        } else {
            kpass_rows(kall, q, att, i == 2 ? 1 : int(KP_NSL))
        }
    }
    for (lbl, us in labels, times) {
        line("kpass {mb} MB of f16 keys a step", lbl, us) $(ms : float) => "{mb / double(ms)} GB/s, {ms / float(KP_L)} ms a layer"
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
    with_dasllama_jobque_() {
        if (cfg.arm == "all" || cfg.arm == "bf16") {
            race_bf16(2560l, 10752l, 512l, cfg.reps)
            race_bf16(4320l, 480l, 800l, cfg.reps)
            race_bf16(1152l, 1152l, 256l, cfg.reps)
        }
        if (cfg.arm == "all" || cfg.arm == "gelu") {
            race_gelu(cfg.reps)
        }
        if (cfg.arm == "all" || cfg.arm == "mel") {
            race_mel(cfg.reps)
        }
        if (cfg.arm == "all" || cfg.arm == "kpass") {
            race_kpass(cfg.reps)
        }
    }
    return 0
}
