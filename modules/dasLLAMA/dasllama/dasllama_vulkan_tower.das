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

//! The vision tower Vulkan driver: one-shot encoder chains - no session, no KV, no mirror - that
//! serve a family's block loop dispatch for dispatch as the family's CPU loop spells it, over the
//! q8 image the CPU chain reads (qwen25v over its baked f16 twin). Every hook is best-effort: it
//! answers false on any shape, knob or device decline and the CPU chain serves that encode. Engage
//! is read from the counter deltas (`vulkan_tower_stats`), never from "the model ran". The Metal
//! driver fills the same slots on an Apple build, so this driver registers only where it is absent.

var private g_vt_env_tower = true
var private g_vt_encodes = 0l
var private g_vt_rows = 0l
var private g_vt_blocks = 0l
var private g_vt_declines : table<string; int64>
var private g_vt_declines_total = 0l

enum private VulkanTowerDecline {
    knob         //! DASLLAMA_VK_TOWER=0, or set_vulkan_tower(false)
    quant_mode   //! the tower's planes are not the lane the driver serves
    shape        //! a width, head size or row count the stamps do not serve
    device       //! the tier has no device, or a class failed to build
}

//! Engage / decline counters; `blocks` = transformer blocks the driver ran. Model-level gates assert DELTAS on these.
def vulkan_tower_stats : tuple<encodes : int64; rows : int64; blocks : int64; declines : int64> {
    return (encodes = g_vt_encodes, rows = g_vt_rows, blocks = g_vt_blocks, declines = g_vt_declines_total)
}

//! The declines-by-reason table (a copy), keyed by `VulkanTowerDecline` value names.
def vulkan_tower_declines : table<string; int64> {
    return <- clone_to_move(g_vt_declines)
}

//! Test A/B seat: pin the tower driver on / off in-process (the env knob's setter twin).
def set_vulkan_tower(on : bool) {
    g_vt_env_tower = on
}

[cold_path]
def private vt_decline(why : VulkanTowerDecline) : bool {
    let key = "{why}"
    g_vt_declines[key] = (g_vt_declines?[key] ?? 0l) + 1l
    g_vt_declines_total++
    return false
}

[arch(at = "../ARCHITECTURE_GPU_TOWER.md#vk-tower-encode-chains")]
def private vulkan_tower_serves_exact() : bool => false

let private VT_KEY_SAMPLE_WORDS = 64l        //! words of the quant plane folded into the residency key beside its address and size
let private VT_NO_CLAMP = 3.4028235e38 //! an inactive ClippableLinear bound: the CPU chain skips the clamp, the device clamps to the float range
let private VT_MAX_ENCODE_ROWS = 4096l   //! the row cap of one encode (the shape decline)

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
    hd : int64            //! the head width the rope tables are sized for
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
    xh_dev : uint64       //! the f16 feed, [cap x max(d, ff)]
    wlo_dev : uint64      //! the windows' row starts (nwin + 1), the f32 per-window route's table
    win_cap : int64       //! the window table's capacity (windows)
    host : HostBuf        //! the x readback staging (the tap stashes behind it)
    bvar : int            //! the q8 batch class variant the d-wide sets were built for (-1 = none)
    bvar_ff : int         //! ... and the ff-wide ones (gate, up): the tile pick reads the output width
    bvar_qkv : int        //! ... and the fused qkv one (3d wide)
    s_qkv : VkDescriptorSet
    s_bias_qkv : VkDescriptorSet
    s_rope_tq : VkDescriptorSet
    s_rope_tk : VkDescriptorSet
    s_cvt_xb : VkDescriptorSet
    s_cvt_att : VkDescriptorSet
    s_cvt_hg : VkDescriptorSet
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
    fa_khr : bool
}

var private g_vt_g4v : VtResident   //! the gemma4v family's resident tower (one a family: a second mmproj replaces it)
var private g_vt_g3v : VtResident
var private g_vt_q3v : VtResident
var private g_vt_q25v : VtResident
var private g_vt_rope2d_tabs : array<float>   //! the packed rope tables staging, [row][cx | sx | cy | sy]

[arch(at = "../ARCHITECTURE_GPU_TOWER.md#vk-tower-encode-chains")]
def private vt_key_u16(wblob : PlaneU16; n_layer : int64) : uint64 {
    var k = intptr(unsafe(reinterpret<void?>(wblob.p))) * 0x9E3779B97F4A7C15ul + uint64(wblob.n) + uint64(n_layer) * 0x100000001B3ul
    let step = max(wblob.n / VT_KEY_SAMPLE_WORDS, 1l)
    var i = 0l
    while (i < wblob.n) {
        k = (k ^ uint64(unsafe(wblob.p[i]))) * 0x100000001B3ul
        i += step
    }
    return k
}

[cold_path, arch(at = "../ARCHITECTURE_GPU_TOWER.md#vk-tower-encode-chains")]
def private vt_upload_f16(var r : VtResident; key : uint64; wblob : PlaneU16; w_off, w_n : int64; blob : PlaneF; rows_off, rows_n : int64) {
    vt_release(r)
    r.key = key
    r.wq_bytes = w_n * 2l
    r.ws_bytes = 0l
    r.wq_dev = make_device_buf(r.wq_bytes)
    r.ws_dev = make_device_buf(64l)   //! no scale plane on this family: a stub the release frees like the others
    upload_region_at(r.wq_dev, 0l, unsafe(reinterpret<uint8 const?>(plane_at(wblob, w_off))), r.wq_bytes)
    r.ones_off = 0l
    r.norms_bytes = rows_n * 4l
    r.norms_dev = make_device_buf(r.norms_bytes)
    upload_region_at(r.norms_dev, 0l, unsafe(reinterpret<uint8 const?>(plane_at(blob, rows_off))), r.norms_bytes)
    r.cap_rows = 0l
    r.bvar = -1
    r.bvar_ff = -1
    r.bvar_qkv = -1
}

//! the padded panels at the compact rows, the f16 feed and the window table; true when (re)built
[cold_path]
def private vt_scratch_f16(var r : VtResident; rows, nwin, d, ff, hd, dp : int64; khr : bool) : bool {
    let cap = (rows + 63l) / 64l * 64l
    if (r.cap_rows >= cap && r.win_cap >= nwin && r.d == d && r.ff == ff && r.pad_dp == dp && r.f16w) {
        return false
    }
    vt_release_scratch(r)
    r.cap_rows = cap
    r.win_cap = nwin
    r.d = d
    r.ff = ff
    r.hd = hd
    r.fa_khr = khr
    r.pad_dp = dp
    r.f16w = true
    r.fused = true   //! the rope tables ride the fused-row seats
    let bx = cap * d * 4l
    let bff = cap * ff * 4l
    let bh = cap * d * 2l
    r.qp_dev = make_device_buf(cap * dp * 4l)
    r.kp16_dev = make_device_buf(cap * dp * 2l)
    r.vp16_dev = make_device_buf(cap * dp * 2l)
    r.attp_dev = make_device_buf(cap * dp * 4l)
    r.wlo_dev = make_device_buf((nwin + 1l) * 4l)
    r.xh_dev = make_device_buf(cap * max(d, ff) * 2l)
    r.qkv_dev = make_device_buf(64l)   //! unused seats of the fused set, stubs the release frees alike
    r.cos_dev = make_device_buf(cap * (hd / 2l) * 4l)
    r.sin_dev = make_device_buf(cap * (hd / 2l) * 4l)
    r.x_dev = make_device_buf(bx)
    r.xb_dev = make_device_buf(bx)
    r.xq_dev = make_device_buf(64l)
    r.xs_dev = make_device_buf(64l)
    r.q_dev = make_device_buf(bx)
    r.k_dev = make_device_buf(bx)
    r.v_dev = make_device_buf(bx)
    r.q2_dev = make_device_buf(64l)
    r.k2_dev = make_device_buf(64l)
    r.v2_dev = make_device_buf(64l)
    r.k16_dev = make_device_buf(bh)
    r.v16_dev = make_device_buf(bh)
    r.att_dev = make_device_buf(bx)
    r.yo_dev = make_device_buf(bx)
    r.hg_dev = make_device_buf(bff)
    r.hu_dev = make_device_buf(bff)
    r.yd_dev = make_device_buf(bx)
    r.tabs_dev = make_device_buf(64l)
    r.host = make_host_buf(bx, true, [cached = true])
    r.meta_bytes = 0l
    return true
}

