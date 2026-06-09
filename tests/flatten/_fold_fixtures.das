options gen2
options indenting = 4
options no_unused_function_arguments = false

require daslib/flatten
require math

//! `[flatten]` corpus for the fold-completeness verification — NOT a [test] file; it is
//! compiled as a unit by tests/flatten/test_flatten_fold.das, which asserts every `*_flat`
//! twin is residual-free. Seeded with the blur center-tap reveal cascade that motivated the
//! re-infer fold loop: a folded `0*s → 0` makes a local constant, the re-infer settles it
//! for const-prop, and only THEN does the revealed `uv + vec(0,…)` identity appear for the
//! next fold pass. Both lanes/widths (float2 1D, float2 2D = the actual 3x3 blur, float4) are
//! exercised so the width-matched zero arms all fire. The simple one-step identities live in
//! test_flatten_const.das.
//!
//! NOTE: a SCALAR value revealed only via a local (`let k = float(i-1)*s; v + k`) does not
//! yet fold its trailing `v + 0` — a separate, pre-existing fold-pipeline gap that does not
//! occur in real shaders (offsets are always vectors). Tracked as a follow-up; its fixture
//! returns here once fixed.

// Center tap, 1D: float(x-1)*s at x=1 -> 0*s -> 0, offset folds to float2(0,0), inlined ->
// uv + float2(0,0) -> uv. length() is a kept builtin, so uv + offset stays visible.
[flatten]
def fx_center(uv : float2; s : float) : float3 {
    var acc = float3(0.0, 0.0, 0.0)
    for (x in range(3)) {
        let offset = float2(float(x - 1) * s, 0.0)
        acc += float3(length(uv + offset))
    }
    return acc
}

// Center tap, 2D (blur's full 3x3): both lanes of the center offset reach zero.
[flatten]
def fx_center2(uv : float2; s : float) : float3 {
    var acc = float3(0.0, 0.0, 0.0)
    for (y in range(3)) {
        for (x in range(3)) {
            let offset = float2(float(x - 1) * s, float(y - 1) * s)
            acc += float3(length(uv + offset))
        }
    }
    return acc
}

// Center tap, float4 width: same cascade, the float4 zero arm must match width.
[flatten]
def fx_center4(uv : float4; s : float) : float4 {
    var acc = float4(0.0, 0.0, 0.0, 0.0)
    for (x in range(3)) {
        let offset = float4(float(x - 1) * s, 0.0, 0.0, 0.0)
        acc += float4(length(uv + offset))
    }
    return acc
}
