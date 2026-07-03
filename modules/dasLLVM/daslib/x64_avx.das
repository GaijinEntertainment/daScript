options gen2

module x64_avx shared public

// x86-64 SIMD intrinsics exposed as plain daslang functions, each with a portable fallback body —
// the x64 mirror of aarch64_neon.das (read that header for the recognition contract).
//
// Every function here computes the correct result on ANY target via its daslang body — so the
// interpreter, AOT, and non-x64 JIT builds all just run that body. On an x86-64 JIT target WITH
// AVX2 the LLVM JIT recognizes the call BY NAME (see g_x64_intrin_lookup in llvm_jit_intrin.das,
// gated on g_target_x64_avx2 — cpuid truth, since AVX2 is not universal on x86-64) and emits the
// hardware instruction sequence in place of the call. Nothing to annotate, nothing to require from
// the llvm module — recognition is purely name-based at JIT codegen time, so the fallback runs
// verbatim everywhere the intrinsic isn't emitted (including AVX2-less x64).

//! Signed dot-product (the SDOT shape on x86): load 16 int8 from each of `w` and `x` (16-wide
//! unaligned loads), multiply elementwise, and accumulate consecutive groups of 4 products into the
//! 4 lanes of `acc` — lane j += sum over e in 0..3 of w[4j+e]*x[4j+e], exact int32, with NO
//! horizontal reduce. Pre-VNNI x86 has no s8×s8 dot, so the x86-64 JIT lowers this via the standard
//! exact sign trick — dot(w, x) == dot(|w| as u8, sign(w) applied to x) — as PABSB/PSIGNB +
//! PMADDUBSW + PMADDWD (ggml's pre-VNNI path); on every other target it is the scalar loop below.
//! EXACTNESS PRECONDITION: `x` values must lie in [-127, 127] (PSIGNB of -128 wraps, and a
//! -128×-128 pair sum would saturate PMADDUBSW). The Q8 quantizer guarantees this by construction
//! (d = amax/127) for every activation it emits; `w` may be ANY int8 including -128 (|−128| = 128
//! fits u8, pair sums stay ≤ 32512 < 32767). Pointers are passed pre-offset by the caller.
def dot4(acc : int4; w : int8 const?; x : int8 const?) : int4 {
    var s0 = 0
    var s1 = 0
    var s2 = 0
    var s3 = 0
    unsafe {
        for (e in range(4)) {
            s0 += int(w[e]) * int(x[e])
            s1 += int(w[4 + e]) * int(x[4 + e])
            s2 += int(w[8 + e]) * int(x[8 + e])
            s3 += int(w[12 + e]) * int(x[12 + e])
        }
    }
    return acc + int4(s0, s1, s2, s3)
}

//! 256-bit dot4: load 32 int8 from each of `w` and `x` (one full Q8_0 block) and accumulate into the
//! 4 lanes of `acc` — lane j += dot of quad j PLUS dot of quad j+4, i.e. exactly
//! dot4(dot4(acc, w, x), w + 16, x + 16), lane for lane. On x86-64 the JIT emits the same sign-trick
//! chain as dot4 on 256-bit registers (VPABSB/VPSIGNB + VPMADDUBSW + VPMADDWD, ymm) and folds the
//! v8i32 result to int4 by adding the high 128-bit half onto the low (VEXTRACTI128 + VPADDD) — that
//! fold IS the quad j + quad j+4 grouping, so the fallback below and the hardware path agree exactly.
//! Same EXACTNESS PRECONDITION as dot4: `x` values in [-127, 127] (the Q8 quantizer's range); `w` may
//! be any int8 including -128. Pointers are passed pre-offset by the caller.
def dot32(acc : int4; w : int8 const?; x : int8 const?) : int4 {
    var s0 = 0
    var s1 = 0
    var s2 = 0
    var s3 = 0
    unsafe {
        for (e in range(4)) {
            s0 += int(w[e]) * int(x[e]) + int(w[16 + e]) * int(x[16 + e])
            s1 += int(w[4 + e]) * int(x[4 + e]) + int(w[20 + e]) * int(x[20 + e])
            s2 += int(w[8 + e]) * int(x[8 + e]) + int(w[24 + e]) * int(x[24 + e])
            s3 += int(w[12 + e]) * int(x[12 + e]) + int(w[28 + e]) * int(x[28 + e])
        }
    }
    return acc + int4(s0, s1, s2, s3)
}

//! Float-accumulating dot32 — the per-block GEMV epilogue fused into the intrinsic:
//! acc + float4(dot32(int4(0), w, x)) * s, lane for lane. On x86-64 the JIT emits dot32's exact
//! sign-trick chain and fold, then VCVTDQ2PS on the int4 partials (each bounded by 32*127*128 <
//! 2^24, so the conversion is exact), a broadcast multiply by `s`, and a vector add into `acc` —
//! replacing the kernel-side scalar hsum + scale chain (~10 µops per block) with 3 vector ops and
//! deferring the horizontal reduce to ONE hsum per row. Lane math matches this fallback exactly
//! under strict FP; under `options _jit_fast_math` LLVM may contract the mul+add into an FMA —
//! the same laxity every float kernel gets, never on tests. Same EXACTNESS PRECONDITION as dot32
//! on `x` (values in [-127, 127]); `s` is the folded per-block scale (weight scale × token scale).
def dot32_ps(acc : float4; w : int8 const?; x : int8 const?; s : float) : float4 {
    let d = dot32(int4(0, 0, 0, 0), w, x)
    return acc + float4(d) * float4(s, s, s, s)
}

//! 8-wide float-accumulating dot32 — llama.cpp's vec_dot per-block shape. `acc` points at 8 floats
//! (a caller-local float[8]); lane j accumulates the dot of byte quad j: acc[j] += float(sum over e
//! in 0..3 of w[4j+e]*x[4j+e]) * s. On x86-64 the JIT emits dot32's exact sign-trick chain WITHOUT
//! the high/low fold — the 8 int32 quads convert straight to float (VCVTDQ2PS ymm, exact: each quad
//! bounded < 2^24) and multiply-accumulate into the 8-lane accumulator as one load-op-store that
//! LICM promotes into a ymm register across a block loop. No per-block fold or hsum; the caller does
//! ONE horizontal reduce per row. Lane for lane this fallback under strict FP; `options
//! _jit_fast_math` may contract the mul+add into VFMADD — the same laxity every float kernel gets,
//! never on tests. Same EXACTNESS PRECONDITION as dot32 on `x` (values in [-127, 127]); `s` is the
//! folded per-block scale (weight scale × token scale).
def dot32_acc8(var acc : float?; w : int8 const?; x : int8 const?; s : float) {
    unsafe {
        for (j in range(8)) {
            var q = 0
            for (e in range(4)) {
                q += int(w[j * 4 + e]) * int(x[j * 4 + e])
            }
            acc[j] += float(q) * s
        }
    }
}
