options gen2
options persistent_heap

require dastest/testing_boost
require metal/msl_shader
require ?das_metal metal/das_metal_boost  // nolint:STYLE030 — GPU driver half, Apple builds only
require ?das_metal dasllama/dasllama_math_metal  // nolint:STYLE030 — the production kernel MSL (v0 baseline)
require dasllama/dasllama_lcpp_mulmm  // nolint:STYLE030 — llama.cpp's verbatim kernel_mul_mm_q8_0_f32, used only in the Apple half
require daslib/fio
require math  // nolint:STYLE030 — used only inside the Apple static_if half; off-Apple lint sees the branch compiled out
require strings

// The Metal Q8 GEMM kernel lab: candidate kernel variants head-to-head against the PRODUCTION
// kernel (v0 = the exact MSL text dasllama_math_metal ships — zero copy drift), in ONE process,
// timed by GPU timestamps (with_compute_encoder_timed) so CPU-side noise and driver memcpys
// never pollute the signal. Rounds interleave variants to neutralize thermal drift.
//
// Correctness is bit-exact by construction: quants are confined to [-8, 7] and scales to powers
// of two, so every f16 staged value, every product, and every f32 partial sum is an exactly
// representable integer times a power of two (max |y| ~2.1M < 2^24) — any accumulation order
// yields identical bits. Each variant must match v0 exactly; v0 must match a CPU reference.
//
//   bin/daslang -jit dastest/dastest.das -- --bench --test modules/dasLLAMA/benchmarks/matmul/bench_metal_gemm_kernels.das
//
// DASLLAMA_BENCH_NTOK: batch width (default 512)
// DASMETAL_LAB_ROUNDS: interleaved timing rounds (default 3, best-of)
// DASMETAL_LAB_SHAPES / DASMETAL_LAB_VARIANTS: substring filters for fast iteration

// ===== kernel variants =====
// All variants share the production buffer contract: bindings 0-6 = wq, ws, xq, xs, y, kdim,
// ndim; 128 threads (4 simdgroups) per threadgroup; W is [d x n] row-major Q8, X is [M' x n]
// with M' padded to the variant's M tile. The MSL emitter is pure das, so the classes compile
// on every platform — only the driver below is Apple-gated.

// v1: 32x64 C tile — same 32-token M block (no driver M-pad change if it lands), double the
// output columns per threadgroup: 3072 staged elements buy 65536 MACs (21.3/elem vs 16 at
// 32x32) and each simdgroup runs a 16x32 quadrant (8 accumulators, 6 loads per k8 step).
class MetalGemmLabN64 {
    @ssbo @binding = 0 wq : array<int8>
    @ssbo @binding = 1 ws : array<float>
    @ssbo @binding = 2 xq : array<int8>
    @ssbo @binding = 3 xs : array<float>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 kdim : uint
    @uniform @binding = 6 ndim : uint
    @workgroup ta : float16[1024]           // A block: 32 tokens x 32 k
    @workgroup tb : float16[2048]           // B block: 32 k x 64 outputs, transposed

    [metal_kernel(name="lab_n64_msl")]
    def lab_n64 {
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let ntileN = ndim / 64u
        let mBase = (gl_WorkGroupID.x / ntileN) * 32u
        let nBase = (gl_WorkGroupID.x % ntileN) * 64u
        let qm = (sg / 2u) * 16u            // this simdgroup's 16x32 quadrant of the C block
        let qn = (sg % 2u) * 32u
        var c00 : simdgroup_float8x8
        var c01 : simdgroup_float8x8
        var c02 : simdgroup_float8x8
        var c03 : simdgroup_float8x8
        var c10 : simdgroup_float8x8
        var c11 : simdgroup_float8x8
        var c12 : simdgroup_float8x8
        var c13 : simdgroup_float8x8
        var kb = 0u
        while (kb * 32u < kdim) {
            var e = lid                     // stage A: 1024 elements, 8 per thread
            while (e < 1024u) {
                let r = e / 32u
                let c = e % 32u
                let k = kb * 32u + c
                ta[e] = float16(float(xq[(mBase + r) * kdim + k]) * xs[(mBase + r) * (kdim / 32u) + kb])
                e += 128u
            }
            e = lid                         // stage B: 2048 elements, 16 per thread
            while (e < 2048u) {
                let r = e / 32u
                let c = e % 32u
                let k = kb * 32u + c
                tb[c * 64u + r] = float16(float(wq[(nBase + r) * kdim + k]) * ws[(nBase + r) * (kdim / 32u) + kb])
                e += 128u
            }
            barrier()
            var k8 = 0u
            while (k8 < 32u) {
                var a0 : simdgroup_half8x8
                var a1 : simdgroup_half8x8
                var b0 : simdgroup_half8x8
                var b1 : simdgroup_half8x8
                var b2 : simdgroup_half8x8
                var b3 : simdgroup_half8x8
                simdgroup_load(a0, ta, qm * 32u + k8, 32u)
                simdgroup_load(a1, ta, (qm + 8u) * 32u + k8, 32u)
                simdgroup_load(b0, tb, k8 * 64u + qn, 64u)
                simdgroup_load(b1, tb, k8 * 64u + (qn + 8u), 64u)
                simdgroup_load(b2, tb, k8 * 64u + (qn + 16u), 64u)
                simdgroup_load(b3, tb, k8 * 64u + (qn + 24u), 64u)
                simdgroup_multiply_accumulate(c00, a0, b0, c00)
                simdgroup_multiply_accumulate(c01, a0, b1, c01)
                simdgroup_multiply_accumulate(c02, a0, b2, c02)
                simdgroup_multiply_accumulate(c03, a0, b3, c03)
                simdgroup_multiply_accumulate(c10, a1, b0, c10)
                simdgroup_multiply_accumulate(c11, a1, b1, c11)
                simdgroup_multiply_accumulate(c12, a1, b2, c12)
                simdgroup_multiply_accumulate(c13, a1, b3, c13)
                k8 += 8u
            }
            barrier()
            kb++
        }
        let row0 = mBase + qm
        let col0 = nBase + qn
        simdgroup_store(c00, y, row0 * ndim + col0, ndim)
        simdgroup_store(c01, y, row0 * ndim + (col0 + 8u), ndim)
        simdgroup_store(c02, y, row0 * ndim + (col0 + 16u), ndim)
        simdgroup_store(c03, y, row0 * ndim + (col0 + 24u), ndim)
        simdgroup_store(c10, y, (row0 + 8u) * ndim + col0, ndim)
        simdgroup_store(c11, y, (row0 + 8u) * ndim + (col0 + 8u), ndim)
        simdgroup_store(c12, y, (row0 + 8u) * ndim + (col0 + 16u), ndim)
        simdgroup_store(c13, y, (row0 + 8u) * ndim + (col0 + 24u), ndim)
    }
}

// v2: 64x32 C tile — the ggml mul_mm block shape. Same 21.3 MACs per staged element as v1 but
// the doubled dimension is M, so the driver would pad activations to 64 rows if this lands.
class MetalGemmLabM64 {
    @ssbo @binding = 0 wq : array<int8>
    @ssbo @binding = 1 ws : array<float>
    @ssbo @binding = 2 xq : array<int8>
    @ssbo @binding = 3 xs : array<float>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 kdim : uint
    @uniform @binding = 6 ndim : uint
    @workgroup ta : float16[2048]           // A block: 64 tokens x 32 k
    @workgroup tb : float16[1024]           // B block: 32 k x 32 outputs, transposed

    [metal_kernel(name="lab_m64_msl")]
    def lab_m64 {
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let ntileN = ndim / 32u
        let mBase = (gl_WorkGroupID.x / ntileN) * 64u
        let nBase = (gl_WorkGroupID.x % ntileN) * 32u
        let qm = (sg / 2u) * 32u            // this simdgroup's 32x16 quadrant of the C block
        let qn = (sg % 2u) * 16u
        var c00 : simdgroup_float8x8
        var c01 : simdgroup_float8x8
        var c10 : simdgroup_float8x8
        var c11 : simdgroup_float8x8
        var c20 : simdgroup_float8x8
        var c21 : simdgroup_float8x8
        var c30 : simdgroup_float8x8
        var c31 : simdgroup_float8x8
        var kb = 0u
        while (kb * 32u < kdim) {
            var e = lid                     // stage A: 2048 elements, 16 per thread
            while (e < 2048u) {
                let r = e / 32u
                let c = e % 32u
                let k = kb * 32u + c
                ta[e] = float16(float(xq[(mBase + r) * kdim + k]) * xs[(mBase + r) * (kdim / 32u) + kb])
                e += 128u
            }
            e = lid                         // stage B: 1024 elements, 8 per thread
            while (e < 1024u) {
                let r = e / 32u
                let c = e % 32u
                let k = kb * 32u + c
                tb[c * 32u + r] = float16(float(wq[(nBase + r) * kdim + k]) * ws[(nBase + r) * (kdim / 32u) + kb])
                e += 128u
            }
            barrier()
            var k8 = 0u
            while (k8 < 32u) {
                var a0 : simdgroup_half8x8
                var a1 : simdgroup_half8x8
                var a2 : simdgroup_half8x8
                var a3 : simdgroup_half8x8
                var b0 : simdgroup_half8x8
                var b1 : simdgroup_half8x8
                simdgroup_load(a0, ta, qm * 32u + k8, 32u)
                simdgroup_load(a1, ta, (qm + 8u) * 32u + k8, 32u)
                simdgroup_load(a2, ta, (qm + 16u) * 32u + k8, 32u)
                simdgroup_load(a3, ta, (qm + 24u) * 32u + k8, 32u)
                simdgroup_load(b0, tb, k8 * 32u + qn, 32u)
                simdgroup_load(b1, tb, k8 * 32u + (qn + 8u), 32u)
                simdgroup_multiply_accumulate(c00, a0, b0, c00)
                simdgroup_multiply_accumulate(c01, a0, b1, c01)
                simdgroup_multiply_accumulate(c10, a1, b0, c10)
                simdgroup_multiply_accumulate(c11, a1, b1, c11)
                simdgroup_multiply_accumulate(c20, a2, b0, c20)
                simdgroup_multiply_accumulate(c21, a2, b1, c21)
                simdgroup_multiply_accumulate(c30, a3, b0, c30)
                simdgroup_multiply_accumulate(c31, a3, b1, c31)
                k8 += 8u
            }
            barrier()
            kb++
        }
        let row0 = mBase + qm
        let col0 = nBase + qn
        simdgroup_store(c00, y, row0 * ndim + col0, ndim)
        simdgroup_store(c01, y, row0 * ndim + (col0 + 8u), ndim)
        simdgroup_store(c10, y, (row0 + 8u) * ndim + col0, ndim)
        simdgroup_store(c11, y, (row0 + 8u) * ndim + (col0 + 8u), ndim)
        simdgroup_store(c20, y, (row0 + 16u) * ndim + col0, ndim)
        simdgroup_store(c21, y, (row0 + 16u) * ndim + (col0 + 8u), ndim)
        simdgroup_store(c30, y, (row0 + 24u) * ndim + col0, ndim)
        simdgroup_store(c31, y, (row0 + 24u) * ndim + (col0 + 8u), ndim)
    }
}

// v3: 32x32 double-buffered — each tile array holds two 1024-element halves; iteration kb
// stages block kb+1 into the idle half while the matrix math consumes block kb, and ONE
// barrier per iteration replaces the production kernel's two.
class MetalGemmLabDb32 {
    @ssbo @binding = 0 wq : array<int8>
    @ssbo @binding = 1 ws : array<float>
    @ssbo @binding = 2 xq : array<int8>
    @ssbo @binding = 3 xs : array<float>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 kdim : uint
    @uniform @binding = 6 ndim : uint
    @workgroup ta : float16[2048]           // two 32x32 A halves
    @workgroup tb : float16[2048]           // two 32x32 B halves (transposed)

    [metal_kernel(name="lab_db32_msl")]
    def lab_db32 {
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let ntileN = ndim / 32u
        let mBase = (gl_WorkGroupID.x / ntileN) * 32u
        let nBase = (gl_WorkGroupID.x % ntileN) * 32u
        let qm = (sg / 2u) * 16u
        let qn = (sg % 2u) * 16u
        var c00 : simdgroup_float8x8
        var c01 : simdgroup_float8x8
        var c10 : simdgroup_float8x8
        var c11 : simdgroup_float8x8
        let nkb = kdim / 32u
        var e = lid                         // prologue: stage block 0 into half 0
        while (e < 1024u) {
            let r = e / 32u
            let c = e % 32u
            ta[e] = float16(float(xq[(mBase + r) * kdim + c]) * xs[(mBase + r) * (kdim / 32u)])
            tb[c * 32u + r] = float16(float(wq[(nBase + r) * kdim + c]) * ws[(nBase + r) * (kdim / 32u)])
            e += 128u
        }
        barrier()
        var kb = 0u
        while (kb < nkb) {
            let cur = (kb % 2u) * 1024u
            if (kb + 1u < nkb) {            // stage block kb+1 into the idle half — the barrier
                let nxt = ((kb + 1u) % 2u) * 1024u
                let kn = kb + 1u            // closing iteration kb-1 made it safe to overwrite
                var e2 = lid
                while (e2 < 1024u) {
                    let r = e2 / 32u
                    let c = e2 % 32u
                    let k = kn * 32u + c
                    ta[nxt + e2] = float16(float(xq[(mBase + r) * kdim + k]) * xs[(mBase + r) * (kdim / 32u) + kn])
                    tb[nxt + c * 32u + r] = float16(float(wq[(nBase + r) * kdim + k]) * ws[(nBase + r) * (kdim / 32u) + kn])
                    e2 += 128u
                }
            }
            var k8 = 0u
            while (k8 < 32u) {
                var a0 : simdgroup_half8x8
                var a1 : simdgroup_half8x8
                var b0 : simdgroup_half8x8
                var b1 : simdgroup_half8x8
                simdgroup_load(a0, ta, cur + qm * 32u + k8, 32u)
                simdgroup_load(a1, ta, cur + (qm + 8u) * 32u + k8, 32u)
                simdgroup_load(b0, tb, cur + k8 * 32u + qn, 32u)
                simdgroup_load(b1, tb, cur + k8 * 32u + (qn + 8u), 32u)
                simdgroup_multiply_accumulate(c00, a0, b0, c00)
                simdgroup_multiply_accumulate(c01, a0, b1, c01)
                simdgroup_multiply_accumulate(c10, a1, b0, c10)
                simdgroup_multiply_accumulate(c11, a1, b1, c11)
                k8 += 8u
            }
            barrier()                       // math read of `cur` done AND next-half writes visible
            kb++
        }
        let row0 = mBase + qm
        let col0 = nBase + qn
        simdgroup_store(c00, y, row0 * ndim + col0, ndim)
        simdgroup_store(c01, y, row0 * ndim + (col0 + 8u), ndim)
        simdgroup_store(c10, y, (row0 + 8u) * ndim + col0, ndim)
        simdgroup_store(c11, y, (row0 + 8u) * ndim + (col0 + 8u), ndim)
    }
}

// v4: 32x32 C tile, 64-wide k step — two Q8 blocks staged per barrier round, halving the
// barrier count per k element; requires n % 64 == 0 (true across the model fleet).
class MetalGemmLabK64 {
    @ssbo @binding = 0 wq : array<int8>
    @ssbo @binding = 1 ws : array<float>
    @ssbo @binding = 2 xq : array<int8>
    @ssbo @binding = 3 xs : array<float>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 kdim : uint
    @uniform @binding = 6 ndim : uint
    @workgroup ta : float16[2048]           // A block: 32 tokens x 64 k
    @workgroup tb : float16[2048]           // B block: 64 k x 32 outputs, transposed

    [metal_kernel(name="lab_k64_msl")]
    def lab_k64 {
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let ntileN = ndim / 32u
        let mBase = (gl_WorkGroupID.x / ntileN) * 32u
        let nBase = (gl_WorkGroupID.x % ntileN) * 32u
        let qm = (sg / 2u) * 16u
        let qn = (sg % 2u) * 16u
        var c00 : simdgroup_float8x8
        var c01 : simdgroup_float8x8
        var c10 : simdgroup_float8x8
        var c11 : simdgroup_float8x8
        var kb = 0u
        while (kb * 64u < kdim) {
            var e = lid                     // stage A: 32 rows x 64 k, 16 per thread
            while (e < 2048u) {
                let r = e / 64u
                let c = e % 64u
                let k = kb * 64u + c
                ta[e] = float16(float(xq[(mBase + r) * kdim + k]) * xs[(mBase + r) * (kdim / 32u) + kb * 2u + c / 32u])
                e += 128u
            }
            e = lid                         // stage B: 64 k x 32 outputs, 16 per thread
            while (e < 2048u) {
                let r = e / 64u
                let c = e % 64u
                let k = kb * 64u + c
                tb[c * 32u + r] = float16(float(wq[(nBase + r) * kdim + k]) * ws[(nBase + r) * (kdim / 32u) + kb * 2u + c / 32u])
                e += 128u
            }
            barrier()
            var k8 = 0u
            while (k8 < 64u) {
                var a0 : simdgroup_half8x8
                var a1 : simdgroup_half8x8
                var b0 : simdgroup_half8x8
                var b1 : simdgroup_half8x8
                simdgroup_load(a0, ta, qm * 64u + k8, 64u)
                simdgroup_load(a1, ta, (qm + 8u) * 64u + k8, 64u)
                simdgroup_load(b0, tb, k8 * 32u + qn, 32u)
                simdgroup_load(b1, tb, k8 * 32u + (qn + 8u), 32u)
                simdgroup_multiply_accumulate(c00, a0, b0, c00)
                simdgroup_multiply_accumulate(c01, a0, b1, c01)
                simdgroup_multiply_accumulate(c10, a1, b0, c10)
                simdgroup_multiply_accumulate(c11, a1, b1, c11)
                k8 += 8u
            }
            barrier()
            kb++
        }
        let row0 = mBase + qm
        let col0 = nBase + qn
        simdgroup_store(c00, y, row0 * ndim + col0, ndim)
        simdgroup_store(c01, y, row0 * ndim + (col0 + 8u), ndim)
        simdgroup_store(c10, y, (row0 + 8u) * ndim + col0, ndim)
        simdgroup_store(c11, y, (row0 + 8u) * ndim + (col0 + 8u), ndim)
    }
}

// v5: 32x32 with TILE-MAJOR shared layout (the ggml mul_mm trick): each 8x8 tile is a
// contiguous 64-half block (stride 8), so every simdgroup_load hits one 128-byte run —
// no strided bank conflicts. Same 4KB threadgroup footprint as production.
class MetalGemmLabTile32 {
    @ssbo @binding = 0 wq : array<int8>
    @ssbo @binding = 1 ws : array<float>
    @ssbo @binding = 2 xq : array<int8>
    @ssbo @binding = 3 xs : array<float>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 kdim : uint
    @uniform @binding = 6 ndim : uint
    @workgroup ta : float16[1024]           // 4x4 grid of 8x8 A tiles, tile-major
    @workgroup tb : float16[1024]           // 4x4 grid of 8x8 B tiles ([k][out]), tile-major

    [metal_kernel(name="lab_tile32_msl")]
    def lab_tile32 {
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let ntileN = ndim / 32u
        let mBase = (gl_WorkGroupID.x / ntileN) * 32u
        let nBase = (gl_WorkGroupID.x % ntileN) * 32u
        let qm = (sg / 2u) * 16u
        let qn = (sg % 2u) * 16u
        var c00 : simdgroup_float8x8
        var c01 : simdgroup_float8x8
        var c10 : simdgroup_float8x8
        var c11 : simdgroup_float8x8
        var kb = 0u
        while (kb * 32u < kdim) {
            var e = lid
            while (e < 1024u) {
                let r = e / 32u
                let c = e % 32u
                let k = kb * 32u + c
                let ha = ((r / 8u) * 4u + c / 8u) * 64u + (r % 8u) * 8u + (c % 8u)
                ta[ha] = float16(float(xq[(mBase + r) * kdim + k]) * xs[(mBase + r) * (kdim / 32u) + kb])
                let hb = ((c / 8u) * 4u + r / 8u) * 64u + (c % 8u) * 8u + (r % 8u)
                tb[hb] = float16(float(wq[(nBase + r) * kdim + k]) * ws[(nBase + r) * (kdim / 32u) + kb])
                e += 128u
            }
            barrier()
            var k8 = 0u
            while (k8 < 32u) {
                var a0 : simdgroup_half8x8
                var a1 : simdgroup_half8x8
                var b0 : simdgroup_half8x8
                var b1 : simdgroup_half8x8
                simdgroup_load(a0, ta, ((qm / 8u) * 4u + k8 / 8u) * 64u, 8u)
                simdgroup_load(a1, ta, ((qm / 8u + 1u) * 4u + k8 / 8u) * 64u, 8u)
                simdgroup_load(b0, tb, ((k8 / 8u) * 4u + qn / 8u) * 64u, 8u)
                simdgroup_load(b1, tb, ((k8 / 8u) * 4u + qn / 8u + 1u) * 64u, 8u)
                simdgroup_multiply_accumulate(c00, a0, b0, c00)
                simdgroup_multiply_accumulate(c01, a0, b1, c01)
                simdgroup_multiply_accumulate(c10, a1, b0, c10)
                simdgroup_multiply_accumulate(c11, a1, b1, c11)
                k8 += 8u
            }
            barrier()
            kb++
        }
        let row0 = mBase + qm
        let col0 = nBase + qn
        simdgroup_store(c00, y, row0 * ndim + col0, ndim)
        simdgroup_store(c01, y, row0 * ndim + (col0 + 8u), ndim)
        simdgroup_store(c10, y, (row0 + 8u) * ndim + col0, ndim)
        simdgroup_store(c11, y, (row0 + 8u) * ndim + (col0 + 8u), ndim)
    }
}

// v6: 32x32 with row stride padded 32 -> 36 halfs — the cheap bank-conflict probe (72-byte
// rows break the power-of-2 stride) at +15% threadgroup memory.
class MetalGemmLabPad36 {
    @ssbo @binding = 0 wq : array<int8>
    @ssbo @binding = 1 ws : array<float>
    @ssbo @binding = 2 xq : array<int8>
    @ssbo @binding = 3 xs : array<float>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 kdim : uint
    @uniform @binding = 6 ndim : uint
    @workgroup ta : float16[1152]           // 32 rows x stride 36
    @workgroup tb : float16[1152]

    [metal_kernel(name="lab_pad36_msl")]
    def lab_pad36 {
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let ntileN = ndim / 32u
        let mBase = (gl_WorkGroupID.x / ntileN) * 32u
        let nBase = (gl_WorkGroupID.x % ntileN) * 32u
        let qm = (sg / 2u) * 16u
        let qn = (sg % 2u) * 16u
        var c00 : simdgroup_float8x8
        var c01 : simdgroup_float8x8
        var c10 : simdgroup_float8x8
        var c11 : simdgroup_float8x8
        var kb = 0u
        while (kb * 32u < kdim) {
            var e = lid
            while (e < 1024u) {
                let r = e / 32u
                let c = e % 32u
                let k = kb * 32u + c
                ta[r * 36u + c] = float16(float(xq[(mBase + r) * kdim + k]) * xs[(mBase + r) * (kdim / 32u) + kb])
                tb[c * 36u + r] = float16(float(wq[(nBase + r) * kdim + k]) * ws[(nBase + r) * (kdim / 32u) + kb])
                e += 128u
            }
            barrier()
            var k8 = 0u
            while (k8 < 32u) {
                var a0 : simdgroup_half8x8
                var a1 : simdgroup_half8x8
                var b0 : simdgroup_half8x8
                var b1 : simdgroup_half8x8
                simdgroup_load(a0, ta, qm * 36u + k8, 36u)
                simdgroup_load(a1, ta, (qm + 8u) * 36u + k8, 36u)
                simdgroup_load(b0, tb, k8 * 36u + qn, 36u)
                simdgroup_load(b1, tb, k8 * 36u + (qn + 8u), 36u)
                simdgroup_multiply_accumulate(c00, a0, b0, c00)
                simdgroup_multiply_accumulate(c01, a0, b1, c01)
                simdgroup_multiply_accumulate(c10, a1, b0, c10)
                simdgroup_multiply_accumulate(c11, a1, b1, c11)
                k8 += 8u
            }
            barrier()
            kb++
        }
        let row0 = mBase + qm
        let col0 = nBase + qn
        simdgroup_store(c00, y, row0 * ndim + col0, ndim)
        simdgroup_store(c01, y, row0 * ndim + (col0 + 8u), ndim)
        simdgroup_store(c10, y, (row0 + 8u) * ndim + col0, ndim)
        simdgroup_store(c11, y, (row0 + 8u) * ndim + (col0 + 8u), ndim)
    }
}

// knockout: production kernel with the STAGING loop removed (math + barriers + stores only,
// tiles read uninitialized) — timing-only, attributes the staging share of the budget
class MetalGemmLabNoStage {
    @ssbo @binding = 0 wq : array<int8>
    @ssbo @binding = 1 ws : array<float>
    @ssbo @binding = 2 xq : array<int8>
    @ssbo @binding = 3 xs : array<float>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 kdim : uint
    @uniform @binding = 6 ndim : uint
    @workgroup ta : float16[1024]
    @workgroup tb : float16[1024]

    [metal_kernel(name="lab_nostage_msl")]
    def lab_nostage {
        let sg = gl_SubgroupID
        let ntileN = ndim / 32u
        let mBase = (gl_WorkGroupID.x / ntileN) * 32u
        let nBase = (gl_WorkGroupID.x % ntileN) * 32u
        let qm = (sg / 2u) * 16u
        let qn = (sg % 2u) * 16u
        var c00 : simdgroup_float8x8
        var c01 : simdgroup_float8x8
        var c10 : simdgroup_float8x8
        var c11 : simdgroup_float8x8
        var kb = 0u
        while (kb * 32u < kdim) {
            barrier()
            var k8 = 0u
            while (k8 < 32u) {
                var a0 : simdgroup_half8x8
                var a1 : simdgroup_half8x8
                var b0 : simdgroup_half8x8
                var b1 : simdgroup_half8x8
                simdgroup_load(a0, ta, qm * 32u + k8, 32u)
                simdgroup_load(a1, ta, (qm + 8u) * 32u + k8, 32u)
                simdgroup_load(b0, tb, k8 * 32u + qn, 32u)
                simdgroup_load(b1, tb, k8 * 32u + (qn + 8u), 32u)
                simdgroup_multiply_accumulate(c00, a0, b0, c00)
                simdgroup_multiply_accumulate(c01, a0, b1, c01)
                simdgroup_multiply_accumulate(c10, a1, b0, c10)
                simdgroup_multiply_accumulate(c11, a1, b1, c11)
                k8 += 8u
            }
            barrier()
            kb++
        }
        let row0 = mBase + qm
        let col0 = nBase + qn
        simdgroup_store(c00, y, row0 * ndim + col0, ndim)
        simdgroup_store(c01, y, row0 * ndim + (col0 + 8u), ndim)
        simdgroup_store(c10, y, (row0 + 8u) * ndim + col0, ndim)
        simdgroup_store(c11, y, (row0 + 8u) * ndim + (col0 + 8u), ndim)
    }
}

// knockout: production kernel with the MATH loop removed (staging + barriers + zero stores
// only) — timing-only, attributes the staging cost directly
class MetalGemmLabNoMath {
    @ssbo @binding = 0 wq : array<int8>
    @ssbo @binding = 1 ws : array<float>
    @ssbo @binding = 2 xq : array<int8>
    @ssbo @binding = 3 xs : array<float>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 kdim : uint
    @uniform @binding = 6 ndim : uint
    @workgroup ta : float16[1024]
    @workgroup tb : float16[1024]

    [metal_kernel(name="lab_nomath_msl")]
    def lab_nomath {
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let ntileN = ndim / 32u
        let mBase = (gl_WorkGroupID.x / ntileN) * 32u
        let nBase = (gl_WorkGroupID.x % ntileN) * 32u
        let qm = (sg / 2u) * 16u
        let qn = (sg % 2u) * 16u
        var c00 : simdgroup_float8x8    // nolint:LINT003 — sgmat locals emit as make_filled fills, keep the production spelling
        var c01 : simdgroup_float8x8    // nolint:LINT003
        var c10 : simdgroup_float8x8    // nolint:LINT003
        var c11 : simdgroup_float8x8    // nolint:LINT003
        var kb = 0u
        while (kb * 32u < kdim) {
            var e = lid
            while (e < 1024u) {
                let r = e / 32u
                let c = e % 32u
                let k = kb * 32u + c
                ta[e] = float16(float(xq[(mBase + r) * kdim + k]) * xs[(mBase + r) * (kdim / 32u) + kb])
                tb[c * 32u + r] = float16(float(wq[(nBase + r) * kdim + k]) * ws[(nBase + r) * (kdim / 32u) + kb])
                e += 128u
            }
            barrier()
            barrier()
            kb++
        }
        let row0 = mBase + qm
        let col0 = nBase + qn
        simdgroup_store(c00, y, row0 * ndim + col0, ndim)
        simdgroup_store(c01, y, row0 * ndim + (col0 + 8u), ndim)
        simdgroup_store(c10, y, (row0 + 8u) * ndim + col0, ndim)
        simdgroup_store(c11, y, (row0 + 8u) * ndim + (col0 + 8u), ndim)
    }
}

// v7: 32x32 with RUN-STAGING (the ggml work split): 128 threads = 64 A runs + 64 B runs of 16
// CONTIGUOUS elements each — ONE scale load per run instead of one per element, contiguous
// device byte loads the compiler can coalesce. The half split lands on simdgroup boundaries
// (threads 0-63 = simdgroups 0-1) so there is no intra-simdgroup divergence. Math loop = v0.
class MetalGemmLabRun16 {
    @ssbo @binding = 0 wq : array<int8>
    @ssbo @binding = 1 ws : array<float>
    @ssbo @binding = 2 xq : array<int8>
    @ssbo @binding = 3 xs : array<float>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 kdim : uint
    @uniform @binding = 6 ndim : uint
    @workgroup ta : float16[1024]
    @workgroup tb : float16[1024]

