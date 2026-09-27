options gen2
options indenting = 4
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_vulkan_tts shared public

require dasllama/dasllama_lint public
require vulkan
require math
require dasllama/dasllama_math
require dasllama/dasllama_common
require dasllama/dasllama_tts_blocks
require dasllama/dasllama_tts_types
require dasllama/dasllama_styletts2
require dasllama/dasllama_pocket
require dasllama/dasllama_tts_slab
require dasllama/dasllama_convert
require dasllama/dasllama_vulkan_common
require dasllama/dasllama_vulkan_classes
require dasllama/dasllama_vulkan_tower

//! The Vulkan TTS driver: the kitten and kokoro families' synthesis seats and the Pocket family's
//! codec and frames seats on the tower's knob (`ARCHITECTURE_GPU_TOWER_VULKAN_TTS.md`); the Metal
//! tower driver fills the same seats on an Apple build.

var private g_ts_declines : VkDeclineCounter
var private g_ts_encodes = 0l   //! seats served, one per call

enum private VulkanTtsDecline {
    knob        //! DASLLAMA_VK_TOWER=0, or set_vulkan_tower(false)
    shape       //! a width off the lattice, a row count or head width the stamps do not serve
    device      //! the tier has no device, or a class failed to build
    memory      //! a device or pinned allocation failed, or a buffer would bind past the device's storage range
    gpu_error   //! a submit failed
}

let private TS_ATTN_MAX_T = int64(TTS_ATTN_TCAP)   //! the attention row stamp's key stage
let private TS_LSTM_MAX_HIDDEN = int64(TTS_LSTM_LANES)   //! the recurrence stamp's lanes

//! seats served and declines counted since the process started
def vulkan_tts_stats : tuple<encodes : int64; declines : int64> => (encodes = g_ts_encodes, declines = g_ts_declines.total)

//! the declines by reason name
def vulkan_tts_declines : table<string; int64> => clone_to_move(g_ts_declines.counts)

def private ts_decline_words(why : VulkanTtsDecline) : string {
    if (why == VulkanTtsDecline.knob) {
        return "the tower knob is off (DASLLAMA_VK_TOWER=0 or set_vulkan_tower(false))"
    } elif (why == VulkanTtsDecline.shape) {
        return "a width, head size, row count or hidden size the stamps do not serve"
    } elif (why == VulkanTtsDecline.memory) {
        return "a device allocation failed, or a buffer would bind past the device's storage range"
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

//! a part's slab on the device: its f32 rows (read-only, bit 0) and, for the decoder, `rows16`, the halfword twin of the whole slab
//! at the same element offsets - the f16 tile's feed; `rows16.dev` 0 = none
struct private TsSlab {
    key : uint64
    rows : TsRows
    rows16 : TsRows
}

var private g_ts_albert : TsSlab
var private g_ts_albert_slots : TtsAlbertSlab
var private g_ts_text : TsSlab
var private g_ts_text_slots : TtsTextSlab
var private g_ts_pred : TsSlab
var private g_ts_pred_slots : TtsPredSlab

def private ts_slab_release(var sl : TsSlab) {
    ts_rows_release(sl.rows)
    ts_rows_release(sl.rows16)
    sl.key = 0ul
}

//! `vk_range_ok`, the decline said under the info log naming `what`
[cold_path]
def private ts_range_ok(what : string; bytes : int64) : bool {
    return true if (vk_range_ok(bytes))
    to_log(LOG_INFO, "vk tts: the {what} of {bytes / 1048576l} MB binds past the device's storage range of {vk_max_storage_range() / 1048576l} MB - the seat declines\n")
    return false
}

//! the first `n` elements of `src` onto the rows at element 0
def private ts_upload(r : TsRows; src : array<auto(TT)>; n : int64) {
    upload_region(r.dev, unsafe(addr<uint8 const?>(src[0])), n * int64(typeinfo sizeof(type<TT>)))
}

//! the rows' first `n` floats back into `out`, sized to them, through the seat scratch's readback buffer
def private ts_readback(r : TsRows; sc_ : TsScratch; @scratch @exact_size var out : array<float>; n : int64) {
    unsafe(scratch_resize(out, n))
    return if (n <= 0l)
    var dst = unsafe(addr(out[0]))
    readback_range(r.dev, r.bit, sc_.hb, dst, n * 4l)
}

//! the host floats onto the device under `key`, and under `half` their f16 image beside them; false on an allocation failure or a slab past the storage range
[cold_path]
def private ts_slab_upload(var sl : TsSlab; key : uint64; host : array<float>; half : bool) : bool {
    ts_slab_release(sl)
    let n = long_length(host)
    return false if (n <= 0l || !ts_rows_alloc(sl.rows, n * 4l, 0u, "slab"))
    ts_upload(sl.rows, host, n)
    if (half) {
        var inscope slab16 <- cvt_f32_to_f16_sat_array(host)
        if (!ts_rows_alloc(sl.rows16, n * 2l, 0u, "halfword slab")) {
            ts_slab_release(sl)
            return false
        }
        ts_upload(sl.rows16, slab16, n)
    }
    sl.key = key
    return true
}

//! the slab for `key` stands: its rows are up under that key
def private ts_slab_current(sl : TsSlab; key : uint64) : bool => sl.rows.dev != 0ul && sl.key == key

//! the writer's two passes into a fresh slot record, the host floats up (their f16 twin under `half`), the slots taken, the wall said; false
//! on a failed upload, the resident slab released. A part's attach reaches it through its own [cold_path] rebuild, the one that holds the
//! writer's block, so the writer's allocations stay off the seat's hot path
[cold_path]
def private ts_slab_rebuild(var sl : TsSlab; var slots : auto(SS); key : uint64; half : bool; what : string;
                            write : block<(var w : TtsSlabWriter; var s : SS) : void>) : bool {
    var inscope fresh : SS
    let t0 = ref_time_ticks()
    var inscope host <- tts_slab_build(fresh, write)
    let built = get_time_usec(t0)
    return false if (!ts_slab_upload(sl, key, host, half))
    ts_attach_log(what, long_length(host), built, get_time_usec(t0) - built)
    delete slots
    slots <- fresh
    return true
}

def private ts_albert_attach(a : St2Albert) : bool {
    let key = tts_albert_key(a)
    return ts_slab_current(g_ts_albert, key) || ts_albert_rebuild(a, key)
}

[cold_path]
def private ts_albert_rebuild(a : St2Albert; key : uint64) : bool {
    return ts_slab_rebuild(g_ts_albert, g_ts_albert_slots, key, false, "albert slab") $(var w : TtsSlabWriter; var s : TtsAlbertSlab) {
        tts_albert_write(w, a, s)
    }
}

def private ts_text_attach(te : St2TextEncoder) : bool {
    let key = tts_text_key(te)
    return ts_slab_current(g_ts_text, key) || ts_text_rebuild(te, key)
}

[cold_path]
def private ts_text_rebuild(te : St2TextEncoder; key : uint64) : bool {
    return ts_slab_rebuild(g_ts_text, g_ts_text_slots, key, false, "text slab") $(var w : TtsSlabWriter; var s : TtsTextSlab) {
        tts_text_write(w, te, s)
    }
}

def private ts_pred_attach(p : St2Predictor) : bool {
    let key = tts_pred_key(p)
    return ts_slab_current(g_ts_pred, key) || ts_pred_rebuild(p, key)
}

[cold_path]
def private ts_pred_rebuild(p : St2Predictor; key : uint64) : bool {
    return ts_slab_rebuild(g_ts_pred, g_ts_pred_slots, key, false, "predictor slab") $(var w : TtsSlabWriter; var s : TtsPredSlab) {
        tts_pred_write(w, p, s)
    }
}

let private TS_SCRATCH_BUFS = 30
let private TS_AUX_BIT = 1u << 30u       //! the hazard bit of a scratch's aux rows
let private TS_STATS_BIT = 1u << 31u     //! ... and of its column stats
let private TS_SCRATCH_GRAIN = 65536l    //! a slot grows in 256 KB steps, so a call a little wider reuses it

//! a seat's activation rows: one device buffer a slot sized in floats (0 = no buffer), the host readback buffer sized to
//! what the seat reads back (the widest slot unless the seat says less), and two more row sets - `aux`, the per-call
//! uploads (a style vector, fc(style) per norm), and `stats`, the AdaIN column stats the device writes
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

//! the floats the readback buffer holds: `hb_floats` where the seat names its readback, else the widest slot
def private ts_hb_floats(floats : TsFloats; hb_floats : int64) : int64 {
    return hb_floats if (hb_floats >= 0l)
    var widest = 0l
    for (i in range(TS_SCRATCH_BUFS)) {
        widest = max(widest, floats[i])
    }
    return widest
}

def private ts_scratch_fits(s : TsScratch; floats : TsFloats; aux_floats, stats_floats, hb_floats : int64) : bool {
    return false if (s.aux.bytes < aux_floats * 4l || s.stats.bytes < stats_floats * 4l)
    for (i in range(TS_SCRATCH_BUFS)) {
        return false if (s.bytes[i] < floats[i] * 4l)
    }
    return s.hb_bytes >= ts_hb_floats(floats, hb_floats) * 4l
}

//! `bytes` on the device under `bit`, none at zero; false when the allocation failed or the rows would bind past the storage range (said naming `what`)
[cold_path]
def private ts_rows_alloc(var r : TsRows; bytes : int64; bit : uint; what : string = "rows") : bool {
    r = TsRows(bit = bit)
    return true if (bytes <= 0l)
    return false if (!ts_range_ok(what, bytes))
    var oom = false
    r.bytes = bytes
    r.dev = soft_device_buf(oom, r.bytes)
    return !oom && r.dev != 0ul
}

let private TS_SCRATCH_HEADROOM = 4l   //! a slot is sized this fraction past the call's need: a longer sentence reuses it

//! `floats` grown by the headroom to the grain; the exact round-up where the grown size would bind past the storage range
def private ts_grown(floats : int64) : int64 {
    let grown = round_up(floats + floats / TS_SCRATCH_HEADROOM, TS_SCRATCH_GRAIN)
    return vk_range_ok(grown * 4l) ? grown : round_up(floats, TS_SCRATCH_GRAIN)
}

//! false when an allocation fails or a slot would bind past the storage range; every slot's size is settled before the old scratch goes, and a slot never shrinks
[cold_path]
def private ts_scratch_build(var s : TsScratch; floats : TsFloats; aux_floats, stats_floats, hb_floats : int64) : bool {
    var grown : TsFloats
    for (i in range(TS_SCRATCH_BUFS)) {
        grown[i] = max(floats[i] > 0l ? ts_grown(floats[i]) : 0l, s.bytes[i] / 4l)   //! a slot this call leaves idle keeps its size: the seats sharing the scratch alternate
        return false if (grown[i] > 0l && !ts_range_ok("scratch slot", grown[i] * 4l))
    }
    ts_scratch_release(s)
    let t0 = ref_time_ticks()
    var oom = false
    var total = 0l
    for (i in range(TS_SCRATCH_BUFS)) {
        if (grown[i] > 0l) {
            s.bytes[i] = grown[i] * 4l
            s.devs[i] = soft_device_buf(oom, s.bytes[i])
            total += grown[i]
        }
    }
    let dev_us = get_time_usec(t0)
    s.hb_bytes = ts_grown(max(ts_hb_floats(floats, hb_floats), 1l)) * 4l
    s.hb = make_host_buf(s.hb_bytes, false, [cached = true, soft = true])
    if (oom || s.hb.buf == 0ul || !ts_rows_alloc(s.aux, ts_grown(aux_floats) * 4l, TS_AUX_BIT) || !ts_rows_alloc(s.stats, ts_grown(stats_floats) * 4l, TS_STATS_BIT)) {
        ts_scratch_release(s)
        return false
    }
    if (vk_prof()) {
        to_log(LOG_INFO, "vk tts scratch: {total * 4l / 1048576l} MB over the slots in {dev_us / 1000} ms, the {s.hb_bytes / 1048576l} MB readback buffer in {(get_time_usec(t0) - dev_us) / 1000} ms\n")
    }
    return true
}

def private ts_scratch_ready(var s : TsScratch; floats : TsFloats; aux_floats, stats_floats : int64; hb_floats : int64 = -1l) : bool
    => ts_scratch_fits(s, floats, aux_floats, stats_floats, hb_floats) || ts_scratch_build(s, floats, aux_floats, stats_floats, hb_floats)

//! the scratch's slot as a dispatch binds it; the hazard bit is the slot's
def private ts_rows(s : TsScratch; slot : int) : TsRows => TsRows(dev = s.devs[slot], bytes = s.bytes[slot], bit = 1u << uint(slot))

//! the model-drop hook and the weights-epoch listener: every slab, scratch and voice slot is released here, ahead of the sweep, and rebuilt on the next call
[cold_path]
def private ts_forget {
    ts_slab_release(g_ts_albert)
    ts_slab_release(g_ts_text)
    ts_slab_release(g_ts_pred)
    ts_slab_release(g_ts_dec)
    ts_slab_release(g_ts_pk_codec)
    ts_slab_release(g_ts_pk_frames)
    ts_scratch_release(g_ts_albert_scratch)
    ts_scratch_release(g_ts_text_scratch)
    ts_scratch_release(g_ts_de_scratch)
    ts_scratch_release(g_ts_du_scratch)
    ts_scratch_release(g_ts_pr_scratch)
    ts_scratch_release(g_ts_dec_scratch)
    ts_scratch_release(g_ts_pkc_scratch)
    ts_scratch_release(g_ts_pkf_scratch)
    ts_pk_voice_drop()
}

var private @scratch g_ts_aux_host : array<float>   //! the staged rows, `g_ts_aux_n` of them live; grow-only
var private g_ts_aux_n = 0l
var private @scratch g_ts_h_tmp : array<float>
var private @scratch g_ts_idx_host : array<int>

def private ts_aux_reset {
    g_ts_aux_n = 0l
}

//! `n` staged floats reserved at the next 16-byte boundary of the rows; the element offset
def private ts_aux_alloc(n : int64) : int64 {
    let at = round_up(g_ts_aux_n, 4l)
    g_ts_aux_host |> ensure_length(at + n)
    g_ts_aux_n = at + n
    return at
}

//! `n` floats of `src` staged; the element offset
def private ts_aux_push(src : array<float>; n : int64) : int64 {
    let at = ts_aux_alloc(n)
    for (i in range64(n)) {
        g_ts_aux_host[at + i] = src[i]
    }
    return at
}

//! a norm's fc(style) rows staged (`tts_style_rows`); the element offset
def private ts_style_rows(fc : TtsLinear; s : array<float>; c : int64; ln_form : bool) : int64 {
    let at = ts_aux_alloc(2l * c)
    tts_style_rows(fc, s, c, ln_form, g_ts_h_tmp, unsafe(addr(g_ts_aux_host[at])))
    return at
}

//! the staged rows onto the scratch's aux buffer; false where the seat sized it short
def private ts_aux_upload(s : TsScratch) : bool {
    let n = g_ts_aux_n
    return true if (n == 0l)
    return false if (n * 4l > s.aux.bytes)
    ts_upload(s.aux, g_ts_aux_host, n)
    return true
}

def private ts_gemm_args(slot : TtsConvSlot; nin, nout, rows : int64) : F32GemmArgs
    => F32GemmArgs(n = uint(nin), d = uint(nout), rows = uint(rows), woff = uint(slot.woff), wstride = uint(slot.kdim_p), boff = uint(slot.boff),
        xoff = 0u, xstride = uint(nin), yoff = 0u, ystride = uint(nout))

//! a dense linear's GEMM: y[rows x nout] = x[rows x nin] W^T + b over the slab's rows, the f32 tile; `biased` false leaves the bias row for the
//! residual seam that follows (`ts_add_ln` adds it there)
def private ts_lin(raw : VkCommandBuffer; var h : VkHaz; slab : TsRows; slot : TtsConvSlot; nin, nout : int64; x, y : TsRows; rows : int64; biased : bool = true) {
    var pa = ts_gemm_args(slot, nin, nout, rows)
    if (biased) {
        var s = set_f32_gemm_b_cls(fixed_array(slab.dev, x.dev, y.dev, slab.dev), fixed_array(slab.bytes, x.bytes, y.bytes, slab.bytes), fixed_array(0u, x.bit, y.bit, 0u))
        enc_f32_gemm_b_cls(raw, h, s, pa, f32_gemm_wgs(rows, nout))
    } else {
        var s = set_f32_gemm_cls(fixed_array(slab.dev, x.dev, y.dev), fixed_array(slab.bytes, x.bytes, y.bytes), fixed_array(0u, x.bit, y.bit))
        enc_f32_gemm_cls(raw, h, s, pa, f32_gemm_wgs(rows, nout))
    }
    vt_pt(raw, VtProfRole.tts_gemm)
}

//! layernorm rows from x into y (y may be x) against a norm slot's scale and shift rows
def private ts_ln(raw : VkCommandBuffer; var h : VkHaz; slab : TsRows; ns : TtsNormSlot; x, y : TsRows; rows, d : int64; eps : float) {
    ts_ln_at(raw, h, slab, ns.gwoff, ns.gboff, x, y, rows, d, eps)
}

//! layernorm rows from x into y (y may be x) against scale and shift rows at `woff` / `boff` of `wn` (the slab, or the aux rows fc(style) landed in)
def private ts_ln_at(raw : VkCommandBuffer; var h : VkHaz; wn : TsRows; woff, boff : int64; x, y : TsRows; rows, d : int64; eps : float) {
    var s = set_tower_ln_cls(fixed_array(x.dev, y.dev, wn.dev), fixed_array(x.bytes, y.bytes, wn.bytes), fixed_array(x.bit, y.bit, wn.bit))
    var pa = TowerLnArgs(d = uint(d), rows = uint(rows), woff = uint(woff), boff = uint(boff), eps = eps, ln_on = 0u, lnoff = 0u, lnboff = 0u, ascale = 1.0)
    enc_tower_ln_cls(raw, h, s, pa, rows)
    vt_pt(raw, VtProfRole.tts_ln)
}

//! the post-LN residual seam: x += y + b (the branch linear's bias row, `bslot`), then xb = layernorm(x) on the norm slot's rows
def private ts_add_ln(raw : VkCommandBuffer; var h : VkHaz; slab : TsRows; bslot : TtsConvSlot; ns : TtsNormSlot; x, y, xb : TsRows; rows, d : int64; eps : float) {
    var s = set_tower_post_add_ln_cls(fixed_array(x.dev, y.dev, xb.dev, slab.dev), fixed_array(x.bytes, y.bytes, xb.bytes, slab.bytes), fixed_array(x.bit, y.bit, xb.bit, 0u))
    var pa = TowerLnArgs(d = uint(d), rows = uint(rows), woff = 0u, boff = uint(bslot.boff), eps = eps, ln_on = 1u, lnoff = uint(ns.gwoff), lnboff = uint(ns.gboff), ascale = 1.0)
    enc_tower_post_add_ln_cls(raw, h, s, pa, rows)
    vt_pt(raw, VtProfRole.tts_ln)
}

//! the leaky ReLU over x's first `nelem` elements in place
def private ts_leaky(raw : VkCommandBuffer; var h : VkHaz; x : TsRows; nelem : int64; slope : float) {
    var s = set_tts_leaky_cls(fixed_array(x.dev), fixed_array(x.bytes), fixed_array(x.bit))
    var pa = TtsElemArgs(nelem = uint(nelem), off = 0u, slope = slope)
    enc_tts_leaky_cls(raw, h, s, pa, nelem)
    vt_pt(raw, VtProfRole.tts_act)
}

//! a conv on rows, forward or transposed: x [tin][cin] gathered into col [tout][kdim_p], then the biased tile over the slot's rows into y [tout][cout]
def private ts_conv(raw : VkCommandBuffer; var h : VkHaz; slab : TsRows; slot : TtsConvSlot; cv : TtsConv1d; tin, tout : int64; x, col, y : TsRows) {
    ts_conv_padded(raw, h, slab, slot, cv, tin, tout, cv.pad_l, x, col, y)
}

//! x [tin][cin] gathered into col [tout][kdim_p] under the left pad `pad_l`, the columns as halves under `half`
def private ts_im2col(raw : VkCommandBuffer; var h : VkHaz; slot : TtsConvSlot; cv : TtsConv1d; tin, tout, pad_l : int64; x, col : TsRows; half : bool) {
    var pc = TtsIm2colArgs(cin = uint(cv.cin), tin = uint(tin), tout = uint(tout), k = uint(cv.k), stride = uint(cv.stride), pad_l = uint(pad_l),
        dilation = uint(cv.dilation), transposed = cv.transposed ? 1u : 0u, kdim = uint(slot.kdim_p), xoff = 0u, coloff = 0u, rows = 1u)
    if (half) {
        var sc = set_tts_im2col16_cls(fixed_array(x.dev, col.dev), fixed_array(x.bytes, col.bytes), fixed_array(x.bit, col.bit))
        enc_tts_im2col16_cls(raw, h, sc, pc, tout * slot.kdim_p)
    } else {
        var sc = set_tts_im2col_cls(fixed_array(x.dev, col.dev), fixed_array(x.bytes, col.bytes), fixed_array(x.bit, col.bit))
        enc_tts_im2col_cls(raw, h, sc, pc, tout * slot.kdim_p)
    }
    vt_pt(raw, VtProfRole.tts_im2col)
}

//! the same under a left pad of the caller's choosing (the codec convs run the CPU's first-window pad, not the conv's own)
def private ts_conv_padded(raw : VkCommandBuffer; var h : VkHaz; slab : TsRows; slot : TtsConvSlot; cv : TtsConv1d; tin, tout, pad_l : int64; x, col, y : TsRows) {
    ts_im2col(raw, h, slot, cv, tin, tout, pad_l, x, col, false)
    var sm = set_f32_gemm_b_cls(fixed_array(slab.dev, col.dev, y.dev, slab.dev), fixed_array(slab.bytes, col.bytes, y.bytes, slab.bytes), fixed_array(0u, col.bit, y.bit, 0u))
    var pm = F32GemmArgs(n = uint(cv.k * cv.cin), d = uint(cv.cout), rows = uint(tout), woff = uint(slot.woff), wstride = uint(slot.kdim_p), boff = uint(slot.boff),
        xoff = 0u, xstride = uint(slot.kdim_p), yoff = 0u, ystride = uint(cv.cout))
    enc_f32_gemm_b_cls(raw, h, sm, pm, f32_gemm_wgs(tout, cv.cout))
    vt_pt(raw, VtProfRole.tts_gemm)
}

//! the recurrence stamp's workgroup is four invocations a lane; a device whose compute limit is under that declines the LSTM seats
def private ts_lstm_wg_ok : bool => g_gpu.dry || g_gpu.max_wg_invocations >= 4l * int64(TTS_LSTM_LANES)

//! a conv on rows through the f16 tile over the decoder slab's halfword twin: col16 [tout][kdim_p] as halves, y [tout][cout], the bias after
def private ts_conv16(raw : VkCommandBuffer; var h : VkHaz; slab, slab16 : TsRows; slot : TtsConvSlot; cv : TtsConv1d; tin, tout : int64; x, col16, y : TsRows) {
    ts_im2col(raw, h, slot, cv, tin, tout, cv.pad_l, x, col16, true)
    let khr = f16_gemm_khr()
    var sm = f16_gemm_set(khr, fixed_array(slab16.dev, col16.dev, y.dev), fixed_array(slab16.bytes, col16.bytes, y.bytes), fixed_array(0u, col16.bit, y.bit))
    var pm = F16GemmArgs(n = uint(slot.kdim_p), d = uint(cv.cout), rows = uint(tout), woff = uint(slot.woff), ybase = 0u, ystride = uint(cv.cout), ksplit = 0u)
    f16_gemm_enc(khr, raw, h, sm, pm, tout, cv.cout)
    vt_pt(raw, VtProfRole.tts_gemm)
    var sb = set_tower_bias_act_cls(fixed_array(y.dev, slab.dev), fixed_array(y.bytes, slab.bytes), fixed_array(y.bit, 0u))
    vt_bias_act(raw, h, sb, cv.cout, tout * cv.cout, slot.boff, BIAS_ACT_NONE)
    vt_pt(raw, VtProfRole.tts_rows)
}

//! a conv on rows: under `half`, a width on the bias pass's four lattice rides the f16 tile; every other conv the f32 tile
def private ts_conv_at(raw : VkCommandBuffer; var h : VkHaz; slab : TsRows; slot : TtsConvSlot; cv : TtsConv1d; tin, tout : int64; x, col, col16, y : TsRows;
                       half : bool) {
    if (half && f16_gemm_on() && g_ts_dec.rows16.dev != 0ul && col16.dev != 0ul && cv.cout % 4l == 0l && cv.cout >= 16l) {
        ts_conv16(raw, h, slab, g_ts_dec.rows16, slot, cv, tin, tout, x, col16, y)
    } else {
        ts_conv(raw, h, slab, slot, cv, tin, tout, x, col, y)
    }
}

//! one LSTM direction: the input gates on the biased tile into g [t][4H], then the recurrence walk into its half of y [t][2H]
def private ts_lstm_dir(raw : VkCommandBuffer; var h : VkHaz; slab : TsRows; xs : TtsConvSlot; hh : int64; l : TtsLstm; t : int64; x, g, y : TsRows; backward : bool) {
    let hd = l.hidden
    ts_lin(raw, h, slab, xs, l.nin, 4l * hd, x, g, t)
    var s = set_tts_lstm_dir_cls(fixed_array(g.dev, slab.dev, y.dev), fixed_array(g.bytes, slab.bytes, y.bytes), fixed_array(g.bit, 0u, y.bit))
    var pa = TtsLstmArgs(t = uint(t), hidden = uint(hd), goff = 0u, gstride = uint(4l * hd), hhoff = uint(hh), hoff = backward ? uint(hd) : 0u,
        hstride = uint(2l * hd), backward = backward ? 1u : 0u)
    enc_tts_lstm_dir_cls(raw, h, s, pa, 1l)
    vt_pt(raw, VtProfRole.tts_lstm)
}

//! the BiLSTM over x [t][nin]: the forward walk into y's first half, the backward into its second (the CPU `bilstm`'s rows)
def private ts_bilstm(raw : VkCommandBuffer; var h : VkHaz; slab : TsRows; ls : TtsLstmSlot; l : TtsBiLstm; t : int64; x, gf, gb, y : TsRows) {
    ts_lstm_dir(raw, h, slab, ls.fx, ls.fhh, l.fwd, t, x, gf, y, false)
    ts_lstm_dir(raw, h, slab, ls.bx, ls.bhh, l.bwd, t, x, gb, y, true)
}

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
    if (!ts_albert_attach(a) || !ts_scratch_ready(g_ts_albert_scratch, fl, 0l, 0l, t * d)) {
        return ts_decline("albert", VulkanTtsDecline.memory)
    }
    let sl & = g_ts_albert_slots
    let sc_ & = g_ts_albert_scratch
    let slab = g_ts_albert.rows
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
    ts_upload(r_ids, ids, t)
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
    //! x the residual stream, then the full-normed rows; xb2 the attention context, then the attention-normed rows the ffn reads
    vt_chain_one_shot("albert", a.n_layers, t) $(raw : VkCommandBuffer; var h : VkHaz) {
        enc_tts_row_gather_cls(raw, h, s_gather, pa_gather, t * a.emb_dim)
        vt_pt(raw, VtProfRole.tts_rows)
        ts_ln(raw, h, slab, sl.emb_ln, r_e, r_e, t, a.emb_dim, eps)
        ts_lin(raw, h, slab, sl.map_in, a.emb_dim, d, r_e, r_x, t)
        for (_l in range64(a.n_layers)) {
            ts_lin(raw, h, slab, sl.q, d, d, r_x, r_q, t)
            ts_lin(raw, h, slab, sl.k, d, d, r_x, r_k, t)
            ts_lin(raw, h, slab, sl.v, d, d, r_x, r_v, t)
            enc_tts_attn_cls(raw, h, s_attn, pa_attn, tts_attn_wgs(t, a.heads))
            vt_pt(raw, VtProfRole.tts_attn)
            ts_lin(raw, h, slab, sl.dense, d, d, r_xb2, r_xb, t, false)
            ts_add_ln(raw, h, slab, sl.dense, sl.attn_ln, r_x, r_xb, r_xb2, t, d, eps)
            ts_lin(raw, h, slab, sl.ffn, d, ff, r_xb2, r_ffn, t)
            enc_tts_gelu_tanh_cls(raw, h, s_gelu, pa_gelu, t * ff)
            vt_pt(raw, VtProfRole.tts_act)
            ts_lin(raw, h, slab, sl.ffn_out, ff, d, r_ffn, r_f2, t, false)
            ts_add_ln(raw, h, slab, sl.ffn_out, sl.full_ln, r_xb2, r_f2, r_x, t, d, eps)
        }
    }
    ts_readback(r_x, sc_, out, t * d)
    g_ts_encodes++
    return true
}

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
    return (ts_lstm_wg_ok() && ensure_f32_gemm_b_cls() && ensure_tower_ln_cls() && ensure_tts_row_gather_cls() && ensure_tts_leaky_cls()
        && ensure_tts_im2col_cls() && ensure_tts_lstm_dir_cls())
}

