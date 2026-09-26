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
var private g_ts_pred : TsSlab
var private g_ts_pred_slots : TtsPredSlab

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

def private ts_pred_attach(p : St2Predictor) : bool {
    let key = tts_pred_key(p)
    return true if (g_ts_pred.dev != 0ul && g_ts_pred.key == key)
    return ts_pred_rebuild(p, key)
}

[cold_path]
def private ts_pred_rebuild(p : St2Predictor; key : uint64) : bool {
    var slots : TtsPredSlab
    let host <- tts_slab_build(slots) $(var w : TtsSlabWriter; var s : TtsPredSlab) {
        tts_pred_write(w, p, s)
    }
    return false if (!ts_slab_upload(g_ts_pred, key, host))
    delete g_ts_pred_slots
    g_ts_pred_slots <- slots
    return true
}

// ----- the seats' scratch: device rows sized once for the widest call, kept across calls -----

let private TS_SCRATCH_BUFS = 30
let private TS_AUX_BIT = 1u << 30u       //! the hazard bit of a scratch's aux rows
let private TS_STATS_BIT = 1u << 31u     //! ... and of its column stats
let private TS_SCRATCH_GRAIN = 65536l    //! a slot grows in 256 KB steps, so a call a little wider reuses it

//! a seat's activation rows: one device buffer a slot sized in floats (0 = no buffer), the host readback buffer the
//! widest slot fills, and two more row sets - `aux`, the per-call uploads (a style vector, fc(style) per norm), and
//! `stats`, the AdaIN column stats the device writes
struct private TsScratch {
    devs : uint64[TS_SCRATCH_BUFS]
    bytes : int64[TS_SCRATCH_BUFS]
    hb : HostBuf
    hb_bytes : int64
    aux : TsRows
    stats : TsRows
}

//! a seat's slot sizes in floats, filled by slot name
typedef TsFloats = int64[TS_SCRATCH_BUFS]

var private g_ts_albert_scratch : TsScratch
var private g_ts_text_scratch : TsScratch
var private g_ts_de_scratch : TsScratch
var private g_ts_du_scratch : TsScratch
var private g_ts_pr_scratch : TsScratch

def private ts_rows_release(var r : TsRows) {
    if (r.dev != 0ul) {
        destroy_device_buf(r.dev)
    }
    r.dev = 0ul
    r.bytes = 0l
}

def private ts_scratch_release(var s : TsScratch) {
    for (i in range(TS_SCRATCH_BUFS)) {
        if (s.devs[i] != 0ul) {
            destroy_device_buf(s.devs[i])
        }
        s.devs[i] = 0ul
        s.bytes[i] = 0l
    }
    ts_rows_release(s.aux)
    ts_rows_release(s.stats)
    if (s.hb.buf != 0ul) {
        destroy_host_buf(s.hb)
    }
    s.hb = HostBuf()
    s.hb_bytes = 0l
}

//! every slot holds its floats, the readback buffer the widest of them
def private ts_scratch_fits(s : TsScratch; floats : TsFloats; aux_floats, stats_floats : int64) : bool {
    return false if (s.aux.bytes < aux_floats * 4l || s.stats.bytes < stats_floats * 4l)
    var widest = 0l
    for (i in range(TS_SCRATCH_BUFS)) {
        return false if (s.bytes[i] < floats[i] * 4l)
        widest = max(widest, floats[i])
    }
    return s.hb_bytes >= widest * 4l
}

//! `floats` floats on the device under `bit`, none at zero; false when the allocation failed
[cold_path]
def private ts_rows_alloc(var r : TsRows; floats : int64; bit : uint) : bool {
    r = TsRows(bit = bit)
    return true if (floats <= 0l)
    var oom = false
    r.bytes = floats * 4l
    r.dev = soft_device_buf(oom, r.bytes)
    return !oom && r.dev != 0ul
}

//! the slots sized for this call at least, each in 256 KB steps (so a call a little wider reuses them); false when an allocation failed
[cold_path]
def private ts_scratch_build(var s : TsScratch; floats : TsFloats; aux_floats, stats_floats : int64) : bool {
    ts_scratch_release(s)
    var oom = false
    var widest = 1l
    for (i in range(TS_SCRATCH_BUFS)) {
        if (floats[i] > 0l) {
            let n = tts_round_up(floats[i], TS_SCRATCH_GRAIN)
            s.bytes[i] = n * 4l
            s.devs[i] = soft_device_buf(oom, s.bytes[i])
            widest = max(widest, n)
        }
    }
    s.hb_bytes = widest * 4l
    s.hb = make_host_buf(s.hb_bytes, false, [cached = true, soft = true])
    if (oom || s.hb.buf == 0ul || !ts_rows_alloc(s.aux, aux_floats, TS_AUX_BIT) || !ts_rows_alloc(s.stats, stats_floats, TS_STATS_BIT)) {
        ts_scratch_release(s)
        return false
    }
    return true
}