    [metal_kernel(name="lab_run16_msl")]
    def lab_run16 {
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let ntileN = ndim / 32u
        let mBase = (gl_WorkGroupID.x / ntileN) * 32u
        let nBase = (gl_WorkGroupID.x % ntileN) * 32u
        let qm = (sg / 2u) * 16u
        let qn = (sg % 2u) * 16u
        var c00 : simdgroup_float8x8
        var c01 : simdgroup_float8x8
        var c10 : simdgroup_float8x8
        var c11 : simdgroup_float8x8
        let run = lid % 64u
        let rr = run / 2u                   // row within the 32-row tile
        let c0 = (run % 2u) * 16u           // k offset within the 32-k block
        var kb = 0u
        while (kb * 32u < kdim) {
            let k0 = kb * 32u + c0
            if (lid < 64u) {                // A half: 64 runs of 16, one scale each
                let s = xs[(mBase + rr) * (kdim / 32u) + kb]
                let src = (mBase + rr) * kdim + k0
                let dst = rr * 32u + c0
                var i = 0u
                while (i < 16u) {
                    ta[dst + i] = float16(float(xq[src + i]) * s)
                    i++
                }
            } else {                        // B half: transposed stores
                let s = ws[(nBase + rr) * (kdim / 32u) + kb]
                let src = (nBase + rr) * kdim + k0
                var i = 0u
                while (i < 16u) {
                    tb[(c0 + i) * 32u + rr] = float16(float(wq[src + i]) * s)
                    i++
                }
            }
            barrier()
            var k8 = 0u
            while (k8 < 32u) {
                var a0 : simdgroup_half8x8
                var a1 : simdgroup_half8x8
                var b0 : simdgroup_half8x8
                var b1 : simdgroup_half8x8
                simdgroup_load(a0, ta, qm * 32u + k8, 32u)
                simdgroup_load(a1, ta, (qm + 8u) * 32u + k8, 32u)
                simdgroup_load(b0, tb, k8 * 32u + qn, 32u)
                simdgroup_load(b1, tb, k8 * 32u + (qn + 8u), 32u)
                simdgroup_multiply_accumulate(c00, a0, b0, c00)
                simdgroup_multiply_accumulate(c01, a0, b1, c01)
                simdgroup_multiply_accumulate(c10, a1, b0, c10)
                simdgroup_multiply_accumulate(c11, a1, b1, c11)
                k8 += 8u
            }
            barrier()
            kb++
        }
        let row0 = mBase + qm
        let col0 = nBase + qn
        simdgroup_store(c00, y, row0 * ndim + col0, ndim)
        simdgroup_store(c01, y, row0 * ndim + (col0 + 8u), ndim)
        simdgroup_store(c10, y, (row0 + 8u) * ndim + col0, ndim)
        simdgroup_store(c11, y, (row0 + 8u) * ndim + (col0 + 8u), ndim)
    }
}

// v8: v7 + VECTORIZED device loads — the quant buffers are viewed as byte4, so each 16-run is
// 4 byte4 loads -> int4 -> float4 * scale -> 4 half stores (device loads per thread per block
// drop from 32 scalar in v0 to 5).
class MetalGemmLabVec4 {
    @ssbo @binding = 0 wq : array<byte4>    // same bytes as the int8 view — 4-wide loads
    @ssbo @binding = 1 ws : array<float>
    @ssbo @binding = 2 xq : array<byte4>
    @ssbo @binding = 3 xs : array<float>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 kdim : uint
    @uniform @binding = 6 ndim : uint
    @workgroup ta : float16[1024]
    @workgroup tb : float16[1024]

    [metal_kernel(name="lab_vec4_msl")]
    def lab_vec4 {
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let ntileN = ndim / 32u
        let mBase = (gl_WorkGroupID.x / ntileN) * 32u
        let nBase = (gl_WorkGroupID.x % ntileN) * 32u
        let qm = (sg / 2u) * 16u
        let qn = (sg % 2u) * 16u
        var c00 : simdgroup_float8x8
        var c01 : simdgroup_float8x8
        var c10 : simdgroup_float8x8
        var c11 : simdgroup_float8x8
        let kdim4 = kdim / 4u
        let run = lid % 64u
        let rr = run / 2u
        let c0 = (run % 2u) * 16u
        var kb = 0u
        while (kb * 32u < kdim) {
            let k40 = (kb * 32u + c0) / 4u
            if (lid < 64u) {
                let s = xs[(mBase + rr) * (kdim / 32u) + kb]
                let src = (mBase + rr) * kdim4 + k40
                let dst = rr * 32u + c0
                var i4 = 0u
                while (i4 < 4u) {
                    let v = float4(int4(xq[src + i4])) * s
                    let o = dst + i4 * 4u
                    ta[o] = float16(v.x)
                    ta[o + 1u] = float16(v.y)
                    ta[o + 2u] = float16(v.z)
                    ta[o + 3u] = float16(v.w)
                    i4++
                }
            } else {
                let s = ws[(nBase + rr) * (kdim / 32u) + kb]
                let src = (nBase + rr) * kdim4 + k40
                var i4 = 0u
                while (i4 < 4u) {
                    let v = float4(int4(wq[src + i4])) * s
                    let o = c0 + i4 * 4u
                    tb[o * 32u + rr] = float16(v.x)
                    tb[(o + 1u) * 32u + rr] = float16(v.y)
                    tb[(o + 2u) * 32u + rr] = float16(v.z)
                    tb[(o + 3u) * 32u + rr] = float16(v.w)
                    i4++
                }
            }
            barrier()
            var k8 = 0u
            while (k8 < 32u) {
                var a0 : simdgroup_half8x8
                var a1 : simdgroup_half8x8
                var b0 : simdgroup_half8x8
                var b1 : simdgroup_half8x8
                simdgroup_load(a0, ta, qm * 32u + k8, 32u)
                simdgroup_load(a1, ta, (qm + 8u) * 32u + k8, 32u)
                simdgroup_load(b0, tb, k8 * 32u + qn, 32u)
                simdgroup_load(b1, tb, k8 * 32u + (qn + 8u), 32u)
                simdgroup_multiply_accumulate(c00, a0, b0, c00)
                simdgroup_multiply_accumulate(c01, a0, b1, c01)
                simdgroup_multiply_accumulate(c10, a1, b0, c10)
                simdgroup_multiply_accumulate(c11, a1, b1, c11)
                k8 += 8u
            }
            barrier()
            kb++
        }
        let row0 = mBase + qm
        let col0 = nBase + qn
        simdgroup_store(c00, y, row0 * ndim + col0, ndim)
        simdgroup_store(c01, y, row0 * ndim + (col0 + 8u), ndim)
        simdgroup_store(c10, y, (row0 + 8u) * ndim + col0, ndim)
        simdgroup_store(c11, y, (row0 + 8u) * ndim + (col0 + 8u), ndim)
    }
}

// v9: v7 + int4 device view — ONE 16-byte load covers the whole 16-run (2 device loads per
// thread per k-block including the scale); quants unpack by sign-extending shifts, landing
// interleaved ({0,4,8,12}, {1,5,9,13}, ...) which only reorders the threadgroup stores.
class MetalGemmLabVec16 {
    @ssbo @binding = 0 wq : array<int4>     // same bytes as the int8 view — 16-wide loads
    @ssbo @binding = 1 ws : array<float>
    @ssbo @binding = 2 xq : array<int4>
    @ssbo @binding = 3 xs : array<float>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 kdim : uint
    @uniform @binding = 6 ndim : uint
    @workgroup ta : float16[1024]
    @workgroup tb : float16[1024]

    [metal_kernel(name="lab_vec16_msl")]
    def lab_vec16 {
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let ntileN = ndim / 32u
        let mBase = (gl_WorkGroupID.x / ntileN) * 32u
        let nBase = (gl_WorkGroupID.x % ntileN) * 32u
        let qm = (sg / 2u) * 16u
        let qn = (sg % 2u) * 16u
        var c00 : simdgroup_float8x8
        var c01 : simdgroup_float8x8
        var c10 : simdgroup_float8x8
        var c11 : simdgroup_float8x8
        let kdim16 = kdim / 16u
        let run = lid % 64u
        let rr = run / 2u
        let c0 = (run % 2u) * 16u
        var kb = 0u
        while (kb * 32u < kdim) {
            let k16 = (kb * 32u + c0) / 16u
            if (lid < 64u) {
                let s = xs[(mBase + rr) * (kdim / 32u) + kb]
                let p = xq[(mBase + rr) * kdim16 + k16]
                let v0 = float4((p << 24) >> 24) * s
                let v1 = float4((p << 16) >> 24) * s
                let v2 = float4((p << 8) >> 24) * s
                let v3 = float4(p >> 24) * s
                let o = rr * 32u + c0
                ta[o] = float16(v0.x)
                ta[o + 4u] = float16(v0.y)
                ta[o + 8u] = float16(v0.z)
                ta[o + 12u] = float16(v0.w)
                ta[o + 1u] = float16(v1.x)
                ta[o + 5u] = float16(v1.y)
                ta[o + 9u] = float16(v1.z)
                ta[o + 13u] = float16(v1.w)
                ta[o + 2u] = float16(v2.x)
                ta[o + 6u] = float16(v2.y)
                ta[o + 10u] = float16(v2.z)
                ta[o + 14u] = float16(v2.w)
                ta[o + 3u] = float16(v3.x)
                ta[o + 7u] = float16(v3.y)
                ta[o + 11u] = float16(v3.z)
                ta[o + 15u] = float16(v3.w)
            } else {
                let s = ws[(nBase + rr) * (kdim / 32u) + kb]
                let p = wq[(nBase + rr) * kdim16 + k16]
                let v0 = float4((p << 24) >> 24) * s
                let v1 = float4((p << 16) >> 24) * s
                let v2 = float4((p << 8) >> 24) * s
                let v3 = float4(p >> 24) * s
                let o = c0 * 32u + rr
                tb[o] = float16(v0.x)
                tb[o + 128u] = float16(v0.y)
                tb[o + 256u] = float16(v0.z)
                tb[o + 384u] = float16(v0.w)
                tb[o + 32u] = float16(v1.x)
                tb[o + 160u] = float16(v1.y)
                tb[o + 288u] = float16(v1.z)
                tb[o + 416u] = float16(v1.w)
                tb[o + 64u] = float16(v2.x)
                tb[o + 192u] = float16(v2.y)
                tb[o + 320u] = float16(v2.z)
                tb[o + 448u] = float16(v2.w)
                tb[o + 96u] = float16(v3.x)
                tb[o + 224u] = float16(v3.y)
                tb[o + 352u] = float16(v3.z)
                tb[o + 480u] = float16(v3.w)
            }
            barrier()
            var k8 = 0u
            while (k8 < 32u) {
                var a0 : simdgroup_half8x8
                var a1 : simdgroup_half8x8
                var b0 : simdgroup_half8x8
                var b1 : simdgroup_half8x8
                simdgroup_load(a0, ta, qm * 32u + k8, 32u)
                simdgroup_load(a1, ta, (qm + 8u) * 32u + k8, 32u)
                simdgroup_load(b0, tb, k8 * 32u + qn, 32u)
                simdgroup_load(b1, tb, k8 * 32u + (qn + 8u), 32u)
                simdgroup_multiply_accumulate(c00, a0, b0, c00)
                simdgroup_multiply_accumulate(c01, a0, b1, c01)
                simdgroup_multiply_accumulate(c10, a1, b0, c10)
                simdgroup_multiply_accumulate(c11, a1, b1, c11)
                k8 += 8u
            }
            barrier()
            kb++
        }
        let row0 = mBase + qm
        let col0 = nBase + qn
        simdgroup_store(c00, y, row0 * ndim + col0, ndim)
        simdgroup_store(c01, y, row0 * ndim + (col0 + 8u), ndim)
        simdgroup_store(c10, y, (row0 + 8u) * ndim + col0, ndim)
        simdgroup_store(c11, y, (row0 + 8u) * ndim + (col0 + 8u), ndim)
    }
}

// v10: v8's byte4 run-staging + v3's double buffering (8KB tiles, one barrier per iteration) —
// re-tests the overlap tradeoff now that staging is cheap.
class MetalGemmLabDbVec4 {
    @ssbo @binding = 0 wq : array<byte4>
    @ssbo @binding = 1 ws : array<float>
    @ssbo @binding = 2 xq : array<byte4>
    @ssbo @binding = 3 xs : array<float>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 kdim : uint
    @uniform @binding = 6 ndim : uint
    @workgroup ta : float16[2048]
    @workgroup tb : float16[2048]

    [metal_kernel(name="lab_dbvec4_msl")]
    def lab_dbvec4 {
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let ntileN = ndim / 32u
        let mBase = (gl_WorkGroupID.x / ntileN) * 32u
        let nBase = (gl_WorkGroupID.x % ntileN) * 32u
        let qm = (sg / 2u) * 16u
        let qn = (sg % 2u) * 16u
        var c00 : simdgroup_float8x8
        var c01 : simdgroup_float8x8
        var c10 : simdgroup_float8x8
        var c11 : simdgroup_float8x8
        let kdim4 = kdim / 4u
        let nkb = kdim / 32u
        let run = lid % 64u
        let rr = run / 2u
        let c0 = (run % 2u) * 16u
        // prologue: stage block 0 into half 0
        if (lid < 64u) {
            let s = xs[(mBase + rr) * (kdim / 32u)]
            let src = (mBase + rr) * kdim4 + c0 / 4u
            let dst = rr * 32u + c0
            var i4 = 0u
            while (i4 < 4u) {
                let v = float4(int4(xq[src + i4])) * s
                let o = dst + i4 * 4u
                ta[o] = float16(v.x)
                ta[o + 1u] = float16(v.y)
                ta[o + 2u] = float16(v.z)
                ta[o + 3u] = float16(v.w)
                i4++
            }
        } else {
            let s = ws[(nBase + rr) * (kdim / 32u)]
            let src = (nBase + rr) * kdim4 + c0 / 4u
            var i4 = 0u
            while (i4 < 4u) {
                let v = float4(int4(wq[src + i4])) * s
                let o = c0 + i4 * 4u
                tb[o * 32u + rr] = float16(v.x)
                tb[(o + 1u) * 32u + rr] = float16(v.y)
                tb[(o + 2u) * 32u + rr] = float16(v.z)
                tb[(o + 3u) * 32u + rr] = float16(v.w)
                i4++
            }
        }
        barrier()
        var kb = 0u
        while (kb < nkb) {
            let cur = (kb % 2u) * 1024u
            if (kb + 1u < nkb) {
                let nxt = ((kb + 1u) % 2u) * 1024u
                let kn = kb + 1u
                let k40 = (kn * 32u + c0) / 4u
                if (lid < 64u) {
                    let s = xs[(mBase + rr) * (kdim / 32u) + kn]
                    let src = (mBase + rr) * kdim4 + k40
                    let dst = nxt + rr * 32u + c0
                    var i4 = 0u
                    while (i4 < 4u) {
                        let v = float4(int4(xq[src + i4])) * s
                        let o = dst + i4 * 4u
                        ta[o] = float16(v.x)
                        ta[o + 1u] = float16(v.y)
                        ta[o + 2u] = float16(v.z)
                        ta[o + 3u] = float16(v.w)
                        i4++
                    }
                } else {
                    let s = ws[(nBase + rr) * (kdim / 32u) + kn]
                    let src = (nBase + rr) * kdim4 + k40
                    var i4 = 0u
                    while (i4 < 4u) {
                        let v = float4(int4(wq[src + i4])) * s
                        let o = c0 + i4 * 4u
                        tb[nxt + o * 32u + rr] = float16(v.x)
                        tb[nxt + (o + 1u) * 32u + rr] = float16(v.y)
                        tb[nxt + (o + 2u) * 32u + rr] = float16(v.z)
                        tb[nxt + (o + 3u) * 32u + rr] = float16(v.w)
                        i4++
                    }
                }
            }
            var k8 = 0u
            while (k8 < 32u) {
                var a0 : simdgroup_half8x8
                var a1 : simdgroup_half8x8
                var b0 : simdgroup_half8x8
                var b1 : simdgroup_half8x8
                simdgroup_load(a0, ta, cur + qm * 32u + k8, 32u)
                simdgroup_load(a1, ta, cur + (qm + 8u) * 32u + k8, 32u)
                simdgroup_load(b0, tb, cur + k8 * 32u + qn, 32u)
                simdgroup_load(b1, tb, cur + k8 * 32u + (qn + 8u), 32u)
                simdgroup_multiply_accumulate(c00, a0, b0, c00)
                simdgroup_multiply_accumulate(c01, a0, b1, c01)
                simdgroup_multiply_accumulate(c10, a1, b0, c10)
                simdgroup_multiply_accumulate(c11, a1, b1, c11)
                k8 += 8u
            }
            barrier()
            kb++
        }
        let row0 = mBase + qm
        let col0 = nBase + qn
        simdgroup_store(c00, y, row0 * ndim + col0, ndim)
        simdgroup_store(c01, y, row0 * ndim + (col0 + 8u), ndim)
        simdgroup_store(c10, y, (row0 + 8u) * ndim + col0, ndim)
        simdgroup_store(c11, y, (row0 + 8u) * ndim + (col0 + 8u), ndim)
    }
}

// v11: 32x64 C tile + vec4 run-staging — the tile-size rematch with cheap staging: doubling
// the output columns halves the activation re-reads (device traffic -21% on w13-like shapes).
// B has exactly 128 16-runs (one per thread); A's 64 runs ride on threads 64-127.
class MetalGemmLabN64Vec4 {
    @ssbo @binding = 0 wq : array<byte4>
    @ssbo @binding = 1 ws : array<float>
    @ssbo @binding = 2 xq : array<byte4>
    @ssbo @binding = 3 xs : array<float>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 kdim : uint
    @uniform @binding = 6 ndim : uint
    @workgroup ta : float16[1024]           // 32 tokens x 32 k
    @workgroup tb : float16[2048]           // 32 k x 64 outputs, transposed

    [metal_kernel(name="lab_n64vec4_msl")]
    def lab_n64vec4 {
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let ntileN = ndim / 64u
        let mBase = (gl_WorkGroupID.x / ntileN) * 32u
        let nBase = (gl_WorkGroupID.x % ntileN) * 64u
        let qm = (sg / 2u) * 16u
        let qn = (sg % 2u) * 32u
        var c00 : simdgroup_float8x8
        var c01 : simdgroup_float8x8
        var c02 : simdgroup_float8x8
        var c03 : simdgroup_float8x8
        var c10 : simdgroup_float8x8
        var c11 : simdgroup_float8x8
        var c12 : simdgroup_float8x8
        var c13 : simdgroup_float8x8
        let kdim4 = kdim / 4u
        let rb = lid / 2u                   // B: one run per thread (64 out-rows x 2 half-blocks)
        let c0b = (lid % 2u) * 16u
        let ra = (lid - 64u) / 2u           // A: threads 64-127
        let c0a = ((lid - 64u) % 2u) * 16u
        var kb = 0u
        while (kb * 32u < kdim) {
            {
                let s = ws[(nBase + rb) * (kdim / 32u) + kb]
                let src = (nBase + rb) * kdim4 + (kb * 32u + c0b) / 4u
                var i4 = 0u
                while (i4 < 4u) {
                    let v = float4(int4(wq[src + i4])) * s
                    let o = c0b + i4 * 4u
                    tb[o * 64u + rb] = float16(v.x)
                    tb[(o + 1u) * 64u + rb] = float16(v.y)
                    tb[(o + 2u) * 64u + rb] = float16(v.z)
                    tb[(o + 3u) * 64u + rb] = float16(v.w)
                    i4++
                }
            }
            if (lid >= 64u) {
                let s = xs[(mBase + ra) * (kdim / 32u) + kb]
                let src = (mBase + ra) * kdim4 + (kb * 32u + c0a) / 4u
                let dst = ra * 32u + c0a
                var i4 = 0u
                while (i4 < 4u) {
                    let v = float4(int4(xq[src + i4])) * s
                    let o = dst + i4 * 4u
                    ta[o] = float16(v.x)
                    ta[o + 1u] = float16(v.y)
                    ta[o + 2u] = float16(v.z)
                    ta[o + 3u] = float16(v.w)
                    i4++
                }
            }
            barrier()
            var k8 = 0u
            while (k8 < 32u) {
                var a0 : simdgroup_half8x8
                var a1 : simdgroup_half8x8
                var b0 : simdgroup_half8x8
                var b1 : simdgroup_half8x8
                var b2 : simdgroup_half8x8
                var b3 : simdgroup_half8x8
                simdgroup_load(a0, ta, qm * 32u + k8, 32u)
                simdgroup_load(a1, ta, (qm + 8u) * 32u + k8, 32u)
                simdgroup_load(b0, tb, k8 * 64u + qn, 64u)
                simdgroup_load(b1, tb, k8 * 64u + (qn + 8u), 64u)
                simdgroup_load(b2, tb, k8 * 64u + (qn + 16u), 64u)
                simdgroup_load(b3, tb, k8 * 64u + (qn + 24u), 64u)
                simdgroup_multiply_accumulate(c00, a0, b0, c00)
                simdgroup_multiply_accumulate(c01, a0, b1, c01)
                simdgroup_multiply_accumulate(c02, a0, b2, c02)
                simdgroup_multiply_accumulate(c03, a0, b3, c03)
                simdgroup_multiply_accumulate(c10, a1, b0, c10)
                simdgroup_multiply_accumulate(c11, a1, b1, c11)
                simdgroup_multiply_accumulate(c12, a1, b2, c12)
                simdgroup_multiply_accumulate(c13, a1, b3, c13)
                k8 += 8u
            }
            barrier()
            kb++
        }
        let row0 = mBase + qm
        let col0 = nBase + qn
        simdgroup_store(c00, y, row0 * ndim + col0, ndim)
        simdgroup_store(c01, y, row0 * ndim + (col0 + 8u), ndim)
        simdgroup_store(c02, y, row0 * ndim + (col0 + 16u), ndim)
        simdgroup_store(c03, y, row0 * ndim + (col0 + 24u), ndim)
        simdgroup_store(c10, y, (row0 + 8u) * ndim + col0, ndim)
        simdgroup_store(c11, y, (row0 + 8u) * ndim + (col0 + 8u), ndim)
        simdgroup_store(c12, y, (row0 + 8u) * ndim + (col0 + 16u), ndim)
        simdgroup_store(c13, y, (row0 + 8u) * ndim + (col0 + 24u), ndim)
    }
}

// v12: 64x32 C tile + vec4 run-staging — the ggml block shape rematch. A has exactly 128
// 16-runs (one per thread); B's 64 runs ride on threads 64-127.
class MetalGemmLabM64Vec4 {
    @ssbo @binding = 0 wq : array<byte4>
    @ssbo @binding = 1 ws : array<float>
    @ssbo @binding = 2 xq : array<byte4>
    @ssbo @binding = 3 xs : array<float>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 kdim : uint
    @uniform @binding = 6 ndim : uint
    @workgroup ta : float16[2048]           // 64 tokens x 32 k
    @workgroup tb : float16[1024]           // 32 k x 32 outputs, transposed

    [metal_kernel(name="lab_m64vec4_msl")]
    def lab_m64vec4 {
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let ntileN = ndim / 32u
        let mBase = (gl_WorkGroupID.x / ntileN) * 64u
        let nBase = (gl_WorkGroupID.x % ntileN) * 32u
        let qm = (sg / 2u) * 32u
        let qn = (sg % 2u) * 16u
        var c00 : simdgroup_float8x8
        var c01 : simdgroup_float8x8
        var c10 : simdgroup_float8x8
        var c11 : simdgroup_float8x8
        var c20 : simdgroup_float8x8
        var c21 : simdgroup_float8x8
        var c30 : simdgroup_float8x8
        var c31 : simdgroup_float8x8
        let kdim4 = kdim / 4u
        let ra = lid / 2u                   // A: one run per thread (64 rows x 2 half-blocks)
        let c0a = (lid % 2u) * 16u
        let rb = (lid - 64u) / 2u           // B: threads 64-127
        let c0b = ((lid - 64u) % 2u) * 16u
        var kb = 0u
        while (kb * 32u < kdim) {
            {
                let s = xs[(mBase + ra) * (kdim / 32u) + kb]
                let src = (mBase + ra) * kdim4 + (kb * 32u + c0a) / 4u
                let dst = ra * 32u + c0a
                var i4 = 0u
                while (i4 < 4u) {
                    let v = float4(int4(xq[src + i4])) * s
                    let o = dst + i4 * 4u
                    ta[o] = float16(v.x)
                    ta[o + 1u] = float16(v.y)
                    ta[o + 2u] = float16(v.z)
                    ta[o + 3u] = float16(v.w)
                    i4++
                }
            }
            if (lid >= 64u) {
                let s = ws[(nBase + rb) * (kdim / 32u) + kb]
                let src = (nBase + rb) * kdim4 + (kb * 32u + c0b) / 4u
                var i4 = 0u
                while (i4 < 4u) {
                    let v = float4(int4(wq[src + i4])) * s
                    let o = c0b + i4 * 4u
                    tb[o * 32u + rb] = float16(v.x)
                    tb[(o + 1u) * 32u + rb] = float16(v.y)
                    tb[(o + 2u) * 32u + rb] = float16(v.z)
                    tb[(o + 3u) * 32u + rb] = float16(v.w)
                    i4++
                }
            }
            barrier()
            var k8 = 0u
            while (k8 < 32u) {
                var a0 : simdgroup_half8x8
                var a1 : simdgroup_half8x8
                var a2 : simdgroup_half8x8
                var a3 : simdgroup_half8x8
                var b0 : simdgroup_half8x8
                var b1 : simdgroup_half8x8
                simdgroup_load(a0, ta, qm * 32u + k8, 32u)
                simdgroup_load(a1, ta, (qm + 8u) * 32u + k8, 32u)
                simdgroup_load(a2, ta, (qm + 16u) * 32u + k8, 32u)
                simdgroup_load(a3, ta, (qm + 24u) * 32u + k8, 32u)
                simdgroup_load(b0, tb, k8 * 32u + qn, 32u)
                simdgroup_load(b1, tb, k8 * 32u + (qn + 8u), 32u)
                simdgroup_multiply_accumulate(c00, a0, b0, c00)
                simdgroup_multiply_accumulate(c01, a0, b1, c01)
                simdgroup_multiply_accumulate(c10, a1, b0, c10)
                simdgroup_multiply_accumulate(c11, a1, b1, c11)
                simdgroup_multiply_accumulate(c20, a2, b0, c20)
                simdgroup_multiply_accumulate(c21, a2, b1, c21)
                simdgroup_multiply_accumulate(c30, a3, b0, c30)
                simdgroup_multiply_accumulate(c31, a3, b1, c31)
                k8 += 8u
            }
            barrier()
            kb++
        }
        let row0 = mBase + qm
        let col0 = nBase + qn
        simdgroup_store(c00, y, row0 * ndim + col0, ndim)
        simdgroup_store(c01, y, row0 * ndim + (col0 + 8u), ndim)
        simdgroup_store(c10, y, (row0 + 8u) * ndim + col0, ndim)
        simdgroup_store(c11, y, (row0 + 8u) * ndim + (col0 + 8u), ndim)
        simdgroup_store(c20, y, (row0 + 16u) * ndim + col0, ndim)
        simdgroup_store(c21, y, (row0 + 16u) * ndim + (col0 + 8u), ndim)
        simdgroup_store(c30, y, (row0 + 24u) * ndim + col0, ndim)
        simdgroup_store(c31, y, (row0 + 24u) * ndim + (col0 + 8u), ndim)
    }
}

// v13: 64x64 C tile + vec4 run-staging — halves BOTH re-read directions (device traffic -54%
// on w13-like shapes); 8KB tiles, 16 accumulators per simdgroup (32x32 quadrant), staging
// perfectly balanced (every thread stages one A run and one B run).
class MetalGemmLab6464Vec4 {
    @ssbo @binding = 0 wq : array<byte4>
    @ssbo @binding = 1 ws : array<float>
    @ssbo @binding = 2 xq : array<byte4>
    @ssbo @binding = 3 xs : array<float>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 kdim : uint
    @uniform @binding = 6 ndim : uint
    @workgroup ta : float16[2048]           // 64 tokens x 32 k
    @workgroup tb : float16[2048]           // 32 k x 64 outputs, transposed

