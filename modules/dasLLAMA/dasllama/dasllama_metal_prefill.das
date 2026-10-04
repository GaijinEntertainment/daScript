options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_metal_prefill shared public

require dasllama/dasllama_lint public
require math
require daslib/math_bits
require dasllama/dasllama_env
require metal/msl_shader
require metal/das_metal_boost
require daslib/typemacro_boost
require dasllama/dasllama_common
require dasllama/dasllama_math
require dasllama/dasllama_metal_common
require dasllama/dasllama_gpu_math
require dasllama/dasllama_metal_kernels  // the shared kq decode helpers (grid tables, ksign7) for the tensor twins
require dasllama/dasllama_metal_lens
require dasllama/dasllama_tower
require dasllama/dasllama_load   // register_devw_kq_mirror: the image bake's CPU twin of the split-scale dequant passes

// Phase-6 dasMetal: the full-GPU-resident prefill kernel set — everything between the GEMMs:
// rmsnorm, table-driven RoPE, fused causal-softmax attention scores, the P·V gather, fused
// SwiGLU, and the residual add (the drivers are BLOB-ONLY mul_mm since the .dlim redesign —
// the quantized-X rail and its quant/fusion kernels are gone). Each kernel mirrors its
// dasLLAMA CPU twin's math exactly (same formula, same rounding ops):
//   metal_rmsnorm       ~ rmsnorm                  (w[i] * x[i] * rsqrt(mean(x^2) + eps))
//   metal_rope          ~ rope_scaled_tab          (NORM adjacent pairs; cos/sin table rows)
//   metal_attn_qk/_rowstat/_av ~ the classic per-head causal scores + softmax + P·V, as a
//                         tiled sgmat GEMM trio over np32-padded per-head f16 score slabs
//                         (rowstat mints rowmax + 1/sum; AV applies exp at P staging)
//   metal_swiglu        ~ swiglu                   (g = g/(1+exp(-g)) * u)
//   metal_add           ~ add_inplace
// Reductions ride simd_sum / simd_max and the kernel home's threadgroup folds (tg_sum_all / tg_max_all) —
// Apple-GPU simdgroups are width 32 (asserted at driver init). Kernels with barriers/simd ops are
// NOT sequentially CPU-replayable; their oracles are the dasLLAMA CPU functions themselves
// (modules/dasLLAMA/tests/test_metal_prefill_kernels.das).


[metal_dispatch(name = "pf_enc_bf16_mm_c", pso = "g_pf_pso_bf16_mm", tgmem = "metal_bf16_mulmm_msl_tgmem", tg = 128, grid = "mp/32, nd/64", params = "mp : int64, nd : int64")]
class MetalBf16MulMm : MetalMmTileBase {
    typedef TILET = float16
    typedef SGT = simdgroup_half8x8
    @ssbo @binding = 0 @role = "weight" @off = "wboff" wbh : array<ushort4> //!< bf16 W rows, ushort4 view (row r elem k at (r*kdim+k)/4)
    @ssbo @binding = 2 @role = "read" xf : array<float4>   //!< raw f32 activations, float4 view
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

[ |> template_struct_instance]
class template MetalQ8MulMmTensorT {
    @template_constant MT : uint = 32u      //!< M-tile rows: W re-streams M/MT times
    @template_constant BK : uint = 64u      //!< K chunk the stage streams per step; 128 double-buffers two tiles
    @template_constant TWB : int = 6144     //!< W stage halves: one 64 wcols x 72-half padded tile (12288B lands mid-plateau); 9216 = two tiles
    @ssbo @binding = 0 @role = "weight" @off = "wboff" wsh : array<float16> //!< 34B-block W blob, half-scale view
    @ssbo @binding = 1 @role = "weight" @off = "wboff" wqb : array<int8>    //!< the SAME blob buffer, byte view
    @ssbo @binding = 2 @off = "xoff" xf : array<XT>  //!< raw activations (float, or the converted f16 panel)
    @ssbo @binding = 3 @off = "yoff" @span = "mp*d*4" y : array<float>
    @uniform @binding = 4 kdim : uint
    @uniform @binding = 5 ndim : uint
    @workgroup twb : float16[TWB]

    [metal_kernel(float_a_ok=true)]
    def metal_q8_mulmm_t {
        let mBase = gl_WorkGroupID.x * MT
        let nBase = gl_WorkGroupID.y * 64u
        let blk0 = nBase * (kdim / 32u)
        var sp = unsafe(addr(wsh[blk0 * 17u]))
        var wp = unsafe(addr(wqb[blk0 * 34u + 2u]))
        var ap = unsafe(addr(xf[mBase * kdim]))
        var cp = unsafe(addr(y[mBase * ndim + nBase]))
        tmm2d_q8u_f32(MT, 64u, 4u, twb, sp, wp, ap, kdim, cp, ndim, kdim, kdim / 32u, gl_LocalInvocationIndex, BK)
    }
}

[metal_dispatch(name = "enc_gemm_mm_t_c", pso = "g_pf_pso_mm_t", tgmem = "MetalQ8MulMmT_metal_q8_mulmm_t_msl_tgmem", tg = 128, grid = "mp/32, d/64", params = "mp : int64, d : int64")]
class MetalQ8MulMmT : MetalQ8MulMmTensorT {
    typedef XT = float
}

[metal_dispatch(name = "enc_gemm_mm_th_c", pso = "g_pf_pso_mm_th", tgmem = "MetalQ8MulMmTH_metal_q8_mulmm_t_msl_tgmem", tg = 128, grid = "mp/32, d/64", params = "mp : int64, d : int64")]
class MetalQ8MulMmTH : MetalQ8MulMmTensorT {
    typedef XT = float16
}

[metal_dispatch(name = "enc_gemm_mm_t128_c", pso = "g_pf_pso_mm_t128", tgmem = "MetalQ8MulMmT128_metal_q8_mulmm_t_msl_tgmem", tg = 128, grid = "mp/128, d/64", params = "mp : int64, d : int64")]
class MetalQ8MulMmT128 : MetalQ8MulMmTensorT {
    typedef XT = float
    override MT = 128u
}

[metal_dispatch(name = "enc_gemm_mm_th128_c", pso = "g_pf_pso_mm_th128", tgmem = "MetalQ8MulMmTH128_metal_q8_mulmm_t_msl_tgmem", tg = 128, grid = "mp/128, d/64", params = "mp : int64, d : int64")]
class MetalQ8MulMmTH128 : MetalQ8MulMmTensorT {
    typedef XT = float16
    override MT = 128u
}

[metal_dispatch(name = "enc_gemm_mm_thdb128_c", pso = "g_pf_pso_mm_thdb128", tgmem = "MetalQ8MulMmTHDb128_metal_q8_mulmm_t_msl_tgmem", tg = 128, grid = "mp/128, d/64", params = "mp : int64, d : int64")]
class MetalQ8MulMmTHDb128 : MetalQ8MulMmTensorT {
    typedef XT = float16
    override MT = 128u
    override BK = 128u
    override TWB = 9216
}

[metal_dispatch(name = "enc_gemm_mm_thdb_c", pso = "g_pf_pso_mm_thdb", tgmem = "MetalQ8MulMmTHDb_metal_q8_mulmm_t_msl_tgmem", tg = 128, grid = "mp/32, d/64", params = "mp : int64, d : int64")]
class MetalQ8MulMmTHDb : MetalQ8MulMmTensorT {
    typedef XT = float16
    override BK = 128u
    override TWB = 9216
}

struct CvtArgs {
    total : uint
}

struct HalfRowsArgs {
    total : uint
    scale : float
}

//! f32 elements to halves at a byte offset of the target, scaled: an activation panel's twin, an attention K or V panel's
//! twin (the softmax scale on K), a window's K and V rows into the KV mirror, its Q rows under the scale the mirror's K lacks.
[metal_dispatch(name = "enc_half_rows", pso = "g_pf_pso_half_rows", tg = 256, grid = "total/256", params = "total : int64")]
class MetalHalfRows {
    @ssbo @binding = 0 @role = "read" x : array<float>
    @ssbo @binding = 1 @role = "write" @off = "hoff" @span = "total*2" xh : array<float16>
    @uniform @binding = 2 ka : HalfRowsArgs

    [metal_kernel(name="metal_half_rows_msl")]
    def metal_half_rows {
        let gid = gl_GlobalInvocationID.x
        if (gid >= ka.total) {
            return
        }
        xh[gid] = float16(min(max(x[gid] * ka.scale, -65504.0), 65504.0))   // nolint:STYLE019 — the MSL emitter lowers min/max only, no clamp; the host convert clamps the same way
    }
}

//! `n` f32 elements of `bx` as halves at the start of `bxh`, unscaled; `grid` is the dispatch's element span where a caller
//! rounds it past `n` (-1 = `n`).
def enc_cvt_half(enc : MetalComputeEncoder?; bx, bxh : MetalBuffer?; n : int64; grid : int64 = -1l) {
    var ha = HalfRowsArgs(total = uint(n), scale = 1.0)
    enc_half_rows(enc, bx, bxh, 0ul, ha, grid < 0l ? n : grid)
}

[metal_dispatch(name = "enc_dequant_q8h", pso = "g_pf_pso_dq_q8", tg = 256, grid = "total/256", params = "total : int64")]
class MetalDequantQ8H {
    @ssbo @binding = 0 @role = "weight" @off = "wboff" wsh : array<float16> //!< 34B-block W blob, half-scale view
    @ssbo @binding = 1 @role = "weight" @off = "wboff" wqb : array<int8>    //!< the SAME blob buffer, byte view
    @ssbo @binding = 2 @role = "write" wh : array<float16>                  //!< the dequantized W panel, d x kdim
    @uniform @binding = 3 ka : CvtArgs

    [metal_kernel(name="metal_dequant_q8h_msl")]
    def metal_dequant_q8h {
        let gid = gl_GlobalInvocationID.x
        if (gid >= ka.total) {
            return
        }
        let dscale = wsh[gid * 17u]
        for (k in range(32)) {
            wh[gid * 32u + uint(k)] = dscale * float16(float(wqb[gid * 34u + 2u + uint(k)]))
        }
    }
}

[ |> template_struct_instance]
class template MetalDequantK45T {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" ksh : array<float16> //!< 16B scale blocks, half view
    @ssbo @binding = 1 @role = "weight" @off = "soff" ks4 : array<uint4>   //!< the same buffer, uint4 view
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    //!< quant plane, uint view
    @ssbo @binding = 3 @role = "write" wh : array<float16>
    @uniform @binding = 4 ka : CvtArgs
    @template_constant SB_UINTS : uint = 32u
    @template_constant QH : bool = false

    [metal_kernel]
    def metal_dequant_k45 {
        let gid = gl_GlobalInvocationID.x
        if (gid >= ka.total) {
            return
        }
        let blk = gid / 8u
        let js = gid % 8u
        let sv = ks4[blk]
        var sc = 0u
        var mn = 0u
        kmask_scmn(sv, js, sc, mn)
        let dsc = float(ksh[blk * 8u]) * float(sc)
        let dmn = float(ksh[blk * 8u + 1u]) * float(mn)
        for (t in range(32)) {
            let e = uint(t)
            static_if (QH) {
                let u = kqu[blk * SB_UINTS + (js / 2u) * 8u + e / 4u]
                let hu = kqu[blk * SB_UINTS + 32u + e / 4u]
                let q = ((u >> (8u * (e % 4u) + (js & 1u) * 4u)) & 15u) | (((hu >> (8u * (e % 4u) + js)) & 1u) << 4u)
                wh[blk * 256u + js * 32u + e] = float16(dsc * float(q) - dmn)
            } else {
                let q = (kqu[blk * SB_UINTS + (js / 2u) * 8u + e / 4u] >> (8u * (e % 4u) + (js & 1u) * 4u)) & 15u
                wh[blk * 256u + js * 32u + e] = float16(dsc * float(q) - dmn)
            }
        }
    }
}

[metal_dispatch(name = "enc_dequant_k4h", pso = "g_pf_pso_dq_k4", tg = 256, grid = "total/256", params = "total : int64")]
class MetalDequantK4H : MetalDequantK45T {}

[metal_dispatch(name = "enc_dequant_k5h", pso = "g_pf_pso_dq_k5", tg = 256, grid = "total/256", params = "total : int64")]
class MetalDequantK5H : MetalDequantK45T {
    override SB_UINTS = 40u
    override QH = true
}

[metal_dispatch(name = "enc_dequant_k6h", pso = "g_pf_pso_dq_k6", tg = 256, grid = "total/256", params = "total : int64")]
class MetalDequantK6H {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16> //!< d plane (f16 tail at doff)
    @ssbo @binding = 1 @role = "weight" @off = "soff" ksc : array<uint4>   //!< 16B sub-scale blocks
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>    //!< ql at 48*blk, qh at 48*blk+32
    @ssbo @binding = 3 @role = "write" wh : array<float16>
    @uniform @binding = 4 ka : CvtArgs

    [metal_kernel(name="metal_dequant_k6h_msl"), arch(at="../ARCHITECTURE_IMAGE.md#image-devw-plane")]
    def metal_dequant_k6h {
        let gid = gl_GlobalInvocationID.x
        if (gid >= ka.total) {
            return
        }
        let blk = gid / 8u
        let js = gid % 8u
        let half6 = js / 4u
        let gg = js % 4u
        let nsh = (gg / 2u) * 4u
        let hsh = gg * 2u
        let dall = float(kdh[blk])
        let sv = ksc[blk]
        for (h in range(2)) {
            let ile = uint(h)
            let si = half6 * 8u + gg * 2u + ile
            let scw = si < 8u ? (si < 4u ? sv.x : sv.y) : (si < 12u ? sv.z : sv.w)
            let s6 = float(((int(scw >> ((si & 3u) * 8u)) & 255) ^ 128) - 128)
            let dsc = dall * s6
            for (t in range(16)) {
                let k = uint(t) / 4u
                let c = uint(t) % 4u
                let u = kqu[blk * 48u + half6 * 16u + (gg & 1u) * 8u + ile * 4u + k]
                let hu = kqu[blk * 48u + 32u + half6 * 8u + ile * 4u + k]
                let q = ((u >> (nsh + 8u * c)) & 15u) | (((hu >> (hsh + 8u * c)) & 3u) << 4u)
                wh[blk * 256u + js * 32u + ile * 16u + uint(t)] = float16(dsc * (float(q) - 32.0))
            }
        }
    }
}

[ |> template_struct_instance]
class template MetalHalfMulMmTensorT {
    @template_constant MT : uint = 32u      //!< M-tile rows: the panel re-reads M/MT times
    @template_constant L1D : bool = false   //!< 1-D grid: the launch order is FORCED linear (M-fastest), not a driver heuristic
    @ssbo @binding = 0 @role = "read" @off = "woff" wh : array<float16>   //!< pre-dequantized W panel, d x kdim (woff = a baked plane's site base)
    @ssbo @binding = 1 @role = "read" @off = "xoff" xf : array<float16>   //!< the f16 activation panel (half-X)
    @ssbo @binding = 2 @off = "yoff" y : array<float>
    @uniform @binding = 3 kdim : uint
    @uniform @binding = 4 ndim : uint

    [metal_kernel]
    def metal_half_mulmm_th {
        var mBase = gl_WorkGroupID.x * MT   // nolint:LINT003,LINT010,LINT019 — template: the L1D instance overwrites, the base instance keeps it
        var nBase = gl_WorkGroupID.y * 64u   // nolint:LINT003,LINT010,LINT019 — same
        static_if (L1D) {
            let mtiles = gl_NumWorkGroups.x / (ndim / 64u)
            mBase = (gl_WorkGroupID.x % mtiles) * MT
            nBase = (gl_WorkGroupID.x / mtiles) * 64u
        }
        var wp = unsafe(addr(wh[nBase * kdim]))
        var ap = unsafe(addr(xf[mBase * kdim]))
        var cp = unsafe(addr(y[mBase * ndim + nBase]))
        tmm2d_f16w_f32(MT, 64u, 4u, ap, kdim, wp, kdim, cp, ndim, kdim)
    }
}

[metal_dispatch(name = "enc_gemm_hmm_th", pso = "g_pf_pso_hmm_th", tg = 128, grid = "mp/32, d/64", params = "mp : int64, d : int64")]
class MetalHalfMulMmTH : MetalHalfMulMmTensorT {}

[metal_dispatch(name = "enc_gemm_hmm_th128", pso = "g_pf_pso_hmm_th128", tg = 128, grid = "mp/128, d/64", params = "mp : int64, d : int64")]
class MetalHalfMulMmTH128 : MetalHalfMulMmTensorT {
    override MT = 128u
}

[metal_dispatch(name = "enc_gemm_hmm_th128_l1", pso = "g_pf_pso_hmm_th128_l1", tg = 128, grid = "ntg", params = "ntg : int64")]
class MetalHalfMulMmTH128L1 : MetalHalfMulMmTensorT {
    override MT = 128u
    override L1D = true
}

[ |> template_struct_instance]
class template MetalBf16MulMmTensorT {
    @template_constant MT : uint = 32u
    @ssbo @binding = 0 @role = "weight" @off = "wboff" wbh : array<uint16> //!< bf16 W rows, halfword view
    @ssbo @binding = 2 @off = "xoff" xf : array<XT>  //!< raw activations (float, or the converted f16 panel)
    @ssbo @binding = 3 @off = "yoff" y : array<float>
    @uniform @binding = 4 kdim : uint
    @uniform @binding = 5 ndim : uint

    [metal_kernel(float_a_ok=true)]
    def metal_bf16_mulmm_t {
        let mBase = gl_WorkGroupID.x * MT
        let nBase = gl_WorkGroupID.y * 64u
        var ap = unsafe(addr(xf[mBase * kdim]))
        var wp = unsafe(addr(wbh[nBase * kdim]))
        var cp = unsafe(addr(y[mBase * ndim + nBase]))
        tmm2d_f32_bf16_f32(MT, 64u, 4u, ap, kdim, wp, kdim, cp, ndim, kdim)
    }
}

[metal_dispatch(name = "pf_enc_bf16_mm_t_c", pso = "g_pf_pso_bf16_mm", tg = 128, grid = "mp/32, nd/64", params = "mp : int64, nd : int64")]
class MetalBf16MulMmT : MetalBf16MulMmTensorT {
    typedef XT = float
}

[metal_dispatch(name = "pf_enc_bf16_mm_th_c", pso = "g_pf_pso_bf16_mm_th", tg = 128, grid = "mp/32, nd/64", params = "mp : int64, nd : int64")]
class MetalBf16MulMmTH : MetalBf16MulMmTensorT {
    typedef XT = float16
}

[metal_dispatch(name = "pf_enc_bf16_mm_t128_c", pso = "g_pf_pso_bf16_mm_t128", tg = 128, grid = "mp/128, nd/64", params = "mp : int64, nd : int64")]
class MetalBf16MulMmT128 : MetalBf16MulMmTensorT {
    typedef XT = float
    override MT = 128u
}

[metal_dispatch(name = "pf_enc_bf16_mm_th128_c", pso = "g_pf_pso_bf16_mm_th128", tg = 128, grid = "mp/128, nd/64", params = "mp : int64, nd : int64")]
class MetalBf16MulMmTH128 : MetalBf16MulMmTensorT {
    typedef XT = float16
    override MT = 128u
}

struct PleArgs {
    npos : uint
    ple : uint
    nlayers : uint
    sc0 : float         //!< sqrt(ple), folded into the gather's dequant
    inv_sd : float      //!< 1/sqrt(dim)
    eps : float
    inv_s2 : float      //!< 1/sqrt(2)
}

def private ple_ka(npos, ple, nlayers, dim : int64; eps : float) : PleArgs => (
    PleArgs(npos = uint(npos), ple = uint(ple), nlayers = uint(nlayers), sc0 = sqrt(float(ple)),
        inv_sd = 1.0 / sqrt(float(dim)), eps = eps, inv_s2 = 1.0 / sqrt(2.0)))

[ |> template_struct_instance]
class template MetalPleGatherBaseT {
    @template_constant Q8 : bool = false    //!< the q8 table: one interleaved 34B blob, both views of it at 0/1
    @template_constant SB_UINTS : uint = 32u     //!< k5's qh strip adds 8
    @template_constant QH : bool = false    //!< k5: the 5th bit off the qh strip
    @template_constant Q40 : bool = false   //!< q40: one f16 d per 32-block, the linear map (q - 8)
    @template_constant KSC : bool = true    //!< the packed 6-bit sc/min uint4 view - k4 and k5 alone
    @ssbo @binding = 0 @role = "weight" @off = "eoff" @template_gate = Q8 wsh : array<float16> //!< ple_emb q8 region, half-scale view (block b at 17*b)
    @ssbo @binding = 1 @role = "weight" @off = "eoff" @template_gate = Q8 wqb : array<int8>    //!< the SAME region, byte view (quants at 34*b + 2)
    @ssbo @binding = 0 @role = "weight" @off = "soff" @template_gate = "!Q8" ksh : array<float16> //!< scale plane, half view (k4/k5: d, dmin at 8*blk; q40: the eight per-32-block d)
    @ssbo @binding = 1 @role = "weight" @off = "soff" @template_gate = KSC ks4 : array<uint4> //!< the SAME 16B block, uint4 view (the packed 6-bit sc/min)
    @ssbo @binding = 2 @role = "read" tok : array<uint>    //!< [npos] token ids
    @ssbo @binding = 3 @role = "write" y : array<float>    //!< [n_layers x npos x ple] layer-major side input
    @uniform @binding = 4 ka : PleArgs
    @ssbo @binding = 6 @role = "weight" @off = "qoff" @template_gate = "!Q8" kqu : array<uint>    //!< quant plane, uint view

    def abstract decode_el(el : uint; var v : float&) : void

    def gather_body {
        let l = gl_WorkGroupID.x
        let p = gl_WorkGroupID.y
        let i = gl_LocalInvocationID.x      //! tg = ple threads (gate: ple % 32 == 0, <= 1024)
        let el = tok[p] * (ka.nlayers * ka.ple) + l * ka.ple + i
        var v = 0.0
        decode_el(el, v)
        y[(l * ka.npos + p) * ka.ple + i] = v * ka.sc0
    }
}

[metal_dispatch(name = "pf_enc_ple_gather", pso = "g_pf_pso_ple_gather", tg = "ple", grid = "nlayers, npos", params = "nlayers : int64, npos : int64, ple : int64")]
class MetalPleGatherQ8 : MetalPleGatherBaseT {
    override Q8 = true
    override KSC = false

    def override decode_el(el : uint; var v : float&) {
        let blk = el / 32u
        let sc = float(wsh[blk * 17u])
        let q = float(int(wqb[blk * 34u + 2u + (el % 32u)]))
        v = q * sc
    }

    [metal_kernel(name="metal_ple_gather_q8_msl")]
    def metal_ple_gather_q8 {
        gather_body()
    }
}

[ |> template_struct_instance]
class template MetalPleGatherKqT : MetalPleGatherBaseT {
    def override decode_el(el : uint; var v : float&) {
        let blk = el / 256u
        let js = (el % 256u) / 32u
        let e = el % 32u
        static_if (Q40) {
            let u = kqu[blk * SB_UINTS + js * 4u + (e % 16u) / 4u]
            v = float(ksh[blk * 8u + js]) * (float((u >> (8u * (e % 4u) + (e / 16u) * 4u)) & 15u) - 8.0)
        } else {
            let sv = ks4[blk]
            var sc = 0u
            var mn = 0u
            kmask_scmn(sv, js, sc, mn)
            let dsc = float(ksh[blk * 8u]) * float(sc)
            let dmn = float(ksh[blk * 8u + 1u]) * float(mn)
            let u = kqu[blk * SB_UINTS + (js / 2u) * 8u + e / 4u]
            static_if (QH) {
                let hu = kqu[blk * SB_UINTS + 32u + e / 4u]
                let q = ((u >> (8u * (e % 4u) + (js & 1u) * 4u)) & 15u) | (((hu >> (8u * (e % 4u) + js)) & 1u) << 4u)
                v = dsc * float(q) - dmn
            } else {
                v = dsc * float((u >> (8u * (e % 4u) + (js & 1u) * 4u)) & 15u) - dmn
            }
        }
    }

    [metal_kernel]
    def metal_ple_gather_kq {
        gather_body()
    }
}

[metal_dispatch(name = "pf_enc_ple_gather_k4", pso = "g_pf_pso_ple_gather_k4", tg = "ple", grid = "nlayers, npos", params = "nlayers : int64, npos : int64, ple : int64")]
class MetalPleGatherK4 : MetalPleGatherKqT {}

[metal_dispatch(name = "pf_enc_ple_gather_k5", pso = "g_pf_pso_ple_gather_k5", tg = "ple", grid = "nlayers, npos", params = "nlayers : int64, npos : int64, ple : int64")]
class MetalPleGatherK5 : MetalPleGatherKqT {
    override SB_UINTS = 40u
    override QH = true
}

[metal_dispatch(name = "pf_enc_ple_gather_q40", pso = "g_pf_pso_ple_gather_q40", tg = "ple", grid = "nlayers, npos", params = "nlayers : int64, npos : int64, ple : int64")]
class MetalPleGatherQ40 : MetalPleGatherKqT {
    override Q40 = true
    override KSC = false
}

[metal_dispatch(name = "pf_enc_ple_finish", pso = "g_pf_pso_ple_finish", tgmem = "metal_ple_finish_msl_tgmem", tg = "ple", grid = "nlayers, npos", params = "nlayers : int64, npos : int64, ple : int64")]
class MetalPleFinish : MetalTgReduceBase {
    @ssbo @binding = 0 @role = "read" proj : array<float>  //!< [mp x n_layers*ple] position-major model_proj·embd
    @ssbo @binding = 1 @role = "weight" pnorm : array<float> //!< [ple] proj-norm weight
    @ssbo @binding = 2 @role = "readwrite" y : array<float>     //!< [n_layers x npos x ple] side input, in place
    @uniform @binding = 4 ka : PleArgs

    [metal_kernel(name="metal_ple_finish_msl")]
    def metal_ple_finish {
        let l = gl_WorkGroupID.x
        let p = gl_WorkGroupID.y
        let i = gl_LocalInvocationID.x      //! tg = ple threads (gate: ple % 32 == 0, <= 1024)
        let v = proj[p * (ka.nlayers * ka.ple) + l * ka.ple + i] * ka.inv_sd
        let inv = tg_rms_inv(v * v, ka.ple, ka.eps)
        let yi = (l * ka.npos + p) * ka.ple + i
        y[yi] = (y[yi] + pnorm[i] * (v * inv)) * ka.inv_s2
    }
}


[ |> template_struct_instance]
class template MetalKqMulMmSplitTensorBase {
    @template_constant MT : uint = 32u
    @template_constant TWB : int = 6144     //!< W stage halves: one 64 wcols x 72-half padded tile (12288B lands mid-plateau); 9216 = the two ping-pong tiles `db_shell` runs
    @template_constant REM : bool = false   //!< the routed THR stamp's remainder pass
    @template_constant MOE : bool = false
    @template_constant DEQW : bool = false
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16>
    @ssbo @binding = 1 @role = "weight" @off = "soff" ksc : array<uint4>  //!< the SAME buffer as binding 0, uint4 view
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>
    @ssbo @binding = 3 @off = "xoff" xf : array<XT> //!< raw activations (float, or the converted f16 panel)
    @ssbo @binding = 4 @off = "yoff" @span = "mp*rows*4" y : array<float>
    @uniform @binding = 5 kdim : uint
    @uniform @binding = 6 ndim : uint
    @ssbo @binding = 7 @template_gate = MOE cnt : array<uint>
    @ssbo @binding = 8 @template_gate = MOE basep : array<uint>
    @ssbo @binding = 7 @template_gate = DEQW wh : array<float16>
    @workgroup twb : float16[TWB]           //!< W chunk: 64 wrows x 72-half padded rows

    //! decodes 16 W elements of chunk kb into the tile at tbase; tbase is 0 on every single-tile shell
    def abstract stage16(work, kb, nBase, nsb, tbase : uint) : void

    //! the format's per-threadgroup table staging - empty here, and every entry calls it
    def prologue {}

    def stage_tile(lid, kb, nBase, nsb, tbase : uint) {
        var work = lid
        while (work < 256u) {
            stage16(work, kb, nBase, nsb, tbase)
            work += gl_WorkGroupSize.x
        }
    }

    def run_shell {
        let mBase = gl_WorkGroupID.x * MT
        let nBase = gl_WorkGroupID.y * 64u
        let nkb = kdim / 32u
        let nsb = kdim / 256u
        let lid = gl_LocalInvocationID.x
        var acc : float[8192]
        var cp = unsafe(addr(y[mBase * ndim + nBase]))
        tmm2d_tg_begin_deva(acc, unsafe(addr(xf[0])), MT, 64u, 4u, 64u)
        var kb = 0u
        while (kb < nkb) {
            stage_tile(lid, kb, nBase, nsb, 0u)
            barrier()
            tmm2d_tg_step_deva(acc, unsafe(addr(xf[mBase * kdim + kb * 32u])), kdim, twb, MT, 64u, 64u)
            barrier()
            kb += 2u
        }
        tmm2d_tg_store(acc, cp, MT, 64u, ndim)
    }

    [arch(at="../ARCHITECTURE_IMAGE.md#image-devw-plane")]
    def db_shell {
        let mBase = gl_WorkGroupID.x * MT
        let nBase = gl_WorkGroupID.y * 64u
        let nkb = kdim / 32u
        let nsb = kdim / 256u
        var acc : float[8192]
        var cp = unsafe(addr(y[mBase * ndim + nBase]))
        tmm2d_tg_begin_deva(acc, unsafe(addr(xf[0])), MT, 64u, 4u, 64u)
        stage_tile(gl_LocalInvocationID.x, 0u, nBase, nsb, 0u)
        var kb = 0u
        while (kb < nkb) {
            barrier()
            if (kb + 2u < nkb) {
                stage_tile(gl_LocalInvocationID.x, kb + 2u, nBase, nsb, (((kb / 2u) + 1u) & 1u) * 4608u)
            }
            tmm2d_tg_step_deva(acc, unsafe(addr(xf[mBase * kdim + kb * 32u])), kdim, unsafe(addr(twb[((kb / 2u) & 1u) * 4608u])), MT, 64u, 64u)
            kb += 2u
        }
        tmm2d_tg_store(acc, cp, MT, 64u, ndim)
    }

    def moe_shell(erows, rbase : uint; rem : bool) {
        let e = gl_WorkGroupID.z
        let mBase = gl_WorkGroupID.x * MT + (rem ? erows / 128u * 128u : 0u)
        let lim = MT == 128u ? erows / 128u * 128u : (erows + 31u) / 32u * 32u
        if (mBase >= lim) {
            return
        }
        let nBase = gl_WorkGroupID.y * 64u
        let nkb = kdim / 32u
        let nsb = kdim / 256u
        let lid = gl_LocalInvocationID.x
        var acc : float[8192]
        var cp = unsafe(addr(y[(rbase + mBase) * ndim + nBase]))
        tmm2d_tg_begin_deva(acc, unsafe(addr(xf[0])), MT, 64u, 4u, 64u)
        var kb = 0u
        while (kb < nkb) {
            // the expert plane base rides the stage16 call - a stage_tile argument is emitted once outside the work loop, which moves the decode's shape
            var work = lid
            while (work < 256u) {
                stage16(work, kb, e * ndim + nBase, nsb, 0u)
                work += gl_WorkGroupSize.x
            }
            barrier()
            tmm2d_tg_step_deva(acc, unsafe(addr(xf[(rbase + mBase) * kdim + kb * 32u])), kdim, twb, MT, 64u, 64u)
            barrier()
            kb += 2u
        }
        tmm2d_tg_store(acc, cp, MT, 64u, ndim)
    }

    def moe_kernel {
        static_if (MOE) {
            prologue()
            moe_shell(cnt[gl_WorkGroupID.z], basep[gl_WorkGroupID.z], REM)
        }
    }

    def dequant_shell {
        static_if (DEQW) {
            prologue()
            dequant_tile(gl_WorkGroupID.x * 2u, gl_WorkGroupID.y * 64u, kdim / 256u)
            for (i in range(32)) {
                let e = gl_LocalInvocationID.x + uint(i) * 128u
                wh[dequant_idx(e)] = dequant_val(e)
            }
        }
    }

    def dequant_tile(kb, nBase, nsb : uint) {
        stage_tile(gl_LocalInvocationID.x, kb, nBase, nsb, 0u)
        barrier()
    }

    // staged element e -> its index in the f16 panel (wrow j = e/64, k column c = e%64)
    def dequant_idx(e : uint) : uint => (gl_WorkGroupID.y * 64u + (e >> 6u)) * kdim + gl_WorkGroupID.x * 64u + (e & 63u)

    def dequant_val(e : uint) : float16 => twb[(e >> 6u) * 72u + (e & 63u)]
}

[ |> template_struct_instance]
class template MetalKqMulMmK45DecodeT : MetalKqMulMmSplitTensorBase {
    @template_constant SB_UINTS : uint = 32u
    @template_constant QH : bool = false

    def override stage16(work, kb, nBase, nsb, tbase : uint) {
        let j = work >> 2u
        let sub = work & 3u
        let jsl = kb + (sub >> 1u)
        let sb = jsl / 8u
        let js = jsl % 8u
        let kh = (sub & 1u) * 16u
        let blk = (nBase + j) * nsb + sb
        let sv = ksc[blk]
        var sc = 0u
        var mn = 0u
        kmask_scmn(sv, js, sc, mn)
        let dsc = float(kdh[blk * 8u]) * float(sc)
        let dmn = float(kdh[blk * 8u + 1u]) * float(mn)
        static_if (QH) {
            let nsh = (js & 1u) * 4u
            let qp = unsafe(addr(kqu[blk * SB_UINTS + (js / 2u) * 8u + kh / 4u]))
            let hp = unsafe(addr(kqu[blk * SB_UINTS + 32u + kh / 4u]))
            let tb0 = tbase + j * 72u + (sub >> 1u) * 32u + kh
            for [unroll_full] (k in range(4)) {
                let u = unsafe(qp[k])
                let hu = unsafe(hp[k])
                let qv = ((u >> nsh) & 0x0F0F0F0F) | (((hu >> js) & 0x01010101) << 4u)
                for [unroll_full] (c in range(4)) {
                    twb[tb0 + uint(k) * 4u + uint(c)] = float16(dsc * float((qv >> (8u * uint(c))) & 255u) - dmn)
                }
            }
        } else {
            let qp = unsafe(addr(kqu[blk * SB_UINTS + (js / 2u) * 8u + kh / 4u]))   //! kh is 0 or 16: element kh + t sits in word kh / 4 + t / 4 at byte t % 4
            for [unroll_full] (t in range(16)) {
                let q = (unsafe(qp[t / 4]) >> (8u * uint(t % 4) + (js & 1u) * 4u)) & 15u
                twb[tbase + j * 72u + (sub >> 1u) * 32u + kh + uint(t)] = float16(dsc * float(q) - dmn)
            }
        }
    }
}

[ |> template_struct_instance]
class template MetalKqMulMmK45TensorT : MetalKqMulMmK45DecodeT {
    [metal_kernel(float_a_ok=true)]
    def metal_kq_mulmm_t {
        run_shell()
    }
}

[metal_dispatch(stamp = "kq_mm:k4:t", pso = "g_pf_pso_kq_mm4_t")]
class MetalKqMulMmK4T : MetalKqMulMmK45TensorT {
    typedef XT = float
}

[metal_dispatch(stamp = "kq_mm:k4:th", pso = "g_pf_pso_kq_mm4_th")]
class MetalKqMulMmK4TH : MetalKqMulMmK45TensorT {
    typedef XT = float16
}

[metal_dispatch(stamp = "kq_mm:k5:t", pso = "g_pf_pso_kq_mm5_t")]
class MetalKqMulMmK5T : MetalKqMulMmK45TensorT {
    typedef XT = float
    override SB_UINTS = 40u
    override QH = true
}

[metal_dispatch(stamp = "kq_mm:k5:th", pso = "g_pf_pso_kq_mm5_th")]
class MetalKqMulMmK5TH : MetalKqMulMmK45TensorT {
    typedef XT = float16
    override SB_UINTS = 40u
    override QH = true
}

// tall 128-row stamps: in-kernel dequant with W read once per 128-row tile - the DRAM-class
// panel form (dev-W's materialized panel re-streams 2B/element; these read the quant plane)
[metal_dispatch(stamp = "kq_mm:k4:th128", pso = "g_pf_pso_kq_mm4_th128")]
class MetalKqMulMmK4TH128 : MetalKqMulMmK45TensorT {
    typedef XT = float16
    override MT = 128u
}

[metal_dispatch(stamp = "kq_mm:k5:th128", pso = "g_pf_pso_kq_mm5_th128")]
class MetalKqMulMmK5TH128 : MetalKqMulMmK45TensorT {
    typedef XT = float16
    override SB_UINTS = 40u
    override QH = true
    override MT = 128u
}

[ |> template_struct_instance]
class template MetalKqMulMmK45DbT : MetalKqMulMmK45DecodeT {
    override TWB = 9216

    [metal_kernel(float_a_ok=true)]
    def metal_kq_mulmm_db {
        db_shell()
    }
}

[metal_dispatch(stamp = "kq_mm:k4:thdb", pso = "g_pf_pso_kq_mm4_thdb")]
class MetalKqMulMmK4THDb : MetalKqMulMmK45DbT {
    typedef XT = float16
}

[metal_dispatch(stamp = "kq_mm:k4:thdb128", pso = "g_pf_pso_kq_mm4_thdb128")]
class MetalKqMulMmK4THDb128 : MetalKqMulMmK45DbT {
    typedef XT = float16
    override MT = 128u
}

[metal_dispatch(stamp = "kq_mm:k5:thdb", pso = "g_pf_pso_kq_mm5_thdb")]
class MetalKqMulMmK5THDb : MetalKqMulMmK45DbT {
    typedef XT = float16
    override SB_UINTS = 40u
    override QH = true
}

[metal_dispatch(stamp = "kq_mm:k5:thdb128", pso = "g_pf_pso_kq_mm5_thdb128")]
class MetalKqMulMmK5THDb128 : MetalKqMulMmK45DbT {
    typedef XT = float16
    override SB_UINTS = 40u
    override QH = true
    override MT = 128u
}

[ |> template_struct_instance]
class template MetalKqMulMmK6DecodeT : MetalKqMulMmSplitTensorBase {
    def override stage16(work, kb, nBase, nsb, tbase : uint) {
        let j = work >> 2u
        let sub = work & 3u
        let jsl = kb + (sub >> 1u)
        let sb = jsl / 8u
        let js = jsl % 8u
        let half6 = js / 4u
        let gg = js % 4u
        let nsh = (gg / 2u) * 4u
        let hsh = gg * 2u
        let kh = (sub & 1u) * 16u
        let ile = kh / 16u
        let blk = (nBase + j) * nsb + sb
        let dall = float(kdh[blk])
        let si = half6 * 8u + gg * 2u + ile
        let sv = ksc[blk]
        let scw = si < 8u ? (si < 4u ? sv.x : sv.y) : (si < 12u ? sv.z : sv.w)
        let s6 = float(((int(scw >> ((si & 3u) * 8u)) & 255) ^ 128) - 128)
        let dsc = dall * s6
        for [unroll_full] (t in range(16)) {
            let k = uint(t) / 4u
            let c = uint(t) % 4u
            let u = kqu[blk * 48u + half6 * 16u + (gg & 1u) * 8u + ile * 4u + k]
            let hu = kqu[blk * 48u + 32u + half6 * 8u + ile * 4u + k]
            let q = ((u >> (nsh + 8u * c)) & 15u) | (((hu >> (hsh + 8u * c)) & 3u) << 4u)
            twb[tbase + j * 72u + (sub >> 1u) * 32u + kh + uint(t)] = float16(dsc * (float(q) - 32.0))
        }
    }
}

[ |> template_struct_instance]
class template MetalKqMulMmK6TensorT : MetalKqMulMmK6DecodeT {
    [metal_kernel(float_a_ok=true)]
    def metal_kq_mulmm_k6_t {
        run_shell()
    }
}

[metal_dispatch(stamp = "kq_mm:k6:t", pso = "g_pf_pso_kq_mm6_t")]
class MetalKqMulMmK6T : MetalKqMulMmK6TensorT {
    typedef XT = float
}

[metal_dispatch(stamp = "kq_mm:k6:th", pso = "g_pf_pso_kq_mm6_th")]
class MetalKqMulMmK6TH : MetalKqMulMmK6TensorT {
    typedef XT = float16
}

[metal_dispatch(stamp = "kq_mm:k6:th128", pso = "g_pf_pso_kq_mm6_th128")]
class MetalKqMulMmK6TH128 : MetalKqMulMmK6TensorT {
    typedef XT = float16
    override MT = 128u
}

[ |> template_struct_instance]
class template MetalKqMulMmK6DbT : MetalKqMulMmK6DecodeT {
    override TWB = 9216

    [metal_kernel(float_a_ok=true)]
    def metal_kq_mulmm_db {
        db_shell()
    }
}

[metal_dispatch(stamp = "kq_mm:k6:thdb", pso = "g_pf_pso_kq_mm6_thdb")]
class MetalKqMulMmK6THDb : MetalKqMulMmK6DbT {
    typedef XT = float16
}

[metal_dispatch(stamp = "kq_mm:k6:thdb128", pso = "g_pf_pso_kq_mm6_thdb128")]
class MetalKqMulMmK6THDb128 : MetalKqMulMmK6DbT {
    typedef XT = float16
    override MT = 128u
}

[ |> template_struct_instance]
class template MetalKqMulMmIq4xsTensorT : MetalKqMulMmSplitTensorBase {
    @template_constant IQ4NL : bool = false
    @template_constant Q40 : bool = false   //!< q40: iq4nl's d plane, the linear nibble map (q - 8) in the codebook slot
    @workgroup iq4lutt : float[16]   //!< kvalues_iq4nl as floats, staged once per threadgroup

    [metal_kernel(float_a_ok=true)]
    def metal_kq_mulmm_iq4xs_t {
        prologue()
        run_shell()
    }

    def override prologue {
        if (gl_LocalInvocationID.x < 16u) {
            stage_lut()
        }
        barrier()
    }

    def stage_lut {
        static_if (Q40) {
            iq4lutt[gl_LocalInvocationID.x] = float(int(gl_LocalInvocationID.x)) - 8.0
        } else {
            iq4lutt[gl_LocalInvocationID.x] = iq4_lut(gl_LocalInvocationID.x)
        }
    }

    def override stage16(work, kb, nBase, nsb, tbase : uint) {
        let j = work >> 2u
        let sub = work & 3u
        let jsl = kb + (sub >> 1u)
        let sb = jsl / 8u
        let js = jsl % 8u
        let kh = (sub & 1u) * 16u
        let blk = (nBase + j) * nsb + sb
        var dsc : float
        static_if (IQ4NL) {
            dsc = float(kdh[blk * 8u + js])
        } else {
            let sv = ksc[blk]
            let scw = js < 4u ? sv.x : sv.y
            dsc = float(kdh[blk]) * float(((int(scw >> ((js & 3u) * 8u)) & 255) ^ 128) - 128)
        }
        let qp = unsafe(addr(kqu[blk * 32u + js * 4u]))
        let nsh = (sub & 1u) * 4u
        for [unroll_full] (k in range(4)) {
            let u = unsafe(qp[k])
            for [unroll_full] (c in range(4)) {
                twb[tbase + j * 72u + (sub >> 1u) * 32u + kh + uint(k) * 4u + uint(c)] = float16(dsc * iq4lutt[(u >> (8u * uint(c) + nsh)) & 15u])
            }
        }
    }
}

[metal_dispatch(stamp = "kq_mm:iq4xs:t", pso = "g_pf_pso_kq_mm44_t")]
class MetalKqMulMmIq4xsT : MetalKqMulMmIq4xsTensorT {
    typedef XT = float
}

[metal_dispatch(stamp = "kq_mm:iq4xs:th", pso = "g_pf_pso_kq_mm44_th")]
class MetalKqMulMmIq4xsTH : MetalKqMulMmIq4xsTensorT {
    typedef XT = float16
}

[ |> template_struct_instance]
class template MetalMoeMulMmIq4xsSplitT : MetalKqMulMmIq4xsTensorT {
    override MOE = true

    [metal_kernel(float_a_ok=true)]
    def metal_moe_mulmm_iq4xs_t {
        moe_kernel()
    }
}

[metal_dispatch(stamp = "moe_mm_split:iq4xs:t")]
class MetalMoeMulMmIq4xsT : MetalMoeMulMmIq4xsSplitT {
    typedef XT = float
}

[metal_dispatch(stamp = "moe_mm_split:iq4xs:th")]
class MetalMoeMulMmIq4xsTH : MetalMoeMulMmIq4xsSplitT {
    typedef XT = float16
}

[metal_dispatch(stamp = "moe_mm_split:iq4xs:th128")]
class MetalMoeMulMmIq4xsTH128 : MetalMoeMulMmIq4xsSplitT {
    typedef XT = float16
    override MT = 128u
}

[metal_dispatch(stamp = "moe_mm_split:iq4xs:thr")]
class MetalMoeMulMmIq4xsTHR : MetalMoeMulMmIq4xsSplitT {
    typedef XT = float16
    override REM = true
}

[metal_dispatch(stamp = "kq_mm:iq4nl:t", pso = "g_pf_pso_kq_mm45_t")]
class MetalKqMulMmIq4nlT : MetalKqMulMmIq4xsTensorT {
    typedef XT = float
    override IQ4NL = true
}

[metal_dispatch(stamp = "kq_mm:iq4nl:th", pso = "g_pf_pso_kq_mm45_th")]
class MetalKqMulMmIq4nlTH : MetalKqMulMmIq4xsTensorT {
    typedef XT = float16
    override IQ4NL = true
}

[metal_dispatch(stamp = "kq_mm:q40:t", pso = "g_pf_pso_kq_mm40_t", requires = "mp % 32, rows % 64")]
class MetalKqMulMmQ40T : MetalKqMulMmIq4xsTensorT {
    typedef XT = float
    override IQ4NL = true
    override Q40 = true
}

[metal_dispatch(stamp = "kq_mm:q40:th", pso = "g_pf_pso_kq_mm40_th")]
class MetalKqMulMmQ40TH : MetalKqMulMmIq4xsTensorT {
    typedef XT = float16
    override IQ4NL = true
    override Q40 = true
}

[metal_dispatch(stamp = "moe_mm_split:q40:t")]
class MetalMoeMulMmQ40T : MetalMoeMulMmIq4xsSplitT {
    typedef XT = float
    override IQ4NL = true
    override Q40 = true
}

[metal_dispatch(stamp = "moe_mm_split:q40:th")]
class MetalMoeMulMmQ40TH : MetalMoeMulMmIq4xsSplitT {
    typedef XT = float16
    override IQ4NL = true
    override Q40 = true
}

[metal_dispatch(stamp = "moe_mm_split:q40:th128")]
class MetalMoeMulMmQ40TH128 : MetalMoeMulMmIq4xsSplitT {
    typedef XT = float16
    override IQ4NL = true
    override Q40 = true
    override MT = 128u
}

[metal_dispatch(stamp = "moe_mm_split:q40:thr")]
class MetalMoeMulMmQ40THR : MetalMoeMulMmIq4xsSplitT {
    typedef XT = float16
    override IQ4NL = true
    override Q40 = true
    override REM = true
}

[ |> template_struct_instance]
class template MetalKqMulMmK3TensorT : MetalKqMulMmSplitTensorBase {
    [metal_kernel(float_a_ok=true)]
    def metal_kq_mulmm_k3_t {
        run_shell()
    }

    def override stage16(work, kb, nBase, nsb, tbase : uint) {
        let j = work >> 2u
        let sub = work & 3u
        let jsl = kb + (sub >> 1u)
        let sb = jsl / 8u
        let js = jsl % 8u
        let kh = (sub & 1u) * 16u
        let ile = sub & 1u
        let blk = (nBase + j) * nsb + sb
        let hf3 = js / 4u
        let gg3 = js % 4u
        let sv = ksc[blk]
        let si = hf3 * 8u + gg3 * 2u + ile
        let scw = si < 8u ? (si < 4u ? sv.x : sv.y) : (si < 12u ? sv.z : sv.w)
        let s3 = float(((int(scw >> ((si & 3u) * 8u)) & 255) ^ 128) - 128)
        let dsc = float(kdh[blk]) * s3
        let dmn = dsc * 4.0
        let dsc1 = dsc * 0.00390625
        let dsc2 = dsc * 0.0000152587890625
        let dsc3 = dsc * 0.000000059604644775390625
        let qp = unsafe(addr(kqu[blk * 24u + hf3 * 8u + ile * 4u]))
        let hp = unsafe(addr(kqu[blk * 24u + 16u + ile * 4u]))
        let nsh = gg3 * 2u
        let hsh = hf3 * 4u + gg3
        let tb0 = tbase + j * 72u + (sub >> 1u) * 32u + kh
        for [unroll_full] (k in range(4)) {
            let u = unsafe(qp[k])
            let hu = unsafe(hp[k])
            let qv = ((u >> nsh) & 0x03030303) | (((hu >> hsh) & 0x01010101) << 2u)
            twb[tb0 + uint(k) * 4u] = float16(dsc * float(qv & 255u) - dmn)
            twb[tb0 + uint(k) * 4u + 1u] = float16(dsc1 * float(qv & 65280u) - dmn)
            twb[tb0 + uint(k) * 4u + 2u] = float16(dsc2 * float(qv & 16711680u) - dmn)
            twb[tb0 + uint(k) * 4u + 3u] = float16(dsc3 * float(qv & 4278190080u) - dmn)
        }
    }
}

[metal_dispatch(stamp = "kq_mm:k3:t", pso = "g_pf_pso_kq_mm3_t")]
class MetalKqMulMmK3T : MetalKqMulMmK3TensorT {
    typedef XT = float
}

[metal_dispatch(stamp = "kq_mm:k3:th", pso = "g_pf_pso_kq_mm3_th")]
class MetalKqMulMmK3TH : MetalKqMulMmK3TensorT {
    typedef XT = float16
}

[ |> template_struct_instance]
class template MetalMoeMulMmK3SplitT : MetalKqMulMmK3TensorT {
    override MOE = true

    [metal_kernel(float_a_ok=true)]
    def metal_moe_mulmm_k3_t {
        moe_kernel()
    }
}

[metal_dispatch(stamp = "moe_mm_split:k3:t")]
class MetalMoeMulMmK3T : MetalMoeMulMmK3SplitT {
    typedef XT = float
}

[metal_dispatch(stamp = "moe_mm_split:k3:th")]
class MetalMoeMulMmK3TH : MetalMoeMulMmK3SplitT {
    typedef XT = float16
}

[metal_dispatch(stamp = "moe_mm_split:k3:th128")]
class MetalMoeMulMmK3TH128 : MetalMoeMulMmK3SplitT {
    typedef XT = float16
    override MT = 128u
}

[metal_dispatch(stamp = "moe_mm_split:k3:thr")]
class MetalMoeMulMmK3THR : MetalMoeMulMmK3SplitT {
    typedef XT = float16
    override REM = true
}

[ |> template_struct_instance]
class template MetalKqMulMmK2TensorT : MetalKqMulMmSplitTensorBase {
    [metal_kernel(float_a_ok=true)]
    def metal_kq_mulmm_k2_t {
        run_shell()
    }

    def override stage16(work, kb, nBase, nsb, tbase : uint) {
        let j = work >> 2u
        let sub = work & 3u
        let jsl = kb + (sub >> 1u)
        let sb = jsl / 8u
        let js = jsl % 8u
        let kh = (sub & 1u) * 16u
        let ile = sub & 1u
        let blk = (nBase + j) * nsb + sb
        let hf3 = js / 4u
        let gg3 = js % 4u
        let sv = ksc[blk]
        let si = hf3 * 8u + gg3 * 2u + ile
        let pw = si < 8u ? (si < 4u ? sv.x : sv.y) : (si < 12u ? sv.z : sv.w)
        let pb = (pw >> ((si & 3u) * 8u)) & 255u
        let dsc = float(kdh[blk * 2u]) * float(pb & 15u)
        let dmn = float(kdh[blk * 2u + 1u]) * float(pb >> 4u)
        let dsc1 = dsc * 0.00390625
        let dsc2 = dsc * 0.0000152587890625
        let dsc3 = dsc * 0.000000059604644775390625
        let qp = unsafe(addr(kqu[blk * 16u + hf3 * 8u + ile * 4u]))
        let nsh = gg3 * 2u
        let tb0 = tbase + j * 72u + (sub >> 1u) * 32u + kh
        for [unroll_full] (k in range(4)) {
            let qv = (unsafe(qp[k]) >> nsh) & 0x03030303
            twb[tb0 + uint(k) * 4u] = float16(dsc * float(qv & 255u) - dmn)
            twb[tb0 + uint(k) * 4u + 1u] = float16(dsc1 * float(qv & 65280u) - dmn)
            twb[tb0 + uint(k) * 4u + 2u] = float16(dsc2 * float(qv & 16711680u) - dmn)
            twb[tb0 + uint(k) * 4u + 3u] = float16(dsc3 * float(qv & 4278190080u) - dmn)
        }
    }
}

[metal_dispatch(stamp = "kq_mm:k2:t", pso = "g_pf_pso_kq_mm2_t")]
class MetalKqMulMmK2T : MetalKqMulMmK2TensorT {
    typedef XT = float
}

[metal_dispatch(stamp = "kq_mm:k2:th", pso = "g_pf_pso_kq_mm2_th")]
class MetalKqMulMmK2TH : MetalKqMulMmK2TensorT {
    typedef XT = float16
}

[ |> template_struct_instance]
class template MetalKqMulMmIq3sTensorT : MetalKqMulMmSplitTensorBase {
    @workgroup gridw3t : uint[512]   //!< the 2 KB iq3s grid, staged once per threadgroup

    [metal_kernel(float_a_ok=true)]
    def metal_kq_mulmm_iq3s_t {
        prologue()
        run_shell()
    }

    def override prologue {
        for [unroll_full] (gk in range(4)) {
            let gi = gl_LocalInvocationID.x + uint(gk) * 128u
            gridw3t[gi] = iq3s_gw(int(gi))
        }
        barrier()
    }

    def override stage16(work, kb, nBase, nsb, tbase : uint) {
        let j = work >> 2u
        let sub = work & 3u
        let jsl = kb + (sub >> 1u)
        let sb = jsl / 8u
        let js = jsl % 8u
        let kh = (sub & 1u) * 16u
        let ile = sub & 1u
        let blk = (nBase + j) * nsb + sb
        let sv = ksc[blk]
        let scw = js < 4u ? sv.x : sv.y
        let dsc = float(kdh[blk]) * float(((int(scw >> ((js & 3u) * 8u)) & 255) ^ 128) - 128)
        let qsw = kqu[blk * 26u + js * 2u + ile]
        let qh3 = (kqu[blk * 26u + 16u + js / 4u] >> ((js % 4u) * 8u)) & 255u
        let sgw3 = kqu[blk * 26u + 18u + js]
        let tb0 = tbase + j * 72u + (sub >> 1u) * 32u + kh
        for [unroll_full] (k in range(4)) {
            let b3 = ile * 4u + uint(k)
            let idx = ((qsw >> (8u * uint(k))) & 255u) | ((qh3 << (8u - b3)) & 256u)
            let nib = (sgw3 >> ((b3 / 2u) * 8u + (b3 % 2u) * 4u)) & 15u
            let gw = gridw3t[idx]
            let fsd = iq_sign_apply(float4(dsc), nib)
            let mv = u8x4_f4(gw) * fsd
            twb[tb0 + uint(k) * 4u] = float16(mv.x)
            twb[tb0 + uint(k) * 4u + 1u] = float16(mv.y)
            twb[tb0 + uint(k) * 4u + 2u] = float16(mv.z)
            twb[tb0 + uint(k) * 4u + 3u] = float16(mv.w)
        }
    }
}

[metal_dispatch(stamp = "kq_mm:iq3s:t", pso = "g_pf_pso_kq_mm33_t")]
class MetalKqMulMmIq3sT : MetalKqMulMmIq3sTensorT {
    typedef XT = float
}

[metal_dispatch(stamp = "kq_mm:iq3s:th", pso = "g_pf_pso_kq_mm33_th")]
class MetalKqMulMmIq3sTH : MetalKqMulMmIq3sTensorT {
    typedef XT = float16
}

[ |> template_struct_instance]
class template MetalMoeMulMmIq3sSplitT : MetalKqMulMmIq3sTensorT {
    override MOE = true

    [metal_kernel(float_a_ok=true)]
    def metal_moe_mulmm_iq3s_t {
        moe_kernel()
    }
}

[metal_dispatch(stamp = "moe_mm_split:iq3s:t")]
class MetalMoeMulMmIq3sT : MetalMoeMulMmIq3sSplitT {
    typedef XT = float
}

[metal_dispatch(stamp = "moe_mm_split:iq3s:th")]
class MetalMoeMulMmIq3sTH : MetalMoeMulMmIq3sSplitT {
    typedef XT = float16
}

[metal_dispatch(stamp = "moe_mm_split:iq3s:th128")]
class MetalMoeMulMmIq3sTH128 : MetalMoeMulMmIq3sSplitT {
    typedef XT = float16
    override MT = 128u
}

[metal_dispatch(stamp = "moe_mm_split:iq3s:thr")]
class MetalMoeMulMmIq3sTHR : MetalMoeMulMmIq3sSplitT {
    typedef XT = float16
    override REM = true
}

[metal_dispatch(stamp = "kq_mm:iq3s:th128", pso = "g_pf_pso_kq_mm33_th128", requires = "mp % 128, rows % 64")]
class MetalKqMulMmIq3sTH128 : MetalKqMulMmIq3sTensorT {
    typedef XT = float16
    override MT = 128u
}

[ |> template_struct_instance]
class template MetalKqMulMmIq3xxsTensorT : MetalKqMulMmSplitTensorBase {
    @workgroup gridw3xt : uint[256]   //!< the 1 KB halved iq3xxs grid, staged once per threadgroup

    [metal_kernel(float_a_ok=true)]
    def metal_kq_mulmm_iq3xxs_t {
        prologue()
        run_shell()
    }

    def override prologue {
        for [unroll_full] (gk in range(2)) {
            let gi = gl_LocalInvocationID.x + uint(gk) * 128u
            gridw3xt[gi] = iq3xxs_gw(int(gi))
        }
        barrier()
    }

    def override stage16(work, kb, nBase, nsb, tbase : uint) {
        let j = work >> 2u
        let sub = work & 3u
        let jsl = kb + (sub >> 1u)
        let sb = jsl / 8u
        let js = jsl % 8u
        let kh = (sub & 1u) * 16u
        let ile = sub & 1u
        let blk = (nBase + j) * nsb + sb
        let sv = ksc[blk]
        let scw = js < 4u ? sv.x : sv.y
        let dsc = float(kdh[blk]) * float(((int(scw >> ((js & 3u) * 8u)) & 255) ^ 128) - 128)
        let qsw = kqu[blk * 24u + js * 2u + ile]
        let aux3 = kqu[blk * 24u + 16u + js]
        let tb0 = tbase + j * 72u + (sub >> 1u) * 32u + kh
        for [unroll_full] (k in range(4)) {
            let q3 = ile * 2u + uint(k) / 2u
            let sgb = ksign7((aux3 >> (7u * q3)) & 127u)
            let nib = uint(k) % 2u == 0u ? sgb & 15u : sgb >> 4u
            let gw = gridw3xt[(qsw >> (8u * uint(k))) & 255u]
            let fsd = iq_sign_apply(float4(dsc), nib)
            let mv = u8x4_f4(gw) * fsd
            twb[tb0 + uint(k) * 4u] = float16(mv.x)
            twb[tb0 + uint(k) * 4u + 1u] = float16(mv.y)
            twb[tb0 + uint(k) * 4u + 2u] = float16(mv.z)
            twb[tb0 + uint(k) * 4u + 3u] = float16(mv.w)
        }
    }
}

[metal_dispatch(stamp = "kq_mm:iq3xxs:t", pso = "g_pf_pso_kq_mm34_t")]
class MetalKqMulMmIq3xxsT : MetalKqMulMmIq3xxsTensorT {
    typedef XT = float
}

[metal_dispatch(stamp = "kq_mm:iq3xxs:th", pso = "g_pf_pso_kq_mm34_th")]
class MetalKqMulMmIq3xxsTH : MetalKqMulMmIq3xxsTensorT {
    typedef XT = float16
}

[ |> template_struct_instance]
class template MetalMoeMulMmIq3xxsSplitT : MetalKqMulMmIq3xxsTensorT {
    override MOE = true

    [metal_kernel(float_a_ok=true)]
    def metal_moe_mulmm_iq3xxs_t {
        moe_kernel()
    }
}

[metal_dispatch(stamp = "moe_mm_split:iq3xxs:t")]
class MetalMoeMulMmIq3xxsT : MetalMoeMulMmIq3xxsSplitT {
    typedef XT = float
}

[metal_dispatch(stamp = "moe_mm_split:iq3xxs:th")]
class MetalMoeMulMmIq3xxsTH : MetalMoeMulMmIq3xxsSplitT {
    typedef XT = float16
}

[metal_dispatch(stamp = "moe_mm_split:iq3xxs:th128")]
class MetalMoeMulMmIq3xxsTH128 : MetalMoeMulMmIq3xxsSplitT {
    typedef XT = float16
    override MT = 128u
}

[metal_dispatch(stamp = "moe_mm_split:iq3xxs:thr")]
class MetalMoeMulMmIq3xxsTHR : MetalMoeMulMmIq3xxsSplitT {
    typedef XT = float16
    override REM = true
}

[metal_dispatch(stamp = "kq_mm:iq3xxs:th128", pso = "g_pf_pso_kq_mm34_th128")]
class MetalKqMulMmIq3xxsTH128 : MetalKqMulMmIq3xxsTensorT {
    typedef XT = float16
    override MT = 128u
}

[ |> template_struct_instance]
class template MetalKqMulMmIq2sTensorT : MetalKqMulMmSplitTensorBase {
    [metal_kernel(float_a_ok=true)]
    def metal_kq_mulmm_iq2s_t {
        run_shell()
    }

    def override stage16(work, kb, nBase, nsb, tbase : uint) {
        let j = work >> 2u
        let sub = work & 3u
        let jsl = kb + (sub >> 1u)
        let sb = jsl / 8u
        let js = jsl % 8u
        let kh = (sub & 1u) * 16u
        let ile = sub & 1u
        let blk = (nBase + j) * nsb + sb
        let sv = ksc[blk]
        let si2 = js * 2u + ile
        let scw = si2 < 8u ? (si2 < 4u ? sv.x : sv.y) : (si2 < 12u ? sv.z : sv.w)
        let dsc = float(kdh[blk]) * float((scw >> ((si2 & 3u) * 8u)) & 255u)
        let qw2 = kqu[blk * 18u + js]
        let sgw2 = kqu[blk * 18u + 8u + js]
        let qh2 = (kqu[blk * 18u + 16u + js / 4u] >> ((js % 4u) * 8u)) & 255u
        let tb0 = tbase + j * 72u + (sub >> 1u) * 32u + kh
        for [unroll_full] (k in range(4)) {
            let l2 = ile * 2u + uint(k) / 2u
            let idx2 = (((qw2 >> (8u * l2)) & 255u) | ((qh2 << (8u - 2u * l2)) & 0x300u)) * 2u + uint(k) % 2u
            let sgb2 = (sgw2 >> (8u * l2)) & 255u
            let nib = uint(k) % 2u == 0u ? sgb2 & 15u : sgb2 >> 4u
            let sw = iq3s_signed(iq2s_gw(int(idx2)), nib)
            for [unroll_full] (c in range(4)) {
                twb[tb0 + uint(k) * 4u + uint(c)] = float16(dsc * iq3s_sx(sw, uint(c)))
            }
        }
    }
}

[metal_dispatch(stamp = "kq_mm:iq2s:t", pso = "g_pf_pso_kq_mm23_t")]
class MetalKqMulMmIq2sT : MetalKqMulMmIq2sTensorT {
    typedef XT = float
}

[metal_dispatch(stamp = "kq_mm:iq2s:th", pso = "g_pf_pso_kq_mm23_th")]
class MetalKqMulMmIq2sTH : MetalKqMulMmIq2sTensorT {
    typedef XT = float16
}

[ |> template_struct_instance]
class template MetalMoeMulMmIq2sSplitT : MetalKqMulMmIq2sTensorT {
    override MOE = true

    [metal_kernel(float_a_ok=true)]
    def metal_moe_mulmm_iq2s_t {
        moe_kernel()
    }
}

[metal_dispatch(stamp = "moe_mm_split:iq2s:t")]
class MetalMoeMulMmIq2sT : MetalMoeMulMmIq2sSplitT {
    typedef XT = float
}

[metal_dispatch(stamp = "moe_mm_split:iq2s:th")]
class MetalMoeMulMmIq2sTH : MetalMoeMulMmIq2sSplitT {
    typedef XT = float16
}

[metal_dispatch(stamp = "moe_mm_split:iq2s:th128")]
class MetalMoeMulMmIq2sTH128 : MetalMoeMulMmIq2sSplitT {
    typedef XT = float16
    override MT = 128u
}

[metal_dispatch(stamp = "moe_mm_split:iq2s:thr")]
class MetalMoeMulMmIq2sTHR : MetalMoeMulMmIq2sSplitT {
    typedef XT = float16
    override REM = true
}

[ |> template_struct_instance]
class template MetalKqMulMmIq2xsTensorT : MetalKqMulMmSplitTensorBase {
    [metal_kernel(float_a_ok=true)]
    def metal_kq_mulmm_iq2xs_t {
        run_shell()
    }

    def override stage16(work, kb, nBase, nsb, tbase : uint) {
        let j = work >> 2u
        let sub = work & 3u
        let jsl = kb + (sub >> 1u)
        let sb = jsl / 8u
        let js = jsl % 8u
        let kh = (sub & 1u) * 16u
        let ile = sub & 1u
        let blk = (nBase + j) * nsb + sb
        let sv = ksc[blk]
        let si2 = js * 2u + ile
        let scw = si2 < 8u ? (si2 < 4u ? sv.x : sv.y) : (si2 < 12u ? sv.z : sv.w)
        let dsc = float(kdh[blk]) * float((scw >> ((si2 & 3u) * 8u)) & 255u)
        let qw2 = kqu[blk * 16u + js * 2u + ile]
        let tb0 = tbase + j * 72u + (sub >> 1u) * 32u + kh
        for [unroll_full] (k in range(4)) {
            let w16 = (qw2 >> (16u * (uint(k) / 2u))) & 0xFFFFu
            let idx2 = (w16 & 511u) * 2u + uint(k) % 2u
            let sgb2 = ksign7(w16 >> 9u)
            let nib = uint(k) % 2u == 0u ? sgb2 & 15u : sgb2 >> 4u
            let sw = iq3s_signed(iq2xs_gw(int(idx2)), nib)
            for [unroll_full] (c in range(4)) {
                twb[tb0 + uint(k) * 4u + uint(c)] = float16(dsc * iq3s_sx(sw, uint(c)))
            }
        }
    }
}

[metal_dispatch(stamp = "kq_mm:iq2xs:t", pso = "g_pf_pso_kq_mm24_t")]
class MetalKqMulMmIq2xsT : MetalKqMulMmIq2xsTensorT {
    typedef XT = float
}

[metal_dispatch(stamp = "kq_mm:iq2xs:th", pso = "g_pf_pso_kq_mm24_th")]
class MetalKqMulMmIq2xsTH : MetalKqMulMmIq2xsTensorT {
    typedef XT = float16
}

[ |> template_struct_instance]
class template MetalMoeMulMmIq2xsSplitT : MetalKqMulMmIq2xsTensorT {
    override MOE = true

    [metal_kernel(float_a_ok=true)]
    def metal_moe_mulmm_iq2xs_t {
        moe_kernel()
    }
}

[metal_dispatch(stamp = "moe_mm_split:iq2xs:t")]
class MetalMoeMulMmIq2xsT : MetalMoeMulMmIq2xsSplitT {
    typedef XT = float
}

[metal_dispatch(stamp = "moe_mm_split:iq2xs:th")]
class MetalMoeMulMmIq2xsTH : MetalMoeMulMmIq2xsSplitT {
    typedef XT = float16
}

[metal_dispatch(stamp = "moe_mm_split:iq2xs:th128")]
class MetalMoeMulMmIq2xsTH128 : MetalMoeMulMmIq2xsSplitT {
    typedef XT = float16
    override MT = 128u
}

[metal_dispatch(stamp = "moe_mm_split:iq2xs:thr")]
class MetalMoeMulMmIq2xsTHR : MetalMoeMulMmIq2xsSplitT {
    typedef XT = float16
    override REM = true
}

[ |> template_struct_instance]
class template MetalKqMulMmIq2xxsTensorT : MetalKqMulMmSplitTensorBase {
    [metal_kernel(float_a_ok=true)]
    def metal_kq_mulmm_iq2xxs_t {
        run_shell()
    }

    def override stage16(work, kb, nBase, nsb, tbase : uint) {
        let j = work >> 2u
        let sub = work & 3u
        let jsl = kb + (sub >> 1u)
        let sb = jsl / 8u
        let js = jsl % 8u
        let kh = (sub & 1u) * 16u
        let ile = sub & 1u
        let blk = (nBase + j) * nsb + sb
        let sv = ksc[blk]
        let scw = js < 4u ? sv.x : sv.y
        let dsc = float(kdh[blk]) * float((scw >> ((js & 3u) * 8u)) & 255u)
        let gw2 = kqu[blk * 16u + js * 2u]
        let aux2 = kqu[blk * 16u + js * 2u + 1u]
        let tb0 = tbase + j * 72u + (sub >> 1u) * 32u + kh
        for [unroll_full] (k in range(4)) {
            let q3 = ile * 2u + uint(k) / 2u
            let bidx = (gw2 >> (8u * q3)) & 255u
            let sgb2 = ksign7((aux2 >> (7u * q3)) & 127u)
            let nib = uint(k) % 2u == 0u ? sgb2 & 15u : sgb2 >> 4u
            let sw = iq3s_signed(iq2xxs_gw(int(bidx * 2u + uint(k) % 2u)), nib)
            for [unroll_full] (c in range(4)) {
                twb[tb0 + uint(k) * 4u + uint(c)] = float16(dsc * iq3s_sx(sw, uint(c)))
            }
        }
    }
}

[metal_dispatch(stamp = "kq_mm:iq2xxs:t", pso = "g_pf_pso_kq_mm25_t")]
class MetalKqMulMmIq2xxsT : MetalKqMulMmIq2xxsTensorT {
    typedef XT = float
}

[metal_dispatch(stamp = "kq_mm:iq2xxs:th", pso = "g_pf_pso_kq_mm25_th")]
class MetalKqMulMmIq2xxsTH : MetalKqMulMmIq2xxsTensorT {
    typedef XT = float16
}

[ |> template_struct_instance]
class template MetalMoeMulMmIq2xxsSplitT : MetalKqMulMmIq2xxsTensorT {
    override MOE = true

    [metal_kernel(float_a_ok=true)]
    def metal_moe_mulmm_iq2xxs_t {
        moe_kernel()
    }
}

[metal_dispatch(stamp = "moe_mm_split:iq2xxs:t")]
class MetalMoeMulMmIq2xxsT : MetalMoeMulMmIq2xxsSplitT {
    typedef XT = float
}

[metal_dispatch(stamp = "moe_mm_split:iq2xxs:th")]
class MetalMoeMulMmIq2xxsTH : MetalMoeMulMmIq2xxsSplitT {
    typedef XT = float16
}

[metal_dispatch(stamp = "moe_mm_split:iq2xxs:th128")]
class MetalMoeMulMmIq2xxsTH128 : MetalMoeMulMmIq2xxsSplitT {
    typedef XT = float16
    override MT = 128u
}

[metal_dispatch(stamp = "moe_mm_split:iq2xxs:thr")]
class MetalMoeMulMmIq2xxsTHR : MetalMoeMulMmIq2xxsSplitT {
    typedef XT = float16
    override REM = true
}

[ |> template_struct_instance]
class template MetalKqDequantIq4xsT : MetalKqMulMmIq4xsTensorT {
    typedef XT = float16
    override DEQW = true

    [metal_kernel]
    def metal_kq_dequant_iq4xs {
        dequant_shell()
    }
}

[metal_dispatch(name = "enc_kq_dq_iq4xs", pso = "g_pf_pso_dq_kq44", kernel = "metal_kq_dequant_iq4xs", tgmem = "MetalKqDequantIq4xs_metal_kq_dequant_iq4xs_msl_tgmem", tg = 128, grid = "kdim/64, d/64", params = "kdim : int64, d : int64")]
class MetalKqDequantIq4xs : MetalKqDequantIq4xsT {}

[metal_dispatch(name = "enc_kq_dq_iq4nl", pso = "g_pf_pso_dq_kq45", kernel = "metal_kq_dequant_iq4xs", tgmem = "MetalKqDequantIq4nl_metal_kq_dequant_iq4xs_msl_tgmem", tg = 128, grid = "kdim/64, d/64", params = "kdim : int64, d : int64")]
class MetalKqDequantIq4nl : MetalKqDequantIq4xsT {
    override IQ4NL = true
}

[metal_dispatch(name = "enc_kq_dq_q40", pso = "g_pf_pso_dq_kq40", kernel = "metal_kq_dequant_iq4xs", tgmem = "MetalKqDequantQ40_metal_kq_dequant_iq4xs_msl_tgmem", tg = 128, grid = "kdim/64, d/64", params = "kdim : int64, d : int64")]
class MetalKqDequantQ40 : MetalKqDequantIq4xsT {
    override IQ4NL = true
    override Q40 = true
}

[metal_dispatch(name = "enc_kq_dq_k3", pso = "g_pf_pso_dq_kq3", kernel = "metal_kq_dequant_k3", tgmem = "MetalKqDequantK3_metal_kq_dequant_k3_msl_tgmem", tg = 128, grid = "kdim/64, d/64", params = "kdim : int64, d : int64")]
class MetalKqDequantK3 : MetalKqMulMmK3TensorT {
    typedef XT = float16
    override DEQW = true

    [metal_kernel]
    def metal_kq_dequant_k3 {
        dequant_shell()
    }
}

[metal_dispatch(name = "enc_kq_dq_iq3s", pso = "g_pf_pso_dq_kq33", kernel = "metal_kq_dequant_iq3s", tgmem = "MetalKqDequantIq3s_metal_kq_dequant_iq3s_msl_tgmem", tg = 128, grid = "kdim/64, d/64", params = "kdim : int64, d : int64")]
class MetalKqDequantIq3s : MetalKqMulMmIq3sTensorT {
    typedef XT = float16
    override DEQW = true

    [metal_kernel]
    def metal_kq_dequant_iq3s {
        dequant_shell()
    }
}

[metal_dispatch(name = "enc_kq_dq_iq3xxs", pso = "g_pf_pso_dq_kq34", kernel = "metal_kq_dequant_iq3xxs", tgmem = "MetalKqDequantIq3xxs_metal_kq_dequant_iq3xxs_msl_tgmem", tg = 128, grid = "kdim/64, d/64", params = "kdim : int64, d : int64")]
class MetalKqDequantIq3xxs : MetalKqMulMmIq3xxsTensorT {
    typedef XT = float16
    override DEQW = true

    [metal_kernel]
    def metal_kq_dequant_iq3xxs {
        dequant_shell()
    }
}

[metal_dispatch(name = "enc_kq_dq_k2", pso = "g_pf_pso_dq_kq2", kernel = "metal_kq_dequant_k2", tgmem = "MetalKqDequantK2_metal_kq_dequant_k2_msl_tgmem", tg = 128, grid = "kdim/64, d/64", params = "kdim : int64, d : int64")]
class MetalKqDequantK2 : MetalKqMulMmK2TensorT {
    typedef XT = float16
    override DEQW = true

    [metal_kernel]
    def metal_kq_dequant_k2 {
        dequant_shell()
    }
}

[metal_dispatch(name = "enc_kq_dq_iq2s", pso = "g_pf_pso_dq_kq23", kernel = "metal_kq_dequant_iq2s", tgmem = "MetalKqDequantIq2s_metal_kq_dequant_iq2s_msl_tgmem", tg = 128, grid = "kdim/64, d/64", params = "kdim : int64, d : int64")]
class MetalKqDequantIq2s : MetalKqMulMmIq2sTensorT {
    typedef XT = float16
    override DEQW = true

    [metal_kernel]
    def metal_kq_dequant_iq2s {
        dequant_shell()
    }
}

[metal_dispatch(name = "enc_kq_dq_iq2xs", pso = "g_pf_pso_dq_kq24", kernel = "metal_kq_dequant_iq2xs", tgmem = "MetalKqDequantIq2xs_metal_kq_dequant_iq2xs_msl_tgmem", tg = 128, grid = "kdim/64, d/64", params = "kdim : int64, d : int64")]
class MetalKqDequantIq2xs : MetalKqMulMmIq2xsTensorT {
    typedef XT = float16
    override DEQW = true

    [metal_kernel]
    def metal_kq_dequant_iq2xs {
        dequant_shell()
    }
}

[metal_dispatch(name = "enc_kq_dq_iq2xxs", pso = "g_pf_pso_dq_kq25", kernel = "metal_kq_dequant_iq2xxs", tgmem = "MetalKqDequantIq2xxs_metal_kq_dequant_iq2xxs_msl_tgmem", tg = 128, grid = "kdim/64, d/64", params = "kdim : int64, d : int64")]
class MetalKqDequantIq2xxs : MetalKqMulMmIq2xxsTensorT {
    typedef XT = float16
    override DEQW = true

    [metal_kernel]
    def metal_kq_dequant_iq2xxs {
        dequant_shell()
    }
}



struct MoeMmArgs {
    kdim : uint
    ndim : uint
    eplane : uint       //!< per-expert plane stride, in the format's own block unit (32 or 256 elems)
    nk : uint
    gather : uint       //!< 1 = bkt-indirect X rows (gate/up), 0 = contiguous bucket rows (down)
    hasb : uint
}

def private moe_mm_ka(fmt : KqFmt; kdim, ndim, nk : int64; gather : bool) : MoeMmArgs => (
    MoeMmArgs(kdim = uint(kdim), ndim = uint(ndim),
        eplane = uint(kdim * ndim / (kq_sb(fmt) ? 256l : 32l)),
        nk = uint(nk), gather = gather ? 1u : 0u))

def private moe_mm_mx4_ka(kdim, ndim, nk : int64; gather, hasb : bool) : MoeMmArgs => (
    MoeMmArgs(kdim = uint(kdim), ndim = uint(ndim), eplane = uint(kdim * ndim / 32l),
        nk = uint(nk), gather = gather ? 1u : 0u, hasb = hasb ? 1u : 0u))

struct MoeGxArgs {
    dim : uint
    nk : uint
    ne : uint
    total : uint    //!< npos*nk: a bkt word >= it marks a pad or stale row, and the gather zeroes that output row
}

[metal_dispatch(name = "enc_moe_gather_x", pso = "g_pf_pso_moe_gx", tg = 256, grid = "rows", params = "rows : int64")]
class MetalMoeGatherX {
    @ssbo @binding = 0 @role = "read" bkt : array<uint>
    @ssbo @binding = 1 @role = "read" xf : array<float>
    @ssbo @binding = 2 @role = "write" xg : array<float16>
    @uniform @binding = 3 ka : MoeGxArgs

    [metal_kernel(name="metal_moe_gather_x_msl")]
    def metal_moe_gather_x {
        let r = gl_WorkGroupID.x
        let v = bkt[r]
        let isreal = v < ka.total
        let token = isreal ? v / ka.nk : 0u
        var i = gl_LocalInvocationID.x
        while (i < ka.dim) {
            xg[r * ka.dim + i] = isreal ? float16(xf[token * ka.dim + i]) : float16(0.0)
            i += 256u
        }
    }
}

[ |> template_struct_instance]
class template MetalMoeMulMmKqTensorBase {
    @template_constant MT : uint = 32u
    @template_constant REM : bool = false
    @template_constant STAGE_ITEMS : uint = 256u     //! per staged chunk
    @template_constant STAGE_K : uint = 64u     //! staged chunk depth, in k elements
    @ssbo @binding = 3 xf : array<XT>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ka : MoeMmArgs
    @ssbo @binding = 6 cnt : array<uint>
    @ssbo @binding = 7 basep : array<uint>

    def abstract stage_block(work, kb, e, nBase : uint) : void

    def store_out(var acc : float[8192]; var cp : float?; e, nBase : uint) {
        tmm2d_tg_store(acc, cp, MT, 64u, ka.ndim)
    }

    def run_shell {
        let e = gl_WorkGroupID.z
        let ce = cnt[e]
        let mBase = gl_WorkGroupID.x * MT + (REM ? ce / 128u * 128u : 0u)
        let lim = MT == 128u ? ce / 128u * 128u : (ce + 31u) / 32u * 32u
        if (mBase >= lim) {
            return
        }
        let rbase = basep[e]
        let nBase = gl_WorkGroupID.y * 64u
        let nkb = ka.kdim / 32u
        let lid = gl_LocalInvocationID.x
        var acc : float[8192]
        var cp = unsafe(addr(y[(rbase + mBase) * ka.ndim + nBase]))
        tmm2d_tg_begin_deva(acc, unsafe(addr(xf[0])), MT, 64u, 4u, STAGE_K)
        var kb = 0u
        while (kb < nkb) {
            var work = lid
            while (work < STAGE_ITEMS) {
                stage_block(work, kb, e, nBase)
                work += gl_WorkGroupSize.x
            }
            barrier()
            tmm2d_tg_step_deva(acc, unsafe(addr(xf[(rbase + mBase) * ka.kdim + kb * 32u])), ka.kdim, twb, MT, 64u, STAGE_K)
            barrier()
            kb += STAGE_K / 32u
        }
        store_out(acc, cp, e, nBase)
    }
}

[ |> template_struct_instance]
class template MetalMoeMulMmMx4TensorT : MetalMoeMulMmKqTensorBase {
    @ssbo @binding = 0 @role = "weight" @off = "qoff" mxq4 : array<uint4>  //!< nibble plane, uint4 view
    @ssbo @binding = 1 @role = "weight" @off = "soff" mxe : array<uint8>   //!< e8m0 scale plane
    @ssbo @binding = 12 @role = "weight" wb : array<float>   //!< [ne x ndim] per-expert bias rows
    @ssbo @binding = 13 @role = "weight" vtab : array<float> //!< [16] signed doubled-e2m1 values
    @workgroup twb : float16[2048]          //!< W chunk: 64 wrows x 32 k, flat row-major
    override STAGE_ITEMS = 128u
    override STAGE_K = 32u

    [metal_kernel(float_a_ok=true)]
    def metal_moe_mulmm_mx4_t {
        run_shell()
    }

    def override stage_block(work, kb, e, nBase : uint) {
        let nkb = ka.kdim / 32u
        let j = work >> 1u
        let kh = (work & 1u) * 16u
        let blk = e * ka.eplane + (nBase + j) * nkb + kb
        let sc = uint(mxe[blk])
        let d = uint_bits_to_float(sc < 2u ? (0x00200000u << sc) : ((sc - 1u) << 23u))
        let u4 = mxq4[blk]
        for [unroll_full] (t in range(16)) {
            let byi = uint(t)
            let w32 = byi < 4u ? u4.x : (byi < 8u ? u4.y : (byi < 12u ? u4.z : u4.w))
            let nby = (w32 >> ((byi % 4u) * 8u)) & 255u
            twb[j * 32u + kh + byi] = float16(vtab[int(kh == 0u ? (nby & 15u) : (nby >> 4u))] * d)
        }
    }

    def override store_out(var acc : float[8192]; var cp : float?; e, nBase : uint) {
        if (ka.hasb != 0u) {
            var bp = unsafe(addr(wb[e * ka.ndim + nBase]))
            tmm2d_tg_store_bias(acc, cp, bp, MT, 64u, ka.ndim)
        } else {
            tmm2d_tg_store(acc, cp, MT, 64u, ka.ndim)
        }
    }
}

[metal_dispatch(stamp = "moe_mm:mx4:t")]
class MetalMoeMulMmMx4T : MetalMoeMulMmMx4TensorT {
    typedef XT = float
}

[metal_dispatch(stamp = "moe_mm:mx4:th")]
class MetalMoeMulMmMx4TH : MetalMoeMulMmMx4TensorT {
    typedef XT = float16
}

[metal_dispatch(stamp = "moe_mm:mx4:th128")]
class MetalMoeMulMmMx4TH128 : MetalMoeMulMmMx4TensorT {
    typedef XT = float16
    override MT = 128u
}

[metal_dispatch(stamp = "moe_mm:mx4:thr")]
class MetalMoeMulMmMx4THR : MetalMoeMulMmMx4TensorT {
    typedef XT = float16
    override REM = true
}

//! The prefill stamp over the padded bucket panel, with the PF-gated liveness exit over slots 3-6.
//! Its total/256 grid covers the callers' total % 4 == 0 contract; the kernel guards gid * 4 >= total.
[metal_dispatch(name = "pf_enc_swiglu_oai", pso = "g_pf_pso_swiglu_oai", tg = 64, grid = "total/256", params = "total : int64")]
class MetalSwigluOaiPf : MetalSwigluOaiT {
    override PF = true
}


[metal_dispatch(name = "pf_enc_moe_count", pso = "g_pf_pso_moe_count", tg = 32, grid = "ne", params = "ne : int64")]
class MetalMoeCount {
    @ssbo @binding = 0 @role = "read" seli : array<uint>   //!< [npos x 2k] selection rows (idx section)
    @ssbo @binding = 1 @role = "write" cnt : array<uint>    //!< [ne] out
    @uniform @binding = 2 total : uint      //!< npos * k entries
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
    @ssbo @binding = 1 @role = "write" basep : array<uint>  //!< [ne] exclusive prefix over the PADDED counts
    @ssbo @binding = 2 @role = "write" bkt : array<uint>    //!< [padded rows] bucket -> entry (row*k + slot)
    @ssbo @binding = 3 @role = "write" inv : array<uint>    //!< [npos*k] entry -> padded bucket row
    @uniform @binding = 4 total : uint
    @uniform @binding = 5 nk : uint
    @ssbo @binding = 6 @role = "read" cnt : array<uint>
    @uniform @binding = 7 ne : uint

    [metal_kernel(name="metal_moe_bucket_msl")]
    def metal_moe_bucket {
        let e = gl_WorkGroupID.x
        let lane = gl_SubgroupInvocationID
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
        if (e == ne) {
            return
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
        let ce = cnt[e]
        var pr = ce + lane
        while (pr < (ce + 31u) / 32u * 32u) {
            bkt[pacc + pr] = 0xFFFFFFFF
            pr += 32u
        }
    }
}

[metal_dispatch(name = "pf_enc_moe_reduce", pso = "g_pf_pso_moe_reduce", tg = 256, grid = "dim/256, npos", params = "dim : int64, npos : int64")]
class MetalMoeReduce : MetalMoeCombineT {
    @ssbo @binding = 3 @role = "write" y : array<float>     //!< [npos x dim]
    @uniform @binding = 4 dim : uint
    @uniform @binding = 5 nk : uint
    override GATHERED = true

    [metal_kernel(name="metal_moe_reduce_msl")]
    def metal_moe_reduce {
        combine_body()
    }
}

[ |> template_struct_instance]
class template MetalMoeMulMmQ8TensorT {
    @template_constant MT : uint = 32u
    @template_constant REM : bool = false
    @ssbo @binding = 0 @role = "weight" @off = "wboff" wsh : array<float16> //!< expert STACK blob, half-scale view
    @ssbo @binding = 1 @role = "weight" @off = "wboff" wqb : array<int8>    //!< the same blob buffer, byte view
    @ssbo @binding = 2 xf : array<XT>
    @ssbo @binding = 3 y : array<float>
    @uniform @binding = 5 ka : MoeMmArgs
    @ssbo @binding = 6 cnt : array<uint>
    @ssbo @binding = 7 basep : array<uint>
    @workgroup twb : float16[6144]          //!< W chunk: 64 wcols x 72-half padded rows (bk=64; 12288B lands mid-plateau)

    [metal_kernel(float_a_ok=true)]
    def metal_moe_mulmm_q8_t {
        let e = gl_WorkGroupID.z
        let ce = cnt[e]
        let mBase = gl_WorkGroupID.x * MT + (REM ? ce / 128u * 128u : 0u)
        let lim = MT == 128u ? ce / 128u * 128u : (ce + 31u) / 32u * 32u
        if (mBase >= lim) {
            return
        }
        let rbase = basep[e]
        let nBase = gl_WorkGroupID.y * 64u
        let blk0 = e * ka.eplane + nBase * (ka.kdim / 32u)
        var sp = unsafe(addr(wsh[blk0 * 17u]))
        var wp = unsafe(addr(wqb[blk0 * 34u + 2u]))
        var ap = unsafe(addr(xf[(rbase + mBase) * ka.kdim]))
        var cp = unsafe(addr(y[(rbase + mBase) * ka.ndim + nBase]))
        tmm2d_q8u_f32(MT, 64u, 4u, twb, sp, wp, ap, ka.kdim, cp, ka.ndim, ka.kdim, ka.kdim / 32u, gl_LocalInvocationIndex, 64u)
    }
}

[metal_dispatch(stamp = "moe_mm:q8:t")]
class MetalMoeMulMmQ8T : MetalMoeMulMmQ8TensorT {
    typedef XT = float
}

[metal_dispatch(stamp = "moe_mm:q8:th")]
class MetalMoeMulMmQ8TH : MetalMoeMulMmQ8TensorT {
    typedef XT = float16
}

[metal_dispatch(stamp = "moe_mm:q8:th128")]
class MetalMoeMulMmQ8TH128 : MetalMoeMulMmQ8TensorT {
    typedef XT = float16
    override MT = 128u
}

[metal_dispatch(stamp = "moe_mm:q8:thr")]
class MetalMoeMulMmQ8THR : MetalMoeMulMmQ8TensorT {
    typedef XT = float16
    override REM = true
}

[ |> template_struct_instance]
class template MetalMoeMulMmK45TensorT : MetalMoeMulMmKqTensorBase {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" ksh : array<float16>
    @ssbo @binding = 1 @role = "weight" @off = "soff" ks4 : array<uint4>
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>
    @workgroup twb : float16[6144]
    @template_constant SB_UINTS : uint = 32u
    @template_constant QH : bool = false

    [metal_kernel(float_a_ok=true)]
    def metal_moe_mulmm_k45_t {
        run_shell()
    }

    [arch(at="../ARCHITECTURE_GPU_PREFILL_WINDOW.md#kernel-load-addressing")]
    def override stage_block(work, kb, e, nBase : uint) {
        let nsb = ka.kdim / 256u
        let j = work >> 2u
        let sub = work & 3u
        let jsl = kb + (sub >> 1u)
        let sb = jsl / 8u
        let js = jsl % 8u
        let kh = (sub & 1u) * 16u
        let blk = e * ka.eplane + (nBase + j) * nsb + sb
        let sv = ks4[blk]
        var sc = 0u
        var mn = 0u
        kmask_scmn(sv, js, sc, mn)
        let dsc = float(ksh[blk * 8u]) * float(sc)
        let dmn = float(ksh[blk * 8u + 1u]) * float(mn)
        let qp = unsafe(addr(kqu[blk * SB_UINTS + (js / 2u) * 8u + kh / 4u]))
        let hp = unsafe(addr(kqu[blk * SB_UINTS + 32u + kh / 4u]))
        let tb = j * 72u + (sub >> 1u) * 32u + kh
        let sh = (js & 1u) * 4u
        for [unroll_full] (w in range(4)) {
            let u = unsafe(qp[w]) >> sh
            static_if (QH) {
                let hu = unsafe(hp[w]) >> js
                for [unroll_full] (b in range(4)) {
                    let q = ((u >> uint(8 * b)) & 15u) | (((hu >> uint(8 * b)) & 1u) << 4u)
                    twb[tb + uint(w * 4 + b)] = float16(dsc * float(q) - dmn)
                }
            } else {
                for [unroll_full] (b in range(4)) {
                    twb[tb + uint(w * 4 + b)] = float16(dsc * float((u >> uint(8 * b)) & 15u) - dmn)
                }
            }
        }
    }
}

[metal_dispatch(stamp = "moe_mm:k4:t")]
class MetalMoeMulMmK4T : MetalMoeMulMmK45TensorT {
    typedef XT = float
}

[metal_dispatch(stamp = "moe_mm:k4:th")]
class MetalMoeMulMmK4TH : MetalMoeMulMmK45TensorT {
    typedef XT = float16
}

[metal_dispatch(stamp = "moe_mm:k4:th128")]
class MetalMoeMulMmK4TH128 : MetalMoeMulMmK45TensorT {
    typedef XT = float16
    override MT = 128u
}

[metal_dispatch(stamp = "moe_mm:k4:thr")]
class MetalMoeMulMmK4THR : MetalMoeMulMmK45TensorT {
    typedef XT = float16
    override REM = true
}

[metal_dispatch(stamp = "moe_mm:k5:t")]
class MetalMoeMulMmK5T : MetalMoeMulMmK45TensorT {
    typedef XT = float
    override SB_UINTS = 40u
    override QH = true
}

[metal_dispatch(stamp = "moe_mm:k5:th")]
class MetalMoeMulMmK5TH : MetalMoeMulMmK45TensorT {
    typedef XT = float16
    override SB_UINTS = 40u
    override QH = true
}

[metal_dispatch(stamp = "moe_mm:k5:th128")]
class MetalMoeMulMmK5TH128 : MetalMoeMulMmK45TensorT {
    typedef XT = float16
    override SB_UINTS = 40u
    override QH = true
    override MT = 128u
}

[metal_dispatch(stamp = "moe_mm:k5:thr")]
class MetalMoeMulMmK5THR : MetalMoeMulMmK45TensorT {
    typedef XT = float16
    override SB_UINTS = 40u
    override QH = true
    override REM = true
}

[ |> template_struct_instance]
class template MetalMoeMulMmQ51TensorT : MetalMoeMulMmKqTensorBase {
    @ssbo @binding = 0 @role = "weight" @off = "soff" wsh : array<float16>
    @ssbo @binding = 1 @role = "weight" @off = "qoff" wqu : array<uint>
    @workgroup twb : float16[6144]

    [metal_kernel(float_a_ok=true)]
    def metal_moe_mulmm_q51_t {
        run_shell()
    }

    def override stage_block(work, kb, e, nBase : uint) {
        let nkb = ka.kdim / 32u
        let j = work >> 2u
        let sub = work & 3u
        let jsl = kb + (sub >> 1u)
        let kh = (sub & 1u) * 16u
        let blk = e * ka.eplane + (nBase + j) * nkb + jsl
        let d = float(wsh[blk * 2u])
        let m = float(wsh[blk * 2u + 1u])
        let qp = unsafe(addr(wqu[blk * 5u]))
        let qh = unsafe(qp[4])
        let nsh = (sub & 1u) * 4u
        for [unroll_full] (t in range(16)) {
            let el = kh + uint(t)
            let u = unsafe(qp[t / 4])
            let q = ((u >> (8u * (uint(t) % 4u) + nsh)) & 15u) | (((qh >> el) & 1u) << 4u)
            twb[j * 72u + (sub >> 1u) * 32u + el] = float16(d * float(q) + m)
        }
    }
}

[metal_dispatch(stamp = "moe_mm:q51:t")]
class MetalMoeMulMmQ51T : MetalMoeMulMmQ51TensorT {
    typedef XT = float
}

[metal_dispatch(stamp = "moe_mm:q51:th")]
class MetalMoeMulMmQ51TH : MetalMoeMulMmQ51TensorT {
    typedef XT = float16
}

[metal_dispatch(stamp = "moe_mm:q51:th128")]
class MetalMoeMulMmQ51TH128 : MetalMoeMulMmQ51TensorT {
    typedef XT = float16
    override MT = 128u
}

[metal_dispatch(stamp = "moe_mm:q51:thr")]
class MetalMoeMulMmQ51THR : MetalMoeMulMmQ51TensorT {
    typedef XT = float16
    override REM = true
}

//! the iq4nl32 tensor twins: the q51 staging over 16B/2B planes, the value d*LUT[q] (the codebook
//! selected in ALU - iq4_lut - since the stamp stages 16 values a work item)
[ |> template_struct_instance]
class template MetalMoeMulMmIq4nl32TensorT : MetalMoeMulMmKqTensorBase {
    @ssbo @binding = 0 @role = "weight" @off = "soff" wsh : array<float16>
    @ssbo @binding = 1 @role = "weight" @off = "qoff" wqu : array<uint>
    @workgroup twb : float16[6144]

    [metal_kernel(float_a_ok=true)]
    def metal_moe_mulmm_iq4nl32_t {
        run_shell()
    }

    def override stage_block(work, kb, e, nBase : uint) {
        let nkb = ka.kdim / 32u
        let j = work >> 2u
        let sub = work & 3u
        let jsl = kb + (sub >> 1u)
        let kh = (sub & 1u) * 16u
        let blk = e * ka.eplane + (nBase + j) * nkb + jsl
        let d = float(wsh[blk])
        let qp = unsafe(addr(wqu[blk * 4u]))
        let nsh = (sub & 1u) * 4u
        for [unroll_full] (t in range(16)) {
            let u = unsafe(qp[t / 4])
            let q = (u >> (8u * (uint(t) % 4u) + nsh)) & 15u
            twb[j * 72u + (sub >> 1u) * 32u + kh + uint(t)] = float16(d * iq4_lut(q))
        }
    }
}

[metal_dispatch(stamp = "moe_mm:iq4nl32:t")]
class MetalMoeMulMmIq4nl32T : MetalMoeMulMmIq4nl32TensorT {
    typedef XT = float
}

[metal_dispatch(stamp = "moe_mm:iq4nl32:th")]
class MetalMoeMulMmIq4nl32TH : MetalMoeMulMmIq4nl32TensorT {
    typedef XT = float16
}

[metal_dispatch(stamp = "moe_mm:iq4nl32:th128")]
class MetalMoeMulMmIq4nl32TH128 : MetalMoeMulMmIq4nl32TensorT {
    typedef XT = float16
    override MT = 128u
}

[metal_dispatch(stamp = "moe_mm:iq4nl32:thr")]
class MetalMoeMulMmIq4nl32THR : MetalMoeMulMmIq4nl32TensorT {
    typedef XT = float16
    override REM = true
}

[ |> template_struct_instance]
class template MetalMoeMulMmK6TensorT : MetalMoeMulMmKqTensorBase {
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16>
    @ssbo @binding = 1 @role = "weight" @off = "soff" ksc : array<uint4>
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>
    @workgroup twb : float16[6144]

    [metal_kernel(float_a_ok=true)]
    def metal_moe_mulmm_k6_t {
        run_shell()
    }

    def override stage_block(work, kb, e, nBase : uint) {
        let nsb = ka.kdim / 256u
        let j = work >> 2u
        let sub = work & 3u
        let jsl = kb + (sub >> 1u)
        let sb = jsl / 8u
        let js = jsl % 8u
        let half6 = js / 4u
        let gg = js % 4u
        let nsh = (gg / 2u) * 4u
        let hsh = gg * 2u
        let kh = (sub & 1u) * 16u
        let ile = kh / 16u
        let blk = e * ka.eplane + (nBase + j) * nsb + sb
        let dall = float(kdh[blk])
        let si = half6 * 8u + gg * 2u + ile
        let sv = ksc[blk]
        let scw = si < 8u ? (si < 4u ? sv.x : sv.y) : (si < 12u ? sv.z : sv.w)
        let s6 = float(((int(scw >> ((si & 3u) * 8u)) & 255) ^ 128) - 128)
        let dsc = dall * s6
        for [unroll_full] (t in range(16)) {
            let k = uint(t) / 4u
            let c = uint(t) % 4u
            let u = kqu[blk * 48u + half6 * 16u + (gg & 1u) * 8u + ile * 4u + k]
            let hu = kqu[blk * 48u + 32u + half6 * 8u + ile * 4u + k]
            let q = ((u >> (nsh + 8u * c)) & 15u) | (((hu >> (hsh + 8u * c)) & 3u) << 4u)
            twb[j * 72u + (sub >> 1u) * 32u + kh + uint(t)] = float16(dsc * (float(q) - 32.0))
        }
    }
}

[metal_dispatch(stamp = "moe_mm:k6:t")]
class MetalMoeMulMmK6T : MetalMoeMulMmK6TensorT {
    typedef XT = float
}

[metal_dispatch(stamp = "moe_mm:k6:th")]
class MetalMoeMulMmK6TH : MetalMoeMulMmK6TensorT {
    typedef XT = float16
}

[metal_dispatch(stamp = "moe_mm:k6:th128")]
class MetalMoeMulMmK6TH128 : MetalMoeMulMmK6TensorT {
    typedef XT = float16
    override MT = 128u
}

[metal_dispatch(stamp = "moe_mm:k6:thr")]
class MetalMoeMulMmK6THR : MetalMoeMulMmK6TensorT {
    typedef XT = float16
    override REM = true
}

class template MetalMoeMulMmBase : MetalMmTileBase {
    typedef TILET = float16
    typedef SGT = simdgroup_half8x8
    @ssbo @binding = 3 @role = "read" xf : array<float4>
    @ssbo @binding = 4 @role = "write" y : array<float>
    @uniform @binding = 5 ka : MoeMmArgs
    @ssbo @binding = 7 @role = "read" cnt : array<uint>
    @ssbo @binding = 8 @role = "read" basep : array<uint>
    @ssbo @binding = 9 @role = "read" bkt : array<uint>

    //! a format owns bindings 0..2 - its weight views; its plane stride rides ka.eplane
    def sb_unit() : uint => 32u   //! the weight plane's block unit; the K-quants override it to the 256-quant superblock
    def wbase(e, nBase, lr0 : uint) : uint => e * ka.eplane + (nBase + lr0) * (ka.kdim / sb_unit())   //! expert plane origin, in that unit
    def abstract stage_a(wb, kb, il0, tA0 : uint) : void   //! decode one k-block into the A tile

    def stage_init(wb, il0 : uint) {}   //! format prologue: bind carried-pointer state (q8's walk)

    def stage_acc(var acc : simdgroup_float8x8[8]) {}   //! accumulator seed hook (mx4's per-expert bias)

    //! publishes a decoded A half into the shared W tile, fenced against the previous math loop
    def publish_a(var va : float16[16]; tA0 : uint) {
        barrier()
        for [unroll_full] (i in range(16)) {
            ta[tA0 + uint(i / 8) * 512u + uint(i % 8) * 8u] = va[i]
        }
    }

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

[metal_dispatch(name = "pf_enc_moe_mm_q8_c", pso = "g_pf_pso_moe_mm_q8", tgmem = "metal_moe_mulmm_q8_msl_tgmem", tg = 128, grid = "npos/32, rows/64, ne", params = "rows : int64, npos : int64, ne : int64")]
class MetalMoeMulMmQ8 : MetalMoeMulMmBase {
    @ssbo @binding = 0 @role = "weight" @off = "wboff" wsh : array<float16> //!< the expert STACK's blob slice, half-scale view
    @ssbo @binding = 1 @role = "weight" @off = "wboff" wqb : array<int8>    //!< the same blob buffer, byte view
    scur : float16 const?   //!< carried scale pointer
    qp : int8 const?        //!< carried quant pointer

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
        publish_a(va, tA0)
        scur = unsafe(scur + 17)
        qp = unsafe(qp + 34)
    }

    [metal_kernel(name="metal_moe_mulmm_q8_msl")]
    def metal_moe_mulmm_q8 {
        run()
    }
}

[ |> template_struct_instance]
class template MetalMoeMulMmK45T : MetalMoeMulMmBase {
    @ssbo @binding = 0 @role = "weight" @off = "soff" ksh : array<float16>
    @ssbo @binding = 1 @role = "weight" @off = "soff" ks4 : array<uint4>
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>
    @template_constant SB_UINTS : uint = 32u
    @template_constant QH : bool = false

    def override sb_unit() : uint => 256u

    def override stage_a(wb, kb, il0, tA0 : uint) {
        var va : float16[16]
        let sb = kb / 8u
        let js = kb % 8u
        let blk = wb + sb
        let sv = ks4[blk]
        var sc = 0u
        var mn = 0u
        kmask_scmn(sv, js, sc, mn)
        let dsc = float(ksh[blk * 8u]) * float(sc)
        let dmn = float(ksh[blk * 8u + 1u]) * float(mn)
        let qp = unsafe(addr(kqu[blk * SB_UINTS + (js / 2u) * 8u + il0 * 4u]))
        static_if (QH) {
            let hp = unsafe(addr(kqu[blk * SB_UINTS + 32u + il0 * 4u]))
            let nsh = (js & 1u) * 4u
            for [unroll_full] (k in range(4)) {
                let u = unsafe(qp[k])
                let hu = unsafe(hp[k])
                for [unroll_full] (c in range(4)) {
                    let q = ((u >> (8u * uint(c) + nsh)) & 15u) | (((hu >> (8u * uint(c) + js)) & 1u) << 4u)
                    va[k * 4 + c] = float16(dsc * float(q) - dmn)
                }
            }
        } else {
            let nsh = (js & 1u) * 4u
            for [unroll_full] (k in range(4)) {
                let u = unsafe(qp[k])
                for [unroll_full] (c in range(4)) {
                    let q = (u >> (8u * uint(c) + nsh)) & 15u
                    va[k * 4 + c] = float16(dsc * float(q) - dmn)
                }
            }
        }
        publish_a(va, tA0)
    }
}

[metal_dispatch(name = "pf_enc_moe_mm_k4_c", pso = "g_pf_pso_moe_mm_k4", tgmem = "metal_moe_mulmm_k4_msl_tgmem", tg = 128, grid = "npos/32, rows/64, ne", params = "rows : int64, npos : int64, ne : int64")]
class MetalMoeMulMmK4 : MetalMoeMulMmK45T {
    [metal_kernel(name="metal_moe_mulmm_k4_msl")]
    def metal_moe_mulmm_k4 {
        run()
    }
}

[metal_dispatch(name = "pf_enc_moe_mm_k5_c", pso = "g_pf_pso_moe_mm_k5", tgmem = "metal_moe_mulmm_k5_msl_tgmem", tg = 128, grid = "npos/32, rows/64, ne", params = "rows : int64, npos : int64, ne : int64")]
class MetalMoeMulMmK5 : MetalMoeMulMmK45T {
    override SB_UINTS = 40u
    override QH = true

    [metal_kernel(name="metal_moe_mulmm_k5_msl")]
    def metal_moe_mulmm_k5 {
        run()
    }
}

[metal_dispatch(name = "pf_enc_moe_mm_q51_c", pso = "g_pf_pso_moe_mm_q51", tgmem = "metal_moe_mulmm_q51_msl_tgmem", tg = 128, grid = "npos/32, rows/64, ne", params = "rows : int64, npos : int64, ne : int64")]
class MetalMoeMulMmQ51 : MetalMoeMulMmBase {
    @ssbo @binding = 0 @role = "weight" @off = "soff" wsh : array<float16> //!< scale plane (d at 2*blk, m at 2*blk+1)
    @ssbo @binding = 1 @role = "weight" @off = "qoff" wqu : array<uint>    //!< quant plane, uint view (block at 5*blk; qh at +4)

    def override stage_a(wb, kb, il0, tA0 : uint) {
        var va : float16[16]
        let nsh = il0 * 4u
        let hb0 = il0 * 16u
        let blk = wb + kb
        let d = float(wsh[blk * 2u])
        let m = float(wsh[blk * 2u + 1u])
        let qp = unsafe(addr(wqu[blk * 5u]))
        let qh = unsafe(qp[4])
        for [unroll_full] (k in range(4)) {
            let u = unsafe(qp[k])
            for [unroll_full] (c in range(4)) {
                let el = uint(k) * 4u + uint(c)
                let q = ((u >> (8u * uint(c) + nsh)) & 15u) | (((qh >> (el + hb0)) & 1u) << 4u)
                va[k * 4 + c] = float16(d * float(q) + m)
            }
        }
        publish_a(va, tA0)
    }

    [metal_kernel(name="metal_moe_mulmm_q51_msl")]
    def metal_moe_mulmm_q51 {
        run()
    }
}

[metal_dispatch(name = "pf_enc_moe_mm_iq4nl32_c", pso = "g_pf_pso_moe_mm_iq4nl32", tgmem = "metal_moe_mulmm_iq4nl32_msl_tgmem", tg = 128, grid = "npos/32, rows/64, ne", params = "rows : int64, npos : int64, ne : int64")]
class MetalMoeMulMmIq4nl32 : MetalMoeMulMmBase {
    @ssbo @binding = 0 @role = "weight" @off = "soff" wsh : array<float16> //!< d plane (block blk's d at blk)
    @ssbo @binding = 1 @role = "weight" @off = "qoff" wqu : array<uint>    //!< nibble plane, uint view (block at 4*blk)

    def override stage_a(wb, kb, il0, tA0 : uint) {
        var va : float16[16]
        let nsh = il0 * 4u
        let blk = wb + kb
        let d = float(wsh[blk])
        let qp = unsafe(addr(wqu[blk * 4u]))
        for [unroll_full] (k in range(4)) {
            let u = unsafe(qp[k])
            for [unroll_full] (c in range(4)) {
                va[k * 4 + c] = float16(d * iq4_lut((u >> (8u * uint(c) + nsh)) & 15u))
            }
        }
        publish_a(va, tA0)
    }

    [metal_kernel(name="metal_moe_mulmm_iq4nl32_msl")]
    def metal_moe_mulmm_iq4nl32 {
        run()
    }
}

[metal_dispatch(name = "pf_enc_moe_mm_k6_c", pso = "g_pf_pso_moe_mm_k6", tgmem = "metal_moe_mulmm_k6_msl_tgmem", tg = 128, grid = "npos/32, rows/64, ne", params = "rows : int64, npos : int64, ne : int64")]
class MetalMoeMulMmK6 : MetalMoeMulMmBase {
    @ssbo @binding = 0 @role = "weight" @off = "doff" kdh : array<float16>
    @ssbo @binding = 1 @role = "weight" @off = "soff" ksc : array<uint4>
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>

    def override sb_unit() : uint => 256u

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
        let dsc_sh8 = dsc * 0.00390625
        let dsc_sh16 = dsc * 0.0000152587890625
        let dsc_sh24 = dsc * 0.000000059604644775390625
        let qp = unsafe(addr(kqu[blk * 48u + half6 * 16u + (gg & 1u) * 8u + il0 * 4u]))
        let hp = unsafe(addr(kqu[blk * 48u + 32u + half6 * 8u + il0 * 4u]))
        let nsh = (gg / 2u) * 4u
        let hsh = gg * 2u
        for [unroll_full] (k in range(4)) {
            let u = unsafe(qp[k])
            let hu = unsafe(hp[k])
            let qv = ((u >> nsh) & 0x0F0F0F0F) | (((hu >> hsh) & 0x03030303) << 4u)
            va[k * 4] = float16(dsc * float(qv & 255u) - dmn)
            va[k * 4 + 1] = float16(dsc_sh8 * float(qv & 65280u) - dmn)
            va[k * 4 + 2] = float16(dsc_sh16 * float(qv & 16711680u) - dmn)
            va[k * 4 + 3] = float16(dsc_sh24 * float(qv & 4278190080u) - dmn)
        }
        publish_a(va, tA0)
    }

    [metal_kernel(name="metal_moe_mulmm_k6_msl")]
    def metal_moe_mulmm_k6 {
        run()
    }
}

[metal_dispatch(name = "pf_enc_moe_mm_mx4_c", pso = "g_pf_pso_moe_mm_mx4", tgmem = "metal_moe_mulmm_mx4_msl_tgmem", tg = 128, grid = "npos/32, rows/64, ne", params = "rows : int64, npos : int64, ne : int64")]
class MetalMoeMulMmMx4 : MetalMoeMulMmBase {
    @ssbo @binding = 0 @role = "weight" @off = "qoff" mxq4 : array<uint4>  //!< nibble plane, uint4 view (one block = one load)
    @ssbo @binding = 1 @role = "weight" @off = "soff" mxe : array<uint8>
    @ssbo @binding = 12 @role = "weight" wbias : array<float> //!< [ne x ndim] per-expert bias rows (hasb models)
    @ssbo @binding = 13 @role = "weight" vtab : array<float>  //!< [16] signed doubled-e2m1 values (host-built)
    @workgroup vt : float[32]

    def override stage_init(wb, il0 : uint) {
        let lid = gl_LocalInvocationID.x
        if (lid < 32u) {
            vt[lid] = vtab[lid % 16u]
        }
        barrier()
    }

    def override stage_acc(var acc : simdgroup_float8x8[8]) {
        if (ka.hasb != 0u) {
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
        let d = uint_bits_to_float(sc < 2u ? (0x00200000u << sc) : ((sc - 1u) << 23u))   //! exact e8m0_half, denormal e < 2 included
        let u4 = mxq4[blk]
        for [unroll_full] (w in range(4)) {
            let uw = w == 0 ? u4.x : (w == 1 ? u4.y : (w == 2 ? u4.z : u4.w))
            for [unroll_full] (c in range(4)) {
                let nby = (uw >> (uint(c) * 8u)) & 255u
                va[w * 4 + c] = float16(vt[il0 == 0u ? (nby & 15u) : (nby >> 4u)] * d)
            }
        }
        publish_a(va, tA0)
    }

    [metal_kernel(name="metal_moe_mulmm_mx4_msl")]
    def metal_moe_mulmm_mx4 {
        run()
    }
}




[metal_dispatch(name = "enc_rope", pso = "g_pf_pso_rope", tg = 64, grid = "npairs/64", params = "npairs : int64")]
class MetalRope {
    @ssbo @binding = 0 @role = "readwrite" @off = "voff" v : array<float>     //!< [ntok x stride] roped in place over n columns from voff
    @ssbo @binding = 1 @role = "weight" tcos : array<float>  //!< [ntok x head_size/2], written once at attach
    @ssbo @binding = 2 @role = "weight" tsin : array<float>
    @uniform @binding = 3 n : uint          //!< roped width (qd or kv_dim), % head_size
    @uniform @binding = 4 head_size : uint
    @uniform @binding = 5 npairs : uint     //!< ntok * n/2 — grid guard
    @uniform @binding = 6 neox : uint       //!< pair (j, j+half) instead of (2j, 2j+1); same table row j
    @uniform @binding = 7 @default = head_size rot : uint   //!< partial rotary width; null bind = full rope (== head_size)
    @uniform @binding = 8 @default = n stride : uint   //!< the row stride; null bind = the rows are the roped width

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
        let in_rotary = j < halfr   //! rotates the (j, j+rot/2) pair; the rest identity-cover [rot, hs)
        let hb = p * stride + h * head_size
        let i = neox != 0u ? (in_rotary ? hb + j : hb + rot + (j - halfr)) : hb + 2u * j
        let i2 = neox != 0u ? (in_rotary ? i + halfr : i + (half - halfr)) : i + 1u
        let fcr = in_rotary ? tcos[p * halfr + j] : 1.0
        let fci = in_rotary ? tsin[p * halfr + j] : 0.0
        let v0 = v[i]
        let v1 = v[i2]
        v[i] = v0 * fcr - v1 * fci
        v[i2] = v0 * fci + v1 * fcr
    }
}

struct QkRopeArgs {
    rstride : uint  //!< row width (qd or kv_dim)
    hs : uint
    eps : float
    donorm : uint   //!< 0 = rope-only (no per-head RMS)
    neox : uint
    rot : uint      //!< partial rotary width (== hs for full rope)
    nreal : uint    //!< rows past this are PAD: copied (and f16-stored), never normed or roped
    hscale : float  //!< the f16 twin's scale - the softmax scale when the attention's K halves carry none, else 1
}

[ |> template_struct_instance]
class template MetalQkRopeT {
    @ssbo @binding = 0 @off = "xoff" x : array<float>     //!< bound at the panel offset; rewritten in place
    @ssbo @binding = 1 @role = "weight" w : array<float>  //!< per-head norm weight (ignored when donorm == 0)
    @ssbo @binding = 2 @role = "weight" tcos : array<float>  //!< [nreal x rot/2]
    @ssbo @binding = 3 @role = "weight" tsin : array<float>
    @uniform @binding = 4 ka : QkRopeArgs
    @ssbo @binding = 5 @role = "write" @template_gate = HX xh : array<float16>   //!< f16 twin of the panel
    @template_constant HX : bool = false
    @workgroup part : float[16]     //!< one per simdgroup - 16 slots = hs/2 <= 512 threads

    [metal_kernel, arch(at="../ARCHITECTURE_GPU_PREFILL.md#prefill-twin-dual-store")]
    def metal_qk_rope {
        let h = gl_WorkGroupID.x
        let p = gl_WorkGroupID.y
        let lid = gl_LocalInvocationID.x
        let hs = ka.hs
        let half = hs / 2u
        let halfr = ka.rot / 2u
        let hb = p * ka.rstride + h * hs
        let in_rot = lid < halfr
        //! the pair mapping is MetalRope's verbatim: neox pairs (j, j+halfr) with the
        //! identity pairs covering [rot, hs); classic pairs (2j, 2j+1)
        var i = 0u
        var i2 = 0u
        if (ka.neox != 0u) {
            i = in_rot ? hb + lid : hb + ka.rot + (lid - halfr)
            i2 = in_rot ? i + halfr : i + (half - halfr)
        } else {
            i = hb + 2u * lid
            i2 = i + 1u
        }
        var v0 = x[i]
        var v1 = x[i2]
        let live = p < ka.nreal   //! workgroup-uniform - the barrier below never diverges
        if (live && ka.donorm != 0u) {
            let total = tg_sum_all(v0 * v0 + v1 * v1, part)
            let inv = 1.0 / sqrt(total / float(hs) + ka.eps)
            v0 = w[i - hb] * (v0 * inv)
            v1 = w[i2 - hb] * (v1 * inv)
        }
        var r0 = v0
        var r1 = v1
        if (live && in_rot) {
            let fcr = tcos[p * halfr + lid]
            let fci = tsin[p * halfr + lid]
            r0 = v0 * fcr - v1 * fci
            r1 = v0 * fci + v1 * fcr
        }
        x[i] = r0
        x[i2] = r1
        static_if (HX) {
            xh[i] = float16(r0 * ka.hscale)
            xh[i2] = float16(r1 * ka.hscale)
        }
    }
}

[metal_dispatch(name = "enc_qk_rope", pso = "g_pf_pso_qkr", tgmem = "MetalQkRope_metal_qk_rope_msl_tgmem", tg = "hs2", grid = "nheads, nrows", params = "nheads : int64, nrows : int64, hs2 : int64")]
class MetalQkRope : MetalQkRopeT {}

[metal_dispatch(name = "enc_qk_rope_hx", pso = "g_pf_pso_qkr_hx", tgmem = "MetalQkRopeHX_metal_qk_rope_msl_tgmem", tg = "hs2", grid = "nheads, nrows", params = "nheads : int64, nrows : int64, hs2 : int64")]
class MetalQkRopeHX : MetalQkRopeT {
    override HX = true
}

//! x [nrows x n] += b [n] (the tower driver binds biases inside the model blob)
[metal_dispatch(name = "enc_add_bias_rows", pso = "g_pf_pso_addbias", tg = 64, grid = "total/64", params = "total : int64")]
class MetalAddBiasRows : MetalRowMapT {
    def override combine(a, b : float) : float => a + b

    [metal_kernel(name="metal_add_bias_rows_msl")]
    def metal_add_bias_rows {
        row_map_body()
    }
}


struct AttnArgs {
    qd : uint
    kv_dim : uint
    head_size : uint
    kv_mul : uint
    npos : uint         //!< KEY rows (continuation: qoff existing + the chunk)
    np32 : uint         //!< padded key extent = the score slab's column stride
    scale : float
    qoff : uint         //!< the chunk's first GLOBAL position (0 = fresh start)
    qrows : uint        //!< padded QUERY rows — the score slab's row extent
    window : uint       //!< > 0 = sliding span (this layer); 0 = global
    softcap : float     //!< > 0 = attention-logit soft cap (gemma2)
    hass : uint
    uend : uint         //!< > 0 = uniform span end (media rows): queries at [ulo, uend) attend [window floor, uend); 0 = causal
    ulo : uint          //!< uniform span start; read only while uend > 0 (queries outside [ulo, uend) stay causal)
}

[metal_dispatch(name = "enc_qk", pso = "g_pf_pso_qk", tgmem = "metal_attn_qk_msl_tgmem", tg = 128, grid = "mp/32, nk64/32, heads", params = "mp : int64, nk64 : int64, heads : int64")]
class MetalAttnQK {
    @ssbo @binding = 0 @role = "read" q : array<float>     //!< [mp x qd], roped
    @ssbo @binding = 1 @role = "read" k : array<float>     //!< [nk64 x kv_dim], roped (existing rows + the chunk)
    @ssbo @binding = 2 @role = "write" att : array<float16>   //!< [n_heads x qrows x np32] raw scores out, f16 slab
    @uniform @binding = 3 ka : AttnArgs
    @workgroup ta : float[2048]             //!< Q block: 32 qi-rows x 64 (row stride 64)
    @workgroup tb : float[2048]             //!< KT block: 64 k-rows x 32 j-cols (row stride 32)

    [metal_kernel(name="metal_attn_qk_msl")]
    def metal_attn_qk {
        let qi0 = gl_WorkGroupID.x * 32u
        let j0 = gl_WorkGroupID.y * 32u
        let h = gl_WorkGroupID.z
        if (pf_qk_skip(ka.uend, ka.window, ka.qoff + qi0, 32u, j0, 32u)) {
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
        var hc : simdgroup_half8x8
        sgmat_to_half(hc, c00)
        simdgroup_store(hc, att, row0, ka.np32)
        sgmat_to_half(hc, c01)
        simdgroup_store(hc, att, row0 + 8u, ka.np32)
        sgmat_to_half(hc, c10)
        simdgroup_store(hc, att, row0 + 8u * ka.np32, ka.np32)
        sgmat_to_half(hc, c11)
        simdgroup_store(hc, att, row0 + 8u * ka.np32 + 8u, ka.np32)
    }
}

[arch(at="../ARCHITECTURE_GPU_PREFILL.md#prefill-attn-slab")]
def pf_p_weight(sr : float; jglob, qpos, window : uint; softcap : float; uend, ulo : uint; m, inv : float) : float {
    let cnt = (uend > 0u && qpos >= ulo && qpos < uend) ? uend : qpos + 1u
    if (jglob < pf_av_lo(window, qpos) || jglob >= cnt) {
        return 0.0
    }
    let tv = softcap > 0.0 ? softcap_exp(sr, softcap) : sr
    return exp(tv - m) * inv
}

//! the first key an AV tile whose query rows start at global position `t0` reduces over - a row's own first key at `t0` its position
def pf_av_lo(window, t0 : uint) : uint {
    return window > 0u && t0 + 1u > window ? t0 + 1u - window : 0u
}

//! one past the last key an AV tile of `mt` query rows starting at `t0` reduces over - live rows only
def pf_av_hi(uend, ulo, npos, t0, mt : uint) : uint {
    let creach = t0 + mt
    return min(uend > 0u && t0 < uend && creach > ulo ? max(uend, creach) : creach, npos)
}

//! A QK tile of `mt` query rows from global position `t0` over the `kw` keys from `k0` has no live score:
//! its keys sit past the rows' causal reach (the uniform span's end where one is set) or under their window.
def pf_qk_skip(uend, window, t0, mt, k0, kw : uint) : bool {
    return (uend > 0u ? k0 >= max(uend, t0 + mt) : k0 > t0 + mt - 1u) || k0 + kw <= pf_av_lo(window, t0)
}

//! A score row's softmax statistics, a threadgroup a (row, head). The staged stamp leaves the raw slab and writes
//! (rowmax, 1/sum); the WRITE_P stamp overwrites the slab with exp(s - rowmax) and writes 1/sum alone.
[ |> template_struct_instance]
class template MetalAttnRowT {
    @ssbo @binding = 0 att : array<float16>                 //!< [n_heads x qrows x np32] raw scores; exp(s - rowmax) out under WRITE_P
    @uniform @binding = 1 ka : AttnArgs
    @ssbo @binding = 2 @role = "write" stat : array<float>  //!< [n_heads x qrows x 2] rowmax, 1/sum; [n_heads x qrows] 1/sum under WRITE_P
    @ssbo @binding = 20 @role = "weight" @default = g_one sink : array<float> //!< [nheads]; null = identity
    @workgroup red : float[4]
    @template_constant WRITE_P : bool = false

    def attn_row_body {
        let qi = gl_WorkGroupID.x
        let h = gl_WorkGroupID.y
        let lid = gl_LocalInvocationID.x
        let nthreads = gl_WorkGroupSize.x
        let qpos = ka.qoff + qi
        let cnt = (ka.uend > 0u && qpos >= ka.ulo && qpos < ka.uend) ? ka.uend : qpos + 1u
        let jlo = pf_av_lo(ka.window, qpos)
        let rowbase = (h * ka.qrows + qi) * ka.np32
        var m = -1.0e30
        var j = jlo + lid
        while (j < cnt) {
            let sr = float(att[rowbase + j])
            m = max(m, ka.softcap > 0.0 ? softcap_exp(sr, ka.softcap) : sr)
            j += nthreads
        }
        let mm = tg_max_all(m, red)
        let rowmax = ka.hass != 0u ? max(mm, sink[h]) : mm
        var sum = 0.0
        static_if (WRITE_P) {
            //! the row's 128-row AV tile reduces over this span, and a 32-row tile over a part of it
            let t0 = ka.qoff + (qi / 128u) * 128u
            let hi = pf_av_hi(ka.uend, ka.ulo, ka.npos, t0, 128u)
            j = pf_av_lo(ka.window, t0) + lid
            while (j < hi) {
                var e = 0.0
                if (j >= jlo && j < cnt) {
                    let sr = float(att[rowbase + j])
                    e = exp((ka.softcap > 0.0 ? softcap_exp(sr, ka.softcap) : sr) - rowmax)
                    sum += e
                }
                att[rowbase + j] = float16(e)
                j += nthreads
            }
        } else {
            j = jlo + lid
            while (j < cnt) {
                let sr = float(att[rowbase + j])
                sum += exp((ka.softcap > 0.0 ? softcap_exp(sr, ka.softcap) : sr) - rowmax)
                j += nthreads
            }
        }
        barrier()
        var tot = tg_sum_all(sum, red)
        if (lid == 0u) {
            if (ka.hass != 0u) {
                tot += exp(max(sink[h] - rowmax, -80.0))
            }
            static_if (WRITE_P) {
                stat[h * ka.qrows + qi] = 1.0 / tot
            } else {
                stat[(h * ka.qrows + qi) * 2u] = rowmax
                stat[(h * ka.qrows + qi) * 2u + 1u] = 1.0 / tot
            }
        }
    }
}

[metal_dispatch(name = "enc_rowstat", pso = "g_pf_pso_rowstat", tgmem = "metal_attn_rowstat_msl_tgmem", tg = 128, grid = "npos, heads", params = "npos : int64, heads : int64")]
class MetalAttnRowstat : MetalAttnRowT {
    [metal_kernel(name="metal_attn_rowstat_msl")]
    def metal_attn_rowstat {
        attn_row_body()
    }
}

[metal_dispatch(name = "enc_attn_pexp", pso = "g_pf_pso_pexp", tgmem = "metal_attn_pexp_msl_tgmem", tg = 128, grid = "npos, heads", params = "npos : int64, heads : int64")]
class MetalAttnPExp : MetalAttnRowT {
    override WRITE_P = true

    [metal_kernel(name="metal_attn_pexp_msl")]
    def metal_attn_pexp {
        attn_row_body()
    }
}

[metal_dispatch(name = "enc_av", pso = "g_pf_pso_av", tgmem = "metal_attn_av_msl_tgmem", tg = 128, grid = "mp/32, head_size/32, heads", params = "mp : int64, heads : int64, head_size : int64")]
class MetalAttnAV {
    @ssbo @binding = 0 @role = "read" att : array<float16>   //!< [n_heads x qrows x np32] RAW scores, f16 slab
    @ssbo @binding = 1 @role = "read" v : array<float>     //!< [nk64 x kv_dim] raw V (existing rows + the chunk)
    @ssbo @binding = 2 @role = "write" xb : array<float>    //!< [mp x qd] attention output
    @uniform @binding = 3 ka : AttnArgs
    @ssbo @binding = 4 @role = "read" stat : array<float>  //!< [n_heads x qrows x 2] rowmax, 1/sum
    @workgroup ta : float[1024]             //!< P block: 32 qi-rows x 32 j (stride 32)
    @workgroup tb : float[1024]             //!< V block: 32 j-rows x 32 d (stride 32)

    [metal_kernel(name="metal_attn_av_msl")]
    def metal_attn_av {
        let qi0 = gl_WorkGroupID.x * 32u
        let d0 = gl_WorkGroupID.y * 32u
        let h = gl_WorkGroupID.z
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let kvhoff = (h / ka.kv_mul) * ka.head_size
        let jlimit = pf_av_hi(ka.uend, ka.ulo, ka.np32, ka.qoff + qi0, 32u)
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
                let qpos = ka.qoff + qi0 + r
                let sbase = (h * ka.qrows + qi0 + r) * 2u
                //! jlimit is np32-clamped and jb steps by 32, so jb + c is always a live score column
                let sr = float(att[(h * ka.qrows + qi0 + r) * ka.np32 + jb + c])
                ta[e] = pf_p_weight(sr, jb + c, qpos, ka.window, ka.softcap, ka.uend, ka.ulo, stat[sbase], stat[sbase + 1u])
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


[ |> template_struct_instance]
class template MetalAttnQKMmTensorT {
    @ssbo @binding = 0 q : array<XT>
    @ssbo @binding = 1 k : array<float>
    @ssbo @binding = 2 att : array<float16>
    @uniform @binding = 3 ka : AttnArgs
    @workgroup twb : float16[2048]          //!< K chunk: 64 keys x 32 k, flat (scale folded here)

    [metal_kernel(float_a_ok=true)]
    def metal_attn_qk_mm_t {
        let mBase = gl_WorkGroupID.x * 32u
        let nBase = gl_WorkGroupID.y * 64u
        if (pf_qk_skip(ka.uend, ka.window, ka.qoff + mBase, 32u, nBase, 64u)) {
            return
        }
        let h = gl_WorkGroupID.z
        let kvhoff = (h / ka.kv_mul) * ka.head_size
        let qhoff = h * ka.head_size
        let lid = gl_LocalInvocationID.x
        var acc : float[2048]
        var cp = unsafe(addr(att[h * ka.qrows * ka.np32 + mBase * ka.np32 + nBase]))
        tmm2d_tg_begin_deva(acc, unsafe(addr(q[0])), 32u, 64u, 4u, 32u)
        var kb = 0u
        while (kb * 32u < ka.head_size) {
            var work = lid
            while (work < 128u) {
                let j = work >> 1u
                let kh = (work & 1u) * 16u
                let kp = unsafe(addr(k[(nBase + j) * ka.kv_dim + kvhoff + kb * 32u + kh]))
                for [unroll_full] (t in range(16)) {
                    twb[j * 32u + kh + uint(t)] = float16(unsafe(kp[t]) * ka.scale)
                }
                work += gl_WorkGroupSize.x
            }
            barrier()
            tmm2d_tg_step_deva(acc, unsafe(addr(q[mBase * ka.qd + qhoff + kb * 32u])), ka.qd, twb, 32u, 64u, 32u)
            barrier()
            kb++
        }
        tmm2d_tg_store_half(acc, cp, 32u, 64u, ka.np32)
    }
}

[metal_dispatch(name = "enc_qk_mm_t_c", pso = "g_pf_pso_qkmm_t", tgmem = "MetalAttnQKMmT_metal_attn_qk_mm_t_msl_tgmem", tg = 128, grid = "mp/32, nk64/64, heads", params = "mp : int64, nk64 : int64, heads : int64")]
class MetalAttnQKMmT : MetalAttnQKMmTensorT {
    typedef XT = float
}

[metal_dispatch(name = "enc_qk_mm_th_c", pso = "g_pf_pso_qkmm_th", tgmem = "MetalAttnQKMmTH_metal_attn_qk_mm_t_msl_tgmem", tg = 128, grid = "mp/32, nk64/64, heads", params = "mp : int64, nk64 : int64, heads : int64")]
class MetalAttnQKMmTH : MetalAttnQKMmTensorT {
    typedef XT = float16
}

[metal_dispatch(name = "enc_qk_mm_c", pso = "g_pf_pso_qkmm", tgmem = "metal_attn_qk_mm_msl_tgmem", tg = 128, grid = "mp/32, nk64/64, heads", params = "mp : int64, nk64 : int64, heads : int64")]
class MetalAttnQKMm : MetalMmTileBase {
    typedef TILET = float16
    typedef SGT = simdgroup_half8x8
    @ssbo @binding = 0 @role = "read" q : array<float4>    //!< [mp x qd/4], roped
    @ssbo @binding = 1 @role = "read" k : array<float4>    //!< [nk64 x kv_dim/4], roped (existing rows + the chunk)
    @ssbo @binding = 2 @role = "write" att : array<float16>   //!< [n_heads x qrows x np32] raw scores out, f16 slab
    @uniform @binding = 3 ka : AttnArgs
    //! inherited ta = K tiles (k row, key col), tb = Q tiles (query row, k col)

    [metal_kernel(name="metal_attn_qk_mm_msl")]
    def metal_attn_qk_mm {
        let mBase = gl_WorkGroupID.x * 32u  //! query rows
        let nBase = gl_WorkGroupID.y * 64u  //! key rows
        if (pf_qk_skip(ka.uend, ka.window, ka.qoff + mBase, 32u, nBase, 64u)) {
            return
        }
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let h = gl_WorkGroupID.z
        let kvhoff4 = ((h / ka.kv_mul) * ka.head_size) / 4u
        let qoff4 = (h * ka.head_size) / 4u
        let qd4 = ka.qd / 4u
        let kvd4 = ka.kv_dim / 4u
        var mc : simdgroup_float8x8[8]      //!< C quadrant: 2 row-tiles x 4 col-tiles
        let lr0 = lid / 2u                  //! A split: key row + 16-k half
        let il0 = lid % 2u
        let syA = lr0 / 8u
        let lxA = lr0 % 8u
        let tA0 = (il0 * 2u * 8u + syA) * 64u + lxA
        let lr1 = lid / 4u                  //! B split: query row + 8-k run
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
            for [unroll_full] (i in range(16)) {
                ta[tA0 + uint(i / 8) * 512u + uint(i % 8) * 8u] = va[i]
            }
            tg_store_half4(tb, ibB, b0)
            tg_store_half4(tb, ibB + 4u, b1)
            barrier()
            acc_quad(aB, bB, ma, mb, mc)
            kb++
        }
        let base = h * ka.qrows * ka.np32
        let row0 = mBase + (sg / 2u) * 16u  //! accumulator rows = queries
        let col0 = nBase + (sg % 2u) * 32u  //! accumulator cols = keys
        var hc : simdgroup_half8x8
        for [unroll_full] (i in range(8)) {
            sgmat_to_half(hc, mc[i])
            simdgroup_store(hc, att, base + (row0 + uint(i / 4) * 8u) * ka.np32 + col0 + uint(i % 4) * 8u, ka.np32)
        }
    }
}


[ |> template_struct_instance]
class template MetalAttnAVMmTensorT {
    @ssbo @binding = 0 att : array<float16>
    @ssbo @binding = 1 v : array<float>
    @ssbo @binding = 2 xb : array<float>
    @uniform @binding = 3 ka : AttnArgs
    @ssbo @binding = 4 @role = "read" stat : array<float>  //!< [n_heads x qrows x 2] rowmax, 1/sum
    @workgroup twp : float16[2048]          //!< P chunk: 32 rows x 64 pos, exp applied at staging
    @workgroup twb : float16[4096]          //!< V chunk: 64 positions x 64 v-cols, NATURAL rows
    @template_constant PADFREE : bool = false

    [metal_kernel]
    def metal_attn_av_mm_t {
        let mBase = gl_WorkGroupID.x * 32u
        let vBase = gl_WorkGroupID.y * 64u
        let h = gl_WorkGroupID.z
        let kvhoff = (h / ka.kv_mul) * ka.head_size
        let lid = gl_LocalInvocationID.x
        let klimit = pf_av_hi(ka.uend, ka.ulo, ka.np32, ka.qoff + mBase, 32u)
        var acc : float[2048]
        var cp = unsafe(addr(xb[mBase * ka.qd + h * ka.head_size + vBase]))
        tmm2d_tg_begin_nt(acc, 32u, 64u, 4u, 64u)
        var kb = 0u
        while (kb * 64u < klimit) {
            var work = lid
            while (work < 128u) {
                let r = work >> 2u
                let ph = (work & 3u) * 16u
                let qpos = ka.qoff + mBase + r
                let sbase = (h * ka.qrows + mBase + r) * 2u
                let sm = stat[sbase]
                let sinv = stat[sbase + 1u]
                let ap = unsafe(addr(att[(h * ka.qrows + mBase + r) * ka.np32 + kb * 64u + ph]))
                for [unroll_full] (t in range(16)) {
                    let jg = kb * 64u + ph + uint(t)
                    //! jg stays below np32 on both stamps, so only the V-row guard below varies with PADFREE
                    let sr = float(unsafe(ap[t]))
                    twp[r * 64u + ph + uint(t)] = float16(pf_p_weight(sr, jg, qpos, ka.window, ka.softcap, ka.uend, ka.ulo, sm, sinv))
                }
                work += gl_WorkGroupSize.x
            }
            work = lid
            while (work < 128u) {
                let p = work >> 1u
                let ch = (work & 1u) * 32u
                let pos = kb * 64u + p
                let vp = unsafe(addr(v[pos * ka.kv_dim + kvhoff + vBase + ch]))
                for [unroll_full] (t in range(32)) {
                    twb[p * 64u + ch + uint(t)] = PADFREE ? float16(unsafe(vp[t])) : (pos < ka.npos ? float16(unsafe(vp[t])) : float16(0.0))
                }
                work += gl_WorkGroupSize.x
            }
            barrier()
            tmm2d_tg_step_nt(acc, twp, twb, 32u, 64u, 64u)
            barrier()
            kb++
        }
        tmm2d_tg_store(acc, cp, 32u, 64u, ka.qd)
    }
}

[metal_dispatch(name = "enc_av_mm_t_c", pso = "g_pf_pso_avmm_t", tgmem = "MetalAttnAVMmT_metal_attn_av_mm_t_msl_tgmem", tg = 128, grid = "mp/32, head_size/64, heads", params = "mp : int64, heads : int64, head_size : int64")]
class MetalAttnAVMmT : MetalAttnAVMmTensorT {
}

[metal_dispatch(name = "enc_av_mm_tp_c", pso = "g_pf_pso_avmm_tp", tgmem = "MetalAttnAVMmTP_metal_attn_av_mm_t_msl_tgmem", tg = 128, grid = "mp/32, head_size/64, heads", params = "mp : int64, heads : int64, head_size : int64")]
class MetalAttnAVMmTP : MetalAttnAVMmTensorT {
    override PADFREE = true
}

[ |> template_struct_instance]
class template MetalAttnAVMmSgT : MetalMmTileBase {
    typedef TILET = float16
    typedef SGT = simdgroup_half8x8
    @ssbo @binding = 0 @role = "read" att : array<float16>  //!< [n_heads x qrows x np32] RAW scores, f16 slab
    @ssbo @binding = 1 @role = "read" v : array<float4>    //!< [nk64 x kv_dim/4] raw V (existing rows + the chunk)
    @ssbo @binding = 2 @role = "write" xb : array<float>    //!< [mp x qd] attention output
    @uniform @binding = 3 ka : AttnArgs
    @ssbo @binding = 4 @role = "read" stat : array<float>  //!< [n_heads x qrows x 2] rowmax, 1/sum
    @template_constant PADFREE : bool = false

    [metal_kernel]
    def metal_attn_av_mm {
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let mBase = gl_WorkGroupID.x * 32u
        let vBase = gl_WorkGroupID.y * 64u  //! V columns within the head
        let h = gl_WorkGroupID.z
        let kvhoff4 = ((h / ka.kv_mul) * ka.head_size) / 4u
        let kvd4 = ka.kv_dim / 4u
        var mc : simdgroup_float8x8[8]      //!< C quadrant: 2 row-tiles x 4 col-tiles
        //! A split: thread stages one position row x 16 contiguous V columns (4 x tg_store_half4)
        let pr = lid / 4u
        let vc16 = (lid % 4u) * 16u
        let sxyA = (pr / 8u) * 8u * 64u + (pr % 8u) * 8u
        //! B split: thread stages one token row x 8 contiguous positions of P
        let lr1 = lid / 4u
        let iy = (lid % 4u) * 8u
        let ibB = ((lid % 4u) * 4u + lr1 / 8u) * 64u + (lr1 % 8u) * 8u
        let aB = (sg % 2u) * 256u
        let bB = (sg / 2u) * 128u
        var ma : simdgroup_half8x8[4]
        var mb : simdgroup_half8x8[2]
        let klimit = pf_av_hi(ka.uend, ka.ulo, ka.np32, ka.qoff + mBase, 32u)
        //! head_size % 64 != 0 (the towers' 96 pad): this thread's 16-col V run may sit past the
        //! head - zero it (loop-invariant, hoists) and clamp the address off the buffer tail
        let vok = vBase + vc16 < ka.head_size
        var kb = 0u
        while (kb * 32u < klimit) {
            let posg = kb * 32u + pr
            let srcA = vok ? posg * kvd4 + kvhoff4 + (vBase + vc16) / 4u : 0u
            let zero = float4(0.0)
            let a0 = vok && (PADFREE || posg < ka.npos) ? v[srcA] : zero
            let a1 = vok && (PADFREE || posg < ka.npos) ? v[srcA + 1u] : zero
            let a2 = vok && (PADFREE || posg < ka.npos) ? v[srcA + 2u] : zero
            let a3 = vok && (PADFREE || posg < ka.npos) ? v[srcA + 3u] : zero
            let srcB = (h * ka.qrows + mBase + lr1) * ka.np32 + kb * 32u + iy
            let qpos = ka.qoff + mBase + lr1
            let sbase = (h * ka.qrows + mBase + lr1) * 2u
            let sm = stat[sbase]
            let sinv = stat[sbase + 1u]
            let jg = kb * 32u + iy
            let b0 = float4(pf_p_weight(float(att[srcB]), jg, qpos, ka.window, ka.softcap, ka.uend, ka.ulo, sm, sinv),
                pf_p_weight(float(att[srcB + 1u]), jg + 1u, qpos, ka.window, ka.softcap, ka.uend, ka.ulo, sm, sinv),
                pf_p_weight(float(att[srcB + 2u]), jg + 2u, qpos, ka.window, ka.softcap, ka.uend, ka.ulo, sm, sinv),
                pf_p_weight(float(att[srcB + 3u]), jg + 3u, qpos, ka.window, ka.softcap, ka.uend, ka.ulo, sm, sinv))
            let b1 = float4(pf_p_weight(float(att[srcB + 4u]), jg + 4u, qpos, ka.window, ka.softcap, ka.uend, ka.ulo, sm, sinv),
                pf_p_weight(float(att[srcB + 5u]), jg + 5u, qpos, ka.window, ka.softcap, ka.uend, ka.ulo, sm, sinv),
                pf_p_weight(float(att[srcB + 6u]), jg + 6u, qpos, ka.window, ka.softcap, ka.uend, ka.ulo, sm, sinv),
                pf_p_weight(float(att[srcB + 7u]), jg + 7u, qpos, ka.window, ka.softcap, ka.uend, ka.ulo, sm, sinv))
            barrier()
            let tbase = sxyA + (vc16 / 8u) * 64u            //! first col-tile this thread touches
            tg_store_half4(ta, tbase, a0)
            tg_store_half4(ta, tbase + 4u, a1)
            tg_store_half4(ta, tbase + 64u, a2)             //! next 8-column tile
            tg_store_half4(ta, tbase + 68u, a3)
            tg_store_half4(tb, ibB, b0)
            tg_store_half4(tb, ibB + 4u, b1)
            barrier()
            acc_quad(aB, bB, ma, mb, mc)
            kb++
        }
        let row0 = mBase + (sg / 2u) * 16u  //! accumulator rows = tokens
        let hcol0 = vBase + (sg % 2u) * 32u   //! head-local: a 96-pad tail tile past head_size would trample the next head
        let col0 = h * ka.head_size + hcol0
        for [unroll_full] (i in range(8)) {
            if (hcol0 + uint(i % 4) * 8u < ka.head_size) {
                simdgroup_store(mc[i], xb, (row0 + uint(i / 4) * 8u) * ka.qd + col0 + uint(i % 4) * 8u, ka.qd)
            }
        }
    }
}

[metal_dispatch(name = "enc_av_mm_c", pso = "g_pf_pso_avmm", tgmem = "MetalAttnAVMm_metal_attn_av_mm_msl_tgmem", tg = 128, grid = "mp/32, head_size/64, heads", params = "mp : int64, heads : int64, head_size : int64")]
class MetalAttnAVMm : MetalAttnAVMmSgT {
}

[metal_dispatch(name = "enc_av_mm_p_c", pso = "g_pf_pso_avmm_p", tgmem = "MetalAttnAVMmP_metal_attn_av_mm_msl_tgmem", tg = 128, grid = "mp/32, head_size/64, heads", params = "mp : int64, heads : int64, head_size : int64")]
class MetalAttnAVMmP : MetalAttnAVMmSgT {
    override PADFREE = true
}

//! A K and a V panel of `total` elements as halves, the softmax scale on K: the device pair's twins.
def enc_kv_twin(enc : MetalComputeEncoder?; bk, bv, bkh, bvh : MetalBuffer?; scale : float; total : int64) {
    var ha = HalfRowsArgs(total = uint(total), scale = scale)
    enc_half_rows(enc, bk, bkh, 0ul, ha, total)
    ha.scale = 1.0
    enc_half_rows(enc, bv, bvh, 0ul, ha, total)
}

[ |> template_struct_instance]
class template MetalAttnQKDevT {
    @template_constant MT : uint = 32u
    @ssbo @binding = 0 @role = "read" q : array<float16>    //!< [mp x qd] roped Q, the half twin
    @ssbo @binding = 1 @role = "read" @off = "koff" k : array<float16>    //!< [nk64 x kv_dim] roped K halves; the softmax scale rides K or Q
    @ssbo @binding = 2 att : array<float16>
    @uniform @binding = 3 ka : AttnArgs

    [metal_kernel]
    def metal_attn_qk_dev {
        let mBase = gl_WorkGroupID.x * MT
        let nBase = gl_WorkGroupID.y * 64u
        if (pf_qk_skip(ka.uend, ka.window, ka.qoff + mBase, MT, nBase, 64u)) {
            return
        }
        let h = gl_WorkGroupID.z
        var acc : float[8192]
        var cp = unsafe(addr(att[(h * ka.qrows + mBase) * ka.np32 + nBase]))
        tmm2d_tg_begin_devab(acc, MT, 64u, 4u)
        tmm2d_tg_step_devab(acc, unsafe(addr(q[mBase * ka.qd + h * ka.head_size])), ka.qd,
            unsafe(addr(k[nBase * ka.kv_dim + (h / ka.kv_mul) * ka.head_size])), ka.kv_dim, MT, 64u, ka.head_size)
        tmm2d_tg_store_half(acc, cp, MT, 64u, ka.np32)
    }
}

[metal_dispatch(name = "enc_qk_dev_c", pso = "g_pf_pso_qk_dev", tg = 128, grid = "mp/32, nk64/64, heads", params = "mp : int64, nk64 : int64, heads : int64", requires = "mp % 32, nk64 % 64")]
class MetalAttnQKDev : MetalAttnQKDevT {
}

[metal_dispatch(name = "enc_qk_dev128_c", pso = "g_pf_pso_qk_dev128", tg = 128, grid = "mp/128, nk64/64, heads", params = "mp : int64, nk64 : int64, heads : int64", requires = "mp % 128, nk64 % 64")]
class MetalAttnQKDev128 : MetalAttnQKDevT {
    override MT = 128u
}

//! The head's columns go 64 a tile; a head off the 64 lattice (96: phi-3) takes its last 32 through the TAIL stamp,
//! one tile at the head's last whole-64 boundary, so no tile reads or writes past the head.
[ |> template_struct_instance]
class template MetalAttnAVDevT {
    @template_constant MT : uint = 32u
    @template_constant NT : uint = 64u      //!< the tile's V columns
    @template_constant TAIL : bool = false  //!< the 32-column tail past the head's 64-wide tiles
    @ssbo @binding = 0 @role = "read" att : array<float16>  //!< [n_heads x qrows x np32] exp(s - rowmax)
    @ssbo @binding = 1 @role = "read" @off = "voff" v : array<float16>    //!< [nk64 x kv_dim] V halves
    @ssbo @binding = 2 xb : array<float>
    @uniform @binding = 3 ka : AttnArgs
    @ssbo @binding = 4 @role = "read" stat : array<float>   //!< [n_heads x qrows] 1/sum

    [metal_kernel]
    def metal_attn_av_dev {
        let mBase = gl_WorkGroupID.x * MT
        let vBase = TAIL ? (ka.head_size / 64u) * 64u : gl_WorkGroupID.y * 64u
        let h = gl_WorkGroupID.z
        let lo = pf_av_lo(ka.window, ka.qoff + mBase)
        let hi = pf_av_hi(ka.uend, ka.ulo, ka.npos, ka.qoff + mBase, MT)
        var acc : float[8192]
        var cp = unsafe(addr(xb[mBase * ka.qd + h * ka.head_size + vBase]))
        tmm2d_tg_begin_devabnt(acc, MT, NT, 4u)
        tmm2d_tg_step_devabnt(acc, unsafe(addr(att[(h * ka.qrows + mBase) * ka.np32 + lo])), ka.np32,
            unsafe(addr(v[lo * ka.kv_dim + (h / ka.kv_mul) * ka.head_size + vBase])), ka.kv_dim, MT, NT, hi - lo)
        tmm2d_tg_rowscale(acc, unsafe(addr(stat[h * ka.qrows + mBase])), MT, NT)
        tmm2d_tg_store(acc, cp, MT, NT, ka.qd)
    }
}

[metal_dispatch(name = "enc_av_dev_c", pso = "g_pf_pso_av_dev", tg = 128, grid = "mp/32, ntile, heads", params = "mp : int64, heads : int64, ntile : int64", requires = "mp % 32")]
class MetalAttnAVDev : MetalAttnAVDevT {
}

[metal_dispatch(name = "enc_av_dev128_c", pso = "g_pf_pso_av_dev128", tg = 128, grid = "mp/128, ntile, heads", params = "mp : int64, heads : int64, ntile : int64", requires = "mp % 128")]
class MetalAttnAVDev128 : MetalAttnAVDevT {
    override MT = 128u
}

[metal_dispatch(name = "enc_av_dev_t_c", pso = "g_pf_pso_av_dev_t", tg = 128, grid = "mp/32, 1, heads", params = "mp : int64, heads : int64, head_size : int64", requires = "mp % 32, head_size % 32")]
class MetalAttnAVDevTail : MetalAttnAVDevT {
    override NT = 32u
    override TAIL = true
}

[metal_dispatch(name = "enc_av_dev128_t_c", pso = "g_pf_pso_av_dev128_t", tg = 128, grid = "mp/128, 1, heads", params = "mp : int64, heads : int64, head_size : int64", requires = "mp % 128, head_size % 32")]
class MetalAttnAVDev128Tail : MetalAttnAVDevT {
    override MT = 128u
    override NT = 32u
    override TAIL = true
}

//!@nolint
//! The tower flash - one-pass attention monomorphized at DK = DV = 72
//! (8 queries/tg, 64 keys/block, 4 simdgroups): bidirectional, no score slab, no rowstat,
//! no head restrides. K/V are per-head-contiguous f16 panels [heads x npos x 72] with
//! npos % 64 rows and >= 256 elements of V tail slack (dead PV-pad tiles read past the last
//! row by design); Q and the output ride the towers' compact f32 [pos x heads*72] rows.
[metal_dispatch(name = "enc_tower_flash", pso = "g_pf_pso_twr_flash", tgmem = "metal_tower_flash_msl_tgmem", tg = 128, grid = "mp/8, heads", params = "mp : int64, heads : int64")]
class MetalTowerFlash {
    @ssbo @binding = 0 @role = "read" q : array<float>      //!< [npos x heads*72] compact, roped, UNSCALED
    @ssbo @binding = 1 @role = "read" kh : array<float16>   //!< [heads x npos x 72] f16
    @ssbo @binding = 2 @role = "read" vh : array<float16>   //!< [heads x npos x 72] f16 + tail slack
    @ssbo @binding = 3 @role = "write" xb : array<float>    //!< [mp x heads*72] compact attention out
    @uniform @binding = 4 ka : AttnArgs
    @workgroup sq : float16[576]        //!< Q tile: 8 q-rows x 72
    @workgroup so : float[1024]         //!< O: 8 rows x PV = 128 (cols past 72 ride dead, never stored)
    @workgroup ss : float[1024]         //!< scores: 8 rows x SH = 128 (64 used per block)

    [metal_kernel(name="metal_tower_flash_msl"), arch(at="../ARCHITECTURE_GPU_TOWER.md#tower-attn-routes")]
    def metal_tower_flash {   // nolint:STYLE038 — the one-pass flash body, barrier-coupled
        let iq2 = gl_WorkGroupID.y
        let iq1 = gl_WorkGroupID.x * 8u
        let lid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID
        let lane = lid & 31u
        let np = ka.npos
        let qd = ka.qd
        for (jj in range(2)) {
            let j = uint(jj) * 4u + sg
            var i = lane
            while (i < 72u) {
                sq[j * 72u + i] = iq1 + j < np ? float16(q[(iq1 + j) * qd + iq2 * 72u + i]) : float16(0.0)
                so[j * 128u + i] = 0.0
                i += 32u
            }
        }
        barrier()
        var s0acc = 0.0
        var s1acc = 0.0
        var m0acc = -1.5e38
        var m1acc = -1.5e38
        var ic = 0u
        while (ic < np) {
            var kp = unsafe(addr(kh[(iq2 * np + ic + sg * 8u) * 72u]))
            var psoff = sg * 8u
            for (_cc in range(2)) {
                var mqk : simdgroup_float8x8
                for [unroll_full] (i in range(9)) {
                    var mk : simdgroup_half8x8
                    var mq : simdgroup_half8x8
                    simdgroup_load(mk, kp, uint(i) * 8u, 72u, true)
                    simdgroup_load(mq, sq, uint(i) * 8u, 72u)
                    simdgroup_multiply_accumulate(mqk, mq, mk, mqk)
                }
                simdgroup_store(mqk, ss, psoff, 128u)
                kp = unsafe(kp + 8 * 4 * 72)
                psoff += 32u
            }
            barrier()
            for (jj in range(2)) {
                let j = uint(jj) * 4u + sg
                let mold = jj == 0 ? m0acc : m1acc
                let sa = ss[j * 128u + 2u * lane] * ka.scale
                let sb = ss[j * 128u + 2u * lane + 1u] * ka.scale
                let mnew = simd_max(max(mold, max(sa, sb)))
                let ms = exp(mold - mnew)
                let va = exp(sa - mnew)
                let vb = exp(sb - mnew)
                if (jj == 0) {
                    m0acc = mnew
                    s0acc = s0acc * ms + simd_sum(va + vb)
                } else {
                    m1acc = mnew
                    s1acc = s1acc * ms + simd_sum(va + vb)
                }
                ss[j * 128u + 2u * lane] = va
                ss[j * 128u + 2u * lane + 1u] = vb
                var i = lane
                while (i < 72u) {
                    so[j * 128u + i] *= ms
                    i += 32u
                }
            }
            barrier()
            var lo : simdgroup_float8x8[4]
            for [unroll_full] (ii in range(4)) {
                simdgroup_load(lo[ii], so, sg * 8u + uint(ii) * 32u, 128u)
            }
            var pv = unsafe(addr(vh[iq2 * np * 72u + ic * 72u + sg * 8u]))
            for (cc in range(4)) {
                var vs0 : simdgroup_float8x8
                var vs1 : simdgroup_float8x8
                simdgroup_load(vs0, ss, 16u * uint(cc), 128u)
                simdgroup_load(vs1, ss, 16u * uint(cc) + 8u, 128u)
                for [unroll_full] (ii in range(2)) {
                    var mv0 : simdgroup_half8x8
                    var mv1 : simdgroup_half8x8
                    var mv2 : simdgroup_half8x8
                    var mv3 : simdgroup_half8x8
                    simdgroup_load(mv0, pv, uint(ii) * 64u, 72u)
                    simdgroup_load(mv1, pv, uint(ii) * 64u + 32u, 72u)
                    simdgroup_load(mv2, pv, uint(ii) * 64u + 8u * 72u, 72u)
                    simdgroup_load(mv3, pv, uint(ii) * 64u + 32u + 8u * 72u, 72u)
                    simdgroup_multiply_accumulate(lo[2 * ii + 0], vs0, mv0, lo[2 * ii + 0])
                    simdgroup_multiply_accumulate(lo[2 * ii + 1], vs0, mv1, lo[2 * ii + 1])
                    simdgroup_multiply_accumulate(lo[2 * ii + 0], vs1, mv2, lo[2 * ii + 0])
                    simdgroup_multiply_accumulate(lo[2 * ii + 1], vs1, mv3, lo[2 * ii + 1])
                }
                pv = unsafe(pv + 16 * 72)
            }
            for [unroll_full] (ii in range(4)) {
                simdgroup_store(lo[ii], so, sg * 8u + uint(ii) * 32u, 128u)
            }
            barrier()
            ic += 64u
        }
        for (jj in range(2)) {
            let j = uint(jj) * 4u + sg
            if (iq1 + j < np) {
                let stot = jj == 0 ? s0acc : s1acc
                let sc = stot == 0.0 ? 0.0 : 1.0 / stot
                var i = lane
                while (i < 72u) {
                    xb[(iq1 + j) * qd + iq2 * 72u + i] = so[j * 128u + i] * sc
                    i += 32u
                }
            }
        }
    }
}

//! Restride+convert one compact [npos x heads*hs] f32 tensor into the per-head-contiguous
//! f16 panels [heads x npos x hs] the tower flash reads - one thread per element.
[metal_dispatch(name = "enc_tower_kv_hc", pso = "g_pf_pso_twr_kvhc", tg = 256, grid = "total/256", params = "total : int64")]
class MetalTowerKvHc {
    @ssbo @binding = 0 @role = "read" x : array<float>
    @ssbo @binding = 1 @role = "write" xh : array<float16>
    @uniform @binding = 2 hs : uint
    @uniform @binding = 3 d : uint        //!< heads*hs, the compact row width
    @uniform @binding = 4 np : uint
    @uniform @binding = 5 total : uint    //!< np*d

    [metal_kernel(name="metal_tower_kv_hc_msl")]
    def metal_tower_kv_hc {
        let gid = gl_GlobalInvocationID.x
        if (gid >= total) {
            return
        }
        let p = gid / d
        let c = gid % d
        xh[((c / hs) * np + p) * hs + c % hs] = float16(x[gid])
    }
}

//! The block-diagonal window attention of the qwen2.5 ViT: one threadgroup per (window, head),
//! one thread per q row (windows cap at 64 rows), plain softmax, all f32 straight from device.
//! Bidirectional, no mask.
[metal_dispatch(name = "enc_tower_win_attn", pso = "g_pf_pso_twr_win", tg = 64, grid = "nwin, heads", params = "nwin : int64, heads : int64")]
class MetalTowerWinAttn {
    @ssbo @binding = 0 @role = "read" q : array<float>      //!< [npos x qd] compact, roped
    @ssbo @binding = 1 @role = "read" k : array<float>      //!< [npos x qd] compact, roped
    @ssbo @binding = 2 @role = "read" v : array<float>      //!< [npos x qd] compact
    @ssbo @binding = 3 @role = "write" xb : array<float>    //!< [npos x qd] compact attention out
    @ssbo @binding = 4 @role = "read" wlo : array<uint>     //!< [nwin + 1] window row starts + total sentinel
    @uniform @binding = 5 ka : AttnArgs

    [metal_kernel(name="metal_tower_win_msl"), arch(at="../ARCHITECTURE_GPU_TOWER.md#tower-attn-routes")]
    def metal_tower_win {
        let wi = gl_WorkGroupID.x
        let h = gl_WorkGroupID.y
        let t = gl_LocalInvocationID.x
        let hs = ka.head_size
        let qd = ka.qd
        let r0 = wlo[wi]
        let wr = wlo[wi + 1u] - r0
        if (t >= wr) {
            return
        }
        let qbase = (r0 + t) * qd + h * hs
        var sarr : float[64]
        var m = -1.5e38
        var j = 0u
        while (j < wr) {
            let kbase = (r0 + j) * qd + h * hs
            var acc = 0.0
            var i = 0u
            while (i < hs) {
                acc += q[qbase + i] * k[kbase + i]
                i++
            }
            acc *= ka.scale
            sarr[j] = acc
            m = max(m, acc)
            j++
        }
        var stot = 0.0
        j = 0u
        while (j < wr) {
            let p = exp(sarr[j] - m)
            sarr[j] = p
            stot += p
            j++
        }
        let inv = 1.0 / stot
        var i = 0u
        while (i < hs) {
            var acc = 0.0
            j = 0u
            while (j < wr) {
                acc += sarr[j] * v[(r0 + j) * qd + h * hs + i]
                j++
            }
            xb[qbase + i] = acc * inv
            i++
        }
    }
}

//! The gemma4a Conformer attention's argument block (the sliding causal window + sinusoidal RPE).
struct G4aAttnArgs {
    d : uint            //!< model width (rows are compact [npos x d])
    hs : uint           //!< head size
    npos : uint
    past : uint         //!< causal window = self + (past - 1) previous rows; RPE rows = past + 1
    qscale : float      //!< the query scale ahead of per_dim_scale
    kscale : float
    cap : float         //!< attention-score soft cap
}

//! The gemma4a causal sliding-window attention: one thread per (q row, head) — window <= 16 rows,
//! per-dim-scaled Q against K and the projected RPE table, softcapped scores, plain softmax.
[metal_dispatch(name = "enc_g4a_attn", pso = "g_pf_pso_g4a_attn", tg = 64, grid = "np64, heads", params = "np64 : int64, heads : int64")]
class MetalG4aAttn {
    @ssbo @binding = 0 @role = "read" q : array<float>      //!< [npos x d] compact
    @ssbo @binding = 1 @role = "read" k : array<float>      //!< [npos x d] compact
    @ssbo @binding = 2 @role = "read" v : array<float>      //!< [npos x d] compact
    @ssbo @binding = 3 @role = "write" ao : array<float>    //!< [npos x d] attention out
    @ssbo @binding = 4 @role = "read" rpe : array<float>    //!< [(past+1) x d] the layer's projected RPE
    @ssbo @binding = 5 @role = "read" pds : array<float>    //!< [hs] the layer's per-dim query scale
    @uniform @binding = 6 ka : G4aAttnArgs

    [metal_kernel(name="metal_g4a_attn_msl")]
    def metal_g4a_attn {
        let i = gl_GlobalInvocationID.x
        let h = gl_WorkGroupID.y
        if (i >= ka.npos) {
            return
        }
        let d = ka.d
        let hs = ka.hs
        let hoff = h * hs
        let j0 = i >= ka.past - 1u ? i - (ka.past - 1u) : 0u
        var sarr : float[16]
        var m = -1.5e38
        var j = j0
        while (j <= i) {
            let rr = (ka.past - (i - j)) * d
            var cs = 0.0
            var rs = 0.0
            var n = 0u
            while (n < hs) {
                let qv = q[i * d + hoff + n] * ka.qscale * pds[n]
                cs += qv * (k[j * d + hoff + n] * ka.kscale)
                rs += qv * rpe[rr + hoff + n]
                n++
            }
            //! tanh via the exp identity (the CPU softcap's form; Metal tanh NaNs past ~44)
            let raw = cs + rs
            let sc = softcap_exp(raw, ka.cap)
            sarr[j - j0] = sc
            m = max(m, sc)
            j++
        }
        var stot = 0.0
        j = j0
        while (j <= i) {
            let p = exp(sarr[j - j0] - m)
            sarr[j - j0] = p
            stot += p
            j++
        }
        let inv = 1.0 / stot
        var n = 0u
        while (n < hs) {
            var acc = 0.0
            j = j0
            while (j <= i) {
                acc += sarr[j - j0] * v[j * d + hoff + n]
                j++
            }
            ao[i * d + hoff + n] = acc * inv
            n++
        }
    }
}

//! The gemma4a conv module's GLU: y[t x d] = h[t][0..d) * sigmoid(h[t][d..2d)) off the pw1 rows.
[metal_dispatch(name = "enc_g4a_glu", pso = "g_pf_pso_g4a_glu", tg = 64, grid = "total/64", params = "total : int64")]
class MetalG4aGlu {
    @ssbo @binding = 0 @role = "read" h : array<float>      //!< [npos x 2d] pw1 output
    @ssbo @binding = 1 @role = "write" y : array<float>     //!< [npos x d]
    @uniform @binding = 2 d : uint
    @uniform @binding = 3 total : uint      //!< grid guard: npos * d

    [metal_kernel(name="metal_g4a_glu_msl")]
    def metal_g4a_glu {
        let gid = gl_GlobalInvocationID.x
        if (gid >= total) {
            return
        }
        let t = gid / d
        let c = gid % d
        y[gid] = h[t * 2u * d + c] / (1.0 + exp(-h[t * 2u * d + d + c]))
    }
}

//! The gemma4a causal depthwise conv (k = 5, taps [d x 5] in the blob): one thread per element,
//! rows before the window's reach read nothing (zero left context — chunk starts cold).
[metal_dispatch(name = "enc_g4a_dw", pso = "g_pf_pso_g4a_dw", tg = 64, grid = "total/64", params = "total : int64")]
class MetalG4aDw {
    @ssbo @binding = 0 @role = "read" x : array<float>      //!< [npos x d] GLU output
    @ssbo @binding = 1 @role = "write" y : array<float>     //!< [npos x d]
    @ssbo @binding = 2 @role = "weight" @off = "woff" w : array<float>   //!< [d x 5] causal taps
    @uniform @binding = 3 d : uint
    @uniform @binding = 4 total : uint      //!< grid guard: npos * d

    [metal_kernel(name="metal_g4a_dw_msl")]
    def metal_g4a_dw {
        let gid = gl_GlobalInvocationID.x
        if (gid >= total) {
            return
        }
        let t = gid / d
        let c = gid % d
        var acc = 0.0
        var kk = 0u
        while (kk < 5u) {
            if (t + kk >= 4u) {   //! src = t - 4 + kk >= 0 in unsigned form
                acc += w[c * 5u + kk] * x[(t + kk - 4u) * d + c]
            }
            kk++
        }
        y[gid] = acc
    }
}

//! the ew1 family's body under this class's OWN entry name - the census key a serving run records
[metal_dispatch(name = "enc_g4a_silu", pso = "g_pf_pso_g4a_silu", tg = 64, grid = "total/64", params = "total : int64")]
class MetalG4aSilu : MetalEw1BaseT {
    def override map(a : float) : float => a / (1.0 + exp(-a))

    [metal_kernel(name="metal_g4a_silu_msl")]
    def metal_g4a_silu {
        ew1_body()
    }
}

//! The FastConformer rel-pos attention's shape: compact rows [npos x d], the GEMM pads
//! (mp rows, nkp key columns, wwpad rel columns) and the CPU form's one scale.
struct FcAttnArgs {
    d : uint            //!< model width (rows are compact [npos x d])
    hs : uint           //!< head size (a multiple of 64 - the AV GEMM's ndim)
    npos : uint
    mp : uint           //!< npos padded to 32 (the GEMM row pad)
    nkp : uint          //!< npos padded to 64 (the content-score GEMM's ndim, the AV GEMM's kdim)
    ww : uint           //!< 2 npos - 1 rel positions
    wwpad : uint        //!< ww padded to 64 (the rel-score GEMM's ndim)
    scale : float       //!< 1/sqrt(hs), applied to content + rel scores together (the CPU form)
}

//! The attention shape from the compact geometry: the GEMM pads derive here and nowhere else.
def fc_attn_args(d, hs, npos : int64) : FcAttnArgs {
    let np = uint(npos)
    let ww = 2u * np - 1u
    return FcAttnArgs(d = uint(d), hs = uint(hs), npos = np, mp = (np + 31u) / 32u * 32u,
        nkp = (np + 63u) / 64u * 64u, ww = ww, wwpad = (ww + 63u) / 64u * 64u, scale = 1.0 / sqrt(float(hs)))
}

//! The rel-pos attention's operands off the compact rows, zeros in every pad row, one thread an element of the tallest panel.
//! The per-head stamp packs head `h` contiguous for the tile GEMMs, V transposed; the ALLHEADS stamp keeps the compact
//! row layout the device pair reads, as halves, the scale folded into K and the rel table.
[ |> template_struct_instance]
class template MetalFcOperandsT {
    @ssbo @binding = 0 @role = "read" q : array<float>      //!< [npos x d] compact
    @ssbo @binding = 1 @role = "read" k : array<float>      //!< [npos x d] compact
    @ssbo @binding = 2 @role = "read" v : array<float>      //!< [npos x d] compact
    @ssbo @binding = 3 @role = "read" posl : array<float>   //!< [ww x d] the layer's projected rel table
    @ssbo @binding = 4 @role = "weight" @off = "buoff" bu : array<float>   //!< [d] bias_u (content)
    @ssbo @binding = 5 @role = "weight" @off = "bvoff" bv : array<float>   //!< [d] bias_v (rel)
    @ssbo @binding = 6 @role = "write" qu : array<OT>       //!< [mp x width]
    @ssbo @binding = 7 @role = "write" qv : array<OT>       //!< [mp x width]
    @ssbo @binding = 8 @role = "write" kh : array<OT>       //!< [nkp x width]
    @ssbo @binding = 9 @role = "write" vo : array<OT>       //!< V_hᵀ [hs x nkp]; V [nkp x d] under ALLHEADS
    @ssbo @binding = 10 @role = "write" ph : array<OT>      //!< [wwpad x width]
    @uniform @binding = 11 ka : FcAttnArgs
    @uniform @binding = 12 @template_gate = "!ALLHEADS" h : uint   //!< the head this dispatch packs
    @template_constant ALLHEADS : bool = false

    def width : uint {
        static_if (ALLHEADS) {
            return ka.d
        } else {
            return ka.hs
        }
    }

    def column(j : uint) : uint {
        static_if (ALLHEADS) {
            return j
        } else {
            return h * ka.hs + j
        }
    }

    def v_slot(gid, r, j : uint) : uint {
        static_if (ALLHEADS) {
            return gid
        } else {
            return j * ka.nkp + r
        }
    }

    def scored(x : float) : float {
        static_if (ALLHEADS) {
            return x * ka.scale
        } else {
            return x
        }
    }

    def fc_operands_body {
        let gid = gl_GlobalInvocationID.x
        let w = width()
        if (gid >= ka.wwpad * w) {   //! the grid guard: the tallest panel's extent
            return
        }
        let r = gid / w
        let j = gid % w
        let c = column(j)
        let src = r * ka.d + c
        let vs = v_slot(gid, r, j)
        if (r < ka.npos) {
            let qval = q[src]
            qu[gid] = OT(qval + bu[c])   // nolint:PERF020 — the half stamp narrows here; on the f32 stamp the cast is the identity
            qv[gid] = OT(qval + bv[c])   // nolint:PERF020 — same narrowing
            kh[gid] = OT(scored(k[src]))   // nolint:PERF020 — same narrowing
            vo[vs] = OT(v[src])   // nolint:PERF020 — same narrowing
        } else {
            if (r < ka.mp) {
                qu[gid] = OT(0.0)   // nolint:PERF020 — same narrowing
                qv[gid] = OT(0.0)   // nolint:PERF020 — same narrowing
            }
            if (r < ka.nkp) {
                kh[gid] = OT(0.0)   // nolint:PERF020 — same narrowing
                vo[vs] = OT(0.0)   // nolint:PERF020 — same narrowing
            }
        }
        if (r < ka.ww) {
            ph[gid] = OT(scored(posl[src]))   // nolint:PERF020 — same narrowing
        } else {
            ph[gid] = OT(0.0)   // nolint:PERF020 — same narrowing
        }
    }
}

[metal_dispatch(name = "enc_fc_pack", pso = "g_pf_pso_fc_pack", tg = 64, grid = "total/64", params = "total : int64")]
class MetalFcPack : MetalFcOperandsT {
    typedef OT = float

    [metal_kernel(name="metal_fc_pack_msl")]
    def metal_fc_pack {
        fc_operands_body()
    }
}

[metal_dispatch(name = "enc_fc_twin", pso = "g_pf_pso_fc_twin", tg = 256, grid = "total/256", params = "total : int64")]
class MetalFcTwin : MetalFcOperandsT {
    typedef OT = float16
    override ALLHEADS = true

    [metal_kernel(name="metal_fc_twin_msl")]
    def metal_fc_twin {
        fc_operands_body()
    }
}

//! The rel-pos softmax row over the live keys, the rel score joining the content score at the CPU form's shift (npos - 1 - q + k).
//! The per-head stamp writes p = softmax((sc + sr) * scale), zeros in the key pad and in every pad row. The DEV stamp, a threadgroup
//! a (row, head), leaves exp(s - max) in the content slab and 1/sum in the stat plane (0 for a pad row) - the pair the device AV reads.
[ |> template_struct_instance]
class template MetalFcRowT : MetalTgReduceBase {
    @ssbo @binding = 0 sc : array<ST>                       //!< [heads x mp x nkp] content scores; exp(s - max) out under DEV
    @ssbo @binding = 1 @role = "read" sr : array<ST>        //!< [heads x mp x wwpad] rel scores
    @ssbo @binding = 2 @role = "readwrite" @template_gate = "!DEV" p : array<float>   //!< [mp x nkp] the probability rows (the exp pass writes, the normalize pass scales in place)
    @ssbo @binding = 2 @role = "write" @template_gate = DEV stat : array<float>       //!< [heads x mp] 1/sum
    @uniform @binding = 3 ka : FcAttnArgs
    @template_constant DEV : bool = false

    def score(ci, ri : uint) : float {
        static_if (DEV) {
            return float(sc[ci]) + float(sr[ri])
        } else {
            return (sc[ci] + sr[ri]) * ka.scale
        }
    }

    def fc_row_body {
        let r = gl_WorkGroupID.x
        let lid = gl_LocalInvocationID.x
        let nt = gl_WorkGroupSize.x
        let nkp = ka.nkp
        let row = gl_WorkGroupID.y * ka.mp + r   //! the per-head stamp's grid has one y
        let crow = row * nkp
        if (r >= ka.npos) {
            static_if (DEV) {
                if (lid == 0u) {
                    stat[row] = 0.0
                }
            } else {
                var z = lid
                while (z < nkp) {
                    p[crow + z] = 0.0
                    z += nt
                }
            }
            return
        }
        let rrow = row * ka.wwpad + (ka.npos - 1u - r)
        var m = -3.0e38
        var k = lid
        while (k < ka.npos) {
            m = max(m, score(crow + k, rrow + k))
            k += nt
        }
        let mx = tg_max(m)
        var l = 0.0
        k = lid
        while (k < ka.npos) {
            let e = exp(score(crow + k, rrow + k) - mx)
            static_if (DEV) {
                sc[crow + k] = float16(e)
            } else {
                p[crow + k] = e
            }
            l += e
            k += nt
        }
        barrier()
        let tot = tg_sum(l)
        static_if (DEV) {
            if (lid == 0u) {
                stat[row] = 1.0 / tot
            }
        } else {
            let inv = 1.0 / tot
            k = lid
            while (k < ka.npos) {
                p[crow + k] *= inv
                k += nt
            }
            k = ka.npos + lid
            while (k < nkp) {
                p[crow + k] = 0.0
                k += nt
            }
        }
    }
}

[metal_dispatch(name = "enc_fc_softmax", pso = "g_pf_pso_fc_softmax", tgmem = "metal_fc_softmax_msl_tgmem", tg = 256, grid = "mp", params = "mp : int64")]
class MetalFcSoftmax : MetalFcRowT {
    typedef ST = float

    [metal_kernel(name="metal_fc_softmax_msl")]
    def metal_fc_softmax {
        fc_row_body()
    }
}

[metal_dispatch(name = "enc_fc_pexp", pso = "g_pf_pso_fc_pexp", tgmem = "metal_fc_pexp_msl_tgmem", tg = 256, grid = "mp, heads", params = "mp : int64, heads : int64")]
class MetalFcPExp : MetalFcRowT {
    typedef ST = float16
    override DEV = true

    [metal_kernel(name="metal_fc_pexp_msl")]
    def metal_fc_pexp {
        fc_row_body()
    }
}

//! The heads' AV rows [heads x mp x hs] back into the compact [mp x d] attention output, zeros
//! in the pad rows so the output projection stages no recycled bytes.
[metal_dispatch(name = "enc_fc_unpack", pso = "g_pf_pso_fc_unpack", tg = 64, grid = "total/64", params = "total : int64")]
class MetalFcUnpack {
    @ssbo @binding = 0 @role = "read" oh : array<float>     //!< [heads x mp x hs]
    @ssbo @binding = 1 @role = "write" ao : array<float>    //!< [mp x d] compact, zero past npos
    @uniform @binding = 2 ka : FcAttnArgs

    [metal_kernel(name="metal_fc_unpack_msl")]
    def metal_fc_unpack {
        let gid = gl_GlobalInvocationID.x
        if (gid >= ka.mp * ka.d) {   //! the grid guard: the padded output's extent
            return
        }
        let r = gid / ka.d
        if (r >= ka.npos) {
            ao[gid] = 0.0
            return
        }
        let c = gid % ka.d
        let hs = ka.hs
        ao[gid] = oh[((c / hs) * ka.mp + r) * hs + c % hs]
    }
}

//! The FastConformer conv module's depthwise k=9 CENTERED conv over time + the folded BatchNorm
//! (bn_w/bn_b carry the fold) + SiLU — one kernel, the CPU loop's exact form. The stamp axis is
//! the tap layout: canary's channel-major [d x nk], parakeet's load-time tap-major [nk x d].
[ |> template_struct_instance]
class template MetalCnDwT {
    @ssbo @binding = 0 @role = "read" x : array<float>      //!< [npos x d] GLU output
    @ssbo @binding = 1 @role = "write" y : array<float>     //!< [npos x d]
    @ssbo @binding = 2 @role = "weight" @off = "woff" w : array<float>     //!< [d x nk] taps, or [nk x d] under TAP_MAJOR
    @ssbo @binding = 3 @role = "weight" @off = "bnwoff" bnw : array<float> //!< [d] folded BN scale
    @ssbo @binding = 4 @role = "weight" @off = "bnboff" bnb : array<float> //!< [d] folded BN shift
    @uniform @binding = 5 d : uint
    @uniform @binding = 6 nk : uint         //!< taps (odd; 9 on both families)
    @uniform @binding = 7 npos : uint
    @uniform @binding = 8 total : uint      //!< grid guard: npos * d
    @template_constant TAP_MAJOR : bool = false

    def tap(c, kk : uint) : float {
        static_if (TAP_MAJOR) {
            return w[kk * d + c]
        } else {
            return w[c * nk + kk]
        }
    }

    def dw_body {
        let gid = gl_GlobalInvocationID.x
        if (gid >= total) {
            return
        }
        let t = gid / d
        let c = gid % d
        let kpad = (nk - 1u) / 2u
        var acc = 0.0
        var kk = 0u
        while (kk < nk) {
            let src = t + kk   //! src row = t + kk - kpad, guarded in unsigned form
            if (src >= kpad && src - kpad < npos) {
                acc += tap(c, kk) * x[(src - kpad) * d + c]
            }
            kk++
        }
        let bv2 = acc * bnw[c] + bnb[c]
        y[gid] = bv2 / (1.0 + exp(-bv2))
    }
}

[metal_dispatch(name = "enc_cn_dw", pso = "g_pf_pso_cn_dw", tg = 64, grid = "total/64", params = "total : int64")]
class MetalCnDw : MetalCnDwT {
    [metal_kernel(name="metal_cn_dw_msl")]
    def metal_cn_dw {
        dw_body()
    }
}

[metal_dispatch(name = "enc_pk_dw", pso = "g_pf_pso_pk_dw", tg = 64, grid = "total/64", params = "total : int64")]
class MetalPkDw : MetalCnDwT {
    override TAP_MAJOR = true

    [metal_kernel(name="metal_pk_dw_msl")]
    def metal_pk_dw {
        dw_body()
    }
}

//! One stage of the parakeet subsample front: a k3 stride-2 pad-1 conv over an [ih x iw] image of channel rows.
struct PkFrontArgs {
    iw : uint       //!< input columns (the mel width, halved a stage)
    ih : uint       //!< input rows (time)
    ow : uint
    oh : uint
    ch : uint       //!< channels of the output (and of a depthwise stage's input)
    total : uint    //!< grid guard: oh * ow * ch
}

def pk_front_args(iw, ih, ch : int64) : PkFrontArgs {
    let ow = uint(conv_s2_out(iw))
    let oh = uint(conv_s2_out(ih))
    return PkFrontArgs(iw = uint(iw), ih = uint(ih), ow = ow, oh = oh, ch = uint(ch), total = oh * ow * uint(ch))
}

//! The front's two conv forms over tap-major weights [9][ch], one thread an output element, a tap off the image
//! adding nothing: the first conv off the one-channel mel with its bias and ReLU (FIRST), and the depthwise conv
//! with its bias - the CPU loops' forms.
[ |> template_struct_instance]
class template MetalPkFrontConvT {
    @ssbo @binding = 0 @role = "read" x : array<float>      //!< [ih x iw] (FIRST) or [ih x iw x ch]
    @ssbo @binding = 1 @role = "write" y : array<float>     //!< [oh x ow x ch]
    @ssbo @binding = 2 @role = "weight" @off = "woff" w : array<float>   //!< [9][ch]
    @ssbo @binding = 3 @role = "weight" @off = "boff" b : array<float>   //!< [ch]
    @uniform @binding = 4 ka : PkFrontArgs
    @template_constant FIRST : bool = false

    def front_conv_body {
        let gid = gl_GlobalInvocationID.x
        if (gid >= ka.total) {
            return
        }
        let c = gid % ka.ch
        let pos = gid / ka.ch
        let ox = pos % ka.ow
        let oy = pos / ka.ow
        var acc = 0.0
        for [unroll_full] (kh in range(3)) {
            let iy = oy * 2u + uint(kh)   //! the source row plus one: a row off either edge fails the guard in unsigned form
            for [unroll_full] (kw in range(3)) {
                let ix = ox * 2u + uint(kw)
                if (iy >= 1u && iy - 1u < ka.ih && ix >= 1u && ix - 1u < ka.iw) {
                    let src = (iy - 1u) * ka.iw + ix - 1u
                    static_if (FIRST) {
                        acc += w[uint(kh * 3 + kw) * ka.ch + c] * x[src]
                    } else {
                        acc += w[uint(kh * 3 + kw) * ka.ch + c] * x[src * ka.ch + c]
                    }
                }
            }
        }
        static_if (FIRST) {
            y[gid] = max(acc + b[c], 0.0)
        } else {
            y[gid] = acc + b[c]
        }
    }
}

[metal_dispatch(name = "enc_pk_conv0", pso = "g_pf_pso_pk_conv0", tg = 64, grid = "total/64", params = "total : int64")]
class MetalPkConv0 : MetalPkFrontConvT {
    override FIRST = true

    [metal_kernel(name="metal_pk_conv0_msl")]
    def metal_pk_conv0 {
        front_conv_body()
    }
}

[metal_dispatch(name = "enc_pk_dw2d", pso = "g_pf_pso_pk_dw2d", tg = 64, grid = "total/64", params = "total : int64")]
class MetalPkDw2d : MetalPkFrontConvT {
    [metal_kernel(name="metal_pk_dw2d_msl")]
    def metal_pk_dw2d {
        front_conv_body()
    }
}

//! The front's feature rows for the pre-projection: xf[t][fr + ow * c] = y[t][fr][c] over the last stage's
//! [oh x ow x ch] output, zeros in the rows past oh (the GEMM's row pad).
[metal_dispatch(name = "enc_pk_xf", pso = "g_pf_pso_pk_xf", tg = 64, grid = "total/64", params = "total : int64")]
class MetalPkXf {
    @ssbo @binding = 0 @role = "read" y : array<float>      //!< [oh x ow x ch]
    @ssbo @binding = 1 @role = "write" xf : array<float>    //!< [rows padded x ow * ch]
    @uniform @binding = 2 ka : PkFrontArgs                  //!< ow, oh, ch of the last stage; total = padded rows * ow * ch

    [metal_kernel(name="metal_pk_xf_msl")]
    def metal_pk_xf {
        let gid = gl_GlobalInvocationID.x
        if (gid >= ka.total) {
            return
        }
        let feat = ka.ow * ka.ch
        let t = gid / feat
        if (t >= ka.oh) {
            xf[gid] = 0.0
            return
        }
        let j = gid % feat
        xf[gid] = y[(t * ka.ow + j % ka.ow) * ka.ch + j / ka.ow]
    }
}

//! x [rows x n] = max(x + v, 0) in place: a 1x1 conv's bias and ReLU over its GEMM's rows.
[metal_dispatch(name = "enc_bias_relu_rows", pso = "g_pf_pso_bias_relu", tg = 64, grid = "total/64", params = "total : int64")]
class MetalBiasReluRows : MetalRowMapT {
    def override combine(a, b : float) : float => max(a + b, 0.0)

    [metal_kernel(name="metal_bias_relu_rows_msl")]
    def metal_bias_relu_rows {
        row_map_body()
    }
}

//! Spectrum off the DFT GEMM's interleaved re/im rows for k < nbins, 0 in the pad columns (the
//! mel GEMM's k-pad reads them): the magnitude sqrt(re^2 + im^2) (SQRT) or the power re^2 + im^2.
[ |> template_struct_instance]
class template MetalSpectrumT {
    @ssbo @binding = 0 @role = "read" reim : array<float>   //!< [nrows x ldin] interleaved re/im
    @ssbo @binding = 1 @role = "write" spec : array<float>  //!< [nrows x ldout]
    @uniform @binding = 2 nbins : uint
    @uniform @binding = 3 ldin : uint
    @uniform @binding = 4 ldout : uint
    @uniform @binding = 5 total : uint      //!< grid guard: nrows * ldout
    @template_constant SQRT : bool = false

    def spectrum_body {
        let gid = gl_GlobalInvocationID.x
        if (gid >= total) {
            return
        }
        let r = gid / ldout
        let k = gid % ldout
        if (k >= nbins) {
            spec[gid] = 0.0
            return
        }
        let re = reim[r * ldin + k * 2u]
        let im = reim[r * ldin + k * 2u + 1u]
        static_if (SQRT) {
            spec[gid] = sqrt(re * re + im * im)
        } else {
            spec[gid] = re * re + im * im
        }
    }
}

[metal_dispatch(name = "enc_g4a_mag", pso = "g_pf_pso_g4a_mag", tg = 64, grid = "total/64", params = "total : int64")]
class MetalG4aMag : MetalSpectrumT {
    override SQRT = true

    [metal_kernel(name="metal_g4a_mag_msl")]
    def metal_g4a_mag {
        spectrum_body()
    }
}

[metal_dispatch(name = "enc_q3a_pow", pso = "g_pf_pso_q3a_pow", tg = 64, grid = "total/64", params = "total : int64")]
class MetalQ3aPow : MetalSpectrumT {
    [metal_kernel(name="metal_q3a_pow_msl")]
    def metal_q3a_pow {
        spectrum_body()
    }
}

//! The mel's log stage in place: x = log(max(x, floor)) — natural log, the CPU form.
[metal_dispatch(name = "enc_g4a_logmel", pso = "g_pf_pso_g4a_logmel", tg = 64, grid = "total/64", params = "total : int64")]
class MetalG4aLogMel {
    @ssbo @binding = 0 @role = "readwrite" x : array<float>
    @uniform @binding = 1 total : uint      //!< grid guard
    @uniform @binding = 2 mfloor : float

    [metal_kernel(name="metal_g4a_logmel_msl")]
    def metal_g4a_logmel {
        let gid = gl_GlobalInvocationID.x
        if (gid >= total) {
            return
        }
        x[gid] = log(max(x[gid], mfloor))
    }
}

struct TwRowsArgs {
    width : uint    //!< the OUTPUT row's width
    total : uint    //!< output elements
}

//! The whisper-class projector tails' pair pool: y [rows x width] the mean of x's row pairs [2 rows x width].
[metal_dispatch(name = "enc_tw_pool2", pso = "g_pf_pso_tw_pool2", tg = 64, grid = "total/64", params = "total : int64")]
class MetalTwPool2 {
    @ssbo @binding = 0 @role = "read" x : array<float>
    @ssbo @binding = 1 @role = "write" y : array<float>
    @uniform @binding = 2 ka : TwRowsArgs

    [metal_kernel(name="metal_tw_pool2_msl")]
    def metal_tw_pool2 {
        let gid = gl_GlobalInvocationID.x
        if (gid >= ka.total) {
            return
        }
        let at = (gid / ka.width) * 2u * ka.width + gid % ka.width
        y[gid] = 0.5 * (x[at] + x[at + ka.width])
    }
}

struct TwPool2dArgs {
    width : uint    //!< a row's width
    gw : uint       //!< the source grid's width, in rows
    k : uint        //!< the pool's side
    ow : uint       //!< the pooled grid's width
    total : uint    //!< output elements
}

//! A vision tail's mean pool: y [oh*ow x width] the mean of each k x k block of the row grid x [gh*gw x width].
[metal_dispatch(name = "enc_tw_pool2d", pso = "g_pf_pso_tw_pool2d", tg = 64, grid = "total/64", params = "total : int64")]
class MetalTwPool2d {
    @ssbo @binding = 0 @role = "read" x : array<float>
    @ssbo @binding = 1 @role = "write" y : array<float>
    @uniform @binding = 2 ka : TwPool2dArgs

    [metal_kernel(name="metal_tw_pool2d_msl")]
    def metal_tw_pool2d {
        let gid = gl_GlobalInvocationID.x
        if (gid >= ka.total) {
            return
        }
        let o = gid / ka.width
        let c = gid % ka.width
        let oy = o / ka.ow
        let ox = o % ka.ow
        var sum = 0.0
        var dy = 0u
        while (dy < ka.k) {
            var dx = 0u
            while (dx < ka.k) {
                sum += x[((oy * ka.k + dy) * ka.gw + ox * ka.k + dx) * ka.width + c]
                dx++
            }
            dy++
        }
        y[gid] = sum / float(ka.k * ka.k)
    }
}

//! The ultravox projector's gate: y [rows x width] = silu(second half of x's row) * its first half, x [rows x 2 width].
[metal_dispatch(name = "enc_tw_swiglu_rows", pso = "g_pf_pso_tw_swiglu_rows", tg = 64, grid = "total/64", params = "total : int64")]
class MetalTwSwigluRows {
    @ssbo @binding = 0 @role = "read" x : array<float>
    @ssbo @binding = 1 @role = "write" y : array<float>
    @uniform @binding = 2 ka : TwRowsArgs

    [metal_kernel(name="metal_tw_swiglu_rows_msl")]
    def metal_tw_swiglu_rows {
        let gid = gl_GlobalInvocationID.x
        if (gid >= ka.total) {
            return
        }
        let at = (gid / ka.width) * 2u * ka.width + gid % ka.width
        let g = x[at + ka.width]
        y[gid] = (g / (1.0 + exp(-g))) * x[at]
    }
}

struct CnMelNormArgs {
    nmel : uint
    nlen : uint     //!< frames the mel holds
    valid : uint    //!< frames of real audio: the statistics' span, the rows past it zeroed
    mfloor : float
}

//! Canary's log and per-feature normalization over the mel GEMM's sums [rows x nmel], in place, one thread a mel
//! feature: log(sum + floor), the mean and the Bessel deviation over the valid frames, every valid frame normalized
//! and the frames past them zero - the CPU form's three passes.
[metal_dispatch(name = "enc_cn_melnorm", pso = "g_pf_pso_cn_melnorm", tg = 64, grid = "nmel/64", params = "nmel : int64")]
class MetalCnMelNorm {
    @ssbo @binding = 0 @role = "readwrite" mel : array<float>   //!< [rows x nmel]
    @uniform @binding = 1 ka : CnMelNormArgs

    [metal_kernel(name="metal_cn_melnorm_msl")]
    def metal_cn_melnorm {
        let j = gl_GlobalInvocationID.x
        if (j >= ka.nmel) {
            return
        }
        var sum = 0.0
        var i = 0u
        while (i < ka.valid) {
            let v = log(mel[i * ka.nmel + j] + ka.mfloor)
            mel[i * ka.nmel + j] = v
            sum += v
            i++
        }
        let mean = sum / float(max(ka.valid, 1u))
        var sq = 0.0
        i = 0u
        while (i < ka.valid) {
            let dv = mel[i * ka.nmel + j] - mean
            sq += dv * dv
            i++
        }
        let denom = sqrt(sq / max(float(ka.valid) - 1.0, 1.0)) + 1e-5
        i = 0u
        while (i < ka.nlen) {
            mel[i * ka.nmel + j] = i < ka.valid ? (mel[i * ka.nmel + j] - mean) / denom : 0.0
            i++
        }
    }
}

//! The subsample conv's LayerNorm(channels, weight-only) + ReLU: one thread per row —
//! mean/variance over ``dim`` at ``ldin`` stride, compact ``dim``-wide rows out at ``ldout``.
[metal_dispatch(name = "enc_g4a_lnrelu", pso = "g_pf_pso_g4a_lnrelu", tg = 64, grid = "nrows/64", params = "nrows : int64")]
class MetalG4aLnRelu {
    @ssbo @binding = 0 @role = "read" x : array<float>      //!< [nrows x ldin]
    @ssbo @binding = 1 @role = "write" y : array<float>     //!< [nrows x ldout]
    @ssbo @binding = 2 @role = "weight" @off = "woff" w : array<float>   //!< [dim]
    @uniform @binding = 3 dim : uint
    @uniform @binding = 4 ldin : uint
    @uniform @binding = 5 ldout : uint
    @uniform @binding = 6 nrows : uint
    @uniform @binding = 7 eps : float

    [metal_kernel(name="metal_g4a_lnrelu_msl")]
    def metal_g4a_lnrelu {
        let r = gl_GlobalInvocationID.x
        if (r >= nrows) {
            return
        }
        var mean = 0.0
        var c = 0u
        while (c < dim) {
            mean += x[r * ldin + c]
            c++
        }
        mean /= float(dim)
        var vv = 0.0
        c = 0u
        while (c < dim) {
            let dv = x[r * ldin + c] - mean
            vv += dv * dv
            c++
        }
        let inv = rsqrt(vv / float(dim) + eps)
        c = 0u
        while (c < dim) {
            let nv = (x[r * ldin + c] - mean) * inv * w[c]
            y[r * ldout + c] = max(nv, 0.0)
            c++
        }
    }
}


//! The qwen3a conv-frontend feature shuffle: feat[ci*13 + ow][h + 16*ch] = y3[ci-chunk row
//! (h*13 + ow), channel ch] off the ncp-strided conv3 slab — one thread per feat element.
[metal_dispatch(name = "enc_q3a_shuffle", pso = "g_pf_pso_q3a_shuffle", tg = 64, grid = "total/64", params = "total : int64")]
class MetalQ3aShuffle {
    @ssbo @binding = 0 @role = "read" y3 : array<float>     //!< [n_chunks x mp3 x ncp]
    @ssbo @binding = 1 @role = "write" feat : array<float>  //!< [npos x featw]
    @uniform @binding = 2 featw : uint      //!< 16 * nch
    @uniform @binding = 3 nch : uint
    @uniform @binding = 4 ncp : uint
    @uniform @binding = 5 mp3ncp : uint     //!< one chunk's slab stride (mp3 * ncp)
    @uniform @binding = 6 total : uint      //!< grid guard: npos * featw

    [metal_kernel(name="metal_q3a_shuffle_msl")]
    def metal_q3a_shuffle {
        let gid = gl_GlobalInvocationID.x
        if (gid >= total) {
            return
        }
        let row = gid / featw
        let j = gid % featw
        let ci = row / 13u
        let ow = row % 13u
        let h = j % 16u
        let ch = j / 16u
        feat[gid] = y3[ci * mp3ncp + (h * 13u + ow) * ncp + ch]
    }
}


//! The whisper-shape mel finish, transposed into the mel-major plane:
//! out[j*n_len + i] = log(max(raw[i*n_mel + j], floor)) * scale (scale = log10 e).
[metal_dispatch(name = "enc_q3a_logmel_t", pso = "g_pf_pso_q3a_logmel_t", tg = 64, grid = "total/64", params = "total : int64")]
class MetalQ3aLogMelT {
    @ssbo @binding = 0 @role = "read" raw : array<float>    //!< [n_len x n_mel] frame-major
    @ssbo @binding = 1 @role = "write" y : array<float>     //!< [n_mel x n_len] mel-major
    @uniform @binding = 2 nmel : uint
    @uniform @binding = 3 nlen : uint
    @uniform @binding = 4 mfloor : float
    @uniform @binding = 5 mscale : float
    @uniform @binding = 6 total : uint      //!< grid guard: n_len * n_mel

    [metal_kernel(name="metal_q3a_logmel_t_msl")]
    def metal_q3a_logmel_t {
        let gid = gl_GlobalInvocationID.x
        if (gid >= total) {
            return
        }
        let i = gid / nmel
        let j = gid % nmel
        y[j * nlen + i] = log(max(raw[gid], mfloor)) * mscale
    }
}

//! The qwen3a conv-frontend epilogue: x += cout bias + the per-chunk position row
//! (positions reset every 13 rows) — one thread per element.
[metal_dispatch(name = "enc_q3a_finish", pso = "g_pf_pso_q3a_finish", tg = 64, grid = "total/64", params = "total : int64")]
class MetalQ3aFinish {
    @ssbo @binding = 0 @role = "readwrite" x : array<float>     //!< [npos x d]
    @ssbo @binding = 1 @role = "weight" @off = "boff" bias : array<float>   //!< [d]
    @ssbo @binding = 2 @role = "weight" @off = "poff" pos : array<float>    //!< [13 x d]
    @uniform @binding = 3 d : uint
    @uniform @binding = 4 total : uint      //!< grid guard: npos * d

    [metal_kernel(name="metal_q3a_finish_msl")]
    def metal_q3a_finish {
        let gid = gl_GlobalInvocationID.x
        if (gid >= total) {
            return
        }
        let c = gid % d
        let row = gid / d
        x[gid] += bias[c] + pos[(row % 13u) * d + c]
    }
}

//! Scaled residual add g += u * ascale — the macaron half-step (enc_add's form is (g+u)*scale).
[metal_dispatch(name = "enc_g4a_axpy", pso = "g_pf_pso_g4a_axpy", tg = 64, grid = "total/64", params = "total : int64")]
class MetalG4aAxpy {
    @ssbo @binding = 0 @role = "readwrite" g : array<float>
    @ssbo @binding = 1 @role = "read" u : array<float>
    @uniform @binding = 2 total : uint      //!< grid guard
    @uniform @binding = 3 ascale : float

    [metal_kernel(name="metal_g4a_axpy_msl")]
    def metal_g4a_axpy {
        let gid = gl_GlobalInvocationID.x
        if (gid >= total) {
            return
        }
        g[gid] += u[gid] * ascale
    }
}

//! Interleave two [npos x d] panels into the [npos x 2d] MTP eh_proj input: rows are [enorm(embed) ; hnorm(hidden)].
[metal_dispatch(name = "pf_enc_cat2", pso = "g_pf_pso_cat2", tg = 64, grid = "total/64", params = "total : int64")]
class MetalPfCat2 {
    @ssbo @binding = 0 @role = "write" y : array<float>
    @ssbo @binding = 1 @role = "read" a : array<float>
    @ssbo @binding = 2 @role = "read" b : array<float>
    @uniform @binding = 3 d : uint
    @uniform @binding = 4 total : uint      //!< grid guard: npos * d

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



var private g_pf_failed = false   //!< prefill PSO bring-up failed (the device latch is common's g_failed)
//! per-prefill transients, reused across calls (prefill orchestration is serial)
var private @scratch g_pf_bks : array<MetalBuffer?>
var private @scratch g_pf_bvs : array<MetalBuffer?>
var private @scratch g_pf_cbs : array<MetalCommandBuffer?>
var private g_pf_pool : MetalBufferPool
var private g_pf_upool : MetalBufferPool     //!< UNTRACKED pool: uniforms + rope tables (GPU-read-only)
let PF_NEEDS_OK = (MetalNeed.neox | MetalNeed.qkv_bias | MetalNeed.qk_norm | MetalNeed.qd_neq_dim |
    MetalNeed.softcap | MetalNeed.sliding | MetalNeed.pre_post_norm | MetalNeed.geglu |
    MetalNeed.v_norm | MetalNeed.attn_scale | MetalNeed.out_scale |
    MetalNeed.sinks | MetalNeed.out_bias | MetalNeed.partial_rope | MetalNeed.q_gated |
    MetalNeed.ple)

var private g_min_npos = 64l
var private g_max_npos = 2048l          //!< naive-attention P footprint cap: heads x npos^2 floats
var private g_prefills = 0l
var private g_pf_inited = false

//! the resident dev-W cache and its seeded set key on plane addresses and hold region
//! buffers - a weights-epoch drop releases those buffers, so the cache dies with them
[init]
def private pf_devw_reload_prep_register {
    register_reload_prep(@@pf_devw_res_release)
}
var private g_pf_pso_rope : MetalComputePipeline?
var private g_pf_pso_qkr : MetalComputePipeline?     //!< fused qk_norm+rope (K rows and twin-less panels)
var private g_pf_pso_qkr_hx : MetalComputePipeline?  //!< its f16 dual-store stamp (the q panel)
var private g_pf_pso_addbias : MetalComputePipeline?
var private g_pf_pso_qk : MetalComputePipeline?
var private g_pf_pso_rowstat : MetalComputePipeline?
var private g_pf_pso_av : MetalComputePipeline?
var private g_pf_pso_cat2 : MetalComputePipeline?
var private g_pf_pso_twr_flash : MetalComputePipeline?   //!< the tower flash (DK=DV=72 monomorph)
var private g_pf_pso_twr_kvhc : MetalComputePipeline?    //!< its K/V per-head-contiguous f16 restride
var private g_pf_pso_twr_win : MetalComputePipeline?     //!< the qwen2.5 ViT per-window attention
var private g_pf_pso_fc_pack : MetalComputePipeline?     //!< the FastConformer rel-pos attention's per-head operand pack
var private g_pf_pso_fc_softmax : MetalComputePipeline?  //!< its shift-add + scale + softmax rows
var private g_pf_pso_fc_unpack : MetalComputePipeline?   //!< its AV rows back to the compact output
var private g_pf_pso_fc_twin : MetalComputePipeline?     //!< the all-heads form's half operands, under the attn_dev crown
var private g_pf_pso_fc_pexp : MetalComputePipeline?     //!< its rel-shift softmax rows
var private g_pf_pso_cn_dw : MetalComputePipeline?       //!< its centered depthwise k9 + folded BN + SiLU
var private g_pf_pso_pk_dw : MetalComputePipeline?       //!< the same on parakeet's tap-major taps
var private g_pf_pso_pk_conv0 : MetalComputePipeline?    //!< the parakeet front: the first conv with its ReLU
var private g_pf_pso_pk_dw2d : MetalComputePipeline?     //!< its depthwise stride-2 conv
var private g_pf_pso_pk_xf : MetalComputePipeline?       //!< its feature rows for the pre-projection
var private g_pf_pso_cn_melnorm : MetalComputePipeline?  //!< canary's log and per-feature normalization over the mel
var private g_pf_pso_tw_pool2 : MetalComputePipeline?    //!< the whisper-class projector tails' pair pool
var private g_pf_pso_tw_swiglu_rows : MetalComputePipeline?   //!< the ultravox projector's row-split gate
var private g_pf_pso_tw_pool2d : MetalComputePipeline?   //!< a vision tail's k x k mean pool over the row grid
var private g_pf_pso_bias_relu : MetalComputePipeline?   //!< a 1x1 conv's bias and ReLU over its GEMM's rows
var private g_pf_pso_g4a_attn : MetalComputePipeline?    //!< the gemma4a sliding-window RPE attention
var private g_pf_pso_g4a_glu : MetalComputePipeline?     //!< its conv-module GLU halves
var private g_pf_pso_g4a_dw : MetalComputePipeline?      //!< its causal depthwise k=5
var private g_pf_pso_g4a_silu : MetalComputePipeline?    //!< in-place SiLU
var private g_pf_pso_g4a_axpy : MetalComputePipeline?    //!< the macaron half-step residual
var private g_pf_pso_q3a_shuffle : MetalComputePipeline? //!< the qwen3a conv feature shuffle
var private g_pf_pso_q3a_finish : MetalComputePipeline?  //!< its bias + per-chunk position epilogue
var private g_pf_pso_q3a_pow : MetalComputePipeline?     //!< power spectrum off the DFT rows
var private g_pf_pso_q3a_logmel_t : MetalComputePipeline? //!< the mel-major log10 finish
var private g_pf_pso_g4a_mag : MetalComputePipeline?     //!< magnitude spectrum off the DFT rows
var private g_pf_pso_g4a_logmel : MetalComputePipeline?  //!< the mel's log stage
var private g_pf_pso_g4a_lnrelu : MetalComputePipeline?  //!< subsample LayerNorm(channels)+ReLU
var private g_pf_pso_mm_t : MetalComputePipeline?  //!< the mul_mm TENSOR twin; the base is the shared g_pso_mm
var private g_pf_pso_mm_th : MetalComputePipeline?  //!< its half-X stamp (the converted f16 activation panel)
var private g_pf_pso_mm_t128 : MetalComputePipeline?
var private g_pf_pso_mm_th128 : MetalComputePipeline?
var private g_pf_pso_mm_thdb128 : MetalComputePipeline?   //!< the double-buffered staging twin (the dbuf A/B)
var private g_pf_pso_mm_thdb : MetalComputePipeline?      //!< its 32-row stamp
var private g_pf_pso_half_rows : MetalComputePipeline?
var private g_pf_bxh : MetalBuffer?     //!< per-forward f16 activation scratch panel (null = the f32 path)
var private g_pf_bxh2 : MetalBuffer?
var private g_pf_bxh_flip : bool
var private g_pf_bxh_bytes : uint64     //!< acquired panel capacity — pf_cvt_panel's fit check
var private g_pf_pso_dq_q8 : MetalComputePipeline?   //!< q8 blob -> device f16 W panel (the dev-W pass)
var private g_pf_pso_dq_k4 : MetalComputePipeline?   //!< the K-quant dequant passes onto the same panel
var private g_pf_pso_dq_k5 : MetalComputePipeline?
var private g_pf_pso_dq_k6 : MetalComputePipeline?
//! the split-scale formats' dev-W dequant passes (iq4xs, iq4nl, k3, iq3s, iq3xxs, k2, iq2s, iq2xs, iq2xxs)
var private g_pf_pso_dq_kq44 : MetalComputePipeline?
var private g_pf_pso_dq_kq45 : MetalComputePipeline?
var private g_pf_pso_dq_kq40 : MetalComputePipeline?
var private g_pf_pso_dq_kq3 : MetalComputePipeline?
var private g_pf_pso_dq_kq33 : MetalComputePipeline?
var private g_pf_pso_dq_kq34 : MetalComputePipeline?
var private g_pf_pso_dq_kq2 : MetalComputePipeline?
var private g_pf_pso_dq_kq23 : MetalComputePipeline?
var private g_pf_pso_dq_kq24 : MetalComputePipeline?
var private g_pf_pso_dq_kq25 : MetalComputePipeline?
var private g_pf_pso_hmm_th : MetalComputePipeline?  //!< all-device half x half mul_mm reading that panel
var private g_pf_pso_hmm_th128 : MetalComputePipeline?
var private g_pf_pso_hmm_th128_l1 : MetalComputePipeline?   //!< its 1-D-grid twin (the launch-order A/B)
var private g_pf_bwh : MetalBuffer?     //!< per-forward f16 W panels (dev-W form), double-buffered like bxh
var private g_pf_bwh2 : MetalBuffer?
struct private DevwResEntry {
    buf : MetalBuffer?
    boff : uint64        //!< site base within the panel buffer (0 for runtime-owned panels)
    owned : bool         //!< runtime-allocated (released at shutdown) vs a baked image plane
    bytes : uint64   //!< the panel bytes the key was seeded for - a hit must cover the request
}

//! one key formula for every seed and lookup: base buffer-handle address + the site offset in
//! the plane's OWN unit (q8: bytes, kq: elems), fmt-tagged so the two unit spaces cannot collide
def private pf_devw_key(base : uint64; off : uint64; fmt : int) : uint64 {
    return (base + off) * 8ul + uint64(fmt)
}
var private g_pf_devw_res : table<uint64; DevwResEntry>   //!< RESIDENT half panels - the dev-W sites, and a routed layer's router rows uncharged - key = blob ptr + site offset (in-buffer, so globally unique), filled once, read every forward
var private g_pf_devw_res_bytes : uint64   //!< runtime-OWNED bytes only — baked planes are file-backed and free
var private g_pf_devw_res_full = false   //!< budget exhausted once — later sites stay on scratch, warned once
var private @scratch g_pf_devw_seeded : table<uint64; bool>   //!< baked-table seeding, once per mapped plane
var private g_pf_bwh_flip : bool
var private g_pf_bwh_bytes : uint64
var private g_pf_pso_bf16_mm : MetalComputePipeline?   //!< native-BF16 A twin (E-series model_proj)
var private g_pf_pso_bf16_mm_th : MetalComputePipeline? //!< its half-X stamp (tensor-crowned only)
var private g_pf_pso_bf16_mm_t128 : MetalComputePipeline?
var private g_pf_pso_bf16_mm_th128 : MetalComputePipeline?
var private g_pf_bf16_mm_tensor : bool   //!< crowned tensor twin selected (no tgmem bind)
var private g_pf_mm_tensor : bool        //!< crowned tensor twin selected for the production q8 mul_mm
var private g_pf_tensor_ok : bool        //!< the box compiles the mpp tensor kernels (probed at init)
var private g_pf_moe_mm_q8_tensor : bool //!< "moe_mulmm_q8" crowned (contiguous sites only)
var private g_pf_pso_moe_mm_q8_t : MetalComputePipeline?
var private g_pf_pso_moe_mm_q8_th : MetalComputePipeline?
var private g_pf_moe_mm_mx4_tensor : bool //!< "moe_mulmm_mx4" crowned (contiguous sites only)
var private g_pf_pso_moe_mm_mx4_t : MetalComputePipeline?
var private g_pf_pso_moe_mm_mx4_th : MetalComputePipeline?
var private g_pf_pso_moe_gx : MetalComputePipeline?   //!< gather-X materialization (up/gate contiguous form)
var private g_pf_pso_moe_mm_q51_t : MetalComputePipeline?   //!< moe q5_1 twins (contiguous rows; the k4 crown proxies — same staging form class)
var private g_pf_pso_moe_mm_q51_th : MetalComputePipeline?
var private g_pf_pso_moe_mm_q51_th128 : MetalComputePipeline?
var private g_pf_pso_moe_mm_q51_thr : MetalComputePipeline?
var private g_pf_pso_moe_mm_iq4nl32_t : MetalComputePipeline?   //!< moe iq4nl32 twins (the same staging form as q51; the k4 crown proxies)
var private g_pf_pso_moe_mm_iq4nl32_th : MetalComputePipeline?
var private g_pf_pso_moe_mm_iq4nl32_th128 : MetalComputePipeline?
var private g_pf_pso_moe_mm_iq4nl32_thr : MetalComputePipeline?
var private g_pf_pso_moe_mm_k4_t : MetalComputePipeline?   //!< moe kq twins (contiguous rows; dense kq crowns gate)
var private g_pf_pso_moe_mm_k4_th : MetalComputePipeline?
var private g_pf_pso_moe_mm_k5_t : MetalComputePipeline?
var private g_pf_pso_moe_mm_k5_th : MetalComputePipeline?
var private g_pf_pso_moe_mm_k6_t : MetalComputePipeline?
var private g_pf_pso_moe_mm_k6_th : MetalComputePipeline?
var private g_pf_pso_moe_mm_q8_th128 : MetalComputePipeline?   //!< moe tall pair (128-floor + remainder)
var private g_pf_pso_moe_mm_q8_thr : MetalComputePipeline?
var private g_pf_pso_moe_mm_mx4_th128 : MetalComputePipeline?
var private g_pf_pso_moe_mm_mx4_thr : MetalComputePipeline?
var private g_pf_pso_moe_mm_k4_th128 : MetalComputePipeline?
var private g_pf_pso_moe_mm_k4_thr : MetalComputePipeline?
var private g_pf_pso_moe_mm_k5_th128 : MetalComputePipeline?
var private g_pf_pso_moe_mm_k5_thr : MetalComputePipeline?
var private g_pf_pso_moe_mm_k6_th128 : MetalComputePipeline?
var private g_pf_pso_moe_mm_k6_thr : MetalComputePipeline?
var private g_pf_pso_moe_mm_k3_t : MetalComputePipeline?   //!< moe split-format twins (contiguous rows; the toolchain probe gates)
var private g_pf_pso_moe_mm_k3_th : MetalComputePipeline?
var private g_pf_pso_moe_mm_k3_th128 : MetalComputePipeline?
var private g_pf_pso_moe_mm_k3_thr : MetalComputePipeline?
var private g_pf_pso_moe_mm_iq4xs_t : MetalComputePipeline?
var private g_pf_pso_moe_mm_iq4xs_th : MetalComputePipeline?
var private g_pf_pso_moe_mm_iq4xs_th128 : MetalComputePipeline?
var private g_pf_pso_moe_mm_iq4xs_thr : MetalComputePipeline?
var private g_pf_pso_moe_mm_q40_t : MetalComputePipeline?
var private g_pf_pso_moe_mm_q40_th : MetalComputePipeline?
var private g_pf_pso_moe_mm_q40_th128 : MetalComputePipeline?
var private g_pf_pso_moe_mm_q40_thr : MetalComputePipeline?
var private g_pf_pso_moe_mm_iq3s_t : MetalComputePipeline?
var private g_pf_pso_moe_mm_iq3s_th : MetalComputePipeline?
var private g_pf_pso_moe_mm_iq3s_th128 : MetalComputePipeline?
var private g_pf_pso_moe_mm_iq3s_thr : MetalComputePipeline?
var private g_pf_pso_moe_mm_iq3xxs_t : MetalComputePipeline?
var private g_pf_pso_moe_mm_iq3xxs_th : MetalComputePipeline?
var private g_pf_pso_moe_mm_iq3xxs_th128 : MetalComputePipeline?
var private g_pf_pso_moe_mm_iq3xxs_thr : MetalComputePipeline?
var private g_pf_pso_moe_mm_iq2s_t : MetalComputePipeline?
var private g_pf_pso_moe_mm_iq2s_th : MetalComputePipeline?
var private g_pf_pso_moe_mm_iq2s_th128 : MetalComputePipeline?
var private g_pf_pso_moe_mm_iq2s_thr : MetalComputePipeline?
var private g_pf_pso_moe_mm_iq2xs_t : MetalComputePipeline?
var private g_pf_pso_moe_mm_iq2xs_th : MetalComputePipeline?
var private g_pf_pso_moe_mm_iq2xs_th128 : MetalComputePipeline?
var private g_pf_pso_moe_mm_iq2xs_thr : MetalComputePipeline?
var private g_pf_pso_moe_mm_iq2xxs_t : MetalComputePipeline?
var private g_pf_pso_moe_mm_iq2xxs_th : MetalComputePipeline?
var private g_pf_pso_moe_mm_iq2xxs_th128 : MetalComputePipeline?
var private g_pf_pso_moe_mm_iq2xxs_thr : MetalComputePipeline?
var private g_pf_kq_mm4_tensor : bool     //!< "kq_mulmm_k4" crowned
var private g_pf_pso_kq_mm4_t : MetalComputePipeline?
var private g_pf_pso_kq_mm4_th : MetalComputePipeline?
var private g_pf_pso_kq_mm4_th128 : MetalComputePipeline?   //!< tall stamp - the DRAM-class panel form
var private g_pf_pso_kq_mm4_thdb : MetalComputePipeline?    //!< the double-buffered twins (the DBUF rail)
var private g_pf_pso_kq_mm4_thdb128 : MetalComputePipeline?
var private g_pf_kq_mm5_tensor : bool     //!< "kq_mulmm_k5" crowned
var private g_pf_pso_kq_mm5_t : MetalComputePipeline?
var private g_pf_pso_kq_mm5_th : MetalComputePipeline?
var private g_pf_pso_kq_mm5_th128 : MetalComputePipeline?
var private g_pf_pso_kq_mm5_thdb : MetalComputePipeline?
var private g_pf_pso_kq_mm5_thdb128 : MetalComputePipeline?
var private g_pf_kq_mm6_tensor : bool     //!< "kq_mulmm_k6" crowned
var private g_pf_pso_kq_mm6_t : MetalComputePipeline?
var private g_pf_pso_kq_mm6_th : MetalComputePipeline?
var private g_pf_pso_kq_mm6_th128 : MetalComputePipeline?
var private g_pf_pso_kq_mm6_thdb : MetalComputePipeline?
var private g_pf_pso_kq_mm6_thdb128 : MetalComputePipeline?
var private g_pf_kq_mm44_tensor : bool    //!< "kq_mulmm_iq4xs" crowned (T + TH stamps only - no tall/db twins yet)
var private g_pf_pso_kq_mm44_t : MetalComputePipeline?
var private g_pf_pso_kq_mm44_th : MetalComputePipeline?
var private g_pf_kq_mm45_tensor : bool    //!< "kq_mulmm_iq4nl" crowned
var private g_pf_pso_kq_mm45_t : MetalComputePipeline?
var private g_pf_pso_kq_mm45_th : MetalComputePipeline?
var private g_pf_kq_mm40_tensor : bool    //!< "kq_mulmm_q40" crowned
var private g_pf_pso_kq_mm40_t : MetalComputePipeline?
var private g_pf_pso_kq_mm40_th : MetalComputePipeline?
var private g_pf_kq_mm3_tensor : bool     //!< "kq_mulmm_k3" crowned
var private g_pf_pso_kq_mm3_t : MetalComputePipeline?
var private g_pf_pso_kq_mm3_th : MetalComputePipeline?
var private g_pf_kq_mm33_tensor : bool    //!< "kq_mulmm_iq3s" crowned
var private g_pf_pso_kq_mm33_t : MetalComputePipeline?
var private g_pf_pso_kq_mm33_th : MetalComputePipeline?
var private g_pf_pso_kq_mm33_th128 : MetalComputePipeline?   //!< the iq3s tall in-kernel-dequant stamp (form 1)
var private g_pf_kq_mm34_tensor : bool    //!< "kq_mulmm_iq3xxs" crowned
var private g_pf_pso_kq_mm34_t : MetalComputePipeline?
var private g_pf_pso_kq_mm34_th : MetalComputePipeline?
var private g_pf_pso_kq_mm34_th128 : MetalComputePipeline?   //!< the iq3xxs tall in-kernel-dequant stamp (form 1)
var private g_pf_kq_mm2_tensor : bool     //!< "kq_mulmm_k2" crowned
var private g_pf_pso_kq_mm2_t : MetalComputePipeline?
var private g_pf_pso_kq_mm2_th : MetalComputePipeline?
var private g_pf_kq_mm23_tensor : bool    //!< "kq_mulmm_iq2s" crowned
var private g_pf_pso_kq_mm23_t : MetalComputePipeline?
var private g_pf_pso_kq_mm23_th : MetalComputePipeline?
var private g_pf_kq_mm24_tensor : bool    //!< "kq_mulmm_iq2xs" crowned
var private g_pf_pso_kq_mm24_t : MetalComputePipeline?
var private g_pf_pso_kq_mm24_th : MetalComputePipeline?
var private g_pf_kq_mm25_tensor : bool    //!< "kq_mulmm_iq2xxs" crowned
var private g_pf_pso_kq_mm25_t : MetalComputePipeline?
var private g_pf_pso_kq_mm25_th : MetalComputePipeline?
var private g_pf_qkmm_tensor : bool       //!< "attn_qkmm" crowned
var private g_pf_pso_qkmm_t : MetalComputePipeline?
var private g_pf_pso_qkmm_th : MetalComputePipeline?
var private g_pf_avmm_tensor : bool       //!< "attn_avmm" crowned
var private g_pf_pso_avmm_t : MetalComputePipeline?
var private g_pf_pso_avmm_tp : MetalComputePipeline?
var private g_pf_pso_avmm_p : MetalComputePipeline?
var private g_pf_attn_dev : bool          //!< "attn_dev" crowned
var private g_pf_pso_qk_dev : MetalComputePipeline?
var private g_pf_pso_qk_dev128 : MetalComputePipeline?
var private g_pf_pso_pexp : MetalComputePipeline?
var private g_pf_pso_av_dev : MetalComputePipeline?
var private g_pf_pso_av_dev128 : MetalComputePipeline?
var private g_pf_pso_av_dev_t : MetalComputePipeline?   //!< the 32-column tail of a head off the 64 lattice
var private g_pf_pso_av_dev128_t : MetalComputePipeline?
var private g_pf_pso_ple_gather : MetalComputePipeline?   //!< PLE pre-step: q8 token-row gather
var private g_pf_pso_ple_gather_k4 : MetalComputePipeline?   //!< PLE pre-step: k4 token-row gather
var private g_pf_pso_ple_gather_k5 : MetalComputePipeline?   //!< PLE pre-step: k5 token-row gather
var private g_pf_pso_ple_gather_q40 : MetalComputePipeline?  //!< PLE pre-step: q40 token-row gather
var private g_pf_pso_ple_finish : MetalComputePipeline?   //!< PLE pre-step: rms + combine, in place
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
var private g_pf_pso_moe_mm_iq4nl32 : MetalComputePipeline?
var private g_mm_legacy = false             //!< set_metal_prefill_mulmm_legacy — tests' exact-parity rail
var private g_pf_logits_cpu = false            //!< set_metal_prefill_logits_cpu — the logits A/B rail
//! Timing-attribution knockout: DASLLAMA_METAL_PREFILL_SKIP / set_metal_prefill_skip names one
//! stage family to leave unencoded. Output is GARBAGE while non-empty; the family names are the
//! g_pf_skip comparisons below.
var private g_pf_skip = ""

//! an "only" family keeps the routing and drops every stage downstream, so it knocks out this one too
def private pf_skip_moe_stage(stage : string) : bool {
    return (g_pf_skip == stage || g_pf_skip == "moe_only_route" || g_pf_skip == "moe_only_router" || g_pf_skip == "moe_only_sel")
}

def private pf_skip_moe_mm : bool => pf_skip_moe_stage("moe_mm") || g_pf_skip == "moe_no_mm_act"

def private pf_skip_moe_act : bool => pf_skip_moe_stage("moe_act") || g_pf_skip == "moe_no_mm_act"

def private pf_skip_moe_reduce : bool => pf_skip_moe_stage("moe_reduce")

//! Test A/B seat: pin the span serving on/off in-process (DASLLAMA_METAL_SPAN's setter twin).
def public set_metal_prefill_span(on : bool) {
    g_pf_env_span = on
}

//! Prefills below this npos decline to the CPU path (submit+readback overhead outweighs the win).
//! Set 1 to force every prefill onto the GPU (tests).
def public set_metal_prefill_min_npos(v : int64) {
    g_min_npos = v
}

def public get_metal_prefill_min_npos() : int64 => g_min_npos

//! Pin the CPU final-norm/classifier step (logits-on-GPU off) — the A/B and fallback rail.
//! `DASLLAMA_METAL_LOGITS=0` pins it process-wide.
def public set_metal_prefill_logits_cpu(v : bool) {
    g_pf_logits_cpu = v
}

//! Pin the legacy quantized-activation GEMM path (q8 X + planar weights) instead of the default
//! mul_mm path (f32 X + q8_0 block blobs). The legacy path is bit-identical to the CPU
//! forward — the tests' exact-parity rail. `DASLLAMA_METAL_MULMM=0` pins it process-wide.
def public set_metal_prefill_mulmm_legacy(v : bool) {
    g_mm_legacy = v
}

//! A/B seat: the GEMV-tail prefill dispatch (small npos % 32 remainders peel onto the mv
//! family) vs the padded-tile path — `DASLLAMA_METAL_MM_TAIL`'s setter twin. Default on.
def public set_metal_prefill_mm_tail(v : bool) {
    g_pf_env_mm_tail = v
}

//! A/B seat: an f16-KV prefill's rows in the session's KV mirror, its attention reading them there, vs the
//! f32 panels and the continuation's host gather - `DASLLAMA_METAL_PF_MIRROR`'s setter twin. Default on.
def public set_metal_prefill_mirror(v : bool) {
    g_pf_env_mirror = v
}

def public get_metal_prefill_mirror() : bool => g_pf_env_mirror

var private g_pf_mirror_served = 0l

//! Lifetime prefill windows served on the mirror-fed form - the form's witness.
def public metal_prefill_mirror_served : int64 => g_pf_mirror_served

//! A/B seat: the half-X activation convert (f16-stream GEMM stamps at >= 256 rows) vs the
//! f32 forms. Default on; a form-identity witness pins it off like the GEMV tail — the
//! npos-gated form swap is the same class of fused-vs-splice spread.
def public set_metal_prefill_half_x(v : bool) {
    g_pf_env_half_x = v
    to_log(LOG_INFO, "dasLLAMA: set_metal_prefill_half_x({v}) - GEMM form override\n")
}

//! A/B seat: the dev-W all-device GEMM form (q8 W dequantized to a device f16 panel per
//! site, >= 256 rows) vs the tg-staged twin. Default on; identity witnesses pin it off with
//! half-X — the same row-gated form-swap class.
def public set_metal_prefill_dev_w(v : bool) {
    g_pf_env_dev_w = v
    to_log(LOG_INFO, "dasLLAMA: set_metal_prefill_dev_w({v}) - GEMM form override\n")
}

//! A/B seat: the tall 128-row M-tile mul_mm stamps (W streamed M/128 times) vs 32-row-only.
//! Default on; identity witnesses pin it off with half-X — the same row-gated form-swap class.
def public set_metal_prefill_tall(v : bool) {
    g_pf_env_tall = v
    to_log(LOG_INFO, "dasLLAMA: set_metal_prefill_tall({v}) - GEMM form override\n")
}

//! A/B seat: the fused qk_norm+rope pass (one panel rewrite + the q twin dual-store) vs the
//! split norm/rope/cvt dispatches. Default on; `DASLLAMA_METAL_QK_ROPE=0` pins it off.
def public set_metal_prefill_qk_rope(v : bool) {
    g_pf_env_qkr = v
    to_log(LOG_INFO, "dasLLAMA: set_metal_prefill_qk_rope({v}) - ew form override\n")
}

//! A/B seat: the 1-D-grid tall hmm twin (launch order forced linear, M-fastest) vs the 2-D
//! grid whose walk order is the driver's. Default off; `DASLLAMA_METAL_GRID1D=1` arms it.
def public set_metal_prefill_grid1d(v : bool) {
    g_pf_env_grid1d = v
    to_log(LOG_INFO, "dasLLAMA: set_metal_prefill_grid1d({v}) - GEMM launch-order override\n")
}

//! A/B seat: last-layer FFN tail narrowing (the final row alone feeds the classifier) vs the
//! full-panel tail. Default on; `DASLLAMA_METAL_LASTROW=0` pins the full tail. Session
//! keep_hidden opts a single forward out regardless.
def public set_metal_prefill_lastrow(v : bool) {
    g_pf_env_lastrow = v
    to_log(LOG_INFO, "dasLLAMA: set_metal_prefill_lastrow({v}) - prefill tail override\n")
}

//! A/B seat: double-buffered W staging on the dense staged GEMMs - the q8 and kq stamps - vs
//! the single-tile forms (the MoE family stays single-tile either way). Default on;
//! `DASLLAMA_METAL_DBUF=0` pins them off. Outputs BIT-equal.
def public set_metal_prefill_dbuf(v : bool) {
    g_pf_env_dbuf = v
    to_log(LOG_INFO, "dasLLAMA: set_metal_prefill_dbuf({v}) - staged GEMM form override\n")
}

//! The prefill knockout rail as an in-process setter (timing attribution — served output is
//! GARBAGE while non-empty): the `DASLLAMA_METAL_PREFILL_SKIP` families above; `""` restores
//! full math. Switchable between measurement windows, same contract as `set_metal_decode_skip`.
def public set_metal_prefill_skip(v : string) {
    g_pf_skip = v
    if (!empty(v)) {
        to_log(LOG_WARNING, "dasLLAMA: set_metal_prefill_skip(\"{v}\") - prefill knockout, served output is GARBAGE\n")
    }
}

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
    return (prefills = g_prefills, declines = g_prefill_declines.total)
}

//! The tower driver's borrow seat: full prefill PSO bring-up, idempotent — the borrowed builders
//! (pf_enc_bf16_mm, enc_add_bias_rows; the attention trio next) bind prefill-owned PSOs.
def public metal_prefill_pso_init : bool {
    return metal_prefill_init()
}

//! Release every GPU object the driver holds — the tests' leak-gate rail (and the reset for
//! refilled weight arrays, same contract as metal_gemm_shutdown). Lazily re-initializes on the
//! next served prefill.
def public metal_prefill_shutdown {
    kcov_fold()   //! PSO pointers recycle across re-init — counts fold into names first
    residency_reset()   //! the set references pooled buffers the drains below release
    pf_devw_res_release()   //! resident dev-W panels die with the driver (and any weight refill re-fills)
    pool_drain(g_pf_pool)
    pool_drain(g_pf_upool)
    release_handles(g_pf_pso_rope, g_pf_pso_qkr, g_pf_pso_qkr_hx, g_pf_pso_addbias, g_pf_pso_cat2, g_pf_pso_half_rows)
    release_handles(g_pf_pso_qk, g_pf_pso_rowstat, g_pf_pso_av, g_pf_pso_qkmm, g_pf_pso_qkmm_t, g_pf_pso_qkmm_th)
    release_handles(g_pf_pso_avmm, g_pf_pso_avmm_p, g_pf_pso_avmm_t, g_pf_pso_avmm_tp)
    release_handles(g_pf_pso_qk_dev, g_pf_pso_qk_dev128, g_pf_pso_pexp, g_pf_pso_av_dev, g_pf_pso_av_dev128)
    release_handles(g_pf_pso_av_dev_t, g_pf_pso_av_dev128_t)
    release_handles(g_pf_pso_fc_twin, g_pf_pso_fc_pexp)
    release_handles(g_pf_pso_mm_t, g_pf_pso_mm_th, g_pf_pso_mm_t128, g_pf_pso_mm_th128, g_pf_pso_mm_thdb, g_pf_pso_mm_thdb128)
    release_handles(g_pf_pso_bf16_mm, g_pf_pso_bf16_mm_th, g_pf_pso_bf16_mm_t128, g_pf_pso_bf16_mm_th128)
    release_handles(g_pf_pso_hmm_th, g_pf_pso_hmm_th128, g_pf_pso_hmm_th128_l1)
    release_handles(g_pf_pso_kq_mm4_t, g_pf_pso_kq_mm4_th, g_pf_pso_kq_mm4_th128, g_pf_pso_kq_mm4_thdb, g_pf_pso_kq_mm4_thdb128)
    release_handles(g_pf_pso_kq_mm5_t, g_pf_pso_kq_mm5_th, g_pf_pso_kq_mm5_th128, g_pf_pso_kq_mm5_thdb, g_pf_pso_kq_mm5_thdb128)
    release_handles(g_pf_pso_kq_mm6_t, g_pf_pso_kq_mm6_th, g_pf_pso_kq_mm6_th128, g_pf_pso_kq_mm6_thdb, g_pf_pso_kq_mm6_thdb128)
    release_handles(g_pf_pso_kq_mm44_t, g_pf_pso_kq_mm44_th, g_pf_pso_kq_mm45_t, g_pf_pso_kq_mm45_th, g_pf_pso_kq_mm40_t, g_pf_pso_kq_mm40_th)
    release_handles(g_pf_pso_kq_mm3_t, g_pf_pso_kq_mm3_th, g_pf_pso_kq_mm2_t, g_pf_pso_kq_mm2_th)
    release_handles(g_pf_pso_kq_mm33_t, g_pf_pso_kq_mm33_th, g_pf_pso_kq_mm33_th128, g_pf_pso_kq_mm34_t, g_pf_pso_kq_mm34_th, g_pf_pso_kq_mm34_th128)
    release_handles(g_pf_pso_kq_mm23_t, g_pf_pso_kq_mm23_th, g_pf_pso_kq_mm24_t, g_pf_pso_kq_mm24_th, g_pf_pso_kq_mm25_t, g_pf_pso_kq_mm25_th)
    release_handles(g_pf_pso_dq_q8, g_pf_pso_dq_k4, g_pf_pso_dq_k5, g_pf_pso_dq_k6)
    release_handles(g_pf_pso_dq_kq44, g_pf_pso_dq_kq45, g_pf_pso_dq_kq40, g_pf_pso_dq_kq3, g_pf_pso_dq_kq2)
    release_handles(g_pf_pso_dq_kq33, g_pf_pso_dq_kq34, g_pf_pso_dq_kq23, g_pf_pso_dq_kq24, g_pf_pso_dq_kq25)
    release_handles(g_pf_pso_moe_count, g_pf_pso_moe_bucket, g_pf_pso_moe_reduce, g_pf_pso_moe_gx, g_pf_pso_swiglu_oai)
    release_handles(g_pf_pso_moe_mm_q8, g_pf_pso_moe_mm_k4, g_pf_pso_moe_mm_k5, g_pf_pso_moe_mm_k6, g_pf_pso_moe_mm_mx4, g_pf_pso_moe_mm_q51, g_pf_pso_moe_mm_iq4nl32)
    release_handles(g_pf_pso_moe_mm_q8_t, g_pf_pso_moe_mm_q8_th, g_pf_pso_moe_mm_q8_th128, g_pf_pso_moe_mm_q8_thr)
    release_handles(g_pf_pso_moe_mm_mx4_t, g_pf_pso_moe_mm_mx4_th, g_pf_pso_moe_mm_mx4_th128, g_pf_pso_moe_mm_mx4_thr)
    release_handles(g_pf_pso_moe_mm_k4_t, g_pf_pso_moe_mm_k4_th, g_pf_pso_moe_mm_k4_th128, g_pf_pso_moe_mm_k4_thr)
    release_handles(g_pf_pso_moe_mm_k5_t, g_pf_pso_moe_mm_k5_th, g_pf_pso_moe_mm_k5_th128, g_pf_pso_moe_mm_k5_thr)
    release_handles(g_pf_pso_moe_mm_k6_t, g_pf_pso_moe_mm_k6_th, g_pf_pso_moe_mm_k6_th128, g_pf_pso_moe_mm_k6_thr)
    release_handles(g_pf_pso_moe_mm_q51_t, g_pf_pso_moe_mm_q51_th, g_pf_pso_moe_mm_q51_th128, g_pf_pso_moe_mm_q51_thr)
    release_handles(g_pf_pso_moe_mm_iq4nl32_t, g_pf_pso_moe_mm_iq4nl32_th, g_pf_pso_moe_mm_iq4nl32_th128, g_pf_pso_moe_mm_iq4nl32_thr)
    release_handles(g_pf_pso_moe_mm_k3_t, g_pf_pso_moe_mm_k3_th, g_pf_pso_moe_mm_k3_th128, g_pf_pso_moe_mm_k3_thr)
    release_handles(g_pf_pso_moe_mm_iq4xs_t, g_pf_pso_moe_mm_iq4xs_th, g_pf_pso_moe_mm_iq4xs_th128, g_pf_pso_moe_mm_iq4xs_thr)
    release_handles(g_pf_pso_moe_mm_q40_t, g_pf_pso_moe_mm_q40_th, g_pf_pso_moe_mm_q40_th128, g_pf_pso_moe_mm_q40_thr)
    release_handles(g_pf_pso_moe_mm_iq3s_t, g_pf_pso_moe_mm_iq3s_th, g_pf_pso_moe_mm_iq3s_th128, g_pf_pso_moe_mm_iq3s_thr)
    release_handles(g_pf_pso_moe_mm_iq3xxs_t, g_pf_pso_moe_mm_iq3xxs_th, g_pf_pso_moe_mm_iq3xxs_th128, g_pf_pso_moe_mm_iq3xxs_thr)
    release_handles(g_pf_pso_moe_mm_iq2s_t, g_pf_pso_moe_mm_iq2s_th, g_pf_pso_moe_mm_iq2s_th128, g_pf_pso_moe_mm_iq2s_thr)
    release_handles(g_pf_pso_moe_mm_iq2xs_t, g_pf_pso_moe_mm_iq2xs_th, g_pf_pso_moe_mm_iq2xs_th128, g_pf_pso_moe_mm_iq2xs_thr)
    release_handles(g_pf_pso_moe_mm_iq2xxs_t, g_pf_pso_moe_mm_iq2xxs_th, g_pf_pso_moe_mm_iq2xxs_th128, g_pf_pso_moe_mm_iq2xxs_thr)
    release_handles(g_pf_pso_twr_flash, g_pf_pso_twr_kvhc, g_pf_pso_twr_win)
    release_handles(g_pf_pso_fc_pack, g_pf_pso_fc_softmax, g_pf_pso_fc_unpack, g_pf_pso_cn_dw, g_pf_pso_pk_dw)
    release_handles(g_pf_pso_pk_conv0, g_pf_pso_pk_dw2d, g_pf_pso_pk_xf, g_pf_pso_bias_relu)
    release_handles(g_pf_pso_cn_melnorm, g_pf_pso_tw_pool2, g_pf_pso_tw_swiglu_rows, g_pf_pso_tw_pool2d)
    release_handles(g_pf_pso_g4a_attn, g_pf_pso_g4a_glu, g_pf_pso_g4a_dw, g_pf_pso_g4a_silu, g_pf_pso_g4a_axpy)
    release_handles(g_pf_pso_g4a_mag, g_pf_pso_g4a_logmel, g_pf_pso_g4a_lnrelu)
    release_handles(g_pf_pso_q3a_shuffle, g_pf_pso_q3a_finish, g_pf_pso_q3a_pow, g_pf_pso_q3a_logmel_t)
    release_handles(g_pf_pso_ple_gather, g_pf_pso_ple_gather_k4, g_pf_pso_ple_gather_k5, g_pf_pso_ple_gather_q40, g_pf_pso_ple_finish)
    g_pf_inited = false
    g_pf_failed = false
}

[cold_path]   //! the moe kq twin PSOs ride the dense kq crowns - the same staging form class
def private pf_compile_moe_kq_twins(var ok : bool&) {
    if (g_pf_kq_mm4_tensor) {
        //! q5_1 has no dense crown; the k4 crown proxies (same file, non-256-divisible axes)
        g_pf_pso_moe_mm_q51_t = compile_stamp(MetalMoeMulMmQ51T_metal_moe_mulmm_q51_t_msl, ok)
        g_pf_pso_moe_mm_q51_th = compile_stamp(MetalMoeMulMmQ51TH_metal_moe_mulmm_q51_t_msl, ok)
        g_pf_pso_moe_mm_q51_th128 = compile_stamp(MetalMoeMulMmQ51TH128_metal_moe_mulmm_q51_t_msl, ok)
        g_pf_pso_moe_mm_q51_thr = compile_stamp(MetalMoeMulMmQ51THR_metal_moe_mulmm_q51_t_msl, ok)
        g_pf_pso_moe_mm_iq4nl32_t = compile_stamp(MetalMoeMulMmIq4nl32T_metal_moe_mulmm_iq4nl32_t_msl, ok)
        g_pf_pso_moe_mm_iq4nl32_th = compile_stamp(MetalMoeMulMmIq4nl32TH_metal_moe_mulmm_iq4nl32_t_msl, ok)
        g_pf_pso_moe_mm_iq4nl32_th128 = compile_stamp(MetalMoeMulMmIq4nl32TH128_metal_moe_mulmm_iq4nl32_t_msl, ok)
        g_pf_pso_moe_mm_iq4nl32_thr = compile_stamp(MetalMoeMulMmIq4nl32THR_metal_moe_mulmm_iq4nl32_t_msl, ok)
        g_pf_pso_moe_mm_k4_t = compile_stamp(MetalMoeMulMmK4T_metal_moe_mulmm_k45_t_msl, ok)
        g_pf_pso_moe_mm_k4_th = compile_stamp(MetalMoeMulMmK4TH_metal_moe_mulmm_k45_t_msl, ok)
        g_pf_pso_moe_mm_k4_th128 = compile_stamp(MetalMoeMulMmK4TH128_metal_moe_mulmm_k45_t_msl, ok)
        g_pf_pso_moe_mm_k4_thr = compile_stamp(MetalMoeMulMmK4THR_metal_moe_mulmm_k45_t_msl, ok)
    }
    if (g_pf_kq_mm5_tensor) {
        g_pf_pso_moe_mm_k5_t = compile_stamp(MetalMoeMulMmK5T_metal_moe_mulmm_k45_t_msl, ok)
        g_pf_pso_moe_mm_k5_th = compile_stamp(MetalMoeMulMmK5TH_metal_moe_mulmm_k45_t_msl, ok)
        g_pf_pso_moe_mm_k5_th128 = compile_stamp(MetalMoeMulMmK5TH128_metal_moe_mulmm_k45_t_msl, ok)
        g_pf_pso_moe_mm_k5_thr = compile_stamp(MetalMoeMulMmK5THR_metal_moe_mulmm_k45_t_msl, ok)
    }
    if (g_pf_kq_mm6_tensor) {
        g_pf_pso_moe_mm_k6_t = compile_stamp(MetalMoeMulMmK6T_metal_moe_mulmm_k6_t_msl, ok)
        g_pf_pso_moe_mm_k6_th = compile_stamp(MetalMoeMulMmK6TH_metal_moe_mulmm_k6_t_msl, ok)
        g_pf_pso_moe_mm_k6_th128 = compile_stamp(MetalMoeMulMmK6TH128_metal_moe_mulmm_k6_t_msl, ok)
        g_pf_pso_moe_mm_k6_thr = compile_stamp(MetalMoeMulMmK6THR_metal_moe_mulmm_k6_t_msl, ok)
    }
    if (g_pf_tensor_ok) {
        g_pf_pso_moe_mm_k3_t = compile_stamp(MetalMoeMulMmK3T_metal_moe_mulmm_k3_t_msl, ok)
        g_pf_pso_moe_mm_k3_th = compile_stamp(MetalMoeMulMmK3TH_metal_moe_mulmm_k3_t_msl, ok)
        g_pf_pso_moe_mm_k3_th128 = compile_stamp(MetalMoeMulMmK3TH128_metal_moe_mulmm_k3_t_msl, ok)
        g_pf_pso_moe_mm_k3_thr = compile_stamp(MetalMoeMulMmK3THR_metal_moe_mulmm_k3_t_msl, ok)
        g_pf_pso_moe_mm_iq4xs_t = compile_stamp(MetalMoeMulMmIq4xsT_metal_moe_mulmm_iq4xs_t_msl, ok)
        g_pf_pso_moe_mm_iq4xs_th = compile_stamp(MetalMoeMulMmIq4xsTH_metal_moe_mulmm_iq4xs_t_msl, ok)
        g_pf_pso_moe_mm_iq4xs_th128 = compile_stamp(MetalMoeMulMmIq4xsTH128_metal_moe_mulmm_iq4xs_t_msl, ok)
        g_pf_pso_moe_mm_iq4xs_thr = compile_stamp(MetalMoeMulMmIq4xsTHR_metal_moe_mulmm_iq4xs_t_msl, ok)
        g_pf_pso_moe_mm_q40_t = compile_stamp(MetalMoeMulMmQ40T_metal_moe_mulmm_iq4xs_t_msl, ok)
        g_pf_pso_moe_mm_q40_th = compile_stamp(MetalMoeMulMmQ40TH_metal_moe_mulmm_iq4xs_t_msl, ok)
        g_pf_pso_moe_mm_q40_th128 = compile_stamp(MetalMoeMulMmQ40TH128_metal_moe_mulmm_iq4xs_t_msl, ok)
        g_pf_pso_moe_mm_q40_thr = compile_stamp(MetalMoeMulMmQ40THR_metal_moe_mulmm_iq4xs_t_msl, ok)
        g_pf_pso_moe_mm_iq3s_t = compile_stamp(MetalMoeMulMmIq3sT_metal_moe_mulmm_iq3s_t_msl, ok)
        g_pf_pso_moe_mm_iq3s_th = compile_stamp(MetalMoeMulMmIq3sTH_metal_moe_mulmm_iq3s_t_msl, ok)
        g_pf_pso_moe_mm_iq3s_th128 = compile_stamp(MetalMoeMulMmIq3sTH128_metal_moe_mulmm_iq3s_t_msl, ok)
        g_pf_pso_moe_mm_iq3s_thr = compile_stamp(MetalMoeMulMmIq3sTHR_metal_moe_mulmm_iq3s_t_msl, ok)
        g_pf_pso_moe_mm_iq3xxs_t = compile_stamp(MetalMoeMulMmIq3xxsT_metal_moe_mulmm_iq3xxs_t_msl, ok)
        g_pf_pso_moe_mm_iq3xxs_th = compile_stamp(MetalMoeMulMmIq3xxsTH_metal_moe_mulmm_iq3xxs_t_msl, ok)
        g_pf_pso_moe_mm_iq3xxs_th128 = compile_stamp(MetalMoeMulMmIq3xxsTH128_metal_moe_mulmm_iq3xxs_t_msl, ok)
        g_pf_pso_moe_mm_iq3xxs_thr = compile_stamp(MetalMoeMulMmIq3xxsTHR_metal_moe_mulmm_iq3xxs_t_msl, ok)
        g_pf_pso_moe_mm_iq2s_t = compile_stamp(MetalMoeMulMmIq2sT_metal_moe_mulmm_iq2s_t_msl, ok)
        g_pf_pso_moe_mm_iq2s_th = compile_stamp(MetalMoeMulMmIq2sTH_metal_moe_mulmm_iq2s_t_msl, ok)
        g_pf_pso_moe_mm_iq2s_th128 = compile_stamp(MetalMoeMulMmIq2sTH128_metal_moe_mulmm_iq2s_t_msl, ok)
        g_pf_pso_moe_mm_iq2s_thr = compile_stamp(MetalMoeMulMmIq2sTHR_metal_moe_mulmm_iq2s_t_msl, ok)
        g_pf_pso_moe_mm_iq2xs_t = compile_stamp(MetalMoeMulMmIq2xsT_metal_moe_mulmm_iq2xs_t_msl, ok)
        g_pf_pso_moe_mm_iq2xs_th = compile_stamp(MetalMoeMulMmIq2xsTH_metal_moe_mulmm_iq2xs_t_msl, ok)
        g_pf_pso_moe_mm_iq2xs_th128 = compile_stamp(MetalMoeMulMmIq2xsTH128_metal_moe_mulmm_iq2xs_t_msl, ok)
        g_pf_pso_moe_mm_iq2xs_thr = compile_stamp(MetalMoeMulMmIq2xsTHR_metal_moe_mulmm_iq2xs_t_msl, ok)
        g_pf_pso_moe_mm_iq2xxs_t = compile_stamp(MetalMoeMulMmIq2xxsT_metal_moe_mulmm_iq2xxs_t_msl, ok)
        g_pf_pso_moe_mm_iq2xxs_th = compile_stamp(MetalMoeMulMmIq2xxsTH_metal_moe_mulmm_iq2xxs_t_msl, ok)
        g_pf_pso_moe_mm_iq2xxs_th128 = compile_stamp(MetalMoeMulMmIq2xxsTH128_metal_moe_mulmm_iq2xxs_t_msl, ok)
        g_pf_pso_moe_mm_iq2xxs_thr = compile_stamp(MetalMoeMulMmIq2xxsTHR_metal_moe_mulmm_iq2xxs_t_msl, ok)
    }
}

[cold_path]
def private metal_prefill_init : bool {   // nolint:STYLE038 — flat one-compile-per-PSO list
    if (g_pf_inited) {
        return true
    }
    if (g_pf_failed) {
        return false
    }
    if (!metal_common_init()) {   //! also arms the hazard-rail knobs on prefill-first runs
        to_log(LOG_WARNING, "dasLLAMA metal prefill: no Metal device - prefill stays on the CPU\n")
        g_pf_failed = true
        return false
    }
    if (!metal_decode_init()) {   //! the family kernel registry — the generated enc_* builders bind ITS PSOs (idempotent)
        g_pf_failed = true
        return false
    }
    TALL_OCC_FLOOR = metal_tall_floor()
    DEVW_SMALL_PANEL = uint64(metal_devw_small_panel_mb()) * 1024ul * 1024ul
    CVT_MIN_ROWS = metal_cvt_min_rows()
    var ok = true
    g_pf_pso_rope = compile_stamp(metal_rope_msl, ok)
    g_pf_pso_qkr = compile_stamp(MetalQkRope_metal_qk_rope_msl, ok)
    g_pf_pso_qkr_hx = compile_stamp(MetalQkRopeHX_metal_qk_rope_msl, ok)
    g_pf_pso_addbias = compile_stamp(metal_add_bias_rows_msl, ok)
    g_pf_pso_qk = compile_stamp(metal_attn_qk_msl, ok)
    g_pf_pso_rowstat = compile_stamp(metal_attn_rowstat_msl, ok)
    g_pf_pso_av = compile_stamp(metal_attn_av_msl, ok)
    g_pf_pso_cat2 = compile_stamp(metal_pf_cat2_msl, ok)
    //! the panel converter serves every half-X family (q8/kq/bf16), crown-independent
    g_pf_pso_half_rows = compile_stamp(metal_half_rows_msl, ok)
    g_pf_mm_tensor = metal_tensor_crowned("mulmm_q8")
    g_pf_tall_crowned = metal_tensor_crowned("hmm_tall")
    if (g_pf_mm_tensor) {
        g_pf_pso_mm_t = compile_stamp(MetalQ8MulMmT_metal_q8_mulmm_t_msl, ok)
        g_pf_pso_mm_th = compile_stamp(MetalQ8MulMmTH_metal_q8_mulmm_t_msl, ok)
        g_pf_pso_mm_t128 = compile_stamp(MetalQ8MulMmT128_metal_q8_mulmm_t_msl, ok)
        g_pf_pso_mm_th128 = compile_stamp(MetalQ8MulMmTH128_metal_q8_mulmm_t_msl, ok)
        g_pf_pso_mm_thdb128 = compile_stamp(MetalQ8MulMmTHDb128_metal_q8_mulmm_t_msl, ok)
        g_pf_pso_mm_thdb = compile_stamp(MetalQ8MulMmTHDb_metal_q8_mulmm_t_msl, ok)
    }
    g_pf_bf16_mm_tensor = metal_tensor_crowned("mulmm_bf16")
    g_pf_pso_bf16_mm = (g_pf_bf16_mm_tensor
        ? compile_stamp(MetalBf16MulMmT_metal_bf16_mulmm_t_msl, ok)
        : compile_stamp(metal_bf16_mulmm_msl, ok))
    if (g_pf_bf16_mm_tensor) {
        g_pf_pso_bf16_mm_th = compile_stamp(MetalBf16MulMmTH_metal_bf16_mulmm_t_msl, ok)
        g_pf_pso_bf16_mm_t128 = compile_stamp(MetalBf16MulMmT128_metal_bf16_mulmm_t_msl, ok)
        g_pf_pso_bf16_mm_th128 = compile_stamp(MetalBf16MulMmTH128_metal_bf16_mulmm_t_msl, ok)
    }
    g_pf_pso_ple_gather = compile_stamp(metal_ple_gather_q8_msl, ok)
    g_pf_pso_ple_gather_k4 = compile_stamp(MetalPleGatherK4_metal_ple_gather_kq_msl, ok)
    g_pf_pso_ple_gather_k5 = compile_stamp(MetalPleGatherK5_metal_ple_gather_kq_msl, ok)
    g_pf_pso_ple_gather_q40 = compile_stamp(MetalPleGatherQ40_metal_ple_gather_kq_msl, ok)
    g_pf_pso_ple_finish = compile_stamp(metal_ple_finish_msl, ok)
    var perr : string
    var probe = pipeline_from_source(g_dev, MetalHalfMulMmTH_metal_half_mulmm_th_msl, MetalHalfMulMmTH_metal_half_mulmm_th_msl_entry, MetalHalfMulMmTH_metal_half_mulmm_th_msl_fastmath, perr)
    g_pf_tensor_ok = probe != null
    release_handle(probe)
    if (!g_pf_tensor_ok) {
        to_log(LOG_WARNING, "dasLLAMA metal prefill: half mul_mm toolchain probe failed - dev-W panels, the dequant passes and the split-format MoE expert twins stay off ({perr})\n")
    }
    g_pf_kq_mm4_tensor = metal_tensor_crowned("kq_mulmm_k4")
    if (g_pf_kq_mm4_tensor) {
        g_pf_pso_kq_mm4_t = compile_stamp(MetalKqMulMmK4T_metal_kq_mulmm_t_msl, ok)
        g_pf_pso_kq_mm4_th = compile_stamp(MetalKqMulMmK4TH_metal_kq_mulmm_t_msl, ok)
        g_pf_pso_kq_mm4_th128 = compile_stamp(MetalKqMulMmK4TH128_metal_kq_mulmm_t_msl, ok)
        g_pf_pso_kq_mm4_thdb = compile_stamp(MetalKqMulMmK4THDb_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm4_thdb128 = compile_stamp(MetalKqMulMmK4THDb128_metal_kq_mulmm_db_msl, ok)
    }
    g_pf_kq_mm5_tensor = metal_tensor_crowned("kq_mulmm_k5")
    if (g_pf_kq_mm5_tensor) {
        g_pf_pso_kq_mm5_t = compile_stamp(MetalKqMulMmK5T_metal_kq_mulmm_t_msl, ok)
        g_pf_pso_kq_mm5_th = compile_stamp(MetalKqMulMmK5TH_metal_kq_mulmm_t_msl, ok)
        g_pf_pso_kq_mm5_th128 = compile_stamp(MetalKqMulMmK5TH128_metal_kq_mulmm_t_msl, ok)
        g_pf_pso_kq_mm5_thdb = compile_stamp(MetalKqMulMmK5THDb_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm5_thdb128 = compile_stamp(MetalKqMulMmK5THDb128_metal_kq_mulmm_db_msl, ok)
    }
    g_pf_kq_mm6_tensor = metal_tensor_crowned("kq_mulmm_k6")
    if (g_pf_kq_mm6_tensor) {
        g_pf_pso_kq_mm6_t = compile_stamp(MetalKqMulMmK6T_metal_kq_mulmm_k6_t_msl, ok)
        g_pf_pso_kq_mm6_th = compile_stamp(MetalKqMulMmK6TH_metal_kq_mulmm_k6_t_msl, ok)
        g_pf_pso_kq_mm6_th128 = compile_stamp(MetalKqMulMmK6TH128_metal_kq_mulmm_k6_t_msl, ok)
        g_pf_pso_kq_mm6_thdb = compile_stamp(MetalKqMulMmK6THDb_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm6_thdb128 = compile_stamp(MetalKqMulMmK6THDb128_metal_kq_mulmm_db_msl, ok)
    }
    pf_compile_kq_iquant_tensor_twins(ok)
    if (g_pf_tensor_ok) {
        g_pf_pso_hmm_th = compile_stamp(MetalHalfMulMmTH_metal_half_mulmm_th_msl, ok)
        g_pf_pso_hmm_th128 = compile_stamp(MetalHalfMulMmTH128_metal_half_mulmm_th_msl, ok)
        g_pf_pso_hmm_th128_l1 = compile_stamp(MetalHalfMulMmTH128L1_metal_half_mulmm_th_msl, ok)
        g_pf_pso_dq_q8 = compile_stamp(metal_dequant_q8h_msl, ok)
        g_pf_pso_dq_k4 = compile_stamp(MetalDequantK4H_metal_dequant_k45_msl, ok)
        g_pf_pso_dq_k5 = compile_stamp(MetalDequantK5H_metal_dequant_k45_msl, ok)
        g_pf_pso_dq_k6 = compile_stamp(metal_dequant_k6h_msl, ok)
        pf_compile_kq_dequant_passes(ok)
    }
    g_pf_pso_qkmm = compile_stamp(metal_attn_qk_mm_msl, ok)
    g_pf_pso_twr_flash = compile_stamp(metal_tower_flash_msl, ok)
    g_pf_pso_twr_kvhc = compile_stamp(metal_tower_kv_hc_msl, ok)
    g_pf_pso_twr_win = compile_stamp(metal_tower_win_msl, ok)
    g_pf_pso_fc_pack = compile_stamp(metal_fc_pack_msl, ok)
    g_pf_pso_fc_softmax = compile_stamp(metal_fc_softmax_msl, ok)
    g_pf_pso_fc_unpack = compile_stamp(metal_fc_unpack_msl, ok)
    g_pf_pso_cn_dw = compile_stamp(metal_cn_dw_msl, ok)
    g_pf_pso_pk_dw = compile_stamp(metal_pk_dw_msl, ok)
    g_pf_pso_pk_conv0 = compile_stamp(metal_pk_conv0_msl, ok)
    g_pf_pso_pk_dw2d = compile_stamp(metal_pk_dw2d_msl, ok)
    g_pf_pso_pk_xf = compile_stamp(metal_pk_xf_msl, ok)
    g_pf_pso_cn_melnorm = compile_stamp(metal_cn_melnorm_msl, ok)
    g_pf_pso_tw_pool2 = compile_stamp(metal_tw_pool2_msl, ok)
    g_pf_pso_tw_swiglu_rows = compile_stamp(metal_tw_swiglu_rows_msl, ok)
    g_pf_pso_tw_pool2d = compile_stamp(metal_tw_pool2d_msl, ok)
    g_pf_pso_bias_relu = compile_stamp(metal_bias_relu_rows_msl, ok)
    g_pf_pso_g4a_attn = compile_stamp(metal_g4a_attn_msl, ok)
    g_pf_pso_g4a_glu = compile_stamp(metal_g4a_glu_msl, ok)
    g_pf_pso_g4a_dw = compile_stamp(metal_g4a_dw_msl, ok)
    g_pf_pso_g4a_silu = compile_stamp(metal_g4a_silu_msl, ok)
    g_pf_pso_g4a_axpy = compile_stamp(metal_g4a_axpy_msl, ok)
    g_pf_pso_q3a_shuffle = compile_stamp(metal_q3a_shuffle_msl, ok)
    g_pf_pso_q3a_finish = compile_stamp(metal_q3a_finish_msl, ok)
    g_pf_pso_q3a_pow = compile_stamp(metal_q3a_pow_msl, ok)
    g_pf_pso_q3a_logmel_t = compile_stamp(metal_q3a_logmel_t_msl, ok)
    g_pf_pso_g4a_mag = compile_stamp(metal_g4a_mag_msl, ok)
    g_pf_pso_g4a_logmel = compile_stamp(metal_g4a_logmel_msl, ok)
    g_pf_pso_g4a_lnrelu = compile_stamp(metal_g4a_lnrelu_msl, ok)
    g_pf_pso_avmm = compile_stamp(MetalAttnAVMm_metal_attn_av_mm_msl, ok)
    g_pf_pso_avmm_p = compile_stamp(MetalAttnAVMmP_metal_attn_av_mm_msl, ok)
    g_pf_qkmm_tensor = metal_tensor_crowned("attn_qkmm")
    if (g_pf_qkmm_tensor) {
        g_pf_pso_qkmm_t = compile_stamp(MetalAttnQKMmT_metal_attn_qk_mm_t_msl, ok)
        g_pf_pso_qkmm_th = compile_stamp(MetalAttnQKMmTH_metal_attn_qk_mm_t_msl, ok)
    }
    g_pf_avmm_tensor = metal_tensor_crowned("attn_avmm")
    if (g_pf_avmm_tensor) {
        g_pf_pso_avmm_t = compile_stamp(MetalAttnAVMmT_metal_attn_av_mm_t_msl, ok)
        g_pf_pso_avmm_tp = compile_stamp(MetalAttnAVMmTP_metal_attn_av_mm_t_msl, ok)
    }
    g_pf_attn_dev = metal_tensor_crowned("attn_dev")
    if (g_pf_attn_dev) {
        g_pf_pso_qk_dev = compile_stamp(MetalAttnQKDev_metal_attn_qk_dev_msl, ok)
        g_pf_pso_qk_dev128 = compile_stamp(MetalAttnQKDev128_metal_attn_qk_dev_msl, ok)
        g_pf_pso_pexp = compile_stamp(metal_attn_pexp_msl, ok)
        g_pf_pso_av_dev = compile_stamp(MetalAttnAVDev_metal_attn_av_dev_msl, ok)
        g_pf_pso_av_dev128 = compile_stamp(MetalAttnAVDev128_metal_attn_av_dev_msl, ok)
        g_pf_pso_av_dev_t = compile_stamp(MetalAttnAVDevTail_metal_attn_av_dev_msl, ok)
        g_pf_pso_av_dev128_t = compile_stamp(MetalAttnAVDev128Tail_metal_attn_av_dev_msl, ok)
        g_pf_pso_fc_twin = compile_stamp(metal_fc_twin_msl, ok)
        g_pf_pso_fc_pexp = compile_stamp(metal_fc_pexp_msl, ok)
    }
    g_pf_pso_moe_count = compile_stamp(metal_moe_count_msl, ok)
    g_pf_pso_moe_bucket = compile_stamp(metal_moe_bucket_msl, ok)
    g_pf_pso_moe_reduce = compile_stamp(metal_moe_reduce_msl, ok)
    g_pf_pso_moe_mm_q8 = compile_stamp(metal_moe_mulmm_q8_msl, ok)
    g_pf_moe_mm_q8_tensor = metal_tensor_crowned("moe_mulmm_q8")
    if (g_pf_moe_mm_q8_tensor) {
        g_pf_pso_moe_mm_q8_t = compile_stamp(MetalMoeMulMmQ8T_metal_moe_mulmm_q8_t_msl, ok)
        g_pf_pso_moe_mm_q8_th = compile_stamp(MetalMoeMulMmQ8TH_metal_moe_mulmm_q8_t_msl, ok)
        g_pf_pso_moe_mm_q8_th128 = compile_stamp(MetalMoeMulMmQ8TH128_metal_moe_mulmm_q8_t_msl, ok)
        g_pf_pso_moe_mm_q8_thr = compile_stamp(MetalMoeMulMmQ8THR_metal_moe_mulmm_q8_t_msl, ok)
    }
    g_pf_pso_moe_mm_k4 = compile_stamp(metal_moe_mulmm_k4_msl, ok)
    g_pf_pso_moe_mm_k5 = compile_stamp(metal_moe_mulmm_k5_msl, ok)
    g_pf_pso_moe_mm_k6 = compile_stamp(metal_moe_mulmm_k6_msl, ok)
    g_pf_pso_swiglu_oai = compile_stamp(MetalSwigluOaiPf_metal_swiglu_oai_msl, ok)
    g_pf_pso_moe_mm_mx4 = compile_stamp(metal_moe_mulmm_mx4_msl, ok)
    g_pf_moe_mm_mx4_tensor = metal_tensor_crowned("moe_mulmm_mx4")
    if (g_pf_moe_mm_mx4_tensor) {
        g_pf_pso_moe_mm_mx4_t = compile_stamp(MetalMoeMulMmMx4T_metal_moe_mulmm_mx4_t_msl, ok)
        g_pf_pso_moe_mm_mx4_th = compile_stamp(MetalMoeMulMmMx4TH_metal_moe_mulmm_mx4_t_msl, ok)
        g_pf_pso_moe_mm_mx4_th128 = compile_stamp(MetalMoeMulMmMx4TH128_metal_moe_mulmm_mx4_t_msl, ok)
        g_pf_pso_moe_mm_mx4_thr = compile_stamp(MetalMoeMulMmMx4THR_metal_moe_mulmm_mx4_t_msl, ok)
    }
    g_pf_pso_moe_mm_q51 = compile_stamp(metal_moe_mulmm_q51_msl, ok)
    g_pf_pso_moe_mm_iq4nl32 = compile_stamp(metal_moe_mulmm_iq4nl32_msl, ok)
    g_pf_pso_moe_gx = compile_stamp(metal_moe_gather_x_msl, ok)
    pf_compile_moe_kq_twins(ok)
    if (!ok) {
        metal_prefill_shutdown()
        g_pf_failed = true
        return false
    }
    to_log(LOG_INFO, "dasLLAMA metal prefill: resident path live on {metal_device_name(g_dev)}\n")
    g_pf_inited = true
    return true
}

def private rope_rows_ready(t : Model; s : Session; npos : int64) : bool {
    let c = t.config
    let swa_hs = c.head_size_swa > 0l ? c.head_size_swa : c.head_size
    return (long_length(s.rope_cos) >= npos * ((c.rope_dim > 0l ? c.rope_dim : c.head_size) / 2l) &&
        (!has_dual_rope(c) || long_length(s.rope_cos_swa) >= npos * (swa_hs / 2l)))
}

//! Returns the FIRST failing clause; .none = the model and window serve.
[unused_argument(start_pos)]   //! a continuation chunk (start_pos > 0) serves - no clause here depends on where it starts
def private prefill_decline(t : Model; s : Session; npos, start_pos : int64) : MetalPrefillDecline {
    //! the A/B rail: DASLLAMA_METAL_SPAN=0 declines the span to the tripwire-guarded CPU arm
    if (s.attn_uniform_end != 0l && !g_pf_env_span) {
        return MetalPrefillDecline.non_causal_span
    }
    let why = prefill_decline_caps(t, npos, s.attn_uniform_end != 0l)
    if (why != MetalPrefillDecline.none) return why
    //! rope tables are the hook's input (partial rope: rows rope_dim/2 wide; dual-rope: swa class width)
    return rope_rows_ready(t, s, npos) ? MetalPrefillDecline.none : MetalPrefillDecline.rope_rows
}

def private prefill_decline_caps(t : Model; npos : int64; span : bool = false) : MetalPrefillDecline {
    //! npos window is a POLICY decline (blob-only and short SPANS serve any npos: neither has a safe CPU path)
    if (!t.metal_blob && ((npos < g_min_npos && !span) || npos > g_max_npos)) {
        return MetalPrefillDecline.npos_window
    }
    if (t.quant != QuantMode.q8) {
        return MetalPrefillDecline.quant_mode
    }
    //! kquant rides the mul_mm path only — mm pinned off or a shape the %64 gate rejects has no kq-capable GEMM and declines
    if (t.kquant_native) {
        //! kq_load_gpu_supported rejects fmts without a GPU kernel (q4_0 QAT tier) before they dispatch a wrong-layout branch
        if (g_mm_legacy || !g_pf_env_mulmm ||
                !mm_tile_widths_64_ok(t) || !kq_load_gpu_supported(t)) {
            return MetalPrefillDecline.kquant_native
        }
    }
    //! wscale_f16 rides the mul_mm path only — a model the mm shape gate rejects has no s16-capable GEMM and declines
    if (t.wscale_f16) {
        if (g_mm_legacy || !g_pf_env_mulmm || !mm_tile_widths_64_ok(t)) {
            return MetalPrefillDecline.wscale_f16
        }
    }
    //! non-std graphs decline; deltanet hybrids serve via dn_metal_ok (ds == 128 makes every dn GEMM width %128)
    if ((!t.blocks.attn_is_std && !(t.config.hybrid_deltanet && dn_metal_ok(t))) ||
            (!t.blocks.ffn_is_dense && (!moe_metal_ok(t) ||
             (t.config.n_ff_exp % 64l) != 0l || (t.config.dim % 64l) != 0l))) {
        return MetalPrefillDecline.graph
    }
    //! the hyper-connection mixer (qwen4exp) rides the mul_mm rail: its widths sit on the GEMM's 64 lattice, the side input's on the q8 block
    if (t.config.hyper_conn && ((hc_dim(t.config) % 64l) != 0l || (t.config.hc_low_rank % 64l) != 0l ||
            (has_ple_ngram(t.config) && (ple_in_dim(t.config) % 32l) != 0l))) {
        return MetalPrefillDecline.shape
    }
    //! the GEMM kernel reads ROW-MAJOR Q8 weights — a repack backend's interleaved layout is unreadable here
    if (kernel_backend_needs_repack(active_kernel_backend())) {
        return MetalPrefillDecline.backend_repack
    }
    if (metal_missing_needs(t, PF_NEEDS_OK) != default<MetalNeed>) {
        return MetalPrefillDecline.feature
    }
    let c = t.config
    let hidden = layer_hidden(t, 0l)
    let h1 = ffn_second_hidden(t)
    if (h1 != 0l && (h1 < 0l || !t.blocks.ffn_is_dense || c.n_layer_nextn > 0l)) {
        return MetalPrefillDecline.layers
    }
    if ((c.dim % 32l) != 0l || (has_dense_ffn(t) && ((hidden % 32l) != 0l || (h1 > 0l && (h1 % 32l) != 0l)))) {
        return MetalPrefillDecline.shape
    }
    //! per-CLASS: head_size/kv_dim % 32 grid the AV kernel (QK k-slabs any size); hetero geometry and missing attn_v serve
    for (l in range64(c.n_layers)) {
        if ((layer_head_size(c, l) % 32l) != 0l || (layer_kv_dim(c, l) % 32l) != 0l) {
            return MetalPrefillDecline.shape
        }
        //! E-series shared-KV layers (Q-only) serve when the source is byte-compatible — the panel aliases it
        if (t.kv_src[l] != l && !kv_share_ok(t, l)) {
            return MetalPrefillDecline.layers
        }
    }
    //! AFTER the capability gates (the support matrix asserts those reasons): drivers are BLOB-ONLY — planar q8 has no upload path, and a blob model rides the mul_mm rail exclusively
    if (!t.metal_blob || g_mm_legacy || !g_pf_env_mulmm ||
            !mm_tile_widths_64_ok(t)) {
        return MetalPrefillDecline.planar
    }
    return MetalPrefillDecline.none
}


let private MM_TAIL_MAX = 8l

var private g_pf_tail_sites = 0l
var private g_pf_tail_rows = 0l

//! GEMV-tail engage counters: (sites, rows) peeled since load — the gates' anti-vacuous witness.
def public metal_prefill_tail_stats() : tuple<sites : int64; rows : int64> {
    return (sites = g_pf_tail_sites, rows = g_pf_tail_rows)
}

[arch(at="../ARCHITECTURE_GPU_PREFILL.md#gemv-tail-peel")]
def private enc_gemm_mm_tail(enc : MetalComputeEncoder?; bwblob : MetalBuffer?; wboff : uint64;
                             bx, by, bk, bn : MetalBuffer?; yoff : uint64; kdim, d, row0, r : int64) {
    g_pf_tail_sites++
    g_pf_tail_rows += r
    let xoff = uint64(row0 * kdim * 4l)
    let yo = yoff + uint64(row0 * d * 4l)
    if (r == 1l) {
        enc_gemv(enc, bwblob, wboff, bx, by, bk, bn, d, yo, xoff)
        return
    }
    var ka = Q8MvArgs(ndim = uint(kdim), ddim = uint(d), nrows = uint(min(r, 4l)),
        xs4 = uint(kdim / 4l), ys = uint(d))
    enc_mv_b(enc, true, bwblob, wboff, bx, by, ka, xoff, yo)
    if (r > 4l) {
        ka.nrows = uint(r - 4l)
        enc_mv_b(enc, true, bwblob, wboff, bx, by, ka, xoff + uint64(4l * kdim * 4l), yo + uint64(4l * d * 4l))
    }
}

//! Whether a forward takes its two f16 scratch panels: some stamp that reads one compiled.
def private pf_twin_panels_wanted : bool {
    return (g_pf_pso_mm_th != null || g_pf_pso_kq_mm4_th != null || g_pf_pso_kq_mm5_th != null
        || g_pf_pso_kq_mm6_th != null || g_pf_pso_bf16_mm_th != null
        || g_pf_pso_hmm_th != null || pf_kq_iquant_th_any()   // the dev-W route and the iquant site twins read the panel
        || g_pf_pso_moe_mm_q8_th != null || g_pf_pso_moe_mm_mx4_th != null)   //! the MoE down-site cvt consumes the panel too
}

def private pf_twin_panel(rows, kdim : int64) : MetalBuffer? {
    if (!g_pf_env_half_x || g_pf_bxh == null || kdim <= 0l || uint64(rows * kdim * 2l) > g_pf_bxh_bytes) {
        return null
    }
    var bxh = g_pf_bxh_flip ? g_pf_bxh2 : g_pf_bxh
    g_pf_bxh_flip = !g_pf_bxh_flip
    return bxh
}

def private pf_cvt_panel(enc : MetalComputeEncoder?; bx : MetalBuffer?; rows, kdim : int64; skip_convert : bool = false) : MetalBuffer? {
    if (rows < CVT_MIN_ROWS) {
        return null
    }
    var bxh = pf_twin_panel(rows, kdim)
    if (bxh == null) {
        return null
    }
    if (skip_convert) {   //! attribution KO: stale panel halves, the twin path itself unchanged
        return bxh
    }
    enc_cvt_half(enc, bx, bxh, rows * kdim)
    return bxh
}

//! the pure knee arithmetic behind pf_devw_tiles - cap_bytes is the staging panel capacity
//! (g_pf_bwh_bytes in production; the kernel-unit test pins its own)
[arch(at="../ARCHITECTURE_GPU_PREFILL.md#devw-panel-knees")]
def devw_tile_pick(rows, d, kdim : int64; slow_fb : bool; cap_bytes : uint64) : int64 {
    if (kdim <= 0l || kdim % 32l != 0l
            || (kdim > DEVW_LONG_K && rows < (slow_fb ? DEVW_WIDE_N_ROWS : DEVW_LONG_K_ROWS))) {
        return 0l
    }
    let pb = uint64(d * kdim * 2l)
    if (pb <= DEVW_SMALL_PANEL && (d <= DEVW_WIDE_N || rows >= DEVW_WIDE_N_ROWS)
            && pb <= cap_bytes) {
        return 1l
    }
    let tile_floor = slow_fb ? DEVW_WIDE_N_ROWS : DEVW_LONG_K_ROWS
    let tile_over = slow_fb ? DEVW_SMALL_PANEL : DEVW_BIG_PANEL
    if (pb > tile_over && rows >= tile_floor) {
        for (tc in range64(2l, DEVW_MAX_TILES + 1l)) {
            if (d % (tc * 64l) == 0l && pb / uint64(tc) <= DEVW_SMALL_PANEL
                    && uint64(d / tc * kdim * 2l) <= cap_bytes) {
                return tc
            }
        }
    }
    if (pb <= DEVW_BIG_PANEL && rows >= DEVW_BIG_ROWS && pb <= cap_bytes) {
        return 1l
    }
    return 0l
}

def private pf_devw_tiles(rows, d, kdim : int64; slow_fb : bool = false) : int64 {
    if (!g_pf_env_dev_w || g_pf_bwh == null) {
        return 0l
    }
    return devw_tile_pick(rows, d, kdim, slow_fb, g_pf_bwh_bytes)
}

[arch(at="../ARCHITECTURE_GPU_PREFILL.md#prefill-twin-dual-store")]
def private pf_hx_panel(enc : MetalComputeEncoder?; var twin : MetalBuffer?; bx : MetalBuffer?; rows, kdim : int64; skip_convert : bool = false) : MetalBuffer? {
    return twin != null ? twin : pf_cvt_panel(enc, bx, rows, kdim, skip_convert)
}

//! the DBUF pick: the double-buffered twin serves when the knob is on and its PSO compiled
def private pf_dbuf(pso : MetalComputePipeline?) : bool => g_pf_env_dbuf && pso != null

def private psos_ready(psos) : bool {
    for (pso in psos) {
        if (pso == null) {
            return false
        }
    }
    return true
}

//! Whether [[pf_enc_hmm]] can serve: the half twins compile only under a tensor crown, so a
//! box without one (every pre-M5 Apple GPU) answers false and callers keep their f32 route.
def pf_hmm_ready() : bool => psos_ready(fixed_array(g_pf_pso_hmm_th, g_pf_pso_half_rows))

//! Whether [[pf_enc_bf16_mm]] would take a converted half-X panel (its tensor half-X stamp
//! compiled) - callers skip the conversion dispatch when the base form would ignore it.
def pf_bf16_half_ready() : bool => psos_ready(fixed_array(g_pf_pso_bf16_mm_th, g_pf_pso_half_rows))

//! Whether the tower flash pair compiled - the tower driver's flash-route gate (pure
//! simdgroup kernels, no tensor crown required).
def pf_tower_flash_ready() : bool => psos_ready(fixed_array(g_pf_pso_twr_flash, g_pf_pso_twr_kvhc))

def pf_tower_win_ready() : bool => psos_ready(fixed_array(g_pf_pso_twr_win, g_pso_row_gather))

//! The FastConformer kernel set compiled — the blocks driver's device gate (LN, GLU, SiLU, axpy
//! ride the shared/g4a kernels); the depthwise stamp follows the family's tap layout.
def pf_fc_ready(tap_major : bool) : bool => psos_ready(fixed_array(g_pf_pso_fc_pack, g_pf_pso_fc_softmax, g_pf_pso_fc_unpack, g_pso_f32mm,
    tap_major ? g_pf_pso_pk_dw : g_pf_pso_cn_dw, g_pf_pso_g4a_glu, g_pf_pso_g4a_silu, g_pf_pso_g4a_axpy))

//! The parakeet subsample front's kernels compiled - the whole-encode driver's extra device gate.
def pf_pk_front_ready() : bool => psos_ready(fixed_array(g_pf_pso_pk_conv0, g_pf_pso_pk_dw2d, g_pf_pso_pk_xf, g_pf_pso_bias_relu, g_pf_pso_addbias))

//! The canary front's kernels compiled - the parakeet front's, the spectrum's and the mel normalization.
def pf_cn_front_ready() : bool => pf_pk_front_ready() && psos_ready(fixed_array(g_pf_pso_cn_melnorm, g_pf_pso_q3a_pow))

//! The projector tails' row kernels compiled: the whisper-class pair pool and gate, the vision grid pool.
def pf_tw_tail_ready() : bool => psos_ready(fixed_array(g_pf_pso_tw_pool2, g_pf_pso_tw_swiglu_rows, g_pf_pso_tw_pool2d))

//! The gemma4a Conformer kernel set compiled — its tower driver's device gate.
def pf_g4a_ready() : bool => psos_ready(fixed_array(g_pf_pso_g4a_attn, g_pf_pso_g4a_glu, g_pf_pso_g4a_dw, g_pf_pso_g4a_silu, g_pf_pso_g4a_axpy))

//! The gemma4a mel/front kernel set compiled — the whole-chunk driver's extra device gate.
def pf_g4a_front_ready() : bool => psos_ready(fixed_array(g_pf_pso_g4a_mag, g_pf_pso_g4a_logmel, g_pf_pso_g4a_lnrelu, g_pso_im2col2d))

//! The qwen3a conv-frontend tail kernels compiled — its front driver's extra device gate.
def pf_q3a_front_ready() : bool => psos_ready(fixed_array(g_pf_pso_q3a_shuffle, g_pf_pso_q3a_finish))

//! The qwen3a mel kernels compiled — its mel driver's extra device gate.
def pf_q3a_mel_ready() : bool => psos_ready(fixed_array(g_pf_pso_q3a_pow, g_pf_pso_q3a_logmel_t))

def private pf_devw_flip : MetalBuffer ? {
    var bwh = g_pf_bwh_flip ? g_pf_bwh2 : g_pf_bwh
    g_pf_bwh_flip = !g_pf_bwh_flip
    return bwh
}

//! The all-device half x half mul_mm dispatch (tall/1-D picks included): f16 W at `woff` bytes,
//! f16 X panel, f32 y. The dev-W route's own form pick — and the Metal tower's f16-W route
//! rides it too, so it is public.
def pf_enc_hmm(enc : MetalComputeEncoder?; bwh, bxh, by : MetalBuffer?; yoff : uint64;
               bk, bn : MetalBuffer?; rows, td, dfull, kdim : int64; woff : uint64 = 0ul) {
    var r0 = 0l
    if (pf_tall_on() && g_pf_pso_hmm_th128 != null && rows >= 128l
            && rows / 128l * (td / 64l) >= TALL_OCC_FLOOR) {
        r0 = rows / 128l * 128l
        if (g_pf_env_grid1d && g_pf_pso_hmm_th128_l1 != null) {
            enc_gemm_hmm_th128_l1(enc, bwh, woff, bxh, 0ul, by, yoff, bk, bn, r0 / 128l * (td / 64l))
        } else {
            enc_gemm_hmm_th128(enc, bwh, woff, bxh, 0ul, by, yoff, bk, bn, r0, td)
        }
    }
    if (rows - r0 > 0l) {
        enc_gemm_hmm_th(enc, bwh, woff, bxh, uint64(r0 * kdim * 2l), by, yoff + uint64(r0 * dfull * 4l), bk, bn, rows - r0, td)
    }
}

//! The resident half panel under `key`, made on a first ask (`fresh`: the caller fills it). `charged` panels count
//! against the dev-W budget and are refused past it; an uncharged one - a megabyte of router rows - is not.
def private pf_devw_resident_panel(key : uint64; bytes : uint64; var fresh : bool&; charged : bool = true) : DevwResEntry {
    fresh = false
    if (key_exists(g_pf_devw_res, key)) {
        if (g_pf_devw_res[key].bytes == bytes) {   // nolint:PERF026 — key_exists-guarded read, never inserts
            return g_pf_devw_res[key]   // nolint:PERF026 — key_exists-guarded read, never inserts
        }
        return DevwResEntry()   //! a hit must cover the request - a size mismatch is a foreign key
    }
    if (charged && g_pf_env_devw_res_bytes == 0ul) {
        return DevwResEntry()
    }
    if (charged && g_pf_devw_res_bytes + bytes > g_pf_env_devw_res_bytes) {
        if (!g_pf_devw_res_full) {
            g_pf_devw_res_full = true
            to_log(LOG_WARNING, "dasLLAMA metal prefill: devw-resident budget exhausted at {int64(g_pf_devw_res_bytes) / 1048576l} MB - later sites stay on the scratch route\n")   // nolint:PERF028,PERF026 — one-shot fill arm
        }
        return DevwResEntry()
    }
    var bwh = metal_new_buffer(g_dev, bytes)
    if (bwh == null) {
        return DevwResEntry()
    }
    residency_note(bwh)
    var e = DevwResEntry(buf = bwh, boff = 0ul, owned = true, bytes = bytes)
    g_pf_devw_res[key] = e   // nolint:PERF026 — one-shot fill arm, 4 inserts per layer per process
    g_pf_devw_res_bytes += charged ? bytes : 0ul
    fresh = true
    return e
}

//! The tests' rail to the resident panel registry: one ask for `bytes` under `key` on the driver's device, and the bytes charged after it.
def public metal_prefill_resident_ask(key, bytes : uint64; charged : bool) : tuple<got : bool; fresh : bool; charged_bytes : uint64> {
    var fresh = false
    let e = pf_devw_resident_panel(key, bytes, fresh, charged)
    return (got = e.buf != null, fresh = fresh, charged_bytes = g_pf_devw_res_bytes)
}

def private pf_devw_res_release {
    for (e in values(g_pf_devw_res)) {
        if (e.owned && e.buf != null) {
            metal_release(e.buf)
        }
    }
    g_pf_devw_res |> clear()
    g_pf_devw_res_bytes = 0ul
    g_pf_devw_res_full = false
    delete g_pf_devw_seeded
}

//! Idempotent per mapped plane; a model without the plane costs one empty-check per forward.
def private pf_devw_seed_baked(t : Model) {
    if (empty(t.devwf16) || !t.metal_blob) {
        return
    }
    let pkey = intptr(unsafe(addr(t.devwf16[0])))
    if (g_pf_devw_seeded?[pkey] ?? false) {
        return
    }
    g_pf_devw_seeded[pkey] = true
    var bpan = plane_buffer(g_dev, unsafe(addr < void? >(t.devwf16[0])), uint64(long_length(t.devwf16) * 2l), t.image_map != null)
    if (bpan == null) {
        return
    }
    var seeded = 0
    for (i in range(length(t.devwf16_key))) {
        let f = t.devwf16_fmt[i]
        var key = 0ul
        var kfmt : KqFmt
        if (f == 0) {
            key = pf_devw_key(intptr(blob_of(g_dev, t, 0l).buf), uint64(t.devwf16_key[i]), 0)
        } elif (kq_fmt_of_stream_code(f, kfmt) && kfmt != KqFmt.q51) {
            key = pf_devw_key(intptr(kq_quants_of(g_dev, t, kfmt, t.devwf16_key[i]).buf), uint64(t.devwf16_key[i]), kq_schema_id(kfmt))
        } else {
            continue   //! an fmt this build does not know - a newer image's site stays runtime-served
        }
        if (!key_exists(g_pf_devw_res, key)) {
            g_pf_devw_res[key] = DevwResEntry(buf = bpan, boff = uint64(t.devwf16_off[i] * 2l), owned = false, bytes = uint64(t.devwf16_n[i] * 2l))   // nolint:PERF026 — once per mapped plane
            seeded++
        }
    }
    to_log(LOG_INFO, "dasLLAMA metal prefill: {seeded} baked dev-W panels seeded ({long_length(t.devwf16) * 2l >> 20l} MB, zero-copy)\n")   // nolint:PERF028,PERF026 — once per mapped plane
}

//! the q8 resident arm: a baked or once-dequantized panel, then the single full-panel hmm.
//! false = not resident (off/budget, no bake) - the caller runs the scratch tile loop
def private pf_devw_panel_q8_resident(enc : MetalComputeEncoder?; bwblob : MetalBuffer?; wboff : uint64;
                                      bxh, by, bk, bn : MetalBuffer?; rows, d, kdim : int64; yoff : uint64) : bool {
    var fresh = false
    let e = pf_devw_resident_panel(pf_devw_key(intptr(bwblob), wboff, 0), uint64(d * kdim * 2l), fresh)
    if (e.buf == null) {
        return false
    }
    if (fresh) {
        let nblk = d * (kdim / 32l)
        var dka = CvtArgs(total = uint(nblk))
        enc_dequant_q8h(enc, bwblob, wboff, bwblob, e.buf, dka, (nblk + 255l) / 256l * 256l)
    }
    pf_enc_hmm(enc, e.buf, bxh, by, yoff, bk, bn, rows, d, d, kdim, e.boff)
    return true
}

//! N-column tile: bn stays the FULL row stride, the y base pointer carries the column base
def private pf_enc_devw_q8_tile(enc : MetalComputeEncoder?; bwblob : MetalBuffer?; wboff : uint64;
                                bxh, by, bk, bn : MetalBuffer?; rows, tbase, td, dfull, kdim : int64; yoff : uint64) {
    var bwh = pf_devw_flip()
    let nb = kdim / 32l
    let nblk = td * nb
    var dka = CvtArgs(total = uint(nblk))
    if (g_pf_skip != "devw_cvt") {   // attribution: stale W panel, the mm's traffic unchanged
        enc_dequant_q8h(enc, bwblob, wboff + uint64(tbase * nb * 34l), bwblob, bwh, dka, (nblk + 255l) / 256l * 256l)
    }
    pf_enc_hmm(enc, bwh, bxh, by, yoff + uint64(tbase * 4l), bk, bn, rows, td, dfull, kdim)
}

def private pf_devw_panel(enc : MetalComputeEncoder?; bwblob : MetalBuffer?; wboff : uint64;
                          bxh, by, bk, bn : MetalBuffer?; rows, d, kdim : int64; yoff : uint64) : bool {
    // the q8 dequant pass steps whole 32-blocks
    if (!g_pf_env_dev_w || kdim <= 0l || (kdim % 32l) != 0l) {
        return false
    }
    if (pf_devw_panel_q8_resident(enc, bwblob, wboff, bxh, by, bk, bn, rows, d, kdim, yoff)) {
        return true
    }
    let tiles = pf_devw_tiles(rows, d, kdim)
    if (tiles <= 0l) {
        return false
    }
    let td = d / tiles
    for (ti in range64(tiles)) {
        pf_enc_devw_q8_tile(enc, bwblob, wboff, bxh, by, bk, bn, rows, ti * td, td, d, kdim, yoff)
    }
    return true
}

//! the split-scale formats whose dev-W dequant rides the mul_mm tensor scaffold (ARCHITECTURE_GPU_PREFILL.md#prefill-kq-tensor-scaffold)
def private pf_kq_split_fmt(fmt : KqFmt) : bool {
    return (fmt == KqFmt.iq4xs || fmt == KqFmt.k3 || fmt == KqFmt.iq3s || fmt == KqFmt.iq3xxs || fmt == KqFmt.iq4nl
        || fmt == KqFmt.q40 || fmt == KqFmt.k2 || fmt == KqFmt.iq2s || fmt == KqFmt.iq2xs || fmt == KqFmt.iq2xxs)
}

//! the format's dev-W dequant PSO, null where the toolchain probe failed
def private pf_kq_dq_pso(fmt : KqFmt) : MetalComputePipeline? {
    if (fmt == KqFmt.k6) return g_pf_pso_dq_k6
    if (fmt == KqFmt.k4) return g_pf_pso_dq_k4
    if (fmt == KqFmt.k5) return g_pf_pso_dq_k5
    if (fmt == KqFmt.iq4xs) return g_pf_pso_dq_kq44
    if (fmt == KqFmt.iq4nl) return g_pf_pso_dq_kq45
    if (fmt == KqFmt.q40) return g_pf_pso_dq_kq40
    if (fmt == KqFmt.k3) return g_pf_pso_dq_kq3
    if (fmt == KqFmt.iq3s) return g_pf_pso_dq_kq33
    if (fmt == KqFmt.iq3xxs) return g_pf_pso_dq_kq34
    if (fmt == KqFmt.k2) return g_pf_pso_dq_kq2
    if (fmt == KqFmt.iq2s) return g_pf_pso_dq_kq23
    if (fmt == KqFmt.iq2xs) return g_pf_pso_dq_kq24
    if (fmt == KqFmt.iq2xxs) return g_pf_pso_dq_kq25
    return null
}

def private pf_kq_iquant_th_any() : bool {
    return (g_pf_pso_kq_mm44_th != null || g_pf_pso_kq_mm45_th != null || g_pf_pso_kq_mm40_th != null
        || g_pf_pso_kq_mm3_th != null || g_pf_pso_kq_mm33_th != null || g_pf_pso_kq_mm34_th != null
        || g_pf_pso_kq_mm2_th != null || g_pf_pso_kq_mm23_th != null || g_pf_pso_kq_mm24_th != null
        || g_pf_pso_kq_mm25_th != null)
}

def private pf_kq_dq_any() : bool {
    return (g_pf_pso_dq_k4 != null || g_pf_pso_dq_k5 != null || g_pf_pso_dq_k6 != null
        || g_pf_pso_dq_kq44 != null || g_pf_pso_dq_kq45 != null || g_pf_pso_dq_kq40 != null || g_pf_pso_dq_kq3 != null
        || g_pf_pso_dq_kq33 != null || g_pf_pso_dq_kq34 != null || g_pf_pso_dq_kq2 != null
        || g_pf_pso_dq_kq23 != null || g_pf_pso_dq_kq24 != null || g_pf_pso_dq_kq25 != null)
}

//! one format's dequant of W rows [tbase, tbase+td) into the f16 panel bwh: the plane offsets
//! stride per superblock (16B scale strips, the 2B d tail - 4B on k2, none on the compact
//! formats - and kq_qsb(fmt) quant bytes); K-quants run per 32-block, the rest per 64x64 chunk
[arch(at="../ARCHITECTURE_GPU.md#gpu-backends")]
def private pf_enc_kq_dq(enc : MetalComputeEncoder?; fmt : KqFmt; bs : tuple<buf : MetalBuffer?; soff : uint64; doff : uint64>;
                         bq : tuple<buf : MetalBuffer?; qoff : uint64>; tbase, td, kdim : int64; bwh, bk, bn : MetalBuffer?) {
    let nsb = kdim / 256l
    let so = bs.soff + uint64(tbase * nsb * 16l)
    let dofs = bs.doff + uint64(tbase * nsb * (fmt == KqFmt.k2 ? 4l : 2l))
    let qo = bq.qoff + uint64(tbase * nsb * kq_qsb(fmt))
    if (fmt == KqFmt.k6 || fmt == KqFmt.k4 || fmt == KqFmt.k5) {
        let nblk = td * kdim / 32l
        var dka = CvtArgs(total = uint(nblk))
        let gt = (nblk + 255l) / 256l * 256l
        if (fmt == KqFmt.k6) {
            enc_dequant_k6h(enc, bs.buf, dofs, bs.buf, so, bq.buf, qo, bwh, dka, gt)
        } elif (fmt == KqFmt.k4) {
            enc_dequant_k4h(enc, bs.buf, so, bs.buf, so, bq.buf, qo, bwh, dka, gt)
        } else {
            enc_dequant_k5h(enc, bs.buf, so, bs.buf, so, bq.buf, qo, bwh, dka, gt)
        }
        return
    }
    if (fmt == KqFmt.iq4xs) {
        enc_kq_dq_iq4xs(enc, bs.buf, dofs, bs.buf, so, bq.buf, qo, bwh, 0ul, bwh, 0ul, bk, bn, bwh, kdim, td)
    } elif (fmt == KqFmt.iq4nl) {
        enc_kq_dq_iq4nl(enc, bs.buf, so, bs.buf, so, bq.buf, qo, bwh, 0ul, bwh, 0ul, bk, bn, bwh, kdim, td)
    } elif (fmt == KqFmt.q40) {
        enc_kq_dq_q40(enc, bs.buf, so, bs.buf, so, bq.buf, qo, bwh, 0ul, bwh, 0ul, bk, bn, bwh, kdim, td)
    } elif (fmt == KqFmt.k3) {
        enc_kq_dq_k3(enc, bs.buf, dofs, bs.buf, so, bq.buf, qo, bwh, 0ul, bwh, 0ul, bk, bn, bwh, kdim, td)
    } elif (fmt == KqFmt.iq3s) {
        enc_kq_dq_iq3s(enc, bs.buf, dofs, bs.buf, so, bq.buf, qo, bwh, 0ul, bwh, 0ul, bk, bn, bwh, kdim, td)
    } elif (fmt == KqFmt.iq3xxs) {
        enc_kq_dq_iq3xxs(enc, bs.buf, dofs, bs.buf, so, bq.buf, qo, bwh, 0ul, bwh, 0ul, bk, bn, bwh, kdim, td)
    } elif (fmt == KqFmt.k2) {
        enc_kq_dq_k2(enc, bs.buf, dofs, bs.buf, so, bq.buf, qo, bwh, 0ul, bwh, 0ul, bk, bn, bwh, kdim, td)
    } elif (fmt == KqFmt.iq2s) {
        enc_kq_dq_iq2s(enc, bs.buf, dofs, bs.buf, so, bq.buf, qo, bwh, 0ul, bwh, 0ul, bk, bn, bwh, kdim, td)
    } elif (fmt == KqFmt.iq2xs) {
        enc_kq_dq_iq2xs(enc, bs.buf, dofs, bs.buf, so, bq.buf, qo, bwh, 0ul, bwh, 0ul, bk, bn, bwh, kdim, td)
    } elif (fmt == KqFmt.iq2xxs) {
        enc_kq_dq_iq2xxs(enc, bs.buf, dofs, bs.buf, so, bq.buf, qo, bwh, 0ul, bwh, 0ul, bk, bn, bwh, kdim, td)
    } else {
        panic("metal prefill dev-W: format {fmt} has no dequant pass - pf_kq_dq_pso admitted it")
    }
}

def private pf_devw_panel_kq(enc : MetalComputeEncoder?; t : Model; fmt : KqFmt; woff : int64;
                             bxh, by, bk, bn : MetalBuffer?; rows, d, kdim : int64; yoff : uint64) : bool {
    // the split formats' chunk passes dequant whole 64x64 chunks; the K-quant passes step 32-blocks
    if (!g_pf_env_dev_w || pf_kq_dq_pso(fmt) == null || kdim <= 0l
            || (kdim % (pf_kq_split_fmt(fmt) ? 64l : 32l)) != 0l) {
        return false
    }
    let bq = kq_quants_of(g_dev, t, fmt, woff)
    let bs = kq_scales_of(g_dev, t, fmt, woff)
    var rfresh = false
    let re = pf_devw_resident_panel(pf_devw_key(intptr(bq.buf), uint64(woff), kq_schema_id(fmt)), uint64(d * kdim * 2l), rfresh)
    if (re.buf != null) {
        if (rfresh) {
            pf_enc_kq_dq(enc, fmt, bs, bq, 0l, d, kdim, re.buf, bk, bn)
        }
        pf_enc_hmm(enc, re.buf, bxh, by, yoff, bk, bn, rows, d, d, kdim, re.boff)
        return true
    }
    let tiles = pf_devw_tiles(rows, d, kdim, slow_fb = true)
    if (tiles <= 0l || (pf_kq_split_fmt(fmt) && (d / tiles) % 64l != 0l)) {
        return false
    }
    let td = d / tiles
    for (ti in range64(tiles)) {
        let tbase = ti * td
        var bwh = pf_devw_flip()
        if (g_pf_skip != "devw_cvt") {   // attribution: stale W panel, the mm's traffic unchanged
            pf_enc_kq_dq(enc, fmt, bs, bq, tbase, td, kdim, bwh, bk, bn)
        }
        pf_enc_hmm(enc, bwh, bxh, by, yoff + uint64(tbase * 4l), bk, bn, rows, td, d, kdim)
    }
    return true
}

//! the grid and LUT prologues fill the class's staging tables where the format has them
def private devw_mirror_run(var k : auto(K)?; fmt : KqFmt; kq, ks : array<uint8>; d, kdim : int64; var dst : array<float16>) : bool {
    k.kqu |> reserve_resize(long_length(kq) / 4l)
    let nsb_all = d * kdim / 256l
    unsafe {
        memcpy(addr(k.kqu[0]), addr < uint8 const? >(kq[0]), long_length(kq))
        if (fmt == KqFmt.iq4nl || fmt == KqFmt.q40) {
            k.kdh |> reserve_resize(long_length(ks) / 2l)
            memcpy(addr(k.kdh[0]), addr < uint8 const? >(ks[0]), long_length(ks))
        } else {
            k.ksc |> reserve_resize(nsb_all)
            memcpy(addr(k.ksc[0]), addr < uint8 const? >(ks[0]), nsb_all * 16l)
            k.kdh |> reserve_resize((long_length(ks) - nsb_all * 16l) / 2l)
            memcpy(addr(k.kdh[0]), addr < uint8 const? >(ks[nsb_all * 16l]), long_length(ks) - nsb_all * 16l)
        }
    }
    static_if (typeinfo safe_has_field < iq4lutt > (*k)) {
        for (i in range(16)) {
            k.iq4lutt[i] = fmt == KqFmt.q40 ? float(i) - 8.0 : iq4_lut(uint(i))
        }
    }
    static_if (typeinfo safe_has_field < gridw3t > (*k)) {
        for (i in range(512)) {
            k.gridw3t[i] = iq3s_gw(i)
        }
    }
    static_if (typeinfo safe_has_field < gridw3xt > (*k)) {
        for (i in range(256)) {
            k.gridw3xt[i] = iq3xxs_gw(i)
        }
    }
    let nsb = uint(kdim / 256l)
    for (nb in range64(d / 64l)) {
        for (kb2 in range64(kdim / 64l)) {
            for (work in range(256)) {
                k->stage16(uint(work), uint(kb2 * 2l), uint(nb * 64l), nsb, 0u)
            }
            for (j in range64(64l)) {
                for (c in range64(64l)) {
                    dst[(nb * 64l + j) * kdim + kb2 * 64l + c] = k.twb[j * 72l + c]
                }
            }
        }
    }
    unsafe {
        delete k
    }
    return true
}

def devw_dequant_kq_mirror(fmt : KqFmt; kq : array<uint8>; ks : array<uint8>; d, kdim : int64; var dst : array<float16>) : bool {
    if (d % 64l != 0l || kdim % 256l != 0l || long_length(dst) < d * kdim) {
        return false
    }
    if (fmt == KqFmt.iq4xs) return devw_mirror_run(new MetalKqDequantIq4xs(), fmt, kq, ks, d, kdim, dst)
    if (fmt == KqFmt.iq4nl) return devw_mirror_run(new MetalKqDequantIq4nl(), fmt, kq, ks, d, kdim, dst)
    if (fmt == KqFmt.q40) return devw_mirror_run(new MetalKqDequantQ40(), fmt, kq, ks, d, kdim, dst)
    if (fmt == KqFmt.k3) return devw_mirror_run(new MetalKqDequantK3(), fmt, kq, ks, d, kdim, dst)
    if (fmt == KqFmt.iq3s) return devw_mirror_run(new MetalKqDequantIq3s(), fmt, kq, ks, d, kdim, dst)
    if (fmt == KqFmt.iq3xxs) return devw_mirror_run(new MetalKqDequantIq3xxs(), fmt, kq, ks, d, kdim, dst)
    if (fmt == KqFmt.k2) return devw_mirror_run(new MetalKqDequantK2(), fmt, kq, ks, d, kdim, dst)
    if (fmt == KqFmt.iq2s) return devw_mirror_run(new MetalKqDequantIq2s(), fmt, kq, ks, d, kdim, dst)
    if (fmt == KqFmt.iq2xs) return devw_mirror_run(new MetalKqDequantIq2xs(), fmt, kq, ks, d, kdim, dst)
    if (fmt == KqFmt.iq2xxs) return devw_mirror_run(new MetalKqDequantIq2xxs(), fmt, kq, ks, d, kdim, dst)
    return false
}

[init]
def private pf_devw_mirror_register {
    register_devw_kq_mirror(@@devw_dequant_kq_mirror)
}

//! kdim-less callers stay whole-dispatch 32-tile: the remainder arm needs kdim for its X stride
[arch(at="../ARCHITECTURE_GPU_PREFILL.md#prefill-gemm-ladder")]
def private enc_gemm_mm_tensor(enc : MetalComputeEncoder?; bwblob : MetalBuffer?; wboff : uint64; bx, by, bk, bn : MetalBuffer?; rows, d : int64; yoff : uint64; bxh : MetalBuffer?; kdim : int64 = 0l) {
    if (bxh != null && g_pf_pso_hmm_th != null) {
        if (pf_devw_panel(enc, bwblob, wboff, bxh, by, bk, bn, rows, d, kdim, yoff)) {
            return
        }
    }
    let half_form = bxh != null && g_pf_pso_mm_th != null
    var r0 = 0l
    if (pf_tall_on() && rows >= 128l && (kdim > 0l || rows % 128l == 0l)
            && rows / 128l * (d / 64l) >= TALL_OCC_FLOOR) {
        let tall = half_form ? g_pf_pso_mm_th128 : g_pf_pso_mm_t128
        if (tall != null) {
            r0 = rows / 128l * 128l
            if (half_form) {
                if (g_pf_env_dbuf && g_pf_pso_mm_thdb128 != null) {
                    enc_gemm_mm_thdb128_c(enc, bwblob, wboff, bwblob, bxh, 0ul, by, yoff, bk, bn, r0, d)
                } else {
                    enc_gemm_mm_th128_c(enc, bwblob, wboff, bwblob, bxh, 0ul, by, yoff, bk, bn, r0, d)
                }
            } else {
                enc_gemm_mm_t128_c(enc, bwblob, wboff, bwblob, bx, 0ul, by, yoff, bk, bn, r0, d)
            }
        }
    }
    if (rows - r0 <= 0l) {
        return
    }
    let xoff = uint64(r0 * kdim * (half_form ? 2l : 4l))
    let yo = yoff + uint64(r0 * d * 4l)
    if (half_form) {
        if (pf_dbuf(g_pf_pso_mm_thdb)) {
            enc_gemm_mm_thdb_c(enc, bwblob, wboff, bwblob, bxh, xoff, by, yo, bk, bn, rows - r0, d)
        } else {
            enc_gemm_mm_th_c(enc, bwblob, wboff, bwblob, bxh, xoff, by, yo, bk, bn, rows - r0, d)
        }
    } else {
        enc_gemm_mm_t_c(enc, bwblob, wboff, bwblob, bx, xoff, by, yo, bk, bn, rows - r0, d)
    }
}

def private pf_q8_devw(enc : MetalComputeEncoder?; bwblob : MetalBuffer?; wboff : uint64; bxh, by, bk, bn : MetalBuffer?; rows, d, kdim : int64; yoff : uint64) : bool {
    return g_pf_tensor_ok && bxh != null && g_pf_pso_hmm_th != null && pf_devw_panel(enc, bwblob, wboff, bxh, by, bk, bn, rows, d, kdim, yoff)
}

//! yoff places K/V outputs past a continuation's panel rows; kdim/npos arm the GEMV tail;
//! bxh is the caller-minted f16 twin (one convert per panel).
def private enc_gemm_mm(enc : MetalComputeEncoder?; bwblob : MetalBuffer?; wboff : uint64; bx, by, bk, bn : MetalBuffer?; mp, d : int64; yoff : uint64 = 0ul; kdim : int64 = 0l; npos : int64 = 0l; bxh : MetalBuffer? = null) {
    let r = npos % 32l
    //! per-form reduction alignment: the gemv walks per-block; the b4 stripe reads whole 128-quant rounds
    let align_ok = r == 1l ? kdim % 32l == 0l : kdim % 128l == 0l
    if (g_pf_env_mm_tail && npos > 0l && r >= 1l && r <= MM_TAIL_MAX && align_ok && d % 8l == 0l) {
        let nfull = npos - r
        if (nfull > 0l) {
            if (g_pf_mm_tensor) {
                enc_gemm_mm_tensor(enc, bwblob, wboff, bx, by, bk, bn, nfull, d, yoff, bxh, kdim)
            } elif (!pf_q8_devw(enc, bwblob, wboff, bxh, by, bk, bn, nfull, d, kdim, yoff)) {
                enc_gemm_mm_c(enc, bwblob, wboff, bwblob, bx, by, yoff, bk, bn, nfull, d)
            }
        }
        enc_gemm_mm_tail(enc, bwblob, wboff, bx, by, bk, bn, yoff, kdim, d, nfull, r)
        return
    }
    if (g_pf_mm_tensor) {
        enc_gemm_mm_tensor(enc, bwblob, wboff, bx, by, bk, bn, mp, d, yoff, bxh, kdim)
    } elif (!pf_q8_devw(enc, bwblob, wboff, bxh, by, bk, bn, mp, d, kdim, yoff)) {
        enc_gemm_mm_c(enc, bwblob, wboff, bwblob, bx, by, yoff, bk, bn, mp, d)
    }
}

//! whether a GEMM through `pf_enc_q8_mm` reads a half activation panel on this box: the caller converts one a panel
def pf_q8_mm_half() : bool => (g_pf_mm_tensor && g_pf_pso_mm_th != null) || (g_pf_tensor_ok && g_pf_pso_hmm_th != null)

//! One q8 GEMM over a 34-byte-block blob on the prefill ladder, for a driver outside this file: `mp` rows padded to 32,
//! no GEMV tail; `bxh` is the caller's half twin of `bx`, null where `pf_q8_mm_half` says none is read.
[arch(at="../ARCHITECTURE_GPU_PREFILL.md#prefill-gemm-ladder")]
def pf_enc_q8_mm(enc : MetalComputeEncoder?; bwblob : MetalBuffer?; wboff : uint64; bx, bxh, by, bk, bn : MetalBuffer?; mp, d, kdim : int64) {
    enc_gemm_mm(enc, bwblob, wboff, bx, by, bk, bn, mp, d, 0ul, kdim, 0l, bxh)
}

//! kdim feeds the tall/remainder split's X stride; kdim-less callers stay whole-dispatch 32-tile
[arch(at="../ARCHITECTURE_GPU_PREFILL.md#prefill-gemm-ladder")]
def pf_enc_bf16_mm(enc : MetalComputeEncoder?; bw : MetalBuffer?; wboff : uint64;
                   bx, by, bk, bn : MetalBuffer?; mp, nd : int64; bxh : MetalBuffer? = null; kdim : int64 = 0l) {
    if (g_pf_bf16_mm_tensor) {
        let half_form = bxh != null && g_pf_pso_bf16_mm_th != null
        var r0 = 0l
        if (pf_tall_on() && mp >= 128l && (kdim > 0l || mp % 128l == 0l)
                && mp / 128l * (nd / 64l) >= TALL_OCC_FLOOR) {
            let tall = half_form ? g_pf_pso_bf16_mm_th128 : g_pf_pso_bf16_mm_t128
            if (tall != null) {
                r0 = mp / 128l * 128l
                if (half_form) {
                    pf_enc_bf16_mm_th128_c(enc, bw, wboff, bxh, 0ul, by, 0ul, bk, bn, r0, nd)
                } else {
                    pf_enc_bf16_mm_t128_c(enc, bw, wboff, bx, 0ul, by, 0ul, bk, bn, r0, nd)
                }
            }
        }
        if (mp - r0 <= 0l) {
            return
        }
        let xoff = uint64(r0 * kdim * (half_form ? 2l : 4l))
        let yo = uint64(r0 * nd * 4l)
        if (half_form) {
            pf_enc_bf16_mm_th_c(enc, bw, wboff, bxh, xoff, by, yo, bk, bn, mp - r0, nd)
        } else {
            pf_enc_bf16_mm_t_c(enc, bw, wboff, bx, xoff, by, yo, bk, bn, mp - r0, nd)
        }
    } else {
        pf_enc_bf16_mm_c(enc, bw, wboff, bx, by, bk, bn, mp, nd)
    }
}

//! the dense tall-kq gate's pure half: env_tall is the set_metal_prefill_tall A/B seat -
//! the tall form must honor it like every other form override
def tallkq_pick(mp, rows, kdim : int64; env_tall : bool) : bool {
    return env_tall && kdim > 0l && mp >= 128l && uint64(rows * kdim * 2l) >= TALLKQ_MIN_PANEL
}

//! the iquant/split-scale formats' tensor twins - compiled only behind each format's crown
[arch(at="../ARCHITECTURE_GPU_PREFILL.md#prefill-kq-tensor-scaffold")]
def private pf_compile_kq_iquant_tensor_twins(var ok : bool&) {
    g_pf_kq_mm44_tensor = metal_tensor_crowned("kq_mulmm_iq4xs")
    if (g_pf_kq_mm44_tensor) {
        g_pf_pso_kq_mm44_t = compile_stamp(MetalKqMulMmIq4xsT_metal_kq_mulmm_iq4xs_t_msl, ok)
        g_pf_pso_kq_mm44_th = compile_stamp(MetalKqMulMmIq4xsTH_metal_kq_mulmm_iq4xs_t_msl, ok)
    }
    g_pf_kq_mm45_tensor = metal_tensor_crowned("kq_mulmm_iq4nl")
    if (g_pf_kq_mm45_tensor) {
        g_pf_pso_kq_mm45_t = compile_stamp(MetalKqMulMmIq4nlT_metal_kq_mulmm_iq4xs_t_msl, ok)
        g_pf_pso_kq_mm45_th = compile_stamp(MetalKqMulMmIq4nlTH_metal_kq_mulmm_iq4xs_t_msl, ok)
    }
    g_pf_kq_mm40_tensor = metal_tensor_crowned("kq_mulmm_q40")
    if (g_pf_kq_mm40_tensor) {
        g_pf_pso_kq_mm40_t = compile_stamp(MetalKqMulMmQ40T_metal_kq_mulmm_iq4xs_t_msl, ok)
        g_pf_pso_kq_mm40_th = compile_stamp(MetalKqMulMmQ40TH_metal_kq_mulmm_iq4xs_t_msl, ok)
    }
    g_pf_kq_mm3_tensor = metal_tensor_crowned("kq_mulmm_k3")
    if (g_pf_kq_mm3_tensor) {
        g_pf_pso_kq_mm3_t = compile_stamp(MetalKqMulMmK3T_metal_kq_mulmm_k3_t_msl, ok)
        g_pf_pso_kq_mm3_th = compile_stamp(MetalKqMulMmK3TH_metal_kq_mulmm_k3_t_msl, ok)
    }
    g_pf_kq_mm33_tensor = metal_tensor_crowned("kq_mulmm_iq3s")
    if (g_pf_kq_mm33_tensor) {
        g_pf_pso_kq_mm33_t = compile_stamp(MetalKqMulMmIq3sT_metal_kq_mulmm_iq3s_t_msl, ok)
        g_pf_pso_kq_mm33_th = compile_stamp(MetalKqMulMmIq3sTH_metal_kq_mulmm_iq3s_t_msl, ok)
        g_pf_pso_kq_mm33_th128 = compile_stamp(MetalKqMulMmIq3sTH128_metal_kq_mulmm_iq3s_t_msl, ok)
    }
    g_pf_kq_mm34_tensor = metal_tensor_crowned("kq_mulmm_iq3xxs")
    if (g_pf_kq_mm34_tensor) {
        g_pf_pso_kq_mm34_t = compile_stamp(MetalKqMulMmIq3xxsT_metal_kq_mulmm_iq3xxs_t_msl, ok)
        g_pf_pso_kq_mm34_th = compile_stamp(MetalKqMulMmIq3xxsTH_metal_kq_mulmm_iq3xxs_t_msl, ok)
        g_pf_pso_kq_mm34_th128 = compile_stamp(MetalKqMulMmIq3xxsTH128_metal_kq_mulmm_iq3xxs_t_msl, ok)
    }
    g_pf_kq_mm2_tensor = metal_tensor_crowned("kq_mulmm_k2")
    if (g_pf_kq_mm2_tensor) {
        g_pf_pso_kq_mm2_t = compile_stamp(MetalKqMulMmK2T_metal_kq_mulmm_k2_t_msl, ok)
        g_pf_pso_kq_mm2_th = compile_stamp(MetalKqMulMmK2TH_metal_kq_mulmm_k2_t_msl, ok)
    }
    g_pf_kq_mm23_tensor = metal_tensor_crowned("kq_mulmm_iq2s")
    if (g_pf_kq_mm23_tensor) {
        g_pf_pso_kq_mm23_t = compile_stamp(MetalKqMulMmIq2sT_metal_kq_mulmm_iq2s_t_msl, ok)
        g_pf_pso_kq_mm23_th = compile_stamp(MetalKqMulMmIq2sTH_metal_kq_mulmm_iq2s_t_msl, ok)
    }
    g_pf_kq_mm24_tensor = metal_tensor_crowned("kq_mulmm_iq2xs")
    if (g_pf_kq_mm24_tensor) {
        g_pf_pso_kq_mm24_t = compile_stamp(MetalKqMulMmIq2xsT_metal_kq_mulmm_iq2xs_t_msl, ok)
        g_pf_pso_kq_mm24_th = compile_stamp(MetalKqMulMmIq2xsTH_metal_kq_mulmm_iq2xs_t_msl, ok)
    }
    g_pf_kq_mm25_tensor = metal_tensor_crowned("kq_mulmm_iq2xxs")
    if (g_pf_kq_mm25_tensor) {
        g_pf_pso_kq_mm25_t = compile_stamp(MetalKqMulMmIq2xxsT_metal_kq_mulmm_iq2xxs_t_msl, ok)
        g_pf_pso_kq_mm25_th = compile_stamp(MetalKqMulMmIq2xxsTH_metal_kq_mulmm_iq2xxs_t_msl, ok)
    }
}

def private pf_compile_kq_dequant_passes(var ok : bool&) {
    g_pf_pso_dq_kq44 = compile_stamp(MetalKqDequantIq4xs_metal_kq_dequant_iq4xs_msl, ok)
    g_pf_pso_dq_kq45 = compile_stamp(MetalKqDequantIq4nl_metal_kq_dequant_iq4xs_msl, ok)
    g_pf_pso_dq_kq40 = compile_stamp(MetalKqDequantQ40_metal_kq_dequant_iq4xs_msl, ok)
    g_pf_pso_dq_kq3 = compile_stamp(MetalKqDequantK3_metal_kq_dequant_k3_msl, ok)
    g_pf_pso_dq_kq33 = compile_stamp(MetalKqDequantIq3s_metal_kq_dequant_iq3s_msl, ok)
    g_pf_pso_dq_kq34 = compile_stamp(MetalKqDequantIq3xxs_metal_kq_dequant_iq3xxs_msl, ok)
    g_pf_pso_dq_kq2 = compile_stamp(MetalKqDequantK2_metal_kq_dequant_k2_msl, ok)
    g_pf_pso_dq_kq23 = compile_stamp(MetalKqDequantIq2s_metal_kq_dequant_iq2s_msl, ok)
    g_pf_pso_dq_kq24 = compile_stamp(MetalKqDequantIq2xs_metal_kq_dequant_iq2xs_msl, ok)
    g_pf_pso_dq_kq25 = compile_stamp(MetalKqDequantIq2xxs_metal_kq_dequant_iq2xxs_msl, ok)
}

typedef KqSplitEnc = function<(enc : MetalComputeEncoder?; bkdh : MetalBuffer?; s0off : uint64; bksc : MetalBuffer?; soff : uint64;
                               bkqu : MetalBuffer?; qoff : uint64; bxf : MetalBuffer?; xoff : uint64; by : MetalBuffer?; yoff : uint64;
                               bkdim : MetalBuffer?; bndim : MetalBuffer?; mp : int64; rows : int64) : void>

//! one dense split-scale format's crown flag, compiled twins and their builders, in one row: the
//! t (f32 X) and th (half-X) forms every split format stamps, plus the 128-row tall form only
//! iq3s and iq3xxs stamp - a format that stamps no tall twin leaves both th128 cells null
struct private KqSplitStamps {
    crown : bool
    pso_t, pso_th, pso_th128 : MetalComputePipeline?
    enc_t, enc_th, enc_th128 : KqSplitEnc
}

def private pf_kq_split_stamps(fmt : KqFmt) : KqSplitStamps {
    if (fmt == KqFmt.iq4xs) return KqSplitStamps(crown = g_pf_kq_mm44_tensor, pso_t = g_pf_pso_kq_mm44_t, pso_th = g_pf_pso_kq_mm44_th,
        enc_t = @@enc_kq_mm_iq4xs_t_c, enc_th = @@enc_kq_mm_iq4xs_th_c)
    if (fmt == KqFmt.iq4nl) return KqSplitStamps(crown = g_pf_kq_mm45_tensor, pso_t = g_pf_pso_kq_mm45_t, pso_th = g_pf_pso_kq_mm45_th,
        enc_t = @@enc_kq_mm_iq4nl_t_c, enc_th = @@enc_kq_mm_iq4nl_th_c)
    if (fmt == KqFmt.q40) return KqSplitStamps(crown = g_pf_kq_mm40_tensor, pso_t = g_pf_pso_kq_mm40_t, pso_th = g_pf_pso_kq_mm40_th,
        enc_t = @@enc_kq_mm_q40_t_c, enc_th = @@enc_kq_mm_q40_th_c)
    if (fmt == KqFmt.k3) return KqSplitStamps(crown = g_pf_kq_mm3_tensor, pso_t = g_pf_pso_kq_mm3_t, pso_th = g_pf_pso_kq_mm3_th,
        enc_t = @@enc_kq_mm_k3_t_c, enc_th = @@enc_kq_mm_k3_th_c)
    if (fmt == KqFmt.iq3s) return KqSplitStamps(crown = g_pf_kq_mm33_tensor, pso_t = g_pf_pso_kq_mm33_t, pso_th = g_pf_pso_kq_mm33_th,
        pso_th128 = g_pf_pso_kq_mm33_th128, enc_t = @@enc_kq_mm_iq3s_t_c, enc_th = @@enc_kq_mm_iq3s_th_c, enc_th128 = @@enc_kq_mm_iq3s_th128_c)
    if (fmt == KqFmt.iq3xxs) return KqSplitStamps(crown = g_pf_kq_mm34_tensor, pso_t = g_pf_pso_kq_mm34_t, pso_th = g_pf_pso_kq_mm34_th,
        pso_th128 = g_pf_pso_kq_mm34_th128, enc_t = @@enc_kq_mm_iq3xxs_t_c, enc_th = @@enc_kq_mm_iq3xxs_th_c, enc_th128 = @@enc_kq_mm_iq3xxs_th128_c)
    if (fmt == KqFmt.k2) return KqSplitStamps(crown = g_pf_kq_mm2_tensor, pso_t = g_pf_pso_kq_mm2_t, pso_th = g_pf_pso_kq_mm2_th,
        enc_t = @@enc_kq_mm_k2_t_c, enc_th = @@enc_kq_mm_k2_th_c)
    if (fmt == KqFmt.iq2s) return KqSplitStamps(crown = g_pf_kq_mm23_tensor, pso_t = g_pf_pso_kq_mm23_t, pso_th = g_pf_pso_kq_mm23_th,
        enc_t = @@enc_kq_mm_iq2s_t_c, enc_th = @@enc_kq_mm_iq2s_th_c)
    if (fmt == KqFmt.iq2xs) return KqSplitStamps(crown = g_pf_kq_mm24_tensor, pso_t = g_pf_pso_kq_mm24_t, pso_th = g_pf_pso_kq_mm24_th,
        enc_t = @@enc_kq_mm_iq2xs_t_c, enc_th = @@enc_kq_mm_iq2xs_th_c)
    if (fmt == KqFmt.iq2xxs) return KqSplitStamps(crown = g_pf_kq_mm25_tensor, pso_t = g_pf_pso_kq_mm25_t, pso_th = g_pf_pso_kq_mm25_th,
        enc_t = @@enc_kq_mm_iq2xxs_t_c, enc_th = @@enc_kq_mm_iq2xxs_th_c)
    return KqSplitStamps(crown = false)   //! pf_kq_split_fmt admits only the rows above
}

struct private KqDeepStamps {
    crown : bool
    d_soff : bool
    pso_t : MetalComputePipeline?
    pso_th : MetalComputePipeline?
    pso_th128 : MetalComputePipeline?
    pso_thdb : MetalComputePipeline?
    pso_thdb128 : MetalComputePipeline?
    enc_t : KqSplitEnc
    enc_th : KqSplitEnc
    enc_th128 : KqSplitEnc
    enc_thdb : KqSplitEnc
    enc_thdb128 : KqSplitEnc
}

def private pf_kq_deep_stamps(fmt : KqFmt) : KqDeepStamps {
    if (fmt == KqFmt.k6) {
        return KqDeepStamps(crown = g_pf_kq_mm6_tensor, d_soff = false,
            pso_t = g_pf_pso_kq_mm6_t, pso_th = g_pf_pso_kq_mm6_th, pso_th128 = g_pf_pso_kq_mm6_th128,
            pso_thdb = g_pf_pso_kq_mm6_thdb, pso_thdb128 = g_pf_pso_kq_mm6_thdb128,
            enc_t = @@enc_kq_mm_k6_t_c, enc_th = @@enc_kq_mm_k6_th_c, enc_th128 = @@enc_kq_mm_k6_th128_c,
            enc_thdb = @@enc_kq_mm_k6_thdb_c, enc_thdb128 = @@enc_kq_mm_k6_thdb128_c)
    }
    if (fmt == KqFmt.k4) {
        return KqDeepStamps(crown = g_pf_kq_mm4_tensor, d_soff = true,
            pso_t = g_pf_pso_kq_mm4_t, pso_th = g_pf_pso_kq_mm4_th, pso_th128 = g_pf_pso_kq_mm4_th128,
            pso_thdb = g_pf_pso_kq_mm4_thdb, pso_thdb128 = g_pf_pso_kq_mm4_thdb128,
            enc_t = @@enc_kq_mm_k4_t_c, enc_th = @@enc_kq_mm_k4_th_c, enc_th128 = @@enc_kq_mm_k4_th128_c,
            enc_thdb = @@enc_kq_mm_k4_thdb_c, enc_thdb128 = @@enc_kq_mm_k4_thdb128_c)
    }
    return KqDeepStamps(crown = g_pf_kq_mm5_tensor, d_soff = true,
        pso_t = g_pf_pso_kq_mm5_t, pso_th = g_pf_pso_kq_mm5_th, pso_th128 = g_pf_pso_kq_mm5_th128,
        pso_thdb = g_pf_pso_kq_mm5_thdb, pso_thdb128 = g_pf_pso_kq_mm5_thdb128,
        enc_t = @@enc_kq_mm_k5_t_c, enc_th = @@enc_kq_mm_k5_th_c, enc_th128 = @@enc_kq_mm_k5_th128_c,
        enc_thdb = @@enc_kq_mm_k5_thdb_c, enc_thdb128 = @@enc_kq_mm_k5_thdb128_c)
}

[arch(at="../ARCHITECTURE_GPU_PREFILL.md#prefill-kq-tensor-scaffold")]
def private pf_enc_kq_site_mm(enc : MetalComputeEncoder?; t : Model; fmt : KqFmt; woff, rows : int64;
                              bx, by, bk, bn : MetalBuffer?; mp : int64; yoff : uint64 = 0ul; bxh : MetalBuffer? = null; kdim : int64 = 0l) {
    let bq = kq_quants_of(g_dev, t, fmt, woff)
    let bs = kq_scales_of(g_dev, t, fmt, woff)
    // the split-scale dev-W arm gates itself (the pass compiles behind the probe); a tall panel is over the knee and declines here
    if (pf_kq_split_fmt(fmt) && bxh != null && g_pf_pso_hmm_th != null && kdim > 0l) {
        if (pf_devw_panel_kq(enc, t, fmt, woff, bxh, by, bk, bn, mp, rows, kdim, yoff)) {
            return
        }
    }
    if (pf_kq_split_fmt(fmt)) {
        let ss <- pf_kq_split_stamps(fmt)
        if (!ss.crown || ss.pso_t == null) {
            enc_kq_gemm_mm_b(enc, fmt, bq.buf, bq.qoff, bs.buf, bs.soff, bs.doff, bx, by, yoff, bk, bn, mp, rows)
            return
        }
        let d0 = (fmt == KqFmt.iq4nl || fmt == KqFmt.q40) ? bs.soff : bs.doff
        if (bxh != null && ss.pso_th128 != null && ss.pso_th != null && tallkq_pick(mp, rows, kdim, pf_tall_on())) {
            let r0 = mp / 128l * 128l
            invoke(ss.enc_th128, enc, bs.buf, d0, bs.buf, bs.soff, bq.buf, bq.qoff, bxh, 0ul, by, yoff, bk, bn, r0, rows)
            if (mp - r0 > 0l) {
                invoke(ss.enc_th, enc, bs.buf, d0, bs.buf, bs.soff, bq.buf, bq.qoff, bxh, uint64(r0 * kdim * 2l), by, yoff + uint64(r0 * rows * 4l), bk, bn, mp - r0, rows)
            }
        } elif (bxh != null && ss.pso_th != null) {
            invoke(ss.enc_th, enc, bs.buf, d0, bs.buf, bs.soff, bq.buf, bq.qoff, bxh, 0ul, by, yoff, bk, bn, mp, rows)
        } else {
            invoke(ss.enc_t, enc, bs.buf, d0, bs.buf, bs.soff, bq.buf, bq.qoff, bx, 0ul, by, yoff, bk, bn, mp, rows)
        }
        return
    }
    //! deep-class arm: a panel dev-W would split past 8 tiles is DRAM-resident - the tall
    //! in-kernel-dequant stamp reads the quant plane once per 128-row tile instead of
    //! materializing and re-streaming a 2B/element f16 panel
    let ks <- pf_kq_deep_stamps(fmt)
    let d0 = ks.d_soff ? bs.soff : bs.doff
    if (ks.crown && bxh != null && ks.pso_th128 != null && tallkq_pick(mp, rows, kdim, pf_tall_on())) {
        let r0 = mp / 128l * 128l
        invoke(pf_dbuf(ks.pso_thdb128) ? ks.enc_thdb128 : ks.enc_th128,
            enc, bs.buf, d0, bs.buf, bs.soff, bq.buf, bq.qoff, bxh, 0ul, by, yoff, bk, bn, r0, rows)
        if (mp - r0 > 0l) {
            invoke(pf_dbuf(ks.pso_thdb) ? ks.enc_thdb : ks.enc_th,
                enc, bs.buf, d0, bs.buf, bs.soff, bq.buf, bq.qoff, bxh, uint64(r0 * kdim * 2l), by, yoff + uint64(r0 * rows * 4l), bk, bn, mp - r0, rows)
        }
        return
    }
    if (g_pf_tensor_ok && bxh != null && g_pf_pso_hmm_th != null && kdim > 0l) {
        if (pf_devw_panel_kq(enc, t, fmt, woff, bxh, by, bk, bn, mp, rows, kdim, yoff)) {
            return
        }
    }
    if (!ks.crown || ks.pso_t == null) {
        enc_kq_gemm_mm_b(enc, fmt, bq.buf, bq.qoff, bs.buf, bs.soff, bs.doff, bx, by, yoff, bk, bn, mp, rows)
    } elif (bxh != null && ks.pso_th != null) {
        invoke(pf_dbuf(ks.pso_thdb) ? ks.enc_thdb : ks.enc_th,
            enc, bs.buf, d0, bs.buf, bs.soff, bq.buf, bq.qoff, bxh, 0ul, by, yoff, bk, bn, mp, rows)
    } else {
        invoke(ks.enc_t, enc, bs.buf, d0, bs.buf, bs.soff, bq.buf, bq.qoff, bx, 0ul, by, yoff, bk, bn, mp, rows)
    }
}


var private g_pf_held : table<string>   //!< the forms a one-process race holds off
var private g_pf_moe_rows_on = false
var private @scratch g_pf_moe_rows : array<uint>

//! Keep (or stop keeping) the rows each expert served in a window's last routed layer.
def public metal_prefill_keep_moe_rows(on : bool) {
    g_pf_moe_rows_on = on
}

//! The rows an expert served in the last window's last routed layer, by expert id; empty until a
//! window ran under `metal_prefill_keep_moe_rows(true)`.
def public metal_prefill_moe_rows() : array<uint> {
    return <- clone_to_move(g_pf_moe_rows)
}

//! Hold a prefill form off (or let it serve again) for a one-process race, the form it replaced
//! serving meanwhile: "attn_dev" (the device attention pair), "router_mm" (the router's half
//! GEMM), "dn_half" (the scan over half rows and prepared gates). Returns the previous hold.
def public set_metal_prefill_held(form : string; held : bool) : bool {
    let was = key_exists(g_pf_held, form)
    if (held) {
        g_pf_held |> insert(form)
    } else {
        g_pf_held |> erase(form)
    }
    return was
}

def private pf_held(form : string) : bool => key_exists(g_pf_held, form)

//! The window's router logits as one half x half GEMM: the layer's router rows as a resident half
//! panel, the routed rows' half twin. false = no twin, no tile for the expert count, or no room
//! for the panel - the slab GEMV serves.
def private pf_enc_router_mm(enc : MetalComputeEncoder?; bw_rt, bxh, blg, bdim, bne : MetalBuffer?; wkey : uint64; ne, dim, mp : int64) : bool {
    if (pf_held("router_mm") || bxh == null || !pf_hmm_ready() || ne % 64l != 0l) {
        return false
    }
    var fresh = false
    let e = pf_devw_resident_panel(wkey, uint64(ne * dim * 2l), fresh, false)
    if (e.buf == null) {
        return false
    }
    if (fresh) {
        enc_cvt_half(enc, bw_rt, e.buf, ne * dim)
    }
    pf_enc_hmm(enc, e.buf, bxh, blg, 0ul, bdim, bne, mp, ne, ne, dim)
    return true
}

[arch(at="../ARCHITECTURE_GPU_PREFILL_MOE.md#prefill-moe-buckets")]
def private pf_enc_moe_route(enc : MetalComputeEncoder?; t : Model; l : int64;
                             bx, blg, bsel, bcnt, bbase, bbkt, binv, bdim, bne, bnk, bgm, brb, bhasb, btot, bnp : MetalBuffer?; npos : int64;
                             bxh : MetalBuffer? = null; mp : int64 = 0l) {
    let c = t.config
    let rows_at = unsafe(addr<void?>(t.fblob[t.router_off + l * c.n_expert * c.dim]))
    var bw_rt = upload_region(rows_at, uint64(c.n_expert * c.dim * 4l))
    if (c.moe_router_bias || !pf_enc_router_mm(enc, bw_rt, bxh, blg, bdim, bne, pf_devw_key(intptr(rows_at), 0ul, 0), c.n_expert, c.dim, mp)) {
        enc_moe_router_b(enc, bw_rt, bx, blg, bdim, bne, bdim, brb, bhasb, bnp, c.n_expert, npos)   //! bxs = bdim: per-position x stride = dim
    }
    return if (g_pf_skip == "moe_only_router")   //! attribution: router GEMV alone, no select/CSR
    if (c.n_expert > 256l) {
        enc_moe_select_512(enc, blg, bsel, bsel, bne, bnk, bgm, npos)
    } else {
        enc_moe_select(enc, blg, bsel, bsel, bne, bnk, bgm, npos)
    }
    return if (g_pf_skip == "moe_only_sel")   //! attribution: router + select, no CSR
    pf_enc_moe_count(enc, bsel, bcnt, btot, bnk, c.n_expert)
    pf_enc_moe_bucket(enc, bsel, bbase, bbkt, binv, btot, bnk, bcnt, bne, c.n_expert + 1l)   // basep[ne] = the padded total
}

def private kn_moe_mm_family_tail(enc : MetalComputeEncoder?; bx, by, bcnt, bbase, bbkt : MetalBuffer?) {
    kn_buffer(enc, bx, 0ul, 3)
    kn_buffer(enc, by, 0ul, 4)
    kn_buffer(enc, bcnt, 0ul, 7)
    kn_buffer(enc, bbase, 0ul, 8)
    kn_buffer(enc, bbkt, 0ul, 9)
}

//! the per-32 twins (q5_1, iq4nl32) walk 64-deep K chunks (2 blocks/step) - an odd 32-block count
//! stays legacy; the kernel-unit gate keys its twin legs on this same predicate
def moe_q51_twin_fits(kdim : uint) : bool {
    return (kdim & 63u) == 0u
}

def private pf_moe_th_pso(fmt : KqFmt) : MetalComputePipeline? {
    if (fmt == KqFmt.q8) return g_pf_pso_moe_mm_q8_th
    if (fmt == KqFmt.k4) return g_pf_pso_moe_mm_k4_th
    if (fmt == KqFmt.k5) return g_pf_pso_moe_mm_k5_th
    if (fmt == KqFmt.k6) return g_pf_pso_moe_mm_k6_th
    if (fmt == KqFmt.q51) return g_pf_pso_moe_mm_q51_th
    if (fmt == KqFmt.iq4nl32) return g_pf_pso_moe_mm_iq4nl32_th
    if (fmt == KqFmt.k3) return g_pf_pso_moe_mm_k3_th
    if (fmt == KqFmt.iq4xs) return g_pf_pso_moe_mm_iq4xs_th
    if (fmt == KqFmt.q40) return g_pf_pso_moe_mm_q40_th
    if (fmt == KqFmt.iq3s) return g_pf_pso_moe_mm_iq3s_th
    if (fmt == KqFmt.iq3xxs) return g_pf_pso_moe_mm_iq3xxs_th
    if (fmt == KqFmt.iq2s) return g_pf_pso_moe_mm_iq2s_th
    if (fmt == KqFmt.iq2xs) return g_pf_pso_moe_mm_iq2xs_th
    if (fmt == KqFmt.iq2xxs) return g_pf_pso_moe_mm_iq2xxs_th
    return null
}

//! t = the f32-X base, th its half-X twin, th128 the 128-row tall floor, thr the tall remainder
enum MoeSplitForm {
    t
    th
    th128
    thr
}

typedef MoeSplitEnc = function<(enc : MetalComputeEncoder?; bkdh : MetalBuffer?; s0off : uint64; bksc : MetalBuffer?; soff : uint64;
                                bkqu : MetalBuffer?; qoff : uint64; bxf : MetalBuffer?; xoff : uint64; by : MetalBuffer?; yoff : uint64;
                                bkdim : MetalBuffer?; bndim : MetalBuffer?; bcnt : MetalBuffer?; bbasep : MetalBuffer?;
                                mp : int64; rows : int64; ne : int64; tiles : int64) : void>

//! one expert-plane format's four compiled twins and their builders, in one row - a pso and the
//! builder that dispatches it are the same cell's pair, so neither can be picked without the other
struct private MoeSplitStamps {
    pso_t, pso_th, pso_th128, pso_thr : MetalComputePipeline?
    enc_t, enc_th, enc_th128, enc_thr : MoeSplitEnc
}

def private pf_moe_split_stamps(fmt : KqFmt) : MoeSplitStamps {
    if (fmt == KqFmt.k3) return MoeSplitStamps(pso_t = g_pf_pso_moe_mm_k3_t, pso_th = g_pf_pso_moe_mm_k3_th, pso_th128 = g_pf_pso_moe_mm_k3_th128, pso_thr = g_pf_pso_moe_mm_k3_thr,
        enc_t = @@pf_enc_moe_mm_k3_t_c, enc_th = @@pf_enc_moe_mm_k3_th_c, enc_th128 = @@pf_enc_moe_mm_k3_th128_c, enc_thr = @@pf_enc_moe_mm_k3_thr_c)
    if (fmt == KqFmt.iq4xs) return MoeSplitStamps(pso_t = g_pf_pso_moe_mm_iq4xs_t, pso_th = g_pf_pso_moe_mm_iq4xs_th, pso_th128 = g_pf_pso_moe_mm_iq4xs_th128, pso_thr = g_pf_pso_moe_mm_iq4xs_thr,
        enc_t = @@pf_enc_moe_mm_iq4xs_t_c, enc_th = @@pf_enc_moe_mm_iq4xs_th_c, enc_th128 = @@pf_enc_moe_mm_iq4xs_th128_c, enc_thr = @@pf_enc_moe_mm_iq4xs_thr_c)
    if (fmt == KqFmt.q40) return MoeSplitStamps(pso_t = g_pf_pso_moe_mm_q40_t, pso_th = g_pf_pso_moe_mm_q40_th, pso_th128 = g_pf_pso_moe_mm_q40_th128, pso_thr = g_pf_pso_moe_mm_q40_thr,
        enc_t = @@pf_enc_moe_mm_q40_t_c, enc_th = @@pf_enc_moe_mm_q40_th_c, enc_th128 = @@pf_enc_moe_mm_q40_th128_c, enc_thr = @@pf_enc_moe_mm_q40_thr_c)
    if (fmt == KqFmt.iq3s) return MoeSplitStamps(pso_t = g_pf_pso_moe_mm_iq3s_t, pso_th = g_pf_pso_moe_mm_iq3s_th, pso_th128 = g_pf_pso_moe_mm_iq3s_th128, pso_thr = g_pf_pso_moe_mm_iq3s_thr,
        enc_t = @@pf_enc_moe_mm_iq3s_t_c, enc_th = @@pf_enc_moe_mm_iq3s_th_c, enc_th128 = @@pf_enc_moe_mm_iq3s_th128_c, enc_thr = @@pf_enc_moe_mm_iq3s_thr_c)
    if (fmt == KqFmt.iq3xxs) return MoeSplitStamps(pso_t = g_pf_pso_moe_mm_iq3xxs_t, pso_th = g_pf_pso_moe_mm_iq3xxs_th, pso_th128 = g_pf_pso_moe_mm_iq3xxs_th128, pso_thr = g_pf_pso_moe_mm_iq3xxs_thr,
        enc_t = @@pf_enc_moe_mm_iq3xxs_t_c, enc_th = @@pf_enc_moe_mm_iq3xxs_th_c, enc_th128 = @@pf_enc_moe_mm_iq3xxs_th128_c, enc_thr = @@pf_enc_moe_mm_iq3xxs_thr_c)
    if (fmt == KqFmt.iq2s) return MoeSplitStamps(pso_t = g_pf_pso_moe_mm_iq2s_t, pso_th = g_pf_pso_moe_mm_iq2s_th, pso_th128 = g_pf_pso_moe_mm_iq2s_th128, pso_thr = g_pf_pso_moe_mm_iq2s_thr,
        enc_t = @@pf_enc_moe_mm_iq2s_t_c, enc_th = @@pf_enc_moe_mm_iq2s_th_c, enc_th128 = @@pf_enc_moe_mm_iq2s_th128_c, enc_thr = @@pf_enc_moe_mm_iq2s_thr_c)
    if (fmt == KqFmt.iq2xs) return MoeSplitStamps(pso_t = g_pf_pso_moe_mm_iq2xs_t, pso_th = g_pf_pso_moe_mm_iq2xs_th, pso_th128 = g_pf_pso_moe_mm_iq2xs_th128, pso_thr = g_pf_pso_moe_mm_iq2xs_thr,
        enc_t = @@pf_enc_moe_mm_iq2xs_t_c, enc_th = @@pf_enc_moe_mm_iq2xs_th_c, enc_th128 = @@pf_enc_moe_mm_iq2xs_th128_c, enc_thr = @@pf_enc_moe_mm_iq2xs_thr_c)
    if (fmt == KqFmt.iq2xxs) return MoeSplitStamps(pso_t = g_pf_pso_moe_mm_iq2xxs_t, pso_th = g_pf_pso_moe_mm_iq2xxs_th, pso_th128 = g_pf_pso_moe_mm_iq2xxs_th128, pso_thr = g_pf_pso_moe_mm_iq2xxs_thr,
        enc_t = @@pf_enc_moe_mm_iq2xxs_t_c, enc_th = @@pf_enc_moe_mm_iq2xxs_th_c, enc_th128 = @@pf_enc_moe_mm_iq2xxs_th128_c, enc_thr = @@pf_enc_moe_mm_iq2xxs_thr_c)
    return MoeSplitStamps()   //! pf_moe_split_fmt admits only the rows above
}

def private pf_moe_split_th_any() : bool => g_pf_pso_moe_mm_k3_th != null || g_pf_pso_moe_mm_iq4xs_th != null || g_pf_pso_moe_mm_q40_th != null || g_pf_pso_moe_mm_iq3s_th != null || g_pf_pso_moe_mm_iq3xxs_th != null || g_pf_pso_moe_mm_iq2s_th != null || g_pf_pso_moe_mm_iq2xs_th != null || g_pf_pso_moe_mm_iq2xxs_th != null

[arch(at="../ARCHITECTURE_GPU_PREFILL_MOE.md#prefill-moe-buckets")]
def private pf_moe_split_enc(enc : MetalComputeEncoder?; fmt : KqFmt; form : MoeSplitForm;
                             bs : tuple<buf : MetalBuffer?; soff : uint64; doff : uint64>; bq : tuple<buf : MetalBuffer?; qoff : uint64>;
                             bx, by, bkd, bnd, bcnt, bbase : MetalBuffer?; mpad, rows, ne, tiles : int64) {
    let ms <- pf_moe_split_stamps(fmt)
    let encf = form == MoeSplitForm.t ? ms.enc_t : (form == MoeSplitForm.th ? ms.enc_th : (form == MoeSplitForm.th128 ? ms.enc_th128 : ms.enc_thr))
    if (encf == null) {
        panic("metal moe prefill: expert plane format {fmt} has no split twin - pf_moe_split_fmt admitted it")
    }
    let d0 = fmt == KqFmt.q40 ? bs.soff : bs.doff
    invoke(encf, enc, bs.buf, d0, bs.buf, bs.soff, bq.buf, bq.qoff, bx, 0ul, by, 0ul, bkd, bnd, bcnt, bbase, mpad, rows, ne, tiles)
}

def private pf_moe_split_fmt(fmt : KqFmt) : bool => fmt == KqFmt.k3 || fmt == KqFmt.iq4xs || fmt == KqFmt.q40 || fmt == KqFmt.iq3s || fmt == KqFmt.iq3xxs || fmt == KqFmt.iq2s || fmt == KqFmt.iq2xs || fmt == KqFmt.iq2xxs

def private pf_moe_model_split(t : Model) : bool {
    for (l in range64(t.config.n_layers)) {
        if (pf_moe_split_fmt(fmt_at(t.we1_fmt, l)) || pf_moe_split_fmt(fmt_at(t.we2_fmt, l)) || pf_moe_split_fmt(fmt_at(t.we3_fmt, l))) {
            return true
        }
    }
    return false
}

def private pf_moe_twins_ready(t : Model) : bool {
    for (l in range64(t.config.n_layers)) {
        let fe1 = fmt_at(t.we1_fmt, l)
        let fe3 = fmt_at(t.we3_fmt, l)
        for (f in [fe1, fmt_at(t.we2_fmt, l), fe3]) {
            if (pf_moe_split_fmt(f) && pf_moe_split_stamps(f).pso_t == null) {
                return false
            }
        }
        // the up and gate planes share ONE gather panel, so a split plane on either needs the gather pso and BOTH siblings' TH twins
        if ((pf_moe_split_fmt(fe1) || pf_moe_split_fmt(fe3)) &&
                (g_pf_pso_moe_gx == null || pf_moe_th_pso(fe1) == null || pf_moe_th_pso(fe3) == null)) {
            return false
        }
    }
    return true
}

var private g_pf_moe_tall_avg = 32l   //!< mean rows an expert at which the routed GEMMs take the tall pair
var private g_pf_mxg_min = 1l   //!< the window at which the routed gate and up read the gathered half panel: every window - the in-kernel gather is the slower form from 17 tokens up

def public set_metal_moe_gather_min(npos : int64) : int64 {
    let was = g_pf_mxg_min
    g_pf_mxg_min = max(npos, 1l)
    return was
}

//! The routed tall pair's engage point, in mean rows an expert; answers the value it replaced.
def public set_metal_moe_tall_avg(rows : int64) : int64 {
    let was = g_pf_moe_tall_avg
    g_pf_moe_tall_avg = max(rows, 1l)
    return was
}

def private pf_moe_tall_pick(npos, nk, ne : int64; pso_th128, pso_thr : MetalComputePipeline?) : bool {
    return pf_tall_on() && npos * nk >= g_pf_moe_tall_avg * ne && pso_th128 != null && pso_thr != null
}

def private pf_enc_moe_mm(enc : MetalComputeEncoder?; t : Model; fmt : KqFmt; stack_off, rows, npos : int64;   // nolint:STYLE037,STYLE038 — flat per-format twin-pick ladder, one arm per weight format
                          bx, by, bcnt, bbase, bbkt : MetalBuffer?; var ka : MoeMmArgs;
                          contiguous : bool = false; bxh : MetalBuffer? = null;
                          bkd : MetalBuffer? = null; bnd : MetalBuffer? = null; mpad : int64 = 0l) {
    return if (g_pf_skip == "moe_mm_disp")   //! attribution: gather+cvt kept, mm dispatches out
    let ne = t.config.n_expert
    if (pf_moe_split_fmt(fmt)) {
        let bq = kq_quants_of(g_dev, t, fmt, stack_off)
        let bs = kq_scales_of(g_dev, t, fmt, stack_off)
        let ms <- pf_moe_split_stamps(fmt)
        if (!contiguous || ms.pso_t == null || bkd == null || bnd == null) {
            panic("metal moe prefill: {fmt} experts serve through the tensor twins over contiguous rows only - pf_moe_twins_ready admitted the model")
        }
        if (bxh != null && ms.pso_th != null) {
            if (pf_moe_tall_pick(npos, int64(ka.nk), ne, ms.pso_th128, ms.pso_thr)) {
                pf_moe_split_enc(enc, fmt, MoeSplitForm.th128, bs, bq, bxh, by, bkd, bnd, bcnt, bbase, mpad, rows, ne, npos / 128l)
                pf_moe_split_enc(enc, fmt, MoeSplitForm.thr, bs, bq, bxh, by, bkd, bnd, bcnt, bbase, mpad, rows, ne, 4l)
            } else {
                pf_moe_split_enc(enc, fmt, MoeSplitForm.th, bs, bq, bxh, by, bkd, bnd, bcnt, bbase, mpad, rows, ne, (npos + 31l) / 32l)
            }
        } else {
            pf_moe_split_enc(enc, fmt, MoeSplitForm.t, bs, bq, bx, by, bkd, bnd, bcnt, bbase, mpad, rows, ne, (npos + 31l) / 32l)
        }
        return
    }
    if (fmt == KqFmt.q8) {
        let bw = blob_of(g_dev, t, stack_off)
        if (g_pf_moe_mm_q8_tensor && contiguous && g_pf_pso_moe_mm_q8_t != null) {
            if (bxh != null && g_pf_pso_moe_mm_q8_th != null) {
                if (pf_moe_tall_pick(npos, int64(ka.nk), ne, g_pf_pso_moe_mm_q8_th128, g_pf_pso_moe_mm_q8_thr)) {
                    pf_enc_moe_mm_q8_th128_c(enc, bw.buf, bw.boff, bw.buf, bxh, by, ka, bcnt, bbase, rows, npos, ne)
                    pf_enc_moe_mm_q8_thr_c(enc, bw.buf, bw.boff, bw.buf, bxh, by, ka, bcnt, bbase, rows, ne)
                } else {
                    pf_enc_moe_mm_q8_th_c(enc, bw.buf, bw.boff, bw.buf, bxh, by, ka, bcnt, bbase, rows, npos, ne)
                }
            } else {
                pf_enc_moe_mm_q8_t_c(enc, bw.buf, bw.boff, bw.buf, bx, by, ka, bcnt, bbase, rows, npos, ne)
            }
        } else {
            pf_enc_moe_mm_q8_c(enc, bw.buf, bw.boff, bw.buf, bx, by, ka, bcnt, bbase, bbkt, rows, npos, ne)
        }
    } elif (fmt == KqFmt.q51) {
        let qp = q51_planes_of(g_dev, t, stack_off)
        if (contiguous && moe_q51_twin_fits(ka.kdim) && g_pf_pso_moe_mm_q51_t != null) {
            if (bxh != null && g_pf_pso_moe_mm_q51_th != null) {
                if (pf_moe_tall_pick(npos, int64(ka.nk), ne, g_pf_pso_moe_mm_q51_th128, g_pf_pso_moe_mm_q51_thr)) {
                    pf_enc_moe_mm_q51_th128_c(enc, qp.sbuf, qp.soff, qp.qbuf, qp.qoff, bxh, by, ka, bcnt, bbase, rows, npos, ne)
                    pf_enc_moe_mm_q51_thr_c(enc, qp.sbuf, qp.soff, qp.qbuf, qp.qoff, bxh, by, ka, bcnt, bbase, rows, ne)
                } else {
                    pf_enc_moe_mm_q51_th_c(enc, qp.sbuf, qp.soff, qp.qbuf, qp.qoff, bxh, by, ka, bcnt, bbase, rows, npos, ne)
                }
            } else {
                pf_enc_moe_mm_q51_t_c(enc, qp.sbuf, qp.soff, qp.qbuf, qp.qoff, bx, by, ka, bcnt, bbase, rows, npos, ne)
            }
        } else {
            pf_enc_moe_mm_q51_c(enc, qp.sbuf, qp.soff, qp.qbuf, qp.qoff, bx, by, ka, bcnt, bbase, bbkt, rows, npos, ne)
        }
    } elif (fmt == KqFmt.iq4nl32) {
        let qp = iq4nl32_planes_of(g_dev, t, stack_off)
        if (contiguous && moe_q51_twin_fits(ka.kdim) && g_pf_pso_moe_mm_iq4nl32_t != null) {
            if (bxh != null && g_pf_pso_moe_mm_iq4nl32_th != null) {
                if (pf_moe_tall_pick(npos, int64(ka.nk), ne, g_pf_pso_moe_mm_iq4nl32_th128, g_pf_pso_moe_mm_iq4nl32_thr)) {
                    pf_enc_moe_mm_iq4nl32_th128_c(enc, qp.sbuf, qp.soff, qp.qbuf, qp.qoff, bxh, by, ka, bcnt, bbase, rows, npos, ne)
                    pf_enc_moe_mm_iq4nl32_thr_c(enc, qp.sbuf, qp.soff, qp.qbuf, qp.qoff, bxh, by, ka, bcnt, bbase, rows, ne)
                } else {
                    pf_enc_moe_mm_iq4nl32_th_c(enc, qp.sbuf, qp.soff, qp.qbuf, qp.qoff, bxh, by, ka, bcnt, bbase, rows, npos, ne)
                }
            } else {
                pf_enc_moe_mm_iq4nl32_t_c(enc, qp.sbuf, qp.soff, qp.qbuf, qp.qoff, bx, by, ka, bcnt, bbase, rows, npos, ne)
            }
        } else {
            pf_enc_moe_mm_iq4nl32_c(enc, qp.sbuf, qp.soff, qp.qbuf, qp.qoff, bx, by, ka, bcnt, bbase, bbkt, rows, npos, ne)
        }
    } elif (fmt == KqFmt.k6) {
        let bq = kq_quants_of(g_dev, t, fmt, stack_off)
        let bs = kq_scales_of(g_dev, t, fmt, stack_off)
        if (contiguous && g_pf_pso_moe_mm_k6_t != null) {
            if (bxh != null && g_pf_pso_moe_mm_k6_th != null) {
                if (pf_moe_tall_pick(npos, int64(ka.nk), ne, g_pf_pso_moe_mm_k6_th128, g_pf_pso_moe_mm_k6_thr)) {
                    pf_enc_moe_mm_k6_th128_c(enc, bs.buf, bs.doff, bs.buf, bs.soff, bq.buf, bq.qoff, bxh, by, ka, bcnt, bbase, rows, npos, ne)
                    pf_enc_moe_mm_k6_thr_c(enc, bs.buf, bs.doff, bs.buf, bs.soff, bq.buf, bq.qoff, bxh, by, ka, bcnt, bbase, rows, ne)
                } else {
                    pf_enc_moe_mm_k6_th_c(enc, bs.buf, bs.doff, bs.buf, bs.soff, bq.buf, bq.qoff, bxh, by, ka, bcnt, bbase, rows, npos, ne)
                }
            } else {
                pf_enc_moe_mm_k6_t_c(enc, bs.buf, bs.doff, bs.buf, bs.soff, bq.buf, bq.qoff, bx, by, ka, bcnt, bbase, rows, npos, ne)
            }
        } else {
            pf_enc_moe_mm_k6_c(enc, bs.buf, bs.doff, bs.buf, bs.soff, bq.buf, bq.qoff, bx, by, ka, bcnt, bbase, bbkt, rows, npos, ne)
        }
    } elif (fmt == KqFmt.k4) {
        let bq = kq_quants_of(g_dev, t, fmt, stack_off)
        let bs = kq_scales_of(g_dev, t, fmt, stack_off)
        if (contiguous && g_pf_pso_moe_mm_k4_t != null) {
            if (bxh != null && g_pf_pso_moe_mm_k4_th != null) {
                if (pf_moe_tall_pick(npos, int64(ka.nk), ne, g_pf_pso_moe_mm_k4_th128, g_pf_pso_moe_mm_k4_thr)) {
                    pf_enc_moe_mm_k4_th128_c(enc, bs.buf, bs.soff, bs.buf, bs.soff, bq.buf, bq.qoff, bxh, by, ka, bcnt, bbase, rows, npos, ne)
                    pf_enc_moe_mm_k4_thr_c(enc, bs.buf, bs.soff, bs.buf, bs.soff, bq.buf, bq.qoff, bxh, by, ka, bcnt, bbase, rows, ne)
                } else {
                    pf_enc_moe_mm_k4_th_c(enc, bs.buf, bs.soff, bs.buf, bs.soff, bq.buf, bq.qoff, bxh, by, ka, bcnt, bbase, rows, npos, ne)
                }
            } else {
                pf_enc_moe_mm_k4_t_c(enc, bs.buf, bs.soff, bs.buf, bs.soff, bq.buf, bq.qoff, bx, by, ka, bcnt, bbase, rows, npos, ne)
            }
        } else {
            pf_enc_moe_mm_k4_c(enc, bs.buf, bs.soff, bs.buf, bq.buf, bq.qoff, bx, by, ka, bcnt, bbase, bbkt, rows, npos, ne)
        }
    } elif (fmt == KqFmt.k5) {
        let bq = kq_quants_of(g_dev, t, fmt, stack_off)
        let bs = kq_scales_of(g_dev, t, fmt, stack_off)
        if (contiguous && g_pf_pso_moe_mm_k5_t != null) {
            if (bxh != null && g_pf_pso_moe_mm_k5_th != null) {
                if (pf_moe_tall_pick(npos, int64(ka.nk), ne, g_pf_pso_moe_mm_k5_th128, g_pf_pso_moe_mm_k5_thr)) {
                    pf_enc_moe_mm_k5_th128_c(enc, bs.buf, bs.soff, bs.buf, bs.soff, bq.buf, bq.qoff, bxh, by, ka, bcnt, bbase, rows, npos, ne)
                    pf_enc_moe_mm_k5_thr_c(enc, bs.buf, bs.soff, bs.buf, bs.soff, bq.buf, bq.qoff, bxh, by, ka, bcnt, bbase, rows, ne)
                } else {
                    pf_enc_moe_mm_k5_th_c(enc, bs.buf, bs.soff, bs.buf, bs.soff, bq.buf, bq.qoff, bxh, by, ka, bcnt, bbase, rows, npos, ne)
                }
            } else {
                pf_enc_moe_mm_k5_t_c(enc, bs.buf, bs.soff, bs.buf, bs.soff, bq.buf, bq.qoff, bx, by, ka, bcnt, bbase, rows, npos, ne)
            }
        } else {
            pf_enc_moe_mm_k5_c(enc, bs.buf, bs.soff, bs.buf, bq.buf, bq.qoff, bx, by, ka, bcnt, bbase, bbkt, rows, npos, ne)
        }
    } else {
        panic("metal moe prefill: expert plane format {fmt} has no Metal expert kernel - moe_metal_ok admitted it (moe_fmt_metal_served)")
    }
}

//! mint the contiguous gather-X panel once per layer - up and gate share it;
//! null = the caller keeps the in-kernel gather form.
[arch(at="../ARCHITECTURE_GPU_PREFILL_MOE.md#prefill-moe-buckets")]
def private pf_moe_gather_panel(enc : MetalComputeEncoder?; bbkt, bx : MetalBuffer?; var bmxg : MetalBuffer?;
                                th_ok : bool; dim, nk, ne, mpad, npos : int64) : MetalBuffer? {
    if (!th_ok || bmxg == null || g_pf_pso_moe_gx == null) {
        return null
    }
    var gxka = MoeGxArgs(dim = uint(dim), nk = uint(nk), ne = uint(ne), total = uint(npos * nk))
    if (g_pf_skip != "moe_gather") {   //! attribution: stale panel, twin path unchanged
        enc_moe_gather_x(enc, bbkt, bx, bmxg, gxka, mpad)
    }
    return bmxg
}

def private pf_enc_moe_mm_mx4(enc : MetalComputeEncoder?; t : Model; stack_off, rows, npos : int64;
                              bx, by, bcnt, bbase, bbkt, bwb : MetalBuffer?; var ka : MoeMmArgs;
                              contiguous : bool = false; bxh : MetalBuffer? = null) {
    return if (g_pf_skip == "moe_mm_disp")   //! attribution: gather+cvt kept, mm dispatches out
    let ne = t.config.n_expert
    let mp = mx4_of(g_dev, t, stack_off)
    if (g_pf_moe_mm_mx4_tensor && contiguous && g_pf_pso_moe_mm_mx4_t != null) {
        if (bxh != null && g_pf_pso_moe_mm_mx4_th != null) {
            if (pf_moe_tall_pick(npos, int64(ka.nk), ne, g_pf_pso_moe_mm_mx4_th128, g_pf_pso_moe_mm_mx4_thr)) {
                pf_enc_moe_mm_mx4_th128_c(enc, mp.qbuf, mp.qoff, mp.sbuf, mp.soff, bxh, by, ka, bcnt, bbase, bwb, g_mx4_vtab, rows, npos, ne)
                pf_enc_moe_mm_mx4_thr_c(enc, mp.qbuf, mp.qoff, mp.sbuf, mp.soff, bxh, by, ka, bcnt, bbase, bwb, g_mx4_vtab, rows, ne)
            } else {
                pf_enc_moe_mm_mx4_th_c(enc, mp.qbuf, mp.qoff, mp.sbuf, mp.soff, bxh, by, ka, bcnt, bbase, bwb, g_mx4_vtab, rows, npos, ne)
            }
        } else {
            pf_enc_moe_mm_mx4_t_c(enc, mp.qbuf, mp.qoff, mp.sbuf, mp.soff, bx, by, ka, bcnt, bbase, bwb, g_mx4_vtab, rows, npos, ne)
        }
    } else {
        pf_enc_moe_mm_mx4_c(enc, mp.qbuf, mp.qoff, mp.sbuf, mp.soff, bx, by, ka, bcnt, bbase, bbkt, bwb, g_mx4_vtab, rows, npos, ne)
    }
}








def enc_qk_mm(enc : MetalComputeEncoder?; bq, bk, batt : MetalBuffer?; var ka : AttnArgs; mp, nk64, heads : int64; bqh : MetalBuffer? = null) {   //! public: the tower driver dispatches the crowned trio too
    if (g_pf_qkmm_tensor && g_pf_pso_qkmm_t != null) {
        if (bqh != null && g_pf_pso_qkmm_th != null) {
            enc_qk_mm_th_c(enc, bqh, bk, batt, ka, mp, nk64, heads)
        } else {
            enc_qk_mm_t_c(enc, bq, bk, batt, ka, mp, nk64, heads)
        }
    } else {
        enc_qk_mm_c(enc, bq, bk, batt, ka, mp, nk64, heads)
    }
}

//! Whether the device pair can serve: its crown, no hold, and every stamp of the trio compiled.
def pf_attn_dev_ready() : bool {
    return g_pf_attn_dev && !pf_held("attn_dev") && psos_ready(fixed_array(g_pf_pso_half_rows, g_pf_pso_qk_dev,
        g_pf_pso_qk_dev128, g_pf_pso_pexp, g_pf_pso_av_dev, g_pf_pso_av_dev128, g_pf_pso_av_dev_t, g_pf_pso_av_dev128_t))
}

//! the 128-row stamp where the query panel divides by 128, the 32-row stamp otherwise
[arch(at="../ARCHITECTURE_GPU_PREFILL_WINDOW.md#prefill-attn-device")]
def private enc_qk_dev(enc : MetalComputeEncoder?; bqh, bkh, batt : MetalBuffer?; var ka : AttnArgs; mp, nk64, heads : int64; koff : uint64 = 0ul) {
    if (mp % 128l == 0l) {
        enc_qk_dev128_c(enc, bqh, bkh, koff, batt, ka, mp, nk64, heads)
    } else {
        enc_qk_dev_c(enc, bqh, bkh, koff, batt, ka, mp, nk64, heads)
    }
}

[arch(at="../ARCHITECTURE_GPU_PREFILL_WINDOW.md#prefill-attn-device")]
def private enc_av_dev(enc : MetalComputeEncoder?; batt, bvh, bxb : MetalBuffer?; var ka : AttnArgs; bstat : MetalBuffer?; mp, heads, head_size : int64; voff : uint64 = 0ul) {
    let ntile = head_size / 64l   //! the head's whole 64-wide tiles; the grid is theirs, not a ceiling that would write past the head
    if (mp % 128l == 0l) {
        enc_av_dev128_c(enc, batt, bvh, voff, bxb, ka, bstat, mp, heads, ntile)
    } else {
        enc_av_dev_c(enc, batt, bvh, voff, bxb, ka, bstat, mp, heads, ntile)
    }
    if (head_size % 64l != 0l) {
        if (mp % 128l == 0l) {
            enc_av_dev128_t_c(enc, batt, bvh, voff, bxb, ka, bstat, mp, heads, head_size)
        } else {
            enc_av_dev_t_c(enc, batt, bvh, voff, bxb, ka, bstat, mp, heads, head_size)
        }
    }
}

//! The all-heads FastConformer attention's scratch: the five half operands, the two score slabs and the stat plane.
struct FcDevPlanes {
    quh : MetalBuffer?
    qvh : MetalBuffer?
    kh : MetalBuffer?
    vh : MetalBuffer?
    ph : MetalBuffer?
    sc : MetalBuffer?      //!< [heads x mp x nkp] halves: content scores, then exp(s - max)
    sr : MetalBuffer?      //!< [heads x mp x wwpad] halves: rel scores
    stat : MetalBuffer?    //!< [heads x mp] floats
}

//! Whether the all-heads FastConformer attention can serve: the device pair and its two own stamps.
def pf_fc_dev_ready() : bool => pf_attn_dev_ready() && psos_ready(fixed_array(g_pf_pso_fc_twin, g_pf_pso_fc_pexp))

//! The FastConformer rel-pos attention over every head in five dispatches: the half operands, the
//! content and rel score GEMMs of the device pair, the rel-shift softmax, the device AV into the compact output.
[arch(at="../ARCHITECTURE_GPU_TOWER.md#tower-encode-chains")]
def pf_enc_fc_attn_dev(enc : MetalComputeEncoder?; bq, bk, bv, bposl, bblob : MetalBuffer?; buoff, bvoff : uint64;
                       p : FcDevPlanes; bao : MetalBuffer?; var fa : FcAttnArgs; heads : int64) {
    let mp = int64(fa.mp)
    let nkp = int64(fa.nkp)
    let wwpad = int64(fa.wwpad)
    enc_fc_twin(enc, bq, bk, bv, bposl, bblob, buoff, bblob, bvoff, p.quh, p.qvh, p.kh, p.vh, p.ph, fa, wwpad * int64(fa.d))
    //! every query attends every live key: the uniform span is the whole row, so no tile is skipped and the AV reduces over [0, npos)
    var ka = AttnArgs(qd = fa.d, kv_dim = fa.d, head_size = fa.hs, kv_mul = 1u, npos = fa.npos, np32 = fa.nkp, scale = fa.scale,
        qoff = 0u, qrows = fa.mp, window = 0u, softcap = 0.0, hass = 0u, uend = fa.npos, ulo = 0u)
    var kr = ka
    kr.np32 = fa.wwpad
    kr.uend = fa.wwpad
    enc_qk_dev(enc, p.quh, p.kh, p.sc, ka, mp, nkp, heads)
    enc_qk_dev(enc, p.qvh, p.ph, p.sr, kr, mp, wwpad, heads)
    enc_fc_pexp(enc, p.sc, p.sr, p.stat, fa, mp, heads)
    enc_av_dev(enc, p.sc, p.vh, bao, ka, p.stat, mp, heads, int64(fa.hs))
}

[arch(at="../ARCHITECTURE_GPU_PREFILL.md#prefill-attn-slab")]
def enc_av_mm(enc : MetalComputeEncoder?; batt, bv, bxb : MetalBuffer?; var ka : AttnArgs; bstat : MetalBuffer?; mp, heads, head_size : int64) {   //! public: the tower driver dispatches the crowned trio too
    let walk_live = ka.qoff + ka.qrows == ka.npos
    if (g_pf_avmm_tensor && g_pf_pso_avmm_t != null && ka.head_size % 64u == 0u) {
        if (walk_live && ka.npos % 64u == 0u && g_pf_pso_avmm_tp != null) {
            enc_av_mm_tp_c(enc, batt, bv, bxb, ka, bstat, mp, heads, head_size)
        } else {
            enc_av_mm_t_c(enc, batt, bv, bxb, ka, bstat, mp, heads, head_size)
        }
    } elif (walk_live && ka.npos % 32u == 0u && g_pf_pso_avmm_p != null) {
        enc_av_mm_p_c(enc, batt, bv, bxb, ka, bstat, mp, heads, head_size)
    } else {
        enc_av_mm_c(enc, batt, bv, bxb, ka, bstat, mp, heads, head_size)
    }
}



//! Batched f32-slab GEMV on MetalMoeRouterB's builder: d rows per position, y[p*d + row].
def private pf_enc_slab_gemv_b(enc : MetalComputeEncoder?; bw, bx, by, bn, bd, bnp : MetalBuffer?; d, npos : int64) {
    enc_moe_router_b(enc, bw, bx, by, bn, bd, bn, g_one, g_zero, bnp, d, npos)   //! bxs = bn: per-position x stride = dim
}


def private ple_gather_pso_of(fmt : KqFmt) : MetalComputePipeline? {
    if (fmt == KqFmt.q8) return g_pf_pso_ple_gather
    if (fmt == KqFmt.k4) return g_pf_pso_ple_gather_k4
    if (fmt == KqFmt.k5) return g_pf_pso_ple_gather_k5
    if (fmt == KqFmt.q40) return g_pf_pso_ple_gather_q40
    return null
}

def private pf_enc_ple_gather_fmt(enc : MetalComputeEncoder?; t : Model; btok, by : MetalBuffer?;
                                  var ka : PleArgs; nlayers, npos, ple : int64) {
    let fmt = t.ple_emb_fmt
    if (fmt == KqFmt.q8) {
        let bemb = blob_of(g_dev, t, t.ple_emb_off)
        pf_enc_ple_gather(enc, bemb.buf, bemb.boff, bemb.buf, btok, by, ka, nlayers, npos, ple)
        return
    }
    let bq = kq_quants_of(g_dev, t, fmt, t.ple_emb_off)
    let bs = kq_scales_of(g_dev, t, fmt, t.ple_emb_off)
    if (fmt == KqFmt.k4) {
        pf_enc_ple_gather_k4(enc, bs.buf, bs.soff, bs.buf, btok, by, ka, bq.buf, bq.qoff, nlayers, npos, ple)
    } elif (fmt == KqFmt.k5) {
        pf_enc_ple_gather_k5(enc, bs.buf, bs.soff, bs.buf, btok, by, ka, bq.buf, bq.qoff, nlayers, npos, ple)
    } elif (fmt == KqFmt.q40) {
        pf_enc_ple_gather_q40(enc, bs.buf, bs.soff, btok, by, ka, bq.buf, bq.qoff, nlayers, npos, ple)
    } else {
        panic("metal ple prefill: per-layer embedding table format {fmt} has no gather twin - ple_gather_pso_of admitted it")
    }
}

//! true iff THIS window's metal prefill will engage AND the pre-step kernels cover the model
[unused_argument(s, start_pos)]
def private metal_ple_pre_gpu_gate(t : Model; s : Session; npos, start_pos : int64) : bool {
    if (active_prefill_override() != "metal" || !t.metal_blob || !t.ple_model_bf16) {
        return false
    }
    let ple = t.config.n_embd_per_layer
    let all = t.config.n_layers * ple
    //! the kq planes bind at the table's base, so base and row stride must both be whole superblocks - that is what makes the kernel's el/256 its plane superblock
    let kq_aligned = t.ple_emb_fmt == KqFmt.q8 || (all % 256l == 0l && t.ple_emb_off % 256l == 0l)
    if (ple % 32l != 0l || ple > 1024l || all % 64l != 0l || !kq_aligned ||
            prefill_decline_caps(t, npos) != MetalPrefillDecline.none) {
        return false
    }
    return metal_prefill_init() && ple_gather_pso_of(t.ple_emb_fmt) != null
}

//! the hyper-connection window (qwen4exp): the wide residual rows and the mixer's row scratch
struct private PfHc {
    on : bool
    bhc : MetalBuffer?        // [mp x hc x dim] the wide residual
    bxn : MetalBuffer?        // [mp x hc x dim] the normed streams
    blo : MetalBuffer?        // [mp x low_rank]
    bgate : MetalBuffer?      // [mp x hc x dim] the up projection
    binj_a : MetalBuffer?     // [mp x hc] the attention site's scatter logits
    binj_f : MetalBuffer?     // [mp x hc] the ffn site's
    u_hcd : MetalBuffer?
    u_lr : MetalBuffer?
    u_hc : MetalBuffer?
    u_np : MetalBuffer?       // npos
    u_lo_tot : MetalBuffer?   // npos x low_rank
    u_tot : MetalBuffer?      // npos x hc x dim
    u_one : MetalBuffer?
    mp : int64
    npos : int64
}

//! the n-gram side input's window rows (qwen4exp)
struct private PfPle {
    on : bool
    bemb : MetalBuffer?     // [npos x in] the gathered heads (host-poked)
    bkey : MetalBuffer?     // [mp x hc x dim]
    bval : MetalBuffer?     // [mp x dim]
    bgated : MetalBuffer?   // [mp x hc x dim]
    bcin : MetalBuffer?     // [mp x hc x dim] the window's conv-input rows
    u_in : MetalBuffer?
}

//! One mixer over the window's wide rows into `by` (the block input rows), its scatter logits into `binj`
//! (inject_off < 0 = the head mixer, no scatter): the grouped norm, the down GEMM, silu(lo / hc), the up GEMM, the gated stream mean
def private pf_hc_mix(enc : MetalComputeEncoder?; t : Model; h : PfHc; norm_off, down_off, up_off, inject_off : int64; by, binj, u_dim, u_eps : MetalBuffer?) {
    let c = t.config
    let hcd = hc_dim(c)
    let lr = c.hc_low_rank
    unsafe {
        if (g_pf_skip != "ew" && g_pf_skip != "nongemm") {
            var bw_n = upload_region(addr < void? >(t.fblob[norm_off]), uint64(hcd * 4l))
            enc_hc_norm(enc, h.bhc, bw_n, h.bxn, u_dim, u_eps, h.u_hc, h.npos * c.hc_count)
        }
        if (g_pf_skip != "gemm") {
            if (inject_off >= 0l) {
                var bw_i = upload_region(addr < void? >(t.fblob[inject_off]), uint64(c.hc_count * hcd * 4l))
                pf_enc_slab_gemv_b(enc, bw_i, h.bxn, binj, h.u_hcd, h.u_hc, h.u_np, c.hc_count, h.npos)
            }
            let bwd = blob_of(g_dev, t, down_off)
            enc_gemm_mm(enc, bwd.buf, bwd.boff, h.bxn, h.blo, h.u_hcd, h.u_lr, h.mp, lr, 0ul, hcd, h.npos, pf_cvt_panel(enc, h.bxn, h.mp, hcd, g_pf_skip == "act_cvt"))
        }
        if (g_pf_skip != "ew" && g_pf_skip != "nongemm") {
            enc_hc_lo(enc, h.blo, 0ul, h.u_lo_tot, h.u_hc, h.npos * lr)
        }
        if (g_pf_skip != "gemm") {
            let bwu = blob_of(g_dev, t, up_off)
            enc_gemm_mm(enc, bwu.buf, bwu.boff, h.blo, h.bgate, h.u_lr, h.u_hcd, h.mp, hcd, 0ul, lr, h.npos, pf_cvt_panel(enc, h.blo, h.mp, lr, g_pf_skip == "act_cvt"))
        }
        if (g_pf_skip != "ew" && g_pf_skip != "nongemm") {
            enc_hc_mix(enc, h.bxn, h.bgate, by, u_dim, h.u_hc, h.u_np, h.npos * c.dim)
        }
    }
}

//! layer l's mixer at its attention (ffn = false) or ffn site - the CPU path's mix_prefill
def private pf_hc_mix_site(enc : MetalComputeEncoder?; t : Model; h : PfHc; l : int64; ffn : bool; by, binj, u_dim, u_eps : MetalBuffer?) {
    let site = l * 2l + (ffn ? 1l : 0l)
    let hcd = hc_dim(t.config)
    pf_hc_mix(enc, t, h, t.hc_norm_off + site * hcd, t.hc_down_offs[site], t.hc_up_offs[site],
        t.hc_inject_off + site * hcd * t.config.hc_count, by, binj, u_dim, u_eps)
}

//! a block's output rows scattered into the streams under the site's logits
def private pf_hc_combine(enc : MetalComputeEncoder?; t : Model; h : PfHc; bout, binj, u_dim : MetalBuffer?) {
    if (g_pf_skip != "ew" && g_pf_skip != "nongemm") {
        enc_hc_combine(enc, h.bhc, bout, binj, u_dim, h.u_hc, h.u_np, h.npos * hc_dim(t.config))
    }
}

//! the head mixer over the window's last wide row into bxf (the classifier's input): the single row's
//! chain over row 0 of the mixer scratch, the row copied out of the wide residual first
def private pf_hc_head(enc : MetalComputeEncoder?; t : Model; h : PfHc; bxf, u_dim, u_eps : MetalBuffer?) {
    let c = t.config
    let hcd = hc_dim(c)
    let lr = c.hc_low_rank
    enc_copy_row(enc, h.bhc, uint64((h.npos - 1l) * hcd * 4l), h.bgate, 0ul, h.u_hcd, hcd)
    var bw_n = upload_region(unsafe(addr < void? >(t.fblob[t.hc_head_norm_off])), uint64(hcd * 4l))
    enc_hc_norm(enc, h.bgate, bw_n, h.bxn, u_dim, u_eps, h.u_hc, c.hc_count)
    enc_site_gemv(enc, t, KqFmt.q8, t.hc_head_down_off, lr, hcd, h.bxn, h.blo, h.u_hcd, h.u_lr)
    enc_hc_lo(enc, h.blo, 0ul, h.u_lr, h.u_hc, lr)
    enc_site_gemv(enc, t, KqFmt.q8, t.hc_head_up_off, hcd, lr, h.blo, h.bgate, h.u_lr, h.u_hcd)
    enc_hc_mix(enc, h.bxn, h.bgate, bxf, u_dim, h.u_hc, h.u_one, c.dim)
}

//! The n-gram side input over the window, before layer ple_layer's attention mixer: the key/value GEMMs
//! off the gathered heads, the per-(row, stream) gate into the window panel, the dilated conv over the
//! panel and the ring, then the window's last rows into the ring (one slot each) for the rows after it
def private pf_ple_side(enc : MetalComputeEncoder?; t : Model; h : PfHc; p : PfPle; u_dim : MetalBuffer?; start_pos : int64; bring : MetalBuffer?; ring_base : uint64) {
    let c = t.config
    let hcd = hc_dim(c)
    let ind = ple_in_dim(c)
    let rows = ple_hist_rows(c)
    unsafe {
        if (g_pf_skip != "gemm") {
            let bxh_e = pf_cvt_panel(enc, p.bemb, h.mp, ind, g_pf_skip == "act_cvt")
            let bwk = blob_of(g_dev, t, t.ple_key_off)
            enc_gemm_mm(enc, bwk.buf, bwk.boff, p.bemb, p.bkey, p.u_in, h.u_hcd, h.mp, hcd, 0ul, ind, h.npos, bxh_e)
            let bwv = blob_of(g_dev, t, t.ple_val_off)
            enc_gemm_mm(enc, bwv.buf, bwv.boff, p.bemb, p.bval, p.u_in, u_dim, h.mp, c.dim, 0ul, ind, h.npos, bxh_e)
        }
        if (g_pf_skip != "ew" && g_pf_skip != "nongemm") {
            var bwk = upload_region(addr < void? >(t.fblob[t.ple_nkey_off]), uint64(hcd * 4l))
            var bwq = upload_region(addr < void? >(t.fblob[t.ple_nquery_off]), uint64(hcd * 4l))
            var bwc = upload_region(addr < void? >(t.fblob[t.ple_nconv_off]), uint64(hcd * 4l))
            var gka = PleGateArgs(dim = uint(c.dim), hc = uint(c.hc_count), eps = c.norm_eps, pos0 = uint(start_pos), rows = uint(rows), direct = 1u)
            enc_ple_gate(enc, h.bhc, 0ul, p.bkey, 0ul, p.bval, 0ul, bwk, bwq, bwc, p.bgated, 0ul, p.bcin, 0ul, gka, h.npos * c.hc_count)
            var bwt = upload_region(addr < void? >(t.fblob[t.ple_conv_off]), uint64(hcd * c.ple_conv_kernel * 4l))
            var cka = PleConvArgs(dim = uint(c.dim), hc = uint(c.hc_count), kern = uint(c.ple_conv_kernel), dil = uint(c.ple_ngram_size),
                rows = uint(rows), pos0 = uint(start_pos), nrows = uint(h.npos), direct = 1u)
            enc_ple_conv(enc, h.bhc, 0ul, p.bgated, 0ul, p.bcin, 0ul, bring, ring_base, bwt, cka, h.npos * hcd)
            for (r in range64(max(0l, h.npos - rows), h.npos)) {
                enc_copy_row(enc, p.bcin, uint64(r * hcd * 4l), bring, ring_base + uint64(((start_pos + r) % rows) * hcd * 4l), h.u_hcd, hcd)
            }
        }
    }
}

[hot_path, arch(at="../ARCHITECTURE_GPU_PREFILL.md#prefill-pad-rows-and-coop"), arch(at="../ARCHITECTURE_GPU_PREFILL.md#prefill-chunked-submit"), arch(at="../ARCHITECTURE_GPU_PREFILL.md#prefill-twin-dual-store"), arch(at="../ARCHITECTURE_GPU_PREFILL.md#prefill-last-row-tail")]
def metal_prefill_forward(t : Model; var s : Session; npos, start_pos : int64) : bool {   // nolint:STYLE037,STYLE038 — the prefill step driver; the layer stack's phases are hz-barrier coupled
    var why = prefill_decline(t, s, npos, start_pos)
    if (why == MetalPrefillDecline.none && !metal_prefill_init()) {
        why = MetalPrefillDecline.device
    }
    if (why == MetalPrefillDecline.none && !t.blocks.ffn_is_dense && !pf_moe_twins_ready(t)) {
        why = MetalPrefillDecline.moe_twin
    }
    if (why == MetalPrefillDecline.none) {
        pf_devw_seed_baked(t)   //! baked image panels land in the resident cache before any encode
    }
    if (why != MetalPrefillDecline.none) {
        if (s.ple_gpu_pending) {   //! the gate promised a device pre-step — build it on CPU first
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
    residency_flush()   //! pin/commit before the first submission (the post-CPU-window slack)
    let ts_all = ref_time_ticks()
    let c = t.config
    let dim = c.dim
    let head_size = c.head_size
    let n_heads = c.n_heads
    let qd = n_heads * head_size
    let kv_dim = c.n_kv_heads * head_size
    let hidden = layer_hidden(t, 0l)
    let hid2 = ffn_second_hidden(t)   //! decline guarantees 0 (uniform) or the one other width
    let hid_mx = max(hidden, hid2)
    let kv_mul = uint(n_heads / c.n_kv_heads)
    let scale = c.attn_scale > 0.0 ? c.attn_scale : 1.0 / sqrt(float(head_size))
    let half = (c.rope_dim > 0l ? c.rope_dim : head_size) / 2l   //! partial rope: table rows are rope_dim/2 wide
    //! the recurrent-state mirror syncs BEFORE encoding - a cold mid-stream window has no repair path
    let dn = c.recr_mask != 0ul
    var bdn_state : MetalBuffer?
    var dn_state_base = 0ul
    var dn_conv_base = 0ul
    var ple_ring_base = 0ul
    if (dn) {
        var dm = dn_mirror_prepare(s, start_pos, dn_mirror_regions(t))
        if (!dm.ok) {
            decline(MetalPrefillDecline.dn_state)
            return false
        }
        bdn_state = dm.buf
        dn_state_base = dm.state_off
        dn_conv_base = dm.conv_off
        ple_ring_base = dm.ple_off
    }
    let hs_sw = c.head_size_swa > 0l ? c.head_size_swa : head_size
    let kvh_sw = c.n_kv_heads_swa > 0l ? c.n_kv_heads_swa : c.n_kv_heads
    let qd_sw = n_heads * hs_sw
    let kvd_sw = kvh_sw * hs_sw
    let hetero = hs_sw != head_size || kvh_sw != c.n_kv_heads
    let qd_mx = max(qd, qd_sw)
    let half_sw = hs_sw / 2l
    let mp = ((npos + 31l) / 32l) * 32l
    let nk = start_pos + npos
    let nk64 = ((nk + 63l) / 64l) * 64l
    //! the mirror-fed form: the chunk's K/V rows land in the session's KV mirror as halves and the device pair reads every key there
    var mirk = (g_pf_env_mirror && s.kv_dtype_k == KVDtype.f16 && s.kv_dtype_v == KVDtype.f16 && !s.device_kv
        && (head_size % 32l) == 0l && (hs_sw % 32l) == 0l && g_pf_env_attn && pf_attn_dev_ready()   //! the device pair takes a head off the 64 lattice through its tail stamp
        && nk64 <= c.seq_len && g_pf_env_half_x && pf_twin_panels_wanted())   //! the Q twin rides a scratch panel, which covers the widest A panel
    var mir_koff = 0ul
    var mir_voff = 0ul
    var mir_cap = 0l
    if (mirk) {
        if (!mirror_stands(s, start_pos, nk64)) {   //! slices move or rows upload: no step may be writing one
            mirror_quiesce()
        }
        let mir = mirror_prepare_rows(t, s, start_pos, nk64)
        mirk = mir.ok
        mir_koff = mir.koff
        mir_voff = mir.voff
        mir_cap = mir.cap
    }
    let kvrows = mirk ? mp : max(nk64, start_pos + mp)
    let bytes_x = uint64(mp * dim * 4l)
    let bytes_xb = uint64(mp * max(dim, qd_mx) * 4l)   //! dim-wide norm outs AND the qd-wide AV out
    let bytes_q = uint64(mp * qd_mx * 4l)
    let nsh = !t.blocks.ffn_is_dense ? c.n_ff_shexp : 0l   //! shared expert (qwen35moe) rides bhb/bhb2
    let bytes_h = uint64(mp * max(hid_mx, nsh) * 4l)
    let bytes_att = uint64(n_heads * mp * nk64 * 2l)   //! per-head f16 score slabs: mp queries x nk64 keys
    let bytes_stat = uint64(n_heads * mp * 2l * 4l)   //! per-row softmax stats: rowmax + 1/sum
    let bytes_tab = uint64(npos * half * 4l)
    let bytes_tab_sw = uint64(npos * half_sw * 4l)   //! the swa pair's row width is ITS class's hs/2
    var bx = pool_acquire_pinned(g_pf_pool, g_dev, bytes_x)
    var bxb = pool_acquire_pinned(g_pf_pool, g_dev, bytes_xb)
    var bxb2 = pool_acquire_pinned(g_pf_pool, g_dev, bytes_x)
    var bq = pool_acquire_pinned(g_pf_pool, g_dev, bytes_q)
    var bhb = pool_acquire_pinned(g_pf_pool, g_dev, bytes_h)
    var bhb2 = pool_acquire_pinned(g_pf_pool, g_dev, bytes_h)
    //! bxb2 doubles as g4moe's pre_ffn2 expert input - the serial encoder makes the reuse safe
    let moe = !t.blocks.ffn_is_dense
    let g4moe = moe && c.moe_dense_shexp
    let mtot = moe ? npos * c.n_expert_used : 0l
    let mpad = moe ? ((mtot + c.n_expert * 31l + 31l) / 32l) * 32l : 0l
    let bytes_mlg = uint64(mp * c.n_expert * 4l)   //! the router's GEMM lands every padded row
    let bytes_msel = uint64(mtot * 2l * 4l)
    let bytes_mcsr = uint64((c.n_expert + 1l) * 4l)   // basep carries the padded total at [ne]
    let bytes_mbkt = uint64(mpad * 4l)
    let bytes_minv = uint64(mtot * 4l)
    let bytes_mg = uint64(mpad * c.n_ff_exp * 4l)
    let bytes_mdn = uint64(mpad * dim * 4l)
    var bmlg = moe ? pool_acquire_pinned(g_pf_pool, g_dev, bytes_mlg) : null
    var bmsel = moe ? pool_acquire_pinned(g_pf_pool, g_dev, bytes_msel) : null
    var bmcnt = moe ? pool_acquire_pinned(g_pf_pool, g_dev, bytes_mcsr) : null
    var bmbase = moe ? pool_acquire_pinned(g_pf_pool, g_dev, bytes_mcsr) : null
    var bmbkt = moe ? pool_acquire_pinned(g_pf_pool, g_dev, bytes_mbkt) : null
    var bminv = moe ? pool_acquire_pinned(g_pf_pool, g_dev, bytes_minv) : null
    var bmg = moe ? pool_acquire_pinned(g_pf_pool, g_dev, bytes_mg) : null
    var bmg2 = moe ? pool_acquire_pinned(g_pf_pool, g_dev, bytes_mg) : null
    var bmdn = moe ? pool_acquire_pinned(g_pf_pool, g_dev, bytes_mdn) : null
    //! acquire only when a contiguous twin can consume it - without a crowned _th twin the panel
    //! would sit wired and untouched
    let mxg_th = (g_pf_pso_moe_mm_mx4_th != null || g_pf_pso_moe_mm_q8_th != null
        || g_pf_pso_moe_mm_k4_th != null || g_pf_pso_moe_mm_k5_th != null || g_pf_pso_moe_mm_k6_th != null
        || pf_moe_split_th_any())
    let want_mxg = moe && g_pf_pso_moe_gx != null && (npos >= g_pf_mxg_min || pf_moe_model_split(t)) && mxg_th
    let bytes_mxg = uint64(mpad * dim * 2l)
    var bmxg = want_mxg ? pool_acquire_pinned(g_pf_pool, g_dev, bytes_mxg) : null
    var bmg4rt = g4moe ? pool_acquire_pinned(g_pf_pool, g_dev, bytes_x) : null
    //! the hyper-connection window: the wide residual and the mixer's row scratch; the side input's rows ride beside
    var hcs = PfHc(on = c.hyper_conn, mp = mp, npos = npos)
    let hcd = c.hyper_conn ? hc_dim(c) : 0l
    let hc_lr = c.hyper_conn ? c.hc_low_rank : 0l
    let bytes_hcd = uint64(mp * hcd * 4l)
    let bytes_hlr = uint64(mp * hc_lr * 4l)
    let bytes_hinj = uint64(mp * c.hc_count * 4l)
    if (hcs.on) {
        hcs.bhc = pool_acquire_pinned(g_pf_pool, g_dev, bytes_hcd)
        hcs.bxn = pool_acquire_pinned(g_pf_pool, g_dev, bytes_hcd)
        hcs.bgate = pool_acquire_pinned(g_pf_pool, g_dev, bytes_hcd)
        hcs.blo = pool_acquire_pinned(g_pf_pool, g_dev, bytes_hlr)
        hcs.binj_a = pool_acquire_pinned(g_pf_pool, g_dev, bytes_hinj)
        hcs.binj_f = pool_acquire_pinned(g_pf_pool, g_dev, bytes_hinj)
        hcs.u_hcd = uniform_u32(uint(hcd))
        hcs.u_lr = uniform_u32(uint(hc_lr))
        hcs.u_hc = uniform_u32(uint(c.hc_count))
        hcs.u_np = uniform_u32(uint(npos))
        hcs.u_lo_tot = uniform_u32(uint(npos * hc_lr))
        hcs.u_tot = uniform_u32(uint(npos * hcd))
        hcs.u_one = uniform_u32(1u)
    }
    var pls = PfPle(on = has_ple_ngram(c))
    let ple_ind = pls.on ? ple_in_dim(c) : 0l
    let bytes_pin = uint64(mp * ple_ind * 4l)
    if (pls.on) {
        pls.bemb = pool_acquire_untracked_pinned(g_pf_upool, g_dev, bytes_pin)
        pls.bkey = pool_acquire_pinned(g_pf_pool, g_dev, bytes_hcd)
        pls.bval = pool_acquire_pinned(g_pf_pool, g_dev, bytes_x)
        pls.bgated = pool_acquire_pinned(g_pf_pool, g_dev, bytes_hcd)
        pls.bcin = pool_acquire_pinned(g_pf_pool, g_dev, bytes_hcd)
        pls.u_in = uniform_u32(uint(ple_ind))
        unsafe {
            memcpy(metal_buffer_contents(pls.bemb), addr < void? >(s.ple_emb_b[0]), npos * ple_ind * 4l)
        }
    }
    let cd_dn = dn ? dn_conv_dim(c) : 0l
    let di_dn = dn ? c.ssm_d_inner : 0l
    let nvh_dn = dn ? c.ssm_dt_rank : 0l
    let bytes_dn_cd = uint64(mp * cd_dn * 4l)
    let bytes_dn_di = uint64(mp * di_dn * 4l)
    let bytes_dn_h = uint64(mp * nvh_dn * 4l)
    let bytes_dn_qg = uint64(mp * 2l * qd * 4l)
    var bdnqkv = dn ? pool_acquire_pinned(g_pf_pool, g_dev, bytes_dn_cd) : null
    var bdncv = dn ? pool_acquire_pinned(g_pf_pool, g_dev, bytes_dn_cd) : null
    let dn_half = dn && !pf_held("dn_half") && psos_ready(fixed_array(g_pso_dn_prep, g_pso_dn_scan_h, g_pf_pso_half_rows))
    var bdncvh = dn_half ? pool_acquire_pinned(g_pf_pool, g_dev, bytes_dn_cd / 2ul) : null
    var bdnz = dn ? pool_acquire_pinned(g_pf_pool, g_dev, bytes_dn_di) : null
    var bdno = dn ? pool_acquire_pinned(g_pf_pool, g_dev, bytes_dn_di) : null
    var bdnb = dn ? pool_acquire_pinned(g_pf_pool, g_dev, bytes_dn_h) : null
    var bdng = dn ? pool_acquire_pinned(g_pf_pool, g_dev, bytes_dn_h) : null
    var bdnqg = c.q_gated ? pool_acquire_pinned(g_pf_pool, g_dev, bytes_dn_qg) : null
    var bdngate = c.q_gated ? pool_acquire_pinned(g_pf_pool, g_dev, bytes_q) : null
    var batt = pool_acquire_pinned(g_pf_pool, g_dev, bytes_att)
    var bstats = pool_acquire_pinned(g_pf_pool, g_dev, bytes_stat)
    let nds = s.ds_active ? c.n_deepstack : 0l
    //! bds is GPU-written by the on-device slice, so it rides the TRACKED pool
    let wide = nds > 0l   //! a ds quantum always arrives as the wide borrow (forward_prefill_embd arms it with ds_active)
    if (wide && s.wide_src == null) {
        panic("metal_prefill_forward: ds_active with no wide borrow - forward_prefill_embd's contract broke")
    }
    let wide_stride = s.wide_stride
    let bytes_ds = uint64(npos * nds * dim * 4l)
    let bytes_wide = uint64(npos * wide_stride * 4l)
    var bds = wide ? pool_acquire_pinned(g_pf_pool, g_dev, bytes_ds) : null
    var bwide = wide ? pool_acquire_untracked_pinned(g_pf_upool, g_dev, bytes_wide) : null
    var bcos = pool_acquire_untracked_pinned(g_pf_upool, g_dev, bytes_tab)
    var bsin = pool_acquire_untracked_pinned(g_pf_upool, g_dev, bytes_tab)
    let dual_rope = has_dual_rope(c)
    var bcos_swa = dual_rope ? pool_acquire_untracked_pinned(g_pf_upool, g_dev, bytes_tab_sw) : null
    var bsin_swa = dual_rope ? pool_acquire_untracked_pinned(g_pf_upool, g_dev, bytes_tab_sw) : null
    var bks & = unsafe(g_pf_bks)
    var bvs & = unsafe(g_pf_bvs)
    bks |> clear()
    bvs |> clear()
    bks |> reserve(c.n_layers + c.n_layer_nextn)
    bvs |> reserve(c.n_layers + c.n_layer_nextn)
    var null_kv_panel : MetalBuffer?
    for (l in range64(c.n_layers + c.n_layer_nextn)) {
        if (layer_kv_dim(c, l) == 0l) {   //! recurrent layer (deltanet): no K/V at all
            bks |> push(null_kv_panel)
            bvs |> push(null_kv_panel)
            continue
        }
        if (kv_shared_layer(t, l)) {   //! E-series: alias the source layer's panels (same class geometry)
            bks |> push(bks[t.kv_src[l]])
            bvs |> push(bvs[t.kv_src[l]])
            continue
        }
        let bytes_kv_l = uint64(kvrows * layer_kv_dim(c, l) * 4l)   //! per-layer panels: each class's width
        bks |> push(pool_acquire_pinned(g_pf_pool, g_dev, bytes_kv_l))
        bvs |> push(pool_acquire_pinned(g_pf_pool, g_dev, bytes_kv_l))
    }
    //! the PLE side input uploads LAYER-major ([l][p][ple]) so the flat geglu reads layer l's
    //! block at one offset
    let plef = has_ple(c)
    let ple_n = c.n_embd_per_layer
    let ple_all = c.n_layers * ple_n
    let bytes_ple_side = uint64(c.n_layers * npos * ple_n * 4l)
    let bytes_ple_g = uint64(mp * ple_n * 4l)
    let bytes_ple_proj = uint64(mp * ple_all * 4l)
    let ple_gpu = plef && s.ple_gpu_pending   //! the gate stashed tokens — build the side input on device
    var bple : MetalBuffer?
    var bpleg : MetalBuffer?
    var bple_proj : MetalBuffer?
    var bple_tok : MetalBuffer?
    if (plef) {
        bpleg = pool_acquire_pinned(g_pf_pool, g_dev, bytes_ple_g)
        if (ple_gpu) {
            bple = pool_acquire_pinned(g_pf_pool, g_dev, bytes_ple_side)
            bple_proj = pool_acquire_pinned(g_pf_pool, g_dev, bytes_ple_proj)
            bple_tok = pool_acquire_untracked_pinned(g_pf_upool, g_dev, uint64(npos * 4l))
            unsafe {
                var tp = reinterpret<uint?>(metal_buffer_contents(bple_tok))
                for (p in range64(npos)) {
                    tp[p] = uint(s.ple_gpu_tokens[p])
                }
            }
        } else {
            bple = pool_acquire_untracked_pinned(g_pf_upool, g_dev, bytes_ple_side)
            unsafe {
                //! transpose s.ple_inp (position-major, p*all + l*ple) into layer-major blocks
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
    //! one f16 scratch covering the largest A panel any mul_mm site converts
    let bytes_xh = max(max(max(max(bytes_xb, bytes_h), uint64(mp * 2l * dim * 4l)),
        max(max(bytes_dn_di, bytes_ple_g), moe ? bytes_mg : 0ul)), max(bytes_hcd, bytes_pin)) / 2ul
    let want_xh = pf_twin_panels_wanted()
    g_pf_bxh = want_xh ? pool_acquire_pinned(g_pf_pool, g_dev, bytes_xh) : null
    g_pf_bxh2 = want_xh ? pool_acquire_pinned(g_pf_pool, g_dev, bytes_xh) : null
    g_pf_bxh_flip = false
    g_pf_bxh_bytes = want_xh ? bytes_xh : 0ul
    //! the largest layer-site N x K dequant output, capped at the big-panel ceiling: over-knee
    //! sites run as N-column TILES each under it, and the vocab classifier keeps the tg twin
    let wh_need = max(uint64(2l * max(dim, qd_mx) * max(hid_mx, max(nsh, max(2l * dim, di_dn)))),
        uint64(2l * hcd * max(hc_lr, ple_ind)))   //! the mixer's down/up and the side input's key panels are hc x dim wide
    let bytes_wh = min(wh_need, DEVW_BIG_PANEL)
    let want_wh = (g_pf_env_dev_w && g_pf_pso_hmm_th != null
        && (g_pf_pso_dq_q8 != null || pf_kq_dq_any()))
    g_pf_bwh = want_wh ? pool_acquire_pinned(g_pf_pool, g_dev, bytes_wh) : null
    g_pf_bwh2 = want_wh ? pool_acquire_pinned(g_pf_pool, g_dev, bytes_wh) : null
    g_pf_bwh_flip = false
    g_pf_bwh_bytes = want_wh ? bytes_wh : 0ul
    //! rope tables are built for the chunk's GLOBAL positions (build_rope_table takes start_pos)
    unsafe {
        if (wide) {   //! ONE wide upload; bx and the bds slices extract on-device at encode time
            memcpy(metal_buffer_contents(bwide), reinterpret<void const?>(s.wide_src + s.wide_soff * wide_stride), npos * wide_stride * 4l)
        } else {
            memcpy(metal_buffer_contents(bx), addr < void? >(s.x_b[0]), npos * dim * 4l)
        }
        memcpy(metal_buffer_contents(bcos), addr < void? >(s.rope_cos[0]), npos * half * 4l)
        memcpy(metal_buffer_contents(bsin), addr < void? >(s.rope_sin[0]), npos * half * 4l)
        if (dual_rope) {
            memcpy(metal_buffer_contents(bcos_swa), addr < void? >(s.rope_cos_swa[0]), npos * half_sw * 4l)
            memcpy(metal_buffer_contents(bsin_swa), addr < void? >(s.rope_sin_swa[0]), npos * half_sw * 4l)
        }
    }
    //! tq4 rows dequant ROTATED and un-rotate per head - GPU attention runs in the raw basis;
    //! K rows are stored ROPED, so no re-rope.
    if (start_pos > 0l && !mirk) {
        unsafe {
            let gather_nrun = kv_runs(s, 0l, start_pos, s.kv_runs)
            let runsg = addr < KVRun const? >(s.kv_runs[0])
            let sgp = (s.kv_dtype_k == KVDtype.tq4 || s.kv_dtype_v == KVDtype.tq4) ? addr < float const? >(s.kv_signs[0]) : null
            for (l in range64(c.n_layers + c.n_layer_nextn)) {
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
    //! layer-invariant uniforms (weight offsets ride the region cache, so nothing varies per layer)
    var u_dim = uniform_u32(uint(dim))
    var u_qd = uniform_u32(uint(qd))
    var u_kvd = uniform_u32(uint(kv_dim))
    var u_hidden = uniform_u32(uint(hidden))
    var u_hidden2 = hid2 > 0l ? uniform_u32(uint(hid2)) : null
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
    var u_wide_rw = wide ? uniform_u32(uint(wide_stride)) : null
    var u_wide_dim = wide ? uniform_u32(uint(dim)) : null
    var u_total_h = uniform_u32(uint(npos * hidden))
    var u_total_h2 = hid2 > 0l ? uniform_u32(uint(npos * hid2)) : null
    var u_moe_ne = moe ? uniform_u32(uint(c.n_expert)) : null
    var u_moe_nk = moe ? uniform_u32(uint(c.n_expert_used)) : null
    var u_moe_np = moe ? uniform_u32(uint(npos)) : null
    var u_moe_tot = moe ? uniform_u32(uint(mtot)) : null
    var u_moe_nfe = moe ? uniform_u32(uint(c.n_ff_exp)) : null
    var u_moe_g0 = moe ? uniform_u32(0u) : null
    var u_moe_ptot = moe ? uniform_u32(uint(mpad * c.n_ff_exp)) : null
    var u_moe_invd = g4moe ? uniform_f32(1.0 / sqrt(float(dim))) : null
    var u_moe_gmode = moe ? uniform_u32(c.moe_gate == MoeGate.softmax_weight ? 1u : (c.norm_topk_prob ? 0u : 2u)) : null
    var u_moe_nsh = nsh > 0l ? uniform_u32(uint(nsh)) : null
    var u_moe_shtot = nsh > 0l ? uniform_u32(uint(npos * nsh)) : null
    var u_moe_one = nsh > 0l ? uniform_u32(1u) : null
    var u_eps = uniform_f32(c.norm_eps)
    var u_dn_cd = dn ? uniform_u32(uint(cd_dn)) : null
    var u_dn_di = dn ? uniform_u32(uint(di_dn)) : null
    var u_dn_nvh = dn ? uniform_u32(uint(nvh_dn)) : null
    var u_dn_np = dn ? uniform_u32(uint(npos)) : null
    var u_dn_qd2 = c.q_gated ? uniform_u32(uint(2l * qd)) : null
    var u_qtot = c.q_gated ? uniform_u32(uint(npos * qd)) : null
    var u_rot = (c.rope_dim > 0l && c.rope_dim != head_size) ? uniform_u32(uint(c.rope_dim)) : null
    var u_ple = plef ? uniform_u32(uint(ple_n)) : null
    var u_ple_tot = plef ? uniform_u32(uint(npos * ple_n)) : null
    var u_ple_all = ple_gpu ? uniform_u32(uint(ple_all)) : null
    //! hetero-only sliding-class uniform twins; uniform models leave these null and bind the base set
    var u_qd_sw = hetero ? uniform_u32(uint(qd_sw)) : null
    var u_kvd_sw = hetero ? uniform_u32(uint(kvd_sw)) : null
    var u_hs_sw = hetero ? uniform_u32(uint(hs_sw)) : null
    var u_npairs_q_sw = hetero ? uniform_u32(uint(npos * qd_sw / 2l)) : null
    var u_npairs_k_sw = hetero ? uniform_u32(uint(npos * kvd_sw / 2l)) : null
    var u_vtot = vfromk ? uniform_u32(uint(npos * kv_dim)) : null
    var u_vtot_sw = (vfromk && hetero) ? uniform_u32(uint(npos * kvd_sw)) : null
    //! prefill_override_logits_done skips the CPU final step; q8 classifiers only, with softcap
    //! and suppress on epilogue kernels
    let cls_fmt = c.shared_weights ? t.emb_fmt : t.wcls_fmt
    let tied_f32 = c.shared_weights && !t.cls_q8 && cls_fmt == KqFmt.q8
    //! GPU logits stay on under the knockout rail (deltas stay clean), and a blob-only
    //! metal-flavor load has no CPU classifier to fall back to
    let gpu_logits = (!g_pf_logits_cpu && g_pf_env_logits &&
        !tied_f32 &&
        t.wcls_off >= 0l)
    var bxf = gpu_logits ? pool_acquire_pinned(g_pf_pool, g_dev, uint64(dim * 4l)) : null
    var blog = gpu_logits ? pool_acquire_pinned(g_pf_pool, g_dev, uint64(c.vocab_size * 4l)) : null
    var u_vocab = gpu_logits ? uniform_u32(uint(c.vocab_size)) : null
    var u_flcap = (gpu_logits && c.final_logit_softcap > 0.0) ? uniform_f32(c.final_logit_softcap) : null
    let nsupp = long_length(t.suppress)
    var bsupp : MetalBuffer?
    var u_nsupp : MetalBuffer?
    if (gpu_logits && nsupp > 0l) {
        bsupp = pool_acquire_untracked_pinned(g_pf_upool, g_dev, uint64(nsupp * 4l))
        unsafe {
            var sp = reinterpret<uint?>(metal_buffer_contents(bsupp))
            for (i in range64(nsupp)) {
                sp[i] = uint(t.suppress[i])
            }
        }
        u_nsupp = uniform_u32(uint(nsupp))
    }
    //! MTP draft-slab warm: ONE extra loop iteration runs the draft block at FULL npos so every
    //! trunk uniform reuses verbatim; the last warm row is garbage by design and causally unread
    let warm = (get_mtp_spec() && c.n_layer_nextn == 1l && gpu_logits && t.mtp_ehproj_off >= 0l &&
        (dim % 64l) == 0l)
    let n_enc_layers = c.n_layers + (warm ? 1l : 0l)
    var bwm_res : MetalBuffer?    //!< warm: trunk residual save (x_b readback + the cls tail read it)
    var bwm_emb : MetalBuffer?    //!< warm: shifted embed rows
    var bwm_cat : MetalBuffer?    //!< warm: [mp x 2dim] eh_proj input (a hyper-connection model: [mp_hc x 2dim], one row a stream)
    var bwm_tmp : MetalBuffer?    //!< warm: rms_final intermediate (a hyper-connection model: the enorm rows repeated per stream)
    var bwm_hc : MetalBuffer?     //!< warm, hyper-connections: the trunk's wide residual rows saved past the draft head's rows
    var u_dim2 : MetalBuffer?
    var u_wm_tot : MetalBuffer?
    var u_wm_hc_tot : MetalBuffer?
    let wm_rows = hcs.on ? npos * c.hc_count : npos   //! the eh_proj rows: one a position, or one a stream of a position
    let mp_hc = ((wm_rows + 31l) / 32l) * 32l
    if (warm) {
        bwm_res = pool_acquire_pinned(g_pf_pool, g_dev, uint64(npos * dim * 4l))
        bwm_emb = pool_acquire_pinned(g_pf_pool, g_dev, uint64(npos * dim * 4l))
        bwm_cat = pool_acquire_pinned(g_pf_pool, g_dev, uint64(mp_hc * 2l * dim * 4l))
        bwm_tmp = pool_acquire_pinned(g_pf_pool, g_dev, uint64(wm_rows * dim * 4l))
        u_dim2 = uniform_u32(uint(2l * dim))
        u_wm_tot = uniform_u32(uint(npos * dim))
        if (hcs.on) {
            bwm_hc = pool_acquire_pinned(g_pf_pool, g_dev, uint64(npos * hcd * 4l))
            u_wm_hc_tot = uniform_u32(uint(npos * hcd))
        }
        if (wide) {   //! this edge reads x_b host-side - split the borrow first
            ds_split_quantum(t, s, npos)
        }
        //! shifted embeds: row p pairs the NEXT position's embed with hidden p; the garbage last row reuses row 0
        unsafe {
            var dst = reinterpret<uint8?>(metal_buffer_contents(bwm_emb))
            memcpy(reinterpret<void?>(dst), addr < void? >(s.x_b[dim]), (npos - 1l) * dim * 4l)
            memcpy(reinterpret<void?>(dst + (npos - 1l) * dim * 4l), addr < void? >(s.x_b[0]), dim * 4l)
        }
    }
    //! the tiled QK/AV pair needs hs % 64 on BOTH classes; DASLLAMA_METAL_ATTN=0 pins the trio
    let mmattn = (head_size % 64l) == 0l && (hs_sw % 64l) == 0l && g_pf_env_attn
    //! the device pair's K and V twins are one scratch pair every layer rewrites
    let attn_dev = mirk || (mmattn && mp >= CVT_MIN_ROWS && pf_attn_dev_ready())
    let kv_twins = attn_dev && !mirk
    let bytes_kvh = uint64(kvrows * max(kv_dim, kvd_sw) * 2l)
    var bkh = kv_twins ? pool_acquire_pinned(g_pf_pool, g_dev, bytes_kvh) : null
    var bvh = kv_twins ? pool_acquire_pinned(g_pf_pool, g_dev, bytes_kvh) : null
    let ts_enc = ref_time_ticks()
    let setup_us = get_time_usec(ts_all)
    var err : string
    var gpu_ms : double
    var encode_ms : double
    let ncb = clamp(int64(g_pf_env_ncb > 0 ? g_pf_env_ncb : int((n_enc_layers + 3l) / 4l)), 1l, n_enc_layers)
    let unret = g_pf_env_unretained
    let layers_per_cb = (n_enc_layers + ncb - 1l) / ncb
    let cap = g_pf_env_capture
    var cbs & = unsafe(g_pf_cbs)
    residency_flush()   //! commit THIS eval's own notes before its first submission
    cbs |> clear()
    cbs |> reserve(ncb)
    var cb : MetalCommandBuffer?
    var enc : MetalComputeEncoder?
    var hz_d0 = 0l   //! per-cb coverage-check snapshots (cap only) — decode's hz_d0/hz_g0 twins
    var hz_g0 = 0l
    var dnli = 0l   //! recurrent-layer index — addresses the dn state/conv mirror slices in order
    {
        for (l in range64(n_enc_layers)) {
            if (l % layers_per_cb == 0l) {
                if (enc != null) {
                    if (cap) {
                        graph_flush_sched(enc)
                        hz_coverage_check(metal_dispatch_calls() - hz_d0, g_hz_gated - hz_g0, "prefill")
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
                    hz_d0 = metal_dispatch_calls()
                    hz_g0 = g_hz_gated
                }
            }
            if (l == 0l && wide) {   //! slice the wide quantum on-device: x, then the slice-major ds planes
                enc_head_restride(enc, bwide, 0ul, bx, 0ul, u_wide_rw, u_wide_dim, u_total_dim, npos * dim)
                for (sl in range64(nds)) {
                    enc_head_restride(enc, bwide, uint64((sl + 1l) * dim * 4l), bds, uint64(sl * npos * dim * 4l), u_wide_rw, u_wide_dim, u_total_dim, npos * dim)
                }
            }
            if (l == 0l && hcs.on && g_pf_skip != "ew" && g_pf_skip != "nongemm") {   //! the wide residual: hc copies of every embed row
                enc_hc_init(enc, bx, hcs.bhc, u_dim, hcs.u_hc, hcs.u_tot, npos * hcd)
            }
            if (l == 0l && ple_gpu) {
                //! the serial encoder orders the three PLE stages: gather -> model_proj GEMM -> rms+combine
                var pka = ple_ka(npos, ple_n, c.n_layers, dim, c.norm_eps)
                pf_enc_ple_gather_fmt(enc, t, bple_tok, bple, pka, c.n_layers, npos, ple_n)
                let bwp = plane_of(g_dev, t, t.bf16blob, t.ple_model_off)
                pf_enc_bf16_mm(enc, bwp.buf, bwp.boff, bx, bple_proj, u_dim, u_ple_all, mp, ple_all, pf_cvt_panel(enc, bx, mp, dim, g_pf_skip == "act_cvt"), dim)
                var bw_pn = upload_region(unsafe(addr < void? >(t.fblob[t.ple_pnorm_off])), uint64(ple_n * 4l))
                pf_enc_ple_finish(enc, bple_proj, bw_pn, bple, pka, c.n_layers, npos, ple_n)
                s.ple_gpu_pending = false
            }
            if (warm && l == c.n_layers) {
                //! trunk done - save the residual panel (x_b readback + cls read it), then swap bx to the
                //! draft block's eh_proj input; the body below runs the draft layer verbatim at index l
                unsafe {
                    enc_copy_row(enc, bx, 0ul, bwm_res, 0ul, u_wm_tot, npos * dim)
                    var bw_en = upload_region(addr < void? >(t.fblob[t.mtp_enorm_off]), uint64(dim * 4l))
                    pf_enc_rms(enc, bwm_emb, 0ul, bw_en, bxb, u_dim, u_eps, npos)
                    if (hcs.on) {
                        //! the carry is the wide residual row: its streams norm under hnorm, the enorm row repeats per stream,
                        //! and one eh_proj GEMM over every (position, stream) row lands the draft's wide rows in place of the trunk's
                        enc_copy_row(enc, hcs.bhc, 0ul, bwm_hc, 0ul, u_wm_hc_tot, npos * hcd)
                        enc_hc_init(enc, bxb, bwm_tmp, u_dim, hcs.u_hc, u_wm_hc_tot, npos * hcd)
                        var bw_hn = upload_region(addr < void? >(t.fblob[t.mtp_hnorm_off]), uint64(hcd * 4l))
                        enc_hc_norm(enc, hcs.bhc, bw_hn, hcs.bxn, u_dim, u_eps, hcs.u_hc, wm_rows)
                        pf_enc_cat2(enc, bwm_cat, bwm_tmp, hcs.bxn, u_dim, u_wm_hc_tot, npos * hcd)
                    } else {
                        var bw_fin = upload_region(addr < void? >(t.fblob[t.rms_final_off]), uint64(dim * 4l))
                        pf_enc_rms(enc, bx, 0ul, bw_fin, bwm_tmp, u_dim, u_eps, npos)
                        var bw_hn = upload_region(addr < void? >(t.fblob[t.mtp_hnorm_off]), uint64(dim * 4l))
                        pf_enc_rms(enc, bwm_tmp, 0ul, bw_hn, bxb2, u_dim, u_eps, npos)
                        pf_enc_cat2(enc, bwm_cat, bxb, bxb2, u_dim, u_wm_tot, npos * dim)
                    }
                    var bwm_out = hcs.on ? hcs.bhc : bx
                    let bxh_cat = pf_cvt_panel(enc, bwm_cat, mp_hc, 2l * dim, g_pf_skip == "act_cvt")
                    if (t.mtp_ehproj_fmt == KqFmt.q8) {
                        let behp = blob_of(g_dev, t, t.mtp_ehproj_off)
                        enc_gemm_mm(enc, behp.buf, behp.boff, bwm_cat, bwm_out, u_dim2, u_dim, mp_hc, dim, 0ul, 2l * dim, wm_rows, bxh_cat)
                    } else {
                        pf_enc_kq_site_mm(enc, t, t.mtp_ehproj_fmt, t.mtp_ehproj_off, dim, bwm_cat, bwm_out, u_dim2, u_dim, mp_hc, 0ul, bxh_cat, 2l * dim)
                    }
                }
            }
            unsafe {
                //! a hyper-connection model carries no per-layer norm rows: its mixers stand at both seams
                var bw_att = hcs.on ? null : upload_region(addr < void? >(t.fblob[t.rms_att_off + l * dim]), uint64(dim * 4l))
                var bw_ffn = hcs.on ? null : upload_region(addr < void? >(t.fblob[t.rms_ffn_off + l * dim]), uint64(dim * 4l))
                let lsl = layer_is_sliding(c, l)
                let het = hetero && lsl
                let hs_l = het ? hs_sw : head_size
                let kvd_l = het ? kvd_sw : kv_dim
                let qd_l = het ? qd_sw : qd
                let koff = mirk ? 0ul : uint64(start_pos * kvd_l * 4l)
                let scale_l = het ? (c.attn_scale > 0.0 ? c.attn_scale : 1.0 / sqrt(float(hs_sw))) : scale
                //! layer l's K and V slabs in the mirror
                let mir_kb = mirk ? mir_koff + uint64(mirror_lbase(t, KVDtype.f16, mir_cap, l)) : 0ul
                let mir_vb = mirk ? mir_voff + uint64(mirror_lbase(t, KVDtype.f16, mir_cap, l)) : 0ul
                var u_qd_l = het ? u_qd_sw : u_qd
                var u_kvd_l = het ? u_kvd_sw : u_kvd
                var u_hs_l = het ? u_hs_sw : u_hs
                var u_npairs_q_l = het ? u_npairs_q_sw : u_npairs_q
                var u_npairs_k_l = het ? u_npairs_k_sw : u_npairs_k
                let has_wv = t.wv_offs[l] >= 0l
                let recr = layer_is_recurrent(c, l)
                let kvsh = kv_shared_layer(t, l)
                var bxh_x : MetalBuffer?
                if (hcs.on) {   //! the side input at its layer, then the attention site's mixer opens the layer into bxb
                    if (pls.on && l == c.ple_layer) {
                        pf_ple_side(enc, t, hcs, pls, u_dim, start_pos, bdn_state, ple_ring_base)
                    }
                    pf_hc_mix_site(enc, t, hcs, l, false, bxb, hcs.binj_a, u_dim, u_eps)
                } elif (g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                    bxh_x = g_pso_rms_hx != null ? pf_twin_panel(mp, dim) : null
                    if (bxh_x != null) {
                        pf_enc_rms_hx(enc, bx, 0ul, bw_att, bxb, u_dim, u_eps, bxh_x, npos)
                    } else {
                        pf_enc_rms(enc, bx, 0ul, bw_att, bxb, u_dim, u_eps, npos)
                    }
                }
                if (recr) {
                    if (g_pf_skip != "gemm") {
                        bxh_x = pf_hx_panel(enc, bxh_x, bxb, mp, dim, g_pf_skip == "act_cvt")
                        let bwqkv = blob_of(g_dev, t, t.dnqkv_offs[l])
                        enc_gemm_mm(enc, bwqkv.buf, bwqkv.boff, bxb, bdnqkv, u_dim, u_dn_cd, mp, cd_dn, 0ul, dim, npos, bxh_x)
                        let bwz = blob_of(g_dev, t, t.dngate_offs[l])
                        enc_gemm_mm(enc, bwz.buf, bwz.boff, bxb, bdnz, u_dim, u_dn_di, mp, di_dn, 0ul, dim, npos, bxh_x)
                        if (t.dn_ba_f32) {
                            //! F32-on-disk β/α (35B): fblob slabs ride the router-b f32 GEMV
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
                    if (g_pf_skip != "attn" && g_pf_skip != "dn" && g_pf_skip != "nongemm") {
                        let soff = dn_state_base + uint64(dnli * nvh_dn * c.ssm_d_state * c.ssm_d_state * 4l)
                        let hoff = dn_conv_base + uint64(dnli * (c.ssm_d_conv - 1l) * cd_dn * 4l)
                        var bw_cv = upload_region(addr < void? >(t.fblob[t.dn_conv_off + l * c.ssm_d_conv * cd_dn]), uint64(c.ssm_d_conv * cd_dn * 4l))
                        var dka = dn_args(c, npos, false)
                        enc_dn_conv(enc, bdnqkv, bdn_state, hoff, bw_cv, bdncv, dka, cd_dn, npos)
                        enc_dn_conv_hist(enc, bdnqkv, bdn_state, hoff, bdn_state, hoff, dka, cd_dn)
                        enc_dn_l2norm(enc, bdncv, dka, 2l * c.ssm_n_group, npos)
                        var baa = upload_region(addr < void? >(t.fblob[t.dn_a_off + l * nvh_dn]), uint64(nvh_dn * 4l))
                        var bdt = upload_region(addr < void? >(t.fblob[t.dn_dt_off + l * nvh_dn]), uint64(nvh_dn * 4l))
                        if (bdncvh != null) {
                            //! the walk reads halves: the conv rows convert once, the gates prepare once
                            enc_cvt_half(enc, bdncv, bdncvh, npos * cd_dn, (npos * cd_dn + 255l) / 256l * 256l)
                            enc_dn_prep(enc, bdnb, bdng, baa, bdt, dka, (npos * nvh_dn + 63l) / 64l * 64l)
                            enc_dn_scan_h(enc, bdncvh, bdn_state, soff, bdnb, bdng, baa, bdt, bdno,
                                bdn_state, soff, dka, c.ssm_d_state, nvh_dn)
                        } else {
                            enc_dn_scan(enc, bdncv, bdn_state, soff, bdnb, bdng, baa, bdt, bdno,
                                bdn_state, soff, dka, c.ssm_d_state, nvh_dn)
                        }
                        var bwn = upload_region(addr < void? >(t.fblob[t.dn_norm_off + l * c.ssm_d_state]), uint64(c.ssm_d_state * 4l))
                        if (c.dn_z_sigmoid) {
                            enc_dn_gate_sig(enc, bdno, bdnz, bwn, dka, nvh_dn, npos)
                        } else {
                            enc_dn_gate(enc, bdno, bdnz, bwn, dka, nvh_dn, npos)
                        }
                    }
                    if (g_pf_skip != "gemm") {
                        let bwo = blob_of(g_dev, t, t.dnout_offs[l])
                        enc_gemm_mm(enc, bwo.buf, bwo.boff, bdno, bxb2, u_dn_di, u_dim, mp, dim, 0ul, di_dn, npos, pf_cvt_panel(enc, bdno, mp, di_dn, g_pf_skip == "act_cvt"))
                    }
                    dnli++
                }
                if (!recr && g_pf_skip != "gemm") {
                    //! K-quant tensors take their mul_mm twins per tensor; q8-tagged tensors keep the blob path
                    bxh_x = pf_hx_panel(enc, bxh_x, bxb, mp, dim, g_pf_skip == "act_cvt")
                    let fq = fmt_at(t.wq_fmt, l)
                    let fk = fmt_at(t.wk_fmt, l)
                    let fv = has_wv ? fmt_at(t.wv_fmt, l) : KqFmt.q8
                    if (fq != KqFmt.q8 || fk != KqFmt.q8 || fv != KqFmt.q8) {
                        if (c.q_gated) {
                            //! 2x-wide packed [q|gate] projection, deinterleaved into the q panel + gate rows
                            if (fq != KqFmt.q8) {
                                pf_enc_kq_site_mm(enc, t, fq, t.wq_offs[l], 2l * qd_l, bxb, bdnqg, u_dim, u_dn_qd2, mp, 0ul, bxh_x, dim)
                            } else {
                                let bwqb = blob_of(g_dev, t, t.wq_offs[l])
                                enc_gemm_mm(enc, bwqb.buf, bwqb.boff, bxb, bdnqg, u_dim, u_dn_qd2, mp, 2l * qd_l, 0ul, dim, npos, bxh_x)
                            }
                            enc_dn_deint(enc, bdnqg, bq, bdngate,
                                DnDeintArgs(hs = uint(hs_l), qd = uint(qd_l), qs = uint(qd_l)), qd_l, npos)
                        } elif (fq != KqFmt.q8) {
                            pf_enc_kq_site_mm(enc, t, fq, t.wq_offs[l], qd_l, bxb, bq, u_dim, u_qd_l, mp, 0ul, bxh_x, dim)
                        } else {
                            let bwqb = blob_of(g_dev, t, t.wq_offs[l])
                            enc_gemm_mm(enc, bwqb.buf, bwqb.boff, bxb, bq, u_dim, u_qd_l, mp, qd_l, 0ul, dim, npos, bxh_x)
                        }
                        if (!kvsh) {
                            if (fk != KqFmt.q8) {
                                pf_enc_kq_site_mm(enc, t, fk, t.wk_offs[l], kvd_l, bxb, bks[l], u_dim, u_kvd_l, mp, koff, bxh_x, dim)
                            } else {
                                let bwkb = blob_of(g_dev, t, t.wk_offs[l])
                                enc_gemm_mm(enc, bwkb.buf, bwkb.boff, bxb, bks[l], u_dim, u_kvd_l, mp, kvd_l, koff, dim, npos, bxh_x)
                            }
                            if (has_wv) {
                                if (fv != KqFmt.q8) {
                                    pf_enc_kq_site_mm(enc, t, fv, t.wv_offs[l], kvd_l, bxb, bvs[l], u_dim, u_kvd_l, mp, koff, bxh_x, dim)
                                } else {
                                    let bwvb = blob_of(g_dev, t, t.wv_offs[l])
                                    enc_gemm_mm(enc, bwvb.buf, bwvb.boff, bxb, bvs[l], u_dim, u_kvd_l, mp, kvd_l, koff, dim, npos, bxh_x)
                                }
                            }
                        }
                    } else {
                        let bwqb = blob_of(g_dev, t, t.wq_offs[l])
                        if (c.q_gated) {
                            //! 2x-wide packed [q|gate] projection, deinterleaved into the q panel + gate rows
                            enc_gemm_mm(enc, bwqb.buf, bwqb.boff, bxb, bdnqg, u_dim, u_dn_qd2, mp, 2l * qd_l, 0ul, dim, npos, bxh_x)
                            enc_dn_deint(enc, bdnqg, bq, bdngate,
                                DnDeintArgs(hs = uint(hs_l), qd = uint(qd_l), qs = uint(qd_l)), qd_l, npos)
                        } else {
                            enc_gemm_mm(enc, bwqb.buf, bwqb.boff, bxb, bq, u_dim, u_qd_l, mp, qd_l, 0ul, dim, npos, bxh_x)
                        }
                        if (!kvsh) {   //! shared layers have no K/V tensors (offs -1)
                            let bwkb = blob_of(g_dev, t, t.wk_offs[l])
                            enc_gemm_mm(enc, bwkb.buf, bwkb.boff, bxb, bks[l], u_dim, u_kvd_l, mp, kvd_l, koff, dim, npos, bxh_x)
                            if (has_wv) {
                                let bwvb = blob_of(g_dev, t, t.wv_offs[l])
                                enc_gemm_mm(enc, bwvb.buf, bwvb.boff, bxb, bvs[l], u_dim, u_kvd_l, mp, kvd_l, koff, dim, npos, bxh_x)
                            }
                        }
                    }
                }
                if (!recr && !kvsh && (!has_wv || c.v_norm) && g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                    //! no-wv layers copy the PRE-norm/PRE-rope K rows into the V panel, then v_norm runs the
                    //! weightless per-head RMS in place - the CPU mm_qkv order, BEFORE qk_norm touches k
                    if (!has_wv) {
                        enc_copy_row(enc, bks[l], koff, bvs[l], koff, het ? u_vtot_sw : u_vtot, npos * kvd_l)
                    }
                    if (c.v_norm) {
                        //! MAX-width ones upload: the region cache is address-keyed and both classes read this one plane
                        var bones = upload_region(addr < void? >(t.fblob[t.rms_ones_off]), uint64(max_head_size(c) * 4l))
                        var vka = QkNormArgs(nq = 0u, koffe = 0u, rstride = uint(kvd_l),
                            hs = uint(hs_l), woffe = 0u, eps = c.norm_eps, soffe = 0u)
                        enc_qk_norm(enc, bvs[l], koff, bones, vka, kvd_l / hs_l, npos, hs_l)
                    }
                }
                if (!recr && has_bias && g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                    //! QKV bias before rope, chunk rows only (existing rows are stored biased+roped); the count
                    //! uniforms stay base-class - no hetero model carries QKV bias
                    var bbq = upload_region(addr < void? >(t.bq[l * qd_l]), uint64(qd_l * 4l))
                    var bbk = upload_region(addr < void? >(t.bk[l * kvd_l]), uint64(kvd_l * 4l))
                    var bbv = upload_region(addr < void? >(t.bv[l * kvd_l]), uint64(kvd_l * 4l))
                    enc_add_bias_rows(enc, bq, 0ul, bbq, 0ul, u_qd, u_totq, npos * qd)
                    enc_add_bias_rows(enc, bks[l], koff, bbk, 0ul, u_kvd, u_totkv, npos * kv_dim)
                    enc_add_bias_rows(enc, bvs[l], koff, bbv, 0ul, u_kvd, u_totkv, npos * kv_dim)
                }
                //! rot mirrors enc_rope's @default: the null u_rot bind means "the class's own hs"
                let rot_l = c.rope_dim > 0l && c.rope_dim != head_size ? c.rope_dim : hs_l
                let qkr_on = (g_pf_env_qkr && g_pf_pso_qkr != null && g_pf_pso_qkr_hx != null
                    && hs_l % 2l == 0l && hs_l / 2l <= 512l && rot_l % 2l == 0l && rot_l >= 2l && rot_l <= hs_l)
                var bqh_q : MetalBuffer? = null    //! the fused arm's q twin, claimed at rope time - the QK site consumes it
                var bqh_q_live = false
                if (!recr && qkr_on && g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                    var bcos_l = lsl && dual_rope ? bcos_swa : bcos
                    var bsin_l = lsl && dual_rope ? bsin_swa : bsin
                    //! donorm == 0 never reads w - bcos_l just keeps the binding valid
                    var bqw = has_qkn ? upload_region(addr < void? >(t.fblob[t.rms_q_offs[l]]), uint64(hs_l * 4l)) : bcos_l
                    var qka = QkRopeArgs(rstride = uint(qd_l), hs = uint(hs_l), eps = c.norm_eps,
                        donorm = has_qkn ? 1u : 0u, neox = c.rope_neox ? 1u : 0u, rot = uint(rot_l), nreal = uint(npos),
                        hscale = mirk ? scale_l : 1.0)
                    bqh_q = (mirk || mp >= CVT_MIN_ROWS) ? pf_twin_panel(mp, qd_l) : null
                    bqh_q_live = true
                    if (bqh_q != null && g_pf_skip != "act_cvt") {
                        enc_qk_rope_hx(enc, bq, 0ul, bqw, bcos_l, bsin_l, qka, bqh_q, n_heads, mp, hs_l / 2l)
                    } else {
                        enc_qk_rope(enc, bq, 0ul, bqw, bcos_l, bsin_l, qka, n_heads, npos, hs_l / 2l)
                    }
                    if (!kvsh) {
                        var bkw = has_qkn ? upload_region(addr < void? >(t.fblob[t.rms_k_offs[l]]), uint64(hs_l * 4l)) : bcos_l
                        var kka = QkRopeArgs(rstride = uint(kvd_l), hs = uint(hs_l), eps = c.norm_eps,
                            donorm = has_qkn ? 1u : 0u, neox = c.rope_neox ? 1u : 0u, rot = uint(rot_l), nreal = uint(npos), hscale = 1.0)
                        enc_qk_rope(enc, bks[l], koff, bkw, bcos_l, bsin_l, kka, kvd_l / hs_l, npos, hs_l / 2l)
                    }
                }
                if (!recr && !qkr_on && has_qkn && g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                    //! per-head QK-norm before rope — the chunk's q panel and its new K rows
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
                if (!recr && !qkr_on && g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                    //! sliding layers rope with the swa row pair, whose rows are the class's own hs/2 wide
                    var bcos_l = lsl && dual_rope ? bcos_swa : bcos
                    var bsin_l = lsl && dual_rope ? bsin_swa : bsin
                    enc_rope(enc, bq, 0ul, bcos_l, bsin_l, u_qd_l, u_hs_l, u_npairs_q_l, u_neox, u_rot, null, npos * qd_l / 2l)
                    if (!kvsh) {
                        enc_rope(enc, bks[l], koff, bcos_l, bsin_l, u_kvd_l, u_hs_l, u_npairs_k_l, u_neox, u_rot, null, npos * kvd_l / 2l)
                    }
                }
                if (mirk && !recr && !kvsh) {
                    var hka = HalfRowsArgs(total = uint(npos * kvd_l), scale = 1.0)
                    let rows_off = uint64(start_pos * kvd_l * 2l)
                    enc_half_rows(enc, bks[l], g_arena, mir_kb + rows_off, hka, npos * kvd_l)
                    enc_half_rows(enc, bvs[l], g_arena, mir_vb + rows_off, hka, npos * kvd_l)
                }
                if (!recr && g_pf_skip != "attn" && g_pf_skip != "nongemm") {
                    var atka = AttnArgs(qd = uint(qd_l), kv_dim = uint(kvd_l), head_size = uint(hs_l),
                        kv_mul = het ? uint(n_heads / kvh_sw) : kv_mul, npos = uint(nk),
                        np32 = uint(nk64),
                        scale = scale_l,
                        qoff = uint(start_pos), qrows = uint(mp),
                        window = uint(lsl && c.sliding_window > 0l ? c.sliding_window : 0l),
                        softcap = c.attn_logit_softcap, hass = c.attn_sinks ? 1u : 0u,
                        uend = s.attn_uniform_end > 0l ? uint(s.attn_uniform_end) : 0u,
                        ulo = s.attn_uniform_end > 0l ? uint(s.attn_uniform_lo) : 0u)
                    var bqh_l : MetalBuffer?
                    if (bqh_q_live) {
                        bqh_l = bqh_q
                    } elif (mirk) {   //! the unfused rope left no twin: Q's halves carry the softmax scale
                        bqh_l = pf_twin_panel(mp, qd_l)
                        var qha = HalfRowsArgs(total = uint(mp * qd_l), scale = scale_l)
                        enc_half_rows(enc, bq, bqh_l, 0ul, qha, mp * qd_l)
                    } elif (mmattn) {
                        bqh_l = pf_cvt_panel(enc, bq, mp, qd_l, g_pf_skip == "act_cvt")
                    }
                    let dev = attn_dev && bqh_l != null
                    if (dev && !mirk) {
                        enc_kv_twin(enc, bks[l], bvs[l], bkh, bvh, atka.scale, nk64 * kvd_l)
                    }
                    if (g_pf_skip != "attn_qk") {
                        if (dev) {
                            enc_qk_dev(enc, bqh_l, mirk ? g_arena : bkh, batt, atka, mp, nk64, n_heads, mir_kb)
                        } elif (mmattn) {
                            enc_qk_mm(enc, bq, bks[l], batt, atka, mp, nk64, n_heads, bqh = bqh_l)
                        } else {
                            enc_qk(enc, bq, bks[l], batt, atka, mp, nk64, n_heads)
                        }
                    }
                    if (g_pf_skip != "attn_sm") {
                        var bsnk : MetalBuffer?
                        if (c.attn_sinks) {
                            bsnk = upload_region(unsafe(addr < void? >(t.fblob[t.sinks_off + l * n_heads])), uint64(n_heads * 4l))
                        }
                        if (dev) {
                            enc_attn_pexp(enc, batt, atka, bstats, bsnk, npos, n_heads)
                        } else {
                            enc_rowstat(enc, batt, atka, bstats, bsnk, npos, n_heads)
                        }
                    }
                    if (g_pf_skip != "attn_av") {
                        if (dev) {
                            enc_av_dev(enc, batt, mirk ? g_arena : bvh, bxb, atka, bstats, mp, n_heads, hs_l, mir_vb)
                        } elif (mmattn) {
                            enc_av_mm(enc, batt, bvs[l], bxb, atka, bstats, mp, n_heads, hs_l)
                        } else {
                            enc_av(enc, batt, bvs[l], bxb, atka, bstats, mp, n_heads, hs_l)
                        }
                    }
                }
                if (!recr && c.q_gated && g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                    //! gated-attention epilogue: out ⊙ σ(gate) between the attention output and wo (the CPU order)
                    enc_sigmul(enc, bxb, 0ul, bdngate, 0ul, u_qtot, npos * qd)
                }
                if (!recr && g_pf_skip != "gemm") {
                    let fo = fmt_at(t.wo_fmt, l)
                    let bxh_av = pf_cvt_panel(enc, bxb, mp, qd_l, g_pf_skip == "act_cvt")
                    if (fo != KqFmt.q8) {
                        pf_enc_kq_site_mm(enc, t, fo, t.wo_offs[l], dim, bxb, bxb2, u_qd_l, u_dim, mp, 0ul, bxh_av, qd_l)
                    } else {
                        let bwob = blob_of(g_dev, t, t.wo_offs[l])
                        enc_gemm_mm(enc, bwob.buf, bwob.boff, bxb, bxb2, u_qd_l, u_dim, mp, dim, 0ul, qd_l, npos, bxh_av)
                    }
                    if (c.attn_out_bias) {
                        var bwo_b = upload_region(unsafe(addr < void? >(t.bo[l * dim])), uint64(dim * 4l))
                        enc_add_bias_rows(enc, bxb2, 0ul, bwo_b, 0ul, u_dim, u_total_dim, npos * dim)
                    }
                }
                let hid_l = layer_hidden(t, l)
                let u_hid_l = (u_hidden2 == null || hid_l == hidden) ? u_hidden : u_hidden2
                let u_th_l = (u_total_h2 == null || hid_l == hidden) ? u_total_h : u_total_h2
                let narrow_last = (g_pf_env_lastrow && !s.keep_hidden && !warm
                    && l == c.n_layers - 1l && npos > 1l && !recr && !moe && !plef && !hcs.on
                    && !c.pre_post_norm && l >= nds && !c.q_gated
                    && c.n_layer_nextn == 0l
                    && dim % 32l == 0l && hid_l % 32l == 0l
                    && fmt_at(t.w1_fmt, l) == KqFmt.q8 && fmt_at(t.w3_fmt, l) == KqFmt.q8
                    && fmt_at(t.w2_fmt, l) == KqFmt.q8)
                if (narrow_last) {
                    let nro = uint64((npos - 1l) * dim * 4l)
                    if (g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                        enc_add(enc, bx, nro, bxb2, nro, u_dim, dim)
                        pf_enc_rms(enc, bx, nro, bw_ffn, bxb, u_dim, u_eps, 1l)
                    }
                    if (g_pf_skip != "gemm") {
                        let bw1b = blob_of(g_dev, t, t.w1_offs[l])
                        enc_gemv(enc, bw1b.buf, bw1b.boff, bxb, bhb, u_dim, u_hid_l, hid_l, 0ul, 0ul)
                        let bw3b = blob_of(g_dev, t, t.w3_offs[l])
                        enc_gemv(enc, bw3b.buf, bw3b.boff, bxb, bhb2, u_dim, u_hid_l, hid_l, 0ul, 0ul)
                    }
                    if (g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                        if (c.ffn_act != FfnAct.silu) {
                            enc_geglu(enc, bhb, 0ul, bhb2, 0ul, u_hid_l, hid_l)
                        } else {
                            enc_swiglu(enc, bhb, 0ul, bhb2, 0ul, u_hid_l, hid_l)
                        }
                    }
                    if (g_pf_skip != "gemm") {
                        let bw2b = blob_of(g_dev, t, t.w2_offs[l])
                        enc_gemv(enc, bw2b.buf, bw2b.boff, bhb, bxb, u_hid_l, u_dim, dim, 0ul, 0ul)
                    }
                    if (g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                        var bosc : MetalBuffer?
                        if (c.layer_out_scale) {
                            bosc = upload_region(addr < void? >(t.fblob[t.out_scale_off + l]), 4ul)
                        }
                        enc_add(enc, bx, nro, bxb, 0ul, u_dim, dim, bosc)
                    }
                }
                var bxh_ffn : MetalBuffer?
                if (hcs.on) {   //! the attention output scatters into the streams; the ffn site's mixer makes the block input rows
                    pf_hc_combine(enc, t, hcs, bxb2, hcs.binj_a, u_dim)
                    pf_hc_mix_site(enc, t, hcs, l, true, bxb, hcs.binj_f, u_dim, u_eps)
                } elif (!narrow_last && g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                    if (c.pre_post_norm) {
                        //! gemma sandwich: norm the attention branch IN PLACE before the residual add
                        var bw_pa = upload_region(addr < void? >(t.fblob[t.rms_post_att_off + l * dim]), uint64(dim * 4l))
                        pf_enc_rms(enc, bxb2, 0ul, bw_pa, bxb2, u_dim, u_eps, npos)
                    }
                    let want_hx = true   //! a dense FFN reads the rows - the layer's own or a routed layer's shared expert - and so does the router's GEMM
                    if (dim <= ADD_RMS_MAX_DIM && g_pso_addrms != null) {
                        bxh_ffn = want_hx && g_pso_addrms_hx != null ? pf_twin_panel(mp, dim) : null
                        if (bxh_ffn != null) {
                            pf_enc_add_rms_bhx(enc, bx, bxb2, bw_ffn, bxb, u_dim, u_eps, bxh_ffn, npos)
                        } else {
                            enc_add_rms_b(enc, bx, bxb2, bw_ffn, bxb, u_dim, u_eps, npos)
                        }
                    } else {
                        enc_add(enc, bx, 0ul, bxb2, 0ul, u_total_dim, npos * dim)
                        bxh_ffn = want_hx && g_pso_rms_hx != null ? pf_twin_panel(mp, dim) : null
                        if (bxh_ffn != null) {
                            pf_enc_rms_hx(enc, bx, 0ul, bw_ffn, bxb, u_dim, u_eps, bxh_ffn, npos)
                        } else {
                            pf_enc_rms(enc, bx, 0ul, bw_ffn, bxb, u_dim, u_eps, npos)
                        }
                    }
                }
                if (moe && g_pf_skip != "gemm" && g_pf_skip != "moe") {
                    //! g4moe routes its own router norm and pre_ffn2 input through bxb2 and reduces into bmg4rt
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
                        u_dim, u_moe_ne, u_moe_nk, u_moe_gmode, brb, bhrb, u_moe_tot, u_moe_np, npos,
                        bxh = g4moe ? null : bxh_ffn, mp = mp)
                    if (g4moe) {
                        var bw_ds = upload_region(unsafe(addr < void? >(t.fblob[t.moe_down_scale_off + l * c.n_expert])), uint64(c.n_expert * 4l))
                        enc_moe_wscale(enc, bmsel, bmsel, bw_ds, u_moe_nk, npos)
                    }
                    if (nsh > 0l) {
                        //! shexp raw gate dots (σ in the combine) — bmlg is free once select consumed it
                        var bw_shg = upload_region(unsafe(addr < void? >(t.fblob[t.shexp_gate_off + l * dim])), uint64(dim * 4l))
                        pf_enc_slab_gemv_b(enc, bw_shg, bxb, bmlg, u_dim, u_moe_one, u_moe_np, 1l, npos)
                    }
                    if (!pf_skip_moe_mm()) {
                        if (t.experts_mx4) {
                            //! per-expert bias folds into the mm accumulator seed (hasb); the moe_bias arm knocks it out
                            let hasb = g_pf_skip != "moe_bias"
                            var bwb1 = upload_region(unsafe(addr < void? >(t.fblob[t.web1_off + l * c.n_expert * c.n_ff_exp])), uint64(c.n_expert * c.n_ff_exp * 4l))
                            var bwb3 = upload_region(unsafe(addr < void? >(t.fblob[t.web3_off + l * c.n_expert * c.n_ff_exp])), uint64(c.n_expert * c.n_ff_exp * 4l))
                            var kaup = moe_mm_mx4_ka(dim, c.n_ff_exp, c.n_expert_used, true, hasb)
                            let bxg1 = pf_moe_gather_panel(enc, bmbkt, g4moe ? bxb2 : bxb, bmxg,
                                g_pf_pso_moe_mm_mx4_th != null, dim, c.n_expert_used, c.n_expert, mpad, npos)
                            pf_enc_moe_mm_mx4(enc, t, t.we1_offs[l], c.n_ff_exp, npos, g4moe ? bxb2 : bxb, bmg,
                                bmcnt, bmbase, bmbkt, bwb1, kaup, contiguous = bxg1 != null, bxh = bxg1)
                            pf_enc_moe_mm_mx4(enc, t, t.we3_offs[l], c.n_ff_exp, npos, g4moe ? bxb2 : bxb, bmg2,
                                bmcnt, bmbase, bmbkt, bwb3, kaup, contiguous = bxg1 != null, bxh = bxg1)
                        } else {
                            let gx_q8 = pf_moe_th_pso(fe1) != null && pf_moe_th_pso(fe3) != null
                            let bxg1 = pf_moe_gather_panel(enc, bmbkt, g4moe ? bxb2 : bxb, bmxg,
                                gx_q8, dim, c.n_expert_used, c.n_expert, mpad, npos)
                            pf_enc_moe_mm(enc, t, fe1, t.we1_offs[l], c.n_ff_exp, npos, g4moe ? bxb2 : bxb, bmg,
                                bmcnt, bmbase, bmbkt, moe_mm_ka(fe1, dim, c.n_ff_exp, c.n_expert_used, true),
                                contiguous = bxg1 != null, bxh = bxg1, bkd = u_dim, bnd = u_moe_nfe, mpad = mpad)
                            pf_enc_moe_mm(enc, t, fe3, t.we3_offs[l], c.n_ff_exp, npos, g4moe ? bxb2 : bxb, bmg2,
                                bmcnt, bmbase, bmbkt, moe_mm_ka(fe3, dim, c.n_ff_exp, c.n_expert_used, true),
                                contiguous = bxg1 != null, bxh = bxg1, bkd = u_dim, bnd = u_moe_nfe, mpad = mpad)
                        }
                        if (nsh > 0l) {
                            //! shexp gate/up: dense q8 mul_mm at the wsh planes (bhb/bhb2 sized max(hidden, nsh))
                            let bxh_sh = pf_hx_panel(enc, bxh_ffn, bxb, mp, dim, g_pf_skip == "act_cvt")
                            let bwsh1 = blob_of(g_dev, t, t.wsh1_off + l * dim * nsh)
                            enc_gemm_mm(enc, bwsh1.buf, bwsh1.boff, bxb, bhb, u_dim, u_moe_nsh, mp, nsh, 0ul, dim, npos, bxh_sh)
                            let bwsh3 = blob_of(g_dev, t, t.wsh3_off + l * dim * nsh)
                            enc_gemm_mm(enc, bwsh3.buf, bwsh3.boff, bxb, bhb2, u_dim, u_moe_nsh, mp, nsh, 0ul, dim, npos, bxh_sh)
                        }
                    }
                }
                // the gpt-oss activation has no HX stamp, so it keeps the convert pass
                let down_th = moe && (t.experts_mx4 ? g_pf_pso_moe_mm_mx4_th != null : pf_moe_th_pso(fmt_at(t.we2_fmt, l)) != null)
                let act_hx = down_th && c.ffn_act != FfnAct.swiglu_oai && mpad >= CVT_MIN_ROWS && g_pf_skip != "moe_cvt" && g_pf_skip != "act_cvt"
                var bxh_mg = act_hx ? pf_twin_panel(mpad, c.n_ff_exp) : null
                var bxh_shh : MetalBuffer?   //!< the shared expert's hidden rows as halves, where its activation wrote them
                if (moe && g_pf_skip != "ew" && g_pf_skip != "nongemm" && g_pf_skip != "moe" && !pf_skip_moe_act()) {
                    if (c.ffn_act == FfnAct.swiglu_oai) {
                        pf_enc_swiglu_oai(enc, bmg, 0ul, bmg2, 0ul, u_moe_ptot, bmcnt, bmbase, u_moe_ne, u_moe_nfe, mpad * c.n_ff_exp)
                    } elif (bxh_mg != null) {
                        if (g4moe) {
                            enc_geglu_hxb(enc, bmg, 0ul, bmg2, 0ul, u_moe_ptot, bxh_mg, bmbase, uint64(c.n_expert * 4l), u_moe_nfe, mpad * c.n_ff_exp)
                        } else {
                            enc_swiglu_hxb(enc, bmg, 0ul, bmg2, 0ul, u_moe_ptot, bxh_mg, bmbase, uint64(c.n_expert * 4l), u_moe_nfe, mpad * c.n_ff_exp)
                        }
                    } else {
                        if (g4moe) {
                            enc_geglu_b(enc, bmg, 0ul, bmg2, 0ul, u_moe_ptot, bmbase, uint64(c.n_expert * 4l), u_moe_nfe, mpad * c.n_ff_exp)
                        } else {
                            enc_swiglu_b(enc, bmg, 0ul, bmg2, 0ul, u_moe_ptot, bmbase, uint64(c.n_expert * 4l), u_moe_nfe, mpad * c.n_ff_exp)
                        }
                    }
                    if (nsh > 0l) {
                        bxh_shh = g_pso_swiglu_hx != null ? pf_twin_panel(mp, nsh) : null
                        if (bxh_shh != null) {
                            enc_swiglu_hx(enc, bhb, 0ul, bhb2, 0ul, u_moe_shtot, bxh_shh, npos * nsh)
                        } else {
                            enc_swiglu(enc, bhb, 0ul, bhb2, 0ul, u_moe_shtot, npos * nsh)
                        }
                    }
                }
                if (moe && g_pf_skip != "gemm" && g_pf_skip != "moe") {
                    let fe2 = fmt_at(t.we2_fmt, l)
                    if (!pf_skip_moe_mm()) {
                        // the bucket panel is PADDED - a convert must cover mpad rows, not the mtot prefix
                        if (down_th && bxh_mg == null) {
                            bxh_mg = pf_cvt_panel(enc, bmg, mpad, c.n_ff_exp, g_pf_skip == "moe_cvt" || g_pf_skip == "act_cvt")
                        }
                        if (t.experts_mx4) {
                            var bwb2 = upload_region(unsafe(addr < void? >(t.fblob[t.web2_off + l * c.n_expert * dim])), uint64(c.n_expert * dim * 4l))
                            pf_enc_moe_mm_mx4(enc, t, t.we2_offs[l], dim, npos, bmg, bmdn,
                                bmcnt, bmbase, bmbkt, bwb2,
                                moe_mm_mx4_ka(c.n_ff_exp, dim, c.n_expert_used, false, g_pf_skip != "moe_bias"),
                                contiguous = true, bxh = bxh_mg)
                        } else {
                            pf_enc_moe_mm(enc, t, fe2, t.we2_offs[l], dim, npos, bmg, bmdn,
                                bmcnt, bmbase, bmbkt,
                                moe_mm_ka(fe2, c.n_ff_exp, dim, c.n_expert_used, false), contiguous = true, bxh = bxh_mg,
                                bkd = u_moe_nfe, bnd = u_dim, mpad = mpad)
                        }
                        if (nsh > 0l) {
                            //! shexp down into bxb2 (free post-residual) — bxb still feeds the reduce
                            let bwsh2 = blob_of(g_dev, t, t.wsh2_off + l * dim * nsh)
                            enc_gemm_mm(enc, bwsh2.buf, bwsh2.boff, bhb, bxb2, u_moe_nsh, u_dim, mp, dim, 0ul, nsh, npos, pf_hx_panel(enc, bxh_shh, bhb, mp, nsh, g_pf_skip == "act_cvt"))
                        }
                    }
                    if (!pf_skip_moe_reduce()) {
                        pf_enc_moe_reduce(enc, bmdn, bmsel, bminv, g4moe ? bmg4rt : bxb, u_dim, u_moe_nk, dim, npos)
                        if (nsh > 0l) {
                            //! routed sum lands in bxb first — the shexp branch adds σ(gate)-scaled on top
                            enc_axpy_sig(enc, bxb, bxb2, bmlg, u_total_dim, u_dim, npos * dim)
                        }
                    }
                    if (g4moe && g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                        var bw_pf2 = upload_region(unsafe(addr < void? >(t.fblob[t.rms_post_ffn2_off + l * dim])), uint64(dim * 4l))
                        pf_enc_rms(enc, bmg4rt, 0ul, bw_pf2, bmg4rt, u_dim, u_eps, npos)
                    }
                }
                if (!narrow_last && (!moe || g4moe) && g_pf_skip != "gemm") {   //! g4: the dense parallel shared branch
                    bxh_ffn = pf_hx_panel(enc, bxh_ffn, bxb, mp, dim, g_pf_skip == "act_cvt")
                    let f1 = fmt_at(t.w1_fmt, l)
                    let f3 = fmt_at(t.w3_fmt, l)
                    if (f1 != KqFmt.q8 || f3 != KqFmt.q8) {
                        if (f1 != KqFmt.q8) {
                            pf_enc_kq_site_mm(enc, t, f1, t.w1_offs[l], hid_l, bxb, bhb, u_dim, u_hid_l, mp, 0ul, bxh_ffn, dim)
                        } else {
                            let bw1b = blob_of(g_dev, t, t.w1_offs[l])
                            enc_gemm_mm(enc, bw1b.buf, bw1b.boff, bxb, bhb, u_dim, u_hid_l, mp, hid_l, 0ul, dim, npos, bxh_ffn)
                        }
                        if (f3 != KqFmt.q8) {
                            pf_enc_kq_site_mm(enc, t, f3, t.w3_offs[l], hid_l, bxb, bhb2, u_dim, u_hid_l, mp, 0ul, bxh_ffn, dim)
                        } else {
                            let bw3b = blob_of(g_dev, t, t.w3_offs[l])
                            enc_gemm_mm(enc, bw3b.buf, bw3b.boff, bxb, bhb2, u_dim, u_hid_l, mp, hid_l, 0ul, dim, npos, bxh_ffn)
                        }
                    } else {
                        let bw1b = blob_of(g_dev, t, t.w1_offs[l])
                        let bw3b = blob_of(g_dev, t, t.w3_offs[l])
                        enc_gemm_mm(enc, bw1b.buf, bw1b.boff, bxb, bhb, u_dim, u_hid_l, mp, hid_l, 0ul, dim, npos, bxh_ffn)
                        enc_gemm_mm(enc, bw3b.buf, bw3b.boff, bxb, bhb2, u_dim, u_hid_l, mp, hid_l, 0ul, dim, npos, bxh_ffn)
                    }
                }
                var bxh_h : MetalBuffer?
                if (!narrow_last && (!moe || g4moe) && g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                    if (c.ffn_act != FfnAct.silu) {
                        bxh_h = g_pso_geglu_hx != null ? pf_twin_panel(mp, hid_l) : null
                        if (bxh_h != null) {
                            enc_geglu_hx(enc, bhb, 0ul, bhb2, 0ul, u_th_l, bxh_h, npos * hid_l)
                        } else {
                            enc_geglu(enc, bhb, 0ul, bhb2, 0ul, u_th_l, npos * hid_l)
                        }
                    } else {
                        bxh_h = g_pso_swiglu_hx != null ? pf_twin_panel(mp, hid_l) : null
                        if (bxh_h != null) {
                            enc_swiglu_hx(enc, bhb, 0ul, bhb2, 0ul, u_th_l, bxh_h, npos * hid_l)
                        } else {
                            enc_swiglu(enc, bhb, 0ul, bhb2, 0ul, u_th_l, npos * hid_l)
                        }
                    }
                }
                if (!narrow_last && (!moe || g4moe) && g_pf_skip != "gemm") {
                    bxh_h = pf_hx_panel(enc, bxh_h, bhb, mp, hid_l, g_pf_skip == "act_cvt")
                    let f2 = fmt_at(t.w2_fmt, l)
                    if (f2 != KqFmt.q8) {
                        pf_enc_kq_site_mm(enc, t, f2, t.w2_offs[l], dim, bhb, bxb, u_hid_l, u_dim, mp, 0ul, bxh_h, hid_l)
                    } else {
                        let bw2b = blob_of(g_dev, t, t.w2_offs[l])
                        enc_gemm_mm(enc, bw2b.buf, bw2b.boff, bhb, bxb, u_hid_l, u_dim, mp, dim, 0ul, hid_l, npos, bxh_h)
                    }
                }
                if (g4moe && g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                    //! dense-branch post-norm, then routed + dense summed into bxb for the epilogue
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
                    //! PLE sits between the FFN residual and out_scale (the CPU order): x_b += ffn_branch, then
                    //! branch = post_norm(proj · (gelu(gate · x_b) ⊙ side[l]))
                    if (g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                        enc_add(enc, bx, 0ul, bxb, 0ul, u_total_dim, npos * dim)
                    }
                    if (g_pf_skip != "gemm") {
                        let bwpg = blob_of(g_dev, t, t.ple_gate_off + l * dim * ple_n)
                        enc_gemm_mm(enc, bwpg.buf, bwpg.boff, bx, bpleg, u_dim, u_ple, mp, ple_n, 0ul, dim, npos, pf_cvt_panel(enc, bx, mp, dim, g_pf_skip == "act_cvt"))
                    }
                    if (g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                        enc_geglu(enc, bpleg, 0ul, bple, uint64(l * npos * ple_n * 4l), u_ple_tot, npos * ple_n)
                    }
                    if (g_pf_skip != "gemm") {
                        let bwpp = blob_of(g_dev, t, t.ple_proj_off + l * ple_n * dim)
                        enc_gemm_mm(enc, bwpp.buf, bwpp.boff, bpleg, bxb, u_ple, u_dim, mp, dim, 0ul, ple_n, npos, pf_cvt_panel(enc, bpleg, mp, ple_n, g_pf_skip == "act_cvt"))
                    }
                    if (g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                        var bw_pn = upload_region(addr < void? >(t.fblob[t.ple_post_off + l * dim]), uint64(dim * 4l))
                        pf_enc_rms(enc, bxb, 0ul, bw_pn, bxb, u_dim, u_eps, npos)
                    }
                }
                if (hcs.on) {
                    pf_hc_combine(enc, t, hcs, bxb, hcs.binj_f, u_dim)
                } elif (!narrow_last && g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                    //! layer_out_scale multiplies the WHOLE layer output: (x + branch) * s folds into the
                    //! residual add's ascale slot (a 4-byte view into the weights blob)
                    var bosc : MetalBuffer?
                    if (c.layer_out_scale) {
                        bosc = upload_region(addr < void? >(t.fblob[t.out_scale_off + l]), 4ul)
                    }
                    enc_add(enc, bx, 0ul, bxb, 0ul, u_total_dim, npos * dim, bosc)
                    if (l < nds) {   //! deepstack: slice l adds to the layer output (every row; text tails are zero)
                        enc_add(enc, bx, 0ul, bds, uint64(l * npos * dim * 4l), u_total_dim, npos * dim)
                    }
                }
            }
        }
        if (gpu_logits) {
            //! the caller samples from s.logits, so this closes the whole forward inside the GPU window
            unsafe {
                if (hcs.on) {   //! the head mixer stands where the final norm would - over the TRUNK's last wide row (the warm swapped the draft's in)
                    var hcs_t = hcs
                    if (warm) {
                        hcs_t.bhc = bwm_hc
                    }
                    pf_hc_head(enc, t, hcs_t, bxf, u_dim, u_eps)
                } else {
                    var bwfin = upload_region(addr < void? >(t.fblob[t.rms_final_off]), uint64(dim * 4l))
                    pf_enc_rms(enc, warm ? bwm_res : bx, uint64((npos - 1l) * dim * 4l), bwfin, bxf, u_dim, u_eps, 1l)
                }
                if (cls_fmt != KqFmt.q8) {
                    enc_site_gemv(enc, t, cls_fmt, t.wcls_off, c.vocab_size, dim, bxf, blog, u_dim, u_vocab)
                } else {
                    let bwc = blob_of(g_dev, t, t.wcls_off)
                    enc_gemv(enc, bwc.buf, bwc.boff, bxf, blog, u_dim, u_vocab, c.vocab_size)
                }
                if (u_flcap != null) {
                    //! final-logit soft cap (gemma2): cap*tanh over the classifier row in place
                    enc_softcap_row(enc, blog, 0ul, u_vocab, u_flcap, c.vocab_size)
                }
                if (bsupp != null) {
                    //! gemma4 suppressed ids pin to -1e30 AFTER the cap (the CPU classifier's order)
                    enc_suppress_row(enc, blog, 0ul, bsupp, u_nsupp, nsupp)
                }
            }
        }
        if (cap) {
            graph_flush_sched(enc)
            hz_coverage_check(metal_dispatch_calls() - hz_d0, g_hz_gated - hz_g0, "prefill")
        }
        metal_end_encoding(enc)
        metal_release(enc)
        encode_ms = double(get_time_usec(ts_enc)) / 1000.0lf
        metal_commit(cb)
        cbs |> push(cb)
    }
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
                    if (bks[l] == null || kv_shared_layer(t, l)) {
                        continue
                    }
                    if (mirk) {   //! the mirror's halves are the rows: the host store takes them byte for byte
                        let rowb = layer_kv_dim(c, l) * 2l
                        let lb = mirror_lbase(t, KVDtype.f16, mir_cap, l)
                        let ab = reinterpret<uint8?>(metal_buffer_contents(g_arena))
                        var kdst = kv_layer_kbase(s, l, c.seq_len)
                        var vdst = kv_layer_vbase(s, l, c.seq_len)
                        for (r in range64(store_nrun)) {
                            let j0 = runsp[r].j0
                            let nb = (runsp[r].j1 - j0) * rowb
                            memcpy(reinterpret<void?>(kdst + runsp[r].row0 * rowb), reinterpret<void?>(ab + mir_koff + uint64(lb + j0 * rowb)), nb)
                            memcpy(reinterpret<void?>(vdst + runsp[r].row0 * rowb), reinterpret<void?>(ab + mir_voff + uint64(lb + j0 * rowb)), nb)
                        }
                        continue
                    }
                    //! the chunk's rows sit past a continuation's gathered panel head
                    var kpan = reinterpret<uint8?>(metal_buffer_contents(bks[l]))
                    var vpan = reinterpret<uint8?>(metal_buffer_contents(bvs[l]))
                    let kvd_l = layer_kv_dim(c, l)
                    let hs_l = layer_head_size(c, l)
                    let koff_l = start_pos * kvd_l * 4l
                    memcpy(addr < void? >(s.k_b[0]), reinterpret<void?>(kpan + koff_l), npos * kvd_l * 4l)
                    memcpy(addr < void? >(s.v_b[0]), reinterpret<void?>(vpan + koff_l), npos * kvd_l * 4l)
                    //! tq4 sessions store in the ROTATED basis: the GPU ran attention unrotated, so rotate the
                    //! readback rows per head before the codec store
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
    gpu_ms = (gpu_t1 - gpu_t0) * 1000.0lf   //! whole-GPU window: first chunk start -> last chunk end
    let encwait_us = get_time_usec(ts_enc)
    let ok = ran
    if (mirk) {   //! a served chunk's rows stand in the mirror; a failed one leaves the rows before it
        mirror_set_watermark(s.uid, ran ? start_pos + npos : start_pos)
        g_pf_mirror_served += ran ? 1l : 0l
    }
    if (ran) {
        if (dn) {   //! the window completed — the GPU state now holds position start_pos+npos-1's update
            dn_mirror_advance(s.uid, start_pos + npos - 1l)
        }
        let ts_rb = ref_time_ticks()
        unsafe {
            memcpy(addr < void? >(s.x_b[0]), metal_buffer_contents(warm ? bwm_res : bx), npos * dim * 4l)
            if (gpu_logits) {
                memcpy(addr < void? >(s.logits[0]), metal_buffer_contents(blog), c.vocab_size * 4l)
            }
        }
        if (gpu_logits) {
            if (c.n_layer_nextn > 0l) {
                //! forward_prefill's mtp_h contract - bxf holds the last row's post-final-norm hidden; a hyper-connection
                //! model carries the trunk's last WIDE row instead (saved past the warm, else still in the residual rows)
                if (hcs.on) {
                    unsafe {
                        let wrows = reinterpret<uint8?>(metal_buffer_contents(warm ? bwm_hc : hcs.bhc))
                        stash_mtp_carry(s, reinterpret<void?>(wrows + uint64((npos - 1l) * hcd * 4l)), uint64(hcd * 4l), start_pos + npos)
                    }
                } else {
                    stash_mtp_carry(s, metal_buffer_contents(bxf), uint64(dim * 4l), start_pos + npos)
                }
            }
            prefill_override_logits_done()
        }
        readback_us += int64(get_time_usec(ts_rb))
        g_prefills++
    } else {
        to_log(LOG_ERROR, "dasLLAMA metal prefill: dispatch failed ({err}) - falling back to the CPU layer loop\n")   // nolint:PERF028,PERF026 — error path, and the CPU fallback dwarfs the log
        decline(MetalPrefillDecline.gpu_error)
    }
    //! return the pooled buffers (uniforms too); the weight regions stay resident
    pool_release(g_pf_pool, bx, bytes_x)
    pool_release(g_pf_pool, bxb, bytes_xb)
    pool_release(g_pf_pool, bxb2, bytes_x)
    pool_release(g_pf_pool, bq, bytes_q)
    pool_release(g_pf_pool, bhb, bytes_h)
    pool_release(g_pf_pool, bhb2, bytes_h)
    if (moe) {
        if (g_pf_moe_rows_on && ran) {   //! the last routed layer's counts, still in the buffer
            g_pf_moe_rows |> resize(c.n_expert)
            unsafe {
                memcpy(addr<void?>(g_pf_moe_rows[0]), metal_buffer_contents(bmcnt), c.n_expert * 4l)
            }
        }
        pool_release(g_pf_pool, bmlg, bytes_mlg)
        pool_release(g_pf_pool, bmsel, bytes_msel)
        pool_release(g_pf_pool, bmcnt, bytes_mcsr)
        pool_release(g_pf_pool, bmbase, bytes_mcsr)
        pool_release(g_pf_pool, bmbkt, bytes_mbkt)
        pool_release(g_pf_pool, bminv, bytes_minv)
        pool_release(g_pf_pool, bmg, bytes_mg)
        pool_release(g_pf_pool, bmg2, bytes_mg)
        pool_release(g_pf_pool, bmdn, bytes_mdn)
        if (bmxg != null) {
            pool_release(g_pf_pool, bmxg, bytes_mxg)
        }
        if (g4moe) {
            pool_release(g_pf_pool, bmg4rt, bytes_x)
        }
    }
    if (hcs.on) {
        pool_release(g_pf_pool, hcs.bhc, bytes_hcd)
        pool_release(g_pf_pool, hcs.bxn, bytes_hcd)
        pool_release(g_pf_pool, hcs.bgate, bytes_hcd)
        pool_release(g_pf_pool, hcs.blo, bytes_hlr)
        pool_release(g_pf_pool, hcs.binj_a, bytes_hinj)
        pool_release(g_pf_pool, hcs.binj_f, bytes_hinj)
        pool_release(g_pf_upool, hcs.u_hcd, 4ul)
        pool_release(g_pf_upool, hcs.u_lr, 4ul)
        pool_release(g_pf_upool, hcs.u_hc, 4ul)
        pool_release(g_pf_upool, hcs.u_np, 4ul)
        pool_release(g_pf_upool, hcs.u_lo_tot, 4ul)
        pool_release(g_pf_upool, hcs.u_tot, 4ul)
        pool_release(g_pf_upool, hcs.u_one, 4ul)
    }
    if (pls.on) {
        pool_release(g_pf_upool, pls.bemb, bytes_pin)
        pool_release(g_pf_pool, pls.bkey, bytes_hcd)
        pool_release(g_pf_pool, pls.bval, bytes_x)
        pool_release(g_pf_pool, pls.bgated, bytes_hcd)
        pool_release(g_pf_pool, pls.bcin, bytes_hcd)
        pool_release(g_pf_upool, pls.u_in, 4ul)
    }
    if (dn) {
        pool_release(g_pf_pool, bdnqkv, bytes_dn_cd)
        pool_release(g_pf_pool, bdncv, bytes_dn_cd)
        if (bdncvh != null) {
            pool_release(g_pf_pool, bdncvh, bytes_dn_cd / 2ul)
        }
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
    if (g_pf_bxh != null) {
        pool_release(g_pf_pool, g_pf_bxh, bytes_xh)
        g_pf_bxh = null
    }
    if (g_pf_bxh2 != null) {
        pool_release(g_pf_pool, g_pf_bxh2, bytes_xh)
        g_pf_bxh2 = null
    }
    g_pf_bxh_bytes = 0ul
    if (g_pf_bwh != null) {
        pool_release(g_pf_pool, g_pf_bwh, bytes_wh)
        g_pf_bwh = null
    }
    if (g_pf_bwh2 != null) {
        pool_release(g_pf_pool, g_pf_bwh2, bytes_wh)
        g_pf_bwh2 = null
    }
    g_pf_bwh_bytes = 0ul
    pool_release(g_pf_pool, batt, bytes_att)
    if (kv_twins) {
        pool_release(g_pf_pool, bkh, bytes_kvh)
        pool_release(g_pf_pool, bvh, bytes_kvh)
    }
    pool_release(g_pf_pool, bstats, bytes_stat)
    if (gpu_logits) {
        pool_release(g_pf_pool, bxf, uint64(dim * 4l))
        pool_release(g_pf_pool, blog, uint64(c.vocab_size * 4l))
        pool_release(g_pf_upool, u_vocab, 4ul)
    }
    if (warm) {
        pool_release(g_pf_pool, bwm_res, uint64(npos * dim * 4l))
        pool_release(g_pf_pool, bwm_emb, uint64(npos * dim * 4l))
        pool_release(g_pf_pool, bwm_cat, uint64(mp_hc * 2l * dim * 4l))
        pool_release(g_pf_pool, bwm_tmp, uint64(wm_rows * dim * 4l))
        pool_release(g_pf_upool, u_dim2, 4ul)
        pool_release(g_pf_upool, u_wm_tot, 4ul)
        if (hcs.on) {
            pool_release(g_pf_pool, bwm_hc, uint64(npos * hcd * 4l))
            pool_release(g_pf_upool, u_wm_hc_tot, 4ul)
        }
    }
    if (bds != null) {
        pool_release(g_pf_pool, bds, bytes_ds)
    }
    if (bwide != null) {
        pool_release(g_pf_upool, bwide, bytes_wide)
    }
    pool_release(g_pf_upool, bcos, bytes_tab)
    if (dual_rope) {
        pool_release(g_pf_upool, bcos_swa, bytes_tab_sw)
        pool_release(g_pf_upool, bsin_swa, bytes_tab_sw)
    }
    pool_release(g_pf_upool, bsin, bytes_tab)
    for (l in range64(c.n_layers + c.n_layer_nextn)) {
        if (bks[l] == null || kv_shared_layer(t, l)) {
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
    bks |> clear()      //! non-owning handles — the pool owns them now
    bvs |> clear()
    pool_release(g_pf_upool, u_dim, 4ul)
    pool_release(g_pf_upool, u_qd, 4ul)
    pool_release(g_pf_upool, u_kvd, 4ul)
    pool_release(g_pf_upool, u_hidden, 4ul)
    if (u_hidden2 != null) {
        pool_release(g_pf_upool, u_hidden2, 4ul)
    }
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
    if (wide) {
        pool_release(g_pf_upool, u_wide_rw, 4ul)
        pool_release(g_pf_upool, u_wide_dim, 4ul)
    }
    pool_release(g_pf_upool, u_total_h, 4ul)
    if (u_total_h2 != null) {
        pool_release(g_pf_upool, u_total_h2, 4ul)
    }
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
        to_log(LOG_INFO, "dasLLAMA metal prefill: npos={npos} gemm=mulmm total={float(total_us) / 1000.0}ms setup={float(setup_us) / 1000.0}ms encode+wait={float(encwait_us) / 1000.0}ms (encode {encode_ms}ms, gpu {gpu_ms}ms) kv+readback={float(readback_us) / 1000.0}ms dispatches={17l * c.n_layers + (gpu_logits ? 2l : 0l)} logits={gpu_logits ? "gpu" : "cpu"} kv={mirk ? "mirror" : "panels"}\n")   // nolint:PERF028,PERF026 — one line per prefill call, not per token
    }
    return ok
}

//! Knobs resolve once at [init]: get_env_variable allocates in the context heap on every call (PERF027).
var private g_pf_env_mulmm = true
var private g_pf_env_mm_tail = true
var private g_pf_env_half_x = true
var private g_pf_env_dev_w = true
var private g_pf_env_devw_res_bytes = 0ul   //!< DASLLAMA_METAL_DEVW_RESIDENT budget (MB env, bytes here); 0 = off
var private g_pf_env_tall = true
var private g_pf_tall_crowned : bool     //!< the box's "hmm_tall" crown (ARCHITECTURE_GPU_PREFILL.md#prefill-gemm-ladder)

def private pf_tall_on : bool => g_pf_env_tall && g_pf_tall_crowned
var private g_pf_env_qkr = true
var private g_pf_env_grid1d = false
var private g_pf_env_lastrow = true
var private g_pf_env_dbuf = true

var private CVT_MIN_ROWS = 256l          //!< sidecar knob "metal_cvt_min_rows" — cached at init
var private TALL_OCC_FLOOR = 64l         //!< sidecar knob "metal_tall_floor" — cached at init
var private DEVW_SMALL_PANEL = 32ul * 1024ul * 1024ul   //!< sidecar knob "metal_devw_small_panel_mb"
let DEVW_BIG_PANEL = 48ul * 1024ul * 1024ul
let DEVW_WIDE_N = 4096l
let DEVW_WIDE_N_ROWS = 512l
let DEVW_BIG_ROWS = 2048l
let DEVW_LONG_K = 4096l
let DEVW_LONG_K_ROWS = 1024l
let DEVW_MAX_TILES = 32l
var private TALLKQ_MIN_PANEL = 96ul * 1024ul * 1024ul //! DRAM-residency knee, raced on panel BYTES (ARCHITECTURE_GPU_PREFILL.md#prefill-gemm-ladder)
var private g_pf_env_span = true
var private g_pf_env_logits = true
var private g_pf_env_attn = true
var private g_pf_env_mirror = true
var private g_pf_env_unretained = false
var private g_pf_env_capture = true
var private g_pf_env_ncb = 0   //!< 0 = unset, fall back to the layer-derived default

//! the registered override: the driver, and the pool's spin window follows a window it served
def private metal_prefill_served(t : Model; var s : Session; npos, start_pos : int64) : bool
    => metal_served(metal_prefill_forward(t, s, npos, start_pos))

[init]
def dasllama_metal_prefill_register() {
    //! registered but dormant - only select_prefill_override("metal") activates it, and
    //! unsupported shapes decline per call
    register_prefill_override("metal", @@metal_prefill_served)
    register_prefill_override_mrope_tables("metal")
    register_prefill_override_ds_adds("metal")
    register_ple_gpu_gate(@@metal_ple_pre_gpu_gate)
    g_pf_env_mulmm = g_env_metal.mulmm
    g_pf_env_mirror = g_env_metal.pf_mirror
    if (!g_pf_env_mirror) {
        to_log(LOG_WARNING, "dasLLAMA metal prefill: DASLLAMA_METAL_PF_MIRROR=0 - a window's K/V rows take the f32 panels and the first decode step uploads them (numerics move with the form)\n")
    }
    g_pf_env_mm_tail = g_env_metal.mm_tail
    if (!g_pf_env_mm_tail) {
        to_log(LOG_WARNING, "dasLLAMA metal prefill: DASLLAMA_METAL_MM_TAIL=0 - small npos % 32 remainders bill the padded tile (numerics move with the form)\n")
    }
    g_pf_env_qkr = g_env_metal.qk_rope
    if (!g_pf_env_qkr) {
        to_log(LOG_INFO, "dasLLAMA metal prefill: DASLLAMA_METAL_QK_ROPE=0 - split norm/rope/convert dispatches (numerics move with the form)\n")
    }
    g_pf_env_grid1d = g_env_metal.grid1d
    g_pf_env_lastrow = g_env_metal.lastrow
    if (!g_pf_env_lastrow) {
        to_log(LOG_INFO, "dasLLAMA metal prefill: DASLLAMA_METAL_LASTROW=0 - the last dense layer's FFN runs every row\n")
    }
    g_pf_env_dbuf = g_env_metal.dbuf
    g_pf_env_span = g_env_metal.span
    g_pf_env_logits = g_env_metal.logits
    g_pf_env_attn = g_env_metal.attn
    g_pf_env_unretained = g_env_metal.unretained
    g_pf_env_devw_res_bytes = uint64(max(g_env_metal.devw_resident, 0)) * 1024ul * 1024ul
    g_pf_env_capture = g_env_metal.pf_capture
    g_pf_env_ncb = g_env_metal.ncb
    if (!empty(g_env_metal.prefill_skip)) {
        g_pf_skip = g_env_metal.prefill_skip
        to_log(LOG_WARNING, "dasLLAMA metal prefill: DASLLAMA_METAL_PREFILL_SKIP={g_pf_skip} - timing-attribution mode, output is GARBAGE\n")
    }
}


def private race_hmm_tall(dev : MetalDevice?; queue : MetalCommandQueue?) : MetalTensorRaceResult {
    let m = 512
    let kdim = 2048
    let ndim = 2048
    var p = race_pso_pair_stamp(dev, "hmm_tall", MetalHalfMulMmTH_metal_half_mulmm_th_msl, MetalHalfMulMmTH128_metal_half_mulmm_th_msl)
    return p.verdict if (!p.ok)
    var res = p.verdict
    let base_pso = p.base_pso
    let twin_pso = p.twin_pso
    var wa <- race_x_f16(ndim * kdim)
    var xa <- race_x_f16(m * kdim)
    var bw = race_buf(dev, uint64(ndim * kdim * 2), unsafe(addr<void?>(wa[0])))
    var bx = race_buf(dev, uint64(m * kdim * 2), unsafe(addr<void?>(xa[0])))
    var by_base = race_buf(dev, uint64(m * ndim * 4), null)
    var by_twin = race_buf(dev, uint64(m * ndim * 4), null)
    var bk = race_uniform_u32(dev, uint(kdim))
    var bn = race_uniform_u32(dev, uint(ndim))
    let tg = uint3(128u, 1u, 1u)
    race_pair_ms(queue, 5, res.base_ms, res.twin_ms, $(enc) {
        kn_pipeline(enc, base_pso)
        kn_buffer(enc, bw, 0ul, 0)
        kn_buffer(enc, bx, 0ul, 1)
        kn_buffer(enc, by_base, 0ul, 2)
        kn_buffer(enc, bk, 0ul, 3)
        kn_buffer(enc, bn, 0ul, 4)
        kn_dispatch(enc, uint3(uint(m / 32), uint(ndim / 64), 1u), tg)
    }, $(enc) {
        kn_pipeline(enc, twin_pso)
        kn_buffer(enc, bw, 0ul, 0)
        kn_buffer(enc, bx, 0ul, 1)
        kn_buffer(enc, by_twin, 0ul, 2)
        kn_buffer(enc, bk, 0ul, 3)
        kn_buffer(enc, bn, 0ul, 4)
        kn_dispatch(enc, uint3(uint(m / 128), uint(ndim / 64), 1u), tg)
    })
    kq_race_verdict(res, by_base, by_twin, m * ndim)
    metal_release(bw)
    metal_release(bx)
    metal_release(by_base)
    metal_release(by_twin)
    metal_release(bk)
    metal_release(bn)
    metal_release(base_pso)
    metal_release(twin_pso)
    delete wa
    delete xa
    return res
}

def private race_mulmm_bf16(dev : MetalDevice?; queue : MetalCommandQueue?) : MetalTensorRaceResult {
    let m = 512
    let kdim = 2048
    let ndim = 1024
    var p = race_pso_pair_stamp(dev, "mulmm_bf16", metal_bf16_mulmm_msl, MetalBf16MulMmT_metal_bf16_mulmm_t_msl)
    return p.verdict if (!p.ok)
    var res = p.verdict
    let base_pso = p.base_pso
    let twin_pso = p.twin_pso
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
    kq_race_verdict(res, by_base, by_twin, m * ndim)
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
    let m = 512
    let kdim = 2048
    let ndim = 1024
    let nkb = kdim / 32
    var p = race_pso_pair_stamp(dev, "mulmm_q8", metal_q8_mulmm_msl, MetalQ8MulMmT_metal_q8_mulmm_t_msl)
    return p.verdict if (!p.ok)
    var res = p.verdict
    let base_pso = p.base_pso
    let twin_pso = p.twin_pso
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
        kn_tgmem(enc, MetalQ8MulMmT_metal_q8_mulmm_t_msl_tgmem, 0)
        kn_buffer(enc, bw, 0ul, 0)
        kn_buffer(enc, bw, 0ul, 1)
        kn_buffer(enc, bxa, 0ul, 2)
        kn_buffer(enc, by_twin, 0ul, 3)
        kn_buffer(enc, bk, 0ul, 4)
        kn_buffer(enc, bn, 0ul, 5)
        kn_dispatch(enc, grid, tg)
    })
    kq_race_verdict(res, by_base, by_twin, m * ndim)
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

def private race_moe_mulmm_q8(dev : MetalDevice?; queue : MetalCommandQueue?) : MetalTensorRaceResult {
    let ne = 4
    let rows_per = 32
    let m = ne * rows_per
    let kdim = 2048
    let ndim = 1024
    let nkb = kdim / 32
    let eblk = ndim * nkb
    var p = race_pso_pair_stamp(dev, "moe_mulmm_q8", metal_moe_mulmm_q8_msl, MetalMoeMulMmQ8T_metal_moe_mulmm_q8_t_msl)
    return p.verdict if (!p.ok)
    var res = p.verdict
    let base_pso = p.base_pso
    let twin_pso = p.twin_pso
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
        bkt[i] = uint(i)   //! identity buckets: gather=0 semantics match
    }
    var bw = race_buf(dev, uint64(ne * eblk * 34), unsafe(addr<void?>(blob[0])))
    var bxa = race_buf(dev, uint64(m * kdim * 4), unsafe(addr<void?>(xa[0])))
    var by_base = race_buf(dev, uint64(m * ndim * 4), null)
    var by_twin = race_buf(dev, uint64(m * ndim * 4), null)
    var bcnt = race_buf(dev, uint64(ne * 4), unsafe(addr<void?>(cnt[0])))
    var bbase = race_buf(dev, uint64(ne * 4), unsafe(addr<void?>(basep[0])))
    var bbkt = race_buf(dev, uint64(m * 4), unsafe(addr<void?>(bkt[0])))
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
        kn_tgmem(enc, MetalMoeMulMmQ8T_metal_moe_mulmm_q8_t_msl_tgmem, 0)
        kn_buffer(enc, bw, 0ul, 0)
        kn_buffer(enc, bw, 0ul, 1)
        kn_buffer(enc, bxa, 0ul, 2)
        kn_buffer(enc, by_twin, 0ul, 3)
        kn_kargs(enc, rka, 5)
        kn_buffer(enc, bcnt, 0ul, 6)
        kn_buffer(enc, bbase, 0ul, 7)
        kn_dispatch(enc, grid, tg)
    })
    kq_race_verdict(res, by_base, by_twin, m * ndim)
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

[arch(at="../ARCHITECTURE_GPU_PREFILL_MOE.md#prefill-moe-buckets")]
def private race_moe_mulmm_mx4(dev : MetalDevice?; queue : MetalCommandQueue?) : MetalTensorRaceResult {   // nolint:STYLE038 — one-shot A/B race harness; buffers stay live to the release tail
    let ne = 4
    let rows_per = 32
    let m = ne * rows_per
    let kdim = 2048
    let ndim = 1024
    let nkb = kdim / 32
    let eblk = ndim * nkb
    var p = race_pso_pair_stamp(dev, "moe_mulmm_mx4", metal_moe_mulmm_mx4_msl, MetalMoeMulMmMx4T_metal_moe_mulmm_mx4_t_msl)
    return p.verdict if (!p.ok)
    var res = p.verdict
    let base_pso = p.base_pso
    let twin_pso = p.twin_pso
    var xa <- race_x_f32(m * kdim)
    var nplane : array<uint>
    nplane |> resize(ne * eblk * 4)   //! 16B nibbles per block
    for (i in range(ne * eblk * 4)) {
        nplane[i] = uint(i) * 2654435761u
    }
    var eplane : array<uint8>
    eplane |> resize(ne * eblk)
    for (i in range(ne * eblk)) {
        eplane[i] = uint8(125 + i % 4)   //! e8m0 near 1.0 — no overflow across the fold
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
        kn_tgmem(enc, MetalMoeMulMmMx4T_metal_moe_mulmm_mx4_t_msl_tgmem, 0)
        kn_buffer(enc, bnq, 0ul, 0)
        kn_buffer(enc, bee, 0ul, 1)
        kn_buffer(enc, bxa, 0ul, 3)
        kn_buffer(enc, by_twin, 0ul, 4)
        kn_kargs(enc, rka, 5)
        kn_buffer(enc, bcnt, 0ul, 6)
        kn_buffer(enc, bbase, 0ul, 7)
        kn_buffer(enc, bwbb, 0ul, 12)
        kn_buffer(enc, bvt, 0ul, 13)
        kn_dispatch(enc, grid, tg)
    })
    kq_race_verdict(res, by_base, by_twin, m * ndim)
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

//! the fixture planes come from `race_kq_planes` (metal_common) - the same planes the decode
//! small-batch race binds, so the two races agree on every format's layout
def private race_kq_mulmm(dev : MetalDevice?; queue : MetalCommandQueue?; family : string;
                          base_src, base_entry : string; base_fm : bool; base_tgmem : uint64;
                          twin_src, twin_entry : string; twin_fm : bool; twin_tgmem : uint64;
                          qu_per_sb : int; sform : KqRaceScaleForm) : MetalTensorRaceResult {
    let m = 512
    let kdim = 2048
    let ndim = 1024
    let nsb = kdim / 256
    var p = race_pso_pair(dev, family, base_src, base_entry, base_fm, twin_src, twin_entry, twin_fm)
    return p.verdict if (!p.ok)
    var res = p.verdict
    let base_pso = p.base_pso
    let twin_pso = p.twin_pso
    var xa <- race_x_f32(m * kdim)
    let nblk = ndim * nsb
    var planes = race_kq_planes(dev, nblk, qu_per_sb, sform)
    var bq = planes.bq
    var bs = planes.bs
    let doff = planes.doff
    var bxa = race_buf(dev, uint64(m * kdim * 4), unsafe(addr<void?>(xa[0])))
    var by_base = race_buf(dev, uint64(m * ndim * 4), null)
    var by_twin = race_buf(dev, uint64(m * ndim * 4), null)
    var bk = race_uniform_u32(dev, uint(kdim))
    var bn = race_uniform_u32(dev, uint(ndim))
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
    kq_race_verdict(res, by_base, by_twin, m * ndim)
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

//! synthetic Q/K/V with BOTH att/xb outputs pre-zeroed: causal-skipped tiles stay unwritten
//! in both kernels, and fresh-buffer garbage would flake the envelope compare
def private race_attn_pair(dev : MetalDevice?; queue : MetalCommandQueue?) : array<MetalTensorRaceResult> {   // nolint:STYLE038 — one-shot A/B race harness; buffers + PSOs stay live to the release tail
    var out : array<MetalTensorRaceResult>
    let heads = 8
    let hs = 128
    let qd = heads * hs
    let kvd = qd
    let mp = 512
    let np32 = 512
    var err = ""
    var q_pso = pipeline_from_source(dev, metal_attn_qk_mm_msl, metal_attn_qk_mm_msl_entry, metal_attn_qk_mm_msl_fastmath, err)
    var qt_pso = pipeline_from_source(dev, MetalAttnQKMmT_metal_attn_qk_mm_t_msl, MetalAttnQKMmT_metal_attn_qk_mm_t_msl_entry, MetalAttnQKMmT_metal_attn_qk_mm_t_msl_fastmath, err)
    var a_pso = pipeline_from_source(dev, MetalAttnAVMm_metal_attn_av_mm_msl, MetalAttnAVMm_metal_attn_av_mm_msl_entry, MetalAttnAVMm_metal_attn_av_mm_msl_fastmath, err)
    var at_pso = pipeline_from_source(dev, MetalAttnAVMmT_metal_attn_av_mm_t_msl, MetalAttnAVMmT_metal_attn_av_mm_t_msl_entry, MetalAttnAVMmT_metal_attn_av_mm_t_msl_fastmath, err)
    if (q_pso == null || qt_pso == null || a_pso == null || at_pso == null) {
        out |> emplace(MetalTensorRaceResult(family = "attn_qkmm", winner = q_pso != null ? "simdgroup" : "", base_ms = -1.0lf, twin_ms = -1.0lf, note = "pso: {err}"))
        out |> emplace(MetalTensorRaceResult(family = "attn_avmm", winner = a_pso != null ? "simdgroup" : "", base_ms = -1.0lf, twin_ms = -1.0lf, note = "pso: {err}"))
    } else {
        var qv <- race_x_f32(mp * qd)
        var kv <- race_x_f32(np32 * kvd)
        var pv <- race_x_f16(mp * np32 * heads)
        var sv <- race_x_f32(heads * mp * 2)
        var zeros : array<float>
        zeros |> resize(heads * mp * np32)
        var bq = race_buf(dev, uint64(mp * qd * 4), unsafe(addr<void?>(qv[0])))
        var bkk = race_buf(dev, uint64(np32 * kvd * 4), unsafe(addr<void?>(kv[0])))
        var bp = race_buf(dev, uint64(heads * mp * np32 * 2), unsafe(addr<void?>(pv[0])))
        var batt_b = race_buf(dev, uint64(heads * mp * np32 * 2), unsafe(addr<void?>(zeros[0])))
        var batt_t = race_buf(dev, uint64(heads * mp * np32 * 2), unsafe(addr<void?>(zeros[0])))
        var bxb_b = race_buf(dev, uint64(mp * qd * 4), unsafe(addr<void?>(zeros[0])))
        var bxb_t = race_buf(dev, uint64(mp * qd * 4), unsafe(addr<void?>(zeros[0])))
        var bstat = race_buf(dev, uint64(heads * mp * 2 * 4), unsafe(addr<void?>(sv[0])))
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
            kn_tgmem(enc, MetalAttnQKMmT_metal_attn_qk_mm_t_msl_tgmem, 0)
            kn_buffer(enc, bq, 0ul, 0)
            kn_buffer(enc, bkk, 0ul, 1)
            kn_buffer(enc, batt_t, 0ul, 2)
            kn_kargs(enc, rka, 3)
            kn_dispatch(enc, qgrid, tg)
        })
        kq_race_verdict(res, batt_b, batt_t, heads * mp * np32, half = true)
        out |> emplace(res)
        var res2 = MetalTensorRaceResult(family = "attn_avmm", winner = "", base_ms = -1.0lf, twin_ms = -1.0lf, note = "")
        race_pair_ms(queue, 5, res2.base_ms, res2.twin_ms, $(enc) {
            kn_pipeline(enc, a_pso)
            kn_tgmem(enc, MetalAttnAVMm_metal_attn_av_mm_msl_tgmem, 0)
            kn_buffer(enc, bp, 0ul, 0)
            kn_buffer(enc, bstat, 0ul, 4)
            kn_buffer(enc, bkk, 0ul, 1)
            kn_buffer(enc, bxb_b, 0ul, 2)
            kn_kargs(enc, rka, 3)
            kn_dispatch(enc, agrid, tg)
        }, $(enc) {
            kn_pipeline(enc, at_pso)
            kn_tgmem(enc, MetalAttnAVMmT_metal_attn_av_mm_t_msl_tgmem, 0)
            kn_buffer(enc, bp, 0ul, 0)
            kn_buffer(enc, bstat, 0ul, 4)
            kn_buffer(enc, bkk, 0ul, 1)
            kn_buffer(enc, bxb_t, 0ul, 2)
            kn_kargs(enc, rka, 3)
            kn_dispatch(enc, agrid, tg)
        })
        kq_race_verdict(res2, bxb_b, bxb_t, mp * qd)
        out |> emplace(res2)
        metal_release(bq)
        metal_release(bkk)
        metal_release(bp)
        metal_release(batt_b)
        metal_release(batt_t)
        metal_release(bxb_b)
        metal_release(bxb_t)
        metal_release(bstat)
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

//! the staged attention trio against the device pair on a continuation window: 512 queries on
//! 1536 rows of context, GQA, the pair's K/V twin pass inside its own window
[arch(at="../ARCHITECTURE_GPU_PREFILL_WINDOW.md#prefill-attn-device")]
def private race_attn_dev(dev : MetalDevice?; queue : MetalCommandQueue?) : MetalTensorRaceResult {
    var res = MetalTensorRaceResult(family = "attn_dev", winner = "", base_ms = -1.0lf, twin_ms = -1.0lf, note = "")
    let heads = 8
    let hs = 128
    let kv_mul = 4
    let qd = heads * hs
    let kvd = qd / kv_mul
    let mp = 512
    let qoff = 1536
    let nk = qoff + mp
    var err = ""
    var srcs <- [(src = MetalAttnQKMmTH_metal_attn_qk_mm_t_msl, entry = MetalAttnQKMmTH_metal_attn_qk_mm_t_msl_entry, fm = MetalAttnQKMmTH_metal_attn_qk_mm_t_msl_fastmath),
        (src = metal_attn_rowstat_msl, entry = metal_attn_rowstat_msl_entry, fm = metal_attn_rowstat_msl_fastmath),
        (src = MetalAttnAVMmP_metal_attn_av_mm_msl, entry = MetalAttnAVMmP_metal_attn_av_mm_msl_entry, fm = MetalAttnAVMmP_metal_attn_av_mm_msl_fastmath),
        (src = metal_half_rows_msl, entry = metal_half_rows_msl_entry, fm = metal_half_rows_msl_fastmath),
        (src = MetalAttnQKDev128_metal_attn_qk_dev_msl, entry = MetalAttnQKDev128_metal_attn_qk_dev_msl_entry, fm = MetalAttnQKDev128_metal_attn_qk_dev_msl_fastmath),
        (src = metal_attn_pexp_msl, entry = metal_attn_pexp_msl_entry, fm = metal_attn_pexp_msl_fastmath),
        (src = MetalAttnAVDev128_metal_attn_av_dev_msl, entry = MetalAttnAVDev128_metal_attn_av_dev_msl_entry, fm = MetalAttnAVDev128_metal_attn_av_dev_msl_fastmath)]
    var psos <- [for (s in srcs); pipeline_from_source(dev, s.src, s.entry, s.fm, err)]
    delete srcs
    if (psos_ready(psos)) {
        var kv <- race_x_f32(nk * kvd)
        var qh <- race_x_f16(mp * qd)
        var bqh = race_buf(dev, uint64(mp * qd * 2), unsafe(addr<void?>(qh[0])))
        var bkk = race_buf(dev, uint64(nk * kvd * 4), unsafe(addr<void?>(kv[0])))
        var bkh = race_buf(dev, uint64(nk * kvd * 2), null)
        var bvh = race_buf(dev, uint64(nk * kvd * 2), null)
        var batt_b = race_buf(dev, uint64(heads * mp * nk * 2), null)
        var batt_t = race_buf(dev, uint64(heads * mp * nk * 2), null)
        var bstat_b = race_buf(dev, uint64(heads * mp * 2 * 4), null)
        var bstat_t = race_buf(dev, uint64(heads * mp * 2 * 4), null)
        var bxb_b = race_buf(dev, uint64(mp * qd * 4), null)
        var bxb_t = race_buf(dev, uint64(mp * qd * 4), null)
        var bsink = race_buf(dev, uint64(heads * 4), null)
        var rka = AttnArgs(qd = uint(qd), kv_dim = uint(kvd), head_size = uint(hs), kv_mul = uint(kv_mul),
            npos = uint(nk), np32 = uint(nk), scale = 0.125, qoff = uint(qoff), qrows = uint(mp),
            window = 0u, softcap = 0.0, hass = 0u)
        var tka = HalfRowsArgs(total = uint(nk * kvd), scale = rka.scale)
        var tva = HalfRowsArgs(total = uint(nk * kvd), scale = 1.0)
        race_pair_ms(queue, 5, res.base_ms, res.twin_ms, $(enc) {
            enc_qk_mm_th_c_pso(enc, psos[0], bqh, bkk, batt_b, rka, int64(mp), int64(nk), int64(heads))
            enc_rowstat_pso(enc, psos[1], batt_b, rka, bstat_b, bsink, int64(mp), int64(heads))
            enc_av_mm_p_c_pso(enc, psos[2], batt_b, bkk, bxb_b, rka, bstat_b, int64(mp), int64(heads), int64(hs))
        }, $(enc) {
            enc_half_rows_pso(enc, psos[3], bkk, bkh, 0ul, tka, int64(nk * kvd))
            enc_half_rows_pso(enc, psos[3], bkk, bvh, 0ul, tva, int64(nk * kvd))
            enc_qk_dev128_c_pso(enc, psos[4], bqh, bkh, 0ul, batt_t, rka, int64(mp), int64(nk), int64(heads))
            enc_attn_pexp_pso(enc, psos[5], batt_t, rka, bstat_t, bsink, int64(mp), int64(heads))
            enc_av_dev128_c_pso(enc, psos[6], batt_t, bvh, 0ul, bxb_t, rka, bstat_t, int64(mp), int64(heads), int64(hs / 64))
        })
        kq_race_verdict(res, bxb_b, bxb_t, mp * qd)
        release_handles(bqh, bkk, bkh, bvh, batt_b, batt_t)
        release_handles(bstat_b, bstat_t, bxb_b, bxb_t, bsink)
        delete kv
        delete qh
    } else {
        res.winner = psos[0] != null ? "simdgroup" : ""
        res.note = "pso: {err}"
    }
    for (p in psos) {
        if (p != null) {
            metal_release(p)
        }
    }
    psos |> clear()   //! handles are metal_release-owned: a delete on the pointer array would free pointees
    unsafe {
        delete psos
    }
    return res
}

//! Tuner entry (the runtime.metal_tensor crown): race every tensor pso twin against its
//! shipped simdgroup kernel on THIS box — compile + output-match to qualify, faster GPU time
//! takes the family's crown. Empty when there is no Metal device.
def metal_tensor_race : array<MetalTensorRaceResult> {
    var results : array<MetalTensorRaceResult>
    results |> reserve(16)
    var dev = metal_create_system_default_device()
    if (dev == null) {
        return <- results
    }
    var queue = metal_new_command_queue(dev)
    results |> emplace(race_mulmm_bf16(dev, queue))
    results |> emplace(race_hmm_tall(dev, queue))
    results |> emplace(race_mulmm_q8(dev, queue))
    results |> emplace(race_moe_mulmm_q8(dev, queue))
    results |> emplace(race_moe_mulmm_mx4(dev, queue))
    for (r in race_attn_pair(dev, queue)) {
        results |> emplace(r)
    }
    results |> emplace(race_attn_dev(dev, queue))
    results |> emplace(race_kq_mulmm(dev, queue, "kq_mulmm_k4",
        MetalKqMulMmK4_metal_kq_mulmm_msl, MetalKqMulMmK4_metal_kq_mulmm_msl_entry, MetalKqMulMmK4_metal_kq_mulmm_msl_fastmath, MetalKqMulMmK4_metal_kq_mulmm_msl_tgmem,
        MetalKqMulMmK4T_metal_kq_mulmm_t_msl, MetalKqMulMmK4T_metal_kq_mulmm_t_msl_entry, MetalKqMulMmK4T_metal_kq_mulmm_t_msl_fastmath, MetalKqMulMmK4T_metal_kq_mulmm_t_msl_tgmem, 32, KqRaceScaleForm.compact))
    results |> emplace(race_kq_mulmm(dev, queue, "kq_mulmm_k5",
        MetalKqMulMmK5_metal_kq_mulmm_msl, MetalKqMulMmK5_metal_kq_mulmm_msl_entry, MetalKqMulMmK5_metal_kq_mulmm_msl_fastmath, MetalKqMulMmK5_metal_kq_mulmm_msl_tgmem,
        MetalKqMulMmK5T_metal_kq_mulmm_t_msl, MetalKqMulMmK5T_metal_kq_mulmm_t_msl_entry, MetalKqMulMmK5T_metal_kq_mulmm_t_msl_fastmath, MetalKqMulMmK5T_metal_kq_mulmm_t_msl_tgmem, 40, KqRaceScaleForm.compact))
    results |> emplace(race_kq_mulmm(dev, queue, "kq_mulmm_k6",
        MetalKqMulMmK6_metal_kq_mulmm_msl, MetalKqMulMmK6_metal_kq_mulmm_msl_entry, MetalKqMulMmK6_metal_kq_mulmm_msl_fastmath, MetalKqMulMmK6_metal_kq_mulmm_msl_tgmem,
        MetalKqMulMmK6T_metal_kq_mulmm_k6_t_msl, MetalKqMulMmK6T_metal_kq_mulmm_k6_t_msl_entry, MetalKqMulMmK6T_metal_kq_mulmm_k6_t_msl_fastmath, MetalKqMulMmK6T_metal_kq_mulmm_k6_t_msl_tgmem, 48, KqRaceScaleForm.split_d))
    results |> emplace(race_kq_mulmm(dev, queue, "kq_mulmm_iq4xs",
        MetalKqMulMmIq4xs_metal_kq_mulmm_msl, MetalKqMulMmIq4xs_metal_kq_mulmm_msl_entry, MetalKqMulMmIq4xs_metal_kq_mulmm_msl_fastmath, MetalKqMulMmIq4xs_metal_kq_mulmm_msl_tgmem,
        MetalKqMulMmIq4xsT_metal_kq_mulmm_iq4xs_t_msl, MetalKqMulMmIq4xsT_metal_kq_mulmm_iq4xs_t_msl_entry, MetalKqMulMmIq4xsT_metal_kq_mulmm_iq4xs_t_msl_fastmath, MetalKqMulMmIq4xsT_metal_kq_mulmm_iq4xs_t_msl_tgmem, 32, KqRaceScaleForm.split_d))
    results |> emplace(race_kq_mulmm(dev, queue, "kq_mulmm_iq4nl",
        MetalKqMulMmIq4nl_metal_kq_mulmm_msl, MetalKqMulMmIq4nl_metal_kq_mulmm_msl_entry, MetalKqMulMmIq4nl_metal_kq_mulmm_msl_fastmath, MetalKqMulMmIq4nl_metal_kq_mulmm_msl_tgmem,
        MetalKqMulMmIq4nlT_metal_kq_mulmm_iq4xs_t_msl, MetalKqMulMmIq4nlT_metal_kq_mulmm_iq4xs_t_msl_entry, MetalKqMulMmIq4nlT_metal_kq_mulmm_iq4xs_t_msl_fastmath, MetalKqMulMmIq4nlT_metal_kq_mulmm_iq4xs_t_msl_tgmem, 32, KqRaceScaleForm.pure_d))
    results |> emplace(race_kq_mulmm(dev, queue, "kq_mulmm_q40",
        MetalKqMulMmQ40_metal_kq_mulmm_msl, MetalKqMulMmQ40_metal_kq_mulmm_msl_entry, MetalKqMulMmQ40_metal_kq_mulmm_msl_fastmath, MetalKqMulMmQ40_metal_kq_mulmm_msl_tgmem,
        MetalKqMulMmQ40T_metal_kq_mulmm_iq4xs_t_msl, MetalKqMulMmQ40T_metal_kq_mulmm_iq4xs_t_msl_entry, MetalKqMulMmQ40T_metal_kq_mulmm_iq4xs_t_msl_fastmath, MetalKqMulMmQ40T_metal_kq_mulmm_iq4xs_t_msl_tgmem, 32, KqRaceScaleForm.pure_d))
    results |> emplace(race_kq_mulmm(dev, queue, "kq_mulmm_k3",
        MetalKqMulMmK3_metal_kq_mulmm_msl, MetalKqMulMmK3_metal_kq_mulmm_msl_entry, MetalKqMulMmK3_metal_kq_mulmm_msl_fastmath, MetalKqMulMmK3_metal_kq_mulmm_msl_tgmem,
        MetalKqMulMmK3T_metal_kq_mulmm_k3_t_msl, MetalKqMulMmK3T_metal_kq_mulmm_k3_t_msl_entry, MetalKqMulMmK3T_metal_kq_mulmm_k3_t_msl_fastmath, MetalKqMulMmK3T_metal_kq_mulmm_k3_t_msl_tgmem, 24, KqRaceScaleForm.split_d))
    results |> emplace(race_kq_mulmm(dev, queue, "kq_mulmm_iq3s",
        MetalKqMulMmIq3s_metal_kq_mulmm_msl, MetalKqMulMmIq3s_metal_kq_mulmm_msl_entry, MetalKqMulMmIq3s_metal_kq_mulmm_msl_fastmath, MetalKqMulMmIq3s_metal_kq_mulmm_msl_tgmem,
        MetalKqMulMmIq3sT_metal_kq_mulmm_iq3s_t_msl, MetalKqMulMmIq3sT_metal_kq_mulmm_iq3s_t_msl_entry, MetalKqMulMmIq3sT_metal_kq_mulmm_iq3s_t_msl_fastmath, MetalKqMulMmIq3sT_metal_kq_mulmm_iq3s_t_msl_tgmem, 26, KqRaceScaleForm.split_d))
    results |> emplace(race_kq_mulmm(dev, queue, "kq_mulmm_iq3xxs",
        MetalKqMulMmIq3xxs_metal_kq_mulmm_msl, MetalKqMulMmIq3xxs_metal_kq_mulmm_msl_entry, MetalKqMulMmIq3xxs_metal_kq_mulmm_msl_fastmath, MetalKqMulMmIq3xxs_metal_kq_mulmm_msl_tgmem,
        MetalKqMulMmIq3xxsT_metal_kq_mulmm_iq3xxs_t_msl, MetalKqMulMmIq3xxsT_metal_kq_mulmm_iq3xxs_t_msl_entry, MetalKqMulMmIq3xxsT_metal_kq_mulmm_iq3xxs_t_msl_fastmath, MetalKqMulMmIq3xxsT_metal_kq_mulmm_iq3xxs_t_msl_tgmem, 24, KqRaceScaleForm.split_d))
    results |> emplace(race_kq_mulmm(dev, queue, "kq_mulmm_k2",
        MetalKqMulMmK2_metal_kq_mulmm_msl, MetalKqMulMmK2_metal_kq_mulmm_msl_entry, MetalKqMulMmK2_metal_kq_mulmm_msl_fastmath, MetalKqMulMmK2_metal_kq_mulmm_msl_tgmem,
        MetalKqMulMmK2T_metal_kq_mulmm_k2_t_msl, MetalKqMulMmK2T_metal_kq_mulmm_k2_t_msl_entry, MetalKqMulMmK2T_metal_kq_mulmm_k2_t_msl_fastmath, MetalKqMulMmK2T_metal_kq_mulmm_k2_t_msl_tgmem, 16, KqRaceScaleForm.split_dmin))
    results |> emplace(race_kq_mulmm(dev, queue, "kq_mulmm_iq2s",
        MetalKqMulMmIq2s_metal_kq_mulmm_msl, MetalKqMulMmIq2s_metal_kq_mulmm_msl_entry, MetalKqMulMmIq2s_metal_kq_mulmm_msl_fastmath, MetalKqMulMmIq2s_metal_kq_mulmm_msl_tgmem,
        MetalKqMulMmIq2sT_metal_kq_mulmm_iq2s_t_msl, MetalKqMulMmIq2sT_metal_kq_mulmm_iq2s_t_msl_entry, MetalKqMulMmIq2sT_metal_kq_mulmm_iq2s_t_msl_fastmath, MetalKqMulMmIq2sT_metal_kq_mulmm_iq2s_t_msl_tgmem, 18, KqRaceScaleForm.split_d))
    results |> emplace(race_kq_mulmm(dev, queue, "kq_mulmm_iq2xs",
        MetalKqMulMmIq2xs_metal_kq_mulmm_msl, MetalKqMulMmIq2xs_metal_kq_mulmm_msl_entry, MetalKqMulMmIq2xs_metal_kq_mulmm_msl_fastmath, MetalKqMulMmIq2xs_metal_kq_mulmm_msl_tgmem,
        MetalKqMulMmIq2xsT_metal_kq_mulmm_iq2xs_t_msl, MetalKqMulMmIq2xsT_metal_kq_mulmm_iq2xs_t_msl_entry, MetalKqMulMmIq2xsT_metal_kq_mulmm_iq2xs_t_msl_fastmath, MetalKqMulMmIq2xsT_metal_kq_mulmm_iq2xs_t_msl_tgmem, 16, KqRaceScaleForm.split_d))
    results |> emplace(race_kq_mulmm(dev, queue, "kq_mulmm_iq2xxs",
        MetalKqMulMmIq2xxs_metal_kq_mulmm_msl, MetalKqMulMmIq2xxs_metal_kq_mulmm_msl_entry, MetalKqMulMmIq2xxs_metal_kq_mulmm_msl_fastmath, MetalKqMulMmIq2xxs_metal_kq_mulmm_msl_tgmem,
        MetalKqMulMmIq2xxsT_metal_kq_mulmm_iq2xxs_t_msl, MetalKqMulMmIq2xxsT_metal_kq_mulmm_iq2xxs_t_msl_entry, MetalKqMulMmIq2xxsT_metal_kq_mulmm_iq2xxs_t_msl_fastmath, MetalKqMulMmIq2xxsT_metal_kq_mulmm_iq2xxs_t_msl_tgmem, 16, KqRaceScaleForm.split_d))
    metal_release(queue)
    metal_release(dev)
    return <- results
}
