options gen2
options _dasllama_internal = true

// Isolated conv-subsampling bench (parakeet encoder pre-stage). Part 1 of the conv_sub
// stage pass: split the single `pk.conv_subsample` profiler bucket into its sub-stages —
// engine-verbatim loop forms at real shapes, synthetic data, best-of-rounds per stage.
// Loop-form variants get added per sub-stage as they earn a pass (one bad loop at a time).
//
//   bin/daslang -jit modules/dasLLAMA/benchmarks/asr/conv_sub_bench.das -- --frames 1101
//
// frames = mel rows: jfk (11 s) ~ 1101, gb1 (3.3 min) ~ 19821. The q8 1x1 / pre GEMMs
// mirror the shipped q8 load; the c0 GEMM stays fp32 like the engine (conv taps are fp32).

require dasllama/dasllama_common // [init]-only: registers the q8 kernel backends (arm64-gen/sdot + portable)
require dasllama/dasllama_math
require dasllama/dasllama_audio
require dasllama/dasllama_convert
require daslib/jobque_boost
require daslib/clargs
require math

let MEL = 128l      // parakeet-tdt-0.6b n_mel
let NCH = 256l      // n_sub_ch
let DSTATE = 1024l  // n_state

[CommandLineArgs]
struct BenchArgs {
    @clarg_short = "f"
    @clarg_doc = "Mel frame count (jfk ~1101, gb1 ~19821; default 1101)"
    frames : int

    @clarg_short = "r"
    @clarg_doc = "Best-of rounds (default 12)"
    rounds : int

    @clarg_doc = "Run the dwconv loop-form variants head-to-head instead of the breakdown"
    dw : bool

    @clarg_doc = "Run the c0 conv loop-form variants head-to-head instead of the breakdown"
    c0 : bool

    @clarg_doc = "Run the permute + bias/relu epilogue variants instead of the breakdown"
    tail : bool

