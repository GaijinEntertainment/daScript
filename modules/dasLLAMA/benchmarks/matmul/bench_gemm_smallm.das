options gen2
options _dasllama_internal = true

// Small-m batched-GEMM shape study (parakeet encoder, jfk tt=138): the d×d projections
// (attn q/k/v/o, conv pw2) aggregate ~132 GMAC/s while the d×4d FFN halves run ~296 at the
// SAME ntok. Times PRODUCTION matmul_q8q8_batch (whatever backend dasllama_common selected —
// arm64-gen/x64-gen repack tier on gen-capable boxes) at the exact shapes, to map the small-m
// falloff and A/B the dispatch knobs (token block, oversplit) that shape the walk.
//
//   bin/daslang -jit modules/dasLLAMA/benchmarks/matmul/bench_gemm_smallm.das
//   bin/daslang -jit modules/dasLLAMA/benchmarks/matmul/bench_gemm_smallm.das -- --tb
//   bin/daslang -jit modules/dasLLAMA/benchmarks/matmul/bench_gemm_smallm.das -- --knobs
//
// Interleaved rounds (config loop inside the round loop) — drift lands on every config evenly.

require dasllama/dasllama_common // [init]-only: registers the q8 kernel backends
require dasllama/dasllama_math
require dasllama/dasllama_audio
require dasllama/dasllama_convert
require dasllama/dasllama_par
require daslib/jobque_boost
require daslib/clargs
require math

let DD = 1024l  // parakeet n_state
let NL = 24     // parakeet-tdt-0.6b n_layer
let EPS = 1e-5

[CommandLineArgs]
struct BenchArgs {
    @clarg_short = "r"
    @clarg_doc = "Best-of rounds (default 5)"
    rounds : int

    @clarg_short = "t"
    @clarg_doc = "Encoder rows for the stage emulations (jfk ~138, gb1 ~2484; default 138)"
    tt : int

    @clarg_doc = "Token-block sweep at the parakeet shapes instead of the shape map"
    tb : bool

    @clarg_doc = "Dispatch-knob sweep (batch oversplit) at the parakeet shapes"
    knobs : bool

    @clarg_doc = "attn_proj stage emulation: 24 layers of ln+requant+q/k/v+pos, cold per-layer weights, per-op laps"
    attn : bool

    @clarg_doc = "ffn half emulation (control): 24 layers of ln+requant+ff1a+silu4+requant+ff1b+residual"
    ffn : bool

    @clarg_doc = "Keep the default (hand) backend instead of the repack tier a model load selects"
    hand : bool

