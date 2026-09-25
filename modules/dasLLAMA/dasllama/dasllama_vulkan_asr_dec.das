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
require dasllama/dasllama_vulkan_common
require dasllama/dasllama_vulkan_classes
require dasllama/dasllama_vulkan_tower

//! The whisper ASR-decoder Vulkan driver, q8-native: the per-window cross-KV and the decode step (`ARCHITECTURE_GPU_TOWER.md`,
//! the Vulkan ASR-decoder section); the Metal ASR-decoder driver fills the same hooks on an Apple build.

var private g_wd_env = true
var private g_wd_declines : VkDeclineCounter
var private g_wd_windows = 0l
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
}

let private WD_ROW_CAP = 8l          //! the rows a decode batch may carry: the N-column GEMV's widest form
let private WD_LAYER_SITES = 6l             //! a layer's GEMV sites: qkv (three regions), o, cq, co, fc1, fc2
let private WD_S_QKV = 0l
let private WD_S_O = 1l
let private WD_S_CQ = 2l
let private WD_S_CO = 3l
let private WD_S_FC1 = 4l
let private WD_S_FC2 = 5l
let private WD_SITE_META_BYTES = 64l      //! a GEMV site's region pairs

//! the hazard bits of the driver's buffers
let private WD_X = 0x1u
let private WD_XB = 0x2u
let private WD_XQ = 0x4u
let private WD_QKV = 0x8u
let private WD_CQ = 0x10u
let private WD_ATT = 0x20u
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
    x_dev : uint64        //! the step's rows: the residual stream, the normed rows, the Q8_0 feed
    xb_dev : uint64
    xq_dev : uint64
    xs_dev : uint64
    qkv_dev : uint64
    cq_dev : uint64
    att_dev : uint64
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
    ta_gemm : int64           //! the cross-KV GEMM record's rows: `ta` rounded up to the tile's token column (`vt_tile_rows`); the projection plane holds them
    kx_dev : uint64
    vx_dev : uint64
    xkv_meta : HostBuf
    xkv_meta_bytes : int64
    xhost : HostBuf       //! the step's rows in, the logits out
    kvhost : HostBuf
    @scratch @exact_size site_meta : array<HostBuf>   //! a GEMV site's region pairs: [l x WD_LAYER_SITES], the tied embedding last
    @scratch @exact_size s_site : array<VkDescriptorSet>
    s_ln : VkDescriptorSet
    s_post : VkDescriptorSet
    s_bias_h : VkDescriptorSet
    s_rq_xb : VkDescriptorSet
    s_rq_att : VkDescriptorSet
    s_rq_h : VkDescriptorSet
    s_kv_k : VkDescriptorSet
    s_kv_v : VkDescriptorSet
    s_attn_self : VkDescriptorSet
    s_attn_cross : VkDescriptorSet
    s_comb : VkDescriptorSet
    s_xfeed : VkDescriptorSet
    s_xkv : VkDescriptorSet
    s_kvx_k : VkDescriptorSet
    s_kvx_v : VkDescriptorSet
    oom : bool
    served_said : bool
}

var private g_wd : WdResident
var private g_wd_window_key = 0ul     //! the live window: ONE DecoderState at a time, claimed by a served cross-KV
var private g_wd_window_live = false
//! the served window's CPU-layout planes (kx, vx) are read back only when a CPU reader can need them: the pending readback names
//! the decoder state's host arrays and its byte count, and `wd_flush_cross_kv` lands it at the decode step's declines, at the next
//! window of another state, and at the release - the served loop never pays the 61 MB copy
var private g_wd_pend_kx : float?
var private g_wd_pend_vx : float?
var private g_wd_pend_bytes = 0l
var private g_wd_pend_key = 0ul
var private @scratch @exact_size g_wd_maps : array<uint>

//! Engage / decline counters: `windows` = cross-KV windows served, `steps` = decode batches, `tokens` = rows across them,
//! `declines` = every decline but the knob's (a pinned-off driver is policy, not a failure).
def vulkan_wdec_stats : tuple<windows : int64; steps : int64; tokens : int64; declines : int64> {
    let policy = (g_wd_declines.counts?["{VulkanWdecDecline.knob}"] ?? 0l)
    return (windows = g_wd_windows, steps = g_wd_steps, tokens = g_wd_tokens, declines = g_wd_declines.total - policy)
}

//! The declines-by-reason table (a copy), keyed by `VulkanWdecDecline` value names.
def vulkan_wdec_declines : table<string; int64> {
    return <- clone_to_move(g_wd_declines.counts)
}

//! The step-time split across every served decode batch: the submit's wall against the host encode, microseconds.
def vulkan_wdec_times : tuple<us_gpu : int64; us_encode : int64> {
    return (us_gpu = g_wd_us_gpu, us_encode = g_wd_us_encode)
}

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
        return "a device allocation failed"
    } elif (why == VulkanWdecDecline.rows) {
        return "the window's first batch is wider than the step's {WD_ROW_CAP}-row cap"
    }
    return "the tier has no device, or a class failed to build"
}

[cold_path]
def private wd_decline(why : VulkanWdecDecline) : bool
    => g_wd_declines |> note("the whisper decoder", "{why}", "the decoder stays on the CPU chain: {wd_decline_words(why)}")