def private ts_scratch_ready(var s : TsScratch; floats : TsFloats; aux_floats, stats_floats : int64) : bool
    => ts_scratch_fits(s, floats, aux_floats, stats_floats) || ts_scratch_build(s, floats, aux_floats, stats_floats)

//! the scratch's slot as a dispatch binds it; the hazard bit is the slot's
def private ts_rows(s : TsScratch; slot : int) : TsRows => TsRows(dev = s.devs[slot], bytes = s.bytes[slot], bit = 1u << uint(slot))

//! the model-drop sweep: every slab's and scratch's handle goes (the sweep destroys the device objects)
def private ts_forget {
    ts_slab_release(g_ts_albert)
    ts_slab_release(g_ts_text)
    ts_slab_release(g_ts_pred)
    ts_scratch_release(g_ts_albert_scratch)
    ts_scratch_release(g_ts_text_scratch)
    ts_scratch_release(g_ts_de_scratch)
    ts_scratch_release(g_ts_du_scratch)
    ts_scratch_release(g_ts_pr_scratch)
}

// ----- the aux rows a seat uploads per call: the style vector, then fc(style) per norm (gamma, then beta; +1 on gamma in the LN form) -----

var private @scratch g_ts_aux_host : array<float>   //! the staged rows, `g_ts_aux_n` of them live; grow-only
var private g_ts_aux_n = 0l
var private @scratch g_ts_h_tmp : array<float>
var private @scratch g_ts_idx_host : array<int>

def private ts_aux_reset {
    g_ts_aux_n = 0l
}

//! `n` floats of `src` at the next 16-byte boundary of the staged rows; the element offset
def private ts_aux_push(src : array<float>; n : int64) : int64 {
    let at = tts_round_up(g_ts_aux_n, 4l)
    g_ts_aux_host |> ensure_length(at + n)
    for (i in range64(n)) {
        g_ts_aux_host[at + i] = src[i]
    }
    g_ts_aux_n = at + n
    return at
}

//! fc(style) as the norm's rows: gamma [c] then beta [c], gamma + 1 where the norm applies it as a layernorm weight; the element offset
def private ts_style_rows(fc : TtsLinear; s : array<float>; c : int64; ln_form : bool) : int64 {
    linear_vec(fc, s, g_ts_h_tmp)
    let at = ts_aux_push(g_ts_h_tmp, 2l * c)
    if (ln_form) {
        for (i in range64(c)) {
            g_ts_aux_host[at + i]++
        }
    }
    return at
}

