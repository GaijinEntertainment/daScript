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

//! bar.sync id, count: a named barrier over `count` threads (a multiple of the warp)
[unused_argument(id, count), sideeffects] def public bar_sync(id, count : uint) : void {}

//! __byte_perm: result byte i is byte `sel` nibble i of the eight bytes {b, a} (0-7); the nibble's high bit is ignored, as the
//! CUDA intrinsic ignores it (the oracle fixture pins this)
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
