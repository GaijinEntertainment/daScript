options gen2

// Per-box kernel tuner — sweeps the FULL grid (20 perms) for every [tuned] kernel (the float
// elementwise set, softmax, rmsnorm, the quantized dots, the activation requant, the NEOX rope
// table leaf, the fp32 GEMM tile, and the arm64 laneq4x4 tile) and writes the winning permutation
// of each to box_profile.json. [dasllama_grid] emits, cross-module, each template's
// <name>__<suffix> variant clones plus a <name>_template_variants() fn-ptr registry; the bench runs
// the registry INTERLEAVED under the JIT (loop hints only bite there — see dasllama_tune),
// correctness-gates every variant vs an f64 reference (exact equality for the requant), and reports
// each variant's time as a DELTA vs that kernel's SHIPPED fallback perm so a "win" that is really
// noise is obvious.
//
// Run:  bin/daslang -jit modules/dasLLAMA/harness/tune_kernels.das
//   then rebuild/rerun the model with -jit to pick up box_profile.json (it re-keys the JIT DLL cache).

require dasllama/dasllama_math   // nolint:STYLE030 — side-effect: loads the *_template kernels for the cross-module scan
require dasllama/dasllama_math_default   // nolint:STYLE030 — dot_q8q8_template lives here
require dasllama/dasllama_math_aarch64_neon   // laneq4x4 template + backend registration (pin for the repack)
require dasllama/dasllama_quant  // quantize templates + the Q4/Q8 reference quantizers
require dasllama/dasllama_common // the runtime-knob getters (the profile's "runtime" section snapshots them)
require dasllama/dasllama_tune   // [dasllama_grid] emitter
require llvm/daslib/aarch64_neon // nolint:STYLE030 — sdot4_laneq (referenced by the cloned laneq template bodies)
require math                     // mad (referenced by the cloned template bodies)
require daslib/json
require daslib/json_boost
require daslib/fio

// One [dasllama_grid] per kernel: emits <src>__<suffix> for all 20 perms + <src>_variants().
[dasllama_grid(src = "dot_template")]
struct GridDot {}
[dasllama_grid(src = "axpy_template")]
struct GridAxpy {}
[dasllama_grid(src = "add_inplace_template")]
struct GridAdd {}
[dasllama_grid(src = "mul_inplace_template")]
struct GridMul {}
[dasllama_grid(src = "scale_inplace_template")]
struct GridScale {}
[dasllama_grid(src = "copy_floats_template")]
struct GridCopy {}
[dasllama_grid(src = "softmax_template")]
struct GridSoftmax {}
[dasllama_grid(src = "rmsnorm_template")]
struct GridRms {}
[dasllama_grid(src = "dot_q4_template")]
struct GridDotQ4 {}
[dasllama_grid(src = "dot_q8q8_template")]
struct GridDotQ8Q8 {}
[dasllama_grid(src = "quantize_q8_0_into_ptr_template")]
struct GridQuant {}
[dasllama_grid(src = "rope_scaled_neox_tab_template")]
struct GridRopeTab {}
[dasllama_grid(src = "gemm_f32_uk_4x16_template")]
struct GridGemm {}
[dasllama_grid(src = "dot_q8q8_laneq4x4_template")]
struct GridLaneq {}

// Registry fn-ptr signatures (== function_to_type of each template).
typedef DotFn = function<(a : float const?; b : float const?; n : int64) : float>
typedef BinFn = function<(var d : float?; s : float const?; n : int64) : void>
typedef AxpyFn = function<(var d : float?; s : float const?; scale : float; n : int64) : void>
typedef ScaleFn = function<(var d : float?; scale : float; n : int64) : void>

let N = 4096l
let ROUNDS = 80
let REPS = 2000
let TOL = 1.0e-3lf
let BASELINE = "vec8_u2"   // the majority hand default; kernels with their own [tuned(fallback=)] pass it to report()

// ===== shared reporting =====

