options gen2
options _comment_hygiene = true

module dasllama_metal_prefill shared public

require math
require daslib/math_bits   // uint_bits_to_float in the mx4 GEMV (msl_emit as_type)
require dasllama/dasllama_env  // the g_env_* knob globals ([EnvConfig] declarations; ENVIRONMENT.md is generated from them)
require metal/msl_shader
require metal/das_metal_boost
require daslib/typemacro_boost   // [template_struct_instance] — the tensor-twin template below
require dasllama/dasllama_common
require dasllama/dasllama_math
require dasllama/dasllama_metal_common   // decline enums/tables + the shared block_q8_0 blob cache (W1 split)
require dasllama/dasllama_metal_kernels   // the family-shared [metal_kernel] classes both drivers dispatch (M1 move)
require dasllama/dasllama_metal_lens   // [metal_dispatch] builders + census policing (lensed classes live next to their use)

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


// MetalQ8MulMm with the A stage swapped to native BF16 rows (no dequant: widen = exact bit
// shift, then RNE to the f16 tile). Serves the E-series per_layer_model_proj GEMM straight off
// the kept-bf16 blob — no resident f32 copy. Same %64 shape gates, kdim % 32.
[metal_dispatch(name = "pf_enc_bf16_mm_c", pso = "g_pf_pso_bf16_mm", tgmem = "metal_bf16_mulmm_msl_tgmem", tg = 128, grid = "mp/32, nd/64", params = "mp : int64, nd : int64")]
class MetalBf16MulMm : MetalMmTileBase {
    @ssbo @binding = 0 @role = "weight" @off = "wboff" wbh : array<ushort4> // bf16 W rows, ushort4 view (row r elem k at (r*kdim+k)/4)
    @ssbo @binding = 2 @role = "read" xf : array<float4>   // raw f32 activations, float4 view
    @ssbo @binding = 3 @role = "write" y : array<float>
    @uniform @binding = 4 kdim : uint
    @uniform @binding = 5 ndim : uint

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
            acc_quad(aB, bB, ma, mb, mc)
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

// Metal-4 tensor twin of MetalQ8MulMm: the SAME 32x64 tile per threadgroup off the same binds
// (blob bound twice — half-scale + byte views), lowered to one tmm2d_q8b_f32. Raced vs the
// simdgroup kernel by the tuner (runtime.metal_tensor crown "mulmm_q8"); dispatch identical.
[metal_dispatch(name = "enc_gemm_mm_t_c", pso = "g_pf_pso_mm_t", tg = 128, grid = "mp/32, d/64", params = "mp : int64, d : int64")]
class MetalQ8MulMmT {
    @ssbo @binding = 0 @role = "weight" @off = "wboff" wsh : array<float16> // 34B-block W blob, half-scale view
    @ssbo @binding = 1 @role = "weight" @off = "wboff" wqb : array<int8>    // the SAME blob buffer, byte view
    @ssbo @binding = 2 xf : array<float>    // raw f32 activations
    @ssbo @binding = 3 @off = "yoff" @span = "mp*d*4" y : array<float>
    @uniform @binding = 4 kdim : uint
    @uniform @binding = 5 ndim : uint

    [metal_kernel(name="metal_q8_mulmm_t_msl")]
    def metal_q8_mulmm_t {
        let mBase = gl_WorkGroupID.x * 32u
        let nBase = gl_WorkGroupID.y * 64u
        let blk0 = nBase * (kdim / 32u)
        var sp = unsafe(addr(wsh[blk0 * 17u]))
        var wp = unsafe(addr(wqb[blk0 * 34u + 2u]))
        var ap = unsafe(addr(xf[mBase * kdim]))
        var cp = unsafe(addr(y[mBase * ndim + nBase]))
        tmm2d_q8b_f32(32u, 64u, 4u, sp, wp, ap, kdim, cp, ndim, kdim, kdim / 32u)
    }
}

// Metal-4 tensor twin of MetalBf16MulMm: the SAME 32x64 tile per threadgroup off the same
// binds, lowered to mpp matmul2d over tensor_inline views. Raced vs the simdgroup kernel by
// the tuner (runtime.metal_tensor crown "mulmm_bf16"); dispatch shape identical (128 = 4 sg).
[metal_dispatch(name = "pf_enc_bf16_mm_t_c", pso = "g_pf_pso_bf16_mm", tg = 128, grid = "mp/32, nd/64", params = "mp : int64, nd : int64")]
class MetalBf16MulMmT {
    @ssbo @binding = 0 @role = "weight" @off = "wboff" wbh : array<uint16> // bf16 W rows, halfword view
    @ssbo @binding = 2 xf : array<float>   // raw f32 activations
    @ssbo @binding = 3 y : array<float>
    @uniform @binding = 4 kdim : uint
    @uniform @binding = 5 ndim : uint

    [metal_kernel(name="metal_bf16_mulmm_t_msl")]
    def metal_bf16_mulmm_t {
        let mBase = gl_WorkGroupID.x * 32u
        let nBase = gl_WorkGroupID.y * 64u
        var ap = unsafe(addr(xf[mBase * kdim]))
        var wp = unsafe(addr(wbh[nBase * kdim]))
        var cp = unsafe(addr(y[mBase * ndim + nBase]))
        tmm2d_f32_bf16_f32(32u, 64u, 4u, ap, kdim, wp, kdim, cp, ndim, kdim)
    }
}

// The E-series PLE pre-step's two device stages walk the SAME [n_layers x npos x ple] panel, so
// one kargs at slot 4 serves both — the gather reads sc0, the finish reads inv_sd/eps/inv_s2, and
// each ignores the other's. Every field derives from the config, hence the one builder below.
struct PleArgs {
    npos : uint
    ple : uint
    nlayers : uint
    sc0 : float         // sqrt(ple), folded into the gather's dequant
    inv_sd : float      // 1/sqrt(dim)
    eps : float
    inv_s2 : float      // 1/sqrt(2)
}

def private ple_ka(npos, ple, nlayers, dim : int64; eps : float) : PleArgs => (
    PleArgs(npos = uint(npos), ple = uint(ple), nlayers = uint(nlayers), sc0 = sqrt(float(ple)),
        inv_sd = 1.0 / sqrt(float(dim)), eps = eps, inv_s2 = 1.0 / sqrt(2.0)))

// E-series PLE gather (GPU pre-step, stage 1): dequant token row slices of the Q8_0
// per_layer_token_embd region straight into the LAYER-major side-input panel, sqrt(ple) folded.
// One tg per (layer, position) slice; binds ride blob_of's q8 pair at the region base.
[metal_dispatch(name = "pf_enc_ple_gather", pso = "g_pf_pso_ple_gather", tg = "ple", grid = "nlayers, npos", params = "nlayers : int64, npos : int64, ple : int64")]
class MetalPleGatherQ8 {
    @ssbo @binding = 0 @role = "weight" @off = "eoff" wsh : array<float16> // ple_emb q8 region, half-scale view (block b at 17*b)
    @ssbo @binding = 1 @role = "weight" @off = "eoff" wqb : array<int8>    // the SAME region, byte view (quants at 34*b + 2)
    @ssbo @binding = 2 @role = "read" tok : array<uint>    // [npos] token ids
    @ssbo @binding = 3 @role = "write" y : array<float>     // [n_layers x npos x ple] layer-major side input
    @uniform @binding = 4 ka : PleArgs

    [metal_kernel(name="metal_ple_gather_q8_msl")]
    def metal_ple_gather_q8 {
        let l = gl_WorkGroupID.x
        let p = gl_WorkGroupID.y
        let i = gl_LocalInvocationID.x      // tg = ple threads (gate: ple % 32 == 0, <= 1024)
        let el = tok[p] * (ka.nlayers * ka.ple) + l * ka.ple + i
        let blk = el / 32u
        let sc = float(wsh[blk * 17u])
        let q = float(int(wqb[blk * 34u + 2u + (el % 32u)]))
        y[(l * ka.npos + p) * ka.ple + i] = q * sc * ka.sc0
    }
}

// E-series PLE finish (GPU pre-step, stage 2): per (layer, position) ple-slice, mirror of
// ple_pre_finish's tail — v = proj/sqrt(dim); side = (side + pnorm·(v·rsqrt(mean(v²)+eps)))/sqrt(2),
// in place on the layer-major panel the gather wrote. proj arrives position-major off the GEMM.
[metal_dispatch(name = "pf_enc_ple_finish", pso = "g_pf_pso_ple_finish", tgmem = "metal_ple_finish_msl_tgmem", tg = "ple", grid = "nlayers, npos", params = "nlayers : int64, npos : int64, ple : int64")]
class MetalPleFinish {
    @ssbo @binding = 0 @role = "read" proj : array<float>  // [mp x n_layers*ple] position-major model_proj·embd
    @ssbo @binding = 1 @role = "weight" pnorm : array<float> // [ple] proj-norm weight
    @ssbo @binding = 2 @role = "readwrite" y : array<float>     // [n_layers x npos x ple] side input, in place
    @uniform @binding = 4 ka : PleArgs
    @workgroup partial : float[32]
    @workgroup inv_sh : float

    [metal_kernel(name="metal_ple_finish_msl")]
    def metal_ple_finish {
        let l = gl_WorkGroupID.x
        let p = gl_WorkGroupID.y
        let i = gl_LocalInvocationID.x      // tg = ple threads (gate: ple % 32 == 0, <= 1024)
        let v = proj[p * (ka.nlayers * ka.ple) + l * ka.ple + i] * ka.inv_sd
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
            inv_sh = rsqrt(tot / float(ka.ple) + ka.eps)
        }
        barrier()
        let yi = (l * ka.npos + p) * ka.ple + i
        y[yi] = (y[yi] + pnorm[i] * (v * inv_sh)) * ka.inv_s2
    }
}

// ===== K-quant mul_mm twins (format-matrix W4 / M2) — MetalQ8MulMm with the A-tile stage
// swapped to superblock dequant. The 32-elem k-block kb IS sub-block kb%8 of superblock kb/8,
// so each thread's 16-elem half decodes ONE (scale, min) pair. Same %64 shape gates, M padded 32/64.

// Metal-4 staged-tile twins of the KqMulMm K4/K5 pair, one template on the parents' exact axes
// (BLK quant-plane stride, QH overlay): flat f16 tiles per K-chunk, per-element kmask scale
// decode + nibble dequant — same math as the base's va, so staged values are identical.
[ |> template_struct_instance]
class template MetalKqMulMmK45TensorT {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" ksh : array<float16> // 16B scale blocks, half view (d, dmin at 8*blk)
    @ssbo @binding = 1 @role = "weight" @off = "soff" ks4 : array<uint4>   // the same buffer, uint4 view
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // quant plane, uint view (qs at BLK*blk; k5 qh at BLK*blk+32)
    @ssbo @binding = 3 xf : array<float>
    @ssbo @binding = 4 @off = "yoff" @span = "mp*rows*4" y : array<float>
    @uniform @binding = 5 kdim : uint
    @uniform @binding = 6 ndim : uint
    @workgroup twa : float16[1024]          // X chunk: 32 tokens x 32 k, flat row-major
    @workgroup twb : float16[2048]          // W chunk: 64 wrows x 32 k, flat row-major
    @template_constant BLK : uint = 32u
    @template_constant QH : bool = false

    [metal_kernel]
    def metal_kq_mulmm_t {
        let mBase = gl_WorkGroupID.x * 32u
        let nBase = gl_WorkGroupID.y * 64u
        let nkb = kdim / 32u
        let nsb = kdim / 256u
        let lid = gl_LocalInvocationID.x
        var acc : float[2048]
        var cp = unsafe(addr(y[mBase * ndim + nBase]))
        tmm2d_tg_begin(acc, 32u, 64u, 4u, 32u)
        var kb = 0u
        while (kb < nkb) {
            var i = lid
            while (i < 1024u) {
                twa[i] = float16(xf[(mBase + i / 32u) * kdim + kb * 32u + i % 32u])
                i += gl_WorkGroupSize.x
            }
            let sb = kb / 8u
            let js = kb % 8u
            i = lid
            while (i < 2048u) {
                let j = i / 32u
                let e = i % 32u
                let blk = (nBase + j) * nsb + sb
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
                static_if (QH) {
                    let u = kqu[blk * BLK + (js / 2u) * 8u + e / 4u]
                    let hu = kqu[blk * BLK + 32u + e / 4u]
                    let q = ((u >> (8u * (e % 4u) + (js & 1u) * 4u)) & 15u) | (((hu >> (8u * (e % 4u) + js)) & 1u) << 4u)
                    twb[i] = float16(dsc * float(q) - dmn)
                } else {
                    let q = (kqu[blk * BLK + (js / 2u) * 8u + e / 4u] >> (8u * (e % 4u) + (js & 1u) * 4u)) & 15u
                    twb[i] = float16(dsc * float(q) - dmn)
                }
                i += gl_WorkGroupSize.x
            }
            barrier()
            tmm2d_tg_step(acc, twa, twb, 32u, 64u, 32u)
            barrier()
            kb++
        }
        tmm2d_tg_store(acc, cp, 32u, 64u, ndim)
    }
}

[metal_dispatch(name = "enc_kq_mm_k4_t_c", pso = "g_pf_pso_kq_mm4_t", tgmem = "MetalKqMulMmK4T_metal_kq_mulmm_t_msl_tgmem", tg = 128, grid = "mp/32, rows/64", params = "mp : int64, rows : int64")]
class MetalKqMulMmK4T : MetalKqMulMmK45TensorT {
}

[metal_dispatch(name = "enc_kq_mm_k5_t_c", pso = "g_pf_pso_kq_mm5_t", tgmem = "MetalKqMulMmK5T_metal_kq_mulmm_t_msl_tgmem", tg = 128, grid = "mp/32, rows/64", params = "mp : int64, rows : int64")]
class MetalKqMulMmK5T : MetalKqMulMmK45TensorT {
    override BLK = 40u
    override QH = true
}

// Metal-4 staged-tile twin of MetalKqMulMmK6 (per-element ql|qh compose x signed sub-scale —
// same real product as the base's pre-scaled fma form, which is bit-identical by construction).
[metal_dispatch(name = "enc_kq_mm_k6_t_c", pso = "g_pf_pso_kq_mm6_t", tgmem = "metal_kq_mulmm_k6_t_msl_tgmem", tg = 128, grid = "mp/32, rows/64", params = "mp : int64, rows : int64")]
class MetalKqMulMmK6T {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16> // d plane (scale buffer at byte nsb*16; s0off = the caller's doff)
    @ssbo @binding = 1 @role = "weight" @off = "soff" ksc : array<uint4>   // 16B sub-scale blocks, same buffer at 0
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    // ql at 48*blk, qh at 48*blk+32
    @ssbo @binding = 3 xf : array<float>
    @ssbo @binding = 4 @off = "yoff" @span = "mp*rows*4" y : array<float>
    @uniform @binding = 5 kdim : uint
    @uniform @binding = 6 ndim : uint
    @workgroup twa : float16[1024]
    @workgroup twb : float16[2048]

    [metal_kernel(name="metal_kq_mulmm_k6_t_msl")]
    def metal_kq_mulmm_k6_t {
        let mBase = gl_WorkGroupID.x * 32u
        let nBase = gl_WorkGroupID.y * 64u
        let nkb = kdim / 32u
        let nsb = kdim / 256u
        let lid = gl_LocalInvocationID.x
        var acc : float[2048]
        var cp = unsafe(addr(y[mBase * ndim + nBase]))
        tmm2d_tg_begin(acc, 32u, 64u, 4u, 32u)
        var kb = 0u
        while (kb < nkb) {
            var i = lid
            while (i < 1024u) {
                twa[i] = float16(xf[(mBase + i / 32u) * kdim + kb * 32u + i % 32u])
                i += gl_WorkGroupSize.x
            }
            let sb = kb / 8u
            let js = kb % 8u
            let half6 = js / 4u
            let gg = js % 4u
            let nsh = (gg / 2u) * 4u
            let hsh = gg * 2u
            i = lid
            while (i < 2048u) {
                let j = i / 32u
                let e = i % 32u
                let blk = (nBase + j) * nsb + sb
                let dall = float(kdh[blk])
                let ile = e / 16u
                let k = (e % 16u) / 4u
                let c = e % 4u
                let si = half6 * 8u + gg * 2u + ile
                let sv = ksc[blk]
                let scw = si < 8u ? (si < 4u ? sv.x : sv.y) : (si < 12u ? sv.z : sv.w)
                let s6 = float(((int(scw >> ((si & 3u) * 8u)) & 255) ^ 128) - 128)
                let dsc = dall * s6
                let u = kqu[blk * 48u + half6 * 16u + (gg & 1u) * 8u + ile * 4u + k]
                let hu = kqu[blk * 48u + 32u + half6 * 8u + ile * 4u + k]
                let q = ((u >> (nsh + 8u * c)) & 15u) | (((hu >> (hsh + 8u * c)) & 3u) << 4u)
                twb[i] = float16(dsc * float(q) - dsc * 32.0)
                i += gl_WorkGroupSize.x
            }
            barrier()
            tmm2d_tg_step(acc, twa, twb, 32u, 64u, 32u)
            barrier()
            kb++
        }
        tmm2d_tg_store(acc, cp, 32u, 64u, ndim)
    }
}

// ===== MoE routing (Wave C) =====
// Router GEMV -> single-simdgroup select -> expert-indexed GEMV twins (grid.y = slot; the
// kernel reads sel[slot] GPU-side — no CPU readback, encode-ahead/spec stays compatible).

// The position-blocked prefill router (MetalMoeRouterB) lives in dasllama_metal_kernels as the
// BATCHED NR=8 stamp of the MetalMoeRouterT template — same k stride, logits bit-identical.

// The MoE gathered mul_mm family (mx4/q8 plus their tensor twins, and k4/k5/k6/q51) binds its
// uniforms as ONE kargs value at slot 5 — each form ignores the fields it does not read: a tensor
// twin serves contiguous sites only, and only the mx4 pair carries a per-expert bias.
struct MoeMmArgs {
    kdim : uint
    ndim : uint
    eplane : uint       // per-expert plane stride, in the format's own block unit (32 or 256 elems)
    nk : uint
    gather : uint       // 1 = bkt-indirect X rows (gate/up), 0 = contiguous bucket rows (down)
    hasb : uint
}

// the plane IS kdim*ndim elements, so its block stride derives from the shape — no separate input
def private moe_mm_ka(fmt : KqFmt; kdim, ndim, nk : int64; gather : bool) : MoeMmArgs => (
    MoeMmArgs(kdim = uint(kdim), ndim = uint(ndim),
        eplane = uint(kdim * ndim / (kq_sb(fmt) ? 256l : 32l)),
        nk = uint(nk), gather = gather ? 1u : 0u))

def private moe_mm_mx4_ka(kdim, ndim, nk : int64; gather, hasb : bool) : MoeMmArgs => (
    MoeMmArgs(kdim = uint(kdim), ndim = uint(ndim), eplane = uint(kdim * ndim / 32l),
        nk = uint(nk), gather = gather ? 1u : 0u, hasb = hasb ? 1u : 0u))

// Metal-4 staged-tile twin of MetalMoeMulMmMx4 — CONTIGUOUS (down-site) shape: flat f16 tiles
// per K-chunk (per-element LUT dequant), bias folded as one extra rank-1 chunk (f16-staged —
// parity-tolerance vs the base's f32 seed).
[metal_dispatch(name = "pf_enc_moe_mm_mx4_t_c", pso = "g_pf_pso_moe_mm_mx4_t", tgmem = "metal_moe_mulmm_mx4_t_msl_tgmem", tg = 128, grid = "npos/32, rows/64, ne", params = "rows : int64, npos : int64, ne : int64")]
class MetalMoeMulMmMx4T {
    @ssbo @binding = 0 @role = "weight" @off = "qoff" mxq4 : array<uint4>  // nibble plane, uint4 view
    @ssbo @binding = 1 @role = "weight" @off = "soff" mxe : array<uint8>   // e8m0 scale plane
    @ssbo @binding = 2 xf : array<float>
    @ssbo @binding = 3 y : array<float>
    @uniform @binding = 5 ka : MoeMmArgs
    @ssbo @binding = 6 cnt : array<uint>
    @ssbo @binding = 7 basep : array<uint>
    @ssbo @binding = 12 @role = "weight" wb : array<float>   // [ne x ndim] per-expert bias rows
    @ssbo @binding = 13 @role = "weight" vtab : array<float> // [16] signed doubled-e2m1 values
    @workgroup twa : float16[1024]          // X chunk: 32 tokens x 32 k, flat row-major
    @workgroup twb : float16[2048]          // W chunk: 64 wrows x 32 k, flat row-major

    [metal_kernel(name="metal_moe_mulmm_mx4_t_msl")]
    def metal_moe_mulmm_mx4_t {
        let e = gl_WorkGroupID.z
        let ce = cnt[e]
        let mBase = gl_WorkGroupID.x * 32u
        if (mBase >= (ce + 31u) / 32u * 32u) {   // threadgroup-uniform exit — cooperative-safe
            return
        }
        let rbase = basep[e]
        let nBase = gl_WorkGroupID.y * 64u
        let nkb = ka.kdim / 32u
        let lid = gl_LocalInvocationID.x
        var acc : float[2048]
        var cp = unsafe(addr(y[(rbase + mBase) * ka.ndim + nBase]))
        tmm2d_tg_begin(acc, 32u, 64u, 4u, 32u)
        var kb = 0u
        while (kb < nkb) {
            var i = lid
            while (i < 1024u) {
                twa[i] = float16(xf[(rbase + mBase + i / 32u) * ka.kdim + kb * 32u + i % 32u])
                i += gl_WorkGroupSize.x
            }
            i = lid
            while (i < 2048u) {
                let j = i / 32u
                let k = i % 32u
                let blk = e * ka.eplane + (nBase + j) * nkb + kb
                let sc = uint(mxe[blk])
                let d = uint_bits_to_float(sc < 2u ? (0x00200000u << sc) : ((sc - 1u) << 23u))
                let byi = k % 16u
                let u4 = mxq4[blk]
                let w32 = byi < 4u ? u4.x : (byi < 8u ? u4.y : (byi < 12u ? u4.z : u4.w))
                let nby = (w32 >> ((byi % 4u) * 8u)) & 255u
                twb[i] = float16(vtab[int(k < 16u ? (nby & 15u) : (nby >> 4u))] * d)
                i += gl_WorkGroupSize.x
            }
            barrier()
            tmm2d_tg_step(acc, twa, twb, 32u, 64u, 32u)
            barrier()
            kb++
        }
        if (ka.hasb != 0u) {
            // rank-1 bias chunk: A col 0 = 1, W col 0 = wb[e][col]
            var i = lid
            while (i < 1024u) {
                twa[i] = float16(i % 32u == 0u ? 1.0 : 0.0)
                i += gl_WorkGroupSize.x
            }
            i = lid
            while (i < 2048u) {
                twb[i] = float16(i % 32u == 0u ? wb[e * ka.ndim + nBase + i / 32u] : 0.0)
                i += gl_WorkGroupSize.x
            }
            barrier()
            tmm2d_tg_step(acc, twa, twb, 32u, 64u, 32u)
            barrier()
        }
        tmm2d_tg_store(acc, cp, 32u, 64u, ka.ndim)
    }
}

// the prefill twin over the padded bucket panel: MetalSwigluOaiT's PF stamp — same math, plus
// the PF-gated liveness exit over the bucket fields (slots 3-6).
[metal_dispatch(name = "pf_enc_swiglu_oai", pso = "g_pf_pso_swiglu_oai", tg = 64, grid = "total/256", params = "total : int64")]
class MetalSwigluOaiPf : MetalSwigluOaiT {
    override PF = true
}