    [metal_kernel(name="lab_6464vec4_msl")]
    def lab_6464vec4 {
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let ntileN = ndim / 64u
        let mBase = (gl_WorkGroupID.x / ntileN) * 64u
        let nBase = (gl_WorkGroupID.x % ntileN) * 64u
        let qm = (sg / 2u) * 32u
        let qn = (sg % 2u) * 32u
        var c00 : simdgroup_float8x8
        var c01 : simdgroup_float8x8
        var c02 : simdgroup_float8x8
        var c03 : simdgroup_float8x8
        var c10 : simdgroup_float8x8
        var c11 : simdgroup_float8x8
        var c12 : simdgroup_float8x8
        var c13 : simdgroup_float8x8
        var c20 : simdgroup_float8x8
        var c21 : simdgroup_float8x8
        var c22 : simdgroup_float8x8
        var c23 : simdgroup_float8x8
        var c30 : simdgroup_float8x8
        var c31 : simdgroup_float8x8
        var c32 : simdgroup_float8x8
        var c33 : simdgroup_float8x8
        let kdim4 = kdim / 4u
        let ra = lid / 2u
        let c0a = (lid % 2u) * 16u
        var kb = 0u
        while (kb * 32u < kdim) {
            {
                let s = xs[(mBase + ra) * (kdim / 32u) + kb]
                let src = (mBase + ra) * kdim4 + (kb * 32u + c0a) / 4u
                let dst = ra * 32u + c0a
                var i4 = 0u
                while (i4 < 4u) {
                    let v = float4(int4(xq[src + i4])) * s
                    let o = dst + i4 * 4u
                    ta[o] = float16(v.x)
                    ta[o + 1u] = float16(v.y)
                    ta[o + 2u] = float16(v.z)
                    ta[o + 3u] = float16(v.w)
                    i4++
                }
            }
            {
                let sw = ws[(nBase + ra) * (kdim / 32u) + kb]
                let srcw = (nBase + ra) * kdim4 + (kb * 32u + c0a) / 4u
                var j4 = 0u
                while (j4 < 4u) {
                    let v = float4(int4(wq[srcw + j4])) * sw
                    let o = c0a + j4 * 4u
                    tb[o * 64u + ra] = float16(v.x)
                    tb[(o + 1u) * 64u + ra] = float16(v.y)
                    tb[(o + 2u) * 64u + ra] = float16(v.z)
                    tb[(o + 3u) * 64u + ra] = float16(v.w)
                    j4++
                }
            }
            barrier()
            var k8 = 0u
            while (k8 < 32u) {
                var a0 : simdgroup_half8x8
                var a1 : simdgroup_half8x8
                var a2 : simdgroup_half8x8
                var a3 : simdgroup_half8x8
                var b0 : simdgroup_half8x8
                var b1 : simdgroup_half8x8
                var b2 : simdgroup_half8x8
                var b3 : simdgroup_half8x8
                simdgroup_load(a0, ta, qm * 32u + k8, 32u)
                simdgroup_load(a1, ta, (qm + 8u) * 32u + k8, 32u)
                simdgroup_load(a2, ta, (qm + 16u) * 32u + k8, 32u)
                simdgroup_load(a3, ta, (qm + 24u) * 32u + k8, 32u)
                simdgroup_load(b0, tb, k8 * 64u + qn, 64u)
                simdgroup_load(b1, tb, k8 * 64u + (qn + 8u), 64u)
                simdgroup_load(b2, tb, k8 * 64u + (qn + 16u), 64u)
                simdgroup_load(b3, tb, k8 * 64u + (qn + 24u), 64u)
                simdgroup_multiply_accumulate(c00, a0, b0, c00)
                simdgroup_multiply_accumulate(c01, a0, b1, c01)
                simdgroup_multiply_accumulate(c02, a0, b2, c02)
                simdgroup_multiply_accumulate(c03, a0, b3, c03)
                simdgroup_multiply_accumulate(c10, a1, b0, c10)
                simdgroup_multiply_accumulate(c11, a1, b1, c11)
                simdgroup_multiply_accumulate(c12, a1, b2, c12)
                simdgroup_multiply_accumulate(c13, a1, b3, c13)
                simdgroup_multiply_accumulate(c20, a2, b0, c20)
                simdgroup_multiply_accumulate(c21, a2, b1, c21)
                simdgroup_multiply_accumulate(c22, a2, b2, c22)
                simdgroup_multiply_accumulate(c23, a2, b3, c23)
                simdgroup_multiply_accumulate(c30, a3, b0, c30)
                simdgroup_multiply_accumulate(c31, a3, b1, c31)
                simdgroup_multiply_accumulate(c32, a3, b2, c32)
                simdgroup_multiply_accumulate(c33, a3, b3, c33)
                k8 += 8u
            }
            barrier()
            kb++
        }
        let row0 = mBase + qm
        let col0 = nBase + qn
        simdgroup_store(c00, y, row0 * ndim + col0, ndim)
        simdgroup_store(c01, y, row0 * ndim + (col0 + 8u), ndim)
        simdgroup_store(c02, y, row0 * ndim + (col0 + 16u), ndim)
        simdgroup_store(c03, y, row0 * ndim + (col0 + 24u), ndim)
        simdgroup_store(c10, y, (row0 + 8u) * ndim + col0, ndim)
        simdgroup_store(c11, y, (row0 + 8u) * ndim + (col0 + 8u), ndim)
        simdgroup_store(c12, y, (row0 + 8u) * ndim + (col0 + 16u), ndim)
        simdgroup_store(c13, y, (row0 + 8u) * ndim + (col0 + 24u), ndim)
        simdgroup_store(c20, y, (row0 + 16u) * ndim + col0, ndim)
        simdgroup_store(c21, y, (row0 + 16u) * ndim + (col0 + 8u), ndim)
        simdgroup_store(c22, y, (row0 + 16u) * ndim + (col0 + 16u), ndim)
        simdgroup_store(c23, y, (row0 + 16u) * ndim + (col0 + 24u), ndim)
        simdgroup_store(c30, y, (row0 + 24u) * ndim + col0, ndim)
        simdgroup_store(c31, y, (row0 + 24u) * ndim + (col0 + 8u), ndim)
        simdgroup_store(c32, y, (row0 + 24u) * ndim + (col0 + 16u), ndim)
        simdgroup_store(c33, y, (row0 + 24u) * ndim + (col0 + 24u), ndim)
    }
}

// v18: v13 with 32-ELEMENT runs — each thread stages one whole Q8 block (row, kb): ONE scale
// load (v13 pays it twice, once per 16-half) + 8 byte4 loads + 32 cvt/stores. 64 threads cover
// A, the other 64 cover B — same tiles, same mac loop, half the scale traffic and address setup.
class MetalGemmLab6464Run32 {
    @ssbo @binding = 0 wq : array<byte4>
    @ssbo @binding = 1 ws : array<float>
    @ssbo @binding = 2 xq : array<byte4>
    @ssbo @binding = 3 xs : array<float>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 kdim : uint
    @uniform @binding = 6 ndim : uint
    @workgroup ta : float16[2048]           // 64 tokens x 32 k
    @workgroup tb : float16[2048]           // 32 k x 64 outputs, transposed

    [metal_kernel(name="lab_6464run32_msl")]
    def lab_6464run32 {
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let ntileN = ndim / 64u
        let mBase = (gl_WorkGroupID.x / ntileN) * 64u
        let nBase = (gl_WorkGroupID.x % ntileN) * 64u
        let qm = (sg / 2u) * 32u
        let qn = (sg % 2u) * 32u
        var c00 : simdgroup_float8x8
        var c01 : simdgroup_float8x8
        var c02 : simdgroup_float8x8
        var c03 : simdgroup_float8x8
        var c10 : simdgroup_float8x8
        var c11 : simdgroup_float8x8
        var c12 : simdgroup_float8x8
        var c13 : simdgroup_float8x8
        var c20 : simdgroup_float8x8
        var c21 : simdgroup_float8x8
        var c22 : simdgroup_float8x8
        var c23 : simdgroup_float8x8
        var c30 : simdgroup_float8x8
        var c31 : simdgroup_float8x8
        var c32 : simdgroup_float8x8
        var c33 : simdgroup_float8x8
        let kdim4 = kdim / 4u
        let ra = lid % 64u                  // staged row (token row for A half, output row for B)
        let isB = lid >= 64u
        var kb = 0u
        while (kb * 32u < kdim) {
            if (isB) {
                let sw = ws[(nBase + ra) * (kdim / 32u) + kb]
                let srcw = (nBase + ra) * kdim4 + kb * 8u
                var j4 = 0u
                while (j4 < 8u) {
                    let v = float4(int4(wq[srcw + j4])) * sw
                    let o = j4 * 4u
                    tb[o * 64u + ra] = float16(v.x)
                    tb[(o + 1u) * 64u + ra] = float16(v.y)
                    tb[(o + 2u) * 64u + ra] = float16(v.z)
                    tb[(o + 3u) * 64u + ra] = float16(v.w)
                    j4++
                }
            } else {
                let s = xs[(mBase + ra) * (kdim / 32u) + kb]
                let src = (mBase + ra) * kdim4 + kb * 8u
                let dst = ra * 32u
                var i4 = 0u
                while (i4 < 8u) {
                    let v = float4(int4(xq[src + i4])) * s
                    let o = dst + i4 * 4u
                    ta[o] = float16(v.x)
                    ta[o + 1u] = float16(v.y)
                    ta[o + 2u] = float16(v.z)
                    ta[o + 3u] = float16(v.w)
                    i4++
                }
            }
            barrier()
            var k8 = 0u
            while (k8 < 32u) {
                var a0 : simdgroup_half8x8
                var a1 : simdgroup_half8x8
                var a2 : simdgroup_half8x8
                var a3 : simdgroup_half8x8
                var b0 : simdgroup_half8x8
                var b1 : simdgroup_half8x8
                var b2 : simdgroup_half8x8
                var b3 : simdgroup_half8x8
                simdgroup_load(a0, ta, qm * 32u + k8, 32u)
                simdgroup_load(a1, ta, (qm + 8u) * 32u + k8, 32u)
                simdgroup_load(a2, ta, (qm + 16u) * 32u + k8, 32u)
                simdgroup_load(a3, ta, (qm + 24u) * 32u + k8, 32u)
                simdgroup_load(b0, tb, k8 * 64u + qn, 64u)
                simdgroup_load(b1, tb, k8 * 64u + (qn + 8u), 64u)
                simdgroup_load(b2, tb, k8 * 64u + (qn + 16u), 64u)
                simdgroup_load(b3, tb, k8 * 64u + (qn + 24u), 64u)
                simdgroup_multiply_accumulate(c00, a0, b0, c00)
                simdgroup_multiply_accumulate(c01, a0, b1, c01)
                simdgroup_multiply_accumulate(c02, a0, b2, c02)
                simdgroup_multiply_accumulate(c03, a0, b3, c03)
                simdgroup_multiply_accumulate(c10, a1, b0, c10)
                simdgroup_multiply_accumulate(c11, a1, b1, c11)
                simdgroup_multiply_accumulate(c12, a1, b2, c12)
                simdgroup_multiply_accumulate(c13, a1, b3, c13)
                simdgroup_multiply_accumulate(c20, a2, b0, c20)
                simdgroup_multiply_accumulate(c21, a2, b1, c21)
                simdgroup_multiply_accumulate(c22, a2, b2, c22)
                simdgroup_multiply_accumulate(c23, a2, b3, c23)
                simdgroup_multiply_accumulate(c30, a3, b0, c30)
                simdgroup_multiply_accumulate(c31, a3, b1, c31)
                simdgroup_multiply_accumulate(c32, a3, b2, c32)
                simdgroup_multiply_accumulate(c33, a3, b3, c33)
                k8 += 8u
            }
            barrier()
            kb++
        }
        let row0 = mBase + qm
        let col0 = nBase + qn
        simdgroup_store(c00, y, row0 * ndim + col0, ndim)
        simdgroup_store(c01, y, row0 * ndim + (col0 + 8u), ndim)
        simdgroup_store(c02, y, row0 * ndim + (col0 + 16u), ndim)
        simdgroup_store(c03, y, row0 * ndim + (col0 + 24u), ndim)
        simdgroup_store(c10, y, (row0 + 8u) * ndim + col0, ndim)
        simdgroup_store(c11, y, (row0 + 8u) * ndim + (col0 + 8u), ndim)
        simdgroup_store(c12, y, (row0 + 8u) * ndim + (col0 + 16u), ndim)
        simdgroup_store(c13, y, (row0 + 8u) * ndim + (col0 + 24u), ndim)
        simdgroup_store(c20, y, (row0 + 16u) * ndim + col0, ndim)
        simdgroup_store(c21, y, (row0 + 16u) * ndim + (col0 + 8u), ndim)
        simdgroup_store(c22, y, (row0 + 16u) * ndim + (col0 + 16u), ndim)
        simdgroup_store(c23, y, (row0 + 16u) * ndim + (col0 + 24u), ndim)
        simdgroup_store(c30, y, (row0 + 24u) * ndim + col0, ndim)
        simdgroup_store(c31, y, (row0 + 24u) * ndim + (col0 + 8u), ndim)
        simdgroup_store(c32, y, (row0 + 24u) * ndim + (col0 + 16u), ndim)
        simdgroup_store(c33, y, (row0 + 24u) * ndim + (col0 + 24u), ndim)
    }
}

// v19: v13 with the staging rebuilt on the two emitter extensions — B stages ROW-major
// ([64 outputs x 32 k], contiguous like A) and the mac loop transposes at simdgroup_load
// (the MSL transpose_matrix flag), killing the 64-strided threadgroup writes; ALL staging
// stores collapse 4:1 through tg_store_half4 (one aligned half4 per byte4 dequant).
class MetalGemmLab6464Trans {
    @ssbo @binding = 0 wq : array<byte4>
    @ssbo @binding = 1 ws : array<float>
    @ssbo @binding = 2 xq : array<byte4>
    @ssbo @binding = 3 xs : array<float>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 kdim : uint
    @uniform @binding = 6 ndim : uint
    @workgroup ta : float16[2048]           // 64 tokens x 32 k
    @workgroup tb : float16[2048]           // 64 outputs x 32 k — ROW-major (transposed at load)

    [metal_kernel(name="lab_6464trans_msl")]
    def lab_6464trans {
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let ntileN = ndim / 64u
        let mBase = (gl_WorkGroupID.x / ntileN) * 64u
        let nBase = (gl_WorkGroupID.x % ntileN) * 64u
        let qm = (sg / 2u) * 32u
        let qn = (sg % 2u) * 32u
        var c00 : simdgroup_float8x8
        var c01 : simdgroup_float8x8
        var c02 : simdgroup_float8x8
        var c03 : simdgroup_float8x8
        var c10 : simdgroup_float8x8
        var c11 : simdgroup_float8x8
        var c12 : simdgroup_float8x8
        var c13 : simdgroup_float8x8
        var c20 : simdgroup_float8x8
        var c21 : simdgroup_float8x8
        var c22 : simdgroup_float8x8
        var c23 : simdgroup_float8x8
        var c30 : simdgroup_float8x8
        var c31 : simdgroup_float8x8
        var c32 : simdgroup_float8x8
        var c33 : simdgroup_float8x8
        let kdim4 = kdim / 4u
        let ra = lid / 2u
        let c0a = (lid % 2u) * 16u
        var kb = 0u
        while (kb * 32u < kdim) {
            {
                let s = xs[(mBase + ra) * (kdim / 32u) + kb]
                let src = (mBase + ra) * kdim4 + (kb * 32u + c0a) / 4u
                let dst = ra * 32u + c0a
                var i4 = 0u
                while (i4 < 4u) {
                    let v = float4(int4(xq[src + i4])) * s
                    tg_store_half4(ta, dst + i4 * 4u, v)
                    i4++
                }
            }
            {
                let sw = ws[(nBase + ra) * (kdim / 32u) + kb]
                let srcw = (nBase + ra) * kdim4 + (kb * 32u + c0a) / 4u
                let dstw = ra * 32u + c0a
                var j4 = 0u
                while (j4 < 4u) {
                    let v = float4(int4(wq[srcw + j4])) * sw
                    tg_store_half4(tb, dstw + j4 * 4u, v)
                    j4++
                }
            }
            barrier()
            var k8 = 0u
            while (k8 < 32u) {
                var a0 : simdgroup_half8x8
                var a1 : simdgroup_half8x8
                var a2 : simdgroup_half8x8
                var a3 : simdgroup_half8x8
                var b0 : simdgroup_half8x8
                var b1 : simdgroup_half8x8
                var b2 : simdgroup_half8x8
                var b3 : simdgroup_half8x8
                simdgroup_load(a0, ta, qm * 32u + k8, 32u)
                simdgroup_load(a1, ta, (qm + 8u) * 32u + k8, 32u)
                simdgroup_load(a2, ta, (qm + 16u) * 32u + k8, 32u)
                simdgroup_load(a3, ta, (qm + 24u) * 32u + k8, 32u)
                simdgroup_load(b0, tb, qn * 32u + k8, 32u, true)
                simdgroup_load(b1, tb, (qn + 8u) * 32u + k8, 32u, true)
                simdgroup_load(b2, tb, (qn + 16u) * 32u + k8, 32u, true)
                simdgroup_load(b3, tb, (qn + 24u) * 32u + k8, 32u, true)
                simdgroup_multiply_accumulate(c00, a0, b0, c00)
                simdgroup_multiply_accumulate(c01, a0, b1, c01)
                simdgroup_multiply_accumulate(c02, a0, b2, c02)
                simdgroup_multiply_accumulate(c03, a0, b3, c03)
                simdgroup_multiply_accumulate(c10, a1, b0, c10)
                simdgroup_multiply_accumulate(c11, a1, b1, c11)
                simdgroup_multiply_accumulate(c12, a1, b2, c12)
                simdgroup_multiply_accumulate(c13, a1, b3, c13)
                simdgroup_multiply_accumulate(c20, a2, b0, c20)
                simdgroup_multiply_accumulate(c21, a2, b1, c21)
                simdgroup_multiply_accumulate(c22, a2, b2, c22)
                simdgroup_multiply_accumulate(c23, a2, b3, c23)
                simdgroup_multiply_accumulate(c30, a3, b0, c30)
                simdgroup_multiply_accumulate(c31, a3, b1, c31)
                simdgroup_multiply_accumulate(c32, a3, b2, c32)
                simdgroup_multiply_accumulate(c33, a3, b3, c33)
                k8 += 8u
            }
            barrier()
            kb++
        }
        let row0 = mBase + qm
        let col0 = nBase + qn
        simdgroup_store(c00, y, row0 * ndim + col0, ndim)
        simdgroup_store(c01, y, row0 * ndim + (col0 + 8u), ndim)
        simdgroup_store(c02, y, row0 * ndim + (col0 + 16u), ndim)
        simdgroup_store(c03, y, row0 * ndim + (col0 + 24u), ndim)
        simdgroup_store(c10, y, (row0 + 8u) * ndim + col0, ndim)
        simdgroup_store(c11, y, (row0 + 8u) * ndim + (col0 + 8u), ndim)
        simdgroup_store(c12, y, (row0 + 8u) * ndim + (col0 + 16u), ndim)
        simdgroup_store(c13, y, (row0 + 8u) * ndim + (col0 + 24u), ndim)
        simdgroup_store(c20, y, (row0 + 16u) * ndim + col0, ndim)
        simdgroup_store(c21, y, (row0 + 16u) * ndim + (col0 + 8u), ndim)
        simdgroup_store(c22, y, (row0 + 16u) * ndim + (col0 + 16u), ndim)
        simdgroup_store(c23, y, (row0 + 16u) * ndim + (col0 + 24u), ndim)
        simdgroup_store(c30, y, (row0 + 24u) * ndim + col0, ndim)
        simdgroup_store(c31, y, (row0 + 24u) * ndim + (col0 + 8u), ndim)
        simdgroup_store(c32, y, (row0 + 24u) * ndim + (col0 + 16u), ndim)
        simdgroup_store(c33, y, (row0 + 24u) * ndim + (col0 + 24u), ndim)
    }
}

// v20: DEVICE-DIRECT B — the W tile never stages at all: B tiles simdgroup_load straight from
// a resident PRE-DEQUANTIZED f16 W panel (row-major, transposed at load), killing the whole
// B-side staging chain (device byte4 loads + dequant ALU + tg stores + half the barrier
// pressure); only the A (activation) half stages. Trades W device traffic 1.03 -> 2 B/weight.
class MetalGemmLab6464DevB {
    @ssbo @binding = 0 wh : array<float16>  // [d x kdim] f16 weights (dequantized at upload)
    @ssbo @binding = 2 xq : array<byte4>
    @ssbo @binding = 3 xs : array<float>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 kdim : uint
    @uniform @binding = 6 ndim : uint
    @workgroup ta : float16[2048]           // 64 tokens x 32 k — the only staged tile

    [metal_kernel(name="lab_6464devb_msl")]
    def lab_6464devb {
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let ntileN = ndim / 64u
        let mBase = (gl_WorkGroupID.x / ntileN) * 64u
        let nBase = (gl_WorkGroupID.x % ntileN) * 64u
        let qm = (sg / 2u) * 32u
        let qn = (sg % 2u) * 32u
        var c00 : simdgroup_float8x8
        var c01 : simdgroup_float8x8
        var c02 : simdgroup_float8x8
        var c03 : simdgroup_float8x8
        var c10 : simdgroup_float8x8
        var c11 : simdgroup_float8x8
        var c12 : simdgroup_float8x8
        var c13 : simdgroup_float8x8
        var c20 : simdgroup_float8x8
        var c21 : simdgroup_float8x8
        var c22 : simdgroup_float8x8
        var c23 : simdgroup_float8x8
        var c30 : simdgroup_float8x8
        var c31 : simdgroup_float8x8
        var c32 : simdgroup_float8x8
        var c33 : simdgroup_float8x8
        let kdim4 = kdim / 4u
        let ra = lid / 2u
        let c0a = (lid % 2u) * 16u
        var kb = 0u
        while (kb * 32u < kdim) {
            {
                let s = xs[(mBase + ra) * (kdim / 32u) + kb]
                let src = (mBase + ra) * kdim4 + (kb * 32u + c0a) / 4u
                let dst = ra * 32u + c0a
                var i4 = 0u
                while (i4 < 4u) {
                    let v = float4(int4(xq[src + i4])) * s
                    tg_store_half4(ta, dst + i4 * 4u, v)
                    i4++
                }
            }
            barrier()
            let kw = kb * 32u
            var k8 = 0u
            while (k8 < 32u) {
                var a0 : simdgroup_half8x8
                var a1 : simdgroup_half8x8
                var a2 : simdgroup_half8x8
                var a3 : simdgroup_half8x8
                var b0 : simdgroup_half8x8
                var b1 : simdgroup_half8x8
                var b2 : simdgroup_half8x8
                var b3 : simdgroup_half8x8
                simdgroup_load(a0, ta, qm * 32u + k8, 32u)
                simdgroup_load(a1, ta, (qm + 8u) * 32u + k8, 32u)
                simdgroup_load(a2, ta, (qm + 16u) * 32u + k8, 32u)
                simdgroup_load(a3, ta, (qm + 24u) * 32u + k8, 32u)
                simdgroup_load(b0, wh, (nBase + qn) * kdim + kw + k8, kdim, true)
                simdgroup_load(b1, wh, (nBase + qn + 8u) * kdim + kw + k8, kdim, true)
                simdgroup_load(b2, wh, (nBase + qn + 16u) * kdim + kw + k8, kdim, true)
                simdgroup_load(b3, wh, (nBase + qn + 24u) * kdim + kw + k8, kdim, true)
                simdgroup_multiply_accumulate(c00, a0, b0, c00)
                simdgroup_multiply_accumulate(c01, a0, b1, c01)
                simdgroup_multiply_accumulate(c02, a0, b2, c02)
                simdgroup_multiply_accumulate(c03, a0, b3, c03)
                simdgroup_multiply_accumulate(c10, a1, b0, c10)
                simdgroup_multiply_accumulate(c11, a1, b1, c11)
                simdgroup_multiply_accumulate(c12, a1, b2, c12)
                simdgroup_multiply_accumulate(c13, a1, b3, c13)
                simdgroup_multiply_accumulate(c20, a2, b0, c20)
                simdgroup_multiply_accumulate(c21, a2, b1, c21)
                simdgroup_multiply_accumulate(c22, a2, b2, c22)
                simdgroup_multiply_accumulate(c23, a2, b3, c23)
                simdgroup_multiply_accumulate(c30, a3, b0, c30)
                simdgroup_multiply_accumulate(c31, a3, b1, c31)
                simdgroup_multiply_accumulate(c32, a3, b2, c32)
                simdgroup_multiply_accumulate(c33, a3, b3, c33)
                k8 += 8u
            }
            barrier()
            kb++
        }
        let row0 = mBase + qm
        let col0 = nBase + qn
        simdgroup_store(c00, y, row0 * ndim + col0, ndim)
        simdgroup_store(c01, y, row0 * ndim + (col0 + 8u), ndim)
        simdgroup_store(c02, y, row0 * ndim + (col0 + 16u), ndim)
        simdgroup_store(c03, y, row0 * ndim + (col0 + 24u), ndim)
        simdgroup_store(c10, y, (row0 + 8u) * ndim + col0, ndim)
        simdgroup_store(c11, y, (row0 + 8u) * ndim + (col0 + 8u), ndim)
        simdgroup_store(c12, y, (row0 + 8u) * ndim + (col0 + 16u), ndim)
        simdgroup_store(c13, y, (row0 + 8u) * ndim + (col0 + 24u), ndim)
        simdgroup_store(c20, y, (row0 + 16u) * ndim + col0, ndim)
        simdgroup_store(c21, y, (row0 + 16u) * ndim + (col0 + 8u), ndim)
        simdgroup_store(c22, y, (row0 + 16u) * ndim + (col0 + 16u), ndim)
        simdgroup_store(c23, y, (row0 + 16u) * ndim + (col0 + 24u), ndim)
        simdgroup_store(c30, y, (row0 + 24u) * ndim + col0, ndim)
        simdgroup_store(c31, y, (row0 + 24u) * ndim + (col0 + 8u), ndim)
        simdgroup_store(c32, y, (row0 + 24u) * ndim + (col0 + 16u), ndim)
        simdgroup_store(c33, y, (row0 + 24u) * ndim + (col0 + 24u), ndim)
    }
}

// knockout: v13 with staging removed — the 64x64 math + barrier + store ceiling
class MetalGemmLab6464NoStage {
    @ssbo @binding = 0 wq : array<byte4>
    @ssbo @binding = 1 ws : array<float>
    @ssbo @binding = 2 xq : array<byte4>
    @ssbo @binding = 3 xs : array<float>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 kdim : uint
    @uniform @binding = 6 ndim : uint
    @workgroup ta : float16[2048]
    @workgroup tb : float16[2048]

    [metal_kernel(name="lab_6464nostage_msl")]
    def lab_6464nostage {
        let sg = gl_SubgroupID
        let ntileN = ndim / 64u
        let mBase = (gl_WorkGroupID.x / ntileN) * 64u
        let nBase = (gl_WorkGroupID.x % ntileN) * 64u
        let qm = (sg / 2u) * 32u
        let qn = (sg % 2u) * 32u
        var c00 : simdgroup_float8x8
        var c01 : simdgroup_float8x8
        var c02 : simdgroup_float8x8
        var c03 : simdgroup_float8x8
        var c10 : simdgroup_float8x8
        var c11 : simdgroup_float8x8
        var c12 : simdgroup_float8x8
        var c13 : simdgroup_float8x8
        var c20 : simdgroup_float8x8
        var c21 : simdgroup_float8x8
        var c22 : simdgroup_float8x8
        var c23 : simdgroup_float8x8
        var c30 : simdgroup_float8x8
        var c31 : simdgroup_float8x8
        var c32 : simdgroup_float8x8
        var c33 : simdgroup_float8x8
        var kb = 0u
        while (kb * 32u < kdim) {
            barrier()
            var k8 = 0u
            while (k8 < 32u) {
                var a0 : simdgroup_half8x8
                var a1 : simdgroup_half8x8
                var a2 : simdgroup_half8x8
                var a3 : simdgroup_half8x8
                var b0 : simdgroup_half8x8
                var b1 : simdgroup_half8x8
                var b2 : simdgroup_half8x8
                var b3 : simdgroup_half8x8
                simdgroup_load(a0, ta, qm * 32u + k8, 32u)
                simdgroup_load(a1, ta, (qm + 8u) * 32u + k8, 32u)
                simdgroup_load(a2, ta, (qm + 16u) * 32u + k8, 32u)
                simdgroup_load(a3, ta, (qm + 24u) * 32u + k8, 32u)
                simdgroup_load(b0, tb, k8 * 64u + qn, 64u)
                simdgroup_load(b1, tb, k8 * 64u + (qn + 8u), 64u)
                simdgroup_load(b2, tb, k8 * 64u + (qn + 16u), 64u)
                simdgroup_load(b3, tb, k8 * 64u + (qn + 24u), 64u)
                simdgroup_multiply_accumulate(c00, a0, b0, c00)
                simdgroup_multiply_accumulate(c01, a0, b1, c01)
                simdgroup_multiply_accumulate(c02, a0, b2, c02)
                simdgroup_multiply_accumulate(c03, a0, b3, c03)
                simdgroup_multiply_accumulate(c10, a1, b0, c10)
                simdgroup_multiply_accumulate(c11, a1, b1, c11)
                simdgroup_multiply_accumulate(c12, a1, b2, c12)
                simdgroup_multiply_accumulate(c13, a1, b3, c13)
                simdgroup_multiply_accumulate(c20, a2, b0, c20)
                simdgroup_multiply_accumulate(c21, a2, b1, c21)
                simdgroup_multiply_accumulate(c22, a2, b2, c22)
                simdgroup_multiply_accumulate(c23, a2, b3, c23)
                simdgroup_multiply_accumulate(c30, a3, b0, c30)
                simdgroup_multiply_accumulate(c31, a3, b1, c31)
                simdgroup_multiply_accumulate(c32, a3, b2, c32)
                simdgroup_multiply_accumulate(c33, a3, b3, c33)
                k8 += 8u
            }
            barrier()
            kb++
        }
        let row0 = mBase + qm
        let col0 = nBase + qn
        simdgroup_store(c00, y, row0 * ndim + col0, ndim)
        simdgroup_store(c01, y, row0 * ndim + (col0 + 8u), ndim)
        simdgroup_store(c02, y, row0 * ndim + (col0 + 16u), ndim)
        simdgroup_store(c03, y, row0 * ndim + (col0 + 24u), ndim)
        simdgroup_store(c10, y, (row0 + 8u) * ndim + col0, ndim)
        simdgroup_store(c11, y, (row0 + 8u) * ndim + (col0 + 8u), ndim)
        simdgroup_store(c12, y, (row0 + 8u) * ndim + (col0 + 16u), ndim)
        simdgroup_store(c13, y, (row0 + 8u) * ndim + (col0 + 24u), ndim)
        simdgroup_store(c20, y, (row0 + 16u) * ndim + col0, ndim)
        simdgroup_store(c21, y, (row0 + 16u) * ndim + (col0 + 8u), ndim)
        simdgroup_store(c22, y, (row0 + 16u) * ndim + (col0 + 16u), ndim)
        simdgroup_store(c23, y, (row0 + 16u) * ndim + (col0 + 24u), ndim)
        simdgroup_store(c30, y, (row0 + 24u) * ndim + col0, ndim)
        simdgroup_store(c31, y, (row0 + 24u) * ndim + (col0 + 8u), ndim)
        simdgroup_store(c32, y, (row0 + 24u) * ndim + (col0 + 16u), ndim)
        simdgroup_store(c33, y, (row0 + 24u) * ndim + (col0 + 24u), ndim)
    }
}

// v15: 64x64 tiles at 256 threads (8 simdgroups, 2x4 grid of 32x16 quadrants) — same tile
// memory, staging split across twice the threads (one 16-run each), macs per load 8:6.
class MetalGemmLab6464X256 {
    @ssbo @binding = 0 wq : array<byte4>
    @ssbo @binding = 1 ws : array<float>
    @ssbo @binding = 2 xq : array<byte4>
    @ssbo @binding = 3 xs : array<float>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 kdim : uint
    @uniform @binding = 6 ndim : uint
    @workgroup ta : float16[2048]           // 64 tokens x 32 k
    @workgroup tb : float16[2048]           // 32 k x 64 outputs, transposed