// Print each passing variant's best time as a % delta vs the kernel's SHIPPED baseline (its
// [tuned] fallback perm — vec8_u2 for most, but e.g. dot_q8q8 ships vec16), mark the winner, and
// return the winning suffix (or "" if nothing passed). A win of only a couple % is flagged but is
// likely noise — vec16 was a real 2% loss, so the bar for "genuinely new" is a repeatable margin.
def report(kernel : string; names : array<string>; best : array<double>; ok : array<bool>; baseline = BASELINE) : string {
    var bi = -1
    for (i, nm in count(), names) {
        if (nm == baseline) {
            bi = i
        }
    }
    let base = (bi >= 0 && ok[bi]) ? best[bi] : -1.0lf
    var winner = -1
    for (i in range(length(names))) {
        if (ok[i] && (winner < 0 || best[i] < best[winner])) {
            winner = i
        }
    }
    print("--- {kernel} ---\n")
    for (i in range(length(names))) {
        if (!ok[i]) {
            print("  {names[i]}: REJECTED\n")
            continue
        }
        let delta = base > 0.0lf ? (best[i] - base) / base * 100.0lf : 0.0lf
        let mark = i == winner ? "  <== winner" : ""
        print("  {names[i]}: {best[i]} us  ({delta}% vs {baseline}){mark}\n")
    }
    if (winner < 0) {
        print("  {kernel}: no passing variant\n")
        return ""
    }
    var wname = names[winner]
    // a tie or sub-noise "win" HOLDS the shipped baseline: flipping the perm re-keys the JIT and
    // reorders the kernel's float math, which is only worth paying for a real, repeatable margin
    let margin = base > 0.0lf ? (base - best[winner]) / base * 100.0lf : 0.0lf
    if (wname != baseline && base > 0.0lf && margin < 0.5lf) {
        wname = baseline
        print("  => {kernel}: {baseline} holds ({names[winner]} within the noise floor, {margin}%)\n")
    } elif (wname == baseline) {
        print("  => {kernel}: {baseline} holds (the shipped default is already best)\n")
    } else {
        print("  => {kernel}: {wname} beats {baseline} by {margin}%  <<< the tuner found something\n")
    }
    return wname
}

def reset(var d : array<float>; d0 : array<float>) {
    for (i in range(length(d))) {
        d[i] = d0[i]
    }
}

// ===== dot: (a,b)->float, repeatable (no mutation) =====

def ref_dot(a : float const?; b : float const?; n : int64) : double {
    var s = 0.0lf
    for (j in range64(n)) {
        s += double(unsafe(a[j])) * double(unsafe(b[j]))
    }
    return s
}

def bench_dot() : string {
    var a : array<float>
    var b : array<float>
    a |> resize(int(N))
    b |> resize(int(N))
    for (i in range(int(N))) {
        a[i] = sin(float(i) * 0.1f)
        b[i] = cos(float(i) * 0.07f)
    }
    let pa = unsafe(addr(a[0]))
    let pb = unsafe(addr(b[0]))
    let want = ref_dot(pa, pb, N)

    let vs <- dot_template_variants()
    let nv = length(vs)
    var names : array<string>
    var ok : array<bool>
    var best : array<double>
    names |> resize(nv)
    ok |> resize(nv)
    best |> resize(nv)
    for (i in range(nv)) {
        names[i] = vs[i]._0
        let f = vs[i]._1
        let got = double(f(pa, pb, N))
        ok[i] = abs(got - want) / (abs(want) + 1.0lf) <= TOL
        best[i] = 1.0e30lf
    }

    var sink = 0.0f
    for (_round in range(ROUNDS)) {
        for (i in range(nv)) {
            if (!ok[i]) {
                continue
            }
            let f = vs[i]._1
            let t0 = ref_time_ticks()
            for (_rep in range(REPS)) {
                sink += f(pa, pb, N)
            }
            let us = double(get_time_usec(t0))
            if (us < best[i]) {
                best[i] = us
            }
        }
    }
    if (sink == 12345.678f) {
        print("")   // keep sink live
    }
    return report("dot", names, best, ok)
}

// ===== binary in-place: (var d, s)->void — add / mul / copy =====
// ref_elem(d0[j], s[j]) is the expected element; the timing loop resets d=d0 before each block so an
// accumulating/multiplicative kernel stays in normal float range over REPS (no denormal/inf slowdown).

def bench_binary(kernel : string; vs : array<tuple<string; BinFn>>;
                 d0 : array<float>; s : array<float>; ref_elem : function<(a : float; b : float) : double>) : string {
    let nv = length(vs)
    var d : array<float>
    d |> resize(int(N))
    var dp = unsafe(addr(d[0]))
    let sp = unsafe(addr(s[0]))

    var names : array<string>
    var ok : array<bool>
    var best : array<double>
    names |> resize(nv)
    ok |> resize(nv)
    best |> resize(nv)
    for (i in range(nv)) {
        names[i] = vs[i]._0
        best[i] = 1.0e30lf
        reset(d, d0)
        let f = vs[i]._1
        invoke(f, dp, sp, N)
        var maxrel = 0.0lf
        for (j in range(int(N))) {
            let want = ref_elem(d0[j], s[j])
            let got = double(d[j])
            let rel = abs(got - want) / (abs(want) + 1.0lf)
            if (rel > maxrel) {
                maxrel = rel
            }
        }
        ok[i] = maxrel <= TOL
    }

    var sink = 0.0f
    for (_round in range(ROUNDS)) {
        for (i in range(nv)) {
            if (!ok[i]) {
                continue
            }
            reset(d, d0)
            let f = vs[i]._1
            let t0 = ref_time_ticks()
            for (_rep in range(REPS)) {
                invoke(f, dp, sp, N)
            }
            let us = double(get_time_usec(t0))
            if (us < best[i]) {
                best[i] = us
            }
        }
        sink += d[0]
    }
    print("  checksum {sink}\n")
    return report(kernel, names, best, ok)
}

// ===== axpy: (var d, s, scale)->void =====

