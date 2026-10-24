options gen2
options indenting = 4
options no_aot   // the stand-ins run only inside a fixture's CPU replay; under AOT the stub's semantic hash of the ones a replay reaches differs from the runtime's (LEDGER.md), so the replay calls them interpreted

// The CUDA-only tier of the kernel vocabulary: the primitives a hand-shaped kernel needs that the
// shared GLSL/SPIR-V surface (`cuda_builtins`) has no twin for - a body that calls one is a CUDA-only
// kernel by construction. Every declaration is an AST marker `cuda_emit` lowers by NAME, spelled as
// PTX/CUDA spells it; the CPU bodies emulate the pure lane functions (so a CPU reference can run
// them) and stand in as width-1 identities or no-ops for the rest.

module cuda_intrinsics shared public

require spirv/spirv_builtins public
require math   // the conversions' CPU bodies: abs, floor, min

[sideeffects, arch(at = "../ARCHITECTURE_EMIT.md#cuda-kernel-vocabulary-tiers")]
def public cp_async_16(var dst : uint4?; src : uint4?) : void {
    //! the CPU form copies the 16 bytes in place: the oracle run fills its staging arrays as the device does
    *dst = *src
}
[sideeffects] def public cp_async_commit() : void {}
//! cp.async.wait_group `pending`: a constant - the groups still allowed in flight
[unused_argument(pending), sideeffects] def public cp_async_wait(pending : int) : void {}

[unused_argument(m, p), sideeffects, arch(at = "../ARCHITECTURE_EMIT.md#cuda-kernel-vocabulary-tiers")]
def public ldmatrix(var m : coopmatA_f16_16x16; p : uint4?) : void {}
[unused_argument(m, p), sideeffects, arch(at = "../ARCHITECTURE_EMIT.md#cuda-kernel-vocabulary-tiers")]
def public ldmatrix(var m : coopmatB_f16_16x16; p : uint4?) : void {}
[unused_argument(m, p), sideeffects, arch(at = "../ARCHITECTURE_EMIT.md#cuda-kernel-vocabulary-tiers")]
def public ldmatrix(var m : coopmatA_s8_16x32; p : uint4?) : void {}
[unused_argument(m, p), sideeffects, arch(at = "../ARCHITECTURE_EMIT.md#cuda-kernel-vocabulary-tiers")]
def public ldmatrix(var m : coopmatB_s8_32x16; p : uint4?) : void {}

[unused_argument(f, s, rs, cs), sideeffects, arch(at = "../ARCHITECTURE_EMIT.md#cuda-kernel-vocabulary-tiers")]
def public mma_scale_acc(var f : coopmatAcc_f32_16x16; s : coopmatAcc_s32_16x16; rs : float2; cs : float4) : void {}

//! the FP4 tensor rung (sm_120a): an e2m1 A fragment of 16 rows x 64 k - four words a lane, k 8t..8t+7 and 32+8t.. of rows g
//! and g + 8, the s8 fragment's lane map over nibbles - and a B fragment of 64 k x 16 n as two n-halves of two words
struct coopmatA_e2m1_16x64 {}
struct coopmatB_e2m1_64x16 {}

[unused_argument(m, p), sideeffects, arch(at = "../ARCHITECTURE_EMIT.md#cuda-kernel-vocabulary-tiers")]
def public ldmatrix(var m : coopmatA_e2m1_16x64; p : uint4?) : void {}
[unused_argument(m, p), sideeffects, arch(at = "../ARCHITECTURE_EMIT.md#cuda-kernel-vocabulary-tiers")]
def public ldmatrix(var m : coopmatB_e2m1_64x16; p : uint4?) : void {}

//! the block-scaled FP4 product: f += A x B over k64 with a UE4M3 scale a k16 strip - `sa` the four scale bytes of the lane's
//! A row (row g from the lanes with lane % 4 == 0, row g + 8 from lane % 4 == 1, the other lanes' words ignored), `sb0` / `sb1`
//! the four scale bytes of column g of each n-half (from the lanes with lane % 4 == 0); every value e2m1 x ue4m3, no halving
[unused_argument(f, a, b, sa, sb0, sb1), sideeffects, arch(at = "../ARCHITECTURE_EMIT.md#cuda-kernel-vocabulary-tiers")]
def public mma_block_scaled(var f : coopmatAcc_f32_16x16; a : coopmatA_e2m1_16x64; b : coopmatB_e2m1_64x16; sa, sb0, sb1 : uint) : void {}

