options gen2
options indenting = 4
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_vulkan_tts shared public

require dasllama/dasllama_lint public
require vulkan
require math
require dasllama/dasllama_math
require dasllama/dasllama_tts_blocks
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
let private TS_LSTM_MAX_HIDDEN = int64(TTS_LSTM_LANES)   //! the recurrence stamp's lanes

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

//! a device buffer as a dispatch binds it: the handle, its byte size and its hazard bit (0 on a read-only slab)
struct private TsRows {
    dev : uint64
    bytes : int64
    bit : uint
}

// ----- the slabs: one device buffer per part, keyed on the part's weights -----

struct private TsSlab {
    key : uint64
    dev : uint64
    bytes : int64
}

var private g_ts_albert : TsSlab
var private g_ts_albert_slots : TtsAlbertSlab
var private g_ts_text : TsSlab
var private g_ts_text_slots : TtsTextSlab

def private ts_slab_release(var sl : TsSlab) {
    if (sl.dev != 0ul) {
        destroy_device_buf(sl.dev)
    }
    sl.dev = 0ul
    sl.key = 0ul
    sl.bytes = 0l
}

def private ts_slab_rows(sl : TsSlab) : TsRows => TsRows(dev = sl.dev, bytes = sl.bytes, bit = 0u)

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

def private ts_text_attach(te : St2TextEncoder) : bool {
    let key = tts_text_key(te)
    return true if (g_ts_text.dev != 0ul && g_ts_text.key == key)
    return ts_text_rebuild(te, key)
}

[cold_path]
def private ts_text_rebuild(te : St2TextEncoder; key : uint64) : bool {
    var slots : TtsTextSlab
    let host <- tts_slab_build(slots) $(var w : TtsSlabWriter; var s : TtsTextSlab) {
        tts_text_write(w, te, s)
    }
    return false if (!ts_slab_upload(g_ts_text, key, host))
    delete g_ts_text_slots
    g_ts_text_slots <- slots
    return true
}

// ----- the seats' scratch: device rows sized once for the widest call, kept across calls -----

let private TS_SCRATCH_BUFS = 10

//! a seat's activation rows: one device buffer a slot, `widths[i]` elements a position (0 = no buffer), and the host
//! readback buffer the widest row set fills
struct private TsScratch {
    devs : uint64[TS_SCRATCH_BUFS]
    bytes : int64[TS_SCRATCH_BUFS]
    widths : int64[TS_SCRATCH_BUFS]
    hb : HostBuf
    cap_t : int64
}

var private g_ts_albert_scratch : TsScratch
var private g_ts_text_scratch : TsScratch

def private ts_scratch_release(var s : TsScratch) {
    for (i in range(TS_SCRATCH_BUFS)) {
        if (s.devs[i] != 0ul) {
            destroy_device_buf(s.devs[i])
        }
        s.devs[i] = 0ul
        s.bytes[i] = 0l
        s.widths[i] = 0l
    }
    if (s.hb.buf != 0ul) {
        destroy_host_buf(s.hb)
    }
    s.hb = HostBuf()
    s.cap_t = 0l
}

def private ts_scratch_fits(s : TsScratch; t : int64; widths : int64[TS_SCRATCH_BUFS]) : bool {
    return false if (s.cap_t < t)
    for (i in range(TS_SCRATCH_BUFS)) {
        return false if (s.widths[i] != widths[i])
    }
    return true
}

//! the rows sized for `t` positions at least (256 at a time, so a sentence a few tokens longer reuses them); false when an allocation failed
[cold_path]
def private ts_scratch_build(var s : TsScratch; t : int64; widths : int64[TS_SCRATCH_BUFS]) : bool {
    ts_scratch_release(s)
    let cap = (t + 255l) / 256l * 256l
    var oom = false
    var widest = 1l
    for (i in range(TS_SCRATCH_BUFS)) {
        s.widths[i] = widths[i]
        if (widths[i] > 0l) {
            s.bytes[i] = cap * widths[i] * 4l
            s.devs[i] = soft_device_buf(oom, s.bytes[i])
            widest = max(widest, widths[i])
        }
    }
    s.hb = make_host_buf(cap * widest * 4l, false, [cached = true, soft = true])
    if (oom || s.hb.buf == 0ul) {
        ts_scratch_release(s)
        return false
    }
    s.cap_t = cap
    return true
}