def bench_axpy() : string {
    let vs <- axpy_template_variants()
    let nv = length(vs)
    let scale = 0.5f
    var d0 : array<float>
    var s : array<float>
    var d : array<float>
    d0 |> resize(int(N))
    s |> resize(int(N))
    d |> resize(int(N))
    for (i in range(int(N))) {
        d0[i] = 0.5f + 0.3f * sin(float(i) * 0.05f)
        s[i] = cos(float(i) * 0.03f)
    }
    var dp = unsafe(addr(d[0]))
    let sp = unsafe(addr(s[0]))

    var names : array<string>
    var ok : array<bool>
    var best : array<double>
    names |> resize(nv)
    ok |> resize(nv)
    best |> resize(nv)
    for (i in range(nv)) {
        names[i] = vs[i]._0
        best[i] = 1.0e30lf
        reset(d, d0)
        let f = vs[i]._1
        invoke(f, dp, sp, scale, N)
        var maxrel = 0.0lf
        for (j in range(int(N))) {
            let want = double(d0[j]) + double(scale) * double(s[j])
            let rel = abs(double(d[j]) - want) / (abs(want) + 1.0lf)
            if (rel > maxrel) {
                maxrel = rel
            }
        }
        ok[i] = maxrel <= TOL
    }

    var sink = 0.0f
    for (_round in range(ROUNDS)) {
        for (i in range(nv)) {
            if (!ok[i]) {
                continue
            }
            reset(d, d0)
            let f = vs[i]._1
            let t0 = ref_time_ticks()
            for (_rep in range(REPS)) {
                invoke(f, dp, sp, scale, N)
            }
            let us = double(get_time_usec(t0))
            if (us < best[i]) {
                best[i] = us
            }
        }
        sink += d[0]
    }
    print("  checksum {sink}\n")
    return report("axpy", names, best, ok)
}

// ===== scale: (var d, scale)->void =====

def bench_scale() : string {
    let vs <- scale_inplace_template_variants()
    let nv = length(vs)
    let scale = 1.0001f   // near 1.0 so scale^REPS stays in normal float range with per-block reset
    var d0 : array<float>
    var d : array<float>
    d0 |> resize(int(N))
    d |> resize(int(N))
    for (i in range(int(N))) {
        d0[i] = 0.5f + 0.3f * sin(float(i) * 0.05f)
    }
    var dp = unsafe(addr(d[0]))

    var names : array<string>
    var ok : array<bool>
    var best : array<double>
    names |> resize(nv)
    ok |> resize(nv)
    best |> resize(nv)
    for (i in range(nv)) {
        names[i] = vs[i]._0
        best[i] = 1.0e30lf
        reset(d, d0)
        let f = vs[i]._1
        invoke(f, dp, scale, N)
        var maxrel = 0.0lf
        for (j in range(int(N))) {
            let want = double(d0[j]) * double(scale)
            let rel = abs(double(d[j]) - want) / (abs(want) + 1.0lf)
            if (rel > maxrel) {
                maxrel = rel
            }
        }
        ok[i] = maxrel <= TOL
    }

    var sink = 0.0f
    for (_round in range(ROUNDS)) {
        for (i in range(nv)) {
            if (!ok[i]) {
                continue
            }
            reset(d, d0)
            let f = vs[i]._1
            let t0 = ref_time_ticks()
            for (_rep in range(REPS)) {
                invoke(f, dp, scale, N)
            }
            let us = double(get_time_usec(t0))
            if (us < best[i]) {
                best[i] = us
            }
        }
        sink += d[0]
    }
    print("  checksum {sink}\n")
    return report("scale_inplace", names, best, ok)
}

// ===== softmax: (var x, size) in-place — reset before each timing block; repeated softmax of a
// distribution stays a bounded distribution, so no denormal/inf drift over REPS.

def bench_softmax() : string {
    let vs <- softmax_template_variants()
    let nv = length(vs)
    var d0 : array<float>
    var d : array<float>
    d0 |> resize(int(N))
    d |> resize(int(N))
    for (i in range(int(N))) {
        d0[i] = 4.0f * sin(float(i) * 0.11f)   // scores in [-4,4], a realistic logit spread
    }
    var dp = unsafe(addr(d[0]))
    var want : array<double>
    want |> resize(int(N))
    var mx = -1.0e30lf
    for (i in range(int(N))) {
        mx = max(mx, double(d0[i]))
    }
    var wsum = 0.0lf
    for (i in range(int(N))) {
        let e = exp(double(d0[i]) - mx)
        want[i] = e
        wsum += e
    }
    for (i in range(int(N))) {
        want[i] /= wsum
    }

    var names : array<string>
    var ok : array<bool>
    var best : array<double>
    names |> resize(nv)
    ok |> resize(nv)
    best |> resize(nv)
    for (i in range(nv)) {
        names[i] = vs[i]._0
        best[i] = 1.0e30lf
        reset(d, d0)
        let f = vs[i]._1
        invoke(f, dp, N)
        var maxrel = 0.0lf
        for (j in range(int(N))) {
            let rel = abs(double(d[j]) - want[j]) / (abs(want[j]) + 1.0lf)
            if (rel > maxrel) {
                maxrel = rel
            }
        }
        ok[i] = maxrel <= TOL
    }

    var sink = 0.0f
    for (_round in range(ROUNDS)) {
        for (i in range(nv)) {
            if (!ok[i]) {
                continue
            }
            reset(d, d0)
            let f = vs[i]._1
            let t0 = ref_time_ticks()
            for (_rep in range(REPS)) {
                invoke(f, dp, N)
            }
            let us = double(get_time_usec(t0))
            if (us < best[i]) {
                best[i] = us
            }
        }
        sink += d[0]
    }
    print("  checksum {sink}\n")
    return report("softmax", names, best, ok)
}