//! f's columns scaled: `cs` the lane's four columns' factors in `mma_scale_acc`'s order (2t, 2t + 1, 8 + 2t, 9 + 2t) - the FP4
//! product's per-token epilogue, where the block-scaled mma left no s32 stage to fold a column scale into
[unused_argument(f, cs), sideeffects, arch(at = "../ARCHITECTURE_EMIT.md#cuda-kernel-vocabulary-tiers")]
def public mma_scale_cols(var f : coopmatAcc_f32_16x16; cs : float4) : void {}

let private E2M1_MAG : float[8] = fixed_array<float>(0.0, 0.5, 1.0, 1.5, 2.0, 3.0, 4.0, 6.0)

//! the e2m1 code of `x`: the nearest of {0, 0.5, 1, 1.5, 2, 3, 4, 6} at the sign, a tie to the even code, past 6 saturated
def private e2m1_code(x : float) : uint {
    let a = min(abs(x), 6.0)
    var best = 0u
    var berr = a
    for (c in range(1, 8)) {
        let e = abs(a - E2M1_MAG[c])
        if (e < berr || (e == berr && (c & 1) == 0)) {
            best = uint(c)
            berr = e
        }
    }
    return (x < 0.0 ? 8u : 0u) | best
}

//! two floats to an e2m1 pair, `lo` in the low nibble: `cvt.rn.satfinite.e2m1x2.f32` (round to nearest even, saturating)
[arch(at = "../ARCHITECTURE_EMIT.md#cuda-kernel-vocabulary-tiers")]
def public cvt_e2m1x2(lo, hi : float) : uint => e2m1_code(lo) | (e2m1_code(hi) << 4u)

//! a non-negative float to its UE4M3 code (bias 7, three mantissa bits, subnormals m x 2^-9): `cvt.rn.satfinite.e4m3x2.f32`'s
//! low byte - round to nearest even, 448 and past it the largest finite code 0x7E, nothing positive below zero
[arch(at = "../ARCHITECTURE_EMIT.md#cuda-kernel-vocabulary-tiers")]
def public cvt_ue4m3(x : float) : uint {
    if (!(x > 0.0)) return 0u
    if (x >= 448.0) return 0x7Eu
    let b = unsafe(reinterpret<uint>(x))
    let e = int(b >> 23u) - 127
    if (e < -6) {
        let q = x * 512.0
        let f = floor(q)
        let d = q - f
        let up = d > 0.5 || (d == 0.5 && (int(f) & 1) == 1)
        return uint(int(f) + (up ? 1 : 0))
    }
    var m3 = (b >> 20u) & 7u
    let rem = b & 0xFFFFFu
    if (rem > 0x80000u || (rem == 0x80000u && (m3 & 1u) == 1u)) {
        m3++
    }
    return min((uint(e + 7) << 3u) + m3, 0x7Eu)
}

//! bar.sync id, count: a named barrier over `count` threads (a multiple of the warp)
[unused_argument(id, count), sideeffects] def public bar_sync(id, count : uint) : void {}

//! __byte_perm: result byte i is byte `sel` nibble i of the eight bytes {b, a} (0-7); the nibble's high bit is ignored, as the
//! CUDA intrinsic ignores it (the oracle fixture's odd lanes set one and pin this)
def public byte_perm(a, b, sel : uint) : uint {
    var r = 0u
    for (i in range(4)) {
        let n = (sel >> uint(i * 4)) & 7u
        let byte = n < 4u ? (a >> (n * 8u)) & 0xffu : (b >> ((n - 4u) * 8u)) & 0xffu
        r |= byte << uint(i * 8)
    }
    return r
}

//! the SM's cycle counter, low 32 bits (a kernel's locals are 32-bit): differences inside one kernel
[sideeffects] def public clock32() : uint => uint(ref_time_ticks())

[unused_argument(fmt), sideeffects] def public cuda_asm(fmt : string) : void {}
[unused_argument(fmt, a0), sideeffects] def public cuda_asm(fmt : string; a0 : uint) : void {}
[unused_argument(fmt, a0, a1), sideeffects] def public cuda_asm(fmt : string; a0, a1 : uint) : void {}
[unused_argument(fmt, a0, a1, a2), sideeffects] def public cuda_asm(fmt : string; a0, a1, a2 : uint) : void {}
[unused_argument(fmt), sideeffects] def public cuda_asm_value(fmt : string) : uint => 0u
[unused_argument(fmt, a0), sideeffects] def public cuda_asm_value(fmt : string; a0 : uint) : uint => 0u
[unused_argument(fmt, a0, a1), sideeffects] def public cuda_asm_value(fmt : string; a0, a1 : uint) : uint => 0u
[unused_argument(fmt, a0, a1, a2), sideeffects] def public cuda_asm_value(fmt : string; a0, a1, a2 : uint) : uint => 0u
