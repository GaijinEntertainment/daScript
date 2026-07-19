options gen2
options _comment_hygiene = true

module dasllama_metal_prefill shared public

require math
require strings
require daslib/fio
require metal/msl_shader
require metal/das_metal_boost
require dasllama/dasllama_common
require dasllama/dasllama_math
require dasllama/dasllama_math_metal
require dasllama/dasllama_metal_common   // decline enums/tables + the shared block_q8_0 blob cache (W1 split)

// Phase-6 dasMetal: the full-GPU-resident prefill kernel set — everything between the Q8 GEMMs
// that Phase 5c left on the CPU: activation requant, rmsnorm, table-driven NORM RoPE, fused
// causal-softmax attention scores, the P·V gather, fused SwiGLU, and the residual add. Each
// kernel mirrors its dasLLAMA CPU twin's math exactly (same formula, same rounding ops):
//   metal_q8_quant      ~ quantize_q8_0_into_ptr   (amax/127 scale, round-half-away quants)
//   metal_rmsnorm       ~ rmsnorm                  (w[i] * x[i] * rsqrt(mean(x^2) + eps))
//   metal_rope          ~ rope_scaled_tab          (NORM adjacent pairs; cos/sin table rows)
//   metal_attn_qk/_softmax/_av ~ the classic per-head causal scores + softmax + P·V, as a
//                         tiled sgmat GEMM trio over np32-padded per-head score slabs
//   metal_swiglu        ~ swiglu                   (g = g/(1+exp(-g)) * u)
//   metal_add           ~ add_inplace
// Reductions ride simd_sum / a max butterfly over simd_shuffle_xor with hardcoded masks 16..1 —
// Apple-GPU simdgroups are width 32 (asserted at driver init). Kernels with barriers/simd ops are
// NOT sequentially CPU-replayable; their oracles are the dasLLAMA CPU functions themselves
// (modules/dasLLAMA/tests/test_metal_prefill_kernels.das).

// One thread per 32-element block of one activation row: amax -> d = amax/127 -> q = round(x*id).
// Grid covers ntok*(n/32) blocks (uniform nblk guards the ceil-padded dispatch); pad rows of the
// resident xq/xs image are never written here — the GEMM's C-block row independence keeps stale pad rows out.
class MetalQ8Quant {
    @ssbo @binding = 0 x : array<float>     // [ntok x n] activations
    @ssbo @binding = 1 xq : array<int8>     // [mp x n] quants (live rows only)
    @ssbo @binding = 2 xs : array<float>    // [mp x n/32] scales (live rows only)
    @uniform @binding = 3 n : uint          // row width, % 32
    @uniform @binding = 4 nblk : uint       // ntok * n/32 — grid guard

    [metal_kernel(name="metal_q8_quant_msl")]
    def metal_q8_quant {
        let gid = gl_GlobalInvocationID.x
        if (gid >= nblk) {
            return
        }
        let nb = n / 32u
        let row = gid / nb
        let cb = gid % nb
        let base = row * n + cb * 32u
        var amax = 0.0
        var k = 0u
        while (k < 32u) {
            amax = max(amax, abs(x[base + k]))
            k++
        }
        let d = amax / 127.0
        let id = d != 0.0 ? 1.0 / d : 0.0
        xs[row * nb + cb] = d
        k = 0u
        while (k < 32u) {
            xq[base + k] = int8(round(x[base + k] * id))
            k++
        }
    }
}

// One threadgroup per row: cooperative sum of squares (simd_sum + per-simdgroup partials),
// lane 0 folds partials into rsqrt(mean + eps), every thread scales its strided slice.
class MetalRmsNorm {
    @ssbo @binding = 0 x : array<float>     // [nrows x dim] source
    @ssbo @binding = 1 w : array<float>     // [dim] weight
    @ssbo @binding = 2 y : array<float>     // [nrows x dim] out (x aliasing is safe: x is fully
                                            // read before the barrier that precedes any write)
    @uniform @binding = 3 dim : uint
    @uniform @binding = 4 eps : float
    @workgroup partial : float[32]          // one per simdgroup — width-agnostic up to 1024
                                            // threads (prefill dispatches 256, decode 1024)
    @workgroup inv_sh : float

    [metal_kernel(name="metal_rmsnorm_msl")]
    def metal_rmsnorm {
        let row = gl_WorkGroupID.x
        let lid = gl_LocalInvocationID.x
        var ss = 0.0
        var k = lid
        while (k < dim) {
            let vv = x[row * dim + k]
            ss += vv * vv
            k += gl_WorkGroupSize.x
        }
        ss = simd_sum(ss)
        if (gl_SubgroupInvocationID == 0u) {
            partial[gl_SubgroupID] = ss
        }
        barrier()
        if (lid == 0u) {
            var tot = 0.0
            var sgi = 0u
            while (sgi < gl_NumSubgroups) {
                tot += partial[sgi]
                sgi++
            }
            inv_sh = rsqrt(tot / float(dim) + eps)
        }
        barrier()
        let inv = inv_sh
        k = lid
        while (k < dim) {
            y[row * dim + k] = w[k] * (x[row * dim + k] * inv)
            k += gl_WorkGroupSize.x
        }
    }
}

// The production GEMM: 64-output x 32-token C tiles (4 simdgroups, 8 accumulators), W as
// interleaved 34B q8 blocks, raw f32 activations. Rolled [unroll_full] math over simdgroup-matrix
// arrays. Requires out-dim % 64, M padded to 64. Beats llama.cpp's equivalent 2-3% on M1 Max.
class MetalQ8MulMm {
    @ssbo @binding = 0 wsh : array<float16> // 34B-block W blob, HALF-SCALE view: scale of block b at 17*b
    @ssbo @binding = 1 wqb : array<int8>    // the SAME blob buffer, BYTE view: quants of block b at 34*b + 2
    @ssbo @binding = 2 xf : array<float4>   // raw f32 activations, float4 view
    @ssbo @binding = 3 y : array<float>
    @uniform @binding = 4 kdim : uint
    @uniform @binding = 5 ndim : uint
    @workgroup ta : float16[2048]           // W: 4 k-tiles x 8 row-tiles of 8x8, each tile (k row, wrow col)
    @workgroup tb : float16[1024]           // X: 4 k-tiles x 4 token-tiles of 8x8, each tile (token row, k col)

    [metal_kernel(name="metal_q8_mulmm_msl")]
    def metal_q8_mulmm {
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
        // device streams walk as REAL das pointers (unsafe(addr()) + bump at the loop tail):
        // pointer-vs-index register-allocates differently on AGX and is shape-dependent — this
        // kernel measured pointer (llama.cpp's loop reads its scale straight off a carried pointer)
        let blk0 = (nBase + lr0) * nkb
        var scur = unsafe(addr(wsh[blk0 * 17u]))                    // half scale rides the quants' cacheline
        var qp = unsafe(addr(wqb[blk0 * 34u + 2u + il0 * 16u]))
        var xcur = unsafe(addr(xf[(mBase + lr1) * kdim4 + iy / 4u]))
        var kb = 0u
        while (kb < nkb) {
            // device reads + dequant first — they fly while the previous math loop drains
            let sA = float(unsafe(scur[0]))
            let b0 = unsafe(xcur[0])
            let b1 = unsafe(xcur[1])
            for [unroll_full] (i in range(16)) {
                va[i] = float16(float(unsafe(qp[i])) * sA)
            }
            barrier()                       // previous math done reading the tiles
            for [unroll_full] (i in range(16)) {
                ta[tA0 + uint(i / 8) * 512u + uint(i % 8) * 8u] = va[i]
            }
            tg_store_half4(tb, ibB, b0)     // no dequant on B — pure convert-and-store
            tg_store_half4(tb, ibB + 4u, b1)
            barrier()
            // math ROLLED: the pragma'd loops reach the backend intact — 4 k-slabs x
            // (6 tile loads + 8 macs); tile POINTERS bump at the slab tail (index form
            // measured max_threads 896 and -10% — the backend precomputes every address)
            var acur = unsafe(addr(ta[aB]))
            var bcur = unsafe(addr(tb[bB]))
            for [unroll_full] (_ik in range(4)) {
                for [unroll_full] (i in range(4)) {
                    simdgroup_load(ma[i], acur, uint(i) * 64u, 8u)
                }
                for [unroll_full] (i in range(2)) {
                    simdgroup_load(mb[i], bcur, uint(i) * 64u, 8u)
                }
                for [unroll_full] (i in range(8)) {
                    simdgroup_multiply_accumulate(mc[i], mb[i / 4], ma[i % 4], mc[i])
                }
                acur = unsafe(acur + 512)
                bcur = unsafe(bcur + 256)
            }
            scur = unsafe(scur + 17)
            qp = unsafe(qp + 34)
            xcur = unsafe(xcur + 8)
            kb++
        }
        let row0 = mBase + (sg / 2u) * 16u  // accumulator rows = tokens
        let col0 = nBase + (sg % 2u) * 32u  // accumulator cols = outputs
        for [unroll_full] (i in range(8)) {
            simdgroup_store(mc[i], y, (row0 + uint(i / 4) * 8u) * ndim + col0 + uint(i % 4) * 8u, ndim)
        }
    }
}

// ===== K-quant mul_mm twins (format-matrix W4 / M2) — MetalQ8MulMm with the A-tile stage
// swapped to superblock dequant. The 32-elem k-block kb IS sub-block kb%8 of superblock kb/8,
// so each thread's 16-elem half decodes ONE (scale, min) pair. Same %64 shape gates, M padded 32/64.

class MetalKqMulMmK4 {
    @ssbo @binding = 0 ksh : array<float16> // 16B compact scale blocks, half view (d at 8*blk, dmin at 8*blk+1)
    @ssbo @binding = 1 ks4 : array<uint4>   // the same buffer, uint4 view
    @ssbo @binding = 2 kqu : array<uint>    // k4 quant plane, uint view (qs of blk at 32*blk)
    @ssbo @binding = 3 xf : array<float4>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 kdim : uint
    @uniform @binding = 6 ndim : uint
    @workgroup ta : float16[2048]
    @workgroup tb : float16[1024]

    [metal_kernel(name="metal_kq_mulmm_k4_msl")]
    def metal_kq_mulmm_k4 {
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let mBase = gl_WorkGroupID.x * 32u
        let nBase = gl_WorkGroupID.y * 64u
        var mc : simdgroup_float8x8[8]
        var ma : simdgroup_half8x8[4]
        var mb : simdgroup_half8x8[2]
        var va : float16[16]
        let kdim4 = kdim / 4u
        let nkb = kdim / 32u
        let lr0 = lid / 2u
        let il0 = lid % 2u
        let syA = lr0 / 8u
        let lxA = lr0 % 8u
        let tA0 = (il0 * 2u * 8u + syA) * 64u + lxA
        let lr1 = lid / 4u
        let iy = (lid % 4u) * 8u
        let ibB = ((lid % 4u) * 4u + lr1 / 8u) * 64u + (lr1 % 8u) * 8u
        let aB = (sg % 2u) * 256u
        let bB = (sg / 2u) * 128u
        let nsb = kdim / 256u
        let wrow = nBase + lr0
        var xcur = unsafe(addr(xf[(mBase + lr1) * kdim4 + iy / 4u]))
        var kb = 0u
        while (kb < nkb) {
            let sb = kb / 8u
            let js = kb % 8u
            let blk = wrow * nsb + sb
            let b0 = unsafe(xcur[0])
            let b1 = unsafe(xcur[1])
            let sv = ks4[blk]
            var sc = 0u
            var mn = 0u
            if (js < 4u) {
                sc = (sv.y >> (8u * js)) & 63u
                mn = (sv.z >> (8u * js)) & 63u
            } else {
                let j2 = js - 4u
                let hi = sv.w >> (8u * j2)
                sc = (hi & 15u) | (((sv.y >> (8u * j2 + 6u)) & 3u) << 4u)
                mn = ((hi >> 4u) & 15u) | (((sv.z >> (8u * j2 + 6u)) & 3u) << 4u)
            }
            let dsc = float(ksh[blk * 8u]) * float(sc)
            let dmn = float(ksh[blk * 8u + 1u]) * float(mn)
            let qb = blk * 32u + (js / 2u) * 8u + il0 * 4u
            let nsh = (js & 1u) * 4u
            for [unroll_full] (k in range(4)) {
                let u = kqu[qb + uint(k)]
                for [unroll_full] (c in range(4)) {
                    let q = (u >> (8u * uint(c) + nsh)) & 15u
                    va[k * 4 + c] = float16(dsc * float(q) - dmn)
                }
            }
            barrier()
            for [unroll_full] (i in range(16)) {
                ta[tA0 + uint(i / 8) * 512u + uint(i % 8) * 8u] = va[i]
            }
            tg_store_half4(tb, ibB, b0)
            tg_store_half4(tb, ibB + 4u, b1)
            barrier()
            var acur = unsafe(addr(ta[aB]))
            var bcur = unsafe(addr(tb[bB]))
            for [unroll_full] (_ik in range(4)) {
                for [unroll_full] (i in range(4)) {
                    simdgroup_load(ma[i], acur, uint(i) * 64u, 8u)
                }
                for [unroll_full] (i in range(2)) {
                    simdgroup_load(mb[i], bcur, uint(i) * 64u, 8u)
                }
                for [unroll_full] (i in range(8)) {
                    simdgroup_multiply_accumulate(mc[i], mb[i / 4], ma[i % 4], mc[i])
                }
                acur = unsafe(acur + 512)
                bcur = unsafe(bcur + 256)
            }
            xcur = unsafe(xcur + 8)
            kb++
        }
        let row0 = mBase + (sg / 2u) * 16u
        let col0 = nBase + (sg % 2u) * 32u
        for [unroll_full] (i in range(8)) {
            simdgroup_store(mc[i], y, (row0 + uint(i / 4) * 8u) * ndim + col0 + uint(i % 4) * 8u, ndim)
        }
    }
}

class MetalKqMulMmK5 {
    @ssbo @binding = 0 ksh : array<float16> // 16B compact scale blocks (the Q4_K form)
    @ssbo @binding = 1 ks4 : array<uint4>
    @ssbo @binding = 2 kqu : array<uint>    // k5 quant plane, uint view (qs at 40*blk, qh at 40*blk+32)
    @ssbo @binding = 3 xf : array<float4>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 kdim : uint
    @uniform @binding = 6 ndim : uint
    @workgroup ta : float16[2048]
    @workgroup tb : float16[1024]

    [metal_kernel(name="metal_kq_mulmm_k5_msl")]
    def metal_kq_mulmm_k5 {
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let mBase = gl_WorkGroupID.x * 32u
        let nBase = gl_WorkGroupID.y * 64u
        var mc : simdgroup_float8x8[8]
        var ma : simdgroup_half8x8[4]
        var mb : simdgroup_half8x8[2]
        var va : float16[16]
        let kdim4 = kdim / 4u
        let nkb = kdim / 32u
        let lr0 = lid / 2u
        let il0 = lid % 2u
        let syA = lr0 / 8u
        let lxA = lr0 % 8u
        let tA0 = (il0 * 2u * 8u + syA) * 64u + lxA
        let lr1 = lid / 4u
        let iy = (lid % 4u) * 8u
        let ibB = ((lid % 4u) * 4u + lr1 / 8u) * 64u + (lr1 % 8u) * 8u
        let aB = (sg % 2u) * 256u
        let bB = (sg / 2u) * 128u
        let nsb = kdim / 256u
        let wrow = nBase + lr0
        var xcur = unsafe(addr(xf[(mBase + lr1) * kdim4 + iy / 4u]))
        var kb = 0u
        while (kb < nkb) {
            let sb = kb / 8u
            let js = kb % 8u
            let blk = wrow * nsb + sb
            let b0 = unsafe(xcur[0])
            let b1 = unsafe(xcur[1])
            let sv = ks4[blk]
            var sc = 0u
            var mn = 0u
            if (js < 4u) {
                sc = (sv.y >> (8u * js)) & 63u
                mn = (sv.z >> (8u * js)) & 63u
            } else {
                let j2 = js - 4u
                let hi = sv.w >> (8u * j2)
                sc = (hi & 15u) | (((sv.y >> (8u * j2 + 6u)) & 3u) << 4u)
                mn = ((hi >> 4u) & 15u) | (((sv.z >> (8u * j2 + 6u)) & 3u) << 4u)
            }
            let dsc = float(ksh[blk * 8u]) * float(sc)
            let dmn = float(ksh[blk * 8u + 1u]) * float(mn)
            let qb = blk * 40u + (js / 2u) * 8u + il0 * 4u
            let hb = blk * 40u + 32u + il0 * 4u
            let nsh = (js & 1u) * 4u
            for [unroll_full] (k in range(4)) {
                let u = kqu[qb + uint(k)]
                let hu = kqu[hb + uint(k)]
                for [unroll_full] (c in range(4)) {
                    let q = ((u >> (8u * uint(c) + nsh)) & 15u) | (((hu >> (8u * uint(c) + js)) & 1u) << 4u)
                    va[k * 4 + c] = float16(dsc * float(q) - dmn)
                }
            }
            barrier()
            for [unroll_full] (i in range(16)) {
                ta[tA0 + uint(i / 8) * 512u + uint(i % 8) * 8u] = va[i]
            }
            tg_store_half4(tb, ibB, b0)
            tg_store_half4(tb, ibB + 4u, b1)
            barrier()
            var acur = unsafe(addr(ta[aB]))
            var bcur = unsafe(addr(tb[bB]))
            for [unroll_full] (_ik in range(4)) {
                for [unroll_full] (i in range(4)) {
                    simdgroup_load(ma[i], acur, uint(i) * 64u, 8u)
                }
                for [unroll_full] (i in range(2)) {
                    simdgroup_load(mb[i], bcur, uint(i) * 64u, 8u)
                }
                for [unroll_full] (i in range(8)) {
                    simdgroup_multiply_accumulate(mc[i], mb[i / 4], ma[i % 4], mc[i])
                }
                acur = unsafe(acur + 512)
                bcur = unsafe(bcur + 256)
            }
            xcur = unsafe(xcur + 8)
            kb++
        }
        let row0 = mBase + (sg / 2u) * 16u
        let col0 = nBase + (sg % 2u) * 32u
        for [unroll_full] (i in range(8)) {
            simdgroup_store(mc[i], y, (row0 + uint(i / 4) * 8u) * ndim + col0 + uint(i % 4) * 8u, ndim)
        }
    }
}

class MetalKqMulMmK6 {
    @ssbo @binding = 0 kdh : array<float16> // the d plane — the scale buffer bound at byte nsb*16
    @ssbo @binding = 1 ksc : array<uint4>   // 16B sub-scale blocks (16 int8 at blk), same buffer at 0
    @ssbo @binding = 2 kqu : array<uint>    // k6 quant plane, uint view (ql at 48*blk, qh at 48*blk+32)
    @ssbo @binding = 3 xf : array<float4>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 kdim : uint
    @uniform @binding = 6 ndim : uint
    @workgroup ta : float16[2048]
    @workgroup tb : float16[1024]