// ===== rmsnorm: (var o, x, weight, size, eps) out-of-place — repeatable, no reset needed.

def bench_rmsnorm() : string {
    let vs <- rmsnorm_template_variants()
    let nv = length(vs)
    let eps = 1.0e-5f
    var x : array<float>
    var w : array<float>
    var o : array<float>
    x |> resize(int(N))
    w |> resize(int(N))
    o |> resize(int(N))
    for (i in range(int(N))) {
        x[i] = sin(float(i) * 0.1f)
        w[i] = 1.0f + 0.2f * cos(float(i) * 0.07f)
    }
    var op = unsafe(addr(o[0]))
    let xp = unsafe(addr(x[0]))
    let wp = unsafe(addr(w[0]))
    var ss = 0.0lf
    for (i in range(int(N))) {
        ss += double(x[i]) * double(x[i])
    }
    ss = 1.0lf / sqrt(ss / double(N) + double(eps))

    var names : array<string>
    var ok : array<bool>
    var best : array<double>
    names |> resize(nv)
    ok |> resize(nv)
    best |> resize(nv)
    for (i in range(nv)) {
        names[i] = vs[i]._0
        best[i] = 1.0e30lf
        let f = vs[i]._1
        invoke(f, op, xp, wp, N, eps)
        var maxrel = 0.0lf
        for (j in range(int(N))) {
            let want = double(w[j]) * double(x[j]) * ss
            let rel = abs(double(o[j]) - want) / (abs(want) + 1.0lf)
            if (rel > maxrel) {
                maxrel = rel
            }
        }
        ok[i] = maxrel <= TOL
    }

    var sink = 0.0f
    for (_round in range(ROUNDS)) {
        for (i in range(nv)) {
            if (!ok[i]) {
                continue
            }
            let f = vs[i]._1
            let t0 = ref_time_ticks()
            for (_rep in range(REPS)) {
                invoke(f, op, xp, wp, N, eps)
            }
            let us = double(get_time_usec(t0))
            if (us < best[i]) {
                best[i] = us
            }
        }
        sink += o[0]
    }
    print("  checksum {sink}\n")
    return report("rmsnorm", names, best, ok, "plain")
}

// ===== dot_q4 / dot_q8q8: quantized dots, repeatable (no mutation). References run in f64 on the
// DEQUANTIZED values (q4) / the exact int block sums (q8q8).

def bench_dot_q4() : string {
    let vs <- dot_q4_template_variants()
    let nv = length(vs)
    var wf : array<float>
    var x : array<float>
    wf |> resize(int(N))
    x |> resize(int(N))
    for (i in range(int(N))) {
        wf[i] = sin(float(i) * 0.1f)
        x[i] = cos(float(i) * 0.07f)
    }
    let qt <- quantize_q4_0(wf, N)
    var df : array<float>
    df |> resize(int(N))
    dequantize_q4_0(qt, df)
    var want = 0.0lf
    for (i in range(int(N))) {
        want += double(df[i]) * double(x[i])
    }
    let wqp = unsafe(addr(qt.q[0]))
    let wsp = unsafe(addr(qt.scales[0]))
    let xp = unsafe(addr(x[0]))

    var names : array<string>
    var ok : array<bool>
    var best : array<double>
    names |> resize(nv)
    ok |> resize(nv)
    best |> resize(nv)
    for (i in range(nv)) {
        names[i] = vs[i]._0
        best[i] = 1.0e30lf
        let f = vs[i]._1
        let got = double(f(wqp, wsp, xp, N))
        ok[i] = abs(got - want) / (abs(want) + 1.0lf) <= TOL
    }

    var sink = 0.0f
    for (_round in range(ROUNDS)) {
        for (i in range(nv)) {
            if (!ok[i]) {
                continue
            }
            let f = vs[i]._1
            let t0 = ref_time_ticks()
            for (_rep in range(REPS)) {
                sink += f(wqp, wsp, xp, N)
            }
            let us = double(get_time_usec(t0))
            if (us < best[i]) {
                best[i] = us
            }
        }
    }
    if (sink == 12345.678f) {
        print("")   // keep sink live
    }
    return report("dot_q4", names, best, ok, "vec4_u4")
}

