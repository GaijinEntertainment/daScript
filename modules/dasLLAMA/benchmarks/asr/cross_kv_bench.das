options gen2
options _dasllama_internal = true

// Isolated whisper cross-attention K/V scatter bench (whisper_cross_kv, dasllama_whisper):
// after each layer's ck/cv GEMM the token-major [ta][d] projection is scattered head-major —
// kx TRANSPOSED [hs][ta] (pre-scaled hs^-0.25), vx [ta][hs] per head. Engine-verbatim scalar
// baselines at real shapes vs f4 / blocked-transpose variants. Winner gets ported into the
// engine (threaded over t there — the loop-form decision is per-row and carries over).
//
//   bin/daslang -jit modules/dasLLAMA/benchmarks/asr/cross_kv_bench.das             (tiny: d=384 hh=6)
//   bin/daslang -jit modules/dasLLAMA/benchmarks/asr/cross_kv_bench.das -- --turbo  (d=1280 hh=20)
//
// ta = 1500 encoder positions (30 s window), hs = 64 on every whisper size.

require dasllama/dasllama_common // nolint:STYLE029,STYLE030,LINT019 — jit_enabled() via the ?-conditional llvm_tune re-export; a direct require would hard-require dasLLVM
require daslib/clargs
require math

let TA = 1500l

[CommandLineArgs]
struct BenchArgs {
    @clarg_doc = "turbo shape (d=1280, 20 heads; default tiny d=384, 6 heads)"
    turbo : bool

