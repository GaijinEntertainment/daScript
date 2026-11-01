options gen2
options indenting = 4
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_vulkan_asr_dec shared public

require dasllama/dasllama_lint public
require vulkan
require math
require dasllama/dasllama_env
require dasllama/dasllama_kqformat
require dasllama/dasllama_math
require dasllama/dasllama_layout
require dasllama/dasllama_plane
require dasllama/dasllama_whisper
require dasllama/dasllama_rnnt
require dasllama/dasllama_gigaam   // the CTC head's device seat
require dasllama/dasllama_gpu_kernels_common   // GkRnntArgs and the transducer stamps' staged widths
require dasllama/dasllama_vulkan_common
require dasllama/dasllama_vulkan_classes
require dasllama/dasllama_vulkan_tower
require dasllama/dasllama_tower

//! The whisper ASR-decoder Vulkan driver, q8-native: the per-window cross-KV and the decode step
//! (`ARCHITECTURE_GPU_TOWER_VULKAN.md`, the Vulkan ASR-decoder section); the Metal ASR-decoder driver fills the same hooks on an Apple build.

var private g_wd_env = true
var private g_wd_declines : VkDeclineCounter
var private g_wd_windows = 0l
var private g_wd_handoffs = 0l   //! cross-KV windows whose encoder rows came device to device off the tower's plane
var private g_wd_steps = 0l
var private g_wd_tokens = 0l
var private g_wd_us_gpu = 0l
var private g_wd_us_encode = 0l

enum private VulkanWdecDecline {
    knob         //! DASLLAMA_VK_WDEC=0, or set_vulkan_wdec(false)
    quant_mode   //! the decoder is the f32 oracle rail, not the q8 image the driver reads
    shape        //! a width off the tile's lattice, or a head width other than the family's 64
    device       //! the tier has no device, or a class failed to build
    memory       //! a device or pinned allocation failed
    rows         //! the window's first batch is wider than the step's row cap: the CPU chain serves the window
    flavor       //! the model is not the Vulkan-flavored image: each backend serves its own (`#tower-dev-block`)
}

let WD_ROW_CAP = 8l          //! the rows a decode batch may carry: the N-column GEMV's widest form
let private WD_LAYER_SITES = 6l             //! a layer's GEMV sites: qkv (three regions), o, cq, co, fc1, fc2
let private WD_S_QKV = 0l
let private WD_S_O = 1l
let private WD_S_CQ = 2l
let private WD_S_CO = 3l
let private WD_S_FC1 = 4l
let private WD_S_FC2 = 5l

//! the hazard bits of the driver's buffers
let private WD_X = 0x1u
let private WD_XQ = 0x4u
let private WD_QKV = 0x8u
let private WD_CQ = 0x10u
let private WD_YO = 0x40u
let private WD_H = 0x80u
let private WD_PARTIALS = 0x100u
let private WD_KC = 0x200u
let private WD_VC = 0x400u
let private WD_KXR = 0x800u
let private WD_VXR = 0x1000u
let private WD_LOGITS = 0x2000u
let private WD_XIN = 0x4000u
let private WD_XFEED = 0x8000u
let private WD_TMP = 0x10000u
let private WD_KX = 0x20000u
let private WD_VX = 0x40000u

struct private WdResident {
    key : uint64
    d : int64
    hh : int64
    hs : int64
    ta : int64
    tmax : int64
    nl : int64
    nvocab : int64
    cm2 : bool
    xkv_tile : VtTile
    wq_dev : uint64
    ws_dev : uint64
    wq_bytes : int64
    ws_bytes : int64
    norms_dev : uint64    //! the layers' f32 rows (norms, biases) then the final norm's weight and bias
    norms_bytes : int64
    norms_base : int64    //! the element the norms plane starts at in dblob
    final_ln_w_off : int64         //! the final norm rows' offsets in the plane
    final_ln_b_off : int64
    kxr_dev : uint64
    vxr_dev : uint64
    kc_dev : uint64
    vc_dev : uint64
    x_dev : uint64        //! the step's rows: the residual stream, its Q8_0 feed (the row passes quantize their output in place of storing it)
    xq_dev : uint64
    xs_dev : uint64
    qkv_dev : uint64
    cq_dev : uint64
    yo_dev : uint64
    h_dev : uint64
    pms_dev : uint64
    py_dev : uint64
    logits_dev : uint64      //! [nvocab]: the last row's logits
    xin_dev : uint64      //! the cross-KV chain: the encoder rows, their feed, the projection output, the CPU-layout planes
    xh_dev : uint64
    xkv_q_dev : uint64
    xkv_s_dev : uint64
    xkv_proj_dev : uint64
    ta_tile_rows : int64           //! the cross-KV GEMM record's rows: `ta` rounded up to the tile's token column (`vt_tile_rows`); the projection plane holds them
    kx_dev : uint64
    vx_dev : uint64
    xkv_meta : HostBuf
    xkv_meta_bytes : int64
    xhost : HostBuf       //! the step's rows in, the logits out
    kvhost : HostBuf
    @scratch @exact_size site_meta : array<HostBuf>   //! a GEMV site's region pairs: [l x WD_LAYER_SITES], the tied embedding last
    @scratch @exact_size s_site : array<VkDescriptorSet>
    s_kv_k : VkDescriptorSet
    s_kv_v : VkDescriptorSet
    s_attn_self : VkDescriptorSet
    s_attn_cross : VkDescriptorSet
    s_xfeed : VkDescriptorSet
    s_xkv : VkDescriptorSet
    s_ln_rq : VkDescriptorSet       //! the decode step's fused row passes: each lands its Q8_0 feed (xq, xs) in the same dispatch
    s_post_rq : VkDescriptorSet
    s_bias_h_rq : VkDescriptorSet
    s_comb_rq : VkDescriptorSet
    s_kvx_k : VkDescriptorSet
    s_kvx_v : VkDescriptorSet
    s_kvx_k16 : VkDescriptorSet   //! the resident f16 twins of the cross stores
    s_kvx_v16 : VkDescriptorSet
    oom : bool
    served_said : bool
    handoff_miss_said : bool   //! the host-upload arm of the cross-KV chain said once: the tower landed no device plane for this window's rows
}

var private g_wd : WdResident
var private g_wd_window_uid = 0ul     //! the live window: ONE DecoderState at a time, claimed by a served cross-KV
var private g_wd_window_live = false
var private g_wd_pend_uid = 0ul       //! the state whose cross-KV CPU layouts wait on the device, `g_wd_pend_bytes` a plane's bytes (0 = none)
var private g_wd_pend_bytes = 0l
var private g_wd_dropped : table<uint64>   //! the states whose waiting layouts were lost unread (another state's window, a release, a model drop); a state leaves it at its next window
var private g_wd_uid_next = 1ul
var private @scratch @exact_size g_wd_maps : array<uint>

//! the driver's identity for a decoder state, minted at its first window: an address is not identity - a state freed and
//! re-created at the same address inherits no window, no pending layouts and no mark
def private wd_uid(var ds : DecoderState) : uint64 {
    if (ds.gpu_uid == 0ul) {
        ds.gpu_uid = g_wd_uid_next++
    }
    return ds.gpu_uid
}

//! Engage / decline counters: `windows` = cross-KV windows served, `steps` = decode batches, `tokens` = rows across them,
//! `declines` = every decline but the knob's (a pinned-off driver is policy, not a failure), `handoffs` = the served windows
//! whose encoder rows came off the tower's device plane, no host upload.
def vulkan_wdec_stats : tuple<windows : int64; steps : int64; tokens : int64; declines : int64; handoffs : int64> {
    let policy = (g_wd_declines.counts?["{VulkanWdecDecline.knob}"] ?? 0l)
    return (windows = g_wd_windows, steps = g_wd_steps, tokens = g_wd_tokens, declines = g_wd_declines.total - policy, handoffs = g_wd_handoffs)
}

//! The declines-by-reason table (a copy), keyed by `VulkanWdecDecline` value names.
def vulkan_wdec_declines : table<string; int64> => <- clone_to_move(g_wd_declines.counts)

//! The step-time split across every served decode batch: the submit's wall against the host encode, microseconds.
def vulkan_wdec_times : tuple<us_gpu : int64; us_encode : int64> =>
    (us_gpu = g_wd_us_gpu, us_encode = g_wd_us_encode)

//! Test A/B seat: pin the ASR-decoder driver on / off in-process (the env knob's setter twin).
def set_vulkan_wdec(on : bool) {
    g_wd_env = on
}

def vulkan_wdec_on : bool => g_wd_env

[cold_path]
def private wd_decline_words(why : VulkanWdecDecline) : string {
    if (why == VulkanWdecDecline.knob) {
        return "the decoder knob is off (DASLLAMA_VK_WDEC=0 or set_vulkan_wdec(false))"
    } elif (why == VulkanWdecDecline.quant_mode) {
        return "the decoder's planes are the f32 rail, not the q8 image the driver serves"
    } elif (why == VulkanWdecDecline.shape) {
        return "a width or head size the stamps do not serve"
    } elif (why == VulkanWdecDecline.memory) {
        return VK_DECLINE_WORDS_MEMORY
    } elif (why == VulkanWdecDecline.rows) {
        return "the window's first batch is wider than the step's {WD_ROW_CAP}-row cap"
    } elif (why == VulkanWdecDecline.flavor) {
        return "the model is not the Vulkan-flavored image"
    }
    return VK_DECLINE_WORDS_DEVICE
}

[cold_path]
def private wd_decline(why : VulkanWdecDecline) : bool
    => g_wd_declines |> note("the whisper decoder", "{why}", "the decoder stays on the CPU chain: {wd_decline_words(why)}")

[cold_path]
def private wd_dev_buf(bytes : int64) : uint64 => soft_device_buf(g_wd.oom, bytes)

[cold_path]
def private wd_call_buf(bytes : int64) : uint64 => soft_call_buf(g_wd.oom, bytes)

[cold_path]
def private wd_host_buf(bytes : int64; cached : bool) : HostBuf => soft_host_buf(g_wd.oom, bytes, cached)

//! the waiting cross-KV layouts of `ds`, where they are its own, landed in its `kx` / `vx` - the state in hand, so the write lands in
//! live memory
[cold_path]
def private wd_flush_cross_kv(var ds : DecoderState) {
    if (g_wd_pend_bytes == 0l || g_wd_pend_uid != ds.gpu_uid) {
        return
    }
    let r & = g_wd
    var kxp = unsafe(addr(ds.kx[0]))
    var vxp = unsafe(addr(ds.vx[0]))
    readback_range(r.kx_dev, WD_KX, r.kvhost, kxp, g_wd_pend_bytes)
    readback_range(r.vx_dev, WD_VX, r.kvhost, vxp, g_wd_pend_bytes)
    g_wd_pend_bytes = 0l
}