def private ts_scratch_ready(var s : TsScratch; t : int64; widths : int64[TS_SCRATCH_BUFS]) : bool
    => ts_scratch_fits(s, t, widths) || ts_scratch_build(s, t, widths)

//! the scratch's slot as a dispatch binds it; the hazard bit is the slot's
def private ts_rows(s : TsScratch; slot : int) : TsRows => TsRows(dev = s.devs[slot], bytes = s.bytes[slot], bit = 1u << uint(slot))

//! the model-drop sweep: every slab's and scratch's handle goes (the sweep destroys the device objects)
def private ts_forget {
    ts_slab_release(g_ts_albert)
    ts_slab_release(g_ts_text)
    ts_scratch_release(g_ts_albert_scratch)
    ts_scratch_release(g_ts_text_scratch)
}

// ----- the dispatches -----

def private ts_gemm_args(slot : TtsConvSlot; nin, nout, rows : int64) : F32GemmArgs
    => F32GemmArgs(n = uint(nin), d = uint(nout), rows = uint(rows), woff = uint(slot.woff), wstride = uint(slot.kdim_p), boff = uint(slot.boff),
        xoff = 0u, xstride = uint(nin), yoff = 0u, ystride = uint(nout))

//! a dense linear's GEMM: y[rows x nout] = x[rows x nin] W^T + b over the slab's rows, the f32 tile
def private ts_lin(raw : VkCommandBuffer; var h : VkHaz; slab : TsRows; slot : TtsConvSlot; nin, nout : int64; x, y : TsRows; rows : int64) {
    var s = set_f32_gemm_b_cls(fixed_array(slab.dev, x.dev, y.dev, slab.dev), fixed_array(slab.bytes, x.bytes, y.bytes, slab.bytes), fixed_array(0u, x.bit, y.bit, 0u))
    var pa = ts_gemm_args(slot, nin, nout, rows)
    enc_f32_gemm_b_cls(raw, h, s, pa, f32_gemm_wgs(rows, nout))
}

//! the same GEMM with its bias row left for the residual seam that follows it (`ts_add_ln` adds it there)
def private ts_lin_branch(raw : VkCommandBuffer; var h : VkHaz; slab : TsRows; slot : TtsConvSlot; nin, nout : int64; x, y : TsRows; rows : int64) {
    var s = set_f32_gemm_cls(fixed_array(slab.dev, x.dev, y.dev), fixed_array(slab.bytes, x.bytes, y.bytes), fixed_array(0u, x.bit, y.bit))
    var pa = ts_gemm_args(slot, nin, nout, rows)
    enc_f32_gemm_cls(raw, h, s, pa, f32_gemm_wgs(rows, nout))
}

//! layernorm rows in place against a norm slot's scale and shift rows
def private ts_ln(raw : VkCommandBuffer; var h : VkHaz; slab : TsRows; ns : TtsNormSlot; x : TsRows; rows, d : int64; eps : float) {
    var s = set_tower_ln_cls(fixed_array(x.dev, x.dev, slab.dev), fixed_array(x.bytes, x.bytes, slab.bytes), fixed_array(x.bit, x.bit, 0u))
    var pa = TowerLnArgs(d = uint(d), rows = uint(rows), woff = uint(ns.gwoff), boff = uint(ns.gboff), eps = eps, ln_on = 0u, lnoff = 0u, lnboff = 0u, ascale = 1.0)
    enc_tower_ln_cls(raw, h, s, pa, rows)
}