[cold_path]
def private wd_dev_buf(bytes : int64) : uint64 => soft_device_buf(g_wd.oom, bytes)

[cold_path]
def private wd_host_buf(bytes : int64; cached : bool) : HostBuf => soft_host_buf(g_wd.oom, bytes, cached)

[cold_path]
//! the pending window's kx / vx CPU layouts copied off the device planes into the decoder state that owns them (a no-op with none pending)
def private wd_flush_cross_kv {
    if (g_wd_pend_bytes == 0l) {
        return
    }
    var r & = g_wd
    let bkxf = g_wd_pend_bytes
    cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
        vhz_dep(raw, h, WD_KX | WD_VX, 0u, true)
        cmd_copy_range(raw, r.kx_dev, 0l, r.kvhost.buf, 0l, bkxf)
        cmd_copy_range(raw, r.vx_dev, 0l, r.kvhost.buf, bkxf, bkxf)
    }
    unsafe {
        memcpy(reinterpret<void?>(g_wd_pend_kx), r.kvhost.mapped, bkxf)
        memcpy(reinterpret<void?>(g_wd_pend_vx), reinterpret<uint8?>(r.kvhost.mapped) + bkxf, bkxf)
    }
    g_wd_pend_bytes = 0l
    g_wd_pend_kx = null
    g_wd_pend_vx = null
}