    [metal_kernel(name="metal_kq_mulmm_k6_msl")]
    def metal_kq_mulmm_k6 {
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let mBase = gl_WorkGroupID.x * 32u
        let nBase = gl_WorkGroupID.y * 64u
        var mc : simdgroup_float8x8[8]
        var ma : simdgroup_half8x8[4]
        var mb : simdgroup_half8x8[2]
        var va : float16[16]
        let kdim4 = kdim / 4u
        let nkb = kdim / 32u
        let lr0 = lid / 2u
        let il0 = lid % 2u
        let syA = lr0 / 8u
        let lxA = lr0 % 8u
        let tA0 = (il0 * 2u * 8u + syA) * 64u + lxA
        let lr1 = lid / 4u
        let iy = (lid % 4u) * 8u
        let ibB = ((lid % 4u) * 4u + lr1 / 8u) * 64u + (lr1 % 8u) * 8u
        let aB = (sg % 2u) * 256u
        let bB = (sg / 2u) * 128u
        let nsb = kdim / 256u
        let wrow = nBase + lr0
        var xcur = unsafe(addr(xf[(mBase + lr1) * kdim4 + iy / 4u]))
        var kb = 0u
        while (kb < nkb) {
            let sb = kb / 8u
            let js = kb % 8u
            let blk = wrow * nsb + sb
            let b0 = unsafe(xcur[0])
            let b1 = unsafe(xcur[1])
            let half6 = js / 4u
            let gg = js % 4u
            let sv = ksc[blk]
            let si = half6 * 8u + gg * 2u + il0
            let scw = si < 8u ? (si < 4u ? sv.x : sv.y) : (si < 12u ? sv.z : sv.w)
            let s6 = float(((int(scw >> ((si & 3u) * 8u)) & 255) ^ 128) - 128)
            let dsc = float(kdh[blk]) * s6
            let dmn = dsc * 32.0   // -32 folded to the fma form (k4's dmn shape)
            let qb = blk * 48u + half6 * 16u + (gg & 1u) * 8u + il0 * 4u
            let hb = blk * 48u + 32u + half6 * 8u + il0 * 4u
            let nsh = (gg / 2u) * 4u
            let hsh = gg * 2u
            for [unroll_full] (k in range(4)) {
                let u = kqu[qb + uint(k)]
                let hu = kqu[hb + uint(k)]
                // all 4 bytes composed at once — cross-byte bleed of the uint-wide shifts is
                // masked off; per element only shift+mask+fma remain
                let qv = ((u >> nsh) & 0x0F0F0F0F) | (((hu >> hsh) & 0x03030303) << 4u)
                for [unroll_full] (c in range(4)) {
                    let q = (qv >> (8u * uint(c))) & 255u
                    va[k * 4 + c] = float16(dsc * float(q) - dmn)
                }
            }
            barrier()
            for [unroll_full] (i in range(16)) {
                ta[tA0 + uint(i / 8) * 512u + uint(i % 8) * 8u] = va[i]
            }
            tg_store_half4(tb, ibB, b0)
            tg_store_half4(tb, ibB + 4u, b1)
            barrier()
            var acur = unsafe(addr(ta[aB]))
            var bcur = unsafe(addr(tb[bB]))
            for [unroll_full] (_ik in range(4)) {
                for [unroll_full] (i in range(4)) {
                    simdgroup_load(ma[i], acur, uint(i) * 64u, 8u)
                }
                for [unroll_full] (i in range(2)) {
                    simdgroup_load(mb[i], bcur, uint(i) * 64u, 8u)
                }
                for [unroll_full] (i in range(8)) {
                    simdgroup_multiply_accumulate(mc[i], mb[i / 4], ma[i % 4], mc[i])
                }
                acur = unsafe(acur + 512)
                bcur = unsafe(bcur + 256)
            }
            xcur = unsafe(xcur + 8)
            kb++
        }
        let row0 = mBase + (sg / 2u) * 16u
        let col0 = nBase + (sg % 2u) * 32u
        for [unroll_full] (i in range(8)) {
            simdgroup_store(mc[i], y, (row0 + uint(i / 4) * 8u) * ndim + col0 + uint(i % 4) * 8u, ndim)
        }
    }
}

// One simdgroup per output row: the classifier GEMV — a dim-long Q8 row dot one f32 vector.
// Bandwidth-bound on the [vocab x dim] weight stream; x stays cache-resident across rows. Lanes
// stride byte4s, each byte4's int dot rides its block's scale. 128 threads = 4 rows per group.
class MetalQ8Gemv {
    @ssbo @binding = 0 wq : array<byte4>    // [d x n] Q8 rows, byte4 view
    @ssbo @binding = 1 ws : array<float>    // [d x n/32] scales
    @ssbo @binding = 2 x : array<float4>    // [n] f32 vector, float4 view
    @ssbo @binding = 3 y : array<float>     // [d] logits
    @uniform @binding = 4 ndim : uint       // n (reduction dim, % 32)
    @uniform @binding = 5 ddim : uint       // d (output rows)

    [metal_kernel(name="metal_q8_gemv_msl")]
    def metal_q8_gemv {
        let row = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        if (row >= ddim) {
            return
        }
        let lane = gl_SubgroupInvocationID
        let nb4 = ndim / 4u
        let wbase = row * nb4
        let sbase = row * (ndim / 32u)
        var acc = 0.0
        var k = lane
        // 4 blocks-in-flight per iteration (128 byte4s per simdgroup step) — amortizes the
        // loop overhead on the bandwidth-bound weight stream; the plain loop drains the tail
        while (k + 96u < nb4) {
            let wa = float4(int4(wq[wbase + k]))
            let xa = x[k]
            let wb = float4(int4(wq[wbase + (k + 32u)]))
            let xb = x[k + 32u]
            let wc = float4(int4(wq[wbase + (k + 64u)]))
            let xc = x[k + 64u]
            let wd = float4(int4(wq[wbase + (k + 96u)]))
            let xd = x[k + 96u]
            acc += (wa.x * xa.x + wa.y * xa.y + wa.z * xa.z + wa.w * xa.w) * ws[sbase + k / 8u]
            acc += (wb.x * xb.x + wb.y * xb.y + wb.z * xb.z + wb.w * xb.w) * ws[sbase + (k + 32u) / 8u]
            acc += (wc.x * xc.x + wc.y * xc.y + wc.z * xc.z + wc.w * xc.w) * ws[sbase + (k + 64u) / 8u]
            acc += (wd.x * xd.x + wd.y * xd.y + wd.z * xd.z + wd.w * xd.w) * ws[sbase + (k + 96u) / 8u]
            k += 128u
        }
        while (k < nb4) {
            let w4 = float4(int4(wq[wbase + k]))
            let x4 = x[k]
            acc += (w4.x * x4.x + w4.y * x4.y + w4.z * x4.z + w4.w * x4.w) * ws[sbase + k / 8u]
            k += gl_SubgroupSize
        }
        acc = simd_sum(acc)
        if (lane == 0u) {
            y[row] = acc
        }
    }
}

// The seed GEMV's f16-SCALE twin (wscale_f16 models — the qscales16 halfword plane): one
// simdgroup per output row, 4 blocks in flight. 1.0625 vs 1.125 bytes/weight — ~5.6% less stream.
// (A uint4 wide-load variant measured 0.5ms/token SLOWER on M1 — don't re-try without new silicon.)
class MetalQ8GemvS16 {
    @ssbo @binding = 0 wq : array<byte4>    // [d x n] Q8 rows, byte4 view
    @ssbo @binding = 1 ws : array<float16>  // [d x n/32] scales, raw binary16
    @ssbo @binding = 2 x : array<float4>    // [n] f32 vector, float4 view
    @ssbo @binding = 3 y : array<float>     // [d]
    @uniform @binding = 4 ndim : uint       // n (reduction dim, % 32)
    @uniform @binding = 5 ddim : uint       // d (output rows)

    [metal_kernel(name="metal_q8_gemv_s16_msl")]
    def metal_q8_gemv_s16 {
        let row = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        if (row >= ddim) {
            return
        }
        let lane = gl_SubgroupInvocationID
        let nb4 = ndim / 4u
        let wbase = row * nb4
        let sbase = row * (ndim / 32u)
        var acc = 0.0
        var k = lane
        while (k + 96u < nb4) {
            let wa = float4(int4(wq[wbase + k]))
            let xa = x[k]
            let wb = float4(int4(wq[wbase + (k + 32u)]))
            let xb = x[k + 32u]
            let wc = float4(int4(wq[wbase + (k + 64u)]))
            let xc = x[k + 64u]
            let wd = float4(int4(wq[wbase + (k + 96u)]))
            let xd = x[k + 96u]
            acc += (wa.x * xa.x + wa.y * xa.y + wa.z * xa.z + wa.w * xa.w) * float(ws[sbase + k / 8u])
            acc += (wb.x * xb.x + wb.y * xb.y + wb.z * xb.z + wb.w * xb.w) * float(ws[sbase + (k + 32u) / 8u])
            acc += (wc.x * xc.x + wc.y * xc.y + wc.z * xc.z + wc.w * xc.w) * float(ws[sbase + (k + 64u) / 8u])
            acc += (wd.x * xd.x + wd.y * xd.y + wd.z * xd.z + wd.w * xd.w) * float(ws[sbase + (k + 96u) / 8u])
            k += 128u
        }
        while (k < nb4) {
            let w4 = float4(int4(wq[wbase + k]))
            let x4 = x[k]
            acc += (w4.x * x4.x + w4.y * x4.y + w4.z * x4.z + w4.w * x4.w) * float(ws[sbase + k / 8u])
            k += gl_SubgroupSize
        }
        acc = simd_sum(acc)
        if (lane == 0u) {
            y[row] = acc
        }
    }
}

// ===== K-quant site GEMVs (format-matrix W4) — the gemv-lab winners, productionized =====
// f32-X single-row weight streams over DISK-ORDER kq quant planes + GPU-form scale buffers:
// k4/k5 read 16B compact scale blocks, k6 reads 16B sub-scale + a trailing packed f16 d plane.

// Q4_K: lane owns 32 elems of every 4th superblock; lcpp's kmask halfword scale decode +
// nibble-mask acc folds (x16/x256 groups folded by pow2 constants).
class MetalKqGemvK4 {
    @ssbo @binding = 0 ksh : array<float16> // 16B scale blocks, half view (d at 8*blk, dmin at 8*blk+1)
    @ssbo @binding = 1 ks4 : array<uint4>   // the same buffer, uint4 view (whole block at blk)
    @ssbo @binding = 2 kqu : array<uint>    // k4 quant plane, uint view (qs of blk at 32*blk)
    @ssbo @binding = 3 x : array<float>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint

    [metal_kernel(name="metal_kq_gemv_k4_msl")]
    def metal_kq_gemv_k4 {
        let lane = gl_SubgroupInvocationID
        let ix = lane / 8u
        let it = lane % 8u
        let iq = it / 4u
        let ir = it % 4u
        let nb = ndim / 256u
        let first_row = (gl_WorkGroupID.x * 2u + gl_SubgroupID) * 2u
        var yl : float[16]
        var yh : float[16]
        var sumf : float[2]
        var ib = ix
        while (ib < nb) {
            let yb = ib * 256u + iq * 64u + ir * 8u
            var sumy : float[4]
            for [unroll_full] (i in range(8)) {
                yl[i] = x[yb + uint(i)]
                sumy[0] += yl[i]
                yl[i + 8] = x[yb + 32u + uint(i)]
                sumy[1] += yl[i + 8]
                yh[i] = x[yb + 128u + uint(i)]
                sumy[2] += yh[i]
                yh[i + 8] = x[yb + 160u + uint(i)]
                sumy[3] += yh[i + 8]
            }
            for [unroll_full] (r in range(2)) {
                let blk = (first_row + uint(r)) * nb + ib
                let sv = ks4[blk]
                let hw0 = (sv.y >> (16u * iq)) & 0xFFFF
                let hw2 = (sv.z >> (16u * iq)) & 0xFFFF
                let hw4 = (sv.w >> (16u * iq)) & 0xFFFF
                let sc0 = hw0 & 0x3F3F
                let sc1 = hw2 & 0x3F3F
                let sc2 = (hw4 & 0x0F0F) | ((hw0 & 0xC0C0) >> 2u)
                let sc3 = ((hw4 >> 4u) & 0x0F0F) | ((hw2 & 0xC0C0) >> 2u)
                let qb = blk * 32u + iq * 8u + ir * 2u
                let q1a = kqu[qb]
                let q1b = kqu[qb + 1u]
                let q2a = kqu[qb + 16u]
                let q2b = kqu[qb + 17u]
                var acc1 : float[4]
                var acc2 : float[4]
                for [unroll_full] (i in range(4)) {
                    let sh = 16u * (uint(i) & 1u)
                    let q1 = ((i < 2 ? q1a : q1b) >> sh) & 0xFFFF
                    let q2 = ((i < 2 ? q2a : q2b) >> sh) & 0xFFFF
                    acc1[0] += yl[2 * i] * float(q1 & 0x000F)
                    acc1[1] += yl[2 * i + 1] * float(q1 & 0x0F00)
                    acc1[2] += yl[2 * i + 8] * float(q1 & 0x00F0)
                    acc1[3] += yl[2 * i + 9] * float(q1 & 0xF000)
                    acc2[0] += yh[2 * i] * float(q2 & 0x000F)
                    acc2[1] += yh[2 * i + 1] * float(q2 & 0x0F00)
                    acc2[2] += yh[2 * i + 8] * float(q2 & 0x00F0)
                    acc2[3] += yh[2 * i + 9] * float(q2 & 0xF000)
                }
                let dd = float(ksh[blk * 8u])
                let dm = float(ksh[blk * 8u + 1u])
                sumf[r] += dd * ((acc1[0] + 0.00390625 * acc1[1]) * float(sc0 & 255u)
                    + (acc1[2] + 0.00390625 * acc1[3]) * float(sc0 >> 8u) * 0.0625
                    + (acc2[0] + 0.00390625 * acc2[1]) * float(sc2 & 255u)
                    + (acc2[2] + 0.00390625 * acc2[3]) * float(sc2 >> 8u) * 0.0625)
                sumf[r] -= dm * (sumy[0] * float(sc1 & 255u) + sumy[1] * float(sc1 >> 8u)
                    + sumy[2] * float(sc3 & 255u) + sumy[3] * float(sc3 >> 8u))
            }
            ib += 4u
        }
        for [unroll_full] (r in range(2)) {
            let s = simd_sum(sumf[r])
            if (lane == 0u && first_row + uint(r) < ddim) {
                y[first_row + uint(r)] = s
            }
        }
    }
}

// Q5_K: int-composed q5 (nibble | qh bit << 4) — one fma per element; same kmask scale decode.
// The quant plane is [qs 128][qh 32] per superblock, read through the byte view.
class MetalKqGemvK5 {
    @ssbo @binding = 0 ksh : array<float16> // 16B scale blocks, half view
    @ssbo @binding = 1 ks4 : array<uint4>   // the same buffer, uint4 view
    @ssbo @binding = 2 kqb : array<uint8>   // k5 quant plane, byte view (qs at 160*blk, qh at +128)
    @ssbo @binding = 3 x : array<float>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint

    [metal_kernel(name="metal_kq_gemv_k5_msl")]
    def metal_kq_gemv_k5 {
        let lane = gl_SubgroupInvocationID
        let tid = lane / 4u
        let ix = lane % 4u
        let iq = tid / 4u
        let ir = tid % 4u
        let l0 = ir * 8u
        let hb0 = 2u * iq
        let nb = ndim / 256u
        let first_row = (gl_WorkGroupID.x * 2u + gl_SubgroupID) * 2u
        var yl : float[16]
        var yh : float[16]
        var sumf : float[2]
        var ib = ix
        while (ib < nb) {
            let yb = ib * 256u + iq * 64u + l0
            var sumy : float[4]
            for [unroll_full] (l in range(8)) {
                yl[l] = x[yb + uint(l)]
                sumy[0] += yl[l]
                yl[l + 8] = x[yb + 32u + uint(l)]
                sumy[1] += yl[l + 8]
                yh[l] = x[yb + 128u + uint(l)]
                sumy[2] += yh[l]
                yh[l + 8] = x[yb + 160u + uint(l)]
                sumy[3] += yh[l + 8]
            }
            for [unroll_full] (r in range(2)) {
                let blk = (first_row + uint(r)) * nb + ib
                let sv = ks4[blk]
                let hw0 = (sv.y >> (16u * iq)) & 0xFFFF
                let hw2 = (sv.z >> (16u * iq)) & 0xFFFF
                let hw4 = (sv.w >> (16u * iq)) & 0xFFFF
                let sc0 = hw0 & 0x3F3F
                let sc1 = hw2 & 0x3F3F
                let sc2 = (hw4 & 0x0F0F) | ((hw0 & 0xC0C0) >> 2u)
                let sc3 = ((hw4 >> 4u) & 0x0F0F) | ((hw2 & 0xC0C0) >> 2u)
                let qs0 = blk * 160u + iq * 32u + l0
                let qh0 = blk * 160u + 128u + l0
                var acc : float[4]
                for [unroll_full] (l in range(8)) {
                    let q1 = uint(kqb[qs0 + uint(l)])
                    let q2 = uint(kqb[qs0 + 64u + uint(l)])
                    let h = uint(kqb[qh0 + uint(l)]) >> hb0
                    acc[0] += yl[l] * float((q1 & 0x0F) | ((h & 1u) << 4u))
                    acc[1] += yl[l + 8] * float((q1 >> 4u) | ((h & 2u) << 3u))
                    acc[2] += yh[l] * float((q2 & 0x0F) | ((h & 16u) << 0u))
                    acc[3] += yh[l + 8] * float((q2 >> 4u) | ((h & 32u) >> 1u))
                }
                let dd = float(ksh[blk * 8u])
                let dm = float(ksh[blk * 8u + 1u])
                sumf[r] += dd * (float(sc0 & 255u) * acc[0] + float(sc0 >> 8u) * acc[1]
                    + float(sc2 & 255u) * acc[2] + float(sc2 >> 8u) * acc[3])
                sumf[r] -= dm * (sumy[0] * float(sc1 & 255u) + sumy[1] * float(sc1 >> 8u)
                    + sumy[2] * float(sc3 & 255u) + sumy[3] * float(sc3 >> 8u))
            }
            ib += 4u
        }
        for [unroll_full] (r in range(2)) {
            let s = simd_sum(sumf[r])
            if (lane == 0u && first_row + uint(r) < ddim) {
                y[first_row + uint(r)] = s
            }
        }
    }
}

// Q6_K: 6-bit q = nibble | 2 qh bits, minus 32; per-16-element SIGNED sub-scales as 4 scattered
// byte loads (the uint4+select form costs 2 resident simdgroups); d off the trailing f16 plane.
class MetalKqGemvK6 {
    @ssbo @binding = 0 kdh : array<float16> // the d plane — the scale buffer bound at byte nsb*16
    @ssbo @binding = 1 kscb : array<uint8>  // 16B sub-scale blocks, byte view (same buffer at 0)
    @ssbo @binding = 2 kqb : array<uint8>   // k6 quant plane, byte view (ql at 192*blk, qh at +128)
    @ssbo @binding = 3 x : array<float>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint

    [metal_kernel(name="metal_kq_gemv_k6_msl")]
    def metal_kq_gemv_k6 {
        let lane = gl_SubgroupInvocationID
        let tid = lane / 2u
        let ix = lane % 2u
        let ip = tid / 8u
        let il = tid % 8u
        let l0 = il * 4u
        let is0 = ip * 8u + l0 / 16u
        let nb = ndim / 256u
        let first_row = (gl_WorkGroupID.x * 2u + gl_SubgroupID) * 2u
        var yl : float[16]
        var sumf : float[2]
        var ib = ix
        while (ib < nb) {
            let yb = ib * 256u + ip * 128u + l0
            for [unroll_full] (l in range(4)) {
                yl[4 * l] = x[yb + uint(l)]
                yl[4 * l + 1] = x[yb + 32u + uint(l)]
                yl[4 * l + 2] = x[yb + 64u + uint(l)]
                yl[4 * l + 3] = x[yb + 96u + uint(l)]
            }
            for [unroll_full] (r in range(2)) {
                let blk = (first_row + uint(r)) * nb + ib
                let ql0 = blk * 192u + ip * 64u + l0
                let qh0 = blk * 192u + 128u + ip * 32u + l0
                let sb = blk * 16u + is0
                var sums : float[4]
                for [unroll_full] (l in range(4)) {
                    let q1 = uint(kqb[ql0 + uint(l)])
                    let q2 = uint(kqb[ql0 + 32u + uint(l)])
                    let h = uint(kqb[qh0 + uint(l)])
                    sums[0] += yl[4 * l] * (float((q1 & 0x0F) | ((h & 0x03) << 4u)) - 32.0)
                    sums[1] += yl[4 * l + 1] * (float((q2 & 0x0F) | ((h & 0x0C) << 2u)) - 32.0)
                    sums[2] += yl[4 * l + 2] * (float((q1 >> 4u) | (h & 0x30)) - 32.0)
                    sums[3] += yl[4 * l + 3] * (float((q2 >> 4u) | ((h & 0xC0) >> 2u)) - 32.0)
                }
                let s0 = float((int(kscb[sb]) ^ 128) - 128)
                let s2 = float((int(kscb[sb + 2u]) ^ 128) - 128)
                let s4 = float((int(kscb[sb + 4u]) ^ 128) - 128)
                let s6 = float((int(kscb[sb + 6u]) ^ 128) - 128)
                let dd = float(kdh[blk])
                sumf[r] += dd * (sums[0] * s0 + sums[1] * s2 + sums[2] * s4 + sums[3] * s6)
            }
            ib += 2u
        }
        for [unroll_full] (r in range(2)) {
            let s = simd_sum(sumf[r])
            if (lane == 0u && first_row + uint(r) < ddim) {
                y[first_row + uint(r)] = s
            }
        }
    }
}