//! the post-LN residual seam: x += y + b (the branch linear's bias row, `bslot`), then xb = layernorm(x) on the norm slot's rows
def private ts_add_ln(raw : VkCommandBuffer; var h : VkHaz; slab : TsRows; bslot : TtsConvSlot; ns : TtsNormSlot; x, y, xb : TsRows; rows, d : int64; eps : float) {
    var s = set_tower_post_add_ln_cls(fixed_array(x.dev, y.dev, xb.dev, slab.dev), fixed_array(x.bytes, y.bytes, xb.bytes, slab.bytes), fixed_array(x.bit, y.bit, xb.bit, 0u))
    var pa = TowerLnArgs(d = uint(d), rows = uint(rows), woff = 0u, boff = uint(bslot.boff), eps = eps, ln_on = 1u, lnoff = uint(ns.gwoff), lnboff = uint(ns.gboff), ascale = 1.0)
    enc_tower_post_add_ln_cls(raw, h, s, pa, rows)
}

//! the leaky ReLU over x's first `nelem` elements in place
def private ts_leaky(raw : VkCommandBuffer; var h : VkHaz; x : TsRows; nelem : int64; slope : float) {
    var s = set_tts_leaky_cls(fixed_array(x.dev), fixed_array(x.bytes), fixed_array(x.bit))
    var pa = TtsElemArgs(nelem = uint(nelem), off = 0u, slope = slope)
    enc_tts_leaky_cls(raw, h, s, pa, tts_elem_wgs(nelem))
}

//! a forward conv on rows: x [tin][cin] gathered into col [tout][kdim_p], then the biased tile over the slot's rows into y [tout][cout]
def private ts_conv(raw : VkCommandBuffer; var h : VkHaz; slab : TsRows; slot : TtsConvSlot; cv : TtsConv1d; tin, tout : int64; x, col, y : TsRows) {
    var sc = set_tts_im2col_cls(fixed_array(x.dev, col.dev), fixed_array(x.bytes, col.bytes), fixed_array(x.bit, col.bit))
    var pc = TtsIm2colArgs(cin = uint(cv.cin), tin = uint(tin), tout = uint(tout), k = uint(cv.k), stride = uint(cv.stride), pad_l = uint(cv.pad_l),
        dilation = uint(cv.dilation), transposed = 0u, kdim = uint(slot.kdim_p), xoff = 0u, coloff = 0u, rows = 1u)
    enc_tts_im2col_cls(raw, h, sc, pc, tts_im2col_wgs(tout * slot.kdim_p))
    var sm = set_f32_gemm_b_cls(fixed_array(slab.dev, col.dev, y.dev, slab.dev), fixed_array(slab.bytes, col.bytes, y.bytes, slab.bytes), fixed_array(0u, col.bit, y.bit, 0u))
    var pm = F32GemmArgs(n = uint(cv.k * cv.cin), d = uint(cv.cout), rows = uint(tout), woff = uint(slot.woff), wstride = uint(slot.kdim_p), boff = uint(slot.boff),
        xoff = 0u, xstride = uint(slot.kdim_p), yoff = 0u, ystride = uint(cv.cout))
    enc_f32_gemm_b_cls(raw, h, sm, pm, f32_gemm_wgs(tout, cv.cout))
}

//! one LSTM direction: the input gates on the biased tile into g [t][4H], then the recurrence walk into its half of y [t][2H]
def private ts_lstm_dir(raw : VkCommandBuffer; var h : VkHaz; slab : TsRows; xs : TtsConvSlot; hh : int64; l : TtsLstm; t : int64; x, g, y : TsRows; backward : bool) {
    let hd = l.hidden
    ts_lin(raw, h, slab, xs, l.nin, 4l * hd, x, g, t)
    var s = set_tts_lstm_dir_cls(fixed_array(g.dev, slab.dev, y.dev), fixed_array(g.bytes, slab.bytes, y.bytes), fixed_array(g.bit, 0u, y.bit))
    var pa = TtsLstmArgs(t = uint(t), hidden = uint(hd), goff = 0u, gstride = uint(4l * hd), hhoff = uint(hh), hoff = backward ? uint(hd) : 0u,
        hstride = uint(2l * hd), backward = backward ? 1u : 0u)
    enc_tts_lstm_dir_cls(raw, h, s, pa, 1l)
}