//! the widest im2col row the encoder's convs take
def private ts_text_kdim(te : St2TextEncoder) : int64 {
    var kdim = 32l
    for (cv in te.cnn) {
        kdim = max(kdim, round_up(cv.k * cv.cin, 32l))
    }
    return kdim
}

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
    if (!ts_text_attach(te) || !ts_scratch_ready(g_ts_text_scratch, fl, 0l, 0l, t * cc)) {
        return ts_decline("text", VulkanTtsDecline.memory)
    }
    let sl & = g_ts_text_slots
    let sc_ & = g_ts_text_scratch
    let slab = g_ts_text.rows
    let r_ids = ts_rows(sc_, TS_T_IDS)
    let r_a = ts_rows(sc_, TS_T_A)
    let r_b = ts_rows(sc_, TS_T_B)
    let r_col = ts_rows(sc_, TS_T_COL)
    let r_gf = ts_rows(sc_, TS_T_GF)
    let r_gb = ts_rows(sc_, TS_T_GB)
    let r_y = ts_rows(sc_, TS_T_Y)
    ts_upload(r_ids, ids, t)
    var s_gather = set_tts_row_gather_cls(fixed_array(slab.dev, r_ids.dev, r_a.dev), fixed_array(slab.bytes, r_ids.bytes, r_a.bytes), fixed_array(0u, r_ids.bit, r_a.bit))
    var pa_gather = TtsGatherArgs(c = uint(cc), rows = uint(t), word_off = uint(sl.emb), pos_off = 0u, type_off = 0u, out_off = 0u, adds = 0u)
    let ncnn = length(te.cnn)
    vt_chain_one_shot("text", int64(ncnn), t) $(raw : VkCommandBuffer; var h : VkHaz) {
        enc_tts_row_gather_cls(raw, h, s_gather, pa_gather, t * cc)
        vt_pt(raw, VtProfRole.tts_rows)
        for (i in range(ncnn)) {
            let src = i % 2 == 0 ? r_a : r_b
            let dst = i % 2 == 0 ? r_b : r_a
            ts_conv(raw, h, slab, sl.cnn[i], te.cnn[i], t, t, src, r_col, dst)
            ts_ln(raw, h, slab, sl.ln[i], dst, dst, t, cc, ST2_ADAIN_EPS)
            ts_leaky(raw, h, dst, t * cc, 0.2)
        }
        let fin = ncnn % 2 == 1 ? r_b : r_a
        ts_bilstm(raw, h, slab, sl.lstm, te.lstm, t, fin, r_gf, r_gb, r_y)
    }
    var rows & = unsafe(sc.te_a)
    ts_readback(r_y, sc_, rows, t * cc)
    unsafe(scratch_resize(out, cc * t))
    transpose_tc(rows, t, cc, out)
    g_ts_encodes++
    return true
}

let private TS_JOIN = 0.70710678118654752   //! the AdaIN residual blocks' join: (out + shortcut) / sqrt 2
let private TS_MAX_DE_LSTMS = 8              //! the duration encoder's LSTM stack the seat holds style rows for
let private TS_MAX_RES_BLOCKS = 8            //! ... and a prosody branch's residual blocks

//! o [rows][width] = cat(x [rows][c], the res rows, the f0 and n columns) zero-padded, as `pa` lays them (rs = 0 broadcasts one res row)
def private ts_concat(raw : VkCommandBuffer; var h : VkHaz; x, res, f0, nd, o : TsRows; pa : TtsConcatArgs) {
    var s = set_tts_concat_cls(fixed_array(x.dev, res.dev, f0.dev, nd.dev, o.dev), fixed_array(x.bytes, res.bytes, f0.bytes, nd.bytes, o.bytes),
        fixed_array(x.bit, res.bit, f0.bit, nd.bit, o.bit))
    var pc = pa
    enc_tts_concat_cls(raw, h, s, pc, int64(pa.rows) * int64(pa.width))
    vt_pt(raw, VtProfRole.tts_rows)
}

//! raw[t] = the sigmoid sum of nb logits a row over speed
def private ts_sigsum(raw : VkCommandBuffer; var h : VkHaz; logits, out : TsRows; t, nb : int64; speed : float) {
    var s = set_tts_sigsum_cls(fixed_array(logits.dev, out.dev), fixed_array(logits.bytes, out.bytes), fixed_array(logits.bit, out.bit))
    var pa = TtsSigSumArgs(t = uint(t), nb = uint(nb), ls = uint(nb), speed = speed)
    enc_tts_sigsum_cls(raw, h, s, pa, t)
    vt_pt(raw, VtProfRole.tts_rows)
}

//! the column statistics of x [t][c] into the scratch's stats rows, the block partials past the two stats rows
def private ts_colstats(raw : VkCommandBuffer; var h : VkHaz; sc_ : TsScratch; x : TsRows; t, c : int64) {
    let st = sc_.stats
    var pa = TtsStatsArgs(t = uint(t), c = uint(c), nblk = uint(tts_stats_blocks(t)), poff = uint(2l * c))
    var s1 = set_tts_colpart_sum_cls(fixed_array(x.dev, st.dev), fixed_array(x.bytes, st.bytes), fixed_array(x.bit, st.bit))
    enc_tts_colpart_sum_cls(raw, h, s1, pa, tts_stats_blocks(t))
    var s2 = set_tts_colreduce_mean_cls(fixed_array(st.dev, st.dev), fixed_array(st.bytes, st.bytes), fixed_array(st.bit, st.bit))
    enc_tts_colreduce_mean_cls(raw, h, s2, pa, c)
    var s3 = set_tts_colpart_sq_cls(fixed_array(x.dev, st.dev, st.dev), fixed_array(x.bytes, st.bytes, st.bytes), fixed_array(x.bit, st.bit, st.bit))
    enc_tts_colpart_sq_cls(raw, h, s3, pa, tts_stats_blocks(t))
    var s4 = set_tts_colreduce_sq_cls(fixed_array(st.dev, st.dev), fixed_array(st.bytes, st.bytes), fixed_array(st.bit, st.bit))
    enc_tts_colreduce_sq_cls(raw, h, s4, pa, c)
    vt_pt(raw, VtProfRole.tts_adain)
}