// ===== MoE prefill CSR + gathered mul_mm (Wave C stage 2b) =====
// Atomics-free CSR: count (tg/expert) -> serial prefix (1 tg) -> bucket fill (tg/expert,
// thread-0 serial walk in row order = the CPU moe_bkt order); buckets PAD to 32-row tiles.

[metal_dispatch(name = "pf_enc_moe_count", pso = "g_pf_pso_moe_count", tg = 32, grid = "ne", params = "ne : int64")]
class MetalMoeCount {
    @ssbo @binding = 0 @role = "read" seli : array<uint>   // [npos x 2k] selection rows (idx section)
    @ssbo @binding = 1 @role = "write" cnt : array<uint>    // [ne] out
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

[metal_dispatch(name = "pf_enc_moe_bucket", pso = "g_pf_pso_moe_bucket", tg = 32, grid = "nex", params = "nex : int64")]
class MetalMoeBucket {
    @ssbo @binding = 0 @role = "read" seli : array<uint>
    @ssbo @binding = 1 @role = "write" basep : array<uint>  // [ne] exclusive prefix over the PADDED counts
    @ssbo @binding = 2 @role = "write" bkt : array<uint>    // [padded rows] bucket -> entry (row*k + slot)
    @ssbo @binding = 3 @role = "write" inv : array<uint>    // [npos*k] entry -> padded bucket row
    @uniform @binding = 4 total : uint
    @uniform @binding = 5 nk : uint
    @ssbo @binding = 6 @role = "read" cnt : array<uint>
    @uniform @binding = 7 ne : uint