    [metal_kernel(name="lab_6464x256_msl")]
    def lab_6464x256 {
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let ntileN = ndim / 64u
        let mBase = (gl_WorkGroupID.x / ntileN) * 64u
        let nBase = (gl_WorkGroupID.x % ntileN) * 64u
        let qm = (sg / 4u) * 32u
        let qn = (sg % 4u) * 16u
        var c00 : simdgroup_float8x8
        var c01 : simdgroup_float8x8
        var c10 : simdgroup_float8x8
        var c11 : simdgroup_float8x8
        var c20 : simdgroup_float8x8
        var c21 : simdgroup_float8x8
        var c30 : simdgroup_float8x8
        var c31 : simdgroup_float8x8
        let kdim4 = kdim / 4u
        let run = lid % 128u
        let rr = run / 2u
        let c0 = (run % 2u) * 16u
        var kb = 0u
        while (kb * 32u < kdim) {
            if (lid < 128u) {               // A half: threads 0-127 (simdgroups 0-3)
                let s = xs[(mBase + rr) * (kdim / 32u) + kb]
                let src = (mBase + rr) * kdim4 + (kb * 32u + c0) / 4u
                let dst = rr * 32u + c0
                var i4 = 0u
                while (i4 < 4u) {
                    let v = float4(int4(xq[src + i4])) * s
                    let o = dst + i4 * 4u
                    ta[o] = float16(v.x)
                    ta[o + 1u] = float16(v.y)
                    ta[o + 2u] = float16(v.z)
                    ta[o + 3u] = float16(v.w)
                    i4++
                }
            } else {                        // B half: threads 128-255 (simdgroups 4-7)
                let s = ws[(nBase + rr) * (kdim / 32u) + kb]
                let src = (nBase + rr) * kdim4 + (kb * 32u + c0) / 4u
                var i4 = 0u
                while (i4 < 4u) {
                    let v = float4(int4(wq[src + i4])) * s
                    let o = c0 + i4 * 4u
                    tb[o * 64u + rr] = float16(v.x)
                    tb[(o + 1u) * 64u + rr] = float16(v.y)
                    tb[(o + 2u) * 64u + rr] = float16(v.z)
                    tb[(o + 3u) * 64u + rr] = float16(v.w)
                    i4++
                }
            }
            barrier()
            var k8 = 0u
            while (k8 < 32u) {
                var a0 : simdgroup_half8x8
                var a1 : simdgroup_half8x8
                var a2 : simdgroup_half8x8
                var a3 : simdgroup_half8x8
                var b0 : simdgroup_half8x8
                var b1 : simdgroup_half8x8
                simdgroup_load(a0, ta, qm * 32u + k8, 32u)
                simdgroup_load(a1, ta, (qm + 8u) * 32u + k8, 32u)
                simdgroup_load(a2, ta, (qm + 16u) * 32u + k8, 32u)
                simdgroup_load(a3, ta, (qm + 24u) * 32u + k8, 32u)
                simdgroup_load(b0, tb, k8 * 64u + qn, 64u)
                simdgroup_load(b1, tb, k8 * 64u + (qn + 8u), 64u)
                simdgroup_multiply_accumulate(c00, a0, b0, c00)
                simdgroup_multiply_accumulate(c01, a0, b1, c01)
                simdgroup_multiply_accumulate(c10, a1, b0, c10)
                simdgroup_multiply_accumulate(c11, a1, b1, c11)
                simdgroup_multiply_accumulate(c20, a2, b0, c20)
                simdgroup_multiply_accumulate(c21, a2, b1, c21)
                simdgroup_multiply_accumulate(c30, a3, b0, c30)
                simdgroup_multiply_accumulate(c31, a3, b1, c31)
                k8 += 8u
            }
            barrier()
            kb++
        }
        let row0 = mBase + qm
        let col0 = nBase + qn
        simdgroup_store(c00, y, row0 * ndim + col0, ndim)
        simdgroup_store(c01, y, row0 * ndim + (col0 + 8u), ndim)
        simdgroup_store(c10, y, (row0 + 8u) * ndim + col0, ndim)
        simdgroup_store(c11, y, (row0 + 8u) * ndim + (col0 + 8u), ndim)
        simdgroup_store(c20, y, (row0 + 16u) * ndim + col0, ndim)
        simdgroup_store(c21, y, (row0 + 16u) * ndim + (col0 + 8u), ndim)
        simdgroup_store(c30, y, (row0 + 24u) * ndim + col0, ndim)
        simdgroup_store(c31, y, (row0 + 24u) * ndim + (col0 + 8u), ndim)
    }
}

// v16: v13 + REGISTER PREFETCH — next block's device loads (8 byte4 + 2 scales per thread)
// issue before the math loop so their latency hides behind the macs; after the post-math
// barrier only the converts and threadgroup stores remain exposed. Same 8KB tiles.
class MetalGemmLab6464Prefetch {
    @ssbo @binding = 0 wq : array<byte4>
    @ssbo @binding = 1 ws : array<float>
    @ssbo @binding = 2 xq : array<byte4>
    @ssbo @binding = 3 xs : array<float>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 kdim : uint
    @uniform @binding = 6 ndim : uint
    @workgroup ta : float16[2048]           // 64 tokens x 32 k
    @workgroup tb : float16[2048]           // 32 k x 64 outputs, transposed

    [metal_kernel(name="lab_6464pf_msl")]
    def lab_6464pf {
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let ntileN = ndim / 64u
        let mBase = (gl_WorkGroupID.x / ntileN) * 64u
        let nBase = (gl_WorkGroupID.x % ntileN) * 64u
        let qm = (sg / 2u) * 32u
        let qn = (sg % 2u) * 32u
        var c00 : simdgroup_float8x8
        var c01 : simdgroup_float8x8
        var c02 : simdgroup_float8x8
        var c03 : simdgroup_float8x8
        var c10 : simdgroup_float8x8
        var c11 : simdgroup_float8x8
        var c12 : simdgroup_float8x8
        var c13 : simdgroup_float8x8
        var c20 : simdgroup_float8x8
        var c21 : simdgroup_float8x8
        var c22 : simdgroup_float8x8
        var c23 : simdgroup_float8x8
        var c30 : simdgroup_float8x8
        var c31 : simdgroup_float8x8
        var c32 : simdgroup_float8x8
        var c33 : simdgroup_float8x8
        let kdim4 = kdim / 4u
        let ra = lid / 2u
        let c0a = (lid % 2u) * 16u
        let srcA0 = (mBase + ra) * kdim4 + c0a / 4u
        let srcB0 = (nBase + ra) * kdim4 + c0a / 4u
        let dstA = ra * 32u + c0a
        // prologue: stage block 0 directly
        {
            let s = xs[(mBase + ra) * (kdim / 32u)]
            var i4 = 0u
            while (i4 < 4u) {
                let v = float4(int4(xq[srcA0 + i4])) * s
                let o = dstA + i4 * 4u
                ta[o] = float16(v.x)
                ta[o + 1u] = float16(v.y)
                ta[o + 2u] = float16(v.z)
                ta[o + 3u] = float16(v.w)
                i4++
            }
        }
        {
            let sw = ws[(nBase + ra) * (kdim / 32u)]
            var j4 = 0u
            while (j4 < 4u) {
                let v = float4(int4(wq[srcB0 + j4])) * sw
                let o = c0a + j4 * 4u
                tb[o * 64u + ra] = float16(v.x)
                tb[(o + 1u) * 64u + ra] = float16(v.y)
                tb[(o + 2u) * 64u + ra] = float16(v.z)
                tb[(o + 3u) * 64u + ra] = float16(v.w)
                j4++
            }
        }
        barrier()
        var kb = 0u
        while (kb * 32u < kdim) {
            // prefetch block kb+1 into registers — loads fly while the macs run
            var pa0 : byte4
            var pa1 : byte4
            var pa2 : byte4
            var pa3 : byte4
            var pb0 : byte4
            var pb1 : byte4
            var pb2 : byte4
            var pb3 : byte4
            var sa = 0.0
            var sb = 0.0
            let more = (kb + 1u) * 32u < kdim
            if (more) {
                let kn = kb + 1u
                sa = xs[(mBase + ra) * (kdim / 32u) + kn]
                sb = ws[(nBase + ra) * (kdim / 32u) + kn]
                let offA = srcA0 + kn * 8u
                pa0 = xq[offA]
                pa1 = xq[offA + 1u]
                pa2 = xq[offA + 2u]
                pa3 = xq[offA + 3u]
                let offB = srcB0 + kn * 8u
                pb0 = wq[offB]
                pb1 = wq[offB + 1u]
                pb2 = wq[offB + 2u]
                pb3 = wq[offB + 3u]
            }
            var k8 = 0u
            while (k8 < 32u) {
                var a0 : simdgroup_half8x8
                var a1 : simdgroup_half8x8
                var a2 : simdgroup_half8x8
                var a3 : simdgroup_half8x8
                var b0 : simdgroup_half8x8
                var b1 : simdgroup_half8x8
                var b2 : simdgroup_half8x8
                var b3 : simdgroup_half8x8
                simdgroup_load(a0, ta, qm * 32u + k8, 32u)
                simdgroup_load(a1, ta, (qm + 8u) * 32u + k8, 32u)
                simdgroup_load(a2, ta, (qm + 16u) * 32u + k8, 32u)
                simdgroup_load(a3, ta, (qm + 24u) * 32u + k8, 32u)
                simdgroup_load(b0, tb, k8 * 64u + qn, 64u)
                simdgroup_load(b1, tb, k8 * 64u + (qn + 8u), 64u)
                simdgroup_load(b2, tb, k8 * 64u + (qn + 16u), 64u)
                simdgroup_load(b3, tb, k8 * 64u + (qn + 24u), 64u)
                simdgroup_multiply_accumulate(c00, a0, b0, c00)
                simdgroup_multiply_accumulate(c01, a0, b1, c01)
                simdgroup_multiply_accumulate(c02, a0, b2, c02)
                simdgroup_multiply_accumulate(c03, a0, b3, c03)
                simdgroup_multiply_accumulate(c10, a1, b0, c10)
                simdgroup_multiply_accumulate(c11, a1, b1, c11)
                simdgroup_multiply_accumulate(c12, a1, b2, c12)
                simdgroup_multiply_accumulate(c13, a1, b3, c13)
                simdgroup_multiply_accumulate(c20, a2, b0, c20)
                simdgroup_multiply_accumulate(c21, a2, b1, c21)
                simdgroup_multiply_accumulate(c22, a2, b2, c22)
                simdgroup_multiply_accumulate(c23, a2, b3, c23)
                simdgroup_multiply_accumulate(c30, a3, b0, c30)
                simdgroup_multiply_accumulate(c31, a3, b1, c31)
                simdgroup_multiply_accumulate(c32, a3, b2, c32)
                simdgroup_multiply_accumulate(c33, a3, b3, c33)
                k8 += 8u
            }
            barrier()                       // tile reads done — safe to overwrite in place
            if (more) {
                let va0 = float4(int4(pa0)) * sa
                let va1 = float4(int4(pa1)) * sa
                let va2 = float4(int4(pa2)) * sa
                let va3 = float4(int4(pa3)) * sa
                ta[dstA] = float16(va0.x)
                ta[dstA + 1u] = float16(va0.y)
                ta[dstA + 2u] = float16(va0.z)
                ta[dstA + 3u] = float16(va0.w)
                ta[dstA + 4u] = float16(va1.x)
                ta[dstA + 5u] = float16(va1.y)
                ta[dstA + 6u] = float16(va1.z)
                ta[dstA + 7u] = float16(va1.w)
                ta[dstA + 8u] = float16(va2.x)
                ta[dstA + 9u] = float16(va2.y)
                ta[dstA + 10u] = float16(va2.z)
                ta[dstA + 11u] = float16(va2.w)
                ta[dstA + 12u] = float16(va3.x)
                ta[dstA + 13u] = float16(va3.y)
                ta[dstA + 14u] = float16(va3.z)
                ta[dstA + 15u] = float16(va3.w)
                let vb0 = float4(int4(pb0)) * sb
                let vb1 = float4(int4(pb1)) * sb
                let vb2 = float4(int4(pb2)) * sb
                let vb3 = float4(int4(pb3)) * sb
                let ob = c0a * 64u + ra
                tb[ob] = float16(vb0.x)
                tb[ob + 64u] = float16(vb0.y)
                tb[ob + 128u] = float16(vb0.z)
                tb[ob + 192u] = float16(vb0.w)
                tb[ob + 256u] = float16(vb1.x)
                tb[ob + 320u] = float16(vb1.y)
                tb[ob + 384u] = float16(vb1.z)
                tb[ob + 448u] = float16(vb1.w)
                tb[ob + 512u] = float16(vb2.x)
                tb[ob + 576u] = float16(vb2.y)
                tb[ob + 640u] = float16(vb2.z)
                tb[ob + 704u] = float16(vb2.w)
                tb[ob + 768u] = float16(vb3.x)
                tb[ob + 832u] = float16(vb3.y)
                tb[ob + 896u] = float16(vb3.z)
                tb[ob + 960u] = float16(vb3.w)
            }
            barrier()                       // next tiles staged
            kb++
        }
        let row0 = mBase + qm
        let col0 = nBase + qn
        simdgroup_store(c00, y, row0 * ndim + col0, ndim)
        simdgroup_store(c01, y, row0 * ndim + (col0 + 8u), ndim)
        simdgroup_store(c02, y, row0 * ndim + (col0 + 16u), ndim)
        simdgroup_store(c03, y, row0 * ndim + (col0 + 24u), ndim)
        simdgroup_store(c10, y, (row0 + 8u) * ndim + col0, ndim)
        simdgroup_store(c11, y, (row0 + 8u) * ndim + (col0 + 8u), ndim)
        simdgroup_store(c12, y, (row0 + 8u) * ndim + (col0 + 16u), ndim)
        simdgroup_store(c13, y, (row0 + 8u) * ndim + (col0 + 24u), ndim)
        simdgroup_store(c20, y, (row0 + 16u) * ndim + col0, ndim)
        simdgroup_store(c21, y, (row0 + 16u) * ndim + (col0 + 8u), ndim)
        simdgroup_store(c22, y, (row0 + 16u) * ndim + (col0 + 16u), ndim)
        simdgroup_store(c23, y, (row0 + 16u) * ndim + (col0 + 24u), ndim)
        simdgroup_store(c30, y, (row0 + 24u) * ndim + col0, ndim)
        simdgroup_store(c31, y, (row0 + 24u) * ndim + (col0 + 8u), ndim)
        simdgroup_store(c32, y, (row0 + 24u) * ndim + (col0 + 16u), ndim)
        simdgroup_store(c33, y, (row0 + 24u) * ndim + (col0 + 24u), ndim)
    }
}

// v21: the ggml kernel_mul_mm GEOMETRY on our planar Q8 contract — measured head-to-head vs
// the verbatim lcpp_mulmm. 64 outputs x 32 tokens per threadgroup (their NR0xNR1 — transposed
// vs our 64x64), TILE-MAJOR 8x8 shared blocks so every simdgroup_load is one contiguous
// 64-half run at stride 8, weights transposed WITHIN their tiles at staging (the math loop
// needs no transposed loads), and all device reads issued into registers BEFORE the
// pre-staging barrier (their latency-hiding order). Each simdgroup owns a 16-token x
// 32-output quadrant: 8 accumulators, 6 tile loads per 8-k slab. Deviations from ggml, both
// deliberate: our planar int8 + f32-scale buffers via byte4 loads (theirs: interleaved blobs
// read a byte at a time), and no simdgroup_barrier(mem_none) scheduling hints (no emitter
// spelling; production would inherit them via a fastmath-safe pragma if this lands).
class MetalGemmLabGgmlGeo {
    @ssbo @binding = 0 wq : array<byte4>
    @ssbo @binding = 1 ws : array<float>
    @ssbo @binding = 2 xq : array<byte4>
    @ssbo @binding = 3 xs : array<float>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 kdim : uint
    @uniform @binding = 6 ndim : uint
    @workgroup ta : float16[2048]           // W: 4 k-tiles x 8 row-tiles of 8x8, each tile (k row, wrow col)
    @workgroup tb : float16[1024]           // X: 4 k-tiles x 4 token-tiles of 8x8, each tile (token row, k col)

    [metal_kernel(name="lab_ggmlgeo_msl")]
    def lab_ggmlgeo {
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let mBase = gl_WorkGroupID.x * 32u  // token rows — FAST axis: concurrent tgs share the W panel (ggml rasterization)
        let nBase = gl_WorkGroupID.y * 64u  // output (weight) rows
        var c00 : simdgroup_float8x8
        var c01 : simdgroup_float8x8
        var c02 : simdgroup_float8x8
        var c03 : simdgroup_float8x8
        var c10 : simdgroup_float8x8
        var c11 : simdgroup_float8x8
        var c12 : simdgroup_float8x8
        var c13 : simdgroup_float8x8
        let kdim4 = kdim / 4u
        let nkb = kdim / 32u
        // A split: each thread stages 16 weight elems — row lr0, k half il0
        let lr0 = lid / 2u
        let il0 = lid % 2u
        let syA = lr0 / 8u
        let lxA = lr0 % 8u
        let tA0 = (il0 * 2u * 8u + syA) * 64u + lxA         // k-tiles il0*2 (lanes at stride 8)
        let tA1 = ((il0 * 2u + 1u) * 8u + syA) * 64u + lxA  // k-tile il0*2+1
        // B split: each thread stages 8 activation elems — token lr1, k offset iy
        let lr1 = lid / 4u
        let iy = (lid % 4u) * 8u
        let ibB = ((lid % 4u) * 4u + lr1 / 8u) * 64u + (lr1 % 8u) * 8u
        let aB = (sg % 2u) * 256u           // this simdgroup's A quadrant: 4 row-tiles of 64
        let bB = (sg / 2u) * 128u           // this simdgroup's B quadrant: 2 token-tiles of 64
        var ma0 : simdgroup_half8x8
        var ma1 : simdgroup_half8x8
        var ma2 : simdgroup_half8x8
        var ma3 : simdgroup_half8x8
        var mb0 : simdgroup_half8x8
        var mb1 : simdgroup_half8x8
        var kb = 0u
        while (kb < nkb) {
            // device reads first — they fly while the previous math loop drains
            let sA = ws[(nBase + lr0) * nkb + kb]
            let srcA = (nBase + lr0) * kdim4 + (kb * 32u + il0 * 16u) / 4u
            let a0 = wq[srcA]
            let a1 = wq[srcA + 1u]
            let a2 = wq[srcA + 2u]
            let a3 = wq[srcA + 3u]
            let sB = xs[(mBase + lr1) * nkb + kb]
            let srcB = (mBase + lr1) * kdim4 + (kb * 32u + iy) / 4u
            let b0 = xq[srcB]
            let b1 = xq[srcB + 1u]
            barrier()                       // previous math done reading the tiles
            let va0 = float4(int4(a0)) * sA
            let va1 = float4(int4(a1)) * sA
            let va2 = float4(int4(a2)) * sA
            let va3 = float4(int4(a3)) * sA
            ta[tA0] = float16(va0.x)        // ly = k%8 -> +8 per lane; +32 = second 4-k half
            ta[tA0 + 8u] = float16(va0.y)
            ta[tA0 + 16u] = float16(va0.z)
            ta[tA0 + 24u] = float16(va0.w)
            ta[tA0 + 32u] = float16(va1.x)
            ta[tA0 + 40u] = float16(va1.y)
            ta[tA0 + 48u] = float16(va1.z)
            ta[tA0 + 56u] = float16(va1.w)
            ta[tA1] = float16(va2.x)
            ta[tA1 + 8u] = float16(va2.y)
            ta[tA1 + 16u] = float16(va2.z)
            ta[tA1 + 24u] = float16(va2.w)
            ta[tA1 + 32u] = float16(va3.x)
            ta[tA1 + 40u] = float16(va3.y)
            ta[tA1 + 48u] = float16(va3.z)
            ta[tA1 + 56u] = float16(va3.w)
            let vb0 = float4(int4(b0)) * sB
            let vb1 = float4(int4(b1)) * sB
            tg_store_half4(tb, ibB, vb0)    // k runs contiguous within the tile row — 2 vector stores
            tg_store_half4(tb, ibB + 4u, vb1)
            barrier()
            // math fully unrolled (their FOR_UNROLL): 4 k-slabs x (6 tile loads + 8 macs),
            // constant tile offsets off hoisted quadrant bases
            simdgroup_load(ma0, ta, aB, 8u)
            simdgroup_load(ma1, ta, aB + 64u, 8u)
            simdgroup_load(ma2, ta, aB + 128u, 8u)
            simdgroup_load(ma3, ta, aB + 192u, 8u)
            simdgroup_load(mb0, tb, bB, 8u)
            simdgroup_load(mb1, tb, bB + 64u, 8u)
            simdgroup_multiply_accumulate(c00, mb0, ma0, c00)
            simdgroup_multiply_accumulate(c01, mb0, ma1, c01)
            simdgroup_multiply_accumulate(c02, mb0, ma2, c02)
            simdgroup_multiply_accumulate(c03, mb0, ma3, c03)
            simdgroup_multiply_accumulate(c10, mb1, ma0, c10)
            simdgroup_multiply_accumulate(c11, mb1, ma1, c11)
            simdgroup_multiply_accumulate(c12, mb1, ma2, c12)
            simdgroup_multiply_accumulate(c13, mb1, ma3, c13)
            simdgroup_load(ma0, ta, aB + 512u, 8u)
            simdgroup_load(ma1, ta, aB + 576u, 8u)
            simdgroup_load(ma2, ta, aB + 640u, 8u)
            simdgroup_load(ma3, ta, aB + 704u, 8u)
            simdgroup_load(mb0, tb, bB + 256u, 8u)
            simdgroup_load(mb1, tb, bB + 320u, 8u)
            simdgroup_multiply_accumulate(c00, mb0, ma0, c00)
            simdgroup_multiply_accumulate(c01, mb0, ma1, c01)
            simdgroup_multiply_accumulate(c02, mb0, ma2, c02)
            simdgroup_multiply_accumulate(c03, mb0, ma3, c03)
            simdgroup_multiply_accumulate(c10, mb1, ma0, c10)
            simdgroup_multiply_accumulate(c11, mb1, ma1, c11)
            simdgroup_multiply_accumulate(c12, mb1, ma2, c12)
            simdgroup_multiply_accumulate(c13, mb1, ma3, c13)
            simdgroup_load(ma0, ta, aB + 1024u, 8u)
            simdgroup_load(ma1, ta, aB + 1088u, 8u)
            simdgroup_load(ma2, ta, aB + 1152u, 8u)
            simdgroup_load(ma3, ta, aB + 1216u, 8u)
            simdgroup_load(mb0, tb, bB + 512u, 8u)
            simdgroup_load(mb1, tb, bB + 576u, 8u)
            simdgroup_multiply_accumulate(c00, mb0, ma0, c00)
            simdgroup_multiply_accumulate(c01, mb0, ma1, c01)
            simdgroup_multiply_accumulate(c02, mb0, ma2, c02)
            simdgroup_multiply_accumulate(c03, mb0, ma3, c03)
            simdgroup_multiply_accumulate(c10, mb1, ma0, c10)
            simdgroup_multiply_accumulate(c11, mb1, ma1, c11)
            simdgroup_multiply_accumulate(c12, mb1, ma2, c12)
            simdgroup_multiply_accumulate(c13, mb1, ma3, c13)
            simdgroup_load(ma0, ta, aB + 1536u, 8u)
            simdgroup_load(ma1, ta, aB + 1600u, 8u)
            simdgroup_load(ma2, ta, aB + 1664u, 8u)
            simdgroup_load(ma3, ta, aB + 1728u, 8u)
            simdgroup_load(mb0, tb, bB + 768u, 8u)
            simdgroup_load(mb1, tb, bB + 832u, 8u)
            simdgroup_multiply_accumulate(c00, mb0, ma0, c00)
            simdgroup_multiply_accumulate(c01, mb0, ma1, c01)
            simdgroup_multiply_accumulate(c02, mb0, ma2, c02)
            simdgroup_multiply_accumulate(c03, mb0, ma3, c03)
            simdgroup_multiply_accumulate(c10, mb1, ma0, c10)
            simdgroup_multiply_accumulate(c11, mb1, ma1, c11)
            simdgroup_multiply_accumulate(c12, mb1, ma2, c12)
            simdgroup_multiply_accumulate(c13, mb1, ma3, c13)
            kb++
        }
        let row0 = mBase + (sg / 2u) * 16u  // accumulator rows = tokens
        let col0 = nBase + (sg % 2u) * 32u  // accumulator cols = outputs
        simdgroup_store(c00, y, row0 * ndim + col0, ndim)
        simdgroup_store(c01, y, row0 * ndim + (col0 + 8u), ndim)
        simdgroup_store(c02, y, row0 * ndim + (col0 + 16u), ndim)
        simdgroup_store(c03, y, row0 * ndim + (col0 + 24u), ndim)
        simdgroup_store(c10, y, (row0 + 8u) * ndim + col0, ndim)
        simdgroup_store(c11, y, (row0 + 8u) * ndim + (col0 + 8u), ndim)
        simdgroup_store(c12, y, (row0 + 8u) * ndim + (col0 + 16u), ndim)
        simdgroup_store(c13, y, (row0 + 8u) * ndim + (col0 + 24u), ndim)
    }
}

class MetalGemmLabGgmlGeoF32X {
    @ssbo @binding = 0 wq : array<byte4>
    @ssbo @binding = 1 ws : array<float>
    @ssbo @binding = 2 xf : array<float4>   // PRE-DEQUANTIZED f32 activations — the lcpp B contract
    @ssbo @binding = 3 xs : array<float>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 kdim : uint
    @uniform @binding = 6 ndim : uint
    @workgroup ta : float16[2048]           // W: 4 k-tiles x 8 row-tiles of 8x8, each tile (k row, wrow col)
    @workgroup tb : float16[1024]           // X: 4 k-tiles x 4 token-tiles of 8x8, each tile (token row, k col)