def private wd_release {
    var r & = g_wd
    wd_flush_cross_kv()   //! the device planes die with the release: a pending window lands in its state first
    if (r.key != 0ul) {
        for (b in fixed_array(r.wq_dev, r.ws_dev, r.norms_dev, r.kxr_dev, r.vxr_dev, r.kc_dev, r.vc_dev, r.x_dev, r.xb_dev, r.xq_dev, r.xs_dev,
                              r.qkv_dev, r.cq_dev, r.att_dev, r.yo_dev, r.h_dev, r.pms_dev, r.py_dev, r.logits_dev, r.xin_dev, r.xh_dev, r.xkv_q_dev,
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

//! the handles forgotten (the model drop's sweep destroys the objects); the next served window uploads again
[cold_path]
def private wd_forget {
    var r & = g_wd
    r.key = 0ul
    r.oom = false
    r.served_said = false
    r.site_meta |> clear()
    r.s_site |> clear()
    r.xkv_meta_bytes = 0l
    g_wd_window_key = 0ul
    g_wd_window_live = false
}

//! Release every device object the driver owns (the tests' leak rail and the model-switch seat); the next served window
//! re-attaches.
def vulkan_wdec_shutdown {
    wd_release()
}

[cold_path]
def private wd_gemv_set(meta : HostBuf; y_dev : uint64; y_bytes : int64; y_bit : uint) : VkDescriptorSet {
    let r & = g_wd
    let bq = WD_ROW_CAP * 4l * r.d
    return gemv_cls_set(int(KqFmt.q8), fixed_array(r.wq_dev, r.ws_dev, meta.buf, r.xq_dev, r.xs_dev, y_dev),
        fixed_array(r.wq_bytes, r.ws_bytes, WD_SITE_META_BYTES, bq, bq / kq_elems(KqFmt.q8) * 4l, y_bytes), fixed_array(0u, 0u, 0u, WD_XQ, WD_XQ, y_bit))
}

//! a site's region pairs: each region's first weight block, its activation rows at block 0
[cold_path]
def private wd_site_meta(woffs : array<int64>) : HostBuf {
    let q8e = kq_elems(KqFmt.q8)   // nolint:LINT021 - the element offset divides in 64-bit ahead of the narrowing store
    let hb = wd_host_buf(WD_SITE_META_BYTES, false)
    if (hb.buf != 0ul) {
        unsafe {
            var mp = reinterpret<uint?>(hb.mapped)
            for (i in range(length(woffs))) {
                mp[i * 2] = uint(woffs[i] / q8e)
                mp[i * 2 + 1] = 0u
            }
        }
    }
    return hb
}

[cold_path]
def private wd_xkv_set : VkDescriptorSet {
    let r & = g_wd
    let bt = r.ta_gemm * r.d * 4l
    if (r.xkv_tile.tc > 0l) {
        return cm2_cls_set(int(KqFmt.q8), r.xkv_tile.tc, fixed_array(r.wq_dev, r.ws_dev, r.xkv_meta.buf, r.xh_dev, r.xkv_proj_dev),
            fixed_array(r.wq_bytes, r.ws_bytes, r.xkv_meta_bytes, (r.ta_gemm + TILE_READ_SLACK) * r.d * 2l, bt), fixed_array(0u, 0u, 0u, WD_XFEED, WD_TMP))
    }
    let bq = r.ta * r.d
    return q8_batch_cls_set(r.xkv_tile.v, fixed_array(r.wq_dev, r.ws_dev, r.xkv_meta.buf, r.xkv_q_dev, r.xkv_s_dev, r.xkv_proj_dev),
        fixed_array(r.wq_bytes, r.ws_bytes, r.xkv_meta_bytes, bq, bq / kq_elems(KqFmt.q8) * 4l, bt), fixed_array(0u, 0u, 0u, WD_XFEED, WD_XFEED, WD_TMP))
}

[cold_path]
def private wd_sets {
    var r & = g_wd
    let d = r.d
    let q8e = kq_elems(KqFmt.q8)
    let bx = WD_ROW_CAP * d * 4l
    let bq = WD_ROW_CAP * 4l * d
    let bs = bq / q8e * 4l
    let bh = WD_ROW_CAP * 4l * d * 4l
    let bkc = r.nl * d * r.tmax * 2l
    let bkx = r.nl * d * r.ta * 2l
    let bkxf = r.nl * d * r.ta * 4l
    let nch_cap = (max(r.ta, r.tmax) + int64(WDEC_CHUNK) - 1l) / int64(WDEC_CHUNK)
    let bpms = nch_cap * r.hh * WD_ROW_CAP * 2l * 4l
    let bpy = nch_cap * r.hh * WD_ROW_CAP * r.hs * 4l
    let bt = r.ta * r.d * 4l
    r.s_ln = set_tower_ln_cls(fixed_array(r.x_dev, r.xb_dev, r.norms_dev), fixed_array(bx, bx, r.norms_bytes), fixed_array(WD_X, WD_XB, 0u))
    r.s_post = set_tower_post_add_ln_cls(fixed_array(r.x_dev, r.yo_dev, r.xb_dev, r.norms_dev), fixed_array(bx, bx, bx, r.norms_bytes), fixed_array(WD_X, WD_YO, WD_XB, 0u))
    r.s_bias_h = set_tower_bias_act_cls(fixed_array(r.h_dev, r.norms_dev), fixed_array(bh, r.norms_bytes), fixed_array(WD_H, 0u))
    r.s_rq_xb = set_tower_clamp_rq_cls(fixed_array(r.xb_dev, r.xq_dev, r.xs_dev), fixed_array(bx, bq, bs), fixed_array(WD_XB, WD_XQ, WD_XQ))
    r.s_rq_att = set_tower_clamp_rq_cls(fixed_array(r.att_dev, r.xq_dev, r.xs_dev), fixed_array(bx, bq, bs), fixed_array(WD_ATT, WD_XQ, WD_XQ))
    r.s_rq_h = set_tower_clamp_rq_cls(fixed_array(r.h_dev, r.xq_dev, r.xs_dev), fixed_array(bh, bq, bs), fixed_array(WD_H, WD_XQ, WD_XQ))
    r.s_kv_k = set_tower_wdec_kv_cls(fixed_array(r.qkv_dev, r.kc_dev, r.kx_dev, r.norms_dev), fixed_array(3l * bx, bkc, bkxf, r.norms_bytes), fixed_array(WD_QKV, WD_KC, WD_KX, 0u))
    r.s_kv_v = set_tower_wdec_kv_cls(fixed_array(r.qkv_dev, r.vc_dev, r.vx_dev, r.norms_dev), fixed_array(3l * bx, bkc, bkxf, r.norms_bytes), fixed_array(WD_QKV, WD_VC, WD_VX, 0u))
    r.s_attn_self = set_tower_wdec_attn_part_cls(fixed_array(r.qkv_dev, r.kc_dev, r.vc_dev, r.pms_dev, r.py_dev, r.norms_dev),
        fixed_array(3l * bx, bkc, bkc, bpms, bpy, r.norms_bytes), fixed_array(WD_QKV, WD_KC, WD_VC, WD_PARTIALS, WD_PARTIALS, 0u))
    r.s_attn_cross = set_tower_wdec_attn_part_cls(fixed_array(r.cq_dev, r.kxr_dev, r.vxr_dev, r.pms_dev, r.py_dev, r.norms_dev),
        fixed_array(bx, bkx, bkx, bpms, bpy, r.norms_bytes), fixed_array(WD_CQ, WD_KXR, WD_VXR, WD_PARTIALS, WD_PARTIALS, 0u))
    r.s_comb = set_tower_wdec_attn_comb_cls(fixed_array(r.pms_dev, r.py_dev, r.att_dev), fixed_array(bpms, bpy, bx), fixed_array(WD_PARTIALS, WD_PARTIALS, WD_ATT))
    if (r.cm2) {
        r.s_xfeed = set_tower_clamp_cvt_cls(fixed_array(r.xin_dev, r.xh_dev), fixed_array(bt, (r.ta_gemm + TILE_READ_SLACK) * d * 2l), fixed_array(WD_XIN, WD_XFEED))
    } else {
        r.s_xfeed = set_tower_clamp_rq_cls(fixed_array(r.xin_dev, r.xkv_q_dev, r.xkv_s_dev), fixed_array(bt, r.ta * d, r.ta * d / q8e * 4l), fixed_array(WD_XIN, WD_XFEED, WD_XFEED))
    }
    r.s_xkv = wd_xkv_set()
    r.s_kvx_k = set_tower_wdec_kv_cls(fixed_array(r.xkv_proj_dev, r.kxr_dev, r.kx_dev, r.norms_dev), fixed_array(bt, bkx, bkxf, r.norms_bytes), fixed_array(WD_TMP, WD_KXR, WD_KX, 0u))
    r.s_kvx_v = set_tower_wdec_kv_cls(fixed_array(r.xkv_proj_dev, r.vxr_dev, r.vx_dev, r.norms_dev), fixed_array(bt, bkx, bkxf, r.norms_bytes), fixed_array(WD_TMP, WD_VXR, WD_VX, 0u))
    r.s_site |> reserve(r.nl * WD_LAYER_SITES + 1l)
    r.s_site |> resize(r.nl * WD_LAYER_SITES + 1l)
    for (l in range64(r.nl)) {
        let b = int(l * WD_LAYER_SITES)
        r.s_site[b + int(WD_S_QKV)] = wd_gemv_set(r.site_meta[b + int(WD_S_QKV)], r.qkv_dev, 3l * bx, WD_QKV)
        r.s_site[b + int(WD_S_O)] = wd_gemv_set(r.site_meta[b + int(WD_S_O)], r.yo_dev, bx, WD_YO)
        r.s_site[b + int(WD_S_CQ)] = wd_gemv_set(r.site_meta[b + int(WD_S_CQ)], r.cq_dev, bx, WD_CQ)
        r.s_site[b + int(WD_S_CO)] = wd_gemv_set(r.site_meta[b + int(WD_S_CO)], r.yo_dev, bx, WD_YO)
        r.s_site[b + int(WD_S_FC1)] = wd_gemv_set(r.site_meta[b + int(WD_S_FC1)], r.h_dev, bh, WD_H)
        r.s_site[b + int(WD_S_FC2)] = wd_gemv_set(r.site_meta[b + int(WD_S_FC2)], r.yo_dev, bx, WD_YO)
    }
    let te = int(r.nl * WD_LAYER_SITES)
    r.s_site[te] = wd_gemv_set(r.site_meta[te], r.logits_dev, r.nvocab * 4l, WD_LOGITS)
}

//! every class the two chains dispatch built ahead of the sets that bind them, or false
[cold_path]
def private wd_ensure_classes(cm2 : bool; tile : VtTile) : bool {
    return (gemv_cls_ensure(int(KqFmt.q8)) && gemv_cls_ensure_n(int(KqFmt.q8), WD_ROW_CAP) && vt_tile_ensure(tile)
        && ensure_tower_ln_cls() && ensure_tower_post_add_ln_cls() && ensure_tower_bias_act_cls() && ensure_tower_clamp_rq_cls()
        && (!cm2 || ensure_tower_clamp_cvt_cls()) && ensure_tower_wdec_kv_cls() && ensure_tower_wdec_attn_part_cls()
        && ensure_tower_wdec_attn_comb_cls())
}

[cold_path]
def private wd_upload_weights(w : WhisperModel) {
    var r & = g_wd
    let d = r.d
    let nl = r.nl
    r.wq_bytes = w.dqblob.n
    r.ws_bytes = w.dqscales.n * 2l
    r.wq_dev = wd_dev_buf(r.wq_bytes)
    r.ws_dev = wd_dev_buf(r.ws_bytes)
    let lay = active_q8_repack_layout(g_active_needs_repack)
    var regions : array<VtRegion>
    regions |> reserve(nl * 10l + 1l)
    regions |> push(VtRegion(woff = w.d_te_q_off, n = d, rows = r.nvocab))
    r.site_meta |> reserve(nl * WD_LAYER_SITES + 1l)
    for (l in range64(nl)) {
        let lo = dec_layer_offsets(w, l)
        for (woff in fixed_array(lo.q_w, lo.k_w, lo.v_w, lo.o_w, lo.cq_w, lo.ck_w, lo.cv_w, lo.co_w)) {
            regions |> push(VtRegion(woff = woff, n = d, rows = d))
        }
        regions |> push(VtRegion(woff = lo.fc1_w, n = d, rows = 4l * d))
        regions |> push(VtRegion(woff = lo.fc2_w, n = 4l * d, rows = d))
        r.site_meta |> push(wd_site_meta([lo.q_w, lo.k_w, lo.v_w]))   // nolint:STYLE012 - a layer's sites append to the previous layer's
        r.site_meta |> push(wd_site_meta([lo.o_w]))
        r.site_meta |> push(wd_site_meta([lo.cq_w]))
        r.site_meta |> push(wd_site_meta([lo.co_w]))
        r.site_meta |> push(wd_site_meta([lo.fc1_w]))
        r.site_meta |> push(wd_site_meta([lo.fc2_w]))
    }
    r.site_meta |> push(wd_site_meta([w.d_te_q_off]))
    var wq : array<uint8>
    var ws : array<uint8>
    wq |> reserve_resize(r.wq_bytes)
    ws |> reserve_resize(r.ws_bytes)
    vt_gather_regions(w.dqblob, w.dqscales, regions, lay, wq, ws)
    delete regions
    if (!r.oom) {
        unsafe {
            upload_region_at(r.wq_dev, 0l, addr<uint8 const?>(wq[0]), r.wq_bytes)
            upload_region_at(r.ws_dev, 0l, addr<uint8 const?>(ws[0]), r.ws_bytes)
        }
    }
    delete wq
    delete ws
}

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
    var norms : array<float>
    norms |> reserve_resize(rows_n + 2l * d)
    unsafe {
        memcpy(addr<void?>(norms[0]), reinterpret<void?>(plane_at(w.dblob, w.dlayers_off)), rows_n * 4l)
        memcpy(addr<void?>(norms[rows_n]), reinterpret<void?>(plane_at(w.dblob, w.d_ln_w_off)), d * 4l)
        memcpy(addr<void?>(norms[rows_n + d]), reinterpret<void?>(plane_at(w.dblob, w.d_ln_b_off)), d * 4l)
    }
    if (!r.oom) {
        upload_region_at(r.norms_dev, 0l, unsafe(addr<uint8 const?>(norms[0])), r.norms_bytes)
    }
    delete norms
}

[arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-asr-decoder")]
def private wd_attach(w : WhisperModel) : bool {
    var r & = g_wd
    let lay = active_q8_repack_layout(g_active_needs_repack)
    let key = vt_key(w.dqblob, w.dqscales, w.dblob, w.dlayers_off, w.n_text_layer * w.dlayer_stride, 0l, w.n_text_layer, lay.mr, lay.kgroup, lay.wbias)
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
    r.ta_gemm = vt_tile_rows(r.xkv_tile, r.ta)
    r.oom = false
    if (!wd_ensure_classes(r.cm2, r.xkv_tile)) {
        return false
    }
    if (!vt_range_ok(w.dqblob.n) || !vt_range_ok(nl * d * r.ta * 4l) || !vt_range_ok(nl * d * r.tmax * 2l) || !vt_range_ok(2l * nl * d * r.ta * 4l)) {
        return wd_decline(VulkanWdecDecline.shape)
    }
    r.key = key
    wd_upload_weights(w)
    wd_upload_norms(w)
    let bx = WD_ROW_CAP * d * 4l
    let bq = WD_ROW_CAP * 4l * d
    let bt = r.ta * d * 4l
    let nch_cap = (max(r.ta, r.tmax) + int64(WDEC_CHUNK) - 1l) / int64(WDEC_CHUNK)
    r.kxr_dev = wd_dev_buf(nl * d * r.ta * 2l)
    r.vxr_dev = wd_dev_buf(nl * d * r.ta * 2l)
    r.kc_dev = wd_dev_buf(nl * d * r.tmax * 2l)
    r.vc_dev = wd_dev_buf(nl * d * r.tmax * 2l)
    r.x_dev = wd_dev_buf(bx)
    r.xb_dev = wd_dev_buf(bx)
    r.xq_dev = wd_dev_buf(bq)
    r.xs_dev = wd_dev_buf(bq / q8e * 4l)
    r.qkv_dev = wd_dev_buf(3l * bx)
    r.cq_dev = wd_dev_buf(bx)
    r.att_dev = wd_dev_buf(bx)
    r.yo_dev = wd_dev_buf(bx)
    r.h_dev = wd_dev_buf(4l * bx)
    r.pms_dev = wd_dev_buf(nch_cap * r.hh * WD_ROW_CAP * 2l * 4l)
    r.py_dev = wd_dev_buf(nch_cap * r.hh * WD_ROW_CAP * r.hs * 4l)
    r.logits_dev = wd_dev_buf(r.nvocab * 4l)
    r.xin_dev = wd_dev_buf(bt)
    if (r.cm2) {
        r.xh_dev = wd_dev_buf((r.ta_gemm + TILE_READ_SLACK) * d * 2l)
        r.xkv_q_dev = wd_dev_buf(64l)
        r.xkv_s_dev = wd_dev_buf(64l)
    } else {
        r.xh_dev = wd_dev_buf(64l)
        r.xkv_q_dev = wd_dev_buf(r.ta * d)
        r.xkv_s_dev = wd_dev_buf(r.ta * d / q8e * 4l)
    }
    r.xkv_proj_dev = wd_dev_buf(r.ta_gemm * d * 4l)
    r.kx_dev = wd_dev_buf(nl * d * r.ta * 4l)
    r.vx_dev = wd_dev_buf(nl * d * r.ta * 4l)
    let groups = vt_tile_groups(r.xkv_tile, d, r.ta)
    r.xkv_meta_bytes = max((2l * nl * 4l + 2l * nl * groups) * 4l, BATCH_META_BYTES)
    r.xkv_meta = wd_host_buf(r.xkv_meta_bytes, false)
    r.xhost = wd_host_buf(max(bx, r.nvocab * 4l), true)
    r.kvhost = wd_host_buf(2l * nl * d * r.ta * 4l, true)
    if (r.oom) {
        wd_release()
        return false
    }
    wd_sets()
    return true
}

[cold_path]
def private wd_announce {
    g_wd.served_said = true
    to_log(LOG_INFO, "dasLLAMA vulkan wdec: the whisper decoder serves on the device (d {g_wd.d}, {g_wd.nl} layers)\n")
}

[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-asr-decoder")]
def private vulkan_whisper_cross_kv(w : WhisperModel; var ds : DecoderState; enc_out : array<float>) : bool {   // nolint:STYLE038 - the dispatch chain is the shape
    g_wd_window_live = false
    if (!g_wd_env) {
        return wd_decline(VulkanWdecDecline.knob)
    }
    if (!w.dq8) {
        return wd_decline(VulkanWdecDecline.quant_mode)
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
    let bt = ta * d * 4l
    let bkxf = nl * d * ta * 4l
    assert(long_length(ds.kx) >= nl * d * ta && long_length(ds.vx) >= nl * d * ta)
    if (g_wd_pend_key != intptr(unsafe(addr(ds)))) {
        wd_flush_cross_kv()   //! another state's window still sits on the device planes this one overwrites; the same state's is superseded
    }
    g_wd_pend_bytes = 0l
    let groups = vt_tile_groups(r.xkv_tile, d, ta)
    g_wd_maps |> reserve(2l * nl)
    g_wd_maps |> resize(2l * nl)
    var mp = unsafe(reinterpret<uint?>(r.xkv_meta.mapped))
    var cur = 2l * nl * 4l
    for (l in range64(nl)) {
        let lo = dec_layer_offsets(w, l)
        g_wd_maps[l * 2l] = vt_sched_map(mp, cur, l * 2l, lo.ck_w / q8e, r.ta_gemm, groups)
        g_wd_maps[l * 2l + 1l] = vt_sched_map(mp, cur, l * 2l + 1l, lo.cv_w / q8e, r.ta_gemm, groups)
    }
    upload_region_at(r.xin_dev, 0l, unsafe(addr<uint8 const?>(enc_out[0])), bt)
    var pc_rq = TowerClampRqArgs(inbase = 0u, nblk = uint(ta * d / q8e), lo = -VT_NO_CLAMP, hi = VT_NO_CLAMP)
    var pc_g = BatchArgs(n = uint(d), d = uint(d), map_off = 0u)
    let total = ta * hh * hs
    let wg_kv = (total + 255l) / 256l
    var pc_k16 = TowerWdecKvArgs(rows = uint(ta), sstride = uint(d), soff = 0u, nheads = uint(hh), tstride = uint(ta), p0 = 0u,
        dst_off = 0u, boff = 0u, bias_on = 0u, mode = WDEC_KV_F16)
    var pc_kx = pc_k16
    pc_kx.mode = WDEC_KV_KX
    var pc_v16 = pc_k16
    pc_v16.bias_on = 1u
    var pc_vx = pc_v16
    pc_vx.mode = WDEC_KV_VX
    let t0 = ref_time_ticks()
    cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
        pfq_begin(raw)
        pfq_ts(raw)
        vt_feed_enc(raw, h, r.cm2, r.s_xfeed, pc_rq, ta * d)
        vt_pt(raw, VtProfRole.rq)
        for (l in range64(nl)) {
            let lo = dec_layer_offsets(w, l)
            let off = uint(l * d * ta)
            pc_g.map_off = g_wd_maps[l * 2l]
            vt_tile_enc(raw, h, r.xkv_tile, r.s_xkv, pc_g, groups)
            vt_pt(raw, VtProfRole.gemm_k)
            pc_k16.dst_off = off
            enc_tower_wdec_kv_cls(raw, h, r.s_kvx_k, pc_k16, wg_kv)
            pc_kx.dst_off = off
            enc_tower_wdec_kv_cls(raw, h, r.s_kvx_k, pc_kx, wg_kv)
            vt_pt(raw, VtProfRole.kv_store)
            pc_g.map_off = g_wd_maps[l * 2l + 1l]
            vt_tile_enc(raw, h, r.xkv_tile, r.s_xkv, pc_g, groups)
            vt_pt(raw, VtProfRole.gemm_v)
            pc_v16.dst_off = off
            pc_v16.boff = uint(lo.cv_b - r.norms_base)
            enc_tower_wdec_kv_cls(raw, h, r.s_kvx_v, pc_v16, wg_kv)
            pc_vx.dst_off = off
            pc_vx.boff = pc_v16.boff
            enc_tower_wdec_kv_cls(raw, h, r.s_kvx_v, pc_vx, wg_kv)
            vt_pt(raw, VtProfRole.kv_store)
        }
        pfq_end(raw)
    }
    if (vk_prof()) {
        vt_prof_report("whisper cross-kv", nl, ta, get_time_usec(t0))
    }
    unsafe {
        g_wd_pend_kx = addr(ds.kx[0])
        g_wd_pend_vx = addr(ds.vx[0])
    }
    g_wd_pend_bytes = bkxf
    g_wd_pend_key = intptr(unsafe(addr(ds)))
    g_wd_windows++
    g_wd_window_key = g_wd_pend_key
    g_wd_window_live = true
    ds.gpu_kv_gen = 0l
    if (!r.served_said) {
        wd_announce()
    }
    return true
}

def private wd_gemv(raw : VkCommandBuffer; var h : VkHaz; site : int; n_in, d_out, nreg, ncols : int64) {
    var pc = GemvArgs(n = uint(n_in), d = uint(d_out), nreg = uint(nreg), ybase = 0u, lanes_per_row = 0u, act = 0u, bias_on = 0u, gboff = 0u, uboff = 0u,
        ncols = uint(ncols), ystride = uint(nreg * d_out))
    gemv_enc(int(KqFmt.q8), raw, h, g_wd.s_site[site], pc, nreg * d_out)
}

[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-asr-decoder")]
def private vulkan_whisper_decode(w : WhisperModel; var ds : DecoderState; toks : int const?; n : int64) : bool {   // nolint:STYLE038 - the dispatch chain is the shape
    if (!g_wd_window_live || g_wd_window_key != intptr(unsafe(addr(ds)))) {
        wd_flush_cross_kv()   //! the CPU chain decodes this state: whatever window is pending lands first
        if (ds.gpu_kv_gen != 0l && ds.n_past > 0l) {
            panic("dasLLAMA vulkan wdec: the device decoder window was evicted mid-window - ONE live decoder window per process (a second DecoderState may not interleave whisper_cross_kv/whisper_decode_batch)")
        }
        return false
    }
    if (n > WD_ROW_CAP) {
        if (ds.n_past == 0l) {
            g_wd_window_live = false
            wd_flush_cross_kv()   //! the CPU chain takes the window: its memory lands now
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
        var p = reinterpret<float?>(r.xhost.mapped)
        for (rr in range64(n)) {
            let te = w.d_te_off + int64(toks[int(rr)]) * d
            let pe = w.d_pe_off + (p0 + rr) * d
            for (j in range64(d)) {
                p[rr * d + j] = w.dblob[te + j] + w.dblob[pe + j]
            }
        }
        var logits_site_meta = reinterpret<uint?>(r.site_meta[int(nl * WD_LAYER_SITES)].mapped)
        logits_site_meta[1] = uint((n - 1l) * (d / q8e))
    }
    let bx = n * d * 4l
    upload_region_at(r.x_dev, 0l, unsafe(reinterpret<uint8 const?>(r.xhost.mapped)), bx)
    let nd = n * d
    let nff = n * 4l * d
    let wg_nd = rq_wgs(false, nd)
    let wg_nff = rq_wgs(false, nff)
    let kvtotal = n * hh * hs
    let wg_kv = (kvtotal + 255l) / 256l
    let nch_s = (p0 + n + int64(WDEC_CHUNK) - 1l) / int64(WDEC_CHUNK)
    let nch_c = (ta + int64(WDEC_CHUNK) - 1l) / int64(WDEC_CHUNK)
    var pc_rq = TowerClampRqArgs(inbase = 0u, nblk = uint(nd / q8e), lo = -VT_NO_CLAMP, hi = VT_NO_CLAMP)
    var pc_rqh = TowerClampRqArgs(inbase = 0u, nblk = uint(nff / q8e), lo = -VT_NO_CLAMP, hi = VT_NO_CLAMP)
    var pc_kv = TowerWdecKvArgs(rows = uint(n), sstride = uint(3l * d), soff = uint(d), nheads = uint(hh), tstride = uint(tmax), p0 = uint(p0),
        dst_off = 0u, boff = 0u, bias_on = 0u, mode = WDEC_KV_F16)
    var pc_as = TowerWdecAttnArgs(dim = uint(3l * d), tstride = uint(tmax), len0 = uint(p0 + 1l), causal = 1u, nheads = uint(hh),
        nrows = uint(n), koff = 0u, voff = 0u, qboff = 0u)
    var pc_ac = TowerWdecAttnArgs(dim = uint(d), tstride = uint(ta), len0 = uint(ta), causal = 0u, nheads = uint(hh),
        nrows = uint(n), koff = 0u, voff = 0u, qboff = 0u)
    var pc_cs = TowerWdecCombArgs(dim = uint(d), nchunks = uint(nch_s), nheads = uint(hh), nrows = uint(n))
    var pc_cc = pc_cs
    pc_cc.nchunks = uint(nch_c)
    var pc_gelu = TowerBiasArgs(d = uint(4l * d), nelem = uint(nff), boff = 0u, act = BIAS_ACT_GELU_TANH)
    let wg_attn_s = nch_s * hh * n
    let wg_attn_c = nch_c * hh * n
    let wg_comb = hh * n
    let t0 = ref_time_ticks()
    cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
        pfq_begin(raw)
        pfq_ts(raw)
        for (l in range64(nl)) {
            let lo = dec_layer_offsets(w, l)
            let site = int(l * WD_LAYER_SITES)
            if (l == 0l) {
                var pc = TowerLnArgs(d = uint(d), rows = uint(n), woff = uint(lo.ln1_w - r.norms_base), boff = uint(lo.ln1_b - r.norms_base), eps = w.eps)
                enc_tower_ln_cls(raw, h, r.s_ln, pc, n)
                vt_pt(raw, VtProfRole.ln)
            }
            enc_tower_clamp_rq_cls(raw, h, r.s_rq_xb, pc_rq, wg_nd)
            vt_pt(raw, VtProfRole.rq)
            wd_gemv(raw, h, site + int(WD_S_QKV), d, d, 3l, n)
            vt_pt(raw, VtProfRole.gemm_q)
            pc_kv.soff = uint(d)
            pc_kv.dst_off = uint(l * d * tmax)
            pc_kv.bias_on = 0u
            enc_tower_wdec_kv_cls(raw, h, r.s_kv_k, pc_kv, wg_kv)
            pc_kv.soff = uint(2l * d)
            pc_kv.bias_on = 1u
            pc_kv.boff = uint(lo.v_b - r.norms_base)
            enc_tower_wdec_kv_cls(raw, h, r.s_kv_v, pc_kv, wg_kv)
            vt_pt(raw, VtProfRole.kv_store)
            pc_as.koff = uint(l * d * tmax)
            pc_as.voff = pc_as.koff
            pc_as.qboff = uint(lo.q_b - r.norms_base)
            enc_tower_wdec_attn_part_cls(raw, h, r.s_attn_self, pc_as, wg_attn_s)
            enc_tower_wdec_attn_comb_cls(raw, h, r.s_comb, pc_cs, wg_comb)
            vt_pt(raw, VtProfRole.attn)
            enc_tower_clamp_rq_cls(raw, h, r.s_rq_att, pc_rq, wg_nd)
            vt_pt(raw, VtProfRole.rq)
            wd_gemv(raw, h, site + int(WD_S_O), d, d, 1l, n)
            vt_pt(raw, VtProfRole.gemm_o)
            var pc_post = TowerLnArgs(d = uint(d), rows = uint(n), boff = uint(lo.o_b - r.norms_base), eps = w.eps,
                ln_on = 1u, lnoff = uint(lo.cln_w - r.norms_base), lnboff = uint(lo.cln_b - r.norms_base), ascale = 1.0)
            enc_tower_post_add_ln_cls(raw, h, r.s_post, pc_post, n)
            vt_pt(raw, VtProfRole.post_add)
            enc_tower_clamp_rq_cls(raw, h, r.s_rq_xb, pc_rq, wg_nd)
            vt_pt(raw, VtProfRole.rq)
            wd_gemv(raw, h, site + int(WD_S_CQ), d, d, 1l, n)
            vt_pt(raw, VtProfRole.gemm_q)
            pc_ac.koff = uint(l * d * ta)
            pc_ac.voff = pc_ac.koff
            pc_ac.qboff = uint(lo.cq_b - r.norms_base)
            enc_tower_wdec_attn_part_cls(raw, h, r.s_attn_cross, pc_ac, wg_attn_c)
            enc_tower_wdec_attn_comb_cls(raw, h, r.s_comb, pc_cc, wg_comb)
            vt_pt(raw, VtProfRole.attn)
            enc_tower_clamp_rq_cls(raw, h, r.s_rq_att, pc_rq, wg_nd)
            vt_pt(raw, VtProfRole.rq)
            wd_gemv(raw, h, site + int(WD_S_CO), d, d, 1l, n)
            vt_pt(raw, VtProfRole.gemm_o)
            pc_post.boff = uint(lo.co_b - r.norms_base)
            pc_post.lnoff = uint(lo.ln2_w - r.norms_base)
            pc_post.lnboff = uint(lo.ln2_b - r.norms_base)
            enc_tower_post_add_ln_cls(raw, h, r.s_post, pc_post, n)
            vt_pt(raw, VtProfRole.post_add)
            enc_tower_clamp_rq_cls(raw, h, r.s_rq_xb, pc_rq, wg_nd)
            vt_pt(raw, VtProfRole.rq)
            wd_gemv(raw, h, site + int(WD_S_FC1), d, 4l * d, 1l, n)
            vt_pt(raw, VtProfRole.gemm_fc1)
            pc_gelu.boff = uint(lo.fc1_b - r.norms_base)
            enc_tower_bias_act_cls(raw, h, r.s_bias_h, pc_gelu, wg_nff)
            vt_pt(raw, VtProfRole.bias)
            enc_tower_clamp_rq_cls(raw, h, r.s_rq_h, pc_rqh, wg_nff)
            vt_pt(raw, VtProfRole.rq)
            wd_gemv(raw, h, site + int(WD_S_FC2), 4l * d, d, 1l, n)
            vt_pt(raw, VtProfRole.gemm_fc2)
            let last = l + 1l == nl
            pc_post.boff = uint(lo.fc2_b - r.norms_base)
            pc_post.lnoff = uint(last ? r.final_ln_w_off : dec_layer_offsets(w, l + 1l).ln1_w - r.norms_base)
            pc_post.lnboff = uint(last ? r.final_ln_b_off : dec_layer_offsets(w, l + 1l).ln1_b - r.norms_base)
            enc_tower_post_add_ln_cls(raw, h, r.s_post, pc_post, n)
            vt_pt(raw, VtProfRole.post_add)
        }
        enc_tower_clamp_rq_cls(raw, h, r.s_rq_xb, pc_rq, wg_nd)
        vt_pt(raw, VtProfRole.rq)
        wd_gemv(raw, h, int(nl * WD_LAYER_SITES), d, r.nvocab, 1l, 1l)
        vt_pt(raw, VtProfRole.logits)
        vhz_dep(raw, h, WD_LOGITS, 0u, true)
        cmd_copy_range(raw, r.logits_dev, 0l, r.xhost.buf, 0l, r.nvocab * 4l)
        vt_pt(raw, VtProfRole.copy_out)
        pfq_end(raw)
    }
    if (vk_prof()) {
        vt_prof_report("whisper decode", nl, n, get_time_usec(t0))
    }
    unsafe {
        memcpy(addr<void?>(ds.logits[0]), r.xhost.mapped, r.nvocab * 4l)
    }
    g_wd_us_gpu += int64(get_time_usec(t0))
    g_wd_us_encode += int64(get_time_usec(te0)) - int64(get_time_usec(t0))
    g_wd_steps++
    g_wd_tokens += n
    ds.gpu_kv_gen = 1l
    return true
}

[init]
def dasllama_vulkan_asr_dec_register {
    g_wd_env = g_env_vulkan.vk_wdec
    register_vk_drop_hook(@@wd_forget)
    static_if (!typeinfo builtin_module_exists(das_metal)) {
        register_whisper_cross_kv_gpu(@@vulkan_whisper_cross_kv)
        register_whisper_decode_gpu(@@vulkan_whisper_decode)
    }
}
