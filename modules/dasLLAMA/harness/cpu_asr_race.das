options gen2
options _dasllama_internal = true

// The CPU ASR line's three in-process races, one line a candidate, best of --reps interleaved:
//   --arm bf16   the bf16 weight plane against the f32 plane through matmul_batch at the gemma-E
//                projection's pp512 shape, the Qwen3-ASR conv's and a vision patch class - the stamped
//                bf16w family where the sidecar names a perm, the widen walk elsewhere
//   --arm mel    log_mel_qwen3a over a 60 s synthetic clip, one lane against the lanes, the two
//                images compared element for element
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
require daslib/jobque_boost
require daslib/f16_cvt
require daslib/clargs
require math

[CommandLineArgs]
struct Args {
    @clarg_doc = "Which race: bf16 | mel | kpass | all (default: all)"
    arm : string = "all"
    @clarg_doc = "Timed rounds a candidate, the best kept (default: 5)"
    reps : int = 5
    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

def private bf16_bits_of(v : float) : uint16 {
    let bits = unsafe(reinterpret<uint>(v))
    return uint16((bits + 0x7FFFu + ((bits >> 16u) & 1u)) >> 16u)
}

def private best_ms(reps : int; blk : block<() : void>) : double {
    var best = 1.0e30lf
    for (_r in range(reps)) {
        let t0 = ref_time_ticks()
        invoke(blk)
        best = min(best, double(get_time_usec(t0)) / 1000.0lf)
    }
    return best
}

def private race_bf16(n, d, ntok : int64; reps : int) {
    var w : array<float>
    var w16 : array<uint16>
    var x, y : array<float>
    w |> reserve_resize(int(n * d))
    w16 |> reserve_resize(int(n * d))
    x |> reserve_resize(int(ntok * n))
    y |> reserve_resize(int(ntok * d))
    for (i in range64(n * d)) {
        w[i] = float(i % 13l) * 0.1 - 0.6
        w16[i] = bf16_bits_of(w[i])
    }
    for (i in range64(ntok * n)) {
        x[i] = float(i % 7l) * 0.2 - 0.7
    }
    matmul_bf16_batch(y, w16, 0l, x, n, d, ntok)   // warm
    matmul_batch(y, w, x, n, d, ntok)
    var bf = 1.0e30lf
    var ff = 1.0e30lf
    for (_r in range(reps)) {
        let tb = best_ms(1) {
            matmul_bf16_batch(y, w16, 0l, x, n, d, ntok)
        }
        bf = min(bf, tb)
        let tf = best_ms(1) {
            matmul_batch(y, w, x, n, d, ntok)
        }
        ff = min(ff, tf)
    }
    let fl = 2.0lf * double(n * d * ntok) / 1.0e6lf
    print("bf16 n={n} d={d} ntok={ntok}: bf16 plane {bf} ms ({fl / bf} GFLOP/s, {bf16w_tileform_gen() > 0 ? "the stamped family" : "the widen walk"}), f32 plane {ff} ms ({fl / ff} GFLOP/s)\n")
}

def private race_mel(reps : int) {
    var samples : array<float>
    samples |> reserve_resize(16000 * 60)
    for (i in range(16000 * 60)) {
        samples[i] = 0.3 * sin(float(i) * 0.01) + 0.1 * sin(float(i) * 0.37) + 0.05 * float((i * 7919) % 101) / 101.0
    }
    set_single_thread_(true)
    var inscope a <- log_mel_qwen3a(samples, 128l)
    let one = best_ms(reps) {
        var inscope m <- log_mel_qwen3a(samples, 128l)
    }
    set_single_thread_(false)
    var inscope b <- log_mel_qwen3a(samples, 128l)
    let thr = best_ms(reps) {
        var inscope m <- log_mel_qwen3a(samples, 128l)
    }
    var diff = 0
    for (p, q in a.data, b.data) {
        if (p != q) {
            diff++
        }
    }
    print("mel 60 s x 128: one lane {one} ms, the lanes {thr} ms, elements apart {diff} of {length(a.data)}\n")
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
    let h1 = best_ms(reps) {
        kpass_heads(kall, q, att, 1)
    }
    let hn = best_ms(reps) {
        kpass_heads(kall, q, att, int(KP_KVH * KP_NSL))
    }
    let r1 = best_ms(reps) {
        kpass_rows(kall, q, att, 1)
    }
    let rn = best_ms(reps) {
        kpass_rows(kall, q, att, int(KP_NSL))
    }
    print("kpass {mb} MB of f16 keys a step, (kv head, slice) units: one lane {h1} ms ({mb / h1} GB/s), {KP_KVH * KP_NSL} units {hn} ms ({mb / hn} GB/s, {hn / double(KP_L)} ms a layer)\n")
    print("kpass {mb} MB of f16 keys a step, a slice over every kv head: one lane {r1} ms ({mb / r1} GB/s), {KP_NSL} units {rn} ms ({mb / rn} GB/s, {rn / double(KP_L)} ms a layer)\n")
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
        if (cfg.arm == "all" || cfg.arm == "mel") {
            race_mel(cfg.reps)
        }
        if (cfg.arm == "all" || cfg.arm == "kpass") {
            race_kpass(cfg.reps)
        }
    }
    return 0
}
