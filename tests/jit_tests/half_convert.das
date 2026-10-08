options gen2
require dastest/testing_boost

// Each [jit] function and its [no_jit] twin run the same half converts; the results must match.
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

[test]
def test_half_converts(t : T?) {
    t |> equal(half_to_float_interp(), half_to_float_jit(), "every half -> float")
    t |> equal(float_to_half_interp(), float_to_half_jit(), "float -> half over a stride of the float bit patterns")
    t |> equal(half4_round_trip_interp(), half4_round_trip_jit(), "half4 <-> float4 round trips")
    t |> success(!jit_enabled() || (is_jit_function(@@half_to_float_jit) && is_jit_function(@@float_to_half_jit)
        && is_jit_function(@@half4_round_trip_jit)), "no converting function falls back to the interpreter")
}
