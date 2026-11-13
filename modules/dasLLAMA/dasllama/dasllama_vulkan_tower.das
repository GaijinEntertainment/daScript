options gen2
options indenting = 4
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_vulkan_tower shared public

require dasllama/dasllama_lint public
require vulkan
require math
require daslib/enum_trait   // each: the audio families the installer walks
require dasllama/dasllama_env
require dasllama/dasllama_kqformat
require dasllama/dasllama_math
require dasllama/dasllama_layout
require dasllama/dasllama_plane
require dasllama/dasllama_tower
require dasllama/dasllama_tower_image   // tower_bake_q8, tower_gather_q8_regions: the discrete homes' q8 block pieces
require dasllama/dasllama_vulkan_common
require dasllama/dasllama_vulkan_classes
require dasllama/dasllama_vulkan_rail
require dasllama/dasllama_tower_chains
require dasllama/dasllama_tower_chains_fc
require dasllama/dasllama_tower_chains_g4a
require dasllama/dasllama_tower_chains_whisper
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

let VK_DECLINE_WORDS_TOWER_KNOB = "the tower knob is off (DASLLAMA_VK_TOWER=0 or set_vulkan_tower(false))"
let VK_DECLINE_WORDS_DEVICE = "the tier has no device (DASLLAMA_GPU=1 arms it), or a class failed to build"
let VK_DECLINE_WORDS_MEMORY = "a device allocation failed"

//! a family's Vulkan flavor stood down at load with the failing condition said (`tower_flavor_refused`)
def private vt_flavor_refused(family, why : string) : bool => tower_flavor_refused(TowerDev.vulkan, family, why)

//! Engage / decline counters; `blocks` = transformer blocks the driver ran. Model-level gates assert DELTAS on these.
def vulkan_tower_stats : tuple<encodes : int64; rows : int64; blocks : int64; declines : int64; convs : int64; mels : int64> =>
    (encodes = g_vt_encodes, rows = g_vt_rows, blocks = g_vt_blocks, declines = g_vt_declines.total, convs = g_vt_convs, mels = g_vt_mels)

//! The declines-by-reason table (a copy), keyed by `TowerDecline` value names.
def vulkan_tower_declines : table<string; int64> => <- clone_to_move(g_vt_declines.counts)

//! Test A/B seat: pin the tower driver on / off in-process (the env knob's setter twin).
def set_vulkan_tower(on : bool) {
    g_vt_env_tower = on
}

var private g_vt_tail_encodes = 0l

//! the encodes whose projector tail the device ran (the soft tokens read back, the blocks' rows never)
def vulkan_tower_tail_encodes : int64 => g_vt_tail_encodes

def vulkan_tower_on : bool => g_vt_env_tower

//! The fronts' route: the audio fronts ride the f16 GEMM feed (cm2 or KHR); off it (the sdot4 mode) every front declines
//! `device` while the block chains still serve.
def vulkan_tower_front_route : bool => vt_cm2_feed()

let private VT_STUB_BYTES = 64l   //! a stub seat's bytes: a family's unused binding, which the release frees like the others
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