    [metal_kernel(name="lab_ggmlgeof32x_msl")]
    def lab_ggmlgeof32x {
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let mBase = gl_WorkGroupID.x * 32u  // token rows — FAST axis: concurrent tgs share the W panel (ggml rasterization)
        let nBase = gl_WorkGroupID.y * 64u  // output (weight) rows
        var c00 : simdgroup_float8x8
        var c01 : simdgroup_float8x8
        var c02 : simdgroup_float8x8
        var c03 : simdgroup_float8x8
        var c10 : simdgroup_float8x8
        var c11 : simdgroup_float8x8
        var c12 : simdgroup_float8x8
        var c13 : simdgroup_float8x8
        let kdim4 = kdim / 4u
        let nkb = kdim / 32u
        // A split: each thread stages 16 weight elems — row lr0, k half il0
        let lr0 = lid / 2u
        let il0 = lid % 2u
        let syA = lr0 / 8u
        let lxA = lr0 % 8u
        let tA0 = (il0 * 2u * 8u + syA) * 64u + lxA         // k-tiles il0*2 (lanes at stride 8)
        let tA1 = ((il0 * 2u + 1u) * 8u + syA) * 64u + lxA  // k-tile il0*2+1
        // B split: each thread stages 8 activation elems — token lr1, k offset iy
        let lr1 = lid / 4u
        let iy = (lid % 4u) * 8u
        let ibB = ((lid % 4u) * 4u + lr1 / 8u) * 64u + (lr1 % 8u) * 8u
        let aB = (sg % 2u) * 256u           // this simdgroup's A quadrant: 4 row-tiles of 64
        let bB = (sg / 2u) * 128u           // this simdgroup's B quadrant: 2 token-tiles of 64
        var ma0 : simdgroup_half8x8
        var ma1 : simdgroup_half8x8
        var ma2 : simdgroup_half8x8
        var ma3 : simdgroup_half8x8
        var mb0 : simdgroup_half8x8
        var mb1 : simdgroup_half8x8
        var kb = 0u
        while (kb < nkb) {
            // device reads first — they fly while the previous math loop drains
            let sA = ws[(nBase + lr0) * nkb + kb]
            let srcA = (nBase + lr0) * kdim4 + (kb * 32u + il0 * 16u) / 4u
            let a0 = wq[srcA]
            let a1 = wq[srcA + 1u]
            let a2 = wq[srcA + 2u]
            let a3 = wq[srcA + 3u]
            let srcB = (mBase + lr1) * kdim4 + (kb * 32u + iy) / 4u
            let b0 = xf[srcB]
            let b1 = xf[srcB + 1u]
            barrier()                       // previous math done reading the tiles
            let va0 = float4(int4(a0)) * sA
            let va1 = float4(int4(a1)) * sA
            let va2 = float4(int4(a2)) * sA
            let va3 = float4(int4(a3)) * sA
            ta[tA0] = float16(va0.x)        // ly = k%8 -> +8 per lane; +32 = second 4-k half
            ta[tA0 + 8u] = float16(va0.y)
            ta[tA0 + 16u] = float16(va0.z)
            ta[tA0 + 24u] = float16(va0.w)
            ta[tA0 + 32u] = float16(va1.x)
            ta[tA0 + 40u] = float16(va1.y)
            ta[tA0 + 48u] = float16(va1.z)
            ta[tA0 + 56u] = float16(va1.w)
            ta[tA1] = float16(va2.x)
            ta[tA1 + 8u] = float16(va2.y)
            ta[tA1 + 16u] = float16(va2.z)
            ta[tA1 + 24u] = float16(va2.w)
            ta[tA1 + 32u] = float16(va3.x)
            ta[tA1 + 40u] = float16(va3.y)
            ta[tA1 + 48u] = float16(va3.z)
            ta[tA1 + 56u] = float16(va3.w)
            tg_store_half4(tb, ibB, b0)     // no dequant on B — pure convert-and-store
            tg_store_half4(tb, ibB + 4u, b1)
            barrier()
            // math fully unrolled (their FOR_UNROLL): 4 k-slabs x (6 tile loads + 8 macs),
            // constant tile offsets off hoisted quadrant bases
            simdgroup_load(ma0, ta, aB, 8u)
            simdgroup_load(ma1, ta, aB + 64u, 8u)
            simdgroup_load(ma2, ta, aB + 128u, 8u)
            simdgroup_load(ma3, ta, aB + 192u, 8u)
            simdgroup_load(mb0, tb, bB, 8u)
            simdgroup_load(mb1, tb, bB + 64u, 8u)
            simdgroup_multiply_accumulate(c00, mb0, ma0, c00)
            simdgroup_multiply_accumulate(c01, mb0, ma1, c01)
            simdgroup_multiply_accumulate(c02, mb0, ma2, c02)
            simdgroup_multiply_accumulate(c03, mb0, ma3, c03)
            simdgroup_multiply_accumulate(c10, mb1, ma0, c10)
            simdgroup_multiply_accumulate(c11, mb1, ma1, c11)
            simdgroup_multiply_accumulate(c12, mb1, ma2, c12)
            simdgroup_multiply_accumulate(c13, mb1, ma3, c13)
            simdgroup_load(ma0, ta, aB + 512u, 8u)
            simdgroup_load(ma1, ta, aB + 576u, 8u)
            simdgroup_load(ma2, ta, aB + 640u, 8u)
            simdgroup_load(ma3, ta, aB + 704u, 8u)
            simdgroup_load(mb0, tb, bB + 256u, 8u)
            simdgroup_load(mb1, tb, bB + 320u, 8u)
            simdgroup_multiply_accumulate(c00, mb0, ma0, c00)
            simdgroup_multiply_accumulate(c01, mb0, ma1, c01)
            simdgroup_multiply_accumulate(c02, mb0, ma2, c02)
            simdgroup_multiply_accumulate(c03, mb0, ma3, c03)
            simdgroup_multiply_accumulate(c10, mb1, ma0, c10)
            simdgroup_multiply_accumulate(c11, mb1, ma1, c11)
            simdgroup_multiply_accumulate(c12, mb1, ma2, c12)
            simdgroup_multiply_accumulate(c13, mb1, ma3, c13)
            simdgroup_load(ma0, ta, aB + 1024u, 8u)
            simdgroup_load(ma1, ta, aB + 1088u, 8u)
            simdgroup_load(ma2, ta, aB + 1152u, 8u)
            simdgroup_load(ma3, ta, aB + 1216u, 8u)
            simdgroup_load(mb0, tb, bB + 512u, 8u)
            simdgroup_load(mb1, tb, bB + 576u, 8u)
            simdgroup_multiply_accumulate(c00, mb0, ma0, c00)
            simdgroup_multiply_accumulate(c01, mb0, ma1, c01)
            simdgroup_multiply_accumulate(c02, mb0, ma2, c02)
            simdgroup_multiply_accumulate(c03, mb0, ma3, c03)
            simdgroup_multiply_accumulate(c10, mb1, ma0, c10)
            simdgroup_multiply_accumulate(c11, mb1, ma1, c11)
            simdgroup_multiply_accumulate(c12, mb1, ma2, c12)
            simdgroup_multiply_accumulate(c13, mb1, ma3, c13)
            simdgroup_load(ma0, ta, aB + 1536u, 8u)
            simdgroup_load(ma1, ta, aB + 1600u, 8u)
            simdgroup_load(ma2, ta, aB + 1664u, 8u)
            simdgroup_load(ma3, ta, aB + 1728u, 8u)
            simdgroup_load(mb0, tb, bB + 768u, 8u)
            simdgroup_load(mb1, tb, bB + 832u, 8u)
            simdgroup_multiply_accumulate(c00, mb0, ma0, c00)
            simdgroup_multiply_accumulate(c01, mb0, ma1, c01)
            simdgroup_multiply_accumulate(c02, mb0, ma2, c02)
            simdgroup_multiply_accumulate(c03, mb0, ma3, c03)
            simdgroup_multiply_accumulate(c10, mb1, ma0, c10)
            simdgroup_multiply_accumulate(c11, mb1, ma1, c11)
            simdgroup_multiply_accumulate(c12, mb1, ma2, c12)
            simdgroup_multiply_accumulate(c13, mb1, ma3, c13)
            kb++
        }
        let row0 = mBase + (sg / 2u) * 16u  // accumulator rows = tokens
        let col0 = nBase + (sg % 2u) * 32u  // accumulator cols = outputs
        simdgroup_store(c00, y, row0 * ndim + col0, ndim)
        simdgroup_store(c01, y, row0 * ndim + (col0 + 8u), ndim)
        simdgroup_store(c02, y, row0 * ndim + (col0 + 16u), ndim)
        simdgroup_store(c03, y, row0 * ndim + (col0 + 24u), ndim)
        simdgroup_store(c10, y, (row0 + 8u) * ndim + col0, ndim)
        simdgroup_store(c11, y, (row0 + 8u) * ndim + (col0 + 8u), ndim)
        simdgroup_store(c12, y, (row0 + 8u) * ndim + (col0 + 16u), ndim)
        simdgroup_store(c13, y, (row0 + 8u) * ndim + (col0 + 24u), ndim)
    }
}

class MetalGemmLabGgmlGeoNoScale {
    @ssbo @binding = 0 wq : array<byte4>
    @ssbo @binding = 1 ws : array<float>
    @ssbo @binding = 2 xq : array<byte4>
    @ssbo @binding = 3 xs : array<float>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 kdim : uint
    @uniform @binding = 6 ndim : uint
    @workgroup ta : float16[2048]           // W: 4 k-tiles x 8 row-tiles of 8x8, each tile (k row, wrow col)
    @workgroup tb : float16[1024]           // X: 4 k-tiles x 4 token-tiles of 8x8, each tile (token row, k col)

    [metal_kernel(name="lab_ggmlgeonosc_msl")]
    def lab_ggmlgeonosc {
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let mBase = gl_WorkGroupID.x * 32u  // token rows — FAST axis: concurrent tgs share the W panel (ggml rasterization)
        let nBase = gl_WorkGroupID.y * 64u  // output (weight) rows
        var c00 : simdgroup_float8x8
        var c01 : simdgroup_float8x8
        var c02 : simdgroup_float8x8
        var c03 : simdgroup_float8x8
        var c10 : simdgroup_float8x8
        var c11 : simdgroup_float8x8
        var c12 : simdgroup_float8x8
        var c13 : simdgroup_float8x8
        let kdim4 = kdim / 4u
        let nkb = kdim / 32u
        // A split: each thread stages 16 weight elems — row lr0, k half il0
        let lr0 = lid / 2u
        let il0 = lid % 2u
        let syA = lr0 / 8u
        let lxA = lr0 % 8u
        let tA0 = (il0 * 2u * 8u + syA) * 64u + lxA         // k-tiles il0*2 (lanes at stride 8)
        let tA1 = ((il0 * 2u + 1u) * 8u + syA) * 64u + lxA  // k-tile il0*2+1
        // B split: each thread stages 8 activation elems — token lr1, k offset iy
        let lr1 = lid / 4u
        let iy = (lid % 4u) * 8u
        let ibB = ((lid % 4u) * 4u + lr1 / 8u) * 64u + (lr1 % 8u) * 8u
        let aB = (sg % 2u) * 256u           // this simdgroup's A quadrant: 4 row-tiles of 64
        let bB = (sg / 2u) * 128u           // this simdgroup's B quadrant: 2 token-tiles of 64
        var ma0 : simdgroup_half8x8
        var ma1 : simdgroup_half8x8
        var ma2 : simdgroup_half8x8
        var ma3 : simdgroup_half8x8
        var mb0 : simdgroup_half8x8
        var mb1 : simdgroup_half8x8
        var kb = 0u
        while (kb < nkb) {
            // device reads first — they fly while the previous math loop drains
            let sA = 1.0
            let srcA = (nBase + lr0) * kdim4 + (kb * 32u + il0 * 16u) / 4u
            let a0 = wq[srcA]
            let a1 = wq[srcA + 1u]
            let a2 = wq[srcA + 2u]
            let a3 = wq[srcA + 3u]
            let sB = 1.0
            let srcB = (mBase + lr1) * kdim4 + (kb * 32u + iy) / 4u
            let b0 = xq[srcB]
            let b1 = xq[srcB + 1u]
            barrier()                       // previous math done reading the tiles
            let va0 = float4(int4(a0)) * sA
            let va1 = float4(int4(a1)) * sA
            let va2 = float4(int4(a2)) * sA
            let va3 = float4(int4(a3)) * sA
            ta[tA0] = float16(va0.x)        // ly = k%8 -> +8 per lane; +32 = second 4-k half
            ta[tA0 + 8u] = float16(va0.y)
            ta[tA0 + 16u] = float16(va0.z)
            ta[tA0 + 24u] = float16(va0.w)
            ta[tA0 + 32u] = float16(va1.x)
            ta[tA0 + 40u] = float16(va1.y)
            ta[tA0 + 48u] = float16(va1.z)
            ta[tA0 + 56u] = float16(va1.w)
            ta[tA1] = float16(va2.x)
            ta[tA1 + 8u] = float16(va2.y)
            ta[tA1 + 16u] = float16(va2.z)
            ta[tA1 + 24u] = float16(va2.w)
            ta[tA1 + 32u] = float16(va3.x)
            ta[tA1 + 40u] = float16(va3.y)
            ta[tA1 + 48u] = float16(va3.z)
            ta[tA1 + 56u] = float16(va3.w)
            let vb0 = float4(int4(b0)) * sB
            let vb1 = float4(int4(b1)) * sB
            tg_store_half4(tb, ibB, vb0)    // k runs contiguous within the tile row — 2 vector stores
            tg_store_half4(tb, ibB + 4u, vb1)
            barrier()
            // math fully unrolled (their FOR_UNROLL): 4 k-slabs x (6 tile loads + 8 macs),
            // constant tile offsets off hoisted quadrant bases
            simdgroup_load(ma0, ta, aB, 8u)
            simdgroup_load(ma1, ta, aB + 64u, 8u)
            simdgroup_load(ma2, ta, aB + 128u, 8u)
            simdgroup_load(ma3, ta, aB + 192u, 8u)
            simdgroup_load(mb0, tb, bB, 8u)
            simdgroup_load(mb1, tb, bB + 64u, 8u)
            simdgroup_multiply_accumulate(c00, mb0, ma0, c00)
            simdgroup_multiply_accumulate(c01, mb0, ma1, c01)
            simdgroup_multiply_accumulate(c02, mb0, ma2, c02)
            simdgroup_multiply_accumulate(c03, mb0, ma3, c03)
            simdgroup_multiply_accumulate(c10, mb1, ma0, c10)
            simdgroup_multiply_accumulate(c11, mb1, ma1, c11)
            simdgroup_multiply_accumulate(c12, mb1, ma2, c12)
            simdgroup_multiply_accumulate(c13, mb1, ma3, c13)
            simdgroup_load(ma0, ta, aB + 512u, 8u)
            simdgroup_load(ma1, ta, aB + 576u, 8u)
            simdgroup_load(ma2, ta, aB + 640u, 8u)
            simdgroup_load(ma3, ta, aB + 704u, 8u)
            simdgroup_load(mb0, tb, bB + 256u, 8u)
            simdgroup_load(mb1, tb, bB + 320u, 8u)
            simdgroup_multiply_accumulate(c00, mb0, ma0, c00)
            simdgroup_multiply_accumulate(c01, mb0, ma1, c01)
            simdgroup_multiply_accumulate(c02, mb0, ma2, c02)
            simdgroup_multiply_accumulate(c03, mb0, ma3, c03)
            simdgroup_multiply_accumulate(c10, mb1, ma0, c10)
            simdgroup_multiply_accumulate(c11, mb1, ma1, c11)
            simdgroup_multiply_accumulate(c12, mb1, ma2, c12)
            simdgroup_multiply_accumulate(c13, mb1, ma3, c13)
            simdgroup_load(ma0, ta, aB + 1024u, 8u)
            simdgroup_load(ma1, ta, aB + 1088u, 8u)
            simdgroup_load(ma2, ta, aB + 1152u, 8u)
            simdgroup_load(ma3, ta, aB + 1216u, 8u)
            simdgroup_load(mb0, tb, bB + 512u, 8u)
            simdgroup_load(mb1, tb, bB + 576u, 8u)
            simdgroup_multiply_accumulate(c00, mb0, ma0, c00)
            simdgroup_multiply_accumulate(c01, mb0, ma1, c01)
            simdgroup_multiply_accumulate(c02, mb0, ma2, c02)
            simdgroup_multiply_accumulate(c03, mb0, ma3, c03)
            simdgroup_multiply_accumulate(c10, mb1, ma0, c10)
            simdgroup_multiply_accumulate(c11, mb1, ma1, c11)
            simdgroup_multiply_accumulate(c12, mb1, ma2, c12)
            simdgroup_multiply_accumulate(c13, mb1, ma3, c13)
            simdgroup_load(ma0, ta, aB + 1536u, 8u)
            simdgroup_load(ma1, ta, aB + 1600u, 8u)
            simdgroup_load(ma2, ta, aB + 1664u, 8u)
            simdgroup_load(ma3, ta, aB + 1728u, 8u)
            simdgroup_load(mb0, tb, bB + 768u, 8u)
            simdgroup_load(mb1, tb, bB + 832u, 8u)
            simdgroup_multiply_accumulate(c00, mb0, ma0, c00)
            simdgroup_multiply_accumulate(c01, mb0, ma1, c01)
            simdgroup_multiply_accumulate(c02, mb0, ma2, c02)
            simdgroup_multiply_accumulate(c03, mb0, ma3, c03)
            simdgroup_multiply_accumulate(c10, mb1, ma0, c10)
            simdgroup_multiply_accumulate(c11, mb1, ma1, c11)
            simdgroup_multiply_accumulate(c12, mb1, ma2, c12)
            simdgroup_multiply_accumulate(c13, mb1, ma3, c13)
            kb++
        }
        let row0 = mBase + (sg / 2u) * 16u  // accumulator rows = tokens
        let col0 = nBase + (sg % 2u) * 32u  // accumulator cols = outputs
        simdgroup_store(c00, y, row0 * ndim + col0, ndim)
        simdgroup_store(c01, y, row0 * ndim + (col0 + 8u), ndim)
        simdgroup_store(c02, y, row0 * ndim + (col0 + 16u), ndim)
        simdgroup_store(c03, y, row0 * ndim + (col0 + 24u), ndim)
        simdgroup_store(c10, y, (row0 + 8u) * ndim + col0, ndim)
        simdgroup_store(c11, y, (row0 + 8u) * ndim + (col0 + 8u), ndim)
        simdgroup_store(c12, y, (row0 + 8u) * ndim + (col0 + 16u), ndim)
        simdgroup_store(c13, y, (row0 + 8u) * ndim + (col0 + 24u), ndim)
    }
}

class MetalGemmLabGgmlGeoScT {
    @ssbo @binding = 0 wq : array<byte4>
    @ssbo @binding = 1 ws : array<float>     // TRANSPOSED: [block][row] — 64 A-scales/tg = 4 cachelines, not 64
    @ssbo @binding = 2 xq : array<byte4>
    @ssbo @binding = 3 xs : array<float>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 kdim : uint
    @uniform @binding = 6 ndim : uint
    @workgroup ta : float16[2048]           // W: 4 k-tiles x 8 row-tiles of 8x8, each tile (k row, wrow col)
    @workgroup tb : float16[1024]           // X: 4 k-tiles x 4 token-tiles of 8x8, each tile (token row, k col)

    [metal_kernel(name="lab_ggmlgeosct_msl")]
    def lab_ggmlgeosct {
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let mBase = gl_WorkGroupID.x * 32u  // token rows — FAST axis: concurrent tgs share the W panel (ggml rasterization)
        let nBase = gl_WorkGroupID.y * 64u  // output (weight) rows
        var c00 : simdgroup_float8x8
        var c01 : simdgroup_float8x8
        var c02 : simdgroup_float8x8
        var c03 : simdgroup_float8x8
        var c10 : simdgroup_float8x8
        var c11 : simdgroup_float8x8
        var c12 : simdgroup_float8x8
        var c13 : simdgroup_float8x8
        let kdim4 = kdim / 4u
        let nkb = kdim / 32u
        // A split: each thread stages 16 weight elems — row lr0, k half il0
        let lr0 = lid / 2u
        let il0 = lid % 2u
        let syA = lr0 / 8u
        let lxA = lr0 % 8u
        let tA0 = (il0 * 2u * 8u + syA) * 64u + lxA         // k-tiles il0*2 (lanes at stride 8)
        let tA1 = ((il0 * 2u + 1u) * 8u + syA) * 64u + lxA  // k-tile il0*2+1
        // B split: each thread stages 8 activation elems — token lr1, k offset iy
        let lr1 = lid / 4u
        let iy = (lid % 4u) * 8u
        let ibB = ((lid % 4u) * 4u + lr1 / 8u) * 64u + (lr1 % 8u) * 8u
        let aB = (sg % 2u) * 256u           // this simdgroup's A quadrant: 4 row-tiles of 64
        let bB = (sg / 2u) * 128u           // this simdgroup's B quadrant: 2 token-tiles of 64
        var ma0 : simdgroup_half8x8
        var ma1 : simdgroup_half8x8
        var ma2 : simdgroup_half8x8
        var ma3 : simdgroup_half8x8
        var mb0 : simdgroup_half8x8
        var mb1 : simdgroup_half8x8
        var kb = 0u
        while (kb < nkb) {
            // device reads first — they fly while the previous math loop drains
            let sA = ws[kb * ndim + nBase + lr0]
            let srcA = (nBase + lr0) * kdim4 + (kb * 32u + il0 * 16u) / 4u
            let a0 = wq[srcA]
            let a1 = wq[srcA + 1u]
            let a2 = wq[srcA + 2u]
            let a3 = wq[srcA + 3u]
            let sB = xs[(mBase + lr1) * nkb + kb]
            let srcB = (mBase + lr1) * kdim4 + (kb * 32u + iy) / 4u
            let b0 = xq[srcB]
            let b1 = xq[srcB + 1u]
            barrier()                       // previous math done reading the tiles
            let va0 = float4(int4(a0)) * sA
            let va1 = float4(int4(a1)) * sA
            let va2 = float4(int4(a2)) * sA
            let va3 = float4(int4(a3)) * sA
            ta[tA0] = float16(va0.x)        // ly = k%8 -> +8 per lane; +32 = second 4-k half
            ta[tA0 + 8u] = float16(va0.y)
            ta[tA0 + 16u] = float16(va0.z)
            ta[tA0 + 24u] = float16(va0.w)
            ta[tA0 + 32u] = float16(va1.x)
            ta[tA0 + 40u] = float16(va1.y)
            ta[tA0 + 48u] = float16(va1.z)
            ta[tA0 + 56u] = float16(va1.w)
            ta[tA1] = float16(va2.x)
            ta[tA1 + 8u] = float16(va2.y)
            ta[tA1 + 16u] = float16(va2.z)
            ta[tA1 + 24u] = float16(va2.w)
            ta[tA1 + 32u] = float16(va3.x)
            ta[tA1 + 40u] = float16(va3.y)
            ta[tA1 + 48u] = float16(va3.z)
            ta[tA1 + 56u] = float16(va3.w)
            let vb0 = float4(int4(b0)) * sB
            let vb1 = float4(int4(b1)) * sB
            tg_store_half4(tb, ibB, vb0)    // k runs contiguous within the tile row — 2 vector stores
            tg_store_half4(tb, ibB + 4u, vb1)
            barrier()
            // math fully unrolled (their FOR_UNROLL): 4 k-slabs x (6 tile loads + 8 macs),
            // constant tile offsets off hoisted quadrant bases
            simdgroup_load(ma0, ta, aB, 8u)
            simdgroup_load(ma1, ta, aB + 64u, 8u)
            simdgroup_load(ma2, ta, aB + 128u, 8u)
            simdgroup_load(ma3, ta, aB + 192u, 8u)
            simdgroup_load(mb0, tb, bB, 8u)
            simdgroup_load(mb1, tb, bB + 64u, 8u)
            simdgroup_multiply_accumulate(c00, mb0, ma0, c00)
            simdgroup_multiply_accumulate(c01, mb0, ma1, c01)
            simdgroup_multiply_accumulate(c02, mb0, ma2, c02)
            simdgroup_multiply_accumulate(c03, mb0, ma3, c03)
            simdgroup_multiply_accumulate(c10, mb1, ma0, c10)
            simdgroup_multiply_accumulate(c11, mb1, ma1, c11)
            simdgroup_multiply_accumulate(c12, mb1, ma2, c12)
            simdgroup_multiply_accumulate(c13, mb1, ma3, c13)
            simdgroup_load(ma0, ta, aB + 512u, 8u)
            simdgroup_load(ma1, ta, aB + 576u, 8u)
            simdgroup_load(ma2, ta, aB + 640u, 8u)
            simdgroup_load(ma3, ta, aB + 704u, 8u)
            simdgroup_load(mb0, tb, bB + 256u, 8u)
            simdgroup_load(mb1, tb, bB + 320u, 8u)
            simdgroup_multiply_accumulate(c00, mb0, ma0, c00)
            simdgroup_multiply_accumulate(c01, mb0, ma1, c01)
            simdgroup_multiply_accumulate(c02, mb0, ma2, c02)
            simdgroup_multiply_accumulate(c03, mb0, ma3, c03)
            simdgroup_multiply_accumulate(c10, mb1, ma0, c10)
            simdgroup_multiply_accumulate(c11, mb1, ma1, c11)
            simdgroup_multiply_accumulate(c12, mb1, ma2, c12)
            simdgroup_multiply_accumulate(c13, mb1, ma3, c13)
            simdgroup_load(ma0, ta, aB + 1024u, 8u)
            simdgroup_load(ma1, ta, aB + 1088u, 8u)
            simdgroup_load(ma2, ta, aB + 1152u, 8u)
            simdgroup_load(ma3, ta, aB + 1216u, 8u)
            simdgroup_load(mb0, tb, bB + 512u, 8u)
            simdgroup_load(mb1, tb, bB + 576u, 8u)
            simdgroup_multiply_accumulate(c00, mb0, ma0, c00)
            simdgroup_multiply_accumulate(c01, mb0, ma1, c01)
            simdgroup_multiply_accumulate(c02, mb0, ma2, c02)
            simdgroup_multiply_accumulate(c03, mb0, ma3, c03)
            simdgroup_multiply_accumulate(c10, mb1, ma0, c10)
            simdgroup_multiply_accumulate(c11, mb1, ma1, c11)
            simdgroup_multiply_accumulate(c12, mb1, ma2, c12)
            simdgroup_multiply_accumulate(c13, mb1, ma3, c13)
            simdgroup_load(ma0, ta, aB + 1536u, 8u)
            simdgroup_load(ma1, ta, aB + 1600u, 8u)
            simdgroup_load(ma2, ta, aB + 1664u, 8u)
            simdgroup_load(ma3, ta, aB + 1728u, 8u)
            simdgroup_load(mb0, tb, bB + 768u, 8u)
            simdgroup_load(mb1, tb, bB + 832u, 8u)
            simdgroup_multiply_accumulate(c00, mb0, ma0, c00)
            simdgroup_multiply_accumulate(c01, mb0, ma1, c01)
            simdgroup_multiply_accumulate(c02, mb0, ma2, c02)
            simdgroup_multiply_accumulate(c03, mb0, ma3, c03)
            simdgroup_multiply_accumulate(c10, mb1, ma0, c10)
            simdgroup_multiply_accumulate(c11, mb1, ma1, c11)
            simdgroup_multiply_accumulate(c12, mb1, ma2, c12)
            simdgroup_multiply_accumulate(c13, mb1, ma3, c13)
            kb++
        }
        let row0 = mBase + (sg / 2u) * 16u  // accumulator rows = tokens
        let col0 = nBase + (sg % 2u) * 32u  // accumulator cols = outputs
        simdgroup_store(c00, y, row0 * ndim + col0, ndim)
        simdgroup_store(c01, y, row0 * ndim + (col0 + 8u), ndim)
        simdgroup_store(c02, y, row0 * ndim + (col0 + 16u), ndim)
        simdgroup_store(c03, y, row0 * ndim + (col0 + 24u), ndim)
        simdgroup_store(c10, y, (row0 + 8u) * ndim + col0, ndim)
        simdgroup_store(c11, y, (row0 + 8u) * ndim + (col0 + 8u), ndim)
        simdgroup_store(c12, y, (row0 + 8u) * ndim + (col0 + 16u), ndim)
        simdgroup_store(c13, y, (row0 + 8u) * ndim + (col0 + 24u), ndim)
    }
}

class MetalGemmLabBlob36 {
    @ssbo @binding = 0 wsc : array<float>   // 36B-block W blob (f32 scale + 32 int8), SCALE view: scale of block b at 9*b
    @ssbo @binding = 1 wqv : array<byte4>   // the SAME blob buffer, QUANT view: block b quants at 9*b + 1 .. 9*b + 8
    @ssbo @binding = 2 xf : array<float4>   // PRE-DEQUANTIZED f32 activations — the lcpp B contract
    @ssbo @binding = 3 xs : array<float>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 kdim : uint
    @uniform @binding = 6 ndim : uint
    @workgroup ta : float16[2048]           // W: 4 k-tiles x 8 row-tiles of 8x8, each tile (k row, wrow col)
    @workgroup tb : float16[1024]           // X: 4 k-tiles x 4 token-tiles of 8x8, each tile (token row, k col)

    [metal_kernel(name="lab_blob36_msl")]
    def lab_blob36 {
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let mBase = gl_WorkGroupID.x * 32u  // token rows — FAST axis: concurrent tgs share the W panel (ggml rasterization)
        let nBase = gl_WorkGroupID.y * 64u  // output (weight) rows
        var c00 : simdgroup_float8x8
        var c01 : simdgroup_float8x8
        var c02 : simdgroup_float8x8
        var c03 : simdgroup_float8x8
        var c10 : simdgroup_float8x8
        var c11 : simdgroup_float8x8
        var c12 : simdgroup_float8x8
        var c13 : simdgroup_float8x8
        let kdim4 = kdim / 4u
        let nkb = kdim / 32u
        // A split: each thread stages 16 weight elems — row lr0, k half il0
        let lr0 = lid / 2u
        let il0 = lid % 2u
        let syA = lr0 / 8u
        let lxA = lr0 % 8u
        let tA0 = (il0 * 2u * 8u + syA) * 64u + lxA         // k-tiles il0*2 (lanes at stride 8)
        let tA1 = ((il0 * 2u + 1u) * 8u + syA) * 64u + lxA  // k-tile il0*2+1
        // B split: each thread stages 8 activation elems — token lr1, k offset iy
        let lr1 = lid / 4u
        let iy = (lid % 4u) * 8u
        let ibB = ((lid % 4u) * 4u + lr1 / 8u) * 64u + (lr1 % 8u) * 8u
        let aB = (sg % 2u) * 256u           // this simdgroup's A quadrant: 4 row-tiles of 64
        let bB = (sg / 2u) * 128u           // this simdgroup's B quadrant: 2 token-tiles of 64
        var ma0 : simdgroup_half8x8
        var ma1 : simdgroup_half8x8
        var ma2 : simdgroup_half8x8
        var ma3 : simdgroup_half8x8
        var mb0 : simdgroup_half8x8
        var mb1 : simdgroup_half8x8
        var kb = 0u
        while (kb < nkb) {
            // device reads first — they fly while the previous math loop drains
            let b9 = ((nBase + lr0) * nkb + kb) * 9u
            let sA = wsc[b9]                // scale rides the quants' cacheline — the blob's point
            let srcA = b9 + 1u + il0 * 4u
            let a0 = wqv[srcA]
            let a1 = wqv[srcA + 1u]
            let a2 = wqv[srcA + 2u]
            let a3 = wqv[srcA + 3u]
            let srcB = (mBase + lr1) * kdim4 + (kb * 32u + iy) / 4u
            let b0 = xf[srcB]
            let b1 = xf[srcB + 1u]
            barrier()                       // previous math done reading the tiles
            let va0 = float4(int4(a0)) * sA
            let va1 = float4(int4(a1)) * sA
            let va2 = float4(int4(a2)) * sA
            let va3 = float4(int4(a3)) * sA
            ta[tA0] = float16(va0.x)        // ly = k%8 -> +8 per lane; +32 = second 4-k half
            ta[tA0 + 8u] = float16(va0.y)
            ta[tA0 + 16u] = float16(va0.z)
            ta[tA0 + 24u] = float16(va0.w)
            ta[tA0 + 32u] = float16(va1.x)
            ta[tA0 + 40u] = float16(va1.y)
            ta[tA0 + 48u] = float16(va1.z)
            ta[tA0 + 56u] = float16(va1.w)
            ta[tA1] = float16(va2.x)
            ta[tA1 + 8u] = float16(va2.y)
            ta[tA1 + 16u] = float16(va2.z)
            ta[tA1 + 24u] = float16(va2.w)
            ta[tA1 + 32u] = float16(va3.x)
            ta[tA1 + 40u] = float16(va3.y)
            ta[tA1 + 48u] = float16(va3.z)
            ta[tA1 + 56u] = float16(va3.w)
            tg_store_half4(tb, ibB, b0)     // no dequant on B — pure convert-and-store
            tg_store_half4(tb, ibB + 4u, b1)
            barrier()
            // math fully unrolled (their FOR_UNROLL): 4 k-slabs x (6 tile loads + 8 macs),
            // constant tile offsets off hoisted quadrant bases
            simdgroup_load(ma0, ta, aB, 8u)
            simdgroup_load(ma1, ta, aB + 64u, 8u)
            simdgroup_load(ma2, ta, aB + 128u, 8u)
            simdgroup_load(ma3, ta, aB + 192u, 8u)
            simdgroup_load(mb0, tb, bB, 8u)
            simdgroup_load(mb1, tb, bB + 64u, 8u)
            simdgroup_multiply_accumulate(c00, mb0, ma0, c00)
            simdgroup_multiply_accumulate(c01, mb0, ma1, c01)
            simdgroup_multiply_accumulate(c02, mb0, ma2, c02)
            simdgroup_multiply_accumulate(c03, mb0, ma3, c03)
            simdgroup_multiply_accumulate(c10, mb1, ma0, c10)
            simdgroup_multiply_accumulate(c11, mb1, ma1, c11)
            simdgroup_multiply_accumulate(c12, mb1, ma2, c12)
            simdgroup_multiply_accumulate(c13, mb1, ma3, c13)
            simdgroup_load(ma0, ta, aB + 512u, 8u)
            simdgroup_load(ma1, ta, aB + 576u, 8u)
            simdgroup_load(ma2, ta, aB + 640u, 8u)
            simdgroup_load(ma3, ta, aB + 704u, 8u)
            simdgroup_load(mb0, tb, bB + 256u, 8u)
            simdgroup_load(mb1, tb, bB + 320u, 8u)
            simdgroup_multiply_accumulate(c00, mb0, ma0, c00)
            simdgroup_multiply_accumulate(c01, mb0, ma1, c01)
            simdgroup_multiply_accumulate(c02, mb0, ma2, c02)
            simdgroup_multiply_accumulate(c03, mb0, ma3, c03)
            simdgroup_multiply_accumulate(c10, mb1, ma0, c10)
            simdgroup_multiply_accumulate(c11, mb1, ma1, c11)
            simdgroup_multiply_accumulate(c12, mb1, ma2, c12)
            simdgroup_multiply_accumulate(c13, mb1, ma3, c13)
            simdgroup_load(ma0, ta, aB + 1024u, 8u)
            simdgroup_load(ma1, ta, aB + 1088u, 8u)
            simdgroup_load(ma2, ta, aB + 1152u, 8u)
            simdgroup_load(ma3, ta, aB + 1216u, 8u)
            simdgroup_load(mb0, tb, bB + 512u, 8u)
            simdgroup_load(mb1, tb, bB + 576u, 8u)
            simdgroup_multiply_accumulate(c00, mb0, ma0, c00)
            simdgroup_multiply_accumulate(c01, mb0, ma1, c01)
            simdgroup_multiply_accumulate(c02, mb0, ma2, c02)
            simdgroup_multiply_accumulate(c03, mb0, ma3, c03)
            simdgroup_multiply_accumulate(c10, mb1, ma0, c10)
            simdgroup_multiply_accumulate(c11, mb1, ma1, c11)
            simdgroup_multiply_accumulate(c12, mb1, ma2, c12)
            simdgroup_multiply_accumulate(c13, mb1, ma3, c13)
            simdgroup_load(ma0, ta, aB + 1536u, 8u)
            simdgroup_load(ma1, ta, aB + 1600u, 8u)
            simdgroup_load(ma2, ta, aB + 1664u, 8u)
            simdgroup_load(ma3, ta, aB + 1728u, 8u)
            simdgroup_load(mb0, tb, bB + 768u, 8u)
            simdgroup_load(mb1, tb, bB + 832u, 8u)
            simdgroup_multiply_accumulate(c00, mb0, ma0, c00)
            simdgroup_multiply_accumulate(c01, mb0, ma1, c01)
            simdgroup_multiply_accumulate(c02, mb0, ma2, c02)
            simdgroup_multiply_accumulate(c03, mb0, ma3, c03)
            simdgroup_multiply_accumulate(c10, mb1, ma0, c10)
            simdgroup_multiply_accumulate(c11, mb1, ma1, c11)
            simdgroup_multiply_accumulate(c12, mb1, ma2, c12)
            simdgroup_multiply_accumulate(c13, mb1, ma3, c13)
            kb++
        }
        let row0 = mBase + (sg / 2u) * 16u  // accumulator rows = tokens
        let col0 = nBase + (sg % 2u) * 32u  // accumulator cols = outputs
        simdgroup_store(c00, y, row0 * ndim + col0, ndim)
        simdgroup_store(c01, y, row0 * ndim + (col0 + 8u), ndim)
        simdgroup_store(c02, y, row0 * ndim + (col0 + 16u), ndim)
        simdgroup_store(c03, y, row0 * ndim + (col0 + 24u), ndim)
        simdgroup_store(c10, y, (row0 + 8u) * ndim + col0, ndim)
        simdgroup_store(c11, y, (row0 + 8u) * ndim + (col0 + 8u), ndim)
        simdgroup_store(c12, y, (row0 + 8u) * ndim + (col0 + 16u), ndim)
        simdgroup_store(c13, y, (row0 + 8u) * ndim + (col0 + 24u), ndim)
    }
}