    [metal_kernel(name="metal_moe_bucket_msl")]
    def metal_moe_bucket {
        // contiguous ascending per-lane chunks + exclusive scan of chunk counts reproduce the
        // serial entry order exactly (a 1-lane scan here was ~0.7ms/layer of load latency)
        let e = gl_WorkGroupID.x
        let lane = gl_SubgroupInvocationID
        // in-tg fold of the old 1-tg prefix dispatch: masked padded sum below this expert
        // (identical value in every tg; tg e publishes basep[e] for the mm/act consumers)
        var pacc = 0u
        var pt = lane
        while (pt < ne) {
            if (pt < e) {
                pacc += (cnt[pt] + 31u) / 32u * 32u
            }
            pt += 32u
        }
        var pm = 16u
        while (pm >= 1u) {
            pacc += simd_shuffle_xor(pacc, pm)
            pm /= 2u
        }
        if (lane == 0u) {
            basep[e] = pacc
        }
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
        var b = pacc + n - mine
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
[metal_dispatch(name = "pf_enc_moe_reduce", pso = "g_pf_pso_moe_reduce", tg = 256, grid = "dim/256, npos", params = "dim : int64, npos : int64")]
class MetalMoeReduce {
    @ssbo @binding = 0 @role = "read" dn : array<float>    // [padded rows x dim] bucket-ordered down outputs
    @ssbo @binding = 1 @role = "read" selw : array<float>  // selection rows, float view (w of row r at r*2k + k)
    @ssbo @binding = 2 @role = "read" inv : array<uint>
    @ssbo @binding = 3 @role = "write" y : array<float>     // [npos x dim]
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

// Metal-4 tensor twin of MetalMoeMulMmQ8 — the CONTIGUOUS (gather=0, down-site) shape only:
// bkt-indirect X rows cannot form a strided tensor view, so gate/up keep the simdgroup kernel
// even when "moe_mulmm_q8" is crowned (the encode picks per dispatch).
[metal_dispatch(name = "pf_enc_moe_mm_q8_t_c", pso = "g_pf_pso_moe_mm_q8_t", tg = 128, grid = "npos/32, rows/64, ne", params = "rows : int64, npos : int64, ne : int64")]
class MetalMoeMulMmQ8T {
    @ssbo @binding = 0 @role = "weight" @off = "wboff" wsh : array<float16> // expert STACK blob, half-scale view
    @ssbo @binding = 1 @role = "weight" @off = "wboff" wqb : array<int8>    // the same blob buffer, byte view
    @ssbo @binding = 2 xf : array<float>
    @ssbo @binding = 3 y : array<float>
    @uniform @binding = 5 ka : MoeMmArgs
    @ssbo @binding = 6 cnt : array<uint>
    @ssbo @binding = 7 basep : array<uint>

    [metal_kernel(name="metal_moe_mulmm_q8_t_msl")]
    def metal_moe_mulmm_q8_t {
        let e = gl_WorkGroupID.z
        let ce = cnt[e]
        let mBase = gl_WorkGroupID.x * 32u
        if (mBase >= (ce + 31u) / 32u * 32u) {   // threadgroup-uniform exit — cooperative-safe
            return
        }
        let rbase = basep[e]
        let nBase = gl_WorkGroupID.y * 64u
        let blk0 = e * ka.eplane + nBase * (ka.kdim / 32u)
        var sp = unsafe(addr(wsh[blk0 * 17u]))
        var wp = unsafe(addr(wqb[blk0 * 34u + 2u]))
        var ap = unsafe(addr(xf[(rbase + mBase) * ka.kdim]))
        var cp = unsafe(addr(y[(rbase + mBase) * ka.ndim + nBase]))
        tmm2d_q8b_f32(32u, 64u, 4u, sp, wp, ap, ka.kdim, cp, ka.ndim, ka.kdim, ka.kdim / 32u)
    }
}

// The MoE mul_mm family: one 32x64 simdgroup tile loop over an expert's rows, identical for every
// weight format — a format differs in its k-block decode, the expert-plane origin it addresses
// from, and (q8) the carried-pointer state stage_init binds. Mx4 stays out: followup_general.md #8.
class MetalMoeMulMmBase : MetalMmTileBase {
    @ssbo @binding = 3 @role = "read" xf : array<float4>
    @ssbo @binding = 4 @role = "write" y : array<float>
    @uniform @binding = 5 ka : MoeMmArgs
    @ssbo @binding = 7 @role = "read" cnt : array<uint>
    @ssbo @binding = 8 @role = "read" basep : array<uint>
    @ssbo @binding = 9 @role = "read" bkt : array<uint>

    // a format owns bindings 0..2 — its weight views; its plane stride rides ka.eplane
    def abstract wbase(e, nBase, lr0 : uint) : uint    // expert plane origin, in the format's block unit
    def abstract stage_a(wb, kb, il0, tA0 : uint) : void   // decode one k-block into the A tile

    def stage_init(wb, il0 : uint) {}   // format prologue: bind carried-pointer state (q8's walk)

    def stage_acc(var acc : simdgroup_float8x8[8]) {}   // accumulator seed hook (mx4's per-expert bias)

    def run {
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
        stage_acc(mc)
        var ma : simdgroup_half8x8[4]
        var mb : simdgroup_half8x8[2]
        let kdim4 = ka.kdim / 4u
        let nkb = ka.kdim / 32u
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
        var srcrow = rbase + mBase + lr1
        if (ka.gather != 0u) {
            srcrow = bkt[rbase + min(mBase + lr1, ce - 1u)] / ka.nk
        }
        var xcur = unsafe(addr(xf[srcrow * kdim4 + iy / 4u]))
        var kb = 0u
        let wb = wbase(e, nBase, lr0)
        stage_init(wb, il0)
        while (kb < nkb) {
            let b0 = unsafe(xcur[0])
            let b1 = unsafe(xcur[1])
            stage_a(wb, kb, il0, tA0)
            tg_store_half4(tb, ibB, b0)
            tg_store_half4(tb, ibB + 4u, b1)
            barrier()
            acc_quad(aB, bB, ma, mb, mc)
            xcur = unsafe(xcur + 8)
            kb++
        }
        let row0 = rbase + mBase + (sg / 2u) * 16u
        let col0 = nBase + (sg % 2u) * 32u
        for [unroll_full] (i in range(8)) {
            simdgroup_store(mc[i], y, (row0 + uint(i / 4) * 8u) * ka.ndim + col0 + uint(i % 4) * 8u, ka.ndim)
        }
    }
}

// Gathered q8 mul_mm — the family base with the CARRIED-POINTER A stage: stage_init binds the
// scale/quant walk once, stage_a advances (+17 halves / +34 bytes per k-block; the stateless
// index form measured +3.4-3.6% in the gmm8 lab — the pointer walk IS the win).
[metal_dispatch(name = "pf_enc_moe_mm_q8_c", pso = "g_pf_pso_moe_mm_q8", tgmem = "metal_moe_mulmm_q8_msl_tgmem", tg = 128, grid = "npos/32, rows/64, ne", params = "rows : int64, npos : int64, ne : int64")]
class MetalMoeMulMmQ8 : MetalMoeMulMmBase {
    @ssbo @binding = 0 @role = "weight" @off = "wboff" wsh : array<float16> // the expert STACK's blob slice, half-scale view
    @ssbo @binding = 1 @role = "weight" @off = "wboff" wqb : array<int8>    // the same blob buffer, byte view
    scur : float16 const?   // carried scale pointer
    qp : int8 const?        // carried quant pointer

    def override wbase(e, nBase, lr0 : uint) : uint => e * ka.eplane + (nBase + lr0) * (ka.kdim / 32u)

    def override stage_init(wb, il0 : uint) {
        scur = unsafe(addr(wsh[wb * 17u]))
        qp = unsafe(addr(wqb[wb * 34u + 2u + il0 * 16u]))
    }

    def override stage_a(wb, kb, il0, tA0 : uint) {
        var va : float16[16]
        let sA = float(unsafe(scur[0]))
        for [unroll_full] (i in range(16)) {
            va[i] = float16(float(unsafe(qp[i])) * sA)
        }
        barrier()
        for [unroll_full] (i in range(16)) {
            ta[tA0 + uint(i / 8) * 512u + uint(i % 8) * 8u] = va[i]
        }
        scur = unsafe(scur + 17)
        qp = unsafe(qp + 34)
    }

    [metal_kernel(name="metal_moe_mulmm_q8_msl")]
    def metal_moe_mulmm_q8 {
        run()
    }
}

// Gathered Q4_K mul_mm — MetalKqMulMmK4's A stage with the expert superblock shift.
[metal_dispatch(name = "pf_enc_moe_mm_k4_c", pso = "g_pf_pso_moe_mm_k4", tgmem = "metal_moe_mulmm_k4_msl_tgmem", tg = 128, grid = "npos/32, rows/64, ne", params = "rows : int64, npos : int64, ne : int64")]
class MetalMoeMulMmK4 : MetalMoeMulMmBase {
    @ssbo @binding = 0 @role = "weight" @off = "soff" ksh : array<float16>
    @ssbo @binding = 1 @role = "weight" @off = "soff" ks4 : array<uint4>
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>

    def override wbase(e, nBase, lr0 : uint) : uint => e * ka.eplane + (nBase + lr0) * (ka.kdim / 256u)

    def override stage_a(wb, kb, il0, tA0 : uint) {
        var va : float16[16]
        let sb = kb / 8u
        let js = kb % 8u
        let blk = wb + sb
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
    }

    [metal_kernel(name="metal_moe_mulmm_k4_msl")]
    def metal_moe_mulmm_k4 {
        run()
    }
}

// Gathered Q5_K mul_mm — MetalKqMulMmK5's A stage with the expert superblock shift.
[metal_dispatch(name = "pf_enc_moe_mm_k5_c", pso = "g_pf_pso_moe_mm_k5", tgmem = "metal_moe_mulmm_k5_msl_tgmem", tg = 128, grid = "npos/32, rows/64, ne", params = "rows : int64, npos : int64, ne : int64")]
class MetalMoeMulMmK5 : MetalMoeMulMmBase {
    @ssbo @binding = 0 @role = "weight" @off = "soff" ksh : array<float16>
    @ssbo @binding = 1 @role = "weight" @off = "soff" ks4 : array<uint4>
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>

    def override wbase(e, nBase, lr0 : uint) : uint => e * ka.eplane + (nBase + lr0) * (ka.kdim / 256u)

    def override stage_a(wb, kb, il0, tA0 : uint) {
        var va : float16[16]
        let sb = kb / 8u
        let js = kb % 8u
        let blk = wb + sb
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
    }

    [metal_kernel(name="metal_moe_mulmm_k5_msl")]
    def metal_moe_mulmm_k5 {
        run()
    }
}

// Gathered Q5_1 mul_mm — the k5 skeleton with the per-32 A stage: each thread's 16-elem half is
// one nibble half of the 32-block (il0), value = d*q + m folded into the f16 A tile.
[metal_dispatch(name = "pf_enc_moe_mm_q51_c", pso = "g_pf_pso_moe_mm_q51", tgmem = "metal_moe_mulmm_q51_msl_tgmem", tg = 128, grid = "npos/32, rows/64, ne", params = "rows : int64, npos : int64, ne : int64")]
class MetalMoeMulMmQ51 : MetalMoeMulMmBase {
    @ssbo @binding = 0 @role = "weight" @off = "soff" wsh : array<float16> // scale plane (d at 2*blk, m at 2*blk+1)
    @ssbo @binding = 1 @role = "weight" @off = "qoff" wqu : array<uint>    // quant plane, uint view (block at 5*blk; qh at +4)

    def override wbase(e, nBase, lr0 : uint) : uint => e * ka.eplane + (nBase + lr0) * (ka.kdim / 32u)

    def override stage_a(wb, kb, il0, tA0 : uint) {
        var va : float16[16]
        let nsh = il0 * 4u
        let hb0 = il0 * 16u
        let blk = wb + kb
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
    }

    [metal_kernel(name="metal_moe_mulmm_q51_msl")]
    def metal_moe_mulmm_q51 {
        run()
    }
}

// Gathered Q6_K mul_mm — the k6 A stage on the family base. The superblock-scalar cache the
// standalone kernel carried measured SLOWER than reload-per-kb (gmm6 lab, 3 launches: −2.4%
// ms/mm at qwen3moe pp512 shapes), so the stateless stage_a is both the join and the win.
[metal_dispatch(name = "pf_enc_moe_mm_k6_c", pso = "g_pf_pso_moe_mm_k6", tgmem = "metal_moe_mulmm_k6_msl_tgmem", tg = 128, grid = "npos/32, rows/64, ne", params = "rows : int64, npos : int64, ne : int64")]
class MetalMoeMulMmK6 : MetalMoeMulMmBase {
    @ssbo @binding = 0 @role = "weight" @off = "doff" kdh : array<float16>
    @ssbo @binding = 1 @role = "weight" @off = "soff" ksc : array<uint4>
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>

    def override wbase(e, nBase, lr0 : uint) : uint => e * ka.eplane + (nBase + lr0) * (ka.kdim / 256u)

    def override stage_a(wb, kb, il0, tA0 : uint) {
        var va : float16[16]
        let sb = kb / 8u
        let js = kb % 8u
        let blk = wb + sb
        let sv = ksc[blk]
        let dall = float(kdh[blk])
        let half6 = js / 4u
        let gg = js % 4u
        let si = half6 * 8u + gg * 2u + il0
        let scw = si < 8u ? (si < 4u ? sv.x : sv.y) : (si < 12u ? sv.z : sv.w)
        let s6 = float(((int(scw >> ((si & 3u) * 8u)) & 255) ^ 128) - 128)
        let dsc = dall * s6
        let dmn = dsc * 32.0
        // shift folded into exact power-of-two pre-scales — see MetalKqMulMmK45T
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
    }

    [metal_kernel(name="metal_moe_mulmm_k6_msl")]
    def metal_moe_mulmm_k6 {
        run()
    }
}

// Gathered MXFP4 mul_mm — the family base with the mx4 A stage: each thread's 16-elem half IS
// one nibble half of the 32-block (il0 = 0 low, 1 high); stage_init stages the doubled-e2m1
// value table, stage_acc seeds per-expert bias (hasb; ggml_add_id order, bias precedes reduce).
[metal_dispatch(name = "pf_enc_moe_mm_mx4_c", pso = "g_pf_pso_moe_mm_mx4", tgmem = "metal_moe_mulmm_mx4_msl_tgmem", tg = 128, grid = "npos/32, rows/64, ne", params = "rows : int64, npos : int64, ne : int64")]
class MetalMoeMulMmMx4 : MetalMoeMulMmBase {
    @ssbo @binding = 0 @role = "weight" @off = "qoff" mxq4 : array<uint4>  // nibble plane, uint4 view (one block = one load)
    @ssbo @binding = 1 @role = "weight" @off = "soff" mxe : array<uint8>
    @ssbo @binding = 12 @role = "weight" wbias : array<float> // [ne x ndim] per-expert bias rows (hasb models)
    @ssbo @binding = 13 @role = "weight" vtab : array<float>  // [16] signed doubled-e2m1 values (host-built)
    @workgroup vt : float[32]

    def override wbase(e, nBase, lr0 : uint) : uint => e * ka.eplane + (nBase + lr0) * (ka.kdim / 32u)

    def override stage_init(wb, il0 : uint) {
        let lid = gl_LocalInvocationID.x
        if (lid < 32u) {
            vt[lid] = vtab[lid % 16u]
        }
        barrier()
    }

    def override stage_acc(var acc : simdgroup_float8x8[8]) {
        if (ka.hasb != 0u) {
            // bias as the accumulator seed: every row of tile i gets wbias[e][col] (stride-0
            // load); padded ghost rows are harmlessly biased — never read
            let e = gl_WorkGroupID.z
            let sg = gl_SubgroupID
            let wb0 = e * ka.ndim + gl_WorkGroupID.y * 64u + (sg % 2u) * 32u
            for [unroll_full] (i in range(8)) {
                simdgroup_load(acc[i], wbias, wb0 + uint(i % 4) * 8u, 0u)
            }
        }
    }

    def override stage_a(wb, kb, il0, tA0 : uint) {
        var va : float16[16]
        let blk = wb + kb
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
    }

    [metal_kernel(name="metal_moe_mulmm_mx4_msl")]
    def metal_moe_mulmm_mx4 {
        run()
    }
}

// ===== K-quant site GEMVs (format-matrix W4) — the gemv-lab winners, productionized =====
// f32-X single-row weight streams over DISK-ORDER kq quant planes + GPU-form scale buffers:
// k4/k5 read 16B compact scale blocks, k6 reads 16B sub-scale + a trailing packed f16 d plane.

// ===== K-quant small-batch mv twins (the kq ext form) — batched decode sites at B = 2..8 =====
// The gemv-lab ext winners productionized (-25..-40% vs the per-stream B x B1 wall at B=4). One
// thread per output row; y binds at the site's column byte offset, grid.y = column groups of R.

// ===== K-quant B=5..8 single-pass twins (lab extx8 form): ONE weight pass vs two B4 groups; x panel
// STAGED through tgmem per superblock (0.45 -> 0.317 ms; the SLC-resident lab can't rank this — in-graph
// A/B decides, DASLLAMA_METAL_KQ_B8=0 falls back). REQUIRES mp == 8 (nlive >= 5): stage reads all 8 rows, stores gate on nr.

// One thread per rotation pair: NORM (2j, 2j+1 within each head) or NEOX (j, j+half) per the
// mode uniform, matching rope_scaled_tab / rope_scaled_neox_tab exactly via the position's
// precomputed table row. One kernel serves q (n = qd) and k (n = kv_dim) as two dispatches.
[metal_dispatch(name = "enc_rope", pso = "g_pf_pso_rope", tg = 64, grid = "npairs/64", params = "npairs : int64")]
class MetalRope {
    @ssbo @binding = 0 @role = "readwrite" @off = "voff" v : array<float>     // [ntok x n], roped in place
    @ssbo @binding = 1 @role = "weight" tcos : array<float>  // [ntok x head_size/2]
    @ssbo @binding = 2 @role = "weight" tsin : array<float>
    @uniform @binding = 3 n : uint          // row width (qd or kv_dim), % head_size
    @uniform @binding = 4 head_size : uint
    @uniform @binding = 5 npairs : uint     // ntok * n/2 — grid guard
    @uniform @binding = 6 neox : uint       // pair (j, j+half) instead of (2j, 2j+1); same table row j
    @uniform @binding = 7 @default = head_size rot : uint   // partial rotary width; null bind = full rope (== head_size)

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
[metal_dispatch(name = "enc_add_bias_rows", pso = "g_pf_pso_addbias", tg = 64, grid = "total/64", params = "total : int64")]
class MetalAddBiasRows {
    @ssbo @binding = 0 @role = "readwrite" @off = "off" @span = "total*4" x : array<float>     // [nrows x n], updated in place (bound at the panel offset)
    @ssbo @binding = 1 @role = "weight" @off = "boff" b : array<float>     // [n] (the tower driver binds biases inside the model blob)
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

// Attention runs as a tiled-GEMM trio over a per-head score slab PADDED to 32-row blocks (np32 =
// ceil32(npos)): metal_attn_qk (S = scale*Q.K^T) -> metal_attn_softmax (causal, zeroes the tail)
// -> metal_attn_av (O = P.V). Replaces a naive per-row pair that owned 44% of the prefill's GPU time.

// One layer's attention geometry as ONE kargs value, shared by all THREE stages and by each
// stage's mm/tensor twins — the trio walks the same slab, so the caller derives this once per
// layer. Stages ignore the fields they don't read (AV has no scale/window, QK no softcap/hass).
struct AttnArgs {
    qd : uint
    kv_dim : uint
    head_size : uint
    kv_mul : uint
    npos : uint         // KEY rows (continuation: qoff existing + the chunk)
    np32 : uint         // padded key extent = the score slab's column stride
    scale : float
    qoff : uint         // the chunk's first GLOBAL position (0 = fresh start)
    qrows : uint        // padded QUERY rows — the score slab's row extent
    window : uint       // > 0 = sliding span (this layer); 0 = global
    softcap : float     // > 0 = attention-logit soft cap (gemma2)
    hass : uint
    uend : uint         // > 0 = uniform span end (media rows): every query attends [window floor, uend); 0 = causal
}

// One threadgroup (128 threads = 4 simdgroups) per 32x32 score block; blocks fully above the
// causal diagonal exit early. Q staging folds `scale` in; K stages transposed, rows >= npos
// zeroed (pads can carry NaN). head_size walks in 64-wide K-SLABS — hs 64 is one slab, 128 two.
[metal_dispatch(name = "enc_qk", pso = "g_pf_pso_qk", tgmem = "metal_attn_qk_msl_tgmem", tg = 128, grid = "mp/32, nk64/32, heads", params = "mp : int64, nk64 : int64, heads : int64")]
class MetalAttnQK {
    @ssbo @binding = 0 @role = "read" q : array<float>     // [mp x qd], roped
    @ssbo @binding = 1 @role = "read" k : array<float>     // [nk64 x kv_dim], roped (existing rows + the chunk)
    @ssbo @binding = 2 @role = "write" att : array<float>   // [n_heads x qrows x np32] raw scores out
    @uniform @binding = 3 ka : AttnArgs
    @workgroup ta : float[2048]             // Q block: 32 qi-rows x 64 (row stride 64)
    @workgroup tb : float[2048]             // KT block: 64 k-rows x 32 j-cols (row stride 32)

    [metal_kernel(name="metal_attn_qk_msl")]
    def metal_attn_qk {
        let qi0 = gl_WorkGroupID.x * 32u
        let j0 = gl_WorkGroupID.y * 32u
        let h = gl_WorkGroupID.z
        // skip blocks fully above the causal diagonal (uniform span: fully past uend), or fully
        // below every row's sliding ka.window (the first row's wstart is the block minimum;
        // softmax zeroes P over the skipped region)
        if ((ka.uend > 0u ? j0 >= ka.uend : j0 > ka.qoff + qi0 + 31u) ||
                (ka.window > 0u && ka.qoff + qi0 + 1u > ka.window && j0 + 32u <= ka.qoff + qi0 + 1u - ka.window)) {
            return
        }
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let kvhoff = (h / ka.kv_mul) * ka.head_size
        let qm = (sg / 2u) * 16u
        let qn = (sg % 2u) * 16u
        var c00 : simdgroup_float8x8
        var c01 : simdgroup_float8x8
        var c10 : simdgroup_float8x8
        var c11 : simdgroup_float8x8
        var ks = 0u
        while (ks < ka.head_size) {
            let kslab = min(ka.head_size - ks, 64u)
            var e = lid
            while (e < 2048u) {
                let r = e / 64u
                let c = e % 64u
                ta[e] = c < kslab ? q[(qi0 + r) * ka.qd + h * ka.head_size + ks + c] * ka.scale : 0.0
                let jr = e % 32u
                let kc = e / 32u
                tb[e] = (kc < kslab && j0 + jr < ka.npos) ? k[(j0 + jr) * ka.kv_dim + kvhoff + ks + kc] : 0.0
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
        let row0 = (h * ka.qrows + qi0 + qm) * ka.np32 + (j0 + qn)
        simdgroup_store(c00, att, row0, ka.np32)
        simdgroup_store(c01, att, row0 + 8u, ka.np32)
        simdgroup_store(c10, att, row0 + 8u * ka.np32, ka.np32)
        simdgroup_store(c11, att, row0 + 8u * ka.np32 + 8u, ka.np32)
    }
}

// One threadgroup (128 threads = 4 simdgroups) per (query row < npos, head): causal in-row
// softmax via a width-32 simd_shuffle_xor butterfly; the tail (cnt, np32) is ZEROED so the AV
// GEMM can run whole blocks. A continuation chunk sees qoff existing keys: cnt = qoff + qi + 1.
[metal_dispatch(name = "enc_softmax", pso = "g_pf_pso_softmax", tgmem = "metal_attn_softmax_msl_tgmem", tg = 128, grid = "npos, heads", params = "npos : int64, heads : int64")]
class MetalAttnSoftmax {
    @ssbo @binding = 0 @role = "readwrite" att : array<float>   // [n_heads x qrows x np32], normalized in place
    @uniform @binding = 1 ka : AttnArgs
    @ssbo @binding = 20 @role = "weight" @default = g_one sink : array<float> // [nheads] this layer's attention sinks (hass models); null = identity
    @workgroup red : float[4]               // one per simdgroup
    @workgroup bcast : float

    [metal_kernel(name="metal_attn_softmax_msl")]
    def metal_attn_softmax {   // nolint:STYLE038 — MSL kernel body; the softmax halves are tgmem-barrier coupled
        let qi = gl_WorkGroupID.x
        let h = gl_WorkGroupID.y
        let lid = gl_LocalInvocationID.x
        let nthreads = gl_WorkGroupSize.x
        let cnt = ka.uend > 0u ? ka.uend : ka.qoff + qi + 1u
        let jlo = ka.window > 0u && ka.qoff + qi + 1u > ka.window ? ka.qoff + qi + 1u - ka.window : 0u
        let rowbase = (h * ka.qrows + qi) * ka.np32
        var m = -1.0e30
        var j = jlo + lid
        while (j < cnt) {
            let sr = att[rowbase + j]
            m = max(m, ka.softcap > 0.0 ? ka.softcap * (1.0 - 2.0 / (1.0 + exp(sr * 2.0 / ka.softcap))) : sr)
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
            if (ka.hass != 0u) {
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
            let ev = exp((ka.softcap > 0.0 ? ka.softcap * (1.0 - 2.0 / (1.0 + exp(sr * 2.0 / ka.softcap))) : sr) - rowmax)
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
            if (ka.hass != 0u) {
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
        while (j < ka.np32) {
            att[rowbase + j] = 0.0          // the AV GEMM runs whole 32-blocks over this tail
            j += nthreads
        }
    }
}

// One threadgroup (128 threads = 4 simdgroups) per 32x32 output block: O = P.V walking j in
// 32-blocks up to the causal limit qi0+32 (P's zeroed tail keeps whole blocks exact). V rows
// >= npos stage as 0 (pad rows can carry NaN; 0*NaN would still poison). hs % 32 == 0 required.
[metal_dispatch(name = "enc_av", pso = "g_pf_pso_av", tgmem = "metal_attn_av_msl_tgmem", tg = 128, grid = "mp/32, head_size/32, heads", params = "mp : int64, heads : int64, head_size : int64")]
class MetalAttnAV {
    @ssbo @binding = 0 @role = "read" att : array<float>   // [n_heads x qrows x np32] normalized P
    @ssbo @binding = 1 @role = "read" v : array<float>     // [nk64 x kv_dim] raw V (existing rows + the chunk)
    @ssbo @binding = 2 @role = "write" xb : array<float>    // [mp x qd] attention output
    @uniform @binding = 3 ka : AttnArgs
    @workgroup ta : float[1024]             // P block: 32 qi-rows x 32 j (stride 32)
    @workgroup tb : float[1024]             // V block: 32 j-rows x 32 d (stride 32)

    [metal_kernel(name="metal_attn_av_msl")]
    def metal_attn_av {
        let qi0 = gl_WorkGroupID.x * 32u
        let d0 = gl_WorkGroupID.y * 32u
        let h = gl_WorkGroupID.z
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let kvhoff = (h / ka.kv_mul) * ka.head_size
        // causal: P rows here are zero past qoff+qi0+31; uniform span: softmax zeroed [uend, np32)
        let jlimit = ka.uend > 0u ? ka.np32 : min(ka.qoff + qi0 + 32u, ka.np32)
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
                ta[e] = att[(h * ka.qrows + qi0 + r) * ka.np32 + jb + c]
                tb[e] = jb + r < ka.npos ? v[(jb + r) * ka.kv_dim + kvhoff + d0 + c] : 0.0
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
        let row0 = (qi0 + qm) * ka.qd + h * ka.head_size + d0 + qn
        simdgroup_store(c00, xb, row0, ka.qd)
        simdgroup_store(c01, xb, row0 + 8u, ka.qd)
        simdgroup_store(c10, xb, row0 + 8u * ka.qd, ka.qd)
        simdgroup_store(c11, xb, row0 + 8u * ka.qd + 8u, ka.qd)
    }
}

// The ggml-geometry attention pair (v21 lessons applied to the trio's QK/AV GEMMs — the old
// 32x32 scalar-f32-staged kernels ran ~60x below mul_mm rate, owning 24ms of the 3B window).
// QK: S[h] = scale * Q_h . K_h^T, 32x64 C tiles; DASLLAMA_METAL_ATTN=0 pins the trio (hs % 64 != 0 too).

// Metal-4 staged-tile twin of MetalAttnQKMm: flat f16 tiles per head-dim chunk (Q pre-scaled
// at staging like the base), same causal/window block skips (threadgroup-uniform).
[metal_dispatch(name = "enc_qk_mm_t_c", pso = "g_pf_pso_qkmm_t", tgmem = "metal_attn_qk_mm_t_msl_tgmem", tg = 128, grid = "mp/32, nk64/64, heads", params = "mp : int64, nk64 : int64, heads : int64")]
class MetalAttnQKMmT {
    @ssbo @binding = 0 q : array<float>
    @ssbo @binding = 1 k : array<float>
    @ssbo @binding = 2 att : array<float>
    @uniform @binding = 3 ka : AttnArgs
    @workgroup twa : float16[1024]          // Q chunk: 32 queries x 32 k, flat (pre-scaled)
    @workgroup twb : float16[2048]          // K chunk: 64 keys x 32 k, flat

    [metal_kernel(name="metal_attn_qk_mm_t_msl")]
    def metal_attn_qk_mm_t {
        let mBase = gl_WorkGroupID.x * 32u
        let nBase = gl_WorkGroupID.y * 64u
        if ((ka.uend > 0u ? nBase >= ka.uend : nBase > ka.qoff + mBase + 31u) ||
                (ka.window > 0u && ka.qoff + mBase + 1u > ka.window && nBase + 64u <= ka.qoff + mBase + 1u - ka.window)) {
            return
        }
        let h = gl_WorkGroupID.z
        let kvhoff = (h / ka.kv_mul) * ka.head_size
        let qhoff = h * ka.head_size
        let lid = gl_LocalInvocationID.x
        var acc : float[2048]
        var cp = unsafe(addr(att[h * ka.qrows * ka.np32 + mBase * ka.np32 + nBase]))
        tmm2d_tg_begin(acc, 32u, 64u, 4u, 32u)
        var kb = 0u
        while (kb * 32u < ka.head_size) {
            var i = lid
            while (i < 1024u) {
                twa[i] = float16(q[(mBase + i / 32u) * ka.qd + qhoff + kb * 32u + i % 32u] * ka.scale)
                i += gl_WorkGroupSize.x
            }
            i = lid
            while (i < 2048u) {
                twb[i] = float16(k[(nBase + i / 32u) * ka.kv_dim + kvhoff + kb * 32u + i % 32u])
                i += gl_WorkGroupSize.x
            }
            barrier()
            tmm2d_tg_step(acc, twa, twb, 32u, 64u, 32u)
            barrier()
            kb++
        }
        tmm2d_tg_store(acc, cp, 32u, 64u, ka.np32)
    }
}

[metal_dispatch(name = "enc_qk_mm_c", pso = "g_pf_pso_qkmm", tgmem = "metal_attn_qk_mm_msl_tgmem", tg = 128, grid = "mp/32, nk64/64, heads", params = "mp : int64, nk64 : int64, heads : int64")]
class MetalAttnQKMm : MetalMmTileBase {
    @ssbo @binding = 0 @role = "read" q : array<float4>    // [mp x qd/4], roped
    @ssbo @binding = 1 @role = "read" k : array<float4>    // [nk64 x kv_dim/4], roped (existing rows + the chunk)
    @ssbo @binding = 2 @role = "write" att : array<float>   // [n_heads x qrows x np32] raw scores out
    @uniform @binding = 3 ka : AttnArgs
    // inherited ta = K tiles (k row, key col), tb = Q tiles (query row, k col)

    [metal_kernel(name="metal_attn_qk_mm_msl")]
    def metal_attn_qk_mm {
        let mBase = gl_WorkGroupID.x * 32u  // query rows
        let nBase = gl_WorkGroupID.y * 64u  // key rows
        // skip blocks fully above the causal diagonal (uniform span: fully past uend), or fully
        // below every row's sliding ka.window
        if ((ka.uend > 0u ? nBase >= ka.uend : nBase > ka.qoff + mBase + 31u) ||
                (ka.window > 0u && ka.qoff + mBase + 1u > ka.window && nBase + 64u <= ka.qoff + mBase + 1u - ka.window)) {
            return
        }
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let h = gl_WorkGroupID.z
        let kvhoff4 = ((h / ka.kv_mul) * ka.head_size) / 4u
        let qoff4 = (h * ka.head_size) / 4u
        let qd4 = ka.qd / 4u
        let kvd4 = ka.kv_dim / 4u
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
        while (kb * 32u < ka.head_size) {
            let srcA = (nBase + lr0) * kvd4 + kvhoff4 + (kb * 32u + il0 * 16u) / 4u
            let a0 = k[srcA]
            let a1 = k[srcA + 1u]
            let a2 = k[srcA + 2u]
            let a3 = k[srcA + 3u]
            let srcB = (mBase + lr1) * qd4 + qoff4 + (kb * 32u + iy) / 4u
            let b0 = q[srcB] * ka.scale
            let b1 = q[srcB + 1u] * ka.scale
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
            acc_quad(aB, bB, ma, mb, mc)
            kb++
        }
        let base = h * ka.qrows * ka.np32
        let row0 = mBase + (sg / 2u) * 16u  // accumulator rows = queries
        let col0 = nBase + (sg % 2u) * 32u  // accumulator cols = keys
        for [unroll_full] (i in range(8)) {
            simdgroup_store(mc[i], att, base + (row0 + uint(i / 4) * 8u) * ka.np32 + col0 + uint(i % 4) * 8u, ka.np32)
        }
    }
}

// AV: O_h = P_h . V_h — C tile 32 tokens x 64 V-columns, k walks the causally limited position
// axis in 32-blocks. V stages contiguous float4 loads, position rows >= npos ZEROED (causal
// softmax zeroes P columns >= npos EXACTLY; 0 * NaN from a pad V row would poison the tile).

// Metal-4 staged-tile twin of MetalAttnAVMm: P x V over flat f16 tiles; V staged TRANSPOSED
// (col-major source) with pad positions >= npos zeroed (0 * stale-NaN would poison the tile).
[metal_dispatch(name = "enc_av_mm_t_c", pso = "g_pf_pso_avmm_t", tgmem = "metal_attn_av_mm_t_msl_tgmem", tg = 128, grid = "mp/32, head_size/64, heads", params = "mp : int64, heads : int64, head_size : int64")]
class MetalAttnAVMmT {
    @ssbo @binding = 0 att : array<float>
    @ssbo @binding = 1 v : array<float>
    @ssbo @binding = 2 xb : array<float>
    @uniform @binding = 3 ka : AttnArgs
    @workgroup twa : float16[1024]          // P chunk: 32 tokens x 32 positions, flat
    @workgroup twb : float16[2048]          // V chunk: 64 v-cols x 32 positions, flat (transposed)

    [metal_kernel(name="metal_attn_av_mm_t_msl")]
    def metal_attn_av_mm_t {
        let mBase = gl_WorkGroupID.x * 32u
        let vBase = gl_WorkGroupID.y * 64u
        let h = gl_WorkGroupID.z
        let kvhoff = (h / ka.kv_mul) * ka.head_size
        let lid = gl_LocalInvocationID.x
        // P rows here are zero past qoff+mBase+31; uniform span: softmax zeroed [uend, np32)
        let klimit = ka.uend > 0u ? ka.np32 : min(ka.qoff + mBase + 32u, ka.np32)
        var acc : float[2048]
        var cp = unsafe(addr(xb[mBase * ka.qd + h * ka.head_size + vBase]))
        tmm2d_tg_begin(acc, 32u, 64u, 4u, 32u)
        var kb = 0u
        while (kb * 32u < klimit) {
            var i = lid
            while (i < 1024u) {
                twa[i] = float16(att[h * ka.qrows * ka.np32 + (mBase + i / 32u) * ka.np32 + kb * 32u + i % 32u])
                i += gl_WorkGroupSize.x
            }
            i = lid
            while (i < 2048u) {
                let pos = kb * 32u + i % 32u
                twb[i] = pos < ka.npos ? float16(v[pos * ka.kv_dim + kvhoff + vBase + i / 32u]) : float16(0.0)
                i += gl_WorkGroupSize.x
            }
            barrier()
            tmm2d_tg_step(acc, twa, twb, 32u, 64u, 32u)
            barrier()
            kb++
        }
        tmm2d_tg_store(acc, cp, 32u, 64u, ka.qd)
    }
}

[metal_dispatch(name = "enc_av_mm_c", pso = "g_pf_pso_avmm", tgmem = "metal_attn_av_mm_msl_tgmem", tg = 128, grid = "mp/32, head_size/64, heads", params = "mp : int64, heads : int64, head_size : int64")]
class MetalAttnAVMm : MetalMmTileBase {
    @ssbo @binding = 0 @role = "read" att : array<float4>  // [n_heads x qrows x np32/4] normalized P
    @ssbo @binding = 1 @role = "read" v : array<float4>    // [nk64 x kv_dim/4] raw V (existing rows + the chunk)
    @ssbo @binding = 2 @role = "write" xb : array<float>    // [mp x qd] attention output
    @uniform @binding = 3 ka : AttnArgs
    // inherited ta = V tiles (pos row, col col), tb = P tiles (token row, pos col)

    [metal_kernel(name="metal_attn_av_mm_msl")]
    def metal_attn_av_mm {
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let mBase = gl_WorkGroupID.x * 32u  // token rows
        let vBase = gl_WorkGroupID.y * 64u  // V columns within the head
        let h = gl_WorkGroupID.z
        let kvhoff4 = ((h / ka.kv_mul) * ka.head_size) / 4u
        let kvd4 = ka.kv_dim / 4u
        let np324 = ka.np32 / 4u
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
        // causal: P rows here are zero past qoff+mBase+31; uniform span: softmax zeroed [uend, np32)
        let klimit = ka.uend > 0u ? ka.np32 : min(ka.qoff + mBase + 32u, ka.np32)
        var kb = 0u
        while (kb * 32u < klimit) {
            let posg = kb * 32u + pr
            let srcA = posg * kvd4 + kvhoff4 + (vBase + vc16) / 4u
            let zero = float4(0.0)
            let a0 = posg < ka.npos ? v[srcA] : zero
            let a1 = posg < ka.npos ? v[srcA + 1u] : zero
            let a2 = posg < ka.npos ? v[srcA + 2u] : zero
            let a3 = posg < ka.npos ? v[srcA + 3u] : zero
            let srcB = (h * ka.qrows + mBase + lr1) * np324 + (kb * 32u + iy) / 4u
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
            acc_quad(aB, bB, ma, mb, mc)
            kb++
        }
        let row0 = mBase + (sg / 2u) * 16u  // accumulator rows = tokens
        let col0 = h * ka.head_size + vBase + (sg % 2u) * 32u
        for [unroll_full] (i in range(8)) {
            simdgroup_store(mc[i], xb, (row0 + uint(i / 4) * 8u) * ka.qd + col0 + uint(i % 4) * 8u, ka.qd)
        }
    }
}

// Interleave two [npos x d] panels into the [npos x 2d] MTP eh_proj input — the draft-warm
// cat rows [enorm(embed) ; hnorm(hidden)]; one thread per source element.
[metal_dispatch(name = "pf_enc_cat2", pso = "g_pf_pso_cat2", tg = 64, grid = "total/64", params = "total : int64")]
class MetalPfCat2 {
    @ssbo @binding = 0 @role = "write" y : array<float>
    @ssbo @binding = 1 @role = "read" a : array<float>
    @ssbo @binding = 2 @role = "read" b : array<float>
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

// DeltaNet (Wave D): the β/α projection GEMV (MetalDnBa) lives in dasllama_metal_kernels as
// the BATCHED stamp of the MetalQ8GemvT template — same stream walk, grid-y position axis.

// ===== the whole-prefill driver =====
// Registered as prefill override "metal" (DASLLAMA_PIN_PREFILL). One command buffer per prefill;
// ONE commit+wait, then unified-memory readback hands x_b + per-layer roped-K/raw-V back to the CPU.

var private g_pf_failed = false   // prefill PSO bring-up failed (the device latch is common's g_failed)
// per-prefill transients, reused across calls (prefill orchestration is serial)
var private @scratch g_pf_bks : array<MetalBuffer?>
var private @scratch g_pf_bvs : array<MetalBuffer?>
var private @scratch g_pf_cbs : array<MetalCommandBuffer?>
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
var private g_pf_inited = false
var private g_pf_pso_rope : MetalComputePipeline?
var private g_pf_pso_addbias : MetalComputePipeline?
var private g_pf_pso_qk : MetalComputePipeline?
var private g_pf_pso_softmax : MetalComputePipeline?
var private g_pf_pso_av : MetalComputePipeline?
var private g_pf_pso_cat2 : MetalComputePipeline?
var private g_pf_pso_mm_t : MetalComputePipeline?  // the mul_mm TENSOR twin; the base is the shared g_pso_mm
var private g_pf_pso_bf16_mm : MetalComputePipeline?   // native-BF16 A twin (E-series model_proj)
var private g_pf_bf16_mm_tensor : bool   // crowned tensor twin selected (no tgmem bind)
var private g_pf_mm_tensor : bool        // ditto for the production q8 mul_mm
var private g_pf_moe_mm_q8_tensor : bool // "moe_mulmm_q8" crowned (contiguous sites only)
var private g_pf_pso_moe_mm_q8_t : MetalComputePipeline?
var private g_pf_moe_mm_mx4_tensor : bool // "moe_mulmm_mx4" crowned (contiguous sites only)
var private g_pf_pso_moe_mm_mx4_t : MetalComputePipeline?
var private g_pf_kq_mm4_tensor : bool     // "kq_mulmm_k4" crowned
var private g_pf_pso_kq_mm4_t : MetalComputePipeline?
var private g_pf_kq_mm5_tensor : bool     // "kq_mulmm_k5" crowned
var private g_pf_pso_kq_mm5_t : MetalComputePipeline?
var private g_pf_kq_mm6_tensor : bool     // "kq_mulmm_k6" crowned
var private g_pf_pso_kq_mm6_t : MetalComputePipeline?
var private g_pf_qkmm_tensor : bool       // "attn_qkmm" crowned
var private g_pf_pso_qkmm_t : MetalComputePipeline?
var private g_pf_avmm_tensor : bool       // "attn_avmm" crowned
var private g_pf_pso_avmm_t : MetalComputePipeline?
var private g_pf_pso_ple_gather : MetalComputePipeline?   // PLE pre-step: q8 token-row gather
var private g_pf_pso_ple_finish : MetalComputePipeline?   // PLE pre-step: rms + combine, in place
var private g_pf_pso_qkmm : MetalComputePipeline?
var private g_pf_pso_avmm : MetalComputePipeline?
var private g_pf_pso_moe_count : MetalComputePipeline?
var private g_pf_pso_moe_bucket : MetalComputePipeline?
var private g_pf_pso_moe_reduce : MetalComputePipeline?
var private g_pf_pso_moe_mm_q8 : MetalComputePipeline?
var private g_pf_pso_moe_mm_k4 : MetalComputePipeline?
var private g_pf_pso_moe_mm_k5 : MetalComputePipeline?
var private g_pf_pso_moe_mm_k6 : MetalComputePipeline?
var private g_pf_pso_swiglu_oai : MetalComputePipeline?
var private g_pf_pso_moe_mm_mx4 : MetalComputePipeline?
var private g_pf_pso_moe_mm_q51 : MetalComputePipeline?
var private g_mm_legacy = false             // set_metal_prefill_mulmm_legacy — tests' exact-parity rail
var private g_pf_logits_cpu = false            // set_metal_prefill_logits_cpu — the logits A/B rail
// timing-attribution knockout (DASLLAMA_METAL_PREFILL_SKIP=attn|gemm|ew|nongemm|moe|moe_mm|
// moe_act|moe_bias|moe_reduce|moe_only_router|moe_only_route|moe_no_mm_act): skips encoding that
// stage family (moe_bias = hasb=0, the mm bias fold off). Output GARBAGE, debug only.
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
//! Test A/B seat: pin the span serving on/off in-process (DASLLAMA_METAL_SPAN's setter twin).
def public set_metal_prefill_span(on : bool) {
    g_pf_env_span = on
}

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

//! The tower driver's borrow seat: full prefill PSO bring-up, idempotent — the borrowed builders
//! (pf_enc_bf16_mm, enc_add_bias_rows; the attention trio next) bind prefill-owned PSOs.
def public metal_prefill_pso_init : bool {
    return metal_prefill_init()
}

//! Release every GPU object the driver holds — the tests' leak-gate rail (and the reset for
//! refilled weight arrays, same contract as metal_gemm_shutdown). Lazily re-initializes on the
//! next served prefill.
def public metal_prefill_shutdown {   // nolint:STYLE037,STYLE038 — flat one-release-per-PSO list
    kcov_fold()   // PSO pointers recycle across re-init — counts fold into names first
    pool_drain(g_pf_pool)
    pool_drain(g_pf_upool)
    if (g_pf_pso_moe_mm_q8_t != null) {
        metal_release(g_pf_pso_moe_mm_q8_t)
        g_pf_pso_moe_mm_q8_t = null
    }
    if (g_pf_pso_moe_mm_mx4_t != null) {
        metal_release(g_pf_pso_moe_mm_mx4_t)
        g_pf_pso_moe_mm_mx4_t = null
    }
    if (g_pf_pso_kq_mm4_t != null) {
        metal_release(g_pf_pso_kq_mm4_t)
        g_pf_pso_kq_mm4_t = null
    }
    if (g_pf_pso_kq_mm5_t != null) {
        metal_release(g_pf_pso_kq_mm5_t)
        g_pf_pso_kq_mm5_t = null
    }
    if (g_pf_pso_kq_mm6_t != null) {
        metal_release(g_pf_pso_kq_mm6_t)
        g_pf_pso_kq_mm6_t = null
    }
    g_pf_inited = false
    if (g_pf_pso_rope != null) {
        metal_release(g_pf_pso_rope)
        g_pf_pso_rope = null
    }
    if (g_pf_pso_addbias != null) {
        metal_release(g_pf_pso_addbias)
        g_pf_pso_addbias = null
    }
    if (g_pf_pso_qk != null) {
        metal_release(g_pf_pso_qk)
        g_pf_pso_qk = null
    }
    if (g_pf_pso_softmax != null) {
        metal_release(g_pf_pso_softmax)
        g_pf_pso_softmax = null
    }
    if (g_pf_pso_av != null) {
        metal_release(g_pf_pso_av)
        g_pf_pso_av = null
    }
    if (g_pf_pso_cat2 != null) {
        metal_release(g_pf_pso_cat2)
        g_pf_pso_cat2 = null
    }
    if (g_pf_pso_mm_t != null) {
        metal_release(g_pf_pso_mm_t)
        g_pf_pso_mm_t = null
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
    for (pso in [g_pf_pso_moe_count,
                 g_pf_pso_moe_bucket, g_pf_pso_moe_reduce, g_pf_pso_moe_mm_q8, g_pf_pso_moe_mm_k4,
                 g_pf_pso_moe_mm_k5, g_pf_pso_moe_mm_k6, g_pf_pso_swiglu_oai,
                 g_pf_pso_moe_mm_mx4, g_pf_pso_moe_mm_q51]) {
        if (pso != null) {
            metal_release(pso)
        }
    }
    g_pf_pso_moe_count = null
    g_pf_pso_moe_bucket = null
    g_pf_pso_moe_reduce = null
    g_pf_pso_moe_mm_q8 = null
    g_pf_pso_moe_mm_k4 = null
    g_pf_pso_moe_mm_k5 = null
    g_pf_pso_moe_mm_k6 = null
    g_pf_pso_swiglu_oai = null
    g_pf_pso_moe_mm_mx4 = null
    g_pf_pso_moe_mm_q51 = null
    if (g_pf_pso_qkmm != null) {
        metal_release(g_pf_pso_qkmm)
        g_pf_pso_qkmm = null
    }
    if (g_pf_pso_qkmm_t != null) {
        metal_release(g_pf_pso_qkmm_t)
        g_pf_pso_qkmm_t = null
    }
    if (g_pf_pso_avmm_t != null) {
        metal_release(g_pf_pso_avmm_t)
        g_pf_pso_avmm_t = null
    }
    if (g_pf_pso_avmm != null) {
        metal_release(g_pf_pso_avmm)
        g_pf_pso_avmm = null
    }
    g_pf_failed = false
}

[cold_path]   // one-time device/PSO bring-up, guarded by its own inited flag
def private metal_prefill_init : bool {   // nolint:STYLE038 — flat one-compile-per-PSO list
    if (g_pf_inited) {
        return true
    }
    if (g_pf_failed) {
        return false
    }
    if (!metal_common_init()) {   // ONE device + queue for the whole family (M3) — this also arms the hazard-rail knobs on prefill-first runs
        to_log(LOG_WARNING, "dasLLAMA metal prefill: no Metal device - prefill stays on the CPU\n")
        g_pf_failed = true
        return false
    }
    if (!metal_decode_init()) {   // the family kernel registry — the generated enc_* builders bind ITS PSOs (idempotent)
        g_pf_failed = true
        return false
    }
    var ok = true
    g_pf_pso_rope = compile_pso(metal_rope_msl, metal_rope_msl_entry, metal_rope_msl_fastmath, ok)
    g_pf_pso_addbias = compile_pso(metal_add_bias_rows_msl, metal_add_bias_rows_msl_entry, metal_add_bias_rows_msl_fastmath, ok)
    g_pf_pso_qk = compile_pso(metal_attn_qk_msl, metal_attn_qk_msl_entry, metal_attn_qk_msl_fastmath, ok)
    g_pf_pso_softmax = compile_pso(metal_attn_softmax_msl, metal_attn_softmax_msl_entry, metal_attn_softmax_msl_fastmath, ok)
    g_pf_pso_av = compile_pso(metal_attn_av_msl, metal_attn_av_msl_entry, metal_attn_av_msl_fastmath, ok)
    g_pf_pso_cat2 = compile_pso(metal_pf_cat2_msl, metal_pf_cat2_msl_entry, metal_pf_cat2_msl_fastmath, ok)
    g_pf_mm_tensor = metal_tensor_crowned("mulmm_q8")
    if (g_pf_mm_tensor) {
        g_pf_pso_mm_t = compile_pso(metal_q8_mulmm_t_msl, metal_q8_mulmm_t_msl_entry, metal_q8_mulmm_t_msl_fastmath, ok)
    }
    g_pf_bf16_mm_tensor = metal_tensor_crowned("mulmm_bf16")
    g_pf_pso_bf16_mm = (g_pf_bf16_mm_tensor
        ? compile_pso(metal_bf16_mulmm_t_msl, metal_bf16_mulmm_t_msl_entry, metal_bf16_mulmm_t_msl_fastmath, ok)
        : compile_pso(metal_bf16_mulmm_msl, metal_bf16_mulmm_msl_entry, metal_bf16_mulmm_msl_fastmath, ok))
    g_pf_pso_ple_gather = compile_pso(metal_ple_gather_q8_msl, metal_ple_gather_q8_msl_entry, metal_ple_gather_q8_msl_fastmath, ok)
    g_pf_pso_ple_finish = compile_pso(metal_ple_finish_msl, metal_ple_finish_msl_entry, metal_ple_finish_msl_fastmath, ok)
    g_pf_kq_mm4_tensor = metal_tensor_crowned("kq_mulmm_k4")
    if (g_pf_kq_mm4_tensor) {
        g_pf_pso_kq_mm4_t = compile_pso(MetalKqMulMmK4T_metal_kq_mulmm_t_msl, MetalKqMulMmK4T_metal_kq_mulmm_t_msl_entry, MetalKqMulMmK4T_metal_kq_mulmm_t_msl_fastmath, ok)
    }
    g_pf_kq_mm5_tensor = metal_tensor_crowned("kq_mulmm_k5")
    if (g_pf_kq_mm5_tensor) {
        g_pf_pso_kq_mm5_t = compile_pso(MetalKqMulMmK5T_metal_kq_mulmm_t_msl, MetalKqMulMmK5T_metal_kq_mulmm_t_msl_entry, MetalKqMulMmK5T_metal_kq_mulmm_t_msl_fastmath, ok)
    }
    g_pf_kq_mm6_tensor = metal_tensor_crowned("kq_mulmm_k6")
    if (g_pf_kq_mm6_tensor) {
        g_pf_pso_kq_mm6_t = compile_pso(metal_kq_mulmm_k6_t_msl, metal_kq_mulmm_k6_t_msl_entry, metal_kq_mulmm_k6_t_msl_fastmath, ok)
    }
    g_pf_pso_qkmm = compile_pso(metal_attn_qk_mm_msl, metal_attn_qk_mm_msl_entry, metal_attn_qk_mm_msl_fastmath, ok)
    g_pf_pso_avmm = compile_pso(metal_attn_av_mm_msl, metal_attn_av_mm_msl_entry, metal_attn_av_mm_msl_fastmath, ok)
    g_pf_qkmm_tensor = metal_tensor_crowned("attn_qkmm")
    if (g_pf_qkmm_tensor) {
        g_pf_pso_qkmm_t = compile_pso(metal_attn_qk_mm_t_msl, metal_attn_qk_mm_t_msl_entry, metal_attn_qk_mm_t_msl_fastmath, ok)
    }
    g_pf_avmm_tensor = metal_tensor_crowned("attn_avmm")
    if (g_pf_avmm_tensor) {
        g_pf_pso_avmm_t = compile_pso(metal_attn_av_mm_t_msl, metal_attn_av_mm_t_msl_entry, metal_attn_av_mm_t_msl_fastmath, ok)
    }
    g_pf_pso_moe_count = compile_pso(metal_moe_count_msl, metal_moe_count_msl_entry, metal_moe_count_msl_fastmath, ok)
    g_pf_pso_moe_bucket = compile_pso(metal_moe_bucket_msl, metal_moe_bucket_msl_entry, metal_moe_bucket_msl_fastmath, ok)
    g_pf_pso_moe_reduce = compile_pso(metal_moe_reduce_msl, metal_moe_reduce_msl_entry, metal_moe_reduce_msl_fastmath, ok)
    g_pf_pso_moe_mm_q8 = compile_pso(metal_moe_mulmm_q8_msl, metal_moe_mulmm_q8_msl_entry, metal_moe_mulmm_q8_msl_fastmath, ok)
    g_pf_moe_mm_q8_tensor = metal_tensor_crowned("moe_mulmm_q8")
    if (g_pf_moe_mm_q8_tensor) {
        g_pf_pso_moe_mm_q8_t = compile_pso(metal_moe_mulmm_q8_t_msl, metal_moe_mulmm_q8_t_msl_entry, metal_moe_mulmm_q8_t_msl_fastmath, ok)
    }
    g_pf_pso_moe_mm_k4 = compile_pso(metal_moe_mulmm_k4_msl, metal_moe_mulmm_k4_msl_entry, metal_moe_mulmm_k4_msl_fastmath, ok)
    g_pf_pso_moe_mm_k5 = compile_pso(metal_moe_mulmm_k5_msl, metal_moe_mulmm_k5_msl_entry, metal_moe_mulmm_k5_msl_fastmath, ok)
    g_pf_pso_moe_mm_k6 = compile_pso(metal_moe_mulmm_k6_msl, metal_moe_mulmm_k6_msl_entry, metal_moe_mulmm_k6_msl_fastmath, ok)
    g_pf_pso_swiglu_oai = compile_pso(MetalSwigluOaiPf_metal_swiglu_oai_msl, MetalSwigluOaiPf_metal_swiglu_oai_msl_entry, MetalSwigluOaiPf_metal_swiglu_oai_msl_fastmath, ok)
    g_pf_pso_moe_mm_mx4 = compile_pso(metal_moe_mulmm_mx4_msl, metal_moe_mulmm_mx4_msl_entry, metal_moe_mulmm_mx4_msl_fastmath, ok)
    g_pf_moe_mm_mx4_tensor = metal_tensor_crowned("moe_mulmm_mx4")
    if (g_pf_moe_mm_mx4_tensor) {
        g_pf_pso_moe_mm_mx4_t = compile_pso(metal_moe_mulmm_mx4_t_msl, metal_moe_mulmm_mx4_t_msl_entry, metal_moe_mulmm_mx4_t_msl_fastmath, ok)
    }
    g_pf_pso_moe_mm_q51 = compile_pso(metal_moe_mulmm_q51_msl, metal_moe_mulmm_q51_msl_entry, metal_moe_mulmm_q51_msl_fastmath, ok)
    if (!ok) {
        metal_prefill_shutdown()
        g_pf_failed = true
        return false
    }
    to_log(LOG_INFO, "dasLLAMA metal prefill: resident path live on {metal_device_name(g_dev)}\n")
    g_pf_inited = true
    return true
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

// The window-READINESS leg: are THIS window's rope tables built yet? forward_prefill builds them
// AFTER it consults the PLE gate, so a capability question must not ask this one.
def private rope_rows_ready(t : Model; s : Session; npos : int64) : bool {
    let c = t.config
    let swa_hs = c.head_size_swa > 0l ? c.head_size_swa : c.head_size
    return (long_length(s.rope_cos) >= npos * ((c.rope_dim > 0l ? c.rope_dim : c.head_size) / 2l) &&
        (!has_dual_rope(c) || long_length(s.rope_cos_swa) >= npos * (swa_hs / 2l)))
}

// the std dense stack + PF_NEEDS_OK features are wired — anything else declines to the CPU
// loop. Returns the FIRST failing clause; .none = supported.
[unused_argument(start_pos)]   // start_pos > 0 (a continuation chunk) is served (M2.5): the
// existing rows gather into the K/V panels and the causal diagonal shifts by qoff — the gate
// keeps the argument so future clauses can window it
def private prefill_decline(t : Model; s : Session; npos, start_pos : int64) : MetalPrefillDecline {
    // the A/B rail: DASLLAMA_METAL_SPAN=0 declines the span to the tripwire-guarded CPU arm
    if (s.attn_uniform_end != 0l && !g_pf_env_span) {
        return MetalPrefillDecline.non_causal_span
    }
    let why = prefill_decline_caps(t, npos, s.attn_uniform_end != 0l)
    if (why != MetalPrefillDecline.none) return why
    // rope tables are the hook's input (partial rope: rows rope_dim/2 wide; dual-rope: swa class width)
    return rope_rows_ready(t, s, npos) ? MetalPrefillDecline.none : MetalPrefillDecline.rope_rows
}

// Everything a model either can or cannot do, independent of how far THIS window has been set up.
// Callers that run before the window is staged (the PLE gpu gate) ask this one; a window that then
// turns out not to be ready declines later, and metal_prefill_forward's fallback covers it.
def private prefill_decline_caps(t : Model; npos : int64; span : bool = false) : MetalPrefillDecline {
    // npos window is a POLICY decline (blob-only and short SPANS serve any npos: neither has a safe CPU path)
    if (!t.metal_blob && ((npos < g_min_npos && !span) || npos > g_max_npos)) {
        return MetalPrefillDecline.npos_window
    }
    if (t.quant != QuantMode.q8) {
        return MetalPrefillDecline.quant_mode
    }
    // kquant rides the mul_mm path only — mm pinned off or a shape the %64 gate rejects has no kq-capable GEMM and declines
    if (t.kquant_native) {
        // kq_load_gpu_supported rejects fmts without a GPU kernel (q4_0 QAT tier) before they dispatch a wrong-layout branch
        if (g_mm_legacy || !g_pf_env_mulmm ||
                !pf_mm_shape64(t) || !kq_load_gpu_supported(t)) {
            return MetalPrefillDecline.kquant_native
        }
    }
    // wscale_f16 rides the mul_mm path only — a model the mm shape gate rejects has no s16-capable GEMM and declines
    if (t.wscale_f16) {
        if (g_mm_legacy || !g_pf_env_mulmm || !pf_mm_shape64(t)) {
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
    if (!t.metal_blob || g_mm_legacy || !g_pf_env_mulmm ||
            !pf_mm_shape64(t)) {
        return MetalPrefillDecline.planar
    }
    return MetalPrefillDecline.none
}


// the production mul_mm dispatch: W blob bound twice (half-scale + byte views), raw f32 X,
// grid (mp/32 token tiles, d/64 output tiles). Requires d % 64 == 0 and mp % 64 == 0.
// yoff places the K/V outputs past a continuation's existing panel rows.
def private enc_gemm_mm(enc : MetalComputeEncoder?; bwblob : MetalBuffer?; wboff : uint64; bx, by, bk, bn : MetalBuffer?; mp, d : int64; yoff : uint64 = 0ul) {
    if (g_pf_mm_tensor) {
        enc_gemm_mm_t_c(enc, bwblob, wboff, bwblob, bx, by, yoff, bk, bn, mp, d)
    } else {
        enc_gemm_mm_c(enc, bwblob, wboff, bwblob, bx, by, yoff, bk, bn, mp, d)
    }
}

// the E-series model_proj GEMM off the kept-bf16 blob — ONE PSO global holds whichever of the
// base/tensor pair compiled (crown resolved at init), so the pick is the tgmem/binding layout.
def pf_enc_bf16_mm(enc : MetalComputeEncoder?; bw : MetalBuffer?; wboff : uint64;
                   bx, by, bk, bn : MetalBuffer?; mp, nd : int64) {
    if (g_pf_bf16_mm_tensor) {
        pf_enc_bf16_mm_t_c(enc, bw, wboff, bx, by, bk, bn, mp, nd)
    } else {
        pf_enc_bf16_mm_c(enc, bw, wboff, bx, by, bk, bn, mp, nd)
    }
}

// one kq weight site on the prefill mul_mm path: the format's mul_mm twin over the whole-plane
// binds (kq_quants_of / kq_scales_of — k6 binds its split scale plane twice, sub-scales at soff
// + the f16 d tail at doff). Same grid/tgmem as enc_gemm_mm.
def private pf_enc_kq_site_mm(enc : MetalComputeEncoder?; t : Model; fmt : KqFmt; woff, rows : int64;
                              bx, by, bk, bn : MetalBuffer?; mp : int64; yoff : uint64 = 0ul) {
    let bq = kq_quants_of(g_dev, t, fmt, woff)
    let bs = kq_scales_of(g_dev, t, fmt, woff)
    if (fmt == KqFmt.k6) {
        if (g_pf_kq_mm6_tensor && g_pf_pso_kq_mm6_t != null) {
            enc_kq_mm_k6_t_c(enc, bs.buf, bs.doff, bs.buf, bs.soff, bq.buf, bq.qoff, bx, by, yoff, bk, bn, mp, rows)
        } else {
            enc_kq_mm_k6_c(enc, bs.buf, bs.doff, bs.buf, bs.soff, bq.buf, bq.qoff, bx, by, yoff, bk, bn, mp, rows)
        }
    } elif (fmt == KqFmt.k4) {
        if (g_pf_kq_mm4_tensor && g_pf_pso_kq_mm4_t != null) {
            enc_kq_mm_k4_t_c(enc, bs.buf, bs.soff, bs.buf, bs.soff, bq.buf, bq.qoff, bx, by, yoff, bk, bn, mp, rows)
        } else {
            enc_kq_mm_k4_c(enc, bs.buf, bs.soff, bs.buf, bs.soff, bq.buf, bq.qoff, bx, by, yoff, bk, bn, mp, rows)
        }
    } else {
        if (g_pf_kq_mm5_tensor && g_pf_pso_kq_mm5_t != null) {
            enc_kq_mm_k5_t_c(enc, bs.buf, bs.soff, bs.buf, bs.soff, bq.buf, bq.qoff, bx, by, yoff, bk, bn, mp, rows)
        } else {
            enc_kq_mm_k5_c(enc, bs.buf, bs.soff, bs.buf, bs.soff, bq.buf, bq.qoff, bx, by, yoff, bk, bn, mp, rows)
        }
    }
}

// the kq classifier GEMV (one f32 row against the [vocab x dim] kq plane) — the moved decode
// kernels' PSOs, prefill-owned twins
def private pf_enc_kq_gemv(enc : MetalComputeEncoder?; t : Model; fmt : KqFmt; woff, rows, n : int64;
                           bx : MetalBuffer?; xoff : uint64; by, bn, bd : MetalBuffer?) {
    let bq = kq_quants_of(g_dev, t, fmt, woff)
    let bs = kq_scales_of(g_dev, t, fmt, woff)
    if (fmt == KqFmt.k6) {
        enc_kq_k6_c(enc, bs.buf, bs.doff, bs.buf, bs.soff, bq.buf, bq.qoff, bx, xoff, by, 0ul, bn, bd, rows, n)
    } elif (fmt == KqFmt.k4) {
        enc_kq_k4_c(enc, bs.buf, bs.soff, bs.buf, bs.soff, bq.buf, bq.qoff, bx, xoff, by, 0ul, bn, bd, rows, n)
    } else {
        enc_kq_k5_c(enc, bs.buf, bs.soff, bs.buf, bs.soff, bq.buf, bq.qoff, bx, xoff, by, 0ul, bn, bd, rows, n)
    }
}


// pf_enc_rms is [metal_dispatch]-generated from MetalRmsNorm (kernels-side class — the 256/tg
// prefill shape rides the same lensed class as decode's enc_rms / enc_rms_b).

// pf_enc_ple_gather is [metal_dispatch]-generated from MetalPleGatherQ8 (PLE stage 1: q8
// token-row gather into the layer-major side panel; the dual-view q8 pair binds at eoff).

// pf_enc_ple_finish is [metal_dispatch]-generated from MetalPleFinish (PLE stage 3: rms +
// combine, in place on the panel the gather wrote).

// ===== MoE prefill encoders (Wave C stage 2b) =====

// per-position router + select + the atomics-free CSR (count -> prefix -> bucket) for layer l
def private pf_enc_moe_route(enc : MetalComputeEncoder?; t : Model; l : int64;
                             bx, blg, bsel, bcnt, bbase, bbkt, binv, bdim, bne, bnk, bgm, brb, bhasb, btot, bnp : MetalBuffer?; npos : int64) {
    let c = t.config
    var bw_rt = upload_region(unsafe(addr < void? >(t.fblob[t.router_off + l * c.n_expert * c.dim])), uint64(c.n_expert * c.dim * 4l))
    enc_moe_router_b(enc, bw_rt, bx, blg, bdim, bne, bdim, brb, bhasb, bnp, c.n_expert, npos)   // bxs = bdim: per-position x stride = dim
    return if (g_pf_skip == "moe_only_router")   // attribution: router GEMV alone, no select/CSR
    enc_moe_select(enc, blg, bsel, bsel, bne, bnk, bgm, npos)
    return if (g_pf_skip == "moe_only_sel")   // attribution: router + select, no CSR
    pf_enc_moe_count(enc, bsel, bcnt, btot, bnk, c.n_expert)
    pf_enc_moe_bucket(enc, bsel, bbase, bbkt, binv, btot, bnk, bcnt, bne, c.n_expert)
}

// the MetalMoeMulMmBase family tail: xf@3 y@4 cnt@7 basep@8 bkt@9 (kargs@5 binds at the site).
// Production rides the generated builders below; the A/B race harnesses still hand-encode
// through this, so a binding renumber desyncs loudly instead of unseen.
def private kn_moe_mm_family_tail(enc : MetalComputeEncoder?; bx, by, bcnt, bbase, bbkt : MetalBuffer?) {
    kn_buffer(enc, bx, 0ul, 3)
    kn_buffer(enc, by, 0ul, 4)
    kn_buffer(enc, bcnt, 0ul, 7)
    kn_buffer(enc, bbase, 0ul, 8)
    kn_buffer(enc, bbkt, 0ul, 9)
}

// one gathered expert mul_mm site: grid (ceil(npos/32) tiles, rows/64, ne experts); the twin
// exits tiles past the expert's padded count. Per-format generated builders; the tensor twins
// keep their own compact binding layout (contiguous-only) so the pick swaps builders.
def private pf_enc_moe_mm(enc : MetalComputeEncoder?; t : Model; fmt : KqFmt; stack_off, rows, npos : int64;
                          bx, by, bcnt, bbase, bbkt : MetalBuffer?; var ka : MoeMmArgs;
                          contiguous : bool = false) {
    let ne = t.config.n_expert
    if (fmt == KqFmt.q8) {
        let bw = blob_of(g_dev, t, stack_off)
        if (g_pf_moe_mm_q8_tensor && contiguous && g_pf_pso_moe_mm_q8_t != null) {
            pf_enc_moe_mm_q8_t_c(enc, bw.buf, bw.boff, bw.buf, bx, by, ka, bcnt, bbase, rows, npos, ne)
        } else {
            pf_enc_moe_mm_q8_c(enc, bw.buf, bw.boff, bw.buf, bx, by, ka, bcnt, bbase, bbkt, rows, npos, ne)
        }
    } elif (fmt == KqFmt.q51) {
        let qp = q51_planes_of(g_dev, t, stack_off)
        pf_enc_moe_mm_q51_c(enc, qp.sbuf, qp.soff, qp.qbuf, qp.qoff, bx, by, ka, bcnt, bbase, bbkt, rows, npos, ne)
    } elif (fmt == KqFmt.k6) {
        let bq = kq_quants_of(g_dev, t, fmt, stack_off)
        let bs = kq_scales_of(g_dev, t, fmt, stack_off)
        pf_enc_moe_mm_k6_c(enc, bs.buf, bs.doff, bs.buf, bs.soff, bq.buf, bq.qoff, bx, by, ka, bcnt, bbase, bbkt, rows, npos, ne)
    } elif (fmt == KqFmt.k4) {
        let bq = kq_quants_of(g_dev, t, fmt, stack_off)
        let bs = kq_scales_of(g_dev, t, fmt, stack_off)
        pf_enc_moe_mm_k4_c(enc, bs.buf, bs.soff, bs.buf, bq.buf, bq.qoff, bx, by, ka, bcnt, bbase, bbkt, rows, npos, ne)
    } else {
        let bq = kq_quants_of(g_dev, t, fmt, stack_off)
        let bs = kq_scales_of(g_dev, t, fmt, stack_off)
        pf_enc_moe_mm_k5_c(enc, bs.buf, bs.soff, bs.buf, bq.buf, bq.qoff, bx, by, ka, bcnt, bbase, bbkt, rows, npos, ne)
    }
}

// gemma4 router-input norm rows (weightless rms * 1/sqrt(dim) * rscale, one tg per position)
// the MXFP4 gathered expert mul_mm (gpt-oss)
def private pf_enc_moe_mm_mx4(enc : MetalComputeEncoder?; t : Model; stack_off, rows, npos : int64;
                              bx, by, bcnt, bbase, bbkt, bwb : MetalBuffer?; var ka : MoeMmArgs;
                              contiguous : bool = false) {
    let ne = t.config.n_expert
    let mp = mx4_of(g_dev, t, stack_off)
    if (g_pf_moe_mm_mx4_tensor && contiguous && g_pf_pso_moe_mm_mx4_t != null) {
        pf_enc_moe_mm_mx4_t_c(enc, mp.qbuf, mp.qoff, mp.sbuf, mp.soff, bx, by, ka, bcnt, bbase, bwb, g_mx4_vtab, rows, npos, ne)
    } else {
        pf_enc_moe_mm_mx4_c(enc, mp.qbuf, mp.qoff, mp.sbuf, mp.soff, bx, by, ka, bcnt, bbase, bbkt, bwb, g_mx4_vtab, rows, npos, ne)
    }
}

// pf_enc_moe_reduce is [metal_dispatch]-generated from MetalMoeReduce (bsel binds the float
// w view — row layout in-kernel).

// enc_add_bias_rows is [metal_dispatch]-generated from MetalAddBiasRows (x binds at the panel
// offset, exact total*4 staging). Per-head QK-norm uses the kernels-side enc_qk_norm builder
// directly — the prefill twin was the same grid/tg with an x offset, now a builder param.

// enc_rope is [metal_dispatch]-generated from MetalRope (rot @default = head_size — full rope on null).




// enc_qk / enc_av / enc_softmax are [metal_dispatch]-generated from MetalAttnQK / MetalAttnAV /
// MetalAttnSoftmax (sink @default = g_one — identity on null). The mm wrappers below pick the
// Metal-4 tensor twin's generated builder (_t_c) when it compiled.

def enc_qk_mm(enc : MetalComputeEncoder?; bq, bk, batt : MetalBuffer?; var ka : AttnArgs; mp, nk64, heads : int64) {   // public: the tower driver dispatches the crowned trio too
    if (g_pf_qkmm_tensor && g_pf_pso_qkmm_t != null) {
        enc_qk_mm_t_c(enc, bq, bk, batt, ka, mp, nk64, heads)
    } else {
        enc_qk_mm_c(enc, bq, bk, batt, ka, mp, nk64, heads)
    }
}

def enc_av_mm(enc : MetalComputeEncoder?; batt, bv, bxb : MetalBuffer?; var ka : AttnArgs; mp, heads, head_size : int64) {   // public: the tower driver dispatches the crowned trio too
    if (g_pf_avmm_tensor && g_pf_pso_avmm_t != null) {
        enc_av_mm_t_c(enc, batt, bv, bxb, ka, mp, heads, head_size)
    } else {
        enc_av_mm_c(enc, batt, bv, bxb, ka, mp, heads, head_size)
    }
}

// swiglu and add share the (a, b, total) elementwise shape
// ===== deltanet prefill encoders (Wave D) — the prefill-local twins of the decode-side set =====
// hist/state bind the per-session DnMirror arena at the layer's byte offset; the rest is window scratch.

// pf_enc_dn_ba is [metal_dispatch]-generated from MetalDnBa (kernels-side class — the dual-view
// q8 blob pair binds at boff, y span d*np*4 exact).

// batched f32-slab GEMV riding MetalMoeRouterB's generated builder: d rows per position
// (dn_ba_f32 β/α d = dt_rank; the shexp gate d = 1); y[p*d + row] — the pf_enc_dn_ba layout.
// The thin wrapper carries the dummy binds (rb = g_one dead, hasb = g_zero) per the lens doctrine.
def private pf_enc_slab_gemv_b(enc : MetalComputeEncoder?; bw, bx, by, bn, bd, bnp : MetalBuffer?; d, npos : int64) {
    enc_moe_router_b(enc, bw, bx, by, bn, bd, bn, g_one, g_zero, bnp, d, npos)   // bxs = bn: per-position x stride = dim
}

// pf_enc_swiglu_oai is [metal_dispatch]-generated from MetalSwigluOaiPf (float4-lane swiglu_oai);
// its total/256 grid == the hand (total/4 + 63)/64 under the callers' total % 4 == 0 contract —
// off-contract the ceil form is the larger side, and the kernel guards gid * 4 >= total.

// GPU-PLE gate (register_ple_gpu_gate): true iff THIS window's metal prefill will engage AND
// the pre-step kernels cover the model — q8 emb table, kept-bf16 model_proj, slice/shape gates.
[unused_argument(s, start_pos)]
def private metal_ple_pre_gpu_gate(t : Model; s : Session; npos, start_pos : int64) : bool {
    if (active_prefill_override() != "metal" ||
            !t.metal_blob || t.ple_emb_fmt != KqFmt.q8 || !t.ple_model_bf16) {
        return false
    }
    let ple = t.config.n_embd_per_layer
    // CAPABILITY only — the rope-rows leg would answer "not yet" every time at this point
    if (ple % 32l != 0l || ple > 1024l || (t.config.n_layers * ple) % 64l != 0l ||
            prefill_decline_caps(t, npos) != MetalPrefillDecline.none) {
        return false
    }
    return metal_prefill_init()
}

[hot_path]
def metal_prefill_forward(t : Model; var s : Session; npos, start_pos : int64) : bool {   // nolint:STYLE037,STYLE038 — the prefill step driver; the layer stack's phases are hz-barrier coupled
    weight_caches_flush_on_reload()
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
    let kv_mul = uint(n_heads / c.n_kv_heads)
    let scale = c.attn_scale > 0.0 ? c.attn_scale : 1.0 / sqrt(float(head_size))
    let half = (c.rope_dim > 0l ? c.rope_dim : head_size) / 2l   // partial rope: table rows are rope_dim/2 wide
    // deltanet (Wave D): the recurrent-state mirror must be resident/synced BEFORE encoding —
    // prepare zeroes at pos 0 or syncs a CPU prefix; a cold mid-stream window has no repair path
    let dn = c.recr_mask != 0ul
    var bdn_state : MetalBuffer?
    var dn_state_base = 0ul
    var dn_conv_base = 0ul
    if (dn) {
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
    var bx = pool_acquire(g_pf_pool, g_dev, bytes_x)
    var bxb = pool_acquire(g_pf_pool, g_dev, bytes_xb)
    var bxb2 = pool_acquire(g_pf_pool, g_dev, bytes_x)
    var bq = pool_acquire(g_pf_pool, g_dev, bytes_q)
    var bhb = pool_acquire(g_pf_pool, g_dev, bytes_h)
    var bhb2 = pool_acquire(g_pf_pool, g_dev, bytes_h)
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
    var bmlg = moe ? pool_acquire(g_pf_pool, g_dev, bytes_mlg) : null
    var bmsel = moe ? pool_acquire(g_pf_pool, g_dev, bytes_msel) : null
    var bmcnt = moe ? pool_acquire(g_pf_pool, g_dev, bytes_mcsr) : null
    var bmbase = moe ? pool_acquire(g_pf_pool, g_dev, bytes_mcsr) : null
    var bmbkt = moe ? pool_acquire(g_pf_pool, g_dev, bytes_mbkt) : null
    var bminv = moe ? pool_acquire(g_pf_pool, g_dev, bytes_minv) : null
    var bmg = moe ? pool_acquire(g_pf_pool, g_dev, bytes_mg) : null
    var bmg2 = moe ? pool_acquire(g_pf_pool, g_dev, bytes_mg) : null
    var bmdn = moe ? pool_acquire(g_pf_pool, g_dev, bytes_mdn) : null
    var bmg4rt = g4moe ? pool_acquire(g_pf_pool, g_dev, bytes_x) : null
    // deltanet scratch (recurrent layers) — GEMM outputs pad to mp rows like every panel
    let cd_dn = dn ? dn_conv_dim(c) : 0l
    let di_dn = dn ? c.ssm_d_inner : 0l
    let nvh_dn = dn ? c.ssm_dt_rank : 0l
    let bytes_dn_cd = uint64(mp * cd_dn * 4l)
    let bytes_dn_di = uint64(mp * di_dn * 4l)
    let bytes_dn_h = uint64(mp * nvh_dn * 4l)
    let bytes_dn_qg = uint64(mp * 2l * qd * 4l)
    var bdnqkv = dn ? pool_acquire(g_pf_pool, g_dev, bytes_dn_cd) : null
    var bdncv = dn ? pool_acquire(g_pf_pool, g_dev, bytes_dn_cd) : null
    var bdnz = dn ? pool_acquire(g_pf_pool, g_dev, bytes_dn_di) : null
    var bdno = dn ? pool_acquire(g_pf_pool, g_dev, bytes_dn_di) : null
    var bdnb = dn ? pool_acquire(g_pf_pool, g_dev, bytes_dn_h) : null
    var bdng = dn ? pool_acquire(g_pf_pool, g_dev, bytes_dn_h) : null
    var bdnqg = c.q_gated ? pool_acquire(g_pf_pool, g_dev, bytes_dn_qg) : null
    var bdngate = c.q_gated ? pool_acquire(g_pf_pool, g_dev, bytes_q) : null
    var batt = pool_acquire(g_pf_pool, g_dev, bytes_att)
    var bcos = pool_acquire_untracked(g_pf_upool, g_dev, bytes_tab)
    var bsin = pool_acquire_untracked(g_pf_upool, g_dev, bytes_tab)
    let dual_rope = has_dual_rope(c)
    var bcos_swa = dual_rope ? pool_acquire_untracked(g_pf_upool, g_dev, bytes_tab_sw) : null
    var bsin_swa = dual_rope ? pool_acquire_untracked(g_pf_upool, g_dev, bytes_tab_sw) : null
    var bks & = unsafe(g_pf_bks)
    var bvs & = unsafe(g_pf_bvs)
    bks |> clear()
    bvs |> clear()
    bks |> reserve(c.n_layers + c.n_layer_nextn)
    bvs |> reserve(c.n_layers + c.n_layer_nextn)
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
        bks |> push(pool_acquire(g_pf_pool, g_dev, bytes_kv_l))
        bvs |> push(pool_acquire(g_pf_pool, g_dev, bytes_kv_l))
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
        bpleg = pool_acquire(g_pf_pool, g_dev, bytes_ple_g)
        if (ple_gpu) {
            bple = pool_acquire(g_pf_pool, g_dev, bytes_ple_side)
            bple_proj = pool_acquire(g_pf_pool, g_dev, bytes_ple_proj)
            bple_tok = pool_acquire_untracked(g_pf_upool, g_dev, uint64(npos * 4l))
            unsafe {
                var tp = reinterpret<uint?>(metal_buffer_contents(bple_tok))
                for (p in range64(npos)) {
                    tp[p] = uint(s.ple_gpu_tokens[p])
                }
            }
        } else {
            bple = pool_acquire_untracked(g_pf_upool, g_dev, bytes_ple_side)
            unsafe {
                // transpose s.ple_inp (position-major, p*all + l*ple) into layer-major blocks
                var dst = reinterpret<float?>(metal_buffer_contents(bple))
                let src = addr < float const? >(s.ple_inp[0])
                for (l in range64(c.n_layers)) {
                    for (p in range64(npos)) {
                        memcpy(reinterpret<void?>(dst + (l * npos + p) * ple_n),
                            reinterpret<void const?>(src + p * ple_all + l * ple_n), ple_n * 4l)
                    }
                }
            }
        }
    }
    // upload the embedded rows + this prefill's rope table(s) (built for the chunk's GLOBAL
    // positions — build_rope_table takes start_pos; dual-rope: the sliding class's pair too)
    unsafe {
        memcpy(metal_buffer_contents(bx), addr < void? >(s.x_b[0]), npos * dim * 4l)
        memcpy(metal_buffer_contents(bcos), addr < void? >(s.rope_cos[0]), npos * half * 4l)
        memcpy(metal_buffer_contents(bsin), addr < void? >(s.rope_sin[0]), npos * half * 4l)
        if (dual_rope) {
            memcpy(metal_buffer_contents(bcos_swa), addr < void? >(s.rope_cos_swa[0]), npos * half_sw * 4l)
            memcpy(metal_buffer_contents(bsin_swa), addr < void? >(s.rope_sin_swa[0]), npos * half_sw * 4l)
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
    var u_dim = uniform_u32(uint(dim))
    var u_qd = uniform_u32(uint(qd))
    var u_kvd = uniform_u32(uint(kv_dim))
    var u_hidden = uniform_u32(uint(hidden))
    var u_hs = uniform_u32(uint(head_size))
    var u_npairs_q = uniform_u32(uint(npos * qd / 2l))
    var u_npairs_k = uniform_u32(uint(npos * kv_dim / 2l))
    var u_neox = uniform_u32(c.rope_neox ? 1u : 0u)
    let has_bias = c.attn_qkv_bias
    var u_totq = has_bias ? uniform_u32(uint(npos * qd)) : null
    var u_totkv = has_bias ? uniform_u32(uint(npos * kv_dim)) : null
    let has_qkn = c.qk_norm
    let vfromk = c.v_from_k_mask != 0ul
    var u_total_dim = uniform_u32(uint(npos * dim))
    var u_total_h = uniform_u32(uint(npos * hidden))
    var u_moe_ne = moe ? uniform_u32(uint(c.n_expert)) : null
    var u_moe_nk = moe ? uniform_u32(uint(c.n_expert_used)) : null
    var u_moe_np = moe ? uniform_u32(uint(npos)) : null
    var u_moe_tot = moe ? uniform_u32(uint(mtot)) : null
    var u_moe_nfe = moe ? uniform_u32(uint(c.n_ff_exp)) : null
    var u_moe_g0 = moe ? uniform_u32(0u) : null
    var u_moe_ptot = moe ? uniform_u32(uint(mpad * c.n_ff_exp)) : null
    var u_moe_invd = g4moe ? uniform_f32(1.0 / sqrt(float(dim))) : null
    var u_moe_gmode = moe ? uniform_u32(c.moe_gate == MoeGate.softmax_weight ? 1u : (c.norm_topk_prob ? 0u : 2u)) : null
    // shared expert (qwen35moe): nsh width + npos*nsh act span + the gate GEMV's 1-row bound
    var u_moe_nsh = nsh > 0l ? uniform_u32(uint(nsh)) : null
    var u_moe_shtot = nsh > 0l ? uniform_u32(uint(npos * nsh)) : null
    var u_moe_one = nsh > 0l ? uniform_u32(1u) : null
    var u_eps = uniform_f32(c.norm_eps)
    // deltanet + gated-attention uniforms (Wave D)
    var u_dn_cd = dn ? uniform_u32(uint(cd_dn)) : null
    var u_dn_di = dn ? uniform_u32(uint(di_dn)) : null
    var u_dn_nvh = dn ? uniform_u32(uint(nvh_dn)) : null
    var u_dn_np = dn ? uniform_u32(uint(npos)) : null
    var u_dn_qd2 = c.q_gated ? uniform_u32(uint(2l * qd)) : null
    var u_qtot = c.q_gated ? uniform_u32(uint(npos * qd)) : null
    var u_rot = (c.rope_dim > 0l && c.rope_dim != head_size) ? uniform_u32(uint(c.rope_dim)) : null
    // E-series PLE: gate/proj widths + the per-layer geglu ⊙ span
    var u_ple = plef ? uniform_u32(uint(ple_n)) : null
    var u_ple_tot = plef ? uniform_u32(uint(npos * ple_n)) : null
    var u_ple_all = ple_gpu ? uniform_u32(uint(ple_all)) : null
    // the SLIDING-class uniform twins (hetero models — uniform models leave these null and
    // every layer binds the base set)
    var u_qd_sw = hetero ? uniform_u32(uint(qd_sw)) : null
    var u_kvd_sw = hetero ? uniform_u32(uint(kvd_sw)) : null
    var u_hs_sw = hetero ? uniform_u32(uint(hs_sw)) : null
    var u_npairs_q_sw = hetero ? uniform_u32(uint(npos * qd_sw / 2l)) : null
    var u_npairs_k_sw = hetero ? uniform_u32(uint(npos * kvd_sw / 2l)) : null
    // the V-from-K panel copy's grid guards (npos rows of the class's kv width)
    var u_vtot = vfromk ? uniform_u32(uint(npos * kv_dim)) : null
    var u_vtot_sw = (vfromk && hetero) ? uniform_u32(uint(npos * kvd_sw)) : null
    // logits-on-GPU (default ON, DASLLAMA_METAL_LOGITS=0 pins the CPU classifier): last-position
    // rmsnorm + classifier GEMV ride the last command buffer (prefill_override_logits_done skips
    // the CPU final step); q8 classifiers only, softcap + suppress ride epilogue kernels
    let cls_fmt = c.shared_weights ? t.emb_fmt : t.wcls_fmt
    let tied_f32 = c.shared_weights && !t.cls_q8 && cls_fmt == KqFmt.q8
    // knockout mode keeps gpu logits: constant across skip configs (deltas stay clean), and a
    // blob-only metal-flavor load HAS no CPU classifier to fall back to. NextN models serve
    // too — the readback stashes mtp_h from bxf (forward_prefill's handoff contract)
    let gpu_logits = (!g_pf_logits_cpu && g_pf_env_logits &&
        !tied_f32 &&
        t.wcls_off >= 0l)
    var bxf = gpu_logits ? pool_acquire(g_pf_pool, g_dev, uint64(dim * 4l)) : null
    var blog = gpu_logits ? pool_acquire(g_pf_pool, g_dev, uint64(c.vocab_size * 4l)) : null
    var u_vocab = gpu_logits ? uniform_u32(uint(c.vocab_size)) : null
    var u_flcap = (gpu_logits && c.final_logit_softcap > 0.0) ? uniform_f32(c.final_logit_softcap) : null
    // suppressed token ids pin to -1e30 AFTER the cap (gemma4) — the CPU classifier's order
    let nsupp = long_length(t.suppress)
    var bsupp : MetalBuffer?
    var u_nsupp : MetalBuffer?
    if (gpu_logits && nsupp > 0l) {
        bsupp = pool_acquire_untracked(g_pf_upool, g_dev, uint64(nsupp * 4l))
        unsafe {
            var sp = reinterpret<uint?>(metal_buffer_contents(bsupp))
            for (i in range64(nsupp)) {
                sp[i] = uint(t.suppress[i])
            }
        }
        u_nsupp = uniform_u32(uint(nsupp))
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
        bwm_res = pool_acquire(g_pf_pool, g_dev, uint64(npos * dim * 4l))
        bwm_emb = pool_acquire(g_pf_pool, g_dev, uint64(npos * dim * 4l))
        bwm_cat = pool_acquire(g_pf_pool, g_dev, uint64(mp * 2l * dim * 4l))
        bwm_tmp = pool_acquire(g_pf_pool, g_dev, uint64(npos * dim * 4l))
        u_dim2 = uniform_u32(uint(2l * dim))
        u_wm_tot = uniform_u32(uint(npos * dim))
        // shifted embeds: row p pairs the NEXT position's embed with hidden p — s.x_b still
        // holds the window's embed panel here; the garbage last row reuses row 0
        unsafe {
            var dst = reinterpret<uint8?>(metal_buffer_contents(bwm_emb))
            memcpy(reinterpret<void?>(dst), addr < void? >(s.x_b[dim]), (npos - 1l) * dim * 4l)
            memcpy(reinterpret<void?>(dst + (npos - 1l) * dim * 4l), addr < void? >(s.x_b[0]), dim * 4l)
        }
    }
    // ggml-geometry QK/AV (default): ~10x the old trio GEMMs' rate; needs hs % 64 (the AV grid;
    // BOTH classes) and mp % 64 (the QK key grid — true by construction). DASLLAMA_METAL_ATTN=0
    // pins the trio.
    let mmattn = (head_size % 64l) == 0l && (hs_sw % 64l) == 0l && g_pf_env_attn
    let ts_enc = ref_time_ticks()
    let setup_us = get_time_usec(ts_all)
    var err : string
    var gpu_ms : double
    var encode_ms : double
    // command-buffer split (DASLLAMA_METAL_NCB chunks): each chunk commits as soon as encoded, so
    // the scheduler analyzes chunk k while chunk k-1 executes. DASLLAMA_METAL_UNRETAINED=1 skips
    // per-dispatch retain/release too. Default ~4 layers/chunk (3B: slack 57ms -> 9ms, +10% pp512).
    let ncb = clamp(int64(g_pf_env_ncb > 0 ? g_pf_env_ncb : int((n_enc_layers + 3l) / 4l)), 1l, n_enc_layers)
    let unret = g_pf_env_unretained
    let layers_per_cb = (n_enc_layers + ncb - 1l) / ncb
    // the capture rail: kn_* record each chunk as a step graph, graph_flush_sched replays on a
    // CONCURRENT encoder (auto-schedule + hz barriers at real hazards). DASLLAMA_METAL_SCHED=0
    // keeps capture order; DASLLAMA_METAL_PF_CAPTURE=0 = the direct serial-encode rollback.
    let cap = g_pf_env_capture
    var cbs & = unsafe(g_pf_cbs)
    cbs |> clear()
    cbs |> reserve(ncb)
    var cb : MetalCommandBuffer?
    var enc : MetalComputeEncoder?
    var dnli = 0l   // recurrent-layer index — addresses the dn state/conv mirror slices in order
    {
        for (l in range64(n_enc_layers)) {
            if (l % layers_per_cb == 0l) {
                if (enc != null) {
                    if (cap) {
                        graph_flush_sched(enc)
                    }
                    metal_end_encoding(enc)
                    metal_release(enc)
                    metal_commit(cb)
                    cbs |> push(cb)
                }
                cb = unret ? metal_new_command_buffer_unretained(g_queue) : metal_new_command_buffer(g_queue)
                enc = cap ? metal_new_compute_encoder_concurrent(cb) : metal_new_compute_encoder(cb)
                if (cap) {
                    gr_arm(true)
                }
            }
            if (l == 0l && ple_gpu) {
                // PLE pre-step on device (the serial encoder orders the three stages): q8 token-row
                // gather (layer-major, sqrt(ple) folded) -> bf16 model_proj GEMM off bx -> rms+combine
                unsafe {
                    var pka = ple_ka(npos, ple_n, c.n_layers, dim, c.norm_eps)
                    let bemb = blob_of(g_dev, t, t.ple_emb_off)
                    pf_enc_ple_gather(enc, bemb.buf, bemb.boff, bemb.buf, bple_tok, bple, pka, c.n_layers, npos, ple_n)
                    let bwp = bf16_of(g_dev, t, t.ple_model_off)
                    pf_enc_bf16_mm(enc, bwp.buf, bwp.boff, bx, bple_proj, u_dim, u_ple_all, mp, ple_all)
                    var bw_pn = upload_region(addr < void? >(t.fblob[t.ple_pnorm_off]), uint64(ple_n * 4l))
                    pf_enc_ple_finish(enc, bple_proj, bw_pn, bple, pka, c.n_layers, npos, ple_n)
                }
                s.ple_gpu_pending = false
            }
            if (warm && l == c.n_layers) {
                // trunk done — save the residual panel (x_b readback + cls read it), then swap
                // bx to the draft block's eh_proj input; the body below runs the draft layer
                // verbatim (same uniforms, npos rows, K/V panel + weight arrays at index l)
                unsafe {
                    enc_copy_row(enc, bx, 0ul, bwm_res, 0ul, u_wm_tot, npos * dim)
                    var bw_en = upload_region(addr < void? >(t.fblob[t.mtp_enorm_off]), uint64(dim * 4l))
                    pf_enc_rms(enc, bwm_emb, 0ul, bw_en, bxb, u_dim, u_eps, npos)
                    var bw_fin = upload_region(addr < void? >(t.fblob[t.rms_final_off]), uint64(dim * 4l))
                    pf_enc_rms(enc, bx, 0ul, bw_fin, bwm_tmp, u_dim, u_eps, npos)
                    var bw_hn = upload_region(addr < void? >(t.fblob[t.mtp_hnorm_off]), uint64(dim * 4l))
                    pf_enc_rms(enc, bwm_tmp, 0ul, bw_hn, bxb2, u_dim, u_eps, npos)
                    pf_enc_cat2(enc, bwm_cat, bxb, bxb2, u_dim, u_wm_tot, npos * dim)
                    if (t.mtp_ehproj_fmt == KqFmt.q8) {
                        let behp = blob_of(g_dev, t, t.mtp_ehproj_off)
                        enc_gemm_mm(enc, behp.buf, behp.boff, bwm_cat, bx, u_dim2, u_dim, mp, dim)
                    } else {
                        pf_enc_kq_site_mm(enc, t, t.mtp_ehproj_fmt, t.mtp_ehproj_off, dim, bwm_cat, bx, u_dim2, u_dim, mp)
                    }
                }
            }
            unsafe {
                var bw_att = upload_region(addr < void? >(t.fblob[t.rms_att_off + l * dim]), uint64(dim * 4l))
                var bw_ffn = upload_region(addr < void? >(t.fblob[t.rms_ffn_off + l * dim]), uint64(dim * 4l))
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
                    pf_enc_rms(enc, bx, 0ul, bw_att, bxb, u_dim, u_eps, npos)
                }
                if (recr) {
                    // deltanet layer (Wave D): batched projections -> conv+SiLU -> q/k L2 -> fused
                    // sequential scan (state walks the whole window in-kernel) -> gated out-norm -> out-proj
                    if (g_pf_skip != "gemm") {
                        let bwqkv = blob_of(g_dev, t, t.dnqkv_offs[l])
                        enc_gemm_mm(enc, bwqkv.buf, bwqkv.boff, bxb, bdnqkv, u_dim, u_dn_cd, mp, cd_dn)
                        let bwz = blob_of(g_dev, t, t.dngate_offs[l])
                        enc_gemm_mm(enc, bwz.buf, bwz.boff, bxb, bdnz, u_dim, u_dn_di, mp, di_dn)
                        if (t.dn_ba_f32) {
                            // F32-on-disk β/α (35B): fblob slabs ride the router-b f32 GEMV
                            var bw_bt = upload_region(addr < void? >(t.fblob[t.dn_betaf_off + l * dim * nvh_dn]), uint64(dim * nvh_dn * 4l))
                            pf_enc_slab_gemv_b(enc, bw_bt, bxb, bdnb, u_dim, u_dn_nvh, u_dn_np, nvh_dn, npos)
                            var bw_al = upload_region(addr < void? >(t.fblob[t.dn_alphaf_off + l * dim * nvh_dn]), uint64(dim * nvh_dn * 4l))
                            pf_enc_slab_gemv_b(enc, bw_al, bxb, bdng, u_dim, u_dn_nvh, u_dn_np, nvh_dn, npos)
                        } else {
                            let bwb = blob_of(g_dev, t, t.dnbeta_offs[l])
                            pf_enc_dn_ba(enc, bwb.buf, bwb.boff, bwb.buf, bxb, bdnb, 0ul, u_dim, u_dn_nvh, nvh_dn, npos)
                            let bwa = blob_of(g_dev, t, t.dnalpha_offs[l])
                            pf_enc_dn_ba(enc, bwa.buf, bwa.boff, bwa.buf, bxb, bdng, 0ul, u_dim, u_dn_nvh, nvh_dn, npos)
                        }
                    }
                    if (g_pf_skip != "attn" && g_pf_skip != "nongemm") {
                        let soff = dn_state_base + uint64(dnli * nvh_dn * c.ssm_d_state * c.ssm_d_state * 4l)
                        let hoff = dn_conv_base + uint64(dnli * (c.ssm_d_conv - 1l) * cd_dn * 4l)
                        var bw_cv = upload_region(addr < void? >(t.fblob[t.dn_conv_off + l * c.ssm_d_conv * cd_dn]), uint64(c.ssm_d_conv * cd_dn * 4l))
                        var dka = dn_args(c, npos, false)
                        enc_dn_conv(enc, bdnqkv, bdn_state, hoff, bw_cv, bdncv, dka, cd_dn, npos)
                        enc_dn_conv_hist(enc, bdnqkv, bdn_state, hoff, bdn_state, hoff, dka, cd_dn)
                        enc_dn_l2norm(enc, bdncv, dka, 2l * c.ssm_n_group, npos)
                        var baa = upload_region(addr < void? >(t.fblob[t.dn_a_off + l * nvh_dn]), uint64(nvh_dn * 4l))
                        var bdt = upload_region(addr < void? >(t.fblob[t.dn_dt_off + l * nvh_dn]), uint64(nvh_dn * 4l))
                        enc_dn_scan(enc, bdncv, bdn_state, soff, bdnb, bdng, baa, bdt, bdno,
                            bdn_state, soff, dka, c.ssm_d_state, nvh_dn)
                        var bwn = upload_region(addr < void? >(t.fblob[t.dn_norm_off + l * c.ssm_d_state]), uint64(c.ssm_d_state * 4l))
                        enc_dn_gate(enc, bdno, bdnz, bwn, dka, nvh_dn, npos)
                    }
                    if (g_pf_skip != "gemm") {
                        let bwo = blob_of(g_dev, t, t.dnout_offs[l])
                        enc_gemm_mm(enc, bwo.buf, bwo.boff, bdno, bxb2, u_dn_di, u_dim, mp, dim)
                    }
                    dnli++
                }
                if (!recr && g_pf_skip != "gemm") {
                    // K-quant tensors take their mul_mm twins per tensor (the gate pins the
                    // mm shape for kquant loads); q8-tagged tensors keep the blob path
                    let fq = fmt_at(t.wq_fmt, l)
                    let fk = fmt_at(t.wk_fmt, l)
                    let fv = has_wv ? fmt_at(t.wv_fmt, l) : KqFmt.q8
                    if (fq != KqFmt.q8 || fk != KqFmt.q8 || fv != KqFmt.q8) {
                        if (c.q_gated) {
                            // 2x-wide packed [q|gate] projection, deinterleaved into the q panel + gate rows
                            if (fq != KqFmt.q8) {
                                pf_enc_kq_site_mm(enc, t, fq, t.wq_offs[l], 2l * qd_l, bxb, bdnqg, u_dim, u_dn_qd2, mp)
                            } else {
                                let bwqb = blob_of(g_dev, t, t.wq_offs[l])
                                enc_gemm_mm(enc, bwqb.buf, bwqb.boff, bxb, bdnqg, u_dim, u_dn_qd2, mp, 2l * qd_l)
                            }
                            enc_dn_deint(enc, bdnqg, bq, bdngate,
                                DnDeintArgs(hs = uint(hs_l), qd = uint(qd_l), qs = uint(qd_l)), qd_l, npos)
                        } elif (fq != KqFmt.q8) {
                            pf_enc_kq_site_mm(enc, t, fq, t.wq_offs[l], qd_l, bxb, bq, u_dim, u_qd_l, mp)
                        } else {
                            let bwqb = blob_of(g_dev, t, t.wq_offs[l])
                            enc_gemm_mm(enc, bwqb.buf, bwqb.boff, bxb, bq, u_dim, u_qd_l, mp, qd_l)
                        }
                        if (!kvsh) {
                            if (fk != KqFmt.q8) {
                                pf_enc_kq_site_mm(enc, t, fk, t.wk_offs[l], kvd_l, bxb, bks[l], u_dim, u_kvd_l, mp, koff)
                            } else {
                                let bwkb = blob_of(g_dev, t, t.wk_offs[l])
                                enc_gemm_mm(enc, bwkb.buf, bwkb.boff, bxb, bks[l], u_dim, u_kvd_l, mp, kvd_l, koff)
                            }
                            if (has_wv) {
                                if (fv != KqFmt.q8) {
                                    pf_enc_kq_site_mm(enc, t, fv, t.wv_offs[l], kvd_l, bxb, bvs[l], u_dim, u_kvd_l, mp, koff)
                                } else {
                                    let bwvb = blob_of(g_dev, t, t.wv_offs[l])
                                    enc_gemm_mm(enc, bwvb.buf, bwvb.boff, bxb, bvs[l], u_dim, u_kvd_l, mp, kvd_l, koff)
                                }
                            }
                        }
                    } else {
                        let bwqb = blob_of(g_dev, t, t.wq_offs[l])
                        if (c.q_gated) {
                            // 2x-wide packed [q|gate] projection, deinterleaved into the q panel + gate rows
                            enc_gemm_mm(enc, bwqb.buf, bwqb.boff, bxb, bdnqg, u_dim, u_dn_qd2, mp, 2l * qd_l)
                            enc_dn_deint(enc, bdnqg, bq, bdngate,
                                DnDeintArgs(hs = uint(hs_l), qd = uint(qd_l), qs = uint(qd_l)), qd_l, npos)
                        } else {
                            enc_gemm_mm(enc, bwqb.buf, bwqb.boff, bxb, bq, u_dim, u_qd_l, mp, qd_l)
                        }
                        if (!kvsh) {   // shared layers have no K/V tensors (offs -1)
                            let bwkb = blob_of(g_dev, t, t.wk_offs[l])
                            enc_gemm_mm(enc, bwkb.buf, bwkb.boff, bxb, bks[l], u_dim, u_kvd_l, mp, kvd_l, koff)
                            if (has_wv) {
                                let bwvb = blob_of(g_dev, t, t.wv_offs[l])
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
                        enc_copy_row(enc, bks[l], koff, bvs[l], koff, het ? u_vtot_sw : u_vtot, npos * kvd_l)
                    }
                    if (c.v_norm) {
                        // MAX-width ones upload: the region cache is address-keyed and both
                        // classes read this one plane (the single-decode lesson)
                        var bones = upload_region(addr < void? >(t.fblob[t.rms_ones_off]), uint64(max_head_size(c) * 4l))
                        var vka = QkNormArgs(nq = 0u, koffe = 0u, rstride = uint(kvd_l),
                            hs = uint(hs_l), woffe = 0u, eps = c.norm_eps, soffe = 0u)
                        enc_qk_norm(enc, bvs[l], koff, bones, vka, kvd_l / hs_l, npos, hs_l)
                    }
                }
                if (!recr && has_bias && g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                    // QKV bias before rope — chunk rows only (existing rows stored biased+roped).
                    // Per-layer widths for the offsets; the count uniforms stay base-class (no
                    // hetero model carries QKV bias — the CPU bias-plane layout is uniform too)
                    var bbq = upload_region(addr < void? >(t.bq[l * qd_l]), uint64(qd_l * 4l))
                    var bbk = upload_region(addr < void? >(t.bk[l * kvd_l]), uint64(kvd_l * 4l))
                    var bbv = upload_region(addr < void? >(t.bv[l * kvd_l]), uint64(kvd_l * 4l))
                    enc_add_bias_rows(enc, bq, 0ul, bbq, 0ul, u_qd, u_totq, npos * qd)
                    enc_add_bias_rows(enc, bks[l], koff, bbk, 0ul, u_kvd, u_totkv, npos * kv_dim)
                    enc_add_bias_rows(enc, bvs[l], koff, bbv, 0ul, u_kvd, u_totkv, npos * kv_dim)
                }
                if (!recr && has_qkn && g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                    // per-head QK-norm before rope — the chunk's q panel and its new K rows
                    var bqw = upload_region(addr < void? >(t.fblob[t.rms_q_offs[l]]), uint64(hs_l * 4l))
                    var qka = QkNormArgs(nq = uint(n_heads), koffe = uint(qd_l), rstride = uint(qd_l),
                        hs = uint(hs_l), woffe = uint(hs_l), eps = c.norm_eps, soffe = uint(qd_l))
                    enc_qk_norm(enc, bq, 0ul, bqw, qka, n_heads, npos, hs_l)
                    if (!kvsh) {
                        var bkw = upload_region(addr < void? >(t.fblob[t.rms_k_offs[l]]), uint64(hs_l * 4l))
                        var kka = QkNormArgs(nq = 0u, koffe = 0u, rstride = uint(kvd_l),
                            hs = uint(hs_l), woffe = 0u, eps = c.norm_eps, soffe = 0u)
                        enc_qk_norm(enc, bks[l], koff, bkw, kka, kvd_l / hs_l, npos, hs_l)
                    }
                }
                if (!recr && g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                    // sliding layers rope with the swa row pair (dual-rope configs — gemma3/4;
                    // hetero: the swa pair's rows are hs_sw/2 wide — the class's own width)
                    var bcos_l = lsl && dual_rope ? bcos_swa : bcos
                    var bsin_l = lsl && dual_rope ? bsin_swa : bsin
                    enc_rope(enc, bq, 0ul, bcos_l, bsin_l, u_qd_l, u_hs_l, u_npairs_q_l, u_neox, u_rot, npos * qd_l / 2l)
                    if (!kvsh) {
                        enc_rope(enc, bks[l], koff, bcos_l, bsin_l, u_kvd_l, u_hs_l, u_npairs_k_l, u_neox, u_rot, npos * kvd_l / 2l)
                    }
                }
                if (!recr && g_pf_skip != "attn" && g_pf_skip != "nongemm") {
                    // sliding layers bind the span — the QK forms skip fully-below-window blocks,
                    // the softmax masks + zeroes the below-window prefix; global layers bind 0
                    // the trio walks one slab, so its whole geometry derives ONCE here
                    var atka = AttnArgs(qd = uint(qd_l), kv_dim = uint(kvd_l), head_size = uint(hs_l),
                        kv_mul = het ? uint(n_heads / kvh_sw) : kv_mul, npos = uint(nk),
                        np32 = uint(nk64),
                        scale = het ? (c.attn_scale > 0.0 ? c.attn_scale : 1.0 / sqrt(float(hs_sw))) : scale,
                        qoff = uint(start_pos), qrows = uint(mp),
                        window = uint(lsl && c.sliding_window > 0l ? c.sliding_window : 0l),
                        softcap = c.attn_logit_softcap, hass = c.attn_sinks ? 1u : 0u,
                        uend = s.attn_uniform_end > 0l ? uint(s.attn_uniform_end) : 0u)
                    if (g_pf_skip != "attn_qk") {
                        if (mmattn) {
                            enc_qk_mm(enc, bq, bks[l], batt, atka, mp, nk64, n_heads)
                        } else {
                            enc_qk(enc, bq, bks[l], batt, atka, mp, nk64, n_heads)
                        }
                    }
                    if (g_pf_skip != "attn_sm") {
                        var bsnk : MetalBuffer?
                        if (c.attn_sinks) {
                            bsnk = upload_region(unsafe(addr < void? >(t.fblob[t.sinks_off + l * n_heads])), uint64(n_heads * 4l))
                        }
                        enc_softmax(enc, batt, atka, bsnk, npos, n_heads)
                    }
                    if (g_pf_skip != "attn_av") {
                        if (mmattn) {
                            enc_av_mm(enc, batt, bvs[l], bxb, atka, mp, n_heads, hs_l)
                        } else {
                            enc_av(enc, batt, bvs[l], bxb, atka, mp, n_heads, hs_l)
                        }
                    }
                }
                if (!recr && c.q_gated && g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                    // gated-attention epilogue: out ⊙ σ(gate) between the attention output and wo (the CPU order)
                    enc_sigmul(enc, bxb, 0ul, bdngate, 0ul, u_qtot, npos * qd)
                }
                if (!recr && g_pf_skip != "gemm") {
                    let fo = fmt_at(t.wo_fmt, l)
                    if (fo != KqFmt.q8) {
                        pf_enc_kq_site_mm(enc, t, fo, t.wo_offs[l], dim, bxb, bxb2, u_qd_l, u_dim, mp)
                    } else {
                        let bwob = blob_of(g_dev, t, t.wo_offs[l])
                        enc_gemm_mm(enc, bwob.buf, bwob.boff, bxb, bxb2, u_qd_l, u_dim, mp, dim)
                    }
                    if (c.attn_out_bias) {
                        var bwo_b = upload_region(unsafe(addr < void? >(t.bo[l * dim])), uint64(dim * 4l))
                        enc_add_bias_rows(enc, bxb2, 0ul, bwo_b, 0ul, u_dim, u_total_dim, npos * dim)
                    }
                }
                // ffn: [post-attn norm ->] [WO residual +] norm -> [requant ->] W1/W3 ->
                // swiglu/geglu[+requant] -> W2 [-> post-ffn norm]
                if (g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                    if (c.pre_post_norm) {
                        // gemma sandwich: norm the attention branch IN PLACE before the residual add
                        var bw_pa = upload_region(addr < void? >(t.fblob[t.rms_post_att_off + l * dim]), uint64(dim * 4l))
                        pf_enc_rms(enc, bxb2, 0ul, bw_pa, bxb2, u_dim, u_eps, npos)
                    }
                    enc_add(enc, bx, 0ul, bxb2, 0ul, u_total_dim, npos * dim)
                    pf_enc_rms(enc, bx, 0ul, bw_ffn, bxb, u_dim, u_eps, npos)
                }
                if (moe && g_pf_skip != "gemm" && g_pf_skip != "moe") {
                    // MoE FFN (Wave C stage 2b): per-position routing + CSR, then one gathered
                    // mul_mm per projection over the touched experts; ordered reduce into bxb
                    // (g4: its own router norm + pre_ffn2 input via bxb2, reduce into bmg4rt)
                    let fe1 = fmt_at(t.we1_fmt, l)
                    let fe3 = fmt_at(t.we3_fmt, l)
                    if (g4moe) {
                        var bw_p2 = upload_region(unsafe(addr < void? >(t.fblob[t.rms_pre_ffn2_off + l * dim])), uint64(dim * 4l))
                        pf_enc_rms(enc, bx, 0ul, bw_p2, bxb2, u_dim, u_eps, npos)
                        var bw_rs = upload_region(unsafe(addr < void? >(t.fblob[t.moe_router_scale_off + l * dim])), uint64(dim * 4l))
                        enc_g4_router_norm(enc, bx, bw_rs, bmg4rt, u_dim, u_eps, u_moe_invd, npos)
                    }
                    var brb : MetalBuffer? = g_one
                    var bhrb = u_moe_g0
                    if (c.moe_router_bias) {
                        brb = upload_region(unsafe(addr < void? >(t.fblob[t.router_b_off + l * c.n_expert])), uint64(c.n_expert * 4l))
                        bhrb = u_moe_gmode
                    }
                    pf_enc_moe_route(enc, t, l, g4moe ? bmg4rt : bxb, bmlg, bmsel, bmcnt, bmbase, bmbkt, bminv,
                        u_dim, u_moe_ne, u_moe_nk, u_moe_gmode, brb, bhrb, u_moe_tot, u_moe_np, npos)
                    if (g4moe) {
                        var bw_ds = upload_region(unsafe(addr < void? >(t.fblob[t.moe_down_scale_off + l * c.n_expert])), uint64(c.n_expert * 4l))
                        enc_moe_wscale(enc, bmsel, bmsel, bw_ds, u_moe_nk, npos)
                    }
                    if (nsh > 0l) {
                        // shexp raw gate dots (σ in the combine) — bmlg is free once select consumed it
                        var bw_shg = upload_region(unsafe(addr < void? >(t.fblob[t.shexp_gate_off + l * dim])), uint64(dim * 4l))
                        pf_enc_slab_gemv_b(enc, bw_shg, bxb, bmlg, u_dim, u_moe_one, u_moe_np, 1l, npos)
                    }
                    if (!pf_skip_moe_mm()) {
                        if (t.experts_mx4) {
                            // per-expert bias folds into the mm accumulator seed (hasb); the
                            // moe_bias chase arm knocks the fold out for attribution
                            let hasb = g_pf_skip != "moe_bias"
                            var bwb1 = upload_region(unsafe(addr < void? >(t.fblob[t.web1_off + l * c.n_expert * c.n_ff_exp])), uint64(c.n_expert * c.n_ff_exp * 4l))
                            var bwb3 = upload_region(unsafe(addr < void? >(t.fblob[t.web3_off + l * c.n_expert * c.n_ff_exp])), uint64(c.n_expert * c.n_ff_exp * 4l))
                            var kaup = moe_mm_mx4_ka(dim, c.n_ff_exp, c.n_expert_used, true, hasb)
                            pf_enc_moe_mm_mx4(enc, t, t.we1_offs[l], c.n_ff_exp, npos, g4moe ? bxb2 : bxb, bmg,
                                bmcnt, bmbase, bmbkt, bwb1, kaup)
                            pf_enc_moe_mm_mx4(enc, t, t.we3_offs[l], c.n_ff_exp, npos, g4moe ? bxb2 : bxb, bmg2,
                                bmcnt, bmbase, bmbkt, bwb3, kaup)
                        } else {
                            pf_enc_moe_mm(enc, t, fe1, t.we1_offs[l], c.n_ff_exp, npos, g4moe ? bxb2 : bxb, bmg,
                                bmcnt, bmbase, bmbkt, moe_mm_ka(fe1, dim, c.n_ff_exp, c.n_expert_used, true))
                            pf_enc_moe_mm(enc, t, fe3, t.we3_offs[l], c.n_ff_exp, npos, g4moe ? bxb2 : bxb, bmg2,
                                bmcnt, bmbase, bmbkt, moe_mm_ka(fe3, dim, c.n_ff_exp, c.n_expert_used, true))
                        }
                        if (nsh > 0l) {
                            // shexp gate/up: dense q8 mul_mm at the wsh planes (bhb/bhb2 sized max(hidden, nsh))
                            let bwsh1 = blob_of(g_dev, t, t.wsh1_off + l * dim * nsh)
                            enc_gemm_mm(enc, bwsh1.buf, bwsh1.boff, bxb, bhb, u_dim, u_moe_nsh, mp, nsh)
                            let bwsh3 = blob_of(g_dev, t, t.wsh3_off + l * dim * nsh)
                            enc_gemm_mm(enc, bwsh3.buf, bwsh3.boff, bxb, bhb2, u_dim, u_moe_nsh, mp, nsh)
                        }
                    }
                }
                if (moe && g_pf_skip != "ew" && g_pf_skip != "nongemm" && g_pf_skip != "moe" && !pf_skip_moe_act()) {
                    if (c.ffn_act == FfnAct.swiglu_oai) {
                        pf_enc_swiglu_oai(enc, bmg, 0ul, bmg2, 0ul, u_moe_ptot, bmcnt, bmbase, u_moe_ne, u_moe_nfe, mpad * c.n_ff_exp)
                    } else {
                        if (g4moe) {
                            enc_geglu(enc, bmg, 0ul, bmg2, 0ul, u_moe_ptot, mpad * c.n_ff_exp)
                        } else {
                            enc_swiglu(enc, bmg, 0ul, bmg2, 0ul, u_moe_ptot, mpad * c.n_ff_exp)
                        }
                    }
                    if (nsh > 0l) {
                        enc_swiglu(enc, bhb, 0ul, bhb2, 0ul, u_moe_shtot, npos * nsh)
                    }
                }
                if (moe && g_pf_skip != "gemm" && g_pf_skip != "moe") {
                    let fe2 = fmt_at(t.we2_fmt, l)
                    if (!pf_skip_moe_mm()) {
                        if (t.experts_mx4) {
                            var bwb2 = upload_region(unsafe(addr < void? >(t.fblob[t.web2_off + l * c.n_expert * dim])), uint64(c.n_expert * dim * 4l))
                            pf_enc_moe_mm_mx4(enc, t, t.we2_offs[l], dim, npos, bmg, bmdn,
                                bmcnt, bmbase, bmbkt, bwb2,
                                moe_mm_mx4_ka(c.n_ff_exp, dim, c.n_expert_used, false, g_pf_skip != "moe_bias"),
                                contiguous = true)
                        } else {
                            pf_enc_moe_mm(enc, t, fe2, t.we2_offs[l], dim, npos, bmg, bmdn,
                                bmcnt, bmbase, bmbkt,
                                moe_mm_ka(fe2, c.n_ff_exp, dim, c.n_expert_used, false), contiguous = true)
                        }
                        if (nsh > 0l) {
                            // shexp down into bxb2 (free post-residual) — bxb still feeds the reduce
                            let bwsh2 = blob_of(g_dev, t, t.wsh2_off + l * dim * nsh)
                            enc_gemm_mm(enc, bwsh2.buf, bwsh2.boff, bhb, bxb2, u_moe_nsh, u_dim, mp, dim)
                        }
                    }
                    if (!pf_skip_moe_reduce()) {
                        pf_enc_moe_reduce(enc, bmdn, bmsel, bminv, g4moe ? bmg4rt : bxb, u_dim, u_moe_nk, dim, npos)
                        if (nsh > 0l) {
                            // routed sum lands in bxb first — the shexp branch adds σ(gate)-scaled on top
                            enc_axpy_sig(enc, bxb, bxb2, bmlg, u_total_dim, u_dim, npos * dim)
                        }
                    }
                    if (g4moe && g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                        var bw_pf2 = upload_region(unsafe(addr < void? >(t.fblob[t.rms_post_ffn2_off + l * dim])), uint64(dim * 4l))
                        pf_enc_rms(enc, bmg4rt, 0ul, bw_pf2, bmg4rt, u_dim, u_eps, npos)
                    }
                }
                if ((!moe || g4moe) && g_pf_skip != "gemm") {   // g4: the dense parallel shared branch
                    let f1 = fmt_at(t.w1_fmt, l)
                    let f3 = fmt_at(t.w3_fmt, l)
                    if (f1 != KqFmt.q8 || f3 != KqFmt.q8) {
                        if (f1 != KqFmt.q8) {
                            pf_enc_kq_site_mm(enc, t, f1, t.w1_offs[l], hidden, bxb, bhb, u_dim, u_hidden, mp)
                        } else {
                            let bw1b = blob_of(g_dev, t, t.w1_offs[l])
                            enc_gemm_mm(enc, bw1b.buf, bw1b.boff, bxb, bhb, u_dim, u_hidden, mp, hidden)
                        }
                        if (f3 != KqFmt.q8) {
                            pf_enc_kq_site_mm(enc, t, f3, t.w3_offs[l], hidden, bxb, bhb2, u_dim, u_hidden, mp)
                        } else {
                            let bw3b = blob_of(g_dev, t, t.w3_offs[l])
                            enc_gemm_mm(enc, bw3b.buf, bw3b.boff, bxb, bhb2, u_dim, u_hidden, mp, hidden)
                        }
                    } else {
                        let bw1b = blob_of(g_dev, t, t.w1_offs[l])
                        let bw3b = blob_of(g_dev, t, t.w3_offs[l])
                        enc_gemm_mm(enc, bw1b.buf, bw1b.boff, bxb, bhb, u_dim, u_hidden, mp, hidden)
                        enc_gemm_mm(enc, bw3b.buf, bw3b.boff, bxb, bhb2, u_dim, u_hidden, mp, hidden)
                    }
                }
                if ((!moe || g4moe) && g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                    if (c.ffn_act != FfnAct.silu) {
                        enc_geglu(enc, bhb, 0ul, bhb2, 0ul, u_total_h, npos * hidden)
                    } else {
                        enc_swiglu(enc, bhb, 0ul, bhb2, 0ul, u_total_h, npos * hidden)
                    }
                }
                if ((!moe || g4moe) && g_pf_skip != "gemm") {
                    let f2 = fmt_at(t.w2_fmt, l)
                    if (f2 != KqFmt.q8) {
                        pf_enc_kq_site_mm(enc, t, f2, t.w2_offs[l], dim, bhb, bxb, u_hidden, u_dim, mp)
                    } else {
                        let bw2b = blob_of(g_dev, t, t.w2_offs[l])
                        enc_gemm_mm(enc, bw2b.buf, bw2b.boff, bhb, bxb, u_hidden, u_dim, mp, dim)
                    }
                }
                if (g4moe && g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                    // dense-branch post-norm, then routed + dense summed into bxb for the epilogue
                    var bw_pf1 = upload_region(unsafe(addr < void? >(t.fblob[t.rms_post_ffn1_off + l * dim])), uint64(dim * 4l))
                    pf_enc_rms(enc, bxb, 0ul, bw_pf1, bxb, u_dim, u_eps, npos)
                    enc_add(enc, bxb, 0ul, bmg4rt, 0ul, u_total_dim, npos * dim)
                }
                if (g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                    if (c.pre_post_norm) {
                        var bw_pf = upload_region(addr < void? >(t.fblob[t.rms_post_ffn_off + l * dim]), uint64(dim * 4l))
                        pf_enc_rms(enc, bxb, 0ul, bw_pf, bxb, u_dim, u_eps, npos)
                    }
                }
                if (plef) {
                    // E-series PLE between FFN residual and out_scale (the CPU order): plain x_b +=
                    // ffn_branch, then branch = post_norm(proj · (gelu(gate · x_b) ⊙ side[l]))
                    if (g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                        enc_add(enc, bx, 0ul, bxb, 0ul, u_total_dim, npos * dim)
                    }
                    if (g_pf_skip != "gemm") {
                        let bwpg = blob_of(g_dev, t, t.ple_gate_off + l * dim * ple_n)
                        enc_gemm_mm(enc, bwpg.buf, bwpg.boff, bx, bpleg, u_dim, u_ple, mp, ple_n)
                    }
                    if (g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                        enc_geglu(enc, bpleg, 0ul, bple, uint64(l * npos * ple_n * 4l), u_ple_tot, npos * ple_n)
                    }
                    if (g_pf_skip != "gemm") {
                        let bwpp = blob_of(g_dev, t, t.ple_proj_off + l * ple_n * dim)
                        enc_gemm_mm(enc, bwpp.buf, bwpp.boff, bpleg, bxb, u_ple, u_dim, mp, dim)
                    }
                    if (g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                        var bw_pn = upload_region(addr < void? >(t.fblob[t.ple_post_off + l * dim]), uint64(dim * 4l))
                        pf_enc_rms(enc, bxb, 0ul, bw_pn, bxb, u_dim, u_eps, npos)
                    }
                }
                if (g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                    // gemma4 layer_out_scale multiplies the WHOLE layer output — (x + branch) * s
                    // folds into the residual add's ascale slot (a 4-byte view into the weights blob)
                    var bosc : MetalBuffer?
                    if (c.layer_out_scale) {
                        bosc = upload_region(addr < void? >(t.fblob[t.out_scale_off + l]), 4ul)
                    }
                    enc_add(enc, bx, 0ul, bxb, 0ul, u_total_dim, npos * dim, bosc)
                }
            }
        }
        if (gpu_logits) {
            // final rmsnorm on the LAST position + classifier GEMV — the caller samples from
            // s.logits, so this closes the whole forward inside the GPU window
            unsafe {
                var bwfin = upload_region(addr < void? >(t.fblob[t.rms_final_off]), uint64(dim * 4l))
                pf_enc_rms(enc, warm ? bwm_res : bx, uint64((npos - 1l) * dim * 4l), bwfin, bxf, u_dim, u_eps, 1l)
                if (cls_fmt != KqFmt.q8) {
                    pf_enc_kq_gemv(enc, t, cls_fmt, t.wcls_off, c.vocab_size, dim, bxf, 0ul, blog, u_dim, u_vocab)
                } else {
                    let bwc = blob_of(g_dev, t, t.wcls_off)
                    enc_gemv(enc, bwc.buf, bwc.boff, bxf, blog, u_dim, u_vocab, c.vocab_size)
                }
                if (u_flcap != null) {
                    // final-logit soft cap (gemma2): cap*tanh over the classifier row in place
                    enc_softcap_row(enc, blog, 0ul, u_vocab, u_flcap, c.vocab_size)
                }
                if (bsupp != null) {
                    // gemma4 suppressed ids pin to -1e30 AFTER the cap (the CPU classifier's order)
                    enc_suppress_row(enc, blog, 0ul, bsupp, u_nsupp, nsupp)
                }
            }
        }
        if (cap) {
            graph_flush_sched(enc)
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
            if (metal_command_buffer_failed(cbi)) {
                ran = false
                err = cb_error_text(cbi)
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
                    memcpy(addr < void? >(s.k_b[0]), reinterpret<void?>(kpan + koff_l), npos * kvd_l * 4l)
                    memcpy(addr < void? >(s.v_b[0]), reinterpret<void?>(vpan + koff_l), npos * kvd_l * 4l)
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
    gpu_ms = (gpu_t1 - gpu_t0) * 1000.0lf   // whole-GPU window: first chunk start -> last chunk end
    let encwait_us = get_time_usec(ts_enc)
    let ok = ran
    if (ran) {
        if (dn) {   // the window completed — the GPU state now holds position start_pos+npos-1's update
            dn_mirror_advance(s.uid, start_pos + npos - 1l)
        }
        let ts_rb = ref_time_ticks()
        unsafe {
            memcpy(addr < void? >(s.x_b[0]), metal_buffer_contents(warm ? bwm_res : bx), npos * dim * 4l)
            if (gpu_logits) {
                memcpy(addr < void? >(s.logits[0]), metal_buffer_contents(blog), c.vocab_size * 4l)
                if (c.n_layer_nextn > 0l) {
                    // forward_prefill's mtp_h contract — bxf holds the last row's post-final-norm hidden
                    memcpy(addr < void? >(s.mtp_h[0]), metal_buffer_contents(bxf), dim * 4l)
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
        to_log(LOG_ERROR, "dasLLAMA metal prefill: dispatch failed ({err}) - falling back to the CPU layer loop\n")   // nolint:PERF028,PERF026 — error path, and the CPU fallback dwarfs the log
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
        pool_release(g_pf_upool, u_dn_np, 4ul)
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
                    pool_release(g_pf_upool, u_ple_all, 4ul)
                    } else {
            pool_release(g_pf_upool, bple, bytes_ple_side)
        }
        pool_release(g_pf_pool, bpleg, bytes_ple_g)
        pool_release(g_pf_upool, u_ple, 4ul)
        pool_release(g_pf_upool, u_ple_tot, 4ul)
    }
    bks |> clear()      // non-owning handles — the pool owns them now
    bvs |> clear()
    pool_release(g_pf_upool, u_dim, 4ul)
    pool_release(g_pf_upool, u_qd, 4ul)
    pool_release(g_pf_upool, u_kvd, 4ul)
    pool_release(g_pf_upool, u_hidden, 4ul)
    pool_release(g_pf_upool, u_hs, 4ul)
    pool_release(g_pf_upool, u_npairs_q, 4ul)
    pool_release(g_pf_upool, u_npairs_k, 4ul)
    pool_release(g_pf_upool, u_neox, 4ul)
    if (has_bias) {
        pool_release(g_pf_upool, u_totq, 4ul)
        pool_release(g_pf_upool, u_totkv, 4ul)
    }
    if (has_qkn) {
    }
    if (hetero) {
        pool_release(g_pf_upool, u_qd_sw, 4ul)
        pool_release(g_pf_upool, u_kvd_sw, 4ul)
        pool_release(g_pf_upool, u_hs_sw, 4ul)
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
        pool_release(g_pf_upool, u_moe_g0, 4ul)
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
    pool_release(g_pf_upool, u_eps, 4ul)
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
        to_log(LOG_INFO, "dasLLAMA metal prefill: npos={npos} gemm=mulmm total={float(total_us) / 1000.0}ms setup={float(setup_us) / 1000.0}ms encode+wait={float(encwait_us) / 1000.0}ms (encode {encode_ms}ms, gpu {gpu_ms}ms) kv+readback={float(readback_us) / 1000.0}ms dispatches={17l * c.n_layers + (gpu_logits ? 2l : 0l)} logits={gpu_logits ? "gpu" : "cpu"}\n")   // nolint:PERF028,PERF026 — one line per prefill call, not per token
    }
    return ok
}

// Knobs resolved once at [init]. They were read per prefill call, and get_env_variable allocates
// its result in the context heap on every call — see PERF027.
var private g_pf_env_mulmm = true
var private g_pf_env_span = true
var private g_pf_env_logits = true
var private g_pf_env_attn = true
var private g_pf_env_unretained = false
var private g_pf_env_capture = true
var private g_pf_env_ncb = 0   // 0 = unset, fall back to the layer-derived default

[init]
def dasllama_metal_prefill_register() {
    // registered but dormant: only select_prefill_override("metal") (env rail
    // DASLLAMA_PIN_PREFILL) activates it, and unsupported shapes decline per call
    register_prefill_override("metal", @@metal_prefill_forward)
    register_ple_gpu_gate(@@metal_ple_pre_gpu_gate)
    g_pf_env_mulmm = g_env_metal.mulmm
    g_pf_env_span = g_env_metal.span
    g_pf_env_logits = g_env_metal.logits
    g_pf_env_attn = g_env_metal.attn
    g_pf_env_unretained = g_env_metal.unretained
    g_pf_env_capture = g_env_metal.pf_capture
    g_pf_env_ncb = g_env_metal.ncb
    if (!empty(g_env_metal.prefill_skip)) {
        g_pf_skip = g_env_metal.prefill_skip
        to_log(LOG_WARNING, "dasLLAMA metal prefill: SKIP={g_pf_skip} - timing-attribution mode, output is GARBAGE\n")
    }
}

// ===== Metal-4 tensor twin race (the tuner's crowning section) =====

def private race_mulmm_bf16(dev : MetalDevice?; queue : MetalCommandQueue?) : MetalTensorRaceResult {
    var res = MetalTensorRaceResult(family = "mulmm_bf16", winner = "", base_ms = -1.0lf, twin_ms = -1.0lf, note = "")
    let m = 512
    let kdim = 2048
    let ndim = 1024
    var err = ""
    var base_pso = pipeline_from_source(dev, metal_bf16_mulmm_msl, metal_bf16_mulmm_msl_entry, metal_bf16_mulmm_msl_fastmath, err)
    if (base_pso == null) {
        res.note = "base pso: {err}"
        return res
    }
    var twin_pso = pipeline_from_source(dev, metal_bf16_mulmm_t_msl, metal_bf16_mulmm_t_msl_entry, metal_bf16_mulmm_t_msl_fastmath, err)
    if (twin_pso == null) {
        // no tensor support on this box/toolchain — the simdgroup kernel keeps the crown
        res.winner = "simdgroup"
        res.note = "twin pso: {err}"
        metal_release(base_pso)
        return res
    }
    var xa <- race_x_f32(m * kdim)
    var wb : array<uint16>
    wb |> resize(ndim * kdim)
    for (i in range(ndim * kdim)) {
        wb[i] = uint16(float_bits_to_uint(0.125 * float((i * 3) % 13 - 6)) >> 16u)
    }
    var bxa = race_buf(dev, uint64(m * kdim * 4), unsafe(addr<void?>(xa[0])))
    var bwb = race_buf(dev, uint64(ndim * kdim * 2), unsafe(addr<void?>(wb[0])))
    var by_base = race_buf(dev, uint64(m * ndim * 4), null)
    var by_twin = race_buf(dev, uint64(m * ndim * 4), null)
    var bk = race_uniform_u32(dev, uint(kdim))
    var bn = race_uniform_u32(dev, uint(ndim))
    let grid = uint3(uint(m / 32), uint(ndim / 64), 1u)
    let tg = uint3(128u, 1u, 1u)
    race_pair_ms(queue, 5, res.base_ms, res.twin_ms, $(enc) {
        kn_pipeline(enc, base_pso)
        kn_tgmem(enc, metal_bf16_mulmm_msl_tgmem, 0)
        kn_buffer(enc, bwb, 0ul, 0)
        kn_buffer(enc, bxa, 0ul, 2)
        kn_buffer(enc, by_base, 0ul, 3)
        kn_buffer(enc, bk, 0ul, 4)
        kn_buffer(enc, bn, 0ul, 5)
        kn_dispatch(enc, grid, tg)
    }, $(enc) {
        kn_pipeline(enc, twin_pso)
        kn_buffer(enc, bwb, 0ul, 0)
        kn_buffer(enc, bxa, 0ul, 2)
        kn_buffer(enc, by_twin, 0ul, 3)
        kn_buffer(enc, bk, 0ul, 4)
        kn_buffer(enc, bn, 0ul, 5)
        kn_dispatch(enc, grid, tg)
    })
    if (res.base_ms < 0.0lf || res.twin_ms < 0.0lf) {
        res.winner = res.base_ms < 0.0lf ? "" : "simdgroup"
        res.note = "dispatch failed"
    } elif (!race_envelope_ok(by_base, by_twin, m * ndim, res.note)) {
        res.winner = "simdgroup"
    } else {
        res.winner = res.twin_ms < res.base_ms ? "tensor" : "simdgroup"
    }
    metal_release(bxa)
    metal_release(bwb)
    metal_release(by_base)
    metal_release(by_twin)
    metal_release(bk)
    metal_release(bn)
    metal_release(base_pso)
    metal_release(twin_pso)
    return res
}

def private race_mulmm_q8(dev : MetalDevice?; queue : MetalCommandQueue?) : MetalTensorRaceResult {
    var res = MetalTensorRaceResult(family = "mulmm_q8", winner = "", base_ms = -1.0lf, twin_ms = -1.0lf, note = "")
    let m = 512
    let kdim = 2048
    let ndim = 1024
    let nkb = kdim / 32
    var err = ""
    var base_pso = pipeline_from_source(dev, metal_q8_mulmm_msl, metal_q8_mulmm_msl_entry, metal_q8_mulmm_msl_fastmath, err)
    if (base_pso == null) {
        res.note = "base pso: {err}"
        return res
    }
    var twin_pso = pipeline_from_source(dev, metal_q8_mulmm_t_msl, metal_q8_mulmm_t_msl_entry, metal_q8_mulmm_t_msl_fastmath, err)
    if (twin_pso == null) {
        res.winner = "simdgroup"
        res.note = "twin pso: {err}"
        metal_release(base_pso)
        return res
    }
    var xa <- race_x_f32(m * kdim)
    var blob <- race_q8_blob(ndim * nkb)
    var bw = race_buf(dev, uint64(ndim * nkb * 34), unsafe(addr<void?>(blob[0])))
    var bxa = race_buf(dev, uint64(m * kdim * 4), unsafe(addr<void?>(xa[0])))
    var by_base = race_buf(dev, uint64(m * ndim * 4), null)
    var by_twin = race_buf(dev, uint64(m * ndim * 4), null)
    var bk = race_uniform_u32(dev, uint(kdim))
    var bn = race_uniform_u32(dev, uint(ndim))
    let grid = uint3(uint(m / 32), uint(ndim / 64), 1u)
    let tg = uint3(128u, 1u, 1u)
    race_pair_ms(queue, 5, res.base_ms, res.twin_ms, $(enc) {
        kn_pipeline(enc, base_pso)
        kn_tgmem(enc, metal_q8_mulmm_msl_tgmem, 0)
        kn_buffer(enc, bw, 0ul, 0)
        kn_buffer(enc, bw, 0ul, 1)
        kn_buffer(enc, bxa, 0ul, 2)
        kn_buffer(enc, by_base, 0ul, 3)
        kn_buffer(enc, bk, 0ul, 4)
        kn_buffer(enc, bn, 0ul, 5)
        kn_dispatch(enc, grid, tg)
    }, $(enc) {
        kn_pipeline(enc, twin_pso)
        kn_buffer(enc, bw, 0ul, 0)
        kn_buffer(enc, bw, 0ul, 1)
        kn_buffer(enc, bxa, 0ul, 2)
        kn_buffer(enc, by_twin, 0ul, 3)
        kn_buffer(enc, bk, 0ul, 4)
        kn_buffer(enc, bn, 0ul, 5)
        kn_dispatch(enc, grid, tg)
    })
    if (res.base_ms < 0.0lf || res.twin_ms < 0.0lf) {
        res.winner = res.base_ms < 0.0lf ? "" : "simdgroup"
        res.note = "dispatch failed"
    } elif (!race_envelope_ok(by_base, by_twin, m * ndim, res.note)) {
        res.winner = "simdgroup"
    } else {
        res.winner = res.twin_ms < res.base_ms ? "tensor" : "simdgroup"
    }
    metal_release(bw)
    metal_release(bxa)
    metal_release(by_base)
    metal_release(by_twin)
    metal_release(bk)
    metal_release(bn)
    metal_release(base_pso)
    metal_release(twin_pso)
    return res
}

def private race_moe_mulmm_q8(dev : MetalDevice?; queue : MetalCommandQueue?) : MetalTensorRaceResult {   // nolint:STYLE038 — one-shot A/B race harness; buffers + PSOs stay live to the release tail
    var res = MetalTensorRaceResult(family = "moe_mulmm_q8", winner = "", base_ms = -1.0lf, twin_ms = -1.0lf, note = "")
    let ne = 4
    let rows_per = 32
    let m = ne * rows_per
    let kdim = 2048
    let ndim = 1024
    let nkb = kdim / 32
    let eblk = ndim * nkb
    var err = ""
    var base_pso = pipeline_from_source(dev, metal_moe_mulmm_q8_msl, metal_moe_mulmm_q8_msl_entry, metal_moe_mulmm_q8_msl_fastmath, err)
    if (base_pso == null) {
        res.note = "base pso: {err}"
        return res
    }
    var twin_pso = pipeline_from_source(dev, metal_moe_mulmm_q8_t_msl, metal_moe_mulmm_q8_t_msl_entry, metal_moe_mulmm_q8_t_msl_fastmath, err)
    if (twin_pso == null) {
        res.winner = "simdgroup"
        res.note = "twin pso: {err}"
        metal_release(base_pso)
        return res
    }
    var xa <- race_x_f32(m * kdim)
    var blob <- race_q8_blob(ne * eblk)
    var cnt : array<uint>
    var basep : array<uint>
    var bkt : array<uint>
    cnt |> resize(ne)
    basep |> resize(ne)
    bkt |> resize(m)
    for (e in range(ne)) {
        cnt[e] = uint(rows_per)
        basep[e] = uint(e * rows_per)
    }
    for (i in range(m)) {
        bkt[i] = uint(i)   // identity buckets: gather=0 semantics match
    }
    var bw = race_buf(dev, uint64(ne * eblk * 34), unsafe(addr<void?>(blob[0])))
    var bxa = race_buf(dev, uint64(m * kdim * 4), unsafe(addr<void?>(xa[0])))
    var by_base = race_buf(dev, uint64(m * ndim * 4), null)
    var by_twin = race_buf(dev, uint64(m * ndim * 4), null)
    var bcnt = race_buf(dev, uint64(ne * 4), unsafe(addr<void?>(cnt[0])))
    var bbase = race_buf(dev, uint64(ne * 4), unsafe(addr<void?>(basep[0])))
    var bbkt = race_buf(dev, uint64(m * 4), unsafe(addr<void?>(bkt[0])))
    // bind through the family's builder — a hand-rolled list here desyncs unseen by any suite
    var rka = moe_mm_ka(KqFmt.q8, int64(kdim), int64(ndim), 1l, false)
    let grid = uint3(uint(rows_per / 32), uint(ndim / 64), uint(ne))
    let tg = uint3(128u, 1u, 1u)
    race_pair_ms(queue, 5, res.base_ms, res.twin_ms, $(enc) {
        kn_pipeline(enc, base_pso)
        kn_tgmem(enc, metal_moe_mulmm_q8_msl_tgmem, 0)
        kn_buffer(enc, bw, 0ul, 0)
        kn_buffer(enc, bw, 0ul, 1)
        kn_kargs(enc, rka, 5)
        kn_moe_mm_family_tail(enc, bxa, by_base, bcnt, bbase, bbkt)
        kn_dispatch(enc, grid, tg)
    }, $(enc) {
        kn_pipeline(enc, twin_pso)
        kn_buffer(enc, bw, 0ul, 0)
        kn_buffer(enc, bw, 0ul, 1)
        kn_buffer(enc, bxa, 0ul, 2)
        kn_buffer(enc, by_twin, 0ul, 3)
        kn_kargs(enc, rka, 5)
        kn_buffer(enc, bcnt, 0ul, 6)
        kn_buffer(enc, bbase, 0ul, 7)
        kn_dispatch(enc, grid, tg)
    })
    if (res.base_ms < 0.0lf || res.twin_ms < 0.0lf) {
        res.winner = res.base_ms < 0.0lf ? "" : "simdgroup"
        res.note = "dispatch failed"
    } elif (!race_envelope_ok(by_base, by_twin, m * ndim, res.note)) {
        res.winner = "simdgroup"
    } else {
        res.winner = res.twin_ms < res.base_ms ? "tensor" : "simdgroup"
    }
    metal_release(bw)
    metal_release(bxa)
    metal_release(by_base)
    metal_release(by_twin)
    metal_release(bcnt)
    metal_release(bbase)
    metal_release(bbkt)
    metal_release(base_pso)
    metal_release(twin_pso)
    return res
}

def private race_moe_mulmm_mx4(dev : MetalDevice?; queue : MetalCommandQueue?) : MetalTensorRaceResult {   // nolint:STYLE038 — one-shot A/B race harness; buffers + PSOs stay live to the release tail
    var res = MetalTensorRaceResult(family = "moe_mulmm_mx4", winner = "", base_ms = -1.0lf, twin_ms = -1.0lf, note = "")
    let ne = 4
    let rows_per = 32
    let m = ne * rows_per
    let kdim = 2048
    let ndim = 1024
    let nkb = kdim / 32
    let eblk = ndim * nkb
    var err = ""
    var base_pso = pipeline_from_source(dev, metal_moe_mulmm_mx4_msl, metal_moe_mulmm_mx4_msl_entry, metal_moe_mulmm_mx4_msl_fastmath, err)
    if (base_pso == null) {
        res.note = "base pso: {err}"
        return res
    }
    var twin_pso = pipeline_from_source(dev, metal_moe_mulmm_mx4_t_msl, metal_moe_mulmm_mx4_t_msl_entry, metal_moe_mulmm_mx4_t_msl_fastmath, err)
    if (twin_pso == null) {
        res.winner = "simdgroup"
        res.note = "twin pso: {err}"
        metal_release(base_pso)
        return res
    }
    var xa <- race_x_f32(m * kdim)
    var nplane : array<uint>
    nplane |> resize(ne * eblk * 4)   // 16B nibbles per block
    for (i in range(ne * eblk * 4)) {
        nplane[i] = uint(i) * 2654435761u
    }
    var eplane : array<uint8>
    eplane |> resize(ne * eblk)
    for (i in range(ne * eblk)) {
        eplane[i] = uint8(125 + i % 4)   // e8m0 near 1.0 — no overflow across the fold
    }
    var vtabh : array<float>
    vtabh |> resize(16)
    for (i in range(8)) {
        let v = float(fixed_array(0, 1, 2, 3, 4, 6, 8, 12)[i])
        vtabh[i] = v
        vtabh[i + 8] = -v
    }
    var cnt : array<uint>
    var basep : array<uint>
    var bkt : array<uint>
    cnt |> resize(ne)
    basep |> resize(ne)
    bkt |> resize(m)
    for (e in range(ne)) {
        cnt[e] = uint(rows_per)
        basep[e] = uint(e * rows_per)
    }
    for (i in range(m)) {
        bkt[i] = uint(i)
    }
    var bnq = race_buf(dev, uint64(ne * eblk * 16), unsafe(addr<void?>(nplane[0])))
    var bee = race_buf(dev, uint64(ne * eblk), unsafe(addr<void?>(eplane[0])))
    var bvt = race_buf(dev, uint64(16 * 4), unsafe(addr<void?>(vtabh[0])))
    var bwbb = race_buf(dev, uint64(ne * ndim * 4), null)
    var bxa = race_buf(dev, uint64(m * kdim * 4), unsafe(addr<void?>(xa[0])))
    var by_base = race_buf(dev, uint64(m * ndim * 4), null)
    var by_twin = race_buf(dev, uint64(m * ndim * 4), null)
    var bcnt = race_buf(dev, uint64(ne * 4), unsafe(addr<void?>(cnt[0])))
    var bbase = race_buf(dev, uint64(ne * 4), unsafe(addr<void?>(basep[0])))
    var bbkt = race_buf(dev, uint64(m * 4), unsafe(addr<void?>(bkt[0])))
    // same rule as the q8 race: bind through the family's builder, never a hand-rolled list
    var rka = moe_mm_mx4_ka(int64(kdim), int64(ndim), 1l, false, false)
    let grid = uint3(uint(rows_per / 32), uint(ndim / 64), uint(ne))
    let tg = uint3(128u, 1u, 1u)
    race_pair_ms(queue, 5, res.base_ms, res.twin_ms, $(enc) {
        kn_pipeline(enc, base_pso)
        kn_tgmem(enc, metal_moe_mulmm_mx4_msl_tgmem, 0)
        kn_buffer(enc, bnq, 0ul, 0)
        kn_buffer(enc, bee, 0ul, 1)
        kn_kargs(enc, rka, 5)
        kn_moe_mm_family_tail(enc, bxa, by_base, bcnt, bbase, bbkt)
        kn_buffer(enc, bwbb, 0ul, 12)
        kn_buffer(enc, bvt, 0ul, 13)
        kn_dispatch(enc, grid, tg)
    }, $(enc) {
        kn_pipeline(enc, twin_pso)
        kn_tgmem(enc, metal_moe_mulmm_mx4_t_msl_tgmem, 0)
        kn_buffer(enc, bnq, 0ul, 0)
        kn_buffer(enc, bee, 0ul, 1)
        kn_buffer(enc, bxa, 0ul, 2)
        kn_buffer(enc, by_twin, 0ul, 3)
        kn_kargs(enc, rka, 5)
        kn_buffer(enc, bcnt, 0ul, 6)
        kn_buffer(enc, bbase, 0ul, 7)
        kn_buffer(enc, bwbb, 0ul, 12)
        kn_buffer(enc, bvt, 0ul, 13)
        kn_dispatch(enc, grid, tg)
    })
    if (res.base_ms < 0.0lf || res.twin_ms < 0.0lf) {
        res.winner = res.base_ms < 0.0lf ? "" : "simdgroup"
        res.note = "dispatch failed"
    } elif (!race_envelope_ok(by_base, by_twin, m * ndim, res.note)) {
        res.winner = "simdgroup"
    } else {
        res.winner = res.twin_ms < res.base_ms ? "tensor" : "simdgroup"
    }
    metal_release(bnq)
    metal_release(bee)
    metal_release(bvt)
    metal_release(bwbb)
    metal_release(bxa)
    metal_release(by_base)
    metal_release(by_twin)
    metal_release(bcnt)
    metal_release(bbase)
    metal_release(bbkt)
    metal_release(base_pso)
    metal_release(twin_pso)
    return res
}

// one kq mulmm race: random quant planes + constant f16 scales (any bit pattern is a valid
// plane — the twin-vs-base envelope doubles as a layout-agreement check). k6 binds the d plane
// at its tail offset; k4/k5 bind the compact scale blocks at 0.
def private race_kq_mulmm(dev : MetalDevice?; queue : MetalCommandQueue?; family : string;   // nolint:STYLE038 — one-shot A/B race harness; buffers + PSOs stay live to the release tail
                          base_src, base_entry : string; base_fm : bool; base_tgmem : uint64;
                          twin_src, twin_entry : string; twin_fm : bool; twin_tgmem : uint64;
                          qu_per_sb : int; k6 : bool) : MetalTensorRaceResult {
    var res = MetalTensorRaceResult(family = family, winner = "", base_ms = -1.0lf, twin_ms = -1.0lf, note = "")
    let m = 512
    let kdim = 2048
    let ndim = 1024
    let nsb = kdim / 256
    var err = ""
    var base_pso = pipeline_from_source(dev, base_src, base_entry, base_fm, err)
    if (base_pso == null) {
        res.note = "base pso: {err}"
        return res
    }
    var twin_pso = pipeline_from_source(dev, twin_src, twin_entry, twin_fm, err)
    if (twin_pso == null) {
        res.winner = "simdgroup"
        res.note = "twin pso: {err}"
        metal_release(base_pso)
        return res
    }
    var xa <- race_x_f32(m * kdim)
    let nblk = ndim * nsb
    var qplane : array<uint>
    qplane |> resize(nblk * qu_per_sb)
    for (i in range(nblk * qu_per_sb)) {
        qplane[i] = uint(i) * 2654435761u   // deterministic pseudo-random bit pattern
    }
    // scales: k4/k5 = compact 16B blocks; k6 = int8 sub-scale plane + the f16 d tail
    var splane : array<uint8>
    let sbytes = k6 ? nblk * 16 + nblk * 2 : nblk * 16
    splane |> resize(sbytes)
    if (k6) {
        for (i in range(nblk * 16)) {
            splane[i] = uint8(2)
        }
        unsafe {
            var ph = addr<float16?>(splane[nblk * 16])
            for (b in range(nblk)) {
                ph[b] = float16(0.25)
            }
        }
    } else {
        unsafe {
            var ph = addr<float16?>(splane[0])
            for (b in range(nblk)) {
                ph[b * 8] = float16(0.25)       // d
                ph[b * 8 + 1] = float16(0.25)   // dmin
            }
        }
        for (b in range(nblk)) {
            for (kx in range(4, 12)) {
                splane[b * 16 + kx] = uint8(17)   // kmask bytes: small scales, both halves
            }
        }
    }
    var bq = race_buf(dev, uint64(nblk * qu_per_sb * 4), unsafe(addr<void?>(qplane[0])))
    var bs = race_buf(dev, uint64(sbytes), unsafe(addr<void?>(splane[0])))
    var bxa = race_buf(dev, uint64(m * kdim * 4), unsafe(addr<void?>(xa[0])))
    var by_base = race_buf(dev, uint64(m * ndim * 4), null)
    var by_twin = race_buf(dev, uint64(m * ndim * 4), null)
    var bk = race_uniform_u32(dev, uint(kdim))
    var bn = race_uniform_u32(dev, uint(ndim))
    let doff = k6 ? uint64(nblk * 16) : 0ul
    let grid = uint3(uint(m / 32), uint(ndim / 64), 1u)
    let tg = uint3(128u, 1u, 1u)
    race_pair_ms(queue, 5, res.base_ms, res.twin_ms, $(enc) {
        kn_pipeline(enc, base_pso)
        kn_tgmem(enc, base_tgmem, 0)
        kn_buffer(enc, bs, doff, 0)
        kn_buffer(enc, bs, 0ul, 1)
        kn_buffer(enc, bq, 0ul, 2)
        kn_buffer(enc, bxa, 0ul, 3)
        kn_buffer(enc, by_base, 0ul, 4)
        kn_buffer(enc, bk, 0ul, 5)
        kn_buffer(enc, bn, 0ul, 6)
        kn_dispatch(enc, grid, tg)
    }, $(enc) {
        kn_pipeline(enc, twin_pso)
        kn_tgmem(enc, twin_tgmem, 0)
        kn_buffer(enc, bs, doff, 0)
        kn_buffer(enc, bs, 0ul, 1)
        kn_buffer(enc, bq, 0ul, 2)
        kn_buffer(enc, bxa, 0ul, 3)
        kn_buffer(enc, by_twin, 0ul, 4)
        kn_buffer(enc, bk, 0ul, 5)
        kn_buffer(enc, bn, 0ul, 6)
        kn_dispatch(enc, grid, tg)
    })
    if (res.base_ms < 0.0lf || res.twin_ms < 0.0lf) {
        res.winner = res.base_ms < 0.0lf ? "" : "simdgroup"
        res.note = "dispatch failed"
    } elif (!race_envelope_ok(by_base, by_twin, m * ndim, res.note)) {
        res.winner = "simdgroup"
    } else {
        res.winner = res.twin_ms < res.base_ms ? "tensor" : "simdgroup"
    }
    metal_release(bq)
    metal_release(bs)
    metal_release(bxa)
    metal_release(by_base)
    metal_release(by_twin)
    metal_release(bk)
    metal_release(bn)
    metal_release(base_pso)
    metal_release(twin_pso)
    return res
}

// attention pair races: synthetic Q/K/V; BOTH att/xb outputs pre-zeroed (causal-skipped tiles
// stay unwritten in both kernels — fresh-buffer garbage would flake the envelope compare).
def private race_attn_pair(dev : MetalDevice?; queue : MetalCommandQueue?) : array<MetalTensorRaceResult> {   // nolint:STYLE038 — one-shot A/B race harness; buffers + PSOs stay live to the release tail
    var out : array<MetalTensorRaceResult>
    let heads = 8
    let hs = 128
    let qd = heads * hs
    let kvd = qd
    let mp = 128
    let np32 = 128
    var err = ""
    var q_pso = pipeline_from_source(dev, metal_attn_qk_mm_msl, metal_attn_qk_mm_msl_entry, metal_attn_qk_mm_msl_fastmath, err)
    var qt_pso = pipeline_from_source(dev, metal_attn_qk_mm_t_msl, metal_attn_qk_mm_t_msl_entry, metal_attn_qk_mm_t_msl_fastmath, err)
    var a_pso = pipeline_from_source(dev, metal_attn_av_mm_msl, metal_attn_av_mm_msl_entry, metal_attn_av_mm_msl_fastmath, err)
    var at_pso = pipeline_from_source(dev, metal_attn_av_mm_t_msl, metal_attn_av_mm_t_msl_entry, metal_attn_av_mm_t_msl_fastmath, err)
    if (q_pso == null || qt_pso == null || a_pso == null || at_pso == null) {
        out |> emplace(MetalTensorRaceResult(family = "attn_qkmm", winner = q_pso != null ? "simdgroup" : "", base_ms = -1.0lf, twin_ms = -1.0lf, note = "pso: {err}"))
        out |> emplace(MetalTensorRaceResult(family = "attn_avmm", winner = a_pso != null ? "simdgroup" : "", base_ms = -1.0lf, twin_ms = -1.0lf, note = "pso: {err}"))
    } else {
        var qv <- race_x_f32(mp * qd)
        var kv <- race_x_f32(np32 * kvd)
        var pv <- race_x_f32(mp * np32 * heads)
        var zeros : array<float>
        zeros |> resize(heads * mp * np32)
        var bq = race_buf(dev, uint64(mp * qd * 4), unsafe(addr<void?>(qv[0])))
        var bkk = race_buf(dev, uint64(np32 * kvd * 4), unsafe(addr<void?>(kv[0])))
        var bp = race_buf(dev, uint64(heads * mp * np32 * 4), unsafe(addr<void?>(pv[0])))
        var batt_b = race_buf(dev, uint64(heads * mp * np32 * 4), unsafe(addr<void?>(zeros[0])))
        var batt_t = race_buf(dev, uint64(heads * mp * np32 * 4), unsafe(addr<void?>(zeros[0])))
        var bxb_b = race_buf(dev, uint64(mp * qd * 4), unsafe(addr<void?>(zeros[0])))
        var bxb_t = race_buf(dev, uint64(mp * qd * 4), unsafe(addr<void?>(zeros[0])))
        // the race binds the SAME kargs the encoders do — a hand-rolled list silently desyncs
        var rka = AttnArgs(qd = uint(qd), kv_dim = uint(kvd), head_size = uint(hs), kv_mul = 1u,
            npos = uint(np32), np32 = uint(np32), scale = 0.125, qoff = 0u, qrows = uint(mp),
            window = 0u, softcap = 0.0, hass = 0u)
        let qgrid = uint3(uint(mp / 32), uint(np32 / 64), uint(heads))
        let agrid = uint3(uint(mp / 32), uint(hs / 64), uint(heads))
        let tg = uint3(128u, 1u, 1u)
        var res = MetalTensorRaceResult(family = "attn_qkmm", winner = "", base_ms = -1.0lf, twin_ms = -1.0lf, note = "")
        race_pair_ms(queue, 5, res.base_ms, res.twin_ms, $(enc) {
            kn_pipeline(enc, q_pso)
            kn_tgmem(enc, metal_attn_qk_mm_msl_tgmem, 0)
            kn_buffer(enc, bq, 0ul, 0)
            kn_buffer(enc, bkk, 0ul, 1)
            kn_buffer(enc, batt_b, 0ul, 2)
            kn_kargs(enc, rka, 3)
            kn_dispatch(enc, qgrid, tg)
        }, $(enc) {
            kn_pipeline(enc, qt_pso)
            kn_tgmem(enc, metal_attn_qk_mm_t_msl_tgmem, 0)
            kn_buffer(enc, bq, 0ul, 0)
            kn_buffer(enc, bkk, 0ul, 1)
            kn_buffer(enc, batt_t, 0ul, 2)
            kn_kargs(enc, rka, 3)
            kn_dispatch(enc, qgrid, tg)
        })
        if (res.base_ms < 0.0lf || res.twin_ms < 0.0lf) {
            res.winner = res.base_ms < 0.0lf ? "" : "simdgroup"
            res.note = "dispatch failed"
        } elif (!race_envelope_ok(batt_b, batt_t, heads * mp * np32, res.note)) {
            res.winner = "simdgroup"
        } else {
            res.winner = res.twin_ms < res.base_ms ? "tensor" : "simdgroup"
        }
        out |> emplace(res)
        var res2 = MetalTensorRaceResult(family = "attn_avmm", winner = "", base_ms = -1.0lf, twin_ms = -1.0lf, note = "")
        race_pair_ms(queue, 5, res2.base_ms, res2.twin_ms, $(enc) {
            kn_pipeline(enc, a_pso)
            kn_tgmem(enc, metal_attn_av_mm_msl_tgmem, 0)
            kn_buffer(enc, bp, 0ul, 0)
            kn_buffer(enc, bkk, 0ul, 1)
            kn_buffer(enc, bxb_b, 0ul, 2)
            kn_kargs(enc, rka, 3)
            kn_dispatch(enc, agrid, tg)
        }, $(enc) {
            kn_pipeline(enc, at_pso)
            kn_tgmem(enc, metal_attn_av_mm_t_msl_tgmem, 0)
            kn_buffer(enc, bp, 0ul, 0)
            kn_buffer(enc, bkk, 0ul, 1)
            kn_buffer(enc, bxb_t, 0ul, 2)
            kn_kargs(enc, rka, 3)
            kn_dispatch(enc, agrid, tg)
        })
        if (res2.base_ms < 0.0lf || res2.twin_ms < 0.0lf) {
            res2.winner = res2.base_ms < 0.0lf ? "" : "simdgroup"
            res2.note = "dispatch failed"
        } elif (!race_envelope_ok(bxb_b, bxb_t, mp * qd, res2.note)) {
            res2.winner = "simdgroup"
        } else {
            res2.winner = res2.twin_ms < res2.base_ms ? "tensor" : "simdgroup"
        }
        out |> emplace(res2)
        metal_release(bq)
        metal_release(bkk)
        metal_release(bp)
        metal_release(batt_b)
        metal_release(batt_t)
        metal_release(bxb_b)
        metal_release(bxb_t)
    }
    if (q_pso != null) {
        metal_release(q_pso)
    }
    if (qt_pso != null) {
        metal_release(qt_pso)
    }
    if (a_pso != null) {
        metal_release(a_pso)
    }
    if (at_pso != null) {
        metal_release(at_pso)
    }
    return <- out
}

//! Tuner entry (the runtime.metal_tensor crown): race every tensor pso twin against its
//! shipped simdgroup kernel on THIS box — compile + output-match to qualify, faster GPU time
//! takes the family's crown. Empty when there is no Metal device.
def metal_tensor_race : array<MetalTensorRaceResult> {
    var results : array<MetalTensorRaceResult>
    var dev = metal_create_system_default_device()
    if (dev == null) {
        return <- results
    }
    var queue = metal_new_command_queue(dev)
    results |> emplace(race_mulmm_bf16(dev, queue))
    results |> emplace(race_mulmm_q8(dev, queue))
    results |> emplace(race_moe_mulmm_q8(dev, queue))
    results |> emplace(race_moe_mulmm_mx4(dev, queue))
    for (r in race_attn_pair(dev, queue)) {
        results |> emplace(r)
    }
    results |> emplace(race_kq_mulmm(dev, queue, "kq_mulmm_k4",
        MetalKqMulMmK4_metal_kq_mulmm_msl, MetalKqMulMmK4_metal_kq_mulmm_msl_entry, MetalKqMulMmK4_metal_kq_mulmm_msl_fastmath, MetalKqMulMmK4_metal_kq_mulmm_msl_tgmem,
        MetalKqMulMmK4T_metal_kq_mulmm_t_msl, MetalKqMulMmK4T_metal_kq_mulmm_t_msl_entry, MetalKqMulMmK4T_metal_kq_mulmm_t_msl_fastmath, MetalKqMulMmK4T_metal_kq_mulmm_t_msl_tgmem, 32, false))
    results |> emplace(race_kq_mulmm(dev, queue, "kq_mulmm_k5",
        MetalKqMulMmK5_metal_kq_mulmm_msl, MetalKqMulMmK5_metal_kq_mulmm_msl_entry, MetalKqMulMmK5_metal_kq_mulmm_msl_fastmath, MetalKqMulMmK5_metal_kq_mulmm_msl_tgmem,
        MetalKqMulMmK5T_metal_kq_mulmm_t_msl, MetalKqMulMmK5T_metal_kq_mulmm_t_msl_entry, MetalKqMulMmK5T_metal_kq_mulmm_t_msl_fastmath, MetalKqMulMmK5T_metal_kq_mulmm_t_msl_tgmem, 40, false))
    results |> emplace(race_kq_mulmm(dev, queue, "kq_mulmm_k6",
        MetalKqMulMmK6_metal_kq_mulmm_msl, MetalKqMulMmK6_metal_kq_mulmm_msl_entry, MetalKqMulMmK6_metal_kq_mulmm_msl_fastmath, MetalKqMulMmK6_metal_kq_mulmm_msl_tgmem,
        metal_kq_mulmm_k6_t_msl, metal_kq_mulmm_k6_t_msl_entry, metal_kq_mulmm_k6_t_msl_fastmath, metal_kq_mulmm_k6_t_msl_tgmem, 48, true))
    metal_release(queue)
    metal_release(dev)
    return <- results
}