    @clarg_doc = "Run the mel matvec variants instead of the breakdown"
    meldot : bool

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

// ===== engine-verbatim loop forms (pk_conv2d_s2 im2col half / pk_conv2d_dw_s2 / pk_relu / xf permute) =====

def im2col_cur(var col : array<float>; x : array<float>; iw, ih, cin : int64) {
    let ow = (iw - 1l) / 2l + 1l
    let oh = (ih - 1l) / 2l + 1l
    let kdim = cin * 9l
    for (oy in range64(oh)) {
        for (ox in range64(ow)) {
            let r = (oy * ow + ox) * kdim
            for (ic in range64(cin)) {
                for (kh in range64(3l)) {
                    let iy = oy * 2l - 1l + kh
                    for (kw in range64(3l)) {
                        let ix = ox * 2l - 1l + kw
                        col[r + (ic * 3l + kh) * 3l + kw] = (iy >= 0l && iy < ih && ix >= 0l && ix < iw
                            ? x[(iy * iw + ix) * cin + ic]
                            : 0.0)
                    }
                }
            }
        }
    }
}

def dw_cur(var out : array<float>; x : array<float>; w, b : array<float>; iw, ih, ch : int64) {
    let ow = (iw - 1l) / 2l + 1l
    let oh = (ih - 1l) / 2l + 1l
    for (oy in range64(oh)) {
        for (ox in range64(ow)) {
            let r = (oy * ow + ox) * ch
            for (c in range64(ch)) {
                var acc = 0.0
                for (kh in range64(3l)) {
                    let iy = oy * 2l - 1l + kh
                    if (iy >= 0l && iy < ih) {
                        for (kw in range64(3l)) {
                            let ix = ox * 2l - 1l + kw
                            if (ix >= 0l && ix < iw) {
                                acc += w[(c * 3l + kh) * 3l + kw] * x[(iy * iw + ix) * ch + c]
                            }
                        }
                    }
                }
                out[r + c] = acc + b[c]
            }
        }
    }
}

def relu_cur(var x : array<float>; n : int64) {
    for (i in range64(n)) {
        x[i] = max(x[i], 0.0)
    }
}

def permute_cur(var xf : array<float>; y6 : array<float>; tt, w3, nch : int64) {
    let feat = w3 * nch
    for (t in range64(tt)) {
        for (fr in range64(w3)) {
            for (c in range64(nch)) {
                xf[t * feat + fr + w3 * c] = y6[(t * w3 + fr) * nch + c]
            }
        }
    }
}

// ===== dwconv loop-form variants =====
// Same op as dw_cur; weight layouts differ per form (repack is load-time in the engine, so it
// stays outside the timed region here). Accumulation order (kh-major, kw-minor, bias last) is
// identical in every form — variants gate bit-exact against dw_cur.

// guarded single position, tap-major weights — the border fallback shared by unroll/f4 forms
def dw_pos_guarded(var op : float?; xp, wp, bp : float const?; oy, ox, iw, ih, ch, ow : int64) {
    let o = (oy * ow + ox) * ch
    unsafe {
        for (c in range64(ch)) {
            var acc = 0.0
            for (kh in range64(3l)) {
                let iy = oy * 2l - 1l + kh
                if (iy >= 0l && iy < ih) {
                    for (kw in range64(3l)) {
                        let ix = ox * 2l - 1l + kw
                        if (ix >= 0l && ix < iw) {
                            acc += wp[(kh * 3l + kw) * ch + c] * xp[(iy * iw + ix) * ch + c]
                        }
                    }
                }
            }
            op[o + c] = acc + bp[c]
        }
    }
}

// scalar, pointers hoisted, 9 taps explicit, interior/border split; original [c][9] weights
def dw_unroll(var out : array<float>; x : array<float>; w, b : array<float>; iw, ih, ch : int64) {
    let ow = (iw - 1l) / 2l + 1l
    let oh = (ih - 1l) / 2l + 1l
    let oy_end = (ih - 2l) / 2l + 1l   // last oy with iy=2*oy+1 in bounds, exclusive
    let ox_end = (iw - 2l) / 2l + 1l
    let rs = iw * ch
    unsafe {
        var op = addr(out[0])
        let xp = addr<float const?>(x[0])
        let wp = addr<float const?>(w[0])
        let bp = addr<float const?>(b[0])
        for (oy in range64(1l, oy_end)) {
            let r0 = ((oy * 2l - 1l) * iw - 1l) * ch   // tap (0,0) of ox=0's neighborhood
            for (c in range64(ch)) {
                let w0 = wp[c * 9l]
                let w1 = wp[c * 9l + 1l]
                let w2 = wp[c * 9l + 2l]
                let w3 = wp[c * 9l + 3l]
                let w4 = wp[c * 9l + 4l]
                let w5 = wp[c * 9l + 5l]
                let w6 = wp[c * 9l + 6l]
                let w7 = wp[c * 9l + 7l]
                let w8 = wp[c * 9l + 8l]
                let bb = bp[c]
                var xr0 = r0 + c + 2l * ch             // interior starts at ox=1
                var o = (oy * ow + 1l) * ch + c
                for (_ox in range64(1l, ox_end)) {
                    var acc = w0 * xp[xr0]
                    acc += w1 * xp[xr0 + ch]
                    acc += w2 * xp[xr0 + 2l * ch]
                    acc += w3 * xp[xr0 + rs]
                    acc += w4 * xp[xr0 + rs + ch]
                    acc += w5 * xp[xr0 + rs + 2l * ch]
                    acc += w6 * xp[xr0 + 2l * rs]
                    acc += w7 * xp[xr0 + 2l * rs + ch]
                    acc += w8 * xp[xr0 + 2l * rs + 2l * ch]
                    op[o] = acc + bb
                    xr0 += 2l * ch
                    o += ch
                }
            }
        }
        // borders via the guarded form — but it wants tap-major weights, so guard inline here
        for (oy in range64(oh)) {
            let interior_y = oy >= 1l && oy < oy_end
            for (ox in range64(ow)) {
                if (!interior_y || ox == 0l || ox >= ox_end) {
                    let o = (oy * ow + ox) * ch
                    for (c in range64(ch)) {
                        var acc = 0.0
                        for (kh in range64(3l)) {
                            let iy = oy * 2l - 1l + kh
                            if (iy >= 0l && iy < ih) {
                                for (kw in range64(3l)) {
                                    let ix = ox * 2l - 1l + kw
                                    if (ix >= 0l && ix < iw) {
                                        acc += wp[c * 9l + kh * 3l + kw] * xp[(iy * iw + ix) * ch + c]
                                    }
                                }
                            }
                        }
                        op[o + c] = acc + bp[c]
                    }
                }
            }
        }
    }
}

// float4 over channels (contiguous, the natural SIMD axis), tap-major [9][ch] weights,
// 9 taps explicit, interior/border split
def dw_f4(var out : array<float>; x : array<float>; wt, b : array<float>; iw, ih, ch : int64) {
    let ow = (iw - 1l) / 2l + 1l
    let oh = (ih - 1l) / 2l + 1l
    let oy_end = (ih - 2l) / 2l + 1l
    let ox_end = (iw - 2l) / 2l + 1l
    let rs = iw * ch
    unsafe {
        var op = addr(out[0])
        let xp = addr<float const?>(x[0])
        let wp = addr<float const?>(wt[0])
        let bp = addr<float const?>(b[0])
        for (oy in range64(1l, oy_end)) {
            for (ox in range64(1l, ox_end)) {
                let x0 = ((oy * 2l - 1l) * iw + ox * 2l - 1l) * ch
                let o = (oy * ow + ox) * ch
                var c = 0l
                while (c < ch) {
                    var acc = (*reinterpret<float4 const?>(xp + x0 + c)) * (*reinterpret<float4 const?>(wp + c))
                    acc = mad(*reinterpret<float4 const?>(xp + x0 + ch + c), *reinterpret<float4 const?>(wp + ch + c), acc)
                    acc = mad(*reinterpret<float4 const?>(xp + x0 + 2l * ch + c), *reinterpret<float4 const?>(wp + 2l * ch + c), acc)
                    acc = mad(*reinterpret<float4 const?>(xp + x0 + rs + c), *reinterpret<float4 const?>(wp + 3l * ch + c), acc)
                    acc = mad(*reinterpret<float4 const?>(xp + x0 + rs + ch + c), *reinterpret<float4 const?>(wp + 4l * ch + c), acc)
                    acc = mad(*reinterpret<float4 const?>(xp + x0 + rs + 2l * ch + c), *reinterpret<float4 const?>(wp + 5l * ch + c), acc)
                    acc = mad(*reinterpret<float4 const?>(xp + x0 + 2l * rs + c), *reinterpret<float4 const?>(wp + 6l * ch + c), acc)
                    acc = mad(*reinterpret<float4 const?>(xp + x0 + 2l * rs + ch + c), *reinterpret<float4 const?>(wp + 7l * ch + c), acc)
                    acc = mad(*reinterpret<float4 const?>(xp + x0 + 2l * rs + 2l * ch + c), *reinterpret<float4 const?>(wp + 8l * ch + c), acc)
                    *reinterpret<float4?>(op + o + c) = acc + *reinterpret<float4 const?>(bp + c)
                    c += 4l
                }
            }
        }
        for (oy in range64(oh)) {
            let interior_y = oy >= 1l && oy < oy_end
            for (ox in range64(ow)) {
                if (!interior_y || ox == 0l || ox >= ox_end) {
                    dw_pos_guarded(op, xp, wp, bp, oy, ox, iw, ih, ch, ow)
                }
            }
        }
    }
}

// ===== c0 (cin=1 conv2d k3 s2) fused direct form =====
// out[(oy*ow+ox)*cout + o] = relu(sum_tap x(iy,ix) * wt[tap*cout + o] + b[o]) — no col image,
// no separate bias/relu passes; the 9 input taps are scalars splatted per position, weights
// tap-major [9][cout] (9 KB — L1-resident). OOB taps contribute exact zeros in the im2col
// form, so skipping them is bit-identical there; per-channel tap order matches the dot.
def c0_fused_f4(var out : array<float>; x : array<float>; wt, b : array<float>; iw, ih, cout : int64) {
    let ow = (iw - 1l) / 2l + 1l
    let oh = (ih - 1l) / 2l + 1l
    unsafe {
        var op = addr(out[0])
        let xp = addr<float const?>(x[0])
        let wp = addr<float const?>(wt[0])
        let bp = addr<float const?>(b[0])
        let zero = float4(0.0)
        for (oy in range64(oh)) {
            let iy0 = oy * 2l - 1l
            for (ox in range64(ow)) {
                let ix0 = ox * 2l - 1l
                var xs : float[9]
                for (kh in range64(3l)) {   // 9 guarded scalar taps, hoisted out of the channel sweep
                    let iy = iy0 + kh
                    for (kw in range64(3l)) {
                        let ix = ix0 + kw
                        xs[kh * 3l + kw] = (iy >= 0l && iy < ih && ix >= 0l && ix < iw ? xp[iy * iw + ix] : 0.0)
                    }
                }
                let x0 = float4(xs[0], xs[0], xs[0], xs[0])
                let x1 = float4(xs[1], xs[1], xs[1], xs[1])
                let x2 = float4(xs[2], xs[2], xs[2], xs[2])
                let x3 = float4(xs[3], xs[3], xs[3], xs[3])
                let x4 = float4(xs[4], xs[4], xs[4], xs[4])
                let x5 = float4(xs[5], xs[5], xs[5], xs[5])
                let x6 = float4(xs[6], xs[6], xs[6], xs[6])
                let x7 = float4(xs[7], xs[7], xs[7], xs[7])
                let x8 = float4(xs[8], xs[8], xs[8], xs[8])
                let o = (oy * ow + ox) * cout
                var c = 0l
                while (c < cout) {
                    var acc = x0 * (*reinterpret<float4 const?>(wp + c))
                    acc = mad(x1, *reinterpret<float4 const?>(wp + cout + c), acc)
                    acc = mad(x2, *reinterpret<float4 const?>(wp + 2l * cout + c), acc)
                    acc = mad(x3, *reinterpret<float4 const?>(wp + 3l * cout + c), acc)
                    acc = mad(x4, *reinterpret<float4 const?>(wp + 4l * cout + c), acc)
                    acc = mad(x5, *reinterpret<float4 const?>(wp + 5l * cout + c), acc)
                    acc = mad(x6, *reinterpret<float4 const?>(wp + 6l * cout + c), acc)
                    acc = mad(x7, *reinterpret<float4 const?>(wp + 7l * cout + c), acc)
                    acc = mad(x8, *reinterpret<float4 const?>(wp + 8l * cout + c), acc)
                    acc += *reinterpret<float4 const?>(bp + c)
                    *reinterpret<float4?>(op + o + c) = max(acc, zero)
                    c += 4l
                }
            }
        }
    }
}

def bench_c0(ih : int64; rounds : int) {
    let iw = MEL
    let cout = NCH
    let ow = (iw - 1l) / 2l + 1l
    let oh = (ih - 1l) / 2l + 1l
    let rows = oh * ow
    print("\n--- c0 variants: {iw}x{ih} cin=1 -> {cout}ch, {rows} rows ---\n")
    var x : array<float>
    var w9 : array<float>   // im2col order [cout][9] (what the engine loads)
    var wt : array<float>   // tap-major [9][cout]
    var b : array<float>
    sized(x, ih * iw)
    sized(w9, cout * 9l)
    sized(wt, 9l * cout)
    sized(b, cout)
    fill_sin(x, 2.0, 0.137)
    fill_sin(w9, 0.5, 0.211)
    fill_sin(b, 0.1, 0.31)
    for (o in range64(cout)) {
        for (tap in range64(9l)) {
            wt[tap * cout + o] = w9[o * 9l + tap]
        }
    }
    var col : array<float>
    var out_cur : array<float>
    var out_f4 : array<float>
    sized(col, rows * 9l)
    sized(out_cur, rows * cout)
    sized(out_f4, rows * cout)

    im2col_cur(col, x, iw, ih, 1l)
    matmul_batch(out_cur, w9, col, 9l, cout, rows)
    add_bias_rows(out_cur, b, 0l, cout, rows)
    relu_cur(out_cur, rows * cout)
    c0_fused_f4(out_f4, x, wt, b, iw, ih, cout)
    var maxrel = 0.0lf
    for (i in range64(rows * cout)) {
        let want = double(out_cur[i])
        maxrel = max(maxrel, abs(double(out_f4[i]) - want) / (abs(want) + 1.0lf))
    }
    print("  gate: fused_f4 maxrel {maxrel}\n")
    if (maxrel > 1.0e-5lf) {
        print("  REJECTED: diverges beyond tolerance\n")
        return
    }

    var best_cur = 1.0e30lf
    var best_f4 = 1.0e30lf
    for (_round in range(rounds)) {
        var t0 = ref_time_ticks()
        im2col_cur(col, x, iw, ih, 1l)
        matmul_batch(out_cur, w9, col, 9l, cout, rows)
        add_bias_rows(out_cur, b, 0l, cout, rows)
        relu_cur(out_cur, rows * cout)
        best_cur = min(best_cur, double(get_time_usec(t0)))
        t0 = ref_time_ticks()
        c0_fused_f4(out_f4, x, wt, b, iw, ih, cout)
        best_f4 = min(best_f4, double(get_time_usec(t0)))
    }
    print("  cur (im2col+gemm+bias+relu, gemm threaded): {best_cur / 1000.0lf} ms\n")
    print("  fused_f4 (single-lane):                     {best_f4 / 1000.0lf} ms  ({best_cur / best_f4}x)\n")
}

// ===== permute + bias/relu epilogue variants =====

// pointer-hoisted, c-outer: writes a contiguous 16-float run per (t, c), reads stride-nch
def permute_ptr(var xf : array<float>; y6 : array<float>; tt, w3, nch : int64) {
    let feat = w3 * nch
    unsafe {
        var fp = addr(xf[0])
        let yp = addr<float const?>(y6[0])
        for (t in range64(tt)) {
            let yb = t * feat
            let fb = t * feat
            for (c in range64(nch)) {
                var o = fb + w3 * c
                var yi = yb + c
                for (_fr in range64(w3)) {
                    fp[o] = yp[yi]
                    o++
                    yi += nch
                }
            }
        }
    }
}

// float4 over c (contiguous reads), scatter 4 strided writes per group of 4 fr
def permute_f4(var xf : array<float>; y6 : array<float>; tt, w3, nch : int64) {
    let feat = w3 * nch
    unsafe {
        var fp = addr(xf[0])
        let yp = addr<float const?>(y6[0])
        for (t in range64(tt)) {
            let yb = t * feat
            let fb = t * feat
            for (fr in range64(w3)) {
                let yr = yb + fr * nch
                let fo = fb + fr
                var c = 0l
                while (c < nch) {
                    let v = *reinterpret<float4 const?>(yp + yr + c)
                    fp[fo + w3 * c] = v.x
                    fp[fo + w3 * (c + 1l)] = v.y
                    fp[fo + w3 * (c + 2l)] = v.z
                    fp[fo + w3 * (c + 3l)] = v.w
                    c += 4l
                }
            }
        }
    }
}

// fused bias+relu, float4 (replaces the add_bias_rows + relu passes after the q8 1x1s)
def bias_relu_f4(var y : array<float>; b : array<float>; d, npos : int64) {
    unsafe {
        var yp = addr(y[0])
        let bp = addr<float const?>(b[0])
        let zero = float4(0.0)
        for (p in range64(npos)) {
            let row = p * d
            var i = 0l
            while (i < d) {
                let v = (*reinterpret<float4 const?>(yp + row + i)) + (*reinterpret<float4 const?>(bp + i))
                *reinterpret<float4?>(yp + row + i) = max(v, zero)
                i += 4l
            }
        }
    }
}

def bench_tail(frames : int64; rounds : int) {   // nolint:STYLE038 — flat bench driver: setup, then one timed variant per stage
    let t1 = (frames - 1l) / 2l + 1l
    let t2 = (t1 - 1l) / 2l + 1l
    let tt = (t2 - 1l) / 2l + 1l
    let feat = 16l * NCH
    print("\n--- permute variants: tt={tt} [t][16][256] -> [t][4096] ---\n")
    var y6 : array<float>
    var xf_cur : array<float>
    var xf_v : array<float>
    sized(y6, tt * feat)
    sized(xf_cur, tt * feat)
    sized(xf_v, tt * feat)
    fill_sin(y6, 1.5, 0.171)
    permute_cur(xf_cur, y6, tt, 16l, NCH)
    permute_ptr(xf_v, y6, tt, 16l, NCH)
    var ok_ptr = true
    for (i in range64(tt * feat)) {
        ok_ptr &&= xf_v[i] == xf_cur[i]
    }
    permute_f4(xf_v, y6, tt, 16l, NCH)
    var ok_f4 = true
    for (i in range64(tt * feat)) {
        ok_f4 &&= xf_v[i] == xf_cur[i]
    }
    let g_ptr = ok_ptr ? "bit-exact" : "MISMATCH"
    let g_f4 = ok_f4 ? "bit-exact" : "MISMATCH"
    print("  gate: ptr {g_ptr}, f4 {g_f4}\n")
    var best_cur = 1.0e30lf
    var best_ptr = 1.0e30lf
    var best_f4 = 1.0e30lf
    for (_round in range(rounds)) {
        var t0 = ref_time_ticks()
        permute_cur(xf_cur, y6, tt, 16l, NCH)
        best_cur = min(best_cur, double(get_time_usec(t0)))
        t0 = ref_time_ticks()
        permute_ptr(xf_v, y6, tt, 16l, NCH)
        best_ptr = min(best_ptr, double(get_time_usec(t0)))
        t0 = ref_time_ticks()
        permute_f4(xf_v, y6, tt, 16l, NCH)
        best_f4 = min(best_f4, double(get_time_usec(t0)))
    }
    print("  cur: {best_cur / 1000.0lf} ms | ptr: {best_ptr / 1000.0lf} ms ({best_cur / best_ptr}x) | f4: {best_f4 / 1000.0lf} ms ({best_cur / best_f4}x)\n")

    print("\n--- bias+relu epilogue: d=256, npos={t2 * 32l} (c3 shape) ---\n")
    let npos = t2 * 32l
    var yy0 : array<float>
    var yy : array<float>
    var yv : array<float>
    var bb : array<float>
    sized(yy0, npos * NCH)
    sized(yy, npos * NCH)
    sized(yv, npos * NCH)
    sized(bb, NCH)
    fill_sin(yy0, 1.5, 0.211)
    fill_sin(bb, 0.1, 0.31)
    copy_from(yy, yy0)
    add_bias_rows(yy, bb, 0l, NCH, npos)
    relu_cur(yy, npos * NCH)
    copy_from(yv, yy0)
    bias_relu_f4(yv, bb, NCH, npos)
    var ok = true
    for (i in range64(npos * NCH)) {
        ok &&= yv[i] == yy[i]
    }
    let g_fus = ok ? "bit-exact" : "MISMATCH"
    print("  gate: fused_f4 {g_fus}\n")
    var best_sep = 1.0e30lf
    var best_fus = 1.0e30lf
    for (_round in range(rounds)) {
        copy_from(yy, yy0)
        var t0 = ref_time_ticks()
        add_bias_rows(yy, bb, 0l, NCH, npos)
        relu_cur(yy, npos * NCH)
        best_sep = min(best_sep, double(get_time_usec(t0)))
        copy_from(yv, yy0)
        t0 = ref_time_ticks()
        bias_relu_f4(yv, bb, NCH, npos)
        best_fus = min(best_fus, double(get_time_usec(t0)))
    }
    print("  bias+relu separate: {best_sep / 1000.0lf} ms | fused_f4: {best_fus / 1000.0lf} ms ({best_sep / best_fus}x)\n")
}

// ===== mel matvec variants (the per-frame filterbank dot in parakeet_log_mel) =====

// engine-verbatim: 4 scalar accumulators, double-summed, double tail, log at the end
def meldot_cur(var md : float?; fp, flt : float const?; n_mel, n_bins : int64; mel_eps : double) {
    unsafe {
        for (jm in range64(n_mel)) {
            let fr = jm * n_bins
            var s0 = 0.0
            var s1 = 0.0
            var s2 = 0.0
            var s3 = 0.0
            var k = 0l
            while (k + 3l < n_bins) {
                s0 += fp[k] * flt[fr + k]
                s1 += fp[k + 1l] * flt[fr + k + 1l]
                s2 += fp[k + 2l] * flt[fr + k + 2l]
                s3 += fp[k + 3l] * flt[fr + k + 3l]
                k += 4l
            }
            var sum = double((s0 + s1) + (s2 + s3))
            while (k < n_bins) {
                sum += double(fp[k] * flt[fr + k])
                k++
            }
            md[jm] = float(log(sum + mel_eps))
        }
    }
}

// tuned dot() per filter row
def meldot_dot(var md : float?; fp, flt : float const?; n_mel, n_bins : int64; mel_eps : double) {
    unsafe {
        for (jm in range64(n_mel)) {
            let sum = double(dot(flt + jm * n_bins, fp, n_bins))
            md[jm] = float(log(sum + mel_eps))
        }
    }
}

// float4 accumulator + scalar tail, horizontal add in double
def meldot_f4(var md : float?; fp, flt : float const?; n_mel, n_bins : int64; mel_eps : double) {
    let n4 = n_bins - n_bins % 4l
    unsafe {
        for (jm in range64(n_mel)) {
            let fr = jm * n_bins
            var acc = float4(0.0)
            var k = 0l
            while (k < n4) {
                acc = mad(*reinterpret<float4 const?>(fp + k), *reinterpret<float4 const?>(flt + fr + k), acc)
                k += 4l
            }
            var sum = double((acc.x + acc.y) + (acc.z + acc.w))
            while (k < n_bins) {
                sum += double(fp[k] * flt[fr + k])
                k++
            }
            md[jm] = float(log(sum + mel_eps))
        }
    }
}

def bench_meldot(rounds : int) {
    let n_mel = 128l
    let n_bins = 257l
    let frames = 1101l   // one jfk of matvecs per timed rep
    let mel_eps = 5.960464477539063e-08lf
    print("\n--- mel matvec variants: {n_mel} x {n_bins}, {frames} frames/rep ---\n")
    var fp : array<float>
    var flt : array<float>
    var md : array<float>
    sized(fp, n_bins)
    sized(flt, n_mel * n_bins)
    sized(md, n_mel * 3l)
    fill_sin(fp, 1.5, 0.171)
    for (v in fp) {
        v = v * v   // power spectrum: non-negative
    }
    fill_sin(flt, 0.5, 0.093)
    for (v in flt) {
        v = abs(v)   // filterbank weights: non-negative (log input stays positive)
    }
    unsafe {
        var mdp = addr(md[0])
        let fpp = addr<float const?>(fp[0])
        let flp = addr<float const?>(flt[0])
        meldot_cur(mdp, fpp, flp, n_mel, n_bins, mel_eps)
        meldot_dot(mdp + n_mel, fpp, flp, n_mel, n_bins, mel_eps)
        meldot_f4(mdp + 2l * n_mel, fpp, flp, n_mel, n_bins, mel_eps)
        var mr_dot = 0.0lf
        var mr_f4 = 0.0lf
        for (j in range64(n_mel)) {
            let want = double(md[j])
            mr_dot = max(mr_dot, abs(double(md[n_mel + j]) - want) / (abs(want) + 1.0lf))
            mr_f4 = max(mr_f4, abs(double(md[2l * n_mel + j]) - want) / (abs(want) + 1.0lf))
        }
        print("  gate (vs cur, LSB-class expected): dot maxrel {mr_dot}, f4 maxrel {mr_f4}\n")
        var best_cur = 1.0e30lf
        var best_dot = 1.0e30lf
        var best_f4 = 1.0e30lf
        for (_round in range(rounds)) {
            var t0 = ref_time_ticks()
            for (_f in range64(frames)) {
                meldot_cur(mdp, fpp, flp, n_mel, n_bins, mel_eps)
            }
            best_cur = min(best_cur, double(get_time_usec(t0)))
            t0 = ref_time_ticks()
            for (_f in range64(frames)) {
                meldot_dot(mdp, fpp, flp, n_mel, n_bins, mel_eps)
            }
            best_dot = min(best_dot, double(get_time_usec(t0)))
            t0 = ref_time_ticks()
            for (_f in range64(frames)) {
                meldot_f4(mdp, fpp, flp, n_mel, n_bins, mel_eps)
            }
            best_f4 = min(best_f4, double(get_time_usec(t0)))
        }
        print("  cur: {best_cur / 1000.0lf} ms | dot(): {best_dot / 1000.0lf} ms ({best_cur / best_dot}x) | f4: {best_f4 / 1000.0lf} ms ({best_cur / best_f4}x)\n")
    }
}

// ===== bench plumbing =====

def sized(var a : array<float>; n : int64) {
    a |> reserve(n)
    a |> resize(n)
}

def copy_from(var dst : array<float>; src : array<float>) {
    for (i, v in count(), src) {
        dst[i] = v
    }
}

def fill_sin(var a : array<float>; scale, step : float) {
    for (i, v in count(), a) {
        v = scale * sin(float(i) * step)
    }
}

// record stage i's elapsed time, return a fresh tick for the next stage
def lap(var best : array<double>; i : int; t0 : int64) : int64 {
    let us = double(get_time_usec(t0))
    if (us < best[i]) {
        best[i] = us
    }
    return ref_time_ticks()
}

def quantize_repacked(w : array<float>; n, d : int64; var q : array<int8>; var s : array<float>) {
    q |> resize(n * d)
    s |> resize(n * d / 32l)
    quantize_q8_0_into(w, n * d, q, s, 0l, 0l)
    repack_q8q8_weight(q, s, 0l, n, d)
}

def bench_dw(iw, ih : int64; rounds : int) {
    let ch = NCH
    let ow = (iw - 1l) / 2l + 1l
    let oh = (ih - 1l) / 2l + 1l
    print("\n--- dwconv variants: {iw}x{ih} x {ch}ch -> {ow}x{oh} ---\n")
    var x : array<float>
    var w9 : array<float>
    var wt : array<float>
    var b : array<float>
    sized(x, ih * iw * ch)
    sized(w9, ch * 9l)
    sized(wt, 9l * ch)
    sized(b, ch)
    fill_sin(x, 2.0, 0.171)
    fill_sin(w9, 0.33, 0.173)
    fill_sin(b, 0.1, 0.29)
    for (c in range64(ch)) {   // tap-major repack (load-time in the engine)
        for (tap in range64(9l)) {
            wt[tap * ch + c] = w9[c * 9l + tap]
        }
    }
    var out_cur : array<float>
    var out_u : array<float>
    var out_f4 : array<float>
    sized(out_cur, oh * ow * ch)
    sized(out_u, oh * ow * ch)
    sized(out_f4, oh * ow * ch)

    dw_cur(out_cur, x, w9, b, iw, ih, ch)
    dw_unroll(out_u, x, w9, b, iw, ih, ch)
    dw_f4(out_f4, x, wt, b, iw, ih, ch)
    var maxrel_u = 0.0lf
    var maxrel_f4 = 0.0lf
    var exact_u = true
    var exact_f4 = true
    for (i in range64(oh * ow * ch)) {
        let want = double(out_cur[i])
        let ru = abs(double(out_u[i]) - want) / (abs(want) + 1.0lf)
        let rf = abs(double(out_f4[i]) - want) / (abs(want) + 1.0lf)
        maxrel_u = max(maxrel_u, ru)
        maxrel_f4 = max(maxrel_f4, rf)
        exact_u &&= out_u[i] == out_cur[i]
        exact_f4 &&= out_f4[i] == out_cur[i]
    }
    let gate_u = exact_u ? "bit-exact" : "maxrel {maxrel_u}"
    let gate_f4 = exact_f4 ? "bit-exact" : "maxrel {maxrel_f4}"
    print("  gate: unroll {gate_u}, f4 {gate_f4}\n")
    if (maxrel_u > 1.0e-5lf || maxrel_f4 > 1.0e-5lf) {
        print("  REJECTED: variant diverges beyond tolerance\n")
        return
    }

    var best_cur = 1.0e30lf
    var best_u = 1.0e30lf
    var best_f4 = 1.0e30lf
    for (_round in range(rounds)) {
        var t0 = ref_time_ticks()
        dw_cur(out_cur, x, w9, b, iw, ih, ch)
        best_cur = min(best_cur, double(get_time_usec(t0)))
        t0 = ref_time_ticks()
        dw_unroll(out_u, x, w9, b, iw, ih, ch)
        best_u = min(best_u, double(get_time_usec(t0)))
        t0 = ref_time_ticks()
        dw_f4(out_f4, x, wt, b, iw, ih, ch)
        best_f4 = min(best_f4, double(get_time_usec(t0)))
    }
    print("  cur:    {best_cur / 1000.0lf} ms\n")
    print("  unroll: {best_u / 1000.0lf} ms  ({best_cur / best_u}x)\n")
    print("  f4:     {best_f4 / 1000.0lf} ms  ({best_cur / best_f4}x)\n")
}

def run_bench(frames : int64; rounds : int) {   // nolint:STYLE038 — flat bench driver: one buffer/weight declaration per stage, then the interleaved timed walk
    let t1 = (frames - 1l) / 2l + 1l
    let ww1 = (MEL - 1l) / 2l + 1l   // 64
    let t2 = (t1 - 1l) / 2l + 1l
    let ww2 = (ww1 - 1l) / 2l + 1l   // 32
    let t3 = (t2 - 1l) / 2l + 1l
    let ww3 = (ww2 - 1l) / 2l + 1l   // 16
    let tt = t3
    let feat = ww3 * NCH             // 4096
    let rows1 = t1 * ww1
    print("conv_sub bench: frames={frames} t1={t1} t2={t2} tt={tt} feat={feat}, {rounds} rounds, jobs={get_total_hw_jobs()}, jit={jit_enabled()}\n")

    // activations
    var mel : array<float>
    var col : array<float>
    var y0 : array<float>
    var y2 : array<float>
    var y3 : array<float>
    var y5 : array<float>
    var y6 : array<float>
    var xf : array<float>
    var xout : array<float>
    sized(mel, frames * MEL)
    sized(col, rows1 * 9l)
    sized(y0, rows1 * NCH)
    sized(y2, t2 * ww2 * NCH)
    sized(y3, t2 * ww2 * NCH)
    sized(y5, t3 * ww3 * NCH)
    sized(y6, t3 * ww3 * NCH)
    sized(xf, tt * feat)
    sized(xout, tt * DSTATE)
    fill_sin(mel, 2.0, 0.137)

    // requant scratch (sized for the largest consumer)
    let rqmax = max(t2 * ww2 * NCH, tt * feat)
    var xq : array<int8>
    var xs : array<float>
    xq |> resize(rqmax)
    xs |> resize(rqmax / 32l)

    // weights: c0/dw taps fp32 (like the engine); 1x1 + pre as repacked q8
    var cw0 : array<float>
    var cb0 : array<float>
    var cw2 : array<float>
    var cb2 : array<float>
    var cw3 : array<float>
    var cb3 : array<float>
    var cw5 : array<float>
    var cb5 : array<float>
    var cw6 : array<float>
    var cb6 : array<float>
    var wpre : array<float>
    var bpre : array<float>
    sized(cw0, NCH * 9l)
    sized(cb0, NCH)
    sized(cw2, NCH * 9l)
    sized(cb2, NCH)
    sized(cw3, NCH * NCH)
    sized(cb3, NCH)
    sized(cw5, NCH * 9l)
    sized(cb5, NCH)
    sized(cw6, NCH * NCH)
    sized(cb6, NCH)
    sized(wpre, DSTATE * feat)
    sized(bpre, DSTATE)
    fill_sin(cw0, 0.5, 0.211)
    fill_sin(cb0, 0.1, 0.31)
    fill_sin(cw2, 0.33, 0.173)
    fill_sin(cb2, 0.1, 0.29)
    fill_sin(cw3, 0.12, 0.191)
    fill_sin(cb3, 0.1, 0.23)
    fill_sin(cw5, 0.33, 0.157)
    fill_sin(cb5, 0.1, 0.27)
    fill_sin(cw6, 0.12, 0.181)
    fill_sin(cb6, 0.1, 0.21)
    fill_sin(wpre, 0.05, 0.149)
    fill_sin(bpre, 0.1, 0.19)

    var q3 : array<int8>
    var s3 : array<float>
    var q6 : array<int8>
    var s6 : array<float>
    var qpre : array<int8>
    var spre : array<float>
    quantize_repacked(cw3, NCH, NCH, q3, s3)
    quantize_repacked(cw6, NCH, NCH, q6, s6)
    quantize_repacked(wpre, feat, DSTATE, qpre, spre)

    let names <- [
        "c0.im2col       (t1*64 x 9 gather)",
        "c0.gemm fp32    (K=9 -> 256ch)",
        "c0.bias",
        "c0.relu",
        "c2.dw 3x3       ({ww1}x{t1} x 256ch)",
        "c3.requant",
        "c3.gemm q8      (256->256, {t2 * ww2} rows)",
        "c3.bias",
        "c3.relu",
        "c5.dw 3x3       ({ww2}x{t2} x 256ch)",
        "c6.requant",
        "c6.gemm q8      (256->256, {t3 * ww3} rows)",
        "c6.bias",
        "c6.relu",
        "permute         ([t][16][256] -> [t][4096])",
        "pre.requant",
        "pre.gemm q8     (4096->1024, {tt} rows)",
        "pre.bias"
    ]
    var best : array<double>
    best |> resize(length(names))
    for (b in best) {
        b = 1.0e30lf
    }

    for (_round in range(rounds)) {
        var t = ref_time_ticks()
        im2col_cur(col, mel, MEL, frames, 1l)
        t = lap(best, 0, t)
        matmul_batch(y0, cw0, col, 9l, NCH, rows1)
        t = lap(best, 1, t)
        add_bias_rows(y0, cb0, 0l, NCH, rows1)
        t = lap(best, 2, t)
        relu_cur(y0, rows1 * NCH)
        t = lap(best, 3, t)
        dw_cur(y2, y0, cw2, cb2, ww1, t1, NCH)
        t = lap(best, 4, t)
        asr_requant_rows_q8(y2, NCH, t2 * ww2, xq, xs)
        t = lap(best, 5, t)
        matmul_q8q8_batch(y3, q3, s3, xq, xs, NCH, NCH, t2 * ww2)
        t = lap(best, 6, t)
        add_bias_rows(y3, cb3, 0l, NCH, t2 * ww2)
        t = lap(best, 7, t)
        relu_cur(y3, t2 * ww2 * NCH)
        t = lap(best, 8, t)
        dw_cur(y5, y3, cw5, cb5, ww2, t2, NCH)
        t = lap(best, 9, t)
        asr_requant_rows_q8(y5, NCH, t3 * ww3, xq, xs)
        t = lap(best, 10, t)
        matmul_q8q8_batch(y6, q6, s6, xq, xs, NCH, NCH, t3 * ww3)
        t = lap(best, 11, t)
        add_bias_rows(y6, cb6, 0l, NCH, t3 * ww3)
        t = lap(best, 12, t)
        relu_cur(y6, t3 * ww3 * NCH)
        t = lap(best, 13, t)
        permute_cur(xf, y6, tt, ww3, NCH)
        t = lap(best, 14, t)
        asr_requant_rows_q8(xf, feat, tt, xq, xs)
        t = lap(best, 15, t)
        matmul_q8q8_batch(xout, qpre, spre, xq, xs, feat, DSTATE, tt)
        t = lap(best, 16, t)
        add_bias_rows(xout, bpre, 0l, DSTATE, tt)
        t = lap(best, 17, t)
    }

    var total = 0.0lf
    for (b in best) {
        total += b
    }
    print("\n--- conv_sub sub-stage breakdown (best of {rounds}) ---\n")
    for (i, nm in count(), names) {
        let ms = best[i] / 1000.0lf
        let pct = best[i] / total * 100.0lf
        print("  {nm}: {ms} ms  ({pct}%)\n")
    }
    print("  TOTAL: {total / 1000.0lf} ms   (checksum {xout[0]})\n")
}

[export]
def main() : int {
    var r <- parse_args(type<BenchArgs>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<BenchArgs>), "conv_sub_bench")
        return 1
    }
    let a <- r |> move_unwrap
    if (a.help) {
        print_help(get_command_info(type<BenchArgs>), "conv_sub_bench")
        return 0
    }
    let frames = a.frames > 0 ? int64(a.frames) : 1101l
    let rounds = a.rounds > 0 ? a.rounds : 12
    if (!jit_enabled()) {
        print("WARNING: run with -jit - interpreted timings do not reflect the engine\n")
    }
    apply_box_profile_runtime()
    with_job_que() {
        setup_dasllama_jobque_()
        let t1 = (frames - 1l) / 2l + 1l
        let t2 = (t1 - 1l) / 2l + 1l
        if (a.dw) {
            bench_dw(64l, t1, rounds)   // c2 shape
            bench_dw(32l, t2, rounds)   // c5 shape
        } elif (a.c0) {
            bench_c0(frames, rounds)
        } elif (a.tail) {
            bench_tail(frames, rounds)
        } elif (a.meldot) {
            bench_meldot(rounds)
        } else {
            run_bench(frames, rounds)
        }
    }
    return 0
}
