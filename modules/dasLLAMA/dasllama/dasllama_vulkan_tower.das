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
require dasllama/dasllama_gemm_schema   // kq_dev_ssb, dev_rows_bytes - the kq lane's device plane strides
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
require dasllama/dasllama_gemma4uv
require dasllama/dasllama_audio
require dasllama/dasllama_gemma4a
require dasllama/dasllama_canary
require dasllama/dasllama_parakeet
require dasllama/dasllama_gigaam
require dasllama/dasllama_vad   // VadModel/VadState - the silero VAD seat
require dasllama/dasllama_qwen3a
require dasllama/dasllama_convert
require dasllama/dasllama_gpu_math   // the row stamps' activation codes

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
var private g_vt_mels = 0l    //! the log-mels served on the device: qwen3a's fronts, gigaam's encodes and mel seat
var private g_vt_declines : VkDeclineCounter

enum private VulkanTowerDecline {
    knob         //! DASLLAMA_VK_TOWER=0, or set_vulkan_tower(false)
    ga_front     //! set_vulkan_gigaam_front(false): the GigaAM conv front pinned to the CPU, the block seat serving behind it
    blocks       //! set_vulkan_audio_blocks(false): the whisper-class block hooks pinned off while the stem still serves
    quant_mode   //! the tower's planes are not the lane the driver serves
    kq_lane      //! a kq lane member with no K-quant tile on the device
    flavor       //! the model is not the Vulkan-flavored image: each backend serves its own (`#tower-dev-block`)
    shape        //! a width, head size, row count or buffer range the stamps do not serve
    device       //! the tier has no device, or a class failed to build
    memory       //! a device or pinned allocation failed: the tower stays on the CPU chain
}

let VK_DECLINE_WORDS_TOWER_KNOB = "the tower knob is off (DASLLAMA_VK_TOWER=0 or set_vulkan_tower(false))"
let VK_DECLINE_WORDS_DEVICE = "the tier has no device (DASLLAMA_GPU=1 arms it), or a class failed to build"
let VK_DECLINE_WORDS_MEMORY = "a device allocation failed"

//! Engage / decline counters; `blocks` = transformer blocks the driver ran. Model-level gates assert DELTAS on these.
def vulkan_tower_stats : tuple<encodes : int64; rows : int64; blocks : int64; declines : int64; convs : int64; mels : int64> =>
    (encodes = g_vt_encodes, rows = g_vt_rows, blocks = g_vt_blocks, declines = g_vt_declines.total, convs = g_vt_convs, mels = g_vt_mels)

//! The declines-by-reason table (a copy), keyed by `VulkanTowerDecline` value names.
def vulkan_tower_declines : table<string; int64> => <- clone_to_move(g_vt_declines.counts)

//! Test A/B seat: pin the tower driver on / off in-process (the env knob's setter twin).
def set_vulkan_tower(on : bool) {
    g_vt_env_tower = on
}

var private g_vt_env_audio_blocks = true

//! Test A/B seat: pin the whisper-class block hooks off while the conv stem still serves - the stem's device rows must land for the
//! CPU block loop (its `blocks` decline)
def set_vulkan_audio_blocks(on : bool) : bool {
    let was = g_vt_env_audio_blocks
    g_vt_env_audio_blocks = on
    return was
}

var private g_vt_env_tail = true
var private g_vt_tail_encodes = 0l

//! Test A/B seat: the chat projector tail behind the whisper-class blocks on the device (`vulkan_audio_tower_blocks_tail`); off,
//! the blocks still serve and the CPU tail reads their rows
def set_vulkan_tower_tail(on : bool) {
    g_vt_env_tail = on
}

def vulkan_tower_tail_knob : bool => g_vt_env_tail

//! the encodes whose projector tail the device ran (the soft tokens read back, the blocks' rows never)
def vulkan_tower_tail_encodes : int64 => g_vt_tail_encodes

var private g_vt_env_pk_front = true

//! Serve the parakeet subsample front on the device (`on`), or leave it to the CPU chain with the block seat serving behind it;
//! answers the setting it replaced.
def set_vulkan_parakeet_front(on : bool) : bool {
    let was = g_vt_env_pk_front
    g_vt_env_pk_front = on
    return was
}

def vulkan_tower_on : bool => g_vt_env_tower

//! The fronts' route: the audio fronts ride the f16 GEMM feed (cm2 or KHR); off it (the sdot4 mode) every front declines
//! `device` while the block chains still serve.
def vulkan_tower_front_route : bool => vt_cm2_feed()

[arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vulkan_tower_serves_exact() : bool => false

let private VT_STUB_BYTES = 64l   //! a stub seat's bytes: a family's unused binding, which the release frees like the others
let private VT_KEY_SAMPLE_WORDS = 64l        //! words sampled from each plane region folded into the residency key beside its address and size
let VT_NO_CLAMP = 3.4028235e38 //! an inactive ClippableLinear bound: the CPU chain skips the clamp, the device clamps to the float range
let private VT_MAX_ENCODE_ROWS = 4096l
let private VT_WIN_HS = 80l              //! the window stamp's head width (`TowerWinAttn` HS)

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
let private VT_CT_HID = 0x40000000u      //! the chat projector tail: the MLP's hidden rows, the gated rows
let private VT_CT_GATE = 0x80000000u
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

//! a K-quant member's device planes on the kq lane: its seats' rows gathered in the tier's layout, in layout order
struct private VtKqPlane {
    q_dev : uint64
    s_dev : uint64
    q_bytes : int64
    s_bytes : int64
}

//! a GEMM record's seat on the kq lane: a Q8_0 seat's block base in the q8 planes, a K-quant seat's superblock base in its member's planes
struct private VtFcSeat {
    fmt : int
    wblk0 : int64
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
    f32w : bool           //! the exact lane (a vision family's f32 planes): the block GEMMs on the f32 tile over the norms plane's rows, no feed
    xc_dev : uint64       //! the exact lane's clamped GEMM input, [cap x d] (gemma4v: the input bounds applied on the way in, xb kept whole)
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
    win_cap : int64       //! the window table's capacity (windows)
    host : HostBuf        //! the x readback staging (the tap stashes behind it)
    phost : HostBuf       //! the projector's readback staging, `phost_bytes` long (0: none)
    phost_bytes : int64
    oom : bool            //! a soft allocation of the batch under way failed
    bound_d : int         //! the tile key (`vt_tile_key`) the bound d-wide GEMM sets were built for (-1 = none)
    bound_ff : int        //! the tile key of the bound ff-wide sets (gate, up): the tile pick reads the output width
    bound_qkv : int       //! the tile key of the bound fused qkv set (3d wide)
    @scratch sets_d : table<int; VtBatchSetsD>     //! the GEMM sets per tile key: built once at the scratch's capacity, rebound on a tile switch
    @scratch sets_ff : table<int; VtBatchSetsFf>
    @scratch sets_qkv : table<int; VkDescriptorSet>
    @scratch sets_rel_proj : table<int; VkDescriptorSet>   //! the rel-table projection's set per tile key (its feed the table's own rows)
    @scratch kq_planes : table<int; VtKqPlane>   //! the kq lane's device planes a K-quant member, keyed by its format slot (`vt_upload_fc_kq`)
    @scratch kq_seats : array<VtFcSeat>          //! the kq lane's seat a GEMM record: the member's slot and the record's block base in its planes
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
    s_cc_xb : VkDescriptorSet      //! the exact lane's sets: the clamp copy xb -> xc, the in-place clamp of att (hg's is the q8 lane's own), the f32 tile over the blob rows a site
    s_clamp_att : VkDescriptorSet
    s32_q : VkDescriptorSet
    s32_k : VkDescriptorSet
    s32_v : VkDescriptorSet
    s32_qkv : VkDescriptorSet
    s32_o : VkDescriptorSet
    s32_hg : VkDescriptorSet
    s32_hu : VkDescriptorSet
    s32_down : VkDescriptorSet
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
    s_rq_x : VkDescriptorSet   //! the residual rows' feed: a projector behind the blocks reads them
    s_rq_rel_tab : VkDescriptorSet
    s_clamp_hg : VkDescriptorSet
    s_silu_hg : VkDescriptorSet
    s_silu_yd : VkDescriptorSet
    s_glu_sig : VkDescriptorSet
    s_dw : VkDescriptorSet
    s_attn : VkDescriptorSet
    s_ln_x : VkDescriptorSet
    s_cn_dw : VkDescriptorSet
    s_pk_dw : VkDescriptorSet           //! the tap-major twin (parakeet)
    s_post_o_ln_nb : VkDescriptorSet    //! the seams of a block with no GEMM biases (parakeet)
    s_post_d_ln_nb : VkDescriptorSet
    s_ga_dw : VkDescriptorSet           //! the rotary families' biased tap-major depthwise conv (gigaam), its LayerNorm in place over the conv rows
    s_ga_dwh : VkDescriptorSet          //! ... over the family's f16 taps in `taps_dev`
    taps_dev : uint64                   //! the depthwise taps as the family's own halfwords (gigaam's f16 taps; 0 bytes = the taps are f32 in `norms_dev`)
    taps_bytes : int64
    s_ln_yd : VkDescriptorSet
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
    vis_w_fresh : bool        //! the f32 GEMM rows' buffer (`vis_w_dev`) was (re)built: the family uploads its slab again
    tail_out_dev : uint64
    tail_proj_dev : uint64
    tail_host : HostBuf
    s_feed_x : VkDescriptorSet
    s_feed_pt : VkDescriptorSet
    s_bias_pt : VkDescriptorSet
    s_rms_pt : VkDescriptorSet
    vis_cap : int64           //! the vision stem's scratch capacity (rows; 0 = none): the image planes and the patch im2col rows
    vis_pd : int64            //! the im2col row stride it was built for
    vis_planes_bytes : int64
    vis_col_bytes : int64
    vis_planes_dev : uint64
    vis_col_dev : uint64
    vis_w_dev : uint64        //! the stem's and tail's f32 GEMM rows the blob does not hold as f32 (the bf16 planes converted)
    vis_w_bytes : int64
    vis_idx_dev : uint64      //! a stem's row map (qwen25v's window sort), uint32 a row
    vis_idx_bytes : int64
    vis_aux_dev : uint64      //! a stem's per-encode f32 rows (qwen3v's positions resized off the learned grid)
    vis_aux_bytes : int64
    vis_gen : int64           //! bumped on every vision scratch rebuild: the family's sets are built again when it moves
    vis_sets_gen : int64
    s_vis_im2col : VkDescriptorSet
    s_vis_patch : VkDescriptorSet   //! the patch conv (with its bias on gemma3v), into x
    s_vis_pos : VkDescriptorSet     //! the learned positions: one bias row the width of the plane, or the 2-axis add
    s_vis_pool : VkDescriptorSet    //! the tail's first row pass: the grid pool (gemma3v, gemma4v), the layernorm ahead of a merger (qwen3v's taps, gemma4uv's patch rows)
    s_vis_affine : VkDescriptorSet
    s_vis_clamp_in : VkDescriptorSet
    s_vis_clamp_out : VkDescriptorSet
    s_vis_proj : VkDescriptorSet
    s_vis_pos_rs : VkDescriptorSet    //! qwen3v: the assemble reading the resized positions
    s_vis_gemm : VkDescriptorSet[3]    //! qwen3v: the tap merger's two GEMMs, the tail merger's first
    s_vis_act : VkDescriptorSet[2]     //! qwen3v: the mergers' GELU over the tap plane and the tail plane
    ct_cap : int64            //! the chat projector tail's row capacity (the whisper-class chat towers; 0 = none) and its plane widths
    ct_hid : int64
    ct_gate : int64
    ct_out : int64
    ct_hid_dev : uint64
    ct_gate_dev : uint64
    ct_out_dev : uint64
    ct_host : HostBuf
    s_ct_pool : VkDescriptorSet
    s_ct_ln : VkDescriptorSet
    s_ct_ln16 : VkDescriptorSet
    s_ct_rms_xb : VkDescriptorSet
    s_ct_rms_gate : VkDescriptorSet
    s_ct_feed_xb : VkDescriptorSet
    s_ct_feed_hid : VkDescriptorSet
    s_ct_feed_gate : VkDescriptorSet
    s_ct_swiglu : VkDescriptorSet
    s_ct_gelu : VkDescriptorSet
    s_ct_bias_out : VkDescriptorSet
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
    gemm_khr : bool       //! the f16 GEMM arm the f16 sets were built for (qwen25v, gigaam's exact lane)
    fc16 : bool           //! the FastConformer chain's GEMM sets are the exact lane's f16 GEMM over the halfword plane (`vt_sets_fc16`)
}

var private g_vt_g4v : VtResident   //! the gemma4v family's resident tower (one a family: a second mmproj replaces it)
var private g_vt_g3v : VtResident
var private g_vt_uv : VtResident    //! the gemma4uv embedder: no blocks, the stem's three layernorms and two GEMMs over the slab
var private g_vt_q3v : VtResident
var private g_vt_q25v : VtResident
var private g_vt_aud : VtResident   //! the whisper-class audio towers' resident (one seat: a second tower replaces it)

var private g_vt_g4a : VtResident
var private g_vt_cn : VtResident
var private g_vt_pk : VtResident   //! parakeet's resident: the FastConformer chain over its q8 planes, its front beside
var private g_vt_ga : VtResident   //! gigaam's resident: the rotary FastConformer chain over its q8 planes, its conv front beside
var private @scratch @exact_size g_vt_fc_offs : array<CanaryLayerOffs>   //! the FastConformer encode's block offsets, the family's record mapped onto canary's
var private @scratch @exact_size g_vt_fc_gseats : array<TowerGemmSeat>   //! the kq lane's seat a GEMM record (`VT_CN_GEMMS` a block, the rel record empty): the family's `tower_gemm_seat` over its offsets
var private g_vt_no_sbits : array<uint>   //! the kq gather's empty site-scale words: no member of the lane carries one
var private g_vt_q3a_front : VtResident   //! the qwen3a conv front's resident: the conv_out q8 plane, the f16 conv slab, the chunk scratch
var private @scratch @exact_size g_vt_rope2d_tabs : array<float>   //! the packed rope tables staging, [row][cx | sx | cy | sy]
var private @scratch @exact_size g_vt_maps : array<uint>           //! the encode's schedule map offsets, one a GEMM record
var private @scratch @exact_size g_vt_wlo : array<uint>            //! the window table staging (nwin + 1)
var private @scratch @exact_size g_vt_idx : array<uint>            //! the window-sort row map staging, one encode's rows
var private @scratch @exact_size g_vt_ln_offs : array<LayerOffs>       //! the pre-LN chain's block offsets, one record a block

[cold_path]
def private vt_decline_words(why : VulkanTowerDecline) : string {
    if (why == VulkanTowerDecline.knob) {
        return VK_DECLINE_WORDS_TOWER_KNOB
    } elif (why == VulkanTowerDecline.ga_front) {
        return "the GigaAM front lever is off (set_vulkan_gigaam_front(false)); the block seat serves behind the CPU front"
    } elif (why == VulkanTowerDecline.blocks) {
        return "the whisper-class block hooks are pinned off (set_vulkan_audio_blocks(false)); the stem still serves"
    } elif (why == VulkanTowerDecline.quant_mode) {
        return "the tower's planes are not the lane the driver serves"
    } elif (why == VulkanTowerDecline.kq_lane) {
        return "a K-quant member of the carrier's own block GEMM planes has no tile on this device's f16 feed - the CPU kq lane serves them"
    } elif (why == VulkanTowerDecline.shape) {
        return "a width, head size, row count, window or buffer range the stamps do not serve"
    } elif (why == VulkanTowerDecline.memory) {
        return VK_DECLINE_WORDS_MEMORY
    }
    return VK_DECLINE_WORDS_DEVICE
}

//! the decline counted under the enum's value name, and said once per reason per tower in words (the counter's `note`)
[cold_path]
def private vt_decline(r : VtResident; why : VulkanTowerDecline) : bool
    => g_vt_declines |> note("the {r.family} tower", "{why}", "the tower stays on the CPU chain: {vt_decline_words(why)}")

//! the kq-lane decline - "a reason naming the site and the member" (REVIEW_KQ_FORMATS.md)
[cold_path]
def private vt_decline_kq(r : VtResident; members : string) : bool
    => g_vt_declines |> note("the {r.family} tower", "{VulkanTowerDecline.kq_lane}", "the tower stays on the CPU chain: the GigaAM block GEMMs ({members}) - {vt_decline_words(VulkanTowerDecline.kq_lane)}")

//! a scratch or upload builder's outcome: built, or the reason the seat declines
enum private VtBuild {
    built
    shape    //! a buffer past the device's storage-buffer range
    memory   //! a device or pinned allocation failed
}

//! true where `b` built; else its decline counted
def private vt_built(r : VtResident; b : VtBuild) : bool {
    return true if (b == VtBuild.built)
    return vt_decline(r, b == VtBuild.shape ? VulkanTowerDecline.shape : VulkanTowerDecline.memory)
}

//! a hook's first gate: the tower knob on and the tower's planes the lane the driver serves (`served_lane`); false with the decline counted
def private vt_gate(r : VtResident; served_lane : bool) : bool {
    return vt_decline(r, VulkanTowerDecline.knob) if (!g_vt_env_tower)
    return served_lane || vt_decline(r, VulkanTowerDecline.quant_mode)
}

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

let private VT_TILE_FMT_KEY_STRIDE = 4096   //! a tile key's format band: the kq lane's K-quant tiles key beside the q8 ones
def private vt_tile_key(t : VtTile) : int => (t.tc > 0l ? VT_TILE_CM2_KEY_BASE + int(t.tc) : t.v) + t.fmt * VT_TILE_FMT_KEY_STRIDE

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

//! A model's Vulkan device block as an upload reads it (`#tower-dev-block`): `on` where the model is the Vulkan-flavored image
struct private VtDevSrc {
    on : bool
    dev : PlaneU8 = PlaneU8()
    devplan : PlaneI64 = PlaneI64()
}

def private vt_dev_src(m) : VtDevSrc => VtDevSrc(on = m.dev_backend == TowerDev.vulkan, dev = m.dev, devplan = m.devplan)

//! the resident key of a Vulkan-flavored model: its block - address, size, plan, sampled words - the rows of `blob` uploaded
//! beside it and the layer count, every field the resident's extent follows
def private vt_key_dev(src : VtDevSrc; blob : PlaneF; rows_off, rows_n, n_layer : int64) : uint64 {
    var k = vt_fold_int(vt_fold_addr(vt_fold_int(0x6b0dul, src.dev.n), unsafe(reinterpret<void?>(src.dev.p))), n_layer)
    k = vt_fold_words(k, unsafe(reinterpret<uint8 const?>(src.devplan.p)), src.devplan.n * 8l)
    k = vt_fold_words(k, src.dev.p, src.dev.n)
    k = vt_fold_addr(k, unsafe(reinterpret<void?>(blob.p)))
    k = vt_fold_int(k, rows_off)
    k = vt_fold_int(k, rows_n)
    return vt_fold_words(k, unsafe(reinterpret<uint8 const?>(plane_at(blob, rows_off))), rows_n * 4l)
}

//! the q8 regions a chain gathers, as a Vulkan device block carries them: the gather at the row-major layout into `<prefix>.wq` /
//! `<prefix>.ws`
[cold_path]
def private vt_bake_q8(var b : TowerDevBuild; prefix : string; qblob : PlaneI8; qscales : PlaneF; regions : array<Q8Region>) {
    var wq, ws : array<uint8>
    wq |> reserve_resize(qblob.n)
    ws |> reserve_resize(qscales.n * 2l)
    vt_gather_regions(qblob, qscales, regions, active_q8_repack_layout(false), wq, ws)
    tower_dev_put(b, "{prefix}.wq", unsafe(addr<uint8 const?>(wq[0])), long_length(wq))
    tower_dev_put(b, "{prefix}.ws", unsafe(addr<uint8 const?>(ws[0])), long_length(ws))
    delete wq
    delete ws
}

//! a block piece's bytes - 0 where the block holds none
def private vt_piece_bytes(src : VtDevSrc; name : string) : int64 => max(tower_dev_find(src.devplan, name).bytes, 0l)

//! the piece `name` uploaded as stored into `buf`
def private vt_put_piece(src : VtDevSrc; name : string; buf : uint64) {
    let p = tower_dev_find(src.devplan, name)
    upload_region_at(buf, 0l, plane_at(src.dev, p.off), p.bytes) if (p.off >= 0l && p.bytes > 0l)
}

//! the soft allocations: a failure marks the resident, and the batch's caller releases what it got and declines `memory`
[cold_path]
def private vt_dev_buf(var r : VtResident; bytes : int64) : uint64 => soft_device_buf(r.oom, bytes)

//! `vt_dev_buf`'s per-call form: a slot a call refills (input rows, mel, rope tables, state)
[cold_path]
def private vt_call_buf(var r : VtResident; bytes : int64) : uint64 => soft_call_buf(r.oom, bytes)

[cold_path]
def private vt_host_buf(var r : VtResident; bytes : int64; cached : bool) : HostBuf => soft_host_buf(r.oom, bytes, cached)

[arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vt_cap_rows(rows : int64) : int64 => round_up(rows, 256l)

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
def vt_cm2_feed : bool => g_gpu != null && g_gpu.coopmat_mode >= COOPMAT_MM   // no device = no feed: the scratch sizing asks before the arm

//! the front scratch released on its own; the block chain's scratch rebuild drops it too, and it regrows on demand
[cold_path]
def private vt_front_release(var r : VtResident) {
    if (r.front_cap > 0l) {
        release_device_buf(r.fxin_dev)
        release_device_buf(r.fcol_dev)
        release_device_buf(r.fy1_dev)
        release_device_buf(r.fy2_dev)
        release_device_buf(r.fy3_dev)
        release_device_buf(r.fmel_dev)
        release_device_buf(r.ffeat_dev)
        release_device_buf(r.fx_dev)
        release_device_buf(r.fcn_pw3_dev)
        release_device_buf(r.fcn_dw5_dev)
        release_device_buf(r.fcn_feat_dev)
        release_device_buf(r.fcn_feed16_dev)
        release_host_buf(r.fhost)
        release_host_buf(r.fmeta)
        r.fmeta_bytes = 0l
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
    release_device_buf(r.x_dev)
    release_device_buf(r.xb_dev)
    release_device_buf(r.xq_dev)
    release_device_buf(r.xs_dev)
    release_device_buf(r.q_dev)
    release_device_buf(r.k_dev)
    release_device_buf(r.v_dev)
    release_device_buf(r.q2_dev)
    release_device_buf(r.k2_dev)
    release_device_buf(r.v2_dev)
    release_device_buf(r.k16_dev)
    release_device_buf(r.v16_dev)
    release_device_buf(r.att_dev)
    release_device_buf(r.yo_dev)
    release_device_buf(r.hg_dev)
    release_device_buf(r.hu_dev)
    release_device_buf(r.yd_dev)
    release_device_buf(r.tabs_dev)
    if (r.pad_dp > 0l) {
        release_device_buf(r.qp_dev)
        release_device_buf(r.kp16_dev)
        release_device_buf(r.vp16_dev)
        release_device_buf(r.attp_dev)
    }
    if (r.fused) {
        release_device_buf(r.qkv_dev)
        release_device_buf(r.cos_dev)
        release_device_buf(r.sin_dev)
    }
    if (r.taps > 0l) {
        release_device_buf(r.xtap_dev)
    }
    if (r.f16w) {
        release_device_buf(r.xh_dev)
        release_device_buf(r.wlo_dev)
    }
    if (r.f32w) {
        release_device_buf(r.xc_dev)
    }
    if (r.cm2) {
        release_device_buf(r.xh_dev)
    }
    vt_sk_release(r)
    vt_front_release(r)
    if (r.cn_plane_rows > 0l) {
        release_device_buf(r.cn_qh_dev)
        release_device_buf(r.cn_ph_dev)
        release_device_buf(r.cn_r_dev)
    }
    vt_tail_release(r)
    vt_vis_release(r)
    vt_ct_release(r)
    release_host_buf(r.host)
    if (r.phost_bytes > 0l) {
        release_host_buf(r.phost)
        r.phost_bytes = 0l
    }
    if (r.meta_bytes > 0l) {
        release_host_buf(r.meta)
    }
    vt_forget_scratch(r)
}

//! the scratch's marks cleared, nothing destroyed: the release calls it after its destroys, the forget path (the device's sweep freed
//! the objects) alone; the stem's pending rows and the encoder-output handoff go with the planes they name
[cold_path]
def private vt_forget_scratch(var r : VtResident) {
    vt_x_drop_pending(r)
    g_vt_aud_out_bytes = 0l
    g_vt_fc_out_bytes = 0l
    r.cap_rows = 0l
    r.pad_dp = 0l
    r.fused = false
    r.taps = 0l
    r.f16w = false
    r.f32w = false
    r.fc16 = false
    r.win_cap = 0l
    r.meta_bytes = 0l
    r.front_cap = 0l
    r.x_ready = false
    r.cm2 = false
    r.sk_bytes = 0l
    r.tail_cap = 0l
    r.tail_dim = 0l
    if (r.vis_cap > 0l) {
        vt_vis_forget(r)
    }
    r.ct_cap = 0l
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
        release_device_buf(r.wq_dev)
        release_device_buf(r.ws_dev)
        release_device_buf(r.norms_dev)
        for (kp in values(r.kq_planes)) {
            release_device_buf(kp.q_dev)
            release_device_buf(kp.s_dev)
        }
        r.kq_planes |> clear()
        if (r.taps_bytes > 0l) {
            release_device_buf(r.taps_dev)
            r.taps_bytes = 0l
        }
        vt_rel_release(r)
        if (r.front) {
            release_device_buf(r.fw16_dev)
            release_device_buf(r.front_twiddles_dev)
            release_device_buf(r.front_filters_dev)
            release_device_buf(r.fnorm_dev)
            r.front = false
        }
        r.key = 0ul
    }
}

//! Release every device object the driver owns (the tests' leak rail beside the tier's shutdown); the next served encode
//! uploads again.
def vulkan_tower_shutdown {
    vt_each_resident() $(var r : VtResident; _family : string) {
        vt_release(r)
    }
}

//! every resident the driver keeps, with the family name its log lines and declines say
def private vt_each_resident(blk : block<(var r : VtResident; family : string) : void>) {
    invoke(blk, g_vt_g4v, "gemma4v")
    invoke(blk, g_vt_pk, "parakeet")
    invoke(blk, g_vt_ga, "gigaam")
    invoke(blk, g_vt_g3v, "gemma3v")
    invoke(blk, g_vt_uv, "gemma4uv")
    invoke(blk, g_vt_q3v, "qwen3v")
    invoke(blk, g_vt_q25v, "qwen25v")
    invoke(blk, g_vt_aud, "whisper-class")
    invoke(blk, g_vt_g4a, "gemma4a")
    invoke(blk, g_vt_cn, "canary")
    invoke(blk, g_vt_q3a_front, "qwen3a-front")
    invoke(blk, g_vt_q3am, "qwen3a-mel")
    invoke(blk, g_vt_vad, "silero-vad")
}

[cold_path]
def private vt_forget_one(var r : VtResident) {
    vt_forget_scratch(r)
    r.key = 0ul
    r.taps_dev = 0ul
    r.taps_bytes = 0l
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
        release_device_buf(r.rel_tab_dev)
        release_device_buf(r.rel_tabq_dev)
        release_device_buf(r.rel_tabs_dev)
        release_device_buf(r.rel_tabh_dev)
        release_device_buf(r.rel_proj_dev)
        r.rel_cap_rows = 0l
    }
    r.sets_rel_proj |> clear()
}

[arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vt_rel_proj_set(var r : VtResident; t : VtTile) : VkDescriptorSet {
    return cached(r.sets_rel_proj, vt_tile_key(t)) $() : VkDescriptorSet {
        return vt_rel_proj_build(r, t)
    }
}

[cold_path]
def private vt_rel_proj_build(r : VtResident; t : VtTile) : VkDescriptorSet {
    let b_relq = r.rel_cap_rows * r.d
    return vt_tile_cls_set(t, r.wq_dev, r.ws_dev, r.wq_bytes, r.ws_bytes, r.meta.buf, r.meta_bytes, r.rel_tabh_dev, (r.rel_cap_rows + TILE_READ_SLACK) * r.d * 2l,
        r.rel_tabq_dev, r.rel_tabs_dev, b_relq, VT_REL_TABQ, r.rel_proj_dev, b_relq * 4l, VT_REL_PROJ)
}

def private vt_conformer_sets(var r : VtResident; v, vff, v2d, v_rel : VtTile) : tuple<s_h2 : VkDescriptorSet; s_rel : VkDescriptorSet> {
    vt_bind_tile_sets(r, v, vff)
    return (s_h2 = vt_sets_ff(r, v2d).s_hg, s_rel = vt_rel_proj_set(r, v_rel))
}

[arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vt_forget {
    vt_each_resident() $(var r : VtResident; _family : string) {
        vt_forget_one(r)
    }
    g_vt_ln_offs |> clear()
    g_vt_fc_offs |> clear()
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
    r.ws_dev = vt_dev_buf(r, VT_STUB_BYTES)   // no scale plane on this family: a stub the release frees like the others
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

//! the norms plane's layout: the blob's `rows_n` rows, the `w_n` widened halfwords behind them, then the ones row; the q8 seats are stubs
[cold_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_VISION.md#vk-vision-chains")]
def private vt_upload_f32(var r : VtResident; key : uint64; blob : PlaneF; rows_n : int64; wblob : PlaneU16; w_n, ones_n : int64) : bool {
    vt_upload_begin(r, key)
    r.wq_bytes = 0l
    r.ws_bytes = 0l
    r.wq_dev = vt_dev_buf(r, VT_STUB_BYTES)
    r.ws_dev = vt_dev_buf(r, VT_STUB_BYTES)
    r.ones_off = rows_n + w_n
    r.norms_bytes = (rows_n + w_n + ones_n) * 4l
    r.norms_dev = vt_dev_buf(r, r.norms_bytes)
    if (r.oom) {
        vt_release(r)
        return false
    }
    if (w_n > 0l) {
        var inscope wide <- vt_rows_f32(blob, wblob, true, 0l, w_n)
        upload_float_ranges(r.norms_dev, [(p = plane_at(blob, 0l), n = rows_n), (p = unsafe(addr<float const?>(wide[0])), n = w_n)], ones_n)
    } else {
        upload_float_ranges(r.norms_dev, [(p = plane_at(blob, 0l), n = rows_n)], ones_n)
    }
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
    r.wlo_dev = vt_call_buf(r, (nwin + 1l) * 4l)
    r.xh_dev = vt_dev_buf(r, cap * max(d, ff) * 2l)
    r.qkv_dev = vt_dev_buf(r, VT_STUB_BYTES)   // unused seats of the fused set, stubs the release frees alike
    r.cos_dev = vt_call_buf(r, cap * (hs / 2l) * 4l)
    r.sin_dev = vt_call_buf(r, cap * (hs / 2l) * 4l)
    r.x_dev = vt_call_buf(r, bx)
    r.xb_dev = vt_call_buf(r, bx)
    r.xq_dev = vt_call_buf(r, VT_STUB_BYTES)
    r.xs_dev = vt_dev_buf(r, VT_STUB_BYTES)
    r.q_dev = vt_dev_buf(r, bx)
    r.k_dev = vt_call_buf(r, bx)
    r.v_dev = vt_dev_buf(r, bx)
    r.q2_dev = vt_dev_buf(r, VT_STUB_BYTES)
    r.k2_dev = vt_dev_buf(r, VT_STUB_BYTES)
    r.v2_dev = vt_dev_buf(r, VT_STUB_BYTES)
    r.k16_dev = vt_dev_buf(r, bh)
    r.v16_dev = vt_dev_buf(r, bh)
    r.att_dev = vt_dev_buf(r, bx)
    r.yo_dev = vt_dev_buf(r, bx)
    r.hg_dev = vt_dev_buf(r, bff)
    r.hu_dev = vt_dev_buf(r, bff)
    r.yd_dev = vt_dev_buf(r, bx)
    r.tabs_dev = vt_call_buf(r, VT_STUB_BYTES)
    r.host = vt_host_buf(r, bx, true)
    r.meta_bytes = 0l
    if (r.oom) {
        vt_release_scratch(r)
        return false
    }
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
    vt_sets_fa_unpad(r)
    r.s_win = set_tower_win_attn_cls(fixed_array(r.q_dev, r.k_dev, r.v_dev, r.att_dev, r.wlo_dev), fixed_array(bx, bx, bx, bx, bwl), fixed_array(VT_Q, VT_K, VT_V, VT_ATT, 0u))
    r.s_post_o_ln = set_tower_post_add_rms_cls(fixed_array(r.x_dev, r.yo_dev, r.xb_dev, r.norms_dev), fixed_array(bx, bx, bx, r.norms_bytes), fixed_array(VT_X, VT_YO, VT_XB, 0u))
    r.s_post_d_ln = set_tower_post_add_rms_cls(fixed_array(r.x_dev, r.yd_dev, r.xb_dev, r.norms_dev), fixed_array(bx, bx, bx, r.norms_bytes), fixed_array(VT_X, VT_YD, VT_XB, 0u))
    r.s_glu = set_tower_bias_gate16_cls(fixed_array(r.hg_dev, r.hu_dev, r.norms_dev, r.xh_dev), fixed_array(bff, bff, r.norms_bytes, bxh),
        fixed_array(VT_HG, VT_HU, 0u, VT_XQ))
}

def private vt_lo(c : Clamp; input : bool) : float => c.active ? (input ? c.imin : c.omin) : -VT_NO_CLAMP
def private vt_hi(c : Clamp; input : bool) : float => c.active ? (input ? c.imax : c.omax) : VT_NO_CLAMP

//! the q8 planes' regions gathered into the device layout (`wq` the quants, `ws` the halfword scales)
[cold_path]
def vt_gather_regions(qblob : PlaneI8; qscales : PlaneF; regions : array<Q8Region>; lay : Q8RepackLayout; var wq, ws : array<uint8>) {
    let q8e = kq_elems(KqFmt.q8)
    for (reg in regions) {
        q8_gather_rows(plane_at(qblob, reg.woff), null, plane_at(qscales, reg.woff / q8e), reg.n, reg.rows, q8_gather_slice(reg.rows, lay.mr), lay.mr, lay.kgroup, lay.wbias,
            wq, ws, reg.woff, reg.woff / q8e)
    }
}

//! one GEMM record of a chain's schedule: its weight block, its rows and its workgroup groups
struct VtSchedRec {
    wblk0 : int64
    rows : int64
    groups : int64
}

//! a chain's batch schedule written into `meta`: `nrec` records at its head and their maps after them, record `rid`'s
//! weight block, rows and groups answered by `rec` in record order; `maps` takes each record's map offset
def vt_sched_walk(@scratch var maps : array<uint>; meta : HostBuf; nrec : int64; rec : block<(rid : int64) : VtSchedRec>) {
    maps |> reserve(nrec)
    maps |> resize(nrec)
    var mp = unsafe(reinterpret<uint?>(meta.mapped))
    var cur = nrec * 4l
    for (rid in range64(nrec)) {
        let rr = invoke(rec, rid)
        maps[rid] = batch_sched_map(mp, cur, rid, rr.wblk0, rr.rows, rr.groups)
    }
}

let Q8_SITE_META_BYTES = 64l   //! a q8 GEMV site's region pairs

//! a resident q8 driver's GEMV feed: the gathered planes (`wq`, `ws` and their bytes) and the requant x's quants and scales
//! (`xq`, `xs`, sized for `x_elems` quants, bound at `xbit`)
struct Q8GemvFeed {
    planes : tuple<wq : uint64; ws : uint64; wqb : int64; wsb : int64>
    xq : uint64
    xs : uint64
    x_elems : int64
    xbit : uint
}

//! a q8 GEMV site's region pairs: each region's first weight block at its element offset in `woffs`, its activation rows at
//! block 0; a failed host buffer marks `oom`
[cold_path]
def q8_site_meta(var oom : bool&; woffs : array<int64>) : HostBuf {
    let q8e = kq_elems(KqFmt.q8)   // nolint:LINT021 - the element offset divides in 64-bit ahead of the narrowing store
    let hb = soft_host_buf(oom, Q8_SITE_META_BYTES, false)
    if (hb.buf != 0ul) {
        for (i in range(length(woffs))) {
            fill_gemv_region(hb, i, uint(woffs[i] / q8e))
        }
    }
    return hb
}

//! a q8 GEMV site's set over the feed `f`, its region pairs `meta` and the output `y` (`y_bytes`, bound at `y_bit`)
[cold_path]
def q8_gemv_set(f : Q8GemvFeed; meta : uint64; y : uint64; y_bytes : int64; y_bit : uint) : VkDescriptorSet {
    return gemv_planes_set(int(KqFmt.q8), f.planes, meta, Q8_SITE_META_BYTES, 0u, f.xq, f.xs, f.x_elems, f.x_elems / kq_elems(KqFmt.q8) * 4l,
        f.xbit, y, y_bytes, y_bit)
}

//! a q8 site's GEMV over `ncols` rows of `n_in` into `nreg` regions of `d_out` outputs a row
def q8_gemv(raw : VkCommandBuffer; var h : VkHaz; var site_set : VkDescriptorSet; n_in, d_out, nreg, ncols : int64) {
    var pc = GemvArgs(n = uint(n_in), d = uint(d_out), nreg = uint(nreg), ybase = 0u, lanes_per_row = 0u, act = 0u, bias_on = 0u, gboff = 0u,
        ncols = uint(ncols), ystride = uint(nreg * d_out))
    gemv_enc(int(KqFmt.q8), raw, h, site_set, pc, nreg * d_out)
}

//! the q8 planes' regions gathered into the device layout and uploaded, `wq_bytes` of quants to `wq_dev` and `ws_bytes` of
//! halfword scales to `ws_dev`
[cold_path]
def vt_upload_regions(qblob : PlaneI8; qscales : PlaneF; regions : array<Q8Region>; lay : Q8RepackLayout; wq_dev, ws_dev : uint64; wq_bytes, ws_bytes : int64) {
    var wq, ws : array<uint8>
    wq |> reserve_resize(wq_bytes)
    ws |> reserve_resize(ws_bytes)
    vt_gather_regions(qblob, qscales, regions, lay, wq, ws)
    unsafe {
        upload_region_at(wq_dev, 0l, addr<uint8 const?>(wq[0]), wq_bytes)
        upload_region_at(ws_dev, 0l, addr<uint8 const?>(ws[0]), ws_bytes)
    }
    delete wq
    delete ws
}

[cold_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vt_upload(var r : VtResident; key : uint64; qblob : PlaneI8; qscales : PlaneF; regions : array<Q8Region>;
                      blob : PlaneF; rows_off, rows_n, ones_n : int64; lay : Q8RepackLayout; src : VtDevSrc = VtDevSrc(); prefix : string = "") : bool {
    vt_upload_begin(r, key)
    r.wq_bytes = src.on ? vt_piece_bytes(src, "{prefix}.wq") : qblob.n
    r.ws_bytes = src.on ? vt_piece_bytes(src, "{prefix}.ws") : qscales.n * 2l
    r.wq_dev = vt_dev_buf(r, r.wq_bytes)
    r.ws_dev = vt_dev_buf(r, r.ws_bytes)
    r.ones_off = rows_n
    r.norms_bytes = (rows_n + ones_n) * 4l
    r.norms_dev = vt_dev_buf(r, r.norms_bytes)
    if (r.oom) {
        vt_release(r)
        return false
    }
    if (src.on) {
        vt_put_piece(src, "{prefix}.wq", r.wq_dev)
        vt_put_piece(src, "{prefix}.ws", r.ws_dev)
    } else {
        vt_upload_regions(qblob, qscales, regions, lay, r.wq_dev, r.ws_dev, r.wq_bytes, r.ws_bytes)
    }
    upload_float_ranges(r.norms_dev, [(p = plane_at(blob, rows_off), n = rows_n)], ones_n)
    r.cap_rows = 0l
    vt_unbind_batch_sets(r)
    return true
}

def private vt_scratch_fits(r : VtResident; rows, d, ff, dp, meta_bytes : int64; khr, fused : bool; taps : int64; f32w : bool = false) : bool
    => (r.cap_rows >= vt_cap_rows(rows) && r.d == d && r.ff == ff && r.pad_dp == dp && r.meta_bytes >= meta_bytes && r.fused == fused
        && r.taps == taps && r.fa_khr == khr && r.f32w == f32w)

//! `dp` > 0 adds the padded panels, `meta_bytes` sizes the schedule buffer, `fused` adds the [q | k | v] rows and the rope tables, `taps` the stashes; false when an allocation failed
[cold_path]
def private vt_scratch_build(var r : VtResident; rows, d, ff, hs, dp, meta_bytes : int64; khr, fused : bool; taps : int64; f32w : bool = false) : bool {
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
    r.f32w = f32w
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
        r.cos_dev = vt_call_buf(r, cap * (hs / 2l) * 4l)
        r.sin_dev = vt_call_buf(r, cap * (hs / 2l) * 4l)
    }
    r.taps = taps
    if (taps > 0l) {
        r.xtap_dev = vt_dev_buf(r, taps * bx)
    }
    if (f32w) {
        r.xc_dev = vt_dev_buf(r, bx)
    }
    r.x_dev = vt_call_buf(r, bx)
    r.xb_dev = vt_call_buf(r, bx)
    r.xq_dev = vt_call_buf(r, bq)
    r.xs_dev = vt_dev_buf(r, bs)
    r.q_dev = vt_dev_buf(r, bx)
    r.k_dev = vt_call_buf(r, bx)
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
    r.tabs_dev = vt_call_buf(r, btabs)
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
def private vt_g4a_regions(lo : G4aLayer; d, ff : int64; var out : Q8Region[int(VT_G4A_GEMMS)]) {
    out[VT_G4A_FF_UP] = Q8Region(woff = lo.ff_up, n = d, rows = ff)
    out[VT_G4A_FF_DOWN] = Q8Region(woff = lo.ff_down, n = ff, rows = d)
    out[VT_G4A_Q] = Q8Region(woff = lo.q, n = d, rows = d)
    out[VT_G4A_K] = Q8Region(woff = lo.k, n = d, rows = d)
    out[VT_G4A_V] = Q8Region(woff = lo.v, n = d, rows = d)
    out[VT_G4A_O] = Q8Region(woff = lo.o, n = d, rows = d)
    out[VT_G4A_REL] = Q8Region(woff = lo.attn_k_rel, n = d, rows = d)
    out[VT_G4A_PW1] = Q8Region(woff = lo.conv_pw1, n = d, rows = 2l * d)
    out[VT_G4A_PW2] = Q8Region(woff = lo.conv_pw2, n = d, rows = d)
    out[VT_G4A_FF_UP_1] = Q8Region(woff = lo.ff_up_1, n = d, rows = ff)
    out[VT_G4A_FF_DOWN_1] = Q8Region(woff = lo.ff_down_1, n = ff, rows = d)
}

def private vt_g4a_key(t : Gemma4aEncoder; lay : Q8RepackLayout) : uint64 {
    let src = vt_dev_src(t)
    let key = (src.on ? vt_key_dev(src, t.blob, 0l, t.blob.n, t.n_layer)
        : vt_key(t.qblob, t.qscales, t.blob, 0l, t.blob.n, 0l, t.n_layer, lay.mr, lay.kgroup, lay.wbias))
    return vt_fold_words(key, unsafe(reinterpret<uint8 const?>(plane_at(t.pos_emb, 0l))), (gemma4a_attn_past() + 1l) * t.d_model * 4l)
}

//! the gemma4a resident's q8 planes, uploaded where its key moved; false where the upload will not take
def private vt_g4a_planes(var r : VtResident; t : Gemma4aEncoder) : bool {
    let lay = active_q8_repack_layout(g_active_needs_repack)
    let key = vt_g4a_key(t, lay)
    return r.key == key || vt_upload_g4a(r, key, t, lay)
}

[cold_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vt_upload_g4a(var r : VtResident; key : uint64; t : Gemma4aEncoder; lay : Q8RepackLayout) : bool {
    let d = t.d_model
    var inscope regions <- vt_g4a_all_regions(t)
    let ok = vt_upload(r, key, t.qblob, t.qscales, regions, t.blob, 0l, t.blob.n, t.proj_dim, lay, vt_dev_src(t), VT_G4A_PIECE)
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
        release_device_buf(r.tail_out_dev)
        release_device_buf(r.tail_proj_dev)
        release_host_buf(r.tail_host)
        r.tail_cap = 0l
    }
}

//! the feed set of a GEMM input plane: the f16 convert into the halves panel on a cm2 device, else the Q8_0 requant into the quant
//! and scale planes
[cold_path]
def private vt_feed_set(r : VtResident; src_dev : uint64; bytes : int64; bit : uint) : VkDescriptorSet {
    let cap = r.cap_rows
    let bq = cap * max(r.d, r.ff)
    if (r.cm2) {
        return set_tower_clamp_cvt_cls(fixed_array(src_dev, r.xh_dev), fixed_array(bytes, (cap + TILE_READ_SLACK) * max(r.d, r.ff) * 2l), fixed_array(bit, VT_XQ))
    }
    return set_tower_clamp_rq_cls(fixed_array(src_dev, r.xq_dev, r.xs_dev), fixed_array(bytes, bq, bq / kq_elems(KqFmt.q8) * 4l), fixed_array(bit, VT_XQ, VT_XQ))
}

[cold_path]
def private vt_tail_bufs(var r : VtResident; tail_dim : int64) : bool {
    vt_tail_release(r)
    let cap = r.cap_rows
    let bp = cap * tail_dim * 4l
    let bx = cap * r.d * 4l
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
    r.s_feed_x = vt_feed_set(r, r.x_dev, bx, VT_X)
    r.s_feed_pt = vt_feed_set(r, r.tail_out_dev, bp, VT_TAIL_OUT)
    r.s_bias_pt = set_tower_bias_act_cls(fixed_array(r.tail_out_dev, r.norms_dev), fixed_array(bp, r.norms_bytes), fixed_array(VT_TAIL_OUT, 0u))
    r.s_rms_pt = set_tower_rms_cls(fixed_array(r.tail_out_dev, r.tail_out_dev, r.norms_dev), fixed_array(bp, bp, r.norms_bytes), fixed_array(VT_TAIL_OUT, VT_TAIL_OUT, 0u))
    return true
}

//! the tail's GEMM set over its tile: the out-proj into the pt rows, the mm-proj into the po rows, built once per tile key
def private vt_tail_gemm_set(var r : VtResident; v : VtTile; mmproj : bool) : VkDescriptorSet {
    return cached(r.sets_front, (mmproj ? VT_G4A_MMPROJ_KEY : VT_G4A_OUTPROJ_KEY) + vt_tile_key(v)) $() : VkDescriptorSet {
        return vt_tile_set(r, v, mmproj ? r.tail_proj_dev : r.tail_out_dev, r.tail_cap * r.tail_dim * 4l, mmproj ? VT_TAIL_PROJ : VT_TAIL_OUT)
    }
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
    r.s_rq_x = set_tower_clamp_cvt_cls(fixed_array(r.x_dev, r.xh_dev), fixed_array(bx, bxh), fixed_array(VT_X, VT_XQ))
    if (r.rel_cap_rows > 0l) {
        r.s_rq_rel_tab = set_tower_clamp_cvt_cls(fixed_array(r.rel_tab_dev, r.rel_tabh_dev), fixed_array(b_rel, b_relh), fixed_array(VT_REL_TAB, VT_REL_TABQ))
    }
}

[arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vt_mm_clamped(raw : VkCommandBuffer; var h : VkHaz; c : Clamp; var s_rq : VkDescriptorSet; nin : int64; var s_gemm : VkDescriptorSet; v : VtTile;
                          n, dout : int64; map_off : uint; groups : int64; var s_cl : VkDescriptorSet; nout : int64) {
    var pc_rq = RqArgs(inbase = 0u, nblk = uint(nin / kq_elems(KqFmt.q8)), lo = vt_lo(c, true), hi = vt_hi(c, true))
    vt_feed_enc(raw, h, v.tc > 0l, s_rq, pc_rq, nin)
    var pc_g = BatchArgs(n = uint(n), d = uint(dout), map_off = map_off)
    vt_tile_enc(raw, h, v, s_gemm, pc_g, groups)
    if (c.active) {
        var pc_cl = GkClampArgs(nelem = uint(nout), lo = c.omin, hi = c.omax, scale = 1.0)
        enc_tower_clamp_cls(raw, h, s_cl, pc_cl, rq_wgs(false, nout))
    }
}

[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vulkan_gemma4a_blocks(t : Gemma4aEncoder; var s : Gemma4aState; npos : int64) : bool {   // nolint:STYLE038 — the dispatch chain is the shape
    var r & = g_vt_g4a
    if (!vt_gate(r, t.q8)) {
        return false
    }
    return vt_decline(r, VulkanTowerDecline.flavor) if (t.dev_backend != TowerDev.vulkan)
    let q8e = kq_elems(KqFmt.q8)
    let d = t.d_model
    let ff = t.n_ff
    let hs = t.d_head
    let past = gemma4a_attn_past()
    let rel_cap_rows = past + 1l
    let tail_dim = t.proj_dim
    if (d % 64l != 0l || ff % 64l != 0l || ff < 2l * d || t.n_head * hs != d || (hs != 64l && hs != 128l) || past != int64(GK_G4A_ATTN_PAST) || gemma4a_softcap() != GK_G4A_ATTN_CAP
            || npos > VT_MAX_ENCODE_ROWS || npos <= 0l || tail_dim % 64l != 0l || tail_dim > max(d, ff) || tail_dim != gemma4a_mid_dim(t)) {
        return vt_decline(r, VulkanTowerDecline.shape)
    }
    let arm = vt_arm(r)
    if (!arm.ok) {
        return false
    }
    if (!vk_range_ok(t.qblob.n) || !vk_range_ok(t.blob.n * 4l) || !vk_range_ok(vt_scratch_max_bytes(vt_cap_rows(npos), d, ff, 0l, 0l, false, false))) {
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
    if (!vt_g4a_planes(r, t)) {
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
    var recs : Q8Region[int(VT_G4A_GEMMS)]
    let rec_tiles = fixed_array(vff, v, v, v, v, v, v_rel, v2d, v, vff, v)
    vt_sched_walk(g_vt_maps, r.meta, nrec) $(rid : int64) : VtSchedRec {
        if (rid >= nrec - 2l) {
            return VtSchedRec(wblk0 = (rid == nrec - 2l ? t.outproj_w : t.mmproj_w) / q8e, rows = vt_tile_rows(vp, npos), groups = g_p)
        }
        let g = rid % VT_G4A_GEMMS
        if (g == 0l) {
            vt_g4a_regions(t.layers[rid / VT_G4A_GEMMS], d, ff, recs)
        }
        let rows = g == VT_G4A_REL ? rel_cap_rows : vt_tile_rows(rec_tiles[g], npos)   // the rel record keeps its raw rows: its planes are sized to them, and a forced l column would read past them
        let groups = g == VT_G4A_REL ? g_rel : (g == VT_G4A_PW1 ? g_2d : ((g == VT_G4A_FF_UP || g == VT_G4A_FF_UP_1) ? g_ff : g_d))
        return VtSchedRec(wblk0 = recs[g].woff / q8e, rows = rows, groups = groups)
    }
    let bx = npos * d * 4l
    let bo = npos * tail_dim * 4l
    upload_region_at(r.x_dev, 0l, unsafe(addr<uint8 const?>(s.x[0])), bx)
    let nd = npos * d
    let nff = npos * ff
    let n2d = npos * 2l * d
    let wg_attn = (npos * t.n_head + 63l) / 64l
    var pc_rq_rel = RqArgs(inbase = 0u, nblk = uint(rel_cap_rows * d / q8e), lo = -VT_NO_CLAMP, hi = VT_NO_CLAMP)
    var pc_rel = BatchArgs(n = uint(d), d = uint(d), map_off = 0u)
    var pc_glu = GkGatedRowsArgs(d = uint(d), nelem = uint(nd))
    var pc_dw = GkDwConvRowsArgs(c = uint(d), t_in = uint(npos))
    var pc_silu_ff = GkBiasActArgs(d = uint(ff), nelem = uint(nff), boff = 0u, act = BIAS_ACT_SILU)
    var pc_silu_d = GkBiasActArgs(d = uint(d), nelem = uint(nd), boff = 0u, act = BIAS_ACT_SILU)
    let sc = gemma4a_attn_scales(hs)
    var pc_attn = GkG4aAttnArgs(d = uint(d), npos = uint(npos), hs = uint(hs), pdsoff = 0u, cap = GK_G4A_ATTN_CAP, qscale = sc.qscale, kscale = sc.kscale)
    var pc_rms = gk_rms_args(uint(d), uint(npos), t.eps, 0u)
    var pc_post = GkPostAddLnArgs(d = uint(d), rows = uint(npos), woff = 0u, boff = 0u, eps = t.eps, ln_on = 1u, lnoff = 0u, lnboff = 0u, ascale = 1.0, xboff = 0u)
    let attn64 = hs == 64l
    vt_chain_one_shot("gemma4a", t.n_layer, npos) $(raw : VkCommandBuffer; var h : VkHaz) {
        vt_feed_enc(raw, h, cm2, r.s_rq_rel_tab, pc_rq_rel, rel_cap_rows * d)
        vt_pt(raw, VtProfRole.rq)
        for (l in range64(t.n_layer)) {
            let lo = t.layers[l]
            let rb = l * VT_G4A_GEMMS
            pc_rms.lnoff = uint(lo.ff_norm)
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
            pc_dw.woff = uint(lo.conv_dw)
            enc_tower_dw_conv5_cls(raw, h, r.s_dw, pc_dw, (nd + 255l) / 256l)
            vt_pt(raw, VtProfRole.dwconv)
            pc_rms.lnoff = uint(lo.conv_norm)
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
            pc_rms.lnoff = uint(lo.ln2)
            enc_tower_rms_cls(raw, h, r.s_rms_x, pc_rms, npos)
            vt_pt(raw, VtProfRole.rms)
        }
        vt_mm_clamped(raw, h, Clamp(), r.s_feed_x, nd, s_outproj, vp, d, tail_dim, g_vt_maps[nrec - 2l], g_p, r.s_clamp_yd, 0l)
        vt_pt(raw, VtProfRole.gemm_o)
        vt_bias_act(raw, h, r.s_bias_pt, tail_dim, npos * tail_dim, t.outproj_b, BIAS_ACT_NONE)
        vt_pt(raw, VtProfRole.bias)
        var pc_rms_pt = gk_rms_args(uint(tail_dim), uint(npos), t.eps, uint(r.ones_off))
        enc_tower_rms_cls(raw, h, r.s_rms_pt, pc_rms_pt, npos)
        vt_pt(raw, VtProfRole.rms)
        vt_mm_clamped(raw, h, Clamp(), r.s_feed_pt, npos * tail_dim, s_mmproj, vp, tail_dim, tail_dim, g_vt_maps[nrec - 1l], g_p, r.s_clamp_yd, 0l)
        vt_pt(raw, VtProfRole.gemm_o)
        vt_tail_copy_out(raw, h, r, bo)
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
//! a FastConformer family as the chain reads it - canary (biased GEMMs, channel-major taps, the mel on the device), parakeet
//! (no GEMM biases, tap-major taps, the CPU mel uploaded) and gigaam (the rotary form); the family's CPU loops are the
//! specification, its block offsets ride `g_vt_fc_offs` mapped onto canary's record
struct private VtFc {
    family : string
    front_name : string
    q8 : bool
    qblob : PlaneI8 = PlaneI8()
    qscales : PlaneF = PlaneF()
    blob : PlaneF = PlaneF()
    f16w : bool          //! gigaam's exact lane: the block GEMMs the carrier's halfwords, `w_n` of them at `w_off` in `wblob`, on the f16 GEMM
    wblob : PlaneU16 = PlaneU16()
    w_off : int64
    w_n : int64
    taps : PlaneU16 = PlaneU16()   //! gigaam's f16 depthwise taps: `taps_n` halfwords at `taps_off`, the layer offsets indexing the plane; empty: f32 in `blob`
    taps_off : int64
    taps_n : int64
    kq_lane : bool       //! gigaam's kq lane: the block GEMMs the carrier's own planes - a Q8_0 seat's blocks in `qblob` at its seat offset, a K-quant seat's superblocks in `kq` / `ks`; the seats `g_vt_fc_gseats`
    kq : PlaneU8 = PlaneU8()
    ks : PlaneU8 = PlaneU8()
    kq_repacked : bool   //! the K-quant planes in the CPU backend's grp interleave, which the gather un-groups; else disk order
    kq_fmts : uint       //! the K-quant members' format slots, a bit each
    dev_backend : TowerDev   //! the device block the model's image carries (`#tower-dev-block`): `vulkan` uploads its pieces
    dev : PlaneU8 = PlaneU8()
    devplan : PlaneI64 = PlaneI64()
    n_layer : int64
    d : int64
    ff : int64
    n_head : int64
    conv_kernel : int64
    eps : float
    biased : bool        //! the GEMMs carry bias rows; off, every bias offset is -1 and the seams add the branch bare
    tap_major : bool     //! the depthwise taps are [tap][ch], else [ch][tap]
    device_mel : bool    //! the front starts from the windowed frames (the spectrum and the mel on the device), else from the CPU mel rows
    rotary : bool        //! gigaam: no rel table; rope on the attention's normed input rows, the heads restrided onto the flash tile's width
    conv_ln : bool       //! gigaam: the depthwise conv biased, then a LayerNorm over the channels (the `bn_w` / `bn_b` slots) and SiLU
    n_mel : int64
    n_fft : int64
    n_fb : int64
    nch : int64
    c0_w : int64
    c0_b : int64
    c2_w : int64
    c2_b : int64
    c3_w : int64
    c3_b : int64
    c5_w : int64
    c5_b : int64
    c6_w : int64
    c6_b : int64
    pre_w : int64
    pre_b : int64
    proj_w : int64 = -1l   //! canary's projector behind the blocks, `proj_dim` rows over `d`: a Vulkan-flavored image's runs on the device
    proj_b : int64 = -1l
    proj_dim : int64
    twiddles : PlaneF = PlaneF()
    filters : PlaneF = PlaneF()
}

def private vt_fc_of_cn(t : CanaryEncoder) : VtFc {
    g_vt_fc_offs |> reserve(t.n_layer)
    g_vt_fc_offs |> resize(t.n_layer)
    for (l in range64(t.n_layer)) {
        g_vt_fc_offs[l] = t.layers[l]
    }
    return VtFc(family = "canary", front_name = "canary-front", q8 = t.q8, qblob = t.qblob, qscales = t.qscales, blob = t.blob,
        n_layer = t.n_layer, d = t.d_model, ff = t.n_ff, n_head = t.n_head, conv_kernel = t.conv_kernel, eps = t.eps,
        biased = true, tap_major = false, device_mel = true, n_mel = t.n_mel, n_fft = t.n_fft, n_fb = t.n_fb, nch = t.n_sub_ch,
        c0_w = t.c0_w, c0_b = t.c0_b, c2_w = t.c2_w, c2_b = t.c2_b, c3_w = t.c3_w, c3_b = t.c3_b, c5_w = t.c5_w, c5_b = t.c5_b,
        c6_w = t.c6_w, c6_b = t.c6_b, pre_w = t.pre_w, pre_b = t.pre_b, twiddles = t.twiddles, filters = t.filters,
        proj_w = t.proj_w, proj_b = t.proj_b, proj_dim = t.proj_dim, dev_backend = t.dev_backend, dev = t.dev, devplan = t.devplan)
}

def private vt_fc_of_pk(m : ParakeetModel) : VtFc {
    g_vt_fc_offs |> reserve(m.n_layer)
    g_vt_fc_offs |> resize(m.n_layer)
    for (l in range64(m.n_layer)) {
        g_vt_fc_offs[l] = pk_fc_layer(pk_layer_offsets(m, l))
    }
    return VtFc(family = "parakeet", front_name = "parakeet-front", q8 = m.q8, qblob = m.qblob, qscales = m.qscales, blob = m.blob,
        n_layer = m.n_layer, d = m.n_state, ff = 4l * m.n_state, n_head = m.n_head, conv_kernel = m.n_conv_kernel, eps = m.eps,
        biased = false, tap_major = true, device_mel = false, n_mel = m.n_mel, n_fft = m.n_fft, n_fb = m.n_fft / 2l + 1l, nch = m.n_sub_ch,
        c0_w = m.c0_w, c0_b = m.c0_b, c2_w = m.c2_w, c2_b = m.c2_b, c3_w = m.c3_w, c3_b = m.c3_b, c5_w = m.c5_w, c5_b = m.c5_b,
        c6_w = m.c6_w, c6_b = m.c6_b, pre_w = m.pre_w, pre_b = m.pre_b, twiddles = PlaneF(), filters = m.filters,
        dev_backend = m.dev_backend, dev = m.dev, devplan = m.devplan)
}

def private vt_fc_of_ga(m : GigaamModel) : VtFc {
    g_vt_fc_offs |> reserve(m.n_layer)
    g_vt_fc_offs |> resize(m.n_layer)
    for (l in range64(m.n_layer)) {
        g_vt_fc_offs[l] = gigaam_layer_offsets(m, l)
    }
    let nseats = m.kq_lane && m.dev_backend == TowerDev.none ? m.n_layer * GIGAAM_FC_RECORDS : 0l   // a Vulkan image carries its seats
    g_vt_fc_gseats |> reserve(nseats)
    g_vt_fc_gseats |> resize(nseats)
    let kq_fmts = (nseats > 0l ? gigaam_fc_record_seats(m, g_vt_fc_gseats)
        : uint(tower_dev_word(m.dev, m.devplan, VT_FC_KQ_FMTS_PIECE, 0l)))
    let f16w = !m.q8 && m.gemm_f16
    return VtFc(family = "gigaam", front_name = "gigaam-front", q8 = m.q8, qblob = m.qblob, qscales = m.qscales, blob = m.blob,
        f16w = f16w, wblob = f16w ? m.wblob : PlaneU16(), w_off = m.gemm_off, w_n = f16w ? m.n_layer * m.gemm_stride : 0l,
        taps = m.dw_f16 ? m.wblob : PlaneU16(), taps_off = m.dw_off, taps_n = m.dw_f16 ? m.n_layer * m.conv_kernel * m.d_model : 0l,
        kq_lane = m.kq_lane, kq = m.kq_lane ? m.kq : PlaneU8(), ks = m.kq_lane ? m.ks : PlaneU8(), kq_repacked = m.kq_repacked,
        kq_fmts = kq_fmts, dev_backend = m.dev_backend, dev = m.dev, devplan = m.devplan,
        n_layer = m.n_layer, d = m.d_model, ff = m.n_ff, n_head = m.n_head, conv_kernel = m.conv_kernel, eps = m.eps,
        biased = true, tap_major = true, device_mel = false, rotary = true, conv_ln = true, n_mel = m.n_mel, n_fft = m.n_fft,
        n_fb = m.n_fft / 2l + 1l, nch = m.n_sub_ch, c0_w = m.c0_w, c0_b = m.c0_b, c2_w = m.c2_w, c2_b = m.c2_b,
        c3_w = -1l, c3_b = -1l, c5_w = -1l, c5_b = -1l, c6_w = -1l, c6_b = -1l, pre_w = -1l, pre_b = -1l, twiddles = PlaneF(), filters = m.filters)
}

//! a GEMM's bias row and activation where the family carries bias rows, the activation alone where it does not (nothing at `BIAS_ACT_NONE`)
def private vt_fc_bias(raw : VkCommandBuffer; var h : VkHaz; biased : bool; var s_bias, s_act : VkDescriptorSet; dout, nout, boff : int64; act : uint) {
    if (biased) {
        vt_bias_act(raw, h, s_bias, dout, nout, boff, act)
    } elif (act != BIAS_ACT_NONE) {
        var pc = GkBiasActArgs(d = uint(dout), nelem = uint(nout), boff = 0u, act = act)
        enc_tower_act_cls(raw, h, s_act, pc, rq_wgs(false, nout))
    }
}

//! a block's seam on the biased stamp or the bare one
def private vt_fc_post(raw : VkCommandBuffer; var h : VkHaz; biased : bool; var s_b, s_nb : VkDescriptorSet; var pc : GkPostAddLnArgs; rows : int64) {
    if (biased) {
        enc_tower_post_add_ln_cls(raw, h, s_b, pc, rows)
    } else {
        enc_tower_post_add_ln_nb_cls(raw, h, s_nb, pc, rows)
    }
}

//! the subsample depthwise conv2d on the family's tap layout
def private vt_fc_dw2d(raw : VkCommandBuffer; var h : VkHaz; tap_major : bool; var s : VkDescriptorSet; var pc : GkDwConv2dArgs; wgs : int64) {
    if (tap_major) {
        enc_tower_pk_dw2d_cls(raw, h, s, pc, wgs)
    } else {
        enc_tower_dw_conv_cls(raw, h, s, pc, wgs)
    }
}

def private vt_cn_regions(lo : CanaryLayerOffs; d, ff : int64; var out : Q8Region[int(VT_CN_GEMMS)]) {
    out[VT_CN_FF1A] = Q8Region(woff = lo.ff1a_w, n = d, rows = ff)
    out[VT_CN_FF1B] = Q8Region(woff = lo.ff1b_w, n = ff, rows = d)
    out[VT_CN_Q] = Q8Region(woff = lo.q_w, n = d, rows = d)
    out[VT_CN_K] = Q8Region(woff = lo.k_w, n = d, rows = d)
    out[VT_CN_V] = Q8Region(woff = lo.v_w, n = d, rows = d)
    out[VT_CN_POS] = Q8Region(woff = max(lo.pos_w, 0l), n = d, rows = lo.pos_w >= 0l ? d : 0l)   //! a rotary block has no rel table: an empty record
    out[VT_CN_O] = Q8Region(woff = lo.o_w, n = d, rows = d)
    out[VT_CN_PW1] = Q8Region(woff = lo.pw1_w, n = d, rows = 2l * d)
    out[VT_CN_PW2] = Q8Region(woff = lo.pw2_w, n = d, rows = d)
    out[VT_CN_FF2A] = Q8Region(woff = lo.ff2a_w, n = d, rows = ff)
    out[VT_CN_FF2B] = Q8Region(woff = lo.ff2b_w, n = ff, rows = d)
}

[cold_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_FC.md#vk-tower-fc-kq")]
//! The kq lane's device plan: every GEMM record's seat in the device planes (`kq_seats`), the Q8_0 seats' regions in ``qblob``,
//! and each K-quant member's quant and scale bytes
struct private VtKqPlan {
    seats : array<VtFcSeat>
    regions : array<Q8Region>
    qbytes : table<int; int64>
    sbytes : table<int; int64>
}

[cold_path]
def private vt_fc_kq_plan(fc : VtFc) : VtKqPlan {
    let q8e = kq_elems(KqFmt.q8)
    let nrec = fc.n_layer * VT_CN_GEMMS
    var p : VtKqPlan
    p.seats |> reserve(nrec)
    p.seats |> resize(nrec)
    var recs : Q8Region[int(VT_CN_GEMMS)]
    for (l in range64(fc.n_layer)) {
        vt_cn_regions(g_vt_fc_offs[l], fc.d, fc.ff, recs)
        for (g in range64(VT_CN_GEMMS)) {
            let rid = l * VT_CN_GEMMS + g
            let seat = g_vt_fc_gseats[rid]
            p.seats[rid] = VtFcSeat(fmt = int(KqFmt.q8), wblk0 = 0l)
            continue if (recs[g].rows == 0l)
            if (seat.fmt == KqFmt.q8) {
                p.regions |> push(Q8Region(woff = seat.qoff, n = recs[g].n, rows = recs[g].rows))
                p.seats[rid] = VtFcSeat(fmt = int(KqFmt.q8), wblk0 = seat.qoff / q8e)
            } else {
                let fmt = int(seat.fmt)
                let bytes = dev_rows_bytes(seat.fmt, recs[g].n, recs[g].rows)
                let q0 = p.qbytes?[fmt] ?? 0l
                p.seats[rid] = VtFcSeat(fmt = fmt, wblk0 = q0 / kq_qsb(seat.fmt))
                p.qbytes[fmt] = q0 + bytes.q
                p.sbytes[fmt] = (p.sbytes?[fmt] ?? 0l) + bytes.s
            }
        }
    }
    return <- p
}

def private vt_upload_fc_kq(var r : VtResident; key : uint64; fc : VtFc; lay : Q8RepackLayout) : bool {
    var inscope p <- vt_fc_kq_plan(fc)
    var regions <- p.regions
    var qbytes <- p.qbytes
    let sbytes <- p.sbytes
    r.kq_seats <- p.seats
    vt_upload_begin(r, key)
    r.wq_bytes = fc.qblob.n
    r.ws_bytes = fc.qscales.n * 2l
    if (r.wq_bytes > 0l) {   // a carrier with no Q8_0 seat has no q8 planes
        r.wq_dev = vt_dev_buf(r, r.wq_bytes)
        r.ws_dev = vt_dev_buf(r, r.ws_bytes)
    }
    r.ones_off = fc.blob.n
    r.norms_bytes = fc.blob.n * 4l
    r.norms_dev = vt_dev_buf(r, r.norms_bytes)
    for (fmt, qb in keys(qbytes), values(qbytes)) {
        let sb = sbytes?[fmt] ?? 0l
        r.kq_planes[fmt] = VtKqPlane(q_dev = vt_dev_buf(r, qb), s_dev = vt_dev_buf(r, sb), q_bytes = qb, s_bytes = sb)
    }
    if (r.oom) {
        vt_release(r)
        delete regions
        return false
    }
    if (r.wq_bytes > 0l) {
        vt_upload_regions(fc.qblob, fc.qscales, regions, lay, r.wq_dev, r.ws_dev, r.wq_bytes, r.ws_bytes)
    }
    delete regions
    upload_float_ranges(r.norms_dev, [(p = plane_at(fc.blob, 0l), n = fc.blob.n)], 0l)
    for (fmt in keys(qbytes)) {
        vt_upload_kq_member(r, fc, fmt)
    }
    r.cap_rows = 0l
    vt_unbind_batch_sets(r)
    return true
}

//! one K-quant member's seats gathered in the tier's layout at their block bases (`kq_seats`) and uploaded into the member's planes
[cold_path]
def private vt_upload_kq_member(r : VtResident; fc : VtFc; fmt : int) {
    let kp = r.kq_planes?[fmt] ?? VtKqPlane()
    var wq, ws : array<uint8>
    vt_gather_kq_member(fc, fmt, r.kq_seats, kp.q_bytes, kp.s_bytes, wq, ws)
    unsafe {
        upload_region_at(kp.q_dev, 0l, addr<uint8 const?>(wq[0]), kp.q_bytes)
        upload_region_at(kp.s_dev, 0l, addr<uint8 const?>(ws[0]), kp.s_bytes)
    }
    delete wq
    delete ws
}

//! one K-quant member's seats gathered in the tier's layout at their block bases (`seats`) into `wq` / `ws`
[cold_path]
def private vt_gather_kq_member(fc : VtFc; fmt : int; seats : array<VtFcSeat>; q_bytes, s_bytes : int64; var wq, ws : array<uint8>) {
    wq |> reserve_resize(q_bytes)
    ws |> reserve_resize(s_bytes)
    var recs : Q8Region[int(VT_CN_GEMMS)]
    for (l in range64(fc.n_layer)) {
        vt_cn_regions(g_vt_fc_offs[l], fc.d, fc.ff, recs)
        for (g in range64(VT_CN_GEMMS)) {
            let rid = l * VT_CN_GEMMS + g
            let seat = g_vt_fc_gseats[rid]
            continue if (recs[g].rows == 0l || int(seat.fmt) != fmt)
            let kf = seat.fmt
            let base = seats[rid].wblk0
            kq_gather_rows_dev(kf, plane_at(fc.kq, seat.qoff), plane_at(fc.ks, seat.soff), recs[g].n, recs[g].rows, recs[g].rows,
                fc.kq_repacked, fc.kq_repacked ? active_kq_layout_mr(kq_schema_id(kf)) : 1l,
                wq, ws, base * kq_qsb(kf), base * kq_dev_ssb(kf), g_vt_no_sbits)
        }
    }
}

[cold_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vt_upload_fc(var r : VtResident; key : uint64; fc : VtFc; lay : Q8RepackLayout) : bool {
    if (fc.kq_lane) {
        return vt_upload_fc_kq(r, key, fc, lay)
    }
    var inscope regions <- vt_fc_q8_regions(fc)
    return vt_upload(r, key, fc.qblob, fc.qscales, regions, fc.blob, 0l, fc.blob.n, 0l, lay)
}

//! the q8 lane's regions of a FastConformer chain: every block GEMM, and a non-rotary family's front GEMMs behind them
def private vt_fc_q8_regions(fc : VtFc) : array<Q8Region> {
    let d = fc.d
    let ff = fc.ff
    var regions : array<Q8Region>
    regions |> reserve(fc.n_layer * VT_CN_GEMMS + 3l)
    var recs : Q8Region[int(VT_CN_GEMMS)]
    for (l in range64(fc.n_layer)) {
        vt_cn_regions(g_vt_fc_offs[l], d, ff, recs)
        for (rg in recs) {
            regions |> push(rg) if (rg.rows > 0l)
        }
    }
    if (!fc.rotary) {   //! the rotary family's front convs stay f32 in the blob, uploaded as the f16 slab
        let nch = fc.nch
        let w3 = conv_s2_out(conv_s2_out(conv_s2_out(fc.n_mel)))
        regions |> push(Q8Region(woff = fc.c3_w, n = nch, rows = nch))
        regions |> push(Q8Region(woff = fc.c6_w, n = nch, rows = nch))
        regions |> push(Q8Region(woff = fc.pre_w, n = w3 * nch, rows = d))
    }
    return <- regions
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
    r.s_rq_x = set_tower_clamp_rq_cls(fixed_array(r.x_dev, r.xq_dev, r.xs_dev), fixed_array(bx, bq, bs), fixed_array(VT_X, VT_XQ, VT_XQ))
    if (r.rel_cap_rows > 0l) {
        r.s_rq_rel_tab = set_tower_clamp_rq_cls(fixed_array(r.rel_tab_dev, r.rel_tabq_dev, r.rel_tabs_dev), fixed_array(b_rel, b_relq, b_relq / q8e * 4l), fixed_array(VT_REL_TAB, VT_REL_TABQ, VT_REL_TABQ))
        r.s_cn_rel_sincos = set_tower_rel_sincos_cls(fixed_array(r.rel_tab_dev), fixed_array(b_rel), fixed_array(VT_REL_TAB))
    }
    r.s_bias_q = set_tower_bias_act_cls(fixed_array(r.q_dev, r.norms_dev), fixed_array(bx, r.norms_bytes), fixed_array(VT_Q, 0u))
    r.s_bias_k = set_tower_bias_act_cls(fixed_array(r.k_dev, r.norms_dev), fixed_array(bx, r.norms_bytes), fixed_array(VT_K, 0u))
    r.s_bias_v = set_tower_bias_act_cls(fixed_array(r.v_dev, r.norms_dev), fixed_array(bx, r.norms_bytes), fixed_array(VT_V, 0u))
    r.s_bias_hg = set_tower_bias_act_cls(fixed_array(r.hg_dev, r.norms_dev), fixed_array(bff, r.norms_bytes), fixed_array(VT_HG, 0u))
    r.s_glu_sig = set_tower_glu_sig_cls(fixed_array(r.hg_dev, r.hu_dev), fixed_array(bff, bff), fixed_array(VT_HG, VT_HU))
    r.s_cn_dw = set_tower_cn_dw_cls(fixed_array(r.hu_dev, r.yd_dev, r.norms_dev), fixed_array(bff, bx, r.norms_bytes), fixed_array(VT_HU, VT_YD, 0u))
    r.s_pk_dw = set_tower_pk_dw_cls(fixed_array(r.hu_dev, r.yd_dev, r.norms_dev), fixed_array(bff, bx, r.norms_bytes), fixed_array(VT_HU, VT_YD, 0u))
    r.s_silu_hg = set_tower_act_cls(fixed_array(r.hg_dev), fixed_array(bff), fixed_array(VT_HG))
    r.s_post_o_ln = set_tower_post_add_ln_cls(fixed_array(r.x_dev, r.yo_dev, r.xb_dev, r.norms_dev), fixed_array(bx, bx, bx, r.norms_bytes), fixed_array(VT_X, VT_YO, VT_XB, 0u))
    r.s_post_d_ln = set_tower_post_add_ln_cls(fixed_array(r.x_dev, r.yd_dev, r.xb_dev, r.norms_dev), fixed_array(bx, bx, bx, r.norms_bytes), fixed_array(VT_X, VT_YD, VT_XB, 0u))
    r.s_post_o_ln_nb = set_tower_post_add_ln_nb_cls(fixed_array(r.x_dev, r.yo_dev, r.xb_dev, r.norms_dev), fixed_array(bx, bx, bx, r.norms_bytes), fixed_array(VT_X, VT_YO, VT_XB, 0u))
    r.s_post_d_ln_nb = set_tower_post_add_ln_nb_cls(fixed_array(r.x_dev, r.yd_dev, r.xb_dev, r.norms_dev), fixed_array(bx, bx, bx, r.norms_bytes), fixed_array(VT_X, VT_YD, VT_XB, 0u))
    if (r.pad_dp > 0l) {
        vt_sets_fc_rot(r)
    }
    vt_sets_cm2_feed(r)
}

//! the rotary form's sets: the rope over the normed rows off the tables in `tabs_dev` (cos, then sin, `hs / 2` a row), the biased
//! head restrides onto the padded panels, the flash tile and the unpad, the conv module's biased taps and its LayerNorm and SiLU
[cold_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_FC.md#vk-tower-fc-rotary")]
def private vt_sets_fc_rot(var r : VtResident) {
    let cap = r.cap_rows
    let bx = cap * r.d * 4l
    let bff = cap * r.ff * 4l
    let btabs = cap * r.hs * 4l
    r.s_rope_tq = set_tower_rope_tab_cls(fixed_array(r.xb_dev, r.tabs_dev, r.tabs_dev), fixed_array(bx, btabs, btabs), fixed_array(VT_XB, 0u, 0u))
    vt_sets_pad_b(r)
    vt_sets_fa_unpad(r)
    r.s_ga_dw = set_tower_ga_dw_cls(fixed_array(r.hu_dev, r.yd_dev, r.norms_dev), fixed_array(bff, bx, r.norms_bytes), fixed_array(VT_HU, VT_YD, 0u))
    if (r.taps_bytes > 0l) {
        r.s_ga_dwh = set_tower_ga_dwh_cls(fixed_array(r.hu_dev, r.yd_dev, r.norms_dev, r.taps_dev), fixed_array(bff, bx, r.norms_bytes, r.taps_bytes),
            fixed_array(VT_HU, VT_YD, 0u, 0u))
    }
    r.s_ln_yd = set_tower_ln_cls(fixed_array(r.yd_dev, r.yd_dev, r.norms_dev), fixed_array(bx, bx, r.norms_bytes), fixed_array(VT_YD, VT_YD, 0u))
    r.s_silu_yd = set_tower_act_cls(fixed_array(r.yd_dev), fixed_array(bx), fixed_array(VT_YD))
}

//! the biased head restrides of q, k and v onto the padded panels - q at f32, k and v at f16 - each head's bias read from `norms_dev`
[cold_path]
def private vt_sets_pad_b(var r : VtResident) {
    let bx = r.cap_rows * r.d * 4l
    let bp = r.cap_rows * r.pad_dp * 4l
    let bph = r.cap_rows * r.pad_dp * 2l
    r.s_pad_qb = set_tower_hpad_b_cls(fixed_array(r.q_dev, r.qp_dev, r.norms_dev), fixed_array(bx, bp, r.norms_bytes), fixed_array(VT_Q, VT_Q2, 0u))
    r.s_pad_kb = set_tower_hpad16_b_cls(fixed_array(r.k_dev, r.kp16_dev, r.norms_dev), fixed_array(bx, bph, r.norms_bytes), fixed_array(VT_K, VT_K16, 0u))
    r.s_pad_vb = set_tower_hpad16_b_cls(fixed_array(r.v_dev, r.vp16_dev, r.norms_dev), fixed_array(bx, bph, r.norms_bytes), fixed_array(VT_V, VT_V16, 0u))
}

//! the flash tile over the padded panels into `attp_dev`, and the unpad of its rows into `att_dev`
[cold_path]
def private vt_sets_fa_unpad(var r : VtResident) {
    let bx = r.cap_rows * r.d * 4l
    let bp = r.cap_rows * r.pad_dp * 4l
    let bph = r.cap_rows * r.pad_dp * 2l
    r.s_fa = fa_stamp_set(r.fa_khr, fixed_array(r.qp_dev, r.kp16_dev, r.vp16_dev, r.attp_dev), fixed_array(bp, bph, bph, bp), fixed_array(VT_Q2, VT_K16, VT_V16, VT_K2))
    r.s_unpad = set_tower_hunpad_cls(fixed_array(r.attp_dev, r.att_dev), fixed_array(bp, bx), fixed_array(VT_K2, VT_ATT))
}

def private vt_cn_rows_ok(npos, d, ff : int64) : bool {
    let cap = vt_cap_rows(npos)
    return (vk_range_ok(vt_scratch_max_bytes(cap, d, ff, 0l, 0l, false, false)) && vk_range_ok(2l * cap * d * 4l)
        && vk_range_ok(cap * (2l * cap - 1l) * 4l))
}

[cold_path]
def private vt_cn_rel_plane(var r : VtResident; cap, hs : int64; gemm_khr : bool) : bool {
    if (r.cn_plane_rows > 0l) {
        release_device_buf(r.cn_qh_dev)
        release_device_buf(r.cn_ph_dev)
        release_device_buf(r.cn_r_dev)
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
        release_device_buf(r.cn_qh_dev)
        release_device_buf(r.cn_ph_dev)
        release_device_buf(r.cn_r_dev)
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
    var pc = gk_f16_rows_args(uint(nelem))
    enc_tower_clamp_cvt_cls(raw, h, s, pc, (nelem / 4l + 255l) / 256l)
}

//! the elements [from, to) of a plane zeroed by the fill command (the length mask's rows past a stage's valid length)
def private vt_zero_elems(raw : VkCommandBuffer; var h : VkHaz; dev : uint64; bit : uint; from, to : int64) {
    if (from < to) {
        vhz_dep(raw, h, 0u, bit, true)
        cmd_fill_zero(raw, dev, from * 4l, (to - from) * 4l)
    }
}

//! the row-broadcast bias (and `act`) over `nout` elements of rows `dout` wide, the bias row at `boff` of the set's second binding
def vt_bias_act(raw : VkCommandBuffer; var h : VkHaz; var s_bias : VkDescriptorSet; dout, nout, boff : int64; act : uint) {
    var pc_b = GkBiasActArgs(d = uint(dout), nelem = uint(nout), boff = uint(boff), act = act)
    enc_tower_bias_act_cls(raw, h, s_bias, pc_b, rq_wgs(false, nout))
}

//! the block chain's planes and scratch inside the device's buffer range at `npos` rows
def private vt_fc_range_ok(fc : VtFc; npos : int64) : bool {
    let hsp = vt_fc_hs(fc.d, fc.n_head) <= 64l ? 64l : 128l
    let dp = fc.rotary ? fc.n_head * hsp : 0l
    let wq_bytes = fc.dev_backend == TowerDev.vulkan ? tower_dev_find(fc.devplan, VT_FC_WQ_PIECE).bytes : fc.qblob.n
    return (vk_range_ok(fc.f16w ? fc.w_n * 2l : wq_bytes) && vk_range_ok(fc.blob.n * 4l) && vt_fc_rows_ok(fc.rotary, npos, fc.d, fc.ff, dp)
        && (!fc.kq_lane || (vk_range_ok(fc.kq.n) && vk_range_ok(fc.ks.n))))
}

//! the exact lane's GEMMs on the device: the f16 feed (cm2 or KHR), the f16 GEMM class built, the widths on its 64 lattice and
//! the pointwise conv's 2d rows inside the ff-wide plane it lands in; the kq lane asks the f16 feed alone, the q8 lane nothing
def private vt_fc_lane_ok(fc : VtFc; gemm_khr : bool) : bool {
    if (fc.f16w) {
        return vt_cm2_feed() && fc.d % 64l == 0l && fc.ff % 64l == 0l && fc.ff >= 2l * fc.d && f16_gemm_ensure(gemm_khr)
    }
    return !fc.kq_lane || vt_cm2_feed()
}

//! asked after `vt_fc_lane_ok`, which holds the f16 feed these tiles read
[arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_FC.md#vk-tower-fc-kq")]
def private vt_fc_kq_tiles_ok(fc : VtFc; npos : int64) : bool {
    return true if (!fc.kq_lane)
    for (fmt in range(KQ_FMT_COUNT)) {
        continue if ((fc.kq_fmts & (1u << uint(fmt))) == 0u)
        if (!vt_tile_ensure(vt_tile(true, fc.d, npos, fmt)) || !vt_tile_ensure(vt_tile(true, fc.ff, npos, fmt)) || !vt_tile_ensure(vt_tile(true, 2l * fc.d, npos, fmt))) {
            return false
        }
    }
    return true
}

//! the block chain would admit `npos` rows behind a front about to run (its own ropes built for them): nothing counted - a
//! front that serves only to have the blocks decline does the CPU's front over again, and the block seat behind the CPU front
//! counts that decline itself
def private vt_fc_admits(fc : VtFc; npos, ww : int64; khr, gemm_khr : bool) : bool {
    let rope_n = npos * (vt_fc_hs(fc.d, fc.n_head) / 2l)
    return (vt_fc_shape_ok(fc.rotary, fc.d, fc.ff, fc.n_head, fc.conv_kernel, npos, ww, rope_n) && vt_fc_range_ok(fc, npos)
        && vt_fc_classes_ok(fc.rotary, fc.d, fc.ff, fc.n_head, npos, ww, khr, gemm_khr, vt_cm2_feed()) && vt_fc_lane_ok(fc, gemm_khr)
        && vt_fc_kq_tiles_ok(fc, npos))
}

//! one block GEMM of the chain off its feed: the f16 GEMM over the halfword plane at `woff` (gigaam's exact lane), else the q8
//! tile over its schedule record (`map_idx`)
struct private VtFcMm {
    f16w : bool
    gemm_khr : bool
    npos : int64
    w_base : int64
}

def private vt_fc_mm(raw : VkCommandBuffer; var h : VkHaz; m : VtFcMm; var s : VkDescriptorSet; t : VtTile; map_idx, groups, woff, n, dout : int64) {
    if (m.f16w) {
        var pc = F16GemmArgs(n = uint(n), d = uint(dout), rows = uint(m.npos), woff = uint(woff - m.w_base), ybase = 0u, ystride = uint(dout), ksplit = 0u)
        f16_gemm_enc(m.gemm_khr, raw, h, s, pc, m.npos, dout)
    } else {
        var pc = BatchArgs(n = uint(n), d = uint(dout), map_off = g_vt_maps[map_idx])
        vt_tile_enc(raw, h, t, s, pc, groups)
    }
}

//! a block GEMM's input fed (the f16 convert on a cm2 device, else the Q8_0 requant), then `vt_fc_mm`
def private vt_fc_fed_mm(raw : VkCommandBuffer; var h : VkHaz; m : VtFcMm; var s_rq : VkDescriptorSet; nin : int64; var s : VkDescriptorSet; t : VtTile;
                         map_idx, groups, woff, n, dout : int64) {
    var pc_rq = RqArgs(inbase = 0u, nblk = uint(nin / kq_elems(KqFmt.q8)), lo = -VT_NO_CLAMP, hi = VT_NO_CLAMP)
    vt_feed_enc(raw, h, m.f16w || t.tc > 0l, s_rq, pc_rq, nin)
    vt_fc_mm(raw, h, m, s, t, map_idx, groups, woff, n, dout)
}

//! the exact lane's GEMM sets: the f16 GEMM over the halfword plane and the f16 feed, into the chain's output planes - the
//! d-wide sites, the ff-wide ones (the pointwise conv's 2d rows among them)
[cold_path]
def private vt_sets_fc16(var r : VtResident; gemm_khr : bool) {
    let cap = r.cap_rows
    let bx = cap * r.d * 4l
    let bff = cap * r.ff * 4l
    let bxh = (cap + TILE_READ_SLACK) * max(r.d, r.ff) * 2l
    r.s_q = f16_gemm_set(gemm_khr, fixed_array(r.wq_dev, r.xh_dev, r.q_dev), fixed_array(r.wq_bytes, bxh, bx), fixed_array(0u, VT_XQ, VT_Q))
    r.s_k = f16_gemm_set(gemm_khr, fixed_array(r.wq_dev, r.xh_dev, r.k_dev), fixed_array(r.wq_bytes, bxh, bx), fixed_array(0u, VT_XQ, VT_K))
    r.s_v = f16_gemm_set(gemm_khr, fixed_array(r.wq_dev, r.xh_dev, r.v_dev), fixed_array(r.wq_bytes, bxh, bx), fixed_array(0u, VT_XQ, VT_V))
    r.s_o = f16_gemm_set(gemm_khr, fixed_array(r.wq_dev, r.xh_dev, r.yo_dev), fixed_array(r.wq_bytes, bxh, bx), fixed_array(0u, VT_XQ, VT_YO))
    r.s_down = f16_gemm_set(gemm_khr, fixed_array(r.wq_dev, r.xh_dev, r.yd_dev), fixed_array(r.wq_bytes, bxh, bx), fixed_array(0u, VT_XQ, VT_YD))
    r.s_hg = f16_gemm_set(gemm_khr, fixed_array(r.wq_dev, r.xh_dev, r.hg_dev), fixed_array(r.wq_bytes, bxh, bff), fixed_array(0u, VT_XQ, VT_HG))
    r.gemm_khr = gemm_khr
    r.fc16 = true
}

let private VT_FC_OUT_Q = 0      //! a chain GEMM's output plane, the set `vt_fc_site` picks from a tile's bundle
let private VT_FC_OUT_K = 1
let private VT_FC_OUT_V = 2
let private VT_FC_OUT_O = 3
let private VT_FC_OUT_DOWN = 4
let private VT_FC_OUT_HG = 5

[arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_FC.md#vk-tower-fc-kq")]
def private vt_fc_site(var r : VtResident; fc : VtFc; rid : int64; t : VtTile; s : VkDescriptorSet; out : int) : tuple<t : VtTile; s : VkDescriptorSet> {
    return (t = t, s = s) if (!fc.kq_lane || r.kq_seats[rid].fmt == int(KqFmt.q8))
    var tt = t
    tt.fmt = r.kq_seats[rid].fmt
    if (out == VT_FC_OUT_HG) {
        return (t = tt, s = vt_sets_ff(r, tt).s_hg)
    }
    let sd = vt_sets_d(r, tt)
    let ss = out == VT_FC_OUT_Q ? sd.s_q : (out == VT_FC_OUT_K ? sd.s_k : (out == VT_FC_OUT_V ? sd.s_v : (out == VT_FC_OUT_O ? sd.s_o : sd.s_down)))
    return (t = tt, s = ss)
}

[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vt_fc_blocks(var r : VtResident; fc : VtFc; var x : array<float>; npos, ww : int64;   // nolint:STYLE038 — the dispatch chain is the shape
                         rope_cos : array<float> = g_vt_no_rows; rope_sin : array<float> = g_vt_no_rows) : bool {
    g_vt_fc_out_bytes = 0l
    let q8e = kq_elems(KqFmt.q8)
    let d = fc.d
    let ff = fc.ff
    let hs = vt_fc_hs(d, fc.n_head)
    let hsp = hs <= 64l ? 64l : 128l
    let dp = fc.rotary ? fc.n_head * hsp : 0l
    if (!vt_fc_shape_ok(fc.rotary, d, ff, fc.n_head, fc.conv_kernel, npos, ww, min(long_length(rope_cos), long_length(rope_sin)))) {
        return vt_decline(r, VulkanTowerDecline.shape)
    }
    let arm = vt_arm(r, true)
    if (!arm.ok) {
        return false
    }
    if (!vt_fc_range_ok(fc, npos)) {
        return vt_decline(r, VulkanTowerDecline.shape)
    }
    let cm2 = vt_cm2_feed()
    if (!vt_fc_classes_ok(fc.rotary, d, ff, fc.n_head, npos, ww, arm.khr, arm.gemm_khr, cm2) || !vt_fc_lane_ok(fc, arm.gemm_khr)) {
        return vt_decline(r, VulkanTowerDecline.device)
    }
    if (!vt_fc_kq_tiles_ok(fc, npos)) {
        return vt_decline_kq(r, gigaam_kq_members_of(fc.kq_fmts, fc.qblob.n > 0l))
    }
    let mm = VtFcMm(f16w = fc.f16w, gemm_khr = arm.gemm_khr, npos = npos, w_base = fc.w_off)
    let v = vt_tile(cm2, d, npos)
    let vff = vt_tile(cm2, ff, npos)
    let v2d = vt_tile(cm2, 2l * d, npos)
    let v_rel = vt_tile(cm2, d, fc.rotary ? npos : ww)
    if (!vt_fc_planes(r, fc)) {
        return vt_decline(r, VulkanTowerDecline.memory)
    }
    let g_d = vt_tile_groups(v, d, npos)
    let g_ff = vt_tile_groups(vff, ff, npos)
    let g_2d = vt_tile_groups(v2d, 2l * d, npos)
    let g_rel = fc.rotary ? 0l : vt_tile_groups(v_rel, d, ww)
    let nrec = fc.n_layer * VT_CN_GEMMS
    let meta_bytes = (nrec * 4l + fc.n_layer * (7l * g_d + 2l * g_ff + g_2d + g_rel)) * 4l
    let rel_cap = 2l * vt_cap_rows(npos)
    let sets_moved = r.fc16 != fc.f16w || (fc.f16w && r.gemm_khr != arm.gemm_khr)
    if (!vt_scratch_fits(r, npos, d, ff, dp, meta_bytes, arm.khr, false, 0l) || (!fc.rotary && r.rel_cap_rows < ww) || r.hs != hs || sets_moved) {
        if (!vt_scratch_build(r, npos, d, ff, hs, dp, meta_bytes, arm.khr, false, 0l) || (!fc.rotary && !vt_rel_bufs(r, rel_cap, d))) {
            vt_release_scratch(r)
            return vt_decline(r, VulkanTowerDecline.memory)
        }
        vt_sets_cn(r)
        if (fc.f16w) {
            vt_sets_fc16(r, arm.gemm_khr)
        }
    }
    if (!fc.rotary && r.cn_plane_rows < vt_cap_rows(npos) && !vt_cn_rel_plane(r, vt_cap_rows(npos), hs, arm.gemm_khr)) {
        return vt_decline(r, VulkanTowerDecline.memory)
    }
    var s_none : VkDescriptorSet
    var s_h2 = r.s_hg
    if (!fc.f16w) {
        vt_bind_tile_sets(r, v, vff)
        s_h2 = vt_sets_ff(r, v2d).s_hg
        var recs : Q8Region[int(VT_CN_GEMMS)]
        let rec_tiles = fixed_array(vff, v, v, v, v, v_rel, v, v2d, v, vff, v)
        vt_sched_walk(g_vt_maps, r.meta, nrec) $(rid : int64) : VtSchedRec {
            let g = rid % VT_CN_GEMMS
            if (g == 0l) {
                vt_cn_regions(g_vt_fc_offs[rid / VT_CN_GEMMS], d, ff, recs)
            }
            let groups = g == VT_CN_POS ? g_rel : (g == VT_CN_PW1 ? g_2d : ((g == VT_CN_FF1A || g == VT_CN_FF2A) ? g_ff : g_d))
            let rows = g == VT_CN_POS ? (fc.rotary ? 0l : vt_tile_rows(rec_tiles[g], ww)) : vt_tile_rows(rec_tiles[g], npos)
            return VtSchedRec(wblk0 = fc.kq_lane ? r.kq_seats[rid].wblk0 : recs[g].woff / q8e, rows = rows, groups = groups)
        }
    }
    var s_rel = fc.rotary ? s_none : vt_rel_proj_set(r, v_rel)
    let bx = npos * d * 4l
    upload_region_at(r.x_dev, 0l, unsafe(addr<uint8 const?>(x[0])), bx)
    let half = hs / 2l
    if (fc.rotary) {   //! the session's tables: cos [npos][hs / 2], then sin, in `tabs_dev`
        upload_region_at(r.tabs_dev, 0l, unsafe(addr<uint8 const?>(rope_cos[0])), npos * half * 4l)
        upload_region_at(r.tabs_dev, npos * half * 4l, unsafe(addr<uint8 const?>(rope_sin[0])), npos * half * 4l)
    }
    let npos_pad = vt_cap_rows(npos)
    var pc_rope = GkRopeTabArgs(rows = uint(npos), qs = uint(d), col = 0u, d = uint(d), dh = uint(hs), coff = 0u, soff = uint(npos * half))
    let wg_rope = (npos * (d / 2l) + 255l) / 256l
    var pc_rs = GkRestrideArgs(rows = uint(npos), rows_pad = uint(npos_pad), nheads = uint(fc.n_head), hs = uint(hs), hsp = uint(hsp), sstride = uint(d), soff = 0u)
    let wg_pad = (npos_pad * dp + 255l) / 256l
    let wg_unpad = (npos * dp + 255l) / 256l
    var pc_fa = FaArgs(rows = uint(npos), w0 = 0u, qd = uint(dp), kvd = uint(dp), kv_mul = 1u, kvbase = 0u, scale = 1.0 / sqrt(float(hs)), window = 0u, sinkoff = 0u)
    let wg_fa = fa_stamp_wgs(arm.khr, fc.n_head, npos)
    var pc_lncv = gk_ln_args(uint(d), uint(npos), fc.eps, 0u, 0u)
    var pc_silu = GkBiasActArgs(d = uint(d), nelem = uint(npos * d), boff = 0u, act = BIAS_ACT_SILU)
    var pc_pos = GkRelSinCosArgs(d = uint(d), tt = uint(npos), rows_pad = uint(ww))
    let nd = npos * d
    let nff = npos * ff
    let n2d = npos * 2l * d
    let wg_attn = (npos + 63l) / 64l
    var pc_rq_rel = RqArgs(inbase = 0u, nblk = uint(ww * d / q8e), lo = -VT_NO_CLAMP, hi = VT_NO_CLAMP)
    var pc_rq_xb = RqArgs(inbase = 0u, nblk = uint(nd / q8e), lo = -VT_NO_CLAMP, hi = VT_NO_CLAMP)
    var pc_rel = BatchArgs(n = uint(d), d = uint(d), map_off = 0u)
    var pc_glu = GkGatedRowsArgs(d = uint(d), nelem = uint(nd))
    var pc_dw = GkDwConvRowsArgs(c = uint(d), k = uint(fc.conv_kernel), t_in = uint(npos))
    var pc_attn_r = TowerCnAttnRArgs(d = uint(d), npos = uint(npos), buoff = 0u, hoff = 0u)
    var pc_ln = gk_ln_args(uint(d), uint(npos), fc.eps, 0u, 0u)
    var pc_post = GkPostAddLnArgs(d = uint(d), rows = uint(npos), woff = 0u, boff = 0u, eps = fc.eps, ln_on = 1u, lnoff = 0u, lnboff = 0u, ascale = 1.0, xboff = 0u)
    let attn64 = hs == 64l
    vt_chain_one_shot(fc.family, fc.n_layer, npos) $(raw : VkCommandBuffer; var h : VkHaz) {
        if (!fc.rotary) {
            enc_tower_rel_sincos_cls(raw, h, r.s_cn_rel_sincos, pc_pos, (ww * d + 255l) / 256l)
            vt_feed_enc(raw, h, cm2, r.s_rq_rel_tab, pc_rq_rel, ww * d)
            vt_pt(raw, VtProfRole.rq)
        }
        for (l in range64(fc.n_layer)) {
            let lo = g_vt_fc_offs[l]
            let rb = l * VT_CN_GEMMS
            pc_ln.lnoff = uint(lo.nf1_w)
            pc_ln.lnboff = uint(lo.nf1_b)
            enc_tower_ln_cls(raw, h, r.s_ln, pc_ln, npos)
            vt_pt(raw, VtProfRole.ln)
            var m_ff1a = vt_fc_site(r, fc, rb + VT_CN_FF1A, vff, r.s_hg, VT_FC_OUT_HG)
            vt_fc_fed_mm(raw, h, mm, r.s_rq_xb, nd, m_ff1a.s, m_ff1a.t, rb + VT_CN_FF1A, g_ff, lo.ff1a_w, d, ff)
            vt_pt(raw, VtProfRole.gemm_fc1)
            vt_fc_bias(raw, h, fc.biased, r.s_bias_hg, r.s_silu_hg, ff, nff, lo.ff1a_b, BIAS_ACT_SILU)
            vt_pt(raw, VtProfRole.bias)
            var m_ff1b = vt_fc_site(r, fc, rb + VT_CN_FF1B, v, r.s_down, VT_FC_OUT_DOWN)
            vt_fc_fed_mm(raw, h, mm, r.s_rq_hg, nff, m_ff1b.s, m_ff1b.t, rb + VT_CN_FF1B, g_d, lo.ff1b_w, ff, d)
            vt_pt(raw, VtProfRole.gemm_fc2)
            pc_post.boff = uint(lo.ff1b_b)
            pc_post.ascale = 0.5
            pc_post.lnoff = uint(lo.nat_w)
            pc_post.lnboff = uint(lo.nat_b)
            vt_fc_post(raw, h, fc.biased, r.s_post_d_ln, r.s_post_d_ln_nb, pc_post, npos)
            vt_pt(raw, VtProfRole.post_add)
            vt_feed_enc(raw, h, cm2, r.s_rq_xb, pc_rq_xb, nd)
            vt_pt(raw, VtProfRole.rq)
            if (fc.rotary) {
                vt_fc_rot_attn(raw, h, r, fc, v, mm, rb, g_d, d, lo, cm2, pc_rq_xb, nd, pc_rope, wg_rope, pc_rs, wg_pad, wg_unpad, arm.khr, hsp, pc_fa, wg_fa)
            } else {
                var pc_g = BatchArgs(n = uint(d), d = uint(d), map_off = g_vt_maps[rb + VT_CN_Q])
                vt_tile_enc(raw, h, v, r.s_q, pc_g, g_d)
                vt_pt(raw, VtProfRole.gemm_q)
                vt_fc_bias(raw, h, fc.biased, r.s_bias_q, s_none, d, nd, lo.q_b, BIAS_ACT_NONE)
                vt_pt(raw, VtProfRole.bias)
                pc_g.map_off = g_vt_maps[rb + VT_CN_K]
                vt_tile_enc(raw, h, v, r.s_k, pc_g, g_d)
                vt_pt(raw, VtProfRole.gemm_k)
                vt_fc_bias(raw, h, fc.biased, r.s_bias_k, s_none, d, nd, lo.k_b, BIAS_ACT_NONE)
                vt_pt(raw, VtProfRole.bias)
                pc_g.map_off = g_vt_maps[rb + VT_CN_V]
                vt_tile_enc(raw, h, v, r.s_v, pc_g, g_d)
                vt_pt(raw, VtProfRole.gemm_v)
                vt_fc_bias(raw, h, fc.biased, r.s_bias_v, s_none, d, nd, lo.v_b, BIAS_ACT_NONE)
                vt_pt(raw, VtProfRole.bias)
                pc_rel.map_off = g_vt_maps[rb + VT_CN_POS]
                vt_tile_enc(raw, h, v_rel, s_rel, pc_rel, g_rel)
                vt_pt(raw, VtProfRole.gemm_rel)
                for (hh in range64(fc.n_head)) {
                    let hoff = hh * hs
                    var pc_gq = GkRestrideArgs(rows = uint(npos), rows_pad = uint(npos), nheads = 1u, hs = uint(hs), hsp = uint(hs), sstride = uint(d), soff = uint(hoff),
                        boff = uint(lo.bias_v + hoff))
                    enc_tower_hpad16_b_cls(raw, h, r.s_cn_gq, pc_gq, (npos * hs + 255l) / 256l)
                    var pc_gp = GkRestrideArgs(rows = uint(ww), rows_pad = uint(ww), nheads = 1u, hs = uint(hs), hsp = uint(hs), sstride = uint(d), soff = uint(hoff))
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
            }
            var m_o = vt_fc_site(r, fc, rb + VT_CN_O, v, r.s_o, VT_FC_OUT_O)
            vt_fc_fed_mm(raw, h, mm, r.s_rq_att, nd, m_o.s, m_o.t, rb + VT_CN_O, g_d, lo.o_w, d, d)
            vt_pt(raw, VtProfRole.gemm_o)
            pc_post.boff = uint(lo.o_b)
            pc_post.ascale = 1.0
            pc_post.lnoff = uint(lo.ncv_w)
            pc_post.lnboff = uint(lo.ncv_b)
            vt_fc_post(raw, h, fc.biased, r.s_post_o_ln, r.s_post_o_ln_nb, pc_post, npos)
            vt_pt(raw, VtProfRole.post_add)
            var m_pw1 = vt_fc_site(r, fc, rb + VT_CN_PW1, v2d, s_h2, VT_FC_OUT_HG)
            vt_fc_fed_mm(raw, h, mm, r.s_rq_xb, nd, m_pw1.s, m_pw1.t, rb + VT_CN_PW1, g_2d, lo.pw1_w, d, 2l * d)
            vt_pt(raw, VtProfRole.gemm_pw)
            vt_fc_bias(raw, h, fc.biased, r.s_bias_hg, s_none, 2l * d, n2d, lo.pw1_b, BIAS_ACT_NONE)
            vt_pt(raw, VtProfRole.bias)
            enc_tower_glu_sig_cls(raw, h, r.s_glu_sig, pc_glu, (nd + 255l) / 256l)
            vt_pt(raw, VtProfRole.glu)
            pc_dw.woff = uint(lo.dw_w)
            pc_dw.bnwoff = uint(lo.bn_w)
            pc_dw.bnboff = uint(lo.bn_b)
            pc_dw.boff = uint(max(lo.dw_b, 0l))
            if (fc.conv_ln && r.taps_bytes > 0l) {
                pc_dw.woff = uint(lo.dw_w - fc.taps_off)
                enc_tower_ga_dwh_cls(raw, h, r.s_ga_dwh, pc_dw, (nd + 255l) / 256l)
            } elif (fc.conv_ln) {
                enc_tower_ga_dw_cls(raw, h, r.s_ga_dw, pc_dw, (nd + 255l) / 256l)
            }
            if (fc.conv_ln) {
                vt_pt(raw, VtProfRole.dwconv)
                pc_lncv.lnoff = uint(lo.bn_w)
                pc_lncv.lnboff = uint(lo.bn_b)
                enc_tower_ln_cls(raw, h, r.s_ln_yd, pc_lncv, npos)
                vt_pt(raw, VtProfRole.ln)
                enc_tower_act_cls(raw, h, r.s_silu_yd, pc_silu, rq_wgs(false, nd))
                vt_pt(raw, VtProfRole.silu)
            } elif (fc.tap_major) {
                enc_tower_pk_dw_cls(raw, h, r.s_pk_dw, pc_dw, (nd + 255l) / 256l)
            } else {
                enc_tower_cn_dw_cls(raw, h, r.s_cn_dw, pc_dw, (nd + 255l) / 256l)
            }
            vt_pt(raw, VtProfRole.dwconv)
            var m_pw2 = vt_fc_site(r, fc, rb + VT_CN_PW2, v, r.s_o, VT_FC_OUT_O)
            vt_fc_fed_mm(raw, h, mm, r.s_rq_yd, nd, m_pw2.s, m_pw2.t, rb + VT_CN_PW2, g_d, lo.pw2_w, d, d)
            vt_pt(raw, VtProfRole.gemm_pw)
            pc_post.boff = uint(lo.pw2_b)
            pc_post.ascale = 1.0
            pc_post.lnoff = uint(lo.nf2_w)
            pc_post.lnboff = uint(lo.nf2_b)
            vt_fc_post(raw, h, fc.biased, r.s_post_o_ln, r.s_post_o_ln_nb, pc_post, npos)
            vt_pt(raw, VtProfRole.post_add)
            var m_ff2a = vt_fc_site(r, fc, rb + VT_CN_FF2A, vff, r.s_hg, VT_FC_OUT_HG)
            vt_fc_fed_mm(raw, h, mm, r.s_rq_xb, nd, m_ff2a.s, m_ff2a.t, rb + VT_CN_FF2A, g_ff, lo.ff2a_w, d, ff)
            vt_pt(raw, VtProfRole.gemm_fc1)
            vt_fc_bias(raw, h, fc.biased, r.s_bias_hg, r.s_silu_hg, ff, nff, lo.ff2a_b, BIAS_ACT_SILU)
            vt_pt(raw, VtProfRole.bias)
            var m_ff2b = vt_fc_site(r, fc, rb + VT_CN_FF2B, v, r.s_down, VT_FC_OUT_DOWN)
            vt_fc_fed_mm(raw, h, mm, r.s_rq_hg, nff, m_ff2b.s, m_ff2b.t, rb + VT_CN_FF2B, g_d, lo.ff2b_w, ff, d)
            vt_pt(raw, VtProfRole.gemm_fc2)
            pc_post.boff = uint(lo.ff2b_b)
            pc_post.ascale = 0.5
            pc_post.ln_on = 0u
            vt_fc_post(raw, h, fc.biased, r.s_post_d_ln, r.s_post_d_ln_nb, pc_post, npos)
            vt_pt(raw, VtProfRole.post_add)
            pc_post.ln_on = 1u
            pc_ln.lnoff = uint(lo.nout_w)
            pc_ln.lnboff = uint(lo.nout_b)
            enc_tower_ln_cls(raw, h, r.s_ln_x, pc_ln, npos)
            vt_pt(raw, VtProfRole.ln)
        }
        vhz_dep(raw, h, VT_X, 0u, true)
        cmd_copy_range(raw, r.x_dev, 0l, r.host.buf, 0l, bx)
        vt_pt(raw, VtProfRole.copy_out)
    }
    g_vt_fc_gen++
    g_vt_fc_out_gen = g_vt_fc_gen
    g_vt_fc_out_bytes = bx
    g_vt_fc_out_dev = r.x_dev
    return vt_served(r, x, bx, npos, fc.n_layer)
}

var private g_vt_fc_gen = 0ul       //! the served FastConformer encodes' generation, bumped by each
var private g_vt_fc_out_gen = 0ul   //! the FastConformer handoff: the served chain's residual rows still on the device (`bytes` = 0: none) - every FastConformer seat entry clears it
var private g_vt_fc_out_bytes = 0l
var private g_vt_fc_out_dev = 0ul

//! the generation of the FastConformer rows the last chain left on the device, 0 where none: a session records it with its rows
def vulkan_tower_fc_gen : uint64 => g_vt_fc_out_bytes > 0l ? g_vt_fc_out_gen : 0ul

//! the device plane holding the FastConformer encoder rows of generation `gen` (`bytes` long) where the chain that served them
//! left them and no seat has run since, or 0: the transducer decode projects them device to device
def vulkan_tower_fc_rows(gen : uint64; bytes : int64) : uint64
    => (gen != 0ul && g_vt_fc_out_bytes == bytes && g_vt_fc_out_gen == gen) ? g_vt_fc_out_dev : 0ul

//! the rotary form's attention off the normed rows in `xb`: v projects them unrotated, the rope turns them in place, q and k
//! project the rotated rows, the biased restrides pad every head onto the flash tile's width (zero lanes, the head's own
//! scale), the bidirectional flash tile, the unpad into `att`
[arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_FC.md#vk-tower-fc-rotary")]
def private vt_fc_rot_attn(raw : VkCommandBuffer; var h : VkHaz; var r : VtResident; fc : VtFc; v : VtTile; mm : VtFcMm; rb, g_d, d : int64; lo : CanaryLayerOffs;
                           cm2 : bool; var pc_rq : RqArgs; nd : int64; var pc_rope : GkRopeTabArgs; wg_rope : int64; var pc_rs : GkRestrideArgs;
                           wg_pad, wg_unpad : int64; khr : bool; hsp : int64; var pc_fa : FaArgs; wg_fa : int64) {
    var m_v = vt_fc_site(r, fc, rb + VT_CN_V, v, r.s_v, VT_FC_OUT_V)
    vt_fc_mm(raw, h, mm, m_v.s, m_v.t, rb + VT_CN_V, g_d, lo.v_w, d, d)
    vt_pt(raw, VtProfRole.gemm_v)
    enc_tower_rope_tab_cls(raw, h, r.s_rope_tq, pc_rope, wg_rope)   //! its time reads under the feed's role
    vt_feed_enc(raw, h, cm2, r.s_rq_xb, pc_rq, nd)
    vt_pt(raw, VtProfRole.rq)
    var m_q = vt_fc_site(r, fc, rb + VT_CN_Q, v, r.s_q, VT_FC_OUT_Q)
    vt_fc_mm(raw, h, mm, m_q.s, m_q.t, rb + VT_CN_Q, g_d, lo.q_w, d, d)
    vt_pt(raw, VtProfRole.gemm_q)
    var m_k = vt_fc_site(r, fc, rb + VT_CN_K, v, r.s_k, VT_FC_OUT_K)
    vt_fc_mm(raw, h, mm, m_k.s, m_k.t, rb + VT_CN_K, g_d, lo.k_w, d, d)
    vt_pt(raw, VtProfRole.gemm_k)
    vt_enc_pad_b(raw, h, r, pc_rs, wg_pad, lo.q_b, lo.k_b, lo.v_b)
    fa_stamp_enc(khr, hsp, false, false, false, false, raw, h, r.s_fa, pc_fa, wg_fa, true)
    vt_pt(raw, VtProfRole.fa)
    enc_tower_hunpad_cls(raw, h, r.s_unpad, pc_rs, wg_unpad)
    vt_pt(raw, VtProfRole.hunpad)
}

//! the biased head restrides of q, k and v onto the padded panels, each head's bias at its offset into `norms_dev`; a negative
//! `k_b` (a K projection without a bias row) restrides k bare
def private vt_enc_pad_b(raw : VkCommandBuffer; var h : VkHaz; var r : VtResident; var pc_rs : GkRestrideArgs&; wg_pad, q_b, k_b, v_b : int64) {
    pc_rs.boff = uint(q_b)
    enc_tower_hpad_b_cls(raw, h, r.s_pad_qb, pc_rs, wg_pad)
    vt_pt(raw, VtProfRole.hpad)
    if (k_b >= 0l) {
        pc_rs.boff = uint(k_b)
        enc_tower_hpad16_b_cls(raw, h, r.s_pad_kb, pc_rs, wg_pad)
    } else {
        enc_tower_hpad16_cls(raw, h, r.s_pad_k, pc_rs, wg_pad)
    }
    vt_pt(raw, VtProfRole.hpad)
    pc_rs.boff = uint(v_b)
    enc_tower_hpad16_b_cls(raw, h, r.s_pad_vb, pc_rs, wg_pad)
    vt_pt(raw, VtProfRole.hpad)
}

//! the chain's range check: the scratch's widest buffer, the rel quartet's and the rel plane's where the family carries the rel
//! table, the padded panels' where it ropes
def private vt_fc_rows_ok(rotary : bool; npos, d, ff, dp : int64) : bool
    => rotary ? vk_range_ok(vt_scratch_max_bytes(vt_cap_rows(npos), d, ff, dp, 0l, false, false)) : vt_cn_rows_ok(npos, d, ff)

[hot_path]
def private vulkan_canary_blocks(t : CanaryEncoder; var s : CanaryState; npos, ww : int64) : bool {
    var r & = g_vt_cn
    if (!vt_gate(r, t.q8)) {
        return false
    }
    return vt_decline(r, VulkanTowerDecline.flavor) if (t.dev_backend != TowerDev.vulkan)
    let fc = vt_fc_of_cn(t)
    return vt_fc_blocks(r, fc, s.x, npos, ww) && vt_fc_proj(r, fc, npos, s.out)
}

[hot_path]
def private vulkan_parakeet_blocks(m : ParakeetModel; var st : ParakeetState; tt, ww : int64) : bool {
    var r & = g_vt_pk
    g_vt_fc_out_bytes = 0l
    if (!vt_gate(r, m.q8)) {
        return false
    }
    return vt_decline(r, VulkanTowerDecline.flavor) if (m.dev_backend != TowerDev.vulkan)
    let fc = vt_fc_of_pk(m)
    return vt_fc_blocks(r, fc, st.x, tt, ww)
}

//! the parakeet encode whole where the front lever is on: the subsample front off the CPU mel, then the blocks over its rows,
//! as the CPU chain orders them; -1 with nothing counted where the front cannot serve (the CPU front runs, the block seat answers
//! for itself) or where the blocks declined behind a served front
[hot_path]
def private vulkan_parakeet_encode(m : ParakeetModel; var st : ParakeetState) : int64 {
    var r & = g_vt_pk
    g_vt_fc_out_bytes = 0l
    if (!g_vt_env_pk_front || !g_vt_env_tower) {
        vt_decline(r, VulkanTowerDecline.knob)
        return -1l
    }
    if (!m.q8) {
        vt_decline(r, VulkanTowerDecline.quant_mode)
        return -1l
    }
    if (m.dev_backend != TowerDev.vulkan) {
        vt_decline(r, VulkanTowerDecline.flavor)
        return -1l
    }
    if (vt_cn_geom(st.mel.n_len, m.n_mel).t3 > parakeet_max_frames()) {
        vt_decline(r, VulkanTowerDecline.shape)
        return -1l
    }
    let fc = vt_fc_of_pk(m)
    let tt = vt_fc_front(r, fc, unsafe(addr<float const?>(st.mel.data[0])), long_length(st.mel.data), st.mel.n_len, st.mel.n_len, st.x, true)
    if (tt < 0l || !vt_fc_blocks(r, fc, st.x, tt, 2l * tt - 1l)) {
        return -1l
    }
    g_vt_convs++
    return tt
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
    if (r.f32w) {
        vt_sets_f32(r, r.xc_dev, VT_XQ)   //! the clamped sites read the copy
        r.s_cc_xb = set_tower_clamp_copy_cls(fixed_array(r.xb_dev, r.xc_dev), fixed_array(bx, bx), fixed_array(VT_XB, VT_XQ))
        r.s_clamp_att = set_tower_clamp_cls(fixed_array(r.att_dev), fixed_array(bx), fixed_array(VT_ATT))
        r.s_clamp_hg = set_tower_clamp_cls(fixed_array(r.hg_dev), fixed_array(bff), fixed_array(VT_HG))
    }
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
    vt_sets_fa_unpad(r)
    r.s_post_o_ln = set_tower_post_add_ln_cls(fixed_array(r.x_dev, r.yo_dev, r.xb_dev, r.norms_dev), fixed_array(bx, bx, bx, r.norms_bytes), fixed_array(VT_X, VT_YO, VT_XB, 0u))
    r.s_post_d_ln = set_tower_post_add_ln_cls(fixed_array(r.x_dev, r.yd_dev, r.xb_dev, r.norms_dev), fixed_array(bx, bx, bx, r.norms_bytes), fixed_array(VT_X, VT_YD, VT_XB, 0u))
    if (r.cm2) {
        let bxh = (cap + TILE_READ_SLACK) * max(r.d, r.ff) * 2l
        r.s_ln16 = set_tower_ln16_cls(fixed_array(r.x_dev, r.norms_dev, r.xh_dev), fixed_array(bx, r.norms_bytes, bxh), fixed_array(VT_X, 0u, VT_XQ))
        r.s_post_o_ln16 = set_tower_post_add_ln16_cls(fixed_array(r.x_dev, r.yo_dev, r.norms_dev, r.xh_dev), fixed_array(bx, bx, r.norms_bytes, bxh), fixed_array(VT_X, VT_YO, 0u, VT_XQ))
        r.s_post_d_ln16 = set_tower_post_add_ln16_cls(fixed_array(r.x_dev, r.yd_dev, r.norms_dev, r.xh_dev), fixed_array(bx, bx, r.norms_bytes, bxh), fixed_array(VT_X, VT_YD, 0u, VT_XQ))
        vt_sets_pad_b(r)
        r.s_unpad16 = set_tower_hunpad16_cls(fixed_array(r.attp_dev, r.xh_dev), fixed_array(bp, bxh), fixed_array(VT_K2, VT_XQ))
        r.s_bias_hg16 = set_tower_bias_act_cvt16_cls(fixed_array(r.hg_dev, r.norms_dev, r.xh_dev), fixed_array(bff, r.norms_bytes, bxh), fixed_array(VT_HG, 0u, VT_XQ))
    }
    if (r.f32w) {
        vt_sets_f32(r, r.xb_dev, VT_XB)
    }
}

[cold_path]
def private vt_sets_f32(var r : VtResident; xin : uint64; xbit : uint) {
    let bx = r.cap_rows * r.d * 4l
    let bff = r.cap_rows * r.ff * 4l
    r.s32_q = set_f32_gemm_cls(fixed_array(r.norms_dev, xin, r.q_dev), fixed_array(r.norms_bytes, bx, bx), fixed_array(0u, xbit, VT_Q))
    r.s32_k = set_f32_gemm_cls(fixed_array(r.norms_dev, xin, r.k_dev), fixed_array(r.norms_bytes, bx, bx), fixed_array(0u, xbit, VT_K))
    r.s32_v = set_f32_gemm_cls(fixed_array(r.norms_dev, xin, r.v_dev), fixed_array(r.norms_bytes, bx, bx), fixed_array(0u, xbit, VT_V))
    r.s32_o = set_f32_gemm_cls(fixed_array(r.norms_dev, r.att_dev, r.yo_dev), fixed_array(r.norms_bytes, bx, bx), fixed_array(0u, VT_ATT, VT_YO))
    r.s32_hg = set_f32_gemm_cls(fixed_array(r.norms_dev, xin, r.hg_dev), fixed_array(r.norms_bytes, bx, bff), fixed_array(0u, xbit, VT_HG))
    r.s32_hu = set_f32_gemm_cls(fixed_array(r.norms_dev, xin, r.hu_dev), fixed_array(r.norms_bytes, bx, bff), fixed_array(0u, xbit, VT_HU))
    r.s32_down = set_f32_gemm_cls(fixed_array(r.norms_dev, r.hg_dev, r.yd_dev), fixed_array(r.norms_bytes, bff, bx), fixed_array(0u, VT_HG, VT_YD))
    if (r.fused) {
        r.s32_qkv = set_f32_gemm_cls(fixed_array(r.norms_dev, xin, r.qkv_dev), fixed_array(r.norms_bytes, bx, 3l * bx), fixed_array(0u, xbit, VT_QKV))
    }
}

//! a GEMM input's Q8_0 feed off its f32 rows, unless the chain skips it (`skip`: the cm2 feed's halves land with the norms, the exact lane reads the rows)
def private vt_rq_feed(raw : VkCommandBuffer; var h : VkHaz; skip : bool; var s_rq : VkDescriptorSet; var pc_rq : RqArgs; wgs : int64) {
    if (!skip) {
        enc_tower_clamp_rq_cls(raw, h, s_rq, pc_rq, wgs)
        vt_pt(raw, VtProfRole.rq)
    }
}

//! the exact lane's f32 tile at a block GEMM site, or the q8 lane's tile over its schedule record (`map_idx`, read only there)
def private vt_mm_lane(raw : VkCommandBuffer; var h : VkHaz; f32w : bool; var s32 : VkDescriptorSet; var pc32 : F32GemmArgs; woff : int64; wg32 : int64;
                       tile : VtTile; var s : VkDescriptorSet; var pc : BatchArgs; map_idx : int64; groups : int64) {
    if (f32w) {
        pc32.woff = uint(woff)
        enc_f32_gemm_cls(raw, h, s32, pc32, wg32)
    } else {
        pc.map_off = g_vt_maps[map_idx]
        vt_tile_enc(raw, h, tile, s, pc, groups)
    }
}

//! the f32 tile's arguments over [rows x n] -> [rows x dout]: the weight rows at `woff` with a stride of n, the bias row at `boff`, the output rows at `yoff`
def private vt_pc32(n, dout, rows : int64; woff : int64 = 0l; boff : int64 = 0l; yoff : int64 = 0l) : F32GemmArgs
    => F32GemmArgs(n = uint(n), d = uint(dout), rows = uint(rows), woff = uint(woff), wstride = uint(n), boff = uint(boff), xoff = 0u, xstride = uint(n), yoff = uint(yoff), ystride = uint(dout))

//! the image planes up for the stem (the canvas's bytes at the plane's start)
def private vt_planes_up(r : VtResident; planes : array<float>; w, h : int64) {
    upload_region_at(r.vis_planes_dev, 0l, unsafe(addr<uint8 const?>(planes[0])), w * h * 3l * 4l)
}

//! the patch im2col over the planes into the column plane at the scratch's row capacity, the pixel map as the stamp's scale and shift
def private vt_vis_im2col(raw : VkCommandBuffer; var h : VkHaz; var r : VtResident; w, patch, pd, npos : int64; scale, shift : float) {
    var pc = GkPatchIm2colArgs(w = uint(w), p = uint(patch), row_dim = uint(pd), npos = uint(npos), total = uint(r.cap_rows * pd),
        scale = scale, shift = shift)
    enc_tower_patch_im2col_cls(raw, h, r.s_vis_im2col, pc, (r.cap_rows * pd + 255l) / 256l)
    vt_pt(raw, VtProfRole.im2col)
}

//! the tail's projected rows to the host buffer behind the projection's hazard
def private vt_tail_copy_out(raw : VkCommandBuffer; var h : VkHaz; r : VtResident; bytes : int64) {
    vhz_dep(raw, h, VT_TAIL_PROJ, 0u, true)
    cmd_copy_range(raw, r.tail_proj_dev, 0l, r.tail_host.buf, 0l, bytes)
    vt_pt(raw, VtProfRole.copy_out)
}

//! a merger MLP over `nout` rows: the biased f32 tile at `w1` / `b1` into the hidden, the tanh GELU in place, the biased tile at `w2` /
//! `b2` to `dout` columns at `yoff` of the projected plane (qwen3v's taps and tail, qwen25v's tail)
def private vt_merger_enc(raw : VkCommandBuffer; var h : VkHaz; var s_fc1, s_act, s_fc2 : VkDescriptorSet; din, dout, nout, w1, b1, w2, b2, yoff : int64) {
    var pc_fc1 = vt_pc32(din, din, nout, w1, b1)
    enc_f32_gemm_b_cls(raw, h, s_fc1, pc_fc1, f32_gemm_wgs(nout, din))
    var pc_act = GkBiasActArgs(d = uint(din), nelem = uint(nout * din), boff = 0u, act = BIAS_ACT_GELU_TANH)
    enc_tower_act_cls(raw, h, s_act, pc_act, rq_wgs(false, nout * din))
    var pc_fc2 = vt_pc32(din, dout, nout, w2, b2, yoff)
    enc_f32_gemm_b_cls(raw, h, s_fc2, pc_fc2, f32_gemm_wgs(nout, dout))
    vt_pt(raw, VtProfRole.gemm_pw)
}

//! gemma4v's exact-lane clamped site: the input bounds into xc under `copy` (else in place), the f32 tile at `woff`, the output bounds under `out_cl`
def private vt_mm_clamped32(raw : VkCommandBuffer; var h : VkHaz; c : Clamp; copy : bool; var s_in : VkDescriptorSet; nin : int64;
                            var s32 : VkDescriptorSet; var pc32 : F32GemmArgs; woff : int64; wg32 : int64; out_cl : bool; var s_cl : VkDescriptorSet; nout : int64) {
    if (copy || c.active) {
        var pc_in = GkClampArgs(nelem = uint(nin), lo = vt_lo(c, true), hi = vt_hi(c, true), scale = 1.0)
        if (copy) {
            enc_tower_clamp_copy_cls(raw, h, s_in, pc_in, rq_wgs(false, nin))
        } else {
            enc_tower_clamp_cls(raw, h, s_in, pc_in, rq_wgs(false, nin))
        }
    }
    pc32.woff = uint(woff)
    enc_f32_gemm_cls(raw, h, s32, pc32, wg32)
    if (out_cl && c.active) {
        var pc_cl = GkClampArgs(nelem = uint(nout), lo = c.omin, hi = c.omax, scale = 1.0)
        enc_tower_clamp_cls(raw, h, s_cl, pc_cl, rq_wgs(false, nout))
    }
}

[cold_path]
//! a tile's set: the q8 planes, or the kq lane member's planes the tile's format names
def private vt_tile_set(r : VtResident; t : VtTile; y_dev : uint64; y_bytes : int64; y_bit : uint) : VkDescriptorSet {
    let w = max(r.d, r.ff)
    var wq = r.wq_dev
    var ws = r.ws_dev
    var wqb = r.wq_bytes
    var wsb = r.ws_bytes
    if (t.fmt != int(KqFmt.q8)) {
        let kp = r.kq_planes?[t.fmt] ?? VtKqPlane()
        wq = kp.q_dev
        ws = kp.s_dev
        wqb = kp.q_bytes
        wsb = kp.s_bytes
    }
    return vt_tile_cls_set(t, wq, ws, wqb, wsb, r.meta.buf, r.meta_bytes, r.xh_dev, (r.cap_rows + TILE_READ_SLACK) * w * 2l,
        r.xq_dev, r.xs_dev, r.cap_rows * w, VT_XQ, y_dev, y_bytes, y_bit)
}

def private vt_tile_words(t : VtTile) : string => (t.tc > 0l ? "token column {t.tc}" : "variant {t.v}") + (t.fmt != int(KqFmt.q8) ? " {kq_fmt_of_slot(t.fmt)}" : "")

[cold_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vt_sets_d(var r : VtResident; t : VtTile) : VtBatchSetsD {
    return cached(r.sets_d, vt_tile_key(t)) $() : VtBatchSetsD {
        let bx = r.cap_rows * r.d * 4l
        to_log(LOG_INFO, "dasLLAMA vulkan tower: d-wide sets at {r.cap_rows} rows - {vt_tile_words(t)} (d {r.d})\n")
        return VtBatchSetsD(s_q = vt_tile_set(r, t, r.q_dev, bx, VT_Q), s_k = vt_tile_set(r, t, r.k_dev, bx, VT_K),
            s_v = vt_tile_set(r, t, r.v_dev, bx, VT_V), s_o = vt_tile_set(r, t, r.yo_dev, bx, VT_YO), s_down = vt_tile_set(r, t, r.yd_dev, bx, VT_YD))
    }
}

[arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vt_sets_ff(var r : VtResident; t : VtTile) : VtBatchSetsFf {
    return cached(r.sets_ff, vt_tile_key(t)) $() : VtBatchSetsFf {
        return vt_sets_ff_build(r, t)
    }
}

[cold_path]
def private vt_sets_ff_build(r : VtResident; t : VtTile) : VtBatchSetsFf {
    let bff = r.cap_rows * r.ff * 4l
    to_log(LOG_INFO, "dasLLAMA vulkan tower: ff-wide sets at {r.cap_rows} rows - {vt_tile_words(t)} (ff {r.ff})\n")
    return VtBatchSetsFf(s_hg = vt_tile_set(r, t, r.hg_dev, bff, VT_HG), s_hu = vt_tile_set(r, t, r.hu_dev, bff, VT_HU))
}

[cold_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vt_set_qkv(var r : VtResident; t : VtTile) : VkDescriptorSet {
    return cached(r.sets_qkv, vt_tile_key(t)) $() : VkDescriptorSet {
        to_log(LOG_INFO, "dasLLAMA vulkan tower: fused qkv set at {r.cap_rows} rows - {vt_tile_words(t)} (3d {3l * r.d})\n")
        return vt_tile_set(r, t, r.qkv_dev, 3l * r.cap_rows * r.d * 4l, VT_QKV)
    }
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
    vt_sets_fa_unpad(r)
    r.s_post_o_ln = set_tower_post_add_ln_cls(fixed_array(r.x_dev, r.yo_dev, r.xb_dev, r.norms_dev), fixed_array(bx, bx, bx, r.norms_bytes), fixed_array(VT_X, VT_YO, VT_XB, 0u))
    r.s_post_d_ln = set_tower_post_add_ln_cls(fixed_array(r.x_dev, r.yd_dev, r.xb_dev, r.norms_dev), fixed_array(bx, bx, bx, r.norms_bytes), fixed_array(VT_X, VT_YD, VT_XB, 0u))
    if (r.f32w) {
        vt_sets_f32(r, r.xb_dev, VT_XB)
    }
}

//! the tier's want (DASLLAMA_GPU) and then the device init - the gate every chain and every twin cell reads first: nothing
//! reads `g_gpu` ahead of this answer
def public vulkan_tower_arms : bool => gpu_want_arms_tier() && vk_moe_init()

//! the arm every chain reads after its shape check: the want and the device first, then the tile family the flash arm and
//! the GEMM feed off cm2 (when the chain's GEMMs ride it, `gemm_off_cm2`) read (`vulkan_tower_tiles_on`); `ok` false counted the decline
def private vt_arm(r : VtResident; gemm_off_cm2 : bool = false) : tuple<ok : bool; khr : bool; gemm_khr : bool> {
    if (!vulkan_tower_arms()) {
        vt_decline(r, VulkanTowerDecline.device)
        return (ok = false, khr = false, gemm_khr = false)
    }
    let khr = !cm2_fa_on()
    let gemm_khr = gemm_off_cm2 && f16_gemm_khr()
    if (!vulkan_tower_tiles_on()) {
        vt_decline(r, VulkanTowerDecline.device)
        return (ok = false, khr = khr, gemm_khr = gemm_khr)
    }
    return (ok = true, khr = khr, gemm_khr = gemm_khr)
}

//! a conv front's resident armed for `n_len` frames: the device, the f16 feed and the front's `classes` (else `device`), its
//! `planes` and slab (else `memory`), then a `scratch` of `cap_floor` frames at least, rebuilt where the frames outgrew it or the
//! GEMM tile moved (else the reason it answers); ok false with the decline counted
def private vt_front_arm(var r : VtResident; n_len, cap_floor : int64; classes : block<(gemm_khr : bool) : bool>;
                         planes : block<(var rr : VtResident) : bool>;
                         scratch : block<(var rr : VtResident; cap : int64; gemm_khr : bool) : VtBuild>) : tuple<ok : bool; khr : bool; gemm_khr : bool> {
    let arm = vt_arm(r, true)
    let off = (ok = false, khr = false, gemm_khr = false)
    return off if (!arm.ok)
    if (!vt_cm2_feed() || !f16_gemm_ensure(arm.gemm_khr) || !invoke(classes, arm.gemm_khr)) {
        vt_decline(r, VulkanTowerDecline.device)
        return off
    }
    if (!invoke(planes, r)) {
        vt_decline(r, VulkanTowerDecline.memory)
        return off
    }
    if (r.front_cap < n_len || r.gemm_khr != arm.gemm_khr) {
        r.gemm_khr = arm.gemm_khr
        if (!vt_built(r, invoke(scratch, r, max(n_len, cap_floor), arm.gemm_khr))) {
            return off
        }
    }
    return arm
}

//! a FastConformer resident's GEMM planes - the halfwords on gigaam's exact lane, else the q8 planes - uploaded where its key
//! moved; false where the upload will not take
def private vt_fc_planes(var r : VtResident; fc : VtFc) : bool {
    if (fc.dev_backend == TowerDev.vulkan && !fc.f16w) {   //! the exact lane's halfwords upload as stored on the f16 path below
        let kd = vt_fold_taps(vt_key_dev(VtDevSrc(on = true, dev = fc.dev, devplan = fc.devplan), fc.blob, 0l, fc.blob.n, fc.n_layer), fc)
        return r.key == kd || (vt_upload_fc_dev(r, kd, fc) && vt_upload_taps(r, fc))
    }
    if (fc.kq_lane) {
        let lay = active_q8_repack_layout(g_active_needs_repack)
        var kk = vt_fold_taps(vt_key(fc.qblob, fc.qscales, fc.blob, 0l, fc.blob.n, 0l, fc.n_layer, lay.mr, lay.kgroup, lay.wbias), fc)
        kk = vt_fold_addr(kk, unsafe(reinterpret<void?>(fc.kq.p)))
        kk = vt_fold_int(kk, fc.kq.n)
        kk = vt_fold_int(kk, fc.ks.n)
        kk = vt_fold_int(kk, fc.kq_repacked ? 1l : 0l)
        kk = vt_fold_words(kk, plane_at(fc.kq, 0l), fc.kq.n)
        kk = vt_fold_words(kk, plane_at(fc.ks, 0l), fc.ks.n)
        return r.key == kk || (vt_upload_fc(r, kk, fc, lay) && vt_upload_taps(r, fc))
    }
    if (fc.f16w) {
        let key16 = vt_fold_taps(vt_key_f16(fc.wblob, fc.w_off, fc.w_n, fc.blob, 0l, fc.blob.n, fc.n_layer), fc)
        return r.key == key16 || (vt_upload_f16(r, key16, fc.wblob, fc.w_off, fc.w_n, fc.blob, 0l, fc.blob.n) && vt_upload_taps(r, fc))
    }
    let lay = active_q8_repack_layout(g_active_needs_repack)
    let key = vt_fold_taps(vt_key(fc.qblob, fc.qscales, fc.blob, 0l, fc.blob.n, 0l, fc.n_layer, lay.mr, lay.kgroup, lay.wbias), fc)
    return r.key == key || (vt_upload_fc(r, key, fc, lay) && vt_upload_taps(r, fc))
}

let private VT_FC_WQ_PIECE = "vk.fc.wq"            //! the q8 planes gathered: the quants
let private VT_FC_WS_PIECE = "vk.fc.ws"            //! ... and their halfword scales
let private VT_FC_KQ_SEATS_PIECE = "vk.fc.kq.seats"  //! the kq lane's record seats, two words each: the format slot, the first block
let private VT_FC_KQ_FMTS_PIECE = "vk.fc.kq.fmts"    //! the kq lane's K-quant members' format slots, a bit each, one word
let private VT_FC_FRONT16_PIECE = "vk.fc.front16"  //! the front's conv panels as one f16 slab
let private VT_GA_TWIN_PIECE = "vk.ga.twin"        //! GigaAM's device mel's twiddles and window

//! a K-quant member's device planes' piece names
def private vt_kq_piece(fmt : int; scales : bool) : string => scales ? "vk.fc.kq.{fmt}.s" : "vk.fc.kq.{fmt}.q"

//! The FastConformer chain's planes off a Vulkan image's pieces (`#tower-dev-block`): each piece uploaded as it is - the q8
//! planes, the kq lane's member planes and seats - beside the blob; false where a buffer cannot be made
[cold_path, arch(at = "../ARCHITECTURE_IMAGE_GPU_PLANES.md#tower-dev-block")]
def private vt_upload_fc_dev(var r : VtResident; key : uint64; fc : VtFc) : bool {
    let wq = tower_dev_find(fc.devplan, VT_FC_WQ_PIECE)
    let ws = tower_dev_find(fc.devplan, VT_FC_WS_PIECE)
    vt_upload_begin(r, key)
    r.wq_bytes = max(wq.bytes, 0l)
    r.ws_bytes = max(ws.bytes, 0l)
    if (r.wq_bytes > 0l) {
        r.wq_dev = vt_dev_buf(r, r.wq_bytes)
        r.ws_dev = vt_dev_buf(r, r.ws_bytes)
    }
    r.ones_off = fc.blob.n
    r.norms_bytes = fc.blob.n * 4l
    r.norms_dev = vt_dev_buf(r, r.norms_bytes)
    r.kq_seats |> clear()
    if (fc.kq_lane) {
        let sp = tower_dev_find(fc.devplan, VT_FC_KQ_SEATS_PIECE)
        let n = sp.bytes / 16l
        r.kq_seats |> reserve(n)
        r.kq_seats |> resize(n)
        unsafe {
            let w = reinterpret<int64 const?>(plane_at(fc.dev, sp.off))
            for (i in range64(n)) {
                r.kq_seats[i] = VtFcSeat(fmt = int(w[2l * i]), wblk0 = w[2l * i + 1l])
            }
        }
        for (fmt in range(KQ_FMT_COUNT)) {
            continue if ((fc.kq_fmts & (1u << uint(fmt))) == 0u)
            let q = tower_dev_find(fc.devplan, vt_kq_piece(fmt, false))
            let sc = tower_dev_find(fc.devplan, vt_kq_piece(fmt, true))
            r.kq_planes[fmt] = VtKqPlane(q_dev = vt_dev_buf(r, q.bytes), s_dev = vt_dev_buf(r, sc.bytes), q_bytes = q.bytes, s_bytes = sc.bytes)
        }
    }
    if (r.oom) {
        vt_release(r)
        return false
    }
    if (r.wq_bytes > 0l) {
        upload_region_at(r.wq_dev, 0l, plane_at(fc.dev, wq.off), r.wq_bytes)
        upload_region_at(r.ws_dev, 0l, plane_at(fc.dev, ws.off), r.ws_bytes)
    }
    upload_float_ranges(r.norms_dev, [(p = plane_at(fc.blob, 0l), n = fc.blob.n)], 0l)
    for (fmt, kp in keys(r.kq_planes), values(r.kq_planes)) {
        upload_region_at(kp.q_dev, 0l, plane_at(fc.dev, tower_dev_find(fc.devplan, vt_kq_piece(fmt, false)).off), kp.q_bytes)
        upload_region_at(kp.s_dev, 0l, plane_at(fc.dev, tower_dev_find(fc.devplan, vt_kq_piece(fmt, true)).off), kp.s_bytes)
    }
    r.cap_rows = 0l
    vt_unbind_batch_sets(r)
    return true
}

//! GigaAM's Vulkan chain serves a carrier on its lane (`e`, the lane's meta): the tower knob on, the device armed with its tiles,
//! and the f16 GEMM feed where the lane reads it - the kq lane's K-quant tiles and the exact lane's halfwords
def private vulkan_gigaam_serves_model(e : GigaamModel) : bool {
    return false if (!g_vt_env_tower || !vulkan_tower_arms() || !vulkan_tower_tiles_on())
    return e.q8 || vt_cm2_feed()
}

//! GigaAM's Vulkan device block (`#tower-dev-block`): the chain's planes, then the front's f16 conv slab with its mel tables
[cold_path, arch(at = "../ARCHITECTURE_IMAGE_GPU_PLANES.md#tower-dev-block")]
def private vulkan_gigaam_bake(m : GigaamModel; var b : TowerDevBuild) : bool {
    vt_fc_bake(vt_fc_of_ga(m), b)
    return vt_ga_front_bake(m, b)
}

//! Parakeet's Vulkan device block (`#tower-dev-block`): the chain's planes, then the front's f16 conv panel
[cold_path, arch(at = "../ARCHITECTURE_IMAGE_GPU_PLANES.md#tower-dev-block")]
def private vulkan_parakeet_bake(m : ParakeetModel; var b : TowerDevBuild) : bool {
    let fc = vt_fc_of_pk(m)
    vt_fc_bake(fc, b)
    vt_cn_front_bake(fc, b)
    return true
}

//! Canary's Vulkan device block (`#tower-dev-block`): the chain's planes with the projector's, then the front's f16 conv panel
[cold_path, arch(at = "../ARCHITECTURE_IMAGE_GPU_PLANES.md#tower-dev-block")]
def private vulkan_canary_bake(m : CanaryEncoder; var b : TowerDevBuild) : bool {
    let fc = vt_fc_of_cn(m)
    vt_fc_bake(fc, b)
    vt_cn_front_bake(fc, b)
    return true
}

//! Canary's Vulkan chain serves the lane: the q8 lane, the tower knob on, the device armed with its tiles
def private vulkan_canary_serves_model(q8 : bool) : bool => q8 && g_vt_env_tower && vulkan_tower_arms() && vulkan_tower_tiles_on()

//! Canary's projector on the device behind its blocks (`#tower-dev-block`): the residual rows fed, the projector GEMM over its
//! gathered planes into `hg`, its bias, the rows read back into `out`; false with the decline counted where the tile or the
//! readback cannot be made
[hot_path]
def private vt_fc_proj(var r : VtResident; fc : VtFc; npos : int64; var out : array<float>) : bool {
    let d = fc.d
    let pdim = fc.proj_dim
    let vp = vt_tile(vt_cm2_feed(), pdim, npos)
    return vt_decline(r, VulkanTowerDecline.shape) if (pdim > fc.ff)
    return vt_decline(r, VulkanTowerDecline.device) if (!vt_tile_ensure(vp))
    let bytes = npos * pdim * 4l
    if (r.phost_bytes < bytes) {
        vt_proj_host(r, vt_cap_rows(npos) * pdim * 4l)
        return vt_decline(r, VulkanTowerDecline.memory) if (r.phost_bytes < bytes)
    }
    let groups = vt_tile_groups(vp, pdim, npos)
    let rows = vt_tile_rows(vp, npos)
    let wblk0 = fc.proj_w / kq_elems(KqFmt.q8)
    vt_sched_walk(g_vt_maps, r.meta, 1l) $ [unused_argument(rid)] (rid : int64) => VtSchedRec(wblk0 = wblk0, rows = rows, groups = groups)
    var s_p = vt_sets_ff(r, vp).s_hg
    var s_none : VkDescriptorSet
    let mm = VtFcMm(npos = npos)
    vt_chain_one_shot("{fc.family} projector", 1l, npos) $(raw : VkCommandBuffer; var h : VkHaz) {
        vt_fc_fed_mm(raw, h, mm, r.s_rq_x, npos * d, s_p, vp, 0l, groups, fc.proj_w, d, pdim)
        vt_fc_bias(raw, h, true, r.s_bias_hg, s_none, pdim, npos * pdim, fc.proj_b, BIAS_ACT_NONE)
        vhz_dep(raw, h, VT_HG, 0u, true)
        cmd_copy_range(raw, r.hg_dev, 0l, r.phost.buf, 0l, bytes)
    }
    ensure_length(out, npos * pdim)
    host_buf_read(r.phost, unsafe(addr<void?>(out[0])), bytes)
    return true
}

//! the projector's readback staging grown to `bytes`; `phost_bytes` stays 0 where the allocation failed
[cold_path]
def private vt_proj_host(var r : VtResident; bytes : int64) {
    if (r.phost_bytes > 0l) {
        release_host_buf(r.phost)
        r.phost_bytes = 0l
    }
    r.oom = false
    r.phost = vt_host_buf(r, bytes, true)
    r.phost_bytes = r.oom ? 0l : bytes
}

//! Parakeet's Vulkan chain serves the lane: the q8 lane, the tower knob and the front lever on, the device armed with its tiles
def private vulkan_parakeet_serves_model(q8 : bool) : bool
    => q8 && g_vt_env_tower && g_vt_env_pk_front && vulkan_tower_arms() && vulkan_tower_tiles_on()

//! A FastConformer chain's planes for its Vulkan device block: the gathers the upload runs - the q8 planes, the kq lane's member
//! planes and seats - each into its piece; the CPU form's planes row-major
[cold_path]
def private vt_fc_bake(fc : VtFc; var b : TowerDevBuild) {
    let lay = active_q8_repack_layout(false)
    var wq, ws : array<uint8>
    if (fc.kq_lane) {
        var inscope p <- vt_fc_kq_plan(fc)
        if (fc.qblob.n > 0l) {
            wq |> reserve_resize(fc.qblob.n)
            ws |> reserve_resize(fc.qscales.n * 2l)
            vt_gather_regions(fc.qblob, fc.qscales, p.regions, lay, wq, ws)
        }
        var seats : array<int64>
        for (s in p.seats) {
            seats |> push_from([int64(s.fmt), s.wblk0])
        }
        tower_dev_put(b, VT_FC_KQ_SEATS_PIECE, unsafe(addr<uint8 const?>(seats[0])), long_length(seats) * 8l)
        delete seats
        let fmts = [int64(fc.kq_fmts)]
        tower_dev_put(b, VT_FC_KQ_FMTS_PIECE, unsafe(addr<uint8 const?>(fmts[0])), 8l)
        for (fmt, qb in keys(p.qbytes), values(p.qbytes)) {
            var kq, ks : array<uint8>
            vt_gather_kq_member(fc, fmt, p.seats, qb, p.sbytes?[fmt] ?? 0l, kq, ks)
            tower_dev_put(b, vt_kq_piece(fmt, false), unsafe(addr<uint8 const?>(kq[0])), long_length(kq))
            tower_dev_put(b, vt_kq_piece(fmt, true), unsafe(addr<uint8 const?>(ks[0])), long_length(ks))
            delete kq
            delete ks
        }
    } elif (fc.q8) {
        var inscope regions <- vt_fc_q8_regions(fc)
        if (fc.proj_w >= 0l) {
            regions |> push(Q8Region(woff = fc.proj_w, n = fc.d, rows = fc.proj_dim))
        }
        wq |> reserve_resize(fc.qblob.n)
        ws |> reserve_resize(fc.qscales.n * 2l)
        vt_gather_regions(fc.qblob, fc.qscales, regions, lay, wq, ws)
    }
    if (!empty(wq)) {
        tower_dev_put(b, VT_FC_WQ_PIECE, unsafe(addr<uint8 const?>(wq[0])), long_length(wq))
        tower_dev_put(b, VT_FC_WS_PIECE, unsafe(addr<uint8 const?>(ws[0])), long_length(ws))
    }
    delete wq
    delete ws
}

//! GigaAM's front for its Vulkan device block: the conv panels as the f16 slab, the device mel's twiddles and window
[cold_path]
def private vt_ga_front_bake(m : GigaamModel; var b : TowerDevBuild) : bool {
    let k = m.subs_kernel
    let nch = m.n_sub_ch
    let d = m.d_model
    let kd0 = round_up(k * m.n_mel, 64l)
    let kd2 = round_up(k * nch, 64l)
    let n_bins = m.n_fft / 2l + 1l
    let ntw = m.n_fft * 2l * n_bins
    return false if (long_length(m.tabs.twiddles) < ntw || length(m.window) < m.n_fft || length(m.filters) < m.n_mel * n_bins)
    var slab : array<float>
    slab |> reserve_resize(nch * kd0 + d * kd2)
    tower_stage_f32_pad_row_stride(slab, m.blob, m.c0_w, 0l, nch, k * m.n_mel, kd0)
    tower_stage_f32_pad_row_stride(slab, m.blob, m.c2_w, nch * kd0, d, k * nch, kd2)
    var inscope slab16 <- cvt_f32_to_f16_sat_array(slab)
    delete slab
    tower_dev_put(b, VT_FC_FRONT16_PIECE, unsafe(addr<uint8 const?>(slab16[0])), long_length(slab16) * 2l)
    var inscope twin : array<float>
    twin |> reserve(ntw + m.n_fft)
    for (i in range64(ntw)) {
        twin |> push(m.tabs.twiddles[i])
    }
    for (j in range64(m.n_fft)) {
        twin |> push(m.window[j])
    }
    tower_dev_put(b, VT_GA_TWIN_PIECE, unsafe(addr<uint8 const?>(twin[0])), long_length(twin) * 4l)
    return true
}

//! the resident key with the family's f16 taps folded in: their plane, region and words
def private vt_fold_taps(k : uint64; fc : VtFc) : uint64 {
    return k if (fc.taps_n == 0l)
    var kt = vt_fold_addr(k, unsafe(reinterpret<void?>(fc.taps.p)))
    kt = vt_fold_int(kt, fc.taps_off)
    kt = vt_fold_int(kt, fc.taps_n)
    return vt_fold_words(kt, unsafe(reinterpret<uint8 const?>(plane_at(fc.taps, fc.taps_off))), fc.taps_n * 2l)
}

//! the family's f16 depthwise taps uploaded as its own halfwords behind a fresh plane upload; nothing where the taps are f32
[cold_path]
def private vt_upload_taps(var r : VtResident; fc : VtFc) : bool {
    return true if (fc.taps_n == 0l)
    r.taps_bytes = fc.taps_n * 2l
    r.taps_dev = vt_dev_buf(r, r.taps_bytes)
    if (r.oom) {
        vt_release(r)
        return false
    }
    upload_region_at(r.taps_dev, 0l, unsafe(reinterpret<uint8 const?>(plane_at(fc.taps, fc.taps_off))), r.taps_bytes)
    return true
}

//! the encode's landing, one text for every chain: the residual rows back to the host, the counters, the announcement once
def private vt_served(var r : VtResident; var x : array<float>; bx, npos, n_layer : int64) : bool
    => vt_served_from(r, r.host, x, bx, npos, n_layer)

def private vt_served_from(var r : VtResident; hb : HostBuf; var x : array<float>; bx, npos, n_layer : int64) : bool {
    host_buf_read(hb, unsafe(addr<void?>(x[0])), bx)
    g_vt_encodes++
    g_vt_rows += npos
    g_vt_blocks += n_layer
    vt_announce(r)
    return true
}

[cold_path]
def private vt_upload_ln(var r : VtResident; key : uint64; qblob : PlaneI8; qscales : PlaneF; d, ff : int64;
                         blob : PlaneF; rows_off, rows_n : int64; lay : Q8RepackLayout; offs : array<LayerOffs>; conv2_w : int64 = -1l;
                         proj : array<Q8Region> = g_vt_no_regions; src : VtDevSrc = VtDevSrc(); prefix : string = "") : bool {
    var inscope regions <- vt_ln_regions(d, ff, offs, conv2_w, proj)
    return vt_upload(r, key, qblob, qscales, regions, blob, rows_off, rows_n, 0l, lay, src, prefix)
}

//! a pre-LN chain's q8 regions: the projector's, the conv stem's second conv where it has one, then every block's GEMMs
def private vt_ln_regions(d, ff : int64; offs : array<LayerOffs>; conv2_w : int64; proj : array<Q8Region>) : array<Q8Region> {
    var regions : array<Q8Region>
    regions |> reserve(long_length(offs) * LN_BLOCK_GEMMS + 1l + long_length(proj))
    regions |> push_from(proj)
    if (conv2_w >= 0l) {
        regions |> push(Q8Region(woff = conv2_w, n = 3l * d, rows = d))
    }
    var recs : Q8Region[int(LN_BLOCK_GEMMS)]
    for (o in offs) {
        ln_block_regions(o, d, ff, recs)
        regions |> push_from(recs)
    }
    return <- regions
}

//! fc2's map words are counted apart from the d-wide GEMMs': under a split its map stacks every plane's tiles
def private vt_ln_meta_bytes(nl, g_d, g_ff, g_fc2 : int64) : int64 => (nl * LN_BLOCK_GEMMS * 4l + nl * (4l * g_d + g_ff + g_fc2)) * 4l

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
        release_device_buf(r.sk_dev)
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
    if (!vk_range_ok(need) || (r.sk_bytes < need && !vt_sk_build(r, nsplit, need))) {
        return false
    }
    cached(r.sets_sk, int(tc)) $() : VkDescriptorSet {
        return vt_tile_set(r, VtTile(tc = tc), r.sk_dev, r.sk_bytes, VT_SPLITK_PLANES)
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

def private vt_ln_scratch(var r : VtResident; nl, npos, d, ff, hs, dp, g_d, g_ff : int64; khr, cm2 : bool; tc_d, tc_ff : int64; v, vff : int; extra_meta : int64;
                          f32w : bool = false) : bool {
    let meta_bytes = vt_ln_meta_bytes(nl, g_d, g_ff, vt_ln_fc2_groups(d, ff, npos, tc_d, cm2, g_d)) + extra_meta
    if (!vt_scratch_fits(r, npos, d, ff, dp, meta_bytes, khr, false, 0l, f32w)) {
        if (!vt_scratch_build(r, npos, d, ff, hs, dp, meta_bytes, khr, false, 0l, f32w)) {
            return false
        }
        vt_sets_ln(r)
    }
    if (!f32w) {
        vt_bind_tile_sets(r, VtTile(v = v, tc = tc_d), VtTile(v = vff, tc = tc_ff))
    }
    return true
}

//! the vision tail behind the blocks a whole chain asks of `vt_ln_chain`: the post-normed rows (in xb) pooled over the patch grid,
//! normed by the soft-token rms row at `soft_norm_w`, projected by the f32 rows at `proj_w`; the soft tokens land in the chain's `out`
struct private VtVisTail {
    on : bool
    nout : int64        //! the soft tokens, (gw / merge) x (gh / merge)
    gw : int64          //! the patch grid's width in rows
    merge : int64       //! the pool's side
    d : int64
    proj : int64        //! the soft-token width
    soft_norm_w : int64 //! the soft rms's weight row in the norms plane, -1 for the weightless rms off the ones row (gemma4v)
    proj_w : int64      //! the projection's rows: in the norms plane (gemma3v), in the stem slab (gemma4v)
    eps : float         //! the soft rms's eps, the tower's
    affine : bool       //! gemma4v: the sqrt(d) scale with the standardize rows between the pool and the rms
    scale : float
    std : bool
    std_bias : int64
    std_scale : int64
    clamp : Clamp       //! gemma4v: the projection's input and output bounds (inactive elsewhere)
}

//! the vision planes at the canvas and the scratch's rows, and the tail's buffers: a rebuild where a shape or a slab size moved, else nothing
def private vt_vis_bufs(var r : VtResident; w, h, pd, vw_bytes, idx_bytes, aux_bytes, tail_dim : int64) : bool {
    let cap = r.cap_rows
    let bpl = w * h * 3l * 4l
    if (r.vis_cap != cap || r.vis_pd != pd || r.vis_planes_bytes < bpl || r.vis_w_bytes != vw_bytes || r.vis_idx_bytes != idx_bytes || r.vis_aux_bytes != aux_bytes) {
        return false if (!vt_vis_grow(r, bpl, pd, vw_bytes, idx_bytes, aux_bytes))
    }
    if (r.tail_cap != cap || r.tail_dim != tail_dim) {
        return false if (!vt_vis_ensures() || !vt_tail_bufs(r, tail_dim))
        r.vis_gen++
    }
    return true
}

//! the stamps the vision and tail sets bind
[cold_path]
def private vt_vis_ensures() : bool
    => ensure_tower_clamp_rq_cls() && ensure_tower_clamp_cvt_cls() && ensure_tower_bias_act_cls() && ensure_tower_rms_cls() && ensure_tower_patch_im2col_cls()

[cold_path]
def private vt_vis_grow(var r : VtResident; bpl, pd, vw_bytes, idx_bytes, aux_bytes : int64) : bool {
    return false if (!vt_vis_ensures())
    let cap = r.cap_rows
    let bcol = cap * pd * 4l
    vt_vis_release(r)
    r.oom = false
    r.vis_cap = cap   //! before the allocations: the oom path's release frees what did allocate
    r.vis_planes_dev = vt_call_buf(r, bpl)
    r.vis_col_dev = vt_dev_buf(r, bcol)
    r.vis_w_dev = vw_bytes > 0l ? vt_dev_buf(r, vw_bytes) : 0ul
    r.vis_idx_dev = idx_bytes > 0l ? vt_call_buf(r, idx_bytes) : 0ul
    r.vis_aux_dev = aux_bytes > 0l ? vt_call_buf(r, aux_bytes) : 0ul
    if (r.oom) {
        vt_vis_release(r)
        return false
    }
    r.vis_pd = pd
    r.vis_planes_bytes = bpl
    r.vis_col_bytes = bcol
    r.vis_w_bytes = vw_bytes
    r.vis_idx_bytes = idx_bytes
    r.vis_aux_bytes = aux_bytes
    r.vis_w_fresh = vw_bytes > 0l
    r.s_vis_im2col = set_tower_patch_im2col_cls(fixed_array(r.vis_planes_dev, r.vis_col_dev), fixed_array(bpl, bcol), fixed_array(VT_STEM_MEL, VT_STEM_COL))
    r.vis_gen++
    return true
}

[cold_path]
def private vt_vis_release(var r : VtResident) {
    if (r.vis_cap > 0l) {
        release_device_buf(r.vis_planes_dev)
        release_device_buf(r.vis_col_dev)
        release_device_buf(r.vis_w_dev)
        release_device_buf(r.vis_idx_dev)
        release_device_buf(r.vis_aux_dev)
        vt_vis_forget(r)
    }
}

//! the vision marks cleared, nothing destroyed: the release after its destroys, the forget path alone (the device's sweep freed the buffers)
[cold_path]
def private vt_vis_forget(var r : VtResident) {
    r.vis_cap = 0l
    r.vis_pd = 0l
    r.vis_planes_bytes = 0l
    r.vis_w_bytes = 0l
    r.vis_idx_bytes = 0l
    r.vis_aux_bytes = 0l
    r.vis_w_fresh = false
    r.vis_gen++
}

def private vt_rows_f32(blob : PlaneF; wblob : PlaneU16; bf16 : bool; off, n : int64) : array<float> {
    var @exact_size rows : array<float>
    rows |> reserve_resize(n)
    unsafe {
        if (bf16) {
            widen_bf16(addr(rows[0]), wblob.p + off, n)
        } else {
            memcpy(addr(rows[0]), plane_at(blob, off), n * 4l)
        }
    }
    return <- rows
}

def private vt_vis_tail_enc(raw : VkCommandBuffer; var h : VkHaz; var r : VtResident; vis : VtVisTail) {
    let nd = vis.nout * vis.d
    var pc_pool = GkPool2dArgs(width = uint(vis.d), gw = uint(vis.gw), k = uint(vis.merge), total = uint(nd))
    enc_tower_pool_2d_cls(raw, h, r.s_vis_pool, pc_pool, (nd + 255l) / 256l)
    vt_pt(raw, VtProfRole.pool)
    if (vis.affine) {
        var pc_aff = GkAffineRowsArgs(width = uint(vis.d), total = uint(nd), scale = vis.scale, boff = uint(vis.std ? vis.std_bias : 0l),
            moff = uint(vis.std ? vis.std_scale : 0l), std = vis.std ? 1u : 0u)
        enc_tower_affine_rows_cls(raw, h, r.s_vis_affine, pc_aff, (nd + 255l) / 256l)
    }
    var pc_rms = gk_rms_args(uint(vis.d), uint(vis.nout), vis.eps, uint(vis.soft_norm_w >= 0l ? vis.soft_norm_w : r.ones_off))
    enc_tower_rms_cls(raw, h, r.s_rms_pt, pc_rms, vis.nout)
    vt_pt(raw, VtProfRole.rms)
    if (vis.clamp.active) {
        var pc_ci = GkClampArgs(nelem = uint(nd), lo = vis.clamp.imin, hi = vis.clamp.imax, scale = 1.0)
        enc_tower_clamp_cls(raw, h, r.s_vis_clamp_in, pc_ci, rq_wgs(false, nd))
    }
    var pc_proj = vt_pc32(vis.d, vis.proj, vis.nout, vis.proj_w)
    enc_f32_gemm_cls(raw, h, r.s_vis_proj, pc_proj, f32_gemm_wgs(vis.nout, vis.proj))
    vt_pt(raw, VtProfRole.gemm_pw)
    if (vis.clamp.active) {
        var pc_co = GkClampArgs(nelem = uint(vis.nout * vis.proj), lo = vis.clamp.omin, hi = vis.clamp.omax, scale = 1.0)
        enc_tower_clamp_cls(raw, h, r.s_vis_clamp_out, pc_co, rq_wgs(false, vis.nout * vis.proj))
    }
}

//! the caller keys `r` first; `lnp_w` >= 0 folds the post-norm (weight at `lnp_w`, bias at `lnp_b`, in the norms plane) into the
//! last post-add, landing f32 rows in `xb`; else in `x`
[arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vt_ln_chain(var r : VtResident; offs : array<LayerOffs>; k_bias : bool; fc1_act : uint; eps : float;   // nolint:STYLE038 — the dispatch chain is the shape
                        norms_base, d, ff, hs, hsp, nheads, npos : int64; khr : bool; v, vff : int; var x : array<float>;
                        lnp_w : int64 = -1l; lnp_b : int64 = -1l; var xb : array<float>& = g_vt_no_rows; var tl : VtChatTail = VtChatTail();
                        var out : array<float>& = g_vt_no_rows; vis : VtVisTail = VtVisTail(); f32w : bool = false) : bool {
    let ln_post = lnp_w >= 0l
    let q8e = kq_elems(KqFmt.q8)
    let nl = long_length(offs)
    let dp = nheads * hsp
    let cm2 = !f32w && vt_cm2_feed()
    let tc_d = cm2 ? cm2_tile_cols(d, npos) : 0l
    let tc_ff = cm2 ? cm2_tile_cols(ff, npos) : 0l
    if (cm2 && (!cm2_cls_ensure(int(KqFmt.q8), tc_d) || !cm2_cls_ensure(int(KqFmt.q8), tc_ff) || !vt_ln16_ensure())) {
        return vt_flush_and_decline(r, VulkanTowerDecline.device)
    }
    let g_d = cm2 ? cm2_tiles_at(d, npos, tc_d) : vt_gemm_groups(d, npos)
    let g_ff = cm2 ? cm2_tiles_at(ff, npos, tc_ff) : vt_gemm_groups(ff, npos)
    let nrec_blocks = nl * LN_BLOCK_GEMMS
    let nrec = nrec_blocks + vt_ct_gemms(tl)
    var sk = vt_ln_fc2_pick(d, ff, npos, vt_cap_rows(npos), tc_d, cm2)
    if (!vt_ln_scratch(r, nl, npos, d, ff, hs, dp, g_d, g_ff, khr, cm2, tc_d, tc_ff, v, vff, vt_ct_meta_bytes(tl), f32w)) {
        return vt_flush_and_decline(r, VulkanTowerDecline.memory)
    }
    if (sk.nsplit > 1l && !vt_sk_ensure(r, sk.tc, sk.nsplit)) {
        sk = (tc = tc_d, nsplit = 1l, ksplit = 0l)
    }
    if (tl.on && !vt_ct_ready(r, tl)) {
        return vt_flush_and_decline(r, VulkanTowerDecline.memory)
    }
    let split = sk.nsplit > 1l
    let g_fc2 = split ? cm2_tiles_at(d, npos, sk.tc) * sk.nsplit : g_d
    var s_fc2 = split ? r.sets_sk[int(sk.tc)] : r.s_down
    let tile_d = VtTile(v = v, tc = tc_d)
    let tile_ff = VtTile(v = vff, tc = tc_ff)
    let tile_fc2 = VtTile(v = v, tc = cm2 ? sk.tc : 0l)
    var recs : Q8Region[int(LN_BLOCK_GEMMS)]
    if (!f32w) {
        vt_sched_walk(g_vt_maps, r.meta, nrec) $(rid : int64) : VtSchedRec {
            if (rid >= nrec_blocks) {
                let second = rid > nrec_blocks
                return VtSchedRec(wblk0 = (second ? tl.w2 : tl.w1) / q8e, rows = vt_tile_rows(second ? tl.tile2 : tl.tile1, tl.rows), groups = second ? tl.g2 : tl.g1)
            }
            let g = rid % LN_BLOCK_GEMMS
            if (g == 0l) {
                ln_block_regions(offs[rid / LN_BLOCK_GEMMS], d, ff, recs)
            }
            let tile = g == LN_GEMM_FC1 ? tile_ff : (g == LN_GEMM_FC2 ? tile_fc2 : tile_d)
            return VtSchedRec(wblk0 = recs[g].woff / q8e, rows = vt_tile_rows(tile, npos), groups = g == LN_GEMM_FC1 ? g_ff : (g == LN_GEMM_FC2 ? g_fc2 : g_d))
        }
    }
    var pc32_d = vt_pc32(d, d, npos)
    var pc32_ff = vt_pc32(d, ff, npos)
    var pc32_dn = vt_pc32(ff, d, npos)
    let wg32_d = f32_gemm_wgs(npos, d)
    let wg32_ff = f32_gemm_wgs(npos, ff)
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
    var pc_rs = GkRestrideArgs(rows = uint(npos), rows_pad = uint(npos_pad), nheads = uint(nheads), hs = uint(hs), hsp = uint(hsp), sstride = uint(d), soff = 0u)
    var pc_rq = RqArgs(inbase = 0u, nblk = uint(nd / q8e), lo = -VT_NO_CLAMP, hi = VT_NO_CLAMP)
    var pc_rqh = RqArgs(inbase = 0u, nblk = uint(nff / q8e), lo = -VT_NO_CLAMP, hi = VT_NO_CLAMP)
    var pc_gd = BatchArgs(n = uint(d), d = uint(d), map_off = 0u)
    var pc_gff = BatchArgs(n = uint(d), d = uint(ff), map_off = 0u)
    var pc_dn = BatchArgs(n = uint(ff), d = uint(d), map_off = 0u, ksplit = uint(sk.ksplit))
    let nd_fc2 = vt_tile_rows(tile_fc2, npos) * d
    var pc_bias = GkBiasActArgs(d = uint(d), nelem = uint(nd), boff = 0u, act = BIAS_ACT_NONE)
    var pc_gelu = GkBiasActArgs(d = uint(ff), nelem = uint(nff), boff = 0u, act = fc1_act)
    vt_chain_one_shot("whisper-class", nl, npos) $(raw : VkCommandBuffer; var h : VkHaz) {
        for (l in range64(nl)) {
            let o = offs[l]
            let rb = l * LN_BLOCK_GEMMS
            if (l == 0l) {
                var pc = gk_ln_args(uint(d), uint(npos), eps, uint(o.ln1_w - norms_base), uint(o.ln1_b - norms_base))
                if (cm2) {
                    enc_tower_ln16_cls(raw, h, r.s_ln16, pc, npos)
                } else {
                    enc_tower_ln_cls(raw, h, r.s_ln, pc, npos)
                }
                vt_pt(raw, VtProfRole.ln)
            }
            vt_rq_feed(raw, h, cm2 || f32w, r.s_rq_xb, pc_rq, wg_nd)
            vt_mm_lane(raw, h, f32w, r.s32_q, pc32_d, o.q_w - norms_base, wg32_d, tile_d, r.s_q, pc_gd, rb + LN_GEMM_Q, g_d)
            vt_pt(raw, VtProfRole.gemm_q)
            vt_mm_lane(raw, h, f32w, r.s32_k, pc32_d, o.k_w - norms_base, wg32_d, tile_d, r.s_k, pc_gd, rb + LN_GEMM_K, g_d)
            vt_pt(raw, VtProfRole.gemm_k)
            vt_mm_lane(raw, h, f32w, r.s32_v, pc32_d, o.v_w - norms_base, wg32_d, tile_d, r.s_v, pc_gd, rb + LN_GEMM_V, g_d)
            vt_pt(raw, VtProfRole.gemm_v)
            if (cm2) {
                vt_enc_pad_b(raw, h, r, pc_rs, wg_pad, o.q_b - norms_base, k_bias ? o.k_b - norms_base : -1l, o.v_b - norms_base)
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
                vt_rq_feed(raw, h, f32w, r.s_rq_att, pc_rq, wg_nd)
            }
            vt_mm_lane(raw, h, f32w, r.s32_o, pc32_d, o.o_w - norms_base, wg32_d, tile_d, r.s_o, pc_gd, rb + LN_GEMM_O, g_d)
            vt_pt(raw, VtProfRole.gemm_o)
            var pc_post = GkPostAddLnArgs(d = uint(d), rows = uint(npos), boff = uint(o.o_b - norms_base), eps = eps,
                ln_on = 1u, lnoff = uint(o.ln2_w - norms_base), lnboff = uint(o.ln2_b - norms_base), ascale = 1.0, xboff = 0u)
            if (cm2) {
                enc_tower_post_add_ln16_cls(raw, h, r.s_post_o_ln16, pc_post, npos)
            } else {
                enc_tower_post_add_ln_cls(raw, h, r.s_post_o_ln, pc_post, npos)
                vt_rq_feed(raw, h, f32w, r.s_rq_xb, pc_rq, wg_nd)
            }
            vt_pt(raw, VtProfRole.post_add)
            vt_mm_lane(raw, h, f32w, r.s32_hg, pc32_ff, o.fc1_w - norms_base, wg32_ff, tile_ff, r.s_hg, pc_gff, rb + LN_GEMM_FC1, g_ff)
            vt_pt(raw, VtProfRole.gemm_fc1)
            pc_gelu.boff = uint(o.fc1_b - norms_base)
            if (cm2) {
                enc_tower_bias_act_cvt16_cls(raw, h, r.s_bias_hg16, pc_gelu, wg_nff)
                vt_pt(raw, VtProfRole.bias)
            } else {
                enc_tower_bias_act_cls(raw, h, r.s_bias_hg, pc_gelu, wg_nff)
                vt_pt(raw, VtProfRole.bias)
                vt_rq_feed(raw, h, f32w, r.s_rq_hg, pc_rqh, wg_nff)
            }
            vt_mm_lane(raw, h, f32w, r.s32_down, pc32_dn, o.fc2_w - norms_base, wg32_d, tile_fc2, s_fc2, pc_dn, rb + LN_GEMM_FC2, g_fc2)
            vt_pt(raw, VtProfRole.gemm_fc2)
            if (split) {
                splitk_reduce_enc(raw, h, r.s_splitk_reduce, nd_fc2, sk.nsplit)
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
        if (vis.on) {
            vt_vis_tail_enc(raw, h, r, vis)
            vt_tail_copy_out(raw, h, r, vis.nout * vis.proj * 4l)
        } elif (tl.on) {
            vt_ct_enc(raw, h, r, tl, d, nrec_blocks, cm2)
            vhz_dep(raw, h, VT_TAIL_OUT, 0u, true)
            cmd_copy_range(raw, r.ct_out_dev, 0l, r.ct_host.buf, 0l, tl.rows * tl.out * 4l)
        } else {
            vhz_dep(raw, h, ln_post ? VT_XB : VT_X, 0u, true)
            cmd_copy_range(raw, ln_post ? r.xb_dev : r.x_dev, 0l, r.host.buf, 0l, bx)
        }
        vt_pt(raw, VtProfRole.copy_out)
    }
    if (vis.on) {
        g_vt_tail_encodes++
        return vt_served_from(r, r.tail_host, out, vis.nout * vis.proj * 4l, npos, nl)
    }
    if (tl.on) {
        g_vt_tail_encodes++
        return vt_served_from(r, r.ct_host, out, tl.rows * tl.out * 4l, npos, nl)
    }
    if (ln_post) {
        g_vt_aud_out_ptr = intptr(unsafe(addr(xb[0])))
        g_vt_aud_out_bytes = bx
        g_vt_aud_out_dev = r.xb_dev
        return vt_served(r, xb, bx, npos, nl)
    }
    return vt_served(r, x, bx, npos, nl)
}

var private g_vt_no_rows : array<float>   //! the chain's xb default where no post-norm lands, its out default where no tail
var private g_vt_no_regions : array<Q8Region>   //! the upload's projector default where a tower has none
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

//! the chat projector tail behind the whisper-class blocks: what the chain dispatches after its last block (`vt_ct_enc`), the
//! CPU tail's steps at its widths (`audio_projector_tail`), on the record the hook prepares per encode
struct private VtChatTail {
    on : bool
    kind : AudioProjKind
    rows : int64        //! the soft tokens a chunk
    pool_rows : int64   //! the rows the post-norm reads: the pooled pairs (qwen2a, voxtral), every position (ultravox)
    m1in : int64        //! the first GEMM's K - the stacked row
    n1out : int64       //! its width
    m2in : int64        //! the second GEMM's K (0 = one GEMM)
    out : int64         //! the soft tokens' width
    tile1 : VtTile
    tile2 : VtTile
    g1 : int64
    g2 : int64
    w1 : int64          //! the GEMMs' weight offsets in the q8 plane
    w2 : int64
    lnoff : uint        //! the post-norm rows, the ultravox norm rows and qwen2a's bias row in the norms plane
    lnboff : uint
    normpre : uint
    normmid : uint
    proj_b : uint
    eps : float
    s_mm1 : VkDescriptorSet
    s_mm2 : VkDescriptorSet
}

let private VT_CT_MM1_KEY = 8000   //! the chat tail's GEMM sets in `sets_front`: mm1 at this + the tile key, mm2 at the next base
let private VT_CT_MM2_KEY = 9500

//! the tail's GEMMs on the q8 plane at the tiles' lattice: every K and N a 64-multiple (a K a 32-multiple is what `tower_quantize_`
//! quantizes, so these planes are q8 on a q8 tower)
def private vt_ct_planes_ok(t : AudioTower) : bool {
    return false if (t.proj_dim <= 0l || t.proj_dim % 64l != 0l)
    return true if (t.proj_kind == AudioProjKind.qwen2a)
    return (t.d_model * t.stack_factor) % 64l == 0l && t.mm1_out % 64l == 0l && audio_proj_mid_width(t) % 64l == 0l
}

//! whether this encode's tail rides the chain: the planes, the pooled rows even (the stacked rows floor as the CPU tail floors them),
//! the feeds inside the scratch
def private vt_ct_shape_ok(t : AudioTower; npos : int64) : bool {
    return false if (!vt_ct_planes_ok(t))
    let pool = audio_has_avgpool(t)
    let rows = audio_tokens_per_chunk(t)   //! the CPU tail's count over the chunk, which is the tower's n_ctx
    let m1in = t.d_model * t.stack_factor
    return (npos == t.n_ctx && (!pool || npos % 2l == 0l) && rows > 0l
        && vt_cap_rows(rows) * max(m1in, audio_proj_mid_width(t)) <= vt_cap_rows(npos) * max(t.d_model, t.n_ff))
}

//! the projector planes the upload gathers beside the block records, in the repack's (K, rows) shape
[cold_path]
def private vt_ct_regions(t : AudioTower; var regions : array<Q8Region>) {
    return if (!vt_ct_planes_ok(t))
    if (t.proj_kind == AudioProjKind.qwen2a) {
        regions |> push(Q8Region(woff = t.proj_w_off, n = t.d_model, rows = t.proj_dim))
        return
    }
    regions |> push(Q8Region(woff = t.proj_w_off, n = t.d_model * t.stack_factor, rows = t.mm1_out))
    regions |> push(Q8Region(woff = t.mm2_w_off, n = audio_proj_mid_width(t), rows = t.proj_dim))
}

//! this encode's tail record: the row counts, the GEMM tiles and groups, the norms-plane offsets; `on` false where a class refused
//! (the blocks alone serve, the CPU tail reads their rows)
def private vt_ct_prepare(t : AudioTower; npos : int64; cm2 : bool) : VtChatTail {
    var ct = VtChatTail(kind = t.proj_kind, eps = t.eps, out = t.proj_dim, w1 = t.proj_w_off, w2 = t.mm2_w_off)
    let uv = t.proj_kind == AudioProjKind.ultravox
    let one = t.proj_kind == AudioProjKind.qwen2a
    ct.pool_rows = uv ? npos : npos / 2l
    ct.rows = audio_tokens_per_chunk(t)
    ct.m1in = t.d_model * t.stack_factor
    ct.n1out = one ? t.proj_dim : t.mm1_out
    ct.m2in = one ? 0l : audio_proj_mid_width(t)
    ct.lnoff = uint(t.lnp_w_off - t.layers_off)
    ct.lnboff = uint(t.lnp_b_off - t.layers_off)
    ct.normpre = uint(t.normpre_off - t.layers_off)
    ct.normmid = uint(t.normmid_off - t.layers_off)
    ct.proj_b = uint(t.proj_b_off - t.layers_off)
    ct.tile1 = vt_tile(cm2, ct.n1out, ct.rows)
    ct.tile2 = one ? ct.tile1 : vt_tile(cm2, ct.out, ct.rows)
    ct.g1 = vt_tile_groups(ct.tile1, ct.n1out, ct.rows)
    ct.g2 = one ? 0l : vt_tile_groups(ct.tile2, ct.out, ct.rows)
    let feed_ok = cm2 ? (ensure_tower_ln16_cls() && ensure_tower_clamp_cvt_cls()) : ensure_tower_clamp_rq_cls()
    ct.on = (vt_tile_ensure(ct.tile1) && vt_tile_ensure(ct.tile2) && ensure_tower_pool_rows_cls() && ensure_tower_ln_cls()
        && ensure_tower_bias_act_cls() && ensure_tower_rms_cls() && ensure_tower_swiglu_swapped_cls() && ensure_tower_act_cls() && feed_ok)
    if (!ct.on) {
        vt_ct_stand_down(t.proj_kind)
    }
    return ct
}

[cold_path]
def private vt_ct_stand_down(kind : AudioProjKind) {
    to_log(LOG_INFO, "dasLLAMA vulkan tower: the {kind} projector tail stands down - a class failed to build; the blocks serve\n")
}

def private vt_ct_gemms(ct : VtChatTail) : int64 => ct.on ? (ct.m2in > 0l ? 2l : 1l) : 0l

//! the tail's share of the schedule buffer: its records' four words and a map word a group
def private vt_ct_meta_bytes(ct : VtChatTail) : int64 => (vt_ct_gemms(ct) * 4l + ct.g1 + ct.g2) * 4l

[cold_path]
def private vt_ct_release(var r : VtResident) {
    if (r.ct_cap > 0l) {
        release_device_buf(r.ct_hid_dev)
        release_device_buf(r.ct_gate_dev)
        release_device_buf(r.ct_out_dev)
        release_host_buf(r.ct_host)
        r.ct_cap = 0l
    }
}

//! the tail's planes at the scratch's capacity - the hidden rows, the gated rows, the soft tokens and their readback - and the
//! row sets over them and the chain's x / xb planes; a rebuild drops the GEMM sets cached over the old planes
[cold_path]
def private vt_ct_bufs(var r : VtResident; hid, gate, out : int64) : bool {
    vt_ct_release(r)
    var stale <- [for (k in keys(r.sets_front)); k; where k >= VT_CT_MM1_KEY]
    for (k in stale) {
        r.sets_front |> erase(k)
    }
    let cap = r.cap_rows
    let bx = cap * r.d * 4l
    let bxh = (cap + TILE_READ_SLACK) * max(r.d, r.ff) * 2l
    let bh = cap * max(hid, 1l) * 4l
    let bg = cap * max(gate, 1l) * 4l
    let bo = cap * out * 4l
    return false if (!vk_range_ok(bh) || !vk_range_ok(bg) || !vk_range_ok(bo))
    r.oom = false
    r.ct_hid_dev = vt_dev_buf(r, bh)
    r.ct_gate_dev = vt_dev_buf(r, bg)
    r.ct_out_dev = vt_dev_buf(r, bo)
    r.ct_host = vt_host_buf(r, bo, true)
    r.ct_cap = cap
    r.ct_hid = hid
    r.ct_gate = gate
    r.ct_out = out
    if (r.oom) {
        vt_ct_release(r)
        return false
    }
    r.s_ct_pool = set_tower_pool_rows_cls(fixed_array(r.x_dev, r.yo_dev), fixed_array(bx, bx), fixed_array(VT_X, VT_YO))
    r.s_ct_ln = set_tower_ln_cls(fixed_array(r.yo_dev, r.xb_dev, r.norms_dev), fixed_array(bx, bx, r.norms_bytes), fixed_array(VT_YO, VT_XB, 0u))
    r.s_ct_rms_xb = set_tower_rms_cls(fixed_array(r.xb_dev, r.xb_dev, r.norms_dev), fixed_array(bx, bx, r.norms_bytes), fixed_array(VT_XB, VT_XB, 0u))
    r.s_ct_rms_gate = set_tower_rms_cls(fixed_array(r.ct_gate_dev, r.ct_gate_dev, r.norms_dev), fixed_array(bg, bg, r.norms_bytes), fixed_array(VT_CT_GATE, VT_CT_GATE, 0u))
    r.s_ct_swiglu = set_tower_swiglu_swapped_cls(fixed_array(r.ct_hid_dev, r.ct_gate_dev), fixed_array(bh, bg), fixed_array(VT_CT_HID, VT_CT_GATE))
    r.s_ct_gelu = set_tower_act_cls(fixed_array(r.ct_hid_dev), fixed_array(bh), fixed_array(VT_CT_HID))
    r.s_ct_bias_out = set_tower_bias_act_cls(fixed_array(r.ct_out_dev, r.norms_dev), fixed_array(bo, r.norms_bytes), fixed_array(VT_TAIL_OUT, 0u))
    if (r.cm2) {
        r.s_ct_ln16 = set_tower_ln16_cls(fixed_array(r.yo_dev, r.norms_dev, r.xh_dev), fixed_array(bx, r.norms_bytes, bxh), fixed_array(VT_YO, 0u, VT_XQ))
    }
    r.s_ct_feed_xb = vt_feed_set(r, r.xb_dev, bx, VT_XB)
    r.s_ct_feed_hid = vt_feed_set(r, r.ct_hid_dev, bh, VT_CT_HID)
    r.s_ct_feed_gate = vt_feed_set(r, r.ct_gate_dev, bg, VT_CT_GATE)
    return true
}

//! a tail GEMM's set over its tile - into the soft tokens (`to_out`) or the hidden rows - built once per tile key
def private vt_ct_gemm_set(var r : VtResident; v : VtTile; to_out : bool; key : int) : VkDescriptorSet {
    return cached(r.sets_front, key + vt_tile_key(v)) $() : VkDescriptorSet {
        if (to_out) {
            return vt_tile_set(r, v, r.ct_out_dev, r.ct_cap * r.ct_out * 4l, VT_TAIL_OUT)
        }
        return vt_tile_set(r, v, r.ct_hid_dev, r.ct_cap * r.ct_hid * 4l, VT_CT_HID)
    }
}

//! the tail's planes and sets for this chain's scratch, its GEMM sets into the record; false = an allocation failed
def private vt_ct_ready(var r : VtResident; var tl : VtChatTail) : bool {
    let hid = tl.m2in > 0l ? tl.n1out : 0l
    let gate = tl.kind == AudioProjKind.ultravox ? tl.m2in : 0l
    if ((r.ct_cap != r.cap_rows || r.ct_hid != hid || r.ct_gate != gate || r.ct_out != tl.out) && !vt_ct_bufs(r, hid, gate, tl.out)) {
        return false
    }
    tl.s_mm1 = vt_ct_gemm_set(r, tl.tile1, tl.m2in == 0l, VT_CT_MM1_KEY)
    if (tl.m2in > 0l) {
        tl.s_mm2 = vt_ct_gemm_set(r, tl.tile2, true, VT_CT_MM2_KEY)
    }
    return true
}

//! a GEMM feed off the tail's f32 rows: the arm's clamp + halfword store or clamp + requant over `nelem` elements
def private vt_ct_feed(raw : VkCommandBuffer; var h : VkHaz; cm2 : bool; var s : VkDescriptorSet; nelem : int64) {
    var pc_rq = RqArgs(inbase = 0u, nblk = uint(nelem / kq_elems(KqFmt.q8)), lo = -VT_NO_CLAMP, hi = VT_NO_CLAMP)
    vt_feed_enc(raw, h, cm2, s, pc_rq, nelem)
    vt_pt(raw, VtProfRole.rq)
}

//! schedule record `map1` carries the first projector GEMM's map, `map1 + 1` the second's
[arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vt_ct_enc(raw : VkCommandBuffer; var h : VkHaz; var r : VtResident; var tl : VtChatTail; d, map1 : int64; cm2 : bool) {
    let uv = tl.kind == AudioProjKind.ultravox
    let n1 = tl.pool_rows * d
    var pc_ln = gk_ln_args(uint(d), uint(tl.pool_rows), tl.eps, tl.lnoff, tl.lnboff)
    if (uv) {
        enc_tower_ln_cls(raw, h, r.s_ln, pc_ln, tl.pool_rows)
        vt_pt(raw, VtProfRole.ln)
        var pc_rms = gk_rms_args(uint(tl.m1in), uint(tl.rows), ULTRAVOX_NORM_EPS, tl.normpre)
        enc_tower_rms_cls(raw, h, r.s_ct_rms_xb, pc_rms, tl.rows)
        vt_pt(raw, VtProfRole.rms)
        vt_ct_feed(raw, h, cm2, r.s_ct_feed_xb, n1)
    } else {
        var pc_pool = GkPoolRowsArgs(width = uint(d), total = uint(n1))
        enc_tower_pool_rows_cls(raw, h, r.s_ct_pool, pc_pool, (n1 + 255l) / 256l)
        vt_pt(raw, VtProfRole.pool)
        if (cm2) {
            enc_tower_ln16_cls(raw, h, r.s_ct_ln16, pc_ln, tl.pool_rows)
            vt_pt(raw, VtProfRole.ln)
        } else {
            enc_tower_ln_cls(raw, h, r.s_ct_ln, pc_ln, tl.pool_rows)
            vt_pt(raw, VtProfRole.ln)
            vt_ct_feed(raw, h, false, r.s_ct_feed_xb, n1)
        }
    }
    var pc_g1 = BatchArgs(n = uint(tl.m1in), d = uint(tl.n1out), map_off = g_vt_maps[map1])
    vt_tile_enc(raw, h, tl.tile1, tl.s_mm1, pc_g1, tl.g1)
    vt_pt(raw, VtProfRole.gemm_o)
    let nh = tl.rows * tl.n1out
    if (tl.m2in == 0l) {
        var pc_b = GkBiasActArgs(d = uint(tl.n1out), nelem = uint(nh), boff = tl.proj_b, act = BIAS_ACT_NONE)
        enc_tower_bias_act_cls(raw, h, r.s_ct_bias_out, pc_b, rq_wgs(false, nh))
        vt_pt(raw, VtProfRole.bias)
        return
    }
    let n2 = tl.rows * tl.m2in
    if (uv) {
        var pc_gate = GkGatedRowsArgs(d = uint(tl.m2in), nelem = uint(n2))
        enc_tower_swiglu_swapped_cls(raw, h, r.s_ct_swiglu, pc_gate, (n2 + 255l) / 256l)
        vt_pt(raw, VtProfRole.glu)
        var pc_rms = gk_rms_args(uint(tl.m2in), uint(tl.rows), ULTRAVOX_NORM_EPS, tl.normmid)
        enc_tower_rms_cls(raw, h, r.s_ct_rms_gate, pc_rms, tl.rows)
        vt_pt(raw, VtProfRole.rms)
        vt_ct_feed(raw, h, cm2, r.s_ct_feed_gate, n2)
    } else {
        var pc_act = GkBiasActArgs(d = uint(tl.n1out), nelem = uint(nh), boff = 0u, act = BIAS_ACT_GELU_ERF)
        enc_tower_act_cls(raw, h, r.s_ct_gelu, pc_act, rq_wgs(false, nh))
        vt_pt(raw, VtProfRole.bias)
        vt_ct_feed(raw, h, cm2, r.s_ct_feed_hid, n2)
    }
    var pc_g2 = BatchArgs(n = uint(tl.m2in), d = uint(tl.out), map_off = g_vt_maps[map1 + 1l])
    vt_tile_enc(raw, h, tl.tile2, tl.s_mm2, pc_g2, tl.g2)
    vt_pt(raw, VtProfRole.gemm_o)
}

[arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vt_aud_blocks(t : AudioTower; var s : EncoderState; npos : int64; ln_post : bool; var ct : VtChatTail = VtChatTail()) : bool {
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
    if (t.dev_backend != TowerDev.vulkan) {
        return vt_flush_and_decline(r, VulkanTowerDecline.flavor)
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
    if (!vk_range_ok(t.qblob.n) || !vk_range_ok(vt_scratch_max_bytes(vt_cap_rows(npos), d, ff, d, 0l, false, false))) {
        return vt_flush_and_decline(r, VulkanTowerDecline.shape)
    }
    let v = vt_tile(false, d, npos).v
    let vff = vt_tile(false, ff, npos).v
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
    if (ct.on) {
        ensure_length(s.out, ct.rows * ct.out)
        return vt_ln_chain(r, g_vt_ln_offs, t.has_k_bias, act, t.eps, t.layers_off, d, ff, hs, hsp, t.n_head, npos, khr, v, vff, s.x, -1l, -1l, g_vt_no_rows, ct, s.out)
    }
    return vt_ln_chain(r, g_vt_ln_offs, t.has_k_bias, act, t.eps, t.layers_off, d, ff, hs, hsp, t.n_head, npos, khr, v, vff, s.x)
}

//! the blocks with the projector tail behind them where the tail lever is on and the tail's shape rides the chain
//! (`vt_ct_shape_ok`), else the blocks alone; which it served, so the CPU tail runs exactly when the device one did not
[hot_path]
def private vulkan_audio_tower_blocks_tail(t : AudioTower; var s : EncoderState; npos : int64) : TowerTailServed {
    var ct = VtChatTail()
    if (g_vt_env_tail && t.q8 && vt_ct_shape_ok(t, npos)) {
        ct = vt_ct_prepare(t, npos, vt_cm2_feed())
    } elif (g_vt_env_tail && t.q8) {
        vt_decline(g_vt_aud, VulkanTowerDecline.shape)   // the tail alone declines; the blocks serve and the CPU tail reads their rows
    }
    if (!vt_aud_blocks(t, s, npos, false, ct)) {
        return TowerTailServed.declined
    }
    return ct.on ? TowerTailServed.tail : TowerTailServed.blocks
}

[hot_path]
def private vulkan_audio_tower_blocks(t : AudioTower; var s : EncoderState; npos : int64) : bool => vt_aud_blocks(t, s, npos, false)

[hot_path]
def private vulkan_audio_tower_blocks_ln_post(t : AudioTower; var s : EncoderState; npos : int64) : bool => vt_aud_blocks(t, s, npos, true)

//! a whisper-class tower has a conv stem when its q8 layers start past the stem's conv2 plane (a conv-less core - qwen3a's - starts at 0)
def private vt_aud_has_conv(t : AudioTower) : bool => t.qlayers_off > 0l

[arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vt_aud_attach(var r : VtResident; t : AudioTower) : bool {
    vt_aud_offs(t)
    let lay = active_q8_repack_layout(g_active_needs_repack)
    let norms_plane_n = t.fblob.n - t.layers_off   //! the blocks' rows, the post-norm and the projector tail's f32 rows behind them
    let src = vt_dev_src(t)
    let key = (src.on ? vt_key_dev(src, t.fblob, t.layers_off, norms_plane_n, t.n_layer)
        : vt_key(t.qblob, t.qscales, t.fblob, t.layers_off, norms_plane_n, 0l, t.n_layer, lay.mr, lay.kgroup, lay.wbias))
    if (r.key == key) {
        return true
    }
    var inscope regions <- vt_aud_regions(t)
    return vt_upload(r, key, t.qblob, t.qscales, regions, t.fblob, t.layers_off, norms_plane_n, 0l, lay, src, VT_AUD_PIECE)
}

let private VT_AUD_PIECE = "vk.aud"                //! a whisper-class tower's q8 pieces' prefix
let private VT_AUD_FRONT16_PIECE = "vk.aud.front16"  //! ... its conv stem's first conv as the f16 slab

//! the tower's block offsets into `g_vt_ln_offs`
def private vt_aud_offs(t : AudioTower) {
    g_vt_ln_offs |> reserve(t.n_layer)
    g_vt_ln_offs |> resize(t.n_layer)
    for (l in range64(t.n_layer)) {
        g_vt_ln_offs[l] = enc_layer_offsets(t, l)
    }
}

//! a whisper-class tower's q8 regions: the projector tail's, the stem's second conv where it has one, the blocks'
def private vt_aud_regions(t : AudioTower) : array<Q8Region> {
    var proj : array<Q8Region>
    vt_ct_regions(t, proj)
    return <- vt_ln_regions(t.d_model, t.n_ff, g_vt_ln_offs, vt_aud_has_conv(t) ? t.conv2_w_off : -1l, proj)
}

//! A whisper-class tower's Vulkan device block (`#tower-dev-block`): its q8 regions gathered, the stem's f16 slab where it has a stem
[cold_path, arch(at = "../ARCHITECTURE_IMAGE_GPU_PLANES.md#tower-dev-block")]
def private vt_aud_bake(t : AudioTower; var b : TowerDevBuild) {
    vt_aud_offs(t)
    var inscope regions <- vt_aud_regions(t)
    vt_bake_q8(b, VT_AUD_PIECE, t.qblob, t.qscales, regions)
    return if (!vt_aud_has_conv(t))
    var inscope slab16 <- vt_aud_stem16(t)
    tower_dev_put(b, VT_AUD_FRONT16_PIECE, unsafe(addr<uint8 const?>(slab16[0])), long_length(slab16) * 2l)
}

//! the standalone tower's Vulkan device block
[cold_path]
def private vulkan_audio_tower_bake(t : AudioTower; var b : TowerDevBuild) : bool {
    vt_aud_bake(t, b)
    return true
}

//! the standalone tower's Vulkan chain serves the lane: the q8 lane, the tower, blocks and tail knobs on, the device armed with
//! the f16 feed its stem reads
def private vulkan_audio_tower_serves_model(q8 : bool) : bool
    => q8 && g_vt_env_tower && g_vt_env_audio_blocks && g_vt_env_tail && vulkan_tower_arms() && vt_cm2_feed()

//! the stem's first conv as its f16 slab: each row's three taps a mel on the f16 GEMM's 64 lattice
def private vt_aud_stem16(t : AudioTower) : array<uint16> {
    let kreal = 3l * t.n_mel
    let kdp = round_up(kreal, 64l)
    var slab : array<float>
    slab |> reserve_resize(t.d_model * kdp)
    tower_stage_f32_pad_row_stride(slab, t.fblob, t.conv1_w_off, 0l, t.d_model, kreal, kdp)
    var slab16 <- cvt_f32_to_f16_sat_array(slab)
    delete slab
    return <- slab16
}

let private VT_STEM_MEL = 0x1000000u   //! the whisper-class stem's hazard bits: the mel block, conv1's im2col and its rows
let private VT_STEM_COL = 0x2000000u
let private VT_STEM_Y1 = 0x4000000u
let private VTA_CONV2_KEY = 5000

[cold_path]
def private vt_aud_front_slab(var r : VtResident; t : AudioTower) : bool {
    let d = t.d_model
    let kdp = round_up(3l * t.n_mel, 64l)
    r.stem_kdp = kdp
    let src = vt_dev_src(t)
    var inscope slab16 : array<uint16>
    if (!src.on) {
        slab16 <- vt_aud_stem16(t)
    }
    let rows = 2l * d + t.n_ctx * d
    r.fw16_bytes = d * kdp * 2l
    if (src.on && vt_piece_bytes(src, VT_AUD_FRONT16_PIECE) != r.fw16_bytes) {
        return false
    }
    r.fw16_dev = vt_dev_buf(r, r.fw16_bytes)
    r.fnorm_bytes = rows * 4l
    r.fnorm_dev = vt_dev_buf(r, r.fnorm_bytes)
    r.front = true
    if (!r.oom) {
        if (src.on) {
            vt_put_piece(src, VT_AUD_FRONT16_PIECE, r.fw16_dev)
        } else {
            upload_region_at(r.fw16_dev, 0l, unsafe(addr<uint8 const?>(slab16[0])), r.fw16_bytes)
        }
        upload_float_ranges(r.fnorm_dev, [(p = plane_at(t.fblob, t.conv1_b_off), n = d), (p = plane_at(t.fblob, t.conv2_b_off), n = d),
                                          (p = plane_at(t.fblob, t.pos_off), n = t.n_ctx * d)])
    }
    delete slab16
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
    r.fxin_dev = vt_call_buf(r, bxin)
    r.fcol_dev = vt_dev_buf(r, bcol)
    r.fy1_dev = vt_dev_buf(r, by1)
    if (r.oom) {
        vt_front_release(r)
        return false
    }
    let bx = r.cap_rows * d * 4l
    let bxh = (r.cap_rows + TILE_READ_SLACK) * max(r.d, r.ff) * 2l
    r.s_fim[0] = set_tts_im2col16_cls(fixed_array(r.fxin_dev, r.fcol_dev), fixed_array(bxin, bcol), fixed_array(VT_STEM_MEL, VT_STEM_COL))
    r.s_fg[0] = f16_gemm_set(gemm_khr, fixed_array(r.fw16_dev, r.fcol_dev, r.fy1_dev), fixed_array(r.fw16_bytes, bcol, by1), fixed_array(0u, VT_STEM_COL, VT_STEM_Y1))
    r.s_fb[0] = set_tower_bias_act_cls(fixed_array(r.fy1_dev, r.fnorm_dev), fixed_array(by1, r.fnorm_bytes), fixed_array(VT_STEM_Y1, 0u))
    r.s_fim[1] = set_tts_im2col16_cls(fixed_array(r.fy1_dev, r.xh_dev), fixed_array(by1, bxh), fixed_array(VT_STEM_Y1, VT_XQ))
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
    if (!vt_gate(r, t.q8)) {
        return false
    }
    return vt_decline(r, VulkanTowerDecline.flavor) if (t.dev_backend != TowerDev.vulkan)
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
    if (!vk_range_ok(t.qblob.n) || !vk_range_ok(vt_scratch_max_bytes(vt_cap_rows(tt), d, ff, d, 0l, false, false))) {
        return vt_decline(r, VulkanTowerDecline.shape)
    }
    let v = vt_tile(false, d, tt).v
    let vff = vt_tile(false, ff, tt).v
    let tc_d = cm2_tile_cols(d, tt)
    let tc_ff = cm2_tile_cols(ff, tt)
    if (!vt_ln_chain_ensure(khr, hsp, v, vff) || !f16_gemm_ensure(arm.gemm_khr) || !ensure_tts_im2col16_cls() || !ensure_tower_bias_act_cls()
            || !cm2_cls_ensure(int(KqFmt.q8), tc_d) || !cm2_cls_ensure(int(KqFmt.q8), tc_ff)) {
        return vt_decline(r, VulkanTowerDecline.device)
    }
    if (!vt_aud_attach(r, t) || (!r.front && !vt_aud_front_slab(r, t))) {
        vt_release(r)
        return vt_decline(r, VulkanTowerDecline.memory)
    }
    let g_d = cm2_tiles_at(d, tt, tc_d)
    let g_ff = cm2_tiles_at(ff, tt, tc_ff)
    if (!vt_ln_scratch(r, t.n_layer, tt, d, ff, hs, d, g_d, g_ff, khr, true, tc_d, tc_ff, v, vff, 0l)) {
        return vt_decline(r, VulkanTowerDecline.memory)
    }
    if (r.front_cap != tin || r.gemm_khr != arm.gemm_khr) {
        r.gemm_khr = arm.gemm_khr
        if (!vt_aud_front_scratch(r, tin, nm, d, arm.gemm_khr)) {
            return vt_decline(r, VulkanTowerDecline.memory)
        }
    }
    let skey = VTA_CONV2_KEY + int(tc_d)
    var s_conv2 = cached(r.sets_front, skey) $() : VkDescriptorSet {
        let bxh = (r.cap_rows + TILE_READ_SLACK) * max(r.d, r.ff) * 2l
        return cm2_cls_set(int(KqFmt.q8), tc_d, fixed_array(r.wq_dev, r.ws_dev, r.meta.buf, r.xh_dev, r.x_dev),
            fixed_array(r.wq_bytes, r.ws_bytes, r.meta_bytes, bxh, r.cap_rows * d * 4l), fixed_array(0u, 0u, 0u, VT_XQ, VT_X))
    }
    var mp = unsafe(reinterpret<uint?>(r.meta.mapped))
    var cur = 4l
    let map_off = batch_sched_map(mp, cur, 0l, t.conv2_w_off / kq_elems(KqFmt.q8), vt_tile_rows(VtTile(v = 0, tc = tc_d), tt), g_d)
    upload_region_at(r.fxin_dev, 0l, unsafe(addr<uint8 const?>(mel.data[0])), nm * tin * 4l)
    let act = t.gelu_tanh ? BIAS_ACT_GELU_TANH : BIAS_ACT_GELU_ERF
    let bx = tt * d * 4l
    vt_chain_one_shot("whisper-class stem", 2l, tt) $(raw : VkCommandBuffer; var h : VkHaz) {
        var pc_im1 = GkIm2col1dArgs(cin = uint(nm), tin = uint(tin), tout = uint(tin), k = 3u, stride = 1u, pad_l = 1u, dilation = 1u, kdim = uint(r.stem_kdp),
            row_major = 0u, r0 = 0u, mp = uint(tin))
        enc_tts_im2col16_cls(raw, h, r.s_fim[0], pc_im1, tin * r.stem_kdp)
        vt_pt(raw, VtProfRole.im2col)
        var pc_g1 = F16GemmArgs(n = uint(r.stem_kdp), d = uint(d), rows = uint(tin), woff = 0u, ybase = 0u, ystride = uint(d), ksplit = 0u)
        f16_gemm_enc(arm.gemm_khr, raw, h, r.s_fg[0], pc_g1, tin, d)
        vt_pt(raw, VtProfRole.gemm_pw)
        vt_bias_act(raw, h, r.s_fb[0], d, tin * d, 0l, act)
        vt_pt(raw, VtProfRole.bias)
        var pc_im2 = GkIm2col1dArgs(cin = uint(d), tin = uint(tin), tout = uint(tt), k = 3u, stride = 2u, pad_l = 1u, dilation = 1u, kdim = uint(3l * d), row_major = 1u, r0 = 0u, mp = uint(tt))
        enc_tts_im2col16_cls(raw, h, r.s_fim[1], pc_im2, tt * 3l * d)
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

[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_VISION.md#vk-vision-chains")]
def private vulkan_gemma3v_chain(t : Gemma3vTower; var s : Gemma3vState; planes : array<float>; w, h, npos : int64) : bool {   // nolint:STYLE038 — the dispatch chain is the shape
    var r & = g_vt_g3v
    if (!vt_gate(r, true)) {
        return false
    }
    let f32w = !t.q8
    let d = t.n_embd
    let ff = t.ff_pad
    let hs = d / t.n_head
    let hsp = hs <= 64l ? 64l : 128l
    let dp = t.n_head * hsp
    let pd = gemma3v_patch_pad(t)
    let n_side = t.image_px / t.patch
    let nout = (n_side / t.merge) * (n_side / t.merge)
    if (hs > 128l || d % 64l != 0l || ff % 64l != 0l || npos > VT_MAX_ENCODE_ROWS || npos <= 0l || w != t.image_px || h != t.image_px
            || n_side % t.merge != 0l || npos != n_side * n_side) {
        return vt_decline(r, VulkanTowerDecline.shape)
    }
    let arm = vt_arm(r)
    if (!arm.ok) {
        return false
    }
    let khr = arm.khr
    if (!vk_range_ok(t.qblob.n) || !vk_range_ok(t.blob.n * 4l) || !vk_range_ok(vt_scratch_max_bytes(vt_cap_rows(npos), d, ff, dp, 0l, false, false))) {
        return vt_decline(r, VulkanTowerDecline.shape)
    }
    let v = vt_tile(false, d, npos).v
    let vff = vt_tile(false, ff, npos).v
    let cm2 = !f32w && vt_cm2_feed()
    let tc_d = cm2 ? cm2_tile_cols(d, npos) : 0l
    let tc_ff = cm2 ? cm2_tile_cols(ff, npos) : 0l
    if (!vt_ln_chain_ensure(khr, hsp, v, vff) || !ensure_tower_patch_im2col_cls() || !ensure_f32_gemm_b_cls() || !ensure_f32_gemm_cls()
            || !ensure_tower_pool_2d_cls() || !ensure_tower_rms_cls() || !ensure_tower_bias_act_cls()
            || (cm2 && (!cm2_cls_ensure(int(KqFmt.q8), tc_d) || !cm2_cls_ensure(int(KqFmt.q8), tc_ff)))) {
        return vt_decline(r, VulkanTowerDecline.device)
    }
    g_vt_ln_offs |> reserve(t.n_layer)
    g_vt_ln_offs |> resize(t.n_layer)
    for (l in range64(t.n_layer)) {
        g_vt_ln_offs[l] = gemma3v_ln_offs(t, l)
    }
    let lay = active_q8_repack_layout(g_active_needs_repack)
    let blob_n = t.blob.n
    let key = f32w ? vt_key_f16(t.wblob, 0l, 0l, t.blob, 0l, blob_n, t.n_layer) : vt_key(t.qblob, t.qscales, t.blob, 0l, blob_n, 0l, t.n_layer, lay.mr, lay.kgroup, lay.wbias)
    if (r.key != key && !(f32w ? vt_upload_f32(r, key, t.blob, blob_n, t.wblob, 0l, 0l) : vt_upload_ln(r, key, t.qblob, t.qscales, d, ff, t.blob, 0l, blob_n, lay, g_vt_ln_offs))) {
        return vt_decline(r, VulkanTowerDecline.memory)
    }
    let g_d = cm2 ? cm2_tiles_at(d, npos, tc_d) : vt_gemm_groups(d, npos)
    let g_ff = cm2 ? cm2_tiles_at(ff, npos, tc_ff) : vt_gemm_groups(ff, npos)
    if (!vt_ln_scratch(r, t.n_layer, npos, d, ff, hs, dp, g_d, g_ff, khr, cm2, tc_d, tc_ff, v, vff, 0l, f32w) || !vt_vis_bufs(r, w, h, pd, 0l, 0l, 0l, max(d, t.proj_dim))) {
        return vt_decline(r, VulkanTowerDecline.memory)
    }
    let cap = r.cap_rows
    if (r.vis_sets_gen != r.vis_gen) {
        let bx = cap * d * 4l
        let bp = cap * r.tail_dim * 4l
        r.s_vis_patch = set_f32_gemm_b_cls(fixed_array(r.norms_dev, r.vis_col_dev, r.x_dev, r.norms_dev), fixed_array(r.norms_bytes, r.vis_col_bytes, bx, r.norms_bytes),
            fixed_array(0u, VT_STEM_COL, VT_X, 0u))
        r.s_vis_pos = set_tower_bias_act_cls(fixed_array(r.x_dev, r.norms_dev), fixed_array(bx, r.norms_bytes), fixed_array(VT_X, 0u))
        r.s_vis_pool = set_tower_pool_2d_cls(fixed_array(r.xb_dev, r.tail_out_dev), fixed_array(bx, bp), fixed_array(VT_XB, VT_TAIL_OUT))
        r.s_vis_proj = set_f32_gemm_cls(fixed_array(r.norms_dev, r.tail_out_dev, r.tail_proj_dev), fixed_array(r.norms_bytes, bp, bp), fixed_array(0u, VT_TAIL_OUT, VT_TAIL_PROJ))
        r.vis_sets_gen = r.vis_gen
    }
    vt_planes_up(r, planes, w, h)
    vt_chain_one_shot("gemma3v stem", 0l, npos) $(raw : VkCommandBuffer; var hz : VkHaz) {
        vt_vis_im2col(raw, hz, r, w, t.patch, pd, npos, 1.0, 0.0)
        var pc_patch = vt_pc32(pd, d, npos, t.patch_w, t.patch_b)
        enc_f32_gemm_b_cls(raw, hz, r.s_vis_patch, pc_patch, f32_gemm_wgs(npos, d))
        vt_pt(raw, VtProfRole.gemm_pw)
        vt_bias_act(raw, hz, r.s_vis_pos, npos * d, npos * d, t.pos_w, BIAS_ACT_NONE)
        vt_pt(raw, VtProfRole.post_add)
    }
    r.x_pend_ptr = unsafe(addr(s.x[0]))
    r.x_pend_bytes = npos * d * 4l
    r.x_ready = true
    g_vt_convs++
    ensure_length(s.tail, nout * t.proj_dim)
    let vis = VtVisTail(on = true, nout = nout, gw = n_side, merge = t.merge, d = d, proj = t.proj_dim, soft_norm_w = t.soft_norm_w, proj_w = t.proj_w, eps = t.eps)
    s.tail_ready = vt_ln_chain(r, g_vt_ln_offs, true, BIAS_ACT_GELU_TANH, t.eps, 0l, d, ff, hs, hsp, t.n_head, npos, khr, v, vff, s.x,
        t.post_ln_w, t.post_ln_b, s.xb, VtChatTail(), s.tail, vis, f32w)
    return s.tail_ready
}

[cold_path]
def private vt_upload_q3v(var r : VtResident; key : uint64; t : Qwen3vTower; lay : Q8RepackLayout) : bool {
    var regions : array<Q8Region>
    regions |> reserve(t.n_layer * QWEN3V_GEMMS)
    var recs : Q8Region[int(QWEN3V_GEMMS)]
    for (l in range64(t.n_layer)) {
        qwen3v_block_regions(t, l, recs)
        regions |> push_from(recs)
    }
    let ok = vt_upload(r, key, t.qblob, t.qscales, regions, t.blob, 0l, t.blob.n, 0l, lay)   //! the whole f32 blob: the stem's, the mergers' and the tail's rows at their file offsets
    delete regions
    return ok
}

[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_VISION.md#vk-vision-chains")]
def private vulkan_qwen3v_chain(t : Qwen3vTower; var s : Qwen3vState; planes : array<float>; w, h, npos : int64) : bool {   // nolint:STYLE038 — the dispatch chain is the shape
    var r & = g_vt_q3v
    if (!vt_gate(r, true)) {
        return false
    }
    let f32w = !t.q8
    let q8e = kq_elems(KqFmt.q8)
    let d = t.n_embd
    let ff = t.ff_pad
    let hs = d / t.n_head
    let hsp = hs <= 64l ? 64l : 128l
    let dp = t.n_head * hsp
    let nds = t.n_deepstack
    let pd = t.patch_dim
    let d4 = 4l * d
    let nout = npos / 4l
    let pw = w / t.patch
    let proj = t.proj_dim
    if (hs > 128l || hs % 2l != 0l || d % 64l != 0l || ff % 64l != 0l || npos > VT_MAX_ENCODE_ROWS || npos <= 0l || npos % 4l != 0l
            || npos != pw * (h / t.patch) || pw % 2l != 0l || d > ff) {
        return vt_decline(r, VulkanTowerDecline.shape)
    }
    let arm = vt_arm(r)
    if (!arm.ok) {
        return false
    }
    let khr = arm.khr
    if (!vk_range_ok(t.qblob.n) || !vk_range_ok(t.blob.n * 4l) || !vk_range_ok(vt_scratch_max_bytes(vt_cap_rows(npos), d, ff, dp, 0l, true, false))) {
        return vt_decline(r, VulkanTowerDecline.shape)
    }
    let fa_ok = fa_stamp_ensure(khr, hsp, false, false, false, false, true)
    if (!fa_ok || !ensure_tower_ln_cls() || !ensure_tower_clamp_rq_cls() || !ensure_tower_bias_act_cls() || !ensure_tower_rope_tab_cls()
            || !ensure_tower_hpad_cls() || !ensure_tower_hpad16_cls() || !ensure_tower_hunpad_cls() || !ensure_tower_post_add_ln_cls()
            || !ensure_tower_patch_im2col_cls() || !ensure_f32_gemm_cls() || !ensure_f32_gemm_b_cls() || !ensure_tower_stem_asm_cls() || !ensure_tower_act_cls()) {
        return vt_decline(r, VulkanTowerDecline.device)
    }
    let v = vt_tile(false, d, npos).v
    let vff = vt_tile(false, ff, npos).v
    let vqkv = vt_tile(false, 3l * d, npos).v
    if (!f32w && (!q8_batch_cls_ensure(v) || !q8_batch_cls_ensure(vff) || !q8_batch_cls_ensure(vqkv))) {
        return vt_decline(r, VulkanTowerDecline.device)
    }
    let lay = active_q8_repack_layout(g_active_needs_repack)
    let key = f32w ? vt_key_f16(t.wblob, 0l, 0l, t.blob, 0l, t.blob.n, t.n_layer) : vt_key(t.qblob, t.qscales, t.blob, 0l, t.blob.n, 0l, t.n_layer, lay.mr, lay.kgroup, lay.wbias)
    if (r.key != key && !(f32w ? vt_upload_f32(r, key, t.blob, t.blob.n, t.wblob, 0l, 0l) : vt_upload_q3v(r, key, t, lay))) {
        return vt_decline(r, VulkanTowerDecline.memory)
    }
    let g_d = vt_gemm_groups(d, npos)
    let g_ff = vt_gemm_groups(ff, npos)
    let g_qkv = vt_gemm_groups(3l * d, npos)
    let nrec = t.n_layer * QWEN3V_GEMMS
    let meta_bytes = (nrec * 4l + t.n_layer * (2l * g_d + g_ff + g_qkv)) * 4l
    if (!vt_scratch_fits(r, npos, d, ff, dp, meta_bytes, khr, true, 0l, f32w)) {
        if (!vt_scratch_build(r, npos, d, ff, hs, dp, meta_bytes, khr, true, 0l, f32w)) {
            return vt_decline(r, VulkanTowerDecline.memory)
        }
        vt_sets_q3v(r)
    }
    if (!f32w) {
        vt_bind_tile_sets(r, VtTile(v = v, tc = 0l), VtTile(v = vff, tc = 0l), VtTile(v = vqkv, tc = 0l))
    }
    let cap = r.cap_rows
    let bx = cap * d * 4l
    let pos_rs = qwen3v_pos_resized(t, s)
    if (!vt_vis_bufs(r, w, h, pd, 0l, 0l, bx, max(d4, (nds + 1l) * proj))) {
        return vt_decline(r, VulkanTowerDecline.memory)
    }
    if (r.vis_sets_gen != r.vis_gen) {
        let bff = cap * ff * 4l
        let bp = cap * r.tail_dim * 4l
        r.s_vis_patch = set_f32_gemm_cls(fixed_array(r.norms_dev, r.vis_col_dev, r.yo_dev), fixed_array(r.norms_bytes, r.vis_col_bytes, bx), fixed_array(0u, VT_STEM_COL, VT_YO))
        r.s_vis_pos = set_tower_stem_asm_cls(fixed_array(r.yo_dev, r.norms_dev, r.norms_dev, r.x_dev), fixed_array(bx, r.norms_bytes, r.norms_bytes, bx), fixed_array(VT_YO, 0u, 0u, VT_X))
        r.s_vis_pos_rs = set_tower_stem_asm_cls(fixed_array(r.yo_dev, r.norms_dev, r.vis_aux_dev, r.x_dev), fixed_array(bx, r.norms_bytes, r.vis_aux_bytes, bx), fixed_array(VT_YO, 0u, 0u, VT_X))
        r.s_vis_pool = set_tower_ln_cls(fixed_array(r.x_dev, r.tail_out_dev, r.norms_dev), fixed_array(bx, bp, r.norms_bytes), fixed_array(VT_X, VT_TAIL_OUT, 0u))
        r.s_vis_gemm[0] = set_f32_gemm_b_cls(fixed_array(r.norms_dev, r.tail_out_dev, r.hg_dev, r.norms_dev), fixed_array(r.norms_bytes, bp, bff, r.norms_bytes), fixed_array(0u, VT_TAIL_OUT, VT_HG, 0u))
        r.s_vis_gemm[1] = set_f32_gemm_b_cls(fixed_array(r.norms_dev, r.hg_dev, r.tail_proj_dev, r.norms_dev), fixed_array(r.norms_bytes, bff, bp, r.norms_bytes), fixed_array(0u, VT_HG, VT_TAIL_PROJ, 0u))
        r.s_vis_gemm[2] = set_f32_gemm_b_cls(fixed_array(r.norms_dev, r.xb_dev, r.tail_out_dev, r.norms_dev), fixed_array(r.norms_bytes, bx, bp, r.norms_bytes), fixed_array(0u, VT_XB, VT_TAIL_OUT, 0u))
        r.s_vis_proj = set_f32_gemm_b_cls(fixed_array(r.norms_dev, r.tail_out_dev, r.tail_proj_dev, r.norms_dev), fixed_array(r.norms_bytes, bp, bp, r.norms_bytes), fixed_array(0u, VT_TAIL_OUT, VT_TAIL_PROJ, 0u))
        r.s_vis_act[0] = set_tower_act_cls(fixed_array(r.hg_dev), fixed_array(bff), fixed_array(VT_HG))
        r.s_vis_act[1] = set_tower_act_cls(fixed_array(r.tail_out_dev), fixed_array(bp), fixed_array(VT_TAIL_OUT))
        r.vis_sets_gen = r.vis_gen
    }
    var recs : Q8Region[int(QWEN3V_GEMMS)]
    let rec_groups = fixed_array(g_qkv, g_d, g_ff, g_d)
    if (!f32w) {
        vt_sched_walk(g_vt_maps, r.meta, nrec) $(rid : int64) : VtSchedRec {
            let g = rid % QWEN3V_GEMMS
            if (g == 0l) {
                qwen3v_block_regions(t, rid / QWEN3V_GEMMS, recs)
            }
            return VtSchedRec(wblk0 = recs[g].woff / q8e, rows = npos, groups = rec_groups[g])
        }
    }
    let half = hs / 2l
    let btab = npos * half * 4l
    let bslice = nout * proj * 4l
    unsafe {
        vt_planes_up(r, planes, w, h)
        if (pos_rs) {
            upload_region_at(r.vis_aux_dev, 0l, addr<uint8 const?>(s.pos_rs[0]), npos * d * 4l)
        }
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
    var pc_fa = FaArgs(rows = uint(npos), w0 = 0u, qd = uint(dp), kvd = uint(dp), kv_mul = 1u, kvbase = 0u, scale = 1.0 / sqrt(float(hs)), window = 0u, sinkoff = 0u)
    var pc_pad_qkv = GkRestrideArgs(rows = uint(npos), rows_pad = uint(npos_pad), nheads = uint(t.n_head), hs = uint(hs), hsp = uint(hsp), sstride = uint(3l * d), soff = 0u)
    var pc_unpad = GkRestrideArgs(rows = uint(npos), rows_pad = uint(npos_pad), nheads = uint(t.n_head), hs = uint(hs), hsp = uint(hsp), sstride = uint(d), soff = 0u)
    var pc_rope = GkRopeTabArgs(rows = uint(npos), qs = uint(3l * d), col = 0u, d = uint(t.n_head * hs), dh = uint(hs), coff = 0u, soff = 0u)
    var pc_rq = RqArgs(inbase = 0u, nblk = uint(nd / q8e), lo = -VT_NO_CLAMP, hi = VT_NO_CLAMP)
    var pc_rqh = RqArgs(inbase = 0u, nblk = uint(nff / q8e), lo = -VT_NO_CLAMP, hi = VT_NO_CLAMP)
    var pc_gqkv = BatchArgs(n = uint(d), d = uint(3l * d), map_off = 0u)
    var pc_gd = BatchArgs(n = uint(d), d = uint(d), map_off = 0u)
    var pc_gff = BatchArgs(n = uint(d), d = uint(ff), map_off = 0u)
    var pc_dn = BatchArgs(n = uint(ff), d = uint(d), map_off = 0u)
    var pc32_qkv = vt_pc32(d, 3l * d, npos)
    var pc32_d = vt_pc32(d, d, npos)
    var pc32_ff = vt_pc32(d, ff, npos)
    var pc32_dn = vt_pc32(ff, d, npos)
    let wg32_qkv = f32_gemm_wgs(npos, 3l * d)
    let wg32_d = f32_gemm_wgs(npos, d)
    let wg32_ff = f32_gemm_wgs(npos, ff)
    let tile_d = VtTile(v = v, tc = 0l)
    var pc_bqkv = GkBiasActArgs(d = uint(3l * d), nelem = uint(3l * nd), boff = 0u, act = BIAS_ACT_NONE)
    var pc_gelu = GkBiasActArgs(d = uint(ff), nelem = uint(nff), boff = 0u, act = BIAS_ACT_GELU_TANH)
    vt_chain_one_shot("qwen3v", t.n_layer, npos) $(raw : VkCommandBuffer; var hz : VkHaz) {
        vt_vis_im2col(raw, hz, r, w, t.patch, pd, npos, 1.0, 0.0)
        var pc_patch = vt_pc32(pd, d, npos, t.patch_w)
        enc_f32_gemm_cls(raw, hz, r.s_vis_patch, pc_patch, f32_gemm_wgs(npos, d))
        vt_pt(raw, VtProfRole.gemm_pw)
        var pc_asm = GkStemAsmArgs(dim = uint(d), pw = uint(pw), total = uint(nd), boff = uint(t.patch_b), poff = pos_rs ? 0u : uint(t.pos_w))
        enc_tower_stem_asm_cls(raw, hz, pos_rs ? r.s_vis_pos_rs : r.s_vis_pos, pc_asm, (nd + 255l) / 256l)
        vt_pt(raw, VtProfRole.shuffle)
        for (l in range64(t.n_layer)) {
            let o = qwen3v_block_offs(t, l)
            let rb = l * QWEN3V_GEMMS
            if (l == 0l) {
                var pc = gk_ln_args(uint(d), uint(npos), t.eps, uint(o.ln1_w), uint(o.ln1_b))
                enc_tower_ln_cls(raw, hz, r.s_ln, pc, npos)
            }
            vt_rq_feed(raw, hz, f32w, r.s_rq_xb, pc_rq, wg_nd)
            vt_mm_lane(raw, hz, f32w, r.s32_qkv, pc32_qkv, o.w[QWEN3V_GEMM_QKV], wg32_qkv, VtTile(v = vqkv, tc = 0l), r.s_qkv, pc_gqkv, rb + QWEN3V_GEMM_QKV, g_qkv)
            pc_bqkv.boff = uint(o.qkv_b)
            enc_tower_bias_act_cls(raw, hz, r.s_bias_qkv, pc_bqkv, wg_n3d)
            pc_rope.col = 0u
            enc_tower_rope_tab_cls(raw, hz, r.s_rope_tq, pc_rope, wg_rope)
            pc_rope.col = uint(d)
            enc_tower_rope_tab_cls(raw, hz, r.s_rope_tk, pc_rope, wg_rope)
            pc_pad_qkv.soff = 0u
            enc_tower_hpad_cls(raw, hz, r.s_pad_q, pc_pad_qkv, wg_pad)
            pc_pad_qkv.soff = uint(d)
            enc_tower_hpad16_cls(raw, hz, r.s_pad_k, pc_pad_qkv, wg_pad)
            pc_pad_qkv.soff = uint(2l * d)
            enc_tower_hpad16_cls(raw, hz, r.s_pad_v, pc_pad_qkv, wg_pad)
            fa_stamp_enc(khr, hsp, false, false, false, false, raw, hz, r.s_fa, pc_fa, wg_fa, true)
            enc_tower_hunpad_cls(raw, hz, r.s_unpad, pc_unpad, wg_unpad)
            vt_rq_feed(raw, hz, f32w, r.s_rq_att, pc_rq, wg_nd)
            vt_mm_lane(raw, hz, f32w, r.s32_o, pc32_d, o.w[QWEN3V_GEMM_O], wg32_d, tile_d, r.s_o, pc_gd, rb + QWEN3V_GEMM_O, g_d)
            var pc_post = gk_add_ln_args(uint(d), uint(npos), t.eps, uint(o.out_b), true, uint(o.ln2_w), uint(o.ln2_b))
            enc_tower_post_add_ln_cls(raw, hz, r.s_post_o_ln, pc_post, npos)
            vt_rq_feed(raw, hz, f32w, r.s_rq_xb, pc_rq, wg_nd)
            vt_mm_lane(raw, hz, f32w, r.s32_hg, pc32_ff, o.w[QWEN3V_GEMM_UP], wg32_ff, VtTile(v = vff, tc = 0l), r.s_hg, pc_gff, rb + QWEN3V_GEMM_UP, g_ff)   //! the up rows ride the gate slot's buffer (hg)
            pc_gelu.boff = uint(o.up_b)
            enc_tower_bias_act_cls(raw, hz, r.s_bias_hg, pc_gelu, wg_nff)
            vt_rq_feed(raw, hz, f32w, r.s_rq_hg, pc_rqh, wg_nff)
            vt_mm_lane(raw, hz, f32w, r.s32_down, pc32_dn, o.w[QWEN3V_GEMM_DOWN], wg32_d, tile_d, r.s_down, pc_dn, rb + QWEN3V_GEMM_DOWN, g_d)
            let last = l + 1l == t.n_layer
            let nx = last ? o : qwen3v_block_offs(t, l + 1l)
            pc_post = gk_add_ln_args(uint(d), uint(npos), t.eps, uint(o.down_b), true, uint(last ? t.post_ln_w : nx.ln1_w), uint(last ? t.post_ln_b : nx.ln1_b))
            enc_tower_post_add_ln_cls(raw, hz, r.s_post_d_ln, pc_post, npos)
            let k = qwen3v_tap_of(t, l)
            if (k >= 0l && k < nds) {
                let fo = t.ds_f + k * t.ds_f_stride
                let wo = t.ds_w + k * t.ds_w_stride
                var pc_tln = gk_ln_args(uint(d4), uint(nout), t.eps, uint(fo), uint(fo + d4))
                enc_tower_ln_cls(raw, hz, r.s_vis_pool, pc_tln, nout)
                vt_merger_enc(raw, hz, r.s_vis_gemm[0], r.s_vis_act[0], r.s_vis_gemm[1], d4, proj, nout, wo, fo + 8l * d, wo + 16l * d * d, fo + 12l * d, (k + 1l) * nout * proj)
            }
        }
        vt_merger_enc(raw, hz, r.s_vis_gemm[2], r.s_vis_act[1], r.s_vis_proj, d4, proj, nout, t.mm0_w, t.mm0_b, t.mm2_w, t.mm2_b, 0l)
        vt_tail_copy_out(raw, hz, r, (nds + 1l) * bslice)
    }
    g_vt_convs++
    g_vt_tail_encodes++
    if (nds > 0l) {
        host_buf_read(r.tail_host, unsafe(addr<void?>(s.ds_stash[0])), nds * bslice, bslice)
    }
    return vt_served_from(r, r.tail_host, s.ds_slice, bslice, npos, t.n_layer)
}

[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_VISION.md#vk-vision-chains")]
def private vulkan_qwen25v_chain(t : Qwen25vTower; var s : Qwen25vState; planes : array<float>; w, h, npos : int64) : bool {   // nolint:STYLE038 — the dispatch chain is the shape, its two attention arms the routing
    var r & = g_vt_q25v
    if (!vt_gate(r, t.wblob.n > 0l && !t.blk_bf16)) {
        return false
    }
    let d = t.n_embd
    let ff = t.ff_pad
    let hs = d / t.n_head
    let hsp = hs <= 64l ? 64l : 128l
    let dp = t.n_head * hsp
    let pd = q25v_patch_pad(t)
    let d4 = 4l * d
    let nout = npos / 4l
    let nwin = long_length(s.win_lo) - 1l
    let has_win = t.n_wa_pattern > 1l && t.n_layer > 0l   //! a window layer exists: block 0 attends per window whenever the pattern spares a block
    let wmax = has_win ? qwen25v_window_widest(s) : 0l
    if (hs > 128l || hsp != 128l || hs % 2l != 0l || d % 64l != 0l || ff % 64l != 0l || npos > VT_MAX_ENCODE_ROWS || npos <= 0l || nwin <= 0l
            || npos % 4l != 0l || npos != (w / t.patch) * (h / t.patch) || (has_win && (wmax > 64l || hs != VT_WIN_HS))) {
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
    if (!vk_range_ok(w_n * 2l) || !vk_range_ok(t.blob.n * 4l) || !vk_range_ok(vt_scratch_max_bytes(vt_cap_rows(npos), d, ff, dp, 0l, false, true))) {
        return vt_decline(r, VulkanTowerDecline.shape)
    }
    let fa_ok = fa_stamp_ensure(khr, hsp, false, false, false, false, true)
    if (!fa_ok || !f16_gemm_ensure(gemm_khr) || !ensure_tower_clamp_cvt_cls() || !ensure_tower_rms_cls() || !ensure_tower_bias_act_cls()
            || !ensure_tower_rope_tab_cls() || !ensure_tower_hpad_cls() || !ensure_tower_hpad16_cls() || !ensure_tower_hunpad_cls()
            || !ensure_tower_win_attn_cls() || !ensure_tower_post_add_rms_cls() || !ensure_tower_bias_gate16_cls() || !ensure_tower_patch_im2col_cls()
            || !ensure_f32_gemm_cls() || !ensure_f32_gemm_b_cls() || !ensure_tts_row_gather_cls() || !ensure_tower_act_cls()) {
        return vt_decline(r, VulkanTowerDecline.device)
    }
    let key = vt_key_f16(t.wblob, w_off, w_n, t.blob, 0l, t.blob.n, t.n_layer)
    if (r.key != key && !vt_upload_f16(r, key, t.wblob, w_off, w_n, t.blob, 0l, t.blob.n)) {
        return vt_decline(r, VulkanTowerDecline.memory)
    }
    if (!vt_scratch_f16_fits(r, npos, nwin, d, ff, dp, khr, gemm_khr)) {
        if (!vt_scratch_f16_build(r, npos, nwin, d, ff, hs, dp, khr, gemm_khr)) {
            return vt_decline(r, VulkanTowerDecline.memory)
        }
        vt_sets_q25v(r)
    }
    let cap = r.cap_rows
    if (!vt_vis_bufs(r, w, h, pd, 0l, cap * 4l, 0l, max(d4, t.proj_dim))) {
        return vt_decline(r, VulkanTowerDecline.memory)
    }
    if (r.vis_sets_gen != r.vis_gen) {
        let bx = cap * d * 4l
        let bp = cap * r.tail_dim * 4l
        r.s_vis_patch = set_f32_gemm_cls(fixed_array(r.norms_dev, r.vis_col_dev, r.yo_dev), fixed_array(r.norms_bytes, r.vis_col_bytes, bx), fixed_array(0u, VT_STEM_COL, VT_YO))
        r.s_vis_pos = set_tts_row_gather_cls(fixed_array(r.yo_dev, r.vis_idx_dev, r.norms_dev, r.norms_dev, r.x_dev), fixed_array(bx, r.vis_idx_bytes, r.norms_bytes, r.norms_bytes, bx),
            fixed_array(VT_YO, 0u, 0u, 0u, VT_X))
        r.s_vis_pool = set_f32_gemm_b_cls(fixed_array(r.norms_dev, r.xb_dev, r.tail_out_dev, r.norms_dev), fixed_array(r.norms_bytes, bx, bp, r.norms_bytes), fixed_array(0u, VT_XB, VT_TAIL_OUT, 0u))
        r.s_vis_affine = set_tower_act_cls(fixed_array(r.tail_out_dev), fixed_array(bp), fixed_array(VT_TAIL_OUT))
        r.s_vis_proj = set_f32_gemm_b_cls(fixed_array(r.norms_dev, r.tail_out_dev, r.tail_proj_dev, r.norms_dev), fixed_array(r.norms_bytes, bp, bp, r.norms_bytes), fixed_array(0u, VT_TAIL_OUT, VT_TAIL_PROJ, 0u))
        r.vis_sets_gen = r.vis_gen
    }
    g_vt_wlo |> reserve(nwin + 1l)
    g_vt_wlo |> resize(nwin + 1l)
    qwen25v_window_table(s, unsafe(addr(g_vt_wlo[0])))
    g_vt_idx |> reserve(npos)
    g_vt_idx |> resize(npos)
    for (i in range64(npos)) {
        g_vt_idx[i] = uint(s.src_of[i])
    }
    let half = hs / 2l
    let btab = npos * half * 4l
    let bp_out = nout * t.proj_dim * 4l
    unsafe {
        vt_planes_up(r, planes, w, h)
        upload_region_at(r.vis_idx_dev, 0l, addr<uint8 const?>(g_vt_idx[0]), npos * 4l)
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
    let scale = 1.0 / sqrt(float(hs))
    var pc_fa = FaArgs(rows = uint(npos), w0 = 0u, qd = uint(dp), kvd = uint(dp), kv_mul = 1u, kvbase = 0u, scale = scale, window = 0u, sinkoff = 0u)
    var pc_rs = GkRestrideArgs(rows = uint(npos), rows_pad = uint(npos_pad), nheads = uint(t.n_head), hs = uint(hs), hsp = uint(hsp), sstride = uint(d), soff = 0u)
    var pc_rope = GkRopeTabArgs(rows = uint(npos), qs = uint(d), col = 0u, d = uint(t.n_head * hs), dh = uint(hs), coff = 0u, soff = 0u)
    var pc_gd = F16GemmArgs(n = uint(d), d = uint(d), rows = uint(npos), woff = 0u, ybase = 0u, ystride = uint(d), ksplit = 0u)
    var pc_gff = F16GemmArgs(n = uint(d), d = uint(ff), rows = uint(npos), woff = 0u, ybase = 0u, ystride = uint(ff), ksplit = 0u)
    var pc_dn = F16GemmArgs(n = uint(ff), d = uint(d), rows = uint(npos), woff = 0u, ybase = 0u, ystride = uint(d), ksplit = 0u)
    var pc_bias = GkBiasActArgs(d = uint(d), nelem = uint(nd), boff = 0u, act = BIAS_ACT_NONE)
    var pc_glu = GkBiasGateArgs(d = uint(ff), nelem = uint(nff), goff = 0u, uoff = 0u)
    var pc_win = GkWinAttnArgs(nheads = uint(t.n_head), hs = uint(hs), scale = scale)
    vt_chain_one_shot("qwen25v", t.n_layer, npos) $(raw : VkCommandBuffer; var hz : VkHaz) {
        vt_vis_im2col(raw, hz, r, w, t.patch, pd, npos, 1.0, 0.0)
        var pc_patch = vt_pc32(pd, d, npos, t.patch_w)
        enc_f32_gemm_cls(raw, hz, r.s_vis_patch, pc_patch, f32_gemm_wgs(npos, d))
        vt_pt(raw, VtProfRole.gemm_pw)
        var pc_g = GkRowGatherArgs(c = uint(d), rows = uint(npos), aoff = 0u, boff = 0u, coff = 0u, out_off = 0u, add_b = 0u, add_c = 0u, scale = 1.0)
        enc_tts_row_gather_cls(raw, hz, r.s_vis_pos, pc_g, nd)
        vt_pt(raw, VtProfRole.shuffle)
        for (l in range64(t.n_layer)) {
            let o = qwen25v_block_offs(t, l)
            let full = qwen25v_layer_full(t, l)
            if (l == 0l) {
                var pc = gk_rms_args(uint(d), uint(npos), t.eps, uint(o.ln1_w))
                enc_tower_rms_cls(raw, hz, r.s_rms, pc, npos)
            }
            vt_cvt16(raw, hz, r.s_cvt_xb, nd)
            pc_gd.woff = uint(o.w[QWEN25V_GEMM_Q] - w_off)
            f16_gemm_enc(gemm_khr, raw, hz, r.s_q, pc_gd, npos, d)
            pc_bias.boff = uint(o.b[QWEN25V_GEMM_Q])
            enc_tower_bias_act_cls(raw, hz, r.s_bias_q, pc_bias, wg_nd)
            pc_gd.woff = uint(o.w[QWEN25V_GEMM_K] - w_off)
            f16_gemm_enc(gemm_khr, raw, hz, r.s_k, pc_gd, npos, d)
            pc_bias.boff = uint(o.b[QWEN25V_GEMM_K])
            enc_tower_bias_act_cls(raw, hz, r.s_bias_k, pc_bias, wg_nd)
            pc_gd.woff = uint(o.w[QWEN25V_GEMM_V] - w_off)
            f16_gemm_enc(gemm_khr, raw, hz, r.s_v, pc_gd, npos, d)
            pc_bias.boff = uint(o.b[QWEN25V_GEMM_V])
            enc_tower_bias_act_cls(raw, hz, r.s_bias_v, pc_bias, wg_nd)
            enc_tower_rope_tab_cls(raw, hz, r.s_rope_tq, pc_rope, wg_rope)
            enc_tower_rope_tab_cls(raw, hz, r.s_rope_tk, pc_rope, wg_rope)
            if (full) {
                enc_tower_hpad_cls(raw, hz, r.s_pad_q, pc_rs, wg_pad)
                enc_tower_hpad16_cls(raw, hz, r.s_pad_k, pc_rs, wg_pad)
                enc_tower_hpad16_cls(raw, hz, r.s_pad_v, pc_rs, wg_pad)
                fa_stamp_enc(khr, hsp, false, false, false, false, raw, hz, r.s_fa, pc_fa, wg_fa, true)
                enc_tower_hunpad_cls(raw, hz, r.s_unpad, pc_rs, wg_unpad)
            } else {
                enc_tower_win_attn_cls(raw, hz, r.s_win, pc_win, nwin * t.n_head)
            }
            vt_cvt16(raw, hz, r.s_cvt_att, nd)
            pc_gd.woff = uint(o.w[QWEN25V_GEMM_O] - w_off)
            f16_gemm_enc(gemm_khr, raw, hz, r.s_o, pc_gd, npos, d)
            var pc_post = gk_add_ln_args(uint(d), uint(npos), t.eps, uint(o.b[QWEN25V_GEMM_O]), true, uint(o.ln2_w), 0u)
            enc_tower_post_add_rms_cls(raw, hz, r.s_post_o_ln, pc_post, npos)
            vt_cvt16(raw, hz, r.s_cvt_xb, nd)
            pc_gff.woff = uint(o.w[QWEN25V_GEMM_GATE] - w_off)
            f16_gemm_enc(gemm_khr, raw, hz, r.s_hg, pc_gff, npos, ff)
            pc_gff.woff = uint(o.w[QWEN25V_GEMM_UP] - w_off)
            f16_gemm_enc(gemm_khr, raw, hz, r.s_hu, pc_gff, npos, ff)
            pc_glu.goff = uint(o.b[QWEN25V_GEMM_GATE])
            pc_glu.uoff = uint(o.b[QWEN25V_GEMM_UP])
            enc_tower_bias_gate16_cls(raw, hz, r.s_glu, pc_glu, wg_nff)
            pc_dn.woff = uint(o.w[QWEN25V_GEMM_DOWN] - w_off)
            f16_gemm_enc(gemm_khr, raw, hz, r.s_down, pc_dn, npos, d)
            let last = l + 1l == t.n_layer
            pc_post = gk_add_ln_args(uint(d), uint(npos), t.eps, uint(o.b[QWEN25V_GEMM_DOWN]), true, uint(last ? t.post_ln_w : qwen25v_block_offs(t, l + 1l).ln1_w), 0u)
            enc_tower_post_add_rms_cls(raw, hz, r.s_post_d_ln, pc_post, npos)
        }
        vt_merger_enc(raw, hz, r.s_vis_pool, r.s_vis_affine, r.s_vis_proj, d4, t.proj_dim, nout, t.mm0_w, t.mm0_b, t.mm2_w, t.mm2_b, 0l)
        vt_tail_copy_out(raw, hz, r, bp_out)
    }
    g_vt_convs++
    g_vt_tail_encodes++
    ensure_length(s.mout, nout * t.proj_dim)
    return vt_served_from(r, r.tail_host, s.mout, bp_out, npos, t.n_layer)
}


[cold_path]
def private vt_upload_g4v(var r : VtResident; key : uint64; t : Gemma4vTower; ones_n : int64; lay : Q8RepackLayout) : bool {
    var regions : array<Q8Region>
    regions |> reserve(t.n_layer * GEMMA4V_GEMMS)
    var recs : Q8Region[int(GEMMA4V_GEMMS)]
    for (l in range64(t.n_layer)) {
        gemma4v_block_regions(t, l, recs)
        regions |> push_from(recs)
    }
    let ok = vt_upload(r, key, t.qblob, t.qscales, regions, t.blob, 0l, t.blob.n, ones_n, lay)   //! the whole f32 blob: the stem's and the tail's rows at their file offsets
    delete regions
    return ok
}



//! the gemma4uv embedder on the device off the planes, one command buffer: the patch im2col, the first layernorm over the raw rows,
//! the patch linear with its bias (the f32 tile over the bf16 plane widened), the second layernorm, the 2-axis positions, the third
//! layernorm, the weightless rms and the projection; `out` takes the soft tokens
[hot_path, unused_argument(s), arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_VISION.md#vk-vision-chains")]
def private vulkan_gemma4uv_encode(t : Gemma4uvEmbedder; var s : Gemma4uvState; planes : array<float>; w, h, npos, n_cols : int64; var out : array<float>) : bool {
    var r & = g_vt_uv
    if (!vt_gate(r, true)) {
        return false
    }
    let d = t.n_embd
    let pd = t.patch_dim
    let proj = t.proj_dim
    if (d % 64l != 0l || npos > VT_MAX_ENCODE_ROWS || npos <= 0l || npos != n_cols * (h / t.patch) || n_cols != w / t.patch) {
        return vt_decline(r, VulkanTowerDecline.shape)
    }
    let arm = vt_arm(r)
    if (!arm.ok) {
        return false
    }
    let vw_bytes = (d * pd + proj * d) * 4l
    if (!vk_range_ok(t.blob.n * 4l) || !vk_range_ok(vw_bytes) || !vk_range_ok(vt_scratch_max_bytes(vt_cap_rows(npos), d, d, 0l, 0l, false, false))) {
        return vt_decline(r, VulkanTowerDecline.shape)
    }
    if (!ensure_tower_patch_im2col_cls() || !ensure_tower_ln_cls() || !ensure_f32_gemm_b_cls() || !ensure_f32_gemm_cls() || !ensure_tower_pos_add_2d_cls()
            || !ensure_tower_rms_cls()) {
        return vt_decline(r, VulkanTowerDecline.device)
    }
    let key = vt_key_vis_w(vt_key_f16(t.wblob, 0l, 0l, t.blob, 0l, t.blob.n, 0l), t, d, pd)
    let fresh_w = r.key != key
    if ((fresh_w && !vt_upload_f32(r, key, t.blob, t.blob.n, t.wblob, 0l, t.n_embd)) || (!vt_scratch_fits(r, npos, d, d, 0l, 0l, arm.khr, false, 0l) && !vt_scratch_build(r, npos, d, d, 64l, 0l, 0l, arm.khr, false, 0l))
            || !vt_vis_bufs(r, w, h, pd, vw_bytes, 0l, 0l, proj)) {
        return vt_decline(r, VulkanTowerDecline.memory)
    }
    let cap = r.cap_rows
    let proj_off = d * pd
    if (fresh_w || r.vis_w_fresh) {
        vt_vis_slab(r, t, d, pd, proj_off)
        r.vis_w_fresh = false
    }
    if (r.vis_sets_gen != r.vis_gen) {
        let bx = cap * d * 4l
        let bp = cap * r.tail_dim * 4l
        r.s_vis_pool = set_tower_ln_cls(fixed_array(r.vis_col_dev, r.vis_col_dev, r.norms_dev), fixed_array(r.vis_col_bytes, r.vis_col_bytes, r.norms_bytes), fixed_array(VT_STEM_COL, VT_STEM_COL, 0u))
        r.s_vis_patch = set_f32_gemm_b_cls(fixed_array(r.vis_w_dev, r.vis_col_dev, r.x_dev, r.norms_dev), fixed_array(vw_bytes, r.vis_col_bytes, bx, r.norms_bytes), fixed_array(0u, VT_STEM_COL, VT_X, 0u))
        r.s_vis_gemm[0] = set_tower_ln_cls(fixed_array(r.x_dev, r.xb_dev, r.norms_dev), fixed_array(bx, bx, r.norms_bytes), fixed_array(VT_X, VT_XB, 0u))
        r.s_vis_pos = set_tower_pos_add_2d_cls(fixed_array(r.xb_dev, r.norms_dev), fixed_array(bx, r.norms_bytes), fixed_array(VT_XB, 0u))
        r.s_vis_gemm[1] = set_tower_ln_cls(fixed_array(r.xb_dev, r.x_dev, r.norms_dev), fixed_array(bx, bx, r.norms_bytes), fixed_array(VT_XB, VT_X, 0u))
        r.s_vis_act[0] = set_tower_rms_cls(fixed_array(r.x_dev, r.xb_dev, r.norms_dev), fixed_array(bx, bx, r.norms_bytes), fixed_array(VT_X, VT_XB, 0u))
        r.s_vis_proj = set_f32_gemm_cls(fixed_array(r.vis_w_dev, r.xb_dev, r.tail_proj_dev), fixed_array(vw_bytes, bx, bp), fixed_array(0u, VT_XB, VT_TAIL_PROJ))
        r.vis_sets_gen = r.vis_gen
    }
    vt_planes_up(r, planes, w, h)
    let nd = npos * d
    let bp_out = npos * proj * 4l
    vt_chain_one_shot("gemma4uv", 0l, npos) $(raw : VkCommandBuffer; var hz : VkHaz) {
        vt_vis_im2col(raw, hz, r, w, t.patch, pd, npos, 1.0, 0.0)
        var pc_ln1 = gk_ln_args(uint(pd), uint(npos), GEMMA4UV_LN_EPS, uint(t.ln1_w), uint(t.ln1_b))
        enc_tower_ln_cls(raw, hz, r.s_vis_pool, pc_ln1, npos)
        vt_pt(raw, VtProfRole.ln)
        var pc_patch = vt_pc32(pd, d, npos, 0l, t.patch_b)
        enc_f32_gemm_b_cls(raw, hz, r.s_vis_patch, pc_patch, f32_gemm_wgs(npos, d))
        vt_pt(raw, VtProfRole.gemm_pw)
        var pc_ln2 = gk_ln_args(uint(d), uint(npos), GEMMA4UV_LN_EPS, uint(t.ln2_w), uint(t.ln2_b))
        enc_tower_ln_cls(raw, hz, r.s_vis_gemm[0], pc_ln2, npos)
        var pc_pos = GkPosAdd2DArgs(dim = uint(d), ncols = uint(n_cols), pos_entries = uint(t.pos_entries), total = uint(nd), tboff = uint(t.pos_w))
        enc_tower_pos_add_2d_cls(raw, hz, r.s_vis_pos, pc_pos, (nd + 255l) / 256l)
        vt_pt(raw, VtProfRole.post_add)
        var pc_ln3 = gk_ln_args(uint(d), uint(npos), GEMMA4UV_LN_EPS, uint(t.ln3_w), uint(t.ln3_b))
        enc_tower_ln_cls(raw, hz, r.s_vis_gemm[1], pc_ln3, npos)
        var pc_rms = gk_rms_args(uint(d), uint(npos), t.rms_eps, uint(r.ones_off))
        enc_tower_rms_cls(raw, hz, r.s_vis_act[0], pc_rms, npos)
        vt_pt(raw, VtProfRole.rms)
        var pc_proj = vt_pc32(d, proj, npos, proj_off)
        enc_f32_gemm_cls(raw, hz, r.s_vis_proj, pc_proj, f32_gemm_wgs(npos, proj))
        vt_pt(raw, VtProfRole.gemm_pw)
        vt_tail_copy_out(raw, hz, r, bp_out)
    }
    g_vt_convs++
    return vt_served_from(r, r.tail_host, out, bp_out, npos, 0l)
}

//! the patch and projection rows as f32 in the vision slab: the bf16 planes widened, the f32 ones copied (gemma4v, gemma4uv)
//! the stem and projection rows' words folded into a slab key where the file keeps them bf16 (the f32 rows ride the blob fold)
def private vt_key_vis_w(k : uint64; t : auto(TT); d, pd : int64) : uint64 {
    var h = k
    if (t.patch_bf16) {
        h = vt_fold_words(h, unsafe(reinterpret<uint8 const?>(plane_at(t.wblob, t.patch_w))), d * pd * 2l)
    }
    if (t.proj_bf16) {
        h = vt_fold_words(h, unsafe(reinterpret<uint8 const?>(plane_at(t.wblob, t.proj_w))), t.proj_dim * d * 2l)
    }
    return h
}

[cold_path]
def private vt_vis_slab(r : VtResident; t : auto(TT); d, pd, proj_off : int64) {
    var inscope patch_rows <- vt_rows_f32(t.blob, t.wblob, t.patch_bf16, t.patch_w, d * pd)
    var inscope proj_rows <- vt_rows_f32(t.blob, t.wblob, t.proj_bf16, t.proj_w, t.proj_dim * d)
    unsafe {
        upload_region_at(r.vis_w_dev, 0l, addr<uint8 const?>(patch_rows[0]), d * pd * 4l)
        upload_region_at(r.vis_w_dev, proj_off * 4l, addr<uint8 const?>(proj_rows[0]), t.proj_dim * d * 4l)
    }
}

[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_VISION.md#vk-vision-chains")]
def private vulkan_gemma4v_chain(t : Gemma4vTower; var s : Gemma4vState; planes : array<float>; w, h, npos : int64) : bool {   // nolint:STYLE038 — the dispatch chain is the shape
    var r & = g_vt_g4v
    if (!vt_gate(r, true)) {
        return false
    }
    let f32w = !t.q8
    let q8e = kq_elems(KqFmt.q8)
    let d = t.n_embd
    let ff = t.n_ff
    let hs = d / t.n_head
    let pd = t.patch_dim
    let n_cols = w / t.patch
    let n_rows = h / t.patch
    let nout = (n_cols / t.merge) * (n_rows / t.merge)
    let vis = VtVisTail(on = true, nout = nout, gw = n_cols, merge = t.merge, d = d, proj = t.proj_dim, soft_norm_w = -1l, proj_w = d * pd, eps = t.eps,
        affine = true, scale = sqrt(float(d)), std = t.has_std, std_bias = t.std_bias, std_scale = t.std_scale, clamp = gemma4v_clamp_at(t, t.proj_clamp))
    if (hs != 64l || d % 64l != 0l || ff % 64l != 0l || npos > VT_MAX_ENCODE_ROWS || npos <= 0l || npos != n_cols * n_rows
            || n_cols % t.merge != 0l || n_rows % t.merge != 0l) {
        return vt_decline(r, VulkanTowerDecline.shape)
    }
    let arm = vt_arm(r)
    if (!arm.ok) {
        return false
    }
    let khr = arm.khr
    let vw_bytes = (d * pd + t.proj_dim * d) * 4l
    if (!vk_range_ok(t.qblob.n) || !vk_range_ok(t.blob.n * 4l) || !vk_range_ok(vw_bytes) || !vk_range_ok(vt_scratch_max_bytes(vt_cap_rows(npos), d, ff, 0l, 0l, false, false))) {
        return vt_decline(r, VulkanTowerDecline.shape)
    }
    let fa_ok = fa_stamp_ensure(khr, hs, false, false, false, false, true)
    if (!fa_ok || !ensure_tower_rms_cls() || !ensure_tower_clamp_cls() || !ensure_tower_clamp_rq_cls() || !ensure_tower_rope2d_cls()
            || !ensure_tower_post_add_cls() || !ensure_tower_geglu_cls() || !ensure_tower_clamp_cvt_cls() || !ensure_tower_patch_im2col_cls()
            || !ensure_f32_gemm_cls() || !ensure_tower_pos_add_2d_cls() || !ensure_tower_pool_2d_cls() || !ensure_tower_affine_rows_cls()
            || !ensure_tower_clamp_copy_cls()) {
        return vt_decline(r, VulkanTowerDecline.device)
    }
    let v = vt_tile(false, d, npos).v
    let vff = vt_tile(false, ff, npos).v
    if (!f32w && (!q8_batch_cls_ensure(v) || !q8_batch_cls_ensure(vff))) {
        return vt_decline(r, VulkanTowerDecline.device)
    }
    let lay = active_q8_repack_layout(g_active_needs_repack)
    let key = vt_key_vis_w(f32w ? vt_key_f16(t.wblob, 0l, t.wblob.n, t.blob, 0l, t.blob.n, t.n_layer) : vt_key(t.qblob, t.qscales, t.blob, 0l, t.blob.n, hs, t.n_layer, lay.mr, lay.kgroup, lay.wbias), t, d, pd)
    let fresh_w = r.key != key
    let ones_n = max(hs, d)
    if (fresh_w && !(f32w ? vt_upload_f32(r, key, t.blob, t.blob.n, t.wblob, t.blk_bf16 ? t.wblob.n : 0l, ones_n) : vt_upload_g4v(r, key, t, ones_n, lay))) {
        return vt_decline(r, VulkanTowerDecline.memory)
    }
    let wbase = (f32w && t.blk_bf16) ? t.blob.n : 0l
    let g_d = vt_gemm_groups(d, npos)
    let g_ff = vt_gemm_groups(ff, npos)
    let nrec = t.n_layer * GEMMA4V_GEMMS
    let meta_bytes = (nrec * 4l + t.n_layer * (5l * g_d + 2l * g_ff)) * 4l
    if (!vt_scratch_fits(r, npos, d, ff, 0l, meta_bytes, khr, false, 0l, f32w)) {
        if (!vt_scratch_build(r, npos, d, ff, hs, 0l, meta_bytes, khr, false, 0l, f32w)) {
            return vt_decline(r, VulkanTowerDecline.memory)
        }
        vt_sets_g4v(r)
    }
    if (!f32w) {
        vt_bind_tile_sets(r, VtTile(v = v, tc = 0l), VtTile(v = vff, tc = 0l))
    }
    if (!vt_vis_bufs(r, w, h, pd, vw_bytes, 0l, 0l, max(d, t.proj_dim))) {
        return vt_decline(r, VulkanTowerDecline.memory)
    }
    let cap = r.cap_rows
    let proj_off = d * pd
    if (fresh_w || r.vis_w_fresh) {
        vt_vis_slab(r, t, d, pd, proj_off)
        r.vis_w_fresh = false
    }
    if (r.vis_sets_gen != r.vis_gen) {
        let bx = cap * d * 4l
        let bp = cap * r.tail_dim * 4l
        r.s_vis_patch = set_f32_gemm_cls(fixed_array(r.vis_w_dev, r.vis_col_dev, r.x_dev), fixed_array(vw_bytes, r.vis_col_bytes, bx), fixed_array(0u, VT_STEM_COL, VT_X))
        r.s_vis_pos = set_tower_pos_add_2d_cls(fixed_array(r.x_dev, r.norms_dev), fixed_array(bx, r.norms_bytes), fixed_array(VT_X, 0u))
        r.s_vis_pool = set_tower_pool_2d_cls(fixed_array(r.x_dev, r.tail_out_dev), fixed_array(bx, bp), fixed_array(VT_X, VT_TAIL_OUT))
        r.s_vis_affine = set_tower_affine_rows_cls(fixed_array(r.tail_out_dev, r.norms_dev, r.norms_dev), fixed_array(bp, r.norms_bytes, r.norms_bytes), fixed_array(VT_TAIL_OUT, 0u, 0u))
        r.s_vis_clamp_in = set_tower_clamp_cls(fixed_array(r.tail_out_dev), fixed_array(bp), fixed_array(VT_TAIL_OUT))
        r.s_vis_proj = set_f32_gemm_cls(fixed_array(r.vis_w_dev, r.tail_out_dev, r.tail_proj_dev), fixed_array(vw_bytes, bp, bp), fixed_array(0u, VT_TAIL_OUT, VT_TAIL_PROJ))
        r.s_vis_clamp_out = set_tower_clamp_cls(fixed_array(r.tail_proj_dev), fixed_array(bp), fixed_array(VT_TAIL_PROJ))
        r.vis_sets_gen = r.vis_gen
    }
    var recs : Q8Region[int(GEMMA4V_GEMMS)]
    if (!f32w) {
        vt_sched_walk(g_vt_maps, r.meta, nrec) $(rid : int64) : VtSchedRec {
            let g = rid % GEMMA4V_GEMMS
            if (g == 0l) {
                gemma4v_block_regions(t, rid / GEMMA4V_GEMMS, recs)
            }
            return VtSchedRec(wblk0 = recs[g].woff / q8e, rows = npos, groups = g == GEMMA4V_GEMM_GATE || g == GEMMA4V_GEMM_UP ? g_ff : g_d)
        }
    }
    let pairs = hs / 4l
    g_vt_rope2d_tabs |> reserve(npos * 4l * pairs)
    g_vt_rope2d_tabs |> resize(npos * 4l * pairs)
    gemma4v_pack_rope_tabs(s, npos, pairs, unsafe(addr(g_vt_rope2d_tabs[0])))
    upload_region_at(r.tabs_dev, 0l, unsafe(addr<uint8 const?>(g_vt_rope2d_tabs[0])), npos * 4l * pairs * 4l)
    vt_planes_up(r, planes, w, h)
    let nd = npos * d
    let nff = npos * ff
    let wg_nd = rq_wgs(false, nd)
    let wg_nff = rq_wgs(false, nff)
    let wg_rope = (npos * t.n_head * pairs + 255l) / 256l
    let wg_fa = fa_stamp_wgs(khr, t.n_head, npos)
    let bp_out = nout * t.proj_dim * 4l
    var pc_fa = FaArgs(rows = uint(npos), w0 = 0u, qd = uint(d), kvd = uint(d), kv_mul = 1u, kvbase = 0u, scale = 1.0, window = 0u, sinkoff = 0u)
    var pc_rope = GkRope2dArgs(nheads = uint(t.n_head), hs = uint(hs), rows = uint(npos))
    var pc_gff = BatchArgs(n = uint(d), d = uint(ff), map_off = 0u)
    var pc32_d = vt_pc32(d, d, npos)
    var pc32_ff = vt_pc32(d, ff, npos)
    var pc32_dn = vt_pc32(ff, d, npos)
    let wg32_d = f32_gemm_wgs(npos, d)
    let wg32_ff = f32_gemm_wgs(npos, ff)
    vt_chain_one_shot("gemma4v", t.n_layer, npos) $(raw : VkCommandBuffer; var hz : VkHaz) {
        vt_vis_im2col(raw, hz, r, w, t.patch, pd, npos, 2.0, -1.0)
        var pc_patch = vt_pc32(pd, d, npos)
        enc_f32_gemm_cls(raw, hz, r.s_vis_patch, pc_patch, f32_gemm_wgs(npos, d))
        vt_pt(raw, VtProfRole.gemm_pw)
        var pc_pos = GkPosAdd2DArgs(dim = uint(d), ncols = uint(n_cols), pos_entries = uint(t.pos_entries), total = uint(nd), tboff = uint(t.pos_w))
        enc_tower_pos_add_2d_cls(raw, hz, r.s_vis_pos, pc_pos, (nd + 255l) / 256l)
        vt_pt(raw, VtProfRole.post_add)
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
                var pc = gk_rms_args(uint(d), uint(npos), t.eps, uint(o.ln1))
                enc_tower_rms_cls(raw, hz, r.s_rms, pc, npos)
            }
            let tile = VtTile(v = v, tc = 0l)
            if (f32w) {
                vt_mm_clamped32(raw, hz, cq, true, r.s_cc_xb, nd, r.s32_q, pc32_d, wbase + o.w[GEMMA4V_GEMM_Q], wg32_d, true, r.s_clamp_q, nd)
                vt_mm_clamped32(raw, hz, ck, true, r.s_cc_xb, nd, r.s32_k, pc32_d, wbase + o.w[GEMMA4V_GEMM_K], wg32_d, true, r.s_clamp_k, nd)
                vt_mm_clamped32(raw, hz, cv, true, r.s_cc_xb, nd, r.s32_v, pc32_d, wbase + o.w[GEMMA4V_GEMM_V], wg32_d, true, r.s_clamp_v, nd)
            } else {
                vt_mm_clamped(raw, hz, cq, r.s_rq_xb, nd, r.s_q, tile, d, d, g_vt_maps[rb + GEMMA4V_GEMM_Q], g_d, r.s_clamp_q, nd)
                vt_mm_clamped(raw, hz, ck, r.s_rq_xb, nd, r.s_k, tile, d, d, g_vt_maps[rb + GEMMA4V_GEMM_K], g_d, r.s_clamp_k, nd)
                vt_mm_clamped(raw, hz, cv, r.s_rq_xb, nd, r.s_v, tile, d, d, g_vt_maps[rb + GEMMA4V_GEMM_V], g_d, r.s_clamp_v, nd)
            }
            var pc_h = gk_rms_args(uint(hs), uint(npos * t.n_head), t.eps, uint(o.q_norm))
            enc_tower_rms_cls(raw, hz, r.s_hrms_q, pc_h, npos * t.n_head)
            pc_h.lnoff = uint(o.k_norm)
            enc_tower_rms_cls(raw, hz, r.s_hrms_k, pc_h, npos * t.n_head)
            pc_h.lnoff = uint(r.ones_off)
            enc_tower_rms_cls(raw, hz, r.s_hrms_v, pc_h, npos * t.n_head)
            enc_tower_rope2d_cls(raw, hz, r.s_rope_q, pc_rope, wg_rope)
            enc_tower_rope2d_cls(raw, hz, r.s_rope_k, pc_rope, wg_rope)
            vt_cvt16(raw, hz, r.s_cvt_k, nd)
            vt_cvt16(raw, hz, r.s_cvt_v, nd)
            fa_stamp_enc(khr, hs, false, false, false, false, raw, hz, r.s_fa, pc_fa, wg_fa, true)
            if (f32w) {
                vt_mm_clamped32(raw, hz, co, false, r.s_clamp_att, nd, r.s32_o, pc32_d, wbase + o.w[GEMMA4V_GEMM_O], wg32_d, true, r.s_clamp_yo, nd)
            } else {
                vt_mm_clamped(raw, hz, co, r.s_rq_att, nd, r.s_o, tile, d, d, g_vt_maps[rb + GEMMA4V_GEMM_O], g_d, r.s_clamp_yo, nd)
            }
            var pc_post = gk_add_post_rms_args(uint(d), uint(npos), t.eps, uint(o.attn_post), true, uint(o.ln2))
            enc_tower_post_add_cls(raw, hz, r.s_post_o, pc_post, npos)
            if (f32w) {
                vt_mm_clamped32(raw, hz, cg, true, r.s_cc_xb, nd, r.s32_hg, pc32_ff, wbase + o.w[GEMMA4V_GEMM_GATE], wg32_ff, false, r.s_clamp_q, 0l)
                vt_mm_clamped32(raw, hz, cu, true, r.s_cc_xb, nd, r.s32_hu, pc32_ff, wbase + o.w[GEMMA4V_GEMM_UP], wg32_ff, false, r.s_clamp_q, 0l)
            } else {
                var pc_rq = RqArgs(inbase = 0u, nblk = uint(nd / q8e), lo = vt_lo(cg, true), hi = vt_hi(cg, true))
                enc_tower_clamp_rq_cls(raw, hz, r.s_rq_xb, pc_rq, wg_nd)
                pc_gff.map_off = g_vt_maps[rb + GEMMA4V_GEMM_GATE]
                q8_batch_cls_enc(vff, raw, hz, r.s_hg, pc_gff, g_ff)
                pc_rq.lo = vt_lo(cu, true)
                pc_rq.hi = vt_hi(cu, true)
                enc_tower_clamp_rq_cls(raw, hz, r.s_rq_xb, pc_rq, wg_nd)
                pc_gff.map_off = g_vt_maps[rb + GEMMA4V_GEMM_UP]
                q8_batch_cls_enc(vff, raw, hz, r.s_hu, pc_gff, g_ff)
            }
            var pc_gg = GkGegluArgs(nelem = uint(nff), glo = vt_lo(cg, false), ghi = vt_hi(cg, false), ulo = vt_lo(cu, false), uhi = vt_hi(cu, false),
                dlo = vt_lo(cd, true), dhi = vt_hi(cd, true))
            enc_tower_geglu_cls(raw, hz, r.s_geglu, pc_gg, wg_nff)
            if (f32w) {
                vt_mm_clamped32(raw, hz, cd, false, r.s_clamp_hg, nff, r.s32_down, pc32_dn, wbase + o.w[GEMMA4V_GEMM_DOWN], wg32_d, true, r.s_clamp_yd, nd)
            } else {
                vt_mm_clamped(raw, hz, cd, r.s_rq_hg, nff, r.s_down, tile, ff, d, g_vt_maps[rb + GEMMA4V_GEMM_DOWN], g_d, r.s_clamp_yd, nd)
            }
            let last = l + 1l == t.n_layer
            pc_post = gk_add_post_rms_args(uint(d), uint(npos), t.eps, uint(o.ffn_post), !last, last ? 0u : uint(gemma4v_block_offs(t, l + 1l).ln1))
            enc_tower_post_add_cls(raw, hz, r.s_post_d, pc_post, npos)
        }
        vt_vis_tail_enc(raw, hz, r, vis)
        vt_tail_copy_out(raw, hz, r, bp_out)
    }
    g_vt_convs++
    g_vt_tail_encodes++
    ensure_length(s.tail, nout * t.proj_dim)
    s.tail_ready = vt_served_from(r, r.tail_host, s.tail, bp_out, npos, t.n_layer)
    return s.tail_ready
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
    return VtConvStage(iw = iw, ih = ih, cin = cin, ow = ow, oh = oh, rows = oh * ow, kdp = round_up(cin * 9l, 64l))
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
    var slab16 <- cvt_f32_to_f16_sat_array(slab)
    delete slab
    let span = vt_q3a_norm_span(t)
    var regions : array<Q8Region>
    regions |> push(Q8Region(woff = t.cout_w_off, n = 16l * nch, rows = t.core.d_model))
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
    let cap = cap_chunks * QWEN3A_TOK_PER_CHUNK
    let featw = 16l * nch
    let bxin = cap_chunks * QWEN3A_CHUNK * nm * 4l
    let bcol = vt_q3a_col_bytes(cap_chunks)
    let by = fixed_array(cap_chunks * g_vt_q3a_stages[0].rows * nch * 4l, cap_chunks * g_vt_q3a_stages[1].rows * nch * 4l, cap_chunks * g_vt_q3a_stages[2].rows * nch * 4l)
    let bfeat = (cap + TILE_READ_SLACK) * featw * 2l
    let bx = cap * d * 4l
    if (!vk_range_ok(max(max(bxin, bcol), max(max(by[0], by[1]), max(by[2], bfeat))))) {
        return false
    }
    r.meta_bytes = BATCH_META_BYTES
    r.meta = vt_host_buf(r, r.meta_bytes, false)
    r.cap_rows = cap
    r.front_cap = cap_chunks
    r.fxin_dev = vt_call_buf(r, bxin)
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
    if (!vt_gate(r, t.cq8)) {
        return false
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
    var s_cout = cached(r.sets_rel_proj, int(tc)) $() : VkDescriptorSet {
        let bfeat = (r.cap_rows + TILE_READ_SLACK) * featw * 2l
        return cm2_cls_set(int(KqFmt.q8), tc, fixed_array(r.wq_dev, r.ws_dev, r.meta.buf, r.ffeat_dev, r.fx_dev),
            fixed_array(r.wq_bytes, r.ws_bytes, r.meta_bytes, bfeat, r.cap_rows * d * 4l), fixed_array(0u, 0u, 0u, VTF_FEAT, VTF_X))
    }
    let g_cout = cm2_tiles_at(d, npos, tc)
    var mp = unsafe(reinterpret<uint?>(r.meta.mapped))
    var cur = 4l
    let map_off = batch_sched_map(mp, cur, 0l, t.cout_w_off / kq_elems(KqFmt.q8), npos, g_cout)
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
            var pc_im = GkIm2col2dArgs(iw = uint(st.iw), ih = uint(st.ih), cin = uint(st.cin), sstride = uint(st.cin), kdp = uint(st.kdp), rows = uint(rows), soff = 0u)
            enc_tower_im2col_cls(raw, h, r.s_fim[si], pc_im, (rows * st.kdp + 255l) / 256l)
            vt_pt(raw, VtProfRole.im2col)
            var pc_g = F16GemmArgs(n = uint(st.kdp), d = uint(nch), rows = uint(rows), woff = uint(st.slab_elem_off), ybase = 0u, ystride = uint(nch), ksplit = 0u)
            f16_gemm_enc(arm.gemm_khr, raw, h, r.s_fg[si], pc_g, rows, nch)
            vt_pt(raw, VtProfRole.gemm_pw)
            vt_bias_act(raw, h, r.s_fb[si], nch, rows * nch, st.boff, BIAS_ACT_GELU_ERF)
            vt_pt(raw, VtProfRole.bias)
        }
        var pc_sh = GkQ3aShuffleArgs(nch = uint(nch), ncp = uint(nch), chunk_stride = uint(16l * 13l * nch), npos = uint(npos))
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
    host_buf_read(r.host, unsafe(addr<void?>(s.x[0])), bx)
    g_vt_convs += n_chunks
    vt_announce(r)
    return true
}

let private VTF_MEL = 0x80u

[cold_path]
def private vt_g4a_front_slab(var r : VtResident; t : Gemma4aEncoder) : bool {
    let n_fft = (t.n_bins - 1l) * 2l
    let btw = n_fft * 2l * t.n_bins * 4l
    let bfil = t.n_mel * t.n_bins * 4l
    let src = vt_dev_src(t)
    if (src.on) {
        let f16 = tower_dev_find(src.devplan, VT_G4A_FRONT16_PIECE)
        return f16.off >= 0l && vt_front_upload16(r, plane_at(src.dev, f16.off), f16.bytes, t.twiddles, btw, t.filters, bfil)
    }
    var slab <- vt_g4a_front32(t)
    return vt_front_upload(r, slab, t.twiddles, btw, t.filters, bfil)
}

//! gemma4a's two subsample convs as the front's f32 slab: each row's taps on the f16 GEMM's 64 lattice
def private vt_g4a_front32(t : Gemma4aEncoder) : array<float> {
    let kdp0 = 64l
    let kd1 = t.sub_ch0 * 9l
    let kdp1 = round_up(kd1, 64l)
    var slab : array<float>
    slab |> reserve_resize(t.sub_ch0 * kdp0 + t.sub_ch1 * kdp1)
    tower_stage_f32_pad_row_stride(slab, t.blob, t.c0_w, 0l, t.sub_ch0, 9l, kdp0)
    tower_stage_f32_pad_row_stride(slab, t.blob, t.c1_w, t.sub_ch0 * kdp0, t.sub_ch1, kd1, kdp1)
    return <- slab
}

let private VT_G4A_PIECE = "vk.g4a"                  //! gemma4a's q8 pieces' prefix
let private VT_G4A_FRONT16_PIECE = "vk.g4a.front16"  //! ... its subsample convs as the f16 slab

//! gemma4a's q8 regions: every block's GEMMs, the input projection, the projector tail's two
def private vt_g4a_all_regions(t : Gemma4aEncoder) : array<Q8Region> {
    let d = t.d_model
    var regions : array<Q8Region>
    regions |> reserve(t.n_layer * VT_G4A_GEMMS + 3l)
    var recs : Q8Region[int(VT_G4A_GEMMS)]
    for (l in range64(t.n_layer)) {
        vt_g4a_regions(t.layers[l], d, t.n_ff, recs)
        regions |> push_from(recs)
    }
    regions |> push(Q8Region(woff = t.inproj_w, n = d, rows = d))
    regions |> push(Q8Region(woff = t.outproj_w, n = d, rows = t.proj_dim))
    regions |> push(Q8Region(woff = t.mmproj_w, n = t.proj_dim, rows = t.proj_dim))
    return <- regions
}

//! gemma4a's Vulkan device block (`#tower-dev-block`): its q8 regions gathered, the subsample convs' f16 slab
[cold_path, arch(at = "../ARCHITECTURE_IMAGE_GPU_PLANES.md#tower-dev-block")]
def private vulkan_gemma4a_bake(t : Gemma4aEncoder; var b : TowerDevBuild) : bool {
    var inscope regions <- vt_g4a_all_regions(t)
    vt_bake_q8(b, VT_G4A_PIECE, t.qblob, t.qscales, regions)
    var inscope slab <- vt_g4a_front32(t)
    var inscope slab16 <- cvt_f32_to_f16_sat_array(slab)
    tower_dev_put(b, VT_G4A_FRONT16_PIECE, unsafe(addr<uint8 const?>(slab16[0])), long_length(slab16) * 2l)
    return true
}

//! gemma4a's Vulkan chain serves the lane: the q8 lane, the tower knob on, the device armed with the f16 feed its front reads
def private vulkan_gemma4a_serves_model(q8 : bool) : bool => q8 && g_vt_env_tower && vulkan_tower_arms() && vt_cm2_feed()

//! a spectral front's model-owned planes uploaded (the f16 conv slab, the DFT twiddles, the mel filters); false with the buffers released on a failed allocation
[cold_path]
def private vt_front_upload(var r : VtResident; var slab : array<float>; twiddles : PlaneF; btw : int64; filters : PlaneF; bfil : int64) : bool {
    var slab16 <- cvt_f32_to_f16_sat_array(slab)
    delete slab
    let ok = vt_front_upload16(r, unsafe(addr<uint8 const?>(slab16[0])), long_length(slab16) * 2l,
        twiddles, btw, filters, bfil)
    delete slab16
    return ok
}

//! the front's f16 conv slab (`b16` bytes at `p16`), its twiddles and its filters uploaded as they are
[cold_path]
def private vt_front_upload16(var r : VtResident; p16 : uint8 const?; b16 : int64; twiddles : PlaneF; btw : int64; filters : PlaneF; bfil : int64) : bool {
    r.fw16_bytes = b16
    r.fw16_dev = vt_dev_buf(r, r.fw16_bytes)
    r.front_twiddles_dev = vt_dev_buf(r, max(btw, 64l))   // 64: a stub where the family's mel stays on the CPU (no twiddles)
    r.front_filters_dev = vt_dev_buf(r, bfil)
    if (r.oom) {
        release_device_buf(r.fw16_dev)
        release_device_buf(r.front_twiddles_dev)
        release_device_buf(r.front_filters_dev)
        return false
    }
    upload_region_at(r.fw16_dev, 0l, p16, r.fw16_bytes)
    if (btw > 0l) {
        upload_region_at(r.front_twiddles_dev, 0l, unsafe(reinterpret<uint8 const?>(plane_at(twiddles, 0l))), btw)
    }
    upload_region_at(r.front_filters_dev, 0l, unsafe(reinterpret<uint8 const?>(plane_at(filters, 0l))), bfil)
    r.front = true
    return true
}

[cold_path]
def private vt_g4a_front_scratch(var r : VtResident; t : Gemma4aEncoder; cap : int64; gemm_khr : bool) : VtBuild {
    vt_front_release(r)
    r.oom = false
    let n_fft = (t.n_bins - 1l) * 2l
    let n_bins = t.n_bins
    let n_mel = t.n_mel
    let ch0 = t.sub_ch0
    let ch1 = t.sub_ch1
    let d = t.d_model
    let kdp0 = 64l
    let kdp1 = round_up(ch0 * 9l, 64l)
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
    if (!vk_range_ok(max(max(bxw, bcol), max(max(by0, by1), max(bfeat, bmag))))) {
        return VtBuild.shape
    }
    r.front_cap = cap
    r.fxin_dev = vt_call_buf(r, bxw)
    r.fy3_dev = vt_dev_buf(r, bmag)
    r.fmel_dev = vt_call_buf(r, bmel)
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
        return VtBuild.memory
    }
    r.s_fdft = set_tower_spec_dft_cls(fixed_array(r.fxin_dev, r.front_twiddles_dev, r.fy3_dev), fixed_array(bxw, btw, bmag), fixed_array(VTF_XIN, 0u, VTF_Y3))
    r.s_fmel = set_tower_mel_log_cls(fixed_array(r.fy3_dev, r.front_filters_dev, r.fmel_dev), fixed_array(bmag, bfil, bmel), fixed_array(VTF_Y3, 0u, VTF_MEL))
    r.s_fim[0] = set_tower_im2col_cls(fixed_array(r.fmel_dev, r.fcol_dev), fixed_array(bmel, bcol), fixed_array(VTF_MEL, VTF_COL))
    r.s_fg[0] = f16_gemm_set(gemm_khr, fixed_array(r.fw16_dev, r.fcol_dev, r.fy1_dev), fixed_array(r.fw16_bytes, bcol, by0), fixed_array(0u, VTF_COL, VTF_Y1))
    r.s_fb[0] = set_tower_ln_relu_cls(fixed_array(r.fy1_dev, r.fy1_dev, r.norms_dev), fixed_array(by0, by0, r.norms_bytes), fixed_array(VTF_Y1, VTF_Y1, 0u))
    r.s_fim[1] = set_tower_im2col_cls(fixed_array(r.fy1_dev, r.fcol_dev), fixed_array(by0, bcol), fixed_array(VTF_Y1, VTF_COL))
    r.s_fg[1] = f16_gemm_set(gemm_khr, fixed_array(r.fw16_dev, r.fcol_dev, r.fy2_dev), fixed_array(r.fw16_bytes, bcol, by1), fixed_array(0u, VTF_COL, VTF_Y2))
    r.s_fb[1] = set_tower_ln_relu_cls(fixed_array(r.fy2_dev, r.fy2_dev, r.norms_dev), fixed_array(by1, by1, r.norms_bytes), fixed_array(VTF_Y2, VTF_Y2, 0u))
    r.s_fcvt = set_tower_clamp_cvt_cls(fixed_array(r.fy2_dev, r.ffeat_dev), fixed_array(by1, bfeat), fixed_array(VTF_Y2, VTF_FEAT))
    return VtBuild.built
}

[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vulkan_gemma4a_chunk(t : Gemma4aEncoder; var s : Gemma4aState; n_len : int64) : int64 {   // nolint:STYLE038 — the dispatch chain is the shape
    var r & = g_vt_g4a
    if (!vt_gate(r, t.q8)) {
        return -1l
    }
    if (t.dev_backend != TowerDev.vulkan) {
        vt_decline(r, VulkanTowerDecline.flavor)
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
    let kdp1 = round_up(ch0 * 9l, 64l)
    if (n_len < 4l || feat != d || ch0 != n_mel || ch1 % 16l != 0l || d % 64l != 0l || long_length(s.xw) < n_len * n_fft
            || npos > VT_MAX_ENCODE_ROWS) {
        vt_decline(r, VulkanTowerDecline.shape)
        return -1l
    }
    let tc = cm2_tile_cols(d, npos)
    let arm = vt_front_arm(r, n_len, 3008l,
        $(_gemm_khr : bool) => (ensure_tower_im2col_cls() && ensure_tower_spec_dft_cls() && ensure_tower_mel_log_cls() && ensure_tower_ln_relu_cls()
            && ensure_tower_clamp_cvt_cls() && cm2_cls_ensure(int(KqFmt.q8), tc)),
        $(var rr : VtResident) => vt_g4a_planes(rr, t) && (rr.front || vt_g4a_front_slab(rr, t)),
        $(var rr : VtResident; cap : int64; gemm_khr : bool) => vt_g4a_front_scratch(rr, t, cap, gemm_khr))
    if (!arm.ok) {
        return -1l
    }
    var s_in = cached(r.sets_front, int(tc)) $() : VkDescriptorSet {
        let cap2 = (r.front_cap - 1l) / 4l + 1l
        return cm2_cls_set(int(KqFmt.q8), tc, fixed_array(r.wq_dev, r.ws_dev, r.fmeta.buf, r.ffeat_dev, r.fx_dev),
            fixed_array(r.wq_bytes, r.ws_bytes, r.fmeta_bytes, (cap2 + TILE_READ_SLACK) * feat * 2l, cap2 * d * 4l), fixed_array(0u, 0u, 0u, VTF_FEAT, VTF_X))
    }
    let g_in = cm2_tiles_at(d, npos, tc)
    var mp = unsafe(reinterpret<uint?>(r.fmeta.mapped))
    var cur = 4l
    let map_off = batch_sched_map(mp, cur, 0l, t.inproj_w / kq_elems(KqFmt.q8), npos, g_in)
    upload_region_at(r.fxin_dev, 0l, unsafe(addr<uint8 const?>(s.xw[0])), n_len * n_fft * 4l)
    let bx = npos * d * 4l
    vt_chain_one_shot("gemma4a-front", 2l, npos) $(raw : VkCommandBuffer; var h : VkHaz) {
        var pc_dft = TowerSpecDftArgs(n_fft = uint(n_fft), n_frames = uint(n_len), mag = 1u)
        enc_tower_spec_dft_cls(raw, h, r.s_fdft, pc_dft, (n_len * n_bins + 255l) / 256l)
        vt_pt(raw, VtProfRole.spectrum)
        var pc_mel = GkMelLogArgs(n_bins = uint(n_bins), n_mel = uint(n_mel), frames = uint(n_len),
            lo = gemma4a_mel_floor(), hi = 0.0, scale = 1.0, frame_major = 1u, form = GK_MEL_LOG_MAX)
        enc_tower_mel_log_cls(raw, h, r.s_fmel, pc_mel, (n_len * n_mel + 255l) / 256l)
        vt_pt(raw, VtProfRole.mel)
        var pc_im0 = GkIm2col2dArgs(iw = uint(n_mel), ih = uint(n_len), cin = 1u, sstride = 1u, kdp = 64u, rows = uint(rows0), soff = 0u)
        enc_tower_im2col_cls(raw, h, r.s_fim[0], pc_im0, (rows0 * 64l + 255l) / 256l)
        vt_pt(raw, VtProfRole.im2col)
        var pc_g0 = F16GemmArgs(n = 64u, d = uint(ch0), rows = uint(rows0), woff = 0u, ybase = 0u, ystride = uint(ch0), ksplit = 0u)
        f16_gemm_enc(arm.gemm_khr, raw, h, r.s_fg[0], pc_g0, rows0, ch0)
        vt_pt(raw, VtProfRole.gemm_pw)
        var pc_ln0 = GkLnReluRowsArgs(cout = uint(ch0), ldin = uint(ch0), ldout = uint(ch0), rows = uint(rows0), woff = uint(t.norm0_w), eps = t.eps)
        enc_tower_ln_relu_cls(raw, h, r.s_fb[0], pc_ln0, (rows0 + 255l) / 256l)
        vt_pt(raw, VtProfRole.bias)
        var pc_im1 = GkIm2col2dArgs(iw = uint(w1), ih = uint(t1), cin = uint(ch0), sstride = uint(ch0), kdp = uint(kdp1), rows = uint(rows1), soff = 0u)
        enc_tower_im2col_cls(raw, h, r.s_fim[1], pc_im1, (rows1 * kdp1 + 255l) / 256l)
        vt_pt(raw, VtProfRole.im2col)
        var pc_g1 = F16GemmArgs(n = uint(kdp1), d = uint(ch1), rows = uint(rows1), woff = uint(ch0 * 64l), ybase = 0u, ystride = uint(ch1), ksplit = 0u)
        f16_gemm_enc(arm.gemm_khr, raw, h, r.s_fg[1], pc_g1, rows1, ch1)
        vt_pt(raw, VtProfRole.gemm_pw)
        var pc_ln1 = GkLnReluRowsArgs(cout = uint(ch1), ldin = uint(ch1), ldout = uint(ch1), rows = uint(rows1), woff = uint(t.norm1_w), eps = t.eps)
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
    host_buf_read(r.fhost, unsafe(addr<void?>(s.x[0])), bx)
    if (!vulkan_gemma4a_blocks(t, s, npos)) {
        return -1l
    }
    g_vt_convs++
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
def private vt_cn_front_slab(var r : VtResident; fc : VtFc) : bool {
    let btw = fc.device_mel ? fc.n_fft * 2l * fc.n_fb * 4l : 0l
    let bfil = fc.n_mel * fc.n_fb * 4l
    if (fc.dev_backend == TowerDev.vulkan) {
        let f16 = tower_dev_find(fc.devplan, VT_FC_FRONT16_PIECE)
        return f16.off >= 0l && vt_front_upload16(r, plane_at(fc.dev, f16.off), f16.bytes, fc.twiddles, btw, fc.filters, bfil)
    }
    var slab : array<float>
    tower_stage_conv0_panel(slab, fc.blob, fc.c0_w, fc.nch, 64l, fc.tap_major)
    return vt_front_upload(r, slab, fc.twiddles, btw, fc.filters, bfil)
}

//! the FastConformer front's f16 conv panel for a Vulkan device block, staged as the upload stages it
[cold_path]
def private vt_cn_front_bake(fc : VtFc; var b : TowerDevBuild) {
    var slab : array<float>
    tower_stage_conv0_panel(slab, fc.blob, fc.c0_w, fc.nch, 64l, fc.tap_major)
    var inscope slab16 <- cvt_f32_to_f16_sat_array(slab)
    delete slab
    tower_dev_put(b, VT_FC_FRONT16_PIECE, unsafe(addr<uint8 const?>(slab16[0])), long_length(slab16) * 2l)
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
def private vt_cn_front_scratch(var r : VtResident; fc : VtFc; cap : int64; gemm_khr : bool) : VtBuild {
    vt_front_release(r)
    r.oom = false
    let n_fft = fc.n_fft
    let n_fb = fc.n_fb
    let n_mel = fc.n_mel
    let nch = fc.nch
    let d = fc.d
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
    if (!vk_range_ok(max(max(by0, bh), max(bxf, bx)))) {
        return VtBuild.shape
    }
    r.front_cap = cap
    r.fxin_dev = vt_call_buf(r, bxw)
    r.fy3_dev = vt_dev_buf(r, bpow)
    r.fmel_dev = vt_call_buf(r, bmel)
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
        return VtBuild.memory
    }
    r.s_fdft = set_tower_spec_dft_cls(fixed_array(r.fxin_dev, r.front_twiddles_dev, r.fy3_dev), fixed_array(bxw, btw, bpow), fixed_array(VTF_XIN, 0u, VTF_Y3))
    r.s_fmel = set_tower_mel_log_cls(fixed_array(r.fy3_dev, r.front_filters_dev, r.fmel_dev), fixed_array(bpow, bfil, bmel), fixed_array(VTF_Y3, 0u, VTF_MEL))
    r.s_fb[2] = set_tower_mel_norm_cls(fixed_array(r.fmel_dev), fixed_array(bmel), fixed_array(VTF_MEL))
    r.s_fim[0] = set_tower_im2col_cls(fixed_array(r.fmel_dev, r.fcol_dev), fixed_array(bmel, bcol), fixed_array(VTF_MEL, VTF_COL))
    r.s_fg[0] = f16_gemm_set(gemm_khr, fixed_array(r.fw16_dev, r.fcol_dev, r.fy1_dev), fixed_array(r.fw16_bytes, bcol, by0), fixed_array(0u, VTF_COL, VTF_Y1))
    r.s_fb[0] = set_tower_bias_act_cls(fixed_array(r.fy1_dev, r.norms_dev), fixed_array(by0, r.norms_bytes), fixed_array(VTF_Y1, 0u))
    if (fc.tap_major) {
        r.s_fg[1] = set_tower_pk_dw2d_cls(fixed_array(r.fy1_dev, r.fy2_dev, r.norms_dev), fixed_array(by0, by2, r.norms_bytes), fixed_array(VTF_Y1, VTF_Y2, 0u))
        r.s_fg[2] = set_tower_pk_dw2d_cls(fixed_array(r.fcn_pw3_dev, r.fcn_dw5_dev, r.norms_dev), fixed_array(by2, by3, r.norms_bytes), fixed_array(VTF_Y4, VTF_Y5, 0u))
    } else {
        r.s_fg[1] = set_tower_dw_conv_cls(fixed_array(r.fy1_dev, r.fy2_dev, r.norms_dev), fixed_array(by0, by2, r.norms_bytes), fixed_array(VTF_Y1, VTF_Y2, 0u))
        r.s_fg[2] = set_tower_dw_conv_cls(fixed_array(r.fcn_pw3_dev, r.fcn_dw5_dev, r.norms_dev), fixed_array(by2, by3, r.norms_bytes), fixed_array(VTF_Y4, VTF_Y5, 0u))
    }
    r.s_fb[1] = set_tower_bias_act_cls(fixed_array(r.fcn_pw3_dev, r.norms_dev), fixed_array(by2, r.norms_bytes), fixed_array(VTF_Y4, 0u))
    r.s_fcvt = set_tower_clamp_cvt_cls(fixed_array(r.fy2_dev, r.fcn_feed16_dev), fixed_array(by2, bh), fixed_array(VTF_Y2, VTF_H))
    r.s_fshuf = set_tower_feat_shuffle_cls(fixed_array(r.ffeat_dev, r.fcn_feat_dev), fixed_array(by3, bxf), fixed_array(VTF_FEAT, VTF_XF))
    r.s_ffin = set_tower_bias_act_cls(fixed_array(r.fx_dev, r.norms_dev), fixed_array(bx, r.norms_bytes), fixed_array(VTF_X, 0u))
    r.s_fcn_feed16_dw5 = set_tower_clamp_cvt_cls(fixed_array(r.fcn_dw5_dev, r.fcn_feed16_dev), fixed_array(by3, bh), fixed_array(VTF_Y5, VTF_H))
    r.s_fcn_feed16_feat = set_tower_clamp_cvt_cls(fixed_array(r.fcn_feat_dev, r.fcn_feed16_dev), fixed_array(bxf, bh), fixed_array(VTF_XF, VTF_H))
    r.s_fcn_bias_pw6 = set_tower_bias_act_cls(fixed_array(r.ffeat_dev, r.norms_dev), fixed_array(by3, r.norms_bytes), fixed_array(VTF_FEAT, 0u))
    return VtBuild.built
}

//! the front's cm2 GEMM set of one stage (the pointwise convs into y3 and y6, the input projection into x) at a token column
def private vt_cn_front_set(var r : VtResident; stage : int; tc : int64; y_dev : uint64; y_bytes : int64; y_bit : uint) : VkDescriptorSet {
    return cached(r.sets_front, stage * VT_CN_FRONT_KEY + int(tc)) $() : VkDescriptorSet {
        return cm2_cls_set(int(KqFmt.q8), tc, fixed_array(r.wq_dev, r.ws_dev, r.fmeta.buf, r.fcn_feed16_dev, y_dev),
            fixed_array(r.wq_bytes, r.ws_bytes, r.fmeta_bytes, r.fh_bytes, y_bytes), fixed_array(0u, 0u, 0u, VTF_H, y_bit))
    }
}

[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vt_fc_front(var r : VtResident; fc : VtFc; src : float const?; src_elems, n_len, n_valid : int64; var x : array<float>;   // nolint:STYLE038 — the dispatch chain is the shape
                        blocks_behind : bool) : int64 {
    let n_mel = fc.n_mel
    let n_fb = fc.n_fb
    let n_fft = fc.n_fft
    let nch = fc.nch
    let d = fc.d
    let g = vt_cn_geom(n_len, n_mel)
    let rows0 = g.t1 * g.w1
    let rows2 = g.t2 * g.w2
    let rows3 = g.t3 * g.w3
    let tt = g.t3
    let feat = g.w3 * nch
    let l0 = conv_s2_out(n_valid)
    let l2 = conv_s2_out(l0)
    if (n_len < 4l || nch % 64l != 0l || feat % 64l != 0l || d % 64l != 0l || n_fb != n_fft / 2l + 1l || src_elems < n_len * (fc.device_mel ? n_fft : n_mel)
            || !vt_cn_rows_ok(tt, d, fc.ff)) {
        vt_decline(r, VulkanTowerDecline.shape)
        return -1l
    }
    let tc2 = cm2_tile_cols(nch, rows2)
    let tc3 = cm2_tile_cols(nch, rows3)
    let tcx = cm2_tile_cols(d, tt)
    let arm = vt_front_arm(r, n_len, 4096l,
        $(_gemm_khr : bool) => (ensure_tower_im2col_cls() && ensure_tower_spec_dft_cls() && ensure_tower_mel_log_cls() && ensure_tower_mel_norm_cls()
            && ensure_tower_dw_conv_cls() && ensure_tower_pk_dw2d_cls() && ensure_tower_feat_shuffle_cls() && ensure_tower_bias_act_cls()
            && ensure_tower_clamp_cvt_cls() && cm2_cls_ensure(int(KqFmt.q8), tc2) && cm2_cls_ensure(int(KqFmt.q8), tc3) && cm2_cls_ensure(int(KqFmt.q8), tcx)),
        $(var rr : VtResident) => vt_fc_planes(rr, fc) && (rr.front || vt_cn_front_slab(rr, fc)),
        $(var rr : VtResident; cap : int64; gemm_khr : bool) => vt_cn_front_scratch(rr, fc, cap, gemm_khr))
    if (!arm.ok || (blocks_behind && !vt_fc_admits(fc, tt, 2l * tt - 1l, arm.khr, arm.gemm_khr))) {
        return -1l
    }
    r.d = d
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
    let map3 = batch_sched_map(mp, cur, 0l, fc.c3_w / q8e, rows2, g_pw3)
    let map6 = batch_sched_map(mp, cur, 1l, fc.c6_w / q8e, rows3, g_pw6)
    let mapx = batch_sched_map(mp, cur, 2l, fc.pre_w / q8e, tt, g_pre)
    if (fc.device_mel) {
        upload_region_at(r.fxin_dev, 0l, unsafe(reinterpret<uint8 const?>(src)), n_len * n_fft * 4l)
    } else {
        upload_region_at(r.fmel_dev, 0l, unsafe(reinterpret<uint8 const?>(src)), n_len * n_mel * 4l)
    }
    let bx = tt * d * 4l
    vt_chain_one_shot(fc.front_name, 3l, tt) $(raw : VkCommandBuffer; var h : VkHaz) {
        if (fc.device_mel) {
            var pc_dft = TowerSpecDftArgs(n_fft = uint(n_fft), n_frames = uint(n_len), mag = 0u)
            enc_tower_spec_dft_cls(raw, h, r.s_fdft, pc_dft, (n_len * n_fb + 255l) / 256l)
            vt_pt(raw, VtProfRole.spectrum)
            var pc_mel = GkMelLogArgs(n_bins = uint(n_fb), n_mel = uint(n_mel), frames = uint(n_len),
                lo = VT_MEL_FLOOR, hi = 0.0, scale = 1.0, frame_major = 1u, form = GK_MEL_LOG_ADD)
            enc_tower_mel_log_cls(raw, h, r.s_fmel, pc_mel, (n_len * n_mel + 255l) / 256l)
            var pc_norm = GkMelNormArgs(n_mel = uint(n_mel), n_frames = uint(n_len), valid = uint(min(n_valid, n_len)))
            enc_tower_mel_norm_cls(raw, h, r.s_fb[2], pc_norm, (n_mel + 63l) / 64l)
            vt_pt(raw, VtProfRole.mel)
        }
        var pc_im0 = GkIm2col2dArgs(iw = uint(n_mel), ih = uint(n_len), cin = 1u, sstride = 1u, kdp = 64u, rows = uint(rows0), soff = 0u)
        enc_tower_im2col_cls(raw, h, r.s_fim[0], pc_im0, (rows0 * 64l + 255l) / 256l)
        vt_pt(raw, VtProfRole.im2col)
        var pc_g0 = F16GemmArgs(n = 64u, d = uint(nch), rows = uint(rows0), woff = 0u, ybase = 0u, ystride = uint(nch), ksplit = 0u)
        f16_gemm_enc(arm.gemm_khr, raw, h, r.s_fg[0], pc_g0, rows0, nch)
        vt_pt(raw, VtProfRole.gemm_pw)
        vt_bias_act(raw, h, r.s_fb[0], nch, rows0 * nch, fc.c0_b, BIAS_ACT_RELU)
        vt_pt(raw, VtProfRole.bias)
        var pc_dw2 = GkDwConv2dArgs(iw = uint(g.w1), ih = uint(g.t1), ih_valid = uint(l0), ch = uint(nch), woff = uint(fc.c2_w), boff = uint(fc.c2_b))
        vt_fc_dw2d(raw, h, fc.tap_major, r.s_fg[1], pc_dw2, (rows2 * nch + 255l) / 256l)
        vt_pt(raw, VtProfRole.dwconv)
        vt_cvt16(raw, h, r.s_fcvt, rows2 * nch)
        var pc_pw3 = BatchArgs(n = uint(nch), d = uint(nch), map_off = map3)
        cm2_cls_enc(int(KqFmt.q8), tc2, raw, h, s_pw3, pc_pw3, g_pw3)
        vt_pt(raw, VtProfRole.gemm_pw)
        vt_bias_act(raw, h, r.s_fb[1], nch, rows2 * nch, fc.c3_b, BIAS_ACT_RELU)
        vt_pt(raw, VtProfRole.bias)
        var pc_dw5 = GkDwConv2dArgs(iw = uint(g.w2), ih = uint(g.t2), ih_valid = uint(l2), ch = uint(nch), woff = uint(fc.c5_w), boff = uint(fc.c5_b))
        vt_fc_dw2d(raw, h, fc.tap_major, r.s_fg[2], pc_dw5, (rows3 * nch + 255l) / 256l)
        vt_pt(raw, VtProfRole.dwconv)
        vt_cvt16(raw, h, r.s_fcn_feed16_dw5, rows3 * nch)
        var pc_pw6 = BatchArgs(n = uint(nch), d = uint(nch), map_off = map6)
        cm2_cls_enc(int(KqFmt.q8), tc3, raw, h, s_pw6, pc_pw6, g_pw6)
        vt_pt(raw, VtProfRole.gemm_pw)
        vt_bias_act(raw, h, r.s_fcn_bias_pw6, nch, rows3 * nch, fc.c6_b, BIAS_ACT_RELU)
        vt_pt(raw, VtProfRole.bias)
        var pc_sh = GkFeatShuffleArgs(w = uint(g.w3), ch = uint(nch), t = uint(tt), rows_pad = uint(tt))
        enc_tower_feat_shuffle_cls(raw, h, r.s_fshuf, pc_sh, (tt * feat + 255l) / 256l)
        vt_pt(raw, VtProfRole.shuffle)
        vt_cvt16(raw, h, r.s_fcn_feed16_feat, tt * feat)
        var pc_pre = BatchArgs(n = uint(feat), d = uint(d), map_off = mapx)
        cm2_cls_enc(int(KqFmt.q8), tcx, raw, h, s_pre, pc_pre, g_pre)
        vt_pt(raw, VtProfRole.gemm_pw)
        vt_bias_act(raw, h, r.s_ffin, d, tt * d, fc.pre_b, BIAS_ACT_NONE)
        vt_pt(raw, VtProfRole.bias)
        vhz_dep(raw, h, VTF_X, 0u, true)
        cmd_copy_range(raw, r.fx_dev, 0l, r.fhost.buf, 0l, bx)
        vt_pt(raw, VtProfRole.copy_out)
    }
    ensure_length(x, tt * d)
    host_buf_read(r.fhost, unsafe(addr<void?>(x[0])), bx)
    return tt
}

[hot_path]
def private vulkan_canary_front(t : CanaryEncoder; var s : CanaryState) : int64 {
    var r & = g_vt_cn
    if (!vt_gate(r, t.q8)) {
        return -1l
    }
    if (t.dev_backend != TowerDev.vulkan) {
        vt_decline(r, VulkanTowerDecline.flavor)
        return -1l
    }
    let fc = vt_fc_of_cn(t)
    let tt = vt_fc_front(r, fc, unsafe(addr<float const?>(s.xw[0])), long_length(s.xw), s.n_len, s.n_valid, s.x, false)
    if (tt >= 0l) {
        g_vt_convs++
    }
    return tt
}

//! the rotary family's two stride-2 conv panels as one f16 slab: conv0 [nch][kd0] at 0, conv2 [d][kd2] behind it, each row's
//! taps on the f16 GEMM's 64 lattice (zero past them)
[cold_path]
def private vt_ga_front_slab(var r : VtResident; m : GigaamModel) : bool {
    if (m.dev_backend == TowerDev.vulkan) {
        let f16 = tower_dev_find(m.devplan, VT_FC_FRONT16_PIECE)
        let twin = tower_dev_find(m.devplan, VT_GA_TWIN_PIECE)
        return false if (f16.off < 0l || twin.off < 0l)
        return vt_front_upload16(r, plane_at(m.dev, f16.off), f16.bytes, PlaneF(p = unsafe(reinterpret<float const?>(plane_at(m.dev, twin.off))),
            n = twin.bytes / 4l), twin.bytes, m.filters, m.n_mel * (m.n_fft / 2l + 1l) * 4l)
    }
    let k = m.subs_kernel
    let nch = m.n_sub_ch
    let d = m.d_model
    let kd0 = round_up(k * m.n_mel, 64l)
    let kd2 = round_up(k * nch, 64l)
    var slab : array<float>
    slab |> reserve_resize(nch * kd0 + d * kd2)
    tower_stage_f32_pad_row_stride(slab, m.blob, m.c0_w, 0l, nch, k * m.n_mel, kd0)
    tower_stage_f32_pad_row_stride(slab, m.blob, m.c2_w, nch * kd0, d, k * nch, kd2)
    let n_bins = m.n_fft / 2l + 1l
    let ntw = m.n_fft * 2l * n_bins
    if (long_length(m.tabs.twiddles) < ntw || length(m.window) < m.n_fft || length(m.filters) < m.n_mel * n_bins) {
        delete slab
        return false
    }
    var inscope twin : array<float>   //! the device mel's DFT twiddles [n_fft][2 n_bins], the window behind them (`vt_ga_mel_woff`)
    twin |> reserve(ntw + m.n_fft)
    for (i in range64(ntw)) {
        twin |> push(m.tabs.twiddles[i])
    }
    for (j in range64(m.n_fft)) {
        twin |> push(m.window[j])
    }
    let tw_plane = PlaneF(p = unsafe(addr<float const?>(twin[0])), n = long_length(twin))
    return vt_front_upload(r, slab, tw_plane, long_length(twin) * 4l, m.filters, m.n_mel * n_bins * 4l)
}

let private VT_GA_COL_LATTICE = 128l   //! the gigaam front's column panel rows: the widest f16 tile's (the cm2 tile's 128, the KHR tile's 64)

//! the rotary family's front scratch at `cap` mel frames: the mel rows, one f16 column panel both convs share, conv0's rows,
//! the residual rows and their readback
[cold_path]
def private vt_ga_front_scratch(var r : VtResident; m : GigaamModel; cap : int64; gemm_khr : bool) : VtBuild {
    vt_front_release(r)
    r.oom = false
    let k = m.subs_kernel
    let nch = m.n_sub_ch
    let d = m.d_model
    let t1 = gigaam_conv_out(cap, k)
    let tt = gigaam_conv_out(t1, k)
    let kd0 = round_up(k * m.n_mel, 64l)
    let kd2 = round_up(k * nch, 64l)
    let bmel = cap * m.n_mel * 4l
    let bcol = (round_up(max(t1, tt), VT_GA_COL_LATTICE) + TILE_READ_SLACK) * max(kd0, kd2) * 2l
    let by1 = t1 * nch * 4l
    let bx = tt * d * 4l
    let n_bins = m.n_fft / 2l + 1l
    let bsamp = ((cap - 1l) * m.hop + m.n_fft) * 4l
    let bxw = cap * m.n_fft * 4l
    let bpow = cap * n_bins * 4l
    if (!vk_range_ok(max(max(max(bmel, bcol), max(by1, bx)), max(bsamp, max(bxw, bpow))))) {
        return VtBuild.shape
    }
    r.front_cap = cap
    r.fmel_dev = vt_call_buf(r, bmel)
    r.fcol_dev = vt_dev_buf(r, bcol)
    r.fy1_dev = vt_dev_buf(r, by1)
    r.fx_dev = vt_dev_buf(r, bx)
    r.fxin_dev = vt_call_buf(r, bsamp)  //! the device mel's raw samples, its windowed frames and its power rows
    r.fy2_dev = vt_dev_buf(r, bxw)
    r.fy3_dev = vt_dev_buf(r, bpow)
    r.fhost = vt_host_buf(r, bx, true)
    if (r.oom) {
        vt_front_release(r)
        return VtBuild.memory
    }
    let btw = (vt_ga_mel_woff(m.n_fft) + m.n_fft) * 4l
    let bfil = m.n_mel * n_bins * 4l
    r.s_fshuf = set_tower_ga_frames_cls(fixed_array(r.fxin_dev, r.front_twiddles_dev, r.fy2_dev), fixed_array(bsamp, btw, bxw), fixed_array(VTF_XIN, 0u, VTF_Y2))
    r.s_fdft = set_tower_spec_dft_cls(fixed_array(r.fy2_dev, r.front_twiddles_dev, r.fy3_dev), fixed_array(bxw, btw, bpow), fixed_array(VTF_Y2, 0u, VTF_Y3))
    r.s_fmel = set_tower_mel_log_cls(fixed_array(r.fy3_dev, r.front_filters_dev, r.fmel_dev), fixed_array(bpow, bfil, bmel), fixed_array(VTF_Y3, 0u, VTF_MEL))
    r.s_fim[0] = set_tts_im2col16_cls(fixed_array(r.fmel_dev, r.fcol_dev), fixed_array(bmel, bcol), fixed_array(VTF_MEL, VTF_COL))
    r.s_fg[0] = f16_gemm_set(gemm_khr, fixed_array(r.fw16_dev, r.fcol_dev, r.fy1_dev), fixed_array(r.fw16_bytes, bcol, by1), fixed_array(0u, VTF_COL, VTF_Y1))
    r.s_fb[0] = set_tower_bias_act_cls(fixed_array(r.fy1_dev, r.norms_dev), fixed_array(by1, r.norms_bytes), fixed_array(VTF_Y1, 0u))
    r.s_fim[1] = set_tts_im2col16_cls(fixed_array(r.fy1_dev, r.fcol_dev), fixed_array(by1, bcol), fixed_array(VTF_Y1, VTF_COL))
    r.s_fg[1] = f16_gemm_set(gemm_khr, fixed_array(r.fw16_dev, r.fcol_dev, r.fx_dev), fixed_array(r.fw16_bytes, bcol, bx), fixed_array(0u, VTF_COL, VTF_X))
    r.s_ffin = set_tower_bias_act_cls(fixed_array(r.fx_dev, r.norms_dev), fixed_array(bx, r.norms_bytes), fixed_array(VTF_X, 0u))
    return VtBuild.built
}

//! the gigaam resident armed for `n_len` frames: the device, the front's classes, the q8 planes, the front slab and the scratch;
//! ok false with the decline counted
def private vt_ga_front_arm(var r : VtResident; fc : VtFc; m : GigaamModel; n_len : int64) : tuple<ok : bool; khr : bool; gemm_khr : bool> {
    return vt_front_arm(r, n_len, GIGAAM_MAX_CLIP_S * AUDIO_SAMPLE_RATE / max(m.hop, 1l),
        $(_gemm_khr : bool) => (ensure_tts_im2col16_cls() && ensure_tower_bias_act_cls() && ensure_tower_ga_frames_cls()
            && ensure_tower_spec_dft_cls() && ensure_tower_mel_log_cls()),
        $(var rr : VtResident) => vt_fc_planes(rr, fc) && (rr.front || vt_ga_front_slab(rr, m)),
        $(var rr : VtResident; cap : int64; gemm_khr : bool) => vt_ga_front_scratch(rr, m, cap, gemm_khr))
}

//! the device mel's record off the samples uploaded at `fxin_dev` (`#gigaam-log-mel`): the frame window, the power spectrum and
//! the clamped-log filterbank into the frame-major mel rows at `fmel_dev`
def private vt_ga_mel_rec(raw : VkCommandBuffer; var h : VkHaz; var r : VtResident; m : GigaamModel; n_len : int64) {
    let n_bins = m.n_fft / 2l + 1l
    var pc_fw = GkFrameWindowArgs(n_fft = uint(m.n_fft), hop = uint(m.hop), frames = uint(n_len), rows = uint(n_len),
        woff = uint(vt_ga_mel_woff(m.n_fft)))
    enc_tower_ga_frames_cls(raw, h, r.s_fshuf, pc_fw, (n_len * m.n_fft + 255l) / 256l)
    var pc_dft = TowerSpecDftArgs(n_fft = uint(m.n_fft), n_frames = uint(n_len), mag = 0u)
    enc_tower_spec_dft_cls(raw, h, r.s_fdft, pc_dft, (n_len * n_bins + 255l) / 256l)
    vt_pt(raw, VtProfRole.spectrum)
    var pc_mel = GkMelLogArgs(n_bins = uint(n_bins), n_mel = uint(m.n_mel), frames = uint(n_len),
        lo = GIGAAM_MEL_FLOOR, hi = GIGAAM_MEL_CEIL, scale = 1.0, frame_major = 1u, form = GK_MEL_LOG_CLAMP)
    enc_tower_mel_log_cls(raw, h, r.s_fmel, pc_mel, (n_len * m.n_mel + 255l) / 256l)
    vt_pt(raw, VtProfRole.mel)
}

//! The gigaam log-mel of `samples` on the device alone, read back frame-major into `mel` - the encode chain's mel record in a
//! one-shot command of its own, the twin's seat; the frame count, or -1 with the decline counted.
def vulkan_gigaam_mel(m : GigaamModel; samples : array<float>; @exact_size var mel : array<float>) : int64 {
    var r & = g_vt_ga
    if (!vt_gate(r, vt_ga_lane_served(m))) {
        return -1l
    }
    let n_len = gigaam_mel_frames(m, long_length(samples))
    if (n_len < 1l || m.hop < 1l) {
        vt_decline(r, VulkanTowerDecline.shape)
        return -1l
    }
    let fc = vt_fc_of_ga(m)
    return -1l if (!vt_ga_front_arm(r, fc, m, n_len).ok)
    upload_region_at(r.fxin_dev, 0l, unsafe(addr<uint8 const?>(samples[0])), ((n_len - 1l) * m.hop + m.n_fft) * 4l)
    let bmel = n_len * m.n_mel * 4l
    vt_chain_one_shot("gigaam-mel", 1l, n_len) $(raw : VkCommandBuffer; var h : VkHaz) {
        vt_ga_mel_rec(raw, h, r, m, n_len)
        vhz_dep(raw, h, VTF_MEL, 0u, true)
        cmd_copy_range(raw, r.fmel_dev, 0l, r.fhost.buf, 0l, bmel)
        vt_pt(raw, VtProfRole.copy_out)
    }
    mel |> reserve(n_len * m.n_mel)
    mel |> resize(n_len * m.n_mel)
    host_buf_read(r.fhost, unsafe(addr<void?>(mel[0])), bmel)
    g_vt_mels++
    return n_len
}

//! the rotary family's conv front off the device mel of the raw `samples` (`#gigaam-log-mel`), else the CPU mel rows: two f16
//! im2col + GEMM + bias-ReLU convs, the rows read back into `x`; the frame count, or -1 - the front's decline counted, none where
//! the blocks would not admit the rows (the block seat counts that); the encode bumps the counters
[arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_FC.md#vk-tower-fc-rotary")]
def private vt_ga_front(var r : VtResident; fc : VtFc; m : GigaamModel; var st : GigaamState; samples : float const?; n_in : int64) : int64 {
    let k = m.subs_kernel
    let nch = m.n_sub_ch
    let d = m.d_model
    let n_len = st.n_len
    let t1 = gigaam_conv_out(n_len, k)
    let tt = gigaam_conv_out(t1, k)
    let kd0 = round_up(k * m.n_mel, 64l)
    let kd2 = round_up(k * nch, 64l)
    let dev_mel = samples != null
    let n_use = (n_len - 1l) * m.hop + m.n_fft   //! the samples the frames reach
    if (n_len < 1l || t1 < 1l || tt < 1l || k % 2l != 1l || nch % 64l != 0l || d % 64l != 0l
            || (dev_mel ? (m.hop < 1l || n_use > n_in) : long_length(st.mel) < n_len * m.n_mel)) {
        vt_decline(r, VulkanTowerDecline.shape)
        return -1l
    }
    let arm = vt_ga_front_arm(r, fc, m, n_len)
    return -1l if (!arm.ok || !vt_fc_admits(fc, tt, 0l, arm.khr, arm.gemm_khr))
    if (dev_mel) {
        upload_region_at(r.fxin_dev, 0l, unsafe(reinterpret<uint8 const?>(samples)), n_use * 4l)
    } else {
        upload_region_at(r.fmel_dev, 0l, unsafe(addr<uint8 const?>(st.mel[0])), n_len * m.n_mel * 4l)
    }
    let pad = uint((k - 1l) / 2l)
    let bx = tt * d * 4l
    let mp1 = round_up(t1, VT_GA_COL_LATTICE)   //! each conv's column panel on the f16 tile's row lattice: the pad rows read zero
    let mp2 = round_up(tt, VT_GA_COL_LATTICE)
    vt_chain_one_shot(fc.front_name, 2l, tt) $(raw : VkCommandBuffer; var h : VkHaz) {
        if (dev_mel) {
            vt_ga_mel_rec(raw, h, r, m, n_len)
        }
        var pc_im0 = GkIm2col1dArgs(cin = uint(m.n_mel), tin = uint(n_len), tout = uint(t1), k = uint(k), stride = 2u, pad_l = pad, dilation = 1u,
            kdim = uint(kd0), row_major = 1u, r0 = 0u, mp = uint(mp1))
        enc_tts_im2col16_cls(raw, h, r.s_fim[0], pc_im0, mp1 * kd0)
        vt_pt(raw, VtProfRole.im2col)
        var pc_g0 = F16GemmArgs(n = uint(kd0), d = uint(nch), rows = uint(t1), woff = 0u, ybase = 0u, ystride = uint(nch), ksplit = 0u)
        f16_gemm_enc(arm.gemm_khr, raw, h, r.s_fg[0], pc_g0, t1, nch)
        vt_pt(raw, VtProfRole.gemm_pw)
        vt_bias_act(raw, h, r.s_fb[0], nch, t1 * nch, m.c0_b, BIAS_ACT_RELU)
        vt_pt(raw, VtProfRole.bias)
        var pc_im2 = GkIm2col1dArgs(cin = uint(nch), tin = uint(t1), tout = uint(tt), k = uint(k), stride = 2u, pad_l = pad, dilation = 1u,
            kdim = uint(kd2), row_major = 1u, r0 = 0u, mp = uint(mp2))
        enc_tts_im2col16_cls(raw, h, r.s_fim[1], pc_im2, mp2 * kd2)
        vt_pt(raw, VtProfRole.im2col)
        var pc_g2 = F16GemmArgs(n = uint(kd2), d = uint(d), rows = uint(tt), woff = uint(nch * kd0), ybase = 0u, ystride = uint(d), ksplit = 0u)
        f16_gemm_enc(arm.gemm_khr, raw, h, r.s_fg[1], pc_g2, tt, d)
        vt_pt(raw, VtProfRole.gemm_pw)
        vt_bias_act(raw, h, r.s_ffin, d, tt * d, m.c2_b, BIAS_ACT_RELU)
        vt_pt(raw, VtProfRole.bias)
        vhz_dep(raw, h, VTF_X, 0u, true)
        cmd_copy_range(raw, r.fx_dev, 0l, r.fhost.buf, 0l, bx)
        vt_pt(raw, VtProfRole.copy_out)
    }
    ensure_length(st.x, tt * d)
    host_buf_read(r.fhost, unsafe(addr<void?>(st.x[0])), bx)
    return tt
}

var private g_vt_env_ga_front = true

//! Serve the GigaAM conv front on the device (`on`), or leave it to the CPU chain with the block seat serving behind it;
//! answers the setting it replaced.
def set_vulkan_gigaam_front(on : bool) : bool {
    let was = g_vt_env_ga_front
    g_vt_env_ga_front = on
    return was
}

def private vt_ga_lane_served(m : GigaamModel) : bool => m.kq_lane || m.q8 || m.gemm_f16

var private g_vt_env_ga_exact = false

//! Prefer GigaAM's exact lane - its block GEMMs on the f16 GEMM over the carrier's halfwords - over the q8 image for the next
//! un-pinned loads (`DASLLAMA_VK_GIGAAM_EXACT` is the env twin); both lanes serve on the device either way. Answers the setting
//! it replaced.
def set_vulkan_gigaam_exact_lane(on : bool) : bool {
    let was = g_vt_env_ga_exact
    g_vt_env_ga_exact = on
    static_if (!typeinfo builtin_module_exists(das_metal)) {
        register_gigaam_gpu(@@vulkan_gigaam_blocks, @@vulkan_tower_on, on, true)
    }
    return was
}

//! the GigaAM block loop behind the CPU front and the session's rope tables, on the lane's planes
[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_FC.md#vk-tower-fc-rotary")]
def private vulkan_gigaam_blocks(m : GigaamModel; var st : GigaamState; tt : int64) : bool {
    var r & = g_vt_ga
    g_vt_fc_out_bytes = 0l
    if (!vt_gate(r, vt_ga_lane_served(m))) {
        return false
    }
    return vt_decline(r, VulkanTowerDecline.flavor) if (m.dev_backend != TowerDev.vulkan)
    let fc = vt_fc_of_ga(m)
    return vt_fc_blocks(r, fc, st.x, tt, 0l, st.rope_cos, st.rope_sin)
}

//! the GigaAM encode whole where the front lever is on: the conv front off the device mel of the raw samples (the CPU mel where
//! `samples` is null), the rope tables, then the blocks over the front's rows; -1 with nothing counted where the front cannot serve (the CPU front runs, the block seat answers for itself)
//! or where the blocks declined behind a served front
[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_FC.md#vk-tower-fc-rotary")]
def private vulkan_gigaam_encode(m : GigaamModel; var st : GigaamState; samples : float const?; n_in : int64) : int64 {
    var r & = g_vt_ga
    g_vt_fc_out_bytes = 0l
    if (!g_vt_env_tower) {
        vt_decline(r, VulkanTowerDecline.knob)
        return -1l
    }
    if (!g_vt_env_ga_front) {
        vt_decline(r, VulkanTowerDecline.ga_front)
        return -1l
    }
    if (!vt_ga_lane_served(m)) {
        vt_decline(r, VulkanTowerDecline.quant_mode)
        return -1l
    }
    if (m.dev_backend != TowerDev.vulkan) {
        vt_decline(r, VulkanTowerDecline.flavor)
        return -1l
    }
    let fc = vt_fc_of_ga(m)
    let tt = vt_ga_front(r, fc, m, st, samples, n_in)
    return -1l if (tt < 0l)
    gigaam_rope_tables(m, st, tt)
    return -1l if (!vt_fc_blocks(r, fc, st.x, tt, 0l, st.rope_cos, st.rope_sin))
    if (samples != null) {
        g_vt_mels++
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
    let bxw = cap * n_fft * 4l
    let btw = n_fft * 2l * n_bins * 4l
    let bfil = n_mel * n_bins * 4l
    let bpow = cap * n_bins * 4l
    let bout = n_mel * cap * 4l
    if (!vk_range_ok(max(bxw, max(bpow, bout)))) {
        return false
    }
    r.cap_rows = cap
    r.x_dev = vt_call_buf(r, bxw)
    r.xb_dev = vt_call_buf(r, btw)
    r.xq_dev = vt_call_buf(r, bfil)
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
        var pc_m = GkMelLogArgs(n_bins = uint(n_bins), n_mel = uint(n_mel), frames = uint(n_len),
            lo = VT_MEL_FLOOR, hi = 0.0, scale = VT_LOG10_E, frame_major = 0u, form = GK_MEL_LOG_MAX)
        enc_tower_mel_log_cls(raw, h, r.s_k, pc_m, (n_mel * n_len + 255l) / 256l)
        vt_pt(raw, VtProfRole.mel)
        vhz_dep(raw, h, VT_YO, 0u, true)
        cmd_copy_range(raw, r.yo_dev, 0l, r.host.buf, 0l, bout)
        vt_pt(raw, VtProfRole.copy_out)
    }
    host_buf_read(r.host, unsafe(addr<void?>(m.data[0])), bout)
    vt_announce(r)
    g_vt_mels++
    return true
}

// ===== the silero VAD seat: the encoder over every chunk of a call, then the recurrence over them in one workgroup =====

var private g_vt_vad : VtResident   //! the VAD's resident: wq_dev = the weight slab, x_dev = the context and the chunks, q_dev = the input gates, k_dev = [h | c], yo_dev = the probabilities
var private @scratch @exact_size g_vt_vad_x : array<float>   //! the upload's staging: the stream's context, the samples, the zero tail
var private g_vt_vad_calls = 0l
var private g_vt_vad_chunks = 0l

//! The VAD calls and chunks the device served.
def vulkan_tower_vad_stats : tuple<calls : int64; chunks : int64> => (calls = g_vt_vad_calls, chunks = g_vt_vad_chunks)

//! the VAD's weight slab on the device, uploaded where its key moved
def private vt_vad_upload(var r : VtResident; m : VadModel) : VtBuild {
    return VtBuild.built if (r.key == m.slab_key && r.wq_dev != 0ul)
    return vt_vad_upload_build(r, m)
}

[cold_path]
def private vt_vad_upload_build(var r : VtResident; m : VadModel) : VtBuild {
    vt_upload_begin(r, m.slab_key)
    r.wq_bytes = long_length(m.slab) * 4l
    if (!vk_range_ok(r.wq_bytes)) {
        vt_release(r)
        return VtBuild.shape
    }
    r.wq_dev = vt_dev_buf(r, r.wq_bytes)
    if (r.oom) {
        vt_release(r)
        return VtBuild.memory
    }
    upload_region_at(r.wq_dev, 0l, unsafe(addr<uint8 const?>(m.slab[0])), r.wq_bytes)
    return VtBuild.built
}

[cold_path]
def private vt_vad_scratch(var r : VtResident; cap : int64) : VtBuild {
    vt_release_scratch(r)
    r.oom = false
    let bx = (64l + cap * GK_VAD_CHUNK) * 4l
    let bgx = cap * 4l * GK_VAD_HIDDEN * 4l
    let bst = 2l * GK_VAD_HIDDEN * 4l
    let bp = cap * 4l
    if (!vk_range_ok(max(bx, bgx))) {
        return VtBuild.shape
    }
    r.cap_rows = cap
    r.x_dev = vt_call_buf(r, bx)
    r.q_dev = vt_dev_buf(r, bgx)
    r.k_dev = vt_call_buf(r, bst)
    r.yo_dev = vt_dev_buf(r, bp)
    r.host = vt_host_buf(r, bst + bp, true)
    if (r.oom) {
        vt_release_scratch(r)
        return VtBuild.memory
    }
    r.s_q = set_tower_vad_enc_cls(fixed_array(r.x_dev, r.wq_dev, r.q_dev), fixed_array(bx, r.wq_bytes, bgx), fixed_array(VT_X, VT_XB, VT_Q))
    r.s_k = set_tower_vad_lstm_cls(fixed_array(r.q_dev, r.wq_dev, r.k_dev, r.yo_dev), fixed_array(bgx, r.wq_bytes, bst, bp),
        fixed_array(VT_Q, VT_XB, VT_K, VT_YO))
    return VtBuild.built
}

//! the n samples at `src` as ceil(n / 512) chunks behind the stream's 64-sample context, one command: the encoder a workgroup
//! a chunk, the recurrence walking them all; the probabilities and the stream's h and c read back once
[hot_path, arch(at = "../ARCHITECTURE_MEDIA.md#tower-gpu-hook")]
def private vulkan_vad_chunks(m : VadModel; var st : VadState; src : float const?; n : int64; var probs : array<float>) : bool {
    var r & = g_vt_vad
    if (!g_vt_env_tower) {
        return vt_decline(r, VulkanTowerDecline.knob)
    }
    if (!gk_vad_shape_ok(m, st, n)) {
        return vt_decline(r, VulkanTowerDecline.shape)
    }
    if (!vt_arm(r).ok) {
        return false
    }
    if (!ensure_tower_vad_enc_cls() || !ensure_tower_vad_lstm_cls()) {
        return vt_decline(r, VulkanTowerDecline.device)
    }
    let nch = (n + GK_VAD_CHUNK - 1l) / GK_VAD_CHUNK
    if (!vt_built(r, vt_vad_upload(r, m)) || (r.cap_rows < nch && !vt_built(r, vt_vad_scratch(r, max(nch, 64l))))) {
        return false
    }
    let nx = 64l + nch * GK_VAD_CHUNK
    ensure_length(g_vt_vad_x, nx)
    gk_vad_stage(unsafe(addr(g_vt_vad_x[0])), st, src, n, nch)
    let bst = 2l * GK_VAD_HIDDEN * 4l
    let bp = nch * 4l
    upload_region_at(r.x_dev, 0l, unsafe(addr<uint8 const?>(g_vt_vad_x[0])), nx * 4l)
    upload_region_at(r.k_dev, 0l, unsafe(addr<uint8 const?>(st.h[0])), GK_VAD_HIDDEN * 4l)
    upload_region_at(r.k_dev, GK_VAD_HIDDEN * 4l, unsafe(addr<uint8 const?>(st.c[0])), GK_VAD_HIDDEN * 4l)
    var pa = gk_vad_args(m.slab_offs, nch)
    vt_chain_one_shot("silero-vad", 1l, nch) $(raw : VkCommandBuffer; var h : VkHaz) {
        enc_tower_vad_enc_cls(raw, h, r.s_q, pa, nch)
        vt_pt(raw, VtProfRole.spectrum)
        enc_tower_vad_lstm_cls(raw, h, r.s_k, pa, 1l)
        vt_pt(raw, VtProfRole.tts_lstm)
        vhz_dep(raw, h, VT_K | VT_YO, 0u, true)
        cmd_copy_range(raw, r.k_dev, 0l, r.host.buf, 0l, bst)
        cmd_copy_range(raw, r.yo_dev, 0l, r.host.buf, bst, bp)
        vt_pt(raw, VtProfRole.copy_out)
    }
    host_buf_read(r.host, unsafe(addr<void?>(st.h[0])), GK_VAD_HIDDEN * 4l)
    host_buf_read(r.host, unsafe(addr<void?>(st.c[0])), GK_VAD_HIDDEN * 4l, GK_VAD_HIDDEN * 4l)
    let p0 = length(probs)
    probs |> resize(p0 + int(nch))   // nolint:PERF026 — the caller's probability row grows by the call's chunks: the seat's one output
    host_buf_read(r.host, unsafe(addr<void?>(probs[p0])), bp, bst)
    for (i in range64(64l)) {
        st.context[i] = g_vt_vad_x[nch * GK_VAD_CHUNK + i]
    }
    vt_announce(r)
    g_vt_vad_calls++
    g_vt_vad_chunks += nch
    return true
}

[init]
def dasllama_vulkan_tower_register {
    g_vt_env_tower = g_env_vulkan.vk_tower
    vt_each_resident() $(var r : VtResident; family : string) {
        vt_unbind_batch_sets(r)
        r.family = family
    }
    register_vk_drop_hook(@@vt_forget)
    static_if (!typeinfo builtin_module_exists(das_metal)) {
        register_gemma4v_gpu(@@vulkan_gemma4v_chain, @@vulkan_tower_on, false)   //! the q8 lane stays the policy's default: the q8 tiles read faster than the f32 tile on every vision family (`PERF_LEDGER.md`, the vision shared-kernels entry)
        register_gemma4uv_gpu(@@vulkan_gemma4uv_encode)
        register_gemma3v_gpu(@@vulkan_gemma3v_chain, @@vulkan_tower_on, false)
        register_qwen3v_gpu(@@vulkan_qwen3v_chain, @@vulkan_tower_on, false)
        register_qwen25v_gpu(@@vulkan_qwen25v_chain)
        register_tower_blocks_gpu(@@vulkan_audio_tower_blocks, @@vulkan_tower_serves_exact)
        register_tower_blocks_ln_post_gpu(@@vulkan_audio_tower_blocks_ln_post)
        register_tower_blocks_tail_gpu(@@vulkan_audio_tower_blocks_tail)
        register_tower_conv_gpu(@@vulkan_audio_conv_front)
        register_gemma4a_gpu(@@vulkan_gemma4a_blocks, @@vulkan_tower_serves_exact)
        register_canary_gpu(@@vulkan_canary_blocks, @@vulkan_tower_serves_exact)
        register_qwen3a_front_gpu(@@vulkan_q3a_front)
        register_whisper_mel_gpu(@@vulkan_q3a_mel)
        register_gemma4a_chunk_gpu(@@vulkan_gemma4a_chunk)
        register_canary_front_gpu(@@vulkan_canary_front)
        register_parakeet_gpu(@@vulkan_parakeet_blocks, @@vulkan_tower_serves_exact)
        register_parakeet_gpu_encode(@@vulkan_parakeet_encode)
        set_vulkan_gigaam_exact_lane(g_env_vulkan.vk_gigaam_exact)   //! the q8 lane stays the default until the two lanes are timed on a Vulkan box
        register_gigaam_gpu_encode(@@vulkan_gigaam_encode)
        register_gigaam_flavor(TowerDev.vulkan, @@vulkan_gigaam_serves_model, @@vulkan_gigaam_bake)
        register_parakeet_flavor(TowerDev.vulkan, @@vulkan_parakeet_serves_model, @@vulkan_parakeet_bake)
        register_canary_flavor(TowerDev.vulkan, @@vulkan_canary_serves_model, @@vulkan_canary_bake)
        register_audio_tower_flavor(TowerDev.vulkan, @@vulkan_audio_tower_serves_model, @@vulkan_audio_tower_bake)
        register_gemma4a_flavor(TowerDev.vulkan, @@vulkan_gemma4a_serves_model, @@vulkan_gemma4a_bake)
        register_vad_gpu(@@vulkan_vad_chunks)
        register_tower_rows_gen(@@vulkan_tower_fc_gen)
    }
}
