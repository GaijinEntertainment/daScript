options gen2
options indenting = 4
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_vulkan_tower shared public

require dasllama/dasllama_lint public
require vulkan
require math
require dasllama/dasllama_env
require dasllama/dasllama_kqformat   // KqFmt.q8 - the batch tile family the block GEMMs ride
require dasllama/dasllama_math    // g_active_needs_repack and the active q8 interleave - what the gather undoes on upload
require dasllama/dasllama_layout  // q8_gather_rows - the prepared plane back to row-major words with f16 scales
require dasllama/dasllama_plane
require dasllama/dasllama_tower   // Clamp - the ClippableLinear bounds
require dasllama/dasllama_vulkan_common
require dasllama/dasllama_vulkan_classes
require dasllama/dasllama_gemma4v   // Gemma4vTower / Gemma4vState - the ViT block loop
require dasllama/dasllama_gemma3v   // Gemma3vTower / Gemma3vState - the SigLIP block loop
require dasllama/dasllama_qwen3v    // Qwen3vTower / Qwen3vState - the qwen3vl ViT block loop
require dasllama/dasllama_qwen25v   // Qwen25vTower / Qwen25vState - the window-attention ViT chain

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

//! why a chain declined, by name in the declines table
enum private VulkanTowerDecline {
    knob         //! DASLLAMA_VK_TOWER=0, or set_vulkan_tower(false)
    unserved     //! a family whose chain the driver does not carry yet
    quant_mode   //! the tower's planes are not the lane the driver serves
    shape        //! a width, head size or row count the stamps do not serve
    device       //! the tier has no device, or a class failed to build
    gpu_error    //! a submit failed
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

//! the lane policy's question - would the driver serve the file's exact planes: no, it serves the q8 image, so a box with
//! this driver keeps the q8 lane the CPU chain reads
def private vulkan_tower_serves_exact() : bool => false

// ===== the resident tower: the q8 planes and the norm rows uploaded once, keyed by the tower's planes =====

let private VT_FP_SAMPLES = 64l        //! words of the quant plane folded into the residency key beside its address and size
let private VT_NO_CLAMP = 3.4028235e38 //! an inactive ClippableLinear bound: the CPU chain skips the clamp, the device clamps to the float range
let private VT_META_CAP_ROWS = 4096l   //! the schedule buffer is sized for this many rows an encode; a taller canvas declines `shape`

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

//! one family's resident tower on the device: the whole block-GEMM plane as Q8_0 words, its scales as f16 halfwords (the
//! image's own halfwords where it carries them, converted on the way up otherwise), the per-block f32 rows with a ones row
//! appended for the weightless norms, and the scratch the last encode sized - grown when a taller canvas arrives
struct private VtResident {
    key : uint64
    wq_dev : uint64
    ws_dev : uint64
    wq_bytes : int64
    ws_bytes : int64
    norms_dev : uint64
    norms_bytes : int64
    ones_off : int64      //! the ones row's element offset in norms_dev
    meta : HostBuf        //! the batch schedules: one record and one map a GEMM dispatch
    cap_rows : int64      //! the scratch's row capacity (0 = none yet)
    d : int64
    ff : int64
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
    host : HostBuf        //! the x readback staging
    bvar : int            //! the q8 batch class variant the d-wide sets were built for (-1 = none)
    bvar_ff : int         //! ... and the ff-wide ones (gate, up): the tile pick reads the output width
    s_q : VkDescriptorSet
    s_k : VkDescriptorSet
    s_v : VkDescriptorSet
    s_o : VkDescriptorSet
    s_gate : VkDescriptorSet
    s_up : VkDescriptorSet
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
    fa_khr : bool
}

var private g_vt_g4v : VtResident   //! the gemma4v family's resident tower (one a family: a second mmproj replaces it)
var private g_vt_tabs : array<float>   //! the packed rope tables staging, [row][cx | sx | cy | sy]

//! the residency key: the quant plane's address and size folded with sampled words, so a tower freed and another mapped
//! at the same address with the same geometry reads as a new tower
def private vt_key(qblob : PlaneI8; qscales_n : int64) : uint64 {
    var k = intptr(unsafe(reinterpret<void?>(qblob.p))) * 0x9E3779B97F4A7C15ul + uint64(qblob.n) + uint64(qscales_n) * 0x100000001B3ul
    let step = max(qblob.n / VT_FP_SAMPLES, 1l)
    var i = 0l
    while (i < qblob.n) {
        unsafe {
            k = (k ^ uint64(uint8(qblob.p[i]))) * 0x100000001B3ul
        }
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
    destroy_host_buf(r.host)
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
        destroy_host_buf(r.meta)
        r.key = 0ul
    }
}

//! Release every device object the driver owns (the tests' leak rail beside the tier's shutdown); the next served encode
//! uploads again.
def vulkan_tower_shutdown {
    vt_release(g_vt_g4v)
}

//! the model drop's sweep takes every buffer and set the driver holds: forget the handles, the next encode rebuilds
def private vt_forget {
    g_vt_g4v = VtResident()
    g_vt_g4v.bvar = -1
    g_vt_g4v.bvar_ff = -1
}

//! one block GEMM's region in the tower's q8 plane: its element offset, its input width and its rows - the repack unit
struct private VtRegion {
    woff : int64
    n : int64
    rows : int64
}

//! the upload: every GEMM region gathered back to row-major Q8_0 words with f16 halfword scales (the CPU lane's plane
//! is the active backend's interleave), the block rows plus the ones row; the schedule buffer at its cap
[cold_path]
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
    unsafe {
        upload_region_at(r.norms_dev, 0l, addr<uint8 const?>(norms[0]), r.norms_bytes)
    }
    delete norms
    r.meta = make_host_buf(BATCH_META_BYTES, true)
    r.cap_rows = 0l
    r.bvar = -1
    r.bvar_ff = -1
}