    @clarg_doc = "silu4 lane-response sweep on the ffn image (tt x 4d), row-range threading"
    silu : bool

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

struct GemmCase {
    name : string
    n : int64   // contraction dim (weight cols)
    d : int64   // output rows
    m : int64   // tokens
}

struct CaseData {
    q : array<int8>
    s : array<float>
    xq : array<int8>
    xs : array<float>
    y : array<float>
    reps : int
}

def fill_sin(var a : array<float>; scale, step : float) {
    for (i, v in count(), a) {
        v = scale * sin(float(i) * step)
    }
}

def build_case(c : GemmCase; var cd : CaseData) {
    let nb = c.n / 32l
    var w : array<float>
    w |> resize(c.n * c.d)
    fill_sin(w, 0.5, 0.173)
    cd.q |> resize(c.n * c.d)
    cd.s |> resize(c.d * nb)
    quantize_q8_0_into(w, c.n * c.d, cd.q, cd.s, 0l, 0l)
    repack_q8q8_weight(cd.q, cd.s, 0l, c.n, c.d)
    delete w
    // one activation row, quantized once, replicated m times (timing is data-independent)
    var x : array<float>
    x |> resize(c.n)
    fill_sin(x, 1.0, 0.377)
    var xq1 : array<int8>
    var xs1 : array<float>
    xq1 |> resize(c.n)
    xs1 |> resize(nb)
    quantize_q8_0_into(x, c.n, xq1, xs1, 0l, 0l)
    cd.xq |> resize(c.m * c.n)
    cd.xs |> resize(c.m * nb)
    for (t in range64(c.m)) {
        for (i in range64(c.n)) {
            cd.xq[t * c.n + i] = xq1[i]
        }
        for (i in range64(nb)) {
            cd.xs[t * nb + i] = xs1[i]
        }
    }
    cd.y |> resize(c.m * c.d)
    let macs = c.n * c.d * c.m
    cd.reps = int(clamp(4_000_000_000l / macs, 1l, 400l))
    delete x
    delete xq1
    delete xs1
}

// one timed measurement: reps back-to-back production calls, returns microseconds
def time_case(var cd : CaseData; c : GemmCase) : double {
    let t0 = ref_time_ticks()
    for (_i in range(cd.reps)) {
        matmul_q8q8_batch(cd.y, cd.q, cd.s, cd.xq, cd.xs, c.n, c.d, c.m)
    }
    return double(get_time_usec(t0))
}

def gmacs(c : GemmCase; reps : int; us : double) : double {
    return double(c.n * c.d * c.m) * double(reps) / (us * 1000.0lf)
}

def report_case(c : GemmCase; var cd : CaseData; best_us : double) {
    // one extra diag call to read the dispatch shape this config takes
    let c0 = get_disp_count()
    let h0 = get_disp_chunks()
    let i0 = get_disp_inline()
    matmul_q8q8_batch(cd.y, cd.q, cd.s, cd.xq, cd.xs, c.n, c.d, c.m)
    let dcnt = get_disp_count() - c0
    let dchk = get_disp_chunks() - h0
    let dinl = get_disp_inline() - i0
    let g = gmacs(c, cd.reps, best_us)
    let ms = best_us / 1000.0lf
    print("  {c.name}  n={c.n} d={c.d} m={c.m}  {g} GMAC/s  ({ms} ms / {cd.reps} reps; disp={dcnt} chunks={dchk} inline={dinl})\n")
}

def run_map(rounds : int) {
    // the parakeet jfk shapes (tt=138, ww=2*tt-1=275) + gb1-scale reference (tt~2484)
    var cases <- [
        GemmCase(name = "qkv  d*d ", n = DD, d = DD, m = 138l),
        GemmCase(name = "pos  d*d ", n = DD, d = DD, m = 275l),
        GemmCase(name = "pw1  d*2d", n = DD, d = 2l * DD, m = 138l),
        GemmCase(name = "ff1a d*4d", n = DD, d = 4l * DD, m = 138l),
        GemmCase(name = "ff1b 4d*d", n = 4l * DD, d = DD, m = 138l),
        GemmCase(name = "qkv  d*d ", n = DD, d = DD, m = 2484l),
        GemmCase(name = "ff1a d*4d", n = DD, d = 4l * DD, m = 2484l)
    ]
    // the small-m falloff curve on the d*d shape
    let msweep = [4l, 8l, 16l, 32l, 64l, 96l, 128l, 138l, 160l, 192l, 256l, 384l, 512l, 1024l]
    cases |> reserve(length(cases) + length(msweep))
    for (m in msweep) {
        cases |> push(GemmCase(name = "map  d*d ", n = DD, d = DD, m = m))
    }

    var data : array<CaseData>
    data |> resize(length(cases))
    for (i, c in count(), cases) {
        build_case(c, data[i])
        time_case(data[i], c)   // warmup (JIT + page-in)
    }
    var best : array<double>
    best |> resize(length(cases))
    for (v in best) {
        v = 1e30lf
    }
    for (_r in range(rounds)) {
        for (i, c in count(), cases) {
            best[i] = min(best[i], time_case(data[i], c))
        }
    }
    print("\n--- shape map (best of {rounds} interleaved rounds) ---\n")
    for (i, c in count(), cases) {
        report_case(c, data[i], best[i])
    }
    delete data
}

def run_tb_sweep(rounds : int) {
    let tb0 = get_q8_token_block()
    var cases <- [
        GemmCase(name = "qkv  d*d ", n = DD, d = DD, m = 138l),
        GemmCase(name = "pos  d*d ", n = DD, d = DD, m = 275l),
        GemmCase(name = "ff1a d*4d", n = DD, d = 4l * DD, m = 138l),
        GemmCase(name = "qkv  d*d ", n = DD, d = DD, m = 2484l)
    ]
    let tbs = [16l, 32l, 46l, 64l, 96l, 128l, 138l, 144l, 256l, 512l]

    var data : array<CaseData>
    data |> resize(length(cases))
    for (i, c in count(), cases) {
        build_case(c, data[i])
        time_case(data[i], c)
    }
    var best : array<double>
    best |> resize(length(cases) * length(tbs))
    for (v in best) {
        v = 1e30lf
    }
    for (_r in range(rounds)) {
        for (i, c in count(), cases) {
            for (ti, tb in count(), tbs) {
                set_q8_token_block(tb)
                let bi = i * length(tbs) + ti
                best[bi] = min(best[bi], time_case(data[i], c))
            }
        }
    }
    set_q8_token_block(tb0)
    print("\n--- token-block sweep (best of {rounds} interleaved rounds; default TB={tb0}) ---\n")
    for (i, c in count(), cases) {
        print("  {c.name}  n={c.n} d={c.d} m={c.m}:\n")
        for (ti, tb in count(), tbs) {
            let g = gmacs(c, data[i].reps, best[i * length(tbs) + ti])
            print("    TB={tb}  {g} GMAC/s\n")
        }
    }
    delete data
}

def run_knob_sweep(rounds : int) {
    let os0 = get_q8_batch_chunks_per_job()
    var cases <- [
        GemmCase(name = "qkv  d*d ", n = DD, d = DD, m = 138l),
        GemmCase(name = "ff1b 4d*d", n = 4l * DD, d = DD, m = 138l)
    ]
    let oss = [1, 2, 4, 8, 16]

    var data : array<CaseData>
    data |> resize(length(cases))
    for (i, c in count(), cases) {
        build_case(c, data[i])
        time_case(data[i], c)
    }
    var best : array<double>
    best |> resize(length(cases) * length(oss))
    for (v in best) {
        v = 1e30lf
    }
    for (_r in range(rounds)) {
        for (i, c in count(), cases) {
            for (oi, os in count(), oss) {
                set_q8_batch_chunks_per_job(os)
                let bi = i * length(oss) + oi
                best[bi] = min(best[bi], time_case(data[i], c))
            }
        }
    }
    set_q8_batch_chunks_per_job(os0)
    print("\n--- batch-oversplit sweep (best of {rounds} interleaved rounds; default={os0}) ---\n")
    for (i, c in count(), cases) {
        print("  {c.name}  n={c.n} d={c.d} m={c.m}:\n")
        for (oi, os in count(), oss) {
            let g = gmacs(c, data[i].reps, best[i * length(oss) + oi])
            print("    oversplit={os}  {g} GMAC/s\n")
        }
    }
    delete data
}

// ===== stage emulations: the engine's per-layer op sequence, verbatim shapes, cold per-layer
// weights (the encode pass streams every layer's weights from DRAM — a single reused weight
// would sit in LLC and overstate the kernel) =====

struct StageWeights {
    q : array<int8>
    s : array<float>
}

def build_weight(n, d : int64; step : float; var sw : StageWeights) {
    var w : array<float>
    w |> resize(n * d)
    fill_sin(w, 0.5, step)
    sw.q |> resize(n * d)
    sw.s |> resize(n * d / 32l)
    quantize_q8_0_into(w, n * d, sw.q, sw.s, 0l, 0l)
    repack_q8q8_weight(sw.q, sw.s, 0l, n, d)
    delete w
}

def lap(var acc : array<double>; i : int; t0 : int64) : int64 {
    acc[i] += double(get_time_usec(t0))
    return ref_time_ticks()
}

def report_ops(names : array<string>; best : array<double>; rounds : int; engine_bucket : string) {
    var total = 0.0lf
    for (b in best) {
        total += b
    }
    for (i, nm in count(), names) {
        let ms = best[i] / 1000.0lf
        let pct = best[i] / total * 100.0lf
        print("  {nm}: {ms} ms  ({pct}%)\n")
    }
    print("  TOTAL: {total / 1000.0lf} ms per encode pass (best-of-{rounds} per op; engine bucket = {engine_bucket})\n")
}

def run_attn_stage(rounds : int; tt : int64) {
    let d = DD
    let ww = 2l * tt - 1l
    let nb = d / 32l
    print("\n--- attn_proj stage emulation: {NL} layers, tt={tt}, ww={ww} ---\n")
    var wts : array<StageWeights>   // per layer: q, k, v, pos
    wts |> resize(NL * 4)
    for (i in range(NL * 4)) {
        build_weight(d, d, 0.171 + 0.001 * float(i), wts[i])
    }
    var blob : array<float>   // LN gamma at 0, beta at d
    blob |> resize(2l * d)
    fill_sin(blob, 1.0, 0.0173)
    var x : array<float>
    var xb : array<float>
    var qq : array<float>
    var kk : array<float>
    var vv : array<float>
    var posl : array<float>
    var pos_emb : array<float>
    x |> resize(tt * d)
    xb |> resize(tt * d)
    qq |> resize(tt * d)
    kk |> resize(tt * d)
    vv |> resize(tt * d)
    posl |> resize(ww * d)
    pos_emb |> resize(ww * d)
    fill_sin(x, 1.0, 0.377)
    fill_sin(pos_emb, 1.0, 0.531)
    var xqi : array<int8>
    var xsi : array<float>
    xqi |> resize(tt * d)
    xsi |> resize(tt * nb)
    // the layer-invariant pos image requants once per encode in the engine — hoisted here too
    var posq : array<int8>
    var poss : array<float>
    posq |> resize(ww * d)
    poss |> resize(ww * nb)
    asr_requant_rows_q8(pos_emb, d, ww, posq, poss)

    let names <- ["ln     ", "requant", "q      ", "k      ", "v      ", "pos    "]
    var best : array<double>
    best |> resize(length(names))
    for (v in best) {
        v = 1e30lf
    }
    var acc : array<double>
    acc |> resize(length(names))
    for (rnd in range(rounds + 1)) {   // round 0 = warmup (JIT + page-in), not recorded
        for (v in acc) {
            v = 0.0lf
        }
        for (l in range(NL)) {
            var t = ref_time_ticks()
            layernorm_batch(xb, x, blob, 0l, d, d, tt, EPS)
            t = lap(acc, 0, t)
            asr_requant_rows_q8(xb, d, tt, xqi, xsi)
            t = lap(acc, 1, t)
            matmul_q8q8_batch(qq, wts[l * 4 + 0].q, wts[l * 4 + 0].s, xqi, xsi, d, d, tt)
            t = lap(acc, 2, t)
            matmul_q8q8_batch(kk, wts[l * 4 + 1].q, wts[l * 4 + 1].s, xqi, xsi, d, d, tt)
            t = lap(acc, 3, t)
            matmul_q8q8_batch(vv, wts[l * 4 + 2].q, wts[l * 4 + 2].s, xqi, xsi, d, d, tt)
            t = lap(acc, 4, t)
            matmul_q8q8_batch(posl, wts[l * 4 + 3].q, wts[l * 4 + 3].s, posq, poss, d, d, ww)
            t = lap(acc, 5, t)
        }
        if (rnd > 0) {
            for (b, a in best, acc) {
                b = min(b, a)
            }
        }
    }
    report_ops(names, best, rounds, "pk.attn_proj")
    delete wts
}

def run_ffn_stage(rounds : int; tt : int64) {
    let d = DD
    let d4 = 4l * d
    print("\n--- ffn half emulation (one macaron half; engine pk.ffn = TWO of these): {NL} layers, tt={tt} ---\n")
    var wts : array<StageWeights>   // per layer: ff1a (d -> 4d), ff1b (4d -> d)
    wts |> resize(NL * 2)
    for (l in range(NL)) {
        build_weight(d, d4, 0.171 + 0.001 * float(l), wts[l * 2 + 0])
        build_weight(d4, d, 0.271 + 0.001 * float(l), wts[l * 2 + 1])
    }
    var blob : array<float>
    blob |> resize(2l * d)
    fill_sin(blob, 1.0, 0.0173)
    var x : array<float>
    var xb : array<float>
    var h : array<float>
    x |> resize(tt * d)
    xb |> resize(tt * d)
    h |> resize(tt * d4)
    fill_sin(x, 1.0, 0.377)
    var xqi : array<int8>
    var xsi : array<float>
    xqi |> resize(tt * d4)   // shared requant scratch sized for the 4d image (engine: st.xqi)
    xsi |> resize(tt * d4 / 32l)

    let names <- ["ln     ", "rq d   ", "ff1a   ", "silu4  ", "rq 4d  ", "ff1b   ", "resid  "]
    var best : array<double>
    best |> resize(length(names))
    for (v in best) {
        v = 1e30lf
    }
    var acc : array<double>
    acc |> resize(length(names))
    for (rnd in range(rounds + 1)) {
        for (v in acc) {
            v = 0.0lf
        }
        for (l in range(NL)) {
            var t = ref_time_ticks()
            layernorm_batch(xb, x, blob, 0l, d, d, tt, EPS)
            t = lap(acc, 0, t)
            asr_requant_rows_q8(xb, d, tt, xqi, xsi)
            t = lap(acc, 1, t)
            matmul_q8q8_batch(h, wts[l * 2 + 0].q, wts[l * 2 + 0].s, xqi, xsi, d, d4, tt)
            t = lap(acc, 2, t)
            silu4_batch(h, d4, tt)
            t = lap(acc, 3, t)
            asr_requant_rows_q8(h, d4, tt, xqi, xsi)
            t = lap(acc, 4, t)
            matmul_q8q8_batch(xb, wts[l * 2 + 1].q, wts[l * 2 + 1].s, xqi, xsi, d4, d, tt)
            t = lap(acc, 5, t)
            for (i in range64(tt * d)) {
                x[i] += 0.5 * xb[i]
            }
            t = lap(acc, 6, t)
        }
        if (rnd > 0) {
            for (b, a in best, acc) {
                b = min(b, a)
            }
        }
    }
    report_ops(names, best, rounds, "pk.ffn / 2")
    delete wts
}

// silu4 lane response on the ffn image (tt rows x 4d): whole-array call vs row-range chunks at
// forced lane counts. Row length 4d is a multiple of 4, so every chunk start is 4-aligned and
// the float4 grouping matches the whole-array pass element-for-element — bit-identical.
def run_silu_sweep(rounds : int; tt : int64) {
    let dimg = 4l * DD
    let n = tt * dimg
    var h : array<float>
    h |> resize(n)
    fill_sin(h, 2.0, 0.377)
    let reps = int(clamp(200_000_000l / n, 1l, 400l))
    let lane_grid = [1, 2, 4, 6, 8]
    var best : array<double>
    best |> resize(length(lane_grid) + 1)
    for (v in best) {
        v = 1e30lf
    }
    unsafe {
        var hp = addr(h[0])
        silu4(h, n)   // warmup
        for (_r in range(rounds)) {
            var t0 = ref_time_ticks()
            for (_i in range(reps)) {
                silu4(h, n)
            }
            best[0] = min(best[0], double(get_time_usec(t0)))
            for (li, lanes in count(), lane_grid) {
                t0 = ref_time_ticks()
                for (_i in range(reps)) {
                    maybe_parallel_for(0, int(tt), lanes) $(rb, re) {
                        silu4(unsafe(hp + int64(rb) * dimg), int64(re - rb) * dimg)
                    }
                }
                best[li + 1] = min(best[li + 1], double(get_time_usec(t0)))
            }
        }
    }
    print("\n--- silu4 lane response (tt={tt}, image {tt}x{dimg}, {reps} reps, best of {rounds}) ---\n")
    print("  whole-array: {best[0] / double(reps) / 1000.0lf} ms/call\n")
    for (li, lanes in count(), lane_grid) {
        let ms = best[li + 1] / double(reps) / 1000.0lf
        print("  lanes={lanes}: {ms} ms/call ({best[0] / best[li + 1]}x)\n")
    }
}

[export]
def main() : int {
    var r <- parse_args(type<BenchArgs>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<BenchArgs>), "bench_gemm_smallm")
        return 1
    }
    let a <- r |> move_unwrap
    if (a.help) {
        print_help(get_command_info(type<BenchArgs>), "bench_gemm_smallm")
        return 0
    }
    let rounds = a.rounds > 0 ? a.rounds : 5
    if (!jit_enabled()) {
        print("WARNING: run with -jit - interpreted timings do not reflect the engine\n")
    }
    apply_box_profile_runtime()
    if (!a.hand) {
        // a Q8 model load selects the repack tier (arm64-gen/x64-gen) — mirror it, or the bench
        // times the wrong (hand) backend
        select_matmul_backend_for_load()
    }
    with_job_que() {
        setup_dasllama_jobque_()
        print("backend={active_kernel_backend()} lanes={get_dispatch_lanes()} TB={get_q8_token_block()} oversplit={get_q8_batch_chunks_per_job()} grain={get_matmul_min_chunk_rows()} grid2d={get_batch_grid_2d()}\n")
        let tt = a.tt > 0 ? int64(a.tt) : 138l
        if (a.tb) {
            run_tb_sweep(rounds)
        } elif (a.knobs) {
            run_knob_sweep(rounds)
        } elif (a.attn) {
            run_attn_stage(rounds, tt)
        } elif (a.ffn) {
            run_ffn_stage(rounds, tt)
        } elif (a.silu) {
            run_silu_sweep(rounds, tt)
        } else {
            run_map(rounds)
        }
    }
    return 0
}