def bench_dot_q8q8() : string {
    let vs <- dot_q8q8_template_variants()
    let nv = length(vs)
    var wf : array<float>
    var xf : array<float>
    wf |> resize(int(N))
    xf |> resize(int(N))
    for (i in range(int(N))) {
        wf[i] = sin(float(i) * 0.1f)
        xf[i] = cos(float(i) * 0.07f)
    }
    let wq <- quantize_q8_0(wf, N)
    let xq <- quantize_q8_0(xf, N)
    let nb = N / 32l
    var want = 0.0lf
    for (bi in range64(nb)) {
        var iacc = 0
        for (k in range64(32l)) {
            iacc += int(wq.q[bi * 32l + k]) * int(xq.q[bi * 32l + k])
        }
        want += double(iacc) * double(wq.scales[bi]) * double(xq.scales[bi])
    }
    let wqp = unsafe(addr(wq.q[0]))
    let wsp = unsafe(addr(wq.scales[0]))
    let xqp = unsafe(addr(xq.q[0]))
    let xsp = unsafe(addr(xq.scales[0]))

    var names : array<string>
    var ok : array<bool>
    var best : array<double>
    names |> resize(nv)
    ok |> resize(nv)
    best |> resize(nv)
    for (i in range(nv)) {
        names[i] = vs[i]._0
        best[i] = 1.0e30lf
        let f = vs[i]._1
        let got = double(f(wqp, wsp, xqp, xsp, N))
        ok[i] = abs(got - want) / (abs(want) + 1.0lf) <= TOL
    }

    var sink = 0.0f
    for (_round in range(ROUNDS)) {
        for (i in range(nv)) {
            if (!ok[i]) {
                continue
            }
            let f = vs[i]._1
            let t0 = ref_time_ticks()
            for (_rep in range(REPS)) {
                sink += f(wqp, wsp, xqp, xsp, N)
            }
            let us = double(get_time_usec(t0))
            if (us < best[i]) {
                best[i] = us
            }
        }
    }
    if (sink == 12345.678f) {
        print("")   // keep sink live
    }
    return report("dot_q8q8", names, best, ok, "vec16")
}

// ===== quantize_q8_0_into_ptr: EXACT gate — every variant must produce byte-identical quants and
// scales to the module's quantize_q8_0 reference (the store loop is elementwise round+narrow; any
// difference under a perm is a miscompile, not a tolerance question).

def bench_quantize() : string {
    let vs <- quantize_q8_0_into_ptr_template_variants()
    let nv = length(vs)
    var src : array<float>
    src |> resize(int(N))
    for (i in range(int(N))) {
        src[i] = 2.0f * sin(float(i) * 0.13f)
    }
    let want <- quantize_q8_0(src, N)
    var qdst : array<int8>
    var sdst : array<float>
    qdst |> resize(int(N))
    sdst |> resize(int(N / 32l))
    let srcp = unsafe(addr(src[0]))
    var qp = unsafe(addr(qdst[0]))
    var sp = unsafe(addr(sdst[0]))

    var names : array<string>
    var ok : array<bool>
    var best : array<double>
    names |> resize(nv)
    ok |> resize(nv)
    best |> resize(nv)
    for (i in range(nv)) {
        names[i] = vs[i]._0
        best[i] = 1.0e30lf
        let f = vs[i]._1
        invoke(f, srcp, N, qp, sp, 0l, 0l)
        var exact = true
        for (j in range(int(N))) {
            if (qdst[j] != want.q[j]) {
                exact = false
            }
        }
        for (b in range(int(N / 32l))) {
            if (sdst[b] != want.scales[b]) {
                exact = false
            }
        }
        ok[i] = exact
    }

    var sink = 0.0f
    for (_round in range(ROUNDS)) {
        for (i in range(nv)) {
            if (!ok[i]) {
                continue
            }
            let f = vs[i]._1
            let t0 = ref_time_ticks()
            for (_rep in range(REPS)) {
                invoke(f, srcp, N, qp, sp, 0l, 0l)
            }
            let us = double(get_time_usec(t0))
            if (us < best[i]) {
                best[i] = us
            }
        }
        sink += sdst[0]
    }
    print("  checksum {sink}\n")
    return report("quantize_q8_0_into_ptr", names, best, ok, "plain")
}

// ===== rope_scaled_neox_tab: in-place rotation, norm-preserving — timing runs without reset.

