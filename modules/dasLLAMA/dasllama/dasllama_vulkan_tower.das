options gen2
options indenting = 4
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_vulkan_tower shared public

require dasllama/dasllama_lint public
require vulkan
require math
require dasllama/dasllama_env
require dasllama/dasllama_kqformat
require dasllama/dasllama_math
require dasllama/dasllama_layout
require dasllama/dasllama_plane
require dasllama/dasllama_tower
require dasllama/dasllama_vulkan_common
require dasllama/dasllama_vulkan_classes
require dasllama/dasllama_gemma4v
require dasllama/dasllama_gemma3v
require dasllama/dasllama_qwen3v
require dasllama/dasllama_qwen25v
require dasllama/dasllama_audio
require dasllama/dasllama_gemma4a
require dasllama/dasllama_canary
require dasllama/dasllama_qwen3a
require dasllama/dasllama_convert

//! The tower Vulkan driver (the vision and audio encoders): one-shot encoder chains - no session, no KV, no mirror - that
//! serve a family's block loop dispatch for dispatch as the family's CPU loop spells it, over the
//! q8 image the CPU chain reads (qwen25v over its baked f16 twin). Every hook is best-effort: it
//! answers false on any shape, knob or device decline and the CPU chain serves that encode. Engage
//! is read from the counter deltas (`vulkan_tower_stats`), never from "the model ran". The Metal
//! driver fills the same slots on an Apple build, so this driver registers only where it is absent.

var private g_vt_env_tower = true
var private g_vt_encodes = 0l
var private g_vt_rows = 0l
var private g_vt_blocks = 0l
var private g_vt_convs = 0l   //! the conv fronts' dispatches served on the device: one per chunk (the whisper stem, gemma4a, qwen3a's chunks), canary's whole front once
var private g_vt_mels = 0l    //! the qwen3a log-mel fronts served on the device
var private g_vt_declines : VkDeclineCounter

enum private VulkanTowerDecline {
    knob         //! DASLLAMA_VK_TOWER=0, or set_vulkan_tower(false)
    blocks       //! set_vulkan_audio_blocks(false): the whisper-class block hooks pinned off while the stem still serves
    quant_mode   //! the tower's planes are not the lane the driver serves
    shape        //! a width, head size, row count or buffer range the stamps do not serve
    device       //! the tier has no device, or a class failed to build
    memory       //! a device or pinned allocation failed: the tower stays on the CPU chain
}

//! Engage / decline counters; `blocks` = transformer blocks the driver ran. Model-level gates assert DELTAS on these.
def vulkan_tower_stats : tuple<encodes : int64; rows : int64; blocks : int64; declines : int64; convs : int64; mels : int64> {
    return (encodes = g_vt_encodes, rows = g_vt_rows, blocks = g_vt_blocks, declines = g_vt_declines.total, convs = g_vt_convs, mels = g_vt_mels)
}

//! The declines-by-reason table (a copy), keyed by `VulkanTowerDecline` value names.
def vulkan_tower_declines : table<string; int64> {
    return <- clone_to_move(g_vt_declines.counts)
}

//! Test A/B seat: pin the tower driver on / off in-process (the env knob's setter twin).
def set_vulkan_tower(on : bool) {
    g_vt_env_tower = on
}

var private g_vt_env_audio_blocks = true

//! Test A/B seat: pin the whisper-class block hooks off while the conv stem still serves - the stem's device rows must land for the
//! CPU block loop (its `blocks` decline)
def set_vulkan_audio_blocks(on : bool) {
    g_vt_env_audio_blocks = on
}

def vulkan_tower_on : bool => g_vt_env_tower

//! The fronts' route: the audio fronts ride the f16 GEMM feed (cm2 or KHR); off it (the sdot4 mode) every front declines
//! `device` while the block chains still serve.
def vulkan_tower_front_route : bool => vt_cm2_feed()

[arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vulkan_tower_serves_exact() : bool => false

let private VT_KEY_SAMPLE_WORDS = 64l        //! words sampled from each plane region folded into the residency key beside its address and size
let VT_NO_CLAMP = 3.4028235e38 //! an inactive ClippableLinear bound: the CPU chain skips the clamp, the device clamps to the float range
let private VT_MAX_ENCODE_ROWS = 4096l
let private VT_GATHER_SLICE_ROWS = 64l   //! the cold q8 gather's parallel slice, where a region's rows divide by it
let private VT_WIN_HS = 80l              //! the window stamp's head width (`TowerWinAttnT` HS)

//! the hazard bits of the per-encode scratch (the weights and the schedule are unbit: the host writes them before the submit)
let private VT_X = 0x1u
let private VT_XB = 0x2u
let private VT_XQ = 0x4u
let private VT_Q = 0x8u
let private VT_K = 0x10u
let private VT_V = 0x20u
let private VT_Q2 = 0x40u
let private VT_K2 = 0x80u
let private VT_V2 = 0x100u
let private VT_K16 = 0x200u
let private VT_V16 = 0x400u
let private VT_ATT = 0x800u
let private VT_YO = 0x1000u
let private VT_HG = 0x2000u
let private VT_HU = 0x4000u
let private VT_YD = 0x8000u
let private VT_QKV = 0x10000u
let private VT_TAP = 0x20000u
let private VT_REL_PROJ = 0x40000u
let private VT_REL_TABQ = 0x80000u
let private VT_CN_QV_PANEL = 0x100000u   //! canary's rel plane: the (q + v) head panel, the rel-table head panel, the plane itself
let private VT_CN_REL_PANEL = 0x200000u
let private VT_CN_REL_PLANE = 0x400000u
let private VT_REL_TAB = 0x800000u
let private VT_TAIL_OUT = 0x8000000u       //! gemma4a's projector tail: the out-proj rows, the projected rows
let private VT_TAIL_PROJ = 0x10000000u
let private VT_SPLITK_PLANES = 0x20000000u      //! the pre-LN chain's split-k partial planes (the fc2 GEMM's k chunks before the reduce)
let private VT_SPLITK_MAX_PLANES = 8l   //! the most k chunks a chain GEMM splits into - the pick's own range
let private VT_SK_MIN_ROWS = 512l       //! the rows under which no split serves: a 128-row window reads 5% slower split (PERF_LEDGER.md's split-k row)
let private VT_MEL_FLOOR = 5.960464477539063e-08   //! 2^-24, the whisper-class log-mel's floor (the CPU fronts' own)
let private VT_LOG10_E = 0.43429448190325176       //! log10(e): the whisper preprocessors' log-mel scales the natural log to log10

//! the q8 batch sets of one variant: the d-wide outputs (q, k, v, o, down) and the ff-wide ones (gate, up)
struct private VtBatchSetsD {
    s_q : VkDescriptorSet
    s_k : VkDescriptorSet
    s_v : VkDescriptorSet
    s_o : VkDescriptorSet
    s_down : VkDescriptorSet
}

struct private VtBatchSetsFf {
    s_hg : VkDescriptorSet
    s_hu : VkDescriptorSet
}

struct private VtResident {
    key : uint64
    wq_dev : uint64
    ws_dev : uint64
    wq_bytes : int64
    ws_bytes : int64
    norms_dev : uint64
    norms_bytes : int64
    ones_off : int64      //! the ones row's element offset in norms_dev
    meta : HostBuf
    meta_bytes : int64
    cap_rows : int64      //! the scratch's row capacity (0 = none yet)
    d : int64
    ff : int64
    hs : int64            //! the head width the rope tables are sized for
    x_dev : uint64
    xb_dev : uint64
    xq_dev : uint64
    xs_dev : uint64
    q_dev : uint64
    k_dev : uint64
    v_dev : uint64
    q2_dev : uint64
    k2_dev : uint64
    v2_dev : uint64
    k16_dev : uint64
    v16_dev : uint64
    att_dev : uint64
    yo_dev : uint64
    hg_dev : uint64
    hu_dev : uint64
    yd_dev : uint64
    tabs_dev : uint64
    pad_dp : int64        //! the padded attention width when the heads restride to the flash tile's width (0 = none)
    qp_dev : uint64
    kp16_dev : uint64
    vp16_dev : uint64
    attp_dev : uint64
    fused : bool          //! the fused [q | k | v] GEMM's rows and the per-token rope tables (qwen3v)
    qkv_dev : uint64
    cos_dev : uint64
    sin_dev : uint64
    taps : int64          //! deepstack tap stashes: the residual after each tap block, read back with x
    xtap_dev : uint64
    f16w : bool           //! the halfword-twin family (qwen25v): the f16 feed rows and the window table
    xh_dev : uint64       //! the f16 feed, [cap x max(d, ff)] (the halfword twin), or [cap + TILE_READ_SLACK x max(d, ff)] on the cm2 feed
    cm2 : bool            //! the q8 chains' GEMMs ride the cm2 f16 feed (a coopmat device): xh_dev is the feed
    sk_dev : uint64       //! the split-k partial planes, [nsplit x cap x d] floats, made at the first pick that splits and grown to the widest
    sk_bytes : int64
    @scratch sets_sk : table<int; VkDescriptorSet>   //! the fc2 GEMM's set into the planes per token column
    s_splitk_reduce : VkDescriptorSet                       //! the planes' sum into the fc2 rows
    front : bool          //! a conv front's resident (qwen3a, gemma4a): the f16 conv weight slab beside the q8 planes, the front scratch below
    fw16_dev : uint64     //! the conv stages' weights as f16 rows padded to each stage's kdp columns
    fw16_bytes : int64
    front_twiddles_dev : uint64      //! gemma4a: the DFT twiddles and the mel filterbank, uploaded with the slab
    front_filters_dev : uint64
    fnorm_dev : uint64    //! the whisper-class stem: conv1's bias, conv2's and the position rows, uploaded with the slab
    fnorm_bytes : int64
    front_cap : int64   //! the front scratch's capacity (qwen3a: chunks; gemma4a: frames), 0 = none yet
    fxin_dev : uint64
    fcol_dev : uint64
    fy1_dev : uint64
    fy2_dev : uint64
    fy3_dev : uint64
    fmel_dev : uint64
    ffeat_dev : uint64
    fx_dev : uint64
    fcn_pw3_dev : uint64      //! canary: the second pointwise stage's rows, the third depthwise stage's, the feature image and the f16 feed
    fcn_dw5_dev : uint64
    fcn_feat_dev : uint64
    fcn_feed16_dev : uint64
    fh_bytes : int64
    s_fcn_feed16_dw5 : VkDescriptorSet   //! canary: the third stage's f16 feed, the feature rows' f16 feed, the second pointwise bias
    s_fcn_feed16_feat : VkDescriptorSet
    s_fcn_bias_pw6 : VkDescriptorSet
    s_fim : VkDescriptorSet[3]
    s_fg : VkDescriptorSet[3]
    s_fb : VkDescriptorSet[3]
    s_fshuf : VkDescriptorSet
    s_ffin : VkDescriptorSet
    s_fdft : VkDescriptorSet
    s_fmel : VkDescriptorSet
    s_fcvt : VkDescriptorSet
    fhost : HostBuf
    fmeta : HostBuf       //! the front's own schedule (gemma4a: the input projection's record and map)
    fmeta_bytes : int64
    @scratch sets_front : table<int; VkDescriptorSet>   //! the front's own cm2 GEMM set per token column (the conv_out / input projection)
    rel_cap_rows : int64      //! the rel quartet's rows (gemma4a: past + 1, uploaded with the tower; canary: 2 x cap, built on the device per encode); 0 = no table
    rel_tab_dev : uint64
    rel_tabq_dev : uint64
    rel_tabh_dev : uint64
    rel_tabs_dev : uint64
    rel_proj_dev : uint64
    wlo_dev : uint64      //! the windows' row starts (nwin + 1), the f32 per-window route's table
    rex_dev : uint64      //! the biased f16 act's row -> expert map at zero (the tower's bias rows are the plane's first): cap_rows words
    win_cap : int64       //! the window table's capacity (windows)
    host : HostBuf        //! the x readback staging (the tap stashes behind it)
    oom : bool            //! a soft allocation of the batch under way failed
    bound_d : int         //! the tile key (`vt_tile_key`) the bound d-wide GEMM sets were built for (-1 = none)
    bound_ff : int        //! the tile key of the bound ff-wide sets (gate, up): the tile pick reads the output width
    bound_qkv : int       //! the tile key of the bound fused qkv set (3d wide)
    @scratch sets_d : table<int; VtBatchSetsD>     //! the GEMM sets per tile key: built once at the scratch's capacity, rebound on a tile switch
    @scratch sets_ff : table<int; VtBatchSetsFf>
    @scratch sets_qkv : table<int; VkDescriptorSet>
    @scratch sets_rel_proj : table<int; VkDescriptorSet>   //! the rel-table projection's set per tile key (its feed the table's own rows)
    family : string       //! the family name the counter and the announce carry
    served_said : bool    //! the serves-on-the-device line said for the model the key names
    s_qkv : VkDescriptorSet
    s_bias_qkv : VkDescriptorSet
    s_rope_tq : VkDescriptorSet
    s_rope_tk : VkDescriptorSet
    s_cvt_xb : VkDescriptorSet
    s_cvt_att : VkDescriptorSet
    s_win : VkDescriptorSet
    s_glu : VkDescriptorSet
    s_q : VkDescriptorSet
    s_k : VkDescriptorSet
    s_v : VkDescriptorSet
    s_o : VkDescriptorSet
    s_hg : VkDescriptorSet
    s_hu : VkDescriptorSet
    s_down : VkDescriptorSet
    s_rms : VkDescriptorSet
    s_rq_xb : VkDescriptorSet
    s_rq_att : VkDescriptorSet
    s_rq_hg : VkDescriptorSet
    s_clamp_q : VkDescriptorSet
    s_clamp_k : VkDescriptorSet
    s_clamp_v : VkDescriptorSet
    s_clamp_yo : VkDescriptorSet
    s_clamp_yd : VkDescriptorSet
    s_hrms_q : VkDescriptorSet
    s_hrms_k : VkDescriptorSet
    s_hrms_v : VkDescriptorSet
    s_rope_q : VkDescriptorSet
    s_rope_k : VkDescriptorSet
    s_cvt_k : VkDescriptorSet
    s_cvt_v : VkDescriptorSet
    s_fa : VkDescriptorSet
    s_post_o : VkDescriptorSet
    s_post_d : VkDescriptorSet
    s_post_o_plain : VkDescriptorSet
    s_geglu : VkDescriptorSet
    s_ln : VkDescriptorSet
    s_bias_q : VkDescriptorSet
    s_bias_k : VkDescriptorSet
    s_bias_v : VkDescriptorSet
    s_bias_hg : VkDescriptorSet
    s_pad_q : VkDescriptorSet
    s_pad_k : VkDescriptorSet
    s_pad_v : VkDescriptorSet
    s_unpad : VkDescriptorSet
    s_post_o_ln : VkDescriptorSet
    s_post_d_ln : VkDescriptorSet
    s_rms_xb : VkDescriptorSet
    s_rms_yd : VkDescriptorSet
    s_rms_x : VkDescriptorSet
    s_rq_yd : VkDescriptorSet
    s_rq_rel_tab : VkDescriptorSet
    s_clamp_hg : VkDescriptorSet
    s_silu_hg : VkDescriptorSet
    s_silu_yd : VkDescriptorSet
    s_glu_sig : VkDescriptorSet
    s_dw : VkDescriptorSet
    s_attn : VkDescriptorSet
    s_ln_x : VkDescriptorSet
    s_cn_dw : VkDescriptorSet
    cn_plane_rows : int64        //! canary's rel plane capacity (rows; 0 = none): the (q + v) and rel-table head panels and the plane R = (q + v) P^T
    cn_qh_dev : uint64
    cn_ph_dev : uint64
    cn_r_dev : uint64
    s_cn_gq : VkDescriptorSet
    s_cn_gp : VkDescriptorSet
    s_cn_rg : VkDescriptorSet
    s_cn_attn_r : VkDescriptorSet
    s_cn_rel_sincos : VkDescriptorSet   //! the sinusoid rel table's writer
    x_ready : bool
    x_pend_ptr : float?       //! the stem's rows still on the device (`x_dev`) and the host array they are owed to; 0 bytes = none pending
    x_pend_bytes : int64
    tail_cap : int64          //! the projector tail's row capacity (gemma4a; 0 = none): the out-proj rows, the projected rows, their readback
    tail_dim : int64
    tail_out_dev : uint64
    tail_proj_dev : uint64
    tail_host : HostBuf
    s_feed_x : VkDescriptorSet
    s_feed_pt : VkDescriptorSet
    s_bias_pt : VkDescriptorSet
    s_rms_pt : VkDescriptorSet
    s_ln16 : VkDescriptorSet
    s_post_o_ln16 : VkDescriptorSet
    s_post_d_ln16 : VkDescriptorSet
    s_pad_qb : VkDescriptorSet
    s_pad_kb : VkDescriptorSet
    s_pad_vb : VkDescriptorSet
    s_unpad16 : VkDescriptorSet
    s_bias_hg16 : VkDescriptorSet
    stem_kdp : int64          //! the whisper-class stem's padded conv1 column width (3 x n_mel rounded up to 64)
    s_stem_pos_add : VkDescriptorSet   //! the stem's position rows' add
    fa_khr : bool
    gemm_khr : bool       //! the f16 GEMM arm the f16 sets were built for (qwen25v)
}

var private g_vt_g4v : VtResident   //! the gemma4v family's resident tower (one a family: a second mmproj replaces it)
var private g_vt_g3v : VtResident
var private g_vt_q3v : VtResident
var private g_vt_q25v : VtResident
var private g_vt_aud : VtResident   //! the whisper-class audio towers' resident (one seat: a second tower replaces it)

var private g_vt_g4a : VtResident
var private g_vt_cn : VtResident
var private g_vt_q3a_front : VtResident   //! the qwen3a conv front's resident: the conv_out q8 plane, the f16 conv slab, the chunk scratch
var private @scratch @exact_size g_vt_rope2d_tabs : array<float>   //! the packed rope tables staging, [row][cx | sx | cy | sy]
var private @scratch @exact_size g_vt_maps : array<uint>           //! the encode's schedule map offsets, one a GEMM record
var private @scratch @exact_size g_vt_wlo : array<uint>            //! the window table staging (nwin + 1)
var private @scratch @exact_size g_vt_ln_offs : array<LayerOffs>       //! the pre-LN chain's block offsets, one record a block

[cold_path]
def private vt_decline_words(why : VulkanTowerDecline) : string {
    if (why == VulkanTowerDecline.knob) {
        return "the tower knob is off (DASLLAMA_VK_TOWER=0 or set_vulkan_tower(false))"
    } elif (why == VulkanTowerDecline.blocks) {
        return "the whisper-class block hooks are pinned off (set_vulkan_audio_blocks(false)); the stem still serves"
    } elif (why == VulkanTowerDecline.quant_mode) {
        return "the tower's planes are not the lane the driver serves"
    } elif (why == VulkanTowerDecline.shape) {
        return "a width, head size, row count, window or buffer range the stamps do not serve"
    } elif (why == VulkanTowerDecline.memory) {
        return "a device allocation failed"
    }
    return "the tier has no device (DASLLAMA_GPU=1 arms it), or a class failed to build"
}

//! the decline counted under the enum's value name, and said once per reason per tower in words (the counter's `note`)
[cold_path]
def private vt_decline(r : VtResident; why : VulkanTowerDecline) : bool
    => g_vt_declines |> note("the {r.family} tower", "{why}", "the tower stays on the CPU chain: {vt_decline_words(why)}")

[cold_path]
def private vt_announce(var r : VtResident) {
    if (r.served_said) {
        return
    }
    r.served_said = true
    to_log(LOG_INFO, "dasLLAMA vulkan tower: {r.family} serves on the device (DASLLAMA_GPU=1 arms the tier, DASLLAMA_VK_TOWER=0 stands it down)\n")
}

def private vt_unbind_batch_sets(var r : VtResident) {
    r.bound_d = -1
    r.bound_ff = -1
    r.bound_qkv = -1
}

let private VT_TILE_CM2_KEY_BASE = 1000   //! the cm2 tiles' key base in the set caches, past the q8 batch variants' 0..5 (a tile's key adds its token column)

def private vt_tile_key(t : VtTile) : int => t.tc > 0l ? VT_TILE_CM2_KEY_BASE + int(t.tc) : t.v

//! `k` folded with `VT_KEY_SAMPLE_WORDS` words sampled evenly over the `bytes` at `p`
def private vt_fold_words(k : uint64; p : uint8 const?; bytes : int64) : uint64 {
    var h = k
    let n = bytes / 4l
    let step = max(n / VT_KEY_SAMPLE_WORDS, 1l)
    let wp = unsafe(reinterpret<uint const?>(p))
    var i = 0l
    while (i < n) {
        h = hash_combine64(h, uint64(unsafe(wp[i])))
        i += step
    }
    return h
}

def private vt_fold_addr(k : uint64; p : void?) : uint64 => hash_combine64(k, intptr(p))

def private vt_fold_int(k : uint64; v : int64) : uint64 => hash_combine64(k, uint64(v))

[arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vt_key_f16(wblob : PlaneU16; w_off, w_n : int64; blob : PlaneF; rows_off, rows_n, n_layer : int64) : uint64 {
    var k = intptr(unsafe(reinterpret<void?>(wblob.p))) * 0x9E3779B97F4A7C15ul + uint64(wblob.n) + uint64(n_layer) * 0x100000001B3ul
    k = vt_fold_int(k, w_off)
    k = vt_fold_int(k, w_n)
    k = vt_fold_addr(k, unsafe(reinterpret<void?>(blob.p)))
    k = vt_fold_int(k, rows_off)
    k = vt_fold_int(k, rows_n)
    k = vt_fold_words(k, unsafe(reinterpret<uint8 const?>(plane_at(wblob, w_off))), w_n * 2l)
    k = vt_fold_words(k, unsafe(reinterpret<uint8 const?>(plane_at(blob, rows_off))), rows_n * 4l)
    return k
}

[arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def vt_key(qblob : PlaneI8; qscales : PlaneF; blob : PlaneF; rows_off, rows_n, ones_n, n_layer : int64; mr, kgroup, wbias : int64) : uint64 {
    var k = intptr(unsafe(reinterpret<void?>(qblob.p))) * 0x9E3779B97F4A7C15ul + uint64(qblob.n) + uint64(qscales.n) * 0x100000001B3ul + uint64(n_layer) * 0x9E3779B1ul
    k = vt_fold_addr(k, unsafe(reinterpret<void?>(qscales.p)))
    k = vt_fold_addr(k, unsafe(reinterpret<void?>(blob.p)))
    k = vt_fold_int(k, rows_off)
    k = vt_fold_int(k, rows_n)
    k = vt_fold_int(k, ones_n)
    k = vt_fold_int(k, mr)
    k = vt_fold_int(k, kgroup)
    k = vt_fold_int(k, wbias)
    k = vt_fold_words(k, unsafe(reinterpret<uint8 const?>(qblob.p)), qblob.n)
    k = vt_fold_words(k, unsafe(reinterpret<uint8 const?>(qscales.p)), qscales.n * 4l)
    k = vt_fold_words(k, unsafe(reinterpret<uint8 const?>(plane_at(blob, rows_off))), rows_n * 4l)
    return k
}

//! the soft allocations: a failure marks the resident, and the batch's caller releases what it got and declines `memory`
[cold_path]
def private vt_dev_buf(var r : VtResident; bytes : int64) : uint64 => soft_device_buf(r.oom, bytes)

[cold_path]
def private vt_host_buf(var r : VtResident; bytes : int64; cached : bool) : HostBuf => soft_host_buf(r.oom, bytes, cached)

[arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vt_cap_rows(rows : int64) : int64 => (rows + 255l) / 256l * 256l

//! the largest buffer the scratch at `cap` rows binds: the range decline reads it against the device's limit
def private vt_scratch_max_bytes(cap, d, ff, dp, taps : int64; fused, f16w : bool) : int64 {
    let bx = cap * d * 4l
    var b = max(cap * max(d, ff) * 4l, cap * dp * 4l)
    if (fused) {
        b = max(b, 3l * bx)
    }
    b = max(b, (1l + taps) * bx)
    if (f16w) {
        b = max(b, cap * max(d, ff) * 2l)
    }
    if (vt_cm2_feed()) {
        b = max(b, (cap + TILE_READ_SLACK) * max(d, ff) * 2l)
    }
    return b
}

//! the q8 chains' GEMM feed: on a coopmat device the cm2 (or KHR) f16 tiles the prefill serves dense q8 GEMMs with, else the mul_mm tiles
def vt_cm2_feed : bool => g_gpu.coopmat_mode >= COOPMAT_MM

def vt_range_ok(bytes : int64) : bool => bytes <= vk_max_storage_range()

//! the front scratch released on its own; the block chain's scratch rebuild drops it too, and it regrows on demand
[cold_path]
def private vt_front_release(var r : VtResident) {
    if (r.front_cap > 0l) {
        for (b in fixed_array(r.fxin_dev, r.fcol_dev, r.fy1_dev, r.fy2_dev, r.fy3_dev, r.fmel_dev, r.ffeat_dev, r.fx_dev, r.fcn_pw3_dev, r.fcn_dw5_dev, r.fcn_feat_dev, r.fcn_feed16_dev)) {
            destroy_device_buf(b)
        }
        destroy_host_buf(r.fhost)
        if (r.fmeta_bytes > 0l) {
            destroy_host_buf(r.fmeta)
            r.fmeta_bytes = 0l
        }
        r.front_cap = 0l
    }
    r.sets_front |> clear()
}

[cold_path]
def private vt_release_scratch(var r : VtResident) {
    if (r.cap_rows == 0l) {
        vt_front_release(r)
        return
    }
    for (b in fixed_array(r.x_dev, r.xb_dev, r.xq_dev, r.xs_dev, r.q_dev, r.k_dev, r.v_dev, r.q2_dev, r.k2_dev, r.v2_dev,
            r.k16_dev, r.v16_dev, r.att_dev, r.yo_dev, r.hg_dev, r.hu_dev, r.yd_dev, r.tabs_dev)) {
        destroy_device_buf(b)
    }
    if (r.pad_dp > 0l) {
        for (b in fixed_array(r.qp_dev, r.kp16_dev, r.vp16_dev, r.attp_dev)) {
            destroy_device_buf(b)
        }
    }
    if (r.fused) {
        for (b in fixed_array(r.qkv_dev, r.cos_dev, r.sin_dev)) {
            destroy_device_buf(b)
        }
    }
    if (r.taps > 0l) {
        destroy_device_buf(r.xtap_dev)
    }
    if (r.f16w) {
        destroy_device_buf(r.xh_dev)
        destroy_device_buf(r.wlo_dev)
        destroy_device_buf(r.rex_dev)
    }
    if (r.cm2) {
        destroy_device_buf(r.xh_dev)
    }
    vt_sk_release(r)
    vt_front_release(r)
    if (r.cn_plane_rows > 0l) {
        for (b in fixed_array(r.cn_qh_dev, r.cn_ph_dev, r.cn_r_dev)) {
            destroy_device_buf(b)
        }
    }
    vt_tail_release(r)
    destroy_host_buf(r.host)
    if (r.meta_bytes > 0l) {
        destroy_host_buf(r.meta)
    }
    vt_forget_scratch(r)
}

//! the scratch's marks cleared, nothing destroyed: the release calls it after its destroys, the forget path (the device's sweep freed
//! the objects) alone; the stem's pending rows and the encoder-output handoff go with the planes they name
[cold_path]
def private vt_forget_scratch(var r : VtResident) {
    vt_x_drop_pending(r)
    g_vt_aud_out_bytes = 0l
    r.cap_rows = 0l
    r.pad_dp = 0l
    r.fused = false
    r.taps = 0l
    r.f16w = false
    r.win_cap = 0l
    r.meta_bytes = 0l
    r.front_cap = 0l
    r.x_ready = false
    r.cm2 = false
    r.sk_bytes = 0l
    r.tail_cap = 0l
    r.tail_dim = 0l
    r.cn_plane_rows = 0l
    r.sets_d |> clear()
    r.sets_ff |> clear()
    r.sets_qkv |> clear()
    r.sets_rel_proj |> clear()
    r.sets_sk |> clear()
    r.sets_front |> clear()
    vt_unbind_batch_sets(r)
}

[cold_path]
def private vt_release(var r : VtResident) {
    vt_release_scratch(r)
    if (r.key != 0ul) {
        destroy_device_buf(r.wq_dev)
        destroy_device_buf(r.ws_dev)
        destroy_device_buf(r.norms_dev)
        vt_rel_release(r)
        if (r.front) {
            for (b in fixed_array(r.fw16_dev, r.front_twiddles_dev, r.front_filters_dev, r.fnorm_dev)) {
                destroy_device_buf(b)
            }
            r.front = false
        }
        r.key = 0ul
    }
}

//! Release every device object the driver owns (the tests' leak rail beside the tier's shutdown); the next served encode
//! uploads again.
def vulkan_tower_shutdown {
    vt_release(g_vt_g4v)
    vt_release(g_vt_g3v)
    vt_release(g_vt_q3v)
    vt_release(g_vt_q25v)
    vt_release(g_vt_aud)
    vt_release(g_vt_g4a)
    vt_release(g_vt_cn)
    vt_release(g_vt_q3a_front)
    vt_release(g_vt_q3am)
}

[cold_path]
def private vt_forget_one(var r : VtResident) {
    vt_forget_scratch(r)
    r.key = 0ul
    r.rel_cap_rows = 0l
    r.front = false
    r.oom = false
    r.served_said = false
}

[cold_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vt_rel_bufs(var r : VtResident; rows, d : int64) : bool {
    if (r.rel_cap_rows >= rows) {
        return true
    }
    vt_rel_release(r)
    let q8e = kq_elems(KqFmt.q8)
    r.rel_tab_dev = vt_dev_buf(r, rows * d * 4l)
    r.rel_tabq_dev = vt_dev_buf(r, rows * d)
    r.rel_tabs_dev = vt_dev_buf(r, rows * d / q8e * 4l)
    r.rel_tabh_dev = vt_dev_buf(r, (rows + TILE_READ_SLACK) * d * 2l)
    r.rel_proj_dev = vt_dev_buf(r, rows * d * 4l)
    if (r.oom) {
        r.rel_cap_rows = rows
        vt_rel_release(r)
        return false
    }
    r.rel_cap_rows = rows
    return true
}

[cold_path]
def private vt_rel_release(var r : VtResident) {
    if (r.rel_cap_rows > 0l) {
        for (b in fixed_array(r.rel_tab_dev, r.rel_tabq_dev, r.rel_tabs_dev, r.rel_tabh_dev, r.rel_proj_dev)) {
            if (b != 0ul) {
                destroy_device_buf(b)
            }
        }
        r.rel_tab_dev = 0ul
        r.rel_tabq_dev = 0ul
        r.rel_tabs_dev = 0ul
        r.rel_tabh_dev = 0ul
        r.rel_proj_dev = 0ul
        r.rel_cap_rows = 0l
    }
    r.sets_rel_proj |> clear()
}

[arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vt_rel_proj_set(var r : VtResident; t : VtTile) : VkDescriptorSet {
    let key = vt_tile_key(t)
    if (!key_exists(r.sets_rel_proj, key)) {
        r.sets_rel_proj[key] = vt_rel_proj_build(r, t)
    }
    return r.sets_rel_proj[key]
}

[cold_path]
def private vt_rel_proj_build(r : VtResident; t : VtTile) : VkDescriptorSet {
    let b_relq = r.rel_cap_rows * r.d
    let b_rel = b_relq * 4l
    if (t.tc > 0l) {
        let b_relh = (r.rel_cap_rows + TILE_READ_SLACK) * r.d * 2l
        return cm2_cls_set(int(KqFmt.q8), t.tc, fixed_array(r.wq_dev, r.ws_dev, r.meta.buf, r.rel_tabh_dev, r.rel_proj_dev),
            fixed_array(r.wq_bytes, r.ws_bytes, r.meta_bytes, b_relh, b_rel), fixed_array(0u, 0u, 0u, VT_REL_TABQ, VT_REL_PROJ))
    }
    return q8_batch_cls_set(t.v, fixed_array(r.wq_dev, r.ws_dev, r.meta.buf, r.rel_tabq_dev, r.rel_tabs_dev, r.rel_proj_dev),
        fixed_array(r.wq_bytes, r.ws_bytes, r.meta_bytes, b_relq, b_relq / kq_elems(KqFmt.q8) * 4l, b_rel), fixed_array(0u, 0u, 0u, VT_REL_TABQ, VT_REL_TABQ, VT_REL_PROJ))
}

def private vt_conformer_sets(var r : VtResident; v, vff, v2d, v_rel : VtTile) : tuple<s_h2 : VkDescriptorSet; s_rel : VkDescriptorSet> {
    vt_bind_tile_sets(r, v, vff)
    return (s_h2 = vt_sets_ff(r, v2d).s_hg, s_rel = vt_rel_proj_set(r, v_rel))
}

[arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vt_forget {
    vt_forget_one(g_vt_g4v)
    vt_forget_one(g_vt_g3v)
    vt_forget_one(g_vt_q3v)
    vt_forget_one(g_vt_q25v)
    vt_forget_one(g_vt_aud)
    vt_forget_one(g_vt_g4a)
    vt_forget_one(g_vt_cn)
    vt_forget_one(g_vt_q3a_front)
    vt_forget_one(g_vt_q3am)
    g_vt_ln_offs |> clear()
}

//! a new model's upload begins: the old resident released, the per-model log state cleared
[cold_path]
def private vt_upload_begin(var r : VtResident; key : uint64) {
    vt_release(r)
    r.key = key
    r.oom = false
    r.served_said = false
    g_vt_declines |> unsay("the {r.family} tower")
}

[cold_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vt_upload_f16(var r : VtResident; key : uint64; wblob : PlaneU16; w_off, w_n : int64; blob : PlaneF; rows_off, rows_n : int64) : bool {
    vt_upload_begin(r, key)
    r.wq_bytes = w_n * 2l
    r.ws_bytes = 0l
    r.wq_dev = vt_dev_buf(r, r.wq_bytes)
    r.ws_dev = vt_dev_buf(r, 64l)   // no scale plane on this family: a stub the release frees like the others
    r.ones_off = 0l
    r.norms_bytes = rows_n * 4l
    r.norms_dev = vt_dev_buf(r, r.norms_bytes)
    if (r.oom) {
        vt_release(r)
        return false
    }
    upload_region_at(r.wq_dev, 0l, unsafe(reinterpret<uint8 const?>(plane_at(wblob, w_off))), r.wq_bytes)
    upload_region_at(r.norms_dev, 0l, unsafe(reinterpret<uint8 const?>(plane_at(blob, rows_off))), r.norms_bytes)
    r.cap_rows = 0l
    vt_unbind_batch_sets(r)
    return true
}

def private vt_scratch_f16_fits(r : VtResident; rows, nwin, d, ff, dp : int64; khr, gemm_khr : bool) : bool
    => (r.cap_rows >= vt_cap_rows(rows) && r.win_cap >= nwin && r.d == d && r.ff == ff && r.pad_dp == dp && r.f16w
        && r.fa_khr == khr && r.gemm_khr == gemm_khr)

//! the padded panels at the compact rows, the f16 feed and the window table; false when an allocation failed
[cold_path]
def private vt_scratch_f16_build(var r : VtResident; rows, nwin, d, ff, hs, dp : int64; khr, gemm_khr : bool) : bool {
    let cap = vt_cap_rows(rows)
    vt_release_scratch(r)
    r.oom = false
    r.cap_rows = cap
    r.win_cap = nwin
    r.d = d
    r.ff = ff
    r.hs = hs
    r.fa_khr = khr
    r.gemm_khr = gemm_khr
    r.pad_dp = dp
    r.f16w = true
    r.fused = true   // the rope tables ride the fused-row seats
    let bx = cap * d * 4l
    let bff = cap * ff * 4l
    let bh = cap * d * 2l
    r.qp_dev = vt_dev_buf(r, cap * dp * 4l)
    r.kp16_dev = vt_dev_buf(r, cap * dp * 2l)
    r.vp16_dev = vt_dev_buf(r, cap * dp * 2l)
    r.attp_dev = vt_dev_buf(r, cap * dp * 4l)
    r.wlo_dev = vt_dev_buf(r, (nwin + 1l) * 4l)
    r.rex_dev = vt_dev_buf(r, cap * 4l)
    r.xh_dev = vt_dev_buf(r, cap * max(d, ff) * 2l)
    r.qkv_dev = vt_dev_buf(r, 64l)   // unused seats of the fused set, stubs the release frees alike
    r.cos_dev = vt_dev_buf(r, cap * (hs / 2l) * 4l)
    r.sin_dev = vt_dev_buf(r, cap * (hs / 2l) * 4l)
    r.x_dev = vt_dev_buf(r, bx)
    r.xb_dev = vt_dev_buf(r, bx)
    r.xq_dev = vt_dev_buf(r, 64l)
    r.xs_dev = vt_dev_buf(r, 64l)
    r.q_dev = vt_dev_buf(r, bx)
    r.k_dev = vt_dev_buf(r, bx)
    r.v_dev = vt_dev_buf(r, bx)
    r.q2_dev = vt_dev_buf(r, 64l)
    r.k2_dev = vt_dev_buf(r, 64l)
    r.v2_dev = vt_dev_buf(r, 64l)
    r.k16_dev = vt_dev_buf(r, bh)
    r.v16_dev = vt_dev_buf(r, bh)
    r.att_dev = vt_dev_buf(r, bx)
    r.yo_dev = vt_dev_buf(r, bx)
    r.hg_dev = vt_dev_buf(r, bff)
    r.hu_dev = vt_dev_buf(r, bff)
    r.yd_dev = vt_dev_buf(r, bx)
    r.tabs_dev = vt_dev_buf(r, 64l)
    r.host = vt_host_buf(r, bx, true)
    r.meta_bytes = 0l
    if (r.oom) {
        vt_release_scratch(r)
        return false
    }
    upload_zeros(r.rex_dev, 0l, cap * 4l)
    return true
}

[cold_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vt_sets_q25v(var r : VtResident) {
    let cap = r.cap_rows
    let gemm_khr = r.gemm_khr
    let bx = cap * r.d * 4l
    let bff = cap * r.ff * 4l
    let bxh = cap * max(r.d, r.ff) * 2l
    let bp = cap * r.pad_dp * 4l
    let bph = cap * r.pad_dp * 2l
    let btab = cap * (r.hs / 2l) * 4l
    let bwl = (r.win_cap + 1l) * 4l
    r.s_rms = set_tower_rms_cls(fixed_array(r.x_dev, r.xb_dev, r.norms_dev), fixed_array(bx, bx, r.norms_bytes), fixed_array(VT_X, VT_XB, 0u))
    r.s_cvt_xb = set_tower_clamp_cvt_cls(fixed_array(r.xb_dev, r.xh_dev), fixed_array(bx, bxh), fixed_array(VT_XB, VT_XQ))
    r.s_cvt_att = set_tower_clamp_cvt_cls(fixed_array(r.att_dev, r.xh_dev), fixed_array(bx, bxh), fixed_array(VT_ATT, VT_XQ))
    r.s_q = f16_gemm_set(gemm_khr, fixed_array(r.wq_dev, r.xh_dev, r.q_dev), fixed_array(r.wq_bytes, bxh, bx), fixed_array(0u, VT_XQ, VT_Q))
    r.s_k = f16_gemm_set(gemm_khr, fixed_array(r.wq_dev, r.xh_dev, r.k_dev), fixed_array(r.wq_bytes, bxh, bx), fixed_array(0u, VT_XQ, VT_K))
    r.s_v = f16_gemm_set(gemm_khr, fixed_array(r.wq_dev, r.xh_dev, r.v_dev), fixed_array(r.wq_bytes, bxh, bx), fixed_array(0u, VT_XQ, VT_V))
    r.s_o = f16_gemm_set(gemm_khr, fixed_array(r.wq_dev, r.xh_dev, r.yo_dev), fixed_array(r.wq_bytes, bxh, bx), fixed_array(0u, VT_XQ, VT_YO))
    r.s_hg = f16_gemm_set(gemm_khr, fixed_array(r.wq_dev, r.xh_dev, r.hg_dev), fixed_array(r.wq_bytes, bxh, bff), fixed_array(0u, VT_XQ, VT_HG))
    r.s_hu = f16_gemm_set(gemm_khr, fixed_array(r.wq_dev, r.xh_dev, r.hu_dev), fixed_array(r.wq_bytes, bxh, bff), fixed_array(0u, VT_XQ, VT_HU))
    r.s_down = f16_gemm_set(gemm_khr, fixed_array(r.wq_dev, r.xh_dev, r.yd_dev), fixed_array(r.wq_bytes, bxh, bx), fixed_array(0u, VT_XQ, VT_YD))
    r.s_bias_q = set_tower_bias_act_cls(fixed_array(r.q_dev, r.norms_dev), fixed_array(bx, r.norms_bytes), fixed_array(VT_Q, 0u))
    r.s_bias_k = set_tower_bias_act_cls(fixed_array(r.k_dev, r.norms_dev), fixed_array(bx, r.norms_bytes), fixed_array(VT_K, 0u))
    r.s_bias_v = set_tower_bias_act_cls(fixed_array(r.v_dev, r.norms_dev), fixed_array(bx, r.norms_bytes), fixed_array(VT_V, 0u))
    r.s_rope_tq = set_tower_rope_tab_cls(fixed_array(r.q_dev, r.cos_dev, r.sin_dev), fixed_array(bx, btab, btab), fixed_array(VT_Q, 0u, 0u))
    r.s_rope_tk = set_tower_rope_tab_cls(fixed_array(r.k_dev, r.cos_dev, r.sin_dev), fixed_array(bx, btab, btab), fixed_array(VT_K, 0u, 0u))
    r.s_pad_q = set_tower_hpad_cls(fixed_array(r.q_dev, r.qp_dev), fixed_array(bx, bp), fixed_array(VT_Q, VT_Q2))
    r.s_pad_k = set_tower_hpad16_cls(fixed_array(r.k_dev, r.kp16_dev), fixed_array(bx, bph), fixed_array(VT_K, VT_K16))
    r.s_pad_v = set_tower_hpad16_cls(fixed_array(r.v_dev, r.vp16_dev), fixed_array(bx, bph), fixed_array(VT_V, VT_V16))
    r.s_fa = fa_stamp_set(r.fa_khr, fixed_array(r.qp_dev, r.kp16_dev, r.vp16_dev, r.attp_dev), fixed_array(bp, bph, bph, bp), fixed_array(VT_Q2, VT_K16, VT_V16, VT_K2))
    r.s_unpad = set_tower_hunpad_cls(fixed_array(r.attp_dev, r.att_dev), fixed_array(bp, bx), fixed_array(VT_K2, VT_ATT))
    r.s_win = set_tower_win_attn_cls(fixed_array(r.q_dev, r.k_dev, r.v_dev, r.att_dev, r.wlo_dev), fixed_array(bx, bx, bx, bx, bwl), fixed_array(VT_Q, VT_K, VT_V, VT_ATT, 0u))
    r.s_post_o_ln = set_tower_post_add_rms_cls(fixed_array(r.x_dev, r.yo_dev, r.xb_dev, r.norms_dev), fixed_array(bx, bx, bx, r.norms_bytes), fixed_array(VT_X, VT_YO, VT_XB, 0u))
    r.s_post_d_ln = set_tower_post_add_rms_cls(fixed_array(r.x_dev, r.yd_dev, r.xb_dev, r.norms_dev), fixed_array(bx, bx, bx, r.norms_bytes), fixed_array(VT_X, VT_YD, VT_XB, 0u))
    r.s_glu = set_actf16_b_cls(fixed_array(r.hg_dev, r.hu_dev, r.xh_dev, r.norms_dev, r.norms_dev, r.rex_dev),
        fixed_array(bff, bff, bxh, r.norms_bytes, r.norms_bytes, cap * 4l), fixed_array(VT_HG, VT_HU, VT_XQ, 0u, 0u, 0u))
}

def private vt_lo(c : Clamp; input : bool) : float => c.active ? (input ? c.imin : c.omin) : -VT_NO_CLAMP
def private vt_hi(c : Clamp; input : bool) : float => c.active ? (input ? c.imax : c.omax) : VT_NO_CLAMP

//! one block GEMM's region in the tower's q8 plane: its element offset, its input width and its rows - the repack unit
struct VtRegion {
    woff : int64
    n : int64
    rows : int64
}

//! the gather's parallel slice of a region: the fixed slice where it divides the rows and the group, else the whole region
def vt_gather_slice(rows, mr : int64) : int64
    => rows % VT_GATHER_SLICE_ROWS == 0l && VT_GATHER_SLICE_ROWS % mr == 0l ? VT_GATHER_SLICE_ROWS : rows

//! the q8 planes' regions gathered into the device layout (`wq` the quants, `ws` the halfword scales)
[cold_path]
def vt_gather_regions(qblob : PlaneI8; qscales : PlaneF; regions : array<VtRegion>; lay : Q8RepackLayout; var wq, ws : array<uint8>) {
    let q8e = kq_elems(KqFmt.q8)
    for (reg in regions) {
        q8_gather_rows(plane_at(qblob, reg.woff), null, plane_at(qscales, reg.woff / q8e), reg.n, reg.rows, vt_gather_slice(reg.rows, lay.mr), lay.mr, lay.kgroup, lay.wbias,
            wq, ws, reg.woff, reg.woff / q8e)
    }
}

[cold_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vt_upload(var r : VtResident; key : uint64; qblob : PlaneI8; qscales : PlaneF; regions : array<VtRegion>;
                      blob : PlaneF; rows_off, rows_n, ones_n : int64; lay : Q8RepackLayout) : bool {
    vt_upload_begin(r, key)
    r.wq_bytes = qblob.n
    r.ws_bytes = qscales.n * 2l
    r.wq_dev = vt_dev_buf(r, r.wq_bytes)
    r.ws_dev = vt_dev_buf(r, r.ws_bytes)
    r.ones_off = rows_n
    r.norms_bytes = (rows_n + ones_n) * 4l
    r.norms_dev = vt_dev_buf(r, r.norms_bytes)
    if (r.oom) {
        vt_release(r)
        return false
    }
    var wq : array<uint8>
    var ws : array<uint8>
    wq |> reserve_resize(r.wq_bytes)
    ws |> reserve_resize(r.ws_bytes)
    vt_gather_regions(qblob, qscales, regions, lay, wq, ws)
    unsafe {
        upload_region_at(r.wq_dev, 0l, addr<uint8 const?>(wq[0]), r.wq_bytes)
        upload_region_at(r.ws_dev, 0l, addr<uint8 const?>(ws[0]), r.ws_bytes)
    }
    delete wq
    delete ws
    var norms : array<float>
    norms |> reserve_resize(rows_n + ones_n)
    unsafe {
        memcpy(addr<void?>(norms[0]), reinterpret<void?>(plane_at(blob, rows_off)), rows_n * 4l)
    }
    for (i in range64(rows_n, rows_n + ones_n)) {
        norms[i] = 1.0
    }
    upload_region_at(r.norms_dev, 0l, unsafe(addr<uint8 const?>(norms[0])), r.norms_bytes)
    delete norms
    r.cap_rows = 0l
    vt_unbind_batch_sets(r)
    return true
}

def private vt_scratch_fits(r : VtResident; rows, d, ff, dp, meta_bytes : int64; khr, fused : bool; taps : int64) : bool
    => (r.cap_rows >= vt_cap_rows(rows) && r.d == d && r.ff == ff && r.pad_dp == dp && r.meta_bytes >= meta_bytes && r.fused == fused
        && r.taps == taps && r.fa_khr == khr)

//! `dp` > 0 adds the padded panels, `meta_bytes` sizes the schedule buffer, `fused` adds the [q | k | v] rows and the rope tables, `taps` the stashes; false when an allocation failed
[cold_path]
def private vt_scratch_build(var r : VtResident; rows, d, ff, hs, dp, meta_bytes : int64; khr, fused : bool; taps : int64) : bool {
    let q8e = kq_elems(KqFmt.q8)
    let cap = vt_cap_rows(rows)
    vt_release_scratch(r)
    r.oom = false
    r.meta_bytes = max(meta_bytes, BATCH_META_BYTES)
    r.meta = vt_host_buf(r, r.meta_bytes, false)
    r.cap_rows = cap
    r.d = d
    r.ff = ff
    r.hs = hs
    r.fa_khr = khr
    let bx = cap * d * 4l
    let bff = cap * ff * 4l
    let bq = cap * max(d, ff)
    let bs = bq / q8e * 4l
    let bh = cap * d * 2l
    let btabs = cap * hs * 4l   // four tables of hs/4 pairs a row
    r.cm2 = vt_cm2_feed()
    if (r.cm2) {
        r.xh_dev = vt_dev_buf(r, (cap + TILE_READ_SLACK) * max(d, ff) * 2l)
    }
    r.pad_dp = dp
    if (dp > 0l) {
        r.qp_dev = vt_dev_buf(r, cap * dp * 4l)
        r.kp16_dev = vt_dev_buf(r, cap * dp * 2l)
        r.vp16_dev = vt_dev_buf(r, cap * dp * 2l)
        r.attp_dev = vt_dev_buf(r, cap * dp * 4l)
    }
    r.fused = fused
    if (fused) {
        r.qkv_dev = vt_dev_buf(r, 3l * bx)
        r.cos_dev = vt_dev_buf(r, cap * (hs / 2l) * 4l)
        r.sin_dev = vt_dev_buf(r, cap * (hs / 2l) * 4l)
    }
    r.taps = taps
    if (taps > 0l) {
        r.xtap_dev = vt_dev_buf(r, taps * bx)
    }
    r.x_dev = vt_dev_buf(r, bx)
    r.xb_dev = vt_dev_buf(r, bx)
    r.xq_dev = vt_dev_buf(r, bq)
    r.xs_dev = vt_dev_buf(r, bs)
    r.q_dev = vt_dev_buf(r, bx)
    r.k_dev = vt_dev_buf(r, bx)
    r.v_dev = vt_dev_buf(r, bx)
    r.q2_dev = vt_dev_buf(r, bx)
    r.k2_dev = vt_dev_buf(r, bx)
    r.v2_dev = vt_dev_buf(r, bx)
    r.k16_dev = vt_dev_buf(r, bh)
    r.v16_dev = vt_dev_buf(r, bh)
    r.att_dev = vt_dev_buf(r, bx)
    r.yo_dev = vt_dev_buf(r, bx)
    r.hg_dev = vt_dev_buf(r, bff)
    r.hu_dev = vt_dev_buf(r, bff)
    r.yd_dev = vt_dev_buf(r, bx)
    r.tabs_dev = vt_dev_buf(r, btabs)
    r.host = vt_host_buf(r, (1l + taps) * bx, true)
    if (r.oom) {
        vt_release_scratch(r)
        return false
    }
    return true
}

let private VT_G4A_GEMMS = 11l   //! a Conformer block's GEMM records, in the order below
let private VT_G4A_FF_UP = 0l
let private VT_G4A_FF_DOWN = 1l
let private VT_G4A_Q = 2l
let private VT_G4A_K = 3l
let private VT_G4A_V = 4l
let private VT_G4A_O = 5l
let private VT_G4A_REL = 6l
let private VT_G4A_PW1 = 7l
let private VT_G4A_PW2 = 8l
let private VT_G4A_FF_UP_1 = 9l
let private VT_G4A_FF_DOWN_1 = 10l
let private VT_G4A_TAIL_GEMMS = 2l   //! the projector tail's records after the blocks': the out-proj, the mm-proj

[arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vt_g4a_regions(lo : G4aLayer; d, ff : int64; var out : VtRegion[int(VT_G4A_GEMMS)]) {
    out[VT_G4A_FF_UP] = VtRegion(woff = lo.ff_up, n = d, rows = ff)
    out[VT_G4A_FF_DOWN] = VtRegion(woff = lo.ff_down, n = ff, rows = d)
    out[VT_G4A_Q] = VtRegion(woff = lo.q, n = d, rows = d)
    out[VT_G4A_K] = VtRegion(woff = lo.k, n = d, rows = d)
    out[VT_G4A_V] = VtRegion(woff = lo.v, n = d, rows = d)
    out[VT_G4A_O] = VtRegion(woff = lo.o, n = d, rows = d)
    out[VT_G4A_REL] = VtRegion(woff = lo.attn_k_rel, n = d, rows = d)
    out[VT_G4A_PW1] = VtRegion(woff = lo.conv_pw1, n = d, rows = 2l * d)
    out[VT_G4A_PW2] = VtRegion(woff = lo.conv_pw2, n = d, rows = d)
    out[VT_G4A_FF_UP_1] = VtRegion(woff = lo.ff_up_1, n = d, rows = ff)
    out[VT_G4A_FF_DOWN_1] = VtRegion(woff = lo.ff_down_1, n = ff, rows = d)
}

def private vt_g4a_key(t : Gemma4aEncoder; lay : Q8RepackLayout) : uint64 {
    let key = vt_key(t.qblob, t.qscales, t.blob, 0l, t.blob.n, 0l, t.n_layer, lay.mr, lay.kgroup, lay.wbias)
    return vt_fold_words(key, unsafe(reinterpret<uint8 const?>(plane_at(t.pos_emb, 0l))), (gemma4a_attn_past() + 1l) * t.d_model * 4l)
}

[cold_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vt_upload_g4a(var r : VtResident; key : uint64; t : Gemma4aEncoder; lay : Q8RepackLayout) : bool {
    let d = t.d_model
    let ff = t.n_ff
    var regions : array<VtRegion>
    regions |> reserve(t.n_layer * VT_G4A_GEMMS + 3l)
    var recs : VtRegion[int(VT_G4A_GEMMS)]
    for (l in range64(t.n_layer)) {
        vt_g4a_regions(t.layers[l], d, ff, recs)
        regions |> push_from(recs)
    }
    regions |> push(VtRegion(woff = t.inproj_w, n = d, rows = d))
    regions |> push(VtRegion(woff = t.outproj_w, n = d, rows = t.proj_dim))
    regions |> push(VtRegion(woff = t.mmproj_w, n = t.proj_dim, rows = t.proj_dim))
    let ok = vt_upload(r, key, t.qblob, t.qscales, regions, t.blob, 0l, t.blob.n, t.proj_dim, lay)
    delete regions
    if (!ok) {
        return false
    }
    let rows = gemma4a_attn_past() + 1l
    if (!vt_rel_bufs(r, rows, d)) {
        vt_release(r)
        return false
    }
    upload_region_at(r.rel_tab_dev, 0l, unsafe(reinterpret<uint8 const?>(plane_at(t.pos_emb, 0l))), rows * d * 4l)
    return true
}

[cold_path]
def private vt_sets_g4a(var r : VtResident) {
    let q8e = kq_elems(KqFmt.q8)
    let cap = r.cap_rows
    let bx = cap * r.d * 4l
    let bff = cap * r.ff * 4l
    let bq = cap * max(r.d, r.ff)
    let bs = bq / q8e * 4l
    let b_relq = r.rel_cap_rows * r.d
    let b_rel = b_relq * 4l
    let b_rels = b_relq / q8e * 4l
    r.s_rms = set_tower_rms_cls(fixed_array(r.x_dev, r.xb_dev, r.norms_dev), fixed_array(bx, bx, r.norms_bytes), fixed_array(VT_X, VT_XB, 0u))
    r.s_rms_yd = set_tower_rms_cls(fixed_array(r.yd_dev, r.yd_dev, r.norms_dev), fixed_array(bx, bx, r.norms_bytes), fixed_array(VT_YD, VT_YD, 0u))
    r.s_rms_x = set_tower_rms_cls(fixed_array(r.x_dev, r.x_dev, r.norms_dev), fixed_array(bx, bx, r.norms_bytes), fixed_array(VT_X, VT_X, 0u))
    r.s_rq_xb = set_tower_clamp_rq_cls(fixed_array(r.xb_dev, r.xq_dev, r.xs_dev), fixed_array(bx, bq, bs), fixed_array(VT_XB, VT_XQ, VT_XQ))
    r.s_rq_att = set_tower_clamp_rq_cls(fixed_array(r.att_dev, r.xq_dev, r.xs_dev), fixed_array(bx, bq, bs), fixed_array(VT_ATT, VT_XQ, VT_XQ))
    r.s_rq_hg = set_tower_clamp_rq_cls(fixed_array(r.hg_dev, r.xq_dev, r.xs_dev), fixed_array(bff, bq, bs), fixed_array(VT_HG, VT_XQ, VT_XQ))
    r.s_rq_yd = set_tower_clamp_rq_cls(fixed_array(r.yd_dev, r.xq_dev, r.xs_dev), fixed_array(bx, bq, bs), fixed_array(VT_YD, VT_XQ, VT_XQ))
    r.s_rq_rel_tab = set_tower_clamp_rq_cls(fixed_array(r.rel_tab_dev, r.rel_tabq_dev, r.rel_tabs_dev), fixed_array(b_rel, b_relq, b_rels), fixed_array(0u, VT_REL_TABQ, VT_REL_TABQ))
    r.s_clamp_q = set_tower_clamp_cls(fixed_array(r.q_dev), fixed_array(bx), fixed_array(VT_Q))
    r.s_clamp_k = set_tower_clamp_cls(fixed_array(r.k_dev), fixed_array(bx), fixed_array(VT_K))
    r.s_clamp_v = set_tower_clamp_cls(fixed_array(r.v_dev), fixed_array(bx), fixed_array(VT_V))
    r.s_clamp_yo = set_tower_clamp_cls(fixed_array(r.yo_dev), fixed_array(bx), fixed_array(VT_YO))
    r.s_clamp_yd = set_tower_clamp_cls(fixed_array(r.yd_dev), fixed_array(bx), fixed_array(VT_YD))
    r.s_clamp_hg = set_tower_clamp_cls(fixed_array(r.hg_dev), fixed_array(bff), fixed_array(VT_HG))
    r.s_silu_hg = set_tower_act_cls(fixed_array(r.hg_dev), fixed_array(bff), fixed_array(VT_HG))
    r.s_silu_yd = set_tower_act_cls(fixed_array(r.yd_dev), fixed_array(bx), fixed_array(VT_YD))
    r.s_glu_sig = set_tower_glu_sig_cls(fixed_array(r.hg_dev, r.hu_dev), fixed_array(bff, bff), fixed_array(VT_HG, VT_HU))
    r.s_dw = set_tower_dw_conv5_cls(fixed_array(r.hu_dev, r.yd_dev, r.norms_dev), fixed_array(bff, bx, r.norms_bytes), fixed_array(VT_HU, VT_YD, 0u))
    if (r.hs == 64l) {
        r.s_attn = set_tower_g4a_attn64_cls(fixed_array(r.q_dev, r.k_dev, r.v_dev, r.att_dev, r.rel_proj_dev, r.norms_dev),
            fixed_array(bx, bx, bx, bx, b_rel, r.norms_bytes), fixed_array(VT_Q, VT_K, VT_V, VT_ATT, VT_REL_PROJ, 0u))
    } else {
        r.s_attn = set_tower_g4a_attn_cls(fixed_array(r.q_dev, r.k_dev, r.v_dev, r.att_dev, r.rel_proj_dev, r.norms_dev),
            fixed_array(bx, bx, bx, bx, b_rel, r.norms_bytes), fixed_array(VT_Q, VT_K, VT_V, VT_ATT, VT_REL_PROJ, 0u))
    }
    r.s_post_o = set_tower_post_add_cls(fixed_array(r.x_dev, r.yo_dev, r.xb_dev, r.norms_dev), fixed_array(bx, bx, bx, r.norms_bytes), fixed_array(VT_X, VT_YO, VT_XB, 0u))
    r.s_post_o_plain = set_tower_post_add_plain_cls(fixed_array(r.x_dev, r.yo_dev, r.xb_dev, r.norms_dev), fixed_array(bx, bx, bx, r.norms_bytes), fixed_array(VT_X, VT_YO, VT_XB, 0u))
    r.s_post_d = set_tower_post_add_cls(fixed_array(r.x_dev, r.yd_dev, r.xb_dev, r.norms_dev), fixed_array(bx, bx, bx, r.norms_bytes), fixed_array(VT_X, VT_YD, VT_XB, 0u))
    vt_sets_cm2_feed(r)
}

let private VT_G4A_OUTPROJ_KEY = 6000   //! the projector tail's GEMM sets in `sets_front`: the out-proj at this + the tile key, the mm-proj at the next hundred
let private VT_G4A_MMPROJ_KEY = 6100

[cold_path]
def private vt_tail_release(var r : VtResident) {
    if (r.tail_cap > 0l) {
        destroy_device_buf(r.tail_out_dev)
        destroy_device_buf(r.tail_proj_dev)
        destroy_host_buf(r.tail_host)
        r.tail_cap = 0l
    }
}

[cold_path]
def private vt_tail_bufs(var r : VtResident; tail_dim : int64) : bool {
    vt_tail_release(r)
    let cap = r.cap_rows
    let bp = cap * tail_dim * 4l
    let bx = cap * r.d * 4l
    let bq = cap * max(r.d, r.ff)
    let bs = bq / kq_elems(KqFmt.q8) * 4l
    let bxh = (cap + TILE_READ_SLACK) * max(r.d, r.ff) * 2l
    r.oom = false
    r.tail_out_dev = vt_dev_buf(r, bp)
    r.tail_proj_dev = vt_dev_buf(r, bp)
    r.tail_host = vt_host_buf(r, bp, true)
    r.tail_cap = cap
    r.tail_dim = tail_dim
    if (r.oom) {
        vt_tail_release(r)
        return false
    }
    if (r.cm2) {
        r.s_feed_x = set_tower_clamp_cvt_cls(fixed_array(r.x_dev, r.xh_dev), fixed_array(bx, bxh), fixed_array(VT_X, VT_XQ))
        r.s_feed_pt = set_tower_clamp_cvt_cls(fixed_array(r.tail_out_dev, r.xh_dev), fixed_array(bp, bxh), fixed_array(VT_TAIL_OUT, VT_XQ))
    } else {
        r.s_feed_x = set_tower_clamp_rq_cls(fixed_array(r.x_dev, r.xq_dev, r.xs_dev), fixed_array(bx, bq, bs), fixed_array(VT_X, VT_XQ, VT_XQ))
        r.s_feed_pt = set_tower_clamp_rq_cls(fixed_array(r.tail_out_dev, r.xq_dev, r.xs_dev), fixed_array(bp, bq, bs), fixed_array(VT_TAIL_OUT, VT_XQ, VT_XQ))
    }
    r.s_bias_pt = set_tower_bias_act_cls(fixed_array(r.tail_out_dev, r.norms_dev), fixed_array(bp, r.norms_bytes), fixed_array(VT_TAIL_OUT, 0u))
    r.s_rms_pt = set_tower_rms_cls(fixed_array(r.tail_out_dev, r.tail_out_dev, r.norms_dev), fixed_array(bp, bp, r.norms_bytes), fixed_array(VT_TAIL_OUT, VT_TAIL_OUT, 0u))
    return true
}

//! the tail's GEMM set over its tile: the out-proj into the pt rows, the mm-proj into the po rows, built once per tile key
def private vt_tail_gemm_set(var r : VtResident; v : VtTile; mmproj : bool) : VkDescriptorSet {
    let key = (mmproj ? VT_G4A_MMPROJ_KEY : VT_G4A_OUTPROJ_KEY) + vt_tile_key(v)
    if (!key_exists(r.sets_front, key)) {
        let bp = r.tail_cap * r.tail_dim * 4l
        r.sets_front[key] = vt_tile_set(r, v, mmproj ? r.tail_proj_dev : r.tail_out_dev, bp, mmproj ? VT_TAIL_PROJ : VT_TAIL_OUT)
    }
    return r.sets_front[key]
}

[cold_path]
def private vt_sets_cm2_feed(var r : VtResident) {
    if (!r.cm2) {
        return
    }
    let cap = r.cap_rows
    let bx = cap * r.d * 4l
    let bff = cap * r.ff * 4l
    let bxh = (cap + TILE_READ_SLACK) * max(r.d, r.ff) * 2l
    let b_rel = r.rel_cap_rows * r.d * 4l
    let b_relh = (r.rel_cap_rows + TILE_READ_SLACK) * r.d * 2l
    r.s_rq_xb = set_tower_clamp_cvt_cls(fixed_array(r.xb_dev, r.xh_dev), fixed_array(bx, bxh), fixed_array(VT_XB, VT_XQ))
    r.s_rq_att = set_tower_clamp_cvt_cls(fixed_array(r.att_dev, r.xh_dev), fixed_array(bx, bxh), fixed_array(VT_ATT, VT_XQ))
    r.s_rq_hg = set_tower_clamp_cvt_cls(fixed_array(r.hg_dev, r.xh_dev), fixed_array(bff, bxh), fixed_array(VT_HG, VT_XQ))
    r.s_rq_yd = set_tower_clamp_cvt_cls(fixed_array(r.yd_dev, r.xh_dev), fixed_array(bx, bxh), fixed_array(VT_YD, VT_XQ))
    r.s_rq_rel_tab = set_tower_clamp_cvt_cls(fixed_array(r.rel_tab_dev, r.rel_tabh_dev), fixed_array(b_rel, b_relh), fixed_array(VT_REL_TAB, VT_REL_TABQ))
}

[arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vt_mm_clamped(raw : VkCommandBuffer; var h : VkHaz; c : Clamp; var s_rq : VkDescriptorSet; nin : int64; var s_gemm : VkDescriptorSet; v : VtTile;
                          n, dout : int64; map_off : uint; groups : int64; var s_cl : VkDescriptorSet; nout : int64) {
    var pc_rq = TowerClampRqArgs(inbase = 0u, nblk = uint(nin / kq_elems(KqFmt.q8)), lo = vt_lo(c, true), hi = vt_hi(c, true))
    vt_feed_enc(raw, h, v.tc > 0l, s_rq, pc_rq, nin)
    var pc_g = BatchArgs(n = uint(n), d = uint(dout), map_off = map_off)
    vt_tile_enc(raw, h, v, s_gemm, pc_g, groups)
    if (c.active) {
        var pc_cl = TowerClampArgs(nelem = uint(nout), lo = c.omin, hi = c.omax)
        enc_tower_clamp_cls(raw, h, s_cl, pc_cl, rq_wgs(false, nout))
    }
}

[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vulkan_gemma4a_blocks(t : Gemma4aEncoder; var s : Gemma4aState; npos : int64) : bool {   // nolint:STYLE038 — the dispatch chain is the shape
    var r & = g_vt_g4a
    if (!g_vt_env_tower) {
        return vt_decline(r, VulkanTowerDecline.knob)
    }
    if (!t.q8) {
        return vt_decline(r, VulkanTowerDecline.quant_mode)
    }
    let q8e = kq_elems(KqFmt.q8)
    let d = t.d_model
    let ff = t.n_ff
    let hs = t.d_head
    let past = gemma4a_attn_past()
    let rel_cap_rows = past + 1l
    let tail_dim = t.proj_dim
    if (d % 64l != 0l || ff % 64l != 0l || ff < 2l * d || t.n_head * hs != d || (hs != 64l && hs != 128l) || past != int64(G4A_ATTN_PAST) || gemma4a_softcap() != G4A_ATTN_CAP
            || npos > VT_MAX_ENCODE_ROWS || npos <= 0l || tail_dim % 64l != 0l || tail_dim > max(d, ff)) {
        return vt_decline(r, VulkanTowerDecline.shape)
    }
    let arm = vt_arm(r)
    if (!arm.ok) {
        return false
    }
    if (!vt_range_ok(t.qblob.n) || !vt_range_ok(t.blob.n * 4l) || !vt_range_ok(vt_scratch_max_bytes(vt_cap_rows(npos), d, ff, 0l, 0l, false, false))) {
        return vt_decline(r, VulkanTowerDecline.shape)
    }
    if (!ensure_tower_rms_cls() || !ensure_tower_clamp_cls() || !ensure_tower_clamp_rq_cls() || !ensure_tower_post_add_cls()
            || !ensure_tower_post_add_plain_cls() || !ensure_tower_act_cls() || !ensure_tower_glu_sig_cls() || !ensure_tower_dw_conv5_cls()
            || !(hs == 64l ? ensure_tower_g4a_attn64_cls() : ensure_tower_g4a_attn_cls())) {
        return vt_decline(r, VulkanTowerDecline.device)
    }
    let cm2 = vt_cm2_feed()
    let v = vt_tile(cm2, d, npos)
    let vff = vt_tile(cm2, ff, npos)
    let v2d = vt_tile(cm2, 2l * d, npos)
    let v_rel = vt_tile(cm2, d, rel_cap_rows)
    let vp = vt_tile(cm2, tail_dim, npos)
    if (!vt_tile_ensure(v) || !vt_tile_ensure(vff) || !vt_tile_ensure(v2d) || !vt_tile_ensure(v_rel) || !vt_tile_ensure(vp)
            || (cm2 && !ensure_tower_clamp_cvt_cls()) || !ensure_tower_bias_act_cls()) {
        return vt_decline(r, VulkanTowerDecline.device)
    }
    let lay = active_q8_repack_layout(g_active_needs_repack)
    let key = vt_g4a_key(t, lay)
    if (r.key != key && !vt_upload_g4a(r, key, t, lay)) {
        return vt_decline(r, VulkanTowerDecline.memory)
    }
    let g_d = vt_tile_groups(v, d, npos)
    let g_ff = vt_tile_groups(vff, ff, npos)
    let g_2d = vt_tile_groups(v2d, 2l * d, npos)
    let g_rel = vt_tile_groups(v_rel, d, rel_cap_rows)
    let g_p = vt_tile_groups(vp, tail_dim, npos)
    let nrec = t.n_layer * VT_G4A_GEMMS + VT_G4A_TAIL_GEMMS
    let meta_bytes = (nrec * 4l + t.n_layer * (7l * g_d + 2l * g_ff + g_2d + g_rel) + 2l * g_p) * 4l
    if (!vt_scratch_fits(r, npos, d, ff, 0l, meta_bytes, arm.khr, false, 0l)) {
        if (!vt_scratch_build(r, npos, d, ff, hs, 0l, meta_bytes, arm.khr, false, 0l)) {
            return vt_decline(r, VulkanTowerDecline.memory)
        }
        vt_sets_g4a(r)
    }
    if ((r.tail_cap < r.cap_rows || r.tail_dim != tail_dim) && !vt_tail_bufs(r, tail_dim)) {
        return vt_decline(r, VulkanTowerDecline.memory)
    }
    var sets = vt_conformer_sets(r, v, vff, v2d, v_rel)
    var s_h2 = sets.s_h2
    var s_rel = sets.s_rel
    var s_outproj = vt_tail_gemm_set(r, vp, false)
    var s_mmproj = vt_tail_gemm_set(r, vp, true)
    g_vt_maps |> reserve(nrec)
    g_vt_maps |> resize(nrec)
    unsafe {
        var mp = reinterpret<uint?>(r.meta.mapped)
        var cur = nrec * 4l
        var recs : VtRegion[int(VT_G4A_GEMMS)]
        let rec_tiles = fixed_array(vff, v, v, v, v, v, v_rel, v2d, v, vff, v)
        for (l in range64(t.n_layer)) {
            vt_g4a_regions(t.layers[l], d, ff, recs)
            for (g in range64(VT_G4A_GEMMS)) {
                let rid = l * VT_G4A_GEMMS + g
                let rows = g == VT_G4A_REL ? rel_cap_rows : vt_tile_rows(rec_tiles[g], npos)   // the rel record keeps its raw rows: its planes are sized to them, and a forced l column would read past them
                let groups = g == VT_G4A_REL ? g_rel : (g == VT_G4A_PW1 ? g_2d : ((g == VT_G4A_FF_UP || g == VT_G4A_FF_UP_1) ? g_ff : g_d))
                g_vt_maps[rid] = vt_sched_map(mp, cur, rid, recs[g].woff / q8e, rows, groups)
            }
        }
        g_vt_maps[nrec - 2l] = vt_sched_map(mp, cur, nrec - 2l, t.outproj_w / q8e, vt_tile_rows(vp, npos), g_p)
        g_vt_maps[nrec - 1l] = vt_sched_map(mp, cur, nrec - 1l, t.mmproj_w / q8e, vt_tile_rows(vp, npos), g_p)
    }
    let bx = npos * d * 4l
    let bo = npos * tail_dim * 4l
    upload_region_at(r.x_dev, 0l, unsafe(addr<uint8 const?>(s.x[0])), bx)
    let nd = npos * d
    let nff = npos * ff
    let n2d = npos * 2l * d
    let wg_attn = (npos * t.n_head + 63l) / 64l
    var pc_rq_rel = TowerClampRqArgs(inbase = 0u, nblk = uint(rel_cap_rows * d / q8e), lo = -VT_NO_CLAMP, hi = VT_NO_CLAMP)
    var pc_rel = BatchArgs(n = uint(d), d = uint(d), map_off = 0u)
    var pc_glu = TowerConvModArgs(d = uint(d), nelem = uint(nd), woff = 0u)
    var pc_silu_ff = TowerBiasArgs(d = uint(ff), nelem = uint(nff), boff = 0u, act = BIAS_ACT_SILU)
    var pc_silu_d = TowerBiasArgs(d = uint(d), nelem = uint(nd), boff = 0u, act = BIAS_ACT_SILU)
    var pc_attn = TowerG4aAttnArgs(d = uint(d), npos = uint(npos), pdsoff = 0u)
    var pc_rms = TowerRowArgs(d = uint(d), rows = uint(npos), woff = 0u, eps = t.eps, ln_on = 0u, lnoff = 0u, ascale = 1.0)
    var pc_post = TowerLnArgs(d = uint(d), rows = uint(npos), woff = 0u, boff = 0u, eps = t.eps, ln_on = 1u, lnoff = 0u, lnboff = 0u, ascale = 1.0)
    let attn64 = hs == 64l
    vt_chain_one_shot("gemma4a", t.n_layer, npos) $(raw : VkCommandBuffer; var h : VkHaz) {
        vt_feed_enc(raw, h, cm2, r.s_rq_rel_tab, pc_rq_rel, rel_cap_rows * d)
        vt_pt(raw, VtProfRole.rq)
        for (l in range64(t.n_layer)) {
            let lo = t.layers[l]
            let rb = l * VT_G4A_GEMMS
            pc_rms.woff = uint(lo.ff_norm)
            enc_tower_rms_cls(raw, h, r.s_rms, pc_rms, npos)
            vt_pt(raw, VtProfRole.rms)
            vt_mm_clamped(raw, h, lo.c_ff_up, r.s_rq_xb, nd, r.s_hg, vff, d, ff, g_vt_maps[rb + VT_G4A_FF_UP], g_ff, r.s_clamp_hg, nff)
            vt_pt(raw, VtProfRole.gemm_fc1)
            enc_tower_act_cls(raw, h, r.s_silu_hg, pc_silu_ff, rq_wgs(false, nff))
            vt_pt(raw, VtProfRole.silu)
            vt_mm_clamped(raw, h, lo.c_ff_down, r.s_rq_hg, nff, r.s_down, v, ff, d, g_vt_maps[rb + VT_G4A_FF_DOWN], g_d, r.s_clamp_yd, nd)
            vt_pt(raw, VtProfRole.gemm_fc2)
            pc_post.woff = uint(lo.ff_post_norm)
            pc_post.ascale = gemma4a_res_w()
            pc_post.lnoff = uint(lo.attn_pre_norm)
            enc_tower_post_add_cls(raw, h, r.s_post_d, pc_post, npos)
            vt_pt(raw, VtProfRole.post_add)
            vt_mm_clamped(raw, h, lo.c_q, r.s_rq_xb, nd, r.s_q, v, d, d, g_vt_maps[rb + VT_G4A_Q], g_d, r.s_clamp_q, nd)
            vt_pt(raw, VtProfRole.gemm_q)
            vt_mm_clamped(raw, h, lo.c_k, r.s_rq_xb, nd, r.s_k, v, d, d, g_vt_maps[rb + VT_G4A_K], g_d, r.s_clamp_k, nd)
            vt_pt(raw, VtProfRole.gemm_k)
            vt_mm_clamped(raw, h, lo.c_v, r.s_rq_xb, nd, r.s_v, v, d, d, g_vt_maps[rb + VT_G4A_V], g_d, r.s_clamp_v, nd)
            vt_pt(raw, VtProfRole.gemm_v)
            pc_rel.map_off = g_vt_maps[rb + VT_G4A_REL]
            vt_tile_enc(raw, h, v_rel, s_rel, pc_rel, g_rel)
            vt_pt(raw, VtProfRole.gemm_rel)
            pc_attn.pdsoff = uint(lo.per_dim_scale)
            if (attn64) {
                enc_tower_g4a_attn64_cls(raw, h, r.s_attn, pc_attn, wg_attn)
            } else {
                enc_tower_g4a_attn_cls(raw, h, r.s_attn, pc_attn, wg_attn)
            }
            vt_pt(raw, VtProfRole.attn)
            vt_mm_clamped(raw, h, lo.c_o, r.s_rq_att, nd, r.s_o, v, d, d, g_vt_maps[rb + VT_G4A_O], g_d, r.s_clamp_yo, nd)
            vt_pt(raw, VtProfRole.gemm_o)
            pc_post.woff = uint(lo.attn_post_norm)
            pc_post.ascale = 1.0
            pc_post.lnoff = uint(lo.norm_conv)
            enc_tower_post_add_cls(raw, h, r.s_post_o, pc_post, npos)
            vt_pt(raw, VtProfRole.post_add)
            vt_mm_clamped(raw, h, lo.c_pw1, r.s_rq_xb, nd, s_h2, v2d, d, 2l * d, g_vt_maps[rb + VT_G4A_PW1], g_2d, r.s_clamp_hg, n2d)
            vt_pt(raw, VtProfRole.gemm_pw)
            enc_tower_glu_sig_cls(raw, h, r.s_glu_sig, pc_glu, (nd + 255l) / 256l)
            vt_pt(raw, VtProfRole.glu)
            pc_glu.woff = uint(lo.conv_dw)
            enc_tower_dw_conv5_cls(raw, h, r.s_dw, pc_glu, (nd + 255l) / 256l)
            vt_pt(raw, VtProfRole.dwconv)
            pc_rms.woff = uint(lo.conv_norm)
            enc_tower_rms_cls(raw, h, r.s_rms_yd, pc_rms, npos)
            vt_pt(raw, VtProfRole.rms)
            enc_tower_act_cls(raw, h, r.s_silu_yd, pc_silu_d, rq_wgs(false, nd))
            vt_pt(raw, VtProfRole.silu)
            vt_mm_clamped(raw, h, lo.c_pw2, r.s_rq_yd, nd, r.s_o, v, d, d, g_vt_maps[rb + VT_G4A_PW2], g_d, r.s_clamp_yo, nd)
            vt_pt(raw, VtProfRole.gemm_pw)
            pc_post.woff = 0u
            pc_post.lnoff = uint(lo.ff_norm_1)
            enc_tower_post_add_plain_cls(raw, h, r.s_post_o_plain, pc_post, npos)
            vt_pt(raw, VtProfRole.post_add)
            vt_mm_clamped(raw, h, lo.c_ff_up_1, r.s_rq_xb, nd, r.s_hg, vff, d, ff, g_vt_maps[rb + VT_G4A_FF_UP_1], g_ff, r.s_clamp_hg, nff)
            vt_pt(raw, VtProfRole.gemm_fc1)
            enc_tower_act_cls(raw, h, r.s_silu_hg, pc_silu_ff, rq_wgs(false, nff))
            vt_pt(raw, VtProfRole.silu)
            vt_mm_clamped(raw, h, lo.c_ff_down_1, r.s_rq_hg, nff, r.s_down, v, ff, d, g_vt_maps[rb + VT_G4A_FF_DOWN_1], g_d, r.s_clamp_yd, nd)
            vt_pt(raw, VtProfRole.gemm_fc2)
            pc_post.woff = uint(lo.ff_post_norm_1)
            pc_post.ascale = gemma4a_res_w()
            pc_post.ln_on = 0u
            enc_tower_post_add_cls(raw, h, r.s_post_d, pc_post, npos)
            vt_pt(raw, VtProfRole.post_add)
            pc_post.ln_on = 1u
            pc_rms.woff = uint(lo.ln2)
            enc_tower_rms_cls(raw, h, r.s_rms_x, pc_rms, npos)
            vt_pt(raw, VtProfRole.rms)
        }
        vt_mm_clamped(raw, h, Clamp(), r.s_feed_x, nd, s_outproj, vp, d, tail_dim, g_vt_maps[nrec - 2l], g_p, r.s_clamp_yd, 0l)
        vt_pt(raw, VtProfRole.gemm_o)
        vt_bias_act(raw, h, r.s_bias_pt, tail_dim, npos * tail_dim, t.outproj_b, BIAS_ACT_NONE)
        vt_pt(raw, VtProfRole.bias)
        var pc_rms_pt = TowerRowArgs(d = uint(tail_dim), rows = uint(npos), woff = uint(r.ones_off), eps = t.eps, ln_on = 0u, lnoff = 0u, ascale = 1.0)
        enc_tower_rms_cls(raw, h, r.s_rms_pt, pc_rms_pt, npos)
        vt_pt(raw, VtProfRole.rms)
        vt_mm_clamped(raw, h, Clamp(), r.s_feed_pt, npos * tail_dim, s_mmproj, vp, tail_dim, tail_dim, g_vt_maps[nrec - 1l], g_p, r.s_clamp_yd, 0l)
        vt_pt(raw, VtProfRole.gemm_o)
        vhz_dep(raw, h, VT_TAIL_PROJ, 0u, true)
        cmd_copy_range(raw, r.tail_proj_dev, 0l, r.tail_host.buf, 0l, bo)
        vt_pt(raw, VtProfRole.copy_out)
    }
    ensure_length(s.out, npos * tail_dim)
    s.out_ready = true
    return vt_served_from(r, r.tail_host, s.out, bo, npos, t.n_layer)
}

let private VT_CN_GEMMS = 11l   //! a FastConformer block's GEMM records, in the order below
let private VT_CN_FF1A = 0l
let private VT_CN_FF1B = 1l
let private VT_CN_Q = 2l
let private VT_CN_K = 3l
let private VT_CN_V = 4l
let private VT_CN_POS = 5l
let private VT_CN_O = 6l
let private VT_CN_PW1 = 7l
let private VT_CN_PW2 = 8l
let private VT_CN_FF2A = 9l
let private VT_CN_FF2B = 10l

[arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vt_cn_regions(lo : CanaryLayerOffs; d, ff : int64; var out : VtRegion[int(VT_CN_GEMMS)]) {
    out[VT_CN_FF1A] = VtRegion(woff = lo.ff1a_w, n = d, rows = ff)
    out[VT_CN_FF1B] = VtRegion(woff = lo.ff1b_w, n = ff, rows = d)
    out[VT_CN_Q] = VtRegion(woff = lo.q_w, n = d, rows = d)
    out[VT_CN_K] = VtRegion(woff = lo.k_w, n = d, rows = d)
    out[VT_CN_V] = VtRegion(woff = lo.v_w, n = d, rows = d)
    out[VT_CN_POS] = VtRegion(woff = lo.pos_w, n = d, rows = d)
    out[VT_CN_O] = VtRegion(woff = lo.o_w, n = d, rows = d)
    out[VT_CN_PW1] = VtRegion(woff = lo.pw1_w, n = d, rows = 2l * d)
    out[VT_CN_PW2] = VtRegion(woff = lo.pw2_w, n = d, rows = d)
    out[VT_CN_FF2A] = VtRegion(woff = lo.ff2a_w, n = d, rows = ff)
    out[VT_CN_FF2B] = VtRegion(woff = lo.ff2b_w, n = ff, rows = d)
}

[cold_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vt_upload_cn(var r : VtResident; key : uint64; t : CanaryEncoder; lay : Q8RepackLayout) : bool {
    let d = t.d_model
    let ff = t.n_ff
    var regions : array<VtRegion>
    regions |> reserve(t.n_layer * VT_CN_GEMMS + 3l)
    var recs : VtRegion[int(VT_CN_GEMMS)]
    for (l in range64(t.n_layer)) {
        vt_cn_regions(t.layers[l], d, ff, recs)
        regions |> push_from(recs)
    }
    let nch = t.n_sub_ch
    let w3 = conv_s2_out(conv_s2_out(conv_s2_out(t.n_mel)))
    regions |> push(VtRegion(woff = t.c3_w, n = nch, rows = nch))
    regions |> push(VtRegion(woff = t.c6_w, n = nch, rows = nch))
    regions |> push(VtRegion(woff = t.pre_w, n = w3 * nch, rows = d))
    let ok = vt_upload(r, key, t.qblob, t.qscales, regions, t.blob, 0l, t.blob.n, 0l, lay)
    delete regions
    return ok
}

[cold_path]
def private vt_sets_cn(var r : VtResident) {
    let q8e = kq_elems(KqFmt.q8)
    let cap = r.cap_rows
    let bx = cap * r.d * 4l
    let bff = cap * r.ff * 4l
    let bq = cap * max(r.d, r.ff)
    let bs = bq / q8e * 4l
    let b_relq = r.rel_cap_rows * r.d
    let b_rel = b_relq * 4l
    r.s_ln = set_tower_ln_cls(fixed_array(r.x_dev, r.xb_dev, r.norms_dev), fixed_array(bx, bx, r.norms_bytes), fixed_array(VT_X, VT_XB, 0u))
    r.s_ln_x = set_tower_ln_cls(fixed_array(r.x_dev, r.x_dev, r.norms_dev), fixed_array(bx, bx, r.norms_bytes), fixed_array(VT_X, VT_X, 0u))
    r.s_rq_xb = set_tower_clamp_rq_cls(fixed_array(r.xb_dev, r.xq_dev, r.xs_dev), fixed_array(bx, bq, bs), fixed_array(VT_XB, VT_XQ, VT_XQ))
    r.s_rq_att = set_tower_clamp_rq_cls(fixed_array(r.att_dev, r.xq_dev, r.xs_dev), fixed_array(bx, bq, bs), fixed_array(VT_ATT, VT_XQ, VT_XQ))
    r.s_rq_hg = set_tower_clamp_rq_cls(fixed_array(r.hg_dev, r.xq_dev, r.xs_dev), fixed_array(bff, bq, bs), fixed_array(VT_HG, VT_XQ, VT_XQ))
    r.s_rq_yd = set_tower_clamp_rq_cls(fixed_array(r.yd_dev, r.xq_dev, r.xs_dev), fixed_array(bx, bq, bs), fixed_array(VT_YD, VT_XQ, VT_XQ))
    r.s_rq_rel_tab = set_tower_clamp_rq_cls(fixed_array(r.rel_tab_dev, r.rel_tabq_dev, r.rel_tabs_dev), fixed_array(b_rel, b_relq, b_relq / q8e * 4l), fixed_array(VT_REL_TAB, VT_REL_TABQ, VT_REL_TABQ))
    r.s_cn_rel_sincos = set_tower_rel_sincos_cls(fixed_array(r.rel_tab_dev), fixed_array(b_rel), fixed_array(VT_REL_TAB))
    r.s_bias_q = set_tower_bias_act_cls(fixed_array(r.q_dev, r.norms_dev), fixed_array(bx, r.norms_bytes), fixed_array(VT_Q, 0u))
    r.s_bias_k = set_tower_bias_act_cls(fixed_array(r.k_dev, r.norms_dev), fixed_array(bx, r.norms_bytes), fixed_array(VT_K, 0u))
    r.s_bias_v = set_tower_bias_act_cls(fixed_array(r.v_dev, r.norms_dev), fixed_array(bx, r.norms_bytes), fixed_array(VT_V, 0u))
    r.s_bias_hg = set_tower_bias_act_cls(fixed_array(r.hg_dev, r.norms_dev), fixed_array(bff, r.norms_bytes), fixed_array(VT_HG, 0u))
    r.s_glu_sig = set_tower_glu_sig_cls(fixed_array(r.hg_dev, r.hu_dev), fixed_array(bff, bff), fixed_array(VT_HG, VT_HU))
    r.s_cn_dw = set_tower_cn_dw_cls(fixed_array(r.hu_dev, r.yd_dev, r.norms_dev), fixed_array(bff, bx, r.norms_bytes), fixed_array(VT_HU, VT_YD, 0u))
    r.s_post_o_ln = set_tower_post_add_ln_cls(fixed_array(r.x_dev, r.yo_dev, r.xb_dev, r.norms_dev), fixed_array(bx, bx, bx, r.norms_bytes), fixed_array(VT_X, VT_YO, VT_XB, 0u))
    r.s_post_d_ln = set_tower_post_add_ln_cls(fixed_array(r.x_dev, r.yd_dev, r.xb_dev, r.norms_dev), fixed_array(bx, bx, bx, r.norms_bytes), fixed_array(VT_X, VT_YD, VT_XB, 0u))
    vt_sets_cm2_feed(r)
}

def private vt_cn_rows_ok(npos, d, ff : int64) : bool {
    let cap = vt_cap_rows(npos)
    return (vt_range_ok(vt_scratch_max_bytes(cap, d, ff, 0l, 0l, false, false)) && vt_range_ok(2l * cap * d * 4l)
        && vt_range_ok(cap * (2l * cap - 1l) * 4l))
}

[cold_path]
def private vt_cn_rel_plane(var r : VtResident; cap, hs : int64; gemm_khr : bool) : bool {
    if (r.cn_plane_rows > 0l) {
        for (b in fixed_array(r.cn_qh_dev, r.cn_ph_dev, r.cn_r_dev)) {
            destroy_device_buf(b)
        }
        r.cn_plane_rows = 0l
    }
    let ww = 2l * cap - 1l
    let bqh = (cap + TILE_READ_SLACK) * 128l * 2l
    let bph = (ww + TILE_READ_SLACK) * 128l * 2l
    let br = cap * ww * 4l
    let bx = r.cap_rows * r.d * 4l
    let b_rel = r.rel_cap_rows * r.d * 4l
    r.oom = false
    r.cn_qh_dev = vt_dev_buf(r, bqh)
    r.cn_ph_dev = vt_dev_buf(r, bph)
    r.cn_r_dev = vt_dev_buf(r, br)
    if (r.oom) {
        for (b in fixed_array(r.cn_qh_dev, r.cn_ph_dev, r.cn_r_dev)) {
            destroy_device_buf(b)
        }
        return false
    }
    r.cn_plane_rows = cap
    r.s_cn_gq = set_tower_hpad16_b_cls(fixed_array(r.q_dev, r.cn_qh_dev, r.norms_dev), fixed_array(bx, bqh, r.norms_bytes), fixed_array(VT_Q, VT_CN_QV_PANEL, 0u))
    r.s_cn_gp = set_tower_hpad16_cls(fixed_array(r.rel_proj_dev, r.cn_ph_dev), fixed_array(b_rel, bph), fixed_array(VT_REL_PROJ, VT_CN_REL_PANEL))
    r.s_cn_rg = f16_gemm_set(gemm_khr, fixed_array(r.cn_ph_dev, r.cn_qh_dev, r.cn_r_dev), fixed_array(bph, bqh, br), fixed_array(VT_CN_REL_PANEL, VT_CN_QV_PANEL, VT_CN_REL_PLANE))
    if (hs == 64l) {
        r.s_cn_attn_r = set_tower_cn_attn_r64_cls(fixed_array(r.q_dev, r.k_dev, r.v_dev, r.att_dev, r.cn_r_dev, r.norms_dev),
            fixed_array(bx, bx, bx, bx, br, r.norms_bytes), fixed_array(VT_Q, VT_K, VT_V, VT_ATT, VT_CN_REL_PLANE, 0u))
    } else {
        r.s_cn_attn_r = set_tower_cn_attn_r_cls(fixed_array(r.q_dev, r.k_dev, r.v_dev, r.att_dev, r.cn_r_dev, r.norms_dev),
            fixed_array(bx, bx, bx, bx, br, r.norms_bytes), fixed_array(VT_Q, VT_K, VT_V, VT_ATT, VT_CN_REL_PLANE, 0u))
    }
    return true
}

def private vt_cvt16(raw : VkCommandBuffer; var h : VkHaz; var s : VkDescriptorSet; nelem : int64) {
    var pc = TowerClampArgs(nelem = uint(nelem), lo = -VT_NO_CLAMP, hi = VT_NO_CLAMP)
    enc_tower_clamp_cvt_cls(raw, h, s, pc, (nelem / 4l + 255l) / 256l)
}

//! the elements [from, to) of a plane zeroed by the fill command (the length mask's rows past a stage's valid length)
def private vt_zero_elems(raw : VkCommandBuffer; var h : VkHaz; dev : uint64; bit : uint; from, to : int64) {
    if (from < to) {
        vhz_dep(raw, h, 0u, bit, true)
        cmd_fill_zero(raw, dev, from * 4l, (to - from) * 4l)
    }
}

def private vt_bias_act(raw : VkCommandBuffer; var h : VkHaz; var s_bias : VkDescriptorSet; dout, nout, boff : int64; act : uint) {
    var pc_b = TowerBiasArgs(d = uint(dout), nelem = uint(nout), boff = uint(boff), act = act)
    enc_tower_bias_act_cls(raw, h, s_bias, pc_b, rq_wgs(false, nout))
}

[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vulkan_canary_blocks(t : CanaryEncoder; var s : CanaryState; npos, ww : int64) : bool {   // nolint:STYLE038 — the dispatch chain is the shape
    var r & = g_vt_cn
    if (!g_vt_env_tower) {
        return vt_decline(r, VulkanTowerDecline.knob)
    }
    if (!t.q8) {
        return vt_decline(r, VulkanTowerDecline.quant_mode)
    }
    let q8e = kq_elems(KqFmt.q8)
    let d = t.d_model
    let ff = t.n_ff
    let hs = t.n_head > 0l ? d / t.n_head : 0l
    if (d % 64l != 0l || ff % 64l != 0l || ff < 2l * d || (hs != 64l && hs != 128l) || t.n_head * hs != d || t.conv_kernel % 2l != 1l
            || npos <= 0l || ww != 2l * npos - 1l) {
        return vt_decline(r, VulkanTowerDecline.shape)
    }
    let arm = vt_arm(r, true)
    if (!arm.ok) {
        return false
    }
    if (!vt_range_ok(t.qblob.n) || !vt_range_ok(t.blob.n * 4l) || !vt_cn_rows_ok(npos, d, ff)) {
        return vt_decline(r, VulkanTowerDecline.shape)
    }
    if (!ensure_tower_ln_cls() || !ensure_tower_clamp_rq_cls() || !ensure_tower_bias_act_cls() || !ensure_tower_post_add_ln_cls()
            || !ensure_tower_glu_sig_cls() || !ensure_tower_cn_dw_cls() || !(hs == 64l ? ensure_tower_cn_attn_r64_cls() : ensure_tower_cn_attn_r_cls())
            || !ensure_tower_hpad16_cls() || !ensure_tower_hpad16_b_cls() || !ensure_tower_rel_sincos_cls() || !f16_gemm_ensure(arm.gemm_khr)) {
        return vt_decline(r, VulkanTowerDecline.device)
    }
    let cm2 = vt_cm2_feed()
    let v = vt_tile(cm2, d, npos)
    let vff = vt_tile(cm2, ff, npos)
    let v2d = vt_tile(cm2, 2l * d, npos)
    let v_rel = vt_tile(cm2, d, ww)
    if (!vt_tile_ensure(v) || !vt_tile_ensure(vff) || !vt_tile_ensure(v2d) || !vt_tile_ensure(v_rel) || (cm2 && !ensure_tower_clamp_cvt_cls())) {
        return vt_decline(r, VulkanTowerDecline.device)
    }
    let lay = active_q8_repack_layout(g_active_needs_repack)
    let key = vt_key(t.qblob, t.qscales, t.blob, 0l, t.blob.n, 0l, t.n_layer, lay.mr, lay.kgroup, lay.wbias)
    if (r.key != key && !vt_upload_cn(r, key, t, lay)) {
        return vt_decline(r, VulkanTowerDecline.memory)
    }
    let g_d = vt_tile_groups(v, d, npos)
    let g_ff = vt_tile_groups(vff, ff, npos)
    let g_2d = vt_tile_groups(v2d, 2l * d, npos)
    let g_rel = vt_tile_groups(v_rel, d, ww)
    let nrec = t.n_layer * VT_CN_GEMMS
    let meta_bytes = (nrec * 4l + t.n_layer * (7l * g_d + 2l * g_ff + g_2d + g_rel)) * 4l
    let rel_cap = 2l * vt_cap_rows(npos)
    if (!vt_scratch_fits(r, npos, d, ff, 0l, meta_bytes, arm.khr, false, 0l) || r.rel_cap_rows < ww) {
        if (!vt_scratch_build(r, npos, d, ff, hs, 0l, meta_bytes, arm.khr, false, 0l) || !vt_rel_bufs(r, rel_cap, d)) {
            vt_release_scratch(r)
            return vt_decline(r, VulkanTowerDecline.memory)
        }
        vt_sets_cn(r)
    }
    if (r.cn_plane_rows < vt_cap_rows(npos) && !vt_cn_rel_plane(r, vt_cap_rows(npos), hs, arm.gemm_khr)) {
        return vt_decline(r, VulkanTowerDecline.memory)
    }
    var sets = vt_conformer_sets(r, v, vff, v2d, v_rel)
    var s_h2 = sets.s_h2
    var s_rel = sets.s_rel
    g_vt_maps |> reserve(nrec)
    g_vt_maps |> resize(nrec)
    unsafe {
        var mp = reinterpret<uint?>(r.meta.mapped)
        var cur = nrec * 4l
        var recs : VtRegion[int(VT_CN_GEMMS)]
        let rec_tiles = fixed_array(vff, v, v, v, v, v_rel, v, v2d, v, vff, v)
        for (l in range64(t.n_layer)) {
            vt_cn_regions(t.layers[l], d, ff, recs)
            for (g in range64(VT_CN_GEMMS)) {
                let rid = l * VT_CN_GEMMS + g
                let rows = vt_tile_rows(rec_tiles[g], g == VT_CN_POS ? ww : npos)
                let groups = g == VT_CN_POS ? g_rel : (g == VT_CN_PW1 ? g_2d : ((g == VT_CN_FF1A || g == VT_CN_FF2A) ? g_ff : g_d))
                g_vt_maps[rid] = vt_sched_map(mp, cur, rid, recs[g].woff / q8e, rows, groups)
            }
        }
    }
    let bx = npos * d * 4l
    upload_region_at(r.x_dev, 0l, unsafe(addr<uint8 const?>(s.x[0])), bx)
    var pc_pos = TowerRelSinCosArgs(d = uint(d), tt = uint(npos))
    let nd = npos * d
    let nff = npos * ff
    let n2d = npos * 2l * d
    let wg_attn = (npos + 63l) / 64l
    var pc_rq_rel = TowerClampRqArgs(inbase = 0u, nblk = uint(ww * d / q8e), lo = -VT_NO_CLAMP, hi = VT_NO_CLAMP)
    var pc_rq_xb = TowerClampRqArgs(inbase = 0u, nblk = uint(nd / q8e), lo = -VT_NO_CLAMP, hi = VT_NO_CLAMP)
    var pc_rel = BatchArgs(n = uint(d), d = uint(d), map_off = 0u)
    var pc_glu = TowerConvModArgs(d = uint(d), nelem = uint(nd), woff = 0u)
    var pc_dw = TowerCnDwArgs(d = uint(d), nk = uint(t.conv_kernel), npos = uint(npos), woff = 0u, bnwoff = 0u, bnboff = 0u)
    var pc_attn_r = TowerCnAttnRArgs(d = uint(d), npos = uint(npos), buoff = 0u, hoff = 0u)
    var pc_ln = TowerLnArgs(d = uint(d), rows = uint(npos), woff = 0u, boff = 0u, eps = t.eps, ln_on = 0u, lnoff = 0u, lnboff = 0u, ascale = 1.0)
    var pc_post = TowerLnArgs(d = uint(d), rows = uint(npos), woff = 0u, boff = 0u, eps = t.eps, ln_on = 1u, lnoff = 0u, lnboff = 0u, ascale = 1.0)
    let no_clamp = Clamp()
    var s_none : VkDescriptorSet
    let attn64 = hs == 64l
    vt_chain_one_shot("canary", t.n_layer, npos) $(raw : VkCommandBuffer; var h : VkHaz) {
        enc_tower_rel_sincos_cls(raw, h, r.s_cn_rel_sincos, pc_pos, (ww * d + 255l) / 256l)
        vt_feed_enc(raw, h, cm2, r.s_rq_rel_tab, pc_rq_rel, ww * d)
        vt_pt(raw, VtProfRole.rq)
        for (l in range64(t.n_layer)) {
            let lo = t.layers[l]
            let rb = l * VT_CN_GEMMS
            pc_ln.woff = uint(lo.nf1_w)
            pc_ln.boff = uint(lo.nf1_b)
            enc_tower_ln_cls(raw, h, r.s_ln, pc_ln, npos)
            vt_pt(raw, VtProfRole.ln)
            vt_mm_clamped(raw, h, no_clamp, r.s_rq_xb, nd, r.s_hg, vff, d, ff, g_vt_maps[rb + VT_CN_FF1A], g_ff, s_none, 0l)
            vt_pt(raw, VtProfRole.gemm_fc1)
            vt_bias_act(raw, h, r.s_bias_hg, ff, nff, lo.ff1a_b, BIAS_ACT_SILU)
            vt_pt(raw, VtProfRole.bias)
            vt_mm_clamped(raw, h, no_clamp, r.s_rq_hg, nff, r.s_down, v, ff, d, g_vt_maps[rb + VT_CN_FF1B], g_d, s_none, 0l)
            vt_pt(raw, VtProfRole.gemm_fc2)
            pc_post.boff = uint(lo.ff1b_b)
            pc_post.ascale = 0.5
            pc_post.lnoff = uint(lo.nat_w)
            pc_post.lnboff = uint(lo.nat_b)
            enc_tower_post_add_ln_cls(raw, h, r.s_post_d_ln, pc_post, npos)
            vt_pt(raw, VtProfRole.post_add)
            vt_feed_enc(raw, h, cm2, r.s_rq_xb, pc_rq_xb, nd)
            vt_pt(raw, VtProfRole.rq)
            var pc_g = BatchArgs(n = uint(d), d = uint(d), map_off = g_vt_maps[rb + VT_CN_Q])
            vt_tile_enc(raw, h, v, r.s_q, pc_g, g_d)
            vt_pt(raw, VtProfRole.gemm_q)
            vt_bias_act(raw, h, r.s_bias_q, d, nd, lo.q_b, BIAS_ACT_NONE)
            vt_pt(raw, VtProfRole.bias)
            pc_g.map_off = g_vt_maps[rb + VT_CN_K]
            vt_tile_enc(raw, h, v, r.s_k, pc_g, g_d)
            vt_pt(raw, VtProfRole.gemm_k)
            vt_bias_act(raw, h, r.s_bias_k, d, nd, lo.k_b, BIAS_ACT_NONE)
            vt_pt(raw, VtProfRole.bias)
            pc_g.map_off = g_vt_maps[rb + VT_CN_V]
            vt_tile_enc(raw, h, v, r.s_v, pc_g, g_d)
            vt_pt(raw, VtProfRole.gemm_v)
            vt_bias_act(raw, h, r.s_bias_v, d, nd, lo.v_b, BIAS_ACT_NONE)
            vt_pt(raw, VtProfRole.bias)
            pc_rel.map_off = g_vt_maps[rb + VT_CN_POS]
            vt_tile_enc(raw, h, v_rel, s_rel, pc_rel, g_rel)
            vt_pt(raw, VtProfRole.gemm_rel)
            for (hh in range64(t.n_head)) {
                let hoff = hh * hs
                var pc_gq = TowerRestrideArgs(rows = uint(npos), rows_pad = uint(npos), nheads = 1u, hs = uint(hs), hsp = uint(hs), sstride = uint(d), soff = uint(hoff),
                    boff = uint(lo.bias_v + hoff))
                enc_tower_hpad16_b_cls(raw, h, r.s_cn_gq, pc_gq, (npos * hs + 255l) / 256l)
                var pc_gp = TowerRestrideArgs(rows = uint(ww), rows_pad = uint(ww), nheads = 1u, hs = uint(hs), hsp = uint(hs), sstride = uint(d), soff = uint(hoff))
                enc_tower_hpad16_cls(raw, h, r.s_cn_gp, pc_gp, (ww * hs + 255l) / 256l)
                var pc_rg = F16GemmArgs(n = uint(hs), d = uint(ww), rows = uint(npos), woff = 0u, ybase = 0u, ystride = uint(ww), ksplit = 0u)
                f16_gemm_enc(arm.gemm_khr, raw, h, r.s_cn_rg, pc_rg, npos, ww)
                pc_attn_r.buoff = uint(lo.bias_u)
                pc_attn_r.hoff = uint(hoff)
                if (attn64) {
                    enc_tower_cn_attn_r64_cls(raw, h, r.s_cn_attn_r, pc_attn_r, wg_attn)
                } else {
                    enc_tower_cn_attn_r_cls(raw, h, r.s_cn_attn_r, pc_attn_r, wg_attn)
                }
            }
            vt_pt(raw, VtProfRole.attn)
            vt_mm_clamped(raw, h, no_clamp, r.s_rq_att, nd, r.s_o, v, d, d, g_vt_maps[rb + VT_CN_O], g_d, s_none, 0l)
            vt_pt(raw, VtProfRole.gemm_o)
            pc_post.boff = uint(lo.o_b)
            pc_post.ascale = 1.0
            pc_post.lnoff = uint(lo.ncv_w)
            pc_post.lnboff = uint(lo.ncv_b)
            enc_tower_post_add_ln_cls(raw, h, r.s_post_o_ln, pc_post, npos)
            vt_pt(raw, VtProfRole.post_add)
            vt_mm_clamped(raw, h, no_clamp, r.s_rq_xb, nd, s_h2, v2d, d, 2l * d, g_vt_maps[rb + VT_CN_PW1], g_2d, s_none, 0l)
            vt_pt(raw, VtProfRole.gemm_pw)
            vt_bias_act(raw, h, r.s_bias_hg, 2l * d, n2d, lo.pw1_b, BIAS_ACT_NONE)
            vt_pt(raw, VtProfRole.bias)
            enc_tower_glu_sig_cls(raw, h, r.s_glu_sig, pc_glu, (nd + 255l) / 256l)
            vt_pt(raw, VtProfRole.glu)
            pc_dw.woff = uint(lo.dw_w)
            pc_dw.bnwoff = uint(lo.bn_w)
            pc_dw.bnboff = uint(lo.bn_b)
            enc_tower_cn_dw_cls(raw, h, r.s_cn_dw, pc_dw, (nd + 255l) / 256l)
            vt_pt(raw, VtProfRole.dwconv)
            vt_mm_clamped(raw, h, no_clamp, r.s_rq_yd, nd, r.s_o, v, d, d, g_vt_maps[rb + VT_CN_PW2], g_d, s_none, 0l)
            vt_pt(raw, VtProfRole.gemm_pw)
            pc_post.boff = uint(lo.pw2_b)
            pc_post.ascale = 1.0
            pc_post.lnoff = uint(lo.nf2_w)
            pc_post.lnboff = uint(lo.nf2_b)
            enc_tower_post_add_ln_cls(raw, h, r.s_post_o_ln, pc_post, npos)
            vt_pt(raw, VtProfRole.post_add)
            vt_mm_clamped(raw, h, no_clamp, r.s_rq_xb, nd, r.s_hg, vff, d, ff, g_vt_maps[rb + VT_CN_FF2A], g_ff, s_none, 0l)
            vt_pt(raw, VtProfRole.gemm_fc1)
            vt_bias_act(raw, h, r.s_bias_hg, ff, nff, lo.ff2a_b, BIAS_ACT_SILU)
            vt_pt(raw, VtProfRole.bias)
            vt_mm_clamped(raw, h, no_clamp, r.s_rq_hg, nff, r.s_down, v, ff, d, g_vt_maps[rb + VT_CN_FF2B], g_d, s_none, 0l)
            vt_pt(raw, VtProfRole.gemm_fc2)
            pc_post.boff = uint(lo.ff2b_b)
            pc_post.ascale = 0.5
            pc_post.ln_on = 0u
            enc_tower_post_add_ln_cls(raw, h, r.s_post_d_ln, pc_post, npos)
            vt_pt(raw, VtProfRole.post_add)
            pc_post.ln_on = 1u
            pc_ln.woff = uint(lo.nout_w)
            pc_ln.boff = uint(lo.nout_b)
            enc_tower_ln_cls(raw, h, r.s_ln_x, pc_ln, npos)
            vt_pt(raw, VtProfRole.ln)
        }
        vhz_dep(raw, h, VT_X, 0u, true)
        cmd_copy_range(raw, r.x_dev, 0l, r.host.buf, 0l, bx)
        vt_pt(raw, VtProfRole.copy_out)
    }
    return vt_served(r, s.x, bx, npos, t.n_layer)
}

[cold_path]
def private vt_sets_g4v(var r : VtResident) {
    let q8e = kq_elems(KqFmt.q8)
    let cap = r.cap_rows
    let bx = cap * r.d * 4l
    let bff = cap * r.ff * 4l
    let bq = cap * max(r.d, r.ff)
    let bs = bq / q8e * 4l
    let bh = cap * r.d * 2l
    let btabs = cap * r.hs * 4l
    let khr = r.fa_khr
    r.s_rms = set_tower_rms_cls(fixed_array(r.x_dev, r.xb_dev, r.norms_dev), fixed_array(bx, bx, r.norms_bytes), fixed_array(VT_X, VT_XB, 0u))
    r.s_rq_xb = set_tower_clamp_rq_cls(fixed_array(r.xb_dev, r.xq_dev, r.xs_dev), fixed_array(bx, bq, bs), fixed_array(VT_XB, VT_XQ, VT_XQ))
    r.s_rq_att = set_tower_clamp_rq_cls(fixed_array(r.att_dev, r.xq_dev, r.xs_dev), fixed_array(bx, bq, bs), fixed_array(VT_ATT, VT_XQ, VT_XQ))
    r.s_rq_hg = set_tower_clamp_rq_cls(fixed_array(r.hg_dev, r.xq_dev, r.xs_dev), fixed_array(bff, bq, bs), fixed_array(VT_HG, VT_XQ, VT_XQ))
    r.s_clamp_q = set_tower_clamp_cls(fixed_array(r.q_dev), fixed_array(bx), fixed_array(VT_Q))
    r.s_clamp_k = set_tower_clamp_cls(fixed_array(r.k_dev), fixed_array(bx), fixed_array(VT_K))
    r.s_clamp_v = set_tower_clamp_cls(fixed_array(r.v_dev), fixed_array(bx), fixed_array(VT_V))
    r.s_clamp_yo = set_tower_clamp_cls(fixed_array(r.yo_dev), fixed_array(bx), fixed_array(VT_YO))
    r.s_clamp_yd = set_tower_clamp_cls(fixed_array(r.yd_dev), fixed_array(bx), fixed_array(VT_YD))
    r.s_hrms_q = set_tower_rms_cls(fixed_array(r.q_dev, r.q2_dev, r.norms_dev), fixed_array(bx, bx, r.norms_bytes), fixed_array(VT_Q, VT_Q2, 0u))
    r.s_hrms_k = set_tower_rms_cls(fixed_array(r.k_dev, r.k2_dev, r.norms_dev), fixed_array(bx, bx, r.norms_bytes), fixed_array(VT_K, VT_K2, 0u))
    r.s_hrms_v = set_tower_rms_cls(fixed_array(r.v_dev, r.v2_dev, r.norms_dev), fixed_array(bx, bx, r.norms_bytes), fixed_array(VT_V, VT_V2, 0u))
    r.s_rope_q = set_tower_rope2d_cls(fixed_array(r.q2_dev, r.tabs_dev), fixed_array(bx, btabs), fixed_array(VT_Q2, 0u))
    r.s_rope_k = set_tower_rope2d_cls(fixed_array(r.k2_dev, r.tabs_dev), fixed_array(bx, btabs), fixed_array(VT_K2, 0u))
    r.s_cvt_k = set_tower_clamp_cvt_cls(fixed_array(r.k2_dev, r.k16_dev), fixed_array(bx, bh), fixed_array(VT_K2, VT_K16))
    r.s_cvt_v = set_tower_clamp_cvt_cls(fixed_array(r.v2_dev, r.v16_dev), fixed_array(bx, bh), fixed_array(VT_V2, VT_V16))
    r.s_fa = fa_stamp_set(khr, fixed_array(r.q2_dev, r.k16_dev, r.v16_dev, r.att_dev), fixed_array(bx, bh, bh, bx), fixed_array(VT_Q2, VT_K16, VT_V16, VT_ATT))
    r.s_post_o = set_tower_post_add_cls(fixed_array(r.x_dev, r.yo_dev, r.xb_dev, r.norms_dev), fixed_array(bx, bx, bx, r.norms_bytes), fixed_array(VT_X, VT_YO, VT_XB, 0u))
    r.s_post_d = set_tower_post_add_cls(fixed_array(r.x_dev, r.yd_dev, r.xb_dev, r.norms_dev), fixed_array(bx, bx, bx, r.norms_bytes), fixed_array(VT_X, VT_YD, VT_XB, 0u))
    r.s_geglu = set_tower_geglu_cls(fixed_array(r.hg_dev, r.hu_dev), fixed_array(bff, bff), fixed_array(VT_HG, VT_HU))
}

[cold_path]
def private vt_sets_ln(var r : VtResident) {
    let q8e = kq_elems(KqFmt.q8)
    let cap = r.cap_rows
    let bx = cap * r.d * 4l
    let bff = cap * r.ff * 4l
    let bq = cap * max(r.d, r.ff)
    let bs = bq / q8e * 4l
    let bp = cap * r.pad_dp * 4l
    let bph = cap * r.pad_dp * 2l
    r.s_ln = set_tower_ln_cls(fixed_array(r.x_dev, r.xb_dev, r.norms_dev), fixed_array(bx, bx, r.norms_bytes), fixed_array(VT_X, VT_XB, 0u))
    r.s_rq_xb = set_tower_clamp_rq_cls(fixed_array(r.xb_dev, r.xq_dev, r.xs_dev), fixed_array(bx, bq, bs), fixed_array(VT_XB, VT_XQ, VT_XQ))
    r.s_rq_att = set_tower_clamp_rq_cls(fixed_array(r.att_dev, r.xq_dev, r.xs_dev), fixed_array(bx, bq, bs), fixed_array(VT_ATT, VT_XQ, VT_XQ))
    r.s_rq_hg = set_tower_clamp_rq_cls(fixed_array(r.hg_dev, r.xq_dev, r.xs_dev), fixed_array(bff, bq, bs), fixed_array(VT_HG, VT_XQ, VT_XQ))
    r.s_bias_q = set_tower_bias_act_cls(fixed_array(r.q_dev, r.norms_dev), fixed_array(bx, r.norms_bytes), fixed_array(VT_Q, 0u))
    r.s_bias_k = set_tower_bias_act_cls(fixed_array(r.k_dev, r.norms_dev), fixed_array(bx, r.norms_bytes), fixed_array(VT_K, 0u))
    r.s_bias_v = set_tower_bias_act_cls(fixed_array(r.v_dev, r.norms_dev), fixed_array(bx, r.norms_bytes), fixed_array(VT_V, 0u))
    r.s_bias_hg = set_tower_bias_act_cls(fixed_array(r.hg_dev, r.norms_dev), fixed_array(bff, r.norms_bytes), fixed_array(VT_HG, 0u))
    r.s_pad_q = set_tower_hpad_cls(fixed_array(r.q_dev, r.qp_dev), fixed_array(bx, bp), fixed_array(VT_Q, VT_Q2))
    r.s_pad_k = set_tower_hpad16_cls(fixed_array(r.k_dev, r.kp16_dev), fixed_array(bx, bph), fixed_array(VT_K, VT_K16))
    r.s_pad_v = set_tower_hpad16_cls(fixed_array(r.v_dev, r.vp16_dev), fixed_array(bx, bph), fixed_array(VT_V, VT_V16))
    r.s_fa = fa_stamp_set(r.fa_khr, fixed_array(r.qp_dev, r.kp16_dev, r.vp16_dev, r.attp_dev), fixed_array(bp, bph, bph, bp), fixed_array(VT_Q2, VT_K16, VT_V16, VT_K2))
    r.s_unpad = set_tower_hunpad_cls(fixed_array(r.attp_dev, r.att_dev), fixed_array(bp, bx), fixed_array(VT_K2, VT_ATT))
    r.s_post_o_ln = set_tower_post_add_ln_cls(fixed_array(r.x_dev, r.yo_dev, r.xb_dev, r.norms_dev), fixed_array(bx, bx, bx, r.norms_bytes), fixed_array(VT_X, VT_YO, VT_XB, 0u))
    r.s_post_d_ln = set_tower_post_add_ln_cls(fixed_array(r.x_dev, r.yd_dev, r.xb_dev, r.norms_dev), fixed_array(bx, bx, bx, r.norms_bytes), fixed_array(VT_X, VT_YD, VT_XB, 0u))
    if (r.cm2) {
        let bxh = (cap + TILE_READ_SLACK) * max(r.d, r.ff) * 2l
        r.s_ln16 = set_tower_ln16_cls(fixed_array(r.x_dev, r.norms_dev, r.xh_dev), fixed_array(bx, r.norms_bytes, bxh), fixed_array(VT_X, 0u, VT_XQ))
        r.s_post_o_ln16 = set_tower_post_add_ln16_cls(fixed_array(r.x_dev, r.yo_dev, r.norms_dev, r.xh_dev), fixed_array(bx, bx, r.norms_bytes, bxh), fixed_array(VT_X, VT_YO, 0u, VT_XQ))
        r.s_post_d_ln16 = set_tower_post_add_ln16_cls(fixed_array(r.x_dev, r.yd_dev, r.norms_dev, r.xh_dev), fixed_array(bx, bx, r.norms_bytes, bxh), fixed_array(VT_X, VT_YD, 0u, VT_XQ))
        r.s_pad_qb = set_tower_hpad_b_cls(fixed_array(r.q_dev, r.qp_dev, r.norms_dev), fixed_array(bx, bp, r.norms_bytes), fixed_array(VT_Q, VT_Q2, 0u))
        r.s_pad_kb = set_tower_hpad16_b_cls(fixed_array(r.k_dev, r.kp16_dev, r.norms_dev), fixed_array(bx, bph, r.norms_bytes), fixed_array(VT_K, VT_K16, 0u))
        r.s_pad_vb = set_tower_hpad16_b_cls(fixed_array(r.v_dev, r.vp16_dev, r.norms_dev), fixed_array(bx, bph, r.norms_bytes), fixed_array(VT_V, VT_V16, 0u))
        r.s_unpad16 = set_tower_hunpad16_cls(fixed_array(r.attp_dev, r.xh_dev), fixed_array(bp, bxh), fixed_array(VT_K2, VT_XQ))
        r.s_bias_hg16 = set_tower_bias_act_cvt16_cls(fixed_array(r.hg_dev, r.norms_dev, r.xh_dev), fixed_array(bff, r.norms_bytes, bxh), fixed_array(VT_HG, 0u, VT_XQ))
    }
}

[cold_path]
def private vt_cm2_set(r : VtResident; tc : int64; y_dev : uint64; y_bytes : int64; y_bit : uint) : VkDescriptorSet {
    let bxh = (r.cap_rows + TILE_READ_SLACK) * max(r.d, r.ff) * 2l
    return cm2_cls_set(int(KqFmt.q8), tc, fixed_array(r.wq_dev, r.ws_dev, r.meta.buf, r.xh_dev, y_dev),
        fixed_array(r.wq_bytes, r.ws_bytes, r.meta_bytes, bxh, y_bytes), fixed_array(0u, 0u, 0u, VT_XQ, y_bit))
}

[cold_path]
def private vt_batch_set(r : VtResident; v : int; y_dev : uint64; y_bytes : int64; y_bit : uint) : VkDescriptorSet {
    let bq = r.cap_rows * max(r.d, r.ff)
    return q8_batch_cls_set(v, fixed_array(r.wq_dev, r.ws_dev, r.meta.buf, r.xq_dev, r.xs_dev, y_dev),
        fixed_array(r.wq_bytes, r.ws_bytes, r.meta_bytes, bq, bq / kq_elems(KqFmt.q8) * 4l, y_bytes), fixed_array(0u, 0u, 0u, VT_XQ, VT_XQ, y_bit))
}

[cold_path]
def private vt_tile_set(r : VtResident; t : VtTile; y_dev : uint64; y_bytes : int64; y_bit : uint) : VkDescriptorSet
    => t.tc > 0l ? vt_cm2_set(r, t.tc, y_dev, y_bytes, y_bit) : vt_batch_set(r, t.v, y_dev, y_bytes, y_bit)

def private vt_tile_words(t : VtTile) : string => t.tc > 0l ? "token column {t.tc}" : "variant {t.v}"

[cold_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vt_sets_d(var r : VtResident; t : VtTile) : VtBatchSetsD {
    let key = vt_tile_key(t)
    if (!key_exists(r.sets_d, key)) {
        let bx = r.cap_rows * r.d * 4l
        r.sets_d[key] = VtBatchSetsD(s_q = vt_tile_set(r, t, r.q_dev, bx, VT_Q), s_k = vt_tile_set(r, t, r.k_dev, bx, VT_K),
            s_v = vt_tile_set(r, t, r.v_dev, bx, VT_V), s_o = vt_tile_set(r, t, r.yo_dev, bx, VT_YO), s_down = vt_tile_set(r, t, r.yd_dev, bx, VT_YD))
        to_log(LOG_INFO, "dasLLAMA vulkan tower: d-wide sets at {r.cap_rows} rows - {vt_tile_words(t)} (d {r.d})\n")
    }
    return r.sets_d[key]
}

[arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vt_sets_ff(var r : VtResident; t : VtTile) : VtBatchSetsFf {
    let key = vt_tile_key(t)
    if (!key_exists(r.sets_ff, key)) {
        r.sets_ff[key] = vt_sets_ff_build(r, t)
    }
    return r.sets_ff[key]
}

[cold_path]
def private vt_sets_ff_build(r : VtResident; t : VtTile) : VtBatchSetsFf {
    let bff = r.cap_rows * r.ff * 4l
    to_log(LOG_INFO, "dasLLAMA vulkan tower: ff-wide sets at {r.cap_rows} rows - {vt_tile_words(t)} (ff {r.ff})\n")
    return VtBatchSetsFf(s_hg = vt_tile_set(r, t, r.hg_dev, bff, VT_HG), s_hu = vt_tile_set(r, t, r.hu_dev, bff, VT_HU))
}

[cold_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vt_set_qkv(var r : VtResident; t : VtTile) : VkDescriptorSet {
    let key = vt_tile_key(t)
    if (!key_exists(r.sets_qkv, key)) {
        r.sets_qkv[key] = vt_tile_set(r, t, r.qkv_dev, 3l * r.cap_rows * r.d * 4l, VT_QKV)
        to_log(LOG_INFO, "dasLLAMA vulkan tower: fused qkv set at {r.cap_rows} rows - {vt_tile_words(t)} (3d {3l * r.d})\n")
    }
    return r.sets_qkv[key]
}

let private VT_NO_TILE = VtTile(v = -1, tc = 0l)

//! the GEMM sets of the encode's tiles bound: a tile seen before is reused, a new one built once
def private vt_bind_tile_sets(var r : VtResident; v, vff : VtTile; vqkv : VtTile = VT_NO_TILE) {
    if (r.bound_d != vt_tile_key(v)) {
        var sd = vt_sets_d(r, v)
        r.s_q = sd.s_q
        r.s_k = sd.s_k
        r.s_v = sd.s_v
        r.s_o = sd.s_o
        r.s_down = sd.s_down
        r.bound_d = vt_tile_key(v)
    }
    if (r.bound_ff != vt_tile_key(vff)) {
        var sf = vt_sets_ff(r, vff)
        r.s_hg = sf.s_hg
        r.s_hu = sf.s_hu
        r.bound_ff = vt_tile_key(vff)
    }
    if (r.fused && r.bound_qkv != vt_tile_key(vqkv)) {
        r.s_qkv = vt_set_qkv(r, vqkv)
        r.bound_qkv = vt_tile_key(vqkv)
    }
}

[cold_path]
def private vt_sets_q3v(var r : VtResident) {
    let q8e = kq_elems(KqFmt.q8)
    let cap = r.cap_rows
    let bx = cap * r.d * 4l
    let bff = cap * r.ff * 4l
    let bq = cap * max(r.d, r.ff)
    let bs = bq / q8e * 4l
    let bp = cap * r.pad_dp * 4l
    let bph = cap * r.pad_dp * 2l
    let btab = cap * (r.hs / 2l) * 4l
    r.s_ln = set_tower_ln_cls(fixed_array(r.x_dev, r.xb_dev, r.norms_dev), fixed_array(bx, bx, r.norms_bytes), fixed_array(VT_X, VT_XB, 0u))
    r.s_rq_xb = set_tower_clamp_rq_cls(fixed_array(r.xb_dev, r.xq_dev, r.xs_dev), fixed_array(bx, bq, bs), fixed_array(VT_XB, VT_XQ, VT_XQ))
    r.s_rq_att = set_tower_clamp_rq_cls(fixed_array(r.att_dev, r.xq_dev, r.xs_dev), fixed_array(bx, bq, bs), fixed_array(VT_ATT, VT_XQ, VT_XQ))
    r.s_rq_hg = set_tower_clamp_rq_cls(fixed_array(r.hg_dev, r.xq_dev, r.xs_dev), fixed_array(bff, bq, bs), fixed_array(VT_HG, VT_XQ, VT_XQ))
    r.s_bias_qkv = set_tower_bias_act_cls(fixed_array(r.qkv_dev, r.norms_dev), fixed_array(3l * bx, r.norms_bytes), fixed_array(VT_QKV, 0u))
    r.s_bias_hg = set_tower_bias_act_cls(fixed_array(r.hg_dev, r.norms_dev), fixed_array(bff, r.norms_bytes), fixed_array(VT_HG, 0u))
    r.s_rope_tq = set_tower_rope_tab_cls(fixed_array(r.qkv_dev, r.cos_dev, r.sin_dev), fixed_array(3l * bx, btab, btab), fixed_array(VT_QKV, 0u, 0u))
    r.s_rope_tk = r.s_rope_tq   // the same set: the k slot is a column offset in the push constants
    r.s_pad_q = set_tower_hpad_cls(fixed_array(r.qkv_dev, r.qp_dev), fixed_array(3l * bx, bp), fixed_array(VT_QKV, VT_Q2))
    r.s_pad_k = set_tower_hpad16_cls(fixed_array(r.qkv_dev, r.kp16_dev), fixed_array(3l * bx, bph), fixed_array(VT_QKV, VT_K16))
    r.s_pad_v = set_tower_hpad16_cls(fixed_array(r.qkv_dev, r.vp16_dev), fixed_array(3l * bx, bph), fixed_array(VT_QKV, VT_V16))
    r.s_fa = fa_stamp_set(r.fa_khr, fixed_array(r.qp_dev, r.kp16_dev, r.vp16_dev, r.attp_dev), fixed_array(bp, bph, bph, bp), fixed_array(VT_Q2, VT_K16, VT_V16, VT_K2))
    r.s_unpad = set_tower_hunpad_cls(fixed_array(r.attp_dev, r.att_dev), fixed_array(bp, bx), fixed_array(VT_K2, VT_ATT))
    r.s_post_o_ln = set_tower_post_add_ln_cls(fixed_array(r.x_dev, r.yo_dev, r.xb_dev, r.norms_dev), fixed_array(bx, bx, bx, r.norms_bytes), fixed_array(VT_X, VT_YO, VT_XB, 0u))
    r.s_post_d_ln = set_tower_post_add_ln_cls(fixed_array(r.x_dev, r.yd_dev, r.xb_dev, r.norms_dev), fixed_array(bx, bx, bx, r.norms_bytes), fixed_array(VT_X, VT_YD, VT_XB, 0u))
}

//! record `rid` = [wblk0, 0, rows, 0] at word 4 rid and its map of `groups` words at the cursor; returns the map's word offset and advances the cursor
def vt_sched_map(var mp : uint?; var cur : int64&; rid : int64; wblk0, rows, groups : int64) : uint {
    unsafe {
        mp[rid * 4l] = uint(wblk0)
        mp[rid * 4l + 1l] = 0u
        mp[rid * 4l + 2l] = uint(rows)
        mp[rid * 4l + 3l] = 0u
        let at = cur
        for (g in range64(groups)) {
            mp[at + g] = uint(rid)
        }
        cur += groups
        return uint(at)
    }
}

//! the tier's want (DASLLAMA_GPU) and then the device init - the gate every chain and every twin cell reads first: nothing
//! reads `g_gpu` ahead of this answer
def public vulkan_tower_arms : bool => gpu_want_arms_tier() && vk_moe_init()

//! the arm every chain reads after its shape check: the want and the device first, then the flash arm and the KHR tile where
//! the arm - or the GEMM feed off cm2, when the chain's GEMMs ride it (`gemm_off_cm2`) - reads it; `ok` false counted the decline
def private vt_arm(r : VtResident; gemm_off_cm2 : bool = false) : tuple<ok : bool; khr : bool; gemm_khr : bool> {
    if (!vulkan_tower_arms()) {
        vt_decline(r, VulkanTowerDecline.device)
        return (ok = false, khr = false, gemm_khr = false)
    }
    let khr = !cm2_fa_on()
    let gemm_khr = gemm_off_cm2 && g_gpu.coopmat_mode != COOPMAT_CM2
    if ((khr || gemm_khr) && !khr_kq_tile_on()) {
        vt_decline(r, VulkanTowerDecline.device)
        return (ok = false, khr = khr, gemm_khr = gemm_khr)
    }
    return (ok = true, khr = khr, gemm_khr = gemm_khr)
}

//! the encode's landing, one text for every chain: the residual rows back to the host, the counters, the announcement once
def private vt_served(var r : VtResident; var x : array<float>; bx, npos, n_layer : int64) : bool
    => vt_served_from(r, r.host, x, bx, npos, n_layer)

def private vt_served_from(var r : VtResident; hb : HostBuf; var x : array<float>; bx, npos, n_layer : int64) : bool {
    unsafe {
        memcpy(addr<void?>(x[0]), hb.mapped, bx)
    }
    g_vt_encodes++
    g_vt_rows += npos
    g_vt_blocks += n_layer
    vt_announce(r)
    return true
}

let private VT_LN_GEMMS = 6l   //! a pre-LN block's GEMM records, in the order below
let private VT_LN_Q = 0l
let private VT_LN_K = 1l
let private VT_LN_V = 2l
let private VT_LN_O = 3l
let private VT_LN_FC1 = 4l
let private VT_LN_FC2 = 5l

[arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vt_ln_regions(o : LayerOffs; d, ff : int64; var out : VtRegion[int(VT_LN_GEMMS)]) {
    out[VT_LN_Q] = VtRegion(woff = o.q_w, n = d, rows = d)
    out[VT_LN_K] = VtRegion(woff = o.k_w, n = d, rows = d)
    out[VT_LN_V] = VtRegion(woff = o.v_w, n = d, rows = d)
    out[VT_LN_O] = VtRegion(woff = o.o_w, n = d, rows = d)
    out[VT_LN_FC1] = VtRegion(woff = o.fc1_w, n = d, rows = ff)
    out[VT_LN_FC2] = VtRegion(woff = o.fc2_w, n = ff, rows = d)
}

[cold_path]
def private vt_upload_ln(var r : VtResident; key : uint64; qblob : PlaneI8; qscales : PlaneF; d, ff : int64;
                         blob : PlaneF; rows_off, rows_n : int64; lay : Q8RepackLayout; offs : array<LayerOffs>; conv2_w : int64 = -1l) : bool {
    var regions : array<VtRegion>
    regions |> reserve(long_length(offs) * VT_LN_GEMMS + 1l)
    if (conv2_w >= 0l) {
        regions |> push(VtRegion(woff = conv2_w, n = 3l * d, rows = d))
    }
    var recs : VtRegion[int(VT_LN_GEMMS)]
    for (o in offs) {
        vt_ln_regions(o, d, ff, recs)
        regions |> push_from(recs)
    }
    let ok = vt_upload(r, key, qblob, qscales, regions, blob, rows_off, rows_n, 0l, lay)
    delete regions
    return ok
}

def private g3v_ln_offs(t : Gemma3vTower; l : int64) : LayerOffs {
    let o = gemma3v_block_offs(t, l)
    return LayerOffs(ln1_w = o.ln1_w, ln1_b = o.ln1_b, ln2_w = o.ln2_w, ln2_b = o.ln2_b,
        q_w = o.w[GEMMA3V_GEMM_Q], q_b = o.b[GEMMA3V_GEMM_Q], k_w = o.w[GEMMA3V_GEMM_K], k_b = o.b[GEMMA3V_GEMM_K],
        v_w = o.w[GEMMA3V_GEMM_V], v_b = o.b[GEMMA3V_GEMM_V], o_w = o.w[GEMMA3V_GEMM_O], o_b = o.b[GEMMA3V_GEMM_O],
        fc1_w = o.w[GEMMA3V_GEMM_UP], fc1_b = o.b[GEMMA3V_GEMM_UP], fc2_w = o.w[GEMMA3V_GEMM_DOWN], fc2_b = o.b[GEMMA3V_GEMM_DOWN])
}

//! fc2's map words are counted apart from the d-wide GEMMs': under a split its map stacks every plane's tiles
def private vt_ln_meta_bytes(nl, g_d, g_ff, g_fc2 : int64) : int64 => (nl * VT_LN_GEMMS * 4l + nl * (4l * g_d + g_ff + g_fc2)) * 4l

def private vt_ln_fc2_pick(d, ff, npos, cap, tc_d : int64; cm2 : bool) : tuple<tc : int64; nsplit : int64; ksplit : int64> {
    if (!cm2 || npos < VT_SK_MIN_ROWS) {
        return (tc = tc_d, nsplit = 1l, ksplit = 0l)
    }
    let sp = cm2_gemm_pick(d, npos, ff, 0l, VT_SPLITK_MAX_PLANES * cap * d * 4l)
    return sp.nsplit > 1l ? sp : (tc = tc_d, nsplit = 1l, ksplit = 0l)
}

def private vt_ln_fc2_groups(d, ff, npos, tc_d : int64; cm2 : bool; g_d : int64) : int64 {
    let sk = vt_ln_fc2_pick(d, ff, npos, vt_cap_rows(npos), tc_d, cm2)
    return sk.nsplit > 1l ? cm2_tiles_at(d, npos, sk.tc) * sk.nsplit : g_d
}

[cold_path]
def private vt_sk_release(var r : VtResident) {
    if (r.sk_bytes > 0l) {
        destroy_device_buf(r.sk_dev)
        r.sk_bytes = 0l
    }
    r.sets_sk |> clear()
}

//! false = no split serves (a class, the range or the allocation refused): the chain runs the fc2 GEMM whole, never declines
def private vt_sk_ensure(var r : VtResident; tc, nsplit : int64) : bool {
    if (!cm2_cls_ensure(int(KqFmt.q8), tc) || !ensure_splitk_reduce_cls()) {
        return false
    }
    let need = nsplit * r.cap_rows * r.d * 4l
    if (!vt_range_ok(need) || (r.sk_bytes < need && !vt_sk_build(r, nsplit, need))) {
        return false
    }
    if (!key_exists(r.sets_sk, int(tc))) {
        r.sets_sk[int(tc)] = vt_cm2_set(r, tc, r.sk_dev, r.sk_bytes, VT_SPLITK_PLANES)
    }
    return true
}

[cold_path]
def private vt_sk_build(var r : VtResident; nsplit, need : int64) : bool {
    vt_sk_release(r)
    r.sk_dev = try_device_buf(need)
    if (r.sk_dev == 0ul) {
        return false
    }
    r.sk_bytes = need
    r.s_splitk_reduce = set_splitk_reduce_cls(fixed_array(r.sk_dev, r.yd_dev), fixed_array(need, r.cap_rows * r.d * 4l), fixed_array(VT_SPLITK_PLANES, VT_YD))
    to_log(LOG_INFO, "dasLLAMA vulkan tower: the fc2 GEMM splits K into {nsplit} chunks (DASLLAMA_CM2_SPLITK={cm2_splitk_env()}: 0 = the wave model's pick) - planes at {nsplit} x {r.cap_rows} rows (d {r.d})\n")
    return true
}

def private vt_ln_scratch(var r : VtResident; nl, npos, d, ff, hs, dp, g_d, g_ff : int64; khr, cm2 : bool; tc_d, tc_ff : int64; v, vff : int) : bool {
    let meta_bytes = vt_ln_meta_bytes(nl, g_d, g_ff, vt_ln_fc2_groups(d, ff, npos, tc_d, cm2, g_d))
    if (!vt_scratch_fits(r, npos, d, ff, dp, meta_bytes, khr, false, 0l)) {
        if (!vt_scratch_build(r, npos, d, ff, hs, dp, meta_bytes, khr, false, 0l)) {
            return false
        }
        vt_sets_ln(r)
    }
    vt_bind_tile_sets(r, VtTile(v = v, tc = tc_d), VtTile(v = vff, tc = tc_ff))
    return true
}

//! the caller keys `r` first; `lnp_w` >= 0 folds the post-norm (weight at `lnp_w`, bias at `lnp_b`, in the norms plane) into the
//! last post-add, landing f32 rows in `xb`; else in `x`
[arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vt_ln_chain(var r : VtResident; offs : array<LayerOffs>; k_bias : bool; fc1_act : uint; eps : float;   // nolint:STYLE038 — the dispatch chain is the shape
                        norms_base, d, ff, hs, hsp, nheads, npos : int64; khr : bool; v, vff : int; var x : array<float>;
                        lnp_w : int64 = -1l; lnp_b : int64 = -1l; var xb : array<float>& = g_vt_no_rows) : bool {
    let ln_post = lnp_w >= 0l
    let q8e = kq_elems(KqFmt.q8)
    let nl = long_length(offs)
    let dp = nheads * hsp
    let cm2 = vt_cm2_feed()
    let tc_d = cm2 ? cm2_tile_cols(d, npos) : 0l
    let tc_ff = cm2 ? cm2_tile_cols(ff, npos) : 0l
    if (cm2 && (!cm2_cls_ensure(int(KqFmt.q8), tc_d) || !cm2_cls_ensure(int(KqFmt.q8), tc_ff) || !vt_ln16_ensure())) {
        return vt_flush_and_decline(r, VulkanTowerDecline.device)
    }
    let g_d = cm2 ? cm2_tiles_at(d, npos, tc_d) : vt_gemm_groups(d, npos)
    let g_ff = cm2 ? cm2_tiles_at(ff, npos, tc_ff) : vt_gemm_groups(ff, npos)
    let nrec = nl * VT_LN_GEMMS
    var sk = vt_ln_fc2_pick(d, ff, npos, vt_cap_rows(npos), tc_d, cm2)
    if (!vt_ln_scratch(r, nl, npos, d, ff, hs, dp, g_d, g_ff, khr, cm2, tc_d, tc_ff, v, vff)) {
        return vt_flush_and_decline(r, VulkanTowerDecline.memory)
    }
    if (sk.nsplit > 1l && !vt_sk_ensure(r, sk.tc, sk.nsplit)) {
        sk = (tc = tc_d, nsplit = 1l, ksplit = 0l)
    }
    let split = sk.nsplit > 1l
    let g_fc2 = split ? cm2_tiles_at(d, npos, sk.tc) * sk.nsplit : g_d
    var s_fc2 = split ? r.sets_sk[int(sk.tc)] : r.s_down
    let tile_d = VtTile(v = v, tc = tc_d)
    let tile_ff = VtTile(v = vff, tc = tc_ff)
    let tile_fc2 = VtTile(v = v, tc = cm2 ? sk.tc : 0l)
    g_vt_maps |> reserve(nrec)
    g_vt_maps |> resize(nrec)
    unsafe {
        var mp = reinterpret<uint?>(r.meta.mapped)
        var cur = nrec * 4l
        var recs : VtRegion[int(VT_LN_GEMMS)]
        for (l in range64(nl)) {
            vt_ln_regions(offs[l], d, ff, recs)
            for (g in range64(VT_LN_GEMMS)) {
                let rid = l * VT_LN_GEMMS + g
                let tile = g == VT_LN_FC1 ? tile_ff : (g == VT_LN_FC2 ? tile_fc2 : tile_d)
                g_vt_maps[rid] = vt_sched_map(mp, cur, rid, recs[g].woff / q8e, vt_tile_rows(tile, npos), g == VT_LN_FC1 ? g_ff : (g == VT_LN_FC2 ? g_fc2 : g_d))
            }
        }
    }
    let bx = npos * d * 4l
    if (r.x_ready) {
        vt_x_drop_pending(r)
    } else {
        upload_region_at(r.x_dev, 0l, unsafe(addr<uint8 const?>(x[0])), bx)
    }
    let nd = npos * d
    let nff = npos * ff
    let npos_pad = vt_cap_rows(npos)
    let wg_nd = rq_wgs(false, nd)
    let wg_nff = rq_wgs(false, nff)
    let wg_pad = (npos_pad * dp + 255l) / 256l
    let wg_unpad = (npos * dp + 255l) / 256l
    let wg_fa = fa_stamp_wgs(khr, nheads, npos)
    var pc_fa = FaArgs(rows = uint(npos), w0 = 0u, qd = uint(dp), kvd = uint(dp), kv_mul = 1u, kvbase = 0u, scale = 1.0 / sqrt(float(hs)), window = 0u, sinkoff = 0u)
    var pc_rs = TowerRestrideArgs(rows = uint(npos), rows_pad = uint(npos_pad), nheads = uint(nheads), hs = uint(hs), hsp = uint(hsp), sstride = uint(d), soff = 0u)
    var pc_rq = TowerClampRqArgs(inbase = 0u, nblk = uint(nd / q8e), lo = -VT_NO_CLAMP, hi = VT_NO_CLAMP)
    var pc_rqh = TowerClampRqArgs(inbase = 0u, nblk = uint(nff / q8e), lo = -VT_NO_CLAMP, hi = VT_NO_CLAMP)
    var pc_gd = BatchArgs(n = uint(d), d = uint(d), map_off = 0u)
    var pc_gff = BatchArgs(n = uint(d), d = uint(ff), map_off = 0u)
    var pc_dn = BatchArgs(n = uint(ff), d = uint(d), map_off = 0u, ksplit = uint(sk.ksplit))
    let nd_fc2 = vt_tile_rows(tile_fc2, npos) * d
    var pc_red = SkRedArgs(nelem = uint(nd_fc2), k = uint(sk.nsplit), ybase = 0u)
    let wg_red = (nd_fc2 + 1_023l) / 1_024l
    var pc_bias = TowerBiasArgs(d = uint(d), nelem = uint(nd), boff = 0u, act = BIAS_ACT_NONE)
    var pc_gelu = TowerBiasArgs(d = uint(ff), nelem = uint(nff), boff = 0u, act = fc1_act)
    vt_chain_one_shot("whisper-class", nl, npos) $(raw : VkCommandBuffer; var h : VkHaz) {
        for (l in range64(nl)) {
            let o = offs[l]
            let rb = l * VT_LN_GEMMS
            if (l == 0l) {
                var pc = TowerLnArgs(d = uint(d), rows = uint(npos), woff = uint(o.ln1_w - norms_base), boff = uint(o.ln1_b - norms_base), eps = eps)
                if (cm2) {
                    enc_tower_ln16_cls(raw, h, r.s_ln16, pc, npos)
                } else {
                    enc_tower_ln_cls(raw, h, r.s_ln, pc, npos)
                }
                vt_pt(raw, VtProfRole.ln)
            }
            if (!cm2) {
                enc_tower_clamp_rq_cls(raw, h, r.s_rq_xb, pc_rq, wg_nd)
                vt_pt(raw, VtProfRole.rq)
            }
            pc_gd.map_off = g_vt_maps[rb + VT_LN_Q]
            vt_tile_enc(raw, h, tile_d, r.s_q, pc_gd, g_d)
            vt_pt(raw, VtProfRole.gemm_q)
            pc_gd.map_off = g_vt_maps[rb + VT_LN_K]
            vt_tile_enc(raw, h, tile_d, r.s_k, pc_gd, g_d)
            vt_pt(raw, VtProfRole.gemm_k)
            pc_gd.map_off = g_vt_maps[rb + VT_LN_V]
            vt_tile_enc(raw, h, tile_d, r.s_v, pc_gd, g_d)
            vt_pt(raw, VtProfRole.gemm_v)
            if (cm2) {
                pc_rs.boff = uint(o.q_b - norms_base)
                enc_tower_hpad_b_cls(raw, h, r.s_pad_qb, pc_rs, wg_pad)
                vt_pt(raw, VtProfRole.hpad)
                if (k_bias) {
                    pc_rs.boff = uint(o.k_b - norms_base)
                    enc_tower_hpad16_b_cls(raw, h, r.s_pad_kb, pc_rs, wg_pad)
                } else {
                    enc_tower_hpad16_cls(raw, h, r.s_pad_k, pc_rs, wg_pad)
                }
                vt_pt(raw, VtProfRole.hpad)
                pc_rs.boff = uint(o.v_b - norms_base)
                enc_tower_hpad16_b_cls(raw, h, r.s_pad_vb, pc_rs, wg_pad)
                vt_pt(raw, VtProfRole.hpad)
            } else {
                pc_bias.boff = uint(o.q_b - norms_base)
                enc_tower_bias_act_cls(raw, h, r.s_bias_q, pc_bias, wg_nd)
                vt_pt(raw, VtProfRole.bias)
                if (k_bias) {
                    pc_bias.boff = uint(o.k_b - norms_base)
                    enc_tower_bias_act_cls(raw, h, r.s_bias_k, pc_bias, wg_nd)
                    vt_pt(raw, VtProfRole.bias)
                }
                pc_bias.boff = uint(o.v_b - norms_base)
                enc_tower_bias_act_cls(raw, h, r.s_bias_v, pc_bias, wg_nd)
                vt_pt(raw, VtProfRole.bias)
                enc_tower_hpad_cls(raw, h, r.s_pad_q, pc_rs, wg_pad)
                vt_pt(raw, VtProfRole.hpad)
                enc_tower_hpad16_cls(raw, h, r.s_pad_k, pc_rs, wg_pad)
                vt_pt(raw, VtProfRole.hpad)
                enc_tower_hpad16_cls(raw, h, r.s_pad_v, pc_rs, wg_pad)
                vt_pt(raw, VtProfRole.hpad)
            }
            fa_stamp_enc(khr, hsp, false, false, false, false, raw, h, r.s_fa, pc_fa, wg_fa, true)
            vt_pt(raw, VtProfRole.fa)
            if (cm2) {
                enc_tower_hunpad16_cls(raw, h, r.s_unpad16, pc_rs, wg_unpad)
                vt_pt(raw, VtProfRole.hunpad)
            } else {
                enc_tower_hunpad_cls(raw, h, r.s_unpad, pc_rs, wg_unpad)
                vt_pt(raw, VtProfRole.hunpad)
                enc_tower_clamp_rq_cls(raw, h, r.s_rq_att, pc_rq, wg_nd)
                vt_pt(raw, VtProfRole.rq)
            }
            pc_gd.map_off = g_vt_maps[rb + VT_LN_O]
            vt_tile_enc(raw, h, tile_d, r.s_o, pc_gd, g_d)
            vt_pt(raw, VtProfRole.gemm_o)
            var pc_post = TowerLnArgs(d = uint(d), rows = uint(npos), boff = uint(o.o_b - norms_base), eps = eps,
                ln_on = 1u, lnoff = uint(o.ln2_w - norms_base), lnboff = uint(o.ln2_b - norms_base), ascale = 1.0)
            if (cm2) {
                enc_tower_post_add_ln16_cls(raw, h, r.s_post_o_ln16, pc_post, npos)
            } else {
                enc_tower_post_add_ln_cls(raw, h, r.s_post_o_ln, pc_post, npos)
                enc_tower_clamp_rq_cls(raw, h, r.s_rq_xb, pc_rq, wg_nd)
            }
            vt_pt(raw, VtProfRole.post_add)
            pc_gff.map_off = g_vt_maps[rb + VT_LN_FC1]
            vt_tile_enc(raw, h, tile_ff, r.s_hg, pc_gff, g_ff)
            vt_pt(raw, VtProfRole.gemm_fc1)
            pc_gelu.boff = uint(o.fc1_b - norms_base)
            if (cm2) {
                enc_tower_bias_act_cvt16_cls(raw, h, r.s_bias_hg16, pc_gelu, wg_nff)
                vt_pt(raw, VtProfRole.bias)
            } else {
                enc_tower_bias_act_cls(raw, h, r.s_bias_hg, pc_gelu, wg_nff)
                vt_pt(raw, VtProfRole.bias)
                enc_tower_clamp_rq_cls(raw, h, r.s_rq_hg, pc_rqh, wg_nff)
                vt_pt(raw, VtProfRole.rq)
            }
            pc_dn.map_off = g_vt_maps[rb + VT_LN_FC2]
            vt_tile_enc(raw, h, tile_fc2, s_fc2, pc_dn, g_fc2)
            vt_pt(raw, VtProfRole.gemm_fc2)
            if (split) {
                enc_splitk_reduce_cls(raw, h, r.s_splitk_reduce, pc_red, wg_red)
                vt_pt(raw, VtProfRole.sk_reduce)
            }
            let last = l + 1l == nl
            pc_post.boff = uint(o.fc2_b - norms_base)
            pc_post.ln_on = (last && !ln_post) ? 0u : 1u
            if (!last) {
                pc_post.lnoff = uint(offs[l + 1l].ln1_w - norms_base)
                pc_post.lnboff = uint(offs[l + 1l].ln1_b - norms_base)
            } elif (ln_post) {
                pc_post.lnoff = uint(lnp_w - norms_base)
                pc_post.lnboff = uint(lnp_b - norms_base)
            }
            if (cm2 && !(last && ln_post)) {
                enc_tower_post_add_ln16_cls(raw, h, r.s_post_d_ln16, pc_post, npos)
            } else {
                enc_tower_post_add_ln_cls(raw, h, r.s_post_d_ln, pc_post, npos)
            }
            vt_pt(raw, VtProfRole.post_add)
        }
        vhz_dep(raw, h, ln_post ? VT_XB : VT_X, 0u, true)
        cmd_copy_range(raw, ln_post ? r.xb_dev : r.x_dev, 0l, r.host.buf, 0l, bx)
        vt_pt(raw, VtProfRole.copy_out)
    }
    if (ln_post) {
        g_vt_aud_out_ptr = intptr(unsafe(addr(xb[0])))
        g_vt_aud_out_bytes = bx
        g_vt_aud_out_dev = r.xb_dev
        return vt_served(r, xb, bx, npos, nl)
    }
    return vt_served(r, x, bx, npos, nl)
}

var private g_vt_no_rows : array<float>   //! the chain's xb default where no post-norm lands
var private g_vt_aud_out_ptr = 0ul   //! the encoder-output handoff: the served post-norm chain's `xb` rows still on the device (`bytes` = 0: none) - every audio hook entry clears it, so it names the encode in flight alone
var private g_vt_aud_out_bytes = 0l
var private g_vt_aud_out_dev = 0ul

//! the device plane holding the encoder rows a caller is about to upload from `rows` (`bytes` long), or 0 when the tower did not
//! land those rows: the whisper cross-KV chain copies them device to device and skips the host upload
def vulkan_tower_enc_out(rows : float const?; bytes : int64) : uint64
    => (g_vt_aud_out_bytes == bytes && g_vt_aud_out_ptr == intptr(rows) && g_vt_aud.key != 0ul) ? g_vt_aud_out_dev : 0ul

//! the stem's pending rows landed in the encoder state's `x` (the CPU block loop reads them); nothing pending after it
def private vt_x_flush(var r : VtResident) {
    if (r.x_pend_bytes == 0l) {
        return
    }
    readback_range(r.x_dev, VT_X, r.host, r.x_pend_ptr, r.x_pend_bytes)
    vt_x_drop_pending(r)
}

//! the stem's rows no longer pending: taken on the device, landed, or gone with their plane
def private vt_x_drop_pending(var r : VtResident) {
    r.x_ready = false
    r.x_pend_bytes = 0l
    r.x_pend_ptr = null
}

//! a decline after the stem served: the stem's rows land in `s.x` first, since the CPU block loop reads them
def private vt_flush_and_decline(var r : VtResident; why : VulkanTowerDecline) : bool {
    vt_x_flush(r)
    return vt_decline(r, why)
}

[arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vt_aud_blocks(t : AudioTower; var s : EncoderState; npos : int64; ln_post : bool) : bool {
    var r & = g_vt_aud
    g_vt_aud_out_bytes = 0l
    if (!g_vt_env_tower) {
        return vt_flush_and_decline(r, VulkanTowerDecline.knob)
    }
    if (!g_vt_env_audio_blocks) {
        return vt_flush_and_decline(r, VulkanTowerDecline.blocks)
    }
    if (!t.q8) {
        return vt_flush_and_decline(r, VulkanTowerDecline.quant_mode)
    }
    let d = t.d_model
    let ff = t.n_ff
    let hs = d / t.n_head
    let hsp = 64l
    if (hs != 64l || d % 64l != 0l || ff % 64l != 0l || npos > VT_MAX_ENCODE_ROWS || npos <= 0l) {
        return vt_flush_and_decline(r, VulkanTowerDecline.shape)
    }
    let arm = vt_arm(r)
    if (!arm.ok) {
        vt_x_flush(r)
        return false
    }
    let khr = arm.khr
    if (!vt_range_ok(t.qblob.n) || !vt_range_ok(vt_scratch_max_bytes(vt_cap_rows(npos), d, ff, d, 0l, false, false))) {
        return vt_flush_and_decline(r, VulkanTowerDecline.shape)
    }
    let v = q8_batch_cls_variant(q8_batch_tile_for(d, npos), q8_batch_aligned(d, npos))
    let vff = q8_batch_cls_variant(q8_batch_tile_for(ff, npos), q8_batch_aligned(ff, npos))
    if (!vt_ln_chain_ensure(khr, hsp, v, vff)) {
        return vt_flush_and_decline(r, VulkanTowerDecline.device)
    }
    if (!vt_aud_attach(r, t)) {
        return vt_flush_and_decline(r, VulkanTowerDecline.memory)
    }
    let act = t.gelu_tanh ? BIAS_ACT_GELU_TANH : BIAS_ACT_GELU_ERF
    if (ln_post) {
        return vt_ln_chain(r, g_vt_ln_offs, t.has_k_bias, act, t.eps, t.layers_off, d, ff, hs, hsp, t.n_head, npos, khr, v, vff, s.x, t.lnp_w_off, t.lnp_b_off, s.xb)
    }
    return vt_ln_chain(r, g_vt_ln_offs, t.has_k_bias, act, t.eps, t.layers_off, d, ff, hs, hsp, t.n_head, npos, khr, v, vff, s.x)
}

[hot_path]
def private vulkan_audio_tower_blocks(t : AudioTower; var s : EncoderState; npos : int64) : bool => vt_aud_blocks(t, s, npos, false)

[hot_path]
def private vulkan_audio_tower_blocks_ln_post(t : AudioTower; var s : EncoderState; npos : int64) : bool => vt_aud_blocks(t, s, npos, true)

//! a whisper-class tower has a conv stem when its q8 layers start past the stem's conv2 plane (a conv-less core - qwen3a's - starts at 0)
def private vt_aud_has_conv(t : AudioTower) : bool => t.qlayers_off > 0l

[arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vt_aud_attach(var r : VtResident; t : AudioTower) : bool {
    g_vt_ln_offs |> reserve(t.n_layer)
    g_vt_ln_offs |> resize(t.n_layer)
    for (l in range64(t.n_layer)) {
        g_vt_ln_offs[l] = enc_layer_offsets(t, l)
    }
    let lay = active_q8_repack_layout(g_active_needs_repack)
    let lnp_rows = 2l * t.d_model
    let rows_n = t.n_layer * t.layer_stride + lnp_rows
    let key = vt_key(t.qblob, t.qscales, t.fblob, t.layers_off, rows_n, 0l, t.n_layer, lay.mr, lay.kgroup, lay.wbias)
    if (r.key == key) {
        return true
    }
    return vt_upload_ln(r, key, t.qblob, t.qscales, t.d_model, t.n_ff, t.fblob, t.layers_off, rows_n, lay, g_vt_ln_offs, vt_aud_has_conv(t) ? t.conv2_w_off : -1l)
}

let private VT_STEM_MEL = 0x1000000u   //! the whisper-class stem's hazard bits: the mel block, conv1's im2col and its rows
let private VT_STEM_COL = 0x2000000u
let private VT_STEM_Y1 = 0x4000000u
let private VTA_CONV2_KEY = 5000

//! the f16 image of a staged f32 slab, the f32 rows released
[cold_path]
def private vt_slab_f16(var slab : array<float>) : array<uint16> {
    var slab16 : array<uint16>
    slab16 |> reserve_resize(long_length(slab))
    unsafe {
        cvt_f32_to_f16_sat(addr(slab16[0]), addr(slab[0]), long_length(slab))
    }
    delete slab
    return <- slab16
}

[cold_path]
def private vt_aud_front_slab(var r : VtResident; t : AudioTower) : bool {
    let d = t.d_model
    let kreal = 3l * t.n_mel
    let kdp = (kreal + 63l) / 64l * 64l
    r.stem_kdp = kdp
    var slab : array<float>
    slab |> reserve_resize(d * kdp)
    tower_stage_f32_pad_row_stride(slab, t.fblob, t.conv1_w_off, 0l, d, kreal, kdp)
    var slab16 <- vt_slab_f16(slab)
    let rows = 2l * d + t.n_ctx * d
    var rows_f : array<float>
    rows_f |> reserve_resize(rows)
    unsafe {
        memcpy(addr<void?>(rows_f[0]), reinterpret<void?>(plane_at(t.fblob, t.conv1_b_off)), d * 4l)
        memcpy(addr<void?>(rows_f[d]), reinterpret<void?>(plane_at(t.fblob, t.conv2_b_off)), d * 4l)
        memcpy(addr<void?>(rows_f[2l * d]), reinterpret<void?>(plane_at(t.fblob, t.pos_off)), t.n_ctx * d * 4l)
    }
    r.fw16_bytes = d * kdp * 2l
    r.fw16_dev = vt_dev_buf(r, r.fw16_bytes)
    r.fnorm_bytes = rows * 4l
    r.fnorm_dev = vt_dev_buf(r, r.fnorm_bytes)
    r.front = true
    if (!r.oom) {
        upload_region_at(r.fw16_dev, 0l, unsafe(addr<uint8 const?>(slab16[0])), r.fw16_bytes)
        upload_region_at(r.fnorm_dev, 0l, unsafe(addr<uint8 const?>(rows_f[0])), r.fnorm_bytes)
    }
    delete slab16
    delete rows_f
    return !r.oom
}

//! the stem's sets bind the chain's x and f16 feed too: the chain's scratch rebuild releases this scratch with it
[cold_path]
def private vt_aud_front_scratch(var r : VtResident; tin, nm, d : int64; gemm_khr : bool) : bool {
    vt_front_release(r)
    r.oom = false
    r.front_cap = tin
    let bxin = nm * tin * 4l
    let bcol = tin * r.stem_kdp * 2l
    let by1 = tin * d * 4l
    r.fxin_dev = vt_dev_buf(r, bxin)
    r.fcol_dev = vt_dev_buf(r, bcol)
    r.fy1_dev = vt_dev_buf(r, by1)
    if (r.oom) {
        vt_front_release(r)
        return false
    }
    let bx = r.cap_rows * d * 4l
    let bxh = (r.cap_rows + TILE_READ_SLACK) * max(r.d, r.ff) * 2l
    r.s_fim[0] = set_tower_im2col1d_cls(fixed_array(r.fxin_dev, r.fcol_dev), fixed_array(bxin, bcol), fixed_array(VT_STEM_MEL, VT_STEM_COL))
    r.s_fg[0] = f16_gemm_set(gemm_khr, fixed_array(r.fw16_dev, r.fcol_dev, r.fy1_dev), fixed_array(r.fw16_bytes, bcol, by1), fixed_array(0u, VT_STEM_COL, VT_STEM_Y1))
    r.s_fb[0] = set_tower_bias_act_cls(fixed_array(r.fy1_dev, r.fnorm_dev), fixed_array(by1, r.fnorm_bytes), fixed_array(VT_STEM_Y1, 0u))
    r.s_fim[1] = set_tower_im2col1d_cls(fixed_array(r.fy1_dev, r.xh_dev), fixed_array(by1, bxh), fixed_array(VT_STEM_Y1, VT_XQ))
    r.s_fb[1] = set_tower_bias_act_cls(fixed_array(r.x_dev, r.fnorm_dev), fixed_array(bx, r.fnorm_bytes), fixed_array(VT_X, 0u))
    r.s_stem_pos_add = set_tower_bias_act_cls(fixed_array(r.x_dev, r.fnorm_dev), fixed_array(bx, r.fnorm_bytes), fixed_array(VT_X, 0u))
    return true
}

def private vt_ln_chain_ensure(khr : bool; hsp : int64; v, vff : int) : bool {
    return (fa_stamp_ensure(khr, hsp, false, false, false, false, true) && ensure_tower_ln_cls() && ensure_tower_clamp_rq_cls()
        && ensure_tower_bias_act_cls() && ensure_tower_hpad_cls() && ensure_tower_hpad16_cls() && ensure_tower_hunpad_cls()
        && ensure_tower_post_add_ln_cls() && q8_batch_cls_ensure(v) && q8_batch_cls_ensure(vff) && (!vt_cm2_feed() || vt_ln16_ensure()))
}

def private vt_ln16_ensure : bool {
    return (ensure_tower_ln16_cls() && ensure_tower_post_add_ln16_cls() && ensure_tower_hpad_b_cls() && ensure_tower_hpad16_b_cls()
        && ensure_tower_hunpad16_cls() && ensure_tower_bias_act_cvt16_cls())
}

[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vulkan_audio_conv_front(t : AudioTower; var s : EncoderState; mel : MelChunk) : bool {   // nolint:STYLE038 - the dispatch chain is the shape
    var r & = g_vt_aud
    g_vt_aud_out_bytes = 0l
    if (!g_vt_env_tower) {
        return vt_decline(r, VulkanTowerDecline.knob)
    }
    if (!t.q8) {
        return vt_decline(r, VulkanTowerDecline.quant_mode)
    }
    let tin = AUDIO_CHUNK_FRAMES
    let tt = t.n_ctx
    let d = t.d_model
    let ff = t.n_ff
    let nm = t.n_mel
    let hs = d / t.n_head
    let hsp = 64l
    if (hs != 64l || d % 64l != 0l || ff % 64l != 0l || 3l * d > max(d, ff) || tt > VT_MAX_ENCODE_ROWS || mel.n_len != tin || mel.n_mel != nm
            || !vt_aud_has_conv(t)) {
        return vt_decline(r, VulkanTowerDecline.shape)
    }
    let arm = vt_arm(r, true)
    if (!arm.ok) {
        return false
    }
    let khr = arm.khr
    if (!vt_cm2_feed()) {
        return vt_decline(r, VulkanTowerDecline.device)
    }
    if (!vt_range_ok(t.qblob.n) || !vt_range_ok(vt_scratch_max_bytes(vt_cap_rows(tt), d, ff, d, 0l, false, false))) {
        return vt_decline(r, VulkanTowerDecline.shape)
    }
    let v = q8_batch_cls_variant(q8_batch_tile_for(d, tt), q8_batch_aligned(d, tt))
    let vff = q8_batch_cls_variant(q8_batch_tile_for(ff, tt), q8_batch_aligned(ff, tt))
    let tc_d = cm2_tile_cols(d, tt)
    let tc_ff = cm2_tile_cols(ff, tt)
    if (!vt_ln_chain_ensure(khr, hsp, v, vff) || !f16_gemm_ensure(arm.gemm_khr) || !ensure_tower_im2col1d_cls() || !ensure_tower_bias_act_cls()
            || !cm2_cls_ensure(int(KqFmt.q8), tc_d) || !cm2_cls_ensure(int(KqFmt.q8), tc_ff)) {
        return vt_decline(r, VulkanTowerDecline.device)
    }
    if (!vt_aud_attach(r, t) || (!r.front && !vt_aud_front_slab(r, t))) {
        vt_release(r)
        return vt_decline(r, VulkanTowerDecline.memory)
    }
    let g_d = cm2_tiles_at(d, tt, tc_d)
    let g_ff = cm2_tiles_at(ff, tt, tc_ff)
    if (!vt_ln_scratch(r, t.n_layer, tt, d, ff, hs, d, g_d, g_ff, khr, true, tc_d, tc_ff, v, vff)) {
        return vt_decline(r, VulkanTowerDecline.memory)
    }
    if (r.front_cap != tin || r.gemm_khr != arm.gemm_khr) {
        r.gemm_khr = arm.gemm_khr
        if (!vt_aud_front_scratch(r, tin, nm, d, arm.gemm_khr)) {
            return vt_decline(r, VulkanTowerDecline.memory)
        }
    }
    let skey = VTA_CONV2_KEY + int(tc_d)
    if (!key_exists(r.sets_front, skey)) {
        let bxh = (r.cap_rows + TILE_READ_SLACK) * max(r.d, r.ff) * 2l
        r.sets_front[skey] = cm2_cls_set(int(KqFmt.q8), tc_d, fixed_array(r.wq_dev, r.ws_dev, r.meta.buf, r.xh_dev, r.x_dev),
            fixed_array(r.wq_bytes, r.ws_bytes, r.meta_bytes, bxh, r.cap_rows * d * 4l), fixed_array(0u, 0u, 0u, VT_XQ, VT_X))
    }
    var s_conv2 = r.sets_front[skey]
    var mp = unsafe(reinterpret<uint?>(r.meta.mapped))
    var cur = 4l
    let map_off = vt_sched_map(mp, cur, 0l, t.conv2_w_off / kq_elems(KqFmt.q8), vt_tile_rows(VtTile(v = 0, tc = tc_d), tt), g_d)
    upload_region_at(r.fxin_dev, 0l, unsafe(addr<uint8 const?>(mel.data[0])), nm * tin * 4l)
    let act = t.gelu_tanh ? BIAS_ACT_GELU_TANH : BIAS_ACT_GELU_ERF
    let bx = tt * d * 4l
    vt_chain_one_shot("whisper-class stem", 2l, tt) $(raw : VkCommandBuffer; var h : VkHaz) {
        var pc_im1 = TowerIm2col1dArgs(nch = uint(nm), srclen = uint(tin), stride = 1u, chmajor = 1u, kdp = uint(r.stem_kdp), rows = uint(tin))
        enc_tower_im2col1d_cls(raw, h, r.s_fim[0], pc_im1, (tin * r.stem_kdp + 255l) / 256l)
        vt_pt(raw, VtProfRole.im2col)
        var pc_g1 = F16GemmArgs(n = uint(r.stem_kdp), d = uint(d), rows = uint(tin), woff = 0u, ybase = 0u, ystride = uint(d), ksplit = 0u)
        f16_gemm_enc(arm.gemm_khr, raw, h, r.s_fg[0], pc_g1, tin, d)
        vt_pt(raw, VtProfRole.gemm_pw)
        vt_bias_act(raw, h, r.s_fb[0], d, tin * d, 0l, act)
        vt_pt(raw, VtProfRole.bias)
        var pc_im2 = TowerIm2col1dArgs(nch = uint(d), srclen = uint(tin), stride = 2u, chmajor = 0u, kdp = uint(3l * d), rows = uint(tt))
        enc_tower_im2col1d_cls(raw, h, r.s_fim[1], pc_im2, (tt * 3l * d + 255l) / 256l)
        vt_pt(raw, VtProfRole.im2col)
        var pc_c2 = BatchArgs(n = uint(3l * d), d = uint(d), map_off = map_off)
        cm2_cls_enc(int(KqFmt.q8), tc_d, raw, h, s_conv2, pc_c2, g_d)
        vt_pt(raw, VtProfRole.gemm_pw)
        vt_bias_act(raw, h, r.s_fb[1], d, tt * d, d, act)
        vt_pt(raw, VtProfRole.bias)
        vt_bias_act(raw, h, r.s_stem_pos_add, tt * d, tt * d, 2l * d, BIAS_ACT_NONE)
        vt_pt(raw, VtProfRole.post_add)
    }
    r.x_pend_ptr = unsafe(addr(s.x[0]))
    r.x_pend_bytes = bx
    r.x_ready = true
    g_vt_convs++
    vt_announce(r)
    return true
}

[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vulkan_gemma3v_blocks(t : Gemma3vTower; var s : Gemma3vState; npos : int64) : bool {
    var r & = g_vt_g3v
    if (!g_vt_env_tower) {
        return vt_decline(r, VulkanTowerDecline.knob)
    }
    if (!t.q8) {
        return vt_decline(r, VulkanTowerDecline.quant_mode)
    }
    let q8e = kq_elems(KqFmt.q8)
    let d = t.n_embd
    let ff = t.ff_pad
    let hs = d / t.n_head
    let hsp = hs <= 64l ? 64l : 128l
    let dp = t.n_head * hsp
    if (hs > 128l || d % 64l != 0l || ff % 64l != 0l || npos > VT_MAX_ENCODE_ROWS || npos <= 0l) {
        return vt_decline(r, VulkanTowerDecline.shape)
    }
    let arm = vt_arm(r)
    if (!arm.ok) {
        return false
    }
    let khr = arm.khr
    if (!vt_range_ok(t.qblob.n) || !vt_range_ok(vt_scratch_max_bytes(vt_cap_rows(npos), d, ff, dp, 0l, false, false))) {
        return vt_decline(r, VulkanTowerDecline.shape)
    }
    let v = q8_batch_cls_variant(q8_batch_tile_for(d, npos), q8_batch_aligned(d, npos))
    let vff = q8_batch_cls_variant(q8_batch_tile_for(ff, npos), q8_batch_aligned(ff, npos))
    if (!vt_ln_chain_ensure(khr, hsp, v, vff)) {
        return vt_decline(r, VulkanTowerDecline.device)
    }
    g_vt_ln_offs |> reserve(t.n_layer)
    g_vt_ln_offs |> resize(t.n_layer)
    for (l in range64(t.n_layer)) {
        g_vt_ln_offs[l] = g3v_ln_offs(t, l)
    }
    let lay = active_q8_repack_layout(g_active_needs_repack)
    let rows_n = t.n_layer * t.layer_f_stride
    let key = vt_key(t.qblob, t.qscales, t.blob, t.layer_f, rows_n, 0l, t.n_layer, lay.mr, lay.kgroup, lay.wbias)
    if (r.key != key && !vt_upload_ln(r, key, t.qblob, t.qscales, d, ff, t.blob, t.layer_f, rows_n, lay, g_vt_ln_offs)) {
        return vt_decline(r, VulkanTowerDecline.memory)
    }
    return vt_ln_chain(r, g_vt_ln_offs, true, BIAS_ACT_GELU_TANH, t.eps, t.layer_f, d, ff, hs, hsp, t.n_head, npos, khr, v, vff, s.x)
}

[arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vt_q3v_regions(t : Qwen3vTower; l : int64; var out : VtRegion[int(QWEN3V_GEMMS)]) {
    let d = t.n_embd
    let ff = t.ff_pad
    let o = qwen3v_block_offs(t, l)
    out[QWEN3V_GEMM_QKV] = VtRegion(woff = o.w[QWEN3V_GEMM_QKV], n = d, rows = 3l * d)
    out[QWEN3V_GEMM_O] = VtRegion(woff = o.w[QWEN3V_GEMM_O], n = d, rows = d)
    out[QWEN3V_GEMM_UP] = VtRegion(woff = o.w[QWEN3V_GEMM_UP], n = d, rows = ff)
    out[QWEN3V_GEMM_DOWN] = VtRegion(woff = o.w[QWEN3V_GEMM_DOWN], n = ff, rows = d)
}

[cold_path]
def private vt_upload_q3v(var r : VtResident; key : uint64; t : Qwen3vTower; lay : Q8RepackLayout) : bool {
    var regions : array<VtRegion>
    regions |> reserve(t.n_layer * QWEN3V_GEMMS)
    var recs : VtRegion[int(QWEN3V_GEMMS)]
    for (l in range64(t.n_layer)) {
        vt_q3v_regions(t, l, recs)
        regions |> push_from(recs)
    }
    let ok = vt_upload(r, key, t.qblob, t.qscales, regions, t.blob, t.layer_f, t.n_layer * t.layer_f_stride, 0l, lay)
    delete regions
    return ok
}

[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vulkan_qwen3v_blocks(t : Qwen3vTower; var s : Qwen3vState; npos : int64) : bool {   // nolint:STYLE038 — the dispatch chain is the shape
    var r & = g_vt_q3v
    if (!g_vt_env_tower) {
        return vt_decline(r, VulkanTowerDecline.knob)
    }
    if (!t.q8) {
        return vt_decline(r, VulkanTowerDecline.quant_mode)
    }
    let q8e = kq_elems(KqFmt.q8)
    let d = t.n_embd
    let ff = t.ff_pad
    let hs = d / t.n_head
    let hsp = hs <= 64l ? 64l : 128l
    let dp = t.n_head * hsp
    let nds = t.n_deepstack
    if (hs > 128l || hs % 2l != 0l || d % 64l != 0l || ff % 64l != 0l || npos > VT_MAX_ENCODE_ROWS || npos <= 0l) {
        return vt_decline(r, VulkanTowerDecline.shape)
    }
    let arm = vt_arm(r)
    if (!arm.ok) {
        return false
    }
    let khr = arm.khr
    if (!vt_range_ok(t.qblob.n) || !vt_range_ok(vt_scratch_max_bytes(vt_cap_rows(npos), d, ff, dp, nds, true, false))) {
        return vt_decline(r, VulkanTowerDecline.shape)
    }
    let fa_ok = fa_stamp_ensure(khr, hsp, false, false, false, false, true)
    if (!fa_ok || !ensure_tower_ln_cls() || !ensure_tower_clamp_rq_cls() || !ensure_tower_bias_act_cls() || !ensure_tower_rope_tab_cls()
            || !ensure_tower_hpad_cls() || !ensure_tower_hpad16_cls() || !ensure_tower_hunpad_cls() || !ensure_tower_post_add_ln_cls()) {
        return vt_decline(r, VulkanTowerDecline.device)
    }
    let v = q8_batch_cls_variant(q8_batch_tile_for(d, npos), q8_batch_aligned(d, npos))
    let vff = q8_batch_cls_variant(q8_batch_tile_for(ff, npos), q8_batch_aligned(ff, npos))
    let vqkv = q8_batch_cls_variant(q8_batch_tile_for(3l * d, npos), q8_batch_aligned(3l * d, npos))
    if (!q8_batch_cls_ensure(v) || !q8_batch_cls_ensure(vff) || !q8_batch_cls_ensure(vqkv)) {
        return vt_decline(r, VulkanTowerDecline.device)
    }
    let lay = active_q8_repack_layout(g_active_needs_repack)
    let key = vt_key(t.qblob, t.qscales, t.blob, t.layer_f, t.n_layer * t.layer_f_stride, 0l, t.n_layer, lay.mr, lay.kgroup, lay.wbias)
    if (r.key != key && !vt_upload_q3v(r, key, t, lay)) {
        return vt_decline(r, VulkanTowerDecline.memory)
    }
    let g_d = vt_gemm_groups(d, npos)
    let g_ff = vt_gemm_groups(ff, npos)
    let g_qkv = vt_gemm_groups(3l * d, npos)
    let nrec = t.n_layer * QWEN3V_GEMMS
    let meta_bytes = (nrec * 4l + t.n_layer * (2l * g_d + g_ff + g_qkv)) * 4l
    if (!vt_scratch_fits(r, npos, d, ff, dp, meta_bytes, khr, true, nds)) {
        if (!vt_scratch_build(r, npos, d, ff, hs, dp, meta_bytes, khr, true, nds)) {
            return vt_decline(r, VulkanTowerDecline.memory)
        }
        vt_sets_q3v(r)
    }
    vt_bind_tile_sets(r, VtTile(v = v, tc = 0l), VtTile(v = vff, tc = 0l), VtTile(v = vqkv, tc = 0l))
    g_vt_maps |> reserve(nrec)
    g_vt_maps |> resize(nrec)
    var mp = unsafe(reinterpret<uint?>(r.meta.mapped))
    var cur = nrec * 4l
    var recs : VtRegion[int(QWEN3V_GEMMS)]
    let rec_groups = fixed_array(g_qkv, g_d, g_ff, g_d)
    for (l in range64(t.n_layer)) {
        vt_q3v_regions(t, l, recs)
        for (g in range64(QWEN3V_GEMMS)) {
            let rid = l * QWEN3V_GEMMS + g
            g_vt_maps[rid] = vt_sched_map(mp, cur, rid, recs[g].woff / q8e, npos, rec_groups[g])
        }
    }
    let bx = npos * d * 4l
    let half = hs / 2l
    let btab = npos * half * 4l
    unsafe {
        upload_region_at(r.x_dev, 0l, addr<uint8 const?>(s.x[0]), bx)
        upload_region_at(r.cos_dev, 0l, addr<uint8 const?>(s.cos_t[0]), btab)
        upload_region_at(r.sin_dev, 0l, addr<uint8 const?>(s.sin_t[0]), btab)
    }
    let nd = npos * d
    let nff = npos * ff
    let npos_pad = vt_cap_rows(npos)
    let wg_nd = rq_wgs(false, nd)
    let wg_n3d = rq_wgs(false, 3l * nd)
    let wg_nff = rq_wgs(false, nff)
    let wg_pad = (npos_pad * dp + 255l) / 256l
    let wg_unpad = (npos * dp + 255l) / 256l
    let wg_rope = (npos * t.n_head * half + 255l) / 256l
    let wg_fa = fa_stamp_wgs(khr, t.n_head, npos)
    let norms_base = t.layer_f
    var pc_fa = FaArgs(rows = uint(npos), w0 = 0u, qd = uint(dp), kvd = uint(dp), kv_mul = 1u, kvbase = 0u, scale = 1.0 / sqrt(float(hs)), window = 0u, sinkoff = 0u)
    var pc_pad_qkv = TowerRestrideArgs(rows = uint(npos), rows_pad = uint(npos_pad), nheads = uint(t.n_head), hs = uint(hs), hsp = uint(hsp), sstride = uint(3l * d), soff = 0u)
    var pc_unpad = TowerRestrideArgs(rows = uint(npos), rows_pad = uint(npos_pad), nheads = uint(t.n_head), hs = uint(hs), hsp = uint(hsp), sstride = uint(d), soff = 0u)   //! the attention rows are d-wide
    var pc_rope = TowerRopeTabArgs(sstride = uint(3l * d), soff = 0u, nheads = uint(t.n_head), hs = uint(hs), rows = uint(npos))
    var pc_rq = TowerClampRqArgs(inbase = 0u, nblk = uint(nd / q8e), lo = -VT_NO_CLAMP, hi = VT_NO_CLAMP)
    var pc_rqh = TowerClampRqArgs(inbase = 0u, nblk = uint(nff / q8e), lo = -VT_NO_CLAMP, hi = VT_NO_CLAMP)
    var pc_gqkv = BatchArgs(n = uint(d), d = uint(3l * d), map_off = 0u)
    var pc_gd = BatchArgs(n = uint(d), d = uint(d), map_off = 0u)
    var pc_gff = BatchArgs(n = uint(d), d = uint(ff), map_off = 0u)
    var pc_dn = BatchArgs(n = uint(ff), d = uint(d), map_off = 0u)
    var pc_bqkv = TowerBiasArgs(d = uint(3l * d), nelem = uint(3l * nd), boff = 0u, act = BIAS_ACT_NONE)
    var pc_gelu = TowerBiasArgs(d = uint(ff), nelem = uint(nff), boff = 0u, act = BIAS_ACT_GELU_TANH)
    cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
        for (l in range64(t.n_layer)) {
            let o = qwen3v_block_offs(t, l)
            let rb = l * QWEN3V_GEMMS
            if (l == 0l) {
                var pc = TowerLnArgs(d = uint(d), rows = uint(npos), woff = uint(o.ln1_w - norms_base), boff = uint(o.ln1_b - norms_base), eps = t.eps)
                enc_tower_ln_cls(raw, h, r.s_ln, pc, npos)
            }
            enc_tower_clamp_rq_cls(raw, h, r.s_rq_xb, pc_rq, wg_nd)
            pc_gqkv.map_off = g_vt_maps[rb + QWEN3V_GEMM_QKV]
            q8_batch_cls_enc(vqkv, raw, h, r.s_qkv, pc_gqkv, g_qkv)
            pc_bqkv.boff = uint(o.qkv_b - norms_base)
            enc_tower_bias_act_cls(raw, h, r.s_bias_qkv, pc_bqkv, wg_n3d)
            pc_rope.soff = 0u
            enc_tower_rope_tab_cls(raw, h, r.s_rope_tq, pc_rope, wg_rope)
            pc_rope.soff = uint(d)
            enc_tower_rope_tab_cls(raw, h, r.s_rope_tk, pc_rope, wg_rope)
            pc_pad_qkv.soff = 0u
            enc_tower_hpad_cls(raw, h, r.s_pad_q, pc_pad_qkv, wg_pad)
            pc_pad_qkv.soff = uint(d)
            enc_tower_hpad16_cls(raw, h, r.s_pad_k, pc_pad_qkv, wg_pad)
            pc_pad_qkv.soff = uint(2l * d)
            enc_tower_hpad16_cls(raw, h, r.s_pad_v, pc_pad_qkv, wg_pad)
            fa_stamp_enc(khr, hsp, false, false, false, false, raw, h, r.s_fa, pc_fa, wg_fa, true)
            enc_tower_hunpad_cls(raw, h, r.s_unpad, pc_unpad, wg_unpad)
            enc_tower_clamp_rq_cls(raw, h, r.s_rq_att, pc_rq, wg_nd)
            pc_gd.map_off = g_vt_maps[rb + QWEN3V_GEMM_O]
            q8_batch_cls_enc(v, raw, h, r.s_o, pc_gd, g_d)
            var pc_post = TowerLnArgs(d = uint(d), rows = uint(npos), boff = uint(o.out_b - norms_base), eps = t.eps,
                ln_on = 1u, lnoff = uint(o.ln2_w - norms_base), lnboff = uint(o.ln2_b - norms_base), ascale = 1.0)
            enc_tower_post_add_ln_cls(raw, h, r.s_post_o_ln, pc_post, npos)
            enc_tower_clamp_rq_cls(raw, h, r.s_rq_xb, pc_rq, wg_nd)
            pc_gff.map_off = g_vt_maps[rb + QWEN3V_GEMM_UP]
            q8_batch_cls_enc(vff, raw, h, r.s_hg, pc_gff, g_ff)   //! the up rows ride the gate slot's buffer (hg)
            pc_gelu.boff = uint(o.up_b - norms_base)
            enc_tower_bias_act_cls(raw, h, r.s_bias_hg, pc_gelu, wg_nff)
            enc_tower_clamp_rq_cls(raw, h, r.s_rq_hg, pc_rqh, wg_nff)
            pc_dn.map_off = g_vt_maps[rb + QWEN3V_GEMM_DOWN]
            q8_batch_cls_enc(v, raw, h, r.s_down, pc_dn, g_d)
            let last = l + 1l == t.n_layer
            pc_post.boff = uint(o.down_b - norms_base)
            pc_post.ln_on = last ? 0u : 1u
            if (!last) {
                let on = qwen3v_block_offs(t, l + 1l)
                pc_post.lnoff = uint(on.ln1_w - norms_base)
                pc_post.lnboff = uint(on.ln1_b - norms_base)
            }
            enc_tower_post_add_ln_cls(raw, h, r.s_post_d_ln, pc_post, npos)
            let k = qwen3v_tap_of(t, l)
            if (k >= 0l && k < nds) {
                vhz_dep(raw, h, VT_X, VT_TAP, true)
                cmd_copy_range(raw, r.x_dev, 0l, r.xtap_dev, k * bx, bx)
            }
        }
        vhz_dep(raw, h, VT_X | VT_TAP, 0u, true)
        cmd_copy_range(raw, r.x_dev, 0l, r.host.buf, 0l, bx)
        if (nds > 0l) {
            cmd_copy_range(raw, r.xtap_dev, 0l, r.host.buf, bx, nds * bx)
        }
    }
    if (nds > 0l) {
        unsafe {
            memcpy(addr<void?>(s.ds_xtaps[0]), reinterpret<void?>(intptr(r.host.mapped) + uint64(bx)), nds * bx)
        }
    }
    return vt_served(r, s.x, bx, npos, t.n_layer)
}

[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vulkan_qwen25v_blocks(t : Qwen25vTower; var s : Qwen25vState; npos : int64) : bool {   // nolint:STYLE038 — the dispatch chain is the shape, its two attention arms the routing
    var r & = g_vt_q25v
    if (!g_vt_env_tower) {
        return vt_decline(r, VulkanTowerDecline.knob)
    }
    if (t.wblob.n <= 0l || t.blk_bf16) {
        return vt_decline(r, VulkanTowerDecline.quant_mode)
    }
    let d = t.n_embd
    let ff = t.ff_pad
    let hs = d / t.n_head
    let hsp = hs <= 64l ? 64l : 128l
    let dp = t.n_head * hsp
    let nwin = long_length(s.win_lo) - 1l
    let has_win = t.n_wa_pattern > 1l && t.n_layer > 0l   //! a window layer exists: block 0 attends per window whenever the pattern spares a block
    let wmax = has_win ? qwen25v_window_widest(s) : 0l
    if (hs > 128l || hsp != 128l || hs % 2l != 0l || d % 64l != 0l || ff % 64l != 0l || npos > VT_MAX_ENCODE_ROWS || npos <= 0l || nwin <= 0l
            || (has_win && (wmax > 64l || hs != VT_WIN_HS))) {
        return vt_decline(r, VulkanTowerDecline.shape)  //! the chain's sets are built at the 128 head alone (the family's 80-wide heads)
    }
    let arm = vt_arm(r, true)
    if (!arm.ok) {
        return false
    }
    let khr = arm.khr
    let gemm_khr = arm.gemm_khr
    let w_off = t.layer_w
    let w_n = t.n_layer * t.layer_w_stride
    if (!vt_range_ok(w_n * 2l) || !vt_range_ok(vt_scratch_max_bytes(vt_cap_rows(npos), d, ff, dp, 0l, false, true))) {
        return vt_decline(r, VulkanTowerDecline.shape)
    }
    let fa_ok = fa_stamp_ensure(khr, hsp, false, false, false, false, true)
    if (!fa_ok || !f16_gemm_ensure(gemm_khr) || !ensure_tower_clamp_cvt_cls() || !ensure_tower_rms_cls() || !ensure_tower_bias_act_cls()
            || !ensure_tower_rope_tab_cls() || !ensure_tower_hpad_cls() || !ensure_tower_hpad16_cls() || !ensure_tower_hunpad_cls()
            || !ensure_tower_win_attn_cls() || !ensure_tower_post_add_rms_cls() || !ensure_actf16_b_cls()) {
        return vt_decline(r, VulkanTowerDecline.device)
    }
    let key = vt_key_f16(t.wblob, w_off, w_n, t.blob, t.layer_f, t.n_layer * t.layer_f_stride, t.n_layer)
    if (r.key != key && !vt_upload_f16(r, key, t.wblob, w_off, w_n, t.blob, t.layer_f, t.n_layer * t.layer_f_stride)) {
        return vt_decline(r, VulkanTowerDecline.memory)
    }
    if (!vt_scratch_f16_fits(r, npos, nwin, d, ff, dp, khr, gemm_khr)) {
        if (!vt_scratch_f16_build(r, npos, nwin, d, ff, hs, dp, khr, gemm_khr)) {
            return vt_decline(r, VulkanTowerDecline.memory)
        }
        vt_sets_q25v(r)
    }
    g_vt_wlo |> reserve(nwin + 1l)
    g_vt_wlo |> resize(nwin + 1l)
    qwen25v_window_table(s, unsafe(addr(g_vt_wlo[0])))
    let bx = npos * d * 4l
    let half = hs / 2l
    let btab = npos * half * 4l
    unsafe {
        upload_region_at(r.x_dev, 0l, addr<uint8 const?>(s.x[0]), bx)
        upload_region_at(r.cos_dev, 0l, addr<uint8 const?>(s.cos_t[0]), btab)
        upload_region_at(r.sin_dev, 0l, addr<uint8 const?>(s.sin_t[0]), btab)
        upload_region_at(r.wlo_dev, 0l, addr<uint8 const?>(g_vt_wlo[0]), (nwin + 1l) * 4l)
    }
    let nd = npos * d
    let nff = npos * ff
    let npos_pad = vt_cap_rows(npos)
    let wg_nd = rq_wgs(false, nd)
    let wg_nff = rq_wgs(false, nff)
    let wg_pad = (npos_pad * dp + 255l) / 256l
    let wg_unpad = (npos * dp + 255l) / 256l
    let wg_rope = (npos * t.n_head * half + 255l) / 256l
    let wg_fa = fa_stamp_wgs(khr, t.n_head, npos)
    let norms_base = t.layer_f
    let scale = 1.0 / sqrt(float(hs))
    var pc_fa = FaArgs(rows = uint(npos), w0 = 0u, qd = uint(dp), kvd = uint(dp), kv_mul = 1u, kvbase = 0u, scale = scale, window = 0u, sinkoff = 0u)
    var pc_rs = TowerRestrideArgs(rows = uint(npos), rows_pad = uint(npos_pad), nheads = uint(t.n_head), hs = uint(hs), hsp = uint(hsp), sstride = uint(d), soff = 0u)
    var pc_rope = TowerRopeTabArgs(sstride = uint(d), soff = 0u, nheads = uint(t.n_head), hs = uint(hs), rows = uint(npos))
    var pc_gd = F16GemmArgs(n = uint(d), d = uint(d), rows = uint(npos), woff = 0u, ybase = 0u, ystride = uint(d), ksplit = 0u)
    var pc_gff = F16GemmArgs(n = uint(d), d = uint(ff), rows = uint(npos), woff = 0u, ybase = 0u, ystride = uint(ff), ksplit = 0u)
    var pc_dn = F16GemmArgs(n = uint(ff), d = uint(d), rows = uint(npos), woff = 0u, ybase = 0u, ystride = uint(d), ksplit = 0u)
    var pc_bias = TowerBiasArgs(d = uint(d), nelem = uint(nd), boff = 0u, act = BIAS_ACT_NONE)
    var pc_glu = ActArgs(nelem = uint(nff), act = ACT_SILU, brow = uint(ff))
    var pc_win = TowerWinArgs(nheads = uint(t.n_head), scale = scale)
    cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
        for (l in range64(t.n_layer)) {
            let o = qwen25v_block_offs(t, l)
            let full = qwen25v_layer_full(t, l)
            if (l == 0l) {
                var pc = TowerRowArgs(d = uint(d), rows = uint(npos), woff = uint(o.ln1_w - norms_base), eps = t.eps)
                enc_tower_rms_cls(raw, h, r.s_rms, pc, npos)
            }
            vt_cvt16(raw, h, r.s_cvt_xb, nd)
            pc_gd.woff = uint(o.w[QWEN25V_GEMM_Q] - w_off)
            f16_gemm_enc(gemm_khr, raw, h, r.s_q, pc_gd, npos, d)
            pc_bias.boff = uint(o.b[QWEN25V_GEMM_Q] - norms_base)
            enc_tower_bias_act_cls(raw, h, r.s_bias_q, pc_bias, wg_nd)
            pc_gd.woff = uint(o.w[QWEN25V_GEMM_K] - w_off)
            f16_gemm_enc(gemm_khr, raw, h, r.s_k, pc_gd, npos, d)
            pc_bias.boff = uint(o.b[QWEN25V_GEMM_K] - norms_base)
            enc_tower_bias_act_cls(raw, h, r.s_bias_k, pc_bias, wg_nd)
            pc_gd.woff = uint(o.w[QWEN25V_GEMM_V] - w_off)
            f16_gemm_enc(gemm_khr, raw, h, r.s_v, pc_gd, npos, d)
            pc_bias.boff = uint(o.b[QWEN25V_GEMM_V] - norms_base)
            enc_tower_bias_act_cls(raw, h, r.s_bias_v, pc_bias, wg_nd)
            enc_tower_rope_tab_cls(raw, h, r.s_rope_tq, pc_rope, wg_rope)
            enc_tower_rope_tab_cls(raw, h, r.s_rope_tk, pc_rope, wg_rope)
            if (full) {
                enc_tower_hpad_cls(raw, h, r.s_pad_q, pc_rs, wg_pad)
                enc_tower_hpad16_cls(raw, h, r.s_pad_k, pc_rs, wg_pad)
                enc_tower_hpad16_cls(raw, h, r.s_pad_v, pc_rs, wg_pad)
                fa_stamp_enc(khr, hsp, false, false, false, false, raw, h, r.s_fa, pc_fa, wg_fa, true)
                enc_tower_hunpad_cls(raw, h, r.s_unpad, pc_rs, wg_unpad)
            } else {
                enc_tower_win_attn_cls(raw, h, r.s_win, pc_win, nwin * t.n_head)
            }
            vt_cvt16(raw, h, r.s_cvt_att, nd)
            pc_gd.woff = uint(o.w[QWEN25V_GEMM_O] - w_off)
            f16_gemm_enc(gemm_khr, raw, h, r.s_o, pc_gd, npos, d)
            var pc_post = TowerLnArgs(d = uint(d), rows = uint(npos), boff = uint(o.b[QWEN25V_GEMM_O] - norms_base), eps = t.eps, ln_on = 1u, lnoff = uint(o.ln2_w - norms_base), lnboff = 0u,
                ascale = 1.0)
            enc_tower_post_add_rms_cls(raw, h, r.s_post_o_ln, pc_post, npos)
            vt_cvt16(raw, h, r.s_cvt_xb, nd)
            pc_gff.woff = uint(o.w[QWEN25V_GEMM_GATE] - w_off)
            f16_gemm_enc(gemm_khr, raw, h, r.s_hg, pc_gff, npos, ff)
            pc_gff.woff = uint(o.w[QWEN25V_GEMM_UP] - w_off)
            f16_gemm_enc(gemm_khr, raw, h, r.s_hu, pc_gff, npos, ff)
            pc_glu.gboff = uint(o.b[QWEN25V_GEMM_GATE] - norms_base)
            pc_glu.uboff = uint(o.b[QWEN25V_GEMM_UP] - norms_base)
            enc_actf16_b_cls(raw, h, r.s_glu, pc_glu, wg_nff)
            pc_dn.woff = uint(o.w[QWEN25V_GEMM_DOWN] - w_off)
            f16_gemm_enc(gemm_khr, raw, h, r.s_down, pc_dn, npos, d)
            let last = l + 1l == t.n_layer
            pc_post.boff = uint(o.b[QWEN25V_GEMM_DOWN] - norms_base)
            pc_post.ln_on = last ? 0u : 1u
            if (!last) {
                pc_post.lnoff = uint(qwen25v_block_offs(t, l + 1l).ln1_w - norms_base)
            }
            enc_tower_post_add_rms_cls(raw, h, r.s_post_d_ln, pc_post, npos)
        }
        vhz_dep(raw, h, VT_X, 0u, true)
        cmd_copy_range(raw, r.x_dev, 0l, r.host.buf, 0l, bx)
    }
    return vt_served(r, s.x, bx, npos, t.n_layer)
}

[cold_path]
def private vt_upload_g4v(var r : VtResident; key : uint64; t : Gemma4vTower; hs : int64; lay : Q8RepackLayout) : bool {
    let d = t.n_embd
    let ff = t.n_ff
    var regions : array<VtRegion>
    regions |> reserve(t.n_layer * GEMMA4V_GEMMS)
    for (l in range64(t.n_layer)) {
        let o = gemma4v_block_offs(t, l)
        for (g in range64(GEMMA4V_GEMMS)) {
            let n = g == GEMMA4V_GEMM_DOWN ? ff : d
            let rows = g == GEMMA4V_GEMM_GATE || g == GEMMA4V_GEMM_UP ? ff : d
            regions |> push(VtRegion(woff = o.w[g], n = n, rows = rows))
        }
    }
    let ok = vt_upload(r, key, t.qblob, t.qscales, regions, t.blob, t.layer_f, t.n_layer * t.layer_f_stride, hs, lay)
    delete regions
    return ok
}

[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vulkan_gemma4v_blocks(t : Gemma4vTower; var s : Gemma4vState; npos : int64) : bool {   // nolint:STYLE038 — the dispatch chain is the shape
    var r & = g_vt_g4v
    if (!g_vt_env_tower) {
        return vt_decline(r, VulkanTowerDecline.knob)
    }
    if (!t.q8) {
        return vt_decline(r, VulkanTowerDecline.quant_mode)
    }
    let q8e = kq_elems(KqFmt.q8)
    let d = t.n_embd
    let ff = t.n_ff
    let hs = d / t.n_head
    if (hs != 64l || d % 64l != 0l || ff % 64l != 0l || npos > VT_MAX_ENCODE_ROWS || npos <= 0l) {
        return vt_decline(r, VulkanTowerDecline.shape)
    }
    let arm = vt_arm(r)
    if (!arm.ok) {
        return false
    }
    let khr = arm.khr
    if (!vt_range_ok(t.qblob.n) || !vt_range_ok(vt_scratch_max_bytes(vt_cap_rows(npos), d, ff, 0l, 0l, false, false))) {
        return vt_decline(r, VulkanTowerDecline.shape)
    }
    let fa_ok = fa_stamp_ensure(khr, hs, false, false, false, false, true)
    if (!fa_ok || !ensure_tower_rms_cls() || !ensure_tower_clamp_cls() || !ensure_tower_clamp_rq_cls() || !ensure_tower_rope2d_cls()
            || !ensure_tower_post_add_cls() || !ensure_tower_geglu_cls() || !ensure_tower_clamp_cvt_cls()) {
        return vt_decline(r, VulkanTowerDecline.device)
    }
    let v = q8_batch_cls_variant(q8_batch_tile_for(d, npos), q8_batch_aligned(d, npos))
    let vff = q8_batch_cls_variant(q8_batch_tile_for(ff, npos), q8_batch_aligned(ff, npos))
    if (!q8_batch_cls_ensure(v) || !q8_batch_cls_ensure(vff)) {
        return vt_decline(r, VulkanTowerDecline.device)
    }
    let lay = active_q8_repack_layout(g_active_needs_repack)
    let key = vt_key(t.qblob, t.qscales, t.blob, t.layer_f, t.n_layer * t.layer_f_stride, hs, t.n_layer, lay.mr, lay.kgroup, lay.wbias)
    if (r.key != key && !vt_upload_g4v(r, key, t, hs, lay)) {
        return vt_decline(r, VulkanTowerDecline.memory)
    }
    let g_d = vt_gemm_groups(d, npos)
    let g_ff = vt_gemm_groups(ff, npos)
    let nrec = t.n_layer * GEMMA4V_GEMMS
    let meta_bytes = (nrec * 4l + t.n_layer * (5l * g_d + 2l * g_ff)) * 4l
    if (!vt_scratch_fits(r, npos, d, ff, 0l, meta_bytes, khr, false, 0l)) {
        if (!vt_scratch_build(r, npos, d, ff, hs, 0l, meta_bytes, khr, false, 0l)) {
            return vt_decline(r, VulkanTowerDecline.memory)
        }
        vt_sets_g4v(r)
    }
    vt_bind_tile_sets(r, VtTile(v = v, tc = 0l), VtTile(v = vff, tc = 0l))
    g_vt_maps |> reserve(nrec)
    g_vt_maps |> resize(nrec)
    unsafe {
        var mp = reinterpret<uint?>(r.meta.mapped)
        var cur = nrec * 4l
        for (l in range64(t.n_layer)) {
            let o = gemma4v_block_offs(t, l)
            for (g in range64(GEMMA4V_GEMMS)) {
                let rid = l * GEMMA4V_GEMMS + g
                g_vt_maps[rid] = vt_sched_map(mp, cur, rid, o.w[g] / q8e, npos, g == GEMMA4V_GEMM_GATE || g == GEMMA4V_GEMM_UP ? g_ff : g_d)
            }
        }
    }
    let pairs = hs / 4l
    g_vt_rope2d_tabs |> reserve(npos * 4l * pairs)
    g_vt_rope2d_tabs |> resize(npos * 4l * pairs)
    gemma4v_pack_rope_tabs(s, npos, pairs, unsafe(addr(g_vt_rope2d_tabs[0])))
    let bx = npos * d * 4l
    unsafe {
        upload_region_at(r.tabs_dev, 0l, addr<uint8 const?>(g_vt_rope2d_tabs[0]), npos * 4l * pairs * 4l)
        upload_region_at(r.x_dev, 0l, addr<uint8 const?>(s.x[0]), bx)
    }
    let nd = npos * d
    let nff = npos * ff
    let wg_nd = rq_wgs(false, nd)
    let wg_nff = rq_wgs(false, nff)
    let wg_rope = (npos * t.n_head * pairs + 255l) / 256l
    let wg_fa = fa_stamp_wgs(khr, t.n_head, npos)
    let norms_base = t.layer_f
    var pc_fa = FaArgs(rows = uint(npos), w0 = 0u, qd = uint(d), kvd = uint(d), kv_mul = 1u, kvbase = 0u, scale = 1.0, window = 0u, sinkoff = 0u)
    var pc_rope = TowerRopeArgs(nheads = uint(t.n_head), hs = uint(hs), rows = uint(npos))
    var pc_gff = BatchArgs(n = uint(d), d = uint(ff), map_off = 0u)
    cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
        for (l in range64(t.n_layer)) {
            let o = gemma4v_block_offs(t, l)
            let cq = gemma4v_clamp_at(t, o.clamp[GEMMA4V_GEMM_Q])
            let ck = gemma4v_clamp_at(t, o.clamp[GEMMA4V_GEMM_K])
            let cv = gemma4v_clamp_at(t, o.clamp[GEMMA4V_GEMM_V])
            let co = gemma4v_clamp_at(t, o.clamp[GEMMA4V_GEMM_O])
            let cg = gemma4v_clamp_at(t, o.clamp[GEMMA4V_GEMM_GATE])
            let cu = gemma4v_clamp_at(t, o.clamp[GEMMA4V_GEMM_UP])
            let cd = gemma4v_clamp_at(t, o.clamp[GEMMA4V_GEMM_DOWN])
            let rb = l * GEMMA4V_GEMMS
            if (l == 0l) {
                var pc = TowerRowArgs(d = uint(d), rows = uint(npos), woff = uint(o.ln1 - norms_base), eps = t.eps)
                enc_tower_rms_cls(raw, h, r.s_rms, pc, npos)
            }
            let tile = VtTile(v = v, tc = 0l)
            vt_mm_clamped(raw, h, cq, r.s_rq_xb, nd, r.s_q, tile, d, d, g_vt_maps[rb + GEMMA4V_GEMM_Q], g_d, r.s_clamp_q, nd)
            vt_mm_clamped(raw, h, ck, r.s_rq_xb, nd, r.s_k, tile, d, d, g_vt_maps[rb + GEMMA4V_GEMM_K], g_d, r.s_clamp_k, nd)
            vt_mm_clamped(raw, h, cv, r.s_rq_xb, nd, r.s_v, tile, d, d, g_vt_maps[rb + GEMMA4V_GEMM_V], g_d, r.s_clamp_v, nd)
            var pc_h = TowerRowArgs(d = uint(hs), rows = uint(npos * t.n_head), woff = uint(o.q_norm - norms_base), eps = t.eps)
            enc_tower_rms_cls(raw, h, r.s_hrms_q, pc_h, npos * t.n_head)
            pc_h.woff = uint(o.k_norm - norms_base)
            enc_tower_rms_cls(raw, h, r.s_hrms_k, pc_h, npos * t.n_head)
            pc_h.woff = uint(r.ones_off)
            enc_tower_rms_cls(raw, h, r.s_hrms_v, pc_h, npos * t.n_head)
            enc_tower_rope2d_cls(raw, h, r.s_rope_q, pc_rope, wg_rope)
            enc_tower_rope2d_cls(raw, h, r.s_rope_k, pc_rope, wg_rope)
            vt_cvt16(raw, h, r.s_cvt_k, nd)
            vt_cvt16(raw, h, r.s_cvt_v, nd)
            fa_stamp_enc(khr, hs, false, false, false, false, raw, h, r.s_fa, pc_fa, wg_fa, true)
            vt_mm_clamped(raw, h, co, r.s_rq_att, nd, r.s_o, tile, d, d, g_vt_maps[rb + GEMMA4V_GEMM_O], g_d, r.s_clamp_yo, nd)
            var pc_post = TowerLnArgs(d = uint(d), rows = uint(npos), woff = uint(o.attn_post - norms_base), boff = 0u, eps = t.eps, ln_on = 1u,
                lnoff = uint(o.ln2 - norms_base), lnboff = 0u, ascale = 1.0)
            enc_tower_post_add_cls(raw, h, r.s_post_o, pc_post, npos)
            var pc_rq = TowerClampRqArgs(inbase = 0u, nblk = uint(nd / q8e), lo = vt_lo(cg, true), hi = vt_hi(cg, true))
            enc_tower_clamp_rq_cls(raw, h, r.s_rq_xb, pc_rq, wg_nd)
            pc_gff.map_off = g_vt_maps[rb + GEMMA4V_GEMM_GATE]
            q8_batch_cls_enc(vff, raw, h, r.s_hg, pc_gff, g_ff)
            pc_rq.lo = vt_lo(cu, true)
            pc_rq.hi = vt_hi(cu, true)
            enc_tower_clamp_rq_cls(raw, h, r.s_rq_xb, pc_rq, wg_nd)
            pc_gff.map_off = g_vt_maps[rb + GEMMA4V_GEMM_UP]
            q8_batch_cls_enc(vff, raw, h, r.s_hu, pc_gff, g_ff)
            var pc_gg = TowerGegluArgs(nelem = uint(nff), glo = vt_lo(cg, false), ghi = vt_hi(cg, false), ulo = vt_lo(cu, false), uhi = vt_hi(cu, false),
                dlo = vt_lo(cd, true), dhi = vt_hi(cd, true))
            enc_tower_geglu_cls(raw, h, r.s_geglu, pc_gg, wg_nff)
            vt_mm_clamped(raw, h, cd, r.s_rq_hg, nff, r.s_down, tile, ff, d, g_vt_maps[rb + GEMMA4V_GEMM_DOWN], g_d, r.s_clamp_yd, nd)
            let last = l + 1l == t.n_layer
            pc_post.woff = uint(o.ffn_post - norms_base)
            pc_post.ln_on = last ? 0u : 1u
            pc_post.lnoff = last ? 0u : uint(gemma4v_block_offs(t, l + 1l).ln1 - norms_base)
            enc_tower_post_add_cls(raw, h, r.s_post_d, pc_post, npos)
        }
        vhz_dep(raw, h, VT_X, 0u, true)
        cmd_copy_range(raw, r.x_dev, 0l, r.host.buf, 0l, bx)
    }
    return vt_served(r, s.x, bx, npos, t.n_layer)
}

let private VTF_XIN = 0x1u
let private VTF_COL = 0x2u
let private VTF_Y1 = 0x4u
let private VTF_Y2 = 0x8u
let private VTF_Y3 = 0x10u
let private VTF_FEAT = 0x20u
let private VTF_X = 0x40u

struct private VtConvStage {
    iw : int64
    ih : int64
    cin : int64
    ow : int64
    oh : int64
    rows : int64     //! oh x ow, the stage's output rows per chunk
    kdp : int64
    slab_elem_off : int64   //! the stage's weight rows' element base in the f16 slab
    boff : int64
}

def private vt_conv_stage(iw, ih, cin : int64) : VtConvStage {
    let ow = conv_s2_out(iw)
    let oh = conv_s2_out(ih)
    return VtConvStage(iw = iw, ih = ih, cin = cin, ow = ow, oh = oh, rows = oh * ow, kdp = (cin * 9l + 63l) / 64l * 64l)
}

var private g_vt_q3a_stages : VtConvStage[3]
var private @scratch @exact_size g_vt_q3a_xin : array<float>   //! the window's chunk blocks [ci][ih][iw] staged for the upload

//! the cblob span uploaded as norms_dev: [lo, hi) covers the three conv biases, the conv_out bias and the position rows
def private vt_q3a_norm_span(t : Qwen3aTower) : tuple<lo : int64; hi : int64> {
    let nch = t.n_ch
    let offs = fixed_array(t.c1_b_off, t.c2_b_off, t.c3_b_off, t.cout_b_off, t.pos_off)
    let ends = fixed_array(t.c1_b_off + nch, t.c2_b_off + nch, t.c3_b_off + nch, t.cout_b_off + t.core.d_model, t.pos_off + QWEN3A_TOK_PER_CHUNK * t.core.d_model)
    var lo = offs[0]
    var hi = ends[0]
    for (i in range(1, 5)) {
        lo = min(lo, offs[i])
        hi = max(hi, ends[i])
    }
    return (lo = lo, hi = hi)
}

def private vt_q3a_geometry(t : Qwen3aTower) {
    let nch = t.n_ch
    g_vt_q3a_stages[0] = vt_conv_stage(QWEN3A_CHUNK, t.core.n_mel, 1l)
    g_vt_q3a_stages[1] = vt_conv_stage(g_vt_q3a_stages[0].ow, g_vt_q3a_stages[0].oh, nch)
    g_vt_q3a_stages[2] = vt_conv_stage(g_vt_q3a_stages[1].ow, g_vt_q3a_stages[1].oh, nch)
    let lo = vt_q3a_norm_span(t).lo
    let boffs = fixed_array(t.c1_b_off, t.c2_b_off, t.c3_b_off)
    var total = 0l
    for (s in range(3)) {
        g_vt_q3a_stages[s].slab_elem_off = total
        g_vt_q3a_stages[s].boff = boffs[s] - lo
        total += nch * g_vt_q3a_stages[s].kdp
    }
}

[cold_path]
def private vt_q3a_slab(var r : VtResident; t : Qwen3aTower; key : uint64; lay : Q8RepackLayout) : bool {
    let nch = t.n_ch
    let woffs = fixed_array(t.c1_w_off, t.c2_w_off, t.c3_w_off)
    var total = 0l
    for (st in g_vt_q3a_stages) {
        total += nch * st.kdp
    }
    var slab : array<float>
    slab |> reserve_resize(total)
    for (s in range(3)) {
        let st = g_vt_q3a_stages[s]
        tower_stage_f32_pad_row_stride(slab, t.cblob, woffs[s], st.slab_elem_off, nch, st.cin * 9l, st.kdp)
    }
    var slab16 <- vt_slab_f16(slab)
    let span = vt_q3a_norm_span(t)
    var regions : array<VtRegion>
    regions |> push(VtRegion(woff = t.cout_w_off, n = 16l * nch, rows = t.core.d_model))
    let ok = vt_upload(r, key, t.qcblob, t.qcscales, regions, t.cblob, span.lo, span.hi - span.lo, 0l, lay)
    delete regions
    if (!ok) {
        delete slab16
        return false
    }
    r.fw16_bytes = total * 2l
    r.fw16_dev = vt_dev_buf(r, r.fw16_bytes)
    if (r.oom) {
        delete slab16
        vt_release(r)
        return false
    }
    upload_region_at(r.fw16_dev, 0l, unsafe(addr<uint8 const?>(slab16[0])), r.fw16_bytes)
    delete slab16
    r.front = true
    return true
}

def private vt_q3a_col_bytes(cap_chunks : int64) : int64 {
    var b = 0l
    for (st in g_vt_q3a_stages) {
        b = max(b, cap_chunks * st.rows * st.kdp * 2l)
    }
    return b
}

[cold_path]
def private vt_q3a_scratch(var r : VtResident; cap_chunks, nm, d : int64; gemm_khr : bool) : bool {
    let nch = r.d
    vt_release_scratch(r)
    r.oom = false
    r.meta_bytes = BATCH_META_BYTES
    r.meta = vt_host_buf(r, r.meta_bytes, false)
    r.cap_rows = cap_chunks * QWEN3A_TOK_PER_CHUNK
    r.front_cap = cap_chunks
    let featw = 16l * nch
    let bxin = cap_chunks * QWEN3A_CHUNK * nm * 4l
    let bcol = vt_q3a_col_bytes(cap_chunks)
    let by = fixed_array(cap_chunks * g_vt_q3a_stages[0].rows * nch * 4l, cap_chunks * g_vt_q3a_stages[1].rows * nch * 4l, cap_chunks * g_vt_q3a_stages[2].rows * nch * 4l)
    let bfeat = (r.cap_rows + TILE_READ_SLACK) * featw * 2l
    let bx = r.cap_rows * d * 4l
    if (!vt_range_ok(max(max(bxin, bcol), max(max(by[0], by[1]), max(by[2], bfeat))))) {
        return false
    }
    r.fxin_dev = vt_dev_buf(r, bxin)
    r.fcol_dev = vt_dev_buf(r, bcol)
    r.fy1_dev = vt_dev_buf(r, by[0])
    r.fy2_dev = vt_dev_buf(r, by[1])
    r.fy3_dev = vt_dev_buf(r, by[2])
    r.ffeat_dev = vt_dev_buf(r, bfeat)
    r.fx_dev = vt_dev_buf(r, bx)
    r.host = vt_host_buf(r, bx, true)
    if (r.oom) {
        vt_release_scratch(r)
        return false
    }
    let srcs = fixed_array(r.fxin_dev, r.fy1_dev, r.fy2_dev)
    let src_bytes = fixed_array(bxin, by[0], by[1])
    let src_bits = fixed_array(VTF_XIN, VTF_Y1, VTF_Y2)
    let ys = fixed_array(r.fy1_dev, r.fy2_dev, r.fy3_dev)
    let y_bits = fixed_array(VTF_Y1, VTF_Y2, VTF_Y3)
    for (s in range(3)) {
        r.s_fim[s] = set_tower_im2col_cls(fixed_array(srcs[s], r.fcol_dev), fixed_array(src_bytes[s], bcol), fixed_array(src_bits[s], VTF_COL))
        r.s_fg[s] = f16_gemm_set(gemm_khr, fixed_array(r.fw16_dev, r.fcol_dev, ys[s]), fixed_array(r.fw16_bytes, bcol, by[s]), fixed_array(0u, VTF_COL, y_bits[s]))
        r.s_fb[s] = set_tower_bias_act_cls(fixed_array(ys[s], r.norms_dev), fixed_array(by[s], r.norms_bytes), fixed_array(y_bits[s], 0u))
    }
    r.s_fshuf = set_tower_q3a_shuffle_cls(fixed_array(r.fy3_dev, r.ffeat_dev), fixed_array(by[2], bfeat), fixed_array(VTF_Y3, VTF_FEAT))
    r.s_ffin = set_tower_bias_act_cls(fixed_array(r.fx_dev, r.norms_dev), fixed_array(bx, r.norms_bytes), fixed_array(VTF_X, 0u))
    r.sets_rel_proj |> clear()   //! the conv_out cm2 sets per token column live here (the front has no rel table)
    return true
}

[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vulkan_q3a_front(t : Qwen3aTower; win : MelChunk; n_chunks : int64; var s : EncoderState) : bool {   // nolint:STYLE038 — the dispatch chain is the shape
    var r & = g_vt_q3a_front
    if (!g_vt_env_tower) {
        return vt_decline(r, VulkanTowerDecline.knob)
    }
    if (!t.cq8) {
        return vt_decline(r, VulkanTowerDecline.quant_mode)
    }
    let nm = t.core.n_mel
    let nch = t.n_ch
    let d = t.core.d_model
    let featw = 16l * nch
    let npos = n_chunks * QWEN3A_TOK_PER_CHUNK
    vt_q3a_geometry(t)
    if (g_vt_q3a_stages[2].ow != QWEN3A_TOK_PER_CHUNK || g_vt_q3a_stages[2].oh != 16l || nch % 16l != 0l || featw % 64l != 0l || d % 64l != 0l
            || n_chunks <= 0l || npos > VT_MAX_ENCODE_ROWS || QWEN3A_TOK_PER_CHUNK != int64(Q3A_TOK_PER_CHUNK)) {
        return vt_decline(r, VulkanTowerDecline.shape)
    }
    let arm = vt_arm(r, true)
    if (!arm.ok) {
        return false
    }
    if (!vt_cm2_feed() || !f16_gemm_ensure(arm.gemm_khr) || !ensure_tower_im2col_cls() || !ensure_tower_bias_act_cls()
            || !ensure_tower_q3a_shuffle_cls()) {
        return vt_decline(r, VulkanTowerDecline.device)
    }
    let tc = cm2_tile_cols(d, npos)
    if (!cm2_cls_ensure(int(KqFmt.q8), tc)) {
        return vt_decline(r, VulkanTowerDecline.device)
    }
    let lay = active_q8_repack_layout(g_active_needs_repack)
    let key = vt_key(t.qcblob, t.qcscales, t.cblob, t.cout_w_off, featw, 0l, 1l, lay.mr, lay.kgroup, lay.wbias)
    if (r.key != key) {
        r.d = nch
        r.ff = featw
        if (!vt_q3a_slab(r, t, key, lay)) {
            return vt_decline(r, VulkanTowerDecline.memory)
        }
    }
    if (r.front_cap < n_chunks || r.gemm_khr != arm.gemm_khr) {
        r.gemm_khr = arm.gemm_khr
        if (!vt_q3a_scratch(r, max(n_chunks, QWEN3A_WINDOW / QWEN3A_CHUNK), nm, d, arm.gemm_khr)) {
            return vt_decline(r, VulkanTowerDecline.memory)
        }
    }
    if (!key_exists(r.sets_rel_proj, int(tc))) {
        let bfeat = (r.cap_rows + TILE_READ_SLACK) * featw * 2l
        r.sets_rel_proj[int(tc)] = cm2_cls_set(int(KqFmt.q8), tc, fixed_array(r.wq_dev, r.ws_dev, r.meta.buf, r.ffeat_dev, r.fx_dev),
            fixed_array(r.wq_bytes, r.ws_bytes, r.meta_bytes, bfeat, r.cap_rows * d * 4l), fixed_array(0u, 0u, 0u, VTF_FEAT, VTF_X))
    }
    var s_cout = r.sets_rel_proj[int(tc)]
    let g_cout = cm2_tiles_at(d, npos, tc)
    var mp = unsafe(reinterpret<uint?>(r.meta.mapped))
    var cur = 4l
    let map_off = vt_sched_map(mp, cur, 0l, t.cout_w_off / kq_elems(KqFmt.q8), npos, g_cout)
    let xin_n = QWEN3A_CHUNK * nm
    g_vt_q3a_xin |> reserve(n_chunks * xin_n)
    g_vt_q3a_xin |> resize(n_chunks * xin_n)
    for (ci in range64(n_chunks)) {
        for (ih in range64(nm)) {
            for (iw in range64(QWEN3A_CHUNK)) {
                g_vt_q3a_xin[ci * xin_n + ih * QWEN3A_CHUNK + iw] = win.data[ih * win.n_len + ci * QWEN3A_CHUNK + iw]
            }
        }
    }
    upload_region_at(r.fxin_dev, 0l, unsafe(addr<uint8 const?>(g_vt_q3a_xin[0])), n_chunks * xin_n * 4l)
    let bx = npos * d * 4l
    vt_chain_one_shot("qwen3a-front", 3l, npos) $(raw : VkCommandBuffer; var h : VkHaz) {
        for (si in range(3)) {
            let st = g_vt_q3a_stages[si]
            let rows = n_chunks * st.rows
            var pc_im = TowerIm2colArgs(iw = uint(st.iw), ih = uint(st.ih), cin = uint(st.cin), kdp = uint(st.kdp), rows = uint(rows))
            enc_tower_im2col_cls(raw, h, r.s_fim[si], pc_im, (rows * st.kdp + 255l) / 256l)
            vt_pt(raw, VtProfRole.im2col)
            var pc_g = F16GemmArgs(n = uint(st.kdp), d = uint(nch), rows = uint(rows), woff = uint(st.slab_elem_off), ybase = 0u, ystride = uint(nch), ksplit = 0u)
            f16_gemm_enc(arm.gemm_khr, raw, h, r.s_fg[si], pc_g, rows, nch)
            vt_pt(raw, VtProfRole.gemm_pw)
            vt_bias_act(raw, h, r.s_fb[si], nch, rows * nch, st.boff, BIAS_ACT_GELU_ERF)
            vt_pt(raw, VtProfRole.bias)
        }
        var pc_sh = TowerQ3aShuffleArgs(nch = uint(nch), npos = uint(npos))
        enc_tower_q3a_shuffle_cls(raw, h, r.s_fshuf, pc_sh, (npos * featw + 255l) / 256l)
        vt_pt(raw, VtProfRole.shuffle)
        var pc_cout = BatchArgs(n = uint(featw), d = uint(d), map_off = map_off)
        cm2_cls_enc(int(KqFmt.q8), tc, raw, h, s_cout, pc_cout, g_cout)
        vt_pt(raw, VtProfRole.gemm_pw)
        let lo = vt_q3a_norm_span(t).lo
        vt_bias_act(raw, h, r.s_ffin, d, npos * d, t.cout_b_off - lo, BIAS_ACT_NONE)
        vt_bias_act(raw, h, r.s_ffin, QWEN3A_TOK_PER_CHUNK * d, npos * d, t.pos_off - lo, BIAS_ACT_NONE)
        vt_pt(raw, VtProfRole.post_add)
        vhz_dep(raw, h, VTF_X, 0u, true)
        cmd_copy_range(raw, r.fx_dev, 0l, r.host.buf, 0l, bx)
        vt_pt(raw, VtProfRole.copy_out)
    }
    unsafe {
        memcpy(addr<void?>(s.x[0]), r.host.mapped, bx)
    }
    g_vt_convs += n_chunks
    vt_announce(r)
    return true
}

let private VTF_MEL = 0x80u

[cold_path]
def private vt_g4a_front_slab(var r : VtResident; t : Gemma4aEncoder) : bool {
    let kdp0 = 64l
    let kd1 = t.sub_ch0 * 9l
    let kdp1 = (kd1 + 63l) / 64l * 64l
    let total = t.sub_ch0 * kdp0 + t.sub_ch1 * kdp1
    var slab : array<float>
    slab |> reserve_resize(total)
    tower_stage_f32_pad_row_stride(slab, t.blob, t.c0_w, 0l, t.sub_ch0, 9l, kdp0)
    tower_stage_f32_pad_row_stride(slab, t.blob, t.c1_w, t.sub_ch0 * kdp0, t.sub_ch1, kd1, kdp1)
    let n_fft = (t.n_bins - 1l) * 2l
    return vt_front_upload(r, slab, t.twiddles, n_fft * 2l * t.n_bins * 4l, t.filters, t.n_mel * t.n_bins * 4l)
}

//! a spectral front's model-owned planes uploaded (the f16 conv slab, the DFT twiddles, the mel filters); false with the buffers released on a failed allocation
[cold_path]
def private vt_front_upload(var r : VtResident; var slab : array<float>; twiddles : PlaneF; btw : int64; filters : PlaneF; bfil : int64) : bool {
    var slab16 <- vt_slab_f16(slab)
    r.fw16_bytes = long_length(slab16) * 2l
    r.fw16_dev = vt_dev_buf(r, r.fw16_bytes)
    r.front_twiddles_dev = vt_dev_buf(r, btw)
    r.front_filters_dev = vt_dev_buf(r, bfil)
    if (r.oom) {
        for (b in fixed_array(r.fw16_dev, r.front_twiddles_dev, r.front_filters_dev)) {
            destroy_device_buf(b)
        }
        delete slab16
        return false
    }
    upload_region_at(r.fw16_dev, 0l, unsafe(addr<uint8 const?>(slab16[0])), r.fw16_bytes)
    delete slab16
    upload_region_at(r.front_twiddles_dev, 0l, unsafe(reinterpret<uint8 const?>(plane_at(twiddles, 0l))), btw)
    upload_region_at(r.front_filters_dev, 0l, unsafe(reinterpret<uint8 const?>(plane_at(filters, 0l))), bfil)
    r.front = true
    return true
}

[cold_path]
def private vt_g4a_front_scratch(var r : VtResident; t : Gemma4aEncoder; cap : int64; gemm_khr : bool) : bool {
    vt_front_release(r)
    r.oom = false
    let n_fft = (t.n_bins - 1l) * 2l
    let n_bins = t.n_bins
    let n_mel = t.n_mel
    let ch0 = t.sub_ch0
    let ch1 = t.sub_ch1
    let d = t.d_model
    let kdp0 = 64l
    let kdp1 = (ch0 * 9l + 63l) / 64l * 64l
    let t1 = conv_s2_out(cap)
    let w1 = conv_s2_out(n_mel)
    let t2 = conv_s2_out(t1)
    let w2 = conv_s2_out(w1)
    let rows0 = t1 * w1
    let rows1 = t2 * w2
    let feat = w2 * ch1
    let bxw = cap * n_fft * 4l
    let bmag = cap * n_bins * 4l
    let bmel = cap * n_mel * 4l
    let bcol = max(rows0 * kdp0, rows1 * kdp1) * 2l
    let by0 = rows0 * ch0 * 4l
    let by1 = rows1 * ch1 * 4l
    let bfeat = (t2 + TILE_READ_SLACK) * feat * 2l
    let bx = t2 * d * 4l
    let btw = n_fft * 2l * n_bins * 4l
    let bfil = n_mel * n_bins * 4l
    if (!vt_range_ok(max(max(bxw, bcol), max(max(by0, by1), max(bfeat, bmag))))) {
        return false
    }
    r.front_cap = cap
    r.fxin_dev = vt_dev_buf(r, bxw)
    r.fy3_dev = vt_dev_buf(r, bmag)
    r.fmel_dev = vt_dev_buf(r, bmel)
    r.fcol_dev = vt_dev_buf(r, bcol)
    r.fy1_dev = vt_dev_buf(r, by0)
    r.fy2_dev = vt_dev_buf(r, by1)
    r.ffeat_dev = vt_dev_buf(r, bfeat)
    r.fx_dev = vt_dev_buf(r, bx)
    r.fhost = vt_host_buf(r, bx, true)
    r.fmeta_bytes = BATCH_META_BYTES
    r.fmeta = vt_host_buf(r, r.fmeta_bytes, false)
    if (r.oom) {
        vt_front_release(r)
        return false
    }
    r.s_fdft = set_tower_spec_dft_cls(fixed_array(r.fxin_dev, r.front_twiddles_dev, r.fy3_dev), fixed_array(bxw, btw, bmag), fixed_array(VTF_XIN, 0u, VTF_Y3))
    r.s_fmel = set_tower_mel_log_cls(fixed_array(r.fy3_dev, r.front_filters_dev, r.fmel_dev), fixed_array(bmag, bfil, bmel), fixed_array(VTF_Y3, 0u, VTF_MEL))
    r.s_fim[0] = set_tower_im2col_cls(fixed_array(r.fmel_dev, r.fcol_dev), fixed_array(bmel, bcol), fixed_array(VTF_MEL, VTF_COL))
    r.s_fg[0] = f16_gemm_set(gemm_khr, fixed_array(r.fw16_dev, r.fcol_dev, r.fy1_dev), fixed_array(r.fw16_bytes, bcol, by0), fixed_array(0u, VTF_COL, VTF_Y1))
    r.s_fb[0] = set_tower_ln_relu_cls(fixed_array(r.fy1_dev, r.norms_dev), fixed_array(by0, r.norms_bytes), fixed_array(VTF_Y1, 0u))
    r.s_fim[1] = set_tower_im2col_cls(fixed_array(r.fy1_dev, r.fcol_dev), fixed_array(by0, bcol), fixed_array(VTF_Y1, VTF_COL))
    r.s_fg[1] = f16_gemm_set(gemm_khr, fixed_array(r.fw16_dev, r.fcol_dev, r.fy2_dev), fixed_array(r.fw16_bytes, bcol, by1), fixed_array(0u, VTF_COL, VTF_Y2))
    r.s_fb[1] = set_tower_ln_relu_cls(fixed_array(r.fy2_dev, r.norms_dev), fixed_array(by1, r.norms_bytes), fixed_array(VTF_Y2, 0u))
    r.s_fcvt = set_tower_clamp_cvt_cls(fixed_array(r.fy2_dev, r.ffeat_dev), fixed_array(by1, bfeat), fixed_array(VTF_Y2, VTF_FEAT))
    return true
}

[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vulkan_gemma4a_chunk(t : Gemma4aEncoder; var s : Gemma4aState; n_len : int64) : int64 {   // nolint:STYLE038 — the dispatch chain is the shape
    var r & = g_vt_g4a
    if (!g_vt_env_tower) {
        vt_decline(r, VulkanTowerDecline.knob)
        return -1l
    }
    if (!t.q8) {
        vt_decline(r, VulkanTowerDecline.quant_mode)
        return -1l
    }
    let d = t.d_model
    let n_mel = t.n_mel
    let n_bins = t.n_bins
    let n_fft = (n_bins - 1l) * 2l
    let ch0 = t.sub_ch0
    let ch1 = t.sub_ch1
    let t1 = conv_s2_out(n_len)
    let w1 = conv_s2_out(n_mel)
    let t2 = conv_s2_out(t1)
    let w2 = conv_s2_out(w1)
    let npos = t2
    let feat = w2 * ch1
    let rows0 = t1 * w1
    let rows1 = t2 * w2
    let kdp1 = (ch0 * 9l + 63l) / 64l * 64l
    if (n_len < 4l || feat != d || ch0 != n_mel || ch1 % 16l != 0l || d % 64l != 0l || long_length(s.xw) < n_len * n_fft
            || npos > VT_MAX_ENCODE_ROWS) {
        vt_decline(r, VulkanTowerDecline.shape)
        return -1l
    }
    let arm = vt_arm(r, true)
    if (!arm.ok) {
        return -1l
    }
    let tc = cm2_tile_cols(d, npos)
    if (!vt_cm2_feed() || !f16_gemm_ensure(arm.gemm_khr) || !ensure_tower_im2col_cls() || !ensure_tower_spec_dft_cls() || !ensure_tower_mel_log_cls()
            || !ensure_tower_ln_relu_cls() || !ensure_tower_clamp_cvt_cls() || !cm2_cls_ensure(int(KqFmt.q8), tc)) {
        vt_decline(r, VulkanTowerDecline.device)
        return -1l
    }
    let lay = active_q8_repack_layout(g_active_needs_repack)
    let key = vt_g4a_key(t, lay)
    if (r.key != key && !vt_upload_g4a(r, key, t, lay)) {
        vt_decline(r, VulkanTowerDecline.memory)
        return -1l
    }
    if (!r.front && !vt_g4a_front_slab(r, t)) {
        vt_decline(r, VulkanTowerDecline.memory)
        return -1l
    }
    if (r.front_cap < n_len || r.gemm_khr != arm.gemm_khr) {
        r.gemm_khr = arm.gemm_khr
        if (!vt_g4a_front_scratch(r, t, max(n_len, 3008l), arm.gemm_khr)) {
            vt_decline(r, VulkanTowerDecline.memory)
            return -1l
        }
    }
    if (!key_exists(r.sets_front, int(tc))) {
        let cap2 = (r.front_cap - 1l) / 4l + 1l
        r.sets_front[int(tc)] = cm2_cls_set(int(KqFmt.q8), tc, fixed_array(r.wq_dev, r.ws_dev, r.fmeta.buf, r.ffeat_dev, r.fx_dev),
            fixed_array(r.wq_bytes, r.ws_bytes, r.fmeta_bytes, (cap2 + TILE_READ_SLACK) * feat * 2l, cap2 * d * 4l), fixed_array(0u, 0u, 0u, VTF_FEAT, VTF_X))
    }
    var s_in = r.sets_front[int(tc)]
    let g_in = cm2_tiles_at(d, npos, tc)
    var mp = unsafe(reinterpret<uint?>(r.fmeta.mapped))
    var cur = 4l
    let map_off = vt_sched_map(mp, cur, 0l, t.inproj_w / kq_elems(KqFmt.q8), npos, g_in)
    upload_region_at(r.fxin_dev, 0l, unsafe(addr<uint8 const?>(s.xw[0])), n_len * n_fft * 4l)
    let bx = npos * d * 4l
    vt_chain_one_shot("gemma4a-front", 2l, npos) $(raw : VkCommandBuffer; var h : VkHaz) {
        var pc_dft = TowerSpecDftArgs(n_fft = uint(n_fft), n_frames = uint(n_len), mag = 1u)
        enc_tower_spec_dft_cls(raw, h, r.s_fdft, pc_dft, (n_len * n_bins + 255l) / 256l)
        vt_pt(raw, VtProfRole.spectrum)
        var pc_mel = TowerMelLogArgs(n_bins = uint(n_bins), n_mel = uint(n_mel), n_frames = uint(n_len),
            floor = gemma4a_mel_floor(), scale = 1.0, frame_major = 1u, add_floor = 0u)
        enc_tower_mel_log_cls(raw, h, r.s_fmel, pc_mel, (n_len * n_mel + 255l) / 256l)
        vt_pt(raw, VtProfRole.mel)
        var pc_im0 = TowerIm2colArgs(iw = uint(n_mel), ih = uint(n_len), cin = 1u, kdp = 64u, rows = uint(rows0))
        enc_tower_im2col_cls(raw, h, r.s_fim[0], pc_im0, (rows0 * 64l + 255l) / 256l)
        vt_pt(raw, VtProfRole.im2col)
        var pc_g0 = F16GemmArgs(n = 64u, d = uint(ch0), rows = uint(rows0), woff = 0u, ybase = 0u, ystride = uint(ch0), ksplit = 0u)
        f16_gemm_enc(arm.gemm_khr, raw, h, r.s_fg[0], pc_g0, rows0, ch0)
        vt_pt(raw, VtProfRole.gemm_pw)
        var pc_ln0 = TowerLnReluArgs(cout = uint(ch0), rows = uint(rows0), woff = uint(t.norm0_w), eps = t.eps)
        enc_tower_ln_relu_cls(raw, h, r.s_fb[0], pc_ln0, (rows0 + 255l) / 256l)
        vt_pt(raw, VtProfRole.bias)
        var pc_im1 = TowerIm2colArgs(iw = uint(w1), ih = uint(t1), cin = uint(ch0), kdp = uint(kdp1), rows = uint(rows1))
        enc_tower_im2col_cls(raw, h, r.s_fim[1], pc_im1, (rows1 * kdp1 + 255l) / 256l)
        vt_pt(raw, VtProfRole.im2col)
        var pc_g1 = F16GemmArgs(n = uint(kdp1), d = uint(ch1), rows = uint(rows1), woff = uint(ch0 * 64l), ybase = 0u, ystride = uint(ch1), ksplit = 0u)
        f16_gemm_enc(arm.gemm_khr, raw, h, r.s_fg[1], pc_g1, rows1, ch1)
        vt_pt(raw, VtProfRole.gemm_pw)
        var pc_ln1 = TowerLnReluArgs(cout = uint(ch1), rows = uint(rows1), woff = uint(t.norm1_w), eps = t.eps)
        enc_tower_ln_relu_cls(raw, h, r.s_fb[1], pc_ln1, (rows1 + 255l) / 256l)
        vt_pt(raw, VtProfRole.bias)
        vt_cvt16(raw, h, r.s_fcvt, npos * feat)
        vt_pt(raw, VtProfRole.hpad)
        var pc_in = BatchArgs(n = uint(feat), d = uint(d), map_off = map_off)
        cm2_cls_enc(int(KqFmt.q8), tc, raw, h, s_in, pc_in, g_in)
        vt_pt(raw, VtProfRole.gemm_pw)
        vhz_dep(raw, h, VTF_X, 0u, true)
        cmd_copy_range(raw, r.fx_dev, 0l, r.fhost.buf, 0l, bx)
        vt_pt(raw, VtProfRole.copy_out)
    }
    ensure_length(s.x, npos * d)
    unsafe {
        memcpy(addr<void?>(s.x[0]), r.fhost.mapped, bx)
    }
    g_vt_convs++
    if (!vulkan_gemma4a_blocks(t, s, npos)) {
        return -1l
    }
    return npos
}

let private VTF_Y4 = 0x100u
let private VTF_Y5 = 0x200u
let private VTF_XF = 0x400u
let private VTF_H = 0x800u
let private VT_CN_FRONT_KEY = 4096
let private VT_CN_FRONT_PW3 = 0   //! the canary front's cm2 set stages: the two pointwise GEMMs and the input projection
let private VT_CN_FRONT_PW6 = 1
let private VT_CN_FRONT_PRE = 2

[cold_path]
def private vt_cn_front_slab(var r : VtResident; t : CanaryEncoder) : bool {
    let nch = t.n_sub_ch
    let kdp0 = 64l
    let total = nch * kdp0
    var slab : array<float>
    slab |> reserve_resize(total)
    tower_stage_f32_pad_row_stride(slab, t.blob, t.c0_w, 0l, nch, 9l, kdp0)
    return vt_front_upload(r, slab, t.twiddles, t.n_fft * 2l * t.n_fb * 4l, t.filters, t.n_mel * t.n_fb * 4l)
}

struct private VtCnGeom {
    t1 : int64
    w1 : int64
    t2 : int64
    w2 : int64
    t3 : int64
    w3 : int64
}

def private vt_cn_geom(n_len, n_mel : int64) : VtCnGeom {
    let t1 = conv_s2_out(n_len)
    let w1 = conv_s2_out(n_mel)
    let t2 = conv_s2_out(t1)
    let w2 = conv_s2_out(w1)
    return VtCnGeom(t1 = t1, w1 = w1, t2 = t2, w2 = w2, t3 = conv_s2_out(t2), w3 = conv_s2_out(w2))
}

[cold_path]
def private vt_cn_front_scratch(var r : VtResident; t : CanaryEncoder; cap : int64; gemm_khr : bool) : bool {
    vt_front_release(r)
    r.oom = false
    let n_fft = t.n_fft
    let n_fb = t.n_fb
    let n_mel = t.n_mel
    let nch = t.n_sub_ch
    let d = t.d_model
    let g = vt_cn_geom(cap, n_mel)
    let rows0 = g.t1 * g.w1
    let rows2 = g.t2 * g.w2
    let rows3 = g.t3 * g.w3
    let feat = g.w3 * nch
    let bxw = cap * n_fft * 4l
    let bpow = cap * n_fb * 4l
    let bmel = cap * n_mel * 4l
    let bcol = rows0 * 64l * 2l
    let by0 = rows0 * nch * 4l
    let by2 = rows2 * nch * 4l
    let by3 = rows3 * nch * 4l
    let bxf = g.t3 * feat * 4l
    let bh = (rows2 + TILE_READ_SLACK) * nch * 2l
    let bx = g.t3 * d * 4l
    let btw = n_fft * 2l * n_fb * 4l
    let bfil = n_mel * n_fb * 4l
    if (!vt_range_ok(max(max(by0, bh), max(bxf, bx)))) {
        return false
    }
    r.front_cap = cap
    r.fxin_dev = vt_dev_buf(r, bxw)
    r.fy3_dev = vt_dev_buf(r, bpow)
    r.fmel_dev = vt_dev_buf(r, bmel)
    r.fcol_dev = vt_dev_buf(r, bcol)
    r.fy1_dev = vt_dev_buf(r, by0)
    r.fy2_dev = vt_dev_buf(r, by2)
    r.fcn_pw3_dev = vt_dev_buf(r, by2)
    r.fcn_dw5_dev = vt_dev_buf(r, by3)
    r.ffeat_dev = vt_dev_buf(r, by3)
    r.fcn_feat_dev = vt_dev_buf(r, bxf)
    r.fcn_feed16_dev = vt_dev_buf(r, bh)
    r.fh_bytes = bh
    r.fx_dev = vt_dev_buf(r, bx)
    r.fhost = vt_host_buf(r, bx, true)
    r.fmeta_bytes = BATCH_META_BYTES
    r.fmeta = vt_host_buf(r, r.fmeta_bytes, false)
    if (r.oom) {
        vt_front_release(r)
        return false
    }
    r.s_fdft = set_tower_spec_dft_cls(fixed_array(r.fxin_dev, r.front_twiddles_dev, r.fy3_dev), fixed_array(bxw, btw, bpow), fixed_array(VTF_XIN, 0u, VTF_Y3))
    r.s_fmel = set_tower_mel_log_cls(fixed_array(r.fy3_dev, r.front_filters_dev, r.fmel_dev), fixed_array(bpow, bfil, bmel), fixed_array(VTF_Y3, 0u, VTF_MEL))
    r.s_fb[2] = set_tower_mel_norm_cls(fixed_array(r.fmel_dev), fixed_array(bmel), fixed_array(VTF_MEL))
    r.s_fim[0] = set_tower_im2col_cls(fixed_array(r.fmel_dev, r.fcol_dev), fixed_array(bmel, bcol), fixed_array(VTF_MEL, VTF_COL))
    r.s_fg[0] = f16_gemm_set(gemm_khr, fixed_array(r.fw16_dev, r.fcol_dev, r.fy1_dev), fixed_array(r.fw16_bytes, bcol, by0), fixed_array(0u, VTF_COL, VTF_Y1))
    r.s_fb[0] = set_tower_bias_act_cls(fixed_array(r.fy1_dev, r.norms_dev), fixed_array(by0, r.norms_bytes), fixed_array(VTF_Y1, 0u))
    r.s_fg[1] = set_tower_dw_conv_cls(fixed_array(r.fy1_dev, r.fy2_dev, r.norms_dev), fixed_array(by0, by2, r.norms_bytes), fixed_array(VTF_Y1, VTF_Y2, 0u))
    r.s_fg[2] = set_tower_dw_conv_cls(fixed_array(r.fcn_pw3_dev, r.fcn_dw5_dev, r.norms_dev), fixed_array(by2, by3, r.norms_bytes), fixed_array(VTF_Y4, VTF_Y5, 0u))
    r.s_fb[1] = set_tower_bias_act_cls(fixed_array(r.fcn_pw3_dev, r.norms_dev), fixed_array(by2, r.norms_bytes), fixed_array(VTF_Y4, 0u))
    r.s_fcvt = set_tower_clamp_cvt_cls(fixed_array(r.fy2_dev, r.fcn_feed16_dev), fixed_array(by2, bh), fixed_array(VTF_Y2, VTF_H))
    r.s_fshuf = set_tower_feat_shuffle_cls(fixed_array(r.ffeat_dev, r.fcn_feat_dev), fixed_array(by3, bxf), fixed_array(VTF_FEAT, VTF_XF))
    r.s_ffin = set_tower_bias_act_cls(fixed_array(r.fx_dev, r.norms_dev), fixed_array(bx, r.norms_bytes), fixed_array(VTF_X, 0u))
    r.s_fcn_feed16_dw5 = set_tower_clamp_cvt_cls(fixed_array(r.fcn_dw5_dev, r.fcn_feed16_dev), fixed_array(by3, bh), fixed_array(VTF_Y5, VTF_H))
    r.s_fcn_feed16_feat = set_tower_clamp_cvt_cls(fixed_array(r.fcn_feat_dev, r.fcn_feed16_dev), fixed_array(bxf, bh), fixed_array(VTF_XF, VTF_H))
    r.s_fcn_bias_pw6 = set_tower_bias_act_cls(fixed_array(r.ffeat_dev, r.norms_dev), fixed_array(by3, r.norms_bytes), fixed_array(VTF_FEAT, 0u))
    return true
}

//! the front's cm2 GEMM set of one stage (the pointwise convs into y3 and y6, the input projection into x) at a token column
def private vt_cn_front_set(var r : VtResident; stage : int; tc : int64; y_dev : uint64; y_bytes : int64; y_bit : uint) : VkDescriptorSet {
    let key = stage * VT_CN_FRONT_KEY + int(tc)
    if (!key_exists(r.sets_front, key)) {
        r.sets_front[key] = cm2_cls_set(int(KqFmt.q8), tc, fixed_array(r.wq_dev, r.ws_dev, r.fmeta.buf, r.fcn_feed16_dev, y_dev),
            fixed_array(r.wq_bytes, r.ws_bytes, r.fmeta_bytes, r.fh_bytes, y_bytes), fixed_array(0u, 0u, 0u, VTF_H, y_bit))
    }
    return r.sets_front[key]
}

[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vulkan_canary_front(t : CanaryEncoder; var s : CanaryState) : int64 {   // nolint:STYLE038 — the dispatch chain is the shape
    var r & = g_vt_cn
    if (!g_vt_env_tower) {
        vt_decline(r, VulkanTowerDecline.knob)
        return -1l
    }
    if (!t.q8) {
        vt_decline(r, VulkanTowerDecline.quant_mode)
        return -1l
    }
    let n_len = s.n_len
    let n_mel = t.n_mel
    let n_fb = t.n_fb
    let n_fft = t.n_fft
    let nch = t.n_sub_ch
    let d = t.d_model
    let g = vt_cn_geom(n_len, n_mel)
    let rows0 = g.t1 * g.w1
    let rows2 = g.t2 * g.w2
    let rows3 = g.t3 * g.w3
    let tt = g.t3
    let feat = g.w3 * nch
    let l0 = conv_s2_out(s.n_valid)
    let l2 = conv_s2_out(l0)
    if (n_len < 4l || nch % 64l != 0l || feat % 64l != 0l || d % 64l != 0l || n_fb != n_fft / 2l + 1l || long_length(s.xw) < n_len * n_fft
            || !vt_cn_rows_ok(tt, d, t.n_ff)) {
        vt_decline(r, VulkanTowerDecline.shape)
        return -1l
    }
    let arm = vt_arm(r, true)
    if (!arm.ok) {
        return -1l
    }
    let tc2 = cm2_tile_cols(nch, rows2)
    let tc3 = cm2_tile_cols(nch, rows3)
    let tcx = cm2_tile_cols(d, tt)
    if (!vt_cm2_feed() || !f16_gemm_ensure(arm.gemm_khr) || !ensure_tower_im2col_cls() || !ensure_tower_spec_dft_cls() || !ensure_tower_mel_log_cls()
            || !ensure_tower_mel_norm_cls() || !ensure_tower_dw_conv_cls() || !ensure_tower_feat_shuffle_cls()
            || !ensure_tower_bias_act_cls() || !ensure_tower_clamp_cvt_cls() || !cm2_cls_ensure(int(KqFmt.q8), tc2) || !cm2_cls_ensure(int(KqFmt.q8), tc3)
            || !cm2_cls_ensure(int(KqFmt.q8), tcx)) {
        vt_decline(r, VulkanTowerDecline.device)
        return -1l
    }
    let lay = active_q8_repack_layout(g_active_needs_repack)
    let key = vt_key(t.qblob, t.qscales, t.blob, 0l, t.blob.n, 0l, t.n_layer, lay.mr, lay.kgroup, lay.wbias)
    if (r.key != key && !vt_upload_cn(r, key, t, lay)) {
        vt_decline(r, VulkanTowerDecline.memory)
        return -1l
    }
    if (!r.front && !vt_cn_front_slab(r, t)) {
        vt_decline(r, VulkanTowerDecline.memory)
        return -1l
    }
    r.d = d
    if (r.front_cap < n_len || r.gemm_khr != arm.gemm_khr) {
        r.gemm_khr = arm.gemm_khr
        if (!vt_cn_front_scratch(r, t, max(n_len, 4096l), arm.gemm_khr)) {
            vt_decline(r, VulkanTowerDecline.memory)
            return -1l
        }
    }
    let gc = vt_cn_geom(r.front_cap, n_mel)
    let by2c = gc.t2 * gc.w2 * nch * 4l
    let by3c = gc.t3 * gc.w3 * nch * 4l
    let bxc = gc.t3 * d * 4l
    var s_pw3 = vt_cn_front_set(r, VT_CN_FRONT_PW3, tc2, r.fcn_pw3_dev, by2c, VTF_Y4)
    var s_pw6 = vt_cn_front_set(r, VT_CN_FRONT_PW6, tc3, r.ffeat_dev, by3c, VTF_FEAT)
    var s_pre = vt_cn_front_set(r, VT_CN_FRONT_PRE, tcx, r.fx_dev, bxc, VTF_X)
    let g_pw3 = cm2_tiles_at(nch, rows2, tc2)
    let g_pw6 = cm2_tiles_at(nch, rows3, tc3)
    let g_pre = cm2_tiles_at(d, tt, tcx)
    let q8e = kq_elems(KqFmt.q8)
    var mp = unsafe(reinterpret<uint?>(r.fmeta.mapped))
    var cur = 12l
    let map3 = vt_sched_map(mp, cur, 0l, t.c3_w / q8e, rows2, g_pw3)
    let map6 = vt_sched_map(mp, cur, 1l, t.c6_w / q8e, rows3, g_pw6)
    let mapx = vt_sched_map(mp, cur, 2l, t.pre_w / q8e, tt, g_pre)
    upload_region_at(r.fxin_dev, 0l, unsafe(addr<uint8 const?>(s.xw[0])), n_len * n_fft * 4l)
    let bx = tt * d * 4l
    vt_chain_one_shot("canary-front", 3l, tt) $(raw : VkCommandBuffer; var h : VkHaz) {
        var pc_dft = TowerSpecDftArgs(n_fft = uint(n_fft), n_frames = uint(n_len), mag = 0u)
        enc_tower_spec_dft_cls(raw, h, r.s_fdft, pc_dft, (n_len * n_fb + 255l) / 256l)
        vt_pt(raw, VtProfRole.spectrum)
        var pc_mel = TowerMelLogArgs(n_bins = uint(n_fb), n_mel = uint(n_mel), n_frames = uint(n_len),
            floor = VT_MEL_FLOOR, scale = 1.0, frame_major = 1u, add_floor = 1u)
        enc_tower_mel_log_cls(raw, h, r.s_fmel, pc_mel, (n_len * n_mel + 255l) / 256l)
        var pc_norm = TowerMelNormArgs(n_mel = uint(n_mel), n_frames = uint(n_len), valid = uint(min(s.n_valid, n_len)))
        enc_tower_mel_norm_cls(raw, h, r.s_fb[2], pc_norm, (n_mel + 63l) / 64l)
        vt_pt(raw, VtProfRole.mel)
        var pc_im0 = TowerIm2colArgs(iw = uint(n_mel), ih = uint(n_len), cin = 1u, kdp = 64u, rows = uint(rows0))
        enc_tower_im2col_cls(raw, h, r.s_fim[0], pc_im0, (rows0 * 64l + 255l) / 256l)
        vt_pt(raw, VtProfRole.im2col)
        var pc_g0 = F16GemmArgs(n = 64u, d = uint(nch), rows = uint(rows0), woff = 0u, ybase = 0u, ystride = uint(nch), ksplit = 0u)
        f16_gemm_enc(arm.gemm_khr, raw, h, r.s_fg[0], pc_g0, rows0, nch)
        vt_pt(raw, VtProfRole.gemm_pw)
        vt_bias_act(raw, h, r.s_fb[0], nch, rows0 * nch, t.c0_b, BIAS_ACT_RELU)
        vt_pt(raw, VtProfRole.bias)
        vt_zero_elems(raw, h, r.fy1_dev, VTF_Y1, l0 * g.w1 * nch, rows0 * nch)
        var pc_dw2 = TowerDwConvArgs(iw = uint(g.w1), ih = uint(g.t1), ch = uint(nch), woff = uint(t.c2_w), boff = uint(t.c2_b))
        enc_tower_dw_conv_cls(raw, h, r.s_fg[1], pc_dw2, (rows2 * nch + 255l) / 256l)
        vt_pt(raw, VtProfRole.dwconv)
        vt_cvt16(raw, h, r.s_fcvt, rows2 * nch)
        var pc_pw3 = BatchArgs(n = uint(nch), d = uint(nch), map_off = map3)
        cm2_cls_enc(int(KqFmt.q8), tc2, raw, h, s_pw3, pc_pw3, g_pw3)
        vt_pt(raw, VtProfRole.gemm_pw)
        vt_bias_act(raw, h, r.s_fb[1], nch, rows2 * nch, t.c3_b, BIAS_ACT_RELU)
        vt_pt(raw, VtProfRole.bias)
        vt_zero_elems(raw, h, r.fcn_pw3_dev, VTF_Y4, l2 * g.w2 * nch, rows2 * nch)
        var pc_dw5 = TowerDwConvArgs(iw = uint(g.w2), ih = uint(g.t2), ch = uint(nch), woff = uint(t.c5_w), boff = uint(t.c5_b))
        enc_tower_dw_conv_cls(raw, h, r.s_fg[2], pc_dw5, (rows3 * nch + 255l) / 256l)
        vt_pt(raw, VtProfRole.dwconv)
        vt_cvt16(raw, h, r.s_fcn_feed16_dw5, rows3 * nch)
        var pc_pw6 = BatchArgs(n = uint(nch), d = uint(nch), map_off = map6)
        cm2_cls_enc(int(KqFmt.q8), tc3, raw, h, s_pw6, pc_pw6, g_pw6)
        vt_pt(raw, VtProfRole.gemm_pw)
        vt_bias_act(raw, h, r.s_fcn_bias_pw6, nch, rows3 * nch, t.c6_b, BIAS_ACT_RELU)
        vt_pt(raw, VtProfRole.bias)
        var pc_sh = TowerFeatShuffleArgs(w = uint(g.w3), ch = uint(nch), t = uint(tt))
        enc_tower_feat_shuffle_cls(raw, h, r.s_fshuf, pc_sh, (tt * feat + 255l) / 256l)
        vt_pt(raw, VtProfRole.shuffle)
        vt_cvt16(raw, h, r.s_fcn_feed16_feat, tt * feat)
        var pc_pre = BatchArgs(n = uint(feat), d = uint(d), map_off = mapx)
        cm2_cls_enc(int(KqFmt.q8), tcx, raw, h, s_pre, pc_pre, g_pre)
        vt_pt(raw, VtProfRole.gemm_pw)
        vt_bias_act(raw, h, r.s_ffin, d, tt * d, t.pre_b, BIAS_ACT_NONE)
        vt_pt(raw, VtProfRole.bias)
        vhz_dep(raw, h, VTF_X, 0u, true)
        cmd_copy_range(raw, r.fx_dev, 0l, r.fhost.buf, 0l, bx)
        vt_pt(raw, VtProfRole.copy_out)
    }
    ensure_length(s.x, tt * d)
    unsafe {
        memcpy(addr<void?>(s.x[0]), r.fhost.mapped, bx)
    }
    g_vt_convs++
    return tt
}

var private g_vt_q3am : VtResident   //! the qwen3a mel's resident: the frame scratch (x_dev = the windowed frames, xb_dev = twiddles, xq_dev = filters, q_dev = power, yo_dev = the mel rows)

//! the twiddle and filter panels are per-call uploads: the CPU builds them per call too
[cold_path]
def private vt_q3a_mel_scratch(var r : VtResident; cap, n_fft, n_bins, n_mel : int64) : bool {
    vt_release_scratch(r)
    r.oom = false
    r.cap_rows = cap
    let bxw = cap * n_fft * 4l
    let btw = n_fft * 2l * n_bins * 4l
    let bfil = n_mel * n_bins * 4l
    let bpow = cap * n_bins * 4l
    let bout = n_mel * cap * 4l
    if (!vt_range_ok(max(bxw, max(bpow, bout)))) {
        return false
    }
    r.x_dev = vt_dev_buf(r, bxw)
    r.xb_dev = vt_dev_buf(r, btw)
    r.xq_dev = vt_dev_buf(r, bfil)
    r.q_dev = vt_dev_buf(r, bpow)
    r.yo_dev = vt_dev_buf(r, bout)
    r.host = vt_host_buf(r, bout, true)
    if (r.oom) {
        vt_release_scratch(r)
        return false
    }
    r.s_q = set_tower_spec_dft_cls(fixed_array(r.x_dev, r.xb_dev, r.q_dev), fixed_array(bxw, btw, bpow), fixed_array(VT_X, VT_XB, VT_Q))
    r.s_k = set_tower_mel_log_cls(fixed_array(r.q_dev, r.xq_dev, r.yo_dev), fixed_array(bpow, bfil, bout), fixed_array(VT_Q, VT_XQ, VT_YO))
    return true
}

[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vulkan_q3a_mel(xw : array<float>; n_len, n_fft, n_bins, n_mel : int64; twiddles, filters : array<float>; var m : WhisperMel) : bool {
    var r & = g_vt_q3am
    if (!g_vt_env_tower) {
        return vt_decline(r, VulkanTowerDecline.knob)
    }
    if (n_len < 1l || n_bins != n_fft / 2l + 1l || long_length(xw) < n_len * n_fft || long_length(twiddles) < n_fft * 2l * n_bins
            || long_length(filters) < n_mel * n_bins || long_length(m.data) < n_mel * n_len) {
        return vt_decline(r, VulkanTowerDecline.shape)
    }
    if (!vt_arm(r).ok) {
        return false
    }
    if (!ensure_tower_spec_dft_cls() || !ensure_tower_mel_log_cls()) {
        return vt_decline(r, VulkanTowerDecline.device)
    }
    if (r.cap_rows < n_len && !vt_q3a_mel_scratch(r, max(n_len, 4096l), n_fft, n_bins, n_mel)) {
        return vt_decline(r, VulkanTowerDecline.memory)
    }
    upload_region_at(r.x_dev, 0l, unsafe(addr<uint8 const?>(xw[0])), n_len * n_fft * 4l)
    upload_region_at(r.xb_dev, 0l, unsafe(addr<uint8 const?>(twiddles[0])), n_fft * 2l * n_bins * 4l)
    upload_region_at(r.xq_dev, 0l, unsafe(addr<uint8 const?>(filters[0])), n_mel * n_bins * 4l)
    let bout = n_mel * n_len * 4l
    vt_chain_one_shot("qwen3a-mel", 1l, n_len) $(raw : VkCommandBuffer; var h : VkHaz) {
        var pc_d = TowerSpecDftArgs(n_fft = uint(n_fft), n_frames = uint(n_len), mag = 0u)
        enc_tower_spec_dft_cls(raw, h, r.s_q, pc_d, (n_len * n_bins + 255l) / 256l)
        vt_pt(raw, VtProfRole.spectrum)
        var pc_m = TowerMelLogArgs(n_bins = uint(n_bins), n_mel = uint(n_mel), n_frames = uint(n_len),
            floor = VT_MEL_FLOOR, scale = VT_LOG10_E, frame_major = 0u, add_floor = 0u)
        enc_tower_mel_log_cls(raw, h, r.s_k, pc_m, (n_mel * n_len + 255l) / 256l)
        vt_pt(raw, VtProfRole.mel)
        vhz_dep(raw, h, VT_YO, 0u, true)
        cmd_copy_range(raw, r.yo_dev, 0l, r.host.buf, 0l, bout)
        vt_pt(raw, VtProfRole.copy_out)
    }
    unsafe {
        memcpy(addr<void?>(m.data[0]), r.host.mapped, bout)
    }
    vt_announce(r)
    g_vt_mels++
    return true
}

[init]
def dasllama_vulkan_tower_register {
    g_vt_env_tower = g_env_vulkan.vk_tower
    vt_unbind_batch_sets(g_vt_g4v)
    vt_unbind_batch_sets(g_vt_g3v)
    vt_unbind_batch_sets(g_vt_q3v)
    vt_unbind_batch_sets(g_vt_q25v)
    vt_unbind_batch_sets(g_vt_aud)
    vt_unbind_batch_sets(g_vt_g4a)
    vt_unbind_batch_sets(g_vt_cn)
    vt_unbind_batch_sets(g_vt_q3a_front)
    vt_unbind_batch_sets(g_vt_q3am)
    g_vt_q3a_front.family = "qwen3a-front"
    g_vt_q3am.family = "qwen3a-mel"
    g_vt_g4v.family = "gemma4v"
    g_vt_g3v.family = "gemma3v"
    g_vt_q3v.family = "qwen3v"
    g_vt_q25v.family = "qwen25v"
    g_vt_aud.family = "whisper-class"
    g_vt_g4a.family = "gemma4a"
    g_vt_cn.family = "canary"
    register_vk_drop_hook(@@vt_forget)
    static_if (!typeinfo builtin_module_exists(das_metal)) {
        register_gemma4v_gpu(@@vulkan_gemma4v_blocks, @@vulkan_tower_serves_exact)
        register_gemma3v_gpu(@@vulkan_gemma3v_blocks, @@vulkan_tower_serves_exact)
        register_qwen3v_gpu_blocks(@@vulkan_qwen3v_blocks)
        register_qwen25v_gpu_blocks(@@vulkan_qwen25v_blocks)
        register_tower_blocks_gpu(@@vulkan_audio_tower_blocks, @@vulkan_tower_serves_exact)
        register_tower_blocks_ln_post_gpu(@@vulkan_audio_tower_blocks_ln_post)
        register_tower_conv_gpu(@@vulkan_audio_conv_front)
        register_gemma4a_gpu(@@vulkan_gemma4a_blocks, @@vulkan_tower_serves_exact)
        register_canary_gpu(@@vulkan_canary_blocks, @@vulkan_tower_serves_exact)
        register_qwen3a_front_gpu(@@vulkan_q3a_front)
        register_qwen3a_mel_gpu(@@vulkan_q3a_mel)
        register_gemma4a_chunk_gpu(@@vulkan_gemma4a_chunk)
        register_canary_front_gpu(@@vulkan_canary_front)
    }
}