// v23: v22's geometry on llama.cpp's OWN 34B blob (half scale + 32 int8, quants 2-byte-offset)
// — their exact W contract and byte-wise load/dequant pattern, das-authored. Isolates the
// residual data-contract share of the verbatim gap: half scale = 5.6% less W traffic, and the
// +2 quant offset forces scalar byte loads (the reason ggml reads bytes, not vectors).
class MetalGemmLabBlob34 {
    @ssbo @binding = 0 wsh : array<float16> // 34B-block W blob, HALF-SCALE view: scale of block b at 17*b
    @ssbo @binding = 1 wqb : array<int8>    // the SAME blob buffer, BYTE view: quants of block b at 34*b + 2
    @ssbo @binding = 2 xf : array<float4>   // PRE-DEQUANTIZED f32 activations — the lcpp B contract
    @ssbo @binding = 3 xs : array<float>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 kdim : uint
    @uniform @binding = 6 ndim : uint
    @workgroup ta : float16[2048]           // W: 4 k-tiles x 8 row-tiles of 8x8, each tile (k row, wrow col)
    @workgroup tb : float16[1024]           // X: 4 k-tiles x 4 token-tiles of 8x8, each tile (token row, k col)

    [metal_kernel(name="lab_blob34_msl")]
    def lab_blob34 {
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let mBase = gl_WorkGroupID.x * 32u  // token rows — FAST axis: concurrent tgs share the W panel (ggml rasterization)
        let nBase = gl_WorkGroupID.y * 64u  // output (weight) rows
        var c00 : simdgroup_float8x8
        var c01 : simdgroup_float8x8
        var c02 : simdgroup_float8x8
        var c03 : simdgroup_float8x8
        var c10 : simdgroup_float8x8
        var c11 : simdgroup_float8x8
        var c12 : simdgroup_float8x8
        var c13 : simdgroup_float8x8
        let kdim4 = kdim / 4u
        let nkb = kdim / 32u
        // A split: each thread stages 16 weight elems — row lr0, k half il0
        let lr0 = lid / 2u
        let il0 = lid % 2u
        let syA = lr0 / 8u
        let lxA = lr0 % 8u
        let tA0 = (il0 * 2u * 8u + syA) * 64u + lxA         // k-tiles il0*2 (lanes at stride 8)
        let tA1 = ((il0 * 2u + 1u) * 8u + syA) * 64u + lxA  // k-tile il0*2+1
        // B split: each thread stages 8 activation elems — token lr1, k offset iy
        let lr1 = lid / 4u
        let iy = (lid % 4u) * 8u
        let ibB = ((lid % 4u) * 4u + lr1 / 8u) * 64u + (lr1 % 8u) * 8u
        let aB = (sg % 2u) * 256u           // this simdgroup's A quadrant: 4 row-tiles of 64
        let bB = (sg / 2u) * 128u           // this simdgroup's B quadrant: 2 token-tiles of 64
        var ma0 : simdgroup_half8x8
        var ma1 : simdgroup_half8x8
        var ma2 : simdgroup_half8x8
        var ma3 : simdgroup_half8x8
        var mb0 : simdgroup_half8x8
        var mb1 : simdgroup_half8x8
        var kb = 0u
        while (kb < nkb) {
            // device reads first — they fly while the previous math loop drains
            let blk = (nBase + lr0) * nkb + kb
            let sA = float(wsh[blk * 17u])   // half scale rides the quants' cacheline
            let qb = blk * 34u + 2u + il0 * 16u
            let q0 = wqb[qb]
            let q1 = wqb[qb + 1u]
            let q2 = wqb[qb + 2u]
            let q3 = wqb[qb + 3u]
            let q4 = wqb[qb + 4u]
            let q5 = wqb[qb + 5u]
            let q6 = wqb[qb + 6u]
            let q7 = wqb[qb + 7u]
            let q8 = wqb[qb + 8u]
            let q9 = wqb[qb + 9u]
            let q10 = wqb[qb + 10u]
            let q11 = wqb[qb + 11u]
            let q12 = wqb[qb + 12u]
            let q13 = wqb[qb + 13u]
            let q14 = wqb[qb + 14u]
            let q15 = wqb[qb + 15u]
            let srcB = (mBase + lr1) * kdim4 + (kb * 32u + iy) / 4u
            let b0 = xf[srcB]
            let b1 = xf[srcB + 1u]
            barrier()                       // previous math done reading the tiles
            ta[tA0] = float16(float(q0) * sA)   // ly = k%8 -> +8 per lane; tA1 = second 8-k tile
            ta[tA0 + 8u] = float16(float(q1) * sA)
            ta[tA0 + 16u] = float16(float(q2) * sA)
            ta[tA0 + 24u] = float16(float(q3) * sA)
            ta[tA0 + 32u] = float16(float(q4) * sA)
            ta[tA0 + 40u] = float16(float(q5) * sA)
            ta[tA0 + 48u] = float16(float(q6) * sA)
            ta[tA0 + 56u] = float16(float(q7) * sA)
            ta[tA1] = float16(float(q8) * sA)
            ta[tA1 + 8u] = float16(float(q9) * sA)
            ta[tA1 + 16u] = float16(float(q10) * sA)
            ta[tA1 + 24u] = float16(float(q11) * sA)
            ta[tA1 + 32u] = float16(float(q12) * sA)
            ta[tA1 + 40u] = float16(float(q13) * sA)
            ta[tA1 + 48u] = float16(float(q14) * sA)
            ta[tA1 + 56u] = float16(float(q15) * sA)
            tg_store_half4(tb, ibB, b0)     // no dequant on B — pure convert-and-store
            tg_store_half4(tb, ibB + 4u, b1)
            barrier()
            // math fully unrolled (their FOR_UNROLL): 4 k-slabs x (6 tile loads + 8 macs),
            // constant tile offsets off hoisted quadrant bases
            simdgroup_load(ma0, ta, aB, 8u)
            simdgroup_load(ma1, ta, aB + 64u, 8u)
            simdgroup_load(ma2, ta, aB + 128u, 8u)
            simdgroup_load(ma3, ta, aB + 192u, 8u)
            simdgroup_load(mb0, tb, bB, 8u)
            simdgroup_load(mb1, tb, bB + 64u, 8u)
            simdgroup_multiply_accumulate(c00, mb0, ma0, c00)
            simdgroup_multiply_accumulate(c01, mb0, ma1, c01)
            simdgroup_multiply_accumulate(c02, mb0, ma2, c02)
            simdgroup_multiply_accumulate(c03, mb0, ma3, c03)
            simdgroup_multiply_accumulate(c10, mb1, ma0, c10)
            simdgroup_multiply_accumulate(c11, mb1, ma1, c11)
            simdgroup_multiply_accumulate(c12, mb1, ma2, c12)
            simdgroup_multiply_accumulate(c13, mb1, ma3, c13)
            simdgroup_load(ma0, ta, aB + 512u, 8u)
            simdgroup_load(ma1, ta, aB + 576u, 8u)
            simdgroup_load(ma2, ta, aB + 640u, 8u)
            simdgroup_load(ma3, ta, aB + 704u, 8u)
            simdgroup_load(mb0, tb, bB + 256u, 8u)
            simdgroup_load(mb1, tb, bB + 320u, 8u)
            simdgroup_multiply_accumulate(c00, mb0, ma0, c00)
            simdgroup_multiply_accumulate(c01, mb0, ma1, c01)
            simdgroup_multiply_accumulate(c02, mb0, ma2, c02)
            simdgroup_multiply_accumulate(c03, mb0, ma3, c03)
            simdgroup_multiply_accumulate(c10, mb1, ma0, c10)
            simdgroup_multiply_accumulate(c11, mb1, ma1, c11)
            simdgroup_multiply_accumulate(c12, mb1, ma2, c12)
            simdgroup_multiply_accumulate(c13, mb1, ma3, c13)
            simdgroup_load(ma0, ta, aB + 1024u, 8u)
            simdgroup_load(ma1, ta, aB + 1088u, 8u)
            simdgroup_load(ma2, ta, aB + 1152u, 8u)
            simdgroup_load(ma3, ta, aB + 1216u, 8u)
            simdgroup_load(mb0, tb, bB + 512u, 8u)
            simdgroup_load(mb1, tb, bB + 576u, 8u)
            simdgroup_multiply_accumulate(c00, mb0, ma0, c00)
            simdgroup_multiply_accumulate(c01, mb0, ma1, c01)
            simdgroup_multiply_accumulate(c02, mb0, ma2, c02)
            simdgroup_multiply_accumulate(c03, mb0, ma3, c03)
            simdgroup_multiply_accumulate(c10, mb1, ma0, c10)
            simdgroup_multiply_accumulate(c11, mb1, ma1, c11)
            simdgroup_multiply_accumulate(c12, mb1, ma2, c12)
            simdgroup_multiply_accumulate(c13, mb1, ma3, c13)
            simdgroup_load(ma0, ta, aB + 1536u, 8u)
            simdgroup_load(ma1, ta, aB + 1600u, 8u)
            simdgroup_load(ma2, ta, aB + 1664u, 8u)
            simdgroup_load(ma3, ta, aB + 1728u, 8u)
            simdgroup_load(mb0, tb, bB + 768u, 8u)
            simdgroup_load(mb1, tb, bB + 832u, 8u)
            simdgroup_multiply_accumulate(c00, mb0, ma0, c00)
            simdgroup_multiply_accumulate(c01, mb0, ma1, c01)
            simdgroup_multiply_accumulate(c02, mb0, ma2, c02)
            simdgroup_multiply_accumulate(c03, mb0, ma3, c03)
            simdgroup_multiply_accumulate(c10, mb1, ma0, c10)
            simdgroup_multiply_accumulate(c11, mb1, ma1, c11)
            simdgroup_multiply_accumulate(c12, mb1, ma2, c12)
            simdgroup_multiply_accumulate(c13, mb1, ma3, c13)
            kb++
        }
        let row0 = mBase + (sg / 2u) * 16u  // accumulator rows = tokens
        let col0 = nBase + (sg % 2u) * 32u  // accumulator cols = outputs
        simdgroup_store(c00, y, row0 * ndim + col0, ndim)
        simdgroup_store(c01, y, row0 * ndim + (col0 + 8u), ndim)
        simdgroup_store(c02, y, row0 * ndim + (col0 + 16u), ndim)
        simdgroup_store(c03, y, row0 * ndim + (col0 + 24u), ndim)
        simdgroup_store(c10, y, (row0 + 8u) * ndim + col0, ndim)
        simdgroup_store(c11, y, (row0 + 8u) * ndim + (col0 + 8u), ndim)
        simdgroup_store(c12, y, (row0 + 8u) * ndim + (col0 + 16u), ndim)
        simdgroup_store(c13, y, (row0 + 8u) * ndim + (col0 + 24u), ndim)
    }
}

// v25: the v24g shape das-AUTHORED — simdgroup-matrix arrays + unroll_range rolled loops
// (the Phase-7 emitter constructs). Same 34B-blob contract as v24g; the emitted MSL is the
// rolled-with-pragma form, so the GPU backend unrolls with register-budget awareness
// (max_threads 1024, vs 832 for the hand-unrolled v22 statements).
class MetalGemmLabRolled34 {
    @ssbo @binding = 0 wsh : array<float16> // 34B-block W blob, HALF-SCALE view: scale of block b at 17*b
    @ssbo @binding = 1 wqb : array<int8>    // the SAME blob buffer, BYTE view: quants of block b at 34*b + 2
    @ssbo @binding = 2 xf : array<float4>   // PRE-DEQUANTIZED f32 activations — the lcpp B contract
    @ssbo @binding = 3 xs : array<float>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 kdim : uint
    @uniform @binding = 6 ndim : uint
    @workgroup ta : float16[2048]           // W: 4 k-tiles x 8 row-tiles of 8x8, each tile (k row, wrow col)
    @workgroup tb : float16[1024]           // X: 4 k-tiles x 4 token-tiles of 8x8, each tile (token row, k col)

    [metal_kernel(name="lab_rolled34_msl")]
    def lab_rolled34 {
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let mBase = gl_WorkGroupID.x * 32u  // token rows — FAST axis: concurrent tgs share the W panel (ggml rasterization)
        let nBase = gl_WorkGroupID.y * 64u  // output (weight) rows
        var mc : simdgroup_float8x8[8]      // per-simdgroup C quadrant: 2 token-tiles x 4 output-tiles
        var ma : simdgroup_half8x8[4]
        var mb : simdgroup_half8x8[2]
        var va : float16[16]                // dequantized A staging regs (this thread's block half)
        let kdim4 = kdim / 4u
        let nkb = kdim / 32u
        // A split: each thread stages 16 weight elems — row lr0, k half il0
        let lr0 = lid / 2u
        let il0 = lid % 2u
        let syA = lr0 / 8u
        let lxA = lr0 % 8u
        let tA0 = (il0 * 2u * 8u + syA) * 64u + lxA         // k-tile il0*2 (lanes at stride 8; +512 = next k-tile)
        // B split: each thread stages 8 activation elems — token lr1, k offset iy
        let lr1 = lid / 4u
        let iy = (lid % 4u) * 8u
        let ibB = ((lid % 4u) * 4u + lr1 / 8u) * 64u + (lr1 % 8u) * 8u
        let aB = (sg % 2u) * 256u           // this simdgroup's A quadrant: 4 row-tiles of 64
        let bB = (sg / 2u) * 128u           // this simdgroup's B quadrant: 2 token-tiles of 64
        var kb = 0u
        while (kb < nkb) {
            // device reads + dequant first — they fly while the previous math loop drains
            let blk = (nBase + lr0) * nkb + kb
            let sA = float(wsh[blk * 17u])  // half scale rides the quants' cacheline
            let qp = dev_cursor(wqb, blk * 34u + 2u + il0 * 16u)
            let srcB = (mBase + lr1) * kdim4 + (kb * 32u + iy) / 4u
            let b0 = xf[srcB]
            let b1 = xf[srcB + 1u]
            for (i in unroll_range(16)) {
                va[i] = float16(float(wqb[qp + uint(i)]) * sA)
            }
            barrier()                       // previous math done reading the tiles
            for (i in unroll_range(16)) {
                ta[tA0 + uint(i / 8) * 512u + uint(i % 8) * 8u] = va[i]
            }
            tg_store_half4(tb, ibB, b0)     // no dequant on B — pure convert-and-store
            tg_store_half4(tb, ibB + 4u, b1)
            barrier()
            // math ROLLED: the pragma'd loops reach the backend intact — 4 k-slabs x
            // (6 tile loads + 8 macs). Tile addresses advance via loop-carried tg_cursor
            // POINTERS, not aB + ik*512 index math — the pointer bump denies LLVM the
            // freedom to precompute every tile address into a long-lived register (index
            // form measured max_threads 896 and -12%; the carried pointer keeps 1024)
            var acur = tg_cursor(ta, aB)
            var bcur = tg_cursor(tb, bB)
            for (_ik in unroll_range(4)) {
                for (i in unroll_range(4)) {
                    simdgroup_load(ma[i], ta, acur + uint(i) * 64u, 8u)
                }
                for (i in unroll_range(2)) {
                    simdgroup_load(mb[i], tb, bcur + uint(i) * 64u, 8u)
                }
                for (i in unroll_range(8)) {
                    simdgroup_multiply_accumulate(mc[i], mb[i / 4], ma[i % 4], mc[i])
                }
                acur += 512u
                bcur += 256u
            }
            kb++
        }
        let row0 = mBase + (sg / 2u) * 16u  // accumulator rows = tokens
        let col0 = nBase + (sg % 2u) * 32u  // accumulator cols = outputs
        for (i in unroll_range(8)) {
            simdgroup_store(mc[i], y, (row0 + uint(i / 4) * 8u) * ndim + col0 + uint(i % 4) * 8u, ndim)
        }
    }
}

// knockouts: v21 staging bisect — A half only / B half only (timing-only)
class MetalGemmLabGgmlGeoAOnly {
    @ssbo @binding = 0 wq : array<byte4>
    @ssbo @binding = 1 ws : array<float>
    @ssbo @binding = 2 xq : array<byte4>
    @ssbo @binding = 3 xs : array<float>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 kdim : uint
    @uniform @binding = 6 ndim : uint
    @workgroup ta : float16[2048]           // W: 4 k-tiles x 8 row-tiles of 8x8, each tile (k row, wrow col)
    @workgroup tb : float16[1024]           // X: 4 k-tiles x 4 token-tiles of 8x8, each tile (token row, k col)

    [metal_kernel(name="lab_ggmlgeoa_msl")]
    def lab_ggmlgeoa {
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let mBase = gl_WorkGroupID.x * 32u  // token rows — FAST axis: concurrent tgs share the W panel (ggml rasterization)
        let nBase = gl_WorkGroupID.y * 64u  // output (weight) rows
        var c00 : simdgroup_float8x8
        var c01 : simdgroup_float8x8
        var c02 : simdgroup_float8x8
        var c03 : simdgroup_float8x8
        var c10 : simdgroup_float8x8
        var c11 : simdgroup_float8x8
        var c12 : simdgroup_float8x8
        var c13 : simdgroup_float8x8
        let kdim4 = kdim / 4u
        let nkb = kdim / 32u
        // A split: each thread stages 16 weight elems — row lr0, k half il0
        let lr0 = lid / 2u
        let il0 = lid % 2u
        let syA = lr0 / 8u
        let lxA = lr0 % 8u
        let tA0 = (il0 * 2u * 8u + syA) * 64u + lxA         // k-tiles il0*2 (lanes at stride 8)
        let tA1 = ((il0 * 2u + 1u) * 8u + syA) * 64u + lxA  // k-tile il0*2+1
        // B split: each thread stages 8 activation elems — token lr1, k offset iy
        let aB = (sg % 2u) * 256u           // this simdgroup's A quadrant: 4 row-tiles of 64
        let bB = (sg / 2u) * 128u           // this simdgroup's B quadrant: 2 token-tiles of 64
        var ma0 : simdgroup_half8x8
        var ma1 : simdgroup_half8x8
        var ma2 : simdgroup_half8x8
        var ma3 : simdgroup_half8x8
        var mb0 : simdgroup_half8x8
        var mb1 : simdgroup_half8x8
        var kb = 0u
        while (kb < nkb) {
            // device reads first — they fly while the previous math loop drains
            let sA = ws[(nBase + lr0) * nkb + kb]
            let srcA = (nBase + lr0) * kdim4 + (kb * 32u + il0 * 16u) / 4u
            let a0 = wq[srcA]
            let a1 = wq[srcA + 1u]
            let a2 = wq[srcA + 2u]
            let a3 = wq[srcA + 3u]
            barrier()                       // previous math done reading the tiles
            let va0 = float4(int4(a0)) * sA
            let va1 = float4(int4(a1)) * sA
            let va2 = float4(int4(a2)) * sA
            let va3 = float4(int4(a3)) * sA
            ta[tA0] = float16(va0.x)        // ly = k%8 -> +8 per lane; +32 = second 4-k half
            ta[tA0 + 8u] = float16(va0.y)
            ta[tA0 + 16u] = float16(va0.z)
            ta[tA0 + 24u] = float16(va0.w)
            ta[tA0 + 32u] = float16(va1.x)
            ta[tA0 + 40u] = float16(va1.y)
            ta[tA0 + 48u] = float16(va1.z)
            ta[tA0 + 56u] = float16(va1.w)
            ta[tA1] = float16(va2.x)
            ta[tA1 + 8u] = float16(va2.y)
            ta[tA1 + 16u] = float16(va2.z)
            ta[tA1 + 24u] = float16(va2.w)
            ta[tA1 + 32u] = float16(va3.x)
            ta[tA1 + 40u] = float16(va3.y)
            ta[tA1 + 48u] = float16(va3.z)
            ta[tA1 + 56u] = float16(va3.w)
            barrier()
            // math fully unrolled (their FOR_UNROLL): 4 k-slabs x (6 tile loads + 8 macs),
            // constant tile offsets off hoisted quadrant bases
            simdgroup_load(ma0, ta, aB, 8u)
            simdgroup_load(ma1, ta, aB + 64u, 8u)
            simdgroup_load(ma2, ta, aB + 128u, 8u)
            simdgroup_load(ma3, ta, aB + 192u, 8u)
            simdgroup_load(mb0, tb, bB, 8u)
            simdgroup_load(mb1, tb, bB + 64u, 8u)
            simdgroup_multiply_accumulate(c00, mb0, ma0, c00)
            simdgroup_multiply_accumulate(c01, mb0, ma1, c01)
            simdgroup_multiply_accumulate(c02, mb0, ma2, c02)
            simdgroup_multiply_accumulate(c03, mb0, ma3, c03)
            simdgroup_multiply_accumulate(c10, mb1, ma0, c10)
            simdgroup_multiply_accumulate(c11, mb1, ma1, c11)
            simdgroup_multiply_accumulate(c12, mb1, ma2, c12)
            simdgroup_multiply_accumulate(c13, mb1, ma3, c13)
            simdgroup_load(ma0, ta, aB + 512u, 8u)
            simdgroup_load(ma1, ta, aB + 576u, 8u)
            simdgroup_load(ma2, ta, aB + 640u, 8u)
            simdgroup_load(ma3, ta, aB + 704u, 8u)
            simdgroup_load(mb0, tb, bB + 256u, 8u)
            simdgroup_load(mb1, tb, bB + 320u, 8u)
            simdgroup_multiply_accumulate(c00, mb0, ma0, c00)
            simdgroup_multiply_accumulate(c01, mb0, ma1, c01)
            simdgroup_multiply_accumulate(c02, mb0, ma2, c02)
            simdgroup_multiply_accumulate(c03, mb0, ma3, c03)
            simdgroup_multiply_accumulate(c10, mb1, ma0, c10)
            simdgroup_multiply_accumulate(c11, mb1, ma1, c11)
            simdgroup_multiply_accumulate(c12, mb1, ma2, c12)
            simdgroup_multiply_accumulate(c13, mb1, ma3, c13)
            simdgroup_load(ma0, ta, aB + 1024u, 8u)
            simdgroup_load(ma1, ta, aB + 1088u, 8u)
            simdgroup_load(ma2, ta, aB + 1152u, 8u)
            simdgroup_load(ma3, ta, aB + 1216u, 8u)
            simdgroup_load(mb0, tb, bB + 512u, 8u)
            simdgroup_load(mb1, tb, bB + 576u, 8u)
            simdgroup_multiply_accumulate(c00, mb0, ma0, c00)
            simdgroup_multiply_accumulate(c01, mb0, ma1, c01)
            simdgroup_multiply_accumulate(c02, mb0, ma2, c02)
            simdgroup_multiply_accumulate(c03, mb0, ma3, c03)
            simdgroup_multiply_accumulate(c10, mb1, ma0, c10)
            simdgroup_multiply_accumulate(c11, mb1, ma1, c11)
            simdgroup_multiply_accumulate(c12, mb1, ma2, c12)
            simdgroup_multiply_accumulate(c13, mb1, ma3, c13)
            simdgroup_load(ma0, ta, aB + 1536u, 8u)
            simdgroup_load(ma1, ta, aB + 1600u, 8u)
            simdgroup_load(ma2, ta, aB + 1664u, 8u)
            simdgroup_load(ma3, ta, aB + 1728u, 8u)
            simdgroup_load(mb0, tb, bB + 768u, 8u)
            simdgroup_load(mb1, tb, bB + 832u, 8u)
            simdgroup_multiply_accumulate(c00, mb0, ma0, c00)
            simdgroup_multiply_accumulate(c01, mb0, ma1, c01)
            simdgroup_multiply_accumulate(c02, mb0, ma2, c02)
            simdgroup_multiply_accumulate(c03, mb0, ma3, c03)
            simdgroup_multiply_accumulate(c10, mb1, ma0, c10)
            simdgroup_multiply_accumulate(c11, mb1, ma1, c11)
            simdgroup_multiply_accumulate(c12, mb1, ma2, c12)
            simdgroup_multiply_accumulate(c13, mb1, ma3, c13)
            kb++
        }
        let row0 = mBase + (sg / 2u) * 16u  // accumulator rows = tokens
        let col0 = nBase + (sg % 2u) * 32u  // accumulator cols = outputs
        simdgroup_store(c00, y, row0 * ndim + col0, ndim)
        simdgroup_store(c01, y, row0 * ndim + (col0 + 8u), ndim)
        simdgroup_store(c02, y, row0 * ndim + (col0 + 16u), ndim)
        simdgroup_store(c03, y, row0 * ndim + (col0 + 24u), ndim)
        simdgroup_store(c10, y, (row0 + 8u) * ndim + col0, ndim)
        simdgroup_store(c11, y, (row0 + 8u) * ndim + (col0 + 8u), ndim)
        simdgroup_store(c12, y, (row0 + 8u) * ndim + (col0 + 16u), ndim)
        simdgroup_store(c13, y, (row0 + 8u) * ndim + (col0 + 24u), ndim)
    }
}

class MetalGemmLabGgmlGeoBOnly {
    @ssbo @binding = 0 wq : array<byte4>
    @ssbo @binding = 1 ws : array<float>
    @ssbo @binding = 2 xq : array<byte4>
    @ssbo @binding = 3 xs : array<float>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 kdim : uint
    @uniform @binding = 6 ndim : uint
    @workgroup ta : float16[2048]           // W: 4 k-tiles x 8 row-tiles of 8x8, each tile (k row, wrow col)
    @workgroup tb : float16[1024]           // X: 4 k-tiles x 4 token-tiles of 8x8, each tile (token row, k col)

