options gen2
options indenting = 4
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_vulkan_tts shared public

require dasllama/dasllama_lint public
require vulkan
require math
require dasllama/dasllama_math
require dasllama/dasllama_styletts2
require dasllama/dasllama_tts_slab
require dasllama/dasllama_vulkan_common
require dasllama/dasllama_vulkan_classes
require dasllama/dasllama_vulkan_tower

//! The StyleTTS2 Vulkan driver: the kitten and kokoro families' synthesis seats on the tower's
//! knob (`ARCHITECTURE_GPU_TOWER_VULKAN_TTS.md`); the Metal tower driver fills the same seats on
//! an Apple build.

var private g_ts_declines : VkDeclineCounter
var private g_ts_encodes = 0l   //! seats served, one per call

enum private VulkanTtsDecline {
    knob        //! DASLLAMA_VK_TOWER=0, or set_vulkan_tower(false)
    shape       //! a width off the lattice, a row count or head width the stamps do not serve
    device      //! the tier has no device, or a class failed to build
    memory      //! a device or pinned allocation failed
    gpu_error   //! a submit failed
}

let private TS_ATTN_MAX_T = 512l   //! the attention row stamp's key stage
let private TS_LSTM_MAX_HIDDEN = 256l   //! the recurrence stamp's lanes

//! the hazard bits of a seat's buffers
let private TS_IDS = 0x1u
let private TS_E = 0x2u
let private TS_X = 0x4u
let private TS_XB = 0x8u
let private TS_XB2 = 0x10u
let private TS_Q = 0x20u
let private TS_K = 0x40u
let private TS_V = 0x80u
let private TS_FFN = 0x100u
let private TS_F2 = 0x200u

//! seats served and declines counted since the process started
def vulkan_tts_stats : tuple<encodes : int64; declines : int64> => (encodes = g_ts_encodes, declines = g_ts_declines.total)

//! the declines by reason name
def vulkan_tts_declines : table<string; int64> {
    var out <- {for (k, v in keys(g_ts_declines.counts), values(g_ts_declines.counts)); k => v}
    return <- out
}

def private ts_decline_words(why : VulkanTtsDecline) : string {
    if (why == VulkanTtsDecline.knob) {
        return "the tower knob is off (DASLLAMA_VK_TOWER=0 or set_vulkan_tower(false))"
    } elif (why == VulkanTtsDecline.shape) {
        return "a width, head size, row count or hidden size the stamps do not serve"
    } elif (why == VulkanTtsDecline.memory) {
        return "a device allocation failed"
    } elif (why == VulkanTtsDecline.gpu_error) {
        return "a submit failed"
    }
    return "the tier has no device (DASLLAMA_GPU=1 arms it), or a class failed to build"
}

[cold_path]
def private ts_decline(seat : string; why : VulkanTtsDecline) : bool
    => g_ts_declines |> note("the {seat} seat", "{why}", "the seat stays on the CPU chain: {ts_decline_words(why)}")

// ----- the slabs: one device buffer per part, keyed on the part's weights -----

struct private TsSlab {
    key : uint64
    dev : uint64
    bytes : int64
}

var private g_ts_albert : TsSlab
var private g_ts_albert_slots : TtsAlbertSlab

def private ts_slab_release(var sl : TsSlab) {
    if (sl.dev != 0ul) {
        destroy_device_buf(sl.dev)
    }
    sl.dev = 0ul
    sl.key = 0ul
    sl.bytes = 0l
}

//! the host floats onto the device under `key`; false on an allocation failure
[cold_path]
def private ts_slab_upload(var sl : TsSlab; key : uint64; host : array<float>) : bool {
    ts_slab_release(sl)
    let bytes = long_length(host) * 4l
    return false if (bytes <= 0l)
    var oom = false
    sl.dev = soft_device_buf(oom, bytes)
    return false if (oom || sl.dev == 0ul)
    upload_region_at(sl.dev, 0l, unsafe(addr<uint8 const?>(host[0])), bytes)
    sl.bytes = bytes
    sl.key = key
    return true
}