def bench_rope_tab() : string {
    let vs <- rope_scaled_neox_tab_template_variants()
    let nv = length(vs)
    let head_size = 64l
    let half = head_size / 2l
    var v0 : array<float>
    var v : array<float>
    v0 |> resize(int(N))
    v |> resize(int(N))
    for (i in range(int(N))) {
        v0[i] = sin(float(i) * 0.1f)
    }
    var cosr : array<float>
    var sinr : array<float>
    cosr |> resize(int(half))
    sinr |> resize(int(half))
    for (j in range(int(half))) {
        let ang = 37.0f / pow(10000.0f, float(2 * j) / float(head_size))
        cosr[j] = cos(ang)
        sinr[j] = sin(ang)
    }
    var vp = unsafe(addr(v[0]))
    let cp = unsafe(addr(cosr[0]))
    let sp = unsafe(addr(sinr[0]))

    var names : array<string>
    var ok : array<bool>
    var best : array<double>
    names |> resize(nv)
    ok |> resize(nv)
    best |> resize(nv)
    for (i in range(nv)) {
        names[i] = vs[i]._0
        best[i] = 1.0e30lf
        reset(v, v0)
        let f = vs[i]._1
        invoke(f, vp, head_size, N, cp, sp)
        var maxrel = 0.0lf
        for (h in range(int(N / head_size))) {
            let base = h * int(head_size)
            for (j in range(int(half))) {
                let a = double(v0[base + j])
                let b = double(v0[base + j + int(half)])
                let wc = double(cosr[j])
                let ws = double(sinr[j])
                let want0 = a * wc - b * ws
                let want1 = a * ws + b * wc
                let r0 = abs(double(v[base + j]) - want0) / (abs(want0) + 1.0lf)
                let r1 = abs(double(v[base + j + int(half)]) - want1) / (abs(want1) + 1.0lf)
                maxrel = max(maxrel, max(r0, r1))
            }
        }
        ok[i] = maxrel <= TOL
    }

    var sink = 0.0f
    for (_round in range(ROUNDS)) {
        for (i in range(nv)) {
            if (!ok[i]) {
                continue
            }
            reset(v, v0)
            let f = vs[i]._1
            let t0 = ref_time_ticks()
            for (_rep in range(REPS)) {
                invoke(f, vp, head_size, N, cp, sp)
            }
            let us = double(get_time_usec(t0))
            if (us < best[i]) {
                best[i] = us
            }
        }
        sink += v[0]
    }
    print("  checksum {sink}\n")
    return report("rope_scaled_neox_tab", names, best, ok)
}

// ===== gemm_f32_uk_4x16: one 4×16 tile over K, C += A·B — reset C per round; over REPS the
// accumulator grows linearly (REPS × O(1) entries), well inside normal float range.

def bench_gemm_tile() : string {
    let vs <- gemm_f32_uk_4x16_template_variants()
    let nv = length(vs)
    let K = 256l
    let NN = 16l   // C/B row stride == tile width, so the 4×16 tile is the whole C
    var a : array<float>
    var b : array<float>
    var c : array<float>
    a |> resize(int(4l * K))
    b |> resize(int(K * NN))
    c |> resize(int(4l * NN))
    for (i in range(int(4l * K))) {
        a[i] = sin(float(i) * 0.1f)
    }
    for (i in range(int(K * NN))) {
        b[i] = cos(float(i) * 0.07f)
    }
    var cp = unsafe(addr(c[0]))
    let ap = unsafe(addr(a[0]))
    let bp = unsafe(addr(b[0]))

    var names : array<string>
    var ok : array<bool>
    var best : array<double>
    names |> resize(nv)
    ok |> resize(nv)
    best |> resize(nv)
    for (i in range(nv)) {
        names[i] = vs[i]._0
        best[i] = 1.0e30lf
        for (j in range(int(4l * NN))) {
            c[j] = 0.0f
        }
        let f = vs[i]._1
        invoke(f, cp, ap, bp, 0l, 0l, K, NN)
        var maxrel = 0.0lf
        for (r in range(4)) {
            for (col in range(int(NN))) {
                var want = 0.0lf
                for (kk in range(int(K))) {
                    want += double(a[r * int(K) + kk]) * double(b[kk * int(NN) + col])
                }
                let rel = abs(double(c[r * int(NN) + col]) - want) / (abs(want) + 1.0lf)
                if (rel > maxrel) {
                    maxrel = rel
                }
            }
        }
        ok[i] = maxrel <= TOL
    }

    var sink = 0.0f
    for (_round in range(ROUNDS)) {
        for (i in range(nv)) {
            if (!ok[i]) {
                continue
            }
            for (j in range(int(4l * NN))) {
                c[j] = 0.0f
            }
            let f = vs[i]._1
            let t0 = ref_time_ticks()
            for (_rep in range(REPS)) {
                invoke(f, cp, ap, bp, 0l, 0l, K, NN)
            }
            let us = double(get_time_usec(t0))
            if (us < best[i]) {
                best[i] = us
            }
        }
        sink += c[0]
    }
    print("  checksum {sink}\n")
    return report("gemm_f32_uk_4x16", names, best, ok, "u2")
}

// ===== dot_q8q8_laneq4x4: arm64+JIT only (the repack backend must be registered so the pin lands
// and the intrinsic lowers to hardware SDOT). Runs LAST — the pin sticks until process exit.