//! AdaIN, then leaky(0.2) or Snake on the norm slot's alpha row, from x into y (y may be x), the style rows at `hoff`
def private ts_adain(raw : VkCommandBuffer; var h : VkHaz; slab : TsRows; sc_ : TsScratch; x, y : TsRows; t, c : int64; ns : TtsNormSlot; hoff : int64; snake : bool) {
    ts_colstats(raw, h, sc_, x, t, c)
    let bufs = fixed_array(x.dev, y.dev, sc_.stats.dev, sc_.aux.dev, slab.dev)
    let sizes = fixed_array(x.bytes, y.bytes, sc_.stats.bytes, sc_.aux.bytes, slab.bytes)
    let bits = fixed_array(x.bit, y.bit, sc_.stats.bit, sc_.aux.bit, 0u)
    static_assert(TTS_ADAIN_EPS == ST2_ADAIN_EPS, "the AdaIN stamps' eps is the model's")
    var p2 = TtsAdainArgs(c = uint(c), t = uint(t), xoff = 0u, yoff = 0u, hoff = uint(hoff), gwoff = uint(ns.gwoff), gboff = uint(ns.gboff),
        aoff = uint(ns.aux_off))
    if (snake) {
        var s2 = set_tts_adain_snake_cls(bufs, sizes, bits)
        enc_tts_adain_snake_cls(raw, h, s2, p2, t * c)
    } else {
        var s2 = set_tts_adain_leaky_cls(bufs, sizes, bits)
        enc_tts_adain_leaky_cls(raw, h, s2, p2, t * c)
    }
    vt_pt(raw, VtProfRole.tts_adain)
}

//! the depthwise transposed pool on rows: y [t_out][c] from x [t_in][c] over the slab's w [c][k] at `woff` and b [c] at `boff`
def private ts_pool_dw(raw : VkCommandBuffer; var h : VkHaz; slab : TsRows; cv : TtsConv1d; woff, boff : int64; x, y : TsRows; t_in, t_out : int64) {
    var s = set_tts_pool_dw_cls(fixed_array(x.dev, y.dev, slab.dev), fixed_array(x.bytes, y.bytes, slab.bytes), fixed_array(x.bit, y.bit, 0u))
    var pa = TtsPoolArgs(c = uint(cv.cin), k = uint(cv.k), stride = uint(cv.stride), pad_l = uint(cv.pad_l), dil = uint(cv.dilation), t_in = uint(t_in),
        t_out = uint(t_out), woff = uint(woff), boff = uint(boff))
    enc_tts_pool_dw_cls(raw, h, s, pa, t_out * cv.cin)
    vt_pt(raw, VtProfRole.tts_rows)
}

//! out [rows][c] = src[idx[r]] - the nearest x2 upsample with idx = r / 2
def private ts_gather_rows(raw : VkCommandBuffer; var h : VkHaz; src, idx, out : TsRows; rows, c : int64) {
    var s = set_tts_row_gather_cls(fixed_array(src.dev, idx.dev, out.dev), fixed_array(src.bytes, idx.bytes, out.bytes), fixed_array(src.bit, idx.bit, out.bit))
    var pa = TtsGatherArgs(c = uint(c), rows = uint(rows), word_off = 0u, pos_off = 0u, type_off = 0u, out_off = 0u, adds = 0u)
    enc_tts_row_gather_cls(raw, h, s, pa, rows * c)
    vt_pt(raw, VtProfRole.tts_rows)
}

//! o = (a + b) * scale over n elements (o may be a)
def private ts_add_scale(raw : VkCommandBuffer; var h : VkHaz; a, b, o : TsRows; n : int64; scale : float) {
    var s = set_tts_add_scale_cls(fixed_array(a.dev, b.dev, o.dev), fixed_array(a.bytes, b.bytes, o.bytes), fixed_array(a.bit, b.bit, o.bit))
    var pa = TtsAddScaleArgs(nelem = uint(n), scale = scale, aoff = 0u, boff = 0u, ooff = 0u)
    enc_tts_add_scale_cls(raw, h, s, pa, n)
    vt_pt(raw, VtProfRole.tts_rows)
}

def private ts_pred_ensure : bool {
    return (ts_lstm_wg_ok() && ensure_f32_gemm_b_cls() && ensure_tower_ln_cls() && ensure_tts_row_gather_cls() && ensure_tts_im2col_cls() && ensure_tts_lstm_dir_cls()
        && ensure_tts_concat_cls() && ensure_tts_sigsum_cls() && ensure_tts_colpart_sum_cls() && ensure_tts_colpart_sq_cls()
        && ensure_tts_colreduce_mean_cls() && ensure_tts_colreduce_sq_cls() && ensure_tts_adain_leaky_cls()
        && ensure_tts_pool_dw_cls() && ensure_tts_add_scale_cls())
}

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
    if (!ts_pred_attach(p) || !ts_scratch_ready(g_ts_de_scratch, fl, 4l + sd + int64(nl) * (2l * cc + 4l), 0l, t * width)) {
        return ts_decline("dur_enc", VulkanTtsDecline.memory)
    }
    let sl & = g_ts_pred_slots
    let sc_ & = g_ts_de_scratch
    let slab = g_ts_pred.rows
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
    ts_upload(r_bert, bert, t * hidden)
    let pa_cat = TtsConcatArgs(c = uint(cc), rc = uint(sd), rs = 0u, f0s = 0u, ns = 0u, with_res = 1u, width = uint(width), rows = uint(t))
    vt_chain_one_shot("dur_enc", int64(nl), t) $(raw : VkCommandBuffer; var h : VkHaz) {
        ts_lin(raw, h, slab, sl.bert_enc, hidden, cc, r_bert, r_den, t)
        ts_concat(raw, h, r_den, aux, aux, aux, r_x, pa_cat)
        for (i in range(nl)) {
            ts_bilstm(raw, h, slab, sl.de_lstm[i], p.lstms[i], t, r_x, r_gf, r_gb, r_y)
            ts_ln_at(raw, h, aux, hoff[i], hoff[i] + cc, r_y, r_y, t, cc, ST2_ADAIN_EPS)
            ts_concat(raw, h, r_y, aux, aux, aux, r_x, pa_cat)
        }
    }
    ts_readback(r_x, sc_, out, t * width)
    g_ts_encodes++
    return true
}

let private TS_DU_D = 0
let private TS_DU_Y = 1
let private TS_DU_GF = 2
let private TS_DU_GB = 3
let private TS_DU_LOG = 4
let private TS_DU_RAW = 5

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
    if (!ts_pred_attach(p) || !ts_scratch_ready(g_ts_du_scratch, fl, 0l, 0l, t)) {
        return ts_decline("durations", VulkanTtsDecline.memory)
    }
    let sl & = g_ts_pred_slots
    let sc_ & = g_ts_du_scratch
    let slab = g_ts_pred.rows
    let r_d = ts_rows(sc_, TS_DU_D)
    let r_y = ts_rows(sc_, TS_DU_Y)
    let r_gf = ts_rows(sc_, TS_DU_GF)
    let r_gb = ts_rows(sc_, TS_DU_GB)
    let r_log = ts_rows(sc_, TS_DU_LOG)
    let r_raw = ts_rows(sc_, TS_DU_RAW)
    ts_upload(r_d, d, t * width)
    vt_chain_one_shot("durations", 1l, t) $(raw_ : VkCommandBuffer; var h : VkHaz) {
        ts_bilstm(raw_, h, slab, sl.dur_lstm, p.lstm, t, r_d, r_gf, r_gb, r_y)
        ts_lin(raw_, h, slab, sl.dur_proj, 2l * hd, nb, r_y, r_log, t)
        ts_sigsum(raw_, h, r_log, r_raw, t, nb, speed)
    }
    ts_readback(r_raw, sc_, raw, t)
    g_ts_encodes++
    return true
}

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
struct private TsShape {
    t_max : int64
    c_max : int64
    col : int64      //! the widest im2col a conv takes: its output rows x its padded row
    kdim : int64     //! the widest padded row among them
    aux : int64
}

//! the im2col floats a conv over `rows` output rows takes
def private ts_col_of(c : TtsConv1d; rows : int64) : int64 => rows * ts_conv_kdim(c)

def private ts_conv_kdim(c : TtsConv1d) : int64 => round_up(c.k * c.cin, 32l)

//! a conv over `rows` output rows noted on the shape: the widest im2col and the widest padded row
def private ts_note_col(var sh : TsShape; c : TtsConv1d; rows : int64) {
    sh.col = max(sh.col, ts_col_of(c, rows))
    sh.kdim = max(sh.kdim, ts_conv_kdim(c))
}

let private TS_COL16_SLACK_ROWS = 64l   //! the half column slot's rows past a conv's: the KHR tile loads whole 16-row blocks

//! the half column slot's floats: the widest im2col as halves, the slack rows past it
def private ts_col16_floats(sh : TsShape) : int64 => (sh.col + TS_COL16_SLACK_ROWS * sh.kdim) / 2l + 1l

//! a residual block's rows form on the seat, its norms reading a style `sd` wide, or -1: the block's output row count, the shape noted
def private ts_res_shape(var sh : TsShape; b : St2AdainResBlk; t, sd : int64) : int64 {
    return -1l if (b.dim_in_s % 32l != 0l || b.dim_out % 64l != 0l || b.dim_in_s < b.dim_in || !tts_res_ok(b) || b.conv1.transposed || b.conv2.transposed
        || b.conv1.cin != b.dim_in_s || b.conv1.cout != b.dim_out || b.conv2.cin != b.dim_out || b.conv2.cout != b.dim_out
        || (b.learned_sc && (b.conv1x1.transposed || b.conv1x1.cout != b.dim_out || b.conv1x1.cin != b.dim_in_s))
        || !tts_lin_ok(b.norm1.fc) || b.norm1.fc.nout != 2l * b.dim_in_s || !tts_lin_ok(b.norm2.fc) || b.norm2.fc.nout != 2l * b.dim_out
        || b.norm1.fc.nin != sd || b.norm2.fc.nin != sd)
    var t1 = t
    if (b.upsample) {
        return -1l if (b.pool.groups != b.pool.cin || b.pool.cin != b.pool.cout || !b.pool.transposed || empty(b.pool.w) || b.pool.cin != b.dim_in_s)
        t1 = conv1d_out_len(b.pool, t)
        return -1l if (t1 != 2l * t)
    }
    return -1l if (conv1d_out_len(b.conv1, t1) != t1 || conv1d_out_len(b.conv2, t1) != t1 || (b.learned_sc && conv1d_out_len(b.conv1x1, t1) != t1))
    sh.t_max = max(sh.t_max, t1)
    sh.c_max = max(sh.c_max, max(b.dim_in_s, b.dim_out))
    ts_note_col(sh, b.conv1, t1)
    ts_note_col(sh, b.conv2, t1)
    if (b.learned_sc) {
        ts_note_col(sh, b.conv1x1, t1)
    }
    sh.aux += 2l * (b.dim_in_s + b.dim_out) + 8l
    return t1
}

//! a branch's blocks then its projection on the seat, or -1: the projection's row count
def private ts_branch_shape(var sh : TsShape; blocks : array<St2AdainResBlk>; proj : TtsConv1d; frames, cin, sd : int64) : int64 {
    return -1l if (length(blocks) > TS_MAX_RES_BLOCKS || !tts_conv_ok(proj) || proj.transposed || proj.cout != 1l)
    var t = frames
    var c = cin
    for (b in blocks) {
        return -1l if (b.dim_in_s != c)
        t = ts_res_shape(sh, b, t, sd)
        return -1l if (t < 0l)
        c = b.dim_out
    }
    return -1l if (proj.cin != c || conv1d_out_len(proj, t) != t)
    ts_note_col(sh, proj, t)
    sh.c_max = max(sh.c_max, c)
    return t
}

//! a seat's slots for the residual blocks' rows: the normed rows, the pooled rows, the first conv's rows, the two shortcuts, the im2col columns, the r / 2 map
struct private TsBlkIdx {
    r : int
    pool : int
    c1 : int
    sc : int
    sc2 : int
    col : int
    col16 : int     //! the half columns' slot, -1 where the blocks stay on the f32 tile (the prosody branches)
    idx : int
}

let private TS_PR_BLK = TsBlkIdx(r = TS_PR_R, pool = TS_PR_POOL, c1 = TS_PR_C1, sc = TS_PR_SC, sc2 = TS_PR_SC2, col = TS_PR_COL, col16 = -1, idx = TS_PR_IDX)

//! the CPU adain_res_blk, dispatch for dispatch, from bin [t][dim_in_s] into bout [t1][dim_out]; t1 back
def private ts_res_blk(raw : VkCommandBuffer; var h : VkHaz; slab : TsRows; sc_ : TsScratch; bi : TsBlkIdx; b : St2AdainResBlk; sl : TtsResSlots; hoff1, hoff2 : int64;
                       bin : TsRows; t : int64; bout : TsRows) : int64 {
    let r_r = ts_rows(sc_, bi.r)
    let r_pool = ts_rows(sc_, bi.pool)
    let r_c1 = ts_rows(sc_, bi.c1)
    let r_sc = ts_rows(sc_, bi.sc)
    let r_sc2 = ts_rows(sc_, bi.sc2)
    let r_col = ts_rows(sc_, bi.col)
    let half = bi.col16 >= 0
    let r_col16 = half ? ts_rows(sc_, bi.col16) : TsRows()
    ts_adain(raw, h, slab, sc_, bin, r_r, t, b.dim_in_s, sl.n1, hoff1, false)
    var t1 = t
    if (b.upsample) {
        t1 = conv1d_out_len(b.pool, t)
        ts_pool_dw(raw, h, slab, b.pool, sl.pool_w, sl.pool_b, r_r, r_pool, t, t1)
        ts_conv_at(raw, h, slab, sl.c1, b.conv1, t1, t1, r_pool, r_col, r_col16, r_c1, half)
    } else {
        ts_conv_at(raw, h, slab, sl.c1, b.conv1, t1, t1, r_r, r_col, r_col16, r_c1, half)
    }
    ts_adain(raw, h, slab, sc_, r_c1, r_c1, t1, b.dim_out, sl.n2, hoff2, false)
    ts_conv_at(raw, h, slab, sl.c2, b.conv2, t1, t1, r_c1, r_col, r_col16, bout, half)
    let n = t1 * b.dim_out
    if (b.upsample) {
        ts_gather_rows(raw, h, bin, ts_rows(sc_, bi.idx), r_sc, t1, b.dim_in_s)
        if (b.learned_sc) {
            ts_conv_at(raw, h, slab, sl.c1x1, b.conv1x1, t1, t1, r_sc, r_col, r_col16, r_sc2, half)
            ts_add_scale(raw, h, bout, r_sc2, bout, n, TS_JOIN)
        } else {
            ts_add_scale(raw, h, bout, r_sc, bout, n, TS_JOIN)
        }
    } elif (b.learned_sc) {
        ts_conv_at(raw, h, slab, sl.c1x1, b.conv1x1, t1, t1, bin, r_col, r_col16, r_sc2, half)
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
        t = ts_res_blk(raw, h, slab, sc_, TS_PR_BLK, blocks[i], slots[i], hoffs[hbase + 2 * i], hoffs[hbase + 2 * i + 1], src, t, dst)
    }
    let fin = nb == 0 ? x : (nb % 2 == 1 ? r_a : r_b)
    ts_conv(raw, h, slab, pslot, proj, t, t, fin, ts_rows(sc_, TS_PR_COL), out)
    return t
}

//! the prosody seat's slots for this call's geometry
def private ts_pr_floats(sh : TsShape; cc, hd, frames : int64) : TsFloats {
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
    fl[TS_PR_COL] = sh.col
    fl[TS_PR_IDX] = tm
    fl[TS_PR_F0] = tm
    fl[TS_PR_N] = tm
    return fl
}

//! the style rows on the device: per branch (the energy branch's from `nbase`), per block, its two norms' fc(style); false where the aux rows ran short
def private ts_pr_style_rows(p : St2Predictor; s : array<float>; var hoffs : int64[TS_MAX_RES_BLOCKS * 4]; nbase : int; sc_ : TsScratch) : bool {
    ts_aux_reset()
    for (i in range(length(p.f0))) {
        ts_res_style_rows(hoffs, 2 * i, p.f0[i], s)
    }
    for (i in range(length(p.n))) {
        ts_res_style_rows(hoffs, nbase + 2 * i, p.n[i], s)
    }
    return ts_aux_upload(sc_)
}

//! an AdaIN residual block's two norms' fc(style) staged, their offsets into `hoffs[at]` and `hoffs[at + 1]`
def private ts_res_style_rows(var hoffs : auto(HA); at : int; b : St2AdainResBlk; s : array<float>) {
    hoffs[at] = ts_style_rows(b.norm1.fc, s, b.dim_in_s, false)
    hoffs[at + 1] = ts_style_rows(b.norm2.fc, s, b.dim_out, false)
}