//! the per-encode scratch at a row capacity, and every descriptor set over it (the batch sets follow the variant, picked
//! per encode from the row count - rebuilt when the pick moves)
[cold_path]
def private vt_scratch(var r : VtResident; rows, d, ff, hd : int64; khr : bool) {
    let cap = (rows + 63l) / 64l * 64l
    if (r.cap_rows >= cap && r.d == d && r.ff == ff) {
        return
    }
    vt_release_scratch(r)
    r.cap_rows = cap
    r.d = d
    r.ff = ff
    let bx = cap * d * 4l
    let bff = cap * ff * 4l
    let bq = cap * max(d, ff)
    let bs = bq / 32l * 4l
    let bh = cap * d * 2l
    let btabs = cap * hd * 4l   //! four tables of hd/4 pairs a row
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
    r.host = make_host_buf(bx, true, [cached = true])
    r.fa_khr = khr
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
    r.bvar = -1
    r.bvar_ff = -1
}

//! one batch tile set over the resident weights, the shared feed and one output role, at a variant
def private vt_batch_set(r : VtResident; v : int; y_dev : uint64; y_bytes : int64; y_bit : uint) : VkDescriptorSet {
    let bq = r.cap_rows * max(r.d, r.ff)
    return q8_batch_cls_set(v, fixed_array(r.wq_dev, r.ws_dev, r.meta.buf, r.xq_dev, r.xs_dev, y_dev),
        fixed_array(r.wq_bytes, r.ws_bytes, BATCH_META_BYTES, bq, bq / 32l * 4l, y_bytes), fixed_array(0u, 0u, 0u, VT_XQ, VT_XQ, y_bit))
}

//! the batch tile sets, one an output role: the d-wide roles at the variant the (d, rows) pick names, gate and up at the
//! (ff, rows) one
[cold_path]
def private vt_batch_sets(var r : VtResident; v, vff : int) {
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
        r.s_gate = vt_batch_set(r, vff, r.hg_dev, bff, VT_HG)
        r.s_up = vt_batch_set(r, vff, r.hu_dev, bff, VT_HU)
        r.bvar_ff = vff
    }
}