// ===== K-quant small-batch mv twins (the kq ext form) — batched decode sites at B = 2..8 =====
// The gemv-lab ext winners productionized (-25..-40% vs the per-stream B x B1 wall at B=4). One
// thread per output row; y binds at the site's column byte offset, grid.y = column groups of R.

class MetalKqMvB2K4 {
    @ssbo @binding = 0 ksh : array<float16> // 16B scale blocks, half view (d at 8*blk, dmin at +1)
    @ssbo @binding = 1 ks4 : array<uint4>   // the same buffer, uint4 view
    @ssbo @binding = 2 kqu : array<uint>    // k4 quant plane, uint view (qs of blk at 32*blk)
    @ssbo @binding = 3 x : array<float4>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint
    @uniform @binding = 7 ys : uint         // y row stride, elements
    @uniform @binding = 8 nr : uint         // live columns — stores gate on it

    [metal_kernel(name="metal_kq_mvb2_k4_msl")]
    def metal_kq_mvb2_k4 {
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + ty
        let nb = ndim / 256u
        let q4i = tx / 4u
        let hp = (tx % 4u) / 2u
        let lo = (tx % 2u) * 4u
        let jl = 2u * q4i + hp
        let nsh = (tx % 4u) < 2u ? 0u : 4u
        let nb4 = ndim / 4u
        var sumf : float[2]
        var sb = 0u
        while (sb < nb) {
            let blk = row * nb + sb
            let sv = ks4[blk]
            let dd = float(ksh[blk * 8u])
            let dm = float(ksh[blk * 8u + 1u])
            for [unroll_full] (ch in range(2)) {
                let sy = (sv.y >> (8u * jl)) & 255u
                let szv = (sv.z >> (8u * jl)) & 255u
                let sw = (sv.w >> (8u * jl)) & 255u
                let sc = ch == 0 ? (sy & 63u) : ((sw & 0x0Fu) | ((sy >> 6u) << 4u))
                let mn = ch == 0 ? (szv & 63u) : ((sw >> 4u) | ((szv >> 6u) << 4u))
                let dl = dd * float(sc)
                let ml = dm * float(mn)
                let qb = blk * 32u + (q4i + uint(ch) * 2u) * 8u + lo
                var w : float[16]
                for [unroll_full] (u in range(4)) {
                    let qv = kqu[qb + uint(u)]
                    for [unroll_full] (e in range(4)) {
                        let byv = (qv >> (8u * uint(e))) & 255u
                        w[4 * u + e] = float((byv >> nsh) & 0x0Fu) * dl - ml
                    }
                }
                let cx4 = sb * 64u + (q4i + uint(ch) * 2u) * 16u + ((tx % 4u) < 2u ? 0u : 8u) + lo
                for [unroll_full] (b in range(2)) {
                    let xb4 = uint(b) * nb4 + cx4
                    var acc = 0.0
                    for [unroll_full] (u in range(4)) {
                        let v = x[xb4 + uint(u)]
                        acc += v.x * w[4 * u] + v.y * w[4 * u + 1] + v.z * w[4 * u + 2] + v.w * w[4 * u + 3]
                    }
                    sumf[b] += acc
                }
            }
            sb++
        }
        for [unroll_full] (b in range(2)) {
            var s = sumf[b]
            s += simd_shuffle_down(s, 4u)
            s += simd_shuffle_down(s, 2u)
            s += simd_shuffle_down(s, 1u)
            if (tx == 0u && row < ddim && uint(b) < nr) {
                y[uint(b) * ys + row] = s
            }
        }
    }
}

class MetalKqMvB4K4 {
    @ssbo @binding = 0 ksh : array<float16>
    @ssbo @binding = 1 ks4 : array<uint4>
    @ssbo @binding = 2 kqu : array<uint>
    @ssbo @binding = 3 x : array<float4>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint
    @uniform @binding = 7 ys : uint
    @uniform @binding = 8 nr : uint

    [metal_kernel(name="metal_kq_mvb4_k4_msl")]
    def metal_kq_mvb4_k4 {
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + ty
        let colbase = gl_WorkGroupID.y * 4u
        let nb = ndim / 256u
        let q4i = tx / 4u
        let hp = (tx % 4u) / 2u
        let lo = (tx % 2u) * 4u
        let jl = 2u * q4i + hp
        let nsh = (tx % 4u) < 2u ? 0u : 4u
        let nb4 = ndim / 4u
        var sumf : float[4]
        var sb = 0u
        while (sb < nb) {
            let blk = row * nb + sb
            let sv = ks4[blk]
            let dd = float(ksh[blk * 8u])
            let dm = float(ksh[blk * 8u + 1u])
            for [unroll_full] (ch in range(2)) {
                let sy = (sv.y >> (8u * jl)) & 255u
                let szv = (sv.z >> (8u * jl)) & 255u
                let sw = (sv.w >> (8u * jl)) & 255u
                let sc = ch == 0 ? (sy & 63u) : ((sw & 0x0Fu) | ((sy >> 6u) << 4u))
                let mn = ch == 0 ? (szv & 63u) : ((sw >> 4u) | ((szv >> 6u) << 4u))
                let dl = dd * float(sc)
                let ml = dm * float(mn)
                let qb = blk * 32u + (q4i + uint(ch) * 2u) * 8u + lo
                var w : float[16]
                for [unroll_full] (u in range(4)) {
                    let qv = kqu[qb + uint(u)]
                    for [unroll_full] (e in range(4)) {
                        let byv = (qv >> (8u * uint(e))) & 255u
                        w[4 * u + e] = float((byv >> nsh) & 0x0Fu) * dl - ml
                    }
                }
                let cx4 = sb * 64u + (q4i + uint(ch) * 2u) * 16u + ((tx % 4u) < 2u ? 0u : 8u) + lo
                for [unroll_full] (b in range(4)) {
                    let xb4 = (colbase + uint(b)) * nb4 + cx4
                    var acc = 0.0
                    for [unroll_full] (u in range(4)) {
                        let v = x[xb4 + uint(u)]
                        acc += v.x * w[4 * u] + v.y * w[4 * u + 1] + v.z * w[4 * u + 2] + v.w * w[4 * u + 3]
                    }
                    sumf[b] += acc
                }
            }
            sb++
        }
        for [unroll_full] (b in range(4)) {
            var s = sumf[b]
            s += simd_shuffle_down(s, 4u)
            s += simd_shuffle_down(s, 2u)
            s += simd_shuffle_down(s, 1u)
            if (tx == 0u && row < ddim && colbase + uint(b) < nr) {
                y[(colbase + uint(b)) * ys + row] = s
            }
        }
    }
}

class MetalKqMvB2K6 {
    @ssbo @binding = 0 kdh : array<float16> // the d plane — the scale buffer bound at byte nsb*16
    @ssbo @binding = 1 kscb : array<uint8>  // 16B sub-scale blocks, byte view (same buffer at 0)
    @ssbo @binding = 2 kqu : array<uint>    // k6 quant plane, uint view (ql at 48*blk, qh at 48*blk+32)
    @ssbo @binding = 3 x : array<float4>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint
    @uniform @binding = 7 ys : uint
    @uniform @binding = 8 nr : uint

    [metal_kernel(name="metal_kq_mvb2_k6_msl")]
    def metal_kq_mvb2_k6 {
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + ty
        let nb = ndim / 256u
        let s = tx / 2u
        let o4 = (tx % 2u) * 4u             // 16B offset within the slot, in uints
        let qoff = (s % 2u) * 8u + o4       // ql uint offset within the 128-half's 16 uints
        let shr = s < 2u ? 0u : 4u
        let hsh = 2u * s
        let nb4 = ndim / 4u
        var sumf : float[2]
        var sb = 0u
        while (sb < nb) {
            let blk = row * nb + sb
            let dd = float(kdh[blk])
            for [unroll_full] (ch in range(2)) {
                let ip = uint(ch)
                let qb = blk * 48u + ip * 16u + qoff
                let hb = blk * 48u + 32u + ip * 8u + o4
                let g = ip * 8u + s * 2u + (tx % 2u)
                let sc = float((int(kscb[blk * 16u + g]) ^ 128) - 128)
                let dl = dd * sc
                let ml = dl * 32.0
                var w : float[16]
                // 4+4 uint loads per chunk (the byte-view form was 32 scattered byte loads);
                // the slot's lsh/rsh dance is identically ((h >> 2s) & 3) << 4, vectorized
                for [unroll_full] (u in range(4)) {
                    let qu = kqu[qb + uint(u)]
                    let hu = kqu[hb + uint(u)]
                    let qv = ((qu >> shr) & 0x0F0F0F0F) | (((hu >> hsh) & 0x03030303) << 4u)
                    for [unroll_full] (e in range(4)) {
                        w[4 * u + e] = float((qv >> (8u * uint(e))) & 255u) * dl - ml
                    }
                }
                let cx4 = sb * 64u + ip * 32u + s * 8u + o4
                for [unroll_full] (b in range(2)) {
                    let xb4 = uint(b) * nb4 + cx4
                    var acc = 0.0
                    for [unroll_full] (u in range(4)) {
                        let v = x[xb4 + uint(u)]
                        acc += v.x * w[4 * u] + v.y * w[4 * u + 1] + v.z * w[4 * u + 2] + v.w * w[4 * u + 3]
                    }
                    sumf[b] += acc
                }
            }
            sb++
        }
        for [unroll_full] (b in range(2)) {
            var sm = sumf[b]
            sm += simd_shuffle_down(sm, 4u)
            sm += simd_shuffle_down(sm, 2u)
            sm += simd_shuffle_down(sm, 1u)
            if (tx == 0u && row < ddim && uint(b) < nr) {
                y[uint(b) * ys + row] = sm
            }
        }
    }
}

class MetalKqMvB4K6 {
    @ssbo @binding = 0 kdh : array<float16>
    @ssbo @binding = 1 kscb : array<uint8>
    @ssbo @binding = 2 kqu : array<uint>
    @ssbo @binding = 3 x : array<float4>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint
    @uniform @binding = 7 ys : uint
    @uniform @binding = 8 nr : uint

    [metal_kernel(name="metal_kq_mvb4_k6_msl")]
    def metal_kq_mvb4_k6 {
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + ty
        let colbase = gl_WorkGroupID.y * 4u
        let nb = ndim / 256u
        let s = tx / 2u
        let o4 = (tx % 2u) * 4u
        let qoff = (s % 2u) * 8u + o4
        let shr = s < 2u ? 0u : 4u
        let hsh = 2u * s
        let nb4 = ndim / 4u
        var sumf : float[4]
        var sb = 0u
        while (sb < nb) {
            let blk = row * nb + sb
            let dd = float(kdh[blk])
            for [unroll_full] (ch in range(2)) {
                let ip = uint(ch)
                let qb = blk * 48u + ip * 16u + qoff
                let hb = blk * 48u + 32u + ip * 8u + o4
                let g = ip * 8u + s * 2u + (tx % 2u)
                let sc = float((int(kscb[blk * 16u + g]) ^ 128) - 128)
                let dl = dd * sc
                let ml = dl * 32.0
                var w : float[16]
                for [unroll_full] (u in range(4)) {
                    let qu = kqu[qb + uint(u)]
                    let hu = kqu[hb + uint(u)]
                    let qv = ((qu >> shr) & 0x0F0F0F0F) | (((hu >> hsh) & 0x03030303) << 4u)
                    for [unroll_full] (e in range(4)) {
                        w[4 * u + e] = float((qv >> (8u * uint(e))) & 255u) * dl - ml
                    }
                }
                let cx4 = sb * 64u + ip * 32u + s * 8u + o4
                for [unroll_full] (b in range(4)) {
                    let xb4 = (colbase + uint(b)) * nb4 + cx4
                    var acc = 0.0
                    for [unroll_full] (u in range(4)) {
                        let v = x[xb4 + uint(u)]
                        acc += v.x * w[4 * u] + v.y * w[4 * u + 1] + v.z * w[4 * u + 2] + v.w * w[4 * u + 3]
                    }
                    sumf[b] += acc
                }
            }
            sb++
        }
        for [unroll_full] (b in range(4)) {
            var sm = sumf[b]
            sm += simd_shuffle_down(sm, 4u)
            sm += simd_shuffle_down(sm, 2u)
            sm += simd_shuffle_down(sm, 1u)
            if (tx == 0u && row < ddim && colbase + uint(b) < nr) {
                y[(colbase + uint(b)) * ys + row] = sm
            }
        }
    }
}

// k5 twins: k4's chunk geometry and kmask scale decode verbatim (q5_K shares the 12-byte scale
// block) + the qh 5th-bit overlay. The 160B plane rides the SAME uint view (40 uints/blk: qs at
// 40*blk, qh at 40*blk+32) — the overlay bit equals its kmask pair index, so scale indexing serves it too.
class MetalKqMvB2K5 {
    @ssbo @binding = 0 ksh : array<float16> // 16B scale blocks, half view (d at 8*blk, dmin at +1)
    @ssbo @binding = 1 ks4 : array<uint4>   // the same buffer, uint4 view
    @ssbo @binding = 2 kqu : array<uint>    // k5 plane, uint view (qs at 40*blk, qh at 40*blk+32)
    @ssbo @binding = 3 x : array<float4>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint
    @uniform @binding = 7 ys : uint         // y row stride, elements
    @uniform @binding = 8 nr : uint         // live columns — stores gate on it

    [metal_kernel(name="metal_kq_mvb2_k5_msl")]
    def metal_kq_mvb2_k5 {
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + ty
        let nb = ndim / 256u
        let q4i = tx / 4u
        let hp = (tx % 4u) / 2u
        let lo = (tx % 2u) * 4u
        let jl = 2u * q4i + hp
        let nib = (tx % 4u) < 2u
        let nsh = nib ? 0u : 4u
        let nb4 = ndim / 4u
        var sumf : float[2]
        var sb = 0u
        while (sb < nb) {
            let blk = row * nb + sb
            let sv = ks4[blk]
            let dd = float(ksh[blk * 8u])
            let dm = float(ksh[blk * 8u + 1u])
            var hq : uint[4]
            for [unroll_full] (u in range(4)) {
                hq[u] = kqu[blk * 40u + 32u + lo + uint(u)]
            }
            for [unroll_full] (ch in range(2)) {
                let sy = (sv.y >> (8u * jl)) & 255u
                let szv = (sv.z >> (8u * jl)) & 255u
                let sw = (sv.w >> (8u * jl)) & 255u
                let sc = ch == 0 ? (sy & 63u) : ((sw & 0x0Fu) | ((sy >> 6u) << 4u))
                let mn = ch == 0 ? (szv & 63u) : ((sw >> 4u) | ((szv >> 6u) << 4u))
                let dl = dd * float(sc)
                let ml = dm * float(mn)
                let qb = blk * 40u + (q4i + uint(ch) * 2u) * 8u + lo
                let hsh = jl + 4u * uint(ch)
                var w : float[16]
                for [unroll_full] (u in range(4)) {
                    let qv = kqu[qb + uint(u)]
                    for [unroll_full] (e in range(4)) {
                        let byv = (qv >> (8u * uint(e))) & 255u
                        let hb = (hq[u] >> (8u * uint(e) + hsh)) & 1u
                        w[4 * u + e] = float(((byv >> nsh) & 0x0Fu) | (hb << 4u)) * dl - ml
                    }
                }
                let cx4 = sb * 64u + (q4i + uint(ch) * 2u) * 16u + (nib ? 0u : 8u) + lo
                for [unroll_full] (b in range(2)) {
                    let xb4 = uint(b) * nb4 + cx4
                    var acc = 0.0
                    for [unroll_full] (u in range(4)) {
                        let v = x[xb4 + uint(u)]
                        acc += v.x * w[4 * u] + v.y * w[4 * u + 1] + v.z * w[4 * u + 2] + v.w * w[4 * u + 3]
                    }
                    sumf[b] += acc
                }
            }
            sb++
        }
        for [unroll_full] (b in range(2)) {
            var s = sumf[b]
            s += simd_shuffle_down(s, 4u)
            s += simd_shuffle_down(s, 2u)
            s += simd_shuffle_down(s, 1u)
            if (tx == 0u && row < ddim && uint(b) < nr) {
                y[uint(b) * ys + row] = s
            }
        }
    }
}

class MetalKqMvB4K5 {
    @ssbo @binding = 0 ksh : array<float16>
    @ssbo @binding = 1 ks4 : array<uint4>
    @ssbo @binding = 2 kqu : array<uint>
    @ssbo @binding = 3 x : array<float4>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint
    @uniform @binding = 7 ys : uint
    @uniform @binding = 8 nr : uint

    [metal_kernel(name="metal_kq_mvb4_k5_msl")]
    def metal_kq_mvb4_k5 {
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + ty
        let colbase = gl_WorkGroupID.y * 4u
        let nb = ndim / 256u
        let q4i = tx / 4u
        let hp = (tx % 4u) / 2u
        let lo = (tx % 2u) * 4u
        let jl = 2u * q4i + hp
        let nib = (tx % 4u) < 2u
        let nsh = nib ? 0u : 4u
        let nb4 = ndim / 4u
        var sumf : float[4]
        var sb = 0u
        while (sb < nb) {
            let blk = row * nb + sb
            let sv = ks4[blk]
            let dd = float(ksh[blk * 8u])
            let dm = float(ksh[blk * 8u + 1u])
            var hq : uint[4]
            for [unroll_full] (u in range(4)) {
                hq[u] = kqu[blk * 40u + 32u + lo + uint(u)]
            }
            for [unroll_full] (ch in range(2)) {
                let sy = (sv.y >> (8u * jl)) & 255u
                let szv = (sv.z >> (8u * jl)) & 255u
                let sw = (sv.w >> (8u * jl)) & 255u
                let sc = ch == 0 ? (sy & 63u) : ((sw & 0x0Fu) | ((sy >> 6u) << 4u))
                let mn = ch == 0 ? (szv & 63u) : ((sw >> 4u) | ((szv >> 6u) << 4u))
                let dl = dd * float(sc)
                let ml = dm * float(mn)
                let qb = blk * 40u + (q4i + uint(ch) * 2u) * 8u + lo
                let hsh = jl + 4u * uint(ch)
                var w : float[16]
                for [unroll_full] (u in range(4)) {
                    let qv = kqu[qb + uint(u)]
                    for [unroll_full] (e in range(4)) {
                        let byv = (qv >> (8u * uint(e))) & 255u
                        let hb = (hq[u] >> (8u * uint(e) + hsh)) & 1u
                        w[4 * u + e] = float(((byv >> nsh) & 0x0Fu) | (hb << 4u)) * dl - ml
                    }
                }
                let cx4 = sb * 64u + (q4i + uint(ch) * 2u) * 16u + (nib ? 0u : 8u) + lo
                for [unroll_full] (b in range(4)) {
                    let xb4 = (colbase + uint(b)) * nb4 + cx4
                    var acc = 0.0
                    for [unroll_full] (u in range(4)) {
                        let v = x[xb4 + uint(u)]
                        acc += v.x * w[4 * u] + v.y * w[4 * u + 1] + v.z * w[4 * u + 2] + v.w * w[4 * u + 3]
                    }
                    sumf[b] += acc
                }
            }
            sb++
        }
        for [unroll_full] (b in range(4)) {
            var s = sumf[b]
            s += simd_shuffle_down(s, 4u)
            s += simd_shuffle_down(s, 2u)
            s += simd_shuffle_down(s, 1u)
            if (tx == 0u && row < ddim && colbase + uint(b) < nr) {
                y[(colbase + uint(b)) * ys + row] = s
            }
        }
    }
}

// One thread per rotation pair, NORM layout (adjacent elements 2j, 2j+1 within each head):
// v0*c - v1*s / v0*s + v1*c with this position's precomputed table row — rope_scaled_tab exactly.
// One kernel serves q (n = qd) and k (n = kv_dim) as two dispatches.
class MetalRope {
    @ssbo @binding = 0 v : array<float>     // [ntok x n], roped in place
    @ssbo @binding = 1 tcos : array<float>  // [ntok x head_size/2]
    @ssbo @binding = 2 tsin : array<float>
    @uniform @binding = 3 n : uint          // row width (qd or kv_dim), % head_size
    @uniform @binding = 4 head_size : uint
    @uniform @binding = 5 npairs : uint     // ntok * n/2 — grid guard