//! the BiLSTM over x [t][nin]: the forward walk into y's first half, the backward into its second (the CPU `bilstm`'s rows)
def private ts_bilstm(raw : VkCommandBuffer; var h : VkHaz; slab : TsRows; ls : TtsLstmSlot; l : TtsBiLstm; t : int64; x, gf, gb, y : TsRows) {
    ts_lstm_dir(raw, h, slab, ls.fx, ls.fhh, l.fwd, t, x, gf, y, false)
    ts_lstm_dir(raw, h, slab, ls.bx, ls.bhh, l.bwd, t, x, gb, y, true)
}

// ----- the albert seat -----

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
    let widths = fixed_array<int64>(1l, a.emb_dim, d, d, d, d, d, d, ff, d)
    if (!ts_albert_attach(a) || !ts_scratch_ready(g_ts_albert_scratch, t, widths)) {
        return ts_decline("albert", VulkanTtsDecline.memory)
    }
    let sl & = g_ts_albert_slots
    let sc_ & = g_ts_albert_scratch
    let slab = ts_slab_rows(g_ts_albert)
    let r_ids = ts_rows(sc_, TS_A_IDS)
    let r_e = ts_rows(sc_, TS_A_E)
    let r_x = ts_rows(sc_, TS_A_X)
    let r_xb = ts_rows(sc_, TS_A_XB)
    let r_xb2 = ts_rows(sc_, TS_A_XB2)
    let r_q = ts_rows(sc_, TS_A_Q)
    let r_k = ts_rows(sc_, TS_A_K)
    let r_v = ts_rows(sc_, TS_A_V)
    let r_ffn = ts_rows(sc_, TS_A_FFN)
    let r_f2 = ts_rows(sc_, TS_A_F2)
    upload_region_at(r_ids.dev, 0l, unsafe(addr<uint8 const?>(ids[0])), t * 4l)
    var s_gather = set_tts_row_gather_cls(fixed_array(slab.dev, r_ids.dev, r_e.dev), fixed_array(slab.bytes, r_ids.bytes, r_e.bytes), fixed_array(0u, r_ids.bit, r_e.bit))
    var s_attn = set_tts_attn_cls(fixed_array(r_q.dev, r_k.dev, r_v.dev, r_xb2.dev), fixed_array(r_q.bytes, r_k.bytes, r_v.bytes, r_xb2.bytes),
        fixed_array(r_q.bit, r_k.bit, r_v.bit, r_xb2.bit))
    var s_gelu = set_tts_gelu_tanh_cls(fixed_array(r_ffn.dev), fixed_array(r_ffn.bytes), fixed_array(r_ffn.bit))
    var pa_gather = TtsGatherArgs(c = uint(a.emb_dim), rows = uint(t), word_off = uint(sl.word_emb), pos_off = uint(sl.pos_emb),
        type_off = uint(sl.type_emb), out_off = 0u, adds = 1u)
    var pa_attn = TtsAttnArgs(t = uint(t), d = uint(d), hs = uint(hs), heads = uint(a.heads), scale = 1.0 / sqrt(float(hs)),
        qoff = 0u, koff = 0u, voff = 0u, ooff = 0u)
    var pa_gelu = TtsElemArgs(nelem = uint(t * ff), off = 0u, slope = 0.0)
    let eps = ST2_ALBERT_EPS
    //! the CPU chain's rows: x the residual stream, xb2 the attention context and then the attention-normed rows the ffn reads
    //! and the second seam adds to, x again the full-normed rows the next layer reads
    vt_chain_one_shot("albert", a.n_layers, t) $(raw : VkCommandBuffer; var h : VkHaz) {
        enc_tts_row_gather_cls(raw, h, s_gather, pa_gather, tts_row_gather_wgs(t * a.emb_dim))
        ts_ln(raw, h, slab, sl.emb_ln, r_e, t, a.emb_dim, eps)
        ts_lin(raw, h, slab, sl.map_in, a.emb_dim, d, r_e, r_x, t)
        for (_l in range64(a.n_layers)) {
            ts_lin(raw, h, slab, sl.q, d, d, r_x, r_q, t)
            ts_lin(raw, h, slab, sl.k, d, d, r_x, r_k, t)
            ts_lin(raw, h, slab, sl.v, d, d, r_x, r_v, t)
            enc_tts_attn_cls(raw, h, s_attn, pa_attn, tts_attn_wgs(t, a.heads))
            ts_lin_branch(raw, h, slab, sl.dense, d, d, r_xb2, r_xb, t)
            ts_add_ln(raw, h, slab, sl.dense, sl.attn_ln, r_x, r_xb, r_xb2, t, d, eps)
            ts_lin(raw, h, slab, sl.ffn, d, ff, r_xb2, r_ffn, t)
            enc_tts_gelu_tanh_cls(raw, h, s_gelu, pa_gelu, tts_elem_wgs(t * ff))
            ts_lin_branch(raw, h, slab, sl.ffn_out, ff, d, r_ffn, r_f2, t)
            ts_add_ln(raw, h, slab, sl.ffn_out, sl.full_ln, r_xb2, r_f2, r_x, t, d, eps)
        }
    }
    unsafe(scratch_resize(out, t * d))
    var dst = unsafe(addr(out[0]))
    readback_range(r_x.dev, r_x.bit, sc_.hb, dst, t * d * 4l)
    g_ts_encodes++
    return true
}

