options gen2

module aarch64_neon shared public

// AArch64 NEON intrinsics exposed as plain daslang functions, each with a portable fallback body.
//
// Every function here computes the correct result on ANY target via its daslang body — so the
// interpreter, AOT, and non-ARM JIT builds all just run that body. On an aarch64 JIT target the
// LLVM JIT recognizes the call BY NAME (see g_aarch64_intrin_lookup in llvm_jit_intrin.das, gated
// on g_target_is_aarch64) and emits the hardware NEON instruction in place of the call. There is
// nothing to annotate and nothing to require from the llvm module — the recognition is purely
// name-based at JIT codegen time, so the fallback runs verbatim everywhere the intrinsic isn't
// emitted. Callers therefore get the fast path for free on arm64 and stay correct elsewhere.

//! Signed dot-product (`SDOT` / `@llvm.aarch64.neon.sdot`): load 16 int8 from each of `w` and `x`
//! (16-wide unaligned loads), multiply elementwise, and accumulate consecutive groups of 4 products
//! into the 4 lanes of `acc` — lane j += sum over e in 0..3 of w[4j+e]*x[4j+e], exact int32, with
//! NO horizontal reduce. On an aarch64 JIT target this lowers to a single SDOT instruction; on every
//! other target it is the scalar loop below. Pointers are passed pre-offset by the caller.
def sdot4(acc : int4; w : int8 const?; x : int8 const?) : int4 {
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