    [metal_kernel(name="metal_rope_msl")]
    def metal_rope {
        let gid = gl_GlobalInvocationID.x
        if (gid >= npairs) {
            return
        }
        let pairs_per_row = n / 2u
        let p = gid / pairs_per_row
        let rem = gid % pairs_per_row
        let half = head_size / 2u
        let h = rem / half
        let j = rem % half
        let i = p * n + h * head_size + 2u * j
        let fcr = tcos[p * half + j]
        let fci = tsin[p * half + j]
        let v0 = v[i]
        let v1 = v[i + 1u]
        v[i] = v0 * fcr - v1 * fci
        v[i + 1u] = v0 * fci + v1 * fcr
    }
}

// The dispatch-fusion set (21 -> 15 dispatches/layer — ew cost on M1 is launch gaps, not
// bandwidth): metal_add_rms_quant fuses add+rmsnorm+quantize in one pass (dim <= 3072 gates it);
// metal_swiglu_quant recomputes silu on the quant pass instead of buffering; metal_rope_qk fuses both rope dispatches.
class MetalAddRmsQuant {
    @ssbo @binding = 0 x : array<float>     // [nrows x dim] residual stream, updated in place
    @ssbo @binding = 1 r : array<float>     // [nrows x dim] delta to fold in (addv != 0)
    @ssbo @binding = 2 w : array<float>     // [dim] rms weight
    @ssbo @binding = 3 xq : array<int8>     // [mp x dim] quants out
    @ssbo @binding = 4 xs : array<float>    // [mp x dim/32] scales out
    @uniform @binding = 5 dim : uint
    @uniform @binding = 6 eps : float
    @uniform @binding = 7 addv : uint       // 0 = plain rms+quant (layer 0 / no pending add)
    @workgroup xrow : float[3072]           // the summed row (pre-norm) — llama-1B/3B dims fit
    @workgroup partial : float[8]
    @workgroup inv_sh : float

    [metal_kernel(name="metal_add_rms_quant_msl")]
    def metal_add_rms_quant {
        let row = gl_WorkGroupID.x
        let lid = gl_LocalInvocationID.x
        var ss = 0.0
        var k = lid
        while (k < dim) {
            var xv = x[row * dim + k]
            if (addv != 0u) {
                xv += r[row * dim + k]
                x[row * dim + k] = xv          // the residual stream keeps the sum
            }
            xrow[k] = xv
            ss += xv * xv
            k += gl_WorkGroupSize.x
        }
        ss = simd_sum(ss)
        if (gl_SubgroupInvocationID == 0u) {
            partial[gl_SubgroupID] = ss
        }
        barrier()
        if (lid == 0u) {
            var tot = 0.0
            var sgi = 0u
            while (sgi < gl_NumSubgroups) {
                tot += partial[sgi]
                sgi++
            }
            inv_sh = rsqrt(tot / float(dim) + eps)
        }
        barrier()
        let inv = inv_sh
        let nb = dim / 32u
        var cb = lid
        while (cb < nb) {
            let base = cb * 32u
            var amax = 0.0
            var e = 0u
            while (e < 32u) {
                amax = max(amax, abs(w[base + e] * (xrow[base + e] * inv)))
                e++
            }
            let d = amax / 127.0
            let id = d != 0.0 ? 1.0 / d : 0.0
            xs[row * nb + cb] = d
            e = 0u
            while (e < 32u) {
                xq[row * dim + base + e] = int8(round(w[base + e] * (xrow[base + e] * inv) * id))
                e++
            }
            cb += gl_WorkGroupSize.x
        }
    }
}

class MetalSwigluQuant {
    @ssbo @binding = 0 g : array<float>     // [ntok x hidden] W1 gate (read-only — dead after)
    @ssbo @binding = 1 u : array<float>     // [ntok x hidden] W3 up
    @ssbo @binding = 2 xq : array<int8>     // [mp x hidden] quants out
    @ssbo @binding = 3 xs : array<float>    // [mp x hidden/32] scales out
    @uniform @binding = 4 n : uint          // hidden, % 32
    @uniform @binding = 5 nblk : uint       // ntok * hidden/32 — grid guard

    [metal_kernel(name="metal_swiglu_quant_msl")]
    def metal_swiglu_quant {
        let gid = gl_GlobalInvocationID.x
        if (gid >= nblk) {
            return
        }
        let nb = n / 32u
        let row = gid / nb
        let cb = gid % nb
        let base = row * n + cb * 32u
        var amax = 0.0
        var k = 0u
        while (k < 32u) {
            let gv = g[base + k]
            let sv = (gv / (1.0 + exp(-gv))) * u[base + k]
            amax = max(amax, abs(sv))
            k++
        }
        let d = amax / 127.0
        let id = d != 0.0 ? 1.0 / d : 0.0
        xs[row * nb + cb] = d
        k = 0u
        while (k < 32u) {
            let gv = g[base + k]
            let sv = (gv / (1.0 + exp(-gv))) * u[base + k]
            xq[base + k] = int8(round(sv * id))
            k++
        }
    }
}

class MetalRopeQK {
    @ssbo @binding = 0 q : array<float>     // [ntok x qd], roped in place
    @ssbo @binding = 1 k : array<float>     // [ntok x kv_dim], roped in place
    @ssbo @binding = 2 tcos : array<float>  // [ntok x head_size/2]
    @ssbo @binding = 3 tsin : array<float>
    @uniform @binding = 4 qd : uint
    @uniform @binding = 5 kvd : uint
    @uniform @binding = 6 head_size : uint
    @uniform @binding = 7 npairs_q : uint   // ntok * qd/2 — the q half of the grid
    @uniform @binding = 8 npairs : uint     // + ntok * kv_dim/2 — grid guard

    [metal_kernel(name="metal_rope_qk_msl")]
    def metal_rope_qk {
        let gid = gl_GlobalInvocationID.x
        if (gid >= npairs) {
            return
        }
        let isK = gid >= npairs_q
        let n = isK ? kvd : qd
        let lgid = isK ? gid - npairs_q : gid
        let pairs_per_row = n / 2u
        let p = lgid / pairs_per_row
        let rem = lgid % pairs_per_row
        let half = head_size / 2u
        let h = rem / half
        let j = rem % half
        let i = p * n + h * head_size + 2u * j
        let fcr = tcos[p * half + j]
        let fci = tsin[p * half + j]
        if (isK) {
            let v0 = k[i]
            let v1 = k[i + 1u]
            k[i] = v0 * fcr - v1 * fci
            k[i + 1u] = v0 * fci + v1 * fcr
        } else {
            let v0 = q[i]
            let v1 = q[i + 1u]
            q[i] = v0 * fcr - v1 * fci
            q[i + 1u] = v0 * fci + v1 * fcr
        }
    }
}

// Attention runs as a tiled-GEMM trio over a per-head score slab PADDED to 32-row blocks (np32 =
// ceil32(npos)): metal_attn_qk (S = scale*Q.K^T) -> metal_attn_softmax (causal, zeroes the tail)
// -> metal_attn_av (O = P.V). Replaces a naive per-row pair that owned 44% of the prefill's GPU time.

// One threadgroup (128 threads = 4 simdgroups) per 32x32 score block; blocks fully above the
// causal diagonal exit early. Q staging folds `scale` in; K stages transposed, rows >= npos
// zeroed (pads can carry NaN). head_size walks in 64-wide K-SLABS — hs 64 is one slab, 128 two.
class MetalAttnQK {
    @ssbo @binding = 0 q : array<float>     // [mp x qd], roped
    @ssbo @binding = 1 k : array<float>     // [nk64 x kv_dim], roped (existing rows + the chunk)
    @ssbo @binding = 2 att : array<float>   // [n_heads x qrows x np32] raw scores out
    @uniform @binding = 3 qd : uint
    @uniform @binding = 4 kv_dim : uint
    @uniform @binding = 5 head_size : uint
    @uniform @binding = 6 kv_mul : uint
    @uniform @binding = 7 npos : uint       // KEY rows (continuation: qoff existing + the chunk)
    @uniform @binding = 8 np32 : uint       // padded key extent = the score slab's column stride
    @uniform @binding = 9 scale : float
    @uniform @binding = 10 qoff : uint      // the chunk's first GLOBAL position (0 = fresh start)
    @uniform @binding = 11 qrows : uint     // padded QUERY rows — the score slab's row extent
    @workgroup ta : float[2048]             // Q block: 32 qi-rows x 64 (row stride 64)
    @workgroup tb : float[2048]             // KT block: 64 k-rows x 32 j-cols (row stride 32)

    [metal_kernel(name="metal_attn_qk_msl")]
    def metal_attn_qk {
        let qi0 = gl_WorkGroupID.x * 32u
        let j0 = gl_WorkGroupID.y * 32u
        let h = gl_WorkGroupID.z
        if (j0 > qoff + qi0 + 31u) {
            return                          // fully above the causal diagonal
        }
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let kvhoff = (h / kv_mul) * head_size
        let qm = (sg / 2u) * 16u
        let qn = (sg % 2u) * 16u
        var c00 : simdgroup_float8x8
        var c01 : simdgroup_float8x8
        var c10 : simdgroup_float8x8
        var c11 : simdgroup_float8x8
        var ks = 0u
        while (ks < head_size) {
            let kslab = min(head_size - ks, 64u)
            var e = lid
            while (e < 2048u) {
                let r = e / 64u
                let c = e % 64u
                ta[e] = c < kslab ? q[(qi0 + r) * qd + h * head_size + ks + c] * scale : 0.0
                let jr = e % 32u
                let kc = e / 32u
                tb[e] = (kc < kslab && j0 + jr < npos) ? k[(j0 + jr) * kv_dim + kvhoff + ks + kc] : 0.0
                e += 128u
            }
            barrier()
            var k8 = 0u
            while (k8 < kslab) {
                var a0 : simdgroup_float8x8
                var a1 : simdgroup_float8x8
                var b0 : simdgroup_float8x8
                var b1 : simdgroup_float8x8
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
            ks += 64u
        }
        let row0 = (h * qrows + qi0 + qm) * np32 + (j0 + qn)
        simdgroup_store(c00, att, row0, np32)
        simdgroup_store(c01, att, row0 + 8u, np32)
        simdgroup_store(c10, att, row0 + 8u * np32, np32)
        simdgroup_store(c11, att, row0 + 8u * np32 + 8u, np32)
    }
}

// One threadgroup (128 threads = 4 simdgroups) per (query row < npos, head): causal in-row
// softmax via a width-32 simd_shuffle_xor butterfly; the tail (cnt, np32) is ZEROED so the AV
// GEMM can run whole blocks. A continuation chunk sees qoff existing keys: cnt = qoff + qi + 1.
class MetalAttnSoftmax {
    @ssbo @binding = 0 att : array<float>   // [n_heads x qrows x np32], normalized in place
    @uniform @binding = 1 np32 : uint
    @uniform @binding = 2 qoff : uint       // the chunk's first GLOBAL position (0 = fresh start)
    @uniform @binding = 3 qrows : uint      // padded query rows — the slab's row extent
    @workgroup red : float[4]               // one per simdgroup
    @workgroup bcast : float

    [metal_kernel(name="metal_attn_softmax_msl")]
    def metal_attn_softmax {
        let qi = gl_WorkGroupID.x
        let h = gl_WorkGroupID.y
        let lid = gl_LocalInvocationID.x
        let nthreads = gl_WorkGroupSize.x
        let cnt = qoff + qi + 1u
        let rowbase = (h * qrows + qi) * np32
        var m = -1.0e30
        var j = lid
        while (j < cnt) {
            m = max(m, att[rowbase + j])
            j += nthreads
        }
        m = max(m, simd_shuffle_xor(m, 16u))
        m = max(m, simd_shuffle_xor(m, 8u))
        m = max(m, simd_shuffle_xor(m, 4u))
        m = max(m, simd_shuffle_xor(m, 2u))
        m = max(m, simd_shuffle_xor(m, 1u))
        if (gl_SubgroupInvocationID == 0u) {
            red[gl_SubgroupID] = m
        }
        barrier()
        if (lid == 0u) {
            var mm = red[0]
            var sgi = 1u
            while (sgi < gl_NumSubgroups) {
                mm = max(mm, red[sgi])
                sgi++
            }
            bcast = mm
        }
        barrier()
        let rowmax = bcast
        var sum = 0.0
        j = lid
        while (j < cnt) {
            let ev = exp(att[rowbase + j] - rowmax)
            att[rowbase + j] = ev
            sum += ev
            j += nthreads
        }
        sum = simd_sum(sum)
        if (gl_SubgroupInvocationID == 0u) {
            red[gl_SubgroupID] = sum
        }
        barrier()
        if (lid == 0u) {
            var tot = 0.0
            var sgi = 0u
            while (sgi < gl_NumSubgroups) {
                tot += red[sgi]
                sgi++
            }
            bcast = 1.0 / tot
        }
        barrier()
        let inv = bcast
        j = lid
        while (j < cnt) {
            att[rowbase + j] *= inv
            j += nthreads
        }
        j = cnt + lid
        while (j < np32) {
            att[rowbase + j] = 0.0          // the AV GEMM runs whole 32-blocks over this tail
            j += nthreads
        }
    }
}

// One threadgroup (128 threads = 4 simdgroups) per 32x32 output block: O = P.V walking j in
// 32-blocks up to the causal limit qi0+32 (P's zeroed tail keeps whole blocks exact). V rows
// >= npos stage as 0 (pad rows can carry NaN; 0*NaN would still poison). hs % 32 == 0 required.
class MetalAttnAV {
    @ssbo @binding = 0 att : array<float>   // [n_heads x qrows x np32] normalized P
    @ssbo @binding = 1 v : array<float>     // [nk64 x kv_dim] raw V (existing rows + the chunk)
    @ssbo @binding = 2 xb : array<float>    // [mp x qd] attention output
    @uniform @binding = 3 qd : uint
    @uniform @binding = 4 kv_dim : uint
    @uniform @binding = 5 head_size : uint
    @uniform @binding = 6 kv_mul : uint
    @uniform @binding = 7 npos : uint       // KEY rows (continuation: qoff existing + the chunk)
    @uniform @binding = 8 np32 : uint
    @uniform @binding = 9 qoff : uint       // the chunk's first GLOBAL position (0 = fresh start)
    @uniform @binding = 10 qrows : uint     // padded query rows — the slab's row extent
    @workgroup ta : float[1024]             // P block: 32 qi-rows x 32 j (stride 32)
    @workgroup tb : float[1024]             // V block: 32 j-rows x 32 d (stride 32)

    [metal_kernel(name="metal_attn_av_msl")]
    def metal_attn_av {
        let qi0 = gl_WorkGroupID.x * 32u
        let d0 = gl_WorkGroupID.y * 32u
        let h = gl_WorkGroupID.z
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let kvhoff = (h / kv_mul) * head_size
        let jlimit = min(qoff + qi0 + 32u, np32)   // causal: P rows here are zero past qoff+qi0+31
        let qm = (sg / 2u) * 16u
        let qn = (sg % 2u) * 16u
        var c00 : simdgroup_float8x8
        var c01 : simdgroup_float8x8
        var c10 : simdgroup_float8x8
        var c11 : simdgroup_float8x8
        var jb = 0u
        while (jb < jlimit) {
            var e = lid
            while (e < 1024u) {
                let r = e / 32u
                let c = e % 32u
                ta[e] = att[(h * qrows + qi0 + r) * np32 + jb + c]
                tb[e] = jb + r < npos ? v[(jb + r) * kv_dim + kvhoff + d0 + c] : 0.0
                e += 128u
            }
            barrier()
            var k8 = 0u
            while (k8 < 32u) {
                var a0 : simdgroup_float8x8
                var a1 : simdgroup_float8x8
                var b0 : simdgroup_float8x8
                var b1 : simdgroup_float8x8
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
            jb += 32u
        }
        let row0 = (qi0 + qm) * qd + h * head_size + d0 + qn
        simdgroup_store(c00, xb, row0, qd)
        simdgroup_store(c01, xb, row0 + 8u, qd)
        simdgroup_store(c10, xb, row0 + 8u * qd, qd)
        simdgroup_store(c11, xb, row0 + 8u * qd + 8u, qd)
    }
}

// The ggml-geometry attention pair (v21 lessons applied to the trio's QK/AV GEMMs — the old
// 32x32 scalar-f32-staged kernels ran ~60x below mul_mm rate, owning 24ms of the 3B window).
// QK: S[h] = scale * Q_h . K_h^T, 32x64 C tiles; DASLLAMA_METAL_ATTN=0 pins the trio (hs % 64 != 0 too).
class MetalAttnQKMm {
    @ssbo @binding = 0 q : array<float4>    // [mp x qd/4], roped
    @ssbo @binding = 1 k : array<float4>    // [nk64 x kv_dim/4], roped (existing rows + the chunk)
    @ssbo @binding = 2 att : array<float>   // [n_heads x qrows x np32] raw scores out
    @uniform @binding = 3 qd : uint
    @uniform @binding = 4 kv_dim : uint
    @uniform @binding = 5 head_size : uint
    @uniform @binding = 6 kv_mul : uint
    @uniform @binding = 7 npos : uint       // KEY rows (continuation: qoff existing + the chunk)
    @uniform @binding = 8 np32 : uint
    @uniform @binding = 9 scale : float
    @uniform @binding = 10 qoff : uint      // the chunk's first GLOBAL position (0 = fresh start)
    @uniform @binding = 11 qrows : uint     // padded query rows — the slab's row extent
    @workgroup ta : float16[2048]           // K: k-tiles x 8 key-tiles of 8x8, tile (k row, key col)
    @workgroup tb : float16[1024]           // Q: k-tiles x 4 query-tiles of 8x8 (query row, k col)

