options gen2

// Isolated conformer conv-module bench (parakeet encoder, the two non-GEMM loops inside
// pk.conv_module): GLU (signal · σ(gate)) and depthwise conv1d k9 over time + inference
// BN + silu. Engine-verbatim baselines at real shapes vs loop-form variants
// ([vectorize]/[unroll] hints, float4 + exp4). Winner gets ported into the engine.
//
//   bin/daslang -jit modules/dasLLAMA/benchmarks/asr/conv_module_bench.das -- --glu
//   bin/daslang -jit modules/dasLLAMA/benchmarks/asr/conv_module_bench.das -- --dw
//
// tt = encoder rows (post 8x subsample): jfk ~138, gb1 ~2484. d = 1024, k = 9 (tdt-0.6b).
// Single-threaded on purpose — the engine wraps both loops in maybe_parallel_for over t;
// the loop-form decision is per-row and carries over.

require dasllama/dasllama_common // nolint:STYLE029,STYLE030 — jit_enabled() via the ?-conditional llvm_tune re-export; a direct require would hard-require dasLLVM
require dasllama/dasllama_math
require daslib/clargs
require math

let DD = 1024l  // n_state
let NK = 9l     // n_conv_kernel (kpad 4)

// exp4 fast-path clamp bounds (see EXP4_LO/EXP4_HI in dasllama_math)
let LO4 = float4(-80.0, -80.0, -80.0, -80.0)
let HI4 = float4(80.0, 80.0, 80.0, 80.0)

[CommandLineArgs]
struct BenchArgs {
    @clarg_short = "t"
    @clarg_doc = "Encoder rows (jfk ~138, gb1 ~2484; default 138)"
    tt : int

    @clarg_short = "r"
    @clarg_doc = "Best-of rounds (default 40)"
    rounds : int

    @clarg_doc = "Run only the GLU variants"
    glu : bool