    @clarg_short = "r"
    @clarg_doc = "Best-of rounds (default 40)"
    rounds : int

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

// ===== kx: [ta][d] -> per-head TRANSPOSED [hs][ta], pre-scaled =====

// engine-verbatim (whisper_cross_kv kx scatter body, single layer)
def kx_cur(var kx : array<float>; tmp : array<float>; d, hh, ta : int64; kscale : float) {
    let hs = d / hh
    for (h in range64(hh)) {
        let base = h * hs * ta
        for (t in range64(ta)) {
            for (j in range64(hs)) {
                kx[base + j * ta + t] = tmp[t * d + h * hs + j] * kscale
            }
        }
    }
}

// dest-major: each K^T row j filled by a quad gather over t (4 strided loads, one f4 store)
def kx_f4t(var kx : array<float>; tmp : array<float>; d, hh, ta : int64; kscale : float) {
    let hs = d / hh
    unsafe {
        let sp = addr<float const?>(tmp[0])
        var op = addr(kx[0])
        for (h in range64(hh)) {
            let hoff = h * hs
            let base = h * hs * ta
            for (j in range64(hs)) {
                var dst = op + base + j * ta
                let src = sp + hoff + j
                var t = 0l
                while (t + 3l < ta) {
                    *reinterpret<float4?>(dst + t) = float4(src[t * d], src[(t + 1l) * d],
                        src[(t + 2l) * d], src[(t + 3l) * d]) * kscale
                    t += 4l
                }
                while (t < ta) {
                    dst[t] = src[t * d] * kscale
                    t++
                }
            }
        }
    }
}

// 4x4 register-blocked transpose, t-block outer so the 4 source rows stream contiguously
def kx_blk4(var kx : array<float>; tmp : array<float>; d, hh, ta : int64; kscale : float) {
    let hs = d / hh
    unsafe {
        let sp = addr<float const?>(tmp[0])
        var op = addr(kx[0])
        var t = 0l
        while (t + 3l < ta) {
            let r0 = sp + t * d
            let r1 = sp + (t + 1l) * d
            let r2 = sp + (t + 2l) * d
            let r3 = sp + (t + 3l) * d
            for (h in range64(hh)) {
                let hoff = h * hs
                let base = h * hs * ta + t
                var j = 0l
                while (j < hs) {
                    let a = (*reinterpret<float4 const?>(r0 + hoff + j)) * kscale
                    let b = (*reinterpret<float4 const?>(r1 + hoff + j)) * kscale
                    let c = (*reinterpret<float4 const?>(r2 + hoff + j)) * kscale
                    let e = (*reinterpret<float4 const?>(r3 + hoff + j)) * kscale
                    *reinterpret<float4?>(op + base + j * ta) = float4(a.x, b.x, c.x, e.x)
                    *reinterpret<float4?>(op + base + (j + 1l) * ta) = float4(a.y, b.y, c.y, e.y)
                    *reinterpret<float4?>(op + base + (j + 2l) * ta) = float4(a.z, b.z, c.z, e.z)
                    *reinterpret<float4?>(op + base + (j + 3l) * ta) = float4(a.w, b.w, c.w, e.w)
                    j += 4l
                }
            }
            t += 4l
        }
        while (t < ta) {   // tail rows, scalar (ta=1500 is 4-aligned; generic safety)
            for (h in range64(hh)) {
                for (j in range64(hs)) {
                    op[h * hs * ta + j * ta + t] = sp[t * d + h * hs + j] * kscale
                }
            }
            t++
        }
    }
}

// ===== vx: [ta][d] -> per-head [ta][hs] =====

// engine-verbatim (whisper_cross_kv vx scatter body, single layer)
def vx_cur(var vx : array<float>; tmp : array<float>; d, hh, ta : int64) {
    let hs = d / hh
    for (h in range64(hh)) {
        let base = h * ta * hs
        for (t in range64(ta)) {
            for (j in range64(hs)) {
                vx[base + t * hs + j] = tmp[t * d + h * hs + j]
            }
        }
    }
}

// t-major f4 copy: each source row streams once, hh contiguous hs-chunks fan out
def vx_f4(var vx : array<float>; tmp : array<float>; d, hh, ta : int64) {
    let hs = d / hh
    unsafe {
        let sp = addr<float const?>(tmp[0])
        var op = addr(vx[0])
        for (t in range64(ta)) {
            let src = sp + t * d
            for (h in range64(hh)) {
                var dst = op + h * ta * hs + t * hs
                let hoff = h * hs
                var j = 0l
                while (j + 3l < hs) {
                    *reinterpret<float4?>(dst + j) = *reinterpret<float4 const?>(src + hoff + j)
                    j += 4l
                }
                while (j < hs) {
                    dst[j] = src[hoff + j]
                    j++
                }
            }
        }
    }
}

// ===== harness =====

def sized(var a : array<float>; n : int64) {
    a |> reserve(n)
    a |> resize(n)
}

def fill_sin(var a : array<float>; scale, step : float) {
    for (i, v in count(), a) {
        v = scale * sin(float(i) * step)
    }
}

def gate(name : string; got, want : array<float>; n : int64) : bool {
    var exact = true
    for (i in range64(n)) {
        exact &&= got[i] == want[i]
    }
    print("  gate {name}: {exact ? "bit-exact" : "DIVERGES"}\n")
    return exact
}

[export]
def main() : int {
    var r <- parse_args(type<BenchArgs>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<BenchArgs>), "cross_kv_bench")
        return 1
    }
    let a <- r |> move_unwrap
    if (a.help) {
        print_help(get_command_info(type<BenchArgs>), "cross_kv_bench")
        return 0
    }
    let d = a.turbo ? 1280l : 384l
    let hh = a.turbo ? 20l : 6l
    let rounds = a.rounds > 0 ? a.rounds : 40
    if (!jit_enabled()) {
        print("WARNING: run with -jit - interpreted timings do not reflect the engine\n")
    }
    let kscale = 1.0 / sqrt(sqrt(float(d / hh)))
    print("cross_kv scatter bench: ta={TA} d={d} heads={hh} hs={d / hh}, {rounds} rounds\n")

    var tmp : array<float>
    sized(tmp, TA * d)
    fill_sin(tmp, 2.0, 0.171)
    var kx_c : array<float>
    var kx_a : array<float>
    var kx_b : array<float>
    var vx_c : array<float>
    var vx_a : array<float>
    sized(kx_c, TA * d)
    sized(kx_a, TA * d)
    sized(kx_b, TA * d)
    sized(vx_c, TA * d)
    sized(vx_a, TA * d)

    kx_cur(kx_c, tmp, d, hh, TA, kscale)
    kx_f4t(kx_a, tmp, d, hh, TA, kscale)
    kx_blk4(kx_b, tmp, d, hh, TA, kscale)
    vx_cur(vx_c, tmp, d, hh, TA)
    vx_f4(vx_a, tmp, d, hh, TA)
    var ok = gate("kx_f4t", kx_a, kx_c, TA * d)
    ok &&= gate("kx_blk4", kx_b, kx_c, TA * d)
    ok &&= gate("vx_f4", vx_a, vx_c, TA * d)
    if (!ok) {
        print("REJECTED: variant diverges - timings skipped\n")
        return 1
    }

    var best_kc = 1e30lf
    var best_ka = 1e30lf
    var best_kb = 1e30lf
    var best_vc = 1e30lf
    var best_va = 1e30lf
    for (_r in range(rounds)) {
        var t0 = ref_time_ticks()
        kx_cur(kx_c, tmp, d, hh, TA, kscale)
        best_kc = min(best_kc, double(get_time_usec(t0)))
        t0 = ref_time_ticks()
        kx_f4t(kx_a, tmp, d, hh, TA, kscale)
        best_ka = min(best_ka, double(get_time_usec(t0)))
        t0 = ref_time_ticks()
        kx_blk4(kx_b, tmp, d, hh, TA, kscale)
        best_kb = min(best_kb, double(get_time_usec(t0)))
        t0 = ref_time_ticks()
        vx_cur(vx_c, tmp, d, hh, TA)
        best_vc = min(best_vc, double(get_time_usec(t0)))
        t0 = ref_time_ticks()
        vx_f4(vx_a, tmp, d, hh, TA)
        best_va = min(best_va, double(get_time_usec(t0)))
    }
    print("\n--- kx (transpose + scale, one layer) ---\n")
    print("  cur:  {best_kc / 1000.0lf} ms\n")
    print("  f4t:  {best_ka / 1000.0lf} ms  ({best_kc / best_ka}x)\n")
    print("  blk4: {best_kb / 1000.0lf} ms  ({best_kc / best_kb}x)\n")
    print("\n--- vx (copy, one layer) ---\n")
    print("  cur: {best_vc / 1000.0lf} ms\n")
    print("  f4:  {best_va / 1000.0lf} ms  ({best_vc / best_va}x)\n")
    return 0
}