//! the slab for this part stands under its key, else the cold rebuild lands it
def private ts_albert_attach(a : St2Albert) : bool {
    let key = tts_albert_key(a)
    return true if (g_ts_albert.dev != 0ul && g_ts_albert.key == key)
    return ts_albert_rebuild(a, key)
}

[cold_path]
def private ts_albert_rebuild(a : St2Albert; key : uint64) : bool {
    var slots : TtsAlbertSlab
    let host <- tts_slab_build(slots) $(var w : TtsSlabWriter; var s : TtsAlbertSlab) {
        tts_albert_write(w, a, s)
    }
    return false if (!ts_slab_upload(g_ts_albert, key, host))
    g_ts_albert_slots = slots
    return true
}

// ----- the seats' scratch: device rows sized once for the widest call, kept across calls -----

let private TS_SCRATCH_BUFS = 10

//! the albert seat's rows: ids, the embedding rows, x, two norm targets, q, k, v, the ffn rows and the ffn output, one host readback buffer
struct private TsAlbertScratch {
    devs : uint64[TS_SCRATCH_BUFS]
    bytes : int64[TS_SCRATCH_BUFS]
    hb : HostBuf
    cap_t : int64
    emb : int64
    d : int64
    ff : int64
}

var private g_ts_albert_scratch : TsAlbertScratch

let private TS_A_IDS = 0
let private TS_A_E = 1
let private TS_A_X = 2
let private TS_A_XB = 3
let private TS_A_XB2 = 4
let private TS_A_Q = 5
let private TS_A_K = 6
let private TS_A_V = 7
let private TS_A_FFN = 8
let private TS_A_F2 = 9

def private ts_albert_scratch_release {
    var s & = g_ts_albert_scratch
    for (i in range(TS_SCRATCH_BUFS)) {
        if (s.devs[i] != 0ul) {
            destroy_device_buf(s.devs[i])
        }
        s.devs[i] = 0ul
        s.bytes[i] = 0l
    }
    if (s.hb.buf != 0ul) {
        destroy_host_buf(s.hb)
    }
    s.hb = HostBuf()
    s.cap_t = 0l
}

def private ts_albert_scratch_fits(t, emb, d, ff : int64) : bool {
    let s & = g_ts_albert_scratch
    return s.cap_t >= t && s.emb == emb && s.d == d && s.ff == ff
}

//! the rows sized for `t` positions at least (256 at a time, so a sentence a few tokens longer reuses them); false when an allocation failed
[cold_path]
def private ts_albert_scratch_build(t, emb, d, ff : int64) : bool {
    ts_albert_scratch_release()
    var s & = g_ts_albert_scratch
    let cap = (t + 255l) / 256l * 256l
    let sizes = fixed_array<int64>(cap * 4l, cap * emb * 4l, cap * d * 4l, cap * d * 4l, cap * d * 4l, cap * d * 4l, cap * d * 4l, cap * d * 4l, cap * ff * 4l, cap * d * 4l)
    var oom = false
    for (i in range(TS_SCRATCH_BUFS)) {
        s.devs[i] = soft_device_buf(oom, sizes[i])
        s.bytes[i] = sizes[i]
    }
    s.hb = make_host_buf(cap * d * 4l, false, [cached = true, soft = true])
    if (oom || s.hb.buf == 0ul) {
        ts_albert_scratch_release()
        return false
    }
    s.cap_t = cap
    s.emb = emb
    s.d = d
    s.ff = ff
    return true
}

//! the model-drop sweep: every slab's and scratch's handle goes (the sweep destroys the device objects)
def private ts_forget {
    ts_slab_release(g_ts_albert)
    ts_albert_scratch_release()
}

