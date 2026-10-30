options gen2
options indenting = 4
options _dasllama_internal = true

module dasllama_cuda_classes shared public

require dasllama/dasllama_lint public
require cuda/cuda_kernel public
require cuda/cuda_intrinsics
require spirv/spirv_markers
require dasllama/dasllama_cuda_common public
require dasllama/dasllama_cuda_dispatch
require dasllama/dasllama_kqformat
require dasllama/dasllama_gemm_schema   // the q8 block geometry the knee race's synthetic planes take
require dasllama/dasllama_gpu_math
require dasllama/dasllama_gpu_tier
require dasllama/dasllama_gpu_knee   // the short window's knee race over this file's s column and reduce: the rung loop, the pricing, the knob
require dasllama/dasllama_kv_dtype
require dasllama/dasllama_gpu_kernels_common public
require dasllama/dasllama_gpu_kernels_discrete public
require daslib/templates
require daslib/typemacro_boost
require math
require daslib/math_bits
require strings   // the skip list's tokens
require daslib/strings_boost

// The CUDA KERNEL home: every kernel is a [cuda_kernel]/[cuda_dispatch] class stamping a shared
// template - the discrete layer's GEMV family first - under the same dispatch stems the Vulkan
// home uses, so the format ladders read alike in both homes. The shared bodies' warp folds resolve
// to the discrete file's gk_subgroup_* shims.

//! the workhorse decode GEMV (`Q8GemvT`): two lanes a 32-block, a warp a row
[cuda_dispatch(name = "q8_gemv_cls", grid = "wgs", params = "wgs : int64")]
class CuQ8Gemv : Q8GemvT {
    [cuda_kernel(local_size_x = 64, name = "q8_gemv_cls_cuda")]
    def run {
        gemv_body()
    }
}

[cuda_dispatch(name = "q8_gemv_n2_cls", grid = "wgs", params = "wgs : int64")]
class CuQ8GemvN2 : Q8GemvNT {
    override NC = 2

    [cuda_kernel(local_size_x = 64, name = "q8_gemv_n2_cls_cuda")]
    def run {
        gemv_n_body()
    }
}

[cuda_dispatch(name = "q8_gemv_n4_cls", grid = "wgs", params = "wgs : int64")]
class CuQ8GemvN4 : Q8GemvNT {
    override NC = 4

    [cuda_kernel(local_size_x = 64, name = "q8_gemv_n4_cls_cuda")]
    def run {
        gemv_n_body()
    }
}

[cuda_dispatch(name = "q8_gemv_n8_cls", grid = "wgs", params = "wgs : int64")]
class CuQ8GemvN8 : Q8GemvNT {
    override NC = 8

    [cuda_kernel(local_size_x = 64, name = "q8_gemv_n8_cls_cuda")]
    def run {
        gemv_n_body()
    }
}

//! the q8 decode GEMV carrying the residual step as its last workgroup's epilogue (`GkResidualRow`)
[cuda_dispatch(name = "q8_gemv_ar_cls", grid = "wgs", params = "wgs : int64")]
class CuQ8GemvAr : Q8GemvArT {
    [cuda_kernel(local_size_x = 256, name = "q8_gemv_ar_cls_cuda")]
    def run {
        gemv_ar_body()
    }
}

[cuda_dispatch(name = "q8_gemv_gu_n1_cls", grid = "wgs", params = "wgs : int64")]
class CuQ8GemvGuN1 : Q8GemvGuNT {
    override NC = 1

    [cuda_kernel(local_size_x = 256, name = "q8_gemv_gu_n1_cls_cuda")]
    def run {
        gu_n_body()
    }
}

[cuda_dispatch(name = "q8_gemv_gu_n2_cls", grid = "wgs", params = "wgs : int64")]
class CuQ8GemvGuN2 : Q8GemvGuNT {
    override NC = 2

    [cuda_kernel(local_size_x = 256, name = "q8_gemv_gu_n2_cls_cuda")]
    def run {
        gu_n_body()
    }
}

[cuda_dispatch(name = "q8_gemv_gu_n4_cls", grid = "wgs", params = "wgs : int64")]
class CuQ8GemvGuN4 : Q8GemvGuNT {
    override NC = 4

    [cuda_kernel(local_size_x = 256, name = "q8_gemv_gu_n4_cls_cuda")]
    def run {
        gu_n_body()
    }
}

[cuda_dispatch(name = "q8_gemv_gu_n8_cls", grid = "wgs", params = "wgs : int64")]
class CuQ8GemvGuN8 : Q8GemvGuNT {
    override NC = 8

    [cuda_kernel(local_size_x = 256, name = "q8_gemv_gu_n8_cls_cuda")]
    def run {
        gu_n_body()
    }
}

//! the residual step over the shared `ArBase` (`GkAddRmsRows`): the normed row as floats
[cuda_dispatch(name = "cls_ar", kernel = "run", grid = "nrows", params = "nrows : int64")]
class CuClsArAddRms : ArBase {
    override OUT_F32 = true

    [cuda_kernel(local_size_x = 256, name = "cls_ar_cuda")]
    def run {
        ar_body()
    }
}

//! the residual step on `GkAddRmsRows`, the window's rows: the normed rows as f16
[cuda_dispatch(name = "cls_ar_f16_b", kernel = "run", grid = "nrows", params = "nrows : int64")]
class CuClsArAddRmsF16B : ArBase {
    override OUT_F16 = true

    [cuda_kernel(local_size_x = 256, name = "cls_ar_f16_b_cuda")]
    def run {
        ar_body()
    }
}

//! the residual step on `GkAddRmsRows` with the Q8_0 requant in one dispatch, a workgroup a row
[cuda_dispatch(name = "cls_ar_rq_b", grid = "nrows", params = "nrows : int64")]
class CuClsArAddRmsRqB : ArRqBase {
    [cuda_kernel(local_size_x = 256, name = "cls_ar_rq_b_cuda")]
    def run {
        rq_b_body()
    }
}

//! the same step writing each block's scale x quant sum beside the scales: the K-quant MMQ chain's x feed - `ArRqBase`
[cuda_dispatch(name = "cls_ar_rq_sum_b", grid = "nrows", params = "nrows : int64")]
class CuClsArAddRmsRqSumB : ArRqBase {
    override OUT_SUM = true

    [cuda_kernel(local_size_x = 256, name = "cls_ar_rq_sum_b_cuda")]
    def run {
        rq_b_body()
    }
}

//! the residual step on `GkAddRmsRows` with the Q8_K requant of its one row
[cuda_dispatch(name = "cls_ar_rqk", grid = "wgs", params = "wgs : int64")]
class CuArAddRmsRqK : ArRqT {
    override KBLK = true

    [cuda_kernel(local_size_x = 256, name = "cls_ar_rqk_cuda")]
    def run {
        ar_rq_body()
    }
}

//! the requant on `GkQ8BlockStore`: a float plane into Q8_0 blocks
[cuda_dispatch(name = "cls_dn_rq", kernel = "run", family = "rq_cls", grid = "wgs", params = "wgs : int64")]
class CuDnRequant : RqPlainT {
    [cuda_kernel(local_size_x = 256, name = "cls_dn_rq_cuda")]
    def run {
        rq_body()
    }
}

//! the Q8_0 requant writing each block's scale x quant sum beside the scales: the K-quant MMQ chain's attention feed - `RqPlainT`
[cuda_dispatch(name = "cls_dn_rq_sum", kernel = "run", family = "rq_cls", grid = "wgs", params = "wgs : int64")]
class CuDnRequantSum : RqPlainT {
    override OUT_SUM = true

    [cuda_kernel(local_size_x = 256, name = "cls_dn_rq_sum_cuda")]
    def run {
        rq_body()
    }
}

//! the requant on `GkQ8BlockStore`: a float plane into Q8_K superblocks
[cuda_dispatch(name = "cls_q8k_rq", kernel = "run", family = "rq_cls", grid = "wgs", params = "wgs : int64")]
class CuQ8kRequant : RqPlainT {
    override KBLK = true

    [cuda_kernel(local_size_x = 256, name = "cls_q8k_rq_cuda")]
    def run {
        rq_body()
    }
}

//! the act + requant of a gate / up pair on `GkQ8BlockStore`, into Q8_0 blocks
[cuda_dispatch(name = "q8_actrq_cls", kernel = "run", family = "actrq_cls", grid = "wgs", params = "wgs : int64")]
class CuQ8ActRq : ActRqT {
    [cuda_kernel(local_size_x = 256, name = "q8_actrq_cls_cuda")]
    def run {
        rq_body()
    }
}

//! the act + requant writing each block's scale x quant sum beside the scales: the K-quant MMQ chain's hidden feed - `ActRqT`
[cuda_dispatch(name = "q8_actrq_sum_cls", kernel = "run", family = "actrq_cls", grid = "wgs", params = "wgs : int64")]
class CuQ8ActRqSum : ActRqT {
    override OUT_SUM = true

    [cuda_kernel(local_size_x = 256, name = "q8_actrq_sum_cls_cuda")]
    def run {
        rq_body()
    }
}

//! the act + requant of a gate / up pair on `GkQ8BlockStore`, into Q8_K superblocks
[cuda_dispatch(name = "q8k_actrq_cls", kernel = "run", family = "actrq_cls", grid = "wgs", params = "wgs : int64")]
class CuQ8kActRq : ActRqT {
    override KBLK = true

    [cuda_kernel(local_size_x = 256, name = "q8k_actrq_cls_cuda")]
    def run {
        rq_body()
    }
}

