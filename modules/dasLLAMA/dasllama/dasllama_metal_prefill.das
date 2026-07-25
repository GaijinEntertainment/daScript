options gen2
options _comment_hygiene = true

module dasllama_metal_prefill shared public

require math
require daslib/math_bits   // uint_bits_to_float in the mx4 GEMV (msl_emit as_type)
require strings
require daslib/fio
require metal/msl_shader
require metal/das_metal_boost
require dasllama/dasllama_common
require dasllama/dasllama_math
require dasllama/dasllama_math_metal
require dasllama/dasllama_metal_common   // decline enums/tables + the shared block_q8_0 blob cache (W1 split)
require dasllama/dasllama_metal_lens   // [metal_dispatch] — generates decode enc_* builders for shared classes defined here

// Phase-6 dasMetal: the full-GPU-resident prefill kernel set — everything between the GEMMs:
// rmsnorm, table-driven RoPE, fused causal-softmax attention scores, the P·V gather, fused
// SwiGLU, and the residual add (the drivers are BLOB-ONLY mul_mm since the .dlim redesign —
// the quantized-X rail and its quant/fusion kernels are gone). Each kernel mirrors its
// dasLLAMA CPU twin's math exactly (same formula, same rounding ops):
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


// One threadgroup per row: cooperative sum of squares (simd_sum + per-simdgroup partials),
// lane 0 folds partials into rsqrt(mean + eps), every thread scales its strided slice.
[metal_dispatch(name = "enc_rms", pso = "g_pso_rms", tgmem = "metal_rmsnorm_msl_tgmem", tg = 1024, grid = "1")]
class MetalRmsNorm {
    @ssbo @binding = 0 @role = "read" x : array<float>     // [nrows x dim] source
    @ssbo @binding = 1 @role = "weight" w : array<float>     // [dim] weight
    @ssbo @binding = 2 @role = "write" y : array<float>     // [nrows x dim] out (x aliasing is safe: x is fully
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

// MetalQ8MulMm with the A stage swapped to native BF16 rows (no dequant: widen = exact bit
// shift, then RNE to the f16 tile). Serves the E-series per_layer_model_proj GEMM straight off
// the kept-bf16 blob — no resident f32 copy. Same %64 shape gates, kdim % 32.
class MetalBf16MulMm {
    @ssbo @binding = 0 wbh : array<ushort4> // bf16 W rows, ushort4 view (row r elem k at (r*kdim+k)/4)
    @ssbo @binding = 2 xf : array<float4>   // raw f32 activations, float4 view
    @ssbo @binding = 3 y : array<float>
    @uniform @binding = 4 kdim : uint
    @uniform @binding = 5 ndim : uint
    @workgroup ta : float16[2048]
    @workgroup tb : float16[1024]