    @clarg_doc = "Run only the dwconv/BN/silu variants"
    dw : bool

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

// ===== GLU: xb[t][j] = h[t][j] * σ(h[t][d + j]) =====

// engine-verbatim (dasllama_parakeet conv module, GLU dispatch body)
def glu_cur(var out : array<float>; h : array<float>; d, tt : int64) {
    unsafe {
        let hp = addr<float const?>(h[0])
        var op = addr(out[0])
        for (t in range64(tt)) {
            for (j in range64(d)) {
                let sg = hp[t * 2l * d + j]
                let gt = hp[t * 2l * d + d + j]
                op[t * d + j] = sg * (1.0 / (1.0 + exp(-gt)))
            }
        }
    }
}

// same loop, LLVM hints on the channel loop
def glu_hint(var out : array<float>; h : array<float>; d, tt : int64) {
    unsafe {
        let hp = addr<float const?>(h[0])
        var op = addr(out[0])
        for (t in range64(tt)) {
            for [vectorize, vectorize_width = 8] (j in range64(d)) {
                let sg = hp[t * 2l * d + j]
                let gt = hp[t * 2l * d + d + j]
                op[t * d + j] = sg * (1.0 / (1.0 + exp(-gt)))
            }
        }
    }
}

// float4 with exp4 (swiglu4 idiom): σ(g) = 1/(1+exp4(-g)), clamped to the fast-path range
def glu_exp4(var out : array<float>; h : array<float>; d, tt : int64) {
    let one = float4(1.0, 1.0, 1.0, 1.0)
    unsafe {
        let hp = addr<float const?>(h[0])
        var op = addr(out[0])
        let n4 = d / 4l
        for (t in range64(tt)) {
            let sgp = reinterpret<float4 const?>(hp + t * 2l * d)
            let gtp = reinterpret<float4 const?>(hp + t * 2l * d + d)
            var o4 = reinterpret<float4?>(op + t * d)
            for (k in range64(n4)) {
                let arg = clamp(-gtp[k], LO4, HI4)
                o4[k] = sgp[k] / (one + exp4(arg))
            }
            for (j in range64(n4 * 4l, d)) {   // tail (d % 4 != 0 models only)
                let sg = hp[t * 2l * d + j]
                let gt = hp[t * 2l * d + d + j]
                op[t * d + j] = sg * (1.0 / (1.0 + exp(-gt)))
            }
        }
    }
}

// exp4 form + unroll hint on the float4 loop (latency-chain check)
def glu_exp4_u2(var out : array<float>; h : array<float>; d, tt : int64) {
    let one = float4(1.0, 1.0, 1.0, 1.0)
    unsafe {
        let hp = addr<float const?>(h[0])
        var op = addr(out[0])
        let n4 = d / 4l
        for (t in range64(tt)) {
            let sgp = reinterpret<float4 const?>(hp + t * 2l * d)
            let gtp = reinterpret<float4 const?>(hp + t * 2l * d + d)
            var o4 = reinterpret<float4?>(op + t * d)
            for [unroll_count = 2] (k in range64(n4)) {
                let arg = clamp(-gtp[k], LO4, HI4)
                o4[k] = sgp[k] / (one + exp4(arg))
            }
            for (j in range64(n4 * 4l, d)) {
                let sg = hp[t * 2l * d + j]
                let gt = hp[t * 2l * d + d + j]
                op[t * d + j] = sg * (1.0 / (1.0 + exp(-gt)))
            }
        }
    }
}

// ===== dwconv k9 over time + inference BN + silu =====
// out[t][j] = silu(bn(Σ_k w[j][k] · x[t+k-4][j]))

// engine-verbatim (runtime nk, channel-major weights [d][nk])
def dwm_cur(var out : array<float>; x, w9, bm, bv, bw, bb : array<float>; d, tt : int64) {
    let nk = NK
    let kpad = (nk - 1l) / 2l
    unsafe {
        var op = addr(out[0])
        let xp = addr<float const?>(x[0])
        let wp = addr<float const?>(w9[0])
        let bmp = addr<float const?>(bm[0])
        let bvp = addr<float const?>(bv[0])
        let bwp = addr<float const?>(bw[0])
        let bbp = addr<float const?>(bb[0])
        for (t in range64(tt)) {
            for (j in range64(d)) {
                var acc = 0.0
                for (k in range64(nk)) {
                    let src = t + k - kpad
                    if (src >= 0l && src < tt) {
                        acc += wp[j * nk + k] * xp[src * d + j]
                    }
                }
                let bn = (acc - bmp[j]) / sqrt(bvp[j])
                let bv2 = bn * bwp[j] + bbp[j]
                op[t * d + j] = bv2 * (1.0 / (1.0 + exp(-bv2)))
            }
        }
    }
}

// same structure, nk specialized to the literal 9 so [unroll_full] can fire, plus
// [vectorize] on the channel loop (weights stay engine-layout [d][9])
def dwm_hint(var out : array<float>; x, w9, bm, bv, bw, bb : array<float>; d, tt : int64) {
    unsafe {
        var op = addr(out[0])
        let xp = addr<float const?>(x[0])
        let wp = addr<float const?>(w9[0])
        let bmp = addr<float const?>(bm[0])
        let bvp = addr<float const?>(bv[0])
        let bwp = addr<float const?>(bw[0])
        let bbp = addr<float const?>(bb[0])
        for (t in range64(tt)) {
            for [vectorize, vectorize_width = 8] (j in range64(d)) {
                var acc = 0.0
                for [unroll_full] (k in range64(9l)) {
                    let src = t + k - 4l
                    if (src >= 0l && src < tt) {
                        acc += wp[j * 9l + k] * xp[src * d + j]
                    }
                }
                let bn = (acc - bmp[j]) / sqrt(bvp[j])
                let bv2 = bn * bwp[j] + bbp[j]
                op[t * d + j] = bv2 * (1.0 / (1.0 + exp(-bv2)))
            }
        }
    }
}

// float4: tap-major weights [9][d] (pk_conv_tap_major precedent), interior/border t split,
// BN folded to per-channel scale/shift (y = acc·sc + sh), silu via exp4 — the conv_sub
// dw-winner recipe applied to the 1D k9 form
def dwm_f4(var out : array<float>; x, wt, bm, bv, bw, bb : array<float>; d, tt : int64) {
    var sc : array<float>
    var sh : array<float>
    sized(sc, d)
    sized(sh, d)
    let one = float4(1.0, 1.0, 1.0, 1.0)
    unsafe {
        var op = addr(out[0])
        let xp = addr<float const?>(x[0])
        let wp = addr<float const?>(wt[0])
        let bmp = addr<float const?>(bm[0])
        let bvp = addr<float const?>(bv[0])
        let bwp = addr<float const?>(bw[0])
        let bbp = addr<float const?>(bb[0])
        var scp = addr(sc[0])
        var shp = addr(sh[0])
        for (j in range64(d)) {
            let s = bwp[j] / sqrt(bvp[j])
            scp[j] = s
            shp[j] = bbp[j] - bmp[j] * s
        }
        if (tt > 8l) {   // interior rows: all 9 taps in range
            for (t in range64(4l, tt - 4l)) {
                var j = 0l
                while (j < d) {
                    let base = (t - 4l) * d + j
                    var acc = (*reinterpret<float4 const?>(xp + base)) * (*reinterpret<float4 const?>(wp + j))
                    acc = mad(*reinterpret<float4 const?>(xp + base + d), *reinterpret<float4 const?>(wp + d + j), acc)
                    acc = mad(*reinterpret<float4 const?>(xp + base + 2l * d), *reinterpret<float4 const?>(wp + 2l * d + j), acc)
                    acc = mad(*reinterpret<float4 const?>(xp + base + 3l * d), *reinterpret<float4 const?>(wp + 3l * d + j), acc)
                    acc = mad(*reinterpret<float4 const?>(xp + base + 4l * d), *reinterpret<float4 const?>(wp + 4l * d + j), acc)
                    acc = mad(*reinterpret<float4 const?>(xp + base + 5l * d), *reinterpret<float4 const?>(wp + 5l * d + j), acc)
                    acc = mad(*reinterpret<float4 const?>(xp + base + 6l * d), *reinterpret<float4 const?>(wp + 6l * d + j), acc)
                    acc = mad(*reinterpret<float4 const?>(xp + base + 7l * d), *reinterpret<float4 const?>(wp + 7l * d + j), acc)
                    acc = mad(*reinterpret<float4 const?>(xp + base + 8l * d), *reinterpret<float4 const?>(wp + 8l * d + j), acc)
                    let bv4 = mad(acc, *reinterpret<float4 const?>(scp + j), *reinterpret<float4 const?>(shp + j))
                    let arg = clamp(-bv4, LO4, HI4)
                    *reinterpret<float4?>(op + t * d + j) = bv4 / (one + exp4(arg))
                    j += 4l
                }
            }
        }
        for (t in range64(tt)) {   // border rows (8 total): guarded taps, same folded BN
            if (tt > 8l && t >= 4l && t < tt - 4l) {
                continue
            }
            for (j in range64(d)) {
                var acc = 0.0
                for (k in range64(9l)) {
                    let src = t + k - 4l
                    if (src >= 0l && src < tt) {
                        acc += wp[k * d + j] * xp[src * d + j]
                    }
                }
                let bv2 = acc * scp[j] + shp[j]
                op[t * d + j] = bv2 * (1.0 / (1.0 + exp(-bv2)))
            }
        }
    }
    delete sc
    delete sh
}

// ===== bench plumbing =====

def sized(var a : array<float>; n : int64) {
    a |> reserve(int(n))
    a |> resize(int(n))
}

def fill_sin(var a : array<float>; scale, step : float) {
    for (i, v in count(), a) {
        v = scale * sin(float(i) * step)
    }
}

def fill_pos(var a : array<float>; base, scale, step : float) {
    for (i, v in count(), a) {
        v = base + scale * sin(float(i) * step)
    }
}

def gate(name : string; got, want : array<float>; n : int64) : bool {
    var maxrel = 0.0lf
    var exact = true
    for (i in range64(n)) {
        let w = double(want[i])
        maxrel = max(maxrel, abs(double(got[i]) - w) / (abs(w) + 1.0lf))
        exact &&= got[i] == want[i]
    }
    let verdict = exact ? "bit-exact" : "maxrel {maxrel}"
    print("  gate {name}: {verdict}\n")
    if (maxrel > 1.0e-5lf) {
        print("  REJECTED: {name} diverges beyond tolerance\n")
        return false
    }
    return true
}

def bench_glu(tt : int64; rounds : int) {
    let d = DD
    print("\n--- GLU variants: {tt} rows x {d}ch ---\n")
    var h : array<float>
    sized(h, tt * 2l * d)
    fill_sin(h, 2.0, 0.171)
    var out_cur : array<float>
    var out_hint : array<float>
    var out_e4 : array<float>
    var out_u2 : array<float>
    sized(out_cur, tt * d)
    sized(out_hint, tt * d)
    sized(out_e4, tt * d)
    sized(out_u2, tt * d)

    glu_cur(out_cur, h, d, tt)
    glu_hint(out_hint, h, d, tt)
    glu_exp4(out_e4, h, d, tt)
    glu_exp4_u2(out_u2, h, d, tt)
    var ok = gate("hint", out_hint, out_cur, tt * d)
    ok &&= gate("exp4", out_e4, out_cur, tt * d)
    ok &&= gate("exp4_u2", out_u2, out_cur, tt * d)
    if (!ok) {
        return
    }

    var best_cur = 1.0e30lf
    var best_hint = 1.0e30lf
    var best_e4 = 1.0e30lf
    var best_u2 = 1.0e30lf
    for (_round in range(rounds)) {
        var t0 = ref_time_ticks()
        glu_cur(out_cur, h, d, tt)
        best_cur = min(best_cur, double(get_time_usec(t0)))
        t0 = ref_time_ticks()
        glu_hint(out_hint, h, d, tt)
        best_hint = min(best_hint, double(get_time_usec(t0)))
        t0 = ref_time_ticks()
        glu_exp4(out_e4, h, d, tt)
        best_e4 = min(best_e4, double(get_time_usec(t0)))
        t0 = ref_time_ticks()
        glu_exp4_u2(out_u2, h, d, tt)
        best_u2 = min(best_u2, double(get_time_usec(t0)))
    }
    print("  cur:     {best_cur / 1000.0lf} ms\n")
    print("  hint:    {best_hint / 1000.0lf} ms  ({best_cur / best_hint}x)\n")
    print("  exp4:    {best_e4 / 1000.0lf} ms  ({best_cur / best_e4}x)\n")
    print("  exp4_u2: {best_u2 / 1000.0lf} ms  ({best_cur / best_u2}x)\n")
}

def bench_dwm(tt : int64; rounds : int) {
    let d = DD
    print("\n--- dwconv+BN+silu variants: {tt} rows x {d}ch, k9 ---\n")
    var x : array<float>
    var w9 : array<float>
    var wt : array<float>
    var bm : array<float>
    var bv : array<float>
    var bw : array<float>
    var bb : array<float>
    sized(x, tt * d)
    sized(w9, d * NK)
    sized(wt, NK * d)
    sized(bm, d)
    sized(bv, d)
    sized(bw, d)
    sized(bb, d)
    fill_sin(x, 2.0, 0.137)
    fill_sin(w9, 0.33, 0.173)
    fill_sin(bm, 0.2, 0.31)
    fill_pos(bv, 0.6, 0.4, 0.23)   // variance stays positive
    fill_pos(bw, 1.0, 0.3, 0.19)
    fill_sin(bb, 0.1, 0.29)
    for (c in range64(d)) {   // tap-major repack (load-time in the engine)
        for (tap in range64(NK)) {
            wt[int(tap * d + c)] = w9[int(c * NK + tap)]
        }
    }
    var out_cur : array<float>
    var out_hint : array<float>
    var out_f4 : array<float>
    sized(out_cur, tt * d)
    sized(out_hint, tt * d)
    sized(out_f4, tt * d)

    dwm_cur(out_cur, x, w9, bm, bv, bw, bb, d, tt)
    dwm_hint(out_hint, x, w9, bm, bv, bw, bb, d, tt)
    dwm_f4(out_f4, x, wt, bm, bv, bw, bb, d, tt)
    var ok = gate("hint", out_hint, out_cur, tt * d)
    ok &&= gate("f4", out_f4, out_cur, tt * d)
    if (!ok) {
        return
    }

    var best_cur = 1.0e30lf
    var best_hint = 1.0e30lf
    var best_f4 = 1.0e30lf
    for (_round in range(rounds)) {
        var t0 = ref_time_ticks()
        dwm_cur(out_cur, x, w9, bm, bv, bw, bb, d, tt)
        best_cur = min(best_cur, double(get_time_usec(t0)))
        t0 = ref_time_ticks()
        dwm_hint(out_hint, x, w9, bm, bv, bw, bb, d, tt)
        best_hint = min(best_hint, double(get_time_usec(t0)))
        t0 = ref_time_ticks()
        dwm_f4(out_f4, x, wt, bm, bv, bw, bb, d, tt)
        best_f4 = min(best_f4, double(get_time_usec(t0)))
    }
    print("  cur:  {best_cur / 1000.0lf} ms\n")
    print("  hint: {best_hint / 1000.0lf} ms  ({best_cur / best_hint}x)\n")
    print("  f4:   {best_f4 / 1000.0lf} ms  ({best_cur / best_f4}x)\n")
}

[export]
def main() : int {
    var r <- parse_args(type<BenchArgs>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<BenchArgs>), "conv_module_bench")
        return 1
    }
    let a <- r |> move_unwrap
    if (a.help) {
        print_help(get_command_info(type<BenchArgs>), "conv_module_bench")
        return 0
    }
    let tt = a.tt > 0 ? int64(a.tt) : 138l
    let rounds = a.rounds > 0 ? a.rounds : 40
    if (!jit_enabled()) {
        print("WARNING: run with -jit - interpreted timings do not reflect the engine\n")
    }
    print("conv_module bench: tt={tt} d={DD} k={NK}, {rounds} rounds\n")
    if (a.glu || !a.dw) {
        bench_glu(tt, rounds)
    }
    if (a.dw || !a.glu) {
        bench_dwm(tt, rounds)
    }
    return 0
}