    [metal_kernel(name="lab_ggmlgeob_msl")]
    def lab_ggmlgeob {
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let mBase = gl_WorkGroupID.x * 32u  // token rows — FAST axis: concurrent tgs share the W panel (ggml rasterization)
        let nBase = gl_WorkGroupID.y * 64u  // output (weight) rows
        var c00 : simdgroup_float8x8
        var c01 : simdgroup_float8x8
        var c02 : simdgroup_float8x8
        var c03 : simdgroup_float8x8
        var c10 : simdgroup_float8x8
        var c11 : simdgroup_float8x8
        var c12 : simdgroup_float8x8
        var c13 : simdgroup_float8x8
        let kdim4 = kdim / 4u
        let nkb = kdim / 32u
        // A split: each thread stages 16 weight elems — row lr0, k half il0
        // B split: each thread stages 8 activation elems — token lr1, k offset iy
        let lr1 = lid / 4u
        let iy = (lid % 4u) * 8u
        let ibB = ((lid % 4u) * 4u + lr1 / 8u) * 64u + (lr1 % 8u) * 8u
        let aB = (sg % 2u) * 256u           // this simdgroup's A quadrant: 4 row-tiles of 64
        let bB = (sg / 2u) * 128u           // this simdgroup's B quadrant: 2 token-tiles of 64
        var ma0 : simdgroup_half8x8
        var ma1 : simdgroup_half8x8
        var ma2 : simdgroup_half8x8
        var ma3 : simdgroup_half8x8
        var mb0 : simdgroup_half8x8
        var mb1 : simdgroup_half8x8
        var kb = 0u
        while (kb < nkb) {
            // device reads first — they fly while the previous math loop drains
            let sB = xs[(mBase + lr1) * nkb + kb]
            let srcB = (mBase + lr1) * kdim4 + (kb * 32u + iy) / 4u
            let b0 = xq[srcB]
            let b1 = xq[srcB + 1u]
            barrier()                       // previous math done reading the tiles
            let vb0 = float4(int4(b0)) * sB
            let vb1 = float4(int4(b1)) * sB
            tg_store_half4(tb, ibB, vb0)    // k runs contiguous within the tile row — 2 vector stores
            tg_store_half4(tb, ibB + 4u, vb1)
            barrier()
            // math fully unrolled (their FOR_UNROLL): 4 k-slabs x (6 tile loads + 8 macs),
            // constant tile offsets off hoisted quadrant bases
            simdgroup_load(ma0, ta, aB, 8u)
            simdgroup_load(ma1, ta, aB + 64u, 8u)
            simdgroup_load(ma2, ta, aB + 128u, 8u)
            simdgroup_load(ma3, ta, aB + 192u, 8u)
            simdgroup_load(mb0, tb, bB, 8u)
            simdgroup_load(mb1, tb, bB + 64u, 8u)
            simdgroup_multiply_accumulate(c00, mb0, ma0, c00)
            simdgroup_multiply_accumulate(c01, mb0, ma1, c01)
            simdgroup_multiply_accumulate(c02, mb0, ma2, c02)
            simdgroup_multiply_accumulate(c03, mb0, ma3, c03)
            simdgroup_multiply_accumulate(c10, mb1, ma0, c10)
            simdgroup_multiply_accumulate(c11, mb1, ma1, c11)
            simdgroup_multiply_accumulate(c12, mb1, ma2, c12)
            simdgroup_multiply_accumulate(c13, mb1, ma3, c13)
            simdgroup_load(ma0, ta, aB + 512u, 8u)
            simdgroup_load(ma1, ta, aB + 576u, 8u)
            simdgroup_load(ma2, ta, aB + 640u, 8u)
            simdgroup_load(ma3, ta, aB + 704u, 8u)
            simdgroup_load(mb0, tb, bB + 256u, 8u)
            simdgroup_load(mb1, tb, bB + 320u, 8u)
            simdgroup_multiply_accumulate(c00, mb0, ma0, c00)
            simdgroup_multiply_accumulate(c01, mb0, ma1, c01)
            simdgroup_multiply_accumulate(c02, mb0, ma2, c02)
            simdgroup_multiply_accumulate(c03, mb0, ma3, c03)
            simdgroup_multiply_accumulate(c10, mb1, ma0, c10)
            simdgroup_multiply_accumulate(c11, mb1, ma1, c11)
            simdgroup_multiply_accumulate(c12, mb1, ma2, c12)
            simdgroup_multiply_accumulate(c13, mb1, ma3, c13)
            simdgroup_load(ma0, ta, aB + 1024u, 8u)
            simdgroup_load(ma1, ta, aB + 1088u, 8u)
            simdgroup_load(ma2, ta, aB + 1152u, 8u)
            simdgroup_load(ma3, ta, aB + 1216u, 8u)
            simdgroup_load(mb0, tb, bB + 512u, 8u)
            simdgroup_load(mb1, tb, bB + 576u, 8u)
            simdgroup_multiply_accumulate(c00, mb0, ma0, c00)
            simdgroup_multiply_accumulate(c01, mb0, ma1, c01)
            simdgroup_multiply_accumulate(c02, mb0, ma2, c02)
            simdgroup_multiply_accumulate(c03, mb0, ma3, c03)
            simdgroup_multiply_accumulate(c10, mb1, ma0, c10)
            simdgroup_multiply_accumulate(c11, mb1, ma1, c11)
            simdgroup_multiply_accumulate(c12, mb1, ma2, c12)
            simdgroup_multiply_accumulate(c13, mb1, ma3, c13)
            simdgroup_load(ma0, ta, aB + 1536u, 8u)
            simdgroup_load(ma1, ta, aB + 1600u, 8u)
            simdgroup_load(ma2, ta, aB + 1664u, 8u)
            simdgroup_load(ma3, ta, aB + 1728u, 8u)
            simdgroup_load(mb0, tb, bB + 768u, 8u)
            simdgroup_load(mb1, tb, bB + 832u, 8u)
            simdgroup_multiply_accumulate(c00, mb0, ma0, c00)
            simdgroup_multiply_accumulate(c01, mb0, ma1, c01)
            simdgroup_multiply_accumulate(c02, mb0, ma2, c02)
            simdgroup_multiply_accumulate(c03, mb0, ma3, c03)
            simdgroup_multiply_accumulate(c10, mb1, ma0, c10)
            simdgroup_multiply_accumulate(c11, mb1, ma1, c11)
            simdgroup_multiply_accumulate(c12, mb1, ma2, c12)
            simdgroup_multiply_accumulate(c13, mb1, ma3, c13)
            kb++
        }
        let row0 = mBase + (sg / 2u) * 16u  // accumulator rows = tokens
        let col0 = nBase + (sg % 2u) * 32u  // accumulator cols = outputs
        simdgroup_store(c00, y, row0 * ndim + col0, ndim)
        simdgroup_store(c01, y, row0 * ndim + (col0 + 8u), ndim)
        simdgroup_store(c02, y, row0 * ndim + (col0 + 16u), ndim)
        simdgroup_store(c03, y, row0 * ndim + (col0 + 24u), ndim)
        simdgroup_store(c10, y, (row0 + 8u) * ndim + col0, ndim)
        simdgroup_store(c11, y, (row0 + 8u) * ndim + (col0 + 8u), ndim)
        simdgroup_store(c12, y, (row0 + 8u) * ndim + (col0 + 16u), ndim)
        simdgroup_store(c13, y, (row0 + 8u) * ndim + (col0 + 24u), ndim)
    }
}

// knockout: v21 with the staging removed (math + barriers + stores only, tiles read
// uninitialized) — the ggml-geometry math ceiling; attributes v21's staging share
class MetalGemmLabGgmlGeoNoStage {
    @ssbo @binding = 0 wq : array<byte4>
    @ssbo @binding = 1 ws : array<float>
    @ssbo @binding = 2 xq : array<byte4>
    @ssbo @binding = 3 xs : array<float>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 kdim : uint
    @uniform @binding = 6 ndim : uint
    @workgroup ta : float16[2048]
    @workgroup tb : float16[1024]

    [metal_kernel(name="lab_ggmlgeons_msl")]
    def lab_ggmlgeons {
        let sg = gl_SubgroupID
        let mBase = gl_WorkGroupID.x * 32u
        let nBase = gl_WorkGroupID.y * 64u
        var c00 : simdgroup_float8x8
        var c01 : simdgroup_float8x8
        var c02 : simdgroup_float8x8
        var c03 : simdgroup_float8x8
        var c10 : simdgroup_float8x8
        var c11 : simdgroup_float8x8
        var c12 : simdgroup_float8x8
        var c13 : simdgroup_float8x8
        let nkb = kdim / 32u
        let aB = (sg % 2u) * 256u
        let bB = (sg / 2u) * 128u
        var ma0 : simdgroup_half8x8
        var ma1 : simdgroup_half8x8
        var ma2 : simdgroup_half8x8
        var ma3 : simdgroup_half8x8
        var mb0 : simdgroup_half8x8
        var mb1 : simdgroup_half8x8
        var kb = 0u
        while (kb < nkb) {
            barrier()
            barrier()
            simdgroup_load(ma0, ta, aB, 8u)
            simdgroup_load(ma1, ta, aB + 64u, 8u)
            simdgroup_load(ma2, ta, aB + 128u, 8u)
            simdgroup_load(ma3, ta, aB + 192u, 8u)
            simdgroup_load(mb0, tb, bB, 8u)
            simdgroup_load(mb1, tb, bB + 64u, 8u)
            simdgroup_multiply_accumulate(c00, mb0, ma0, c00)
            simdgroup_multiply_accumulate(c01, mb0, ma1, c01)
            simdgroup_multiply_accumulate(c02, mb0, ma2, c02)
            simdgroup_multiply_accumulate(c03, mb0, ma3, c03)
            simdgroup_multiply_accumulate(c10, mb1, ma0, c10)
            simdgroup_multiply_accumulate(c11, mb1, ma1, c11)
            simdgroup_multiply_accumulate(c12, mb1, ma2, c12)
            simdgroup_multiply_accumulate(c13, mb1, ma3, c13)
            simdgroup_load(ma0, ta, aB + 512u, 8u)
            simdgroup_load(ma1, ta, aB + 576u, 8u)
            simdgroup_load(ma2, ta, aB + 640u, 8u)
            simdgroup_load(ma3, ta, aB + 704u, 8u)
            simdgroup_load(mb0, tb, bB + 256u, 8u)
            simdgroup_load(mb1, tb, bB + 320u, 8u)
            simdgroup_multiply_accumulate(c00, mb0, ma0, c00)
            simdgroup_multiply_accumulate(c01, mb0, ma1, c01)
            simdgroup_multiply_accumulate(c02, mb0, ma2, c02)
            simdgroup_multiply_accumulate(c03, mb0, ma3, c03)
            simdgroup_multiply_accumulate(c10, mb1, ma0, c10)
            simdgroup_multiply_accumulate(c11, mb1, ma1, c11)
            simdgroup_multiply_accumulate(c12, mb1, ma2, c12)
            simdgroup_multiply_accumulate(c13, mb1, ma3, c13)
            simdgroup_load(ma0, ta, aB + 1024u, 8u)
            simdgroup_load(ma1, ta, aB + 1088u, 8u)
            simdgroup_load(ma2, ta, aB + 1152u, 8u)
            simdgroup_load(ma3, ta, aB + 1216u, 8u)
            simdgroup_load(mb0, tb, bB + 512u, 8u)
            simdgroup_load(mb1, tb, bB + 576u, 8u)
            simdgroup_multiply_accumulate(c00, mb0, ma0, c00)
            simdgroup_multiply_accumulate(c01, mb0, ma1, c01)
            simdgroup_multiply_accumulate(c02, mb0, ma2, c02)
            simdgroup_multiply_accumulate(c03, mb0, ma3, c03)
            simdgroup_multiply_accumulate(c10, mb1, ma0, c10)
            simdgroup_multiply_accumulate(c11, mb1, ma1, c11)
            simdgroup_multiply_accumulate(c12, mb1, ma2, c12)
            simdgroup_multiply_accumulate(c13, mb1, ma3, c13)
            simdgroup_load(ma0, ta, aB + 1536u, 8u)
            simdgroup_load(ma1, ta, aB + 1600u, 8u)
            simdgroup_load(ma2, ta, aB + 1664u, 8u)
            simdgroup_load(ma3, ta, aB + 1728u, 8u)
            simdgroup_load(mb0, tb, bB + 768u, 8u)
            simdgroup_load(mb1, tb, bB + 832u, 8u)
            simdgroup_multiply_accumulate(c00, mb0, ma0, c00)
            simdgroup_multiply_accumulate(c01, mb0, ma1, c01)
            simdgroup_multiply_accumulate(c02, mb0, ma2, c02)
            simdgroup_multiply_accumulate(c03, mb0, ma3, c03)
            simdgroup_multiply_accumulate(c10, mb1, ma0, c10)
            simdgroup_multiply_accumulate(c11, mb1, ma1, c11)
            simdgroup_multiply_accumulate(c12, mb1, ma2, c12)
            simdgroup_multiply_accumulate(c13, mb1, ma3, c13)
            kb++
        }
        let row0 = mBase + (sg / 2u) * 16u
        let col0 = nBase + (sg % 2u) * 32u
        simdgroup_store(c00, y, row0 * ndim + col0, ndim)
        simdgroup_store(c01, y, row0 * ndim + (col0 + 8u), ndim)
        simdgroup_store(c02, y, row0 * ndim + (col0 + 16u), ndim)
        simdgroup_store(c03, y, row0 * ndim + (col0 + 24u), ndim)
        simdgroup_store(c10, y, (row0 + 8u) * ndim + col0, ndim)
        simdgroup_store(c11, y, (row0 + 8u) * ndim + (col0 + 8u), ndim)
        simdgroup_store(c12, y, (row0 + 8u) * ndim + (col0 + 16u), ndim)
        simdgroup_store(c13, y, (row0 + 8u) * ndim + (col0 + 24u), ndim)
    }
}

// ===== driver (Apple-only; every Metal-typed helper is a generic so non-Apple never compiles it) =====

struct private LabVariant {
    name : string
    src : string
    entry : string
    fastmath : bool
    bm : int                                // C-tile token rows — M pads to this
    bn : int                                // C-tile output columns — requires d % bn == 0
    check : bool = true                     // false = knockout: time it, never compare output
    threads : int = 128                     // threads per threadgroup
    f16w : bool = false                     // binding 0 gets the PRE-DEQUANTIZED f16 W panel
    lcpp : bool = false                     // llama.cpp buffer contract (kargs/blob/f32-X/dst) + 3D grid
    grid2d : bool = false                   // dispatch (M'/bm, d/bn, 1) — tokens on the fast axis (ggml rasterization)
    f32x : bool = false                     // binding 2 gets the PRE-DEQUANTIZED f32 activation panel
    wscT : bool = false                     // binding 1 gets the [block][row]-TRANSPOSED W scales
    blob36 : bool = false                   // bindings 0+1 get the SAME 36B-block W blob (scale/quant views) + f32 X
    blob34 : bool = false                   // bindings 0+1 get llama.cpp's OWN 34B blob (half-scale/byte views) + f32 X
}

def private align_up(v, a : int64) : int64 => ((v + a - 1l) / a) * a

// exact-arithmetic Q8 fill: quants in [-8, 7], pow2 scales, zero pad rows past `rows`
def private fill_exact_q8(var q : array<int8>; var s : array<float>; rows, padded_rows, k : int64; seed : int64) {
    let nb = k / 32l
    q |> resize(padded_rows * k)
    s |> resize(padded_rows * nb)
    let lut = fixed_array(0.25, 0.5, 1.0, 2.0)
    for (i in range64(padded_rows * k)) {
        q[i] = int8(i < rows * k ? int((i * seed + i / 31l) % 16l) - 8 : 0)
    }
    for (i in range64(padded_rows * nb)) {
        s[i] = i < rows * nb ? lut[int((i / 7l) % 4l)] : 0.0
    }
}

// CPU reference for the first `nrows` output rows — every partial sum is exact (see header)
def private cpu_ref_rows(wq : array<int8>; ws : array<float>; xq : array<int8>; xs : array<float>;
                         n, d, nrows : int64) : array<float> {
    var y : array<float>
    y |> resize(nrows * d)
    let nb = n / 32l
    for (r in range64(nrows)) {
        for (j in range64(d)) {
            var acc = 0.0
            for (kb in range64(nb)) {
                var blk = 0.0
                for (c in range64(32l)) {
                    blk += float(wq[j * n + kb * 32l + c]) * float(xq[r * n + kb * 32l + c])
                }
                acc += blk * (ws[j * nb + kb] * xs[r * nb + kb])
            }
            y[r * d + j] = acc
        }
    }
    return <- y
}

def private upload_array(dev; arr; bytes : int64) {
    var buf = metal_new_buffer(dev, uint64(bytes))
    unsafe {
        memcpy(metal_buffer_contents(buf), reinterpret<void?>(addr(arr[0])), int(bytes))
    }
    return buf
}

def private upload_u32(dev; v : uint) {
    var buf = metal_new_buffer(dev, 4ul)
    var p = unsafe(reinterpret<uint?>(metal_buffer_contents(buf)))
    unsafe(p[0]) = v
    return buf
}

def private download_floats(buf; var dst : array<float>; count : int64) {
    dst |> resize(count)
    unsafe {
        memcpy(reinterpret<void?>(addr(dst[0])), metal_buffer_contents(buf), int(count * 4l))
    }
}

// ===== v24 probe: hand-restructured MSL — v22's emitted text, math section lcpp-style =====
// The emitted v22 is fully unrolled SSA: LLVM hoists ~42 loop-invariant threadgroup addresses
// (16 A-store + 2 B-store + 24 sgmat-load pointers) out of the k-loop, holding them in registers
// for the kernel's whole life — max_threads drops 1024 -> 832 (register-limited) and occupancy
// eats ~13%. This probe keeps v22's staging verbatim but rewrites the math as llama.cpp shapes
// it: matrix ARRAYS + rolled unroll(full) loops + two bumped tile pointers, so the AGX backend
// unrolls with register-budget awareness. das-side equivalent needs emitter work — probe first.
let LAB_V22ROLL_MSL = "#include <metal_stdlib>
using namespace metal;
kernel void lab_v22roll(device const float* wsc [[buffer(0)]],
        device const char4* wqv [[buffer(1)]],
        device const float4* xf [[buffer(2)]],
        device const float* xs [[buffer(3)]],
        device float* y [[buffer(4)]],
        constant uint& kdim [[buffer(5)]],
        constant uint& ndim [[buffer(6)]],
        uint3 gl_LocalInvocationID [[thread_position_in_threadgroup]],
        uint gl_SubgroupID [[simdgroup_index_in_threadgroup]],
        uint3 gl_WorkGroupID [[threadgroup_position_in_grid]]) \{
    threadgroup half ta[2048];
    threadgroup half tb[1024];
    uint lid = gl_LocalInvocationID.x;
    uint sg = gl_SubgroupID;
    uint mBase = (gl_WorkGroupID.x * 32u);
    uint nBase = (gl_WorkGroupID.y * 64u);
    simdgroup_half8x8 ma[4];
    simdgroup_half8x8 mb[2];
    simdgroup_float8x8 mc[8];
    #pragma clang loop unroll(full)
    for (short i = 0; i < 8; i++) \{
        mc[i] = make_filled_simdgroup_matrix<float, 8>(0.f);
    \}
    uint kdim4 = (kdim / 4u);
    uint nkb = (kdim / 32u);
    uint lr0 = (lid / 2u);
    uint il0 = (lid % 2u);
    uint syA = (lr0 / 8u);
    uint lxA = (lr0 % 8u);
    uint tA0 = (((((il0 * 2u) * 8u) + syA) * 64u) + lxA);
    uint tA1 = ((((((il0 * 2u) + 1u) * 8u) + syA) * 64u) + lxA);
    uint lr1 = (lid / 4u);
    uint iy = ((lid % 4u) * 8u);
    uint ibB = (((((lid % 4u) * 4u) + (lr1 / 8u)) * 64u) + ((lr1 % 8u) * 8u));
    uint aB = ((sg % 2u) * 256u);
    uint bB = ((sg / 2u) * 128u);
    uint kb = 0u;
    while ((kb < nkb)) \{
        uint b9 = ((((nBase + lr0) * nkb) + kb) * 9u);
        float sA = wsc[b9];
        uint srcA = ((b9 + 1u) + (il0 * 4u));
        char4 a0 = wqv[srcA];
        char4 a1 = wqv[(srcA + 1u)];
        char4 a2 = wqv[(srcA + 2u)];
        char4 a3 = wqv[(srcA + 3u)];
        uint srcB = (((mBase + lr1) * kdim4) + (((kb * 32u) + iy) / 4u));
        float4 b0 = xf[srcB];
        float4 b1 = xf[(srcB + 1u)];
        threadgroup_barrier(mem_flags::mem_threadgroup);
        float4 va0 = (float4(int4(a0)) * sA);
        float4 va1 = (float4(int4(a1)) * sA);
        float4 va2 = (float4(int4(a2)) * sA);
        float4 va3 = (float4(int4(a3)) * sA);
        ta[tA0] = half(va0.x);
        ta[(tA0 + 8u)] = half(va0.y);
        ta[(tA0 + 16u)] = half(va0.z);
        ta[(tA0 + 24u)] = half(va0.w);
        ta[(tA0 + 32u)] = half(va1.x);
        ta[(tA0 + 40u)] = half(va1.y);
        ta[(tA0 + 48u)] = half(va1.z);
        ta[(tA0 + 56u)] = half(va1.w);
        ta[tA1] = half(va2.x);
        ta[(tA1 + 8u)] = half(va2.y);
        ta[(tA1 + 16u)] = half(va2.z);
        ta[(tA1 + 24u)] = half(va2.w);
        ta[(tA1 + 32u)] = half(va3.x);
        ta[(tA1 + 40u)] = half(va3.y);
        ta[(tA1 + 48u)] = half(va3.z);
        ta[(tA1 + 56u)] = half(va3.w);
        *(threadgroup half4*)(tb + (ibB)) = half4(b0);
        *(threadgroup half4*)(tb + ((ibB + 4u))) = half4(b1);
        threadgroup_barrier(mem_flags::mem_threadgroup);
        threadgroup const half * lsma = ta + aB;
        threadgroup const half * lsmb = tb + bB;
        #pragma clang loop unroll(full)
        for (short ik = 0; ik < 4; ik++) \{
            #pragma clang loop unroll(full)
            for (short i = 0; i < 4; i++) \{
                simdgroup_load(ma[i], lsma + 64*i, 8, 0, false);
            \}
            #pragma clang loop unroll(full)
            for (short i = 0; i < 2; i++) \{
                simdgroup_load(mb[i], lsmb + 64*i, 8, 0, false);
            \}
            #pragma clang loop unroll(full)
            for (short i = 0; i < 8; i++) \{
                simdgroup_multiply_accumulate(mc[i], mb[i/4], ma[i%4], mc[i]);
            \}
            lsma += 8*64;
            lsmb += 4*64;
        \}
        ++kb;
    \}
    uint row0 = (mBase + ((sg / 2u) * 16u));
    uint col0 = (nBase + ((sg % 2u) * 32u));
    device float * C = y + ((row0 * ndim) + col0);
    #pragma clang loop unroll(full)
    for (short i = 0; i < 8; i++) \{
        simdgroup_store(mc[i], C + 8*(i%4) + 8*ndim*(i/4), ndim, 0, false);
    \}
\}
"

let LAB_V24G_MSL = "#include <metal_stdlib>
using namespace metal;
kernel void lab_v24g(device const half* wsh [[buffer(0)]],
        device const char* wqb [[buffer(1)]],
        device const float4* xf [[buffer(2)]],
        device const float* xs [[buffer(3)]],
        device float* y [[buffer(4)]],
        constant uint& kdim [[buffer(5)]],
        constant uint& ndim [[buffer(6)]],
        uint3 gl_LocalInvocationID [[thread_position_in_threadgroup]],
        uint gl_SubgroupID [[simdgroup_index_in_threadgroup]],
        uint3 gl_WorkGroupID [[threadgroup_position_in_grid]]) \{
    threadgroup half ta[2048];
    threadgroup half tb[1024];
    uint lid = gl_LocalInvocationID.x;
    uint sg = gl_SubgroupID;
    uint mBase = (gl_WorkGroupID.x * 32u);
    uint nBase = (gl_WorkGroupID.y * 64u);
    simdgroup_half8x8 ma[4];
    simdgroup_half8x8 mb[2];
    simdgroup_float8x8 mc[8];
    #pragma clang loop unroll(full)
    for (short i = 0; i < 8; i++) \{
        mc[i] = make_filled_simdgroup_matrix<float, 8>(0.f);
    \}
    uint kdim4 = (kdim / 4u);
    uint nkb = (kdim / 32u);
    uint lr0 = (lid / 2u);
    uint il0 = (lid % 2u);
    uint syA = (lr0 / 8u);
    uint lxA = (lr0 % 8u);
    uint tA0 = (((((il0 * 2u) * 8u) + syA) * 64u) + lxA);
    uint lr1 = (lid / 4u);
    uint iy = ((lid % 4u) * 8u);
    uint ibB = (((((lid % 4u) * 4u) + (lr1 / 8u)) * 64u) + ((lr1 % 8u) * 8u));
    uint aB = ((sg % 2u) * 256u);
    uint bB = ((sg / 2u) * 128u);
    uint kb = 0u;
    while ((kb < nkb)) \{
        uint blk = (((nBase + lr0) * nkb) + kb);
        float sA = (float)wsh[blk * 17u];
        device const char * qp = wqb + blk * 34u + 2u + il0 * 16u;
        uint srcB = (((mBase + lr1) * kdim4) + (((kb * 32u) + iy) / 4u));
        float4 b0 = xf[srcB];
        float4 b1 = xf[(srcB + 1u)];
        half va[16];
        #pragma clang loop unroll(full)
        for (short i = 0; i < 16; i++) \{
            va[i] = (half)((float)qp[i] * sA);
        \}
        threadgroup_barrier(mem_flags::mem_threadgroup);
        #pragma clang loop unroll(full)
        for (short i = 0; i < 16; i++) \{
            ta[tA0 + (i/8)*512 + (i%8)*8] = va[i];
        \}
        *(threadgroup half4*)(tb + (ibB)) = half4(b0);
        *(threadgroup half4*)(tb + ((ibB + 4u))) = half4(b1);
        threadgroup_barrier(mem_flags::mem_threadgroup);
        threadgroup const half * lsma = ta + aB;
        threadgroup const half * lsmb = tb + bB;
        #pragma clang loop unroll(full)
        for (short ik = 0; ik < 4; ik++) \{
            #pragma clang loop unroll(full)
            for (short i = 0; i < 4; i++) \{
                simdgroup_load(ma[i], lsma + 64*i, 8, 0, false);
            \}
            #pragma clang loop unroll(full)
            for (short i = 0; i < 2; i++) \{
                simdgroup_load(mb[i], lsmb + 64*i, 8, 0, false);
            \}
            #pragma clang loop unroll(full)
            for (short i = 0; i < 8; i++) \{
                simdgroup_multiply_accumulate(mc[i], mb[i/4], ma[i%4], mc[i]);
            \}
            lsma += 8*64;
            lsmb += 4*64;
        \}
        ++kb;
    \}
    uint row0 = (mBase + ((sg / 2u) * 16u));
    uint col0 = (nBase + ((sg % 2u) * 32u));
    device float * C = y + ((row0 * ndim) + col0);
    #pragma clang loop unroll(full)
    for (short i = 0; i < 8; i++) \{
        simdgroup_store(mc[i], C + 8*(i%4) + 8*ndim*(i/4), ndim, 0, false);
    \}
\}
"

let LAB_V24H1_MSL = "#include <metal_stdlib>
using namespace metal;
kernel void lab_v24h1(device const half* wsh [[buffer(0)]],
        device const char* wqb [[buffer(1)]],
        device const float4* xf [[buffer(2)]],
        device const float* xs [[buffer(3)]],
        device float* y [[buffer(4)]],
        constant uint& kdim [[buffer(5)]],
        constant uint& ndim [[buffer(6)]],
        uint3 gl_LocalInvocationID [[thread_position_in_threadgroup]],
        uint gl_SubgroupID [[simdgroup_index_in_threadgroup]],
        uint3 gl_WorkGroupID [[threadgroup_position_in_grid]]) \{
    threadgroup half ta[2048];
    threadgroup half tb[1024];
    uint lid = gl_LocalInvocationID.x;
    uint sg = gl_SubgroupID;
    uint mBase = (gl_WorkGroupID.x * 32u);
    uint nBase = (gl_WorkGroupID.y * 64u);
    simdgroup_half8x8 ma[4];
    simdgroup_half8x8 mb[2];
    simdgroup_float8x8 mc[8];
    #pragma clang loop unroll(full)
    for (short i = 0; i < 8; i++) \{
        mc[i] = make_filled_simdgroup_matrix<float, 8>(0.f);
    \}
    uint kdim4 = (kdim / 4u);
    uint nkb = (kdim / 32u);
    uint lr0 = (lid / 2u);
    uint il0 = (lid % 2u);
    uint syA = (lr0 / 8u);
    uint lxA = (lr0 % 8u);
    uint tA0 = (((((il0 * 2u) * 8u) + syA) * 64u) + lxA);
    uint lr1 = (lid / 4u);
    uint iy = ((lid % 4u) * 8u);
    uint ibB = (((((lid % 4u) * 4u) + (lr1 / 8u)) * 64u) + ((lr1 % 8u) * 8u));
    uint aB = ((sg % 2u) * 256u);
    uint bB = ((sg / 2u) * 128u);
    uint kb = 0u;
    while ((kb < nkb)) \{
        uint blk = (((nBase + lr0) * nkb) + kb);
        float sA = (float)wsh[blk * 17u];
        device const char * qp = wqb + blk * 34u + 2u + il0 * 16u;
        uint srcB = (((mBase + lr1) * kdim4) + (((kb * 32u) + iy) / 4u));
        float4 b0 = xf[srcB];
        float4 b1 = xf[(srcB + 1u)];
        half va[16];
        #pragma clang loop unroll(full)
        for (short i = 0; i < 16; i++) \{
            va[i] = (half)((float)qp[i] * sA);
        \}
        threadgroup_barrier(mem_flags::mem_threadgroup);
        #pragma clang loop unroll(full)
        for (short i = 0; i < 16; i++) \{
            ta[tA0 + (i/8)*512 + (i%8)*8] = va[i];
        \}
        *(threadgroup half4*)(tb + (ibB)) = half4(b0);
        *(threadgroup half4*)(tb + ((ibB + 4u))) = half4(b1);
        threadgroup_barrier(mem_flags::mem_threadgroup);
        #pragma clang loop unroll(full)
        for (int ik = 0; ik < 4; ++ik) \{
            #pragma clang loop unroll(full)
            for (int i = 0; i < 4; ++i) \{
                simdgroup_load(ma[i], ta + ((aB + (uint(ik) * 512u)) + (uint(i) * 64u)), 8u);
            \}
            #pragma clang loop unroll(full)
            for (int i = 0; i < 2; ++i) \{
                simdgroup_load(mb[i], tb + ((bB + (uint(ik) * 256u)) + (uint(i) * 64u)), 8u);
            \}
            #pragma clang loop unroll(full)
            for (int i = 0; i < 8; ++i) \{
                simdgroup_multiply_accumulate(mc[i], mb[(i / 4)], ma[(i % 4)], mc[i]);
            \}
        \}
        ++kb;
    \}
    uint row0 = (mBase + ((sg / 2u) * 16u));
    uint col0 = (nBase + ((sg % 2u) * 32u));
    device float * C = y + ((row0 * ndim) + col0);
    #pragma clang loop unroll(full)
    for (short i = 0; i < 8; i++) \{
        simdgroup_store(mc[i], C + 8*(i%4) + 8*ndim*(i/4), ndim, 0, false);
    \}
\}
"