    [metal_kernel(name="metal_bf16_mulmm_msl")]
    def metal_bf16_mulmm {
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
        var wcur = unsafe(addr(wbh[((nBase + lr0) * kdim + il0 * 16u) / 4u]))
        var xcur = unsafe(addr(xf[(mBase + lr1) * kdim4 + iy / 4u]))
        var kb = 0u
        while (kb < nkb) {
            let b0 = unsafe(xcur[0])
            let b1 = unsafe(xcur[1])
            for [unroll_full] (q in range(4)) {
                let u4 = unsafe(wcur[q])
                va[q * 4 + 0] = float16(uint_bits_to_float(uint(u4.x) << 16u))
                va[q * 4 + 1] = float16(uint_bits_to_float(uint(u4.y) << 16u))
                va[q * 4 + 2] = float16(uint_bits_to_float(uint(u4.z) << 16u))
                va[q * 4 + 3] = float16(uint_bits_to_float(uint(u4.w) << 16u))
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
            wcur = unsafe(wcur + 8)
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

// E-series PLE gather (GPU pre-step, stage 1): dequant token row slices of the Q8_0
// per_layer_token_embd region straight into the LAYER-major side-input panel, sqrt(ple) folded.
// One tg per (layer, position) slice; binds ride blob_of's q8 pair at the region base.
class MetalPleGatherQ8 {
    @ssbo @binding = 0 wsh : array<float16> // ple_emb q8 region, half-scale view (block b at 17*b)
    @ssbo @binding = 1 wqb : array<int8>    // the SAME region, byte view (quants at 34*b + 2)
    @ssbo @binding = 2 tok : array<uint>    // [npos] token ids
    @ssbo @binding = 3 y : array<float>     // [n_layers x npos x ple] layer-major side input
    @uniform @binding = 4 npos : uint
    @uniform @binding = 5 ple : uint
    @uniform @binding = 6 nlayers : uint
    @uniform @binding = 7 sc0 : float       // sqrt(ple)

    [metal_kernel(name="metal_ple_gather_q8_msl")]
    def metal_ple_gather_q8 {
        let l = gl_WorkGroupID.x
        let p = gl_WorkGroupID.y
        let i = gl_LocalInvocationID.x      // tg = ple threads (gate: ple % 32 == 0, <= 1024)
        let el = tok[p] * (nlayers * ple) + l * ple + i
        let blk = el / 32u
        let sc = float(wsh[blk * 17u])
        let q = float(int(wqb[blk * 34u + 2u + (el % 32u)]))
        y[(l * npos + p) * ple + i] = q * sc * sc0
    }
}

// E-series PLE finish (GPU pre-step, stage 2): per (layer, position) ple-slice, mirror of
// ple_pre_finish's tail — v = proj/sqrt(dim); side = (side + pnorm·(v·rsqrt(mean(v²)+eps)))/sqrt(2),
// in place on the layer-major panel the gather wrote. proj arrives position-major off the GEMM.
class MetalPleFinish {
    @ssbo @binding = 0 proj : array<float>  // [mp x n_layers*ple] position-major model_proj·embd
    @ssbo @binding = 1 pnorm : array<float> // [ple] proj-norm weight
    @ssbo @binding = 2 y : array<float>     // [n_layers x npos x ple] side input, in place
    @uniform @binding = 3 npos : uint
    @uniform @binding = 4 ple : uint
    @uniform @binding = 5 nlayers : uint
    @uniform @binding = 6 inv_sd : float    // 1/sqrt(dim)
    @uniform @binding = 7 eps : float
    @uniform @binding = 8 inv_s2 : float    // 1/sqrt(2)
    @workgroup partial : float[32]
    @workgroup inv_sh : float

    [metal_kernel(name="metal_ple_finish_msl")]
    def metal_ple_finish {
        let l = gl_WorkGroupID.x
        let p = gl_WorkGroupID.y
        let i = gl_LocalInvocationID.x      // tg = ple threads (gate: ple % 32 == 0, <= 1024)
        let v = proj[p * (nlayers * ple) + l * ple + i] * inv_sd
        let ss = simd_sum(v * v)
        if (gl_SubgroupInvocationID == 0u) {
            partial[gl_SubgroupID] = ss
        }
        barrier()
        if (i == 0u) {
            var tot = 0.0
            var sgi = 0u
            while (sgi < gl_NumSubgroups) {
                tot += partial[sgi]
                sgi++
            }
            inv_sh = rsqrt(tot / float(ple) + eps)
        }
        barrier()
        let yi = (l * npos + p) * ple + i
        y[yi] = (y[yi] + pnorm[i] * (v * inv_sh)) * inv_s2
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
        var sv : uint4
        var dall = 0.0
        var kb = 0u
        while (kb < nkb) {
            let sb = kb / 8u
            let js = kb % 8u
            let blk = wrow * nsb + sb
            let b0 = unsafe(xcur[0])
            let b1 = unsafe(xcur[1])
            let half6 = js / 4u
            let gg = js % 4u
            if (js == 0u) {   // superblock scalars change every 8th kb; uniform branch, no divergence
                sv = ksc[blk]
                dall = float(kdh[blk])
            }
            let si = half6 * 8u + gg * 2u + il0
            let scw = si < 8u ? (si < 4u ? sv.x : sv.y) : (si < 12u ? sv.z : sv.w)
            let s6 = float(((int(scw >> ((si & 3u) * 8u)) & 255) ^ 128) - 128)
            let dsc = dall * s6
            let dmn = dsc * 32.0   // -32 folded to the fma form (k4's dmn shape)
            // byte-position shift folded into exact power-of-two pre-scales (lcpp dequantize_q6_K
            // shape): q<<8c is an exact f32 integer (6-bit quants), dsc/2^8c is an exponent shift,
            // so each fma sees the same real product — bit-identical, one shift fewer per element
            let dsc1 = dsc * 0.00390625
            let dsc2 = dsc * 0.0000152587890625
            let dsc3 = dsc * 0.000000059604644775390625
            let qb = blk * 48u + half6 * 16u + (gg & 1u) * 8u + il0 * 4u
            let hb = blk * 48u + 32u + half6 * 8u + il0 * 4u
            let nsh = (gg / 2u) * 4u
            let hsh = gg * 2u
            for [unroll_full] (k in range(4)) {
                let u = kqu[qb + uint(k)]
                let hu = kqu[hb + uint(k)]
                // all 4 bytes composed at once — cross-byte bleed of the uint-wide shifts is
                // masked off; per element only mask+fma remain (shift folded into dsc1..3)
                let qv = ((u >> nsh) & 0x0F0F0F0F) | (((hu >> hsh) & 0x03030303) << 4u)
                va[k * 4] = float16(dsc * float(qv & 255u) - dmn)
                va[k * 4 + 1] = float16(dsc1 * float(qv & 65280u) - dmn)
                va[k * 4 + 2] = float16(dsc2 * float(qv & 16711680u) - dmn)
                va[k * 4 + 3] = float16(dsc3 * float(qv & 4278190080u) - dmn)
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

// ===== MoE routing (Wave C) =====
// Router GEMV -> single-simdgroup select -> expert-indexed GEMV twins (grid.y = slot; the
// kernel reads sel[slot] GPU-side — no CPU readback, encode-ahead/spec stays compatible).

// Router GEMV over the uploaded fp32 router slab: y[ne] = W[ne x dim] . x per stream. ne <= 256
// rows — latency-trivial next to the expert GEMVs. grid.y = stream (batch; single decode = 1).
[metal_dispatch(name = "enc_moe_router", pso = "g_pso_moe_router", tg = "g_gemv_rows*32", grid = "ne/g_gemv_rows, nst", params = "ne : int64, nst : int64")]
class MetalMoeRouter {
    @ssbo @binding = 0 @role = "weight" w : array<float4>    // [ne x dim] router weights, float4 view
    @ssbo @binding = 1 @role = "read" x : array<float4>    // [streams x dim] activations, float4 view
    @ssbo @binding = 2 @role = "write" @span = "ne*nst*4" y : array<float>     // [streams x ne] logits out
    @uniform @binding = 3 ndim : uint       // dim (% 32)
    @uniform @binding = 4 ddim : uint       // ne
    @uniform @binding = 5 xs : uint         // per-stream x stride, elements
    @ssbo @binding = 6 @role = "weight" rb : array<float>    // [ne] router bias row (hasb models — gpt-oss)
    @uniform @binding = 7 hasb : uint

    [metal_kernel(name="metal_moe_router_msl")]
    def metal_moe_router {
        let row = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        if (row >= ddim) {
            return
        }
        let st = gl_WorkGroupID.y
        let lane = gl_SubgroupInvocationID
        let nb4 = ndim / 4u
        let x0e = st * (xs / 4u)
        var acc = 0.0
        var k = lane
        while (k < nb4) {
            let wv = w[row * nb4 + k]
            let xv = x[x0e + k]
            acc += wv.x * xv.x + wv.y * xv.y + wv.z * xv.z + wv.w * xv.w
            k += gl_SubgroupSize
        }
        acc = simd_sum(acc)
        if (lane == 0u) {
            y[st * ddim + row] = hasb != 0u ? acc + rb[row] : acc
        }
    }
}

// Position-blocked prefill router: MetalMoeRouter's shape but each sg serves 8 positions per
// weight load — slab SLC traffic /8 (the prefill router's entire cost); tails clamp, stores gate.
// Per-lane k stride and per-position dot order match MetalMoeRouter, so logits stay bit-identical.
class MetalMoeRouterB {
    @ssbo @binding = 0 w : array<float4>    // [ne x dim] router weights, float4 view
    @ssbo @binding = 1 x : array<float4>    // [streams x dim] activations, float4 view
    @ssbo @binding = 2 y : array<float>     // [streams x ne] logits out
    @uniform @binding = 3 ndim : uint       // dim (% 32)
    @uniform @binding = 4 ddim : uint       // ne
    @uniform @binding = 5 xs : uint         // per-stream x stride, elements
    @ssbo @binding = 6 rb : array<float>    // [ne] router bias row (hasb models — gpt-oss)
    @uniform @binding = 7 hasb : uint
    @uniform @binding = 8 ns : uint         // stream count (npos)

    [metal_kernel(name="metal_moe_router_b_msl")]
    def metal_moe_router_b {
        let row = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        if (row >= ddim) {
            return
        }
        let st0 = gl_WorkGroupID.y * 8u
        let lane = gl_SubgroupInvocationID
        let nb4 = ndim / 4u
        let xs4 = xs / 4u
        var acc : float[8]
        var k = lane
        while (k < nb4) {
            let wv = w[row * nb4 + k]
            for [unroll_full] (i in range(8)) {
                let st = min(st0 + uint(i), ns - 1u)
                let xv = x[st * xs4 + k]
                acc[i] += wv.x * xv.x + wv.y * xv.y + wv.z * xv.z + wv.w * xv.w
            }
            k += gl_SubgroupSize
        }
        for [unroll_full] (i in range(8)) {
            let s = simd_sum(acc[i])
            if (lane == 0u && st0 + uint(i) < ns) {
                y[(st0 + uint(i)) * ddim + row] = hasb != 0u ? s + rb[row] : s
            }
        }
    }
}

// Router select — moe_select_core's softmax + top-k + norm_topk on one simdgroup per stream
// (grid.x): per-lane strided ownership (ne <= 256), per-winner argmax butterfly with the CPU
// tie-break (lowest index), register knockout. Stream row layout: [k idx u32][k w f32].
[metal_dispatch(name = "enc_moe_select", pso = "g_pso_moe_select", tg = 32, grid = "nst", params = "nst : int64")]
class MetalMoeSelect {
    @ssbo @binding = 0 @role = "read" lg : array<float>     // [streams x ne] router logits
    @ssbo @binding = 1 @role = "write" seli : array<uint>    // selection buffer, uint view — idx of stream s at s*2k
    @ssbo @binding = 2 @role = "alias" selw : array<float>   // the same buffer, float view — w of stream s at s*2k + k
    @uniform @binding = 3 ne : uint
    @uniform @binding = 4 nk : uint
    @uniform @binding = 5 gmode : uint       // 0 = softmax + norm_topk; 1 = softmax_weight (gpt-oss); 2 = softmax, no renorm (qwen2moe)

    [metal_kernel(name="metal_moe_select_msl")]
    def metal_moe_select {
        let st = gl_WorkGroupID.x
        let lg0 = st * ne
        let sel0 = st * 2u * nk
        let lane = gl_SubgroupInvocationID
        var v : float[8]           // lane owns logits lane, lane+32, ... — 8 deep covers ne <= 256
        for [unroll_full] (t in range(8)) {
            let j = lane + uint(t) * 32u
            v[t] = j < ne ? lg[lg0 + j] : -3.0e38
        }
        // gmode 0/2: softmax over all ne (the CPU order: max, exp with the -80 floor, sum,
        // divide-all); gmode 1 (gpt-oss softmax_weight) selects on the RAW logits instead
        if (gmode != 1u) {
            var mx = v[0]
            for [unroll_full] (t in range(8)) {
                mx = max(mx, v[t])
            }
            var bm = 16u
            while (bm >= 1u) {
                mx = max(mx, simd_shuffle_xor(mx, bm))
                bm /= 2u
            }
            var psum = 0.0
            for [unroll_full] (t in range(8)) {
                let j = lane + uint(t) * 32u
                v[t] = j < ne ? exp(max(v[t] - mx, -80.0)) : 0.0
                psum += v[t]
            }
            let sum = simd_sum(psum)
            for [unroll_full] (t in range(8)) {
                v[t] /= sum
            }
        }
        var ws = 0.0
        var j = 0u
        while (j < nk) {
            var bv = -3.0e38
            var bt = 0u
            for [unroll_full] (t in range(8)) {
                if (v[t] > bv) {
                    bv = v[t]
                    bt = uint(t)
                }
            }
            var bi = lane + bt * 32u
            var m = 16u
            while (m >= 1u) {
                let ov = simd_shuffle_xor(bv, m)
                let oi = simd_shuffle_xor(bi, m)
                if (ov > bv || (ov == bv && oi < bi)) {
                    bv = ov
                    bi = oi
                }
                m /= 2u
            }
            if (lane == 0u) {
                seli[sel0 + j] = bi
                selw[sel0 + nk + j] = bv
            }
            ws += bv   // every lane holds the same winner value after the butterfly
            for [unroll_full] (t in range(8)) {   // owning lane knocks the winner out (constant indices)
                if (uint(t) == (bi >> 5u) && (bi & 31u) == lane) {
                    v[t] = -3.0e38
                }
            }
            j++
        }
        if (gmode == 1u) {
            // gpt-oss: softmax over just the selected k raw logits
            if (lane == 0u) {
                var mxw = selw[sel0 + nk]
                var i = 1u
                while (i < nk) {
                    mxw = max(mxw, selw[sel0 + nk + i])
                    i++
                }
                var sw = 0.0
                i = 0u
                while (i < nk) {
                    let ev = exp(max(selw[sel0 + nk + i] - mxw, -80.0))
                    selw[sel0 + nk + i] = ev
                    sw += ev
                    i++
                }
                i = 0u
                while (i < nk) {
                    selw[sel0 + nk + i] /= sw
                    i++
                }
            }
        } elif (gmode == 0u) {
            // norm_topk renorm to sum 1 (the CPU f16-min clamp); gmode 2 (qwen2moe) keeps
            // the softmax weights as selected — no renorm
            ws = max(ws, 6.103515625e-5)
            if (lane == 0u) {
                var i = 0u
                while (i < nk) {
                    selw[sel0 + nk + i] /= ws
                    i++
                }
            }
        }
    }
}

// Expert-indexed q8 GEMV — MetalQ8Gemv's exact dot with the expert plane picked GPU-side:
// the blob binds at the layer's expert STACK base, the kernel adds sel[slot] * eblk to the
// block index, reads x at slot * xs (0 = shared activation), writes y at slot * ddim.
class MetalMoeGemvQ8 {
    @ssbo @binding = 0 wsh : array<float16> // the expert stack's blob slice, half-scale view
    @ssbo @binding = 1 wqb : array<int8>    // the same blob buffer, byte view
    @ssbo @binding = 2 x : array<float4>
    @ssbo @binding = 3 y : array<float>
    @uniform @binding = 4 ndim : uint
    @uniform @binding = 5 ddim : uint
    @ssbo @binding = 6 sel : array<uint>    // [streams x 2k] selection rows (idx section read)
    @uniform @binding = 7 eblk : uint       // 34B blocks per expert plane (ege / 32)
    @uniform @binding = 8 xs : uint         // per-slot x stride, elements (0 = shared)
    @uniform @binding = 9 ss : uint         // per-stream sel row stride, uints (2k; 0 single)
    @uniform @binding = 10 bxs : uint       // per-stream x stride, elements (0 single)
    @uniform @binding = 11 bys : uint       // per-stream y stride, elements (0 single)

    [metal_kernel(name="metal_moe_gemv_q8_msl")]
    def metal_moe_gemv_q8 {
        let row = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        if (row >= ddim) {
            return
        }
        let slot = gl_WorkGroupID.y
        let st = gl_WorkGroupID.z
        let eb = sel[st * ss + slot] * eblk
        let lane = gl_SubgroupInvocationID
        let nb = ndim / 32u
        let nb4 = ndim / 4u
        let x0e = (st * bxs + slot * xs) / 4u
        var sp = unsafe(addr(wsh[(eb + row * nb + lane / 8u) * 17u]))
        var qp = unsafe(addr(wqb[(eb + row * nb + lane / 8u) * 34u + 2u + (lane % 8u) * 4u]))
        var x0 = unsafe(addr(x[x0e + lane]))
        var acc = 0.0
        var lw : float4[4]
        var k = lane
        while (k + 96u < nb4) {
            for [unroll_full] (c in range(4)) {
                let d = float(unsafe(sp[uint(c) * 68u]))
                let qb = uint(c) * 136u
                lw[c] = float4(float(unsafe(qp[qb])), float(unsafe(qp[qb + 1u])),
                    float(unsafe(qp[qb + 2u])), float(unsafe(qp[qb + 3u]))) * d
            }
            for [unroll_full] (c in range(4)) {
                let xv = unsafe(x0[uint(c) * 32u])
                acc += lw[c].x * xv.x + lw[c].y * xv.y + lw[c].z * xv.z + lw[c].w * xv.w
            }
            sp = unsafe(sp + 272)
            qp = unsafe(qp + 544)
            x0 = unsafe(x0 + 128)
            k += 128u
        }
        while (k < nb4) {
            let bi = eb + row * nb + k / 8u
            let d = float(wsh[bi * 17u])
            let qb = bi * 34u + 2u + (k % 8u) * 4u
            let xv = x[x0e + k]
            acc += (float(wqb[qb]) * xv.x + float(wqb[qb + 1u]) * xv.y +
                float(wqb[qb + 2u]) * xv.z + float(wqb[qb + 3u]) * xv.w) * d
            k += gl_SubgroupSize
        }
        acc = simd_sum(acc)
        if (lane == 0u) {
            y[st * bys + slot * ddim + row] = acc
        }
    }
}

// Expert-indexed Q4_K GEMV — MetalKqGemvK4's dot; the superblock index shifts by sel[slot] * esb.
// x rides a float4 view: the 32-scalar-x-load form was load-issue-bound at the routed expert
// shapes (nfe-row planes) — the vector view is 2.25x there (lab: bench_metal_moe_lab, 142→321 wGB/s).
class MetalMoeGemvK4 {
    @ssbo @binding = 0 ksh : array<float16> // 16B scale blocks, half view (d at 8*blk, dmin at +1)
    @ssbo @binding = 1 ks4 : array<uint4>   // the same buffer, uint4 view (whole block at blk)
    @ssbo @binding = 2 kqu : array<uint>    // k4 quant plane, uint view (qs of blk at 32*blk)
    @ssbo @binding = 3 x4 : array<float4>   // activations, float4 view (every x stride is % 4)
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint
    @ssbo @binding = 7 sel : array<uint>    // [streams x 2k] selection rows (idx section read)
    @uniform @binding = 8 esb : uint        // superblocks per expert plane (ege / 256)
    @uniform @binding = 9 xs : uint         // per-slot x stride, elements (0 = shared)
    @uniform @binding = 10 ss : uint        // per-stream sel row stride, uints (2k; 0 single)
    @uniform @binding = 11 bxs : uint       // per-stream x stride, elements (0 single)
    @uniform @binding = 12 bys : uint       // per-stream y stride, elements (0 single)

    [metal_kernel(name="metal_moe_gemv_k4_msl")]
    def metal_moe_gemv_k4 {
        let lane = gl_SubgroupInvocationID
        let ix = lane / 8u
        let it = lane % 8u
        let iq = it / 4u
        let ir = it % 4u
        let nb = ndim / 256u
        let slot = gl_WorkGroupID.y
        let st = gl_WorkGroupID.z
        let eb = sel[st * ss + slot] * esb
        let xoff = st * bxs + slot * xs
        let y0 = st * bys + slot * ddim
        let first_row = (gl_WorkGroupID.x * 2u + gl_SubgroupID) * 2u
        var yl : float[16]
        var yh : float[16]
        var sumf : float[2]
        var ib = ix
        while (ib < nb) {
            let yb4 = (xoff + ib * 256u + iq * 64u + ir * 8u) / 4u
            var sumy : float[4]
            for [unroll_full] (h in range(4)) {   // h: yl lo/hi, yh lo/hi — float4 pairs at 0/8/32/40
                let xi = yb4 + (h < 2 ? uint(h) * 8u : 16u + uint(h) * 8u)
                let va = x4[xi]
                let vb = x4[xi + 1u]
                let o = (h % 2) * 8
                if (h < 2) {   // constant under unroll_full — folds per iteration
                    yl[o] = va.x
                    yl[o + 1] = va.y
                    yl[o + 2] = va.z
                    yl[o + 3] = va.w
                    yl[o + 4] = vb.x
                    yl[o + 5] = vb.y
                    yl[o + 6] = vb.z
                    yl[o + 7] = vb.w
                } else {
                    yh[o] = va.x
                    yh[o + 1] = va.y
                    yh[o + 2] = va.z
                    yh[o + 3] = va.w
                    yh[o + 4] = vb.x
                    yh[o + 5] = vb.y
                    yh[o + 6] = vb.z
                    yh[o + 7] = vb.w
                }
                sumy[h] = va.x + va.y + va.z + va.w + vb.x + vb.y + vb.z + vb.w
            }
            for [unroll_full] (r in range(2)) {
                let blk = eb + (first_row + uint(r)) * nb + ib
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
                y[y0 + first_row + uint(r)] = s
            }
        }
    }
}

// Expert-indexed Q5_K GEMV — MetalKqGemvK5's dot with the expert superblock shift.
class MetalMoeGemvK5 {
    @ssbo @binding = 0 ksh : array<float16> // 16B scale blocks, half view
    @ssbo @binding = 1 ks4 : array<uint4>   // the same buffer, uint4 view
    @ssbo @binding = 2 kqb : array<uint8>   // k5 quant plane, byte view (qs at 160*blk, qh at +128)
    @ssbo @binding = 3 x : array<float>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint
    @ssbo @binding = 7 sel : array<uint>    // [streams x 2k] selection rows (idx section read)
    @uniform @binding = 8 esb : uint        // superblocks per expert plane (ege / 256)
    @uniform @binding = 9 xs : uint         // per-slot x stride, elements (0 = shared)
    @uniform @binding = 10 ss : uint        // per-stream sel row stride, uints (2k; 0 single)
    @uniform @binding = 11 bxs : uint       // per-stream x stride, elements (0 single)
    @uniform @binding = 12 bys : uint       // per-stream y stride, elements (0 single)

    [metal_kernel(name="metal_moe_gemv_k5_msl")]
    def metal_moe_gemv_k5 {
        let lane = gl_SubgroupInvocationID
        let tid = lane / 4u
        let ix = lane % 4u
        let iq = tid / 4u
        let ir = tid % 4u
        let l0 = ir * 8u
        let hb0 = 2u * iq
        let nb = ndim / 256u
        let slot = gl_WorkGroupID.y
        let st = gl_WorkGroupID.z
        let eb = sel[st * ss + slot] * esb
        let xoff = st * bxs + slot * xs
        let y0 = st * bys + slot * ddim
        let first_row = (gl_WorkGroupID.x * 2u + gl_SubgroupID) * 2u
        var yl : float[16]
        var yh : float[16]
        var sumf : float[2]
        var ib = ix
        while (ib < nb) {
            let yb = xoff + ib * 256u + iq * 64u + l0
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
                let blk = eb + (first_row + uint(r)) * nb + ib
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
                y[y0 + first_row + uint(r)] = s
            }
        }
    }
}

// Expert-indexed Q6_K GEMV — MetalKqGemvK6's dot; the expert shift covers sub-scales AND the
// trailing f16 d plane (both indexed by the same region-relative superblock).
class MetalMoeGemvK6 {
    @ssbo @binding = 0 kdh : array<float16> // the d plane — the scale buffer bound at byte nsb*16
    @ssbo @binding = 1 kscb : array<uint8>  // 16B sub-scale blocks, byte view (same buffer at 0)
    @ssbo @binding = 2 kqb : array<uint8>   // k6 quant plane, byte view (ql at 192*blk, qh at +128)
    @ssbo @binding = 3 x : array<float>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint
    @ssbo @binding = 7 sel : array<uint>    // [streams x 2k] selection rows (idx section read)
    @uniform @binding = 8 esb : uint        // superblocks per expert plane (ege / 256)
    @uniform @binding = 9 xs : uint         // per-slot x stride, elements (0 = shared)
    @uniform @binding = 10 ss : uint        // per-stream sel row stride, uints (2k; 0 single)
    @uniform @binding = 11 bxs : uint       // per-stream x stride, elements (0 single)
    @uniform @binding = 12 bys : uint       // per-stream y stride, elements (0 single)

    [metal_kernel(name="metal_moe_gemv_k6_msl")]
    def metal_moe_gemv_k6 {
        let lane = gl_SubgroupInvocationID
        let tid = lane / 2u
        let ix = lane % 2u
        let ip = tid / 8u
        let il = tid % 8u
        let l0 = il * 4u
        let is0 = ip * 8u + l0 / 16u
        let nb = ndim / 256u
        let slot = gl_WorkGroupID.y
        let st = gl_WorkGroupID.z
        let eb = sel[st * ss + slot] * esb
        let xoff = st * bxs + slot * xs
        let y0 = st * bys + slot * ddim
        let first_row = (gl_WorkGroupID.x * 2u + gl_SubgroupID) * 2u
        var yl : float[16]
        var sumf : float[2]
        var ib = ix
        while (ib < nb) {
            let yb = xoff + ib * 256u + ip * 128u + l0
            for [unroll_full] (l in range(4)) {
                yl[4 * l] = x[yb + uint(l)]
                yl[4 * l + 1] = x[yb + 32u + uint(l)]
                yl[4 * l + 2] = x[yb + 64u + uint(l)]
                yl[4 * l + 3] = x[yb + 96u + uint(l)]
            }
            for [unroll_full] (r in range(2)) {
                let blk = eb + (first_row + uint(r)) * nb + ib
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
                y[y0 + first_row + uint(r)] = s
            }
        }
    }
}

// Expert-indexed MXFP4 GEMV (gpt-oss): a 32-elem block = 16 nibble bytes (byte b holds elems
// b LOW / b+16 HIGH — the split-half layout) + one E8M0 scale byte; value = doubled-e2m1
// magnitude x halved scale (2^(e-128)). Optional per-expert bias rows — gpt-oss's biased MoE.
class MetalMoeGemvMx4 {
    @ssbo @binding = 0 mxu2 : array<uint2>  // nibble plane, uint2 view (block at 2*blk; off/2 % 8)
    @ssbo @binding = 1 mxe : array<uint8>   // E8M0 plane, bound at off/32
    @ssbo @binding = 2 xf : array<float4>   // activations, float4 view (xoff always % 4)
    @ssbo @binding = 3 y : array<float>
    @uniform @binding = 4 ndim : uint
    @uniform @binding = 5 ddim : uint
    @ssbo @binding = 6 sel : array<uint>    // [streams x 2k] selection rows (idx section read)
    @uniform @binding = 7 eblk : uint       // 32-elem blocks per expert plane (ege / 32)
    @uniform @binding = 8 xs : uint         // per-slot x stride, elements (0 = shared)
    @uniform @binding = 9 ss : uint         // per-stream sel row stride, uints (2k; 0 single)
    @uniform @binding = 10 bxs : uint       // per-stream x stride, elements (0 single)
    @uniform @binding = 11 bys : uint       // per-stream y stride, elements (0 single)
    @ssbo @binding = 12 wb : array<float>   // [ne x ddim] per-expert bias rows (hasb models)
    @uniform @binding = 13 hasb : uint
    @ssbo @binding = 15 vtab : array<float> // [16] signed doubled-e2m1 values (host-built)
    @workgroup vt : float[32]

    [metal_kernel(name="metal_moe_gemv_mx4_msl")]
    def metal_moe_gemv_mx4 {
        // the lcpp mul_mv_mxfp4 shape (moe-lab lcppe rung, 239->301 wGB/s at gpt-oss shapes):
        // 16 blocks in flight x 2 threads/block, yl[4] float4 x prefetch shared across both rows,
        // uint2 weight loads, tgmem value LUT, e8m0 scale computed in ALU (no dependent etab load)
        let lane = gl_SubgroupInvocationID
        vt[lane] = vtab[lane % 16u]
        barrier()
        let ix = lane / 2u   // 16-way block stride
        let it = lane % 2u   // 2 threads per block, 8 nibble bytes (one uint2) each
        let nb = ndim / 32u
        let slot = gl_WorkGroupID.y
        let st = gl_WorkGroupID.z
        let e = sel[st * ss + slot]
        let eb = e * eblk
        let xoff = st * bxs + slot * xs
        let y0 = st * bys + slot * ddim
        let first_row = (gl_WorkGroupID.x * 2u + gl_SubgroupID) * 2u
        var sumf : float[2]
        var ib = ix
        while (ib < nb) {
            let x4 = (xoff + ib * 32u + it * 8u) / 4u
            let yl0 = xf[x4]
            let yl1 = xf[x4 + 4u]
            let yl2 = xf[x4 + 1u]
            let yl3 = xf[x4 + 5u]
            for [unroll_full] (r in range(2)) {
                let blk = eb + (first_row + uint(r)) * nb + ib
                let u2 = mxu2[blk * 2u + it]
                let u0 = u2.x
                let u1 = u2.y
                let acc1 = yl0 * float4(vt[u0 & 15u], vt[(u0 >> 8u) & 15u], vt[(u0 >> 16u) & 15u], vt[(u0 >> 24u) & 15u])
                let acc2 = yl1 * float4(vt[(u0 >> 4u) & 15u], vt[(u0 >> 12u) & 15u], vt[(u0 >> 20u) & 15u], vt[u0 >> 28u])
                let acc3 = yl2 * float4(vt[u1 & 15u], vt[(u1 >> 8u) & 15u], vt[(u1 >> 16u) & 15u], vt[(u1 >> 24u) & 15u])
                let acc4 = yl3 * float4(vt[(u1 >> 4u) & 15u], vt[(u1 >> 12u) & 15u], vt[(u1 >> 20u) & 15u], vt[u1 >> 28u])
                let am = (acc1 + acc3) + (acc2 + acc4)
                let ex = uint(mxe[blk])
                let sc = uint_bits_to_float(ex < 2u ? (0x00200000u << ex) : ((ex - 1u) << 23u))
                sumf[r] += sc * ((am.x + am.y) + (am.z + am.w))
            }
            ib += 16u
        }
        for [unroll_full] (r in range(2)) {
            let sr = simd_sum(sumf[r])
            if (lane == 0u && first_row + uint(r) < ddim) {
                var outv = sr
                if (hasb != 0u) {
                    outv += wb[e * ddim + first_row + uint(r)]
                }
                y[y0 + first_row + uint(r)] = outv
            }
        }
    }
}

// Expert-indexed Q5_1 GEMV — per-32 planes (a 20B quant block = 5 uints: 16 nibble bytes in the
// k/k+16 split-half pairing + the qh u32; 4B scales = f16 d, m). value = d*q + m with
// q = nibble | qh_bit << 4, folded as acc += d*Σ(q·x) + m*Σx per block (f32 activations).
class MetalMoeGemvQ51 {
    @ssbo @binding = 0 wqu : array<uint>    // quant plane, uint view (block at 5*blk; qh at +4)
    @ssbo @binding = 1 wsh : array<float16> // scale plane (d at 2*blk, m at 2*blk+1)
    @ssbo @binding = 2 xf : array<float4>   // activations, float4 view (xoff always % 4)
    @ssbo @binding = 3 y : array<float>
    @uniform @binding = 4 ndim : uint
    @uniform @binding = 5 ddim : uint
    @ssbo @binding = 6 sel : array<uint>    // [streams x 2k] selection rows (idx section read)
    @uniform @binding = 7 eblk : uint       // 32-elem blocks per expert plane (ege / 32)
    @uniform @binding = 8 xs : uint         // per-slot x stride, elements (0 = shared)
    @uniform @binding = 9 ss : uint         // per-stream sel row stride, uints (2k; 0 single)
    @uniform @binding = 10 bxs : uint       // per-stream x stride, elements (0 single)
    @uniform @binding = 11 bys : uint       // per-stream y stride, elements (0 single)

    [metal_kernel(name="metal_moe_gemv_q51_msl")]
    def metal_moe_gemv_q51 {
        let lane = gl_SubgroupInvocationID
        let ix = lane / 8u    // 4-way block stride
        let it = lane % 8u    // 8 threads per block, one float4 (4 elems) each
        let nb = ndim / 32u
        let slot = gl_WorkGroupID.y
        let st = gl_WorkGroupID.z
        let eb = sel[st * ss + slot] * eblk
        let xoff = st * bxs + slot * xs
        let y0 = st * bys + slot * ddim
        let first_row = (gl_WorkGroupID.x * 2u + gl_SubgroupID) * 2u
        // this thread's 4 elems: half = it/4 selects the nibble half, ku = it%4 the quant uint
        let half = it / 4u
        let ku = it % 4u
        let nsh = half * 4u
        let hb = ku * 4u + half * 16u
        var sumf : float[2]
        var ib = ix
        while (ib < nb) {
            let xv = xf[(xoff + ib * 32u) / 4u + it]
            let sx = xv.x + xv.y + xv.z + xv.w
            for [unroll_full] (r in range(2)) {
                let blk = eb + (first_row + uint(r)) * nb + ib
                let qb = blk * 5u
                let u = wqu[qb + ku]
                let qh = wqu[qb + 4u] >> hb
                var qx = float((u >> nsh) & 15u) * xv.x
                qx += float((u >> (8u + nsh)) & 15u) * xv.y
                qx += float((u >> (16u + nsh)) & 15u) * xv.z
                qx += float((u >> (24u + nsh)) & 15u) * xv.w
                var hx = (qh & 1u) != 0u ? xv.x : 0.0
                hx += (qh & 2u) != 0u ? xv.y : 0.0
                hx += (qh & 4u) != 0u ? xv.z : 0.0
                hx += (qh & 8u) != 0u ? xv.w : 0.0
                sumf[r] += float(wsh[blk * 2u]) * (qx + 16.0 * hx) + float(wsh[blk * 2u + 1u]) * sx
            }
            ib += 4u
        }
        for [unroll_full] (r in range(2)) {
            let sr = simd_sum(sumf[r])
            if (lane == 0u && first_row + uint(r) < ddim) {
                y[y0 + first_row + uint(r)] = sr
            }
        }
    }
}

// Gathered MXFP4 mul_mm — the gathered-GEMM shape with the mx4 A stage: each thread's 16-elem
// half IS one nibble half of the 32-block (il0 = 0 low, 1 high). Bias rides MetalMoeBiasRows.
class MetalMoeMulMmMx4 {
    @ssbo @binding = 0 mxq4 : array<uint4>  // nibble plane, uint4 view (one block = one load)
    @ssbo @binding = 1 mxe : array<uint8>
    @ssbo @binding = 2 xf : array<float4>
    @ssbo @binding = 3 y : array<float>
    @uniform @binding = 4 kdim : uint
    @uniform @binding = 5 ndim : uint
    @ssbo @binding = 6 cnt : array<uint>
    @ssbo @binding = 7 basep : array<uint>
    @ssbo @binding = 8 bkt : array<uint>
    @uniform @binding = 9 eblk : uint       // 32-elem blocks per expert plane (ege / 32)
    @uniform @binding = 10 nk : uint
    @uniform @binding = 11 gather : uint
    @ssbo @binding = 13 vtab : array<float> // [16] signed doubled-e2m1 values (host-built)
    @workgroup ta : float16[2048]
    @workgroup tb : float16[1024]
    @workgroup vt : float[32]

    [metal_kernel(name="metal_moe_mulmm_mx4_msl")]
    def metal_moe_mulmm_mx4 {
        let e = gl_WorkGroupID.z
        let ce = cnt[e]
        let mBase = gl_WorkGroupID.x * 32u
        if (mBase >= (ce + 31u) / 32u * 32u) {
            return
        }
        let rbase = basep[e]
        let lid = gl_LocalInvocationID.x
        if (lid < 32u) {
            vt[lid] = vtab[lid % 16u]
        }
        barrier()
        let sg = gl_SubgroupID
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
        let eb = e * eblk + (nBase + lr0) * nkb
        var srcrow = rbase + mBase + lr1
        if (gather != 0u) {
            srcrow = bkt[rbase + min(mBase + lr1, ce - 1u)] / nk
        }
        var xcur = unsafe(addr(xf[srcrow * kdim4 + iy / 4u]))
        var kb = 0u
        while (kb < nkb) {
            let blk = eb + kb
            let b0 = unsafe(xcur[0])
            let b1 = unsafe(xcur[1])
            let sc = uint(mxe[blk])
            let d = uint_bits_to_float(sc < 2u ? (0x00200000u << sc) : ((sc - 1u) << 23u))   // exact e8m0_half, denormal e < 2 included
            let u4 = mxq4[blk]
            for [unroll_full] (w in range(4)) {
                let uw = w == 0 ? u4.x : (w == 1 ? u4.y : (w == 2 ? u4.z : u4.w))
                for [unroll_full] (c in range(4)) {
                    let nby = (uw >> (uint(c) * 8u)) & 255u
                    va[w * 4 + c] = float16(vt[il0 == 0u ? (nby & 15u) : (nby >> 4u)] * d)
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
        let row0 = rbase + mBase + (sg / 2u) * 16u
        let col0 = nBase + (sg % 2u) * 32u
        for [unroll_full] (i in range(8)) {
            simdgroup_store(mc[i], y, (row0 + uint(i / 4) * 8u) * ndim + col0 + uint(i % 4) * 8u, ndim)
        }
    }
}

// gpt-oss expert bias over the padded bucket rows (grid (cols, rows, experts)): rows past the
// expert's live count exit — ggml_add_id order (bias precedes the routing-weight reduce).
class MetalMoeBiasRows {
    @ssbo @binding = 0 y : array<float>     // [padded rows x d] bucket-ordered panel
    @ssbo @binding = 1 wb : array<float>    // [ne x d] this layer's per-expert bias rows
    @ssbo @binding = 2 cnt : array<uint>
    @ssbo @binding = 3 basep : array<uint>
    @uniform @binding = 4 d : uint

    [metal_kernel(name="metal_moe_bias_rows_msl")]
    def metal_moe_bias_rows {
        let col = gl_GlobalInvocationID.x
        if (col >= d) {
            return
        }
        let e = gl_WorkGroupID.z
        let r = gl_WorkGroupID.y
        if (r >= cnt[e]) {
            return
        }
        y[(basep[e] + r) * d + col] += wb[e * d + col]
    }
}

// Weighted top-k reduce per stream (grid.y): y[st*dim + i] = sum_j w[st][j] * dn[(st*k+j)*dim + i]
// — the CPU slot-order axpy chain as one deterministic in-kernel loop. Overwrites y (the dense
// w2 site's write shape).
[metal_dispatch(name = "enc_moe_combine", pso = "g_pso_moe_combine", tg = 256, grid = "dim/256, nst", params = "dim : int64, nst : int64")]
class MetalMoeCombine {
    @ssbo @binding = 0 @role = "read" dn : array<float>    // [streams x k x dim] per-slot down outputs
    @ssbo @binding = 1 @role = "read" selw : array<float>  // the selection buffer's float view — w of stream s at s*2k + k
    @ssbo @binding = 2 @role = "write" y : array<float>     // [streams x dim] combined out
    @uniform @binding = 3 dim : uint
    @uniform @binding = 4 nk : uint

    [metal_kernel(name="metal_moe_combine_msl")]
    def metal_moe_combine {
        let gid = gl_GlobalInvocationID.x
        if (gid >= dim) {
            return
        }
        let st = gl_WorkGroupID.y
        let w0 = st * 2u * nk + nk
        let dn0 = st * nk * dim
        var acc = 0.0
        var j = 0u
        while (j < nk) {
            acc += selw[w0 + j] * dn[dn0 + j * dim + gid]
            j++
        }
        y[st * dim + gid] = acc
    }
}

// gpt-oss clamped swiglu (stage 4): x = min(g, 7), y = clamp(u, -7, 7),
// g = (x / (1 + exp(-1.702 x))) * (y + 1) — the clamps and the +1 are trained weights' contract.
// float4 lanes, per-element math identical to the scalar form it replaced (bit-exact).
class MetalSwigluOai {
    @ssbo @binding = 0 g : array<float4>
    @ssbo @binding = 1 u : array<float4>
    @uniform @binding = 2 total : uint      // scalar element count

    [metal_kernel(name="metal_swiglu_oai_msl")]
    def metal_swiglu_oai {
        let gid = gl_GlobalInvocationID.x
        if (gid * 4u >= total) {
            return
        }
        let x = min(g[gid], float4(7.0))
        let y = min(max(u[gid], float4(-7.0)), float4(7.0))   // nolint:STYLE019 — the MSL emitter doesn't lower clamp
        g[gid] = (x / (float4(1.0) + exp(-1.702 * x))) * (y + float4(1.0))
    }
}

// gemma4 MoE router input (stage 3): y = x * rsqrt(mean(x^2) + eps) * (1/sqrt(dim)) * rscale[k],
// one tg per row. f32 accumulation (the CPU runs this sum in double for top-8 tie stability;
// no GPU doubles — rare tie flips ride the tolerance methodology, per the wave ruling).
[metal_dispatch(name = "enc_g4_router_norm", pso = "g_pso_g4rnorm", tgmem = "metal_g4_router_norm_msl_tgmem", tg = 256, grid = "nst", params = "nst : int64")]
class MetalG4RouterNorm {
    @ssbo @binding = 0 @role = "read" x : array<float>     // [rows x dim]
    @ssbo @binding = 1 @role = "weight" rs : array<float>    // [dim] per-channel router scale
    @ssbo @binding = 2 @role = "write" y : array<float>     // [rows x dim]
    @uniform @binding = 3 dim : uint
    @uniform @binding = 4 eps : float
    @uniform @binding = 5 invd : float      // 1/sqrt(dim)
    @workgroup partial : float[32]
    @workgroup inv_sh : float

    [metal_kernel(name="metal_g4_router_norm_msl")]
    def metal_g4_router_norm {
        let row = gl_WorkGroupID.x
        let lid = gl_LocalInvocationID.x
        // the norm scale is an inverse TEMPERATURE on the router softmax (selection is
        // scale-invariant; the expert WEIGHTS are not) — Kahan-compensated f32 tracks the
        // CPU's double sum to f32 rounding, and 1/sqrt avoids the fast-rsqrt ulp error
        var ss = 0.0
        var comp = 0.0
        var k = lid
        while (k < dim) {
            let vv = x[row * dim + k]
            let yv = vv * vv - comp
            let tt = ss + yv
            comp = (tt - ss) - yv
            ss = tt
            k += gl_WorkGroupSize.x
        }
        ss = simd_sum(ss)
        if (gl_SubgroupInvocationID == 0u) {
            partial[gl_SubgroupID] = ss
        }
        barrier()
        if (lid == 0u) {
            var tot = 0.0
            var tc = 0.0
            var sgi = 0u
            while (sgi < gl_NumSubgroups) {
                let yv = partial[sgi] - tc
                let tt = tot + yv
                tc = (tt - tot) - yv
                tot = tt
                sgi++
            }
            inv_sh = (1.0 / sqrt(tot / float(dim) + eps)) * invd
        }
        barrier()
        let inv = inv_sh
        k = lid
        while (k < dim) {
            y[row * dim + k] = x[row * dim + k] * inv * rs[k]
            k += gl_WorkGroupSize.x
        }
    }
}

// gemma4 per-expert down-scale fold (stage 3): selw[row][j] *= ds[seli[row][j]] — the CPU
// folds ffn_down_exps_s into the selection weights right after select.
[metal_dispatch(name = "enc_moe_wscale", pso = "g_pso_moe_wscale", tg = 32, grid = "nst", params = "nst : int64")]
class MetalMoeWScale {
    @ssbo @binding = 0 @role = "readwrite" seli : array<uint>   // selection rows, uint view (idx of row r at r*2k)
    @ssbo @binding = 1 @role = "alias" selw : array<float>  // the same buffer, float view (w of row r at r*2k + k)
    @ssbo @binding = 2 @role = "weight" ds : array<float>    // [ne] this layer's per-expert down scales
    @uniform @binding = 3 nk : uint

    [metal_kernel(name="metal_moe_wscale_msl")]
    def metal_moe_wscale {
        let row = gl_WorkGroupID.x
        let j = gl_LocalInvocationID.x
        if (j < nk) {
            selw[row * 2u * nk + nk + j] *= ds[seli[row * 2u * nk + j]]
        }
    }
}

// ===== MoE prefill CSR + gathered mul_mm (Wave C stage 2b) =====
// Atomics-free CSR: count (tg/expert) -> serial prefix (1 tg) -> bucket fill (tg/expert,
// thread-0 serial walk in row order = the CPU moe_bkt order); buckets PAD to 32-row tiles.

class MetalMoeCount {
    @ssbo @binding = 0 seli : array<uint>   // [npos x 2k] selection rows (idx section)
    @ssbo @binding = 1 cnt : array<uint>    // [ne] out
    @uniform @binding = 2 total : uint      // npos * k entries
    @uniform @binding = 3 nk : uint

    [metal_kernel(name="metal_moe_count_msl")]
    def metal_moe_count {
        let e = gl_WorkGroupID.x
        let lane = gl_SubgroupInvocationID
        var n = 0u
        var t = lane
        while (t < total) {
            if (seli[(t / nk) * 2u * nk + t % nk] == e) {
                n++
            }
            t += 32u
        }
        var m = 16u
        while (m >= 1u) {
            n += simd_shuffle_xor(n, m)
            m /= 2u
        }
        if (lane == 0u) {
            cnt[e] = n
        }
    }
}

class MetalMoePrefix {
    @ssbo @binding = 0 cnt : array<uint>
    @ssbo @binding = 1 basep : array<uint>  // [ne] exclusive prefix over the PADDED counts
    @uniform @binding = 2 ne : uint

    [metal_kernel(name="metal_moe_prefix_msl")]
    def metal_moe_prefix {
        if (gl_LocalInvocationID.x != 0u) {
            return
        }
        var b = 0u
        var e = 0u
        while (e < ne) {
            basep[e] = b
            b += (cnt[e] + 31u) / 32u * 32u
            e++
        }
    }
}

class MetalMoeBucket {
    @ssbo @binding = 0 seli : array<uint>
    @ssbo @binding = 1 basep : array<uint>
    @ssbo @binding = 2 bkt : array<uint>    // [padded rows] bucket -> entry (row*k + slot)
    @ssbo @binding = 3 inv : array<uint>    // [npos*k] entry -> padded bucket row
    @uniform @binding = 4 total : uint
    @uniform @binding = 5 nk : uint

    [metal_kernel(name="metal_moe_bucket_msl")]
    def metal_moe_bucket {
        // contiguous ascending per-lane chunks + exclusive scan of chunk counts reproduce the
        // serial entry order exactly (a 1-lane scan here was ~0.7ms/layer of load latency)
        let e = gl_WorkGroupID.x
        let lane = gl_SubgroupInvocationID
        let chunk = (total + 31u) / 32u
        let t0 = lane * chunk
        let t1 = min(t0 + chunk, total)
        var n = 0u
        var t = t0
        while (t < t1) {
            if (seli[(t / nk) * 2u * nk + t % nk] == e) {
                n++
            }
            t++
        }
        let mine = n
        var off = 1u
        while (off < 32u) {
            let v = simd_shuffle_up(n, off)
            if (lane >= off) {
                n += v
            }
            off *= 2u
        }
        var b = basep[e] + n - mine
        t = t0
        while (t < t1) {
            if (seli[(t / nk) * 2u * nk + t % nk] == e) {
                bkt[b] = t
                inv[t] = b
                b++
            }
            t++
        }
    }
}

// the ordered weighted reduce: per (position, dim slice), the k slots in slot order via inv —
// bit-stable vs the CPU park-and-accumulate; pad bucket rows are never referenced
class MetalMoeReduce {
    @ssbo @binding = 0 dn : array<float>    // [padded rows x dim] bucket-ordered down outputs
    @ssbo @binding = 1 selw : array<float>  // selection rows, float view (w of row r at r*2k + k)
    @ssbo @binding = 2 inv : array<uint>
    @ssbo @binding = 3 y : array<float>     // [npos x dim]
    @uniform @binding = 4 dim : uint
    @uniform @binding = 5 nk : uint

    [metal_kernel(name="metal_moe_reduce_msl")]
    def metal_moe_reduce {
        let gid = gl_GlobalInvocationID.x
        if (gid >= dim) {
            return
        }
        let row = gl_WorkGroupID.y
        var acc = 0.0
        var j = 0u
        while (j < nk) {
            acc += selw[row * 2u * nk + nk + j] * dn[inv[row * nk + j] * dim + gid]
            j++
        }
        y[row * dim + gid] = acc
    }
}

// Gathered q8 mul_mm — MetalQ8MulMm per expert (grid.z): weight base e*eblk, X rows gathered
// via bkt (gather=1: src row = entry/k; gather=0: contiguous bucket rows — the down site),
// C tiles land at the expert's padded bucket rows; tiles past the padded count exit.
class MetalMoeMulMmQ8 {
    @ssbo @binding = 0 wsh : array<float16> // the expert STACK's blob slice, half-scale view
    @ssbo @binding = 1 wqb : array<int8>    // the same blob buffer, byte view
    @ssbo @binding = 2 xf : array<float4>
    @ssbo @binding = 3 y : array<float>
    @uniform @binding = 4 kdim : uint
    @uniform @binding = 5 ndim : uint
    @ssbo @binding = 6 cnt : array<uint>
    @ssbo @binding = 7 basep : array<uint>
    @ssbo @binding = 8 bkt : array<uint>
    @uniform @binding = 9 eblk : uint       // 34B blocks per expert plane (ege / 32)
    @uniform @binding = 10 nk : uint
    @uniform @binding = 11 gather : uint    // 1 = bkt-indirect X (gate/up), 0 = contiguous (down)
    @workgroup ta : float16[2048]
    @workgroup tb : float16[1024]

    [metal_kernel(name="metal_moe_mulmm_q8_msl")]
    def metal_moe_mulmm_q8 {
        let e = gl_WorkGroupID.z
        let ce = cnt[e]
        let mBase = gl_WorkGroupID.x * 32u
        if (mBase >= (ce + 31u) / 32u * 32u) {
            return
        }
        let rbase = basep[e]
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
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
        let blk0 = e * eblk + (nBase + lr0) * nkb
        var srcrow = rbase + mBase + lr1
        if (gather != 0u) {
            srcrow = bkt[rbase + min(mBase + lr1, ce - 1u)] / nk
        }
        var scur = unsafe(addr(wsh[blk0 * 17u]))
        var qp = unsafe(addr(wqb[blk0 * 34u + 2u + il0 * 16u]))
        var xcur = unsafe(addr(xf[srcrow * kdim4 + iy / 4u]))
        var kb = 0u
        while (kb < nkb) {
            let sA = float(unsafe(scur[0]))
            let b0 = unsafe(xcur[0])
            let b1 = unsafe(xcur[1])
            for [unroll_full] (i in range(16)) {
                va[i] = float16(float(unsafe(qp[i])) * sA)
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
            scur = unsafe(scur + 17)
            qp = unsafe(qp + 34)
            xcur = unsafe(xcur + 8)
            kb++
        }
        let row0 = rbase + mBase + (sg / 2u) * 16u
        let col0 = nBase + (sg % 2u) * 32u
        for [unroll_full] (i in range(8)) {
            simdgroup_store(mc[i], y, (row0 + uint(i / 4) * 8u) * ndim + col0 + uint(i % 4) * 8u, ndim)
        }
    }
}

// Gathered Q4_K mul_mm — MetalKqMulMmK4's A stage with the expert superblock shift.
class MetalMoeMulMmK4 {
    @ssbo @binding = 0 ksh : array<float16>
    @ssbo @binding = 1 ks4 : array<uint4>
    @ssbo @binding = 2 kqu : array<uint>
    @ssbo @binding = 3 xf : array<float4>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 kdim : uint
    @uniform @binding = 6 ndim : uint
    @ssbo @binding = 7 cnt : array<uint>
    @ssbo @binding = 8 basep : array<uint>
    @ssbo @binding = 9 bkt : array<uint>
    @uniform @binding = 10 esb : uint       // superblocks per expert plane (ege / 256)
    @uniform @binding = 11 nk : uint
    @uniform @binding = 12 gather : uint
    @workgroup ta : float16[2048]
    @workgroup tb : float16[1024]

    [metal_kernel(name="metal_moe_mulmm_k4_msl")]
    def metal_moe_mulmm_k4 {
        let e = gl_WorkGroupID.z
        let ce = cnt[e]
        let mBase = gl_WorkGroupID.x * 32u
        if (mBase >= (ce + 31u) / 32u * 32u) {
            return
        }
        let rbase = basep[e]
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
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
        let ewsb = e * esb + (nBase + lr0) * nsb
        var srcrow = rbase + mBase + lr1
        if (gather != 0u) {
            srcrow = bkt[rbase + min(mBase + lr1, ce - 1u)] / nk
        }
        var xcur = unsafe(addr(xf[srcrow * kdim4 + iy / 4u]))
        var kb = 0u
        while (kb < nkb) {
            let sb = kb / 8u
            let js = kb % 8u
            let blk = ewsb + sb
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
        let row0 = rbase + mBase + (sg / 2u) * 16u
        let col0 = nBase + (sg % 2u) * 32u
        for [unroll_full] (i in range(8)) {
            simdgroup_store(mc[i], y, (row0 + uint(i / 4) * 8u) * ndim + col0 + uint(i % 4) * 8u, ndim)
        }
    }
}

// Gathered Q5_K mul_mm — MetalKqMulMmK5's A stage with the expert superblock shift.
class MetalMoeMulMmK5 {
    @ssbo @binding = 0 ksh : array<float16>
    @ssbo @binding = 1 ks4 : array<uint4>
    @ssbo @binding = 2 kqu : array<uint>
    @ssbo @binding = 3 xf : array<float4>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 kdim : uint
    @uniform @binding = 6 ndim : uint
    @ssbo @binding = 7 cnt : array<uint>
    @ssbo @binding = 8 basep : array<uint>
    @ssbo @binding = 9 bkt : array<uint>
    @uniform @binding = 10 esb : uint
    @uniform @binding = 11 nk : uint
    @uniform @binding = 12 gather : uint
    @workgroup ta : float16[2048]
    @workgroup tb : float16[1024]

    [metal_kernel(name="metal_moe_mulmm_k5_msl")]
    def metal_moe_mulmm_k5 {
        let e = gl_WorkGroupID.z
        let ce = cnt[e]
        let mBase = gl_WorkGroupID.x * 32u
        if (mBase >= (ce + 31u) / 32u * 32u) {
            return
        }
        let rbase = basep[e]
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
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
        let ewsb = e * esb + (nBase + lr0) * nsb
        var srcrow = rbase + mBase + lr1
        if (gather != 0u) {
            srcrow = bkt[rbase + min(mBase + lr1, ce - 1u)] / nk
        }
        var xcur = unsafe(addr(xf[srcrow * kdim4 + iy / 4u]))
        var kb = 0u
        while (kb < nkb) {
            let sb = kb / 8u
            let js = kb % 8u
            let blk = ewsb + sb
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
        let row0 = rbase + mBase + (sg / 2u) * 16u
        let col0 = nBase + (sg % 2u) * 32u
        for [unroll_full] (i in range(8)) {
            simdgroup_store(mc[i], y, (row0 + uint(i / 4) * 8u) * ndim + col0 + uint(i % 4) * 8u, ndim)
        }
    }
}

// Gathered Q5_1 mul_mm — the k5 skeleton with the per-32 A stage: each thread's 16-elem half is
// one nibble half of the 32-block (il0), value = d*q + m folded into the f16 A tile.
class MetalMoeMulMmQ51 {
    @ssbo @binding = 0 wsh : array<float16> // scale plane (d at 2*blk, m at 2*blk+1)
    @ssbo @binding = 1 wqu : array<uint>    // quant plane, uint view (block at 5*blk; qh at +4)
    @ssbo @binding = 2 xf : array<float4>
    @ssbo @binding = 3 y : array<float>
    @uniform @binding = 4 kdim : uint
    @uniform @binding = 5 ndim : uint
    @ssbo @binding = 6 cnt : array<uint>
    @ssbo @binding = 7 basep : array<uint>
    @ssbo @binding = 8 bkt : array<uint>
    @uniform @binding = 9 eblk : uint       // 32-elem blocks per expert plane (ege / 32)
    @uniform @binding = 10 nk : uint
    @uniform @binding = 11 gather : uint
    @workgroup ta : float16[2048]
    @workgroup tb : float16[1024]

    [metal_kernel(name="metal_moe_mulmm_q51_msl")]
    def metal_moe_mulmm_q51 {
        let e = gl_WorkGroupID.z
        let ce = cnt[e]
        let mBase = gl_WorkGroupID.x * 32u
        if (mBase >= (ce + 31u) / 32u * 32u) {
            return
        }
        let rbase = basep[e]
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
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
        let ewb = e * eblk + (nBase + lr0) * nkb
        let nsh = il0 * 4u
        let hb0 = il0 * 16u
        var srcrow = rbase + mBase + lr1
        if (gather != 0u) {
            srcrow = bkt[rbase + min(mBase + lr1, ce - 1u)] / nk
        }
        var xcur = unsafe(addr(xf[srcrow * kdim4 + iy / 4u]))
        var kb = 0u
        while (kb < nkb) {
            let blk = ewb + kb
            let b0 = unsafe(xcur[0])
            let b1 = unsafe(xcur[1])
            let d = float(wsh[blk * 2u])
            let m = float(wsh[blk * 2u + 1u])
            let qb = blk * 5u
            let qh = wqu[qb + 4u]
            for [unroll_full] (k in range(4)) {
                let u = wqu[qb + uint(k)]
                for [unroll_full] (c in range(4)) {
                    let el = uint(k) * 4u + uint(c)
                    let q = ((u >> (8u * uint(c) + nsh)) & 15u) | (((qh >> (el + hb0)) & 1u) << 4u)
                    va[k * 4 + c] = float16(d * float(q) + m)
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
        let row0 = rbase + mBase + (sg / 2u) * 16u
        let col0 = nBase + (sg % 2u) * 32u
        for [unroll_full] (i in range(8)) {
            simdgroup_store(mc[i], y, (row0 + uint(i / 4) * 8u) * ndim + col0 + uint(i % 4) * 8u, ndim)
        }
    }
}

// Gathered Q6_K mul_mm — MetalKqMulMmK6's A stage with the expert superblock shift.
class MetalMoeMulMmK6 {
    @ssbo @binding = 0 kdh : array<float16>
    @ssbo @binding = 1 ksc : array<uint4>
    @ssbo @binding = 2 kqu : array<uint>
    @ssbo @binding = 3 xf : array<float4>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 kdim : uint
    @uniform @binding = 6 ndim : uint
    @ssbo @binding = 7 cnt : array<uint>
    @ssbo @binding = 8 basep : array<uint>
    @ssbo @binding = 9 bkt : array<uint>
    @uniform @binding = 10 esb : uint
    @uniform @binding = 11 nk : uint
    @uniform @binding = 12 gather : uint
    @workgroup ta : float16[2048]
    @workgroup tb : float16[1024]

    [metal_kernel(name="metal_moe_mulmm_k6_msl")]
    def metal_moe_mulmm_k6 {
        let e = gl_WorkGroupID.z
        let ce = cnt[e]
        let mBase = gl_WorkGroupID.x * 32u
        if (mBase >= (ce + 31u) / 32u * 32u) {
            return
        }
        let rbase = basep[e]
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
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
        let ewsb = e * esb + (nBase + lr0) * nsb
        var srcrow = rbase + mBase + lr1
        if (gather != 0u) {
            srcrow = bkt[rbase + min(mBase + lr1, ce - 1u)] / nk
        }
        var xcur = unsafe(addr(xf[srcrow * kdim4 + iy / 4u]))
        var sv : uint4
        var dall = 0.0
        var kb = 0u
        while (kb < nkb) {
            let sb = kb / 8u
            let js = kb % 8u
            let blk = ewsb + sb
            let b0 = unsafe(xcur[0])
            let b1 = unsafe(xcur[1])
            let half6 = js / 4u
            let gg = js % 4u
            if (js == 0u) {   // superblock scalars change every 8th kb; uniform branch, no divergence
                sv = ksc[blk]
                dall = float(kdh[blk])
            }
            let si = half6 * 8u + gg * 2u + il0
            let scw = si < 8u ? (si < 4u ? sv.x : sv.y) : (si < 12u ? sv.z : sv.w)
            let s6 = float(((int(scw >> ((si & 3u) * 8u)) & 255) ^ 128) - 128)
            let dsc = dall * s6
            let dmn = dsc * 32.0
            // shift folded into exact power-of-two pre-scales — see MetalKqMulMmK6
            let dsc1 = dsc * 0.00390625
            let dsc2 = dsc * 0.0000152587890625
            let dsc3 = dsc * 0.000000059604644775390625
            let qb = blk * 48u + half6 * 16u + (gg & 1u) * 8u + il0 * 4u
            let hb = blk * 48u + 32u + half6 * 8u + il0 * 4u
            let nsh = (gg / 2u) * 4u
            let hsh = gg * 2u
            for [unroll_full] (k in range(4)) {
                let u = kqu[qb + uint(k)]
                let hu = kqu[hb + uint(k)]
                let qv = ((u >> nsh) & 0x0F0F0F0F) | (((hu >> hsh) & 0x03030303) << 4u)
                va[k * 4] = float16(dsc * float(qv & 255u) - dmn)
                va[k * 4 + 1] = float16(dsc1 * float(qv & 65280u) - dmn)
                va[k * 4 + 2] = float16(dsc2 * float(qv & 16711680u) - dmn)
                va[k * 4 + 3] = float16(dsc3 * float(qv & 4278190080u) - dmn)
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
        let row0 = rbase + mBase + (sg / 2u) * 16u
        let col0 = nBase + (sg % 2u) * 32u
        for [unroll_full] (i in range(8)) {
            simdgroup_store(mc[i], y, (row0 + uint(i / 4) * 8u) * ndim + col0 + uint(i % 4) * 8u, ndim)
        }
    }
}

// One simdgroup per output row over the 34B q8 blob (f16 scale + 32 int8 inline — the SAME
// dual-view W the mul_mm reads, bound at the tensor's byte offset): lane owns chunk lane%8 of
// every 4th block, 4 in flight; the scale rides the quants' cacheline. Width-agnostic — rows/tg via DASLLAMA_METAL_GEMV_TG (g_gemv_rows).
[metal_dispatch(name = "enc_gemv_c", pso = "g_pso_gemv", tg = "g_gemv_rows*32", grid = "d/g_gemv_rows", params = "d : int64")]
class MetalQ8Gemv {
    @ssbo @binding = 0 @role = "weight" @off = "boff" wsh : array<float16> // 34B-block W blob, half-scale view (block b at 17*b)
    @ssbo @binding = 1 @role = "weight" @off = "boff" wqb : array<int8>    // the same blob buffer, byte view (quants at 34*b + 2)
    @ssbo @binding = 2 @role = "read" x : array<float4>    // [n] f32 vector, float4 view
    @ssbo @binding = 3 @role = "write" @off = "yoff" @span = "d*4" y : array<float>     // [d] outputs
    @uniform @binding = 4 ndim : uint       // n (reduction dim, % 32)
    @uniform @binding = 5 ddim : uint       // d (output rows)

    [metal_kernel(name="metal_q8_gemv_msl")]
    def metal_q8_gemv {
        let row = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        if (row >= ddim) {
            return
        }
        let lane = gl_SubgroupInvocationID
        let nb = ndim / 32u
        let nb4 = ndim / 4u
        var sp = unsafe(addr(wsh[(row * nb + lane / 8u) * 17u]))
        var qp = unsafe(addr(wqb[(row * nb + lane / 8u) * 34u + 2u + (lane % 8u) * 4u]))
        var x0 = unsafe(addr(x[lane]))
        var acc = 0.0
        var lw : float4[4]
        var k = lane
        while (k + 96u < nb4) {
            // all 4 chunk dequants batch AHEAD of the fma chains (the MvB2 load batching)
            for [unroll_full] (c in range(4)) {
                let d = float(unsafe(sp[uint(c) * 68u]))   // chunk c sits 4 blocks (68 halves) on
                let qb = uint(c) * 136u                    // ... and 136 quant bytes
                lw[c] = float4(float(unsafe(qp[qb])), float(unsafe(qp[qb + 1u])),
                    float(unsafe(qp[qb + 2u])), float(unsafe(qp[qb + 3u]))) * d
            }
            for [unroll_full] (c in range(4)) {
                let xv = unsafe(x0[uint(c) * 32u])
                acc += lw[c].x * xv.x + lw[c].y * xv.y + lw[c].z * xv.z + lw[c].w * xv.w
            }
            sp = unsafe(sp + 272)           // 16 blocks x 17 halves per stripe
            qp = unsafe(qp + 544)           // 16 blocks x 34 bytes
            x0 = unsafe(x0 + 128)
            k += 128u
        }
        while (k < nb4) {
            let bi = row * nb + k / 8u
            let d = float(wsh[bi * 17u])
            let qb = bi * 34u + 2u + (k % 8u) * 4u
            let xv = x[k]
            acc += (float(wqb[qb]) * xv.x + float(wqb[qb + 1u]) * xv.y +
                float(wqb[qb + 2u]) * xv.z + float(wqb[qb + 3u]) * xv.w) * d
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

// Q5_K, select-form nr0=1 twin (the gemv-lab b1c winner): one row per simdgroup, lcpp's q5 lane
// map, qh via mask-selects. Wins at n < 3072 at EVERY d (-9..-11% vs b2r) and the small-d kv
// shape (b2r under-occupied); b2r keeps n >= 3072. enc_kq_gemv picks per site.
class MetalKqGemvK5C {
    @ssbo @binding = 0 ksh : array<float16> // 16B scale blocks, half view (d at 8*blk, dmin at 8*blk+1)
    @ssbo @binding = 1 ks4 : array<uint4>   // the same buffer, uint4 view (whole block at blk)
    @ssbo @binding = 2 kqb : array<uint8>   // k5 quant plane, byte view (qs at 160*blk, qh at +128)
    @ssbo @binding = 3 x : array<float>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint

    [metal_kernel(name="metal_kq_gemv_k5c_msl")]
    def metal_kq_gemv_k5c {
        let lane = gl_SubgroupInvocationID
        let tid = lane / 4u
        let ix = lane % 4u
        let iq = tid / 4u
        let ir = tid % 4u
        let l0 = ir * 8u
        let hm1 = 1u << (2u * iq)
        let hm2 = hm1 << 1u
        let hm3 = hm1 << 4u
        let hm4 = hm2 << 4u
        let nb = ndim / 256u
        let first_row = gl_WorkGroupID.x * 2u + gl_SubgroupID
        var yl : float[16]
        var yh : float[16]
        var sumf = 0.0
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
            let blk = first_row * nb + ib
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
            var acc1 : float[4]
            var acc2 : float[4]
            for [unroll_full] (l in range(8)) {
                let q1 = uint(kqb[qs0 + uint(l)])
                let q2 = uint(kqb[qs0 + 64u + uint(l)])
                let h = uint(kqb[qh0 + uint(l)])
                acc1[0] += yl[l] * float(q1 & 0x0F)
                acc1[1] += yl[l + 8] * float(q1 & 0xF0)
                acc1[2] += yh[l] * float(q2 & 0x0F)
                acc1[3] += yh[l + 8] * float(q2 & 0xF0)
                acc2[0] += (h & hm1) != 0u ? yl[l] : 0.0
                acc2[1] += (h & hm2) != 0u ? yl[l + 8] : 0.0
                acc2[2] += (h & hm3) != 0u ? yh[l] : 0.0
                acc2[3] += (h & hm4) != 0u ? yh[l + 8] : 0.0
            }
            let dd = float(ksh[blk * 8u])
            let dm = float(ksh[blk * 8u + 1u])
            sumf += dd * (float(sc0 & 255u) * (acc1[0] + 16.0 * acc2[0])
                + float(sc0 >> 8u) * (acc1[1] * 0.0625 + 16.0 * acc2[1])
                + float(sc2 & 255u) * (acc1[2] + 16.0 * acc2[2])
                + float(sc2 >> 8u) * (acc1[3] * 0.0625 + 16.0 * acc2[3]))
            sumf -= dm * (sumy[0] * float(sc1 & 255u) + sumy[1] * float(sc1 >> 8u)
                + sumy[2] * float(sc3 & 255u) + sumy[3] * float(sc3 >> 8u))
            ib += 4u
        }
        let s = simd_sum(sumf)
        if (lane == 0u && first_row < ddim) {
            y[first_row] = s
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

// ===== K-quant B=5..8 single-pass twins (lab extx8 form): ONE weight pass vs two B4 groups; x panel
// STAGED through tgmem per superblock (0.45 -> 0.317 ms; the SLC-resident lab can't rank this — in-graph
// A/B decides, DASLLAMA_METAL_KQ_B8=0 falls back). REQUIRES mp == 8 (nlive >= 5): stage reads all 8 rows, stores gate on nr.

class MetalKqMvB8K4 {
    @ssbo @binding = 0 ksh : array<float16> // 16B scale blocks, half view (d at 8*blk, dmin at +1)
    @ssbo @binding = 1 ks4 : array<uint4>   // the same buffer, uint4 view
    @ssbo @binding = 2 kqu : array<uint>    // k4 quant plane, uint view (qs of blk at 32*blk)
    @ssbo @binding = 3 x : array<float4>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint
    @uniform @binding = 7 ys : uint         // y row stride, elements
    @uniform @binding = 8 nr : uint         // live columns — stores gate on it
    @workgroup txp : float4[512]            // [8 streams x 64 float4] one superblock's X panel

    [metal_kernel(name="metal_kq_mvb8_k4_msl")]
    def metal_kq_mvb8_k4 {
        let lid = gl_LocalInvocationID.x
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
        var acc : float4[8]
        var sb = 0u
        while (sb < nb) {
            barrier()
            for [unroll_full] (j in range(8)) {
                let slot = uint(j) * 64u + lid
                txp[slot] = x[(slot / 64u) * nb4 + sb * 64u + (slot % 64u)]
            }
            barrier()
            let blk = row * nb + sb
            let sv = ks4[blk]
            let dd = float(ksh[blk * 8u])
            let dm = float(ksh[blk * 8u + 1u])
            let qb = blk * 32u + q4i * 8u + lo
            var qv : uint[8]
            for [unroll_full] (u in range(4)) {
                qv[u] = kqu[qb + uint(u)]
                qv[4 + u] = kqu[qb + 16u + uint(u)]
            }
            for [unroll_full] (ch in range(2)) {
                let sy = (sv.y >> (8u * jl)) & 255u
                let szv = (sv.z >> (8u * jl)) & 255u
                let sw = (sv.w >> (8u * jl)) & 255u
                let sc = ch == 0 ? (sy & 63u) : ((sw & 0x0Fu) | ((sy >> 6u) << 4u))
                let mn = ch == 0 ? (szv & 63u) : ((sw >> 4u) | ((szv >> 6u) << 4u))
                let dl = dd * float(sc)
                let ml = dm * float(mn)
                var w4 : float4[4]
                for [unroll_full] (u in range(4)) {
                    let qvv = qv[4 * ch + u]
                    w4[u] = float4(float((qvv >> nsh) & 15u), float((qvv >> (8u + nsh)) & 15u),
                        float((qvv >> (16u + nsh)) & 15u), float((qvv >> (24u + nsh)) & 15u)) * dl - float4(ml)
                }
                let cxl = (q4i + uint(ch) * 2u) * 16u + (nib ? 0u : 8u) + lo
                for [unroll_full] (b in range(8)) {
                    for [unroll_full] (u in range(4)) {
                        acc[b] += txp[uint(b) * 64u + cxl + uint(u)] * w4[u]
                    }
                }
            }
            sb++
        }
        for [unroll_full] (b in range(8)) {
            var s = acc[b].x + acc[b].y + acc[b].z + acc[b].w
            s += simd_shuffle_down(s, 4u)
            s += simd_shuffle_down(s, 2u)
            s += simd_shuffle_down(s, 1u)
            if (tx == 0u && row < ddim && uint(b) < nr) {
                y[uint(b) * ys + row] = s
            }
        }
    }
}

class MetalKqMvB8K5 {
    @ssbo @binding = 0 ksh : array<float16>
    @ssbo @binding = 1 ks4 : array<uint4>
    @ssbo @binding = 2 kqu : array<uint>    // k5 plane, uint view (qs at 40*blk, qh at 40*blk+32)
    @ssbo @binding = 3 x : array<float4>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint
    @uniform @binding = 7 ys : uint
    @uniform @binding = 8 nr : uint
    @workgroup txp : float4[512]

    [metal_kernel(name="metal_kq_mvb8_k5_msl")]
    def metal_kq_mvb8_k5 {
        let lid = gl_LocalInvocationID.x
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
        var acc : float4[8]
        var sb = 0u
        while (sb < nb) {
            barrier()
            for [unroll_full] (j in range(8)) {
                let slot = uint(j) * 64u + lid
                txp[slot] = x[(slot / 64u) * nb4 + sb * 64u + (slot % 64u)]
            }
            barrier()
            let blk = row * nb + sb
            let sv = ks4[blk]
            let dd = float(ksh[blk * 8u])
            let dm = float(ksh[blk * 8u + 1u])
            let qb = blk * 40u + q4i * 8u + lo
            var qv : uint[8]
            var hq : uint[4]
            for [unroll_full] (u in range(4)) {
                qv[u] = kqu[qb + uint(u)]
                qv[4 + u] = kqu[qb + 16u + uint(u)]
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
                let hsh = jl + 4u * uint(ch)
                var w4 : float4[4]
                for [unroll_full] (u in range(4)) {
                    let qvv = qv[4 * ch + u]
                    let hv = hq[u] >> hsh
                    w4[u] = float4(float(((qvv >> nsh) & 15u) | ((hv & 1u) << 4u)),
                        float(((qvv >> (8u + nsh)) & 15u) | (((hv >> 8u) & 1u) << 4u)),
                        float(((qvv >> (16u + nsh)) & 15u) | (((hv >> 16u) & 1u) << 4u)),
                        float(((qvv >> (24u + nsh)) & 15u) | (((hv >> 24u) & 1u) << 4u))) * dl - float4(ml)
                }
                let cxl = (q4i + uint(ch) * 2u) * 16u + (nib ? 0u : 8u) + lo
                for [unroll_full] (b in range(8)) {
                    for [unroll_full] (u in range(4)) {
                        acc[b] += txp[uint(b) * 64u + cxl + uint(u)] * w4[u]
                    }
                }
            }
            sb++
        }
        for [unroll_full] (b in range(8)) {
            var s = acc[b].x + acc[b].y + acc[b].z + acc[b].w
            s += simd_shuffle_down(s, 4u)
            s += simd_shuffle_down(s, 2u)
            s += simd_shuffle_down(s, 1u)
            if (tx == 0u && row < ddim && uint(b) < nr) {
                y[uint(b) * ys + row] = s
            }
        }
    }
}

class MetalKqMvB8K6 {
    @ssbo @binding = 0 kdh : array<float16> // the d plane — the scale buffer bound at byte nsb*16
    @ssbo @binding = 1 kscb : array<uint8>  // 16B sub-scale blocks, byte view (same buffer at 0)
    @ssbo @binding = 2 kqu : array<uint>    // k6 quant plane, uint view (ql at 48*blk, qh at 48*blk+32)
    @ssbo @binding = 3 x : array<float4>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ndim : uint
    @uniform @binding = 6 ddim : uint
    @uniform @binding = 7 ys : uint
    @uniform @binding = 8 nr : uint
    @workgroup txp : float4[512]

    [metal_kernel(name="metal_kq_mvb8_k6_msl")]
    def metal_kq_mvb8_k6 {
        let lid = gl_LocalInvocationID.x
        let lane = gl_SubgroupInvocationID
        let tx = lane % 8u
        let ty = lane / 8u
        let row = gl_WorkGroupID.x * 8u + gl_SubgroupID * 4u + ty
        let nb = ndim / 256u
        let s = tx / 2u
        let o4 = (tx % 2u) * 4u
        let qoff = (s % 2u) * 8u + o4
        let shr = s < 2u ? 0u : 4u
        let hsh = 2u * s
        let nb4 = ndim / 4u
        var acc : float4[8]
        var sb = 0u
        while (sb < nb) {
            barrier()
            for [unroll_full] (j in range(8)) {
                let slot = uint(j) * 64u + lid
                txp[slot] = x[(slot / 64u) * nb4 + sb * 64u + (slot % 64u)]
            }
            barrier()
            let blk = row * nb + sb
            let dd = float(kdh[blk])
            for [unroll_full] (ch in range(2)) {
                let ip = uint(ch)
                let qb = blk * 48u + ip * 16u + qoff
                let hb = blk * 48u + 32u + ip * 8u + o4
                var qu : uint[4]
                var hu : uint[4]
                for [unroll_full] (u in range(4)) {
                    qu[u] = kqu[qb + uint(u)]
                    hu[u] = kqu[hb + uint(u)]
                }
                let g = ip * 8u + s * 2u + (tx % 2u)
                let sc = float((int(kscb[blk * 16u + g]) ^ 128) - 128)
                let dl = dd * sc
                let ml = dl * 32.0
                var w4 : float4[4]
                for [unroll_full] (u in range(4)) {
                    let qvc = ((qu[u] >> shr) & 0x0F0F0F0F) | (((hu[u] >> hsh) & 0x03030303) << 4u)
                    w4[u] = float4(float(qvc & 255u), float((qvc >> 8u) & 255u),
                        float((qvc >> 16u) & 255u), float((qvc >> 24u) & 255u)) * dl - float4(ml)
                }
                let cxl = ip * 32u + s * 8u + o4
                for [unroll_full] (b in range(8)) {
                    for [unroll_full] (u in range(4)) {
                        acc[b] += txp[uint(b) * 64u + cxl + uint(u)] * w4[u]
                    }
                }
            }
            sb++
        }
        for [unroll_full] (b in range(8)) {
            var sm = acc[b].x + acc[b].y + acc[b].z + acc[b].w
            sm += simd_shuffle_down(sm, 4u)
            sm += simd_shuffle_down(sm, 2u)
            sm += simd_shuffle_down(sm, 1u)
            if (tx == 0u && row < ddim && uint(b) < nr) {
                y[uint(b) * ys + row] = sm
            }
        }
    }
}

// One thread per rotation pair: NORM (2j, 2j+1 within each head) or NEOX (j, j+half) per the
// mode uniform, matching rope_scaled_tab / rope_scaled_neox_tab exactly via the position's
// precomputed table row. One kernel serves q (n = qd) and k (n = kv_dim) as two dispatches.
class MetalRope {
    @ssbo @binding = 0 v : array<float>     // [ntok x n], roped in place
    @ssbo @binding = 1 tcos : array<float>  // [ntok x head_size/2]
    @ssbo @binding = 2 tsin : array<float>
    @uniform @binding = 3 n : uint          // row width (qd or kv_dim), % head_size
    @uniform @binding = 4 head_size : uint
    @uniform @binding = 5 npairs : uint     // ntok * n/2 — grid guard
    @uniform @binding = 6 neox : uint       // pair (j, j+half) instead of (2j, 2j+1); same table row j
    @uniform @binding = 7 rot : uint        // partial rotary width; == head_size for full (table rows rot/2 wide)

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
        let halfr = rot / 2u
        let h = rem / half
        let j = rem % half
        // threads j < rot/2 rotate the (j, j+rot/2) pair; the rest identity-cover [rot, hs)
        let rotp = j < halfr
        let hb = p * n + h * head_size
        let i = neox != 0u ? (rotp ? hb + j : hb + rot + (j - halfr)) : hb + 2u * j
        let i2 = neox != 0u ? (rotp ? i + halfr : i + (half - halfr)) : i + 1u
        let fcr = rotp ? tcos[p * halfr + j] : 1.0
        let fci = rotp ? tsin[p * halfr + j] : 0.0
        let v0 = v[i]
        let v1 = v[i2]
        v[i] = v0 * fcr - v1 * fci
        v[i2] = v0 * fci + v1 * fcr
    }
}

// Additive per-row bias (x[r*n + i] += b[i]) — the QKV bias epilogue for the prefill panels
// (decode folds it into rope-store; prefill is GEMM-bound, so a plain elementwise pass is free).
class MetalAddBiasRows {
    @ssbo @binding = 0 x : array<float>     // [nrows x n], updated in place (bound at the panel offset)
    @ssbo @binding = 1 b : array<float>     // [n]
    @uniform @binding = 2 n : uint
    @uniform @binding = 3 total : uint      // nrows * n — grid guard

    [metal_kernel(name="metal_add_bias_rows_msl")]
    def metal_add_bias_rows {
        let gid = gl_GlobalInvocationID.x
        if (gid >= total) {
            return
        }
        x[gid] += b[gid % n]
    }
}

// Per-head RMSNorm on Q/K before rope (qwen3-line QK-norm): one threadgroup = hs threads per
// (head, row), nq q-head groups then the k heads, weight w[(k ? woffe : 0) + j]. Decode covers
// q+k in ONE grid over the fused QKV buffer; prefill dispatches separate q/k panels per-plane.
[metal_dispatch(name = "enc_qk_norm", pso = "g_pso_qknorm", tgmem = "metal_qk_norm_msl_tgmem", tg = "head_size", grid = "ntg, nrows", params = "ntg : int64, nrows : int64, head_size : int64")]
class MetalQkNorm {
    @ssbo @binding = 0 @role = "readwrite" x : array<float>     // bound at the panel offset; normalized in place
    @ssbo @binding = 1 @role = "weight" w : array<float>
    @uniform @binding = 2 nq : uint         // q-head threadgroups before the k heads
    @uniform @binding = 3 koffe : uint      // k region's element offset within a row (the WRITE side)
    @uniform @binding = 4 rstride : uint    // row stride (qkvd / qd / kv_dim)
    @uniform @binding = 5 hs : uint         // head size == the threadgroup width
    @uniform @binding = 6 woffe : uint      // k plane's element offset in w
    @uniform @binding = 7 eps : float
    @uniform @binding = 8 soffe : uint      // READ offset — == koffe for in-place; the no-wv V
                                            // fusion reads the pre-norm K slice and writes the v
                                            // segment (the copy dispatch dissolves into this kernel)
    @workgroup part : float[16]     // one per simdgroup — hs <= 512 (gemma4 global heads)

    [metal_kernel(name="metal_qk_norm_msl")]
    def metal_qk_norm {
        let tgid = gl_WorkGroupID.x
        let lid = gl_LocalInvocationID.x
        let isK = tgid >= nq
        let h = isK ? tgid - nq : tgid
        let rbase = gl_WorkGroupID.y * rstride + (isK ? soffe : 0u) + h * hs
        let wbase = gl_WorkGroupID.y * rstride + (isK ? koffe : 0u) + h * hs
        let v = x[rbase + lid]
        let s1 = simd_sum(v * v)
        if (gl_SubgroupInvocationID == 0u) {
            part[gl_SubgroupID] = s1
        }
        barrier()
        var total = 0.0
        var sg = 0u
        while (sg < gl_NumSubgroups) {
            total += part[sg]
            sg++
        }
        let inv = 1.0 / sqrt(total / float(hs) + eps)
        x[wbase + lid] = w[(isK ? woffe : 0u) + lid] * (v * inv)
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
    @uniform @binding = 12 window : uint    // > 0 = sliding span (this layer); 0 = global
    @workgroup ta : float[2048]             // Q block: 32 qi-rows x 64 (row stride 64)
    @workgroup tb : float[2048]             // KT block: 64 k-rows x 32 j-cols (row stride 32)

    [metal_kernel(name="metal_attn_qk_msl")]
    def metal_attn_qk {
        let qi0 = gl_WorkGroupID.x * 32u
        let j0 = gl_WorkGroupID.y * 32u
        let h = gl_WorkGroupID.z
        // skip blocks fully above the causal diagonal, or fully below every row's sliding window
        // (the first row's wstart is the block minimum; softmax zeroes P over the skipped region)
        if (j0 > qoff + qi0 + 31u ||
                (window > 0u && qoff + qi0 + 1u > window && j0 + 32u <= qoff + qi0 + 1u - window)) {
            return
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
    @uniform @binding = 4 window : uint     // > 0 = sliding span: keys < cnt - window are masked
    @uniform @binding = 5 softcap : float   // > 0 = attention-logit soft cap (gemma2)
    @ssbo @binding = 20 sink : array<float> // [nheads] this layer's attention sinks (hass models)
    @uniform @binding = 21 hass : uint
    @workgroup red : float[4]               // one per simdgroup
    @workgroup bcast : float

    [metal_kernel(name="metal_attn_softmax_msl")]
    def metal_attn_softmax {
        let qi = gl_WorkGroupID.x
        let h = gl_WorkGroupID.y
        let lid = gl_LocalInvocationID.x
        let nthreads = gl_WorkGroupSize.x
        let cnt = qoff + qi + 1u
        let jlo = window > 0u && cnt > window ? cnt - window : 0u
        let rowbase = (h * qrows + qi) * np32
        var m = -1.0e30
        var j = jlo + lid
        while (j < cnt) {
            let sr = att[rowbase + j]
            m = max(m, softcap > 0.0 ? softcap * (1.0 - 2.0 / (1.0 + exp(sr * 2.0 / softcap))) : sr)
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
            if (hass != 0u) {
                mm = max(mm, sink[h])
            }
            bcast = mm
        }
        barrier()
        let rowmax = bcast
        var sum = 0.0
        j = jlo + lid
        while (j < cnt) {
            let sr = att[rowbase + j]
            let ev = exp((softcap > 0.0 ? softcap * (1.0 - 2.0 / (1.0 + exp(sr * 2.0 / softcap))) : sr) - rowmax)
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
            if (hass != 0u) {
                tot += exp(max(sink[h] - rowmax, -80.0))
            }
            bcast = 1.0 / tot
        }
        barrier()
        let inv = bcast
        j = jlo + lid
        while (j < cnt) {
            att[rowbase + j] *= inv
            j += nthreads
        }
        j = lid
        while (j < jlo) {
            att[rowbase + j] = 0.0          // below-window prefix: P zeroes (QK may have skipped it)
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
    @uniform @binding = 12 window : uint    // > 0 = sliding span (this layer); 0 = global
    @workgroup ta : float16[2048]           // K: k-tiles x 8 key-tiles of 8x8, tile (k row, key col)
    @workgroup tb : float16[1024]           // Q: k-tiles x 4 query-tiles of 8x8 (query row, k col)

    [metal_kernel(name="metal_attn_qk_mm_msl")]
    def metal_attn_qk_mm {
        let mBase = gl_WorkGroupID.x * 32u  // query rows
        let nBase = gl_WorkGroupID.y * 64u  // key rows
        // skip blocks fully above the causal diagonal, or fully below every row's sliding window
        if (nBase > qoff + mBase + 31u ||
                (window > 0u && qoff + mBase + 1u > window && nBase + 64u <= qoff + mBase + 1u - window)) {
            return
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

// GeGLU twin (gemma line): g = gelu(g) * u with the CPU's tanh-approx gelu (dasllama_math gelu —
// 0.5*v*(1+tanh(sqrt(2/pi)*(v + 0.044715*v^3)))); GPU tanh vs the CPU exp identity differ inside
// the shared tolerance envelope, same as swiglu's exp.
class MetalGeglu {
    @ssbo @binding = 0 g : array<float>     // [ntok x hidden] W1 gate, in place
    @ssbo @binding = 1 u : array<float>     // [ntok x hidden] W3 up
    @uniform @binding = 2 total : uint      // ntok * hidden — grid guard

    [metal_kernel(name="metal_geglu_msl")]
    def metal_geglu {
        let gid = gl_GlobalInvocationID.x
        if (gid >= total) {
            return
        }
        let vv = g[gid]
        // tanh via the exp identity (the CPU geglu4's form) — Metal tanh NaNs past ~44,
        // while exp saturates: exp(INF) -> denom INF -> the term is 0
        let a = 0.7978845608028654 * (vv + 0.044715 * vv * vv * vv)
        g[gid] = 0.5 * vv * (2.0 - 2.0 / (1.0 + exp(2.0 * a))) * u[gid]
    }
}

// Residual add, one thread per element: x += y (add_inplace).
class MetalAdd {
    @ssbo @binding = 0 x : array<float>
    @ssbo @binding = 1 y : array<float>
    @uniform @binding = 2 total : uint      // grid guard
    @uniform @binding = 3 ascale : float    // (x + y) * ascale — gemma4 layer_out_scale scales
                                            // the WHOLE layer output; every other site binds 1.0

    [metal_kernel(name="metal_add_msl")]
    def metal_add {
        let gid = gl_GlobalInvocationID.x
        if (gid >= total) {
            return
        }
        x[gid] = (x[gid] + y[gid]) * ascale
    }
}

// Flat element copy (dst/src bound at byte offsets) — the gemma4 V-from-K panel copy: no-wv
// layers reuse the pre-norm/pre-rope K projection as V (mm_qkv's copy half; the ones-plane
// norm follows when v_norm is on — bit-identical to the CPU's fused copy+norm).
class MetalPfCopy {
    @ssbo @binding = 0 y : array<float>
    @ssbo @binding = 1 x : array<float>
    @uniform @binding = 2 total : uint      // grid guard

    [metal_kernel(name="metal_pf_copy_msl")]
    def metal_pf_copy {
        let gid = gl_GlobalInvocationID.x
        if (gid >= total) {
            return
        }
        y[gid] = x[gid]
    }
}

// Interleave two [npos x d] panels into the [npos x 2d] MTP eh_proj input — the draft-warm
// cat rows [enorm(embed) ; hnorm(hidden)]; one thread per source element.
class MetalPfCat2 {
    @ssbo @binding = 0 y : array<float>
    @ssbo @binding = 1 a : array<float>
    @ssbo @binding = 2 b : array<float>
    @uniform @binding = 3 d : uint
    @uniform @binding = 4 total : uint      // grid guard: npos * d

    [metal_kernel(name="metal_pf_cat2_msl")]
    def metal_pf_cat2 {
        let gid = gl_GlobalInvocationID.x
        if (gid >= total) {
            return
        }
        let p = gid / d
        let i = gid % d
        y[p * 2u * d + i] = a[gid]
        y[p * 2u * d + d + i] = b[gid]
    }
}

// Suppress-token pinning (gemma4): logits[ids[i]] = -1e30 after the soft cap — the CPU
// forward's pin value exactly; one thread per suppressed id.
[metal_dispatch(name = "enc_suppress_row", pso = "g_pso_suppress", tg = 64, grid = "n/64", params = "n : int64")]
class MetalSuppressRow {
    @ssbo @binding = 0 @role = "readwrite" @off = "xoff" x : array<float>
    @ssbo @binding = 1 @role = "weight" ids : array<uint>
    @uniform @binding = 2 n : uint

    [metal_kernel(name="metal_suppress_row_msl")]
    def metal_suppress_row {
        let gid = gl_GlobalInvocationID.x
        if (gid >= n) {
            return
        }
        x[ids[gid]] = -1.0e30
    }
}

// Logit soft-cap, one thread per element: x = cap * tanh(x / cap) — softcap4's scalar reference
// (gemma2/gemma4 final_logit_softcap on the GPU classifier row).
[metal_dispatch(name = "enc_softcap_row", pso = "g_pso_softcap", tg = 64, grid = "total/64", params = "total : int64")]
class MetalSoftcapRow {
    @ssbo @binding = 0 @role = "readwrite" @off = "xoff" @span = "total*4" x : array<float>
    @uniform @binding = 1 total : uint      // grid guard
    @uniform @binding = 2 cap : float

    [metal_kernel(name="metal_softcap_row_msl")]
    def metal_softcap_row {
        let gid = gl_GlobalInvocationID.x
        if (gid >= total) {
            return
        }
        // tanh via the exp identity (the CPU softcap4's form; Metal tanh NaNs past ~44)
        x[gid] = cap * (1.0 - 2.0 / (1.0 + exp(x[gid] * 2.0 / cap)))
    }
}

// ===== DeltaNet (Wave D) — the qwen35 recurrent-layer chain =====

// Causal depthwise conv over the fused [q;k;v] rows + SiLU (dn_conv_step's batch form): one
// thread per (channel, position); positions reaching before the window read the channel-major
// history taps (the CPU dn_conv_state layout). Never writes history — MetalDnConvHist advances.
[metal_dispatch(name = "enc_dn_conv", pso = "g_pso_dn_conv", tg = 256, grid = "cd/256, npos", params = "cd : int64, npos : int64")]
class MetalDnConv {
    @ssbo @binding = 0 @role = "read" x : array<float>     // [npos x cd] fused qkv projection rows
    @ssbo @binding = 1 @role = "read" @off = "hoff" hist : array<float>  // [cd x taps] channel-major history (layer slice)
    @ssbo @binding = 2 @role = "weight" w : array<float>     // [cd x dconv] conv taps (raw fblob layout)
    @ssbo @binding = 3 @role = "write" y : array<float>     // [npos x cd] conv output, silu'd
    @uniform @binding = 4 cd : uint
    @uniform @binding = 5 dconv : uint
    @uniform @binding = 6 npos : uint

    [metal_kernel(name="metal_dn_conv_msl")]
    def metal_dn_conv {
        let ch = gl_GlobalInvocationID.x
        let p = gl_GlobalInvocationID.y
        if (ch >= cd) {
            return
        }
        let taps = dconv - 1u
        var acc = 0.0
        var k = 0u
        while (k < dconv) {
            let sp = int(p + k) - int(taps)
            let xv = sp >= 0 ? x[uint(sp) * cd + ch] : hist[ch * taps + (p + k)]
            acc += xv * w[ch * dconv + k]
            k++
        }
        y[p * cd + ch] = acc / (1.0 + exp(-acc))
    }
}

// Advance the channel-major conv history past an npos window: the new taps are the window's
// last `taps` input rows, shifted in from the old history when npos < taps (straddle). Reads
// land in registers before any write, so the in-place buffer update is safe per channel.
[metal_dispatch(name = "enc_dn_conv_hist", pso = "g_pso_dn_hist", tg = 256, grid = "cd/256", params = "cd : int64")]
class MetalDnConvHist {
    @ssbo @binding = 0 @role = "read" x : array<float>     // [npos x cd] the window's fused qkv rows
    @ssbo @binding = 1 @role = "readwrite" @off = "hoff" hist : array<float>  // [cd x taps] channel-major history (layer slice)
    @uniform @binding = 2 cd : uint
    @uniform @binding = 3 dconv : uint
    @uniform @binding = 4 npos : uint
    @ssbo @binding = 5 @role = "alias" @off = "hmoff" histm : array<float> // dual-store: the npos-1 window's history (verify shadow region)
    @uniform @binding = 6 mid : uint        // 1 = also store the npos-1 window's history to histm

    [metal_kernel(name="metal_dn_conv_hist_msl")]
    def metal_dn_conv_hist {
        let ch = gl_GlobalInvocationID.x
        if (ch >= cd) {
            return
        }
        let taps = dconv - 1u
        var nh : float[8]                   // dconv <= 8 by the serve gate
        var k = 0u
        while (k < taps) {
            let src = int(npos + k) - int(taps)
            nh[k] = src >= 0 ? x[uint(src) * cd + ch] : hist[ch * taps + uint(src + int(taps))]
            k++
        }
        if (mid != 0u) {                    // every hist read precedes the in-place write below
            k = 0u
            while (k < taps) {
                let src = int(npos - 1u + k) - int(taps)
                histm[ch * taps + k] = src >= 0 ? x[uint(src) * cd + ch] : hist[ch * taps + uint(src + int(taps))]
                k++
            }
        }
        k = 0u
        while (k < taps) {
            hist[ch * taps + k] = nh[k]
            k++
        }
    }
}

// Per-head L2 norm over the conv-out q/k regions in place, q pre-scale folded (l2_norm_rows +
// scale_inplace): one simdgroup per (head block, position). CPU accumulates the square-sum in
// DOUBLE — the f32 simd tree here is the deltanet drift class (same as the Vulkan tier).
[metal_dispatch(name = "enc_dn_l2norm", pso = "g_pso_dn_l2", tg = 256, grid = "nblk/8, npos", params = "nblk : int64, npos : int64")]
class MetalDnL2Norm {
    @ssbo @binding = 0 @role = "readwrite" y : array<float>     // [npos x cd] conv rows: q at 0, k at kd
    @uniform @binding = 1 cd : uint
    @uniform @binding = 2 kd : uint         // key_dim = nkh * ds
    @uniform @binding = 3 ds : uint         // 128 (gated)
    @uniform @binding = 4 eps : float
    @uniform @binding = 5 qscale : float    // 1/sqrt(ds); k blocks get 1

    [metal_kernel(name="metal_dn_l2norm_msl")]
    def metal_dn_l2norm {
        let blk = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        let p = gl_WorkGroupID.y
        let nblk = (2u * kd) / ds
        if (blk >= nblk) {
            return
        }
        let lane = gl_SubgroupInvocationID
        let base = p * cd + blk * ds + lane * 4u
        var v : float[4]
        var ss = 0.0
        for [unroll_full] (c in range(4)) {
            v[c] = y[base + uint(c)]
            ss += v[c] * v[c]
        }
        ss = simd_sum(ss)
        let sc = (1.0 / max(sqrt(ss), eps)) * (blk < kd / ds ? qscale : 1.0)
        for [unroll_full] (c in range(4)) {
            y[base + uint(c)] = v[c] * sc
        }
    }
}

// The fused sequential delta rule (llama.cpp's gated_delta_net shape): one simdgroup per
// (state column j, v-head) holds S[:,j] in registers and walks ALL npos tokens in-kernel —
// zero per-token state traffic; β/σ/softplus fold into the loop (redundant per column, trivial).
[metal_dispatch(name = "enc_dn_scan", pso = "g_pso_dn_scan", tg = 128, grid = "ds/4, nvh", params = "ds : int64, nvh : int64")]
class MetalDnScan {
    @ssbo @binding = 0 @role = "read" cv : array<float>    // [npos x cd] conv rows: q at 0 / k at kd / v at 2kd
    @ssbo @binding = 1 @role = "readwrite" @off = "stoff" st : array<float>    // state layer slice [nvh x ds x ds] row-major (i·ds + j)
    @ssbo @binding = 2 @role = "read" braw : array<float>  // [npos x nvh] raw beta projections
    @ssbo @binding = 3 @role = "read" graw : array<float>  // [npos x nvh] raw alpha projections
    @ssbo @binding = 4 @role = "weight" aa : array<float>    // [nvh] ssm_a = -exp(A_log) (fblob row)
    @ssbo @binding = 5 @role = "weight" dtb : array<float>   // [nvh] dt_bias (fblob row)
    @ssbo @binding = 6 @role = "write" o : array<float>     // [npos x di] delta-rule output rows
    @uniform @binding = 7 cd : uint
    @uniform @binding = 8 kd : uint
    @uniform @binding = 9 ds : uint         // 128 (gated)
    @uniform @binding = 10 nvh : uint
    @uniform @binding = 11 nkh : uint
    @uniform @binding = 12 di : uint        // nvh * ds
    @uniform @binding = 13 npos : uint
    @ssbo @binding = 14 @role = "alias" @off = "stmoff" stm : array<float>  // dual-store: post-token-(npos-2) state (verify shadow region)
    @uniform @binding = 15 mid : uint       // 1 = store S after token npos-2 to stm

    [metal_kernel(name="metal_dn_scan_msl")]
    def metal_dn_scan {
        let j = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        let h = gl_WorkGroupID.y
        if (j >= ds) {
            return
        }
        let lane = gl_SubgroupInvocationID
        let kh = h % nkh
        let qoff = kh * ds + lane * 4u
        let koff = kd + kh * ds + lane * 4u
        let voff = 2u * kd + h * ds + j
        let a_h = aa[h]
        let dt_h = dtb[h]
        let s0 = h * ds * ds + (lane * 4u) * ds + j
        var ls : float[4]
        for [unroll_full] (c in range(4)) {
            ls[c] = st[s0 + uint(c) * ds]
        }
        var p = 0u
        while (p < npos) {
            let gr = graw[p * nvh + h] + dt_h
            let dec = exp(a_h * (gr > 20.0 ? gr : log(1.0 + exp(gr))))
            let beta = 1.0 / (1.0 + exp(-braw[p * nvh + h]))
            let cb = p * cd
            var kv4 : float[4]
            var sk = 0.0
            for [unroll_full] (c in range(4)) {
                kv4[c] = cv[cb + koff + uint(c)]
                ls[c] *= dec
                sk += kv4[c] * ls[c]
            }
            sk = simd_sum(sk)
            let d = (cv[cb + voff] - sk) * beta
            var yy = 0.0
            for [unroll_full] (c in range(4)) {
                ls[c] += kv4[c] * d
                yy += cv[cb + qoff + uint(c)] * ls[c]
            }
            yy = simd_sum(yy)
            if (lane == 0u) {
                o[p * di + h * ds + j] = yy
            }
            if (mid != 0u && p + 2u == npos) {
                for [unroll_full] (c in range(4)) {
                    stm[s0 + uint(c) * ds] = ls[c]
                }
            }
            p++
        }
        for [unroll_full] (c in range(4)) {
            st[s0 + uint(c) * ds] = ls[c]
        }
    }
}

// Gated out-norm (dn_token_core's tail): per (v-head, position) RMSNorm over ds with the
// SHARED ssm_norm row, then ⊙ SiLU(z) — in place on o, one simdgroup per (head, position).
[metal_dispatch(name = "enc_dn_gate", pso = "g_pso_dn_gate", tg = 256, grid = "nvh/8, npos", params = "nvh : int64, npos : int64")]
class MetalDnGate {
    @ssbo @binding = 0 @role = "readwrite" o : array<float>     // [npos x di] delta-rule output, gated in place
    @ssbo @binding = 1 @role = "read" z : array<float>     // [npos x di] raw gate projections
    @ssbo @binding = 2 @role = "weight" wn : array<float>    // [ds] shared norm weight (fblob row)
    @uniform @binding = 3 ds : uint         // 128 (gated)
    @uniform @binding = 4 di : uint
    @uniform @binding = 5 eps : float

    [metal_kernel(name="metal_dn_gate_msl")]
    def metal_dn_gate {
        let h = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        let p = gl_WorkGroupID.y
        if (h >= di / ds) {
            return
        }
        let lane = gl_SubgroupInvocationID
        let base = p * di + h * ds + lane * 4u
        var v : float[4]
        var ss = 0.0
        for [unroll_full] (c in range(4)) {
            v[c] = o[base + uint(c)]
            ss += v[c] * v[c]
        }
        ss = simd_sum(ss)
        let inv = rsqrt(ss / float(ds) + eps)
        for [unroll_full] (c in range(4)) {
            let zz = z[base + uint(c)]
            o[base + uint(c)] = (wn[lane * 4u + uint(c)] * (v[c] * inv)) * (zz / (1.0 + exp(-zz)))
        }
    }
}

// Deinterleave the 2x-wide gated-attention Q projection (head h's q at 2h·hs, its gate at
// (2h+1)·hs): q lands in the qkv panel's q segment, the gate in its own rows — keeps the
// rope/attention classes untouched by the packing.
[metal_dispatch(name = "enc_dn_deint", pso = "g_pso_dn_deint", tg = 256, grid = "qd/256, npos", params = "qd : int64, npos : int64")]
class MetalDnDeint {
    @ssbo @binding = 0 @role = "read" qg : array<float>    // [npos x 2qd] packed [q|gate] projection rows
    @ssbo @binding = 1 @role = "write" q : array<float>     // qkv panel, q segment at row stride qs
    @ssbo @binding = 2 @role = "write" gt : array<float>    // [npos x qd] sigmoid-gate rows
    @uniform @binding = 3 hs : uint
    @uniform @binding = 4 qd : uint
    @uniform @binding = 5 qs : uint         // q panel row stride, elements (qkvd; decode binds qd)

    [metal_kernel(name="metal_dn_deint_msl")]
    def metal_dn_deint {
        let i = gl_GlobalInvocationID.x
        let p = gl_GlobalInvocationID.y
        if (i >= qd) {
            return
        }
        let sb = p * 2u * qd + (i / hs) * 2u * hs + i % hs
        q[p * qs + i] = qg[sb]
        gt[p * qd + i] = qg[sb + hs]
    }
}

// Per-head sigmoid output gate (sigmoid_gate): x ⊙ σ(g) in place — the gated-attention
// epilogue between the attention output and the out projection.
class MetalSigmul {
    @ssbo @binding = 0 x : array<float>
    @ssbo @binding = 1 g : array<float>
    @uniform @binding = 2 total : uint      // grid guard

    [metal_kernel(name="metal_sigmul_msl")]
    def metal_sigmul {
        let gid = gl_GlobalInvocationID.x
        if (gid >= total) {
            return
        }
        x[gid] = x[gid] / (1.0 + exp(-g[gid]))
    }
}

// Shared-expert combine (qwen35moe): y += x ⊙ σ(g[row]) — one raw gate dot per position
// (decode total = dim; prefill total = npos x dim, row = the position).
[metal_dispatch(name = "enc_axpy_sig", pso = "g_pso_axpy_sig", tg = 256, grid = "total/256", params = "total : int64")]
class MetalAxpySig {
    @ssbo @binding = 0 @role = "readwrite" y : array<float>
    @ssbo @binding = 1 @role = "read" x : array<float>
    @ssbo @binding = 2 @role = "read" g : array<float>     // per-position raw gate dots (σ applied here)
    @uniform @binding = 3 total : uint      // grid guard
    @uniform @binding = 4 dim : uint

    [metal_kernel(name="metal_axpy_sig_msl")]
    def metal_axpy_sig {
        let gid = gl_GlobalInvocationID.x
        if (gid >= total) {
            return
        }
        y[gid] += x[gid] / (1.0 + exp(-g[gid / dim]))
    }
}

// Batched narrow q8 GEMV for the β/α projections (d = dt_rank, 16..64 — under every GEMM
// N tile): one simdgroup per (output row, position), the MetalQ8Gemv stream walk per row.
class MetalDnBa {
    @ssbo @binding = 0 wsh : array<float16> // 34B-block W blob, half-scale view (block b at 17*b)
    @ssbo @binding = 1 wqb : array<int8>    // the same blob buffer, byte view (quants at 34*b + 2)
    @ssbo @binding = 2 x : array<float4>    // [npos x n] f32 rows, float4 view
    @ssbo @binding = 3 y : array<float>     // [npos x d]
    @uniform @binding = 4 ndim : uint       // n (reduction dim, % 32)
    @uniform @binding = 5 ddim : uint       // d (output rows per position)

    [metal_kernel(name="metal_dn_ba_msl")]
    def metal_dn_ba {
        let row = gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID
        let p = gl_WorkGroupID.y
        if (row >= ddim) {
            return
        }
        let lane = gl_SubgroupInvocationID
        let nb = ndim / 32u
        let nb4 = ndim / 4u
        var sp = unsafe(addr(wsh[(row * nb + lane / 8u) * 17u]))
        var qp = unsafe(addr(wqb[(row * nb + lane / 8u) * 34u + 2u + (lane % 8u) * 4u]))
        var x0 = unsafe(addr(x[p * nb4 + lane]))
        var acc = 0.0
        var lw : float4[4]
        var k = lane
        while (k + 96u < nb4) {
            // all 4 chunk dequants batch AHEAD of the fma chains (the MvB2 load batching)
            for [unroll_full] (c in range(4)) {
                let d = float(unsafe(sp[uint(c) * 68u]))   // chunk c sits 4 blocks (68 halves) on
                let qb = uint(c) * 136u                    // ... and 136 quant bytes
                lw[c] = float4(float(unsafe(qp[qb])), float(unsafe(qp[qb + 1u])),
                    float(unsafe(qp[qb + 2u])), float(unsafe(qp[qb + 3u]))) * d
            }
            for [unroll_full] (c in range(4)) {
                let xv = unsafe(x0[uint(c) * 32u])
                acc += lw[c].x * xv.x + lw[c].y * xv.y + lw[c].z * xv.z + lw[c].w * xv.w
            }
            sp = unsafe(sp + 272)           // 16 blocks x 17 halves per stripe
            qp = unsafe(qp + 544)           // 16 blocks x 34 bytes
            x0 = unsafe(x0 + 128)
            k += 128u
        }
        while (k < nb4) {
            let bi = row * nb + k / 8u
            let d = float(wsh[bi * 17u])
            let qb = bi * 34u + 2u + (k % 8u) * 4u
            let xv = x[p * nb4 + k]
            acc += (float(wqb[qb]) * xv.x + float(wqb[qb + 1u]) * xv.y +
                float(wqb[qb + 2u]) * xv.z + float(wqb[qb + 3u]) * xv.w) * d
            k += gl_SubgroupSize
        }
        acc = simd_sum(acc)
        if (lane == 0u) {
            y[p * ddim + row] = acc
        }
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
// what the prefill kernels implement beyond the base llama dense block — grows wave by wave
let PF_NEEDS_OK = (MetalNeed.neox | MetalNeed.qkv_bias | MetalNeed.qk_norm | MetalNeed.qd_neq_dim |
    MetalNeed.softcap | MetalNeed.sliding | MetalNeed.pre_post_norm | MetalNeed.geglu |
    MetalNeed.v_norm | MetalNeed.attn_scale | MetalNeed.out_scale |
    MetalNeed.sinks | MetalNeed.out_bias | MetalNeed.partial_rope | MetalNeed.q_gated |
    MetalNeed.ple)

var private g_min_npos = 64l
var private g_max_npos = 2048l          // naive-attention P footprint cap: heads x npos^2 floats
var private g_prefills = 0l
var private g_pf_pso_dn_conv : MetalComputePipeline?    // deltanet (Wave D) — prefill-local PSO set
var private g_pf_pso_dn_hist : MetalComputePipeline?
var private g_pf_pso_dn_l2 : MetalComputePipeline?
var private g_pf_pso_dn_scan : MetalComputePipeline?
var private g_pf_pso_dn_gate : MetalComputePipeline?
var private g_pf_pso_dn_deint : MetalComputePipeline?
var private g_pf_pso_dn_ba : MetalComputePipeline?
var private g_pf_pso_sigmul : MetalComputePipeline?
var private g_pf_pso_axpy_sig : MetalComputePipeline?   // shexp combine (Wave D stage 3)
var private g_pso_gemm : MetalComputePipeline?
var private g_pso_gemm64 : MetalComputePipeline?
var private g_pf_pso_rms : MetalComputePipeline?
var private g_pso_rope : MetalComputePipeline?
var private g_pso_addbias : MetalComputePipeline?
var private g_pf_pso_qknorm : MetalComputePipeline?
var private g_pso_qk : MetalComputePipeline?
var private g_pso_softmax : MetalComputePipeline?
var private g_pso_av : MetalComputePipeline?
var private g_pf_pso_swiglu : MetalComputePipeline?
var private g_pf_pso_geglu : MetalComputePipeline?
var private g_pf_one : MetalBuffer?         // persistent 1.0f — the rms oscale default bind
var private g_pf_zero : MetalBuffer?        // persistent 0u — the sink/hass identity bind
var private g_pf_pso_softcap : MetalComputePipeline?
var private g_pf_pso_suppress : MetalComputePipeline?
var private g_pf_pso_copy : MetalComputePipeline?
var private g_pf_pso_cat2 : MetalComputePipeline?
var private g_pf_pso_add : MetalComputePipeline?
var private g_pf_pso_mm : MetalComputePipeline?    // the production das mul_mm (34B q8 blocks + f32 X)
var private g_pf_pso_bf16_mm : MetalComputePipeline?   // native-BF16 A twin (E-series model_proj)
var private g_pf_pso_ple_gather : MetalComputePipeline?   // PLE pre-step: q8 token-row gather
var private g_pf_pso_ple_finish : MetalComputePipeline?   // PLE pre-step: rms + combine, in place
var private g_pso_qkmm : MetalComputePipeline?
var private g_pso_avmm : MetalComputePipeline?
var private g_pf_pso_gemv : MetalComputePipeline?
var private g_pso_kq_mm4 : MetalComputePipeline?       // K-quant mul_mm twins (W4/M2)
var private g_pso_kq_mm5 : MetalComputePipeline?
var private g_pso_kq_mm6 : MetalComputePipeline?
var private g_pf_pso_kq4 : MetalComputePipeline?       // ... and the kq classifier GEMVs
var private g_pf_pso_kq5 : MetalComputePipeline?
var private g_pf_pso_kq6 : MetalComputePipeline?      // classifier GEMV (logits-on-GPU)
var private g_pf_pso_moe_router : MetalComputePipeline?   // MoE prefill (Wave C stage 2b)
var private g_pf_pso_moe_router_b : MetalComputePipeline?   // position-blocked prefill router
var private g_pf_pso_moe_select : MetalComputePipeline?
var private g_pf_pso_moe_count : MetalComputePipeline?
var private g_pf_pso_moe_prefix : MetalComputePipeline?
var private g_pf_pso_moe_bucket : MetalComputePipeline?
var private g_pf_pso_moe_reduce : MetalComputePipeline?
var private g_pf_pso_moe_mm_q8 : MetalComputePipeline?
var private g_pf_pso_moe_mm_k4 : MetalComputePipeline?
var private g_pf_pso_moe_mm_k5 : MetalComputePipeline?
var private g_pf_pso_moe_mm_k6 : MetalComputePipeline?
var private g_pf_pso_g4rnorm : MetalComputePipeline?   // gemma4 MoE router-input norm (stage 3)
var private g_pf_pso_moe_wscale : MetalComputePipeline?
var private g_pf_pso_swiglu_oai : MetalComputePipeline?
var private g_pf_pso_moe_mm_mx4 : MetalComputePipeline?
var private g_pf_pso_moe_mm_q51 : MetalComputePipeline?
var private g_pf_pso_moe_bias_rows : MetalComputePipeline?
var private g_pf_mx4_vtab : MetalBuffer?
var private g_mm_legacy = false             // set_metal_prefill_mulmm_legacy — tests' exact-parity rail
var private g_pf_logits_cpu = false            // set_metal_prefill_logits_cpu — the logits A/B rail
// timing-attribution knockout (DASLLAMA_METAL_PREFILL_SKIP=attn|gemm|ew|nongemm|moe|moe_mm|
// moe_act|moe_bias|moe_reduce|moe_only_router|moe_only_route|moe_no_mm_act): skips encoding that
// stage family; consumers of skipped stages always skip too (no garbage OOB). Output GARBAGE, debug only.
var private g_pf_skip = ""

// composite skip predicates for the MoE stages (moe_only_* isolate the route chain per-kernel)
def private pf_skip_moe_mm : bool {
    return (g_pf_skip == "moe_mm" || g_pf_skip == "moe_only_route" || g_pf_skip == "moe_only_router" ||
        g_pf_skip == "moe_only_sel" || g_pf_skip == "moe_no_mm_act")
}

def private pf_skip_moe_act : bool {
    return (g_pf_skip == "moe_act" || g_pf_skip == "moe_only_route" || g_pf_skip == "moe_only_router" ||
        g_pf_skip == "moe_only_sel" || g_pf_skip == "moe_no_mm_act")
}

def private pf_skip_moe_reduce : bool {
    return (g_pf_skip == "moe_reduce" || g_pf_skip == "moe_only_route" || g_pf_skip == "moe_only_router" ||
        g_pf_skip == "moe_only_sel")
}

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

//! The prefill knockout rail as an in-process setter (timing attribution — served output is
//! GARBAGE while non-empty): the `DASLLAMA_METAL_PREFILL_SKIP` families above; `""` restores
//! full math. Switchable between measurement windows, same contract as `set_metal_decode_skip`.
def public set_metal_prefill_skip(v : string) {
    g_pf_skip = v
}

// cumulative per-stage prefill wall since load/reset — the pp chase harness's numbers rail
var private g_pf_stage_prefills = 0l
var private g_pf_stage_setup_us = 0l
var private g_pf_stage_encwait_us = 0l
var private g_pf_stage_encode_ms = 0.0lf
var private g_pf_stage_gpu_ms = 0.0lf
var private g_pf_stage_rb_us = 0l

//! Zero the cumulative prefill stage timers + the served-prefill counter — windows a chase run's
//! stage stats to one measurement loop (mirrors `metal_decode_stage_reset`).
def public metal_prefill_stage_reset {
    g_pf_stage_prefills = 0l
    g_pf_stage_setup_us = 0l
    g_pf_stage_encwait_us = 0l
    g_pf_stage_encode_ms = 0.0lf
    g_pf_stage_gpu_ms = 0.0lf
    g_pf_stage_rb_us = 0l
}

//! Cumulative per-stage prefill wall since load/reset — (prefills served, CPU setup us, encode us,
//! commit+wait us minus encode, GPU-timestamp us within the window, kv+readback us). Mirrors
//! `metal_decode_stage_stats`.
def public metal_prefill_stage_stats() : tuple<prefills : int64; setup_us : int64; encode_us : int64; wait_us : int64; gpu_us : double; readback_us : int64> {
    let enc_us = int64(g_pf_stage_encode_ms * 1000.0lf)
    return (prefills = g_pf_stage_prefills, setup_us = g_pf_stage_setup_us,
        encode_us = enc_us, wait_us = g_pf_stage_encwait_us - enc_us,
        gpu_us = g_pf_stage_gpu_ms * 1000.0lf, readback_us = g_pf_stage_rb_us)
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
    if (g_pf_pso_rms != null) {
        metal_release(g_pf_pso_rms)
        g_pf_pso_rms = null
    }
    if (g_pf_pso_gemv != null) {
        metal_release(g_pf_pso_gemv)
        g_pf_pso_gemv = null
    }
    if (g_pso_rope != null) {
        metal_release(g_pso_rope)
        g_pso_rope = null
    }
    if (g_pf_pso_dn_conv != null) {
        metal_release(g_pf_pso_dn_conv)
        g_pf_pso_dn_conv = null
    }
    if (g_pf_pso_dn_hist != null) {
        metal_release(g_pf_pso_dn_hist)
        g_pf_pso_dn_hist = null
    }
    if (g_pf_pso_dn_l2 != null) {
        metal_release(g_pf_pso_dn_l2)
        g_pf_pso_dn_l2 = null
    }
    if (g_pf_pso_dn_scan != null) {
        metal_release(g_pf_pso_dn_scan)
        g_pf_pso_dn_scan = null
    }
    if (g_pf_pso_dn_gate != null) {
        metal_release(g_pf_pso_dn_gate)
        g_pf_pso_dn_gate = null
    }
    if (g_pf_pso_dn_deint != null) {
        metal_release(g_pf_pso_dn_deint)
        g_pf_pso_dn_deint = null
    }
    if (g_pf_pso_dn_ba != null) {
        metal_release(g_pf_pso_dn_ba)
        g_pf_pso_dn_ba = null
    }
    if (g_pf_pso_sigmul != null) {
        metal_release(g_pf_pso_sigmul)
        g_pf_pso_sigmul = null
    }
    if (g_pf_pso_axpy_sig != null) {
        metal_release(g_pf_pso_axpy_sig)
        g_pf_pso_axpy_sig = null
    }
    if (g_pso_addbias != null) {
        metal_release(g_pso_addbias)
        g_pso_addbias = null
    }
    if (g_pf_pso_qknorm != null) {
        metal_release(g_pf_pso_qknorm)
        g_pf_pso_qknorm = null
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
    if (g_pf_one != null) {
        metal_release(g_pf_one)
        g_pf_one = null
    }
    if (g_pf_zero != null) {
        metal_release(g_pf_zero)
        g_pf_zero = null
    }
    if (g_pf_pso_geglu != null) {
        metal_release(g_pf_pso_geglu)
        g_pf_pso_geglu = null
    }
    if (g_pf_pso_softcap != null) {
        metal_release(g_pf_pso_softcap)
        g_pf_pso_softcap = null
    }
    if (g_pf_pso_suppress != null) {
        metal_release(g_pf_pso_suppress)
        g_pf_pso_suppress = null
    }
    if (g_pf_pso_copy != null) {
        metal_release(g_pf_pso_copy)
        g_pf_pso_copy = null
    }
    if (g_pf_pso_cat2 != null) {
        metal_release(g_pf_pso_cat2)
        g_pf_pso_cat2 = null
    }
    if (g_pf_pso_add != null) {
        metal_release(g_pf_pso_add)
        g_pf_pso_add = null
    }
    if (g_pf_pso_mm != null) {
        metal_release(g_pf_pso_mm)
        g_pf_pso_mm = null
    }
    if (g_pf_pso_bf16_mm != null) {
        metal_release(g_pf_pso_bf16_mm)
        g_pf_pso_bf16_mm = null
    }
    if (g_pf_pso_ple_gather != null) {
        metal_release(g_pf_pso_ple_gather)
        g_pf_pso_ple_gather = null
    }
    if (g_pf_pso_ple_finish != null) {
        metal_release(g_pf_pso_ple_finish)
        g_pf_pso_ple_finish = null
    }
    for (pso in [g_pso_kq_mm4, g_pso_kq_mm5, g_pso_kq_mm6, g_pf_pso_kq4, g_pf_pso_kq5, g_pf_pso_kq6,
                 g_pf_pso_moe_router, g_pf_pso_moe_router_b, g_pf_pso_moe_select, g_pf_pso_moe_count, g_pf_pso_moe_prefix,
                 g_pf_pso_moe_bucket, g_pf_pso_moe_reduce, g_pf_pso_moe_mm_q8, g_pf_pso_moe_mm_k4,
                 g_pf_pso_moe_mm_k5, g_pf_pso_moe_mm_k6, g_pf_pso_g4rnorm, g_pf_pso_moe_wscale, g_pf_pso_swiglu_oai,
                 g_pf_pso_moe_mm_mx4, g_pf_pso_moe_mm_q51, g_pf_pso_moe_bias_rows]) {
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
    g_pf_pso_moe_router = null
    g_pf_pso_moe_router_b = null
    g_pf_pso_moe_select = null
    g_pf_pso_moe_count = null
    g_pf_pso_moe_prefix = null
    g_pf_pso_moe_bucket = null
    g_pf_pso_moe_reduce = null
    g_pf_pso_moe_mm_q8 = null
    g_pf_pso_moe_mm_k4 = null
    g_pf_pso_moe_mm_k5 = null
    g_pf_pso_moe_mm_k6 = null
    g_pf_pso_g4rnorm = null
    g_pf_pso_moe_wscale = null
    g_pf_pso_swiglu_oai = null
    g_pf_pso_moe_mm_mx4 = null
    g_pf_pso_moe_mm_q51 = null
    g_pf_pso_moe_bias_rows = null
    if (g_pf_mx4_vtab != null) {
        metal_release(g_pf_mx4_vtab)
        g_pf_mx4_vtab = null
    }
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
    g_pf_pso_rms = pf_compile_pso(metal_rmsnorm_msl, metal_rmsnorm_msl_entry, metal_rmsnorm_msl_fastmath, ok)
    g_pso_rope = pf_compile_pso(metal_rope_msl, metal_rope_msl_entry, metal_rope_msl_fastmath, ok)
    g_pso_addbias = pf_compile_pso(metal_add_bias_rows_msl, metal_add_bias_rows_msl_entry, metal_add_bias_rows_msl_fastmath, ok)
    g_pf_pso_qknorm = pf_compile_pso(metal_qk_norm_msl, metal_qk_norm_msl_entry, metal_qk_norm_msl_fastmath, ok)
    g_pso_qk = pf_compile_pso(metal_attn_qk_msl, metal_attn_qk_msl_entry, metal_attn_qk_msl_fastmath, ok)
    g_pso_softmax = pf_compile_pso(metal_attn_softmax_msl, metal_attn_softmax_msl_entry, metal_attn_softmax_msl_fastmath, ok)
    g_pso_av = pf_compile_pso(metal_attn_av_msl, metal_attn_av_msl_entry, metal_attn_av_msl_fastmath, ok)
    g_pf_pso_swiglu = pf_compile_pso(metal_swiglu_msl, metal_swiglu_msl_entry, metal_swiglu_msl_fastmath, ok)
    g_pf_one = metal_new_buffer(g_pf_dev, 4ul)
    unsafe {
        var onep = reinterpret<float?>(metal_buffer_contents(g_pf_one))
        onep[0] = 1.0
    }
    g_pf_zero = metal_new_buffer(g_pf_dev, 4ul)
    unsafe {
        var zp = reinterpret<uint?>(metal_buffer_contents(g_pf_zero))
        zp[0] = 0u
    }
    g_pf_pso_geglu = pf_compile_pso(metal_geglu_msl, metal_geglu_msl_entry, metal_geglu_msl_fastmath, ok)
    g_pf_pso_softcap = pf_compile_pso(metal_softcap_row_msl, metal_softcap_row_msl_entry, metal_softcap_row_msl_fastmath, ok)
    g_pf_pso_suppress = pf_compile_pso(metal_suppress_row_msl, metal_suppress_row_msl_entry, metal_suppress_row_msl_fastmath, ok)
    g_pf_pso_copy = pf_compile_pso(metal_pf_copy_msl, metal_pf_copy_msl_entry, metal_pf_copy_msl_fastmath, ok)
    g_pf_pso_cat2 = pf_compile_pso(metal_pf_cat2_msl, metal_pf_cat2_msl_entry, metal_pf_cat2_msl_fastmath, ok)
    g_pf_pso_add = pf_compile_pso(metal_add_msl, metal_add_msl_entry, metal_add_msl_fastmath, ok)
    g_pf_pso_mm = pf_compile_pso(metal_q8_mulmm_msl, metal_q8_mulmm_msl_entry, metal_q8_mulmm_msl_fastmath, ok)
    g_pf_pso_bf16_mm = pf_compile_pso(metal_bf16_mulmm_msl, metal_bf16_mulmm_msl_entry, metal_bf16_mulmm_msl_fastmath, ok)
    g_pf_pso_ple_gather = pf_compile_pso(metal_ple_gather_q8_msl, metal_ple_gather_q8_msl_entry, metal_ple_gather_q8_msl_fastmath, ok)
    g_pf_pso_ple_finish = pf_compile_pso(metal_ple_finish_msl, metal_ple_finish_msl_entry, metal_ple_finish_msl_fastmath, ok)
    g_pso_kq_mm4 = pf_compile_pso(metal_kq_mulmm_k4_msl, metal_kq_mulmm_k4_msl_entry, metal_kq_mulmm_k4_msl_fastmath, ok)
    g_pso_kq_mm5 = pf_compile_pso(metal_kq_mulmm_k5_msl, metal_kq_mulmm_k5_msl_entry, metal_kq_mulmm_k5_msl_fastmath, ok)
    g_pso_kq_mm6 = pf_compile_pso(metal_kq_mulmm_k6_msl, metal_kq_mulmm_k6_msl_entry, metal_kq_mulmm_k6_msl_fastmath, ok)
    g_pf_pso_kq4 = pf_compile_pso(metal_kq_gemv_k4_msl, metal_kq_gemv_k4_msl_entry, metal_kq_gemv_k4_msl_fastmath, ok)
    g_pf_pso_kq5 = pf_compile_pso(metal_kq_gemv_k5_msl, metal_kq_gemv_k5_msl_entry, metal_kq_gemv_k5_msl_fastmath, ok)
    g_pf_pso_kq6 = pf_compile_pso(metal_kq_gemv_k6_msl, metal_kq_gemv_k6_msl_entry, metal_kq_gemv_k6_msl_fastmath, ok)
    g_pso_qkmm = pf_compile_pso(metal_attn_qk_mm_msl, metal_attn_qk_mm_msl_entry, metal_attn_qk_mm_msl_fastmath, ok)
    g_pso_avmm = pf_compile_pso(metal_attn_av_mm_msl, metal_attn_av_mm_msl_entry, metal_attn_av_mm_msl_fastmath, ok)
    g_pf_pso_gemv = pf_compile_pso(metal_q8_gemv_msl, metal_q8_gemv_msl_entry, metal_q8_gemv_msl_fastmath, ok)
    g_pf_pso_moe_router = pf_compile_pso(metal_moe_router_msl, metal_moe_router_msl_entry, metal_moe_router_msl_fastmath, ok)
    g_pf_pso_moe_router_b = pf_compile_pso(metal_moe_router_b_msl, metal_moe_router_b_msl_entry, metal_moe_router_b_msl_fastmath, ok)
    g_pf_pso_moe_select = pf_compile_pso(metal_moe_select_msl, metal_moe_select_msl_entry, metal_moe_select_msl_fastmath, ok)
    g_pf_pso_moe_count = pf_compile_pso(metal_moe_count_msl, metal_moe_count_msl_entry, metal_moe_count_msl_fastmath, ok)
    g_pf_pso_moe_prefix = pf_compile_pso(metal_moe_prefix_msl, metal_moe_prefix_msl_entry, metal_moe_prefix_msl_fastmath, ok)
    g_pf_pso_moe_bucket = pf_compile_pso(metal_moe_bucket_msl, metal_moe_bucket_msl_entry, metal_moe_bucket_msl_fastmath, ok)
    g_pf_pso_moe_reduce = pf_compile_pso(metal_moe_reduce_msl, metal_moe_reduce_msl_entry, metal_moe_reduce_msl_fastmath, ok)
    g_pf_pso_moe_mm_q8 = pf_compile_pso(metal_moe_mulmm_q8_msl, metal_moe_mulmm_q8_msl_entry, metal_moe_mulmm_q8_msl_fastmath, ok)
    g_pf_pso_moe_mm_k4 = pf_compile_pso(metal_moe_mulmm_k4_msl, metal_moe_mulmm_k4_msl_entry, metal_moe_mulmm_k4_msl_fastmath, ok)
    g_pf_pso_moe_mm_k5 = pf_compile_pso(metal_moe_mulmm_k5_msl, metal_moe_mulmm_k5_msl_entry, metal_moe_mulmm_k5_msl_fastmath, ok)
    g_pf_pso_moe_mm_k6 = pf_compile_pso(metal_moe_mulmm_k6_msl, metal_moe_mulmm_k6_msl_entry, metal_moe_mulmm_k6_msl_fastmath, ok)
    g_pf_pso_g4rnorm = pf_compile_pso(metal_g4_router_norm_msl, metal_g4_router_norm_msl_entry, metal_g4_router_norm_msl_fastmath, ok)
    g_pf_pso_moe_wscale = pf_compile_pso(metal_moe_wscale_msl, metal_moe_wscale_msl_entry, metal_moe_wscale_msl_fastmath, ok)
    g_pf_pso_swiglu_oai = pf_compile_pso(metal_swiglu_oai_msl, metal_swiglu_oai_msl_entry, metal_swiglu_oai_msl_fastmath, ok)
    g_pf_pso_moe_mm_mx4 = pf_compile_pso(metal_moe_mulmm_mx4_msl, metal_moe_mulmm_mx4_msl_entry, metal_moe_mulmm_mx4_msl_fastmath, ok)
    g_pf_pso_moe_mm_q51 = pf_compile_pso(metal_moe_mulmm_q51_msl, metal_moe_mulmm_q51_msl_entry, metal_moe_mulmm_q51_msl_fastmath, ok)
    g_pf_pso_moe_bias_rows = pf_compile_pso(metal_moe_bias_rows_msl, metal_moe_bias_rows_msl_entry, metal_moe_bias_rows_msl_fastmath, ok)
    g_pf_pso_dn_conv = pf_compile_pso(metal_dn_conv_msl, metal_dn_conv_msl_entry, metal_dn_conv_msl_fastmath, ok)
    g_pf_pso_dn_hist = pf_compile_pso(metal_dn_conv_hist_msl, metal_dn_conv_hist_msl_entry, metal_dn_conv_hist_msl_fastmath, ok)
    g_pf_pso_dn_l2 = pf_compile_pso(metal_dn_l2norm_msl, metal_dn_l2norm_msl_entry, metal_dn_l2norm_msl_fastmath, ok)
    g_pf_pso_dn_scan = pf_compile_pso(metal_dn_scan_msl, metal_dn_scan_msl_entry, metal_dn_scan_msl_fastmath, ok)
    g_pf_pso_dn_gate = pf_compile_pso(metal_dn_gate_msl, metal_dn_gate_msl_entry, metal_dn_gate_msl_fastmath, ok)
    g_pf_pso_dn_deint = pf_compile_pso(metal_dn_deint_msl, metal_dn_deint_msl_entry, metal_dn_deint_msl_fastmath, ok)
    g_pf_pso_dn_ba = pf_compile_pso(metal_dn_ba_msl, metal_dn_ba_msl_entry, metal_dn_ba_msl_fastmath, ok)
    g_pf_pso_sigmul = pf_compile_pso(metal_sigmul_msl, metal_sigmul_msl_entry, metal_sigmul_msl_fastmath, ok)
    g_pf_pso_axpy_sig = pf_compile_pso(metal_axpy_sig_msl, metal_axpy_sig_msl_entry, metal_axpy_sig_msl_fastmath, ok)
    g_pf_mx4_vtab = metal_new_buffer(g_pf_dev, 64ul)
    unsafe {
        var vp = reinterpret<float?>(metal_buffer_contents(g_pf_mx4_vtab))
        let mags = fixed_array(0.0, 1.0, 2.0, 3.0, 4.0, 6.0, 8.0, 12.0)
        for (vi in range(16)) {
            vp[vi] = vi < 8 ? mags[vi] : -mags[vi - 8]
        }
    }
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

// one tensor's slice of the whole-model blob on THIS driver's device (blob_of's pf twin)
def private pf_blob_of(t : Model; off : int64) : tuple<buf : MetalBuffer?; boff : uint64> {
    return blob_of(g_pf_dev, t, off)
}

def private pf_bf16_of(t : Model; off : int64) : tuple<buf : MetalBuffer?; boff : uint64> {
    return bf16_of(g_pf_dev, t, off)
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

// the mul_mm %64 shape gate over EVERY GEMM'd width — both attention classes' qd/kv_dim
// (hetero models GEMM each class's width; uniform models reduce to the old layer-0 check)
def private pf_mm_shape64(t : Model) : bool {
    let c = t.config
    let hs_sw = c.head_size_swa > 0l ? c.head_size_swa : c.head_size
    let kvh_sw = c.n_kv_heads_swa > 0l ? c.n_kv_heads_swa : c.n_kv_heads
    return ((c.dim % 64l) == 0l && (layer_hidden(t, 0l) % 64l) == 0l &&
        ((c.n_heads * c.head_size) % 64l) == 0l && ((c.n_kv_heads * c.head_size) % 64l) == 0l &&
        ((c.n_heads * hs_sw) % 64l) == 0l && ((kvh_sw * hs_sw) % 64l) == 0l)
}

// the std dense stack + PF_NEEDS_OK features are wired — anything else declines to the CPU
// loop. Returns the FIRST failing clause; .none = supported.
[unused_argument(start_pos)]   // start_pos > 0 (a continuation chunk) is served (M2.5): the
// existing rows gather into the K/V panels and the causal diagonal shifts by qoff — the gate
// keeps the argument so future clauses can window it
def private prefill_decline(t : Model; s : Session; npos, start_pos : int64) : MetalPrefillDecline {
    // npos window is a POLICY decline (short prefills were cheaper on CPU); blob-only has no CPU path, so any npos serves
    if (!t.metal_blob && (npos < g_min_npos || npos > g_max_npos)) {
        return MetalPrefillDecline.npos_window
    }
    if (t.quant != QuantMode.q8) {
        return MetalPrefillDecline.quant_mode
    }
    // kquant rides the mul_mm path only — mm pinned off or a shape the %64 gate rejects has no kq-capable GEMM and declines
    if (t.kquant_native) {
        // kq_load_gpu_supported rejects fmts without a GPU kernel (q4_0 QAT tier) before they dispatch a wrong-layout branch
        if (g_mm_legacy || get_env_variable("DASLLAMA_METAL_MULMM") == "0" ||
                !pf_mm_shape64(t) || !kq_load_gpu_supported(t)) {
            return MetalPrefillDecline.kquant_native
        }
    }
    // wscale_f16 rides the mul_mm path only — a model the mm shape gate rejects has no s16-capable GEMM and declines
    if (t.wscale_f16) {
        if (g_mm_legacy || get_env_variable("DASLLAMA_METAL_MULMM") == "0" || !pf_mm_shape64(t)) {
            return MetalPrefillDecline.wscale_f16
        }
    }
    // non-std graphs decline; deltanet hybrids serve via dn_metal_ok (ds == 128 makes every dn GEMM width %128)
    if ((!t.blocks.attn_is_std && !(t.config.hybrid_deltanet && dn_metal_ok(t))) ||
            (!t.blocks.ffn_is_dense && (!moe_metal_ok(t) ||
             (t.config.n_ff_exp % 64l) != 0l || (t.config.dim % 64l) != 0l))) {
        return MetalPrefillDecline.graph
    }
    // the GEMM kernel reads ROW-MAJOR Q8 weights — a repack backend's interleaved layout is unreadable here
    if (kernel_backend_needs_repack(active_kernel_backend())) {
        return MetalPrefillDecline.backend_repack
    }
    if (metal_missing_needs(t, PF_NEEDS_OK) != default<MetalNeed>) {
        return MetalPrefillDecline.feature
    }
    let c = t.config
    let hidden = layer_hidden(t, 0l)
    if ((c.dim % 32l) != 0l || (hidden % 32l) != 0l) {
        return MetalPrefillDecline.shape
    }
    // per-CLASS: head_size/kv_dim % 32 grid the AV kernel (QK k-slabs any size); hetero geometry and missing attn_v serve, hidden stays pinned
    for (l in range64(c.n_layers)) {
        if ((layer_head_size(c, l) % 32l) != 0l || (layer_kv_dim(c, l) % 32l) != 0l) {
            return MetalPrefillDecline.shape
        }
        // E-series shared-KV layers (Q-only) serve when the source is byte-compatible — the panel aliases it
        if ((t.kv_src[l] != l && !kv_share_ok(t, l)) || layer_hidden(t, l) != hidden) {
            return MetalPrefillDecline.layers
        }
    }
    // AFTER the capability gates (the support matrix asserts those reasons): drivers are BLOB-ONLY — planar q8 has no upload path, and a blob model rides the mul_mm rail exclusively
    if (!t.metal_blob || g_mm_legacy || get_env_variable("DASLLAMA_METAL_MULMM") == "0" ||
            !pf_mm_shape64(t)) {
        return MetalPrefillDecline.planar
    }
    // rope tables are the hook's input (partial rope: rows rope_dim/2 wide; dual-rope: swa class width)
    let swa_hs = c.head_size_swa > 0l ? c.head_size_swa : c.head_size
    if (int64(length(s.rope_cos)) < npos * ((c.rope_dim > 0l ? c.rope_dim : c.head_size) / 2l) ||
            (has_dual_rope(c) && int64(length(s.rope_cos_swa)) < npos * (swa_hs / 2l))) {
        return MetalPrefillDecline.rope_rows
    }
    return MetalPrefillDecline.none
}


// the production mul_mm dispatch: W blob bound twice (half-scale + byte views), raw f32 X,
// grid (mp/32 token tiles, d/64 output tiles). Requires d % 64 == 0 and mp % 64 == 0.
// yoff places the K/V outputs past a continuation's existing panel rows.
def private enc_gemm_mm(enc : MetalComputeEncoder?; bwblob : MetalBuffer?; wboff : uint64; bx, by, bk, bn : MetalBuffer?; mp, d : int64; yoff : uint64 = 0ul) {
    metal_set_pipeline(enc, g_pf_pso_mm)
    metal_set_threadgroup_memory_length(enc, metal_q8_mulmm_msl_tgmem, 0)
    metal_set_buffer(enc, bwblob, wboff, 0)
    metal_set_buffer(enc, bwblob, wboff, 1)
    metal_set_buffer(enc, bx, 0ul, 2)
    metal_set_buffer(enc, by, yoff, 3)
    metal_set_buffer(enc, bk, 0ul, 4)
    metal_set_buffer(enc, bn, 0ul, 5)
    metal_dispatch_threadgroups(enc, uint3(uint(mp / 32l), uint(d / 64l), 1u), uint3(128u, 1u, 1u))
}

def private pf_fmt_at(a : array<KqFmt>; l : int64) : KqFmt => empty(a) ? KqFmt.q8 : a[l]

// one kq weight site on the prefill mul_mm path: the format's mul_mm twin over the whole-plane
// binds (kq_quants_of / kq_scales_of — k6 binds its split scale plane twice, sub-scales at soff
// + the f16 d tail at doff). Same grid/tgmem as enc_gemm_mm.
def private pf_enc_kq_site_mm(enc : MetalComputeEncoder?; t : Model; fmt : KqFmt; woff, rows : int64;
                              bx, by, bk, bn : MetalBuffer?; mp : int64; yoff : uint64 = 0ul) {
    let bq = kq_quants_of(g_pf_dev, t, fmt, woff)
    let bs = kq_scales_of(g_pf_dev, t, fmt, woff)
    if (fmt == KqFmt.k6) {
        metal_set_pipeline(enc, g_pso_kq_mm6)
        metal_set_threadgroup_memory_length(enc, metal_kq_mulmm_k6_msl_tgmem, 0)
        metal_set_buffer(enc, bs.buf, bs.doff, 0)
    } else {
        metal_set_pipeline(enc, fmt == KqFmt.k4 ? g_pso_kq_mm4 : g_pso_kq_mm5)
        metal_set_threadgroup_memory_length(enc, fmt == KqFmt.k4 ? metal_kq_mulmm_k4_msl_tgmem : metal_kq_mulmm_k5_msl_tgmem, 0)
        metal_set_buffer(enc, bs.buf, bs.soff, 0)
    }
    metal_set_buffer(enc, bs.buf, bs.soff, 1)
    metal_set_buffer(enc, bq.buf, bq.qoff, 2)
    metal_set_buffer(enc, bx, 0ul, 3)
    metal_set_buffer(enc, by, yoff, 4)
    metal_set_buffer(enc, bk, 0ul, 5)
    metal_set_buffer(enc, bn, 0ul, 6)
    metal_dispatch_threadgroups(enc, uint3(uint(mp / 32l), uint(rows / 64l), 1u), uint3(128u, 1u, 1u))
}

// the kq classifier GEMV (one f32 row against the [vocab x dim] kq plane) — the moved decode
// kernels' PSOs, prefill-owned twins
def private pf_enc_kq_gemv(enc : MetalComputeEncoder?; t : Model; fmt : KqFmt; woff, rows : int64;
                           bx : MetalBuffer?; xoff : uint64; by, bn, bd : MetalBuffer?) {
    let bq = kq_quants_of(g_pf_dev, t, fmt, woff)
    let bs = kq_scales_of(g_pf_dev, t, fmt, woff)
    if (fmt == KqFmt.k6) {
        metal_set_pipeline(enc, g_pf_pso_kq6)
        metal_set_buffer(enc, bs.buf, bs.doff, 0)
    } else {
        metal_set_pipeline(enc, fmt == KqFmt.k4 ? g_pf_pso_kq4 : g_pf_pso_kq5)
        metal_set_buffer(enc, bs.buf, bs.soff, 0)
    }
    metal_set_buffer(enc, bs.buf, bs.soff, 1)
    metal_set_buffer(enc, bq.buf, bq.qoff, 2)
    metal_set_buffer(enc, bx, xoff, 3)
    metal_set_buffer(enc, by, 0ul, 4)
    metal_set_buffer(enc, bn, 0ul, 5)
    metal_set_buffer(enc, bd, 0ul, 6)
    metal_dispatch_threadgroups(enc, uint3(uint((rows + 3l) / 4l), 1u, 1u), uint3(64u, 1u, 1u))
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

// the classifier GEMV: d rows of logits, 4 rows (one simdgroup each) per threadgroup — the
// blob slice binds through both views at the classifier's byte offset
def private pf_enc_gemv(enc : MetalComputeEncoder?; bw : MetalBuffer?; boff : uint64; bx, by, bn, bd : MetalBuffer?; d : int64) {
    metal_set_pipeline(enc, g_pf_pso_gemv)
    metal_set_buffer(enc, bw, boff, 0)   // half-scale view
    metal_set_buffer(enc, bw, boff, 1)   // byte view
    metal_set_buffer(enc, bx, 0ul, 2)
    metal_set_buffer(enc, by, 0ul, 3)
    metal_set_buffer(enc, bn, 0ul, 4)
    metal_set_buffer(enc, bd, 0ul, 5)
    metal_dispatch_threadgroups(enc, uint3(uint((d + 3l) / 4l), 1u, 1u), uint3(128u, 1u, 1u))
}

// ===== MoE prefill encoders (Wave C stage 2b) =====

// per-position router + select + the atomics-free CSR (count -> prefix -> bucket) for layer l
def private pf_enc_moe_route(enc : MetalComputeEncoder?; t : Model; l : int64;
                             bx, blg, bsel, bcnt, bbase, bbkt, binv, bdim, bne, bnk, bgm, brb, bhasb, btot, bnp : MetalBuffer?; npos : int64) {
    let c = t.config
    var bw_rt = pf_upload_region(unsafe(addr < void? >(t.fblob[t.router_off + l * c.n_expert * c.dim])), uint64(c.n_expert * c.dim * 4l))
    metal_set_pipeline(enc, g_pf_pso_moe_router_b)
    metal_set_buffer(enc, bw_rt, 0ul, 0)
    metal_set_buffer(enc, bx, 0ul, 1)
    metal_set_buffer(enc, blg, 0ul, 2)
    metal_set_buffer(enc, bdim, 0ul, 3)
    metal_set_buffer(enc, bne, 0ul, 4)
    metal_set_buffer(enc, bdim, 0ul, 5)   // per-position x stride = dim
    metal_set_buffer(enc, brb, 0ul, 6)
    metal_set_buffer(enc, bhasb, 0ul, 7)
    metal_set_buffer(enc, bnp, 0ul, 8)
    metal_dispatch_threadgroups(enc, uint3(uint((c.n_expert + 3l) / 4l), uint((npos + 7l) / 8l), 1u), uint3(128u, 1u, 1u))
    return if (g_pf_skip == "moe_only_router")   // attribution: router GEMV alone, no select/CSR
    metal_set_pipeline(enc, g_pf_pso_moe_select)
    metal_set_buffer(enc, blg, 0ul, 0)
    metal_set_buffer(enc, bsel, 0ul, 1)
    metal_set_buffer(enc, bsel, 0ul, 2)
    metal_set_buffer(enc, bne, 0ul, 3)
    metal_set_buffer(enc, bnk, 0ul, 4)
    metal_set_buffer(enc, bgm, 0ul, 5)
    metal_dispatch_threadgroups(enc, uint3(uint(npos), 1u, 1u), uint3(32u, 1u, 1u))
    return if (g_pf_skip == "moe_only_sel")   // attribution: router + select, no CSR
    metal_set_pipeline(enc, g_pf_pso_moe_count)
    metal_set_buffer(enc, bsel, 0ul, 0)
    metal_set_buffer(enc, bcnt, 0ul, 1)
    metal_set_buffer(enc, btot, 0ul, 2)
    metal_set_buffer(enc, bnk, 0ul, 3)
    metal_dispatch_threadgroups(enc, uint3(uint(c.n_expert), 1u, 1u), uint3(32u, 1u, 1u))
    metal_set_pipeline(enc, g_pf_pso_moe_prefix)
    metal_set_buffer(enc, bcnt, 0ul, 0)
    metal_set_buffer(enc, bbase, 0ul, 1)
    metal_set_buffer(enc, bne, 0ul, 2)
    metal_dispatch_threadgroups(enc, uint3(1u, 1u, 1u), uint3(32u, 1u, 1u))
    metal_set_pipeline(enc, g_pf_pso_moe_bucket)
    metal_set_buffer(enc, bsel, 0ul, 0)
    metal_set_buffer(enc, bbase, 0ul, 1)
    metal_set_buffer(enc, bbkt, 0ul, 2)
    metal_set_buffer(enc, binv, 0ul, 3)
    metal_set_buffer(enc, btot, 0ul, 4)
    metal_set_buffer(enc, bnk, 0ul, 5)
    metal_dispatch_threadgroups(enc, uint3(uint(c.n_expert), 1u, 1u), uint3(32u, 1u, 1u))
}

// one gathered expert mul_mm site: grid (ceil(npos/32) tiles, rows/64, ne experts); the twin
// exits tiles past the expert's padded count. bgather = 1 (bkt-indirect X) / 0 (bucket rows).
def private pf_enc_moe_mm(enc : MetalComputeEncoder?; t : Model; fmt : KqFmt; stack_off, rows, npos : int64;
                          bx, by, bkdim, bndim, bcnt, bbase, bbkt, bes, bnk, bgather : MetalBuffer?) {
    let tiles = (npos + 31l) / 32l
    let ne = t.config.n_expert
    if (fmt == KqFmt.q8) {
        let bw = pf_blob_of(t, stack_off)
        metal_set_pipeline(enc, g_pf_pso_moe_mm_q8)
        metal_set_threadgroup_memory_length(enc, metal_moe_mulmm_q8_msl_tgmem, 0)
        metal_set_buffer(enc, bw.buf, bw.boff, 0)
        metal_set_buffer(enc, bw.buf, bw.boff, 1)
        metal_set_buffer(enc, bx, 0ul, 2)
        metal_set_buffer(enc, by, 0ul, 3)
        metal_set_buffer(enc, bkdim, 0ul, 4)
        metal_set_buffer(enc, bndim, 0ul, 5)
        metal_set_buffer(enc, bcnt, 0ul, 6)
        metal_set_buffer(enc, bbase, 0ul, 7)
        metal_set_buffer(enc, bbkt, 0ul, 8)
        metal_set_buffer(enc, bes, 0ul, 9)
        metal_set_buffer(enc, bnk, 0ul, 10)
        metal_set_buffer(enc, bgather, 0ul, 11)
        metal_dispatch_threadgroups(enc, uint3(uint(tiles), uint(rows / 64l), uint(ne)), uint3(128u, 1u, 1u))
    } elif (fmt == KqFmt.q51) {
        let qp = q51_planes_of(g_pf_dev, t, stack_off)
        metal_set_pipeline(enc, g_pf_pso_moe_mm_q51)
        metal_set_threadgroup_memory_length(enc, metal_moe_mulmm_q51_msl_tgmem, 0)
        metal_set_buffer(enc, qp.sbuf, qp.soff, 0)
        metal_set_buffer(enc, qp.qbuf, qp.qoff, 1)
        metal_set_buffer(enc, bx, 0ul, 2)
        metal_set_buffer(enc, by, 0ul, 3)
        metal_set_buffer(enc, bkdim, 0ul, 4)
        metal_set_buffer(enc, bndim, 0ul, 5)
        metal_set_buffer(enc, bcnt, 0ul, 6)
        metal_set_buffer(enc, bbase, 0ul, 7)
        metal_set_buffer(enc, bbkt, 0ul, 8)
        metal_set_buffer(enc, bes, 0ul, 9)
        metal_set_buffer(enc, bnk, 0ul, 10)
        metal_set_buffer(enc, bgather, 0ul, 11)
        metal_dispatch_threadgroups(enc, uint3(uint(tiles), uint(rows / 64l), uint(ne)), uint3(128u, 1u, 1u))
    } else {
        let bq = kq_quants_of(g_pf_dev, t, fmt, stack_off)
        let bs = kq_scales_of(g_pf_dev, t, fmt, stack_off)
        if (fmt == KqFmt.k6) {
            metal_set_pipeline(enc, g_pf_pso_moe_mm_k6)
            metal_set_threadgroup_memory_length(enc, metal_moe_mulmm_k6_msl_tgmem, 0)
            metal_set_buffer(enc, bs.buf, bs.doff, 0)
        } elif (fmt == KqFmt.k4) {
            metal_set_pipeline(enc, g_pf_pso_moe_mm_k4)
            metal_set_threadgroup_memory_length(enc, metal_moe_mulmm_k4_msl_tgmem, 0)
            metal_set_buffer(enc, bs.buf, bs.soff, 0)
        } else {
            metal_set_pipeline(enc, g_pf_pso_moe_mm_k5)
            metal_set_threadgroup_memory_length(enc, metal_moe_mulmm_k5_msl_tgmem, 0)
            metal_set_buffer(enc, bs.buf, bs.soff, 0)
        }
        metal_set_buffer(enc, bs.buf, bs.soff, 1)
        metal_set_buffer(enc, bq.buf, bq.qoff, 2)
        metal_set_buffer(enc, bx, 0ul, 3)
        metal_set_buffer(enc, by, 0ul, 4)
        metal_set_buffer(enc, bkdim, 0ul, 5)
        metal_set_buffer(enc, bndim, 0ul, 6)
        metal_set_buffer(enc, bcnt, 0ul, 7)
        metal_set_buffer(enc, bbase, 0ul, 8)
        metal_set_buffer(enc, bbkt, 0ul, 9)
        metal_set_buffer(enc, bes, 0ul, 10)
        metal_set_buffer(enc, bnk, 0ul, 11)
        metal_set_buffer(enc, bgather, 0ul, 12)
        metal_dispatch_threadgroups(enc, uint3(uint(tiles), uint(rows / 64l), uint(ne)), uint3(128u, 1u, 1u))
    }
}

// gemma4 router-input norm rows (weightless rms * 1/sqrt(dim) * rscale, one tg per position)
// the MXFP4 gathered expert mul_mm (gpt-oss)
def private pf_enc_moe_mm_mx4(enc : MetalComputeEncoder?; t : Model; stack_off, rows, npos : int64;
                              bx, by, bkdim, bndim, bcnt, bbase, bbkt, beblk, bnk, bgather : MetalBuffer?) {
    let tiles = (npos + 31l) / 32l
    let ne = t.config.n_expert
    let mp = mx4_of(g_pf_dev, t, stack_off)
    metal_set_pipeline(enc, g_pf_pso_moe_mm_mx4)
    metal_set_threadgroup_memory_length(enc, metal_moe_mulmm_mx4_msl_tgmem, 0)
    metal_set_buffer(enc, mp.qbuf, mp.qoff, 0)
    metal_set_buffer(enc, mp.sbuf, mp.soff, 1)
    metal_set_buffer(enc, bx, 0ul, 2)
    metal_set_buffer(enc, by, 0ul, 3)
    metal_set_buffer(enc, bkdim, 0ul, 4)
    metal_set_buffer(enc, bndim, 0ul, 5)
    metal_set_buffer(enc, bcnt, 0ul, 6)
    metal_set_buffer(enc, bbase, 0ul, 7)
    metal_set_buffer(enc, bbkt, 0ul, 8)
    metal_set_buffer(enc, beblk, 0ul, 9)
    metal_set_buffer(enc, bnk, 0ul, 10)
    metal_set_buffer(enc, bgather, 0ul, 11)
    metal_set_buffer(enc, g_pf_mx4_vtab, 0ul, 13)
    metal_dispatch_threadgroups(enc, uint3(uint(tiles), uint(rows / 64l), uint(ne)), uint3(128u, 1u, 1u))
}

// the per-expert bias over the padded bucket rows (ggml_add_id order — before the reduce)
def private pf_enc_moe_bias_rows(enc : MetalComputeEncoder?; by, bwb, bcnt, bbase, bd : MetalBuffer?; d, npos, ne : int64) {
    metal_set_pipeline(enc, g_pf_pso_moe_bias_rows)
    metal_set_buffer(enc, by, 0ul, 0)
    metal_set_buffer(enc, bwb, 0ul, 1)
    metal_set_buffer(enc, bcnt, 0ul, 2)
    metal_set_buffer(enc, bbase, 0ul, 3)
    metal_set_buffer(enc, bd, 0ul, 4)
    metal_dispatch_threadgroups(enc, uint3(uint((d + 255l) / 256l), uint(npos), uint(ne)), uint3(256u, 1u, 1u))
}

def private pf_enc_g4_router_norm(enc : MetalComputeEncoder?; bx, brs, by, bdim, beps, binvd : MetalBuffer?; npos : int64) {
    metal_set_pipeline(enc, g_pf_pso_g4rnorm)
    metal_set_threadgroup_memory_length(enc, metal_g4_router_norm_msl_tgmem, 0)
    metal_set_buffer(enc, bx, 0ul, 0)
    metal_set_buffer(enc, brs, 0ul, 1)
    metal_set_buffer(enc, by, 0ul, 2)
    metal_set_buffer(enc, bdim, 0ul, 3)
    metal_set_buffer(enc, beps, 0ul, 4)
    metal_set_buffer(enc, binvd, 0ul, 5)
    metal_dispatch_threadgroups(enc, uint3(uint(npos), 1u, 1u), uint3(256u, 1u, 1u))
}

// gemma4 per-expert down-scale fold over the selection rows
def private pf_enc_moe_wscale(enc : MetalComputeEncoder?; bsel, bds, bnk : MetalBuffer?; npos : int64) {
    metal_set_pipeline(enc, g_pf_pso_moe_wscale)
    metal_set_buffer(enc, bsel, 0ul, 0)
    metal_set_buffer(enc, bsel, 0ul, 1)
    metal_set_buffer(enc, bds, 0ul, 2)
    metal_set_buffer(enc, bnk, 0ul, 3)
    metal_dispatch_threadgroups(enc, uint3(uint(npos), 1u, 1u), uint3(32u, 1u, 1u))
}

def private pf_enc_moe_reduce(enc : MetalComputeEncoder?; bdn, bsel, binv, by, bdim, bnk : MetalBuffer?; dim, npos : int64) {
    metal_set_pipeline(enc, g_pf_pso_moe_reduce)
    metal_set_buffer(enc, bdn, 0ul, 0)
    metal_set_buffer(enc, bsel, 0ul, 1)   // the float w view — row layout in-kernel
    metal_set_buffer(enc, binv, 0ul, 2)
    metal_set_buffer(enc, by, 0ul, 3)
    metal_set_buffer(enc, bdim, 0ul, 4)
    metal_set_buffer(enc, bnk, 0ul, 5)
    metal_dispatch_threadgroups(enc, uint3(uint((dim + 255l) / 256l), uint(npos), 1u), uint3(256u, 1u, 1u))
}

def private enc_add_bias_rows(enc : MetalComputeEncoder?; bx, bb, bn, btotal : MetalBuffer?; total : int64; off : uint64 = 0ul) {
    metal_set_pipeline(enc, g_pso_addbias)
    metal_set_buffer(enc, bx, off, 0)
    metal_set_buffer(enc, bb, 0ul, 1)
    metal_set_buffer(enc, bn, 0ul, 2)
    metal_set_buffer(enc, btotal, 0ul, 3)
    metal_dispatch_threadgroups(enc, uint3(uint((total + 63l) / 64l), 1u, 1u), uint3(64u, 1u, 1u))
}

def private enc_qk_norm_pf(enc : MetalComputeEncoder?; bx, bw, bnq, bkoffe, brstride, bhs, bwoffe, beps, bsoffe : MetalBuffer?; ntg, nrows, hs : int64; off : uint64 = 0ul) {
    metal_set_pipeline(enc, g_pf_pso_qknorm)
    metal_set_threadgroup_memory_length(enc, metal_qk_norm_msl_tgmem, 0)
    metal_set_buffer(enc, bx, off, 0)
    metal_set_buffer(enc, bw, 0ul, 1)
    metal_set_buffer(enc, bnq, 0ul, 2)
    metal_set_buffer(enc, bkoffe, 0ul, 3)
    metal_set_buffer(enc, brstride, 0ul, 4)
    metal_set_buffer(enc, bhs, 0ul, 5)
    metal_set_buffer(enc, bwoffe, 0ul, 6)
    metal_set_buffer(enc, beps, 0ul, 7)
    metal_set_buffer(enc, bsoffe, 0ul, 8)
    metal_dispatch_threadgroups(enc, uint3(uint(ntg), uint(nrows), 1u), uint3(uint(hs), 1u, 1u))
}

def private enc_rope(enc : MetalComputeEncoder?; bv, bcos, bsin, bn, bhs, bnpairs, bneox : MetalBuffer?; npairs : int64; off : uint64 = 0ul; brot : MetalBuffer? = null) {
    metal_set_pipeline(enc, g_pso_rope)
    metal_set_buffer(enc, bv, off, 0)
    metal_set_buffer(enc, bcos, 0ul, 1)
    metal_set_buffer(enc, bsin, 0ul, 2)
    metal_set_buffer(enc, bn, 0ul, 3)
    metal_set_buffer(enc, bhs, 0ul, 4)
    metal_set_buffer(enc, bnpairs, 0ul, 5)
    metal_set_buffer(enc, bneox, 0ul, 6)
    metal_set_buffer(enc, brot != null ? brot : bhs, 0ul, 7)   // rot defaults to head_size (full rope)
    metal_dispatch_threadgroups(enc, uint3(uint((npairs + 63l) / 64l), 1u, 1u), uint3(64u, 1u, 1u))
}




// the ggml-geometry attention pair: same buffer lists as the trio, 32x64 C tiles, z = heads.
// The key grid spans nk64 (= np32's value — existing rows + the chunk on a continuation);
// bqoff/bqrows carry the causal shift and the score slab's row extent.
def private enc_qk_mm(enc : MetalComputeEncoder?; bq, bk, batt, bqd, bkvd, bhs, bkvm, bnp, bnp32, bscale, bqoff, bqrows, bwin : MetalBuffer?; mp, nk64, heads : int64) {
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
    metal_set_buffer(enc, bwin, 0ul, 12)
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

def private enc_qk(enc : MetalComputeEncoder?; bq, bk, batt, bqd, bkvd, bhs, bkvm, bnp, bnp32, bscale, bqoff, bqrows, bwin : MetalBuffer?; mp, nk64, heads : int64) {
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
    metal_set_buffer(enc, bwin, 0ul, 12)
    metal_dispatch_threadgroups(enc, uint3(uint(mp / 32l), uint(nk64 / 32l), uint(heads)), uint3(128u, 1u, 1u))
}

def private enc_softmax(enc : MetalComputeEncoder?; batt, bnp32, bqoff, bqrows, bwin, bcap : MetalBuffer?; npos, heads : int64; bsink : MetalBuffer? = null; bhass : MetalBuffer? = null) {
    metal_set_pipeline(enc, g_pso_softmax)
    metal_set_threadgroup_memory_length(enc, metal_attn_softmax_msl_tgmem, 0)
    metal_set_buffer(enc, batt, 0ul, 0)
    metal_set_buffer(enc, bnp32, 0ul, 1)
    metal_set_buffer(enc, bqoff, 0ul, 2)
    metal_set_buffer(enc, bqrows, 0ul, 3)
    metal_set_buffer(enc, bwin, 0ul, 4)
    metal_set_buffer(enc, bcap, 0ul, 5)
    metal_set_buffer(enc, bsink != null ? bsink : g_pf_one, 0ul, 20)
    metal_set_buffer(enc, bhass != null ? bhass : g_pf_zero, 0ul, 21)
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
// ===== deltanet prefill encoders (Wave D) — the prefill-local twins of the decode-side set =====
// hist/state bind the per-session DnMirror arena at the layer's byte offset; the rest is window scratch.

def private pf_enc_dn_conv(enc : MetalComputeEncoder?; bx, bhist : MetalBuffer?; hoff : uint64;
                           bw, by, bcd, bdconv, bnp : MetalBuffer?; cd, npos : int64) {
    metal_set_pipeline(enc, g_pf_pso_dn_conv)
    metal_set_buffer(enc, bx, 0ul, 0)
    metal_set_buffer(enc, bhist, hoff, 1)
    metal_set_buffer(enc, bw, 0ul, 2)
    metal_set_buffer(enc, by, 0ul, 3)
    metal_set_buffer(enc, bcd, 0ul, 4)
    metal_set_buffer(enc, bdconv, 0ul, 5)
    metal_set_buffer(enc, bnp, 0ul, 6)
    metal_dispatch_threadgroups(enc, uint3(uint((cd + 255l) / 256l), uint(npos), 1u), uint3(256u, 1u, 1u))
}

def private pf_enc_dn_conv_hist(enc : MetalComputeEncoder?; bx, bhist : MetalBuffer?; hoff : uint64;
                                bcd, bdconv, bnp : MetalBuffer?; cd : int64; bmid : MetalBuffer?) {
    metal_set_pipeline(enc, g_pf_pso_dn_hist)
    metal_set_buffer(enc, bx, 0ul, 0)
    metal_set_buffer(enc, bhist, hoff, 1)
    metal_set_buffer(enc, bcd, 0ul, 2)
    metal_set_buffer(enc, bdconv, 0ul, 3)
    metal_set_buffer(enc, bnp, 0ul, 4)
    metal_set_buffer(enc, bhist, hoff, 5)
    metal_set_buffer(enc, bmid, 0ul, 6)
    metal_dispatch_threadgroups(enc, uint3(uint((cd + 255l) / 256l), 1u, 1u), uint3(256u, 1u, 1u))
}

def private pf_enc_dn_l2norm(enc : MetalComputeEncoder?; by, bcd, bkd, bds, beps, bqscale : MetalBuffer?; nblk, npos : int64) {
    metal_set_pipeline(enc, g_pf_pso_dn_l2)
    metal_set_buffer(enc, by, 0ul, 0)
    metal_set_buffer(enc, bcd, 0ul, 1)
    metal_set_buffer(enc, bkd, 0ul, 2)
    metal_set_buffer(enc, bds, 0ul, 3)
    metal_set_buffer(enc, beps, 0ul, 4)
    metal_set_buffer(enc, bqscale, 0ul, 5)
    metal_dispatch_threadgroups(enc, uint3(uint((nblk + 7l) / 8l), uint(npos), 1u), uint3(256u, 1u, 1u))
}

def private pf_enc_dn_scan(enc : MetalComputeEncoder?; bcv, bst : MetalBuffer?; stoff : uint64;
                           bbraw, bgraw, baa, bdtb, bo, bcd, bkd, bds, bnvh, bnkh, bdi, bnp : MetalBuffer?; ds, nvh : int64;
                           bmid : MetalBuffer?) {
    metal_set_pipeline(enc, g_pf_pso_dn_scan)
    metal_set_buffer(enc, bcv, 0ul, 0)
    metal_set_buffer(enc, bst, stoff, 1)
    metal_set_buffer(enc, bbraw, 0ul, 2)
    metal_set_buffer(enc, bgraw, 0ul, 3)
    metal_set_buffer(enc, baa, 0ul, 4)
    metal_set_buffer(enc, bdtb, 0ul, 5)
    metal_set_buffer(enc, bo, 0ul, 6)
    metal_set_buffer(enc, bcd, 0ul, 7)
    metal_set_buffer(enc, bkd, 0ul, 8)
    metal_set_buffer(enc, bds, 0ul, 9)
    metal_set_buffer(enc, bnvh, 0ul, 10)
    metal_set_buffer(enc, bnkh, 0ul, 11)
    metal_set_buffer(enc, bdi, 0ul, 12)
    metal_set_buffer(enc, bnp, 0ul, 13)
    metal_set_buffer(enc, bst, stoff, 14)
    metal_set_buffer(enc, bmid, 0ul, 15)
    metal_dispatch_threadgroups(enc, uint3(uint(ds / 4l), uint(nvh), 1u), uint3(128u, 1u, 1u))
}

def private pf_enc_dn_gate(enc : MetalComputeEncoder?; bo, bz, bwn, bds, bdi, beps : MetalBuffer?; nvh, npos : int64) {
    metal_set_pipeline(enc, g_pf_pso_dn_gate)
    metal_set_buffer(enc, bo, 0ul, 0)
    metal_set_buffer(enc, bz, 0ul, 1)
    metal_set_buffer(enc, bwn, 0ul, 2)
    metal_set_buffer(enc, bds, 0ul, 3)
    metal_set_buffer(enc, bdi, 0ul, 4)
    metal_set_buffer(enc, beps, 0ul, 5)
    metal_dispatch_threadgroups(enc, uint3(uint((nvh + 7l) / 8l), uint(npos), 1u), uint3(256u, 1u, 1u))
}

def private pf_enc_dn_deint(enc : MetalComputeEncoder?; bqg, bq, bgt, bhs, bqd, bqs : MetalBuffer?; qd, npos : int64) {
    metal_set_pipeline(enc, g_pf_pso_dn_deint)
    metal_set_buffer(enc, bqg, 0ul, 0)
    metal_set_buffer(enc, bq, 0ul, 1)
    metal_set_buffer(enc, bgt, 0ul, 2)
    metal_set_buffer(enc, bhs, 0ul, 3)
    metal_set_buffer(enc, bqd, 0ul, 4)
    metal_set_buffer(enc, bqs, 0ul, 5)
    metal_dispatch_threadgroups(enc, uint3(uint((qd + 255l) / 256l), uint(npos), 1u), uint3(256u, 1u, 1u))
}

def private pf_enc_dn_ba(enc : MetalComputeEncoder?; bw : MetalBuffer?; boff : uint64; bx, by, bn, bd : MetalBuffer?; d, npos : int64) {
    metal_set_pipeline(enc, g_pf_pso_dn_ba)
    metal_set_buffer(enc, bw, boff, 0)   // half-scale view
    metal_set_buffer(enc, bw, boff, 1)   // byte view — the same blob slice
    metal_set_buffer(enc, bx, 0ul, 2)
    metal_set_buffer(enc, by, 0ul, 3)
    metal_set_buffer(enc, bn, 0ul, 4)
    metal_set_buffer(enc, bd, 0ul, 5)
    metal_dispatch_threadgroups(enc, uint3(uint((d + 7l) / 8l), uint(npos), 1u), uint3(256u, 1u, 1u))
}

// batched f32-slab GEMV riding MetalMoeRouterB: d rows per position (dn_ba_f32 β/α d = dt_rank;
// the shexp gate d = 1); y[p*d + row] — the pf_enc_dn_ba layout
def private pf_enc_slab_gemv_b(enc : MetalComputeEncoder?; bw, bx, by, bn, bd, bnp : MetalBuffer?; d, npos : int64) {
    metal_set_pipeline(enc, g_pf_pso_moe_router_b)
    metal_set_buffer(enc, bw, 0ul, 0)
    metal_set_buffer(enc, bx, 0ul, 1)
    metal_set_buffer(enc, by, 0ul, 2)
    metal_set_buffer(enc, bn, 0ul, 3)
    metal_set_buffer(enc, bd, 0ul, 4)
    metal_set_buffer(enc, bn, 0ul, 5)   // per-position x stride = dim
    metal_set_buffer(enc, g_pf_one, 0ul, 6)
    metal_set_buffer(enc, g_pf_zero, 0ul, 7)
    metal_set_buffer(enc, bnp, 0ul, 8)
    metal_dispatch_threadgroups(enc, uint3(uint((d + 3l) / 4l), uint((npos + 7l) / 8l), 1u), uint3(128u, 1u, 1u))
}

def private pf_enc_axpy_sig(enc : MetalComputeEncoder?; by, bx, bg, btot, bdim : MetalBuffer?; total : int64) {
    metal_set_pipeline(enc, g_pf_pso_axpy_sig)
    metal_set_buffer(enc, by, 0ul, 0)
    metal_set_buffer(enc, bx, 0ul, 1)
    metal_set_buffer(enc, bg, 0ul, 2)
    metal_set_buffer(enc, btot, 0ul, 3)
    metal_set_buffer(enc, bdim, 0ul, 4)
    metal_dispatch_threadgroups(enc, uint3(uint((total + 255l) / 256l), 1u, 1u), uint3(256u, 1u, 1u))
}

def private pf_enc_ew2(enc : MetalComputeEncoder?; pso : MetalComputePipeline?; ba, bb, btot : MetalBuffer?; total : int64; bascale : MetalBuffer? = null; boff : uint64 = 0ul) {
    metal_set_pipeline(enc, pso)
    metal_set_buffer(enc, ba, 0ul, 0)
    metal_set_buffer(enc, bb, boff, 1)
    metal_set_buffer(enc, btot, 0ul, 2)
    metal_set_buffer(enc, bascale != null ? bascale : g_pf_one, 0ul, 3)   // the add's ascale slot (unused by swiglu/geglu)
    metal_dispatch_threadgroups(enc, uint3(uint((total + 63l) / 64l), 1u, 1u), uint3(64u, 1u, 1u))
}

// the float4-lane twin (swiglu_oai): grid over total/4 threads — total % 4 == 0 by the callers'
// 32-row-padded panel contract
def private pf_enc_ew2_v4(enc : MetalComputeEncoder?; pso : MetalComputePipeline?; ba, bb, btot : MetalBuffer?; total : int64) {
    metal_set_pipeline(enc, pso)
    metal_set_buffer(enc, ba, 0ul, 0)
    metal_set_buffer(enc, bb, 0ul, 1)
    metal_set_buffer(enc, btot, 0ul, 2)
    metal_dispatch_threadgroups(enc, uint3(uint((total / 4l + 63l) / 64l), 1u, 1u), uint3(64u, 1u, 1u))
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

// GPU-PLE gate (register_ple_gpu_gate): true iff THIS window's metal prefill will engage AND
// the pre-step kernels cover the model — q8 emb table, kept-bf16 model_proj, slice/shape gates.
def private metal_ple_pre_gpu_gate(t : Model; s : Session; npos, start_pos : int64) : bool {
    if (active_prefill_override() != "metal" ||
            !t.metal_blob || t.ple_emb_fmt != KqFmt.q8 || !t.ple_model_bf16) {
        return false
    }
    let ple = t.config.n_embd_per_layer
    if (ple % 32l != 0l || ple > 1024l || (t.config.n_layers * ple) % 64l != 0l ||
            prefill_decline(t, s, npos, start_pos) != MetalPrefillDecline.none) {
        return false
    }
    return metal_prefill_init()
}

def metal_prefill_forward(t : Model; var s : Session; npos, start_pos : int64) : bool {
    pf_weight_caches_flush_on_reload()
    var why = prefill_decline(t, s, npos, start_pos)
    if (why == MetalPrefillDecline.none && !metal_prefill_init()) {
        why = MetalPrefillDecline.device
    }
    if (why != MetalPrefillDecline.none) {
        if (s.ple_gpu_pending) {   // the gate promised a device pre-step — build it on CPU first
            ple_pre_prefill(t, s, s.ple_gpu_tokens, npos)
            s.ple_gpu_pending = false
        }
        if (why == MetalPrefillDecline.feature) {
            decline(why, metal_missing_needs(t, PF_NEEDS_OK))
        } else {
            decline(why)
        }
        return false
    }
    let ts_all = ref_time_ticks()
    let c = t.config
    let dim = c.dim
    let head_size = c.head_size
    let n_heads = c.n_heads
    let qd = n_heads * head_size
    let kv_dim = c.n_kv_heads * head_size
    let hidden = layer_hidden(t, 0l)
    let kv_mul = n_heads / c.n_kv_heads
    let scale = c.attn_scale > 0.0 ? c.attn_scale : 1.0 / sqrt(float(head_size))
    let half = (c.rope_dim > 0l ? c.rope_dim : head_size) / 2l   // partial rope: table rows are rope_dim/2 wide
    // deltanet (Wave D): the recurrent-state mirror must be resident/synced BEFORE encoding —
    // prepare zeroes at pos 0 or syncs a CPU prefix; a cold mid-stream window has no repair path
    let dn = c.recr_mask != 0ul
    var bdn_state : MetalBuffer?
    var dn_state_base = 0ul
    var dn_conv_base = 0ul
    if (dn) {
        if (!metal_common_init()) {   // the mirror allocates on the shared decode-side device
            decline(MetalPrefillDecline.device)
            return false
        }
        var dm = dn_mirror_prepare(s, start_pos, dn_mirror_regions(t))
        if (!dm.ok) {
            decline(MetalPrefillDecline.dn_state)
            return false
        }
        bdn_state = dm.buf
        dn_state_base = dm.state_off
        dn_conv_base = dm.conv_off
    }
    // the two-class attention geometry (gemma4): the config fields describe the GLOBAL class,
    // the _sw twins the SLIDING class (the loader guarantees at most two) — buffers size to the
    // class maxima and each layer binds its class's uniform set
    let hs_sw = c.head_size_swa > 0l ? c.head_size_swa : head_size
    let kvh_sw = c.n_kv_heads_swa > 0l ? c.n_kv_heads_swa : c.n_kv_heads
    let qd_sw = n_heads * hs_sw
    let kvd_sw = kvh_sw * hs_sw
    let hetero = hs_sw != head_size || kvh_sw != c.n_kv_heads
    let qd_mx = max(qd, qd_sw)
    let half_sw = hs_sw / 2l
    let mp = ((npos + 63l) / 64l) * 64l    // 64 = the big GEMM kernel's M tile; attention kernels only assume %32
    // a continuation chunk (M2.5) attends the session's existing rows too: the K/V panels hold
    // [0, start_pos) gathered rows + the chunk at the start_pos offset, keys pad to the QK key
    // grid's 64-tile, and the score slabs widen to nk64 columns (rows stay the mp queries)
    let nk = start_pos + npos
    let nk64 = ((nk + 63l) / 64l) * 64l
    // the K/V GEMMs write mp FULL M-tile rows at the start_pos row offset — a continuation's
    // panel must hold [0, start_pos + mp) rows or the pad rows write PAST the pool buffer
    // (silent neighbor corruption; whether it bites depends on pool layout)
    let kvrows = max(nk64, start_pos + mp)
    // pooled per-prefill buffers (activations sized mp rows — the GEMM has no edge masking; rows
    // past npos are never read by the norm/rope/attention/elementwise kernels, and stale bytes in
    // the requant image only ever land in pad output rows: C-block rows are independent)
    let bytes_x = uint64(mp * dim * 4l)
    let bytes_xb = uint64(mp * max(dim, qd_mx) * 4l)   // dim-wide norm outs AND the qd-wide AV out
    let bytes_q = uint64(mp * qd_mx * 4l)
    let nsh = !t.blocks.ffn_is_dense ? c.n_ff_shexp : 0l   // shared expert (qwen35moe) rides bhb/bhb2
    let bytes_h = uint64(mp * max(hidden, nsh) * 4l)
    let bytes_att = uint64(n_heads * mp * nk64 * 4l)   // per-head score slabs: mp queries x nk64 keys
    let bytes_tab = uint64(npos * half * 4l)
    let bytes_tab_sw = uint64(npos * half_sw * 4l)   // the swa pair's row width is ITS class's hs/2
    var bx = pool_acquire(g_pf_pool, g_pf_dev, bytes_x)
    var bxb = pool_acquire(g_pf_pool, g_pf_dev, bytes_xb)
    var bxb2 = pool_acquire(g_pf_pool, g_pf_dev, bytes_x)
    var bq = pool_acquire(g_pf_pool, g_pf_dev, bytes_q)
    var bhb = pool_acquire(g_pf_pool, g_pf_dev, bytes_h)
    var bhb2 = pool_acquire(g_pf_pool, g_pf_dev, bytes_h)
    // MoE (Wave C stage 2b): per-position routing + padded bucket panels (32-row tiles/expert);
    // g4moe adds the router-input/acc panel (bxb2 doubles as the pre_ffn2 expert input — the
    // serial encoder makes the reuse safe)
    let moe = !t.blocks.ffn_is_dense
    let g4moe = moe && c.moe_dense_shexp
    let mtot = moe ? npos * c.n_expert_used : 0l
    let mpad = moe ? ((mtot + c.n_expert * 31l + 31l) / 32l) * 32l : 0l
    let bytes_mlg = uint64(npos * c.n_expert * 4l)
    let bytes_msel = uint64(mtot * 2l * 4l)
    let bytes_mcsr = uint64(c.n_expert * 4l)
    let bytes_mbkt = uint64(mpad * 4l)
    let bytes_minv = uint64(mtot * 4l)
    let bytes_mg = uint64(mpad * c.n_ff_exp * 4l)
    let bytes_mdn = uint64(mpad * dim * 4l)
    var bmlg = moe ? pool_acquire(g_pf_pool, g_pf_dev, bytes_mlg) : null
    var bmsel = moe ? pool_acquire(g_pf_pool, g_pf_dev, bytes_msel) : null
    var bmcnt = moe ? pool_acquire(g_pf_pool, g_pf_dev, bytes_mcsr) : null
    var bmbase = moe ? pool_acquire(g_pf_pool, g_pf_dev, bytes_mcsr) : null
    var bmbkt = moe ? pool_acquire(g_pf_pool, g_pf_dev, bytes_mbkt) : null
    var bminv = moe ? pool_acquire(g_pf_pool, g_pf_dev, bytes_minv) : null
    var bmg = moe ? pool_acquire(g_pf_pool, g_pf_dev, bytes_mg) : null
    var bmg2 = moe ? pool_acquire(g_pf_pool, g_pf_dev, bytes_mg) : null
    var bmdn = moe ? pool_acquire(g_pf_pool, g_pf_dev, bytes_mdn) : null
    var bmg4rt = g4moe ? pool_acquire(g_pf_pool, g_pf_dev, bytes_x) : null
    // deltanet scratch (recurrent layers) — GEMM outputs pad to mp rows like every panel
    let cd_dn = dn ? dn_conv_dim(c) : 0l
    let di_dn = dn ? c.ssm_d_inner : 0l
    let nvh_dn = dn ? c.ssm_dt_rank : 0l
    let bytes_dn_cd = uint64(mp * cd_dn * 4l)
    let bytes_dn_di = uint64(mp * di_dn * 4l)
    let bytes_dn_h = uint64(mp * nvh_dn * 4l)
    let bytes_dn_qg = uint64(mp * 2l * qd * 4l)
    var bdnqkv = dn ? pool_acquire(g_pf_pool, g_pf_dev, bytes_dn_cd) : null
    var bdncv = dn ? pool_acquire(g_pf_pool, g_pf_dev, bytes_dn_cd) : null
    var bdnz = dn ? pool_acquire(g_pf_pool, g_pf_dev, bytes_dn_di) : null
    var bdno = dn ? pool_acquire(g_pf_pool, g_pf_dev, bytes_dn_di) : null
    var bdnb = dn ? pool_acquire(g_pf_pool, g_pf_dev, bytes_dn_h) : null
    var bdng = dn ? pool_acquire(g_pf_pool, g_pf_dev, bytes_dn_h) : null
    var bdnqg = c.q_gated ? pool_acquire(g_pf_pool, g_pf_dev, bytes_dn_qg) : null
    var bdngate = c.q_gated ? pool_acquire(g_pf_pool, g_pf_dev, bytes_q) : null
    var batt = pool_acquire(g_pf_pool, g_pf_dev, bytes_att)
    var bcos = pool_acquire_untracked(g_pf_upool, g_pf_dev, bytes_tab)
    var bsin = pool_acquire_untracked(g_pf_upool, g_pf_dev, bytes_tab)
    let dual_rope = has_dual_rope(c)
    var bcos_swa = dual_rope ? pool_acquire_untracked(g_pf_upool, g_pf_dev, bytes_tab_sw) : null
    var bsin_swa = dual_rope ? pool_acquire_untracked(g_pf_upool, g_pf_dev, bytes_tab_sw) : null
    var bks : array<MetalBuffer?>
    var bvs : array<MetalBuffer?>
    bks |> reserve(int(c.n_layers + c.n_layer_nextn))
    bvs |> reserve(int(c.n_layers + c.n_layer_nextn))
    var no_panel : MetalBuffer?   // recurrent layers' null K/V slot
    for (l in range64(c.n_layers + c.n_layer_nextn)) {
        if (layer_kv_dim(c, l) == 0l) {   // recurrent layer (deltanet): no K/V at all
            bks |> push(no_panel)
            bvs |> push(no_panel)
            continue
        }
        if (kv_shared_layer(t, l)) {   // E-series: alias the source layer's panels (same class geometry)
            bks |> push(bks[t.kv_src[l]])
            bvs |> push(bvs[t.kv_src[l]])
            continue
        }
        let bytes_kv_l = uint64(kvrows * layer_kv_dim(c, l) * 4l)   // per-layer panels: each class's width
        bks |> push(pool_acquire(g_pf_pool, g_pf_dev, bytes_kv_l))
        bvs |> push(pool_acquire(g_pf_pool, g_pf_dev, bytes_kv_l))
    }
    // E-series PLE: side input uploaded LAYER-major ([l][p][ple] — the flat geglu ⊙ reads layer l's
    // block at one offset); gate outputs pad to mp rows like every GEMM panel
    let plef = has_ple(c)
    let ple_n = c.n_embd_per_layer
    let ple_all = c.n_layers * ple_n
    let bytes_ple_side = uint64(c.n_layers * npos * ple_n * 4l)
    let bytes_ple_g = uint64(mp * ple_n * 4l)
    let bytes_ple_proj = uint64(mp * ple_all * 4l)
    let ple_gpu = plef && s.ple_gpu_pending   // the gate stashed tokens — build the side input on device
    var bple : MetalBuffer?
    var bpleg : MetalBuffer?
    var bple_proj : MetalBuffer?
    var bple_tok : MetalBuffer?
    if (plef) {
        bpleg = pool_acquire(g_pf_pool, g_pf_dev, bytes_ple_g)
        if (ple_gpu) {
            bple = pool_acquire(g_pf_pool, g_pf_dev, bytes_ple_side)
            bple_proj = pool_acquire(g_pf_pool, g_pf_dev, bytes_ple_proj)
            bple_tok = pool_acquire_untracked(g_pf_upool, g_pf_dev, uint64(npos * 4l))
            unsafe {
                var tp = reinterpret<uint?>(metal_buffer_contents(bple_tok))
                for (p in range64(npos)) {
                    tp[p] = uint(s.ple_gpu_tokens[p])
                }
            }
        } else {
            bple = pool_acquire_untracked(g_pf_upool, g_pf_dev, bytes_ple_side)
            unsafe {
                // transpose s.ple_inp (position-major, p*all + l*ple) into layer-major blocks
                var dst = reinterpret<float?>(metal_buffer_contents(bple))
                let src = addr < float const? >(s.ple_inp[0])
                for (l in range64(c.n_layers)) {
                    for (p in range64(npos)) {
                        memcpy(reinterpret<void?>(dst + (l * npos + p) * ple_n),
                            reinterpret<void const?>(src + p * ple_all + l * ple_n), int(ple_n * 4l))
                    }
                }
            }
        }
    }
    // upload the embedded rows + this prefill's rope table(s) (built for the chunk's GLOBAL
    // positions — build_rope_table takes start_pos; dual-rope: the sliding class's pair too)
    unsafe {
        memcpy(metal_buffer_contents(bx), addr < void? >(s.x_b[0]), int(npos * dim * 4l))
        memcpy(metal_buffer_contents(bcos), addr < void? >(s.rope_cos[0]), int(npos * half * 4l))
        memcpy(metal_buffer_contents(bsin), addr < void? >(s.rope_sin[0]), int(npos * half * 4l))
        if (dual_rope) {
            memcpy(metal_buffer_contents(bcos_swa), addr < void? >(s.rope_cos_swa[0]), int(npos * half_sw * 4l))
            memcpy(metal_buffer_contents(bsin_swa), addr < void? >(s.rope_sin_swa[0]), int(npos * half_sw * 4l))
        }
    }
    // continuation: gather the session's existing rows [0, start_pos) into every layer's panel
    // head as f32 (paged sessions gather through kv_runs; tq4 rows dequant ROTATED and un-rotate
    // per head — GPU attention runs in the raw basis). K rows are stored ROPED, so no re-rope.
    if (start_pos > 0l) {
        unsafe {
            let gather_nrun = kv_runs(s, 0l, start_pos, s.kv_runs)
            let runsg = addr < KVRun const? >(s.kv_runs[0])
            let sgp = (s.kv_dtype_k == KVDtype.tq4 || s.kv_dtype_v == KVDtype.tq4) ? addr < float const? >(s.kv_signs[0]) : null
            for (l in range64(c.n_layers + c.n_layer_nextn)) {
                // recurrent: no rows; shared (E-series): aliased panel — the source's gather covers it
                continue if (bks[l] == null || kv_shared_layer(t, l))
                var kdst = reinterpret<float?>(metal_buffer_contents(bks[l]))
                var vdst = reinterpret<float?>(metal_buffer_contents(bvs[l]))
                let kcp = kv_layer_kbase(s, l, c.seq_len)
                let vcp = kv_layer_vbase(s, l, c.seq_len)
                let kvd_l = layer_kv_dim(c, l)
                for (r in range64(gather_nrun)) {
                    let rowbase = runsg[r].row0 - runsg[r].j0
                    let cnt = (runsg[r].j1 - runsg[r].j0) * kvd_l
                    kv_load_row(kcp, s.kv_dtype_k, kvd_l, rowbase + runsg[r].j0, 0l, cnt, kdst + runsg[r].j0 * kvd_l)
                    kv_load_row(vcp, s.kv_dtype_v, kvd_l, rowbase + runsg[r].j0, 0l, cnt, vdst + runsg[r].j0 * kvd_l)
                }
                if (s.kv_dtype_k == KVDtype.tq4) {
                    tq4_unrotate_rows(kdst, start_pos, kvd_l, layer_head_size(c, l), sgp)
                }
                if (s.kv_dtype_v == KVDtype.tq4) {
                    tq4_unrotate_rows(vdst, start_pos, kvd_l, layer_head_size(c, l), sgp)
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
    var u_npairs_q = pf_uniform_u32(uint(npos * qd / 2l))
    var u_npairs_k = pf_uniform_u32(uint(npos * kv_dim / 2l))
    var u_neox = pf_uniform_u32(c.rope_neox ? 1u : 0u)
    let has_bias = c.attn_qkv_bias
    var u_totq = has_bias ? pf_uniform_u32(uint(npos * qd)) : null
    var u_totkv = has_bias ? pf_uniform_u32(uint(npos * kv_dim)) : null
    let has_qkn = c.qk_norm
    let vfromk = c.v_from_k_mask != 0ul
    var u_nh_pf = has_qkn ? pf_uniform_u32(uint(n_heads)) : null
    var u_zero = (has_qkn || c.v_norm || vfromk) ? pf_uniform_u32(0u) : null
    var u_total_dim = pf_uniform_u32(uint(npos * dim))
    var u_total_h = pf_uniform_u32(uint(npos * hidden))
    var u_moe_ne = moe ? pf_uniform_u32(uint(c.n_expert)) : null
    var u_moe_nk = moe ? pf_uniform_u32(uint(c.n_expert_used)) : null
    var u_moe_np = moe ? pf_uniform_u32(uint(npos)) : null
    var u_moe_tot = moe ? pf_uniform_u32(uint(mtot)) : null
    var u_moe_nfe = moe ? pf_uniform_u32(uint(c.n_ff_exp)) : null
    var u_moe_eblk = moe ? pf_uniform_u32(uint(dim * c.n_ff_exp / 32l)) : null
    var u_moe_esb = moe ? pf_uniform_u32(uint(dim * c.n_ff_exp / 256l)) : null
    var u_moe_g0 = moe ? pf_uniform_u32(0u) : null
    var u_moe_g1 = moe ? pf_uniform_u32(1u) : null
    var u_moe_ptot = moe ? pf_uniform_u32(uint(mpad * c.n_ff_exp)) : null
    var u_moe_invd = g4moe ? pf_uniform_f32(1.0 / sqrt(float(dim))) : null
    var u_moe_gmode = moe ? pf_uniform_u32(c.moe_gate == MoeGate.softmax_weight ? 1u : (c.norm_topk_prob ? 0u : 2u)) : null
    // shared expert (qwen35moe): nsh width + npos*nsh act span + the gate GEMV's 1-row bound
    var u_moe_nsh = nsh > 0l ? pf_uniform_u32(uint(nsh)) : null
    var u_moe_shtot = nsh > 0l ? pf_uniform_u32(uint(npos * nsh)) : null
    var u_moe_one = nsh > 0l ? pf_uniform_u32(1u) : null
    var u_hass = c.attn_sinks ? pf_uniform_u32(1u) : null
    var u_scale = pf_uniform_f32(scale)
    var u_eps = pf_uniform_f32(c.norm_eps)
    var u_softcap = pf_uniform_f32(c.attn_logit_softcap)
    var u_win0 = pf_uniform_u32(0u)          // global layers' window bind
    var u_winsl = c.sliding_window > 0l ? pf_uniform_u32(uint(c.sliding_window)) : null
    // deltanet + gated-attention uniforms (Wave D)
    var u_dn_cd = dn ? pf_uniform_u32(uint(cd_dn)) : null
    var u_dn_di = dn ? pf_uniform_u32(uint(di_dn)) : null
    var u_dn_nvh = dn ? pf_uniform_u32(uint(nvh_dn)) : null
    var u_dn_kd = dn ? pf_uniform_u32(uint(dn_key_dim(c))) : null
    var u_dn_ds = dn ? pf_uniform_u32(uint(c.ssm_d_state)) : null
    var u_dn_nkh = dn ? pf_uniform_u32(uint(c.ssm_n_group)) : null
    var u_dn_dconv = dn ? pf_uniform_u32(uint(c.ssm_d_conv)) : null
    var u_dn_np = dn ? pf_uniform_u32(uint(npos)) : null
    var u_dn_qscale = dn ? pf_uniform_f32(1.0 / sqrt(float(c.ssm_d_state))) : null
    var u_dn_qd2 = c.q_gated ? pf_uniform_u32(uint(2l * qd)) : null
    var u_qtot = c.q_gated ? pf_uniform_u32(uint(npos * qd)) : null
    var u_rot = (c.rope_dim > 0l && c.rope_dim != head_size) ? pf_uniform_u32(uint(c.rope_dim)) : null
    // E-series PLE: gate/proj widths + the per-layer geglu ⊙ span
    var u_ple = plef ? pf_uniform_u32(uint(ple_n)) : null
    var u_ple_tot = plef ? pf_uniform_u32(uint(npos * ple_n)) : null
    var u_ple_np = ple_gpu ? pf_uniform_u32(uint(npos)) : null
    var u_ple_nl = ple_gpu ? pf_uniform_u32(uint(c.n_layers)) : null
    var u_ple_all = ple_gpu ? pf_uniform_u32(uint(ple_all)) : null
    var u_ple_sc0 = ple_gpu ? pf_uniform_f32(sqrt(float(ple_n))) : null
    var u_ple_isd = ple_gpu ? pf_uniform_f32(1.0 / sqrt(float(dim))) : null
    var u_ple_is2 = ple_gpu ? pf_uniform_f32(1.0 / sqrt(2.0)) : null
    // the SLIDING-class uniform twins (hetero models — uniform models leave these null and
    // every layer binds the base set)
    var u_qd_sw = hetero ? pf_uniform_u32(uint(qd_sw)) : null
    var u_kvd_sw = hetero ? pf_uniform_u32(uint(kvd_sw)) : null
    var u_hs_sw = hetero ? pf_uniform_u32(uint(hs_sw)) : null
    var u_kvm_sw = hetero ? pf_uniform_u32(uint(n_heads / kvh_sw)) : null
    var u_scale_sw = hetero ? pf_uniform_f32(c.attn_scale > 0.0 ? c.attn_scale : 1.0 / sqrt(float(hs_sw))) : null
    var u_npairs_q_sw = hetero ? pf_uniform_u32(uint(npos * qd_sw / 2l)) : null
    var u_npairs_k_sw = hetero ? pf_uniform_u32(uint(npos * kvd_sw / 2l)) : null
    // the V-from-K panel copy's grid guards (npos rows of the class's kv width)
    var u_vtot = vfromk ? pf_uniform_u32(uint(npos * kv_dim)) : null
    var u_vtot_sw = (vfromk && hetero) ? pf_uniform_u32(uint(npos * kvd_sw)) : null
    // logits-on-GPU (default ON, DASLLAMA_METAL_LOGITS=0 pins the CPU classifier): last-position
    // rmsnorm + classifier GEMV ride the last command buffer (prefill_override_logits_done skips
    // the CPU final step); q8 classifiers only, softcap + suppress ride epilogue kernels
    let cls_fmt = c.shared_weights ? t.emb_fmt : t.wcls_fmt
    let tied_f32 = c.shared_weights && !t.cls_q8 && cls_fmt == KqFmt.q8
    // knockout mode keeps gpu logits: constant across skip configs (deltas stay clean), and a
    // blob-only metal-flavor load HAS no CPU classifier to fall back to. NextN models serve
    // too — the readback stashes mtp_h from bxf (forward_prefill's handoff contract)
    let gpu_logits = (!g_pf_logits_cpu && get_env_variable("DASLLAMA_METAL_LOGITS") != "0" &&
        !tied_f32 &&
        t.wcls_off >= 0l)
    var bxf = gpu_logits ? pool_acquire(g_pf_pool, g_pf_dev, uint64(dim * 4l)) : null
    var blog = gpu_logits ? pool_acquire(g_pf_pool, g_pf_dev, uint64(c.vocab_size * 4l)) : null
    var u_vocab = gpu_logits ? pf_uniform_u32(uint(c.vocab_size)) : null
    var u_flcap = (gpu_logits && c.final_logit_softcap > 0.0) ? pf_uniform_f32(c.final_logit_softcap) : null
    // suppressed token ids pin to -1e30 AFTER the cap (gemma4) — the CPU classifier's order
    let nsupp = int64(length(t.suppress))
    var bsupp : MetalBuffer?
    var u_nsupp : MetalBuffer?
    if (gpu_logits && nsupp > 0l) {
        bsupp = pool_acquire_untracked(g_pf_upool, g_pf_dev, uint64(nsupp * 4l))
        unsafe {
            var sp = reinterpret<uint?>(metal_buffer_contents(bsupp))
            for (i in range64(nsupp)) {
                sp[i] = uint(t.suppress[i])
            }
        }
        u_nsupp = pf_uniform_u32(uint(nsupp))
    }
    // MTP draft-slab warm (spec on): ONE extra loop iteration runs the draft block at FULL
    // npos so every trunk uniform reuses verbatim — the last warm row is garbage by design
    // (needs the unknown next token), causally unread, rewritten by the next draft/seam
    let warm = (get_mtp_spec() && c.n_layer_nextn == 1l && gpu_logits && t.mtp_ehproj_off >= 0l &&
        (dim % 64l) == 0l)
    let n_enc_layers = c.n_layers + (warm ? 1l : 0l)
    var bwm_res : MetalBuffer?    // warm: trunk residual save (x_b readback + the cls tail read it)
    var bwm_emb : MetalBuffer?    // warm: shifted embed rows
    var bwm_cat : MetalBuffer?    // warm: [mp x 2dim] eh_proj input
    var bwm_tmp : MetalBuffer?    // warm: rms_final intermediate
    var u_dim2 : MetalBuffer?
    var u_wm_tot : MetalBuffer?
    if (warm) {
        bwm_res = pool_acquire(g_pf_pool, g_pf_dev, uint64(npos * dim * 4l))
        bwm_emb = pool_acquire(g_pf_pool, g_pf_dev, uint64(npos * dim * 4l))
        bwm_cat = pool_acquire(g_pf_pool, g_pf_dev, uint64(mp * 2l * dim * 4l))
        bwm_tmp = pool_acquire(g_pf_pool, g_pf_dev, uint64(npos * dim * 4l))
        u_dim2 = pf_uniform_u32(uint(2l * dim))
        u_wm_tot = pf_uniform_u32(uint(npos * dim))
        // shifted embeds: row p pairs the NEXT position's embed with hidden p — s.x_b still
        // holds the window's embed panel here; the garbage last row reuses row 0
        unsafe {
            var dst = reinterpret<uint8?>(metal_buffer_contents(bwm_emb))
            memcpy(reinterpret<void?>(dst), addr < void? >(s.x_b[dim]), int((npos - 1l) * dim * 4l))
            memcpy(reinterpret<void?>(dst + (npos - 1l) * dim * 4l), addr < void? >(s.x_b[0]), int(dim * 4l))
        }
    }
    // ggml-geometry QK/AV (default): ~10x the old trio GEMMs' rate; needs hs % 64 (the AV grid;
    // BOTH classes) and mp % 64 (the QK key grid — true by construction). DASLLAMA_METAL_ATTN=0
    // pins the trio.
    let mmattn = (head_size % 64l) == 0l && (hs_sw % 64l) == 0l && get_env_variable("DASLLAMA_METAL_ATTN") != "0"
    let ts_enc = ref_time_ticks()
    let setup_us = get_time_usec(ts_all)
    var err : string
    var gpu_ms : double
    var encode_ms : double
    // command-buffer split (DASLLAMA_METAL_NCB chunks): each chunk commits as soon as encoded, so
    // the scheduler analyzes chunk k while chunk k-1 executes. DASLLAMA_METAL_UNRETAINED=1 skips
    // per-dispatch retain/release too. Default ~4 layers/chunk (3B: slack 57ms -> 9ms, +10% pp512).
    let ncb = clamp(int64(pf_env_int("DASLLAMA_METAL_NCB", int((n_enc_layers + 3l) / 4l))), 1l, n_enc_layers)
    let unret = get_env_variable("DASLLAMA_METAL_UNRETAINED") == "1"
    let layers_per_cb = (n_enc_layers + ncb - 1l) / ncb
    var cbs : array<MetalCommandBuffer?>
    cbs |> reserve(int(ncb))
    var cb : MetalCommandBuffer?
    var enc : MetalComputeEncoder?
    var dnli = 0l   // recurrent-layer index — addresses the dn state/conv mirror slices in order
    {
        for (l in range64(n_enc_layers)) {
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
            if (l == 0l && ple_gpu) {
                // PLE pre-step on device (the serial encoder orders the three stages): q8 token-row
                // gather (layer-major, sqrt(ple) folded) -> bf16 model_proj GEMM off bx -> rms+combine
                unsafe {
                    let bemb = pf_blob_of(t, t.ple_emb_off)
                    metal_set_pipeline(enc, g_pf_pso_ple_gather)
                    metal_set_buffer(enc, bemb.buf, bemb.boff, 0)
                    metal_set_buffer(enc, bemb.buf, bemb.boff, 1)
                    metal_set_buffer(enc, bple_tok, 0ul, 2)
                    metal_set_buffer(enc, bple, 0ul, 3)
                    metal_set_buffer(enc, u_ple_np, 0ul, 4)
                    metal_set_buffer(enc, u_ple, 0ul, 5)
                    metal_set_buffer(enc, u_ple_nl, 0ul, 6)
                    metal_set_buffer(enc, u_ple_sc0, 0ul, 7)
                    metal_dispatch_threadgroups(enc, uint3(uint(c.n_layers), uint(npos), 1u), uint3(uint(ple_n), 1u, 1u))
                    let bwp = pf_bf16_of(t, t.ple_model_off)
                    metal_set_pipeline(enc, g_pf_pso_bf16_mm)
                    metal_set_threadgroup_memory_length(enc, metal_bf16_mulmm_msl_tgmem, 0)
                    metal_set_buffer(enc, bwp.buf, bwp.boff, 0)
                    metal_set_buffer(enc, bx, 0ul, 2)
                    metal_set_buffer(enc, bple_proj, 0ul, 3)
                    metal_set_buffer(enc, u_dim, 0ul, 4)
                    metal_set_buffer(enc, u_ple_all, 0ul, 5)
                    metal_dispatch_threadgroups(enc, uint3(uint(mp / 32l), uint(ple_all / 64l), 1u), uint3(128u, 1u, 1u))
                    var bw_pn = pf_upload_region(addr < void? >(t.fblob[t.ple_pnorm_off]), uint64(ple_n * 4l))
                    metal_set_pipeline(enc, g_pf_pso_ple_finish)
                    metal_set_threadgroup_memory_length(enc, metal_ple_finish_msl_tgmem, 0)
                    metal_set_buffer(enc, bple_proj, 0ul, 0)
                    metal_set_buffer(enc, bw_pn, 0ul, 1)
                    metal_set_buffer(enc, bple, 0ul, 2)
                    metal_set_buffer(enc, u_ple_np, 0ul, 3)
                    metal_set_buffer(enc, u_ple, 0ul, 4)
                    metal_set_buffer(enc, u_ple_nl, 0ul, 5)
                    metal_set_buffer(enc, u_ple_isd, 0ul, 6)
                    metal_set_buffer(enc, u_eps, 0ul, 7)
                    metal_set_buffer(enc, u_ple_is2, 0ul, 8)
                    metal_dispatch_threadgroups(enc, uint3(uint(c.n_layers), uint(npos), 1u), uint3(uint(ple_n), 1u, 1u))
                }
                s.ple_gpu_pending = false
            }
            if (warm && l == c.n_layers) {
                // trunk done — save the residual panel (x_b readback + cls read it), then swap
                // bx to the draft block's eh_proj input; the body below runs the draft layer
                // verbatim (same uniforms, npos rows, K/V panel + weight arrays at index l)
                unsafe {
                    metal_set_pipeline(enc, g_pf_pso_copy)
                    metal_set_buffer(enc, bwm_res, 0ul, 0)
                    metal_set_buffer(enc, bx, 0ul, 1)
                    metal_set_buffer(enc, u_wm_tot, 0ul, 2)
                    metal_dispatch_threadgroups(enc, uint3(uint((npos * dim + 63l) / 64l), 1u, 1u), uint3(64u, 1u, 1u))
                    var bw_en = pf_upload_region(addr < void? >(t.fblob[t.mtp_enorm_off]), uint64(dim * 4l))
                    pf_enc_rms(enc, bwm_emb, bw_en, bxb, u_dim, u_eps, npos)
                    var bw_fin = pf_upload_region(addr < void? >(t.fblob[t.rms_final_off]), uint64(dim * 4l))
                    pf_enc_rms(enc, bx, bw_fin, bwm_tmp, u_dim, u_eps, npos)
                    var bw_hn = pf_upload_region(addr < void? >(t.fblob[t.mtp_hnorm_off]), uint64(dim * 4l))
                    pf_enc_rms(enc, bwm_tmp, bw_hn, bxb2, u_dim, u_eps, npos)
                    metal_set_pipeline(enc, g_pf_pso_cat2)
                    metal_set_buffer(enc, bwm_cat, 0ul, 0)
                    metal_set_buffer(enc, bxb, 0ul, 1)
                    metal_set_buffer(enc, bxb2, 0ul, 2)
                    metal_set_buffer(enc, u_dim, 0ul, 3)
                    metal_set_buffer(enc, u_wm_tot, 0ul, 4)
                    metal_dispatch_threadgroups(enc, uint3(uint((npos * dim + 63l) / 64l), 1u, 1u), uint3(64u, 1u, 1u))
                    if (t.mtp_ehproj_fmt == KqFmt.q8) {
                        let behp = pf_blob_of(t, t.mtp_ehproj_off)
                        enc_gemm_mm(enc, behp.buf, behp.boff, bwm_cat, bx, u_dim2, u_dim, mp, dim)
                    } else {
                        pf_enc_kq_site_mm(enc, t, t.mtp_ehproj_fmt, t.mtp_ehproj_off, dim, bwm_cat, bx, u_dim2, u_dim, mp)
                    }
                }
            }
            unsafe {
                var bw_att = pf_upload_region(addr < void? >(t.fblob[t.rms_att_off + l * dim]), uint64(dim * 4l))
                var bw_ffn = pf_upload_region(addr < void? >(t.fblob[t.rms_ffn_off + l * dim]), uint64(dim * 4l))
                // sliding layers on hetero models (gemma4) swap the whole class geometry —
                // sizes, GEMM grids, uniform binds; uniform models bind the base set everywhere
                let lsl = layer_is_sliding(c, l)
                let het = hetero && lsl
                let hs_l = het ? hs_sw : head_size
                let kvd_l = het ? kvd_sw : kv_dim
                let qd_l = het ? qd_sw : qd
                let koff = uint64(start_pos * kvd_l * 4l)
                var u_qd_l = het ? u_qd_sw : u_qd
                var u_kvd_l = het ? u_kvd_sw : u_kvd
                var u_hs_l = het ? u_hs_sw : u_hs
                var u_kvm_l = het ? u_kvm_sw : u_kvm
                var u_scale_l = het ? u_scale_sw : u_scale
                var u_npairs_q_l = het ? u_npairs_q_sw : u_npairs_q
                var u_npairs_k_l = het ? u_npairs_k_sw : u_npairs_k
                let has_wv = t.wv_offs[l] >= 0l
                let recr = layer_is_recurrent(c, l)
                // E-series shared-KV layer: Q-only — bks[l]/bvs[l] alias the source's panels
                let kvsh = kv_shared_layer(t, l)
                // attention: [pending W2 residual +] norm -> [requant ->] Q/K/V -> rope ->
                // scores+softmax -> P.V -> out-proj (fused path folds each residual add into the
                // FOLLOWING add+rms+quant; the lcpp path feeds normed f32 rows straight to mul_mm)
                if (g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                    pf_enc_rms(enc, bx, bw_att, bxb, u_dim, u_eps, npos)
                }
                if (recr) {
                    // deltanet layer (Wave D): batched projections -> conv+SiLU -> q/k L2 -> fused
                    // sequential scan (state walks the whole window in-kernel) -> gated out-norm -> out-proj
                    if (g_pf_skip != "gemm") {
                        let bwqkv = pf_blob_of(t, t.dnqkv_offs[l])
                        enc_gemm_mm(enc, bwqkv.buf, bwqkv.boff, bxb, bdnqkv, u_dim, u_dn_cd, mp, cd_dn)
                        let bwz = pf_blob_of(t, t.dngate_offs[l])
                        enc_gemm_mm(enc, bwz.buf, bwz.boff, bxb, bdnz, u_dim, u_dn_di, mp, di_dn)
                        if (t.dn_ba_f32) {
                            // F32-on-disk β/α (35B): fblob slabs ride the router-b f32 GEMV
                            var bw_bt = pf_upload_region(addr < void? >(t.fblob[t.dn_betaf_off + l * dim * nvh_dn]), uint64(dim * nvh_dn * 4l))
                            pf_enc_slab_gemv_b(enc, bw_bt, bxb, bdnb, u_dim, u_dn_nvh, u_dn_np, nvh_dn, npos)
                            var bw_al = pf_upload_region(addr < void? >(t.fblob[t.dn_alphaf_off + l * dim * nvh_dn]), uint64(dim * nvh_dn * 4l))
                            pf_enc_slab_gemv_b(enc, bw_al, bxb, bdng, u_dim, u_dn_nvh, u_dn_np, nvh_dn, npos)
                        } else {
                            let bwb = pf_blob_of(t, t.dnbeta_offs[l])
                            pf_enc_dn_ba(enc, bwb.buf, bwb.boff, bxb, bdnb, u_dim, u_dn_nvh, nvh_dn, npos)
                            let bwa = pf_blob_of(t, t.dnalpha_offs[l])
                            pf_enc_dn_ba(enc, bwa.buf, bwa.boff, bxb, bdng, u_dim, u_dn_nvh, nvh_dn, npos)
                        }
                    }
                    if (g_pf_skip != "attn" && g_pf_skip != "nongemm") {
                        let soff = dn_state_base + uint64(dnli * nvh_dn * c.ssm_d_state * c.ssm_d_state * 4l)
                        let hoff = dn_conv_base + uint64(dnli * (c.ssm_d_conv - 1l) * cd_dn * 4l)
                        var bw_cv = pf_upload_region(addr < void? >(t.fblob[t.dn_conv_off + l * c.ssm_d_conv * cd_dn]), uint64(c.ssm_d_conv * cd_dn * 4l))
                        pf_enc_dn_conv(enc, bdnqkv, bdn_state, hoff, bw_cv, bdncv, u_dn_cd, u_dn_dconv, u_dn_np, cd_dn, npos)
                        pf_enc_dn_conv_hist(enc, bdnqkv, bdn_state, hoff, u_dn_cd, u_dn_dconv, u_dn_np, cd_dn, u_win0)
                        pf_enc_dn_l2norm(enc, bdncv, u_dn_cd, u_dn_kd, u_dn_ds, u_eps, u_dn_qscale, 2l * c.ssm_n_group, npos)
                        var baa = pf_upload_region(addr < void? >(t.fblob[t.dn_a_off + l * nvh_dn]), uint64(nvh_dn * 4l))
                        var bdt = pf_upload_region(addr < void? >(t.fblob[t.dn_dt_off + l * nvh_dn]), uint64(nvh_dn * 4l))
                        pf_enc_dn_scan(enc, bdncv, bdn_state, soff, bdnb, bdng, baa, bdt, bdno,
                            u_dn_cd, u_dn_kd, u_dn_ds, u_dn_nvh, u_dn_nkh, u_dn_di, u_dn_np, c.ssm_d_state, nvh_dn, u_win0)
                        var bwn = pf_upload_region(addr < void? >(t.fblob[t.dn_norm_off + l * c.ssm_d_state]), uint64(c.ssm_d_state * 4l))
                        pf_enc_dn_gate(enc, bdno, bdnz, bwn, u_dn_ds, u_dn_di, u_eps, nvh_dn, npos)
                    }
                    if (g_pf_skip != "gemm") {
                        let bwo = pf_blob_of(t, t.dnout_offs[l])
                        enc_gemm_mm(enc, bwo.buf, bwo.boff, bdno, bxb2, u_dn_di, u_dim, mp, dim)
                    }
                    dnli++
                }
                if (!recr && g_pf_skip != "gemm") {
                    // K-quant tensors take their mul_mm twins per tensor (the gate pins the
                    // mm shape for kquant loads); q8-tagged tensors keep the blob path
                    let fq = pf_fmt_at(t.wq_fmt, l)
                    let fk = pf_fmt_at(t.wk_fmt, l)
                    let fv = has_wv ? pf_fmt_at(t.wv_fmt, l) : KqFmt.q8
                    if (fq != KqFmt.q8 || fk != KqFmt.q8 || fv != KqFmt.q8) {
                        if (c.q_gated) {
                            // 2x-wide packed [q|gate] projection, deinterleaved into the q panel + gate rows
                            if (fq != KqFmt.q8) {
                                pf_enc_kq_site_mm(enc, t, fq, t.wq_offs[l], 2l * qd_l, bxb, bdnqg, u_dim, u_dn_qd2, mp)
                            } else {
                                let bwqb = pf_blob_of(t, t.wq_offs[l])
                                enc_gemm_mm(enc, bwqb.buf, bwqb.boff, bxb, bdnqg, u_dim, u_dn_qd2, mp, 2l * qd_l)
                            }
                            pf_enc_dn_deint(enc, bdnqg, bq, bdngate, u_hs_l, u_qd_l, u_qd_l, qd_l, npos)
                        } elif (fq != KqFmt.q8) {
                            pf_enc_kq_site_mm(enc, t, fq, t.wq_offs[l], qd_l, bxb, bq, u_dim, u_qd_l, mp)
                        } else {
                            let bwqb = pf_blob_of(t, t.wq_offs[l])
                            enc_gemm_mm(enc, bwqb.buf, bwqb.boff, bxb, bq, u_dim, u_qd_l, mp, qd_l)
                        }
                        if (!kvsh) {
                            if (fk != KqFmt.q8) {
                                pf_enc_kq_site_mm(enc, t, fk, t.wk_offs[l], kvd_l, bxb, bks[l], u_dim, u_kvd_l, mp, koff)
                            } else {
                                let bwkb = pf_blob_of(t, t.wk_offs[l])
                                enc_gemm_mm(enc, bwkb.buf, bwkb.boff, bxb, bks[l], u_dim, u_kvd_l, mp, kvd_l, koff)
                            }
                            if (has_wv) {
                                if (fv != KqFmt.q8) {
                                    pf_enc_kq_site_mm(enc, t, fv, t.wv_offs[l], kvd_l, bxb, bvs[l], u_dim, u_kvd_l, mp, koff)
                                } else {
                                    let bwvb = pf_blob_of(t, t.wv_offs[l])
                                    enc_gemm_mm(enc, bwvb.buf, bwvb.boff, bxb, bvs[l], u_dim, u_kvd_l, mp, kvd_l, koff)
                                }
                            }
                        }
                    } else {
                        let bwqb = pf_blob_of(t, t.wq_offs[l])
                        if (c.q_gated) {
                            // 2x-wide packed [q|gate] projection, deinterleaved into the q panel + gate rows
                            enc_gemm_mm(enc, bwqb.buf, bwqb.boff, bxb, bdnqg, u_dim, u_dn_qd2, mp, 2l * qd_l)
                            pf_enc_dn_deint(enc, bdnqg, bq, bdngate, u_hs_l, u_qd_l, u_qd_l, qd_l, npos)
                        } else {
                            enc_gemm_mm(enc, bwqb.buf, bwqb.boff, bxb, bq, u_dim, u_qd_l, mp, qd_l)
                        }
                        if (!kvsh) {   // shared layers have no K/V tensors (offs -1)
                            let bwkb = pf_blob_of(t, t.wk_offs[l])
                            enc_gemm_mm(enc, bwkb.buf, bwkb.boff, bxb, bks[l], u_dim, u_kvd_l, mp, kvd_l, koff)
                            if (has_wv) {
                                let bwvb = pf_blob_of(t, t.wv_offs[l])
                                enc_gemm_mm(enc, bwvb.buf, bwvb.boff, bxb, bvs[l], u_dim, u_kvd_l, mp, kvd_l, koff)
                            }
                        }
                    }
                }
                if (!recr && !kvsh && (!has_wv || c.v_norm) && g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                    // gemma4 V: no-wv layers copy the PRE-norm/PRE-rope K panel rows into the V
                    // panel; v_norm then runs the weightless per-head RMS (ones plane) in place —
                    // the CPU mm_qkv order exactly, BEFORE qk_norm touches k
                    if (!has_wv) {
                        metal_set_pipeline(enc, g_pf_pso_copy)
                        metal_set_buffer(enc, bvs[l], koff, 0)
                        metal_set_buffer(enc, bks[l], koff, 1)
                        metal_set_buffer(enc, het ? u_vtot_sw : u_vtot, 0ul, 2)
                        metal_dispatch_threadgroups(enc, uint3(uint((npos * kvd_l + 63l) / 64l), 1u, 1u), uint3(64u, 1u, 1u))
                    }
                    if (c.v_norm) {
                        // MAX-width ones upload: the region cache is address-keyed and both
                        // classes read this one plane (the single-decode lesson)
                        var bones = pf_upload_region(addr < void? >(t.fblob[t.rms_ones_off]), uint64(max_head_size(c) * 4l))
                        enc_qk_norm_pf(enc, bvs[l], bones, u_zero, u_zero, u_kvd_l, u_hs_l, u_zero, u_eps,
                            u_zero, kvd_l / hs_l, npos, hs_l, koff)
                    }
                }
                if (!recr && has_bias && g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                    // QKV bias before rope — chunk rows only (existing rows stored biased+roped).
                    // Per-layer widths for the offsets; the count uniforms stay base-class (no
                    // hetero model carries QKV bias — the CPU bias-plane layout is uniform too)
                    var bbq = pf_upload_region(addr < void? >(t.bq[l * qd_l]), uint64(qd_l * 4l))
                    var bbk = pf_upload_region(addr < void? >(t.bk[l * kvd_l]), uint64(kvd_l * 4l))
                    var bbv = pf_upload_region(addr < void? >(t.bv[l * kvd_l]), uint64(kvd_l * 4l))
                    enc_add_bias_rows(enc, bq, bbq, u_qd, u_totq, npos * qd)
                    enc_add_bias_rows(enc, bks[l], bbk, u_kvd, u_totkv, npos * kv_dim, koff)
                    enc_add_bias_rows(enc, bvs[l], bbv, u_kvd, u_totkv, npos * kv_dim, koff)
                }
                if (!recr && has_qkn && g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                    // per-head QK-norm before rope — the chunk's q panel and its new K rows
                    var bqw = pf_upload_region(addr < void? >(t.fblob[t.rms_q_offs[l]]), uint64(hs_l * 4l))
                    enc_qk_norm_pf(enc, bq, bqw, u_nh_pf, u_qd_l, u_qd_l, u_hs_l, u_hs_l, u_eps, u_qd_l, n_heads, npos, hs_l)
                    if (!kvsh) {
                        var bkw = pf_upload_region(addr < void? >(t.fblob[t.rms_k_offs[l]]), uint64(hs_l * 4l))
                        enc_qk_norm_pf(enc, bks[l], bkw, u_zero, u_zero, u_kvd_l, u_hs_l, u_zero, u_eps,
                            u_zero, kvd_l / hs_l, npos, hs_l, koff)
                    }
                }
                if (!recr && g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                    // sliding layers rope with the swa row pair (dual-rope configs — gemma3/4;
                    // hetero: the swa pair's rows are hs_sw/2 wide — the class's own width)
                    var bcos_l = lsl && dual_rope ? bcos_swa : bcos
                    var bsin_l = lsl && dual_rope ? bsin_swa : bsin
                    enc_rope(enc, bq, bcos_l, bsin_l, u_qd_l, u_hs_l, u_npairs_q_l, u_neox, npos * qd_l / 2l, [brot = u_rot])
                    if (!kvsh) {
                        enc_rope(enc, bks[l], bcos_l, bsin_l, u_kvd_l, u_hs_l, u_npairs_k_l, u_neox, npos * kvd_l / 2l, koff, [brot = u_rot])
                    }
                }
                if (!recr && g_pf_skip != "attn" && g_pf_skip != "nongemm") {
                    // sliding layers bind the span — the QK forms skip fully-below-window blocks,
                    // the softmax masks + zeroes the below-window prefix; global layers bind 0
                    var bwin = lsl && u_winsl != null ? u_winsl : u_win0
                    if (g_pf_skip != "attn_qk") {
                        if (mmattn) {
                            enc_qk_mm(enc, bq, bks[l], batt, u_qd_l, u_kvd_l, u_hs_l, u_kvm_l, u_np, u_np32, u_scale_l, u_qoff, u_qrows, bwin, mp, nk64, n_heads)
                        } else {
                            enc_qk(enc, bq, bks[l], batt, u_qd_l, u_kvd_l, u_hs_l, u_kvm_l, u_np, u_np32, u_scale_l, u_qoff, u_qrows, bwin, mp, nk64, n_heads)
                        }
                    }
                    if (g_pf_skip != "attn_sm") {
                        var bsnk : MetalBuffer?
                        if (c.attn_sinks) {
                            bsnk = pf_upload_region(unsafe(addr < void? >(t.fblob[t.sinks_off + l * n_heads])), uint64(n_heads * 4l))
                        }
                        enc_softmax(enc, batt, u_np32, u_qoff, u_qrows, bwin, u_softcap, npos, n_heads, bsnk, u_hass)
                    }
                    if (g_pf_skip != "attn_av") {
                        if (mmattn) {
                            enc_av_mm(enc, batt, bvs[l], bxb, u_qd_l, u_kvd_l, u_hs_l, u_kvm_l, u_np, u_np32, u_qoff, u_qrows, mp, n_heads, hs_l)
                        } else {
                            enc_av(enc, batt, bvs[l], bxb, u_qd_l, u_kvd_l, u_hs_l, u_kvm_l, u_np, u_np32, u_qoff, u_qrows, mp, n_heads, hs_l)
                        }
                    }
                }
                if (!recr && c.q_gated && g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                    // gated-attention epilogue: out ⊙ σ(gate) between the attention output and wo (the CPU order)
                    pf_enc_ew2(enc, g_pf_pso_sigmul, bxb, bdngate, u_qtot, npos * qd)
                }
                if (!recr && g_pf_skip != "gemm") {
                    let fo = pf_fmt_at(t.wo_fmt, l)
                    if (fo != KqFmt.q8) {
                        pf_enc_kq_site_mm(enc, t, fo, t.wo_offs[l], dim, bxb, bxb2, u_qd_l, u_dim, mp)
                    } else {
                        let bwob = pf_blob_of(t, t.wo_offs[l])
                        enc_gemm_mm(enc, bwob.buf, bwob.boff, bxb, bxb2, u_qd_l, u_dim, mp, dim)
                    }
                    if (c.attn_out_bias) {
                        var bwo_b = pf_upload_region(unsafe(addr < void? >(t.bo[l * dim])), uint64(dim * 4l))
                        enc_add_bias_rows(enc, bxb2, bwo_b, u_dim, u_total_dim, npos * dim)
                    }
                }
                // ffn: [post-attn norm ->] [WO residual +] norm -> [requant ->] W1/W3 ->
                // swiglu/geglu[+requant] -> W2 [-> post-ffn norm]
                if (g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                    if (c.pre_post_norm) {
                        // gemma sandwich: norm the attention branch IN PLACE before the residual add
                        var bw_pa = pf_upload_region(addr < void? >(t.fblob[t.rms_post_att_off + l * dim]), uint64(dim * 4l))
                        pf_enc_rms(enc, bxb2, bw_pa, bxb2, u_dim, u_eps, npos)
                    }
                    pf_enc_ew2(enc, g_pf_pso_add, bx, bxb2, u_total_dim, npos * dim)
                    pf_enc_rms(enc, bx, bw_ffn, bxb, u_dim, u_eps, npos)
                }
                if (moe && g_pf_skip != "gemm" && g_pf_skip != "moe") {
                    // MoE FFN (Wave C stage 2b): per-position routing + CSR, then one gathered
                    // mul_mm per projection over the touched experts; ordered reduce into bxb
                    // (g4: its own router norm + pre_ffn2 input via bxb2, reduce into bmg4rt)
                    let fe1 = pf_fmt_at(t.we1_fmt, l)
                    let fe3 = pf_fmt_at(t.we3_fmt, l)
                    if (g4moe) {
                        var bw_p2 = pf_upload_region(unsafe(addr < void? >(t.fblob[t.rms_pre_ffn2_off + l * dim])), uint64(dim * 4l))
                        pf_enc_rms(enc, bx, bw_p2, bxb2, u_dim, u_eps, npos)
                        var bw_rs = pf_upload_region(unsafe(addr < void? >(t.fblob[t.moe_router_scale_off + l * dim])), uint64(dim * 4l))
                        pf_enc_g4_router_norm(enc, bx, bw_rs, bmg4rt, u_dim, u_eps, u_moe_invd, npos)
                    }
                    var brb : MetalBuffer? = g_pf_one
                    var bhrb = u_moe_g0
                    if (c.moe_router_bias) {
                        brb = pf_upload_region(unsafe(addr < void? >(t.fblob[t.router_b_off + l * c.n_expert])), uint64(c.n_expert * 4l))
                        bhrb = u_moe_gmode
                    }
                    pf_enc_moe_route(enc, t, l, g4moe ? bmg4rt : bxb, bmlg, bmsel, bmcnt, bmbase, bmbkt, bminv,
                        u_dim, u_moe_ne, u_moe_nk, u_moe_gmode, brb, bhrb, u_moe_tot, u_moe_np, npos)
                    if (g4moe) {
                        var bw_ds = pf_upload_region(unsafe(addr < void? >(t.fblob[t.moe_down_scale_off + l * c.n_expert])), uint64(c.n_expert * 4l))
                        pf_enc_moe_wscale(enc, bmsel, bw_ds, u_moe_nk, npos)
                    }
                    if (nsh > 0l) {
                        // shexp raw gate dots (σ in the combine) — bmlg is free once select consumed it
                        var bw_shg = pf_upload_region(unsafe(addr < void? >(t.fblob[t.shexp_gate_off + l * dim])), uint64(dim * 4l))
                        pf_enc_slab_gemv_b(enc, bw_shg, bxb, bmlg, u_dim, u_moe_one, u_moe_np, 1l, npos)
                    }
                    if (!pf_skip_moe_mm()) {
                        if (t.experts_mx4) {
                            var bwb1 = pf_upload_region(unsafe(addr < void? >(t.fblob[t.web1_off + l * c.n_expert * c.n_ff_exp])), uint64(c.n_expert * c.n_ff_exp * 4l))
                            var bwb3 = pf_upload_region(unsafe(addr < void? >(t.fblob[t.web3_off + l * c.n_expert * c.n_ff_exp])), uint64(c.n_expert * c.n_ff_exp * 4l))
                            pf_enc_moe_mm_mx4(enc, t, t.we1_offs[l], c.n_ff_exp, npos, g4moe ? bxb2 : bxb, bmg,
                                u_dim, u_moe_nfe, bmcnt, bmbase, bmbkt, u_moe_eblk, u_moe_nk, u_moe_g1)
                            pf_enc_moe_mm_mx4(enc, t, t.we3_offs[l], c.n_ff_exp, npos, g4moe ? bxb2 : bxb, bmg2,
                                u_dim, u_moe_nfe, bmcnt, bmbase, bmbkt, u_moe_eblk, u_moe_nk, u_moe_g1)
                            if (g_pf_skip != "moe_bias") {
                                pf_enc_moe_bias_rows(enc, bmg, bwb1, bmcnt, bmbase, u_moe_nfe, c.n_ff_exp, npos, c.n_expert)
                                pf_enc_moe_bias_rows(enc, bmg2, bwb3, bmcnt, bmbase, u_moe_nfe, c.n_ff_exp, npos, c.n_expert)
                            }
                        } else {
                            pf_enc_moe_mm(enc, t, fe1, t.we1_offs[l], c.n_ff_exp, npos, g4moe ? bxb2 : bxb, bmg,
                                u_dim, u_moe_nfe, bmcnt, bmbase, bmbkt, fe1 == KqFmt.q8 || fe1 == KqFmt.q51 ? u_moe_eblk : u_moe_esb, u_moe_nk, u_moe_g1)
                            pf_enc_moe_mm(enc, t, fe3, t.we3_offs[l], c.n_ff_exp, npos, g4moe ? bxb2 : bxb, bmg2,
                                u_dim, u_moe_nfe, bmcnt, bmbase, bmbkt, fe3 == KqFmt.q8 || fe3 == KqFmt.q51 ? u_moe_eblk : u_moe_esb, u_moe_nk, u_moe_g1)
                        }
                        if (nsh > 0l) {
                            // shexp gate/up: dense q8 mul_mm at the wsh planes (bhb/bhb2 sized max(hidden, nsh))
                            let bwsh1 = pf_blob_of(t, t.wsh1_off + l * dim * nsh)
                            enc_gemm_mm(enc, bwsh1.buf, bwsh1.boff, bxb, bhb, u_dim, u_moe_nsh, mp, nsh)
                            let bwsh3 = pf_blob_of(t, t.wsh3_off + l * dim * nsh)
                            enc_gemm_mm(enc, bwsh3.buf, bwsh3.boff, bxb, bhb2, u_dim, u_moe_nsh, mp, nsh)
                        }
                    }
                }
                if (moe && g_pf_skip != "ew" && g_pf_skip != "nongemm" && g_pf_skip != "moe" && !pf_skip_moe_act()) {
                    if (c.ffn_act == FfnAct.swiglu_oai) {
                        pf_enc_ew2_v4(enc, g_pf_pso_swiglu_oai, bmg, bmg2, u_moe_ptot, mpad * c.n_ff_exp)
                    } else {
                        pf_enc_ew2(enc, g4moe ? g_pf_pso_geglu : g_pf_pso_swiglu, bmg, bmg2, u_moe_ptot, mpad * c.n_ff_exp)
                    }
                    if (nsh > 0l) {
                        pf_enc_ew2(enc, g_pf_pso_swiglu, bhb, bhb2, u_moe_shtot, npos * nsh)
                    }
                }
                if (moe && g_pf_skip != "gemm" && g_pf_skip != "moe") {
                    let fe2 = pf_fmt_at(t.we2_fmt, l)
                    if (!pf_skip_moe_mm()) {
                        if (t.experts_mx4) {
                            var bwb2 = pf_upload_region(unsafe(addr < void? >(t.fblob[t.web2_off + l * c.n_expert * dim])), uint64(c.n_expert * dim * 4l))
                            pf_enc_moe_mm_mx4(enc, t, t.we2_offs[l], dim, npos, bmg, bmdn,
                                u_moe_nfe, u_dim, bmcnt, bmbase, bmbkt, u_moe_eblk, u_moe_nk, u_moe_g0)
                            if (g_pf_skip != "moe_bias") {
                                pf_enc_moe_bias_rows(enc, bmdn, bwb2, bmcnt, bmbase, u_dim, dim, npos, c.n_expert)
                            }
                        } else {
                            pf_enc_moe_mm(enc, t, fe2, t.we2_offs[l], dim, npos, bmg, bmdn,
                                u_moe_nfe, u_dim, bmcnt, bmbase, bmbkt, fe2 == KqFmt.q8 || fe2 == KqFmt.q51 ? u_moe_eblk : u_moe_esb, u_moe_nk, u_moe_g0)
                        }
                        if (nsh > 0l) {
                            // shexp down into bxb2 (free post-residual) — bxb still feeds the reduce
                            let bwsh2 = pf_blob_of(t, t.wsh2_off + l * dim * nsh)
                            enc_gemm_mm(enc, bwsh2.buf, bwsh2.boff, bhb, bxb2, u_moe_nsh, u_dim, mp, dim)
                        }
                    }
                    if (!pf_skip_moe_reduce()) {
                        pf_enc_moe_reduce(enc, bmdn, bmsel, bminv, g4moe ? bmg4rt : bxb, u_dim, u_moe_nk, dim, npos)
                        if (nsh > 0l) {
                            // routed sum lands in bxb first — the shexp branch adds σ(gate)-scaled on top
                            pf_enc_axpy_sig(enc, bxb, bxb2, bmlg, u_total_dim, u_dim, npos * dim)
                        }
                    }
                    if (g4moe && g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                        var bw_pf2 = pf_upload_region(unsafe(addr < void? >(t.fblob[t.rms_post_ffn2_off + l * dim])), uint64(dim * 4l))
                        pf_enc_rms(enc, bmg4rt, bw_pf2, bmg4rt, u_dim, u_eps, npos)
                    }
                }
                if ((!moe || g4moe) && g_pf_skip != "gemm") {   // g4: the dense parallel shared branch
                    let f1 = pf_fmt_at(t.w1_fmt, l)
                    let f3 = pf_fmt_at(t.w3_fmt, l)
                    if (f1 != KqFmt.q8 || f3 != KqFmt.q8) {
                        if (f1 != KqFmt.q8) {
                            pf_enc_kq_site_mm(enc, t, f1, t.w1_offs[l], hidden, bxb, bhb, u_dim, u_hidden, mp)
                        } else {
                            let bw1b = pf_blob_of(t, t.w1_offs[l])
                            enc_gemm_mm(enc, bw1b.buf, bw1b.boff, bxb, bhb, u_dim, u_hidden, mp, hidden)
                        }
                        if (f3 != KqFmt.q8) {
                            pf_enc_kq_site_mm(enc, t, f3, t.w3_offs[l], hidden, bxb, bhb2, u_dim, u_hidden, mp)
                        } else {
                            let bw3b = pf_blob_of(t, t.w3_offs[l])
                            enc_gemm_mm(enc, bw3b.buf, bw3b.boff, bxb, bhb2, u_dim, u_hidden, mp, hidden)
                        }
                    } else {
                        let bw1b = pf_blob_of(t, t.w1_offs[l])
                        let bw3b = pf_blob_of(t, t.w3_offs[l])
                        enc_gemm_mm(enc, bw1b.buf, bw1b.boff, bxb, bhb, u_dim, u_hidden, mp, hidden)
                        enc_gemm_mm(enc, bw3b.buf, bw3b.boff, bxb, bhb2, u_dim, u_hidden, mp, hidden)
                    }
                }
                if ((!moe || g4moe) && g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                    pf_enc_ew2(enc, c.ffn_act != FfnAct.silu ? g_pf_pso_geglu : g_pf_pso_swiglu, bhb, bhb2, u_total_h, npos * hidden)
                }
                if ((!moe || g4moe) && g_pf_skip != "gemm") {
                    let f2 = pf_fmt_at(t.w2_fmt, l)
                    if (f2 != KqFmt.q8) {
                        pf_enc_kq_site_mm(enc, t, f2, t.w2_offs[l], dim, bhb, bxb, u_hidden, u_dim, mp)
                    } else {
                        let bw2b = pf_blob_of(t, t.w2_offs[l])
                        enc_gemm_mm(enc, bw2b.buf, bw2b.boff, bhb, bxb, u_hidden, u_dim, mp, dim)
                    }
                }
                if (g4moe && g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                    // dense-branch post-norm, then routed + dense summed into bxb for the epilogue
                    var bw_pf1 = pf_upload_region(unsafe(addr < void? >(t.fblob[t.rms_post_ffn1_off + l * dim])), uint64(dim * 4l))
                    pf_enc_rms(enc, bxb, bw_pf1, bxb, u_dim, u_eps, npos)
                    pf_enc_ew2(enc, g_pf_pso_add, bxb, bmg4rt, u_total_dim, npos * dim)
                }
                if (g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                    if (c.pre_post_norm) {
                        var bw_pf = pf_upload_region(addr < void? >(t.fblob[t.rms_post_ffn_off + l * dim]), uint64(dim * 4l))
                        pf_enc_rms(enc, bxb, bw_pf, bxb, u_dim, u_eps, npos)
                    }
                }
                if (plef) {
                    // E-series PLE between FFN residual and out_scale (the CPU order): plain x_b +=
                    // ffn_branch, then branch = post_norm(proj · (gelu(gate · x_b) ⊙ side[l]))
                    if (g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                        pf_enc_ew2(enc, g_pf_pso_add, bx, bxb, u_total_dim, npos * dim)
                    }
                    if (g_pf_skip != "gemm") {
                        let bwpg = pf_blob_of(t, t.ple_gate_off + l * dim * ple_n)
                        enc_gemm_mm(enc, bwpg.buf, bwpg.boff, bx, bpleg, u_dim, u_ple, mp, ple_n)
                    }
                    if (g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                        pf_enc_ew2(enc, g_pf_pso_geglu, bpleg, bple, u_ple_tot, npos * ple_n,
                            [boff = uint64(l * npos * ple_n * 4l)])
                    }
                    if (g_pf_skip != "gemm") {
                        let bwpp = pf_blob_of(t, t.ple_proj_off + l * ple_n * dim)
                        enc_gemm_mm(enc, bwpp.buf, bwpp.boff, bpleg, bxb, u_ple, u_dim, mp, dim)
                    }
                    if (g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                        var bw_pn = pf_upload_region(addr < void? >(t.fblob[t.ple_post_off + l * dim]), uint64(dim * 4l))
                        pf_enc_rms(enc, bxb, bw_pn, bxb, u_dim, u_eps, npos)
                    }
                }
                if (g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                    // gemma4 layer_out_scale multiplies the WHOLE layer output — (x + branch) * s
                    // folds into the residual add's ascale slot (a 4-byte view into the weights blob)
                    var bosc : MetalBuffer?
                    if (c.layer_out_scale) {
                        bosc = pf_upload_region(addr < void? >(t.fblob[t.out_scale_off + l]), 4ul)
                    }
                    pf_enc_ew2(enc, g_pf_pso_add, bx, bxb, u_total_dim, npos * dim, bosc)
                }
            }
        }
        if (gpu_logits) {
            // final rmsnorm on the LAST position + classifier GEMV — the caller samples from
            // s.logits, so this closes the whole forward inside the GPU window
            unsafe {
                var bwfin = pf_upload_region(addr < void? >(t.fblob[t.rms_final_off]), uint64(dim * 4l))
                enc_rms_last(enc, warm ? bwm_res : bx, uint64((npos - 1l) * dim * 4l), bwfin, bxf, u_dim, u_eps)
                if (cls_fmt != KqFmt.q8) {
                    pf_enc_kq_gemv(enc, t, cls_fmt, t.wcls_off, c.vocab_size, bxf, 0ul, blog, u_dim, u_vocab)
                } else {
                    let bwc = pf_blob_of(t, t.wcls_off)
                    pf_enc_gemv(enc, bwc.buf, bwc.boff, bxf, blog, u_dim, u_vocab, c.vocab_size)
                }
                if (u_flcap != null) {
                    // final-logit soft cap (gemma2): cap*tanh over the classifier row in place
                    metal_set_pipeline(enc, g_pf_pso_softcap)
                    metal_set_buffer(enc, blog, 0ul, 0)
                    metal_set_buffer(enc, u_vocab, 0ul, 1)
                    metal_set_buffer(enc, u_flcap, 0ul, 2)
                    metal_dispatch_threadgroups(enc, uint3(uint((c.vocab_size + 63l) / 64l), 1u, 1u), uint3(64u, 1u, 1u))
                }
                if (bsupp != null) {
                    // gemma4 suppressed ids pin to -1e30 AFTER the cap (the CPU classifier's order)
                    metal_set_pipeline(enc, g_pf_pso_suppress)
                    metal_set_buffer(enc, blog, 0ul, 0)
                    metal_set_buffer(enc, bsupp, 0ul, 1)
                    metal_set_buffer(enc, u_nsupp, 0ul, 2)
                    metal_dispatch_threadgroups(enc, uint3(uint((nsupp + 63l) / 64l), 1u, 1u), uint3(64u, 1u, 1u))
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
                let l1 = min(int64(i + 1) * layers_per_cb, n_enc_layers)
                for (l in range64(int64(i) * layers_per_cb, l1)) {
                    // recurrent: no rows; shared (E-series): the source layer's store covers the alias
                    if (bks[l] == null || kv_shared_layer(t, l)) {
                        continue
                    }
                    // the chunk's rows sit past a continuation's gathered panel head
                    var kpan = reinterpret<uint8?>(metal_buffer_contents(bks[l]))
                    var vpan = reinterpret<uint8?>(metal_buffer_contents(bvs[l]))
                    let kvd_l = layer_kv_dim(c, l)
                    let hs_l = layer_head_size(c, l)
                    let koff_l = start_pos * kvd_l * 4l
                    memcpy(addr < void? >(s.k_b[0]), reinterpret<void?>(kpan + koff_l), int(npos * kvd_l * 4l))
                    memcpy(addr < void? >(s.v_b[0]), reinterpret<void?>(vpan + koff_l), int(npos * kvd_l * 4l))
                    // tq4 sessions store in the ROTATED basis: the GPU ran attention unrotated,
                    // so rotate the readback rows per head before the codec store (the CPU
                    // prefill's exact discipline — dasllama_common's tq4 store path)
                    if (s.kv_dtype_k == KVDtype.tq4 || s.kv_dtype_v == KVDtype.tq4) {
                        let sgp = addr < float const? >(s.kv_signs[0])
                        if (s.kv_dtype_k == KVDtype.tq4) {
                            tq4_rotate_batch(s.k_b, npos, kvd_l, hs_l, sgp)
                        }
                        if (s.kv_dtype_v == KVDtype.tq4) {
                            tq4_rotate_batch(s.v_b, npos, kvd_l, hs_l, sgp)
                        }
                    }
                    kv_store_batch(kv_layer_kbase(s, l, c.seq_len), kv_layer_vbase(s, l, c.seq_len),
                        s.kv_dtype_k, s.kv_dtype_v, s.k_b, s.v_b, runsp, store_nrun, start_pos, npos, kvd_l)
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
        if (dn) {   // the window completed — the GPU state now holds position start_pos+npos-1's update
            dn_mirror_advance(s.uid, start_pos + npos - 1l)
        }
        let ts_rb = ref_time_ticks()
        unsafe {
            memcpy(addr < void? >(s.x_b[0]), metal_buffer_contents(warm ? bwm_res : bx), int(npos * dim * 4l))
            if (gpu_logits) {
                memcpy(addr < void? >(s.logits[0]), metal_buffer_contents(blog), int(c.vocab_size * 4l))
                if (c.n_layer_nextn > 0l) {
                    // forward_prefill's mtp_h contract — bxf holds the last row's post-final-norm hidden
                    memcpy(addr < void? >(s.mtp_h[0]), metal_buffer_contents(bxf), int(dim * 4l))
                }
            }
        }
        if (gpu_logits) {
            if (c.n_layer_nextn > 0l) {
                s.mtp_h_pos1 = start_pos + npos
            }
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
    pool_release(g_pf_pool, bxb, bytes_xb)
    pool_release(g_pf_pool, bxb2, bytes_x)
    pool_release(g_pf_pool, bq, bytes_q)
    pool_release(g_pf_pool, bhb, bytes_h)
    pool_release(g_pf_pool, bhb2, bytes_h)
    if (moe) {
        pool_release(g_pf_pool, bmlg, bytes_mlg)
        pool_release(g_pf_pool, bmsel, bytes_msel)
        pool_release(g_pf_pool, bmcnt, bytes_mcsr)
        pool_release(g_pf_pool, bmbase, bytes_mcsr)
        pool_release(g_pf_pool, bmbkt, bytes_mbkt)
        pool_release(g_pf_pool, bminv, bytes_minv)
        pool_release(g_pf_pool, bmg, bytes_mg)
        pool_release(g_pf_pool, bmg2, bytes_mg)
        pool_release(g_pf_pool, bmdn, bytes_mdn)
        if (g4moe) {
            pool_release(g_pf_pool, bmg4rt, bytes_x)
        }
    }
    if (dn) {
        pool_release(g_pf_pool, bdnqkv, bytes_dn_cd)
        pool_release(g_pf_pool, bdncv, bytes_dn_cd)
        pool_release(g_pf_pool, bdnz, bytes_dn_di)
        pool_release(g_pf_pool, bdno, bytes_dn_di)
        pool_release(g_pf_pool, bdnb, bytes_dn_h)
        pool_release(g_pf_pool, bdng, bytes_dn_h)
        pool_release(g_pf_upool, u_dn_cd, 4ul)
        pool_release(g_pf_upool, u_dn_di, 4ul)
        pool_release(g_pf_upool, u_dn_nvh, 4ul)
        pool_release(g_pf_upool, u_dn_kd, 4ul)
        pool_release(g_pf_upool, u_dn_ds, 4ul)
        pool_release(g_pf_upool, u_dn_nkh, 4ul)
        pool_release(g_pf_upool, u_dn_dconv, 4ul)
        pool_release(g_pf_upool, u_dn_np, 4ul)
        pool_release(g_pf_upool, u_dn_qscale, 4ul)
    }
    if (c.q_gated) {
        pool_release(g_pf_pool, bdnqg, bytes_dn_qg)
        pool_release(g_pf_pool, bdngate, bytes_q)
        pool_release(g_pf_upool, u_dn_qd2, 4ul)
        pool_release(g_pf_upool, u_qtot, 4ul)
    }
    if (u_rot != null) {
        pool_release(g_pf_upool, u_rot, 4ul)
    }
    pool_release(g_pf_pool, batt, bytes_att)
    if (gpu_logits) {
        pool_release(g_pf_pool, bxf, uint64(dim * 4l))
        pool_release(g_pf_pool, blog, uint64(c.vocab_size * 4l))
        pool_release(g_pf_upool, u_vocab, 4ul)
    }
    if (warm) {
        pool_release(g_pf_pool, bwm_res, uint64(npos * dim * 4l))
        pool_release(g_pf_pool, bwm_emb, uint64(npos * dim * 4l))
        pool_release(g_pf_pool, bwm_cat, uint64(mp * 2l * dim * 4l))
        pool_release(g_pf_pool, bwm_tmp, uint64(npos * dim * 4l))
        pool_release(g_pf_upool, u_dim2, 4ul)
        pool_release(g_pf_upool, u_wm_tot, 4ul)
    }
    pool_release(g_pf_upool, bcos, bytes_tab)
    if (dual_rope) {
        pool_release(g_pf_upool, bcos_swa, bytes_tab_sw)
        pool_release(g_pf_upool, bsin_swa, bytes_tab_sw)
    }
    pool_release(g_pf_upool, bsin, bytes_tab)
    for (l in range64(c.n_layers + c.n_layer_nextn)) {
        if (bks[l] == null || kv_shared_layer(t, l)) {   // shared: aliased — the source releases it
            continue
        }
        let bytes_kv_l = uint64(kvrows * layer_kv_dim(c, l) * 4l)
        pool_release(g_pf_pool, bks[l], bytes_kv_l)
        pool_release(g_pf_pool, bvs[l], bytes_kv_l)
    }
    if (plef) {
        if (ple_gpu) {
            pool_release(g_pf_pool, bple, bytes_ple_side)
            pool_release(g_pf_pool, bple_proj, bytes_ple_proj)
            pool_release(g_pf_upool, bple_tok, uint64(npos * 4l))
            pool_release(g_pf_upool, u_ple_np, 4ul)
            pool_release(g_pf_upool, u_ple_nl, 4ul)
            pool_release(g_pf_upool, u_ple_all, 4ul)
            pool_release(g_pf_upool, u_ple_sc0, 4ul)
            pool_release(g_pf_upool, u_ple_isd, 4ul)
            pool_release(g_pf_upool, u_ple_is2, 4ul)
        } else {
            pool_release(g_pf_upool, bple, bytes_ple_side)
        }
        pool_release(g_pf_pool, bpleg, bytes_ple_g)
        pool_release(g_pf_upool, u_ple, 4ul)
        pool_release(g_pf_upool, u_ple_tot, 4ul)
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
    pool_release(g_pf_upool, u_npairs_q, 4ul)
    pool_release(g_pf_upool, u_npairs_k, 4ul)
    pool_release(g_pf_upool, u_neox, 4ul)
    if (has_bias) {
        pool_release(g_pf_upool, u_totq, 4ul)
        pool_release(g_pf_upool, u_totkv, 4ul)
    }
    if (has_qkn) {
        pool_release(g_pf_upool, u_nh_pf, 4ul)
    }
    if (u_zero != null) {
        pool_release(g_pf_upool, u_zero, 4ul)
    }
    if (hetero) {
        pool_release(g_pf_upool, u_qd_sw, 4ul)
        pool_release(g_pf_upool, u_kvd_sw, 4ul)
        pool_release(g_pf_upool, u_hs_sw, 4ul)
        pool_release(g_pf_upool, u_kvm_sw, 4ul)
        pool_release(g_pf_upool, u_scale_sw, 4ul)
        pool_release(g_pf_upool, u_npairs_q_sw, 4ul)
        pool_release(g_pf_upool, u_npairs_k_sw, 4ul)
    }
    if (u_vtot != null) {
        pool_release(g_pf_upool, u_vtot, 4ul)
    }
    if (u_vtot_sw != null) {
        pool_release(g_pf_upool, u_vtot_sw, 4ul)
    }
    if (bsupp != null) {
        pool_release(g_pf_upool, bsupp, uint64(nsupp * 4l))
        pool_release(g_pf_upool, u_nsupp, 4ul)
    }
    pool_release(g_pf_upool, u_total_dim, 4ul)
    pool_release(g_pf_upool, u_total_h, 4ul)
    if (moe) {
        pool_release(g_pf_upool, u_moe_ne, 4ul)
        pool_release(g_pf_upool, u_moe_nk, 4ul)
        pool_release(g_pf_upool, u_moe_np, 4ul)
        pool_release(g_pf_upool, u_moe_tot, 4ul)
        pool_release(g_pf_upool, u_moe_nfe, 4ul)
        pool_release(g_pf_upool, u_moe_eblk, 4ul)
        pool_release(g_pf_upool, u_moe_esb, 4ul)
        pool_release(g_pf_upool, u_moe_g0, 4ul)
        pool_release(g_pf_upool, u_moe_g1, 4ul)
        pool_release(g_pf_upool, u_moe_ptot, 4ul)
        if (g4moe) {
            pool_release(g_pf_upool, u_moe_invd, 4ul)
        }
        pool_release(g_pf_upool, u_moe_gmode, 4ul)
        if (nsh > 0l) {
            pool_release(g_pf_upool, u_moe_nsh, 4ul)
            pool_release(g_pf_upool, u_moe_shtot, 4ul)
            pool_release(g_pf_upool, u_moe_one, 4ul)
        }
    }
    if (u_hass != null) {
        pool_release(g_pf_upool, u_hass, 4ul)
    }
    pool_release(g_pf_upool, u_scale, 4ul)
    pool_release(g_pf_upool, u_eps, 4ul)
    pool_release(g_pf_upool, u_softcap, 4ul)
    pool_release(g_pf_upool, u_win0, 4ul)
    if (u_winsl != null) {
        pool_release(g_pf_upool, u_winsl, 4ul)
    }
    if (u_flcap != null) {
        pool_release(g_pf_upool, u_flcap, 4ul)
    }
    if (ok) {
        g_pf_stage_prefills++
        g_pf_stage_setup_us += int64(setup_us)
        g_pf_stage_encwait_us += int64(encwait_us)
        g_pf_stage_encode_ms += encode_ms
        g_pf_stage_gpu_ms += gpu_ms
        g_pf_stage_rb_us += readback_us
        let total_us = get_time_usec(ts_all)
        to_log(LOG_INFO, "dasLLAMA metal prefill: npos={npos} gemm=mulmm total={float(total_us) / 1000.0}ms setup={float(setup_us) / 1000.0}ms encode+wait={float(encwait_us) / 1000.0}ms (encode {encode_ms}ms, gpu {gpu_ms}ms) kv+readback={float(readback_us) / 1000.0}ms dispatches={17l * c.n_layers + (gpu_logits ? 2l : 0l)} logits={gpu_logits ? "gpu" : "cpu"}\n")
    }
    return ok
}

[init]
def dasllama_metal_prefill_register() {
    // registered but dormant: only select_prefill_override("metal") (env rail
    // DASLLAMA_PIN_PREFILL) activates it, and unsupported shapes decline per call
    register_prefill_override("metal", @@metal_prefill_forward)
    register_ple_gpu_gate(@@metal_ple_pre_gpu_gate)
    if (has_env_variable("DASLLAMA_METAL_PREFILL_SKIP")) {
        g_pf_skip = get_env_variable("DASLLAMA_METAL_PREFILL_SKIP")
        to_log(LOG_WARNING, "dasLLAMA metal prefill: SKIP={g_pf_skip} — timing-attribution mode, output is GARBAGE\n")
    }
}
