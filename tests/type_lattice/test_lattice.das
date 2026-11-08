options gen2

require dastest/testing_boost public

// hand-written (NOT generated): the idot family — exact integer dot products and the byte
// table shuffle, the first compute ops on the 8-bit lattice. Values are hand-computed so
// every tier (interp, JIT native/generic, AOT verbatim-extern) must reproduce them bit-exact.
// Compute lives in plain helpers (T? params keep a function off the JIT — the lambdas only assert).

def private mk_a() : byte16 => byte16(1, -2, 3, -4, 5, -6, 7, -8, 9, -10, 11, -12, 13, -14, 15, -16)
def private mk_b() : byte16 => byte16(2, 3, -4, 5, -6, 7, 8, -9, 10, 11, -12, 13, -14, 15, 16, -17)
// full-range unsigned lanes (255, 128) prove zero-extension of a
def private mk_ua() : ubyte16 => ubyte16(255u, 2u, 3u, 4u, 5u, 6u, 7u, 8u, 9u, 10u, 11u, 12u, 13u, 14u, 15u, 128u)

def private idot4_ss_pure() : int4 => idot4(mk_a(), mk_b())
def private idot4_ss_acc() : int4 => idot4(int4(100, 200, 300, 400), mk_a(), mk_b())
def private idot4_us_pure() : int4 => idot4(mk_ua(), mk_b())
def private idot4_us_acc() : int4 => idot4(int4(1, 2, 3, 4), mk_ua(), mk_b())
def private idot_ss() : int => idot(mk_a(), mk_b())
def private idot_us() : int => idot(mk_ua(), mk_b())

def private splat_b16(v : int) : byte16 => byte16(v)

def private splat_ub16(v : uint) : ubyte16 => ubyte16(v)

def private idot4_extreme_ss() : int4 => idot4(splat_b16(-128), splat_b16(127))
def private idot_extreme_ss() : int => idot(splat_b16(-128), splat_b16(127))
def private idot4_extreme_us() : int4 => idot4(splat_ub16(255u), splat_b16(-128))
def private idot_extreme_us() : int => idot(splat_ub16(255u), splat_b16(-128))

def private mk_lut() : byte16 => byte16(10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, -10, -20, -30, -40)

def private shuffle_masked() : byte16 {
    // indices 31 and 16 exercise the &15 mask (tbl-with-mask / pshufb-positive semantic)
    return shuffle(mk_lut(), ubyte16(15u, 14u, 13u, 12u, 0u, 1u, 2u, 3u, 31u, 16u, 5u, 5u, 5u, 5u, 7u, 8u))
}

def private shuffle_identity() : byte16 =>
    shuffle(mk_lut(), ubyte16(0u, 1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u, 9u, 10u, 11u, 12u, 13u, 14u, 15u))

[test(description = "idot4, idot and the byte table shuffle give the hand-computed results with exact lanes on every tier")]
def test_lattice_idot(t : T?) {
    t |> run("idot4 signed x signed") @(t : T?) {
        t |> success(idot4_ss_pure() == int4(-36, 56, -308, 120), "pure form")
        t |> success(idot4_ss_acc() == int4(64, 256, -8, 520), "accumulate form")
    }
    t |> run("idot4 unsigned x signed") @(t : T?) {
        let r = idot4_us_pure()
        t |> equal(r.x, 524)
        t |> equal(r.w, -1908)
        t |> success(idot4_us_acc() == r + int4(1, 2, 3, 4), "accumulate form")
    }
    t |> run("idot full reduce") @(t : T?) {
        t |> equal(idot_ss(), -168)
        let m = idot4_us_pure()
        t |> equal(idot_us(), m.x + m.y + m.z + m.w)
    }
    t |> run("idot extremes do not overflow lanes") @(t : T?) {
        // most negative product set: (-128 * 127) * 16 = -260096 — far inside int32, must be exact
        t |> success(idot4_extreme_ss() == int4(-65024, -65024, -65024, -65024), "sxs lane sums")
        t |> equal(idot_extreme_ss(), -260096)
        t |> success(idot4_extreme_us() == int4(-130560, -130560, -130560, -130560), "uxs lane sums")
        t |> equal(idot_extreme_us(), -522240)
    }
    t |> run("shuffle byte table") @(t : T?) {
        t |> success(shuffle_masked() == byte16(-40, -30, -20, -10, 10, 20, 30, 40, -40, 10, 60, 60, 60, 60, 80, 90), "masked select")
        t |> success(shuffle_identity() == mk_lut(), "identity round-trip")
    }
}