[cold_path]
def private vt_sets_q25v(var r : VtResident; gemm_khr : bool) {
    let cap = r.cap_rows
    let bx = cap * r.d * 4l
    let bff = cap * r.ff * 4l
    let bxh = cap * max(r.d, r.ff) * 2l
    let bp = cap * r.pad_dp * 4l
    let bph = cap * r.pad_dp * 2l
    let btab = cap * (r.hd / 2l) * 4l
    let bwl = (r.win_cap + 1l) * 4l
    r.s_rms = set_tower_rms_cls(fixed_array(r.x_dev, r.xb_dev, r.norms_dev), fixed_array(bx, bx, r.norms_bytes), fixed_array(VT_X, VT_XB, 0u))
    r.s_cvt_xb = set_f16cvt_cls(fixed_array(r.xb_dev, r.xh_dev), fixed_array(bx, bxh), fixed_array(VT_XB, VT_XQ))
    r.s_cvt_att = set_f16cvt_cls(fixed_array(r.att_dev, r.xh_dev), fixed_array(bx, bxh), fixed_array(VT_ATT, VT_XQ))
    r.s_cvt_hg = set_f16cvt_cls(fixed_array(r.hg_dev, r.xh_dev), fixed_array(bff, bxh), fixed_array(VT_HG, VT_XQ))
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
    r.s_glu = set_tower_glu_act_cls(fixed_array(r.hg_dev, r.hu_dev, r.norms_dev), fixed_array(bff, bff, r.norms_bytes), fixed_array(VT_HG, VT_HU, 0u))
}

[arch(at = "../ARCHITECTURE_GPU_TOWER.md#vk-tower-encode-chains")]
def private vt_key(qblob : PlaneI8; qscales_n, n_layer : int64) : uint64 {
    var k = intptr(unsafe(reinterpret<void?>(qblob.p))) * 0x9E3779B97F4A7C15ul + uint64(qblob.n) + uint64(qscales_n) * 0x100000001B3ul + uint64(n_layer) * 0x9E3779B1ul
    let step = max(qblob.n / VT_KEY_SAMPLE_WORDS, 1l)
    var i = 0l
    while (i < qblob.n) {
        k = (k ^ uint64(uint8(unsafe(qblob.p[i])))) * 0x100000001B3ul
        i += step
    }
    return k
}

def private vt_lo(c : Clamp; input : bool) : float => c.active ? (input ? c.imin : c.omin) : -VT_NO_CLAMP
def private vt_hi(c : Clamp; input : bool) : float => c.active ? (input ? c.imax : c.omax) : VT_NO_CLAMP

[cold_path]
def private vt_release_scratch(var r : VtResident) {
    if (r.cap_rows == 0l) {
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
        r.pad_dp = 0l
    }
    if (r.fused) {
        for (b in fixed_array(r.qkv_dev, r.cos_dev, r.sin_dev)) {
            destroy_device_buf(b)
        }
        r.fused = false
    }
    if (r.taps > 0l) {
        destroy_device_buf(r.xtap_dev)
        r.taps = 0l
    }
    if (r.f16w) {
        destroy_device_buf(r.xh_dev)
        destroy_device_buf(r.wlo_dev)
        r.f16w = false
        r.win_cap = 0l
    }
    destroy_host_buf(r.host)
    if (r.meta_bytes > 0l) {
        destroy_host_buf(r.meta)
        r.meta_bytes = 0l
    }
    r.cap_rows = 0l
    r.bvar = -1
    r.bvar_ff = -1
}

