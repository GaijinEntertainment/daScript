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
require dasllama/dasllama_gpu_math
require dasllama/dasllama_gpu_tier
require dasllama/dasllama_kv_dtype
require dasllama/dasllama_gpu_kernels_common public
require dasllama/dasllama_gpu_kernels_discrete public
require daslib/templates
require daslib/typemacro_boost
require math
require daslib/math_bits

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

//! the act + requant of a gate / up pair on `GkQ8BlockStore`, into Q8_K superblocks
[cuda_dispatch(name = "q8k_actrq_cls", kernel = "run", family = "actrq_cls", grid = "wgs", params = "wgs : int64")]
class CuQ8kActRq : ActRqT {
    override KBLK = true

    [cuda_kernel(local_size_x = 256, name = "q8k_actrq_cls_cuda")]
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

//! the f16-mirror stamp of the fused q/k norm, rope and K/V store, the stores clamped
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
[spirv_inline]
def cud_iq4_word(n : uint) : uint {
    let sel = (n & 7u) | ((n >> 4u) & 0x70u) | ((n >> 8u) & 0x700u) | ((n >> 12u) & 0x7000u)
    let lo = byte_perm(IQ4NL_WORD0, IQ4NL_WORD1, sel)
    let hi = byte_perm(IQ4NL_WORD2, IQ4NL_WORD3, sel)
    let m = ((n >> 3u) & 0x01010101u) * 0xFFu
    return (lo & ~m) | (hi & m)
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
def cud_gemv_ensure(fmt : int) : bool {
    var r = false
    kq_tile_stamp(fmt, CUD_FMT_SKIP, "gemv") <| $() {
        r = tile_ensure_cls()
    }
    return r
}

//! `bufs` is the family's [wq, ws, regions, xq, xs, y]; a kq leaf gets the grid codebooks appended as its seventh
def cud_gemv_cls_enc(fmt : int; var s : void?; bufs : uint64 const[6]; var pc : GemvArgs; wgs : int64) {   // nolint:STYLE037 — kq_tile_stamp's generated arms, one a format
    if (fmt == int(KqFmt.q8)) {
        enc_q8_gemv_cls(s, bufs, pc, wgs)
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
def cud_emb_ensure(fmt : int) : bool {
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

//! the format no tile serves: mx4 is an expert-only form with no dense plane on the slice
let private CUD_CM2_SKIP = "mx4"

//! the cm2 tile of format `fmt` at column `tc` loads
def cud_cm2_ensure(fmt, tc : int) : bool {   // nolint:STYLE037 — kq_tile_stamp's generated arms, one a format's column
    var r = false
    kq_tile_stamp(fmt, CUD_CM2_SKIP, "batch", tc, "cm2l_cls cm2m_cls cm2s_cls") <| $() {
        r = tile_ensure()
    }
    return r
}

//! `tc` 0 is the l tile (256 tokens), 1 the m tile (128), 2 the s tile (32); `bufs` is [wq, ws, schedule, xf, y]
def cud_cm2_enc(fmt, tc : int; var s : void?; bufs : uint64 const[5]; var pc : BatchArgs; wgs : int64) {   // nolint:STYLE037 — kq_tile_stamp's generated arms, one a format's column
    kq_tile_stamp(fmt, CUD_CM2_SKIP, "batch", tc, "cm2l_cls cm2m_cls cm2s_cls") <| $() {
        tile_enc(s, bufs, pc, wgs)
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
class template Q8MmqT {
    @ssbo @binding = 0 @readonly wq4 : array<uint4>    //! Q8_0 blocks as two uint4: (row, kb) at (row * nb + kb) * 2
    @ssbo @binding = 1 @readonly ws4 : array<uint4>    //! weight scales as halves, (row, kb) at row * nb + kb
    @ssbo @binding = 2 @readonly xq4 : array<uint4>    //! the activation quants, the same shape over tokens
    @ssbo @binding = 3 @readonly xs4 : array<uint4>    //! activation scales as floats, (token, kb) at token * nb + kb
    @ssbo @binding = 4 y : array<float>                //! [token][d]
    @kargs pa : MmqArgs
    @workgroup sa : uint4[2304]     //! two stages x 128 rows x nine uint4 (four blocks and a pad)
    @workgroup sb : uint4[2304]
    @workgroup sws : float2[512]    //! row-scale pairs: slot (r >> 4) * 8 + (r & 7) holds rows g, g + 8
    @workgroup sxs : float4[256]    //! token-scale quads: slot (c >> 4) * 4 + ((c & 7) >> 1)

    [spirv_inline]
    def mmq_body {   // nolint:STYLE038 - the pipeline and its unrolled products, register- and barrier-coupled
        let nb = pa.n / 32u
        let nb4 = pa.n / 128u
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
        cp_async_16(unsafe(addr(sa[sdst])), unsafe(addr(wq4[wsrc])))
        cp_async_16(unsafe(addr(sa[sdst + 576u])), unsafe(addr(wq4[wsrc + src2])))
        cp_async_16(unsafe(addr(sb[sdst])), unsafe(addr(xq4[xsrc])))
        cp_async_16(unsafe(addr(sb[sdst + 576u])), unsafe(addr(xq4[xsrc + src2])))
        cp_async_commit()
        var wcur = uint4(0u)
        var xcur = uint4(0u)
        if (wstage) {
            wcur = ws4[wb8 + (m0 + wr) * nb8]
            let h0 = unpackHalf2x16(wcur.x)
            let h1 = unpackHalf2x16(wcur.y)
            sws[wslot][int(wcomp)] = h0.x
            sws[64u + wslot][int(wcomp)] = h0.y
            sws[128u + wslot][int(wcomp)] = h1.x
            sws[192u + wslot][int(wcomp)] = h1.y
        } elif (xstage) {
            xcur = xs4[((t0 + xc) * nb) / 4u]
            sxs[xslot][int(xcomp)] = uint_bits_to_float(xcur.x)
            sxs[32u + xslot][int(xcomp)] = uint_bits_to_float(xcur.y)
            sxs[64u + xslot][int(xcomp)] = uint_bits_to_float(xcur.z)
            sxs[96u + xslot][int(xcomp)] = uint_bits_to_float(xcur.w)
            if (nb4 > 1u) {
                xcur = xs4[((t0 + xc) * nb) / 4u + 1u]
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
                    cp_async_16(unsafe(addr(sa[st2 * 1152u + sdst])), unsafe(addr(wq4[wsrc + nsrc])))
                    cp_async_16(unsafe(addr(sa[st2 * 1152u + sdst + 576u])), unsafe(addr(wq4[wsrc + nsrc + src2])))
                    cp_async_16(unsafe(addr(sb[st2 * 1152u + sdst])), unsafe(addr(xq4[xsrc + nsrc])))
                    cp_async_16(unsafe(addr(sb[st2 * 1152u + sdst + 576u])), unsafe(addr(xq4[xsrc + nsrc + src2])))
                    if (wstage) {
                        let h0 = unpackHalf2x16(q == 0 ? wcur.z : wcur.x)   // step nk's four halves: words 2 (nk & 1) .. of its pair's eight
                        let h1 = unpackHalf2x16(q == 0 ? wcur.w : wcur.y)
                        sws[st2 * 256u + wslot][int(wcomp)] = h0.x
                        sws[st2 * 256u + 64u + wslot][int(wcomp)] = h0.y
                        sws[st2 * 256u + 128u + wslot][int(wcomp)] = h1.x
                        sws[st2 * 256u + 192u + wslot][int(wcomp)] = h1.y
                        if (q == 0 && kb2 + 2u < nb4) {   // the quad's last word read: the next pair's quad takes its registers a step ahead
                            wcur = ws4[wb8 + (m0 + wr) * nb8 + (kb2 >> 1u) + 1u]
                        }
                    } elif (xstage) {
                        sxs[st2 * 128u + xslot][int(xcomp)] = uint_bits_to_float(xcur.x)
                        sxs[st2 * 128u + 32u + xslot][int(xcomp)] = uint_bits_to_float(xcur.y)
                        sxs[st2 * 128u + 64u + xslot][int(xcomp)] = uint_bits_to_float(xcur.z)
                        sxs[st2 * 128u + 96u + xslot][int(xcomp)] = uint_bits_to_float(xcur.w)
                        if (nk + 1u < nb4) {
                            xcur = xs4[((t0 + xc) * nb) / 4u + nk + 1u]
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
class CuQ8Mmq : Q8MmqT {
    [cuda_kernel(local_size_x = 512, name = "q8_mmq_cuda")]
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

//! the flash tile at head 96 (`FaH96T`): phi-3's heads, 32 keys a step
[cuda_dispatch(name = "fa_cm2_h96_cls", kernel = "run", family = "fa_cm2_cls", grid = "wgs", params = "wgs : int64")]
class CuFaCm2H96 : FaH96T {
    typedef OT = float

    [cuda_kernel(local_size_x = 128, name = "fa_cm2_h96_cls_cuda")]
    def run {
        fa_body()
    }
}

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