//! the waiting layouts lost unread (their planes released, or another window's about to overwrite them): the owner is marked,
//! and its next decode step panics until it starts a window of its own
[cold_path]
def private wd_drop_pending {
    if (g_wd_pend_bytes > 0l) {
        g_wd_dropped |> insert(g_wd_pend_uid)
        g_wd_pend_bytes = 0l
    }
}

[cold_path]
def private wd_release {
    var r & = g_wd
    wd_drop_pending()
    if (r.key != 0ul) {
        for (b in fixed_array(r.wq_dev, r.ws_dev, r.norms_dev, r.kxr_dev, r.vxr_dev, r.kc_dev, r.vc_dev, r.x_dev, r.xq_dev, r.xs_dev,
                              r.qkv_dev, r.cq_dev, r.yo_dev, r.h_dev, r.pms_dev, r.py_dev, r.logits_dev, r.xin_dev, r.xh_dev, r.xkv_q_dev,
                              r.xkv_s_dev, r.xkv_proj_dev, r.kx_dev, r.vx_dev)) {
            if (b != 0ul) {
                destroy_device_buf(b)
            }
        }
        destroy_host_buf(r.xkv_meta)
        destroy_host_buf(r.xhost)
        destroy_host_buf(r.kvhost)
        for (hb in r.site_meta) {
            destroy_host_buf(hb)
        }
    }
    wd_forget()
}

//! the handles forgotten (the model drop's sweep destroys the objects, the waiting layouts with them); the next served window
//! uploads again
[cold_path]
def private wd_forget {
    var r & = g_wd
    wd_drop_pending()
    r.key = 0ul
    r.oom = false
    r.served_said = false
    r.handoff_miss_said = false
    r.site_meta |> clear()
    r.s_site |> clear()
    r.xkv_meta_bytes = 0l
    g_wd_window_uid = 0ul
    g_wd_window_live = false
}

//! Release every device object the driver owns (the tests' leak rail and the model-switch seat); the next served window
//! re-attaches.
def vulkan_wdec_shutdown {
    wd_release()
}

[cold_path]
def private wd_xkv_set : VkDescriptorSet {
    let r & = g_wd
    return vt_tile_cls_set(r.xkv_tile, r.wq_dev, r.ws_dev, r.wq_bytes, r.ws_bytes, r.xkv_meta.buf, r.xkv_meta_bytes, r.xh_dev,
        (r.ta_tile_rows + TILE_READ_SLACK) * r.d * 2l, r.xkv_q_dev, r.xkv_s_dev, r.ta * r.d, WD_XFEED, r.xkv_proj_dev, r.ta_tile_rows * r.d * 4l, WD_TMP)
}

//! the decoder's buffer sizes in bytes from its geometry: the row-cap planes (`bx` floats, `bq` / `bs` their Q8_0 quants and
//! scales, `bh` the FFN's), the self K/V planes (`bkc`), the cross K/V planes as halves and floats (`bkx`, `bkxf`), the
//! encoder rows (`bt`) and the attention's chunk partials (`bpms`, `bpy`)
def private wd_sizes(r : WdResident) : tuple<bx : int64; bq : int64; bs : int64; bh : int64; bkc : int64; bkx : int64; bkxf : int64; bt : int64; bpms : int64; bpy : int64> {
    let d = r.d
    let bq = WD_ROW_CAP * 4l * d
    let nch_cap = (max(r.ta, r.tmax) + int64(WDEC_CHUNK) - 1l) / int64(WDEC_CHUNK)
    return (bx = WD_ROW_CAP * d * 4l, bq = bq, bs = bq / kq_elems(KqFmt.q8) * 4l, bh = WD_ROW_CAP * 4l * d * 4l,
        bkc = r.nl * d * r.tmax * 2l, bkx = r.nl * d * r.ta * 2l, bkxf = r.nl * d * r.ta * 4l, bt = r.ta * d * 4l,
        bpms = nch_cap * r.hh * WD_ROW_CAP * 2l * 4l, bpy = nch_cap * r.hh * WD_ROW_CAP * r.hs * 4l)
}

[cold_path]
def private wd_sets {
    var r & = g_wd
    let d = r.d
    let q8e = kq_elems(KqFmt.q8)
    let (bx, bq, bs, bh, bkc, bkx, bkxf, bt, bpms, bpy) = wd_sizes(r)
    r.s_kv_k = set_tower_wdec_kv16_cls(fixed_array(r.qkv_dev, r.kc_dev, r.norms_dev), fixed_array(3l * bx, bkc, r.norms_bytes), fixed_array(WD_QKV, WD_KC, 0u))
    r.s_kv_v = set_tower_wdec_kv16_cls(fixed_array(r.qkv_dev, r.vc_dev, r.norms_dev), fixed_array(3l * bx, bkc, r.norms_bytes), fixed_array(WD_QKV, WD_VC, 0u))
    r.s_attn_self = set_tower_wdec_attn_part_cls(fixed_array(r.qkv_dev, r.kc_dev, r.vc_dev, r.pms_dev, r.py_dev, r.norms_dev),
        fixed_array(3l * bx, bkc, bkc, bpms, bpy, r.norms_bytes), fixed_array(WD_QKV, WD_KC, WD_VC, WD_PARTIALS, WD_PARTIALS, 0u))
    r.s_attn_cross = set_tower_wdec_attn_part_cls(fixed_array(r.cq_dev, r.kxr_dev, r.vxr_dev, r.pms_dev, r.py_dev, r.norms_dev),
        fixed_array(bx, bkx, bkx, bpms, bpy, r.norms_bytes), fixed_array(WD_CQ, WD_KXR, WD_VXR, WD_PARTIALS, WD_PARTIALS, 0u))
    r.s_ln_rq = set_tower_ln_rq_cls(fixed_array(r.x_dev, r.norms_dev, r.xq_dev, r.xs_dev), fixed_array(bx, r.norms_bytes, bq, bs), fixed_array(WD_X, 0u, WD_XQ, WD_XQ))
    r.s_post_rq = set_tower_post_add_ln_rq_cls(fixed_array(r.x_dev, r.yo_dev, r.norms_dev, r.xq_dev, r.xs_dev), fixed_array(bx, bx, r.norms_bytes, bq, bs), fixed_array(WD_X, WD_YO, 0u, WD_XQ, WD_XQ))
    r.s_bias_h_rq = set_tower_bias_act_rq_cls(fixed_array(r.h_dev, r.norms_dev, r.xq_dev, r.xs_dev), fixed_array(bh, r.norms_bytes, bq, bs), fixed_array(WD_H, 0u, WD_XQ, WD_XQ))
    r.s_comb_rq = set_tower_wdec_attn_comb_rq_cls(fixed_array(r.pms_dev, r.py_dev, r.xq_dev, r.xs_dev), fixed_array(bpms, bpy, bq, bs), fixed_array(WD_PARTIALS, WD_PARTIALS, WD_XQ, WD_XQ))
    if (r.cm2) {
        r.s_xfeed = set_tower_clamp_cvt_cls(fixed_array(r.xin_dev, r.xh_dev), fixed_array(bt, (r.ta_tile_rows + TILE_READ_SLACK) * d * 2l), fixed_array(WD_XIN, WD_XFEED))
    } else {
        r.s_xfeed = set_tower_clamp_rq_cls(fixed_array(r.xin_dev, r.xkv_q_dev, r.xkv_s_dev), fixed_array(bt, r.ta * d, r.ta * d / q8e * 4l), fixed_array(WD_XIN, WD_XFEED, WD_XFEED))
    }
    r.s_xkv = wd_xkv_set()
    r.s_kvx_k16 = set_tower_wdec_kv16_cls(fixed_array(r.xkv_proj_dev, r.kxr_dev, r.norms_dev), fixed_array(bt, bkx, r.norms_bytes), fixed_array(WD_TMP, WD_KXR, 0u))
    r.s_kvx_k = set_tower_wdec_kv_cls(fixed_array(r.xkv_proj_dev, r.kx_dev, r.norms_dev), fixed_array(bt, bkxf, r.norms_bytes), fixed_array(WD_TMP, WD_KX, 0u))
    r.s_kvx_v16 = set_tower_wdec_kv16_cls(fixed_array(r.xkv_proj_dev, r.vxr_dev, r.norms_dev), fixed_array(bt, bkx, r.norms_bytes), fixed_array(WD_TMP, WD_VXR, 0u))
    r.s_kvx_v = set_tower_wdec_kv_cls(fixed_array(r.xkv_proj_dev, r.vx_dev, r.norms_dev), fixed_array(bt, bkxf, r.norms_bytes), fixed_array(WD_TMP, WD_VX, 0u))
    let f = Q8GemvFeed(planes = (wq = r.wq_dev, ws = r.ws_dev, wqb = r.wq_bytes, wsb = r.ws_bytes), xq = r.xq_dev, xs = r.xs_dev,
        x_elems = WD_ROW_CAP * 4l * r.d, xbit = WD_XQ)
    r.s_site |> reserve(r.nl * WD_LAYER_SITES + 1l)
    r.s_site |> resize(r.nl * WD_LAYER_SITES + 1l)
    for (l in range64(r.nl)) {
        let b = int(l * WD_LAYER_SITES)
        r.s_site[b + int(WD_S_QKV)] = q8_gemv_set(f, r.site_meta[b + int(WD_S_QKV)].buf, r.qkv_dev, 3l * bx, WD_QKV)
        r.s_site[b + int(WD_S_O)] = q8_gemv_set(f, r.site_meta[b + int(WD_S_O)].buf, r.yo_dev, bx, WD_YO)
        r.s_site[b + int(WD_S_CQ)] = q8_gemv_set(f, r.site_meta[b + int(WD_S_CQ)].buf, r.cq_dev, bx, WD_CQ)
        r.s_site[b + int(WD_S_CO)] = q8_gemv_set(f, r.site_meta[b + int(WD_S_CO)].buf, r.yo_dev, bx, WD_YO)
        r.s_site[b + int(WD_S_FC1)] = q8_gemv_set(f, r.site_meta[b + int(WD_S_FC1)].buf, r.h_dev, bh, WD_H)
        r.s_site[b + int(WD_S_FC2)] = q8_gemv_set(f, r.site_meta[b + int(WD_S_FC2)].buf, r.yo_dev, bx, WD_YO)
    }
    let te = int(r.nl * WD_LAYER_SITES)
    r.s_site[te] = q8_gemv_set(f, r.site_meta[te].buf, r.logits_dev, r.nvocab * 4l, WD_LOGITS)
}