[cold_path]
def private vt_release(var r : VtResident) {
    vt_release_scratch(r)
    if (r.key != 0ul) {
        destroy_device_buf(r.wq_dev)
        destroy_device_buf(r.ws_dev)
        destroy_device_buf(r.norms_dev)
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
}

def private vt_blank : VtResident => VtResident(bvar = -1, bvar_ff = -1, bvar_qkv = -1)

[arch(at = "../ARCHITECTURE_GPU_TOWER.md#vk-tower-encode-chains")]
def private vt_forget {
    g_vt_g4v = vt_blank()
    g_vt_g3v = vt_blank()
    g_vt_q3v = vt_blank()
    g_vt_q25v = vt_blank()
}

//! one block GEMM's region in the tower's q8 plane: its element offset, its input width and its rows - the repack unit
struct private VtRegion {
    woff : int64
    n : int64
    rows : int64
}

[cold_path, arch(at = "../ARCHITECTURE_GPU_TOWER.md#vk-tower-encode-chains")]
def private vt_upload(var r : VtResident; key : uint64; qblob : PlaneI8; qscales : PlaneF; regions : array<VtRegion>;
                      blob : PlaneF; rows_off, rows_n, ones_n : int64) {
    vt_release(r)
    r.key = key
    r.wq_bytes = qblob.n
    r.ws_bytes = qscales.n * 2l
    r.wq_dev = make_device_buf(r.wq_bytes)
    r.ws_dev = make_device_buf(r.ws_bytes)
    var wq : array<uint8>
    var ws : array<uint8>
    wq |> reserve_resize(r.wq_bytes)
    ws |> reserve_resize(r.ws_bytes)
    let repacked = g_active_needs_repack
    let mr = repacked ? active_q8_layout_mr() : 1l
    let kgroup = repacked ? active_q8_kgroup() : 4l
    let wbias = repacked ? active_q8_wbias() : 0l
    for (reg in regions) {
        q8_gather_rows(plane_at(qblob, reg.woff), null, plane_at(qscales, reg.woff / 32l), reg.n, reg.rows, reg.rows, mr, kgroup, wbias,
            wq, ws, reg.woff, reg.woff / 32l)
    }
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
    r.ones_off = rows_n
    r.norms_bytes = (rows_n + ones_n) * 4l
    r.norms_dev = make_device_buf(r.norms_bytes)
    upload_region_at(r.norms_dev, 0l, unsafe(addr<uint8 const?>(norms[0])), r.norms_bytes)
    delete norms
    r.cap_rows = 0l
    r.bvar = -1
    r.bvar_ff = -1
    r.bvar_qkv = -1
}

//! `dp` > 0 adds the padded panels, `meta_bytes` sizes the schedule buffer, `fused` adds the [q | k | v] rows and the rope tables, `taps` the stashes; true when (re)built
[cold_path]
def private vt_scratch(var r : VtResident; rows, d, ff, hd, dp, meta_bytes : int64; khr, fused : bool; taps : int64) : bool {
    let cap = (rows + 63l) / 64l * 64l
    if (r.cap_rows >= cap && r.d == d && r.ff == ff && r.pad_dp == dp && r.meta_bytes >= meta_bytes && r.fused == fused && r.taps == taps) {
        return false
    }
    vt_release_scratch(r)
    r.meta_bytes = max(meta_bytes, BATCH_META_BYTES)
    r.meta = make_host_buf(r.meta_bytes, true)
    r.cap_rows = cap
    r.d = d
    r.ff = ff
    r.hd = hd
    r.fa_khr = khr
    let bx = cap * d * 4l
    let bff = cap * ff * 4l
    let bq = cap * max(d, ff)
    let bs = bq / 32l * 4l
    let bh = cap * d * 2l
    let btabs = cap * hd * 4l   //! four tables of hd/4 pairs a row
    r.pad_dp = dp
    if (dp > 0l) {
        r.qp_dev = make_device_buf(cap * dp * 4l)
        r.kp16_dev = make_device_buf(cap * dp * 2l)
        r.vp16_dev = make_device_buf(cap * dp * 2l)
        r.attp_dev = make_device_buf(cap * dp * 4l)
    }
    r.fused = fused
    if (fused) {
        r.qkv_dev = make_device_buf(3l * bx)
        r.cos_dev = make_device_buf(cap * (hd / 2l) * 4l)
        r.sin_dev = make_device_buf(cap * (hd / 2l) * 4l)
    }
    r.taps = taps
    if (taps > 0l) {
        r.xtap_dev = make_device_buf(taps * bx)
    }
    r.x_dev = make_device_buf(bx)
    r.xb_dev = make_device_buf(bx)
    r.xq_dev = make_device_buf(bq)
    r.xs_dev = make_device_buf(bs)
    r.q_dev = make_device_buf(bx)
    r.k_dev = make_device_buf(bx)
    r.v_dev = make_device_buf(bx)
    r.q2_dev = make_device_buf(bx)
    r.k2_dev = make_device_buf(bx)
    r.v2_dev = make_device_buf(bx)
    r.k16_dev = make_device_buf(bh)
    r.v16_dev = make_device_buf(bh)
    r.att_dev = make_device_buf(bx)
    r.yo_dev = make_device_buf(bx)
    r.hg_dev = make_device_buf(bff)
    r.hu_dev = make_device_buf(bff)
    r.yd_dev = make_device_buf(bx)
    r.tabs_dev = make_device_buf(btabs)
    r.host = make_host_buf((1l + taps) * bx, true, [cached = true])
    r.bvar = -1
    r.bvar_ff = -1
    r.bvar_qkv = -1
    return true
}

[cold_path]
def private vt_sets_g4v(var r : VtResident) {
    let cap = r.cap_rows
    let bx = cap * r.d * 4l
    let bff = cap * r.ff * 4l
    let bq = cap * max(r.d, r.ff)
    let bs = bq / 32l * 4l
    let bh = cap * r.d * 2l
    let btabs = cap * r.hd * 4l
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
    r.s_cvt_k = set_f16cvt_cls(fixed_array(r.k2_dev, r.k16_dev), fixed_array(bx, bh), fixed_array(VT_K2, VT_K16))
    r.s_cvt_v = set_f16cvt_cls(fixed_array(r.v2_dev, r.v16_dev), fixed_array(bx, bh), fixed_array(VT_V2, VT_V16))
    r.s_fa = fa_stamp_set(khr, fixed_array(r.q2_dev, r.k16_dev, r.v16_dev, r.att_dev), fixed_array(bx, bh, bh, bx), fixed_array(VT_Q2, VT_K16, VT_V16, VT_ATT))
    r.s_post_o = set_tower_post_add_cls(fixed_array(r.x_dev, r.yo_dev, r.xb_dev, r.norms_dev), fixed_array(bx, bx, bx, r.norms_bytes), fixed_array(VT_X, VT_YO, VT_XB, 0u))
    r.s_post_d = set_tower_post_add_cls(fixed_array(r.x_dev, r.yd_dev, r.xb_dev, r.norms_dev), fixed_array(bx, bx, bx, r.norms_bytes), fixed_array(VT_X, VT_YD, VT_XB, 0u))
    r.s_geglu = set_tower_geglu_cls(fixed_array(r.hg_dev, r.hu_dev), fixed_array(bff, bff), fixed_array(VT_HG, VT_HU))
}

[cold_path]
def private vt_sets_g3v(var r : VtResident) {
    let cap = r.cap_rows
    let bx = cap * r.d * 4l
    let bff = cap * r.ff * 4l
    let bq = cap * max(r.d, r.ff)
    let bs = bq / 32l * 4l
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
}

def private vt_batch_set(r : VtResident; v : int; y_dev : uint64; y_bytes : int64; y_bit : uint) : VkDescriptorSet {
    let bq = r.cap_rows * max(r.d, r.ff)
    return q8_batch_cls_set(v, fixed_array(r.wq_dev, r.ws_dev, r.meta.buf, r.xq_dev, r.xs_dev, y_dev),
        fixed_array(r.wq_bytes, r.ws_bytes, r.meta_bytes, bq, bq / 32l * 4l, y_bytes), fixed_array(0u, 0u, 0u, VT_XQ, VT_XQ, y_bit))
}

[cold_path, arch(at = "../ARCHITECTURE_GPU_TOWER.md#vk-tower-encode-chains")]
def private vt_batch_sets(var r : VtResident; v, vff : int; vqkv : int = -1) {
    let bx = r.cap_rows * r.d * 4l
    let bff = r.cap_rows * r.ff * 4l
    if (r.bvar != v) {
        r.s_q = vt_batch_set(r, v, r.q_dev, bx, VT_Q)
        r.s_k = vt_batch_set(r, v, r.k_dev, bx, VT_K)
        r.s_v = vt_batch_set(r, v, r.v_dev, bx, VT_V)
        r.s_o = vt_batch_set(r, v, r.yo_dev, bx, VT_YO)
        r.s_down = vt_batch_set(r, v, r.yd_dev, bx, VT_YD)
        r.bvar = v
    }
    if (r.bvar_ff != vff) {
        r.s_hg = vt_batch_set(r, vff, r.hg_dev, bff, VT_HG)
        r.s_hu = vt_batch_set(r, vff, r.hu_dev, bff, VT_HU)
        r.bvar_ff = vff
    }
    if (r.fused && r.bvar_qkv != vqkv) {
        r.s_qkv = vt_batch_set(r, vqkv, r.qkv_dev, 3l * bx, VT_QKV)
        r.bvar_qkv = vqkv
    }
    to_log(LOG_INFO, "dasLLAMA vulkan tower: batch sets at {r.cap_rows} rows - variants {v} (d {r.d}) / {vff} (ff {r.ff}) / {vqkv} (qkv), tiles {q8_batch_tile_for(r.d, r.cap_rows)} / {q8_batch_tile_for(r.ff, r.cap_rows)}\n")
}

[cold_path]
def private vt_sets_q3v(var r : VtResident) {
    let cap = r.cap_rows
    let bx = cap * r.d * 4l
    let bff = cap * r.ff * 4l
    let bq = cap * max(r.d, r.ff)
    let bs = bq / 32l * 4l
    let bp = cap * r.pad_dp * 4l
    let bph = cap * r.pad_dp * 2l
    let btab = cap * (r.hd / 2l) * 4l
    r.s_ln = set_tower_ln_cls(fixed_array(r.x_dev, r.xb_dev, r.norms_dev), fixed_array(bx, bx, r.norms_bytes), fixed_array(VT_X, VT_XB, 0u))
    r.s_rq_xb = set_tower_clamp_rq_cls(fixed_array(r.xb_dev, r.xq_dev, r.xs_dev), fixed_array(bx, bq, bs), fixed_array(VT_XB, VT_XQ, VT_XQ))
    r.s_rq_att = set_tower_clamp_rq_cls(fixed_array(r.att_dev, r.xq_dev, r.xs_dev), fixed_array(bx, bq, bs), fixed_array(VT_ATT, VT_XQ, VT_XQ))
    r.s_rq_hg = set_tower_clamp_rq_cls(fixed_array(r.hg_dev, r.xq_dev, r.xs_dev), fixed_array(bff, bq, bs), fixed_array(VT_HG, VT_XQ, VT_XQ))
    r.s_bias_qkv = set_tower_bias_act_cls(fixed_array(r.qkv_dev, r.norms_dev), fixed_array(3l * bx, r.norms_bytes), fixed_array(VT_QKV, 0u))
    r.s_bias_hg = set_tower_bias_act_cls(fixed_array(r.hg_dev, r.norms_dev), fixed_array(bff, r.norms_bytes), fixed_array(VT_HG, 0u))
    r.s_rope_tq = set_tower_rope_tab_cls(fixed_array(r.qkv_dev, r.cos_dev, r.sin_dev), fixed_array(3l * bx, btab, btab), fixed_array(VT_QKV, 0u, 0u))
    r.s_rope_tk = r.s_rope_tq   //! the same set: the k slot is a column offset in the push constants
    r.s_pad_q = set_tower_hpad_cls(fixed_array(r.qkv_dev, r.qp_dev), fixed_array(3l * bx, bp), fixed_array(VT_QKV, VT_Q2))
    r.s_pad_k = set_tower_hpad16_cls(fixed_array(r.qkv_dev, r.kp16_dev), fixed_array(3l * bx, bph), fixed_array(VT_QKV, VT_K16))
    r.s_pad_v = set_tower_hpad16_cls(fixed_array(r.qkv_dev, r.vp16_dev), fixed_array(3l * bx, bph), fixed_array(VT_QKV, VT_V16))
    r.s_fa = fa_stamp_set(r.fa_khr, fixed_array(r.qp_dev, r.kp16_dev, r.vp16_dev, r.attp_dev), fixed_array(bp, bph, bph, bp), fixed_array(VT_Q2, VT_K16, VT_V16, VT_K2))
    r.s_unpad = set_tower_hunpad_cls(fixed_array(r.attp_dev, r.att_dev), fixed_array(bp, bx), fixed_array(VT_K2, VT_ATT))
    r.s_post_o_ln = set_tower_post_add_ln_cls(fixed_array(r.x_dev, r.yo_dev, r.xb_dev, r.norms_dev), fixed_array(bx, bx, bx, r.norms_bytes), fixed_array(VT_X, VT_YO, VT_XB, 0u))
    r.s_post_d_ln = set_tower_post_add_ln_cls(fixed_array(r.x_dev, r.yd_dev, r.xb_dev, r.norms_dev), fixed_array(bx, bx, bx, r.norms_bytes), fixed_array(VT_X, VT_YD, VT_XB, 0u))
}

//! record `rid` = [wblk0, 0, rows, 0] at word 4 rid and its map of `groups` words at the cursor; returns the map's word offset and advances the cursor
def private vt_sched_map(var mp : uint?; var cur : int64&; rid : int64; wblk0, rows, groups : int64) : uint {
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

def private vt_gemm_groups(d, rows : int64) : int64 {
    let te = batch_tile_edges(int(KqFmt.q8), d, rows)
    return ((d + te.td - 1l) / te.td) * ((rows + te.tc - 1l) / te.tc)
}

def private g3v_regions(t : Gemma3vTower) : array<VtRegion> {
    let d = t.n_embd
    let ff = t.ff_pad
    var regions : array<VtRegion>
    regions |> reserve(t.n_layer * GEMMA3V_GEMMS)
    for (l in range64(t.n_layer)) {
        let o = gemma3v_block_offs(t, l)
        for (g in range64(GEMMA3V_GEMMS)) {
            let n = g == GEMMA3V_GEMM_DOWN ? ff : d
            let rows = g == GEMMA3V_GEMM_UP ? ff : d
            regions |> push(VtRegion(woff = o.w[g], n = n, rows = rows))
        }
    }
    return <- regions
}

[arch(at = "../ARCHITECTURE_GPU_TOWER.md#vk-tower-encode-chains")]
def private vulkan_gemma3v_blocks(t : Gemma3vTower; var s : Gemma3vState; npos : int64) : bool {   // nolint:STYLE038 — the dispatch chain is the shape
    if (!g_vt_env_tower) {
        return vt_decline(VulkanTowerDecline.knob)
    }
    if (!t.q8) {
        return vt_decline(VulkanTowerDecline.quant_mode)
    }
    let d = t.n_embd
    let ff = t.ff_pad
    let hs = d / t.n_head
    let hsp = hs <= 64l ? 64l : 128l
    let dp = t.n_head * hsp
    if (hs > 128l || d % 64l != 0l || ff % 64l != 0l || npos > VT_MAX_ENCODE_ROWS || npos <= 0l) {
        return vt_decline(VulkanTowerDecline.shape)
    }
    if (!gpu_want_arms_tier() || !vk_moe_init()) {
        return vt_decline(VulkanTowerDecline.device)
    }
    let khr = !cm2_fa_on()
    if (khr && !khr_kq_tile_on()) {
        return vt_decline(VulkanTowerDecline.device)
    }
    var fa_ok = false
    if (hsp == 64l) {
        fa_ok = khr ? ensure_fa_khr_h64_bidir_cls() : ensure_fa_cm2_h64_bidir_cls()
    } else {
        fa_ok = khr ? ensure_fa_khr_h128_bidir_cls() : ensure_fa_cm2_h128_bidir_cls()
    }
    if (!fa_ok || !ensure_tower_ln_cls() || !ensure_tower_clamp_rq_cls() || !ensure_tower_bias_act_cls() || !ensure_tower_hpad_cls()
            || !ensure_tower_hpad16_cls() || !ensure_tower_hunpad_cls() || !ensure_tower_post_add_ln_cls()) {
        return vt_decline(VulkanTowerDecline.device)
    }
    let v = q8_batch_cls_variant(q8_batch_tile_for(d, npos), q8_batch_aligned(d, npos))
    let vff = q8_batch_cls_variant(q8_batch_tile_for(ff, npos), q8_batch_aligned(ff, npos))
    if (!q8_batch_cls_ensure(v) || !q8_batch_cls_ensure(vff)) {
        return vt_decline(VulkanTowerDecline.device)
    }
    var r & = g_vt_g3v
    let key = vt_key(t.qblob, t.qscales.n, t.n_layer)
    if (r.key != key) {
        var regions <- g3v_regions(t)
        vt_upload(r, key, t.qblob, t.qscales, regions, t.blob, t.layer_f, t.n_layer * t.layer_f_stride, 0l)
        delete regions
    }
    let g_d = vt_gemm_groups(d, npos)
    let g_ff = vt_gemm_groups(ff, npos)
    let nrec = t.n_layer * GEMMA3V_GEMMS
    if (vt_scratch(r, npos, d, ff, hs, dp, (nrec * 4l + t.n_layer * (5l * g_d + g_ff)) * 4l, khr, false, 0l)) {
        vt_sets_g3v(r)
    }
    vt_batch_sets(r, v, vff)
    var maps : array<uint>
    maps |> resize(nrec)
    unsafe {
        var mp = reinterpret<uint?>(r.meta.mapped)
        var cur = nrec * 4l
        for (l in range64(t.n_layer)) {
            let o = gemma3v_block_offs(t, l)
            for (g in range64(GEMMA3V_GEMMS)) {
                let rid = l * GEMMA3V_GEMMS + g
                maps[rid] = vt_sched_map(mp, cur, rid, o.w[g] / 32l, npos, g == GEMMA3V_GEMM_UP ? g_ff : g_d)
            }
        }
    }
    let bx = npos * d * 4l
    upload_region_at(r.x_dev, 0l, unsafe(addr<uint8 const?>(s.x[0])), bx)
    let nd = npos * d
    let nff = npos * ff
    let wg_nd = (nd / 4l + 255l) / 256l
    let wg_nff = (nff / 4l + 255l) / 256l
    let wg_pad = (npos * dp + 255l) / 256l
    let wg_fa = t.n_head * ((npos + (khr ? 15l : 63l)) / (khr ? 16l : 64l))
    let norms_base = t.layer_f
    var pc_fa = FaArgs(rows = uint(npos), w0 = 0u, qd = uint(dp), kvd = uint(dp), kv_mul = 1u, kvbase = 0u, scale = 1.0 / sqrt(float(hs)), window = 0u, sinkoff = 0u)
    var pc_rs = TowerRestrideArgs(rows = uint(npos), nheads = uint(t.n_head), hs = uint(hs), hsp = uint(hsp), sstride = uint(d), soff = 0u)
    var pc_rq = TowerClampRqArgs(inbase = 0u, nblk = uint(nd / 32l), lo = -VT_NO_CLAMP, hi = VT_NO_CLAMP)
    var pc_rqh = TowerClampRqArgs(inbase = 0u, nblk = uint(nff / 32l), lo = -VT_NO_CLAMP, hi = VT_NO_CLAMP)
    var pc_gd = BatchArgs(n = uint(d), d = uint(d), map_off = 0u)
    var pc_gff = BatchArgs(n = uint(d), d = uint(ff), map_off = 0u)
    var pc_dn = BatchArgs(n = uint(ff), d = uint(d), map_off = 0u)
    var pc_bias = TowerBiasArgs(d = uint(d), nelem = uint(nd), boff = 0u, gelu_on = 0u)
    var pc_gelu = TowerBiasArgs(d = uint(ff), nelem = uint(nff), boff = 0u, gelu_on = 1u)
    cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
        for (l in range64(t.n_layer)) {
            let o = gemma3v_block_offs(t, l)
            let rb = l * GEMMA3V_GEMMS
            if (l == 0l) {
                var pc = TowerLnArgs(d = uint(d), rows = uint(npos), woff = uint(o.ln1_w - norms_base), boff = uint(o.ln1_b - norms_base), eps = t.eps)
                enc_tower_ln_cls(raw, h, r.s_ln, pc, npos)
            }
            enc_tower_clamp_rq_cls(raw, h, r.s_rq_xb, pc_rq, wg_nd)
            pc_gd.map_off = maps[rb + GEMMA3V_GEMM_Q]
            q8_batch_cls_enc(v, raw, h, r.s_q, pc_gd, g_d)
            pc_bias.boff = uint(o.b[GEMMA3V_GEMM_Q] - norms_base)
            enc_tower_bias_act_cls(raw, h, r.s_bias_q, pc_bias, wg_nd)
            pc_gd.map_off = maps[rb + GEMMA3V_GEMM_K]
            q8_batch_cls_enc(v, raw, h, r.s_k, pc_gd, g_d)
            pc_bias.boff = uint(o.b[GEMMA3V_GEMM_K] - norms_base)
            enc_tower_bias_act_cls(raw, h, r.s_bias_k, pc_bias, wg_nd)
            pc_gd.map_off = maps[rb + GEMMA3V_GEMM_V]
            q8_batch_cls_enc(v, raw, h, r.s_v, pc_gd, g_d)
            pc_bias.boff = uint(o.b[GEMMA3V_GEMM_V] - norms_base)
            enc_tower_bias_act_cls(raw, h, r.s_bias_v, pc_bias, wg_nd)
            enc_tower_hpad_cls(raw, h, r.s_pad_q, pc_rs, wg_pad)
            enc_tower_hpad16_cls(raw, h, r.s_pad_k, pc_rs, wg_pad)
            enc_tower_hpad16_cls(raw, h, r.s_pad_v, pc_rs, wg_pad)
            if (hsp == 64l) {
                if (khr) {
                    enc_fa_khr_h64_bidir_cls(raw, h, r.s_fa, pc_fa, wg_fa)
                } else {
                    enc_fa_cm2_h64_bidir_cls(raw, h, r.s_fa, pc_fa, wg_fa)
                }
            } elif (khr) {
                enc_fa_khr_h128_bidir_cls(raw, h, r.s_fa, pc_fa, wg_fa)
            } else {
                enc_fa_cm2_h128_bidir_cls(raw, h, r.s_fa, pc_fa, wg_fa)
            }
            enc_tower_hunpad_cls(raw, h, r.s_unpad, pc_rs, wg_pad)
            enc_tower_clamp_rq_cls(raw, h, r.s_rq_att, pc_rq, wg_nd)
            pc_gd.map_off = maps[rb + GEMMA3V_GEMM_O]
            q8_batch_cls_enc(v, raw, h, r.s_o, pc_gd, g_d)
            var pc_post = TowerLnArgs(d = uint(d), rows = uint(npos), boff = uint(o.b[GEMMA3V_GEMM_O] - norms_base), eps = t.eps,
                ln_on = 1u, lnoff = uint(o.ln2_w - norms_base), lnboff = uint(o.ln2_b - norms_base))
            enc_tower_post_add_ln_cls(raw, h, r.s_post_o_ln, pc_post, npos)
            enc_tower_clamp_rq_cls(raw, h, r.s_rq_xb, pc_rq, wg_nd)
            pc_gff.map_off = maps[rb + GEMMA3V_GEMM_UP]
            q8_batch_cls_enc(vff, raw, h, r.s_hg, pc_gff, g_ff)   //! the up rows ride the gate slot's buffer (hg)
            pc_gelu.boff = uint(o.b[GEMMA3V_GEMM_UP] - norms_base)
            enc_tower_bias_act_cls(raw, h, r.s_bias_hg, pc_gelu, wg_nff)
            enc_tower_clamp_rq_cls(raw, h, r.s_rq_hg, pc_rqh, wg_nff)
            pc_dn.map_off = maps[rb + GEMMA3V_GEMM_DOWN]
            q8_batch_cls_enc(v, raw, h, r.s_down, pc_dn, g_d)
            let last = l + 1l == t.n_layer
            pc_post.boff = uint(o.b[GEMMA3V_GEMM_DOWN] - norms_base)
            pc_post.ln_on = last ? 0u : 1u
            if (!last) {
                let on = gemma3v_block_offs(t, l + 1l)
                pc_post.lnoff = uint(on.ln1_w - norms_base)
                pc_post.lnboff = uint(on.ln1_b - norms_base)
            }
            enc_tower_post_add_ln_cls(raw, h, r.s_post_d_ln, pc_post, npos)
        }
        vhz_dep(raw, h, VT_X, 0u, true)
        cmd_copy_range(raw, r.x_dev, 0l, r.host.buf, 0l, bx)
    }
    unsafe {
        memcpy(addr<void?>(s.x[0]), r.host.mapped, bx)
    }
    delete maps
    g_vt_encodes++
    g_vt_rows += npos
    g_vt_blocks += t.n_layer
    return true
}

def private q3v_regions(t : Qwen3vTower) : array<VtRegion> {
    let d = t.n_embd
    let ff = t.ff_pad
    var regions : array<VtRegion>
    regions |> reserve(t.n_layer * QWEN3V_GEMMS)
    for (l in range64(t.n_layer)) {
        let o = qwen3v_block_offs(t, l)
        regions |> push(VtRegion(woff = o.w[QWEN3V_GEMM_QKV], n = d, rows = 3l * d))
        regions |> push(VtRegion(woff = o.w[QWEN3V_GEMM_O], n = d, rows = d))
        regions |> push(VtRegion(woff = o.w[QWEN3V_GEMM_UP], n = d, rows = ff))
        regions |> push(VtRegion(woff = o.w[QWEN3V_GEMM_DOWN], n = ff, rows = d))
    }
    return <- regions
}

//! the tap index of block `l`, -1 off the taps
def private q3v_tap_of(t : Qwen3vTower; l : int64) : int64 => l == t.ds_l0 ? 0l : (l == t.ds_l1 ? 1l : (l == t.ds_l2 ? 2l : -1l))

[arch(at = "../ARCHITECTURE_GPU_TOWER.md#vk-tower-encode-chains")]
def private vulkan_qwen3v_blocks(t : Qwen3vTower; var s : Qwen3vState; npos : int64) : bool {   // nolint:STYLE038 — the dispatch chain is the shape
    if (!g_vt_env_tower) {
        return vt_decline(VulkanTowerDecline.knob)
    }
    if (!t.q8) {
        return vt_decline(VulkanTowerDecline.quant_mode)
    }
    let d = t.n_embd
    let ff = t.ff_pad
    let hs = d / t.n_head
    let hsp = hs <= 64l ? 64l : 128l
    let dp = t.n_head * hsp
    let nds = t.n_deepstack
    if (hs > 128l || hs % 2l != 0l || d % 64l != 0l || ff % 64l != 0l || npos > VT_MAX_ENCODE_ROWS || npos <= 0l) {
        return vt_decline(VulkanTowerDecline.shape)
    }
    if (!gpu_want_arms_tier() || !vk_moe_init()) {
        return vt_decline(VulkanTowerDecline.device)
    }
    let khr = !cm2_fa_on()
    if (khr && !khr_kq_tile_on()) {
        return vt_decline(VulkanTowerDecline.device)
    }
    var fa_ok = false
    if (hsp == 64l) {
        fa_ok = khr ? ensure_fa_khr_h64_bidir_cls() : ensure_fa_cm2_h64_bidir_cls()
    } else {
        fa_ok = khr ? ensure_fa_khr_h128_bidir_cls() : ensure_fa_cm2_h128_bidir_cls()
    }
    if (!fa_ok || !ensure_tower_ln_cls() || !ensure_tower_clamp_rq_cls() || !ensure_tower_bias_act_cls() || !ensure_tower_rope_tab_cls()
            || !ensure_tower_hpad_cls() || !ensure_tower_hpad16_cls() || !ensure_tower_hunpad_cls() || !ensure_tower_post_add_ln_cls()) {
        return vt_decline(VulkanTowerDecline.device)
    }
    let v = q8_batch_cls_variant(q8_batch_tile_for(d, npos), q8_batch_aligned(d, npos))
    let vff = q8_batch_cls_variant(q8_batch_tile_for(ff, npos), q8_batch_aligned(ff, npos))
    let vqkv = q8_batch_cls_variant(q8_batch_tile_for(3l * d, npos), q8_batch_aligned(3l * d, npos))
    if (!q8_batch_cls_ensure(v) || !q8_batch_cls_ensure(vff) || !q8_batch_cls_ensure(vqkv)) {
        return vt_decline(VulkanTowerDecline.device)
    }
    var r & = g_vt_q3v
    let key = vt_key(t.qblob, t.qscales.n, t.n_layer)
    if (r.key != key) {
        var regions <- q3v_regions(t)
        vt_upload(r, key, t.qblob, t.qscales, regions, t.blob, t.layer_f, t.n_layer * t.layer_f_stride, 0l)
        delete regions
    }
    let g_d = vt_gemm_groups(d, npos)
    let g_ff = vt_gemm_groups(ff, npos)
    let g_qkv = vt_gemm_groups(3l * d, npos)
    let nrec = t.n_layer * QWEN3V_GEMMS
    if (vt_scratch(r, npos, d, ff, hs, dp, (nrec * 4l + t.n_layer * (2l * g_d + g_ff + g_qkv)) * 4l, khr, true, nds)) {
        vt_sets_q3v(r)
    }
    vt_batch_sets(r, v, vff, vqkv)
    var maps : array<uint>
    maps |> resize(nrec)
    var mp = unsafe(reinterpret<uint?>(r.meta.mapped))
    var cur = nrec * 4l
    for (l in range64(t.n_layer)) {
        let o = qwen3v_block_offs(t, l)
        let rb = l * QWEN3V_GEMMS
        maps[rb + QWEN3V_GEMM_QKV] = vt_sched_map(mp, cur, rb + QWEN3V_GEMM_QKV, o.w[QWEN3V_GEMM_QKV] / 32l, npos, g_qkv)
        maps[rb + QWEN3V_GEMM_O] = vt_sched_map(mp, cur, rb + QWEN3V_GEMM_O, o.w[QWEN3V_GEMM_O] / 32l, npos, g_d)
        maps[rb + QWEN3V_GEMM_UP] = vt_sched_map(mp, cur, rb + QWEN3V_GEMM_UP, o.w[QWEN3V_GEMM_UP] / 32l, npos, g_ff)
        maps[rb + QWEN3V_GEMM_DOWN] = vt_sched_map(mp, cur, rb + QWEN3V_GEMM_DOWN, o.w[QWEN3V_GEMM_DOWN] / 32l, npos, g_d)
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
    let wg_nd = (nd / 4l + 255l) / 256l
    let wg_n3d = (3l * nd / 4l + 255l) / 256l
    let wg_nff = (nff / 4l + 255l) / 256l
    let wg_pad = (npos * dp + 255l) / 256l
    let wg_rope = (npos * t.n_head * half + 255l) / 256l
    let wg_fa = t.n_head * ((npos + (khr ? 15l : 63l)) / (khr ? 16l : 64l))
    let norms_base = t.layer_f
    var pc_fa = FaArgs(rows = uint(npos), w0 = 0u, qd = uint(dp), kvd = uint(dp), kv_mul = 1u, kvbase = 0u, scale = 1.0 / sqrt(float(hs)), window = 0u, sinkoff = 0u)
    var pc_rs = TowerRestrideArgs(rows = uint(npos), nheads = uint(t.n_head), hs = uint(hs), hsp = uint(hsp), sstride = uint(3l * d), soff = 0u)
    var pc_un = TowerRestrideArgs(rows = uint(npos), nheads = uint(t.n_head), hs = uint(hs), hsp = uint(hsp), sstride = uint(d), soff = 0u)   //! the attention rows are d-wide
    var pc_rope = TowerRopeTabArgs(sstride = uint(3l * d), soff = 0u, nheads = uint(t.n_head), hs = uint(hs), half = uint(half), rows = uint(npos))
    var pc_rq = TowerClampRqArgs(inbase = 0u, nblk = uint(nd / 32l), lo = -VT_NO_CLAMP, hi = VT_NO_CLAMP)
    var pc_rqh = TowerClampRqArgs(inbase = 0u, nblk = uint(nff / 32l), lo = -VT_NO_CLAMP, hi = VT_NO_CLAMP)
    var pc_gqkv = BatchArgs(n = uint(d), d = uint(3l * d), map_off = 0u)
    var pc_gd = BatchArgs(n = uint(d), d = uint(d), map_off = 0u)
    var pc_gff = BatchArgs(n = uint(d), d = uint(ff), map_off = 0u)
    var pc_dn = BatchArgs(n = uint(ff), d = uint(d), map_off = 0u)
    var pc_bqkv = TowerBiasArgs(d = uint(3l * d), nelem = uint(3l * nd), boff = 0u, gelu_on = 0u)
    var pc_gelu = TowerBiasArgs(d = uint(ff), nelem = uint(nff), boff = 0u, gelu_on = 1u)
    cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
        for (l in range64(t.n_layer)) {
            let o = qwen3v_block_offs(t, l)
            let rb = l * QWEN3V_GEMMS
            if (l == 0l) {
                var pc = TowerLnArgs(d = uint(d), rows = uint(npos), woff = uint(o.ln1_w - norms_base), boff = uint(o.ln1_b - norms_base), eps = t.eps)
                enc_tower_ln_cls(raw, h, r.s_ln, pc, npos)
            }
            enc_tower_clamp_rq_cls(raw, h, r.s_rq_xb, pc_rq, wg_nd)
            pc_gqkv.map_off = maps[rb + QWEN3V_GEMM_QKV]
            q8_batch_cls_enc(vqkv, raw, h, r.s_qkv, pc_gqkv, g_qkv)
            pc_bqkv.boff = uint(o.qkv_b - norms_base)
            enc_tower_bias_act_cls(raw, h, r.s_bias_qkv, pc_bqkv, wg_n3d)
            pc_rope.soff = 0u
            enc_tower_rope_tab_cls(raw, h, r.s_rope_tq, pc_rope, wg_rope)
            pc_rope.soff = uint(d)
            enc_tower_rope_tab_cls(raw, h, r.s_rope_tk, pc_rope, wg_rope)
            pc_rs.soff = 0u
            enc_tower_hpad_cls(raw, h, r.s_pad_q, pc_rs, wg_pad)
            pc_rs.soff = uint(d)
            enc_tower_hpad16_cls(raw, h, r.s_pad_k, pc_rs, wg_pad)
            pc_rs.soff = uint(2l * d)
            enc_tower_hpad16_cls(raw, h, r.s_pad_v, pc_rs, wg_pad)
            if (hsp == 64l) {
                if (khr) {
                    enc_fa_khr_h64_bidir_cls(raw, h, r.s_fa, pc_fa, wg_fa)
                } else {
                    enc_fa_cm2_h64_bidir_cls(raw, h, r.s_fa, pc_fa, wg_fa)
                }
            } elif (khr) {
                enc_fa_khr_h128_bidir_cls(raw, h, r.s_fa, pc_fa, wg_fa)
            } else {
                enc_fa_cm2_h128_bidir_cls(raw, h, r.s_fa, pc_fa, wg_fa)
            }
            enc_tower_hunpad_cls(raw, h, r.s_unpad, pc_un, wg_pad)
            enc_tower_clamp_rq_cls(raw, h, r.s_rq_att, pc_rq, wg_nd)
            pc_gd.map_off = maps[rb + QWEN3V_GEMM_O]
            q8_batch_cls_enc(v, raw, h, r.s_o, pc_gd, g_d)
            var pc_post = TowerLnArgs(d = uint(d), rows = uint(npos), boff = uint(o.out_b - norms_base), eps = t.eps,
                ln_on = 1u, lnoff = uint(o.ln2_w - norms_base), lnboff = uint(o.ln2_b - norms_base))
            enc_tower_post_add_ln_cls(raw, h, r.s_post_o_ln, pc_post, npos)
            enc_tower_clamp_rq_cls(raw, h, r.s_rq_xb, pc_rq, wg_nd)
            pc_gff.map_off = maps[rb + QWEN3V_GEMM_UP]
            q8_batch_cls_enc(vff, raw, h, r.s_hg, pc_gff, g_ff)   //! the up rows ride the gate slot's buffer (hg)
            pc_gelu.boff = uint(o.up_b - norms_base)
            enc_tower_bias_act_cls(raw, h, r.s_bias_hg, pc_gelu, wg_nff)
            enc_tower_clamp_rq_cls(raw, h, r.s_rq_hg, pc_rqh, wg_nff)
            pc_dn.map_off = maps[rb + QWEN3V_GEMM_DOWN]
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
            let k = q3v_tap_of(t, l)
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
    unsafe {
        memcpy(addr<void?>(s.x[0]), r.host.mapped, bx)
        if (nds > 0l) {
            memcpy(addr<void?>(s.ds_xtaps[0]), reinterpret<void?>(intptr(r.host.mapped) + uint64(bx)), nds * bx)
        }
    }
    delete maps
    g_vt_encodes++
    g_vt_rows += npos
    g_vt_blocks += t.n_layer
    return true
}

[arch(at = "../ARCHITECTURE_GPU_TOWER.md#vk-tower-encode-chains")]
def private vulkan_qwen25v_blocks(t : Qwen25vTower; var s : Qwen25vState; npos : int64) : bool {   // nolint:STYLE038 — the dispatch chain is the shape, its two attention arms the routing
    if (!g_vt_env_tower) {
        return vt_decline(VulkanTowerDecline.knob)
    }
    if (t.wblob.n <= 0l || t.blk_bf16) {
        return vt_decline(VulkanTowerDecline.quant_mode)
    }
    let d = t.n_embd
    let ff = t.ff_pad
    let hs = d / t.n_head
    let hsp = hs <= 64l ? 64l : 128l
    let dp = t.n_head * hsp
    let nwin = long_length(s.win_lo) - 1l
    var wmax = 0l
    for (i in range64(nwin)) {
        wmax = max(wmax, s.win_lo[i + 1l] - s.win_lo[i])
    }
    if (hs > 128l || hs % 2l != 0l || d % 64l != 0l || ff % 64l != 0l || npos > VT_MAX_ENCODE_ROWS || npos <= 0l || wmax > 64l || nwin <= 0l) {
        return vt_decline(VulkanTowerDecline.shape)
    }
    if (!gpu_want_arms_tier() || !vk_moe_init()) {
        return vt_decline(VulkanTowerDecline.device)
    }
    let khr = !cm2_fa_on()
    let gemm_khr = g_gpu.coopmat_mode != COOPMAT_CM2
    if ((khr || gemm_khr) && !khr_kq_tile_on()) {
        return vt_decline(VulkanTowerDecline.device)
    }
    var fa_ok = false
    if (hsp == 64l) {
        fa_ok = khr ? ensure_fa_khr_h64_bidir_cls() : ensure_fa_cm2_h64_bidir_cls()
    } else {
        fa_ok = khr ? ensure_fa_khr_h128_bidir_cls() : ensure_fa_cm2_h128_bidir_cls()
    }
    if (hsp == 64l) {
        return vt_decline(VulkanTowerDecline.shape)   //! the chain's sets are built at the 128 head alone (the family's 80-wide heads)
    }
    if (!fa_ok || !f16_gemm_ensure(gemm_khr) || !ensure_f16cvt_cls() || !ensure_tower_rms_cls() || !ensure_tower_bias_act_cls()
            || !ensure_tower_rope_tab_cls() || !ensure_tower_hpad_cls() || !ensure_tower_hpad16_cls() || !ensure_tower_hunpad_cls()
            || !ensure_tower_win_attn_cls() || !ensure_tower_post_add_rms_cls() || !ensure_tower_glu_act_cls()) {
        return vt_decline(VulkanTowerDecline.device)
    }
    var r & = g_vt_q25v
    let key = vt_key_u16(t.wblob, t.n_layer)
    let w_off = t.layer_w
    if (r.key != key) {
        vt_upload_f16(r, key, t.wblob, w_off, t.n_layer * t.layer_w_stride, t.blob, t.layer_f, t.n_layer * t.layer_f_stride)
    }
    if (vt_scratch_f16(r, npos, nwin, d, ff, hs, dp, khr)) {
        vt_sets_q25v(r, gemm_khr)
    }
    var wlo : array<uint>
    wlo |> resize(nwin + 1l)
    for (w in range64(nwin + 1l)) {
        wlo[w] = uint(s.win_lo[w])
    }
    let bx = npos * d * 4l
    let half = hs / 2l
    let btab = npos * half * 4l
    unsafe {
        upload_region_at(r.x_dev, 0l, addr<uint8 const?>(s.x[0]), bx)
        upload_region_at(r.cos_dev, 0l, addr<uint8 const?>(s.cos_t[0]), btab)
        upload_region_at(r.sin_dev, 0l, addr<uint8 const?>(s.sin_t[0]), btab)
        upload_region_at(r.wlo_dev, 0l, addr<uint8 const?>(wlo[0]), (nwin + 1l) * 4l)
    }
    delete wlo
    let nd = npos * d
    let nff = npos * ff
    let wg_nd = (nd / 4l + 255l) / 256l
    let wg_nff = (nff / 4l + 255l) / 256l
    let wg_pad = (npos * dp + 255l) / 256l
    let wg_rope = (npos * t.n_head * half + 255l) / 256l
    let wg_fa = t.n_head * ((npos + (khr ? 15l : 63l)) / (khr ? 16l : 64l))
    let norms_base = t.layer_f
    let scale = 1.0 / sqrt(float(hs))
    var pc_fa = FaArgs(rows = uint(npos), w0 = 0u, qd = uint(dp), kvd = uint(dp), kv_mul = 1u, kvbase = 0u, scale = scale, window = 0u, sinkoff = 0u)
    var pc_rs = TowerRestrideArgs(rows = uint(npos), nheads = uint(t.n_head), hs = uint(hs), hsp = uint(hsp), sstride = uint(d), soff = 0u)
    var pc_rope = TowerRopeTabArgs(sstride = uint(d), soff = 0u, nheads = uint(t.n_head), hs = uint(hs), half = uint(half), rows = uint(npos))
    var pc_cvt = ActArgs(nelem = uint(nd), nblk = 0u)
    var pc_cvth = ActArgs(nelem = uint(nff), nblk = 0u)
    var pc_gd = F16GemmArgs(n = uint(d), d = uint(d), rows = uint(npos), woff = 0u, ybase = 0u, ystride = uint(d), ksplit = 0u)
    var pc_gff = F16GemmArgs(n = uint(d), d = uint(ff), rows = uint(npos), woff = 0u, ybase = 0u, ystride = uint(ff), ksplit = 0u)
    var pc_dn = F16GemmArgs(n = uint(ff), d = uint(d), rows = uint(npos), woff = 0u, ybase = 0u, ystride = uint(d), ksplit = 0u)
    var pc_bias = TowerBiasArgs(d = uint(d), nelem = uint(nd), boff = 0u, gelu_on = 0u)
    var pc_glu = TowerGluArgs(d = uint(ff), nelem = uint(nff), bgoff = 0u, buoff = 0u)
    var pc_win = TowerWinArgs(d = uint(d), nheads = uint(t.n_head), hs = uint(hs), scale = scale)
    cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
        for (l in range64(t.n_layer)) {
            let o = qwen25v_block_offs(t, l)
            let full = t.n_wa_pattern <= 0l || (l + 1l) % t.n_wa_pattern == 0l
            if (l == 0l) {
                var pc = TowerRowArgs(d = uint(d), rows = uint(npos), woff = uint(o.ln1_w - norms_base), eps = t.eps)
                enc_tower_rms_cls(raw, h, r.s_rms, pc, npos)
            }
            enc_f16cvt_cls(raw, h, r.s_cvt_xb, pc_cvt, wg_nd)
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
                if (khr) {
                    enc_fa_khr_h128_bidir_cls(raw, h, r.s_fa, pc_fa, wg_fa)
                } else {
                    enc_fa_cm2_h128_bidir_cls(raw, h, r.s_fa, pc_fa, wg_fa)
                }
                enc_tower_hunpad_cls(raw, h, r.s_unpad, pc_rs, wg_pad)
            } else {
                enc_tower_win_attn_cls(raw, h, r.s_win, pc_win, nwin * t.n_head)
            }
            enc_f16cvt_cls(raw, h, r.s_cvt_att, pc_cvt, wg_nd)
            pc_gd.woff = uint(o.w[QWEN25V_GEMM_O] - w_off)
            f16_gemm_enc(gemm_khr, raw, h, r.s_o, pc_gd, npos, d)
            var pc_post = TowerLnArgs(d = uint(d), rows = uint(npos), boff = uint(o.b[QWEN25V_GEMM_O] - norms_base), eps = t.eps, ln_on = 1u, lnoff = uint(o.ln2_w - norms_base), lnboff = 0u)
            enc_tower_post_add_rms_cls(raw, h, r.s_post_o_ln, pc_post, npos)
            enc_f16cvt_cls(raw, h, r.s_cvt_xb, pc_cvt, wg_nd)
            pc_gff.woff = uint(o.w[QWEN25V_GEMM_GATE] - w_off)
            f16_gemm_enc(gemm_khr, raw, h, r.s_hg, pc_gff, npos, ff)
            pc_gff.woff = uint(o.w[QWEN25V_GEMM_UP] - w_off)
            f16_gemm_enc(gemm_khr, raw, h, r.s_hu, pc_gff, npos, ff)
            pc_glu.bgoff = uint(o.b[QWEN25V_GEMM_GATE] - norms_base)
            pc_glu.buoff = uint(o.b[QWEN25V_GEMM_UP] - norms_base)
            enc_tower_glu_act_cls(raw, h, r.s_glu, pc_glu, wg_nff)
            enc_f16cvt_cls(raw, h, r.s_cvt_hg, pc_cvth, wg_nff)
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
    unsafe {
        memcpy(addr<void?>(s.x[0]), r.host.mapped, bx)
    }
    g_vt_encodes++
    g_vt_rows += npos
    g_vt_blocks += t.n_layer
    return true
}

def private g4v_regions(t : Gemma4vTower) : array<VtRegion> {
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
    return <- regions
}

[arch(at = "../ARCHITECTURE_GPU_TOWER.md#vk-tower-encode-chains")]
def private vulkan_gemma4v_blocks(t : Gemma4vTower; var s : Gemma4vState; npos : int64) : bool {   // nolint:STYLE038 — the dispatch chain is the shape
    if (!g_vt_env_tower) {
        return vt_decline(VulkanTowerDecline.knob)
    }
    if (!t.q8) {
        return vt_decline(VulkanTowerDecline.quant_mode)
    }
    let d = t.n_embd
    let ff = t.n_ff
    let hd = d / t.n_head
    if (hd != 64l || d % 64l != 0l || ff % 64l != 0l || npos > VT_MAX_ENCODE_ROWS || npos <= 0l) {
        return vt_decline(VulkanTowerDecline.shape)
    }
    if (!gpu_want_arms_tier() || !vk_moe_init()) {
        return vt_decline(VulkanTowerDecline.device)
    }
    let khr = !cm2_fa_on()
    if (khr && !khr_kq_tile_on()) {
        return vt_decline(VulkanTowerDecline.device)
    }
    let fa_ok = khr ? ensure_fa_khr_h64_bidir_cls() : ensure_fa_cm2_h64_bidir_cls()
    if (!fa_ok || !ensure_tower_rms_cls() || !ensure_tower_clamp_cls() || !ensure_tower_clamp_rq_cls() || !ensure_tower_rope2d_cls()
            || !ensure_tower_post_add_cls() || !ensure_tower_geglu_cls() || !ensure_f16cvt_cls()) {
        return vt_decline(VulkanTowerDecline.device)
    }
    let v = q8_batch_cls_variant(q8_batch_tile_for(d, npos), q8_batch_aligned(d, npos))
    let vff = q8_batch_cls_variant(q8_batch_tile_for(ff, npos), q8_batch_aligned(ff, npos))
    if (!q8_batch_cls_ensure(v) || !q8_batch_cls_ensure(vff)) {
        return vt_decline(VulkanTowerDecline.device)
    }
    var r & = g_vt_g4v
    let key = vt_key(t.qblob, t.qscales.n, t.n_layer)
    if (r.key != key) {
        var regions <- g4v_regions(t)
        vt_upload(r, key, t.qblob, t.qscales, regions, t.blob, t.layer_f, t.n_layer * t.layer_f_stride, hd)
        delete regions
    }
    let g_d = vt_gemm_groups(d, npos)
    let g_ff = vt_gemm_groups(ff, npos)
    let nrec = t.n_layer * GEMMA4V_GEMMS
    if (vt_scratch(r, npos, d, ff, hd, 0l, (nrec * 4l + t.n_layer * (5l * g_d + 2l * g_ff)) * 4l, khr, false, 0l)) {
        vt_sets_g4v(r)
    }
    vt_batch_sets(r, v, vff)
    var maps : array<uint>
    maps |> resize(nrec)
    unsafe {
        var mp = reinterpret<uint?>(r.meta.mapped)
        var cur = nrec * 4l
        for (l in range64(t.n_layer)) {
            let o = gemma4v_block_offs(t, l)
            for (g in range64(GEMMA4V_GEMMS)) {
                let rid = l * GEMMA4V_GEMMS + g
                maps[rid] = vt_sched_map(mp, cur, rid, o.w[g] / 32l, npos, g == GEMMA4V_GEMM_GATE || g == GEMMA4V_GEMM_UP ? g_ff : g_d)
            }
        }
    }
    let pairs = hd / 4l
    g_vt_rope2d_tabs |> resize(npos * 4l * pairs)
    for (row in range64(npos)) {
        let rb = row * 4l * pairs
        for (j in range64(pairs)) {
            g_vt_rope2d_tabs[rb + j] = s.cos_x[row * pairs + j]
            g_vt_rope2d_tabs[rb + pairs + j] = s.sin_x[row * pairs + j]
            g_vt_rope2d_tabs[rb + 2l * pairs + j] = s.cos_y[row * pairs + j]
            g_vt_rope2d_tabs[rb + 3l * pairs + j] = s.sin_y[row * pairs + j]
        }
    }
    let bx = npos * d * 4l
    unsafe {
        upload_region_at(r.tabs_dev, 0l, addr<uint8 const?>(g_vt_rope2d_tabs[0]), npos * 4l * pairs * 4l)
        upload_region_at(r.x_dev, 0l, addr<uint8 const?>(s.x[0]), bx)
    }
    let nd = npos * d
    let nff = npos * ff
    let wg_nd = (nd / 4l + 255l) / 256l
    let wg_nff = (nff / 4l + 255l) / 256l
    let wg_rope = (npos * t.n_head * pairs + 255l) / 256l
    let wg_fa = t.n_head * ((npos + (khr ? 15l : 63l)) / (khr ? 16l : 64l))
    let norms_base = t.layer_f   //! the norms buffer holds the block rows from layer_f
    var pc_fa = FaArgs(rows = uint(npos), w0 = 0u, qd = uint(d), kvd = uint(d), kv_mul = 1u, kvbase = 0u, scale = 1.0, window = 0u, sinkoff = 0u)
    var pc_cvt = ActArgs(nelem = uint(nd), nblk = 0u)
    var pc_rope = TowerRopeArgs(d = uint(d), nheads = uint(t.n_head), hs = uint(hd), pairs = uint(pairs), rows = uint(npos))
    var pc_gd = BatchArgs(n = uint(d), d = uint(d), map_off = 0u)
    var pc_gff = BatchArgs(n = uint(d), d = uint(ff), map_off = 0u)
    var pc_dn = BatchArgs(n = uint(ff), d = uint(d), map_off = 0u)
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
            var pc_rq = TowerClampRqArgs(inbase = 0u, nblk = uint(nd / 32l), lo = vt_lo(cq, true), hi = vt_hi(cq, true))
            enc_tower_clamp_rq_cls(raw, h, r.s_rq_xb, pc_rq, wg_nd)
            pc_gd.map_off = maps[rb + GEMMA4V_GEMM_Q]
            q8_batch_cls_enc(v, raw, h, r.s_q, pc_gd, g_d)
            var pc_cl = TowerClampArgs(nelem = uint(nd), lo = vt_lo(cq, false), hi = vt_hi(cq, false))
            enc_tower_clamp_cls(raw, h, r.s_clamp_q, pc_cl, wg_nd)
            pc_rq.lo = vt_lo(ck, true)
            pc_rq.hi = vt_hi(ck, true)
            enc_tower_clamp_rq_cls(raw, h, r.s_rq_xb, pc_rq, wg_nd)
            pc_gd.map_off = maps[rb + GEMMA4V_GEMM_K]
            q8_batch_cls_enc(v, raw, h, r.s_k, pc_gd, g_d)
            pc_cl.lo = vt_lo(ck, false)
            pc_cl.hi = vt_hi(ck, false)
            enc_tower_clamp_cls(raw, h, r.s_clamp_k, pc_cl, wg_nd)
            pc_rq.lo = vt_lo(cv, true)
            pc_rq.hi = vt_hi(cv, true)
            enc_tower_clamp_rq_cls(raw, h, r.s_rq_xb, pc_rq, wg_nd)
            pc_gd.map_off = maps[rb + GEMMA4V_GEMM_V]
            q8_batch_cls_enc(v, raw, h, r.s_v, pc_gd, g_d)
            pc_cl.lo = vt_lo(cv, false)
            pc_cl.hi = vt_hi(cv, false)
            enc_tower_clamp_cls(raw, h, r.s_clamp_v, pc_cl, wg_nd)
            var pc_h = TowerRowArgs(d = uint(hd), rows = uint(npos * t.n_head), woff = uint(o.q_norm - norms_base), eps = t.eps)
            enc_tower_rms_cls(raw, h, r.s_hrms_q, pc_h, npos * t.n_head)
            pc_h.woff = uint(o.k_norm - norms_base)
            enc_tower_rms_cls(raw, h, r.s_hrms_k, pc_h, npos * t.n_head)
            pc_h.woff = uint(r.ones_off)
            enc_tower_rms_cls(raw, h, r.s_hrms_v, pc_h, npos * t.n_head)
            enc_tower_rope2d_cls(raw, h, r.s_rope_q, pc_rope, wg_rope)
            enc_tower_rope2d_cls(raw, h, r.s_rope_k, pc_rope, wg_rope)
            enc_f16cvt_cls(raw, h, r.s_cvt_k, pc_cvt, wg_nd)
            enc_f16cvt_cls(raw, h, r.s_cvt_v, pc_cvt, wg_nd)
            if (khr) {
                enc_fa_khr_h64_bidir_cls(raw, h, r.s_fa, pc_fa, wg_fa)
            } else {
                enc_fa_cm2_h64_bidir_cls(raw, h, r.s_fa, pc_fa, wg_fa)
            }
            pc_rq.lo = vt_lo(co, true)
            pc_rq.hi = vt_hi(co, true)
            enc_tower_clamp_rq_cls(raw, h, r.s_rq_att, pc_rq, wg_nd)
            pc_gd.map_off = maps[rb + GEMMA4V_GEMM_O]
            q8_batch_cls_enc(v, raw, h, r.s_o, pc_gd, g_d)
            pc_cl.lo = vt_lo(co, false)
            pc_cl.hi = vt_hi(co, false)
            enc_tower_clamp_cls(raw, h, r.s_clamp_yo, pc_cl, wg_nd)
            var pc_post = TowerRowArgs(d = uint(d), rows = uint(npos), woff = uint(o.attn_post - norms_base), eps = t.eps, ln_on = 1u, lnoff = uint(o.ln2 - norms_base))
            enc_tower_post_add_cls(raw, h, r.s_post_o, pc_post, npos)
            pc_rq.lo = vt_lo(cg, true)
            pc_rq.hi = vt_hi(cg, true)
            enc_tower_clamp_rq_cls(raw, h, r.s_rq_xb, pc_rq, wg_nd)
            pc_gff.map_off = maps[rb + GEMMA4V_GEMM_GATE]
            q8_batch_cls_enc(vff, raw, h, r.s_hg, pc_gff, g_ff)
            pc_rq.lo = vt_lo(cu, true)
            pc_rq.hi = vt_hi(cu, true)
            enc_tower_clamp_rq_cls(raw, h, r.s_rq_xb, pc_rq, wg_nd)
            pc_gff.map_off = maps[rb + GEMMA4V_GEMM_UP]
            q8_batch_cls_enc(vff, raw, h, r.s_hu, pc_gff, g_ff)
            var pc_gg = TowerGegluArgs(nelem = uint(nff), glo = vt_lo(cg, false), ghi = vt_hi(cg, false), ulo = vt_lo(cu, false), uhi = vt_hi(cu, false),
                dlo = vt_lo(cd, true), dhi = vt_hi(cd, true))
            enc_tower_geglu_cls(raw, h, r.s_geglu, pc_gg, wg_nff)
            var pc_rqd = TowerClampRqArgs(inbase = 0u, nblk = uint(nff / 32l), lo = -VT_NO_CLAMP, hi = VT_NO_CLAMP)
            enc_tower_clamp_rq_cls(raw, h, r.s_rq_hg, pc_rqd, wg_nff)
            pc_dn.map_off = maps[rb + GEMMA4V_GEMM_DOWN]
            q8_batch_cls_enc(v, raw, h, r.s_down, pc_dn, g_d)
            pc_cl.lo = vt_lo(cd, false)
            pc_cl.hi = vt_hi(cd, false)
            enc_tower_clamp_cls(raw, h, r.s_clamp_yd, pc_cl, wg_nd)
            let last = l + 1l == t.n_layer
            pc_post.woff = uint(o.ffn_post - norms_base)
            pc_post.ln_on = last ? 0u : 1u
            pc_post.lnoff = last ? 0u : uint(gemma4v_block_offs(t, l + 1l).ln1 - norms_base)
            enc_tower_post_add_cls(raw, h, r.s_post_d, pc_post, npos)
        }
        vhz_dep(raw, h, VT_X, 0u, true)
        cmd_copy_range(raw, r.x_dev, 0l, r.host.buf, 0l, bx)
    }
    unsafe {
        memcpy(addr<void?>(s.x[0]), r.host.mapped, bx)
    }
    delete maps
    g_vt_encodes++
    g_vt_rows += npos
    g_vt_blocks += t.n_layer
    return true
}

[init]
def dasllama_vulkan_tower_register {
    g_vt_env_tower = g_env_vulkan.vk_tower
    register_vk_drop_hook(@@vt_forget)
    static_if (!typeinfo builtin_module_exists(das_metal)) {
        register_gemma4v_gpu(@@vulkan_gemma4v_blocks, @@vulkan_tower_serves_exact)
        register_gemma3v_gpu(@@vulkan_gemma3v_blocks, @@vulkan_tower_serves_exact)
        register_qwen3v_gpu_q8(@@vulkan_qwen3v_blocks)
        register_qwen25v_gpu_blocks(@@vulkan_qwen25v_blocks)
    }
}