def private ts_gemm_args(slot : TtsConvSlot; nin, nout, rows : int64) : F32GemmArgs
    => F32GemmArgs(n = uint(nin), d = uint(nout), rows = uint(rows), woff = uint(slot.woff), wstride = uint(slot.kdim_p), boff = uint(slot.boff),
        xoff = 0u, xstride = uint(nin), yoff = 0u, ystride = uint(nout))

//! a dense linear's GEMM: y[rows x nout] = x[rows x nin] W^T + b over the slab's rows, the f32 tile
def private ts_lin(raw : VkCommandBuffer; var h : VkHaz; slab : uint64; slab_bytes : int64; slot : TtsConvSlot; nin, nout : int64;
                   x : uint64; x_bytes : int64; x_bit : uint; y : uint64; y_bytes : int64; y_bit : uint; rows : int64) {
    var s = set_f32_gemm_b_cls(fixed_array(slab, x, y, slab), fixed_array(slab_bytes, x_bytes, y_bytes, slab_bytes), fixed_array(0u, x_bit, y_bit, 0u))
    var pa = ts_gemm_args(slot, nin, nout, rows)
    enc_f32_gemm_b_cls(raw, h, s, pa, f32_gemm_wgs(rows, nout))
}

//! the same GEMM with its bias row left for the residual seam that follows it (`ts_add_ln` adds it there)
def private ts_lin_branch(raw : VkCommandBuffer; var h : VkHaz; slab : uint64; slab_bytes : int64; slot : TtsConvSlot; nin, nout : int64;
                          x : uint64; x_bytes : int64; x_bit : uint; y : uint64; y_bytes : int64; y_bit : uint; rows : int64) {
    var s = set_f32_gemm_cls(fixed_array(slab, x, y), fixed_array(slab_bytes, x_bytes, y_bytes), fixed_array(0u, x_bit, y_bit))
    var pa = ts_gemm_args(slot, nin, nout, rows)
    enc_f32_gemm_cls(raw, h, s, pa, f32_gemm_wgs(rows, nout))
}

//! layernorm rows in place against a norm slot's scale and shift rows
def private ts_ln(raw : VkCommandBuffer; var h : VkHaz; slab : uint64; slab_bytes : int64; ns : TtsNormSlot; x : uint64; x_bytes : int64; x_bit : uint;
                  rows, d : int64; eps : float) {
    var s = set_tower_ln_cls(fixed_array(x, x, slab), fixed_array(x_bytes, x_bytes, slab_bytes), fixed_array(x_bit, x_bit, 0u))
    var pa = TowerLnArgs(d = uint(d), rows = uint(rows), woff = uint(ns.gwoff), boff = uint(ns.gboff), eps = eps, ln_on = 0u, lnoff = 0u, lnboff = 0u, ascale = 1.0)
    enc_tower_ln_cls(raw, h, s, pa, rows)
}

//! the post-LN residual seam: x += y + b (the branch linear's bias row, `bslot`), then xb = layernorm(x) on the norm slot's rows
def private ts_add_ln(raw : VkCommandBuffer; var h : VkHaz; slab : uint64; slab_bytes : int64; bslot : TtsConvSlot; ns : TtsNormSlot;
                      x : uint64; x_bit : uint; y : uint64; y_bit : uint; xb : uint64; xb_bit : uint; row_bytes : int64; rows, d : int64; eps : float) {
    var s = set_tower_post_add_ln_cls(fixed_array(x, y, xb, slab), fixed_array(row_bytes, row_bytes, row_bytes, slab_bytes), fixed_array(x_bit, y_bit, xb_bit, 0u))
    var pa = TowerLnArgs(d = uint(d), rows = uint(rows), woff = 0u, boff = uint(bslot.boff), eps = eps, ln_on = 1u, lnoff = uint(ns.gwoff), lnboff = uint(ns.gboff), ascale = 1.0)
    enc_tower_post_add_ln_cls(raw, h, s, pa, rows)
}

