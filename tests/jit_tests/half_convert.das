options gen2

require dastest/testing_boost
require daslib/f16_cvt

// Each [jit] function and its [no_jit] twin run the same half code; the results must match.
// Run under DAS_JIT_BASELINE=x86-base, the JIT converts in IR (no F16C). A NaN counts as NaN:
// hardware converts quiet a signaling NaN, so a payload is not part of the contract.
// The helpers only move bits, so every convert happens in the twins.

def mix(var h : uint64; v : uint) : uint64 => (h ^ uint64(v)) * 1099511628211ul

def half_of(b : uint) : float16 => unsafe(reinterpret<float16>(uint16(b)))

def hbits(v : float16) : uint {
    let b = uint(unsafe(reinterpret<uint16>(v)))
    return (b & 0x7c00u) == 0x7c00u && (b & 0x3ffu) != 0u ? 0x7e00u : b
}

def fbits(v : float) : uint {
    let b = unsafe(reinterpret<uint>(v))
    return (b & 0x7f800000u) == 0x7f800000u && (b & 0x7fffffu) != 0u ? 0x7fc00000u : b
}

[jit]
def half_to_float_jit() : uint64 {
    var h = 14695981039346656037ul
    for (b in urange(65536u)) {
        h = mix(h, fbits(float(half_of(b))))
    }
    return h
}

[no_jit]
def half_to_float_interp() : uint64 {
    var h = 14695981039346656037ul
    for (b in urange(65536u)) {
        h = mix(h, fbits(float(half_of(b))))
    }
    return h
}

[jit]
def float_to_half_jit() : uint64 {
    var h = 14695981039346656037ul
    var u = 0u
    while (true) {
        h = mix(h, hbits(float16(unsafe(reinterpret<float>(u)))))
        let next = u + 65521u
        break if (next < u)
        u = next
    }
    return h
}

[no_jit]
def float_to_half_interp() : uint64 {
    var h = 14695981039346656037ul
    var u = 0u
    while (true) {
        h = mix(h, hbits(float16(unsafe(reinterpret<float>(u)))))
        let next = u + 65521u
        break if (next < u)
        u = next
    }
    return h
}

[jit]
def half4_round_trip_jit() : uint64 {
    var h = 14695981039346656037ul
    for (i in urange(261u)) {
        let b = i * 251u
        let w = float4(half4(float4(float(half_of(b)), float(half_of(b ^ 0x3c55u)), float(b), -float(b))))
        h = mix(h, fbits(w.x) ^ fbits(w.y) ^ fbits(w.z) ^ fbits(w.w))
    }
    return h
}

[no_jit]
def half4_round_trip_interp() : uint64 {
    var h = 14695981039346656037ul
    for (i in urange(261u)) {
        let b = i * 251u
        let w = float4(half4(float4(float(half_of(b)), float(half_of(b ^ 0x3c55u)), float(b), -float(b))))
        h = mix(h, fbits(w.x) ^ fbits(w.y) ^ fbits(w.z) ^ fbits(w.w))
    }
    return h
}

[jit]
def fp16_ops_jit() : uint64 {
    var h = 14695981039346656037ul
    for (i in urange(261u)) {
        let a = half_of(i * 251u)
        let c = half_of((i * 251u) ^ 0x3c55u)
        h = mix(h, hbits(a + c) ^ (hbits(a - c) << 16u))
        h = mix(h, hbits(a * c) ^ (hbits(a / c) << 16u))
        h = mix(h, hbits(-a) ^ ((a < c) ? 0x10000u : 0u) ^ ((a == c) ? 0x20000u : 0u))
    }
    return h
}

[no_jit]
def fp16_ops_interp() : uint64 {
    var h = 14695981039346656037ul
    for (i in urange(261u)) {
        let a = half_of(i * 251u)
        let c = half_of((i * 251u) ^ 0x3c55u)
        h = mix(h, hbits(a + c) ^ (hbits(a - c) << 16u))
        h = mix(h, hbits(a * c) ^ (hbits(a / c) << 16u))
        h = mix(h, hbits(-a) ^ ((a < c) ? 0x10000u : 0u) ^ ((a == c) ? 0x20000u : 0u))
    }
    return h
}