//! the biased twins (`ActRqBT`): the picks' gate and up bias rows join before the activation (gpt-oss's stacks), into Q8_0 blocks
//! and into Q8_K superblocks
[cuda_dispatch(name = "q8_actrq_b_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuQ8ActRqB : ActRqBT {
    [cuda_kernel(local_size_x = 256, name = "q8_actrq_b_cls_cuda")]
    def run {
        rq_body()
    }
}

//! the biased act + requant into Q8_K superblocks (`ActRqBT`)
[cuda_dispatch(name = "q8k_actrq_b_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuQ8kActRqB : ActRqBT {
    override KBLK = true

    [cuda_kernel(local_size_x = 256, name = "q8k_actrq_b_cls_cuda")]
    def run {
        rq_body()
    }
}

//! the act straight to the f16 hidden plane (`ActF16T`)
[cuda_dispatch(name = "actf16_cls", grid = "wgs", params = "wgs : int64")]
class CuActF16 : ActF16T {
    [cuda_kernel(local_size_x = 256, name = "actf16_cls_cuda")]
    def run {
        actf16_body()
    }
}

//! the token-table gather (`EmbGatherT`): the q8 table
[cuda_dispatch(name = "emb_gather_cls", grid = "wgs", params = "wgs : int64")]
class CuEmbGather : EmbGatherT {
    [cuda_kernel(local_size_x = 256, name = "emb_gather_cls_cuda")]
    def run {
        emb_body()
    }
}

//! the per-layer-embedding branch's act as floats, the f32 projection's x, on the template every home stamps (`GkPleAct`): out
//! of place, an element a thread
[cuda_dispatch(name = "ple_act_f32_cls", grid = "wgs", params = "wgs : int64")]
class CuPleActF32 : GkPleAct {
    override OUT = true

    [cuda_kernel(local_size_x = 256, name = "ple_act_f32_cls_cuda")]
    def run {
        act_elem(gl_GlobalInvocationID.x)
    }
}

//! the token-table gather off an F32 / F16 / BF16 table's exact f32 rows (`EmbGatherT` under `WF`)
[cuda_dispatch(name = "emb_gather_f32_cls", grid = "wgs", params = "wgs : int64")]
class CuEmbGatherF32 : EmbGatherT {
    override WF = true

    [cuda_kernel(local_size_x = 256, name = "emb_gather_f32_cls_cuda")]
    def run {
        emb_body()
    }
}

//! the classifier tail's epilogue on the template both homes stamp (`GkClsEpilogue`), an invocation a logit
[cuda_dispatch(name = "cls_epi", grid = "wgs", params = "wgs : int64")]
class CuClsEpilogue : GkClsEpilogue {
    [cuda_kernel(local_size_x = 256, name = "cls_epi_cuda")]
    def run {
        epi_body()
    }
}

//! the classifier tail's pick in two passes (`ClsArgmaxPartT`, `ClsArgmaxFinT`)
[cuda_dispatch(name = "cls_argmax_part", grid = "wgs", params = "wgs : int64")]
class CuClsArgmaxPart : ClsArgmaxPartT {
    [cuda_kernel(local_size_x = 256, name = "cls_argmax_part_cuda")]
    def run {
        part_body()
    }
}

[cuda_dispatch(name = "cls_argmax_fin", grid = "rows", params = "rows : int64")]
class CuClsArgmaxFin : ClsArgmaxFinT {
    [cuda_kernel(local_size_x = 256, name = "cls_argmax_fin_cuda")]
    def run {
        fin_body()
    }
}

//! the classifier tail's selection on the template every home stamps (`GkClsSelect`), a block of 1024 threads a row
[cuda_dispatch(name = "cls_select", grid = "rows", params = "rows : int64")]
class CuClsSelect : GkClsSelect {
    [cuda_kernel(local_size_x = 1024, name = "cls_select_cuda")]
    def run {
        select_row()
    }
}

//! the decode attention (`DaAttnT`) over the K/V mirror's codecs and slab widths
[cuda_dispatch(name = "da_attn_cls", grid = "wgs", params = "wgs : int64")]
class CuDaAttn : DaAttnT {
    typedef KT = float

    [cuda_kernel(local_size_x = 256, name = "da_attn_cls_cuda")]
    def run {
        da_attn_body()
    }
}

[cuda_dispatch(name = "da_attn_f16_cls", grid = "wgs", params = "wgs : int64")]
class CuDaAttnF16 : DaAttnT {
    typedef KT = float16
    override KV16 = true

    [cuda_kernel(local_size_x = 256, name = "da_attn_f16_cls_cuda")]
    def run {
        da_attn_body()
    }
}

[cuda_dispatch(name = "da_attn_g2_cls", grid = "wgs", params = "wgs : int64")]
class CuDaAttnG2 : DaAttnT {
    typedef KT = float
    override G = 2

    [cuda_kernel(local_size_x = 256, name = "da_attn_g2_cls_cuda")]
    def run {
        da_attn_body()
    }
}

[cuda_dispatch(name = "da_attn_f16_g2_cls", grid = "wgs", params = "wgs : int64")]
class CuDaAttnF16G2 : DaAttnT {
    typedef KT = float16
    override KV16 = true
    override G = 2

    [cuda_kernel(local_size_x = 256, name = "da_attn_f16_g2_cls_cuda")]
    def run {
        da_attn_body()
    }
}

[cuda_dispatch(name = "da_attn_q8_cls", grid = "wgs", params = "wgs : int64")]
class CuDaAttnQ8 : DaAttnT {
    typedef KT = uint
    override KVQ8 = true
    override KVBLK = true

    [cuda_kernel(local_size_x = 256, name = "da_attn_q8_cls_cuda")]
    def run {
        da_attn_body()
    }
}

[cuda_dispatch(name = "da_attn_q8_g2_cls", grid = "wgs", params = "wgs : int64")]
class CuDaAttnQ8G2 : DaAttnT {
    typedef KT = uint
    override KVQ8 = true
    override KVBLK = true
    override G = 2

    [cuda_kernel(local_size_x = 256, name = "da_attn_q8_g2_cls_cuda")]
    def run {
        da_attn_body()
    }
}

[cuda_dispatch(name = "da_attn_tq4_cls", grid = "wgs", params = "wgs : int64")]
class CuDaAttnTq4 : DaAttnT {
    typedef KT = uint
    override KVTQ4 = true
    override KVBLK = true

    [cuda_kernel(local_size_x = 256, name = "da_attn_tq4_cls_cuda")]
    def run {
        da_attn_body()
    }
}

[cuda_dispatch(name = "da_attn_tq4_g2_cls", grid = "wgs", params = "wgs : int64")]
class CuDaAttnTq4G2 : DaAttnT {
    typedef KT = uint
    override KVTQ4 = true
    override KVBLK = true
    override G = 2

    [cuda_kernel(local_size_x = 256, name = "da_attn_tq4_g2_cls_cuda")]
    def run {
        da_attn_body()
    }
}

//! the sink stamps (gpt-oss, `DaAttnT` under `SINK`): the head's sink logit joins the softmax's max and denominator, one a mirror codec
[cuda_dispatch(name = "da_attn_sink_cls", grid = "wgs", params = "wgs : int64")]
class CuDaAttnSink : DaAttnT {
    typedef KT = float
    override SINK = true

    [cuda_kernel(local_size_x = 256, name = "da_attn_sink_cls_cuda")]
    def run {
        da_attn_body()
    }
}

//! the f16-mirror sink stamp (`DaAttnT`)
[cuda_dispatch(name = "da_attn_f16_sink_cls", grid = "wgs", params = "wgs : int64")]
class CuDaAttnF16Sink : DaAttnT {
    typedef KT = float16
    override KV16 = true
    override SINK = true

    [cuda_kernel(local_size_x = 256, name = "da_attn_f16_sink_cls_cuda")]
    def run {
        da_attn_body()
    }
}

//! the q8-mirror sink stamp (`DaAttnT`)
[cuda_dispatch(name = "da_attn_q8_sink_cls", grid = "wgs", params = "wgs : int64")]
class CuDaAttnQ8Sink : DaAttnT {
    typedef KT = uint
    override KVQ8 = true
    override KVBLK = true
    override SINK = true

    [cuda_kernel(local_size_x = 256, name = "da_attn_q8_sink_cls_cuda")]
    def run {
        da_attn_body()
    }
}

//! the tq4-mirror sink stamp (`DaAttnT`)
[cuda_dispatch(name = "da_attn_tq4_sink_cls", grid = "wgs", params = "wgs : int64")]
class CuDaAttnTq4Sink : DaAttnT {
    typedef KT = uint
    override KVTQ4 = true
    override KVBLK = true
    override SINK = true

    [cuda_kernel(local_size_x = 256, name = "da_attn_tq4_sink_cls_cuda")]
    def run {
        da_attn_body()
    }
}

//! the token step's stamp: each staged row at its token block's region and position
[cuda_dispatch(name = "kv_q8_store_cls", grid = "wgs", params = "wgs : int64")]
class CuKvQ8Store : KvQ8StoreT {
    [cuda_kernel(local_size_x = 256, name = "kv_q8_store_cls_cuda")]
    def run {
        store_body()
    }
}

//! the window chain's stamp: the staged rows at consecutive positions from `pos0`
[cuda_dispatch(name = "kv_q8_store_b_cls", grid = "wgs", params = "wgs : int64")]
class CuKvQ8StoreB : KvQ8StoreT {
    override TOKROW = false

    [cuda_kernel(local_size_x = 256, name = "kv_q8_store_b_cls_cuda")]
    def run {
        store_body()
    }
}

//! the KV store stamps' grid over `nelem` staged elements: a thread an element
def public kv_store_wgs(nelem : int64) : int64 => (nelem + 255l) / 256l

[cuda_dispatch(name = "kv_q8_shadow_cls", grid = "wgs", params = "wgs : int64")]
class CuKvQ8Shadow : KvShadowT {
    [cuda_kernel(local_size_x = 256, name = "kv_q8_shadow_cls_cuda")]
    def run {
        shadow_body()
    }
}

[cuda_dispatch(name = "kv_tq4_shadow_cls", grid = "wgs", params = "wgs : int64")]
class CuKvTq4Shadow : KvShadowT {
    override TQ4 = true

    [cuda_kernel(local_size_x = 256, name = "kv_tq4_shadow_cls_cuda")]
    def run {
        shadow_body()
    }
}

//! the token step's tq4 stamp on `GkBlkSignedMax`: the q heads rotated in place, the K and V heads rotated and quantized at
//! the token's row; precise, so the butterflies and the quant sums round as the CPU's and the stored bytes are the cache's
[cuda_dispatch(name = "kv_tq4_store_cls", grid = "wgs", params = "wgs : int64")]
class CuKvTq4Store : KvTq4StoreT {
    [cuda_kernel(local_size_x = 256, precise = true, name = "kv_tq4_store_cls_cuda")]
    def run {
        store_body()
    }
}

//! the window chain's tq4 stamp on `GkBlkSignedMax`: the rows at consecutive positions from `pos0`
[cuda_dispatch(name = "kv_tq4_store_b_cls", grid = "wgs", params = "wgs : int64")]
class CuKvTq4StoreB : KvTq4StoreT {
    override TOKROW = false

    [cuda_kernel(local_size_x = 256, precise = true, name = "kv_tq4_store_b_cls_cuda")]
    def run {
        store_body()
    }
}

//! tq4's un-rotation of the attention rows on `GkBlkSignedMax`
[cuda_dispatch(name = "kv_tq4_unrot_cls", grid = "wgs", params = "wgs : int64")]
class CuKvTq4Unrot : KvTq4UnrotT {
    [cuda_kernel(local_size_x = 256, precise = true, name = "kv_tq4_unrot_cls_cuda")]
    def run {
        unrot_body()
    }
}

//! the fused rope and K/V store of the token command (`RopeKvStoreT` over the shared `GkRopeKvStore`)
[cuda_dispatch(name = "rope_kv_cls", grid = "wgs", params = "wgs : int64")]
class CuRopeKvStore : RopeKvStoreT {
    typedef KT = float
    override CLAMP16 = false

    [cuda_kernel(local_size_x = 256, name = "rope_kv_cls_cuda")]
    def run {
        rope_kv_body()
    }
}

//! the f16-mirror stamp of the token command's rope and K/V store on `GkRopeKvStore`, the stores clamped
[cuda_dispatch(name = "rope_kv_f16_cls", grid = "wgs", params = "wgs : int64")]
class CuRopeKvStoreF16 : RopeKvStoreT {
    typedef KT = float16

    [cuda_kernel(local_size_x = 256, name = "rope_kv_f16_cls_cuda")]
    def run {
        rope_kv_body()
    }
}

//! the fused per-head q/k norm, rope and K/V store of the token command and, under `win`, of the window (`QknRopeKvT` over
//! the shared `GkQknRopeKv`): a q/k-normed model's one rope pass on this home, the token rows' and the window's forms alike
[cuda_dispatch(name = "qkn_rope_cls", grid = "wgs", params = "wgs : int64")]
class CuQknRopeKv : QknRopeKvT {
    typedef KT = float
    override CLAMP16 = false

    [cuda_kernel(local_size_x = 256, name = "qkn_rope_cls_cuda")]
    def run {
        qkn_rope_body()
    }
}

//! the f16-mirror stamp of the fused q/k norm, rope and K/V store, the stores clamped - `QknRopeKvT`
[cuda_dispatch(name = "qkn_rope_f16_cls", grid = "wgs", params = "wgs : int64")]
class CuQknRopeKvF16 : QknRopeKvT {
    typedef KT = float16

    [cuda_kernel(local_size_x = 256, name = "qkn_rope_f16_cls_cuda")]
    def run {
        qkn_rope_body()
    }
}

//! the window's rope and K/V store (`RopeKvStoreBT`)
[cuda_dispatch(name = "rope_kv_b_cls", grid = "wgs", params = "wgs : int64")]
class CuRopeKvStoreB : RopeKvStoreBT {
    typedef KT = float
    override CLAMP16 = false

    [cuda_kernel(local_size_x = 256, name = "rope_kv_b_cls_cuda")]
    def run {
        rope_kv_b_body()
    }
}

[cuda_dispatch(name = "rope_kv_b_f16_cls", grid = "wgs", params = "wgs : int64")]
class CuRopeKvStoreBF16 : RopeKvStoreBT {
    typedef KT = float16

    [cuda_kernel(local_size_x = 256, name = "rope_kv_b_f16_cls_cuda")]
    def run {
        rope_kv_b_body()
    }
}

//! the GEMV grid over `rows` rows: `lanes_per_row` 0 is a whole warp a row (the Vulkan home's `gemv_wgs` over this warp)
def cud_gemv_wgs(rows : int64; lanes_per_row : uint) : int64 => gemv_wgs_of(rows, lanes_per_row, g_cud.subgroup_size, g_cud.rows_per_wg)

[cuda_dispatch(name = "kq_gemv_k4_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class CuKqGemvK4 : K4GemvT {
    [cuda_kernel(local_size_x = 64, name = "kq_gemv_k4_cls_cuda")]
    def run {
        leaf_body()
    }
}

[cuda_dispatch(name = "kq_gemv_q40_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class CuKqGemvQ40 : Q40GemvT {
    [cuda_kernel(local_size_x = 64, name = "kq_gemv_q40_cls_cuda")]
    def run {
        leaf_body()
    }
}

//! the IQ4_NL codebook decode of a word of four nibble lanes through `__byte_perm`: the shared `iq4_word` picks a codebook word
//! by a dynamic vector index, which NVCC lowers to a local-memory array - eight such reads a word, and the iq4xs GEMV ran at
//! a third of Vulkan's rate on the wide planes. Here each nibble's three low bits select a byte of the codebook's lower or
//! upper half in one permute each, and the fourth bit picks between the halves
//! four bytes picked by `sel` out of the pair (a0, a1) where `m`'s byte is clear and out of (b0, b1) where it is set: one codebook half or table a byte
[spirv_inline]
def prmt_pick(sel, a0, a1, b0, b1, m : uint) : uint => (byte_perm(a0, a1, sel) & ~m) | (byte_perm(b0, b1, sel) & m)

[spirv_inline]
def cud_iq4_word(n : uint) : uint {
    let sel = (n & 7u) | ((n >> 4u) & 0x70u) | ((n >> 8u) & 0x700u) | ((n >> 12u) & 0x7000u)
    return prmt_pick(sel, IQ4NL_WORD0, IQ4NL_WORD1, IQ4NL_WORD2, IQ4NL_WORD3, ((n >> 3u) & 0x01010101u) * 0xFFu)
}

//! the codebook GEMV templates on the permute decode: home-only templates under the shared leaf shape, the shared
//! templates' `iq4_word_of` hook overridden (`ARCHITECTURE_GPU_SHARED_KERNELS.md#gpu-shared-kernels`)
[ |> template_struct_instance]
class template Iq4xsGemvCudT : Iq4xsGemvT {
    def override iq4_word_of(n : uint) : uint => cud_iq4_word(n)
}

[ |> template_struct_instance]
class template Iq4nlGemvCudT : Iq4nlGemvT {
    def override iq4_word_of(n : uint) : uint => cud_iq4_word(n)
}

[ |> template_struct_instance]
class template Iq4nl32GemvCudT : Blk32GemvRunT {
    typedef FMT = KqTag_iq4nl32

    def override iq4_word_of(n : uint) : uint => cud_iq4_word(n)
}

[cuda_dispatch(name = "kq_gemv_iq4xs_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class CuKqGemvIq4xs : Iq4xsGemvCudT {
    [cuda_kernel(local_size_x = 64, name = "kq_gemv_iq4xs_cls_cuda")]
    def run {
        leaf_body()
    }
}

[cuda_dispatch(name = "kq_gemv_iq3s4_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class CuKqGemvIq3s4 : Iq4xsGemvT {
    override IQODD = true

    [cuda_kernel(local_size_x = 64, name = "kq_gemv_iq3s4_cls_cuda")]
    def run {
        leaf_body()
    }
}

[cuda_dispatch(name = "kq_gemv_iq3s_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class CuKqGemvIq3s : Iq3sGemvT {
    [cuda_kernel(local_size_x = 64, name = "kq_gemv_iq3s_cls_cuda")]
    def run {
        leaf_body()
    }
}

[cuda_dispatch(name = "kq_gemv_iq3xxs_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class CuKqGemvIq3xxs : Iq3xxsGemvT {
    [cuda_kernel(local_size_x = 64, name = "kq_gemv_iq3xxs_cls_cuda")]
    def run {
        leaf_body()
    }
}

[cuda_dispatch(name = "kq_gemv_iq4nl_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class CuKqGemvIq4nl : Iq4nlGemvCudT {
    [cuda_kernel(local_size_x = 64, name = "kq_gemv_iq4nl_cls_cuda")]
    def run {
        leaf_body()
    }
}

//! eight nibble codes to signed int8 e2m1 lanes through `byte_perm`; the selectors mask each sign bit, since a selector nibble's high bit is prmt's sign-replicate mode
def nv4_prmt_split(w : uint; var lo, hi : uint&) {
    let s = w & 0x77777777u
    let p0 = byte_perm(MX4_BYTES_LO, MX4_BYTES_HI, s)
    let p1 = byte_perm(MX4_BYTES_LO, MX4_BYTES_HI, s >> 16u)
    let n0 = byte_perm(MX4_NEG_BYTES_LO, MX4_NEG_BYTES_HI, s)
    let n1 = byte_perm(MX4_NEG_BYTES_LO, MX4_NEG_BYTES_HI, s >> 16u)
    lo = prmt_pick(0x6420u, p0, p1, n0, n1, ((w >> 3u) & 0x01010101u) * 0xFFu)
    hi = prmt_pick(0x7531u, p0, p1, n0, n1, ((w >> 7u) & 0x01010101u) * 0xFFu)
}

//! the nv4 decode GEMV leaf on the CUDA home: the nibble decode through `byte_perm`, the block scales the shared leaf's
[ |> template_struct_instance]
class template Nv4GemvPrmtT : Nv4GemvT {
    def override e2m1_split(w : uint; var lo, hi : uint&) {
        nv4_prmt_split(w, lo, hi)
    }
}

//! the nv4 decode GEMV: `Nv4GemvT` through the home's permute leaf
[cuda_dispatch(name = "kq_gemv_nv4_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class CuKqGemvNv4 : Nv4GemvPrmtT {
    [cuda_kernel(local_size_x = 64, name = "kq_gemv_nv4_cls_cuda")]
    def run {
        leaf_body()
    }
}

[cuda_dispatch(name = "kq_gemv_k3_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class CuKqGemvK3 : K3GemvT {
    [cuda_kernel(local_size_x = 64, name = "kq_gemv_k3_cls_cuda")]
    def run {
        leaf_body()
    }
}

[cuda_dispatch(name = "kq_gemv_k2_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class CuKqGemvK2 : K2GemvT {
    [cuda_kernel(local_size_x = 64, name = "kq_gemv_k2_cls_cuda")]
    def run {
        leaf_body()
    }
}

[cuda_dispatch(name = "kq_gemv_k5_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class CuKqGemvK5 : K5GemvT {
    [cuda_kernel(local_size_x = 64, name = "kq_gemv_k5_cls_cuda")]
    def run {
        leaf_body()
    }
}

[cuda_dispatch(name = "kq_gemv_k6_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class CuKqGemvK6 : K6GemvT {
    [cuda_kernel(local_size_x = 64, name = "kq_gemv_k6_cls_cuda")]
    def run {
        leaf_body()
    }
}

[cuda_dispatch(name = "kq_gemv_iq2s_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class CuKqGemvIq2s : Iq2sGemvT {
    [cuda_kernel(local_size_x = 64, name = "kq_gemv_iq2s_cls_cuda")]
    def run {
        leaf_body()
    }
}

[cuda_dispatch(name = "kq_gemv_iq2xs_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class CuKqGemvIq2xs : Iq2xsGemvT {
    [cuda_kernel(local_size_x = 64, name = "kq_gemv_iq2xs_cls_cuda")]
    def run {
        leaf_body()
    }
}

[cuda_dispatch(name = "kq_gemv_iq2xxs_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class CuKqGemvIq2xxs : Iq2xxsGemvT {
    [cuda_kernel(local_size_x = 64, name = "kq_gemv_iq2xxs_cls_cuda")]
    def run {
        leaf_body()
    }
}

[cuda_dispatch(name = "kq_gemv_k4_n_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class CuKqGemvK4N : K4GemvT {
    override NCOL = true

    [cuda_kernel(local_size_x = 64, name = "kq_gemv_k4_n_cls_cuda")]
    def run {
        leaf_body()
    }
}

[cuda_dispatch(name = "kq_gemv_q40_n_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class CuKqGemvQ40N : Q40GemvT {
    override NCOL = true

    [cuda_kernel(local_size_x = 64, name = "kq_gemv_q40_n_cls_cuda")]
    def run {
        leaf_body()
    }
}

[cuda_dispatch(name = "kq_gemv_iq4xs_n_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class CuKqGemvIq4xsN : Iq4xsGemvCudT {
    override NCOL = true

    [cuda_kernel(local_size_x = 64, name = "kq_gemv_iq4xs_n_cls_cuda")]
    def run {
        leaf_body()
    }
}

[cuda_dispatch(name = "kq_gemv_iq3s4_n_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class CuKqGemvIq3s4N : Iq4xsGemvT {
    override IQODD = true
    override NCOL = true

    [cuda_kernel(local_size_x = 64, name = "kq_gemv_iq3s4_n_cls_cuda")]
    def run {
        leaf_body()
    }
}

[cuda_dispatch(name = "kq_gemv_iq3s_n_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class CuKqGemvIq3sN : Iq3sGemvT {
    override NCOL = true

    [cuda_kernel(local_size_x = 64, name = "kq_gemv_iq3s_n_cls_cuda")]
    def run {
        leaf_body()
    }
}

[cuda_dispatch(name = "kq_gemv_iq3xxs_n_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class CuKqGemvIq3xxsN : Iq3xxsGemvT {
    override NCOL = true

    [cuda_kernel(local_size_x = 64, name = "kq_gemv_iq3xxs_n_cls_cuda")]
    def run {
        leaf_body()
    }
}

[cuda_dispatch(name = "kq_gemv_iq4nl_n_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class CuKqGemvIq4nlN : Iq4nlGemvCudT {
    override NCOL = true

    [cuda_kernel(local_size_x = 64, name = "kq_gemv_iq4nl_n_cls_cuda")]
    def run {
        leaf_body()
    }
}

//! its N-column stamp: `Nv4GemvT` through the home's permute leaf
[cuda_dispatch(name = "kq_gemv_nv4_n_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class CuKqGemvNv4N : Nv4GemvPrmtT {
    override NCOL = true

    [cuda_kernel(local_size_x = 64, name = "kq_gemv_nv4_n_cls_cuda")]
    def run {
        leaf_body()
    }
}

[cuda_dispatch(name = "kq_gemv_k3_n_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class CuKqGemvK3N : K3GemvT {
    override NCOL = true

    [cuda_kernel(local_size_x = 64, name = "kq_gemv_k3_n_cls_cuda")]
    def run {
        leaf_body()
    }
}

[cuda_dispatch(name = "kq_gemv_k2_n_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class CuKqGemvK2N : K2GemvT {
    override NCOL = true

    [cuda_kernel(local_size_x = 64, name = "kq_gemv_k2_n_cls_cuda")]
    def run {
        leaf_body()
    }
}

[cuda_dispatch(name = "kq_gemv_k5_n_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class CuKqGemvK5N : K5GemvT {
    override NCOL = true

    [cuda_kernel(local_size_x = 64, name = "kq_gemv_k5_n_cls_cuda")]
    def run {
        leaf_body()
    }
}

[cuda_dispatch(name = "kq_gemv_k6_n_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class CuKqGemvK6N : K6GemvT {
    override NCOL = true

    [cuda_kernel(local_size_x = 64, name = "kq_gemv_k6_n_cls_cuda")]
    def run {
        leaf_body()
    }
}

[cuda_dispatch(name = "kq_gemv_iq2s_n_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class CuKqGemvIq2sN : Iq2sGemvT {
    override NCOL = true

    [cuda_kernel(local_size_x = 64, name = "kq_gemv_iq2s_n_cls_cuda")]
    def run {
        leaf_body()
    }
}

[cuda_dispatch(name = "kq_gemv_iq2xs_n_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class CuKqGemvIq2xsN : Iq2xsGemvT {
    override NCOL = true

    [cuda_kernel(local_size_x = 64, name = "kq_gemv_iq2xs_n_cls_cuda")]
    def run {
        leaf_body()
    }
}

[cuda_dispatch(name = "kq_gemv_iq2xxs_n_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class CuKqGemvIq2xxsN : Iq2xxsGemvT {
    override NCOL = true

    [cuda_kernel(local_size_x = 64, name = "kq_gemv_iq2xxs_n_cls_cuda")]
    def run {
        leaf_body()
    }
}

[cuda_dispatch(name = "q51_gemv_cls", grid = "wgs", params = "wgs : int64")]
class CuQ51Gemv : Blk32GemvRunT {
    typedef FMT = KqTag_q51
    override Q51 = true

    [cuda_kernel(local_size_x = 64, name = "q51_gemv_cls_cuda")]
    def run {
        b32_body()
    }
}

[cuda_dispatch(name = "q51_gemv_n_cls", grid = "wgs", params = "wgs : int64")]
class CuQ51GemvN : Blk32GemvRunT {
    typedef FMT = KqTag_q51
    override Q51 = true
    override NCOL = true

    [cuda_kernel(local_size_x = 64, name = "q51_gemv_n_cls_cuda")]
    def run {
        b32_body()
    }
}

//! the native-MXFP4 expert rail's decode GEMVs (`Blk32GemvRunT` under `MX4`): gpt-oss's routed stacks over Q8_0 activations, a
//! lane a block - the one per-32 form with no dense plane, so the ladders take it by name ahead of the per-32 arm
[cuda_dispatch(name = "mx4_gemv_cls", grid = "wgs", params = "wgs : int64")]
class CuMx4Gemv : Blk32GemvRunT {
    typedef FMT = KqTag_mx4
    override MX4 = true

    [cuda_kernel(local_size_x = 64, name = "mx4_gemv_cls_cuda")]
    def run {
        b32_body()
    }
}

//! the MXFP4 decode GEMV's N-column stamp (`Blk32GemvRunT`)
[cuda_dispatch(name = "mx4_gemv_n_cls", grid = "wgs", params = "wgs : int64")]
class CuMx4GemvN : Blk32GemvRunT {
    typedef FMT = KqTag_mx4
    override MX4 = true
    override NCOL = true

    [cuda_kernel(local_size_x = 64, name = "mx4_gemv_n_cls_cuda")]
    def run {
        b32_body()
    }
}

//! the MXFP4 expert stacks' gate + up dots, the act and its Q8_0 requant in one dispatch on the one-row step (`Mx4GemvGuT`): a
//! workgroup an activated block of the hidden rows, the gate and up bias rows joined under the slot's expert where the model carries them
[cuda_dispatch(name = "mx4_gemv_gu_cls", grid = "wgs", params = "wgs : int64")]
class CuMx4GemvGu : Mx4GemvGuT {
    [cuda_kernel(local_size_x = 256, name = "mx4_gemv_gu_cls_cuda"), arch(at="../ARCHITECTURE_GPU_SHARED_GEMM.md#moe-gate-up-fused")]
    def run {
        gu_body()
    }
}

[cuda_dispatch(name = "iq4nl32_gemv_n_cls", grid = "wgs", params = "wgs : int64")]
class CuIq4nl32GemvN : Iq4nl32GemvCudT {
    override NCOL = true

    [cuda_kernel(local_size_x = 64, name = "iq4nl32_gemv_n_cls_cuda")]
    def run {
        b32_body()
    }
}

[cuda_dispatch(name = "q4032_gemv_n_cls", grid = "wgs", params = "wgs : int64")]
class CuQ4032GemvN : Blk32GemvRunT {
    typedef FMT = KqTag_q4032
    override NCOL = true

    [cuda_kernel(local_size_x = 64, name = "q4032_gemv_n_cls_cuda")]
    def run {
        b32_body()
    }
}

[cuda_dispatch(name = "q5032_gemv_n_cls", grid = "wgs", params = "wgs : int64")]
class CuQ5032GemvN : Blk32GemvRunT {
    typedef FMT = KqTag_q5032
    override NCOL = true

    [cuda_kernel(local_size_x = 64, name = "q5032_gemv_n_cls_cuda")]
    def run {
        b32_body()
    }
}

[cuda_dispatch(name = "iq4nl32_gemv_cls", grid = "wgs", params = "wgs : int64")]
class CuIq4nl32Gemv : Iq4nl32GemvCudT {
    [cuda_kernel(local_size_x = 64, name = "iq4nl32_gemv_cls_cuda")]
    def run {
        b32_body()
    }
}

[cuda_dispatch(name = "q4032_gemv_cls", grid = "wgs", params = "wgs : int64")]
class CuQ4032Gemv : Blk32GemvRunT {
    typedef FMT = KqTag_q4032

    [cuda_kernel(local_size_x = 64, name = "q4032_gemv_cls_cuda")]
    def run {
        b32_body()
    }
}

[cuda_dispatch(name = "q5032_gemv_cls", grid = "wgs", params = "wgs : int64")]
class CuQ5032Gemv : Blk32GemvRunT {
    typedef FMT = KqTag_q5032

    [cuda_kernel(local_size_x = 64, name = "q5032_gemv_cls_cuda")]
    def run {
        b32_body()
    }
}

[cuda_dispatch(name = "kq_emb_k4_cls", kernel = "run", family = "kq_emb_cls", grid = "wgs", params = "wgs : int64")]
class CuKqEmbK4 : K4GemvT {
    override EMB = true

    [cuda_kernel(local_size_x = 64, name = "kq_emb_k4_cls_cuda")]
    def run {
        leaf_body()
    }
}

[cuda_dispatch(name = "kq_emb_q40_cls", kernel = "run", family = "kq_emb_cls", grid = "wgs", params = "wgs : int64")]
class CuKqEmbQ40 : Q40GemvT {
    override EMB = true

    [cuda_kernel(local_size_x = 64, name = "kq_emb_q40_cls_cuda")]
    def run {
        leaf_body()
    }
}

[cuda_dispatch(name = "kq_emb_iq4xs_cls", kernel = "run", family = "kq_emb_cls", grid = "wgs", params = "wgs : int64")]
class CuKqEmbIq4xs : Iq4xsGemvCudT {
    override EMB = true

    [cuda_kernel(local_size_x = 64, name = "kq_emb_iq4xs_cls_cuda")]
    def run {
        leaf_body()
    }
}

[cuda_dispatch(name = "kq_emb_iq3s4_cls", kernel = "run", family = "kq_emb_cls", grid = "wgs", params = "wgs : int64")]
class CuKqEmbIq3s4 : Iq4xsGemvT {
    override IQODD = true
    override EMB = true

    [cuda_kernel(local_size_x = 64, name = "kq_emb_iq3s4_cls_cuda")]
    def run {
        leaf_body()
    }
}

[cuda_dispatch(name = "kq_emb_iq3s_cls", kernel = "run", family = "kq_emb_cls", grid = "wgs", params = "wgs : int64")]
class CuKqEmbIq3s : Iq3sGemvT {
    override EMB = true

    [cuda_kernel(local_size_x = 64, name = "kq_emb_iq3s_cls_cuda")]
    def run {
        leaf_body()
    }
}

[cuda_dispatch(name = "kq_emb_iq3xxs_cls", kernel = "run", family = "kq_emb_cls", grid = "wgs", params = "wgs : int64")]
class CuKqEmbIq3xxs : Iq3xxsGemvT {
    override EMB = true

    [cuda_kernel(local_size_x = 64, name = "kq_emb_iq3xxs_cls_cuda")]
    def run {
        leaf_body()
    }
}

[cuda_dispatch(name = "kq_emb_iq4nl_cls", kernel = "run", family = "kq_emb_cls", grid = "wgs", params = "wgs : int64")]
class CuKqEmbIq4nl : Iq4nlGemvCudT {
    override EMB = true

    [cuda_kernel(local_size_x = 64, name = "kq_emb_iq4nl_cls_cuda")]
    def run {
        leaf_body()
    }
}

//! the nv4 token-table gather: `Nv4GemvT`'s decode over the table rows
[cuda_dispatch(name = "kq_emb_nv4_cls", kernel = "run", family = "kq_emb_cls", grid = "wgs", params = "wgs : int64")]
class CuKqEmbNv4 : Nv4GemvT {
    override EMB = true

    [cuda_kernel(local_size_x = 64, name = "kq_emb_nv4_cls_cuda")]
    def run {
        leaf_body()
    }
}

[cuda_dispatch(name = "kq_emb_k3_cls", kernel = "run", family = "kq_emb_cls", grid = "wgs", params = "wgs : int64")]
class CuKqEmbK3 : K3GemvT {
    override EMB = true

    [cuda_kernel(local_size_x = 64, name = "kq_emb_k3_cls_cuda")]
    def run {
        leaf_body()
    }
}

[cuda_dispatch(name = "kq_emb_k2_cls", kernel = "run", family = "kq_emb_cls", grid = "wgs", params = "wgs : int64")]
class CuKqEmbK2 : K2GemvT {
    override EMB = true

    [cuda_kernel(local_size_x = 64, name = "kq_emb_k2_cls_cuda")]
    def run {
        leaf_body()
    }
}

[cuda_dispatch(name = "kq_emb_k5_cls", kernel = "run", family = "kq_emb_cls", grid = "wgs", params = "wgs : int64")]
class CuKqEmbK5 : K5GemvT {
    override EMB = true

    [cuda_kernel(local_size_x = 64, name = "kq_emb_k5_cls_cuda")]
    def run {
        leaf_body()
    }
}

[cuda_dispatch(name = "kq_emb_k6_cls", kernel = "run", family = "kq_emb_cls", grid = "wgs", params = "wgs : int64")]
class CuKqEmbK6 : K6GemvT {
    override EMB = true

    [cuda_kernel(local_size_x = 64, name = "kq_emb_k6_cls_cuda")]
    def run {
        leaf_body()
    }
}

[cuda_dispatch(name = "kq_emb_iq2s_cls", kernel = "run", family = "kq_emb_cls", grid = "wgs", params = "wgs : int64")]
class CuKqEmbIq2s : Iq2sGemvT {
    override EMB = true

    [cuda_kernel(local_size_x = 64, name = "kq_emb_iq2s_cls_cuda")]
    def run {
        leaf_body()
    }
}

[cuda_dispatch(name = "kq_emb_iq2xs_cls", kernel = "run", family = "kq_emb_cls", grid = "wgs", params = "wgs : int64")]
class CuKqEmbIq2xs : Iq2xsGemvT {
    override EMB = true

    [cuda_kernel(local_size_x = 64, name = "kq_emb_iq2xs_cls_cuda")]
    def run {
        leaf_body()
    }
}

[cuda_dispatch(name = "kq_emb_iq2xxs_cls", kernel = "run", family = "kq_emb_cls", grid = "wgs", params = "wgs : int64")]
class CuKqEmbIq2xxs : Iq2xxsGemvT {
    override EMB = true

    [cuda_kernel(local_size_x = 64, name = "kq_emb_iq2xxs_cls_cuda")]
    def run {
        leaf_body()
    }
}

[cuda_dispatch(name = "q51_emb_cls", kernel = "run", family = "b32_emb_cls", grid = "wgs", params = "wgs : int64")]
class CuQ51Emb : Blk32GemvRunT {
    typedef FMT = KqTag_q51
    override Q51 = true
    override EMB = true

    [cuda_kernel(local_size_x = 64, name = "q51_emb_cls_cuda")]
    def run {
        b32_body()
    }
}

[cuda_dispatch(name = "iq4nl32_emb_cls", kernel = "run", family = "b32_emb_cls", grid = "wgs", params = "wgs : int64")]
class CuIq4nl32Emb : Iq4nl32GemvCudT {
    override EMB = true

    [cuda_kernel(local_size_x = 64, name = "iq4nl32_emb_cls_cuda")]
    def run {
        b32_body()
    }
}

[cuda_dispatch(name = "q4032_emb_cls", kernel = "run", family = "b32_emb_cls", grid = "wgs", params = "wgs : int64")]
class CuQ4032Emb : Blk32GemvRunT {
    typedef FMT = KqTag_q4032
    override EMB = true

    [cuda_kernel(local_size_x = 64, name = "q4032_emb_cls_cuda")]
    def run {
        b32_body()
    }
}

[cuda_dispatch(name = "q5032_emb_cls", kernel = "run", family = "b32_emb_cls", grid = "wgs", params = "wgs : int64")]
class CuQ5032Emb : Blk32GemvRunT {
    typedef FMT = KqTag_q5032
    override EMB = true

    [cuda_kernel(local_size_x = 64, name = "q5032_emb_cls_cuda")]
    def run {
        b32_body()
    }
}

//! the format the CUDA home's ladders stamp no leaf for: mx4, an expert-only form with no dense plane on the slice; the table gather's
//! q8 arm is `EmbGather`, outside the ladder
let private CUD_FMT_SKIP = "mx4"
let private CUD_EMB_SKIP = "q8 mx4"
//! the ladders' arms by plane shape: a superblock leaf binds the grid codebooks as a seventh plane, a per-32 leaf six (`!b32`: every format but the dense per-32 ones)
let private CUD_SKIP_KQ = "q8 q51 mx4 iq4nl32 q4032 q5032"
let private CUD_SKIP_B32 = "!b32"

var private g_cud_kq_grid = 0ul
var private g_cud_kq_grid_epoch = -1

//! the grid codebooks on the device, the kq leaves' seventh binding: uploaded once a device epoch, freed by the shutdown
def cud_kq_grid() : uint64 {
    if (g_cud_kq_grid == 0ul || g_cud_kq_grid_epoch != cud_epoch()) {
        var words <- kq_grid_words()
        g_cud_kq_grid = buf_upload(words, g_cud.stream)
        g_cud_kq_grid_epoch = cud_epoch()
        cud_proc_buf(g_cud_kq_grid)
    }
    return g_cud_kq_grid
}

//! the decode GEMV leaf of format `fmt` loads
def cud_gemv_ensure(fmt : int) : bool {   // nolint:STYLE037 - the stamp's ladder, one arm per KqFmt member
    return ensure_mx4_gemv_cls() if (fmt == int(KqFmt.mx4))
    var r = false
    kq_tile_stamp(fmt, CUD_FMT_SKIP, "gemv") <| $() {
        r = tile_ensure_cls()
    }
    return r
}

//! the int8 MMQ leaf of `fmt` compiled, Q8_0's for a format with no leaf of its own
def cud_mmq_ensure(fmt : int) : bool {
    if (fmt == int(KqFmt.k4)) {
        return ensure_k4_mmq()
    } elif (fmt == int(KqFmt.iq4xs)) {
        return ensure_iq4xs_mmq()
    } elif (fmt == int(KqFmt.q40)) {
        return ensure_q40_mmq()
    } elif (fmt == int(KqFmt.iq4nl)) {
        return ensure_iq4nl_mmq()
    } elif (fmt == int(KqFmt.q51)) {
        return ensure_q51_mmq()
    } elif (fmt == int(KqFmt.q4032)) {
        return ensure_q4032_mmq()
    } elif (fmt == int(KqFmt.q5032)) {
        return ensure_q5032_mmq()
    } elif (fmt == int(KqFmt.iq4nl32)) {
        return ensure_iq4nl32_mmq()
    }
    return ensure_q8_mmq()
}

//! one dispatch of the int8 MMQ leaf of `fmt`; `bufs` is [wq, ws, xq, xs, y, xsum] - a leaf without a min term binds the first five
def cud_mmq_enc(fmt : int; var s : void?; bufs : uint64 const[6]; var pc : MmqArgs; wgs : int64) {
    var b5 = fixed_array(bufs[0], bufs[1], bufs[2], bufs[3], bufs[4])
    if (fmt == int(KqFmt.k4)) {
        enc_k4_mmq(s, bufs, pc, wgs)
    } elif (fmt == int(KqFmt.q51)) {
        enc_q51_mmq(s, bufs, pc, wgs)
    } elif (fmt == int(KqFmt.iq4xs)) {
        enc_iq4xs_mmq(s, b5, pc, wgs)
    } elif (fmt == int(KqFmt.q40)) {
        enc_q40_mmq(s, b5, pc, wgs)
    } elif (fmt == int(KqFmt.iq4nl)) {
        enc_iq4nl_mmq(s, b5, pc, wgs)
    } elif (fmt == int(KqFmt.q4032)) {
        enc_q4032_mmq(s, b5, pc, wgs)
    } elif (fmt == int(KqFmt.q5032)) {
        enc_q5032_mmq(s, b5, pc, wgs)
    } elif (fmt == int(KqFmt.iq4nl32)) {
        enc_iq4nl32_mmq(s, b5, pc, wgs)
    } else {
        enc_q8_mmq(s, b5, pc, wgs)
    }
}

//! `bufs` is the family's [wq, ws, regions, xq, xs, y]; a kq leaf gets the grid codebooks appended as its seventh
def cud_gemv_cls_enc(fmt : int; var s : void?; bufs : uint64 const[6]; var pc : GemvArgs; wgs : int64) {   // nolint:STYLE037 — kq_tile_stamp's generated arms, one a format
    if (fmt == int(KqFmt.q8)) {
        enc_q8_gemv_cls(s, bufs, pc, wgs)
    } elif (fmt == int(KqFmt.mx4)) {
        enc_mx4_gemv_cls(s, bufs, pc, wgs)
    } elif (kq_sb(fmt)) {
        var kb = fixed_array(bufs[0], bufs[1], bufs[2], bufs[3], bufs[4], bufs[5], cud_kq_grid())
        kq_tile_stamp(fmt, CUD_SKIP_KQ, "gemv") <| $() {
            tile_enc_cls(s, kb, pc, wgs)
        }
    } else {
        kq_tile_stamp(fmt, CUD_SKIP_B32, "gemv") <| $() {
            tile_enc_cls(s, bufs, pc, wgs)
        }
    }
}

//! the token-table gather of format `fmt` loads: q8's `EmbGather`, a superblock format's `KqEmb<Fmt>` leaf, a per-32 format's `<Fmt>Emb`
def cud_emb_ensure(fmt : int) : bool {   // nolint:STYLE037 - the stamp's ladder, one arm per KqFmt member
    return ensure_emb_gather_cls() if (fmt == int(KqFmt.q8))
    var r = false
    kq_tile_stamp(fmt, CUD_EMB_SKIP, "emb") <| $() {
        r = tile_ensure_cls()
    }
    return r
}

//! row r of `y` is the table row `ids[r]`, or the host's staged row in `xh` where the id is `EMB_ID_HOST`
def cud_emb_enc(fmt : int; var s : void?; wq, ws, ids, xh, y : uint64; npos, dim, wblk0 : int64; escale : float) {   // nolint:STYLE037 — kq_tile_stamp's generated arms, one a format
    if (fmt == int(KqFmt.q8)) {
        var pce = EmbArgs(npos = uint(npos), dim = uint(dim), wblk0 = uint(wblk0), embed_scale = escale)
        enc_emb_gather_cls(s, fixed_array(wq, ws, ids, y, xh), pce, (npos * dim / 4l + 255l) / 256l)
        return
    }
    var pc = GemvArgs(n = uint(dim), nreg = uint(npos), wblk0 = uint(wblk0), escale = escale)
    let wgs = (npos * (dim / KQ_B32_ELEMS) + int64(CUD_WG_X) - 1l) / int64(CUD_WG_X)
    if (kq_sb(fmt)) {
        var kb = fixed_array(wq, ws, ids, ids, xh, y, cud_kq_grid())
        kq_tile_stamp(fmt, CUD_SKIP_KQ, "emb") <| $() {
            tile_enc_cls(s, kb, pc, wgs)
        }
    } else {
        var bb = fixed_array(wq, ws, ids, ids, xh, y)
        kq_tile_stamp(fmt, CUD_SKIP_B32, "emb") <| $() {
            tile_enc_cls(s, bb, pc, wgs)
        }
    }
}

//! the N-column leaf of format `fmt` over `pc.ncols` activation rows (1 to GEMV_COL_TILE): q8's stamp by the count, a kq leaf
//! with the grid codebooks as its seventh plane, a per-32 leaf over the family's six
def private cud_gemv_cls_enc_n(fmt : int; var s : void?; bufs : uint64 const[6]; var pc : GemvArgs; wgs : int64) {   // nolint:STYLE037 — kq_tile_stamp's generated arms, one a format
    if (fmt == int(KqFmt.q8)) {
        if (pc.ncols <= 2u) {
            enc_q8_gemv_n2_cls(s, bufs, pc, wgs)
        } elif (pc.ncols <= 4u) {
            enc_q8_gemv_n4_cls(s, bufs, pc, wgs)
        } else {
            enc_q8_gemv_n8_cls(s, bufs, pc, wgs)
        }
    } elif (fmt == int(KqFmt.mx4)) {
        enc_mx4_gemv_n_cls(s, bufs, pc, wgs)
    } elif (kq_sb(fmt)) {
        var kb = fixed_array(bufs[0], bufs[1], bufs[2], bufs[3], bufs[4], bufs[5], cud_kq_grid())
        kq_tile_stamp(fmt, CUD_SKIP_KQ, "gemv") <| $() {
            tile_enc_n_cls(s, kb, pc, wgs)
        }
    } else {
        kq_tile_stamp(fmt, CUD_SKIP_B32, "gemv") <| $() {
            tile_enc_n_cls(s, bufs, pc, wgs)
        }
    }
}

//! `pc.ncols` past one tiles the N-column leaf by GEMV_COL_TILE, the y rows `pc.ystride` apart (`ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-col-tiles`)
def cud_gemv_enc(fmt : int; var s : void?; bufs : uint64 const[6]; var pc : GemvArgs; rows : int64) {
    pc.lanes_per_row = gemv_lanes_per_row(fmt, int64(pc.n))
    let wgs = cud_gemv_wgs(rows, pc.lanes_per_row)
    if (pc.ncols <= 1u) {
        cud_gemv_cls_enc(fmt, s, bufs, pc, wgs)
        return
    }
    each_col_tile(pc, pc.ncols, GEMV_COL_TILE) $ {
        cud_gemv_cls_enc_n(fmt, s, bufs, pc, wgs)
    }
}

//! the fused gate-up stamp over `pc.ncols` rows in tiles of GU_N_COLS: the narrowest stamp that holds a tile's columns
def cud_gu_n_enc(var s : void?; bufs : uint64 const[8]; var pc : GuNArgs; wgs : int64) {
    each_col_tile(pc, pc.ncols, GU_N_COLS) $ {
        if (pc.ncols <= 1u) {
            enc_q8_gemv_gu_n1_cls(s, bufs, pc, wgs)
        } elif (pc.ncols <= 2u) {
            enc_q8_gemv_gu_n2_cls(s, bufs, pc, wgs)
        } elif (pc.ncols <= 4u) {
            enc_q8_gemv_gu_n4_cls(s, bufs, pc, wgs)
        } else {
            enc_q8_gemv_gu_n8_cls(s, bufs, pc, wgs)
        }
    }
}

//! the Q8_0 cm2 l tile (`Q8Cm2T` over the shared 128 x 256 workgroup fragments): the window's GEMM over the f16 activation panel
[cuda_dispatch(name = "q8_batch_cm2l_cls", grid = "wgs", params = "wgs : int64")]
class CuQ8Cm2L : Q8Cm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256

    [cuda_kernel(local_size_x = 256, min_blocks = 2, name = "q8_batch_cm2l_cls_cuda")]   //! two blocks an SM at 128 registers; the down shape gives a little back
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "q8_batch_cm2m_cls", grid = "wgs", params = "wgs : int64")]
class CuQ8Cm2M : Q8Cm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [cuda_kernel(local_size_x = 256, min_blocks = 2, name = "q8_batch_cm2m_cls_cuda")]   //! two blocks an SM at 128 registers under the pipelined decode
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "q8_batch_cm2s_cls", grid = "wgs", params = "wgs : int64")]
class CuQ8Cm2S : Q8Cm2T {
    override BN = 32u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32

    [cuda_kernel(local_size_x = 256, name = "q8_batch_cm2s_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "kq_batch_k4_cm2l_cls", grid = "wgs", params = "wgs : int64")]
class CuK4Cm2L : K4Cm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256

    //! two blocks an SM: unbounded, the l tile takes 174 registers and one block, and the gate/up shape runs a quarter slower
    [cuda_kernel(local_size_x = 256, min_blocks = 2, name = "kq_batch_k4_cm2l_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "kq_batch_k4_cm2m_cls", grid = "wgs", params = "wgs : int64")]
class CuK4Cm2M : K4Cm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [cuda_kernel(local_size_x = 256, name = "kq_batch_k4_cm2m_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "kq_batch_k4_cm2s_cls", grid = "wgs", params = "wgs : int64")]
class CuK4Cm2S : K4Cm2T {
    override BN = 32u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32

    [cuda_kernel(local_size_x = 256, name = "kq_batch_k4_cm2s_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "kq_batch_q40_cm2l_cls", grid = "wgs", params = "wgs : int64")]
class CuQ40Cm2L : Q40Cm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256

    [cuda_kernel(local_size_x = 256, min_blocks = 2, name = "kq_batch_q40_cm2l_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "kq_batch_q40_cm2m_cls", grid = "wgs", params = "wgs : int64")]
class CuQ40Cm2M : Q40Cm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [cuda_kernel(local_size_x = 256, name = "kq_batch_q40_cm2m_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "kq_batch_q40_cm2s_cls", grid = "wgs", params = "wgs : int64")]
class CuQ40Cm2S : Q40Cm2T {
    override BN = 32u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32

    [cuda_kernel(local_size_x = 256, name = "kq_batch_q40_cm2s_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "kq_batch_k5_cm2l_cls", grid = "wgs", params = "wgs : int64")]
class CuK5Cm2L : K5Cm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256

    [cuda_kernel(local_size_x = 256, min_blocks = 2, name = "kq_batch_k5_cm2l_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "kq_batch_k5_cm2m_cls", grid = "wgs", params = "wgs : int64")]
class CuK5Cm2M : K5Cm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [cuda_kernel(local_size_x = 256, name = "kq_batch_k5_cm2m_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "kq_batch_k5_cm2s_cls", grid = "wgs", params = "wgs : int64")]
class CuK5Cm2S : K5Cm2T {
    override BN = 32u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32

    [cuda_kernel(local_size_x = 256, name = "kq_batch_k5_cm2s_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "kq_batch_k6_cm2l_cls", grid = "wgs", params = "wgs : int64")]
class CuK6Cm2L : K6Cm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256

    //! two blocks an SM, as the k4 l leaf
    [cuda_kernel(local_size_x = 256, min_blocks = 2, name = "kq_batch_k6_cm2l_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "kq_batch_k6_cm2m_cls", grid = "wgs", params = "wgs : int64")]
class CuK6Cm2M : K6Cm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [cuda_kernel(local_size_x = 256, name = "kq_batch_k6_cm2m_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "kq_batch_k6_cm2s_cls", grid = "wgs", params = "wgs : int64")]
class CuK6Cm2S : K6Cm2T {
    override BN = 32u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32

    [cuda_kernel(local_size_x = 256, name = "kq_batch_k6_cm2s_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "kq_batch_iq4xs_cm2l_cls", grid = "wgs", params = "wgs : int64")]
class CuIq4xsCm2L : Iq4xsCm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256

    [cuda_kernel(local_size_x = 256, min_blocks = 2, name = "kq_batch_iq4xs_cm2l_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "kq_batch_iq4xs_cm2m_cls", grid = "wgs", params = "wgs : int64")]
class CuIq4xsCm2M : Iq4xsCm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [cuda_kernel(local_size_x = 256, name = "kq_batch_iq4xs_cm2m_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "kq_batch_iq4xs_cm2s_cls", grid = "wgs", params = "wgs : int64")]
class CuIq4xsCm2S : Iq4xsCm2T {
    override BN = 32u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32

    [cuda_kernel(local_size_x = 256, name = "kq_batch_iq4xs_cm2s_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "kq_batch_k3_cm2l_cls", grid = "wgs", params = "wgs : int64")]
class CuK3Cm2L : K3Cm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256

    [cuda_kernel(local_size_x = 256, min_blocks = 2, name = "kq_batch_k3_cm2l_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "kq_batch_k3_cm2m_cls", grid = "wgs", params = "wgs : int64")]
class CuK3Cm2M : K3Cm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [cuda_kernel(local_size_x = 256, name = "kq_batch_k3_cm2m_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "kq_batch_k3_cm2s_cls", grid = "wgs", params = "wgs : int64")]
class CuK3Cm2S : K3Cm2T {
    override BN = 32u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32

    [cuda_kernel(local_size_x = 256, name = "kq_batch_k3_cm2s_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "kq_batch_iq3s_cm2l_cls", grid = "wgs", params = "wgs : int64")]
class CuIq3sCm2L : Iq3sCm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256

    [cuda_kernel(local_size_x = 256, min_blocks = 2, name = "kq_batch_iq3s_cm2l_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "kq_batch_iq3s_cm2m_cls", grid = "wgs", params = "wgs : int64")]
class CuIq3sCm2M : Iq3sCm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [cuda_kernel(local_size_x = 256, name = "kq_batch_iq3s_cm2m_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "kq_batch_iq3s_cm2s_cls", grid = "wgs", params = "wgs : int64")]
class CuIq3sCm2S : Iq3sCm2T {
    override BN = 32u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32

    [cuda_kernel(local_size_x = 256, name = "kq_batch_iq3s_cm2s_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "kq_batch_iq3xxs_cm2l_cls", grid = "wgs", params = "wgs : int64")]
class CuIq3xxsCm2L : Iq3xxsCm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256

    [cuda_kernel(local_size_x = 256, min_blocks = 2, name = "kq_batch_iq3xxs_cm2l_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "kq_batch_iq3xxs_cm2m_cls", grid = "wgs", params = "wgs : int64")]
class CuIq3xxsCm2M : Iq3xxsCm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [cuda_kernel(local_size_x = 256, name = "kq_batch_iq3xxs_cm2m_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "kq_batch_iq3xxs_cm2s_cls", grid = "wgs", params = "wgs : int64")]
class CuIq3xxsCm2S : Iq3xxsCm2T {
    override BN = 32u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32

    [cuda_kernel(local_size_x = 256, name = "kq_batch_iq3xxs_cm2s_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "kq_batch_iq4nl_cm2l_cls", grid = "wgs", params = "wgs : int64")]
class CuIq4nlCm2L : Iq4nlCm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256

    [cuda_kernel(local_size_x = 256, min_blocks = 2, name = "kq_batch_iq4nl_cm2l_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "kq_batch_iq4nl_cm2m_cls", grid = "wgs", params = "wgs : int64")]
class CuIq4nlCm2M : Iq4nlCm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [cuda_kernel(local_size_x = 256, name = "kq_batch_iq4nl_cm2m_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "kq_batch_iq4nl_cm2s_cls", grid = "wgs", params = "wgs : int64")]
class CuIq4nlCm2S : Iq4nlCm2T {
    override BN = 32u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32

    [cuda_kernel(local_size_x = 256, name = "kq_batch_iq4nl_cm2s_cls_cuda")]
    def run {
        cm2_body()
    }
}

//! the nv4 cm2 l tile: `Nv4Cm2T`
[cuda_dispatch(name = "kq_batch_nv4_cm2l_cls", grid = "wgs", params = "wgs : int64")]
class CuNv4Cm2L : Nv4Cm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256

    [cuda_kernel(local_size_x = 256, min_blocks = 2, name = "kq_batch_nv4_cm2l_cls_cuda")]
    def run {
        cm2_body()
    }
}

//! the nv4 cm2 m tile: `Nv4Cm2T`
[cuda_dispatch(name = "kq_batch_nv4_cm2m_cls", grid = "wgs", params = "wgs : int64")]
class CuNv4Cm2M : Nv4Cm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [cuda_kernel(local_size_x = 256, name = "kq_batch_nv4_cm2m_cls_cuda")]
    def run {
        cm2_body()
    }
}

//! the nv4 cm2 s tile: `Nv4Cm2T`
[cuda_dispatch(name = "kq_batch_nv4_cm2s_cls", grid = "wgs", params = "wgs : int64")]
class CuNv4Cm2S : Nv4Cm2T {
    override BN = 32u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32

    [cuda_kernel(local_size_x = 256, name = "kq_batch_nv4_cm2s_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "kq_batch_k2_cm2l_cls", grid = "wgs", params = "wgs : int64")]
class CuK2Cm2L : K2Cm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256

    [cuda_kernel(local_size_x = 256, min_blocks = 2, name = "kq_batch_k2_cm2l_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "kq_batch_k2_cm2m_cls", grid = "wgs", params = "wgs : int64")]
class CuK2Cm2M : K2Cm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [cuda_kernel(local_size_x = 256, name = "kq_batch_k2_cm2m_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "kq_batch_k2_cm2s_cls", grid = "wgs", params = "wgs : int64")]
class CuK2Cm2S : K2Cm2T {
    override BN = 32u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32

    [cuda_kernel(local_size_x = 256, name = "kq_batch_k2_cm2s_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "kq_batch_iq2s_cm2l_cls", grid = "wgs", params = "wgs : int64")]
class CuIq2sCm2L : Iq2sCm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256

    [cuda_kernel(local_size_x = 256, min_blocks = 2, name = "kq_batch_iq2s_cm2l_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "kq_batch_iq2s_cm2m_cls", grid = "wgs", params = "wgs : int64")]
class CuIq2sCm2M : Iq2sCm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [cuda_kernel(local_size_x = 256, name = "kq_batch_iq2s_cm2m_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "kq_batch_iq2s_cm2s_cls", grid = "wgs", params = "wgs : int64")]
class CuIq2sCm2S : Iq2sCm2T {
    override BN = 32u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32

    [cuda_kernel(local_size_x = 256, name = "kq_batch_iq2s_cm2s_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "kq_batch_iq2xs_cm2l_cls", grid = "wgs", params = "wgs : int64")]
class CuIq2xsCm2L : Iq2xsCm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256

    [cuda_kernel(local_size_x = 256, min_blocks = 2, name = "kq_batch_iq2xs_cm2l_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "kq_batch_iq2xs_cm2m_cls", grid = "wgs", params = "wgs : int64")]
class CuIq2xsCm2M : Iq2xsCm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [cuda_kernel(local_size_x = 256, name = "kq_batch_iq2xs_cm2m_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "kq_batch_iq2xs_cm2s_cls", grid = "wgs", params = "wgs : int64")]
class CuIq2xsCm2S : Iq2xsCm2T {
    override BN = 32u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32

    [cuda_kernel(local_size_x = 256, name = "kq_batch_iq2xs_cm2s_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "kq_batch_iq2xxs_cm2l_cls", grid = "wgs", params = "wgs : int64")]
class CuIq2xxsCm2L : Iq2xxsCm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256

    [cuda_kernel(local_size_x = 256, min_blocks = 2, name = "kq_batch_iq2xxs_cm2l_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "kq_batch_iq2xxs_cm2m_cls", grid = "wgs", params = "wgs : int64")]
class CuIq2xxsCm2M : Iq2xxsCm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [cuda_kernel(local_size_x = 256, name = "kq_batch_iq2xxs_cm2m_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "kq_batch_iq2xxs_cm2s_cls", grid = "wgs", params = "wgs : int64")]
class CuIq2xxsCm2S : Iq2xxsCm2T {
    override BN = 32u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32

    [cuda_kernel(local_size_x = 256, name = "kq_batch_iq2xxs_cm2s_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "kq_batch_iq3s4_cm2l_cls", grid = "wgs", params = "wgs : int64")]
class CuIq3s4Cm2L : Iq3s4Cm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256

    [cuda_kernel(local_size_x = 256, min_blocks = 2, name = "kq_batch_iq3s4_cm2l_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "kq_batch_iq3s4_cm2m_cls", grid = "wgs", params = "wgs : int64")]
class CuIq3s4Cm2M : Iq3s4Cm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [cuda_kernel(local_size_x = 256, name = "kq_batch_iq3s4_cm2m_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "kq_batch_iq3s4_cm2s_cls", grid = "wgs", params = "wgs : int64")]
class CuIq3s4Cm2S : Iq3s4Cm2T {
    override BN = 32u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32

    [cuda_kernel(local_size_x = 256, name = "kq_batch_iq3s4_cm2s_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "q51_batch_cm2l_cls", grid = "wgs", params = "wgs : int64")]
class CuQ51Cm2L : Q51Cm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256

    [cuda_kernel(local_size_x = 256, min_blocks = 2, name = "q51_batch_cm2l_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "q51_batch_cm2m_cls", grid = "wgs", params = "wgs : int64")]
class CuQ51Cm2M : Q51Cm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [cuda_kernel(local_size_x = 256, name = "q51_batch_cm2m_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "iq4nl32_batch_cm2l_cls", grid = "wgs", params = "wgs : int64")]
class CuIq4nl32Cm2L : Iq4nl32Cm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256

    [cuda_kernel(local_size_x = 256, min_blocks = 2, name = "iq4nl32_batch_cm2l_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "iq4nl32_batch_cm2m_cls", grid = "wgs", params = "wgs : int64")]
class CuIq4nl32Cm2M : Iq4nl32Cm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [cuda_kernel(local_size_x = 256, name = "iq4nl32_batch_cm2m_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "q4032_batch_cm2l_cls", grid = "wgs", params = "wgs : int64")]
class CuQ4032Cm2L : Q4032Cm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256

    [cuda_kernel(local_size_x = 256, min_blocks = 2, name = "q4032_batch_cm2l_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "q4032_batch_cm2m_cls", grid = "wgs", params = "wgs : int64")]
class CuQ4032Cm2M : Q4032Cm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [cuda_kernel(local_size_x = 256, name = "q4032_batch_cm2m_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "q5032_batch_cm2l_cls", grid = "wgs", params = "wgs : int64")]
class CuQ5032Cm2L : Q5032Cm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256

    [cuda_kernel(local_size_x = 256, min_blocks = 2, name = "q5032_batch_cm2l_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "q5032_batch_cm2m_cls", grid = "wgs", params = "wgs : int64")]
class CuQ5032Cm2M : Q5032Cm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [cuda_kernel(local_size_x = 256, name = "q5032_batch_cm2m_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "q51_batch_cm2s_cls", grid = "wgs", params = "wgs : int64")]
class CuQ51Cm2S : Q51Cm2T {
    override BN = 32u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32

    [cuda_kernel(local_size_x = 256, name = "q51_batch_cm2s_cls_cuda")]
    def run {
        cm2_body()
    }
}

//! the mx4 rail's s column (`Mx4Cm2T`): the expert schedule's 32-row pieces (gpt-oss's routed stacks; a dense mx4 plane never exists)
[cuda_dispatch(name = "mx4_batch_cm2s_cls", grid = "wgs", params = "wgs : int64")]
class CuMx4Cm2S : Mx4Cm2T {
    override BN = 32u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32

    [cuda_kernel(local_size_x = 256, name = "mx4_batch_cm2s_cls_cuda")]
    def run {
        cm2_body()
    }
}

//! ... and its e column (`Mx4Cm2T`): the schedule's 128-row pieces
[cuda_dispatch(name = "mx4_batch_cm2e_cls", grid = "wgs", params = "wgs : int64")]
class CuMx4Cm2E : Mx4Cm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [cuda_kernel(local_size_x = 256, name = "mx4_batch_cm2e_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "iq4nl32_batch_cm2s_cls", grid = "wgs", params = "wgs : int64")]
class CuIq4nl32Cm2S : Iq4nl32Cm2T {
    override BN = 32u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32

    [cuda_kernel(local_size_x = 256, name = "iq4nl32_batch_cm2s_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "q4032_batch_cm2s_cls", grid = "wgs", params = "wgs : int64")]
class CuQ4032Cm2S : Q4032Cm2T {
    override BN = 32u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32

    [cuda_kernel(local_size_x = 256, name = "q4032_batch_cm2s_cls_cuda")]
    def run {
        cm2_body()
    }
}

[cuda_dispatch(name = "q5032_batch_cm2s_cls", grid = "wgs", params = "wgs : int64")]
class CuQ5032Cm2S : Q5032Cm2T {
    override BN = 32u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32

    [cuda_kernel(local_size_x = 256, name = "q5032_batch_cm2s_cls_cuda")]
    def run {
        cm2_body()
    }
}

//! the columns no tile serves: mx4 is an expert-only form, so it has the expert schedule's s and e columns and no dense l or m tile
let private CUD_CM2_SKIP = "mx4/cm2l_cls mx4/cm2m_cls"

//! the format slots the skip list leaves the s column alone to: both their l and m stamps are skipped (`<fmt>/cm2l_cls <fmt>/cm2m_cls`)
def private cm2_s_only_slots(skip : string) : table<int> {
    var seen : table<int; int>
    for (tok in split(skip, " ")) {
        let cut = find(tok, "/")
        continue if (cut < 0)
        var f = KqFmt.q8
        continue if (!kq_fmt_of_name(slice(tok, 0, cut), f))
        let stamp = slice(tok, cut + 1)
        seen[int(f)] |= stamp == "cm2l_cls" ? 1 : (stamp == "cm2m_cls" ? 2 : 0)
    }
    var out : table<int>
    for (slot, mask in keys(seen), values(seen)) {
        if (mask == 3) {
            out |> insert(slot)
        }
    }
    return <- out
}

let private CUD_CM2_S_ONLY <- cm2_s_only_slots(CUD_CM2_SKIP)

//! whether format slot `fmt`'s cm2 tiles stop at the s column (`CUD_CM2_SKIP` names no l or m stamp for it): a window dispatches no column that was never stamped
def cud_cm2_s_only(fmt : int) : bool => key_exists(CUD_CM2_S_ONLY, fmt)

let CUD_CM2_TILES = "cm2l_cls cm2m_cls cm2s_cls cm2e_cls"   //! e is the expert schedule's m pieces at the format's own k step (the m stamp on the formats whose step is 64)
let CUD_CM2_TC_L = 0
let CUD_CM2_TC_M = 1
let CUD_CM2_TC_S = 2
let CUD_CM2_TC_E = 3   //! `tc` of the e column

//! the cm2 tile of format `fmt` at column `tc` loads
def cud_cm2_ensure(fmt, tc : int) : bool {   // nolint:STYLE037 — kq_tile_stamp's generated arms, one a format's column
    var r = false
    kq_tile_stamp(fmt, CUD_CM2_SKIP, "batch", tc, CUD_CM2_TILES) <| $() {
        r = tile_ensure()
    }
    return r
}

//! `bufs` is [wq, ws, schedule, xf, y]
def cud_cm2_enc(fmt, tc : int; var s : void?; bufs : uint64 const[5]; var pc : BatchArgs; wgs : int64) {   // nolint:STYLE037 — kq_tile_stamp's generated arms, one a format's column
    kq_tile_stamp(fmt, CUD_CM2_SKIP, "batch", tc, CUD_CM2_TILES) <| $() {
        tile_enc(s, bufs, pc, wgs)
    }
}

//! the iq3_s e column (`Iq3sCm2T`)
[cuda_dispatch(name = "kq_batch_iq3s_cm2e_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuIq3sCm2E : Iq3sCm2T {
    override BN = 128u
    override STILE = true
    override BK = 32u
    override UNR = 8u
    typedef AT = coopmatWgA_f16_128x32
    typedef BT = coopmatWgB_f16_32x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [cuda_kernel(local_size_x = 256, name = "kq_batch_iq3s_cm2e_cls_cuda")]
    def run {
        cm2_body()
    }
}

//! the iq3_xxs e column (`Iq3xxsCm2T`)
[cuda_dispatch(name = "kq_batch_iq3xxs_cm2e_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuIq3xxsCm2E : Iq3xxsCm2T {
    override BN = 128u
    override STILE = true
    override BK = 32u
    override UNR = 8u
    typedef AT = coopmatWgA_f16_128x32
    typedef BT = coopmatWgB_f16_32x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [cuda_kernel(local_size_x = 256, name = "kq_batch_iq3xxs_cm2e_cls_cuda")]
    def run {
        cm2_body()
    }
}

//! the iq2_s e column (`Iq2sCm2T`)
[cuda_dispatch(name = "kq_batch_iq2s_cm2e_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuIq2sCm2E : Iq2sCm2T {
    override BN = 128u
    override STILE = true
    override BK = 32u
    override UNR = 8u
    typedef AT = coopmatWgA_f16_128x32
    typedef BT = coopmatWgB_f16_32x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [cuda_kernel(local_size_x = 256, name = "kq_batch_iq2s_cm2e_cls_cuda")]
    def run {
        cm2_body()
    }
}

//! the iq2_xs e column (`Iq2xsCm2T`)
[cuda_dispatch(name = "kq_batch_iq2xs_cm2e_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuIq2xsCm2E : Iq2xsCm2T {
    override BN = 128u
    override STILE = true
    override BK = 32u
    override UNR = 8u
    typedef AT = coopmatWgA_f16_128x32
    typedef BT = coopmatWgB_f16_32x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [cuda_kernel(local_size_x = 256, name = "kq_batch_iq2xs_cm2e_cls_cuda")]
    def run {
        cm2_body()
    }
}

//! the iq2_xxs e column (`Iq2xxsCm2T`)
[cuda_dispatch(name = "kq_batch_iq2xxs_cm2e_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuIq2xxsCm2E : Iq2xxsCm2T {
    override BN = 128u
    override STILE = true
    override BK = 32u
    override UNR = 8u
    typedef AT = coopmatWgA_f16_128x32
    typedef BT = coopmatWgB_f16_32x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [cuda_kernel(local_size_x = 256, name = "kq_batch_iq2xxs_cm2e_cls_cuda")]
    def run {
        cm2_body()
    }
}

struct MmqArgs {
    n : uint      //! reduction width, a multiple of 256 (eight blocks a scale load)
    d : uint      //! the weight rows (the output width), a multiple of 128
    rows : uint   //! the tokens, a multiple of 128
    wblk : uint   //! the plane's first Q8_0 block, a multiple of 8 (eight a quad)
}

//! the MMQ grid: a block a 128 x 128 tile of (weight rows x tokens)
def public mmq_wgs(d, rows : int64) : int64 => (d / 128l) * (rows / 128l)

//! the Q8_0 x int8-activation GEMM in the MMQ shape, the CUDA home's own template over the `cuda_intrinsics` tier
//! (ARCHITECTURE_GPU_SHARED_KERNELS.md, the pairs that stay per home): a block of sixteen warps owns a
//! 128 x 128 tile of (weight rows x tokens) and walks k 128 wide - four Q8_0 blocks a step - with the raw quant blocks of
//! both operands staged by cp.async in a two-deep ring at 144-byte rows (eight uint4 and a pad, so ldmatrix's eight rows
//! hit eight bank groups), the next step's copies in flight under this step's products, the fragments through ldmatrix, the int8
//! mma, and the s32 product of each block scaled by its weight and activation scales into the f32 accumulators; the
//! scales ride registers a 16-byte quad at a time (eight weight halves, four activation floats) and land in shared
//! memory in the lane layouts the fixup reads; a warp owns 32 rows x 32 tokens (two row fragments by two column
//! fragments), sixteen warps the tile; a format joins by staging its blocks decoded to int8 in the weight tile's slot
[ |> template_struct_instance]
class template MmqT {
    //! every leaf names a format tag `FMT`: a per-32 leaf's (`B32`) picks its block words, scale word and decode through the `blk32_*` family, the others' is unread
    @template_constant K4 : bool = false      //!< the weight side: Q4_K superblocks decoded to int8 through registers, a scale and a min a 32-block
    @template_constant LUT : bool = false     //!< ... a codebook superblock format (IQ4_XS, IQ4_NL): the nibbles through the codebook to signed int8, a scale a 32-block, no min
    @template_constant QSC : bool = false     //!< the superblock scale row is Q4_0's (five words of d pairs: Q4_0, IQ4_NL); off, IQ4_XS's two words or Q4_K's five
    @template_constant KQ : bool = false      //!< a superblock leaf (K4, LUT or the Q4_0 superblock leaf): the scale rows as words, the weight stage through registers
    @template_constant B32 : bool = false     //!< a per-32 leaf (`FMT`: Q5_1, Q4_0, Q5_0, IQ4_NL): a block's words through registers, a scale word a block; off every stage axis, raw Q8_0 blocks by cp.async
    @template_constant WORDS : bool = false   //!< the scale plane as words (`wsu`: every leaf but Q8_0's halves)
    @template_constant MIN : bool = false     //!< the leaf's blocks carry a min (Q4_K's dmin x mn, Q5_1's m): the activation sums feed the rank-sixteen correction
    @ssbo @binding = 0 @readonly @role = "weight" wq4 : array<uint4>    //! Q8_0 blocks as two uint4: (row, kb) at (row * nb + kb) * 2; Q4_K and IQ4_XS superblocks as eight, block g's 16 nibble bytes at (row * nsb + sb) * 8 + g
    @ssbo @binding = 0 @readonly @role = "weight" @template_gate = B32 wq : array<uint>        //! the same plane as words: a per-32 block's four nibble words and, on Q5_1 and Q5_0, its qh word
    @ssbo @binding = 1 @readonly @template_gate = "!WORDS" ws4 : array<uint4>    //! weight scales as halves, (row, kb) at row * nb + kb
    @ssbo @binding = 1 @readonly @role = "weight" @template_gate = WORDS wsu : array<uint>        //! the scale rows: Q4_K's five words a superblock - d | dmin, eight sc bytes, eight mn bytes; IQ4_XS's two - d with the scales' high bits, the scales' low nibbles; a per-32 format's word a block or a pair
    @ssbo @binding = 2 @readonly xq4 : array<uint4>    //! the activation quants, the same shape over tokens
    @ssbo @binding = 3 @readonly xs4 : array<uint4>    //! activation scales as floats, (token, kb) at token * nb + kb
    @ssbo @binding = 4 y : array<float>                //! [token][d]
    @ssbo @binding = 5 @readonly @template_gate = MIN xsum4 : array<uint4>   //! the activation blocks' scale x quant sum, the shape of xs4
    @kargs pa : MmqArgs
    @workgroup sa : uint4[2304]     //! two stages x 128 rows x nine uint4 (four blocks and a pad)
    @workgroup sb : uint4[2304]
    @workgroup sws : float2[512]    //! row-scale pairs: slot (r >> 4) * 8 + (r & 7) holds rows g, g + 8
    @workgroup sxs : float4[256]    //! token-scale quads: slot (c >> 4) * 4 + ((c & 7) >> 1)
    @workgroup @template_gate = MIN rm16 : uint4[512]    //! the rows' min terms as halves (-dmin x mn, or m): [group][row][16 blocks], two groups of four steps in turn
    @workgroup @template_gate = MIN cx16 : uint4[512]    //! the tokens' scale x sum as halves: [group][token][16 blocks]

    //! a quad of IQ4_XS nibble lanes through the codebook to signed int8 lanes, the home's permute decode a word
    def iq4_quad(n : uint4) : uint4 => uint4(cud_iq4_word(n.x), cud_iq4_word(n.y), cud_iq4_word(n.z), cud_iq4_word(n.w))

    //! a superblock leaf's 32-block half `sh` (0: elements 0..15, 4: 16..31) as the int8 codes its format means off the block's nibble quad
    def sb_codes(w : uint4; sh : uint) : uint4 {
        static_if (K4) {
            return blk32_nib_quad(w, sh)
        } elif (LUT) {
            return iq4_quad(blk32_nib_quad(w, sh))
        } else {
            return blk32_q40_quad(w, sh)
        }
    }

    //! a per-32 block's half `sh` as signed int8 lanes: the format's decode, IQ4_NL's through the home's permute
    def b32_codes(w4 : uint4; qh, sh : uint) : uint4 {
        static_if (typeinfo typename(type<FMT>) == typeinfo typename(type<KqTag_iq4nl32>)) {
            return iq4_quad(blk32_nib_quad(w4, sh))
        } else {
            return blk32_quad(type<FMT>, w4, qh, sh)
        }
    }

    //! block `wb`'s four nibble words and its qh word (the first nibble word again where the format has none)
    def b32_words(wb : uint; var w4 : uint4&; var qh : uint&) {
        static_if (B32) {
            let b = wb * blk32_qwords(type<FMT>)
            w4 = uint4(wq[b], wq[b + 1u], wq[b + 2u], wq[b + 3u])
            qh = wq[b + blk32_qh_off(type<FMT>)]
        } else {
            w4 = uint4(0u)
            qh = 0u
        }
    }

    //! block `wb`'s (scale, min) pair off the scale plane
    def b32_scale(wb : uint) : float2 {
        static_if (B32) {
            return blk32_scale(type<FMT>, wsu[blk32_sword(type<FMT>, wb)], wb)
        } else {
            return float2(0.0)
        }
    }

    [spirv_inline, arch(at="../ARCHITECTURE_GPU_ASYMMETRIES_CUDA.md#cuda-asymmetries")]
    def mmq_body {   // nolint:STYLE037,STYLE038 - the pipeline and its unrolled products, register- and barrier-coupled; the Q4_K arm's stage and scale rows ride the same barriers
        let nb = pa.n / 32u
        let nb4 = pa.n / 128u
        let nsb = pa.n / 256u
        let ttiles = pa.rows / 128u
        let tix = gl_WorkGroupID.x
        let m0 = (tix / ttiles) * 128u
        let t0 = (tix % ttiles) * 128u
        let tid = gl_LocalInvocationID.x
        let warp = tid >> 5u
        let lane = tid & 31u
        let wm = warp >> 2u
        let wn = warp & 3u
        let g = lane >> 2u
        let t = lane & 3u
        let mt = lane >> 3u
        let mr = lane & 7u
        // the staging walk: a warp's eight-lane groups each walk one row's whole 128-byte step, so a warp instruction gathers
        // four contiguous rows (four wavefronts, the floor) and each group's eight 16-byte stores land on eight consecutive
        // bank groups; the second copy is the row 64 down
        let srow = (warp << 2u) + (lane >> 3u)
        let sj = lane & 7u
        // thread w of 128 writes word w of its stage's scale slots (consecutive words, no bank conflict); w names the row or
        // column it carries
        let wi = tid & 127u
        let wr = ((wi >> 4u) << 4u) + ((wi & 1u) << 3u) + ((wi >> 1u) & 7u)   // the row of slot wi >> 1, component wi & 1
        let wslot = wi >> 1u
        let wcomp = wi & 1u
        let xc = ((wi >> 4u) << 4u) + (((wi & 3u) >> 1u) << 3u) + (((wi >> 2u) & 3u) << 1u) + (wi & 1u)   // the column of slot wi >> 2, component wi & 3
        let xslot = wi >> 2u
        let xcomp = wi & 3u
        let wstage = tid < 128u
        let xstage = tid >= 128u && tid < 256u
        let nb8 = nb / 8u
        var f00, f01, f10, f11 : coopmatAcc_f32_16x16
        // the scales ride registers a 16-byte quad at a time: a row's eight weight halves serve two steps, the next pair's
        // quad loaded into the registers the second step's read frees; a token's four activation floats serve one step,
        // the next step's quad loaded once this step's is stored
        let wsrc = ((m0 + srow) * nb + pa.wblk) * 2u + sj
        let xsrc = ((t0 + srow) * nb) * 2u + sj
        let wb8 = pa.wblk / 8u
        let src2 = nb * 128u   // the row 64 down, in uint4
        let sdst = srow * 9u + sj
        // the KQ and B32 leaves' weight stage goes through registers: a lane one 32-block of a row (16 nibble bytes in, two int8 quads out),
        // eight rows a warp, the whole tile in one copy; the next step's quad is loaded under this step's products
        let krow = (warp << 3u) + (lane >> 2u)
        let kblk = lane & 3u
        let ksrc = (pa.wblk + (m0 + krow) * nsb) * 8u + kblk   // the lane's quad of superblock 0: step kb adds (kb >> 1) * 8 + (kb & 1) * 4
        let kdst = krow * 9u + kblk * 2u
        var wnext : uint4
        var qhn = 0u
        let b32b0 = pa.wblk + (m0 + krow) * nb + kblk   // a per-32 leaf: the lane's block of step 0; step kb adds kb * 4
        static_if (KQ) {
            wnext = wq4[ksrc]
            sa[kdst] = sb_codes(wnext, 0u)
            sa[kdst + 1u] = sb_codes(wnext, 4u)
            if (nb4 > 1u) {
                wnext = wq4[ksrc + 4u]
            }
        } else {
            static_if (B32) {
                b32_words(b32b0, wnext, qhn)
                sa[kdst] = b32_codes(wnext, qhn, 0u)
                sa[kdst + 1u] = b32_codes(wnext, qhn, 4u)
                if (nb4 > 1u) {
                    b32_words(b32b0 + 4u, wnext, qhn)
                }
            } else {
                cp_async_16(unsafe(addr(sa[sdst])), unsafe(addr(wq4[wsrc])))
                cp_async_16(unsafe(addr(sa[sdst + 576u])), unsafe(addr(wq4[wsrc + src2])))
            }
        }
        cp_async_16(unsafe(addr(sb[sdst])), unsafe(addr(xq4[xsrc])))
        cp_async_16(unsafe(addr(sb[sdst + 576u])), unsafe(addr(xq4[xsrc + src2])))
        cp_async_commit()
        var wcur = uint4(0u)
        var xcur = uint4(0u)
        var xscur = uint4(0u)
        let sb_sc_words = (K4 || QSC) ? 5u : 2u
        var sb_sc_cur, sb_sc_next : uint[5]
        let ksrow = (pa.wblk + (m0 + wr) * nsb) * sb_sc_words
        var b32_sc_min : float2[4]
        let b32_row_blk0 = pa.wblk + (m0 + wr) * nb
        if (wstage) {
            static_if (KQ) {
                for [unroll] (j in range(5)) {
                    if (uint(j) < sb_sc_words) {
                        sb_sc_cur[j] = wsu[ksrow + uint(j)]
                    }
                }
                static_if (K4) {
                    let dm = unpackHalf2x16(sb_sc_cur[0])
                    var mn4 : float[4]
                    for [unroll] (b in range(4)) {
                        let sm = k4_scale_min(dm, sb_sc_cur[1], sb_sc_cur[3], uint(b))
                        sws[uint(b) * 64u + wslot][int(wcomp)] = sm.x
                        mn4[b] = -sm.y
                    }
                    rm16[wr * 2u][0] = packHalf2x16(float2(mn4[0], mn4[1]))   // step 0: group 0, piece 0, the first half of the piece
                    rm16[wr * 2u][1] = packHalf2x16(float2(mn4[2], mn4[3]))
                } elif (QSC) {
                    for [unroll] (b in range(4)) {
                        sws[uint(b) * 64u + wslot][int(wcomp)] = q40_d_of(sb_sc_cur[b >> 1], uint(b))
                    }
                } else {
                    let dx = unpackHalf2x16(sb_sc_cur[0]).x
                    for [unroll] (b in range(4)) {
                        sws[uint(b) * 64u + wslot][int(wcomp)] = dx * float(iq4xs_sc(sb_sc_cur[0], sb_sc_cur[1], uint(b)))
                    }
                }
                if (nsb > 1u) {
                    for [unroll] (j in range(5)) {
                        if (uint(j) < sb_sc_words) {
                            sb_sc_next[j] = wsu[ksrow + sb_sc_words + uint(j)]
                        }
                    }
                }
            } else {
                static_if (B32) {
                    for [unroll] (b in range(4)) {
                        b32_sc_min[b] = b32_scale(b32_row_blk0 + uint(b))
                        sws[uint(b) * 64u + wslot][int(wcomp)] = b32_sc_min[b].x
                    }
                    static_if (MIN) {
                        rm16[wr * 2u][0] = packHalf2x16(float2(b32_sc_min[0].y, b32_sc_min[1].y))
                        rm16[wr * 2u][1] = packHalf2x16(float2(b32_sc_min[2].y, b32_sc_min[3].y))
                    }
                    if (nb4 > 1u) {
                        for [unroll] (b in range(4)) {
                            b32_sc_min[b] = b32_scale(b32_row_blk0 + 4u + uint(b))
                        }
                    }
                } else {
                    wcur = ws4[wb8 + (m0 + wr) * nb8]
                    let h0 = unpackHalf2x16(wcur.x)
                    let h1 = unpackHalf2x16(wcur.y)
                    sws[wslot][int(wcomp)] = h0.x
                    sws[64u + wslot][int(wcomp)] = h0.y
                    sws[128u + wslot][int(wcomp)] = h1.x
                    sws[192u + wslot][int(wcomp)] = h1.y
                }
            }
        } elif (xstage) {
            xcur = xs4[((t0 + xc) * nb) / 4u]
            sxs[xslot][int(xcomp)] = uint_bits_to_float(xcur.x)
            sxs[32u + xslot][int(xcomp)] = uint_bits_to_float(xcur.y)
            sxs[64u + xslot][int(xcomp)] = uint_bits_to_float(xcur.z)
            sxs[96u + xslot][int(xcomp)] = uint_bits_to_float(xcur.w)
            static_if (MIN) {
                xscur = xsum4[((t0 + xc) * nb) / 4u]
                cx16[xc * 2u][0] = packHalf2x16(float2(uint_bits_to_float(xscur.x), uint_bits_to_float(xscur.y)))
                cx16[xc * 2u][1] = packHalf2x16(float2(uint_bits_to_float(xscur.z), uint_bits_to_float(xscur.w)))
            }
            if (nb4 > 1u) {
                xcur = xs4[((t0 + xc) * nb) / 4u + 1u]
                static_if (MIN) {
                    xscur = xsum4[((t0 + xc) * nb) / 4u + 1u]
                }
            }
        }
        var kb2 = 0u
        while (kb2 < nb4) {
            for [unroll] (q in range(2)) {
                let kb = kb2 + uint(q)
                cp_async_wait(0)   // this step's stage landed
                barrier()          // ... and every warp is done with the other stage, which the next copies overwrite
                let st = kb & 1u
                let nk = kb + 1u
                if (nk < nb4) {
                    let st2 = nk & 1u
                    let nsrc = nk * 8u
                    static_if (KQ) {
                        sa[st2 * 1152u + kdst] = sb_codes(wnext, 0u)
                        sa[st2 * 1152u + kdst + 1u] = sb_codes(wnext, 4u)
                        if (nk + 1u < nb4) {
                            wnext = wq4[ksrc + ((nk + 1u) >> 1u) * 8u + ((nk + 1u) & 1u) * 4u]
                        }
                    } else {
                        static_if (B32) {
                            sa[st2 * 1152u + kdst] = b32_codes(wnext, qhn, 0u)
                            sa[st2 * 1152u + kdst + 1u] = b32_codes(wnext, qhn, 4u)
                            if (nk + 1u < nb4) {
                                b32_words(b32b0 + (nk + 1u) * 4u, wnext, qhn)
                            }
                        } else {
                            cp_async_16(unsafe(addr(sa[st2 * 1152u + sdst])), unsafe(addr(wq4[wsrc + nsrc])))
                            cp_async_16(unsafe(addr(sa[st2 * 1152u + sdst + 576u])), unsafe(addr(wq4[wsrc + nsrc + src2])))
                        }
                    }
                    cp_async_16(unsafe(addr(sb[st2 * 1152u + sdst])), unsafe(addr(xq4[xsrc + nsrc])))
                    cp_async_16(unsafe(addr(sb[st2 * 1152u + sdst + 576u])), unsafe(addr(xq4[xsrc + nsrc + src2])))
                    if (wstage) {
                        static_if (KQ) {
                            if (q == 1) {   // step nk opens a superblock: the prefetched row takes over, the row after it is fetched a superblock ahead
                                for [unroll] (j in range(5)) {
                                    sb_sc_cur[j] = sb_sc_next[j]
                                }
                                if ((nk >> 1u) + 1u < nsb) {
                                    for [unroll] (j in range(5)) {
                                        if (uint(j) < sb_sc_words) {
                                            sb_sc_next[j] = wsu[ksrow + ((nk >> 1u) + 1u) * sb_sc_words + uint(j)]
                                        }
                                    }
                                }
                            }
                            static_if (K4) {
                                let dm = unpackHalf2x16(sb_sc_cur[0])
                                let sc4 = q == 1 ? sb_sc_cur[1] : sb_sc_cur[2]   // step nk's four blocks: the superblock's first four (nk even) or last four
                                let mnw = q == 1 ? sb_sc_cur[3] : sb_sc_cur[4]
                                var mn4 : float[4]
                                for [unroll] (b in range(4)) {
                                    let sm = k4_scale_min(dm, sc4, mnw, uint(b))
                                    sws[st2 * 256u + uint(b) * 64u + wslot][int(wcomp)] = sm.x
                                    mn4[b] = -sm.y
                                }
                                let me = (((nk >> 2u) & 1u) * 128u + wr) * 2u + ((nk >> 1u) & 1u)   // step nk's group, its row, the piece holding its four blocks
                                let mc = int((nk & 1u) * 2u)
                                rm16[me][mc] = packHalf2x16(float2(mn4[0], mn4[1]))
                                rm16[me][mc + 1] = packHalf2x16(float2(mn4[2], mn4[3]))
                            } elif (QSC) {
                                let b0 = (nk & 1u) * 4u   // step nk's four blocks of the superblock: their d pairs in words 2 .. 3 (nk odd) or 0 .. 1
                                for [unroll] (b in range(4)) {
                                    sws[st2 * 256u + uint(b) * 64u + wslot][int(wcomp)] = q40_d_of(sb_sc_cur[int((b0 + uint(b)) >> 1u)], b0 + uint(b))
                                }
                            } else {
                                let dx = unpackHalf2x16(sb_sc_cur[0]).x
                                let b0 = (nk & 1u) * 4u
                                for [unroll] (b in range(4)) {
                                    sws[st2 * 256u + uint(b) * 64u + wslot][int(wcomp)] = dx * float(iq4xs_sc(sb_sc_cur[0], sb_sc_cur[1], b0 + uint(b)))
                                }
                            }
                        } else {
                            static_if (B32) {
                                for [unroll] (b in range(4)) {
                                    sws[st2 * 256u + uint(b) * 64u + wslot][int(wcomp)] = b32_sc_min[b].x
                                }
                                static_if (MIN) {
                                    let me = (((nk >> 2u) & 1u) * 128u + wr) * 2u + ((nk >> 1u) & 1u)
                                    let mc = int((nk & 1u) * 2u)
                                    rm16[me][mc] = packHalf2x16(float2(b32_sc_min[0].y, b32_sc_min[1].y))
                                    rm16[me][mc + 1] = packHalf2x16(float2(b32_sc_min[2].y, b32_sc_min[3].y))
                                }
                                if (nk + 1u < nb4) {
                                    for [unroll] (b in range(4)) {
                                        b32_sc_min[b] = b32_scale(b32_row_blk0 + (nk + 1u) * 4u + uint(b))
                                    }
                                }
                            } else {
                                let h0 = unpackHalf2x16(q == 0 ? wcur.z : wcur.x)   // step nk's four halves: words 2 (nk & 1) .. of its pair's eight
                                let h1 = unpackHalf2x16(q == 0 ? wcur.w : wcur.y)
                                sws[st2 * 256u + wslot][int(wcomp)] = h0.x
                                sws[st2 * 256u + 64u + wslot][int(wcomp)] = h0.y
                                sws[st2 * 256u + 128u + wslot][int(wcomp)] = h1.x
                                sws[st2 * 256u + 192u + wslot][int(wcomp)] = h1.y
                                if (q == 0 && kb2 + 2u < nb4) {   // the quad's last word read: the next pair's quad takes its registers a step ahead
                                    wcur = ws4[wb8 + (m0 + wr) * nb8 + (kb2 >> 1u) + 1u]
                                }
                            }
                        }
                    } elif (xstage) {
                        sxs[st2 * 128u + xslot][int(xcomp)] = uint_bits_to_float(xcur.x)
                        sxs[st2 * 128u + 32u + xslot][int(xcomp)] = uint_bits_to_float(xcur.y)
                        sxs[st2 * 128u + 64u + xslot][int(xcomp)] = uint_bits_to_float(xcur.z)
                        sxs[st2 * 128u + 96u + xslot][int(xcomp)] = uint_bits_to_float(xcur.w)
                        static_if (MIN) {
                            let xe = (((nk >> 2u) & 1u) * 128u + xc) * 2u + ((nk >> 1u) & 1u)
                            let xcc = int((nk & 1u) * 2u)
                            cx16[xe][xcc] = packHalf2x16(float2(uint_bits_to_float(xscur.x), uint_bits_to_float(xscur.y)))
                            cx16[xe][xcc + 1] = packHalf2x16(float2(uint_bits_to_float(xscur.z), uint_bits_to_float(xscur.w)))
                        }
                        if (nk + 1u < nb4) {
                            xcur = xs4[((t0 + xc) * nb) / 4u + nk + 1u]
                            static_if (MIN) {
                                xscur = xsum4[((t0 + xc) * nb) / 4u + nk + 1u]
                            }
                        }
                    }
                }
                cp_async_commit()
                for [unroll] (s in range(4)) {
                    let su = uint(s)
                    let abase = st * 1152u + (wm * 32u + mr + 8u * (mt & 1u)) * 9u + su * 2u + (mt >> 1u)   // the lane's A row: matrices (rows lo, k lo), (rows hi, k lo), (rows lo, k hi), (rows hi, k hi)
                    let bbase = st * 1152u + (wn * 32u + mr + 8u * (mt >> 1u)) * 9u + su * 2u + (mt & 1u)   // the lane's B row: (n lo, k lo), (n lo, k hi), (n hi, k lo), (n hi, k hi)
                    var a0, a1 : coopmatA_s8_16x32
                    ldmatrix(a0, unsafe(addr(sa[abase])))
                    ldmatrix(a1, unsafe(addr(sa[abase + 144u])))
                    var b0, b1 : coopmatB_s8_32x16
                    ldmatrix(b0, unsafe(addr(sb[bbase])))
                    ldmatrix(b1, unsafe(addr(sb[bbase + 144u])))
                    let sw = st * 256u + su * 64u + wm * 16u + g
                    let sx = st * 128u + su * 32u + wn * 8u + t
                    let rs0 = sws[sw]
                    let rs1 = sws[sw + 8u]
                    let cs0 = sxs[sx]
                    let cs1 = sxs[sx + 4u]
                    var s00, s01, s10, s11 : coopmatAcc_s32_16x16
                    s00 = coopmatMulAdd(a0, b0, s00)   // nolint:STYLE011 - MulAdd seeds from its own zero-filled tile
                    mma_scale_acc(f00, s00, rs0, cs0)
                    s01 = coopmatMulAdd(a0, b1, s01)
                    mma_scale_acc(f01, s01, rs0, cs1)
                    s10 = coopmatMulAdd(a1, b0, s10)
                    mma_scale_acc(f10, s10, rs1, cs0)
                    s11 = coopmatMulAdd(a1, b1, s11)
                    mma_scale_acc(f11, s11, rs1, cs1)
                }
                static_if (MIN) {
                    if ((kb & 3u) == 3u) {
                        let mg = (kb >> 2u) & 1u
                        let ra = (mg * 128u + wm * 32u + mr + 8u * (mt & 1u)) * 2u + (mt >> 1u)   // the lane's A row: (rows lo, blocks lo) .. (rows hi, blocks hi)
                        let cb = (mg * 128u + wn * 32u + mr + 8u * (mt >> 1u)) * 2u + (mt & 1u)   // the lane's B row off the [token][blocks] tile
                        var am0, am1 : coopmatA_f16_16x16
                        ldmatrix(am0, unsafe(addr(rm16[ra])))
                        ldmatrix(am1, unsafe(addr(rm16[ra + 32u])))
                        var bm0, bm1 : coopmatB_f16_16x16
                        ldmatrix(bm0, unsafe(addr(cx16[cb])))
                        ldmatrix(bm1, unsafe(addr(cx16[cb + 32u])))
                        f00 = coopmatMulAdd(am0, bm0, f00)
                        f01 = coopmatMulAdd(am0, bm1, f01)
                        f10 = coopmatMulAdd(am1, bm0, f10)
                        f11 = coopmatMulAdd(am1, bm1, f11)
                    }
                }
            }
            kb2 += 2u
        }
        let d = int(pa.d)
        let ybase = int((t0 + wn * 32u) * pa.d + m0 + wm * 32u)   // column-major over [token][d]: a token's row is the fragment's column
        coopmatStore(f00, y, ybase, d, 1)
        coopmatStore(f01, y, ybase + 16 * d, d, 1)
        coopmatStore(f10, y, ybase + 16, d, 1)
        coopmatStore(f11, y, ybase + 16 * d + 16, d, 1)
    }
}

//! the Q8_0 leaf of the MMQ template: the window chain's GEMM under DASLLAMA_CUDA_MMQ
[cuda_dispatch(name = "q8_mmq", grid = "wgs", params = "wgs : int64")]
class CuQ8Mmq : MmqT {
    typedef FMT = KqTag_q4032

    [cuda_kernel(local_size_x = 512, name = "q8_mmq_cuda")]
    def run {
        mmq_body()
    }
}

struct MmqFp4Args {
    n : uint      //! reduction width, a multiple of 256 (a superblock the scale row); a `KTAIL` stamp takes any multiple of 64
    d : uint      //! the weight rows (the output width), a multiple of 128; a `MOE` stamp takes any width (its last tile stores under a guard)
    rows : uint   //! the tokens, a multiple of 128 (unread on a `MOE` stamp: the record's rows)
    wblk : uint   //! the plane's first superblock (an `MX` stamp: its first 32-block); unread on a `MOE` stamp (the record's)
    map_off : uint   //! a `MOE` stamp: the dispatch's per-workgroup map word offset in the schedule plane
}

//! a k4-tiled 32-block re-paired into the FP4 feed's (k, k + 8) byte order: the scaled mma needs only both operands under one permutation a strip
def nv4_fp4_pairs(w : uint4) : uint4 {
    let m = KQC_NIBBLE_MASK
    return uint4((w.x & m) | ((w.z & m) << 4u), (w.y & m) | ((w.w & m) << 4u),
        ((w.x >> 4u) & m) | (w.z & 0xF0F0F0F0u), ((w.y >> 4u) & m) | (w.w & 0xF0F0F0F0u))
}

//! NVFP4 on the FP4 tensor rung (`cuda_intrinsics`' block-scaled mma, sm_120a): the MMQ tile - a block of sixteen warps a
//! 128 x 128 tile of weight rows x tokens, k 128 a step - over raw e2m1 nibbles on both sides: the weights' k4-tiled 32-blocks
//! re-paired through registers into the feed's (k, k + 8) order, the activations' FP4 feed staged by cp.async, each side's
//! UE4M3 strip scales a word a k64 beside its row; no decode, no per-32 fold - the per-token row scale and the plane's
//! factor (the fifth word of its scale rows, read once a tile off the tile's first row: a tile never straddles two planes, since
//! every plane is whole 128-row tiles) land in the epilogue. A two-deep ring at 80-byte rows (four uint4 and a pad: eight rows
//! on eight bank groups)
[ |> template_struct_instance]
class template MmqFp4T {
    @template_constant MX : bool = false      //!< the MXFP4 form: the plane by 32-block (no superblock lattice), its UE8M0 byte a block read as the k64's pair, the feed's codes a UE8M0 a k32 block, the product through `mma_block_scaled_mx`; no per-tensor factor
    @template_constant KTAIL : bool = false   //!< a width off the 128 lattice (a multiple of 64): the last step's second k64 under a zero scale on both sides (the weight stage zero codes, the feed stage the next row's codes the zero scale annuls)
    @template_constant MOE : bool = false     //!< the expert schedule's form (`MmqT`'s): the workgroup's record names the plane's block base, the bucket's row base and count and the tile; a tile's columns past `d` stay unstored
    @ssbo @binding = 0 @readonly @role = "weight" wq4 : array<uint4>    //! the nv4 quant plane: superblock sb's 32-block g at sb * 8 + g; an `MX` stamp's mx4 plane: 32-block b at b
    @ssbo @binding = 1 @readonly @role = "weight" wsu : array<uint>     //! the nv4 scale rows: five words a superblock, word j < 4 the k64 strip j's four UE4M3 codes, word 4 the plane's per-tensor factor as f32 bits; an `MX` stamp's mx4 scale plane: a UE8M0 byte a block, four a word
    @ssbo @binding = 2 @readonly xq4 : array<uint4>    //! the FP4 feed: a token's k / 2 nibble bytes, (k, k + 8) pairs inside each 16
    @ssbo @binding = 3 @readonly xsw : array<uint>     //! ... its k / 64 scale words, four UE4M3 codes each (an `MX` stamp: two UE8M0 codes in the low bytes)
    @ssbo @binding = 4 @readonly xrs : array<float>    //! ... its row scale
    @ssbo @binding = 5 y : array<float>                //! [token][d]
    @ssbo @binding = 6 @readonly @template_gate = MOE sched : array<uint>   //! the expert schedule: 4-word records [wblk0, row0, cnt, wg0], the per-workgroup map at `pa.map_off`
    @kargs pa : MmqFp4Args
    @workgroup sa : uint4[1280]     //! two stages x 128 rows x five uint4 (a step's four 32-blocks and a pad)
    @workgroup sb : uint4[1280]
    @workgroup swa : uint[512]      //! two stages x 128 rows x the step's two k64 scale words
    @workgroup swb : uint[512]      //! ... the tokens'
    @workgroup @template_gate = MOE ystage : float[2048]   //! two warps' 32 x 32 tiles at a time, [token][d], the guarded store's stage

    //! the tile's plane block base: record `rid`'s on a `MOE` stamp, the args' else
    def tile_wblk(rid : uint) : uint {
        static_if (MOE) {
            return sched[rid * 4u]
        } else {
            return pa.wblk
        }
    }

    //! the tile's first weight row and first token row: on a `MOE` stamp record `rid`'s bucket row base and the tile among the
    //! bucket's `ceil(cnt / 128)` row tiles by the plane's `ceil(d / 128)` column tiles, else the grid's tile over `rows`
    def tile_base(rid : uint) : uint2 {
        static_if (MOE) {
            let rb = rid * 4u
            let ttiles = (sched[rb + 2u] + 127u) / 128u
            let lt = gl_WorkGroupID.x - sched[rb + 3u]
            return uint2((lt / ttiles) * 128u, sched[rb + 1u] + (lt % ttiles) * 128u)
        } else {
            let ttiles = pa.rows / 128u
            let tix = gl_WorkGroupID.x
            return uint2((tix / ttiles) * 128u, (tix % ttiles) * 128u)
        }
    }

    def is_past_row_blocks(kb, h, nb : uint) : bool => KTAIL && kb * 4u + h * 2u >= nb

    def wq_block_index(ksrc, kb : uint) : uint {
        static_if (MX) {
            return ksrc + kb * 4u
        } else {
            return ksrc + (kb >> 1u) * 8u + (kb & 1u) * 4u
        }
    }

    //! the lane's staged quad of step `kb`: the block's nibbles re-paired, zero past the row's blocks and past the plane's rows
    def w_stage(ksrc, kb, kblk, nb : uint; row_ok : bool) : uint4 => (!row_ok || is_past_row_blocks(kb, kblk >> 1u, nb)) ? uint4(0u) : nv4_fp4_pairs(wq4[wq_block_index(ksrc, kb)])

    //! the weight row's scale word of step `kb`'s k64 half `h`: an `MX` stamp's two UE8M0 bytes off the byte plane (the pair sits in one
    //! word: a row's first block is even), else the nv4 scale row's word; zero past the row's blocks and past the plane's rows
    def w_scale_word(wblk, row, wsrow, kb, h, nb : uint; row_ok : bool) : uint {
        if (!row_ok || is_past_row_blocks(kb, h, nb)) {
            return 0u
        }
        static_if (MX) {
            let bi = wblk + row * nb + kb * 4u + h * 2u
            return (wsu[bi >> 2u] >> ((bi & 3u) * 8u)) & 0xFFFFu
        } else {
            return wsu[wsrow + (kb >> 1u) * 5u + (kb & 1u) * 2u + h]
        }
    }

    //! the token row's scale word of step `kb`'s k64 half `h`, zero past the row's blocks
    def x_scale_word(xsrow, kb, h, nb : uint) : uint => is_past_row_blocks(kb, h, nb) ? 0u : xsw[xsrow + kb * 2u + h]

    [spirv_inline]
    def mmq_body {
        static_if (MOE) {   // the workgroup's record off the map; a workgroup past the schedule's end (the sentinel) does nothing, as one block
            let rid = sched[pa.map_off + gl_WorkGroupID.x]
            if (rid != SCHED_NONE) {
                mmq_tile(rid)
            }
        } else {
            mmq_tile(0u)
        }
    }

    //! the tile: the grid's (`rid` unread), or on a `MOE` stamp record `rid`'s
    [spirv_inline, arch(at="../ARCHITECTURE_GPU_ASYMMETRIES_CUDA.md#cuda-asymmetries")]
    def mmq_tile(rid : uint) {   // nolint:STYLE038 - the pipeline and its unrolled products, register- and barrier-coupled
        let nb = pa.n / 32u
        let nsb = pa.n / 256u
        let nb4 = KTAIL ? (nb + 3u) / 4u : pa.n / 128u
        let wblk = tile_wblk(rid)
        let tbase = tile_base(rid)
        let m0 = tbase.x
        let t0 = tbase.y
        let tid = gl_LocalInvocationID.x
        let warp = tid >> 5u
        let lane = tid & 31u
        let wm = warp >> 2u
        let wn = warp & 3u
        let g = lane >> 2u
        let t = lane & 3u
        let mt = lane >> 3u
        let mr = lane & 7u
        // the weight stage through registers: a lane one 32-block of a row (the k4 uint4 in, the re-paired uint4 out), eight rows
        // a warp; the activation stage by cp.async: the same lane map, a token's four uint4 of the step
        let krow = (warp << 3u) + (lane >> 2u)
        let kblk = lane & 3u
        let ksrc = MX ? wblk + (m0 + krow) * nb + kblk : (wblk + (m0 + krow) * nsb) * 8u + kblk   // the lane's block of step 0: `wq_block_index` steps it
        let kdst = krow * 5u + kblk
        let xsrc = (t0 + krow) * nb + kblk             // a token's row is n / 32 uint4; step kb adds kb * 4
        let krow_ok = m0 + krow < pa.d   // a `MOE` stamp's last column tile reaches past the plane's rows: those stage zero
        let wi = tid & 127u
        let wstage = tid < 128u
        let xstage = tid >= 128u && tid < 256u
        let wsrow = (wblk + (m0 + wi) * nsb) * 5u   // the nv4 scale row (unread on an `MX` stamp: `w_scale_word` reads the byte plane)
        let wrow_ok = m0 + wi < pa.d
        let xsrow = (t0 + wi) * (pa.n / 64u)           // step kb: + kb * 2
        var f00, f01, f10, f11 : coopmatAcc_f32_16x16
        var wnext = w_stage(ksrc, 0u, kblk, nb, krow_ok)
        sa[kdst] = wnext
        if (nb4 > 1u) {
            wnext = w_stage(ksrc, 1u, kblk, nb, krow_ok)
        }
        cp_async_16(unsafe(addr(sb[kdst])), unsafe(addr(xq4[xsrc])))
        cp_async_commit()
        if (wstage) {
            swa[wi * 2u] = w_scale_word(wblk, m0 + wi, wsrow, 0u, 0u, nb, wrow_ok)
            swa[wi * 2u + 1u] = w_scale_word(wblk, m0 + wi, wsrow, 0u, 1u, nb, wrow_ok)
        } elif (xstage) {
            swb[wi * 2u] = x_scale_word(xsrow, 0u, 0u, nb)
            swb[wi * 2u + 1u] = x_scale_word(xsrow, 0u, 1u, nb)
        }
        var kb = 0u
        while (kb < nb4) {
            cp_async_wait(0)   // this step's stage landed
            barrier()          // ... and every warp is done with the other stage, which the next copies overwrite
            let st = kb & 1u
            let nk = kb + 1u
            if (nk < nb4) {
                let st2 = nk & 1u
                sa[st2 * 640u + kdst] = wnext
                if (nk + 1u < nb4) {
                    wnext = w_stage(ksrc, nk + 1u, kblk, nb, krow_ok)
                }
                cp_async_16(unsafe(addr(sb[st2 * 640u + kdst])), unsafe(addr(xq4[xsrc + nk * 4u])))
                if (wstage) {
                    swa[st2 * 256u + wi * 2u] = w_scale_word(wblk, m0 + wi, wsrow, nk, 0u, nb, wrow_ok)
                    swa[st2 * 256u + wi * 2u + 1u] = w_scale_word(wblk, m0 + wi, wsrow, nk, 1u, nb, wrow_ok)
                } elif (xstage) {
                    swb[st2 * 256u + wi * 2u] = x_scale_word(xsrow, nk, 0u, nb)
                    swb[st2 * 256u + wi * 2u + 1u] = x_scale_word(xsrow, nk, 1u, nb)
                }
            }
            cp_async_commit()
            for [unroll] (k64h in range(2)) {
                let k64hu = uint(k64h)
                let abase = st * 640u + (wm * 32u + mr + 8u * (mt & 1u)) * 5u + k64hu * 2u + (mt >> 1u)   // the lane's A row: (rows lo, k lo), (rows hi, k lo), (rows lo, k hi), (rows hi, k hi)
                let bbase = st * 640u + (wn * 32u + mr + 8u * (mt >> 1u)) * 5u + k64hu * 2u + (mt & 1u)   // the lane's B row: (n lo, k lo), (n lo, k hi), (n hi, k lo), (n hi, k hi)
                var a0, a1 : coopmatA_e2m1_16x64
                ldmatrix(a0, unsafe(addr(sa[abase])))
                ldmatrix(a1, unsafe(addr(sa[abase + 80u])))
                var b0, b1 : coopmatB_e2m1_64x16
                ldmatrix(b0, unsafe(addr(sb[bbase])))
                ldmatrix(b1, unsafe(addr(sb[bbase + 80u])))
                let sw = st * 256u + (wm * 32u + g + 8u * (lane & 1u)) * 2u + k64hu   // the lane's A rows' words: row g from lane % 4 == 0, row g + 8 from lane % 4 == 1
                let sx = st * 256u + (wn * 32u + g) * 2u + k64hu                   // column g of each n-half, from lane % 4 == 0
                let sa0 = swa[sw]
                let sa1 = swa[sw + 32u]
                let sb00 = swb[sx]
                let sb01 = swb[sx + 16u]
                let sb10 = swb[sx + 32u]
                let sb11 = swb[sx + 48u]
                static_if (MX) {
                    mma_block_scaled_mx(f00, a0, b0, sa0, sb00, sb01)
                    mma_block_scaled_mx(f01, a0, b1, sa0, sb10, sb11)
                    mma_block_scaled_mx(f10, a1, b0, sa1, sb00, sb01)
                    mma_block_scaled_mx(f11, a1, b1, sa1, sb10, sb11)
                } else {
                    mma_block_scaled(f00, a0, b0, sa0, sb00, sb01)
                    mma_block_scaled(f01, a0, b1, sa0, sb10, sb11)
                    mma_block_scaled(f10, a1, b0, sa1, sb00, sb01)
                    mma_block_scaled(f11, a1, b1, sa1, sb10, sb11)
                }
            }
            kb++
        }
        // the store is column-major over [token][d]: a token's row is the fragment's column
        let site_scale = MX ? 1.0 : uint_bits_to_float(wsu[(wblk + m0 * nsb) * 5u + 4u])
        let c0 = t0 + wn * 32u + 2u * t
        let cs0 = float4(xrs[c0], xrs[c0 + 1u], xrs[c0 + 8u], xrs[c0 + 9u]) * site_scale
        let cs1 = float4(xrs[c0 + 16u], xrs[c0 + 17u], xrs[c0 + 24u], xrs[c0 + 25u]) * site_scale
        mma_scale_cols(f00, cs0)
        mma_scale_cols(f10, cs0)
        mma_scale_cols(f01, cs1)
        mma_scale_cols(f11, cs1)
        static_if (MOE) {
            for [unroll] (rnd in range(8)) {
                let mine = (warp >> 1u) == uint(rnd)
                let slot = (warp & 1u) * 1024u
                if (mine) {
                    coopmatStore(f00, ystage, int(slot), 32, 1)
                    coopmatStore(f01, ystage, int(slot + 512u), 32, 1)
                    coopmatStore(f10, ystage, int(slot + 16u), 32, 1)
                    coopmatStore(f11, ystage, int(slot + 512u + 16u), 32, 1)
                }
                barrier()
                if (mine) {
                    let col = m0 + wm * 32u + lane
                    if (col < pa.d) {
                        for [unroll] (j in range(32)) {
                            y[(t0 + wn * 32u + uint(j)) * pa.d + col] = ystage[slot + uint(j) * 32u + lane]
                        }
                    }
                }
                barrier()
            }
        } else {
            let d = int(pa.d)
            let ybase = int((t0 + wn * 32u) * pa.d + m0 + wm * 32u)
            coopmatStore(f00, y, ybase, d, 1)
            coopmatStore(f01, y, ybase + 16 * d, d, 1)
            coopmatStore(f10, y, ybase + 16, d, 1)
            coopmatStore(f11, y, ybase + 16 * d + 16, d, 1)
        }
    }
}

//! the MXFP4 expert leaf on the FP4 tensor rung fed from the expert schedule (`MX`, `KTAIL`, `MOE`): gpt-oss's routed stacks over
//! the gathered rows' MXFP4 feed - raw e2m1 nibbles on both sides under the UE8M0 block scales, a 2880-wide row on the k tail, a
//! 2880-wide plane's last column tile under the store's guard
[cuda_dispatch(name = "mx4_fp4_mmq_moe", grid = "wgs", params = "wgs : int64")]
class CuMx4Fp4MmqMoe : MmqFp4T {
    override MX = true
    override KTAIL = true
    override MOE = true

    [cuda_kernel(local_size_x = 512, name = "mx4_fp4_mmq_moe_cuda")]
    def run {
        mmq_body()
    }
}

//! the NVFP4 leaf on the FP4 tensor rung: the window chain's GEMM for nv4 planes under DASLLAMA_CUDA_MMQ
[cuda_dispatch(name = "nv4_mmq", grid = "wgs", params = "wgs : int64")]
class CuNv4Mmq : MmqFp4T {
    [cuda_kernel(local_size_x = 512, name = "nv4_mmq_cuda")]
    def run {
        mmq_body()
    }
}

let FP4_ROW_SCALE_DIV = 2688.0   //! 6 x 448: the e2m1 range times the UE4M3 range - the FP4 feed's row scale divides by it

let FP4_NIBBLES_PER_BYTE = 2l   //! the FP4 feeds' quant plane: two e2m1 nibbles a byte
let NV4_FEED_STRIP = 16l        //! the NVFP4 feed's code plane: one UE4M3 code byte a 16-element strip
let MX4_FEED_WORD_ELEMS = 64l   //! the MXFP4 feed's code plane: one 32-bit word a k64 chunk (its two 32-blocks' UE8M0 codes)

//! the strip pass's geometry as a kernel body reads it: a lane's chunk is four elements, so an NVFP4 strip is `NV4_STRIP_CHUNKS`
//! chunks and `NV4_CODES_PER_WORD` strips' codes share a word, an MXFP4 32-block is `MX4_BLOCK_CHUNKS` chunks and
//! `MX4_CODES_PER_WORD` blocks' codes share a word; a code is `FP4_CODE_BITS` wide in it
let FP4_CHUNK_ELEMS_U = 4u
let NV4_STRIP_CHUNKS = uint(NV4_FEED_STRIP) / FP4_CHUNK_ELEMS_U
let NV4_CODES_PER_WORD = 4u   //! four UE4M3 code bytes a word
let MX4_BLOCK_CHUNKS = KQ_B32_ELEMS_U / FP4_CHUNK_ELEMS_U
let MX4_CODES_PER_WORD = uint(MX4_FEED_WORD_ELEMS) / KQ_B32_ELEMS_U
let FP4_CODE_BITS = 8u

def fp4_amax4(v : float4) : float => max(max(abs(v.x), abs(v.y)), max(abs(v.z), abs(v.w)))

let FP4_CODE_MAX = 126u   //! the largest UE4M3 code a strip scale takes (448; the code past it is the format's NaN)

//! the UE4M3 code of a strip whose largest magnitude is `m`: the nearest to m / 6, stepped up once where that code's scale leaves `m` past e2m1's 6
def fp4_strip_code(m : float) : uint {
    let code = cvt_ue4m3(m / 6.0)
    return ue4m3_half_f32(code) * 12.0 < m && code < FP4_CODE_MAX ? code + 1u : code
}

//! the UE8M0 code of a 32-block of largest magnitude `m` by the MXFP4 rule (the nearest power of two to `m` over e2m1's top exponent, 2); 0 for an empty block
def mx4_block_code(m : float) : uint => m > 0.0 ? uint(clamp(int(round(log2(m))) - 2 + 127, 0, 254)) : 0u

//! the inverse of a UE8M0 code's scale: 2^(127 - code), exact
def mx4_code_inv(code : uint) : float => e8m0_half_f32(255u - code)

//! the strip pass over chunk `c` (a lane's `FP4_CHUNK_ELEMS_U` elements): `w` the nibble word in the (k, k + 8) pairing, valid where
//! c % 4 < 2; `cw` the code word, valid where c % 16 == 0; every subgroup lane calls it, a lane past the row with zeros. A code is a
//! UE8M0 a 32-block under `mx` (MXFP4), `MX4_CODES_PER_WORD` a k64 word; else (NVFP4) a UE4M3 a strip, `NV4_CODES_PER_WORD` a word,
//! the elements over the row scale; the max folds over the strip's chunks (the block's under `mx`) by lane xor
def fp4_strip_chunk(v : float4; c : uint; var w, cw : uint&; mx : bool) {
    var m = fp4_amax4(v)
    m = max(m, gk_subgroup_xor(m, 1u))
    m = max(m, gk_subgroup_xor(m, 2u))
    if (mx) {
        m = max(m, gk_subgroup_xor(m, NV4_STRIP_CHUNKS))
    }
    let code = mx ? mx4_block_code(m) : fp4_strip_code(m)
    let sval = mx ? 0.0 : ue4m3_half_f32(code) * 2.0
    let inv = mx ? mx4_code_inv(code) : (sval > 0.0 ? 1.0 / sval : 0.0)
    let p = float4(gk_subgroup_xor(v.x, 2u), gk_subgroup_xor(v.y, 2u), gk_subgroup_xor(v.z, 2u), gk_subgroup_xor(v.w, 2u))
    w = cvt_e2m1x2(v.x * inv, p.x * inv) | (cvt_e2m1x2(v.y * inv, p.y * inv) << 8u) | (cvt_e2m1x2(v.z * inv, p.z * inv) << 16u) | (cvt_e2m1x2(v.w * inv, p.w * inv) << 24u)
    let slot = mx ? (c / MX4_BLOCK_CHUNKS) % MX4_CODES_PER_WORD : (c / NV4_STRIP_CHUNKS) % NV4_CODES_PER_WORD
    cw = code << (slot * FP4_CODE_BITS)
    if (!mx) {
        cw |= gk_subgroup_xor_u(cw, NV4_STRIP_CHUNKS)
    }
    cw |= gk_subgroup_xor_u(cw, MX4_BLOCK_CHUNKS)
}

struct Fp4RowsArgs {
    k : uint        //! the row width, a multiple of 64
    inbase : uint   //! the source rows' element base
    act : uint      //! the activation code (the act form)
    ubase : uint    //! the act form's up side: its element base, and with `ustride` the rows of `ulen` elements sit `ustride` apart
    ustride : uint
    ulen : uint
    gboff : uint    //! the biased act form: this layer's row base in the gate-bias plane and the up-bias plane ([ne x k] rows a layer each)
}

//! The FP4 activation feed a workgroup a row (256 lanes), a lane four consecutive elements a round: the row's largest magnitude
//! over 6 x 448 as its f32 scale, then the strip pass (`fp4_strip_chunk`) into the nibble words, the code words and the row scale the
//! FP4 tensor rung reads; the source element is the stamp's (a float plane, or the gate / up pair through the activation),
//! read as float4 when the planes sit on the four lattice and the up rows are contiguous, and read twice
[ |> template_struct_instance]
class template Fp4FeedT : GkWgReduce {
    @template_constant ACT : bool = false    //!< the act form: element e is act(gate[e]) x up[e] (`gk_up_at` for the up side's layout)
    @template_constant BIAS : bool = false   //!< ... the biased act form (gpt-oss's stacks): the row's expert (`rex`) names the gate and up bias rows added before the activation
    @template_constant MX : bool = false     //!< the MXFP4 feed: a UE8M0 code a 32-block (two a k64 word), no row scale (1.0 stored)
    @ssbo @binding = 0 @readonly src : array<float>                      //! the rows (the act form's gate)
    @ssbo @binding = 1 @readonly @template_gate = ACT up : array<float>
    @ssbo @binding = 0 @readonly @role = "alias" src4 : array<float4>
    @ssbo @binding = 1 @readonly @role = "alias" @template_gate = ACT up4 : array<float4>
    @ssbo @binding = 2 outq : array<uint>      //! [row][k / 8] nibble words
    @ssbo @binding = 3 outsw : array<uint>     //! [row][k / 64] code words
    @ssbo @binding = 4 outrs : array<float>    //! [row] scales
    @ssbo @binding = 5 @readonly @role = "weight" @template_gate = BIAS gb : array<float>   //! the gate-bias plane
    @ssbo @binding = 6 @readonly @role = "weight" @template_gate = BIAS ub : array<float>   //! the up-bias plane
    @ssbo @binding = 7 @readonly @template_gate = BIAS rex : array<uint>                   //! row -> its expert
    @kargs pa : Fp4RowsArgs

    //! element `e`'s column in its row (the biased form's bias column)
    def bias_col(e : uint) : uint => e - pa.inbase - gl_WorkGroupID.x * pa.k

    def gate_up_at(e, ue : uint) : float2 {
        static_if (BIAS) {
            let ex = rex[gl_WorkGroupID.x]
            let c = bias_col(e)
            return float2(src[e] + gb[pa.gboff + ex * pa.k + c], up[ue] + ub[pa.gboff + ex * pa.k + c])
        } else {
            static_if (ACT) {
                return float2(src[e], up[ue])
            } else {
                return float2(src[e], 0.0)
            }
        }
    }

    def load1(e : uint) : float {
        static_if (ACT) {
            let p = gate_up_at(e, gk_up_at(e, pa.ubase, pa.ustride, pa.ulen))
            return gk_act_mul(p.x, p.y, pa.act)
        } else {
            return src[e]
        }
    }

    //! elements e .. e + 3 off the float4 bindings, `e` a four-multiple off a four-aligned row base
    def load4_lanes(e : uint) : float4 {
        let g = src4[e / 4u]
        static_if (ACT) {
            let u = up4[(pa.ubase + e) / 4u]
            static_if (BIAS) {
                let ex = rex[gl_WorkGroupID.x]
                let c = bias_col(e)
                let gbo = pa.gboff + ex * pa.k + c
                let ubo = pa.gboff + ex * pa.k + c
                return float4(gk_act_mul(g.x + gb[gbo], u.x + ub[ubo], pa.act), gk_act_mul(g.y + gb[gbo + 1u], u.y + ub[ubo + 1u], pa.act),
                    gk_act_mul(g.z + gb[gbo + 2u], u.z + ub[ubo + 2u], pa.act), gk_act_mul(g.w + gb[gbo + 3u], u.w + ub[ubo + 3u], pa.act))
            } else {
                return float4(gk_act_mul(g.x, u.x, pa.act), gk_act_mul(g.y, u.y, pa.act), gk_act_mul(g.z, u.z, pa.act), gk_act_mul(g.w, u.w, pa.act))
            }
        } else {
            return g
        }
    }

    //! elements e .. e + 3, `e` a four-multiple off the row base: the float4 lanes where both bases sit on the four lattice and the up side is unstrided, else four scalar reads
    def load4(e : uint) : float4 {
        if (pa.ustride == 0u && ((pa.inbase | pa.ubase) & 3u) == 0u) {
            let lanes = load4_lanes(e)
            return lanes
        }
        return float4(load1(e), load1(e + 1u), load1(e + 2u), load1(e + 3u))
    }

    //! chunk `cc` of row `tok` through the strip pass and its stores; `on` for a chunk inside the row
    def strip_put(tok, cc : uint; on : bool; v : float4) {
        var w = 0u
        var cw = 0u
        fp4_strip_chunk(v, cc, w, cw, MX)
        if (on && (cc & 3u) < 2u) {
            outq[tok * (pa.k / 8u) + (cc >> 2u) * 2u + (cc & 3u)] = w
        }
        if (on && (cc & 15u) == 0u) {
            outsw[tok * (pa.k / 64u) + (cc >> 4u)] = cw
        }
    }

    [spirv_inline, arch(at="../ARCHITECTURE_GPU_ASYMMETRIES_CUDA.md#cuda-asymmetries")]
    def fp4_body {
        let tok = gl_WorkGroupID.x
        let base = pa.inbase + tok * pa.k
        let tid = gl_LocalInvocationID.x
        let nch = pa.k / 4u
        var amax = 0.0   // an `MX` feed's blocks carry the range; else the row's largest magnitude over the strip code's and e2m1's tops
        static_if (!MX) {
            var m = 0.0
            var c = tid
            while (c < nch) {
                m = max(m, fp4_amax4(load4(base + c * 4u)))
                c += 256u
            }
            wg_max_into(m, 0u, amax)
        }
        let rs = MX ? 1.0 : amax / FP4_ROW_SCALE_DIV
        let inv_rs = MX ? 1.0 : (rs > 0.0 ? 1.0 / rs : 0.0)
        if (tid == 0u) {
            outrs[tok] = rs
        }
        let nit = (nch + 255u) / 256u
        var i = 0u
        while (i < nit) {
            let cc = tid + i * 256u
            let on = cc < nch
            var v = float4(0.0)
            if (on) {
                v = load4(base + cc * 4u) * inv_rs
            }
            strip_put(tok, cc, on, v)
            i++
        }
    }
}

//! the FP4 feed of a float plane's rows
[cuda_dispatch(name = "fp4_rows", grid = "nrows", params = "nrows : int64")]
class CuFp4Rows : Fp4FeedT {
    [cuda_kernel(local_size_x = 256, name = "fp4_rows_cuda", precise = true)]
    def run {
        fp4_body()
    }
}

//! the FP4 feed of a gate / up pair through the activation: the down GEMM's hidden feed
[cuda_dispatch(name = "fp4_actrows", grid = "nrows", params = "nrows : int64")]
class CuFp4ActRows : Fp4FeedT {
    override ACT = true

    [cuda_kernel(local_size_x = 256, name = "fp4_actrows_cuda", precise = true)]
    def run {
        fp4_body()
    }
}

//! the MXFP4 feed of a float plane's rows: the routed block's gathered rows for the mx4 expert leaf
[cuda_dispatch(name = "mx4_rows", grid = "nrows", params = "nrows : int64")]
class CuMx4Rows : Fp4FeedT {
    override MX = true

    [cuda_kernel(local_size_x = 256, name = "mx4_rows_cuda", precise = true)]
    def run {
        fp4_body()
    }
}

//! the MXFP4 feed of a gate / up pair through the activation: the routed block's hidden rows for the mx4 down leaf
[cuda_dispatch(name = "mx4_actrows", grid = "nrows", params = "nrows : int64")]
class CuMx4ActRows : Fp4FeedT {
    override MX = true
    override ACT = true

    [cuda_kernel(local_size_x = 256, name = "mx4_actrows_cuda", precise = true)]
    def run {
        fp4_body()
    }
}

//! ... the biased twin (gpt-oss's stacks): each row's expert's gate and up bias rows added before the activation
[cuda_dispatch(name = "mx4_actrows_b", grid = "nrows", params = "nrows : int64")]
class CuMx4ActRowsB : Fp4FeedT {
    override MX = true
    override ACT = true
    override BIAS = true

    [cuda_kernel(local_size_x = 256, name = "mx4_actrows_b_cuda", precise = true)]
    def run {
        fp4_body()
    }
}

//! the residual step on `GkAddRmsRows` with the FP4 feed of its normed row in one dispatch, a workgroup a row: the x feed
//! of the FP4 tensor rung's q / k / v and gate / up sites
[ |> template_struct_instance]
class template ArRqFp4T : ArBase {
    override NORM_SWITCH = true
    @ssbo @binding = 3 outq4 : array<uint>      //! [row][dim / 8] nibble words (the Q8_0 form's `outq` sits off this stamp's gate)
    @ssbo @binding = 10 outsw : array<uint>     //! [row][dim / 64] code words - past every combine family's own bindings
    @ssbo @binding = 11 outrs : array<float>    //! [row] scales

    [spirv_inline]
    def ar_fp4_body {
        let tok = gl_WorkGroupID.x
        var ss = 0.0
        accum_row_into(tok * pa.dim, ss)
        var inv = 0.0
        row_inv_into(ss, inv)
        let tid = gl_LocalInvocationID.x
        var m = 0.0
        var e = tid
        while (e < pa.dim) {
            m = max(m, abs(normed_at(e, 0.0, inv)))
            e += 256u
        }
        var amax = 0.0
        wg_max_into(m, 0u, amax)
        let rs = amax / FP4_ROW_SCALE_DIV
        let inv_rs = rs > 0.0 ? 1.0 / rs : 0.0
        if (tid == 0u) {
            outrs[tok] = rs
        }
        let nch = pa.dim / 4u
        let nit = (nch + 255u) / 256u
        var i = 0u
        while (i < nit) {
            let cc = tid + i * 256u
            let on = cc < nch
            var v = float4(0.0)
            if (on) {
                let e0 = cc * 4u
                v = float4(normed_at(e0, 0.0, inv), normed_at(e0 + 1u, 0.0, inv), normed_at(e0 + 2u, 0.0, inv), normed_at(e0 + 3u, 0.0, inv)) * inv_rs
            }
            var w = 0u
            var cw = 0u
            fp4_strip_chunk(v, cc, w, cw, false)
            if (on && (cc & 3u) < 2u) {
                outq4[tok * (pa.dim / 8u) + (cc >> 2u) * 2u + (cc & 3u)] = w
            }
            if (on && (cc & 15u) == 0u) {
                outsw[tok * (pa.dim / 64u) + (cc >> 4u)] = cw
            }
            i++
        }
    }
}

//! the residual step with the FP4 feed, the window's rows
[cuda_dispatch(name = "cls_ar_fp4_b", grid = "nrows", params = "nrows : int64")]
class CuClsArAddRmsFp4B : ArRqFp4T {
    [cuda_kernel(local_size_x = 256, name = "cls_ar_fp4_b_cuda", precise = true)]
    def run {
        ar_fp4_body()
    }
}

//! the Q4_K leaf: the same tile over nibble codes decoded in the weight stage, with the min term's rank-sixteen correction
[cuda_dispatch(name = "k4_mmq", grid = "wgs", params = "wgs : int64")]
class CuK4Mmq : MmqT {
    typedef FMT = KqTag_q4032
    override KQ = true
    override WORDS = true
    override MIN = true
    override K4 = true

    [cuda_kernel(local_size_x = 512, name = "k4_mmq_cuda")]
    def run {
        mmq_body()
    }
}

//! the IQ4_XS leaf: the nibbles through the codebook to signed codes in the weight stage, a scale a block, no min term
[cuda_dispatch(name = "iq4xs_mmq", grid = "wgs", params = "wgs : int64")]
class CuIq4xsMmq : MmqT {
    typedef FMT = KqTag_q4032
    override KQ = true
    override WORDS = true
    override LUT = true

    [cuda_kernel(local_size_x = 512, name = "iq4xs_mmq_cuda")]
    def run {
        mmq_body()
    }
}

//! the superblock Q4_0 leaf (a Q4_0 plane on the 256 lattice): the nibbles less eight as signed codes, a d a block off the d-pair row
[cuda_dispatch(name = "q40_mmq", grid = "wgs", params = "wgs : int64")]
class CuQ40Mmq : MmqT {
    typedef FMT = KqTag_q4032
    override KQ = true
    override WORDS = true
    override QSC = true

    [cuda_kernel(local_size_x = 512, name = "q40_mmq_cuda")]
    def run {
        mmq_body()
    }
}

//! the superblock IQ4_NL leaf (an IQ4_NL plane on the 256 lattice): the nibbles through the codebook, a d a block off the d-pair row
[cuda_dispatch(name = "iq4nl_mmq", grid = "wgs", params = "wgs : int64")]
class CuIq4nlMmq : MmqT {
    typedef FMT = KqTag_q4032
    override KQ = true
    override WORDS = true
    override QSC = true
    override LUT = true

    [cuda_kernel(local_size_x = 512, name = "iq4nl_mmq_cuda")]
    def run {
        mmq_body()
    }
}

//! the Q5_1 leaf: five words a block, the nibbles with their fifth bits, d and m - the m through the rank-sixteen correction
[cuda_dispatch(name = "q51_mmq", grid = "wgs", params = "wgs : int64")]
class CuQ51Mmq : MmqT {
    typedef FMT = KqTag_q51
    override B32 = true
    override WORDS = true
    override MIN = true

    [cuda_kernel(local_size_x = 512, name = "q51_mmq_cuda")]
    def run {
        mmq_body()
    }
}

//! the Q4_0 leaf: four words a block, the nibbles less eight as signed codes, a d a block
[cuda_dispatch(name = "q4032_mmq", grid = "wgs", params = "wgs : int64")]
class CuQ4032Mmq : MmqT {
    typedef FMT = KqTag_q4032
    override B32 = true
    override WORDS = true

    [cuda_kernel(local_size_x = 512, name = "q4032_mmq_cuda")]
    def run {
        mmq_body()
    }
}

//! the Q5_0 leaf: five words a block, the five-bit codes less sixteen as signed codes, a d a block
[cuda_dispatch(name = "q5032_mmq", grid = "wgs", params = "wgs : int64")]
class CuQ5032Mmq : MmqT {
    typedef FMT = KqTag_q5032
    override B32 = true
    override WORDS = true

    [cuda_kernel(local_size_x = 512, name = "q5032_mmq_cuda")]
    def run {
        mmq_body()
    }
}

//! the IQ4_NL leaf: four words a block, the nibbles through the codebook, a d a block
[cuda_dispatch(name = "iq4nl32_mmq", grid = "wgs", params = "wgs : int64")]
class CuIq4nl32Mmq : MmqT {
    typedef FMT = KqTag_iq4nl32
    override B32 = true
    override WORDS = true

    [cuda_kernel(local_size_x = 512, name = "iq4nl32_mmq_cuda")]
    def run {
        mmq_body()
    }
}

struct WordsArgs {
    n : uint
}

//! a word copy on the compute queue, CUDA-only: the token step's inputs off its mapped host block into their device buffers and its
//! logits into the mapped answer, so the captured graph holds kernel nodes alone - a memcpy node crosses to the copy engine and
//! back, two fence hops of 50-120 us each on this driver; a thread moves four words where the run allows (a 16-byte store
//! across PCIe is one transaction where four 4-byte stores are four), the tail a word at a time
[cuda_dispatch(name = "words_copy", grid = "wgs", params = "wgs : int64")]
class CuWordsCopy {
    @ssbo @binding = 0 @readonly src : array<uint>
    @ssbo @binding = 1 dst : array<uint>
    @ssbo @binding = 0 @readonly src4 : array<uint4>
    @ssbo @binding = 1 dst4 : array<uint4>
    @kargs pa : WordsArgs

    [cuda_kernel(local_size_x = 256, name = "words_copy_cuda")]
    def run {
        let i = gl_GlobalInvocationID.x
        let n4 = pa.n >> 2u
        if (i < n4) {
            dst4[i] = src4[i]
        } elif (i < pa.n - (n4 << 2u) + n4) {
            let w = (n4 << 2u) + (i - n4)
            dst[w] = src[w]
        }
    }
}

//! the words-copy grid over `words`: a thread a word quad, the tail's words a thread each
def public words_copy_wgs(words : int64) : int64 => ((words >> 2l) + (words & 3l) + 255l) / 256l

//! `words` words of device (or mapped) `src` into device `dst` by the copy kernel on stream `s`: a graph node a memcpy over mapped
//! memory cannot be
def public cud_words(var s : void?; src, dst : uint64; words : int64) {
    var pc = WordsArgs(n = uint(words))
    enc_words_copy(s, fixed_array(src, dst), pc, words_copy_wgs(words))
}

//! `bytes` of device memory, zeroed on the compute stream; 0 where the card has no room
[arch(at="../ARCHITECTURE_GPU.md#gpu-backends")]
def public cud_dev_zeroed(bytes : int64) : uint64 {
    let d = cuda_mem_alloc(uint64(bytes))
    if (d != 0ul) {
        cuda_memset_d8(d, 0u8, uint64(bytes), g_cud.stream)
    }
    return d
}

// ===== the short window's split-K knee race (ARCHITECTURE_GPU_VULKAN_GEMM.md#vk-short-split) over this file's q8 s column and reduce: synthetic planes, no model =====

//! a width's synthetic planes on the device: a ring of hashed q8 weight copies (`d` x K), `KNEE_S_ROWS` f16 activation rows,
//! the output, the split's partial planes and the one-region schedule of each split count (the map zero: every block region 0)
struct private CudKneeSite {
    wq, ws : array<uint64>
    xf, y, part : uint64
    sched : uint64[4]
}

def private knee_site(d, n : int64) : CudKneeSite {
    let blocks = d * n / Q8_BLOCK_ELEMS
    let copies = int(max(2l, KNEE_DISCRETE_RING_BYTES / (blocks * Q8_BLOCK_BYTES) + 1l))
    var inscope xf_words <- [for (i in range64((KNEE_S_ROWS + TILE_READ_SLACK) * n / 2l)); knee_xf_word(uint(i))]
    var st = CudKneeSite(xf = buf_upload(xf_words, g_cud.stream), y = cud_dev_zeroed(KNEE_S_ROWS * d * 4l), part = cud_dev_zeroed(8l * KNEE_S_ROWS * d * 4l))
    st.wq |> reserve(copies)
    st.ws |> reserve(copies)
    var inscope scale_words <- [for (_i in range64((blocks * Q8_SPB + 3l) / 4l)); KNEE_Q8_SCALE_WORD]
    for (c in range(copies)) {
        var inscope quant_words <- [for (i in range64(blocks * (Q8_QPB / 4l))); knee_q8_word(uint(i), uint(c))]
        st.wq |> push(buf_upload(quant_words, g_cud.stream))
        st.ws |> push(buf_upload(scale_words, g_cud.stream))
    }
    let groups = (d + 127l) / 128l
    for (si, nsk in range(4), [1l, 2l, 4l, 8l]) {
        var inscope words : array<uint>
        words |> resize(4l + groups * nsk)
        var cur = 4l
        batch_sched_map(unsafe(addr(words[0])), cur, 0l, 0l, KNEE_S_ROWS, groups * nsk)
        st.sched[si] = buf_upload(words, g_cud.stream)
    }
    return <- st
}

def private knee_site_release(var st : CudKneeSite) {
    for (b in st.wq) {
        cuda_mem_free(b)
    }
    for (b in st.ws) {
        cuda_mem_free(b)
    }
    for (b in [st.xf, st.y, st.part]) {
        cuda_mem_free(b)
    }
    for (b in st.sched) {
        cuda_mem_free(b)
    }
    delete st
}

//! One width's times at `nsk` chunks: a step is one role on the one stream - a window's GEMM waits on the role before it, so its
//! latency is the cost - its weights the next copy of the ring; the whole arm the s column into the output, the split arm through
//! the partial-plane scratch and its reduce
def private knee_times(st : CudKneeSite; d, n, nsk : int64) : tuple<b : double; t : double> {
    let groups = (d + 127l) / 128l
    let nring = length(st.wq)
    let ck = split_k_share(n, nsk, SplitKnee(align = 256l, exact = false))
    var cur = 0   // the ring cursor, carried across the runs: each run reads the copies after the last run's
    return knee_arms_us($ {
        for (_i in range(KNEE_STEPS)) {
            var pc = BatchArgs(n = uint(n), d = uint(d), map_off = 4u, ksplit = 0u)
            cud_cm2_enc(int(KqFmt.q8), CUD_CM2_TC_S, g_cud.stream, fixed_array(st.wq[cur % nring], st.ws[cur % nring], st.sched[0], st.xf, st.y), pc, groups)
            cur++
        }
        panic("dasLLAMA cuda: the knee race's whole arm did not land ({cuda_last_error()})") if (!cud_sync())
    }) $ {
        for (_i in range(KNEE_STEPS)) {
            var pc = BatchArgs(n = uint(n), d = uint(d), map_off = 4u, ksplit = uint(ck))
            cud_cm2_enc(int(KqFmt.q8), CUD_CM2_TC_S, g_cud.stream, fixed_array(st.wq[cur % nring], st.ws[cur % nring], st.sched[knee_way_index(nsk) + 1], st.xf, st.part), pc, groups * nsk)
            var pr = SkRedArgs(nelem = uint(KNEE_S_ROWS * d), k = uint(nsk), ybase = 0u)
            enc_splitk_reduce_cls(g_cud.stream, fixed_array(st.part, st.y), pr, splitk_reduce_wgs(KNEE_S_ROWS * d))
            cur++
        }
        panic("dasLLAMA cuda: the knee race's split arm did not land ({cuda_last_error()})") if (!cud_sync())
    }
}

//! The box's short-window knee on the CUDA s column (`cud_split_k_tgs`): the q8 rung table (`knee_q8_race`) at 31 rows, one role
//! a rung - the one stream co-runs none - priced as the other homes price theirs; the knob stands where the device serves no s
//! column. Leaves the knob set.
[arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#vk-short-split")]
def cud_split_knee_race : int64 {
    if (!ensure_q8_batch_cm2s_cls() || !ensure_splitk_reduce_cls()) {
        to_log(LOG_INFO, "dasLLAMA cuda: short-window knee race skipped - no q8 s column on this device\n")
        return cud_split_k_tgs()
    }
    var site : array<CudKneeSite>   // the width the rungs time now
    let tgs = knee_q8_race("cuda knee", $(d : int64) => (d + 127l) / 128l, $(d : int64) {
        site |> emplace(knee_site(d, KNEE_Q8_K))
    }, $ {
        knee_site_release(site[0])
        site |> clear()
    }) $(d, nsk : int64) : tuple<b : double; t : double> {
        return knee_times(site[0], d, KNEE_Q8_K, nsk)
    }
    set_cud_split_k_tgs(tgs)
    return tgs
}

//! the split-k reduce (`SplitKReduceT`): the partial planes a split GEMM leaves summed into its y
[cuda_dispatch(name = "splitk_reduce_cls", grid = "wgs", params = "wgs : int64")]
class CuSplitKReduce : SplitKReduceT {
    [cuda_kernel(local_size_x = 256, name = "splitk_reduce_cls_cuda")]
    def run {
        splitk_reduce_body()
    }
}

//! the window's f32 rows as the next GEMM's f16 feed (`GkClampRows` at the half range)
[cuda_dispatch(name = "tower_clamp_cvt_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuTowerClampCvt : GkClampRows {
    override OUT16 = true

    [cuda_kernel(local_size_x = 256, name = "tower_clamp_cvt_cls_cuda")]
    def run {
        clamp_rows_body()
    }
}

//! the cm2 flash attention at head size 64 (`FaH64T` over the shared 64 x 64 workgroup fragments): the window's rows against
//! the f16 mirror, the o rows as f32, and as the wo GEMM's f16 feed
[cuda_dispatch(name = "fa_cm2_h64_cls", kernel = "run", family = "fa_cm2_cls", grid = "wgs", params = "wgs : int64")]
class CuFaCm2H64 : FaH64T {
    typedef OT = float

    [cuda_kernel(local_size_x = 128, name = "fa_cm2_h64_cls_cuda")]
    def run {
        fa_body()
    }
}

[cuda_dispatch(name = "fa_cm2_h64_f16_cls", kernel = "run", family = "fa_cm2_cls", grid = "wgs", params = "wgs : int64")]
class CuFaCm2H64F16 : FaH64T {
    typedef OT = float16
    override OUT16 = true

    [cuda_kernel(local_size_x = 128, name = "fa_cm2_h64_f16_cls_cuda")]
    def run {
        fa_body()
    }
}

//! the sink stamps at head 64 (gpt-oss, `FaH64T` under `SINK`): the online softmax seeded with the head's sink logit, the o rows
//! as f32 and as f16
[cuda_dispatch(name = "fa_cm2_h64_sink_cls", kernel = "run", family = "fa_cm2_sink_cls", grid = "wgs", params = "wgs : int64")]
class CuFaCm2H64Sink : FaH64T {
    typedef OT = float
    override SINK = true

    [cuda_kernel(local_size_x = 128, name = "fa_cm2_h64_sink_cls_cuda")]
    def run {
        fa_body()
    }
}

//! the head-64 sink stamp with the f16 O store (`FaH64T`)
[cuda_dispatch(name = "fa_cm2_h64_sink_f16_cls", kernel = "run", family = "fa_cm2_sink_cls", grid = "wgs", params = "wgs : int64")]
class CuFaCm2H64SinkF16 : FaH64T {
    typedef OT = float16
    override OUT16 = true
    override SINK = true

    [cuda_kernel(local_size_x = 128, name = "fa_cm2_h64_sink_f16_cls_cuda")]
    def run {
        fa_body()
    }
}

//! the flash tile at head 96 (`FaH96T`): phi-3's heads, 32 keys a step
[cuda_dispatch(name = "fa_cm2_h96_cls", kernel = "run", family = "fa_cm2_cls", grid = "wgs", params = "wgs : int64")]
class CuFaCm2H96 : FaH96T {
    typedef OT = float

    [cuda_kernel(local_size_x = 128, name = "fa_cm2_h96_cls_cuda")]
    def run {
        fa_body()
    }
}

//! the head-96 flash tile with the f16 O store: `FaH96T` over `FaT`
[cuda_dispatch(name = "fa_cm2_h96_f16_cls", kernel = "run", family = "fa_cm2_cls", grid = "wgs", params = "wgs : int64")]
class CuFaCm2H96F16 : FaH96T {
    typedef OT = float16
    override OUT16 = true

    [cuda_kernel(local_size_x = 128, name = "fa_cm2_h96_f16_cls_cuda")]
    def run {
        fa_body()
    }
}

//! the flash tile at head 128 (`FaH128T`): the llama 3B class and up, 32 keys a step
[cuda_dispatch(name = "fa_cm2_h128_cls", kernel = "run", family = "fa_cm2_cls", grid = "wgs", params = "wgs : int64")]
class CuFaCm2H128 : FaH128T {
    typedef OT = float

    [cuda_kernel(local_size_x = 128, name = "fa_cm2_h128_cls_cuda")]
    def run {
        fa_body()
    }
}

[cuda_dispatch(name = "fa_cm2_h128_f16_cls", kernel = "run", family = "fa_cm2_cls", grid = "wgs", params = "wgs : int64")]
class CuFaCm2H128F16 : FaH128T {
    typedef OT = float16
    override OUT16 = true

    [cuda_kernel(local_size_x = 128, name = "fa_cm2_h128_f16_cls_cuda")]
    def run {
        fa_body()
    }
}

//! the flash tile at head 256 (`FaH256T`): the deltanet hybrids' attention heads, 32 keys a step; the `G` stamps gate each head's
//! output by the sigmoid of its [q | gate] half
[cuda_dispatch(name = "fa_cm2_h256_cls", kernel = "run", family = "fa_cm2_cls", grid = "wgs", params = "wgs : int64")]
class CuFaCm2H256 : FaH256T {
    typedef OT = float

    [cuda_kernel(local_size_x = 128, name = "fa_cm2_h256_cls_cuda")]
    def run {
        fa_body()
    }
}

//! the head-256 flash tile with the f16 O store (`FaH256T`)
[cuda_dispatch(name = "fa_cm2_h256_f16_cls", kernel = "run", family = "fa_cm2_cls", grid = "wgs", params = "wgs : int64")]
class CuFaCm2H256F16 : FaH256T {
    typedef OT = float16
    override OUT16 = true

    [cuda_kernel(local_size_x = 128, name = "fa_cm2_h256_f16_cls_cuda")]
    def run {
        fa_body()
    }
}

//! the head-256 gated flash tile (`FaH256T` under `GATED`)
[cuda_dispatch(name = "fa_cm2_h256_g_cls", kernel = "run", family = "fa_cm2_cls", grid = "wgs", params = "wgs : int64")]
class CuFaCm2H256G : FaH256T {
    typedef OT = float
    override GATED = true

    [cuda_kernel(local_size_x = 128, name = "fa_cm2_h256_g_cls_cuda")]
    def run {
        fa_body()
    }
}

//! the head-256 gated flash tile with the f16 O store (`FaH256T` under `GATED`)
[cuda_dispatch(name = "fa_cm2_h256_g_f16_cls", kernel = "run", family = "fa_cm2_cls", grid = "wgs", params = "wgs : int64")]
class CuFaCm2H256GF16 : FaH256T {
    typedef OT = float16
    override OUT16 = true
    override GATED = true

    [cuda_kernel(local_size_x = 128, name = "fa_cm2_h256_g_f16_cls_cuda")]
    def run {
        fa_body()
    }
}

//! the head-256 softcap flash tile (`FaH256T` under `CAP`): gemma-2's layers, every scaled score through cap * tanh(s / cap)
[cuda_dispatch(name = "fa_cm2_h256_cap_cls", kernel = "run", family = "fa_cm2_cls", grid = "wgs", params = "wgs : int64")]
class CuFaCm2H256Cap : FaH256T {
    typedef OT = float
    override CAP = true

    [cuda_kernel(local_size_x = 128, name = "fa_cm2_h256_cap_cls_cuda")]
    def run {
        fa_body()
    }
}

//! the head-256 softcap flash tile with the f16 O store (`FaH256T` under `CAP`)
[cuda_dispatch(name = "fa_cm2_h256_cap_f16_cls", kernel = "run", family = "fa_cm2_cls", grid = "wgs", params = "wgs : int64")]
class CuFaCm2H256CapF16 : FaH256T {
    typedef OT = float16
    override OUT16 = true
    override CAP = true

    [cuda_kernel(local_size_x = 128, name = "fa_cm2_h256_cap_f16_cls_cuda")]
    def run {
        fa_body()
    }
}

//! the flash tile at head 512 (`FaH512T`, gemma-4's global layers, V off the raw k row) on the template's KHR arm - the 16-row
//! q tile over the warps' 16 x 16 fragments: the workgroup-tile arm's 64-row q stage at 512 wide passes the card's shared memory
[cuda_dispatch(name = "fa_khr_h512_cls", kernel = "run", family = "fa_khr_cls", grid = "wgs", params = "wgs : int64")]
class CuFaKhrH512 : FaH512T {
    override KHR = true
    typedef OT = float

    [cuda_kernel(local_size_x = 128, name = "fa_khr_h512_cls_cuda")]
    def run {
        fa_body()
    }
}

//! the head-512 flash tile with the f16 O store (`FaH512T`, the KHR arm)
[cuda_dispatch(name = "fa_khr_h512_f16_cls", kernel = "run", family = "fa_khr_cls", grid = "wgs", params = "wgs : int64")]
class CuFaKhrH512F16 : FaH512T {
    override KHR = true
    typedef OT = float16
    override OUT16 = true

    [cuda_kernel(local_size_x = 128, name = "fa_khr_h512_f16_cls_cuda")]
    def run {
        fa_body()
    }
}

//! the small f16 x f16 GEMM on the cm2 128 x 128 tile (`F16GemmCm2T`): the E-series pre-step's projection over the window's rows,
//! whole and at the panel's row 0 at its one site
[cuda_dispatch(name = "f16_gemm_cm2_cls", grid = "wgs", params = "wgs : int64")]
class CuF16GemmCm2 : F16GemmCm2T {
    override KSPLIT = false
    override WOFF = false

    [cuda_kernel(local_size_x = 256, name = "f16_gemm_cm2_cls_cuda")]
    def run {
        f16_gemm_body()
    }
}

//! bf16 rows widened by their bit shift and stored f16 (saturated at the half range), on the template both homes stamp
//! (`GkClampRows`): the E-series projection's f16 panel, written once at the seat
[cuda_dispatch(name = "ple_bf16_cvt_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuPleBf16Cvt : GkClampRows {
    override OUT16 = true
    override BF16IN = true

    [cuda_kernel(local_size_x = 256, name = "ple_bf16_cvt_cls_cuda")]
    def run {
        clamp_rows_body()
    }
}

//! the flash tile of head size `hs` takes the KHR arm's 16-row workgroups (`fa_stamp_wgs`): the 512 stamps alone
def cud_fa_khr(hs : int64) : bool => hs == 512l

//! the flash tile of head size `hs` serves a scored softcap: the h256 stamps alone carry the `CAP` arm (gemma-2's shape)
def cud_fa_cap_ok(hs : int64; cap : float) : bool => cap == 0.0 || hs == 256l

//! the flash stamp of head size `hs` (64, 96, 128, 256 or 512) with the f32 or the f16 O store; the gate arm and the softcap arm
//! (`pc.cap` past zero) are the h256 stamps' alone, and `sink` takes the h64 sink stamps over the per-head sink plane `sinks` (the
//! sink upload holds the head size to 64)
def cud_fa_enc(var s : void?; hs : int64; gated, f16out, sink : bool; bufs : uint64 const[4]; sinks : uint64; var pc : FaArgs; wgs : int64) {
    assert(cud_fa_cap_ok(hs, pc.cap), "cud flash: a softcap off head 256 - the cap stamps are the h256 tile's")
    if (sink) {
        let sbufs = fixed_array(bufs[0], bufs[1], bufs[2], bufs[3], sinks)
        if (f16out) {
            enc_fa_cm2_h64_sink_f16_cls(s, sbufs, pc, wgs)
        } else {
            enc_fa_cm2_h64_sink_cls(s, sbufs, pc, wgs)
        }
    } elif (hs == 64l) {
        if (f16out) {
            enc_fa_cm2_h64_f16_cls(s, bufs, pc, wgs)
        } else {
            enc_fa_cm2_h64_cls(s, bufs, pc, wgs)
        }
    } elif (hs == 96l) {
        if (f16out) {
            enc_fa_cm2_h96_f16_cls(s, bufs, pc, wgs)
        } else {
            enc_fa_cm2_h96_cls(s, bufs, pc, wgs)
        }
    } elif (hs == 128l) {
        if (f16out) {
            enc_fa_cm2_h128_f16_cls(s, bufs, pc, wgs)
        } else {
            enc_fa_cm2_h128_cls(s, bufs, pc, wgs)
        }
    } elif (hs == 512l) {
        if (f16out) {
            enc_fa_khr_h512_f16_cls(s, bufs, pc, wgs)
        } else {
            enc_fa_khr_h512_cls(s, bufs, pc, wgs)
        }
    } elif (gated) {
        if (f16out) {
            enc_fa_cm2_h256_g_f16_cls(s, bufs, pc, wgs)
        } else {
            enc_fa_cm2_h256_g_cls(s, bufs, pc, wgs)
        }
    } elif (pc.cap != 0.0) {
        if (f16out) {
            enc_fa_cm2_h256_cap_f16_cls(s, bufs, pc, wgs)
        } else {
            enc_fa_cm2_h256_cap_cls(s, bufs, pc, wgs)
        }
    } elif (f16out) {
        enc_fa_cm2_h256_f16_cls(s, bufs, pc, wgs)
    } else {
        enc_fa_cm2_h256_cls(s, bufs, pc, wgs)
    }
}

//! the batch attention over the mirror (`DaAttnBT`): the window's rows at a head size or a mirror width the flash tiles do not serve
[cuda_dispatch(name = "da_attn_b_cls", grid = "wgs", params = "wgs : int64")]
class CuDaAttnB : DaAttnBT {
    typedef KT = float

    [cuda_kernel(local_size_x = 256, name = "da_attn_b_cls_cuda")]
    def run {
        at_attn_body()
    }
}

[cuda_dispatch(name = "da_attn_b_f16_cls", grid = "wgs", params = "wgs : int64")]
class CuDaAttnBF16 : DaAttnBT {
    typedef KT = float16

    [cuda_kernel(local_size_x = 256, name = "da_attn_b_f16_cls_cuda")]
    def run {
        at_attn_body()
    }
}

//! the batch attention's wide-head twin (`DaAttnBWT`): a head past 256 (gemma-4's global layers at 512), a 4-row q tile
[cuda_dispatch(name = "da_attn_bw_cls", grid = "wgs", params = "wgs : int64")]
class CuDaAttnBW : DaAttnBWT {
    typedef KT = float

    [cuda_kernel(local_size_x = 256, name = "da_attn_bw_cls_cuda")]
    def run {
        da_attn_bw_body()
    }
}

[cuda_dispatch(name = "da_attn_bw_f16_cls", grid = "wgs", params = "wgs : int64")]
class CuDaAttnBWF16 : DaAttnBWT {
    typedef KT = float16

    [cuda_kernel(local_size_x = 256, name = "da_attn_bw_f16_cls_cuda")]
    def run {
        da_attn_bw_body()
    }
}

//! the residual step with the Q8_0 requant and the normed row stored as floats (`ArRqT` under `STOREX`): the row the recurrent
//! head's f32 beta/alpha GEMV reads
[cuda_dispatch(name = "cls_ar_rqx", grid = "wgs", params = "wgs : int64")]
class CuArAddRmsRqX : ArRqT {
    override STOREX = true

    [cuda_kernel(local_size_x = 256, name = "cls_ar_rqx_cuda")]
    def run {
        ar_rq_body()
    }
}

//! ... and its Q8_K twin (`ArRqT` under `KBLK`)
[cuda_dispatch(name = "cls_ar_rqkx", grid = "wgs", params = "wgs : int64")]
class CuArAddRmsRqKX : ArRqT {
    override KBLK = true
    override STOREX = true

    [cuda_kernel(local_size_x = 256, name = "cls_ar_rqkx_cuda")]
    def run {
        ar_rq_body()
    }
}

//! the router GEMV over the f32 rows (`RouterGemvT`): the recurrent head's beta and alpha rows in one dispatch
[cuda_dispatch(name = "router_gemv_cls", grid = "ne", params = "ne : int64")]
class CuRouterGemv : RouterGemvT {
    typedef KT = float
    typedef XT = float

    [cuda_kernel(local_size_x = 256, name = "router_gemv_cls_cuda")]
    def run {
        router_body()
    }
}

//! the router GEMV (`RouterGemvT`) over the bf16 rows read as bf16 pairs, one word a step (the E-series pre-step's projection,
//! its own rows on the device)
[cuda_dispatch(name = "router_gemv_bf16_cls", grid = "ne", params = "ne : int64")]
class CuRouterGemvBf16 : RouterGemvT {
    typedef KT = uint
    typedef XT = float
    override VW = 2u
    override BF16 = true

    [cuda_kernel(local_size_x = 256, name = "router_gemv_bf16_cls_cuda")]
    def run {
        router_body()
    }
}

//! the router GEMV stamp (`RouterGemvT`) over the bf16 rows a halfword a lane, for an odd dim or base: a row then starts mid-word,
//! off the pairs' word grid
[cuda_dispatch(name = "router_gemv_bf16_hw_cls", grid = "ne", params = "ne : int64")]
class CuRouterGemvBf16Hw : RouterGemvT {
    typedef KT = uint16
    typedef XT = float
    override BF16 = true

    [cuda_kernel(local_size_x = 256, name = "router_gemv_bf16_hw_cls_cuda")]
    def run {
        router_body()
    }
}

//! the per-layer-embedding branch's activation, its Q8_0 requant and the proj GEMV in one dispatch (`Q8GemvPleActT`): a workgroup
//! rebuilds the side-wide x and dots one proj row a warp
[cuda_dispatch(name = "q8_gemv_pleact_cls", grid = "wgs", params = "wgs : int64")]
class CuQ8GemvPleAct : Q8GemvPleActT {
    [cuda_kernel(local_size_x = 256, name = "q8_gemv_pleact_cls_cuda"), arch(at="../ARCHITECTURE_GPU_SHARED_ROWS.md#q8-requant-byte-store")]
    def run {
        pleact_body()
    }
}

//! the E-series pre-step's finish on the template both discrete homes stamp (`GkPleFinish`): a workgroup a slice, the projection
//! rows arriving scaled (the router GEMV's `YSCALE`)
[cuda_dispatch(name = "ple_finish_cls", grid = "nslices", params = "nslices : int64")]
class CuPleFinish : GkPleFinish {
    [cuda_kernel(local_size_x = 256, name = "ple_finish_cls_cuda")]
    def run {
        finish_slice(gl_WorkGroupID.x)
    }
}

//! the conv front (`DnConvT` over the template every home stamps, `GkDnConv`)
[cuda_dispatch(name = "dn_conv_cls", grid = "wgs", params = "wgs : int64")]
class CuDnConv : DnConvT {
    [cuda_kernel(local_size_x = 256, name = "dn_conv_cls_cuda")]
    def run {
        dn_conv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! the window's scan (`DnScanT`)
[cuda_dispatch(name = "dn_scan_cls", grid = "wgs", params = "wgs : int64")]
class CuDnScan : DnScanT {
    [cuda_kernel(local_size_x = 128, name = "dn_scan_cls_cuda")]
    def run {
        dn_scan_body()
    }
}

//! the gated out-norm (`DnScanP3T` over `GkDnGate`), silu and sigmoid out-gates
[cuda_dispatch(name = "dn_scan_p3_cls", kernel = "run", grid = "npos", params = "npos : int64")]
class CuDnScanP3 : DnScanP3T {
    [cuda_kernel(local_size_x = 256, name = "dn_scan_p3_cls_cuda")]
    def run {
        dn_gate_body()
    }
}

//! the gated out-norm's sigmoid out-gate stamp (`DnScanP3T`)
[cuda_dispatch(name = "dn_scan_p3_sig_cls", kernel = "run", grid = "npos", params = "npos : int64")]
class CuDnScanP3Sig : DnScanP3T {
    override ZSIG = true

    [cuda_kernel(local_size_x = 256, name = "dn_scan_p3_sig_cls_cuda")]
    def run {
        dn_gate_body()
    }
}

//! the window's conv ring hand-off (`DnTailRingT`)
[cuda_dispatch(name = "dn_tail_cls", grid = "wgs", params = "wgs : int64")]
class CuDnTail : DnTailRingT {
    [cuda_kernel(local_size_x = 256, name = "dn_tail_cls_cuda")]
    def run {
        tail_ring_body()
    }
}

//! a checkpoint capture's conv history into its checkpoint row (`DnCapTailT`)
[cuda_dispatch(name = "dn_cap_tail_cls", grid = "wgs", params = "wgs : int64")]
class CuDnCapTail : DnCapTailT {
    [cuda_kernel(local_size_x = 256, name = "dn_cap_tail_cls_cuda")]
    def run {
        cap_tail_body()
    }
}

//! the routed combine on the template both homes stamp (`GkMoeCombine`): a workgroup a position over its row; the hot expert pool's
//! hit rows added onto the host's sum under the slot window
[cuda_dispatch(name = "combine_cls", grid = "npos", params = "npos : int64")]
class CuMoeCombine : GkMoeCombine {
    [cuda_kernel(local_size_x = 256, name = "combine_cls_cuda")]
    def run {
        comb_body()
    }
}

//! `GkMoeCombine`'s by-row form for the per-expert chain: a workgroup a window row, its weighted down row added onto its
//! position's accumulated row (the chain zeroes the accumulator at its first expert)
[cuda_dispatch(name = "combine_row_cls", grid = "rows", params = "rows : int64")]
class CuMoeCombineRow : GkMoeCombine {
    override GATHERED = false
    override BYROW = true

    [cuda_kernel(local_size_x = 256, name = "combine_row_cls_cuda"), arch(at="../ARCHITECTURE_GPU_VULKAN_STREAM.md#streamed-layer-split")]
    def run {
        comb_body()
    }
}

//! the NextN head's eh_proj input rows, [a row ; b row] a row (`GkCat2`, the template both homes stamp)
[cuda_dispatch(name = "cat2_cls", grid = "wgs", params = "wgs : int64")]
class CuCat2 : GkCat2 {
    [cuda_kernel(local_size_x = 256, name = "cat2_cls_cuda")]
    def run {
        cat2_body()
    }
}

//! the window's f32 beta/alpha tile GEMM (`DnBaGemmT`) over the window's f16 rows
[cuda_dispatch(name = "dn_ba_cls", grid = "wgs", params = "wgs : int64")]
class CuDnBaGemm : DnBaGemmT {
    typedef XT = float16

    [cuda_kernel(local_size_x = 256, name = "dn_ba_cls_cuda")]
    def run {
        dn_ba_body()
    }
}

//! the fused deltanet decode step (`DnStepFusedT`), silu and sigmoid out-gates
[cuda_dispatch(name = "dn_step_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuDnStep : DnStepFusedT {
    [cuda_kernel(local_size_x = 256, name = "dn_step_cls_cuda")]
    def run {
        dn_step_body()
    }
}

//! the fused decode step's sigmoid out-gate stamp (`DnStepFusedT`)
[cuda_dispatch(name = "dn_step_sig_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuDnStepSig : DnStepFusedT {
    override ZSIG = true

    [cuda_kernel(local_size_x = 256, name = "dn_step_sig_cls_cuda")]
    def run {
        dn_step_body()
    }
}

//! the window's router GEMM (`RouterGemmT`)
[cuda_dispatch(name = "router_gemm_cls", grid = "wgs", params = "wgs : int64")]
class CuRouterGemm : RouterGemmT {
    [cuda_kernel(local_size_x = 256, name = "router_gemm_cls_cuda")]
    def run {
        router_gemm_body()
    }
}

//! the decode's per-row select with its slot records (`TopKRecords` under META): a row a workgroup, one logit a thread to 256 experts
[cuda_dispatch(name = "topk_n_cls", grid = "npos", params = "npos : int64")]
class CuTopKN : TopKRecords {
    [cuda_kernel(local_size_x = 256, name = "topk_n_cls_cuda")]
    def run {
        row_picks(gl_WorkGroupID.x)
    }
}

//! ... two logits a thread past 256 experts (`TopKRecords`)
[cuda_dispatch(name = "topk_n2_cls", kernel = "run", family = "topk_n_cls", grid = "npos", params = "npos : int64")]
class CuTopKN2 : TopKRecords {
    override DEPTH = 2

    [cuda_kernel(local_size_x = 256, name = "topk_n2_cls_cuda")]
    def run {
        row_picks(gl_WorkGroupID.x)
    }
}

//! the window's per-row select (`TopKRecords`): the picks and weights alone, which the bucket schedule reads
[cuda_dispatch(name = "topk_rows_cls", grid = "npos", params = "npos : int64")]
class CuTopKRows : TopKRecords {
    override META = false

    [cuda_kernel(local_size_x = 256, name = "topk_rows_cls_cuda")]
    def run {
        row_picks(gl_WorkGroupID.x)
    }
}

//! ... two logits a thread past 256 experts (`TopKRecords`)
[cuda_dispatch(name = "topk_rows2_cls", kernel = "run", family = "topk_rows_cls", grid = "npos", params = "npos : int64")]
class CuTopKRows2 : TopKRecords {
    override META = false
    override DEPTH = 2

    [cuda_kernel(local_size_x = 256, name = "topk_rows2_cls_cuda")]
    def run {
        row_picks(gl_WorkGroupID.x)
    }
}

//! the window's bucket schedule (`MoeSchedT`), a thread an expert: 256 experts, and the 512-thread stamp past them
[cuda_dispatch(name = "moe_sched_cls", grid = "one", params = "one : int64")]
class CuMoeSched : MoeSchedT {
    [cuda_kernel(local_size_x = 256, name = "moe_sched_cls_cuda")]
    def run {
        sched_body()
    }
}

//! the 512-thread bucket schedule past 256 experts (`MoeSchedT`)
[cuda_dispatch(name = "moe_sched2_cls", kernel = "run", family = "moe_sched_cls", grid = "one", params = "one : int64")]
class CuMoeSched2 : MoeSchedT {
    override WG = 512u

    [cuda_kernel(local_size_x = 512, name = "moe_sched2_cls_cuda")]
    def run {
        sched_body()
    }
}

let MMQ_SCHED_ROW_PAD = 128u   //! the FP4 expert chain's bucket row pad: a bucket's rows start at a tile's multiple, so a tile past them lands in its pad

//! the bucket schedule the FP4 expert chain reads (`MoeSchedT`): every bucket padded to the MMQ tile and on the m column, no site
//! asking the s column alone
[cuda_dispatch(name = "moe_sched_mmq_cls", kernel = "run", family = "moe_sched_cls", grid = "one", params = "one : int64")]
class CuMoeSchedMmq : MoeSchedT {
    override ROW_PAD = 128u   //! `MMQ_SCHED_ROW_PAD`: a template constant takes the literal
    override M_ONLY = true
    override S_ONLY_ARG = false

    [cuda_kernel(local_size_x = 256, name = "moe_sched_mmq_cls_cuda")]
    def run {
        sched_body()
    }
}

//! the 512-thread MMQ-padded bucket schedule past 256 experts (`MoeSchedT`)
[cuda_dispatch(name = "moe_sched2_mmq_cls", kernel = "run", family = "moe_sched_cls", grid = "one", params = "one : int64")]
class CuMoeSched2Mmq : MoeSchedT {
    override WG = 512u
    override ROW_PAD = 128u   //! `MMQ_SCHED_ROW_PAD`: a template constant takes the literal
    override M_ONLY = true
    override S_ONLY_ARG = false

    [cuda_kernel(local_size_x = 512, name = "moe_sched2_mmq_cls_cuda")]
    def run {
        sched_body()
    }
}

//! the routed block's gather (`MoeGatherT`): every slot row's normed row into its bucket row as halves, the expert tiles' feed
[cuda_dispatch(name = "moe_gather_f16_cls", grid = "nk", params = "nk : int64")]
class CuMoeGatherF16 : MoeGatherT {
    typedef XG = float16

    [cuda_kernel(local_size_x = 256, name = "moe_gather_f16_cls_cuda")]
    def run {
        gather_body()
    }
}

//! the per-expert chain's gather (`MoeGatherT` BYROW): a workgroup a window row, over the rows of one expert alone
[cuda_dispatch(name = "moe_gather_f16_row_cls", grid = "rows", params = "rows : int64")]
class CuMoeGatherF16Row : MoeGatherT {
    typedef XG = float16
    override BYROW = true

    [cuda_kernel(local_size_x = 256, name = "moe_gather_f16_row_cls_cuda"), arch(at="../ARCHITECTURE_GPU_VULKAN_STREAM.md#streamed-layer-split")]
    def run {
        gather_body()
    }
}

//! the combine on the residual step (`ArCombBase`): the shared expert's row at its gate, the k weighted routed rows through the
//! slot map under their down biases, then the next norm as floats - a row a workgroup, the token step's and the window's
[cuda_dispatch(name = "cls_ar_comb", kernel = "run", grid = "nrows", params = "nrows : int64")]
class CuClsArComb : ArCombBase {
    override OUT_F32 = true

    [cuda_kernel(local_size_x = 256, name = "cls_ar_comb_cuda")]
    def run {
        ar_body()
    }
}

//! the gemma-4 combine on the template both homes stamp (`ArCombG4T`): the routed sum and the dense row each under its post norm,
//! their sum under the post-FFN norm onto the residual, the next norm as floats - a row a workgroup, the token step's and the window's
[cuda_dispatch(name = "cls_ar_comb_g4", kernel = "run", grid = "nrows", params = "nrows : int64")]
class CuClsArCombG4 : ArCombG4T {
    override OUT_F32 = true

    [cuda_kernel(local_size_x = 256, name = "cls_ar_comb_g4_cuda")]
    def run {
        comb_g4_body()
    }
}

//! the gather as floats (`MoeGatherT` F16 off): the rows the FP4 expert chain's MXFP4 feed reads
[cuda_dispatch(name = "moe_gather_f32_cls", grid = "nk", params = "nk : int64")]
class CuMoeGatherF32 : MoeGatherT {
    typedef XG = float
    override F16 = false

    [cuda_kernel(local_size_x = 256, name = "moe_gather_f32_cls_cuda")]
    def run {
        gather_body()
    }
}

//! the act to the f16 hidden plane under the picks' gate and up bias rows (`ActF16T` BIASED): gpt-oss's stacks
[cuda_dispatch(name = "actf16_b_cls", grid = "wgs", params = "wgs : int64")]
class CuActF16B : ActF16T {
    override BIASED = true

    [cuda_kernel(local_size_x = 256, name = "actf16_b_cls_cuda")]
    def run {
        actf16_body()
    }
}

//! the decode's per-row select over `ne` experts on the stamp its depth names
def cud_topk_n_ensure(ne : int64) : bool => topk_depth(ne) > 1 ? ensure_topk_n2_cls() : ensure_topk_n_cls()

def cud_topk_n_enc(ne : int64; var s : void?; bufs : uint64 const[8]; var pc : GkTopKArgs; npos : int64) {
    if (topk_depth(ne) > 1) {
        enc_topk_n2_cls(s, bufs, pc, npos)
    } else {
        enc_topk_n_cls(s, bufs, pc, npos)
    }
}

//! the window's per-row select over `ne` experts, the same way
def cud_topk_rows_ensure(ne : int64) : bool => topk_depth(ne) > 1 ? ensure_topk_rows2_cls() : ensure_topk_rows_cls()

def cud_topk_rows_enc(ne : int64; var s : void?; bufs : uint64 const[5]; var pc : GkTopKArgs; npos : int64) {
    if (topk_depth(ne) > 1) {
        enc_topk_rows2_cls(s, bufs, pc, npos)
    } else {
        enc_topk_rows_cls(s, bufs, pc, npos)
    }
}

//! the window's bucket schedule over `ne` experts on the stamp that reaches them
//! the schedule stamp for `ne` experts: the 512-thread one past 256, the MMQ-padded one under `mmq`
def cud_moe_sched_ensure(ne : int64; mmq : bool) : bool =>
    mmq ? (topk_depth(ne) > 1 ? ensure_moe_sched2_mmq_cls() : ensure_moe_sched_mmq_cls()) : (topk_depth(ne) > 1 ? ensure_moe_sched2_cls() : ensure_moe_sched_cls())

def cud_moe_sched_enc(ne : int64; mmq : bool; var s : void?; bufs : uint64 const[6]; var pc : SchedArgs) {
    if (mmq) {
        if (topk_depth(ne) > 1) {
            enc_moe_sched2_mmq_cls(s, bufs, pc, 1l)
        } else {
            enc_moe_sched_mmq_cls(s, bufs, pc, 1l)
        }
    } elif (topk_depth(ne) > 1) {
        enc_moe_sched2_cls(s, bufs, pc, 1l)
    } else {
        enc_moe_sched_cls(s, bufs, pc, 1l)
    }
}