def bench_laneq4x4() : string {
    if (get_architecture_name() != "arm64" || !jit_enabled()) {
        print("--- dot_q8q8_laneq4x4 ---\n  skipped (needs arm64 + -jit: lane-indexed SDOT + the repack backend)\n")
        return ""
    }
    let vs <- dot_q8q8_laneq4x4_template_variants()
    let nv = length(vs)
    let n = N
    let nb = n / 32l
    let d = 4l     // one interleaved 4-row group
    let ntok = 4l
    var wf : array<float>
    wf |> resize(int(d * n))
    for (i in range(int(d * n))) {
        wf[i] = sin(float(i) * 0.1f)
    }
    var wq <- quantize_q8_0(wf, d * n)
    var refq : array<int8>    // row-major copies for the reference (repack rewrites wq in place)
    var refs : array<float>
    refq |> resize(int(d * n))
    refs |> resize(int(d * nb))
    for (i in range(int(d * n))) {
        refq[i] = wq.q[i]
    }
    for (i in range(int(d * nb))) {
        refs[i] = wq.scales[i]
    }
    pin_kernel_backend("arm64-laneq")             // activates the repack fn the next call uses
    repack_q8q8_weight(wq.q, wq.scales, 0l, n, d)
    var xf : array<float>
    xf |> resize(int(ntok * n))
    for (i in range(int(ntok * n))) {
        xf[i] = cos(float(i) * 0.07f)
    }
    var xq : array<int8>
    var xs : array<float>
    xq |> resize(int(ntok * n))
    xs |> resize(int(ntok * nb))
    quantize_q8_0_into(xf, ntok * n, xq, xs, 0l, 0l)
    var y : array<float>
    y |> resize(int(ntok * d))
    var yp = unsafe(addr(y[0]))
    let wgp = unsafe(addr(wq.q[0]))
    let sgp = unsafe(addr(wq.scales[0]))
    let xqp = unsafe(addr(xq[0]))
    let xsp = unsafe(addr(xs[0]))

    var names : array<string>
    var ok : array<bool>
    var best : array<double>
    names |> resize(nv)
    ok |> resize(nv)
    best |> resize(nv)
    for (i in range(nv)) {
        names[i] = vs[i]._0
        best[i] = 1.0e30lf
        let f = vs[i]._1
        invoke(f, yp, wgp, sgp, xqp, xsp, n, d, 0l, 0l)
        var maxrel = 0.0lf
        for (t in range(int(ntok))) {
            for (r in range(int(d))) {
                var want = 0.0lf
                for (bi in range64(nb)) {
                    var iacc = 0
                    for (k in range64(32l)) {
                        iacc += int(refq[r * int(n) + int(bi * 32l + k)]) * int(xq[t * int(n) + int(bi * 32l + k)])
                    }
                    want += double(iacc) * double(refs[r * int(nb) + int(bi)]) * double(xs[t * int(nb) + int(bi)])
                }
                let got = double(y[t * int(d) + r])
                let rel = abs(got - want) / (abs(want) + 1.0lf)
                if (rel > maxrel) {
                    maxrel = rel
                }
            }
        }
        ok[i] = maxrel <= TOL
    }

    var sink = 0.0f
    for (_round in range(ROUNDS)) {
        for (i in range(nv)) {
            if (!ok[i]) {
                continue
            }
            let f = vs[i]._1
            let t0 = ref_time_ticks()
            for (_rep in range(REPS)) {
                invoke(f, yp, wgp, sgp, xqp, xsp, n, d, 0l, 0l)
            }
            let us = double(get_time_usec(t0))
            if (us < best[i]) {
                best[i] = us
            }
        }
        sink += y[0]
    }
    print("  checksum {sink}\n")
    return report("dot_q8q8_laneq4x4", names, best, ok, "u2")
}