//! the staged rows onto the scratch's aux buffer; false where the seat sized it short
def private ts_aux_upload(s : TsScratch) : bool {
    let n = g_ts_aux_n
    return true if (n == 0l)
    return false if (n * 4l > s.aux.bytes)
    upload_region_at(s.aux.dev, 0l, unsafe(addr<uint8 const?>(g_ts_aux_host[0])), n * 4l)
    return true
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
    var fl : TsFloats
    fl[TS_A_IDS] = t
    fl[TS_A_E] = t * a.emb_dim
    for (i in range(TS_A_X, TS_A_V + 1)) {
        fl[i] = t * d
    }
    fl[TS_A_FFN] = t * ff
    fl[TS_A_F2] = t * d
    if (!ts_albert_attach(a) || !ts_scratch_ready(g_ts_albert_scratch, fl, 0l, 0l)) {
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
    var fl : TsFloats
    fl[TS_T_IDS] = t
    fl[TS_T_A] = t * cc
    fl[TS_T_B] = t * cc
    fl[TS_T_COL] = t * ts_text_kdim(te)
    fl[TS_T_GF] = t * 4l * hd
    fl[TS_T_GB] = t * 4l * hd
    fl[TS_T_Y] = t * 2l * hd
    if (!ts_text_attach(te) || !ts_scratch_ready(g_ts_text_scratch, fl, 0l, 0l)) {
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

// ----- the predictor's dispatches -----

let private TS_JOIN = 0.70710678118654752   //! the AdaIN residual blocks' join: (out + shortcut) / sqrt 2
let private TS_MAX_DE_LSTMS = 8              //! the duration encoder's LSTM stack the seat holds style rows for
let private TS_MAX_RES_BLOCKS = 8            //! ... and a prosody branch's residual blocks

//! layernorm rows in place against scale and shift rows at `woff` / `boff` of `wn` (the slab, or the aux rows fc(style) landed in)
def private ts_ln_rows(raw : VkCommandBuffer; var h : VkHaz; wn : TsRows; woff, boff : int64; x : TsRows; rows, d : int64; eps : float) {
    var s = set_tower_ln_cls(fixed_array(x.dev, x.dev, wn.dev), fixed_array(x.bytes, x.bytes, wn.bytes), fixed_array(x.bit, x.bit, wn.bit))
    var pa = TowerLnArgs(d = uint(d), rows = uint(rows), woff = uint(woff), boff = uint(boff), eps = eps, ln_on = 0u, lnoff = 0u, lnboff = 0u, ascale = 1.0)
    enc_tower_ln_cls(raw, h, s, pa, rows)
}

//! o [rows][width] = cat(x [rows][c], the res rows of `res`) zero-padded, as `pa` lays them (rs = 0 broadcasts one res row)
def private ts_concat(raw : VkCommandBuffer; var h : VkHaz; x, res, o : TsRows; pa : TtsConcatArgs) {
    var s = set_tts_concat_cls(fixed_array(x.dev, res.dev, res.dev, res.dev, o.dev), fixed_array(x.bytes, res.bytes, res.bytes, res.bytes, o.bytes),
        fixed_array(x.bit, res.bit, res.bit, res.bit, o.bit))
    var pc = pa
    enc_tts_concat_cls(raw, h, s, pc, tts_concat_wgs(int64(pa.rows) * int64(pa.width)))
}

//! raw[t] = the sigmoid sum of nb logits a row over speed
def private ts_sigsum(raw : VkCommandBuffer; var h : VkHaz; logits, out : TsRows; t, nb : int64; speed : float) {
    var s = set_tts_sigsum_cls(fixed_array(logits.dev, out.dev), fixed_array(logits.bytes, out.bytes), fixed_array(logits.bit, out.bit))
    var pa = TtsSigSumArgs(t = uint(t), nb = uint(nb), ls = uint(nb), speed = speed)
    enc_tts_sigsum_cls(raw, h, s, pa, tts_sigsum_wgs(t))
}

//! AdaIN then leaky(0.2) from x into y (y may be x): the column stats over t, then the fold with the norm slot's rows and the style rows at `hoff`
def private ts_adain(raw : VkCommandBuffer; var h : VkHaz; slab : TsRows; sc_ : TsScratch; x, y : TsRows; t, c : int64; ns : TtsNormSlot; hoff : int64) {
    var s1 = set_tts_colstats_cls(fixed_array(x.dev, sc_.stats.dev), fixed_array(x.bytes, sc_.stats.bytes), fixed_array(x.bit, sc_.stats.bit))
    var p1 = TtsStatsArgs(t = uint(t), c = uint(c))
    enc_tts_colstats_cls(raw, h, s1, p1, c)
    var s2 = set_tts_adain_leaky_cls(fixed_array(x.dev, y.dev, sc_.stats.dev, sc_.aux.dev, slab.dev), fixed_array(x.bytes, y.bytes, sc_.stats.bytes, sc_.aux.bytes, slab.bytes),
        fixed_array(x.bit, y.bit, sc_.stats.bit, sc_.aux.bit, 0u))
    var p2 = TtsAdainArgs(c = uint(c), t = uint(t), eps = ST2_ADAIN_EPS, xoff = 0u, yoff = 0u, hoff = uint(hoff), gwoff = uint(ns.gwoff), gboff = uint(ns.gboff), aoff = 0u)
    enc_tts_adain_leaky_cls(raw, h, s2, p2, tts_adain_wgs(t * c))
}

//! the depthwise transposed pool on rows: y [t_out][c] from x [t_in][c] over the slab's w [c][k] at `woff` and b [c] at `boff`
def private ts_pool_dw(raw : VkCommandBuffer; var h : VkHaz; slab : TsRows; cv : TtsConv1d; woff, boff : int64; x, y : TsRows; t_in, t_out : int64) {
    var s = set_tts_pool_dw_cls(fixed_array(x.dev, y.dev, slab.dev), fixed_array(x.bytes, y.bytes, slab.bytes), fixed_array(x.bit, y.bit, 0u))
    var pa = TtsPoolArgs(c = uint(cv.cin), k = uint(cv.k), stride = uint(cv.stride), pad_l = uint(cv.pad_l), dil = uint(cv.dilation), t_in = uint(t_in),
        t_out = uint(t_out), woff = uint(woff), boff = uint(boff))
    enc_tts_pool_dw_cls(raw, h, s, pa, tts_pool_dw_wgs(t_out * cv.cin))
}

//! out [rows][c] = src[idx[r]] - the nearest x2 upsample with idx = r / 2
def private ts_gather_rows(raw : VkCommandBuffer; var h : VkHaz; src, idx, out : TsRows; rows, c : int64) {
    var s = set_tts_row_gather_cls(fixed_array(src.dev, idx.dev, out.dev), fixed_array(src.bytes, idx.bytes, out.bytes), fixed_array(src.bit, idx.bit, out.bit))
    var pa = TtsGatherArgs(c = uint(c), rows = uint(rows), word_off = 0u, pos_off = 0u, type_off = 0u, out_off = 0u, adds = 0u)
    enc_tts_row_gather_cls(raw, h, s, pa, tts_row_gather_wgs(rows * c))
}

//! o = (a + b) * scale over n elements (o may be a)
def private ts_add_scale(raw : VkCommandBuffer; var h : VkHaz; a, b, o : TsRows; n : int64; scale : float) {
    var s = set_tts_add_scale_cls(fixed_array(a.dev, b.dev, o.dev), fixed_array(a.bytes, b.bytes, o.bytes), fixed_array(a.bit, b.bit, o.bit))
    var pa = TtsAddScaleArgs(nelem = uint(n), scale = scale, aoff = 0u, boff = 0u, ooff = 0u)
    enc_tts_add_scale_cls(raw, h, s, pa, tts_add_scale_wgs(n))
}

def private ts_pred_ensure : bool {
    return (ensure_f32_gemm_b_cls() && ensure_tower_ln_cls() && ensure_tts_row_gather_cls() && ensure_tts_im2col_cls() && ensure_tts_lstm_dir_cls()
        && ensure_tts_concat_cls() && ensure_tts_sigsum_cls() && ensure_tts_colstats_cls() && ensure_tts_adain_leaky_cls()
        && ensure_tts_pool_dw_cls() && ensure_tts_add_scale_cls())
}

// ----- the duration encoder seat -----

let private TS_DE_BERT = 0
let private TS_DE_DEN = 1
let private TS_DE_X = 2
let private TS_DE_Y = 3
let private TS_DE_GF = 4
let private TS_DE_GB = 5

def private ts_dur_enc_shape_ok(p : St2Predictor; t, sd : int64) : bool {
    let cc = p.bert_encoder.nout
    let width = cc + sd
    return false if (t < 1l || cc % 64l != 0l || p.bert_encoder.nin % 32l != 0l || width % 32l != 0l || empty(p.lstms)
        || length(p.lns) != length(p.lstms) || length(p.lstms) > TS_MAX_DE_LSTMS)
    for (l in p.lstms) {
        return false if (!tts_lstm_ok(l, width, TS_LSTM_MAX_HIDDEN) || 2l * l.fwd.hidden != cc)
    }
    for (ln in p.lns) {
        return false if (!tts_lin_ok(ln.fc) || ln.fc.nin != sd || ln.fc.nout != 2l * cc)
    }
    return tts_pred_ok(p, TS_LSTM_MAX_HIDDEN)
}

//! The duration encoder seat: the BERT projection, the style appended, then per LSTM the BiLSTM, the AdaLayerNorm and the style appended again; out [t][c + style]
[hot_path, unused_argument(sc), arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_TTS.md#vk-tts-chain")]
def private vulkan_styletts2_dur_enc(p : St2Predictor; bert : array<float>; t : int64; s : array<float>; @scratch @exact_size var out : array<float>; var sc : St2Scratch) : bool {
    if (!vulkan_tower_on()) {
        return ts_decline("dur_enc", VulkanTtsDecline.knob)
    }
    let cc = p.bert_encoder.nout
    let hidden = p.bert_encoder.nin
    let sd = long_length(s)
    let width = cc + sd
    if (!ts_dur_enc_shape_ok(p, t, sd) || long_length(bert) < t * hidden) {
        return ts_decline("dur_enc", VulkanTtsDecline.shape)
    }
    if (!vulkan_tower_arms() || !ts_pred_ensure()) {
        return ts_decline("dur_enc", VulkanTtsDecline.device)
    }
    let hd = cc / 2l
    let nl = length(p.lstms)
    var fl : TsFloats
    fl[TS_DE_BERT] = t * hidden
    fl[TS_DE_DEN] = t * cc
    fl[TS_DE_X] = t * width
    fl[TS_DE_Y] = t * cc
    fl[TS_DE_GF] = t * 4l * hd
    fl[TS_DE_GB] = t * 4l * hd
    if (!ts_pred_attach(p) || !ts_scratch_ready(g_ts_de_scratch, fl, 4l + sd + int64(nl) * (2l * cc + 4l), 0l)) {
        return ts_decline("dur_enc", VulkanTtsDecline.memory)
    }
    let sl & = g_ts_pred_slots
    let sc_ & = g_ts_de_scratch
    let slab = ts_slab_rows(g_ts_pred)
    let r_bert = ts_rows(sc_, TS_DE_BERT)
    let r_den = ts_rows(sc_, TS_DE_DEN)
    let r_x = ts_rows(sc_, TS_DE_X)
    let r_y = ts_rows(sc_, TS_DE_Y)
    let r_gf = ts_rows(sc_, TS_DE_GF)
    let r_gb = ts_rows(sc_, TS_DE_GB)
    let aux = sc_.aux
    ts_aux_reset()
    ts_aux_push(s, sd)
    var hoff : int64[TS_MAX_DE_LSTMS]
    for (i in range(nl)) {
        hoff[i] = ts_style_rows(p.lns[i].fc, s, cc, true)
    }
    if (!ts_aux_upload(sc_)) {
        return ts_decline("dur_enc", VulkanTtsDecline.memory)
    }
    upload_region_at(r_bert.dev, 0l, unsafe(addr<uint8 const?>(bert[0])), t * hidden * 4l)
    //! the style appended as a broadcast residual of stride 0; the width holds the f0/n columns out
    let pa_cat = TtsConcatArgs(c = uint(cc), rc = uint(sd), rs = 0u, f0s = 0u, ns = 0u, with_res = 1u, width = uint(width), rows = uint(t))
    vt_chain_one_shot("dur_enc", int64(nl), t) $(raw : VkCommandBuffer; var h : VkHaz) {
        ts_lin(raw, h, slab, sl.bert_enc, hidden, cc, r_bert, r_den, t)
        ts_concat(raw, h, r_den, aux, r_x, pa_cat)
        for (i in range(nl)) {
            ts_bilstm(raw, h, slab, sl.de_lstm[i], p.lstms[i], t, r_x, r_gf, r_gb, r_y)
            ts_ln_rows(raw, h, aux, hoff[i], hoff[i] + cc, r_y, t, cc, ST2_ADAIN_EPS)
            ts_concat(raw, h, r_y, aux, r_x, pa_cat)
        }
    }
    unsafe(scratch_resize(out, t * width))
    var dst = unsafe(addr(out[0]))
    readback_range(r_x.dev, r_x.bit, sc_.hb, dst, t * width * 4l)
    g_ts_encodes++
    return true
}

// ----- the durations seat -----

let private TS_DU_D = 0
let private TS_DU_Y = 1
let private TS_DU_GF = 2
let private TS_DU_GB = 3
let private TS_DU_LOG = 4
let private TS_DU_RAW = 5

//! The durations seat: the BiLSTM, the duration projection, the sigmoid sum a token over speed; raw [t]
[hot_path, unused_argument(sc), arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_TTS.md#vk-tts-chain")]
def private vulkan_styletts2_durations(p : St2Predictor; d : array<float>; t : int64; speed : float; @scratch @exact_size var raw : array<float>; var sc : St2Scratch) : bool {
    if (!vulkan_tower_on()) {
        return ts_decline("durations", VulkanTtsDecline.knob)
    }
    let width = p.lstm.fwd.nin
    let hd = p.lstm.fwd.hidden
    let nb = p.duration_proj.nout
    if (t < 1l || width % 32l != 0l || !tts_lstm_ok(p.lstm, width, TS_LSTM_MAX_HIDDEN) || p.duration_proj.nin != 2l * hd || nb < 1l || nb > 64l
        || speed <= 0.0 || long_length(d) < t * width || !tts_pred_ok(p, TS_LSTM_MAX_HIDDEN)) {
        return ts_decline("durations", VulkanTtsDecline.shape)
    }
    if (!vulkan_tower_arms() || !ts_pred_ensure()) {
        return ts_decline("durations", VulkanTtsDecline.device)
    }
    var fl : TsFloats
    fl[TS_DU_D] = t * width
    fl[TS_DU_Y] = t * 2l * hd
    fl[TS_DU_GF] = t * 4l * hd
    fl[TS_DU_GB] = t * 4l * hd
    fl[TS_DU_LOG] = t * nb
    fl[TS_DU_RAW] = t
    if (!ts_pred_attach(p) || !ts_scratch_ready(g_ts_du_scratch, fl, 0l, 0l)) {
        return ts_decline("durations", VulkanTtsDecline.memory)
    }
    let sl & = g_ts_pred_slots
    let sc_ & = g_ts_du_scratch
    let slab = ts_slab_rows(g_ts_pred)
    let r_d = ts_rows(sc_, TS_DU_D)
    let r_y = ts_rows(sc_, TS_DU_Y)
    let r_gf = ts_rows(sc_, TS_DU_GF)
    let r_gb = ts_rows(sc_, TS_DU_GB)
    let r_log = ts_rows(sc_, TS_DU_LOG)
    let r_raw = ts_rows(sc_, TS_DU_RAW)
    upload_region_at(r_d.dev, 0l, unsafe(addr<uint8 const?>(d[0])), t * width * 4l)
    vt_chain_one_shot("durations", 1l, t) $(raw_ : VkCommandBuffer; var h : VkHaz) {
        ts_bilstm(raw_, h, slab, sl.dur_lstm, p.lstm, t, r_d, r_gf, r_gb, r_y)
        ts_lin(raw_, h, slab, sl.dur_proj, 2l * hd, nb, r_y, r_log, t)
        ts_sigsum(raw_, h, r_log, r_raw, t, nb, speed)
    }
    unsafe(scratch_resize(raw, t))
    var dst = unsafe(addr(raw[0]))
    readback_range(r_raw.dev, r_raw.bit, sc_.hb, dst, t * 4l)
    g_ts_encodes++
    return true
}

// ----- the prosody seat -----

let private TS_PR_EN = 0     //! the encoder rows [frames][c]
let private TS_PR_Y = 1      //! the shared BiLSTM's rows [frames][2H]
let private TS_PR_GF = 2
let private TS_PR_GB = 3
let private TS_PR_A = 4      //! a branch's ping-pong rows
let private TS_PR_B = 5
let private TS_PR_R = 6      //! a block's normed rows
let private TS_PR_POOL = 7   //! ... its pooled rows
let private TS_PR_C1 = 8     //! ... its first conv's rows
let private TS_PR_SC = 9     //! ... its shortcut rows (the nearest copy)
let private TS_PR_SC2 = 10   //! ... and the learned shortcut's
let private TS_PR_COL = 11   //! the im2col columns
let private TS_PR_IDX = 12   //! the nearest upsample's row map, r / 2
let private TS_PR_F0 = 13    //! the F0 branch's projection [t][1]
let private TS_PR_N = 14     //! ... and the energy branch's

//! a prosody branch's geometry: the widest row count and width its blocks reach, the widest im2col row, the aux floats its norms take
struct private TsPrShape {
    t_max : int64
    c_max : int64
    kdim_max : int64
    aux : int64
}

def private ts_conv_kdim(c : TtsConv1d) : int64 => tts_round_up(c.k * c.cin, 32l)

//! a residual block's rows form on the seat, or -1: the block's output row count, the shape noted
def private ts_res_shape(var sh : TsPrShape; b : St2AdainResBlk; t : int64) : int64 {
    return -1l if (b.dim_in_s % 32l != 0l || b.dim_out % 64l != 0l || b.dim_in_s < b.dim_in || !tts_res_ok(b) || b.conv1.transposed || b.conv2.transposed
        || b.conv1.cin != b.dim_in_s || b.conv1.cout != b.dim_out || b.conv2.cin != b.dim_out || b.conv2.cout != b.dim_out
        || (b.learned_sc && (b.conv1x1.transposed || b.conv1x1.cout != b.dim_out || b.conv1x1.cin != b.dim_in_s))
        || !tts_lin_ok(b.norm1.fc) || b.norm1.fc.nout != 2l * b.dim_in_s || !tts_lin_ok(b.norm2.fc) || b.norm2.fc.nout != 2l * b.dim_out)
    var t1 = t
    if (b.upsample) {
        return -1l if (b.pool.groups != b.pool.cin || b.pool.cin != b.pool.cout || !b.pool.transposed || empty(b.pool.w) || b.pool.cin != b.dim_in_s)
        t1 = conv1d_out_len(b.pool, t)
        return -1l if (t1 != 2l * t)
    }
    return -1l if (conv1d_out_len(b.conv1, t1) != t1 || conv1d_out_len(b.conv2, t1) != t1 || (b.learned_sc && conv1d_out_len(b.conv1x1, t1) != t1))
    sh.t_max = max(sh.t_max, t1)
    sh.c_max = max(sh.c_max, max(b.dim_in_s, b.dim_out))
    sh.kdim_max = max(sh.kdim_max, max(ts_conv_kdim(b.conv1), ts_conv_kdim(b.conv2)))
    if (b.learned_sc) {
        sh.kdim_max = max(sh.kdim_max, ts_conv_kdim(b.conv1x1))
    }
    sh.aux += 2l * (b.dim_in_s + b.dim_out) + 8l
    return t1
}

//! a branch's blocks then its projection on the seat, or -1: the projection's row count
def private ts_branch_shape(var sh : TsPrShape; blocks : array<St2AdainResBlk>; proj : TtsConv1d; frames, cin : int64) : int64 {
    return -1l if (length(blocks) > TS_MAX_RES_BLOCKS || !tts_conv_ok(proj) || proj.transposed || proj.cout != 1l)
    var t = frames
    var c = cin
    for (b in blocks) {
        return -1l if (b.dim_in_s != c)
        t = ts_res_shape(sh, b, t)
        return -1l if (t < 0l)
        c = b.dim_out
    }
    return -1l if (proj.cin != c || conv1d_out_len(proj, t) != t)
    sh.kdim_max = max(sh.kdim_max, ts_conv_kdim(proj))
    sh.c_max = max(sh.c_max, c)
    return t
}

//! the CPU adain_res_blk, dispatch for dispatch, from bin [t][dim_in_s] into bout [t1][dim_out]; t1 back
def private ts_res_blk(raw : VkCommandBuffer; var h : VkHaz; slab : TsRows; sc_ : TsScratch; b : St2AdainResBlk; sl : TtsResSlots; hoff1, hoff2 : int64;
                       bin : TsRows; t : int64; bout : TsRows) : int64 {
    let r_r = ts_rows(sc_, TS_PR_R)
    let r_pool = ts_rows(sc_, TS_PR_POOL)
    let r_c1 = ts_rows(sc_, TS_PR_C1)
    let r_sc = ts_rows(sc_, TS_PR_SC)
    let r_sc2 = ts_rows(sc_, TS_PR_SC2)
    let r_col = ts_rows(sc_, TS_PR_COL)
    ts_adain(raw, h, slab, sc_, bin, r_r, t, b.dim_in_s, sl.n1, hoff1)
    var t1 = t
    if (b.upsample) {
        t1 = conv1d_out_len(b.pool, t)
        ts_pool_dw(raw, h, slab, b.pool, sl.pool_w, sl.pool_b, r_r, r_pool, t, t1)
        ts_conv(raw, h, slab, sl.c1, b.conv1, t1, t1, r_pool, r_col, r_c1)
    } else {
        ts_conv(raw, h, slab, sl.c1, b.conv1, t1, t1, r_r, r_col, r_c1)
    }
    ts_adain(raw, h, slab, sc_, r_c1, r_c1, t1, b.dim_out, sl.n2, hoff2)
    ts_conv(raw, h, slab, sl.c2, b.conv2, t1, t1, r_c1, r_col, bout)
    let n = t1 * b.dim_out
    if (b.upsample) {
        ts_gather_rows(raw, h, bin, ts_rows(sc_, TS_PR_IDX), r_sc, t1, b.dim_in_s)
        if (b.learned_sc) {
            ts_conv(raw, h, slab, sl.c1x1, b.conv1x1, t1, t1, r_sc, r_col, r_sc2)
            ts_add_scale(raw, h, bout, r_sc2, bout, n, TS_JOIN)
        } else {
            ts_add_scale(raw, h, bout, r_sc, bout, n, TS_JOIN)
        }
    } elif (b.learned_sc) {
        ts_conv(raw, h, slab, sl.c1x1, b.conv1x1, t1, t1, bin, r_col, r_sc2)
        ts_add_scale(raw, h, bout, r_sc2, bout, n, TS_JOIN)
    } else {
        ts_add_scale(raw, h, bout, bin, bout, n, TS_JOIN)
    }
    return t1
}

//! a prosody branch's blocks, pinging between the A and B rows, then its single-channel projection into `out` [t][1]; t back
def private ts_branch(raw : VkCommandBuffer; var h : VkHaz; slab : TsRows; sc_ : TsScratch; blocks : array<St2AdainResBlk>; slots : array<TtsResSlots>;
                      proj : TtsConv1d; pslot : TtsConvSlot; hoffs : int64[TS_MAX_RES_BLOCKS * 4]; hbase : int; x : TsRows; frames : int64; out : TsRows) : int64 {
    let r_a = ts_rows(sc_, TS_PR_A)
    let r_b = ts_rows(sc_, TS_PR_B)
    var t = frames
    let nb = length(blocks)
    for (i in range(nb)) {
        let src = i == 0 ? x : (i % 2 == 1 ? r_a : r_b)
        let dst = i % 2 == 0 ? r_a : r_b
        t = ts_res_blk(raw, h, slab, sc_, blocks[i], slots[i], hoffs[hbase + 2 * i], hoffs[hbase + 2 * i + 1], src, t, dst)
    }
    let fin = nb == 0 ? x : (nb % 2 == 1 ? r_a : r_b)
    ts_conv(raw, h, slab, pslot, proj, t, t, fin, ts_rows(sc_, TS_PR_COL), out)
    return t
}

//! the prosody seat's slots for this call's geometry
def private ts_pr_floats(sh : TsPrShape; cc, hd, frames : int64) : TsFloats {
    let cm = sh.c_max
    let tm = sh.t_max
    var fl : TsFloats
    fl[TS_PR_EN] = frames * cc
    fl[TS_PR_Y] = frames * 2l * hd
    fl[TS_PR_GF] = frames * 4l * hd
    fl[TS_PR_GB] = frames * 4l * hd
    for (i in range(TS_PR_A, TS_PR_SC2 + 1)) {
        fl[i] = tm * cm
    }
    fl[TS_PR_COL] = tm * sh.kdim_max
    fl[TS_PR_IDX] = tm
    fl[TS_PR_F0] = tm
    fl[TS_PR_N] = tm
    return fl
}

//! the style rows on the device: per branch (the energy branch's from `nbase`), per block, its two norms' fc(style); false where the aux rows ran short
def private ts_pr_style_rows(p : St2Predictor; s : array<float>; var hoffs : int64[TS_MAX_RES_BLOCKS * 4]; nbase : int; sc_ : TsScratch) : bool {
    ts_aux_reset()
    for (i in range(length(p.f0))) {
        hoffs[2 * i] = ts_style_rows(p.f0[i].norm1.fc, s, p.f0[i].dim_in_s, false)
        hoffs[2 * i + 1] = ts_style_rows(p.f0[i].norm2.fc, s, p.f0[i].dim_out, false)
    }
    for (i in range(length(p.n))) {
        hoffs[nbase + 2 * i] = ts_style_rows(p.n[i].norm1.fc, s, p.n[i].dim_in_s, false)
        hoffs[nbase + 2 * i + 1] = ts_style_rows(p.n[i].norm2.fc, s, p.n[i].dim_out, false)
    }
    return ts_aux_upload(sc_)
}

//! The prosody seat: the shared BiLSTM over the aligned rows, then each branch's residual blocks and projection; f0 and n [f2]
[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_TTS.md#vk-tts-chain")]
def private vulkan_styletts2_prosody(p : St2Predictor; en : array<float>; cc, frames : int64; s : array<float>;
                                     @scratch @exact_size var f0 : array<float>; @scratch @exact_size var n : array<float>; var f2 : int64&; var sc : St2Scratch) : bool {
    if (!vulkan_tower_on()) {
        return ts_decline("prosody", VulkanTtsDecline.knob)
    }
    let hd = p.shared_lstm.fwd.hidden
    let sd = long_length(s)
    var sh = TsPrShape(t_max = frames, c_max = max(cc, 2l * hd), kdim_max = 32l, aux = 4l)
    if (frames < 1l || cc % 32l != 0l || !tts_lstm_ok(p.shared_lstm, cc, TS_LSTM_MAX_HIDDEN) || empty(p.f0) || long_length(en) < cc * frames
        || !tts_pred_ok(p, TS_LSTM_MAX_HIDDEN)) {
        return ts_decline("prosody", VulkanTtsDecline.shape)
    }
    let tf = ts_branch_shape(sh, p.f0, p.f0_proj, frames, 2l * hd)
    let tn = ts_branch_shape(sh, p.n, p.n_proj, frames, 2l * hd)
    if (tf < 0l || tn < 0l || tf != tn) {
        return ts_decline("prosody", VulkanTtsDecline.shape)
    }
    for (b in p.f0) {
        return ts_decline("prosody", VulkanTtsDecline.shape) if (b.norm1.fc.nin != sd || b.norm2.fc.nin != sd)
    }
    for (b in p.n) {
        return ts_decline("prosody", VulkanTtsDecline.shape) if (b.norm1.fc.nin != sd || b.norm2.fc.nin != sd)
    }
    if (!vulkan_tower_arms() || !ts_pred_ensure()) {
        return ts_decline("prosody", VulkanTtsDecline.device)
    }
    let fl = ts_pr_floats(sh, cc, hd, frames)
    if (!ts_pred_attach(p) || !ts_scratch_ready(g_ts_pr_scratch, fl, sh.aux, 2l * sh.c_max)) {
        return ts_decline("prosody", VulkanTtsDecline.memory)
    }
    let sl & = g_ts_pred_slots
    let sc_ & = g_ts_pr_scratch
    let slab = ts_slab_rows(g_ts_pred)
    let r_en = ts_rows(sc_, TS_PR_EN)
    let r_y = ts_rows(sc_, TS_PR_Y)
    let r_gf = ts_rows(sc_, TS_PR_GF)
    let r_gb = ts_rows(sc_, TS_PR_GB)
    let r_idx = ts_rows(sc_, TS_PR_IDX)
    let r_f0 = ts_rows(sc_, TS_PR_F0)
    let r_n = ts_rows(sc_, TS_PR_N)
    var hoffs : int64[TS_MAX_RES_BLOCKS * 4]
    let nbase = TS_MAX_RES_BLOCKS * 2
    if (!ts_pr_style_rows(p, s, hoffs, nbase, sc_)) {
        return ts_decline("prosody", VulkanTtsDecline.memory)
    }
    g_ts_idx_host |> ensure_length(sh.t_max)
    for (i in range64(sh.t_max)) {
        g_ts_idx_host[i] = int(i / 2l)
    }
    upload_region_at(r_idx.dev, 0l, unsafe(addr<uint8 const?>(g_ts_idx_host[0])), sh.t_max * 4l)
    var en_tm & = unsafe(sc.pr_en_tm)
    transpose_ct(en, cc, frames, en_tm)
    upload_region_at(r_en.dev, 0l, unsafe(addr<uint8 const?>(en_tm[0])), frames * cc * 4l)
    vt_chain_one_shot("prosody", int64(length(p.f0) + length(p.n)), frames) $(raw : VkCommandBuffer; var h : VkHaz) {
        ts_bilstm(raw, h, slab, sl.sh_lstm, p.shared_lstm, frames, r_en, r_gf, r_gb, r_y)
        ts_branch(raw, h, slab, sc_, p.f0, sl.f0b, p.f0_proj, sl.f0_proj, hoffs, 0, r_y, frames, r_f0)
        ts_branch(raw, h, slab, sc_, p.n, sl.nb, p.n_proj, sl.n_proj, hoffs, nbase, r_y, frames, r_n)
    }
    unsafe(scratch_resize(f0, tf))
    unsafe(scratch_resize(n, tn))
    var dst_f0 = unsafe(addr(f0[0]))
    readback_range(r_f0.dev, r_f0.bit, sc_.hb, dst_f0, tf * 4l)
    var dst_n = unsafe(addr(n[0]))
    readback_range(r_n.dev, r_n.bit, sc_.hb, dst_n, tn * 4l)
    f2 = tf
    g_ts_encodes++
    return true
}

[init]
def dasllama_vulkan_tts_register {
    register_vk_drop_hook(@@ts_forget)
    static_if (!typeinfo builtin_module_exists(das_metal)) {
        register_styletts2_gpu(St2GpuDriver(albert = @@vulkan_styletts2_albert, text = @@vulkan_styletts2_text, dur_enc = @@vulkan_styletts2_dur_enc,
            durations = @@vulkan_styletts2_durations, prosody = @@vulkan_styletts2_prosody))
    }
}
