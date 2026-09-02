options gen2

require dastest/testing_boost
require math

// Accuracy contract for the inline vector sin/cos the aarch64 JIT emits instead of
// @llvm.sin.vNf32 / @llvm.cos.vNf32 (which scalarize to N libm calls on Darwin-ARM) -
// build_vector_sincos in daslib/llvm_jit_intrin.das. The emitter mirrors vecmath's
// v_sincos, the same polynomial the interpreter and AOT already run, so all three rails
// must land within one bound of libm.
// Arguments arrive through a runtime array on purpose: a compile-time-constant argument is
// const-folded by the AST and never reaches the intrinsic, so a folded test proves nothing.
// The tight bound needs a fused a*b+c on whichever rail runs: the two-constant Cody-Waite
// reduction only stays exact at |x| ~ 1e5 while q*(pi/2) is not rounded. aarch64 always
// fuses; a generic x64 build has no FMA and only the coarse bound survives there.

let private TIGHT_ABS_ERROR = 3.0e-7    // measured 1.1920929e-07 (1 ulp) over |x| <= 1e5 on M-series
let private COARSE_ABS_ERROR = 1.0e-2   // unfused rails: still catches quadrant, sign and NaN failures

struct private ErrReport {
    maxAbs : float
    at : float
    nans : int
    lanes : int
}

// written before sampling so the samples cannot fold to constants at compile time
var private g_offset = 0.0

def private error_bound : float {
    return get_architecture_name() == "arm64" ? TIGHT_ABS_ERROR : COARSE_ABS_ERROR
}

def private build_samples : array<float4> {
    var xs : array<float4>
    xs |> reserve(10000 * 5 + 401 * 33 + 20000)
    // decade sweep, four unrelated magnitudes per entry, staying inside |x| <= 1e5
    for (i in range(10000)) {
        let u = float(i) * 2.0e-4 - 1.0
        for (d in range(5)) {
            let s = pow(10.0, float(d))
            xs |> push(float4(u * s, -u * s * 3.7, u * s * 0.31 + g_offset, u * s * 9.13))
        }
    }
    // fine steps across the quadrant boundaries, where the select tree swaps polynomials
    for (k in range(-200, 201)) {
        let b = float(k) * (PI * 0.5)
        for (j in range(-16, 17)) {
            let x = b + float(j) * 1.0e-4
            xs |> push(float4(x, x + 1.0e-6, x - 1.0e-6, -x))
        }
    }
    // the far end of the range, where the range reduction carries the largest quadrant count
    for (i in range(20000)) {
        let b = 99000.0 + float(i) * 5.0e-2
        xs |> push(float4(b, -b, b + 1.0e-2, -b - 1.0e-2))
    }
    return <- xs
}

def private track(var r : ErrReport; got, ref, x : float) {
    r.lanes++
    let e = abs(got - ref)
    if (!(e >= 0.0)) {    // NaN never compares >= 0
        r.nans++
        return
    }
    if (e > r.maxAbs) {
        r.maxAbs = e
        r.at = x
    }
}

def private sincos_error4(xs : array<float4>; wantCos : bool) : ErrReport {
    var r : ErrReport
    for (v in xs) {
        let got = wantCos ? cos(v) : sin(v)
        for (l in range(4)) {
            let x = v[l]
            track(r, got[l], float(wantCos ? cos(double(x)) : sin(double(x))), x)
        }
    }
    return r
}

def private sin_error2(xs : array<float4>) : ErrReport {
    var r : ErrReport
    for (v in xs) {
        let got = sin(float2(v.x, v.y))
        for (l in range(2)) {
            let x = v[l]
            track(r, got[l], float(sin(double(x))), x)
        }
    }
    return r
}

def private sin_error3(xs : array<float4>) : ErrReport {
    var r : ErrReport
    for (v in xs) {
        let got = sin(float3(v.x, v.y, v.z))
        for (l in range(3)) {
            let x = v[l]
            track(r, got[l], float(sin(double(x))), x)
        }
    }
    return r
}

// the lane count is asserted, not reported: a sample set that silently folded or emptied would
// otherwise pass every bound
def private report(t : T?; what : string; r : ErrReport; bound : float; wantLanes : int) {
    t |> equal(r.lanes, wantLanes, "{what}: lanes compared")
    t |> success(r.nans == 0, "{what}: no NaN lanes")
    t |> success(r.maxAbs <= bound, "{what}: max abs error {r.maxAbs} at x = {r.at} (bound {bound})")
}

[test]
def test_vector_sin_matches_libm(t : T?) {
    g_offset = 0.0
    let xs <- build_samples()
    t |> success(!jit_enabled() || is_jit_function(@@sincos_error4), "sin worker is jitted")
    report(t, "sin(float4)", sincos_error4(xs, false), error_bound(), length(xs) * 4)
}

[test]
def test_vector_cos_matches_libm(t : T?) {
    g_offset = 0.0
    let xs <- build_samples()
    report(t, "cos(float4)", sincos_error4(xs, true), error_bound(), length(xs) * 4)
}

[test]
def test_vector_sin_narrow_widths(t : T?) {
    g_offset = 0.0
    let xs <- build_samples()
    let bound = error_bound()
    t |> success(!jit_enabled() || is_jit_function(@@sin_error2), "float2 worker is jitted")
    t |> success(!jit_enabled() || is_jit_function(@@sin_error3), "float3 worker is jitted")
    report(t, "sin(float2)", sin_error2(xs), bound, length(xs) * 2)
    report(t, "sin(float3)", sin_error3(xs), bound, length(xs) * 3)
}