// ----- the text seat -----

let private TS_T_IDS = 0
let private TS_T_A = 1
let private TS_T_B = 2
let private TS_T_COL = 3
let private TS_T_GF = 4
let private TS_T_GB = 5
let private TS_T_Y = 6

def private ts_text_shape_ok(te : St2TextEncoder; t : int64) : bool {
    let cc = te.c
    return false if (t < 1l || cc % 32l != 0l || length(te.cnn_ln) != length(te.cnn) || !tts_lstm_ok(te.lstm, cc, TS_LSTM_MAX_HIDDEN)
        || 2l * te.lstm.fwd.hidden != cc)
    for (cv in te.cnn) {
        return false if (cv.cin != cc || cv.cout != cc || cv.transposed || conv1d_out_len(cv, t) != t)
    }
    return tts_text_ok(te, TS_LSTM_MAX_HIDDEN)
}

def private ts_text_ensure : bool {
    return (ensure_f32_gemm_b_cls() && ensure_tower_ln_cls() && ensure_tts_row_gather_cls() && ensure_tts_leaky_cls()
        && ensure_tts_im2col_cls() && ensure_tts_lstm_dir_cls())
}

//! the widest im2col row the encoder's convs take
def private ts_text_kdim(te : St2TextEncoder) : int64 {
    var kdim = 32l
    for (cv in te.cnn) {
        kdim = max(kdim, tts_round_up(cv.k * cv.cin, 32l))
    }
    return kdim
}