[test(description = "JIT-compiled half to float, float to half and half4 round trips return the interpreter's bits and never fall back to the interpreter")]
def test_half_converts(t : T?) {
    t |> equal(half_to_float_interp(), half_to_float_jit(), "every half -> float")
    t |> equal(float_to_half_interp(), float_to_half_jit(), "float -> half over a stride of the float bit patterns")
    t |> equal(half4_round_trip_interp(), half4_round_trip_jit(), "half4 <-> float4 round trips")
    t |> success(!jit_enabled() || (is_jit_function(@@half_to_float_jit) && is_jit_function(@@float_to_half_jit)
        && is_jit_function(@@half4_round_trip_jit)), "no converting function falls back to the interpreter")
}

[test(description = "JIT-compiled + - * / unary minus < and == on half return the interpreter's results")]
def test_fp16_operators(t : T?) {
    t |> equal(fp16_ops_interp(), fp16_ops_jit(), "+ - * /, unary -, < and == on half")
    t |> success(!jit_enabled() || is_jit_function(@@fp16_ops_jit), "the operators do not fall back to the interpreter")
}

// f16_cvt intrinsic emission: under -jit on aarch64 / x64+F16C the module's f16_to_f32 /
// f32_to_f16 lower to hardware half converts (fpext/fptrunc through half), while the ref_*
// copies below — same bit-twiddle, different module, so the name-keyed lookup never matches —
// compile as-is; integer ops are exact in every tier, so hardware-vs-reference isolates the
// intrinsic. Interp / no-F16C lanes run the identical body on both sides (trivially green).
// NaN inputs compare by predicate only: hardware quietens sNaN / keeps payloads where the
// reference flattens f32->f16 NaN to sign|0x7E00 — both stay NaN, payloads are not contractual.

def ref_f16_to_f32(h : uint) : float {
    let exp = (h >> 10u) & 0x1Fu
    let mant = h & 0x3FFu
    let neg = (h & 0x8000u) != 0u
    if (exp == 0u) {
        let v = float(mant) * 5.9604644775390625e-8
        return neg ? -v : v
    }
    let sign = (h & 0x8000u) << 16u
    var bits : uint
    if (exp == 0x1Fu) {
        bits = sign | 0x7F800000u | (mant << 13u)
    } else {
        bits = sign | ((exp + 112u) << 23u) | (mant << 13u)
    }
    return unsafe(reinterpret<float>(bits))
}

def ref_f32_to_f16(f : float) : uint {
    let bits = unsafe(reinterpret<uint>(f))
    let sign = (bits >> 16u) & 0x8000u
    let absb = bits & 0x7FFFFFFFu
    if (absb > 0x7F800000u) {
        return sign | 0x7E00u
    }
    if (absb >= 0x47800000u) {
        return sign | 0x7C00u
    }
    if (absb >= 0x38800000u) {
        let v = absb - 0x38000000u
        return sign | ((v + 0xFFFu + ((v >> 13u) & 1u)) >> 13u)
    }
    let e = absb >> 23u
    let m = (absb & 0x7FFFFFu) | 0x800000u
    if (e < 102u || (e == 102u && m == 0x800000u)) {
        return sign
    }
    let sh = 126u - e
    let base = m >> sh
    let rem = m & ((1u << sh) - 1u)
    let half = 1u << (sh - 1u)
    let up = rem > half || (rem == half && (base & 1u) == 1u)
    return sign | (up ? base + 1u : base)
}

def is_f16_nan(h : uint) : bool => ((h >> 10u) & 0x1Fu) == 0x1Fu && (h & 0x3FFu) != 0u

def is_f32_nan_bits(v : float) : bool {
    let bits = unsafe(reinterpret<uint>(v))
    return (bits & 0x7F800000u) == 0x7F800000u && (bits & 0x007FFFFFu) != 0u
}

def f32_bits(v : float) : uint => unsafe(reinterpret<uint>(v))

[test(description = "the hardware f16 to f32 conversion equals the reference bitwise for all 65536 half patterns (NaNs by predicate only)")]
def test_f16_to_f32_exhaustive(t : T?) {
    t |> run("all 65536 f16 patterns: hardware == reference bitwise (NaNs predicate-only)") @(t : T?) {
        var bad = 0
        var bad_nan = 0
        for (h in range(65536)) {
            let hu = uint(h)
            let hw = f16_to_f32(hu)
            if (is_f16_nan(hu)) {
                if (!is_f32_nan_bits(hw)) {
                    bad_nan++
                }
            } elif (f32_bits(hw) != f32_bits(ref_f16_to_f32(hu))) {
                bad++
            }
        }
        t |> equal(bad, 0, "non-nan f16->f32 is bit-exact vs the reference")
        t |> equal(bad_nan, 0, "every f16 nan pattern converts to an f32 nan")
    }
}

