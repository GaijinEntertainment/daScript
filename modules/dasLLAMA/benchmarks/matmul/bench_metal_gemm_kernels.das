options gen2
options persistent_heap

require dastest/testing_boost
require metal/msl_shader
require ?das_metal metal/das_metal_boost  // nolint:STYLE030 — GPU driver half, Apple builds only
require ?das_metal dasllama/dasllama_math_metal  // nolint:STYLE030 — the production kernel MSL (v0 baseline)
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
def private time_reps(queue; pso; bufs; groups, threads : uint; reps : int) : double {
    var err : string
    var gpu_ms = 0.0lf
    let ok = with_compute_encoder_timed(queue, err, gpu_ms) $(enc) {
        metal_set_pipeline(enc, pso)
        for (bi in range(length(bufs))) {
            metal_set_buffer(enc, bufs[bi], 0ul, bi)
        }
        for (_i in range(reps)) {
            metal_dispatch_threadgroups(enc, uint3(groups, 1u, 1u), uint3(threads, 1u, 1u))
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
        var xqbuf = upload_array(dev, xq, mp_max * n)
        var xsbuf = upload_array(dev, xs, mp_max * nb * 4l)
        var ybuf = metal_new_buffer(dev, uint64(mp_max * d * 4l))
        var kdimbuf = upload_u32(dev, uint(n))
        var ndimbuf = upload_u32(dev, uint(d))
        var bufs <- [wbuf, sbuf, xqbuf, xsbuf, ybuf, kdimbuf, ndimbuf]
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
            if (pso == null || (d % int64(v.bn)) != 0l || !want("DASMETAL_LAB_VARIANTS", v.name)) {
                continue
            }
            let mp = align_up(ntok, int64(v.bm))
            let groups = uint((mp / int64(v.bm)) * (d / int64(v.bn)))
            let ms = time_reps(queue, pso, bufs, groups, uint(v.threads), 1)   // warmup + correctness dispatch
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
                let groups = uint((mp / int64(v.bm)) * (d / int64(v.bn)))
                let reps = clamp(int(25.0lf / max(est_ms[idx], 0.01lf)) + 1, 1, 128)
                let ms = time_reps(queue, pso, bufs, groups, uint(v.threads), reps)
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
            LabVariant(name = "v15_6464x256", src = lab_6464x256_msl, entry = lab_6464x256_msl_entry,
                       fastmath = lab_6464x256_msl_fastmath, bm = 64, bn = 64, threads = 256),
            LabVariant(name = "v16_6464pf", src = lab_6464pf_msl, entry = lab_6464pf_msl_entry,
                       fastmath = lab_6464pf_msl_fastmath, bm = 64, bn = 64),
            LabVariant(name = "vk_nostage", src = lab_nostage_msl, entry = lab_nostage_msl_entry,
                       fastmath = lab_nostage_msl_fastmath, bm = 32, bn = 32, check = false),
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