[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_TTS.md#vk-tts-chain")]
def private vulkan_styletts2_prosody(p : St2Predictor; en : array<float>; cc, frames : int64; s : array<float>;
                                     @scratch @exact_size var f0 : array<float>; @scratch @exact_size var n : array<float>; var f2 : int64&; var sc : St2Scratch) : bool {
    if (!vulkan_tower_on()) {
        return ts_decline("prosody", VulkanTtsDecline.knob)
    }
    let hd = p.shared_lstm.fwd.hidden
    let sd = long_length(s)
    var sh = TsShape(t_max = frames, c_max = max(cc, 2l * hd), col = 32l, aux = 4l)
    if (frames < 1l || cc % 32l != 0l || !tts_lstm_ok(p.shared_lstm, cc, TS_LSTM_MAX_HIDDEN) || empty(p.f0) || long_length(en) < cc * frames
        || !tts_pred_ok(p, TS_LSTM_MAX_HIDDEN)) {
        return ts_decline("prosody", VulkanTtsDecline.shape)
    }
    let tf = ts_branch_shape(sh, p.f0, p.f0_proj, frames, 2l * hd, sd)
    let tn = ts_branch_shape(sh, p.n, p.n_proj, frames, 2l * hd, sd)
    if (tf < 0l || tn < 0l || tf != tn) {
        return ts_decline("prosody", VulkanTtsDecline.shape)
    }
    if (!vulkan_tower_arms() || !ts_pred_ensure()) {
        return ts_decline("prosody", VulkanTtsDecline.device)
    }
    let fl = ts_pr_floats(sh, cc, hd, frames)
    if (!ts_pred_attach(p) || !ts_scratch_ready(g_ts_pr_scratch, fl, sh.aux, tts_stats_floats(sh.t_max, sh.c_max), max(tf, tn))) {
        return ts_decline("prosody", VulkanTtsDecline.memory)
    }
    let sl & = g_ts_pred_slots
    let sc_ & = g_ts_pr_scratch
    let slab = g_ts_pred.rows
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
    ts_idx_upload(r_idx, sh.t_max)
    var en_tm & = unsafe(sc.pr_en_tm)
    transpose_ct(en, cc, frames, en_tm)
    ts_upload(r_en, en_tm, frames * cc)
    vt_chain_one_shot("prosody", int64(length(p.f0) + length(p.n)), frames) $(raw : VkCommandBuffer; var h : VkHaz) {
        ts_bilstm(raw, h, slab, sl.sh_lstm, p.shared_lstm, frames, r_en, r_gf, r_gb, r_y)
        ts_branch(raw, h, slab, sc_, p.f0, sl.f0b, p.f0_proj, sl.f0_proj, hoffs, 0, r_y, frames, r_f0)
        ts_branch(raw, h, slab, sc_, p.n, sl.nb, p.n_proj, sl.n_proj, hoffs, nbase, r_y, frames, r_n)
    }
    ts_readback(r_f0, sc_, f0, tf)
    ts_readback(r_n, sc_, n, tn)
    f2 = tf
    g_ts_encodes++
    return true
}

var private g_ts_dec : TsSlab
var private g_ts_dec_slots : TtsDecSlab
var private g_ts_dec_scratch : TsScratch
var private @scratch g_ts_hoffs : array<int64>   //! the style rows' offsets, two a norm site, per block from `g_ts_hbase`
var private @scratch g_ts_hbase : array<int64>

let private TS_MAX_STAGES = 8l   //! the generator stages the seat holds slots for

def private ts_dec_attach(d : St2Decoder) : bool {
    let key = tts_dec_key(d)
    return ts_slab_current(g_ts_dec, key) || ts_dec_rebuild(d, key)
}

[cold_path]
def private ts_dec_rebuild(d : St2Decoder; key : uint64) : bool {
    return ts_slab_rebuild(g_ts_dec, g_ts_dec_slots, key, f16_gemm_on(), "decoder slab") $(var w : TtsSlabWriter; var s : TtsDecSlab) {
        tts_dec_write(w, d, s)
    }
}

let private TS_D_X = 0        //! the stream rows the decode blocks and the generator ping-pong with BO
let private TS_D_ASR = 1      //! the aligned text rows [frames][text_c]
let private TS_D_RES = 2      //! ... through asr_res
let private TS_D_F0R = 3      //! F0 and energy [f2][1]
let private TS_D_NR = 4
let private TS_D_F0D = 5      //! ... through their front convs [frames][1]
let private TS_D_ND = 6
let private TS_D_CAT = 7      //! the concat rows a decode block reads
let private TS_D_NU = 8       //! the source: the initial phases [nh]
let private TS_D_NOISE = 9    //! ... the noise rows [samples][nh]
let private TS_D_LOW = 10     //! ... the phase increments and the phase per frame [nh][nlow]
let private TS_D_LOWC = 11
let private TS_D_HAR = 12     //! ... the mixed signal [samples]
let private TS_D_SPEC = 13    //! ... its spectrum rows [spec_frames][2 bins]
let private TS_D_WAVE = 14    //! the waveform out
let private TS_D_POST = 15    //! conv_post's rows [t_post][cout]
let private TS_D_BY = 16      //! the generator's rows: a stage's sum, the source rows, a block's out, two block temporaries, the ping-pong twin of X
let private TS_D_BSRC = 17
let private TS_D_BSRES = 18
let private TS_D_BXT = 19
let private TS_D_BY1 = 20
let private TS_D_BO = 21
let private TS_D_R = 22       //! the residual blocks' rows
let private TS_D_POOL = 23
let private TS_D_C1 = 24
let private TS_D_SC = 25
let private TS_D_SC2 = 26
let private TS_D_COL = 27
let private TS_D_IDX = 28
let private TS_D_COL16 = 29   //! the im2col columns as halves, the f16 tile's feed

let private TS_D_BLK = TsBlkIdx(r = TS_D_R, pool = TS_D_POOL, c1 = TS_D_C1, sc = TS_D_SC, sc2 = TS_D_SC2, col = TS_D_COL, col16 = TS_D_COL16, idx = TS_D_IDX)

//! the decode chain's geometry: the blocks' widest rows, the generator's stages, the source and the inverse STFT
struct private TsDecShape {
    blk : TsShape
    dec_rows : int64
    dec_width : int64
    t_last : int64      //! the generator's last stage: its rows and width
    c_last : int64
    t_post : int64
    samples : int64
    nlow : int64
    spec_frames : int64
    n_out : int64
}

def private ts_gen_block_shape(var sh : TsShape; b : St2GenResBlock; t, c : int64) : bool {
    let n = length(b.convs1)
    return false if (b.c != c || n == 0 || length(b.convs2) != n || length(b.adain1) != n || length(b.adain2) != n
        || length(b.alpha1) != n || length(b.alpha2) != n)
    for (j in range(n)) {
        let c1 & = unsafe(b.convs1[j])
        let c2 & = unsafe(b.convs2[j])
        return false if (!tts_conv_ok(c1) || !tts_conv_ok(c2) || c1.transposed || c2.transposed || c1.cin != c || c1.cout != c || c2.cin != c || c2.cout != c
            || conv1d_out_len(c1, t) != t || conv1d_out_len(c2, t) != t || long_length(b.alpha1[j].a) < c || long_length(b.alpha2[j].a) < c
            || !tts_lin_ok(b.adain1[j].fc) || b.adain1[j].fc.nout != 2l * c || !tts_lin_ok(b.adain2[j].fc) || b.adain2[j].fc.nout != 2l * c)
        ts_note_col(sh, c1, t)
        ts_note_col(sh, c2, t)
    }
    sh.aux += int64(n) * (4l * c + 16l)
    return true
}

//! the generator's shape from the CPU chain's own length rules; false on any shape the kernels do not take
def private ts_gen_shape(d : St2Decoder; c, t, har_frames : int64; var sh : TsDecShape) : bool {
    let nst = long_length(d.ups)
    return false if (nst == 0l || nst > TS_MAX_STAGES || t < 1l || har_frames < 1l || d.n_kernels < 1l
        || long_length(d.noise_convs) != nst || long_length(d.noise_res) != nst || long_length(d.resblocks) != nst * d.n_kernels)
    var tt = t
    var cc = c
    sh.blk.c_max = max(sh.blk.c_max, c)
    sh.blk.t_max = max(sh.blk.t_max, t)
    for (i in range64(nst)) {
        let up & = unsafe(d.ups[i])
        let nc & = unsafe(d.noise_convs[i])
        return false if (!tts_conv_ok(up) || !tts_conv_ok(nc) || nc.transposed || up.cin != cc || nc.cin != d.noise_convs[0].cin)
        var t2 = conv1d_out_len(up, tt)
        if (i == nst - 1l) {
            t2++
        }
        let c2 = up.cout
        return false if (c2 % 64l != 0l || nc.cout != c2 || conv1d_out_len(nc, har_frames) != t2 || !ts_gen_block_shape(sh.blk, d.noise_res[i], t2, c2))
        for (j in range64(d.n_kernels)) {
            return false if (!ts_gen_block_shape(sh.blk, d.resblocks[i * d.n_kernels + j], t2, c2))
        }
        ts_note_col(sh.blk, up, t2)
        ts_note_col(sh.blk, nc, t2)
        sh.blk.t_max = max(sh.blk.t_max, t2)
        sh.blk.c_max = max(sh.blk.c_max, c2)
        tt = t2
        cc = c2
    }
    return false if (!tts_conv_ok(d.conv_post) || d.conv_post.transposed || d.conv_post.cin != cc || d.conv_post.cout < 2l * d.bins)
    sh.t_post = conv1d_out_len(d.conv_post, tt)
    ts_note_col(sh.blk, d.conv_post, sh.t_post)
    sh.t_last = tt
    sh.c_last = cc
    return true
}

//! the shared source geometry into the decode shape; false where the source parts do not fit the stamps
def private ts_source_shape(d : St2Decoder; cfg : SineSourceCfg; f2 : int64; var sh : TsDecShape) : bool {
    let ss = tts_source_shape(d, cfg, f2)
    sh.samples = ss.samples
    sh.nlow = ss.nlow
    sh.spec_frames = ss.spec_frames
    return ss.ok
}

//! the decode chain's shape: the front convs and the residual as the concat rows ask, the AdaIN residual blocks, the source, the generator, the inverse STFT
def private ts_decode_shape(d : St2Decoder; text_c : int64; source : SineSourceCfg; frames, f2 : int64; noise : TtsNoise; sd : int64; var sh : TsDecShape) : bool {
    let bwd & = unsafe(d.stft_bwd_re)
    return false if (frames < 1l || f2 < 1l || text_c % 32l != 0l || !tts_conv_ok(d.f0_conv) || !tts_conv_ok(d.n_conv) || !tts_conv_ok(d.asr_res)
        || d.asr_res.transposed || d.f0_conv.cin != 1l || d.n_conv.cin != 1l || d.f0_conv.cout != 1l || d.n_conv.cout != 1l
        || conv1d_out_len(d.f0_conv, f2) != frames || conv1d_out_len(d.n_conv, f2) != frames || d.asr_res.cin != text_c || d.asr_res.cout % 64l != 0l
        || conv1d_out_len(d.asr_res, frames) != frames || d.encode.dim_in != text_c + 2l || !ts_source_shape(d, source, f2, sh)
        || bwd.cin != d.bins || bwd.cout != 1l || !bwd.transposed || empty(bwd.wt) || d.stft_bwd_im.k != bwd.k
        || (d.istft_envelope && long_length(d.window.a) < bwd.k) || (noise.captured && long_length(noise.normal) < sh.samples * source.n_harm)
        || long_length(noise.uniform) < source.n_harm || !tts_dec_ok(d))
    sh.blk = TsShape(t_max = frames, c_max = max(text_c, d.encode.dim_in_s), col = 32l, aux = 8l)
    ts_note_col(sh.blk, d.f0_conv, frames)
    ts_note_col(sh.blk, d.n_conv, frames)
    ts_note_col(sh.blk, d.asr_res, frames)
    sh.dec_rows = ts_res_shape(sh.blk, d.encode, frames, sd)
    return false if (sh.dec_rows < 0l || length(d.decode) > TS_MAX_RES_BLOCKS)
    sh.dec_width = d.encode.dim_out
    for (b in d.decode) {
        return false if ((b.dim_in != sh.dec_width && b.dim_in != sh.dec_width + d.asr_res.cout + 2l) || (b.dim_in == sh.dec_width && b.dim_in_s != b.dim_in))
        sh.dec_rows = ts_res_shape(sh.blk, b, sh.dec_rows, sd)
        return false if (sh.dec_rows < 0l)
        sh.dec_width = b.dim_out
    }
    return false if (!ts_gen_shape(d, sh.dec_width, sh.dec_rows, sh.spec_frames, sh))
    sh.n_out = conv1d_out_len(bwd, sh.t_post) - 2l * ST2_STFT_PAD
    return sh.n_out >= 1l
}

//! the generator's slots for this call's geometry: the stream rows, conv_post's rows, the block rows and the columns (the generator seat's whole table)
def private ts_gen_floats(d : St2Decoder; sh : TsDecShape) : TsFloats {
    let rows = sh.blk.t_max * sh.blk.c_max
    var fl : TsFloats
    fl[TS_D_X] = rows
    fl[TS_D_POST] = sh.t_post * d.conv_post.cout
    for (i in range(TS_D_BY, TS_D_SC2 + 1)) {
        fl[i] = rows
    }
    fl[TS_D_COL] = sh.blk.col
    fl[TS_D_COL16] = ts_col16_floats(sh.blk)
    return fl
}

//! the decode seat's slots for this call's geometry: the generator's, then the front, the concat rows, the source and the wave
def private ts_dec_floats(d : St2Decoder; sh : TsDecShape; nh, text_c, frames, f2 : int64) : TsFloats {
    var fl = ts_gen_floats(d, sh)
    fl[TS_D_ASR] = frames * text_c
    fl[TS_D_RES] = frames * d.asr_res.cout
    fl[TS_D_F0R] = f2
    fl[TS_D_NR] = f2
    fl[TS_D_F0D] = frames
    fl[TS_D_ND] = frames
    fl[TS_D_CAT] = fl[TS_D_X]
    fl[TS_D_NU] = nh
    fl[TS_D_NOISE] = sh.samples * nh
    fl[TS_D_LOW] = nh * sh.nlow
    fl[TS_D_LOWC] = nh * sh.nlow
    fl[TS_D_HAR] = sh.samples
    fl[TS_D_SPEC] = sh.spec_frames * 2l * d.bins
    fl[TS_D_WAVE] = sh.n_out
    fl[TS_D_IDX] = sh.blk.t_max
    return fl
}

//! the decode blocks' style rows (two a block into `dhoff`), then the generator's (`ts_gen_style_rows`); false where the aux rows ran short
def private ts_dec_style_rows(d : St2Decoder; s : array<float>; var dhoff : int64[2 + 2 * TS_MAX_RES_BLOCKS]; sc_ : TsScratch) : bool {
    ts_aux_reset()
    ts_res_style_rows(dhoff, 0, d.encode, s)
    for (i in range(length(d.decode))) {
        ts_res_style_rows(dhoff, 2 + 2 * i, d.decode[i], s)
    }
    ts_gen_style_rows(d, s)
    return ts_aux_upload(sc_)
}

//! the generator's blocks' style rows alone (the generator seat's)
def private ts_gen_style_rows(d : St2Decoder; s : array<float>) {
    let nblk = long_length(d.noise_res) + long_length(d.resblocks)
    g_ts_hbase |> ensure_length(nblk)
    var n = 0l
    for (b in d.noise_res) {
        n += 2l * long_length(b.convs1)
    }
    for (b in d.resblocks) {
        n += 2l * long_length(b.convs1)
    }
    g_ts_hoffs |> ensure_length(n)
    var at = 0l
    var bi = 0l
    for (b in d.noise_res) {
        g_ts_hbase[bi] = at
        ts_block_style_rows(b, s, at)
        bi++
    }
    for (b in d.resblocks) {
        g_ts_hbase[bi] = at
        ts_block_style_rows(b, s, at)
        bi++
    }
}

def private ts_block_style_rows(b : St2GenResBlock; s : array<float>; var at : int64&) {
    for (j in range(length(b.convs1))) {
        g_ts_hoffs[at] = ts_style_rows(b.adain1[j].fc, s, b.c, false)
        g_ts_hoffs[at + 1l] = ts_style_rows(b.adain2[j].fc, s, b.c, false)
        at += 2l
    }
}

//! o += scale . y, or o = scale . y under `zero`
def private ts_axpy(raw : VkCommandBuffer; var h : VkHaz; o, y : TsRows; n : int64; scale : float; zero : bool) {
    var s = set_tts_axpy_cls(fixed_array(o.dev, y.dev), fixed_array(o.bytes, y.bytes), fixed_array(o.bit, y.bit))
    var pa = TtsAxpyArgs(nelem = uint(n), scale = scale, zero = zero ? 1u : 0u, ooff = 0u, yoff = 0u)
    enc_tts_axpy_cls(raw, h, s, pa, n)
    vt_pt(raw, VtProfRole.tts_rows)
}

//! a Snake residual block from bin into bout, the CPU `gen_res_block` dispatch for dispatch; `bi` the block's index in the slab's block order
def private ts_gen_res_block(raw : VkCommandBuffer; var h : VkHaz; slab : TsRows; sc_ : TsScratch; b : St2GenResBlock; sl : TtsBlockSlots; bi : int64;
                             bxt, by1 : TsRows; bin, bout : TsRows; t : int64) {
    let n = t * b.c
    let r_col = ts_rows(sc_, TS_D_COL)
    let r_col16 = ts_rows(sc_, TS_D_COL16)
    let hb = g_ts_hbase[bi]
    for (j in range(length(b.convs1))) {
        let src = j == 0 ? bin : bout
        ts_adain(raw, h, slab, sc_, src, bxt, t, b.c, sl.n1[j], g_ts_hoffs[hb + int64(2 * j)], true)
        ts_conv_at(raw, h, slab, sl.c1[j], b.convs1[j], t, t, bxt, r_col, r_col16, by1, true)
        ts_adain(raw, h, slab, sc_, by1, by1, t, b.c, sl.n2[j], g_ts_hoffs[hb + int64(2 * j + 1)], true)
        ts_conv_at(raw, h, slab, sl.c2[j], b.convs2[j], t, t, by1, r_col, r_col16, bxt, true)
        ts_add_scale(raw, h, src, bxt, bout, n, 1.0)
    }
}

//! the CPU generator_stage, dispatch for dispatch, from bx_in into bout: tt and cc carry the stream's rows and width across the stages
def private ts_stage(raw : VkCommandBuffer; var h : VkHaz; slab : TsRows; sc_ : TsScratch; d : St2Decoder; i : int64; bspec : TsRows; bx_in, bout : TsRows;
                     var tt, cc : int64&; har_frames : int64) {
    let sl & = g_ts_dec_slots
    let r_by = ts_rows(sc_, TS_D_BY)
    let r_bsrc = ts_rows(sc_, TS_D_BSRC)
    let r_bsres = ts_rows(sc_, TS_D_BSRES)
    let r_bxt = ts_rows(sc_, TS_D_BXT)
    let r_by1 = ts_rows(sc_, TS_D_BY1)
    let r_col = ts_rows(sc_, TS_D_COL)
    let r_col16 = ts_rows(sc_, TS_D_COL16)
    ts_leaky(raw, h, bx_in, tt * cc, 0.1)
    let nst = long_length(d.ups)
    let last = i == nst - 1l
    var t2 = conv1d_out_len(d.ups[i], tt)
    if (last) {
        t2++
    }
    let c2 = d.ups[i].cout
    let src_t = conv1d_out_len(d.noise_convs[i], har_frames)
    ts_conv_at(raw, h, slab, sl.noise[i], d.noise_convs[i], har_frames, src_t, bspec, r_col, r_col16, r_bsrc, true)
    ts_gen_res_block(raw, h, slab, sc_, d.noise_res[i], sl.blocks[i], i, r_bxt, r_by1, r_bsrc, r_bsres, src_t)
    ts_conv_at(raw, h, slab, sl.ups[i], d.ups[i], tt, last ? t2 - 1l : t2, bx_in, r_col, r_col16, last ? r_bxt : r_by, true)
    let n = t2 * c2
    if (last) {
        var s = set_tts_pk_rows_cls(fixed_array(r_bxt.dev, r_by.dev), fixed_array(r_bxt.bytes, r_by.bytes), fixed_array(r_bxt.bit, r_by.bit))
        var shift = TtsPkRowsArgs(rows = uint(t2 - 1l), width = uint(c2), ss = uint(c2), sc = 0u, sr0 = 0u, ds = uint(c2), dc = 0u, dr0 = 1u)
        enc_tts_pk_rows_cls(raw, h, s, shift, n - c2)
        var head = TtsPkRowsArgs(rows = 1u, width = uint(c2), ss = uint(c2), sc = 0u, sr0 = 1u, ds = uint(c2), dc = 0u, dr0 = 0u)
        enc_tts_pk_rows_cls(raw, h, s, head, c2)
        vt_pt(raw, VtProfRole.tts_rows)
    }
    ts_add_scale(raw, h, r_by, r_bsres, r_by, n, 1.0)
    let nk = d.n_kernels
    let nnoise = long_length(d.noise_res)
    for (j in range64(nk)) {
        ts_gen_res_block(raw, h, slab, sc_, d.resblocks[i * nk + j], sl.blocks[nnoise + i * nk + j], nnoise + i * nk + j, r_bxt, r_by1, r_by, r_bsres, t2)
        ts_axpy(raw, h, bout, r_bsres, n, 1.0 / float(nk), j == 0l)
    }
    tt = t2
    cc = c2
}

//! the generator from the rows in X through conv_post into POST [t_post][cout]
def private ts_generator_chain(raw : VkCommandBuffer; var h : VkHaz; slab : TsRows; sc_ : TsScratch; d : St2Decoder; cx, t, har_frames : int64) {
    let r_x = ts_rows(sc_, TS_D_X)
    let r_bo = ts_rows(sc_, TS_D_BO)
    let r_spec = ts_rows(sc_, TS_D_SPEC)
    let nst = long_length(d.ups)
    var tt = t
    var cc = cx
    for (i in range64(nst)) {
        if (i % 2l == 0l) {
            ts_stage(raw, h, slab, sc_, d, i, r_spec, r_x, r_bo, tt, cc, har_frames)
        } else {
            ts_stage(raw, h, slab, sc_, d, i, r_spec, r_bo, r_x, tt, cc, har_frames)
        }
    }
    let fin = nst % 2l == 1l ? r_bo : r_x
    ts_leaky(raw, h, fin, tt * cc, 0.01)
    ts_conv_at(raw, h, slab, g_ts_dec_slots.post, d.conv_post, tt, conv1d_out_len(d.conv_post, tt), fin, ts_rows(sc_, TS_D_COL), ts_rows(sc_, TS_D_COL16),
        ts_rows(sc_, TS_D_POST), true)
}

//! the source: the noise drawn unless captured, the phases on the law's stamps, the mixed signal, its spectrum on the pad law's stamp
def private ts_source_chain(raw : VkCommandBuffer; var h : VkHaz; slab : TsRows; sc_ : TsScratch; d : St2Decoder; cfg : SineSourceCfg; sh : TsDecShape;
                            captured : bool; seed : uint64) {
    let sl & = g_ts_dec_slots
    let nh = cfg.n_harm
    let r_f0 = ts_rows(sc_, TS_D_F0R)
    let r_nu = ts_rows(sc_, TS_D_NU)
    let r_noise = ts_rows(sc_, TS_D_NOISE)
    let r_low = ts_rows(sc_, TS_D_LOW)
    let r_lowc = ts_rows(sc_, TS_D_LOWC)
    let r_har = ts_rows(sc_, TS_D_HAR)
    let r_spec = ts_rows(sc_, TS_D_SPEC)
    var ka = TtsSourceArgs(n = uint(sh.samples), nlow = uint(sh.nlow), nh = uint(nh), up = uint(cfg.upsample), sr = cfg.sample_rate, sine_amp = cfg.sine_amp,
        noise_std = cfg.noise_std, voiced_thr = cfg.voiced_thr, seed = uint(seed ^ (seed >> 32ul)), woff = uint(sl.src_w), boff = uint(sl.src_b))
    if (!captured) {
        var s0 = set_tts_src_noise_cls(fixed_array(r_noise.dev), fixed_array(r_noise.bytes), fixed_array(r_noise.bit))
        enc_tts_src_noise_cls(raw, h, s0, ka, sh.samples * nh)
    }
    let low_bufs = fixed_array(r_f0.dev, r_nu.dev, r_low.dev)
    let low_sizes = fixed_array(r_f0.bytes, r_nu.bytes, r_low.bytes)
    let low_bits = fixed_array(r_f0.bit, r_nu.bit, r_low.bit)
    let cum_bufs = fixed_array(r_low.dev, r_lowc.dev)
    let cum_sizes = fixed_array(r_low.bytes, r_lowc.bytes)
    let cum_bits = fixed_array(r_low.bit, r_lowc.bit)
    let sin_bufs = fixed_array(r_f0.dev, r_lowc.dev, r_noise.dev, slab.dev, r_har.dev)
    let sin_sizes = fixed_array(r_f0.bytes, r_lowc.bytes, r_noise.bytes, slab.bytes, r_har.bytes)
    let sin_bits = fixed_array(r_f0.bit, r_lowc.bit, r_noise.bit, 0u, r_har.bit)
    if (cfg.torch_math) {
        var s1 = set_tts_src_low_torch_cls(low_bufs, low_sizes, low_bits)
        enc_tts_src_low_torch_cls(raw, h, s1, ka, nh * sh.nlow)
        var s2 = set_tts_src_cumsum_torch_cls(cum_bufs, cum_sizes, cum_bits)
        enc_tts_src_cumsum_torch_cls(raw, h, s2, ka, 1l)
        var s3 = set_tts_src_sines_torch_cls(sin_bufs, sin_sizes, sin_bits)
        enc_tts_src_sines_torch_cls(raw, h, s3, ka, sh.samples)
        vt_pt(raw, VtProfRole.tts_source)
    } else {
        var s1 = set_tts_src_low_onnx_cls(low_bufs, low_sizes, low_bits)
        enc_tts_src_low_onnx_cls(raw, h, s1, ka, nh * sh.nlow)
        var s2 = set_tts_src_cumsum_onnx_cls(cum_bufs, cum_sizes, cum_bits)
        enc_tts_src_cumsum_onnx_cls(raw, h, s2, ka, 1l)
        var s3 = set_tts_src_sines_onnx_cls(sin_bufs, sin_sizes, sin_bits)
        enc_tts_src_sines_onnx_cls(raw, h, s3, ka, sh.samples)
        vt_pt(raw, VtProfRole.tts_source)
    }
    let re & = unsafe(d.stft_fwd_re)
    var ks = TtsStftArgs(n = uint(sh.samples), frames = uint(sh.spec_frames), bins = uint(d.bins), k = uint(re.k), hop = uint(re.stride), pad = uint(ST2_STFT_PAD),
        eps = d.stft_eps, reoff = uint(sl.stft_re), imoff = uint(sl.stft_im))
    let st_bufs = fixed_array(r_har.dev, slab.dev, r_spec.dev)
    let st_sizes = fixed_array(r_har.bytes, slab.bytes, r_spec.bytes)
    let st_bits = fixed_array(r_har.bit, 0u, r_spec.bit)
    if (d.stft_pad_reflect) {
        var s4 = set_tts_stft_reflect_cls(st_bufs, st_sizes, st_bits)
        enc_tts_stft_reflect_cls(raw, h, s4, ks, sh.spec_frames * d.bins)
    } else {
        var s4 = set_tts_stft_edge_cls(st_bufs, st_sizes, st_bits)
        enc_tts_stft_edge_cls(raw, h, s4, ks, sh.spec_frames * d.bins)
    }
    vt_pt(raw, VtProfRole.tts_stft)
}

//! the encode block, then each decode block (re-concatenated where its width asks); the stream lands in X
def private ts_decoder_chain(raw : VkCommandBuffer; var h : VkHaz; slab : TsRows; sc_ : TsScratch; d : St2Decoder; dhoff : int64[2 + 2 * TS_MAX_RES_BLOCKS];
                             text_c, frames : int64) {
    let sl & = g_ts_dec_slots
    let r_x = ts_rows(sc_, TS_D_X)
    let r_bo = ts_rows(sc_, TS_D_BO)
    let r_asr = ts_rows(sc_, TS_D_ASR)
    let r_res = ts_rows(sc_, TS_D_RES)
    let r_f0d = ts_rows(sc_, TS_D_F0D)
    let r_nd = ts_rows(sc_, TS_D_ND)
    let r_cat = ts_rows(sc_, TS_D_CAT)
    let rc = uint(d.asr_res.cout)
    let kc = TtsConcatArgs(c = uint(text_c), rc = rc, rs = rc, f0s = 1u, ns = 1u, with_res = 0u, width = uint(d.encode.dim_in_s), rows = uint(frames))
    ts_concat(raw, h, r_asr, r_res, r_f0d, r_nd, r_cat, kc)
    var tt = ts_res_blk(raw, h, slab, sc_, TS_D_BLK, d.encode, sl.encode, dhoff[0], dhoff[1], r_cat, frames, r_x)
    var cwr = d.encode.dim_out
    var in_x = true
    for (i in range(length(d.decode))) {
        let blk & = unsafe(d.decode[i])
        let src = in_x ? r_x : r_bo
        let dst = in_x ? r_bo : r_x
        if (blk.dim_in != cwr) {
            let kc2 = TtsConcatArgs(c = uint(cwr), rc = rc, rs = rc, f0s = 1u, ns = 1u, with_res = 1u, width = uint(blk.dim_in_s), rows = uint(tt))
            ts_concat(raw, h, src, r_res, r_f0d, r_nd, r_cat, kc2)
            tt = ts_res_blk(raw, h, slab, sc_, TS_D_BLK, blk, sl.decode[i], dhoff[2 + 2 * i], dhoff[3 + 2 * i], r_cat, tt, dst)
        } else {
            tt = ts_res_blk(raw, h, slab, sc_, TS_D_BLK, blk, sl.decode[i], dhoff[2 + 2 * i], dhoff[3 + 2 * i], src, tt, dst)
        }
        in_x = !in_x
        cwr = blk.dim_out
    }
    if (!in_x) {   //! the generator ping-pongs X and BO from X: the stream lands in X first
        ts_add_scale(raw, h, r_bo, r_bo, r_x, tt * cwr, 0.5)
    }
}

def private ts_dec_ensure : bool {
    return (ts_pred_ensure() && (!f16_gemm_on() || (ensure_tts_im2col16_cls() && f16_gemm_ensure(f16_gemm_khr()))) && ensure_tower_bias_act_cls()
        && ensure_tts_leaky_cls() && ensure_tts_adain_snake_cls() && ensure_tts_axpy_cls() && ensure_tts_pk_rows_cls() && ensure_tts_src_noise_cls()
        && ensure_tts_src_low_torch_cls() && ensure_tts_src_low_onnx_cls() && ensure_tts_src_cumsum_torch_cls() && ensure_tts_src_cumsum_onnx_cls()
        && ensure_tts_src_sines_torch_cls() && ensure_tts_src_sines_onnx_cls() && ensure_tts_stft_reflect_cls() && ensure_tts_stft_edge_cls()
        && ensure_tts_istft_cls())
}

//! the r / 2 row map for the nearest upsamples, `rows` of it
def private ts_idx_upload(r_idx : TsRows; rows : int64) {
    g_ts_idx_host |> ensure_length(rows)
    for (i in range64(rows)) {
        g_ts_idx_host[i] = int(i / 2l)
    }
    ts_upload(r_idx, g_ts_idx_host, rows)
}

[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_TTS.md#vk-tts-chain")]
def private vulkan_styletts2_decode(d : St2Decoder; text_c : int64; source : SineSourceCfg; asr : array<float>; frames : int64; f0, nrg : array<float>; f2 : int64;
                                    s_ref : array<float>; noise : TtsNoise; seed : uint64; @scratch @exact_size var wave : array<float>; var sc : St2Scratch) : bool {
    if (!vulkan_tower_on()) {
        return ts_decline("decode", VulkanTtsDecline.knob)
    }
    var sh = TsDecShape()
    if (!ts_decode_shape(d, text_c, source, frames, f2, noise, long_length(s_ref), sh) || long_length(asr) < text_c * frames
        || long_length(f0) < f2 || long_length(nrg) < f2) {
        return ts_decline("decode", VulkanTtsDecline.shape)
    }
    if (!vulkan_tower_arms() || !ts_dec_ensure()) {
        return ts_decline("decode", VulkanTtsDecline.device)
    }
    let nh = source.n_harm
    let fl = ts_dec_floats(d, sh, nh, text_c, frames, f2)
    if (!ts_dec_attach(d) || !ts_scratch_ready(g_ts_dec_scratch, fl, sh.blk.aux, tts_stats_floats(sh.blk.t_max, sh.blk.c_max), sh.n_out)) {
        return ts_decline("decode", VulkanTtsDecline.memory)
    }
    let sl & = g_ts_dec_slots
    let sc_ & = g_ts_dec_scratch
    let slab = g_ts_dec.rows
    var dhoff : int64[2 + 2 * TS_MAX_RES_BLOCKS]
    if (!ts_dec_style_rows(d, s_ref, dhoff, sc_)) {
        return ts_decline("decode", VulkanTtsDecline.memory)
    }
    let r_asr = ts_rows(sc_, TS_D_ASR)
    let r_f0 = ts_rows(sc_, TS_D_F0R)
    let r_nr = ts_rows(sc_, TS_D_NR)
    let r_wave = ts_rows(sc_, TS_D_WAVE)
    var asr_tm & = unsafe(sc.dec_asr_tm)
    transpose_ct(asr, text_c, frames, asr_tm)
    ts_upload(r_asr, asr_tm, frames * text_c)
    ts_upload(r_f0, f0, f2)
    ts_upload(r_nr, nrg, f2)
    ts_upload(ts_rows(sc_, TS_D_NU), noise.uniform, nh)
    if (noise.captured) {
        ts_upload(ts_rows(sc_, TS_D_NOISE), noise.normal, sh.samples * nh)
    }
    ts_idx_upload(ts_rows(sc_, TS_D_IDX), sh.blk.t_max)
    let bwd & = unsafe(d.stft_bwd_re)
    vt_chain_one_shot("decode", long_length(d.ups), frames) $(raw : VkCommandBuffer; var h : VkHaz) {
        let r_col = ts_rows(sc_, TS_D_COL)
        let r_col16 = ts_rows(sc_, TS_D_COL16)
        ts_conv(raw, h, slab, sl.f0c, d.f0_conv, f2, frames, r_f0, r_col, ts_rows(sc_, TS_D_F0D))
        ts_conv(raw, h, slab, sl.nc, d.n_conv, f2, frames, r_nr, r_col, ts_rows(sc_, TS_D_ND))
        ts_conv_at(raw, h, slab, sl.asr_res, d.asr_res, frames, frames, r_asr, r_col, r_col16, ts_rows(sc_, TS_D_RES), true)
        ts_decoder_chain(raw, h, slab, sc_, d, dhoff, text_c, frames)
        ts_source_chain(raw, h, slab, sc_, d, source, sh, noise.captured, seed)
        ts_generator_chain(raw, h, slab, sc_, d, sh.dec_width, sh.dec_rows, sh.spec_frames)
        let r_post = ts_rows(sc_, TS_D_POST)
        var ki = TtsIstftArgs(tt = uint(sh.t_post), stride = uint(d.conv_post.cout), bins = uint(d.bins), k = uint(bwd.k), hop = uint(bwd.stride),
            pad = uint(ST2_STFT_PAD), envelope = d.istft_envelope ? 1u : 0u, n = uint(sh.n_out), reoff = uint(sl.istft_re), imoff = uint(sl.istft_im),
            wnoff = uint(sl.window))
        var s = set_tts_istft_cls(fixed_array(r_post.dev, slab.dev, r_wave.dev), fixed_array(r_post.bytes, slab.bytes, r_wave.bytes), fixed_array(r_post.bit, 0u, r_wave.bit))
        enc_tts_istft_cls(raw, h, s, ki, sh.n_out)
        vt_pt(raw, VtProfRole.tts_stft)
    }
    ts_readback(r_wave, sc_, wave, sh.n_out)
    g_ts_encodes++
    return true
}

[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_TTS.md#vk-tts-chain")]
def private vulkan_styletts2_generator(d : St2Decoder; x_rows : array<float>; cx, t : int64; s : array<float>; har_spec : array<float>; har_frames : int64;
                                       @scratch @exact_size var wave : array<float>; var sc : St2Scratch) : bool {
    if (!vulkan_tower_on()) {
        return ts_decline("generator", VulkanTtsDecline.knob)
    }
    var sh = TsDecShape()
    sh.blk = TsShape(t_max = t, c_max = cx, col = 32l, aux = 8l)
    let spec_floats = har_frames * (empty(d.noise_convs) ? 0l : d.noise_convs[0].cin)
    if (cx % 32l != 0l || !ts_gen_shape(d, cx, t, har_frames, sh) || !tts_dec_ok(d) || long_length(x_rows) < t * cx || long_length(har_spec) < spec_floats) {
        return ts_decline("generator", VulkanTtsDecline.shape)
    }
    for (b in d.noise_res) {
        return ts_decline("generator", VulkanTtsDecline.shape) if (!empty(b.adain1) && b.adain1[0].fc.nin != long_length(s))
    }
    if (!vulkan_tower_arms() || !ts_dec_ensure()) {
        return ts_decline("generator", VulkanTtsDecline.device)
    }
    var fl = ts_gen_floats(d, sh)
    fl[TS_D_SPEC] = spec_floats
    if (!ts_dec_attach(d) || !ts_scratch_ready(g_ts_dec_scratch, fl, sh.blk.aux, tts_stats_floats(sh.blk.t_max, sh.blk.c_max), fl[TS_D_POST])) {
        return ts_decline("generator", VulkanTtsDecline.memory)
    }
    let sc_ & = g_ts_dec_scratch
    let slab = g_ts_dec.rows
    ts_aux_reset()
    ts_gen_style_rows(d, s)
    if (!ts_aux_upload(sc_)) {
        return ts_decline("generator", VulkanTtsDecline.memory)
    }
    let r_x = ts_rows(sc_, TS_D_X)
    let r_spec = ts_rows(sc_, TS_D_SPEC)
    let r_post = ts_rows(sc_, TS_D_POST)
    ts_upload(r_x, x_rows, t * cx)
    ts_upload(r_spec, har_spec, spec_floats)
    vt_chain_one_shot("generator", long_length(d.ups), t) $(raw : VkCommandBuffer; var h : VkHaz) {
        ts_generator_chain(raw, h, slab, sc_, d, cx, t, har_frames)
    }
    var y & = unsafe(sc.gen_post)
    ts_readback(r_post, sc_, y, sh.t_post * d.conv_post.cout)
    styletts2_istft(d, y, d.conv_post.cout, sh.t_post, wave, sc)
    g_ts_encodes++
    return true
}

let private TS_PK_ATTN_HS = int64(TTS_PK_ATTN_HS)             //! the attention row's head size, the kernel body's literal
let private TS_PK_ATTN_MAX_KEYS = int64(TTS_PK_ATTN_MAX_KEYS) //! the scores the attention row stages
let private TS_PK_GEMV_MAX_NIN = int64(TTS_PK_GEMV_MAX_NIN)   //! the x the row GEMV stages
let private TS_PK_CODEC_MAX_FRAMES = 512l    //! the one-shot codec chain's row budget: past this the CPU's windows serve
let private TS_PK_KV_BIT = 1u << 28u         //! the hazard bit the voice's key rows share, and the value rows'
let private TS_PK_VV_BIT = 1u << 29u

var private g_ts_pk_codec : TsSlab
var private g_ts_pk_codec_slots : TtsPkCodecSlab
var private g_ts_pk_frames : TsSlab
var private g_ts_pk_frames_slots : TtsPkFramesSlab
var private g_ts_pkc_scratch : TsScratch
var private g_ts_pkf_scratch : TsScratch
var private @scratch g_ts_pk_rows : array<int64>   //! the codec chain's row counts: the frames, the 200 Hz rows, each stage's rows
var private @scratch g_ts_pk_kv_host : array<float>

[cold_path]
def set_vulkan_pocket_frame_batch(n : int64) {
    set_pocket_frame_batch(n)
}

[cold_path]
def vulkan_pocket_frame_batch() : int64 => pocket_frame_batch()

def private ts_pk_codec_attach(mm : PocketMimi) : bool {
    let key = tts_pk_codec_key(mm)
    return ts_slab_current(g_ts_pk_codec, key) || ts_pk_codec_rebuild(mm, key)
}

[cold_path]
def private ts_pk_codec_rebuild(mm : PocketMimi; key : uint64) : bool {
    let rope_rows = TS_PK_CODEC_MAX_FRAMES * mm.frame_steps
    return ts_slab_rebuild(g_ts_pk_codec, g_ts_pk_codec_slots, key, false, "pocket codec slab") $(var w : TtsSlabWriter; var s : TtsPkCodecSlab) {
        tts_pk_codec_write(w, mm, rope_rows, s)
    }
}

//! the attach ledger under the info log: a slab's floats, its host build and its upload
[cold_path]
def private ts_attach_log(what : string; floats : int64; build_us, upload_us : int) {
    to_log(LOG_INFO, "vk tts {what}: {floats * 4l / 1048576l} MB, built in {build_us / 1000} ms, uploaded in {upload_us / 1000} ms\n")
}

//! a seat's pipeline build wall `us` and the driver's share since `drv0`, and `arms_us`, the device bring-up the call paid; the call site holds the ms test
[cold_path]
def private ts_ensure_log(seat : string; us, drv0, arms_us : int) {
    if (arms_us >= 1000) {
        to_log(LOG_INFO, "vk tts {seat}: the device up in {arms_us / 1000} ms\n")
    }
    if (us >= 1000) {
        to_log(LOG_INFO, "vk tts {seat} pipelines: built in {us / 1000} ms, the driver's {(g_vkd_pipe_build_us - drv0) / 1000} ms of it\n")
    }
}

//! the tier's device armed, then the seat's classes built (`ensure`); false on either (the seat's `device` decline); the bring-up and the pipeline
//! build wall the call paid past it logged when either reached a ms
def private ts_arm_ensure(seat : string; ensure : function<() : bool>) : bool {
    let te = ref_time_ticks()
    let drv0 = g_vkd_pipe_build_us
    return false if (!vulkan_tower_arms())
    let arms_us = get_time_usec(te)
    return false if (!invoke(ensure))
    let us = get_time_usec(te) - arms_us
    if (us >= 1000 || arms_us >= 1000) {
        ts_ensure_log(seat, us, drv0, arms_us)
    }
    return true
}

def private ts_pk_frames_attach(m : PocketModel) : bool {
    let key = tts_pk_frames_key(m)
    return ts_slab_current(g_ts_pk_frames, key) || ts_pk_frames_rebuild(m, key)
}

[cold_path]
def private ts_pk_frames_rebuild(m : PocketModel; key : uint64) : bool {
    return ts_slab_rebuild(g_ts_pk_frames, g_ts_pk_frames_slots, key, false, "pocket frames slab") $(var w : TtsSlabWriter; var s : TtsPkFramesSlab) {
        tts_pk_frames_write(w, m, s)
    }
}

//! one voice's device K/V rows [cap][d] per backbone layer, and the rope tables (cos then sin) for every position they can hold
struct private TsPkVoice {
    meta : TtsPkVoiceMeta
    k : array<TsRows>
    v : array<TsRows>
    rope : TsRows
    sin_off : int64
}

var private g_ts_pk_voice : TsPkVoice

def private ts_pk_voice_drop {
    var v & = g_ts_pk_voice
    for (r in v.k) {
        ts_rows_release(r)
    }
    for (r in v.v) {
        ts_rows_release(r)
    }
    v.k |> clear()
    v.v |> clear()
    ts_rows_release(v.rope)
    v.meta = TtsPkVoiceMeta()
}

//! host rows [r0, r1) of one cache as the device's [pos][d] rows, staged then uploaded at row r0
[arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_TTS.md#vk-pocket-chain")]
def private ts_pk_kv_upload(kc : TtsKvCache; bk, bv : TsRows; r0, r1 : int64) {
    let d = kc.heads * kc.dh
    let rows = r1 - r0
    return if (rows <= 0l)
    g_ts_pk_kv_host |> ensure_length(2l * rows * d)
    unsafe {
        tts_pk_kv_rows(kc, r0, r1, 0l, addr(g_ts_pk_kv_host[0]), addr(g_ts_pk_kv_host[rows * d]))
    }
    upload_region_at(bk.dev, r0 * d * 4l, unsafe(addr<uint8 const?>(g_ts_pk_kv_host[0])), rows * d * 4l)
    upload_region_at(bv.dev, r0 * d * 4l, unsafe(addr<uint8 const?>(g_ts_pk_kv_host[rows * d])), rows * d * 4l)
}

def private ts_pk_voice_attach(vs : PocketVoiceState; tf : PocketTransformer) : bool
    => tts_pk_voice_current(g_ts_pk_voice.meta, vs, length(g_ts_pk_voice.k)) || ts_pk_voice_rebuild(vs, tf)

[cold_path]
def private ts_pk_voice_rebuild(vs : PocketVoiceState; tf : PocketTransformer) : bool {
    var v & = g_ts_pk_voice
    ts_pk_voice_drop()
    let t0 = ref_time_ticks()
    let cap = vs.caches[0].cap
    let d = tf.d
    let half = d / tf.heads / 2l
    v.k |> reserve(length(vs.caches))
    v.v |> reserve(length(vs.caches))
    for (kc in vs.caches) {
        var bk : TsRows
        var bv : TsRows
        if (!ts_rows_alloc(bk, cap * d * 4l, TS_PK_KV_BIT) || !ts_rows_alloc(bv, cap * d * 4l, TS_PK_VV_BIT)) {
            ts_rows_release(bk)
            ts_pk_voice_drop()
            return false
        }
        v.k |> push(bk)
        v.v |> push(bv)
        ts_pk_kv_upload(kc, bk, bv, 0l, vs.len)
    }
    var inscope @exact_size tcos : array<float>
    var inscope @exact_size tsin : array<float>
    tts_pk_rope_tabs(tf, cap, tcos, tsin)
    if (!ts_rows_alloc(v.rope, 2l * cap * half * 4l, 0u)) {
        ts_pk_voice_drop()
        return false
    }
    ts_upload(v.rope, tcos, cap * half)
    upload_region_at(v.rope.dev, cap * half * 4l, unsafe(addr<uint8 const?>(tsin[0])), cap * half * 4l)
    v.sin_off = cap * half
    v.meta = tts_pk_voice_meta(vs, d)
    to_log(LOG_INFO, "vk tts pocket voice slot: {vs.len} of {cap} rows over {length(vs.caches)} layers, up in {get_time_usec(t0) / 1000} ms\n")
    return true
}

//! a codec conv in the CPU's first-window form (forward: causal pad k - stride; transposed: t * stride rows, no pad); the output row count
def private ts_pk_conv(raw : VkCommandBuffer; var h : VkHaz; slab : TsRows; slot : TtsConvSlot; cv : TtsConv1d; t : int64; x, col, y : TsRows) : int64 {
    let rows = cv.transposed ? t * cv.stride : t
    ts_conv_padded(raw, h, slab, slot, cv, t, rows, cv.transposed ? 0l : cv.k - cv.stride, x, col, y)
    return rows
}

//! a window of rows copied: `rows` x `width` from src [.][ss] at column sc, rows sr0.., into dst [.][ds] at column dc, rows dr0.., the ELU on the way where asked
def private ts_pk_rows(raw : VkCommandBuffer; var h : VkHaz; src, dst : TsRows; pa : TtsPkRowsArgs; elu : bool) {
    var pc = pa
    let total = int64(pa.rows) * int64(pa.width)
    if (elu) {
        var s = set_tts_pk_rows_elu_cls(fixed_array(src.dev, dst.dev), fixed_array(src.bytes, dst.bytes), fixed_array(src.bit, dst.bit))
        enc_tts_pk_rows_elu_cls(raw, h, s, pc, total)
    } else {
        var s = set_tts_pk_rows_cls(fixed_array(src.dev, dst.dev), fixed_array(src.bytes, dst.bytes), fixed_array(src.bit, dst.bit))
        enc_tts_pk_rows_cls(raw, h, s, pc, total)
    }
    vt_pt(raw, VtProfRole.tts_pk)
}

def private ts_pk_rows_args(rows, width : int64) : TtsPkRowsArgs
    => TtsPkRowsArgs(rows = uint(rows), width = uint(width), ss = uint(width), sc = 0u, sr0 = 0u, ds = uint(width), dc = 0u, dr0 = 0u)

//! the ELU over x's first rows x width elements in place
def private ts_pk_elu(raw : VkCommandBuffer; var h : VkHaz; x : TsRows; rows, width : int64) {
    ts_pk_rows(raw, h, x, x, ts_pk_rows_args(rows, width), true)
}

//! x [rows][c] *= the slab's scale row at `soff`
def private ts_pk_row_scale(raw : VkCommandBuffer; var h : VkHaz; slab : TsRows; x : TsRows; rows, c, soff : int64) {
    var s = set_tts_pk_row_scale_cls(fixed_array(x.dev, slab.dev), fixed_array(x.bytes, slab.bytes), fixed_array(x.bit, 0u))
    var pa = TowerBiasArgs(d = uint(c), nelem = uint(rows * c), boff = uint(soff))
    enc_tts_pk_row_scale_cls(raw, h, s, pa, rows * c)
    vt_pt(raw, VtProfRole.tts_pk)
}

//! the NORM rope over the span `d` wide at column `col` of x's rows [rows][qs], positions pos0.., on the tables in `tabs` (cos at coff, sin at soff)
def private ts_pk_rope(raw : VkCommandBuffer; var h : VkHaz; tabs : TsRows; coff, soff : int64; x : TsRows; rows, qs, col, d, dh, pos0 : int64) {
    var s = set_tts_pk_rope_cls(fixed_array(x.dev, tabs.dev, tabs.dev), fixed_array(x.bytes, tabs.bytes, tabs.bytes), fixed_array(x.bit, tabs.bit, tabs.bit))
    var pa = TtsPkRopeArgs(rows = uint(rows), qs = uint(qs), col = uint(col), d = uint(d), dh = uint(dh), pos0 = uint(pos0), xoff = 0u, coff = uint(coff), soff = uint(soff))
    enc_tts_pk_rope_cls(raw, h, s, pa, rows * d / 2l)
    vt_pt(raw, VtProfRole.tts_pk)
}

//! the causal cached attention: q from the qkv rows [t][qs] at column 0, k and v the cache rows [pos][d], o [t][d]; query i at position pos0 + i
def private ts_pk_attn(raw : VkCommandBuffer; var h : VkHaz; qkv, k, v, o : TsRows; t, qs, d, heads, pos0, ctx : int64) {
    var s = set_tts_pk_attn_cls(fixed_array(qkv.dev, k.dev, v.dev, o.dev), fixed_array(qkv.bytes, k.bytes, v.bytes, o.bytes), fixed_array(qkv.bit, k.bit, v.bit, o.bit))
    var pa = TtsPkAttnArgs(t = uint(t), qs = uint(qs), qcol = 0u, d = uint(d), pos0 = uint(pos0), ctx = uint(ctx), qoff = 0u, koff = 0u, voff = 0u, ooff = 0u)
    enc_tts_pk_attn_cls(raw, h, s, pa, tts_attn_wgs(t, heads))
    vt_pt(raw, VtProfRole.tts_pk_attn)
}

//! x += y (no bias: the slab's zero row), then xb = layernorm(x) on the norm slot's rows - a transformer layer's residual join and the norm after it
def private ts_pk_add_ln(raw : VkCommandBuffer; var h : VkHaz; slab : TsRows; zero : int64; ns : TtsNormSlot; x, y, xb : TsRows; rows, d : int64; eps : float) {
    let bslot = TtsConvSlot(boff = zero)
    ts_add_ln(raw, h, slab, bslot, ns, x, y, xb, rows, d, eps)
}

//! one GEMV site of the frame: the row in and out, and the operands a stamp's prologue and epilogue read - the modulation rows, the norm's
//! scale and shift, the slab vector, the noise row and the latent row
struct private TsPkSite {
    x : TsRows
    xoff : int64
    y : TsRows
    yoff : int64
    mod : TsRows
    moff : int64
    lwoff : int64
    lboff : int64
    aoff : int64
    x0 : TsRows
    x0off : int64
    lat : TsRows
    loff : int64
    eps : float = POCKET_HEAD_LN_EPS   //! the input norm's epsilon: the head's unless the site sits in the backbone
}

def private ts_pk_site(x : TsRows; xoff : int64; y : TsRows; yoff : int64 = 0l) : TsPkSite => TsPkSite(x = x, xoff = xoff, y = y, yoff = yoff)

let private TS_PK_GEMV_DOT = 0
let private TS_PK_GEMV_LN_SILU = 1
let private TS_PK_GEMV_GATE = 2
let private TS_PK_GEMV_ADD_SILU = 3
let private TS_PK_GEMV_TAIL = 4
let private TS_PK_GEMV_LN_GELU = 5      //! the frame loop's ffn up: the normed input, the GELU out
let private TS_PK_GEMV_ADD_SCALE = 6    //! ... and its residual joins: y += d times the slab's scale row at aoff
let private TS_PK_GEMV_ADD = 7          //! ... or y += d alone

//! the row GEMV at a site under the stamp `kind` names (a `TS_PK_GEMV_*`), over the slab's f32 rows of `p`
def private ts_pk_gemv(raw : VkCommandBuffer; var h : VkHaz; slab : TsRows; p : TtsConvSlot; nin, nout : int64; kind : int; s : TsPkSite) {
    var pa = TtsPkGemvArgs(nin = uint(nin), nout = uint(nout), stride = uint(p.kdim_p), eps = s.eps, woff = uint(p.woff), boff = uint(p.boff),
        xoff = uint(s.xoff), yoff = uint(s.yoff), moff = uint(s.moff), lwoff = uint(s.lwoff), lboff = uint(s.lboff), aoff = uint(s.aoff),
        x0off = uint(s.x0off), loff = uint(s.loff))
    if (kind == TS_PK_GEMV_DOT) {
        var d0 = set_tts_pk_gemv_dot_cls(fixed_array(slab.dev, s.x.dev, s.y.dev), fixed_array(slab.bytes, s.x.bytes, s.y.bytes), fixed_array(0u, s.x.bit, s.y.bit))
        enc_tts_pk_gemv_dot_cls(raw, h, d0, pa, nout)
    } elif (kind == TS_PK_GEMV_LN_SILU) {
        var d1 = set_tts_pk_gemv_ln_silu_cls(fixed_array(slab.dev, s.x.dev, s.y.dev, s.mod.dev), fixed_array(slab.bytes, s.x.bytes, s.y.bytes, s.mod.bytes),
            fixed_array(0u, s.x.bit, s.y.bit, s.mod.bit))
        enc_tts_pk_gemv_ln_silu_cls(raw, h, d1, pa, nout)
    } elif (kind == TS_PK_GEMV_GATE) {
        var d2 = set_tts_pk_gemv_gate_cls(fixed_array(slab.dev, s.x.dev, s.y.dev, s.mod.dev), fixed_array(slab.bytes, s.x.bytes, s.y.bytes, s.mod.bytes),
            fixed_array(0u, s.x.bit, s.y.bit, s.mod.bit))
        enc_tts_pk_gemv_gate_cls(raw, h, d2, pa, nout)
    } elif (kind == TS_PK_GEMV_ADD_SILU) {
        var d3 = set_tts_pk_gemv_add_silu_cls(fixed_array(slab.dev, s.x.dev, s.y.dev), fixed_array(slab.bytes, s.x.bytes, s.y.bytes), fixed_array(0u, s.x.bit, s.y.bit))
        enc_tts_pk_gemv_add_silu_cls(raw, h, d3, pa, nout)
    } elif (kind == TS_PK_GEMV_LN_GELU) {
        var d5 = set_tts_pk_gemv_ln_gelu_cls(fixed_array(slab.dev, s.x.dev, s.y.dev), fixed_array(slab.bytes, s.x.bytes, s.y.bytes), fixed_array(0u, s.x.bit, s.y.bit))
        enc_tts_pk_gemv_ln_gelu_cls(raw, h, d5, pa, nout)
    } elif (kind == TS_PK_GEMV_ADD_SCALE) {
        var d6 = set_tts_pk_gemv_add_scale_cls(fixed_array(slab.dev, s.x.dev, s.y.dev), fixed_array(slab.bytes, s.x.bytes, s.y.bytes), fixed_array(0u, s.x.bit, s.y.bit))
        enc_tts_pk_gemv_add_scale_cls(raw, h, d6, pa, nout)
    } elif (kind == TS_PK_GEMV_ADD) {
        var d7 = set_tts_pk_gemv_add_cls(fixed_array(slab.dev, s.x.dev, s.y.dev), fixed_array(slab.bytes, s.x.bytes, s.y.bytes), fixed_array(0u, s.x.bit, s.y.bit))
        enc_tts_pk_gemv_add_cls(raw, h, d7, pa, nout)
    } else {
        var d4 = set_tts_pk_gemv_tail_cls(fixed_array(slab.dev, s.x.dev, s.y.dev, s.mod.dev, s.x0.dev, s.lat.dev),
            fixed_array(slab.bytes, s.x.bytes, s.y.bytes, s.mod.bytes, s.x0.bytes, s.lat.bytes), fixed_array(0u, s.x.bit, s.y.bit, s.mod.bit, s.x0.bit, s.lat.bit))
        enc_tts_pk_gemv_tail_cls(raw, h, d4, pa, nout)
    }
    vt_pt(raw, ts_pk_gemv_role(kind))
}

//! the ledger role of a row GEMV stamp
def private ts_pk_gemv_role(kind : int) : VtProfRole {
    if (kind == TS_PK_GEMV_DOT) return VtProfRole.tts_pk_dot
    if (kind == TS_PK_GEMV_LN_GELU) return VtProfRole.tts_pk_ffn
    if (kind == TS_PK_GEMV_ADD_SCALE || kind == TS_PK_GEMV_ADD) return VtProfRole.tts_pk_res
    return VtProfRole.tts_pk_head
}

def private ts_pk_lin(raw : VkCommandBuffer; var h : VkHaz; slab : TsRows; p : TtsConvSlot; nin, nout : int64; x : TsRows; xoff : int64; y : TsRows; yoff : int64) {
    ts_pk_gemv(raw, h, slab, p, nin, nout, TS_PK_GEMV_DOT, ts_pk_site(x, xoff, y, yoff))
}

let private TS_C_LAT = 0     //! the latent frames [frames][cin]
let private TS_C_Q = 1       //! ... projected [frames][d]
let private TS_C_X = 2       //! the transformer's residual rows [t1][d]
let private TS_C_XB = 3
let private TS_C_QKV = 4
let private TS_C_CTX = 5
let private TS_C_H = 6
let private TS_C_K = 7       //! the layer's keys and values as rows [t1][d], one set every layer reuses
let private TS_C_V = 8
let private TS_C_A = 9       //! the stages' rows: the stream pinging between A and B, the upsample, the block's two temporaries
let private TS_C_B = 10
let private TS_C_UP = 11
let private TS_C_CC = 12
let private TS_C_CP = 13
let private TS_C_O = 14      //! dec_out's rows [samples][cout]
let private TS_C_WAVE = 15
let private TS_C_COL = 16

def private ts_pk_codec_ensure : bool {
    return (ensure_f32_gemm_b_cls() && ensure_tower_ln_cls() && ensure_tower_post_add_ln_cls() && ensure_tts_im2col_cls() && ensure_tts_pool_dw_cls()
        && ensure_tts_gelu_tanh_cls() && ensure_tts_add_scale_cls() && ensure_tts_pk_rows_cls() && ensure_tts_pk_rows_elu_cls()
        && ensure_tts_pk_row_scale_cls() && ensure_tts_pk_attn_cls() && ensure_tts_pk_rope_cls())
}

//! the codec seat's slots: the transformer's rows at t1, the stages' at their widest rows x width, the im2col at the widest row
def private ts_pk_codec_floats(mm : PocketMimi; frames : int64) : TsFloats {
    let d = mm.dec_tf.d
    let t1 = g_ts_pk_rows[1]
    var fl : TsFloats
    fl[TS_C_LAT] = frames * mm.quant_proj.cin
    fl[TS_C_Q] = frames * d
    fl[TS_C_X] = t1 * d
    fl[TS_C_XB] = t1 * d
    fl[TS_C_QKV] = t1 * 3l * d
    fl[TS_C_CTX] = t1 * d
    fl[TS_C_H] = t1 * mm.dec_tf.ffn
    fl[TS_C_K] = t1 * d
    fl[TS_C_V] = t1 * d
    var stage = t1 * mm.dec_in.cout
    var col = max(frames * ts_conv_kdim(mm.quant_proj), t1 * ts_conv_kdim(mm.dec_in))
    for (i in range64(long_length(mm.ratios))) {
        let tn = g_ts_pk_rows[2l + i]
        let up & = unsafe(mm.dec_up[i])
        let rb & = unsafe(mm.dec_res[i])
        stage = max(stage, tn * max(up.cout, rb.conv1.cout))
        col = max(col, tn * max(ts_conv_kdim(up), max(ts_conv_kdim(rb.conv1), ts_conv_kdim(rb.conv2))))
    }
    let samples = g_ts_pk_rows[long_length(g_ts_pk_rows) - 1l]
    for (i in range(TS_C_A, TS_C_CP + 1)) {
        fl[i] = stage
    }
    fl[TS_C_O] = samples * mm.dec_out.cout
    fl[TS_C_WAVE] = samples
    fl[TS_C_COL] = max(col, samples * ts_conv_kdim(mm.dec_out))
    return fl
}

//! the codec transformer over the residual rows X [t][d] at positions 0..: the CPU's layer loop, dispatch for dispatch
def private ts_pk_transformer(raw : VkCommandBuffer; var h : VkHaz; slab : TsRows; sc_ : TsScratch; tf : PocketTransformer; sl : TtsPkCodecSlab; t : int64) {
    let d = tf.d
    let dh = d / tf.heads
    let nd = t * d
    let r_x = ts_rows(sc_, TS_C_X)
    let r_xb = ts_rows(sc_, TS_C_XB)
    let r_qkv = ts_rows(sc_, TS_C_QKV)
    let r_ctx = ts_rows(sc_, TS_C_CTX)
    let r_h = ts_rows(sc_, TS_C_H)
    let r_k = ts_rows(sc_, TS_C_K)
    let r_v = ts_rows(sc_, TS_C_V)
    var kc = TtsPkRowsArgs(rows = uint(t), width = uint(d), ss = uint(3l * d), sc = uint(d), sr0 = 0u, ds = uint(d), dc = 0u, dr0 = 0u)
    var pa_gelu = TtsElemArgs(nelem = uint(t * tf.ffn), off = 0u, slope = 0.0)
    var s_gelu = set_tts_gelu_tanh_cls(fixed_array(r_h.dev), fixed_array(r_h.bytes), fixed_array(r_h.bit))
    for (i in range(length(tf.layers))) {
        let s & = unsafe(sl.layers[i])
        ts_ln(raw, h, slab, s.n1, r_x, r_xb, t, d, tf.eps)
        ts_lin(raw, h, slab, s.in_proj, d, 3l * d, r_xb, r_qkv, t)
        ts_pk_rope(raw, h, slab, sl.rope_cos, sl.rope_sin, r_qkv, t, 3l * d, 0l, d, dh, 0l)
        ts_pk_rope(raw, h, slab, sl.rope_cos, sl.rope_sin, r_qkv, t, 3l * d, d, d, dh, 0l)
        kc.sc = uint(d)
        ts_pk_rows(raw, h, r_qkv, r_k, kc, false)
        kc.sc = uint(2l * d)
        ts_pk_rows(raw, h, r_qkv, r_v, kc, false)
        ts_pk_attn(raw, h, r_qkv, r_k, r_v, r_ctx, t, 3l * d, d, tf.heads, 0l, tf.context)
        ts_lin(raw, h, slab, s.out_proj, d, d, r_ctx, r_xb, t)
        if (s.has_s1) {
            ts_pk_row_scale(raw, h, slab, r_xb, t, d, s.scale1_off)
        }
        ts_pk_add_ln(raw, h, slab, sl.zero, s.n2, r_x, r_xb, r_xb, t, d, tf.eps)
        ts_lin(raw, h, slab, s.ffn1, d, tf.ffn, r_xb, r_h, t)
        enc_tts_gelu_tanh_cls(raw, h, s_gelu, pa_gelu, t * tf.ffn)
        vt_pt(raw, VtProfRole.tts_act)
        ts_lin(raw, h, slab, s.ffn2, tf.ffn, d, r_h, r_xb, t)
        if (s.has_s2) {
            ts_pk_row_scale(raw, h, slab, r_xb, t, d, s.scale2_off)
        }
        ts_add_scale(raw, h, r_x, r_xb, r_x, nd, 1.0)
    }
}

//! one ELU-conv-ELU-conv residual block over x [t][w] (a stage's rows) into y, through the block's two temporaries
def private ts_pk_res_block(raw : VkCommandBuffer; var h : VkHaz; slab : TsRows; sc_ : TsScratch; b : PocketResConv; s1, s2 : TtsConvSlot; x, y : TsRows; t, w : int64) {
    let r_cc = ts_rows(sc_, TS_C_CC)
    let r_cp = ts_rows(sc_, TS_C_CP)
    let r_col = ts_rows(sc_, TS_C_COL)
    ts_pk_rows(raw, h, x, r_cc, ts_pk_rows_args(t, w), true)
    ts_pk_conv(raw, h, slab, s1, b.conv1, t, r_cc, r_col, r_cp)
    ts_pk_elu(raw, h, r_cp, t, b.conv1.cout)
    ts_pk_conv(raw, h, slab, s2, b.conv2, t, r_cp, r_col, y)
    ts_add_scale(raw, h, y, x, y, t * w, 1.0)
}

//! the codec decoder: the latent projection, the depthwise upsample, the transformer, dec_in, each stage's upsample and residual block, dec_out, the sample column out
def private ts_pk_codec_chain(raw : VkCommandBuffer; var h : VkHaz; slab : TsRows; sc_ : TsScratch; mm : PocketMimi; sl : TtsPkCodecSlab) {
    let d = mm.dec_tf.d
    let frames = g_ts_pk_rows[0]
    let t1 = g_ts_pk_rows[1]
    let r_col = ts_rows(sc_, TS_C_COL)
    let r_q = ts_rows(sc_, TS_C_Q)
    let r_x = ts_rows(sc_, TS_C_X)
    ts_pk_conv(raw, h, slab, sl.quant, mm.quant_proj, frames, ts_rows(sc_, TS_C_LAT), r_col, r_q)
    var sp = set_tts_pool_dw_cls(fixed_array(r_q.dev, r_x.dev, slab.dev), fixed_array(r_q.bytes, r_x.bytes, slab.bytes), fixed_array(r_q.bit, r_x.bit, 0u))
    var pp = TtsPoolArgs(c = uint(d), k = uint(mm.upsample.k), stride = uint(mm.upsample.stride), pad_l = 0u, dil = uint(mm.upsample.dilation), t_in = uint(frames),
        t_out = uint(t1), woff = uint(sl.up_w), boff = uint(sl.up_b))
    enc_tts_pool_dw_cls(raw, h, sp, pp, t1 * d)
    vt_pt(raw, VtProfRole.tts_rows)
    ts_pk_transformer(raw, h, slab, sc_, mm.dec_tf, sl, t1)
    var cur = ts_rows(sc_, TS_C_A)
    ts_pk_conv(raw, h, slab, sl.dec_in, mm.dec_in, t1, r_x, r_col, cur)
    var t = t1
    var w = mm.dec_in.cout
    let r_up = ts_rows(sc_, TS_C_UP)
    for (i in range64(long_length(mm.ratios))) {
        ts_pk_elu(raw, h, cur, t, w)
        let up & = unsafe(mm.dec_up[i])
        let tn = ts_pk_conv(raw, h, slab, sl.ups[i], up, t, cur, r_col, r_up)
        let wn = up.cout
        let next = ts_rows(sc_, i % 2l == 0l ? TS_C_B : TS_C_A)
        ts_pk_res_block(raw, h, slab, sc_, mm.dec_res[i], sl.res1[i], sl.res2[i], r_up, next, tn, wn)
        cur = next
        t = tn
        w = wn
    }
    ts_pk_elu(raw, h, cur, t, w)
    let r_o = ts_rows(sc_, TS_C_O)
    ts_pk_conv(raw, h, slab, sl.dec_out, mm.dec_out, t, cur, r_col, r_o)
    let kw = TtsPkRowsArgs(rows = uint(t), width = 1u, ss = uint(mm.dec_out.cout), sc = 0u, sr0 = 0u, ds = 1u, dc = 0u, dr0 = 0u)
    ts_pk_rows(raw, h, r_o, ts_rows(sc_, TS_C_WAVE), kw, false)
}

[hot_path, unused_argument(sc), arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_TTS.md#vk-pocket-chain")]
def private vulkan_pocket_codec(m : PocketModel; latents : array<float>; frames : int64; var sc : PocketScratch; @scratch @exact_size var wave : array<float>) : bool {
    if (!vulkan_tower_on()) {
        return ts_decline("codec", VulkanTtsDecline.knob)
    }
    let mm & = unsafe(m.mimi)
    if (frames < 1l || frames > TS_PK_CODEC_MAX_FRAMES || !tts_pk_codec_ok(mm, TS_PK_ATTN_HS, TS_PK_ATTN_MAX_KEYS) || m.frame_samples != mm.frame_steps * mm.hop
        || long_length(latents) < frames * mm.quant_proj.cin) {
        return ts_decline("codec", VulkanTtsDecline.shape)
    }
    if (!ts_arm_ensure("pocket codec", @@ts_pk_codec_ensure)) {
        return ts_decline("codec", VulkanTtsDecline.device)
    }
    tts_pk_stage_rows(mm, frames, g_ts_pk_rows)
    let samples = g_ts_pk_rows[long_length(g_ts_pk_rows) - 1l]
    if (!ts_pk_codec_attach(mm)) {
        return ts_decline("codec", VulkanTtsDecline.memory)
    }
    let sl & = g_ts_pk_codec_slots
    let fl = ts_pk_codec_floats(mm, frames)
    if (!ts_scratch_ready(g_ts_pkc_scratch, fl, 0l, 0l, samples)) {
        return ts_decline("codec", VulkanTtsDecline.memory)
    }
    let sc_ & = g_ts_pkc_scratch
    let slab = g_ts_pk_codec.rows
    let r_lat = ts_rows(sc_, TS_C_LAT)
    let r_wave = ts_rows(sc_, TS_C_WAVE)
    ts_upload(r_lat, latents, frames * mm.quant_proj.cin)
    vt_chain_one_shot("pocket codec", long_length(mm.dec_tf.layers), g_ts_pk_rows[1]) $(raw : VkCommandBuffer; var h : VkHaz) {
        ts_pk_codec_chain(raw, h, slab, sc_, mm, sl)
    }
    ts_readback(r_wave, sc_, wave, samples)
    g_ts_encodes++
    return true
}

let private TS_F_X = 0        //! the backbone's residual row [d]
let private TS_F_XB = 1
let private TS_F_QKV = 2
let private TS_F_CTX = 3
let private TS_F_H = 4        //! the ffn's row
let private TS_F_HY = 5       //! the head's conditioning
let private TS_F_HX = 6       //! ... its running state
let private TS_F_HM = 7
let private TS_F_HMOD = 8     //! the modulation rows [3 n]
let private TS_F_X1 = 9       //! the frame's latent, the next frame's input
let private TS_F_NOISE = 10   //! the batch's noise rows [batch][ld]
let private TS_F_FORCED = 11  //! the forced frames [forced_n][ld]
let private TS_F_LATENTS = 12 //! every frame's latent row [max_frames][ld]
let private TS_F_CONDS = 13   //! ... and its conditioning row [max_frames][d]
let private TS_F_EOS = 14     //! ... and its EOS logit

def private ts_pk_frames_ensure : bool {
    return (ensure_tower_ln_cls() && ensure_tts_pk_rows_cls() && ensure_tts_pk_attn_cls() && ensure_tts_pk_gemv_dot_cls() && ensure_tts_pk_gemv_ln_silu_cls()
        && ensure_tts_pk_gemv_gate_cls() && ensure_tts_pk_gemv_add_silu_cls() && ensure_tts_pk_gemv_tail_cls() && ensure_tts_pk_gemv_ln_qkv_cls()
        && ensure_tts_pk_gemv_add_scale_cls() && ensure_tts_pk_gemv_add_cls() && ensure_tts_pk_gemv_ln_gelu_cls())
}

def private ts_pk_frames_floats(m : PocketModel; max_frames, batch, forced_n : int64) : TsFloats {
    let d = m.backbone.d
    let n = m.head.dim
    let ld = m.latent_dim
    var fl : TsFloats
    fl[TS_F_X] = d
    fl[TS_F_XB] = d
    fl[TS_F_QKV] = 3l * d
    fl[TS_F_CTX] = d
    fl[TS_F_H] = m.backbone.ffn
    fl[TS_F_HY] = n
    fl[TS_F_HX] = n
    fl[TS_F_HM] = n
    fl[TS_F_HMOD] = 3l * n
    fl[TS_F_X1] = ld
    fl[TS_F_NOISE] = batch * ld
    fl[TS_F_FORCED] = max(forced_n, 1l) * ld
    fl[TS_F_LATENTS] = max_frames * ld
    fl[TS_F_CONDS] = max_frames * d
    fl[TS_F_EOS] = max_frames
    return fl
}

//! the residual join of a frame's projection: x += W . x_in + b, times the layer scale where the layer carries one
def private ts_pk_gemv_add(raw : VkCommandBuffer; var h : VkHaz; slab : TsRows; p : TtsConvSlot; nin, nout : int64; x_in : TsRows; x : TsRows; scaled : bool; soff : int64) {
    var s = ts_pk_site(x_in, 0l, x)
    s.aoff = soff
    ts_pk_gemv(raw, h, slab, p, nin, nout, scaled ? TS_PK_GEMV_ADD_SCALE : TS_PK_GEMV_ADD, s)
}

//! one backbone layer at position p: the CPU's `layer_attention` and `layer_ffn` at t = 1 in five dispatches, x the residual in and out
def private ts_pk_fr_layer(raw : VkCommandBuffer; var h : VkHaz; slab : TsRows; sc_ : TsScratch; tf : PocketTransformer; L : TtsPkLayerSlots; li : int; p : int64) {
    let d = tf.d
    let dh = uint(d / tf.heads)
    let r_x = ts_rows(sc_, TS_F_X)
    let r_qkv = ts_rows(sc_, TS_F_QKV)
    let r_ctx = ts_rows(sc_, TS_F_CTX)
    let r_h = ts_rows(sc_, TS_F_H)
    let v & = g_ts_pk_voice
    var sq = set_tts_pk_gemv_ln_qkv_cls(fixed_array(slab.dev, r_x.dev, r_qkv.dev, v.rope.dev, v.k[li].dev, v.v[li].dev),
        fixed_array(slab.bytes, r_x.bytes, r_qkv.bytes, v.rope.bytes, v.k[li].bytes, v.v[li].bytes),
        fixed_array(0u, r_x.bit, r_qkv.bit, v.rope.bit, v.k[li].bit, v.v[li].bit))
    var pq = TtsPkGemvArgs(nin = uint(d), nout = uint(3l * d), stride = uint(L.in_proj.kdim_p), eps = tf.eps, woff = uint(L.in_proj.woff),
        boff = uint(L.in_proj.boff), xoff = 0u, yoff = 0u, moff = 0u, lwoff = uint(L.n1.gwoff), lboff = uint(L.n1.gboff), aoff = 0u, x0off = 0u, loff = 0u,
        span = uint(d), dh = dh, p = uint(p), coff = 0u, soff = uint(v.sin_off))
    enc_tts_pk_gemv_ln_qkv_cls(raw, h, sq, pq, 3l * d)
    vt_pt(raw, VtProfRole.tts_pk_qkv)
    ts_pk_attn(raw, h, r_qkv, v.k[li], v.v[li], r_ctx, 1l, 3l * d, d, tf.heads, p, tf.context)
    ts_pk_gemv_add(raw, h, slab, L.out_proj, d, d, r_ctx, r_x, L.has_s1, L.scale1_off)
    var s1 = ts_pk_site(r_x, 0l, r_h)
    s1.lwoff = L.n2.gwoff
    s1.lboff = L.n2.gboff
    s1.eps = tf.eps
    ts_pk_gemv(raw, h, slab, L.ffn1, d, tf.ffn, TS_PK_GEMV_LN_GELU, s1)
    ts_pk_gemv_add(raw, h, slab, L.ffn2, tf.ffn, d, r_h, r_x, L.has_s2, L.scale2_off)
}

//! frame `f` at cache position p, noise row `i` of the batch: the CPU's `frame_step` and `head_step`, the conditioning row and EOS logit kept
def private ts_pk_frame(raw : VkCommandBuffer; var h : VkHaz; slab : TsRows; sc_ : TsScratch; m : PocketModel; sl : TtsPkFramesSlab; p, f, i, forced_rows : int64) {
    let d = m.backbone.d
    let ld = m.latent_dim
    let n = m.head.dim
    let r_x = ts_rows(sc_, TS_F_X)
    let r_xb = ts_rows(sc_, TS_F_XB)
    let r_hy = ts_rows(sc_, TS_F_HY)
    let r_hx = ts_rows(sc_, TS_F_HX)
    let r_hm = ts_rows(sc_, TS_F_HM)
    let r_hmod = ts_rows(sc_, TS_F_HMOD)
    let r_x1 = ts_rows(sc_, TS_F_X1)
    let r_noise = ts_rows(sc_, TS_F_NOISE)
    let r_conds = ts_rows(sc_, TS_F_CONDS)
    let r_lat = ts_rows(sc_, TS_F_LATENTS)
    var xin = slab
    var xoff = sl.bos
    if (f > 0l) {
        if (f <= forced_rows) {
            xin = ts_rows(sc_, TS_F_FORCED)
            xoff = (f - 1l) * ld
        } else {
            xin = r_x1
            xoff = 0l
        }
    }
    ts_pk_lin(raw, h, slab, sl.input_linear, ld, d, xin, xoff, r_x, 0l)
    for (li in range(length(sl.layers))) {
        ts_pk_fr_layer(raw, h, slab, sc_, m.backbone, sl.layers[li], li, p)
    }
    ts_ln(raw, h, slab, sl.out_norm, r_x, r_xb, 1l, d, m.backbone.eps)
    let coff = f * d
    let kc = TtsPkRowsArgs(rows = 1u, width = uint(d), ss = uint(d), sc = 0u, sr0 = 0u, ds = uint(d), dc = 0u, dr0 = uint(f))
    ts_pk_rows(raw, h, r_xb, r_conds, kc, false)
    ts_pk_lin(raw, h, slab, sl.out_eos, d, 1l, r_conds, coff, ts_rows(sc_, TS_F_EOS), f)
    let noff = i * ld
    var s = ts_pk_site(r_conds, coff, r_hy)
    s.aoff = sl.time_const
    ts_pk_gemv(raw, h, slab, sl.cond_embed, d, n, TS_PK_GEMV_ADD_SILU, s)
    ts_pk_gemv(raw, h, slab, sl.input_proj, ld, n, TS_PK_GEMV_DOT, ts_pk_site(r_noise, noff, r_hx))
    for (blk in sl.blocks) {
        ts_pk_gemv(raw, h, slab, blk.adaln, n, 3l * n, TS_PK_GEMV_DOT, ts_pk_site(r_hy, 0l, r_hmod))
        var s1 = ts_pk_site(r_hx, 0l, r_hm)
        s1.mod = r_hmod
        s1.lwoff = blk.ln.gwoff
        s1.lboff = blk.ln.gboff
        ts_pk_gemv(raw, h, slab, blk.mlp1, n, n, TS_PK_GEMV_LN_SILU, s1)
        var s2 = ts_pk_site(r_hm, 0l, r_hx)
        s2.mod = r_hmod
        ts_pk_gemv(raw, h, slab, blk.mlp2, n, n, TS_PK_GEMV_GATE, s2)
    }
    ts_pk_gemv(raw, h, slab, sl.final_adaln, n, 2l * n, TS_PK_GEMV_DOT, ts_pk_site(r_hy, 0l, r_hmod))
    var st = ts_pk_site(r_hx, 0l, r_x1)
    st.mod = r_hmod
    st.lwoff = sl.final_ln.gwoff
    st.lboff = sl.final_ln.gboff
    st.aoff = sl.std_mean
    st.x0 = r_noise
    st.x0off = noff
    st.lat = r_lat
    st.loff = f * ld
    ts_pk_gemv(raw, h, slab, sl.final_linear, n, ld, TS_PK_GEMV_TAIL, st)
}

var private @scratch g_ts_pk_noise_host : array<float>
var private @scratch g_ts_pk_eos_host : array<float>

//! the host frame loop (`pocket_frames_batched`) on this driver: the noise staged host-side and uploaded ahead of each batch's submit, the
//! batch's EOS logits read back after it
def private ts_pk_frames_loop(m : PocketModel; sl : TtsPkFramesSlab; slab : TsRows; sc_ : TsScratch; base, batch, max_frames, forced_rows : int64; std : float;
                              frames_after_eos : int; captured : array<float>; var sc : PocketScratch; var f0 : int64&) : tuple<frames : int64; eos_at : int64> {
    let ld = m.latent_dim
    let r_noise = ts_rows(sc_, TS_F_NOISE)
    let r_eos = ts_rows(sc_, TS_F_EOS)
    g_ts_pk_noise_host |> ensure_length(batch * ld)
    g_ts_pk_eos_host |> ensure_length(batch)
    return pocket_frames_batched(m, batch, max_frames, std, frames_after_eos, captured, sc, f0, $(_k : int64) : float? {
        return unsafe(addr(g_ts_pk_noise_host[0]))
    }, $(fb, k : int64) : bool {
        ts_upload(r_noise, g_ts_pk_noise_host, k * ld)
        vt_chain_one_shot("pocket frames", long_length(m.backbone.layers), k) $(raw : VkCommandBuffer; var h : VkHaz) {
            for (i in range64(k)) {
                ts_pk_frame(raw, h, slab, sc_, m, sl, base + fb + i, fb + i, i, forced_rows)
            }
        }
        g_ts_encodes++
        var ep = unsafe(addr(g_ts_pk_eos_host[0]))
        readback_range(r_eos.dev, r_eos.bit, fb * 4l, sc_.hb, ep, k * 4l)
        return true
    }, $(_fb, i : int64) : float {
        return g_ts_pk_eos_host[i]
    })
}

[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_TTS.md#vk-pocket-chain")]
def private vulkan_pocket_frames(m : PocketModel; var vs : PocketVoiceState; n_txt : int64; std : float; frames_after_eos : int;
                                 captured, forced : array<float>; max_frames : int64; var sc : PocketScratch) : tuple<frames : int64; eos_at : int64> {
    let none = (frames = -1l, eos_at = -1l)
    if (!vulkan_tower_on()) {
        ts_decline("frames", VulkanTtsDecline.knob)
        return none
    }
    let tf & = unsafe(m.backbone)
    let d = tf.d
    let ld = m.latent_dim
    let base = vs.len + n_txt
    let shape_ok = tts_pk_frames_ok(m, TS_PK_ATTN_HS, TS_PK_GEMV_MAX_NIN) $(l : TtsLinear) : bool {
        return tts_gemm_lin_ok(l)
    }
    if (!tts_pk_frames_admit(m, vs, n_txt, max_frames, TS_PK_ATTN_MAX_KEYS) || d % 4l != 0l || !shape_ok) {
        ts_decline("frames", VulkanTtsDecline.shape)
        return none
    }
    if (!ts_arm_ensure("pocket frames", @@ts_pk_frames_ensure)) {
        ts_decline("frames", VulkanTtsDecline.device)
        return none
    }
    let batch = min(pocket_frame_batch(), max_frames)
    let forced_rows = long_length(forced) / ld
    let fl = ts_pk_frames_floats(m, max_frames, batch, forced_rows)
    if (!ts_pk_frames_attach(m) || !ts_pk_voice_attach(vs, tf) || !ts_scratch_ready(g_ts_pkf_scratch, fl, 0l, 0l, max((max_frames + 1l) * d, max_frames * ld))) {
        ts_decline("frames", VulkanTtsDecline.memory)
        return none
    }
    for (li, kc in iter_range(vs.caches), vs.caches) {
        ts_pk_kv_upload(kc, g_ts_pk_voice.k[li], g_ts_pk_voice.v[li], vs.len, base)
    }
    let sl & = g_ts_pk_frames_slots
    let sc_ & = g_ts_pkf_scratch
    let slab = g_ts_pk_frames.rows
    if (forced_rows > 0l) {
        ts_upload(ts_rows(sc_, TS_F_FORCED), forced, forced_rows * ld)
    }
    var f0 = 0l
    let r = ts_pk_frames_loop(m, sl, slab, sc_, base, batch, max_frames, forced_rows, std, frames_after_eos, captured, sc, f0)
    if (r.frames >= 0l) {
        ts_readback(ts_rows(sc_, TS_F_LATENTS), sc_, sc.latents, r.frames * ld)
        ts_readback(ts_rows(sc_, TS_F_CONDS), sc_, sc.conds, tts_pk_cond_rows(r.frames, f0) * d)
    }
    return r
}

[init]
def dasllama_vulkan_tts_register {
    register_vk_drop_hook(@@ts_forget)
    register_weights_epoch_listener(@@ts_forget)   //! the slab keys are weight addresses: a reload landing at the same addresses must not find the old slab
    static_if (!typeinfo builtin_module_exists(das_metal)) {
        register_styletts2_gpu(St2GpuDriver(albert = @@vulkan_styletts2_albert, text = @@vulkan_styletts2_text, dur_enc = @@vulkan_styletts2_dur_enc,
            durations = @@vulkan_styletts2_durations, prosody = @@vulkan_styletts2_prosody, decode = @@vulkan_styletts2_decode,
            gen = @@vulkan_styletts2_generator))
        register_pocket_gpu(PocketGpuDriver(codec = @@vulkan_pocket_codec, frames = @@vulkan_pocket_frames))
    }
}