    [metal_kernel(name="metal_attn_qk_mm_msl")]
    def metal_attn_qk_mm {
        let mBase = gl_WorkGroupID.x * 32u  // query rows
        let nBase = gl_WorkGroupID.y * 64u  // key rows
        if (nBase > qoff + mBase + 31u) {
            return                          // fully above the causal diagonal
        }
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let h = gl_WorkGroupID.z
        let kvhoff4 = ((h / kv_mul) * head_size) / 4u
        let qoff4 = (h * head_size) / 4u
        let qd4 = qd / 4u
        let kvd4 = kv_dim / 4u
        var mc : simdgroup_float8x8[8]      // C quadrant: 2 row-tiles x 4 col-tiles
        let lr0 = lid / 2u                  // A split: key row + 16-k half
        let il0 = lid % 2u
        let syA = lr0 / 8u
        let lxA = lr0 % 8u
        let tA0 = (il0 * 2u * 8u + syA) * 64u + lxA
        let lr1 = lid / 4u                  // B split: query row + 8-k run
        let iy = (lid % 4u) * 8u
        let ibB = ((lid % 4u) * 4u + lr1 / 8u) * 64u + (lr1 % 8u) * 8u
        let aB = (sg % 2u) * 256u
        let bB = (sg / 2u) * 128u
        var ma : simdgroup_half8x8[4]
        var mb : simdgroup_half8x8[2]
        var kb = 0u
        while (kb * 32u < head_size) {
            let srcA = (nBase + lr0) * kvd4 + kvhoff4 + (kb * 32u + il0 * 16u) / 4u
            let a0 = k[srcA]
            let a1 = k[srcA + 1u]
            let a2 = k[srcA + 2u]
            let a3 = k[srcA + 3u]
            let srcB = (mBase + lr1) * qd4 + qoff4 + (kb * 32u + iy) / 4u
            let b0 = q[srcB] * scale
            let b1 = q[srcB + 1u] * scale
            var va : float16[16]
            va[0] = float16(a0.x)
            va[1] = float16(a0.y)
            va[2] = float16(a0.z)
            va[3] = float16(a0.w)
            va[4] = float16(a1.x)
            va[5] = float16(a1.y)
            va[6] = float16(a1.z)
            va[7] = float16(a1.w)
            va[8] = float16(a2.x)
            va[9] = float16(a2.y)
            va[10] = float16(a2.z)
            va[11] = float16(a2.w)
            va[12] = float16(a3.x)
            va[13] = float16(a3.y)
            va[14] = float16(a3.z)
            va[15] = float16(a3.w)
            barrier()
            // K stage as a rolled loop off tA0 (tA1 = tA0 + 512) — 16 hand-written strided
            // stores hoist 16 tile addresses into loop-lifetime registers
            for [unroll_full] (i in range(16)) {
                ta[tA0 + uint(i / 8) * 512u + uint(i % 8) * 8u] = va[i]
            }
            tg_store_half4(tb, ibB, b0)
            tg_store_half4(tb, ibB + 4u, b1)
            barrier()
            // math ROLLED over matrix arrays + das-pointer tile bumps — the hand-unrolled
            // spelling costs an occupancy tier (this kernel measured max_threads 704)
            var acur = unsafe(addr(ta[aB]))
            var bcur = unsafe(addr(tb[bB]))
            for [unroll_full] (_ik in range(4)) {
                for [unroll_full] (i in range(4)) {
                    simdgroup_load(ma[i], acur, uint(i) * 64u, 8u)
                }
                for [unroll_full] (i in range(2)) {
                    simdgroup_load(mb[i], bcur, uint(i) * 64u, 8u)
                }
                for [unroll_full] (i in range(8)) {
                    simdgroup_multiply_accumulate(mc[i], mb[i / 4], ma[i % 4], mc[i])
                }
                acur = unsafe(acur + 512)
                bcur = unsafe(bcur + 256)
            }
            kb++
        }
        let base = h * qrows * np32
        let row0 = mBase + (sg / 2u) * 16u  // accumulator rows = queries
        let col0 = nBase + (sg % 2u) * 32u  // accumulator cols = keys
        for [unroll_full] (i in range(8)) {
            simdgroup_store(mc[i], att, base + (row0 + uint(i / 4) * 8u) * np32 + col0 + uint(i % 4) * 8u, np32)
        }
    }
}

// AV: O_h = P_h . V_h — C tile 32 tokens x 64 V-columns, k walks the causally limited position
// axis in 32-blocks. V stages contiguous float4 loads, position rows >= npos ZEROED (causal
// softmax zeroes P columns >= npos EXACTLY; 0 * NaN from a pad V row would poison the tile).
class MetalAttnAVMm {
    @ssbo @binding = 0 att : array<float4>  // [n_heads x qrows x np32/4] normalized P
    @ssbo @binding = 1 v : array<float4>    // [nk64 x kv_dim/4] raw V (existing rows + the chunk)
    @ssbo @binding = 2 xb : array<float>    // [mp x qd] attention output
    @uniform @binding = 3 qd : uint
    @uniform @binding = 4 kv_dim : uint
    @uniform @binding = 5 head_size : uint
    @uniform @binding = 6 kv_mul : uint
    @uniform @binding = 7 npos : uint       // KEY rows (continuation: qoff existing + the chunk)
    @uniform @binding = 8 np32 : uint
    @uniform @binding = 9 qoff : uint       // the chunk's first GLOBAL position (0 = fresh start)
    @uniform @binding = 10 qrows : uint     // padded query rows — the slab's row extent
    @workgroup ta : float16[2048]           // V: pos-tiles x 8 col-tiles of 8x8, tile (pos row, col col)
    @workgroup tb : float16[1024]           // P: pos-tiles x 4 token-tiles of 8x8 (token row, pos col)

    [metal_kernel(name="metal_attn_av_mm_msl")]
    def metal_attn_av_mm {
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let mBase = gl_WorkGroupID.x * 32u  // token rows
        let vBase = gl_WorkGroupID.y * 64u  // V columns within the head
        let h = gl_WorkGroupID.z
        let kvhoff4 = ((h / kv_mul) * head_size) / 4u
        let kvd4 = kv_dim / 4u
        let np324 = np32 / 4u
        var mc : simdgroup_float8x8[8]      // C quadrant: 2 row-tiles x 4 col-tiles
        // A split: thread stages one position row x 16 contiguous V columns (4 x tg_store_half4)
        let pr = lid / 4u
        let vc16 = (lid % 4u) * 16u
        let sxyA = (pr / 8u) * 8u * 64u + (pr % 8u) * 8u   // 64*(8*(pos/8)) + 8*(pos%8)
        // B split: thread stages one token row x 8 contiguous positions of P
        let lr1 = lid / 4u
        let iy = (lid % 4u) * 8u
        let ibB = ((lid % 4u) * 4u + lr1 / 8u) * 64u + (lr1 % 8u) * 8u
        let aB = (sg % 2u) * 256u
        let bB = (sg / 2u) * 128u
        var ma : simdgroup_half8x8[4]
        var mb : simdgroup_half8x8[2]
        let klimit = min(qoff + mBase + 32u, np32)   // causal: P rows here are zero past qoff+mBase+31
        var kb = 0u
        while (kb * 32u < klimit) {
            let posg = kb * 32u + pr
            let srcA = posg * kvd4 + kvhoff4 + (vBase + vc16) / 4u
            let zero = float4(0.0)
            let a0 = posg < npos ? v[srcA] : zero
            let a1 = posg < npos ? v[srcA + 1u] : zero
            let a2 = posg < npos ? v[srcA + 2u] : zero
            let a3 = posg < npos ? v[srcA + 3u] : zero
            let srcB = (h * qrows + mBase + lr1) * np324 + (kb * 32u + iy) / 4u
            let b0 = att[srcB]
            let b1 = att[srcB + 1u]
            barrier()
            let tbase = sxyA + (vc16 / 8u) * 64u            // first col-tile this thread touches
            tg_store_half4(ta, tbase, a0)
            tg_store_half4(ta, tbase + 4u, a1)
            tg_store_half4(ta, tbase + 64u, a2)             // next 8-column tile
            tg_store_half4(ta, tbase + 68u, a3)
            tg_store_half4(tb, ibB, b0)
            tg_store_half4(tb, ibB + 4u, b1)
            barrier()
            // math ROLLED over matrix arrays + das-pointer tile bumps — the hand-unrolled
            // spelling costs an occupancy tier (this kernel measured max_threads 704)
            var acur = unsafe(addr(ta[aB]))
            var bcur = unsafe(addr(tb[bB]))
            for [unroll_full] (_ik in range(4)) {
                for [unroll_full] (i in range(4)) {
                    simdgroup_load(ma[i], acur, uint(i) * 64u, 8u)
                }
                for [unroll_full] (i in range(2)) {
                    simdgroup_load(mb[i], bcur, uint(i) * 64u, 8u)
                }
                for [unroll_full] (i in range(8)) {
                    simdgroup_multiply_accumulate(mc[i], mb[i / 4], ma[i % 4], mc[i])
                }
                acur = unsafe(acur + 512)
                bcur = unsafe(bcur + 256)
            }
            kb++
        }
        let row0 = mBase + (sg / 2u) * 16u  // accumulator rows = tokens
        let col0 = h * head_size + vBase + (sg % 2u) * 32u
        for [unroll_full] (i in range(8)) {
            simdgroup_store(mc[i], xb, (row0 + uint(i / 4) * 8u) * qd + col0 + uint(i % 4) * 8u, qd)
        }
    }
}

// Fused SwiGLU gate, one thread per element: g = silu(g) * u — swiglu exactly (the CPU side's
// exp4 polynomial vs the GPU exp differ inside the shared tolerance envelope).
class MetalSwiglu {
    @ssbo @binding = 0 g : array<float>     // [ntok x hidden] W1 gate, in place
    @ssbo @binding = 1 u : array<float>     // [ntok x hidden] W3 up
    @uniform @binding = 2 total : uint      // ntok * hidden — grid guard

    [metal_kernel(name="metal_swiglu_msl")]
    def metal_swiglu {
        let gid = gl_GlobalInvocationID.x
        if (gid >= total) {
            return
        }
        let vv = g[gid]
        g[gid] = (vv / (1.0 + exp(-vv))) * u[gid]
    }
}

// Residual add, one thread per element: x += y (add_inplace).
class MetalAdd {
    @ssbo @binding = 0 x : array<float>
    @ssbo @binding = 1 y : array<float>
    @uniform @binding = 2 total : uint      // grid guard