//! The text encoder seat: the embedding rows, the conv + layernorm + leaky stack on rows, the BiLSTM; out channel-major [c][t]
[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_TTS.md#vk-tts-chain")]
def private vulkan_styletts2_text(te : St2TextEncoder; ids : array<int>; @scratch @exact_size var out : array<float>; var sc : St2Scratch) : bool {
    if (!vulkan_tower_on()) {
        return ts_decline("text", VulkanTtsDecline.knob)
    }
    let t = long_length(ids)
    let cc = te.c
    if (!ts_text_shape_ok(te, t)) {
        return ts_decline("text", VulkanTtsDecline.shape)
    }
    for (id in ids) {
        return ts_decline("text", VulkanTtsDecline.shape) if (id < 0 || int64(id + 1) * cc > long_length(te.emb.a))
    }
    if (!vulkan_tower_arms() || !ts_text_ensure()) {
        return ts_decline("text", VulkanTtsDecline.device)
    }
    let hd = te.lstm.fwd.hidden
    let widths = fixed_array<int64>(1l, cc, cc, ts_text_kdim(te), 4l * hd, 4l * hd, 2l * hd, 0l, 0l, 0l)
    if (!ts_text_attach(te) || !ts_scratch_ready(g_ts_text_scratch, t, widths)) {
        return ts_decline("text", VulkanTtsDecline.memory)
    }
    let sl & = g_ts_text_slots
    let sc_ & = g_ts_text_scratch
    let slab = ts_slab_rows(g_ts_text)
    let r_ids = ts_rows(sc_, TS_T_IDS)
    let r_a = ts_rows(sc_, TS_T_A)
    let r_b = ts_rows(sc_, TS_T_B)
    let r_col = ts_rows(sc_, TS_T_COL)
    let r_gf = ts_rows(sc_, TS_T_GF)
    let r_gb = ts_rows(sc_, TS_T_GB)
    let r_y = ts_rows(sc_, TS_T_Y)
    upload_region_at(r_ids.dev, 0l, unsafe(addr<uint8 const?>(ids[0])), t * 4l)
    var s_gather = set_tts_row_gather_cls(fixed_array(slab.dev, r_ids.dev, r_a.dev), fixed_array(slab.bytes, r_ids.bytes, r_a.bytes), fixed_array(0u, r_ids.bit, r_a.bit))
    var pa_gather = TtsGatherArgs(c = uint(cc), rows = uint(t), word_off = uint(sl.emb), pos_off = 0u, type_off = 0u, out_off = 0u, adds = 0u)
    let ncnn = length(te.cnn)
    vt_chain_one_shot("text", int64(ncnn), t) $(raw : VkCommandBuffer; var h : VkHaz) {
        enc_tts_row_gather_cls(raw, h, s_gather, pa_gather, tts_row_gather_wgs(t * cc))
        for (i in range(ncnn)) {   // the conv stack ping-pongs a and b, as the CPU chain does
            let src = i % 2 == 0 ? r_a : r_b
            let dst = i % 2 == 0 ? r_b : r_a
            ts_conv(raw, h, slab, sl.cnn[i], te.cnn[i], t, t, src, r_col, dst)
            ts_ln(raw, h, slab, sl.ln[i], dst, t, cc, ST2_ADAIN_EPS)
            ts_leaky(raw, h, dst, t * cc, 0.2)
        }
        let fin = ncnn % 2 == 1 ? r_b : r_a
        ts_bilstm(raw, h, slab, sl.lstm, te.lstm, t, fin, r_gf, r_gb, r_y)
    }
    var rows & = unsafe(sc.te_a)
    unsafe(scratch_resize(rows, t * cc))
    var dst = unsafe(addr(rows[0]))
    readback_range(r_y.dev, r_y.bit, sc_.hb, dst, t * cc * 4l)
    unsafe(scratch_resize(out, cc * t))
    transpose_tc(rows, t, cc, out)
    g_ts_encodes++
    return true
}

[init]
def dasllama_vulkan_tts_register {
    register_vk_drop_hook(@@ts_forget)
    static_if (!typeinfo builtin_module_exists(das_metal)) {
        register_styletts2_gpu(St2GpuDriver(albert = @@vulkan_styletts2_albert, text = @@vulkan_styletts2_text))
    }
}