def private ts_albert_shape_ok(a : St2Albert; t : int64) : bool {
    let d = a.hidden
    let hs = a.heads > 0l ? d / a.heads : 0l
    let ff = a.ffn.nout
    return (t >= 1l && t <= a.max_pos && t <= TS_ATTN_MAX_T && d % 64l == 0l && (hs == 64l || hs == 128l) && hs * a.heads == d
        && ff % 64l == 0l && a.emb_dim % 32l == 0l && a.map_in.nin == a.emb_dim && a.map_in.nout == d && a.q.nout == d && a.k.nout == d
        && a.v.nout == d && a.dense.nout == d && a.ffn.nin == d && a.ffn_out.nin == ff && a.ffn_out.nout == d
        && long_length(a.pos_emb.a) >= t * a.emb_dim && long_length(a.type_emb.a) >= a.emb_dim && a.n_layers >= 1l && tts_albert_ok(a))
}

def private ts_albert_ensure : bool {
    return (ensure_f32_gemm_cls() && ensure_f32_gemm_b_cls() && ensure_tower_ln_cls() && ensure_tower_post_add_ln_cls()
        && ensure_tts_row_gather_cls() && ensure_tts_gelu_tanh_cls() && ensure_tts_attn_cls())
}

//! The PL-BERT seat: the embeddings gathered and normed, the map-in, then the one ALBERT layer n_layers times, out [t][hidden]
[hot_path, unused_argument(sc), arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_TTS.md#vk-tts-chain")]
def private vulkan_styletts2_albert(a : St2Albert; ids : array<int>; @scratch @exact_size var out : array<float>; var sc : St2Scratch) : bool {
    if (!vulkan_tower_on()) {
        return ts_decline("albert", VulkanTtsDecline.knob)
    }
    let t = long_length(ids)
    if (!ts_albert_shape_ok(a, t)) {
        return ts_decline("albert", VulkanTtsDecline.shape)
    }
    for (id in ids) {
        return ts_decline("albert", VulkanTtsDecline.shape) if (id < 0 || int64(id + 1) * a.emb_dim > long_length(a.word_emb.a))
    }
    if (!vulkan_tower_arms() || !ts_albert_ensure()) {
        return ts_decline("albert", VulkanTtsDecline.device)
    }
    let d = a.hidden
    let hs = d / a.heads
    let ff = a.ffn.nout
    if (!ts_albert_attach(a) || (!ts_albert_scratch_fits(t, a.emb_dim, d, ff) && !ts_albert_scratch_build(t, a.emb_dim, d, ff))) {
        return ts_decline("albert", VulkanTtsDecline.memory)
    }
    let sl & = g_ts_albert_slots
    let sc_ & = g_ts_albert_scratch
    let slab = g_ts_albert.dev
    let sb = g_ts_albert.bytes
    let be = sc_.bytes[TS_A_E]
    let bx = sc_.bytes[TS_A_X]
    let bff = sc_.bytes[TS_A_FFN]
    let bids = sc_.bytes[TS_A_IDS]
    let ids_d = sc_.devs[TS_A_IDS]
    let e_d = sc_.devs[TS_A_E]
    let x_d = sc_.devs[TS_A_X]
    let xb_d = sc_.devs[TS_A_XB]
    let xb2_d = sc_.devs[TS_A_XB2]
    let q_d = sc_.devs[TS_A_Q]
    let k_d = sc_.devs[TS_A_K]
    let v_d = sc_.devs[TS_A_V]
    let ffn_d = sc_.devs[TS_A_FFN]
    let f2_d = sc_.devs[TS_A_F2]
    upload_region_at(ids_d, 0l, unsafe(addr<uint8 const?>(ids[0])), t * 4l)
    var s_gather = set_tts_row_gather_cls(fixed_array(slab, ids_d, e_d), fixed_array(sb, bids, be), fixed_array(0u, TS_IDS, TS_E))
    var s_attn = set_tts_attn_cls(fixed_array(q_d, k_d, v_d, xb2_d), fixed_array(bx, bx, bx, bx), fixed_array(TS_Q, TS_K, TS_V, TS_XB2))
    var s_gelu = set_tts_gelu_tanh_cls(fixed_array(ffn_d), fixed_array(bff), fixed_array(TS_FFN))
    let ne = t * a.emb_dim
    var pa_gather = TtsGatherArgs(c = uint(a.emb_dim), rows = uint(t), word_off = uint(sl.word_emb), pos_off = uint(sl.pos_emb),
        type_off = uint(sl.type_emb), out_off = 0u)
    var pa_attn = TtsAttnArgs(t = uint(t), d = uint(d), hs = uint(hs), heads = uint(a.heads), scale = 1.0 / sqrt(float(hs)),
        qoff = 0u, koff = 0u, voff = 0u, ooff = 0u)
    var pa_gelu = TtsElemArgs(nelem = uint(t * ff), off = 0u, slope = 0.0)
    let eps = ST2_ALBERT_EPS
    //! the CPU chain's rows: x the residual stream, xb2 the attention context and then the attention-normed rows the ffn reads
    //! and the second seam adds to, x again the full-normed rows the next layer reads
    vt_chain_one_shot("albert", a.n_layers, t) $(raw : VkCommandBuffer; var h : VkHaz) {
        enc_tts_row_gather_cls(raw, h, s_gather, pa_gather, tts_row_gather_wgs(ne))
        ts_ln(raw, h, slab, sb, sl.emb_ln, e_d, be, TS_E, t, a.emb_dim, eps)
        ts_lin(raw, h, slab, sb, sl.map_in, a.emb_dim, d, e_d, be, TS_E, x_d, bx, TS_X, t)
        for (_l in range64(a.n_layers)) {
            ts_lin(raw, h, slab, sb, sl.q, d, d, x_d, bx, TS_X, q_d, bx, TS_Q, t)
            ts_lin(raw, h, slab, sb, sl.k, d, d, x_d, bx, TS_X, k_d, bx, TS_K, t)
            ts_lin(raw, h, slab, sb, sl.v, d, d, x_d, bx, TS_X, v_d, bx, TS_V, t)
            enc_tts_attn_cls(raw, h, s_attn, pa_attn, tts_attn_wgs(t, a.heads))
            ts_lin_branch(raw, h, slab, sb, sl.dense, d, d, xb2_d, bx, TS_XB2, xb_d, bx, TS_XB, t)
            ts_add_ln(raw, h, slab, sb, sl.dense, sl.attn_ln, x_d, TS_X, xb_d, TS_XB, xb2_d, TS_XB2, bx, t, d, eps)
            ts_lin(raw, h, slab, sb, sl.ffn, d, ff, xb2_d, bx, TS_XB2, ffn_d, bff, TS_FFN, t)
            enc_tts_gelu_tanh_cls(raw, h, s_gelu, pa_gelu, tts_elem_wgs(t * ff))
            ts_lin_branch(raw, h, slab, sb, sl.ffn_out, ff, d, ffn_d, bff, TS_FFN, f2_d, bx, TS_F2, t)
            ts_add_ln(raw, h, slab, sb, sl.ffn_out, sl.full_ln, xb2_d, TS_XB2, f2_d, TS_F2, x_d, TS_X, bx, t, d, eps)
        }
    }
    unsafe(scratch_resize(out, t * d))
    var dst = unsafe(addr(out[0]))
    readback_range(x_d, TS_X, sc_.hb, dst, t * d * 4l)
    g_ts_encodes++
    return true
}

[init]
def dasllama_vulkan_tts_register {
    register_vk_drop_hook(@@ts_forget)
    static_if (!typeinfo builtin_module_exists(das_metal)) {
        register_styletts2_gpu(St2GpuDriver(albert = @@vulkan_styletts2_albert))
    }
}