//! the schedule writer: record `rid` = [wblk0, 0, rows, 0] at word 4 rid, its map of `groups` words all naming it at the
//! cursor; returns the map's word offset and advances the cursor. The whole tower's dispatches share one buffer, so every
//! record the command buffer reads sits still until the submit returns.
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

//! the workgroups of one q8 batch GEMM at the variant's tile
def private vt_gemm_groups(d, rows : int64) : int64 {
    let te = batch_tile_edges(int(KqFmt.q8), d, rows)
    return ((d + te.td - 1l) / te.td) * ((rows + te.tc - 1l) / te.tc)
}

[unused_argument(t, s, npos)]
def private vulkan_gemma3v_blocks(t : Gemma3vTower; var s : Gemma3vState; npos : int64) : bool {
    if (!g_vt_env_tower) {
        return vt_decline(VulkanTowerDecline.knob)
    }
    return vt_decline(VulkanTowerDecline.unserved)
}

[unused_argument(t, s, npos)]
def private vulkan_qwen3v_blocks(t : Qwen3vTower; var s : Qwen3vState; npos : int64) : bool {
    if (!g_vt_env_tower) {
        return vt_decline(VulkanTowerDecline.knob)
    }
    return vt_decline(VulkanTowerDecline.unserved)
}

[unused_argument(t, s, npos)]
def private vulkan_qwen25v_blocks(t : Qwen25vTower; var s : Qwen25vState; npos : int64) : bool {
    if (!g_vt_env_tower) {
        return vt_decline(VulkanTowerDecline.knob)
    }
    return vt_decline(VulkanTowerDecline.unserved)
}

//! the gemma4v block GEMM regions in GEMMA4V_GEMM_* order, each its own repack unit
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