[test(description = "the hardware f32 to f16 conversion equals the reference across 65536 upper halves crossed with rounding and sticky low patterns")]
def test_f32_to_f16_sweep(t : T?) {
    t |> run("65536 upper halves x round/sticky low patterns: hardware == reference") @(t : T?) {
        // hi exhausts sign+exponent+kept-mantissa bits; lo exercises the round bit (0x1000),
        // sticky combinations, and the tie cases of the RNE decision at the normal-range
        // rounding position (bit 12) and across the moving subnormal shift.
        let lows = fixed_array(0x0000u, 0x1000u, 0x1001u, 0x0FFFu, 0x2000u, 0x8000u, 0xFFFFu)
        var bad = 0
        var bad_nan = 0
        for (hi in range(65536)) {
            for (lo in lows) {
                let bits = (uint(hi) << 16u) | lo
                let f = unsafe(reinterpret<float>(bits))
                let hw = f32_to_f16(f)
                if (is_f32_nan_bits(f)) {
                    if (!is_f16_nan(hw)) {
                        bad_nan++
                    }
                } elif (hw != ref_f32_to_f16(f)) {
                    bad++
                }
            }
        }
        t |> equal(bad, 0, "non-nan f32->f16 matches the reference RNE bit-twiddle")
        t |> equal(bad_nan, 0, "every f32 nan input converts to an f16 nan")
    }
}

[test(description = "f32_to_f16(f16_to_f32(h)) returns h for every non-NaN half pattern")]
def test_f16_round_trip(t : T?) {
    t |> run("f32_to_f16(f16_to_f32(h)) == h for every non-nan pattern") @(t : T?) {
        var bad = 0
        for (h in range(65536)) {
            let hu = uint(h)
            if (is_f16_nan(hu)) {
                continue
            }
            if (f32_to_f16(f16_to_f32(hu)) != hu) {
                bad++
            }
        }
        t |> equal(bad, 0, "all non-nan f16 patterns round-trip")
    }
}

[test(description = "f32 to f16 rounds to nearest even at the boundaries the same way numpy float16 does")]
def test_f32_to_f16_boundaries(t : T?) {
    t |> run("round-to-nearest-even boundaries match numpy float16") @(t : T?) {
        t |> equal(f32_to_f16(0.1), 0x2e66u)
        t |> equal(f32_to_f16(-1.5), 0xbe00u)
        t |> equal(f32_to_f16(65504.0), 0x7bffu)   // f16 max
        t |> equal(f32_to_f16(65519.9), 0x7bffu)   // just under the overflow tie
        t |> equal(f32_to_f16(65520.0), 0x7c00u)   // IEEE overflow to inf
        t |> equal(f32_to_f16(2.9802322387695312e-8), 0x0u)  // exactly 2^-25: tie to even 0
        t |> equal(f32_to_f16(2.9805302e-8), 0x1u)           // a hair above: min subnormal
        t |> equal(f32_to_f16(10.0), 0x4900u)
        t |> equal(f32_bits(f16_to_f32(0x8000u)), 0x80000000u)  // -0 survives f16->f32
        t |> equal(f32_to_f16(unsafe(reinterpret<float>(0x80000000u))), 0x8000u)  // and back
        t |> equal(f32_to_f16(unsafe(reinterpret<float>(0x7F800000u))), 0x7C00u)  // +inf
        t |> equal(f32_bits(f16_to_f32(0xFC00u)), 0xFF800000u)                    // -inf
        let h = f32_to_f16(unsafe(reinterpret<float>(0x7FC00001u)))  // quiet nan in -> nan out
        t |> success(is_f16_nan(h), "nan input yields an f16 nan")
    }
}

def direct_half_json : string {
    let h = 2.5h
    return sprint_json(h, false)
}

[test(description = "a half passed by value to the interop builtin sprint_json rides the vec4f argument slot, so the function stays jitted and prints the value")]
def test_half_by_value_into_interop_stays_jitted(t : T?) {
    t |> equal(direct_half_json(), "2.5")
    t |> success(!jit_enabled() || is_jit_function(@@direct_half_json),
        "a by-value half into an interop builtin no longer declines the function")
}