// hand-written (NOT generated): value-reinterpret consuming a lattice-returning extern
// DIRECTLY (no local hop) makes the interpreter read the call node through a typed eval
// slot (evalUInt/evalUInt64) — the interop dispatch must round-trip the vec4f slot for
// lattice results there. Regression for the W7 GPU-parity-gate catch: with a local hop
// or const-folded arguments the shape never executes, which is how it slipped the
// original lattice conformance suite.

// runtime-opaque zeros: seeded from argv length so no optimizer pass can fold the
// lattice ctor calls below (a plain never-written `var` global gets const-promoted)
var runtime_fzero = 0.0
var runtime_izero = 0

[init]
def seed_runtime_zeros {
    let args <- get_command_line_arguments()
    // float mul-by-zero can't fold (NaN rules), int mul-by-zero folds — route through float
    runtime_fzero = float(length(args)) * 0.0
    runtime_izero = int(runtime_fzero)
}

def pack_bits(v : float2) : uint => unsafe(reinterpret<uint>(half2(v)))

[test(description = "reinterpret of a lattice constructor call composes with the constructor in both directions at 32 and 64 bits")]
def test_lattice_reinterpret_composition(t : T?) {
    // every composition binds a `let` local: the typed local-set (Set_TT<uint>) is the
    // consumer that reads the extern call through evalUInt — argument-position use goes
    // through the safe vec4f path and would not regress
    t |> run("32-bit: reinterpret<uint> straight off the ctor call") @(t : T?) {
        let v = float2(1.5 + runtime_fzero, 2.0 + runtime_fzero)
        let got_half2 = unsafe(reinterpret<uint>(half2(v)))
        t |> equal(0x40003e00, got_half2)
        let got_fn = pack_bits(v)
        t |> equal(0x40003e00, got_fn)
        let got_short2 = unsafe(reinterpret<uint>(short2(int2(1000 + runtime_izero, -2000))))
        t |> equal(0xf83003e8, got_short2)
        let got_byte4 = unsafe(reinterpret<uint>(byte4(int4(1 + runtime_izero, 2, 3, -4))))
        t |> equal(0xfc030201, got_byte4)
    }
    t |> run("64-bit: reinterpret<uint64> straight off the ctor call") @(t : T?) {
        let got_half4 = unsafe(reinterpret<uint64>(half4(float4(1.5 + runtime_fzero, 0., 0., 2.0))))
        t |> equal(0x4000000000003e00ul, got_half4)
        let got_short4 = unsafe(reinterpret<uint64>(short4(int4(1000 + runtime_izero, -2000, 3000, -4000))))
        t |> equal(0xf0600bb8f83003e8ul, got_short4)
    }
    t |> run("reverse direction: lattice from reinterpreted runtime scalar") @(t : T?) {
        let bits = 0x40003e00 + uint(runtime_izero)
        let up = float2(unsafe(reinterpret<half2>(bits)))
        t |> equal(float2(1.5, 2.0), up)
    }
}

// hand-written (NOT generated): user-defined operators involving lattice types must
// resolve to the user function on every tier — under JIT they route to the function
// path, never the native lattice op lowering (which only knows the builtin signatures)

def operator ==(a : short4; b : int4) : bool => int4(a) == b

def operator +(a : half4; b : float) : half4 => a + half4(float16(b))

def use_eq(a : short4; b : int4) : bool => a == b

def use_add(a : half4; b : float) : half4 => a + b

[test(description = "user-defined operators on lattice types resolve to the user function instead of the builtin lowering")]
def test_lattice_user_operators(t : T?) {
    t |> run("mixed equality routes to the user operator") @(t : T?) {
        t |> success(use_eq(short4(1, 2, 3, 4), int4(1, 2, 3, 4)), "equal")
        t |> success(!use_eq(short4(1, 2, 3, 4), int4(1, 2, 3, 5)), "not equal")
    }
    t |> run("mixed add routes to the user operator") @(t : T?) {
        let r = use_add(half4(1h, 2h, 3h, 4h), 0.5)
        t |> success(r == half4(1.5h, 2.5h, 3.5h, 4.5h), "sum")
    }
}