    [metal_kernel(name="metal_add_msl")]
    def metal_add {
        let gid = gl_GlobalInvocationID.x
        if (gid >= total) {
            return
        }
        x[gid] += y[gid]
    }
}

// ===== the whole-prefill driver =====
// Registered as prefill override "metal" (DASLLAMA_PIN_PREFILL). One command buffer per prefill;
// ONE commit+wait, then unified-memory readback hands x_b + per-layer roped-K/raw-V back to the CPU.

var private g_pf_dev : MetalDevice?
var private g_pf_queue : MetalCommandQueue?
var private g_pf_failed = false
var private g_pf_regions : table<uint64; MetalBuffer?>    // base address -> resident weight/scale/norm region
var private g_pf_pool : MetalBufferPool
var private g_pf_upool : MetalBufferPool     // UNTRACKED pool: uniforms + rope tables (GPU-read-only)
var private g_min_npos = 64l
var private g_max_npos = 2048l          // naive-attention P footprint cap: heads x npos^2 floats
var private g_prefills = 0l
var private g_pso_gemm : MetalComputePipeline?
var private g_pso_gemm64 : MetalComputePipeline?
var private g_pso_quant : MetalComputePipeline?
var private g_pf_pso_rms : MetalComputePipeline?
var private g_pso_rope : MetalComputePipeline?
var private g_pso_qk : MetalComputePipeline?
var private g_pso_softmax : MetalComputePipeline?
var private g_pso_av : MetalComputePipeline?
var private g_pf_pso_swiglu : MetalComputePipeline?
var private g_pf_pso_add : MetalComputePipeline?
var private g_pso_addrmsq : MetalComputePipeline?
var private g_pso_swigluq : MetalComputePipeline?
var private g_pso_ropeqk : MetalComputePipeline?
var private g_pf_pso_mm : MetalComputePipeline?    // the production das mul_mm (34B q8 blocks + f32 X)
var private g_pso_qkmm : MetalComputePipeline?
var private g_pso_avmm : MetalComputePipeline?
var private g_pf_pso_gemv : MetalComputePipeline?
var private g_pso_kq_mm4 : MetalComputePipeline?       // K-quant mul_mm twins (W4/M2)
var private g_pso_kq_mm5 : MetalComputePipeline?
var private g_pso_kq_mm6 : MetalComputePipeline?
var private g_pf_pso_kq4 : MetalComputePipeline?       // ... and the kq classifier GEMVs
var private g_pf_pso_kq5 : MetalComputePipeline?
var private g_pf_pso_kq6 : MetalComputePipeline?      // classifier GEMV (logits-on-GPU)
var private g_pf_pso_gemv16 : MetalComputePipeline?    // ... its f16-scale twin (wscale_f16 models)
var private g_mm_legacy = false             // set_metal_prefill_mulmm_legacy — tests' exact-parity rail
var private g_pf_logits_cpu = false            // set_metal_prefill_logits_cpu — the logits A/B rail
// timing-attribution knockout (DASLLAMA_METAL_PREFILL_SKIP=attn|gemm|ew|nongemm): skips encoding
// that stage family (nongemm = attn AND ew, leaving the pure GEMM chain) — output is GARBAGE,
// gpu-time deltas attribute the budget. Debug rail only.
var private g_pf_skip = ""

//! Prefills below this npos decline to the CPU path (submit+readback overhead outweighs the win).
//! Set 1 to force every prefill onto the GPU (tests).
def public set_metal_prefill_min_npos(v : int64) {
    g_min_npos = v
}

//! Pin the CPU final-norm/classifier step (logits-on-GPU off) — the A/B and fallback rail.
//! `DASLLAMA_METAL_LOGITS=0` pins it process-wide.
def public set_metal_prefill_logits_cpu(v : bool) {
    g_pf_logits_cpu = v
}

//! Pin the legacy quantized-activation GEMM path (q8 X + planar weights) instead of the default
//! llama.cpp mul_mm path (f32 X + block_q8_0 blobs). The legacy path is bit-identical to the CPU
//! forward — the tests' exact-parity rail. `DASLLAMA_METAL_MULMM=0` pins it process-wide.
def public set_metal_prefill_mulmm_legacy(v : bool) {
    g_mm_legacy = v
}

//! (gpu prefills served, declines to the CPU path) since load — diagnostics / tests. The decline
//! machinery lives in dasllama_metal_common (W1 split), so the count reads from there.
def public metal_prefill_stats() : tuple<prefills : int64; declines : int64> {
    return (prefills = g_prefills, declines = g_prefill_declines)
}

//! Release every GPU object the driver holds — the tests' leak-gate rail (and the reset for
//! refilled weight arrays, same contract as metal_gemm_shutdown). Lazily re-initializes on the
//! next served prefill.
def public metal_prefill_shutdown {
    for (buf in values(g_pf_regions)) {
        if (buf != null) {
            metal_release(buf)
        }
        buf = null
    }
    unsafe {
        delete g_pf_regions
    }
    metal_blob_regions_release()
    pool_drain(g_pf_pool)
    pool_drain(g_pf_upool)
    if (g_pso_gemm != null) {
        metal_release(g_pso_gemm)
        g_pso_gemm = null
    }
    if (g_pso_gemm64 != null) {
        metal_release(g_pso_gemm64)
        g_pso_gemm64 = null
    }
    if (g_pso_quant != null) {
        metal_release(g_pso_quant)
        g_pso_quant = null
    }
    if (g_pf_pso_rms != null) {
        metal_release(g_pf_pso_rms)
        g_pf_pso_rms = null
    }
    if (g_pf_pso_gemv != null) {
        metal_release(g_pf_pso_gemv)
        g_pf_pso_gemv = null
    }
    if (g_pf_pso_gemv16 != null) {
        metal_release(g_pf_pso_gemv16)
        g_pf_pso_gemv16 = null
    }
    if (g_pso_rope != null) {
        metal_release(g_pso_rope)
        g_pso_rope = null
    }
    if (g_pso_qk != null) {
        metal_release(g_pso_qk)
        g_pso_qk = null
    }
    if (g_pso_softmax != null) {
        metal_release(g_pso_softmax)
        g_pso_softmax = null
    }
    if (g_pso_av != null) {
        metal_release(g_pso_av)
        g_pso_av = null
    }
    if (g_pf_pso_swiglu != null) {
        metal_release(g_pf_pso_swiglu)
        g_pf_pso_swiglu = null
    }
    if (g_pf_pso_add != null) {
        metal_release(g_pf_pso_add)
        g_pf_pso_add = null
    }
    if (g_pso_addrmsq != null) {
        metal_release(g_pso_addrmsq)
        g_pso_addrmsq = null
    }
    if (g_pso_swigluq != null) {
        metal_release(g_pso_swigluq)
        g_pso_swigluq = null
    }
    if (g_pso_ropeqk != null) {
        metal_release(g_pso_ropeqk)
        g_pso_ropeqk = null
    }
    if (g_pf_pso_mm != null) {
        metal_release(g_pf_pso_mm)
        g_pf_pso_mm = null
    }
    for (pso in [g_pso_kq_mm4, g_pso_kq_mm5, g_pso_kq_mm6, g_pf_pso_kq4, g_pf_pso_kq5, g_pf_pso_kq6]) {
        if (pso != null) {
            metal_release(pso)
        }
    }
    g_pso_kq_mm4 = null
    g_pso_kq_mm5 = null
    g_pso_kq_mm6 = null
    g_pf_pso_kq4 = null
    g_pf_pso_kq5 = null
    g_pf_pso_kq6 = null
    if (g_pso_qkmm != null) {
        metal_release(g_pso_qkmm)
        g_pso_qkmm = null
    }
    if (g_pso_avmm != null) {
        metal_release(g_pso_avmm)
        g_pso_avmm = null
    }
    if (g_pf_queue != null) {
        metal_release(g_pf_queue)
        g_pf_queue = null
    }
    if (g_pf_dev != null) {
        metal_release(g_pf_dev)
        g_pf_dev = null
    }
    g_pf_failed = false
}

def private pf_env_int(name : string; dflt : int) : int {
    return has_env_variable(name) ? to_int(get_env_variable(name)) : dflt
}

def private pf_compile_pso(source, entry : string; fastmath : bool; var ok : bool&) : MetalComputePipeline? {
    var err : string
    var pso = pipeline_from_source(g_pf_dev, source, entry, fastmath, err)
    if (pso == null) {
        to_log(LOG_ERROR, "dasLLAMA metal prefill: `{entry}` compile failed — {err}\n")
        ok = false
    }
    return pso
}

def private metal_prefill_init : bool {
    if (g_pf_pso_add != null) {
        return true
    }
    if (g_pf_failed) {
        return false
    }
    g_pf_dev = metal_create_system_default_device()
    if (g_pf_dev == null) {
        to_log(LOG_WARNING, "dasLLAMA metal prefill: no Metal device — prefill stays on the CPU\n")
        g_pf_failed = true
        return false
    }
    g_pf_queue = metal_new_command_queue(g_pf_dev)
    var ok = true
    g_pso_gemm = pf_compile_pso(metal_q8_gemm_msl, metal_q8_gemm_msl_entry, metal_q8_gemm_msl_fastmath, ok)
    g_pso_gemm64 = pf_compile_pso(metal_q8_gemm64_msl, metal_q8_gemm64_msl_entry, metal_q8_gemm64_msl_fastmath, ok)
    g_pso_quant = pf_compile_pso(metal_q8_quant_msl, metal_q8_quant_msl_entry, metal_q8_quant_msl_fastmath, ok)
    g_pf_pso_rms = pf_compile_pso(metal_rmsnorm_msl, metal_rmsnorm_msl_entry, metal_rmsnorm_msl_fastmath, ok)
    g_pso_rope = pf_compile_pso(metal_rope_msl, metal_rope_msl_entry, metal_rope_msl_fastmath, ok)
    g_pso_qk = pf_compile_pso(metal_attn_qk_msl, metal_attn_qk_msl_entry, metal_attn_qk_msl_fastmath, ok)
    g_pso_softmax = pf_compile_pso(metal_attn_softmax_msl, metal_attn_softmax_msl_entry, metal_attn_softmax_msl_fastmath, ok)
    g_pso_av = pf_compile_pso(metal_attn_av_msl, metal_attn_av_msl_entry, metal_attn_av_msl_fastmath, ok)
    g_pf_pso_swiglu = pf_compile_pso(metal_swiglu_msl, metal_swiglu_msl_entry, metal_swiglu_msl_fastmath, ok)
    g_pf_pso_add = pf_compile_pso(metal_add_msl, metal_add_msl_entry, metal_add_msl_fastmath, ok)
    g_pso_addrmsq = pf_compile_pso(metal_add_rms_quant_msl, metal_add_rms_quant_msl_entry, metal_add_rms_quant_msl_fastmath, ok)
    g_pso_swigluq = pf_compile_pso(metal_swiglu_quant_msl, metal_swiglu_quant_msl_entry, metal_swiglu_quant_msl_fastmath, ok)
    g_pso_ropeqk = pf_compile_pso(metal_rope_qk_msl, metal_rope_qk_msl_entry, metal_rope_qk_msl_fastmath, ok)
    g_pf_pso_mm = pf_compile_pso(metal_q8_mulmm_msl, metal_q8_mulmm_msl_entry, metal_q8_mulmm_msl_fastmath, ok)
    g_pso_kq_mm4 = pf_compile_pso(metal_kq_mulmm_k4_msl, metal_kq_mulmm_k4_msl_entry, metal_kq_mulmm_k4_msl_fastmath, ok)
    g_pso_kq_mm5 = pf_compile_pso(metal_kq_mulmm_k5_msl, metal_kq_mulmm_k5_msl_entry, metal_kq_mulmm_k5_msl_fastmath, ok)
    g_pso_kq_mm6 = pf_compile_pso(metal_kq_mulmm_k6_msl, metal_kq_mulmm_k6_msl_entry, metal_kq_mulmm_k6_msl_fastmath, ok)
    g_pf_pso_kq4 = pf_compile_pso(metal_kq_gemv_k4_msl, metal_kq_gemv_k4_msl_entry, metal_kq_gemv_k4_msl_fastmath, ok)
    g_pf_pso_kq5 = pf_compile_pso(metal_kq_gemv_k5_msl, metal_kq_gemv_k5_msl_entry, metal_kq_gemv_k5_msl_fastmath, ok)
    g_pf_pso_kq6 = pf_compile_pso(metal_kq_gemv_k6_msl, metal_kq_gemv_k6_msl_entry, metal_kq_gemv_k6_msl_fastmath, ok)
    g_pso_qkmm = pf_compile_pso(metal_attn_qk_mm_msl, metal_attn_qk_mm_msl_entry, metal_attn_qk_mm_msl_fastmath, ok)
    g_pso_avmm = pf_compile_pso(metal_attn_av_mm_msl, metal_attn_av_mm_msl_entry, metal_attn_av_mm_msl_fastmath, ok)
    g_pf_pso_gemv = pf_compile_pso(metal_q8_gemv_msl, metal_q8_gemv_msl_entry, metal_q8_gemv_msl_fastmath, ok)
    g_pf_pso_gemv16 = pf_compile_pso(metal_q8_gemv_s16_msl, metal_q8_gemv_s16_msl_entry, metal_q8_gemv_s16_msl_fastmath, ok)
    if (!ok) {
        metal_prefill_shutdown()
        g_pf_failed = true
        return false
    }
    to_log(LOG_INFO, "dasLLAMA metal prefill: resident path live on {metal_device_name(g_pf_dev)}\n")
    return true
}

// lazily upload one stable region (weight quants / scales / norm vectors), keyed by base address
def private pf_upload_region(src : void?; bytes : uint64) : MetalBuffer? {
    let key = intptr(src)
    if (key_exists(g_pf_regions, key)) {
        return g_pf_regions[key]
    }
    // UNTRACKED: weights are GPU-read-only (written once, before any commit) — tracking ~450 of them made scheduler dependency analysis the dominant non-GPU cost (~70ms/prefill on 3B)
    var buf = metal_new_buffer_untracked(g_pf_dev, bytes)
    unsafe {
        memcpy(metal_buffer_contents(buf), src, int(bytes))
    }
    g_pf_regions[key] = buf
    return buf
}

// per-tensor block_q8_0 blob on THIS driver's device — picks the model's live scale plane
// (f32, or the raw-f16 qscales16 under wscale_f16; the encoding is part of the cache identity)
def private upload_blob_region(t : Model; off, rows, k : int64) : MetalBuffer? {
    unsafe {
        let s = t.wscale_f16 ? addr < void? >(t.qscales16[off / 32l]) : addr < void? >(t.qscales[off / 32l])
        return metal_blob_region(g_pf_dev, addr < void? >(t.qblob[off]), s, rows, k, t.wscale_f16)
    }
}

def private pf_uniform_u32(v : uint) : MetalBuffer? {
    var buf = pool_acquire_untracked(g_pf_upool, g_pf_dev, 4ul)
    var p = unsafe(reinterpret<uint?>(metal_buffer_contents(buf)))
    unsafe(p[0]) = v
    return buf
}

def private pf_uniform_f32(v : float) : MetalBuffer? {
    var buf = pool_acquire_untracked(g_pf_upool, g_pf_dev, 4ul)
    var p = unsafe(reinterpret<float?>(metal_buffer_contents(buf)))
    unsafe(p[0]) = v
    return buf
}

// only the llama-family std shape is wired (Llama-3.2 class — 1B hs=64 and 3B hs=128 both
// serve) — anything else declines to the CPU loop. Returns the FIRST failing clause; .none =
// supported.
[unused_argument(start_pos)]   // start_pos > 0 (a continuation chunk) is served (M2.5): the
// existing rows gather into the K/V panels and the causal diagonal shifts by qoff — the gate
// keeps the argument so future clauses can window it
def private prefill_decline(t : Model; s : Session; npos, start_pos : int64) : MetalPrefillDecline {
    if (npos < g_min_npos || npos > g_max_npos) {
        return MetalPrefillDecline.npos_window
    }
    if (t.quant != QuantMode.q8) {
        return MetalPrefillDecline.quant_mode
    }
    // kquant rides the mul_mm path only — mm pinned off or a shape the %64 gate rejects has no kq-capable GEMM and declines
    if (t.kquant_native) {
        let ck = t.config
        let qdk = ck.n_heads * ck.head_size
        // every fmt needs a GPU kernel: kq_load_gpu_supported rejects formats without one instead of dispatching a wrong-layout branch
        if (g_mm_legacy || get_env_variable("DASLLAMA_METAL_MULMM") == "0" ||
                (ck.dim % 64l) != 0l || (qdk % 64l) != 0l ||
                (layer_kv_dim(ck, 0l) % 64l) != 0l || (layer_hidden(t, 0l) % 64l) != 0l ||
                !kq_load_gpu_supported(t)) {
            return MetalPrefillDecline.kquant_native
        }
    }
    // wscale_f16 rides the mul_mm path only — a model the mm shape gate rejects has no s16-capable GEMM and declines
    if (t.wscale_f16) {
        let c16 = t.config
        let qd16 = c16.n_heads * c16.head_size
        if (g_mm_legacy || get_env_variable("DASLLAMA_METAL_MULMM") == "0" ||
                (c16.dim % 64l) != 0l || (qd16 % 64l) != 0l ||
                (layer_kv_dim(c16, 0l) % 64l) != 0l || (layer_hidden(t, 0l) % 64l) != 0l) {
            return MetalPrefillDecline.wscale_f16
        }
    }
    if (t.arch != "llama") {
        return MetalPrefillDecline.arch
    }
    // the GEMM kernel reads ROW-MAJOR Q8 weights — a repack backend's interleaved layout is unreadable here
    if (kernel_backend_needs_repack(active_kernel_backend())) {
        return MetalPrefillDecline.backend_repack
    }
    let c = t.config
    if ((c.rope_neox || c.attn_qkv_bias || c.qk_norm || c.v_norm || c.pre_post_norm) ||
            (c.layer_out_scale || c.attn_sinks || c.attn_logit_softcap > 0.0 || c.sliding_window > 0l) ||
            (c.ffn_act != FfnAct.silu || c.attn_scale > 0.0)) {
        return MetalPrefillDecline.config
    }
    let head_size = c.head_size
    let qd = c.n_heads * head_size
    let kv_dim = layer_kv_dim(c, 0l)
    let hidden = layer_hidden(t, 0l)
    // head_size % 32: the AV kernel grids d in 32-blocks; the QK kernel k-slabs any size
    if ((qd != c.dim || (head_size % 32l) != 0l) ||
            ((c.dim % 32l) != 0l || (kv_dim % 32l) != 0l || (hidden % 32l) != 0l)) {
        return MetalPrefillDecline.shape
    }
    for (l in range64(c.n_layers)) {
        if ((t.kv_src[l] != l || layer_kv_dim(c, l) != kv_dim || layer_head_size(c, l) != head_size) ||
                (layer_hidden(t, l) != hidden || layer_is_sliding(c, l) || t.wv_offs[l] < 0l)) {
            return MetalPrefillDecline.layers
        }
    }
    // the rope table is the kernel's input — forward_prefill_body builds it right before the hook
    if (int64(length(s.rope_cos)) < npos * (head_size / 2l)) {
        return MetalPrefillDecline.rope_rows
    }
    return MetalPrefillDecline.none
}

def private enc_gemm(enc : MetalComputeEncoder?; bwq, bws, bxq, bxs, by, bkdim, bndim : MetalBuffer?; mp, d : int64; yoff : uint64 = 0ul) {
    let use64 = gemm_use64(mp, d)
    metal_set_pipeline(enc, use64 ? g_pso_gemm64 : g_pso_gemm)
    metal_set_threadgroup_memory_length(enc, use64 ? metal_q8_gemm64_msl_tgmem : metal_q8_gemm_msl_tgmem, 0)
    metal_set_buffer(enc, bwq, 0ul, 0)
    metal_set_buffer(enc, bws, 0ul, 1)
    metal_set_buffer(enc, bxq, 0ul, 2)
    metal_set_buffer(enc, bxs, 0ul, 3)
    metal_set_buffer(enc, by, yoff, 4)
    metal_set_buffer(enc, bkdim, 0ul, 5)
    metal_set_buffer(enc, bndim, 0ul, 6)
    let groups = uint(use64 ? (mp / 64l) * (d / 64l) : (mp / 32l) * (d / 32l))
    metal_dispatch_threadgroups(enc, uint3(groups, 1u, 1u), uint3(128u, 1u, 1u))
}

// the production mul_mm dispatch: W blob bound twice (half-scale + byte views), raw f32 X,
// grid (mp/32 token tiles, d/64 output tiles). Requires d % 64 == 0 and mp % 64 == 0.
// yoff places the K/V outputs past a continuation's existing panel rows.
def private enc_gemm_mm(enc : MetalComputeEncoder?; bwblob, bx, by, bk, bn : MetalBuffer?; mp, d : int64; yoff : uint64 = 0ul) {
    metal_set_pipeline(enc, g_pf_pso_mm)
    metal_set_threadgroup_memory_length(enc, metal_q8_mulmm_msl_tgmem, 0)
    metal_set_buffer(enc, bwblob, 0ul, 0)
    metal_set_buffer(enc, bwblob, 0ul, 1)
    metal_set_buffer(enc, bx, 0ul, 2)
    metal_set_buffer(enc, by, yoff, 3)
    metal_set_buffer(enc, bk, 0ul, 4)
    metal_set_buffer(enc, bn, 0ul, 5)
    metal_dispatch_threadgroups(enc, uint3(uint(mp / 32l), uint(d / 64l), 1u), uint3(128u, 1u, 1u))
}

def private pf_fmt_at(a : array<KqFmt>; l : int64) : KqFmt => empty(a) ? KqFmt.q8 : a[l]

// one kq weight site on the prefill mul_mm path: the format's mul_mm twin over the kq plane
// pair (k6 binds its scale buffer twice — sub-scale blocks at 0, the trailing f16 d plane at
// byte nsb*16). Same grid/tgmem as enc_gemm_mm.
def private pf_enc_kq_site_mm(enc : MetalComputeEncoder?; t : Model; fmt : KqFmt; woff, rows, n : int64;
                              bx, by, bk, bn : MetalBuffer?; mp : int64; yoff : uint64 = 0ul) {
    var bq = kq_upload_quants(g_pf_dev, t, fmt, woff, rows, n)
    var bs = kq_upload_scales(g_pf_dev, t, fmt, woff, rows, n)
    if (fmt == KqFmt.k6) {
        metal_set_pipeline(enc, g_pso_kq_mm6)
        metal_set_threadgroup_memory_length(enc, metal_kq_mulmm_k6_msl_tgmem, 0)
        metal_set_buffer(enc, bs, uint64(((rows * n) / 256l) * 16l), 0)
    } else {
        metal_set_pipeline(enc, fmt == KqFmt.k4 ? g_pso_kq_mm4 : g_pso_kq_mm5)
        metal_set_threadgroup_memory_length(enc, fmt == KqFmt.k4 ? metal_kq_mulmm_k4_msl_tgmem : metal_kq_mulmm_k5_msl_tgmem, 0)
        metal_set_buffer(enc, bs, 0ul, 0)
    }
    metal_set_buffer(enc, bs, 0ul, 1)
    metal_set_buffer(enc, bq, 0ul, 2)
    metal_set_buffer(enc, bx, 0ul, 3)
    metal_set_buffer(enc, by, yoff, 4)
    metal_set_buffer(enc, bk, 0ul, 5)
    metal_set_buffer(enc, bn, 0ul, 6)
    metal_dispatch_threadgroups(enc, uint3(uint(mp / 32l), uint(rows / 64l), 1u), uint3(128u, 1u, 1u))
}

// the kq classifier GEMV (one f32 row against the [vocab x dim] kq plane) — the moved decode
// kernels' PSOs, prefill-owned twins
def private pf_enc_kq_gemv(enc : MetalComputeEncoder?; t : Model; fmt : KqFmt; woff, rows, n : int64;
                           bx : MetalBuffer?; xoff : uint64; by, bn, bd : MetalBuffer?) {
    var bq = kq_upload_quants(g_pf_dev, t, fmt, woff, rows, n)
    var bs = kq_upload_scales(g_pf_dev, t, fmt, woff, rows, n)
    if (fmt == KqFmt.k6) {
        metal_set_pipeline(enc, g_pf_pso_kq6)
        metal_set_buffer(enc, bs, uint64(((rows * n) / 256l) * 16l), 0)
    } else {
        metal_set_pipeline(enc, fmt == KqFmt.k4 ? g_pf_pso_kq4 : g_pf_pso_kq5)
        metal_set_buffer(enc, bs, 0ul, 0)
    }
    metal_set_buffer(enc, bs, 0ul, 1)
    metal_set_buffer(enc, bq, 0ul, 2)
    metal_set_buffer(enc, bx, xoff, 3)
    metal_set_buffer(enc, by, 0ul, 4)
    metal_set_buffer(enc, bn, 0ul, 5)
    metal_set_buffer(enc, bd, 0ul, 6)
    metal_dispatch_threadgroups(enc, uint3(uint((rows + 3l) / 4l), 1u, 1u), uint3(64u, 1u, 1u))
}

def private enc_quant(enc : MetalComputeEncoder?; bx, bxq, bxs, bn, bnblk : MetalBuffer?; nblk : int64) {
    metal_set_pipeline(enc, g_pso_quant)
    metal_set_buffer(enc, bx, 0ul, 0)
    metal_set_buffer(enc, bxq, 0ul, 1)
    metal_set_buffer(enc, bxs, 0ul, 2)
    metal_set_buffer(enc, bn, 0ul, 3)
    metal_set_buffer(enc, bnblk, 0ul, 4)
    metal_dispatch_threadgroups(enc, uint3(uint((nblk + 63l) / 64l), 1u, 1u), uint3(64u, 1u, 1u))
}

def private pf_enc_rms(enc : MetalComputeEncoder?; bx, bw, by, bdim, beps : MetalBuffer?; nrows : int64) {
    metal_set_pipeline(enc, g_pf_pso_rms)
    metal_set_threadgroup_memory_length(enc, metal_rmsnorm_msl_tgmem, 0)
    metal_set_buffer(enc, bx, 0ul, 0)
    metal_set_buffer(enc, bw, 0ul, 1)
    metal_set_buffer(enc, by, 0ul, 2)
    metal_set_buffer(enc, bdim, 0ul, 3)
    metal_set_buffer(enc, beps, 0ul, 4)
    metal_dispatch_threadgroups(enc, uint3(uint(nrows), 1u, 1u), uint3(256u, 1u, 1u))
}

// the final rmsnorm on ONE row: bx bound at the last position's byte offset (one threadgroup)
def private enc_rms_last(enc : MetalComputeEncoder?; bx : MetalBuffer?; xoff : uint64; bw, by, bdim, beps : MetalBuffer?) {
    metal_set_pipeline(enc, g_pf_pso_rms)
    metal_set_threadgroup_memory_length(enc, metal_rmsnorm_msl_tgmem, 0)
    metal_set_buffer(enc, bx, xoff, 0)
    metal_set_buffer(enc, bw, 0ul, 1)
    metal_set_buffer(enc, by, 0ul, 2)
    metal_set_buffer(enc, bdim, 0ul, 3)
    metal_set_buffer(enc, beps, 0ul, 4)
    metal_dispatch_threadgroups(enc, uint3(1u, 1u, 1u), uint3(256u, 1u, 1u))
}

// the classifier GEMV: d rows of logits, 4 rows (one simdgroup each) per threadgroup; s16 reads
// the qscales16 halfword plane (wscale_f16 models)
def private pf_enc_gemv(enc : MetalComputeEncoder?; s16 : bool; bwq, bws, bx, by, bn, bd : MetalBuffer?; d : int64) {
    metal_set_pipeline(enc, s16 ? g_pf_pso_gemv16 : g_pf_pso_gemv)
    metal_set_buffer(enc, bwq, 0ul, 0)
    metal_set_buffer(enc, bws, 0ul, 1)
    metal_set_buffer(enc, bx, 0ul, 2)
    metal_set_buffer(enc, by, 0ul, 3)
    metal_set_buffer(enc, bn, 0ul, 4)
    metal_set_buffer(enc, bd, 0ul, 5)
    metal_dispatch_threadgroups(enc, uint3(uint((d + 3l) / 4l), 1u, 1u), uint3(128u, 1u, 1u))
}

def private enc_rope(enc : MetalComputeEncoder?; bv, bcos, bsin, bn, bhs, bnpairs : MetalBuffer?; npairs : int64; off : uint64 = 0ul) {
    metal_set_pipeline(enc, g_pso_rope)
    metal_set_buffer(enc, bv, off, 0)
    metal_set_buffer(enc, bcos, 0ul, 1)
    metal_set_buffer(enc, bsin, 0ul, 2)
    metal_set_buffer(enc, bn, 0ul, 3)
    metal_set_buffer(enc, bhs, 0ul, 4)
    metal_set_buffer(enc, bnpairs, 0ul, 5)
    metal_dispatch_threadgroups(enc, uint3(uint((npairs + 63l) / 64l), 1u, 1u), uint3(64u, 1u, 1u))
}

// the fused add?+rms+quant: one threadgroup per row, r folded in when baddv reads 1
def private enc_add_rms_quant(enc : MetalComputeEncoder?; bx, br, bw, bxq, bxs, bdim, beps, baddv : MetalBuffer?; nrows : int64) {
    metal_set_pipeline(enc, g_pso_addrmsq)
    metal_set_threadgroup_memory_length(enc, metal_add_rms_quant_msl_tgmem, 0)
    metal_set_buffer(enc, bx, 0ul, 0)
    metal_set_buffer(enc, br, 0ul, 1)
    metal_set_buffer(enc, bw, 0ul, 2)
    metal_set_buffer(enc, bxq, 0ul, 3)
    metal_set_buffer(enc, bxs, 0ul, 4)
    metal_set_buffer(enc, bdim, 0ul, 5)
    metal_set_buffer(enc, beps, 0ul, 6)
    metal_set_buffer(enc, baddv, 0ul, 7)
    metal_dispatch_threadgroups(enc, uint3(uint(nrows), 1u, 1u), uint3(256u, 1u, 1u))
}

def private enc_swiglu_quant(enc : MetalComputeEncoder?; bg, bu, bxq, bxs, bn, bnblk : MetalBuffer?; nblk : int64) {
    metal_set_pipeline(enc, g_pso_swigluq)
    metal_set_buffer(enc, bg, 0ul, 0)
    metal_set_buffer(enc, bu, 0ul, 1)
    metal_set_buffer(enc, bxq, 0ul, 2)
    metal_set_buffer(enc, bxs, 0ul, 3)
    metal_set_buffer(enc, bn, 0ul, 4)
    metal_set_buffer(enc, bnblk, 0ul, 5)
    metal_dispatch_threadgroups(enc, uint3(uint((nblk + 63l) / 64l), 1u, 1u), uint3(64u, 1u, 1u))
}

def private enc_rope_qk(enc : MetalComputeEncoder?; bq, bk, bcos, bsin, bqd, bkvd, bhs, bnpq, bnpall : MetalBuffer?; npairs : int64; koff : uint64 = 0ul) {
    metal_set_pipeline(enc, g_pso_ropeqk)
    metal_set_buffer(enc, bq, 0ul, 0)
    metal_set_buffer(enc, bk, koff, 1)
    metal_set_buffer(enc, bcos, 0ul, 2)
    metal_set_buffer(enc, bsin, 0ul, 3)
    metal_set_buffer(enc, bqd, 0ul, 4)
    metal_set_buffer(enc, bkvd, 0ul, 5)
    metal_set_buffer(enc, bhs, 0ul, 6)
    metal_set_buffer(enc, bnpq, 0ul, 7)
    metal_set_buffer(enc, bnpall, 0ul, 8)
    metal_dispatch_threadgroups(enc, uint3(uint((npairs + 63l) / 64l), 1u, 1u), uint3(64u, 1u, 1u))
}

// the ggml-geometry attention pair: same buffer lists as the trio, 32x64 C tiles, z = heads.
// The key grid spans nk64 (= np32's value — existing rows + the chunk on a continuation);
// bqoff/bqrows carry the causal shift and the score slab's row extent.
def private enc_qk_mm(enc : MetalComputeEncoder?; bq, bk, batt, bqd, bkvd, bhs, bkvm, bnp, bnp32, bscale, bqoff, bqrows : MetalBuffer?; mp, nk64, heads : int64) {
    metal_set_pipeline(enc, g_pso_qkmm)
    metal_set_threadgroup_memory_length(enc, metal_attn_qk_mm_msl_tgmem, 0)
    metal_set_buffer(enc, bq, 0ul, 0)
    metal_set_buffer(enc, bk, 0ul, 1)
    metal_set_buffer(enc, batt, 0ul, 2)
    metal_set_buffer(enc, bqd, 0ul, 3)
    metal_set_buffer(enc, bkvd, 0ul, 4)
    metal_set_buffer(enc, bhs, 0ul, 5)
    metal_set_buffer(enc, bkvm, 0ul, 6)
    metal_set_buffer(enc, bnp, 0ul, 7)
    metal_set_buffer(enc, bnp32, 0ul, 8)
    metal_set_buffer(enc, bscale, 0ul, 9)
    metal_set_buffer(enc, bqoff, 0ul, 10)
    metal_set_buffer(enc, bqrows, 0ul, 11)
    metal_dispatch_threadgroups(enc, uint3(uint(mp / 32l), uint(nk64 / 64l), uint(heads)), uint3(128u, 1u, 1u))
}

def private enc_av_mm(enc : MetalComputeEncoder?; batt, bv, bxb, bqd, bkvd, bhs, bkvm, bnp, bnp32, bqoff, bqrows : MetalBuffer?; mp, heads, head_size : int64) {
    metal_set_pipeline(enc, g_pso_avmm)
    metal_set_threadgroup_memory_length(enc, metal_attn_av_mm_msl_tgmem, 0)
    metal_set_buffer(enc, batt, 0ul, 0)
    metal_set_buffer(enc, bv, 0ul, 1)
    metal_set_buffer(enc, bxb, 0ul, 2)
    metal_set_buffer(enc, bqd, 0ul, 3)
    metal_set_buffer(enc, bkvd, 0ul, 4)
    metal_set_buffer(enc, bhs, 0ul, 5)
    metal_set_buffer(enc, bkvm, 0ul, 6)
    metal_set_buffer(enc, bnp, 0ul, 7)
    metal_set_buffer(enc, bnp32, 0ul, 8)
    metal_set_buffer(enc, bqoff, 0ul, 9)
    metal_set_buffer(enc, bqrows, 0ul, 10)
    metal_dispatch_threadgroups(enc, uint3(uint(mp / 32l), uint(head_size / 64l), uint(heads)), uint3(128u, 1u, 1u))
}

def private enc_qk(enc : MetalComputeEncoder?; bq, bk, batt, bqd, bkvd, bhs, bkvm, bnp, bnp32, bscale, bqoff, bqrows : MetalBuffer?; mp, nk64, heads : int64) {
    metal_set_pipeline(enc, g_pso_qk)
    metal_set_threadgroup_memory_length(enc, metal_attn_qk_msl_tgmem, 0)
    metal_set_buffer(enc, bq, 0ul, 0)
    metal_set_buffer(enc, bk, 0ul, 1)
    metal_set_buffer(enc, batt, 0ul, 2)
    metal_set_buffer(enc, bqd, 0ul, 3)
    metal_set_buffer(enc, bkvd, 0ul, 4)
    metal_set_buffer(enc, bhs, 0ul, 5)
    metal_set_buffer(enc, bkvm, 0ul, 6)
    metal_set_buffer(enc, bnp, 0ul, 7)
    metal_set_buffer(enc, bnp32, 0ul, 8)
    metal_set_buffer(enc, bscale, 0ul, 9)
    metal_set_buffer(enc, bqoff, 0ul, 10)
    metal_set_buffer(enc, bqrows, 0ul, 11)
    metal_dispatch_threadgroups(enc, uint3(uint(mp / 32l), uint(nk64 / 32l), uint(heads)), uint3(128u, 1u, 1u))
}

def private enc_softmax(enc : MetalComputeEncoder?; batt, bnp32, bqoff, bqrows : MetalBuffer?; npos, heads : int64) {
    metal_set_pipeline(enc, g_pso_softmax)
    metal_set_threadgroup_memory_length(enc, metal_attn_softmax_msl_tgmem, 0)
    metal_set_buffer(enc, batt, 0ul, 0)
    metal_set_buffer(enc, bnp32, 0ul, 1)
    metal_set_buffer(enc, bqoff, 0ul, 2)
    metal_set_buffer(enc, bqrows, 0ul, 3)
    metal_dispatch_threadgroups(enc, uint3(uint(npos), uint(heads), 1u), uint3(128u, 1u, 1u))
}

def private enc_av(enc : MetalComputeEncoder?; batt, bv, bxb, bqd, bkvd, bhs, bkvm, bnp, bnp32, bqoff, bqrows : MetalBuffer?; mp, heads, head_size : int64) {
    metal_set_pipeline(enc, g_pso_av)
    metal_set_threadgroup_memory_length(enc, metal_attn_av_msl_tgmem, 0)
    metal_set_buffer(enc, batt, 0ul, 0)
    metal_set_buffer(enc, bv, 0ul, 1)
    metal_set_buffer(enc, bxb, 0ul, 2)
    metal_set_buffer(enc, bqd, 0ul, 3)
    metal_set_buffer(enc, bkvd, 0ul, 4)
    metal_set_buffer(enc, bhs, 0ul, 5)
    metal_set_buffer(enc, bkvm, 0ul, 6)
    metal_set_buffer(enc, bnp, 0ul, 7)
    metal_set_buffer(enc, bnp32, 0ul, 8)
    metal_set_buffer(enc, bqoff, 0ul, 9)
    metal_set_buffer(enc, bqrows, 0ul, 10)
    metal_dispatch_threadgroups(enc, uint3(uint(mp / 32l), uint(head_size / 32l), uint(heads)), uint3(128u, 1u, 1u))
}

// swiglu and add share the (a, b, total) elementwise shape
def private pf_enc_ew2(enc : MetalComputeEncoder?; pso : MetalComputePipeline?; ba, bb, btot : MetalBuffer?; total : int64) {
    metal_set_pipeline(enc, pso)
    metal_set_buffer(enc, ba, 0ul, 0)
    metal_set_buffer(enc, bb, 0ul, 1)
    metal_set_buffer(enc, btot, 0ul, 2)
    metal_dispatch_threadgroups(enc, uint3(uint((total + 63l) / 64l), 1u, 1u), uint3(64u, 1u, 1u))
}

// the prefill half of the weights-epoch guard (see dasllama_metal_common): flush the module's
// own address-keyed region cache when a model (re)load may have recycled the addresses
var private g_pf_cache_epoch = -1l

def private pf_weight_caches_flush_on_reload {
    if (weights_epoch() == g_pf_cache_epoch) {
        return
    }
    weight_caches_flush_on_reload()   // the shared caches (blob regions live there too)
    for (buf in values(g_pf_regions)) {
        if (buf != null) {
            metal_release(buf)
        }
        buf = null
    }
    unsafe {
        delete g_pf_regions
    }
    g_pf_cache_epoch = weights_epoch()
}

def metal_prefill_forward(t : Model; var s : Session; npos, start_pos : int64) : bool {
    pf_weight_caches_flush_on_reload()
    var why = prefill_decline(t, s, npos, start_pos)
    if (why == MetalPrefillDecline.none && !metal_prefill_init()) {
        why = MetalPrefillDecline.device
    }
    if (why != MetalPrefillDecline.none) {
        decline(why)
        return false
    }
    let ts_all = ref_time_ticks()
    let c = t.config
    let dim = c.dim
    let head_size = c.head_size
    let n_heads = c.n_heads
    let qd = n_heads * head_size
    let kv_dim = layer_kv_dim(c, 0l)
    let hidden = layer_hidden(t, 0l)
    let kv_mul = n_heads / layer_n_kv_heads(c, 0l)
    let scale = 1.0 / sqrt(float(head_size))
    let half = head_size / 2l
    let mp = ((npos + 63l) / 64l) * 64l    // 64 = the big GEMM kernel's M tile; attention kernels only assume %32
    // a continuation chunk (M2.5) attends the session's existing rows too: the K/V panels hold
    // [0, start_pos) gathered rows + the chunk at the start_pos offset, keys pad to the QK key
    // grid's 64-tile, and the score slabs widen to nk64 columns (rows stay the mp queries)
    let nk = start_pos + npos
    let nk64 = ((nk + 63l) / 64l) * 64l
    let koff = uint64(start_pos * kv_dim * 4l)
    let maxn = max(qd, hidden)
    // pooled per-prefill buffers (activations sized mp rows — the GEMM has no edge masking; rows
    // past npos are never read by the norm/rope/attention/elementwise kernels, and stale bytes in
    // the requant image only ever land in pad output rows: C-block rows are independent)
    let bytes_x = uint64(mp * dim * 4l)
    let bytes_q = uint64(mp * qd * 4l)
    let bytes_kv = uint64(nk64 * kv_dim * 4l)
    let bytes_h = uint64(mp * hidden * 4l)
    let bytes_xq = uint64(mp * maxn)
    let bytes_xs = uint64(mp * (maxn / 32l) * 4l)
    let bytes_att = uint64(n_heads * mp * nk64 * 4l)   // per-head score slabs: mp queries x nk64 keys
    let bytes_tab = uint64(npos * half * 4l)
    var bx = pool_acquire(g_pf_pool, g_pf_dev, bytes_x)
    var bxb = pool_acquire(g_pf_pool, g_pf_dev, bytes_x)
    var bxb2 = pool_acquire(g_pf_pool, g_pf_dev, bytes_x)
    var bq = pool_acquire(g_pf_pool, g_pf_dev, bytes_q)
    var bhb = pool_acquire(g_pf_pool, g_pf_dev, bytes_h)
    var bhb2 = pool_acquire(g_pf_pool, g_pf_dev, bytes_h)
    var bxq = pool_acquire(g_pf_pool, g_pf_dev, bytes_xq)
    var bxs = pool_acquire(g_pf_pool, g_pf_dev, bytes_xs)
    var batt = pool_acquire(g_pf_pool, g_pf_dev, bytes_att)
    var bcos = pool_acquire_untracked(g_pf_upool, g_pf_dev, bytes_tab)
    var bsin = pool_acquire_untracked(g_pf_upool, g_pf_dev, bytes_tab)
    var bks : array<MetalBuffer?>
    var bvs : array<MetalBuffer?>
    bks |> reserve(int(c.n_layers))
    bvs |> reserve(int(c.n_layers))
    for (_l in range64(c.n_layers)) {
        bks |> push(pool_acquire(g_pf_pool, g_pf_dev, bytes_kv))
        bvs |> push(pool_acquire(g_pf_pool, g_pf_dev, bytes_kv))
    }
    // upload the embedded rows + this prefill's rope table (built for the chunk's GLOBAL
    // positions — build_rope_table takes start_pos)
    unsafe {
        memcpy(metal_buffer_contents(bx), addr < void? >(s.x_b[0]), int(npos * dim * 4l))
        memcpy(metal_buffer_contents(bcos), addr < void? >(s.rope_cos[0]), int(npos * half * 4l))
        memcpy(metal_buffer_contents(bsin), addr < void? >(s.rope_sin[0]), int(npos * half * 4l))
    }
    // continuation: gather the session's existing rows [0, start_pos) into every layer's panel
    // head as f32 (paged sessions gather through kv_runs; tq4 rows dequant ROTATED and un-rotate
    // per head — GPU attention runs in the raw basis). K rows are stored ROPED, so no re-rope.
    if (start_pos > 0l) {
        unsafe {
            let gather_nrun = kv_runs(s, 0l, start_pos, s.kv_runs)
            let runsg = addr < KVRun const? >(s.kv_runs[0])
            let sgp = (s.kv_dtype_k == KVDtype.tq4 || s.kv_dtype_v == KVDtype.tq4) ? addr < float const? >(s.kv_signs[0]) : null
            for (l in range64(c.n_layers)) {
                var kdst = reinterpret<float?>(metal_buffer_contents(bks[l]))
                var vdst = reinterpret<float?>(metal_buffer_contents(bvs[l]))
                let kcp = kv_layer_kbase(s, l, c.seq_len)
                let vcp = kv_layer_vbase(s, l, c.seq_len)
                for (r in range64(gather_nrun)) {
                    let rowbase = runsg[r].row0 - runsg[r].j0
                    let cnt = (runsg[r].j1 - runsg[r].j0) * kv_dim
                    kv_load_row(kcp, s.kv_dtype_k, kv_dim, rowbase + runsg[r].j0, 0l, cnt, kdst + runsg[r].j0 * kv_dim)
                    kv_load_row(vcp, s.kv_dtype_v, kv_dim, rowbase + runsg[r].j0, 0l, cnt, vdst + runsg[r].j0 * kv_dim)
                }
                if (s.kv_dtype_k == KVDtype.tq4) {
                    tq4_unrotate_rows(kdst, start_pos, kv_dim, head_size, sgp)
                }
                if (s.kv_dtype_v == KVDtype.tq4) {
                    tq4_unrotate_rows(vdst, start_pos, kv_dim, head_size, sgp)
                }
            }
        }
    }
    // layer-invariant uniforms (weight offsets ride the region cache, so nothing varies per layer)
    var u_dim = pf_uniform_u32(uint(dim))
    var u_qd = pf_uniform_u32(uint(qd))
    var u_kvd = pf_uniform_u32(uint(kv_dim))
    var u_hidden = pf_uniform_u32(uint(hidden))
    var u_hs = pf_uniform_u32(uint(head_size))
    var u_kvm = pf_uniform_u32(uint(kv_mul))
    var u_np = pf_uniform_u32(uint(nk))       // the attention kernels' KEY row count
    var u_np32 = pf_uniform_u32(uint(nk64))   // ... and the score slabs' column stride
    var u_qoff = pf_uniform_u32(uint(start_pos))
    var u_qrows = pf_uniform_u32(uint(mp))    // the score slabs' row extent (padded queries)
    var u_nblk_dim = pf_uniform_u32(uint(npos * (dim / 32l)))
    var u_nblk_qd = pf_uniform_u32(uint(npos * (qd / 32l)))
    var u_nblk_h = pf_uniform_u32(uint(npos * (hidden / 32l)))
    var u_npairs_q = pf_uniform_u32(uint(npos * qd / 2l))
    var u_npairs_k = pf_uniform_u32(uint(npos * kv_dim / 2l))
    var u_total_dim = pf_uniform_u32(uint(npos * dim))
    var u_total_h = pf_uniform_u32(uint(npos * hidden))
    var u_scale = pf_uniform_f32(scale)
    var u_eps = pf_uniform_f32(c.norm_eps)
    var u_addv0 = pf_uniform_u32(0u)
    var u_addv1 = pf_uniform_u32(1u)
    var u_npairs_all = pf_uniform_u32(uint(npos * (qd + kv_dim) / 2l))
    // the mul_mm GEMM path (the default): W as resident 34B q8-block blobs, raw f32 activations —
    // the X-quant dispatches disappear and the GEMMs run at mul_mm rate. Needs every GEMM output
    // dim % 64; DASLLAMA_METAL_MULMM=0 pins the legacy quantized path (bit-identical to CPU).
    let mm = (!g_mm_legacy && get_env_variable("DASLLAMA_METAL_MULMM") != "0" &&
        (dim % 64l) == 0l && (qd % 64l) == 0l && (kv_dim % 64l) == 0l && (hidden % 64l) == 0l)
    // logits-on-GPU (default ON, DASLLAMA_METAL_LOGITS=0 pins the CPU classifier): final rmsnorm
    // + classifier GEMV ride the last command buffer, and the caller's CPU final step is skipped
    // (prefill_override_logits_done). softcap / suppression / non-q8 classifiers decline to CPU.
    let cls_fmt = c.shared_weights ? t.emb_fmt : t.wcls_fmt
    let tied_f32 = c.shared_weights && !t.cls_q8 && cls_fmt == KqFmt.q8
    let gpu_logits = (!g_pf_logits_cpu && get_env_variable("DASLLAMA_METAL_LOGITS") != "0" && empty(g_pf_skip) &&
        !tied_f32 && c.final_logit_softcap <= 0.0 &&
        empty(t.suppress) && t.wcls_off >= 0l)
    var bxf = gpu_logits ? pool_acquire(g_pf_pool, g_pf_dev, uint64(dim * 4l)) : null
    var blog = gpu_logits ? pool_acquire(g_pf_pool, g_pf_dev, uint64(c.vocab_size * 4l)) : null
    var u_vocab = gpu_logits ? pf_uniform_u32(uint(c.vocab_size)) : null
    // the fused add+rms+quant stages one row in threadgroup memory — dim must fit its 3072 cap.
    // DASLLAMA_METAL_FUSE=0 pins the unfused dispatch set (the A/B rail). The lcpp path runs the
    // unfused elementwise set (no quant tails to fuse).
    let fused = !mm && dim <= 3072l && get_env_variable("DASLLAMA_METAL_FUSE") != "0"
    // ggml-geometry QK/AV (default): ~10x the old trio GEMMs' rate; needs hs % 64 (the AV grid)
    // and mp % 64 (the QK key grid — true by construction). DASLLAMA_METAL_ATTN=0 pins the trio.
    let mmattn = (head_size % 64l) == 0l && get_env_variable("DASLLAMA_METAL_ATTN") != "0"
    let ts_enc = ref_time_ticks()
    let setup_us = get_time_usec(ts_all)
    var err : string
    var gpu_ms : double
    var encode_ms : double
    // command-buffer split (DASLLAMA_METAL_NCB chunks): each chunk commits as soon as encoded, so
    // the scheduler analyzes chunk k while chunk k-1 executes. DASLLAMA_METAL_UNRETAINED=1 skips
    // per-dispatch retain/release too. Default ~4 layers/chunk (3B: slack 57ms -> 9ms, +10% pp512).
    let ncb = clamp(int64(pf_env_int("DASLLAMA_METAL_NCB", int((c.n_layers + 3l) / 4l))), 1l, c.n_layers)
    let unret = get_env_variable("DASLLAMA_METAL_UNRETAINED") == "1"
    let layers_per_cb = (c.n_layers + ncb - 1l) / ncb
    var cbs : array<MetalCommandBuffer?>
    cbs |> reserve(int(ncb))
    var cb : MetalCommandBuffer?
    var enc : MetalComputeEncoder?
    {
        for (l in range64(c.n_layers)) {
            if (l % layers_per_cb == 0l) {
                if (enc != null) {
                    metal_end_encoding(enc)
                    metal_release(enc)
                    metal_commit(cb)
                    cbs |> push(cb)
                }
                cb = unret ? metal_new_command_buffer_unretained(g_pf_queue) : metal_new_command_buffer(g_pf_queue)
                enc = metal_new_compute_encoder(cb)
            }
            unsafe {
                var bw_att = pf_upload_region(addr < void? >(t.fblob[t.rms_att_off + l * dim]), uint64(dim * 4l))
                var bw_ffn = pf_upload_region(addr < void? >(t.fblob[t.rms_ffn_off + l * dim]), uint64(dim * 4l))
                // attention: [pending W2 residual +] norm -> [requant ->] Q/K/V -> rope ->
                // scores+softmax -> P.V -> out-proj (fused path folds each residual add into the
                // FOLLOWING add+rms+quant; the lcpp path feeds normed f32 rows straight to mul_mm)
                if (g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                    if (fused) {
                        enc_add_rms_quant(enc, bx, bxb, bw_att, bxq, bxs, u_dim, u_eps, l > 0l ? u_addv1 : u_addv0, npos)
                    } else {
                        pf_enc_rms(enc, bx, bw_att, bxb, u_dim, u_eps, npos)
                        if (!mm) {
                            enc_quant(enc, bxb, bxq, bxs, u_dim, u_nblk_dim, npos * (dim / 32l))
                        }
                    }
                }
                if (g_pf_skip != "gemm") {
                    // K-quant tensors take their mul_mm twins per tensor (the gate pins the
                    // mm shape for kquant loads); q8-tagged tensors keep the blob path
                    let fq = pf_fmt_at(t.wq_fmt, l)
                    let fk = pf_fmt_at(t.wk_fmt, l)
                    let fv = pf_fmt_at(t.wv_fmt, l)
                    if (fq != KqFmt.q8 || fk != KqFmt.q8 || fv != KqFmt.q8) {
                        if (fq != KqFmt.q8) {
                            pf_enc_kq_site_mm(enc, t, fq, t.wq_offs[l], qd, dim, bxb, bq, u_dim, u_qd, mp)
                        } else {
                            enc_gemm_mm(enc, upload_blob_region(t, t.wq_offs[l], qd, dim), bxb, bq, u_dim, u_qd, mp, qd)
                        }
                        if (fk != KqFmt.q8) {
                            pf_enc_kq_site_mm(enc, t, fk, t.wk_offs[l], kv_dim, dim, bxb, bks[l], u_dim, u_kvd, mp, koff)
                        } else {
                            enc_gemm_mm(enc, upload_blob_region(t, t.wk_offs[l], kv_dim, dim), bxb, bks[l], u_dim, u_kvd, mp, kv_dim, koff)
                        }
                        if (fv != KqFmt.q8) {
                            pf_enc_kq_site_mm(enc, t, fv, t.wv_offs[l], kv_dim, dim, bxb, bvs[l], u_dim, u_kvd, mp, koff)
                        } else {
                            enc_gemm_mm(enc, upload_blob_region(t, t.wv_offs[l], kv_dim, dim), bxb, bvs[l], u_dim, u_kvd, mp, kv_dim, koff)
                        }
                    } elif (mm) {
                        var bwqb = upload_blob_region(t, t.wq_offs[l], qd, dim)
                        var bwkb = upload_blob_region(t, t.wk_offs[l], kv_dim, dim)
                        var bwvb = upload_blob_region(t, t.wv_offs[l], kv_dim, dim)
                        enc_gemm_mm(enc, bwqb, bxb, bq, u_dim, u_qd, mp, qd)
                        enc_gemm_mm(enc, bwkb, bxb, bks[l], u_dim, u_kvd, mp, kv_dim, koff)
                        enc_gemm_mm(enc, bwvb, bxb, bvs[l], u_dim, u_kvd, mp, kv_dim, koff)
                    } else {
                        var bwq = pf_upload_region(addr < void? >(t.qblob[t.wq_offs[l]]), uint64(qd * dim))
                        var bwq_s = pf_upload_region(addr < void? >(t.qscales[t.wq_offs[l] / 32l]), uint64(qd * (dim / 32l) * 4l))
                        var bwk = pf_upload_region(addr < void? >(t.qblob[t.wk_offs[l]]), uint64(kv_dim * dim))
                        var bwk_s = pf_upload_region(addr < void? >(t.qscales[t.wk_offs[l] / 32l]), uint64(kv_dim * (dim / 32l) * 4l))
                        var bwv = pf_upload_region(addr < void? >(t.qblob[t.wv_offs[l]]), uint64(kv_dim * dim))
                        var bwv_s = pf_upload_region(addr < void? >(t.qscales[t.wv_offs[l] / 32l]), uint64(kv_dim * (dim / 32l) * 4l))
                        enc_gemm(enc, bwq, bwq_s, bxq, bxs, bq, u_dim, u_qd, mp, qd)
                        enc_gemm(enc, bwk, bwk_s, bxq, bxs, bks[l], u_dim, u_kvd, mp, kv_dim, koff)
                        enc_gemm(enc, bwv, bwv_s, bxq, bxs, bvs[l], u_dim, u_kvd, mp, kv_dim, koff)
                    }
                }
                if (g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                    if (fused) {
                        enc_rope_qk(enc, bq, bks[l], bcos, bsin, u_qd, u_kvd, u_hs, u_npairs_q, u_npairs_all, npos * (qd + kv_dim) / 2l, koff)
                    } else {
                        enc_rope(enc, bq, bcos, bsin, u_qd, u_hs, u_npairs_q, npos * qd / 2l)
                        enc_rope(enc, bks[l], bcos, bsin, u_kvd, u_hs, u_npairs_k, npos * kv_dim / 2l, koff)
                    }
                }
                if (g_pf_skip != "attn" && g_pf_skip != "nongemm") {
                    if (g_pf_skip != "attn_qk") {
                        if (mmattn) {
                            enc_qk_mm(enc, bq, bks[l], batt, u_qd, u_kvd, u_hs, u_kvm, u_np, u_np32, u_scale, u_qoff, u_qrows, mp, nk64, n_heads)
                        } else {
                            enc_qk(enc, bq, bks[l], batt, u_qd, u_kvd, u_hs, u_kvm, u_np, u_np32, u_scale, u_qoff, u_qrows, mp, nk64, n_heads)
                        }
                    }
                    if (g_pf_skip != "attn_sm") {
                        enc_softmax(enc, batt, u_np32, u_qoff, u_qrows, npos, n_heads)
                    }
                    if (g_pf_skip != "attn_av") {
                        if (mmattn) {
                            enc_av_mm(enc, batt, bvs[l], bxb, u_qd, u_kvd, u_hs, u_kvm, u_np, u_np32, u_qoff, u_qrows, mp, n_heads, head_size)
                        } else {
                            enc_av(enc, batt, bvs[l], bxb, u_qd, u_kvd, u_hs, u_kvm, u_np, u_np32, u_qoff, u_qrows, mp, n_heads, head_size)
                        }
                    }
                }
                if (!mm && g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                    enc_quant(enc, bxb, bxq, bxs, u_qd, u_nblk_qd, npos * (qd / 32l))
                }
                if (g_pf_skip != "gemm") {
                    let fo = pf_fmt_at(t.wo_fmt, l)
                    if (fo != KqFmt.q8) {
                        pf_enc_kq_site_mm(enc, t, fo, t.wo_offs[l], dim, qd, bxb, bxb2, u_qd, u_dim, mp)
                    } elif (mm) {
                        var bwob = upload_blob_region(t, t.wo_offs[l], dim, qd)
                        enc_gemm_mm(enc, bwob, bxb, bxb2, u_qd, u_dim, mp, dim)
                    } else {
                        var bwo = pf_upload_region(addr < void? >(t.qblob[t.wo_offs[l]]), uint64(dim * qd))
                        var bwo_s = pf_upload_region(addr < void? >(t.qscales[t.wo_offs[l] / 32l]), uint64(dim * (qd / 32l) * 4l))
                        enc_gemm(enc, bwo, bwo_s, bxq, bxs, bxb2, u_qd, u_dim, mp, dim)
                    }
                }
                // ffn: [WO residual +] norm -> [requant ->] W1/W3 -> swiglu[+requant] -> W2
                if (g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                    if (fused) {
                        enc_add_rms_quant(enc, bx, bxb2, bw_ffn, bxq, bxs, u_dim, u_eps, u_addv1, npos)
                    } else {
                        pf_enc_ew2(enc, g_pf_pso_add, bx, bxb2, u_total_dim, npos * dim)
                        pf_enc_rms(enc, bx, bw_ffn, bxb, u_dim, u_eps, npos)
                        if (!mm) {
                            enc_quant(enc, bxb, bxq, bxs, u_dim, u_nblk_dim, npos * (dim / 32l))
                        }
                    }
                }
                if (g_pf_skip != "gemm") {
                    let f1 = pf_fmt_at(t.w1_fmt, l)
                    let f3 = pf_fmt_at(t.w3_fmt, l)
                    if (f1 != KqFmt.q8 || f3 != KqFmt.q8) {
                        if (f1 != KqFmt.q8) {
                            pf_enc_kq_site_mm(enc, t, f1, t.w1_offs[l], hidden, dim, bxb, bhb, u_dim, u_hidden, mp)
                        } else {
                            enc_gemm_mm(enc, upload_blob_region(t, t.w1_offs[l], hidden, dim), bxb, bhb, u_dim, u_hidden, mp, hidden)
                        }
                        if (f3 != KqFmt.q8) {
                            pf_enc_kq_site_mm(enc, t, f3, t.w3_offs[l], hidden, dim, bxb, bhb2, u_dim, u_hidden, mp)
                        } else {
                            enc_gemm_mm(enc, upload_blob_region(t, t.w3_offs[l], hidden, dim), bxb, bhb2, u_dim, u_hidden, mp, hidden)
                        }
                    } elif (mm) {
                        var bw1b = upload_blob_region(t, t.w1_offs[l], hidden, dim)
                        var bw3b = upload_blob_region(t, t.w3_offs[l], hidden, dim)
                        enc_gemm_mm(enc, bw1b, bxb, bhb, u_dim, u_hidden, mp, hidden)
                        enc_gemm_mm(enc, bw3b, bxb, bhb2, u_dim, u_hidden, mp, hidden)
                    } else {
                        var bw1 = pf_upload_region(addr < void? >(t.qblob[t.w1_offs[l]]), uint64(hidden * dim))
                        var bw1_s = pf_upload_region(addr < void? >(t.qscales[t.w1_offs[l] / 32l]), uint64(hidden * (dim / 32l) * 4l))
                        var bw3 = pf_upload_region(addr < void? >(t.qblob[t.w3_offs[l]]), uint64(hidden * dim))
                        var bw3_s = pf_upload_region(addr < void? >(t.qscales[t.w3_offs[l] / 32l]), uint64(hidden * (dim / 32l) * 4l))
                        enc_gemm(enc, bw1, bw1_s, bxq, bxs, bhb, u_dim, u_hidden, mp, hidden)
                        enc_gemm(enc, bw3, bw3_s, bxq, bxs, bhb2, u_dim, u_hidden, mp, hidden)
                    }
                }
                if (g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                    if (fused) {
                        enc_swiglu_quant(enc, bhb, bhb2, bxq, bxs, u_hidden, u_nblk_h, npos * (hidden / 32l))
                    } else {
                        pf_enc_ew2(enc, g_pf_pso_swiglu, bhb, bhb2, u_total_h, npos * hidden)
                        if (!mm) {
                            enc_quant(enc, bhb, bxq, bxs, u_hidden, u_nblk_h, npos * (hidden / 32l))
                        }
                    }
                }
                if (g_pf_skip != "gemm") {
                    let f2 = pf_fmt_at(t.w2_fmt, l)
                    if (f2 != KqFmt.q8) {
                        pf_enc_kq_site_mm(enc, t, f2, t.w2_offs[l], dim, hidden, bhb, bxb, u_hidden, u_dim, mp)
                    } elif (mm) {
                        var bw2b = upload_blob_region(t, t.w2_offs[l], dim, hidden)
                        enc_gemm_mm(enc, bw2b, bhb, bxb, u_hidden, u_dim, mp, dim)
                    } else {
                        var bw2 = pf_upload_region(addr < void? >(t.qblob[t.w2_offs[l]]), uint64(dim * hidden))
                        var bw2_s = pf_upload_region(addr < void? >(t.qscales[t.w2_offs[l] / 32l]), uint64(dim * (hidden / 32l) * 4l))
                        enc_gemm(enc, bw2, bw2_s, bxq, bxs, bxb, u_hidden, u_dim, mp, dim)
                    }
                }
                if (!fused && g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                    pf_enc_ew2(enc, g_pf_pso_add, bx, bxb, u_total_dim, npos * dim)
                }
            }
        }
        if (fused && g_pf_skip != "ew" && g_pf_skip != "nongemm") {
            pf_enc_ew2(enc, g_pf_pso_add, bx, bxb, u_total_dim, npos * dim)   // the last layer's pending W2 add
        }
        if (gpu_logits) {
            // final rmsnorm on the LAST position + classifier GEMV — the caller samples from
            // s.logits, so this closes the whole forward inside the GPU window
            unsafe {
                var bwfin = pf_upload_region(addr < void? >(t.fblob[t.rms_final_off]), uint64(dim * 4l))
                enc_rms_last(enc, bx, uint64((npos - 1l) * dim * 4l), bwfin, bxf, u_dim, u_eps)
                if (cls_fmt != KqFmt.q8) {
                    pf_enc_kq_gemv(enc, t, cls_fmt, t.wcls_off, c.vocab_size, dim, bxf, 0ul, blog, u_dim, u_vocab)
                } else {
                    var bwc = pf_upload_region(addr < void? >(t.qblob[t.wcls_off]), uint64(c.vocab_size * dim))
                    var bwc_s = (t.wscale_f16 ?
                        pf_upload_region(addr < void? >(t.qscales16[t.wcls_off / 32l]), uint64(c.vocab_size * (dim / 32l) * 2l)) :
                        pf_upload_region(addr < void? >(t.qscales[t.wcls_off / 32l]), uint64(c.vocab_size * (dim / 32l) * 4l)))
                    pf_enc_gemv(enc, t.wscale_f16, bwc, bwc_s, bxf, blog, u_dim, u_vocab, c.vocab_size)
                }
            }
        }
        metal_end_encoding(enc)
        metal_release(enc)
        encode_ms = double(get_time_usec(ts_enc)) / 1000.0lf
        metal_commit(cb)
        cbs |> push(cb)
    }
    // completion + readback INTERLEAVE: chunks complete in commit order, each completed chunk's
    // roped-K/raw-V rows stream into the CPU KV codec while later chunks keep the GPU busy (safe
    // the moment a chunk reports complete). The residual-stream copy needs the LAST chunk.
    var ran = true
    var gpu_t0 = 0.0lf
    var gpu_t1 = 0.0lf
    var readback_us = 0l
    unsafe {
        let store_nrun = kv_runs(s, start_pos, start_pos + npos, s.kv_runs)
        let runsp = addr < KVRun const? >(s.kv_runs[0])
        for (cbi, i in cbs, count()) {
            metal_wait_until_completed(cbi)
            let e = metal_command_buffer_error(cbi)
            if (!empty(e)) {
                ran = false
                err = e
            }
            let t0 = metal_command_buffer_gpu_start_time(cbi)
            let t1 = metal_command_buffer_gpu_end_time(cbi)
            if (i == 0 || t0 < gpu_t0) {
                gpu_t0 = t0
            }
            if (i == 0 || t1 > gpu_t1) {
                gpu_t1 = t1
            }
            metal_release(cbi)
            if (ran) {
                let ts_rb = ref_time_ticks()
                let l1 = min(int64(i + 1) * layers_per_cb, c.n_layers)
                for (l in range64(int64(i) * layers_per_cb, l1)) {
                    // the chunk's rows sit past a continuation's gathered panel head
                    var kpan = reinterpret<uint8?>(metal_buffer_contents(bks[l]))
                    var vpan = reinterpret<uint8?>(metal_buffer_contents(bvs[l]))
                    memcpy(addr < void? >(s.k_b[0]), reinterpret<void?>(kpan + int64(koff)), int(npos * kv_dim * 4l))
                    memcpy(addr < void? >(s.v_b[0]), reinterpret<void?>(vpan + int64(koff)), int(npos * kv_dim * 4l))
                    // tq4 sessions store in the ROTATED basis: the GPU ran attention unrotated,
                    // so rotate the readback rows per head before the codec store (the CPU
                    // prefill's exact discipline — dasllama_common's tq4 store path)
                    if (s.kv_dtype_k == KVDtype.tq4 || s.kv_dtype_v == KVDtype.tq4) {
                        let sgp = addr < float const? >(s.kv_signs[0])
                        if (s.kv_dtype_k == KVDtype.tq4) {
                            tq4_rotate_batch(s.k_b, npos, kv_dim, head_size, sgp)
                        }
                        if (s.kv_dtype_v == KVDtype.tq4) {
                            tq4_rotate_batch(s.v_b, npos, kv_dim, head_size, sgp)
                        }
                    }
                    kv_store_batch(kv_layer_kbase(s, l, c.seq_len), kv_layer_vbase(s, l, c.seq_len),
                        s.kv_dtype_k, s.kv_dtype_v, s.k_b, s.v_b, runsp, store_nrun, start_pos, npos, kv_dim)
                }
                readback_us += int64(get_time_usec(ts_rb))
            }
        }
    }
    cbs |> clear()
    unsafe {
        delete cbs
    }
    gpu_ms = (gpu_t1 - gpu_t0) * 1000.0lf   // whole-GPU window: first chunk start -> last chunk end
    let encwait_us = get_time_usec(ts_enc)
    let ok = ran
    if (ran) {
        let ts_rb = ref_time_ticks()
        unsafe {
            memcpy(addr < void? >(s.x_b[0]), metal_buffer_contents(bx), int(npos * dim * 4l))
            if (gpu_logits) {
                memcpy(addr < void? >(s.logits[0]), metal_buffer_contents(blog), int(c.vocab_size * 4l))
            }
        }
        if (gpu_logits) {
            prefill_override_logits_done()
        }
        readback_us += int64(get_time_usec(ts_rb))
        g_prefills++
    } else {
        to_log(LOG_ERROR, "dasLLAMA metal prefill: dispatch failed ({err}) — falling back to the CPU layer loop\n")
        decline(MetalPrefillDecline.gpu_error)
    }
    // return the pooled buffers (uniforms too); the weight regions stay resident
    pool_release(g_pf_pool, bx, bytes_x)
    pool_release(g_pf_pool, bxb, bytes_x)
    pool_release(g_pf_pool, bxb2, bytes_x)
    pool_release(g_pf_pool, bq, bytes_q)
    pool_release(g_pf_pool, bhb, bytes_h)
    pool_release(g_pf_pool, bhb2, bytes_h)
    pool_release(g_pf_pool, bxq, bytes_xq)
    pool_release(g_pf_pool, bxs, bytes_xs)
    pool_release(g_pf_pool, batt, bytes_att)
    if (gpu_logits) {
        pool_release(g_pf_pool, bxf, uint64(dim * 4l))
        pool_release(g_pf_pool, blog, uint64(c.vocab_size * 4l))
        pool_release(g_pf_upool, u_vocab, 4ul)
    }
    pool_release(g_pf_upool, bcos, bytes_tab)
    pool_release(g_pf_upool, bsin, bytes_tab)
    for (l in range64(c.n_layers)) {
        pool_release(g_pf_pool, bks[l], bytes_kv)
        pool_release(g_pf_pool, bvs[l], bytes_kv)
    }
    bks |> clear()      // non-owning handles — the pool owns them now
    bvs |> clear()
    unsafe {
        delete bks
        delete bvs
    }
    pool_release(g_pf_upool, u_dim, 4ul)
    pool_release(g_pf_upool, u_qd, 4ul)
    pool_release(g_pf_upool, u_kvd, 4ul)
    pool_release(g_pf_upool, u_hidden, 4ul)
    pool_release(g_pf_upool, u_hs, 4ul)
    pool_release(g_pf_upool, u_kvm, 4ul)
    pool_release(g_pf_upool, u_np, 4ul)
    pool_release(g_pf_upool, u_np32, 4ul)
    pool_release(g_pf_upool, u_qoff, 4ul)
    pool_release(g_pf_upool, u_qrows, 4ul)
    pool_release(g_pf_upool, u_nblk_dim, 4ul)
    pool_release(g_pf_upool, u_nblk_qd, 4ul)
    pool_release(g_pf_upool, u_nblk_h, 4ul)
    pool_release(g_pf_upool, u_npairs_q, 4ul)
    pool_release(g_pf_upool, u_npairs_k, 4ul)
    pool_release(g_pf_upool, u_total_dim, 4ul)
    pool_release(g_pf_upool, u_total_h, 4ul)
    pool_release(g_pf_upool, u_scale, 4ul)
    pool_release(g_pf_upool, u_eps, 4ul)
    pool_release(g_pf_upool, u_addv0, 4ul)
    pool_release(g_pf_upool, u_addv1, 4ul)
    pool_release(g_pf_upool, u_npairs_all, 4ul)
    if (ok) {
        let total_us = get_time_usec(ts_all)
        let repack_note = g_repack_us > 0l ? " weight-repack={float(g_repack_us) / 1000.0}ms(one-time)" : ""
        g_repack_us = 0l
        to_log(LOG_INFO, "dasLLAMA metal prefill: npos={npos} gemm={mm ? "mulmm" : "q8"} total={float(total_us) / 1000.0}ms setup={float(setup_us) / 1000.0}ms encode+wait={float(encwait_us) / 1000.0}ms (encode {encode_ms}ms, gpu {gpu_ms}ms) kv+readback={float(readback_us) / 1000.0}ms dispatches={(fused ? 15l : mm ? 17l : 21l) * c.n_layers + (fused ? 1l : 0l) + (gpu_logits ? 2l : 0l)} logits={gpu_logits ? "gpu" : "cpu"}{repack_note}\n")
    }
    return ok
}

[init]
def dasllama_metal_prefill_register() {
    // registered but dormant: only select_prefill_override("metal") (env rail
    // DASLLAMA_PIN_PREFILL) activates it, and unsupported shapes decline per call
    register_prefill_override("metal", @@metal_prefill_forward)
    if (has_env_variable("DASLLAMA_METAL_PREFILL_SKIP")) {
        g_pf_skip = get_env_variable("DASLLAMA_METAL_PREFILL_SKIP")
        to_log(LOG_WARNING, "dasLLAMA metal prefill: SKIP={g_pf_skip} — timing-attribution mode, output is GARBAGE\n")
    }
}