// ===== llama.cpp verbatim-kernel leg (see _lcpp_mul_mm_q8.das) =====
// Their production contract differs from ours end to end: src0 = interleaved block_q8_0 blobs
// (half scale + 32 int8, 34 B/block), src1 = raw f32 activations (their mul_mm never sees
// quantized X), dst = same y[tok*d + out] layout as v0, kargs struct in buffer 0, 6144 B of
// dynamic threadgroup scratch, grid (ne1/32, ne0/64, 1) x (32,4,1) threads.

def private upload_lcpp_q8_blob(dev; q : array<int8>; s : array<float>; rows, k : int64; enabled : bool) {
    if (!enabled) {
        return metal_new_buffer(dev, 64ul)
    }
    let nb = int(k / 32l)
    let kk = int(k)
    var buf = metal_new_buffer(dev, uint64(rows * (k / 32l) * 34l))
    var base = unsafe(reinterpret<int8?>(metal_buffer_contents(buf)))
    for (r in range(int(rows))) {
        for (bi in range(nb)) {
            let off = (r * nb + bi) * 34
            unsafe {
                var ph = reinterpret<float16?>(base + off)
                ph[0] = float16(s[r * nb + bi])
                let src = r * kk + bi * 32
                for (c in range(32)) {
                    base[off + 2 + c] = q[src + c]
                }
            }
        }
    }
    return buf
}

def private upload_lcpp_f32_x(dev; xq : array<int8>; xs : array<float>; rows, k : int64; enabled : bool) {
    if (!enabled) {
        return metal_new_buffer(dev, 64ul)
    }
    let nb = int(k / 32l)
    let kk = int(k)
    var buf = metal_new_buffer(dev, uint64(rows * k * 4l))
    var pf = unsafe(reinterpret<float?>(metal_buffer_contents(buf)))
    for (r in range(int(rows))) {
        let s0 = r * nb
        for (i in range(kk)) {
            unsafe(pf[r * kk + i]) = float(xq[r * kk + i]) * xs[s0 + i / 32]
        }
    }
    return buf
}

// interleaved 36B-block W blob (f32 scale + 32 int8 quants, 4-aligned) — the Option-A
// production layout: same bytes as planar quants+scales, but the scale rides the quants'
// cacheline and byte4 quant loads stay aligned (ggml's 34B half-scale blocks force scalar reads)
def private upload_blob36(dev; q : array<int8>; s : array<float>; rows, k : int64; enabled : bool) {
    if (!enabled) {
        return metal_new_buffer(dev, 64ul)
    }
    let nb = int(k / 32l)
    let kk = int(k)
    var buf = metal_new_buffer(dev, uint64(rows * (k / 32l) * 36l))
    var pf = unsafe(reinterpret<float?>(metal_buffer_contents(buf)))
    var p8 = unsafe(reinterpret<int8?>(metal_buffer_contents(buf)))
    for (r in range(int(rows))) {
        for (bi in range(nb)) {
            let b = r * nb + bi
            unsafe {
                pf[b * 9] = s[b]
                let dst = b * 36 + 4
                let src = r * kk + bi * 32
                for (c in range(32)) {
                    p8[dst + c] = q[src + c]
                }
            }
        }
    }
    return buf
}

// byte-exact image of ggml_metal_kargs_mul_mm (2 i32, 3 u64, i32 + 4 pad, 4 u64, 2 i32, 2 i16 = 88 B)
def private upload_lcpp_args(dev; k, d, ntok : int64) {
    var buf = metal_new_buffer(dev, 88ul)
    var pc = metal_buffer_contents(buf)
    var pi = unsafe(reinterpret<int?>(pc))
    var pu = unsafe(reinterpret<uint64?>(pc))
    var ps = unsafe(reinterpret<int16?>(pc))
    let nb01 = (k / 32l) * 34l
    unsafe {
        pi[0] = int(k)                      // ne00
        pi[1] = 1                           // ne02
        pu[1] = uint64(nb01)                // nb01
        pu[2] = uint64(nb01 * d)            // nb02
        pu[3] = uint64(nb01 * d)            // nb03
        pi[8] = 1                           // ne12
        pi[9] = 0                           // struct padding — keep deterministic
        pu[5] = 4ul                         // nb10
        pu[6] = uint64(k * 4l)              // nb11
        pu[7] = uint64(k * 4l * ntok)       // nb12
        pu[8] = uint64(k * 4l * ntok)       // nb13
        pi[18] = int(d)                     // ne0
        pi[19] = int(ntok)                  // ne1
        ps[40] = int16(1)                   // r2
        ps[41] = int16(1)                   // r3
    }
    return buf
}

def private time_reps_lcpp(queue; pso; abuf, wblob, xbuf, ybuf; groups : uint3; reps : int) : double {
    var err : string
    var gpu_ms = 0.0lf
    let ok = with_compute_encoder_timed(queue, err, gpu_ms) $(enc) {
        metal_set_pipeline(enc, pso)
        metal_set_buffer(enc, abuf, 0ul, 0)
        metal_set_buffer(enc, wblob, 0ul, 1)
        metal_set_buffer(enc, xbuf, 0ul, 2)
        metal_set_buffer(enc, ybuf, 0ul, 3)
        metal_set_threadgroup_memory_length(enc, 6144ul, 0)
        for (_i in range(reps)) {
            metal_dispatch_threadgroups(enc, groups, uint3(32u, 4u, 1u))
        }
    }
    if (!ok) {
        to_log(LOG_ERROR, "lcpp dispatch failed: {err}\n")
        return -1.0lf
    }
    return gpu_ms / double(reps)
}

// count bit-mismatches vs the reference (exact-arithmetic data — any deviation is a real bug)
def private count_mismatch(got, ref_v : array<float>; cnt : int64; var firstBad : int64&) : int64 {
    var bad = 0l
    firstBad = -1l
    for (i in range64(cnt)) {
        if (got[i] != ref_v[i]) {
            if (firstBad < 0l) {
                firstBad = i
            }
            bad++
        }
    }
    return bad
}

def private compile_lab_pso(dev; v : LabVariant) {
    var err : string
    var pso = pipeline_from_source(dev, v.src, v.entry, v.fastmath, err)
    if (pso == null) {
        to_log(LOG_ERROR, "{v.name}: kernel compile failed — {err}\n")
    }
    return pso
}

// one timed encoder: set state once, dispatch `reps` back-to-back (hazard tracking on the shared
// y buffer serializes them); returns per-dispatch GPU milliseconds, or -1 on failure
def private time_reps(queue; pso; bufs; groups : uint3; threads : uint; reps : int) : double {
    var err : string
    var gpu_ms = 0.0lf
    let ok = with_compute_encoder_timed(queue, err, gpu_ms) $(enc) {
        metal_set_pipeline(enc, pso)
        for (bi in range(length(bufs))) {
            metal_set_buffer(enc, bufs[bi], 0ul, bi)
        }
        for (_i in range(reps)) {
            metal_dispatch_threadgroups(enc, groups, uint3(threads, 1u, 1u))
        }
    }
    if (!ok) {
        to_log(LOG_ERROR, "dispatch failed: {err}\n")
        return -1.0lf
    }
    return gpu_ms / double(reps)
}

def private env_or(name : string; dflt : int) : int {
    return has_env_variable(name) ? to_int(get_env_variable(name)) : dflt
}

def private want(filter_env, name : string) : bool {
    return !has_env_variable(filter_env) || find(get_env_variable(filter_env), name) >= 0
}

def private run_lab(b : B?; variants) {
    let ntok = int64(env_or("DASLLAMA_BENCH_NTOK", 512))
    let rounds = env_or("DASMETAL_LAB_ROUNDS", 3)
    let ref_rows = 2l
    var dev = metal_create_system_default_device()
    if (dev == null) {
        to_log(LOG_WARNING, "skipped: no Metal device\n")
        return
    }
    to_log(LOG_INFO, "device={metal_device_name(dev)} ntok={ntok} rounds={rounds}\n")
    var queue = metal_new_command_queue(dev)
    var psos <- [for (v in variants); compile_lab_pso(dev, v)]
    for (v, pso in variants, psos) {
        b |> success(pso != null, "pipeline compiles: {v.name}")
        if (pso != null && want("DASMETAL_LAB_VARIANTS", v.name)) {
            // register-pressure signal: a reg-limited kernel reports max_threads < 1024
            to_log(LOG_INFO, "{v.name}: max_threads={metal_pipeline_max_total_threads(pso)} simd_width={metal_pipeline_thread_execution_width(pso)}\n")
        }
    }
    // llama1b GEMM geometries (n = reduction dim, d = output rows)
    var shapes <- [("kv", 2048l, 512l), ("q", 2048l, 2048l), ("w13", 2048l, 8192l),
                   ("w2", 8192l, 2048l), ("cls", 2048l, 128256l),
                   // the llama-3B set (dim 3072, hidden 8192, kv_dim 1024)
                   ("kv3b", 3072l, 1024l), ("q3b", 3072l, 3072l), ("w13_3b", 3072l, 8192l),
                   ("w2_3b", 8192l, 3072l)]
    let mp_max = align_up(ntok, 64l)
    for (sh in shapes) {
        if (!want("DASMETAL_LAB_SHAPES", sh._0)) {
            continue
        }
        let n = sh._1
        let d = sh._2
        let nb = n / 32l
        let t_shape0 = ref_time_ticks()
        var wq : array<int8>
        var ws : array<float>
        var xq : array<int8>
        var xs : array<float>
        fill_exact_q8(wq, ws, d, d, n, 7l)
        fill_exact_q8(xq, xs, ntok, mp_max, n, 11l)
        var yref <- cpu_ref_rows(wq, ws, xq, xs, n, d, ref_rows)
        var wbuf = upload_array(dev, wq, d * n)
        var sbuf = upload_array(dev, ws, d * nb * 4l)
        // the f16 W panel (v.f16w variants): W dequantized once host-side — exact in f16 under
        // the lab's exact-arithmetic fill (|w| <= 16, int x pow2)
        var wh : array<float16>
        wh |> resize(int(d * n))
        for (i in range64(d * n)) {
            wh[i] = float16(float(wq[i]) * ws[(i / n) * nb + (i % n) / 32l])
        }
        var whbuf = upload_array(dev, wh, d * n * 2l)
        var xqbuf = upload_array(dev, xq, mp_max * n)
        var xsbuf = upload_array(dev, xs, mp_max * nb * 4l)
        var ybuf = metal_new_buffer(dev, uint64(mp_max * d * 4l))
        var kdimbuf = upload_u32(dev, uint(n))
        var ndimbuf = upload_u32(dev, uint(d))
        var bufs <- [wbuf, sbuf, xqbuf, xsbuf, ybuf, kdimbuf, ndimbuf]
        var bufs16 <- [whbuf, sbuf, xqbuf, xsbuf, ybuf, kdimbuf, ndimbuf]
        // the llama.cpp leg's buffers — built only when a variant that needs them will run
        var lcppOn = false
        var xf32On = false
        for (v in variants) {
            if (v.lcpp && want("DASMETAL_LAB_VARIANTS", v.name) && ntok % 32l == 0l && (d % int64(v.bn)) == 0l) {
                lcppOn = true
            }
            if ((v.lcpp || v.f32x || v.blob36 || v.blob34) && want("DASMETAL_LAB_VARIANTS", v.name) && (d % int64(v.bn)) == 0l) {
                xf32On = true
            }
        }
        var blob36On = false
        var blob34On = false
        for (v in variants) {
            if (v.blob36 && want("DASMETAL_LAB_VARIANTS", v.name) && (d % int64(v.bn)) == 0l) {
                blob36On = true
            }
            if (v.blob34 && want("DASMETAL_LAB_VARIANTS", v.name) && (d % int64(v.bn)) == 0l) {
                blob34On = true
            }
        }
        var lcpp_wblob = upload_lcpp_q8_blob(dev, wq, ws, d, n, lcppOn || blob34On)
        var lcpp_xbuf = upload_lcpp_f32_x(dev, xq, xs, mp_max, n, xf32On)
        var lcpp_abuf = upload_lcpp_args(dev, n, d, ntok)
        var bufsf32x <- [wbuf, sbuf, lcpp_xbuf, xsbuf, ybuf, kdimbuf, ndimbuf]
        // [block][row]-transposed W scales (wscT variants)
        var wsT : array<float>
        wsT |> resize(int(d * nb))
        for (r in range64(d)) {
            for (bi in range64(nb)) {
                wsT[int(bi * d + r)] = ws[r * nb + bi]
            }
        }
        var wsTbuf = upload_array(dev, wsT, d * nb * 4l)
        delete wsT
        var bufswsct <- [wbuf, wsTbuf, xqbuf, xsbuf, ybuf, kdimbuf, ndimbuf]
        var wblob36 = upload_blob36(dev, wq, ws, d, n, blob36On)
        var bufs36 <- [wblob36, wblob36, lcpp_xbuf, xsbuf, ybuf, kdimbuf, ndimbuf]
        var bufs34 <- [lcpp_wblob, lcpp_wblob, lcpp_xbuf, xsbuf, ybuf, kdimbuf, ndimbuf]
        let setup_ms = double(ref_time_ticks() - t_shape0) / 1000000.0lf
        to_log(LOG_INFO, "=== {sh._0} n={n} d={d} === (setup {setup_ms} ms)\n")
        var y0 : array<float>                // v0's full output — the cross-variant reference
        var ygot : array<float>
        var alive : array<bool>
        var est_ms : array<double>
        alive |> resize(length(variants))
        est_ms |> resize(length(variants))
        for (v, pso, idx in variants, psos, count()) {
            alive[idx] = false
            est_ms[idx] = -1.0lf
            if (pso == null || (d % int64(v.bn)) != 0l || !want("DASMETAL_LAB_VARIANTS", v.name) || (v.lcpp && !lcppOn)) {
                continue
            }
            let mp = align_up(ntok, int64(v.bm))
            let groups = (v.grid2d
                ? uint3(uint(mp / int64(v.bm)), uint(d / int64(v.bn)), 1u)
                : uint3(uint((mp / int64(v.bm)) * (d / int64(v.bn))), 1u, 1u))
            let ms = (v.lcpp                    // warmup + correctness dispatch
                ? time_reps_lcpp(queue, pso, lcpp_abuf, lcpp_wblob, lcpp_xbuf, ybuf,
                                 uint3(uint(ntok / 32l), uint(d / 64l), 1u), 1)
                : time_reps(queue, pso, v.blob36 ? bufs36 : (v.blob34 ? bufs34 : (v.wscT ? bufswsct : (v.f32x ? bufsf32x : (v.f16w ? bufs16 : bufs)))), groups, uint(v.threads), 1))
            if (ms < 0.0lf) {
                b |> success(false, "{v.name} {sh._0}: dispatch")
                continue
            }
            est_ms[idx] = ms
            if (!v.check) {
                to_log(LOG_INFO, "{v.name} {sh._0}: timing-only knockout (output unchecked)\n")
                alive[idx] = true
                continue
            }
            var firstBad = -1l
            var bad = 0l
            if (idx == 0) {
                download_floats(ybuf, y0, ntok * d)
                bad = count_mismatch(y0, yref, ref_rows * d, firstBad)
                b |> success(bad == 0l, "{v.name} {sh._0}: bit-exact vs CPU reference rows")
            } else {
                download_floats(ybuf, ygot, ntok * d)
                bad = count_mismatch(ygot, y0, ntok * d, firstBad)
                b |> success(bad == 0l, "{v.name} {sh._0}: bit-exact vs v0")
            }
            if (bad != 0l) {
                let gv = idx == 0 ? y0[firstBad] : ygot[firstBad]
                let wv = idx == 0 ? yref[firstBad] : y0[firstBad]
                to_log(LOG_ERROR, "{v.name} {sh._0}: {bad} mismatches, first at {firstBad}: got {gv} want {wv}\n")
                continue
            }
            alive[idx] = true
        }
        // interleaved best-of-N timing rounds
        var best : array<double>
        best |> resize(length(variants))
        for (i in range(length(variants))) {
            best[i] = 1.0e30lf
        }
        for (_round in range(rounds)) {
            for (v, pso, idx in variants, psos, count()) {
                if (!alive[idx]) {
                    continue
                }
                let mp = align_up(ntok, int64(v.bm))
                let groups = (v.grid2d
                    ? uint3(uint(mp / int64(v.bm)), uint(d / int64(v.bn)), 1u)
                    : uint3(uint((mp / int64(v.bm)) * (d / int64(v.bn))), 1u, 1u))
                let reps = clamp(int(25.0lf / max(est_ms[idx], 0.01lf)) + 1, 1, 128)
                let ms = (v.lcpp
                    ? time_reps_lcpp(queue, pso, lcpp_abuf, lcpp_wblob, lcpp_xbuf, ybuf,
                                     uint3(uint(ntok / 32l), uint(d / 64l), 1u), reps)
                    : time_reps(queue, pso, v.blob36 ? bufs36 : (v.blob34 ? bufs34 : (v.wscT ? bufswsct : (v.f32x ? bufsf32x : (v.f16w ? bufs16 : bufs)))), groups, uint(v.threads), reps))
                if (ms > 0.0lf && ms < best[idx]) {
                    best[idx] = ms
                }
            }
        }
        let macs = double(n * d * ntok)
        var base_gmacs = 0.0lf
        for (v, idx in variants, count()) {
            if (!alive[idx]) {
                continue
            }
            let gmacs = macs / (best[idx] * 1000000.0lf)
            if (idx == 0) {
                base_gmacs = gmacs
            }
            let rel = base_gmacs > 0.0lf ? gmacs / base_gmacs : 0.0lf
            to_log(LOG_INFO, "{v.name}  {sh._0} {best[idx]} ms  {gmacs} GMAC/s  {rel}x vs v0\n")
        }
        metal_release(lcpp_wblob)
        metal_release(lcpp_xbuf)
        metal_release(lcpp_abuf)
        bufsf32x |> clear()                  // pointees released via bufs loop + lcpp releases
        unsafe {
            delete bufsf32x
        }
        metal_release(wsTbuf)
        bufswsct |> clear()
        unsafe {
            delete bufswsct
        }
        metal_release(wblob36)               // bound twice in bufs36 — release exactly once here
        bufs36 |> clear()
        unsafe {
            delete bufs36
        }
        bufs34 |> clear()                    // pointees released via lcpp releases + bufs loop
        unsafe {
            delete bufs34
        }
        metal_release(whbuf)
        bufs16 |> clear()
        unsafe {
            delete bufs16
        }
        delete wh
        for (buf in bufs) {
            metal_release(buf)
        }
        bufs |> clear()                      // pointees released above — clear before delete (house rule)
        unsafe {
            delete bufs
        }
        delete wq
        delete ws
        delete xq
        delete xs
        delete yref
        delete y0
        delete ygot
        delete alive
        delete est_ms
        delete best
    }
    delete shapes
    for (pso in psos) {
        if (pso != null) {
            metal_release(pso)
        }
    }
    psos |> clear()
    unsafe {
        delete psos
    }
    metal_release(queue)
    metal_release(dev)
    assert_no_metal_leaks()
}

[benchmark, unused_argument(b)]   // b is consumed only inside the Apple static_if half
def bench_metal_gemm_kernels(b : B?) {
    if (!jit_enabled()) {
        to_log(LOG_WARNING, "skipped: interpreted (dasLLAMA benchmarks are JIT-only)\n")
        return
    }
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var variants <- [
            LabVariant(name = "v0_prod32", src = metal_q8_gemm_msl, entry = metal_q8_gemm_msl_entry,
                       fastmath = metal_q8_gemm_msl_fastmath, bm = 32, bn = 32),
            LabVariant(name = "v0b_prod64", src = metal_q8_gemm64_msl, entry = metal_q8_gemm64_msl_entry,
                       fastmath = metal_q8_gemm64_msl_fastmath, bm = 64, bn = 64),
            // llama.cpp's kernel_mul_mm_q8_0_f32, byte-verbatim, on its own production contract
            // (block_q8_0 blob + f32 activations); ggml compiles with fast math ON
            LabVariant(name = "lcpp_mulmm", src = LCPP_MUL_MM_Q8_MSL, entry = LCPP_MUL_MM_Q8_ENTRY,
                       fastmath = true, bm = 32, bn = 64, lcpp = true),
            LabVariant(name = "v21_ggmlgeo", src = lab_ggmlgeo_msl, entry = lab_ggmlgeo_msl_entry,
                       fastmath = lab_ggmlgeo_msl_fastmath, bm = 32, bn = 64, grid2d = true),
            // attribution probe: the verbatim kernel MINUS its simdgroup_barrier(mem_none)
            // scheduling hints — if this drops to v21's rate, the hints are the residual gap
            LabVariant(name = "lcpp_nosgbar", src = replace(LCPP_MUL_MM_Q8_MSL, "simdgroup_barrier(mem_flags::mem_none);", ""),
                       entry = LCPP_MUL_MM_Q8_ENTRY, fastmath = true, bm = 32, bn = 64, lcpp = true),
            // v21 with B staged from the pre-dequantized f32 panel — the lcpp B contract on our
            // W path; tests whether in-kernel X dequant is the residual staging tax
            LabVariant(name = "v21f32x", src = lab_ggmlgeof32x_msl, entry = lab_ggmlgeof32x_msl_entry,
                       fastmath = lab_ggmlgeof32x_msl_fastmath, bm = 32, bn = 64, grid2d = true, f32x = true),
            // v22: the Option-A production kernel — v21 geometry, W from the interleaved 36B
            // blob (scale + quants one cacheline, byte4-aligned), X from the f32 panel
            LabVariant(name = "v22_blob36", src = lab_blob36_msl, entry = lab_blob36_msl_entry,
                       fastmath = lab_blob36_msl_fastmath, bm = 32, bn = 64, grid2d = true, blob36 = true),
            // v23: v22's geometry on llama.cpp's OWN 34B blob (half scale, byte-wise quant
            // loads) — das-authored on their exact W contract; isolates the data-contract
            // residue of the verbatim gap from emitter codegen
            LabVariant(name = "v23_blob34", src = lab_blob34_msl, entry = lab_blob34_msl_entry,
                       fastmath = lab_blob34_msl_fastmath, bm = 32, bn = 64, grid2d = true, blob34 = true),
            // v24: hand-MSL probe — v22's emitted text with the math section restructured
            // lcpp-style (matrix arrays + rolled unroll(full) loops + bumped tile pointers);
            // tests the register-pressure/occupancy hypothesis (v22 max_threads 832 vs 1024)
            LabVariant(name = "v24_roll", src = LAB_V22ROLL_MSL, entry = "lab_v22roll",
                       fastmath = true, bm = 32, bn = 64, grid2d = true, blob36 = true),
            LabVariant(name = "v24g_roll34", src = LAB_V24G_MSL, entry = "lab_v24g",
                       fastmath = true, bm = 32, bn = 64, grid2d = true, blob34 = true),
            LabVariant(name = "v24h1_idx", src = LAB_V24H1_MSL, entry = "lab_v24h1",
                       fastmath = true, bm = 32, bn = 64, grid2d = true, blob34 = true),
            // v25: the v24g shape das-AUTHORED via the Phase-7 emitter constructs (matrix
            // arrays + unroll_range) — the emitter-codegen-chase deliverable
            LabVariant(name = "v25_das_rolled", src = lab_rolled34_msl, entry = lab_rolled34_msl_entry,
                       fastmath = lab_rolled34_msl_fastmath, bm = 32, bn = 64, grid2d = true, blob34 = true),
            // v21 + [block][row]-transposed W scales — format-compatible (planar q8 unchanged,
            // scale array re-laid at upload); collapses the A-scale reads to 4 cachelines/tg
            LabVariant(name = "v21sct", src = lab_ggmlgeosct_msl, entry = lab_ggmlgeosct_msl_entry,
                       fastmath = lab_ggmlgeosct_msl_fastmath, bm = 32, bn = 64, grid2d = true, wscT = true),
            // knockout: the verbatim kernel with both staging stores surgically removed (dequant
            // + index math dead-codes away; barriers + math + C store remain) — lcpp's true math
            // ceiling, timing-only
            LabVariant(name = "lcpp_nostage",
                       src = replace(replace(LCPP_MUL_MM_Q8_MSL,
                                             "*(sa + 64*ib + 8*ly + lx) = temp_a[i/4][i%4];", ""),
                                     "*(threadgroup S1_2x4 *)(sb + 64*ib + 8*ly) = (S1_2x4)(*((device T1_2x4 *) y));", ""),
                       entry = LCPP_MUL_MM_Q8_ENTRY, fastmath = true, bm = 32, bn = 64, lcpp = true, check = false),
            LabVariant(name = "v1_n64", src = lab_n64_msl, entry = lab_n64_msl_entry,
                       fastmath = lab_n64_msl_fastmath, bm = 32, bn = 64),
            LabVariant(name = "v2_m64", src = lab_m64_msl, entry = lab_m64_msl_entry,
                       fastmath = lab_m64_msl_fastmath, bm = 64, bn = 32),
            LabVariant(name = "v3_db32", src = lab_db32_msl, entry = lab_db32_msl_entry,
                       fastmath = lab_db32_msl_fastmath, bm = 32, bn = 32),
            LabVariant(name = "v4_k64", src = lab_k64_msl, entry = lab_k64_msl_entry,
                       fastmath = lab_k64_msl_fastmath, bm = 32, bn = 32),
            LabVariant(name = "v5_tile", src = lab_tile32_msl, entry = lab_tile32_msl_entry,
                       fastmath = lab_tile32_msl_fastmath, bm = 32, bn = 32),
            LabVariant(name = "v6_pad36", src = lab_pad36_msl, entry = lab_pad36_msl_entry,
                       fastmath = lab_pad36_msl_fastmath, bm = 32, bn = 32),
            LabVariant(name = "v7_run16", src = lab_run16_msl, entry = lab_run16_msl_entry,
                       fastmath = lab_run16_msl_fastmath, bm = 32, bn = 32),
            LabVariant(name = "v8_vec4", src = lab_vec4_msl, entry = lab_vec4_msl_entry,
                       fastmath = lab_vec4_msl_fastmath, bm = 32, bn = 32),
            LabVariant(name = "v9_vec16", src = lab_vec16_msl, entry = lab_vec16_msl_entry,
                       fastmath = lab_vec16_msl_fastmath, bm = 32, bn = 32),
            LabVariant(name = "v10_dbvec4", src = lab_dbvec4_msl, entry = lab_dbvec4_msl_entry,
                       fastmath = lab_dbvec4_msl_fastmath, bm = 32, bn = 32),
            LabVariant(name = "v11_n64vec4", src = lab_n64vec4_msl, entry = lab_n64vec4_msl_entry,
                       fastmath = lab_n64vec4_msl_fastmath, bm = 32, bn = 64),
            LabVariant(name = "v12_m64vec4", src = lab_m64vec4_msl, entry = lab_m64vec4_msl_entry,
                       fastmath = lab_m64vec4_msl_fastmath, bm = 64, bn = 32),
            LabVariant(name = "v13_6464vec4", src = lab_6464vec4_msl, entry = lab_6464vec4_msl_entry,
                       fastmath = lab_6464vec4_msl_fastmath, bm = 64, bn = 64),
            LabVariant(name = "v18_6464run32", src = lab_6464run32_msl, entry = lab_6464run32_msl_entry,
                       fastmath = lab_6464run32_msl_fastmath, bm = 64, bn = 64),
            LabVariant(name = "v19_6464trans", src = lab_6464trans_msl, entry = lab_6464trans_msl_entry,
                       fastmath = lab_6464trans_msl_fastmath, bm = 64, bn = 64),
            LabVariant(name = "v20_6464devb", src = lab_6464devb_msl, entry = lab_6464devb_msl_entry,
                       fastmath = lab_6464devb_msl_fastmath, bm = 64, bn = 64, f16w = true),
            LabVariant(name = "v15_6464x256", src = lab_6464x256_msl, entry = lab_6464x256_msl_entry,
                       fastmath = lab_6464x256_msl_fastmath, bm = 64, bn = 64, threads = 256),
            LabVariant(name = "v16_6464pf", src = lab_6464pf_msl, entry = lab_6464pf_msl_entry,
                       fastmath = lab_6464pf_msl_fastmath, bm = 64, bn = 64),
            LabVariant(name = "vk_nostage", src = lab_nostage_msl, entry = lab_nostage_msl_entry,
                       fastmath = lab_nostage_msl_fastmath, bm = 32, bn = 32, check = false),
            LabVariant(name = "vk21_nostage", src = lab_ggmlgeons_msl, entry = lab_ggmlgeons_msl_entry,
                       fastmath = lab_ggmlgeons_msl_fastmath, bm = 32, bn = 64, grid2d = true, check = false),
            LabVariant(name = "vk21_aonly", src = lab_ggmlgeoa_msl, entry = lab_ggmlgeoa_msl_entry,
                       fastmath = lab_ggmlgeoa_msl_fastmath, bm = 32, bn = 64, grid2d = true, check = false),
            LabVariant(name = "vk21_noscale", src = lab_ggmlgeonosc_msl, entry = lab_ggmlgeonosc_msl_entry,
                       fastmath = lab_ggmlgeonosc_msl_fastmath, bm = 32, bn = 64, grid2d = true, check = false),
            LabVariant(name = "vk21_bonly", src = lab_ggmlgeob_msl, entry = lab_ggmlgeob_msl_entry,
                       fastmath = lab_ggmlgeob_msl_fastmath, bm = 32, bn = 64, grid2d = true, check = false),
            LabVariant(name = "vk_6464nostage", src = lab_6464nostage_msl, entry = lab_6464nostage_msl_entry,
                       fastmath = lab_6464nostage_msl_fastmath, bm = 64, bn = 64, check = false),
            LabVariant(name = "vk_nomath", src = lab_nomath_msl, entry = lab_nomath_msl_entry,
                       fastmath = lab_nomath_msl_fastmath, bm = 32, bn = 32, check = false)
        ]
        run_lab(b, variants)
        delete variants
    } else {
        to_log(LOG_WARNING, "skipped: das_metal is not part of this build\n")
    }
}