//! the gemma4v block loop as one command buffer over the q8 image, dispatch for dispatch the CPU loop's order: the q/k/v
//! GEMMs each over its own clamped Q8_0 feed, the per-head norms and the rope, the bidirectional flash tile, the o GEMM,
//! the attention seam, the gate and up GEMMs, the GEGLU with the three bounds folded, the down GEMM, the FFN seam
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
    if (hd != 64l || d % 64l != 0l || ff % 64l != 0l || npos > VT_META_CAP_ROWS || npos <= 0l) {
        return vt_decline(VulkanTowerDecline.shape)
    }
    if (!vk_moe_init()) {
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
    let key = vt_key(t.qblob, t.qscales.n)
    if (r.key != key) {
        var regions <- g4v_regions(t)
        vt_upload(r, key, t.qblob, t.qscales, regions, t.blob, t.layer_f, t.n_layer * t.layer_f_stride, hd)
        delete regions
    }
    vt_scratch(r, npos, d, ff, hd, khr)
    vt_batch_sets(r, v, vff)
    // the schedules: seven records and maps a block, all in one buffer
    let g_d = vt_gemm_groups(d, npos)
    let g_ff = vt_gemm_groups(ff, npos)
    let nrec = t.n_layer * GEMMA4V_GEMMS
    if ((nrec * 4l + t.n_layer * (5l * g_d + 2l * g_ff)) * 4l > BATCH_META_BYTES) {
        return vt_decline(VulkanTowerDecline.shape)
    }
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
    // the rope tables packed a row, the residual rows up
    let pairs = hd / 4l
    g_vt_tabs |> resize(npos * 4l * pairs)
    for (row in range64(npos)) {
        let rb = row * 4l * pairs
        for (j in range64(pairs)) {
            g_vt_tabs[rb + j] = s.cos_x[row * pairs + j]
            g_vt_tabs[rb + pairs + j] = s.sin_x[row * pairs + j]
            g_vt_tabs[rb + 2l * pairs + j] = s.cos_y[row * pairs + j]
            g_vt_tabs[rb + 3l * pairs + j] = s.sin_y[row * pairs + j]
        }
    }
    let bx = npos * d * 4l
    unsafe {
        upload_region_at(r.tabs_dev, 0l, addr<uint8 const?>(g_vt_tabs[0]), npos * 4l * pairs * 4l)
        upload_region_at(r.x_dev, 0l, addr<uint8 const?>(s.x[0]), bx)
    }
    let nd = npos * d
    let nff = npos * ff
    let wg_nd = (nd / 4l + 255l) / 256l
    let wg_nff = (nff / 4l + 255l) / 256l
    let wg_rope = (npos * t.n_head * pairs + 255l) / 256l
    let wg_fa = t.n_head * ((npos + (khr ? 15l : 63l)) / (khr ? 16l : 64l))
    let n_off = t.layer_f   //! the norms buffer holds the block rows from layer_f
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
                var pc = TowerRowArgs(d = uint(d), rows = uint(npos), woff = uint(o.ln1 - n_off), eps = t.eps)
                enc_tower_rms_cls(raw, h, r.s_rms, pc, npos)
            }
            // q, k, v: each over its own clamped feed and bounds
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
            // the per-head norms, the rope, the shadows, the flash tile
            var pc_h = TowerRowArgs(d = uint(hd), rows = uint(npos * t.n_head), woff = uint(o.q_norm - n_off), eps = t.eps)
            enc_tower_rms_cls(raw, h, r.s_hrms_q, pc_h, npos * t.n_head)
            pc_h.woff = uint(o.k_norm - n_off)
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
            // o, then the attention seam
            pc_rq.lo = vt_lo(co, true)
            pc_rq.hi = vt_hi(co, true)
            enc_tower_clamp_rq_cls(raw, h, r.s_rq_att, pc_rq, wg_nd)
            pc_gd.map_off = maps[rb + GEMMA4V_GEMM_O]
            q8_batch_cls_enc(v, raw, h, r.s_o, pc_gd, g_d)
            pc_cl.lo = vt_lo(co, false)
            pc_cl.hi = vt_hi(co, false)
            enc_tower_clamp_cls(raw, h, r.s_clamp_yo, pc_cl, wg_nd)
            var pc_post = TowerRowArgs(d = uint(d), rows = uint(npos), woff = uint(o.attn_post - n_off), eps = t.eps, ln_on = 1u, lnoff = uint(o.ln2 - n_off))
            enc_tower_post_add_cls(raw, h, r.s_post_o, pc_post, npos)
            // gate and up, the GEGLU with the three bounds, down over the plain feed, the FFN seam
            pc_rq.lo = vt_lo(cg, true)
            pc_rq.hi = vt_hi(cg, true)
            enc_tower_clamp_rq_cls(raw, h, r.s_rq_xb, pc_rq, wg_nd)
            pc_gff.map_off = maps[rb + GEMMA4V_GEMM_GATE]
            q8_batch_cls_enc(vff, raw, h, r.s_gate, pc_gff, g_ff)
            pc_rq.lo = vt_lo(cu, true)
            pc_rq.hi = vt_hi(cu, true)
            enc_tower_clamp_rq_cls(raw, h, r.s_rq_xb, pc_rq, wg_nd)
            pc_gff.map_off = maps[rb + GEMMA4V_GEMM_UP]
            q8_batch_cls_enc(vff, raw, h, r.s_up, pc_gff, g_ff)
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
            pc_post.woff = uint(o.ffn_post - n_off)
            pc_post.ln_on = last ? 0u : 1u
            pc_post.lnoff = last ? 0u : uint(gemma4v_block_offs(t, l + 1l).ln1 - n_off)
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
    static_if (!typeinfo builtin_module_exists(das_metal)) {   //! the Metal driver owns the slots on an Apple build
        register_gemma4v_gpu(@@vulkan_gemma4v_blocks, @@vulkan_tower_serves_exact)
        register_gemma3v_gpu(@@vulkan_gemma3v_blocks, @@vulkan_tower_serves_exact)
        register_qwen3v_gpu(@@vulkan_qwen3v_blocks, @@vulkan_tower_serves_exact)
        register_qwen25v_gpu(@@vulkan_qwen25v_blocks)
    }
}
