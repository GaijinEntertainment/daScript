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
require dasllama/dasllama_gpu_knee
require dasllama/dasllama_metal_kernels  // the shared kq decode helpers (grid tables, ksign7) for the tensor twins
require dasllama/dasllama_metal_lens
require dasllama/dasllama_tower
require dasllama/dasllama_load   // register_devw_kq_mirror: the image bake's CPU twin of the split-scale dequant passes
require dasllama/dasllama_ple   // ple_plane: the per-layer-embedding planes

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
    @template_constant NT : uint = 64u      //!< output columns a threadgroup holds
    @template_constant SK : bool = false    //!< split-K: the grid's z threadgroups share a tile's K, each into its own partial plane
    @ssbo @binding = 0 @role = "weight" @off = "wboff" wsh : array<float16> //!< 34B-block W blob, half-scale view
    @ssbo @binding = 1 @role = "weight" @off = "wboff" wqb : array<int8>    //!< the SAME blob buffer, byte view
    @ssbo @binding = 2 @off = "xoff" xf : array<XT>  //!< raw activations (float, or the converted f16 panel)
    @ssbo @binding = 3 @off = "yoff" @span = "mp*d*4" @template_gate = "!SK" y : array<float>
    @ssbo @binding = 3 @off = "yoff" @span = "nsk*mp*d*4" @template_gate = SK yk : array<float>   //!< `nsk` partial planes of mp x d
    @uniform @binding = 4 kdim : uint
    @uniform @binding = 5 ndim : uint
    @workgroup twb : float16[TWB]

    [metal_kernel(float_a_ok=true), arch(at="../ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-split-k")]
    def metal_q8_mulmm_t {
        let mBase = gl_WorkGroupID.x * MT
        let nBase = gl_WorkGroupID.y * NT
        let nkb = kdim / 32u
        static_if (SK) {
            let kper = nkb / gl_NumWorkGroups.z
            let kb0 = gl_WorkGroupID.z * kper
            let blk0 = nBase * nkb + kb0
            let sp = unsafe(addr(wsh[blk0 * 17u]))
            let wp = unsafe(addr(wqb[blk0 * 34u + 2u]))
            let ap = unsafe(addr(xf[mBase * kdim + kb0 * 32u]))
            var cp = unsafe(addr(yk[gl_WorkGroupID.z * gl_NumWorkGroups.x * MT * ndim + mBase * ndim + nBase]))
            tmm2d_q8u_f32(MT, NT, 4u, twb, sp, wp, ap, kdim, cp, ndim, kper * 32u, nkb, gl_LocalInvocationIndex, BK)
        } else {
            let blk0 = nBase * nkb
            let sp = unsafe(addr(wsh[blk0 * 17u]))
            let wp = unsafe(addr(wqb[blk0 * 34u + 2u]))
            let ap = unsafe(addr(xf[mBase * kdim]))
            var cp = unsafe(addr(y[mBase * ndim + nBase]))
            tmm2d_q8u_f32(MT, NT, 4u, twb, sp, wp, ap, kdim, cp, ndim, kdim, nkb, gl_LocalInvocationIndex, BK)
        }
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

//! the split-K stamps: `nsk` threadgroups share a tile's K, each writing its own partial plane of the scratch
[metal_dispatch(name = "enc_gemm_mm_thsk_c", pso = "g_pf_pso_mm_thsk", tgmem = "MetalQ8MulMmTHSk_metal_q8_mulmm_t_msl_tgmem", tg = 128, grid = "mp/32, d/32, nsk", params = "mp : int64, d : int64, nsk : int64")]
class MetalQ8MulMmTHSk : MetalQ8MulMmTensorT {
    typedef XT = float16
    override BK = 128u
    override NT = 32u
    override SK = true
}

//! the 16-row split stamps: a window of 16 rows or fewer runs the tensor op at 16 rows, which costs 0.69 of the 32-row op
[metal_dispatch(name = "enc_gemm_mm_thsk16_c", pso = "g_pf_pso_mm_thsk16", tgmem = "MetalQ8MulMmTHSk16_metal_q8_mulmm_t_msl_tgmem", tg = 128, grid = "mp/16, d/32, nsk", params = "mp : int64, d : int64, nsk : int64")]
class MetalQ8MulMmTHSk16 : MetalQ8MulMmTensorT {
    typedef XT = float16
    override BK = 128u
    override NT = 32u
    override SK = true
    override MT = 16u
}

[metal_dispatch(name = "enc_gemm_mm_tsk16_c", pso = "g_pf_pso_mm_tsk16", tgmem = "MetalQ8MulMmTSk16_metal_q8_mulmm_t_msl_tgmem", tg = 128, grid = "mp/16, d/32, nsk", params = "mp : int64, d : int64, nsk : int64")]
class MetalQ8MulMmTSk16 : MetalQ8MulMmTensorT {
    typedef XT = float
    override BK = 128u
    override NT = 32u
    override SK = true
    override MT = 16u
}

[metal_dispatch(name = "enc_gemm_mm_tsk_c", pso = "g_pf_pso_mm_tsk", tgmem = "MetalQ8MulMmTSk_metal_q8_mulmm_t_msl_tgmem", tg = 128, grid = "mp/32, d/32, nsk", params = "mp : int64, d : int64, nsk : int64")]
class MetalQ8MulMmTSk : MetalQ8MulMmTensorT {
    typedef XT = float
    override BK = 128u
    override NT = 32u
    override SK = true
}

struct CvtArgs {
    total : uint
}

//! f32 elements to halves at an element base of the target, scaled: an activation panel's twin, an attention K or V panel's
//! twin (the softmax scale on K), a window's K and V rows into the KV mirror, its Q rows under the scale the mirror carries; the
//! clamp template's f16 arm at the half range's bounds (`total` = the elements over four)
[metal_dispatch(name = "enc_half_rows", pso = "g_pf_pso_half_rows", tg = 64, grid = "total/64", params = "total : int64")]
class MetalHalfRows : GkClampRows {
    override OUT16 = true

    [metal_kernel(name = "metal_half_rows_msl")]
    def metal_half_rows {
        clamp_rows_body()
    }
}

//! `n` f32 elements of `bx` from element `xoff` as halves at element `hoff` of `bxh`, scaled by `scale`
def tw_half(enc : MetalComputeEncoder?; bx, bxh : MetalBuffer?; n : int64; scale : float = 1.0; hoff : uint64 = 0ul; xoff : uint64 = 0ul) {
    var a = gk_f16_rows_args(uint(n), scale, uint(hoff))
    a.xoff = uint(xoff)
    enc_half_rows(enc, bx, bxh, a, (n + 3l) / 4l)
}

//! The window's `rows` roped K and V rows from row `src_row` of the layer's panels into a mirror slab of codec `dt` at byte
//! offsets `mkoff` / `mvoff`: q8_0 and tq4 through their store stamps, f32 a copy (`u_n` holds the rows' elements)
[arch(at = "../ARCHITECTURE_GPU_PREFILL_WINDOW.md#prefill-kv-mirror")]
def private pf_enc_kv_feed(enc : MetalComputeEncoder?; dt : KVDtype; bk, bv : MetalBuffer?; src_row, rows, kvd, hs : int64;
                           mkoff, mvoff : uint64; u_n : MetalBuffer?) {
    let soff = uint64(src_row * kvd * 4l)
    if (dt == KVDtype.f32) {
        enc_copy_row(enc, bk, soff, g_arena, mkoff, u_n, rows * kvd)
        enc_copy_row(enc, bv, soff, g_arena, mvoff, u_n, rows * kvd)
        return
    }
    var ka = KvRowsArgs(rows = uint(rows), nblk = uint(kvd / 32l), hs = uint(hs))
    if (dt == KVDtype.tq4) {
        enc_kv_rows_tq4(enc, bk, soff, bv, soff, g_arena, mkoff, g_arena, g_arena, mvoff, g_arena, signs_buffer(), ka, 2l * (kvd / hs), rows, hs)
    } else {
        enc_kv_rows_q8(enc, bk, soff, bv, soff, g_arena, mkoff, g_arena, g_arena, mvoff, g_arena, ka, 2l * rows * kvd)   // a lane an element
    }
}

//! A window session's K/V codec its mirror feed serves: the f16 halves, or a block or f32 codec on both sides whose rows the
//! store stamps quantize (a tq4 head is the rotation's power of two inside the threadgroup)
def private pf_kv_feed_codec_ok(t : Model; s : Session) : bool {
    let dt = s.kv_dtype_k
    if (dt != s.kv_dtype_v) {
        return false
    }
    if (dt == KVDtype.f16 || dt == KVDtype.f32) {
        return true
    }
    if (dt == KVDtype.q8_0) {
        return g_pso_kv_rows_q8 != null
    }
    if (dt == KVDtype.tq4) {
        let hs = t.config.head_size
        let hs_sw = t.config.head_size_swa > 0l ? t.config.head_size_swa : hs
        return g_pso_kv_rows_tq4 != null && tq4_head_ok(hs) && tq4_head_ok(hs_sw)
    }
    return false
}

//! a head the tq4 rotation spans inside one threadgroup: a power of two, a whole simdgroup, no wider than the shell's buffer
def private tq4_head_ok(hs : int64) : bool => hs >= 32l && hs <= TQ4_SIGNS_MAX_HS && (hs & (hs - 1l)) == 0l

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

    [metal_kernel(name="metal_dequant_k6h_msl"), arch(at="../ARCHITECTURE_IMAGE_GPU_PLANES.md#image-devw-plane")]
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
    @template_constant SK : bool = false    //!< split-K: the grid's z threadgroups share a tile's K, each into its own partial plane
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
        let kper = SK ? kdim / gl_NumWorkGroups.z : kdim
        let kbeg = SK ? gl_WorkGroupID.z * kper : 0u
        let ldc = SK ? gl_NumWorkGroups.y * 64u : ndim   //! a split tile's partial planes are its own columns wide; `ndim` is the site's output stride
        let plane = SK ? gl_WorkGroupID.z * gl_NumWorkGroups.x * MT * ldc : 0u
        let wp = unsafe(addr(wh[nBase * kdim + kbeg]))
        let ap = unsafe(addr(xf[mBase * kdim + kbeg]))
        var cp = unsafe(addr(y[plane + mBase * ldc + nBase]))
        tmm2d_f16w_f32(MT, 64u, 4u, ap, kdim, wp, kdim, cp, ldc, kper)
    }
}

[metal_dispatch(name = "enc_gemm_hmm_th", pso = "g_pf_pso_hmm_th", tg = 128, grid = "mp/32, d/64", params = "mp : int64, d : int64")]
class MetalHalfMulMmTH : MetalHalfMulMmTensorT {}

//! the dev-W tile split along K (`ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-split-k`): `nsk` partial planes into `y`
[metal_dispatch(name = "enc_gemm_hmm_thsk", pso = "g_pf_pso_hmm_thsk", tg = 128, grid = "mp/32, d/64, nsk", params = "mp : int64, d : int64, nsk : int64")]
class MetalHalfMulMmTHSk : MetalHalfMulMmTensorT {
    override SK = true
}

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
    @template_constant SK : bool = false   //!< split-K: the grid's z threadgroups share a tile's K, each into its own partial plane
    @ssbo @binding = 0 @role = "weight" @off = "wboff" wbh : array<uint16> //!< bf16 W rows, halfword view
    @ssbo @binding = 2 @off = "xoff" xf : array<XT>  //!< raw activations (float, or the converted f16 panel)
    @ssbo @binding = 3 @off = "yoff" @template_gate = "!SK" y : array<float>
    @ssbo @binding = 3 @off = "yoff" @span = "nsk*mp*nd*4" @template_gate = SK yk : array<float>   //!< `nsk` partial planes of mp x nd
    @uniform @binding = 4 kdim : uint
    @uniform @binding = 5 ndim : uint

    [metal_kernel(float_a_ok=true), arch(at="../ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-split-k")]
    def metal_bf16_mulmm_t {
        let mBase = gl_WorkGroupID.x * MT
        let nBase = gl_WorkGroupID.y * 64u
        static_if (SK) {
            let kper = kdim / gl_NumWorkGroups.z
            let k0 = gl_WorkGroupID.z * kper
            let ap = unsafe(addr(xf[mBase * kdim + k0]))
            let wp = unsafe(addr(wbh[nBase * kdim + k0]))
            var cp = unsafe(addr(yk[gl_WorkGroupID.z * gl_NumWorkGroups.x * MT * ndim + mBase * ndim + nBase]))
            tmm2d_f32_bf16_f32(MT, 64u, 4u, ap, kdim, wp, kdim, cp, ndim, kper)
        } else {
            let ap = unsafe(addr(xf[mBase * kdim]))
            let wp = unsafe(addr(wbh[nBase * kdim]))
            var cp = unsafe(addr(y[mBase * ndim + nBase]))
            tmm2d_f32_bf16_f32(MT, 64u, 4u, ap, kdim, wp, kdim, cp, ndim, kdim)
        }
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

[metal_dispatch(name = "pf_enc_bf16_mm_tsk_c", pso = "g_pf_pso_bf16_mm_tsk", tg = 128, grid = "mp/32, nd/64, nsk", params = "mp : int64, nd : int64, nsk : int64")]
class MetalBf16MulMmTSk : MetalBf16MulMmTensorT {
    typedef XT = float
    override SK = true
}

[metal_dispatch(name = "pf_enc_bf16_mm_thsk_c", pso = "g_pf_pso_bf16_mm_thsk", tg = 128, grid = "mp/32, nd/64, nsk", params = "mp : int64, nd : int64, nsk : int64")]
class MetalBf16MulMmTHSk : MetalBf16MulMmTensorT {
    typedef XT = float16
    override SK = true
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

//! the rows a routed tile's tensor op runs at: a 32-row tile whose expert leaves it eight or sixteen live rows runs
//! the op at that height (a tile costs its op whatever its live rows; 8 and 16 rows cost 0.62 and 0.69 of 32), every
//! other tile its own height - both MoE scaffolds' ADAPT arm
def moe_tile_rows(mt, live : uint) : uint => mt != 32u ? mt : (live <= 8u ? 8u : (live <= 16u ? 16u : 32u))

//! The routed tile walk both MoE tensor scaffolds run: expert z's tile of MT rows (8 or 16 under `ADAPT` when it holds that few) over
//! its bucket rows, W staged a chunk of `STAGE_K` a round by `STAGE_ITEMS` work items, the tensor op over the stage; a scaffold
//! supplies its widths and the format's stage, and the derived class the x, y and twb planes
[ |> template_struct_instance]
class template MetalMoeTileWalkT {
    @template_constant MT : uint = 32u
    @template_constant REM : bool = false   //!< the routed THR stamp's remainder pass
    @template_constant STAGE_ITEMS : uint = 256u     //! per staged chunk
    @template_constant STAGE_K : uint = 64u     //! staged chunk depth, in k elements
    @template_constant ADAPT : bool = true   //!< a routed 32-row tile runs the tensor op at 8 or 16 rows when its expert holds that few (bit-equal; the op is the tile's cost)
    @template_constant KO_STAGE : bool = false   //!< lab knockout: no W staging (the tensor op over whatever the tile holds)
    @template_constant KO_TENSOR : bool = false   //!< lab knockout: no tensor op (the staging alone)
    @template_constant STORE_HOOK : bool = false   //!< the MT-row tile stores through `store_out` (the mx4 twin's bias store), else plain
    @template_constant SK : bool = false   //!< a split-K dense stamp: the routed walk is absent (its output plane is the partial planes')

    def abstract walk_kdim : uint
    def abstract walk_ndim : uint
    def abstract walk_stage(work, kb, e, nBase, nsb : uint) : void   //! one work item of chunk kb of expert e's columns from nBase

    //! the MT-row tile's store (the mx4 twin's bias store overrides it); the ADAPT walk's 8- and 16-row tiles store plain
    def store_out(var acc : float[8192]; var cp : float?; e, nBase : uint) {
        tmm2d_tg_store(acc, cp, MT, 64u, walk_ndim())
    }

    def tile_rows(live : uint) : uint {
        static_if (ADAPT) {
            return moe_tile_rows(MT, live)
        } else {
            return MT
        }
    }

    //! expert z's `erows` bucket rows from row `rbase`
    def moe_walk(erows, rbase : uint; rem : bool) {   // nolint:STYLE038 — one kernel body: the three tile walks are one op each in its own scope
        static_if (!SK) {
            let e = gl_WorkGroupID.z
            let mBase = gl_WorkGroupID.x * MT + (rem ? erows / 128u * 128u : 0u)
            let lim = MT == 128u ? erows / 128u * 128u : (erows + 31u) / 32u * 32u
            if (mBase >= lim) {
                return
            }
            let live = min(erows - mBase, MT)
            let tr = tile_rows(live)
            let nBase = gl_WorkGroupID.y * 64u
            let nkb = walk_kdim() / 32u
            let nsb = walk_kdim() / 256u
            let lid = gl_LocalInvocationID.x
            var cp = unsafe(addr(y[(rbase + mBase) * walk_ndim() + nBase]))
            // one tile walk per tensor height: the op's begin, steps and store share one scope (the emitter's
            // cooperative-tensor declarations are block-scoped), so each height carries its own copy of the walk
            if (tr == 8u) {
                var acc : float[8192]
                tmm2d_tg_begin_deva(acc, unsafe(addr(xf[0])), 8u, 64u, 4u, STAGE_K)
                var kb = 0u
                while (kb < nkb) {
                    static_if (!KO_STAGE) {
                        var work = lid
                        while (work < STAGE_ITEMS) {
                            walk_stage(work, kb, e, nBase, nsb)
                            work += gl_WorkGroupSize.x
                        }
                    }
                    barrier()
                    static_if (!KO_TENSOR) {
                        tmm2d_tg_step_deva(acc, unsafe(addr(xf[(rbase + mBase) * walk_kdim() + kb * 32u])), walk_kdim(), twb, 8u, 64u, STAGE_K)
                    }
                    barrier()
                    kb += STAGE_K / 32u
                }
                static_if (!KO_TENSOR) {
                    tmm2d_tg_store(acc, cp, 8u, 64u, walk_ndim())
                }
            } elif (tr == 16u) {   // nolint:LINT009,LINT019 — the lab's knockout stamps drop the op, and the row heights then read alike
                var acc : float[8192]
                tmm2d_tg_begin_deva(acc, unsafe(addr(xf[0])), 16u, 64u, 4u, STAGE_K)
                var kb = 0u
                while (kb < nkb) {
                    static_if (!KO_STAGE) {
                        var work = lid
                        while (work < STAGE_ITEMS) {
                            walk_stage(work, kb, e, nBase, nsb)
                            work += gl_WorkGroupSize.x
                        }
                    }
                    barrier()
                    static_if (!KO_TENSOR) {
                        tmm2d_tg_step_deva(acc, unsafe(addr(xf[(rbase + mBase) * walk_kdim() + kb * 32u])), walk_kdim(), twb, 16u, 64u, STAGE_K)
                    }
                    barrier()
                    kb += STAGE_K / 32u
                }
                static_if (!KO_TENSOR) {
                    tmm2d_tg_store(acc, cp, 16u, 64u, walk_ndim())
                }
            } else {
                var acc : float[8192]
                tmm2d_tg_begin_deva(acc, unsafe(addr(xf[0])), MT, 64u, 4u, STAGE_K)
                var kb = 0u
                while (kb < nkb) {
                    static_if (!KO_STAGE) {
                        var work = lid
                        while (work < STAGE_ITEMS) {
                            walk_stage(work, kb, e, nBase, nsb)
                            work += gl_WorkGroupSize.x
                        }
                    }
                    barrier()
                    static_if (!KO_TENSOR) {
                        tmm2d_tg_step_deva(acc, unsafe(addr(xf[(rbase + mBase) * walk_kdim() + kb * 32u])), walk_kdim(), twb, MT, 64u, STAGE_K)
                    }
                    barrier()
                    kb += STAGE_K / 32u
                }
                static_if (!KO_TENSOR) {
                    static_if (STORE_HOOK) {
                        store_out(acc, cp, e, nBase)
                    } else {
                        tmm2d_tg_store(acc, cp, MT, 64u, walk_ndim())
                    }
                }
            }
        }
    }
}

[ |> template_struct_instance]
class template MetalKqMulMmSplitTensorBase : MetalMoeTileWalkT {
    @template_constant TWB : int = 6144     //!< W stage halves: one 64 wcols x 72-half padded tile (12288B lands mid-plateau); 9216 = the two ping-pong tiles `db_shell` runs
    @template_constant MOE : bool = false
    @template_constant DEQW : bool = false
    @ssbo @binding = 0 @role = "weight" @off = "s0off" kdh : array<float16>
    @ssbo @binding = 1 @role = "weight" @off = "soff" ksc : array<uint4>  //!< the SAME buffer as binding 0, uint4 view
    @ssbo @binding = 2 @role = "weight" @off = "qoff" kqu : array<uint>
    @ssbo @binding = 3 @off = "xoff" xf : array<XT> //!< raw activations (float, or the converted f16 panel)
    @ssbo @binding = 4 @off = "yoff" @span = "mp*rows*4" @template_gate = "!SK" y : array<float>
    @ssbo @binding = 4 @off = "yoff" @span = "nsk*mp*rows*4" @template_gate = SK yk : array<float>   //!< `nsk` partial planes of mp x rows
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
        static_if (!SK) {
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
    }

    //! the double-buffered walk over the tile's K - under `SK` the z-th share of it, stored into the z-th partial plane
    [arch(at="../ARCHITECTURE_IMAGE_GPU_PLANES.md#image-devw-plane")]
    def db_shell {
        let mBase = gl_WorkGroupID.x * MT
        let nBase = gl_WorkGroupID.y * 64u
        let nkb = kdim / 32u
        let nsb = kdim / 256u
        let kper = SK ? nkb / gl_NumWorkGroups.z : nkb
        let kbeg = SK ? gl_WorkGroupID.z * kper : 0u
        let kend = kbeg + kper
        var acc : float[8192]
        tmm2d_tg_begin_deva(acc, unsafe(addr(xf[0])), MT, 64u, 4u, 64u)
        stage_tile(gl_LocalInvocationID.x, kbeg, nBase, nsb, 0u)
        var kb = kbeg
        while (kb < kend) {
            barrier()
            let ph = ((kb - kbeg) / 2u) & 1u
            if (kb + 2u < kend) {
                stage_tile(gl_LocalInvocationID.x, kb + 2u, nBase, nsb, (ph ^ 1u) * 4608u)
            }
            tmm2d_tg_step_deva(acc, unsafe(addr(xf[mBase * kdim + kb * 32u])), kdim, unsafe(addr(twb[ph * 4608u])), MT, 64u, 64u)
            kb += 2u
        }
        static_if (SK) {
            var cp = unsafe(addr(yk[gl_WorkGroupID.z * gl_NumWorkGroups.x * MT * ndim + mBase * ndim + nBase]))
            tmm2d_tg_store(acc, cp, MT, 64u, ndim)
        } else {
            var cp = unsafe(addr(y[mBase * ndim + nBase]))
            tmm2d_tg_store(acc, cp, MT, 64u, ndim)
        }
    }

    def override walk_kdim : uint => kdim

    def override walk_ndim : uint => ndim

    //! the expert plane's base rides the stage16 call: a stage_tile argument is emitted once outside the work loop, which moves the decode's shape
    def override walk_stage(work, kb, e, nBase, nsb : uint) {
        stage16(work, kb, e * ndim + nBase, nsb, 0u)
    }

    def moe_kernel {
        static_if (MOE) {
            prologue()
            moe_walk(cnt[gl_WorkGroupID.z], basep[gl_WorkGroupID.z], REM)
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

    [metal_kernel(float_a_ok=true)]
    def metal_kq_mulmm_db {
        prologue()
        db_shell()
    }

    [metal_kernel]
    def metal_kq_dequant {
        prologue()
        dequant_shell()
    }
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

[metal_dispatch(stamp = "kq_mm:k4:t", kernel = "metal_kq_mulmm_t", pso = "g_pf_pso_kq_mm4_t")]
class MetalKqMulMmK4T : MetalKqMulMmK45TensorT {
    typedef XT = float
}

[metal_dispatch(stamp = "kq_mm:k4:th", kernel = "metal_kq_mulmm_t", pso = "g_pf_pso_kq_mm4_th")]
class MetalKqMulMmK4TH : MetalKqMulMmK45TensorT {
    typedef XT = float16
}

[metal_dispatch(stamp = "kq_mm:k5:t", kernel = "metal_kq_mulmm_t", pso = "g_pf_pso_kq_mm5_t")]
class MetalKqMulMmK5T : MetalKqMulMmK45TensorT {
    typedef XT = float
    override SB_UINTS = 40u
    override QH = true
}

[metal_dispatch(stamp = "kq_mm:k5:th", kernel = "metal_kq_mulmm_t", pso = "g_pf_pso_kq_mm5_th")]
class MetalKqMulMmK5TH : MetalKqMulMmK45TensorT {
    typedef XT = float16
    override SB_UINTS = 40u
    override QH = true
}

// tall 128-row stamps: in-kernel dequant with W read once per 128-row tile - the DRAM-class
// panel form (dev-W's materialized panel re-streams 2B/element; these read the quant plane)
[metal_dispatch(stamp = "kq_mm:k4:th128", kernel = "metal_kq_mulmm_t", pso = "g_pf_pso_kq_mm4_th128")]
class MetalKqMulMmK4TH128 : MetalKqMulMmK45TensorT {
    typedef XT = float16
    override MT = 128u
}

[metal_dispatch(stamp = "kq_mm:k5:th128", kernel = "metal_kq_mulmm_t", pso = "g_pf_pso_kq_mm5_th128")]
class MetalKqMulMmK5TH128 : MetalKqMulMmK45TensorT {
    typedef XT = float16
    override SB_UINTS = 40u
    override QH = true
    override MT = 128u
}

[metal_dispatch(kernel = "metal_kq_mulmm_db", stamp = "kq_mm:k4:thdb", pso = "g_pf_pso_kq_mm4_thdb")]
class MetalKqMulMmK4THDb : MetalKqMulMmK45TensorT {
    typedef XT = float16
    override TWB = 9216
}

[metal_dispatch(stamp = "kq_mm:k4:thsk", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm4_thsk", requires = "mp % 32, rows % 64")]
class MetalKqMulMmK4THSk : MetalKqMulMmK45TensorT {
    typedef XT = float16
    override TWB = 9216
    override SK = true
}

[metal_dispatch(stamp = "kq_mm:k4:tsk", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm4_tsk", requires = "mp % 32, rows % 64")]
class MetalKqMulMmK4TSk : MetalKqMulMmK45TensorT {
    typedef XT = float
    override TWB = 9216
    override SK = true
}

[metal_dispatch(stamp = "kq_mm:k4:thsk16", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm4_thsk16", requires = "rows % 64")]
class MetalKqMulMmK4THSk16 : MetalKqMulMmK45TensorT {
    typedef XT = float16
    override TWB = 9216
    override SK = true
    override MT = 16u
}

[metal_dispatch(stamp = "kq_mm:k4:tsk16", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm4_tsk16", requires = "rows % 64")]
class MetalKqMulMmK4TSk16 : MetalKqMulMmK45TensorT {
    typedef XT = float
    override TWB = 9216
    override SK = true
    override MT = 16u
}

[metal_dispatch(kernel = "metal_kq_mulmm_db", stamp = "kq_mm:k4:thdb128", pso = "g_pf_pso_kq_mm4_thdb128")]
class MetalKqMulMmK4THDb128 : MetalKqMulMmK45TensorT {
    typedef XT = float16
    override TWB = 9216
    override MT = 128u
}

[metal_dispatch(kernel = "metal_kq_mulmm_db", stamp = "kq_mm:k5:thdb", pso = "g_pf_pso_kq_mm5_thdb")]
class MetalKqMulMmK5THDb : MetalKqMulMmK45TensorT {
    typedef XT = float16
    override TWB = 9216
    override SB_UINTS = 40u
    override QH = true
}

[metal_dispatch(stamp = "kq_mm:k5:thsk", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm5_thsk", requires = "mp % 32, rows % 64")]
class MetalKqMulMmK5THSk : MetalKqMulMmK45TensorT {
    typedef XT = float16
    override TWB = 9216
    override SB_UINTS = 40u
    override QH = true
    override SK = true
}

[metal_dispatch(stamp = "kq_mm:k5:tsk", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm5_tsk", requires = "mp % 32, rows % 64")]
class MetalKqMulMmK5TSk : MetalKqMulMmK45TensorT {
    typedef XT = float
    override TWB = 9216
    override SB_UINTS = 40u
    override QH = true
    override SK = true
}

[metal_dispatch(stamp = "kq_mm:k5:thsk16", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm5_thsk16", requires = "rows % 64")]
class MetalKqMulMmK5THSk16 : MetalKqMulMmK45TensorT {
    typedef XT = float16
    override TWB = 9216
    override SB_UINTS = 40u
    override QH = true
    override SK = true
    override MT = 16u
}

[metal_dispatch(stamp = "kq_mm:k5:tsk16", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm5_tsk16", requires = "rows % 64")]
class MetalKqMulMmK5TSk16 : MetalKqMulMmK45TensorT {
    typedef XT = float
    override TWB = 9216
    override SB_UINTS = 40u
    override QH = true
    override SK = true
    override MT = 16u
}

[metal_dispatch(kernel = "metal_kq_mulmm_db", stamp = "kq_mm:k5:thdb128", pso = "g_pf_pso_kq_mm5_thdb128")]
class MetalKqMulMmK5THDb128 : MetalKqMulMmK45TensorT {
    typedef XT = float16
    override TWB = 9216
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

[metal_dispatch(stamp = "kq_mm:k6:t", kernel = "metal_kq_mulmm_k6_t", pso = "g_pf_pso_kq_mm6_t")]
class MetalKqMulMmK6T : MetalKqMulMmK6TensorT {
    typedef XT = float
}

[metal_dispatch(stamp = "kq_mm:k6:th", kernel = "metal_kq_mulmm_k6_t", pso = "g_pf_pso_kq_mm6_th")]
class MetalKqMulMmK6TH : MetalKqMulMmK6TensorT {
    typedef XT = float16
}

[metal_dispatch(stamp = "kq_mm:k6:th128", kernel = "metal_kq_mulmm_k6_t", pso = "g_pf_pso_kq_mm6_th128")]
class MetalKqMulMmK6TH128 : MetalKqMulMmK6TensorT {
    typedef XT = float16
    override MT = 128u
}

[metal_dispatch(kernel = "metal_kq_mulmm_db", stamp = "kq_mm:k6:thdb", pso = "g_pf_pso_kq_mm6_thdb")]
class MetalKqMulMmK6THDb : MetalKqMulMmK6TensorT {
    typedef XT = float16
    override TWB = 9216
}

[metal_dispatch(stamp = "kq_mm:k6:thsk", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm6_thsk", requires = "mp % 32, rows % 64")]
class MetalKqMulMmK6THSk : MetalKqMulMmK6TensorT {
    typedef XT = float16
    override TWB = 9216
    override SK = true
}

[metal_dispatch(stamp = "kq_mm:k6:tsk", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm6_tsk", requires = "mp % 32, rows % 64")]
class MetalKqMulMmK6TSk : MetalKqMulMmK6TensorT {
    typedef XT = float
    override TWB = 9216
    override SK = true
}

[metal_dispatch(stamp = "kq_mm:k6:thsk16", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm6_thsk16", requires = "rows % 64")]
class MetalKqMulMmK6THSk16 : MetalKqMulMmK6TensorT {
    typedef XT = float16
    override TWB = 9216
    override SK = true
    override MT = 16u
}

[metal_dispatch(stamp = "kq_mm:k6:tsk16", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm6_tsk16", requires = "rows % 64")]
class MetalKqMulMmK6TSk16 : MetalKqMulMmK6TensorT {
    typedef XT = float
    override TWB = 9216
    override SK = true
    override MT = 16u
}

[metal_dispatch(kernel = "metal_kq_mulmm_db", stamp = "kq_mm:k6:thdb128", pso = "g_pf_pso_kq_mm6_thdb128")]
class MetalKqMulMmK6THDb128 : MetalKqMulMmK6TensorT {
    typedef XT = float16
    override TWB = 9216
    override MT = 128u
}

[ |> template_struct_instance]
class template MetalKqMulMmIq4xsTensorT : MetalKqMulMmSplitTensorBase {
    @template_constant IQ4NL : bool = false
    @template_constant Q40 : bool = false   //!< q40: iq4nl's d plane, the linear nibble map (q - 8) in the codebook slot
    @template_constant IQODD : bool = false   //!< iq3s4: iq4xs's strip and d, the odd codebook (`iq3s4_val`) in the codebook slot
    @template_constant NV4 : bool = false   //!< nv4: the compact 16-byte UE4M3 row at soff (byte 2 js + half the half's scale), the doubled e2m1 codebook
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
            static_if (IQODD) {
                iq4lutt[gl_LocalInvocationID.x] = iq3s4_val(gl_LocalInvocationID.x)
            } else {
                static_if (NV4) {
                    iq4lutt[gl_LocalInvocationID.x] = mx4_val(gl_LocalInvocationID.x)
                } else {
                    iq4lutt[gl_LocalInvocationID.x] = iq4_lut(gl_LocalInvocationID.x)
                }
            }
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
        static_if (NV4) {
            dsc = nv4_sub_scale(ksc[blk], js * 2u + (sub & 1u))
        } else {
            static_if (IQ4NL) {
                dsc = float(kdh[blk * 8u + js])
            } else {
                let sv = ksc[blk]
                let scw = js < 4u ? sv.x : sv.y
                dsc = float(kdh[blk]) * float(((int(scw >> ((js & 3u) * 8u)) & 255) ^ 128) - 128)
            }
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

[metal_dispatch(stamp = "kq_mm:iq4xs:t", kernel = "metal_kq_mulmm_iq4xs_t", pso = "g_pf_pso_kq_mm44_t")]
class MetalKqMulMmIq4xsT : MetalKqMulMmIq4xsTensorT {
    typedef XT = float
}

[metal_dispatch(stamp = "kq_mm:iq4xs:th", kernel = "metal_kq_mulmm_iq4xs_t", pso = "g_pf_pso_kq_mm44_th")]
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

[metal_dispatch(stamp = "kq_mm:iq3s4:t", kernel = "metal_kq_mulmm_iq4xs_t", pso = "g_pf_pso_kq_mm35_t")]
class MetalKqMulMmIq3s4T : MetalKqMulMmIq4xsTensorT {
    typedef XT = float
    override IQODD = true
}

[metal_dispatch(stamp = "kq_mm:iq3s4:th", kernel = "metal_kq_mulmm_iq4xs_t", pso = "g_pf_pso_kq_mm35_th")]
class MetalKqMulMmIq3s4TH : MetalKqMulMmIq4xsTensorT {
    typedef XT = float16
    override IQODD = true
}

[metal_dispatch(stamp = "moe_mm_split:iq3s4:t")]
class MetalMoeMulMmIq3s4T : MetalMoeMulMmIq4xsSplitT {
    typedef XT = float
    override IQODD = true
}

[metal_dispatch(stamp = "moe_mm_split:iq3s4:th")]
class MetalMoeMulMmIq3s4TH : MetalMoeMulMmIq4xsSplitT {
    typedef XT = float16
    override IQODD = true
}

[metal_dispatch(stamp = "moe_mm_split:iq3s4:th128")]
class MetalMoeMulMmIq3s4TH128 : MetalMoeMulMmIq4xsSplitT {
    typedef XT = float16
    override MT = 128u
    override IQODD = true
}

[metal_dispatch(stamp = "moe_mm_split:iq3s4:thr")]
class MetalMoeMulMmIq3s4THR : MetalMoeMulMmIq4xsSplitT {
    typedef XT = float16
    override REM = true
    override IQODD = true
}

[metal_dispatch(stamp = "kq_mm:iq4nl:t", kernel = "metal_kq_mulmm_iq4xs_t", pso = "g_pf_pso_kq_mm45_t")]
class MetalKqMulMmIq4nlT : MetalKqMulMmIq4xsTensorT {
    typedef XT = float
    override IQ4NL = true
}

[metal_dispatch(stamp = "kq_mm:iq4nl:th", kernel = "metal_kq_mulmm_iq4xs_t", pso = "g_pf_pso_kq_mm45_th")]
class MetalKqMulMmIq4nlTH : MetalKqMulMmIq4xsTensorT {
    typedef XT = float16
    override IQ4NL = true
}

[metal_dispatch(stamp = "kq_mm:q40:t", kernel = "metal_kq_mulmm_iq4xs_t", pso = "g_pf_pso_kq_mm40_t", requires = "mp % 32, rows % 64")]
class MetalKqMulMmQ40T : MetalKqMulMmIq4xsTensorT {
    typedef XT = float
    override IQ4NL = true
    override Q40 = true
}

[metal_dispatch(stamp = "kq_mm:q40:th", kernel = "metal_kq_mulmm_iq4xs_t", pso = "g_pf_pso_kq_mm40_th")]
class MetalKqMulMmQ40TH : MetalKqMulMmIq4xsTensorT {
    typedef XT = float16
    override IQ4NL = true
    override Q40 = true
}

[metal_dispatch(stamp = "kq_mm:nv4:t", kernel = "metal_kq_mulmm_iq4xs_t", pso = "g_pf_pso_kq_mm49_t", requires = "mp % 32, rows % 64")]
class MetalKqMulMmNv4T : MetalKqMulMmIq4xsTensorT {
    typedef XT = float
    override NV4 = true
}

[metal_dispatch(stamp = "kq_mm:nv4:th", kernel = "metal_kq_mulmm_iq4xs_t", pso = "g_pf_pso_kq_mm49_th")]
class MetalKqMulMmNv4TH : MetalKqMulMmIq4xsTensorT {
    typedef XT = float16
    override NV4 = true
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

[metal_dispatch(stamp = "moe_mm_split:nv4:t")]
class MetalMoeMulMmNv4T : MetalMoeMulMmIq4xsSplitT {
    typedef XT = float
    override NV4 = true
}

[metal_dispatch(stamp = "moe_mm_split:nv4:th")]
class MetalMoeMulMmNv4TH : MetalMoeMulMmIq4xsSplitT {
    typedef XT = float16
    override NV4 = true
}

[metal_dispatch(stamp = "moe_mm_split:nv4:th128")]
class MetalMoeMulMmNv4TH128 : MetalMoeMulMmIq4xsSplitT {
    typedef XT = float16
    override NV4 = true
    override MT = 128u
}

[metal_dispatch(stamp = "moe_mm_split:nv4:thr")]
class MetalMoeMulMmNv4THR : MetalMoeMulMmIq4xsSplitT {
    typedef XT = float16
    override NV4 = true
    override REM = true
}

[metal_dispatch(stamp = "moe_mm_split:iq4nl:t")]
class MetalMoeMulMmIq4nlT : MetalMoeMulMmIq4xsSplitT {
    typedef XT = float
    override IQ4NL = true
}

[metal_dispatch(stamp = "moe_mm_split:iq4nl:th")]
class MetalMoeMulMmIq4nlTH : MetalMoeMulMmIq4xsSplitT {
    typedef XT = float16
    override IQ4NL = true
}

[metal_dispatch(stamp = "moe_mm_split:iq4nl:th128")]
class MetalMoeMulMmIq4nlTH128 : MetalMoeMulMmIq4xsSplitT {
    typedef XT = float16
    override IQ4NL = true
    override MT = 128u
}

[metal_dispatch(stamp = "moe_mm_split:iq4nl:thr")]
class MetalMoeMulMmIq4nlTHR : MetalMoeMulMmIq4xsSplitT {
    typedef XT = float16
    override IQ4NL = true
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
            twb[tb0 + uint(k) * 4u] = float16(mad(dsc, float(qv & 255u), -dmn))
            twb[tb0 + uint(k) * 4u + 1u] = float16(mad(dsc1, float(qv & 65280u), -dmn))
            twb[tb0 + uint(k) * 4u + 2u] = float16(mad(dsc2, float(qv & 16711680u), -dmn))
            twb[tb0 + uint(k) * 4u + 3u] = float16(mad(dsc3, float(qv & 4278190080u), -dmn))
        }
    }
}

[metal_dispatch(stamp = "kq_mm:k3:t", kernel = "metal_kq_mulmm_k3_t", pso = "g_pf_pso_kq_mm3_t")]
class MetalKqMulMmK3T : MetalKqMulMmK3TensorT {
    typedef XT = float
}

[metal_dispatch(stamp = "kq_mm:k3:th", kernel = "metal_kq_mulmm_k3_t", pso = "g_pf_pso_kq_mm3_th")]
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
            twb[tb0 + uint(k) * 4u] = float16(mad(dsc, float(qv & 255u), -dmn))
            twb[tb0 + uint(k) * 4u + 1u] = float16(mad(dsc1, float(qv & 65280u), -dmn))
            twb[tb0 + uint(k) * 4u + 2u] = float16(mad(dsc2, float(qv & 16711680u), -dmn))
            twb[tb0 + uint(k) * 4u + 3u] = float16(mad(dsc3, float(qv & 4278190080u), -dmn))
        }
    }
}

[metal_dispatch(stamp = "kq_mm:k2:t", kernel = "metal_kq_mulmm_k2_t", pso = "g_pf_pso_kq_mm2_t")]
class MetalKqMulMmK2T : MetalKqMulMmK2TensorT {
    typedef XT = float
}

[metal_dispatch(stamp = "kq_mm:k2:th", kernel = "metal_kq_mulmm_k2_t", pso = "g_pf_pso_kq_mm2_th")]
class MetalKqMulMmK2TH : MetalKqMulMmK2TensorT {
    typedef XT = float16
}

[ |> template_struct_instance]
class template MetalMoeMulMmK2SplitT : MetalKqMulMmK2TensorT {
    override MOE = true

    [metal_kernel(float_a_ok=true)]
    def metal_moe_mulmm_k2_t {
        moe_kernel()
    }
}
[metal_dispatch(stamp = "moe_mm_split:k2:t")]
class MetalMoeMulMmK2T : MetalMoeMulMmK2SplitT {
    typedef XT = float
}

[metal_dispatch(stamp = "moe_mm_split:k2:th")]
class MetalMoeMulMmK2TH : MetalMoeMulMmK2SplitT {
    typedef XT = float16
}

[metal_dispatch(stamp = "moe_mm_split:k2:th128")]
class MetalMoeMulMmK2TH128 : MetalMoeMulMmK2SplitT {
    typedef XT = float16
    override MT = 128u
}

[metal_dispatch(stamp = "moe_mm_split:k2:thr")]
class MetalMoeMulMmK2THR : MetalMoeMulMmK2SplitT {
    typedef XT = float16
    override REM = true
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

[metal_dispatch(stamp = "kq_mm:iq3s:t", kernel = "metal_kq_mulmm_iq3s_t", pso = "g_pf_pso_kq_mm33_t")]
class MetalKqMulMmIq3sT : MetalKqMulMmIq3sTensorT {
    typedef XT = float
}

[metal_dispatch(stamp = "kq_mm:iq3s:th", kernel = "metal_kq_mulmm_iq3s_t", pso = "g_pf_pso_kq_mm33_th")]
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

[metal_dispatch(stamp = "kq_mm:iq3s:th128", kernel = "metal_kq_mulmm_iq3s_t", pso = "g_pf_pso_kq_mm33_th128", requires = "mp % 128, rows % 64")]
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

[metal_dispatch(stamp = "kq_mm:iq3xxs:t", kernel = "metal_kq_mulmm_iq3xxs_t", pso = "g_pf_pso_kq_mm34_t")]
class MetalKqMulMmIq3xxsT : MetalKqMulMmIq3xxsTensorT {
    typedef XT = float
}

[metal_dispatch(stamp = "kq_mm:iq3xxs:th", kernel = "metal_kq_mulmm_iq3xxs_t", pso = "g_pf_pso_kq_mm34_th")]
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

[metal_dispatch(stamp = "kq_mm:iq3xxs:th128", kernel = "metal_kq_mulmm_iq3xxs_t", pso = "g_pf_pso_kq_mm34_th128")]
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

[metal_dispatch(stamp = "kq_mm:iq2s:t", kernel = "metal_kq_mulmm_iq2s_t", pso = "g_pf_pso_kq_mm23_t")]
class MetalKqMulMmIq2sT : MetalKqMulMmIq2sTensorT {
    typedef XT = float
}

[metal_dispatch(stamp = "kq_mm:iq2s:th", kernel = "metal_kq_mulmm_iq2s_t", pso = "g_pf_pso_kq_mm23_th")]
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

[metal_dispatch(stamp = "kq_mm:iq2xs:t", kernel = "metal_kq_mulmm_iq2xs_t", pso = "g_pf_pso_kq_mm24_t")]
class MetalKqMulMmIq2xsT : MetalKqMulMmIq2xsTensorT {
    typedef XT = float
}

[metal_dispatch(stamp = "kq_mm:iq2xs:th", kernel = "metal_kq_mulmm_iq2xs_t", pso = "g_pf_pso_kq_mm24_th")]
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

[metal_dispatch(stamp = "kq_mm:iq2xxs:t", kernel = "metal_kq_mulmm_iq2xxs_t", pso = "g_pf_pso_kq_mm25_t")]
class MetalKqMulMmIq2xxsT : MetalKqMulMmIq2xxsTensorT {
    typedef XT = float
}

[metal_dispatch(stamp = "kq_mm:iq2xxs:th", kernel = "metal_kq_mulmm_iq2xxs_t", pso = "g_pf_pso_kq_mm25_th")]
class MetalKqMulMmIq2xxsTH : MetalKqMulMmIq2xxsTensorT {
    typedef XT = float16
}

[metal_dispatch(stamp = "kq_mm:iq4xs:th128", kernel = "metal_kq_mulmm_iq4xs_t", pso = "g_pf_pso_kq_mm44_th128", requires = "mp % 128, rows % 64")]
class MetalKqMulMmIq4xsTH128 : MetalKqMulMmIq4xsTensorT {
    typedef XT = float16
    override MT = 128u
}

[metal_dispatch(stamp = "kq_mm:iq3s4:th128", kernel = "metal_kq_mulmm_iq4xs_t", pso = "g_pf_pso_kq_mm35_th128", requires = "mp % 128, rows % 64")]
class MetalKqMulMmIq3s4TH128 : MetalKqMulMmIq4xsTensorT {
    typedef XT = float16
    override IQODD = true
    override MT = 128u
}

[metal_dispatch(stamp = "kq_mm:iq4nl:th128", kernel = "metal_kq_mulmm_iq4xs_t", pso = "g_pf_pso_kq_mm45_th128", requires = "mp % 128, rows % 64")]
class MetalKqMulMmIq4nlTH128 : MetalKqMulMmIq4xsTensorT {
    typedef XT = float16
    override IQ4NL = true
    override MT = 128u
}

[metal_dispatch(stamp = "kq_mm:q40:th128", kernel = "metal_kq_mulmm_iq4xs_t", pso = "g_pf_pso_kq_mm40_th128", requires = "mp % 128, rows % 64")]
class MetalKqMulMmQ40TH128 : MetalKqMulMmIq4xsTensorT {
    typedef XT = float16
    override IQ4NL = true
    override Q40 = true
    override MT = 128u
}

[metal_dispatch(stamp = "kq_mm:nv4:th128", kernel = "metal_kq_mulmm_iq4xs_t", pso = "g_pf_pso_kq_mm49_th128", requires = "mp % 128, rows % 64")]
class MetalKqMulMmNv4TH128 : MetalKqMulMmIq4xsTensorT {
    typedef XT = float16
    override NV4 = true
    override MT = 128u
}

[metal_dispatch(stamp = "kq_mm:k3:th128", kernel = "metal_kq_mulmm_k3_t", pso = "g_pf_pso_kq_mm3_th128", requires = "mp % 128, rows % 64")]
class MetalKqMulMmK3TH128 : MetalKqMulMmK3TensorT {
    typedef XT = float16
    override MT = 128u
}

[metal_dispatch(stamp = "kq_mm:k2:th128", kernel = "metal_kq_mulmm_k2_t", pso = "g_pf_pso_kq_mm2_th128", requires = "mp % 128, rows % 64")]
class MetalKqMulMmK2TH128 : MetalKqMulMmK2TensorT {
    typedef XT = float16
    override MT = 128u
}

[metal_dispatch(stamp = "kq_mm:iq2s:th128", kernel = "metal_kq_mulmm_iq2s_t", pso = "g_pf_pso_kq_mm23_th128", requires = "mp % 128, rows % 64")]
class MetalKqMulMmIq2sTH128 : MetalKqMulMmIq2sTensorT {
    typedef XT = float16
    override MT = 128u
}

[metal_dispatch(stamp = "kq_mm:iq2xs:th128", kernel = "metal_kq_mulmm_iq2xs_t", pso = "g_pf_pso_kq_mm24_th128", requires = "mp % 128, rows % 64")]
class MetalKqMulMmIq2xsTH128 : MetalKqMulMmIq2xsTensorT {
    typedef XT = float16
    override MT = 128u
}

[metal_dispatch(stamp = "kq_mm:iq2xxs:th128", kernel = "metal_kq_mulmm_iq2xxs_t", pso = "g_pf_pso_kq_mm25_th128", requires = "mp % 128, rows % 64")]
class MetalKqMulMmIq2xxsTH128 : MetalKqMulMmIq2xxsTensorT {
    typedef XT = float16
    override MT = 128u
}

[metal_dispatch(stamp = "kq_mm:iq4xs:thdb", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm44_thdb", requires = "mp % 32, rows % 64")]
class MetalKqMulMmIq4xsTHDb : MetalKqMulMmIq4xsTensorT {
    typedef XT = float16
    override TWB = 9216
}

[metal_dispatch(stamp = "kq_mm:iq4xs:thsk", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm44_thsk", requires = "mp % 32, rows % 64")]
class MetalKqMulMmIq4xsTHSk : MetalKqMulMmIq4xsTensorT {
    typedef XT = float16
    override TWB = 9216
    override SK = true
}

[metal_dispatch(stamp = "kq_mm:iq4xs:tsk", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm44_tsk", requires = "mp % 32, rows % 64")]
class MetalKqMulMmIq4xsTSk : MetalKqMulMmIq4xsTensorT {
    typedef XT = float
    override TWB = 9216
    override SK = true
}

[metal_dispatch(stamp = "kq_mm:iq4xs:thsk16", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm44_thsk16", requires = "rows % 64")]
class MetalKqMulMmIq4xsTHSk16 : MetalKqMulMmIq4xsTensorT {
    typedef XT = float16
    override TWB = 9216
    override SK = true
    override MT = 16u
}

[metal_dispatch(stamp = "kq_mm:iq4xs:tsk16", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm44_tsk16", requires = "rows % 64")]
class MetalKqMulMmIq4xsTSk16 : MetalKqMulMmIq4xsTensorT {
    typedef XT = float
    override TWB = 9216
    override SK = true
    override MT = 16u
}

[metal_dispatch(stamp = "kq_mm:iq4xs:thdb128", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm44_thdb128", requires = "mp % 128, rows % 64")]
class MetalKqMulMmIq4xsTHDb128 : MetalKqMulMmIq4xsTensorT {
    typedef XT = float16
    override TWB = 9216
    override MT = 128u
}

[metal_dispatch(stamp = "kq_mm:iq3s4:thdb", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm35_thdb", requires = "mp % 32, rows % 64")]
class MetalKqMulMmIq3s4THDb : MetalKqMulMmIq4xsTensorT {
    typedef XT = float16
    override TWB = 9216
    override IQODD = true
}

[metal_dispatch(stamp = "kq_mm:iq3s4:thsk", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm35_thsk", requires = "mp % 32, rows % 64")]
class MetalKqMulMmIq3s4THSk : MetalKqMulMmIq4xsTensorT {
    typedef XT = float16
    override TWB = 9216
    override IQODD = true
    override SK = true
}

[metal_dispatch(stamp = "kq_mm:iq3s4:tsk", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm35_tsk", requires = "mp % 32, rows % 64")]
class MetalKqMulMmIq3s4TSk : MetalKqMulMmIq4xsTensorT {
    typedef XT = float
    override TWB = 9216
    override IQODD = true
    override SK = true
}

[metal_dispatch(stamp = "kq_mm:iq3s4:thsk16", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm35_thsk16", requires = "rows % 64")]
class MetalKqMulMmIq3s4THSk16 : MetalKqMulMmIq4xsTensorT {
    typedef XT = float16
    override TWB = 9216
    override IQODD = true
    override SK = true
    override MT = 16u
}

[metal_dispatch(stamp = "kq_mm:iq3s4:tsk16", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm35_tsk16", requires = "rows % 64")]
class MetalKqMulMmIq3s4TSk16 : MetalKqMulMmIq4xsTensorT {
    typedef XT = float
    override TWB = 9216
    override IQODD = true
    override SK = true
    override MT = 16u
}

[metal_dispatch(stamp = "kq_mm:iq3s4:thdb128", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm35_thdb128", requires = "mp % 128, rows % 64")]
class MetalKqMulMmIq3s4THDb128 : MetalKqMulMmIq4xsTensorT {
    typedef XT = float16
    override TWB = 9216
    override IQODD = true
    override MT = 128u
}

[metal_dispatch(stamp = "kq_mm:iq4nl:thdb", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm45_thdb", requires = "mp % 32, rows % 64")]
class MetalKqMulMmIq4nlTHDb : MetalKqMulMmIq4xsTensorT {
    typedef XT = float16
    override TWB = 9216
    override IQ4NL = true
}

[metal_dispatch(stamp = "kq_mm:iq4nl:thsk", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm45_thsk", requires = "mp % 32, rows % 64")]
class MetalKqMulMmIq4nlTHSk : MetalKqMulMmIq4xsTensorT {
    typedef XT = float16
    override TWB = 9216
    override IQ4NL = true
    override SK = true
}

[metal_dispatch(stamp = "kq_mm:iq4nl:tsk", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm45_tsk", requires = "mp % 32, rows % 64")]
class MetalKqMulMmIq4nlTSk : MetalKqMulMmIq4xsTensorT {
    typedef XT = float
    override TWB = 9216
    override IQ4NL = true
    override SK = true
}

[metal_dispatch(stamp = "kq_mm:iq4nl:thsk16", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm45_thsk16", requires = "rows % 64")]
class MetalKqMulMmIq4nlTHSk16 : MetalKqMulMmIq4xsTensorT {
    typedef XT = float16
    override TWB = 9216
    override IQ4NL = true
    override SK = true
    override MT = 16u
}

[metal_dispatch(stamp = "kq_mm:iq4nl:tsk16", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm45_tsk16", requires = "rows % 64")]
class MetalKqMulMmIq4nlTSk16 : MetalKqMulMmIq4xsTensorT {
    typedef XT = float
    override TWB = 9216
    override IQ4NL = true
    override SK = true
    override MT = 16u
}

[metal_dispatch(stamp = "kq_mm:iq4nl:thdb128", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm45_thdb128", requires = "mp % 128, rows % 64")]
class MetalKqMulMmIq4nlTHDb128 : MetalKqMulMmIq4xsTensorT {
    typedef XT = float16
    override TWB = 9216
    override IQ4NL = true
    override MT = 128u
}

[metal_dispatch(stamp = "kq_mm:q40:thdb", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm40_thdb", requires = "mp % 32, rows % 64")]
class MetalKqMulMmQ40THDb : MetalKqMulMmIq4xsTensorT {
    typedef XT = float16
    override TWB = 9216
    override IQ4NL = true
    override Q40 = true
}

[metal_dispatch(stamp = "kq_mm:q40:thsk", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm40_thsk", requires = "mp % 32, rows % 64")]
class MetalKqMulMmQ40THSk : MetalKqMulMmIq4xsTensorT {
    typedef XT = float16
    override TWB = 9216
    override IQ4NL = true
    override Q40 = true
    override SK = true
}

[metal_dispatch(stamp = "kq_mm:q40:tsk", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm40_tsk", requires = "mp % 32, rows % 64")]
class MetalKqMulMmQ40TSk : MetalKqMulMmIq4xsTensorT {
    typedef XT = float
    override TWB = 9216
    override IQ4NL = true
    override Q40 = true
    override SK = true
}

[metal_dispatch(stamp = "kq_mm:q40:thsk16", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm40_thsk16", requires = "rows % 64")]
class MetalKqMulMmQ40THSk16 : MetalKqMulMmIq4xsTensorT {
    typedef XT = float16
    override TWB = 9216
    override IQ4NL = true
    override Q40 = true
    override SK = true
    override MT = 16u
}

[metal_dispatch(stamp = "kq_mm:q40:tsk16", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm40_tsk16", requires = "rows % 64")]
class MetalKqMulMmQ40TSk16 : MetalKqMulMmIq4xsTensorT {
    typedef XT = float
    override TWB = 9216
    override IQ4NL = true
    override Q40 = true
    override SK = true
    override MT = 16u
}

[metal_dispatch(stamp = "kq_mm:q40:thdb128", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm40_thdb128", requires = "mp % 128, rows % 64")]
class MetalKqMulMmQ40THDb128 : MetalKqMulMmIq4xsTensorT {
    typedef XT = float16
    override TWB = 9216
    override IQ4NL = true
    override Q40 = true
    override MT = 128u
}

[metal_dispatch(stamp = "kq_mm:nv4:thdb", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm49_thdb", requires = "mp % 32, rows % 64")]
class MetalKqMulMmNv4THDb : MetalKqMulMmIq4xsTensorT {
    typedef XT = float16
    override TWB = 9216
    override NV4 = true
}

[metal_dispatch(stamp = "kq_mm:nv4:thsk", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm49_thsk", requires = "mp % 32, rows % 64")]
class MetalKqMulMmNv4THSk : MetalKqMulMmIq4xsTensorT {
    typedef XT = float16
    override TWB = 9216
    override NV4 = true
    override SK = true
}

[metal_dispatch(stamp = "kq_mm:nv4:tsk", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm49_tsk", requires = "mp % 32, rows % 64")]
class MetalKqMulMmNv4TSk : MetalKqMulMmIq4xsTensorT {
    typedef XT = float
    override TWB = 9216
    override NV4 = true
    override SK = true
}

[metal_dispatch(stamp = "kq_mm:nv4:thsk16", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm49_thsk16", requires = "rows % 64")]
class MetalKqMulMmNv4THSk16 : MetalKqMulMmIq4xsTensorT {
    typedef XT = float16
    override TWB = 9216
    override NV4 = true
    override SK = true
    override MT = 16u
}

[metal_dispatch(stamp = "kq_mm:nv4:tsk16", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm49_tsk16", requires = "rows % 64")]
class MetalKqMulMmNv4TSk16 : MetalKqMulMmIq4xsTensorT {
    typedef XT = float
    override TWB = 9216
    override NV4 = true
    override SK = true
    override MT = 16u
}

[metal_dispatch(stamp = "kq_mm:nv4:thdb128", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm49_thdb128", requires = "mp % 128, rows % 64")]
class MetalKqMulMmNv4THDb128 : MetalKqMulMmIq4xsTensorT {
    typedef XT = float16
    override TWB = 9216
    override NV4 = true
    override MT = 128u
}

[metal_dispatch(stamp = "kq_mm:k3:thdb", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm3_thdb", requires = "mp % 32, rows % 64")]
class MetalKqMulMmK3THDb : MetalKqMulMmK3TensorT {
    typedef XT = float16
    override TWB = 9216
}

[metal_dispatch(stamp = "kq_mm:k3:thsk", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm3_thsk", requires = "mp % 32, rows % 64")]
class MetalKqMulMmK3THSk : MetalKqMulMmK3TensorT {
    typedef XT = float16
    override TWB = 9216
    override SK = true
}

[metal_dispatch(stamp = "kq_mm:k3:tsk", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm3_tsk", requires = "mp % 32, rows % 64")]
class MetalKqMulMmK3TSk : MetalKqMulMmK3TensorT {
    typedef XT = float
    override TWB = 9216
    override SK = true
}

[metal_dispatch(stamp = "kq_mm:k3:thsk16", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm3_thsk16", requires = "rows % 64")]
class MetalKqMulMmK3THSk16 : MetalKqMulMmK3TensorT {
    typedef XT = float16
    override TWB = 9216
    override SK = true
    override MT = 16u
}

[metal_dispatch(stamp = "kq_mm:k3:tsk16", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm3_tsk16", requires = "rows % 64")]
class MetalKqMulMmK3TSk16 : MetalKqMulMmK3TensorT {
    typedef XT = float
    override TWB = 9216
    override SK = true
    override MT = 16u
}

[metal_dispatch(stamp = "kq_mm:k3:thdb128", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm3_thdb128", requires = "mp % 128, rows % 64")]
class MetalKqMulMmK3THDb128 : MetalKqMulMmK3TensorT {
    typedef XT = float16
    override TWB = 9216
    override MT = 128u
}

[metal_dispatch(stamp = "kq_mm:k2:thdb", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm2_thdb", requires = "mp % 32, rows % 64")]
class MetalKqMulMmK2THDb : MetalKqMulMmK2TensorT {
    typedef XT = float16
    override TWB = 9216
}

[metal_dispatch(stamp = "kq_mm:k2:thsk", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm2_thsk", requires = "mp % 32, rows % 64")]
class MetalKqMulMmK2THSk : MetalKqMulMmK2TensorT {
    typedef XT = float16
    override TWB = 9216
    override SK = true
}

[metal_dispatch(stamp = "kq_mm:k2:tsk", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm2_tsk", requires = "mp % 32, rows % 64")]
class MetalKqMulMmK2TSk : MetalKqMulMmK2TensorT {
    typedef XT = float
    override TWB = 9216
    override SK = true
}

[metal_dispatch(stamp = "kq_mm:k2:thsk16", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm2_thsk16", requires = "rows % 64")]
class MetalKqMulMmK2THSk16 : MetalKqMulMmK2TensorT {
    typedef XT = float16
    override TWB = 9216
    override SK = true
    override MT = 16u
}

[metal_dispatch(stamp = "kq_mm:k2:tsk16", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm2_tsk16", requires = "rows % 64")]
class MetalKqMulMmK2TSk16 : MetalKqMulMmK2TensorT {
    typedef XT = float
    override TWB = 9216
    override SK = true
    override MT = 16u
}

[metal_dispatch(stamp = "kq_mm:k2:thdb128", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm2_thdb128", requires = "mp % 128, rows % 64")]
class MetalKqMulMmK2THDb128 : MetalKqMulMmK2TensorT {
    typedef XT = float16
    override TWB = 9216
    override MT = 128u
}

[metal_dispatch(stamp = "kq_mm:iq3s:thdb", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm33_thdb", requires = "mp % 32, rows % 64")]
class MetalKqMulMmIq3sTHDb : MetalKqMulMmIq3sTensorT {
    typedef XT = float16
    override TWB = 9216
}

[metal_dispatch(stamp = "kq_mm:iq3s:thsk", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm33_thsk", requires = "mp % 32, rows % 64")]
class MetalKqMulMmIq3sTHSk : MetalKqMulMmIq3sTensorT {
    typedef XT = float16
    override TWB = 9216
    override SK = true
}

[metal_dispatch(stamp = "kq_mm:iq3s:tsk", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm33_tsk", requires = "mp % 32, rows % 64")]
class MetalKqMulMmIq3sTSk : MetalKqMulMmIq3sTensorT {
    typedef XT = float
    override TWB = 9216
    override SK = true
}

[metal_dispatch(stamp = "kq_mm:iq3s:thsk16", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm33_thsk16", requires = "rows % 64")]
class MetalKqMulMmIq3sTHSk16 : MetalKqMulMmIq3sTensorT {
    typedef XT = float16
    override TWB = 9216
    override SK = true
    override MT = 16u
}

[metal_dispatch(stamp = "kq_mm:iq3s:tsk16", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm33_tsk16", requires = "rows % 64")]
class MetalKqMulMmIq3sTSk16 : MetalKqMulMmIq3sTensorT {
    typedef XT = float
    override TWB = 9216
    override SK = true
    override MT = 16u
}

[metal_dispatch(stamp = "kq_mm:iq3s:thdb128", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm33_thdb128", requires = "mp % 128, rows % 64")]
class MetalKqMulMmIq3sTHDb128 : MetalKqMulMmIq3sTensorT {
    typedef XT = float16
    override TWB = 9216
    override MT = 128u
}

[metal_dispatch(stamp = "kq_mm:iq3xxs:thdb", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm34_thdb", requires = "mp % 32, rows % 64")]
class MetalKqMulMmIq3xxsTHDb : MetalKqMulMmIq3xxsTensorT {
    typedef XT = float16
    override TWB = 9216
}

[metal_dispatch(stamp = "kq_mm:iq3xxs:thsk", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm34_thsk", requires = "mp % 32, rows % 64")]
class MetalKqMulMmIq3xxsTHSk : MetalKqMulMmIq3xxsTensorT {
    typedef XT = float16
    override TWB = 9216
    override SK = true
}

[metal_dispatch(stamp = "kq_mm:iq3xxs:tsk", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm34_tsk", requires = "mp % 32, rows % 64")]
class MetalKqMulMmIq3xxsTSk : MetalKqMulMmIq3xxsTensorT {
    typedef XT = float
    override TWB = 9216
    override SK = true
}

[metal_dispatch(stamp = "kq_mm:iq3xxs:thsk16", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm34_thsk16", requires = "rows % 64")]
class MetalKqMulMmIq3xxsTHSk16 : MetalKqMulMmIq3xxsTensorT {
    typedef XT = float16
    override TWB = 9216
    override SK = true
    override MT = 16u
}

[metal_dispatch(stamp = "kq_mm:iq3xxs:tsk16", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm34_tsk16", requires = "rows % 64")]
class MetalKqMulMmIq3xxsTSk16 : MetalKqMulMmIq3xxsTensorT {
    typedef XT = float
    override TWB = 9216
    override SK = true
    override MT = 16u
}

[metal_dispatch(stamp = "kq_mm:iq3xxs:thdb128", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm34_thdb128", requires = "mp % 128, rows % 64")]
class MetalKqMulMmIq3xxsTHDb128 : MetalKqMulMmIq3xxsTensorT {
    typedef XT = float16
    override TWB = 9216
    override MT = 128u
}

[metal_dispatch(stamp = "kq_mm:iq2s:thdb", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm23_thdb", requires = "mp % 32, rows % 64")]
class MetalKqMulMmIq2sTHDb : MetalKqMulMmIq2sTensorT {
    typedef XT = float16
    override TWB = 9216
}

[metal_dispatch(stamp = "kq_mm:iq2s:thsk", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm23_thsk", requires = "mp % 32, rows % 64")]
class MetalKqMulMmIq2sTHSk : MetalKqMulMmIq2sTensorT {
    typedef XT = float16
    override TWB = 9216
    override SK = true
}

[metal_dispatch(stamp = "kq_mm:iq2s:tsk", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm23_tsk", requires = "mp % 32, rows % 64")]
class MetalKqMulMmIq2sTSk : MetalKqMulMmIq2sTensorT {
    typedef XT = float
    override TWB = 9216
    override SK = true
}

[metal_dispatch(stamp = "kq_mm:iq2s:thsk16", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm23_thsk16", requires = "rows % 64")]
class MetalKqMulMmIq2sTHSk16 : MetalKqMulMmIq2sTensorT {
    typedef XT = float16
    override TWB = 9216
    override SK = true
    override MT = 16u
}

[metal_dispatch(stamp = "kq_mm:iq2s:tsk16", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm23_tsk16", requires = "rows % 64")]
class MetalKqMulMmIq2sTSk16 : MetalKqMulMmIq2sTensorT {
    typedef XT = float
    override TWB = 9216
    override SK = true
    override MT = 16u
}

[metal_dispatch(stamp = "kq_mm:iq2s:thdb128", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm23_thdb128", requires = "mp % 128, rows % 64")]
class MetalKqMulMmIq2sTHDb128 : MetalKqMulMmIq2sTensorT {
    typedef XT = float16
    override TWB = 9216
    override MT = 128u
}

[metal_dispatch(stamp = "kq_mm:iq2xs:thdb", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm24_thdb", requires = "mp % 32, rows % 64")]
class MetalKqMulMmIq2xsTHDb : MetalKqMulMmIq2xsTensorT {
    typedef XT = float16
    override TWB = 9216
}

[metal_dispatch(stamp = "kq_mm:iq2xs:thsk", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm24_thsk", requires = "mp % 32, rows % 64")]
class MetalKqMulMmIq2xsTHSk : MetalKqMulMmIq2xsTensorT {
    typedef XT = float16
    override TWB = 9216
    override SK = true
}

[metal_dispatch(stamp = "kq_mm:iq2xs:tsk", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm24_tsk", requires = "mp % 32, rows % 64")]
class MetalKqMulMmIq2xsTSk : MetalKqMulMmIq2xsTensorT {
    typedef XT = float
    override TWB = 9216
    override SK = true
}

[metal_dispatch(stamp = "kq_mm:iq2xs:thsk16", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm24_thsk16", requires = "rows % 64")]
class MetalKqMulMmIq2xsTHSk16 : MetalKqMulMmIq2xsTensorT {
    typedef XT = float16
    override TWB = 9216
    override SK = true
    override MT = 16u
}

[metal_dispatch(stamp = "kq_mm:iq2xs:tsk16", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm24_tsk16", requires = "rows % 64")]
class MetalKqMulMmIq2xsTSk16 : MetalKqMulMmIq2xsTensorT {
    typedef XT = float
    override TWB = 9216
    override SK = true
    override MT = 16u
}

[metal_dispatch(stamp = "kq_mm:iq2xs:thdb128", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm24_thdb128", requires = "mp % 128, rows % 64")]
class MetalKqMulMmIq2xsTHDb128 : MetalKqMulMmIq2xsTensorT {
    typedef XT = float16
    override TWB = 9216
    override MT = 128u
}

[metal_dispatch(stamp = "kq_mm:iq2xxs:thdb", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm25_thdb", requires = "mp % 32, rows % 64")]
class MetalKqMulMmIq2xxsTHDb : MetalKqMulMmIq2xxsTensorT {
    typedef XT = float16
    override TWB = 9216
}

[metal_dispatch(stamp = "kq_mm:iq2xxs:thsk", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm25_thsk", requires = "mp % 32, rows % 64")]
class MetalKqMulMmIq2xxsTHSk : MetalKqMulMmIq2xxsTensorT {
    typedef XT = float16
    override TWB = 9216
    override SK = true
}

[metal_dispatch(stamp = "kq_mm:iq2xxs:tsk", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm25_tsk", requires = "mp % 32, rows % 64")]
class MetalKqMulMmIq2xxsTSk : MetalKqMulMmIq2xxsTensorT {
    typedef XT = float
    override TWB = 9216
    override SK = true
}

[metal_dispatch(stamp = "kq_mm:iq2xxs:thsk16", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm25_thsk16", requires = "rows % 64")]
class MetalKqMulMmIq2xxsTHSk16 : MetalKqMulMmIq2xxsTensorT {
    typedef XT = float16
    override TWB = 9216
    override SK = true
    override MT = 16u
}

[metal_dispatch(stamp = "kq_mm:iq2xxs:tsk16", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm25_tsk16", requires = "rows % 64")]
class MetalKqMulMmIq2xxsTSk16 : MetalKqMulMmIq2xxsTensorT {
    typedef XT = float
    override TWB = 9216
    override SK = true
    override MT = 16u
}

[metal_dispatch(stamp = "kq_mm:iq2xxs:thdb128", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm25_thdb128", requires = "mp % 128, rows % 64")]
class MetalKqMulMmIq2xxsTHDb128 : MetalKqMulMmIq2xxsTensorT {
    typedef XT = float16
    override TWB = 9216
    override MT = 128u
}

//! the per-32 formats' dense tensor twins on the split scaffold: a work item decodes 16 of one 32-block - `B32W` quant words and
//! `B32S` scale halves a block, the block index off `kdim` since a per-32 plane has no superblock
[ |> template_struct_instance]
class template MetalKqMulMmB32TensorT : MetalKqMulMmSplitTensorBase {
    @template_constant Q51 : bool = false   //!< q5_1: d * q + m, the fifth bit off the block's last word
    @template_constant Q50 : bool = false   //!< q5032: q5_1's block under one d, the value d * (q - 16)
    @template_constant Q40L : bool = false  //!< q4032: the linear code q - 8 in the codebook slot
    @template_constant B32W : uint = 4u     //!< quant-plane words a block: 5 on the five-bit blocks (the qh word last)
    @template_constant B32S : uint = 1u     //!< scale halves a block: 2 on q5_1 (d, then m)
    @template_constant B32E : uint = 32u    //!< a block's elements: KQ_B32_ELEMS

    [metal_kernel(float_a_ok=true)]
    def metal_kq_mulmm_b32_t {
        run_shell()
    }

    def override stage16(work, kb, nBase, nsb, tbase : uint) {
        let j = work >> 2u
        let sub = work & 3u
        let kh = (sub & 1u) * 16u
        let blk = (nBase + j) * (kdim / B32E) + kb + (sub >> 1u)
        let nsh = (sub & 1u) * 4u
        let qp = unsafe(addr(kqu[blk * B32W]))
        let tb0 = tbase + j * 72u + (sub >> 1u) * 32u + kh
        let d = float(kdh[blk * B32S])
        static_if (Q51) {
            let m = float(kdh[blk * B32S + 1u])
            let qh = unsafe(qp[B32W - 1u])
            for [unroll_full] (t in range(16)) {
                let u = unsafe(qp[t / 4])
                let q = ((u >> (8u * uint(t % 4) + nsh)) & 15u) | (((qh >> (kh + uint(t))) & 1u) << 4u)
                twb[tb0 + uint(t)] = float16(d * float(q) + m)
            }
        } else {
            static_if (Q50) {
                let qh = unsafe(qp[B32W - 1u])
                for [unroll_full] (t in range(16)) {
                    let u = unsafe(qp[t / 4])
                    let q = ((u >> (8u * uint(t % 4) + nsh)) & 15u) | (((qh >> (kh + uint(t))) & 1u) << 4u)
                    twb[tb0 + uint(t)] = float16(d * float(int(q) - 16))
                }
            } else {
                for [unroll_full] (t in range(16)) {
                    let u = unsafe(qp[t / 4])
                    let q = (u >> (8u * uint(t % 4) + nsh)) & 15u
                    twb[tb0 + uint(t)] = float16(d * b32_code_val(q, Q40L))
                }
            }
        }
    }
}

[ |> template_struct_instance]
class template MetalKqDequantB32T : MetalKqMulMmB32TensorT {
    typedef XT = float16
    override DEQW = true
}

[metal_dispatch(stamp = "kq_mm:q51:t", kernel = "metal_kq_mulmm_b32_t", pso = "g_pf_pso_kq_mm51_t", requires = "mp % 32, rows % 64")]
class MetalKqMulMmQ51T : MetalKqMulMmB32TensorT {
    typedef XT = float
    override Q51 = true
    override B32W = 5u
    override B32S = 2u
}

[metal_dispatch(stamp = "kq_mm:q51:th", kernel = "metal_kq_mulmm_b32_t", pso = "g_pf_pso_kq_mm51_th", requires = "mp % 32, rows % 64")]
class MetalKqMulMmQ51TH : MetalKqMulMmB32TensorT {
    typedef XT = float16
    override Q51 = true
    override B32W = 5u
    override B32S = 2u
}

[metal_dispatch(stamp = "kq_mm:q51:th128", kernel = "metal_kq_mulmm_b32_t", pso = "g_pf_pso_kq_mm51_th128", requires = "mp % 128, rows % 64")]
class MetalKqMulMmQ51TH128 : MetalKqMulMmB32TensorT {
    typedef XT = float16
    override Q51 = true
    override B32W = 5u
    override B32S = 2u
    override MT = 128u
}

[metal_dispatch(stamp = "kq_mm:q51:thdb", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm51_thdb", requires = "mp % 32, rows % 64")]
class MetalKqMulMmQ51THDb : MetalKqMulMmB32TensorT {
    typedef XT = float16
    override TWB = 9216
    override Q51 = true
    override B32W = 5u
    override B32S = 2u
}

[metal_dispatch(stamp = "kq_mm:q51:thsk", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm51_thsk", requires = "mp % 32, rows % 64")]
class MetalKqMulMmQ51THSk : MetalKqMulMmB32TensorT {
    typedef XT = float16
    override TWB = 9216
    override Q51 = true
    override B32W = 5u
    override B32S = 2u
    override SK = true
}

[metal_dispatch(stamp = "kq_mm:q51:tsk", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm51_tsk", requires = "mp % 32, rows % 64")]
class MetalKqMulMmQ51TSk : MetalKqMulMmB32TensorT {
    typedef XT = float
    override TWB = 9216
    override Q51 = true
    override B32W = 5u
    override B32S = 2u
    override SK = true
}

[metal_dispatch(stamp = "kq_mm:q51:thsk16", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm51_thsk16", requires = "rows % 64")]
class MetalKqMulMmQ51THSk16 : MetalKqMulMmB32TensorT {
    typedef XT = float16
    override TWB = 9216
    override Q51 = true
    override B32W = 5u
    override B32S = 2u
    override SK = true
    override MT = 16u
}

[metal_dispatch(stamp = "kq_mm:q51:tsk16", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm51_tsk16", requires = "rows % 64")]
class MetalKqMulMmQ51TSk16 : MetalKqMulMmB32TensorT {
    typedef XT = float
    override TWB = 9216
    override Q51 = true
    override B32W = 5u
    override B32S = 2u
    override SK = true
    override MT = 16u
}

[metal_dispatch(stamp = "kq_mm:q51:thdb128", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm51_thdb128", requires = "mp % 128, rows % 64")]
class MetalKqMulMmQ51THDb128 : MetalKqMulMmB32TensorT {
    typedef XT = float16
    override TWB = 9216
    override Q51 = true
    override B32W = 5u
    override B32S = 2u
    override MT = 128u
}

[metal_dispatch(name = "enc_kq_dq_q51", pso = "g_pf_pso_dq_kq51", kernel = "metal_kq_dequant", tgmem = "MetalKqDequantQ51_metal_kq_dequant_msl_tgmem", tg = 128, grid = "kdim/64, d/64", params = "kdim : int64, d : int64", requires = "kdim % 64, d % 64")]
class MetalKqDequantQ51 : MetalKqDequantB32T {
    override Q51 = true
    override B32W = 5u
    override B32S = 2u
}

[metal_dispatch(stamp = "kq_mm:iq4nl32:t", kernel = "metal_kq_mulmm_b32_t", pso = "g_pf_pso_kq_mm4n32_t", requires = "mp % 32, rows % 64")]
class MetalKqMulMmIq4nl32T : MetalKqMulMmB32TensorT {
    typedef XT = float
}

[metal_dispatch(stamp = "kq_mm:iq4nl32:th", kernel = "metal_kq_mulmm_b32_t", pso = "g_pf_pso_kq_mm4n32_th", requires = "mp % 32, rows % 64")]
class MetalKqMulMmIq4nl32TH : MetalKqMulMmB32TensorT {
    typedef XT = float16
}

[metal_dispatch(stamp = "kq_mm:iq4nl32:th128", kernel = "metal_kq_mulmm_b32_t", pso = "g_pf_pso_kq_mm4n32_th128", requires = "mp % 128, rows % 64")]
class MetalKqMulMmIq4nl32TH128 : MetalKqMulMmB32TensorT {
    typedef XT = float16
    override MT = 128u
}

[metal_dispatch(stamp = "kq_mm:iq4nl32:thdb", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm4n32_thdb", requires = "mp % 32, rows % 64")]
class MetalKqMulMmIq4nl32THDb : MetalKqMulMmB32TensorT {
    typedef XT = float16
    override TWB = 9216
}

[metal_dispatch(stamp = "kq_mm:iq4nl32:thsk", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm4n32_thsk", requires = "mp % 32, rows % 64")]
class MetalKqMulMmIq4nl32THSk : MetalKqMulMmB32TensorT {
    typedef XT = float16
    override TWB = 9216
    override SK = true
}

[metal_dispatch(stamp = "kq_mm:iq4nl32:tsk", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm4n32_tsk", requires = "mp % 32, rows % 64")]
class MetalKqMulMmIq4nl32TSk : MetalKqMulMmB32TensorT {
    typedef XT = float
    override TWB = 9216
    override SK = true
}

[metal_dispatch(stamp = "kq_mm:iq4nl32:thsk16", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm4n32_thsk16", requires = "rows % 64")]
class MetalKqMulMmIq4nl32THSk16 : MetalKqMulMmB32TensorT {
    typedef XT = float16
    override TWB = 9216
    override SK = true
    override MT = 16u
}

[metal_dispatch(stamp = "kq_mm:iq4nl32:tsk16", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm4n32_tsk16", requires = "rows % 64")]
class MetalKqMulMmIq4nl32TSk16 : MetalKqMulMmB32TensorT {
    typedef XT = float
    override TWB = 9216
    override SK = true
    override MT = 16u
}

[metal_dispatch(stamp = "kq_mm:iq4nl32:thdb128", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm4n32_thdb128", requires = "mp % 128, rows % 64")]
class MetalKqMulMmIq4nl32THDb128 : MetalKqMulMmB32TensorT {
    typedef XT = float16
    override TWB = 9216
    override MT = 128u
}

[metal_dispatch(name = "enc_kq_dq_iq4nl32", pso = "g_pf_pso_dq_kq4n32", kernel = "metal_kq_dequant", tgmem = "MetalKqDequantIq4nl32_metal_kq_dequant_msl_tgmem", tg = 128, grid = "kdim/64, d/64", params = "kdim : int64, d : int64", requires = "kdim % 64, d % 64")]
class MetalKqDequantIq4nl32 : MetalKqDequantB32T {}

[metal_dispatch(stamp = "kq_mm:q4032:t", kernel = "metal_kq_mulmm_b32_t", pso = "g_pf_pso_kq_mm4032_t", requires = "mp % 32, rows % 64")]
class MetalKqMulMmQ4032T : MetalKqMulMmB32TensorT {
    typedef XT = float
    override Q40L = true
}

[metal_dispatch(stamp = "kq_mm:q4032:th", kernel = "metal_kq_mulmm_b32_t", pso = "g_pf_pso_kq_mm4032_th", requires = "mp % 32, rows % 64")]
class MetalKqMulMmQ4032TH : MetalKqMulMmB32TensorT {
    typedef XT = float16
    override Q40L = true
}

[metal_dispatch(stamp = "kq_mm:q4032:th128", kernel = "metal_kq_mulmm_b32_t", pso = "g_pf_pso_kq_mm4032_th128", requires = "mp % 128, rows % 64")]
class MetalKqMulMmQ4032TH128 : MetalKqMulMmB32TensorT {
    typedef XT = float16
    override Q40L = true
    override MT = 128u
}

[metal_dispatch(stamp = "kq_mm:q4032:thdb", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm4032_thdb", requires = "mp % 32, rows % 64")]
class MetalKqMulMmQ4032THDb : MetalKqMulMmB32TensorT {
    typedef XT = float16
    override TWB = 9216
    override Q40L = true
}

[metal_dispatch(stamp = "kq_mm:q4032:thsk", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm4032_thsk", requires = "mp % 32, rows % 64")]
class MetalKqMulMmQ4032THSk : MetalKqMulMmB32TensorT {
    typedef XT = float16
    override TWB = 9216
    override Q40L = true
    override SK = true
}

[metal_dispatch(stamp = "kq_mm:q4032:tsk", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm4032_tsk", requires = "mp % 32, rows % 64")]
class MetalKqMulMmQ4032TSk : MetalKqMulMmB32TensorT {
    typedef XT = float
    override TWB = 9216
    override Q40L = true
    override SK = true
}

[metal_dispatch(stamp = "kq_mm:q4032:thsk16", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm4032_thsk16", requires = "rows % 64")]
class MetalKqMulMmQ4032THSk16 : MetalKqMulMmB32TensorT {
    typedef XT = float16
    override TWB = 9216
    override Q40L = true
    override SK = true
    override MT = 16u
}

[metal_dispatch(stamp = "kq_mm:q4032:tsk16", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm4032_tsk16", requires = "rows % 64")]
class MetalKqMulMmQ4032TSk16 : MetalKqMulMmB32TensorT {
    typedef XT = float
    override TWB = 9216
    override Q40L = true
    override SK = true
    override MT = 16u
}

[metal_dispatch(stamp = "kq_mm:q4032:thdb128", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm4032_thdb128", requires = "mp % 128, rows % 64")]
class MetalKqMulMmQ4032THDb128 : MetalKqMulMmB32TensorT {
    typedef XT = float16
    override TWB = 9216
    override Q40L = true
    override MT = 128u
}

[metal_dispatch(name = "enc_kq_dq_q4032", pso = "g_pf_pso_dq_kq4032", kernel = "metal_kq_dequant", tgmem = "MetalKqDequantQ4032_metal_kq_dequant_msl_tgmem", tg = 128, grid = "kdim/64, d/64", params = "kdim : int64, d : int64", requires = "kdim % 64, d % 64")]
class MetalKqDequantQ4032 : MetalKqDequantB32T {
    override Q40L = true
}

[metal_dispatch(stamp = "kq_mm:q5032:t", kernel = "metal_kq_mulmm_b32_t", pso = "g_pf_pso_kq_mm5032_t", requires = "mp % 32, rows % 64")]
class MetalKqMulMmQ5032T : MetalKqMulMmB32TensorT {
    typedef XT = float
    override Q50 = true
    override B32W = 5u
}

[metal_dispatch(stamp = "kq_mm:q5032:th", kernel = "metal_kq_mulmm_b32_t", pso = "g_pf_pso_kq_mm5032_th", requires = "mp % 32, rows % 64")]
class MetalKqMulMmQ5032TH : MetalKqMulMmB32TensorT {
    typedef XT = float16
    override Q50 = true
    override B32W = 5u
}

[metal_dispatch(stamp = "kq_mm:q5032:th128", kernel = "metal_kq_mulmm_b32_t", pso = "g_pf_pso_kq_mm5032_th128", requires = "mp % 128, rows % 64")]
class MetalKqMulMmQ5032TH128 : MetalKqMulMmB32TensorT {
    typedef XT = float16
    override Q50 = true
    override B32W = 5u
    override MT = 128u
}

[metal_dispatch(stamp = "kq_mm:q5032:thdb", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm5032_thdb", requires = "mp % 32, rows % 64")]
class MetalKqMulMmQ5032THDb : MetalKqMulMmB32TensorT {
    typedef XT = float16
    override TWB = 9216
    override Q50 = true
    override B32W = 5u
}

[metal_dispatch(stamp = "kq_mm:q5032:thsk", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm5032_thsk", requires = "mp % 32, rows % 64")]
class MetalKqMulMmQ5032THSk : MetalKqMulMmB32TensorT {
    typedef XT = float16
    override TWB = 9216
    override Q50 = true
    override B32W = 5u
    override SK = true
}

[metal_dispatch(stamp = "kq_mm:q5032:tsk", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm5032_tsk", requires = "mp % 32, rows % 64")]
class MetalKqMulMmQ5032TSk : MetalKqMulMmB32TensorT {
    typedef XT = float
    override TWB = 9216
    override Q50 = true
    override B32W = 5u
    override SK = true
}

[metal_dispatch(stamp = "kq_mm:q5032:thsk16", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm5032_thsk16", requires = "rows % 64")]
class MetalKqMulMmQ5032THSk16 : MetalKqMulMmB32TensorT {
    typedef XT = float16
    override TWB = 9216
    override Q50 = true
    override B32W = 5u
    override SK = true
    override MT = 16u
}

[metal_dispatch(stamp = "kq_mm:q5032:tsk16", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm5032_tsk16", requires = "rows % 64")]
class MetalKqMulMmQ5032TSk16 : MetalKqMulMmB32TensorT {
    typedef XT = float
    override TWB = 9216
    override Q50 = true
    override B32W = 5u
    override SK = true
    override MT = 16u
}

[metal_dispatch(stamp = "kq_mm:q5032:thdb128", kernel = "metal_kq_mulmm_db", pso = "g_pf_pso_kq_mm5032_thdb128", requires = "mp % 128, rows % 64")]
class MetalKqMulMmQ5032THDb128 : MetalKqMulMmB32TensorT {
    typedef XT = float16
    override TWB = 9216
    override Q50 = true
    override B32W = 5u
    override MT = 128u
}

[metal_dispatch(name = "enc_kq_dq_q5032", pso = "g_pf_pso_dq_kq5032", kernel = "metal_kq_dequant", tgmem = "MetalKqDequantQ5032_metal_kq_dequant_msl_tgmem", tg = 128, grid = "kdim/64, d/64", params = "kdim : int64, d : int64", requires = "kdim % 64, d % 64")]
class MetalKqDequantQ5032 : MetalKqDequantB32T {
    override Q50 = true
    override B32W = 5u
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
}

[metal_dispatch(name = "enc_kq_dq_iq4xs", pso = "g_pf_pso_dq_kq44", kernel = "metal_kq_dequant", tgmem = "MetalKqDequantIq4xs_metal_kq_dequant_msl_tgmem", tg = 128, grid = "kdim/64, d/64", params = "kdim : int64, d : int64")]
class MetalKqDequantIq4xs : MetalKqDequantIq4xsT {}

[metal_dispatch(name = "enc_kq_dq_iq3s4", pso = "g_pf_pso_dq_kq35", kernel = "metal_kq_dequant", tgmem = "MetalKqDequantIq3s4_metal_kq_dequant_msl_tgmem", tg = 128, grid = "kdim/64, d/64", params = "kdim : int64, d : int64", requires = "kdim % 64, d % 64")]
class MetalKqDequantIq3s4 : MetalKqDequantIq4xsT {
    override IQODD = true
}

[metal_dispatch(name = "enc_kq_dq_iq4nl", pso = "g_pf_pso_dq_kq45", kernel = "metal_kq_dequant", tgmem = "MetalKqDequantIq4nl_metal_kq_dequant_msl_tgmem", tg = 128, grid = "kdim/64, d/64", params = "kdim : int64, d : int64")]
class MetalKqDequantIq4nl : MetalKqDequantIq4xsT {
    override IQ4NL = true
}

[metal_dispatch(name = "enc_kq_dq_q40", pso = "g_pf_pso_dq_kq40", kernel = "metal_kq_dequant", tgmem = "MetalKqDequantQ40_metal_kq_dequant_msl_tgmem", tg = 128, grid = "kdim/64, d/64", params = "kdim : int64, d : int64")]
class MetalKqDequantQ40 : MetalKqDequantIq4xsT {
    override IQ4NL = true
    override Q40 = true
}

[metal_dispatch(name = "enc_kq_dq_nv4", pso = "g_pf_pso_dq_kq49", kernel = "metal_kq_dequant", tgmem = "MetalKqDequantNv4_metal_kq_dequant_msl_tgmem", tg = 128, grid = "kdim/64, d/64", params = "kdim : int64, d : int64")]
class MetalKqDequantNv4 : MetalKqDequantIq4xsT {
    override NV4 = true
}

[metal_dispatch(name = "enc_kq_dq_k3", pso = "g_pf_pso_dq_kq3", kernel = "metal_kq_dequant", tgmem = "MetalKqDequantK3_metal_kq_dequant_msl_tgmem", tg = 128, grid = "kdim/64, d/64", params = "kdim : int64, d : int64")]
class MetalKqDequantK3 : MetalKqMulMmK3TensorT {
    typedef XT = float16
    override DEQW = true
}

[metal_dispatch(name = "enc_kq_dq_iq3s", pso = "g_pf_pso_dq_kq33", kernel = "metal_kq_dequant", tgmem = "MetalKqDequantIq3s_metal_kq_dequant_msl_tgmem", tg = 128, grid = "kdim/64, d/64", params = "kdim : int64, d : int64")]
class MetalKqDequantIq3s : MetalKqMulMmIq3sTensorT {
    typedef XT = float16
    override DEQW = true
}

[metal_dispatch(name = "enc_kq_dq_iq3xxs", pso = "g_pf_pso_dq_kq34", kernel = "metal_kq_dequant", tgmem = "MetalKqDequantIq3xxs_metal_kq_dequant_msl_tgmem", tg = 128, grid = "kdim/64, d/64", params = "kdim : int64, d : int64")]
class MetalKqDequantIq3xxs : MetalKqMulMmIq3xxsTensorT {
    typedef XT = float16
    override DEQW = true
}

[metal_dispatch(name = "enc_kq_dq_k2", pso = "g_pf_pso_dq_kq2", kernel = "metal_kq_dequant", tgmem = "MetalKqDequantK2_metal_kq_dequant_msl_tgmem", tg = 128, grid = "kdim/64, d/64", params = "kdim : int64, d : int64")]
class MetalKqDequantK2 : MetalKqMulMmK2TensorT {
    typedef XT = float16
    override DEQW = true
}

[metal_dispatch(name = "enc_kq_dq_iq2s", pso = "g_pf_pso_dq_kq23", kernel = "metal_kq_dequant", tgmem = "MetalKqDequantIq2s_metal_kq_dequant_msl_tgmem", tg = 128, grid = "kdim/64, d/64", params = "kdim : int64, d : int64")]
class MetalKqDequantIq2s : MetalKqMulMmIq2sTensorT {
    typedef XT = float16
    override DEQW = true
}

[metal_dispatch(name = "enc_kq_dq_iq2xs", pso = "g_pf_pso_dq_kq24", kernel = "metal_kq_dequant", tgmem = "MetalKqDequantIq2xs_metal_kq_dequant_msl_tgmem", tg = 128, grid = "kdim/64, d/64", params = "kdim : int64, d : int64")]
class MetalKqDequantIq2xs : MetalKqMulMmIq2xsTensorT {
    typedef XT = float16
    override DEQW = true
}

[metal_dispatch(name = "enc_kq_dq_iq2xxs", pso = "g_pf_pso_dq_kq25", kernel = "metal_kq_dequant", tgmem = "MetalKqDequantIq2xxs_metal_kq_dequant_msl_tgmem", tg = 128, grid = "kdim/64, d/64", params = "kdim : int64, d : int64")]
class MetalKqDequantIq2xxs : MetalKqMulMmIq2xxsTensorT {
    typedef XT = float16
    override DEQW = true
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
            xg[r * ka.dim + i] = isreal ? half_sat(xf[token * ka.dim + i]) : float16(0.0)
            i += 256u
        }
    }
}

//! the routed tensor twins' scaffold over the mx4 expert stacks: the shared tile walk, a stamp staging its format's block
//! (`stage_block`)
[ |> template_struct_instance]
class template MetalMoeMulMmKqTensorBase : MetalMoeTileWalkT {
    override STORE_HOOK = true
    @ssbo @binding = 3 xf : array<XT>
    @ssbo @binding = 4 y : array<float>
    @uniform @binding = 5 ka : MoeMmArgs
    @ssbo @binding = 6 cnt : array<uint>
    @ssbo @binding = 7 basep : array<uint>

    def abstract stage_block(work, kb, e, nBase : uint) : void

    def override walk_kdim : uint => ka.kdim

    def override walk_ndim : uint => ka.ndim

    [unused_argument(nsb)]
    def override walk_stage(work, kb, e, nBase, nsb : uint) {
        stage_block(work, kb, e, nBase)
    }

    def run_shell {
        moe_walk(cnt[gl_WorkGroupID.z], basep[gl_WorkGroupID.z], REM)
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
    override ADAPT = false   //! the per-expert bias store is MT-only, so the mx4 tile keeps its 32-row op

    [metal_kernel(float_a_ok=true)]
    def metal_moe_mulmm_mx4_t {
        run_shell()
    }

    def override stage_block(work, kb, e, nBase : uint) {
        let nkb = ka.kdim / 32u
        let j = work >> 1u
        let kh = (work & 1u) * 16u
        let blk = e * ka.eplane + (nBase + j) * nkb + kb
        let d = e8m0_half_f32(uint(mxe[blk]))
        let u4 = mxq4[blk]
        for [unroll_full] (t in range(16)) {
            let byi = uint(t)
            let w32 = byi < 4u ? u4.x : (byi < 8u ? u4.y : (byi < 12u ? u4.z : u4.w))
            let nby = (w32 >> ((byi % 4u) * 8u)) & 255u
            twb[j * 32u + kh + byi] = float16(vtab[int(kh == 0u ? (nby & 15u) : (nby >> 4u))] * d)
        }
    }

    //! the bias store is MT-only, so the mx4 twins keep ADAPT off (an 8- or 16-row tile would store without the bias)
    def override store_out(var acc : float[8192]; var cp : float?; e, nBase : uint) {
        if (ka.hasb != 0u) {
            let bp = unsafe(addr(wb[e * ka.ndim + nBase]))
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

//! derives GkMoeCombine, the template both homes stamp
[metal_dispatch(name = "pf_enc_moe_reduce", pso = "g_pf_pso_moe_reduce", tgmem = "metal_moe_reduce_msl_tgmem", tg = 256, grid = "dim/256, npos", params = "dim : int64, npos : int64")]
class MetalMoeReduce : GkMoeCombine {
    override WINDOW = false
    [metal_kernel(name="metal_moe_reduce_msl")]
    def metal_moe_reduce {
        if (gl_GlobalInvocationID.x < pa.n) {
            comb_elem(gl_WorkGroupID.y, gl_GlobalInvocationID.x)
        }
    }
}

//! ... and its shared-expert stamp: the shared expert's rows under their gate logit's sigmoid added in the same pass
//! derives GkMoeCombine, the template both homes stamp
[metal_dispatch(name = "pf_enc_moe_reduce_sh", pso = "g_pf_pso_moe_reduce_sh", tgmem = "metal_moe_reduce_sh_msl_tgmem", tg = 256, grid = "dim/256, npos", params = "dim : int64, npos : int64")]
class MetalMoeReduceSh : GkMoeCombine {
    override WINDOW = false
    override SH = true

    [metal_kernel(name="metal_moe_reduce_sh_msl")]
    def metal_moe_reduce_sh {
        if (gl_GlobalInvocationID.x < pa.n) {
            comb_elem(gl_WorkGroupID.y, gl_GlobalInvocationID.x)
        }
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
        let sp = unsafe(addr(wsh[blk0 * 17u]))
        let wp = unsafe(addr(wqb[blk0 * 34u + 2u]))
        let ap = unsafe(addr(xf[(rbase + mBase) * ka.kdim]))
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

//! the k4 / k5 routed tensor twins: the dense split class under the MoE shell - one decode for the dense and the routed site
[ |> template_struct_instance]
class template MetalMoeMulMmK45SplitT : MetalKqMulMmK45DecodeT {
    override MOE = true

    [metal_kernel(float_a_ok=true)]
    def metal_moe_mulmm_k45_t {
        moe_kernel()
    }
}

[metal_dispatch(stamp = "moe_mm_split:k4:t")]
class MetalMoeMulMmK4T : MetalMoeMulMmK45SplitT {
    typedef XT = float
}

[metal_dispatch(stamp = "moe_mm_split:k4:th")]
class MetalMoeMulMmK4TH : MetalMoeMulMmK45SplitT {
    typedef XT = float16
}

[metal_dispatch(stamp = "moe_mm_split:k4:th128")]
class MetalMoeMulMmK4TH128 : MetalMoeMulMmK45SplitT {
    typedef XT = float16
    override MT = 128u
}

[metal_dispatch(stamp = "moe_mm_split:k4:thr")]
class MetalMoeMulMmK4THR : MetalMoeMulMmK45SplitT {
    typedef XT = float16
    override REM = true
}

[metal_dispatch(stamp = "moe_mm_split:k5:t")]
class MetalMoeMulMmK5T : MetalMoeMulMmK45SplitT {
    typedef XT = float
    override SB_UINTS = 40u
    override QH = true
}

[metal_dispatch(stamp = "moe_mm_split:k5:th")]
class MetalMoeMulMmK5TH : MetalMoeMulMmK45SplitT {
    typedef XT = float16
    override SB_UINTS = 40u
    override QH = true
}

[metal_dispatch(stamp = "moe_mm_split:k5:th128")]
class MetalMoeMulMmK5TH128 : MetalMoeMulMmK45SplitT {
    typedef XT = float16
    override SB_UINTS = 40u
    override QH = true
    override MT = 128u
}

[metal_dispatch(stamp = "moe_mm_split:k5:thr")]
class MetalMoeMulMmK5THR : MetalMoeMulMmK45SplitT {
    typedef XT = float16
    override SB_UINTS = 40u
    override QH = true
    override REM = true
}

//! the per-32 formats' routed twins: `MetalKqMulMmB32TensorT`'s decode on the split scaffold's expert walk
[ |> template_struct_instance]
class template MetalMoeMulMmB32SplitT : MetalKqMulMmB32TensorT {
    override MOE = true

    [metal_kernel(float_a_ok=true)]
    def metal_moe_mulmm_b32_t {
        moe_kernel()
    }
}

[metal_dispatch(stamp = "moe_mm_split:q51:t")]
class MetalMoeMulMmQ51T : MetalMoeMulMmB32SplitT {
    typedef XT = float
    override Q51 = true
    override B32W = 5u
    override B32S = 2u
}

[metal_dispatch(stamp = "moe_mm_split:q51:th")]
class MetalMoeMulMmQ51TH : MetalMoeMulMmB32SplitT {
    typedef XT = float16
    override Q51 = true
    override B32W = 5u
    override B32S = 2u
}

[metal_dispatch(stamp = "moe_mm_split:q51:th128")]
class MetalMoeMulMmQ51TH128 : MetalMoeMulMmB32SplitT {
    typedef XT = float16
    override Q51 = true
    override B32W = 5u
    override B32S = 2u
    override MT = 128u
}

[metal_dispatch(stamp = "moe_mm_split:q51:thr")]
class MetalMoeMulMmQ51THR : MetalMoeMulMmB32SplitT {
    typedef XT = float16
    override Q51 = true
    override B32W = 5u
    override B32S = 2u
    override REM = true
}

[metal_dispatch(stamp = "moe_mm_split:iq4nl32:t")]
class MetalMoeMulMmIq4nl32T : MetalMoeMulMmB32SplitT {
    typedef XT = float
}

[metal_dispatch(stamp = "moe_mm_split:iq4nl32:th")]
class MetalMoeMulMmIq4nl32TH : MetalMoeMulMmB32SplitT {
    typedef XT = float16
}

[metal_dispatch(stamp = "moe_mm_split:iq4nl32:th128")]
class MetalMoeMulMmIq4nl32TH128 : MetalMoeMulMmB32SplitT {
    typedef XT = float16
    override MT = 128u
}

[metal_dispatch(stamp = "moe_mm_split:iq4nl32:thr")]
class MetalMoeMulMmIq4nl32THR : MetalMoeMulMmB32SplitT {
    typedef XT = float16
    override REM = true
}

[metal_dispatch(stamp = "moe_mm_split:q4032:t")]
class MetalMoeMulMmQ4032T : MetalMoeMulMmB32SplitT {
    typedef XT = float
    override Q40L = true
}

[metal_dispatch(stamp = "moe_mm_split:q4032:th")]
class MetalMoeMulMmQ4032TH : MetalMoeMulMmB32SplitT {
    typedef XT = float16
    override Q40L = true
}

[metal_dispatch(stamp = "moe_mm_split:q4032:th128")]
class MetalMoeMulMmQ4032TH128 : MetalMoeMulMmB32SplitT {
    typedef XT = float16
    override Q40L = true
    override MT = 128u
}

[metal_dispatch(stamp = "moe_mm_split:q4032:thr")]
class MetalMoeMulMmQ4032THR : MetalMoeMulMmB32SplitT {
    typedef XT = float16
    override Q40L = true
    override REM = true
}

[metal_dispatch(stamp = "moe_mm_split:q5032:t")]
class MetalMoeMulMmQ5032T : MetalMoeMulMmB32SplitT {
    typedef XT = float
    override Q50 = true
    override B32W = 5u
}

[metal_dispatch(stamp = "moe_mm_split:q5032:th")]
class MetalMoeMulMmQ5032TH : MetalMoeMulMmB32SplitT {
    typedef XT = float16
    override Q50 = true
    override B32W = 5u
}

[metal_dispatch(stamp = "moe_mm_split:q5032:th128")]
class MetalMoeMulMmQ5032TH128 : MetalMoeMulMmB32SplitT {
    typedef XT = float16
    override Q50 = true
    override B32W = 5u
    override MT = 128u
}

[metal_dispatch(stamp = "moe_mm_split:q5032:thr")]
class MetalMoeMulMmQ5032THR : MetalMoeMulMmB32SplitT {
    typedef XT = float16
    override Q50 = true
    override B32W = 5u
    override REM = true
}


//! the k6 routed tensor twins: the dense split class under the MoE shell
[ |> template_struct_instance]
class template MetalMoeMulMmK6SplitT : MetalKqMulMmK6DecodeT {
    override MOE = true

    [metal_kernel(float_a_ok=true)]
    def metal_moe_mulmm_k6_t {
        moe_kernel()
    }
}

[metal_dispatch(stamp = "moe_mm_split:k6:t")]
class MetalMoeMulMmK6T : MetalMoeMulMmK6SplitT {
    typedef XT = float
}

[metal_dispatch(stamp = "moe_mm_split:k6:th")]
class MetalMoeMulMmK6TH : MetalMoeMulMmK6SplitT {
    typedef XT = float16
}

[metal_dispatch(stamp = "moe_mm_split:k6:th128")]
class MetalMoeMulMmK6TH128 : MetalMoeMulMmK6SplitT {
    typedef XT = float16
    override MT = 128u
}

[metal_dispatch(stamp = "moe_mm_split:k6:thr")]
class MetalMoeMulMmK6THR : MetalMoeMulMmK6SplitT {
    typedef XT = float16
    override REM = true
}

class template MetalMoeMulMmBase : MetalKqTileDecodeT {
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

//! the routed base form of every superblock and per-32 format: the walk of `MetalMoeMulMmBase` around the dense mul_mm's
//! decode (`MetalKqTileDecodeT`), so a format's routed tile and its dense tile decode one source; a stamp sets the format
[ |> template_struct_instance]
class template MetalMoeMulMmKqT : MetalMoeMulMmBase {
    override KQ = true

    def override sb_unit() : uint => kq_unit()

    def override stage_init(wb, il0 : uint) {
        kq_tables()
    }

    def override stage_a(wb, kb, il0, tA0 : uint) {
        var va : float16[16]
        kq_decode(wb, kb, il0, va)
        publish_a(va, tA0)
    }
}

[metal_dispatch(name = "pf_enc_moe_mm_k4_c", pso = "g_pf_pso_moe_mm_k4", tgmem = "metal_moe_mulmm_k4_msl_tgmem", tg = 128, grid = "npos/32, rows/64, ne", params = "rows : int64, npos : int64, ne : int64")]
class MetalMoeMulMmK4 : MetalMoeMulMmKqT {
    override KQ_SC = true

    [metal_kernel(name="metal_moe_mulmm_k4_msl")]
    def metal_moe_mulmm_k4 {
        run()
    }
}

[metal_dispatch(name = "pf_enc_moe_mm_k5_c", pso = "g_pf_pso_moe_mm_k5", tgmem = "metal_moe_mulmm_k5_msl_tgmem", tg = 128, grid = "npos/32, rows/64, ne", params = "rows : int64, npos : int64, ne : int64")]
class MetalMoeMulMmK5 : MetalMoeMulMmKqT {
    override KQ_SC = true
    override QH = true
    override SB_UINTS = 40u

    [metal_kernel(name="metal_moe_mulmm_k5_msl")]
    def metal_moe_mulmm_k5 {
        run()
    }
}

[metal_dispatch(name = "pf_enc_moe_mm_k6_c", pso = "g_pf_pso_moe_mm_k6", tgmem = "metal_moe_mulmm_k6_msl_tgmem", tg = 128, grid = "npos/32, rows/64, ne", params = "rows : int64, npos : int64, ne : int64")]
class MetalMoeMulMmK6 : MetalMoeMulMmKqT {
    override KQ_SC = true
    override SIXBIT = true

    [metal_kernel(name="metal_moe_mulmm_k6_msl")]
    def metal_moe_mulmm_k6 {
        run()
    }
}

[metal_dispatch(name = "pf_enc_moe_mm_k3_c", pso = "g_pf_pso_moe_mm_k3", tgmem = "metal_moe_mulmm_k3_msl_tgmem", tg = 128, grid = "npos/32, rows/64, ne", params = "rows : int64, npos : int64, ne : int64")]
class MetalMoeMulMmK3 : MetalMoeMulMmKqT {
    override KQ_SC = true
    override K3 = true

    [metal_kernel(name="metal_moe_mulmm_k3_msl")]
    def metal_moe_mulmm_k3 {
        run()
    }
}

[metal_dispatch(name = "pf_enc_moe_mm_k2_c", pso = "g_pf_pso_moe_mm_k2", tgmem = "metal_moe_mulmm_k2_msl_tgmem", tg = 128, grid = "npos/32, rows/64, ne", params = "rows : int64, npos : int64, ne : int64")]
class MetalMoeMulMmK2 : MetalMoeMulMmKqT {
    override KQ_SC = true
    override K2 = true

    [metal_kernel(name="metal_moe_mulmm_k2_msl")]
    def metal_moe_mulmm_k2 {
        run()
    }
}

[metal_dispatch(name = "pf_enc_moe_mm_iq4xs_c", pso = "g_pf_pso_moe_mm_iq4xs", tgmem = "metal_moe_mulmm_iq4xs_msl_tgmem", tg = 128, grid = "npos/32, rows/64, ne", params = "rows : int64, npos : int64, ne : int64")]
class MetalMoeMulMmIq4xs : MetalMoeMulMmKqT {
    override KQ_SC = true
    override IQ4XS = true

    [metal_kernel(name="metal_moe_mulmm_iq4xs_msl")]
    def metal_moe_mulmm_iq4xs {
        run()
    }
}

[metal_dispatch(name = "pf_enc_moe_mm_iq3s4_c", pso = "g_pf_pso_moe_mm_iq3s4", tgmem = "metal_moe_mulmm_iq3s4_msl_tgmem", tg = 128, grid = "npos/32, rows/64, ne", params = "rows : int64, npos : int64, ne : int64")]
class MetalMoeMulMmIq3s4 : MetalMoeMulMmKqT {
    override KQ_SC = true
    override IQ4XS = true
    override IQODD = true

    [metal_kernel(name="metal_moe_mulmm_iq3s4_msl")]
    def metal_moe_mulmm_iq3s4 {
        run()
    }
}

[metal_dispatch(name = "pf_enc_moe_mm_iq4nl_c", pso = "g_pf_pso_moe_mm_iq4nl", tgmem = "metal_moe_mulmm_iq4nl_msl_tgmem", tg = 128, grid = "npos/32, rows/64, ne", params = "rows : int64, npos : int64, ne : int64")]
class MetalMoeMulMmIq4nl : MetalMoeMulMmKqT {
    override KQ_SC = true
    override IQ4XS = true
    override IQ4NL = true

    [metal_kernel(name="metal_moe_mulmm_iq4nl_msl")]
    def metal_moe_mulmm_iq4nl {
        run()
    }
}

[metal_dispatch(name = "pf_enc_moe_mm_q40_c", pso = "g_pf_pso_moe_mm_q40", tgmem = "metal_moe_mulmm_q40_msl_tgmem", tg = 128, grid = "npos/32, rows/64, ne", params = "rows : int64, npos : int64, ne : int64")]
class MetalMoeMulMmQ40 : MetalMoeMulMmKqT {
    override KQ_SC = true
    override IQ4XS = true
    override IQ4NL = true
    override Q40 = true

    [metal_kernel(name="metal_moe_mulmm_q40_msl")]
    def metal_moe_mulmm_q40 {
        run()
    }
}

[metal_dispatch(name = "pf_enc_moe_mm_nv4_c", pso = "g_pf_pso_moe_mm_nv4", tgmem = "metal_moe_mulmm_nv4_msl_tgmem", tg = 128, grid = "npos/32, rows/64, ne", params = "rows : int64, npos : int64, ne : int64")]
class MetalMoeMulMmNv4 : MetalMoeMulMmKqT {
    override KQ_SC = true
    override IQ4XS = true
    override NV4 = true

    [metal_kernel(name="metal_moe_mulmm_nv4_msl")]
    def metal_moe_mulmm_nv4 {
        run()
    }
}

[metal_dispatch(name = "pf_enc_moe_mm_iq3s_c", pso = "g_pf_pso_moe_mm_iq3s", tgmem = "metal_moe_mulmm_iq3s_msl_tgmem", tg = 128, grid = "npos/32, rows/64, ne", params = "rows : int64, npos : int64, ne : int64")]
class MetalMoeMulMmIq3s : MetalMoeMulMmKqT {
    override KQ_SC = true
    override IQ3S = true

    [metal_kernel(name="metal_moe_mulmm_iq3s_msl")]
    def metal_moe_mulmm_iq3s {
        run()
    }
}

[metal_dispatch(name = "pf_enc_moe_mm_iq3xxs_c", pso = "g_pf_pso_moe_mm_iq3xxs", tgmem = "metal_moe_mulmm_iq3xxs_msl_tgmem", tg = 128, grid = "npos/32, rows/64, ne", params = "rows : int64, npos : int64, ne : int64")]
class MetalMoeMulMmIq3xxs : MetalMoeMulMmKqT {
    override KQ_SC = true
    override IQ3XXS = true

    [metal_kernel(name="metal_moe_mulmm_iq3xxs_msl")]
    def metal_moe_mulmm_iq3xxs {
        run()
    }
}

[metal_dispatch(name = "pf_enc_moe_mm_iq2s_c", pso = "g_pf_pso_moe_mm_iq2s", tgmem = "metal_moe_mulmm_iq2s_msl_tgmem", tg = 128, grid = "npos/32, rows/64, ne", params = "rows : int64, npos : int64, ne : int64")]
class MetalMoeMulMmIq2s : MetalMoeMulMmKqT {
    override KQ_SC = true
    override IQ2S = true

    [metal_kernel(name="metal_moe_mulmm_iq2s_msl")]
    def metal_moe_mulmm_iq2s {
        run()
    }
}

[metal_dispatch(name = "pf_enc_moe_mm_iq2xs_c", pso = "g_pf_pso_moe_mm_iq2xs", tgmem = "metal_moe_mulmm_iq2xs_msl_tgmem", tg = 128, grid = "npos/32, rows/64, ne", params = "rows : int64, npos : int64, ne : int64")]
class MetalMoeMulMmIq2xs : MetalMoeMulMmKqT {
    override KQ_SC = true
    override IQ2XS = true

    [metal_kernel(name="metal_moe_mulmm_iq2xs_msl")]
    def metal_moe_mulmm_iq2xs {
        run()
    }
}

[metal_dispatch(name = "pf_enc_moe_mm_iq2xxs_c", pso = "g_pf_pso_moe_mm_iq2xxs", tgmem = "metal_moe_mulmm_iq2xxs_msl_tgmem", tg = 128, grid = "npos/32, rows/64, ne", params = "rows : int64, npos : int64, ne : int64")]
class MetalMoeMulMmIq2xxs : MetalMoeMulMmKqT {
    override KQ_SC = true
    override IQ2XXS = true

    [metal_kernel(name="metal_moe_mulmm_iq2xxs_msl")]
    def metal_moe_mulmm_iq2xxs {
        run()
    }
}

[metal_dispatch(name = "pf_enc_moe_mm_q51_c", pso = "g_pf_pso_moe_mm_q51", tgmem = "metal_moe_mulmm_q51_msl_tgmem", tg = 128, grid = "npos/32, rows/64, ne", params = "rows : int64, npos : int64, ne : int64")]
class MetalMoeMulMmQ51 : MetalMoeMulMmKqT {
    override B32 = true
    override Q51 = true
    override B32W = 5u
    override B32S = 2u

    [metal_kernel(name="metal_moe_mulmm_q51_msl")]
    def metal_moe_mulmm_q51 {
        run()
    }
}

[metal_dispatch(name = "pf_enc_moe_mm_iq4nl32_c", pso = "g_pf_pso_moe_mm_iq4nl32", tgmem = "metal_moe_mulmm_iq4nl32_msl_tgmem", tg = 128, grid = "npos/32, rows/64, ne", params = "rows : int64, npos : int64, ne : int64")]
class MetalMoeMulMmIq4nl32 : MetalMoeMulMmKqT {
    override B32 = true
    override IQ4XS = true

    [metal_kernel(name="metal_moe_mulmm_iq4nl32_msl")]
    def metal_moe_mulmm_iq4nl32 {
        run()
    }
}

[metal_dispatch(name = "pf_enc_moe_mm_q4032_c", pso = "g_pf_pso_moe_mm_q4032", tgmem = "metal_moe_mulmm_q4032_msl_tgmem", tg = 128, grid = "npos/32, rows/64, ne", params = "rows : int64, npos : int64, ne : int64")]
class MetalMoeMulMmQ4032 : MetalMoeMulMmKqT {
    override B32 = true
    override IQ4XS = true
    override Q40 = true

    [metal_kernel(name="metal_moe_mulmm_q4032_msl")]
    def metal_moe_mulmm_q4032 {
        run()
    }
}

[metal_dispatch(name = "pf_enc_moe_mm_q5032_c", pso = "g_pf_pso_moe_mm_q5032", tgmem = "metal_moe_mulmm_q5032_msl_tgmem", tg = 128, grid = "npos/32, rows/64, ne", params = "rows : int64, npos : int64, ne : int64")]
class MetalMoeMulMmQ5032 : MetalMoeMulMmKqT {
    override B32 = true
    override Q50 = true
    override B32W = 5u

    [metal_kernel(name="metal_moe_mulmm_q5032_msl")]
    def metal_moe_mulmm_q5032 {
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
        let d = e8m0_half_f32(uint(mxe[blk]))
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

//! The window's q/k norm and rope (`GkQknRopeKv`) in place over its panels - K `kvsrc` and `kmir` at `kbase`, V `vsrc` and `vmir`
//! at `vbase`, a k head storing its V row only under `vnorm` / `vfk`; a threadgroup a head row, the rows past `nreal` pad, the HX
//! stamp the q panel's half twin
[ |> template_struct_instance]
class template MetalPfQknT : GkQknRopeKv {
    @ssbo @binding = 6 @role = "read" tsin : array<float>   //! the rope table's sin rows
    override CLAMP16 = false
    override LANES = 256u
    override EPT = 1
    override V_IN_PLACE = true

    def override mir_base(rowi : uint) : uint2 => uint2((pa.row0 + rowi) * pa.kvrow + pa.kbase, (pa.row0 + rowi) * pa.kvrow + pa.vbase)

    def override rope_sin(cb, j : uint) : float => tsin[cb + j]
}

[metal_dispatch(name = "enc_pf_qkn", pso = "g_pf_pso_qkr", tgmem = "metal_pf_qkn_rope_msl_tgmem", tg = "npair", grid = "nwg, nrows", params = "nwg : int64, nrows : int64, npair : int64")]
class MetalPfQkn : MetalPfQknT {
    typedef KT = float

    [metal_kernel(name="metal_pf_qkn_rope_msl")]
    def metal_pf_qkn_rope {
        qkn_head(gl_WorkGroupID.y, gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

[metal_dispatch(name = "enc_pf_qkn_hx", pso = "g_pf_pso_qkr_hx", tgmem = "metal_pf_qkn_rope_hx_msl_tgmem", tg = "npair", grid = "nwg, nrows", params = "nwg : int64, nrows : int64, npair : int64")]
class MetalPfQknHX : MetalPfQknT {
    typedef KT = float
    override HX = true

    [metal_kernel(name="metal_pf_qkn_rope_hx_msl"), arch(at="../ARCHITECTURE_GPU_PREFILL.md#prefill-twin-dual-store")]
    def metal_pf_qkn_rope_hx {
        qkn_head(gl_WorkGroupID.y, gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! the window's q/k norm and rope over the q panel `bq` and the K panel at byte `koff` of `bk`: `nq` q heads and `nk` k heads of
//! `npos` live rows over `nrows` (the q twin's padded rows under `bqh`), the norm rows at `bqkw` unless `norm` is off; the V panel
//! `bv` at the same offset takes the k heads' V rows under `vfk` (V the k row under its own rms) or `vnorm` (normed in place)
def pf_enc_qkn(enc : MetalComputeEncoder?; bq, bk, bv : MetalBuffer?; koff : uint64; bcos, bsin, bqkw, bqh : MetalBuffer?; nq, nk, qd, kvd, hs, rot : int64;
               neox, norm, vfk, vnorm : bool; eps, hscale : float; npos, nrows : int64) {
    var ka = GkQknRopeArgs(kvd = uint(kvd), hs = uint(hs), half = uint(rot / 2l), neox = neox ? 1u : 0u, nh = uint(nq), kwoff = uint(hs), eps = eps,
        win = 1u, kbase = uint(koff / 4ul), vbase = uint(koff / 4ul), kvrow = uint(kvd), qrow = uint(qd), cosrow = uint(rot / 2l), nreal = uint(npos),
        skipnorm = norm ? 0u : 1u, hscale = hscale, vfk = vfk ? 1u : 0u, vnorm = vnorm ? 1u : 0u)
    if (bqh != null) {
        enc_pf_qkn_hx(enc, bq, bk, bk, 0ul, bv, 0ul, bcos, 0ul, bqkw, bsin, bcos, bqh, bv, ka, nq + nk, nrows, hs / 2l)
    } else {
        enc_pf_qkn(enc, bq, bk, bk, 0ul, bv, 0ul, bcos, 0ul, bqkw, bsin, bcos, bv, ka, nq + nk, nrows, hs / 2l)
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
    ring : uint         //!< the device pair's K/V rows: a sliding layer's ring of the mirror (`kv_ring_row`); 0 = a row a position
}

[metal_dispatch(name = "enc_qk", pso = "g_pf_pso_qk", tgmem = "metal_attn_qk_msl_tgmem", tg = 128, grid = "mp/32, nk64/32, heads", params = "mp : int64, nk64 : int64, heads : int64")]
class MetalAttnQK {
    @ssbo @binding = 0 @role = "read" @off = "qboff" q : array<float>     //!< [mp x qd], roped; a segment's rows at its byte offset
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
        var c00, c01, c10, c11 : simdgroup_float8x8
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
                var a0, a1, b0, b1 : simdgroup_float8x8
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
def pf_av_lo(window, t0 : uint) : uint => window > 0u && t0 + 1u > window ? t0 + 1u - window : 0u

//! one past the last key an AV tile of `mt` query rows starting at `t0` reduces over - live rows only
def pf_av_hi(uend, ulo, npos, t0, mt : uint) : uint {
    let creach = t0 + mt
    return min(uend > 0u && t0 < uend && creach > ulo ? max(uend, creach) : creach, npos)
}

//! A QK tile of `mt` query rows from global position `t0` over the `kw` keys from `k0` has no live score:
//! its keys sit past the rows' causal reach (the uniform span's end where one is set) or under their window.
def pf_qk_skip(uend, window, t0, mt, k0, kw : uint) : bool =>
    (uend > 0u ? k0 >= max(uend, t0 + mt) : k0 > t0 + mt - 1u) || k0 + kw <= pf_av_lo(window, t0)

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
    @ssbo @binding = 2 @role = "write" @off = "xboff" xb : array<float>    //!< [mp x qd] attention output; a segment's rows at its byte offset
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
        var c00, c01, c10, c11 : simdgroup_float8x8
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
                var a0, a1, b0, b1 : simdgroup_float8x8
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
    @ssbo @binding = 0 @off = "qboff" q : array<XT>
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
    @ssbo @binding = 0 @role = "read" @off = "qboff" q : array<float4>    //!< [mp x qd/4], roped; a segment's rows at its byte offset
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
    @ssbo @binding = 2 @off = "xboff" xb : array<float>
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
    @ssbo @binding = 2 @role = "write" @off = "xboff" xb : array<float>    //!< [mp x qd] attention output; a segment's rows at its byte offset
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

//! A K and a V panel of `total` elements from element `xoff` as halves, the softmax scale on K: the device pair's twins, or
//! the KV mirror's rows at `khoff`/`vhoff`.
def enc_kv_twin(enc : MetalComputeEncoder?; bk, bv, bkh, bvh : MetalBuffer?; scale : float; total : int64; khoff : uint64 = 0ul; vhoff : uint64 = 0ul; xoff : uint64 = 0ul) {
    tw_half(enc, bk, bkh, total, scale, khoff, xoff)
    tw_half(enc, bv, bvh, total, 1.0, vhoff, xoff)
}

[ |> template_struct_instance]
class template MetalAttnQKDevT {
    @template_constant MT : uint = 32u
    @ssbo @binding = 0 @role = "read" @off = "qboff" q : array<float16>    //!< [mp x qd] roped Q, the half twin; a segment's rows at its byte offset
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
            unsafe(addr(k[kv_ring_row(nBase, ka.ring) * ka.kv_dim + (h / ka.kv_mul) * ka.head_size])), ka.kv_dim, MT, 64u, ka.head_size)   //! 64 keys from a 64-row step: one ring piece
        tmm2d_tg_store_half(acc, cp, MT, 64u, ka.np32)
    }
}

[metal_dispatch(name = "enc_qk_dev_c", pso = "g_pf_pso_qk_dev", tg = 128, grid = "mp/32, nk64/64, heads", params = "mp : int64, nk64 : int64, heads : int64", requires = "mp % 32, nk64 % 64, ka.ring % 64")]
class MetalAttnQKDev : MetalAttnQKDevT {
}

[metal_dispatch(name = "enc_qk_dev128_c", pso = "g_pf_pso_qk_dev128", tg = 128, grid = "mp/128, nk64/64, heads", params = "mp : int64, nk64 : int64, heads : int64", requires = "mp % 128, nk64 % 64, ka.ring % 64")]
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
    @ssbo @binding = 2 @off = "xboff" xb : array<float>    //!< a segment's rows at its byte offset
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
        let wrap = ka.ring == 0u ? hi : min(hi, (lo / ka.ring + 1u) * ka.ring)   //! a ring's keys run back to back up to its wrap, then from slot 0
        tmm2d_tg_begin_devabnt(acc, MT, NT, 4u)
        tmm2d_tg_step_devabnt(acc, unsafe(addr(att[(h * ka.qrows + mBase) * ka.np32 + lo])), ka.np32,
            unsafe(addr(v[kv_ring_row(lo, ka.ring) * ka.kv_dim + (h / ka.kv_mul) * ka.head_size + vBase])), ka.kv_dim, MT, NT, wrap - lo)
        if (wrap < hi) {
            tmm2d_tg_step_devabnt(acc, unsafe(addr(att[(h * ka.qrows + mBase) * ka.np32 + wrap])), ka.np32,
                unsafe(addr(v[kv_ring_row(wrap, ka.ring) * ka.kv_dim + (h / ka.kv_mul) * ka.head_size + vBase])), ka.kv_dim, MT, NT, hi - wrap)
        }
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
                    var mk, mq : simdgroup_half8x8
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
                var vs0, vs1 : simdgroup_float8x8
                simdgroup_load(vs0, ss, 16u * uint(cc), 128u)
                simdgroup_load(vs1, ss, 16u * uint(cc) + 8u, 128u)
                for [unroll_full] (ii in range(2)) {
                    var mv0, mv1, mv2, mv3 : simdgroup_half8x8
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

//! the block-diagonal window attention of the qwen2.5 ViT on the template both homes stamp, pinned to the family's head width
[metal_dispatch(name = "enc_tower_win_attn", pso = "g_pf_pso_twr_win", tgmem = "metal_tower_win_msl_tgmem", tg = 256, grid = "wgs", params = "wgs : int64")]
class MetalTowerWinAttn : GkWinAttn {
    override HS = 80u

    [metal_kernel(name="metal_tower_win_msl"), arch(at="../ARCHITECTURE_GPU_TOWER.md#tower-attn-routes")]
    def metal_tower_win {
        win_attn_body()
    }
}

//! The gemma4a Conformer attention's argument block (the sliding causal window + sinusoidal RPE).
//! The gemma4a causal sliding-window attention on the template both homes stamp, the head width at run time
[metal_dispatch(name = "enc_g4a_attn", pso = "g_pf_pso_g4a_attn", tg = 64, grid = "total/64", params = "total : int64")]
class MetalG4aAttn : GkG4aAttn {
    [metal_kernel(name="metal_g4a_attn_msl")]
    def metal_g4a_attn {
        g4a_attn_body()
    }
}

//! The conv module's GLU off the pointwise rows (gemma4a, the FastConformers), on the gated-rows template both homes stamp
[metal_dispatch(name = "enc_g4a_glu", pso = "g_pf_pso_g4a_glu", tg = 64, grid = "nelem/64", params = "nelem : int64")]
class MetalG4aGlu : GkGatedRows {
    [metal_kernel(name="metal_g4a_glu_msl")]
    def metal_g4a_glu {
        gated_rows_body()
    }
}

//! The gemma4a causal depthwise conv, k = 5, on the template both homes stamp (the chunk's first rows read nothing before the start)
[metal_dispatch(name = "enc_g4a_dw", pso = "g_pf_pso_g4a_dw", tg = 64, grid = "total/64", params = "total : int64")]
class MetalG4aDw : GkDwConvRows {
    override FORM = 1
    override K = 5u

    [metal_kernel(name="metal_g4a_dw_msl")]
    def metal_g4a_dw {
        dw_conv_rows_body()
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

//! The FastConformer conv module's centered depthwise conv with the folded BatchNorm and SiLU, on the template both homes
//! stamp: canary's channel-major taps, and parakeet's load-time tap-major ones under TAP_MAJOR
[metal_dispatch(name = "enc_cn_dw", pso = "g_pf_pso_cn_dw", tg = 64, grid = "total/64", params = "total : int64")]
class MetalCnDw : GkDwConvRows {
    override FORM = 2

    [metal_kernel(name="metal_cn_dw_msl")]
    def metal_cn_dw {
        dw_conv_rows_body()
    }
}

//! parakeet's tap-major depthwise rows conv on the template both homes stamp (GkDwConvRows)
[metal_dispatch(name = "enc_pk_dw", pso = "g_pf_pso_pk_dw", tg = 64, grid = "total/64", params = "total : int64")]
class MetalPkDw : GkDwConvRows {
    override FORM = 2
    override TAP_MAJOR = true

    [metal_kernel(name="metal_pk_dw_msl")]
    def metal_pk_dw {
        dw_conv_rows_body()
    }
}

//! gigaam's centered depthwise conv on the template both homes stamp (GkDwConvRows): the file's tap-major taps, its bias row,
//! no epilogue - the conv module's LayerNorm and SiLU follow as their own rows passes
[metal_dispatch(name = "enc_ga_dw", pso = "g_pf_pso_ga_dw", tg = 64, grid = "total/64", params = "total : int64")]
class MetalGaDw : GkDwConvRows {
    override FORM = 3
    override TAP_MAJOR = true

    [metal_kernel(name="metal_ga_dw_msl")]
    def metal_ga_dw {
        dw_conv_rows_body()
    }
}

//! gigaam's depthwise conv over its taps as the carrier's f16 halfwords (`GkDwConvRows`, TAP_F16): the halfword plane at `wh`,
//! the bias row in the f32 plane
[metal_dispatch(name = "enc_ga_dwh", pso = "g_pf_pso_ga_dwh", tg = 64, grid = "total/64", params = "total : int64")]
class MetalGaDwH : GkDwConvRows {
    override FORM = 3
    override TAP_MAJOR = true
    override TAP_F16 = true

    [metal_kernel(name="metal_ga_hdw_msl")]
    def metal_ga_hdw {
        dw_conv_rows_body()
    }
}

// ===== the silero VAD's two stamps (the tower driver's VAD seat) =====

//! the VAD's encoder on the template both homes stamp (`GkVadEnc`): a threadgroup of 128 a chunk, every chunk of the call
[metal_dispatch(name = "enc_vad_enc", pso = "g_pf_pso_vad_enc", tgmem = "metal_vad_enc_msl_tgmem", tg = 128, grid = "n", params = "n : int64")]
class MetalVadEnc : GkVadEnc {
    [metal_kernel(name="metal_vad_enc_msl")]
    def metal_vad_enc {
        vad_enc_body()
    }
}

//! the VAD's recurrence and decoder on the template both homes stamp (`GkVadLstm`): one threadgroup of 1024 walks every chunk
[metal_dispatch(name = "enc_vad_lstm", pso = "g_pf_pso_vad_lstm", tgmem = "metal_vad_lstm_msl_tgmem", tg = 1024, grid = "1")]
class MetalVadLstm : GkVadLstm {
    override PTR_LOADS = true

    [metal_kernel(name="metal_vad_lstm_msl", max_threads=1024)]
    def metal_vad_lstm {
        vad_lstm_body()
    }
}

//! One stage of the parakeet subsample front's geometry: a k3 stride-2 pad-1 conv over an [ih x iw] image of channel rows.
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

//! The subsample stacks' depthwise conv with its bias on the template both homes stamp: parakeet's tap-major taps, canary's
//! channel-major ones as the file keeps them
[metal_dispatch(name = "enc_pk_dw2d", pso = "g_pf_pso_pk_dw2d", tg = 64, grid = "total/64", params = "total : int64")]
class MetalPkDw2d : GkDwConv2d {
    override TAP_MAJOR = true

    [metal_kernel(name="metal_pk_dw2d_msl")]
    def metal_pk_dw2d {
        dw_conv2d_body()
    }
}

//! canary's channel-major depthwise conv2d on the template both homes stamp (GkDwConv2d)
[metal_dispatch(name = "enc_cn_dw2d", pso = "g_pf_pso_cn_dw2d", tg = 64, grid = "total/64", params = "total : int64")]
class MetalCnDw2d : GkDwConv2d {
    [metal_kernel(name="metal_cn_dw2d_msl")]
    def metal_cn_dw2d {
        dw_conv2d_body()
    }
}

//! The front's feature rows for the pre-projection, on the template both homes stamp
[metal_dispatch(name = "enc_pk_xf", pso = "g_pf_pso_pk_xf", tg = 64, grid = "total/64", params = "total : int64")]
class MetalPkXf : GkFeatShuffle {
    [metal_kernel(name="metal_pk_xf_msl")]
    def metal_pk_xf {
        feat_shuffle_body()
    }
}

//! The Transformer-XL rel-position table of the FastConformers on the device, on the template both homes stamp
[metal_dispatch(name = "enc_tw_rel_sincos", pso = "g_pf_pso_tw_rel_sincos", tg = 64, grid = "total/64", params = "total : int64")]
class MetalTwRelSinCos : GkRelSinCos {
    [metal_kernel(name="metal_tw_rel_sincos_msl")]
    def metal_tw_rel_sincos {
        rel_sincos_body()
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

//! The whisper-class projector tails' pair pool, on the template both homes stamp.
[metal_dispatch(name = "enc_tw_pool2", pso = "g_pf_pso_tw_pool2", tg = 64, grid = "total/64", params = "total : int64")]
class MetalTwPool2 : GkPoolRows {
    [metal_kernel(name="metal_tw_pool2_msl")]
    def metal_tw_pool2 {
        pool_rows_body()
    }
}

//! a vision tail's row standardize on the template both homes stamp
[metal_dispatch(name = "enc_tw_affine_rows", pso = "g_pf_pso_tw_affine", tg = 64, grid = "total/64", params = "total : int64")]
class MetalTwAffineRows : GkAffineRows {
    [metal_kernel(name = "metal_tw_affine_rows_msl")]
    def metal_tw_affine_rows {
        affine_rows_body()
    }
}

//! a vision tail's grid mean pool on the template both homes stamp
[metal_dispatch(name = "enc_tw_pool2d", pso = "g_pf_pso_tw_pool2d", tg = 64, grid = "total/64", params = "total : int64")]
class MetalTwPool2d : GkPool2d {
    [metal_kernel(name = "metal_tw_pool2d_msl")]
    def metal_tw_pool2d {
        pool_2d_body()
    }
}

//! The ultravox projector's swapped SwiGLU gate, on the gated-rows template both homes stamp
[metal_dispatch(name = "enc_tw_swiglu_rows", pso = "g_pf_pso_tw_swiglu_rows", tg = 64, grid = "nelem/64", params = "nelem : int64")]
class MetalTwSwigluRows : GkGatedRows {
    override FORM = 1

    [metal_kernel(name="metal_tw_swiglu_rows_msl")]
    def metal_tw_swiglu_rows {
        gated_rows_body()
    }
}

//! Canary's log and per-feature normalization over the mel GEMM's sums, on the template both homes stamp - the log first (LOG_IN),
//! since the Metal mel GEMM leaves the sums
[metal_dispatch(name = "enc_cn_melnorm", pso = "g_pf_pso_cn_melnorm", tg = 64, grid = "nmel/64", params = "nmel : int64")]
class MetalCnMelNorm : GkMelNorm {
    override LOG_IN = true

    [metal_kernel(name="metal_cn_melnorm_msl")]
    def metal_cn_melnorm {
        mel_norm_body()
    }
}

//! gigaam's windowed frames off the raw samples, on the template both homes stamp (GkFrameWindow)
[metal_dispatch(name = "enc_ga_frames", pso = "g_pf_pso_ga_frames", tg = 64, grid = "total/64", params = "total : int64")]
class MetalGaFrames : GkFrameWindow {
    [metal_kernel(name="metal_ga_frames_msl")]
    def metal_ga_frames {
        frame_window_body()
    }
}

//! gigaam's clamped-log filterbank over the power rows, on the template both homes stamp (GkMelLog)
[metal_dispatch(name = "enc_ga_mel", pso = "g_pf_pso_ga_mel", tg = 64, grid = "total/64", params = "total : int64")]
class MetalGaMel : GkMelLog {
    override GIGAAM = true

    [metal_kernel(name="metal_ga_mel_msl")]
    def metal_ga_mel {
        mel_log_body()
    }
}

//! The subsample conv's LayerNorm(channels, weight-only) + ReLU, on the template both homes stamp: the conv GEMM's padded
//! rows in, compact rows out
[metal_dispatch(name = "enc_g4a_lnrelu", pso = "g_pf_pso_g4a_lnrelu", tg = 64, grid = "rows/64", params = "rows : int64")]
class MetalG4aLnRelu : GkLnReluRows {
    [metal_kernel(name="metal_g4a_lnrelu_msl")]
    def metal_g4a_lnrelu {
        ln_relu_rows_body()
    }
}


//! The qwen3a conv-frontend feature shuffle off the ncp-strided conv3 slab, on the template both homes stamp
[metal_dispatch(name = "enc_q3a_shuffle", pso = "g_pf_pso_q3a_shuffle", tg = 64, grid = "total/64", params = "total : int64")]
class MetalQ3aShuffle : GkQ3aShuffle {
    typedef OT = float

    [metal_kernel(name="metal_q3a_shuffle_msl")]
    def metal_q3a_shuffle {
        q3a_shuffle_body()
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

//! The macaron half-step's scaled residual add, g += u * scale, on the axpy template both homes stamp
[metal_dispatch(name = "enc_g4a_axpy", pso = "g_pf_pso_g4a_axpy", tg = 64, grid = "nelem/64", params = "nelem : int64")]
class MetalG4aAxpy : GkAxpy {
    [metal_kernel(name="metal_g4a_axpy_msl")]
    def metal_g4a_axpy {
        axpy_body()
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
var private g_pf_pso_qk : MetalComputePipeline?
var private g_pf_pso_rowstat : MetalComputePipeline?
var private g_pf_pso_av : MetalComputePipeline?
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
var private g_pf_pso_ga_dw : MetalComputePipeline?       //!< gigaam's biased centered depthwise conv, no epilogue
var private g_pf_pso_ga_dwh : MetalComputePipeline?      //!< ... over the carrier's f16 taps
var private g_pf_pso_vad_enc : MetalComputePipeline?     //!< the silero VAD's encoder, a threadgroup a chunk
var private g_pf_pso_vad_lstm : MetalComputePipeline?    //!< ... and its recurrence over every chunk
var private g_pf_pso_pk_dw2d : MetalComputePipeline?     //!< its depthwise stride-2 conv
var private g_pf_pso_cn_dw2d : MetalComputePipeline?     //!< canary's, over the file's channel-major taps
var private g_pf_pso_tw_rel_sincos : MetalComputePipeline?   //!< the FastConformers' rel-position table on the device
var private g_pf_pso_pk_xf : MetalComputePipeline?       //!< its feature rows for the pre-projection
var private g_pf_pso_cn_melnorm : MetalComputePipeline?  //!< canary's log and per-feature normalization over the mel
var private g_pf_pso_ga_frames : MetalComputePipeline?   //!< gigaam's windowed frames off the raw samples
var private g_pf_pso_ga_mel : MetalComputePipeline?      //!< gigaam's clamped-log filterbank
var private g_pf_pso_tw_pool2 : MetalComputePipeline?    //!< the whisper-class projector tails' pair pool
var private g_pf_pso_tw_swiglu_rows : MetalComputePipeline?   //!< the ultravox projector's row-split gate
var private g_pf_pso_tw_pool2d : MetalComputePipeline?   //!< a vision tail's k x k mean pool over the row grid
var private g_pf_pso_tw_affine : MetalComputePipeline?   //!< a vision tail's scale and standardize over rows
var private g_pf_pso_g4a_attn : MetalComputePipeline?    //!< the gemma4a sliding-window RPE attention
var private g_pf_pso_g4a_glu : MetalComputePipeline?     //!< its conv-module GLU halves
var private g_pf_pso_g4a_dw : MetalComputePipeline?      //!< its causal depthwise k=5
var private g_pf_pso_g4a_axpy : MetalComputePipeline?    //!< the macaron half-step residual
var private g_pf_pso_q3a_shuffle : MetalComputePipeline? //!< the qwen3a conv feature shuffle
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
var private g_pf_pso_mm_thsk : MetalComputePipeline?      //!< the split-K stamps (ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-split-k)
var private g_pf_pso_mm_tsk : MetalComputePipeline?
var private g_pf_pso_mm_thsk16 : MetalComputePipeline?
var private g_pf_pso_mm_tsk16 : MetalComputePipeline?
var private g_pf_sk_part : MetalBuffer?    //!< the split-K partial planes, two alternated so consecutive sites do not serialize on one
var private g_pf_sk_part2 : MetalBuffer?
var private g_pf_sk_flip = false
var private g_pf_sk_bytes = 0ul
var private g_pf_sk_sites = 0l
var private g_pf_pso_half_rows : MetalComputePipeline?
var private g_pf_bxh : MetalBuffer?    //!< per-forward f16 activation scratch panel (null = the f32 path)
var private g_pf_bxh2 : MetalBuffer?
var private g_pf_bxh_flip : bool
var private g_pf_bxh_bytes : uint64     //!< acquired panel capacity — pf_cvt_panel's fit check
var private g_pf_pso_dq_q8 : MetalComputePipeline?   //!< q8 blob -> device f16 W panel (the dev-W pass)
var private g_pf_pso_dq_k4 : MetalComputePipeline?   //!< the K-quant dequant passes onto the same panel
var private g_pf_pso_dq_k5 : MetalComputePipeline?
var private g_pf_pso_dq_k6 : MetalComputePipeline?
//! the split-scale and per-32 formats' dev-W dequant passes
var private g_pf_pso_dq_kq44 : MetalComputePipeline?
var private g_pf_pso_dq_kq35 : MetalComputePipeline?
var private g_pf_pso_dq_kq45 : MetalComputePipeline?
var private g_pf_pso_dq_kq40 : MetalComputePipeline?
var private g_pf_pso_dq_kq49 : MetalComputePipeline?
var private g_pf_pso_dq_kq3 : MetalComputePipeline?
var private g_pf_pso_dq_kq33 : MetalComputePipeline?
var private g_pf_pso_dq_kq34 : MetalComputePipeline?
var private g_pf_pso_dq_kq2 : MetalComputePipeline?
var private g_pf_pso_dq_kq23 : MetalComputePipeline?
var private g_pf_pso_dq_kq24 : MetalComputePipeline?
var private g_pf_pso_dq_kq25 : MetalComputePipeline?
var private g_pf_pso_dq_kq51 : MetalComputePipeline?
var private g_pf_pso_dq_kq4n32 : MetalComputePipeline?
var private g_pf_pso_dq_kq4032 : MetalComputePipeline?
var private g_pf_pso_dq_kq5032 : MetalComputePipeline?
var private g_pf_pso_hmm_th : MetalComputePipeline?  //!< all-device half x half mul_mm reading that panel
var private g_pf_pso_hmm_thsk : MetalComputePipeline?  //!< its split-K twin, compiled under the dev-W knee
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
def private pf_devw_key(base : uint64; off : uint64; fmt : int) : uint64 =>
    (base + off) * 8ul + uint64(fmt)
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
var private g_pf_pso_bf16_mm_tsk : MetalComputePipeline?
var private g_pf_pso_bf16_mm_thsk : MetalComputePipeline?
var private g_pf_bf16_mm_tensor : bool   //!< crowned tensor twin selected (no tgmem bind)
var private g_pf_mm_tensor : bool        //!< crowned tensor twin selected for the production q8 mul_mm
var private g_pf_tensor_ok : bool        //!< the box compiles the mpp tensor kernels (probed at init)
var private g_pf_moe_mm_q8_tensor : bool //!< "moe_mulmm_q8" crowned (contiguous sites only)
var private g_pf_pso_moe_mm_q8_t : MetalComputePipeline?
var private g_pf_pso_moe_mm_q8_th : MetalComputePipeline?
var private g_pf_moe_mm_mx4_tensor : bool //!< "moe_mulmm_mx4" crowned (contiguous sites only)
var private g_pf_moe_kq_crown : bool[KQ_FMT_COUNT] //!< the routed twin verdict by format slot; false where the format has no routed twin
var private g_pf_pso_moe_mm_mx4_t : MetalComputePipeline?
var private g_pf_pso_moe_mm_mx4_th : MetalComputePipeline?
var private g_pf_pso_moe_gx : MetalComputePipeline?   //!< gather-X materialization (up/gate contiguous form)
var private g_pf_pso_moe_mm_q51_t : MetalComputePipeline?   //!< moe q5_1 twins (contiguous rows)
var private g_pf_pso_moe_mm_q51_th : MetalComputePipeline?
var private g_pf_pso_moe_mm_q51_th128 : MetalComputePipeline?
var private g_pf_pso_moe_mm_q51_thr : MetalComputePipeline?
var private g_pf_pso_moe_mm_iq4nl32_t : MetalComputePipeline?   //!< moe iq4nl32 twins (the same staging form as q51)
var private g_pf_pso_moe_mm_iq4nl32_th : MetalComputePipeline?
var private g_pf_pso_moe_mm_iq4nl32_th128 : MetalComputePipeline?
var private g_pf_pso_moe_mm_iq4nl32_thr : MetalComputePipeline?
var private g_pf_pso_moe_mm_q4032_t : MetalComputePipeline?   //!< moe q4032 twins (iq4nl32's staging under the linear code)
var private g_pf_pso_moe_mm_q4032_th : MetalComputePipeline?
var private g_pf_pso_moe_mm_q4032_th128 : MetalComputePipeline?
var private g_pf_pso_moe_mm_q4032_thr : MetalComputePipeline?
var private g_pf_pso_moe_mm_q5032_t : MetalComputePipeline?   //!< moe q5032 twins (q5_1's staging under one d, the code q - 16)
var private g_pf_pso_moe_mm_q5032_th : MetalComputePipeline?
var private g_pf_pso_moe_mm_q5032_th128 : MetalComputePipeline?
var private g_pf_pso_moe_mm_q5032_thr : MetalComputePipeline?
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
var private g_pf_pso_moe_mm_iq3s4_t : MetalComputePipeline?
var private g_pf_pso_moe_mm_iq3s4_th : MetalComputePipeline?
var private g_pf_pso_moe_mm_iq3s4_th128 : MetalComputePipeline?
var private g_pf_pso_moe_mm_iq3s4_thr : MetalComputePipeline?
var private g_pf_pso_moe_mm_q40_t : MetalComputePipeline?
var private g_pf_pso_moe_mm_q40_th : MetalComputePipeline?
var private g_pf_pso_moe_mm_q40_th128 : MetalComputePipeline?
var private g_pf_pso_moe_mm_q40_thr : MetalComputePipeline?
var private g_pf_pso_moe_mm_nv4_t : MetalComputePipeline?
var private g_pf_pso_moe_mm_nv4_th : MetalComputePipeline?
var private g_pf_pso_moe_mm_nv4_th128 : MetalComputePipeline?
var private g_pf_pso_moe_mm_nv4_thr : MetalComputePipeline?
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
var private g_pf_pso_kq_mm4_thsk : MetalComputePipeline?
var private g_pf_pso_kq_mm4_tsk : MetalComputePipeline?
var private g_pf_pso_kq_mm4_thsk16 : MetalComputePipeline?
var private g_pf_pso_kq_mm4_tsk16 : MetalComputePipeline?
var private g_pf_pso_kq_mm4_thdb128 : MetalComputePipeline?
var private g_pf_kq_mm5_tensor : bool     //!< "kq_mulmm_k5" crowned
var private g_pf_pso_kq_mm5_t : MetalComputePipeline?
var private g_pf_pso_kq_mm5_th : MetalComputePipeline?
var private g_pf_pso_kq_mm5_th128 : MetalComputePipeline?
var private g_pf_pso_kq_mm5_thdb : MetalComputePipeline?
var private g_pf_pso_kq_mm5_thsk : MetalComputePipeline?
var private g_pf_pso_kq_mm5_tsk : MetalComputePipeline?
var private g_pf_pso_kq_mm5_thsk16 : MetalComputePipeline?
var private g_pf_pso_kq_mm5_tsk16 : MetalComputePipeline?
var private g_pf_pso_kq_mm5_thdb128 : MetalComputePipeline?
var private g_pf_kq_mm6_tensor : bool     //!< "kq_mulmm_k6" crowned
var private g_pf_pso_kq_mm6_t : MetalComputePipeline?
var private g_pf_pso_kq_mm6_th : MetalComputePipeline?
var private g_pf_pso_kq_mm6_th128 : MetalComputePipeline?
var private g_pf_pso_kq_mm6_thdb : MetalComputePipeline?
var private g_pf_pso_kq_mm6_thsk : MetalComputePipeline?
var private g_pf_pso_kq_mm6_tsk : MetalComputePipeline?
var private g_pf_pso_kq_mm6_thsk16 : MetalComputePipeline?
var private g_pf_pso_kq_mm6_tsk16 : MetalComputePipeline?
var private g_pf_pso_kq_mm6_thdb128 : MetalComputePipeline?
var private g_pf_kq_mm44_tensor : bool    //!< "kq_mulmm_iq4xs" crowned
var private g_pf_pso_kq_mm44_t : MetalComputePipeline?
var private g_pf_pso_kq_mm44_th : MetalComputePipeline?
var private g_pf_kq_mm35_tensor : bool    //!< "kq_mulmm_iq3s4" crowned
var private g_pf_pso_kq_mm35_t : MetalComputePipeline?    //!< the iq3s4 tensor twins (iq4xs's kernel under the odd codebook)
var private g_pf_pso_kq_mm35_th : MetalComputePipeline?
var private g_pf_kq_mm45_tensor : bool    //!< "kq_mulmm_iq4nl" crowned
var private g_pf_pso_kq_mm45_t : MetalComputePipeline?
var private g_pf_pso_kq_mm45_th : MetalComputePipeline?
var private g_pf_kq_mm40_tensor : bool    //!< "kq_mulmm_q40" crowned
var private g_pf_pso_kq_mm40_t : MetalComputePipeline?
var private g_pf_pso_kq_mm40_th : MetalComputePipeline?
var private g_pf_kq_mm49_tensor : bool    //!< "kq_mulmm_nv4" crowned
var private g_pf_pso_kq_mm49_t : MetalComputePipeline?
var private g_pf_pso_kq_mm49_th : MetalComputePipeline?
var private g_pf_pso_kq_mm49_th128 : MetalComputePipeline?
var private g_pf_pso_kq_mm49_thdb : MetalComputePipeline?
var private g_pf_pso_kq_mm49_thsk : MetalComputePipeline?
var private g_pf_pso_kq_mm49_tsk : MetalComputePipeline?
var private g_pf_pso_kq_mm49_thsk16 : MetalComputePipeline?
var private g_pf_pso_kq_mm49_tsk16 : MetalComputePipeline?
var private g_pf_pso_kq_mm49_thdb128 : MetalComputePipeline?
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
var private g_pf_pso_kq_mm44_th128 : MetalComputePipeline?
var private g_pf_pso_kq_mm44_thdb : MetalComputePipeline?
var private g_pf_pso_kq_mm44_thsk : MetalComputePipeline?
var private g_pf_pso_kq_mm44_tsk : MetalComputePipeline?
var private g_pf_pso_kq_mm44_thsk16 : MetalComputePipeline?
var private g_pf_pso_kq_mm44_tsk16 : MetalComputePipeline?
var private g_pf_pso_kq_mm44_thdb128 : MetalComputePipeline?
var private g_pf_pso_kq_mm35_th128 : MetalComputePipeline?
var private g_pf_pso_kq_mm35_thdb : MetalComputePipeline?
var private g_pf_pso_kq_mm35_thsk : MetalComputePipeline?
var private g_pf_pso_kq_mm35_tsk : MetalComputePipeline?
var private g_pf_pso_kq_mm35_thsk16 : MetalComputePipeline?
var private g_pf_pso_kq_mm35_tsk16 : MetalComputePipeline?
var private g_pf_pso_kq_mm35_thdb128 : MetalComputePipeline?
var private g_pf_pso_kq_mm45_th128 : MetalComputePipeline?
var private g_pf_pso_kq_mm45_thdb : MetalComputePipeline?
var private g_pf_pso_kq_mm45_thsk : MetalComputePipeline?
var private g_pf_pso_kq_mm45_tsk : MetalComputePipeline?
var private g_pf_pso_kq_mm45_thsk16 : MetalComputePipeline?
var private g_pf_pso_kq_mm45_tsk16 : MetalComputePipeline?
var private g_pf_pso_kq_mm45_thdb128 : MetalComputePipeline?
var private g_pf_pso_kq_mm40_th128 : MetalComputePipeline?
var private g_pf_pso_kq_mm40_thdb : MetalComputePipeline?
var private g_pf_pso_kq_mm40_thsk : MetalComputePipeline?
var private g_pf_pso_kq_mm40_tsk : MetalComputePipeline?
var private g_pf_pso_kq_mm40_thsk16 : MetalComputePipeline?
var private g_pf_pso_kq_mm40_tsk16 : MetalComputePipeline?
var private g_pf_pso_kq_mm40_thdb128 : MetalComputePipeline?
var private g_pf_pso_kq_mm3_th128 : MetalComputePipeline?
var private g_pf_pso_kq_mm3_thdb : MetalComputePipeline?
var private g_pf_pso_kq_mm3_thsk : MetalComputePipeline?
var private g_pf_pso_kq_mm3_tsk : MetalComputePipeline?
var private g_pf_pso_kq_mm3_thsk16 : MetalComputePipeline?
var private g_pf_pso_kq_mm3_tsk16 : MetalComputePipeline?
var private g_pf_pso_kq_mm3_thdb128 : MetalComputePipeline?
var private g_pf_pso_kq_mm2_th128 : MetalComputePipeline?
var private g_pf_pso_kq_mm2_thdb : MetalComputePipeline?
var private g_pf_pso_kq_mm2_thsk : MetalComputePipeline?
var private g_pf_pso_kq_mm2_tsk : MetalComputePipeline?
var private g_pf_pso_kq_mm2_thsk16 : MetalComputePipeline?
var private g_pf_pso_kq_mm2_tsk16 : MetalComputePipeline?
var private g_pf_pso_kq_mm2_thdb128 : MetalComputePipeline?
var private g_pf_pso_kq_mm33_thdb : MetalComputePipeline?
var private g_pf_pso_kq_mm33_thsk : MetalComputePipeline?
var private g_pf_pso_kq_mm33_tsk : MetalComputePipeline?
var private g_pf_pso_kq_mm33_thsk16 : MetalComputePipeline?
var private g_pf_pso_kq_mm33_tsk16 : MetalComputePipeline?
var private g_pf_pso_kq_mm33_thdb128 : MetalComputePipeline?
var private g_pf_pso_kq_mm34_thdb : MetalComputePipeline?
var private g_pf_pso_kq_mm34_thsk : MetalComputePipeline?
var private g_pf_pso_kq_mm34_tsk : MetalComputePipeline?
var private g_pf_pso_kq_mm34_thsk16 : MetalComputePipeline?
var private g_pf_pso_kq_mm34_tsk16 : MetalComputePipeline?
var private g_pf_pso_kq_mm34_thdb128 : MetalComputePipeline?
var private g_pf_pso_kq_mm23_th128 : MetalComputePipeline?
var private g_pf_pso_kq_mm23_thdb : MetalComputePipeline?
var private g_pf_pso_kq_mm23_thsk : MetalComputePipeline?
var private g_pf_pso_kq_mm23_tsk : MetalComputePipeline?
var private g_pf_pso_kq_mm23_thsk16 : MetalComputePipeline?
var private g_pf_pso_kq_mm23_tsk16 : MetalComputePipeline?
var private g_pf_pso_kq_mm23_thdb128 : MetalComputePipeline?
var private g_pf_pso_kq_mm24_th128 : MetalComputePipeline?
var private g_pf_pso_kq_mm24_thdb : MetalComputePipeline?
var private g_pf_pso_kq_mm24_thsk : MetalComputePipeline?
var private g_pf_pso_kq_mm24_tsk : MetalComputePipeline?
var private g_pf_pso_kq_mm24_thsk16 : MetalComputePipeline?
var private g_pf_pso_kq_mm24_tsk16 : MetalComputePipeline?
var private g_pf_pso_kq_mm24_thdb128 : MetalComputePipeline?
var private g_pf_pso_kq_mm25_th128 : MetalComputePipeline?
var private g_pf_pso_kq_mm25_thdb : MetalComputePipeline?
var private g_pf_pso_kq_mm25_thsk : MetalComputePipeline?
var private g_pf_pso_kq_mm25_tsk : MetalComputePipeline?
var private g_pf_pso_kq_mm25_thsk16 : MetalComputePipeline?
var private g_pf_pso_kq_mm25_tsk16 : MetalComputePipeline?
var private g_pf_pso_kq_mm25_thdb128 : MetalComputePipeline?
var private g_pf_kq_mm51_tensor : bool    //!< "kq_mulmm_q51" crowned
var private g_pf_pso_kq_mm51_t : MetalComputePipeline?
var private g_pf_pso_kq_mm51_th : MetalComputePipeline?
var private g_pf_pso_kq_mm51_th128 : MetalComputePipeline?
var private g_pf_pso_kq_mm51_thdb : MetalComputePipeline?
var private g_pf_pso_kq_mm51_thsk : MetalComputePipeline?
var private g_pf_pso_kq_mm51_tsk : MetalComputePipeline?
var private g_pf_pso_kq_mm51_thsk16 : MetalComputePipeline?
var private g_pf_pso_kq_mm51_tsk16 : MetalComputePipeline?
var private g_pf_pso_kq_mm51_thdb128 : MetalComputePipeline?
var private g_pf_kq_mm4n32_tensor : bool    //!< "kq_mulmm_iq4nl32" crowned
var private g_pf_pso_kq_mm4n32_t : MetalComputePipeline?
var private g_pf_pso_kq_mm4n32_th : MetalComputePipeline?
var private g_pf_pso_kq_mm4n32_th128 : MetalComputePipeline?
var private g_pf_pso_kq_mm4n32_thdb : MetalComputePipeline?
var private g_pf_pso_kq_mm4n32_thsk : MetalComputePipeline?
var private g_pf_pso_kq_mm4n32_tsk : MetalComputePipeline?
var private g_pf_pso_kq_mm4n32_thsk16 : MetalComputePipeline?
var private g_pf_pso_kq_mm4n32_tsk16 : MetalComputePipeline?
var private g_pf_pso_kq_mm4n32_thdb128 : MetalComputePipeline?
var private g_pf_kq_mm4032_tensor : bool    //!< "kq_mulmm_q4032" crowned
var private g_pf_pso_kq_mm4032_t : MetalComputePipeline?
var private g_pf_pso_kq_mm4032_th : MetalComputePipeline?
var private g_pf_pso_kq_mm4032_th128 : MetalComputePipeline?
var private g_pf_pso_kq_mm4032_thdb : MetalComputePipeline?
var private g_pf_pso_kq_mm4032_thsk : MetalComputePipeline?
var private g_pf_pso_kq_mm4032_tsk : MetalComputePipeline?
var private g_pf_pso_kq_mm4032_thsk16 : MetalComputePipeline?
var private g_pf_pso_kq_mm4032_tsk16 : MetalComputePipeline?
var private g_pf_pso_kq_mm4032_thdb128 : MetalComputePipeline?
var private g_pf_kq_mm5032_tensor : bool    //!< "kq_mulmm_q5032" crowned
var private g_pf_pso_kq_mm5032_t : MetalComputePipeline?
var private g_pf_pso_kq_mm5032_th : MetalComputePipeline?
var private g_pf_pso_kq_mm5032_th128 : MetalComputePipeline?
var private g_pf_pso_kq_mm5032_thdb : MetalComputePipeline?
var private g_pf_pso_kq_mm5032_thsk : MetalComputePipeline?
var private g_pf_pso_kq_mm5032_tsk : MetalComputePipeline?
var private g_pf_pso_kq_mm5032_thsk16 : MetalComputePipeline?
var private g_pf_pso_kq_mm5032_tsk16 : MetalComputePipeline?
var private g_pf_pso_kq_mm5032_thdb128 : MetalComputePipeline?
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
var private g_pf_pso_qkmm : MetalComputePipeline?
var private g_pf_pso_avmm : MetalComputePipeline?
var private g_pf_pso_moe_count : MetalComputePipeline?
var private g_pf_pso_moe_bucket : MetalComputePipeline?
var private g_pf_pso_moe_reduce : MetalComputePipeline?
var private g_pf_pso_moe_reduce_sh : MetalComputePipeline?
var private g_pf_pso_moe_mm_q8 : MetalComputePipeline?
var private g_pf_pso_moe_mm_k4 : MetalComputePipeline?
var private g_pf_pso_moe_mm_k5 : MetalComputePipeline?
var private g_pf_pso_moe_mm_k6 : MetalComputePipeline?
var private g_pf_pso_swiglu_oai : MetalComputePipeline?
var private g_pf_pso_moe_mm_mx4 : MetalComputePipeline?
var private g_pf_pso_moe_mm_q51 : MetalComputePipeline?
var private g_pf_pso_moe_mm_iq4nl32 : MetalComputePipeline?
var private g_pf_pso_moe_mm_q4032 : MetalComputePipeline?
var private g_pf_pso_moe_mm_q5032 : MetalComputePipeline?
var private g_pf_pso_moe_mm_k3 : MetalComputePipeline?
var private g_pf_pso_moe_mm_k2 : MetalComputePipeline?
var private g_pf_pso_moe_mm_iq4xs : MetalComputePipeline?
var private g_pf_pso_moe_mm_iq3s4 : MetalComputePipeline?
var private g_pf_pso_moe_mm_iq4nl : MetalComputePipeline?
var private g_pf_pso_moe_mm_q40 : MetalComputePipeline?
var private g_pf_pso_moe_mm_nv4 : MetalComputePipeline?
var private g_pf_pso_moe_mm_iq3s : MetalComputePipeline?
var private g_pf_pso_moe_mm_iq3xxs : MetalComputePipeline?
var private g_pf_pso_moe_mm_iq2s : MetalComputePipeline?
var private g_pf_pso_moe_mm_iq2xs : MetalComputePipeline?
var private g_pf_pso_moe_mm_iq2xxs : MetalComputePipeline?
var private g_pf_pso_moe_mm_k2_t : MetalComputePipeline?
var private g_pf_pso_moe_mm_k2_th : MetalComputePipeline?
var private g_pf_pso_moe_mm_k2_th128 : MetalComputePipeline?
var private g_pf_pso_moe_mm_k2_thr : MetalComputePipeline?
var private g_pf_pso_moe_mm_iq4nl_t : MetalComputePipeline?
var private g_pf_pso_moe_mm_iq4nl_th : MetalComputePipeline?
var private g_pf_pso_moe_mm_iq4nl_th128 : MetalComputePipeline?
var private g_pf_pso_moe_mm_iq4nl_thr : MetalComputePipeline?
var private g_mm_legacy = false             //!< set_metal_prefill_mulmm_legacy — tests' exact-parity rail
var private g_pf_logits_cpu = false            //!< set_metal_prefill_logits_cpu — the logits A/B rail
//! Timing-attribution knockout: DASLLAMA_METAL_PREFILL_SKIP / set_metal_prefill_skip names one
//! stage family to leave unencoded. Output is GARBAGE while non-empty; the family names are the
//! g_pf_skip comparisons below.
var private g_pf_skip = ""

//! an "only" family keeps the routing and drops every stage downstream, so it knocks out this one too
def private pf_skip_moe_stage(stage : string) : bool =>
    (g_pf_skip == stage || g_pf_skip == "moe_only_route" || g_pf_skip == "moe_only_router" || g_pf_skip == "moe_only_sel")

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
    if (v != g_pf_env_mm_tail) {
        to_log(LOG_WARNING, "dasLLAMA metal prefill: set_metal_prefill_mm_tail({v}) - {v ? "small remainders and K-quant sites of eight rows or fewer peel onto the GEMV forms" : "small npos % 32 remainders bill the padded tile; the last layer's narrowed FFN row keeps its one-row form (DASLLAMA_METAL_LASTROW=0 for the full panel)"} (numerics move with the form)\n")
    }
    g_pf_env_mm_tail = v
}

def public get_metal_prefill_mm_tail : bool => g_pf_env_mm_tail

var private g_pf_split_k_on = true

//! A/B seat: the short window's split-K tiles (`ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-split-k`) vs the whole-K tiles - a
//! one-process twin of `DASLLAMA_METAL_SPLIT_K=0` for a driver whose stamps compiled; default on. The forms differ in the f32
//! summation order alone.
def public set_metal_prefill_split_k(v : bool) {
    if (v != g_pf_split_k_on) {
        to_log(LOG_WARNING, "dasLLAMA metal prefill: set_metal_prefill_split_k({v}) - {v ? "short-window tiles split their K under the knee" : "every tile walks its whole K"} (numerics move with the form)\n")
    }
    g_pf_split_k_on = v
}

def public get_metal_prefill_split_k : bool => g_pf_split_k_on

//! The sites the split-K form served since load - the knee's anti-vacuous witness
def public metal_prefill_split_k_sites : int64 => g_pf_sk_sites

//! A/B seat: an f16-KV prefill's rows in the session's KV mirror, its attention reading them there, vs the
//! f32 panels and the continuation's host gather - `DASLLAMA_METAL_PF_MIRROR`'s setter twin. Default on.
def public set_metal_prefill_mirror(v : bool) {
    g_pf_env_mirror = v
}

def public get_metal_prefill_mirror() : bool => g_pf_env_mirror

var private g_pf_mirror_served = 0l

//! Lifetime prefill windows served on the mirror-fed form - the form's witness.
def public metal_prefill_mirror_served : int64 => g_pf_mirror_served

var private g_pf_mirror_panel_served = 0l

//! Lifetime prefill windows whose panel form fed the mirror - that form's witness.
def public metal_prefill_mirror_panel_served : int64 => g_pf_mirror_panel_served

var private g_pf_seg_windows = 0l

//! Lifetime prefill windows that held several sessions' segments - the segmented window's witness.
def public metal_prefill_segment_windows : int64 => g_pf_seg_windows

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

def public get_metal_prefill_dev_w : bool => g_pf_env_dev_w

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

def public get_metal_prefill_lastrow : bool => g_pf_env_lastrow

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

var private g_pf_last_logits = 0   //! the last served window's classifier: 0 the CPU step after it, 1 on the GPU, 2 none (hidden-only)
var private g_pf_last_kv = 0       //! ... and its K/V form: 0 panels, 1 panels feeding the mirror, 2 mirror-fed

//! The last served window's forms, as its stage line spells them: `logits` gpu / cpu / none and `kv` panels / panels+mirror /
//! mirror - a hidden-only window reads none and panels
[arch(at="../ARCHITECTURE_GPU_PREFILL.md#prefill-hidden-only")]
def public metal_prefill_window_forms() : tuple<logits : string; kv : string> {
    let lg = g_pf_last_logits == 1 ? "gpu" : (g_pf_last_logits == 2 ? "none" : "cpu")
    let kv = g_pf_last_kv == 2 ? "mirror" : (g_pf_last_kv == 1 ? "panels+mirror" : "panels")
    return (logits = lg, kv = kv)
}

//! (gpu prefills served, declines to the CPU path) since load — diagnostics / tests. The decline
//! machinery lives in dasllama_metal_common (W1 split), so the count reads from there.
def public metal_prefill_stats() : tuple<prefills : int64; declines : int64> =>
    (prefills = g_prefills, declines = g_prefill_declines.total)

//! The tower driver's borrow seat: full prefill PSO bring-up, idempotent — the borrowed builders
//! (pf_enc_bf16_mm) bind prefill-owned PSOs.
def public metal_prefill_pso_init : bool => metal_prefill_init()

//! the split-K stamps and their partial-plane scratch
def private pf_release_split_k {
    release_handles(g_pf_pso_mm_thsk, g_pf_pso_mm_tsk, g_pf_pso_bf16_mm_tsk, g_pf_pso_bf16_mm_thsk)
    release_handles(g_pf_pso_kq_mm4_thsk, g_pf_pso_kq_mm4_tsk, g_pf_pso_kq_mm5_thsk, g_pf_pso_kq_mm5_tsk, g_pf_pso_kq_mm6_thsk, g_pf_pso_kq_mm6_tsk)
    release_handles(g_pf_pso_kq_mm44_thsk, g_pf_pso_kq_mm44_tsk, g_pf_pso_kq_mm35_thsk, g_pf_pso_kq_mm35_tsk, g_pf_pso_kq_mm45_thsk, g_pf_pso_kq_mm45_tsk)
    release_handles(g_pf_pso_kq_mm40_thsk, g_pf_pso_kq_mm40_tsk, g_pf_pso_kq_mm49_thsk, g_pf_pso_kq_mm49_tsk, g_pf_pso_kq_mm3_thsk, g_pf_pso_kq_mm3_tsk)
    release_handles(g_pf_pso_kq_mm2_thsk, g_pf_pso_kq_mm2_tsk, g_pf_pso_kq_mm33_thsk, g_pf_pso_kq_mm33_tsk, g_pf_pso_kq_mm34_thsk, g_pf_pso_kq_mm34_tsk)
    release_handles(g_pf_pso_kq_mm23_thsk, g_pf_pso_kq_mm23_tsk, g_pf_pso_kq_mm24_thsk, g_pf_pso_kq_mm24_tsk, g_pf_pso_kq_mm25_thsk, g_pf_pso_kq_mm25_tsk)
    release_handles(g_pf_pso_kq_mm51_thsk, g_pf_pso_kq_mm51_tsk, g_pf_pso_kq_mm4n32_thsk, g_pf_pso_kq_mm4n32_tsk, g_pf_pso_kq_mm4032_thsk, g_pf_pso_kq_mm4032_tsk)
    release_handles(g_pf_pso_kq_mm5032_thsk, g_pf_pso_kq_mm5032_tsk)
    release_handles(g_pf_pso_kq_mm4_thsk16, g_pf_pso_kq_mm4_tsk16, g_pf_pso_kq_mm5_thsk16, g_pf_pso_kq_mm5_tsk16, g_pf_pso_kq_mm6_thsk16, g_pf_pso_kq_mm6_tsk16)
    release_handles(g_pf_pso_kq_mm44_thsk16, g_pf_pso_kq_mm44_tsk16, g_pf_pso_kq_mm35_thsk16, g_pf_pso_kq_mm35_tsk16, g_pf_pso_kq_mm45_thsk16, g_pf_pso_kq_mm45_tsk16)
    release_handles(g_pf_pso_kq_mm40_thsk16, g_pf_pso_kq_mm40_tsk16, g_pf_pso_kq_mm49_thsk16, g_pf_pso_kq_mm49_tsk16, g_pf_pso_kq_mm3_thsk16, g_pf_pso_kq_mm3_tsk16)
    release_handles(g_pf_pso_kq_mm2_thsk16, g_pf_pso_kq_mm2_tsk16, g_pf_pso_kq_mm33_thsk16, g_pf_pso_kq_mm33_tsk16, g_pf_pso_kq_mm34_thsk16, g_pf_pso_kq_mm34_tsk16)
    release_handles(g_pf_pso_kq_mm23_thsk16, g_pf_pso_kq_mm23_tsk16, g_pf_pso_kq_mm24_thsk16, g_pf_pso_kq_mm24_tsk16, g_pf_pso_kq_mm25_thsk16, g_pf_pso_kq_mm25_tsk16)
    release_handles(g_pf_pso_kq_mm51_thsk16, g_pf_pso_kq_mm51_tsk16, g_pf_pso_kq_mm4n32_thsk16, g_pf_pso_kq_mm4n32_tsk16, g_pf_pso_kq_mm4032_thsk16, g_pf_pso_kq_mm4032_tsk16)
    release_handles(g_pf_pso_kq_mm5032_thsk16, g_pf_pso_kq_mm5032_tsk16)
    release_handles(g_pf_pso_mm_thsk16, g_pf_pso_mm_tsk16)
    release_handles(g_pf_sk_part, g_pf_sk_part2)
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
    release_handles(g_pf_pso_rope, g_pf_pso_qkr, g_pf_pso_qkr_hx, g_pf_pso_half_rows)
    release_handles(g_pf_pso_qk, g_pf_pso_rowstat, g_pf_pso_av, g_pf_pso_qkmm, g_pf_pso_qkmm_t, g_pf_pso_qkmm_th)
    release_handles(g_pf_pso_avmm, g_pf_pso_avmm_p, g_pf_pso_avmm_t, g_pf_pso_avmm_tp)
    release_handles(g_pf_pso_qk_dev, g_pf_pso_qk_dev128, g_pf_pso_pexp, g_pf_pso_av_dev, g_pf_pso_av_dev128)
    release_handles(g_pf_pso_av_dev_t, g_pf_pso_av_dev128_t)
    release_handles(g_pf_pso_fc_twin, g_pf_pso_fc_pexp)
    release_handles(g_pf_pso_mm_t, g_pf_pso_mm_th, g_pf_pso_mm_t128, g_pf_pso_mm_th128, g_pf_pso_mm_thdb, g_pf_pso_mm_thdb128)
    pf_release_split_k()
    release_handles(g_pf_pso_bf16_mm, g_pf_pso_bf16_mm_th, g_pf_pso_bf16_mm_t128, g_pf_pso_bf16_mm_th128)
    release_handles(g_pf_pso_hmm_th, g_pf_pso_hmm_th128, g_pf_pso_hmm_th128_l1, g_pf_pso_hmm_thsk)
    release_handles(g_pf_pso_kq_mm4_t, g_pf_pso_kq_mm4_th, g_pf_pso_kq_mm4_th128, g_pf_pso_kq_mm4_thdb, g_pf_pso_kq_mm4_thdb128)
    release_handles(g_pf_pso_kq_mm5_t, g_pf_pso_kq_mm5_th, g_pf_pso_kq_mm5_th128, g_pf_pso_kq_mm5_thdb, g_pf_pso_kq_mm5_thdb128)
    release_handles(g_pf_pso_kq_mm6_t, g_pf_pso_kq_mm6_th, g_pf_pso_kq_mm6_th128, g_pf_pso_kq_mm6_thdb, g_pf_pso_kq_mm6_thdb128)
    release_handles(g_pf_pso_kq_mm44_t, g_pf_pso_kq_mm44_th, g_pf_pso_kq_mm45_t, g_pf_pso_kq_mm45_th, g_pf_pso_kq_mm40_t, g_pf_pso_kq_mm40_th)
    release_handles(g_pf_pso_kq_mm49_t, g_pf_pso_kq_mm49_th, g_pf_pso_kq_mm49_th128, g_pf_pso_kq_mm49_thdb, g_pf_pso_kq_mm49_thdb128, g_pf_pso_dq_kq49)
    release_handles(g_pf_pso_moe_mm_nv4, g_pf_pso_moe_mm_nv4_t, g_pf_pso_moe_mm_nv4_th, g_pf_pso_moe_mm_nv4_th128, g_pf_pso_moe_mm_nv4_thr)
    release_handles(g_pf_pso_kq_mm35_t, g_pf_pso_kq_mm35_th, g_pf_pso_moe_mm_iq3s4)
    release_handles(g_pf_pso_moe_mm_iq3s4_t, g_pf_pso_moe_mm_iq3s4_th, g_pf_pso_moe_mm_iq3s4_th128, g_pf_pso_moe_mm_iq3s4_thr)
    release_handles(g_pf_pso_kq_mm3_t, g_pf_pso_kq_mm3_th, g_pf_pso_kq_mm2_t, g_pf_pso_kq_mm2_th)
    release_handles(g_pf_pso_kq_mm33_t, g_pf_pso_kq_mm33_th, g_pf_pso_kq_mm33_th128, g_pf_pso_kq_mm34_t, g_pf_pso_kq_mm34_th, g_pf_pso_kq_mm34_th128)
    release_handles(g_pf_pso_kq_mm23_t, g_pf_pso_kq_mm23_th, g_pf_pso_kq_mm24_t, g_pf_pso_kq_mm24_th, g_pf_pso_kq_mm25_t, g_pf_pso_kq_mm25_th)
    release_handles(g_pf_pso_kq_mm44_th128, g_pf_pso_kq_mm44_thdb, g_pf_pso_kq_mm44_thdb128, g_pf_pso_kq_mm35_th128, g_pf_pso_kq_mm35_thdb, g_pf_pso_kq_mm35_thdb128, g_pf_pso_kq_mm45_th128, g_pf_pso_kq_mm45_thdb, g_pf_pso_kq_mm45_thdb128)
    release_handles(g_pf_pso_kq_mm40_th128, g_pf_pso_kq_mm40_thdb, g_pf_pso_kq_mm40_thdb128, g_pf_pso_kq_mm3_th128, g_pf_pso_kq_mm3_thdb, g_pf_pso_kq_mm3_thdb128, g_pf_pso_kq_mm2_th128, g_pf_pso_kq_mm2_thdb, g_pf_pso_kq_mm2_thdb128)
    release_handles(g_pf_pso_kq_mm33_thdb, g_pf_pso_kq_mm33_thdb128, g_pf_pso_kq_mm34_thdb, g_pf_pso_kq_mm34_thdb128)
    release_handles(g_pf_pso_kq_mm23_th128, g_pf_pso_kq_mm23_thdb, g_pf_pso_kq_mm23_thdb128, g_pf_pso_kq_mm24_th128, g_pf_pso_kq_mm24_thdb, g_pf_pso_kq_mm24_thdb128, g_pf_pso_kq_mm25_th128, g_pf_pso_kq_mm25_thdb, g_pf_pso_kq_mm25_thdb128)
    release_handles(g_pf_pso_kq_mm51_t, g_pf_pso_kq_mm51_th, g_pf_pso_kq_mm51_th128, g_pf_pso_kq_mm51_thdb, g_pf_pso_kq_mm51_thdb128)
    release_handles(g_pf_pso_kq_mm4n32_t, g_pf_pso_kq_mm4n32_th, g_pf_pso_kq_mm4n32_th128, g_pf_pso_kq_mm4n32_thdb, g_pf_pso_kq_mm4n32_thdb128)
    release_handles(g_pf_pso_kq_mm4032_t, g_pf_pso_kq_mm4032_th, g_pf_pso_kq_mm4032_th128, g_pf_pso_kq_mm4032_thdb, g_pf_pso_kq_mm4032_thdb128)
    release_handles(g_pf_pso_kq_mm5032_t, g_pf_pso_kq_mm5032_th, g_pf_pso_kq_mm5032_th128, g_pf_pso_kq_mm5032_thdb, g_pf_pso_kq_mm5032_thdb128)
    release_handles(g_pf_pso_dq_kq35, g_pf_pso_dq_kq51, g_pf_pso_dq_kq4n32, g_pf_pso_dq_kq4032, g_pf_pso_dq_kq5032)
    release_handles(g_pf_pso_dq_q8, g_pf_pso_dq_k4, g_pf_pso_dq_k5, g_pf_pso_dq_k6)
    release_handles(g_pf_pso_dq_kq44, g_pf_pso_dq_kq45, g_pf_pso_dq_kq40, g_pf_pso_dq_kq3, g_pf_pso_dq_kq2)
    release_handles(g_pf_pso_dq_kq33, g_pf_pso_dq_kq34, g_pf_pso_dq_kq23, g_pf_pso_dq_kq24, g_pf_pso_dq_kq25)
    release_handles(g_pf_pso_moe_count, g_pf_pso_moe_bucket, g_pf_pso_moe_reduce, g_pf_pso_moe_gx, g_pf_pso_swiglu_oai)
    release_handles(g_pf_pso_moe_reduce_sh)
    release_handles(g_pf_pso_moe_mm_q8, g_pf_pso_moe_mm_k4, g_pf_pso_moe_mm_k5, g_pf_pso_moe_mm_k6, g_pf_pso_moe_mm_mx4, g_pf_pso_moe_mm_q51, g_pf_pso_moe_mm_iq4nl32)
    release_handles(g_pf_pso_moe_mm_k3, g_pf_pso_moe_mm_k2, g_pf_pso_moe_mm_iq4xs, g_pf_pso_moe_mm_iq4nl, g_pf_pso_moe_mm_q40)
    release_handles(g_pf_pso_moe_mm_iq3s, g_pf_pso_moe_mm_iq3xxs, g_pf_pso_moe_mm_iq2s, g_pf_pso_moe_mm_iq2xs, g_pf_pso_moe_mm_iq2xxs)
    release_handles(g_pf_pso_moe_mm_k2_t, g_pf_pso_moe_mm_k2_th, g_pf_pso_moe_mm_k2_th128, g_pf_pso_moe_mm_k2_thr)
    release_handles(g_pf_pso_moe_mm_iq4nl_t, g_pf_pso_moe_mm_iq4nl_th, g_pf_pso_moe_mm_iq4nl_th128, g_pf_pso_moe_mm_iq4nl_thr)
    release_handles(g_pf_pso_moe_mm_q8_t, g_pf_pso_moe_mm_q8_th, g_pf_pso_moe_mm_q8_th128, g_pf_pso_moe_mm_q8_thr)
    release_handles(g_pf_pso_moe_mm_mx4_t, g_pf_pso_moe_mm_mx4_th, g_pf_pso_moe_mm_mx4_th128, g_pf_pso_moe_mm_mx4_thr)
    release_handles(g_pf_pso_moe_mm_k4_t, g_pf_pso_moe_mm_k4_th, g_pf_pso_moe_mm_k4_th128, g_pf_pso_moe_mm_k4_thr)
    release_handles(g_pf_pso_moe_mm_k5_t, g_pf_pso_moe_mm_k5_th, g_pf_pso_moe_mm_k5_th128, g_pf_pso_moe_mm_k5_thr)
    release_handles(g_pf_pso_moe_mm_k6_t, g_pf_pso_moe_mm_k6_th, g_pf_pso_moe_mm_k6_th128, g_pf_pso_moe_mm_k6_thr)
    release_handles(g_pf_pso_moe_mm_q51_t, g_pf_pso_moe_mm_q51_th, g_pf_pso_moe_mm_q51_th128, g_pf_pso_moe_mm_q51_thr)
    release_handles(g_pf_pso_moe_mm_iq4nl32_t, g_pf_pso_moe_mm_iq4nl32_th, g_pf_pso_moe_mm_iq4nl32_th128, g_pf_pso_moe_mm_iq4nl32_thr)
    release_handles(g_pf_pso_moe_mm_q4032_t, g_pf_pso_moe_mm_q4032_th, g_pf_pso_moe_mm_q4032_th128, g_pf_pso_moe_mm_q4032_thr, g_pf_pso_moe_mm_q4032)
    release_handles(g_pf_pso_moe_mm_q5032_t, g_pf_pso_moe_mm_q5032_th, g_pf_pso_moe_mm_q5032_th128, g_pf_pso_moe_mm_q5032_thr, g_pf_pso_moe_mm_q5032)
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
    release_handles(g_pf_pso_pk_dw2d, g_pf_pso_pk_xf, g_pf_pso_ga_dw, g_pf_pso_ga_dwh)
    release_handles(g_pf_pso_vad_enc, g_pf_pso_vad_lstm)
    release_handles(g_pf_pso_ga_frames, g_pf_pso_ga_mel)
    release_handles(g_pf_pso_cn_dw2d, g_pf_pso_tw_rel_sincos)
    pf_rnnt_release()
    pf_ctc_pick_release()
    release_handles(g_pf_pso_cn_melnorm, g_pf_pso_tw_pool2, g_pf_pso_tw_swiglu_rows, g_pf_pso_tw_pool2d)
    release_handle(g_pf_pso_tw_affine)
    release_handles(g_pf_pso_g4a_attn, g_pf_pso_g4a_glu, g_pf_pso_g4a_dw, g_pf_pso_g4a_axpy)
    release_handles(g_pf_pso_g4a_mag, g_pf_pso_g4a_logmel, g_pf_pso_g4a_lnrelu)
    release_handles(g_pf_pso_q3a_shuffle, g_pf_pso_q3a_pow, g_pf_pso_q3a_logmel_t)
    g_pf_inited = false
    g_pf_failed = false
}

//! the format's dense `kq_mulmm_<fmt>` verdict
def private pf_kq_dense_crown(fmt : KqFmt) : bool => pf_kq_dense_stamps(fmt).crown

[cold_path]   //! the moe kq twins compile behind the toolchain probe; their own MoE-shaped crowns decide the whole-tile shape, the knee the short one
def private pf_compile_moe_kq_twins(var ok : bool&) {
    //! a sidecar minted before the MoE-shaped races carries no verdict for them, and the section keeps winners alone - the dense crown stands in until the box re-races
    for (i in range(KQ_FMT_COUNT)) {
        let f = kq_fmt_of_slot(i)
        g_pf_moe_kq_crown[i] = (kq_sb(f) || kq_dense_b32(f)) && (metal_tensor_crowned("moe_mulmm_{f}") || pf_kq_dense_crown(f))
    }
    pf_compile_moe_b32_twins(ok)
    if (g_pf_tensor_ok) {
        g_pf_pso_moe_mm_k4_t = compile_stamp(MetalMoeMulMmK4T_metal_moe_mulmm_k45_t_msl, ok)
        g_pf_pso_moe_mm_k4_th = compile_stamp(MetalMoeMulMmK4TH_metal_moe_mulmm_k45_t_msl, ok)
        g_pf_pso_moe_mm_k4_th128 = compile_stamp(MetalMoeMulMmK4TH128_metal_moe_mulmm_k45_t_msl, ok)
        g_pf_pso_moe_mm_k4_thr = compile_stamp(MetalMoeMulMmK4THR_metal_moe_mulmm_k45_t_msl, ok)
        g_pf_pso_moe_mm_k5_t = compile_stamp(MetalMoeMulMmK5T_metal_moe_mulmm_k45_t_msl, ok)
        g_pf_pso_moe_mm_k5_th = compile_stamp(MetalMoeMulMmK5TH_metal_moe_mulmm_k45_t_msl, ok)
        g_pf_pso_moe_mm_k5_th128 = compile_stamp(MetalMoeMulMmK5TH128_metal_moe_mulmm_k45_t_msl, ok)
        g_pf_pso_moe_mm_k5_thr = compile_stamp(MetalMoeMulMmK5THR_metal_moe_mulmm_k45_t_msl, ok)
        g_pf_pso_moe_mm_k6_t = compile_stamp(MetalMoeMulMmK6T_metal_moe_mulmm_k6_t_msl, ok)
        g_pf_pso_moe_mm_k6_th = compile_stamp(MetalMoeMulMmK6TH_metal_moe_mulmm_k6_t_msl, ok)
        g_pf_pso_moe_mm_k6_th128 = compile_stamp(MetalMoeMulMmK6TH128_metal_moe_mulmm_k6_t_msl, ok)
        g_pf_pso_moe_mm_k6_thr = compile_stamp(MetalMoeMulMmK6THR_metal_moe_mulmm_k6_t_msl, ok)
    }
    pf_compile_moe_sb_twins(ok)
}

[cold_path]   //! the per-32 moe twins, behind the toolchain probe like every format's; each races its own `moe_mulmm_<fmt>` verdict
def private pf_compile_moe_b32_twins(var ok : bool&) {
    if (g_pf_tensor_ok) {
        g_pf_pso_moe_mm_q51_t = compile_stamp(MetalMoeMulMmQ51T_metal_moe_mulmm_b32_t_msl, ok)
        g_pf_pso_moe_mm_q51_th = compile_stamp(MetalMoeMulMmQ51TH_metal_moe_mulmm_b32_t_msl, ok)
        g_pf_pso_moe_mm_q51_th128 = compile_stamp(MetalMoeMulMmQ51TH128_metal_moe_mulmm_b32_t_msl, ok)
        g_pf_pso_moe_mm_q51_thr = compile_stamp(MetalMoeMulMmQ51THR_metal_moe_mulmm_b32_t_msl, ok)
        g_pf_pso_moe_mm_iq4nl32_t = compile_stamp(MetalMoeMulMmIq4nl32T_metal_moe_mulmm_b32_t_msl, ok)
        g_pf_pso_moe_mm_iq4nl32_th = compile_stamp(MetalMoeMulMmIq4nl32TH_metal_moe_mulmm_b32_t_msl, ok)
        g_pf_pso_moe_mm_iq4nl32_th128 = compile_stamp(MetalMoeMulMmIq4nl32TH128_metal_moe_mulmm_b32_t_msl, ok)
        g_pf_pso_moe_mm_iq4nl32_thr = compile_stamp(MetalMoeMulMmIq4nl32THR_metal_moe_mulmm_b32_t_msl, ok)
        g_pf_pso_moe_mm_q4032_t = compile_stamp(MetalMoeMulMmQ4032T_metal_moe_mulmm_b32_t_msl, ok)
        g_pf_pso_moe_mm_q4032_th = compile_stamp(MetalMoeMulMmQ4032TH_metal_moe_mulmm_b32_t_msl, ok)
        g_pf_pso_moe_mm_q4032_th128 = compile_stamp(MetalMoeMulMmQ4032TH128_metal_moe_mulmm_b32_t_msl, ok)
        g_pf_pso_moe_mm_q4032_thr = compile_stamp(MetalMoeMulMmQ4032THR_metal_moe_mulmm_b32_t_msl, ok)
        g_pf_pso_moe_mm_q5032_t = compile_stamp(MetalMoeMulMmQ5032T_metal_moe_mulmm_b32_t_msl, ok)
        g_pf_pso_moe_mm_q5032_th = compile_stamp(MetalMoeMulMmQ5032TH_metal_moe_mulmm_b32_t_msl, ok)
        g_pf_pso_moe_mm_q5032_th128 = compile_stamp(MetalMoeMulMmQ5032TH128_metal_moe_mulmm_b32_t_msl, ok)
        g_pf_pso_moe_mm_q5032_thr = compile_stamp(MetalMoeMulMmQ5032THR_metal_moe_mulmm_b32_t_msl, ok)
    }
}

[cold_path]   //! the moe twins of the superblock formats past k4, k5 and k6 (k3, the codebook and grid formats, k2)
def private pf_compile_moe_sb_twins(var ok : bool&) {
    if (g_pf_tensor_ok) {
        g_pf_pso_moe_mm_k3_t = compile_stamp(MetalMoeMulMmK3T_metal_moe_mulmm_k3_t_msl, ok)
        g_pf_pso_moe_mm_k3_th = compile_stamp(MetalMoeMulMmK3TH_metal_moe_mulmm_k3_t_msl, ok)
        g_pf_pso_moe_mm_k3_th128 = compile_stamp(MetalMoeMulMmK3TH128_metal_moe_mulmm_k3_t_msl, ok)
        g_pf_pso_moe_mm_k3_thr = compile_stamp(MetalMoeMulMmK3THR_metal_moe_mulmm_k3_t_msl, ok)
        g_pf_pso_moe_mm_iq4xs_t = compile_stamp(MetalMoeMulMmIq4xsT_metal_moe_mulmm_iq4xs_t_msl, ok)
        g_pf_pso_moe_mm_iq4xs_th = compile_stamp(MetalMoeMulMmIq4xsTH_metal_moe_mulmm_iq4xs_t_msl, ok)
        g_pf_pso_moe_mm_iq4xs_th128 = compile_stamp(MetalMoeMulMmIq4xsTH128_metal_moe_mulmm_iq4xs_t_msl, ok)
        g_pf_pso_moe_mm_iq4xs_thr = compile_stamp(MetalMoeMulMmIq4xsTHR_metal_moe_mulmm_iq4xs_t_msl, ok)
        g_pf_pso_moe_mm_iq3s4_t = compile_stamp(MetalMoeMulMmIq3s4T_metal_moe_mulmm_iq4xs_t_msl, ok)
        g_pf_pso_moe_mm_iq3s4_th = compile_stamp(MetalMoeMulMmIq3s4TH_metal_moe_mulmm_iq4xs_t_msl, ok)
        g_pf_pso_moe_mm_iq3s4_th128 = compile_stamp(MetalMoeMulMmIq3s4TH128_metal_moe_mulmm_iq4xs_t_msl, ok)
        g_pf_pso_moe_mm_iq3s4_thr = compile_stamp(MetalMoeMulMmIq3s4THR_metal_moe_mulmm_iq4xs_t_msl, ok)
        g_pf_pso_moe_mm_q40_t = compile_stamp(MetalMoeMulMmQ40T_metal_moe_mulmm_iq4xs_t_msl, ok)
        g_pf_pso_moe_mm_q40_th = compile_stamp(MetalMoeMulMmQ40TH_metal_moe_mulmm_iq4xs_t_msl, ok)
        g_pf_pso_moe_mm_q40_th128 = compile_stamp(MetalMoeMulMmQ40TH128_metal_moe_mulmm_iq4xs_t_msl, ok)
        g_pf_pso_moe_mm_q40_thr = compile_stamp(MetalMoeMulMmQ40THR_metal_moe_mulmm_iq4xs_t_msl, ok)
        g_pf_pso_moe_mm_nv4_t = compile_stamp(MetalMoeMulMmNv4T_metal_moe_mulmm_iq4xs_t_msl, ok)
        g_pf_pso_moe_mm_nv4_th = compile_stamp(MetalMoeMulMmNv4TH_metal_moe_mulmm_iq4xs_t_msl, ok)
        g_pf_pso_moe_mm_nv4_th128 = compile_stamp(MetalMoeMulMmNv4TH128_metal_moe_mulmm_iq4xs_t_msl, ok)
        g_pf_pso_moe_mm_nv4_thr = compile_stamp(MetalMoeMulMmNv4THR_metal_moe_mulmm_iq4xs_t_msl, ok)
        g_pf_pso_moe_mm_iq4nl_t = compile_stamp(MetalMoeMulMmIq4nlT_metal_moe_mulmm_iq4xs_t_msl, ok)
        g_pf_pso_moe_mm_iq4nl_th = compile_stamp(MetalMoeMulMmIq4nlTH_metal_moe_mulmm_iq4xs_t_msl, ok)
        g_pf_pso_moe_mm_iq4nl_th128 = compile_stamp(MetalMoeMulMmIq4nlTH128_metal_moe_mulmm_iq4xs_t_msl, ok)
        g_pf_pso_moe_mm_iq4nl_thr = compile_stamp(MetalMoeMulMmIq4nlTHR_metal_moe_mulmm_iq4xs_t_msl, ok)
        g_pf_pso_moe_mm_k2_t = compile_stamp(MetalMoeMulMmK2T_metal_moe_mulmm_k2_t_msl, ok)
        g_pf_pso_moe_mm_k2_th = compile_stamp(MetalMoeMulMmK2TH_metal_moe_mulmm_k2_t_msl, ok)
        g_pf_pso_moe_mm_k2_th128 = compile_stamp(MetalMoeMulMmK2TH128_metal_moe_mulmm_k2_t_msl, ok)
        g_pf_pso_moe_mm_k2_thr = compile_stamp(MetalMoeMulMmK2THR_metal_moe_mulmm_k2_t_msl, ok)
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

//! the split-K knobs this driver serves (`ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-split-k`), all 0 under `DASLLAMA_METAL_SPLIT_K=0`
def private pf_split_k_knobs {
    let on = g_env_metal.split_k
    SPLIT_K_TGS = on ? metal_split_k_tgs() : 0l
    SPLIT_K_TGS_KQ = on ? metal_split_k_tgs_kq() : 0l
    Q8_N32_ROWS = on ? metal_q8_n32_rows() : 0l
    SPLIT_K_TGS_BASE = on ? metal_split_k_tgs_base() : 0l
    SPLIT_K_TGS_KQ_BASE = on ? metal_split_k_tgs_kq_base() : 0l
    SPLIT_K_TGS_HMM = on ? metal_split_k_tgs_hmm() : 0l
}

//! every dense format's split-K pair behind its own crown (ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-split-k), the q8 blob's beside them
def private pf_compile_split_k_stamps(var ok : bool&) {   // nolint:STYLE037,STYLE038 — one crown-gated pair a format
    if (SPLIT_K_TGS <= 0l && SPLIT_K_TGS_KQ <= 0l && Q8_N32_ROWS <= 0l && SPLIT_K_TGS_BASE <= 0l && SPLIT_K_TGS_KQ_BASE <= 0l && SPLIT_K_TGS_HMM <= 0l) {
        return
    }
    metal_base_split_init(ok)   //! the base tiles' split stamps are the kernels file's, compiled under their knees there
    if (g_pf_mm_tensor) {
        g_pf_pso_mm_thsk = compile_stamp(MetalQ8MulMmTHSk_metal_q8_mulmm_t_msl, ok)
        g_pf_pso_mm_tsk = compile_stamp(MetalQ8MulMmTSk_metal_q8_mulmm_t_msl, ok)
        g_pf_pso_mm_thsk16 = compile_stamp(MetalQ8MulMmTHSk16_metal_q8_mulmm_t_msl, ok)
        g_pf_pso_mm_tsk16 = compile_stamp(MetalQ8MulMmTSk16_metal_q8_mulmm_t_msl, ok)
    }
    if (g_pf_bf16_mm_tensor) {
        g_pf_pso_bf16_mm_tsk = compile_stamp(MetalBf16MulMmTSk_metal_bf16_mulmm_t_msl, ok)
        g_pf_pso_bf16_mm_thsk = compile_stamp(MetalBf16MulMmTHSk_metal_bf16_mulmm_t_msl, ok)
    }
    if (g_pf_kq_mm4_tensor) {
        g_pf_pso_kq_mm4_thsk = compile_stamp(MetalKqMulMmK4THSk_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm4_tsk = compile_stamp(MetalKqMulMmK4TSk_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm4_thsk16 = compile_stamp(MetalKqMulMmK4THSk16_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm4_tsk16 = compile_stamp(MetalKqMulMmK4TSk16_metal_kq_mulmm_db_msl, ok)
    }
    if (g_pf_kq_mm5_tensor) {
        g_pf_pso_kq_mm5_thsk = compile_stamp(MetalKqMulMmK5THSk_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm5_tsk = compile_stamp(MetalKqMulMmK5TSk_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm5_thsk16 = compile_stamp(MetalKqMulMmK5THSk16_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm5_tsk16 = compile_stamp(MetalKqMulMmK5TSk16_metal_kq_mulmm_db_msl, ok)
    }
    if (g_pf_kq_mm6_tensor) {
        g_pf_pso_kq_mm6_thsk = compile_stamp(MetalKqMulMmK6THSk_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm6_tsk = compile_stamp(MetalKqMulMmK6TSk_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm6_thsk16 = compile_stamp(MetalKqMulMmK6THSk16_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm6_tsk16 = compile_stamp(MetalKqMulMmK6TSk16_metal_kq_mulmm_db_msl, ok)
    }
    if (g_pf_kq_mm44_tensor) {
        g_pf_pso_kq_mm44_thsk = compile_stamp(MetalKqMulMmIq4xsTHSk_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm44_tsk = compile_stamp(MetalKqMulMmIq4xsTSk_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm44_thsk16 = compile_stamp(MetalKqMulMmIq4xsTHSk16_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm44_tsk16 = compile_stamp(MetalKqMulMmIq4xsTSk16_metal_kq_mulmm_db_msl, ok)
    }
    if (g_pf_kq_mm35_tensor) {
        g_pf_pso_kq_mm35_thsk = compile_stamp(MetalKqMulMmIq3s4THSk_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm35_tsk = compile_stamp(MetalKqMulMmIq3s4TSk_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm35_thsk16 = compile_stamp(MetalKqMulMmIq3s4THSk16_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm35_tsk16 = compile_stamp(MetalKqMulMmIq3s4TSk16_metal_kq_mulmm_db_msl, ok)
    }
    if (g_pf_kq_mm45_tensor) {
        g_pf_pso_kq_mm45_thsk = compile_stamp(MetalKqMulMmIq4nlTHSk_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm45_tsk = compile_stamp(MetalKqMulMmIq4nlTSk_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm45_thsk16 = compile_stamp(MetalKqMulMmIq4nlTHSk16_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm45_tsk16 = compile_stamp(MetalKqMulMmIq4nlTSk16_metal_kq_mulmm_db_msl, ok)
    }
    if (g_pf_kq_mm40_tensor) {
        g_pf_pso_kq_mm40_thsk = compile_stamp(MetalKqMulMmQ40THSk_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm40_tsk = compile_stamp(MetalKqMulMmQ40TSk_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm40_thsk16 = compile_stamp(MetalKqMulMmQ40THSk16_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm40_tsk16 = compile_stamp(MetalKqMulMmQ40TSk16_metal_kq_mulmm_db_msl, ok)
    }
    if (g_pf_kq_mm49_tensor) {
        g_pf_pso_kq_mm49_thsk = compile_stamp(MetalKqMulMmNv4THSk_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm49_tsk = compile_stamp(MetalKqMulMmNv4TSk_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm49_thsk16 = compile_stamp(MetalKqMulMmNv4THSk16_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm49_tsk16 = compile_stamp(MetalKqMulMmNv4TSk16_metal_kq_mulmm_db_msl, ok)
    }
    if (g_pf_kq_mm3_tensor) {
        g_pf_pso_kq_mm3_thsk = compile_stamp(MetalKqMulMmK3THSk_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm3_tsk = compile_stamp(MetalKqMulMmK3TSk_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm3_thsk16 = compile_stamp(MetalKqMulMmK3THSk16_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm3_tsk16 = compile_stamp(MetalKqMulMmK3TSk16_metal_kq_mulmm_db_msl, ok)
    }
    if (g_pf_kq_mm2_tensor) {
        g_pf_pso_kq_mm2_thsk = compile_stamp(MetalKqMulMmK2THSk_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm2_tsk = compile_stamp(MetalKqMulMmK2TSk_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm2_thsk16 = compile_stamp(MetalKqMulMmK2THSk16_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm2_tsk16 = compile_stamp(MetalKqMulMmK2TSk16_metal_kq_mulmm_db_msl, ok)
    }
    if (g_pf_kq_mm33_tensor) {
        g_pf_pso_kq_mm33_thsk = compile_stamp(MetalKqMulMmIq3sTHSk_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm33_tsk = compile_stamp(MetalKqMulMmIq3sTSk_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm33_thsk16 = compile_stamp(MetalKqMulMmIq3sTHSk16_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm33_tsk16 = compile_stamp(MetalKqMulMmIq3sTSk16_metal_kq_mulmm_db_msl, ok)
    }
    if (g_pf_kq_mm34_tensor) {
        g_pf_pso_kq_mm34_thsk = compile_stamp(MetalKqMulMmIq3xxsTHSk_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm34_tsk = compile_stamp(MetalKqMulMmIq3xxsTSk_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm34_thsk16 = compile_stamp(MetalKqMulMmIq3xxsTHSk16_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm34_tsk16 = compile_stamp(MetalKqMulMmIq3xxsTSk16_metal_kq_mulmm_db_msl, ok)
    }
    if (g_pf_kq_mm23_tensor) {
        g_pf_pso_kq_mm23_thsk = compile_stamp(MetalKqMulMmIq2sTHSk_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm23_tsk = compile_stamp(MetalKqMulMmIq2sTSk_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm23_thsk16 = compile_stamp(MetalKqMulMmIq2sTHSk16_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm23_tsk16 = compile_stamp(MetalKqMulMmIq2sTSk16_metal_kq_mulmm_db_msl, ok)
    }
    if (g_pf_kq_mm24_tensor) {
        g_pf_pso_kq_mm24_thsk = compile_stamp(MetalKqMulMmIq2xsTHSk_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm24_tsk = compile_stamp(MetalKqMulMmIq2xsTSk_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm24_thsk16 = compile_stamp(MetalKqMulMmIq2xsTHSk16_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm24_tsk16 = compile_stamp(MetalKqMulMmIq2xsTSk16_metal_kq_mulmm_db_msl, ok)
    }
    if (g_pf_kq_mm25_tensor) {
        g_pf_pso_kq_mm25_thsk = compile_stamp(MetalKqMulMmIq2xxsTHSk_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm25_tsk = compile_stamp(MetalKqMulMmIq2xxsTSk_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm25_thsk16 = compile_stamp(MetalKqMulMmIq2xxsTHSk16_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm25_tsk16 = compile_stamp(MetalKqMulMmIq2xxsTSk16_metal_kq_mulmm_db_msl, ok)
    }
    if (g_pf_kq_mm51_tensor) {
        g_pf_pso_kq_mm51_thsk = compile_stamp(MetalKqMulMmQ51THSk_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm51_tsk = compile_stamp(MetalKqMulMmQ51TSk_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm51_thsk16 = compile_stamp(MetalKqMulMmQ51THSk16_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm51_tsk16 = compile_stamp(MetalKqMulMmQ51TSk16_metal_kq_mulmm_db_msl, ok)
    }
    if (g_pf_kq_mm4n32_tensor) {
        g_pf_pso_kq_mm4n32_thsk = compile_stamp(MetalKqMulMmIq4nl32THSk_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm4n32_tsk = compile_stamp(MetalKqMulMmIq4nl32TSk_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm4n32_thsk16 = compile_stamp(MetalKqMulMmIq4nl32THSk16_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm4n32_tsk16 = compile_stamp(MetalKqMulMmIq4nl32TSk16_metal_kq_mulmm_db_msl, ok)
    }
    if (g_pf_kq_mm4032_tensor) {
        g_pf_pso_kq_mm4032_thsk = compile_stamp(MetalKqMulMmQ4032THSk_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm4032_tsk = compile_stamp(MetalKqMulMmQ4032TSk_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm4032_thsk16 = compile_stamp(MetalKqMulMmQ4032THSk16_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm4032_tsk16 = compile_stamp(MetalKqMulMmQ4032TSk16_metal_kq_mulmm_db_msl, ok)
    }
    if (g_pf_kq_mm5032_tensor) {
        g_pf_pso_kq_mm5032_thsk = compile_stamp(MetalKqMulMmQ5032THSk_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm5032_tsk = compile_stamp(MetalKqMulMmQ5032TSk_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm5032_thsk16 = compile_stamp(MetalKqMulMmQ5032THSk16_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm5032_tsk16 = compile_stamp(MetalKqMulMmQ5032TSk16_metal_kq_mulmm_db_msl, ok)
    }
    g_pf_sk_bytes = uint64(max(max(max(SPLIT_K_TGS, SPLIT_K_TGS_KQ), max(SPLIT_K_TGS_BASE, SPLIT_K_TGS_KQ_BASE)), SPLIT_K_TGS_HMM)) * 2048ul * 4ul
    g_pf_sk_part = metal_new_buffer(g_dev, g_pf_sk_bytes)
    g_pf_sk_part2 = metal_new_buffer(g_dev, g_pf_sk_bytes)
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
    DEVW_MIN_TGS = metal_devw_min_tgs()
    pf_split_k_knobs()
    var ok = true
    g_pf_pso_rope = compile_stamp(metal_rope_msl, ok)
    g_pf_pso_qkr = compile_stamp(metal_pf_qkn_rope_msl, ok)
    g_pf_pso_qkr_hx = compile_stamp(metal_pf_qkn_rope_hx_msl, ok)
    g_pf_pso_qk = compile_stamp(metal_attn_qk_msl, ok)
    g_pf_pso_rowstat = compile_stamp(metal_attn_rowstat_msl, ok)
    g_pf_pso_av = compile_stamp(metal_attn_av_msl, ok)
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
    var perr : string
    var probe = pso_from_source(g_dev, MetalHalfMulMmTH_metal_half_mulmm_th_msl, MetalHalfMulMmTH_metal_half_mulmm_th_msl_entry, MetalHalfMulMmTH_metal_half_mulmm_th_msl_fastmath, perr)
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
    pf_compile_kq_b32_tensor_twins(ok)
    pf_compile_split_k_stamps(ok)
    if (g_pf_tensor_ok) {
        g_pf_pso_hmm_th = compile_stamp(MetalHalfMulMmTH_metal_half_mulmm_th_msl, ok)
        if (SPLIT_K_TGS_HMM > 0l) {
            g_pf_pso_hmm_thsk = compile_stamp(MetalHalfMulMmTHSk_metal_half_mulmm_th_msl, ok)
        }
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
    g_pf_pso_ga_dw = compile_stamp(metal_ga_dw_msl, ok)
    g_pf_pso_ga_dwh = compile_stamp(metal_ga_hdw_msl, ok)
    g_pf_pso_vad_enc = compile_stamp(metal_vad_enc_msl, ok)
    g_pf_pso_vad_lstm = compile_stamp(metal_vad_lstm_msl, ok)
    g_pf_pso_ga_frames = compile_stamp(metal_ga_frames_msl, ok)
    g_pf_pso_ga_mel = compile_stamp(metal_ga_mel_msl, ok)
    g_pf_pso_pk_dw2d = compile_stamp(metal_pk_dw2d_msl, ok)
    g_pf_pso_cn_dw2d = compile_stamp(metal_cn_dw2d_msl, ok)
    g_pf_pso_tw_rel_sincos = compile_stamp(metal_tw_rel_sincos_msl, ok)
    g_pf_pso_pk_xf = compile_stamp(metal_pk_xf_msl, ok)
    g_pf_pso_cn_melnorm = compile_stamp(metal_cn_melnorm_msl, ok)
    g_pf_pso_tw_pool2 = compile_stamp(metal_tw_pool2_msl, ok)
    g_pf_pso_tw_swiglu_rows = compile_stamp(metal_tw_swiglu_rows_msl, ok)
    g_pf_pso_tw_pool2d = compile_stamp(metal_tw_pool2d_msl, ok)
    g_pf_pso_tw_affine = compile_stamp(metal_tw_affine_rows_msl, ok)
    g_pf_pso_g4a_attn = compile_stamp(metal_g4a_attn_msl, ok)
    g_pf_pso_g4a_glu = compile_stamp(metal_g4a_glu_msl, ok)
    g_pf_pso_g4a_dw = compile_stamp(metal_g4a_dw_msl, ok)
    g_pf_pso_g4a_axpy = compile_stamp(metal_g4a_axpy_msl, ok)
    g_pf_pso_q3a_shuffle = compile_stamp(metal_q3a_shuffle_msl, ok)
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
    g_pf_pso_moe_reduce_sh = compile_stamp(metal_moe_reduce_sh_msl, ok)
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
    g_pf_pso_moe_mm_q4032 = compile_stamp(metal_moe_mulmm_q4032_msl, ok)
    g_pf_pso_moe_mm_q5032 = compile_stamp(metal_moe_mulmm_q5032_msl, ok)
    g_pf_pso_moe_mm_k3 = compile_stamp(metal_moe_mulmm_k3_msl, ok)
    g_pf_pso_moe_mm_k2 = compile_stamp(metal_moe_mulmm_k2_msl, ok)
    g_pf_pso_moe_mm_iq4xs = compile_stamp(metal_moe_mulmm_iq4xs_msl, ok)
    g_pf_pso_moe_mm_iq3s4 = compile_stamp(metal_moe_mulmm_iq3s4_msl, ok)
    g_pf_pso_moe_mm_iq4nl = compile_stamp(metal_moe_mulmm_iq4nl_msl, ok)
    g_pf_pso_moe_mm_q40 = compile_stamp(metal_moe_mulmm_q40_msl, ok)
    g_pf_pso_moe_mm_nv4 = compile_stamp(metal_moe_mulmm_nv4_msl, ok)
    g_pf_pso_moe_mm_iq3s = compile_stamp(metal_moe_mulmm_iq3s_msl, ok)
    g_pf_pso_moe_mm_iq3xxs = compile_stamp(metal_moe_mulmm_iq3xxs_msl, ok)
    g_pf_pso_moe_mm_iq2s = compile_stamp(metal_moe_mulmm_iq2s_msl, ok)
    g_pf_pso_moe_mm_iq2xs = compile_stamp(metal_moe_mulmm_iq2xs_msl, ok)
    g_pf_pso_moe_mm_iq2xxs = compile_stamp(metal_moe_mulmm_iq2xxs_msl, ok)
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
    if ((!t.blocks.attn_is_std && !(t.config.hybrid_deltanet && dn_metal_ok(t) && dn_ba_metal_window_ok(t))) ||
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
let private MM_TAIL_SPLIT_MAX = 1l   //! the peel's reach past whole tiles where the split-K form takes the padded tile (ARCHITECTURE_GPU_PREFILL_GEMM.md#gemv-tail-peel)
let private MM_TAIL_SPLIT_WINDOW = 4l   //! ... and over a window of no whole tile: the rows one batched GEMV dispatch carries
let private MM_TAIL_KQ_PAST_KNEE = 2l   //! a K-quant remainder past the knee: the row forms win at two rows, even at four, lose at eight

var private g_pf_tail_sites = 0l
var private g_pf_kq_tail_sites = 0l
var private g_pf_tail_rows = 0l

//! GEMV-tail engage counters: (sites, rows) peeled since load — the gates' anti-vacuous witness; `kq_sites` counts the
//! K-quant sites among them, which ride the decode's row-batched forms rather than the q8 blob's remainder peel
def public metal_prefill_tail_stats() : tuple<sites : int64; kq_sites : int64; rows : int64> =>
    (sites = g_pf_tail_sites, kq_sites = g_pf_kq_tail_sites, rows = g_pf_tail_rows)

var private g_pf_devw_scratch_sites = 0l
var private g_pf_devw_scratch_bytes = 0ul

//! K-quant sites served off a scratch panel since load, and the panel bytes their dequant passes wrote
def public metal_prefill_devw_scratch_stats() : tuple<sites : int64; bytes : uint64> =>
    (sites = g_pf_devw_scratch_sites, bytes = g_pf_devw_scratch_bytes)

[arch(at="../ARCHITECTURE_GPU_PREFILL_GEMM.md#gemv-tail-peel")]
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
        || g_pf_pso_moe_mm_q8_th != null || g_pf_pso_moe_mm_mx4_th != null   //! the MoE down-site cvt consumes the panel too
        || metal_kq_half_any())   //! the short window's half-x rows forms read the dual-stored twin
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
    tw_half(enc, bx, bxh, rows * kdim)
    return bxh
}

//! the pure knee arithmetic behind pf_devw_tiles - cap_bytes is the staging panel capacity
//! (g_pf_bwh_bytes in production; the kernel-unit test pins its own)
[arch(at="../ARCHITECTURE_GPU_PREFILL_GEMM.md#devw-panel-knees")]
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
def private pf_hx_panel(enc : MetalComputeEncoder?; var twin : MetalBuffer?; bx : MetalBuffer?; rows, kdim : int64; skip_convert : bool = false) : MetalBuffer? =>
    twin != null ? twin : pf_cvt_panel(enc, bx, rows, kdim, skip_convert)

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
    tap_major ? g_pf_pso_pk_dw : g_pf_pso_cn_dw, g_pf_pso_g4a_glu, g_pf_pso_g4a_axpy, g_pf_pso_tw_rel_sincos))

//! The rotary FastConformer kernel set compiled (gigaam): the conv module's biased depthwise stamps (f32 and f16 taps), the GLU,
//! the axpy, the slab trio the attention rides with its head restrides and its rope, and the front's im2col.
def pf_ga_ready() : bool => psos_ready(fixed_array(g_pso_f32mm, g_pf_pso_ga_dw, g_pf_pso_ga_dwh, g_pf_pso_g4a_glu, g_pf_pso_g4a_axpy,
    g_pf_pso_qkmm, g_pf_pso_rowstat, g_pf_pso_avmm, g_pso_head_pad, g_pso_head_unpad, g_pso_tw_rope_tab, g_pso_st2_im2col))

//! The silero VAD's two kernels compiled - the tower driver's VAD seat's device gate.
def pf_vad_ready() : bool => psos_ready(fixed_array(g_pf_pso_vad_enc, g_pf_pso_vad_lstm))

//! The largest threadgroup the VAD recurrence's compiled pipeline admits (0 before it compiles): its registers can hold it under
//! the kernel's 1024 lanes on a GPU with a smaller register file.
def pf_vad_lstm_lanes() : uint => g_pf_pso_vad_lstm != null ? metal_pipeline_max_total_threads(g_pf_pso_vad_lstm) : 0u

//! The gigaam mel's kernels compiled: the frame window, the exact DFT tile, the power spectrum and the clamped-log filterbank.
def pf_ga_mel_ready() : bool => psos_ready(fixed_array(g_pf_pso_ga_frames, g_pso_f32_exact_mm, g_pf_pso_q3a_pow, g_pf_pso_ga_mel))

//! The parakeet subsample front's kernels compiled - the whole-encode driver's extra device gate.
def pf_pk_front_ready() : bool => psos_ready(fixed_array(g_pf_pso_pk_dw2d, g_pf_pso_pk_xf))

//! The canary front's kernels compiled - the parakeet front's, the spectrum's and the mel normalization.
def pf_cn_front_ready() : bool => pf_pk_front_ready() && psos_ready(fixed_array(g_pf_pso_cn_melnorm, g_pf_pso_q3a_pow, g_pf_pso_cn_dw2d))

//! The projector tails' row kernels compiled: the whisper-class pair pool and gate, the vision grid pool.
def pf_tw_tail_ready() : bool => psos_ready(fixed_array(g_pf_pso_tw_pool2, g_pf_pso_tw_swiglu_rows, g_pf_pso_tw_pool2d, g_pf_pso_tw_affine))

//! The gemma4a Conformer kernel set compiled — its tower driver's device gate.
def pf_g4a_ready() : bool => psos_ready(fixed_array(g_pf_pso_g4a_attn, g_pf_pso_g4a_glu, g_pf_pso_g4a_dw, g_pf_pso_g4a_axpy))

//! The gemma4a mel/front kernel set compiled — the whole-chunk driver's extra device gate.
def pf_g4a_front_ready() : bool => psos_ready(fixed_array(g_pf_pso_g4a_mag, g_pf_pso_g4a_logmel, g_pf_pso_g4a_lnrelu, g_pso_im2col2d))

//! The qwen3a conv-frontend tail kernels compiled — its front driver's extra device gate.
def pf_q3a_front_ready() : bool => psos_ready(fixed_array(g_pf_pso_q3a_shuffle))

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
    if (rows - r0 > 0l && !pf_enc_hmm_split(enc, bwh, woff, bxh, uint64(r0 * kdim * 2l), by, yoff + uint64(r0 * dfull * 4l), bk, bn, rows - r0, td, dfull, kdim)) {
        enc_gemm_hmm_th(enc, bwh, woff, bxh, uint64(r0 * kdim * 2l), by, yoff + uint64(r0 * dfull * 4l), bk, bn, rows - r0, td)
    }
}

//! The dev-W tile split along K under its knee: a narrow panel tile over a long K walks it serially on few threadgroups. The
//! partial planes are `rows` x `td`, so a tile of a wider site (`td` < `dfull`) sums them into its output columns row by row.
[arch(at="../ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-split-k")]
def private pf_enc_hmm_split(enc : MetalComputeEncoder?; bwh : MetalBuffer?; woff : uint64; bxh : MetalBuffer?; xoff : uint64;
                             by : MetalBuffer?; yoff : uint64; bk, bn : MetalBuffer?; rows, td, dfull, kdim : int64) : bool {
    let knee = SplitKnee(tgs = SPLIT_K_TGS_HMM, min_k = 256l)
    let tiles = rows / 32l * (td / 64l)
    if (g_pf_pso_hmm_thsk == null || rows % 32l != 0l || !pf_split_k_on(tiles, kdim, knee)) {
        return false
    }
    return pf_split_k_run(enc, tiles, kdim, knee, by, yoff, rows, td, dfull, false) $(out, ooff, nsk) {
        enc_gemm_hmm_thsk(enc, bwh, woff, bxh, xoff, out, ooff, bk, bn, rows, td, nsk)
        return true
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

[cold_path]
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
        } elif (kq_fmt_of_stream_code(f, kfmt) && kq_sb(kfmt)) {
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
                          bxh, by, bk, bn : MetalBuffer?; rows, d, kdim : int64; yoff : uint64; twin : bool) : bool {
    // the q8 dequant pass steps whole 32-blocks
    if (!g_pf_env_dev_w || kdim <= 0l || (kdim % 32l) != 0l) {
        return false
    }
    if (metal_devw_panels()) {
        if (pf_devw_panel_q8_resident(enc, bwblob, wboff, bxh, by, bk, bn, rows, d, kdim, yoff)) {
            return true
        }
    } elif (rows < devw_scratch_floor(true, twin)) {
        return false
    }
    let tiles = pf_devw_tiles(rows, d, kdim)
    if (tiles <= 0l || pf_devw_grid_short(rows, d / tiles)) {
        return false
    }
    g_pf_devw_scratch_sites++
    g_pf_devw_scratch_bytes += uint64(d * kdim * 2l)
    let td = d / tiles
    for (ti in range64(tiles)) {
        pf_enc_devw_q8_tile(enc, bwblob, wboff, bxh, by, bk, bn, rows, ti * td, td, d, kdim, yoff)
    }
    return true
}

//! the split-scale formats whose dev-W dequant rides the mul_mm tensor scaffold (ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-kq-tensor-scaffold)
def private pf_kq_split_fmt(fmt : KqFmt) : bool {
    return (fmt == KqFmt.iq4xs || fmt == KqFmt.k3 || fmt == KqFmt.iq3s || fmt == KqFmt.iq3xxs || fmt == KqFmt.iq4nl
        || fmt == KqFmt.q40 || fmt == KqFmt.nv4 || fmt == KqFmt.k2 || fmt == KqFmt.iq2s || fmt == KqFmt.iq2xs || fmt == KqFmt.iq2xxs || fmt == KqFmt.iq3s4)
}

//! the format's dev-W dequant PSO, null where the toolchain probe failed
def private pf_kq_dq_pso(fmt : KqFmt) : MetalComputePipeline? {
    if (fmt == KqFmt.k6) return g_pf_pso_dq_k6
    if (fmt == KqFmt.k4) return g_pf_pso_dq_k4
    if (fmt == KqFmt.k5) return g_pf_pso_dq_k5
    if (fmt == KqFmt.iq4xs) return g_pf_pso_dq_kq44
    if (fmt == KqFmt.iq3s4) return g_pf_pso_dq_kq35
    if (fmt == KqFmt.iq4nl) return g_pf_pso_dq_kq45
    if (fmt == KqFmt.q40) return g_pf_pso_dq_kq40
    if (fmt == KqFmt.nv4) return g_pf_pso_dq_kq49
    if (fmt == KqFmt.k3) return g_pf_pso_dq_kq3
    if (fmt == KqFmt.iq3s) return g_pf_pso_dq_kq33
    if (fmt == KqFmt.iq3xxs) return g_pf_pso_dq_kq34
    if (fmt == KqFmt.k2) return g_pf_pso_dq_kq2
    if (fmt == KqFmt.iq2s) return g_pf_pso_dq_kq23
    if (fmt == KqFmt.iq2xs) return g_pf_pso_dq_kq24
    if (fmt == KqFmt.iq2xxs) return g_pf_pso_dq_kq25
    if (fmt == KqFmt.q51) return g_pf_pso_dq_kq51
    if (fmt == KqFmt.iq4nl32) return g_pf_pso_dq_kq4n32
    if (fmt == KqFmt.q4032) return g_pf_pso_dq_kq4032
    if (fmt == KqFmt.q5032) return g_pf_pso_dq_kq5032
    return null
}

def private pf_kq_iquant_th_any() : bool {
    return (g_pf_pso_kq_mm44_th != null || g_pf_pso_kq_mm35_th != null || g_pf_pso_kq_mm45_th != null || g_pf_pso_kq_mm40_th != null
        || g_pf_pso_kq_mm3_th != null || g_pf_pso_kq_mm33_th != null || g_pf_pso_kq_mm34_th != null
        || g_pf_pso_kq_mm2_th != null || g_pf_pso_kq_mm23_th != null || g_pf_pso_kq_mm24_th != null
        || g_pf_pso_kq_mm25_th != null)
}

//! some format's dev-W dequant pass compiled - the panel the passes write is allocated on the same map the dispatch reads
def private pf_kq_dq_any() : bool {
    for (i in range(KQ_FMT_COUNT)) {
        return true if (pf_kq_dq_pso(kq_fmt_of_slot(i)) != null)
    }
    return false
}

//! one format's dequant of W rows [tbase, tbase+td) into the f16 panel bwh: the plane offsets
//! stride per superblock (16B scale strips, the 2B d tail - 4B on k2, none on the compact
//! formats - and kq_qsb(fmt) quant bytes), a per-32 plane's per 32-block; K-quants run per 32-block, the rest per 64x64 chunk
[arch(at="../ARCHITECTURE_GPU.md#gpu-backends")]
def private pf_enc_kq_dq(enc : MetalComputeEncoder?; fmt : KqFmt; bs : tuple<buf : MetalBuffer?; soff : uint64; doff : uint64>;
                         bq : tuple<buf : MetalBuffer?; qoff : uint64>; tbase, td, kdim : int64; bwh, bk, bn : MetalBuffer?) {
    if (kq_dense_b32(fmt)) {
        let nkb = kdim / KQ_B32_ELEMS
        let so32 = bs.soff + uint64(tbase * nkb * kq_ssb(fmt))
        let qo32 = bq.qoff + uint64(tbase * nkb * kq_qsb(fmt))
        invoke(b32_moe_pick(fmt, @@enc_kq_dq_q51, @@enc_kq_dq_iq4nl32, @@enc_kq_dq_q4032, @@enc_kq_dq_q5032),
            enc, bs.buf, so32, bs.buf, so32, bq.buf, qo32, bwh, 0ul, bwh, 0ul, bk, bn, bwh, kdim, td)
        return
    }
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
    } elif (fmt == KqFmt.iq3s4) {
        enc_kq_dq_iq3s4(enc, bs.buf, dofs, bs.buf, so, bq.buf, qo, bwh, 0ul, bwh, 0ul, bk, bn, bwh, kdim, td)
    } elif (fmt == KqFmt.iq4nl) {
        enc_kq_dq_iq4nl(enc, bs.buf, so, bs.buf, so, bq.buf, qo, bwh, 0ul, bwh, 0ul, bk, bn, bwh, kdim, td)
    } elif (fmt == KqFmt.q40) {
        enc_kq_dq_q40(enc, bs.buf, so, bs.buf, so, bq.buf, qo, bwh, 0ul, bwh, 0ul, bk, bn, bwh, kdim, td)
    } elif (fmt == KqFmt.nv4) {
        enc_kq_dq_nv4(enc, bs.buf, so, bs.buf, so, bq.buf, qo, bwh, 0ul, bwh, 0ul, bk, bn, bwh, kdim, td)
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

[arch(at="../ARCHITECTURE_GPU_PREFILL_GEMM.md#devw-panel-knees")]
def private pf_devw_grid_short(rows, td : int64) : bool => DEVW_MIN_TGS > 0l && (rows + 31l) / 32l * (td / 64l) < DEVW_MIN_TGS

def devw_scratch_floor(q8, twin : bool) : int64 => !twin ? DEVW_SCRATCH_ROWS : (q8 ? DEVW_Q8_TWIN_ROWS : DEVW_KQ_TWIN_ROWS)

def private pf_devw_panel_kq(enc : MetalComputeEncoder?; fmt : KqFmt; woff : int64;
                             bq : tuple<buf : MetalBuffer?; qoff : uint64>; bs : tuple<buf : MetalBuffer?; soff : uint64; doff : uint64>;
                             bxh, by, bk, bn : MetalBuffer?; rows, d, kdim : int64; yoff : uint64; twin : bool) : bool {
    // the split and per-32 chunk passes dequant whole 64x64 chunks; the K-quant passes step 32-blocks
    let chunked = pf_kq_split_fmt(fmt) || kq_dense_b32(fmt)
    if (!g_pf_env_dev_w || pf_kq_dq_pso(fmt) == null || kdim <= 0l || (kdim % (chunked ? 64l : 32l)) != 0l) {
        return false
    }
    let lever = metal_devw_panels()
    if (lever) {
        var rfresh = false
        let re = pf_devw_resident_panel(pf_devw_key(intptr(bq.buf), uint64(woff), kq_schema_id(fmt)), uint64(d * kdim * 2l), rfresh)
        if (re.buf != null) {
            if (rfresh) {
                pf_enc_kq_dq(enc, fmt, bs, bq, 0l, d, kdim, re.buf, bk, bn)
            }
            pf_enc_hmm(enc, re.buf, bxh, by, yoff, bk, bn, rows, d, d, kdim, re.boff)
            return true
        }
    } elif (rows < devw_scratch_floor(false, twin)) {
        return false
    }
    let tiles = pf_devw_tiles(rows, d, kdim, slow_fb = true)
    if (tiles <= 0l || (chunked && (d / tiles) % 64l != 0l) || pf_devw_grid_short(rows, d / tiles)) {
        return false
    }
    g_pf_devw_scratch_sites++
    g_pf_devw_scratch_bytes += uint64(d * kdim * 2l)
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
        } elif (fmt == KqFmt.nv4) {   //! the compact UE4M3 row is the strip view alone
            k.ksc |> reserve_resize(nsb_all)
            memcpy(addr(k.ksc[0]), addr < uint8 const? >(ks[0]), nsb_all * NV4_SSB)
        } else {
            k.ksc |> reserve_resize(nsb_all)
            memcpy(addr(k.ksc[0]), addr < uint8 const? >(ks[0]), nsb_all * 16l)
            k.kdh |> reserve_resize((long_length(ks) - nsb_all * 16l) / 2l)
            memcpy(addr(k.kdh[0]), addr < uint8 const? >(ks[nsb_all * 16l]), long_length(ks) - nsb_all * 16l)
        }
    }
    static_if (typeinfo safe_has_field < iq4lutt > (*k)) {
        for (i in range(16)) {
            k.iq4lutt[i] = fmt == KqFmt.q40 ? float(i) - 8.0 : (fmt == KqFmt.nv4 ? mx4_val(uint(i)) : iq4_lut(uint(i)))
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
    if (fmt == KqFmt.nv4) return devw_mirror_run(new MetalKqDequantNv4(), fmt, kq, ks, d, kdim, dst)
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
[arch(at="../ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-gemm-ladder")]
def private enc_gemm_mm_tensor(enc : MetalComputeEncoder?; bwblob : MetalBuffer?; wboff : uint64; bx, by, bk, bn : MetalBuffer?; rows, d : int64; yoff : uint64; bxh : MetalBuffer?; kdim : int64 = 0l;
                               live : int64 = 0l) {
    if (bxh != null && g_pf_pso_hmm_th != null) {
        if (pf_devw_panel(enc, bwblob, wboff, bxh, by, bk, bn, rows, d, kdim, yoff, true)) {
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
    if (r0 == 0l && pf_enc_q8_split(enc, bwblob, wboff, half_form ? bxh : bx, half_form, by, bk, bn, rows, d, kdim, yo, live)) {
        return
    }
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

//! A family's split-K knee: the threadgroup target its grid is sized against and the least K a share keeps - the q8 blob's and
//! bf16's, or the K-quant scaffold's (its stage decodes more per element, so a wider grid still pays); `base` the simdgroup base
//! tiles' knees, the forms a box serves where its dense race crowned no tensor twin
def private pf_knee(kq : bool; base : bool = false) : SplitKnee {
    let tgs = base ? (kq ? SPLIT_K_TGS_KQ_BASE : SPLIT_K_TGS_BASE) : (kq ? SPLIT_K_TGS_KQ : SPLIT_K_TGS)
    return SplitKnee(tgs = tgs, min_k = pf_knee_min_k(kq))
}

//! the least K a split share walks: four 32-blocks on the q8 blob, eight on the K-quant scaffold, whose shorter shares lose to their reduce
def private pf_knee_min_k(kq : bool) : int64 => kq ? 256l : 128l

//! whether the split-K form takes a site whose 32-row tile grid is `tiles` threadgroups: the grid is at most half the target
def private pf_split_k_on(tiles, kdim : int64; knee : SplitKnee) : bool => g_pf_split_k_on && split_k_on(tiles, kdim, knee)

//! the next partial-plane scratch, or null where `bytes` outgrows it
def private pf_split_k_part(bytes : uint64) : MetalBuffer? {
    if (g_pf_sk_part == null || bytes > g_pf_sk_bytes) {
        return null
    }
    g_pf_sk_flip = !g_pf_sk_flip
    return g_pf_sk_flip ? g_pf_sk_part2 : g_pf_sk_part
}

//! whether a window of `live` rows takes the 16-row split stamp `pso`: one 32-row tile holds it and it fits 16 rows - the tensor op
//! at 16 rows costs 0.69 of the 32-row op the padding would pay
def private pf_split_k_short(live, mp : int64; pso : MetalComputePipeline?) : bool => pso != null && live > 0l && live <= 16l && mp == 32l && !pf_held("split_k16")

//! One site's split-K pass: `stamp` writes `nsk` ways (sized on `ways_tiles`) into the scratch, the reduce sums them into `by` -
//! rows `ld` apart for a column tile of a wider output; `whole_ok` false or a stamp answering false declines the site
//! (`ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-split-k`)
[arch(at="../ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-split-k")]
def private pf_split_k_run(enc : MetalComputeEncoder?; ways_tiles, kdim : int64; knee : SplitKnee; by : MetalBuffer?; yoff : uint64;
                           mp, d, ld : int64; whole_ok : bool; stamp : block<(out : MetalBuffer?; ooff : uint64; nsk : int64) : bool>) : bool {
    let nsk = split_k_ways(ways_tiles, kdim, knee)
    if (nsk == 1l) {
        return false if (!whole_ok || !invoke(stamp, by, yoff, 1l))
    } else {
        let part = pf_split_k_part(uint64(nsk * mp * d * 4l))
        return false if (part == null || !invoke(stamp, part, 0ul, nsk))
        enc_sk_reduce_b(enc, part, by, mp * d, nsk, yoff, ld > d ? d : 0l, ld)
    }
    g_pf_sk_sites++
    return true
}

//! The q8 blob's short-window form: the 32-column split-K stamp over the whole 32-row grid; false where the site is past the
//! knee or the stamp is absent - the caller's tile serves
[arch(at="../ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-split-k")]
def private pf_enc_q8_split(enc : MetalComputeEncoder?; bwblob : MetalBuffer?; wboff : uint64; bxa : MetalBuffer?; half_form : bool;
                            by, bk, bn : MetalBuffer?; mp, d, kdim : int64; yoff : uint64; live : int64 = 0l) : bool {
    let pso = half_form ? g_pf_pso_mm_thsk : g_pf_pso_mm_tsk
    if (pso == null || d % 32l != 0l || kdim <= 0l) {
        return false
    }
    let knee = pf_knee(false)
    //! past the split knee the 32-column stamp still takes a grid of few M tiles at one way (the row-58 knee, `Q8_N32_ROWS`)
    let one_way = !pf_split_k_on(mp / 32l * (d / 64l), kdim, knee)
    if (one_way && (!g_pf_split_k_on || mp > Q8_N32_ROWS)) {
        return false
    }
    let short = pf_split_k_short(live, mp, half_form ? g_pf_pso_mm_thsk16 : g_pf_pso_mm_tsk16)
    let mpe = short ? 16l : mp
    return pf_split_k_run(enc, mp / 32l * (d / 32l), kdim, one_way ? SplitKnee(tgs = 0l, min_k = pf_knee_min_k(false)) : knee, by, yoff, mpe, d, 0l, true) $(out, ooff, nsk) {
        if (short && half_form) {
            enc_gemm_mm_thsk16_c(enc, bwblob, wboff, bwblob, bxa, 0ul, out, ooff, bk, bn, 16l, d, nsk)
        } elif (short) {
            enc_gemm_mm_tsk16_c(enc, bwblob, wboff, bwblob, bxa, 0ul, out, ooff, bk, bn, 16l, d, nsk)
        } elif (half_form) {
            enc_gemm_mm_thsk_c(enc, bwblob, wboff, bwblob, bxa, 0ul, out, ooff, bk, bn, mp, d, nsk)
        } else {
            enc_gemm_mm_tsk_c(enc, bwblob, wboff, bwblob, bxa, 0ul, out, ooff, bk, bn, mp, d, nsk)
        }
        return true
    }
}

//! The q8 blob's simdgroup base tile split along K under its own knee - the form a box with no q8 tensor crown serves
[arch(at="../ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-split-k")]
def private pf_enc_q8_base_split(enc : MetalComputeEncoder?; bwblob : MetalBuffer?; wboff : uint64; bx, by, bk, bn : MetalBuffer?;
                                 mp, d, kdim : int64; yoff : uint64) : bool {
    let tiles = mp / 32l * (d / 64l)
    let knee = pf_knee(false, true)
    if (g_pso_mm_sk == null || d % 64l != 0l || !pf_split_k_on(tiles, kdim, knee)) {
        return false
    }
    return pf_split_k_run(enc, tiles, kdim, knee, by, yoff, mp, d, 0l, false) $(out, ooff, nsk) {
        enc_gemm_mm_sk_c(enc, bwblob, wboff, bwblob, bx, out, ooff, bk, bn, mp, d, nsk)
        return true
    }
}

//! the q8 blob's simdgroup base tile: split along K under its knee, else whole
def private pf_enc_q8_base(enc : MetalComputeEncoder?; bwblob : MetalBuffer?; wboff : uint64; bx, by, bk, bn : MetalBuffer?;
                           mp, d, kdim : int64; yoff : uint64) {
    if (!pf_enc_q8_base_split(enc, bwblob, wboff, bx, by, bk, bn, mp, d, kdim, yoff)) {
        enc_gemm_mm_c(enc, bwblob, wboff, bwblob, bx, by, yoff, bk, bn, mp, d)
    }
}

def private pf_q8_devw(enc : MetalComputeEncoder?; bwblob : MetalBuffer?; wboff : uint64; bxh, by, bk, bn : MetalBuffer?; rows, d, kdim : int64; yoff : uint64) : bool =>
    g_pf_tensor_ok && bxh != null && g_pf_pso_hmm_th != null && pf_devw_panel(enc, bwblob, wboff, bxh, by, bk, bn, rows, d, kdim, yoff, false)

//! yoff places K/V outputs past a continuation's panel rows; kdim/npos arm the GEMV tail;
//! bxh is the caller-minted f16 twin (one convert per panel).
def private enc_gemm_mm(enc : MetalComputeEncoder?; bwblob : MetalBuffer?; wboff : uint64; bx, by, bk, bn : MetalBuffer?; mp, d : int64; yoff : uint64 = 0ul; kdim : int64 = 0l; npos : int64 = 0l; bxh : MetalBuffer? = null) {
    let r = npos % 32l
    //! per-form reduction alignment: the gemv walks per-block; the b4 stripe reads whole 128-quant rounds
    let align_ok = r == 1l ? kdim % 32l == 0l : kdim % 128l == 0l
    let split_on = g_pf_mm_tensor && g_pf_pso_mm_thsk != null && pf_split_k_on(mp / 32l * (d / 64l), kdim, pf_knee(false))
    let peel_max = !split_on ? MM_TAIL_MAX : (npos < 32l ? MM_TAIL_SPLIT_WINDOW : MM_TAIL_SPLIT_MAX)
    if (g_pf_env_mm_tail && npos > 0l && r >= 1l && r <= peel_max && align_ok && d % 8l == 0l) {
        let nfull = npos - r
        if (nfull > 0l) {
            if (g_pf_mm_tensor) {
                enc_gemm_mm_tensor(enc, bwblob, wboff, bx, by, bk, bn, nfull, d, yoff, bxh, kdim)
            } elif (!pf_q8_devw(enc, bwblob, wboff, bxh, by, bk, bn, nfull, d, kdim, yoff)) {
                pf_enc_q8_base(enc, bwblob, wboff, bx, by, bk, bn, nfull, d, kdim, yoff)
            }
        }
        enc_gemm_mm_tail(enc, bwblob, wboff, bx, by, bk, bn, yoff, kdim, d, nfull, r)
        return
    }
    if (g_pf_mm_tensor) {
        enc_gemm_mm_tensor(enc, bwblob, wboff, bx, by, bk, bn, mp, d, yoff, bxh, kdim, npos)
    } elif (!pf_q8_devw(enc, bwblob, wboff, bxh, by, bk, bn, mp, d, kdim, yoff)) {
        pf_enc_q8_base(enc, bwblob, wboff, bx, by, bk, bn, mp, d, kdim, yoff)
    }
}

//! whether a GEMM through `pf_enc_q8_mm` reads a half activation panel on this box: the caller converts one a panel
def pf_q8_mm_half() : bool => (g_pf_mm_tensor && g_pf_pso_mm_th != null) || (g_pf_tensor_ok && g_pf_pso_hmm_th != null)

//! One q8 GEMM over a 34-byte-block blob on the prefill ladder, for a driver outside this file: `mp` rows padded to 32,
//! no GEMV tail; `bxh` is the caller's half twin of `bx`, null where `pf_q8_mm_half` says none is read.
[arch(at="../ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-gemm-ladder")]
def pf_enc_q8_mm(enc : MetalComputeEncoder?; bwblob : MetalBuffer?; wboff : uint64; bx, bxh, by, bk, bn : MetalBuffer?; mp, d, kdim : int64) {
    enc_gemm_mm(enc, bwblob, wboff, bx, by, bk, bn, mp, d, 0ul, kdim, 0l, bxh)
}

var private g_pf_no_model = Model()   //! the tower's K-quant sites bind their own planes: the dense site reads its Model for per-32 planes alone, which a superblock site never asks for

//! One K-quant GEMM over a driver's own planes on the prefill ladder, for a driver outside this file: the member's superblocks
//! at `bq` and its Metal-form scales at `bs` (`kq_scales_of`'s pair), `mp` rows padded to 32, no GEMV tail; `bxh` the caller's
//! half twin of `bx`, null where `pf_q8_mm_half` says none is read; `site` keys the site's dev-W panel beside its buffer.
[arch(at="../ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-gemm-ladder")]
def pf_enc_kq_mm(enc : MetalComputeEncoder?; fmt : KqFmt; site : int64; bq : tuple<buf : MetalBuffer?; qoff : uint64>;
                 bs : tuple<buf : MetalBuffer?; soff : uint64; doff : uint64>; bx, bxh, by, bk, bn : MetalBuffer?; mp, d, kdim : int64) {
    verify(kq_sb(fmt) && !kq_dense_b32(fmt), "pf_enc_kq_mm: a superblock format alone")
    pf_enc_kq_dense_site(enc, g_pf_no_model, fmt, site, d, bq, bs, bx, by, bk, bn, mp, 0ul, bxh, kdim, true, !pf_kq_split_fmt(fmt))
}

//! kdim feeds the tall/remainder split's X stride; kdim-less callers stay whole-dispatch 32-tile
[arch(at="../ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-gemm-ladder")]
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
        if (r0 == 0l && pf_enc_bf16_split(enc, bw, wboff, half_form ? bxh : bx, half_form, by, bk, bn, mp, nd, kdim)) {
            return
        }
        if (half_form) {
            pf_enc_bf16_mm_th_c(enc, bw, wboff, bxh, xoff, by, yo, bk, bn, mp - r0, nd)
        } else {
            pf_enc_bf16_mm_t_c(enc, bw, wboff, bx, xoff, by, yo, bk, bn, mp - r0, nd)
        }
    } else {
        pf_enc_bf16_mm_c(enc, bw, wboff, bx, by, bk, bn, mp, nd)
    }
}

//! The bf16 plane's short-window form: its split-K stamp over the 32-row grid; false past the knee, at one way (the stamp is the
//! TH tile there) or where the stamp is absent
[arch(at="../ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-split-k")]
def private pf_enc_bf16_split(enc : MetalComputeEncoder?; bw : MetalBuffer?; wboff : uint64; bxa : MetalBuffer?; half_form : bool;
                              by, bk, bn : MetalBuffer?; mp, nd, kdim : int64) : bool {
    let pso = half_form ? g_pf_pso_bf16_mm_thsk : g_pf_pso_bf16_mm_tsk
    let tiles = mp / 32l * (nd / 64l)
    let knee = pf_knee(false)
    if (pso == null || !pf_split_k_on(tiles, kdim, knee)) {
        return false
    }
    return pf_split_k_run(enc, tiles, kdim, knee, by, 0ul, mp, nd, 0l, false) $(out, ooff, nsk) {
        if (half_form) {
            pf_enc_bf16_mm_thsk_c(enc, bw, wboff, bxa, 0ul, out, ooff, bk, bn, mp, nd, nsk)
        } else {
            pf_enc_bf16_mm_tsk_c(enc, bw, wboff, bxa, 0ul, out, ooff, bk, bn, mp, nd, nsk)
        }
        return true
    }
}

//! the dense tall-kq gate's pure half: env_tall is the set_metal_prefill_tall A/B seat -
//! the tall form must honor it like every other form override
def tallkq_pick(mp, rows, kdim : int64; env_tall : bool) : bool =>
    env_tall && kdim > 0l && mp >= 128l && uint64(rows * kdim * 2l) >= TALLKQ_MIN_PANEL

//! the iq4xs family's twins: iq4xs, iq3s4, iq4nl and q40 share one template, each behind its own crown
def private pf_compile_kq_iq4_tensor_twins(var ok : bool&) {
    g_pf_kq_mm44_tensor = metal_tensor_crowned("kq_mulmm_iq4xs")
    if (g_pf_kq_mm44_tensor) {
        g_pf_pso_kq_mm44_t = compile_stamp(MetalKqMulMmIq4xsT_metal_kq_mulmm_iq4xs_t_msl, ok)
        g_pf_pso_kq_mm44_th = compile_stamp(MetalKqMulMmIq4xsTH_metal_kq_mulmm_iq4xs_t_msl, ok)
        g_pf_pso_kq_mm44_th128 = compile_stamp(MetalKqMulMmIq4xsTH128_metal_kq_mulmm_iq4xs_t_msl, ok)
        g_pf_pso_kq_mm44_thdb = compile_stamp(MetalKqMulMmIq4xsTHDb_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm44_thdb128 = compile_stamp(MetalKqMulMmIq4xsTHDb128_metal_kq_mulmm_db_msl, ok)
    }
    g_pf_kq_mm35_tensor = metal_tensor_crowned("kq_mulmm_iq3s4")
    if (g_pf_kq_mm35_tensor) {
        g_pf_pso_kq_mm35_t = compile_stamp(MetalKqMulMmIq3s4T_metal_kq_mulmm_iq4xs_t_msl, ok)
        g_pf_pso_kq_mm35_th = compile_stamp(MetalKqMulMmIq3s4TH_metal_kq_mulmm_iq4xs_t_msl, ok)
        g_pf_pso_kq_mm35_th128 = compile_stamp(MetalKqMulMmIq3s4TH128_metal_kq_mulmm_iq4xs_t_msl, ok)
        g_pf_pso_kq_mm35_thdb = compile_stamp(MetalKqMulMmIq3s4THDb_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm35_thdb128 = compile_stamp(MetalKqMulMmIq3s4THDb128_metal_kq_mulmm_db_msl, ok)
    }
    g_pf_kq_mm45_tensor = metal_tensor_crowned("kq_mulmm_iq4nl")
    if (g_pf_kq_mm45_tensor) {
        g_pf_pso_kq_mm45_t = compile_stamp(MetalKqMulMmIq4nlT_metal_kq_mulmm_iq4xs_t_msl, ok)
        g_pf_pso_kq_mm45_th = compile_stamp(MetalKqMulMmIq4nlTH_metal_kq_mulmm_iq4xs_t_msl, ok)
        g_pf_pso_kq_mm45_th128 = compile_stamp(MetalKqMulMmIq4nlTH128_metal_kq_mulmm_iq4xs_t_msl, ok)
        g_pf_pso_kq_mm45_thdb = compile_stamp(MetalKqMulMmIq4nlTHDb_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm45_thdb128 = compile_stamp(MetalKqMulMmIq4nlTHDb128_metal_kq_mulmm_db_msl, ok)
    }
    g_pf_kq_mm40_tensor = metal_tensor_crowned("kq_mulmm_q40")
    if (g_pf_kq_mm40_tensor) {
        g_pf_pso_kq_mm40_t = compile_stamp(MetalKqMulMmQ40T_metal_kq_mulmm_iq4xs_t_msl, ok)
        g_pf_pso_kq_mm40_th = compile_stamp(MetalKqMulMmQ40TH_metal_kq_mulmm_iq4xs_t_msl, ok)
        g_pf_pso_kq_mm40_th128 = compile_stamp(MetalKqMulMmQ40TH128_metal_kq_mulmm_iq4xs_t_msl, ok)
        g_pf_pso_kq_mm40_thdb = compile_stamp(MetalKqMulMmQ40THDb_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm40_thdb128 = compile_stamp(MetalKqMulMmQ40THDb128_metal_kq_mulmm_db_msl, ok)
    }
    g_pf_kq_mm49_tensor = metal_tensor_crowned("kq_mulmm_nv4")
    if (g_pf_kq_mm49_tensor) {
        g_pf_pso_kq_mm49_t = compile_stamp(MetalKqMulMmNv4T_metal_kq_mulmm_iq4xs_t_msl, ok)
        g_pf_pso_kq_mm49_th = compile_stamp(MetalKqMulMmNv4TH_metal_kq_mulmm_iq4xs_t_msl, ok)
        g_pf_pso_kq_mm49_th128 = compile_stamp(MetalKqMulMmNv4TH128_metal_kq_mulmm_iq4xs_t_msl, ok)
        g_pf_pso_kq_mm49_thdb = compile_stamp(MetalKqMulMmNv4THDb_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm49_thdb128 = compile_stamp(MetalKqMulMmNv4THDb128_metal_kq_mulmm_db_msl, ok)
    }
}

//! the iquant/split-scale formats' tensor twins - compiled only behind each format's crown
[arch(at="../ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-kq-tensor-scaffold")]
def private pf_compile_kq_iquant_tensor_twins(var ok : bool&) {
    pf_compile_kq_iq4_tensor_twins(ok)
    g_pf_kq_mm3_tensor = metal_tensor_crowned("kq_mulmm_k3")
    if (g_pf_kq_mm3_tensor) {
        g_pf_pso_kq_mm3_t = compile_stamp(MetalKqMulMmK3T_metal_kq_mulmm_k3_t_msl, ok)
        g_pf_pso_kq_mm3_th = compile_stamp(MetalKqMulMmK3TH_metal_kq_mulmm_k3_t_msl, ok)
        g_pf_pso_kq_mm3_th128 = compile_stamp(MetalKqMulMmK3TH128_metal_kq_mulmm_k3_t_msl, ok)
        g_pf_pso_kq_mm3_thdb = compile_stamp(MetalKqMulMmK3THDb_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm3_thdb128 = compile_stamp(MetalKqMulMmK3THDb128_metal_kq_mulmm_db_msl, ok)
    }
    g_pf_kq_mm2_tensor = metal_tensor_crowned("kq_mulmm_k2")
    if (g_pf_kq_mm2_tensor) {
        g_pf_pso_kq_mm2_t = compile_stamp(MetalKqMulMmK2T_metal_kq_mulmm_k2_t_msl, ok)
        g_pf_pso_kq_mm2_th = compile_stamp(MetalKqMulMmK2TH_metal_kq_mulmm_k2_t_msl, ok)
        g_pf_pso_kq_mm2_th128 = compile_stamp(MetalKqMulMmK2TH128_metal_kq_mulmm_k2_t_msl, ok)
        g_pf_pso_kq_mm2_thdb = compile_stamp(MetalKqMulMmK2THDb_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm2_thdb128 = compile_stamp(MetalKqMulMmK2THDb128_metal_kq_mulmm_db_msl, ok)
    }
    g_pf_kq_mm33_tensor = metal_tensor_crowned("kq_mulmm_iq3s")
    if (g_pf_kq_mm33_tensor) {
        g_pf_pso_kq_mm33_t = compile_stamp(MetalKqMulMmIq3sT_metal_kq_mulmm_iq3s_t_msl, ok)
        g_pf_pso_kq_mm33_th = compile_stamp(MetalKqMulMmIq3sTH_metal_kq_mulmm_iq3s_t_msl, ok)
        g_pf_pso_kq_mm33_th128 = compile_stamp(MetalKqMulMmIq3sTH128_metal_kq_mulmm_iq3s_t_msl, ok)
        g_pf_pso_kq_mm33_thdb = compile_stamp(MetalKqMulMmIq3sTHDb_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm33_thdb128 = compile_stamp(MetalKqMulMmIq3sTHDb128_metal_kq_mulmm_db_msl, ok)
    }
    g_pf_kq_mm34_tensor = metal_tensor_crowned("kq_mulmm_iq3xxs")
    if (g_pf_kq_mm34_tensor) {
        g_pf_pso_kq_mm34_t = compile_stamp(MetalKqMulMmIq3xxsT_metal_kq_mulmm_iq3xxs_t_msl, ok)
        g_pf_pso_kq_mm34_th = compile_stamp(MetalKqMulMmIq3xxsTH_metal_kq_mulmm_iq3xxs_t_msl, ok)
        g_pf_pso_kq_mm34_th128 = compile_stamp(MetalKqMulMmIq3xxsTH128_metal_kq_mulmm_iq3xxs_t_msl, ok)
        g_pf_pso_kq_mm34_thdb = compile_stamp(MetalKqMulMmIq3xxsTHDb_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm34_thdb128 = compile_stamp(MetalKqMulMmIq3xxsTHDb128_metal_kq_mulmm_db_msl, ok)
    }
    g_pf_kq_mm23_tensor = metal_tensor_crowned("kq_mulmm_iq2s")
    if (g_pf_kq_mm23_tensor) {
        g_pf_pso_kq_mm23_t = compile_stamp(MetalKqMulMmIq2sT_metal_kq_mulmm_iq2s_t_msl, ok)
        g_pf_pso_kq_mm23_th = compile_stamp(MetalKqMulMmIq2sTH_metal_kq_mulmm_iq2s_t_msl, ok)
        g_pf_pso_kq_mm23_th128 = compile_stamp(MetalKqMulMmIq2sTH128_metal_kq_mulmm_iq2s_t_msl, ok)
        g_pf_pso_kq_mm23_thdb = compile_stamp(MetalKqMulMmIq2sTHDb_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm23_thdb128 = compile_stamp(MetalKqMulMmIq2sTHDb128_metal_kq_mulmm_db_msl, ok)
    }
    g_pf_kq_mm24_tensor = metal_tensor_crowned("kq_mulmm_iq2xs")
    if (g_pf_kq_mm24_tensor) {
        g_pf_pso_kq_mm24_t = compile_stamp(MetalKqMulMmIq2xsT_metal_kq_mulmm_iq2xs_t_msl, ok)
        g_pf_pso_kq_mm24_th = compile_stamp(MetalKqMulMmIq2xsTH_metal_kq_mulmm_iq2xs_t_msl, ok)
        g_pf_pso_kq_mm24_th128 = compile_stamp(MetalKqMulMmIq2xsTH128_metal_kq_mulmm_iq2xs_t_msl, ok)
        g_pf_pso_kq_mm24_thdb = compile_stamp(MetalKqMulMmIq2xsTHDb_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm24_thdb128 = compile_stamp(MetalKqMulMmIq2xsTHDb128_metal_kq_mulmm_db_msl, ok)
    }
    g_pf_kq_mm25_tensor = metal_tensor_crowned("kq_mulmm_iq2xxs")
    if (g_pf_kq_mm25_tensor) {
        g_pf_pso_kq_mm25_t = compile_stamp(MetalKqMulMmIq2xxsT_metal_kq_mulmm_iq2xxs_t_msl, ok)
        g_pf_pso_kq_mm25_th = compile_stamp(MetalKqMulMmIq2xxsTH_metal_kq_mulmm_iq2xxs_t_msl, ok)
        g_pf_pso_kq_mm25_th128 = compile_stamp(MetalKqMulMmIq2xxsTH128_metal_kq_mulmm_iq2xxs_t_msl, ok)
        g_pf_pso_kq_mm25_thdb = compile_stamp(MetalKqMulMmIq2xxsTHDb_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm25_thdb128 = compile_stamp(MetalKqMulMmIq2xxsTHDb128_metal_kq_mulmm_db_msl, ok)
    }
}

//! the per-32 formats' dense tensor twins - the same five forms, behind each format's own crown
def private pf_compile_kq_b32_tensor_twins(var ok : bool&) {
    g_pf_kq_mm51_tensor = metal_tensor_crowned("kq_mulmm_q51")
    if (g_pf_kq_mm51_tensor) {
        g_pf_pso_kq_mm51_t = compile_stamp(MetalKqMulMmQ51T_metal_kq_mulmm_b32_t_msl, ok)
        g_pf_pso_kq_mm51_th = compile_stamp(MetalKqMulMmQ51TH_metal_kq_mulmm_b32_t_msl, ok)
        g_pf_pso_kq_mm51_th128 = compile_stamp(MetalKqMulMmQ51TH128_metal_kq_mulmm_b32_t_msl, ok)
        g_pf_pso_kq_mm51_thdb = compile_stamp(MetalKqMulMmQ51THDb_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm51_thdb128 = compile_stamp(MetalKqMulMmQ51THDb128_metal_kq_mulmm_db_msl, ok)
    }
    g_pf_kq_mm4n32_tensor = metal_tensor_crowned("kq_mulmm_iq4nl32")
    if (g_pf_kq_mm4n32_tensor) {
        g_pf_pso_kq_mm4n32_t = compile_stamp(MetalKqMulMmIq4nl32T_metal_kq_mulmm_b32_t_msl, ok)
        g_pf_pso_kq_mm4n32_th = compile_stamp(MetalKqMulMmIq4nl32TH_metal_kq_mulmm_b32_t_msl, ok)
        g_pf_pso_kq_mm4n32_th128 = compile_stamp(MetalKqMulMmIq4nl32TH128_metal_kq_mulmm_b32_t_msl, ok)
        g_pf_pso_kq_mm4n32_thdb = compile_stamp(MetalKqMulMmIq4nl32THDb_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm4n32_thdb128 = compile_stamp(MetalKqMulMmIq4nl32THDb128_metal_kq_mulmm_db_msl, ok)
    }
    g_pf_kq_mm4032_tensor = metal_tensor_crowned("kq_mulmm_q4032")
    if (g_pf_kq_mm4032_tensor) {
        g_pf_pso_kq_mm4032_t = compile_stamp(MetalKqMulMmQ4032T_metal_kq_mulmm_b32_t_msl, ok)
        g_pf_pso_kq_mm4032_th = compile_stamp(MetalKqMulMmQ4032TH_metal_kq_mulmm_b32_t_msl, ok)
        g_pf_pso_kq_mm4032_th128 = compile_stamp(MetalKqMulMmQ4032TH128_metal_kq_mulmm_b32_t_msl, ok)
        g_pf_pso_kq_mm4032_thdb = compile_stamp(MetalKqMulMmQ4032THDb_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm4032_thdb128 = compile_stamp(MetalKqMulMmQ4032THDb128_metal_kq_mulmm_db_msl, ok)
    }
    g_pf_kq_mm5032_tensor = metal_tensor_crowned("kq_mulmm_q5032")
    if (g_pf_kq_mm5032_tensor) {
        g_pf_pso_kq_mm5032_t = compile_stamp(MetalKqMulMmQ5032T_metal_kq_mulmm_b32_t_msl, ok)
        g_pf_pso_kq_mm5032_th = compile_stamp(MetalKqMulMmQ5032TH_metal_kq_mulmm_b32_t_msl, ok)
        g_pf_pso_kq_mm5032_th128 = compile_stamp(MetalKqMulMmQ5032TH128_metal_kq_mulmm_b32_t_msl, ok)
        g_pf_pso_kq_mm5032_thdb = compile_stamp(MetalKqMulMmQ5032THDb_metal_kq_mulmm_db_msl, ok)
        g_pf_pso_kq_mm5032_thdb128 = compile_stamp(MetalKqMulMmQ5032THDb128_metal_kq_mulmm_db_msl, ok)
    }
}

def private pf_compile_kq_dequant_passes(var ok : bool&) {
    g_pf_pso_dq_kq44 = compile_stamp(MetalKqDequantIq4xs_metal_kq_dequant_msl, ok)
    g_pf_pso_dq_kq35 = compile_stamp(MetalKqDequantIq3s4_metal_kq_dequant_msl, ok)
    g_pf_pso_dq_kq45 = compile_stamp(MetalKqDequantIq4nl_metal_kq_dequant_msl, ok)
    g_pf_pso_dq_kq40 = compile_stamp(MetalKqDequantQ40_metal_kq_dequant_msl, ok)
    g_pf_pso_dq_kq49 = compile_stamp(MetalKqDequantNv4_metal_kq_dequant_msl, ok)
    g_pf_pso_dq_kq3 = compile_stamp(MetalKqDequantK3_metal_kq_dequant_msl, ok)
    g_pf_pso_dq_kq33 = compile_stamp(MetalKqDequantIq3s_metal_kq_dequant_msl, ok)
    g_pf_pso_dq_kq34 = compile_stamp(MetalKqDequantIq3xxs_metal_kq_dequant_msl, ok)
    g_pf_pso_dq_kq2 = compile_stamp(MetalKqDequantK2_metal_kq_dequant_msl, ok)
    g_pf_pso_dq_kq23 = compile_stamp(MetalKqDequantIq2s_metal_kq_dequant_msl, ok)
    g_pf_pso_dq_kq24 = compile_stamp(MetalKqDequantIq2xs_metal_kq_dequant_msl, ok)
    g_pf_pso_dq_kq25 = compile_stamp(MetalKqDequantIq2xxs_metal_kq_dequant_msl, ok)
    g_pf_pso_dq_kq51 = compile_stamp(MetalKqDequantQ51_metal_kq_dequant_msl, ok)
    g_pf_pso_dq_kq4n32 = compile_stamp(MetalKqDequantIq4nl32_metal_kq_dequant_msl, ok)
    g_pf_pso_dq_kq4032 = compile_stamp(MetalKqDequantQ4032_metal_kq_dequant_msl, ok)
    g_pf_pso_dq_kq5032 = compile_stamp(MetalKqDequantQ5032_metal_kq_dequant_msl, ok)
}

typedef KqSplitEnc = function<(enc : MetalComputeEncoder?; bkdh : MetalBuffer?; s0off : uint64; bksc : MetalBuffer?; soff : uint64;
                               bkqu : MetalBuffer?; qoff : uint64; bxf : MetalBuffer?; xoff : uint64; by : MetalBuffer?; yoff : uint64;
                               bkdim : MetalBuffer?; bndim : MetalBuffer?; mp : int64; rows : int64) : void>

//! a split-K stamp's builder: the dense builder's arguments and the K split
typedef KqSplitEncSk = function<(enc : MetalComputeEncoder?; bkdh : MetalBuffer?; s0off : uint64; bksc : MetalBuffer?; soff : uint64;
                                 bkqu : MetalBuffer?; qoff : uint64; bxf : MetalBuffer?; xoff : uint64; byk : MetalBuffer?; yoff : uint64;
                                 bkdim : MetalBuffer?; bndim : MetalBuffer?; mp : int64; rows : int64; nsk : int64) : void>

//! one dense format's crown flag, compiled twins and their builders, in one row: the t (f32 X), th (half X), th128 (tall),
//! thdb and thdb128 (double-buffered) forms every dense format stamps; `d_soff` binds slot 0 at the compact scale plane's
//! soff (k4, k5, iq4nl, q40 and the per-32 planes carry no d tail)
struct private KqDenseStamps {
    crown, d_soff : bool
    pso_t, pso_th, pso_th128, pso_thdb, pso_thdb128, pso_thsk, pso_tsk, pso_thsk16, pso_tsk16 : MetalComputePipeline?
    enc_t, enc_th, enc_th128, enc_thdb, enc_thdb128 : KqSplitEnc
    enc_thsk, enc_tsk, enc_thsk16, enc_tsk16 : KqSplitEncSk
}

def private pf_kq_dense_stamps(fmt : KqFmt) : KqDenseStamps => kq_dense_b32(fmt) ? pf_kq_b32_stamps(fmt) : pf_kq_sb_stamps(fmt)

//! the superblock formats' rows
def private pf_kq_sb_stamps(fmt : KqFmt) : KqDenseStamps {   // nolint:STYLE038 — one table row a format
    if (fmt == KqFmt.k4) return KqDenseStamps(crown = g_pf_kq_mm4_tensor, d_soff = true,
        pso_t = g_pf_pso_kq_mm4_t, pso_th = g_pf_pso_kq_mm4_th, pso_th128 = g_pf_pso_kq_mm4_th128,
        pso_thdb = g_pf_pso_kq_mm4_thdb, pso_thdb128 = g_pf_pso_kq_mm4_thdb128,
        enc_t = @@enc_kq_mm_k4_t_c, enc_th = @@enc_kq_mm_k4_th_c, enc_th128 = @@enc_kq_mm_k4_th128_c,
        enc_thdb = @@enc_kq_mm_k4_thdb_c, enc_thdb128 = @@enc_kq_mm_k4_thdb128_c,
        pso_thsk = g_pf_pso_kq_mm4_thsk, pso_tsk = g_pf_pso_kq_mm4_tsk, enc_thsk = @@enc_kq_mm_k4_thsk_c, enc_tsk = @@enc_kq_mm_k4_tsk_c,
        pso_thsk16 = g_pf_pso_kq_mm4_thsk16, pso_tsk16 = g_pf_pso_kq_mm4_tsk16, enc_thsk16 = @@enc_kq_mm_k4_thsk16_c, enc_tsk16 = @@enc_kq_mm_k4_tsk16_c)
    if (fmt == KqFmt.k5) return KqDenseStamps(crown = g_pf_kq_mm5_tensor, d_soff = true,
        pso_t = g_pf_pso_kq_mm5_t, pso_th = g_pf_pso_kq_mm5_th, pso_th128 = g_pf_pso_kq_mm5_th128,
        pso_thdb = g_pf_pso_kq_mm5_thdb, pso_thdb128 = g_pf_pso_kq_mm5_thdb128,
        enc_t = @@enc_kq_mm_k5_t_c, enc_th = @@enc_kq_mm_k5_th_c, enc_th128 = @@enc_kq_mm_k5_th128_c,
        enc_thdb = @@enc_kq_mm_k5_thdb_c, enc_thdb128 = @@enc_kq_mm_k5_thdb128_c,
        pso_thsk = g_pf_pso_kq_mm5_thsk, pso_tsk = g_pf_pso_kq_mm5_tsk, enc_thsk = @@enc_kq_mm_k5_thsk_c, enc_tsk = @@enc_kq_mm_k5_tsk_c,
        pso_thsk16 = g_pf_pso_kq_mm5_thsk16, pso_tsk16 = g_pf_pso_kq_mm5_tsk16, enc_thsk16 = @@enc_kq_mm_k5_thsk16_c, enc_tsk16 = @@enc_kq_mm_k5_tsk16_c)
    if (fmt == KqFmt.k6) return KqDenseStamps(crown = g_pf_kq_mm6_tensor, d_soff = false,
        pso_t = g_pf_pso_kq_mm6_t, pso_th = g_pf_pso_kq_mm6_th, pso_th128 = g_pf_pso_kq_mm6_th128,
        pso_thdb = g_pf_pso_kq_mm6_thdb, pso_thdb128 = g_pf_pso_kq_mm6_thdb128,
        enc_t = @@enc_kq_mm_k6_t_c, enc_th = @@enc_kq_mm_k6_th_c, enc_th128 = @@enc_kq_mm_k6_th128_c,
        enc_thdb = @@enc_kq_mm_k6_thdb_c, enc_thdb128 = @@enc_kq_mm_k6_thdb128_c,
        pso_thsk = g_pf_pso_kq_mm6_thsk, pso_tsk = g_pf_pso_kq_mm6_tsk, enc_thsk = @@enc_kq_mm_k6_thsk_c, enc_tsk = @@enc_kq_mm_k6_tsk_c,
        pso_thsk16 = g_pf_pso_kq_mm6_thsk16, pso_tsk16 = g_pf_pso_kq_mm6_tsk16, enc_thsk16 = @@enc_kq_mm_k6_thsk16_c, enc_tsk16 = @@enc_kq_mm_k6_tsk16_c)
    if (fmt == KqFmt.iq4xs) return KqDenseStamps(crown = g_pf_kq_mm44_tensor, d_soff = false,
        pso_t = g_pf_pso_kq_mm44_t, pso_th = g_pf_pso_kq_mm44_th, pso_th128 = g_pf_pso_kq_mm44_th128,
        pso_thdb = g_pf_pso_kq_mm44_thdb, pso_thdb128 = g_pf_pso_kq_mm44_thdb128,
        enc_t = @@enc_kq_mm_iq4xs_t_c, enc_th = @@enc_kq_mm_iq4xs_th_c, enc_th128 = @@enc_kq_mm_iq4xs_th128_c,
        enc_thdb = @@enc_kq_mm_iq4xs_thdb_c, enc_thdb128 = @@enc_kq_mm_iq4xs_thdb128_c,
        pso_thsk = g_pf_pso_kq_mm44_thsk, pso_tsk = g_pf_pso_kq_mm44_tsk, enc_thsk = @@enc_kq_mm_iq4xs_thsk_c, enc_tsk = @@enc_kq_mm_iq4xs_tsk_c,
        pso_thsk16 = g_pf_pso_kq_mm44_thsk16, pso_tsk16 = g_pf_pso_kq_mm44_tsk16, enc_thsk16 = @@enc_kq_mm_iq4xs_thsk16_c, enc_tsk16 = @@enc_kq_mm_iq4xs_tsk16_c)
    if (fmt == KqFmt.iq3s4) return KqDenseStamps(crown = g_pf_kq_mm35_tensor, d_soff = false,
        pso_t = g_pf_pso_kq_mm35_t, pso_th = g_pf_pso_kq_mm35_th, pso_th128 = g_pf_pso_kq_mm35_th128,
        pso_thdb = g_pf_pso_kq_mm35_thdb, pso_thdb128 = g_pf_pso_kq_mm35_thdb128,
        enc_t = @@enc_kq_mm_iq3s4_t_c, enc_th = @@enc_kq_mm_iq3s4_th_c, enc_th128 = @@enc_kq_mm_iq3s4_th128_c,
        enc_thdb = @@enc_kq_mm_iq3s4_thdb_c, enc_thdb128 = @@enc_kq_mm_iq3s4_thdb128_c,
        pso_thsk = g_pf_pso_kq_mm35_thsk, pso_tsk = g_pf_pso_kq_mm35_tsk, enc_thsk = @@enc_kq_mm_iq3s4_thsk_c, enc_tsk = @@enc_kq_mm_iq3s4_tsk_c,
        pso_thsk16 = g_pf_pso_kq_mm35_thsk16, pso_tsk16 = g_pf_pso_kq_mm35_tsk16, enc_thsk16 = @@enc_kq_mm_iq3s4_thsk16_c, enc_tsk16 = @@enc_kq_mm_iq3s4_tsk16_c)
    if (fmt == KqFmt.iq4nl) return KqDenseStamps(crown = g_pf_kq_mm45_tensor, d_soff = true,
        pso_t = g_pf_pso_kq_mm45_t, pso_th = g_pf_pso_kq_mm45_th, pso_th128 = g_pf_pso_kq_mm45_th128,
        pso_thdb = g_pf_pso_kq_mm45_thdb, pso_thdb128 = g_pf_pso_kq_mm45_thdb128,
        enc_t = @@enc_kq_mm_iq4nl_t_c, enc_th = @@enc_kq_mm_iq4nl_th_c, enc_th128 = @@enc_kq_mm_iq4nl_th128_c,
        enc_thdb = @@enc_kq_mm_iq4nl_thdb_c, enc_thdb128 = @@enc_kq_mm_iq4nl_thdb128_c,
        pso_thsk = g_pf_pso_kq_mm45_thsk, pso_tsk = g_pf_pso_kq_mm45_tsk, enc_thsk = @@enc_kq_mm_iq4nl_thsk_c, enc_tsk = @@enc_kq_mm_iq4nl_tsk_c,
        pso_thsk16 = g_pf_pso_kq_mm45_thsk16, pso_tsk16 = g_pf_pso_kq_mm45_tsk16, enc_thsk16 = @@enc_kq_mm_iq4nl_thsk16_c, enc_tsk16 = @@enc_kq_mm_iq4nl_tsk16_c)
    if (fmt == KqFmt.q40) return KqDenseStamps(crown = g_pf_kq_mm40_tensor, d_soff = true,
        pso_t = g_pf_pso_kq_mm40_t, pso_th = g_pf_pso_kq_mm40_th, pso_th128 = g_pf_pso_kq_mm40_th128,
        pso_thdb = g_pf_pso_kq_mm40_thdb, pso_thdb128 = g_pf_pso_kq_mm40_thdb128,
        enc_t = @@enc_kq_mm_q40_t_c, enc_th = @@enc_kq_mm_q40_th_c, enc_th128 = @@enc_kq_mm_q40_th128_c,
        enc_thdb = @@enc_kq_mm_q40_thdb_c, enc_thdb128 = @@enc_kq_mm_q40_thdb128_c,
        pso_thsk = g_pf_pso_kq_mm40_thsk, pso_tsk = g_pf_pso_kq_mm40_tsk, enc_thsk = @@enc_kq_mm_q40_thsk_c, enc_tsk = @@enc_kq_mm_q40_tsk_c,
        pso_thsk16 = g_pf_pso_kq_mm40_thsk16, pso_tsk16 = g_pf_pso_kq_mm40_tsk16, enc_thsk16 = @@enc_kq_mm_q40_thsk16_c, enc_tsk16 = @@enc_kq_mm_q40_tsk16_c)
    if (fmt == KqFmt.nv4) return KqDenseStamps(crown = g_pf_kq_mm49_tensor, d_soff = true,
        pso_t = g_pf_pso_kq_mm49_t, pso_th = g_pf_pso_kq_mm49_th, pso_th128 = g_pf_pso_kq_mm49_th128,
        pso_thdb = g_pf_pso_kq_mm49_thdb, pso_thdb128 = g_pf_pso_kq_mm49_thdb128,
        enc_t = @@enc_kq_mm_nv4_t_c, enc_th = @@enc_kq_mm_nv4_th_c, enc_th128 = @@enc_kq_mm_nv4_th128_c,
        enc_thdb = @@enc_kq_mm_nv4_thdb_c, enc_thdb128 = @@enc_kq_mm_nv4_thdb128_c,
        pso_thsk = g_pf_pso_kq_mm49_thsk, pso_tsk = g_pf_pso_kq_mm49_tsk, enc_thsk = @@enc_kq_mm_nv4_thsk_c, enc_tsk = @@enc_kq_mm_nv4_tsk_c,
        pso_thsk16 = g_pf_pso_kq_mm49_thsk16, pso_tsk16 = g_pf_pso_kq_mm49_tsk16, enc_thsk16 = @@enc_kq_mm_nv4_thsk16_c, enc_tsk16 = @@enc_kq_mm_nv4_tsk16_c)
    if (fmt == KqFmt.k3) return KqDenseStamps(crown = g_pf_kq_mm3_tensor, d_soff = false,
        pso_t = g_pf_pso_kq_mm3_t, pso_th = g_pf_pso_kq_mm3_th, pso_th128 = g_pf_pso_kq_mm3_th128,
        pso_thdb = g_pf_pso_kq_mm3_thdb, pso_thdb128 = g_pf_pso_kq_mm3_thdb128,
        enc_t = @@enc_kq_mm_k3_t_c, enc_th = @@enc_kq_mm_k3_th_c, enc_th128 = @@enc_kq_mm_k3_th128_c,
        enc_thdb = @@enc_kq_mm_k3_thdb_c, enc_thdb128 = @@enc_kq_mm_k3_thdb128_c,
        pso_thsk = g_pf_pso_kq_mm3_thsk, pso_tsk = g_pf_pso_kq_mm3_tsk, enc_thsk = @@enc_kq_mm_k3_thsk_c, enc_tsk = @@enc_kq_mm_k3_tsk_c,
        pso_thsk16 = g_pf_pso_kq_mm3_thsk16, pso_tsk16 = g_pf_pso_kq_mm3_tsk16, enc_thsk16 = @@enc_kq_mm_k3_thsk16_c, enc_tsk16 = @@enc_kq_mm_k3_tsk16_c)
    if (fmt == KqFmt.k2) return KqDenseStamps(crown = g_pf_kq_mm2_tensor, d_soff = false,
        pso_t = g_pf_pso_kq_mm2_t, pso_th = g_pf_pso_kq_mm2_th, pso_th128 = g_pf_pso_kq_mm2_th128,
        pso_thdb = g_pf_pso_kq_mm2_thdb, pso_thdb128 = g_pf_pso_kq_mm2_thdb128,
        enc_t = @@enc_kq_mm_k2_t_c, enc_th = @@enc_kq_mm_k2_th_c, enc_th128 = @@enc_kq_mm_k2_th128_c,
        enc_thdb = @@enc_kq_mm_k2_thdb_c, enc_thdb128 = @@enc_kq_mm_k2_thdb128_c,
        pso_thsk = g_pf_pso_kq_mm2_thsk, pso_tsk = g_pf_pso_kq_mm2_tsk, enc_thsk = @@enc_kq_mm_k2_thsk_c, enc_tsk = @@enc_kq_mm_k2_tsk_c,
        pso_thsk16 = g_pf_pso_kq_mm2_thsk16, pso_tsk16 = g_pf_pso_kq_mm2_tsk16, enc_thsk16 = @@enc_kq_mm_k2_thsk16_c, enc_tsk16 = @@enc_kq_mm_k2_tsk16_c)
    if (fmt == KqFmt.iq3s) return KqDenseStamps(crown = g_pf_kq_mm33_tensor, d_soff = false,
        pso_t = g_pf_pso_kq_mm33_t, pso_th = g_pf_pso_kq_mm33_th, pso_th128 = g_pf_pso_kq_mm33_th128,
        pso_thdb = g_pf_pso_kq_mm33_thdb, pso_thdb128 = g_pf_pso_kq_mm33_thdb128,
        enc_t = @@enc_kq_mm_iq3s_t_c, enc_th = @@enc_kq_mm_iq3s_th_c, enc_th128 = @@enc_kq_mm_iq3s_th128_c,
        enc_thdb = @@enc_kq_mm_iq3s_thdb_c, enc_thdb128 = @@enc_kq_mm_iq3s_thdb128_c,
        pso_thsk = g_pf_pso_kq_mm33_thsk, pso_tsk = g_pf_pso_kq_mm33_tsk, enc_thsk = @@enc_kq_mm_iq3s_thsk_c, enc_tsk = @@enc_kq_mm_iq3s_tsk_c,
        pso_thsk16 = g_pf_pso_kq_mm33_thsk16, pso_tsk16 = g_pf_pso_kq_mm33_tsk16, enc_thsk16 = @@enc_kq_mm_iq3s_thsk16_c, enc_tsk16 = @@enc_kq_mm_iq3s_tsk16_c)
    if (fmt == KqFmt.iq3xxs) return KqDenseStamps(crown = g_pf_kq_mm34_tensor, d_soff = false,
        pso_t = g_pf_pso_kq_mm34_t, pso_th = g_pf_pso_kq_mm34_th, pso_th128 = g_pf_pso_kq_mm34_th128,
        pso_thdb = g_pf_pso_kq_mm34_thdb, pso_thdb128 = g_pf_pso_kq_mm34_thdb128,
        enc_t = @@enc_kq_mm_iq3xxs_t_c, enc_th = @@enc_kq_mm_iq3xxs_th_c, enc_th128 = @@enc_kq_mm_iq3xxs_th128_c,
        enc_thdb = @@enc_kq_mm_iq3xxs_thdb_c, enc_thdb128 = @@enc_kq_mm_iq3xxs_thdb128_c,
        pso_thsk = g_pf_pso_kq_mm34_thsk, pso_tsk = g_pf_pso_kq_mm34_tsk, enc_thsk = @@enc_kq_mm_iq3xxs_thsk_c, enc_tsk = @@enc_kq_mm_iq3xxs_tsk_c,
        pso_thsk16 = g_pf_pso_kq_mm34_thsk16, pso_tsk16 = g_pf_pso_kq_mm34_tsk16, enc_thsk16 = @@enc_kq_mm_iq3xxs_thsk16_c, enc_tsk16 = @@enc_kq_mm_iq3xxs_tsk16_c)
    if (fmt == KqFmt.iq2s) return KqDenseStamps(crown = g_pf_kq_mm23_tensor, d_soff = false,
        pso_t = g_pf_pso_kq_mm23_t, pso_th = g_pf_pso_kq_mm23_th, pso_th128 = g_pf_pso_kq_mm23_th128,
        pso_thdb = g_pf_pso_kq_mm23_thdb, pso_thdb128 = g_pf_pso_kq_mm23_thdb128,
        enc_t = @@enc_kq_mm_iq2s_t_c, enc_th = @@enc_kq_mm_iq2s_th_c, enc_th128 = @@enc_kq_mm_iq2s_th128_c,
        enc_thdb = @@enc_kq_mm_iq2s_thdb_c, enc_thdb128 = @@enc_kq_mm_iq2s_thdb128_c,
        pso_thsk = g_pf_pso_kq_mm23_thsk, pso_tsk = g_pf_pso_kq_mm23_tsk, enc_thsk = @@enc_kq_mm_iq2s_thsk_c, enc_tsk = @@enc_kq_mm_iq2s_tsk_c,
        pso_thsk16 = g_pf_pso_kq_mm23_thsk16, pso_tsk16 = g_pf_pso_kq_mm23_tsk16, enc_thsk16 = @@enc_kq_mm_iq2s_thsk16_c, enc_tsk16 = @@enc_kq_mm_iq2s_tsk16_c)
    if (fmt == KqFmt.iq2xs) return KqDenseStamps(crown = g_pf_kq_mm24_tensor, d_soff = false,
        pso_t = g_pf_pso_kq_mm24_t, pso_th = g_pf_pso_kq_mm24_th, pso_th128 = g_pf_pso_kq_mm24_th128,
        pso_thdb = g_pf_pso_kq_mm24_thdb, pso_thdb128 = g_pf_pso_kq_mm24_thdb128,
        enc_t = @@enc_kq_mm_iq2xs_t_c, enc_th = @@enc_kq_mm_iq2xs_th_c, enc_th128 = @@enc_kq_mm_iq2xs_th128_c,
        enc_thdb = @@enc_kq_mm_iq2xs_thdb_c, enc_thdb128 = @@enc_kq_mm_iq2xs_thdb128_c,
        pso_thsk = g_pf_pso_kq_mm24_thsk, pso_tsk = g_pf_pso_kq_mm24_tsk, enc_thsk = @@enc_kq_mm_iq2xs_thsk_c, enc_tsk = @@enc_kq_mm_iq2xs_tsk_c,
        pso_thsk16 = g_pf_pso_kq_mm24_thsk16, pso_tsk16 = g_pf_pso_kq_mm24_tsk16, enc_thsk16 = @@enc_kq_mm_iq2xs_thsk16_c, enc_tsk16 = @@enc_kq_mm_iq2xs_tsk16_c)
    if (fmt == KqFmt.iq2xxs) return KqDenseStamps(crown = g_pf_kq_mm25_tensor, d_soff = false,
        pso_t = g_pf_pso_kq_mm25_t, pso_th = g_pf_pso_kq_mm25_th, pso_th128 = g_pf_pso_kq_mm25_th128,
        pso_thdb = g_pf_pso_kq_mm25_thdb, pso_thdb128 = g_pf_pso_kq_mm25_thdb128,
        enc_t = @@enc_kq_mm_iq2xxs_t_c, enc_th = @@enc_kq_mm_iq2xxs_th_c, enc_th128 = @@enc_kq_mm_iq2xxs_th128_c,
        enc_thdb = @@enc_kq_mm_iq2xxs_thdb_c, enc_thdb128 = @@enc_kq_mm_iq2xxs_thdb128_c,
        pso_thsk = g_pf_pso_kq_mm25_thsk, pso_tsk = g_pf_pso_kq_mm25_tsk, enc_thsk = @@enc_kq_mm_iq2xxs_thsk_c, enc_tsk = @@enc_kq_mm_iq2xxs_tsk_c,
        pso_thsk16 = g_pf_pso_kq_mm25_thsk16, pso_tsk16 = g_pf_pso_kq_mm25_tsk16, enc_thsk16 = @@enc_kq_mm_iq2xxs_thsk16_c, enc_tsk16 = @@enc_kq_mm_iq2xxs_tsk16_c)
    return KqDenseStamps(crown = false)   //! the dispatcher admits only the rows above
}

//! the per-32 formats' rows
def private pf_kq_b32_stamps(fmt : KqFmt) : KqDenseStamps {
    if (fmt == KqFmt.q51) return KqDenseStamps(crown = g_pf_kq_mm51_tensor, d_soff = true,
        pso_t = g_pf_pso_kq_mm51_t, pso_th = g_pf_pso_kq_mm51_th, pso_th128 = g_pf_pso_kq_mm51_th128,
        pso_thdb = g_pf_pso_kq_mm51_thdb, pso_thdb128 = g_pf_pso_kq_mm51_thdb128,
        enc_t = @@enc_kq_mm_q51_t_c, enc_th = @@enc_kq_mm_q51_th_c, enc_th128 = @@enc_kq_mm_q51_th128_c,
        enc_thdb = @@enc_kq_mm_q51_thdb_c, enc_thdb128 = @@enc_kq_mm_q51_thdb128_c,
        pso_thsk = g_pf_pso_kq_mm51_thsk, pso_tsk = g_pf_pso_kq_mm51_tsk, enc_thsk = @@enc_kq_mm_q51_thsk_c, enc_tsk = @@enc_kq_mm_q51_tsk_c,
        pso_thsk16 = g_pf_pso_kq_mm51_thsk16, pso_tsk16 = g_pf_pso_kq_mm51_tsk16, enc_thsk16 = @@enc_kq_mm_q51_thsk16_c, enc_tsk16 = @@enc_kq_mm_q51_tsk16_c)
    if (fmt == KqFmt.iq4nl32) return KqDenseStamps(crown = g_pf_kq_mm4n32_tensor, d_soff = true,
        pso_t = g_pf_pso_kq_mm4n32_t, pso_th = g_pf_pso_kq_mm4n32_th, pso_th128 = g_pf_pso_kq_mm4n32_th128,
        pso_thdb = g_pf_pso_kq_mm4n32_thdb, pso_thdb128 = g_pf_pso_kq_mm4n32_thdb128,
        enc_t = @@enc_kq_mm_iq4nl32_t_c, enc_th = @@enc_kq_mm_iq4nl32_th_c, enc_th128 = @@enc_kq_mm_iq4nl32_th128_c,
        enc_thdb = @@enc_kq_mm_iq4nl32_thdb_c, enc_thdb128 = @@enc_kq_mm_iq4nl32_thdb128_c,
        pso_thsk = g_pf_pso_kq_mm4n32_thsk, pso_tsk = g_pf_pso_kq_mm4n32_tsk, enc_thsk = @@enc_kq_mm_iq4nl32_thsk_c, enc_tsk = @@enc_kq_mm_iq4nl32_tsk_c,
        pso_thsk16 = g_pf_pso_kq_mm4n32_thsk16, pso_tsk16 = g_pf_pso_kq_mm4n32_tsk16, enc_thsk16 = @@enc_kq_mm_iq4nl32_thsk16_c, enc_tsk16 = @@enc_kq_mm_iq4nl32_tsk16_c)
    if (fmt == KqFmt.q4032) return KqDenseStamps(crown = g_pf_kq_mm4032_tensor, d_soff = true,
        pso_t = g_pf_pso_kq_mm4032_t, pso_th = g_pf_pso_kq_mm4032_th, pso_th128 = g_pf_pso_kq_mm4032_th128,
        pso_thdb = g_pf_pso_kq_mm4032_thdb, pso_thdb128 = g_pf_pso_kq_mm4032_thdb128,
        enc_t = @@enc_kq_mm_q4032_t_c, enc_th = @@enc_kq_mm_q4032_th_c, enc_th128 = @@enc_kq_mm_q4032_th128_c,
        enc_thdb = @@enc_kq_mm_q4032_thdb_c, enc_thdb128 = @@enc_kq_mm_q4032_thdb128_c,
        pso_thsk = g_pf_pso_kq_mm4032_thsk, pso_tsk = g_pf_pso_kq_mm4032_tsk, enc_thsk = @@enc_kq_mm_q4032_thsk_c, enc_tsk = @@enc_kq_mm_q4032_tsk_c,
        pso_thsk16 = g_pf_pso_kq_mm4032_thsk16, pso_tsk16 = g_pf_pso_kq_mm4032_tsk16, enc_thsk16 = @@enc_kq_mm_q4032_thsk16_c, enc_tsk16 = @@enc_kq_mm_q4032_tsk16_c)
    if (fmt == KqFmt.q5032) return KqDenseStamps(crown = g_pf_kq_mm5032_tensor, d_soff = true,
        pso_t = g_pf_pso_kq_mm5032_t, pso_th = g_pf_pso_kq_mm5032_th, pso_th128 = g_pf_pso_kq_mm5032_th128,
        pso_thdb = g_pf_pso_kq_mm5032_thdb, pso_thdb128 = g_pf_pso_kq_mm5032_thdb128,
        enc_t = @@enc_kq_mm_q5032_t_c, enc_th = @@enc_kq_mm_q5032_th_c, enc_th128 = @@enc_kq_mm_q5032_th128_c,
        enc_thdb = @@enc_kq_mm_q5032_thdb_c, enc_thdb128 = @@enc_kq_mm_q5032_thdb128_c,
        pso_thsk = g_pf_pso_kq_mm5032_thsk, pso_tsk = g_pf_pso_kq_mm5032_tsk, enc_thsk = @@enc_kq_mm_q5032_thsk_c, enc_tsk = @@enc_kq_mm_q5032_tsk_c,
        pso_thsk16 = g_pf_pso_kq_mm5032_thsk16, pso_tsk16 = g_pf_pso_kq_mm5032_tsk16, enc_thsk16 = @@enc_kq_mm_q5032_thsk16_c, enc_tsk16 = @@enc_kq_mm_q5032_tsk16_c)
    return KqDenseStamps(crown = false)
}

[arch(at="../ARCHITECTURE_GPU_PREFILL.md#prefill-last-row-tail")]
def private pf_enc_narrow_gemv(enc : MetalComputeEncoder?; t : Model; fmt : KqFmt; woff : int64; bx, by, bk, bn : MetalBuffer?; kdim, d : int64) {
    enc_site_gemv(enc, t, fmt, woff, d, kdim, bx, by, bk, bn)
}

//! one weight site of the window by its tag: a K-quant plane through its format's mul_mm site, a q8 plane through the blob GEMM
def private pf_enc_site_mm(enc : MetalComputeEncoder?; t : Model; fmt : KqFmt; woff, rows : int64; bx, by, bk, bn : MetalBuffer?;
                           mp, kdim, npos : int64; bxh : MetalBuffer?) {
    if (fmt != KqFmt.q8) {
        pf_enc_kq_site_mm(enc, t, fmt, woff, rows, bx, by, bk, bn, mp, 0ul, bxh, kdim, npos)
        return
    }
    let bw = blob_of(g_dev, t, woff)
    enc_gemm_mm(enc, bw.buf, bw.boff, bx, by, bk, bn, mp, rows, 0ul, kdim, npos, bxh)
}

//! one deltanet beta/alpha plane over the window's rows: q8 through the narrow-projection GEMV, a K-quant plane through its
//! format's expert GEMV as a one-expert stack, every row a stream - no mul_mm stamp tiles a plane of dt_rank rows
def private pf_enc_dn_ba_plane(enc : MetalComputeEncoder?; t : Model; fmt : KqFmt; woff : int64; bx, by, bk, bn : MetalBuffer?;
                               dim, nvh, npos : int64) {
    if (fmt == KqFmt.q8) {
        let bw = blob_of(g_dev, t, woff)
        pf_enc_dn_ba(enc, bw.buf, bw.boff, bw.buf, bx, by, 0ul, bk, bn, nvh, npos)
        return
    }
    var ka = MoeGemvArgs(ndim = uint(dim), ddim = uint(nvh), eplane = 0u, xs = 0u, ss = 0u, bxs = uint(dim), bys = uint(nvh), hasb = 0u)
    enc_moe_gemv(enc, t, fmt, woff, nvh, 1l, npos, bx, by, g_zero, ka)
}

//! one dense site past the tail peel, over its plane pair: the tall twin first where `tall_first` (the K-quant deep class reads
//! a DRAM-resident plane once a 128-row tile where a dev-W panel would re-stream it), the dev-W panel, the format's twin under
//! its crown, else the simdgroup base tile; `twin_fits` false (a K off the twins' 64-wide chunks) sends the site to its base tile
[arch(at="../ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-kq-tensor-scaffold")]
def private pf_enc_kq_dense_site(enc : MetalComputeEncoder?; t : Model; fmt : KqFmt; woff, rows : int64;
                                 bq : tuple<buf : MetalBuffer?; qoff : uint64>; bs : tuple<buf : MetalBuffer?; soff : uint64; doff : uint64>;
                                 bx, by, bk, bn : MetalBuffer?; mp : int64; yoff : uint64; bxh : MetalBuffer?; kdim : int64;
                                 twin_fits, tall_first : bool; live : int64 = 0l) {
    let ss <- pf_kq_dense_stamps(fmt)
    let fits = twin_fits && rows % 64l == 0l   //! the twins' tiles span 64 output rows; the base tile and the tail take any count
    let d0 = ss.d_soff ? bs.soff : bs.doff
    let tall = fits && ss.crown && bxh != null && ss.pso_th128 != null && ss.pso_th != null && tallkq_pick(mp, rows, kdim, pf_tall_on())
    if (tall && tall_first) {
        pf_enc_kq_tall(enc, ss, bq, bs, d0, bxh, by, bk, bn, mp, rows, kdim, yoff)
        return
    }
    // the split-K form under its knee, then the dev-W arm, which gates itself; a tall panel is over the knee and declines here
    if ((fits && ss.crown && !tall && pf_enc_kq_split(enc, ss, bq, bs, d0, bxh != null ? bxh : bx, bxh != null, by, bk, bn, mp, rows, kdim, yoff, live))
            || (fits && g_pf_tensor_ok && bxh != null && g_pf_pso_hmm_th != null && kdim > 0l
            && pf_devw_panel_kq(enc, fmt, woff, bq, bs, bxh, by, bk, bn, mp, rows, kdim, yoff, ss.crown && ss.pso_th != null))) {
        return
    }
    if (!fits || !ss.crown || ss.pso_t == null) {
        if (pf_enc_kq_base_split(enc, t, fmt, woff, bq, bs, bx, by, bk, bn, mp, rows, kdim, yoff)) {
            return
        }
        if (kq_dense_b32(fmt)) {
            enc_b32_gemm_mm(enc, t, fmt, woff, bx, by, yoff, bk, bn, mp, rows)
        } else {
            enc_kq_gemm_mm_b(enc, fmt, bq.buf, bq.qoff, bs.buf, bs.soff, bs.doff, bx, by, yoff, bk, bn, mp, rows)
        }
    } elif (tall) {
        pf_enc_kq_tall(enc, ss, bq, bs, d0, bxh, by, bk, bn, mp, rows, kdim, yoff)
    } elif (bxh != null && ss.pso_th != null) {
        invoke(pf_dbuf(ss.pso_thdb) ? ss.enc_thdb : ss.enc_th, enc, bs.buf, d0, bs.buf, bs.soff, bq.buf, bq.qoff, bxh, 0ul, by, yoff, bk, bn, mp, rows)
    } else {
        invoke(ss.enc_t, enc, bs.buf, d0, bs.buf, bs.soff, bq.buf, bq.qoff, bx, 0ul, by, yoff, bk, bn, mp, rows)
    }
}

//! A dense K-quant, grid or per-32 site's short-window form: its format's split-K stamp over the 32-row grid; false past the knee
//! or where the format's stamp is absent - the caller's tile serves
[arch(at="../ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-split-k")]
def private pf_enc_kq_split(enc : MetalComputeEncoder?; ss : KqDenseStamps; bq : tuple<buf : MetalBuffer?; qoff : uint64>;
                            bs : tuple<buf : MetalBuffer?; soff : uint64; doff : uint64>; d0 : uint64; bxa : MetalBuffer?; half_form : bool;
                            by, bk, bn : MetalBuffer?; mp, rows, kdim : int64; yoff : uint64; live : int64) : bool {
    let pso = half_form ? ss.pso_thsk : ss.pso_tsk
    let tiles = mp / 32l * (rows / 64l)
    let knee = pf_knee(true)
    if (pso == null || !pf_split_k_on(tiles, kdim, knee)) {
        return false
    }
    let short = pf_split_k_short(live, mp, half_form ? ss.pso_thsk16 : ss.pso_tsk16)
    let mpe = short ? 16l : mp
    let sk = short ? (half_form ? ss.enc_thsk16 : ss.enc_tsk16) : (half_form ? ss.enc_thsk : ss.enc_tsk)
    return pf_split_k_run(enc, tiles, kdim, knee, by, yoff, mpe, rows, 0l, true) $(out, ooff, nsk) {
        invoke(sk, enc, bs.buf, d0, bs.buf, bs.soff, bq.buf, bq.qoff, bxa, 0ul, out, ooff, bk, bn, mpe, rows, nsk)
        return true
    }
}

//! A dense K-quant, grid or per-32 site's simdgroup base tile split along K under the base knee - the form a box whose dense
//! race crowned no tensor twin serves; false past the knee, off the 64-row grid or where the stamp is not compiled
[arch(at="../ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-split-k")]
def private pf_enc_kq_base_split(enc : MetalComputeEncoder?; t : Model; fmt : KqFmt; woff : int64; bq : tuple<buf : MetalBuffer?; qoff : uint64>;
                                 bs : tuple<buf : MetalBuffer?; soff : uint64; doff : uint64>; bx, by, bk, bn : MetalBuffer?;
                                 mp, rows, kdim : int64; yoff : uint64) : bool {
    let tiles = mp / 32l * (rows / 64l)
    let knee = pf_knee(true, true)
    if (rows % 64l != 0l || mp % 32l != 0l || !pf_split_k_on(tiles, kdim, knee)) {
        return false
    }
    return pf_split_k_run(enc, tiles, kdim, knee, by, yoff, mp, rows, 0l, false) $(out, ooff, nsk) {
        return (kq_dense_b32(fmt) ? enc_b32_gemm_mm_sk(enc, t, fmt, woff, bx, out, ooff, bk, bn, mp, rows, nsk)
            : enc_kq_gemm_mm_sk(enc, fmt, bq.buf, bq.qoff, bs.buf, bs.soff, bs.doff, bx, out, ooff, bk, bn, mp, rows, nsk))
    }
}

//! the tall pair: the 128-row stamp over the whole tiles, the 32-row half-X stamp over the remainder at its x and y offsets
def private pf_enc_kq_tall(enc : MetalComputeEncoder?; ss : KqDenseStamps; bq : tuple<buf : MetalBuffer?; qoff : uint64>;
                           bs : tuple<buf : MetalBuffer?; soff : uint64; doff : uint64>; d0 : uint64; bxh, by, bk, bn : MetalBuffer?;
                           mp, rows, kdim : int64; yoff : uint64) {
    let r0 = mp / 128l * 128l
    invoke(pf_dbuf(ss.pso_thdb128) ? ss.enc_thdb128 : ss.enc_th128, enc, bs.buf, d0, bs.buf, bs.soff, bq.buf, bq.qoff, bxh, 0ul, by, yoff, bk, bn, r0, rows)
    if (mp - r0 > 0l) {
        invoke(pf_dbuf(ss.pso_thdb) ? ss.enc_thdb : ss.enc_th,
            enc, bs.buf, d0, bs.buf, bs.soff, bq.buf, bq.qoff, bxh, uint64(r0 * kdim * 2l), by, yoff + uint64(r0 * rows * 4l), bk, bn, mp - r0, rows)
    }
}

//! whether a dense K-quant, grid or per-32 site's split-K form would take an mp x rows grid: its crown, its stamps and the knee
def private pf_kq_split_takes(fmt : KqFmt; mp, rows, kdim : int64) : bool {
    let ss <- pf_kq_dense_stamps(fmt)
    return ss.crown && ss.pso_thsk != null && ss.pso_tsk != null && rows % 64l == 0l && pf_split_k_on(mp / 32l * (rows / 64l), kdim, pf_knee(true))
}

def private pf_enc_kq_site_mm(enc : MetalComputeEncoder?; t : Model; fmt : KqFmt; woff, rows : int64;
                              bx, by, bk, bn : MetalBuffer?; mp : int64; yoff : uint64 = 0ul; bxh : MetalBuffer? = null; kdim : int64 = 0l;
                              npos : int64 = 0l) {
    //! a window of eight rows or fewer rides the decode's row-batched GEMV forms, which stream the plane once for every row,
    //! where the 32-row tile walks K serially under barriers one threadgroup per core (ARCHITECTURE_GPU_PREFILL_GEMM.md#gemv-tail-peel)
    if (g_pf_env_mm_tail && npos >= 1l && npos <= (pf_kq_split_takes(fmt, mp, rows, kdim) ? MM_TAIL_SPLIT_WINDOW : MM_TAIL_MAX)
            && kdim > 0l && kdim % kq_unit(fmt) == 0l && yoff % 4ul == 0ul) {
        enc_kq_site_b(enc, t, fmt, woff, rows, kdim, bx, kdim, by, rows, int64(yoff / 4ul), bk, bn, bn, npos, mp, bxh)
        g_pf_tail_sites ++
        g_pf_kq_tail_sites ++
        g_pf_tail_rows += npos
        return
    }
    let rem = npos % 32l
    let rem_max = pf_kq_split_takes(fmt, mp, rows, kdim) ? MM_TAIL_SPLIT_MAX : MM_TAIL_KQ_PAST_KNEE
    if (g_pf_env_mm_tail && npos > MM_TAIL_MAX && rem >= 1l && rem <= rem_max && kdim > 0l && kdim % kq_unit(fmt) == 0l && yoff % 4ul == 0ul) {
        let nfull = npos - rem
        pf_enc_kq_site_mm(enc, t, fmt, woff, rows, bx, by, bk, bn, nfull, yoff, bxh, kdim, nfull)
        enc_kq_site_b(enc, t, fmt, woff, rows, kdim, bx, kdim, by, rows, int64(yoff / 4ul) + nfull * rows, bk, bn, bn, rem, mp, bxh, nfull)
        g_pf_tail_sites ++
        g_pf_kq_tail_sites ++
        g_pf_tail_rows += rem
        return
    }
    if (kq_dense_b32(fmt)) {
        let bp = b32_planes_of(g_dev, t, fmt, woff)
        pf_enc_kq_dense_site(enc, t, fmt, woff, rows, (buf = bp.qbuf, qoff = bp.qoff), (buf = bp.sbuf, soff = bp.soff, doff = bp.soff),
            bx, by, bk, bn, mp, yoff, bxh, kdim, kdim > 0l && b32_twin_fits(uint(kdim)), false, npos)
        return
    }
    let bq = kq_quants_of(g_dev, t, fmt, woff)
    let bs = kq_scales_of(g_dev, t, fmt, woff)
    pf_enc_kq_dense_site(enc, t, fmt, woff, rows, bq, bs, bx, by, bk, bn, mp, yoff, bxh, kdim, true, !pf_kq_split_fmt(fmt), npos)
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
def public metal_prefill_moe_rows() : array<uint> => <- clone_to_move(g_pf_moe_rows)

//! Hold a prefill form off (or let it serve again) for a one-process race, the form it replaced serving meanwhile: "attn_dev"
//! (the device attention pair), "router_mm" (the router's f32 tile GEMM), "dn_half" (the scan over half rows and prepared gates),
//! "split_k16" (the 16-row split stamps; the 32-row ones serve). Returns the previous hold.
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

//! The window's router logits as one f32 tile GEMM over the layer's router rows in their own f32 bytes and the routed rows
//! (`MetalF32ExactMm`: float tiles, no half rounding of either operand). false = no tile for the expert count, the width or the
//! panel - the slab GEMV serves.
[arch(at="../ARCHITECTURE_GPU_PREFILL_MOE.md#prefill-moe-buckets")]
def private pf_enc_router_mm(enc : MetalComputeEncoder?; bw_rt, bx, blg, bdim, bne : MetalBuffer?; ne, dim, mp : int64) : bool {
    if (pf_held("router_mm") || g_pso_f32_exact_mm == null || ne % 64l != 0l || dim % 32l != 0l || mp <= 0l || mp % 32l != 0l) {
        return false
    }
    enc_f32_exact_mm(enc, bw_rt, 0ul, bx, blg, 0ul, bdim, bne, mp, ne)
    return true
}

//! layer l's router rows in their f32 bytes, the shared expert's gate row past them where the model has one (the decode router's
//! one region: the rows tile reads the first ne rows, the gate GEMV the last)
def private pf_moe_router_rows(t : Model; l : int64) : MetalBuffer? {
    let c = t.config
    let rows_bytes = uint64(c.n_expert * c.dim * 4l)
    unsafe {
        return (c.n_ff_shexp > 0l ? upload_region_cat(addr<void?>(t.fblob[t.router_off + l * c.n_expert * c.dim]), rows_bytes,
                addr<void?>(t.fblob[t.shexp_gate_off + l * c.dim]), uint64(c.dim * 4l))
            : upload_region(addr<void?>(t.fblob[t.router_off + l * c.n_expert * c.dim]), rows_bytes))
    }
}

[arch(at="../ARCHITECTURE_GPU_PREFILL_MOE.md#prefill-moe-buckets")]
def private pf_enc_moe_route(enc : MetalComputeEncoder?; t : Model; l : int64;
                             bx, blg, bsel, bcnt, bbase, bbkt, binv, bdim, bne, bnk, brb, btot : MetalBuffer?; npos : int64;
                             mp : int64 = 0l; bucket : bool = true; bds : MetalBuffer? = null) {
    let c = t.config
    var bw_rt = pf_moe_router_rows(t, l)
    if (c.moe_router_bias || !pf_enc_router_mm(enc, bw_rt, bx, blg, bdim, bne, c.n_expert, c.dim, mp)) {
        var rka = moe_router_args(c.n_expert, c.dim, npos, 0l, c.moe_router_bias)
        enc_moe_router_b(enc, bw_rt, bx, blg, brb, rka, c.n_expert, npos)
    }
    return if (g_pf_skip == "moe_only_router")   //! attribution: router GEMV alone, no select/CSR
    enc_moe_select_rows(enc, c, blg, bsel, npos, 0l, bds)
    return if (g_pf_skip == "moe_only_sel" || !bucket)   //! attribution: router + select, no CSR; the gathered route reads the selection alone
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

//! the per-32 twins walk 64-deep K chunks (2 blocks/step) - an odd 32-block count
//! stays legacy; the kernel-unit gate keys its twin legs on this same predicate
def b32_twin_fits(kdim : uint) : bool => (kdim & 63u) == 0u

//! a superblock or per-32 format's routed verdict
def private pf_moe_kq_crowned(fmt : KqFmt) : bool => g_pf_moe_kq_crown[int(fmt)]

//! whether a routed site of this format takes its contiguous tensor twin at this window - the one answer the gather panel, the
//! activation's half store and the dispatch arms all read: the twin compiled, and a per-32 format under its own verdict or the
//! knee and its 64-wide K fit, a superblock format under its own verdict or the knee, q8 whenever it compiled
[arch(at="../ARCHITECTURE_GPU_PREFILL_MOE.md#prefill-moe-kq-twin-pick")]
def private pf_moe_twin_live(fmt : KqFmt; kdim, npos, nk, ne : int64) : bool {
    return false if (pf_moe_th_pso(fmt) == null)
    return b32_twin_fits(uint(kdim)) && metal_moe_kq_twin_pick(pf_moe_kq_crowned(fmt), npos, nk, ne) if (kq_dense_b32(fmt))
    return metal_moe_kq_twin_pick(pf_moe_kq_crowned(fmt), npos, nk, ne) if (kq_sb(fmt))
    return true
}

//! a routed format's half-X tensor twin, where it compiled
def private pf_moe_th_pso(fmt : KqFmt) : MetalComputePipeline? {
    if (fmt == KqFmt.q8) return g_pf_pso_moe_mm_q8_th
    return pf_moe_kq_row(fmt).pso_th if (kq_sb(fmt) || kq_dense_b32(fmt))
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

typedef MoeBaseEnc = function<(enc : MetalComputeEncoder?; bksh : MetalBuffer?; s0off : uint64; bks4 : MetalBuffer?; soff : uint64;
                               bkqu : MetalBuffer?; qoff : uint64; bxf : MetalBuffer?; by : MetalBuffer?; var ka : MoeMmArgs;
                               bcnt : MetalBuffer?; bbasep : MetalBuffer?; bbkt : MetalBuffer?; rows : int64; npos : int64; ne : int64) : void>

//! one superblock expert format's routed forms in one row - the simdgroup base, the four tensor twins, each pso with the
//! builder that dispatches it; `d_soff` binds slot 0 at the compact scale plane's soff
struct private MoeKqRow {
    pso_base, pso_t, pso_th, pso_th128, pso_thr : MetalComputePipeline?
    enc_base : MoeBaseEnc
    enc_t, enc_th, enc_th128, enc_thr : MoeSplitEnc
    d_soff : bool
}

//! the routed forms of every superblock and per-32 expert format: the one table the twin picks, the panel gate and the dispatch
//! walk read; a per-32 row carries no `enc_base` - its base binds the verbatim plane pair without the strip view
def private pf_moe_kq_row(fmt : KqFmt) : MoeKqRow {
    if (fmt == KqFmt.k4) return MoeKqRow(pso_base = g_pf_pso_moe_mm_k4, enc_base = @@pf_enc_moe_mm_k4_c, d_soff = true,
        pso_t = g_pf_pso_moe_mm_k4_t, pso_th = g_pf_pso_moe_mm_k4_th, pso_th128 = g_pf_pso_moe_mm_k4_th128, pso_thr = g_pf_pso_moe_mm_k4_thr,
        enc_t = @@pf_enc_moe_mm_k4_t_c, enc_th = @@pf_enc_moe_mm_k4_th_c, enc_th128 = @@pf_enc_moe_mm_k4_th128_c, enc_thr = @@pf_enc_moe_mm_k4_thr_c)
    if (fmt == KqFmt.k5) return MoeKqRow(pso_base = g_pf_pso_moe_mm_k5, enc_base = @@pf_enc_moe_mm_k5_c, d_soff = true,
        pso_t = g_pf_pso_moe_mm_k5_t, pso_th = g_pf_pso_moe_mm_k5_th, pso_th128 = g_pf_pso_moe_mm_k5_th128, pso_thr = g_pf_pso_moe_mm_k5_thr,
        enc_t = @@pf_enc_moe_mm_k5_t_c, enc_th = @@pf_enc_moe_mm_k5_th_c, enc_th128 = @@pf_enc_moe_mm_k5_th128_c, enc_thr = @@pf_enc_moe_mm_k5_thr_c)
    if (fmt == KqFmt.k6) return MoeKqRow(pso_base = g_pf_pso_moe_mm_k6, enc_base = @@pf_enc_moe_mm_k6_c,
        pso_t = g_pf_pso_moe_mm_k6_t, pso_th = g_pf_pso_moe_mm_k6_th, pso_th128 = g_pf_pso_moe_mm_k6_th128, pso_thr = g_pf_pso_moe_mm_k6_thr,
        enc_t = @@pf_enc_moe_mm_k6_t_c, enc_th = @@pf_enc_moe_mm_k6_th_c, enc_th128 = @@pf_enc_moe_mm_k6_th128_c, enc_thr = @@pf_enc_moe_mm_k6_thr_c)
    if (fmt == KqFmt.k3) return MoeKqRow(pso_base = g_pf_pso_moe_mm_k3, enc_base = @@pf_enc_moe_mm_k3_c,
        pso_t = g_pf_pso_moe_mm_k3_t, pso_th = g_pf_pso_moe_mm_k3_th, pso_th128 = g_pf_pso_moe_mm_k3_th128, pso_thr = g_pf_pso_moe_mm_k3_thr,
        enc_t = @@pf_enc_moe_mm_k3_t_c, enc_th = @@pf_enc_moe_mm_k3_th_c, enc_th128 = @@pf_enc_moe_mm_k3_th128_c, enc_thr = @@pf_enc_moe_mm_k3_thr_c)
    if (fmt == KqFmt.k2) return MoeKqRow(pso_base = g_pf_pso_moe_mm_k2, enc_base = @@pf_enc_moe_mm_k2_c,
        pso_t = g_pf_pso_moe_mm_k2_t, pso_th = g_pf_pso_moe_mm_k2_th, pso_th128 = g_pf_pso_moe_mm_k2_th128, pso_thr = g_pf_pso_moe_mm_k2_thr,
        enc_t = @@pf_enc_moe_mm_k2_t_c, enc_th = @@pf_enc_moe_mm_k2_th_c, enc_th128 = @@pf_enc_moe_mm_k2_th128_c, enc_thr = @@pf_enc_moe_mm_k2_thr_c)
    if (fmt == KqFmt.iq4xs) return MoeKqRow(pso_base = g_pf_pso_moe_mm_iq4xs, enc_base = @@pf_enc_moe_mm_iq4xs_c,
        pso_t = g_pf_pso_moe_mm_iq4xs_t, pso_th = g_pf_pso_moe_mm_iq4xs_th, pso_th128 = g_pf_pso_moe_mm_iq4xs_th128, pso_thr = g_pf_pso_moe_mm_iq4xs_thr,
        enc_t = @@pf_enc_moe_mm_iq4xs_t_c, enc_th = @@pf_enc_moe_mm_iq4xs_th_c, enc_th128 = @@pf_enc_moe_mm_iq4xs_th128_c, enc_thr = @@pf_enc_moe_mm_iq4xs_thr_c)
    if (fmt == KqFmt.iq3s4) return MoeKqRow(pso_base = g_pf_pso_moe_mm_iq3s4, enc_base = @@pf_enc_moe_mm_iq3s4_c,
        pso_t = g_pf_pso_moe_mm_iq3s4_t, pso_th = g_pf_pso_moe_mm_iq3s4_th, pso_th128 = g_pf_pso_moe_mm_iq3s4_th128, pso_thr = g_pf_pso_moe_mm_iq3s4_thr,
        enc_t = @@pf_enc_moe_mm_iq3s4_t_c, enc_th = @@pf_enc_moe_mm_iq3s4_th_c, enc_th128 = @@pf_enc_moe_mm_iq3s4_th128_c, enc_thr = @@pf_enc_moe_mm_iq3s4_thr_c)
    if (fmt == KqFmt.iq4nl) return MoeKqRow(pso_base = g_pf_pso_moe_mm_iq4nl, enc_base = @@pf_enc_moe_mm_iq4nl_c, d_soff = true,
        pso_t = g_pf_pso_moe_mm_iq4nl_t, pso_th = g_pf_pso_moe_mm_iq4nl_th, pso_th128 = g_pf_pso_moe_mm_iq4nl_th128, pso_thr = g_pf_pso_moe_mm_iq4nl_thr,
        enc_t = @@pf_enc_moe_mm_iq4nl_t_c, enc_th = @@pf_enc_moe_mm_iq4nl_th_c, enc_th128 = @@pf_enc_moe_mm_iq4nl_th128_c, enc_thr = @@pf_enc_moe_mm_iq4nl_thr_c)
    if (fmt == KqFmt.q40) return MoeKqRow(pso_base = g_pf_pso_moe_mm_q40, enc_base = @@pf_enc_moe_mm_q40_c, d_soff = true,
        pso_t = g_pf_pso_moe_mm_q40_t, pso_th = g_pf_pso_moe_mm_q40_th, pso_th128 = g_pf_pso_moe_mm_q40_th128, pso_thr = g_pf_pso_moe_mm_q40_thr,
        enc_t = @@pf_enc_moe_mm_q40_t_c, enc_th = @@pf_enc_moe_mm_q40_th_c, enc_th128 = @@pf_enc_moe_mm_q40_th128_c, enc_thr = @@pf_enc_moe_mm_q40_thr_c)
    if (fmt == KqFmt.nv4) return MoeKqRow(pso_base = g_pf_pso_moe_mm_nv4, enc_base = @@pf_enc_moe_mm_nv4_c, d_soff = true,
        pso_t = g_pf_pso_moe_mm_nv4_t, pso_th = g_pf_pso_moe_mm_nv4_th, pso_th128 = g_pf_pso_moe_mm_nv4_th128, pso_thr = g_pf_pso_moe_mm_nv4_thr,
        enc_t = @@pf_enc_moe_mm_nv4_t_c, enc_th = @@pf_enc_moe_mm_nv4_th_c, enc_th128 = @@pf_enc_moe_mm_nv4_th128_c, enc_thr = @@pf_enc_moe_mm_nv4_thr_c)
    if (fmt == KqFmt.iq3s) return MoeKqRow(pso_base = g_pf_pso_moe_mm_iq3s, enc_base = @@pf_enc_moe_mm_iq3s_c,
        pso_t = g_pf_pso_moe_mm_iq3s_t, pso_th = g_pf_pso_moe_mm_iq3s_th, pso_th128 = g_pf_pso_moe_mm_iq3s_th128, pso_thr = g_pf_pso_moe_mm_iq3s_thr,
        enc_t = @@pf_enc_moe_mm_iq3s_t_c, enc_th = @@pf_enc_moe_mm_iq3s_th_c, enc_th128 = @@pf_enc_moe_mm_iq3s_th128_c, enc_thr = @@pf_enc_moe_mm_iq3s_thr_c)
    if (fmt == KqFmt.iq3xxs) return MoeKqRow(pso_base = g_pf_pso_moe_mm_iq3xxs, enc_base = @@pf_enc_moe_mm_iq3xxs_c,
        pso_t = g_pf_pso_moe_mm_iq3xxs_t, pso_th = g_pf_pso_moe_mm_iq3xxs_th, pso_th128 = g_pf_pso_moe_mm_iq3xxs_th128, pso_thr = g_pf_pso_moe_mm_iq3xxs_thr,
        enc_t = @@pf_enc_moe_mm_iq3xxs_t_c, enc_th = @@pf_enc_moe_mm_iq3xxs_th_c, enc_th128 = @@pf_enc_moe_mm_iq3xxs_th128_c, enc_thr = @@pf_enc_moe_mm_iq3xxs_thr_c)
    if (fmt == KqFmt.iq2s) return MoeKqRow(pso_base = g_pf_pso_moe_mm_iq2s, enc_base = @@pf_enc_moe_mm_iq2s_c,
        pso_t = g_pf_pso_moe_mm_iq2s_t, pso_th = g_pf_pso_moe_mm_iq2s_th, pso_th128 = g_pf_pso_moe_mm_iq2s_th128, pso_thr = g_pf_pso_moe_mm_iq2s_thr,
        enc_t = @@pf_enc_moe_mm_iq2s_t_c, enc_th = @@pf_enc_moe_mm_iq2s_th_c, enc_th128 = @@pf_enc_moe_mm_iq2s_th128_c, enc_thr = @@pf_enc_moe_mm_iq2s_thr_c)
    if (fmt == KqFmt.iq2xs) return MoeKqRow(pso_base = g_pf_pso_moe_mm_iq2xs, enc_base = @@pf_enc_moe_mm_iq2xs_c,
        pso_t = g_pf_pso_moe_mm_iq2xs_t, pso_th = g_pf_pso_moe_mm_iq2xs_th, pso_th128 = g_pf_pso_moe_mm_iq2xs_th128, pso_thr = g_pf_pso_moe_mm_iq2xs_thr,
        enc_t = @@pf_enc_moe_mm_iq2xs_t_c, enc_th = @@pf_enc_moe_mm_iq2xs_th_c, enc_th128 = @@pf_enc_moe_mm_iq2xs_th128_c, enc_thr = @@pf_enc_moe_mm_iq2xs_thr_c)
    if (fmt == KqFmt.iq2xxs) return MoeKqRow(pso_base = g_pf_pso_moe_mm_iq2xxs, enc_base = @@pf_enc_moe_mm_iq2xxs_c,
        pso_t = g_pf_pso_moe_mm_iq2xxs_t, pso_th = g_pf_pso_moe_mm_iq2xxs_th, pso_th128 = g_pf_pso_moe_mm_iq2xxs_th128, pso_thr = g_pf_pso_moe_mm_iq2xxs_thr,
        enc_t = @@pf_enc_moe_mm_iq2xxs_t_c, enc_th = @@pf_enc_moe_mm_iq2xxs_th_c, enc_th128 = @@pf_enc_moe_mm_iq2xxs_th128_c, enc_thr = @@pf_enc_moe_mm_iq2xxs_thr_c)
    if (fmt == KqFmt.q51) return MoeKqRow(pso_base = g_pf_pso_moe_mm_q51, d_soff = true,
        pso_t = g_pf_pso_moe_mm_q51_t, pso_th = g_pf_pso_moe_mm_q51_th, pso_th128 = g_pf_pso_moe_mm_q51_th128, pso_thr = g_pf_pso_moe_mm_q51_thr,
        enc_t = @@pf_enc_moe_mm_q51_t_c, enc_th = @@pf_enc_moe_mm_q51_th_c, enc_th128 = @@pf_enc_moe_mm_q51_th128_c, enc_thr = @@pf_enc_moe_mm_q51_thr_c)
    if (fmt == KqFmt.iq4nl32) return MoeKqRow(pso_base = g_pf_pso_moe_mm_iq4nl32, d_soff = true,
        pso_t = g_pf_pso_moe_mm_iq4nl32_t, pso_th = g_pf_pso_moe_mm_iq4nl32_th, pso_th128 = g_pf_pso_moe_mm_iq4nl32_th128, pso_thr = g_pf_pso_moe_mm_iq4nl32_thr,
        enc_t = @@pf_enc_moe_mm_iq4nl32_t_c, enc_th = @@pf_enc_moe_mm_iq4nl32_th_c, enc_th128 = @@pf_enc_moe_mm_iq4nl32_th128_c, enc_thr = @@pf_enc_moe_mm_iq4nl32_thr_c)
    if (fmt == KqFmt.q4032) return MoeKqRow(pso_base = g_pf_pso_moe_mm_q4032, d_soff = true,
        pso_t = g_pf_pso_moe_mm_q4032_t, pso_th = g_pf_pso_moe_mm_q4032_th, pso_th128 = g_pf_pso_moe_mm_q4032_th128, pso_thr = g_pf_pso_moe_mm_q4032_thr,
        enc_t = @@pf_enc_moe_mm_q4032_t_c, enc_th = @@pf_enc_moe_mm_q4032_th_c, enc_th128 = @@pf_enc_moe_mm_q4032_th128_c, enc_thr = @@pf_enc_moe_mm_q4032_thr_c)
    if (fmt == KqFmt.q5032) return MoeKqRow(pso_base = g_pf_pso_moe_mm_q5032, d_soff = true,
        pso_t = g_pf_pso_moe_mm_q5032_t, pso_th = g_pf_pso_moe_mm_q5032_th, pso_th128 = g_pf_pso_moe_mm_q5032_th128, pso_thr = g_pf_pso_moe_mm_q5032_thr,
        enc_t = @@pf_enc_moe_mm_q5032_t_c, enc_th = @@pf_enc_moe_mm_q5032_th_c, enc_th128 = @@pf_enc_moe_mm_q5032_th128_c, enc_thr = @@pf_enc_moe_mm_q5032_thr_c)
    panic("metal moe prefill: expert plane format {fmt} has no routed row - moe_fmt_metal_served admitted it")
    return MoeKqRow()
}

//! some routed twin a contiguous gather panel can feed compiled on this box
def private pf_moe_th_any() : bool {
    return true if (g_pf_pso_moe_mm_mx4_th != null || g_pf_pso_moe_mm_q8_th != null || g_pf_pso_moe_mm_q51_th != null || g_pf_pso_moe_mm_iq4nl32_th != null
        || g_pf_pso_moe_mm_q4032_th != null || g_pf_pso_moe_mm_q5032_th != null)
    for (i in range(KQ_FMT_COUNT)) {
        let f = kq_fmt_of_slot(i)
        return true if (kq_sb(f) && moe_fmt_metal_served(f) && pf_moe_kq_row(f).pso_th != null)
    }
    return false
}

var private g_pf_moe_tall_avg = 32l   //!< mean rows an expert at which the routed GEMMs take the tall pair
var private g_pf_moe_kq_twin_avg = 16l   //!< mean rows an expert at or under which an uncrowned box takes a K-quant MoE twin: the adaptive op's region
var private g_pf_mxg_min = 1l   //!< the window at which the routed gate and up read the gathered half panel: every window - the in-kernel gather is the slower form from 17 tokens up

def public set_metal_moe_gather_min(npos : int64) : int64 {
    let was = g_pf_mxg_min
    g_pf_mxg_min = max(npos, 1l)
    return was
}

//! the bucket rail's padded row total: every populated expert pads to a whole 32-row tile, and at most min(ne, mtot)
//! experts hold rows, so the total never passes mtot plus 31 pad rows an expert for that many
def public pf_moe_padded_rows(mtot, ne : int64) : int64 => ((mtot + min(ne, mtot) * 31l + 31l) / 32l) * 32l

var private g_pf_moe_gemv_windows = 0l

//! windows whose routed block rode the gathered GEMV route since load - the route's engage witness
def public metal_prefill_moe_gemv_stats() : int64 => g_pf_moe_gemv_windows

//! the route's knee in tokens: the sidecar's where it names one, else 16 - the widest window the route won on every MoE
//! carrier swept on both the M1 Max and the M5 Max; the crossing itself is per model and per box (PERF_LEDGER.md)
[arch(at="../ARCHITECTURE_GPU_PREFILL_MOE.md#prefill-moe-gemv-route")]
def public pf_moe_gemv_knee : int64 {
    let knob = metal_moe_gemv_max()
    return knob >= 0l ? knob : 16l
}

//! whether this window's routed block rides the decode's gathered expert GEMVs: the window is within the knee and every
//! expert plane of every layer has a gathered kernel (`moe_fmt_metal_served`, or the mx4 planes)
[arch(at="../ARCHITECTURE_GPU_PREFILL_MOE.md#prefill-moe-gemv-route")]
//! the window rides the gathered route under the knee; every expert plane's format is served here, since the decline
//! gate admitted the model through `moe_metal_ok`, so the only other refusal is mx4 before its GEMV compiled
def private pf_moe_gemv_route(t : Model; npos : int64) : bool =>
    npos <= pf_moe_gemv_knee() && (!t.experts_mx4 || g_pso_moe_gemv_mx4 != null)

//! the gathered GEMV form of a window's routed block: gate and up over every (token, slot) pair off the selection the route
//! wrote, the activation over the live rows alone, down the same way, and the decode's combine - no bucket panel, no
//! padded rows, the kernels the decode step dispatches
[arch(at="../ARCHITECTURE_GPU_PREFILL_MOE.md#prefill-moe-gemv-route")]
def private pf_enc_moe_gemv_block(enc : MetalComputeEncoder?; t : Model; l : int64; bx, bg, bu, bdn, bsel, bout : MetalBuffer?;
                                  npos : int64; btot_act : MetalBuffer?; bsh : MetalBuffer? = null; bgate : MetalBuffer? = null) {
    let c = t.config
    let dim = c.dim
    let nfe = c.n_ff_exp
    let nk = c.n_expert_used
    let hasb = t.experts_mx4 && c.moe_exps_bias && g_pf_skip != "moe_bias" ? 1u : 0u
    //! the up/gate sites read a token's x row for each of its slots (xs = 0); the down site reads the slot's hidden row
    enc_moe_gate_up(enc, t, l, npos, bx, bg, bu, bsel, 0ul,
        MoeGemvArgs(ndim = uint(dim), ddim = uint(nfe), xs = 0u, ss = uint(2l * nk), bxs = uint(dim), bys = uint(nk * nfe), hasb = hasb))
    if (!pf_skip_moe_act()) {
        enc_moe_act(enc, c, bg, bu, 0ul, btot_act, npos * nk * nfe)
    }
    enc_moe_down(enc, t, l, npos, bg, bdn, bsel,
        MoeGemvArgs(ndim = uint(nfe), ddim = uint(dim), xs = uint(nfe), ss = uint(2l * nk), bxs = uint(nk * nfe), bys = uint(nk * dim), hasb = hasb))
    if (pf_skip_moe_reduce()) {
        return
    }
    if (bsh != null) {   //! the shared expert's rows `bsh` under their gate logits `bgate` added in the same pass
        enc_moe_combine_sh(enc, bout, bsel, bdn, bsh, bgate, moe_comb_args(dim, nk), dim, npos)
    } else {
        enc_moe_combine(enc, bout, bsel, bdn, moe_comb_args(dim, nk), dim, npos)
    }
}

//! The routed tall pair's engage point, in mean rows an expert; answers the value it replaced.
def public set_metal_moe_tall_avg(rows : int64) : int64 {
    let was = g_pf_moe_tall_avg
    g_pf_moe_tall_avg = max(rows, 1l)
    return was
}

def private pf_moe_tall_pick(npos, nk, ne : int64; pso_th128, pso_thr : MetalComputePipeline?) : bool =>
    pf_tall_on() && npos * nk >= g_pf_moe_tall_avg * ne && pso_th128 != null && pso_thr != null

//! The K-quant MoE twins' knee on an uncrowned box, in mean rows an expert; answers the value it replaced.
def public set_metal_moe_kq_twin_avg(rows : int64) : int64 {
    let was = g_pf_moe_kq_twin_avg
    g_pf_moe_kq_twin_avg = max(rows, 0l)
    return was
}

[arch(at="../ARCHITECTURE_GPU_PREFILL_MOE.md#prefill-moe-kq-twin-pick")]
def public metal_moe_kq_twin_pick(crowned : bool; npos, nk, ne : int64) : bool =>
    crowned || npos * nk <= g_pf_moe_kq_twin_avg * ne

//! one routed site: q8's blob forms, the per-32 plane pair's, or a superblock format's row of `pf_moe_kq_row` - the tensor
//! twins over contiguous rows where the pick admits them (the tall pair past `set_metal_moe_tall_avg`), else the simdgroup
//! base form over the bucket rows, the in-kernel gather included
[arch(at="../ARCHITECTURE_GPU_PREFILL_MOE.md#prefill-moe-buckets")]
def private pf_enc_moe_mm(enc : MetalComputeEncoder?; t : Model; fmt : KqFmt; stack_off, rows, npos : int64;
                          bx, by, bcnt, bbase, bbkt : MetalBuffer?; var ka : MoeMmArgs;
                          contiguous : bool = false; bxh : MetalBuffer? = null;
                          bkd : MetalBuffer? = null; bnd : MetalBuffer? = null; mpad : int64 = 0l) {
    return if (g_pf_skip == "moe_mm_disp")   //! attribution: gather+cvt kept, mm dispatches out
    if (fmt == KqFmt.q8) {
        pf_enc_moe_mm_q8(enc, t, stack_off, rows, npos, bx, by, bcnt, bbase, bbkt, ka, contiguous, bxh)
    } elif (kq_dense_b32(fmt)) {
        pf_enc_moe_mm_b32(enc, t, fmt, stack_off, rows, npos, bx, by, bcnt, bbase, bbkt, ka, contiguous, bxh, bkd, bnd, mpad)
    } else {
        pf_enc_moe_mm_kq(enc, t, fmt, stack_off, rows, npos, bx, by, bcnt, bbase, bbkt, ka, contiguous, bxh, bkd, bnd, mpad)
    }
}

//! the q8 routed site: the blob's tensor twins under their crown over contiguous rows, else the simdgroup base
def private pf_enc_moe_mm_q8(enc : MetalComputeEncoder?; t : Model; stack_off, rows, npos : int64;
                             bx, by, bcnt, bbase, bbkt : MetalBuffer?; var ka : MoeMmArgs; contiguous : bool; bxh : MetalBuffer?) {
    let ne = t.config.n_expert
    let nk = int64(ka.nk)
    let bw = blob_of(g_dev, t, stack_off)
    if (g_pf_moe_mm_q8_tensor && contiguous && g_pf_pso_moe_mm_q8_t != null) {
        if (bxh != null && g_pf_pso_moe_mm_q8_th != null) {
            if (pf_moe_tall_pick(npos, nk, ne, g_pf_pso_moe_mm_q8_th128, g_pf_pso_moe_mm_q8_thr)) {
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
}

//! the per-32 routed formats' pick among their four values, in the order q5_1, iq4nl32, q4032, q5032
def private b32_moe_pick(fmt : KqFmt; a51, aiq, a40, a50 : auto(T)) : T =>
    fmt == KqFmt.q51 ? a51 : (fmt == KqFmt.iq4nl32 ? aiq : (fmt == KqFmt.q4032 ? a40 : a50))

//! the per-32 routed site (q5_1, iq4nl32, q4032, q5032): the plane pair's split tensor twins under the format's verdict at a
//! 64-deep K, else the base
def private pf_enc_moe_mm_b32(enc : MetalComputeEncoder?; t : Model; fmt : KqFmt; stack_off, rows, npos : int64;
                              bx, by, bcnt, bbase, bbkt : MetalBuffer?; var ka : MoeMmArgs; contiguous : bool; bxh, bkd, bnd : MetalBuffer?;
                              mpad : int64) {
    let ne = t.config.n_expert
    let nk = int64(ka.nk)
    let qp = b32_planes_of(g_dev, t, fmt, stack_off)
    let row <- pf_moe_kq_row(fmt)
    if (contiguous && row.pso_t != null && bkd != null && bnd != null && pf_moe_twin_live(fmt, int64(ka.kdim), npos, nk, ne)) {
        pf_enc_moe_split_twins(enc, row, (buf = qp.qbuf, qoff = qp.qoff), (buf = qp.sbuf, soff = qp.soff, doff = qp.soff),
            bx, bxh, by, bkd, bnd, bcnt, bbase, mpad, rows, ne, nk, npos)
        return
    }
    invoke(b32_moe_pick(fmt, @@pf_enc_moe_mm_q51_c, @@pf_enc_moe_mm_iq4nl32_c, @@pf_enc_moe_mm_q4032_c, @@pf_enc_moe_mm_q5032_c),
        enc, qp.sbuf, qp.soff, qp.qbuf, qp.qoff, bx, by, ka, bcnt, bbase, bbkt, rows, npos, ne)
}

//! a superblock routed site through its `pf_moe_kq_row` row: the split twins where the pick admits them, else the base
def private pf_enc_moe_mm_kq(enc : MetalComputeEncoder?; t : Model; fmt : KqFmt; stack_off, rows, npos : int64;
                             bx, by, bcnt, bbase, bbkt : MetalBuffer?; var ka : MoeMmArgs;
                             contiguous : bool; bxh, bkd, bnd : MetalBuffer?; mpad : int64) {
    let ne = t.config.n_expert
    let nk = int64(ka.nk)
    let row <- pf_moe_kq_row(fmt)
    let bq = kq_quants_of(g_dev, t, fmt, stack_off)
    let bs = kq_scales_of(g_dev, t, fmt, stack_off)
    if (contiguous && row.pso_t != null && bkd != null && bnd != null && pf_moe_twin_live(fmt, int64(ka.kdim), npos, nk, ne)) {
        pf_enc_moe_split_twins(enc, row, bq, bs, bx, bxh, by, bkd, bnd, bcnt, bbase, mpad, rows, ne, nk, npos)
        return
    }
    let d0 = row.d_soff ? bs.soff : bs.doff
    invoke(row.enc_base, enc, bs.buf, d0, bs.buf, bs.soff, bq.buf, bq.qoff, bx, by, ka, bcnt, bbase, bbkt, rows, npos, ne)
}

//! a routed site's split tensor twins over the contiguous gather panel: the tall pair past `pf_moe_tall_pick`, else the half-X
//! twin, the f32-X twin where no half panel was minted
def private pf_enc_moe_split_twins(enc : MetalComputeEncoder?; row : MoeKqRow; bq : tuple<buf : MetalBuffer?; qoff : uint64>;
                                   bs : tuple<buf : MetalBuffer?; soff : uint64; doff : uint64>; bx, bxh, by, bkd, bnd, bcnt, bbase : MetalBuffer?;
                                   mpad, rows, ne, nk, npos : int64) {
    let d0 = row.d_soff ? bs.soff : bs.doff
    if (bxh != null && row.pso_th != null) {
        if (pf_moe_tall_pick(npos, nk, ne, row.pso_th128, row.pso_thr)) {
            invoke(row.enc_th128, enc, bs.buf, d0, bs.buf, bs.soff, bq.buf, bq.qoff, bxh, 0ul, by, 0ul, bkd, bnd, bcnt, bbase, mpad, rows, ne, npos / 128l)
            invoke(row.enc_thr, enc, bs.buf, d0, bs.buf, bs.soff, bq.buf, bq.qoff, bxh, 0ul, by, 0ul, bkd, bnd, bcnt, bbase, mpad, rows, ne, 4l)
        } else {
            invoke(row.enc_th, enc, bs.buf, d0, bs.buf, bs.soff, bq.buf, bq.qoff, bxh, 0ul, by, 0ul, bkd, bnd, bcnt, bbase, mpad, rows, ne, (npos + 31l) / 32l)
        }
    } else {
        invoke(row.enc_t, enc, bs.buf, d0, bs.buf, bs.soff, bq.buf, bq.qoff, bx, 0ul, by, 0ul, bkd, bnd, bcnt, bbase, mpad, rows, ne, (npos + 31l) / 32l)
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








//! `qboff` is the segment's first Q row as a byte offset of the panel bound - the f32 panel's or the half twin's.
def enc_qk_mm(enc : MetalComputeEncoder?; bq, bk, batt : MetalBuffer?; var ka : AttnArgs; mp, nk64, heads : int64; bqh : MetalBuffer? = null; qboff : uint64 = 0ul; qhoff : uint64 = 0ul) {   //! public: the tower driver dispatches the crowned trio too
    if (g_pf_qkmm_tensor && g_pf_pso_qkmm_t != null) {
        if (bqh != null && g_pf_pso_qkmm_th != null) {
            enc_qk_mm_th_c(enc, bqh, qhoff, bk, batt, ka, mp, nk64, heads)
        } else {
            enc_qk_mm_t_c(enc, bq, qboff, bk, batt, ka, mp, nk64, heads)
        }
    } else {
        enc_qk_mm_c(enc, bq, qboff, bk, batt, ka, mp, nk64, heads)
    }
}

//! Whether the device pair can serve: its crown, no hold, and every stamp of the trio compiled.
def pf_attn_dev_ready() : bool {
    return g_pf_attn_dev && !pf_held("attn_dev") && psos_ready(fixed_array(g_pf_pso_half_rows, g_pf_pso_qk_dev,
        g_pf_pso_qk_dev128, g_pf_pso_pexp, g_pf_pso_av_dev, g_pf_pso_av_dev128, g_pf_pso_av_dev_t, g_pf_pso_av_dev128_t))
}

//! the 128-row stamp where the query panel divides by 128, the 32-row stamp otherwise
[arch(at="../ARCHITECTURE_GPU_PREFILL_WINDOW.md#prefill-attn-device")]
def private enc_qk_dev(enc : MetalComputeEncoder?; bqh, bkh, batt : MetalBuffer?; var ka : AttnArgs; mp, nk64, heads : int64; koff : uint64 = 0ul; qboff : uint64 = 0ul) {
    if (mp % 128l == 0l) {
        enc_qk_dev128_c(enc, bqh, qboff, bkh, koff, batt, ka, mp, nk64, heads)
    } else {
        enc_qk_dev_c(enc, bqh, qboff, bkh, koff, batt, ka, mp, nk64, heads)
    }
}

[arch(at="../ARCHITECTURE_GPU_PREFILL_WINDOW.md#prefill-attn-device")]
def private enc_av_dev(enc : MetalComputeEncoder?; batt, bvh, bxb : MetalBuffer?; var ka : AttnArgs; bstat : MetalBuffer?; mp, heads, head_size : int64; voff : uint64 = 0ul; xboff : uint64 = 0ul) {
    let ntile = head_size / 64l   //! the head's whole 64-wide tiles; the grid is theirs, not a ceiling that would write past the head
    if (mp % 128l == 0l) {
        enc_av_dev128_c(enc, batt, bvh, voff, bxb, xboff, ka, bstat, mp, heads, ntile)
    } else {
        enc_av_dev_c(enc, batt, bvh, voff, bxb, xboff, ka, bstat, mp, heads, ntile)
    }
    if (head_size % 64l != 0l) {
        if (mp % 128l == 0l) {
            enc_av_dev128_t_c(enc, batt, bvh, voff, bxb, xboff, ka, bstat, mp, heads, head_size)
        } else {
            enc_av_dev_t_c(enc, batt, bvh, voff, bxb, xboff, ka, bstat, mp, heads, head_size)
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
//! `xboff` is the segment's first output row as a byte offset of `bxb`.
def enc_av_mm(enc : MetalComputeEncoder?; batt, bv, bxb : MetalBuffer?; var ka : AttnArgs; bstat : MetalBuffer?; mp, heads, head_size : int64; xboff : uint64 = 0ul) {   //! public: the tower driver dispatches the crowned trio too
    let walk_live = ka.qoff + ka.qrows == ka.npos
    if (g_pf_avmm_tensor && g_pf_pso_avmm_t != null && ka.head_size % 64u == 0u) {
        if (walk_live && ka.npos % 64u == 0u && g_pf_pso_avmm_tp != null) {
            enc_av_mm_tp_c(enc, batt, bv, bxb, xboff, ka, bstat, mp, heads, head_size)
        } else {
            enc_av_mm_t_c(enc, batt, bv, bxb, xboff, ka, bstat, mp, heads, head_size)
        }
    } elif (walk_live && ka.npos % 32u == 0u && g_pf_pso_avmm_p != null) {
        enc_av_mm_p_c(enc, batt, bv, bxb, xboff, ka, bstat, mp, heads, head_size)
    } else {
        enc_av_mm_c(enc, batt, bv, bxb, xboff, ka, bstat, mp, heads, head_size)
    }
}



//! Batched f32-slab GEMV on MetalMoeRouterB's builder: d rows per position, y[p*d + row].
def private pf_enc_slab_gemv_b(enc : MetalComputeEncoder?; bw, bx, by : MetalBuffer?; n, d, npos : int64) {
    var ka = moe_router_args(d, n, npos)
    enc_f32_rows_gemv(enc, bw, bx, by, g_one, ka, d, npos, true)
}

//! the window's rows through the file's f32 rows at `off` in fblob (`n` in, `d` out): the f32 tile (`MetalF32ExactMm`), else the
//! slab GEMV
def private pf_enc_f32_rows(enc : MetalComputeEncoder?; t : Model; off, n, d : int64; bx, by, bk, bn : MetalBuffer?; mp, npos : int64) {
    var bw = upload_region(unsafe(addr<void?>(t.fblob[off])), uint64(n * d * 4l))
    if (!pf_enc_router_mm(enc, bw, bx, by, bk, bn, d, n, mp)) {
        pf_enc_slab_gemv_b(enc, bw, bx, by, n, d, npos)
    }
}

//! the window's rows through layer l's per-layer-embedding gate (dim -> ple), or with `proj` its projection (ple -> dim), in the
//! plane's own format: a tagged plane on its dense site, the file's f32 rows on the f32 tile (`MetalF32ExactMm`) or the slab GEMV
def private pf_enc_ple_site(enc : MetalComputeEncoder?; t : Model; l : int64; proj : bool; bx, by, bk, bn : MetalBuffer?; mp, npos : int64;
                            bxh : MetalBuffer?) {
    let pp = ple_plane(t, l, proj)
    if (!pp.f32) {
        pf_enc_site_mm(enc, t, pp.f, pp.off, pp.d, bx, by, bk, bn, mp, pp.n, npos, bxh)
        return
    }
    pf_enc_f32_rows(enc, t, pp.off, pp.n, pp.d, bx, by, bk, bn, mp, npos)
}


//! whether layer l's per-layer gate (`proj` off) or projection serves the narrowed last row: the file's f32 rows, or a plane the
//! one-row GEMV takes
def private pf_narrow_ple_ok(t : Model; l : int64; proj : bool) : bool {
    let pp = ple_plane(t, l, proj)
    return pp.f32 || kq_fmt_one_row_gemv(pp.f, pp.n)
}

//! layer l's per-layer gate (dim -> ple) or projection (ple -> dim) over the narrowed last row at row 0 of `bx` (`pf_narrow_ple_ok`)
def private pf_enc_narrow_ple(enc : MetalComputeEncoder?; t : Model; l : int64; proj : bool; bx, by, bk, bn : MetalBuffer?) {
    let pp = ple_plane(t, l, proj)
    if (!pp.f32) {
        pf_enc_narrow_gemv(enc, t, pp.f, pp.off, bx, by, bk, bn, pp.n, pp.d)
        return
    }
    var bw = upload_region(unsafe(addr<void?>(t.fblob[pp.off])), uint64(pp.n * pp.d * 4l))
    pf_enc_slab_gemv_b(enc, bw, bx, by, pp.n, pp.d, 1l)
}

//! the bf16 tile GEMM (`pf_enc_bf16_mm`) steps a K of `kdim` in its 32-element chunks and its `nd` output rows in 64-row tiles;
//! a projection off that grid rides the position-blocked bf16 GEMV
def pf_bf16_tile_ok(kdim, nd : int64) : bool => kdim % 32l == 0l && nd % 64l == 0l

//! the window's form for the E-series pre-step's projection
enum PfPleProjForm {
    bf16_tile   //! the file's bf16 rows on the tile GEMM (`pf_enc_bf16_mm`)
    bf16_gemv   //! the bf16 rows off the tile's grid, on the position-blocked GEMV (`enc_ple_proj_bf16_rows`)
    f32_rows    //! the file's f32 rows (`pf_enc_f32_rows`)
}

//! the form the window projects `t`'s embedded rows into its side rows on: the bf16 tile where the model dim and layers x ple sit
//! on its grid (`pf_bf16_tile_ok`), else the bf16 GEMV; the f32 rows where the file holds the projection in f32
def pf_ple_proj_form(t : Model) : PfPleProjForm {
    return PfPleProjForm.f32_rows if (!t.ple_model_bf16)
    let tile = pf_bf16_tile_ok(t.config.dim, t.config.n_layers * t.config.n_embd_per_layer)
    return tile ? PfPleProjForm.bf16_tile : PfPleProjForm.bf16_gemv
}

//! true iff THIS window's metal prefill will engage AND the pre-step kernels cover the model
[unused_argument(s, start_pos)]
def private metal_ple_pre_gpu_gate(t : Model; s : Session; npos, start_pos : int64) : bool {
    if (active_prefill_override() != "metal" || !metal_ple_dev_said("prefill", t)
            || prefill_decline_caps(t, npos) != MetalPrefillDecline.none) {
        return false
    }
    return g_pf_inited || metal_prefill_init()
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
def private pf_hc_mix(enc : MetalComputeEncoder?; t : Model; h : PfHc; norm_off, down_off, up_off, inject_off : int64; by, binj : MetalBuffer?) {
    let c = t.config
    let hcd = hc_dim(c)
    let lr = c.hc_low_rank
    unsafe {
        if (g_pf_skip != "ew" && g_pf_skip != "nongemm") {
            var bw_n = upload_region(addr < void? >(t.fblob[norm_off]), uint64(hcd * 4l))
            enc_hc_norm(enc, h.bhc, h.bxn, bw_n, hc_norm_args(c), h.npos * c.hc_count)
        }
        if (g_pf_skip != "gemm") {
            if (inject_off >= 0l) {
                var bw_i = upload_region(addr < void? >(t.fblob[inject_off]), uint64(c.hc_count * hcd * 4l))
                pf_enc_slab_gemv_b(enc, bw_i, h.bxn, binj, hcd, c.hc_count, h.npos)
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
            enc_hc_mix(enc, h.bxn, h.bgate, by, hc_mix_args(c, h.npos), h.npos * c.dim)
        }
    }
}

//! layer l's mixer at its attention (ffn = false) or ffn site - the CPU path's mix_prefill
def private pf_hc_mix_site(enc : MetalComputeEncoder?; t : Model; h : PfHc; l : int64; ffn : bool; by, binj : MetalBuffer?) {
    let site = l * 2l + (ffn ? 1l : 0l)
    let hcd = hc_dim(t.config)
    pf_hc_mix(enc, t, h, t.hc_norm_off + site * hcd, t.hc_down_offs[site], t.hc_up_offs[site],
        t.hc_inject_off + site * hcd * t.config.hc_count, by, binj)
}

//! a block's output rows scattered into the streams under the site's logits
def private pf_hc_combine(enc : MetalComputeEncoder?; t : Model; h : PfHc; bout, binj : MetalBuffer?) {
    if (g_pf_skip != "ew" && g_pf_skip != "nongemm") {
        enc_hc_combine(enc, h.bhc, bout, binj, hc_comb_args(t.config, h.npos), h.npos * hc_dim(t.config))
    }
}

//! the head mixer over the window's last wide row into bxf (the classifier's input): the single row's
//! chain over row 0 of the mixer scratch, the row copied out of the wide residual first
def private pf_hc_head(enc : MetalComputeEncoder?; t : Model; h : PfHc; bxf : MetalBuffer?) {
    let c = t.config
    let hcd = hc_dim(c)
    let lr = c.hc_low_rank
    enc_copy_row(enc, h.bhc, uint64((h.npos - 1l) * hcd * 4l), h.bgate, 0ul, h.u_hcd, hcd)
    var bw_n = upload_region(unsafe(addr < void? >(t.fblob[t.hc_head_norm_off])), uint64(hcd * 4l))
    enc_hc_norm(enc, h.bgate, h.bxn, bw_n, hc_norm_args(c), c.hc_count)
    enc_site_gemv(enc, t, KqFmt.q8, t.hc_head_down_off, lr, hcd, h.bxn, h.blo, h.u_hcd, h.u_lr)
    enc_hc_lo(enc, h.blo, 0ul, h.u_lr, h.u_hc, lr)
    enc_site_gemv(enc, t, KqFmt.q8, t.hc_head_up_off, hcd, lr, h.blo, h.bgate, h.u_lr, h.u_hcd)
    enc_hc_mix(enc, h.bxn, h.bgate, bxf, hc_mix_args(c, 1l), c.dim)
}

//! the n-gram side input over the window before layer ple_layer's mixer: the key/value GEMMs, gate and conv, each capture's ring into the arena, then the window's last rows into the ring
def private pf_ple_side(enc : MetalComputeEncoder?; t : Model; h : PfHc; p : PfPle; u_dim : MetalBuffer?; start_pos : int64; bring : MetalBuffer?; ring_base : uint64;
                        caps : array<DnSnapshot>; bcap : MetalBuffer?; cap_bytes, cap_ring_off : int64) {
    let c = t.config
    let hcd = hc_dim(c)
    let ind = ple_in_dim(c)
    let rows = ple_hist_rows(c)
    if (g_pf_skip != "gemm") {
        let bxh_e = pf_cvt_panel(enc, p.bemb, h.mp, ind, g_pf_skip == "act_cvt")
        let bwk = blob_of(g_dev, t, t.ple_key_off)
        enc_gemm_mm(enc, bwk.buf, bwk.boff, p.bemb, p.bkey, p.u_in, h.u_hcd, h.mp, hcd, 0ul, ind, h.npos, bxh_e)
        let bwv = blob_of(g_dev, t, t.ple_val_off)
        enc_gemm_mm(enc, bwv.buf, bwv.boff, p.bemb, p.bval, p.u_in, u_dim, h.mp, c.dim, 0ul, ind, h.npos, bxh_e)
    }
    if (g_pf_skip != "ew" && g_pf_skip != "nongemm") {
        enc_ple_side_rows(enc, t, h.bhc, 0ul, p.bkey, 0ul, p.bval, 0ul, p.bgated, 0ul, p.bcin, 0ul, bring, ring_base, start_pos, h.npos, true)
        for (cap, ci in caps, count()) {
            let r = cap.pos - start_pos
            let cbase = uint64(int64(ci) * cap_bytes + cap_ring_off)
            cap_ring_rows(r, rows, start_pos) $(rr, slot : int64; from_window : bool) {
                let soff = uint64(slot * hcd * 4l)
                if (from_window) {
                    enc_copy_row(enc, p.bcin, uint64(rr * hcd * 4l), bcap, cbase + soff, h.u_hcd, hcd)
                } else {
                    enc_copy_row(enc, bring, ring_base + soff, bcap, cbase + soff, h.u_hcd, hcd)
                }
            }
        }
        for (r in range64(max(0l, h.npos - rows), h.npos)) {
            enc_copy_row(enc, p.bcin, uint64(r * hcd * 4l), bring, ring_base + uint64(((start_pos + r) % rows) * hcd * 4l), h.u_hcd, hcd)
        }
    }
}

def private dn_captures_from_arena(var segs : array<PfSeg>; bcap : MetalBuffer?; cap_bytes : int64) {
    var capi = 0l
    for (sg in segs) {
        var ss & = unsafe(*sg.s)
        let sb = uint64(long_length(ss.dn_state) * 4l)
        let cb = uint64(long_length(ss.dn_conv_state) * 4l)
        for (cap in ss.dn_captures) {
            unsafe {
                var base = reinterpret<uint8?>(contents_at(bcap, uint64(capi * cap_bytes)))
                dn_spans_copy(cap.state, cap.conv, cap.ple_hist, base, base + sb, base + sb + cb, true)
            }
            capi++
        }
        ss.dn_captured = !empty(ss.dn_captures)
    }
}

//! One segment of a prefill window: a session's chunk of `npos` rows from its position `start_pos`, standing at the window's
//! row `row0`. The window's rows are its segments' back to back; a segment's attention, recurrent scan, K/V rows and last-row
//! logits are its own, the row-independent stages the window's.
struct PfSeg {
    s : Session?
    start_pos : int64
    npos : int64
    row0 : int64
    mp : int64            //!< the segment's query rows padded to the attention tiles' 32
    nk : int64            //!< the segment's keys: its session's cached rows and its own
    nk64 : int64
    kvrows : int64        //!< the panel form's rows: the gathered context and the chunk
    mir_koff : uint64
    mir_voff : uint64
    mir_cap : int64
    ple_ring_base : uint64
    u_kv : MetalBuffer?   //!< the chunk's K or V elements, the base class
    u_kv_sw : MetalBuffer?
    store_nrun : int64    //!< the chunk's physical runs in its session's cache
}

//! a segment's K and V panels at layer `l`: its own gathered pair under the panel form over several segments, the window's otherwise
def private pf_seg_panels(seg_panels : bool; i, nl_kv, l : int64; sbks, sbvs, bks, bvs : array<MetalBuffer?>) : tuple<k : MetalBuffer?; v : MetalBuffer?> {
    let at = i * nl_kv + l
    return (k = seg_panels ? sbks[at] : bks[l], v = seg_panels ? sbvs[at] : bvs[l])
}

//! a buffer's contents at a byte offset
def private contents_at(buf : MetalBuffer?; off : uint64) : void? {
    unsafe {
        return reinterpret<void?>(reinterpret<uint8?>(metal_buffer_contents(buf)) + off)
    }
}

//! The panel form's per-layer K/V panels of `rows` rows into `ks`/`vs` from slot `base`: none for a recurrent layer, the
//! source's pair for a shared layer, an acquired pair otherwise; `pf_kv_panels_release` returns the acquired ones.
def private pf_kv_panels_acquire(t : Model; rows, base, nl_kv : int64; var ks, vs : array<MetalBuffer?>) {
    let c = t.config
    var null_kv_panel : MetalBuffer?
    for (l in range64(nl_kv)) {
        if (layer_kv_dim(c, l) == 0l) {   //! recurrent layer (deltanet): no K/V at all
            ks[base + l] = null_kv_panel
            vs[base + l] = null_kv_panel
        } elif (kv_shared_layer(t, l)) {   //! E-series: alias the source layer's panels (same class geometry)
            ks[base + l] = ks[base + t.kv_src[l]]
            vs[base + l] = vs[base + t.kv_src[l]]
        } else {
            let bytes_kv_l = uint64(rows * layer_kv_dim(c, l) * 4l)   //! per-layer panels: each class's width
            ks[base + l] = pool_acquire_pinned(g_pf_pool, g_dev, bytes_kv_l)
            vs[base + l] = pool_acquire_pinned(g_pf_pool, g_dev, bytes_kv_l)
        }
    }
}

def private pf_kv_panels_release(t : Model; rows, base, nl_kv : int64; var ks, vs : array<MetalBuffer?>) {
    let c = t.config
    for (l in range64(nl_kv)) {
        continue if (layer_kv_dim(c, l) == 0l || kv_shared_layer(t, l))
        let bytes_kv_l = uint64(rows * layer_kv_dim(c, l) * 4l)
        pool_release(g_pf_pool, ks[base + l], bytes_kv_l)
        pool_release(g_pf_pool, vs[base + l], bytes_kv_l)
    }
}

var private @scratch g_pf_segs : array<PfSeg>              //!< the one-segment window's record, reused
var private @scratch g_pf_sbks : array<MetalBuffer?>       //!< the panel form's per-segment K panels, [segment][layer]
var private @scratch g_pf_sbvs : array<MetalBuffer?>

[hot_path]
def metal_prefill_forward(t : Model; var s : Session; npos, start_pos : int64) : bool {
    g_pf_segs |> clear()
    g_pf_segs |> push(PfSeg(s = unsafe(addr(s)), start_pos = start_pos, npos = npos))
    let ok = metal_prefill_window(t, g_pf_segs)
    g_pf_segs |> clear()   //! the records borrow the session and the recurrent mirror's buffer: nothing outlives the call
    return ok
}

//! the window's token ids at each segment's rows; `shifted` = the warm's row p reads the segment's token p + 1, its last row token 0
def private pf_fill_window_tokens(var b : MetalBuffer?; segs : array<PfSeg>; shifted : bool) {
    unsafe {
        var tp = reinterpret<uint?>(metal_buffer_contents(b))
        for (sg in segs) {
            for (p in range64(sg.npos)) {
                tp[sg.row0 + p] = uint(sg.s.embed_gpu_tokens[shifted ? (p + 1l) % sg.npos : p])
            }
        }
    }
}

def private pf_segments_servable(t : Model; segs : array<PfSeg>) : bool {
    let c = t.config
    return false if (c.hyper_conn || has_ple_ngram(c))
    let s0 = segs[0].s
    for (sg in segs) {
        let s = sg.s
        return false if (s.attn_uniform_end != 0l || !empty(s.mrope_pos) || s.ds_active || s.wide_src != null
            || s.kv_dtype_k != s0.kv_dtype_k || s.kv_dtype_v != s0.kv_dtype_v || s.device_kv != s0.device_kv
            || s.ple_gpu_pending != s0.ple_gpu_pending || s.keep_hidden != s0.keep_hidden || s.kv_pool != s0.kv_pool)
    }
    return true
}

[hot_path, arch(at="../ARCHITECTURE_GPU_PREFILL.md#prefill-pad-rows-and-coop"), arch(at="../ARCHITECTURE_GPU_PREFILL.md#prefill-chunked-submit"), arch(at="../ARCHITECTURE_GPU_PREFILL.md#prefill-twin-dual-store"), arch(at="../ARCHITECTURE_GPU_PREFILL.md#prefill-last-row-tail"), arch(at="../ARCHITECTURE_GPU_PREFILL_WINDOW.md#prefill-segments")]
def metal_prefill_window(t : Model; var segs : array<PfSeg>) : bool {   // nolint:STYLE037,STYLE038 — the prefill step driver; the layer stack's phases are hz-barrier coupled
    let nseg = long_length(segs)
    var s & = unsafe(*segs[0].s)   //! the segments agree on every session-wide shape the window reads; the first speaks for them
    var npos = 0l
    for (sg in segs) {
        sg.row0 = npos
        npos += sg.npos
    }
    var why = MetalPrefillDecline.none
    for (sg in segs) {
        why = prefill_decline(t, *sg.s, sg.npos, sg.start_pos)
        break if (why != MetalPrefillDecline.none)
    }
    if (why == MetalPrefillDecline.none && nseg > 1l) {
        why = prefill_decline_caps(t, npos)
    }
    if (why == MetalPrefillDecline.none && !metal_prefill_init()) {
        why = MetalPrefillDecline.device
    }
    if (why == MetalPrefillDecline.none) {
        pf_devw_seed_baked(t)   //! baked image panels land in the resident cache before any encode
    }
    if (why != MetalPrefillDecline.none) {
        for (sg in segs) {
            if (sg.s.ple_gpu_pending) {   //! the gate promised a device pre-step — build it on CPU first
                ple_pre_prefill(t, *sg.s, sg.s.ple_gpu_tokens, sg.npos)
                sg.s.ple_gpu_pending = false
            }
        }
        if (why == MetalPrefillDecline.feature) {
            decline(why, metal_missing_needs(t, PF_NEEDS_OK))
        } else {
            decline(why, metal_fmt_decline_say("prefill", why, t))
        }
        return false
    }
    residency_flush()   //! pin/commit before the first submission (the post-CPU-window slack)
    let ts_all = ref_time_ticks()
    let devw0 = metal_prefill_devw_scratch_stats()
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
    //! the recurrent-state mirrors sync BEFORE encoding - a cold mid-stream window has no repair path
    let dn = c.recr_mask != 0ul
    var dn_tab : tuple<buf : MetalBuffer?; bytes : uint64>
    var dn_npmax = 0l
    var bcap : MetalBuffer? = null
    var bytes_cap = 0ul
    var cap_bytes = 0l
    if (dn) {
        dn_mirror_room(nseg)
        dn_grp_clear()
        var ncap = 0l
        for (sg in segs) {
            ncap += long_length(sg.s.dn_captures)
        }
        cap_bytes = (long_length(segs[0].s.dn_state) + long_length(segs[0].s.dn_conv_state) + long_length(segs[0].s.ple_conv_hist)) * 4l
        if (ncap > 0l) {
            bytes_cap = uint64(ncap * cap_bytes)
            bcap = pool_acquire_untracked(g_upool, g_dev, bytes_cap)
        }
        var capi = 0l
        for (sg in segs) {
            var dm = dn_mirror_prepare(*sg.s, sg.start_pos)
            if (!dm.ok) {
                decline(MetalPrefillDecline.dn_state)
                return false
            }
            let nc = long_length(sg.s.dn_captures)
            dn_grp_push(sg.row0, sg.npos, dm.state_off, dm.state_off, dm.conv_off, dm.conv_off, nc, DN_GRP_WORDS * nseg + DN_CAP_WORDS * capi)
            capi += nc
            dn_npmax = max(dn_npmax, sg.npos)
            sg.ple_ring_base = dm.ple_off
        }
        capi = 0l
        for (sg in segs) {
            for (cap in sg.s.dn_captures) {
                let base = uint64(capi * cap_bytes)
                dn_grp_cap(cap.pos - sg.start_pos, base, base + uint64(long_length(sg.s.dn_state) * 4l))
                capi++
            }
        }
        dn_tab = dn_grp_upload()
    }
    let hs_sw = c.head_size_swa > 0l ? c.head_size_swa : head_size
    let kvh_sw = c.n_kv_heads_swa > 0l ? c.n_kv_heads_swa : c.n_kv_heads
    let qd_sw = n_heads * hs_sw
    let kvd_sw = kvh_sw * hs_sw
    let hetero = hs_sw != head_size || kvh_sw != c.n_kv_heads
    let qd_mx = max(qd, qd_sw)
    let half_sw = hs_sw / 2l
    let mp = ((npos + 31l) / 32l) * 32l
    let mp_q = nseg > 1l ? mp + 32l : mp
    var nk64_mx = 0l
    var mp_seg_mx = 0l
    var att_slab_mx = 0l   //! the largest score slab any segment asks: its padded queries by its padded keys
    var seq_ok = true
    for (sg in segs) {
        sg.mp = ((sg.npos + 31l) / 32l) * 32l
        sg.nk = sg.start_pos + sg.npos
        sg.nk64 = ((sg.nk + 63l) / 64l) * 64l
        nk64_mx = max(nk64_mx, sg.nk64)
        mp_seg_mx = max(mp_seg_mx, sg.mp)
        att_slab_mx = max(att_slab_mx, sg.mp * sg.nk64)
        seq_ok = seq_ok && sg.nk64 <= c.seq_len
    }
    //! the mirror-fed form: the chunk's K/V rows land in the session's KV mirror as halves and the device pair reads every key there,
    //! a ringed layer's through `kv_ring_row` (`AttnArgs.ring`)
    var npos_seg = 0l
    for (sg in segs) {
        npos_seg = max(npos_seg, sg.npos)
    }
    mirror_ring_ask(t, npos_seg)   //! the window's rows sit in a ring whole until the host store reads them back
    let hidden_only = prefill_logits_skipped() && s.keep_hidden   //! the segments share keep_hidden (`pf_segments_servable`)
    var mirk = (!hidden_only && g_pf_env_mirror && s.kv_dtype_k == KVDtype.f16 && s.kv_dtype_v == KVDtype.f16 && !s.device_kv
        && (head_size % 32l) == 0l && (hs_sw % 32l) == 0l && g_pf_env_attn && pf_attn_dev_ready()   //! the device pair takes a head off the 64 lattice through its tail stamp
        && seq_ok && g_pf_env_half_x && pf_twin_panels_wanted())   //! the Q twin rides a scratch panel, which covers the widest A panel
    //! the panel forms feed the mirror too, so the first decode step uploads none: an f16 session's whole panel as halves, another
    //! codec's window rows through its store stamp from the window's first row
    var mirw = (!mirk && !hidden_only && g_pf_env_mirror && pf_kv_feed_codec_ok(t, s) && !s.device_kv
        && seq_ok && psos_ready(fixed_array(g_pf_pso_half_rows)))
    let kvdt = s.kv_dtype_k
    let mirc = mirk || kvdt != KVDtype.f16   //! the mirror stands at the window's first row and takes the window's rows alone
    if (mirk || mirw) {
        var demand = 0ul
        var largest = 0ul
        for (sg in segs) {
            let d = mirror_arena_demand(t, *sg.s, sg.nk64 - 1l)
            demand += d
            largest = max(largest, d)
        }
        if (nseg > 1l) {   //! the reservation may rebuild the arena, which no step may be reading
            mirror_quiesce()
        }
        var all_ok = nseg == 1l || mirror_reserve(demand, largest)
        if (all_ok && nseg > 1l) {
            for (sg in segs) {
                g_pin |> insert(sg.s.uid)
            }
        }
        for (sg in segs) {
            break if (!all_ok)
            if (!mirror_stands(*sg.s, sg.start_pos, sg.nk64)) {   //! slices move or rows upload: no step may be writing one
                mirror_quiesce()
            }
            let mir = mirror_prepare_rows(t, *sg.s, mirc ? sg.start_pos : 0l, sg.nk64)
            //! the f16 forms convert into the arena at half-element offsets from its start, so the slice must lie in their reach
            all_ok = all_ok && mir.ok && (kvdt != KVDtype.f16 || mirror_slice_in_reach(sg.s.uid))
            sg.mir_koff = mir.koff
            sg.mir_voff = mir.voff
            sg.mir_cap = mir.cap
        }
        if (nseg > 1l) {
            g_pin |> clear()
        }
        mirk = mirk && all_ok
        mirw = mirw && all_ok
    }
    let seg_panels = nseg > 1l && !mirk
    var kvrows_mx = 0l
    for (sg in segs) {
        sg.kvrows = mirk ? sg.mp : max(sg.nk64, sg.start_pos + sg.mp)
        kvrows_mx = max(kvrows_mx, sg.kvrows)
    }
    let kvrows_w = (mirk || seg_panels) ? mp : segs[0].kvrows   //! the window's own K/V panel rows
    let bytes_x = uint64(mp * dim * 4l)
    let bytes_xb = uint64(mp_q * max(dim, qd_mx) * 4l)   //! dim-wide norm outs AND the qd-wide AV out
    let bytes_q = uint64(mp_q * qd_mx * 4l)
    let nsh = !t.blocks.ffn_is_dense ? c.n_ff_shexp : 0l   //! shared expert (qwen35moe) rides bhb/bhb2
    let bytes_h = uint64(mp * max(hid_mx, nsh) * 4l)
    let bytes_att = uint64(n_heads * att_slab_mx * 2l)   //! per-head f16 score slabs: a segment's mp queries x nk64 keys
    let bytes_stat = uint64(n_heads * mp_seg_mx * 2l * 4l)   //! per-row softmax stats: rowmax + 1/sum
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
    let mpad = moe ? pf_moe_padded_rows(mtot, c.n_expert) : 0l
    let moe_gemv_route = moe && pf_moe_gemv_route(t, npos)
    if (moe_gemv_route) {
        g_pf_moe_gemv_windows++
    }
    let bytes_mlg = uint64(mp * c.n_expert * 4l)   //! the router's GEMM lands every padded row
    let bytes_msel = uint64(mtot * 2l * 4l)
    let bytes_mcsr = uint64((c.n_expert + 1l) * 4l)   // basep carries the padded total at [ne]
    let rows_mg = moe_gemv_route ? mtot : mpad   //! the gathered route's panels hold the live (token, slot) rows alone, no pad
    let bytes_mbkt = uint64(rows_mg * 4l)
    let bytes_minv = uint64(mtot * 4l)
    let bytes_mg = uint64(rows_mg * c.n_ff_exp * 4l)
    let bytes_mdn = uint64(rows_mg * dim * 4l)
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
    let mxg_th = pf_moe_th_any()
    let want_mxg = moe && !moe_gemv_route && g_pf_pso_moe_gx != null && npos >= g_pf_mxg_min && mxg_th
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
    let nl_kv = c.n_layers + c.n_layer_nextn
    bks |> resize(nl_kv)
    bvs |> resize(nl_kv)
    pf_kv_panels_acquire(t, kvrows_w, 0l, nl_kv, bks, bvs)
    var sbks & = unsafe(g_pf_sbks)
    var sbvs & = unsafe(g_pf_sbvs)
    sbks |> resize(seg_panels ? nseg * nl_kv : 0l)
    sbvs |> resize(seg_panels ? nseg * nl_kv : 0l)
    let seg_counts = seg_panels || (mirw && kvdt == KVDtype.f32)   //! the chunk's element counts: the segment copies' and the f32 feed's
    for (sg, i in segs, count()) {
        if (seg_panels) {
            pf_kv_panels_acquire(t, sg.kvrows, int64(i) * nl_kv, nl_kv, sbks, sbvs)
        }
        if (seg_counts) {
            sg.u_kv = uniform_u32(uint(sg.npos * kv_dim))
            sg.u_kv_sw = hetero ? uniform_u32(uint(sg.npos * kvd_sw)) : null
        }
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
    var bple, bpleg, bple_proj, bple_tok : MetalBuffer?
    if (plef) {
        bpleg = pool_acquire_pinned(g_pf_pool, g_dev, bytes_ple_g)
        if (ple_gpu) {
            bple = pool_acquire_pinned(g_pf_pool, g_dev, bytes_ple_side)
            bple_proj = pool_acquire_pinned(g_pf_pool, g_dev, bytes_ple_proj)
            bple_tok = pool_acquire_untracked_pinned(g_pf_upool, g_dev, uint64(npos * 4l))
            unsafe {
                var tp = reinterpret<uint?>(metal_buffer_contents(bple_tok))
                for (sg in segs) {
                    for (p in range64(sg.npos)) {
                        tp[sg.row0 + p] = uint(sg.s.ple_gpu_tokens[p])
                    }
                }
            }
        } else {
            bple = pool_acquire_untracked_pinned(g_pf_upool, g_dev, bytes_ple_side)
            unsafe {
                //! each segment's ple_inp, position-major as the window reads it, at the segment's rows
                var dst = reinterpret<float?>(metal_buffer_contents(bple))
                for (sg in segs) {
                    memcpy(reinterpret<void?>(dst + sg.row0 * ple_all), addr < void? >(sg.s.ple_inp[0]), sg.npos * ple_all * 4l)
                }
            }
        }
    }
    //! one f16 scratch covering the largest A panel any mul_mm site converts
    let bytes_xh = max(max(max(max(bytes_xb, bytes_h), uint64(mp_q * 2l * dim * 4l)),
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
    let want_wh = ((metal_devw_panels() || mp >= DEVW_SCRATCH_ROWS) && g_pf_env_dev_w && g_pf_pso_hmm_th != null
        && (g_pf_pso_dq_q8 != null || pf_kq_dq_any()))
    g_pf_bwh = want_wh ? pool_acquire_pinned(g_pf_pool, g_dev, bytes_wh) : null
    g_pf_bwh2 = want_wh ? pool_acquire_pinned(g_pf_pool, g_dev, bytes_wh) : null
    g_pf_bwh_flip = false
    g_pf_bwh_bytes = want_wh ? bytes_wh : 0ul
    //! the token rows: the device gathers them where every segment's gate left them to it, else the host's rows go up
    var emb_dev = !wide
    for (sg in segs) {
        emb_dev = emb_dev && sg.s.embed_gpu_pending
    }
    var btok_win : MetalBuffer?
    if (emb_dev) {
        btok_win = pool_acquire_untracked_pinned(g_pf_upool, g_dev, uint64(npos * 4l))
        pf_fill_window_tokens(btok_win, segs, false)
    } else {
        for (sg in segs) {
            if (sg.s.embed_gpu_pending) {
                embed_backfill_rows(t, *sg.s, sg.npos)
            }
        }
    }
    //! rope tables are built for the chunk's GLOBAL positions (build_rope_table takes start_pos)
    unsafe {
        if (wide) {   //! ONE wide upload; bx and the bds slices extract on-device at encode time
            memcpy(metal_buffer_contents(bwide), reinterpret<void const?>(s.wide_src + s.wide_soff * wide_stride), npos * wide_stride * 4l)
        }
        for (sg in segs) {
            if (!wide && !emb_dev) {
                memcpy(contents_at(bx, uint64(sg.row0 * dim * 4l)), addr < void? >(sg.s.x_b[0]), sg.npos * dim * 4l)
            }
            memcpy(contents_at(bcos, uint64(sg.row0 * half * 4l)), addr < void? >(sg.s.rope_cos[0]), sg.npos * half * 4l)
            memcpy(contents_at(bsin, uint64(sg.row0 * half * 4l)), addr < void? >(sg.s.rope_sin[0]), sg.npos * half * 4l)
            if (dual_rope) {
                memcpy(contents_at(bcos_swa, uint64(sg.row0 * half_sw * 4l)), addr < void? >(sg.s.rope_cos_swa[0]), sg.npos * half_sw * 4l)
                memcpy(contents_at(bsin_swa, uint64(sg.row0 * half_sw * 4l)), addr < void? >(sg.s.rope_sin_swa[0]), sg.npos * half_sw * 4l)
            }
        }
    }
    //! tq4 rows dequant ROTATED and un-rotate per head - GPU attention runs in the raw basis; K rows are stored ROPED, so no re-rope
    if (!mirk) {
        for (sg, i in segs, count()) {
            continue if (sg.start_pos <= 0l)
            unsafe {
                var ss & = *sg.s
                let gather_nrun = kv_runs(ss, 0l, sg.start_pos, ss.kv_runs)
                let runsg = addr < KVRun const? >(ss.kv_runs[0])
                for (l in range64(nl_kv)) {
                    continue if (bks[l] == null || kv_shared_layer(t, l))
                    let pan = pf_seg_panels(seg_panels, int64(i), nl_kv, l, sbks, sbvs, bks, bvs)
                    var kdst = reinterpret<float?>(metal_buffer_contents(pan.k))
                    var vdst = reinterpret<float?>(metal_buffer_contents(pan.v))
                    let kcp = kv_layer_kbase(ss, l, c.seq_len)
                    let vcp = kv_layer_vbase(ss, l, c.seq_len)
                    let kvd_l = layer_kv_dim(c, l)
                    for (r in range64(gather_nrun)) {
                        let rowbase = runsg[r].row0 - runsg[r].j0
                        let cnt = (runsg[r].j1 - runsg[r].j0) * kvd_l
                        kv_load_row(kcp, ss.kv_dtype_k, kvd_l, rowbase + runsg[r].j0, 0l, cnt, kdst + runsg[r].j0 * kvd_l)
                        kv_load_row(vcp, ss.kv_dtype_v, kvd_l, rowbase + runsg[r].j0, 0l, cnt, vdst + runsg[r].j0 * kvd_l)
                    }
                    tq4_sides(ss, kdst, vdst, sg.start_pos, kvd_l, layer_head_size(c, l), true)
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
    var u_total_h = uniform_u32(uint(npos * hidden))
    var u_total_h2 = hid2 > 0l ? uniform_u32(uint(npos * hid2)) : null
    var u_moe_ne = moe ? uniform_u32(uint(c.n_expert)) : null
    var u_moe_nk = moe ? uniform_u32(uint(c.n_expert_used)) : null
    var u_moe_tot = moe ? uniform_u32(uint(mtot)) : null
    var u_moe_nfe = moe ? uniform_u32(uint(c.n_ff_exp)) : null
    var u_moe_ptot = moe ? uniform_u32(uint(mpad * c.n_ff_exp)) : null
    var u_moe_act_tot = moe_gemv_route ? uniform_u32(uint(mtot * c.n_ff_exp)) : null   //! the gathered route's live hidden elements
    var u_moe_invd = g4moe ? uniform_f32(1.0 / sqrt(float(dim))) : null
    var u_moe_gmode = moe ? uniform_u32(c.moe_gate == MoeGate.softmax_weight ? 1u : (c.norm_topk_prob ? 0u : 2u)) : null
    var u_moe_nsh = nsh > 0l ? uniform_u32(uint(nsh)) : null
    var u_moe_shtot = nsh > 0l ? uniform_u32(uint(npos * nsh)) : null
    var u_eps = uniform_f32(c.norm_eps)
    var u_esc = emb_dev && c.embed_scale != 1.0 ? uniform_f32(c.embed_scale) : null
    var u_dn_cd = dn ? uniform_u32(uint(cd_dn)) : null
    var u_dn_di = dn ? uniform_u32(uint(di_dn)) : null
    var u_dn_nvh = dn ? uniform_u32(uint(nvh_dn)) : null
    var u_dn_qd2 = c.q_gated ? uniform_u32(uint(2l * qd)) : null
    var u_qtot = c.q_gated ? uniform_u32(uint(npos * qd)) : null
    var u_rot = (c.rope_dim > 0l && c.rope_dim != head_size) ? uniform_u32(uint(c.rope_dim)) : null
    var u_ple = plef ? uniform_u32(uint(ple_n)) : null
    var u_ple_all = ple_gpu ? uniform_u32(uint(ple_all)) : null
    var u_ple_sq = ple_gpu ? uniform_f32(sqrt(float(ple_n))) : null   //! the q8 table stamp's scale: the gather's sqrt(ple)
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
    let cls_fmt = cls_fmt_of(t)
    let tied_f32 = cls_tied_f32(t)
    //! GPU logits stay on under the knockout rail (deltas stay clean), and a blob-only
    //! metal-flavor load has no CPU classifier to fall back to
    let gpu_logits = (!g_pf_logits_cpu && g_pf_env_logits && !hidden_only &&
        !tied_f32 &&
        t.wcls_off >= 0l)
    let bytes_xf = uint64(nseg * dim * 4l)
    let bytes_log = uint64(nseg * c.vocab_size * 4l)
    var bxf = gpu_logits ? pool_acquire_pinned(g_pf_pool, g_dev, bytes_xf) : null
    var bxf1 = gpu_logits && nseg > 1l ? pool_acquire_pinned(g_pf_pool, g_dev, uint64(dim * 4l)) : null
    var blog = gpu_logits ? pool_acquire_pinned(g_pf_pool, g_dev, bytes_log) : null
    var blog1 = gpu_logits && nseg > 1l ? pool_acquire_pinned(g_pf_pool, g_dev, uint64(c.vocab_size * 4l)) : null
    var u_vocab = gpu_logits ? uniform_u32(uint(c.vocab_size)) : null
    let nsupp = long_length(t.suppress)
    var bsupp : MetalBuffer?
    if (gpu_logits && nsupp > 0l) {
        bsupp = pool_acquire_untracked_pinned(g_pf_upool, g_dev, uint64(nsupp * 4l))
        unsafe {
            var sp = reinterpret<uint?>(metal_buffer_contents(bsupp))
            for (i in range64(nsupp)) {
                sp[i] = uint(t.suppress[i])
            }
        }
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
    var u_dim2, u_wm_tot, u_wm_hc_tot : MetalBuffer?
    var btok_wm : MetalBuffer?    //!< warm on the device gather: the shifted rows' token ids
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
        //! shifted embeds: row p pairs the NEXT position's embed with hidden p; a segment's garbage last row reuses its row 0
        if (emb_dev) {
            btok_wm = pool_acquire_untracked_pinned(g_pf_upool, g_dev, uint64(npos * 4l))
            pf_fill_window_tokens(btok_wm, segs, true)
        } else {
            unsafe {
                var dst = reinterpret<uint8?>(metal_buffer_contents(bwm_emb))
                for (sg in segs) {
                    let d0 = dst + uint64(sg.row0 * dim * 4l)
                    memcpy(reinterpret<void?>(d0), addr < void? >(sg.s.x_b[dim]), (sg.npos - 1l) * dim * 4l)
                    memcpy(reinterpret<void?>(d0 + (sg.npos - 1l) * dim * 4l), addr < void? >(sg.s.x_b[0]), dim * 4l)
                }
            }
        }
    }
    //! the tiled QK/AV pair needs hs % 64 on BOTH classes; DASLLAMA_METAL_ATTN=0 pins the trio
    let mmattn = (head_size % 64l) == 0l && (hs_sw % 64l) == 0l && g_pf_env_attn
    //! the device pair's K and V twins are one scratch pair every layer rewrites
    let attn_dev = mirk || (mmattn && mp >= CVT_MIN_ROWS && pf_attn_dev_ready())
    let kv_twins = attn_dev && !mirk
    let bytes_kvh = uint64(kvrows_mx * max(kv_dim, kvd_sw) * 2l)
    var bkh = kv_twins ? pool_acquire_pinned(g_pf_pool, g_dev, bytes_kvh) : null
    var bvh = kv_twins ? pool_acquire_pinned(g_pf_pool, g_dev, bytes_kvh) : null
    let ts_enc = ref_time_ticks()
    let setup_us = get_time_usec(ts_all)
    var err : string
    var gpu_ms, encode_ms : double
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
            if (l == 0l && emb_dev) {
                enc_embed_dev(enc, t, btok_win, 0ul, bx, 0ul, npos, u_dim, u_esc)
                if (btok_wm != null) {
                    enc_embed_dev(enc, t, btok_wm, 0ul, bwm_emb, 0ul, npos, u_dim, u_esc)
                }
            }
            if (l == 0l && wide) {   //! slice the wide quantum on-device: x, then the slice-major ds planes
                var wide_slice = GkRestrideArgs(rows = uint(npos), rows_pad = uint(npos), nheads = 1u, hs = uint(dim), hsp = uint(dim), sstride = uint(wide_stride), soff = 0u, boff = 0u, doff = 0u)
                enc_head_pad(enc, bwide, bx, wide_slice, npos * dim)
                for (sl in range64(nds)) {
                    wide_slice.soff = uint((sl + 1l) * dim)
                    wide_slice.doff = uint(sl * npos * dim)
                    enc_head_pad(enc, bwide, bds, wide_slice, npos * dim)
                }
            }
            if (l == 0l && hcs.on && g_pf_skip != "ew" && g_pf_skip != "nongemm") {   //! the wide residual: hc copies of every embed row
                enc_hc_init(enc, bx, hcs.bhc, hc_init_args(c, npos), npos * hcd)
            }
            if (l == 0l && ple_gpu) {
                //! the serial encoder orders the three PLE stages: gather -> model_proj GEMM -> rms+combine
                enc_ple_pre(enc, t, bple_tok, 0ul, bple, bple_proj, u_ple_all, u_ple_sq, npos) $ {
                    let form = pf_ple_proj_form(t)
                    if (form == PfPleProjForm.bf16_tile) {
                        let bwp = plane_of(g_dev, t, t.bf16blob, t.ple_model_off)
                        pf_enc_bf16_mm(enc, bwp.buf, bwp.boff, bx, bple_proj, u_dim, u_ple_all, mp, ple_all, pf_cvt_panel(enc, bx, mp, dim, g_pf_skip == "act_cvt"), dim)
                    } elif (form == PfPleProjForm.bf16_gemv) {
                        let bwp = plane_of(g_dev, t, t.bf16blob, t.ple_model_off)
                        var ka = moe_router_args(ple_all, dim, npos)
                        ka.woff = uint(bwp.boff / 2ul)
                        enc_ple_proj_bf16_rows(enc, bwp.buf, bx, bple_proj, ka, ple_all, npos, true)
                    } else {
                        pf_enc_f32_rows(enc, t, t.ple_model_off, dim, ple_all, bx, bple_proj, u_dim, u_ple_all, mp, npos)
                    }
                }
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
                        var bw_hn = upload_region(addr < void? >(t.fblob[t.mtp_hnorm_off]), uint64(hcd * 4l))
                        enc_hc_norm(enc, hcs.bhc, hcs.bxn, bw_hn, hc_norm_args(c), wm_rows)
                        enc_cat2(enc, bwm_cat, bxb, hcs.bxn, cat2_args(dim, wm_rows, c.hc_count), npos * hcd)
                    } else {
                        var bw_fin = upload_region(addr < void? >(t.fblob[t.rms_final_off]), uint64(dim * 4l))
                        pf_enc_rms(enc, bx, 0ul, bw_fin, bwm_tmp, u_dim, u_eps, npos)
                        var bw_hn = upload_region(addr < void? >(t.fblob[t.mtp_hnorm_off]), uint64(dim * 4l))
                        pf_enc_rms(enc, bwm_tmp, 0ul, bw_hn, bxb2, u_dim, u_eps, npos)
                        enc_cat2(enc, bwm_cat, bxb, bxb2, cat2_args(dim, npos), npos * dim)
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
                let head_kv = warm && l == c.n_layers   //! the draft head's prompt warm: its K/V rows are all a later round reads
                //! a hyper-connection model carries no per-layer norm rows: its mixers stand at both seams
                var bw_att = hcs.on ? null : upload_region(addr < void? >(t.fblob[t.rms_att_off + l * dim]), uint64(dim * 4l))
                var bw_ffn = hcs.on ? null : upload_region(addr < void? >(t.fblob[t.rms_ffn_off + l * dim]), uint64(dim * 4l))
                let lsl = layer_is_sliding(c, l)
                let het = hetero && lsl
                let hs_l = het ? hs_sw : head_size
                let kvd_l = het ? kvd_sw : kv_dim
                let qd_l = het ? qd_sw : qd
                //! the window's K/V panel takes the chunk rows at the first segment's position when it is the gathered panel too
                let koff = (mirk || seg_panels) ? 0ul : uint64(segs[0].start_pos * kvd_l * 4l)
                let scale_l = het ? (c.attn_scale > 0.0 ? c.attn_scale : 1.0 / sqrt(float(hs_sw))) : scale
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
                        pf_ple_side(enc, t, hcs, pls, u_dim, segs[0].start_pos, dn_pool_buffer(), segs[0].ple_ring_base,
                            segs[0].s.dn_captures, bcap, cap_bytes, (long_length(segs[0].s.dn_state) + long_length(segs[0].s.dn_conv_state)) * 4l)
                    }
                    pf_hc_mix_site(enc, t, hcs, l, false, bxb, hcs.binj_a)
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
                        pf_enc_site_mm(enc, t, fmt_at(t.dnqkv_fmt, l), t.dnqkv_offs[l], cd_dn, bxb, bdnqkv, u_dim, u_dn_cd, mp, dim, npos, bxh_x)
                        pf_enc_site_mm(enc, t, fmt_at(t.dngate_fmt, l), t.dngate_offs[l], di_dn, bxb, bdnz, u_dim, u_dn_di, mp, dim, npos, bxh_x)
                        if (t.dn_ba_f32) {
                            //! F32-on-disk β/α (35B): fblob slabs ride the router-b f32 GEMV
                            var bw_bt = upload_region(addr < void? >(t.fblob[t.dn_betaf_off + l * dim * nvh_dn]), uint64(dim * nvh_dn * 4l))
                            pf_enc_slab_gemv_b(enc, bw_bt, bxb, bdnb, dim, nvh_dn, npos)
                            var bw_al = upload_region(addr < void? >(t.fblob[t.dn_alphaf_off + l * dim * nvh_dn]), uint64(dim * nvh_dn * 4l))
                            pf_enc_slab_gemv_b(enc, bw_al, bxb, bdng, dim, nvh_dn, npos)
                        } else {
                            pf_enc_dn_ba_plane(enc, t, fmt_at(t.dnbeta_fmt, l), t.dnbeta_offs[l], bxb, bdnb, u_dim, u_dn_nvh, dim, nvh_dn, npos)
                            pf_enc_dn_ba_plane(enc, t, fmt_at(t.dnalpha_fmt, l), t.dnalpha_offs[l], bxb, bdng, u_dim, u_dn_nvh, dim, nvh_dn, npos)
                        }
                    }
                    if (g_pf_skip != "attn" && g_pf_skip != "dn" && g_pf_skip != "nongemm") {
                        let d = DnStages(qkv = bdnqkv, cv = bdncv, cvh = bdncvh, b = bdnb, g = bdng, o = bdno, z = bdnz,
                            grp = dn_tab.buf, capb = bcap, ng = nseg, npmax = dn_npmax, rows = npos)
                        enc_dn_front(enc, t, l, dnli, d)
                        if (bdncvh != null) {
                            //! the walk reads halves: the conv rows convert once, the gates prepare once
                            var baa = upload_region(addr < void? >(t.fblob[t.dn_a_off + l * nvh_dn]), uint64(nvh_dn * 4l))
                            var bdt = upload_region(addr < void? >(t.fblob[t.dn_dt_off + l * nvh_dn]), uint64(nvh_dn * 4l))
                            tw_half(enc, bdncv, bdncvh, npos * cd_dn)
                            enc_dn_prep(enc, bdnb, bdng, baa, bdt, dn_args(c, npos, false), (npos * nvh_dn + 63l) / 64l * 64l)
                        }
                        enc_dn_back(enc, t, l, dnli, d)
                    }
                    if (g_pf_skip != "gemm") {
                        pf_enc_site_mm(enc, t, fmt_at(t.dnout_fmt, l), t.dnout_offs[l], dim, bdno, bxb2, u_dn_di, u_dim, mp, di_dn, npos,
                            pf_cvt_panel(enc, bdno, mp, di_dn, g_pf_skip == "act_cvt"))
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
                        if (head_kv) {
                        } elif (c.q_gated) {
                            //! 2x-wide packed [q|gate] projection, deinterleaved into the q panel + gate rows
                            if (fq != KqFmt.q8) {
                                pf_enc_kq_site_mm(enc, t, fq, t.wq_offs[l], 2l * qd_l, bxb, bdnqg, u_dim, u_dn_qd2, mp, 0ul, bxh_x, dim, npos)
                            } else {
                                let bwqb = blob_of(g_dev, t, t.wq_offs[l])
                                enc_gemm_mm(enc, bwqb.buf, bwqb.boff, bxb, bdnqg, u_dim, u_dn_qd2, mp, 2l * qd_l, 0ul, dim, npos, bxh_x)
                            }
                            enc_dn_deint(enc, bdnqg, bq, bdngate,
                                DnDeintArgs(hs = uint(hs_l), qd = uint(qd_l), qs = uint(qd_l)), qd_l, npos)
                        } elif (fq != KqFmt.q8) {
                            pf_enc_kq_site_mm(enc, t, fq, t.wq_offs[l], qd_l, bxb, bq, u_dim, u_qd_l, mp, 0ul, bxh_x, dim, npos)
                        } else {
                            let bwqb = blob_of(g_dev, t, t.wq_offs[l])
                            enc_gemm_mm(enc, bwqb.buf, bwqb.boff, bxb, bq, u_dim, u_qd_l, mp, qd_l, 0ul, dim, npos, bxh_x)
                        }
                        if (!kvsh) {
                            if (fk != KqFmt.q8) {
                                pf_enc_kq_site_mm(enc, t, fk, t.wk_offs[l], kvd_l, bxb, bks[l], u_dim, u_kvd_l, mp, koff, bxh_x, dim, npos)
                            } else {
                                let bwkb = blob_of(g_dev, t, t.wk_offs[l])
                                enc_gemm_mm(enc, bwkb.buf, bwkb.boff, bxb, bks[l], u_dim, u_kvd_l, mp, kvd_l, koff, dim, npos, bxh_x)
                            }
                            if (has_wv) {
                                if (fv != KqFmt.q8) {
                                    pf_enc_kq_site_mm(enc, t, fv, t.wv_offs[l], kvd_l, bxb, bvs[l], u_dim, u_kvd_l, mp, koff, bxh_x, dim, npos)
                                } else {
                                    let bwvb = blob_of(g_dev, t, t.wv_offs[l])
                                    enc_gemm_mm(enc, bwvb.buf, bwvb.boff, bxb, bvs[l], u_dim, u_kvd_l, mp, kvd_l, koff, dim, npos, bxh_x)
                                }
                            }
                        }
                    } else {
                        let bwqb = blob_of(g_dev, t, t.wq_offs[l])
                        if (head_kv) {
                        } elif (c.q_gated) {
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
                //! rot mirrors enc_rope's @default: the null u_rot bind means "the class's own hs"
                let rot_l = c.rope_dim > 0l && c.rope_dim != head_size ? c.rope_dim : hs_l
                let qkr_on = (g_pf_env_qkr && g_pf_pso_qkr != null && g_pf_pso_qkr_hx != null
                    && hs_l % 2l == 0l && hs_l <= 512l && rot_l % 2l == 0l && rot_l >= 2l && rot_l <= hs_l)
                //! a v-norm layer's V rows ride the fused q/k pass (`MetalPfQknT`: V off the k row's registers where the layer has no v
                //! plane, else normed in place); otherwise the copy and the norm pass below
                let v_fused = qkr_on && c.v_norm && !recr && !kvsh && !has_bias
                if (!recr && !kvsh && !v_fused && (!has_wv || c.v_norm) && g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                    //! no-wv layers copy the PRE-norm/PRE-rope K rows into the V panel, then v_norm runs the
                    //! weightless per-head RMS in place - the CPU mm_qkv order, BEFORE qk_norm touches k
                    if (!has_wv) {
                        enc_copy_row(enc, bks[l], koff, bvs[l], koff, het ? u_vtot_sw : u_vtot, npos * kvd_l)
                    }
                    if (c.v_norm) {
                        //! MAX-width ones upload: the region cache is address-keyed and both classes read this one plane
                        var bones = upload_region(addr < void? >(t.fblob[t.rms_ones_off]), uint64(max_head_size(c) * 4l))
                        var vka = qk_norm_args(0u, uint(kvd_l / hs_l), uint(hs_l), uint(kvd_l), 0u, 0u, 0u, c.norm_eps, koff)
                        enc_qk_norm(enc, bvs[l], bvs[l], bones, vka, kvd_l / hs_l, npos)
                    }
                }
                if (!recr && has_bias && g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                    //! QKV bias before rope, chunk rows only (existing rows are stored biased+roped); the count
                    //! uniforms stay base-class - no hetero model carries QKV bias
                    var bbq = upload_region(addr < void? >(t.bq[l * qd_l]), uint64(qd_l * 4l))
                    var bbk = upload_region(addr < void? >(t.bk[l * kvd_l]), uint64(kvd_l * 4l))
                    var bbv = upload_region(addr < void? >(t.bv[l * kvd_l]), uint64(kvd_l * 4l))
                    tw_bias(enc, bq, bbq, GkBiasActArgs(d = uint(qd), nelem = uint(npos * qd), boff = 0u, act = BIAS_ACT_NONE))
                    tw_bias(enc, bks[l], bbk, GkBiasActArgs(d = uint(kv_dim), nelem = uint(npos * kv_dim), boff = 0u, act = BIAS_ACT_NONE, xoff = uint(koff / 4ul)))
                    tw_bias(enc, bvs[l], bbv, GkBiasActArgs(d = uint(kv_dim), nelem = uint(npos * kv_dim), boff = 0u, act = BIAS_ACT_NONE, xoff = uint(koff / 4ul)))
                }
                var bqh_q : MetalBuffer? = null    //! the fused arm's q twin, claimed at rope time - the QK site consumes it
                var bqh_q_live = false
                if (!recr && qkr_on && g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                    var bcos_l = lsl && dual_rope ? bcos_swa : bcos
                    var bsin_l = lsl && dual_rope ? bsin_swa : bsin
                    //! a model with no q/k norm never reads the weights - bcos_l just keeps the binding valid
                    var bqkw = has_qkn ? upload_region_cat(addr < void? >(t.fblob[t.rms_q_offs[l]]), uint64(hs_l * 4l),
                        addr < void? >(t.fblob[t.rms_k_offs[l]]), uint64(hs_l * 4l)) : bcos_l
                    bqh_q = (mirk || mp >= CVT_MIN_ROWS) ? pf_twin_panel(mp, qd_l) : null
                    bqh_q_live = true
                    let twin = !head_kv && bqh_q != null && g_pf_skip != "act_cvt"
                    pf_enc_qkn(enc, bq, kvsh ? bq : bks[l], kvsh ? bq : bvs[l], koff, bcos_l, bsin_l, bqkw, twin ? bqh_q : null, head_kv ? 0l : n_heads,
                        kvsh ? 0l : kvd_l / hs_l, qd_l, kvd_l, hs_l, rot_l, c.rope_neox, has_qkn, v_fused && !has_wv, v_fused && has_wv, c.norm_eps,
                        mirk ? scale_l : 1.0, npos, twin ? mp : npos)
                }
                if (!recr && !qkr_on && has_qkn && g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                    //! per-head QK-norm before rope — the chunk's q panel and its new K rows
                    var bqw = upload_region(addr < void? >(t.fblob[t.rms_q_offs[l]]), uint64(hs_l * 4l))
                    var qka = qk_norm_args(uint(n_heads), 0u, uint(hs_l), uint(qd_l), 0u, 0u, 0u, c.norm_eps)
                    if (!head_kv) {
                        enc_qk_norm(enc, bq, bq, bqw, qka, n_heads, npos)
                    }
                    if (!kvsh) {
                        var bkw = upload_region(addr < void? >(t.fblob[t.rms_k_offs[l]]), uint64(hs_l * 4l))
                        var kka = qk_norm_args(0u, uint(kvd_l / hs_l), uint(hs_l), uint(kvd_l), 0u, 0u, 0u, c.norm_eps, koff)
                        enc_qk_norm(enc, bks[l], bks[l], bkw, kka, kvd_l / hs_l, npos)
                    }
                }
                if (!recr && !qkr_on && g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                    //! sliding layers rope with the swa row pair, whose rows are the class's own hs/2 wide
                    var bcos_l = lsl && dual_rope ? bcos_swa : bcos
                    var bsin_l = lsl && dual_rope ? bsin_swa : bsin
                    if (!head_kv) {
                        enc_rope(enc, bq, 0ul, bcos_l, bsin_l, u_qd_l, u_hs_l, u_npairs_q_l, u_neox, u_rot, null, npos * qd_l / 2l)
                    }
                    if (!kvsh) {
                        enc_rope(enc, bks[l], koff, bcos_l, bsin_l, u_kvd_l, u_hs_l, u_npairs_k_l, u_neox, u_rot, null, npos * kvd_l / 2l)
                    }
                }
                if (seg_panels && !recr && !kvsh) {
                    for (sg, i in segs, count()) {
                        let at = int64(i) * nl_kv + l
                        enc_copy_row(enc, bks[l], uint64(sg.row0 * kvd_l * 4l), sbks[at], uint64(sg.start_pos * kvd_l * 4l), het ? sg.u_kv_sw : sg.u_kv, sg.npos * kvd_l)
                        enc_copy_row(enc, bvs[l], uint64(sg.row0 * kvd_l * 4l), sbvs[at], uint64(sg.start_pos * kvd_l * 4l), het ? sg.u_kv_sw : sg.u_kv, sg.npos * kvd_l)
                    }
                }
                if ((mirk || mirw) && !recr && !kvsh) {
                    for (sg, i in segs, count()) {
                        let lb = uint64(mirror_lbase(t, kvdt, sg.mir_cap, l))
                        let mir_row0 = mirc ? sg.start_pos : 0l
                        let pan = pf_seg_panels(seg_panels, int64(i), nl_kv, l, sbks, sbvs, bks, bvs)
                        let src_row = kvdt == KVDtype.f16 && mirk ? sg.row0 : mir_row0   //! the window panel's rows at the segment's, the gathered panel's at their positions
                        mirror_ring_pieces(t, l, mir_row0, sg.nk) $(p0, n, slot : int64) {
                            let rows_off = uint64(slot * kv_row_bytes(kvdt, kvd_l))
                            let src = src_row + (p0 - mir_row0)
                            if (kvdt == KVDtype.f16) {
                                enc_kv_twin(enc, pan.k, pan.v, g_arena, g_arena, 1.0, n * kvd_l,
                                    (sg.mir_koff + lb + rows_off) / 2ul, (sg.mir_voff + lb + rows_off) / 2ul, uint64(src * kvd_l))
                            } else {
                                pf_enc_kv_feed(enc, kvdt, pan.k, pan.v, src, n, kvd_l, hs_l, sg.mir_koff + lb + rows_off, sg.mir_voff + lb + rows_off,
                                    n == sg.npos ? (het ? sg.u_kv_sw : sg.u_kv) : uniform_u32(uint(n * kvd_l)))
                            }
                        }
                    }
                }
                if (head_kv) {
                    continue
                }
                if (!recr && g_pf_skip != "attn" && g_pf_skip != "nongemm") {
                    var bqh_l : MetalBuffer?
                    if (bqh_q_live) {
                        bqh_l = bqh_q
                    } elif (mirk) {   //! the unfused rope left no twin: Q's halves carry the softmax scale
                        bqh_l = pf_twin_panel(mp_q, qd_l)
                        tw_half(enc, bq, bqh_l, mp_q * qd_l, scale_l)
                    } elif (mmattn) {
                        bqh_l = pf_cvt_panel(enc, bq, mp_q, qd_l, g_pf_skip == "act_cvt")
                    }
                    let dev = attn_dev && bqh_l != null
                    var bsnk : MetalBuffer?
                    if (c.attn_sinks && g_pf_skip != "attn_sm") {
                        bsnk = upload_region(unsafe(addr < void? >(t.fblob[t.sinks_off + l * n_heads])), uint64(n_heads * 4l))
                    }
                    for (sg, i in segs, count()) {
                        var atka = AttnArgs(qd = uint(qd_l), kv_dim = uint(kvd_l), head_size = uint(hs_l),
                            kv_mul = het ? uint(n_heads / kvh_sw) : kv_mul, npos = uint(sg.nk),
                            np32 = uint(sg.nk64),
                            scale = scale_l,
                            qoff = uint(sg.start_pos), qrows = uint(sg.mp),
                            window = uint(lsl && c.sliding_window > 0l ? c.sliding_window : 0l),
                            softcap = c.attn_logit_softcap, hass = c.attn_sinks ? 1u : 0u,
                            uend = sg.s.attn_uniform_end > 0l ? uint(sg.s.attn_uniform_end) : 0u,
                            ulo = sg.s.attn_uniform_end > 0l ? uint(sg.s.attn_uniform_lo) : 0u,
                            ring = mirk ? uint(mirror_ring(t, l)) : 0u)
                        let pan = pf_seg_panels(seg_panels, int64(i), nl_kv, l, sbks, sbvs, bks, bvs)
                        let kpan = pan.k
                        let vpan = pan.v
                        let lb = mirk ? uint64(mirror_lbase(t, KVDtype.f16, sg.mir_cap, l)) : 0ul
                        let mir_kb = mirk ? sg.mir_koff + lb : 0ul
                        let mir_vb = mirk ? sg.mir_voff + lb : 0ul
                        let qboff = uint64(sg.row0 * qd_l * 4l)
                        let qhoff = uint64(sg.row0 * qd_l * 2l)
                        let xboff = uint64(sg.row0 * qd_l * 4l)
                        if (dev && !mirk) {
                            enc_kv_twin(enc, kpan, vpan, bkh, bvh, atka.scale, sg.nk64 * kvd_l)
                        }
                        if (g_pf_skip != "attn_qk") {
                            if (dev) {
                                enc_qk_dev(enc, bqh_l, mirk ? g_arena : bkh, batt, atka, sg.mp, sg.nk64, n_heads, mir_kb, qhoff)
                            } elif (mmattn) {
                                enc_qk_mm(enc, bq, kpan, batt, atka, sg.mp, sg.nk64, n_heads, bqh = bqh_l, qboff = qboff, qhoff = qhoff)
                            } else {
                                enc_qk(enc, bq, qboff, kpan, batt, atka, sg.mp, sg.nk64, n_heads)
                            }
                        }
                        if (g_pf_skip != "attn_sm") {
                            if (dev) {
                                enc_attn_pexp(enc, batt, atka, bstats, bsnk, sg.npos, n_heads)
                            } else {
                                enc_rowstat(enc, batt, atka, bstats, bsnk, sg.npos, n_heads)
                            }
                        }
                        if (g_pf_skip != "attn_av") {
                            if (dev) {
                                enc_av_dev(enc, batt, mirk ? g_arena : bvh, bxb, atka, bstats, sg.mp, n_heads, hs_l, mir_vb, xboff)
                            } elif (mmattn) {
                                enc_av_mm(enc, batt, vpan, bxb, atka, bstats, sg.mp, n_heads, hs_l, xboff)
                            } else {
                                enc_av(enc, batt, vpan, bxb, xboff, atka, bstats, sg.mp, n_heads, hs_l)
                            }
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
                        pf_enc_kq_site_mm(enc, t, fo, t.wo_offs[l], dim, bxb, bxb2, u_qd_l, u_dim, mp, 0ul, bxh_av, qd_l, npos)
                    } else {
                        let bwob = blob_of(g_dev, t, t.wo_offs[l])
                        enc_gemm_mm(enc, bwob.buf, bwob.boff, bxb, bxb2, u_qd_l, u_dim, mp, dim, 0ul, qd_l, npos, bxh_av)
                    }
                    if (c.attn_out_bias) {
                        var bwo_b = upload_region(unsafe(addr < void? >(t.bo[l * dim])), uint64(dim * 4l))
                        tw_bias(enc, bxb2, bwo_b, GkBiasActArgs(d = uint(dim), nelem = uint(npos * dim), boff = 0u, act = BIAS_ACT_NONE))
                    }
                }
                let hid_l = layer_hidden(t, l)
                let u_hid_l = (u_hidden2 == null || hid_l == hidden) ? u_hidden : u_hidden2
                let u_th_l = (u_total_h2 == null || hid_l == hidden) ? u_total_h : u_total_h2
                let narrow_last = (g_pf_env_lastrow && !s.keep_hidden && !warm
                    && l == c.n_layers - 1l && npos > 1l && !recr && !moe && !hcs.on
                    && l >= nds && !c.q_gated
                    && c.n_layer_nextn == 0l
                    && dim % 32l == 0l && hid_l % 32l == 0l
                    && kq_fmt_one_row_gemv(fmt_at(t.w1_fmt, l), dim) && kq_fmt_one_row_gemv(fmt_at(t.w3_fmt, l), dim)
                    && kq_fmt_one_row_gemv(fmt_at(t.w2_fmt, l), hid_l)
                    && (!plef || (pf_narrow_ple_ok(t, l, false) && pf_narrow_ple_ok(t, l, true))))
                //! the gemma-4 MoE tail as one step (`MetalMoeCombG4Pf`): no seam between the FFN and the residual, a row the step's stash holds
                let g4_step = (moe && c.moe_dense_shexp && !g_moe_g4_unfused && c.pre_post_norm && !plef && !hcs.on && l >= nds
                    && dim <= ADD_RMS_MAX_DIM && g_pf_skip == "")
                if (narrow_last) {
                    for (sg in segs) {
                        let nro = uint64((sg.row0 + sg.npos - 1l) * dim * 4l)
                        if (g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                            if (c.pre_post_norm) {   //! the sandwich: the attention branch's row under its post norm before the add
                                var bw_pa = upload_region(unsafe(addr < void? >(t.fblob[t.rms_post_att_off + l * dim])), uint64(dim * 4l))
                                pf_enc_rms(enc, bxb2, nro, bw_pa, bxb, u_dim, u_eps, 1l)
                                enc_add(enc, bx, nro, bxb, 0ul, u_dim, dim)
                            } else {
                                enc_add(enc, bx, nro, bxb2, nro, u_dim, dim)
                            }
                            pf_enc_rms(enc, bx, nro, bw_ffn, bxb, u_dim, u_eps, 1l)
                        }
                        if (g_pf_skip != "gemm") {
                            pf_enc_narrow_gemv(enc, t, fmt_at(t.w1_fmt, l), t.w1_offs[l], bxb, bhb, u_dim, u_hid_l, dim, hid_l)
                            pf_enc_narrow_gemv(enc, t, fmt_at(t.w3_fmt, l), t.w3_offs[l], bxb, bhb2, u_dim, u_hid_l, dim, hid_l)
                        }
                        if (g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                            if (c.ffn_act != FfnAct.silu) {
                                enc_geglu(enc, bhb, 0ul, bhb2, 0ul, u_hid_l, hid_l)
                            } else {
                                enc_swiglu(enc, bhb, 0ul, bhb2, 0ul, u_hid_l, hid_l)
                            }
                        }
                        if (g_pf_skip != "gemm") {
                            pf_enc_narrow_gemv(enc, t, fmt_at(t.w2_fmt, l), t.w2_offs[l], bhb, bxb, u_hid_l, u_dim, hid_l, dim)
                        }
                        var bosc : MetalBuffer?
                        if (c.layer_out_scale) {
                            bosc = upload_region(addr < void? >(t.fblob[t.out_scale_off + l]), 4ul)
                        }
                        if (g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                            if (c.pre_post_norm) {   //! ... and the FFN branch's under its own
                                var bw_pf = upload_region(unsafe(addr < void? >(t.fblob[t.rms_post_ffn_off + l * dim])), uint64(dim * 4l))
                                pf_enc_rms(enc, bxb, 0ul, bw_pf, bxb, u_dim, u_eps, 1l)
                            }
                            enc_add(enc, bx, nro, bxb, 0ul, u_dim, dim, plef ? null : bosc)   //! the per-layer embedding's branch takes the scale
                        }
                        if (plef) {   //! the per-layer embedding's branch over the final row: its side row the layer's at the row's position
                            let side_off = uint64(((sg.row0 + sg.npos - 1l) * ple_all + l * ple_n) * 4l)
                            if (g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                                enc_copy_row(enc, bx, nro, bxb2, 0ul, u_dim, dim)
                            }
                            if (g_pf_skip != "gemm") {
                                pf_enc_narrow_ple(enc, t, l, false, bxb2, bpleg, u_dim, u_ple)
                            }
                            if (g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                                enc_geglu(enc, bpleg, 0ul, bple, side_off, u_ple, ple_n)
                            }
                            if (g_pf_skip != "gemm") {
                                pf_enc_narrow_ple(enc, t, l, true, bpleg, bxb, u_ple, u_dim)
                            }
                            if (g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                                var bw_pn = upload_region(unsafe(addr < void? >(t.fblob[t.ple_post_off + l * dim])), uint64(dim * 4l))
                                pf_enc_rms(enc, bxb, 0ul, bw_pn, bxb, u_dim, u_eps, 1l)
                                enc_add(enc, bx, nro, bxb, 0ul, u_dim, dim, bosc)
                            }
                        }
                    }
                }
                var bxh_ffn : MetalBuffer?
                if (hcs.on) {   //! the attention output scatters into the streams; the ffn site's mixer makes the block input rows
                    pf_hc_combine(enc, t, hcs, bxb2, hcs.binj_a)
                    pf_hc_mix_site(enc, t, hcs, l, true, bxb, hcs.binj_f)
                } elif (!narrow_last && g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                    //! the attention branch under its post norm (the sandwich), the add and the FFN norm with its f16 twin - a dense FFN
                    //! reads the rows, the layer's own or a routed layer's shared expert, and so does the router's GEMM
                    bxh_ffn = pf_twin_panel(mp, dim)
                    enc_residual(enc, t, bx, bxb2, bxb, bxh_ffn, seam(c.pre_post_norm ? t.rms_post_att_off + l * dim : -1l, t.rms_ffn_off + l * dim),
                        npos, ResidualForm.window, false, ResidualUniforms(bdim = u_dim, beps = u_eps, btot = u_total_dim))
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
                    if (c.moe_router_bias) {
                        brb = upload_region(unsafe(addr < void? >(t.fblob[t.router_b_off + l * c.n_expert])), uint64(c.n_expert * 4l))
                    }
                    var bw_ds : MetalBuffer?
                    if (g4moe) {
                        bw_ds = upload_region(unsafe(addr < void? >(t.fblob[t.moe_down_scale_off + l * c.n_expert])), uint64(c.n_expert * 4l))
                    }
                    pf_enc_moe_route(enc, t, l, g4moe ? bmg4rt : bxb, bmlg, bmsel, bmcnt, bmbase, bmbkt, bminv,
                        u_dim, u_moe_ne, u_moe_nk, brb, u_moe_tot, npos,
                        mp = mp, bucket = !moe_gemv_route, bds = bw_ds)
                    if (nsh > 0l) {
                        //! shexp raw gate dots (σ in the combine) off the router region's last row — bmlg is free once select consumed it
                        var gka = moe_router_args(1l, dim, npos)
                        gka.woff = uint(c.n_expert * dim)
                        enc_moe_router_b(enc, pf_moe_router_rows(t, l), bxb, bmlg, g_one, gka, 1l, npos)
                    }
                    if (!pf_skip_moe_mm()) {
                        if (moe_gemv_route) {
                            //! the gathered route runs whole at the reduce's seat below, after the shared expert read bxb
                        } elif (t.experts_mx4) {
                            //! a model's per-expert bias folds into the mm accumulator seed (hasb); the moe_bias arm knocks it out
                            let hasb = c.moe_exps_bias && g_pf_skip != "moe_bias"
                            var bwb1 = mx4_bias_rows(t, t.web1_off, l, c.n_ff_exp)
                            var bwb3 = mx4_bias_rows(t, t.web3_off, l, c.n_ff_exp)
                            var kaup = moe_mm_mx4_ka(dim, c.n_ff_exp, c.n_expert_used, true, hasb)
                            let bxg1 = pf_moe_gather_panel(enc, bmbkt, g4moe ? bxb2 : bxb, bmxg,
                                g_pf_pso_moe_mm_mx4_th != null, dim, c.n_expert_used, c.n_expert, mpad, npos)
                            pf_enc_moe_mm_mx4(enc, t, t.we1_offs[l], c.n_ff_exp, npos, g4moe ? bxb2 : bxb, bmg,
                                bmcnt, bmbase, bmbkt, bwb1, kaup, contiguous = bxg1 != null, bxh = bxg1)
                            pf_enc_moe_mm_mx4(enc, t, t.we3_offs[l], c.n_ff_exp, npos, g4moe ? bxb2 : bxb, bmg2,
                                bmcnt, bmbase, bmbkt, bwb3, kaup, contiguous = bxg1 != null, bxh = bxg1)
                        } else {
                            let gx_q8 = pf_moe_twin_live(fe1, dim, npos, c.n_expert_used, c.n_expert) || pf_moe_twin_live(fe3, dim, npos, c.n_expert_used, c.n_expert)
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
                            //! shexp gate/up: each plane's site by its tag (bhb/bhb2 sized max(hidden, nsh))
                            let bxh_sh = pf_hx_panel(enc, bxh_ffn, bxb, mp, dim, g_pf_skip == "act_cvt")
                            let p1 = shexp_plane(t, l, 1)
                            let p3 = shexp_plane(t, l, 3)
                            pf_enc_site_mm(enc, t, p1.f, p1.off, nsh, bxb, bhb, u_dim, u_moe_nsh, mp, dim, npos, bxh_sh)
                            pf_enc_site_mm(enc, t, p3.f, p3.off, nsh, bxb, bhb2, u_dim, u_moe_nsh, mp, dim, npos, bxh_sh)
                        }
                    }
                }
                // the gpt-oss activation has no HX stamp, so it keeps the convert pass
                let down_th = moe && (t.experts_mx4 ? g_pf_pso_moe_mm_mx4_th != null : pf_moe_twin_live(fmt_at(t.we2_fmt, l), c.n_ff_exp, npos, c.n_expert_used, c.n_expert))
                let act_hx = down_th && !moe_gemv_route && c.ffn_act != FfnAct.swiglu_oai && mpad >= CVT_MIN_ROWS && g_pf_skip != "moe_cvt" && g_pf_skip != "act_cvt"
                var bxh_mg = act_hx ? pf_twin_panel(mpad, c.n_ff_exp) : null
                var bxh_shh : MetalBuffer?   //!< the shared expert's hidden rows as halves, where its activation wrote them
                //! the bucket panel's activation reads padded rows bounded a bucket at a time, a shape the live-row ladder
                //! (`enc_moe_act`) does not take, so its stamps stay their own
                if (moe && g_pf_skip != "ew" && g_pf_skip != "nongemm" && g_pf_skip != "moe" && !pf_skip_moe_act()) {
                    if (moe_gemv_route) {
                        //! the gathered route's activation runs inside its block over the live rows
                    } elif (c.ffn_act == FfnAct.swiglu_oai) {
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
                        if (moe_gemv_route) {
                            //! the gathered route's down site runs inside its block below
                        } elif (down_th && bxh_mg == null) {
                            // the bucket panel is PADDED - a convert must cover mpad rows, not the mtot prefix
                            bxh_mg = pf_cvt_panel(enc, bmg, mpad, c.n_ff_exp, g_pf_skip == "moe_cvt" || g_pf_skip == "act_cvt")
                        }
                        if (moe_gemv_route) {
                            // the route block below
                        } elif (t.experts_mx4) {
                            pf_enc_moe_mm_mx4(enc, t, t.we2_offs[l], dim, npos, bmg, bmdn,
                                bmcnt, bmbase, bmbkt, mx4_bias_rows(t, t.web2_off, l, dim),
                                moe_mm_mx4_ka(c.n_ff_exp, dim, c.n_expert_used, false, c.moe_exps_bias && g_pf_skip != "moe_bias"),
                                contiguous = true, bxh = bxh_mg)
                        } else {
                            pf_enc_moe_mm(enc, t, fe2, t.we2_offs[l], dim, npos, bmg, bmdn,
                                bmcnt, bmbase, bmbkt,
                                moe_mm_ka(fe2, c.n_ff_exp, dim, c.n_expert_used, false), contiguous = true, bxh = bxh_mg,
                                bkd = u_moe_nfe, bnd = u_dim, mpad = mpad)
                        }
                        if (nsh > 0l) {
                            //! shexp down into bxb2 (free post-residual) — bxb still feeds the reduce
                            let p2 = shexp_plane(t, l, 2)
                            pf_enc_site_mm(enc, t, p2.f, p2.off, dim, bhb, bxb2, u_moe_nsh, u_dim, mp, nsh, npos, pf_hx_panel(enc, bxh_shh, bhb, mp, nsh, g_pf_skip == "act_cvt"))
                        }
                    }
                    if (moe_gemv_route) {
                        //! after the shared expert's GEMMs read bxb: the route's combine lands the routed sum there
                        if (!pf_skip_moe_mm()) {
                            let sh = nsh > 0l && !g4moe
                            pf_enc_moe_gemv_block(enc, t, l, g4moe ? bxb2 : bxb, bmg, bmg2, bmdn, bmsel, g4moe ? bmg4rt : bxb, npos, u_moe_act_tot,
                                sh ? bxb2 : null, sh ? bmlg : null)
                        }
                    } elif (g4_step) {
                        //! the gemma-4 step below sums the routed rows itself
                    } elif (!pf_skip_moe_reduce()) {
                        if (nsh > 0l && !g4moe) {   //! the routed sum and the shared expert's rows under their gate's sigmoid, one pass
                            pf_enc_moe_reduce_sh(enc, bxb, bminv, bmsel, bmdn, bxb2, bmlg, moe_comb_args(dim, c.n_expert_used), dim, npos)
                        } else {
                            pf_enc_moe_reduce(enc, g4moe ? bmg4rt : bxb, bminv, bmsel, bmdn, moe_comb_args(dim, c.n_expert_used), dim, npos)
                        }
                    }
                    if (g4moe && !g4_step && g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                        var bw_pf2 = upload_region(unsafe(addr < void? >(t.fblob[t.rms_post_ffn2_off + l * dim])), uint64(dim * 4l))
                        pf_enc_rms(enc, bmg4rt, 0ul, bw_pf2, bmg4rt, u_dim, u_eps, npos)
                    }
                }
                if (!narrow_last && (!moe || g4moe) && g_pf_skip != "gemm") {   //! g4: the dense parallel shared branch
                    bxh_ffn = pf_hx_panel(enc, bxh_ffn, bxb, mp, dim, g_pf_skip == "act_cvt")
                    pf_enc_site_mm(enc, t, fmt_at(t.w1_fmt, l), t.w1_offs[l], hid_l, bxb, bhb, u_dim, u_hid_l, mp, dim, npos, bxh_ffn)
                    pf_enc_site_mm(enc, t, fmt_at(t.w3_fmt, l), t.w3_offs[l], hid_l, bxb, bhb2, u_dim, u_hid_l, mp, dim, npos, bxh_ffn)
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
                    pf_enc_site_mm(enc, t, fmt_at(t.w2_fmt, l), t.w2_offs[l], dim, bhb, bxb, u_hid_l, u_dim, mp, hid_l, npos, bxh_h)
                }
                if (g4_step) {   //! the routed sum, both branches' norms and the residual: one step; the next layer opens on its own norm
                    var ka = moe_g4_layer_args(t, l, moe_gemv_route)
                    enc_moe_comb_g4_pf(enc, bx, bminv, bmsel, moe_gemv_route ? bmg4rt : bmdn, bxb, moe_g4_norms(t, l, false), ka, npos)
                } elif (g4moe && g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                    //! dense-branch post-norm, then routed + dense summed into bxb for the epilogue
                    var bw_pf1 = upload_region(unsafe(addr < void? >(t.fblob[t.rms_post_ffn1_off + l * dim])), uint64(dim * 4l))
                    pf_enc_rms(enc, bxb, 0ul, bw_pf1, bxb, u_dim, u_eps, npos)
                    enc_add(enc, bxb, 0ul, bmg4rt, 0ul, u_total_dim, npos * dim)
                }
                let pf_ew = g_pf_skip != "ew" && g_pf_skip != "nongemm"
                let pf_uni = ResidualUniforms(bdim = u_dim, beps = u_eps, btot = u_total_dim)
                let post_ffn = c.pre_post_norm ? t.rms_post_ffn_off + l * dim : -1l   //! the FFN branch's post norm (the sandwich)
                if (hcs.on && post_ffn >= 0l && !g4_step && !narrow_last && pf_ew) {
                    var bw_pf = upload_region(addr < void? >(t.fblob[post_ffn]), uint64(dim * 4l))
                    pf_enc_rms(enc, bxb, 0ul, bw_pf, bxb, u_dim, u_eps, npos)
                }
                if (plef && !narrow_last) {
                    //! PLE sits between the FFN residual and out_scale (the CPU order): x_b += post_norm(ffn_branch), then
                    //! branch = proj · (gelu(gate · x_b) ⊙ side[l]), its post norm the layer's last seam's
                    if (pf_ew) {
                        enc_residual(enc, t, bx, bxb, null, null, seam(post_ffn, -1l), npos, ResidualForm.window, false, pf_uni)
                    }
                    if (g_pf_skip != "gemm") {
                        pf_enc_ple_site(enc, t, l, false, bx, bpleg, u_dim, u_ple, mp, npos, pf_cvt_panel(enc, bx, mp, dim, g_pf_skip == "act_cvt"))
                    }
                    if (g_pf_skip != "ew" && g_pf_skip != "nongemm") {
                        var ka = GkPleActArgs(nelem = uint(npos * ple_n), sbase = uint(l * ple_n), sstride = uint(ple_all), slen = uint(ple_n))
                        enc_geglu_side(enc, bpleg, bple, ka, npos * ple_n)
                    }
                    if (g_pf_skip != "gemm") {
                        pf_enc_ple_site(enc, t, l, true, bpleg, bxb, u_ple, u_dim, mp, npos, pf_cvt_panel(enc, bpleg, mp, ple_n, g_pf_skip == "act_cvt"))
                    }
                }
                if (hcs.on) {
                    pf_hc_combine(enc, t, hcs, bxb, hcs.binj_f)
                } elif (!g4_step && !narrow_last && pf_ew) {
                    //! the layer's last seam: the branch under its post norm, the add under the layer's output scale (gemma-4); the
                    //! next layer opens on its own norm
                    let pre = plef ? t.ple_post_off + l * dim : post_ffn
                    enc_residual(enc, t, bx, bxb, null, null, seam(pre, -1l, c.layer_out_scale ? t.out_scale_off + l : -1l), npos, ResidualForm.window, false, pf_uni)
                    if (l < nds) {   //! deepstack: slice l adds to the layer output (every row; text tails are zero)
                        enc_add(enc, bx, 0ul, bds, uint64(l * npos * dim * 4l), u_total_dim, npos * dim)
                    }
                }
            }
        }
        if (gpu_logits) {
            //! the caller samples from s.logits, so this closes the whole forward inside the GPU window
            unsafe {
                for (sg, i in segs, count()) {
                    let xf_off = uint64(int64(i) * dim * 4l)
                    let lg_off = uint64(int64(i) * c.vocab_size * 4l)
                    var bxf_i = nseg > 1l ? bxf1 : bxf
                    var blog_i = nseg > 1l ? blog1 : blog
                    if (hcs.on) {   //! the head mixer stands where the final norm would - over the TRUNK's last wide row (the warm swapped the draft's in)
                        var hcs_t = hcs
                        if (warm) {
                            hcs_t.bhc = bwm_hc
                        }
                        pf_hc_head(enc, t, hcs_t, bxf_i)
                    } else {
                        var bwfin = upload_region(addr < void? >(t.fblob[t.rms_final_off]), uint64(dim * 4l))
                        pf_enc_rms(enc, warm ? bwm_res : bx, uint64((sg.row0 + sg.npos - 1l) * dim * 4l), bwfin, bxf_i, u_dim, u_eps, 1l)
                    }
                    if (cls_fmt != KqFmt.q8) {
                        enc_site_gemv(enc, t, cls_fmt, t.wcls_off, c.vocab_size, dim, bxf_i, blog_i, u_dim, u_vocab)
                    } else {
                        let bwc = blob_of(g_dev, t, t.wcls_off)
                        enc_gemv(enc, bwc.buf, bwc.boff, bxf_i, blog_i, u_dim, u_vocab, c.vocab_size)
                    }
                    if (c.final_logit_softcap > 0.0 || bsupp != null) {   //! the final softcap, then the suppressed ids (the CPU order)
                        enc_cls_epi_rows(enc, blog_i, bsupp, c.vocab_size, 1l, c.final_logit_softcap, nsupp)
                    }
                    if (nseg > 1l) {
                        enc_copy_row(enc, bxf1, 0ul, bxf, xf_off, u_dim, dim)
                        enc_copy_row(enc, blog1, 0ul, blog, lg_off, u_vocab, c.vocab_size)
                    }
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
        for (sg in segs) {   //! each session's physical runs for its chunk, once; its scratch holds them through the store
            sg.store_nrun = kv_runs(*sg.s, sg.start_pos, sg.start_pos + sg.npos, sg.s.kv_runs)
        }
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
                    for (sg, si in segs, count()) {
                        var ss & = *sg.s
                        let runsp = addr < KVRun const? >(ss.kv_runs[0])
                        if (mirk || mirw) {   //! the mirror's codec bytes are the rows: the host store takes them byte for byte
                            let rowb = kv_row_bytes(kvdt, layer_kv_dim(c, l))
                            let lb = mirror_lbase(t, kvdt, sg.mir_cap, l)
                            let ab = reinterpret<uint8?>(metal_buffer_contents(g_arena))
                            var kdst = kv_layer_kbase(ss, l, c.seq_len)
                            var vdst = kv_layer_vbase(ss, l, c.seq_len)
                            for (r in range64(sg.store_nrun)) {
                                let row0 = runsp[r].row0 - runsp[r].j0   //! the run's cache row of position p is `row0 + p`
                                mirror_ring_pieces(t, l, runsp[r].j0, runsp[r].j1) $(p0, n, slot : int64) {
                                    memcpy(reinterpret<void?>(kdst + (row0 + p0) * rowb), reinterpret<void?>(ab + sg.mir_koff + uint64(lb + slot * rowb)), n * rowb)
                                    memcpy(reinterpret<void?>(vdst + (row0 + p0) * rowb), reinterpret<void?>(ab + sg.mir_voff + uint64(lb + slot * rowb)), n * rowb)
                                }
                            }
                            continue
                        }
                        //! the chunk's rows sit past a continuation's gathered panel head - the window's panel or the segment's own
                        let pan = pf_seg_panels(seg_panels, int64(si), nl_kv, l, sbks, sbvs, bks, bvs)
                        var kpan = reinterpret<uint8?>(metal_buffer_contents(pan.k))
                        var vpan = reinterpret<uint8?>(metal_buffer_contents(pan.v))
                        let kvd_l = layer_kv_dim(c, l)
                        let hs_l = layer_head_size(c, l)
                        let koff_l = sg.start_pos * kvd_l * 4l
                        memcpy(addr < void? >(ss.k_b[0]), reinterpret<void?>(kpan + koff_l), sg.npos * kvd_l * 4l)
                        memcpy(addr < void? >(ss.v_b[0]), reinterpret<void?>(vpan + koff_l), sg.npos * kvd_l * 4l)
                        //! tq4 sessions store in the ROTATED basis: the GPU ran attention unrotated, so rotate the
                        //! readback rows per head before the codec store
                        tq4_rotate_for_store(ss, ss.k_b, ss.v_b, sg.npos, kvd_l, hs_l)
                        kv_store_batch(kv_layer_kbase(ss, l, c.seq_len), kv_layer_vbase(ss, l, c.seq_len),
                            ss.kv_dtype_k, ss.kv_dtype_v, ss.k_b, ss.v_b, runsp, sg.store_nrun, sg.start_pos, sg.npos, kvd_l)
                    }
                }
                readback_us += int64(get_time_usec(ts_rb))
            }
        }
    }
    cbs |> clear()
    gpu_ms = (gpu_t1 - gpu_t0) * 1000.0lf   //! whole-GPU window: first chunk start -> last chunk end
    let encwait_us = get_time_usec(ts_enc)
    let ok = ran
    if (mirk || mirw) {
        for (sg in segs) {
            mirror_set_watermark(sg.s.uid, ran ? sg.start_pos + sg.npos : (mirc ? sg.start_pos : 0l))
        }
        g_pf_mirror_served += ran && mirk ? nseg : 0l
        g_pf_mirror_panel_served += ran && mirw ? nseg : 0l
    }
    if (ran) {
        if (dn) {   //! the window completed — each segment's GPU state now holds its last position's update
            for (sg in segs) {
                dn_mirror_advance(sg.s.uid, sg.start_pos + sg.npos - 1l)
            }
            if (bcap != null) {
                dn_captures_from_arena(segs, bcap, cap_bytes)
            }
        }
        let ts_rb = ref_time_ticks()
        for (sg, i in segs, count()) {
            var ss & = unsafe(*sg.s)
            unsafe {
                memcpy(addr < void? >(ss.x_b[0]), contents_at(warm ? bwm_res : bx, uint64(sg.row0 * dim * 4l)), sg.npos * dim * 4l)
                if (gpu_logits) {
                    memcpy(addr < void? >(ss.logits[0]), contents_at(blog, uint64(int64(i) * c.vocab_size * 4l)), c.vocab_size * 4l)
                }
            }
            if (gpu_logits && c.n_layer_nextn > 0l) {
                //! forward_prefill's mtp_h contract: bxf holds each last row's post-final-norm hidden, a hyper-connection model the trunk's last WIDE row
                unsafe {
                    if (hcs.on) {
                        let wrows = reinterpret<uint8?>(metal_buffer_contents(warm ? bwm_hc : hcs.bhc))
                        stash_mtp_carry(ss, reinterpret<void?>(wrows + uint64((sg.row0 + sg.npos - 1l) * hcd * 4l)), uint64(hcd * 4l), sg.start_pos + sg.npos)
                    } else {
                        stash_mtp_carry(ss, contents_at(bxf, uint64(int64(i) * dim * 4l)), uint64(dim * 4l), sg.start_pos + sg.npos)
                    }
                }
            }
        }
        if (gpu_logits || hidden_only) {   //! a hidden-only window's reader takes x_b alone: no final step runs after it
            prefill_override_logits_done()
        }
        readback_us += int64(get_time_usec(ts_rb))
        g_prefills++
        g_pf_seg_windows += nseg > 1l ? 1l : 0l
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
        if (g_pf_moe_rows_on && ran && !moe_gemv_route) {   //! the last routed layer's counts, still in the buffer (the gathered route writes none)
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
        pool_release(g_upool, dn_tab.buf, dn_tab.bytes)
        if (bcap != null) {
            pool_release(g_upool, bcap, bytes_cap)
        }
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
    if (seg_counts) {
        for (sg in segs) {
            pool_release(g_pf_upool, sg.u_kv, 4ul)
            if (hetero) {
                pool_release(g_pf_upool, sg.u_kv_sw, 4ul)
            }
        }
    }
    if (seg_panels) {
        for (sg, i in segs, count()) {
            pf_kv_panels_release(t, sg.kvrows, int64(i) * nl_kv, nl_kv, sbks, sbvs)
        }
        sbks |> clear()
        sbvs |> clear()
    }
    if (gpu_logits) {
        pool_release(g_pf_pool, bxf, bytes_xf)
        pool_release(g_pf_pool, blog, bytes_log)
        if (nseg > 1l) {
            pool_release(g_pf_pool, bxf1, uint64(dim * 4l))
            pool_release(g_pf_pool, blog1, uint64(c.vocab_size * 4l))
        }
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
    pf_kv_panels_release(t, kvrows_w, 0l, nl_kv, bks, bvs)
    if (plef) {
        if (ple_gpu) {
            pool_release(g_pf_pool, bple, bytes_ple_side)
            pool_release(g_pf_pool, bple_proj, bytes_ple_proj)
            pool_release(g_pf_upool, bple_tok, uint64(npos * 4l))
            pool_release(g_pf_upool, u_ple_all, 4ul)
            pool_release(g_pf_upool, u_ple_sq, 4ul)
        } else {
            pool_release(g_pf_upool, bple, bytes_ple_side)
        }
        pool_release(g_pf_pool, bpleg, bytes_ple_g)
        pool_release(g_pf_upool, u_ple, 4ul)
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
    }
    pool_release(g_pf_upool, u_total_dim, 4ul)
    pool_release(g_pf_upool, u_total_h, 4ul)
    if (u_total_h2 != null) {
        pool_release(g_pf_upool, u_total_h2, 4ul)
    }
    if (moe) {
        pool_release(g_pf_upool, u_moe_ne, 4ul)
        pool_release(g_pf_upool, u_moe_nk, 4ul)
        pool_release(g_pf_upool, u_moe_tot, 4ul)
        pool_release(g_pf_upool, u_moe_nfe, 4ul)
        pool_release(g_pf_upool, u_moe_ptot, 4ul)
        if (u_moe_act_tot != null) {
            pool_release(g_pf_upool, u_moe_act_tot, 4ul)
        }
        if (g4moe) {
            pool_release(g_pf_upool, u_moe_invd, 4ul)
        }
        pool_release(g_pf_upool, u_moe_gmode, 4ul)
        if (nsh > 0l) {
            pool_release(g_pf_upool, u_moe_nsh, 4ul)
            pool_release(g_pf_upool, u_moe_shtot, 4ul)
        }
    }
    pool_release(g_pf_upool, u_eps, 4ul)
    if (u_esc != null) {
        pool_release(g_pf_upool, u_esc, 4ul)
    }
    if (btok_win != null) {
        pool_release(g_pf_upool, btok_win, uint64(npos * 4l))
    }
    if (btok_wm != null) {
        pool_release(g_pf_upool, btok_wm, uint64(npos * 4l))
    }
    if (ok) {
        for (sg in segs) {
            sg.s.embed_gpu_pending = false   //! the window gathered the rows the gate left to it
        }
    }
    if (ok) {
        g_pf_stage_prefills++
        g_pf_stage_setup_us += int64(setup_us)
        g_pf_stage_encwait_us += int64(encwait_us)
        g_pf_stage_encode_ms += encode_ms
        g_pf_stage_gpu_ms += gpu_ms
        g_pf_stage_rb_us += readback_us
        g_pf_last_logits = gpu_logits ? 1 : (hidden_only ? 2 : 0)
        g_pf_last_kv = mirk ? 2 : (mirw ? 1 : 0)
        let total_us = get_time_usec(ts_all)
        to_log(LOG_INFO, "dasLLAMA metal prefill: npos={npos} segs={nseg} gemm=mulmm total={float(total_us) / 1000.0}ms setup={float(setup_us) / 1000.0}ms encode+wait={float(encwait_us) / 1000.0}ms (encode {encode_ms}ms, gpu {gpu_ms}ms) kv+readback={float(readback_us) / 1000.0}ms dispatches={17l * c.n_layers + (gpu_logits ? 2l : 0l)} logits={gpu_logits ? "gpu" : (hidden_only ? "none" : "cpu")} kv={mirk ? "mirror" : (mirw ? "panels+mirror" : "panels")} devw_scratch={g_pf_devw_scratch_sites - devw0.sites} sites/{int64(g_pf_devw_scratch_bytes - devw0.bytes) >> 20l} MB\n")   // nolint:PERF028,PERF026 — one line per prefill call, not per token
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
var private g_pf_tall_crowned : bool     //!< the box's "hmm_tall" crown (ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-gemm-ladder)

def private pf_tall_on : bool => g_pf_env_tall && g_pf_tall_crowned
var private g_pf_env_qkr = true
var private g_pf_env_grid1d = false
var private g_pf_env_lastrow = true
var private g_pf_env_dbuf = true

var private CVT_MIN_ROWS = 256l          //!< sidecar knob "metal_cvt_min_rows" — cached at init
var private SPLIT_K_TGS = 256l           //!< sidecar knob "metal_split_k_tgs" (0 under DASLLAMA_METAL_SPLIT_K=0) — cached at init
var private SPLIT_K_TGS_KQ = 2048l       //!< sidecar knob "metal_split_k_tgs_kq" — the K-quant scaffold's knee
var private Q8_N32_ROWS = 64l            //!< sidecar knob "metal_q8_n32_rows" — the one-way 32-column q8 stamp past the split knee
var private SPLIT_K_TGS_BASE = 256l        //!< sidecar knob "metal_split_k_tgs_base" — the q8 simdgroup base tile's knee
var private SPLIT_K_TGS_KQ_BASE = 2048l     //!< sidecar knob "metal_split_k_tgs_kq_base" — the K-quant and per-32 base tiles' knee
var private SPLIT_K_TGS_HMM = 64l           //!< sidecar knob "metal_split_k_tgs_hmm" — the dev-W half tile's knee
var private TALL_OCC_FLOOR = 64l         //!< sidecar knob "metal_tall_floor" — cached at init
var private DEVW_SMALL_PANEL = 32ul * 1024ul * 1024ul   //!< sidecar knob "metal_devw_small_panel_mb"
let DEVW_BIG_PANEL = 48ul * 1024ul * 1024ul
let DEVW_WIDE_N = 4096l
let DEVW_WIDE_N_ROWS = 64l
let DEVW_BIG_ROWS = 2048l
let DEVW_LONG_K = 4096l
let DEVW_LONG_K_ROWS = 1024l
let DEVW_MAX_TILES = 32l
let DEVW_SCRATCH_ROWS = 64l
var private DEVW_MIN_TGS = 64l             //!< sidecar knob "metal_devw_min_tgs" — the dev-W GEMM's per-tile grid floor, cached at init
let DEVW_KQ_TWIN_ROWS = 384l
let DEVW_Q8_TWIN_ROWS = 2048l
var private TALLKQ_MIN_PANEL = 96ul * 1024ul * 1024ul //! DRAM-residency knee, raced on panel BYTES (ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-gemm-ladder)
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

//! the registered segments override: the window over several sessions' chunks where every segment takes it, declined
//! whole otherwise so the caller runs them one at a time
def private metal_prefill_segments_served(t : Model; var segs : array<PrefillSeg>) : bool {
    g_pf_segs |> clear()
    g_pf_segs |> reserve(long_length(segs))
    for (sg in segs) {
        g_pf_segs |> push(PfSeg(s = sg.s, start_pos = sg.start_pos, npos = sg.npos))
    }
    var ok = false
    if (pf_segments_servable(t, g_pf_segs)) {
        ok = metal_served(metal_prefill_window(t, g_pf_segs))
    }
    g_pf_segs |> clear()   //! the records borrow the sessions and the recurrent mirrors' buffers: nothing outlives the call
    return ok
}

[init]
def dasllama_metal_prefill_register() {
    //! registered but dormant - only select_prefill_override("metal") activates it, and
    //! unsupported shapes decline per call
    register_prefill_override("metal", @@metal_prefill_served)
    register_prefill_segments_override("metal", @@metal_prefill_segments_served)
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
        to_log(LOG_WARNING, "dasLLAMA metal prefill: DASLLAMA_METAL_MM_TAIL=0 - small npos % 32 remainders bill the padded tile; the last layer's narrowed FFN row keeps its one-row form (DASLLAMA_METAL_LASTROW=0 for the full panel) (numerics move with the form)\n")
    }
    if (!g_env_metal.split_k) {
        to_log(LOG_WARNING, "dasLLAMA metal prefill: DASLLAMA_METAL_SPLIT_K=0 - every short window's tile walks its whole K (the sidecar's metal_split_k_tgs is not read) (numerics move with the form)\n")
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
    var bw = metal_buffer_from(dev, uint64(ndim * kdim * 2), unsafe(addr<void?>(wa[0])))
    var bx = metal_buffer_from(dev, uint64(m * kdim * 2), unsafe(addr<void?>(xa[0])))
    var by_base = metal_buffer_from(dev, uint64(m * ndim * 4), null)
    var by_twin = metal_buffer_from(dev, uint64(m * ndim * 4), null)
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
    var bxa = metal_buffer_from(dev, uint64(m * kdim * 4), unsafe(addr<void?>(xa[0])))
    var bwb = metal_buffer_from(dev, uint64(ndim * kdim * 2), unsafe(addr<void?>(wb[0])))
    var by_base = metal_buffer_from(dev, uint64(m * ndim * 4), null)
    var by_twin = metal_buffer_from(dev, uint64(m * ndim * 4), null)
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
    var p = race_pso_pair_stamp(dev, "mulmm_q8", MetalQ8MulMm_metal_q8_mulmm_msl, MetalQ8MulMmT_metal_q8_mulmm_t_msl)
    return p.verdict if (!p.ok)
    var res = p.verdict
    let base_pso = p.base_pso
    let twin_pso = p.twin_pso
    var xa <- race_x_f32(m * kdim)
    var blob <- race_q8_blob(ndim * nkb)
    var bw = metal_buffer_from(dev, uint64(ndim * nkb * 34), unsafe(addr<void?>(blob[0])))
    var bxa = metal_buffer_from(dev, uint64(m * kdim * 4), unsafe(addr<void?>(xa[0])))
    var by_base = metal_buffer_from(dev, uint64(m * ndim * 4), null)
    var by_twin = metal_buffer_from(dev, uint64(m * ndim * 4), null)
    var bk = race_uniform_u32(dev, uint(kdim))
    var bn = race_uniform_u32(dev, uint(ndim))
    let grid = uint3(uint(m / 32), uint(ndim / 64), 1u)
    let tg = uint3(128u, 1u, 1u)
    race_pair_ms(queue, 5, res.base_ms, res.twin_ms, $(enc) {
        kn_pipeline(enc, base_pso)
        kn_tgmem(enc, MetalQ8MulMm_metal_q8_mulmm_msl_tgmem, 0)
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
    var cnt, basep, bkt : array<uint>
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
    var bw = metal_buffer_from(dev, uint64(ne * eblk * 34), unsafe(addr<void?>(blob[0])))
    var bxa = metal_buffer_from(dev, uint64(m * kdim * 4), unsafe(addr<void?>(xa[0])))
    var by_base = metal_buffer_from(dev, uint64(m * ndim * 4), null)
    var by_twin = metal_buffer_from(dev, uint64(m * ndim * 4), null)
    var bcnt = metal_buffer_from(dev, uint64(ne * 4), unsafe(addr<void?>(cnt[0])))
    var bbase = metal_buffer_from(dev, uint64(ne * 4), unsafe(addr<void?>(basep[0])))
    var bbkt = metal_buffer_from(dev, uint64(m * 4), unsafe(addr<void?>(bkt[0])))
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

//! one dispatch of a routed split-class tensor twin, hand-bound at the dense split layout: the weight views 0..2, x 3, y 4,
//! the kdim/ndim uniforms 5/6, the bucket counts and bases 7/8 - the generated builder's numbers, its tile count the grid's
def private race_moe_split_twin_arm(enc : MetalComputeEncoder?; pso : MetalComputePipeline?; tgmem : uint64; bs : MetalBuffer?; doff : uint64;
                                    bq, bx, by, bkd, bnd, bcnt, bbase : MetalBuffer?; grid, tg : uint3) {
    kn_pipeline(enc, pso)
    kn_tgmem(enc, tgmem, 0)
    kn_buffer(enc, bs, doff, 0)
    kn_buffer(enc, bs, 0ul, 1)
    kn_buffer(enc, bq, 0ul, 2)
    kn_buffer(enc, bx, 0ul, 3)
    kn_buffer(enc, by, 0ul, 4)
    kn_buffer(enc, bkd, 0ul, 5)
    kn_buffer(enc, bnd, 0ul, 6)
    kn_buffer(enc, bcnt, 0ul, 7)
    kn_buffer(enc, bbase, 0ul, 8)
    kn_dispatch(enc, grid, tg)
}

let MOE_KQ_RACE_EXPERTS = 128     //! the K-quant MoE races' expert count: the planes stream past the last-level cache, as a served model's do
let MOE_KQ_RACE_PART_ROWS = 12    //! their partial region's live rows an expert: the house window's mean

[arch(at="../ARCHITECTURE_GPU_PREFILL_MOE.md#prefill-moe-kq-twin-pick")]
def private race_moe_mulmm_kq(dev : MetalDevice?; queue : MetalCommandQueue?; family : string; fmt : KqFmt;   // nolint:STYLE038 — one-shot A/B race harness; buffers stay live to the release tail
                              base_src, base_entry : string; base_fm : bool; base_tgmem : uint64;
                              twin_src, twin_entry : string; twin_fm : bool; twin_tgmem : uint64;
                              qu_per_sb : int; sform : KqRaceScaleForm) : MetalTensorRaceResult {
    //! the shape the verdict stands for: 128 experts of a 1024 x 2048 plane (past the last-level cache - the tile is
    //! staging-bound where the planes stream), one whole-tile region and one of twelve live rows an expert (the house
    //! window's mean), each timed in the encoder; the outputs compared on the whole-tile region, where both arms write every row
    let ne = MOE_KQ_RACE_EXPERTS
    let rows_per = 32
    let m = ne * rows_per
    let kdim = 2048
    let ndim = 1024
    let nsb = kdim / 256
    var p = race_pso_pair(dev, family, base_src, base_entry, base_fm, twin_src, twin_entry, twin_fm)
    return p.verdict if (!p.ok)
    var res = p.verdict
    let base_pso = p.base_pso
    let twin_pso = p.twin_pso
    var xa <- race_x_f32(m * kdim)
    var planes = race_kq_planes(dev, ne * ndim * nsb, qu_per_sb, sform)
    var bq = planes.bq
    var bs = planes.bs
    let doff = planes.doff
    var cnt, cnt_part, basep, bkt : array<uint>
    cnt |> reserve_resize(ne)
    cnt_part |> reserve_resize(ne)
    basep |> reserve_resize(ne)
    bkt |> reserve_resize(m)
    for (e in range(ne)) {
        cnt[e] = uint(rows_per)
        cnt_part[e] = uint(MOE_KQ_RACE_PART_ROWS)
        basep[e] = uint(e * rows_per)
    }
    for (i in range(m)) {
        bkt[i] = uint(i)   //! identity buckets: gather=0 semantics match
    }
    var bxa = metal_buffer_from(dev, uint64(m * kdim * 4), unsafe(addr<void?>(xa[0])))
    var by_base = metal_buffer_from(dev, uint64(m * ndim * 4), null)
    var by_twin = metal_buffer_from(dev, uint64(m * ndim * 4), null)
    var by_part_base = metal_buffer_from(dev, uint64(m * ndim * 4), null)
    var by_part_twin = metal_buffer_from(dev, uint64(m * ndim * 4), null)
    var bcnt = metal_buffer_from(dev, uint64(ne * 4), unsafe(addr<void?>(cnt[0])))
    var bcnt_part = metal_buffer_from(dev, uint64(ne * 4), unsafe(addr<void?>(cnt_part[0])))
    var bbase = metal_buffer_from(dev, uint64(ne * 4), unsafe(addr<void?>(basep[0])))
    var bbkt = metal_buffer_from(dev, uint64(m * 4), unsafe(addr<void?>(bkt[0])))
    var dims = fixed_array(uint(kdim), uint(ndim))
    var bkd = metal_buffer_from(dev, 4ul, unsafe(addr<void?>(dims[0])))
    var bnd = metal_buffer_from(dev, 4ul, unsafe(addr<void?>(dims[1])))
    var rka = moe_mm_ka(fmt, int64(kdim), int64(ndim), 1l, false)
    let grid = uint3(uint(rows_per / 32), uint(ndim / 64), uint(ne))
    let tg = uint3(128u, 1u, 1u)
    race_pair_ms(queue, 5, res.base_ms, res.twin_ms, $(enc) {
        kn_pipeline(enc, base_pso)
        kn_tgmem(enc, base_tgmem, 0)
        kn_buffer(enc, bs, doff, 0)
        kn_buffer(enc, bs, 0ul, 1)
        kn_buffer(enc, bq, 0ul, 2)
        kn_kargs(enc, rka, 5)
        kn_moe_mm_family_tail(enc, bxa, by_base, bcnt, bbase, bbkt)
        kn_dispatch(enc, grid, tg)
        kn_kargs(enc, rka, 5)
        kn_moe_mm_family_tail(enc, bxa, by_part_base, bcnt_part, bbase, bbkt)
        kn_dispatch(enc, grid, tg)
    }, $(enc) {
        race_moe_split_twin_arm(enc, twin_pso, twin_tgmem, bs, doff, bq, bxa, by_twin, bkd, bnd, bcnt, bbase, grid, tg)
        race_moe_split_twin_arm(enc, twin_pso, twin_tgmem, bs, doff, bq, bxa, by_part_twin, bkd, bnd, bcnt_part, bbase, grid, tg)
    })
    kq_race_verdict(res, by_base, by_twin, m * ndim)
    metal_release(bq)
    metal_release(bs)
    metal_release(bxa)
    metal_release(by_base)
    metal_release(by_twin)
    metal_release(by_part_base)
    metal_release(by_part_twin)
    metal_release(bcnt)
    metal_release(bcnt_part)
    metal_release(bbase)
    metal_release(bbkt)
    metal_release(bkd)
    metal_release(bnd)
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
    var cnt, basep, bkt : array<uint>
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
    var bnq = metal_buffer_from(dev, uint64(ne * eblk * 16), unsafe(addr<void?>(nplane[0])))
    var bee = metal_buffer_from(dev, uint64(ne * eblk), unsafe(addr<void?>(eplane[0])))
    var bvt = metal_buffer_from(dev, uint64(16 * 4), unsafe(addr<void?>(vtabh[0])))
    var bwbb = metal_buffer_from(dev, uint64(ne * ndim * 4), null)
    var bxa = metal_buffer_from(dev, uint64(m * kdim * 4), unsafe(addr<void?>(xa[0])))
    var by_base = metal_buffer_from(dev, uint64(m * ndim * 4), null)
    var by_twin = metal_buffer_from(dev, uint64(m * ndim * 4), null)
    var bcnt = metal_buffer_from(dev, uint64(ne * 4), unsafe(addr<void?>(cnt[0])))
    var bbase = metal_buffer_from(dev, uint64(ne * 4), unsafe(addr<void?>(basep[0])))
    var bbkt = metal_buffer_from(dev, uint64(m * 4), unsafe(addr<void?>(bkt[0])))
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
    var bxa = metal_buffer_from(dev, uint64(m * kdim * 4), unsafe(addr<void?>(xa[0])))
    var by_base = metal_buffer_from(dev, uint64(m * ndim * 4), null)
    var by_twin = metal_buffer_from(dev, uint64(m * ndim * 4), null)
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
    var q_pso = pso_from_source(dev, metal_attn_qk_mm_msl, metal_attn_qk_mm_msl_entry, metal_attn_qk_mm_msl_fastmath, err)
    var qt_pso = pso_from_source(dev, MetalAttnQKMmT_metal_attn_qk_mm_t_msl, MetalAttnQKMmT_metal_attn_qk_mm_t_msl_entry, MetalAttnQKMmT_metal_attn_qk_mm_t_msl_fastmath, err)
    var a_pso = pso_from_source(dev, MetalAttnAVMm_metal_attn_av_mm_msl, MetalAttnAVMm_metal_attn_av_mm_msl_entry, MetalAttnAVMm_metal_attn_av_mm_msl_fastmath, err)
    var at_pso = pso_from_source(dev, MetalAttnAVMmT_metal_attn_av_mm_t_msl, MetalAttnAVMmT_metal_attn_av_mm_t_msl_entry, MetalAttnAVMmT_metal_attn_av_mm_t_msl_fastmath, err)
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
        var bq = metal_buffer_from(dev, uint64(mp * qd * 4), unsafe(addr<void?>(qv[0])))
        var bkk = metal_buffer_from(dev, uint64(np32 * kvd * 4), unsafe(addr<void?>(kv[0])))
        var bp = metal_buffer_from(dev, uint64(heads * mp * np32 * 2), unsafe(addr<void?>(pv[0])))
        var batt_b = metal_buffer_from(dev, uint64(heads * mp * np32 * 2), unsafe(addr<void?>(zeros[0])))
        var batt_t = metal_buffer_from(dev, uint64(heads * mp * np32 * 2), unsafe(addr<void?>(zeros[0])))
        var bxb_b = metal_buffer_from(dev, uint64(mp * qd * 4), unsafe(addr<void?>(zeros[0])))
        var bxb_t = metal_buffer_from(dev, uint64(mp * qd * 4), unsafe(addr<void?>(zeros[0])))
        var bstat = metal_buffer_from(dev, uint64(heads * mp * 2 * 4), unsafe(addr<void?>(sv[0])))
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
    var psos <- [for (s in srcs); pso_from_source(dev, s.src, s.entry, s.fm, err)]
    delete srcs
    if (psos_ready(psos)) {
        var kv <- race_x_f32(nk * kvd)
        var qh <- race_x_f16(mp * qd)
        var bqh = metal_buffer_from(dev, uint64(mp * qd * 2), unsafe(addr<void?>(qh[0])))
        var bkk = metal_buffer_from(dev, uint64(nk * kvd * 4), unsafe(addr<void?>(kv[0])))
        var bkh = metal_buffer_from(dev, uint64(nk * kvd * 2), null)
        var bvh = metal_buffer_from(dev, uint64(nk * kvd * 2), null)
        var batt_b = metal_buffer_from(dev, uint64(heads * mp * nk * 2), null)
        var batt_t = metal_buffer_from(dev, uint64(heads * mp * nk * 2), null)
        var bstat_b = metal_buffer_from(dev, uint64(heads * mp * 2 * 4), null)
        var bstat_t = metal_buffer_from(dev, uint64(heads * mp * 2 * 4), null)
        var bxb_b = metal_buffer_from(dev, uint64(mp * qd * 4), null)
        var bxb_t = metal_buffer_from(dev, uint64(mp * qd * 4), null)
        var bsink = metal_buffer_from(dev, uint64(heads * 4), null)
        var rka = AttnArgs(qd = uint(qd), kv_dim = uint(kvd), head_size = uint(hs), kv_mul = uint(kv_mul),
            npos = uint(nk), np32 = uint(nk), scale = 0.125, qoff = uint(qoff), qrows = uint(mp),
            window = 0u, softcap = 0.0, hass = 0u)
        var tka = gk_f16_rows_args(uint(nk * kvd), rka.scale)
        var tva = gk_f16_rows_args(uint(nk * kvd))
        race_pair_ms(queue, 5, res.base_ms, res.twin_ms, $(enc) {
            enc_qk_mm_th_c_pso(enc, psos[0], bqh, 0ul, bkk, batt_b, rka, int64(mp), int64(nk), int64(heads))
            enc_rowstat_pso(enc, psos[1], batt_b, rka, bstat_b, bsink, int64(mp), int64(heads))
            enc_av_mm_p_c_pso(enc, psos[2], batt_b, bkk, bxb_b, 0ul, rka, bstat_b, int64(mp), int64(heads), int64(hs))
        }, $(enc) {
            enc_half_rows_pso(enc, psos[3], bkk, bkh, tka, int64((nk * kvd + 3) / 4))
            enc_half_rows_pso(enc, psos[3], bkk, bvh, tva, int64((nk * kvd + 3) / 4))
            enc_qk_dev128_c_pso(enc, psos[4], bqh, 0ul, bkh, 0ul, batt_t, rka, int64(mp), int64(nk), int64(heads))
            enc_attn_pexp_pso(enc, psos[5], batt_t, rka, bstat_t, bsink, int64(mp), int64(heads))
            enc_av_dev128_c_pso(enc, psos[6], batt_t, bvh, 0ul, bxb_t, 0ul, rka, bstat_t, int64(mp), int64(heads), int64(hs / 64))
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

//! a format's quant words a race superblock: its stride units of a 256-element superblock, kq_qsb bytes each, four bytes a word

//! the per-32 formats' dense races, one `kq_mulmm_<fmt>` verdict a format: the simdgroup tile against the f32-X twin
def private race_kq_mulmm_b32(dev : MetalDevice?; queue : MetalCommandQueue?; var results : array<MetalTensorRaceResult>) {
    results |> emplace(race_stems(race_kq_mulmm, dev, queue, "kq_mulmm_q51", MetalKqMulMmQ51_metal_kq_mulmm_msl, MetalKqMulMmQ51T_metal_kq_mulmm_b32_t_msl, race_qu(KqFmt.q51), KqRaceScaleForm.pair_dm))
    results |> emplace(race_stems(race_kq_mulmm, dev, queue, "kq_mulmm_iq4nl32", MetalKqMulMmIq4nl32_metal_kq_mulmm_msl, MetalKqMulMmIq4nl32T_metal_kq_mulmm_b32_t_msl, race_qu(KqFmt.iq4nl32), KqRaceScaleForm.pure_d))
    results |> emplace(race_stems(race_kq_mulmm, dev, queue, "kq_mulmm_q4032", MetalKqMulMmQ4032_metal_kq_mulmm_msl, MetalKqMulMmQ4032T_metal_kq_mulmm_b32_t_msl, race_qu(KqFmt.q4032), KqRaceScaleForm.pure_d))
    results |> emplace(race_stems(race_kq_mulmm, dev, queue, "kq_mulmm_q5032", MetalKqMulMmQ5032_metal_kq_mulmm_msl, MetalKqMulMmQ5032T_metal_kq_mulmm_b32_t_msl, race_qu(KqFmt.q5032), KqRaceScaleForm.pure_d))
}

//! every superblock and per-32 expert format's MoE-shaped race, one `moe_mulmm_<fmt>` verdict a format
def private race_moe_mulmm_formats(dev : MetalDevice?; queue : MetalCommandQueue?; var results : array<MetalTensorRaceResult>) {
    results |> emplace(race_stems(race_moe_mulmm_kq, dev, queue, "moe_mulmm_k4", KqFmt.k4, metal_moe_mulmm_k4_msl, MetalMoeMulMmK4T_metal_moe_mulmm_k45_t_msl, race_qu(KqFmt.k4), KqRaceScaleForm.compact))
    results |> emplace(race_stems(race_moe_mulmm_kq, dev, queue, "moe_mulmm_k5", KqFmt.k5, metal_moe_mulmm_k5_msl, MetalMoeMulMmK5T_metal_moe_mulmm_k45_t_msl, race_qu(KqFmt.k5), KqRaceScaleForm.compact))
    results |> emplace(race_stems(race_moe_mulmm_kq, dev, queue, "moe_mulmm_k6", KqFmt.k6, metal_moe_mulmm_k6_msl, MetalMoeMulMmK6T_metal_moe_mulmm_k6_t_msl, race_qu(KqFmt.k6), KqRaceScaleForm.split_d))
    results |> emplace(race_stems(race_moe_mulmm_kq, dev, queue, "moe_mulmm_k3", KqFmt.k3, metal_moe_mulmm_k3_msl, MetalMoeMulMmK3T_metal_moe_mulmm_k3_t_msl, race_qu(KqFmt.k3), KqRaceScaleForm.split_d))
    results |> emplace(race_stems(race_moe_mulmm_kq, dev, queue, "moe_mulmm_k2", KqFmt.k2, metal_moe_mulmm_k2_msl, MetalMoeMulMmK2T_metal_moe_mulmm_k2_t_msl, race_qu(KqFmt.k2), KqRaceScaleForm.split_dmin))
    results |> emplace(race_stems(race_moe_mulmm_kq, dev, queue, "moe_mulmm_iq4xs", KqFmt.iq4xs, metal_moe_mulmm_iq4xs_msl, MetalMoeMulMmIq4xsT_metal_moe_mulmm_iq4xs_t_msl, race_qu(KqFmt.iq4xs), KqRaceScaleForm.split_d))
    results |> emplace(race_stems(race_moe_mulmm_kq, dev, queue, "moe_mulmm_iq3s4", KqFmt.iq3s4, metal_moe_mulmm_iq3s4_msl, MetalMoeMulMmIq3s4T_metal_moe_mulmm_iq4xs_t_msl, race_qu(KqFmt.iq3s4), KqRaceScaleForm.split_d))
    results |> emplace(race_stems(race_moe_mulmm_kq, dev, queue, "moe_mulmm_iq4nl", KqFmt.iq4nl, metal_moe_mulmm_iq4nl_msl, MetalMoeMulMmIq4nlT_metal_moe_mulmm_iq4xs_t_msl, race_qu(KqFmt.iq4nl), KqRaceScaleForm.pure_d))
    results |> emplace(race_stems(race_moe_mulmm_kq, dev, queue, "moe_mulmm_q40", KqFmt.q40, metal_moe_mulmm_q40_msl, MetalMoeMulMmQ40T_metal_moe_mulmm_iq4xs_t_msl, race_qu(KqFmt.q40), KqRaceScaleForm.pure_d))
    results |> emplace(race_stems(race_moe_mulmm_kq, dev, queue, "moe_mulmm_nv4", KqFmt.nv4, metal_moe_mulmm_nv4_msl, MetalMoeMulMmNv4T_metal_moe_mulmm_iq4xs_t_msl, race_qu(KqFmt.nv4), KqRaceScaleForm.ue4m3))
    results |> emplace(race_stems(race_moe_mulmm_kq, dev, queue, "moe_mulmm_iq3s", KqFmt.iq3s, metal_moe_mulmm_iq3s_msl, MetalMoeMulMmIq3sT_metal_moe_mulmm_iq3s_t_msl, race_qu(KqFmt.iq3s), KqRaceScaleForm.split_d))
    results |> emplace(race_stems(race_moe_mulmm_kq, dev, queue, "moe_mulmm_iq3xxs", KqFmt.iq3xxs, metal_moe_mulmm_iq3xxs_msl, MetalMoeMulMmIq3xxsT_metal_moe_mulmm_iq3xxs_t_msl, race_qu(KqFmt.iq3xxs), KqRaceScaleForm.split_d))
    results |> emplace(race_stems(race_moe_mulmm_kq, dev, queue, "moe_mulmm_iq2s", KqFmt.iq2s, metal_moe_mulmm_iq2s_msl, MetalMoeMulMmIq2sT_metal_moe_mulmm_iq2s_t_msl, race_qu(KqFmt.iq2s), KqRaceScaleForm.split_d))
    results |> emplace(race_stems(race_moe_mulmm_kq, dev, queue, "moe_mulmm_iq2xs", KqFmt.iq2xs, metal_moe_mulmm_iq2xs_msl, MetalMoeMulMmIq2xsT_metal_moe_mulmm_iq2xs_t_msl, race_qu(KqFmt.iq2xs), KqRaceScaleForm.split_d))
    results |> emplace(race_stems(race_moe_mulmm_kq, dev, queue, "moe_mulmm_iq2xxs", KqFmt.iq2xxs, metal_moe_mulmm_iq2xxs_msl, MetalMoeMulMmIq2xxsT_metal_moe_mulmm_iq2xxs_t_msl, race_qu(KqFmt.iq2xxs), KqRaceScaleForm.split_d))
    results |> emplace(race_stems(race_moe_mulmm_kq, dev, queue, "moe_mulmm_q51", KqFmt.q51, metal_moe_mulmm_q51_msl, MetalMoeMulMmQ51T_metal_moe_mulmm_b32_t_msl, race_qu(KqFmt.q51), KqRaceScaleForm.pair_dm))
    results |> emplace(race_stems(race_moe_mulmm_kq, dev, queue, "moe_mulmm_iq4nl32", KqFmt.iq4nl32, metal_moe_mulmm_iq4nl32_msl, MetalMoeMulMmIq4nl32T_metal_moe_mulmm_b32_t_msl, race_qu(KqFmt.iq4nl32), KqRaceScaleForm.pure_d))
    results |> emplace(race_stems(race_moe_mulmm_kq, dev, queue, "moe_mulmm_q4032", KqFmt.q4032, metal_moe_mulmm_q4032_msl, MetalMoeMulMmQ4032T_metal_moe_mulmm_b32_t_msl, race_qu(KqFmt.q4032), KqRaceScaleForm.pure_d))
    results |> emplace(race_stems(race_moe_mulmm_kq, dev, queue, "moe_mulmm_q5032", KqFmt.q5032, metal_moe_mulmm_q5032_msl, MetalMoeMulMmQ5032T_metal_moe_mulmm_b32_t_msl, race_qu(KqFmt.q5032), KqRaceScaleForm.pure_d))
}

//! Tuner entry (the runtime.metal_tensor crown): race every tensor pso twin against its
//! shipped simdgroup kernel on THIS box — compile + output-match to qualify, faster GPU time
//! takes the family's crown. Empty when there is no Metal device.
def metal_tensor_race : array<MetalTensorRaceResult> {
    var results : array<MetalTensorRaceResult>
    results |> reserve(48)
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
    race_moe_mulmm_formats(dev, queue, results)
    for (r in race_attn_pair(dev, queue)) {
        results |> emplace(r)
    }
    results |> emplace(race_attn_dev(dev, queue))
    results |> emplace(race_stems(race_kq_mulmm, dev, queue, "kq_mulmm_k4", MetalKqMulMmK4_metal_kq_mulmm_msl, MetalKqMulMmK4T_metal_kq_mulmm_t_msl, race_qu(KqFmt.k4), KqRaceScaleForm.compact))
    results |> emplace(race_stems(race_kq_mulmm, dev, queue, "kq_mulmm_k5", MetalKqMulMmK5_metal_kq_mulmm_msl, MetalKqMulMmK5T_metal_kq_mulmm_t_msl, race_qu(KqFmt.k5), KqRaceScaleForm.compact))
    results |> emplace(race_stems(race_kq_mulmm, dev, queue, "kq_mulmm_k6", MetalKqMulMmK6_metal_kq_mulmm_msl, MetalKqMulMmK6T_metal_kq_mulmm_k6_t_msl, race_qu(KqFmt.k6), KqRaceScaleForm.split_d))
    results |> emplace(race_stems(race_kq_mulmm, dev, queue, "kq_mulmm_iq4xs", MetalKqMulMmIq4xs_metal_kq_mulmm_msl, MetalKqMulMmIq4xsT_metal_kq_mulmm_iq4xs_t_msl, race_qu(KqFmt.iq4xs), KqRaceScaleForm.split_d))
    results |> emplace(race_stems(race_kq_mulmm, dev, queue, "kq_mulmm_iq3s4", MetalKqMulMmIq3s4_metal_kq_mulmm_msl, MetalKqMulMmIq3s4T_metal_kq_mulmm_iq4xs_t_msl, race_qu(KqFmt.iq3s4), KqRaceScaleForm.split_d))
    results |> emplace(race_stems(race_kq_mulmm, dev, queue, "kq_mulmm_iq4nl", MetalKqMulMmIq4nl_metal_kq_mulmm_msl, MetalKqMulMmIq4nlT_metal_kq_mulmm_iq4xs_t_msl, race_qu(KqFmt.iq4nl), KqRaceScaleForm.pure_d))
    results |> emplace(race_stems(race_kq_mulmm, dev, queue, "kq_mulmm_q40", MetalKqMulMmQ40_metal_kq_mulmm_msl, MetalKqMulMmQ40T_metal_kq_mulmm_iq4xs_t_msl, race_qu(KqFmt.q40), KqRaceScaleForm.pure_d))
    results |> emplace(race_stems(race_kq_mulmm, dev, queue, "kq_mulmm_nv4", MetalKqMulMmNv4_metal_kq_mulmm_msl, MetalKqMulMmNv4T_metal_kq_mulmm_iq4xs_t_msl, race_qu(KqFmt.nv4), KqRaceScaleForm.ue4m3))
    results |> emplace(race_stems(race_kq_mulmm, dev, queue, "kq_mulmm_k3", MetalKqMulMmK3_metal_kq_mulmm_msl, MetalKqMulMmK3T_metal_kq_mulmm_k3_t_msl, race_qu(KqFmt.k3), KqRaceScaleForm.split_d))
    results |> emplace(race_stems(race_kq_mulmm, dev, queue, "kq_mulmm_iq3s", MetalKqMulMmIq3s_metal_kq_mulmm_msl, MetalKqMulMmIq3sT_metal_kq_mulmm_iq3s_t_msl, race_qu(KqFmt.iq3s), KqRaceScaleForm.split_d))
    results |> emplace(race_stems(race_kq_mulmm, dev, queue, "kq_mulmm_iq3xxs", MetalKqMulMmIq3xxs_metal_kq_mulmm_msl, MetalKqMulMmIq3xxsT_metal_kq_mulmm_iq3xxs_t_msl, race_qu(KqFmt.iq3xxs), KqRaceScaleForm.split_d))
    results |> emplace(race_stems(race_kq_mulmm, dev, queue, "kq_mulmm_k2", MetalKqMulMmK2_metal_kq_mulmm_msl, MetalKqMulMmK2T_metal_kq_mulmm_k2_t_msl, race_qu(KqFmt.k2), KqRaceScaleForm.split_dmin))
    results |> emplace(race_stems(race_kq_mulmm, dev, queue, "kq_mulmm_iq2s", MetalKqMulMmIq2s_metal_kq_mulmm_msl, MetalKqMulMmIq2sT_metal_kq_mulmm_iq2s_t_msl, race_qu(KqFmt.iq2s), KqRaceScaleForm.split_d))
    results |> emplace(race_stems(race_kq_mulmm, dev, queue, "kq_mulmm_iq2xs", MetalKqMulMmIq2xs_metal_kq_mulmm_msl, MetalKqMulMmIq2xsT_metal_kq_mulmm_iq2xs_t_msl, race_qu(KqFmt.iq2xs), KqRaceScaleForm.split_d))
    race_kq_mulmm_b32(dev, queue, results)
    results |> emplace(race_stems(race_kq_mulmm, dev, queue, "kq_mulmm_iq2xxs", MetalKqMulMmIq2xxs_metal_kq_mulmm_msl, MetalKqMulMmIq2xxsT_metal_kq_mulmm_iq2xxs_t_msl, race_qu(KqFmt.iq2xxs), KqRaceScaleForm.split_d))
    metal_release(queue)
    metal_release(dev)
    return <- results
}

//! The box-raced split-K knees (`ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-split-k`): the q8 blob's and the K-quant scaffold's
//! threadgroup targets and the rows the one-way 32-column q8 stamp takes past the split knee; `ran` false = no device raced
struct public MetalSplitKnees {
    q8_tgs : int64
    kq_tgs : int64
    q8_n32_rows : int64
    q8_base_tgs : int64
    kq_base_tgs : int64
    devw_min_tgs : int64
    hmm_tgs : int64
    ran : bool
}

let private KNEE_MARGIN = 0.97lf    //! a one-way arm takes the rows only at 3% under the tile: a tie keeps the tile

//! A tensor split race's rungs (`knee_split_rungs`) on the rung table of `fmt`'s family: its double-buffered tile (`p_base`)
//! against its split stamp (`p_sk`), the half activation panel the tensor stamps read; empty where a pipeline is missing
def private knee_tensor_rungs(dev : MetalDevice?; queue : MetalCommandQueue?; fmt : KqFmt; shape : SplitKnee; p_base, p_sk : MetalComputePipeline?;
                              tile : KneeTile; split : KneeSplit) : array<KneeRung> {
    var rungs : array<KneeRung>
    if (p_base != null && p_sk != null) {
        let q8 = fmt == KqFmt.q8
        let k = q8 ? KNEE_Q8_K : KNEE_KQ_K
        rungs <- knee_split_rungs(dev, queue, q8 ? KNEE_Q8_WIDTHS : KNEE_KQ_WIDTHS, k, shape, true, $(n : int64) => knee_ring(dev, fmt, n, k), tile, split)
    }
    return <- rungs
}

//! a ring of bf16 plane copies of an `n` x `k` site past the SLC (the halfword pattern an f16 race panel holds)
def private knee_ring_bf16(dev : MetalDevice?; n, k : int64) : array<KneePlanes> {
    var wh <- race_x_f16(int(n * k))
    let bytes = n * k * 2l
    let copies = int(max(2l, KNEE_RING_BYTES / bytes + 1l))
    var ring <- [for (_c in range(copies)); knee_buf_planes(metal_buffer_from(dev, uint64(bytes), unsafe(addr<void?>(wh[0]))))]
    delete wh
    return <- ring
}

def private knee_buf_planes(b : MetalBuffer?) : KneePlanes => (bq = b, bs = b, doff = 0ul)

//! the q8 knee (`metal_split_k_tgs`): the q8 blob's tensor tile against its 32-column split stamp, and the bf16 plane's tile
//! against its split stamp - the two families the knee serves - their rungs priced together
[arch(at="../ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-split-k")]
def private knee_race_q8(dev : MetalDevice?; queue : MetalCommandQueue?; var kn : MetalSplitKnees) {
    var err = ""
    let shape = SplitKnee(min_k = pf_knee_min_k(false))
    var p_base = race_stems(knee_pso, dev, MetalQ8MulMmTHDb_metal_q8_mulmm_t_msl, err)
    var p_sk = race_stems(knee_pso, dev, MetalQ8MulMmTHSk_metal_q8_mulmm_t_msl, err)
    var all <- knee_tensor_rungs(dev, queue, KqFmt.q8, shape, p_base, p_sk, $(enc : MetalComputeEncoder?; pl : KneePlanes; x, y, bk, bn : MetalBuffer?; n : int64) {
        enc_gemm_mm_thdb_c_pso(enc, p_base, pl.bq, 0ul, pl.bq, x, 0ul, y, 0ul, bk, bn, KNEE_ROWS, n)
    }, $(enc : MetalComputeEncoder?; pl : KneePlanes; x, part, bk, bn : MetalBuffer?; n, nsk : int64) {
        enc_gemm_mm_thsk_c_pso(enc, p_sk, pl.bq, 0ul, pl.bq, x, 0ul, part, 0ul, bk, bn, KNEE_ROWS, n, nsk)
    })
    var b_base = race_stems(knee_pso, dev, MetalBf16MulMmTH_metal_bf16_mulmm_t_msl, err)
    var b_sk = race_stems(knee_pso, dev, MetalBf16MulMmTHSk_metal_bf16_mulmm_t_msl, err)
    if (b_base != null && b_sk != null) {
        var rungs <- knee_split_rungs(dev, queue, KNEE_Q8_WIDTHS, KNEE_Q8_K, shape, true, $(n : int64) => knee_ring_bf16(dev, n, KNEE_Q8_K), $(enc : MetalComputeEncoder?; pl : KneePlanes; x, y, bk, bn : MetalBuffer?; n : int64) {
            pf_enc_bf16_mm_th_c_pso(enc, b_base, pl.bq, 0ul, x, 0ul, y, 0ul, bk, bn, KNEE_ROWS, n)
        }, $(enc : MetalComputeEncoder?; pl : KneePlanes; x, part, bk, bn : MetalBuffer?; n, nsk : int64) {
            pf_enc_bf16_mm_thsk_c_pso(enc, b_sk, pl.bq, 0ul, x, 0ul, part, 0ul, bk, bn, KNEE_ROWS, n, nsk)
        })
        all |> push_from(rungs)
        delete rungs
    }
    if (!empty(all)) {
        kn.q8_tgs = knee_pick(all, shape, "METAL_KNEE q8+bf16")
    } else {
        to_log(LOG_INFO, "dasLLAMA metal prefill: split-K knee race skipped - {err}\n")
    }
    delete all
    kn.q8_n32_rows = p_base != null && p_sk != null ? knee_race_n32(dev, queue, p_base, p_sk) : 0l
    release_handles(p_base, p_sk, b_base, b_sk)
}

//! the row counts the one-way rows race offers `metal_q8_n32_rows`, in order; it stops at the first loss
let public METAL_N32_ROWS = fixed_array(32l, 64l, 96l, 128l)

//! the one-way 32-column rows (`metal_q8_n32_rows`): the 8B down site (14336 x 4096) at 32 to 128 rows, a pair of independent
//! sites a step as the window issues them, the tile against the one-way 32-column stamp; the rows stop at the first loss
[arch(at="../ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-split-k")]
def private knee_race_n32(dev : MetalDevice?; queue : MetalCommandQueue?; p_base, p_sk : MetalComputePipeline?) : int64 {
    let kw = 14336l
    let nw = 4096l
    var xa <- race_x_f16(int(128l * kw))
    var bx = metal_buffer_from(dev, uint64(128l * kw * 2l), unsafe(addr<void?>(xa[0])))
    var ys <- [for (_i in range(4)); metal_buffer_from(dev, uint64(128l * nw * 4l), null)]
    var ring <- knee_ring(dev, KqFmt.q8, nw, kw)
    var bk = race_uniform_u32(dev, uint(kw))
    var bn = race_uniform_u32(dev, uint(nw))
    var rows = 0l
    for (mp in METAL_N32_ROWS) {
        let pr = knee_pair_ms(queue, $(enc; i) {
            for (j in range(2)) {
                let w = ring[(2 * i + j) % length(ring)].bq
                enc_gemm_mm_thdb_c_pso(enc, p_base, w, 0ul, w, bx, 0ul, ys[(i & 1) * 2 + j], 0ul, bk, bn, mp, nw)
            }
        }, $(enc; i) {
            for (j in range(2)) {
                let w = ring[(2 * i + j) % length(ring)].bq
                enc_gemm_mm_thsk_c_pso(enc, p_sk, w, 0ul, w, bx, 0ul, ys[(i & 1) * 2 + j], 0ul, bk, bn, mp, nw, 1l)
            }
        })
        break if (!(pr.b > 0.0lf && pr.t > 0.0lf && pr.t < pr.b * KNEE_MARGIN))
        rows = mp
    }
    knee_ring_release(ring)
    for (b in [bx, bk, bn]) {
        metal_release(b)
    }
    for (b in ys) {
        metal_release(b)
    }
    delete xa
    return rows
}

//! the K-quant knee (`metal_split_k_tgs_kq`): the k4 tensor scaffold's double-buffered tile against its split stamp on the
//! K-quant rung table
[arch(at="../ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-split-k")]
def private knee_race_kq(dev : MetalDevice?; queue : MetalCommandQueue?; var kn : MetalSplitKnees) {
    var err = ""
    let shape = SplitKnee(min_k = pf_knee_min_k(true))
    var p_base = race_stems(knee_pso, dev, MetalKqMulMmK4THDb_metal_kq_mulmm_db_msl, err)
    var p_sk = race_stems(knee_pso, dev, MetalKqMulMmK4THSk_metal_kq_mulmm_db_msl, err)
    var rungs <- knee_tensor_rungs(dev, queue, KqFmt.k4, shape, p_base, p_sk, $(enc : MetalComputeEncoder?; pl : KneePlanes; x, y, bk, bn : MetalBuffer?; n : int64) {
        enc_kq_mm_k4_thdb_c_pso(enc, p_base, pl.bs, pl.doff, pl.bs, 0ul, pl.bq, 0ul, x, 0ul, y, 0ul, bk, bn, KNEE_ROWS, n)
    }, $(enc : MetalComputeEncoder?; pl : KneePlanes; x, part, bk, bn : MetalBuffer?; n, nsk : int64) {
        enc_kq_mm_k4_thsk_c_pso(enc, p_sk, pl.bs, pl.doff, pl.bs, 0ul, pl.bq, 0ul, x, 0ul, part, 0ul, bk, bn, KNEE_ROWS, n, nsk)
    })
    if (!empty(rungs)) {
        kn.kq_tgs = knee_pick(rungs, shape, "METAL_KNEE kq")
    } else {
        to_log(LOG_INFO, "dasLLAMA metal prefill: K-quant split-K knee race skipped - {err}\n")
    }
    delete rungs
    release_handles(p_base, p_sk)
}

//! the dev-W half tile's knee (`metal_split_k_tgs_hmm`): the half GEMM against its split stamp over the half panel the dequant pass
//! leaves, on the q8 rung table
[arch(at="../ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-split-k")]
def private knee_race_hmm(dev : MetalDevice?; queue : MetalCommandQueue?; var kn : MetalSplitKnees) {
    var err = ""
    let shape = SplitKnee(min_k = 256l)
    var p_base = race_stems(knee_pso, dev, MetalHalfMulMmTH_metal_half_mulmm_th_msl, err)
    var p_sk = race_stems(knee_pso, dev, MetalHalfMulMmTHSk_metal_half_mulmm_th_msl, err)
    if (p_base != null && p_sk != null) {
        var rungs <- knee_split_rungs(dev, queue, KNEE_Q8_WIDTHS, KNEE_Q8_K, shape, true, $(n : int64) => knee_ring_bf16(dev, n, KNEE_Q8_K), $(enc : MetalComputeEncoder?; pl : KneePlanes; x, y, bk, bn : MetalBuffer?; n : int64) {
            enc_gemm_hmm_th_pso(enc, p_base, pl.bq, 0ul, x, 0ul, y, 0ul, bk, bn, KNEE_ROWS, n)
        }, $(enc : MetalComputeEncoder?; pl : KneePlanes; x, part, bk, bn : MetalBuffer?; n, nsk : int64) {
            enc_gemm_hmm_thsk_pso(enc, p_sk, pl.bq, 0ul, x, 0ul, part, 0ul, bk, bn, KNEE_ROWS, n, nsk)
        })
        if (!empty(rungs)) {
            kn.hmm_tgs = knee_pick(rungs, shape, "METAL_KNEE hmm")
        }
        delete rungs
    } else {
        to_log(LOG_INFO, "dasLLAMA metal prefill: dev-W split-K knee race skipped - {err}\n")
    }
    release_handles(p_base, p_sk)
}

//! the dev-W floor race's pipelines: the k4 base tile and its split stamp, the reduce, the dequant pass, the half tile and its split
struct private DevwRacePsos {
    base : MetalComputePipeline?
    sk : MetalComputePipeline?
    red : MetalComputePipeline?
    dq : MetalComputePipeline?
    hmm : MetalComputePipeline?
    hmm_sk : MetalComputePipeline?
}

struct private DevwRaceSite {
    rows : int
    k : int
    d : int
    tc : int
}

//! the dev-W floor race's sites: a 1B-to-27B gate's and down's k4 shapes at 64 and 128 rows, each in the column tiles its panel
//! takes (`devw_tile_pick`)
def private devw_race_sites : array<DevwRaceSite> {
    var sites : array<DevwRaceSite>
    for (rows in [64, 128]) {
        for (kd in [int2(5120, 17408), int2(17408, 5120), int2(3072, 8192), int2(8192, 3072), int2(4096, 14336), int2(14336, 4096)]) {
            let tc = devw_tile_pick(int64(rows), int64(kd.y), int64(kd.x), true, ~0ul)
            continue if (tc <= 0l)
            sites |> push(DevwRaceSite(rows = rows, k = kd.x, d = kd.y, tc = int(tc)))
        }
    }
    return <- sites
}

//! every grid the dev-W floor race prices (a column tile's threadgroups at its rows), the floor's candidates
def public metal_devw_race_grids : array<int64> {
    var sites <- devw_race_sites()
    var grids <- [for (st in sites); int64(st.rows / 32 * (st.d / st.tc / 64))]
    delete sites
    return <- grids
}

//! One dev-W rung, `ARCHITECTURE_GPU_PREFILL_GEMM.md#devw-panel-knees`: a k4 site's quantized base tile against the dev-W route the
//! window encodes - its column tiles in turn through two half panels; the base pair co-runs, the dev-W pair shares the scratch
def private knee_devw_rung(dev : MetalDevice?; queue : MetalCommandQueue?; ps : DevwRacePsos; kn : MetalSplitKnees; st : DevwRaceSite) : CutRung {
    let rows = st.rows
    let k = st.k
    let d = st.d
    let td = d / st.tc
    var ring <- knee_ring(dev, KqFmt.k4, int64(d), int64(k))
    var xa <- race_x_f32(rows * k)
    var bx = metal_buffer_from(dev, uint64(rows * k * 4), unsafe(addr<void?>(xa[0])))
    var xh <- race_x_f16(rows * k)
    var bxh = metal_buffer_from(dev, uint64(rows * k * 2), unsafe(addr<void?>(xh[0])))
    let ybytes = uint64(rows * d * 4)
    var y0 = metal_buffer_from(dev, ybytes, null)
    var y1 = metal_buffer_from(dev, ybytes, null)
    var p0 = metal_buffer_from(dev, ybytes * 8ul, null)
    var p1 = metal_buffer_from(dev, ybytes * 8ul, null)
    var wa = metal_buffer_from(dev, uint64(td * k * 2), null)
    var wb = metal_buffer_from(dev, uint64(td * k * 2), null)
    var bk = race_uniform_u32(dev, uint(k))
    var bn = race_uniform_u32(dev, uint(d))
    let tiles = int64(rows / 32 * (d / 64))
    let knee = SplitKnee(tgs = kn.kq_base_tgs, min_k = pf_knee_min_k(true))
    let nsk = split_k_count(tiles, int64(k), knee)
    var ka = SkReduceArgs(total = uint(rows * d / 4), ksplit = uint(nsk))
    let ttiles = int64(rows / 32 * (td / 64))
    let hknee = SplitKnee(tgs = kn.hmm_tgs, min_k = 256l)
    let hsk = split_k_count(ttiles, int64(k), hknee)
    var hka = SkReduceArgs(total = uint(rows * td / 4), ksplit = uint(hsk), cols4 = st.tc > 1 ? uint(td / 4) : 0u, ld4 = uint(d / 4))
    let red_n = int64(st.tc > 1 ? ((rows - 1) * d + td) / 4 : rows * td / 4)   //! the reduce's reach from the tile's column, as `enc_sk_reduce_b` launches it
    let dblk = int64(td) * int64(k) / KQ_B32_ELEMS   //! the dequant pass's unit: one thread a 32-element block
    var dka = CvtArgs(total = uint(dblk))
    let gt = (dblk + 255l) / 256l * 256l
    let nsb = int64(k) / KQ_SB_ELEMS
    let pr = knee_pair_ms(queue, $(enc; i) {
        for (j, y, p in range(2), [y0, y1], [p0, p1]) {
            let pl = ring[(2 * i + j) % length(ring)]
            if (nsk == 1l) {
                enc_kq_mm_k4_c_pso(enc, ps.base, pl.bs, 0ul, pl.bs, 0ul, pl.bq, 0ul, bx, y, 0ul, bk, bn, int64(rows), int64(d))
            } else {
                enc_kq_mm_k4_sk_c_pso(enc, ps.sk, pl.bs, 0ul, pl.bs, 0ul, pl.bq, 0ul, bx, p, 0ul, bk, bn, int64(rows), int64(d), nsk)
            }
        }
        if (nsk > 1l) {
            metal_memory_barrier(enc)
            enc_sk_reduce_c_pso(enc, ps.red, p0, y0, 0ul, ka, int64(rows * d / 4))
            enc_sk_reduce_c_pso(enc, ps.red, p1, y1, 0ul, ka, int64(rows * d / 4))
        }
    }, $(enc; i) {
        for (j, y, p in range(2), [y0, y1], [p0, p1]) {
            let pl = ring[(2 * i + j) % length(ring)]
            for (ti in range(st.tc)) {
                let so = uint64(int64(ti * td) * nsb * kq_ssb(KqFmt.k4))
                let qo = uint64(int64(ti * td) * nsb * kq_qsb(KqFmt.k4))
                let w = ((j * st.tc + ti) & 1) == 0 ? wa : wb
                enc_dequant_k4h_pso(enc, ps.dq, pl.bs, so, pl.bs, so, pl.bq, qo, w, dka, gt)
                metal_memory_barrier(enc)
                let yo = uint64(ti * td * 4)
                if (hsk == 1l) {
                    enc_gemm_hmm_th_pso(enc, ps.hmm, w, 0ul, bxh, 0ul, y, yo, bk, bn, int64(rows), int64(td))
                } else {
                    enc_gemm_hmm_thsk_pso(enc, ps.hmm_sk, w, 0ul, bxh, 0ul, p, 0ul, bk, bn, int64(rows), int64(td), hsk)
                    metal_memory_barrier(enc)
                    enc_sk_reduce_c_pso(enc, ps.red, p, y, yo, hka, red_n)
                }
                metal_memory_barrier(enc)
            }
        }
    })
    knee_ring_release(ring)
    for (b in [bx, bxh, y0, y1, p0, p1, wa, wb, bk, bn]) {
        metal_release(b)
    }
    delete xa
    delete xh
    return CutRung(x = ttiles, base = pr.b, alt = pr.t)
}

//! the dev-W grid floor (`metal_devw_min_tgs`, in threadgroups a column tile's half GEMM launches): dense k4 sites at the widths
//! and depths stocked models hold - a 1B-to-27B gate's and down's - at 64 and 128 rows, each in the column tiles its panel takes
//! (`devw_tile_pick`); the floor is the least tile grid from which the dev-W route is the cheaper total
[arch(at="../ARCHITECTURE_GPU_PREFILL_GEMM.md#devw-panel-knees")]
def private knee_race_devw(dev : MetalDevice?; queue : MetalCommandQueue?; var kn : MetalSplitKnees) {
    var err = ""
    var ps = DevwRacePsos(
        base = race_stems(knee_pso, dev, MetalKqMulMmK4_metal_kq_mulmm_msl, err),
        sk = race_stems(knee_pso, dev, MetalKqMulMmK4Sk_metal_kq_mulmm_msl, err),
        red = race_stems(knee_pso, dev, metal_sk_reduce_msl, err),
        dq = race_stems(knee_pso, dev, MetalDequantK4H_metal_dequant_k45_msl, err),
        hmm = race_stems(knee_pso, dev, MetalHalfMulMmTH_metal_half_mulmm_th_msl, err),
        hmm_sk = race_stems(knee_pso, dev, MetalHalfMulMmTHSk_metal_half_mulmm_th_msl, err))
    if (ps.base == null || ps.sk == null || ps.red == null || ps.dq == null || ps.hmm == null || ps.hmm_sk == null) {
        to_log(LOG_INFO, "dasLLAMA metal prefill: dev-W floor race skipped - {err}\n")
        release_handles(ps.base, ps.sk, ps.red, ps.dq, ps.hmm, ps.hmm_sk)
        return
    }
    var sites <- devw_race_sites()
    var rungs <- [for (st in sites); knee_devw_rung(dev, queue, ps, kn, st)]
    delete sites
    kn.devw_min_tgs = knee_cut_pick(rungs, true, 1l << 20l, "METAL_DEVW")
    delete rungs
    release_handles(ps.base, ps.sk, ps.red, ps.dq, ps.hmm, ps.hmm_sk)
}

//! Race this box's split-K knees on synthetic planes - no model, no child process; `ran` false without a device. The base tiles'
//! knees race in the file declaring them (`metal_knee_race_q8_base`, `metal_knee_race_kq_base`)
def public metal_split_knee_race : MetalSplitKnees {
    var kn = MetalSplitKnees(q8_tgs = metal_split_k_tgs(), kq_tgs = metal_split_k_tgs_kq(), q8_n32_rows = metal_q8_n32_rows(),
        q8_base_tgs = metal_split_k_tgs_base(), kq_base_tgs = metal_split_k_tgs_kq_base(), devw_min_tgs = metal_devw_min_tgs(),
        hmm_tgs = metal_split_k_tgs_hmm())
    var dev = metal_create_system_default_device()
    return kn if (dev == null)
    var queue = metal_new_command_queue(dev)
    knee_race_q8(dev, queue, kn)
    knee_race_kq(dev, queue, kn)
    kn.q8_base_tgs = metal_knee_race_q8_base(dev, queue, SplitKnee(min_k = pf_knee_min_k(false)))
    kn.kq_base_tgs = metal_knee_race_kq_base(dev, queue, SplitKnee(min_k = pf_knee_min_k(true)))
    knee_race_hmm(dev, queue, kn)
    knee_race_devw(dev, queue, kn)
    kn.ran = true
    metal_release(queue)
    metal_release(dev)
    return kn
}

// ===== the transducer greedy decode's four stamps (the ASR-decoder driver's transducer seat) =====

//! an LSTM layer of the transducer's predictor step on the template both homes stamp (`GkRnntPred`): eight gate elements a
//! threadgroup of 256
[metal_dispatch(name = "enc_rnnt_gates", pso = "g_pf_pso_rnnt_gates", tgmem = "metal_rnnt_gates_msl_tgmem", tg = 256, grid = "groups", params = "groups : int64")]
class MetalRnntGates : GkRnntPred {
    [metal_kernel(name="metal_rnnt_gates_msl")]
    def metal_rnnt_gates {
        pred_body()
    }
}

//! joint.pred's projection behind the predictor's last layer (`GkRnntPred`, GATES off): eight rows a threadgroup of 256
[metal_dispatch(name = "enc_rnnt_proj", pso = "g_pf_pso_rnnt_proj", tgmem = "metal_rnnt_proj_msl_tgmem", tg = 256, grid = "groups", params = "groups : int64")]
class MetalRnntProj : GkRnntPred {
    override GATES = false

    [metal_kernel(name="metal_rnnt_proj_msl")]
    def metal_rnnt_proj {
        pred_body()
    }
}

//! the transducer's joint over a window of frames (`GkRnntJoint`): a 64-logit slice a threadgroup, four a 256-logit block
[metal_dispatch(name = "enc_rnnt_joint", pso = "g_pf_pso_rnnt_joint", tgmem = "metal_rnnt_joint_msl_tgmem", tg = 256, grid = "slices", params = "slices : int64")]
class MetalRnntJoint : GkRnntJoint {
    [metal_kernel(name="metal_rnnt_joint_msl")]
    def metal_rnnt_joint {
        joint_body()
    }
}

//! the transducer's greedy pick over the window (`GkRnntPick`): one threadgroup of 256
[metal_dispatch(name = "enc_rnnt_pick", pso = "g_pf_pso_rnnt_pick", tgmem = "metal_rnnt_pick_msl_tgmem", tg = 256, grid = "1")]
class MetalRnntPick : GkRnntPick {
    [metal_kernel(name="metal_rnnt_pick_msl")]
    def metal_rnnt_pick {
        pick_body()
    }
}

var private g_pf_pso_rnnt_gates : MetalComputePipeline?
var private g_pf_pso_rnnt_proj : MetalComputePipeline?
var private g_pf_pso_rnnt_joint : MetalComputePipeline?
var private g_pf_pso_rnnt_pick : MetalComputePipeline?
var private g_pf_rnnt_tried = false

//! The transducer decode's four pipelines, compiled at the first ask: false where the device or a compile failed.
def pf_rnnt_ready() : bool {
    if (!g_pf_rnnt_tried) {
        pf_rnnt_build()
    }
    return psos_ready(fixed_array(g_pf_pso_rnnt_gates, g_pf_pso_rnnt_proj, g_pf_pso_rnnt_joint, g_pf_pso_rnnt_pick))
}

[cold_path]
def private pf_rnnt_build {
    g_pf_rnnt_tried = true
    if (metal_common_init()) {
        var ok = true
        g_pf_pso_rnnt_gates = compile_stamp(metal_rnnt_gates_msl, ok)
        g_pf_pso_rnnt_proj = compile_stamp(metal_rnnt_proj_msl, ok)
        g_pf_pso_rnnt_joint = compile_stamp(metal_rnnt_joint_msl, ok)
        g_pf_pso_rnnt_pick = compile_stamp(metal_rnnt_pick_msl, ok)
    }
}

//! The transducer pipelines released; the next ask compiles them again.
def private pf_rnnt_release {
    release_handles(g_pf_pso_rnnt_gates, g_pf_pso_rnnt_proj, g_pf_pso_rnnt_joint, g_pf_pso_rnnt_pick)
    g_pf_rnnt_tried = false
}

// ===== the CTC head's pick (GigaAM's CTC carrier): the head GEMM is the tower's half or f32 GEMM, its rows land here =====

//! The CTC head's pick on the template both homes stamp (`GkCtcPick`): a threadgroup of 256 a frame
[metal_dispatch(name = "enc_ctc_pick", pso = "g_pf_pso_ctc_pick", tgmem = "metal_ctc_pick_msl_tgmem", tg = 256, grid = "rows", params = "rows : int64")]
class MetalCtcPick : GkCtcPick {
    [metal_kernel(name="metal_ctc_pick_msl")]
    def metal_ctc_pick {
        ctc_pick_body()
    }
}

var private g_pf_pso_ctc_pick : MetalComputePipeline?
var private g_pf_ctc_tried = false

//! The CTC pick's pipeline, compiled at the first ask: false where the device or the compile failed.
def pf_ctc_pick_ready() : bool {
    if (!g_pf_ctc_tried) {
        pf_ctc_pick_build()
    }
    return g_pf_pso_ctc_pick != null
}

[cold_path]
def private pf_ctc_pick_build {
    g_pf_ctc_tried = true
    if (metal_common_init()) {
        var ok = true
        g_pf_pso_ctc_pick = compile_stamp(metal_ctc_pick_msl, ok)
    }
}

//! The CTC pick's pipeline released; the next ask compiles it again.
def private pf_ctc_pick_release {
    release_handles(g_pf_pso_ctc_pick)
    g_pf_ctc_tried = false
}