[export]
def main {
    create_job_que()   // the benches are single-thread, but the runtime snapshot reads get_total_hw_jobs()
    print("kernel tuner: N={N}, {ROUNDS} rounds x {REPS} reps, interleaved best-of\n\n")

    // add / mul / copy share the (var d, s) signature — one bench, per-kernel data + reference.
    var add_d0 : array<float>
    var add_s : array<float>
    add_d0 |> resize(int(N))
    add_s |> resize(int(N))
    for (i in range(int(N))) {
        add_d0[i] = 0.5f + 0.3f * sin(float(i) * 0.05f)
        add_s[i] = cos(float(i) * 0.03f)
    }
    var mul_d0 : array<float>
    var mul_s : array<float>
    mul_d0 |> resize(int(N))
    mul_s |> resize(int(N))
    for (i in range(int(N))) {
        mul_d0[i] = 0.5f + 0.3f * sin(float(i) * 0.05f)
        mul_s[i] = 1.0f + 0.0005f * sin(float(i) * 0.02f)   // near 1.0: s^REPS stays finite/normal
    }
    var cpy_d0 : array<float>
    var cpy_s : array<float>
    cpy_d0 |> resize(int(N))
    cpy_s |> resize(int(N))
    for (i in range(int(N))) {
        cpy_d0[i] = 0.0f
        cpy_s[i] = cos(float(i) * 0.03f)
    }

    let add_vs <- add_inplace_template_variants()
    let mul_vs <- mul_inplace_template_variants()
    let cpy_vs <- copy_floats_template_variants()

    let dotw = bench_dot()
    let axpyw = bench_axpy()
    let addw = bench_binary("add_inplace", add_vs, add_d0, add_s, @@(a, b : float) : double => double(a) + double(b))
    let mulw = bench_binary("mul_inplace", mul_vs, mul_d0, mul_s, @@(a, b : float) : double => double(a) * double(b))
    let scalew = bench_scale()
    let copyw = bench_binary("copy_floats", cpy_vs, cpy_d0, cpy_s, @@(_a, b : float) : double => double(b))
    let softmaxw = bench_softmax()
    let rmsw = bench_rmsnorm()
    let dq4w = bench_dot_q4()
    let dq8w = bench_dot_q8q8()
    let quantw = bench_quantize()
    let ropew = bench_rope_tab()
    let gemmw = bench_gemm_tile()
    let laneqw = bench_laneq4x4()   // last: it pins the repack backend (sticks until process exit)

    // Summary: which kernels the tuner re-tuned away from THEIR OWN shipped fallback (the
    // [tuned(fallback=...)] perm; bare [tuned] ships the vec8_u2 majority = BASELINE).
    print("\n===== summary (winner per kernel) =====\n")
    let knames = fixed_array("dot", "axpy", "add_inplace", "mul_inplace", "scale_inplace", "copy_floats",
        "softmax", "rmsnorm", "dot_q4", "dot_q8q8", "quantize_q8_0_into_ptr",
        "rope_scaled_neox_tab", "gemm_f32_uk_4x16", "dot_q8q8_laneq4x4")
    let kwin = fixed_array(dotw, axpyw, addw, mulw, scalew, copyw,
        softmaxw, rmsw, dq4w, dq8w, quantw, ropew, gemmw, laneqw)
    let kfall = fixed_array(BASELINE, BASELINE, BASELINE, BASELINE, BASELINE, BASELINE,
        BASELINE, "plain", "vec4_u4", "vec16", "plain",
        BASELINE, "u2", "u2")
    var changed = 0
    for (k, w, f in knames, kwin, kfall) {
        let note = (w == "" || w == f) ? "" : "  (shipped fallback {f})"
        print("  {k}: {w}{note}\n")
        if (w != "" && w != f) {
            changed++
        }
    }
    print(changed == 0
        ? "\nevery kernel keeps its shipped fallback — the tuner found nothing new on this box.\n"
        : "\n{changed} kernel(s) beat their shipped fallback on this box.\n")

    // softmax_sink shares softmax's loop shape exactly (one extra scalar joins the reduction), so it
    // mirrors softmax's winner instead of paying its own sweep.
    // The "runtime" section snapshots the CURRENT runtime knobs (defaults unless this process changed
    // them) so the written profile is complete and hand-editable: load_model applies it via
    // apply_box_profile_runtime. TB/L2-budget refinement stays manual (tune_tb.das shows the raw
    // cliff; edit the file under the knee); "threads" is advisory — set DAS_JOBQUE_THREADS to change it.
    let runtime = JV((q8_token_block = get_q8_token_block(), q8_l2_budget = get_q8_l2_budget(),
        matmul_par_threshold = get_matmul_par_threshold(), attn_par_threshold = get_attn_par_threshold(),
        decode_attn_par_threshold = get_decode_attn_par_threshold(),
        requant_par_threshold = get_requant_par_threshold(), norm_par_threshold = get_norm_par_threshold(),
        rope_par_threshold = get_rope_par_threshold(), kv_store_par_threshold = get_kv_store_par_threshold(),
        act_par_threshold = get_act_par_threshold(), q8_chunks_per_job = get_q8_chunks_per_job(),
        q4_chunks_per_job = get_q4_chunks_per_job(), threads = get_total_hw_jobs()))
    let profile = JV((dot = dotw, axpy = axpyw, add_inplace = addw,
        mul_inplace = mulw, scale_inplace = scalew, copy_floats = copyw,
        softmax = softmaxw, softmax_sink = softmaxw, rmsnorm = rmsw,
        dot_q4 = dq4w, dot_q8q8 = dq8w, quantize_q8_0_into_ptr = quantw,
        rope_scaled_neox_tab = ropew, gemm_f32_uk_4x16 = gemmw, dot_q8q8_laneq4x4 = laneqw,
        runtime = runtime))
    let js = write_json(profile)
    let ppath = box_profile_path()   // same resolution [tuned] reads from: env override, else <das_root>
    if (fwrite(ppath, js)) {
        print("wrote {ppath}: {js}\n")
    } else {
        print("FAILED to write {ppath} — if das_root is read-only (installed SDK), set DASLLAMA_BOX_PROFILE to a writable path\n")
    }
}