//! a K-quant member's device planes on the kq lane: its seats' rows gathered in the tier's layout, in layout order
struct private VtKqPlane {
    q_dev : uint64
    s_dev : uint64
    q_bytes : int64
    s_bytes : int64
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
    f16w : bool           //! the halfword-twin family (qwen25v): the f16 feed rows and the window table
    f32w : bool           //! the exact lane (a vision family's f32 planes): the block GEMMs on the f32 tile over the norms plane's rows, no feed
    xc_dev : uint64       //! the exact lane's clamped GEMM input, [cap x d] (gemma4v: the input bounds applied on the way in, xb kept whole)
    xh_dev : uint64       //! the f16 feed, [cap x max(d, ff)] (the halfword twin), or [cap + TILE_READ_SLACK x max(d, ff)] on the cm2 feed
    cm2 : bool            //! the q8 chains' GEMMs ride the cm2 f16 feed (a coopmat device): xh_dev is the feed
    sk_dev : uint64       //! the split-k partial planes, [nsplit x cap x d] floats, made at the first pick that splits and grown to the widest
    sk_bytes : int64
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
    fhost : HostBuf
    fmeta : HostBuf       //! the front's own schedule (gemma4a: the input projection's record and map)
    fmeta_bytes : int64
    rel_cap_rows : int64      //! the rel quartet's rows (gemma4a: past + 1, uploaded with the tower; canary: 2 x cap, built on the device per encode); 0 = no table
    rel_tab_dev : uint64
    rel_tabq_dev : uint64
    rel_tabh_dev : uint64
    rel_tabs_dev : uint64
    rel_proj_dev : uint64
    wlo_dev : uint64      //! the windows' row starts (nwin + 1), the f32 per-window route's table
    win_cap : int64       //! the window table's capacity (windows)
    host : HostBuf        //! the x readback staging
    phost : HostBuf       //! the projector's readback staging, `phost_bytes` long (0: none)
    phost_bytes : int64
    pj_h_dev : uint64     //! qwen3a's device projector: the hidden rows and the projected ones, `pj_cap` rows each (0: none)
    pj_y_dev : uint64
    pj_cap : int64
    oom : bool            //! a soft allocation of the batch under way failed
    @scratch kq_planes : table<int; VtKqPlane>   //! the kq lane's device planes a K-quant member, keyed by its format slot (`vt_upload_fc_kq`)
    @scratch @exact_size kq_seats : array<TowerFcSeat>     //! the kq lane's seat a GEMM record: the member's slot and pair, the record's block base in its planes
    family : string       //! the family name the counter and the announce carry
    served_said : bool    //! the serves-on-the-device line said for the model the key names
    taps_dev : uint64                   //! the depthwise taps as the family's own halfwords (gigaam's f16 taps; 0 bytes = the taps are f32 in `norms_dev`)
    taps_bytes : int64
    cn_plane_rows : int64        //! canary's rel plane capacity (rows; 0 = none): the (q + v) and rel-table head panels and the plane R = (q + v) P^T
    cn_qh_dev : uint64
    cn_ph_dev : uint64
    cn_r_dev : uint64
    x_ready : bool
    x_pend_ptr : float?       //! the stem's rows still on the device (`x_dev`) and the host array they are owed to; 0 bytes = none pending
    x_pend_bytes : int64
    tail_cap : int64          //! the projector tail's row capacity (gemma4a; 0 = none): the out-proj rows, the projected rows, their readback
    tail_dim : int64          //! the projected rows' width
    tail_out_dim : int64      //! the out-proj rows' width (gemma4a: the encoder's own, narrower than `tail_dim` under a widening embedder)
    tail_out_dev : uint64
    tail_proj_dev : uint64
    tail_host : HostBuf
    vis_cap : int64           //! the vision stem's scratch capacity (rows; 0 = none): the image planes and the patch im2col rows
    vis_pd : int64            //! the im2col row stride it was built for
    vis_planes_bytes : int64
    vis_col_bytes : int64
    vis_planes_dev : uint64
    vis_col_dev : uint64
    wb_dev : uint64           //! the family's halfword plane as stored (the gemma-4 towers' bf16 rows): the f32 tile's bf16 arm reads it
    wb_bytes : int64
    vis_idx_dev : uint64      //! a stem's row map (qwen25v's window sort), uint32 a row
    vis_idx_bytes : int64
    vis_aux_dev : uint64      //! a stem's per-encode f32 rows (qwen3v's positions resized off the learned grid)
    vis_aux_bytes : int64
    ct_cap : int64            //! the chat projector tail's row capacity (the whisper-class chat towers; 0 = none) and its plane widths
    ct_hid : int64
    ct_gate : int64
    ct_out : int64
    ct_hid_dev : uint64
    ct_gate_dev : uint64
    ct_out_dev : uint64
    ct_host : HostBuf
    stem_kdp : int64          //! the whisper-class stem's padded conv1 column width (3 x n_mel rounded up to 64)
    fa_khr : bool
    gemm_khr : bool       //! the f16 GEMM arm the f16 sets were built for (qwen25v)
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
var private @scratch @exact_size g_vt_fc_gseats : array<TowerGemmSeat>   //! the kq lane's seat a GEMM record (`FC_GEMM_RECORDS` a block, the rel record empty): the family's `tower_gemm_seat` over its offsets
var private g_vt_q3a_front : VtResident   //! the qwen3a conv front's resident: the conv_out q8 plane, the f16 conv slab, the chunk scratch
var private @scratch @exact_size g_vt_rope2d_tabs : array<float>   //! the packed rope tables staging, [row][cx | sx | cy | sy]
var private @scratch @exact_size g_vt_wlo : array<uint>            //! the window table staging (nwin + 1)
var private @scratch @exact_size g_vt_idx : array<uint>            //! the window-sort row map staging, one encode's rows
var private @scratch @exact_size g_vt_ln_offs : array<LayerOffs>       //! the pre-LN chain's block offsets, one record a block
var private g_vt_no_rows : array<float>   //! the empty rows a defaulted rows argument names

[cold_path]
def private vt_decline_words(why : TowerDecline) : string => tower_decline_words(why, "Vulkan", VK_DECLINE_WORDS_TOWER_KNOB, VK_DECLINE_WORDS_DEVICE, VK_DECLINE_WORDS_MEMORY)

//! the decline counted under the enum's value name, and said once per reason per tower in words (the counter's `note`)
[cold_path]
def private vt_decline(r : VtResident; why : TowerDecline) : bool
    => g_vt_declines |> note("the {r.family} tower", "{why}", "the tower stays on the CPU chain: {vt_decline_words(why)}")

//! the kq-lane decline - "a reason naming the site and the member" (REVIEW_KQ_FORMATS.md)
[cold_path]
def private vt_decline_kq(r : VtResident; members : string) : bool
    => g_vt_declines |> note("the {r.family} tower", "{TowerDecline.kq_lane}", "the Vulkan image's chain cannot run: the {r.family} block GEMMs ({members}) - {vt_decline_words(TowerDecline.kq_lane)}")

//! a scratch or upload builder's outcome: built, or the reason the seat declines
enum private VtBuild {
    built
    shape    //! a buffer past the device's storage-buffer range
    memory   //! a device or pinned allocation failed
}

//! true where `b` built; else its decline counted
def private vt_built(r : VtResident; b : VtBuild) : bool {
    return true if (b == VtBuild.built)
    return vt_decline(r, b == VtBuild.shape ? TowerDecline.shape : TowerDecline.memory)
}

//! a hook's first gate: the tower knob on and the tower's planes the lane the driver serves (`served_lane`); false with the decline counted
def private vt_gate(r : VtResident; served_lane : bool) : bool {
    return vt_decline(r, TowerDecline.knob) if (!g_vt_env_tower)
    return served_lane || vt_decline(r, TowerDecline.quant_mode)
}

//! a vision chain's gate: the knob, then a q8 tower its Vulkan image (`#tower-dev-block`) - the exact lane's planes upload as stored
def private vt_vis_gate(r : VtResident; q8 : bool; dev : TowerDev) : bool {
    return false if (!vt_gate(r, true))
    return !q8 || dev == TowerDev.vulkan || vt_decline(r, TowerDecline.flavor)
}

[cold_path]
def private vt_announce(var r : VtResident) {
    if (r.served_said) {
        return
    }
    r.served_said = true
    to_log(LOG_INFO, "dasLLAMA vulkan tower: {r.family} serves on the device (DASLLAMA_GPU=1 arms the tier, DASLLAMA_VK_TOWER=0 stands it down)\n")
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
    k = tower_fold_words(k, unsafe(reinterpret<uint8 const?>(plane_at(wblob, w_off))), w_n * 2l)
    k = tower_fold_words(k, unsafe(reinterpret<uint8 const?>(plane_at(blob, rows_off))), rows_n * 4l)
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
    k = tower_fold_words(k, unsafe(reinterpret<uint8 const?>(qblob.p)), qblob.n)
    k = tower_fold_words(k, unsafe(reinterpret<uint8 const?>(qscales.p)), qscales.n * 4l)
    k = tower_fold_words(k, unsafe(reinterpret<uint8 const?>(plane_at(blob, rows_off))), rows_n * 4l)
    return k
}

//! A model's Vulkan device block as an upload reads it (`#tower-dev-block`): `on` where the model is the Vulkan-flavored image
struct VtDevSrc {
    on : bool
    dev : PlaneU8 = PlaneU8()
    devplan : PlaneI64 = PlaneI64()
}

def vt_dev_src(m) : VtDevSrc => VtDevSrc(on = m.dev_backend == TowerDev.vulkan, dev = m.dev, devplan = m.devplan)

//! the resident key of a Vulkan-flavored model: its block - address, size, plan, sampled words - the rows of `blob` uploaded
//! beside it and the layer count, every field the resident's extent follows
def vt_key_dev(src : VtDevSrc; blob : PlaneF; rows_off, rows_n, n_layer : int64) : uint64 {
    var k = vt_fold_int(vt_fold_addr(vt_fold_int(0x6b0dul, src.dev.n), unsafe(reinterpret<void?>(src.dev.p))), n_layer)
    k = tower_fold_words(k, unsafe(reinterpret<uint8 const?>(src.devplan.p)), src.devplan.n * 8l)
    k = tower_fold_words(k, src.dev.p, src.dev.n)
    k = vt_fold_addr(k, unsafe(reinterpret<void?>(blob.p)))
    k = vt_fold_int(k, rows_off)
    k = vt_fold_int(k, rows_n)
    return tower_fold_words(k, unsafe(reinterpret<uint8 const?>(plane_at(blob, rows_off))), rows_n * 4l)
}

//! a block piece's bytes - 0 where the block holds none
def vt_piece_bytes(src : VtDevSrc; name : string) : int64 => max(tower_dev_find(src.devplan, name).bytes, 0l)

//! the piece `name` uploaded as stored into `buf`
def vt_put_piece(src : VtDevSrc; name : string; buf : uint64) {
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

//! the largest buffer the scratch at `cap` rows binds: the range decline reads it against the device's limit
def private vt_scratch_max_bytes(cap, d, ff, dp : int64; fused, f16w : bool) : int64 {
    let bx = cap * d * 4l
    var b = max(cap * max(d, ff) * 4l, cap * dp * 4l)
    if (fused) {
        b = max(b, 3l * bx)
    }
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
    if (r.pj_cap > 0l) {
        release_device_buf(r.pj_h_dev)
        release_device_buf(r.pj_y_dev)
        r.pj_cap = 0l
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
    g_vt_aud_out.bytes = 0l
    g_vt_fc_rows.bytes = 0l
    r.cap_rows = 0l
    r.pad_dp = 0l
    r.fused = false
    r.f16w = false
    r.f32w = false
    r.win_cap = 0l
    r.meta_bytes = 0l
    r.front_cap = 0l
    r.x_ready = false
    r.cm2 = false
    r.sk_bytes = 0l
    r.tail_cap = 0l
    r.tail_dim = 0l
    r.tail_out_dim = 0l
    if (r.vis_cap > 0l) {
        vt_vis_forget(r)
    }
    r.ct_cap = 0l
    r.cn_plane_rows = 0l
    r.pj_cap = 0l         // qwen3a's device projector planes and readback: gone with the scratch they sit beside
    r.phost_bytes = 0l
}

[cold_path]
def private vt_release(var r : VtResident) {
    vt_release_scratch(r)
    if (r.key != 0ul) {
        release_device_buf(r.wq_dev)
        release_device_buf(r.ws_dev)
        release_device_buf(r.norms_dev)
        if (r.wb_bytes > 0l) {
            release_device_buf(r.wb_dev)
            r.wb_bytes = 0l
        }
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

var private g_vt_releases : array<function<() : void>>   //! every seat module's resident release, the shutdown's sweep

//! a seat module's residents released at the shutdown's sweep (`vulkan_tower_shutdown`)
def vt_on_release(fn : function<() : void>) {
    g_vt_releases |> push(fn)
}

//! Release every device object the driver and the seat modules own (the tests' leak rail); the next served encode uploads again.
def vulkan_tower_shutdown {
    vt_each_resident() $(var r : VtResident; _family : string) {
        vt_release(r)
    }
    for (fn in g_vt_releases) {
        invoke(fn)
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
    r.wq_dev = 0ul
    r.ws_dev = 0ul
    r.wb_dev = 0ul
    r.wb_bytes = 0l
    r.kq_planes |> clear()
    r.kq_seats |> clear()
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
    return true
}

//! the norms plane's layout: the blob's `rows_n` rows, the `w_n` widened halfwords behind them, then the ones row; the q8 seats are stubs
[cold_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_VISION.md#vk-vision-chains")]
def private vt_upload_f32(var r : VtResident; key : uint64; blob : PlaneF; rows_n : int64; ones_n : int64) : bool {
    vt_upload_begin(r, key)
    r.wq_bytes = 0l
    r.ws_bytes = 0l
    r.wq_dev = vt_dev_buf(r, VT_STUB_BYTES)
    r.ws_dev = vt_dev_buf(r, VT_STUB_BYTES)
    r.ones_off = rows_n
    r.norms_bytes = (rows_n + ones_n) * 4l
    r.norms_dev = vt_dev_buf(r, r.norms_bytes)
    if (r.oom) {
        vt_release(r)
        return false
    }
    upload_float_ranges(r.norms_dev, [(p = plane_at(blob, 0l), n = rows_n)], ones_n)
    r.cap_rows = 0l
    return true
}

//! the halfword plane as stored, behind a fresh plane upload; false with the resident released on a refused allocation
[cold_path]
def private vt_upload_wb(var r : VtResident; wblob : PlaneU16) : bool {
    return true if (wblob.n == 0l)
    r.wb_bytes = wblob.n * 2l
    r.wb_dev = vt_dev_buf(r, r.wb_bytes)
    if (r.oom) {
        r.wb_bytes = 0l
        vt_release(r)
        return false
    }
    upload_region_at(r.wb_dev, 0l, unsafe(reinterpret<uint8 const?>(wblob.p)), r.wb_bytes)
    return true
}

def private vt_scratch_f16_fits(r : VtResident; rows, nwin, d, ff, dp : int64; khr, gemm_khr : bool) : bool
    => (r.cap_rows >= tower_cap_rows(rows) && r.win_cap >= nwin && r.d == d && r.ff == ff && r.pad_dp == dp && r.f16w
        && r.fa_khr == khr && r.gemm_khr == gemm_khr)

//! the padded panels at the compact rows, the f16 feed and the window table; false when an allocation failed
[cold_path]
def private vt_scratch_f16_build(var r : VtResident; rows, nwin, d, ff, hs, dp : int64; khr, gemm_khr : bool) : bool {
    let cap = tower_cap_rows(rows)
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

//! a chain's batch schedule written into the host-mapped `meta` (`tower_sched_walk`)
def vt_sched_walk(@scratch var maps : array<uint>; meta : HostBuf; nrec : int64; rec : block<(rid : int64) : TowerSchedRec>) {
    tower_sched_walk(maps, unsafe(reinterpret<uint?>(meta.mapped)), nrec, rec)
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
    tower_gather_q8_regions(qblob, qscales, regions, lay, wq, ws)
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
    if (r.wq_bytes > 0l) {   // a kq-lane chain with no Q8_0 seats carries no q8 planes
        r.wq_dev = vt_dev_buf(r, r.wq_bytes)
        r.ws_dev = vt_dev_buf(r, r.ws_bytes)
    }
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
    return true
}

def private vt_scratch_fits(r : VtResident; rows, d, ff, dp, meta_bytes : int64; khr, fused : bool; f32w : bool = false) : bool
    => (r.cap_rows >= tower_cap_rows(rows) && r.d == d && r.ff == ff && r.pad_dp == dp && r.meta_bytes >= meta_bytes && r.fused == fused
        && r.fa_khr == khr && r.f32w == f32w)

//! `dp` > 0 adds the padded panels, `meta_bytes` sizes the schedule buffer, `fused` adds the [q | k | v] rows and the rope tables; false when an allocation failed
[cold_path]
def private vt_scratch_build(var r : VtResident; rows, d, ff, hs, dp, meta_bytes : int64; khr, fused : bool; f32w : bool = false) : bool {
    let q8e = kq_elems(KqFmt.q8)
    let cap = tower_cap_rows(rows)
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
    r.host = vt_host_buf(r, bx, true)
    if (r.oom) {
        vt_release_scratch(r)
        return false
    }
    return true
}

//! the gemma4a resident's key: the device block's or the q8 planes', the f16 lane's halfword plane (address, size, sampled words), the rel table
def private vt_g4a_key(t : Gemma4aEncoder; lay : Q8RepackLayout) : uint64 {
    let src = vt_dev_src(t)
    var key = (src.on ? vt_key_dev(src, t.blob, 0l, t.blob.n, t.n_layer)
        : vt_key(t.qblob, t.qscales, t.blob, 0l, t.blob.n, 0l, t.n_layer, lay.mr, lay.kgroup, lay.wbias))
    key = vt_fold_int(vt_fold_addr(key, unsafe(reinterpret<void?>(t.wblob.p))), t.wblob.n)
    key = tower_fold_words(key, unsafe(reinterpret<uint8 const?>(t.wblob.p)), t.wblob.n * 2l)
    return tower_fold_words(key, unsafe(reinterpret<uint8 const?>(plane_at(t.pos_emb, 0l))), (gemma4a_attn_past() + 1l) * t.d_model * 4l)
}

//! the gemma4a resident's planes on the image's lane, uploaded where its key moved; false where the upload will not take
def private vt_g4a_planes(var r : VtResident; t : Gemma4aEncoder) : bool {
    let lay = active_q8_repack_layout(g_active_needs_repack)
    let key = vt_g4a_key(t, lay)
    return r.key == key || vt_upload_g4a(r, key, t, lay)
}

//! the blob with the ones row at the encoder's own width beside the image's lane planes (q8 pieces or f16 halfwords, as stored) and the rel table
[cold_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vt_upload_g4a(var r : VtResident; key : uint64; t : Gemma4aEncoder; lay : Q8RepackLayout) : bool {
    let d = t.d_model
    let mid = gemma4a_mid_dim(t)
    var inscope regions <- gemma4a_all_regions(t)
    let ok = (t.gemm_f16 ? (vt_upload_f32(r, key, t.blob, t.blob.n, mid) && vt_upload_wb(r, t.wblob))
        : vt_upload(r, key, t.qblob, t.qscales, regions, t.blob, 0l, t.blob.n, mid, lay, vt_dev_src(t), VT_G4A_PIECE))
    if (!ok) {
        return false
    }
    let rows = tower_g4a_rel_rows()
    if (!vt_rel_bufs(r, rows, d)) {
        vt_release(r)
        return false
    }
    upload_region_at(r.rel_tab_dev, 0l, unsafe(reinterpret<uint8 const?>(plane_at(t.pos_emb, 0l))), rows * d * 4l)
    return true
}

[cold_path]
def private vt_tail_release(var r : VtResident) {
    if (r.tail_cap > 0l) {
        release_device_buf(r.tail_out_dev)
        release_device_buf(r.tail_proj_dev)
        release_host_buf(r.tail_host)
        r.tail_cap = 0l
    }
}

//! the tail's planes at the scratch's rows: the out-proj rows `out_dim` wide, the projected rows and their readback `tail_dim` wide
[cold_path]
def private vt_tail_bufs(var r : VtResident; out_dim, tail_dim : int64) : bool {
    vt_tail_release(r)
    let cap = r.cap_rows
    let bp = cap * tail_dim * 4l
    r.oom = false
    r.tail_out_dev = vt_dev_buf(r, cap * out_dim * 4l)
    r.tail_proj_dev = vt_dev_buf(r, bp)
    r.tail_host = vt_host_buf(r, bp, true)
    r.tail_cap = cap
    r.tail_dim = tail_dim
    r.tail_out_dim = out_dim
    if (r.oom) {
        vt_tail_release(r)
        return false
    }
    return true
}

var private g_vt_rl : VtRail   //! the encode's rail: every seat refills it, its schedule maps kept across encodes

//! every seat at `r`'s planes with the norms base, ones row and scratch rows, after the encode's last allocation; the `f16w` twin stubs the seats it never binds
def private vt_rail_seats(var rl : VtRail; r : VtResident; norms_base : int64) {
    let cap = r.cap_rows
    let w = max(r.d, r.ff)
    let bx = cap * r.d * 4l
    let bff = cap * r.ff * 4l
    let bp = cap * r.pad_dp * 4l
    let bph = cap * r.pad_dp * 2l
    let bh = cap * r.d * 2l
    let btab = cap * (r.hs / 2l) * 4l
    let bpt = r.tail_cap * r.tail_dim * 4l
    let twin = r.f16w
    rl.norms_base = norms_base
    rl.ones_off = r.ones_off
    rl.cap_rows = cap
    rail_seat(rl, TowerBuf.x, r.x_dev, bx, VT_X)
    rail_seat(rl, TowerBuf.xb, r.xb_dev, bx, VT_XB)
    rail_seat(rl, TowerBuf.xc, r.xc_dev, bx, VT_XQ)
    rail_seat(rl, TowerBuf.xh, r.xh_dev, (twin ? cap : cap + TILE_READ_SLACK) * w * 2l, VT_XQ)
    rail_seat(rl, TowerBuf.xq, r.xq_dev, twin ? VT_STUB_BYTES : cap * w, VT_XQ)
    rail_seat(rl, TowerBuf.xs, r.xs_dev, twin ? VT_STUB_BYTES : cap * w / kq_elems(KqFmt.q8) * 4l, VT_XQ)
    rail_seat(rl, TowerBuf.q, r.q_dev, bx, VT_Q)
    rail_seat(rl, TowerBuf.k, r.k_dev, bx, VT_K)
    rail_seat(rl, TowerBuf.v, r.v_dev, bx, VT_V)
    rail_seat(rl, TowerBuf.q2, r.q2_dev, twin ? VT_STUB_BYTES : bx, VT_Q2)
    rail_seat(rl, TowerBuf.k2, r.k2_dev, twin ? VT_STUB_BYTES : bx, VT_K2)
    rail_seat(rl, TowerBuf.v2, r.v2_dev, twin ? VT_STUB_BYTES : bx, VT_V2)
    rail_seat(rl, TowerBuf.k16, r.k16_dev, bh, VT_K16)
    rail_seat(rl, TowerBuf.v16, r.v16_dev, bh, VT_V16)
    rail_seat(rl, TowerBuf.qp, r.qp_dev, bp, VT_Q2)
    rail_seat(rl, TowerBuf.kp16, r.kp16_dev, bph, VT_K16)
    rail_seat(rl, TowerBuf.vp16, r.vp16_dev, bph, VT_V16)
    rail_seat(rl, TowerBuf.attp, r.attp_dev, bp, VT_K2)
    rail_seat(rl, TowerBuf.att, r.att_dev, bx, VT_ATT)
    rail_seat(rl, TowerBuf.yo, r.yo_dev, bx, VT_YO)
    rail_seat(rl, TowerBuf.hg, r.hg_dev, bff, VT_HG)
    rail_seat(rl, TowerBuf.hu, r.hu_dev, bff, VT_HU)
    rail_seat(rl, TowerBuf.yd, r.yd_dev, bx, VT_YD)
    rail_seat(rl, TowerBuf.tabs, r.tabs_dev, twin ? VT_STUB_BYTES : cap * r.hs * 4l, 0u)
    rail_seat(rl, TowerBuf.qkv, r.qkv_dev, twin ? VT_STUB_BYTES : 3l * bx, VT_QKV)
    rail_seat(rl, TowerBuf.cos, r.cos_dev, btab, 0u)
    rail_seat(rl, TowerBuf.sin, r.sin_dev, btab, 0u)
    rail_seat(rl, TowerBuf.sk, r.sk_dev, r.sk_bytes, VT_SPLITK_PLANES)
    rail_seat(rl, TowerBuf.planes, r.vis_planes_dev, r.vis_planes_bytes, VT_STEM_MEL)
    rail_seat(rl, TowerBuf.col, r.vis_col_dev, r.vis_col_bytes, VT_STEM_COL)
    rail_seat(rl, TowerBuf.win_sort, r.vis_idx_dev, r.vis_idx_bytes, 0u)
    rail_seat(rl, TowerBuf.pos_resized, r.vis_aux_dev, r.vis_aux_bytes, 0u)
    rail_seat(rl, TowerBuf.win_table, r.wlo_dev, (r.win_cap + 1l) * 4l, 0u)
    rail_seat(rl, TowerBuf.tail_out, r.tail_out_dev, r.tail_cap * r.tail_out_dim * 4l, VT_TAIL_OUT)
    rail_seat(rl, TowerBuf.tail_proj, r.tail_proj_dev, bpt, VT_TAIL_PROJ)
    rail_seat(rl, TowerBuf.norms, r.norms_dev, r.norms_bytes, 0u)
    rail_seat(rl, TowerBuf.wb, twin ? r.wq_dev : r.wb_dev, twin ? r.wq_bytes : r.wb_bytes, 0u)
    rail_seat(rl, TowerBuf.wq, r.wq_dev, r.wq_bytes, 0u)
    rail_seat(rl, TowerBuf.ws, r.ws_dev, r.ws_bytes, 0u)
    rail_seat(rl, TowerBuf.meta, r.meta.buf, r.meta_bytes, 0u)
    let brel = r.rel_cap_rows * r.d
    rail_seat(rl, TowerBuf.rel_tab, r.rel_tab_dev, brel * 4l, VT_REL_TAB)
    rail_seat(rl, TowerBuf.rel_tabq, r.rel_tabq_dev, brel, VT_REL_TABQ)
    rail_seat(rl, TowerBuf.rel_tabs, r.rel_tabs_dev, brel / kq_elems(KqFmt.q8) * 4l, VT_REL_TABQ)
    rail_seat(rl, TowerBuf.rel_tabh, r.rel_tabh_dev, (r.rel_cap_rows + TILE_READ_SLACK) * r.d * 2l, VT_REL_TABQ)
    rail_seat(rl, TowerBuf.rel_proj, r.rel_proj_dev, brel * 4l, VT_REL_PROJ)
}

//! the gemma4a blocks' rail at `npos` rows on the image's lane: the block sites and the tail's two at the device's tiles, the rel site off its own feeds; `gemm_khr` the f16 tile
def private vt_g4a_block_sites(var rl : VtRail; t : Gemma4aEncoder; npos : int64; khr, gemm_khr : bool) {
    let cm2 = vt_cm2_feed()
    vt_rail_begin(rl, rail_lane_of(t.gemm_f16), cm2, khr, gemm_khr)
    rl.sites[int(RailSite.d)] = vt_tile(cm2, t.d_model, npos)
    rl.sites[int(RailSite.ff)] = vt_tile(cm2, t.n_ff, npos)
    rl.sites[int(RailSite.d2)] = vt_tile(cm2, 2l * t.d_model, npos)
    rl.sites[int(RailSite.rel)] = vt_tile(cm2, t.d_model, tower_g4a_rel_rows())
    rl.sites[int(RailSite.tail1)] = vt_tile(cm2, gemma4a_mid_dim(t), npos)
    rl.sites[int(RailSite.tail2)] = vt_tile(cm2, t.proj_dim, npos)
    rail_bind_rel(rl)
}

//! the q8 lane's GEMM tiles at the blocks' sites, built
def private vt_g4a_tiles_ok(rl : VtRail) : bool {
    for (site in fixed_array(RailSite.d, RailSite.ff, RailSite.d2, RailSite.rel, RailSite.tail1, RailSite.tail2)) {
        return false if (!vt_tile_ensure(rl.sites[int(site)]))
    }
    return true
}

//! the gemma4a blocks admitted at `npos` rows ahead of the front: shape, classes, tiles, planes, scratch, tail planes; `ok` false with the decline counted
def private vt_g4a_blocks_arm(var r : VtResident; t : Gemma4aEncoder; npos : int64) : tuple<ok : bool; khr : bool; gemm_khr : bool> {
    let off = (ok = false, khr = false, gemm_khr = false)
    let d = t.d_model
    let ff = t.n_ff
    let hs = t.d_head
    if (!tower_g4a_shape_ok(t, npos) || npos > VT_MAX_ENCODE_ROWS || !vk_range_ok(t.qblob.n) || !vk_range_ok(t.blob.n * 4l) || !vk_range_ok(t.wblob.n * 2l)
            || !vk_range_ok(vt_scratch_max_bytes(tower_cap_rows(npos), d, ff, 0l, false, false))) {
        vt_decline(r, TowerDecline.shape)
        return off
    }
    let arm = vt_arm(r, t.gemm_f16)
    return off if (!arm.ok)
    var rl & = g_vt_rl
    vt_g4a_block_sites(rl, t, npos, arm.khr, arm.gemm_khr)
    let mid = gemma4a_mid_dim(t)
    if (!(t.gemm_f16 ? f16_gemm_ensure(arm.gemm_khr) : vt_g4a_tiles_ok(rl)) || !ensure_tower_rms_cls() || !ensure_tower_clamp_cls()
            || !ensure_tower_clamp_rq_cls() || !ensure_tower_post_add_cls() || !ensure_tower_post_add_plain_cls() || !ensure_tower_act_cls()
            || !ensure_tower_glu_sig_cls() || !ensure_tower_dw_conv5_cls() || !(hs == 64l ? ensure_tower_g4a_attn64_cls() : ensure_tower_g4a_attn_cls())
            || (rl.cm2 && !ensure_tower_clamp_cvt_cls()) || !ensure_tower_bias_act_cls()) {
        vt_decline(r, TowerDecline.device)
        return off
    }
    let meta_bytes = (tower_g4a_block_recs(t) * 4l + tower_g4a_map_words(rl, t, npos)) * 4l
    if (!vt_g4a_planes(r, t)
            || (!vt_scratch_fits(r, npos, d, ff, 0l, meta_bytes, arm.khr, false) && !vt_scratch_build(r, npos, d, ff, hs, 0l, meta_bytes, arm.khr, false))
            || ((r.tail_cap < r.cap_rows || r.tail_dim != t.proj_dim || r.tail_out_dim != mid) && !vt_tail_bufs(r, mid, t.proj_dim))) {
        vt_decline(r, TowerDecline.memory)
        return off
    }
    return (ok = true, khr = arm.khr, gemm_khr = arm.gemm_khr)
}

//! the gemma4a blocks and the projector tail over the `npos` rows the front landed in `x`, the soft tokens read back into `s.out`
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#g4a-chains")]
def private vt_g4a_blocks_run(var r : VtResident; t : Gemma4aEncoder; var s : Gemma4aState; npos : int64; khr, gemm_khr : bool) : bool {
    var rl & = g_vt_rl
    vt_g4a_block_sites(rl, t, npos, khr, gemm_khr)
    vt_rail_seats(rl, r, 0l)
    if (t.q8) {
        vt_sched_walk(rl.maps, r.meta, tower_g4a_block_recs(t)) $(rid : int64) : TowerSchedRec {
            return tower_g4a_rec(rl, t, npos, rid)
        }
    }
    let bo = npos * t.proj_dim * 4l
    vt_rail_one_shot(rl, "gemma4a", t.n_layer, npos) $ {
        tower_g4a_blocks(rl, t, npos)
        vt_tail_copy_out(rl.raw, rl.h, r, bo)
    }
    s.out |> grow_resize(npos * t.proj_dim)   // the clip's rows exactly: a shorter clip's soft tokens are fewer than the last one's
    s.out_ready = true
    return vt_served_from(r, r.tail_host, s.out, bo, npos, t.n_layer)
}

def private vt_cn_rows_ok(npos, d, ff : int64) : bool {
    let cap = tower_cap_rows(npos)
    return (vk_range_ok(vt_scratch_max_bytes(cap, d, ff, 0l, false, false)) && vk_range_ok(2l * cap * d * 4l)
        && vk_range_ok(cap * (2l * cap - 1l) * 4l))
}

[cold_path]
def private vt_cn_rel_plane(var r : VtResident; cap : int64) : bool {
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
    return true
}

//! the row-broadcast bias (and `act`) over `nout` elements of rows `dout` wide, the bias row at `boff` of the set's second binding
def vt_bias_act(raw : VkCommandBuffer; var h : VkHaz; var s_bias : VkDescriptorSet; dout, nout, boff : int64; act : uint) {
    var pc_b = GkBiasActArgs(d = uint(dout), nelem = uint(nout), boff = uint(boff), act = act)
    enc_tower_bias_act_cls(raw, h, s_bias, pc_b, rq_wgs(false, nout))
}

var private g_vt_fc_rows : TowerRowsHandoff   //! the FastConformer handoff: the served chain's residual rows still on the device

//! the generation of the FastConformer rows the last chain left on the device, 0 where none: a session records it with its rows
[hot_path]
def vulkan_tower_fc_gen : uint64 => rows_handoff_gen(g_vt_fc_rows)

//! the device plane holding the FastConformer encoder rows of generation `gen` (`bytes` long) where the chain that served them
//! left them and no seat has run since, or 0: the transducer decode projects them device to device
def vulkan_tower_fc_rows(gen : uint64; bytes : int64) : uint64 => rows_handoff_dev(g_vt_fc_rows, gen, bytes)

let private VT_HOME = "vk"   //! this home's device-block pieces' prefix (`tower_fc_piece`, `tower_family_piece`)

//! the block chain's seats beyond the shared rail's (`vt_rail_seats`, the rel quartet among them): the exact lane's halfwords, canary's rel plane, the f16 taps, the kq member pairs in slot order
def private vt_fc_rail_seats(var rl : VtRail; r : VtResident; fc : TowerFc) {
    vt_rail_seats(rl, r, 0l)
    if (fc.f16w) {
        rail_seat(rl, TowerBuf.wb, r.wq_dev, r.wq_bytes, 0u)
    }
    let cap = r.cn_plane_rows
    let ww = max(2l * cap - 1l, 0l)
    rail_seat(rl, TowerBuf.rel_qh, r.cn_qh_dev, (cap + TILE_READ_SLACK) * 128l * 2l, VT_CN_QV_PANEL)
    rail_seat(rl, TowerBuf.rel_ph, r.cn_ph_dev, (ww + TILE_READ_SLACK) * 128l * 2l, VT_CN_REL_PANEL)
    rail_seat(rl, TowerBuf.rel_r, r.cn_r_dev, cap * ww * 4l, VT_CN_REL_PLANE)
    rail_seat(rl, TowerBuf.taps, r.taps_dev, r.taps_bytes, 0u)
    vt_fc_kq_member_seats(rl, r, fc)
}

//! the kq lane's member pairs on the rail in slot order: the blocks' rail and the rel-form front's
def private vt_fc_kq_member_seats(var rl : VtRail; r : VtResident; fc : TowerFc) {
    var m = 0
    for (fmt in range(KQ_FMT_COUNT)) {
        continue if ((fc.kq_fmts & (1u << uint(fmt))) == 0u || m >= TOWER_KQ_MEMBERS)
        let kp = r.kq_planes?[fmt] ?? VtKqPlane()
        rail_seat(rl, TOWER_KQ_Q[m], kp.q_dev, kp.q_bytes, 0u)
        rail_seat(rl, TOWER_KQ_S[m], kp.s_dev, kp.s_bytes, 0u)
        m++
    }
}

//! the block chain's planes and scratch inside the device's buffer range at `npos` rows
def private vt_fc_range_ok(fc : TowerFc; npos : int64) : bool {
    let dp = fc.rotary ? fc.n_head * tower_fc_hsp(tower_fc_hs(fc.d, fc.n_head)) : 0l
    let wq_bytes = fc.dev_backend == TowerDev.vulkan ? tower_dev_find(fc.devplan, tower_fc_piece(VT_HOME, "wq")).bytes : fc.qblob.n
    return (vk_range_ok(fc.f16w ? fc.w_n * 2l : wq_bytes) && vk_range_ok(fc.blob.n * 4l) && vt_fc_rows_ok(fc.rotary, npos, fc.d, fc.ff, dp)
        && (!fc.kq_lane || vt_fc_kq_range_ok(fc)))
}

//! the kq lane's member planes inside the storage range: a Vulkan image's pieces a member, a CPU image's planes whole
def private vt_fc_kq_range_ok(fc : TowerFc) : bool {
    return vk_range_ok(fc.kq.n) && vk_range_ok(fc.ks.n) if (fc.dev_backend != TowerDev.vulkan)
    for (fmt in range(KQ_FMT_COUNT)) {
        return false if (!vk_range_ok(tower_dev_find(fc.devplan, tower_fc_kq_piece(VT_HOME, fmt, false)).bytes)
            || !vk_range_ok(tower_dev_find(fc.devplan, tower_fc_kq_piece(VT_HOME, fmt, true)).bytes))
    }
    return true
}

//! the exact lane's GEMMs on the device: the f16 feed (cm2 or KHR), the f16 GEMM class built, the widths on its 64 lattice and
//! the pointwise conv's 2d rows inside the ff-wide plane it lands in; the kq lane asks the f16 feed alone, the q8 lane nothing
def private vt_fc_lane_ok(fc : TowerFc; gemm_khr : bool) : bool {
    if (fc.f16w) {
        return vt_cm2_feed() && fc.d % 64l == 0l && fc.ff % 64l == 0l && fc.ff >= 2l * fc.d && f16_gemm_ensure(gemm_khr)
    }
    return !fc.kq_lane || vt_cm2_feed()
}

//! asked after `vt_fc_lane_ok`, which holds the f16 feed these tiles read
[arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_FC.md#vk-tower-fc-kq")]
def private vt_fc_kq_tiles_ok(fc : TowerFc; npos, ww : int64) : bool {
    return true if (!fc.kq_lane)
    for (fmt in range(KQ_FMT_COUNT)) {
        continue if ((fc.kq_fmts & (1u << uint(fmt))) == 0u)
        if (!vt_tile_ensure(vt_tile(true, fc.d, npos, fmt)) || !vt_tile_ensure(vt_tile(true, fc.ff, npos, fmt)) || !vt_tile_ensure(vt_tile(true, 2l * fc.d, npos, fmt))
                || (!fc.rotary && !vt_tile_ensure(vt_tile(true, fc.d, ww, fmt)))) {
            return false
        }
    }
    return true
}

//! the rel-form front's three GEMM tiles at the columns `tcs`, a class each of the q8 slot's and of every K-quant member's
def private vt_fc_front_tiles_ok(fc : TowerFc; tcs : int64 const[3]) : bool {
    for (fmt in range(KQ_FMT_COUNT)) {
        continue if (fmt != int(KqFmt.q8) && (fc.kq_fmts & (1u << uint(fmt))) == 0u)
        for (tc in tcs) {
            return false if (!cm2_cls_ensure(fmt, tc))
        }
    }
    return true
}

//! the block chain would admit `npos` rows behind a front about to run (its own ropes built for them): nothing counted - a
//! front that serves only to have the blocks decline does the CPU's front over again, and the block seat behind the CPU front
//! counts that decline itself
def private vt_fc_admits(fc : TowerFc; npos, ww : int64; khr, gemm_khr : bool) : bool {
    let rope_n = npos * (tower_fc_hs(fc.d, fc.n_head) / 2l)
    return (tower_fc_shape_ok(fc.rotary, fc.d, fc.ff, fc.n_head, fc.conv_kernel, npos, ww, rope_n) && vt_fc_range_ok(fc, npos)
        && vt_fc_classes_ok(fc.rotary, fc.d, fc.ff, fc.n_head, npos, ww, khr, gemm_khr, vt_cm2_feed()) && vt_fc_lane_ok(fc, gemm_khr)
        && vt_fc_kq_tiles_ok(fc, npos, ww))
}

[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#fc-chains")]
def private vt_fc_blocks(var r : VtResident; fc : TowerFc; var x : array<float>; npos, ww : int64;
                         rope_cos : array<float> = g_vt_no_rows; rope_sin : array<float> = g_vt_no_rows) : bool {
    g_vt_fc_rows.bytes = 0l
    let d = fc.d
    let ff = fc.ff
    let hs = tower_fc_hs(d, fc.n_head)
    let dp = fc.rotary ? fc.n_head * tower_fc_hsp(hs) : 0l
    if (!tower_fc_shape_ok(fc.rotary, d, ff, fc.n_head, fc.conv_kernel, npos, ww, min(long_length(rope_cos), long_length(rope_sin)))) {
        return vt_decline(r, TowerDecline.shape)
    }
    let arm = vt_arm(r, true)
    return false if (!arm.ok)
    if (!vt_fc_range_ok(fc, npos)) {
        return vt_decline(r, TowerDecline.shape)
    }
    let cm2 = vt_cm2_feed()
    if (!vt_fc_classes_ok(fc.rotary, d, ff, fc.n_head, npos, ww, arm.khr, arm.gemm_khr, cm2) || !vt_fc_lane_ok(fc, arm.gemm_khr)) {
        return vt_decline(r, TowerDecline.device)
    }
    if (!vt_fc_kq_tiles_ok(fc, npos, ww)) {
        return vt_decline_kq(r, tower_kq_members_of(fc.kq_fmts, fc.qblob.n > 0l))
    }
    let v = vt_tile(cm2, d, npos)
    let vff = vt_tile(cm2, ff, npos)
    let v2d = vt_tile(cm2, 2l * d, npos)
    let v_rel = vt_tile(cm2, d, fc.rotary ? npos : ww)
    if (!vt_fc_planes(r, fc)) {
        return vt_decline(r, TowerDecline.memory)
    }
    let g_d = vt_tile_groups(v, d, npos)
    let g_ff = vt_tile_groups(vff, ff, npos)
    let g_2d = vt_tile_groups(v2d, 2l * d, npos)
    let g_rel = fc.rotary ? 0l : vt_tile_groups(v_rel, d, ww)
    let nrec = fc.n_layer * FC_GEMM_RECORDS
    let meta_bytes = (nrec * 4l + fc.n_layer * (7l * g_d + 2l * g_ff + g_2d + g_rel)) * 4l
    let rel_cap = 2l * tower_cap_rows(npos)
    if (!vt_scratch_fits(r, npos, d, ff, dp, meta_bytes, arm.khr, false) || (!fc.rotary && r.rel_cap_rows < ww) || r.hs != hs) {
        if (!vt_scratch_build(r, npos, d, ff, hs, dp, meta_bytes, arm.khr, false) || (!fc.rotary && !vt_rel_bufs(r, rel_cap, d))) {
            vt_release_scratch(r)
            return vt_decline(r, TowerDecline.memory)
        }
    }
    if (!fc.rotary && r.cn_plane_rows < tower_cap_rows(npos) && !vt_cn_rel_plane(r, tower_cap_rows(npos))) {
        return vt_decline(r, TowerDecline.memory)
    }
    var rl & = g_vt_rl
    vt_rail_begin(rl, rail_lane_of(fc.f16w), cm2, arm.khr, arm.gemm_khr)
    rl.sites[int(RailSite.d)] = v
    rl.sites[int(RailSite.ff)] = vff
    rl.sites[int(RailSite.d2)] = v2d
    rl.sites[int(RailSite.rel)] = v_rel
    rail_bind_rel(rl)
    vt_fc_rail_seats(rl, r, fc)
    if (!fc.f16w) {
        vt_sched_walk(rl.maps, r.meta, nrec) $(rid : int64) : TowerSchedRec {
            return tower_fc_rec(rl, fc, g_vt_fc_offs, r.kq_seats, npos, ww, rid)
        }
    }
    let bx = npos * d * 4l
    upload_region_at(r.x_dev, 0l, unsafe(addr<uint8 const?>(x[0])), bx)
    let half = hs / 2l
    if (fc.rotary) {   //! the session's tables: cos [npos][hs / 2], then sin, in `tabs_dev`
        upload_region_at(r.tabs_dev, 0l, unsafe(addr<uint8 const?>(rope_cos[0])), npos * half * 4l)
        upload_region_at(r.tabs_dev, npos * half * 4l, unsafe(addr<uint8 const?>(rope_sin[0])), npos * half * 4l)
    }
    vt_rail_one_shot(rl, fc.family, fc.n_layer, npos) $ {
        tower_fc_blocks(rl, fc, g_vt_fc_offs, r.kq_seats, npos, ww)
        vhz_dep(rl.raw, rl.h, VT_X, 0u, true)
        cmd_copy_range(rl.raw, r.x_dev, 0l, r.host.buf, 0l, bx)
        vt_pt(rl.raw, RailProfRole.copy_out)
    }
    rows_handoff_land(g_vt_fc_rows, r.x_dev, bx)
    return vt_served(r, x, bx, npos, fc.n_layer)
}

//! the chain's range check: the scratch's widest buffer, the rel quartet's and the rel plane's where the family carries the rel
//! table, the padded panels' where it ropes
def private vt_fc_rows_ok(rotary : bool; npos, d, ff, dp : int64) : bool
    => rotary ? vk_range_ok(vt_scratch_max_bytes(tower_cap_rows(npos), d, ff, dp, false, false)) : vt_cn_rows_ok(npos, d, ff)

[hot_path]
def private vulkan_canary_blocks(t : CanaryEncoder; var s : CanaryState; npos, ww : int64) : bool {
    var r & = g_vt_cn
    if (!vt_gate(r, true)) {
        return false
    }
    return vt_decline(r, TowerDecline.flavor) if (t.dev_backend != TowerDev.vulkan)
    let fc = tower_fc_of_cn(t, g_vt_fc_offs)
    return vt_fc_blocks(r, fc, s.x, npos, ww) && vt_fc_proj(r, fc, npos, s.out)
}

//! the parakeet encode whole: the subsample front off the CPU mel, then the blocks over its rows, as the CPU chain orders them;
//! -1 with nothing counted where the front cannot serve or where the blocks declined behind a served front
[hot_path]
def private vulkan_parakeet_encode(m : ParakeetModel; var st : ParakeetState) : int64 {
    var r & = g_vt_pk
    g_vt_fc_rows.bytes = 0l
    if (!g_vt_env_tower) {
        vt_decline(r, TowerDecline.knob)
        return -1l
    }
    if (m.dev_backend != TowerDev.vulkan) {
        vt_decline(r, TowerDecline.flavor)
        return -1l
    }
    if (tower_fc_geom(st.mel.n_len, m.n_mel).t3 > parakeet_max_frames()) {
        vt_decline(r, TowerDecline.shape)
        return -1l
    }
    let fc = tower_fc_of_pk(m, VT_HOME, g_vt_fc_offs, g_vt_fc_gseats)
    let tt = vt_fc_front(r, fc, unsafe(addr<float const?>(st.mel.data[0])), long_length(st.mel.data), st.mel.n_len, st.mel.n_len, st.x, true)
    let first = !r.served_said   //! read past the front, whose attach re-keys the resident
    if (tt < 0l || !vt_fc_blocks(r, fc, st.x, tt, 2l * tt - 1l)) {
        return -1l
    }
    tower_fc_cpu_mel_say("vulkan", r.family) if (first && !fc.device_mel)
    g_vt_convs++
    return tt
}

//! the image planes up for the stem (the canvas's bytes at the plane's start)
def private vt_planes_up(r : VtResident; planes : array<float>; w, h : int64) {
    upload_region_at(r.vis_planes_dev, 0l, unsafe(addr<uint8 const?>(planes[0])), w * h * 3l * 4l)
}

//! the tail's projected rows to the host buffer behind the projection's hazard
def private vt_tail_copy_out(raw : VkCommandBuffer; var h : VkHaz; r : VtResident; bytes : int64) {
    vhz_dep(raw, h, VT_TAIL_PROJ, 0u, true)
    cmd_copy_range(raw, r.tail_proj_dev, 0l, r.tail_host.buf, 0l, bytes)
    vt_pt(raw, RailProfRole.copy_out)
}

//! the tier's want (DASLLAMA_GPU) and then the device init - the gate every chain and every twin cell reads first: nothing
//! reads `g_gpu` ahead of this answer
def public vulkan_tower_arms : bool => gpu_want_arms_tier() && vk_moe_init()

//! the arm every chain reads after its shape check: the want and the device first, then the tile family the flash arm and
//! the GEMM feed off cm2 (when the chain's GEMMs ride it, `gemm_off_cm2`) read (`vulkan_tower_tiles_on`); `ok` false counted the decline
def private vt_arm(r : VtResident; gemm_off_cm2 : bool = false) : tuple<ok : bool; khr : bool; gemm_khr : bool> {
    if (!vulkan_tower_arms()) {
        vt_decline(r, TowerDecline.device)
        return (ok = false, khr = false, gemm_khr = false)
    }
    let khr = !cm2_fa_on()
    let gemm_khr = gemm_off_cm2 && f16_gemm_khr()
    if (!vulkan_tower_tiles_on()) {
        vt_decline(r, TowerDecline.device)
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
        vt_decline(r, TowerDecline.device)
        return off
    }
    if (!invoke(planes, r)) {
        vt_decline(r, TowerDecline.memory)
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

//! a FastConformer resident's GEMM planes off its Vulkan image (`#tower-dev-block`), as stored where its key moved; reached only past a CPU image's decline
def private vt_fc_planes(var r : VtResident; fc : TowerFc) : bool {
    verify(fc.dev_backend == TowerDev.vulkan, "vulkan tower: a FastConformer seat reached the planes with a CPU image")
    if (fc.f16w) {
        let key16 = vt_fold_taps(vt_key_f16(fc.wblob, fc.w_off, fc.w_n, fc.blob, 0l, fc.blob.n, fc.n_layer), fc)
        return r.key == key16 || (vt_upload_f16(r, key16, fc.wblob, fc.w_off, fc.w_n, fc.blob, 0l, fc.blob.n) && vt_upload_taps(r, fc))
    }
    let kd = vt_fold_taps(vt_key_dev(VtDevSrc(on = true, dev = fc.dev, devplan = fc.devplan), fc.blob, 0l, fc.blob.n, fc.n_layer), fc)
    return r.key == kd || (vt_upload_fc_dev(r, kd, fc) && vt_upload_taps(r, fc))
}

//! The FastConformer chain's planes off a Vulkan image's pieces (`#tower-dev-block`): each piece uploaded as it is - the q8
//! planes, the kq lane's member planes and seats - beside the blob; false where a buffer cannot be made
[cold_path, arch(at = "../ARCHITECTURE_IMAGE_GPU_PLANES.md#tower-dev-block")]
def private vt_upload_fc_dev(var r : VtResident; key : uint64; fc : TowerFc) : bool {
    let src = VtDevSrc(on = true, dev = fc.dev, devplan = fc.devplan)
    var inscope no_regions : array<Q8Region>
    return false if (!vt_upload(r, key, PlaneI8(), PlaneF(), no_regions, fc.blob, 0l, fc.blob.n, 0l, active_q8_repack_layout(false), src, "{VT_HOME}.fc"))
    tower_fc_dev_seats(fc, VT_HOME, r.kq_seats)
    return true if (!fc.kq_lane)
    for (fmt in range(KQ_FMT_COUNT)) {
        continue if ((fc.kq_fmts & (1u << uint(fmt))) == 0u)
        let qb = vt_piece_bytes(src, tower_fc_kq_piece(VT_HOME, fmt, false))
        let sb = vt_piece_bytes(src, tower_fc_kq_piece(VT_HOME, fmt, true))
        r.kq_planes[fmt] = VtKqPlane(q_dev = vt_dev_buf(r, qb), s_dev = vt_dev_buf(r, sb), q_bytes = qb, s_bytes = sb)
    }
    if (r.oom) {
        vt_release(r)
        return false
    }
    for (fmt, kp in keys(r.kq_planes), values(r.kq_planes)) {
        vt_put_piece(src, tower_fc_kq_piece(VT_HOME, fmt, false), kp.q_dev)
        vt_put_piece(src, tower_fc_kq_piece(VT_HOME, fmt, true), kp.s_dev)
    }
    return true
}

//! GigaAM's Vulkan chain serves a carrier on its lane (`e`, the lane's meta): the tower knob on, the device armed with its tiles,
//! the f16 GEMM feed its conv front rides, and block GEMM planes the chain reads - off the feed the load takes the CPU image
def private vulkan_gigaam_serves_model(e : GigaamModel) : bool {
    return false if (!g_vt_env_tower || !vulkan_tower_arms() || !vulkan_tower_tiles_on() || !gigaam_gpu_lane_planes(e))
    if (!vt_cm2_feed()) {
        to_log(LOG_INFO, "dasLLAMA vulkan tower: GigaAM's block GEMMs ride the f16 GEMM feed, off on this device - the load maps the CPU image\n")
        return false
    }
    return true
}

//! GigaAM's Vulkan device block (`#tower-dev-block`): the chain's planes, then the front's f16 conv slab with its mel tables
[cold_path, arch(at = "../ARCHITECTURE_IMAGE_GPU_PLANES.md#tower-dev-block")]
def private vulkan_gigaam_bake(m : GigaamModel; var b : TowerDevBuild) : bool {
    tower_fc_bake(tower_fc_of_ga(m, VT_HOME, g_vt_fc_offs, g_vt_fc_gseats), g_vt_fc_offs, g_vt_fc_gseats, b, VT_HOME)
    return tower_ga_front_bake(m, b, VT_HOME)
}

//! Parakeet's Vulkan device block (`#tower-dev-block`): the chain's planes, then the front's f16 conv panel
[cold_path, arch(at = "../ARCHITECTURE_IMAGE_GPU_PLANES.md#tower-dev-block")]
def private vulkan_parakeet_bake(m : ParakeetModel; var b : TowerDevBuild) : bool {
    let fc = tower_fc_of_pk(m, VT_HOME, g_vt_fc_offs, g_vt_fc_gseats)
    tower_fc_bake(fc, g_vt_fc_offs, g_vt_fc_gseats, b, VT_HOME)
    tower_cn_front_bake(fc, b, VT_HOME)
    return true
}

//! Canary's Vulkan device block (`#tower-dev-block`): the chain's planes with the projector's, then the front's f16 conv panel
[cold_path, arch(at = "../ARCHITECTURE_IMAGE_GPU_PLANES.md#tower-dev-block")]
def private vulkan_canary_bake(m : CanaryEncoder; var b : TowerDevBuild) : bool {
    let fc = tower_fc_of_cn(m, g_vt_fc_offs)
    tower_fc_bake(fc, g_vt_fc_offs, g_vt_fc_gseats, b, VT_HOME)
    tower_cn_front_bake(fc, b, VT_HOME)
    return true
}

//! a FastConformer serves on the Vulkan chain on either lane (q8 tiles, the exact pick's f16 image): the knob, the device and tiles, the f16 feed its front rides
[unused_argument(q8)]
def private vulkan_fc_serves_model(q8 : bool) : bool {
    return vt_flavor_refused("canary / parakeet", VK_DECLINE_WORDS_TOWER_KNOB) if (!g_vt_env_tower)
    return vt_flavor_refused("canary / parakeet", VK_DECLINE_WORDS_DEVICE) if (!vulkan_tower_arms() || !vulkan_tower_tiles_on())
    return vt_cm2_feed() || vt_flavor_refused("canary / parakeet", "the front rides the f16 GEMM feed, off on this device")
}

//! parakeet's lanes (`e`, the lane's meta) on the Vulkan chain: q8, the exact pick, and the kq lane, whose tiles read the same f16 feed
def private vulkan_parakeet_serves_model(e : ParakeetModel) : bool => vulkan_fc_serves_model(e.q8)

//! canary's projector on the device behind its blocks (`#tower-dev-block`): the GEMM on the chain's lane, its bias, rows into `out`; false with the decline counted
def private vt_fc_proj(var r : VtResident; fc : TowerFc; npos : int64; @scratch @exact_size var out : array<float>) : bool {
    let pdim = fc.proj_dim
    let cm2 = vt_cm2_feed()
    let vp = vt_tile(cm2, pdim, npos)
    return vt_decline(r, TowerDecline.shape) if (pdim > fc.ff)
    return vt_decline(r, TowerDecline.device) if (!fc.f16w && !vt_tile_ensure(vp))
    let bytes = npos * pdim * 4l
    if (r.phost_bytes < bytes) {
        vt_proj_host(r, tower_cap_rows(npos) * pdim * 4l)
        return vt_decline(r, TowerDecline.memory) if (r.phost_bytes < bytes)
    }
    var rl & = g_vt_rl
    vt_rail_begin(rl, rail_lane_of(fc.f16w), cm2, false, fc.f16w && f16_gemm_khr())
    rl.sites[int(RailSite.tail1)] = vp
    vt_fc_rail_seats(rl, r, fc)
    if (!fc.f16w) {
        let wblk0 = fc.proj_w / kq_elems(KqFmt.q8)
        vt_sched_walk(rl.maps, r.meta, 1l) $ [unused_argument(rid)] (rid : int64) : TowerSchedRec {
            return TowerSchedRec(wblk0 = wblk0, rows = rail_site_rows(rl, RailSite.tail1, npos), groups = rail_site_groups(rl, RailSite.tail1, pdim, npos))
        }
    }
    vt_rail_one_shot(rl, "fastconformer projector", 1l, npos) $ {
        tower_fc_proj(rl, fc, npos)
        vhz_dep(rl.raw, rl.h, VT_HG, 0u, true)
        cmd_copy_range(rl.raw, r.hg_dev, 0l, r.phost.buf, 0l, bytes)
    }
    out |> grow_resize(npos * pdim)   // the encode's rows exactly: a shorter clip's projected rows are fewer than the last one's
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


//! the resident key with the family's f16 taps folded in: their plane, region and words
def private vt_fold_taps(k : uint64; fc : TowerFc) : uint64 {
    return k if (fc.taps_n == 0l)
    var kt = vt_fold_addr(k, unsafe(reinterpret<void?>(fc.taps.p)))
    kt = vt_fold_int(kt, fc.taps_off)
    kt = vt_fold_int(kt, fc.taps_n)
    return tower_fold_words(kt, unsafe(reinterpret<uint8 const?>(plane_at(fc.taps, fc.taps_off))), fc.taps_n * 2l)
}

//! the family's f16 depthwise taps uploaded as its own halfwords behind a fresh plane upload; nothing where the taps are f32
[cold_path]
def private vt_upload_taps(var r : VtResident; fc : TowerFc) : bool {
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
    let sk = vt_ln_fc2_pick(d, ff, npos, tower_cap_rows(npos), tc_d, cm2)
    return sk.nsplit > 1l ? cm2_tiles_at(d, npos, sk.tc) * sk.nsplit : g_d
}

[cold_path]
def private vt_sk_release(var r : VtResident) {
    if (r.sk_bytes > 0l) {
        release_device_buf(r.sk_dev)
        r.sk_bytes = 0l
    }
}

//! false = no split serves (a class, the range or the allocation refused): the chain runs the fc2 GEMM whole, never declines
def private vt_sk_ensure(var r : VtResident; tc, nsplit : int64) : bool {
    if (!cm2_cls_ensure(int(KqFmt.q8), tc) || !ensure_splitk_reduce_cls()) {
        return false
    }
    let need = nsplit * r.cap_rows * r.d * 4l
    return vk_range_ok(need) && (r.sk_bytes >= need || vt_sk_build(r, nsplit, need))
}

[cold_path]
def private vt_sk_build(var r : VtResident; nsplit, need : int64) : bool {
    vt_sk_release(r)
    r.sk_dev = try_device_buf(need)
    if (r.sk_dev == 0ul) {
        return false
    }
    r.sk_bytes = need
    to_log(LOG_INFO, "dasLLAMA vulkan tower: the fc2 GEMM splits K into {nsplit} chunks (DASLLAMA_CM2_SPLITK={cm2_splitk_env()}: 0 = the wave model's pick) - planes at {nsplit} x {r.cap_rows} rows (d {r.d})\n")
    return true
}

//! the pre-LN chain's scratch at `npos` rows, its schedule sized to the blocks' records plus `extra_meta` - none on a vision exact lane (`f32w`), which reads none
def private vt_ln_scratch(var r : VtResident; nl, npos, d, ff, hs, dp, g_d, g_ff : int64; khr, cm2 : bool; tc_d, extra_meta : int64; f32w : bool = false) : bool {
    let meta_bytes = f32w ? 0l : vt_ln_meta_bytes(nl, g_d, g_ff, vt_ln_fc2_groups(d, ff, npos, tc_d, cm2, g_d)) + extra_meta
    return (vt_scratch_fits(r, npos, d, ff, dp, meta_bytes, khr, false, f32w)
        || vt_scratch_build(r, npos, d, ff, hs, dp, meta_bytes, khr, false, f32w))
}

//! the pre-LN chain's rail at `npos` rows on `lane`: the f16 feed's columns under `cm2`, the scratch with `extra_meta`, the fc2 k split; false with the decline counted
def private vt_ln_ready(var rl : VtRail; var r : VtResident; nl, npos, d, ff, hs, dp : int64; cm2 : bool; lane : RailLane; khr : bool; v, vff : int; extra_meta : int64) : bool {
    let f32w = lane == RailLane.f32
    let tile_d = VtTile(v = v, tc = cm2 ? cm2_tile_cols(d, npos) : 0l)
    let tile_ff = VtTile(v = vff, tc = cm2 ? cm2_tile_cols(ff, npos) : 0l)
    if (cm2 && (!cm2_cls_ensure(int(KqFmt.q8), tile_d.tc) || !cm2_cls_ensure(int(KqFmt.q8), tile_ff.tc) || !vt_ln16_ensure())) {
        return vt_flush_and_decline(r, TowerDecline.device)
    }
    if (!vt_ln_scratch(r, nl, npos, d, ff, hs, dp, vt_tile_groups(tile_d, d, npos), vt_tile_groups(tile_ff, ff, npos), khr, cm2, tile_d.tc, extra_meta, f32w)) {
        return vt_flush_and_decline(r, TowerDecline.memory)
    }
    var sk = vt_ln_fc2_pick(d, ff, npos, tower_cap_rows(npos), tile_d.tc, cm2)
    if (sk.nsplit > 1l && !vt_sk_ensure(r, sk.tc, sk.nsplit)) {
        sk = (tc = tile_d.tc, nsplit = 1l, ksplit = 0l)
    }
    vt_rail_begin(rl, lane, cm2, khr, false)
    rl.sites[int(RailSite.d)] = tile_d
    rl.sites[int(RailSite.ff)] = tile_ff
    rl.sites[int(RailSite.fc2)] = VtTile(v = v, tc = cm2 ? sk.tc : 0l, nsplit = sk.nsplit, ksplit = sk.ksplit)
    return true
}

//! the vision planes at the canvas and the scratch's rows, and the tail's buffers: a rebuild where a shape or a slab size moved, else nothing
def private vt_vis_bufs(var r : VtResident; w, h, pd, idx_bytes, aux_bytes, tail_dim : int64) : bool {
    let cap = r.cap_rows
    let bpl = w * h * 3l * 4l
    if (r.vis_cap != cap || r.vis_pd != pd || r.vis_planes_bytes < bpl || r.vis_idx_bytes != idx_bytes || r.vis_aux_bytes != aux_bytes) {
        return false if (!vt_vis_grow(r, bpl, pd, idx_bytes, aux_bytes))
    }
    return (r.tail_cap == cap && r.tail_dim == tail_dim && r.tail_out_dim == tail_dim) || (vt_vis_ensures() && vt_tail_bufs(r, tail_dim, tail_dim))
}

//! the stamps the vision and tail sets bind
[cold_path]
def private vt_vis_ensures() : bool
    => ensure_tower_clamp_rq_cls() && ensure_tower_clamp_cvt_cls() && ensure_tower_bias_act_cls() && ensure_tower_rms_cls() && ensure_tower_patch_im2col_cls()

[cold_path]
def private vt_vis_grow(var r : VtResident; bpl, pd, idx_bytes, aux_bytes : int64) : bool {
    return false if (!vt_vis_ensures())
    let cap = r.cap_rows
    let bcol = cap * pd * 4l
    vt_vis_release(r)
    r.oom = false
    r.vis_cap = cap   //! before the allocations: the oom path's release frees what did allocate
    r.vis_planes_dev = vt_call_buf(r, bpl)
    r.vis_col_dev = vt_dev_buf(r, bcol)
    r.vis_idx_dev = idx_bytes > 0l ? vt_call_buf(r, idx_bytes) : 0ul
    r.vis_aux_dev = aux_bytes > 0l ? vt_call_buf(r, aux_bytes) : 0ul
    if (r.oom) {
        vt_vis_release(r)
        return false
    }
    r.vis_pd = pd
    r.vis_planes_bytes = bpl
    r.vis_col_bytes = bcol
    r.vis_idx_bytes = idx_bytes
    r.vis_aux_bytes = aux_bytes
    return true
}

[cold_path]
def private vt_vis_release(var r : VtResident) {
    if (r.vis_cap > 0l) {
        release_device_buf(r.vis_planes_dev)
        release_device_buf(r.vis_col_dev)
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
    r.vis_idx_bytes = 0l
    r.vis_aux_bytes = 0l
}

var private g_vt_aud_out : TowerEncOut   //! the encoder-output handoff: the served post-norm chain's `xb` rows still on the device

//! the device plane holding the encoder rows a caller is about to upload from `rows` (`bytes` long), or 0 when the tower did not
//! land those rows: the whisper cross-KV chain copies them device to device and skips the host upload
def vulkan_tower_enc_out(rows : float const?; bytes : int64) : uint64 => enc_out_dev(g_vt_aud_out, rows, bytes, g_vt_aud.key != 0ul)

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
def private vt_flush_and_decline(var r : VtResident; why : TowerDecline) : bool {
    vt_x_flush(r)
    return vt_decline(r, why)
}

//! the chat tail's tiles at this encode's rows: the first GEMM's and the second's (the first's again on a one-GEMM tail)
def private vt_ct_tiles(ct : TowerChatTail; cm2 : bool) : tuple<t1 : VtTile; t2 : VtTile> {
    let t1 = vt_tile(cm2, ct.n1out, ct.rows)
    return (t1 = t1, t2 = ct.m2in == 0l ? t1 : vt_tile(cm2, ct.out, ct.rows))
}

//! this encode's tail (`tower_chat_tail_of`), `on` where every class it dispatches on the lane built; else the blocks alone serve and the CPU tail reads their rows
def private vt_ct_prepare(t : AudioTower; npos : int64; cm2 : bool) : TowerChatTail {
    var ct = tower_chat_tail_of(t, npos)
    let tl = vt_ct_tiles(ct, cm2)
    let feed_ok = cm2 ? (ensure_tower_ln16_cls() && ensure_tower_clamp_cvt_cls()) : ensure_tower_clamp_rq_cls()
    let gemm_ok = !t.q8 || (vt_tile_ensure(tl.t1) && vt_tile_ensure(tl.t2))
    ct.on = (feed_ok && gemm_ok && ensure_tower_pool_rows_cls() && ensure_tower_ln_cls()
        && ensure_tower_bias_act_cls() && ensure_tower_rms_cls() && ensure_tower_swiglu_swapped_cls() && ensure_tower_act_cls())
    if (!ct.on) {
        tower_ct_stand_down("vulkan", t.proj_kind)
    }
    return ct
}

//! the tail's share of the schedule buffer: its records' four words and a map word a group
def private vt_ct_meta_bytes(ct : TowerChatTail; cm2 : bool) : int64 {
    return 0l if (!ct.on)
    let tl = vt_ct_tiles(ct, cm2)
    let g2 = ct.m2in > 0l ? vt_tile_groups(tl.t2, ct.out, ct.rows) : 0l
    return (tower_ct_gemms(ct) * 4l + vt_tile_groups(tl.t1, ct.n1out, ct.rows) + g2) * 4l
}

//! the tail's two GEMM sites on the rail at their tiles
def private vt_ct_sites(var rl : VtRail; ct : TowerChatTail) {
    let tl = vt_ct_tiles(ct, rl.cm2)
    rl.sites[int(RailSite.tail1)] = tl.t1
    rl.sites[int(RailSite.tail2)] = tl.t2
}

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

//! the tail's planes at the scratch's capacity - the hidden rows, the gated rows, the soft tokens and their readback
[cold_path]
def private vt_ct_bufs(var r : VtResident; hid, gate, out : int64) : bool {
    vt_ct_release(r)
    let cap = r.cap_rows
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
    return true
}

//! the tail's planes for this chain's scratch, grown where its widths or the scratch moved; false = an allocation failed
def private vt_ct_ready(var r : VtResident; ct : TowerChatTail) : bool {
    let hid = tower_ct_hid(ct)
    let gate = tower_ct_gate(ct)
    return (r.ct_cap == r.cap_rows && r.ct_hid == hid && r.ct_gate == gate && r.ct_out == ct.out) || vt_ct_bufs(r, hid, gate, ct.out)
}

//! the tail's planes seated on the rail: its hidden and gated rows, the soft tokens at `tail_proj`
def private vt_ct_seats(var rl : VtRail; r : VtResident) {
    rail_seat(rl, TowerBuf.tail_hid, r.ct_hid_dev, r.ct_cap * max(r.ct_hid, 1l) * 4l, VT_CT_HID)
    rail_seat(rl, TowerBuf.tail_gate, r.ct_gate_dev, r.ct_cap * max(r.ct_gate, 1l) * 4l, VT_CT_GATE)
    rail_seat(rl, TowerBuf.tail_proj, r.ct_out_dev, r.ct_cap * r.ct_out * 4l, VT_TAIL_OUT)
}

[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#whisper-chains")]
def private vt_aud_blocks(t : AudioTower; var s : EncoderState; npos : int64; ln_post : bool; ct : TowerChatTail = TowerChatTail()) : bool {
    var r & = g_vt_aud
    g_vt_aud_out.bytes = 0l
    if (!g_vt_env_tower) {
        return vt_flush_and_decline(r, TowerDecline.knob)
    }
    if (t.dev_backend != TowerDev.vulkan) {
        return vt_flush_and_decline(r, TowerDecline.flavor)
    }
    if (!t.q8 && !t.gemm_f16) {
        return vt_flush_and_decline(r, TowerDecline.quant_mode)
    }
    let f16w = t.gemm_f16
    let d = t.d_model
    let ff = t.n_ff
    let hs = d / t.n_head
    let hsp = 64l
    if (hs != 64l || d % 64l != 0l || ff % 64l != 0l || npos > VT_MAX_ENCODE_ROWS || npos <= 0l) {
        return vt_flush_and_decline(r, TowerDecline.shape)
    }
    let arm = vt_arm(r)
    if (!arm.ok) {
        vt_x_flush(r)
        return false
    }
    let khr = arm.khr
    if (!vk_range_ok(vt_aud_weight_bytes(t)) || !vk_range_ok(vt_scratch_max_bytes(tower_cap_rows(npos), d, ff, d, false, false))) {
        return vt_flush_and_decline(r, TowerDecline.shape)
    }
    let v = vt_tile(false, d, npos).v
    let vff = vt_tile(false, ff, npos).v
    let cm2 = vt_cm2_feed()
    if (!vt_ln_chain_ensure(khr, hsp, v, vff) || (f16w && (!cm2 || !f16_gemm_ensure(arm.gemm_khr)))) {
        return vt_flush_and_decline(r, TowerDecline.device)
    }
    if (!vt_aud_attach(r, t)) {
        return vt_flush_and_decline(r, TowerDecline.memory)
    }
    var rl & = g_vt_rl
    if (!vt_ln_ready(rl, r, t.n_layer, npos, d, ff, hs, t.n_head * hsp, cm2, rail_lane_of(f16w), khr, v, vff, vt_ct_meta_bytes(ct, cm2))) {
        return false
    }
    rl.gemm_khr = arm.gemm_khr
    if (ct.on && !vt_ct_ready(r, ct)) {
        return vt_flush_and_decline(r, TowerDecline.memory)
    }
    vt_rail_seats(rl, r, t.layers_off)
    if (ct.on) {
        vt_ct_sites(rl, ct)
        vt_ct_seats(rl, r)
    }
    if (!f16w) {
        let nrec_blocks = t.n_layer * LN_BLOCK_GEMMS
        vt_sched_walk(rl.maps, r.meta, nrec_blocks + tower_ct_gemms(ct)) $(rid : int64) : TowerSchedRec {
            return rid < nrec_blocks ? tower_ln_rec(rl, g_vt_ln_offs, d, ff, npos, rid) : tower_ct_rec(rl, ct, rid - nrec_blocks)
        }
    }
    let bx = npos * d * 4l
    if (r.x_ready) {
        vt_x_drop_pending(r)
    } else {
        upload_region_at(r.x_dev, 0l, unsafe(addr<uint8 const?>(s.x[0])), bx)
    }
    vt_rail_one_shot(rl, "whisper-class", t.n_layer, npos) $ {
        tower_aud_blocks(rl, t, g_vt_ln_offs, ct, ln_post, npos)
        if (ct.on) {
            vhz_dep(rl.raw, rl.h, VT_TAIL_OUT, 0u, true)
            cmd_copy_range(rl.raw, r.ct_out_dev, 0l, r.ct_host.buf, 0l, ct.rows * ct.out * 4l)
        } else {
            vhz_dep(rl.raw, rl.h, ln_post ? VT_XB : VT_X, 0u, true)
            cmd_copy_range(rl.raw, ln_post ? r.xb_dev : r.x_dev, 0l, r.host.buf, 0l, bx)
        }
        vt_pt(rl.raw, RailProfRole.copy_out)
    }
    return vt_aud_served(r, s, ct, ln_post, npos, t.n_layer)
}

//! the rows the encode hands back: the tail's soft tokens, the post-normed rows (named by the handoff while on the device), or the residual rows
def private vt_aud_served(var r : VtResident; var s : EncoderState; ct : TowerChatTail; ln_post : bool; npos, nl : int64) : bool {
    if (ct.on) {
        g_vt_tail_encodes++
        ensure_length(s.out, ct.rows * ct.out)
        return vt_served_from(r, r.ct_host, s.out, ct.rows * ct.out * 4l, npos, nl)
    }
    let bx = npos * r.d * 4l
    if (ln_post) {
        g_vt_aud_out = TowerEncOut(host = intptr(unsafe(addr(s.xb[0]))), bytes = bx, dev = r.xb_dev)
        return vt_served(r, s.xb, bx, npos, nl)
    }
    return vt_served(r, s.x, bx, npos, nl)
}

//! the blocks, with the projector tail behind where `tower_ct_shape_ok` admits it; answers which served, so the CPU tail runs exactly when the device's did not
[hot_path]
def private vulkan_audio_tower_blocks_tail(t : AudioTower; var s : EncoderState; npos : int64) : TowerTailServed {
    var ct = TowerChatTail()
    if (tower_ct_shape_ok(t, npos)) {
        ct = vt_ct_prepare(t, npos, vt_cm2_feed())
    } else {
        vt_decline(g_vt_aud, TowerDecline.shape)   // the tail alone declines; the blocks serve and the CPU tail reads their rows
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

//! the largest weight plane the lane binds: the q8 quants, or the f16 lane's halfwords
def private vt_aud_weight_bytes(t : AudioTower) : int64 => t.q8 ? t.qblob.n : t.wblob.n * 2l

//! a new model's f32 rows from `rows_off` as the norms plane, the q8 seats stubs: the f16 lane's upload ahead of its halfword plane
[cold_path]
def private vt_upload_rows(var r : VtResident; key : uint64; blob : PlaneF; rows_off, rows_n : int64) : bool {
    vt_upload_begin(r, key)
    r.wq_bytes = 0l
    r.ws_bytes = 0l
    r.wq_dev = vt_dev_buf(r, VT_STUB_BYTES)
    r.ws_dev = vt_dev_buf(r, VT_STUB_BYTES)
    r.ones_off = rows_n
    r.norms_bytes = rows_n * 4l
    r.norms_dev = vt_dev_buf(r, r.norms_bytes)
    if (r.oom) {
        vt_release(r)
        return false
    }
    upload_region_at(r.norms_dev, 0l, unsafe(reinterpret<uint8 const?>(plane_at(blob, rows_off))), r.norms_bytes)
    r.cap_rows = 0l
    return true
}

[arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains")]
def private vt_aud_attach(var r : VtResident; t : AudioTower) : bool {
    tower_aud_offs(t, g_vt_ln_offs)
    let lay = active_q8_repack_layout(g_active_needs_repack)
    let norms_plane_n = t.fblob.n - t.layers_off   //! the blocks' rows, the post-norm and the projector tail's f32 rows behind them
    let src = vt_dev_src(t)
    let key = (src.on ? vt_key_dev(src, t.fblob, t.layers_off, norms_plane_n, t.n_layer)
        : vt_key(t.qblob, t.qscales, t.fblob, t.layers_off, norms_plane_n, 0l, t.n_layer, lay.mr, lay.kgroup, lay.wbias))
    return r.key == key || vt_aud_upload(r, t, key, lay, src, norms_plane_n)
}

//! the whisper-class tower's planes uploaded under `key`: the lane's GEMM planes (q8 pieces, or the f16 halfwords as `wb`) beside the blocks' f32 rows
[cold_path]
def private vt_aud_upload(var r : VtResident; t : AudioTower; key : uint64; lay : Q8RepackLayout; src : VtDevSrc; norms_plane_n : int64) : bool {
    return vt_upload_rows(r, key, t.fblob, t.layers_off, norms_plane_n) && vt_upload_wb(r, t.wblob) if (t.gemm_f16)
    var inscope regions <- tower_aud_regions(t, g_vt_ln_offs)
    return vt_upload(r, key, t.qblob, t.qscales, regions, t.fblob, t.layers_off, norms_plane_n, 0l, lay, src, VT_AUD_PIECE)
}

let private VT_AUD_PIECE = tower_family_piece(VT_HOME, "aud")           //! a whisper-class tower's q8 pieces
let private VT_AUD_FRONT16_PIECE = tower_front16_piece(VT_HOME, "aud")  //! ... its conv stem's first conv as the f16 slab

//! a whisper-class tower's pieces into a Vulkan device block (`#tower-dev-block`): `tower_aud_bake` under this home's prefix, the whisper bake's too
def vulkan_audio_tower_bake_into(t : AudioTower; var b : TowerDevBuild) {
    tower_aud_bake(t, b, VT_HOME)
}

//! the whisper-class chains serve `family`'s load on its lane (`q8`, else f16): the knob, the device with the stem's f16 feed, the f16 tile on the f16 lane; false said
def private vt_whisper_class_serves(family : string; q8 : bool) : bool {
    return vt_flavor_refused(family, VK_DECLINE_WORDS_TOWER_KNOB) if (!g_vt_env_tower)
    return vt_flavor_refused(family, VK_DECLINE_WORDS_DEVICE) if (!vulkan_tower_arms())
    return vt_flavor_refused(family, "its conv stem rides the f16 GEMM feed, off on this device") if (!vt_cm2_feed())
    return q8 || f16_gemm_ensure(f16_gemm_khr()) || vt_flavor_refused(family, "the f16 lane's GEMM tile did not build")
}

//! a whisper encoder's Vulkan chain serves the load's lane (`q8`, else f16): `vt_whisper_class_serves`
def vulkan_whisper_tower_serves(q8 : bool) : bool => vt_whisper_class_serves("the whisper encoder", q8)

//! the standalone tower's Vulkan device block
[cold_path]
def private vulkan_audio_tower_bake(t : AudioTower; var b : TowerDevBuild) : bool {
    vulkan_audio_tower_bake_into(t, b)
    return true
}

//! gemma4a's Vulkan chain serves the load's lane (`q8`, else exact as f16): the knob, the device with the front's f16 feed, the f16 lane's tile family and tile
def private vulkan_gemma4a_serves_model(q8 : bool) : bool
    => (vt_whisper_class_serves("gemma4a", true)
        && (q8 || (vulkan_tower_tiles_on() && f16_gemm_ensure(f16_gemm_khr())) || vt_flavor_refused("gemma4a", "the f16 lane's tile family or GEMM tile did not build")))

//! the standalone whisper-class tower serves on the Vulkan chain on the load's lane (`vt_whisper_class_serves`)
def private vulkan_aud_serves_model(q8 : bool) : bool => vt_whisper_class_serves("the whisper-class tower", q8)

//! qwen3a serves on the Vulkan chains on the load's lane: its core's (`vt_whisper_class_serves`), its front's shuffle
def private vulkan_q3a_serves_model(q8 : bool) : bool
    => vt_whisper_class_serves("qwen3a", q8) && (ensure_tower_q3a_shuffle_cls() || vt_flavor_refused("qwen3a", "its front's shuffle class did not build"))

let private VT_STEM_MEL = 0x1000000u   //! the whisper-class stem's hazard bits: the mel block, conv1's im2col and its rows
let private VT_STEM_COL = 0x2000000u
let private VT_STEM_Y1 = 0x4000000u

[cold_path]
def private vt_aud_front_slab(var r : VtResident; t : AudioTower) : bool {
    let d = t.d_model
    let kdp = tower_aud_stem_kdp(t)
    r.stem_kdp = kdp
    let src = vt_dev_src(t)
    var inscope slab16 <- src.on ? default<array<uint16>> : tower_aud_stem16(t)
    let rows = tower_aud_stem_rows(t)
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

//! the stem's mel block, conv1's im2col and its rows at `tin` frames; the chain's scratch rebuild releases this scratch with it
[cold_path]
def private vt_aud_front_scratch(var r : VtResident; tin, nm, d : int64) : bool {
    vt_front_release(r)
    r.oom = false
    r.front_cap = tin
    r.fxin_dev = vt_call_buf(r, nm * tin * 4l)
    r.fcol_dev = vt_dev_buf(r, tin * r.stem_kdp * 2l)
    r.fy1_dev = vt_dev_buf(r, tin * d * 4l)
    if (r.oom) {
        vt_front_release(r)
        return false
    }
    return true
}

//! the stem's planes seated on the rail at `tin` frames of `nm` mels: the mel block, the im2col, conv1's rows, the slabs
def private vt_aud_front_seats(var rl : VtRail; r : VtResident; tin, nm : int64) {
    rail_seat(rl, TowerBuf.planes, r.fxin_dev, nm * tin * 4l, VT_STEM_MEL)
    rail_seat(rl, TowerBuf.col, r.fcol_dev, tin * r.stem_kdp * 2l, VT_STEM_COL)
    rail_seat(rl, TowerBuf.stage1, r.fy1_dev, tin * r.d * 4l, VT_STEM_Y1)
    rail_seat(rl, TowerBuf.fw16, r.fw16_dev, r.fw16_bytes, 0u)
    rail_seat(rl, TowerBuf.fnorm, r.fnorm_dev, r.fnorm_bytes, 0u)
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

[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#whisper-chains")]
def private vulkan_audio_conv_front(t : AudioTower; var s : EncoderState; mel : MelChunk) : bool {
    var r & = g_vt_aud
    g_vt_aud_out.bytes = 0l
    if (!vt_gate(r, true)) {
        return false
    }
    return vt_decline(r, TowerDecline.flavor) if (t.dev_backend != TowerDev.vulkan)
    return vt_decline(r, TowerDecline.quant_mode) if (!t.q8 && !t.gemm_f16)
    let f16w = t.gemm_f16
    let tin = AUDIO_CHUNK_FRAMES
    let tt = t.n_ctx
    let d = t.d_model
    let ff = t.n_ff
    let nm = t.n_mel
    let hs = d / t.n_head
    let hsp = 64l
    if (hs != 64l || d % 64l != 0l || ff % 64l != 0l || 3l * d > max(d, ff) || tt > VT_MAX_ENCODE_ROWS || mel.n_len != tin || mel.n_mel != nm
            || !tower_aud_has_conv(t)) {
        return vt_decline(r, TowerDecline.shape)
    }
    let arm = vt_arm(r, true)
    if (!arm.ok) {
        return false
    }
    let khr = arm.khr
    if (!vt_cm2_feed()) {
        return vt_decline(r, TowerDecline.device)
    }
    if (!vk_range_ok(vt_aud_weight_bytes(t)) || !vk_range_ok(vt_scratch_max_bytes(tower_cap_rows(tt), d, ff, d, false, false))) {
        return vt_decline(r, TowerDecline.shape)
    }
    let v = vt_tile(false, d, tt).v
    let vff = vt_tile(false, ff, tt).v
    let tc_d = cm2_tile_cols(d, tt)
    let tc_ff = cm2_tile_cols(ff, tt)
    if (!vt_ln_chain_ensure(khr, hsp, v, vff) || !f16_gemm_ensure(arm.gemm_khr) || !ensure_tts_im2col16_cls() || !ensure_tower_bias_act_cls()
            || !cm2_cls_ensure(int(KqFmt.q8), tc_d) || !cm2_cls_ensure(int(KqFmt.q8), tc_ff)) {
        return vt_decline(r, TowerDecline.device)
    }
    if (!vt_aud_attach(r, t) || (!r.front && !vt_aud_front_slab(r, t))) {
        vt_release(r)
        return vt_decline(r, TowerDecline.memory)
    }
    let g_d = cm2_tiles_at(d, tt, tc_d)
    let g_ff = cm2_tiles_at(ff, tt, tc_ff)
    if (!vt_ln_scratch(r, t.n_layer, tt, d, ff, hs, d, g_d, g_ff, khr, true, tc_d, 0l) || (r.front_cap != tin && !vt_aud_front_scratch(r, tin, nm, d))) {
        return vt_decline(r, TowerDecline.memory)
    }
    var rl & = g_vt_rl
    vt_rail_begin(rl, rail_lane_of(f16w), true, khr, arm.gemm_khr)
    if (!f16w) {
        rl.sites[int(RailSite.conv2)] = VtTile(v = 0, tc = tc_d)
        vt_sched_walk(rl.maps, r.meta, 1l) $(_rid : int64) : TowerSchedRec {
            return tower_aud_stem_rec(rl, t)
        }
    }
    upload_region_at(r.fxin_dev, 0l, unsafe(addr<uint8 const?>(mel.data[0])), nm * tin * 4l)
    vt_rail_seats(rl, r, t.layers_off)
    vt_aud_front_seats(rl, r, tin, nm)
    vt_rail_one_shot(rl, "whisper-class stem", 2l, tt) $ {
        tower_aud_stem(rl, t, tin)
    }
    let bx = tt * d * 4l
    r.x_pend_ptr = unsafe(addr(s.x[0]))
    r.x_pend_bytes = bx
    r.x_ready = true
    g_vt_convs++
    vt_announce(r)
    return true
}

[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_VISION.md#vk-vision-chains")]
def private vulkan_gemma3v_chain(t : Gemma3vTower; var s : Gemma3vState; planes : array<float>; w, h, npos : int64) : bool {
    var r & = g_vt_g3v
    return false if (!vt_vis_gate(r, t.q8, t.dev_backend))
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
        return vt_decline(r, TowerDecline.shape)
    }
    let arm = vt_arm(r)
    if (!arm.ok) {
        return false
    }
    let khr = arm.khr
    if (!vk_range_ok(t.qblob.n) || !vk_range_ok(t.blob.n * 4l) || !vk_range_ok(vt_scratch_max_bytes(tower_cap_rows(npos), d, ff, dp, false, false))) {
        return vt_decline(r, TowerDecline.shape)
    }
    let v = vt_tile(false, d, npos).v
    let vff = vt_tile(false, ff, npos).v
    let cm2 = !f32w && vt_cm2_feed()
    if (!vt_ln_chain_ensure(khr, hsp, v, vff) || !ensure_tower_patch_im2col_cls() || !ensure_f32_gemm_b_cls() || !ensure_f32_gemm_cls()
            || !ensure_tower_pool_2d_cls() || !ensure_tower_rms_cls() || !ensure_tower_bias_act_cls()) {
        return vt_decline(r, TowerDecline.device)
    }
    tower_gemma3v_offs(t, g_vt_ln_offs)
    let lay = active_q8_repack_layout(g_active_needs_repack)
    let blob_n = t.blob.n
    let src = vt_dev_src(t)
    let key = (f32w ? vt_key_f16(t.wblob, 0l, 0l, t.blob, 0l, blob_n, t.n_layer)
        : (src.on ? vt_key_dev(src, t.blob, 0l, blob_n, t.n_layer) : vt_key(t.qblob, t.qscales, t.blob, 0l, blob_n, 0l, t.n_layer, lay.mr, lay.kgroup, lay.wbias)))
    if (r.key != key && !(f32w ? vt_upload_f32(r, key, t.blob, blob_n, 0l)
            : vt_upload_g3v(r, key, t, lay, src))) {
        return vt_decline(r, TowerDecline.memory)
    }
    var rl & = g_vt_rl
    if (!vt_ln_ready(rl, r, t.n_layer, npos, d, ff, hs, dp, cm2, f32w ? RailLane.f32 : RailLane.q8, khr, v, vff, 0l)) {
        return false
    }
    if (!vt_vis_bufs(r, w, h, pd, 0l, 0l, max(d, t.proj_dim))) {
        return vt_decline(r, TowerDecline.memory)
    }
    vt_rail_seats(rl, r, 0l)
    if (!f32w) {
        vt_sched_walk(rl.maps, r.meta, t.n_layer * LN_BLOCK_GEMMS) $(rid : int64) : TowerSchedRec {
            return tower_ln_rec(rl, g_vt_ln_offs, d, ff, npos, rid)
        }
    }
    vt_planes_up(r, planes, w, h)
    let vis = TowerVisTail(nout = nout, gw = n_side, merge = t.merge, d = d, proj = t.proj_dim, soft_norm_w = t.soft_norm_w, proj_w = t.proj_w, eps = t.eps)
    let bp_out = nout * t.proj_dim * 4l
    vt_rail_one_shot(rl, "gemma3v", t.n_layer, npos) $ {
        tower_gemma3v_chain(rl, t, g_vt_ln_offs, vis, w, pd, hsp, npos)
        vt_tail_copy_out(rl.raw, rl.h, r, bp_out)
    }
    g_vt_convs++
    g_vt_tail_encodes++
    ensure_length(s.tail, nout * t.proj_dim)
    s.tail_ready = vt_served_from(r, r.tail_host, s.tail, bp_out, npos, t.n_layer)
    return s.tail_ready
}

[cold_path]
def private vt_upload_q3v(var r : VtResident; key : uint64; t : Qwen3vTower; lay : Q8RepackLayout) : bool {
    var inscope regions <- qwen3v_all_regions(t)
    return vt_upload(r, key, t.qblob, t.qscales, regions, t.blob, 0l, t.blob.n, 0l, lay, vt_dev_src(t), VT_Q3V_PIECE)   // the whole f32 blob: the stem's, the mergers' and the tail's rows at their file offsets
}

let private VT_Q3V_PIECE = "vk.q3v"   //! qwen3v's q8 pieces' prefix

//! qwen3v's Vulkan device block (`#tower-dev-block`): the q8 lane's block GEMMs gathered
[cold_path, arch(at = "../ARCHITECTURE_IMAGE_GPU_PLANES.md#tower-dev-block")]
def private vulkan_qwen3v_bake(t : Qwen3vTower; var b : TowerDevBuild) : bool {
    var inscope regions <- qwen3v_all_regions(t)
    tower_bake_q8(b, VT_Q3V_PIECE, t.qblob, t.qscales, regions)
    return true
}

[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_VISION.md#vk-vision-chains")]
def private vulkan_qwen3v_chain(t : Qwen3vTower; var s : Qwen3vState; planes : array<float>; w, h, npos : int64) : bool {   // nolint:STYLE038 — the dispatch chain is the shape
    var r & = g_vt_q3v
    return false if (!vt_vis_gate(r, t.q8, t.dev_backend))
    let f32w = !t.q8
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
        return vt_decline(r, TowerDecline.shape)
    }
    let arm = vt_arm(r)
    if (!arm.ok) {
        return false
    }
    let khr = arm.khr
    if (!vk_range_ok(t.qblob.n) || !vk_range_ok(t.blob.n * 4l) || !vk_range_ok(vt_scratch_max_bytes(tower_cap_rows(npos), d, ff, dp, true, false))) {
        return vt_decline(r, TowerDecline.shape)
    }
    let fa_ok = fa_stamp_ensure(khr, hsp, false, false, false, false, true)
    if (!fa_ok || !ensure_tower_ln_cls() || !ensure_tower_clamp_rq_cls() || !ensure_tower_bias_act_cls() || !ensure_tower_rope_tab_cls()
            || !ensure_tower_hpad_cls() || !ensure_tower_hpad16_cls() || !ensure_tower_hunpad_cls() || !ensure_tower_post_add_ln_cls()
            || !ensure_tower_patch_im2col_cls() || !ensure_f32_gemm_cls() || !ensure_f32_gemm_b_cls() || !ensure_tower_stem_asm_cls() || !ensure_tower_act_cls()) {
        return vt_decline(r, TowerDecline.device)
    }
    let v = vt_tile(false, d, npos).v
    let vff = vt_tile(false, ff, npos).v
    let vqkv = vt_tile(false, 3l * d, npos).v
    if (!f32w && (!q8_batch_cls_ensure(v) || !q8_batch_cls_ensure(vff) || !q8_batch_cls_ensure(vqkv))) {
        return vt_decline(r, TowerDecline.device)
    }
    let lay = active_q8_repack_layout(g_active_needs_repack)
    let src = vt_dev_src(t)
    let key = (f32w ? vt_key_f16(t.wblob, 0l, 0l, t.blob, 0l, t.blob.n, t.n_layer)
        : (src.on ? vt_key_dev(src, t.blob, 0l, t.blob.n, t.n_layer) : vt_key(t.qblob, t.qscales, t.blob, 0l, t.blob.n, 0l, t.n_layer, lay.mr, lay.kgroup, lay.wbias)))
    if (r.key != key && !(f32w ? vt_upload_f32(r, key, t.blob, t.blob.n, 0l) : vt_upload_q3v(r, key, t, lay))) {
        return vt_decline(r, TowerDecline.memory)
    }
    let nrec = t.n_layer * QWEN3V_GEMMS
    let meta_bytes = (nrec * 4l + t.n_layer * (2l * vt_gemm_groups(d, npos) + vt_gemm_groups(ff, npos) + vt_gemm_groups(3l * d, npos))) * 4l
    let scratch_ok = vt_scratch_fits(r, npos, d, ff, dp, meta_bytes, khr, true, f32w) || vt_scratch_build(r, npos, d, ff, hs, dp, meta_bytes, khr, true, f32w)
    if (!scratch_ok || !vt_vis_bufs(r, w, h, pd, 0l, r.cap_rows * d * 4l, max(d4, (nds + 1l) * proj))) {
        return vt_decline(r, TowerDecline.memory)
    }
    let pos_rs = qwen3v_pos_resized(t, s)
    var rl & = g_vt_rl
    vt_rail_begin(rl, f32w ? RailLane.f32 : RailLane.q8, false, khr, false)
    rl.sites[int(RailSite.d)] = VtTile(v = v)
    rl.sites[int(RailSite.ff)] = VtTile(v = vff)
    rl.sites[int(RailSite.qkv)] = VtTile(v = vqkv)
    vt_rail_seats(rl, r, 0l)
    if (!f32w) {
        vt_sched_walk(rl.maps, r.meta, nrec) $(rid : int64) : TowerSchedRec {
            return tower_qwen3v_rec(rl, t, npos, rid)
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
    vt_rail_one_shot(rl, "qwen3v", t.n_layer, npos) $ {
        tower_qwen3v_chain(rl, t, w, hsp, npos, pos_rs)
        vt_tail_copy_out(rl.raw, rl.h, r, (nds + 1l) * bslice)
    }
    g_vt_convs++
    g_vt_tail_encodes++
    if (nds > 0l) {
        host_buf_read(r.tail_host, unsafe(addr<void?>(s.ds_stash[0])), nds * bslice, bslice)
    }
    return vt_served_from(r, r.tail_host, s.ds_slice, bslice, npos, t.n_layer)
}

[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_VISION.md#vk-vision-chains")]
def private vulkan_qwen25v_chain(t : Qwen25vTower; var s : Qwen25vState; planes : array<float>; w, h, npos : int64) : bool {
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
        return vt_decline(r, TowerDecline.shape)  // the chain's sets are built at the 128 head alone (the family's 80-wide heads)
    }
    let arm = vt_arm(r, true)
    if (!arm.ok) {
        return false
    }
    let khr = arm.khr
    let gemm_khr = arm.gemm_khr
    let w_off = t.layer_w
    let w_n = t.n_layer * t.layer_w_stride
    if (!vk_range_ok(w_n * 2l) || !vk_range_ok(t.blob.n * 4l) || !vk_range_ok(vt_scratch_max_bytes(tower_cap_rows(npos), d, ff, dp, false, true))) {
        return vt_decline(r, TowerDecline.shape)
    }
    let fa_ok = fa_stamp_ensure(khr, hsp, false, false, false, false, true)
    if (!fa_ok || !f16_gemm_ensure(gemm_khr) || !ensure_tower_clamp_cvt_cls() || !ensure_tower_rms_cls() || !ensure_tower_bias_act_cls()
            || !ensure_tower_rope_tab_cls() || !ensure_tower_hpad_cls() || !ensure_tower_hpad16_cls() || !ensure_tower_hunpad_cls()
            || !ensure_tower_win_attn_cls() || !ensure_tower_post_add_rms_cls() || !ensure_tower_bias_gate16_cls() || !ensure_tower_patch_im2col_cls()
            || !ensure_f32_gemm_cls() || !ensure_f32_gemm_b_cls() || !ensure_tts_row_gather_cls() || !ensure_tower_act_cls()) {
        return vt_decline(r, TowerDecline.device)
    }
    let key = vt_key_f16(t.wblob, w_off, w_n, t.blob, 0l, t.blob.n, t.n_layer)
    let planes_ok = r.key == key || vt_upload_f16(r, key, t.wblob, w_off, w_n, t.blob, 0l, t.blob.n)
    let scratch_ok = planes_ok && (vt_scratch_f16_fits(r, npos, nwin, d, ff, dp, khr, gemm_khr) || vt_scratch_f16_build(r, npos, nwin, d, ff, hs, dp, khr, gemm_khr))
    if (!scratch_ok || !vt_vis_bufs(r, w, h, pd, r.cap_rows * 4l, 0l, max(d4, t.proj_dim))) {
        return vt_decline(r, TowerDecline.memory)
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
    var rl & = g_vt_rl
    vt_rail_begin(rl, RailLane.q8, true, khr, gemm_khr)
    vt_rail_seats(rl, r, 0l)
    vt_rail_one_shot(rl, "qwen25v", t.n_layer, npos) $ {
        tower_qwen25v_chain(rl, t, w, hsp, npos, nwin)
        vt_tail_copy_out(rl.raw, rl.h, r, bp_out)
    }
    g_vt_convs++
    g_vt_tail_encodes++
    ensure_length(s.mout, nout * t.proj_dim)
    return vt_served_from(r, r.tail_host, s.mout, bp_out, npos, t.n_layer)
}


[cold_path]
def private vt_upload_g4v(var r : VtResident; key : uint64; t : Gemma4vTower; ones_n : int64; lay : Q8RepackLayout) : bool {
    var inscope regions <- gemma4v_all_regions(t)
    return vt_upload(r, key, t.qblob, t.qscales, regions, t.blob, 0l, t.blob.n, ones_n, lay, vt_dev_src(t), VT_G4V_PIECE)   // the whole f32 blob: the stem's and the tail's rows at their file offsets
}

let private VT_G4V_PIECE = "vk.g4v"   //! gemma4v's q8 pieces' prefix

//! gemma4v's Vulkan device block (`#tower-dev-block`): the q8 lane's block GEMMs gathered
[cold_path, arch(at = "../ARCHITECTURE_IMAGE_GPU_PLANES.md#tower-dev-block")]
def private vulkan_gemma4v_bake(t : Gemma4vTower; var b : TowerDevBuild) : bool {
    var inscope regions <- gemma4v_all_regions(t)
    tower_bake_q8(b, VT_G4V_PIECE, t.qblob, t.qscales, regions)
    return true
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
        return vt_decline(r, TowerDecline.shape)
    }
    let arm = vt_arm(r)
    if (!arm.ok) {
        return false
    }
    if (!vk_range_ok(t.blob.n * 4l) || !vk_range_ok(t.wblob.n * 2l) || !vk_range_ok(vt_scratch_max_bytes(tower_cap_rows(npos), d, d, 0l, false, false))) {
        return vt_decline(r, TowerDecline.shape)
    }
    if (!ensure_tower_patch_im2col_cls() || !ensure_tower_ln_cls() || !vt_f32_ensure(t.patch_bf16) || !vt_f32_ensure(t.proj_bf16) || !ensure_tower_pos_add_2d_cls()
            || !ensure_tower_rms_cls()) {
        return vt_decline(r, TowerDecline.device)
    }
    let key = vt_key_f16(t.wblob, 0l, t.wblob.n, t.blob, 0l, t.blob.n, 0l)
    let fresh_w = r.key != key
    if ((fresh_w && !(vt_upload_f32(r, key, t.blob, t.blob.n, t.n_embd) && vt_upload_wb(r, t.wblob)))
            || (!vt_scratch_fits(r, npos, d, d, 0l, 0l, arm.khr, false) && !vt_scratch_build(r, npos, d, d, 64l, 0l, 0l, arm.khr, false))
            || !vt_vis_bufs(r, w, h, pd, 0l, 0l, proj)) {
        return vt_decline(r, TowerDecline.memory)
    }
    vt_planes_up(r, planes, w, h)
    let bp_out = npos * proj * 4l
    var rl & = g_vt_rl
    vt_rail_begin(rl, RailLane.q8, false, arm.khr, false)
    vt_rail_seats(rl, r, 0l)
    vt_rail_one_shot(rl, "gemma4uv", 0l, npos) $ {
        tower_gemma4uv_chain(rl, t, w, n_cols, npos)
        vt_tail_copy_out(rl.raw, rl.h, r, bp_out)
    }
    g_vt_convs++
    return vt_served_from(r, r.tail_host, out, bp_out, npos, 0l)
}

def private vt_f32_ensure(bf16 : bool) : bool
    => bf16 ? (ensure_f32_gemm_bf16_cls() && ensure_f32_gemm_bf16_b_cls()) : (ensure_f32_gemm_cls() && ensure_f32_gemm_b_cls())

let private VT_G3V_PIECE = "vk.g3v"   //! gemma3v's q8 pieces' prefix

[cold_path]
def private vt_upload_g3v(var r : VtResident; key : uint64; t : Gemma3vTower; lay : Q8RepackLayout; src : VtDevSrc) : bool {
    var inscope regions <- gemma3v_all_regions(t)
    return vt_upload(r, key, t.qblob, t.qscales, regions, t.blob, 0l, t.blob.n, 0l, lay, src, VT_G3V_PIECE)
}

//! gemma3v's Vulkan device block (`#tower-dev-block`): the q8 lane's block GEMMs gathered
[cold_path, arch(at = "../ARCHITECTURE_IMAGE_GPU_PLANES.md#tower-dev-block")]
def private vulkan_gemma3v_bake(t : Gemma3vTower; var b : TowerDevBuild) : bool {
    var inscope regions <- gemma3v_all_regions(t)
    tower_bake_q8(b, VT_G3V_PIECE, t.qblob, t.qscales, regions)
    return true
}

//! a vision chain's q8 lane takes the Vulkan flavor where the tower knob is on and the device arms; its exact lane's planes upload
//! as stored, so the CPU image serves it
def private vulkan_vision_serves_model(q8 : bool) : bool => q8 && g_vt_env_tower && vulkan_tower_arms()

[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_VISION.md#vk-vision-chains")]
def private vulkan_gemma4v_chain(t : Gemma4vTower; var s : Gemma4vState; planes : array<float>; w, h, npos : int64) : bool {
    var r & = g_vt_g4v
    return false if (!vt_vis_gate(r, t.q8, t.dev_backend))
    let f32w = !t.q8
    let d = t.n_embd
    let ff = t.n_ff
    let hs = d / t.n_head
    let pd = t.patch_dim
    let n_cols = w / t.patch
    let n_rows = h / t.patch
    let nout = (n_cols / t.merge) * (n_rows / t.merge)
    let vis = TowerVisTail(nout = nout, gw = n_cols, merge = t.merge, d = d, proj = t.proj_dim, soft_norm_w = -1l, proj_w = t.proj_w, proj_bf16 = t.proj_bf16, eps = t.eps,
        affine = true, scale = sqrt(float(d)), std = t.has_std, std_bias = t.std_bias, std_scale = t.std_scale, clamp = gemma4v_clamp_at(t, t.proj_clamp))
    if (hs != 64l || d % 64l != 0l || ff % 64l != 0l || npos > VT_MAX_ENCODE_ROWS || npos <= 0l || npos != n_cols * n_rows
            || n_cols % t.merge != 0l || n_rows % t.merge != 0l) {
        return vt_decline(r, TowerDecline.shape)
    }
    let arm = vt_arm(r)
    if (!arm.ok) {
        return false
    }
    let khr = arm.khr
    if (!vk_range_ok(t.qblob.n) || !vk_range_ok(t.blob.n * 4l) || !vk_range_ok(t.wblob.n * 2l) || !vk_range_ok(vt_scratch_max_bytes(tower_cap_rows(npos), d, ff, 0l, false, false))) {
        return vt_decline(r, TowerDecline.shape)
    }
    let fa_ok = fa_stamp_ensure(khr, hs, false, false, false, false, true)
    if (!fa_ok || !ensure_tower_rms_cls() || !ensure_tower_clamp_cls() || !ensure_tower_clamp_rq_cls() || !ensure_tower_rope2d_cls()
            || !ensure_tower_post_add_cls() || !ensure_tower_geglu_cls() || !ensure_tower_clamp_cvt_cls() || !ensure_tower_patch_im2col_cls()
            || !vt_f32_ensure(t.patch_bf16) || !vt_f32_ensure(t.proj_bf16) || !vt_f32_ensure(f32w && t.blk_bf16) || !ensure_tower_pos_add_2d_cls() || !ensure_tower_pool_2d_cls()
            || !ensure_tower_affine_rows_cls() || !ensure_tower_clamp_copy_cls()) {
        return vt_decline(r, TowerDecline.device)
    }
    let v = vt_tile(false, d, npos).v
    let vff = vt_tile(false, ff, npos).v
    if (!f32w && (!q8_batch_cls_ensure(v) || !q8_batch_cls_ensure(vff))) {
        return vt_decline(r, TowerDecline.device)
    }
    let lay = active_q8_repack_layout(g_active_needs_repack)
    let src = vt_dev_src(t)
    let key = tower_fold_words(f32w ? vt_key_f16(t.wblob, 0l, t.wblob.n, t.blob, 0l, t.blob.n, t.n_layer)
        : (src.on ? vt_key_dev(src, t.blob, 0l, t.blob.n, t.n_layer) : vt_key(t.qblob, t.qscales, t.blob, 0l, t.blob.n, hs, t.n_layer, lay.mr, lay.kgroup, lay.wbias)),
        unsafe(reinterpret<uint8 const?>(t.wblob.p)), t.wblob.n * 2l)
    let fresh_w = r.key != key
    let ones_n = max(hs, d)
    if (fresh_w && !((f32w ? vt_upload_f32(r, key, t.blob, t.blob.n, ones_n) : vt_upload_g4v(r, key, t, ones_n, lay)) && vt_upload_wb(r, t.wblob))) {
        return vt_decline(r, TowerDecline.memory)
    }
    let nrec = t.n_layer * GEMMA4V_GEMMS
    let meta_bytes = (nrec * 4l + t.n_layer * (5l * vt_gemm_groups(d, npos) + 2l * vt_gemm_groups(ff, npos))) * 4l
    let scratch_ok = vt_scratch_fits(r, npos, d, ff, 0l, meta_bytes, khr, false, f32w) || vt_scratch_build(r, npos, d, ff, hs, 0l, meta_bytes, khr, false, f32w)
    if (!scratch_ok || !vt_vis_bufs(r, w, h, pd, 0l, 0l, max(d, t.proj_dim))) {
        return vt_decline(r, TowerDecline.memory)
    }
    var rl & = g_vt_rl
    vt_rail_begin(rl, f32w ? RailLane.f32 : RailLane.q8, false, khr, false)
    rl.sites[int(RailSite.d)] = VtTile(v = v)
    rl.sites[int(RailSite.ff)] = VtTile(v = vff)
    vt_rail_seats(rl, r, 0l)
    if (!f32w) {
        vt_sched_walk(rl.maps, r.meta, nrec) $(rid : int64) : TowerSchedRec {
            return tower_gemma4v_rec(rl, t, npos, rid)
        }
    }
    let pairs = hs / 4l
    g_vt_rope2d_tabs |> reserve(npos * 4l * pairs)
    g_vt_rope2d_tabs |> resize(npos * 4l * pairs)
    gemma4v_pack_rope_tabs(s, npos, pairs, unsafe(addr(g_vt_rope2d_tabs[0])))
    upload_region_at(r.tabs_dev, 0l, unsafe(addr<uint8 const?>(g_vt_rope2d_tabs[0])), npos * 4l * pairs * 4l)
    vt_planes_up(r, planes, w, h)
    let bp_out = nout * t.proj_dim * 4l
    vt_rail_one_shot(rl, "gemma4v", t.n_layer, npos) $ {
        tower_gemma4v_chain(rl, t, vis, w, n_cols, npos)
        vt_tail_copy_out(rl.raw, rl.h, r, bp_out)
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

var private @scratch @exact_size g_vt_q3a_xin : array<float>   //! the window's chunk blocks [ci][ih][iw] staged for the upload

[cold_path]
def private vt_q3a_slab(var r : VtResident; t : Qwen3aTower; key : uint64; lay : Q8RepackLayout) : bool {
    let src = vt_dev_src(t.core)
    var inscope slab16 <- src.on ? default<array<uint16>> : tower_q3a_front16(t)
    let span = tower_q3a_norm_span(t)
    var inscope regions <- t.cq8 ? tower_q3a_regions(t, false) : default<array<Q8Region>>
    let up = (t.cq8 ? vt_upload(r, key, t.qcblob, t.qcscales, regions, t.cblob, span.lo, span.hi - span.lo, 0l, lay, src, VT_Q3A_PIECE)
        : (vt_upload_rows(r, key, t.cblob, span.lo, span.hi - span.lo) && vt_upload_wb(r, t.wcblob)))
    return false if (!up)
    r.fw16_bytes = src.on ? vt_piece_bytes(src, VT_Q3A_FRONT16_PIECE) : long_length(slab16) * 2l
    r.fw16_dev = vt_dev_buf(r, r.fw16_bytes)
    if (r.oom || r.fw16_bytes == 0l) {
        vt_release(r)
        return false
    }
    if (src.on) {
        vt_put_piece(src, VT_Q3A_FRONT16_PIECE, r.fw16_dev)
    } else {
        upload_region_at(r.fw16_dev, 0l, unsafe(addr<uint8 const?>(slab16[0])), r.fw16_bytes)
    }
    r.front = true
    return true
}

let private VT_Q3A_PIECE = tower_family_piece(VT_HOME, "q3a")           //! qwen3a's own q8 pieces: the conv-out GEMM and the projector's pair
let private VT_Q3A_FRONT16_PIECE = tower_front16_piece(VT_HOME, "q3a")  //! ... its three convs as the f16 slab

//! qwen3a's Vulkan device block (`#tower-dev-block`): the core's whisper-class pieces, then qwen3a's own
[cold_path]
def private vulkan_qwen3a_bake(t : Qwen3aTower; var b : TowerDevBuild) : bool {
    vulkan_audio_tower_bake_into(t.core, b)
    tower_q3a_bake(t, b, VT_HOME)
    return true
}

let private VTF_PJH = 0x1000u   //! the device projector's hidden rows
let private VTF_PJY = 0x2000u   //! ... and its projected rows

//! qwen3a's front resident's seats on the tower's lane (feed, rows, slab, schedule, halfwords) and its front and projector sites over the f16 feed
def private vt_q3a_rail(var rl : VtRail; r : VtResident; t : Qwen3aTower; gemm_khr : bool) {
    let featw = 16l * t.n_ch
    vt_rail_begin(rl, rail_lane_of(t.cf16), true, false, gemm_khr)
    rl.norms_base = tower_q3a_norm_span(t).lo
    rl.cap_rows = r.cap_rows
    rail_seat(rl, TowerBuf.feat16, r.ffeat_dev, (r.cap_rows + TILE_READ_SLACK) * featw * 2l, VTF_FEAT)
    rail_seat(rl, TowerBuf.rows_out, r.fx_dev, r.cap_rows * t.core.d_model * 4l, VTF_X)
    rail_seat(rl, TowerBuf.fw16, r.fw16_dev, r.fw16_bytes, 0u)
    rail_seat(rl, TowerBuf.wb, r.wb_dev, r.wb_bytes, 0u)
    rail_seat(rl, TowerBuf.norms, r.norms_dev, r.norms_bytes, 0u)
    rail_seat(rl, TowerBuf.wq, r.wq_dev, r.wq_bytes, 0u)
    rail_seat(rl, TowerBuf.ws, r.ws_dev, r.ws_bytes, 0u)
    rail_seat(rl, TowerBuf.meta, r.meta.buf, r.meta_bytes, 0u)
    tower_q3a_binds(rl)
}

//! qwen3a's MLP projector on the device (`#tower-dev-block`) over `s.xb` into `s.out`; false declined, or where the front did not run this encode
[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#g4a-chains")]
def private vulkan_q3a_proj(t : Qwen3aTower; var s : EncoderState; npos : int64) : bool {
    var r & = g_vt_q3a_front
    return vt_decline(r, TowerDecline.flavor) if (t.core.dev_backend != TowerDev.vulkan)
    let d = t.core.d_model
    let pdim = t.proj_dim
    let featw = 16l * t.n_ch
    if (r.cap_rows < npos || r.fx_dev == 0ul || d % 64l != 0l || pdim % 64l != 0l || max(d, pdim) > featw || long_length(s.xb) < npos * d) {
        return vt_decline(r, TowerDecline.shape)
    }
    let t1 = vt_tile(true, d, npos)
    let t2 = vt_tile(true, pdim, npos)
    let lane_ok = t.cq8 ? (vt_tile_ensure(t1) && vt_tile_ensure(t2)) : f16_gemm_ensure(r.gemm_khr)
    if (!lane_ok || !ensure_tower_clamp_cvt_cls() || !ensure_tower_bias_act_cls()) {
        return vt_decline(r, TowerDecline.device)
    }
    if (r.pj_cap < r.cap_rows && !vt_q3a_proj_bufs(r, d, pdim)) {
        return vt_decline(r, TowerDecline.memory)
    }
    var rl & = g_vt_rl
    vt_q3a_rail(rl, r, t, r.gemm_khr)
    rail_seat(rl, TowerBuf.tail_hid, r.pj_h_dev, r.pj_cap * d * 4l, VTF_PJH)
    rail_seat(rl, TowerBuf.tail_proj, r.pj_y_dev, r.pj_cap * pdim * 4l, VTF_PJY)
    if (t.cq8) {
        rl.sites[int(RailSite.tail1)] = t1
        rl.sites[int(RailSite.tail2)] = t2
        let q8e = kq_elems(KqFmt.q8)
        vt_sched_walk(rl.maps, r.meta, 2l) $(rid : int64) : TowerSchedRec {
            let site = rid == 0l ? RailSite.tail1 : RailSite.tail2
            return TowerSchedRec(wblk0 = (rid == 0l ? t.mm1_w_off : t.mm2_w_off) / q8e, rows = npos, groups = rail_site_groups(rl, site, rid == 0l ? d : pdim, npos))
        }
    }
    upload_region_at(r.fx_dev, 0l, unsafe(addr<uint8 const?>(s.xb[0])), npos * d * 4l)
    let ny = npos * pdim
    vt_rail_one_shot(rl, "qwen3a projector", 2l, npos) $ {
        tower_q3a_proj(rl, t, npos, 0l)
        vhz_dep(rl.raw, rl.h, VTF_PJY, 0u, true)
        cmd_copy_range(rl.raw, r.pj_y_dev, 0l, r.phost.buf, 0l, ny * 4l)
        vt_pt(rl.raw, RailProfRole.copy_out)
    }
    s.out |> grow_resize(ny)   // the window's rows exactly: a shorter window's soft tokens are fewer than the last one's
    host_buf_read(r.phost, unsafe(addr<void?>(s.out[0])), ny * 4l)
    g_vt_tail_encodes++
    return true
}

//! the device projector's planes and readback at the front's row capacity; false with them released where an allocation failed
[cold_path]
def private vt_q3a_proj_bufs(var r : VtResident; d, pdim : int64) : bool {
    if (r.pj_cap > 0l) {
        release_device_buf(r.pj_h_dev)
        release_device_buf(r.pj_y_dev)
        r.pj_cap = 0l
    }
    r.oom = false
    let cap = r.cap_rows
    r.pj_h_dev = vt_dev_buf(r, cap * d * 4l)
    r.pj_y_dev = vt_dev_buf(r, cap * pdim * 4l)
    vt_proj_host(r, cap * pdim * 4l)
    if (r.oom || r.phost_bytes < cap * pdim * 4l) {
        release_device_buf(r.pj_h_dev)
        release_device_buf(r.pj_y_dev)
        return false
    }
    r.pj_cap = cap
    return true
}

[cold_path]
def private vt_q3a_scratch(var r : VtResident; t : Qwen3aTower; cap_chunks : int64) : bool {
    vt_release_scratch(r)
    r.oom = false
    let cap = cap_chunks * QWEN3A_TOK_PER_CHUNK
    let b = tower_q3a_front_bytes(t, cap_chunks)
    let bfeat = (cap + TILE_READ_SLACK) * 16l * t.n_ch * 2l
    let bx = cap * t.core.d_model * 4l
    if (!vk_range_ok(max(max(b.xin, b.col), max(max(b.y[0], b.y[1]), max(b.y[2], bfeat))))) {
        return false
    }
    r.meta_bytes = BATCH_META_BYTES
    r.meta = vt_host_buf(r, r.meta_bytes, false)
    r.cap_rows = cap
    r.front_cap = cap_chunks
    r.fxin_dev = vt_call_buf(r, b.xin)
    r.fcol_dev = vt_dev_buf(r, b.col)
    r.fy1_dev = vt_dev_buf(r, b.y[0])
    r.fy2_dev = vt_dev_buf(r, b.y[1])
    r.fy3_dev = vt_dev_buf(r, b.y[2])
    r.ffeat_dev = vt_dev_buf(r, bfeat)
    r.fx_dev = vt_call_buf(r, bx)
    r.host = vt_host_buf(r, bx, true)
    if (r.oom) {
        vt_release_scratch(r)
        return false
    }
    return true
}

//! the qwen3a front's stage seats beside `vt_q3a_rail`'s: the chunk blocks, the columns, the three stages' rows
def private vt_q3a_front_seats(var rl : VtRail; r : VtResident; t : Qwen3aTower) {
    let b = tower_q3a_front_bytes(t, r.front_cap)
    rail_seat(rl, TowerBuf.planes, r.fxin_dev, b.xin, VTF_XIN)
    rail_seat(rl, TowerBuf.col, r.fcol_dev, b.col, VTF_COL)
    rail_seat(rl, TowerBuf.stage1, r.fy1_dev, b.y[0], VTF_Y1)
    rail_seat(rl, TowerBuf.stage2, r.fy2_dev, b.y[1], VTF_Y2)
    rail_seat(rl, TowerBuf.stage3, r.fy3_dev, b.y[2], VTF_Y3)
}

[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#g4a-chains")]
def private vulkan_q3a_front(t : Qwen3aTower; win : MelChunk; n_chunks : int64; var s : EncoderState) : bool {
    var r & = g_vt_q3a_front
    if (!vt_gate(r, t.cq8 || t.cf16)) {
        return false
    }
    return vt_decline(r, TowerDecline.flavor) if (t.core.dev_backend != TowerDev.vulkan)
    let nm = t.core.n_mel
    let d = t.core.d_model
    let featw = 16l * t.n_ch
    let npos = n_chunks * QWEN3A_TOK_PER_CHUNK
    if (!tower_q3a_front_ok(t, n_chunks) || npos > VT_MAX_ENCODE_ROWS) {
        return vt_decline(r, TowerDecline.shape)
    }
    let arm = vt_arm(r, true)
    if (!arm.ok) {
        return false
    }
    let tile = vt_tile(true, d, npos)
    let lane_ok = !t.cq8 || vt_tile_ensure(tile)
    if (!vt_cm2_feed() || !f16_gemm_ensure(arm.gemm_khr) || !ensure_tower_im2col_cls() || !ensure_tower_bias_act_cls() || !ensure_tower_q3a_shuffle_cls()
            || !lane_ok) {
        return vt_decline(r, TowerDecline.device)
    }
    let lay = active_q8_repack_layout(g_active_needs_repack)
    let src = vt_dev_src(t.core)
    let span = tower_q3a_norm_span(t)
    let key = (src.on ? vt_key_dev(src, t.cblob, span.lo, span.hi - span.lo, 1l)
        : vt_key(t.qcblob, t.qcscales, t.cblob, t.cout_w_off, featw, 0l, 1l, lay.mr, lay.kgroup, lay.wbias))
    if (r.key != key && !vt_q3a_slab(r, t, key, lay)) {
        return vt_decline(r, TowerDecline.memory)
    }
    if (r.front_cap < n_chunks || r.gemm_khr != arm.gemm_khr) {
        r.gemm_khr = arm.gemm_khr
        if (!vt_q3a_scratch(r, t, max(n_chunks, QWEN3A_WINDOW / QWEN3A_CHUNK))) {
            return vt_decline(r, TowerDecline.memory)
        }
    }
    var rl & = g_vt_rl
    vt_q3a_rail(rl, r, t, arm.gemm_khr)
    vt_q3a_front_seats(rl, r, t)
    if (t.cq8) {
        rl.sites[int(RailSite.front1)] = tile
        vt_sched_walk(rl.maps, r.meta, 1l) $(_rid : int64) : TowerSchedRec {
            return TowerSchedRec(wblk0 = t.cout_w_off / kq_elems(KqFmt.q8), rows = npos, groups = rail_site_groups(rl, RailSite.front1, d, npos))
        }
    }
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
    vt_rail_one_shot(rl, "qwen3a-front", 3l, npos) $ {
        tower_q3a_front(rl, t, n_chunks, 0l)
        vhz_dep(rl.raw, rl.h, VTF_X, 0u, true)
        cmd_copy_range(rl.raw, r.fx_dev, 0l, r.host.buf, 0l, bx)
        vt_pt(rl.raw, RailProfRole.copy_out)
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
    let f16 = tower_dev_find(src.devplan, VT_G4A_FRONT16_PIECE)
    return f16.off >= 0l && vt_front_upload16(r, plane_at(src.dev, f16.off), f16.bytes, t.twiddles, btw, t.filters, bfil)
}

let private VT_G4A_PIECE = tower_family_piece(VT_HOME, "g4a")           //! gemma4a's q8 pieces
let private VT_G4A_FRONT16_PIECE = tower_front16_piece(VT_HOME, "g4a")  //! ... its subsample convs as the f16 slab

[cold_path]
def private vulkan_gemma4a_bake(t : Gemma4aEncoder; var b : TowerDevBuild) : bool {
    tower_g4a_bake(t, b, VT_HOME)
    return true
}

//! the front's f16 conv slab (`b16` bytes at `p16`), its twiddles and filters uploaded as they are; false with the buffers released on a failed allocation
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
def private vt_g4a_front_scratch(var r : VtResident; t : Gemma4aEncoder; cap : int64) : VtBuild {
    vt_front_release(r)
    r.oom = false
    let b = tower_g4a_front_bytes(t, cap)
    if (!vk_range_ok(max(max(b.frames, b.col), max(max(b.stage1, b.stage2), max(b.feat16, b.spec))))) {
        return VtBuild.shape
    }
    r.front_cap = cap
    r.fxin_dev = vt_call_buf(r, b.frames)
    r.fy3_dev = vt_dev_buf(r, b.spec)
    r.fmel_dev = vt_call_buf(r, b.mel)
    r.fcol_dev = vt_dev_buf(r, b.col)
    r.fy1_dev = vt_dev_buf(r, b.stage1)
    r.fy2_dev = vt_dev_buf(r, b.stage2)
    r.ffeat_dev = vt_dev_buf(r, b.feat16)
    r.fmeta_bytes = BATCH_META_BYTES
    r.fmeta = vt_host_buf(r, r.fmeta_bytes, false)
    if (r.oom) {
        vt_front_release(r)
        return VtBuild.memory
    }
    return VtBuild.built
}

//! the gemma4a front's rail at its scratch: every plane `tower_g4a_front` binds, its own schedule, the input projection's site over the f16 feed
def private vt_g4a_front_rail(var rl : VtRail; r : VtResident; t : Gemma4aEncoder; gemm_khr : bool) {
    let b = tower_g4a_front_bytes(t, r.front_cap)
    vt_rail_begin(rl, rail_lane_of(t.gemm_f16), true, false, gemm_khr)
    rl.ones_off = r.ones_off
    rail_seat(rl, TowerBuf.planes, r.fxin_dev, b.frames, VTF_XIN)
    rail_seat(rl, TowerBuf.twiddle, r.front_twiddles_dev, b.twiddle, 0u)
    rail_seat(rl, TowerBuf.spec, r.fy3_dev, b.spec, VTF_Y3)
    rail_seat(rl, TowerBuf.filters, r.front_filters_dev, b.filters, 0u)
    rail_seat(rl, TowerBuf.mel, r.fmel_dev, b.mel, VTF_MEL)
    rail_seat(rl, TowerBuf.col, r.fcol_dev, b.col, VTF_COL)
    rail_seat(rl, TowerBuf.stage1, r.fy1_dev, b.stage1, VTF_Y1)
    rail_seat(rl, TowerBuf.stage2, r.fy2_dev, b.stage2, VTF_Y2)
    rail_seat(rl, TowerBuf.feat16, r.ffeat_dev, b.feat16, VTF_FEAT)
    rail_seat(rl, TowerBuf.x, r.x_dev, r.cap_rows * t.d_model * 4l, VT_X)
    rail_seat(rl, TowerBuf.fw16, r.fw16_dev, r.fw16_bytes, 0u)
    rail_seat(rl, TowerBuf.norms, r.norms_dev, r.norms_bytes, 0u)
    rail_seat(rl, TowerBuf.wb, r.wb_dev, r.wb_bytes, 0u)
    rail_seat(rl, TowerBuf.wq, r.wq_dev, r.wq_bytes, 0u)
    rail_seat(rl, TowerBuf.ws, r.ws_dev, r.ws_bytes, 0u)
    rail_seat(rl, TowerBuf.meta, r.fmeta.buf, r.fmeta_bytes, 0u)
    rail_bind_f16(rl, fixed_array(RailSite.front1), TowerBuf.feat16)
}

[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#g4a-chains")]
def private vulkan_gemma4a_chunk(t : Gemma4aEncoder; var s : Gemma4aState; n_len : int64) : int64 {
    var r & = g_vt_g4a
    if (!vt_gate(r, true)) {
        return -1l
    }
    if (t.dev_backend != TowerDev.vulkan) {
        vt_decline(r, TowerDecline.flavor)
        return -1l
    }
    let d = t.d_model
    let n_fft = (t.n_bins - 1l) * 2l
    let npos = tower_g4a_front_geom(t, n_len).npos
    if (!tower_g4a_front_ok(t, n_len) || long_length(s.xw) < n_len * n_fft || npos > VT_MAX_ENCODE_ROWS) {
        vt_decline(r, TowerDecline.shape)
        return -1l
    }
    let blocks = vt_g4a_blocks_arm(r, t, npos)
    return -1l if (!blocks.ok)
    let tile = vt_tile(true, d, npos)
    let arm = vt_front_arm(r, n_len, 3008l,
        $(_gemm_khr : bool) => (ensure_tower_im2col_cls() && ensure_tower_spec_dft_cls() && ensure_tower_mel_log_cls() && ensure_tower_ln_relu_cls()
            && ensure_tower_clamp_cvt_cls() && (t.gemm_f16 || vt_tile_ensure(tile))),
        $(var rr : VtResident) => vt_g4a_planes(rr, t) && (rr.front || vt_g4a_front_slab(rr, t)),
        $(var rr : VtResident; cap : int64; _gemm_khr : bool) => vt_g4a_front_scratch(rr, t, cap))
    if (!arm.ok) {
        return -1l
    }
    var rl & = g_vt_rl
    vt_g4a_front_rail(rl, r, t, arm.gemm_khr)
    if (t.q8) {
        rl.sites[int(RailSite.front1)] = tile
        vt_sched_walk(rl.maps, r.fmeta, 1l) $(_rid : int64) : TowerSchedRec {
            return TowerSchedRec(wblk0 = t.inproj_w / kq_elems(KqFmt.q8), rows = npos, groups = rail_site_groups(rl, RailSite.front1, d, npos))
        }
    }
    upload_region_at(r.fxin_dev, 0l, unsafe(addr<uint8 const?>(s.xw[0])), n_len * n_fft * 4l)
    vt_rail_one_shot(rl, "gemma4a-front", 2l, npos) $ {
        tower_g4a_front(rl, t, n_len, 0l, TowerBuf.x)
    }
    return -1l if (!vt_g4a_blocks_run(r, t, s, npos, blocks.khr, blocks.gemm_khr))
    g_vt_convs++
    return npos
}

let private VTF_Y4 = 0x100u
let private VTF_Y5 = 0x200u
let private VTF_XF = 0x400u
let private VTF_H = 0x800u

[cold_path]
def private vt_cn_front_slab(var r : VtResident; fc : TowerFc) : bool {
    let btw = fc.device_mel ? fc.n_fft * 2l * fc.n_fb * 4l : 0l
    let bfil = fc.n_mel * fc.n_fb * 4l
    let f16 = tower_dev_find(fc.devplan, tower_fc_piece(VT_HOME, "front16"))
    return f16.off >= 0l && vt_front_upload16(r, plane_at(fc.dev, f16.off), f16.bytes, fc.twiddles, btw, fc.filters, bfil)
}

//! the front's planes on the rail at the scratch's frames: the stages, the feeds, the slabs and the front's own schedule
def private vt_cn_front_seats(var rl : VtRail; r : VtResident; fc : TowerFc) {
    let b = tower_fc_front_bytes(fc, r.front_cap)
    rail_seat(rl, TowerBuf.planes, r.fxin_dev, b.xw, VTF_XIN)
    rail_seat(rl, TowerBuf.spec, r.fy3_dev, b.pow, VTF_Y3)
    rail_seat(rl, TowerBuf.mel, r.fmel_dev, b.mel, VTF_MEL)
    rail_seat(rl, TowerBuf.col, r.fcol_dev, b.col, VTF_COL)
    rail_seat(rl, TowerBuf.stage1, r.fy1_dev, b.y0, VTF_Y1)
    rail_seat(rl, TowerBuf.stage2, r.fy2_dev, b.y2, VTF_Y2)
    rail_seat(rl, TowerBuf.stage3, r.fcn_pw3_dev, b.y2, VTF_Y4)
    rail_seat(rl, TowerBuf.stage4, r.fcn_dw5_dev, b.y3, VTF_Y5)
    rail_seat(rl, TowerBuf.stage5, r.ffeat_dev, b.y3, VTF_FEAT)
    rail_seat(rl, TowerBuf.feat, r.fcn_feat_dev, b.xf, VTF_XF)
    rail_seat(rl, TowerBuf.feat16, r.fcn_feed16_dev, b.h, VTF_H)
    rail_seat(rl, TowerBuf.rows_out, r.fx_dev, b.x, VTF_X)
    rail_seat(rl, TowerBuf.fw16, r.fw16_dev, r.fw16_bytes, 0u)
    rail_seat(rl, TowerBuf.twiddle, r.front_twiddles_dev, max(b.tw, 64l), 0u)
    rail_seat(rl, TowerBuf.filters, r.front_filters_dev, b.fil, 0u)
    rail_seat(rl, TowerBuf.norms, r.norms_dev, r.norms_bytes, 0u)
    rail_seat(rl, TowerBuf.wq, r.wq_dev, r.wq_bytes, 0u)
    rail_seat(rl, TowerBuf.ws, r.ws_dev, r.ws_bytes, 0u)
    rail_seat(rl, TowerBuf.wb, r.wq_dev, r.wq_bytes, 0u) if (fc.f16w)   // the exact lane's halfwords sit in the q8 plane's seat (`vt_upload_f16`)
    rail_seat(rl, TowerBuf.meta, r.fmeta.buf, r.fmeta_bytes, 0u)
    vt_fc_kq_member_seats(rl, r, fc)
}

[cold_path, unused_argument(gemm_khr)]
def private vt_cn_front_scratch(var r : VtResident; fc : TowerFc; cap : int64; gemm_khr : bool) : VtBuild {
    vt_front_release(r)
    r.oom = false
    let b = tower_fc_front_bytes(fc, cap)
    let bxw = b.xw
    let bpow = b.pow
    let bmel = b.mel
    let bcol = b.col
    let by0 = b.y0
    let by2 = b.y2
    let by3 = b.y3
    let bxf = b.xf
    let bh = b.h
    let bx = b.x
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
    r.fx_dev = vt_call_buf(r, bx)
    r.fhost = vt_host_buf(r, bx, true)
    r.fmeta_bytes = BATCH_META_BYTES
    r.fmeta = vt_host_buf(r, r.fmeta_bytes, false)
    if (r.oom) {
        vt_front_release(r)
        return VtBuild.memory
    }
    return VtBuild.built
}

[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#fc-chains")]
def private vt_fc_front(var r : VtResident; fc : TowerFc; src : float const?; src_elems, n_len, n_valid : int64; var x : array<float>;
                        blocks_behind : bool) : int64 {
    let n_mel = fc.n_mel
    let n_fb = fc.n_fb
    let n_fft = fc.n_fft
    let nch = fc.nch
    let d = fc.d
    let g = tower_fc_geom(n_len, n_mel)
    let rows2 = g.t2 * g.w2
    let rows3 = g.t3 * g.w3
    let tt = g.t3
    let feat = g.w3 * nch
    if (n_len < 1l || nch % 64l != 0l || feat % 64l != 0l || d % 64l != 0l || n_fb != n_fft / 2l + 1l || src_elems < n_len * (fc.device_mel ? n_fft : n_mel)) {
        vt_decline(r, TowerDecline.shape)
        return -1l
    }
    if (!vulkan_tower_arms()) {   // the range check below reads the device's limits: an image loaded by its path reaches here with no device yet
        vt_decline(r, TowerDecline.device)
        return -1l
    }
    if (!vt_cn_rows_ok(tt, d, fc.ff)) {
        vt_decline(r, TowerDecline.shape)
        return -1l
    }
    let tc2 = cm2_tile_cols(nch, rows2)
    let tc3 = cm2_tile_cols(nch, rows3)
    let tcx = cm2_tile_cols(d, tt)
    let arm = vt_front_arm(r, n_len, 4096l,
        $(_gemm_khr : bool) => (ensure_tower_im2col_cls() && ensure_tower_spec_dft_cls() && ensure_tower_mel_log_cls() && ensure_tower_mel_norm_cls()
            && ensure_tower_dw_conv_cls() && ensure_tower_pk_dw2d_cls() && ensure_tower_feat_shuffle_cls() && ensure_tower_bias_act_cls()
            && ensure_tower_clamp_cvt_cls()
            && (fc.f16w || vt_fc_front_tiles_ok(fc, fixed_array(tc2, tc3, tcx)))),
        $(var rr : VtResident) => vt_fc_planes(rr, fc) && (rr.front || vt_cn_front_slab(rr, fc)),
        $(var rr : VtResident; cap : int64; gemm_khr : bool) => vt_cn_front_scratch(rr, fc, cap, gemm_khr))
    if (!arm.ok || (blocks_behind && !vt_fc_admits(fc, tt, 2l * tt - 1l, arm.khr, arm.gemm_khr))) {
        return -1l
    }
    r.d = d
    var rl & = g_vt_rl
    vt_rail_begin(rl, rail_lane_of(fc.f16w), true, arm.khr, arm.gemm_khr)
    vt_cn_front_seats(rl, r, fc)
    tower_fc_front_binds(rl)
    if (!fc.f16w) {
        rl.sites[int(RailSite.front1)] = VtTile(tc = tc2)
        rl.sites[int(RailSite.front2)] = VtTile(tc = tc3)
        rl.sites[int(RailSite.front3)] = VtTile(tc = tcx)
        tower_sched_walk(rl.maps, unsafe(reinterpret<uint?>(r.fmeta.mapped)), FC_FRONT_RECORDS) $(rid : int64) : TowerSchedRec {
            return tower_fc_front_rec(rl, fc, r.kq_seats, n_len, rid)
        }
    }
    if (fc.device_mel) {
        upload_region_at(r.fxin_dev, 0l, unsafe(reinterpret<uint8 const?>(src)), n_len * n_fft * 4l)
    } else {
        upload_region_at(r.fmel_dev, 0l, unsafe(reinterpret<uint8 const?>(src)), n_len * n_mel * 4l)
    }
    let bx = tt * d * 4l
    vt_rail_one_shot(rl, fc.front_name, 3l, tt) $ {
        tower_fc_front(rl, fc, r.kq_seats, n_len, n_valid)
        vhz_dep(rl.raw, rl.h, VTF_X, 0u, true)
        cmd_copy_range(rl.raw, r.fx_dev, 0l, r.fhost.buf, 0l, bx)
        vt_pt(rl.raw, RailProfRole.copy_out)
    }
    ensure_length(x, tt * d)
    host_buf_read(r.fhost, unsafe(addr<void?>(x[0])), bx)
    return tt
}

[hot_path]
def private vulkan_canary_front(t : CanaryEncoder; var s : CanaryState) : int64 {
    var r & = g_vt_cn
    if (!vt_gate(r, true)) {
        return -1l
    }
    if (t.dev_backend != TowerDev.vulkan) {
        vt_decline(r, TowerDecline.flavor)
        return -1l
    }
    let fc = tower_fc_of_cn(t, g_vt_fc_offs)
    let tt = vt_fc_front(r, fc, unsafe(addr<float const?>(s.xw[0])), long_length(s.xw), s.n_len, s.n_valid, s.x, false)
    if (tt >= 0l) {
        g_vt_convs++
    }
    return tt
}

//! the rotary family's front slab off the Vulkan image: both stride-2 conv panels as one f16 slab, the mel's DFT twiddles with the window behind
[cold_path]
def private vt_ga_front_slab(var r : VtResident; m : GigaamModel) : bool {
    let f16 = tower_dev_find(m.devplan, tower_fc_piece(VT_HOME, "front16"))
    let twin = tower_dev_find(m.devplan, tower_ga_twin_piece(VT_HOME))
    return false if (f16.off < 0l || twin.off < 0l)
    return vt_front_upload16(r, plane_at(m.dev, f16.off), f16.bytes, PlaneF(p = unsafe(reinterpret<float const?>(plane_at(m.dev, twin.off))),
        n = twin.bytes / 4l), twin.bytes, m.filters, m.n_mel * (m.n_fft / 2l + 1l) * 4l)
}

//! the rotary family's front scratch at `cap` mel frames (`tower_ga_front_bytes`) and the residual rows' readback
[cold_path, unused_argument(gemm_khr)]
def private vt_ga_front_scratch(var r : VtResident; m : GigaamModel; cap : int64; gemm_khr : bool) : VtBuild {
    vt_front_release(r)
    r.oom = false
    let b = tower_ga_front_bytes(m, cap)
    let bmel = b.mel
    let bcol = b.col
    let by1 = b.y1
    let bx = b.x
    let bsamp = b.samples
    let bxw = b.frames
    let bpow = b.pow
    if (!vk_range_ok(max(max(max(bmel, bcol), max(by1, bx)), max(bsamp, max(bxw, bpow))))) {
        return VtBuild.shape
    }
    r.front_cap = cap
    r.fmel_dev = vt_call_buf(r, bmel)
    r.fcol_dev = vt_dev_buf(r, bcol)
    r.fy1_dev = vt_dev_buf(r, by1)
    r.fx_dev = vt_call_buf(r, bx)
    r.fxin_dev = vt_call_buf(r, bsamp)  //! the device mel's raw samples, its windowed frames and its power rows
    r.fy2_dev = vt_dev_buf(r, bxw)
    r.fy3_dev = vt_dev_buf(r, bpow)
    r.fhost = vt_host_buf(r, bx, true)
    if (r.oom) {
        vt_front_release(r)
        return VtBuild.memory
    }
    return VtBuild.built
}

//! the rotary family's front planes on the rail at the scratch's frames: samples, frames, power and mel rows, the column panel, conv0's rows, rows out, slabs
def private vt_ga_front_seats(var rl : VtRail; r : VtResident; m : GigaamModel) {
    let b = tower_ga_front_bytes(m, r.front_cap)
    rail_seat(rl, TowerBuf.planes, r.fxin_dev, b.samples, VTF_XIN)
    rail_seat(rl, TowerBuf.frames, r.fy2_dev, b.frames, VTF_Y2)
    rail_seat(rl, TowerBuf.spec, r.fy3_dev, b.pow, VTF_Y3)
    rail_seat(rl, TowerBuf.mel, r.fmel_dev, b.mel, VTF_MEL)
    rail_seat(rl, TowerBuf.col, r.fcol_dev, b.col, VTF_COL)
    rail_seat(rl, TowerBuf.stage1, r.fy1_dev, b.y1, VTF_Y1)
    rail_seat(rl, TowerBuf.rows_out, r.fx_dev, b.x, VTF_X)
    rail_seat(rl, TowerBuf.fw16, r.fw16_dev, r.fw16_bytes, 0u)
    rail_seat(rl, TowerBuf.twiddle, r.front_twiddles_dev, b.tw, 0u)
    rail_seat(rl, TowerBuf.filters, r.front_filters_dev, b.fil, 0u)
    rail_seat(rl, TowerBuf.norms, r.norms_dev, r.norms_bytes, 0u)
}

//! the gigaam resident armed for `n_len` frames: the device, the front's classes, the q8 planes, the front slab and the scratch;
//! ok false with the decline counted
def private vt_ga_front_arm(var r : VtResident; fc : TowerFc; m : GigaamModel; n_len : int64) : tuple<ok : bool; khr : bool; gemm_khr : bool> {
    return vt_front_arm(r, n_len, GIGAAM_MAX_CLIP_S * AUDIO_SAMPLE_RATE / max(m.hop, 1l),
        $(_gemm_khr : bool) => (ensure_tts_im2col16_cls() && ensure_tower_bias_act_cls() && ensure_tower_ga_frames_cls()
            && ensure_tower_spec_dft_cls() && ensure_tower_mel_log_cls()),
        $(var rr : VtResident) => vt_fc_planes(rr, fc) && (rr.front || vt_ga_front_slab(rr, m)),
        $(var rr : VtResident; cap : int64; gemm_khr : bool) => vt_ga_front_scratch(rr, m, cap, gemm_khr))
}

//! The gigaam log-mel of `samples` on the device alone, read back frame-major into `mel` - the encode chain's mel record in a
//! one-shot command of its own, the twin's seat; the frame count, or -1 with the decline counted.
def vulkan_gigaam_mel(m : GigaamModel; samples : array<float>; @exact_size var mel : array<float>) : int64 {
    var r & = g_vt_ga
    if (!vt_gate(r, gigaam_gpu_lane_planes(m))) {
        return -1l
    }
    if (m.dev_backend != TowerDev.vulkan) {
        vt_decline(r, TowerDecline.flavor)
        return -1l
    }
    let n_len = gigaam_mel_frames(m, long_length(samples))
    if (n_len < 1l || m.hop < 1l) {
        vt_decline(r, TowerDecline.shape)
        return -1l
    }
    let fc = tower_fc_of_ga(m, VT_HOME, g_vt_fc_offs, g_vt_fc_gseats)
    let arm = vt_ga_front_arm(r, fc, m, n_len)
    return -1l if (!arm.ok)
    upload_region_at(r.fxin_dev, 0l, unsafe(addr<uint8 const?>(samples[0])), ((n_len - 1l) * m.hop + m.n_fft) * 4l)
    let bmel = n_len * m.n_mel * 4l
    var rl & = g_vt_rl
    vt_rail_begin(rl, RailLane.q8, true, arm.khr, arm.gemm_khr)
    vt_ga_front_seats(rl, r, m)
    vt_rail_one_shot(rl, "gigaam-mel", 1l, n_len) $ {
        tower_ga_mel(rl, m, n_len)
        vhz_dep(rl.raw, rl.h, VTF_MEL, 0u, true)
        cmd_copy_range(rl.raw, r.fmel_dev, 0l, r.fhost.buf, 0l, bmel)
        vt_pt(rl.raw, RailProfRole.copy_out)
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
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#fc-chains")]
def private vt_ga_front(var r : VtResident; fc : TowerFc; m : GigaamModel; var st : GigaamState; samples : float const?; n_in : int64) : int64 {
    let k = m.subs_kernel
    let nch = m.n_sub_ch
    let d = m.d_model
    let n_len = st.n_len
    let t1 = gigaam_conv_out(n_len, k)
    let tt = gigaam_conv_out(t1, k)
    let dev_mel = samples != null
    let n_use = (n_len - 1l) * m.hop + m.n_fft   //! the samples the frames reach
    if (n_len < 1l || t1 < 1l || tt < 1l || k % 2l != 1l || nch % 64l != 0l || d % 64l != 0l
            || (dev_mel ? (m.hop < 1l || n_use > n_in) : long_length(st.mel) < n_len * m.n_mel)) {
        vt_decline(r, TowerDecline.shape)
        return -1l
    }
    let arm = vt_ga_front_arm(r, fc, m, n_len)
    return -1l if (!arm.ok || !vt_fc_admits(fc, tt, 0l, arm.khr, arm.gemm_khr))
    if (dev_mel) {
        upload_region_at(r.fxin_dev, 0l, unsafe(reinterpret<uint8 const?>(samples)), n_use * 4l)
    } else {
        upload_region_at(r.fmel_dev, 0l, unsafe(addr<uint8 const?>(st.mel[0])), n_len * m.n_mel * 4l)
    }
    let bx = tt * d * 4l
    var rl & = g_vt_rl
    vt_rail_begin(rl, RailLane.q8, true, arm.khr, arm.gemm_khr)
    vt_ga_front_seats(rl, r, m)
    vt_rail_one_shot(rl, fc.front_name, 2l, tt) $ {
        tower_ga_front(rl, m, n_len, dev_mel)
        vhz_dep(rl.raw, rl.h, VTF_X, 0u, true)
        cmd_copy_range(rl.raw, r.fx_dev, 0l, r.fhost.buf, 0l, bx)
        vt_pt(rl.raw, RailProfRole.copy_out)
    }
    ensure_length(st.x, tt * d)
    host_buf_read(r.fhost, unsafe(addr<void?>(st.x[0])), bx)
    return tt
}

var private g_vt_env_ga_exact = false

//! Prefer GigaAM's exact lane - its block GEMMs on the f16 GEMM over the carrier's halfwords - over the q8 image for the next
//! un-pinned loads (`DASLLAMA_VK_GIGAAM_EXACT` is the env twin); both lanes serve on the device, the exact one on the f16 GEMM feed alone. Answers the setting
//! it replaced.
def set_vulkan_gigaam_exact_lane(on : bool) : bool {
    let was = g_vt_env_ga_exact
    g_vt_env_ga_exact = on
    static_if (!typeinfo builtin_module_exists(das_metal)) {
        register_gigaam_gpu(@@vulkan_tower_on, on)
    }
    return was
}

//! the GigaAM encode whole: the conv front off the device mel of the raw samples (the CPU mel where `samples` is null), the rope
//! tables, then the blocks over the front's rows; -1 with nothing counted where the front cannot serve or where the blocks
//! declined behind a served front
[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_FC.md#vk-tower-fc-rotary")]
def private vulkan_gigaam_encode(m : GigaamModel; var st : GigaamState; samples : float const?; n_in : int64) : int64 {
    var r & = g_vt_ga
    g_vt_fc_rows.bytes = 0l
    if (!g_vt_env_tower) {
        vt_decline(r, TowerDecline.knob)
        return -1l
    }
    if (!gigaam_gpu_lane_planes(m)) {
        vt_decline(r, TowerDecline.quant_mode)
        return -1l
    }
    if (m.dev_backend != TowerDev.vulkan) {
        vt_decline(r, TowerDecline.flavor)
        return -1l
    }
    let fc = tower_fc_of_ga(m, VT_HOME, g_vt_fc_offs, g_vt_fc_gseats)
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

var private g_vt_q3am : VtResident   //! the device mel's resident: the frame scratch at `cap_rows` frames for an `ff`-tap window into `d` mel rows

//! the twiddle and filter panels are per-call uploads: the CPU builds them per call too
[cold_path]
def private vt_q3a_mel_scratch(var r : VtResident; cap, n_fft, n_bins, n_mel : int64) : bool {
    vt_release_scratch(r)
    r.oom = false
    let bxw = cap * n_fft * 4l
    let bpow = cap * n_bins * 4l
    let bout = n_mel * cap * 4l
    if (!vk_range_ok(max(bxw, max(bpow, bout)))) {
        return false
    }
    r.cap_rows = cap
    r.ff = n_fft
    r.d = n_mel
    r.x_dev = vt_call_buf(r, bxw)
    r.xb_dev = vt_call_buf(r, n_fft * 2l * n_bins * 4l)
    r.xq_dev = vt_call_buf(r, n_mel * n_bins * 4l)
    r.q_dev = vt_dev_buf(r, bpow)
    r.yo_dev = vt_dev_buf(r, bout)
    r.host = vt_host_buf(r, bout, true)
    if (r.oom) {
        vt_release_scratch(r)
        return false
    }
    return true
}

//! the device mel's rail seats at its scratch: the frames, the twiddles, the filters, the spectrum, the mel rows
def private vt_mel_seats(var rl : VtRail; r : VtResident) {
    let n_bins = r.ff / 2l + 1l
    rail_seat(rl, TowerBuf.planes, r.x_dev, r.cap_rows * r.ff * 4l, VT_X)
    rail_seat(rl, TowerBuf.twiddle, r.xb_dev, r.ff * 2l * n_bins * 4l, VT_XB)
    rail_seat(rl, TowerBuf.filters, r.xq_dev, r.d * n_bins * 4l, VT_XQ)
    rail_seat(rl, TowerBuf.spec, r.q_dev, r.cap_rows * n_bins * 4l, VT_Q)
    rail_seat(rl, TowerBuf.mel, r.yo_dev, r.d * r.cap_rows * 4l, VT_YO)
}

[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#g4a-chains")]
def private vulkan_q3a_mel(xw : array<float>; n_len, n_fft, n_bins, n_mel : int64; twiddles, filters : array<float>; var m : WhisperMel) : bool {
    var r & = g_vt_q3am
    if (!g_vt_env_tower) {
        return vt_decline(r, TowerDecline.knob)
    }
    if (n_len < 1l || n_bins != n_fft / 2l + 1l || long_length(xw) < n_len * n_fft || long_length(twiddles) < n_fft * 2l * n_bins
            || long_length(filters) < n_mel * n_bins || long_length(m.data) < n_mel * n_len) {
        return vt_decline(r, TowerDecline.shape)
    }
    if (!vt_arm(r).ok) {
        return false
    }
    if (!ensure_tower_spec_dft_cls() || !ensure_tower_mel_log_cls()) {
        return vt_decline(r, TowerDecline.device)
    }
    if ((r.cap_rows < n_len || r.ff != n_fft || r.d != n_mel) && !vt_q3a_mel_scratch(r, max(n_len, 4096l), n_fft, n_bins, n_mel)) {
        return vt_decline(r, TowerDecline.memory)
    }
    upload_region_at(r.x_dev, 0l, unsafe(addr<uint8 const?>(xw[0])), n_len * n_fft * 4l)
    upload_region_at(r.xb_dev, 0l, unsafe(addr<uint8 const?>(twiddles[0])), n_fft * 2l * n_bins * 4l)
    upload_region_at(r.xq_dev, 0l, unsafe(addr<uint8 const?>(filters[0])), n_mel * n_bins * 4l)
    let bout = n_mel * n_len * 4l
    var rl & = g_vt_rl
    vt_rail_begin(rl, RailLane.q8, false, false, false)
    vt_mel_seats(rl, r)
    vt_rail_one_shot(rl, "qwen3a-mel", 1l, n_len) $ {
        tower_mel(rl, n_len, tower_whisper_mel_spec(n_fft, n_mel))
        vhz_dep(rl.raw, rl.h, VT_YO, 0u, true)
        cmd_copy_range(rl.raw, r.yo_dev, 0l, r.host.buf, 0l, bout)
        vt_pt(rl.raw, RailProfRole.copy_out)
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
    let bx = (GK_VAD_CONTEXT + cap * GK_VAD_CHUNK) * 4l
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
    return VtBuild.built
}

//! the VAD's rail seats at its scratch: the context and chunks, the weight slab, the input gates, the [h | c] state, the probabilities
def private vt_vad_seats(var rl : VtRail; r : VtResident) {
    let cap = r.cap_rows
    rail_seat(rl, TowerBuf.planes, r.x_dev, (GK_VAD_CONTEXT + cap * GK_VAD_CHUNK) * 4l, VT_X)
    rail_seat(rl, TowerBuf.norms, r.wq_dev, r.wq_bytes, 0u)
    rail_seat(rl, TowerBuf.vad_gates, r.q_dev, cap * 4l * GK_VAD_HIDDEN * 4l, VT_Q)
    rail_seat(rl, TowerBuf.vad_state, r.k_dev, 2l * GK_VAD_HIDDEN * 4l, VT_K)
    rail_seat(rl, TowerBuf.vad_probs, r.yo_dev, cap * 4l, VT_YO)
}

//! the n samples at `src` as ceil(n / 512) chunks behind the stream's 64-sample context, one command: the encoder a workgroup
//! a chunk, the recurrence walking them all; the probabilities and the stream's h and c read back once
[hot_path, arch(at = "../ARCHITECTURE_MEDIA.md#tower-gpu-hook")]
def private vulkan_vad_chunks(m : VadModel; var st : VadState; src : float const?; n : int64; var probs : array<float>) : bool {
    var r & = g_vt_vad
    if (!g_vt_env_tower) {
        return vt_decline(r, TowerDecline.knob)
    }
    if (!gk_vad_shape_ok(m, st, n)) {
        return vt_decline(r, TowerDecline.shape)
    }
    if (!vt_arm(r).ok) {
        return false
    }
    if (!ensure_tower_vad_enc_cls() || !ensure_tower_vad_lstm_cls()) {
        return vt_decline(r, TowerDecline.device)
    }
    let nch = (n + GK_VAD_CHUNK - 1l) / GK_VAD_CHUNK
    if (!vt_built(r, vt_vad_upload(r, m)) || (r.cap_rows < nch && !vt_built(r, vt_vad_scratch(r, max(nch, 64l))))) {
        return false
    }
    let nx = GK_VAD_CONTEXT + nch * GK_VAD_CHUNK
    ensure_length(g_vt_vad_x, nx)
    gk_vad_stage(unsafe(addr(g_vt_vad_x[0])), st, src, n, nch)
    let bst = 2l * GK_VAD_HIDDEN * 4l
    let bp = nch * 4l
    upload_region_at(r.x_dev, 0l, unsafe(addr<uint8 const?>(g_vt_vad_x[0])), nx * 4l)
    upload_region_at(r.k_dev, 0l, unsafe(addr<uint8 const?>(st.h[0])), GK_VAD_HIDDEN * 4l)
    upload_region_at(r.k_dev, GK_VAD_HIDDEN * 4l, unsafe(addr<uint8 const?>(st.c[0])), GK_VAD_HIDDEN * 4l)
    var pa = gk_vad_args(m.slab_offs, nch)
    var rl & = g_vt_rl
    vt_rail_begin(rl, RailLane.q8, false, false, false)
    vt_vad_seats(rl, r)
    vt_rail_one_shot(rl, "silero-vad", 1l, nch) $ {
        tower_vad_chunks(rl, pa)
        vhz_dep(rl.raw, rl.h, VT_K | VT_YO, 0u, true)
        cmd_copy_range(rl.raw, r.k_dev, 0l, r.host.buf, 0l, bst)
        cmd_copy_range(rl.raw, r.yo_dev, 0l, r.host.buf, bst, bp)
        vt_pt(rl.raw, RailProfRole.copy_out)
    }
    host_buf_read(r.host, unsafe(addr<void?>(st.h[0])), GK_VAD_HIDDEN * 4l)
    host_buf_read(r.host, unsafe(addr<void?>(st.c[0])), GK_VAD_HIDDEN * 4l, GK_VAD_HIDDEN * 4l)
    let p0 = length(probs)
    probs |> resize(p0 + int(nch))   // nolint:PERF026 — the caller's probability row grows by the call's chunks: the seat's one output
    host_buf_read(r.host, unsafe(addr<void?>(probs[p0])), bp, bst)
    for (i in range64(GK_VAD_CONTEXT)) {
        st.context[i] = g_vt_vad_x[nch * GK_VAD_CHUNK + i]
    }
    vt_announce(r)
    g_vt_vad_calls++
    g_vt_vad_chunks += nch
    return true
}

//! the Vulkan home's vision seats - the chain hooks and the q8 flavors - at [init] unless the CUDA home is asked for, or when `set_gpu_home("vulkan")` moves the tier here
def vulkan_tower_install_vision {
    static_if (!typeinfo builtin_module_exists(das_metal)) {
        register_gemma4v_gpu(@@vulkan_gemma4v_chain, @@vulkan_tower_on, false)
        register_gemma4uv_gpu(@@vulkan_gemma4uv_encode)
        register_gemma3v_gpu(@@vulkan_gemma3v_chain, @@vulkan_tower_on, false)
        register_qwen3v_gpu(@@vulkan_qwen3v_chain, @@vulkan_tower_on, false)
        register_qwen25v_gpu(@@vulkan_qwen25v_chain)
        register_gemma3v_flavor(TowerDev.vulkan, @@vulkan_vision_serves_model, @@vulkan_gemma3v_bake)
        register_qwen3v_flavor(TowerDev.vulkan, @@vulkan_vision_serves_model, @@vulkan_qwen3v_bake)
        register_gemma4v_flavor(TowerDev.vulkan, @@vulkan_vision_serves_model, @@vulkan_gemma4v_bake)
    }
}

//! the Vulkan home's tower seats of family `fam` (hooks and q8 flavor): run for every family here, and by the CUDA home for a row naming this home
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#rail-audio-installs")]
def vulkan_tower_install_audio_of(fam : TowerAudioFamily) {
    static_if (!typeinfo builtin_module_exists(das_metal)) {
        if (fam == TowerAudioFamily.whisper) {
            register_tower_blocks_gpu(@@vulkan_audio_tower_blocks)
            register_tower_blocks_ln_post_gpu(@@vulkan_audio_tower_blocks_ln_post)
            register_tower_blocks_tail_gpu(@@vulkan_audio_tower_blocks_tail)
            register_tower_conv_gpu(@@vulkan_audio_conv_front)
            register_whisper_mel_gpu(@@vulkan_q3a_mel)
            register_audio_tower_flavor(TowerDev.vulkan, @@vulkan_aud_serves_model, @@vulkan_audio_tower_bake)
        } elif (fam == TowerAudioFamily.qwen3a) {
            register_qwen3a_front_gpu(@@vulkan_q3a_front)
            register_qwen3a_proj_gpu(@@vulkan_q3a_proj)
            register_qwen3a_flavor(TowerDev.vulkan, @@vulkan_q3a_serves_model, @@vulkan_qwen3a_bake)
        } elif (fam == TowerAudioFamily.gemma4a) {
            register_gemma4a_chunk_gpu(@@vulkan_gemma4a_chunk)
            register_gemma4a_flavor(TowerDev.vulkan, @@vulkan_gemma4a_serves_model, @@vulkan_gemma4a_bake)
        } elif (fam == TowerAudioFamily.canary) {
            register_canary_gpu(@@vulkan_canary_blocks)
            register_canary_front_gpu(@@vulkan_canary_front)
            register_canary_flavor(TowerDev.vulkan, @@vulkan_fc_serves_model, @@vulkan_canary_bake)
        } elif (fam == TowerAudioFamily.parakeet) {
            register_parakeet_gpu_encode(@@vulkan_parakeet_encode)
            register_parakeet_flavor(TowerDev.vulkan, @@vulkan_parakeet_serves_model, @@vulkan_parakeet_bake)
        } elif (fam == TowerAudioFamily.gigaam) {
            set_vulkan_gigaam_exact_lane(g_env_vulkan.vk_gigaam_exact)
            register_gigaam_gpu_encode(@@vulkan_gigaam_encode)
            register_gigaam_flavor(TowerDev.vulkan, @@vulkan_gigaam_serves_model, @@vulkan_gigaam_bake)
        } elif (fam == TowerAudioFamily.transducer) {
            register_tower_rows_gen(@@vulkan_tower_fc_gen)
        } elif (fam == TowerAudioFamily.vad) {
            register_vad_gpu(@@vulkan_vad_chunks)
        }
    }
}

//! the Vulkan home's audio seats at [init] unless the CUDA home is asked for, or when `set_gpu_home("vulkan")` moves the tier here
def vulkan_tower_install_audio {
    for (fam in each(type<TowerAudioFamily>)) {
        vulkan_tower_install_audio_of(fam)
    }
}

[init]
def dasllama_vulkan_tower_register {
    g_vt_env_tower = g_env_vulkan.vk_tower
    vt_each_resident() $(var r : VtResident; family : string) {
        r.family = family
    }
    register_vk_drop_hook(@@vt_forget)
    static_if (!typeinfo builtin_module_exists(das_metal)) {
        register_gpu_home("vulkan", @@vulkan_tower_install_vision)
        register_gpu_home("vulkan", @@vulkan_tower_install_audio)
        if (!gpu_want_cuda()) {
            vulkan_tower_install_vision()
            vulkan_tower_install_audio()
        }
    }
}