//! every class the two chains dispatch built ahead of the sets that bind them, or false
[cold_path]
def private wd_ensure_classes(cm2 : bool; tile : VtTile) : bool {
    return (gemv_cls_ensure(int(KqFmt.q8)) && gemv_cls_ensure_n(int(KqFmt.q8), WD_ROW_CAP) && vt_tile_ensure(tile)
        && ensure_tower_ln_rq_cls() && ensure_tower_post_add_ln_rq_cls() && ensure_tower_bias_act_rq_cls() && ensure_tower_clamp_rq_cls()
        && (!cm2 || ensure_tower_clamp_cvt_cls()) && ensure_tower_wdec_kv_cls() && ensure_tower_wdec_kv16_cls() && ensure_tower_wdec_attn_part_cls()
        && ensure_tower_wdec_attn_comb_rq_cls())
}

[cold_path]
def private wd_upload_weights(w : WhisperModel) {
    var r & = g_wd
    let nl = r.nl
    let src = vt_dev_src(w.enc)
    r.wq_bytes = src.on ? vt_piece_bytes(src, "{WD_PIECE}.wq") : w.dqblob.n
    r.ws_bytes = src.on ? vt_piece_bytes(src, "{WD_PIECE}.ws") : w.dqscales.n * 2l
    r.wq_dev = wd_dev_buf(r.wq_bytes)
    r.ws_dev = wd_dev_buf(r.ws_bytes)
    let lay = active_q8_repack_layout(g_active_needs_repack)
    r.site_meta |> reserve(nl * WD_LAYER_SITES + 1l)
    for (l in range64(nl)) {
        let lo = dec_layer_offsets(w, l)
        r.site_meta |> push(q8_site_meta(r.oom, [lo.q_w, lo.k_w, lo.v_w]))   // nolint:STYLE012 - a layer's sites append to the previous layer's
        r.site_meta |> push(q8_site_meta(r.oom, [lo.o_w]))
        r.site_meta |> push(q8_site_meta(r.oom, [lo.cq_w]))
        r.site_meta |> push(q8_site_meta(r.oom, [lo.co_w]))
        r.site_meta |> push(q8_site_meta(r.oom, [lo.fc1_w]))
        r.site_meta |> push(q8_site_meta(r.oom, [lo.fc2_w]))
    }
    r.site_meta |> push(q8_site_meta(r.oom, [w.d_te_q_off]))
    return if (r.oom)
    if (src.on) {
        vt_put_piece(src, "{WD_PIECE}.wq", r.wq_dev)
        vt_put_piece(src, "{WD_PIECE}.ws", r.ws_dev)
        return
    }
    var inscope regions <- wd_regions(w)
    vt_upload_regions(w.dqblob, w.dqscales, regions, lay, r.wq_dev, r.ws_dev, r.wq_bytes, r.ws_bytes)
}

let private WD_PIECE = "vk.wdec"   //! the whisper decoder's q8 pieces' prefix, in the encoder tower's device block

//! the whisper decoder's q8 regions: the token table the logits read, then every layer's eight attention GEMMs and its FFN pair
def private wd_regions(w : WhisperModel) : array<Q8Region> {
    let d = w.n_text_state
    let nl = w.n_text_layer
    var regions : array<Q8Region>
    regions |> reserve(nl * 10l + 1l)
    regions |> push(Q8Region(woff = w.d_te_q_off, n = d, rows = w.n_vocab))
    for (l in range64(nl)) {
        let lo = dec_layer_offsets(w, l)
        for (woff in fixed_array(lo.q_w, lo.k_w, lo.v_w, lo.o_w, lo.cq_w, lo.ck_w, lo.cv_w, lo.co_w)) {
            regions |> push(Q8Region(woff = woff, n = d, rows = d))
        }
        regions |> push(Q8Region(woff = lo.fc1_w, n = d, rows = 4l * d))
        regions |> push(Q8Region(woff = lo.fc2_w, n = 4l * d, rows = d))
    }
    return <- regions
}

//! whisper's Vulkan device block (`#tower-dev-block`): the encoder tower's pieces, then the decoder's q8 regions gathered
[cold_path, arch(at = "../ARCHITECTURE_IMAGE_GPU_PLANES.md#tower-dev-block")]
def private vulkan_whisper_bake(w : WhisperModel; var b : TowerDevBuild) : bool {
    vulkan_audio_tower_bake_into(w.enc, b)
    var inscope regions <- wd_regions(w)
    vt_bake_q8(b, WD_PIECE, w.dqblob, w.dqscales, regions)
    return true
}

//! whisper's Vulkan chains serve the q8 lane: the encoder's and the decoder's knobs on
def private vulkan_whisper_serves_model(q8 : bool) : bool => q8 && g_wd_env && vulkan_whisper_tower_serves()

[cold_path]
def private wd_upload_norms(w : WhisperModel) {
    var r & = g_wd
    let d = r.d
    r.norms_base = w.dlayers_off
    let rows_n = r.nl * w.dlayer_stride
    r.final_ln_w_off = rows_n
    r.final_ln_b_off = rows_n + d
    r.norms_bytes = (rows_n + 2l * d) * 4l
    r.norms_dev = wd_dev_buf(r.norms_bytes)
    if (!r.oom) {
        upload_float_ranges(r.norms_dev, [(p = plane_at(w.dblob, w.dlayers_off), n = rows_n), (p = plane_at(w.dblob, w.d_ln_w_off), n = d),
                                          (p = plane_at(w.dblob, w.d_ln_b_off), n = d)])
    }
}

[arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-asr-decoder")]
def private wd_attach(w : WhisperModel) : bool {
    var r & = g_wd
    let lay = active_q8_repack_layout(g_active_needs_repack)
    let src = vt_dev_src(w.enc)
    let rows_n = w.n_text_layer * w.dlayer_stride
    let key = (src.on ? vt_key_dev(src, w.dblob, w.dlayers_off, rows_n, w.n_text_layer)
        : vt_key(w.dqblob, w.dqscales, w.dblob, w.dlayers_off, rows_n, 0l, w.n_text_layer, lay.mr, lay.kgroup, lay.wbias))
    if (r.key != 0ul && key == r.key) {
        return true
    }
    return wd_build(w, key)
}

//! the resident built for a model the key does not know: the old one released, the planes gathered and uploaded, the
//! scratch sized to the model, the sets bound
[cold_path]
def private wd_build(w : WhisperModel; key : uint64) : bool {
    var r & = g_wd
    wd_release()
    let q8e = kq_elems(KqFmt.q8)
    let d = w.n_text_state
    let nl = w.n_text_layer
    r.d = d
    r.hh = w.n_text_head
    r.hs = d / r.hh
    r.ta = w.enc.n_ctx
    r.tmax = w.n_text_ctx
    r.nl = nl
    r.nvocab = w.n_vocab
    r.cm2 = vt_cm2_feed()
    r.xkv_tile = vt_tile(r.cm2, d, r.ta)
    r.ta_tile_rows = vt_tile_rows(r.xkv_tile, r.ta)
    r.oom = false
    if (!wd_ensure_classes(r.cm2, r.xkv_tile)) {
        return false
    }
    let src = vt_dev_src(w.enc)
    let wq_n = src.on ? vt_piece_bytes(src, "{WD_PIECE}.wq") : w.dqblob.n
    if (!vk_range_ok(wq_n) || !vk_range_ok(nl * d * r.ta * 4l) || !vk_range_ok(nl * d * r.tmax * 2l) || !vk_range_ok(2l * nl * d * r.ta * 4l)) {
        return wd_decline(VulkanWdecDecline.shape)
    }
    r.key = key
    wd_upload_weights(w)
    wd_upload_norms(w)
    let z = wd_sizes(r)
    r.kxr_dev = wd_dev_buf(z.bkx)
    r.vxr_dev = wd_dev_buf(z.bkx)
    r.kc_dev = wd_dev_buf(z.bkc)
    r.vc_dev = wd_dev_buf(z.bkc)
    r.x_dev = wd_call_buf(z.bx)
    r.xq_dev = wd_call_buf(z.bq)
    r.xs_dev = wd_dev_buf(z.bs)
    r.qkv_dev = wd_dev_buf(3l * z.bx)
    r.cq_dev = wd_dev_buf(z.bx)
    r.yo_dev = wd_dev_buf(z.bx)
    r.h_dev = wd_dev_buf(z.bh)
    r.pms_dev = wd_dev_buf(z.bpms)
    r.py_dev = wd_dev_buf(z.bpy)
    r.logits_dev = wd_dev_buf(r.nvocab * 4l)
    r.xin_dev = wd_call_buf(z.bt)
    if (r.cm2) {
        r.xh_dev = wd_dev_buf((r.ta_tile_rows + TILE_READ_SLACK) * d * 2l)
        r.xkv_q_dev = wd_dev_buf(64l)
        r.xkv_s_dev = wd_dev_buf(64l)
    } else {
        r.xh_dev = wd_dev_buf(64l)
        r.xkv_q_dev = wd_dev_buf(r.ta * d)
        r.xkv_s_dev = wd_dev_buf(r.ta * d / q8e * 4l)
    }
    r.xkv_proj_dev = wd_dev_buf(r.ta_tile_rows * d * 4l)
    r.kx_dev = wd_dev_buf(z.bkxf)
    r.vx_dev = wd_dev_buf(z.bkxf)
    let groups = vt_tile_groups(r.xkv_tile, d, r.ta)
    r.xkv_meta_bytes = max((2l * nl * 4l + 2l * nl * groups) * 4l, BATCH_META_BYTES)
    r.xkv_meta = wd_host_buf(r.xkv_meta_bytes, false)
    r.xhost = wd_host_buf(max(z.bx, r.nvocab * 4l), true)
    r.kvhost = wd_host_buf(2l * z.bkxf, true)
    if (r.oom) {
        wd_release()
        return false
    }
    wd_sets()
    return true
}

[cold_path]
def private wd_say_handoff_miss {
    g_wd.handoff_miss_said = true
    to_log(LOG_INFO, "dasLLAMA vulkan wdec: the encoder rows arrive by host upload - the tower landed no device plane for this window (the encoder ran on the CPU, or declined)\n")
}

[cold_path]
def private wd_announce {
    g_wd.served_said = true
    to_log(LOG_INFO, "dasLLAMA vulkan wdec: the whisper decoder serves on the device (d {g_wd.d}, {g_wd.nl} layers)\n")
}

//! one decoder attention pass: the chunk partials over the K/V rows at element `kvoff` (the query's bias row at `qboff`),
//! then their combine with its requant
def private wd_attn(raw : VkCommandBuffer; var h : VkHaz; var s_part : VkDescriptorSet; var pc : GkWdecAttnArgs; var pc_comb : GkWdecCombArgs;
                    kvoff, qboff : uint; wg_part, wg_comb : int64) {
    pc.koff = kvoff
    pc.voff = kvoff
    pc.qboff = qboff
    enc_tower_wdec_attn_part_cls(raw, h, s_part, pc, wg_part)
    enc_tower_wdec_attn_comb_rq_cls(raw, h, g_wd.s_comb_rq, pc_comb, wg_comb)
    vt_pt(raw, VtProfRole.attn)
}

//! a site's GEMV over `n` rows, then the residual seam that adds its bias row `boff` and norms the next block's input
//! (the weight and bias rows at `lnoff` / `lnboff`)
def private wd_gemv_post(raw : VkCommandBuffer; var h : VkHaz; site : int; n_in, d_out, n : int64; role : VtProfRole; var pc_post : GkPostAddLnArgs;
                         boff, lnoff, lnboff : uint) {
    q8_gemv(raw, h, g_wd.s_site[site], n_in, d_out, 1l, n)
    vt_pt(raw, role)
    pc_post.boff = boff
    pc_post.lnoff = lnoff
    pc_post.lnboff = lnboff
    enc_tower_post_add_ln_rq_cls(raw, h, g_wd.s_post_rq, pc_post, n)
    vt_pt(raw, VtProfRole.post_add)
}

//! one side of a layer's cross K/V: its GEMM over the encoder rows (the schedule map at `map_off`), then its f16-plane and
//! f32-row stores at element `off`, the bias row at `boff`
def private wd_xkv_side(raw : VkCommandBuffer; var h : VkHaz; var pc_g : BatchArgs; map_off : uint; groups : int64; gemm_role : VtProfRole;
                        var s_kv16, s_kvx : VkDescriptorSet; var pc16, pcx : GkWdecKvArgs; off, boff : uint; wg_kv : int64) {
    pc_g.map_off = map_off
    vt_tile_enc(raw, h, g_wd.xkv_tile, g_wd.s_xkv, pc_g, groups)
    vt_pt(raw, gemm_role)
    pc16.dst_off = off
    pc16.boff = boff
    enc_tower_wdec_kv16_cls(raw, h, s_kv16, pc16, wg_kv)
    pcx.dst_off = off
    pcx.boff = boff
    enc_tower_wdec_kv_cls(raw, h, s_kvx, pcx, wg_kv)
    vt_pt(raw, VtProfRole.kv_store)
}

[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-asr-decoder")]
def private vulkan_whisper_cross_kv(w : WhisperModel; var ds : DecoderState; enc_out : array<float>) : bool {   // nolint:STYLE038 - the dispatch chain is the shape
    let uid = wd_uid(ds)
    g_wd_window_live = false
    g_wd_dropped |> erase(uid)
    if (g_wd_pend_uid == uid) {
        g_wd_pend_bytes = 0l   // this window supersedes the state's waiting one, served or not: the CPU chain recomputes on a decline
    }
    ds.gpu_kv_gen = 0l
    if (!g_wd_env) {
        return wd_decline(VulkanWdecDecline.knob)
    }
    if (!w.dq8) {
        return wd_decline(VulkanWdecDecline.quant_mode)
    }
    if (w.enc.dev_backend != TowerDev.vulkan) {
        return wd_decline(VulkanWdecDecline.flavor)
    }
    let d = w.n_text_state
    let hs = d / w.n_text_head
    if (d % 64l != 0l || hs != int64(WDEC_HS)) {
        return wd_decline(VulkanWdecDecline.shape)
    }
    if (!vulkan_tower_arms()) {
        return wd_decline(VulkanWdecDecline.device)
    }
    if (!wd_attach(w)) {
        return wd_decline(g_wd.oom ? VulkanWdecDecline.memory : VulkanWdecDecline.device)
    }
    var r & = g_wd
    let q8e = kq_elems(KqFmt.q8)
    let nl = r.nl
    let ta = r.ta
    let hh = r.hh
    let z = wd_sizes(r)
    let bt = z.bt
    assert(long_length(ds.kx) >= nl * d * ta && long_length(ds.vx) >= nl * d * ta)
    wd_drop_pending()
    let groups = vt_tile_groups(r.xkv_tile, d, ta)
    vt_sched_walk(g_wd_maps, r.xkv_meta, 2l * nl) $(rid : int64) : VtSchedRec {
        let lo = dec_layer_offsets(w, rid / 2l)
        return VtSchedRec(wblk0 = (rid % 2l == 0l ? lo.ck_w : lo.cv_w) / q8e, rows = r.ta_tile_rows, groups = groups)
    }
    let enc_dev = vulkan_tower_enc_out(unsafe(addr<float const?>(enc_out[0])), bt)
    if (enc_dev == 0ul) {
        upload_region_at(r.xin_dev, 0l, unsafe(addr<uint8 const?>(enc_out[0])), bt)
        if (!r.handoff_miss_said) {
            wd_say_handoff_miss()
        }
    } else {
        g_wd_handoffs++
    }
    var pc_rq = RqArgs(inbase = 0u, nblk = uint(ta * d / q8e), lo = -VT_NO_CLAMP, hi = VT_NO_CLAMP)
    var pc_g = BatchArgs(n = uint(d), d = uint(d), map_off = 0u)
    let total = ta * hh * hs
    let wg_kv = (total + 255l) / 256l
    var pc_k16 = GkWdecKvArgs(rows = uint(ta), sstride = uint(d), soff = 0u, nheads = uint(hh), tstride = uint(ta), p0 = 0u,
        dst_off = 0u, boff = 0u, bias_on = 0u, mode = WDEC_KV_F16, hs = uint(hs), kscale = wdec_kscale(hs))
    var pc_kx = pc_k16
    pc_kx.mode = WDEC_KV_KX
    var pc_v16 = pc_k16
    pc_v16.bias_on = 1u
    var pc_vx = pc_v16
    pc_vx.mode = WDEC_KV_VX
    vt_chain_one_shot("whisper cross-kv", nl, ta) $(raw : VkCommandBuffer; var h : VkHaz) {
        if (enc_dev != 0ul) {
            vhz_dep(raw, h, 0u, WD_XIN, true)
            cmd_copy_range(raw, enc_dev, 0l, r.xin_dev, 0l, bt)
            vt_pt(raw, VtProfRole.copy_out)
        }
        vt_feed_enc(raw, h, r.cm2, r.s_xfeed, pc_rq, ta * d)
        vt_pt(raw, VtProfRole.rq)
        for (l in range64(nl)) {
            let lo = dec_layer_offsets(w, l)
            let off = uint(l * d * ta)
            wd_xkv_side(raw, h, pc_g, g_wd_maps[l * 2l], groups, VtProfRole.gemm_k, r.s_kvx_k16, r.s_kvx_k, pc_k16, pc_kx, off, 0u, wg_kv)
            wd_xkv_side(raw, h, pc_g, g_wd_maps[l * 2l + 1l], groups, VtProfRole.gemm_v, r.s_kvx_v16, r.s_kvx_v, pc_v16, pc_vx, off, uint(lo.cv_b - r.norms_base), wg_kv)
        }
    }
    g_wd_pend_uid = uid
    g_wd_pend_bytes = z.bkxf
    g_wd_windows++
    g_wd_window_uid = uid
    g_wd_window_live = true
    if (!r.served_said) {
        wd_announce()
    }
    return true
}

[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-asr-decoder")]
def private vulkan_whisper_decode(w : WhisperModel; var ds : DecoderState; toks : int const?; n : int64) : bool {   // nolint:STYLE038 - the dispatch chain is the shape
    let uid = ds.gpu_uid
    if (uid != 0ul && key_exists(g_wd_dropped, uid)) {
        panic("dasLLAMA vulkan wdec: this decoder window's cross-attention memory was lost before it was read back - another state's window, a release or a model drop took its device planes; ONE live decoder window per process, and a state decodes only a window it started after the loss")
    }
    if (!g_wd_window_live || g_wd_window_uid != uid) {
        wd_flush_cross_kv(ds)
        if (ds.gpu_kv_gen != 0l && ds.n_past > 0l) {
            panic("dasLLAMA vulkan wdec: the device decoder window was evicted mid-window - ONE live decoder window per process (a second DecoderState may not interleave whisper_cross_kv/whisper_decode_batch)")
        }
        return false
    }
    if (n > WD_ROW_CAP) {
        if (ds.n_past == 0l) {
            g_wd_window_live = false
            wd_flush_cross_kv(ds)
            return wd_decline(VulkanWdecDecline.rows)
        }
        panic("dasLLAMA vulkan wdec: decode batch of {n} exceeds the {WD_ROW_CAP}-row step cap mid-window")
    }
    var r & = g_wd
    let q8e = kq_elems(KqFmt.q8)   // nolint:LINT021 - the block counts divide in 64-bit ahead of the push constants' narrowing
    let d = r.d
    let hh = r.hh
    let hs = r.hs
    let ta = r.ta
    let tmax = r.tmax
    let nl = r.nl
    let p0 = ds.n_past
    let te0 = ref_time_ticks()
    unsafe {
        whisper_embed_rows(w, toks, n, p0, reinterpret<float?>(r.xhost.mapped))
        var logits_site_meta = reinterpret<uint?>(r.site_meta[int(nl * WD_LAYER_SITES)].mapped)
        logits_site_meta[1] = uint((n - 1l) * (d / q8e))
    }
    let bx = n * d * 4l
    upload_region_at(r.x_dev, 0l, unsafe(reinterpret<uint8 const?>(r.xhost.mapped)), bx)
    let nff = n * 4l * d
    let wg_nff = rq_wgs(false, nff)
    let kvtotal = n * hh * hs
    let wg_kv = (kvtotal + 255l) / 256l
    let nch_s = (p0 + n + int64(WDEC_CHUNK) - 1l) / int64(WDEC_CHUNK)
    let nch_c = (ta + int64(WDEC_CHUNK) - 1l) / int64(WDEC_CHUNK)
    var pc_kv = GkWdecKvArgs(rows = uint(n), sstride = uint(3l * d), soff = uint(d), nheads = uint(hh), tstride = uint(tmax), p0 = uint(p0),
        dst_off = 0u, boff = 0u, bias_on = 0u, mode = WDEC_KV_F16, hs = uint(hs), kscale = wdec_kscale(hs))
    let wd_scale = 1.0 / sqrt(float(WDEC_HS))
    var pc_as = GkWdecAttnArgs(dim = uint(3l * d), tstride = uint(tmax), len0 = uint(p0 + 1l), causal = 1u, nheads = uint(hh),
        nrows = uint(n), koff = 0u, voff = 0u, qboff = 0u, hs = WDEC_HS, scale = wd_scale)
    var pc_ac = GkWdecAttnArgs(dim = uint(d), tstride = uint(ta), len0 = uint(ta), causal = 0u, nheads = uint(hh),
        nrows = uint(n), koff = 0u, voff = 0u, qboff = 0u, hs = WDEC_HS, scale = wd_scale)
    var pc_cs = GkWdecCombArgs(dim = uint(d), nchunks = uint(nch_s), nheads = uint(hh), nrows = uint(n), hs = WDEC_HS)
    var pc_cc = pc_cs
    pc_cc.nchunks = uint(nch_c)
    var pc_gelu = TowerBiasRqArgs(d = uint(4l * d), nblk = uint(nff / q8e), boff = 0u)
    var pc_post = GkPostAddLnArgs(d = uint(d), rows = uint(n), eps = w.eps, ln_on = 1u, ascale = 1.0, xboff = 0u)
    let wg_attn_s = nch_s * hh * n
    let wg_attn_c = nch_c * hh * n
    let wg_comb = hh * n
    let t0 = ref_time_ticks()
    vt_chain_one_shot("whisper decode", nl, n) $(raw : VkCommandBuffer; var h : VkHaz) {
        for (l in range64(nl)) {
            let lo = dec_layer_offsets(w, l)
            let site = int(l * WD_LAYER_SITES)
            if (l == 0l) {
                var pc = gk_ln_args(uint(d), uint(n), w.eps, uint(lo.ln1_w - r.norms_base), uint(lo.ln1_b - r.norms_base))
                enc_tower_ln_rq_cls(raw, h, r.s_ln_rq, pc, n)
                vt_pt(raw, VtProfRole.ln)
            }
            q8_gemv(raw, h, g_wd.s_site[site + int(WD_S_QKV)], d, d, 3l, n)
            vt_pt(raw, VtProfRole.gemm_q)
            pc_kv.soff = uint(d)
            pc_kv.dst_off = uint(l * d * tmax)
            pc_kv.bias_on = 0u
            enc_tower_wdec_kv16_cls(raw, h, r.s_kv_k, pc_kv, wg_kv)
            pc_kv.soff = uint(2l * d)
            pc_kv.bias_on = 1u
            pc_kv.boff = uint(lo.v_b - r.norms_base)
            enc_tower_wdec_kv16_cls(raw, h, r.s_kv_v, pc_kv, wg_kv)
            vt_pt(raw, VtProfRole.kv_store)
            wd_attn(raw, h, r.s_attn_self, pc_as, pc_cs, uint(l * d * tmax), uint(lo.q_b - r.norms_base), wg_attn_s, wg_comb)
            wd_gemv_post(raw, h, site + int(WD_S_O), d, d, n, VtProfRole.gemm_o, pc_post,
                uint(lo.o_b - r.norms_base), uint(lo.cln_w - r.norms_base), uint(lo.cln_b - r.norms_base))
            q8_gemv(raw, h, g_wd.s_site[site + int(WD_S_CQ)], d, d, 1l, n)
            vt_pt(raw, VtProfRole.gemm_q)
            wd_attn(raw, h, r.s_attn_cross, pc_ac, pc_cc, uint(l * d * ta), uint(lo.cq_b - r.norms_base), wg_attn_c, wg_comb)
            wd_gemv_post(raw, h, site + int(WD_S_CO), d, d, n, VtProfRole.gemm_o, pc_post,
                uint(lo.co_b - r.norms_base), uint(lo.ln2_w - r.norms_base), uint(lo.ln2_b - r.norms_base))
            q8_gemv(raw, h, g_wd.s_site[site + int(WD_S_FC1)], d, 4l * d, 1l, n)
            vt_pt(raw, VtProfRole.gemm_fc1)
            pc_gelu.boff = uint(lo.fc1_b - r.norms_base)
            enc_tower_bias_act_rq_cls(raw, h, r.s_bias_h_rq, pc_gelu, wg_nff)
            vt_pt(raw, VtProfRole.bias)
            let last = l + 1l == nl
            wd_gemv_post(raw, h, site + int(WD_S_FC2), 4l * d, d, n, VtProfRole.gemm_fc2, pc_post, uint(lo.fc2_b - r.norms_base),
                uint(last ? r.final_ln_w_off : dec_layer_offsets(w, l + 1l).ln1_w - r.norms_base),
                uint(last ? r.final_ln_b_off : dec_layer_offsets(w, l + 1l).ln1_b - r.norms_base))
        }
        q8_gemv(raw, h, g_wd.s_site[int(nl * WD_LAYER_SITES)], d, r.nvocab, 1l, 1l)
        vt_pt(raw, VtProfRole.logits)
        vhz_dep(raw, h, WD_LOGITS, 0u, true)
        cmd_copy_range(raw, r.logits_dev, 0l, r.xhost.buf, 0l, r.nvocab * 4l)
        vt_pt(raw, VtProfRole.copy_out)
    }
    host_buf_read(r.xhost, unsafe(addr<void?>(ds.logits[0])), r.nvocab * 4l)
    g_wd_us_gpu += int64(get_time_usec(t0))
    g_wd_us_encode += int64(get_time_usec(te0)) - int64(get_time_usec(t0))
    g_wd_steps++
    g_wd_tokens += n
    ds.gpu_kv_gen = 1l
    return true
}

// ===== the transducer greedy decode (parakeet, GigaAM's RNNT head) =====

enum private VulkanRnntDecline {
    knob     //! DASLLAMA_VK_RNNT=0, or set_vulkan_rnnt(false)
    shape    //! a predictor or joint past the bodies' staged widths, a third layer, a duration table past the state's
    device   //! the tier has no device, or a class failed to build
    memory   //! a device or host allocation failed
    overrun  //! the rounds ran past the clip's budget with the clip not done
}

//! the hazard bits of the transducer seat's buffers
let private RV_ST = 0x1u
let private RV_HB = 0x2u
let private RV_CB = 0x4u
let private RV_XIN = 0x8u
let private RV_PRED = 0x10u
let private RV_LG = 0x20u
let private RV_TOKS = 0x40u
let private RV_ENCP = 0x80u
let private RV_ROWS = 0x100u

//! The transducer seat's device objects: the weights laid once a model (`rnnt_dev_key`: the state's halves `wh` and joint columns
//! `jw`, the slab `wf` at `lay`'s offsets, the encoder projection's f32 rows `wenc`), and the per-clip planes sized to the longest
//! clip yet.
struct private RvResident {
    key : uint64
    lay : RnntDevLayout
    wh_dev : uint64
    jw_dev : uint64
    wf_dev : uint64
    wenc_dev : uint64
    wh_bytes : int64
    jw_bytes : int64
    wf_bytes : int64
    wenc_bytes : int64
    cap_frames : int64     //! the per-clip planes' frames
    cap_shape : uint64     //! the net shape the per-clip planes are sized for
    st_dev : uint64
    hb_dev : uint64
    cb_dev : uint64
    xin_dev : uint64
    pred_dev : uint64
    lg_dev : uint64
    toks_dev : uint64
    encp_dev : uint64
    rows_dev : uint64
    host : HostBuf         //! the state words, then the token records
    st_bytes : int64
    hb_bytes : int64
    cb_bytes : int64
    xin_bytes : int64
    pred_bytes : int64
    lg_bytes : int64
    toks_bytes : int64
    encp_bytes : int64
    rows_bytes : int64
    s_gates : VkDescriptorSet
    s_proj : VkDescriptorSet
    s_joint : VkDescriptorSet
    s_pick : VkDescriptorSet
    oom : bool
    served_said : bool
    miss_said : bool       //! the handoff miss said: a projecting decode uploaded its rows
}

var private g_rv : RvResident
var private @scratch g_rv_words : array<uint>   //! the token records read back
var private g_rv_env = true
var private g_rv_declines : VkDeclineCounter
var private g_rv_decodes = 0l
var private g_rv_handoffs = 0l
var private g_rv_projects = 0l
var private g_rv_rounds = 0l
var private g_rv_steps = 0l
var private g_rv_tokens = 0l
var private g_rv_cbs = 0l
var private g_rv_uploads = 0l   //! the weight uploads: one a model, again only where another net takes the seat
var private g_rv_rate = 0.3lf   //! the rounds a frame the last served clip took: the first command buffer's estimate

//! Engage counters: `decodes` served on the device, `handoffs` among them off the encode's device rows, `projects` among them
//! projected on the device, `rounds` / `steps` the picks' rounds and predictor steps, `tokens`, `cbs` the command buffers,
//! `uploads` the weight uploads, `declines` every decline but the knob's.
def vulkan_rnnt_stats : tuple<decodes : int64; handoffs : int64; projects : int64; rounds : int64; steps : int64; tokens : int64; cbs : int64; uploads : int64; declines : int64> {
    return (decodes = g_rv_decodes, handoffs = g_rv_handoffs, projects = g_rv_projects, rounds = g_rv_rounds, steps = g_rv_steps,
        tokens = g_rv_tokens, cbs = g_rv_cbs, uploads = g_rv_uploads, declines = g_rv_declines.total - (g_rv_declines.counts?["{VulkanRnntDecline.knob}"] ?? 0l))
}

//! The transducer seat's declines-by-reason table (a copy), keyed by the decline's name, the knob's among them.
def vulkan_rnnt_declines : table<string; int64> => <- clone_to_move(g_rv_declines.counts)

//! Pin the transducer decode on the device on/off in-process (DASLLAMA_VK_RNNT's twin); answers the setting it replaced.
def set_vulkan_rnnt(on : bool) : bool {
    let was = g_rv_env
    g_rv_env = on
    return was
}

[cold_path]
def private rv_decline(why : VulkanRnntDecline) : bool {
    let words = (why == VulkanRnntDecline.knob ? "the transducer knob is off (DASLLAMA_VK_RNNT=0 or set_vulkan_rnnt(false))"
        : why == VulkanRnntDecline.shape ? "a predictor, joint or duration table past the stamps' staged widths, or a buffer past the device's range"
        : why == VulkanRnntDecline.memory ? VK_DECLINE_WORDS_MEMORY
        : why == VulkanRnntDecline.overrun ? "the rounds ran past the clip's budget with the clip not done" : VK_DECLINE_WORDS_DEVICE)
    return g_rv_declines |> note("the transducer decode", "{why}", "the decode stays on the CPU loop: {words}")
}

[cold_path]
def private rv_dev_buf(bytes : int64) : uint64 => soft_device_buf(g_rv.oom, bytes)

[cold_path]
def private rv_call_buf(bytes : int64) : uint64 => soft_call_buf(g_rv.oom, bytes)

[cold_path]
def private rv_release_scratch {
    var r & = g_rv
    release_device_buf(r.st_dev)
    release_device_buf(r.hb_dev)
    release_device_buf(r.cb_dev)
    release_device_buf(r.xin_dev)
    release_device_buf(r.pred_dev)
    release_device_buf(r.lg_dev)
    release_device_buf(r.toks_dev)
    release_device_buf(r.encp_dev)
    release_device_buf(r.rows_dev)
    if (r.host.buf != 0ul) {
        destroy_host_buf(r.host)
    }
    r.host = HostBuf()
    r.cap_frames = 0l
    r.cap_shape = 0ul
}

[cold_path]
def private rv_release {
    var r & = g_rv
    rv_release_scratch()
    release_device_buf(r.wh_dev)
    release_device_buf(r.jw_dev)
    release_device_buf(r.wf_dev)
    release_device_buf(r.wenc_dev)
    rv_forget()
}

//! the handles forgotten (the model drop's sweep destroys the objects); the next served decode uploads again and says its
//! declines afresh
[cold_path]
def private rv_forget {
    g_rv = RvResident()
    g_rv_declines |> unsay("the transducer decode")
}

//! Release every device object the transducer seat owns (the tests' leak rail and the model-switch seat).
def vulkan_rnnt_shutdown {
    rv_release()
}

//! the weights laid once a weight plane, as the Metal seat lays them
def private rv_attach(net : RnntNet; s : RnntState; enc : RnntEnc) : bool {
    let key = rnnt_dev_key(net, enc)
    return true if (g_rv.key == key && g_rv.wh_dev != 0ul)
    return rv_attach_build(net, s, enc, key)
}

//! a new net's weights uploaded under `key`, the old seat released and its said declines forgotten
[cold_path]
def private rv_attach_build(net : RnntNet; s : RnntState; enc : RnntEnc; key : uint64) : bool {
    var r & = g_rv
    rv_release()
    r.oom = false
    r.lay = rnnt_dev_layout(net)
    var inscope wf <- rnnt_dev_slab(net, s, enc, r.lay)
    r.wh_bytes = r.lay.wh_halves * 2l
    r.jw_bytes = r.lay.jw_halves * 2l
    r.wf_bytes = long_length(wf) * 4l
    r.wenc_bytes = enc.q8 ? 0l : net.joint_dim * enc.d * 4l
    if (!vk_range_ok(max(max(r.wh_bytes, r.wf_bytes), max(r.jw_bytes, r.wenc_bytes)))) {
        return rv_decline(VulkanRnntDecline.shape)
    }
    r.wh_dev = rv_dev_buf(r.wh_bytes)
    r.jw_dev = rv_dev_buf(r.jw_bytes)
    r.wf_dev = rv_dev_buf(r.wf_bytes)
    r.wenc_dev = enc.q8 ? 0ul : rv_dev_buf(r.wenc_bytes)
    r.oom = r.oom || rnnt_dev_faulted(RnntDevFault.memory)
    if (r.oom) {
        rv_release()
        return rv_decline(VulkanRnntDecline.memory)
    }
    upload_region_at(r.wh_dev, 0l, unsafe(addr<uint8 const?>(s.wh[0])), r.wh_bytes)
    upload_region_at(r.jw_dev, 0l, unsafe(addr<uint8 const?>(s.joint_wt[0])), r.jw_bytes)
    upload_region_at(r.wf_dev, 0l, unsafe(addr<uint8 const?>(wf[0])), r.wf_bytes)
    if (!enc.q8) {
        upload_region_at(r.wenc_dev, 0l, unsafe(reinterpret<uint8 const?>(plane_at(net.blob, enc.w))), r.wenc_bytes)
    }
    r.key = key
    g_rv_uploads++
    return true
}

//! the per-clip planes for `n_frames` (a whole 64-frame step past the largest clip yet) and the round's four sets
def private rv_scratch(net : RnntNet; n_frames, n_logits, d : int64) : bool {
    let shape = hash_combine64(hash_combine64(uint64(net.pred_dim), uint64(net.joint_dim)),
        hash_combine64(uint64(net.n_layers), hash_combine64(uint64(n_logits), uint64(d))))
    return true if (g_rv.cap_frames >= n_frames && g_rv.cap_shape == shape && g_rv.st_dev != 0ul)
    return rv_scratch_build(net, n_frames, n_logits, d, shape)
}

[cold_path]
def private rv_scratch_build(net : RnntNet; n_frames, n_logits, d : int64; shape : uint64) : bool {
    var r & = g_rv
    let pd = net.pred_dim
    let jd = net.joint_dim
    rv_release_scratch()
    let cap = (n_frames + 63l) / 64l * 64l
    r.st_bytes = int64(GK_RNNT_ST_WORDS) * 4l
    r.hb_bytes = 2l * net.n_layers * pd * 4l
    r.cb_bytes = net.n_layers * pd * 4l
    r.xin_bytes = pd * 4l
    r.pred_bytes = jd * 4l
    r.lg_bytes = int64(GK_RNNT_FRAMES) * rnnt_joint_blocks(n_logits) * RNNT_JOINT_BLOCK * 4l
    r.toks_bytes = rnnt_dev_cap(net, cap) * int64(GK_RNNT_TOK_WORDS) * 4l
    r.encp_bytes = cap * jd * 4l
    r.rows_bytes = cap * d * 4l
    if (!vk_range_ok(max(max(r.toks_bytes, r.lg_bytes), max(r.encp_bytes, r.rows_bytes)))) {
        return rv_decline(VulkanRnntDecline.shape)
    }
    r.oom = false
    r.st_dev = rv_call_buf(r.st_bytes)
    r.hb_dev = rv_call_buf(r.hb_bytes)
    r.cb_dev = rv_call_buf(r.cb_bytes)
    r.xin_dev = rv_call_buf(r.xin_bytes)
    r.pred_dev = rv_dev_buf(r.pred_bytes)
    r.lg_dev = rv_dev_buf(r.lg_bytes)
    r.toks_dev = rv_dev_buf(r.toks_bytes)
    r.encp_dev = rv_call_buf(r.encp_bytes)
    r.rows_dev = d > 0l ? rv_call_buf(r.rows_bytes) : 0ul   //! a net with no encoder half has no rows, and a 0-byte buffer faults the driver
    r.host = soft_host_buf(r.oom, max(r.st_bytes + r.toks_bytes, r.hb_bytes + r.cb_bytes + r.xin_bytes), true)
    if (r.oom) {
        rv_release_scratch()
        return rv_decline(VulkanRnntDecline.memory)
    }
    r.s_gates = set_rnnt_gates_cls(fixed_array(r.st_dev, r.wh_dev, r.wf_dev, r.xin_dev, r.hb_dev, r.cb_dev),
        fixed_array(r.st_bytes, r.wh_bytes, r.wf_bytes, r.xin_bytes, r.hb_bytes, r.cb_bytes), fixed_array(RV_ST, 0u, 0u, RV_XIN, RV_HB, RV_CB))
    r.s_proj = set_rnnt_proj_cls(fixed_array(r.st_dev, r.wh_dev, r.wf_dev, r.hb_dev, r.pred_dev),
        fixed_array(r.st_bytes, r.wh_bytes, r.wf_bytes, r.hb_bytes, r.pred_bytes), fixed_array(RV_ST, 0u, 0u, RV_HB, RV_PRED))
    r.s_joint = set_rnnt_joint_cls(fixed_array(r.st_dev, r.encp_dev, r.wf_dev, r.pred_dev, r.jw_dev, r.lg_dev),
        fixed_array(r.st_bytes, r.encp_bytes, r.wf_bytes, r.pred_bytes, r.jw_bytes, r.lg_bytes), fixed_array(RV_ST, RV_ENCP, 0u, RV_PRED, 0u, RV_LG))
    r.s_pick = set_rnnt_pick_cls(fixed_array(r.st_dev, r.lg_dev, r.wf_dev, r.xin_dev, r.toks_dev),
        fixed_array(r.st_bytes, r.lg_bytes, r.wf_bytes, r.xin_bytes, r.toks_bytes), fixed_array(RV_ST, RV_LG, 0u, RV_XIN, RV_TOKS))
    r.cap_frames = cap
    r.cap_shape = shape
    return true
}

//! The clip's start uploaded: the state words (the first round's predictor step asked, the duration table), the hidden pair and
//! the cells zeroed, the first input - zeros, or the blank's embedding row.
def private rv_state_init(net : RnntNet) {
    var r & = g_rv
    rnnt_dev_state_words(net, unsafe(reinterpret<uint?>(r.host.mapped)))
    upload_region_at(r.st_dev, 0l, unsafe(reinterpret<uint8 const?>(r.host.mapped)), r.st_bytes)
    unsafe {
        var zp = reinterpret<float?>(r.host.mapped)
        for (i in range64((r.hb_bytes + r.cb_bytes) / 4l)) {
            zp[i] = 0.0
        }
        rnnt_dev_first_input(net, zp + (r.hb_bytes + r.cb_bytes) / 4l)
        let hp = reinterpret<uint8 const?>(r.host.mapped)
        upload_region_at(r.hb_dev, 0l, hp, r.hb_bytes)
        upload_region_at(r.cb_dev, 0l, hp + r.hb_bytes, r.cb_bytes)
        upload_region_at(r.xin_dev, 0l, hp + r.hb_bytes + r.cb_bytes, r.xin_bytes)
    }
}

//! A round's dispatches: each layer's gates, joint.pred's projection, the joint window, the pick.
def private rv_round(raw : VkCommandBuffer; var h : VkHaz; net : RnntNet; ka : GkRnntArgs; n_logits : int64) {
    var r & = g_rv
    for (l in range64(net.n_layers)) {
        var kl = rnnt_dev_layer_args(ka, r.lay, l)
        enc_rnnt_gates_cls(raw, h, r.s_gates, kl, (net.pred_dim + 7l) / 8l)
    }
    var kr = ka
    enc_rnnt_proj_cls(raw, h, r.s_proj, kr, (net.joint_dim + 7l) / 8l)
    enc_rnnt_joint_cls(raw, h, r.s_joint, kr, rnnt_joint_blocks(n_logits) * 4l)
    enc_rnnt_pick_cls(raw, h, r.s_pick, kr, 1l)
}

//! a projecting decode that found no encode rows of its generation on the device uploads them; said once a seat, by reason
[cold_path]
def private rv_handoff_miss_say(gen : uint64) {
    g_rv.miss_said = true
    let why = gen == 0ul ? "the session recorded no device encode" : "another seat ran since the encode, or the rows are another clip's"
    to_log(LOG_INFO, "dasLLAMA vulkan rnnt: the decode uploads its encoder rows - {why}\n")
}

[cold_path]
def private rv_announce(pd, jd, n_logits : int64) {
    g_rv.served_said = true
    to_log(LOG_INFO, "dasLLAMA vulkan rnnt: the transducer decode serves on the device (pred {pd}, joint {jd}, {n_logits} logits)\n")
}

//! Whether the Vulkan seat projects a lane's encoder half: the f32 plane's, through the exact f32 tile.
def private vulkan_rnnt_projects(enc : RnntEnc) : bool => g_rv_env && !enc.q8

//! The transducer's greedy decode on the device, the Metal seat's form on the Vulkan stamps: the joint's encoder projection off
//! the encode's own device rows (the exact f32 tile) or the family's projection uploaded, then command buffers of rounds until
//! the pick marks the clip done; the state words read back a command buffer, the token records once.
[hot_path, arch(at="../ARCHITECTURE_MEDIA_ASR.md#rnnt-device-decode")]
def private vulkan_rnnt_greedy(net : RnntNet; var s : RnntState; enc : RnntEnc; x, encp : array<float>; n_frames : int64;   // nolint:STYLE038 — the dispatch chain is the shape
                               @scratch var toks : array<RnntToken>) : bool {
    if (!g_rv_env) {
        return rv_decline(VulkanRnntDecline.knob)
    }
    let jd = net.joint_dim
    let d = enc.d
    let n_logits = rnnt_n_logits(net)
    let projects = !enc.q8
    if (!rnnt_dev_shape_ok(net, enc, 1l)) {
        return rv_decline(VulkanRnntDecline.shape)
    }
    if (!vulkan_tower_arms() || !ensure_rnnt_gates_cls() || !ensure_rnnt_proj_cls() || !ensure_rnnt_joint_cls() || !ensure_rnnt_pick_cls()
            || (projects && !ensure_f32_gemm_cls()) || rnnt_dev_faulted(RnntDevFault.device)) {
        return rv_decline(VulkanRnntDecline.device)
    }
    if (!rv_attach(net, s, enc) || !rv_scratch(net, max(n_frames, 1l), n_logits, d)) {
        return false
    }
    if (n_frames <= 0l) {
        g_rv_decodes++
        return true
    }
    var r & = g_rv
    rv_state_init(net)
    var rows_dev = 0ul
    var handoff = false
    if (projects) {
        rows_dev = vulkan_tower_fc_rows(enc.gen, n_frames * d * 4l)
        handoff = rows_dev != 0ul
        if (!handoff) {
            if (!r.miss_said) {
                rv_handoff_miss_say(enc.gen)
            }
            upload_region_at(r.rows_dev, 0l, unsafe(addr<uint8 const?>(x[0])), n_frames * d * 4l)
            rows_dev = r.rows_dev
        }
    } else {
        upload_region_at(r.encp_dev, 0l, unsafe(addr<uint8 const?>(encp[0])), n_frames * jd * 4l)
    }
    var s_enc : VkDescriptorSet
    if (projects) {
        s_enc = set_f32_gemm_cls(fixed_array(r.wenc_dev, rows_dev, r.encp_dev), fixed_array(r.wenc_bytes, n_frames * d * 4l, r.encp_bytes),
            fixed_array(0u, RV_ROWS, RV_ENCP))
    }
    var pc_enc = F32GemmArgs(n = uint(d), d = uint(jd), rows = uint(n_frames), woff = 0u, wstride = uint(d), boff = 0u, xoff = 0u, xstride = uint(d),
        yoff = 0u, ystride = uint(jd))
    let ka = rnnt_dev_args(net, r.lay, n_frames, projects)
    let run = rnnt_dev_rounds(net, n_frames, unsafe(reinterpret<uint const?>(r.host.mapped)), g_rv_rate) $(rounds : int64; first : bool) : bool {
        vt_chain_one_shot("rnnt decode", net.n_layers, n_frames) $(raw : VkCommandBuffer; var h : VkHaz) {   //! a failed submit panics in the chain's own check
            if (first && projects) {
                enc_f32_gemm_cls(raw, h, s_enc, pc_enc, f32_gemm_wgs(n_frames, jd))
            }
            for (_r in range64(rounds)) {
                rv_round(raw, h, net, ka, n_logits)
            }
            vhz_dep(raw, h, RV_ST, 0u, true)
            cmd_copy_range(raw, r.st_dev, 0l, r.host.buf, 0l, r.st_bytes)
        }
        return true
    }
    g_rv_cbs += run.cbs
    if (!run.ok) {
        return rv_decline(VulkanRnntDecline.overrun)
    }
    let ntok = run.p.ntok
    g_rv_words |> resize(ntok * int(GK_RNNT_TOK_WORDS))
    if (ntok > 0) {
        readback_range(r.toks_dev, RV_TOKS, r.host, unsafe(addr<float?>(g_rv_words[0])), long_length(g_rv_words) * 4l)
        rnnt_dev_land_tokens(unsafe(addr<uint const?>(g_rv_words[0])), ntok, toks)
    }
    if (!r.served_said) {
        rv_announce(net.pred_dim, jd, n_logits)
    }
    g_rv_decodes++
    g_rv_tokens += int64(ntok)
    g_rv_rounds += run.p.rounds
    g_rv_steps += run.p.steps
    if (projects) {
        g_rv_projects++
    }
    if (handoff) {
        g_rv_handoffs++
    }
    return true
}

// ===== GigaAM's CTC head: the head GEMM over the head's own plane and the pick =====

enum private VulkanCtcDecline {
    knob     //! the tower knob is off (DASLLAMA_VK_TOWER=0, or set_vulkan_tower(false))
    shape    //! d off the f16 GEMM's 64-wide K lattice, no frames, or a buffer past the device's range
    device   //! the tier has no device, a class failed to build, or an f16 head on a device with no f16 GEMM tile
    memory   //! a device or host allocation failed
    flavor   //! the model is not the Vulkan-flavored image: each backend serves its own (`#tower-dev-block`)
}

//! the hazard bits of the CTC seat's buffers
let private CV_ROWS = 0x1u
let private CV_XH = 0x2u
let private CV_LG = 0x4u
let private CV_PK = 0x8u

//! The CTC seat's device objects: the head's planes laid once a model (`key`: the f16 head's padded halfword rows off ``wblob``,
//! or the f32 head's rows off ``blob``, and the bias row), and the per-clip planes sized to the longest clip yet.
struct private CvResident {
    key : uint64
    w_dev : uint64
    b_dev : uint64
    w_bytes : int64
    b_bytes : int64
    cap_frames : int64
    cap_shape : uint64
    rows_dev : uint64      //! the encoder rows uploaded where no device encode of their generation is held
    xh_dev : uint64        //! the f16 head's half feed, [cap + TILE_READ_SLACK][d]
    lg_dev : uint64        //! the head GEMM's rows [cap][ncp]
    pk_dev : uint64        //! a frame's record [cap][2]: the class, plog's bits
    host : HostBuf
    rows_bytes : int64
    xh_bytes : int64
    lg_bytes : int64
    pk_bytes : int64
    oom : bool
    served_said : bool
    miss_said : bool
}

var private g_cv : CvResident
var private g_cv_declines : VkDeclineCounter
var private g_cv_heads = 0l      //! CTC heads served on the device
var private g_cv_handoffs = 0l   //! ... among them off the encode's own device rows
var private g_cv_uploads = 0l

//! Engage counters: `heads` served on the device, `handoffs` among them off the encode's device rows, `uploads` the head
//! uploads, `declines` every decline but the knob's.
def vulkan_ctc_stats : tuple<heads : int64; handoffs : int64; uploads : int64; declines : int64> {
    return (heads = g_cv_heads, handoffs = g_cv_handoffs, uploads = g_cv_uploads,
        declines = g_cv_declines.total - (g_cv_declines.counts?["{VulkanCtcDecline.knob}"] ?? 0l))
}

//! The CTC seat's declines-by-reason table (a copy), keyed by the decline's name, the knob's among them.
def vulkan_ctc_declines : table<string; int64> => <- clone_to_move(g_cv_declines.counts)

[cold_path]
def private cv_decline(why : VulkanCtcDecline) : bool {
    let words = (why == VulkanCtcDecline.knob ? VK_DECLINE_WORDS_TOWER_KNOB
        : why == VulkanCtcDecline.shape ? "d off the f16 GEMM's 64-wide lattice, no frames, or a buffer past the device's range"
        : why == VulkanCtcDecline.memory ? VK_DECLINE_WORDS_MEMORY
        : why == VulkanCtcDecline.flavor ? "the model is not the Vulkan-flavored image" : VK_DECLINE_WORDS_DEVICE)
    return g_cv_declines |> note("the CTC head", "{why}", "the head stays on the CPU: {words}")
}

[cold_path]
def private cv_release_scratch {
    var r & = g_cv
    release_device_buf(r.rows_dev)
    release_device_buf(r.xh_dev)
    release_device_buf(r.lg_dev)
    release_device_buf(r.pk_dev)
    if (r.host.buf != 0ul) {
        destroy_host_buf(r.host)
    }
    r.host = HostBuf()
    r.cap_frames = 0l
    r.cap_shape = 0ul
}

[cold_path]
def private cv_release {
    cv_release_scratch()
    release_device_buf(g_cv.w_dev)
    release_device_buf(g_cv.b_dev)
    cv_forget()
}

//! the handles forgotten (the model drop's sweep destroys the objects); the next served head uploads again
[cold_path]
def private cv_forget {
    g_cv = CvResident()
    g_cv_declines |> unsay("the CTC head")
}

//! Release every device object the CTC seat owns (the tests' leak rail and the model-switch seat).
def vulkan_ctc_shutdown {
    cv_release()
}

//! the head's padded rows: the f16 head's `ncp` halfword rows (the zero pad rows the mint wrote among them), or the f32 head's
//! `nc` rows
def private cv_head_n(m : GigaamModel) : int64 => (m.ctc_f16 ? round_up(m.n_classes, GIGAAM_CTC_ROW_TILE) : m.n_classes) * m.d_model

//! the head's planes uploaded once a model, as the image holds them
def private cv_attach(m : GigaamModel) : bool {
    let nw = cv_head_n(m)
    var key = hash_combine64(m.ctc_f16 ? intptr(unsafe(reinterpret<void?>(m.wblob.p))) : intptr(unsafe(reinterpret<void?>(m.blob.p))),
        hash_combine64(uint64(m.ctc_w), uint64(m.ctc_b)))
    key = hash_combine64(key, hash_combine64(uint64(nw), uint64(m.n_classes)))
    return true if (g_cv.key == key && g_cv.w_dev != 0ul)
    return cv_attach_build(m, key, nw)
}

[cold_path]
def private cv_attach_build(m : GigaamModel; key : uint64; nw : int64) : bool {
    var r & = g_cv
    cv_release()
    r.oom = false
    r.w_bytes = nw * (m.ctc_f16 ? 2l : 4l)
    r.b_bytes = m.n_classes * 4l
    if (!vk_range_ok(r.w_bytes)) {
        return cv_decline(VulkanCtcDecline.shape)
    }
    r.w_dev = soft_device_buf(r.oom, r.w_bytes)
    r.b_dev = soft_device_buf(r.oom, r.b_bytes)
    if (r.oom) {
        cv_release()
        return cv_decline(VulkanCtcDecline.memory)
    }
    if (m.ctc_f16) {
        upload_region_at(r.w_dev, 0l, unsafe(reinterpret<uint8 const?>(plane_at(m.wblob, m.ctc_w))), r.w_bytes)
    } else {
        upload_region_at(r.w_dev, 0l, unsafe(reinterpret<uint8 const?>(plane_at(m.blob, m.ctc_w))), r.w_bytes)
    }
    upload_region_at(r.b_dev, 0l, unsafe(reinterpret<uint8 const?>(plane_at(m.blob, m.ctc_b))), r.b_bytes)
    r.key = key
    g_cv_uploads++
    return true
}

//! the per-clip planes for `n_frames` (a whole 64-frame step past the largest clip yet)
def private cv_scratch(n_frames, ncp, d : int64; f16 : bool) : bool {
    let shape = hash_combine64(hash_combine64(uint64(ncp), uint64(d)), uint64(f16 ? 1 : 0))
    return true if (g_cv.cap_frames >= n_frames && g_cv.cap_shape == shape && g_cv.lg_dev != 0ul)
    return cv_scratch_build(n_frames, ncp, d, f16, shape)
}

[cold_path]
def private cv_scratch_build(n_frames, ncp, d : int64; f16 : bool; shape : uint64) : bool {
    var r & = g_cv
    cv_release_scratch()
    let cap = (n_frames + 63l) / 64l * 64l
    r.rows_bytes = cap * d * 4l
    r.xh_bytes = f16 ? (cap + TILE_READ_SLACK) * d * 2l : 0l
    r.lg_bytes = cap * ncp * 4l
    r.pk_bytes = cap * 2l * 4l
    if (!vk_range_ok(max(r.rows_bytes, r.lg_bytes))) {
        return cv_decline(VulkanCtcDecline.shape)
    }
    r.oom = false
    r.rows_dev = soft_call_buf(r.oom, r.rows_bytes)
    r.xh_dev = f16 ? soft_device_buf(r.oom, r.xh_bytes) : 0ul
    r.lg_dev = soft_device_buf(r.oom, r.lg_bytes)
    r.pk_dev = soft_device_buf(r.oom, r.pk_bytes)
    r.host = soft_host_buf(r.oom, r.pk_bytes, true)
    if (r.oom) {
        cv_release_scratch()
        return cv_decline(VulkanCtcDecline.memory)
    }
    r.cap_frames = cap
    r.cap_shape = shape
    return true
}

//! a head that found no encode rows of its generation on the device uploads them; said once a seat, by reason
[cold_path]
def private cv_handoff_miss_say(gen : uint64) {
    g_cv.miss_said = true
    let why = gen == 0ul ? "the session recorded no device encode" : "another seat ran since the encode, or the rows are another clip's"
    to_log(LOG_INFO, "dasLLAMA vulkan ctc: the head uploads its encoder rows - {why}\n")
}

[cold_path]
def private cv_announce(nc, d : int64; f16 : bool) {
    g_cv.served_said = true
    to_log(LOG_INFO, "dasLLAMA vulkan ctc: the CTC head serves on the device ({nc} classes over {d}, {f16 ? "the f16 GEMM over the carrier's halfwords" : "the f32 tile"})\n")
}

//! GigaAM's CTC head as ONE command buffer on the Vulkan stamps (`#gigaam-planes`): the encode's device rows by generation (else
//! ``st.x`` uploaded), the head GEMM over its own plane, then the pick (`CtcPick`) - each frame's biased argmax and plog read
//! back into ``st.picks`` / ``st.plogs``. False declines.
[hot_path, arch(at="../ARCHITECTURE_MEDIA_ASR.md#gigaam-planes")]
def private vulkan_gigaam_ctc(m : GigaamModel; var st : GigaamState; n_frames : int64) : bool {
    if (!vulkan_tower_on()) {
        return cv_decline(VulkanCtcDecline.knob)
    }
    if (m.dev_backend != TowerDev.vulkan) {
        return cv_decline(VulkanCtcDecline.flavor)
    }
    let d = m.d_model
    let nc = m.n_classes
    let ncp = round_up(nc, GIGAAM_CTC_ROW_TILE)
    let f16 = m.ctc_f16
    if (n_frames < 1l || d % 64l != 0l || long_length(st.x) < n_frames * d) {
        return cv_decline(VulkanCtcDecline.shape)
    }
    let khr = f16_gemm_khr()
    if (!vulkan_tower_arms() || !ensure_ctc_pick_cls() || (f16 ? !(f16_gemm_on() && ensure_tower_clamp_cvt_cls() && f16_gemm_ensure(khr))
            : !ensure_f32_gemm_cls())) {
        return cv_decline(VulkanCtcDecline.device)
    }
    if (!cv_attach(m) || !cv_scratch(n_frames, ncp, d, f16)) {
        return false
    }
    let r & = g_cv
    let bx = n_frames * d * 4l
    var rows_dev = vulkan_tower_fc_rows(st.enc_gen, bx)
    let handoff = rows_dev != 0ul
    if (!handoff) {
        if (!r.miss_said) {
            cv_handoff_miss_say(st.enc_gen)
        }
        upload_region_at(r.rows_dev, 0l, unsafe(addr<uint8 const?>(st.x[0])), bx)
        rows_dev = r.rows_dev
    }
    var s_pick = set_ctc_pick_cls(fixed_array(r.lg_dev, r.b_dev, r.pk_dev), fixed_array(r.lg_bytes, r.b_bytes, r.pk_bytes), fixed_array(CV_LG, 0u, CV_PK))
    var pc_pick = GkCtcPickArgs(nc = uint(nc), ldl = uint(ncp), boff = 0u)
    let pk_n = n_frames * 2l * 4l
    if (f16) {
        var s_cvt = set_tower_clamp_cvt_cls(fixed_array(rows_dev, r.xh_dev), fixed_array(bx, r.xh_bytes), fixed_array(CV_ROWS, CV_XH))
        var s_mm = f16_gemm_set(khr, fixed_array(r.w_dev, r.xh_dev, r.lg_dev), fixed_array(r.w_bytes, r.xh_bytes, r.lg_bytes), fixed_array(0u, CV_XH, CV_LG))
        var pc_cvt = gk_f16_rows_args(uint(n_frames * d))
        var pc_mm = F16GemmArgs(n = uint(d), d = uint(nc), rows = uint(n_frames), woff = 0u, ybase = 0u, ystride = uint(ncp), ksplit = 0u)
        vt_chain_one_shot("gigaam ctc", 1l, n_frames) $(raw : VkCommandBuffer; var h : VkHaz) {
            enc_tower_clamp_cvt_cls(raw, h, s_cvt, pc_cvt, (n_frames * d / 4l + 255l) / 256l)
            f16_gemm_enc(khr, raw, h, s_mm, pc_mm, n_frames, nc)
            enc_ctc_pick_cls(raw, h, s_pick, pc_pick, n_frames)
            vhz_dep(raw, h, CV_PK, 0u, true)
            cmd_copy_range(raw, r.pk_dev, 0l, r.host.buf, 0l, pk_n)
        }
    } else {
        var s_mm = set_f32_gemm_cls(fixed_array(r.w_dev, rows_dev, r.lg_dev), fixed_array(r.w_bytes, bx, r.lg_bytes), fixed_array(0u, CV_ROWS, CV_LG))
        var pc_mm = F32GemmArgs(n = uint(d), d = uint(nc), rows = uint(n_frames), woff = 0u, wstride = uint(d), boff = 0u, xoff = 0u, xstride = uint(d),
            yoff = 0u, ystride = uint(ncp))
        vt_chain_one_shot("gigaam ctc", 1l, n_frames) $(raw : VkCommandBuffer; var h : VkHaz) {
            enc_f32_gemm_cls(raw, h, s_mm, pc_mm, f32_gemm_wgs(n_frames, nc))
            enc_ctc_pick_cls(raw, h, s_pick, pc_pick, n_frames)
            vhz_dep(raw, h, CV_PK, 0u, true)
            cmd_copy_range(raw, r.pk_dev, 0l, r.host.buf, 0l, pk_n)
        }
    }
    unsafe {
        let pp = reinterpret<uint const?>(r.host.mapped)
        for (t in range64(n_frames)) {
            st.picks[t] = int(pp[2l * t])
            st.plogs[t] = reinterpret<float>(pp[2l * t + 1l])
        }
    }
    if (!r.served_said) {
        cv_announce(nc, d, f16)
    }
    g_cv_heads++
    if (handoff) {
        g_cv_handoffs++
    }
    return true
}

[init]
def dasllama_vulkan_asr_dec_register {
    g_wd_env = g_env_vulkan.vk_wdec
    g_rv_env = g_env_vulkan.vk_rnnt
    register_vk_drop_hook(@@wd_forget)
    register_vk_drop_hook(@@rv_forget)
    register_vk_drop_hook(@@cv_forget)
    static_if (!typeinfo builtin_module_exists(das_metal)) {
        register_whisper_cross_kv_gpu(@@vulkan_whisper_cross_kv)
        register_whisper_decode_gpu(@@vulkan_whisper_decode)
        register_whisper_flavor(TowerDev.vulkan, @@vulkan_whisper_serves_model, @@vulkan_whisper_bake)
        register_rnnt_gpu(@@vulkan_rnnt_greedy, @@vulkan_rnnt_projects)
        register_gigaam_gpu_ctc(@@vulkan_gigaam_ctc)
    }
}
