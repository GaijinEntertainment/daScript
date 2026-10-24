options gen2
options indenting = 4
options _dasllama_internal = true

module dasllama_vulkan_classes shared public

require dasllama/dasllama_lint public
require spirv/spirv_shader
require vulkan
require vulkan/vulkan_boost
require dasllama/dasllama_vulkan_common
require dasllama/dasllama_vulkan_dispatch
require dasllama/dasllama_kqformat
require dasllama/dasllama_kv_dtype   // KVDtype: the K/V mirror codec the stamp picks key on
require dasllama/dasllama_gpu_math
require dasllama/dasllama_gpu_tier   // sel_lane_val and the SEL_* landing layout the classifier tail's kernels share with the host
require dasllama/dasllama_gpu_kernels_common public
require dasllama/dasllama_gpu_kernels_discrete public
require dasllama/dasllama_math
require daslib/templates   // [template]: the per-32 decode's compile-time format tag
require daslib/typemacro_boost  // [template_struct_instance] — the KV-codec kernel templates
require daslib/math_bits   // uint_bits_to_float: the KHR tile's f32 store bounce read back through its uint staging array
require daslib/enum_trait   // kq_gemv_fmts walks the enum
require daslib/strings_boost
require math

//! the flash tiles' head gates - the pre-flight and the window chain pick from these predicates over their arguments alone
//! the head sizes a flash tile is stamped at
def fa_head_ok(hs : int64) : bool => hs == 64l || hs == 128l || hs == 256l || hs == 512l

//! the KHR flash tile serves a head of 64, 128, 256 or 512 on an ungated model, the softcap at 256 alone (its cap stamps)
def fa_khr_serves(hs : int64; cap : float; gated : bool) : bool
    => fa_head_ok(hs) && !gated && (cap == 0.0 || hs == 256l)

//! the cm2 flash tile serves a head of 64, 96, 128, 256 or 512 (96 is its alone: the KHR lane map walks a head in 64-wide
//! halves), gated at 256 alone, the softcap at an ungated 256 alone
def fa_cm2_serves(hs : int64; cap : float; gated : bool) : bool
    => (fa_head_ok(hs) || hs == 96l) && (!gated || hs == 256l) && (cap == 0.0 || (hs == 256l && !gated))


// ===== add + rmsnorm =====


[vk_dispatch(name = "cls_ar", kernel = "run", grid = "nrows", params = "nrows : int64")]
class ClsArAddRms : ArBase {
    override OUT_F32 = true

    [spirv_kernel(local_size_x = 256)]
    def run {
        ar_body()
    }
}


[vk_dispatch(name = "cls_ar_rq_b", grid = "nrows", params = "nrows : int64")]
class ClsArAddRmsRqB : ArRqBase {
    [spirv_kernel(local_size_x = 256, name = "cls_ar_rq_b_spv")]
    def run {
        rq_b_body()
    }
}


[vk_dispatch(name = "cls_ar_rqx", grid = "wgs", params = "wgs : int64")]
class ArAddRmsRqX : ArRqT {
    override STOREX = true

    [spirv_kernel(local_size_x = 256)]
    def run {
        ar_rq_body()
    }
}

[vk_dispatch(name = "cls_ar_rqk", grid = "wgs", params = "wgs : int64")]
class ArAddRmsRqK : ArRqT {
    override KBLK = true

    [spirv_kernel(local_size_x = 256)]
    def run {
        ar_rq_body()
    }
}

[vk_dispatch(name = "cls_ar_rqkx", grid = "wgs", params = "wgs : int64")]
class ArAddRmsRqKX : ArRqT {
    override KBLK = true
    override STOREX = true

    [spirv_kernel(local_size_x = 256)]
    def run {
        ar_rq_body()
    }
}


[vk_dispatch(name = "cls_ar_comb", kernel = "run", grid = "nrows", params = "nrows : int64")]
class ClsArComb : ArCombBase {
    override OUT_F32 = true

    [spirv_kernel(local_size_x = 256)]
    def run {
        ar_body()
    }
}

//! the plain combine's quantizing twin (`ClsArCombG4Rq`'s form): the normed row into Q8_0 blocks at the combine's free bindings, so no requant dispatch follows
[vk_dispatch(name = "cls_ar_comb_rq", kernel = "run", grid = "nrows", params = "nrows : int64")]
class ClsArCombRq : ArCombBase {
    override OUT_Q8 = true

    [spirv_kernel(local_size_x = 256)]
    def run {
        ar_body()
    }
}

[vk_dispatch(name = "cls_ar_comb_f16_b", kernel = "run", grid = "nrows", params = "nrows : int64")]
class ClsArCombF16B : ArCombBase {
    override OUT_F16 = true

    [spirv_kernel(local_size_x = 256)]
    def run {
        ar_body()
    }
}


//! off `ArBase`, the `cls_ar_comb*` siblings' shell: the four-reduce step over the slot map's routed rows and the dense row, its next norm as floats
[vk_dispatch(name = "cls_ar_comb_g4", kernel = "run", grid = "nrows", params = "nrows : int64")]
class ClsArCombG4 : ArCombG4T {
    override OUT_F32 = true

    [spirv_kernel(local_size_x = 256)]
    def run {
        comb_g4_body()
    }
}

//! off `ArBase`, the `cls_ar_comb*` siblings' shell: the four-reduce step over the slot map's routed rows and the dense row, its next norm as f16
[vk_dispatch(name = "cls_ar_comb_g4_f16_b", kernel = "run", grid = "nrows", params = "nrows : int64")]
class ClsArCombG4F16B : ArCombG4T {
    override OUT_F16 = true

    [spirv_kernel(local_size_x = 256)]
    def run {
        comb_g4_body()
    }
}

//! off `ArBase`, the `cls_ar_comb*` siblings' shell: the four-reduce step over the slot map's routed rows and the dense row, its next norm in Q8_0 blocks
[vk_dispatch(name = "cls_ar_comb_g4_rq", kernel = "run", grid = "nrows", params = "nrows : int64")]
class ClsArCombG4Rq : ArCombG4T {
    override OUT_Q8 = true

    [spirv_kernel(local_size_x = 256)]
    def run {
        comb_g4_body()
    }
}

[vk_dispatch(name = "cls_ar_f16_b", kernel = "run", grid = "nrows", params = "nrows : int64")]
class ClsArAddRmsF16B : ArBase {
    override OUT_F16 = true

    [spirv_kernel(local_size_x = 256)]
    def run {
        ar_body()
    }
}


[vk_dispatch(name = "cls_dn_rq", kernel = "run", family = "rq_cls", grid = "wgs", params = "wgs : int64")]
class DnRequant : RqPlainT {
    [spirv_kernel(local_size_x = 256)]
    def run {
        rq_body()
    }
}

[vk_dispatch(name = "cls_q8k_rq", kernel = "run", family = "rq_cls", grid = "wgs", params = "wgs : int64")]
class Q8kRequant : RqPlainT {
    override KBLK = true

    [spirv_kernel(local_size_x = 256)]
    def run {
        rq_body()
    }
}

//! the requant of `elems` floats from element `inbase` of a `rq_cls` set into the feed a plane reads: Q8_K superblocks where
//! `kq` (a `kq_sb` format), Q8_0 blocks otherwise - the one spelling of the pair every host requant site records
def rq_enc(raw : VkCommandBuffer; var h : VkHaz; var rq_set : VkDescriptorSet; kq : bool; inbase, elems : int64) {
    var pcr = RqArgs(inbase = uint(inbase), nblk = uint(elems / kq_unit(kq)))
    if (kq) {
        enc_cls_q8k_rq(raw, h, rq_set, pcr, rq_wgs(kq, elems))
    } else {
        enc_cls_dn_rq(raw, h, rq_set, pcr, rq_wgs(kq, elems))
    }
}

//! the same from element 0 into the feed of a plane in format `fmt` (the decode's per-plane sites)
def rq_enc(raw : VkCommandBuffer; var h : VkHaz; var rq_set : VkDescriptorSet; fmt : int; elems : int64) {
    rq_enc(raw, h, rq_set, kq_sb(fmt), 0l, elems)
}


//! the activation + requant of `elems` gate/up floats into the feed the down plane reads (`pca` carries the site's act code
//! and its offsets; its block count is set here): Q8_K where `kq`, Q8_0 otherwise - the one spelling of the pair
def actrq_enc(raw : VkCommandBuffer; var h : VkHaz; var act_set : VkDescriptorSet; kq : bool; var pca : ActArgs; elems : int64) {
    pca.nblk = uint(elems / kq_unit(kq))
    if (kq) {
        enc_q8k_actrq_cls(raw, h, act_set, pca, rq_wgs(kq, elems))
    } else {
        enc_q8_actrq_cls(raw, h, act_set, pca, rq_wgs(kq, elems))
    }
}

// ===== act + requant (the FFN mid-chain elementwise family) =====


[vk_dispatch(name = "q8_actrq_cls", kernel = "run", family = "actrq_cls", grid = "wgs", params = "wgs : int64")]
class Q8ActRq : ActRqT {
    [spirv_kernel(local_size_x = 256)]
    def run {
        rq_body()
    }
}

//! the biased twin of the fused act + Q8_0 requant: element e's row (e / brow) names the expert whose gate and up bias rows join before the activation
[vk_dispatch(name = "q8_actrq_b_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Q8ActRqB : ActRqT {
    @ssbo @binding = 4 @role = "weight" gb : array<float>   //! the gate-bias plane
    @ssbo @binding = 5 @role = "weight" ub : array<float>   //! the up-bias plane
    @ssbo @binding = 6 rex : array<uint>     //! row -> its expert

    //! the biased v, u pair of element e (the gate row's element and the up element at ue)
    def biased(e, ue : uint) : float2 {
        let ex = rex[e / pa.brow]
        let c = e % pa.brow
        return float2(gate[e] + gb[pa.gboff + ex * pa.brow + c], up[ue] + ub[pa.uboff + ex * pa.brow + c])
    }

    def override load4(eb : uint) : float4 {
        let ue = up_at(eb)
        let p0 = biased(eb, ue)
        let p1 = biased(eb + 1u, ue + 1u)
        let p2 = biased(eb + 2u, ue + 2u)
        let p3 = biased(eb + 3u, ue + 3u)
        return float4(act_mul(p0.x, p0.y), act_mul(p1.x, p1.y), act_mul(p2.x, p2.y), act_mul(p3.x, p3.y))
    }

    [spirv_kernel(local_size_x = 256)]
    def run {
        rq_body()
    }
}

// the Q8_K twin: one subgroup per 256-weight superblock, whole-subgroup 5-shuffle amax
[vk_dispatch(name = "q8k_actrq_cls", kernel = "run", family = "actrq_cls", grid = "wgs", params = "wgs : int64")]
class Q8kActRq : ActRqT {
    override KBLK = true

    [spirv_kernel(local_size_x = 256)]
    def run {
        rq_body()
    }
}


[vk_dispatch(name = "actf16_cls", grid = "wgs", params = "wgs : int64")]
class ActF16 : ActF16T {
    [spirv_kernel(local_size_x = 256)]
    def run {
        actf16_body()
    }
}

[vk_dispatch(name = "actf16_b_cls", grid = "wgs", params = "wgs : int64")]
class ActF16B : ActF16T {
    override BIASED = true

    [spirv_kernel(local_size_x = 256)]
    def run {
        actf16_body()
    }
}

//! the per-layer-embedding branch's act as floats, the f32 projection's x, on the template both homes stamp (`GkPleAct`): out of
//! place, an element a thread
[vk_dispatch(name = "ple_act_f32_cls", grid = "wgs", params = "wgs : int64")]
class PleActF32 : GkPleAct {
    override OUT = true

    [spirv_kernel(local_size_x = 256), arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#whole-token-decode-span")]
    def run {
        act_elem(gl_GlobalInvocationID.x)
    }
}

//! the E-series pre-step's finish on the template both homes stamp (`GkPleFinish`): a workgroup a slice, the projection rows
//! arriving scaled (the router GEMV's `YSCALE`, the window's scaled f16 rows)
[vk_dispatch(name = "ple_finish_cls", grid = "nslices", params = "nslices : int64")]
class PleFinish : GkPleFinish {
    [spirv_kernel(local_size_x = 256)]
    def run {
        finish_slice(gl_WorkGroupID.x)
    }
}


[vk_dispatch(name = "emb_gather_cls", grid = "wgs", params = "wgs : int64")]
class EmbGather : EmbGatherT {
    [spirv_kernel(local_size_x = 256)]
    def run {
        emb_body()
    }
}

[vk_dispatch(name = "emb_gather_f32_cls", grid = "wgs", params = "wgs : int64")]
class EmbGatherF32 : EmbGatherT {
    override WF = true

    [spirv_kernel(local_size_x = 256)]
    def run {
        emb_body()
    }
}

//! the classifier tail's epilogue on the template both homes stamp (`GkClsEpilogue`), an invocation a logit
[vk_dispatch(name = "cls_epi", grid = "wgs", params = "wgs : int64")]
class ClsEpilogue : GkClsEpilogue {
    [spirv_kernel(local_size_x = 256, name = "cls_epi_spv")]
    def run {
        epi_body()
    }
}


[vk_dispatch(name = "cls_argmax_part", grid = "wgs", params = "wgs : int64")]
class ClsArgmaxPart : ClsArgmaxPartT {
    [spirv_kernel(local_size_x = 256, name = "cls_argmax_part_spv")]
    def run {
        part_body()
    }
}

[vk_dispatch(name = "cls_argmax_fin", grid = "rows", params = "rows : int64")]
class ClsArgmaxFin : ClsArgmaxFinT {
    [spirv_kernel(local_size_x = 256, name = "cls_argmax_fin_spv")]
    def run {
        fin_body()
    }
}

//! the selection kernel's twins of the host's `SEL_*` landing layout (`dasllama_gpu_tier.das`): the SPIR-V emitter reads a
//! `uint` literal `let` alone, so the kernel spells them again and the driver's arm verifies the two sets agree (`vk_rdec_prepare`)
let CLS_SELECT_K = 1024u        //! SEL_K: the survivors a row lands - every logit at or above the K-th largest, ties kept, the host sampler's candidate cap
let CLS_SELECT_RECENT = 128u    //! SEL_RECENT: the recent-window ids a row's sampler words carry, whose logits land beside the survivors for the penalties
let CLS_SEL_WORDS = 132u        //! SEL_WORDS: one row's sampler words
let CLS_SEL_HDR = 4u            //! SEL_HDR: one row's landed header words
let CLS_SEL_ASK_TEMP = 0u       //! SEL_ASK_TEMP
let CLS_SEL_ASK_COUNT = 1u      //! SEL_ASK_COUNT
let CLS_SEL_ASK_IDS = 4u        //! SEL_ASK_IDS
let CLS_SEL_HDR_COUNT = 0u      //! SEL_HDR_COUNT
let CLS_SEL_HDR_OVERFLOW = 1u   //! SEL_HDR_OVERFLOW
let CLS_SEL_HDR_MAX = 2u        //! SEL_HDR_MAX
let CLS_SEL_HDR_DEN = 3u        //! SEL_HDR_DEN
let CLS_SELECT_WG = 1024u       //! the selection's threads a row: a 262144-wide row is 256 strided loads a thread, and 32 warps hide their latency

//! whether the kernel's twins and the host's `SEL_*` layout agree - the driver's arm verifies it before the first ask
def cls_sel_layout_agrees : bool {
    let sizes = int64(CLS_SEL_WORDS) == SEL_WORDS && int64(CLS_SEL_HDR) == SEL_HDR && int64(CLS_SELECT_RECENT) == SEL_RECENT && int64(CLS_SELECT_K) == SEL_K
    let ask = int64(CLS_SEL_ASK_TEMP) == SEL_ASK_TEMP && int64(CLS_SEL_ASK_COUNT) == SEL_ASK_COUNT && int64(CLS_SEL_ASK_IDS) == SEL_ASK_IDS
    let hdr = int64(CLS_SEL_HDR_COUNT) == SEL_HDR_COUNT && int64(CLS_SEL_HDR_OVERFLOW) == SEL_HDR_OVERFLOW && int64(CLS_SEL_HDR_MAX) == SEL_HDR_MAX && int64(CLS_SEL_HDR_DEN) == SEL_HDR_DEN
    return sizes && ask && hdr
}

//! the selection's landing planes a row: the header words, the survivor ids, their logits, the recent-window logits - bytes of
//! plane `k` over `nb` rows, and plane `k`'s offset in the packed host landing (`k` = 4 the landing's whole size)
def cls_sel_plane_bytes(k, nb : int64) : int64 {
    if (k == 0l) return SEL_HDR * 4l * nb
    if (k == 3l) return SEL_RECENT * 4l * nb
    return SEL_CAP * 4l * nb
}

def cls_sel_plane_off(k, nb : int64) : int64 {
    var off = 0l
    for (j in range64(k)) {
        off += cls_sel_plane_bytes(j, nb)
    }
    return off
}

//! the Vulkan row kernels' value form of the common scan (`GkWgScan`): the exclusive prefix over the `WG` threads' v
[ |> template_struct_instance]
class template WgScanT : GkWgScan {
    def excl_scan(v : uint) : uint {
        var cum = 0u
        excl_scan_into(v, cum)
        return cum
    }
}

//! the classifier tail's selection on the template both homes stamp (`GkClsSelect`), a workgroup of 1024 threads a row
[vk_dispatch(name = "cls_select", grid = "rows", params = "rows : int64")]
class ClsSelect : GkClsSelect {
    [spirv_kernel(local_size_x = 1024, name = "cls_select_spv")]
    def run {
        select_row()
    }
}

// ===== the MoE combine =====

//! the routed combine on the template both homes stamp (`GkMoeCombine`), one workgroup a position over its row; chunk-spanning
//! positions accumulate
[vk_dispatch(name = "combine_cls", grid = "npos", params = "npos : int64")]
class MoeCombine : GkMoeCombine {
    [spirv_kernel(local_size_x = 256, name = "combine_cls_spv")]
    def run {
        var i = gl_LocalInvocationID.x
        while (i < pa.n) {
            comb_elem(gl_WorkGroupID.x, i)
            i += 256u
        }
    }
}

//! GkMoeCombine's by-row form for the per-expert chain: one workgroup a window row, its weighted down row added onto its
//! position's accumulated row (the chain zeroes the accumulator at its first expert)
[vk_dispatch(name = "combine_row_cls", grid = "rows", params = "rows : int64")]
class MoeCombineRow : GkMoeCombine {
    override GATHERED = false
    override BYROW = true

    [spirv_kernel(local_size_x = 256, name = "combine_row_cls_spv"), arch(at="../ARCHITECTURE_GPU_VULKAN_STREAM.md#streamed-layer-split")]
    def run {
        var i = gl_LocalInvocationID.x
        while (i < pa.n) {
            comb_row_elem(gl_WorkGroupID.x, i)
            i += 256u
        }
    }
}

struct DecCombineArgs {
    n : uint      //! row width
    k : uint      //! routed slots
    gidx : uint   //! the shared expert's gate logit index in `gate` (the shared-row stamp)
    gated : uint  //! ... and whether that gate applies
}

//! the decode MoE combine, x + sum_j w_j d_j over the k routed rows; under `SH` the shared expert's row joins, gated by its logit
[ |> template_struct_instance]
class template DecCombineT {
    @template_constant SH : bool = false
    @ssbo @binding = 0 xin : array<float>        //! the residual row before the FFN
    @ssbo @binding = 1 w : array<float>          //! the k routing weights
    @ssbo @binding = 2 down_rows : array<float>  //! the routed down GEMV's k rows
    @ssbo @binding = 3 @template_gate = SH sh_row : array<float>   //! the shared expert's down GEMV row
    @ssbo @binding = 4 @template_gate = SH gate : array<float>     //! the logits row carrying the gate at pa.gidx
    @ssbo @binding = 5 xout : array<float>       //! the residual row after the FFN - past the shared-row members, one set order for both stamps
    @push_constant pa : DecCombineArgs

    [spirv_kernel(local_size_x = 256), arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#decode-attention-block"), arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#whole-token-decode-span")]
    def run {
        let i = gl_GlobalInvocationID.x
        if (i < pa.n) {
            var acc = 0.0
            var j = 0u
            while (j < pa.k) {
                acc += w[j] * down_rows[j * pa.n + i]
                j++
            }
            static_if (SH) {
                let g = pa.gated != 0u ? sigmoid_f32(gate[pa.gidx]) : 1.0
                xout[i] = xin[i] + acc + g * sh_row[i]
            } else {
                xout[i] = xin[i] + acc
            }
        }
    }
}

[vk_dispatch(name = "dec_combine_cls", grid = "wgs", params = "wgs : int64")]
class DecCombine : DecCombineT {}

[vk_dispatch(name = "dec_combine_sh_cls", grid = "wgs", params = "wgs : int64")]
class DecCombineSh : DecCombineT {
    override SH = true
}

//! the router GEMV on the template both homes stamp (`GkRouterGemv`): a workgroup of 256 an expert row, eight columns a dispatch,
//! the row read as its weightless rms under the norm row where `norm_on`
[ |> template_struct_instance]
class template RouterGemvT : GkRouterGemv {
    override NORM = true
    override YSCALE = true

    [spirv_kernel(local_size_x = 256), arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#whole-token-decode-span")]
    def run {
        router_row(gl_WorkGroupID.x, gl_LocalInvocationID.x, 256u)
    }
}

[vk_dispatch(name = "router_gemv_cls", grid = "ne", params = "ne : int64")]
class RouterGemv : RouterGemvT {
    typedef KT = float
    typedef XT = float
}

//! the bf16 rows read as bf16 pairs, one word a step (the E-series pre-step's projection, its own rows on the device)
[vk_dispatch(name = "router_gemv_bf16_cls", grid = "ne", params = "ne : int64")]
class RouterGemvBf16 : RouterGemvT {
    typedef KT = uint
    typedef XT = float
    override VW = 2u
    override BF16 = true
}

//! the router GEMV stamp of the template both homes stamp (`GkRouterGemv`) over the bf16 rows a halfword a lane, for an odd dim
//! or base: a row then starts mid-word, off the pairs' word grid
[vk_dispatch(name = "router_gemv_bf16_hw_cls", grid = "ne", params = "ne : int64")]
class RouterGemvBf16Hw : RouterGemvT {
    typedef KT = uint16
    typedef XT = float
    override BF16 = true
}

//! the pre-step projection's bf16 rows of model dim `dim` take the halfword stamp: every row starts at a multiple of `dim`, so an odd
//! dim puts every other row mid-word
def public ple_proj_bf16_hw(dim : int64) : bool => !gk_bf16_pairs(dim, 0l)

//! the router GEMV over `pc.ncols` columns (0 or 1 = one row), past `GEMV_COL_TILE` in tiles of that many, each tile's `col0` its first
//! column; `bf16` the bf16 rows' stamp (`hw` its halfword walk), else the f32 rows'
[arch(at="../ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-col-tiles")]
def router_enc_cols(bf16 : bool; raw : VkCommandBuffer; var h : VkHaz; var set_ : VkDescriptorSet; var pc : GkRouterArgs; ne : int64; hw : bool = false) {
    each_col_tile(pc, max(pc.ncols, 1u), GEMV_COL_TILE) $ {
        if (bf16 && hw) {
            enc_router_gemv_bf16_hw_cls(raw, h, set_, pc, ne)
        } elif (bf16) {
            enc_router_gemv_bf16_cls(raw, h, set_, pc, ne)
        } else {
            enc_router_gemv_cls(raw, h, set_, pc, ne)
        }
    }
}

struct DnBaArgs {
    npos : uint
    dim : uint
    nvh : uint       // beta rows (the alpha rows follow them in w)
    woff : uint      // this layer's [beta ; alpha] row base in w (elements)
    xstride : uint   // x row stride (elements)
    obase_b : uint   // beta output base in outp: row p's nvh values at obase_b + p * ostride
    obase_g : uint   // alpha output base, the same layout
    ostride : uint
}

[vk_dispatch(name = "dn_ba_cls", grid = "wgs", params = "wgs : int64")]
class DnBaGemm {
    @ssbo @binding = 0 @role = "weight" w : array<float>   //! the [beta ; alpha] rows per layer, the file's f32
    @ssbo @binding = 1 x : array<float>       // the normed rows
    @ssbo @binding = 2 outp : array<float>    // the layer smalls (beta / g rows, position-major)
    @push_constant pa : DnBaArgs
    @workgroup xs : float[528]                 // 16 positions x 32 k, stride 33 (bank-spread)
    @workgroup ws : float[528]                 // 16 rows x 32 k, stride 33

    [spirv_kernel(local_size_x = 256, name = "dn_ba_cls_spv"), arch(at="../ARCHITECTURE_GPU_VULKAN_WINDOW.md#vk-prefill-dn-block")]
    def run {
        let tid = gl_LocalInvocationID.x
        let ne = 2u * pa.nvh
        let egroups = (ne + 15u) / 16u
        let p0 = (gl_WorkGroupID.x / egroups) * 16u
        let eg0 = (gl_WorkGroupID.x % egroups) * 16u
        let p = tid / 16u
        let el = tid % 16u
        let e = eg0 + el
        let live = p0 + p < pa.npos && e < ne
        let r = tid / 32u
        let k = tid % 32u
        var a = 0.0
        var kb = 0u
        while (kb < pa.dim) {
            if (p0 + r < pa.npos) {
                xs[r * 33u + k] = x[(p0 + r) * pa.xstride + kb + k]
            } else {
                xs[r * 33u + k] = 0.0
            }
            if (p0 + r + 8u < pa.npos) {
                xs[(r + 8u) * 33u + k] = x[(p0 + r + 8u) * pa.xstride + kb + k]
            } else {
                xs[(r + 8u) * 33u + k] = 0.0
            }
            if (eg0 + r < ne) {
                ws[r * 33u + k] = w[pa.woff + (eg0 + r) * pa.dim + kb + k]
            } else {
                ws[r * 33u + k] = 0.0
            }
            if (eg0 + r + 8u < ne) {
                ws[(r + 8u) * 33u + k] = w[pa.woff + (eg0 + r + 8u) * pa.dim + kb + k]
            } else {
                ws[(r + 8u) * 33u + k] = 0.0
            }
            barrier()
            if (live) {
                var kk = 0u
                while (kk < 32u) {
                    a += xs[p * 33u + kk] * ws[el * 33u + kk]
                    kk++
                }
            }
            barrier()
            kb += 32u
        }
        if (live) {
            outp[(e < pa.nvh ? pa.obase_b : pa.obase_g) + (p0 + p) * pa.ostride + (e % pa.nvh)] = a
        }
    }
}

//! the top-k's records on the select both homes stamp (`GkTopKRow`): row `p`'s picks land in slots `p * k + j` - the expert and
//! its weight, and under `META` the slot's GEMV records (the decode's), off it the two alone (the window's per-row select, which
//! the bucket schedule reads)
[ |> template_struct_instance]
class template TopKRecords : GkTopKRow {
    @template_constant META : bool = true
    @ssbo @binding = 0 logits : array<float>  //! [npos x lstride]
    @ssbo @binding = 1 @template_gate = META meta1 : array<uint>   //! the gate stack's (wblock, xblock) per slot row
    @ssbo @binding = 2 @template_gate = META meta3 : array<uint>
    @ssbo @binding = 3 @template_gate = META meta2 : array<uint>
    @ssbo @binding = 4 w : array<float>      //! the routing weights, a slot each
    @ssbo @binding = 5 @role = "weight" ds : array<float>     //! the per-expert down scales (gemma-4), read at dsoff when the push names a row
    @ssbo @binding = 6 @role = "weight" rb : array<float>     //! the router biases ([ne] a layer, gpt-oss), read at rboff under `rbias_on`
    @ssbo @binding = 7 eidx : array<uint>    //! the picks' experts, a slot each: the bias rows the act and the combine read
    override DSCALE = true

    def override logit_at(p, e : uint) : float => logits[p * pa.lstride + e] + (pa.rbias_on != 0u ? rb[pa.rboff + e] : 0.0)
    def override down_scale(e : uint) : float => ds[pa.dsoff + e]

    //! row `p`: its logits selected, its k slots' records written by thread 0
    def row_picks(p : uint) {
        let tid = gl_LocalInvocationID.x
        select_row(p, pa.ne, pa.k, false)
        if (tid == 0u) {
            write_picks(p)
        }
    }

    def write_picks(p : uint) {
        var ws = 0.0
        pick_wsum_into(pa.k, ws)
        var jj = 0u
        while (jj < pa.k) {
            let s = p * pa.k + jj
            let ex = min(pick[jj], pa.ne - 1u)   // a NaN row leaves the sentinel: never a wild weight block
            var wj = 0.0
            pick_weight_into(jj, ex, pa.norm, ws, pa.wscale, wj)
            w[s] = wj
            eidx[s] = ex
            static_if (META) {
                meta1[s * 2u] = pa.base1 + ex * pa.stride1
                meta1[s * 2u + 1u] = p * pa.xnb1
                meta3[s * 2u] = pa.base3 + ex * pa.stride3
                meta3[s * 2u + 1u] = p * pa.xnb1
                meta2[s * 2u] = pa.base2 + ex * pa.stride2
                meta2[s * 2u + 1u] = s * pa.xnb2
            }
            jj++
        }
    }
}

//! a row a workgroup: the decode's per-row select with its slot records (`META`), or the window's picks alone
[ |> template_struct_instance]
class template TopKRowsT : TopKRecords {
    [spirv_kernel(local_size_x = 256), arch(at="../ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-token-command"), arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#whole-token-decode-span"),
     arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#vk-prefill-moe-block")]
    def run {
        row_picks(gl_WorkGroupID.x)
    }
}

[vk_dispatch(name = "topk_n_cls", grid = "npos", params = "npos : int64")]
class TopKN : TopKRowsT {}

[vk_dispatch(name = "topk_n2_cls", kernel = "run", family = "topk_n_cls", grid = "npos", params = "npos : int64")]
class TopKN2 : TopKRowsT {
    override DEPTH = 2
}

[vk_dispatch(name = "topk_rows_cls", grid = "npos", params = "npos : int64")]
class TopKRows : TopKRowsT {
    override META = false
}

[vk_dispatch(name = "topk_rows2_cls", kernel = "run", family = "topk_rows_cls", grid = "npos", params = "npos : int64")]
class TopKRows2 : TopKRowsT {
    override META = false
    override DEPTH = 2
}

//! the experts one select workgroup reaches: 256 threads, `TOPK_DEPTH_MAX` logits a thread
let TOPK_DEPTH_MAX = 2
let TOPK_EXPERTS_MAX = 256l * int64(TOPK_DEPTH_MAX)

//! the select stamps' depth for `ne` experts: one logit a thread to 256, two past it
def topk_depth(ne : int64) : int => ne > 256l ? 2 : 1

//! the decode's per-row select over `ne` experts on the stamp its depth names (`topk_n_cls`' one set serves both)
def topk_n_ensure(ne : int64) : bool => topk_depth(ne) > 1 ? ensure_topk_n2_cls() : ensure_topk_n_cls()

def topk_n_enc(ne : int64; raw : VkCommandBuffer; var h : VkHaz; var set_ : VkDescriptorSet; var pc : GkTopKArgs; npos : int64) {
    if (topk_depth(ne) > 1) {
        enc_topk_n2_cls(raw, h, set_, pc, npos)
    } else {
        enc_topk_n_cls(raw, h, set_, pc, npos)
    }
}

//! the window's per-row select over `ne` experts, the same way (`topk_rows_cls`' one set)
def topk_rows_ensure(ne : int64) : bool => topk_depth(ne) > 1 ? ensure_topk_rows2_cls() : ensure_topk_rows_cls()

def topk_rows_enc(ne : int64; raw : VkCommandBuffer; var h : VkHaz; var set_ : VkDescriptorSet; var pc : GkTopKArgs; npos : int64) {
    if (topk_depth(ne) > 1) {
        enc_topk_rows2_cls(raw, h, set_, pc, npos)
    } else {
        enc_topk_rows_cls(raw, h, set_, pc, npos)
    }
}

struct RouterGemmArgs {
    npos : uint      //! rows
    ne : uint        //! router rows (the experts, plus the shared expert's gate row when it rides along)
    dim : uint       //! row width
    woff : uint      //! this layer's row base in w (elements)
    ostride : uint   //! the logits row stride (elements)
}

let ROUTER_TP = 64u   //! the router tile's positions per workgroup
let ROUTER_TE = 32u   //! ... and router rows
let ROUTER_TH = ROUTER_TE / 2u   //! an invocation's two rows sit a half-tile apart: e and e + ROUTER_TH
let ROUTER_K4 = 16u   //! a 64-wide K step in float4
let ROUTER_LD = ROUTER_K4 + 1u   //! the stage's row stride in float4: the step plus one of pad, so the sixteen rows a warp reads (and a fetch writes) spread the eight bank groups

[vk_dispatch(name = "router_gemm_cls", grid = "wgs", params = "wgs : int64")]
class RouterGemm {
    @ssbo @binding = 0 @role = "weight" w4 : array<float4>   //! the router rows, row-major [ne x dim] per layer, as the stage's float4
    @ssbo @binding = 1 x4 : array<float4>      //! the FFN-normed rows, as float4
    @ssbo @binding = 2 logits : array<float>   //! [npos x ostride]
    @push_constant pa : RouterGemmArgs
    @workgroup xs : float4[int(ROUTER_TP * ROUTER_LD)]
    @workgroup ws : float4[int(ROUTER_TE * ROUTER_LD)]

    //! the stage's float4 of slot `s` (row s / ROUTER_K4, k4 s % ROUTER_K4) of x rows from `p0` at step `kb`; zero past the window
    def fetch_x(p0, kb, s : uint) : float4 {
        let row = s / ROUTER_K4
        if (p0 + row >= pa.npos) return float4(0.0)
        return x4[((p0 + row) * pa.dim + kb) / 4u + (s % ROUTER_K4)]
    }

    //! ... and of the router rows from `eg0`; zero past the experts
    def fetch_w(eg0, kb, s : uint) : float4 {
        let row = s / ROUTER_K4
        if (eg0 + row >= pa.ne) return float4(0.0)
        return w4[(pa.woff + (eg0 + row) * pa.dim + kb) / 4u + (s % ROUTER_K4)]
    }

    //! a fetched float4 into its stage row at k4 `s % ROUTER_K4`
    def stage_x(s : uint; v : float4) {
        xs[(s / ROUTER_K4) * ROUTER_LD + (s % ROUTER_K4)] = v
    }

    def stage_w(s : uint; v : float4) {
        ws[(s / ROUTER_K4) * ROUTER_LD + (s % ROUTER_K4)] = v
    }

    //! `a` plus the four products in k order - the scalar loop's sums to the bit
    def acc4(a : float; x, w : float4) : float {
        var s = a
        s += x.x * w.x
        s += x.y * w.y
        s += x.z * w.z
        s += x.w * w.w
        return s
    }

    [spirv_kernel(local_size_x = 256, name = "router_gemm_cls_spv"), arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#vk-prefill-moe-block")]
    def run {
        let tid = gl_LocalInvocationID.x
        let egroups = (pa.ne + ROUTER_TE - 1u) / ROUTER_TE
        let p0 = (gl_WorkGroupID.x / egroups) * ROUTER_TP
        let eg0 = (gl_WorkGroupID.x % egroups) * ROUTER_TE
        let tp = tid / ROUTER_TH   //! the invocation's four positions 4tp..4tp+3
        let te = tid % ROUTER_TH   //! ... and rows te, te + ROUTER_TH
        var a00 = 0.0
        var a01 = 0.0
        var a10 = 0.0
        var a11 = 0.0
        var a20 = 0.0
        var a21 = 0.0
        var a30 = 0.0
        var a31 = 0.0
        //! the stage: 64 x rows x 16 float4 = four per invocation, 32 w rows x 16 = two
        var xr0 = fetch_x(p0, 0u, tid)
        var xr1 = fetch_x(p0, 0u, tid + 256u)
        var xr2 = fetch_x(p0, 0u, tid + 512u)
        var xr3 = fetch_x(p0, 0u, tid + 768u)
        var wr0 = fetch_w(eg0, 0u, tid)
        var wr1 = fetch_w(eg0, 0u, tid + 256u)
        var kb = 0u
        while (kb < pa.dim) {
            stage_x(tid, xr0)
            stage_x(tid + 256u, xr1)
            stage_x(tid + 512u, xr2)
            stage_x(tid + 768u, xr3)
            stage_w(tid, wr0)
            stage_w(tid + 256u, wr1)
            barrier()
            let nb = kb + 64u
            if (nb < pa.dim) {
                xr0 = fetch_x(p0, nb, tid)
                xr1 = fetch_x(p0, nb, tid + 256u)
                xr2 = fetch_x(p0, nb, tid + 512u)
                xr3 = fetch_x(p0, nb, tid + 768u)
                wr0 = fetch_w(eg0, nb, tid)
                wr1 = fetch_w(eg0, nb, tid + 256u)
            }
            let xb = (4u * tp) * ROUTER_LD
            let wb = te * ROUTER_LD
            let wb1 = wb + ROUTER_TH * ROUTER_LD
            var kk = 0u
            while (kk < ROUTER_K4) {
                let x0 = xs[xb + kk]
                let x1 = xs[xb + ROUTER_LD + kk]
                let x2 = xs[xb + 2u * ROUTER_LD + kk]
                let x3 = xs[xb + 3u * ROUTER_LD + kk]
                let w0 = ws[wb + kk]
                let w1 = ws[wb1 + kk]
                a00 = acc4(a00, x0, w0)
                a01 = acc4(a01, x0, w1)
                a10 = acc4(a10, x1, w0)
                a11 = acc4(a11, x1, w1)
                a20 = acc4(a20, x2, w0)
                a21 = acc4(a21, x2, w1)
                a30 = acc4(a30, x3, w0)
                a31 = acc4(a31, x3, w1)
                kk++
            }
            barrier()
            kb = nb
        }
        let pa0 = p0 + 4u * tp
        let ea0 = eg0 + te
        store_pair(pa0, ea0, a00, a01)
        store_pair(pa0 + 1u, ea0, a10, a11)
        store_pair(pa0 + 2u, ea0, a20, a21)
        store_pair(pa0 + 3u, ea0, a30, a31)
    }

    //! position p's two logits at rows e, e + ROUTER_TH, each within the window and the experts
    def store_pair(p, e : uint; v0, v1 : float) {
        if (p >= pa.npos) return
        if (e < pa.ne) {
            logits[p * pa.ostride + e] = v0
        }
        if (e + ROUTER_TH < pa.ne) {
            logits[p * pa.ostride + e + ROUTER_TH] = v1
        }
    }
}

//! npos rows of ne router logits at a row stride of ne, over the router rows at woff: one workgroup per 64 rows x 32 router rows
def router_gemm_enc(raw : VkCommandBuffer; var h : VkHaz; var rset : VkDescriptorSet; npos, ne, dim, woff : int64) {
    var pc = RouterGemmArgs(npos = uint(npos), ne = uint(ne), dim = uint(dim), woff = uint(woff), ostride = uint(ne))
    enc_router_gemm_cls(raw, h, rset, pc, ((npos + int64(ROUTER_TP) - 1l) / int64(ROUTER_TP)) * ((ne + int64(ROUTER_TE) - 1l) / int64(ROUTER_TE)))
}

struct SchedArgs {
    nk : uint          //! slot rows (npos x k)
    ne : uint          //! experts (at most 256: one thread per expert)
    wt_gu : uint       //! the gate/up GEMMs' weight tiles (ceil(nfe / 128))
    wt_dn : uint       //! the down GEMM's (ceil(dim / 128))
    map_off : uint     //! the s dispatch's per-wg map word offset in every sched plane; the records precede it
    map_off_mgu : uint //! the m dispatch's map offset in the gate/up planes
    map_off_mdn : uint //! ... and in the down plane
    bound_gu : uint    //! the s dispatch's workgroups per gate/up plane; the map's tail past the schedule reads the sentinel
    bound_dn : uint
    bound_mgu : uint   //! the m dispatch's
    bound_mdn : uint
    base1 : uint       //! expert 0's block base in the gate plane (slab-local, in the plane's units)
    stride1 : uint     //! blocks per expert
    base3 : uint
    stride3 : uint
    base2 : uint
    stride2 : uint
    s_only : uint      //! every bucket on the s column, a 32-row tile a piece (the Q8-fed chain's sdot4 tiles); 0 = the ladder's m columns too
}

let SCHED_S_ROWS = 32u    //! the s column: the ladder's small-bucket and remainder tile
let SCHED_M_ROWS = 128u   //! the m column: whole columns over a big bucket, the last one partial
let CM2_TC_E = -1l        //! the class ladders' `tc` key for the expert schedule's e column: a sentinel no column width can be (the dense m stamp already keys by its 128 rows)

[arch(at="../ARCHITECTURE_GPU_SHARED_GEMM.md#khr-mm-kq-tile")]
def khr_cls_ensure(fmt : int) : bool {   // nolint:STYLE037 — kq_tile_stamp's generated arms, one a format's stamp
    var r = false
    kq_tile_stamp(fmt) <| $() {
        r = tile_ensure_khr_cls()
    }
    return r
}

[arch(at="../ARCHITECTURE_GPU_SHARED_GEMM.md#khr-mm-kq-tile")]
def khr_cls_set(fmt : int; bufs : uint64 const[5]; sizes : int64 const[5]; gbits : uint const[5]) : VkDescriptorSet {   // nolint:STYLE037 — kq_tile_stamp's generated arms, one a format's stamp
    var r : VkDescriptorSet
    kq_tile_stamp(fmt) <| $() {
        r = tile_set_khr_cls(bufs, sizes, gbits)
    }
    return r
}

[arch(at="../ARCHITECTURE_GPU_SHARED_GEMM.md#khr-mm-kq-tile")]
def khr_cls_enc(fmt : int; raw : VkCommandBuffer; var h : VkHaz; var s : VkDescriptorSet; var pc : BatchArgs; groups : int64) {   // nolint:STYLE037 — kq_tile_stamp's generated arms, one a format's stamp
    kq_tile_stamp(fmt) <| $() {
        tile_enc_khr_cls(raw, h, s, pc, groups)
    }
}

def private cm2_require_s_column(fmt : int; tc : int64) {
    if (tc != 32l) {
        panic("vk classes: an expert-only plane ({kq_fmt_of_slot(fmt)}) reached a dense tile column - its one cm2 stamp is the s tile")
    }
}

let private CM2_TILE_TAILS = "khr_cls cm2e_cls cm2l_cls cm2m_cls cm2s_cls"   //! the cm2 class ladders' tiles, by `cm2_tile_tail`
//! the (format, tile) pairs no stamp serves: mx4 is an expert-only form with no dense plane, so no dense l or m column
let private CM2_TILE_NONE = "mx4/cm2l_cls mx4/cm2m_cls"

//! the tile a cm2 class pick takes, its ordinal in CM2_TILE_TAILS: the KHR tile on a KHR-mode card, the e column for the expert
//! schedule's m pieces, the s column on an expert-only plane, else the dense column of `tc`
def private cm2_tile_tail(fmt : int; tc : int64) : int {
    verify(tc == 256l || tc == 128l || tc == 32l || tc == CM2_TC_E, "vk classes: a cm2 tile column the pick never returns")
    if (g_gpu.coopmat_mode == COOPMAT_MM) return 0
    if (tc == CM2_TC_E) return 1
    if (cm2_s_only(fmt)) {
        cm2_require_s_column(fmt, tc)
        return 4
    }
    return tc == 256l ? 2 : (tc == 128l ? 3 : 4)
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-tile-pick-and-default")]
def cm2_cls_ensure(fmt : int; tc : int64) : bool {   // nolint:STYLE037 — kq_tile_stamp's generated arms, one a (format, tile) stamp
    verify(fmt == int(KqFmt.q8) || kq_block32(fmt) || kq_sb(fmt), "vk classes: a cm2 tile for a format the f16 feed never admits")
    let tail = cm2_tile_tail(fmt, tc)
    var r = false
    kq_tile_stamp(fmt, CM2_TILE_NONE, "batch", tail, CM2_TILE_TAILS) <| $() {
        r = tile_ensure()
    }
    return r
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-tile-pick-and-default")]
def cm2_cls_set(fmt : int; tc : int64; bufs : uint64 const[5]; sizes : int64 const[5]; gbits : uint const[5]) : VkDescriptorSet {   // nolint:STYLE037 — kq_tile_stamp's generated arms, one a (format, tile) stamp
    let tail = cm2_tile_tail(fmt, tc)
    var r : VkDescriptorSet
    kq_tile_stamp(fmt, CM2_TILE_NONE, "batch", tail, CM2_TILE_TAILS) <| $() {
        r = tile_set(bufs, sizes, gbits)
    }
    return r
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-tile-pick-and-default")]
def cm2_cls_enc(fmt : int; tc : int64; raw : VkCommandBuffer; var h : VkHaz; var s : VkDescriptorSet; var pc : BatchArgs; groups : int64) {   // nolint:STYLE037 — kq_tile_stamp's generated arms, one a (format, tile) stamp
    let tail = cm2_tile_tail(fmt, tc)
    kq_tile_stamp(fmt, CM2_TILE_NONE, "batch", tail, CM2_TILE_TAILS) <| $() {
        tile_enc(raw, h, s, pc, groups)
    }
}

//! the ladder's m rows of a bucket of `c` rows: none up to the s column, else whole m columns plus a
//! partial one when the remainder passes the s column (a remainder within it takes the s column)
def sched_ladder_m_rows(c : uint) : uint {
    if (c <= SCHED_S_ROWS) return 0u
    return (c % SCHED_M_ROWS) > SCHED_S_ROWS ? c : (c / SCHED_M_ROWS) * SCHED_M_ROWS
}

//! the window's bucket schedule, a thread an expert: `WG` threads reach `WG` experts (the 512-thread stamp past 256); its scan is `WgScanT`'s
[ |> template_struct_instance]
class template MoeSchedT : WgScanT {
    @ssbo @binding = 0 idx : array<uint>       //! [nk] the picked expert per slot row
    @ssbo @binding = 1 inv : array<uint>       //! [nk] slot row -> bucket row
    @ssbo @binding = 2 sched1 : array<uint>    //! the gate plane's records + maps
    @ssbo @binding = 3 sched3 : array<uint>
    @ssbo @binding = 4 sched2 : array<uint>
    @ssbo @binding = 5 bex : array<uint>       //! [nk] bucket row -> its expert (the biased act's row map)
    @push_constant pa : SchedArgs
    @workgroup cnt : uint[int(WG)]    //! the experts' slot counts, then their bucket cursors

    //! one piece's record and per-wg map on the three planes: expert `tid`'s `rows` from `row0`,
    //! record `ri`, its `rt` column tiles from workgroup `wg_gu` / `wg_dn` of the dispatch whose
    //! maps sit at `map_gu` / `map_dn`
    def write_piece(ri, row0, rows, rt, wg_gu, wg_dn, map_gu, map_dn : uint) {
        let tid = gl_LocalInvocationID.x
        let rb = ri * 4u
        sched1[rb] = pa.base1 + tid * pa.stride1
        sched1[rb + 1u] = row0
        sched1[rb + 2u] = rows
        sched1[rb + 3u] = wg_gu
        sched3[rb] = pa.base3 + tid * pa.stride3
        sched3[rb + 1u] = row0
        sched3[rb + 2u] = rows
        sched3[rb + 3u] = wg_gu
        sched2[rb] = pa.base2 + tid * pa.stride2
        sched2[rb + 1u] = row0
        sched2[rb + 2u] = rows
        sched2[rb + 3u] = wg_dn
        var g = 0u
        while (g < rt * pa.wt_gu) {
            sched1[map_gu + wg_gu + g] = ri
            sched3[map_gu + wg_gu + g] = ri
            g++
        }
        g = 0u
        while (g < rt * pa.wt_dn) {
            sched2[map_dn + wg_dn + g] = ri
            g++
        }
    }

    //! the sentinel tails of the four maps, from the live workgroup counts (the s count in the low
    //! half of each total, the m count in the high) to the dispatch bounds
    def write_tails(total_gu, total_dn : uint) {
        let tid = gl_LocalInvocationID.x
        var t = (total_gu & 0xFFFFu) + tid
        while (t < pa.bound_gu) {
            sched1[pa.map_off + t] = SCHED_NONE
            sched3[pa.map_off + t] = SCHED_NONE
            t += WG
        }
        t = (total_dn & 0xFFFFu) + tid
        while (t < pa.bound_dn) {
            sched2[pa.map_off + t] = SCHED_NONE
            t += WG
        }
        t = (total_gu >> 16u) + tid
        while (t < pa.bound_mgu) {
            sched1[pa.map_off_mgu + t] = SCHED_NONE
            sched3[pa.map_off_mgu + t] = SCHED_NONE
            t += WG
        }
        t = (total_dn >> 16u) + tid
        while (t < pa.bound_mdn) {
            sched2[pa.map_off_mdn + t] = SCHED_NONE
            t += WG
        }
    }

    [arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#vk-prefill-moe-block")]
    def sched_body {
        let tid = gl_LocalInvocationID.x
        let mine = tid < pa.ne
        cnt[tid] = 0u
        barrier()
        var i = tid
        while (i < pa.nk) {   //! the top-k clamps every pick under ne, so the tally stays inside cnt
            let _c = atomicAdd(cnt[idx[i]], 1u)
            i += WG
        }
        barrier()
        let c = mine ? cnt[tid] : 0u
        let st = excl_scan(c)
        let mrows = mine && pa.s_only == 0u ? sched_ladder_m_rows(c) : 0u
        let srows = mine ? c - mrows : 0u
        let ri_s = excl_scan(srows > 0u ? 1u : 0u)
        let ri_m = pa.ne + excl_scan(mrows > 0u ? 1u : 0u)
        let rt_s = (srows + SCHED_S_ROWS - 1u) / SCHED_S_ROWS
        let rt_m = (mrows + SCHED_M_ROWS - 1u) / SCHED_M_ROWS
        //! the packed word each scan carries: the s tile count in the low half, the m count in the high
        let g_gu = excl_scan(rt_s * pa.wt_gu | ((rt_m * pa.wt_gu) << 16u))
        let total_gu = scan[WG - 1u]
        let g_dn = excl_scan(rt_s * pa.wt_dn | ((rt_m * pa.wt_dn) << 16u))
        let total_dn = scan[WG - 1u]
        if (srows > 0u) {
            write_piece(ri_s, st + mrows, srows, rt_s, g_gu & 0xFFFFu, g_dn & 0xFFFFu, pa.map_off, pa.map_off)
        }
        if (mrows > 0u) {
            write_piece(ri_m, st, mrows, rt_m, g_gu >> 16u, g_dn >> 16u, pa.map_off_mgu, pa.map_off_mdn)
        }
        write_tails(total_gu, total_dn)
        barrier()   //! every thread has read its count
        cnt[tid] = st
        barrier()
        i = tid
        while (i < pa.nk) {
            let b = atomicAdd(cnt[idx[i]], 1u)
            inv[i] = b
            bex[b] = idx[i]
            i += WG
        }
    }
}

[vk_dispatch(name = "moe_sched_cls", grid = "one", params = "one : int64")]
class MoeSched : MoeSchedT {
    [spirv_kernel(local_size_x = 256)]
    def run {
        sched_body()
    }
}

[vk_dispatch(name = "moe_sched2_cls", kernel = "run", family = "moe_sched_cls", grid = "one", params = "one : int64")]
class MoeSched2 : MoeSchedT {
    override WG = 512u

    [spirv_kernel(local_size_x = 512)]
    def run {
        sched_body()
    }
}

//! the window's bucket schedule over `ne` experts on the stamp that reaches them (`moe_sched_cls`' one set serves both)
def moe_sched_ensure(ne : int64) : bool => ne > 256l ? ensure_moe_sched2_cls() : ensure_moe_sched_cls()

def moe_sched_enc(ne : int64; raw : VkCommandBuffer; var h : VkHaz; var set_ : VkDescriptorSet; var pc : SchedArgs) {
    if (ne > 256l) {
        enc_moe_sched2_cls(raw, h, set_, pc, 1l)
    } else {
        enc_moe_sched_cls(raw, h, set_, pc, 1l)
    }
}

struct GatherArgs {
    n : uint    // row width
    k : uint    // slots per position (grid entry r sits at position r / k)
    r0 : uint   // this dispatch's bucket-row window [r0, r1)
    r1 : uint
}

//! the routed block's gather: every slot row's activation row into its bucket row, as halves (`XG` float16, the f16-fed tiles'
//! feed) or as floats (the Q8-fed chain requantizes the gathered rows)
[ |> template_struct_instance]
class template MoeGatherT {
    @template_constant F16 : bool = true
    @template_constant BYROW : bool = false    //! a workgroup a bucket row of the window (the per-expert chain), not a grid entry
    @ssbo @binding = 0 x : array<float>        // the normed activation rows, one per position
    @ssbo @binding = 1 @template_gate = "!BYROW" slot_idx : array<uint>  // position -> its k slot rows (the combine's map)
    @ssbo @binding = 1 @template_gate = BYROW bkt_grid : array<uint>     // bucket row -> its grid entry (the map's inverse)
    @ssbo @binding = 2 xg : array<XG>          // the gathered rows, bucket order, window-relative
    @push_constant pa : GatherArgs

    def gather_body {
        static_if (BYROW) {
            let b = gl_WorkGroupID.x
            let src = (bkt_grid[pa.r0 + b] / max(1u, pa.k)) * pa.n
            let dst = b * pa.n
            gather_row(src, dst)
        } else {
            let r = gl_WorkGroupID.x
            let b = slot_idx[r]
            if (b < pa.r0 || b >= pa.r1) {
                return
            }
            let src = (r / pa.k) * pa.n
            let dst = (b - pa.r0) * pa.n
            gather_row(src, dst)
        }
    }

    def gather_row(src, dst : uint) {
        var i = gl_LocalInvocationID.x
        while (i < pa.n) {
            static_if (F16) {
                xg[dst + i] = half_sat(x[src + i])
            } else {
                xg[dst + i] = x[src + i]
            }
            i += 256u
        }
    }
}

[vk_dispatch(name = "moe_gather_f16_cls", grid = "nk", params = "nk : int64")]
class MoeGatherF16 : MoeGatherT {
    typedef XG = float16

    [spirv_kernel(local_size_x = 256, name = "moe_gather_f16_cls_spv"), arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-expert-chain")]
    def run {
        gather_body()
    }
}

//! the per-expert chain's gather: one workgroup a window row, over the rows of one expert alone
[vk_dispatch(name = "moe_gather_f16_row_cls", grid = "rows", params = "rows : int64")]
class MoeGatherF16Row : MoeGatherT {
    typedef XG = float16
    override BYROW = true

    [spirv_kernel(local_size_x = 256, name = "moe_gather_f16_row_cls_spv"), arch(at="../ARCHITECTURE_GPU_VULKAN_STREAM.md#streamed-layer-split")]
    def run {
        gather_body()
    }
}

[vk_dispatch(name = "moe_gather_f32_cls", grid = "nk", params = "nk : int64")]
class MoeGatherF32 : MoeGatherT {
    typedef XG = float
    override F16 = false

    [spirv_kernel(local_size_x = 256, name = "moe_gather_f32_cls_spv"), arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#vk-moe-q8-chain")]
    def run {
        gather_body()
    }
}

//! the wide residual's opening copies on the template both homes stamp (`GkHcInit`)
[vk_dispatch(name = "hc_init_cls", grid = "wgs", params = "wgs : int64")]
class HcInit : GkHcInit {
    [spirv_kernel(local_size_x = 256, name = "hc_init_cls_spv")]
    def run {
        hc_init_body()
    }
}

//! the MTP head's eh_proj input rows on the template both homes stamp (`GkCat2`)
[vk_dispatch(name = "cat2_cls", grid = "wgs", params = "wgs : int64")]
class Cat2 : GkCat2 {
    [spirv_kernel(local_size_x = 256, name = "cat2_cls_spv")]
    def run {
        cat2_body()
    }
}

//! the grouped rms of the wide rows on the template both homes stamp (`GkHcNorm`), the CPU's `1 / sqrt`
[vk_dispatch(name = "hc_norm_cls", grid = "wgs", params = "wgs : int64")]
class HcNorm : GkHcNorm {
    [spirv_kernel(local_size_x = 256, name = "hc_norm_cls_spv")]
    def run {
        hc_norm_body()
    }
}

struct HcLoArgs {
    nblk : uint      //! the low-rank row's 32-blocks
    inv_hc : float   //! 1 / hc: the mixer's low-rank row is silu(lo / hc) before the up projection
}

//! the mixer's low-rank row activated and requantized for the up GEMV: silu(lo / hc) into Q8_0 blocks
[vk_dispatch(name = "hc_lo_rq_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class HcLoRq : RqT {
    @ssbo @binding = 0 src : array<float>
    @ssbo @binding = 1 outq : array<int8>
    @ssbo @binding = 2 outs : array<float>
    @push_constant pa : HcLoArgs

    def override load1(e : uint) : float => silu_f32(src[e] * pa.inv_hc)

    [spirv_kernel(local_size_x = 256)]
    def run {
        rq_body()
    }
}

//! the mixer's tail on the template both homes stamp (`GkHcMix`); the scatter logits read xn before this: they ride
//! `RouterGemv` over the inject rows
[vk_dispatch(name = "hc_mix_cls", grid = "wgs", params = "wgs : int64")]
class HcMix : GkHcMix {
    [spirv_kernel(local_size_x = 256, name = "hc_mix_cls_spv")]
    def run {
        hc_mix_body()
    }
}

//! the scatter into the streams on the template both homes stamp (`GkHcCombine`), its MoE arm on: a MoE layer's output row
//! is the routed sum the host landed in `acc`, plus the shared expert's row at its gate
[vk_dispatch(name = "hc_comb_cls", grid = "wgs", params = "wgs : int64")]
class HcCombine : GkHcCombine {
    override MOE = true

    [spirv_kernel(local_size_x = 256, name = "hc_comb_cls_spv")]
    def run {
        hc_combine_body()
    }
}

//! a window's `pos0` that reads the first position off the token record (`tok[0].pos`): a command recorded once and resubmitted at every position
let PLE_POS_TOK = 0xFFFFFFFFu

//! the n-gram side input's gate on the template both homes stamp (`GkPleGate`): `rows` 0 lands the recorded token's conv input
//! row in the ring's slot of its position (`tok[0].pos % nrows` of the row's region `dnslot`), else a window's rows in the panel
[vk_dispatch(name = "ple_gate_cls", grid = "wgs", params = "wgs : int64")]
class PleGate : GkPleGate {
    @ssbo @binding = 4 hist : array<float>    //! the conv ring: [regions x nrows x hc x dim]
    @ssbo @binding = 6 tok : array<TokMeta>
    @ssbo @binding = 7 panel : array<float>   //! the window's normed gated rows [rows x hc x dim] (the ring itself when `rows` is 0)

    def override cin_base(r, c : uint) : uint {
        if (pa.rows == 0u) {
            return (tok[0].dnslot * pa.nrows + tok[0].pos % max(1u, pa.nrows)) * pa.hc * pa.dim + c * pa.dim
        }
        return (r * pa.hc + c) * pa.dim
    }

    def override cin_store(e : uint; v : float) {
        if (pa.rows == 0u) {
            hist[e] = v
        } else {
            panel[e] = v
        }
    }

    [spirv_kernel(local_size_x = 256, name = "ple_gate_cls_spv")]
    def run {
        ple_gate_body()
    }
}

//! the side input's landing on the template both homes stamp (`GkPleConv`): `rows` 0 is the recorded token's row off `tok`,
//! every tap off the ring; else `rows` rows from `pos0` (`PLE_POS_TOK`: off `tok`), a tap inside the window off the panel
[vk_dispatch(name = "ple_conv_cls", grid = "wgs", params = "wgs : int64")]
class PleConv : GkPleConv {
    @ssbo @binding = 2 hist : array<float>
    @ssbo @binding = 4 tok : array<TokMeta>
    @ssbo @binding = 5 panel : array<float>   //! the window's normed gated rows (the ring itself when `rows` is 0)

    def win_pos0 : uint => pa.pos0 == PLE_POS_TOK ? tok[0].pos : pa.pos0

    def override row_pos(r : uint) : int => int(pa.rows == 0u ? tok[0].pos : win_pos0() + r)

    def override tap(e : uint; src_pos : int; nrows : uint) : float {
        let hcd = pa.hc * pa.dim
        let pos0 = win_pos0()
        if (pa.rows != 0u && src_pos >= int(pos0)) {
            return panel[uint(src_pos - int(pos0)) * hcd + e]
        }
        return hist[tok[0].dnslot * nrows * hcd + uint(src_pos) % nrows * hcd + e]
    }

    [spirv_kernel(local_size_x = 256, name = "ple_conv_cls_spv")]
    def run {
        ple_conv_body()
    }
}

struct PleCommitArgs {
    dim : uint
    hc : uint
    nrows : uint   //! the conv ring's rows
    rows : uint    //! the panel rows committed: row r lands in the ring's slot of position tok[0].pos + r
}

//! the window's normed gated rows into the conv ring, one element a thread: the verify's rows land after the conv read them,
//! at the slot of each row's position off the token record (the ring's slots wrap at `nrows`)
[vk_dispatch(name = "ple_commit_cls", grid = "wgs", params = "wgs : int64")]
class PleCommit {
    @ssbo @binding = 0 @readonly panel : array<float>   //! the window's normed gated rows [rows x hc x dim]
    @ssbo @binding = 1 hist : array<float>              //! the conv ring [regions x nrows x hc x dim]
    @ssbo @binding = 2 @readonly tok : array<TokMeta>
    @push_constant pa : PleCommitArgs

    [spirv_kernel(local_size_x = 256, name = "ple_commit_cls_spv")]
    def run {
        let g = gl_GlobalInvocationID.x
        let hcd = pa.hc * pa.dim
        if (g < pa.rows * hcd) {
            let r = g / hcd
            let e = g - r * hcd
            let slot = tok[0].dnslot * pa.nrows + (tok[0].pos + r) % max(1u, pa.nrows)
            hist[slot * hcd + e] = panel[g]
        }
    }
}

// ===== deltanet: the conv front =====

//! the conv front on the template both homes stamp (`GkDnConv`): the taps and the history out of the layer's smalls
[vk_dispatch(name = "dn_conv_cls", grid = "wgs", params = "wgs : int64")]
class DnConv : GkDnConv {
    @ssbo @binding = 2 @role = "read" smalls : array<float>   //! taps + history + any small planes (push offsets)

    def override seg_row0 : uint => pa.row0
    def override seg_npos : uint => pa.npos
    def override tap_at(ch, j : uint) : float => smalls[pa.taps_off + ch * pa.dconv + j]
    def override hist_at(ch, taps, hidx : uint) : float {
        if (pa.hist_mode == 1u) {
            return smalls[pa.hist_off + ch * taps + hidx]
        }
        return smalls[pa.hist_off + hidx * pa.cd + ch]
    }

    [spirv_kernel(local_size_x = 256, name = "dn_conv_cls_spv"), arch(at="../ARCHITECTURE_GPU_VULKAN_WINDOW.md#vk-prefill-dn-block")]
    def run {
        dn_conv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

struct DnTailArgs {
    cd : uint         // conv width (channels)
    taps : uint       // history rows (dconv - 1)
    row0 : uint       // the first tail row in src (the window's last taps raw rows)
    hist_off : uint   // the ring image's float offset in smalls
    old_rows : uint   //! leading ring rows the call's entry image supplies (a lone window shorter than the taps): new row k is old row k + taps - old_rows
    old_off : uint    //! the entry image's float offset in smalls (per-channel, the decode step's layout); never the image at `hist_off`
}

//! the conv ring write both tails stamp (`DnTail` the window's, `DnCapTail` a capture's): thread `idx` of `cd * taps` takes
//! channel `idx / taps`'s ring row `k = idx % taps` - projected row `row0 + k - old_rows` where `k >= old_rows`, else the entry
//! image's row `k + taps - old_rows` at `old_off` - into the destination image at `dst0`
[ |> template_struct_instance]
class template DnTailT {
    def abstract src_at(i : uint) : float     //! projected-rows float `i`
    def abstract entry_at(i : uint) : float   //! smalls float `i`
    def abstract put(i : uint; v : float) : void

    def tail_tap(idx, cd, taps, row0, old_rows, old_off, dst0 : uint) {
        if (taps != 0u && idx < cd * taps) {
            let ch = idx / taps
            let k = idx % taps
            var v = 0.0
            if (k >= old_rows) {
                v = src_at((row0 + k - old_rows) * cd + ch)
            } else {
                v = entry_at(old_off + ch * taps + k + taps - old_rows)
            }
            put(dst0 + ch * taps + k, v)
        }
    }
}

[vk_dispatch(name = "dn_tail_cls", grid = "wgs", params = "wgs : int64")]
class DnTail : DnTailT {
    @ssbo @binding = 0 src : array<float>      // the projected rows (position-major)
    @ssbo @binding = 1 smalls : array<float>   // the layer's smalls (the ring image at hist_off)
    @push_constant pa : DnTailArgs

    def override src_at(i : uint) : float => src[i]
    def override entry_at(i : uint) : float => smalls[i]
    def override put(i : uint; v : float) : void {
        smalls[i] = v
    }

    [spirv_kernel(local_size_x = 256, name = "dn_tail_cls_spv")]
    def run {
        tail_tap(gl_GlobalInvocationID.x, pa.cd, pa.taps, pa.row0, pa.old_rows, pa.old_off, pa.hist_off)
    }
}

// ===== deltanet: the fused decode step =====

//! the row kernels' reduces in value form over the shared `GkWgReduce` (one wg reduces one row), and the inverse rms
[ |> template_struct_instance]
class template WgReduceBase : GkQ8BlockStore {
    //! `slot` alternates 0 / 1 between consecutive reduces of one kernel
    def wg_rms_inv(ss0 : float; n : uint; eps : float; slot : uint) : float => 1.0 / sqrt(wg_sum(ss0, slot) / float(n) + eps)

    //! the workgroup sum every thread holds alike (the layernorm's mean and variance ride it, and `wg_rms_inv`) - the
    //! shared base's reduce in value form, which the SPIR-V emitter takes
    def wg_sum(v0 : float; slot : uint) : float {
        var tot = 0.0
        wg_sum_into(v0, slot, tot)
        return tot
    }

    //! the workgroup max every thread holds alike (the chunked attention's score max)
    def wg_max(v0 : float; slot : uint) : float {
        var m = 0.0
        wg_max_into(v0, slot, m)
        return m
    }
}

struct DnStepArgs {
    cd : uint
    kd : uint
    nkh : uint
    ds : uint
    dconv : uint
    beta_off : uint
    g_off : uint
    taps_off : uint
    wnorm_off : uint
    a_off : uint
    dt_off : uint
    hist_off : uint
    eps : float
    qscale : float
    out_f32 : uint
    beta_row_stride : uint
    row0 : uint   //! the dispatch's first row: the same-slab verify steps one stream's rows one dispatch a row in one slot, each reading the state the row before it wrote
}

//! the fused deltanet decode step; its stamps differ on the out-gate alone (`ZSIG`)
[ |> template_struct_instance]
class template DnStepFusedT : WgReduceBase {
    @template_constant ZSIG : bool = false   //!< the out-gate is sigmoid(z) (qwen4exp's `Config.dn_z_sigmoid`) instead of silu(z)
    @ssbo @binding = 0 proj : array<float>     // the fresh projection row (+ z rows at cd)
    @ssbo @binding = 1 state : array<float>    // the persistent per-head f32 state (in place)
    @ssbo @binding = 2 smalls : array<float>   // beta/g/taps/wnorm/a/dt + the conv HISTORY ring (rw)
    @ssbo @binding = 3 outs : array<float>     // the head's Q8_0 scales
    @ssbo @binding = 4 outq : array<uint>      // ... and quant words
    @ssbo @binding = 5 tok : array<TokMeta>    // the shared per-token block (element 0)
    @ssbo @binding = 6 outf : array<float>     // the head's o row as f32
    @push_constant pa : DnStepArgs
    @workgroup dq : float[128]
    @workgroup dk : float[128]
    @workgroup dv : float[128]
    @workgroup dvn : float[128]
    @workgroup dout : float[128]
    @workgroup partk : float[256]   //! each row part's k.S column partials (part-major, ds wide: the thread's own slot)
    @workgroup partq : float[256]   //! ... and q.S
    @workgroup sg_sumsq : float[16]   //! per-subgroup partial squared sums (q and k); the out-norm's reduce is `WgReduceBase`'s
    @workgroup sg_qk : float[16]      //! per-subgroup partials of the raw q.k
    @workgroup ds8 : float[8]

    [spirv_kernel(local_size_x = 256), arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#hybrid-token-command")]
    def run {   // nolint:STYLE037,STYLE038 — the fused chain: conv, norms, the delta rule, out-norm and the o row in one dispatch
        let ds = pa.ds
        let nvh = max(1u, (pa.cd - 2u * pa.kd) / ds)   //! the head count the geometry pins: cd = 2 kd + nvh ds
        let row = pa.row0 + gl_WorkGroupID.x / nvh
        let h = gl_WorkGroupID.x % nvh
        let taps = pa.dconv - 1u
        let par = tok[row].parity
        let slot = tok[row].dnslot
        let prow = row * (pa.cd + nvh * ds)
        let ringb = pa.hist_off + slot * 2u * pa.cd * taps
        let rrb = ringb + par * pa.cd * taps
        let wrb = ringb + (1u - par) * pa.cd * taps
        let tid = gl_LocalInvocationID.x
        let kh = h % pa.nkh
        var e = tid
        while (e < 3u * ds) {
            let sl = e / ds
            let j = e % ds
            let ch = (sl == 0u ? kh * ds : (sl == 1u ? pa.kd + kh * ds : 2u * pa.kd + h * ds)) + j
            let fresh = proj[prow + ch]
            var acc = fresh * smalls[pa.taps_off + ch * pa.dconv + taps]
            var k = 0u
            while (k < taps) {
                acc += smalls[rrb + ch * taps + k] * smalls[pa.taps_off + ch * pa.dconv + k]
                k++
            }
            if (sl == 2u || h < pa.nkh) {   // this head owns the channel's ring advance
                k = 0u
                while (k + 1u < taps) {
                    smalls[wrb + ch * taps + k] = smalls[rrb + ch * taps + k + 1u]
                    k++
                }
                smalls[wrb + ch * taps + taps - 1u] = fresh
            }
            let sv = silu_f32(acc)
            if (sl == 0u) {
                dq[j] = sv
            } elif (sl == 1u) {
                dk[j] = sv
            } else {
                dv[j] = sv
            }
            e += 256u
        }
        barrier()
        //! the q and k squared sums and the raw q.k by subgroup adds: the first half of the threads hold q, the second k
        var pn = 0.0
        var pq = 0.0
        if (tid < ds) {
            pn = dq[tid] * dq[tid]
            pq = dq[tid] * dk[tid]
        } elif (tid >= 128u && tid < 128u + ds) {
            pn = dk[tid - 128u] * dk[tid - 128u]
        }
        let sn = subgroupAdd(pn)
        let spq = subgroupAdd(pq)
        if (gl_SubgroupInvocationID == 0u) {
            sg_sumsq[gl_SubgroupID] = sn
            sg_qk[gl_SubgroupID] = spq
        }
        barrier()
        if (tid == 0u) {   //! q.k is summed raw; the q and k scales apply here
            var sumq = 0.0
            var sumk = 0.0
            var qkr = 0.0
            let half = gl_NumSubgroups / 2u
            var sgi = 0u
            while (sgi < gl_NumSubgroups) {
                if (sgi < half) {
                    sumq += sg_sumsq[sgi]
                    qkr += sg_qk[sgi]
                } else {
                    sumk += sg_sumsq[sgi]
                }
                sgi++
            }
            let sq = (1.0 / max(sqrt(sumq), pa.eps)) * pa.qscale
            let sk = 1.0 / max(sqrt(sumk), pa.eps)
            ds8[0u] = sq
            ds8[1u] = sk
            let graw = smalls[pa.g_off + row * nvh + h] + smalls[pa.dt_off + h]
            ds8[2u] = dn_decay(smalls[pa.a_off + h], graw)
            ds8[3u] = sigmoid_f32(smalls[pa.beta_off + row * pa.beta_row_stride + h])
            ds8[4u] = qkr * sq * sk
        }
        barrier()
        let sq = ds8[0u]
        let sk = ds8[1u]
        let eg = ds8[2u]
        let bt = ds8[3u]
        let qk = ds8[4u]
        let rows_per_part = min(ds, 64u)
        let nparts = ds / rows_per_part
        let active = tid < ds * nparts
        let c = tid % ds
        let r0 = (tid / ds) * rows_per_part
        let sbase = slot * nvh * ds * ds + h * ds * ds
        var sreg : float[64]
        var ksp = 0.0
        var qsp = 0.0
        if (active) {
            //! the loads first, straight-line (a short part re-reads its last row), so all 64 issue before a sum waits on one
            for [unroll] (j in range(64)) {
                sreg[j] = state[sbase + (r0 + min(uint(j), rows_per_part - 1u)) * ds + c]
            }
            for [unroll] (j in range(64)) {
                if (uint(j) < rows_per_part) {
                    ksp += dk[r0 + uint(j)] * sreg[j]
                    qsp += dq[r0 + uint(j)] * sreg[j]
                }
            }
        }
        if (active) {
            partk[tid] = ksp
            partq[tid] = qsp
        }
        barrier()
        if (tid < ds) {
            var ks = 0.0
            var qs = 0.0
            var p = 0u
            while (p < nparts) {
                ks += partk[p * ds + tid]
                qs += partq[p * ds + tid]
                p++
            }
            let vn = bt * (dv[tid] - eg * (sk * ks))
            dvn[tid] = vn
            dout[tid] = eg * (sq * qs) + qk * vn
        }
        barrier()
        if (active) {
            let vnc = dvn[c]
            for [unroll] (j in range(64)) {
                if (uint(j) < rows_per_part) {
                    state[sbase + (r0 + uint(j)) * ds + c] = sreg[j] * eg + (sk * dk[r0 + uint(j)]) * vnc
                }
            }
        }
        var po = 0.0
        if (tid < ds) {
            po = dout[tid] * dout[tid]
        }
        let oinv = wg_rms_inv(po, ds, pa.eps, 0u)
        e = tid
        while (e < ds) {
            let zv = proj[prow + pa.cd + h * ds + e]
            let xn = dout[e] * oinv * smalls[pa.wnorm_off + e]
            dout[e] = xn * dn_zgate(zv, ZSIG)
            e += 256u
        }
        barrier()
        let orow = (row * nvh + h) * ds
        if (pa.out_f32 != 0u) {
            e = tid
            while (e < ds) {
                outf[orow + e] = dout[e]
                e += 256u
            }
        } elif (tid < ds / 32u) {   // the head's Q8_0 blocks (ds % 32 asserted host-side)
            let gb = orow / 32u + tid
            let jb = tid * 32u
            var amax = 0.0
            for (i in range(32)) {
                amax = max(amax, abs(dout[jb + uint(i)]))
            }
            let d = amax / 127.0
            let id = d != 0.0 ? 1.0 / d : 0.0
            outs[gb] = d
            for (wi in range(8)) {
                let wj = jb + uint(wi) * 4u
                var word = 0u
                for (j in range(4)) {
                    let q = int(round(dout[wj + uint(j)] * id))
                    word = word | ((uint(q) & 0xFF) << (uint(j) * 8u))
                }
                outq[gb * 8u + uint(wi)] = word
            }
        }
    }
}

[vk_dispatch(name = "dn_step_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class DnStepFused : DnStepFusedT {}

[vk_dispatch(name = "dn_step_sig_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class DnStepFusedSig : DnStepFusedT {
    override ZSIG = true
}

// ===== deltanet: the sequential scan + the gated out-norm =====

struct DnScanArgs {
    npos : uint
    cd : uint
    kd : uint
    nvh : uint
    nkh : uint
    ds : uint
    beta_off : uint   // raw beta per (pos, head) in smalls; sigmoid in kernel
    g_off : uint      // raw g per (pos, head); the decay is exp(a * softplus(g + dt))
    a_off : uint
    dt_off : uint
    o_off : uint      // the raw o rows' base in orows (position-major, di wide)
    di : uint
    region_state_base : uint
    row0 : uint       //! the dispatch's first row in the conv rows, the beta / g rows and the o rows: a segmented window scans a segment's rows over its own region's state
    cap_n : uint      //! the part's captures: records of two words (the row the state stands after, the checkpoint row's float base) at `cap_rec` of `caps`
    cap_rec : uint
    cap_state_off : uint   //! the layer's state floats into a checkpoint row (`RLayer.snap_off / 4`)
}

[vk_dispatch(name = "dn_scan_cls", grid = "wgs", params = "wgs : int64")]
class DnScan {
    @ssbo @binding = 0 @readonly conv : array<float>     //! the conv output rows (position-major: q | k | v)
    @ssbo @binding = 1 state : array<float>              // the persistent per-head f32 state [nvh][ds][ds]
    @ssbo @binding = 2 @readonly smalls : array<float>   // raw beta/g rows, a/dt per head (push offsets)
    @ssbo @binding = 3 orows : array<float>              // the raw o rows at o_off
    @ssbo @binding = 4 @readonly caps : array<uint>      //! the window's capture records
    @ssbo @binding = 5 capst : array<float>              //! the checkpoint rows (`snap_dev`): a capture's state lands at its row's base plus the layer's
    @push_constant pa : DnScanArgs
    @workgroup egs : float[int(DN_WINDOW)]   //! the head's decay per token, exp(a * softplus(g + dt)), staged once per workgroup
    @workgroup bts : float[int(DN_WINDOW)]   //! the head's sigmoid(beta) per token, the same way

    def cluster_add(v : float; lanes_per_col : uint) : float {
        var r = v
        if (lanes_per_col > 4u) {
            r += subgroupShuffleXor(r, 4u)
        }
        if (lanes_per_col > 2u) {
            r += subgroupShuffleXor(r, 2u)
        }
        if (lanes_per_col > 1u) {
            r += subgroupShuffleXor(r, 1u)
        }
        return r
    }

    [spirv_kernel(local_size_x = 128, name = "dn_scan_cls_spv"), arch(at="../ARCHITECTURE_GPU_VULKAN_WINDOW.md#vk-prefill-dn-block")]
    def run {   // nolint:STYLE037,STYLE038 — the per-token recurrence over 16 register rows of one column
        let ds = pa.ds
        let lanes_per_col = ds / 16u           // 16 state rows each; ds % 16 == 0 host-side
        let cols_per_block = 32u / lanes_per_col   //! the 128-thread workgroup's four 32-lane blocks, by the invocation id: a cluster's shuffles (masks under 8) stay inside a block on any subgroup width
        let cols_per_group = 4u * cols_per_block
        let cgroups = (ds + cols_per_group - 1u) / cols_per_group
        let h = gl_WorkGroupID.x / cgroups
        let sl = gl_LocalInvocationID.x & 31u
        let lane = sl % lanes_per_col
        let col = (gl_WorkGroupID.x % cgroups) * cols_per_group + (gl_LocalInvocationID.x >> 5u) * cols_per_block + sl / lanes_per_col
        let live = col < ds                    // a whole cluster shares col, so its shuffles stay uniform
        let kh = h % pa.nkh
        let cbase = pa.row0 * pa.cd
        let q_elem0 = cbase + kh * ds + lane
        let k_elem0 = cbase + pa.kd + kh * ds + lane
        let voff = cbase + 2u * pa.kd + h * ds + (live ? col : 0u)   // an idle lane reads column 0: past the last head its own column is past the plane
        let aa = smalls[pa.a_off + h]
        let dtb = smalls[pa.dt_off + h]
        var ti = gl_LocalInvocationID.x
        while (ti < pa.npos) {
            let graw = smalls[pa.g_off + (pa.row0 + ti) * pa.nvh + h] + dtb
            egs[ti] = dn_decay(aa, graw)
            bts[ti] = sigmoid_f32(smalls[pa.beta_off + (pa.row0 + ti) * pa.nvh + h])
            ti += 128u
        }
        barrier()
        let sb = pa.region_state_base + h * ds * ds + (live ? col : 0u) + lane * ds
        let row_stride = lanes_per_col * ds
        var s : float[16]
        for [unroll_full] (i in range(16)) {
            s[i] = state[sb + uint(i) * row_stride]
        }
        //! the last token re-reads its own row
        var k_next, q_next : float[16]
        var v_next = conv[voff]
        for [unroll_full] (i in range(16)) {
            k_next[i] = conv[k_elem0 + uint(i) * lanes_per_col]
        }
        for [unroll_full] (i in range(16)) {
            q_next[i] = conv[q_elem0 + uint(i) * lanes_per_col]
        }
        var t = 0u
        while (t < pa.npos) {
            let eg = egs[t]
            let bt = bts[t]
            let v = v_next
            var k, q : float[16]
            for [unroll_full] (i in range(16)) {
                k[i] = k_next[i]
            }
            for [unroll_full] (i in range(16)) {
                q[i] = q_next[i]
            }
            let crow_next = min(t + 1u, pa.npos - 1u) * pa.cd
            v_next = conv[crow_next + voff]
            let kbn = crow_next + k_elem0
            for [unroll_full] (i in range(16)) {
                k_next[i] = conv[kbn + uint(i) * lanes_per_col]
            }
            let qbn = crow_next + q_elem0
            for [unroll_full] (i in range(16)) {
                q_next[i] = conv[qbn + uint(i) * lanes_per_col]
            }
            var kv = k[0] * s[0]
            for [unroll_full] (i in range(1, 16)) {
                kv += k[i] * s[i]
            }
            kv = cluster_add(kv, lanes_per_col)
            let delta = bt * (v - eg * kv)
            for [unroll_full] (i in range(16)) {
                s[i] = s[i] * eg + k[i] * delta
            }
            var o = q[0] * s[0]
            for [unroll_full] (i in range(1, 16)) {
                o += q[i] * s[i]
            }
            o = cluster_add(o, lanes_per_col)
            if (live && lane == 0u) {
                orows[pa.o_off + (pa.row0 + t) * pa.di + h * ds + col] = o
            }
            var ci = 0u
            while (ci < pa.cap_n) {
                let rec = pa.cap_rec + ci * 2u
                if (caps[rec] == t + 1u && live) {
                    let cb = caps[rec + 1u] + pa.cap_state_off + h * ds * ds + col + lane * ds
                    for [unroll_full] (i in range(16)) {
                        capst[cb + uint(i) * row_stride] = s[i]
                    }
                }
                ci++
            }
            t++
        }
        if (live) {
            for [unroll_full] (i in range(16)) {
                state[sb + uint(i) * row_stride] = s[i]
            }
        }
    }
}

struct DnCapTailArgs {
    cd : uint
    taps : uint
    row0 : uint       //! the part's first row in the projected rows
    entry_off : uint  //! the ring image the part entered on (float offset in smalls): the rows before the part, for a capture under the taps
    cap_rec : uint    //! the part's records (two words a capture: the row the state stands after, the checkpoint row's float base)
    cap_hist_off : uint   //! the layer's ring image 0 floats into a checkpoint row ((`RLayer.snap_off` + the slot's state bytes) / 4)
    wg_per_cap : uint     //! the workgroups one capture's elements take: the dispatch is that many a capture, back to back
}

//! a capture's conv history into its checkpoint row's ring image 0 (the row's parity reads image 0): the taps projected rows before
//! the capture's row, the rows before the part off the image it entered on
[vk_dispatch(name = "dn_cap_tail_cls", grid = "wgs", params = "wgs : int64")]
class DnCapTail : DnTailT {
    @ssbo @binding = 0 @readonly src : array<float>      //! the projected rows (position-major)
    @ssbo @binding = 1 @readonly smalls : array<float>   //! the layer's smalls (the entry ring image at entry_off)
    @ssbo @binding = 2 @readonly caps : array<uint>      //! the window's capture records
    @ssbo @binding = 3 dst : array<float>                //! the checkpoint rows
    @push_constant pa : DnCapTailArgs

    def override src_at(i : uint) : float => src[i]
    def override entry_at(i : uint) : float => smalls[i]
    def override put(i : uint; v : float) : void {
        dst[i] = v
    }

    //! a capture at part row `crow` is a window tail ending there: the rows under `crow` come off the entry image
    [spirv_kernel(local_size_x = 256, name = "dn_cap_tail_cls_spv")]
    def run {
        let wpc = max(1u, pa.wg_per_cap)
        let cap = gl_WorkGroupID.x / wpc
        let rec = pa.cap_rec + cap * 2u
        let crow = caps[rec]
        let lead = min(crow, pa.taps)
        tail_tap((gl_WorkGroupID.x % wpc) * 256u + gl_LocalInvocationID.x, pa.cd, pa.taps, pa.row0 + crow - lead, pa.taps - lead,
            pa.entry_off, caps[rec + 1u] + pa.cap_hist_off)
    }
}

//! the gated out-norm over the scan's raw o rows on the template both homes stamp (`GkDnGate`), one workgroup a row, its
//! subgroups walking the value heads
[ |> template_struct_instance]
class template DnScanP3T : GkDnGate {
    [spirv_kernel(local_size_x = 256)]
    def run {
        var h = gl_SubgroupID
        while (h < pa.nvh) {
            dn_gate_head(h, gl_WorkGroupID.x)
            h += gl_NumSubgroups
        }
    }
}

[vk_dispatch(name = "dn_scan_p3_cls", kernel = "run", grid = "npos", params = "npos : int64")]
class DnScanP3 : DnScanP3T {}

[vk_dispatch(name = "dn_scan_p3_sig_cls", kernel = "run", grid = "npos", params = "npos : int64")]
class DnScanP3Sig : DnScanP3T {
    override ZSIG = true
}

// ===== prefill attention: the prep pass =====

let AT_FLAG_GATED_Q = 1u   //! `AtPrepArgs.flags`: the q rows interleave with their gate rows
let AT_FLAG_QK_NORM = 2u   //! `AtPrepArgs.flags`: the head RMSNorm runs

struct AtPrepArgs {
    d : uint         //! this pass's row width (qd for q, kvd for k/v)
    nh : uint        //! heads in this pass
    hs : uint
    half : uint      //! partial-rope half (0 = no rope; the v pass runs here)
    flags : uint     //! AT_FLAG_* bits
    rms_off : uint   //! this pass's norm-weight row in smalls
    cos_off : uint   //! per-position cos rows in smalls
    sin_off : uint
    obase : uint     //! output panel's row base (0 for q, npos for k)
    is_q_pass : uint //! 1 on the q pass, where the gating applies
    boff : uint      //! this pass's row base in the [q | k | v] bias rows
    hasb : uint
    eps : float
}

//! deinterleave, head RMSNorm, NEOX rope; a dispatch a pass (q, k/v)
[ |> template_struct_instance]
class template AtPrepT {
    @ssbo @binding = 0 src : array<float>      //! the projection rows
    @ssbo @binding = 1 outp : array<float>     //! the prepped panel rows
    @ssbo @binding = 2 gates : array<float>    //! the gate stash (gated-q models; written only then)
    @ssbo @binding = 3 smalls : array<float>   //! cos/sin rows + norm weights (push offsets)
    @ssbo @binding = 4 bias : array<float>     //! per-layer [q | k | v] projection biases (hasb), else a placeholder
    @push_constant pa : AtPrepArgs
    @workgroup row : float[8192]               //! one position's projection row (d <= 8192)
    @workgroup hsinv : float[64]               //! per-head inverse rms (heads <= 64)

    [spirv_inline]
    def at_prep_body {
        let p = gl_WorkGroupID.x
        let hs = pa.hs
        let half = pa.half
        let gated = pa.is_q_pass != 0u ? (pa.flags & AT_FLAG_GATED_Q) : 0u
        let qknorm = pa.flags & AT_FLAG_QK_NORM
        let tid = gl_LocalInvocationID.x
        var i = tid
        while (i < pa.d) {
            let h = i / hs
            let j = i % hs
            if (gated != 0u) {
                row[i] = src[p * 2u * pa.d + h * 2u * hs + j] + (pa.hasb != 0u ? bias[pa.boff + i] : 0.0)
                gates[p * pa.d + i] = src[p * 2u * pa.d + h * 2u * hs + hs + j]
            } else {
                row[i] = src[p * pa.d + i] + (pa.hasb != 0u ? bias[pa.boff + i] : 0.0)
            }
            i += 256u
        }
        barrier()
        if (qknorm != 0u && tid < pa.nh) {
            var ss = 0.0
            var j = 0u
            while (j < hs) {
                let v = row[tid * hs + j]
                ss += v * v
                j++
            }
            hsinv[tid] = 1.0 / sqrt(ss / float(hs) + pa.eps)
        }
        barrier()
        i = tid
        while (i < pa.d) {
            let h = i / hs
            let j = i % hs
            var v = row[i]
            if (qknorm != 0u) {
                v = smalls[pa.rms_off + j] * (v * hsinv[h])
            }
            var outv = v
            if (j < half) {
                var v1 = row[h * hs + j + half]
                if (qknorm != 0u) {
                    v1 = smalls[pa.rms_off + j + half] * (v1 * hsinv[h])
                }
                outv = v * smalls[pa.cos_off + p * half + j] - v1 * smalls[pa.sin_off + p * half + j]
            } elif (j < 2u * half) {
                let jj = j - half
                var v0 = row[h * hs + jj]
                if (qknorm != 0u) {
                    v0 = smalls[pa.rms_off + jj] * (v0 * hsinv[h])
                }
                outv = v0 * smalls[pa.sin_off + p * half + jj] + v * smalls[pa.cos_off + p * half + jj]
            }
            outp[(pa.obase + p) * pa.d + i] = outv
            i += 256u
        }
    }
}


[vk_dispatch(name = "at_prep_cls", grid = "npos", params = "npos : int64")]
class AtPrep : AtPrepT {
    [spirv_kernel(local_size_x = 256, name = "at_prep_cls_spv")]
    def run {
        at_prep_body()
    }
}


//! prefill attention Q tile: a workgroup a (head, 8-query tile)
[vk_dispatch(name = "at_attn_cls", grid = "wgs", params = "wgs : int64")]
class AtAttn : AtAttnTileT {
    typedef KT = float
    @ssbo @binding = 3 outp : array<float>   //! attention out (gated models read the gate stash here)
    @push_constant pa : AtAttnArgs

    [spirv_kernel(local_size_x = 256, name = "at_attn_cls_spv")]
    def run {
        let qtiles = (pa.rows + (TQ - 1u)) / TQ
        let h = gl_WorkGroupID.x / qtiles
        let q0 = (gl_WorkGroupID.x % qtiles) * TQ
        let tid = gl_LocalInvocationID.x
        let r = tid / KS
        let c = tid % KS
        let kvh = h / pa.kv_mul
        load_q(q0, pa.rows, pa.qd, h, pa.hs, pa.hs)
        var o0 = 0.0
        var o1 = 0.0
        var o2 = 0.0
        var o3 = 0.0
        var o4 = 0.0
        var o5 = 0.0
        var o6 = 0.0
        var o7 = 0.0
        barrier()
        let gq = pa.w0 + q0 + r
        let kend = pa.w0 + min(q0 + TQ, pa.rows)   // causal bound (exclusive) of the tile's last row
        var k0 = 0u
        while (k0 < kend) {
            load_k(k0, kend, 0u, pa.kvd, kvh, pa.hs, 0u)
            barrier()
            qk_score(r, c, pa.hs, k0, gq, q0, pa.rows, 0u, pa.scale, 0.0)   //! the per-op tile serves no softcap model
            barrier()
            softmax_update()
            barrier()
            let av = fal[r]
            o0 *= av
            o1 *= av
            o2 *= av
            o3 *= av
            if (pa.dpl > 4u) {
                o4 *= av
                o5 *= av
                o6 *= av
                o7 *= av
            }
            load_v(k0, kend, 0u, pa.kvd, kvh, pa.hs, 0u)
            barrier()
            var c2 = 0u
            while (c2 < KS) {
                let pw = score_tile[r * KS + c2]
                let vb = c2 * LD + c
                o0 += pw * kv_stage[vb]
                o1 += pw * kv_stage[vb + 32u]
                o2 += pw * kv_stage[vb + 64u]
                o3 += pw * kv_stage[vb + 96u]
                if (pa.dpl > 4u) {
                    o4 += pw * kv_stage[vb + 128u]
                    o5 += pw * kv_stage[vb + 160u]
                    o6 += pw * kv_stage[vb + 192u]
                    o7 += pw * kv_stage[vb + 224u]
                }
                c2++
            }
            barrier()
            k0 += KS
        }
        if (q0 + r < pa.rows) {
            let inv = 1.0 / fl[r]
            let ob = (q0 + r) * pa.qd + h * pa.hs + c
            let gated = pa.flags & AT_ATTN_FLAG_GATED
            var jj = 0u
            while (jj < pa.dpl) {
                var v = (jj == 0u ? o0 : (jj == 1u ? o1 : (jj == 2u ? o2 : (jj == 3u ? o3
                    : (jj == 4u ? o4 : (jj == 5u ? o5 : (jj == 6u ? o6 : o7))))))) * inv
                if (gated != 0u) {
                    v = sig_gate(v, outp[ob + jj * 32u])
                }
                outp[ob + jj * 32u] = v
                jj++
            }
        }
    }
}

// ===== cm2 flash attention (Br=Bc=64, online softmax on
// whole-workgroup tiles). One wg per (head, 64-query tile); Q loads as an f32 accumulator and
// use-converts to f16 A; K^T loads through the transpose view from the f16 K shadow; the causal
// mask computes on the fly in a per-element callback (no mask buffer); softmax state, the P
// conversion, and the O rescale ride the reduce/per-element/componentwise rails. =====


[vk_dispatch(name = "fa_cm2_h64_cls", kernel = "run", family = "fa_cm2_cls", grid = "wgs", params = "wgs : int64")]
class FaCm2H64 : FaH64T {
    typedef OT = float

    [spirv_kernel(local_size_x = 128)]
    def run {
        fa_body()
    }
}

[vk_dispatch(name = "fa_cm2_h64_f16_cls", kernel = "run", family = "fa_cm2_cls", grid = "wgs", params = "wgs : int64")]
class FaCm2H64F16 : FaH64T {
    typedef OT = float16
    override OUT16 = true

    [spirv_kernel(local_size_x = 128)]
    def run {
        fa_body()
    }
}

//! the bidirectional twin (the vision towers' encoder attention: gemma4v at its native head width)
[vk_dispatch(name = "fa_cm2_h64_bidir_cls", kernel = "run", family = "fa_cm2_cls", grid = "wgs", params = "wgs : int64")]
class FaCm2H64Bidir : FaH64T {
    typedef OT = float
    override CAUSAL = false

    [spirv_kernel(local_size_x = 128)]
    def run {
        fa_body()
    }
}

[vk_dispatch(name = "fa_cm2_h64_sink_cls", kernel = "run", family = "fa_cm2_sink_cls", grid = "wgs", params = "wgs : int64")]
class FaCm2H64Sink : FaH64T {
    typedef OT = float
    override SINK = true

    [spirv_kernel(local_size_x = 128)]
    def run {
        fa_body()
    }
}

[vk_dispatch(name = "fa_cm2_h64_sink_f16_cls", kernel = "run", family = "fa_cm2_sink_cls", grid = "wgs", params = "wgs : int64")]
class FaCm2H64SinkF16 : FaH64T {
    typedef OT = float16
    override OUT16 = true
    override SINK = true

    [spirv_kernel(local_size_x = 128)]
    def run {
        fa_body()
    }
}


[vk_dispatch(name = "fa_cm2_h128_cls", kernel = "run", family = "fa_cm2_cls", grid = "wgs", params = "wgs : int64")]
class FaCm2H128 : FaH128T {
    typedef OT = float

    [spirv_kernel(local_size_x = 128)]
    def run {
        fa_body()
    }
}

[vk_dispatch(name = "fa_cm2_h128_f16_cls", kernel = "run", family = "fa_cm2_cls", grid = "wgs", params = "wgs : int64")]
class FaCm2H128F16 : FaH128T {
    typedef OT = float16
    override OUT16 = true

    [spirv_kernel(local_size_x = 128)]
    def run {
        fa_body()
    }
}

//! the flash tile at head 96 (`FaH96T`): phi-3's heads, the cm2 arm alone
[vk_dispatch(name = "fa_cm2_h96_cls", kernel = "run", family = "fa_cm2_cls", grid = "wgs", params = "wgs : int64")]
class FaCm2H96 : FaH96T {
    typedef OT = float

    [spirv_kernel(local_size_x = 128)]
    def run {
        fa_body()
    }
}

[vk_dispatch(name = "fa_cm2_h96_f16_cls", kernel = "run", family = "fa_cm2_cls", grid = "wgs", params = "wgs : int64")]
class FaCm2H96F16 : FaH96T {
    typedef OT = float16
    override OUT16 = true

    [spirv_kernel(local_size_x = 128)]
    def run {
        fa_body()
    }
}

//! the bidirectional twin: the padded route of the 72- and 80-wide tower heads (gemma3v, qwen3v, qwen25v), zero pads exact
[vk_dispatch(name = "fa_cm2_h128_bidir_cls", kernel = "run", family = "fa_cm2_cls", grid = "wgs", params = "wgs : int64")]
class FaCm2H128Bidir : FaH128T {
    typedef OT = float
    override CAUSAL = false

    [spirv_kernel(local_size_x = 128)]
    def run {
        fa_body()
    }
}


[vk_dispatch(name = "fa_cm2_h256_cls", kernel = "run", family = "fa_cm2_cls", grid = "wgs", params = "wgs : int64")]
class FaCm2H256 : FaH256T {
    typedef OT = float

    [spirv_kernel(local_size_x = 128)]
    def run {
        fa_body()
    }
}

[vk_dispatch(name = "fa_cm2_h256_f16_cls", kernel = "run", family = "fa_cm2_cls", grid = "wgs", params = "wgs : int64")]
class FaCm2H256F16 : FaH256T {
    typedef OT = float16
    override OUT16 = true

    [spirv_kernel(local_size_x = 128)]
    def run {
        fa_body()
    }
}

[vk_dispatch(name = "fa_cm2_h256_g_cls", kernel = "run", family = "fa_cm2_cls", grid = "wgs", params = "wgs : int64")]
class FaCm2H256G : FaH256T {
    typedef OT = float
    override GATED = true

    [spirv_kernel(local_size_x = 128)]
    def run {
        fa_body()
    }
}

[vk_dispatch(name = "fa_cm2_h256_g_f16_cls", kernel = "run", family = "fa_cm2_cls", grid = "wgs", params = "wgs : int64")]
class FaCm2H256GF16 : FaH256T {
    typedef OT = float16
    override OUT16 = true
    override GATED = true

    [spirv_kernel(local_size_x = 128)]
    def run {
        fa_body()
    }
}

//! the softcap leaves (gemma-2's 256-wide heads): the h256 pass with every scaled score capped before the mask
[vk_dispatch(name = "fa_cm2_h256_cap_cls", kernel = "run", family = "fa_cm2_cls", grid = "wgs", params = "wgs : int64")]
class FaCm2H256Cap : FaH256T {
    typedef OT = float
    override CAP = true

    [spirv_kernel(local_size_x = 128)]
    def run {
        fa_body()
    }
}

[vk_dispatch(name = "fa_cm2_h256_cap_f16_cls", kernel = "run", family = "fa_cm2_cls", grid = "wgs", params = "wgs : int64")]
class FaCm2H256CapF16 : FaH256T {
    typedef OT = float16
    override OUT16 = true
    override CAP = true

    [spirv_kernel(local_size_x = 128)]
    def run {
        fa_body()
    }
}


[vk_dispatch(name = "fa_cm2_h512_cls", kernel = "run", family = "fa_cm2_cls", grid = "wgs", params = "wgs : int64")]
class FaCm2H512 : FaH512T {
    typedef OT = float

    [spirv_kernel(local_size_x = 128)]
    def run {
        fa_body()
    }
}

[vk_dispatch(name = "fa_cm2_h512_f16_cls", kernel = "run", family = "fa_cm2_cls", grid = "wgs", params = "wgs : int64")]
class FaCm2H512F16 : FaH512T {
    typedef OT = float16
    override OUT16 = true

    [spirv_kernel(local_size_x = 128)]
    def run {
        fa_body()
    }
}

[vk_dispatch(name = "fa_khr_h64_cls", kernel = "run", family = "fa_khr_cls", grid = "wgs", params = "wgs : int64")]
class FaKhrH64 : FaH64T {
    override KHR = true
    typedef OT = float

    [spirv_kernel(local_size_x = 128)]
    def run {
        fa_body()
    }
}

[vk_dispatch(name = "fa_khr_h64_bidir_cls", kernel = "run", family = "fa_khr_cls", grid = "wgs", params = "wgs : int64")]
class FaKhrH64Bidir : FaH64T {
    override KHR = true
    override CAUSAL = false
    typedef OT = float

    [spirv_kernel(local_size_x = 128)]
    def run {
        fa_body()
    }
}

[vk_dispatch(name = "fa_khr_h64_f16_cls", kernel = "run", family = "fa_khr_cls", grid = "wgs", params = "wgs : int64")]
class FaKhrH64F16 : FaH64T {
    override KHR = true
    typedef OT = float16
    override OUT16 = true

    [spirv_kernel(local_size_x = 128)]
    def run {
        fa_body()
    }
}

[vk_dispatch(name = "fa_khr_h64_sink_cls", kernel = "run", family = "fa_khr_sink_cls", grid = "wgs", params = "wgs : int64")]
class FaKhrH64Sink : FaH64T {
    override KHR = true
    typedef OT = float
    override SINK = true

    [spirv_kernel(local_size_x = 128)]
    def run {
        fa_body()
    }
}

[vk_dispatch(name = "fa_khr_h64_sink_f16_cls", kernel = "run", family = "fa_khr_sink_cls", grid = "wgs", params = "wgs : int64")]
class FaKhrH64SinkF16 : FaH64T {
    override KHR = true
    typedef OT = float16
    override OUT16 = true
    override SINK = true

    [spirv_kernel(local_size_x = 128)]
    def run {
        fa_body()
    }
}

[vk_dispatch(name = "fa_khr_h128_cls", kernel = "run", family = "fa_khr_cls", grid = "wgs", params = "wgs : int64")]
class FaKhrH128 : FaH128T {
    override KHR = true
    typedef OT = float

    [spirv_kernel(local_size_x = 128)]
    def run {
        fa_body()
    }
}

[vk_dispatch(name = "fa_khr_h128_bidir_cls", kernel = "run", family = "fa_khr_cls", grid = "wgs", params = "wgs : int64")]
class FaKhrH128Bidir : FaH128T {
    override KHR = true
    override CAUSAL = false
    typedef OT = float

    [spirv_kernel(local_size_x = 128)]
    def run {
        fa_body()
    }
}

[vk_dispatch(name = "fa_khr_h128_f16_cls", kernel = "run", family = "fa_khr_cls", grid = "wgs", params = "wgs : int64")]
class FaKhrH128F16 : FaH128T {
    override KHR = true
    typedef OT = float16
    override OUT16 = true

    [spirv_kernel(local_size_x = 128)]
    def run {
        fa_body()
    }
}

[vk_dispatch(name = "fa_khr_h256_cls", kernel = "run", family = "fa_khr_cls", grid = "wgs", params = "wgs : int64")]
class FaKhrH256 : FaH256T {
    override KHR = true
    typedef OT = float

    [spirv_kernel(local_size_x = 128)]
    def run {
        fa_body()
    }
}

[vk_dispatch(name = "fa_khr_h256_f16_cls", kernel = "run", family = "fa_khr_cls", grid = "wgs", params = "wgs : int64")]
class FaKhrH256F16 : FaH256T {
    override KHR = true
    typedef OT = float16
    override OUT16 = true

    [spirv_kernel(local_size_x = 128)]
    def run {
        fa_body()
    }
}

[vk_dispatch(name = "fa_khr_h256_cap_cls", kernel = "run", family = "fa_khr_cls", grid = "wgs", params = "wgs : int64")]
class FaKhrH256Cap : FaH256T {
    override KHR = true
    typedef OT = float
    override CAP = true

    [spirv_kernel(local_size_x = 128)]
    def run {
        fa_body()
    }
}

[vk_dispatch(name = "fa_khr_h256_cap_f16_cls", kernel = "run", family = "fa_khr_cls", grid = "wgs", params = "wgs : int64")]
class FaKhrH256CapF16 : FaH256T {
    override KHR = true
    typedef OT = float16
    override OUT16 = true
    override CAP = true

    [spirv_kernel(local_size_x = 128)]
    def run {
        fa_body()
    }
}

[vk_dispatch(name = "fa_khr_h512_cls", kernel = "run", family = "fa_khr_cls", grid = "wgs", params = "wgs : int64")]
class FaKhrH512 : FaH512T {
    override KHR = true
    typedef OT = float

    [spirv_kernel(local_size_x = 128)]
    def run {
        fa_body()
    }
}

[vk_dispatch(name = "fa_khr_h512_f16_cls", kernel = "run", family = "fa_khr_cls", grid = "wgs", params = "wgs : int64")]
class FaKhrH512F16 : FaH512T {
    override KHR = true
    typedef OT = float16
    override OUT16 = true

    [spirv_kernel(local_size_x = 128)]
    def run {
        fa_body()
    }
}

//! the refusals are `panic`s, not `verify`s: a `verify` prints a stack walk before it throws, and the refusal cell probes them under `recover`
def private fa_stamp_refuse(khr : bool; hs : int64; gated, cap, sink, f16, bidir : bool) {
    if (bidir && (gated || cap || sink || f16)) {
        panic("vk flash: the bidirectional stamps take no gate, cap, sink or f16 output")
    }
    if (bidir && hs != 64l && hs != 128l) {
        panic("vk flash: the bidirectional stamps serve head sizes 64 and 128 alone")
    }
    if (khr && gated) {
        panic("vk flash: the KHR flash tile has no gated stamp")
    }
    if (khr && hs == 96l) {
        panic("vk flash: the KHR flash tile has no head-96 stamp (its lane map walks the head in 64-wide halves)")
    }
    if (cap && hs != 256l) {
        panic("vk flash: the softcap stamps serve head size 256 alone")
    }
    if (gated && hs != 256l) {
        panic("vk flash: the gated stamps serve head size 256 alone")
    }
    if (sink && hs != 64l) {   //! the gated and capped stamps sit at 256 alone (above), so this one test also keeps the sinks off them
        panic("vk flash: the sink stamps serve head size 64 alone")
    }
    if (hs != 64l && hs != 96l && hs != 128l && hs != 256l && hs != 512l) {
        panic("vk flash: a stamp for a head size the tile never serves")
    }
}

//! every flash stamp's stem, four a form - cm2 f32, KHR f32, cm2 f16, KHR f16 - over the forms h64, h128, h256, h512, gated h256,
//! capped h256, the h64 sink and the h64 and h128 bidirectional twins; `-` where a form has no such stamp
let private FA_STAMPS = "fa_cm2_h64 fa_khr_h64 fa_cm2_h64_f16 fa_khr_h64_f16 fa_cm2_h128 fa_khr_h128 fa_cm2_h128_f16 fa_khr_h128_f16 fa_cm2_h256 fa_khr_h256 fa_cm2_h256_f16 fa_khr_h256_f16 fa_cm2_h512 fa_khr_h512 fa_cm2_h512_f16 fa_khr_h512_f16 fa_cm2_h256_g - fa_cm2_h256_g_f16 - fa_cm2_h256_cap fa_khr_h256_cap fa_cm2_h256_cap_f16 fa_khr_h256_cap_f16 fa_cm2_h64_sink fa_khr_h64_sink fa_cm2_h64_sink_f16 fa_khr_h64_sink_f16 fa_cm2_h64_bidir fa_khr_h64_bidir - - fa_cm2_h128_bidir fa_khr_h128_bidir - - fa_cm2_h96 - fa_cm2_h96_f16 -"

//! the flash stamp's ordinal in FA_STAMPS, once the refusals passed
def private fa_stamp_key(khr : bool; hs : int64; gated, cap, sink, f16, bidir : bool) : int {
    fa_stamp_refuse(khr, hs, gated, cap, sink, f16, bidir)
    let form = bidir ? (hs == 64l ? 7 : 8) : (sink ? 6 : (hs == 64l ? 0 : (hs == 128l ? 1 : (hs == 96l ? 9 : (hs == 512l ? 3 : (gated ? 4 : (cap ? 5 : 2)))))))
    return form * 4 + (f16 ? 2 : 0) + (khr ? 1 : 0)
}

//! `bidir` picks the vision towers' bidirectional twins (h64 and h128, f32 output, no gate, cap or sink)
def fa_stamp_ensure(khr : bool; hs : int64; gated, cap, sink, f16 : bool; bidir : bool = false) : bool {   // nolint:STYLE037 — stamp_ladder's generated arms, one a flash stamp
    let key = fa_stamp_key(khr, hs, gated, cap, sink, f16, bidir)
    var r = false
    stamp_ladder(key, FA_STAMPS) <| $() {
        r = stamp_ensure_cls()
    }
    return r
}

def fa_stamp_pair_ensure(khr : bool; hs : int64; gated, cap, sink, with_f16 : bool) : bool
    => fa_stamp_ensure(khr, hs, gated, cap, sink, false) && (!with_f16 || fa_stamp_ensure(khr, hs, gated, cap, sink, true))

def fa_stamp_set(khr : bool; bufs : uint64 const[4]; sizes : int64 const[4]; gbits : uint const[4]) : VkDescriptorSet
    => khr ? set_fa_khr_cls(bufs, sizes, gbits) : set_fa_cm2_cls(bufs, sizes, gbits)

//! the sink stamps' set: the four planes and the sink plane
def fa_stamp_sink_set(khr : bool; bufs : uint64 const[5]; sizes : int64 const[5]; gbits : uint const[5]) : VkDescriptorSet
    => khr ? set_fa_khr_sink_cls(bufs, sizes, gbits) : set_fa_cm2_sink_cls(bufs, sizes, gbits)

def fa_stamp_enc(khr : bool; hs : int64; gated, cap, sink, f16 : bool; raw : VkCommandBuffer; var h : VkHaz; var s : VkDescriptorSet; var pc : FaArgs; wgs : int64; bidir : bool = false) {   // nolint:STYLE037 — stamp_ladder's generated arms, one a flash stamp
    let key = fa_stamp_key(khr, hs, gated, cap, sink, f16, bidir)
    stamp_ladder(key, FA_STAMPS) <| $() {
        stamp_enc_cls(raw, h, s, pc, wgs)
    }
}


[vk_dispatch(name = "da_attn_bw_cls", grid = "wgs", params = "wgs : int64")]
class DaAttnBW : DaAttnBWT {
    typedef KT = float

    [spirv_kernel(local_size_x = 256)]
    def run {
        da_attn_bw_body()
    }
}

// f16-mirror twin — convert-on-load, staging/compute stay f32
[vk_dispatch(name = "da_attn_bw_f16_cls", grid = "wgs", params = "wgs : int64")]
class DaAttnBWF16 : DaAttnBWT {
    typedef KT = float16

    [spirv_kernel(local_size_x = 256)]
    def run {
        da_attn_bw_body()
    }
}

// batched causal attention over the decode mirrors (the template's run): K/V read at kbase/vbase,
// out = plain rows (no gating). Reduce order differs from the row kernel so the parity bar there
// is IDS; vs the shipped da_attn_b it is bit-exact
[vk_dispatch(name = "da_attn_b_cls", grid = "wgs", params = "wgs : int64")]
class DaAttnB : DaAttnBT {
    typedef KT = float

    [spirv_kernel(local_size_x = 256)]
    def run {
        at_attn_body()
    }
}

// f16-mirror twin — convert-on-load, staging/compute stay f32
[vk_dispatch(name = "da_attn_b_f16_cls", grid = "wgs", params = "wgs : int64")]
class DaAttnBF16 : DaAttnBT {
    typedef KT = float16

    [spirv_kernel(local_size_x = 256)]
    def run {
        at_attn_body()
    }
}


[vk_dispatch(name = "da_attn_b_h128_cls", grid = "wgs", params = "wgs : int64")]
class DaAttnBH128 : DaAttnBH128T {
    typedef KT = float

    [spirv_kernel(local_size_x = 128)]
    def run {
        da_attn_bh128_body()
    }
}

// f16-mirror twin — the tiles were f16 all along, only the plane loads change
[vk_dispatch(name = "da_attn_b_h128_f16_cls", grid = "wgs", params = "wgs : int64")]
class DaAttnBH128F16 : DaAttnBH128T {
    typedef KT = float16

    [spirv_kernel(local_size_x = 128)]
    def run {
        da_attn_bh128_body()
    }
}

//! The chunked prefill attention's stamp: the h128 coopmat twin under `h128`, the wide pass under `wide` (a head past 256),
//! else the narrow pass; `f16` picks each one's f16-mirror twin
def da_attn_b_stamp_ensure(h128, wide, f16 : bool) : bool {
    if (h128) return f16 ? ensure_da_attn_b_h128_f16_cls() : ensure_da_attn_b_h128_cls()
    if (wide) return f16 ? ensure_da_attn_bw_f16_cls() : ensure_da_attn_bw_cls()
    return f16 ? ensure_da_attn_b_f16_cls() : ensure_da_attn_b_cls()
}

//! ... its set
def da_attn_b_stamp_set(h128, wide, f16 : bool; bufs : uint64 const[4]; sizes : int64 const[4]; gbits : uint const[4]) : VkDescriptorSet {
    if (h128) return f16 ? set_da_attn_b_h128_f16_cls(bufs, sizes, gbits) : set_da_attn_b_h128_cls(bufs, sizes, gbits)
    if (wide) return f16 ? set_da_attn_bw_f16_cls(bufs, sizes, gbits) : set_da_attn_bw_cls(bufs, sizes, gbits)
    return f16 ? set_da_attn_b_f16_cls(bufs, sizes, gbits) : set_da_attn_b_cls(bufs, sizes, gbits)
}

//! ... and its dispatch
def da_attn_b_stamp_enc(h128, wide, f16 : bool; raw : VkCommandBuffer; var h : VkHaz; var s : VkDescriptorSet; var pc : DaAttnBArgs; wgs : int64) {
    if (h128 && f16) {
        enc_da_attn_b_h128_f16_cls(raw, h, s, pc, wgs)
    } elif (h128) {
        enc_da_attn_b_h128_cls(raw, h, s, pc, wgs)
    } elif (wide && f16) {
        enc_da_attn_bw_f16_cls(raw, h, s, pc, wgs)
    } elif (wide) {
        enc_da_attn_bw_cls(raw, h, s, pc, wgs)
    } elif (f16) {
        enc_da_attn_b_f16_cls(raw, h, s, pc, wgs)
    } else {
        enc_da_attn_b_cls(raw, h, s, pc, wgs)
    }
}

// ===== decode attention =====


[vk_dispatch(name = "da_attn_cls", grid = "wgs", params = "wgs : int64")]
class DaAttn : DaAttnT {
    typedef KT = float

    [spirv_kernel(local_size_x = 256)]
    def run {
        da_attn_body()
    }
}

//! the f16-mirror twin: convert-on-load, compute stays f32
[vk_dispatch(name = "da_attn_f16_cls", grid = "wgs", params = "wgs : int64")]
class DaAttnF16 : DaAttnT {
    typedef KT = float16
    override KV16 = true

    [spirv_kernel(local_size_x = 256)]
    def run {
        da_attn_body()
    }
}

[vk_dispatch(name = "da_attn_g2_cls", grid = "wgs", params = "wgs : int64")]
class DaAttnG2 : DaAttnT {
    typedef KT = float
    override G = 2

    [spirv_kernel(local_size_x = 256)]
    def run {
        da_attn_body()
    }
}

[vk_dispatch(name = "da_attn_f16_g2_cls", grid = "wgs", params = "wgs : int64")]
class DaAttnF16G2 : DaAttnT {
    typedef KT = float16
    override KV16 = true
    override G = 2

    [spirv_kernel(local_size_x = 256)]
    def run {
        da_attn_body()
    }
}

[vk_dispatch(name = "da_attn_sink_cls", grid = "wgs", params = "wgs : int64")]
class DaAttnSink : DaAttnT {
    typedef KT = float
    override SINK = true

    [spirv_kernel(local_size_x = 256)]
    def run {
        da_attn_body()
    }
}

[vk_dispatch(name = "da_attn_f16_sink_cls", grid = "wgs", params = "wgs : int64")]
class DaAttnF16Sink : DaAttnT {
    typedef KT = float16
    override KV16 = true
    override SINK = true

    [spirv_kernel(local_size_x = 256)]
    def run {
        da_attn_body()
    }
}

[vk_dispatch(name = "da_attn_q8_cls", grid = "wgs", params = "wgs : int64")]
class DaAttnQ8 : DaAttnT {
    typedef KT = uint
    override KVQ8 = true
    override KVBLK = true

    [spirv_kernel(local_size_x = 256)]
    def run {
        da_attn_body()
    }
}

[vk_dispatch(name = "da_attn_q8_g2_cls", grid = "wgs", params = "wgs : int64")]
class DaAttnQ8G2 : DaAttnT {
    typedef KT = uint
    override KVQ8 = true
    override KVBLK = true
    override G = 2

    [spirv_kernel(local_size_x = 256)]
    def run {
        da_attn_body()
    }
}

[vk_dispatch(name = "da_attn_q8_sink_cls", grid = "wgs", params = "wgs : int64")]
class DaAttnQ8Sink : DaAttnT {
    typedef KT = uint
    override KVQ8 = true
    override KVBLK = true
    override SINK = true

    [spirv_kernel(local_size_x = 256)]
    def run {
        da_attn_body()
    }
}

[vk_dispatch(name = "da_attn_tq4_cls", grid = "wgs", params = "wgs : int64")]
class DaAttnTq4 : DaAttnT {
    typedef KT = uint
    override KVTQ4 = true
    override KVBLK = true

    [spirv_kernel(local_size_x = 256)]
    def run {
        da_attn_body()
    }
}

[vk_dispatch(name = "da_attn_tq4_g2_cls", grid = "wgs", params = "wgs : int64")]
class DaAttnTq4G2 : DaAttnT {
    typedef KT = uint
    override KVTQ4 = true
    override KVBLK = true
    override G = 2

    [spirv_kernel(local_size_x = 256)]
    def run {
        da_attn_body()
    }
}

[vk_dispatch(name = "da_attn_tq4_sink_cls", grid = "wgs", params = "wgs : int64")]
class DaAttnTq4Sink : DaAttnT {
    typedef KT = uint
    override KVTQ4 = true
    override KVBLK = true
    override SINK = true

    [spirv_kernel(local_size_x = 256)]
    def run {
        da_attn_body()
    }
}

//! The decode attention's slab: the four-head group, the two-head one, or the sink stamp (four heads, the sink plane bound).
enum DaSlab {
    four
    two
    sink
}

//! The slab a layer's attention takes: the sink stamp on a model with attention sinks, else the two-head slab where
//! `da_slab_is_g2`, else the four-head one.
def da_slab_of(has_sinks : bool; kv_mul : int64) : DaSlab => has_sinks ? DaSlab.sink : (da_slab_is_g2(kv_mul) ? DaSlab.two : DaSlab.four)

//! every decode attention stamp's stem, a `DaSlab` each over the mirror codecs in `KVDtype` order
let private DA_STAMPS = "da_attn da_attn_g2 da_attn_sink da_attn_f16 da_attn_f16_g2 da_attn_f16_sink da_attn_q8 da_attn_q8_g2 da_attn_q8_sink da_attn_tq4 da_attn_tq4_g2 da_attn_tq4_sink"

def private da_stamp_key(dt : KVDtype; slab : DaSlab) : int => int(dt) * 3 + int(slab)

//! The decode attention stamp's name over a mirror in codec `dt` at `slab`: its `DA_STAMPS` stem, the census key less `_cls_spv`.
def da_attn_stamp_name(dt : KVDtype; slab : DaSlab) : string {
    let stems <- split(DA_STAMPS, " ")
    return stems[da_stamp_key(dt, slab)]
}

//! The decode attention's stamp over a mirror in codec `dt` at `slab`.
def da_attn_stamp_ensure(dt : KVDtype; slab : DaSlab) : bool {
    let key = da_stamp_key(dt, slab)
    var r = false
    stamp_ladder(key, DA_STAMPS) <| $() {
        r = stamp_ensure_cls()
    }
    return r
}

//! ... its set over the pass's eight planes and, ninth, the sink plane the sink stamps alone bind
def da_attn_stamp_set(dt : KVDtype; slab : DaSlab; bufs : uint64 const[9]; sizes : int64 const[9]; gbits : uint const[9]) : VkDescriptorSet {
    if (slab == DaSlab.sink) {
        if (dt == KVDtype.tq4) return set_da_attn_tq4_sink_cls(bufs, sizes, gbits)
        if (dt == KVDtype.q8_0) return set_da_attn_q8_sink_cls(bufs, sizes, gbits)
        return dt == KVDtype.f16 ? set_da_attn_f16_sink_cls(bufs, sizes, gbits) : set_da_attn_sink_cls(bufs, sizes, gbits)
    }
    let bufs8 = fixed_array(bufs[0], bufs[1], bufs[2], bufs[3], bufs[4], bufs[5], bufs[6], bufs[7])
    let sizes8 = fixed_array(sizes[0], sizes[1], sizes[2], sizes[3], sizes[4], sizes[5], sizes[6], sizes[7])
    let gbits8 = fixed_array(gbits[0], gbits[1], gbits[2], gbits[3], gbits[4], gbits[5], gbits[6], gbits[7])
    if (dt == KVDtype.tq4) return set_da_attn_tq4_cls(bufs8, sizes8, gbits8)
    if (dt == KVDtype.q8_0) return set_da_attn_q8_cls(bufs8, sizes8, gbits8)
    return dt == KVDtype.f16 ? set_da_attn_f16_cls(bufs8, sizes8, gbits8) : set_da_attn_cls(bufs8, sizes8, gbits8)
}

//! ... and its dispatch
def da_attn_stamp_enc(dt : KVDtype; slab : DaSlab; raw : VkCommandBuffer; var h : VkHaz; var s : VkDescriptorSet; var pc : DaAttnArgs; wgs : int64) {
    let key = da_stamp_key(dt, slab)
    stamp_ladder(key, DA_STAMPS) <| $() {
        stamp_enc_cls(raw, h, s, pc, wgs)
    }
}

// ===== the Q8_0 batch GEMM tile (the sdot4 prefill workhorse) =====


//! a fork of the per-256 `KqBatchBase` shell kept apart on purpose: its per-32 f16 scale pass runs over 8-block steps
//! (32x32 tiles through shared, K advancing 8 blocks per barrier pair), so only the 32x32 store (`MoeCmBase.store_y`) is shared
[vk_dispatch(name = "q8_batch_cls", grid = "wgs", params = "wgs : int64")]
class Q8Batch : MoeCmBase {
    @ssbo @binding = 0 wq : array<uint>        // weight quant words
    @ssbo @binding = 1 wsh : array<float16>    // per-block weight scales
    @ssbo @binding = 3 xqw : array<uint>       // activation quant words
    @ssbo @binding = 4 axs : array<float>      // per-block activation scales
    @workgroup xw4 : uint4[544]   // x tile: 32 rows x 16 uint4 chunks, stride 17 (bank-spread)
    @workgroup ww4 : uint4[544]   // w tile: 32 cols x 16 chunks, stride 17
    @workgroup xsw : float[256]   // x scales per (block, row)
    @workgroup wsw : float[256]   // w scales per (block, col)

    [spirv_kernel(local_size_x = 256, name = "q8_batch_cls_spv")]
    def run {   // nolint:STYLE038 — the verbatim tile pass (stage/reduce/store), parity-locked to the shipped kernel
        let reg = region_rec()
        let wblk0 = reg.x
        let row0 = reg.y
        let cnt = reg.z
        let tix = reg.w
        let wtiles = (pa.d + 31u) / 32u
        let xt = tix / wtiles
        let wt = tix % wtiles
        let nbb = pa.n / 32u
        let nsteps = (nbb + 7u) / 8u   // q8 K need not be superblock-aligned
        let tid = gl_LocalInvocationID.x
        let word = tid % 8u
        let grp = tid / 8u             // 0..31: the x row AND w col this thread stages
        let xrow = xt * 32u + grp
        let wcol = wt * 32u + grp
        let slane = tid % 32u
        let sblk = tid / 32u
        var f0 = 0.0
        var f1 = 0.0
        var f2 = 0.0
        var f3 = 0.0
        var s = 0u
        while (s < nsteps) {
            let b0 = s * 8u
            for (ki in range(2)) {     // this thread stages v4 chunks word and word+8 of its row/col
                let k = word + uint(ki) * 8u
                let bu = k / 2u
                let h = (k % 2u) * 4u
                var xv = uint4(0u)
                if (xrow < cnt && b0 + bu < nbb) {
                    let xb4 = ((row0 + xrow) * nbb + b0 + bu) * 8u + h
                    xv = uint4(xqw[xb4], xqw[xb4 + 1u], xqw[xb4 + 2u], xqw[xb4 + 3u])
                }
                xw4[grp * 17u + k] = xv
                var wv = uint4(0u)
                if (wcol < pa.d && b0 + bu < nbb) {
                    let wb4 = (wblk0 + wcol * nbb + b0 + bu) * 8u + h
                    wv = uint4(wq[wb4], wq[wb4 + 1u], wq[wb4 + 2u], wq[wb4 + 3u])
                }
                ww4[grp * 17u + k] = wv
            }
            var xs = 0.0
            if (xt * 32u + slane < cnt && b0 + sblk < nbb) {
                xs = axs[(row0 + xt * 32u + slane) * nbb + b0 + sblk]
            }
            xsw[sblk * 32u + slane] = xs
            var ws = 0.0
            if (wt * 32u + slane < pa.d && b0 + sblk < nbb) {
                ws = float(wsh[wblk0 + (wt * 32u + slane) * nbb + b0 + sblk])
            }
            wsw[sblk * 32u + slane] = ws
            barrier()
            for (bb in range(8)) {     // zero-filled tail blocks contribute 0 (ws = xs = 0)
                let bu = uint(bb)
                var i0 = 0
                var i1 = 0
                var i2 = 0
                var i3 = 0
                for (kk in range(2)) {
                    let k = bu * 2u + uint(kk)
                    let xw = xw4[grp * 17u + k]
                    let wv0 = ww4[word * 17u + k]
                    let wv1 = ww4[(word + 8u) * 17u + k]
                    let wv2 = ww4[(word + 16u) * 17u + k]
                    let wv3 = ww4[(word + 24u) * 17u + k]
                    i0 += sdot4(wv0.x, xw.x) + sdot4(wv0.y, xw.y) + sdot4(wv0.z, xw.z) + sdot4(wv0.w, xw.w)
                    i1 += sdot4(wv1.x, xw.x) + sdot4(wv1.y, xw.y) + sdot4(wv1.z, xw.z) + sdot4(wv1.w, xw.w)
                    i2 += sdot4(wv2.x, xw.x) + sdot4(wv2.y, xw.y) + sdot4(wv2.z, xw.z) + sdot4(wv2.w, xw.w)
                    i3 += sdot4(wv3.x, xw.x) + sdot4(wv3.y, xw.y) + sdot4(wv3.z, xw.z) + sdot4(wv3.w, xw.w)
                }
                let xscl = xsw[bu * 32u + grp]
                f0 += float(i0) * wsw[bu * 32u + word] * xscl
                f1 += float(i1) * wsw[bu * 32u + word + 8u] * xscl
                f2 += float(i2) * wsw[bu * 32u + word + 16u] * xscl
                f3 += float(i3) * wsw[bu * 32u + word + 24u] * xscl
            }
            barrier()
            s++
        }
        store_y(xrow, cnt, row0, wt, word, float4(f0, f1, f2, f3))
    }
}

//! Q8Batch's 32x32 tile shape on Q8_K activations (256-weight superblocks); only the w-nibble
//! compose, the scale stage, and the per-block FMA (float order contractual per format) differ -
//! each is an override, and the devirtualizer inlines the family back to standalone kernels, one a leaf.
class KqBatchBase : MoeCmBase {
    @ssbo @binding = 0 wq : array<uint>        // weight quant planes (per-format block layout)
    @ssbo @binding = 1 wsu : array<uint>       // scale planes (5-word superblock layout)
    @ssbo @binding = 3 xqw : array<uint>       // Q8_K activation words (64 per row-superblock)
    @ssbo @binding = 4 axs : array<float>      // one Q8_K d per (row, superblock)
    @workgroup xw4 : uint4[544]
    @workgroup ww4 : uint4[544]
    @workgroup xsw : float[256]
    @workgroup wsw : float[256]    // first per-(block, col) scale plane
    @workgroup wsw2 : float[256]   // second plane (the d/min pair's min side; k6's hi sub-scale)

    // x tile stage: this thread's two uint4 chunks of the Q8_K activation rows
    def load_x(row0, xrow, cnt, nsb, s, word, grp : uint) {
        for (ki in range(2)) {
            let k = word + uint(ki) * 8u
            let bu = k / 2u
            let hh = k % 2u
            var xv = uint4(0u)
            if (xrow < cnt) {
                let xb4 = ((row0 + xrow) * nsb + s) * 64u + bu * 8u + hh * 4u
                xv = uint4(xqw[xb4], xqw[xb4 + 1u], xqw[xb4 + 2u], xqw[xb4 + 3u])
            }
            xw4[grp * 17u + k] = xv
        }
    }

    def load_xs(row0, xt, cnt, nsb, s, tid : uint) {
        if (tid < 32u) {
            var xs = 0.0
            if (xt * 32u + tid < cnt) {
                xs = axs[(row0 + xt * 32u + tid) * nsb + s]
            }
            xsw[tid] = xs
        }
    }

    // w-scale stage for the d/min pair form (Q4_K/Q5_K share the 5-word wsu layout)
    def load_dm(wsb0, wt, nsb, s, slane, sblk : uint) {
        var sa = 0.0
        var sbv = 0.0
        if (wt * 32u + slane < pa.d) {
            let wsb = wsb0 + (wt * 32u + slane) * nsb + s
            let sm = k4_scale_min(unpackHalf2x16(wsu[wsb * 5u]), wsu[wsb * 5u + 1u + sblk / 4u], wsu[wsb * 5u + 3u + sblk / 4u], sblk)
            sa = sm.x
            sbv = sm.y
        }
        wsw[sblk * 32u + slane] = sa
        wsw2[sblk * 32u + slane] = sbv
    }

    // one 32-weight block's integer tile dots: this thread's 4 output columns + the x block sum
    def dots(bu, grp, word : uint; var i : int4&) : int {
        var bs = 0
        for (kk in range(2)) {
            let k = bu * 2u + uint(kk)
            let xw = xw4[grp * 17u + k]
            let wv0 = ww4[word * 17u + k]
            let wv1 = ww4[(word + 8u) * 17u + k]
            let wv2 = ww4[(word + 16u) * 17u + k]
            let wv3 = ww4[(word + 24u) * 17u + k]
            bs += sdot4(xw.x, 0x01010101) + sdot4(xw.y, 0x01010101) + sdot4(xw.z, 0x01010101) + sdot4(xw.w, 0x01010101)
            i.x += sdot4(wv0.x, xw.x) + sdot4(wv0.y, xw.y) + sdot4(wv0.z, xw.z) + sdot4(wv0.w, xw.w)
            i.y += sdot4(wv1.x, xw.x) + sdot4(wv1.y, xw.y) + sdot4(wv1.z, xw.z) + sdot4(wv1.w, xw.w)
            i.z += sdot4(wv2.x, xw.x) + sdot4(wv2.y, xw.y) + sdot4(wv2.z, xw.z) + sdot4(wv2.w, xw.w)
            i.w += sdot4(wv3.x, xw.x) + sdot4(wv3.y, xw.y) + sdot4(wv3.z, xw.z) + sdot4(wv3.w, xw.w)
        }
        return bs
    }

    //! one 32-weight block's split-half integer dots: the low and the high 16-weight half's four column dots, and each half's x block sum
    def split_dots(bu, grp, word : uint; var l, h : int4&; var blo, bhi : int&) {
        let kL = bu * 2u
        let kH = bu * 2u + 1u
        let xwL = xw4[grp * 17u + kL]
        let xwH = xw4[grp * 17u + kH]
        blo = sdot4(xwL.x, 0x01010101) + sdot4(xwL.y, 0x01010101) + sdot4(xwL.z, 0x01010101) + sdot4(xwL.w, 0x01010101)
        bhi = sdot4(xwH.x, 0x01010101) + sdot4(xwH.y, 0x01010101) + sdot4(xwH.z, 0x01010101) + sdot4(xwH.w, 0x01010101)
        let wL0 = ww4[word * 17u + kL]
        let wL1 = ww4[(word + 8u) * 17u + kL]
        let wL2 = ww4[(word + 16u) * 17u + kL]
        let wL3 = ww4[(word + 24u) * 17u + kL]
        let wH0 = ww4[word * 17u + kH]
        let wH1 = ww4[(word + 8u) * 17u + kH]
        let wH2 = ww4[(word + 16u) * 17u + kH]
        let wH3 = ww4[(word + 24u) * 17u + kH]
        l.x = sdot4(wL0.x, xwL.x) + sdot4(wL0.y, xwL.y) + sdot4(wL0.z, xwL.z) + sdot4(wL0.w, xwL.w)
        l.y = sdot4(wL1.x, xwL.x) + sdot4(wL1.y, xwL.y) + sdot4(wL1.z, xwL.z) + sdot4(wL1.w, xwL.w)
        l.z = sdot4(wL2.x, xwL.x) + sdot4(wL2.y, xwL.y) + sdot4(wL2.z, xwL.z) + sdot4(wL2.w, xwL.w)
        l.w = sdot4(wL3.x, xwL.x) + sdot4(wL3.y, xwL.y) + sdot4(wL3.z, xwL.z) + sdot4(wL3.w, xwL.w)
        h.x = sdot4(wH0.x, xwH.x) + sdot4(wH0.y, xwH.y) + sdot4(wH0.z, xwH.z) + sdot4(wH0.w, xwH.w)
        h.y = sdot4(wH1.x, xwH.x) + sdot4(wH1.y, xwH.y) + sdot4(wH1.z, xwH.z) + sdot4(wH1.w, xwH.w)
        h.z = sdot4(wH2.x, xwH.x) + sdot4(wH2.y, xwH.y) + sdot4(wH2.z, xwH.z) + sdot4(wH2.w, xwH.w)
        h.w = sdot4(wH3.x, xwH.x) + sdot4(wH3.y, xwH.y) + sdot4(wH3.z, xwH.z) + sdot4(wH3.w, xwH.w)
    }

    // per-block FMA, d/min form: xscl * (d*sc*idot - dmin*mn*bsum)
    def fma_dm(bu, grp, word : uint; xscl : float) : float4 {
        var i = int4(0)
        let bs = dots(bu, grp, word, i)
        let ws = float4(wsw[bu * 32u + word], wsw[bu * 32u + word + 8u],
            wsw[bu * 32u + word + 16u], wsw[bu * 32u + word + 24u])
        let wsb2 = float4(wsw2[bu * 32u + word], wsw2[bu * 32u + word + 8u],
            wsw2[bu * 32u + word + 16u], wsw2[bu * 32u + word + 24u])
        return xscl * (ws * float4(i) - wsb2 * float(bs))
    }

    // the variant points — every [spirv_kernel] leaf overrides w_quad; q40/k6 also stage_ws/blk_fma
    //! the format's staged int8 lanes of chunk `k` - block `bu`'s half `hh` - of the weight superblock at `wsb`
    [unused_argument(wsb, bu, hh, k)]
    def w_quad(wsb, bu, hh, k : uint) : uint4 => uint4(0u)

    def stage_w(wsb0, wcol, nsb, s, word, grp : uint) {
        for (ki in range(2)) {
            let k = word + uint(ki) * 8u
            var wv = uint4(0u)
            if (wcol < pa.d) {
                wv = w_quad(wsb0 + wcol * nsb + s, k / 2u, k % 2u, k)
            }
            ww4[grp * 17u + k] = wv
        }
    }

    def stage_ws(wsb0, wt, nsb, s, tid : uint) {
        load_dm(wsb0, wt, nsb, s, tid % 32u, tid / 32u)
    }

    //! the per-32 f16 d scale stage of the Q4_0-plane formats (q40, iq4nl): block `tid / 32`'s d of row `tid % 32`, two d a word
    def stage_d32(wsb0, wt, nsb, s, tid : uint) {
        let slane = tid % 32u
        let sblk = tid / 32u
        var sa = 0.0
        if (wt * 32u + slane < pa.d) {
            let wsb = wsb0 + (wt * 32u + slane) * nsb + s
            sa = q40_d_of(wsu[wsb * 5u + sblk / 2u], sblk)
        }
        wsw[sblk * 32u + slane] = sa
    }

    def blk_fma(bu, grp, word : uint; xscl : float) : float4 => fma_dm(bu, grp, word, xscl)

    def tile_shell {
        let reg = region_rec()
        let wsb0 = reg.x
        let row0 = reg.y
        let cnt = reg.z
        let tix = reg.w
        let wtiles = (pa.d + 31u) / 32u
        let xt = tix / wtiles
        let wt = tix % wtiles
        let nsb = pa.n / 256u
        let tid = gl_LocalInvocationID.x
        let word = tid % 8u
        let grp = tid / 8u
        let xrow = xt * 32u + grp
        let wcol = wt * 32u + grp
        var f = float4(0.0)
        var s = 0u
        while (s < nsb) {
            load_x(row0, xrow, cnt, nsb, s, word, grp)
            stage_w(wsb0, wcol, nsb, s, word, grp)
            load_xs(row0, xt, cnt, nsb, s, tid)
            stage_ws(wsb0, wt, nsb, s, tid)
            barrier()
            let xscl = xsw[grp]
            for (bb in range(8)) {
                f += blk_fma(uint(bb), grp, word, xscl)
            }
            barrier()
            s++
        }
        store_y(xrow, cnt, row0, wt, word, f)
    }
}

// Q4_K tile: per block acc += xs * (d*sc*idot - dmin*mn*bsum)
[vk_dispatch(name = "kq_batch_k4_cls", kernel = "run", family = "kq_batch_cls", grid = "wgs", params = "wgs : int64")]
class KqBatchK4 : KqBatchBase {
    [unused_argument(k)]
    def override w_quad(wsb, bu, hh, k : uint) : uint4 {
        let rb4 = wsb * 32u + bu * 4u
        let r0 = wq[rb4]
        let r1 = wq[rb4 + 1u]
        let r2 = wq[rb4 + 2u]
        let r3 = wq[rb4 + 3u]
        if (hh == 0u) {
            return uint4(r0 & 0x0F0F0F0F, r1 & 0x0F0F0F0F, r2 & 0x0F0F0F0F, r3 & 0x0F0F0F0F)
        }
        return uint4((r0 >> 4u) & 0x0F0F0F0F, (r1 >> 4u) & 0x0F0F0F0F,
            (r2 >> 4u) & 0x0F0F0F0F, (r3 >> 4u) & 0x0F0F0F0F)
    }

    [spirv_kernel(local_size_x = 256, name = "kq_batch_k4_cls_spv")]
    def run {
        tile_shell()
    }
}

// Q4_0 tile: the k4 staging with one scale plane — per block acc += xs * d * (idot - 8*bsum)
[vk_dispatch(name = "kq_batch_q40_cls", kernel = "run", family = "kq_batch_cls", grid = "wgs", params = "wgs : int64")]
class KqBatchQ40 : KqBatchK4 {
    def override stage_ws(wsb0, wt, nsb, s, tid : uint) {
        stage_d32(wsb0, wt, nsb, s, tid)
    }

    def override blk_fma(bu, grp, word : uint; xscl : float) : float4 {
        var i = int4(0)
        let bs = dots(bu, grp, word, i)
        let ws = float4(wsw[bu * 32u + word], wsw[bu * 32u + word + 8u],
            wsw[bu * 32u + word + 16u], wsw[bu * 32u + word + 24u])
        return xscl * ws * float4(i - int4(8 * bs))
    }

    [spirv_kernel(local_size_x = 256, name = "kq_batch_q40_cls_spv")]
    def override run {
        tile_shell()
    }
}

// IQ4_XS tile: the k4 staging with the codebook decode in stage_w (signed weight lanes, so no
// block-sum term) and one signed-scale plane — per block acc += xs * d*sc * idot
[vk_dispatch(name = "kq_batch_iq4xs_cls", kernel = "run", family = "kq_batch_cls", grid = "wgs", params = "wgs : int64")]
class KqBatchIq4xs : KqBatchBase {
    //! the format's codebook over a word of four nibble lanes, and its sub-block scale off the two-word row: the two
    //! places a codebook format on this tiling differs (`KqBatchIq3s4` overrides both)
    def cb_word(n : uint) : uint => iq4_word(n)
    def sub_sc(w0, w1, blk : uint) : int => iq4xs_sc(w0, w1, blk)

    [unused_argument(k)]
    def override w_quad(wsb, bu, hh, k : uint) : uint4 {
        let rb4 = wsb * 32u + bu * 4u
        let sh = hh * 4u
        return uint4(cb_word((wq[rb4] >> sh) & 0x0F0F0F0F), cb_word((wq[rb4 + 1u] >> sh) & 0x0F0F0F0F),
            cb_word((wq[rb4 + 2u] >> sh) & 0x0F0F0F0F), cb_word((wq[rb4 + 3u] >> sh) & 0x0F0F0F0F))
    }

    def override stage_ws(wsb0, wt, nsb, s, tid : uint) {
        let slane = tid % 32u
        let sblk = tid / 32u
        var sa = 0.0
        if (wt * 32u + slane < pa.d) {
            let wsb = wsb0 + (wt * 32u + slane) * nsb + s
            let w0 = wsu[wsb * 2u]
            sa = unpackHalf2x16(w0).x * float(sub_sc(w0, wsu[wsb * 2u + 1u], sblk))
        }
        wsw[sblk * 32u + slane] = sa
    }

    def override blk_fma(bu, grp, word : uint; xscl : float) : float4 {
        var i = int4(0)
        dots(bu, grp, word, i)
        let ws = float4(wsw[bu * 32u + word], wsw[bu * 32u + word + 8u],
            wsw[bu * 32u + word + 16u], wsw[bu * 32u + word + 24u])
        return xscl * ws * float4(i)
    }

    [spirv_kernel(local_size_x = 256, name = "kq_batch_iq4xs_cls_spv")]
    def run {
        tile_shell()
    }
}

// the grid formats' scale stage over the iq3 row: d x the (1 + 2s) fields, not iq4xs's ls - 32;
// iq4xs's fold (no bsum term) inherits. iq3s, iq3xxs and iq2xxs stage their grids and gather on it
class KqBatchGridBase : KqBatchIq4xs {
    [unused_argument(w0)]
    def override sub_sc(w0, w1, blk : uint) : int => grid_sc(w1, blk)
}

//! iq3s4 (IQ3_S as a nibble codebook plane): iq4xs's tiling and staging, the odd codebook 2q - 15, the iq3 row's (1 + 2s) sub-scale
[vk_dispatch(name = "kq_batch_iq3s4_cls", kernel = "run", family = "kq_batch_cls", grid = "wgs", params = "wgs : int64")]
class KqBatchIq3s4 : KqBatchGridBase {
    def override cb_word(n : uint) : uint => iq3s4_word(n)

    [spirv_kernel(local_size_x = 256, name = "kq_batch_iq3s4_cls_spv")]
    def override run {
        tile_shell()
    }
}

// IQ3_S tile: the iq4xs staging with the GRID GATHER in stage_w - each staged uint4 is four
// grid words sign-applied. The 2 KB grid stages into workgroup memory once, before the tile loop.
[vk_dispatch(name = "kq_batch_iq3s_cls", kernel = "run", family = "kq_batch_cls", grid = "wgs", params = "wgs : int64")]
class KqBatchIq3s : KqBatchGridBase {
    @workgroup gridw : uint[512]

    [unused_argument(k)]
    def override w_quad(wsb, bu, hh, k : uint) : uint4 {
        let qsw = wq[wsb * 26u + bu * 2u + hh]
        let qh = (wq[wsb * 26u + 16u + bu / 4u] >> ((bu % 4u) * 8u)) & 255u
        let i = iq3s_idx4(qsw, qh, hh * 2u)
        let sg = sign_word_sgn4(wq[wsb * 26u + 18u + bu], hh * 2u)
        return uint4(iq3s_signed(gridw[i.x], sg.x), iq3s_signed(gridw[i.y], sg.y), iq3s_signed(gridw[i.z], sg.z), iq3s_signed(gridw[i.w], sg.w))
    }

    [spirv_kernel(local_size_x = 256, name = "kq_batch_iq3s_cls_spv")]
    def override run {
        let tid = gl_LocalInvocationID.x
        gridw[tid] = iq3s_grid_word(int(tid))
        gridw[tid + 256u] = iq3s_grid_word(int(tid + 256u))
        barrier()   // the grid visible before the first stage_w
        tile_shell()
    }
}

// IQ3_XXS tile: the iq3s staging with the halved-grid gather - bare 8-bit indices (no ninth
// bit), signs from the aux32's four 7-bit ksigns indices via the parity helper (no table).
// The 1 KB grid stages once.
[vk_dispatch(name = "kq_batch_iq3xxs_cls", kernel = "run", family = "kq_batch_cls", grid = "wgs", params = "wgs : int64")]
class KqBatchIq3xxs : KqBatchGridBase {
    @workgroup gridw : uint[256]

    [unused_argument(k)]
    def override w_quad(wsb, bu, hh, k : uint) : uint4 {
        let i = iq3xxs_idx4(wq[wsb * 24u + bu * 2u + hh])
        let sg = iq3xxs_sgn4(wq[wsb * 24u + 16u + bu], hh * 2u)
        return uint4(iq3s_signed(gridw[i.x], sg.x), iq3s_signed(gridw[i.y], sg.y), iq3s_signed(gridw[i.z], sg.z), iq3s_signed(gridw[i.w], sg.w))
    }

    [spirv_kernel(local_size_x = 256, name = "kq_batch_iq3xxs_cls_spv")]
    def override run {
        let tid = gl_LocalInvocationID.x
        gridw[tid] = iq3xxs_grid_word(int(tid))
        barrier()   // the grid visible before the first stage_w
        tile_shell()
    }
}

// IQ2_XXS tile: the iq3xxs shell over the two-word u64 grid - block b's grid word (column 2b)
// carries four BYTE indices, its aux32 (column 2b+1) the parity-derived ksigns; the iq3 strip
// stage and iq4xs's fma inherit (the (2ls+1) strips read exactly under the signed extract,
// staying below 128).
[vk_dispatch(name = "kq_batch_iq2xxs_cls", kernel = "run", family = "kq_batch_cls", grid = "wgs", params = "wgs : int64")]
class KqBatchIq2xxs : KqBatchGridBase {
    @workgroup gridw : uint[512]

    [unused_argument(k)]
    def override w_quad(wsb, bu, hh, k : uint) : uint4 {
        let i = iq2xxs_idx4(wq[wsb * 16u + bu * 2u], hh)
        let sg = iq2xxs_sgn4(wq[wsb * 16u + bu * 2u + 1u], hh)
        return uint4(iq3s_signed(gridw[i.x], sg.x), iq3s_signed(gridw[i.y], sg.y), iq3s_signed(gridw[i.z], sg.z), iq3s_signed(gridw[i.w], sg.w))
    }

    [spirv_kernel(local_size_x = 256, name = "kq_batch_iq2xxs_cls_spv")]
    def override run {
        let tid = gl_LocalInvocationID.x
        gridw[tid] = iq2xxs_grid_word(int(tid))
        gridw[tid + 256u] = iq2xxs_grid_word(int(tid + 256u))
        barrier()   // the iq2xxs grid visible before the first stage_w
        tile_shell()
    }
}

// IQ4_NL tile: iq4xs's LUT staging (stage_w and the no-min blk_fma inherit) with q40's
// per-block f16 d staged as the scale - no sub-scales, no bsum term anywhere.
[vk_dispatch(name = "kq_batch_iq4nl_cls", kernel = "run", family = "kq_batch_cls", grid = "wgs", params = "wgs : int64")]
class KqBatchIq4nl : KqBatchIq4xs {
    def override stage_ws(wsb0, wt, nsb, s, tid : uint) {
        stage_d32(wsb0, wt, nsb, s, tid)
    }

    [spirv_kernel(local_size_x = 256, name = "kq_batch_iq4nl_cls_spv")]
    def override run {
        tile_shell()
    }
}

// Q5_K tile: the k4 compose with the 5th bit OR'd in from the 40B block's qh words
[vk_dispatch(name = "kq_batch_k5_cls", kernel = "run", family = "kq_batch_cls", grid = "wgs", params = "wgs : int64")]
class KqBatchK5 : KqBatchBase {
    [unused_argument(k)]
    def override w_quad(wsb, bu, hh, k : uint) : uint4 {
        let rb4 = wsb * 40u + bu * 4u
        return k5_quad(uint4(wq[rb4], wq[rb4 + 1u], wq[rb4 + 2u], wq[rb4 + 3u]), wq[wsb * 40u + 32u + bu], hh * 4u)
    }

    [spirv_kernel(local_size_x = 256, name = "kq_batch_k5_cls_spv")]
    def run {
        tile_shell()
    }
}

// Q6_K tile: composed 6-bit quants, the -32 folded per 16-half as slo/shi vs the half's block
// sum — the lo/hi word split IS the sub-scale split
[vk_dispatch(name = "kq_batch_k6_cls", kernel = "run", family = "kq_batch_cls", grid = "wgs", params = "wgs : int64")]
class KqBatchK6 : KqBatchBase {
    [unused_argument(k)]
    def override w_quad(wsb, bu, hh, k : uint) : uint4 {
        let rb4 = wsb * 48u + bu * 4u
        let qb4 = wsb * 48u + 32u + (bu / 4u) * 8u + hh * 4u
        let sh = hh * 4u
        let nib = uint4((wq[rb4] >> sh) & KQC_NIBBLE_MASK, (wq[rb4 + 1u] >> sh) & KQC_NIBBLE_MASK, (wq[rb4 + 2u] >> sh) & KQC_NIBBLE_MASK, (wq[rb4 + 3u] >> sh) & KQC_NIBBLE_MASK)
        return k6_quad(nib, uint4(wq[qb4], wq[qb4 + 1u], wq[qb4 + 2u], wq[qb4 + 3u]), (bu % 4u) * 2u)
    }

    def override stage_ws(wsb0, wt, nsb, s, tid : uint) {
        let slane = tid % 32u
        let sblk = tid / 32u
        var sa = 0.0
        var sbv = 0.0
        if (wt * 32u + slane < pa.d) {
            let wsb = wsb0 + (wt * 32u + slane) * nsb + s
            let s4 = int4(unpack8(int(wsu[wsb * 5u + sblk / 2u])))
            let slo = (sblk % 2u) == 0u ? s4.x : s4.z
            let shi = (sblk % 2u) == 0u ? s4.y : s4.w
            let dd = unpackHalf2x16(wsu[wsb * 5u + 4u]).x
            sa = dd * float(slo)
            sbv = dd * float(shi)
        }
        wsw[sblk * 32u + slane] = sa
        wsw2[sblk * 32u + slane] = sbv
    }

    //! the quant offset the split-half fold subtracts through each half's block sum
    def q_off : int => 32

    // the split-half FMA: each 16-weight half keeps its own dot, sum, and signed sub-scale
    def override blk_fma(bu, grp, word : uint; xscl : float) : float4 {
        let off = q_off()
        var l = int4(0)
        var h = int4(0)
        var blo = 0
        var bhi = 0
        split_dots(bu, grp, word, l, h, blo, bhi)
        return float4(xscl * (wsw[bu * 32u + word] * float(l.x - off * blo) + wsw2[bu * 32u + word] * float(h.x - off * bhi)),
            xscl * (wsw[bu * 32u + word + 8u] * float(l.y - off * blo) + wsw2[bu * 32u + word + 8u] * float(h.y - off * bhi)),
            xscl * (wsw[bu * 32u + word + 16u] * float(l.z - off * blo) + wsw2[bu * 32u + word + 16u] * float(h.z - off * bhi)),
            xscl * (wsw[bu * 32u + word + 24u] * float(l.w - off * blo) + wsw2[bu * 32u + word + 24u] * float(h.w - off * bhi)))
    }

    [spirv_kernel(local_size_x = 256, name = "kq_batch_k6_cls_spv")]
    def run {
        tile_shell()
    }
}

// Q3_K tile: k6's split-half fold with the offset 4 over the k3 planes (24 words per superblock:
// 16 qs words, 8 hmask words) — block bu takes qs words (bu/4)*8 + hh*4.. at shift 2*(bu%4)
// OR the hmask words 16 + hh*4.. bit bu, shifted up two
[vk_dispatch(name = "kq_batch_k3_cls", kernel = "run", family = "kq_batch_cls", grid = "wgs", params = "wgs : int64")]
class KqBatchK3 : KqBatchK6 {
    [unused_argument(k)]
    def override w_quad(wsb, bu, hh, k : uint) : uint4 {
        let qb4 = wsb * 24u + (bu / 4u) * 8u + hh * 4u
        let hb4 = wsb * 24u + 16u + hh * 4u
        return k3_quad(uint4(wq[qb4], wq[qb4 + 1u], wq[qb4 + 2u], wq[qb4 + 3u]), uint4(wq[hb4], wq[hb4 + 1u], wq[hb4 + 2u], wq[hb4 + 3u]), (bu % 4u) * 2u, bu)
    }

    def override q_off : int => 4

    [spirv_kernel(local_size_x = 256, name = "kq_batch_k3_cls_spv")]
    def override run {
        tile_shell()
    }
}

// Q2_K tile: k3's staging minus the hmask (unsigned 0..3 lanes), FOUR scale planes - d*sc and
// dmin*mn per 16-half - and a split-half fma folding the min sides against the half sums.
[vk_dispatch(name = "kq_batch_k2_cls", kernel = "run", family = "kq_batch_cls", grid = "wgs", params = "wgs : int64")]
class KqBatchK2 : KqBatchK6 {
    @workgroup wsw3 : float[256]   // dmin*mn, lo half
    @workgroup wsw4 : float[256]   // dmin*mn, hi half

    [unused_argument(k)]
    def override w_quad(wsb, bu, hh, k : uint) : uint4 {
        let qb4 = wsb * 16u + (bu / 4u) * 8u + hh * 4u
        return k2_quad(uint4(wq[qb4], wq[qb4 + 1u], wq[qb4 + 2u], wq[qb4 + 3u]), (bu % 4u) * 2u)
    }

    def override stage_ws(wsb0, wt, nsb, s, tid : uint) {
        let slane = tid % 32u
        let sblk = tid / 32u
        var sa = 0.0
        var sbv = 0.0
        var ma = 0.0
        var mb = 0.0
        if (wt * 32u + slane < pa.d) {
            let wsb = wsb0 + (wt * 32u + slane) * nsb + s
            let w = wsu[wsb * 5u + 1u + sblk / 2u]
            let b0 = (w >> ((sblk % 2u) * 16u)) & 255u
            let b1 = (w >> ((sblk % 2u) * 16u + 8u)) & 255u
            let dm = unpackHalf2x16(wsu[wsb * 5u])
            sa = dm.x * float(b0 & 15u)
            sbv = dm.x * float(b1 & 15u)
            ma = dm.y * float(b0 >> 4u)
            mb = dm.y * float(b1 >> 4u)
        }
        wsw[sblk * 32u + slane] = sa
        wsw2[sblk * 32u + slane] = sbv
        wsw3[sblk * 32u + slane] = ma
        wsw4[sblk * 32u + slane] = mb
    }

    // the split-half fma with the min planes: xscl * (sc_lo*l + sc_hi*h - mn_lo*blo - mn_hi*bhi)
    def override blk_fma(bu, grp, word : uint; xscl : float) : float4 {
        var l = int4(0)
        var h = int4(0)
        var blo = 0
        var bhi = 0
        split_dots(bu, grp, word, l, h, blo, bhi)
        return float4(xscl * (wsw[bu * 32u + word] * float(l.x) + wsw2[bu * 32u + word] * float(h.x) - wsw3[bu * 32u + word] * float(blo) - wsw4[bu * 32u + word] * float(bhi)),
            xscl * (wsw[bu * 32u + word + 8u] * float(l.y) + wsw2[bu * 32u + word + 8u] * float(h.y) - wsw3[bu * 32u + word + 8u] * float(blo) - wsw4[bu * 32u + word + 8u] * float(bhi)),
            xscl * (wsw[bu * 32u + word + 16u] * float(l.z) + wsw2[bu * 32u + word + 16u] * float(h.z) - wsw3[bu * 32u + word + 16u] * float(blo) - wsw4[bu * 32u + word + 16u] * float(bhi)),
            xscl * (wsw[bu * 32u + word + 24u] * float(l.w) + wsw2[bu * 32u + word + 24u] * float(h.w) - wsw3[bu * 32u + word + 24u] * float(blo) - wsw4[bu * 32u + word + 24u] * float(bhi)))
    }

    [spirv_kernel(local_size_x = 256, name = "kq_batch_k2_cls_spv")]
    def override run {
        tile_shell()
    }
}

// the iq2 grid formats' scale stage and fold: per-16 UNSIGNED strips on k6's wsw/wsw2 planes and a
// plain split fma (no offset, no min term); iq2s and iq2xs stage their grids and gather on it
class KqBatchIq2Base : KqBatchK6 {
    def override stage_ws(wsb0, wt, nsb, s, tid : uint) {
        let slane = tid % 32u
        let sblk = tid / 32u
        var sa = 0.0
        var sbv = 0.0
        if (wt * 32u + slane < pa.d) {
            let wsb = wsb0 + (wt * 32u + slane) * nsb + s
            let w = wsu[wsb * 5u + 1u + sblk / 2u]
            let s0 = (w >> ((sblk % 2u) * 16u)) & 255u
            let s1 = (w >> ((sblk % 2u) * 16u + 8u)) & 255u
            let dd = unpackHalf2x16(wsu[wsb * 5u]).x
            sa = dd * float(s0)
            sbv = dd * float(s1)
        }
        wsw[sblk * 32u + slane] = sa
        wsw2[sblk * 32u + slane] = sbv
    }

    // the split-half fma with UNSIGNED strips: xscl * (d*s0*l + d*s1*h) - no offset, no min
    def override blk_fma(bu, grp, word : uint; xscl : float) : float4 {
        var l = int4(0)
        var h = int4(0)
        var blo = 0
        var bhi = 0
        split_dots(bu, grp, word, l, h, blo, bhi)
        return float4(xscl * (wsw[bu * 32u + word] * float(l.x) + wsw2[bu * 32u + word] * float(h.x)),
            xscl * (wsw[bu * 32u + word + 8u] * float(l.y) + wsw2[bu * 32u + word + 8u] * float(h.y)),
            xscl * (wsw[bu * 32u + word + 16u] * float(l.z) + wsw2[bu * 32u + word + 16u] * float(h.z)),
            xscl * (wsw[bu * 32u + word + 24u] * float(l.w) + wsw2[bu * 32u + word + 24u] * float(h.w)))
    }
}

// IQ2_S tile: the iq3s grid gather widened to the u64 grid (a 10-bit qs|qh index picks TWO
// words - eight magnitudes) with the block's own sign bytes. The 8 KB grid stages once, 8 words
// per thread.
[vk_dispatch(name = "kq_batch_iq2s_cls", kernel = "run", family = "kq_batch_cls", grid = "wgs", params = "wgs : int64")]
class KqBatchIq2s : KqBatchIq2Base {
    @workgroup gridw : uint[2048]

    [unused_argument(k)]
    def override w_quad(wsb, bu, hh, k : uint) : uint4 {
        let qh = (wq[wsb * 18u + 16u + bu / 4u] >> ((bu % 4u) * 8u)) & 255u
        let i = iq2s_idx4(wq[wsb * 18u + bu], qh, hh * 2u)
        let sg = sign_word_sgn4(wq[wsb * 18u + 8u + bu], hh * 2u)
        return uint4(iq3s_signed(gridw[i.x], sg.x), iq3s_signed(gridw[i.y], sg.y), iq3s_signed(gridw[i.z], sg.z), iq3s_signed(gridw[i.w], sg.w))
    }

    [spirv_kernel(local_size_x = 256, name = "kq_batch_iq2s_cls_spv")]
    def override run {
        let tid = gl_LocalInvocationID.x
        for (k in range(8)) {
            gridw[tid + uint(k) * 256u] = iq2s_grid_word(int(tid + uint(k) * 256u))
        }
        barrier()   // the grid visible before the first stage_w
        tile_shell()
    }
}

// IQ2_XS tile: the iq2s gather with ksigns-by-parity - each u16 qs word carries a 9-bit grid
// index (two staged words) and a 7-bit KSIGNS_IQ2XS index whose sign byte ksign7 recomputes
// (no sign plane, no qh). The 4 KB grid stages once.
[vk_dispatch(name = "kq_batch_iq2xs_cls", kernel = "run", family = "kq_batch_cls", grid = "wgs", params = "wgs : int64")]
class KqBatchIq2xs : KqBatchIq2Base {
    @workgroup gridw : uint[1024]

    [unused_argument(bu, hh)]
    def override w_quad(wsb, bu, hh, k : uint) : uint4 {
        let qw = wq[wsb * 16u + k]
        let i = iq2xs_idx4(qw)
        let sg = iq2xs_sgn4(qw)
        return uint4(iq3s_signed(gridw[i.x], sg.x), iq3s_signed(gridw[i.y], sg.y), iq3s_signed(gridw[i.z], sg.z), iq3s_signed(gridw[i.w], sg.w))
    }

    [spirv_kernel(local_size_x = 256, name = "kq_batch_iq2xs_cls_spv")]
    def override run {
        let tid = gl_LocalInvocationID.x
        for (k in range(4)) {
            gridw[tid + uint(k) * 256u] = iq2xs_grid_word(int(tid + uint(k) * 256u))
        }
        barrier()   // the iq2xs grid visible before the first stage_w
        tile_shell()
    }
}

// ===== the prefill batch GEMMs — cooperative-matrix variants (DASLLAMA_COOPMAT=f16|int8|mm) =====

let CML_SST = 20u   //! mm shared row stride in uints: 16 f16 pairs + 4 pad (banks)

// The f16 tensor-core 32x32 tile shell (DASLLAMA_COOPMAT=f16): the MoE routing of Q8Batch with the
// inner GEMM on the tensor cores - 128 threads / 4 subgroups, one 16x16 subtile each, the staged
// f16 A (32 rows x 256 k) and B (256 k x 32 cols) tiles, 16 MMAs per step, the f32 scratch bounce
// for the bounds-checked store. A leaf stages its own weight format into the tiles (edge rows
// zero-fill) and hands the shell its step count and the block count its stage reads.
class CmF16Base : MoeCmBase {
    @ssbo @binding = 3 xqw : array<uint>       // activation quant words
    @ssbo @binding = 4 axs : array<float>      // per-block activation scales
    @workgroup cm_as : float16[8192]   // 32 local rows x 256 k, row-major stride 256 (f16 dequant A)
    @workgroup cm_bs : float16[8192]   // 256 k x 32 cols, row-major stride 32 (f16 dequant B)
    @workgroup cm_out : float[1024]    // 4 subgroups x 16x16 f32 scratch (bounds-checked write-out)

    // step s of the leaf's format: the activation rows into cm_as, the weight columns into cm_bs
    def stage(s, wblk0, row0, cnt, xt, wt, nblk, tid : uint) {
    }

    def body(nsteps, nblk : uint) {
        let reg = region_rec()
        let wblk0 = reg.x
        let row0 = reg.y
        let cnt = reg.z
        let tix = reg.w
        let wtiles = (pa.d + 31u) / 32u
        let xt = tix / wtiles
        let wt = tix % wtiles
        let tid = gl_LocalInvocationID.x
        let sg = gl_SubgroupID          // 0..3 (subgroupSize 32)
        let row_half = sg / 2u               // which 16-row half of the tile
        let col_half = sg % 2u               // which 16-col half of the tile
        var acc : coopmatAcc_f32_16x16
        var s = 0u
        while (s < nsteps) {
            stage(s, wblk0, row0, cnt, xt, wt, nblk, tid)
            barrier()
            for (bb in range(8)) {       // 8 blocks x 2 K-subtiles = 16 MMAs into one acc
                for (ks in range(2)) {
                    var a : coopmatA_f16_16x16
                    var b : coopmatB_f16_16x16
                    coopmatLoad(a, cm_as, int(row_half * 16u * 256u + uint(bb) * 32u + uint(ks) * 16u), 256, 0)
                    coopmatLoad(b, cm_bs, int((uint(bb) * 32u + uint(ks) * 16u) * 32u + col_half * 16u), 32, 0)
                    acc = coopmatMulAdd(a, b, acc)
                }
            }
            barrier()
            s++
        }
        coopmatStore(acc, cm_out, int(sg * 256u), 16, 0)   // scratch, then bounds-checked write-out
        barrier()
        let lane = gl_SubgroupInvocationID
        for (e in range(8)) {
            let idx = lane * 8u + uint(e)
            let m = idx / 16u
            let nn = idx % 16u
            let orow = xt * 32u + row_half * 16u + m
            let ocol = wt * 32u + col_half * 16u + nn
            if (orow < cnt && ocol < pa.d) {
                y[(row0 + orow) * pa.d + ocol] = cm_out[sg * 256u + idx]
            }
        }
    }
}

//! the mul_mm tile (DASLLAMA_COOPMAT=mm) over the tile edge and the edge policy: `l_tile` and `m_tile` differ in BT alone and stay two bodies
//! with named accumulators - held as one coopmat array walked under `[unroll_full]` the tile read flat per dispatch and 2.5% under on the served
//! Qwen3-4B Q8_0 pp512 (followup_vulkan.md row 93); each body's MMA order is the shipped kernel's, A row by A row, each against every B column
[ |> template_struct_instance]
class template MmBatchT : MoeCmBase {
    @ssbo @binding = 0 wq : array<uint>        // weight quant words
    @ssbo @binding = 1 wsh : array<float16>    // per-block weight scales
    @ssbo @binding = 3 xqw : array<uint>       //! activation quant words
    @ssbo @binding = 4 axs : array<float>      //! per-block activation scales
    @template_constant BT : uint = 128u        //! tile edge: 128 aligned-L (BM=BN=128, BK=32), 64 the M twin
    @template_constant EDGE : bool = true      //! rows past d / cnt: guarded staging, bounced stores
    @workgroup cml_a : uint[int(BT * CML_SST)]   //! A tile: BT weight rows x one block (32 K) as f16 pairs
    @workgroup cml_b : uint[int(BT * CML_SST)]   //! B tile: BT token rows x one block
    @workgroup @template_gate = EDGE cml_o : float16[1024]   //! 4 warps x 16x16 f16-acc store bounce (edge tiles only)

    // stage A: BT weight rows x 1 block; OOR rows -> 0 on the edge policy
    def stage_a(wblk0, gm0, nbb, kb, lw, lr0 : uint) {
        for [unroll] (p in range(int(BT) / 16)) {
            let row = lr0 + uint(p) * 16u
            let wrow = gm0 + row
            var v = float4(0.0)
            static_if (EDGE) {
                if (wrow < pa.d) {
                    let ib = wblk0 + wrow * nbb + kb
                    v = float4(int4(unpack8(int(wq[ib * 8u + lw])))) * float(wsh[ib])
                }
            } else {
                let ib = wblk0 + wrow * nbb + kb
                v = float4(int4(unpack8(int(wq[ib * 8u + lw])))) * float(wsh[ib])
            }
            let o = row * CML_SST + lw * 2u
            cml_a[o] = packHalf2x16(v.xy)
            cml_a[o + 1u] = packHalf2x16(v.zw)
        }
    }

    // stage B: BT token rows x 1 block; OOR rows -> 0 on the edge policy
    def stage_b(row0, gn0, cnt, nbb, kb, lw, lr0 : uint) {
        for [unroll] (p in range(int(BT) / 16)) {
            let row = lr0 + uint(p) * 16u
            var v = float4(0.0)
            static_if (EDGE) {
                if (gn0 + row < cnt) {
                    let ib = (row0 + gn0 + row) * nbb + kb
                    v = float4(int4(unpack8(int(xqw[ib * 8u + lw])))) * axs[ib]
                }
            } else {
                let ib = (row0 + gn0 + row) * nbb + kb
                v = float4(int4(unpack8(int(xqw[ib * 8u + lw])))) * axs[ib]
            }
            let o = row * CML_SST + lw * 2u
            cml_b[o] = packHalf2x16(v.xy)
            cml_b[o + 1u] = packHalf2x16(v.zw)
        }
    }

    // an edge tile's fragment, already stored to this warp's cml_o slab (rowmajor, row = weight
    // dim): 32 lanes widen and write 8 elements each under the row and column bounds
    def bounce_out(fr, fc, dr, dc, row0, cnt : uint) {
        static_if (EDGE) {
            let lane = gl_SubgroupInvocationID
            let ob = gl_SubgroupID * 256u
            for (e in range(8)) {
                let idx = lane * 8u + uint(e)
                let wr = dr + fr * 16u + idx / 16u
                let tc = dc + fc * 16u + idx % 16u
                if (wr < pa.d && tc < cnt) {
                    y[(row0 + tc) * pa.d + wr] = float(cml_o[ob + idx])
                }
            }
        }
    }

    [spirv_inline]
    def mm_batch_body {
        let reg = region_rec()
        let wblk0 = reg.x
        let row0 = reg.y
        let cnt = reg.z
        let tix = reg.w
        let wtiles = (pa.d + BT - 1u) / BT
        let xt = tix / wtiles             // token (B) tile
        let wt = tix % wtiles             // weight (A) tile
        let gm0 = wt * BT
        let gn0 = xt * BT
        let nbb = pa.n / 32u
        let tid = gl_LocalInvocationID.x
        let warp_r = gl_SubgroupID % 2u   // which half of the A tile
        let warp_c = gl_SubgroupID / 2u   // which half of the B tile
        let lw = tid % 8u                 // which packed word (4 qs) of the staged row's block
        let lr0 = tid / 8u                // staging row base 0..15
        static_if (BT == 128u) {
            l_tile(wblk0, row0, cnt, gm0, gn0, nbb, warp_r, warp_c, lw, lr0)
        } else {
            m_tile(wblk0, row0, cnt, gm0, gn0, nbb, warp_r, warp_c, lw, lr0)
        }
    }

    def l_tile(wblk0, row0, cnt, gm0, gn0, nbb, warp_r, warp_c, lw, lr0 : uint) {   // nolint:STYLE038 — the verbatim tile pass, parity-locked to the shipped kernel
        static_if (BT == 128u) {
        var acc00, acc01, acc02, acc03, acc10, acc11, acc12, acc13, acc20, acc21, acc22, acc23, acc30, acc31, acc32, acc33 : coopmatAcc_f16_16x16
        var kb = 0u
        while (kb < nbb) {
            stage_a(wblk0, gm0, nbb, kb, lw, lr0)
            stage_b(row0, gn0, cnt, nbb, kb, lw, lr0)
            barrier()
            for [unroll] (ks in range(2)) {
                let k8 = uint(ks) * 8u
                var ca : coopmatA_f16_16x16
                var cb : coopmatB_f16_16x16
                coopmatLoad(ca, cml_a, int((warp_r * 64u) * CML_SST + k8), int(CML_SST), 0)
                coopmatLoad(cb, cml_b, int((warp_c * 64u) * CML_SST + k8), int(CML_SST), 1)
                acc00 = coopmatMulAdd(ca, cb, acc00)
                coopmatLoad(cb, cml_b, int((warp_c * 64u + 16u) * CML_SST + k8), int(CML_SST), 1)
                acc01 = coopmatMulAdd(ca, cb, acc01)
                coopmatLoad(cb, cml_b, int((warp_c * 64u + 32u) * CML_SST + k8), int(CML_SST), 1)
                acc02 = coopmatMulAdd(ca, cb, acc02)
                coopmatLoad(cb, cml_b, int((warp_c * 64u + 48u) * CML_SST + k8), int(CML_SST), 1)
                acc03 = coopmatMulAdd(ca, cb, acc03)
                coopmatLoad(ca, cml_a, int((warp_r * 64u + 16u) * CML_SST + k8), int(CML_SST), 0)
                coopmatLoad(cb, cml_b, int((warp_c * 64u) * CML_SST + k8), int(CML_SST), 1)
                acc10 = coopmatMulAdd(ca, cb, acc10)
                coopmatLoad(cb, cml_b, int((warp_c * 64u + 16u) * CML_SST + k8), int(CML_SST), 1)
                acc11 = coopmatMulAdd(ca, cb, acc11)
                coopmatLoad(cb, cml_b, int((warp_c * 64u + 32u) * CML_SST + k8), int(CML_SST), 1)
                acc12 = coopmatMulAdd(ca, cb, acc12)
                coopmatLoad(cb, cml_b, int((warp_c * 64u + 48u) * CML_SST + k8), int(CML_SST), 1)
                acc13 = coopmatMulAdd(ca, cb, acc13)
                coopmatLoad(ca, cml_a, int((warp_r * 64u + 32u) * CML_SST + k8), int(CML_SST), 0)
                coopmatLoad(cb, cml_b, int((warp_c * 64u) * CML_SST + k8), int(CML_SST), 1)
                acc20 = coopmatMulAdd(ca, cb, acc20)
                coopmatLoad(cb, cml_b, int((warp_c * 64u + 16u) * CML_SST + k8), int(CML_SST), 1)
                acc21 = coopmatMulAdd(ca, cb, acc21)
                coopmatLoad(cb, cml_b, int((warp_c * 64u + 32u) * CML_SST + k8), int(CML_SST), 1)
                acc22 = coopmatMulAdd(ca, cb, acc22)
                coopmatLoad(cb, cml_b, int((warp_c * 64u + 48u) * CML_SST + k8), int(CML_SST), 1)
                acc23 = coopmatMulAdd(ca, cb, acc23)
                coopmatLoad(ca, cml_a, int((warp_r * 64u + 48u) * CML_SST + k8), int(CML_SST), 0)
                coopmatLoad(cb, cml_b, int((warp_c * 64u) * CML_SST + k8), int(CML_SST), 1)
                acc30 = coopmatMulAdd(ca, cb, acc30)
                coopmatLoad(cb, cml_b, int((warp_c * 64u + 16u) * CML_SST + k8), int(CML_SST), 1)
                acc31 = coopmatMulAdd(ca, cb, acc31)
                coopmatLoad(cb, cml_b, int((warp_c * 64u + 32u) * CML_SST + k8), int(CML_SST), 1)
                acc32 = coopmatMulAdd(ca, cb, acc32)
                coopmatLoad(cb, cml_b, int((warp_c * 64u + 48u) * CML_SST + k8), int(CML_SST), 1)
                acc33 = coopmatMulAdd(ca, cb, acc33)
            }
            barrier()
            kb++
        }
        // stores: y[(row0 + token) * d + wrow] — fragment weight-dim contiguous = column-major, stride d
        let dr = gm0 + warp_r * 64u
        let dc = gn0 + warp_c * 64u
        let whole = !EDGE || (gm0 + 128u <= pa.d && gn0 + 128u <= cnt)
        if (whole) {
            var accw : coopmatAcc_f32_16x16
            let yb = (row0 + dc) * pa.d + dr
            coopmatConvert(accw, acc00)
            coopmatStore(accw, y, int(yb), int(pa.d), 1)
            coopmatConvert(accw, acc01)
            coopmatStore(accw, y, int(yb + 16u * pa.d), int(pa.d), 1)
            coopmatConvert(accw, acc02)
            coopmatStore(accw, y, int(yb + 32u * pa.d), int(pa.d), 1)
            coopmatConvert(accw, acc03)
            coopmatStore(accw, y, int(yb + 48u * pa.d), int(pa.d), 1)
            coopmatConvert(accw, acc10)
            coopmatStore(accw, y, int(yb + 16u), int(pa.d), 1)
            coopmatConvert(accw, acc11)
            coopmatStore(accw, y, int(yb + 16u * pa.d + 16u), int(pa.d), 1)
            coopmatConvert(accw, acc12)
            coopmatStore(accw, y, int(yb + 32u * pa.d + 16u), int(pa.d), 1)
            coopmatConvert(accw, acc13)
            coopmatStore(accw, y, int(yb + 48u * pa.d + 16u), int(pa.d), 1)
            coopmatConvert(accw, acc20)
            coopmatStore(accw, y, int(yb + 32u), int(pa.d), 1)
            coopmatConvert(accw, acc21)
            coopmatStore(accw, y, int(yb + 16u * pa.d + 32u), int(pa.d), 1)
            coopmatConvert(accw, acc22)
            coopmatStore(accw, y, int(yb + 32u * pa.d + 32u), int(pa.d), 1)
            coopmatConvert(accw, acc23)
            coopmatStore(accw, y, int(yb + 48u * pa.d + 32u), int(pa.d), 1)
            coopmatConvert(accw, acc30)
            coopmatStore(accw, y, int(yb + 48u), int(pa.d), 1)
            coopmatConvert(accw, acc31)
            coopmatStore(accw, y, int(yb + 16u * pa.d + 48u), int(pa.d), 1)
            coopmatConvert(accw, acc32)
            coopmatStore(accw, y, int(yb + 32u * pa.d + 48u), int(pa.d), 1)
            coopmatConvert(accw, acc33)
            coopmatStore(accw, y, int(yb + 48u * pa.d + 48u), int(pa.d), 1)
        } else {
            static_if (EDGE) {
                // edge tile: every fragment bounces its f16 acc through this warp's cml_o slab
                let ob = gl_SubgroupID * 256u
                for (fi in range(16)) {
                    let fr = uint(fi) / 4u       // fragment row: weight offset fr*16
                    let fc = uint(fi) % 4u       // fragment col: token offset fc*16
                    if (fi == 0) {
                        coopmatStore(acc00, cml_o, int(ob), 16, 0)
                    } elif (fi == 1) {
                        coopmatStore(acc01, cml_o, int(ob), 16, 0)
                    } elif (fi == 2) {
                        coopmatStore(acc02, cml_o, int(ob), 16, 0)
                    } elif (fi == 3) {
                        coopmatStore(acc03, cml_o, int(ob), 16, 0)
                    } elif (fi == 4) {
                        coopmatStore(acc10, cml_o, int(ob), 16, 0)
                    } elif (fi == 5) {
                        coopmatStore(acc11, cml_o, int(ob), 16, 0)
                    } elif (fi == 6) {
                        coopmatStore(acc12, cml_o, int(ob), 16, 0)
                    } elif (fi == 7) {
                        coopmatStore(acc13, cml_o, int(ob), 16, 0)
                    } elif (fi == 8) {
                        coopmatStore(acc20, cml_o, int(ob), 16, 0)
                    } elif (fi == 9) {
                        coopmatStore(acc21, cml_o, int(ob), 16, 0)
                    } elif (fi == 10) {
                        coopmatStore(acc22, cml_o, int(ob), 16, 0)
                    } elif (fi == 11) {
                        coopmatStore(acc23, cml_o, int(ob), 16, 0)
                    } elif (fi == 12) {
                        coopmatStore(acc30, cml_o, int(ob), 16, 0)
                    } elif (fi == 13) {
                        coopmatStore(acc31, cml_o, int(ob), 16, 0)
                    } elif (fi == 14) {
                        coopmatStore(acc32, cml_o, int(ob), 16, 0)
                    } else {
                        coopmatStore(acc33, cml_o, int(ob), 16, 0)
                    }
                    barrier()
                    bounce_out(fr, fc, dr, dc, row0, cnt)
                    barrier()
                }
            }
        }
        }
    }

    def m_tile(wblk0, row0, cnt, gm0, gn0, nbb, warp_r, warp_c, lw, lr0 : uint) {
        static_if (BT == 64u) {
        var acc00, acc01, acc10, acc11 : coopmatAcc_f16_16x16
        var kb = 0u
        while (kb < nbb) {
            stage_a(wblk0, gm0, nbb, kb, lw, lr0)
            stage_b(row0, gn0, cnt, nbb, kb, lw, lr0)
            barrier()
            for [unroll] (ks in range(2)) {   // 2 K-subtiles x (2 A rows x 2 B cols), the L MMA order
                let k8 = uint(ks) * 8u
                var ca : coopmatA_f16_16x16
                var cb : coopmatB_f16_16x16
                coopmatLoad(ca, cml_a, int((warp_r * 32u) * CML_SST + k8), int(CML_SST), 0)
                coopmatLoad(cb, cml_b, int((warp_c * 32u) * CML_SST + k8), int(CML_SST), 1)
                acc00 = coopmatMulAdd(ca, cb, acc00)
                coopmatLoad(cb, cml_b, int((warp_c * 32u + 16u) * CML_SST + k8), int(CML_SST), 1)
                acc01 = coopmatMulAdd(ca, cb, acc01)
                coopmatLoad(ca, cml_a, int((warp_r * 32u + 16u) * CML_SST + k8), int(CML_SST), 0)
                coopmatLoad(cb, cml_b, int((warp_c * 32u) * CML_SST + k8), int(CML_SST), 1)
                acc10 = coopmatMulAdd(ca, cb, acc10)
                coopmatLoad(cb, cml_b, int((warp_c * 32u + 16u) * CML_SST + k8), int(CML_SST), 1)
                acc11 = coopmatMulAdd(ca, cb, acc11)
            }
            barrier()
            kb++
        }
        // stores: y[(row0 + token) * d + wrow] — same column-major layout as the L tile
        let dr = gm0 + warp_r * 32u
        let dc = gn0 + warp_c * 32u
        let whole = !EDGE || (gm0 + 64u <= pa.d && gn0 + 64u <= cnt)
        if (whole) {
            var accw : coopmatAcc_f32_16x16
            let yb = (row0 + dc) * pa.d + dr
            coopmatConvert(accw, acc00)
            coopmatStore(accw, y, int(yb), int(pa.d), 1)
            coopmatConvert(accw, acc01)
            coopmatStore(accw, y, int(yb + 16u * pa.d), int(pa.d), 1)
            coopmatConvert(accw, acc10)
            coopmatStore(accw, y, int(yb + 16u), int(pa.d), 1)
            coopmatConvert(accw, acc11)
            coopmatStore(accw, y, int(yb + 16u * pa.d + 16u), int(pa.d), 1)
        } else {
            static_if (EDGE) {
                // edge tile: bounce each f16 acc through this warp's cml_o slab, bounds-checked write-out
                let ob = gl_SubgroupID * 256u
                for (fi in range(4)) {
                    let fr = uint(fi) / 2u       // fragment row: weight offset fr*16
                    let fc = uint(fi) % 2u       // fragment col: token offset fc*16
                    if (fi == 0) {
                        coopmatStore(acc00, cml_o, int(ob), 16, 0)
                    } elif (fi == 1) {
                        coopmatStore(acc01, cml_o, int(ob), 16, 0)
                    } elif (fi == 2) {
                        coopmatStore(acc10, cml_o, int(ob), 16, 0)
                    } else {
                        coopmatStore(acc11, cml_o, int(ob), 16, 0)
                    }
                    barrier()
                    bounce_out(fr, fc, dr, dc, row0, cnt)
                    barrier()
                }
            }
        }
        }
    }
}


//! the Q8_0 leaf: eight 32-blocks a step, a staged uint a block word
[vk_dispatch(name = "q8_batch_cmf16_cls", grid = "wgs", params = "wgs : int64")]
class CmF16Batch : CmF16Base {
    @ssbo @binding = 0 wq : array<uint>        //! weight quant words
    @ssbo @binding = 1 wsh : array<float16>    //! per-block weight scales

    def override stage(s, wblk0, row0, cnt, xt, wt, nblk, tid : uint) {
        let b0 = s * 8u
        for (i in range(16)) {       // stage A: 32x64 = 2048 uints, 16/thread; OOR -> 0
            let u = tid * 16u + uint(i)
            let tok_row = u / 64u
            let chunk = u % 64u
            let blk = chunk / 8u
            let w = chunk % 8u
            let xrow = xt * 32u + tok_row
            let gblk = b0 + blk
            var packed = 0u
            var xs = 0.0
            if (xrow < cnt && gblk < nblk) {
                packed = xqw[((row0 + xrow) * nblk + gblk) * 8u + w]
                xs = axs[(row0 + xrow) * nblk + gblk]
            }
            let qv = int4(unpack8(int(packed)))
            let o = tok_row * 256u + blk * 32u + w * 4u
            cm_as[o] = float16(float(qv.x) * xs)
            cm_as[o + 1u] = float16(float(qv.y) * xs)
            cm_as[o + 2u] = float16(float(qv.z) * xs)
            cm_as[o + 3u] = float16(float(qv.w) * xs)
        }
        for (i in range(16)) {       // stage B: 32x64 = 2048 uints, 16/thread; OOR -> 0
            let u = tid * 16u + uint(i)
            let out_col = u / 64u
            let chunk = u % 64u
            let blk = chunk / 8u
            let w = chunk % 8u
            let wcol = wt * 32u + out_col
            let gblk = b0 + blk
            var packed = 0u
            var ws = 0.0
            if (wcol < pa.d && gblk < nblk) {
                packed = wq[(wblk0 + wcol * nblk + gblk) * 8u + w]
                ws = float(wsh[wblk0 + wcol * nblk + gblk])
            }
            let qv = int4(unpack8(int(packed)))
            let k = blk * 32u + w * 4u
            cm_bs[k * 32u + out_col] = float16(float(qv.x) * ws)
            cm_bs[(k + 1u) * 32u + out_col] = float16(float(qv.y) * ws)
            cm_bs[(k + 2u) * 32u + out_col] = float16(float(qv.z) * ws)
            cm_bs[(k + 3u) * 32u + out_col] = float16(float(qv.w) * ws)
        }
    }

    [spirv_kernel(local_size_x = 128, name = "q8_batch_cmf16_cls_spv")]
    def run {
        let nbb = pa.n / 32u
        body((nbb + 7u) / 8u, nbb)
    }
}

//! int8 path: one MMA a Q8_0 block, s32 spilled for the SIMT scale
[vk_dispatch(name = "q8_batch_cmi8_cls", grid = "wgs", params = "wgs : int64")]
class CmI8Batch : MoeCmBase {
    @ssbo @binding = 0 wq8 : array<int8>       //! int8 view of the weight quant bytes
    @ssbo @binding = 1 wsh : array<float16>    //! per-block weight scales
    @ssbo @binding = 3 xq8 : array<int8>       //! int8 view of the activation quant bytes
    @ssbo @binding = 4 axs : array<float>      //! per-block activation scales
    @workgroup cmi_s32 : int[1024]     //! 4 subgroups x 16x16 s32 spill
    @workgroup cmi_acc : float[1024]   //! 4 subgroups x 16x16 f32 accumulator

    [spirv_kernel(local_size_x = 128, name = "q8_batch_cmi8_cls_spv")]
    def run {
        let reg = region_rec()
        let wblk0 = reg.x
        let row0 = reg.y
        let cnt = reg.z
        let tix = reg.w
        let wtiles = (pa.d + 31u) / 32u
        let xt = tix / wtiles
        let wt = tix % wtiles
        let nbb = pa.n / 32u
        let sg = gl_SubgroupID
        let lane = gl_SubgroupInvocationID
        let row_half = sg / 2u
        let col_half = sg % 2u
        let sg_tok_row0 = xt * 32u + row_half * 16u
        let sg_out_col0 = wt * 32u + col_half * 16u
        for (e in range(8)) {
            cmi_acc[sg * 256u + lane * 8u + uint(e)] = 0.0
        }
        var blk = 0u
        while (blk < nbb) {
            var a : coopmatA_s8_16x32
            var b : coopmatB_s8_32x16
            coopmatLoad(a, xq8, ((row0 + sg_tok_row0) * nbb + blk) * 32u, int(pa.n), 0)      // A 16 rows, rowmajor
            coopmatLoad(b, wq8, (wblk0 + sg_out_col0 * nbb + blk) * 32u, int(pa.n), 1)       // B 16 cols, colmajor
            var s32 : coopmatAcc_s32_16x16
            s32 = coopmatMulAdd(a, b, s32)   // nolint:STYLE011 — MulAdd seeds from its own zero-init tile
            coopmatStore(s32, cmi_s32, int(sg * 256u), 16, 0)
            barrier()
            for (e in range(8)) {
                let idx = lane * 8u + uint(e)
                let m = idx / 16u
                let nn = idx % 16u
                let xs = axs[(row0 + sg_tok_row0 + m) * nbb + blk]
                let ws = float(wsh[wblk0 + (sg_out_col0 + nn) * nbb + blk])
                // i*ws*xs: Q8Batch's order, bit-parity with sdot4
                cmi_acc[sg * 256u + idx] += float(cmi_s32[sg * 256u + idx]) * ws * xs
            }
            barrier()
            blk++
        }
        for (e in range(8)) {
            let idx = lane * 8u + uint(e)
            let m = idx / 16u
            let nn = idx % 16u
            let orow = sg_tok_row0 + m
            let ocol = sg_out_col0 + nn
            if (orow < cnt && ocol < pa.d) {
                y[(row0 + orow) * pa.d + ocol] = cmi_acc[sg * 256u + idx]
            }
        }
    }
}


// the L tile with edge tiles
[vk_dispatch(name = "q8_batch_mm_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class MmBatch : MmBatchT {
    override BT = 128u
    override EDGE = true

    [spirv_kernel(local_size_x = 128)]
    def run {
        mm_batch_body()
    }
}

// the aligned L twin: staging guards + edge branch removed — its own pipeline for d and every cnt 128-multiples
[vk_dispatch(name = "q8_batch_mm_a_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class MmABatch : MmBatchT {
    override BT = 128u
    override EDGE = false

    [spirv_kernel(local_size_x = 128)]
    def run {
        mm_batch_body()
    }
}

// the M twin (64x64), routed in when cnt or d <= 64
[vk_dispatch(name = "q8_batch_mm_m_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class MmMBatch : MmBatchT {
    override BT = 64u
    override EDGE = true

    [spirv_kernel(local_size_x = 128)]
    def run {
        mm_batch_body()
    }
}

// Q4_0 coopmat f16 variant (fmt=4 stacks): the q40 math baked into f16 dequant — w = (nib-8)*d_b,
// a = aq*xs (per-256 Q8_K) — then the same 4-subgroup MMA as CmF16Batch
[vk_dispatch(name = "kq_batch_q40_cmf16_cls", grid = "wgs", params = "wgs : int64")]
class KqQ40CmF16 : CmF16Base {
    @ssbo @binding = 0 wq : array<uint>        // nibble-packed weight superblocks (32 words each)
    @ssbo @binding = 1 wsu : array<uint>       // 5-word scale superblocks (d halves)

    def override stage(s, wblk0, row0, cnt, xt, wt, nblk, tid : uint) {
        // stage A: 32 rows x 64 uints (one superblock of Q8_K acts), 16/thread — one thread stays
        // in one row (16 divides 64), so xs (per-256) loads ONCE per thread
        let arow0 = tid * 16u / 64u            // this thread's row
        let ac0 = tid * 16u % 64u              // first chunk within the row
        let axrow = xt * 32u + arow0
        var xs = 0.0
        if (axrow < cnt) {
            xs = axs[(row0 + axrow) * nblk + s]
        }
        for (i in range(16)) {
            let c = ac0 + uint(i)
            let bu = c / 8u
            let ww = c % 8u
            var packed = 0u
            if (axrow < cnt) {
                packed = xqw[((row0 + axrow) * nblk + s) * 64u + c]
            }
            let qv = int4(unpack8(int(packed)))
            let o = arow0 * 256u + bu * 32u + ww * 4u
            cm_as[o] = float16(float(qv.x) * xs)
            cm_as[o + 1u] = float16(float(qv.y) * xs)
            cm_as[o + 2u] = float16(float(qv.z) * xs)
            cm_as[o + 3u] = float16(float(qv.w) * xs)
        }
        // stage B: 32 cols x 32 uints (one superblock of nibble-packed weights), 8/thread; each
        // uint yields 4 lo-nibble weights (k = bu*32 + w*4 + j) and 4 hi (k + 16), dequant (nib-8)*d_b
        for (i in range(8)) {
            let u2 = tid * 8u + uint(i)
            let col = u2 / 32u
            let c = u2 % 32u
            let bu = c / 4u
            let w = c % 4u
            let wcol = wt * 32u + col
            var r = 0u
            var db = 0.0
            if (wcol < pa.d) {
                let wsb = wblk0 + wcol * nblk + s
                r = wq[wsb * 32u + c]
                db = q40_d_of(wsu[wsb * 5u + bu / 2u], bu)
            }
            let k = bu * 32u + w * 4u
            for (j in range(4)) {
                let lo = int((r >> (uint(j) * 8u)) & 0x0Fu)
                let hi = int((r >> (uint(j) * 8u + 4u)) & 0x0Fu)
                cm_bs[(k + uint(j)) * 32u + col] = float16(float(lo - 8) * db)
                cm_bs[(k + 16u + uint(j)) * 32u + col] = float16(float(hi - 8) * db)
            }
        }
    }

    [spirv_kernel(local_size_x = 128, name = "kq_batch_q40_cmf16_cls_spv")]
    def run {
        let nsb = pa.n / 256u
        body(nsb, nsb)
    }
}

[vk_dispatch(name = "q8_batch_cm2l_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Q8Cm2LBatch : Q8Cm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "q8_batch_cm2m_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Q8Cm2MBatch : Q8Cm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "q8_batch_cm2s_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Q8Cm2SBatch : Q8Cm2T {
    override BN = 32u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "q51_batch_cm2l_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Q51Cm2LBatch : Q51Cm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "q51_batch_cm2m_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Q51Cm2MBatch : Q51Cm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "iq4nl32_batch_cm2l_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq4nl32Cm2LBatch : Iq4nl32Cm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "iq4nl32_batch_cm2m_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq4nl32Cm2MBatch : Iq4nl32Cm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "q4032_batch_cm2l_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Q4032Cm2LBatch : Q4032Cm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "q4032_batch_cm2m_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Q4032Cm2MBatch : Q4032Cm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "q5032_batch_cm2l_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Q5032Cm2LBatch : Q5032Cm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "q5032_batch_cm2m_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Q5032Cm2MBatch : Q5032Cm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

//! the q51 rail's s column: the expert schedule's 32-row pieces and the narrow window's column
[vk_dispatch(name = "q51_batch_cm2s_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Q51Cm2SBatch : Q51Cm2T {
    override BN = 32u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

//! the iq4nl32 rail's s column: the expert schedule's 32-row pieces (the routed down planes off the 256 lattice) and the narrow window's column
[vk_dispatch(name = "iq4nl32_batch_cm2s_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq4nl32Cm2SBatch : Iq4nl32Cm2T {
    override BN = 32u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

//! the q4032 rail's s column: iq4nl32's shape (the window's dense planes and the expert schedule's 32-row pieces)
[vk_dispatch(name = "q4032_batch_cm2s_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Q4032Cm2SBatch : Q4032Cm2T {
    override BN = 32u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

//! the q5032 rail's s column: q4032's shape (the window's dense planes and the expert schedule's 32-row pieces)
[vk_dispatch(name = "q5032_batch_cm2s_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Q5032Cm2SBatch : Q5032Cm2T {
    override BN = 32u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

//! the mx4 rail's s column: the expert schedule's 32-row pieces (gpt-oss's routed stacks; a dense mx4 plane never exists)
[vk_dispatch(name = "mx4_batch_cm2s_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Mx4Cm2SBatch : Mx4Cm2T {
    override BN = 32u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_k4_cm2l_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K4Cm2LBatch : K4Cm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_k4_cm2m_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K4Cm2MBatch : K4Cm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_k4_cm2s_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K4Cm2SBatch : K4Cm2T {
    override BN = 32u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_k6_cm2l_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K6Cm2LBatch : K6Cm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_k6_cm2m_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K6Cm2MBatch : K6Cm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_k6_cm2s_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K6Cm2SBatch : K6Cm2T {
    override BN = 32u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_k5_cm2l_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K5Cm2LBatch : K5Cm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_k5_cm2m_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K5Cm2MBatch : K5Cm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_k5_cm2s_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K5Cm2SBatch : K5Cm2T {
    override BN = 32u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_q40_cm2l_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Q40Cm2LBatch : Q40Cm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_q40_cm2m_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Q40Cm2MBatch : Q40Cm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_q40_cm2s_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Q40Cm2SBatch : Q40Cm2T {
    override BN = 32u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_iq4xs_cm2l_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq4xsCm2LBatch : Iq4xsCm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_iq4xs_cm2m_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq4xsCm2MBatch : Iq4xsCm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_iq4xs_cm2s_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq4xsCm2SBatch : Iq4xsCm2T {
    override BN = 32u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_iq3s4_cm2l_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq3s4Cm2LBatch : Iq3s4Cm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_iq3s4_cm2m_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq3s4Cm2MBatch : Iq3s4Cm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_iq3s4_cm2s_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq3s4Cm2SBatch : Iq3s4Cm2T {
    override BN = 32u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_k3_cm2l_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K3Cm2LBatch : K3Cm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_k3_cm2m_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K3Cm2MBatch : K3Cm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_k3_cm2s_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K3Cm2SBatch : K3Cm2T {
    override BN = 32u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_iq3s_cm2l_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq3sCm2LBatch : Iq3sCm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_iq3s_cm2m_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq3sCm2MBatch : Iq3sCm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_iq3s_cm2s_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq3sCm2SBatch : Iq3sCm2T {
    override BN = 32u
    override STILE = true
    override BK = 32u
    override UNR = 8u
    typedef AT = coopmatWgA_f16_128x32
    typedef BT = coopmatWgB_f16_32x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_iq3xxs_cm2l_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq3xxsCm2LBatch : Iq3xxsCm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_iq3xxs_cm2m_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq3xxsCm2MBatch : Iq3xxsCm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_iq3xxs_cm2s_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq3xxsCm2SBatch : Iq3xxsCm2T {
    override BN = 32u
    override STILE = true
    override BK = 32u
    override UNR = 8u
    typedef AT = coopmatWgA_f16_128x32
    typedef BT = coopmatWgB_f16_32x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_iq4nl_cm2l_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq4nlCm2LBatch : Iq4nlCm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_iq4nl_cm2m_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq4nlCm2MBatch : Iq4nlCm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_iq4nl_cm2s_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq4nlCm2SBatch : Iq4nlCm2T {
    override BN = 32u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_k2_cm2l_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K2Cm2LBatch : K2Cm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_k2_cm2m_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K2Cm2MBatch : K2Cm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_k2_cm2s_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K2Cm2SBatch : K2Cm2T {
    override BN = 32u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_iq2s_cm2l_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq2sCm2LBatch : Iq2sCm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_iq2s_cm2m_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq2sCm2MBatch : Iq2sCm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_iq2s_cm2s_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq2sCm2SBatch : Iq2sCm2T {
    override BN = 32u
    override STILE = true
    override BK = 32u
    override UNR = 8u
    typedef AT = coopmatWgA_f16_128x32
    typedef BT = coopmatWgB_f16_32x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_iq2xs_cm2l_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq2xsCm2LBatch : Iq2xsCm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_iq2xs_cm2m_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq2xsCm2MBatch : Iq2xsCm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_iq2xs_cm2s_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq2xsCm2SBatch : Iq2xsCm2T {
    override BN = 32u
    override STILE = true
    override BK = 32u
    override UNR = 8u
    typedef AT = coopmatWgA_f16_128x32
    typedef BT = coopmatWgB_f16_32x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_iq2xxs_cm2l_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq2xxsCm2LBatch : Iq2xxsCm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_iq2xxs_cm2m_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq2xxsCm2MBatch : Iq2xxsCm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_iq2xxs_cm2s_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq2xxsCm2SBatch : Iq2xxsCm2T {
    override BN = 32u
    override STILE = true
    override BK = 32u
    override UNR = 8u
    typedef AT = coopmatWgA_f16_128x32
    typedef BT = coopmatWgB_f16_32x32
    typedef ACC = coopmatWgAcc_f16_128x32
    typedef ACCW = coopmatWgAcc_f32_128x32

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "mx4_batch_cm2e_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Mx4Cm2EBatch : Mx4Cm2T {
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_iq3s_cm2e_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq3sCm2EBatch : Iq3sCm2T {
    override BN = 128u
    override STILE = true
    override BK = 32u
    override UNR = 8u
    typedef AT = coopmatWgA_f16_128x32
    typedef BT = coopmatWgB_f16_32x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_iq3xxs_cm2e_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq3xxsCm2EBatch : Iq3xxsCm2T {
    override BN = 128u
    override STILE = true
    override BK = 32u
    override UNR = 8u
    typedef AT = coopmatWgA_f16_128x32
    typedef BT = coopmatWgB_f16_32x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_iq2s_cm2e_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq2sCm2EBatch : Iq2sCm2T {
    override BN = 128u
    override STILE = true
    override BK = 32u
    override UNR = 8u
    typedef AT = coopmatWgA_f16_128x32
    typedef BT = coopmatWgB_f16_32x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_iq2xs_cm2e_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq2xsCm2EBatch : Iq2xsCm2T {
    override BN = 128u
    override STILE = true
    override BK = 32u
    override UNR = 8u
    typedef AT = coopmatWgA_f16_128x32
    typedef BT = coopmatWgB_f16_32x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_iq2xxs_cm2e_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq2xxsCm2EBatch : Iq2xxsCm2T {
    override BN = 128u
    override STILE = true
    override BK = 32u
    override UNR = 8u
    typedef AT = coopmatWgA_f16_128x32
    typedef BT = coopmatWgB_f16_32x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

// the KHR arm of every kq format (KHR, BN 128); the cm2 typedefs belong to the template's uncalled
// body and are never materialized
[vk_dispatch(name = "kq_batch_k4_khr_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K4KhrBatch : K4Cm2T {
    override KHR = true
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "q8_batch_khr_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Q8KhrBatch : Q8Cm2T {
    override KHR = true
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}


[vk_dispatch(name = "f32_batch_khr_cls", kernel = "run", grid = "wgs", params = "wgs : int64"), arch(at = "../ARCHITECTURE_GPU_VULKAN_KHR.md#f32-rows-staged-tile")]
class F32KhrBatch : F32RowsKhrT {
    override KHR = true
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "q51_batch_khr_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Q51KhrBatch : Q51Cm2T {
    override KHR = true
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "iq4nl32_batch_khr_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq4nl32KhrBatch : Iq4nl32Cm2T {
    override KHR = true
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "q4032_batch_khr_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Q4032KhrBatch : Q4032Cm2T {
    override KHR = true
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "q5032_batch_khr_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Q5032KhrBatch : Q5032Cm2T {
    override KHR = true
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "mx4_batch_khr_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Mx4KhrBatch : Mx4Cm2T {
    override KHR = true
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_k5_khr_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K5KhrBatch : K5Cm2T {
    override KHR = true
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_k6_khr_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K6KhrBatch : K6Cm2T {
    override KHR = true
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_q40_khr_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Q40KhrBatch : Q40Cm2T {
    override KHR = true
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_iq4xs_khr_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq4xsKhrBatch : Iq4xsCm2T {
    override KHR = true
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_iq3s4_khr_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq3s4KhrBatch : Iq3s4Cm2T {
    override KHR = true
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_k3_khr_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K3KhrBatch : K3Cm2T {
    override KHR = true
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_iq3s_khr_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq3sKhrBatch : Iq3sCm2T {
    override KHR = true
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_iq3xxs_khr_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq3xxsKhrBatch : Iq3xxsCm2T {
    override KHR = true
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_iq4nl_khr_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq4nlKhrBatch : Iq4nlCm2T {
    override KHR = true
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_k2_khr_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class K2KhrBatch : K2Cm2T {
    override KHR = true
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_iq2s_khr_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq2sKhrBatch : Iq2sCm2T {
    override KHR = true
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_iq2xs_khr_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq2xsKhrBatch : Iq2xsCm2T {
    override KHR = true
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "kq_batch_iq2xxs_khr_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class Iq2xxsKhrBatch : Iq2xxsCm2T {
    override KHR = true
    override BN = 128u
    override STILE = true
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f16_128x128
    typedef ACCW = coopmatWgAcc_f32_128x128

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

// ===== the split-k reduce (sums the cm2 partial planes into y) =====

[vk_dispatch(name = "splitk_reduce_cls", grid = "wgs", params = "wgs : int64")]
class SplitKReduce : SplitKReduceT {
    [spirv_kernel(local_size_x = 256, name = "splitk_reduce_cls_spv")]
    def run {
        splitk_reduce_body()
    }
}

//! sums k partial planes of nelem floats into y at ybase, four floats per lane
def splitk_reduce_enc(raw : VkCommandBuffer; var h : VkHaz; var rset : VkDescriptorSet; nelem, k : int64; ybase : int64 = 0l) {
    var pcr = SkRedArgs(nelem = uint(nelem), k = uint(k), ybase = uint(ybase))
    enc_splitk_reduce_cls(raw, h, rset, pcr, splitk_reduce_wgs(nelem))
}

struct F16GemmArgs {
    n : uint        //! K, the row width (elements), a 64-multiple
    d : uint        //! M, the weight rows
    rows : uint     //! N, the positions
    woff : uint     //! the weight rows' element base
    ybase : uint    //! the output's element base: the sum's, or the first partial plane's
    ystride : uint  //! the output row stride (elements) when the GEMM stores whole
    ksplit : uint   //! the k chunk per workgroup; 0 = the whole K, one plane at ybase with ystride
}

//! a small f16 x f16 GEMM on the cm2 128 x 128 tile, y[rows x d] = x[rows x n] w[d x n]^T; a k chunk's partial plane lands at ybase + chunk * rows * d (stride d)
[vk_dispatch(name = "f16_gemm_cm2_cls", grid = "wgs", params = "wgs : int64")]
class F16GemmCm2 {
    @ssbo @binding = 0 w : array<float16>   //! the weight rows; no `weight` role: the E-series projection binds its f16 panel, rewritten every window
    @ssbo @binding = 1 xf16 : array<float16>
    @ssbo @binding = 2 y : array<float>
    @push_constant pa : F16GemmArgs
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x128
    typedef ACC = coopmatWgAcc_f32_128x128

    [spirv_kernel(local_size_x = 256, name = "f16_gemm_cm2_cls_spv"), arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#vk-prefill-moe-block")]
    def run {
        let xtiles = (pa.rows + 127u) / 128u
        let wtiles = (pa.d + 127u) / 128u
        let chunk = gl_WorkGroupID.x / (xtiles * wtiles)
        let rem = gl_WorkGroupID.x % (xtiles * wtiles)
        let t0 = (rem / wtiles) * 128u
        let m0 = (rem % wtiles) * 128u
        let split = pa.ksplit != 0u
        let k0 = split ? (chunk * pa.ksplit) & ~(F16G_BK - 1u) : 0u
        let k1 = split ? min(k0 + pa.ksplit, pa.n) : pa.n
        let ybase = split ? pa.ybase + chunk * pa.rows * pa.d : pa.ybase
        let ystride = split ? pa.d : pa.ystride
        var tv : tensorView2Dt
        tensorViewCreate(tv)
        var tla : tensorLayout2DPad
        tensorLayoutCreate(tla)
        tensorLayoutSetDimension(tla, pa.d, pa.n)
        tensorLayoutSetStride(tla, pa.n, 1u)
        var tlb : tensorLayout2DPad
        tensorLayoutCreate(tlb)
        tensorLayoutSetDimension(tlb, pa.rows, pa.n)
        tensorLayoutSetStride(tlb, pa.n, 1u)
        var tlo : tensorLayout2DPad
        tensorLayoutCreate(tlo)
        tensorLayoutSetDimension(tlo, pa.rows, pa.d)
        tensorLayoutSetStride(tlo, ystride, 1u)
        var a : AT
        var b : BT
        var acc : ACC
        coopmatFill(acc, 0.0)   //! a split chunk past K (a k split rounded up to the step on a short K) stores a zero plane, never a stale accumulator
        var k = k0
        while (k < k1) {
            coopmatLoadTensor(a, w, pa.woff, tla, m0, 128u, k, F16G_BK)
            coopmatLoadTensor(b, xf16, 0u, tlb, t0, 128u, k, F16G_BK, tv)
            acc = coopmatMulAdd(a, b, acc)
            k += F16G_BK
        }
        coopmatStoreTensor(acc, y, ybase, tlo, t0, 128u, m0, 128u, tv)
    }
}

[vk_dispatch(name = "f16_gemm_khr_cls", grid = "wgs", params = "wgs : int64")]
class F16GemmKhr {
    @ssbo @binding = 0 w : array<float16>   //! the weight rows; no `weight` role, as `F16GemmCm2`'s
    @ssbo @binding = 1 xf16 : array<float16>
    @ssbo @binding = 2 y : array<float>
    @push_constant pa : F16GemmArgs
    @workgroup ys : float[4096]   //! the four subgroups' four 16 x 16 fragments, staged for the masked store: a fragment straddling `rows` or `d` lands only its live elements

    [spirv_kernel(local_size_x = 128, name = "f16_gemm_khr_cls_spv"), arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#vk-prefill-moe-block")]
    def run {
        let wtiles = (pa.d + 63u) / 64u
        let tile_row0 = (gl_WorkGroupID.x / wtiles) * 64u
        let tile_col0 = (gl_WorkGroupID.x % wtiles) * 64u
        let sg = gl_LocalInvocationID.x / 32u
        let sg_row0 = tile_row0 + (sg / 2u) * 32u
        let sg_col0 = tile_col0 + (sg % 2u) * 32u
        //! a fragment past `rows` or `d` is not loaded; a fragment straddling them lands only its live elements through the staged store below
        let a1_on = sg_row0 + 16u < pa.rows
        let b1_on = sg_col0 + 16u < pa.d
        var acc : coopmatAcc_f32_16x16[4]
        var k = 0u
        while (k < pa.n) {
            var a0, a1 : coopmatA_f16_16x16
            var b0, b1 : coopmatB_f16_16x16
            if (sg_row0 < pa.rows) {
                coopmatLoad(a0, xf16, sg_row0 * pa.n + k, int(pa.n), 0)
            }
            if (a1_on) {
                coopmatLoad(a1, xf16, (sg_row0 + 16u) * pa.n + k, int(pa.n), 0)
            }
            if (sg_col0 < pa.d) {
                coopmatLoad(b0, w, pa.woff + sg_col0 * pa.n + k, int(pa.n), 1)
            }
            if (b1_on) {
                coopmatLoad(b1, w, pa.woff + (sg_col0 + 16u) * pa.n + k, int(pa.n), 1)
            }
            acc[0] = coopmatMulAdd(a0, b0, acc[0])
            acc[1] = coopmatMulAdd(a0, b1, acc[1])
            acc[2] = coopmatMulAdd(a1, b0, acc[2])
            acc[3] = coopmatMulAdd(a1, b1, acc[3])
            k += 16u
        }
        for [unroll] (f in range(4)) {
            coopmatStore(acc[f], ys, int(sg * 1024u + uint(f) * 256u), 16, 0)
        }
        barrier()
        let lane = gl_LocalInvocationID.x % 32u
        for [unroll] (f in range(4)) {
            let r = sg_row0 + uint(f / 2) * 16u
            let c = sg_col0 + uint(f % 2) * 16u
            let sbase = sg * 1024u + uint(f) * 256u
            var e = lane
            while (e < 256u) {
                let rr = r + e / 16u
                let cc = c + e % 16u
                if (rr < pa.rows && cc < pa.d) {
                    y[pa.ybase + rr * pa.ystride + cc] = ys[sbase + e]
                }
                e += 32u
            }
        }
    }
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#vk-prefill-moe-block")]
def f16_gemm_wgs(khr : bool; rows, d : int64) : int64 {
    let tile = khr ? 64l : 128l
    return ((rows + tile - 1l) / tile) * ((d + tile - 1l) / tile)
}

//! an f16 GEMM off a cm2-mode device takes the KHR tile (`f16_gemm_set`'s `khr`)
def f16_gemm_khr : bool => g_gpu.coopmat_mode != COOPMAT_CM2

//! the f16 GEMM tile serves on a cm2-mode device or where the KHR tile does; elsewhere an f16 GEMM route declines to the f32 tile
def f16_gemm_on : bool => g_gpu.coopmat_mode == COOPMAT_CM2 || khr_kq_tile_on()

def f16_gemm_ensure(khr : bool) : bool => khr ? ensure_f16_gemm_khr_cls() : ensure_f16_gemm_cm2_cls()

def f16_gemm_set(khr : bool; bufs : uint64 const[3]; sizes : int64 const[3]; gbits : uint const[3]) : VkDescriptorSet
    => khr ? set_f16_gemm_khr_cls(bufs, sizes, gbits) : set_f16_gemm_cm2_cls(bufs, sizes, gbits)

[arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#vk-prefill-moe-block")]
def f16_gemm_enc(khr : bool; raw : VkCommandBuffer; var h : VkHaz; var s : VkDescriptorSet; var pc : F16GemmArgs; rows, d : int64) {
    let wgs = f16_gemm_wgs(khr, rows, d)
    if (khr) {
        enc_f16_gemm_khr_cls(raw, h, s, pc, wgs)
    } else {
        enc_f16_gemm_cm2_cls(raw, h, s, pc, wgs)
    }
}

struct F32GemmArgs {
    n : uint        //! K: the columns multiplied, a 32-multiple (the slab pads a row to 32 with zeros)
    d : uint        //! M: the weight rows, the output width
    rows : uint     //! N: the positions
    woff : uint     //! the weight rows' element base in w
    wstride : uint  //! the weight row stride (elements)
    boff : uint     //! the bias row's element base in b (the BIAS stamp)
    xoff : uint     //! the x rows' element base
    xstride : uint  //! the x row stride (elements)
    yoff : uint     //! the y rows' element base
    ystride : uint  //! the y row stride (elements)
}

let F32G_TP = 64u            //! the f32 tile's positions per workgroup
let F32G_TD = 64u            //! ... and weight rows
let F32G_BK = 32u            //! the K step staged per pass
let F32G_LD = F32G_BK + 1u   //! the stage row stride: the step plus one of pad, so a column read spreads the banks

//! y[rows x d] = x[rows x n] w[d x n]^T in f32 with every product carried by one mad in k order - the exact
//! feed the TTS front end needs (its outputs round to integer durations, its F0 drives a phase): no
//! coopmat, no f16 staging, workgroup tiles of 64 positions x 64 rows, each invocation four positions by
//! four rows; the BIAS stamp adds the bias row at the store
[ |> template_struct_instance]
class template F32GemmT {
    @template_constant BIAS : bool = false
    @ssbo @binding = 0 @role = "weight" w : array<float>
    @ssbo @binding = 1 x : array<float>
    @ssbo @binding = 2 y : array<float>
    @ssbo @binding = 3 @role = "weight" @template_gate = BIAS b : array<float>
    @push_constant pa : F32GemmArgs
    @workgroup xs : float[int(F32G_TP * F32G_LD)]
    @workgroup ws : float[int(F32G_TD * F32G_LD)]

    //! stage slot `s` (row s / BK, k s % BK) of the x rows from `p0` at step `kb`; zero past the rows or past n
    def stage_x(p0, kb, s : uint) {
        let row = s / F32G_BK
        let kk = s % F32G_BK
        var v = 0.0
        if (p0 + row < pa.rows && kb + kk < pa.n) {
            v = x[pa.xoff + (p0 + row) * pa.xstride + kb + kk]
        }
        xs[row * F32G_LD + kk] = v
    }

    //! ... and of the weight rows from `o0`; zero past d or past n
    def stage_w(o0, kb, s : uint) {
        let row = s / F32G_BK
        let kk = s % F32G_BK
        var v = 0.0
        if (o0 + row < pa.d && kb + kk < pa.n) {
            v = w[pa.woff + (o0 + row) * pa.wstride + kb + kk]
        }
        ws[row * F32G_LD + kk] = v
    }

    //! the stage of one 32-wide K step: 64 x rows and 64 w rows, eight slots of each per invocation
    def stage(p0, o0, kb, tid : uint) {
        var s = tid
        while (s < F32G_TP * F32G_BK) {
            stage_x(p0, kb, s)
            stage_w(o0, kb, s)
            s += 256u
        }
    }

    //! one output, the bias row added on the BIAS stamp; a position or a row past the extent stores nothing
    def store(p, o : uint; v : float) {
        if (p < pa.rows && o < pa.d) {
            var out = v
            static_if (BIAS) {
                out += b[pa.boff + o]
            }
            y[pa.yoff + p * pa.ystride + o] = out
        }
    }

    //! position p's four rows from o
    def store4(p, o : uint; v0, v1, v2, v3 : float) {
        store(p, o, v0)
        store(p, o + 1u, v1)
        store(p, o + 2u, v2)
        store(p, o + 3u, v3)
    }

    [spirv_kernel(local_size_x = 256)]
    def run {
        let tid = gl_LocalInvocationID.x
        let dgroups = max((pa.d + F32G_TD - 1u) / F32G_TD, 1u)
        let p0 = (gl_WorkGroupID.x / dgroups) * F32G_TP
        let o0 = (gl_WorkGroupID.x % dgroups) * F32G_TD
        let tp = (tid / 16u) * 4u
        let to = (tid % 16u) * 4u
        var a00 = 0.0
        var a01 = 0.0
        var a02 = 0.0
        var a03 = 0.0
        var a10 = 0.0
        var a11 = 0.0
        var a12 = 0.0
        var a13 = 0.0
        var a20 = 0.0
        var a21 = 0.0
        var a22 = 0.0
        var a23 = 0.0
        var a30 = 0.0
        var a31 = 0.0
        var a32 = 0.0
        var a33 = 0.0
        var kb = 0u
        while (kb < pa.n) {
            stage(p0, o0, kb, tid)
            barrier()
            var kk = 0u
            while (kk < F32G_BK) {
                let x0 = xs[tp * F32G_LD + kk]
                let x1 = xs[(tp + 1u) * F32G_LD + kk]
                let x2 = xs[(tp + 2u) * F32G_LD + kk]
                let x3 = xs[(tp + 3u) * F32G_LD + kk]
                let w0 = ws[to * F32G_LD + kk]
                let w1 = ws[(to + 1u) * F32G_LD + kk]
                let w2 = ws[(to + 2u) * F32G_LD + kk]
                let w3 = ws[(to + 3u) * F32G_LD + kk]
                a00 = mad(x0, w0, a00)
                a01 = mad(x0, w1, a01)
                a02 = mad(x0, w2, a02)
                a03 = mad(x0, w3, a03)
                a10 = mad(x1, w0, a10)
                a11 = mad(x1, w1, a11)
                a12 = mad(x1, w2, a12)
                a13 = mad(x1, w3, a13)
                a20 = mad(x2, w0, a20)
                a21 = mad(x2, w1, a21)
                a22 = mad(x2, w2, a22)
                a23 = mad(x2, w3, a23)
                a30 = mad(x3, w0, a30)
                a31 = mad(x3, w1, a31)
                a32 = mad(x3, w2, a32)
                a33 = mad(x3, w3, a33)
                kk++
            }
            barrier()
            kb += F32G_BK
        }
        let p = p0 + tp
        let o = o0 + to
        store4(p, o, a00, a01, a02, a03)
        store4(p + 1u, o, a10, a11, a12, a13)
        store4(p + 2u, o, a20, a21, a22, a23)
        store4(p + 3u, o, a30, a31, a32, a33)
    }
}

[vk_dispatch(name = "f32_gemm_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class F32Gemm : F32GemmT {}

[vk_dispatch(name = "f32_gemm_b_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class F32GemmB : F32GemmT {
    override BIAS = true
}

//! the f32 tile's workgroup count over rows x d
def f32_gemm_wgs(rows, d : int64) : int64 => ((rows + int64(F32G_TP) - 1l) / int64(F32G_TP)) * ((d + int64(F32G_TD) - 1l) / int64(F32G_TD))

//! the PL-BERT embedding sum, the text encoder's word rows and the nearest-x2 upsample on the row gather both homes stamp
[vk_dispatch(name = "tts_row_gather_cls", kernel = "run", grid = "total/256", params = "total : int64")]
class TtsRowGather : GkRowGather {
    [spirv_kernel(local_size_x = 256, name = "tts_row_gather_cls_spv")]
    def run {
        row_gather_body()
    }
}

struct TtsElemArgs {
    nelem : uint   //! the elements mapped
    off : uint     //! their element base in x
    slope : float  //! the leaky stamp's negative slope
}

//! an in-place elementwise map of x[off .. off + nelem), four elements an invocation; a stamp supplies `act`
[ |> template_struct_instance]
class template TtsElemT {
    @ssbo @binding = 0 x : array<float>
    @push_constant pa : TtsElemArgs

    def abstract act(v : float) : float   //! the map of one element

    [spirv_kernel(local_size_x = 256)]
    def run {
        let e = gl_GlobalInvocationID.x * 4u
        let b = pa.off + e
        if (e + 4u <= pa.nelem) {
            let v0 = act(x[b])
            let v1 = act(x[b + 1u])
            let v2 = act(x[b + 2u])
            let v3 = act(x[b + 3u])
            x[b] = v0
            x[b + 1u] = v1
            x[b + 2u] = v2
            x[b + 3u] = v3
        } else {
            var i = e
            while (i < pa.nelem) {
                let vi = act(x[pa.off + i])
                x[pa.off + i] = vi
                i++
            }
        }
    }
}

//! the CPU `gelu`'s tanh form in f32 (not the f16 LUT form `act_f32` spells - its roundings would move the front end's
//! durations); the clamp keeps a device tanh off its exp overflow and changes no f32 result
[vk_dispatch(name = "tts_gelu_tanh_cls", kernel = "run", grid = "nelem/1024", params = "nelem : int64")]
class TtsGeluTanh : TtsElemT {
    def override act(v : float) : float => tts_gelu_tanh(v)
}

//! the CPU `leaky_relu`: v > 0 ? v : v * slope
[vk_dispatch(name = "tts_leaky_cls", kernel = "run", grid = "nelem/1024", params = "nelem : int64")]
class TtsLeaky : TtsElemT {
    def override act(v : float) : float => v > 0.0 ? v : v * pa.slope
}

struct TtsAttnArgs {
    t : uint       //! the rows, queries and keys alike
    d : uint       //! the planes' row stride
    hs : uint      //! the head size
    heads : uint
    scale : float  //! the score scale, 1 / sqrt(hs)
    qoff : uint    //! each plane's element base
    koff : uint
    voff : uint
    ooff : uint
}

let TTS_ATTN_TCAP = 512u   //! the staged score row's capacity

//! bidirectional multi-head attention in f32 over q, k, v [t][d] (head h at columns h * hs) into out - the CPU
//! `attention_rows` per (query row, head) workgroup: every key's scaled dot, the softmax as max, exp clamped at -80, sum,
//! then the value sum of the normalized weights, 256 / hs invocations a value column. The driver dispatches it only at
//! t <= TTS_ATTN_TCAP (the staged score row) and hs 64 or 128 (the value split's lattice); the class declines nothing
[vk_dispatch(name = "tts_attn_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TtsAttn : WgReduceBase {
    @ssbo @binding = 0 q : array<float>
    @ssbo @binding = 1 k : array<float>
    @ssbo @binding = 2 v : array<float>
    @ssbo @binding = 3 out : array<float>
    @push_constant pa : TtsAttnArgs
    @workgroup sc : float[int(TTS_ATTN_TCAP)]   //! the row's scores, then its exp weights
    @workgroup vpart : float[256]                //! the value sums' partials, one an invocation

    //! the scaled dot of the query row at `qb` and the key row at `kb`, one mad a column in column order
    def score(qb, kb : uint) : float {
        var acc = 0.0
        var e = 0u
        while (e < pa.hs) {
            acc = mad(q[qb + e], k[kb + e], acc)
            e++
        }
        return acc * pa.scale
    }

    [spirv_kernel(local_size_x = 256)]
    def run {
        let tid = gl_LocalInvocationID.x
        let t = max(pa.t, 1u)
        let hs = max(pa.hs, 1u)
        let h = gl_WorkGroupID.x / t
        let i = gl_WorkGroupID.x % t
        let col = h * pa.hs
        let qb = pa.qoff + i * pa.d + col
        var m0 = -3.0e38
        var j = tid
        while (j < pa.t) {
            let s = score(qb, pa.koff + j * pa.d + col)
            sc[j] = s
            m0 = max(m0, s)
            j += 256u
        }
        let m = wg_max(m0, 0u)
        var l0 = 0.0
        j = tid
        while (j < pa.t) {
            let p = exp(max(sc[j] - m, -80.0))
            sc[j] = p
            l0 += p
            j += 256u
        }
        let inv = 1.0 / wg_sum(l0, 1u)
        let parts = 256u / hs
        let vb = pa.voff + col + tid % hs
        var acc = 0.0
        j = tid / hs
        while (j < pa.t) {
            acc = mad(sc[j] * inv, v[vb + j * pa.d], acc)
            j += parts
        }
        vpart[tid] = acc
        barrier()
        if (tid < pa.hs) {
            var o = vpart[tid]
            var pp = 1u
            while (pp < parts) {
                o += vpart[pp * pa.hs + tid]
                pp++
            }
            out[pa.ooff + i * pa.d + col + tid] = o
        }
    }
}

//! the attention's workgroup count: one a (query row, head)
def tts_attn_wgs(t, heads : int64) : int64 => t * heads

// ===== rope + KV store + qk-norm =====

//! the per-head q/k RMSNorm on the template both homes stamp (`GkQkRms`), one workgroup a head row over the rows: workgroup
//! wid is group wid % per of row wid / per
[vk_dispatch(name = "qk_rms_cls", grid = "wgs", params = "wgs : int64")]
class QkRms : GkQkRms {
    [spirv_kernel(local_size_x = 256, name = "qk_rms_cls_spv")]
    def run {
        let vgroups = (pa.vnorm != 0u && pa.vfk == 0u) ? pa.nkvh : 0u
        let per = max(pa.nh + pa.nkvh + vgroups, 1u)
        qk_rms_group(gl_WorkGroupID.x / per, gl_WorkGroupID.x % per)
    }
}

//! rows clamped in place (a gemma4v ClippableLinear's output bounds), on the template both homes stamp
[vk_dispatch(name = "tower_clamp_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TowerClamp : GkClampRows {
    [spirv_kernel(local_size_x = 256, name = "tower_clamp_cls_spv")]
    def run {
        clamp_rows_body()
    }
}

//! the f16 feed of a clamped GEMM input: the input bounds applied on the way into the tile so the normed rows stay whole for
//! the next GEMM's own bounds (at the half range's bounds, `f16_rows_args`: the plain f32 -> f16 rows)
[vk_dispatch(name = "tower_clamp_cvt_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TowerClampCvt : GkClampRows {
    override OUT16 = true

    [spirv_kernel(local_size_x = 256, name = "tower_clamp_cvt_cls_spv")]
    def run {
        clamp_rows_body()
    }
}

//! bf16 rows widened by their bit shift and stored f16 (saturated at the half range), on the template both homes stamp
//! (`GkClampRows`): the E-series projection's per-window f16 panel
[vk_dispatch(name = "ple_bf16_cvt_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class PleBf16Cvt : GkClampRows {
    override OUT16 = true
    override BF16IN = true

    [spirv_kernel(local_size_x = 256, name = "ple_bf16_cvt_cls_spv")]
    def run {
        clamp_rows_body()
    }
}

//! the f32 copy of a clamped GEMM input (the exact lane's f32 tile): the input bounds applied into `dst` so the normed rows stay
//! whole for the next GEMM's own bounds
[vk_dispatch(name = "tower_clamp_copy_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TowerClampCopy : GkClampRows {
    override COPY = true

    [spirv_kernel(local_size_x = 256, name = "tower_clamp_copy_cls_spv")]
    def run {
        clamp_rows_body()
    }
}

//! the Q8_0 feed of a clamped GEMM input: the requant over the clamped elements (the mm and KHR modes' tile feed)
[vk_dispatch(name = "tower_clamp_rq_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TowerClampRq : RqPlainT {
    override CLAMP = true

    [spirv_kernel(local_size_x = 256)]
    def run {
        rq_body()
    }
}

//! the conv2d im2col over a window's chunks as the f16 GEMM's feed, on the template both homes stamp
[vk_dispatch(name = "tower_im2col_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TowerIm2col : GkIm2col2d {
    typedef OT = float16

    [spirv_kernel(local_size_x = 256, name = "tower_im2col_cls_spv")]
    def run {
        im2col2d_body()
    }
}

//! the qwen3a feature rows as the f16 tile's feed, on the template both homes stamp
[vk_dispatch(name = "tower_q3a_shuffle_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TowerQ3aShuffle : GkQ3aShuffle {
    typedef OT = float16

    [spirv_kernel(local_size_x = 256, name = "tower_q3a_shuffle_cls_spv")]
    def run {
        q3a_shuffle_body()
    }
}

struct TowerSpecDftArgs {
    n_fft : uint    //! the window's taps
    n_frames : uint
    mag : uint      //! 1 = the magnitude (gemma4a), 0 = the power (the whisper preprocessors)
}

//! the spectrum of the windowed frames: one thread per (frame, bin) sums the frame's taps against the bin's
//! (cos, sin) twiddle pair - the CPU DFT GEMM's rows [k][2 bin], [k][2 bin + 1] - and stores re^2 + im^2 or its root
[vk_dispatch(name = "tower_spec_dft_cls", grid = "wgs", params = "wgs : int64")]
class TowerSpecDft {
    @ssbo @binding = 0 xw : array<float>
    @ssbo @binding = 1 tw : array<float>
    @ssbo @binding = 2 pw : array<float>
    @push_constant pa : TowerSpecDftArgs

    [spirv_kernel(local_size_x = 256, name = "tower_spec_dft_cls_spv")]
    def run {
        let gid = gl_GlobalInvocationID.x
        let nb = pa.n_fft / 2u + 1u
        if (gid < pa.n_frames * nb) {
            let frame = gid / nb
            let b = gid % nb
            let xb = frame * pa.n_fft
            let stride = 2u * nb
            var re = 0.0
            var im = 0.0
            for (k in urange(pa.n_fft)) {
                let x = xw[xb + k]
                re += x * tw[k * stride + 2u * b]
                im += x * tw[k * stride + 2u * b + 1u]
            }
            let p = re * re + im * im
            pw[gid] = pa.mag != 0u ? sqrt(p) : p
        }
    }
}

struct TowerMelLogArgs {
    n_bins : uint
    n_mel : uint
    n_frames : uint
    floor : float
    scale : float        //! the log's scale: log10 e for a log10 store, 1 for the natural log
    frame_major : uint   //! 1 = rows [frame][mel] (gemma4a, canary), 0 = [mel][frame] (the whisper preprocessors)
    add_floor : uint     //! 1 = log(sum + floor) (canary), 0 = log(max(sum, floor))
}

//! the log filterbank: one thread per (mel row, frame) sums the frame's spectrum against the row's filter; the thread
//! order is the store's, so either layout writes coalesced
[vk_dispatch(name = "tower_mel_log_cls", grid = "wgs", params = "wgs : int64")]
class TowerMelLog {
    @ssbo @binding = 0 pw : array<float>
    @ssbo @binding = 1 fil : array<float>
    @ssbo @binding = 2 outm : array<float>
    @push_constant pa : TowerMelLogArgs

    [spirv_kernel(local_size_x = 256, name = "tower_mel_log_cls_spv")]
    def run {
        let gid = gl_GlobalInvocationID.x
        if (gid < pa.n_mel * pa.n_frames) {
            let nl = max(1u, pa.n_frames)
            let nm = max(1u, pa.n_mel)
            let j = pa.frame_major != 0u ? gid % nm : gid / nl
            let frame = pa.frame_major != 0u ? gid / nm : gid % nl
            let pb = frame * pa.n_bins
            let fb = j * pa.n_bins
            var sum = 0.0
            for (k in urange(pa.n_bins)) {
                sum += pw[pb + k] * fil[fb + k]
            }
            outm[gid] = log(pa.add_floor != 0u ? sum + pa.floor : max(sum, pa.floor)) * pa.scale
        }
    }
}

//! canary's per-feature mel normalization, on the template both homes stamp
[vk_dispatch(name = "tower_mel_norm_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TowerMelNorm : GkMelNorm {
    [spirv_kernel(local_size_x = 64, name = "tower_mel_norm_cls_spv")]
    def run {
        mel_norm_body()
    }
}

//! canary's subsample depthwise conv2d k3 s2 p1 with its bias, on the template both homes stamp
[vk_dispatch(name = "tower_dw_conv_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TowerDwConv : GkDwConv2d {
    [spirv_kernel(local_size_x = 256, name = "tower_dw_conv_cls_spv")]
    def run {
        dw_conv2d_body()
    }
}

//! parakeet's subsample depthwise conv2d: the same stamp over tap-major taps
[vk_dispatch(name = "tower_pk_dw2d_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TowerPkDwConv2d : GkDwConv2d {
    override TAP_MAJOR = true

    [spirv_kernel(local_size_x = 256, name = "tower_pk_dw2d_cls_spv")]
    def run {
        dw_conv2d_body()
    }
}

//! the Transformer-XL rel-position table, on the template both homes stamp
[vk_dispatch(name = "tower_rel_sincos_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TowerRelSinCos : GkRelSinCos {
    [spirv_kernel(local_size_x = 256, name = "tower_rel_sincos_cls_spv")]
    def run {
        rel_sincos_body()
    }
}

//! the subsample stack's feature rows, on the template both homes stamp
[vk_dispatch(name = "tower_feat_shuffle_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TowerFeatShuffle : GkFeatShuffle {
    [spirv_kernel(local_size_x = 256, name = "tower_feat_shuffle_cls_spv")]
    def run {
        feat_shuffle_body()
    }
}

//! the whisper decoder's K/V stores, on the template both homes stamp (the modes by push)
[vk_dispatch(name = "tower_wdec_kv_cls", grid = "wgs", params = "wgs : int64"), arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-asr-decoder")]
class TowerWdecKv : GkWdecKv {
    [spirv_kernel(local_size_x = 256, name = "tower_wdec_kv_cls_spv")]
    def run {
        wdec_kv_body()
    }
}

//! ... the resident f16 plane
[vk_dispatch(name = "tower_wdec_kv16_cls", grid = "wgs", params = "wgs : int64")]
class TowerWdecKv16 : GkWdecKv {
    override OUT16 = true

    [spirv_kernel(local_size_x = 256, name = "tower_wdec_kv16_cls_spv")]
    def run {
        wdec_kv_body()
    }
}

let WDEC_CHUNK = 256u   //! the keys a partial's workgroup covers (`TowerWdecAttnPart`) and the extent of its `scores` array
let WDEC_HS = 64u       //! the whisper decoder's head width, every size of the family; the driver declines another
let Q3A_TOK_PER_CHUNK = 13u   //! the qwen3a front's positions a chunk (the family's QWEN3A_TOK_PER_CHUNK; the driver verifies the two agree)

//! the whisper decoder's chunked attention partials, on the template both homes stamp
[vk_dispatch(name = "tower_wdec_attn_part_cls", kernel = "run", grid = "wgs", params = "wgs : int64"), arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-asr-decoder")]
class TowerWdecAttnPart : GkWdecAttnPart {
    override HS = 64u

    [spirv_kernel(local_size_x = 128, name = "tower_wdec_attn_part_cls_spv")]
    def run {
        wdec_attn_part_body()
    }
}

//! the whisper decoder's attention combine, the f32 store, on the template both homes stamp (GkWdecAttnComb)
[vk_dispatch(name = "tower_wdec_attn_comb_cls", kernel = "run", grid = "wgs", params = "wgs : int64"), arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-asr-decoder")]
class TowerWdecAttnComb : GkWdecAttnComb {
    override HS = 64u

    [spirv_kernel(local_size_x = 64, name = "tower_wdec_attn_comb_cls_spv")]
    def run {
        wdec_attn_comb_body()
    }
}

[vk_dispatch(name = "tower_wdec_attn_comb_rq_cls", kernel = "run", grid = "wgs", params = "wgs : int64"), arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-asr-decoder")]
class TowerWdecAttnCombRq : GkWdecAttnComb {
    override HS = 64u
    override OUT_Q8 = true

    [spirv_kernel(local_size_x = 64, name = "tower_wdec_attn_comb_rq_cls_spv")]
    def run {
        wdec_attn_comb_body()
    }
}

//! the subsample convs' LayerNorm then ReLU, on the template both homes stamp - in place here, the rows bound to x and y both
[vk_dispatch(name = "tower_ln_relu_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TowerLnRelu : GkLnReluRows {
    [spirv_kernel(local_size_x = 256, name = "tower_ln_relu_cls_spv")]
    def run {
        ln_relu_rows_body()
    }
}

//! the 2D neox rope over q or k rows in place, on the template both homes stamp
[vk_dispatch(name = "tower_rope2d_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TowerRope2d : GkRope2d {
    [spirv_kernel(local_size_x = 256, name = "tower_rope2d_cls_spv")]
    def run {
        rope2d_body()
    }
}

//! gemma4a's causal sliding-window attention with the projected RPE table, on the template both homes stamp, at the two head widths
[vk_dispatch(name = "tower_g4a_attn_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TowerG4aAttn : GkG4aAttn {
    override HS = 128u

    [spirv_kernel(local_size_x = 64)]
    def run {
        g4a_attn_body()
    }
}

//! gemma4a's windowed attention at a head of 64, on the template both homes stamp (GkG4aAttn)
[vk_dispatch(name = "tower_g4a_attn64_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TowerG4aAttn64 : GkG4aAttn {
    override HS = 64u

    [spirv_kernel(local_size_x = 64)]
    def run {
        g4a_attn_body()
    }
}

//! the conv module's GLU off the pointwise rows, on the gated-rows template both homes stamp
[vk_dispatch(name = "tower_glu_sig_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TowerGluSig : GkGatedRows {
    [spirv_kernel(local_size_x = 256, name = "tower_glu_sig_cls_spv")]
    def run {
        gated_rows_body()
    }
}

//! the ultravox projector's gate, silu(second half) . first half, on the same template at FORM 1
[vk_dispatch(name = "tower_swiglu_swapped_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TowerSwigluSwapped : GkGatedRows {
    override FORM = 1

    [spirv_kernel(local_size_x = 256, name = "tower_swiglu_swapped_cls_spv")]
    def run {
        gated_rows_body()
    }
}

//! the chat projector tails' pair pool ahead of the post-norm (qwen2a, voxtral), on the template both homes stamp
[vk_dispatch(name = "tower_pool_rows_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TowerPoolRows : GkPoolRows {
    [spirv_kernel(local_size_x = 256, name = "tower_pool_rows_cls_spv")]
    def run {
        pool_rows_body()
    }
}

//! gemma4a's causal depthwise conv, k = 5: the chunk's first rows read nothing before the start
[vk_dispatch(name = "tower_dw_conv5_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TowerDwConv5 : GkDwConvRows {
    override FORM = 1
    override K = 5u

    [spirv_kernel(local_size_x = 256)]
    def run {
        dw_conv_rows_body()
    }
}

struct TowerCnAttnRArgs {
    d : uint        //! the row width (compact [npos x d] rows)
    npos : uint
    buoff : uint    //! the content bias row (bias_u) in wn, [d]
    hoff : uint     //! the head's element base in a row
}

[ |> template_struct_instance]
class template TowerCnAttnRT {
    @template_constant HS : uint = 128u   //! the head width served (the accumulators are sized for 128)
    @ssbo @binding = 0 q : array<float>
    @ssbo @binding = 1 k : array<float>
    @ssbo @binding = 2 v : array<float>
    @ssbo @binding = 3 ao : array<float>
    @ssbo @binding = 4 rmat : array<float>
    @ssbo @binding = 5 @role = "weight" wn : array<float>
    @push_constant pa : TowerCnAttnRArgs
    @workgroup ks : float[4096]   //! 32 keys x 128
    @workgroup vs : float[4096]

    [spirv_kernel(local_size_x = 64), arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
    def run {
        let lid = gl_LocalInvocationID.x
        let i = gl_WorkGroupID.x * 64u + lid
        let npos = max(1u, pa.npos)
        let live = i < pa.npos
        let ii = min(i, npos - 1u)
        let d = pa.d
        let hoff = pa.hoff
        let hs = HS
        var qu, acc : float[128]
        var n = 0u
        while (n < hs) {
            qu[n] = q[ii * d + hoff + n] + wn[pa.buoff + hoff + n]
            acc[n] = 0.0
            n++
        }
        var m = -1.5e38
        var den = 0.0
        let base = npos - 1u - ii
        let rrow = ii * (2u * npos - 1u)   //! the rel plane's row width
        var j0 = 0u
        while (j0 < pa.npos) {
            barrier()
            var e = lid
            while (e < 32u * hs) {
                let jj = e / hs
                let row = j0 + jj
                let ok = row < pa.npos
                let src = row * d + hoff + e % hs
                ks[e] = ok ? k[src] : 0.0
                vs[e] = ok ? v[src] : 0.0
                e += 64u
            }
            barrier()
            if (live) {
                let jn = min(32u, pa.npos - j0)   //! the last chunk's live keys
                var jj = 0u
                while (jj < jn) {
                    var cs = 0.0
                    n = 0u
                    while (n < hs) {
                        cs += qu[n] * ks[jj * hs + n]
                        n++
                    }
                    let s = (cs + rmat[rrow + base + j0 + jj]) * (1.0 / sqrt(float(HS)))
                    if (s > m) {
                        let r = exp(m - s)
                        den *= r
                        n = 0u
                        while (n < hs) {
                            acc[n] *= r
                            n++
                        }
                        m = s
                    }
                    let w = exp(s - m)
                    den += w
                    n = 0u
                    while (n < hs) {
                        acc[n] += w * vs[jj * hs + n]
                        n++
                    }
                    jj++
                }
            }
            j0 += 32u
        }
        if (live) {
            let inv = 1.0 / den
            n = 0u
            while (n < hs) {
                ao[i * d + hoff + n] = acc[n] * inv
                n++
            }
        }
    }
}

[vk_dispatch(name = "tower_cn_attn_r_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TowerCnAttnR : TowerCnAttnRT {}

[vk_dispatch(name = "tower_cn_attn_r64_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TowerCnAttnR64 : TowerCnAttnRT {
    override HS = 64u
}

//! canary's conv module tail: the centered depthwise conv over time (k taps, odd - canary 9), the folded BatchNorm and
//! silu in one pass, the CPU loop's form
[vk_dispatch(name = "tower_cn_dw_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TowerCnDw : GkDwConvRows {
    override FORM = 2

    [spirv_kernel(local_size_x = 256)]
    def run {
        dw_conv_rows_body()
    }
}

//! parakeet's conv module tail: the same centered conv with the folded BatchNorm over tap-major taps
[vk_dispatch(name = "tower_pk_dw_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TowerPkDw : GkDwConvRows {
    override FORM = 2
    override TAP_MAJOR = true

    [spirv_kernel(local_size_x = 256)]
    def run {
        dw_conv_rows_body()
    }
}

//! the vision stems' and tails' row kernels on the templates both homes stamp
[vk_dispatch(name = "tower_patch_im2col_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TowerPatchIm2col : GkPatchIm2col {
    [spirv_kernel(local_size_x = 256, name = "tower_patch_im2col_cls_spv")]
    def run {
        patch_im2col_body()
    }
}

[vk_dispatch(name = "tower_pos_add_2d_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TowerPosAdd2d : GkPosAdd2D {
    [spirv_kernel(local_size_x = 256, name = "tower_pos_add_2d_cls_spv")]
    def run {
        pos_add_2d_body()
    }
}

[vk_dispatch(name = "tower_stem_asm_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TowerStemAsm : GkStemAsm {
    [spirv_kernel(local_size_x = 256, name = "tower_stem_asm_cls_spv")]
    def run {
        stem_asm_body()
    }
}

[vk_dispatch(name = "tower_pool_2d_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TowerPool2d : GkPool2d {
    [spirv_kernel(local_size_x = 256, name = "tower_pool_2d_cls_spv")]
    def run {
        pool_2d_body()
    }
}

[vk_dispatch(name = "tower_affine_rows_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TowerAffineRows : GkAffineRows {
    [spirv_kernel(local_size_x = 256, name = "tower_affine_rows_cls_spv")]
    def run {
        affine_rows_body()
    }
}

//! GEGLU-quick with the gemma4v bounds on the template both homes stamp
[vk_dispatch(name = "tower_geglu_cls", grid = "wgs", params = "wgs : int64")]
class TowerGeglu : GkGeglu {
    [spirv_kernel(local_size_x = 256, name = "tower_geglu_cls_spv")]
    def run {
        geglu_body()
    }
}

//! the Vulkan shell of the residual seam and layernorm family both homes stamp
[ |> template_struct_instance]
class template TowerPostAddLnT : GkPostAddLn {
    [spirv_kernel(local_size_x = 256)]
    def run {
        post_add_ln_body()
    }
}

[vk_dispatch(name = "tower_post_add_ln_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TowerPostAddLn : TowerPostAddLnT {}

//! the towers' rms rows alone: xb = rms(x) . w over the weight row at lnoff, a workgroup a row (the ADD-off RMS stamp)
[vk_dispatch(name = "tower_rms_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TowerRms : TowerPostAddLnT {
    override ADD = false
    override RMS = true
}

//! the qwen25v hidden: the gate and up rows with their biases through the silu gate, as halves for the f16 tile
[vk_dispatch(name = "tower_bias_gate16_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TowerBiasGate16 : GkBiasGate {
    override OUT16 = true

    [spirv_kernel(local_size_x = 256, name = "tower_bias_gate16_cls_spv")]
    def run {
        bias_gate_body()
    }
}

//! the seam of a block whose GEMMs carry no bias row (parakeet): the branch added bare, the next layernorm behind it
[vk_dispatch(name = "tower_post_add_ln_nb_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TowerPostAddLnNb : TowerPostAddLnT {
    override BIAS = false
}

[vk_dispatch(name = "tower_post_add_ln16_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TowerPostAddLn16 : TowerPostAddLnT {
    override OUT16 = true
    override OUT32 = false
}

[vk_dispatch(name = "tower_post_add_ln_rq_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TowerPostAddLnRq : TowerPostAddLnT {
    override OUT_Q8 = true
    override OUT32 = false
}

[vk_dispatch(name = "tower_post_add_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TowerPostAdd : TowerPostAddLnT {
    override RMS = true
    override POST_NORM = true
    override BIAS = false
}

[vk_dispatch(name = "tower_post_add_plain_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TowerPostAddPlain : TowerPostAddLnT {
    override RMS = true
    override BIAS = false
}

[vk_dispatch(name = "tower_post_add_rms_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TowerPostAddRms : TowerPostAddLnT {
    override RMS = true
}

//! layernorm rows (`ln_on` set) against the weight and bias rows at `lnoff` / `lnboff`: xb[r] = (x[r] - mean) . inv . w + b, the CPU
//! `layernorm`'s form with its mean and variance as workgroup sums - one workgroup a row; the Rq stamp quantizes the normed row
//! into Q8_0 blocks (`outq`, `outs`) in place of storing it, so the value never lands in f32
[vk_dispatch(name = "tower_ln_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TowerLn : TowerPostAddLnT {
    override ADD = false
}

[vk_dispatch(name = "tower_ln16_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TowerLn16 : TowerPostAddLnT {
    override ADD = false
    override OUT16 = true
    override OUT32 = false
}

[vk_dispatch(name = "tower_ln_rq_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TowerLnRq : TowerPostAddLnT {
    override ADD = false
    override OUT_Q8 = true
    override OUT32 = false
}


struct TowerBiasRqArgs {
    d : uint        //! the row width, a 32-multiple
    nblk : uint     //! the plane's Q8_0 blocks
    boff : uint     //! the bias row's base in wn
}

//! the bias + tanh GELU of a plane and its Q8_0 requant in one dispatch (`RqT`'s grid: eight lanes a block), the source read alone -
//! the whisper decoder's fc1 epilogue, whose activation the architecture fixes
[vk_dispatch(name = "tower_bias_act_rq_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TowerBiasActRq : RqT {
    @ssbo @binding = 0 x : array<float>
    @ssbo @binding = 1 @role = "weight" wn : array<float>
    @ssbo @binding = 2 outq : array<int8>
    @ssbo @binding = 3 outs : array<float>
    @push_constant pa : TowerBiasRqArgs

    def override load1(e : uint) : float => gelu_tanh_lut_f32(x[e] + wn[pa.boff + e % max(1u, pa.d)])

    [spirv_kernel(local_size_x = 256)]
    def run {
        rq_body()
    }
}

[vk_dispatch(name = "tower_bias_act_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TowerBiasAct : GkBiasAct {
    [spirv_kernel(local_size_x = 256, name = "tower_bias_act_cls_spv")]
    def run {
        bias_act_body()
    }
}

[vk_dispatch(name = "tower_bias_act_cvt16_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TowerBiasActCvt16 : GkBiasAct {
    override OUT16 = true

    [spirv_kernel(local_size_x = 256, name = "tower_bias_act_cvt16_cls_spv")]
    def run {
        bias_act_body()
    }
}

[vk_dispatch(name = "tower_act_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TowerAct : GkBiasAct {
    override BIAS = false

    [spirv_kernel(local_size_x = 256, name = "tower_act_cls_spv")]
    def run {
        bias_act_body()
    }
}

[vk_dispatch(name = "tower_hpad_cls", kernel = "run", grid = "wgs", params = "wgs : int64"), arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-routes")]
class TowerHeadPad : GkRestride {
    [spirv_kernel(local_size_x = 256)]
    def run {
        restride_body()
    }
}

[vk_dispatch(name = "tower_hpad16_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TowerHeadPad16 : GkRestride {
    override OUT16 = true

    [spirv_kernel(local_size_x = 256)]
    def run {
        restride_body()
    }
}

[vk_dispatch(name = "tower_hpad_b_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TowerHeadPadB : GkRestride {
    override BIAS = true

    [spirv_kernel(local_size_x = 256)]
    def run {
        restride_body()
    }
}

[vk_dispatch(name = "tower_hpad16_b_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TowerHeadPad16B : GkRestride {
    override OUT16 = true
    override BIAS = true

    [spirv_kernel(local_size_x = 256)]
    def run {
        restride_body()
    }
}

[vk_dispatch(name = "tower_hunpad_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TowerHeadUnpad : GkRestride {
    override PAD = false

    [spirv_kernel(local_size_x = 256)]
    def run {
        restride_body()
    }
}

[vk_dispatch(name = "tower_hunpad16_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TowerHeadUnpad16 : GkRestride {
    override PAD = false
    override OUT16 = true

    [spirv_kernel(local_size_x = 256)]
    def run {
        restride_body()
    }
}

//! the qwen25v window attention over the compact rows, one workgroup a window and head, the score block in workgroup memory
[vk_dispatch(name = "tower_win_attn_cls", kernel = "run", grid = "wgs", params = "wgs : int64"), arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-routes")]
class TowerWinAttn : GkWinAttn {
    override HS = 80u

    [spirv_kernel(local_size_x = 256, name = "tower_win_attn_cls_spv")]
    def run {
        win_attn_body()
    }
}

//! the vision towers' full-head NEOX rope over q or k rows, one table row a position shared by every head
[vk_dispatch(name = "tower_rope_tab_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TowerRopeTab : GkRopeTab {
    override NEOX = true

    [spirv_kernel(local_size_x = 256)]
    def run {
        rope_tab_body()
    }
}


//! the f32-mirror stamp of `RopeKvStoreT` on `GkRopeKvStore`
[vk_dispatch(name = "rope_kv_cls", grid = "wgs", params = "wgs : int64")]
class RopeKvStore : RopeKvStoreT {
    typedef KT = float
    override CLAMP16 = false

    [spirv_kernel(local_size_x = 256)]
    def run {
        rope_kv_body()
    }
}

//! the f16-mirror stamp of `RopeKvStoreT` on `GkRopeKvStore`, the stores clamped
[vk_dispatch(name = "rope_kv_f16_cls", grid = "wgs", params = "wgs : int64")]
class RopeKvStoreF16 : RopeKvStoreT {
    typedef KT = float16

    [spirv_kernel(local_size_x = 256)]
    def run {
        rope_kv_body()
    }
}


[vk_dispatch(name = "rope_kv_b_cls", grid = "wgs", params = "wgs : int64")]
class RopeKvStoreB : RopeKvStoreBT {
    typedef KT = float
    override CLAMP16 = false

    [spirv_kernel(local_size_x = 256)]
    def run {
        rope_kv_b_body()
    }
}

// f16-mirror twin — clamped stores
[vk_dispatch(name = "rope_kv_b_f16_cls", grid = "wgs", params = "wgs : int64")]
class RopeKvStoreBF16 : RopeKvStoreBT {
    typedef KT = float16

    [spirv_kernel(local_size_x = 256)]
    def run {
        rope_kv_b_body()
    }
}

//! The window rope-and-store's stamp: the f16-mirror twin under `kv16`, else the f32 one
def rope_kv_b_stamp_ensure(kv16 : bool) : bool => kv16 ? ensure_rope_kv_b_f16_cls() : ensure_rope_kv_b_cls()

//! ... its set
def rope_kv_b_stamp_set(kv16 : bool; bufs : uint64 const[6]; sizes : int64 const[6]; gbits : uint const[6]) : VkDescriptorSet
    => kv16 ? set_rope_kv_b_f16_cls(bufs, sizes, gbits) : set_rope_kv_b_cls(bufs, sizes, gbits)

//! ... and its dispatch
def rope_kv_b_stamp_enc(kv16 : bool; raw : VkCommandBuffer; var h : VkHaz; var s : VkDescriptorSet; var pc : RopeKvBArgs; wgs : int64) {
    if (kv16) {
        enc_rope_kv_b_f16_cls(raw, h, s, pc, wgs)
    } else {
        enc_rope_kv_b_cls(raw, h, s, pc, wgs)
    }
}


[vk_dispatch(name = "qkn_rope_cls", grid = "wgs", params = "wgs : int64")]
class QknRopeKv : QknRopeKvT {
    typedef KT = float
    override CLAMP16 = false

    [spirv_kernel(local_size_x = 256)]
    def run {
        qkn_rope_body()
    }
}

// f16-mirror twin — clamped stores
[vk_dispatch(name = "qkn_rope_f16_cls", grid = "wgs", params = "wgs : int64")]
class QknRopeKvF16 : QknRopeKvT {
    typedef KT = float16

    [spirv_kernel(local_size_x = 256)]
    def run {
        qkn_rope_body()
    }
}

//! the token command's stamp: each staged row at its token block's region and position
[vk_dispatch(name = "kv_q8_store_cls", grid = "wgs", params = "wgs : int64")]
class KvQ8Store : KvQ8StoreT {
    [spirv_kernel(local_size_x = 256)]
    def run {
        store_body()
    }
}

//! the prefill window's stamp: the staged rows at consecutive positions from `pos0`
[vk_dispatch(name = "kv_q8_store_b_cls", grid = "wgs", params = "wgs : int64")]
class KvQ8StoreB : KvQ8StoreT {
    override TOKROW = false

    [spirv_kernel(local_size_x = 256)]
    def run {
        store_body()
    }
}

//! the token command's stamp: each row at its token block's region and position; `precise`, so the butterflies and the quant
//! sums round as the CPU's and the stored bytes are the cache's
[vk_dispatch(name = "kv_tq4_store_cls", grid = "wgs", params = "wgs : int64")]
class KvTq4Store : KvTq4StoreT {
    [spirv_kernel(local_size_x = 256, precise = true)]
    def run {
        store_body()
    }
}

//! the prefill window's stamp: the rows at consecutive positions from `pos0`
[vk_dispatch(name = "kv_tq4_store_b_cls", grid = "wgs", params = "wgs : int64")]
class KvTq4StoreB : KvTq4StoreT {
    override TOKROW = false

    [spirv_kernel(local_size_x = 256, precise = true)]
    def run {
        store_body()
    }
}

[vk_dispatch(name = "kv_tq4_unrot_cls", grid = "wgs", params = "wgs : int64")]
class KvTq4Unrot : KvTq4UnrotT {
    [spirv_kernel(local_size_x = 256, precise = true)]
    def run {
        unrot_body()
    }
}

[vk_dispatch(name = "kv_q8_shadow_cls", grid = "wgs", params = "wgs : int64")]
class KvQ8Shadow : KvShadowT {
    [spirv_kernel(local_size_x = 256)]
    def run {
        shadow_body()
    }
}

[vk_dispatch(name = "kv_tq4_shadow_cls", grid = "wgs", params = "wgs : int64")]
class KvTq4Shadow : KvShadowT {
    override TQ4 = true

    [spirv_kernel(local_size_x = 256)]
    def run {
        shadow_body()
    }
}

//! every kernel class a block-codec mirror's driver dispatches: the stores, the shadow, tq4's un-rotation, the attention stamps
//! a mirror codec's three decode attention stamps - the four-head slab, the two-head one and the sink stamp
def da_attn_stamps_ensure(dt : KVDtype) : bool => da_attn_stamp_ensure(dt, DaSlab.four) && da_attn_stamp_ensure(dt, DaSlab.two) && da_attn_stamp_ensure(dt, DaSlab.sink)

def kv_codec_cls_ensure(dt : KVDtype) : bool {
    let attn = da_attn_stamps_ensure(dt)
    if (dt == KVDtype.q8_0) return attn && ensure_kv_q8_store_cls() && ensure_kv_q8_store_b_cls() && ensure_kv_q8_shadow_cls()
    return attn && ensure_kv_tq4_store_cls() && ensure_kv_tq4_store_b_cls() && ensure_kv_tq4_shadow_cls() && ensure_kv_tq4_unrot_cls()
}


[vk_dispatch(name = "q8_gemv_cls", grid = "wgs", params = "wgs : int64")]
class Q8Gemv : Q8GemvT {
    [spirv_kernel(local_size_x = 64, local_size_x_id = 0, name = "q8_gemv_cls_spv"), arch(at="../ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-token-command")]
    def run {
        gemv_body()
    }
}


[vk_dispatch(name = "q8_gemv_n2_cls", grid = "wgs", params = "wgs : int64")]
class Q8GemvN2 : Q8GemvNT {
    override NC = 2

    [spirv_kernel(local_size_x = 64, local_size_x_id = 0)]
    def run {
        gemv_n_body()
    }
}

[vk_dispatch(name = "q8_gemv_n4_cls", grid = "wgs", params = "wgs : int64")]
class Q8GemvN4 : Q8GemvNT {
    override NC = 4

    [spirv_kernel(local_size_x = 64, local_size_x_id = 0)]
    def run {
        gemv_n_body()
    }
}

[vk_dispatch(name = "q8_gemv_n8_cls", grid = "wgs", params = "wgs : int64")]
class Q8GemvN8 : Q8GemvNT {
    override NC = 8

    [spirv_kernel(local_size_x = 64, local_size_x_id = 0)]
    def run {
        gemv_n_body()
    }
}

[vk_dispatch(name = "q8_gemv_n2p_cls", grid = "wgs", params = "wgs : int64")]
class Q8GemvN2P : Q8GemvNT {
    override NC = 2
    override R2 = true

    [spirv_kernel(local_size_x = 64, local_size_x_id = 0)]
    def run {
        gemv_n_body()
    }
}

[vk_dispatch(name = "q8_gemv_n4p_cls", grid = "wgs", params = "wgs : int64")]
class Q8GemvN4P : Q8GemvNT {
    override NC = 4
    override R2 = true

    [spirv_kernel(local_size_x = 64, local_size_x_id = 0)]
    def run {
        gemv_n_body()
    }
}

[vk_dispatch(name = "q8_gemv_n8p_cls", grid = "wgs", params = "wgs : int64")]
class Q8GemvN8P : Q8GemvNT {
    override NC = 8
    override R2 = true

    [spirv_kernel(local_size_x = 64, local_size_x_id = 0)]
    def run {
        gemv_n_body()
    }
}

//! the q8 decode GEMV carrying the residual step as its last workgroup's epilogue (`Q8GemvArT`)
[vk_dispatch(name = "q8_gemv_ar_cls", grid = "wgs", params = "wgs : int64")]
class Q8GemvAr : Q8GemvArT {
    [spirv_kernel(local_size_x = 256, name = "q8_gemv_ar_cls_spv"), arch(at="../ARCHITECTURE_GPU_SHARED_ROWS.md#q8-requant-byte-store")]
    def run {
        gemv_ar_body()
    }
}


[vk_dispatch(name = "q8_gemv_gu_n1_cls", grid = "wgs", params = "wgs : int64")]
class Q8GemvGuN1 : Q8GemvGuNT {
    override NC = 1

    [spirv_kernel(local_size_x = 256)]
    def run {
        gu_n_body()
    }
}

[vk_dispatch(name = "q8_gemv_gu_n2_cls", grid = "wgs", params = "wgs : int64")]
class Q8GemvGuN2 : Q8GemvGuNT {
    override NC = 2

    [spirv_kernel(local_size_x = 256)]
    def run {
        gu_n_body()
    }
}

[vk_dispatch(name = "q8_gemv_gu_n4_cls", grid = "wgs", params = "wgs : int64")]
class Q8GemvGuN4 : Q8GemvGuNT {
    override NC = 4

    [spirv_kernel(local_size_x = 256)]
    def run {
        gu_n_body()
    }
}

[vk_dispatch(name = "q8_gemv_gu_n8_cls", grid = "wgs", params = "wgs : int64")]
class Q8GemvGuN8 : Q8GemvGuNT {
    override NC = 8

    [spirv_kernel(local_size_x = 256)]
    def run {
        gu_n_body()
    }
}

//! the fused gate-up N stamps for up to `nb` columns
def q8_gemv_gu_n_ensure(nb : int64 = 8l) : bool {
    var ok = ensure_q8_gemv_gu_n2_cls()
    if (nb > 2l) {
        ok = ok && ensure_q8_gemv_gu_n4_cls()
    }
    if (nb > 4l) {
        ok = ok && ensure_q8_gemv_gu_n8_cls()
    }
    return ok
}

//! the gate-up N stamps share one layout, so the narrowest stamp's set binds them all
def q8_gemv_gu_n_set(bufs : uint64 const[8]; sizes : int64 const[8]; gbits : uint const[8]) : VkDescriptorSet
    => set_q8_gemv_gu_n2_cls(bufs, sizes, gbits)

//! the stamp for `pc.ncols` columns: the narrowest that holds them, past `GU_N_COLS` in tiles of that many (`col0` a tile's first column)
[arch(at="../ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-col-tiles")]
def q8_gemv_gu_n_enc(raw : VkCommandBuffer; var h : VkHaz; var set_ : VkDescriptorSet; var pc : GuNArgs; wgs : int64) {
    each_col_tile(pc, pc.ncols, GU_N_COLS) $ {
        if (pc.ncols <= 2u) {
            enc_q8_gemv_gu_n2_cls(raw, h, set_, pc, wgs)
        } elif (pc.ncols <= 4u) {
            enc_q8_gemv_gu_n4_cls(raw, h, set_, pc, wgs)
        } else {
            enc_q8_gemv_gu_n8_cls(raw, h, set_, pc, wgs)
        }
    }
}

//! the per-layer-embedding branch's activation, its Q8_0 requant and the proj GEMV in one dispatch (`Q8GemvPleActT`)
[vk_dispatch(name = "q8_gemv_pleact_cls", grid = "wgs", params = "wgs : int64")]
class Q8GemvPleAct : Q8GemvPleActT {
    [spirv_kernel(local_size_x = 256, name = "q8_gemv_pleact_cls_spv"), arch(at="../ARCHITECTURE_GPU_SHARED_ROWS.md#q8-requant-byte-store")]
    def run {
        pleact_body()
    }
}

//! the per-layer-embedding act + proj over `pc.ncols` columns (0 or 1 = one row), past eight in tiles of eight
[arch(at="../ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-col-tiles")]
def pleact_enc_cols(raw : VkCommandBuffer; var h : VkHaz; var set_ : VkDescriptorSet; var pc : PleActArgs; wgs : int64) {
    each_col_tile(pc, max(pc.ncols, 1u), GEMV_COL_TILE) $ {
        enc_q8_gemv_pleact_cls(raw, h, set_, pc, wgs)
    }
}


[vk_dispatch(name = "q51_gemv_cls", grid = "wgs", params = "wgs : int64")]
class Q51Gemv : Blk32GemvRunT {
    typedef FMT = KqTag_q51
    override Q51 = true

    [spirv_kernel(local_size_x = 64, local_size_x_id = 0)]
    def run {
        b32_body()
    }
}

[vk_dispatch(name = "q51_gemv_n_cls", kernel = "run", family = "q51_gemv_cls", grid = "wgs", params = "wgs : int64"),
 arch(at="../ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-block32-leaf")]
class Q51GemvN : Blk32GemvRunT {
    typedef FMT = KqTag_q51
    override Q51 = true
    override NCOL = true

    [spirv_kernel(local_size_x = 64, local_size_x_id = 0)]
    def run {
        b32_body()
    }
}

//! the routed down GEMV summed under the routing weights (`Blk32GemvT.sum_shell`) on each per-32 format's stacks, one set layout
[vk_dispatch(name = "q51_gemv_sum_cls", grid = "wgs", params = "wgs : int64")]
class Q51GemvSum : Blk32GemvT {
    typedef FMT = KqTag_q51
    override Q51 = true
    override SUM = true

    [spirv_kernel(local_size_x = 64, local_size_x_id = 0, name = "q51_gemv_sum_cls_spv")]
    def run {
        sum_shell()
    }
}

[vk_dispatch(name = "iq4nl32_gemv_sum_cls", kernel = "run", family = "q51_gemv_sum_cls", grid = "wgs", params = "wgs : int64")]
class Iq4nl32GemvSum : Blk32GemvT {
    typedef FMT = KqTag_iq4nl32
    override SUM = true

    [spirv_kernel(local_size_x = 64, local_size_x_id = 0, name = "iq4nl32_gemv_sum_cls_spv")]
    def run {
        sum_shell()
    }
}

[vk_dispatch(name = "q4032_gemv_sum_cls", kernel = "run", family = "q51_gemv_sum_cls", grid = "wgs", params = "wgs : int64")]
class Q4032GemvSum : Blk32GemvT {
    typedef FMT = KqTag_q4032
    override SUM = true

    [spirv_kernel(local_size_x = 64, local_size_x_id = 0, name = "q4032_gemv_sum_cls_spv")]
    def run {
        sum_shell()
    }
}

[vk_dispatch(name = "q5032_gemv_sum_cls", kernel = "run", family = "q51_gemv_sum_cls", grid = "wgs", params = "wgs : int64")]
class Q5032GemvSum : Blk32GemvT {
    typedef FMT = KqTag_q5032
    override SUM = true

    [spirv_kernel(local_size_x = 64, local_size_x_id = 0, name = "q5032_gemv_sum_cls_spv")]
    def run {
        sum_shell()
    }
}

[vk_dispatch(name = "mx4_gemv_sum_cls", kernel = "run", family = "q51_gemv_sum_cls", grid = "wgs", params = "wgs : int64")]
class Mx4GemvSum : Blk32GemvT {
    typedef FMT = KqTag_mx4
    override MX4 = true
    override SUM = true

    [spirv_kernel(local_size_x = 64, local_size_x_id = 0, name = "mx4_gemv_sum_cls_spv")]
    def run {
        sum_shell()
    }
}

[vk_dispatch(name = "iq4nl32_gemv_cls", grid = "wgs", params = "wgs : int64")]
class Iq4nl32Gemv : Blk32GemvRunT {
    typedef FMT = KqTag_iq4nl32

    [spirv_kernel(local_size_x = 64, local_size_x_id = 0)]
    def run {
        b32_body()
    }
}

[vk_dispatch(name = "iq4nl32_gemv_n_cls", kernel = "run", family = "iq4nl32_gemv_cls", grid = "wgs", params = "wgs : int64")]
class Iq4nl32GemvN : Blk32GemvRunT {
    typedef FMT = KqTag_iq4nl32
    override NCOL = true

    [spirv_kernel(local_size_x = 64, local_size_x_id = 0)]
    def run {
        b32_body()
    }
}

[vk_dispatch(name = "q4032_gemv_cls", grid = "wgs", params = "wgs : int64")]
class Q4032Gemv : Blk32GemvRunT {
    typedef FMT = KqTag_q4032

    [spirv_kernel(local_size_x = 64, local_size_x_id = 0)]
    def run {
        b32_body()
    }
}

//! the per-32 token-table gathers (`Blk32GemvRunT.emb_shell`), one family layout - the GEMV's six bindings
[vk_dispatch(name = "q51_emb_cls", kernel = "run", family = "b32_emb_cls", grid = "wgs", params = "wgs : int64")]
class Q51Emb : Blk32GemvRunT {
    typedef FMT = KqTag_q51
    override Q51 = true
    override EMB = true

    [spirv_kernel(local_size_x = 64, local_size_x_id = 0)]
    def run {
        b32_body()
    }
}

[vk_dispatch(name = "iq4nl32_emb_cls", kernel = "run", family = "b32_emb_cls", grid = "wgs", params = "wgs : int64")]
class Iq4nl32Emb : Blk32GemvRunT {
    typedef FMT = KqTag_iq4nl32
    override EMB = true

    [spirv_kernel(local_size_x = 64, local_size_x_id = 0)]
    def run {
        b32_body()
    }
}

[vk_dispatch(name = "q4032_emb_cls", kernel = "run", family = "b32_emb_cls", grid = "wgs", params = "wgs : int64")]
class Q4032Emb : Blk32GemvRunT {
    typedef FMT = KqTag_q4032
    override EMB = true

    [spirv_kernel(local_size_x = 64, local_size_x_id = 0)]
    def run {
        b32_body()
    }
}

[vk_dispatch(name = "q4032_gemv_n_cls", kernel = "run", family = "q4032_gemv_cls", grid = "wgs", params = "wgs : int64")]
class Q4032GemvN : Blk32GemvRunT {
    typedef FMT = KqTag_q4032
    override NCOL = true

    [spirv_kernel(local_size_x = 64, local_size_x_id = 0)]
    def run {
        b32_body()
    }
}

[vk_dispatch(name = "q5032_gemv_cls", grid = "wgs", params = "wgs : int64")]
class Q5032Gemv : Blk32GemvRunT {
    typedef FMT = KqTag_q5032

    [spirv_kernel(local_size_x = 64, local_size_x_id = 0)]
    def run {
        b32_body()
    }
}

[vk_dispatch(name = "q5032_emb_cls", kernel = "run", family = "b32_emb_cls", grid = "wgs", params = "wgs : int64")]
class Q5032Emb : Blk32GemvRunT {
    typedef FMT = KqTag_q5032
    override EMB = true

    [spirv_kernel(local_size_x = 64, local_size_x_id = 0)]
    def run {
        b32_body()
    }
}

[vk_dispatch(name = "q5032_gemv_n_cls", kernel = "run", family = "q5032_gemv_cls", grid = "wgs", params = "wgs : int64")]
class Q5032GemvN : Blk32GemvRunT {
    typedef FMT = KqTag_q5032
    override NCOL = true

    [spirv_kernel(local_size_x = 64, local_size_x_id = 0)]
    def run {
        b32_body()
    }
}


[vk_dispatch(name = "mx4_gemv_cls", grid = "wgs", params = "wgs : int64")]
class Mx4Gemv : Blk32GemvRunT {
    typedef FMT = KqTag_mx4
    override MX4 = true

    [spirv_kernel(local_size_x = 64, local_size_x_id = 0)]
    def run {
        b32_body()
    }
}

[vk_dispatch(name = "mx4_gemv_n_cls", kernel = "run", family = "mx4_gemv_cls", grid = "wgs", params = "wgs : int64")]
class Mx4GemvN : Blk32GemvRunT {
    typedef FMT = KqTag_mx4
    override MX4 = true
    override NCOL = true

    [spirv_kernel(local_size_x = 64, local_size_x_id = 0)]
    def run {
        b32_body()
    }
}


[ |> template_struct_instance, arch(at="../ARCHITECTURE_GPU_VULKAN.md#vk-b32-window-gemm")]
class template Blk32BatchT : MoeCmBase {
    @template_constant Q51 : bool = false
    @template_constant MX4 : bool = false
    @ssbo @binding = 0 wq : array<uint>        //! weight quant words, `Blk32GemvT`'s layout
    @ssbo @binding = 1 wsu : array<uint>       //! per-block scales, `Blk32GemvT`'s layout
    @ssbo @binding = 3 xqw : array<uint>       //! Q8_0 activation words
    @ssbo @binding = 4 axs : array<float>      // per-block activation scales
    @workgroup xw4 : uint4[544]
    @workgroup ww4 : uint4[544]
    @workgroup xsw : float[256]
    @workgroup wsw : float[256]    //! per-(block, col) d
    @workgroup wmw : float[256]    //! per-(block, col) m (Q5_1)

    //! weight block `wb`'s half `sh` (`blk32_quad`): its words as this class binds the plane
    def blk_quad(wb, sh : uint) : uint4 {
        let b = wb * blk32_qwords(type<FMT>)
        return blk32_quad(type<FMT>, uint4(wq[b], wq[b + 1u], wq[b + 2u], wq[b + 3u]), wq[b + blk32_qh_off(type<FMT>)], sh)
    }

    //! weight block `wb`'s scale pair (`blk32_scale`)
    def blk_scale(wb : uint) : float2 => blk32_scale(type<FMT>, wsu[blk32_sword(type<FMT>, wb)], wb)

    def wdot(w, x : uint) : int {
        static_if (MX4) {
            return mx4_sdot(w, x)
        } else {
            return sdot4(w, x)
        }
    }

    [spirv_inline]
    def blk32_body {   // nolint:STYLE038 — Q8Batch's stage/reduce/store pass over the per-32 decode
        let reg = region_rec()
        let wblk0 = reg.x
        let row0 = reg.y
        let cnt = reg.z
        let tix = reg.w
        let wtiles = (pa.d + 31u) / 32u
        let xt = tix / wtiles
        let wt = tix % wtiles
        let nbb = pa.n / 32u
        let nsteps = (nbb + 7u) / 8u
        let tid = gl_LocalInvocationID.x
        let word = tid % 8u
        let grp = tid / 8u
        let xrow = xt * 32u + grp
        let wcol = wt * 32u + grp
        let slane = tid % 32u
        let sblk = tid / 32u
        var f0 = 0.0
        var f1 = 0.0
        var f2 = 0.0
        var f3 = 0.0
        var s = 0u
        while (s < nsteps) {
            let b0 = s * 8u
            for (ki in range(2)) {
                let k = word + uint(ki) * 8u
                let bu = k / 2u
                let h = (k % 2u) * 4u
                var xv = uint4(0u)
                if (xrow < cnt && b0 + bu < nbb) {
                    let xb4 = ((row0 + xrow) * nbb + b0 + bu) * 8u + h
                    xv = uint4(xqw[xb4], xqw[xb4 + 1u], xqw[xb4 + 2u], xqw[xb4 + 3u])
                }
                xw4[grp * 17u + k] = xv
                var wv = uint4(0u)
                if (wcol < pa.d && b0 + bu < nbb) {
                    wv = blk_quad(wblk0 + wcol * nbb + b0 + bu, h)
                }
                ww4[grp * 17u + k] = wv
            }
            var xs = 0.0
            if (xt * 32u + slane < cnt && b0 + sblk < nbb) {
                xs = axs[(row0 + xt * 32u + slane) * nbb + b0 + sblk]
            }
            xsw[sblk * 32u + slane] = xs
            var sc = float2(0.0, 0.0)
            if (wt * 32u + slane < pa.d && b0 + sblk < nbb) {
                sc = blk_scale(wblk0 + (wt * 32u + slane) * nbb + b0 + sblk)
            }
            wsw[sblk * 32u + slane] = sc.x
            wmw[sblk * 32u + slane] = sc.y
            barrier()
            for (bb in range(8)) {     // a tail block past the plane stages zero quants and zero scales
                let bu = uint(bb)
                var i0 = 0
                var i1 = 0
                var i2 = 0
                var i3 = 0
                var asum = 0
                for (kk in range(2)) {
                    let k = bu * 2u + uint(kk)
                    let xw = xw4[grp * 17u + k]
                    let wv0 = ww4[word * 17u + k]
                    let wv1 = ww4[(word + 8u) * 17u + k]
                    let wv2 = ww4[(word + 16u) * 17u + k]
                    let wv3 = ww4[(word + 24u) * 17u + k]
                    i0 += wdot(wv0.x, xw.x) + wdot(wv0.y, xw.y) + wdot(wv0.z, xw.z) + wdot(wv0.w, xw.w)
                    i1 += wdot(wv1.x, xw.x) + wdot(wv1.y, xw.y) + wdot(wv1.z, xw.z) + wdot(wv1.w, xw.w)
                    i2 += wdot(wv2.x, xw.x) + wdot(wv2.y, xw.y) + wdot(wv2.z, xw.z) + wdot(wv2.w, xw.w)
                    i3 += wdot(wv3.x, xw.x) + wdot(wv3.y, xw.y) + wdot(wv3.z, xw.z) + wdot(wv3.w, xw.w)
                    static_if (Q51) {
                        asum += sdot4(xw.x, 0x01010101u) + sdot4(xw.y, 0x01010101u) + sdot4(xw.z, 0x01010101u) + sdot4(xw.w, 0x01010101u)
                    }
                }
                let xscl = xsw[bu * 32u + grp]
                static_if (Q51) {
                    let fa = float(asum)
                    f0 += mad(wsw[bu * 32u + word], float(i0), wmw[bu * 32u + word] * fa) * xscl
                    f1 += mad(wsw[bu * 32u + word + 8u], float(i1), wmw[bu * 32u + word + 8u] * fa) * xscl
                    f2 += mad(wsw[bu * 32u + word + 16u], float(i2), wmw[bu * 32u + word + 16u] * fa) * xscl
                    f3 += mad(wsw[bu * 32u + word + 24u], float(i3), wmw[bu * 32u + word + 24u] * fa) * xscl
                } else {
                    f0 += float(i0) * wsw[bu * 32u + word] * xscl
                    f1 += float(i1) * wsw[bu * 32u + word + 8u] * xscl
                    f2 += float(i2) * wsw[bu * 32u + word + 16u] * xscl
                    f3 += float(i3) * wsw[bu * 32u + word + 24u] * xscl
                }
            }
            barrier()
            s++
        }
        store_y(xrow, cnt, row0, wt, word, float4(f0, f1, f2, f3))
    }
}

[vk_dispatch(name = "q51_batch_cls", kernel = "run", family = "blk32_batch_cls", grid = "wgs", params = "wgs : int64")]
class Q51Batch : Blk32BatchT {
    typedef FMT = KqTag_q51
    override Q51 = true

    [spirv_kernel(local_size_x = 256)]
    def run {
        blk32_body()
    }
}

[vk_dispatch(name = "iq4nl32_batch_cls", kernel = "run", family = "blk32_batch_cls", grid = "wgs", params = "wgs : int64")]
class Iq4nl32Batch : Blk32BatchT {
    typedef FMT = KqTag_iq4nl32

    [spirv_kernel(local_size_x = 256)]
    def run {
        blk32_body()
    }
}

[vk_dispatch(name = "q4032_batch_cls", kernel = "run", family = "blk32_batch_cls", grid = "wgs", params = "wgs : int64")]
class Q4032Batch : Blk32BatchT {
    typedef FMT = KqTag_q4032

    [spirv_kernel(local_size_x = 256)]
    def run {
        blk32_body()
    }
}

[vk_dispatch(name = "q5032_batch_cls", kernel = "run", family = "blk32_batch_cls", grid = "wgs", params = "wgs : int64")]
class Q5032Batch : Blk32BatchT {
    typedef FMT = KqTag_q5032

    [spirv_kernel(local_size_x = 256)]
    def run {
        blk32_body()
    }
}

[vk_dispatch(name = "mx4_batch_cls", kernel = "run", family = "blk32_batch_cls", grid = "wgs", params = "wgs : int64")]
class Mx4Batch : Blk32BatchT {
    typedef FMT = KqTag_mx4
    override MX4 = true

    [spirv_kernel(local_size_x = 256)]
    def run {
        blk32_body()
    }
}

//! bindings 10 and 11 sit at the hazard rail's cap (`SetBits` holds twelve): a thirteenth plane here is a rail change, not a line
[vk_dispatch(name = "mx4_gemv_gu_cls", grid = "wgs", params = "wgs : int64")]
class Mx4GemvGu : Mx4GemvGuT {
    [spirv_kernel(local_size_x = 256, name = "mx4_gemv_gu_cls_spv"), arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#resident-moe-token")]
    def run {
        gu_body()
    }
}


[arch(at="../ARCHITECTURE_GPU_VULKAN.md#kq-gemv-grid-buffer")]
def kq_grid_dev() : uint64 {
    if (g_gpu.kq_grid_dev == 0ul) {
        var words <- kq_grid_words()
        g_gpu.kq_grid_dev = make_device_buf(KQ_GRID_BYTES)
        upload_region_at(g_gpu.kq_grid_dev, 0l, unsafe(addr<uint8 const?>(words[0])), KQ_GRID_BYTES)
    }
    return g_gpu.kq_grid_dev
}


// ===== the q8 batch tile router (class side) =====
// The (mode, tile, aligned) pick as a variant ordinal — all six classes
// share the 6-buffer member layout + BatchArgs, but each carries its own set layout.
// variant: 0 = sdot4 tile, 1 = cmf16, 2 = cmi8, 3 = mm M-tile, 4 = mm aligned-L, 5 = mm L

def q8_batch_cls_variant(tile : int64; aligned : bool) : int {
    if (g_gpu.coopmat_mode == COOPMAT_F16) {
        return 1
    } elif (g_gpu.coopmat_mode == COOPMAT_INT8) {
        return 2
    } elif (g_gpu.coopmat_mode >= COOPMAT_MM) {
        if (tile == BATCH_TILE) {   // the small tier IS the sdot4 kernel — measured, not a fallback
            return 0
        }
        return tile == 64l ? 3 : (aligned ? 4 : 5)
    }
    return 0
}

def q8_batch_cls_ensure(v : int) : bool {
    if (v == 1) {
        return ensure_q8_batch_cmf16_cls()
    } elif (v == 2) {
        return ensure_q8_batch_cmi8_cls()
    } elif (v == 3) {
        return ensure_q8_batch_mm_m_cls()
    } elif (v == 4) {
        return ensure_q8_batch_mm_a_cls()
    } elif (v == 5) {
        return ensure_q8_batch_mm_cls()
    } elif (v == 0) {
        return ensure_q8_batch_cls()
    }
    panic("dasLLAMA vulkan tier: no q8 batch class for variant {v}")
    return false
}

def q8_batch_cls_set(v : int; bufs : uint64 const[6]; sizes : int64 const[6]; gbits : uint const[6]) : VkDescriptorSet {
    if (v == 1) {
        return set_q8_batch_cmf16_cls(bufs, sizes, gbits)
    } elif (v == 2) {
        return set_q8_batch_cmi8_cls(bufs, sizes, gbits)
    } elif (v == 3) {
        return set_q8_batch_mm_m_cls(bufs, sizes, gbits)
    } elif (v == 4) {
        return set_q8_batch_mm_a_cls(bufs, sizes, gbits)
    } elif (v == 5) {
        return set_q8_batch_mm_cls(bufs, sizes, gbits)
    } elif (v == 0) {
        return set_q8_batch_cls(bufs, sizes, gbits)
    }
    panic("dasLLAMA vulkan tier: no q8 batch class for variant {v}")
    return VkDescriptorSet()
}

def q8_batch_cls_enc(v : int; raw : VkCommandBuffer; var h : VkHaz; var set_ : VkDescriptorSet; var pc : BatchArgs; wgs : int64) {
    if (v == 1) {
        enc_q8_batch_cmf16_cls(raw, h, set_, pc, wgs)
    } elif (v == 2) {
        enc_q8_batch_cmi8_cls(raw, h, set_, pc, wgs)
    } elif (v == 3) {
        enc_q8_batch_mm_m_cls(raw, h, set_, pc, wgs)
    } elif (v == 4) {
        enc_q8_batch_mm_a_cls(raw, h, set_, pc, wgs)
    } elif (v == 5) {
        enc_q8_batch_mm_cls(raw, h, set_, pc, wgs)
    } elif (v == 0) {
        enc_q8_batch_cls(raw, h, set_, pc, wgs)
    } else {
        panic("dasLLAMA vulkan tier: no q8 batch class for variant {v}")
    }
}

// ===== the kq batch tile router (class side) =====
// The family shares one set layout ("kq_batch_cls"); the q40 f16-MMA twin (coopmat_mode 1 —
// no s8-native for nibbles) carries its own. The stack and resident-prefill encoders share it.

def kq_batch_cls_ensure(fmt : int; q40cm : bool) : bool {   // nolint:STYLE037 — kq_tile_stamp's generated arms, one a format's stamp
    if (q40cm) return ensure_kq_batch_q40_cmf16_cls()
    var r = false
    kq_tile_stamp(fmt, "q8") <| $() {
        r = tile_ensure_cls()
    }
    return r
}

def kq_batch_cls_set_for(fmt : int; q40cm : bool; bufs : uint64 const[6]; sizes : int64 const[6]; gbits : uint const[6]) : VkDescriptorSet {
    if (q40cm) {
        return set_kq_batch_q40_cmf16_cls(bufs, sizes, gbits)
    }
    if (blk32_rail(fmt)) {
        return set_blk32_batch_cls(bufs, sizes, gbits)
    }
    if (!kq_sb(fmt)) {
        panic("dasLLAMA vulkan tier: no kq batch class for format {fmt}")
    }
    return set_kq_batch_cls(bufs, sizes, gbits)
}

def kq_batch_cls_enc_for(fmt : int; q40cm : bool; raw : VkCommandBuffer; var h : VkHaz; var set_ : VkDescriptorSet; var pc : BatchArgs; wgs : int64) {   // nolint:STYLE037 — kq_tile_stamp's generated arms, one a format's stamp
    if (q40cm) {
        enc_kq_batch_q40_cmf16_cls(raw, h, set_, pc, wgs)
        return
    }
    kq_tile_stamp(fmt, "q8") <| $() {
        tile_enc_cls(raw, h, set_, pc, wgs)
    }
}

// ===== format-dispatched class-GEMV rail =====
// The drivers' format pick as ensure/set/enc: every decode GEMV class
// shares the 6-buffer shape (wq, ws, regions, xq, xs, y) and GemvArgs.

//! the decode GEMV shells a format stamps, `kq_tile_stamp`'s tails: the plain rows, the summed routed down stacks (`sum_shell`),
//! the fused routed gate and up (`gu_shell`)
let GEMV_SHELLS = "cls sum_cls gu_cls"
let GEMV_SHELL_PLAIN = 0
let GEMV_SHELL_SUM = 1   //! the per-32 formats past q8
let GEMV_SHELL_GU = 2    //! the kq formats the twins stamp, and mx4's biased twin
//! the (format, shell) pairs no stamp serves
let GEMV_SHELL_SKIP = "q8/sum_cls k2/sum_cls k3/sum_cls k4/sum_cls k5/sum_cls k6/sum_cls q40/sum_cls iq4xs/sum_cls iq3s/sum_cls iq3xxs/sum_cls iq4nl/sum_cls iq2s/sum_cls iq2xs/sum_cls iq2xxs/sum_cls iq3s4/sum_cls q8/gu_cls q51/gu_cls iq4nl32/gu_cls q4032/gu_cls q5032/gu_cls k2/gu_cls k3/gu_cls k5/gu_cls k6/gu_cls q40/gu_cls iq3s/gu_cls iq3xxs/gu_cls iq4nl/gu_cls iq2xs/gu_cls iq2xxs/gu_cls"

//! whether format `fmt` stamps decode GEMV shell `shell`: no pair of `GEMV_SHELL_SKIP` names it
def gemv_shell_fmt(fmt, shell : int) : bool => kq_tile_stamped(fmt, GEMV_SHELL_SKIP, shell, GEMV_SHELLS)

def gemv_shell_ensure(fmt, shell : int) : bool {   // nolint:STYLE037 — kq_tile_stamp's generated arms, one a format's stamp a shell
    var r = false
    kq_tile_stamp(fmt, GEMV_SHELL_SKIP, "gemv", shell, GEMV_SHELLS) <| $() {
        r = tile_ensure()
    }
    return r
}

def gemv_shell_enc(fmt, shell : int; raw : VkCommandBuffer; var h : VkHaz; var set_ : VkDescriptorSet; var pc : GemvArgs; wgs : int64) {   // nolint:STYLE037 — kq_tile_stamp's generated arms, one a format's stamp a shell
    kq_tile_stamp(fmt, GEMV_SHELL_SKIP, "gemv", shell, GEMV_SHELLS) <| $() {
        tile_enc(raw, h, set_, pc, wgs)
    }
}

def gemv_cls_ensure(fmt : int) : bool => gemv_shell_ensure(fmt, GEMV_SHELL_PLAIN)

def gemv_cls_set(fmt : int; bufs : uint64 const[6]; sizes : int64 const[6]; gbits : uint const[6]) : VkDescriptorSet {
    if (fmt == int(KqFmt.q8)) {
        return set_q8_gemv_cls(bufs, sizes, gbits)
    } elif (fmt == int(KqFmt.q51)) {
        return set_q51_gemv_cls(bufs, sizes, gbits)
    } elif (fmt == int(KqFmt.mx4)) {
        return set_mx4_gemv_cls(bufs, sizes, gbits)
    } elif (fmt == int(KqFmt.iq4nl32)) {
        return set_iq4nl32_gemv_cls(bufs, sizes, gbits)
    } elif (fmt == int(KqFmt.q4032)) {
        return set_q4032_gemv_cls(bufs, sizes, gbits)
    } elif (fmt == int(KqFmt.q5032)) {
        return set_q5032_gemv_cls(bufs, sizes, gbits)
    } elif (kq_sb(fmt)) {
        // the family shares one layout — one set serves every kq format
        return set_kq_gemv_cls(fixed_array(bufs[0], bufs[1], bufs[2], bufs[3], bufs[4], bufs[5], kq_grid_dev()),
            fixed_array(sizes[0], sizes[1], sizes[2], sizes[3], sizes[4], sizes[5], KQ_GRID_BYTES),
            fixed_array(gbits[0], gbits[1], gbits[2], gbits[3], gbits[4], gbits[5], 0u))
    }
    panic("dasLLAMA vulkan tier: no GEMV class for format {fmt}")
    return VkDescriptorSet()
}

//! a decode GEMV set over a plane pair `p` (an arena's `arena_planes`, a stack's or a driver's own weights): the quant and
//! scale planes, the region meta buffer with its hazard bit, the activation pair the GEMV reads (one bit for both) and its output
def gemv_planes_set(fmt : int; p : tuple<wq : uint64; ws : uint64; wqb : int64; wsb : int64>; meta : uint64; meta_bytes : int64; meta_bit : uint;
                    xq, xs : uint64; xq_bytes, xs_bytes : int64; xbit : uint; y : uint64; y_bytes : int64; ybit : uint) : VkDescriptorSet {
    return gemv_cls_set(fmt, fixed_array(p.wq, p.ws, meta, xq, xs, y), fixed_array(p.wqb, p.wsb, meta_bytes, xq_bytes, xs_bytes, y_bytes),
        fixed_array(0u, 0u, meta_bit, xbit, xbit, ybit))
}

def gemv_cls_enc(fmt : int; raw : VkCommandBuffer; var h : VkHaz; var set_ : VkDescriptorSet; var pc : GemvArgs; wgs : int64) {
    gemv_shell_enc(fmt, GEMV_SHELL_PLAIN, raw, h, set_, pc, wgs)
}

//! the GEMV family's dispatch grid over `rows` output rows at `lanes_per_row` (0 = a whole subgroup per
//! row, the q8 GEMV's only form): a workgroup's subgroups each take subgroup_size / lanes_per_row rows
def gemv_wgs(rows : int64; lanes_per_row : uint) : int64 => gemv_wgs_of(rows, lanes_per_row, g_gpu.subgroup_size, g_gpu.rows_per_wg)

//! the family's dispatch over `rows` output rows: the lanes per row by the row length, the grid to match; `pc.ncols`
//! past one takes the N-column leaf over that many activation rows, its y rows `pc.ystride` apart
[arch(at="../ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-col-tiles")]
def gemv_enc(fmt : int; raw : VkCommandBuffer; var h : VkHaz; var set_ : VkDescriptorSet; var pc : GemvArgs; rows : int64) {
    pc.lanes_per_row = gemv_lanes_per_row(fmt, int64(pc.n))
    let wgs = gemv_wgs(rows, pc.lanes_per_row)
    if (pc.ncols <= 1u) {
        gemv_cls_enc(fmt, raw, h, set_, pc, wgs)
        return
    }
    each_col_tile(pc, pc.ncols, GEMV_COL_TILE) $ {
        gemv_cls_enc_n(fmt, raw, h, set_, pc, wgs)
    }
}

// ===== the N-column GEMV leaves: one weight pass over `pa.ncols` activation rows =====

//! the format the N-column ladder skips: q8, its own stamps picked by row count
let private GEMV_NO_N_LEAF = "q8"

//! the superblock decode GEMV family's roster - q8 and every kq leaf, in enum order: what the probe's ruler walks (the
//! per-32 expert GEMVs carry another plane shape, so the kernel cells walk every member themselves)
def kq_gemv_fmts() : array<KqFmt> => <- [for (f in type<KqFmt>); f; where f == KqFmt.q8 || kq_sb(f)]

//! the reduction width past which the q8 N-column form pairs output rows a subgroup; off until a box's token command proves the pair
var g_q8_n2_min_n = 0xFFFFFFFFu

//! the q8 row-pair form is armed: `g_q8_n2_min_n` sits under the widest reduction
def q8_pair_armed() : bool => g_q8_n2_min_n != 0xFFFFFFFFu

//! the N-column leaves of `fmt` for up to `nb` columns: the q8 stamps the ladder can pick, the pair twins only while armed
def gemv_cls_ensure_n(fmt : int; nb : int64 = 8l) : bool {   // nolint:STYLE037 — kq_tile_stamp's generated arms, one a format
    if (fmt == int(KqFmt.q8)) {
        let pair = q8_pair_armed()
        var ok = ensure_q8_gemv_n2_cls() && (!pair || ensure_q8_gemv_n2p_cls())
        if (nb > 2l) {
            ok = ok && ensure_q8_gemv_n4_cls() && (!pair || ensure_q8_gemv_n4p_cls())
        }
        if (nb > 4l) {
            ok = ok && ensure_q8_gemv_n8_cls() && (!pair || ensure_q8_gemv_n8p_cls())
        }
        return ok
    }
    var r = false
    kq_tile_stamp(fmt, GEMV_NO_N_LEAF, "gemv") <| $() {
        r = tile_ensure_n_cls()
    }
    return r
}

//! the q8 stamps share one layout of their own, so the narrowest stamp's set - the one every row count builds - binds them all; a kq leaf shares the family's
def gemv_cls_set_n(fmt : int; bufs : uint64 const[6]; sizes : int64 const[6]; gbits : uint const[6]) : VkDescriptorSet {
    if (fmt == int(KqFmt.q8)) {
        return set_q8_gemv_n2_cls(bufs, sizes, gbits)
    }
    return gemv_cls_set(fmt, bufs, sizes, gbits)
}

//! the q8 stamp for `pc.ncols` columns: the narrowest that holds them, the row-pair form past `g_q8_n2_min_n` on an even row count
[arch(at="../ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-token-command")]
def private q8_gemv_n_enc(raw : VkCommandBuffer; var h : VkHaz; var set_ : VkDescriptorSet; var pc : GemvArgs; wgs : int64) {
    let pair_wanted = q8_pair_armed() && pc.n >= g_q8_n2_min_n && pc.d % 2u == 0u
    let g2 = (wgs + 1l) / 2l   //! a subgroup a row pair: half the row grid
    if (pc.ncols <= 2u) {   //! each pair twin builds on first use, since the lever can move after the family's ensure ran
        if (pair_wanted && ensure_q8_gemv_n2p_cls()) {
            enc_q8_gemv_n2p_cls(raw, h, set_, pc, g2)
        } else {
            enc_q8_gemv_n2_cls(raw, h, set_, pc, wgs)
        }
    } elif (pc.ncols <= 4u) {
        if (pair_wanted && ensure_q8_gemv_n4p_cls()) {
            enc_q8_gemv_n4p_cls(raw, h, set_, pc, g2)
        } else {
            enc_q8_gemv_n4_cls(raw, h, set_, pc, wgs)
        }
    } elif (pair_wanted && ensure_q8_gemv_n8p_cls()) {
        enc_q8_gemv_n8p_cls(raw, h, set_, pc, g2)
    } else {
        enc_q8_gemv_n8_cls(raw, h, set_, pc, wgs)
    }
}

def gemv_cls_enc_n(fmt : int; raw : VkCommandBuffer; var h : VkHaz; var set_ : VkDescriptorSet; var pc : GemvArgs; wgs : int64) {   // nolint:STYLE037 — kq_tile_stamp's generated arms, one a format's stamp
    if (fmt == int(KqFmt.q8)) {
        q8_gemv_n_enc(raw, h, set_, pc, wgs)
        return
    }
    kq_tile_stamp(fmt, GEMV_NO_N_LEAF, "gemv") <| $() {
        tile_enc_n_cls(raw, h, set_, pc, wgs)
    }
}


[vk_dispatch(name = "kq_gemv_k4_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvK4 : K4GemvT {
    [spirv_kernel(local_size_x = 64)]
    def run {
        leaf_body()
    }
}

[vk_dispatch(name = "kq_gemv_k4_n_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvK4N : K4GemvT {
    override NCOL = true

    [spirv_kernel(local_size_x = 64)]
    def run {
        leaf_body()
    }
}

//! the routed experts' fused gate and up GEMVs (`gu_shell`) on the Q4_K stacks; the iq4xs, iq3s4 and iq2s twins share its set layout
[vk_dispatch(name = "kq_gemv_k4_gu_cls", kernel = "run_gu", grid = "wgs", params = "wgs : int64")]
class KqGemvK4Gu : K4GemvT {
    override GU = true

    [spirv_kernel(local_size_x = 256, name = "kq_gemv_k4_gu_cls_spv")]
    def run_gu {
        gu_shell()
    }
}


[vk_dispatch(name = "kq_gemv_q40_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvQ40 : Q40GemvT {
    [spirv_kernel(local_size_x = 64)]
    def run {
        leaf_body()
    }
}

[vk_dispatch(name = "kq_gemv_q40_n_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvQ40N : Q40GemvT {
    override NCOL = true

    [spirv_kernel(local_size_x = 64)]
    def run {
        leaf_body()
    }
}


[vk_dispatch(name = "kq_gemv_iq4xs_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvIq4xs : Iq4xsGemvT {
    [spirv_kernel(local_size_x = 64)]
    def run {
        leaf_body()
    }
}

[vk_dispatch(name = "kq_gemv_iq4xs_n_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvIq4xsN : Iq4xsGemvT {
    override NCOL = true

    [spirv_kernel(local_size_x = 64)]
    def run {
        leaf_body()
    }
}

[vk_dispatch(name = "kq_gemv_iq3s4_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvIq3s4 : Iq4xsGemvT {
    override IQODD = true

    [spirv_kernel(local_size_x = 64)]
    def run {
        leaf_body()
    }
}

[vk_dispatch(name = "kq_gemv_iq3s4_n_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvIq3s4N : Iq4xsGemvT {
    override IQODD = true
    override NCOL = true

    [spirv_kernel(local_size_x = 64)]
    def run {
        leaf_body()
    }
}

[vk_dispatch(name = "kq_gemv_iq4xs_gu_cls", kernel = "run_gu", family = "kq_gemv_k4_gu_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvIq4xsGu : Iq4xsGemvT {
    override GU = true

    [spirv_kernel(local_size_x = 256, name = "kq_gemv_iq4xs_gu_cls_spv")]
    def run_gu {
        gu_shell()
    }
}

[vk_dispatch(name = "kq_gemv_iq3s4_gu_cls", kernel = "run_gu", family = "kq_gemv_k4_gu_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvIq3s4Gu : Iq4xsGemvT {
    override IQODD = true
    override GU = true

    [spirv_kernel(local_size_x = 256, name = "kq_gemv_iq3s4_gu_cls_spv")]
    def run_gu {
        gu_shell()
    }
}


[vk_dispatch(name = "kq_gemv_iq3s_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvIq3s : Iq3sGemvT {
    [spirv_kernel(local_size_x = 64)]
    def run {
        leaf_body()
    }
}

[vk_dispatch(name = "kq_gemv_iq3s_n_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvIq3sN : Iq3sGemvT {
    override NCOL = true

    [spirv_kernel(local_size_x = 64)]
    def run {
        leaf_body()
    }
}


[vk_dispatch(name = "kq_gemv_iq3xxs_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvIq3xxs : Iq3xxsGemvT {
    [spirv_kernel(local_size_x = 64)]
    def run {
        leaf_body()
    }
}

[vk_dispatch(name = "kq_gemv_iq3xxs_n_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvIq3xxsN : Iq3xxsGemvT {
    override NCOL = true

    [spirv_kernel(local_size_x = 64)]
    def run {
        leaf_body()
    }
}


[vk_dispatch(name = "kq_gemv_iq4nl_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvIq4nl : Iq4nlGemvT {
    [spirv_kernel(local_size_x = 64)]
    def run {
        leaf_body()
    }
}

[vk_dispatch(name = "kq_gemv_iq4nl_n_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvIq4nlN : Iq4nlGemvT {
    override NCOL = true

    [spirv_kernel(local_size_x = 64)]
    def run {
        leaf_body()
    }
}


[vk_dispatch(name = "kq_gemv_k3_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvK3 : K3GemvT {
    [spirv_kernel(local_size_x = 64)]
    def run {
        leaf_body()
    }
}

[vk_dispatch(name = "kq_gemv_k3_n_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvK3N : K3GemvT {
    override NCOL = true

    [spirv_kernel(local_size_x = 64)]
    def run {
        leaf_body()
    }
}


[vk_dispatch(name = "kq_gemv_k2_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvK2 : K2GemvT {
    [spirv_kernel(local_size_x = 64)]
    def run {
        leaf_body()
    }
}

[vk_dispatch(name = "kq_gemv_k2_n_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvK2N : K2GemvT {
    override NCOL = true

    [spirv_kernel(local_size_x = 64)]
    def run {
        leaf_body()
    }
}


[vk_dispatch(name = "kq_gemv_k5_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvK5 : K5GemvT {
    [spirv_kernel(local_size_x = 64)]
    def run {
        leaf_body()
    }
}

[vk_dispatch(name = "kq_gemv_k5_n_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvK5N : K5GemvT {
    override NCOL = true

    [spirv_kernel(local_size_x = 64)]
    def run {
        leaf_body()
    }
}


[vk_dispatch(name = "kq_gemv_k6_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvK6 : K6GemvT {
    [spirv_kernel(local_size_x = 64)]
    def run {
        leaf_body()
    }
}

[vk_dispatch(name = "kq_gemv_k6_n_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvK6N : K6GemvT {
    override NCOL = true

    [spirv_kernel(local_size_x = 64)]
    def run {
        leaf_body()
    }
}


[vk_dispatch(name = "kq_gemv_iq2s_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvIq2s : Iq2sGemvT {
    [spirv_kernel(local_size_x = 64)]
    def run {
        leaf_body()
    }
}

[vk_dispatch(name = "kq_gemv_iq2s_n_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvIq2sN : Iq2sGemvT {
    override NCOL = true

    [spirv_kernel(local_size_x = 64)]
    def run {
        leaf_body()
    }
}

[vk_dispatch(name = "kq_gemv_iq2s_gu_cls", kernel = "run_gu", family = "kq_gemv_k4_gu_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvIq2sGu : Iq2sGemvT {
    override GU = true

    [spirv_kernel(local_size_x = 256, name = "kq_gemv_iq2s_gu_cls_spv")]
    def run_gu {
        gu_shell()
    }
}


[vk_dispatch(name = "kq_gemv_iq2xs_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvIq2xs : Iq2xsGemvT {
    [spirv_kernel(local_size_x = 64)]
    def run {
        leaf_body()
    }
}

[vk_dispatch(name = "kq_gemv_iq2xs_n_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvIq2xsN : Iq2xsGemvT {
    override NCOL = true

    [spirv_kernel(local_size_x = 64)]
    def run {
        leaf_body()
    }
}


[vk_dispatch(name = "kq_gemv_iq2xxs_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvIq2xxs : Iq2xxsGemvT {
    [spirv_kernel(local_size_x = 64)]
    def run {
        leaf_body()
    }
}

[vk_dispatch(name = "kq_gemv_iq2xxs_n_cls", kernel = "run", family = "kq_gemv_cls", grid = "wgs", params = "wgs : int64")]
class KqGemvIq2xxsN : Iq2xxsGemvT {
    override NCOL = true

    [spirv_kernel(local_size_x = 64)]
    def run {
        leaf_body()
    }
}

[vk_dispatch(name = "kq_emb_k4_cls", kernel = "run", family = "kq_emb_cls", grid = "wgs", params = "wgs : int64")]
class KqEmbK4 : K4GemvT {
    override EMB = true

    [spirv_kernel(local_size_x = 64)]
    def run {
        leaf_body()
    }
}

[vk_dispatch(name = "kq_emb_k5_cls", kernel = "run", family = "kq_emb_cls", grid = "wgs", params = "wgs : int64")]
class KqEmbK5 : K5GemvT {
    override EMB = true

    [spirv_kernel(local_size_x = 64)]
    def run {
        leaf_body()
    }
}

[vk_dispatch(name = "kq_emb_k6_cls", kernel = "run", family = "kq_emb_cls", grid = "wgs", params = "wgs : int64")]
class KqEmbK6 : K6GemvT {
    override EMB = true

    [spirv_kernel(local_size_x = 64)]
    def run {
        leaf_body()
    }
}

[vk_dispatch(name = "kq_emb_q40_cls", kernel = "run", family = "kq_emb_cls", grid = "wgs", params = "wgs : int64")]
class KqEmbQ40 : Q40GemvT {
    override EMB = true

    [spirv_kernel(local_size_x = 64)]
    def run {
        leaf_body()
    }
}

[vk_dispatch(name = "kq_emb_iq4xs_cls", kernel = "run", family = "kq_emb_cls", grid = "wgs", params = "wgs : int64")]
class KqEmbIq4xs : Iq4xsGemvT {
    override EMB = true

    [spirv_kernel(local_size_x = 64)]
    def run {
        leaf_body()
    }
}

[vk_dispatch(name = "kq_emb_k3_cls", kernel = "run", family = "kq_emb_cls", grid = "wgs", params = "wgs : int64")]
class KqEmbK3 : K3GemvT {
    override EMB = true

    [spirv_kernel(local_size_x = 64)]
    def run {
        leaf_body()
    }
}

[vk_dispatch(name = "kq_emb_iq3s_cls", kernel = "run", family = "kq_emb_cls", grid = "wgs", params = "wgs : int64")]
class KqEmbIq3s : Iq3sGemvT {
    override EMB = true

    [spirv_kernel(local_size_x = 64)]
    def run {
        leaf_body()
    }
}

[vk_dispatch(name = "kq_emb_iq3xxs_cls", kernel = "run", family = "kq_emb_cls", grid = "wgs", params = "wgs : int64")]
class KqEmbIq3xxs : Iq3xxsGemvT {
    override EMB = true

    [spirv_kernel(local_size_x = 64)]
    def run {
        leaf_body()
    }
}

[vk_dispatch(name = "kq_emb_iq4nl_cls", kernel = "run", family = "kq_emb_cls", grid = "wgs", params = "wgs : int64")]
class KqEmbIq4nl : Iq4nlGemvT {
    override EMB = true

    [spirv_kernel(local_size_x = 64)]
    def run {
        leaf_body()
    }
}

[vk_dispatch(name = "kq_emb_k2_cls", kernel = "run", family = "kq_emb_cls", grid = "wgs", params = "wgs : int64")]
class KqEmbK2 : K2GemvT {
    override EMB = true

    [spirv_kernel(local_size_x = 64)]
    def run {
        leaf_body()
    }
}

[vk_dispatch(name = "kq_emb_iq2s_cls", kernel = "run", family = "kq_emb_cls", grid = "wgs", params = "wgs : int64")]
class KqEmbIq2s : Iq2sGemvT {
    override EMB = true

    [spirv_kernel(local_size_x = 64)]
    def run {
        leaf_body()
    }
}

[vk_dispatch(name = "kq_emb_iq2xs_cls", kernel = "run", family = "kq_emb_cls", grid = "wgs", params = "wgs : int64")]
class KqEmbIq2xs : Iq2xsGemvT {
    override EMB = true

    [spirv_kernel(local_size_x = 64)]
    def run {
        leaf_body()
    }
}

[vk_dispatch(name = "kq_emb_iq2xxs_cls", kernel = "run", family = "kq_emb_cls", grid = "wgs", params = "wgs : int64")]
class KqEmbIq2xxs : Iq2xxsGemvT {
    override EMB = true

    [spirv_kernel(local_size_x = 64)]
    def run {
        leaf_body()
    }
}

[vk_dispatch(name = "kq_emb_iq3s4_cls", kernel = "run", family = "kq_emb_cls", grid = "wgs", params = "wgs : int64")]
class KqEmbIq3s4 : Iq4xsGemvT {
    override IQODD = true
    override EMB = true

    [spirv_kernel(local_size_x = 64)]
    def run {
        leaf_body()
    }
}

//! the formats the token-table gather has no leaf for: q8 takes `EmbGather`, mx4 tags no table (an expert-only form)
let private EMB_NO_KQ_LEAF = "q8 mx4"

//! a token table in `fmt` gathers on the device: q8 through `EmbGather`, a superblock format through its `KqEmb<Fmt>` leaf, a
//! per-32 format through its `<Fmt>Emb` stamp - any slot `EMB_NO_KQ_LEAF` leaves an arm
[cold_path]
def emb_cls_fmt_ok(fmt : int) : bool {
    return true if (fmt == int(KqFmt.q8))
    return false if (fmt < 0 || fmt >= KQ_FMT_COUNT)
    return find(" {EMB_NO_KQ_LEAF} ", " {kq_fmt_of_slot(fmt)} ") < 0
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#vk-token-gather")]
def emb_cls_ensure(fmt : int) : bool {   // nolint:STYLE037 — kq_tile_stamp's generated arms, one a format's stamp
    return ensure_emb_gather_cls() if (fmt == int(KqFmt.q8))
    return false if (!emb_cls_fmt_ok(fmt))
    var r = false
    kq_tile_stamp(fmt, EMB_NO_KQ_LEAF, "emb") <| $() {
        r = tile_ensure_cls()
    }
    return r
}

//! the gather's set over a table plane pair `p`: its token ids `ids` (`ids_bytes`, hazard bit `ids_bit`), the host's staged rows
//! `xh` an `EMB_ID_HOST` row copies, and the rows out `y`; one layout serves every superblock leaf, q8 its own
[arch(at="../ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#vk-token-gather")]
def emb_cls_set(fmt : int; p : tuple<wq : uint64; ws : uint64; wqb : int64; wsb : int64>; ids : uint64; ids_bytes : int64; ids_bit : uint;
                xh : uint64; xh_bytes : int64; y : uint64; y_bytes : int64; ybit : uint) : VkDescriptorSet {
    if (fmt == int(KqFmt.q8)) {
        return set_emb_gather_cls(fixed_array(p.wq, p.ws, ids, y, xh), fixed_array(p.wqb, p.wsb, ids_bytes, y_bytes, xh_bytes),
            fixed_array(0u, 0u, ids_bit, ybit, 0u))
    }
    if (kq_block32(fmt)) {
        return set_b32_emb_cls(fixed_array(p.wq, p.ws, ids, ids, xh, y), fixed_array(p.wqb, p.wsb, ids_bytes, ids_bytes, xh_bytes, y_bytes),
            fixed_array(0u, 0u, ids_bit, ids_bit, 0u, ybit))
    }
    return set_kq_emb_cls(fixed_array(p.wq, p.ws, ids, ids, xh, y, kq_grid_dev()), fixed_array(p.wqb, p.wsb, ids_bytes, ids_bytes, xh_bytes, y_bytes, KQ_GRID_BYTES),
        fixed_array(0u, 0u, ids_bit, ids_bit, 0u, ybit, 0u))
}

//! the f32 gather's set over an F32/F16/BF16 table's exact rows `wf` (`wf_bytes`), the ids, the host's staged rows and the rows out
[arch(at="../ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#vk-token-gather")]
def emb_f32_set(wf : uint64; wf_bytes : int64; ids : uint64; ids_bytes : int64; ids_bit : uint; xh : uint64; xh_bytes : int64;
                y : uint64; y_bytes : int64; ybit : uint) : VkDescriptorSet
    => set_emb_gather_f32_cls(fixed_array(wf, ids, y, xh), fixed_array(wf_bytes, ids_bytes, y_bytes, xh_bytes), fixed_array(0u, ids_bit, ybit, 0u))

//! `npos` rows of `dim` gathered off the f32 table
[arch(at="../ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#vk-token-gather")]
def emb_f32_enc(raw : VkCommandBuffer; var h : VkHaz; var set_ : VkDescriptorSet; npos, dim : int64; escale : float) {
    var pce = EmbArgs(npos = uint(npos), dim = uint(dim), wblk0 = 0u, embed_scale = escale)
    enc_emb_gather_f32_cls(raw, h, set_, pce, (npos * dim / 4l + 255l) / 256l)
}

//! `npos` rows of `dim` gathered off the table at slab-local block `wblk0` (q8: 32-element blocks, a superblock format: superblocks)
[arch(at="../ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#vk-token-gather")]
def emb_cls_enc(fmt : int; raw : VkCommandBuffer; var h : VkHaz; var set_ : VkDescriptorSet; npos, dim, wblk0 : int64; escale : float) {
    if (fmt == int(KqFmt.q8)) {
        var pce = EmbArgs(npos = uint(npos), dim = uint(dim), wblk0 = uint(wblk0), embed_scale = escale)
        enc_emb_gather_cls(raw, h, set_, pce, (npos * dim / 4l + 255l) / 256l)
        return
    }
    var pc = GemvArgs(n = uint(dim), nreg = uint(npos), wblk0 = uint(wblk0), escale = escale)
    let wgs = (npos * (dim / KQ_B32_ELEMS) + 63l) / 64l   //! a thread a 32-block, 64 a workgroup
    kq_tile_stamp(fmt, EMB_NO_KQ_LEAF, "emb") <| $() {
        tile_enc_cls(raw, h, set_, pc, wgs)
    }
}

// ===== the tower chains' q8 GEMM tile: the pick ladder and the grid rule over the q8 batch and cm2 stamps =====

//! the q8 batch tile's workgroups over a (d, rows) GEMM
def vt_gemm_groups(d, rows : int64) : int64 {
    let te = batch_tile_edges(int(KqFmt.q8), d, rows)
    return ((d + te.td - 1l) / te.td) * ((rows + te.tc - 1l) / te.tc)
}

//! a GEMM site's tile: the cm2 token column on the f16 feed (tc > 0), else the q8 batch variant on the mul_mm tiles
struct VtTile {
    v : int
    tc : int64
}

def vt_tile(cm2 : bool; width, rows : int64) : VtTile {
    if (cm2) {
        return VtTile(v = 0, tc = cm2_tile_cols(width, rows))
    }
    return VtTile(v = q8_batch_cls_variant(q8_batch_tile_for(width, rows), q8_batch_aligned(width, rows)), tc = 0l)
}

def vt_tile_ensure(t : VtTile) : bool => t.tc > 0l ? cm2_cls_ensure(int(KqFmt.q8), t.tc) : q8_batch_cls_ensure(t.v)

def vt_tile_groups(t : VtTile; width, rows : int64) : int64 => t.tc > 0l ? cm2_tiles_at(width, rows, t.tc) : vt_gemm_groups(width, rows)

//! the rows a GEMM record carries: on the l column the encode's rounded up to the column, else the raw count
[arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def vt_tile_rows(t : VtTile; rows : int64) : int64 => t.tc == 256l ? round_up(rows, 256l) : rows

//! a tower GEMM tile's set: the q8 planes, the schedule meta, the feed the tile reads - the halves at `xh` on the cm2 tile,
//! else the Q8_0 quants and scales at `xq` / `xs` over `xq_elems` - its hazard bit, and the output
def vt_tile_cls_set(t : VtTile; wq, ws : uint64; wqb, wsb : int64; meta : uint64; meta_bytes : int64; xh : uint64; xh_bytes : int64;
                    xq, xs : uint64; xq_elems : int64; xbit : uint; y : uint64; y_bytes : int64; ybit : uint) : VkDescriptorSet {
    if (t.tc > 0l) {
        return cm2_cls_set(int(KqFmt.q8), t.tc, fixed_array(wq, ws, meta, xh, y), fixed_array(wqb, wsb, meta_bytes, xh_bytes, y_bytes),
            fixed_array(0u, 0u, 0u, xbit, ybit))
    }
    return q8_batch_cls_set(t.v, fixed_array(wq, ws, meta, xq, xs, y), fixed_array(wqb, wsb, meta_bytes, xq_elems, xq_elems / kq_elems(KqFmt.q8) * 4l, y_bytes),
        fixed_array(0u, 0u, 0u, xbit, xbit, ybit))
}

def vt_tile_enc(raw : VkCommandBuffer; var h : VkHaz; t : VtTile; var s : VkDescriptorSet; var pc : BatchArgs; groups : int64) {
    if (t.tc > 0l) {
        cm2_cls_enc(int(KqFmt.q8), t.tc, raw, h, s, pc, groups)
    } else {
        q8_batch_cls_enc(t.v, raw, h, s, pc, groups)
    }
}

//! a GEMM input's feed off its f32 rows: the clamp + halfword store on the cm2 feed, the clamp + Q8_0 requant on the mul_mm tiles
def vt_feed_enc(raw : VkCommandBuffer; var h : VkHaz; cm2 : bool; var s_rq : VkDescriptorSet; var pc_rq : RqArgs; nelem : int64) {
    if (cm2) {
        var pc = GkClampArgs(nelem = uint(nelem), lo = pc_rq.lo, hi = pc_rq.hi, scale = 1.0)
        enc_tower_clamp_cvt_cls(raw, h, s_rq, pc, (nelem / 4l + 255l) / 256l)
    } else {
        enc_tower_clamp_rq_cls(raw, h, s_rq, pc_rq, rq_wgs(false, nelem))
    }
}

//! the 1-D conv's im2col on the template both homes stamp: the f32 columns, the f32 tile's feed
[vk_dispatch(name = "tts_im2col_cls", kernel = "run", grid = "total/256", params = "total : int64")]
class TtsIm2col : GkIm2col1d {
    [spirv_kernel(local_size_x = 256, name = "tts_im2col_cls_spv")]
    def run {
        im2col1d_body()
    }
}

//! ... the columns as halves, the f16 tile's feed (the decoder's and generator's convs, the whisper-class stem)
[vk_dispatch(name = "tts_im2col16_cls", kernel = "run", grid = "total/1024", params = "total : int64")]
class TtsIm2col16 : GkIm2col1d {
    override HALF = true

    [spirv_kernel(local_size_x = 256, name = "tts_im2col16_cls_spv")]
    def run {
        im2col1d_body()
    }
}

struct TtsLstmArgs {
    t : uint          //! the steps
    hidden : uint     //! H, at most `TTS_LSTM_LANES`
    goff : uint       //! the input gates' element base: [t][4H] rows in gate order i, f, g, o
    gstride : uint    //! ... and their row stride (4H on the GEMM's 64 lattice)
    hhoff : uint      //! the recurrence's element base: w_hh transposed [H][4H], element (i, g H + j) = w_hh[(g H + j) H + i]
    hoff : uint       //! the h rows' element base
    hstride : uint    //! ... and their row stride: 2H where the two directions share a row
    backward : uint   //! 1 = the steps walk t - 1 down to 0
}

let TTS_LSTM_LANES = 256u   //! the hidden units a direction's workgroup holds (four invocations each): a direction past it declines at the driver

//! one LSTM direction end to end in one workgroup (the CPU `lstm_direction`, wgs = 1 a direction): four invocations a
//! hidden unit j, one a gate g, each taking its gate's input row (x w_ih^T + b, an F32GemmB ahead of it) plus the dot
//! of the transposed recurrence's column g H + j with h_prev over four accumulators (i mod 4, folded at the end, so a
//! step's loads pipeline; the CPU's `matmul` dot carries its own order, so no k order is the reference's) - a step's
//! lanes read consecutive floats - and the unit's first lane folding the four into c = f c + i g and h = o tanh(c); h
//! lives in `hs` between steps, c in that
//! lane. A direction over `TTS_LSTM_LANES` hidden units declines at the driver
[vk_dispatch(name = "tts_lstm_dir_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TtsLstmDir {
    @ssbo @binding = 0 gates : array<float>
    @ssbo @binding = 1 @role = "weight" whh : array<float>
    @ssbo @binding = 2 h : array<float>
    @push_constant pa : TtsLstmArgs
    @workgroup hs : float[int(TTS_LSTM_LANES)]
    @workgroup gs : float[int(TTS_LSTM_LANES) * 4]   //! a step's four gate dots a unit, the unit's lanes side by side

    [spirv_kernel(local_size_x = 1024)]
    def run {
        let lane = gl_LocalInvocationID.x
        let j = lane >> 2u
        let g = lane & 3u
        let hd = pa.hidden
        let live = j < hd
        let lead = live && g == 0u
        if (lead) {
            hs[j] = 0.0
        }
        var cst = 0.0
        let t_first = pa.backward != 0u ? pa.t - 1u : 0u
        let w4 = 4u * hd
        let col = pa.hhoff + g * hd + j
        var step = 0u
        while (step < pa.t) {
            barrier()
            var acc = 0.0
            if (live) {
                var a0 = 0.0
                var a1 = 0.0
                var a2 = 0.0
                var a3 = 0.0
                var i = 0u
                while (i + 3u < hd) {
                    a0 = mad(whh[col + i * w4], hs[i], a0)
                    a1 = mad(whh[col + (i + 1u) * w4], hs[i + 1u], a1)
                    a2 = mad(whh[col + (i + 2u) * w4], hs[i + 2u], a2)
                    a3 = mad(whh[col + (i + 3u) * w4], hs[i + 3u], a3)
                    i += 4u
                }
                while (i < hd) {
                    a0 = mad(whh[col + i * w4], hs[i], a0)
                    i++
                }
                acc = (a0 + a1) + (a2 + a3)
            }
            gs[lane] = acc
            barrier()
            if (lead) {
                let ti = pa.backward != 0u ? t_first - step : t_first + step
                let gb = pa.goff + ti * pa.gstride + j
                let gi = sigmoid_f32(gates[gb] + gs[lane])
                let gf = sigmoid_f32(gates[gb + hd] + gs[lane + 1u])
                let gg = tts_tanh(gates[gb + 2u * hd] + gs[lane + 2u])
                let go = sigmoid_f32(gates[gb + 3u * hd] + gs[lane + 3u])
                cst = gf * cst + gi * gg
                let hn = go * tts_tanh(cst)
                hs[j] = hn
                h[pa.hoff + ti * pa.hstride + j] = hn
            }
            step++
        }
    }
}

//! o [rows][width] = cat(x, res, f0, nd) - the CPU `concat_rows`
[vk_dispatch(name = "tts_concat_cls", kernel = "run", grid = "total/256", params = "total : int64")]
class TtsConcat : GkConcat {
    [spirv_kernel(local_size_x = 256)]
    def run {
        concat_body()
    }
}

//! the duration sigmoid sums - the CPU `styletts2_durations`' raw durations
[vk_dispatch(name = "tts_sigsum_cls", kernel = "run", grid = "t/256", params = "t : int64")]
class TtsSigSum : GkSigSum {
    [spirv_kernel(local_size_x = 256)]
    def run {
        sigsum_body()
    }
}

struct TtsStatsArgs {
    t : uint      //! the rows
    c : uint      //! the channels, x's row width
    nblk : uint   //! the blocks: ceil(t / TTS_STATS_ROWS)
    poff : uint   //! the block partials' element base in `part`: [nblk][c]
}

let TTS_STATS_ROWS = 128u   //! the rows a column-statistics block sums, the kernel body's literal

//! the block partials of the instance norm's column statistics of x [t][c]: workgroup `blk` sums rows blk rb .. + rb of every
//! channel, a lane a channel reading the rows as they lie (the lanes of a row consecutive) - the plain sum, or under SQ the
//! squared deviations from the mean `stats[ch]` holds; part[poff + blk c + ch]; wgs = nblk. The CPU `column_stats`'s two
//! passes, each over row blocks so every SM takes a share of the rows
[ |> template_struct_instance]
class template TtsColPartT {
    @template_constant SQ : bool = false
    @ssbo @binding = 0 x : array<float>
    @ssbo @binding = 1 @template_gate = SQ stats : array<float>
    @ssbo @binding = 2 part : array<float>
    @push_constant pa : TtsStatsArgs

    [spirv_kernel(local_size_x = 256)]
    def run {
        let blk = gl_WorkGroupID.x
        let r0 = blk * TTS_STATS_ROWS
        let r1 = min(r0 + TTS_STATS_ROWS, pa.t)
        var ch = gl_LocalInvocationID.x
        while (ch < pa.c) {
            var m = 0.0
            static_if (SQ) {
                m = stats[ch]
            }
            var s = 0.0
            var r = r0
            while (r < r1) {
                let v = x[r * pa.c + ch]
                static_if (SQ) {
                    let dv = v - m
                    s += dv * dv
                } else {
                    s += v
                }
                r++
            }
            part[pa.poff + blk * pa.c + ch] = s
            ch += 256u
        }
    }
}

[vk_dispatch(name = "tts_colpart_sum_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TtsColPartSum : TtsColPartT {}

[vk_dispatch(name = "tts_colpart_sq_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TtsColPartSq : TtsColPartT {
    override SQ = true
}

//! the partials folded a channel a lane: stats[ch] = the mean, or under SQ stats[c + ch] = the sum of squared deviations
[ |> template_struct_instance]
class template TtsColReduceT {
    @template_constant SQ : bool = false
    @ssbo @binding = 0 part : array<float>
    @ssbo @binding = 1 stats : array<float>
    @push_constant pa : TtsStatsArgs

    [spirv_kernel(local_size_x = 256)]
    def run {
        let ch = gl_GlobalInvocationID.x
        if (ch < pa.c) {
            var s = 0.0
            var blk = 0u
            while (blk < pa.nblk) {
                s += part[pa.poff + blk * pa.c + ch]
                blk++
            }
            static_if (SQ) {
                stats[pa.c + ch] = s
            } else {
                stats[ch] = s / float(pa.t)
            }
        }
    }
}

[vk_dispatch(name = "tts_colreduce_mean_cls", kernel = "run", grid = "c/256", params = "c : int64")]
class TtsColReduceMean : TtsColReduceT {}

[vk_dispatch(name = "tts_colreduce_sq_cls", kernel = "run", grid = "c/256", params = "c : int64")]
class TtsColReduceSq : TtsColReduceT {
    override SQ = true
}

//! the column statistics' blocks over `t` rows
def tts_stats_blocks(t : int64) : int64 => (t + int64(TTS_STATS_ROWS) - 1l) / int64(TTS_STATS_ROWS)

//! the stats rows a seat holds for `c_max` channels over `t_max` rows: the mean and squared-deviation rows, then the partials
def tts_stats_floats(t_max, c_max : int64) : int64 => 2l * c_max + tts_stats_blocks(t_max) * c_max

//! AdaIN then LeakyReLU(0.2) / Snake
[vk_dispatch(name = "tts_adain_leaky_cls", kernel = "run", grid = "total/256", params = "total : int64")]
class TtsAdainLeaky : GkAdain {
    [spirv_kernel(local_size_x = 256)]
    def run {
        adain_body()
    }
}

[vk_dispatch(name = "tts_adain_snake_cls", kernel = "run", grid = "total/256", params = "total : int64")]
class TtsAdainSnake : GkAdain {
    override SNAKE = true

    [spirv_kernel(local_size_x = 256)]
    def run {
        adain_body()
    }
}

//! the depthwise transposed pool on rows: y [t_out][c] gathers the rows of x [t_in][c] its taps reach - the CPU
//! `conv1d_rows_transposed_depthwise`
[vk_dispatch(name = "tts_pool_dw_cls", kernel = "run", grid = "total/256", params = "total : int64")]
class TtsPoolDw : GkDwConvRows {
    [spirv_kernel(local_size_x = 256)]
    def run {
        dw_conv_rows_body()
    }
}

//! the residual join's add-and-scale, in place where o is a
[vk_dispatch(name = "tts_add_scale_cls", kernel = "run", grid = "nelem/256", params = "nelem : int64")]
class TtsAddScale : GkAddScale {
    [spirv_kernel(local_size_x = 256)]
    def run {
        add_scale_body()
    }
}

//! o = (zero ? 0 : o) + scale * y over nelem elements - the generator stage's averaged residual-block sum
[vk_dispatch(name = "tts_axpy_cls", kernel = "run", grid = "nelem/256", params = "nelem : int64")]
class TtsAxpy : GkAxpy {
    [spirv_kernel(local_size_x = 256)]
    def run {
        axpy_body()
    }
}

//! the low-rate phase increments on either resample law
[vk_dispatch(name = "tts_src_low_torch_cls", kernel = "run", grid = "total/256", params = "total : int64")]
class TtsSrcLowTorch : GkSrcLow {
    override TORCH = true

    [spirv_kernel(local_size_x = 256)]
    def run {
        src_low_body()
    }
}

[vk_dispatch(name = "tts_src_low_onnx_cls", kernel = "run", grid = "total/256", params = "total : int64")]
class TtsSrcLowOnnx : GkSrcLow {
    [spirv_kernel(local_size_x = 256)]
    def run {
        src_low_body()
    }
}

//! the phase per frame on the torch law, wgs = 1; `precise` keeps the two-float sum's compensation
[vk_dispatch(name = "tts_src_cumsum_torch_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TtsSrcCumsumTorch : GkSrcCumsum {
    override TORCH = true

    [spirv_kernel(local_size_x = 256, precise = true)]
    def run {
        src_cumsum_body()
    }
}

[vk_dispatch(name = "tts_src_cumsum_onnx_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TtsSrcCumsumOnnx : GkSrcCumsum {
    [spirv_kernel(local_size_x = 256, precise = true)]
    def run {
        src_cumsum_body()
    }
}

//! the source's own noise draw
[vk_dispatch(name = "tts_src_noise_cls", kernel = "run", grid = "total/256", params = "total : int64")]
class TtsSrcNoise : GkSrcNoise {
    [spirv_kernel(local_size_x = 256)]
    def run {
        src_noise_body()
    }
}

//! the mixed source signal on either resample law
[vk_dispatch(name = "tts_src_sines_torch_cls", kernel = "run", grid = "n/256", params = "n : int64")]
class TtsSrcSinesTorch : GkSrcSines {
    override TORCH = true

    [spirv_kernel(local_size_x = 256)]
    def run {
        src_sines_body()
    }
}

[vk_dispatch(name = "tts_src_sines_onnx_cls", kernel = "run", grid = "n/256", params = "n : int64")]
class TtsSrcSinesOnnx : GkSrcSines {
    [spirv_kernel(local_size_x = 256)]
    def run {
        src_sines_body()
    }
}

//! the STFT on the reflect pad law
[vk_dispatch(name = "tts_stft_reflect_cls", kernel = "run", grid = "total/256", params = "total : int64")]
class TtsStftReflect : GkStft {
    override REFLECT = true

    [spirv_kernel(local_size_x = 256)]
    def run {
        stft_body()
    }
}

[vk_dispatch(name = "tts_stft_edge_cls", kernel = "run", grid = "total/256", params = "total : int64")]
class TtsStftEdge : GkStft {
    [spirv_kernel(local_size_x = 256)]
    def run {
        stft_body()
    }
}

//! the inverse STFT from conv_post's rows
[vk_dispatch(name = "tts_istft_cls", kernel = "run", grid = "n/256", params = "n : int64")]
class TtsIstft : GkIstft {
    [spirv_kernel(local_size_x = 256)]
    def run {
        istft_body()
    }
}

[vk_dispatch(name = "tts_pk_rows_cls", kernel = "run", grid = "total/256", params = "total : int64")]
class TtsPkRows : GkPkRows {
    [spirv_kernel(local_size_x = 256)]
    def run {
        pk_rows_body()
    }
}

[vk_dispatch(name = "tts_pk_rows_elu_cls", kernel = "run", grid = "total/256", params = "total : int64")]
class TtsPkRowsElu : GkPkRows {
    override ELU = true

    [spirv_kernel(local_size_x = 256)]
    def run {
        pk_rows_body()
    }
}

//! x [rows][d] *= the scale row at boff in place - a transformer layer's per-channel layer scale (the CPU
//! `layer_scale_rows`) on the bias stamp's row-broadcast shell: four elements an invocation, d a 4-multiple, `act` unread
[vk_dispatch(name = "tts_pk_row_scale_cls", kernel = "run", grid = "nelem/1024", params = "nelem : int64")]
class TtsPkRowScale : GkBiasAct {
    override SCALE = true

    [spirv_kernel(local_size_x = 256, name = "tts_pk_row_scale_cls_spv")]
    def run {
        bias_act_body()
    }
}

let TTS_PK_ATTN_HS = GK_PK_ATTN_HS
let TTS_PK_ATTN_MAX_KEYS = GK_PK_ATTN_MAX_KEYS

//! the Pocket chains' causal cached attention row - off `WgReduceBase` onto the template both homes stamp (`GkPkAttn`, which
//! derives the same reductions through `GkWgReduce`)
[vk_dispatch(name = "tts_pk_attn_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class TtsPkAttn : GkPkAttn {
    [spirv_kernel(local_size_x = 256)]
    def run {
        pk_attn_body()
    }
}

let TTS_PK_GEMV_MAX_NIN = GK_PK_GEMV_MAX_NIN

//! the Pocket frame's row GEMV on the template both homes stamp (`GkPkGemv`) over the slab's f32 rows, the inverse norm `1 / sqrt`
[ |> template_struct_instance]
class template TtsPkGemvT : GkPkGemv {
    [spirv_inline]
    def override row_dot_into(row, lane : uint; var acc : float&) {
        f32_row_dot_into(row, lane, acc)
    }

    [spirv_kernel(local_size_x = 128)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

[vk_dispatch(name = "tts_pk_gemv_dot_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class TtsPkGemvDot : TtsPkGemvT {}

[vk_dispatch(name = "tts_pk_gemv_ln_silu_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class TtsPkGemvLnSilu : TtsPkGemvT {
    override NORM_MOD_IN = true
    override MOD_IN = true
    override EPI = 1
}

[vk_dispatch(name = "tts_pk_gemv_gate_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class TtsPkGemvGate : TtsPkGemvT {
    override MOD_IN = true
    override EPI = 2
}

[vk_dispatch(name = "tts_pk_gemv_add_silu_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class TtsPkGemvAddSilu : TtsPkGemvT {
    override AUX_IN = true
    override EPI = 3
}

[vk_dispatch(name = "tts_pk_gemv_tail_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class TtsPkGemvTail : TtsPkGemvT {
    override NORM_MOD_IN = true
    override MOD_IN = true
    override AUX_IN = true
    override TAIL = true
    override EPI = 4
}

[vk_dispatch(name = "tts_pk_gemv_ln_qkv_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class TtsPkGemvLnQkv : TtsPkGemvT {
    override NORM_IN = true
    override ROPE_STORE = true
    override EPI = 5
}

[vk_dispatch(name = "tts_pk_gemv_add_scale_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class TtsPkGemvAddScale : TtsPkGemvT {
    override AUX_IN = true
    override EPI = 6
}

[vk_dispatch(name = "tts_pk_gemv_add_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class TtsPkGemvAdd : TtsPkGemvT {
    override EPI = 7
}

[vk_dispatch(name = "tts_pk_gemv_ln_gelu_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class TtsPkGemvLnGelu : TtsPkGemvT {
    override NORM_IN = true
    override EPI = 8
}

//! the Pocket chains' adjacent-pair rope over the q and k spans, the tables in a slab or the voice slot (cos_t and sin_t bind one buffer)
[vk_dispatch(name = "tts_pk_rope_cls", kernel = "run", grid = "pairs/256", params = "pairs : int64")]
class TtsPkRope : GkRopeTab {
    [spirv_kernel(local_size_x = 256)]
    def run {
        rope_tab_body()
    }
}


