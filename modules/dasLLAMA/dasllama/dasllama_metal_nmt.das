options gen2
options indenting = 4
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_metal_nmt shared public

require dasllama/dasllama_lint public
require math
require dasllama/dasllama_common
require dasllama/dasllama_math   // the active repack layout - the q8 planes carry the CPU backend's
require dasllama/dasllama_layout   // q8_region_append - the 34-byte-block upload transform
require dasllama/dasllama_gemm_schema   // Q8_BLOCK_ELEMS, Q8_QPB, Q8_SPB - the Q8_0 block's elements and bytes
require dasllama/dasllama_nmt
require metal/msl_shader
require metal/das_metal_boost
require daslib/typemacro_boost
require dasllama/dasllama_metal_common
require dasllama/dasllama_metal_lens   // release_handles
require dasllama/dasllama_gpu_math   // the row stamps' activation codes
require dasllama/dasllama_gpu_tier   // gpu_said_once - the knob's announce
require dasllama/dasllama_gpu_kernels_common
require dasllama/dasllama_metal_kernels
require dasllama/dasllama_metal_prefill   // pf_enc_q8_mm, tw_half - the borrowed GEMM ladder

//! The step's embed rows on the template both homes stamp (`GkNmtEmbed`)
[metal_dispatch(name = "enc_nmt_embed", pso = "g_nm_pso_embed", tg = 64, grid = "total/64", params = "total : int64")]
class MetalNmtEmbed : GkNmtEmbed {
    [metal_kernel(name = "metal_nmt_embed_msl")]
    def metal_nmt_embed {
        nmt_embed_body()
    }
}

//! The SSRU cell on the template both homes stamp (`GkNmtSsru`)
[metal_dispatch(name = "enc_nmt_ssru", pso = "g_nm_pso_ssru", tg = 64, grid = "total/64", params = "total : int64")]
class MetalNmtSsru : GkNmtSsru {
    [metal_kernel(name = "metal_nmt_ssru_msl")]
    def metal_nmt_ssru {
        nmt_ssru_body()
    }
}

//! The greedy pick on the template both homes stamp (`GkNmtPick`): one threadgroup of 256
[metal_dispatch(name = "enc_nmt_pick", pso = "g_nm_pso_pick", tgmem = "metal_nmt_pick_msl_tgmem", tg = 256, grid = "1")]
class MetalNmtPick : GkNmtPick {
    [metal_kernel(name = "metal_nmt_pick_msl")]
    def metal_nmt_pick {
        nmt_pick_body()
    }
}

var private g_nm_pso_embed : MetalComputePipeline?
var private g_nm_pso_ssru : MetalComputePipeline?
var private g_nm_pso_pick : MetalComputePipeline?
var private g_nm_tried = false
var private g_nm_env = true
var private g_nm_pool : MetalBufferPool
var private g_nm_declines : DeclineCounter
var private g_nm_sentences = 0l
var private g_nm_steps = 0l
var private g_nm_cbs = 0l
var private g_nm_tokens = 0l
var private g_nm_us_gpu = 0l
var private g_nm_us_encode = 0l

// the attached model, keyed on its q8 plane (base, length); a key mismatch re-attaches
var private g_nm_key = 0ul   //! the attached model's generation (`NmtModel.gen`); 0 with none attached
var private g_nm_mblob : MetalBuffer?
var private g_nm_nvpad = 0l
// driver-chosen byte offsets into the blob, per layer: the encoder's [q | k | v], o, w1, w2; the decoder's ssru [W | Wf], q,
// [k | v], o, w1, w2; then the classifier
var private @exact_size g_nm_enc_boffs : array<uint64>
var private @exact_size g_nm_dec_boffs : array<uint64>
var private g_nm_cls_boff = 0ul
//! a Q8_0 block's bytes on the device: the quants and the f16 scale
let private NM_Q8_BLOCK_BYTES = Q8_QPB + Q8_SPB
let private NM_ENC_MATS = 4l
let private NM_DEC_MATS = 6l

enum private MetalNmtDecline {
    knob         //! set_metal_nmt(false)
    quant_mode   //! the exact lane: the driver serves the q8 planes
    shape        //! a geometry `nmt_device_shape_ok` refuses
    length       //! a source longer than `NMT_DEVICE_SRC_CAP` tokens
    device       //! no device, a pipeline failed to compile, or a buffer failed
    gpu_error
}

var private g_nm_said : table<string>   //! the knob's announce already made

[cold_path]
def private nm_decline(why : MetalNmtDecline) : bool {
    if (why == MetalNmtDecline.knob) {
        gpu_said_once(g_nm_said, LOG_INFO, "metal nmt: ", "set_metal_nmt(false) - the translation family runs the CPU chain")
    }
    note_decline(g_nm_declines, "nmt", "{why}", policy = why == MetalNmtDecline.knob || why == MetalNmtDecline.quant_mode)
    return false
}

//! Serve the translation family's sentences on the device (default on).
def set_metal_nmt(on : bool) {
    g_nm_env = on
}

def metal_nmt_knob() : bool => g_nm_env

//! `sentences` served whole on the device, the greedy `steps` they ran (a command buffer's tail past a sentence's end
//! included), the command buffers, the tokens they landed.
def metal_nmt_stats : tuple<sentences : int64; steps : int64; cbs : int64; tokens : int64; declines : int64>
    => (sentences = g_nm_sentences, steps = g_nm_steps, cbs = g_nm_cbs, tokens = g_nm_tokens, declines = g_nm_declines.total)

def metal_nmt_declines : table<string; int64> => clone_to_move(g_nm_declines.by_reason)

//! The command buffers' device-busy and host-encode microseconds, summed.
def metal_nmt_times : tuple<gpu_us : int64; encode_us : int64> => (gpu_us = g_nm_us_gpu, encode_us = g_nm_us_encode)

// one command buffer on a serial encoder: a step is one dependency chain, which a concurrent encoder's barriers only lengthen
def private nm_run_cb(var err : string&; blk : block<(enc : MetalComputeEncoder?) : void>) : bool {
    var gpu_ms = 0.0lf
    var encode_ms = 0.0lf
    let ok = with_compute_encoder_timed(g_queue, err, gpu_ms, encode_ms) $(enc) {
        invoke(blk, enc)
    }
    g_nm_us_gpu += int64(gpu_ms * 1000.0lf)
    g_nm_us_encode += int64(encode_ms * 1000.0lf)
    return ok
}

def private nm_release_all {
    release_handles(g_nm_mblob)
    g_nm_key = 0ul
}

//! Release the driver's device objects - the leak gate's rail and the model-switch seat; the next sentence re-attaches.
def metal_nmt_shutdown {
    nm_release_all()
    pool_drain(g_nm_pool)
    g_nm_said |> clear()
    g_nm_rate = 1.0lf
}

//! Probe seam: the attached classifier's quants zeroed on the device, so every step's logits are the output bias alone - the
//! device cells' control that the device served the compare; `metal_nmt_shutdown` drops the poisoned blob.
def metal_nmt_zero_cls_probe_(m : NmtModel) {
    if (g_nm_mblob != null) {
        unsafe {
            memset8(reinterpret<uint8?>(metal_buffer_contents(g_nm_mblob)) + int64(g_nm_cls_boff), uint8(0), int(m.n_vocab * (m.d / Q8_BLOCK_ELEMS) * NM_Q8_BLOCK_BYTES))
        }
    }
}

[cold_path]
def private nm_init : bool {
    if (!metal_common_init() || !metal_prefill_pso_init()) {
        return false
    }
    if (!g_nm_tried) {
        g_nm_tried = true
        var ok = true
        g_nm_pso_embed = compile_stamp(metal_nmt_embed_msl, ok)
        g_nm_pso_ssru = compile_stamp(metal_nmt_ssru_msl, ok)
        g_nm_pso_pick = compile_stamp(metal_nmt_pick_msl, ok)
    }
    return g_nm_pso_embed != null && g_nm_pso_ssru != null && g_nm_pso_pick != null && g_pso_wdec_kv_h != null
}

// regions laid back to back, each its own repack unit, read by one GEMM as `rows` x n: the stacked [q | k | v] and [k | v]
def private nm_stack(m : NmtModel; offs : array<int64>; n, rows : int64; lay : Q8RepackLayout; var staging : array<uint8>;
                     var cur : uint64&) : uint64 {
    let first = q8_region_append(m.qblob, m.qscales, offs[0], n, rows, lay, staging, cur)
    for (i in range(1, length(offs))) {
        let at = q8_region_append(m.qblob, m.qscales, offs[i], n, rows, lay, staging, cur)
        assert(at == first + uint64(i) * uint64((rows * n / Q8_BLOCK_ELEMS) * NM_Q8_BLOCK_BYTES), "metal nmt: a stacked region is not contiguous")
    }
    return first
}

[cold_path]
def private nm_attach(m : NmtModel) : bool {
    if (g_nm_mblob != null && m.gen == g_nm_key) {
        return true
    }
    nm_release_all()
    let d = m.d
    let ff = m.ff
    g_nm_nvpad = (m.n_vocab + 63l) & ~63l
    var staging : array<uint8>
    staging |> reserve_resize((length(m.qblob) / Q8_BLOCK_ELEMS) * NM_Q8_BLOCK_BYTES + (g_nm_nvpad - m.n_vocab) * (d / Q8_BLOCK_ELEMS) * NM_Q8_BLOCK_BYTES
                            + 16l * (m.n_enc * 8l + m.n_dec * 10l + 2l))
    let lay = active_q8_repack_layout(kernel_backend_needs_repack(active_kernel_backend()))
    var cur = 0ul
    g_nm_enc_boffs |> reserve(m.n_enc * NM_ENC_MATS)
    g_nm_enc_boffs |> resize(m.n_enc * NM_ENC_MATS)
    for (l in range64(m.n_enc)) {
        let o = nmt_enc_offsets(m, l)
        g_nm_enc_boffs[l * NM_ENC_MATS] = nm_stack(m, [o.wq, o.wk, o.wv], d, d, lay, staging, cur)
        g_nm_enc_boffs[l * NM_ENC_MATS + 1l] = q8_region_append(m.qblob, m.qscales, o.wo, d, d, lay, staging, cur)
        g_nm_enc_boffs[l * NM_ENC_MATS + 2l] = q8_region_append(m.qblob, m.qscales, o.w1, d, ff, lay, staging, cur)
        g_nm_enc_boffs[l * NM_ENC_MATS + 3l] = q8_region_append(m.qblob, m.qscales, o.w2, ff, d, lay, staging, cur)
    }
    g_nm_dec_boffs |> reserve(m.n_dec * NM_DEC_MATS)
    g_nm_dec_boffs |> resize(m.n_dec * NM_DEC_MATS)
    for (l in range64(m.n_dec)) {
        let o = nmt_dec_offsets(m, l)
        g_nm_dec_boffs[l * NM_DEC_MATS] = q8_region_append(m.qblob, m.qscales, o.wrnn, d, 2l * d, lay, staging, cur)
        g_nm_dec_boffs[l * NM_DEC_MATS + 1l] = q8_region_append(m.qblob, m.qscales, o.wq, d, d, lay, staging, cur)
        g_nm_dec_boffs[l * NM_DEC_MATS + 2l] = nm_stack(m, [o.wk, o.wv], d, d, lay, staging, cur)
        g_nm_dec_boffs[l * NM_DEC_MATS + 3l] = q8_region_append(m.qblob, m.qscales, o.wo, d, d, lay, staging, cur)
        g_nm_dec_boffs[l * NM_DEC_MATS + 4l] = q8_region_append(m.qblob, m.qscales, o.w1, d, ff, lay, staging, cur)
        g_nm_dec_boffs[l * NM_DEC_MATS + 5l] = q8_region_append(m.qblob, m.qscales, o.w2, ff, d, lay, staging, cur)
    }
    g_nm_cls_boff = q8_region_append(m.qblob, m.qscales, m.cls_off, d, m.n_vocab, lay, staging, cur)
    cur += uint64((g_nm_nvpad - m.n_vocab) * (d / Q8_BLOCK_ELEMS) * NM_Q8_BLOCK_BYTES)   // zero rows: their logits go unread
    g_nm_mblob = metal_new_buffer(g_dev, cur)
    if (g_nm_mblob == null) {
        delete staging
        return false
    }
    unsafe {
        memcpy(metal_buffer_contents(g_nm_mblob), addr(staging[0]), cur)
    }
    delete staging
    g_nm_key = m.gen
    g_nm_rate = 1.0lf   // the previous model's steps a token sized nothing of this one
    return true
}

struct private NmBufs {
    sizes : array<uint64>
    bufs : array<MetalBuffer?>
}

def private nm_take(var b : NmBufs; bytes : uint64) : MetalBuffer? {
    b.sizes |> push(bytes)
    var buf = pool_acquire(g_nm_pool, g_dev, bytes)
    b.bufs |> push(buf)
    return buf
}

def private nm_give(var b : NmBufs) {
    for (buf, bytes in b.bufs, b.sizes) {
        pool_release(g_nm_pool, buf, bytes)
    }
    b.bufs |> clear()
    b.sizes |> clear()
}

//! A sentence's width uniforms, held until its last command buffer completes.
struct private NmUniforms {
    d : MetalBuffer?
    d2 : MetalBuffer?
    d3 : MetalBuffer?
    ff : MetalBuffer?
    nv : MetalBuffer?
}

def private nm_uniforms(m : NmtModel) : NmUniforms {
    return NmUniforms(d = uniform_u32(uint(m.d)), d2 = uniform_u32(uint(2l * m.d)), d3 = uniform_u32(uint(3l * m.d)),
                      ff = uniform_u32(uint(m.ff)), nv = uniform_u32(uint(g_nm_nvpad)))
}

def private nm_uniforms_give(var u : NmUniforms) {
    pool_release(g_upool, u.d, 4ul)
    pool_release(g_upool, u.d2, 4ul)
    pool_release(g_upool, u.d3, 4ul)
    pool_release(g_upool, u.ff, 4ul)
    pool_release(g_upool, u.nv, 4ul)
}

// one q8 GEMM over `mp` rows, `uk` / `un` the uniforms of `kdim` / `n`: the half twin of `bx` converted first where the
// ladder reads one
def private nm_mm(enc : MetalComputeEncoder?; boff : uint64; bx, bxh, by, uk, un : MetalBuffer?; mp, kdim, n : int64) {
    if (bxh != null) {
        tw_half(enc, bx, bxh, mp * kdim)
    }
    pf_enc_q8_mm(enc, g_nm_mblob, boff, bx, bxh, by, uk, un, mp, n, kdim)
}

//! The steps a command buffer carries after the first: a sentence's tail is short, and a step past its end is wasted work.
let private NM_TAIL_STEPS = 4l

// the served sentences' steps a source token so far (the end token included), the first command buffer's estimate
var private g_nm_rate = 1.0lf

//! The whole sentence on the device: the encode and every decoder layer's cross keys and values in one command buffer, then
//! command buffers of greedy steps - the argmax and the state on the device - until the pick marks the sentence done.
def private metal_nmt_translate(m : NmtModel; var st : NmtState; src : array<int64>; var out : array<int64>) : bool {   // nolint:STYLE038 — the dispatch chain is the shape
    if (!g_nm_env) {
        return nm_decline(MetalNmtDecline.knob)
    }
    if (!m.q8) {
        return nm_decline(MetalNmtDecline.quant_mode)
    }
    let d = m.d
    let ff = m.ff
    let hh = m.n_head
    let hs = d / hh
    if (!nmt_device_shape_ok(m)) {
        return nm_decline(MetalNmtDecline.shape)
    }
    if (long_length(src) > NMT_DEVICE_SRC_CAP) {
        return nm_decline(MetalNmtDecline.length)
    }
    if (!nm_init() || !nm_attach(m)) {
        return nm_decline(MetalNmtDecline.device)
    }
    let s = long_length(src)
    let cap = NMT_MAX_LEN_FACTOR * s
    let mp = (s + 31l) / 32l * 32l
    nmt_positions(m, st, max(s, cap))
    let mapped = m.image_map != null
    var wb = plane_buffer(g_dev, unsafe(reinterpret<void?>(plane_at(m.blob, 0l))), uint64(length(m.blob) * 4l), mapped)
    var b : NmBufs
    let half = pf_q8_mm_half()
    let wide = max(3l * d, ff)
    var bids = nm_take(b, uint64(mp * 4l))
    var bpos = nm_take(b, uint64(max(s, cap) * d * 4l))
    var bx = nm_take(b, uint64(mp * d * 4l))
    var by = nm_take(b, uint64(mp * d * 4l))
    var bat = nm_take(b, uint64(mp * d * 4l))
    var bh = nm_take(b, uint64(mp * wide * 4l))
    var bxh = half ? nm_take(b, uint64(mp * wide * 2l)) : null
    var bk = nm_take(b, uint64(d * s * 2l))
    var bv = nm_take(b, uint64(d * s * 2l))
    var bck = nm_take(b, uint64(m.n_dec * d * s * 2l))
    var bcv = nm_take(b, uint64(m.n_dec * d * s * 2l))
    let nch = (s + 255l) / 256l
    var bpms = nm_take(b, uint64(nch * hh * s * 2l * 4l))
    var bpy = nm_take(b, uint64(nch * hh * s * hs * 4l))
    var bcell = nm_take(b, uint64(m.n_dec * d * 4l))
    var bst = nm_take(b, uint64(NMT_ST_WORDS) * 4ul)
    var btoks = nm_take(b, uint64(max(cap, 1l) * 4l))
    var blg = nm_take(b, uint64(g_nm_nvpad * 4l))
    unsafe {
        var ip = reinterpret<uint?>(metal_buffer_contents(bids))
        for (r in range64(mp)) {
            ip[r] = r < s ? uint(src[r]) : 0u
        }
        memcpy(metal_buffer_contents(bpos), addr(st.pos[0]), max(s, cap) * d * 4l)
        memset8(reinterpret<uint8?>(metal_buffer_contents(bcell)), uint8(0), int(m.n_dec * d * 4l))
        var sp = reinterpret<uint?>(metal_buffer_contents(bst))
        sp[NMT_ST_T] = 0u
        sp[NMT_ST_PREV] = NMT_NO_TOKEN
        sp[NMT_ST_DONE] = 0u
        sp[NMT_ST_NTOK] = 0u
    }
    zero_rows(bx, 0l, mp, d)
    var u = nm_uniforms(m)
    let scale = 1.0 / sqrt(float(hs))
    var err = ""
    // the encode, then every decoder layer's cross keys and values (staged as halves, biases folded)
    let encoded = nm_run_cb(err) $(enc : MetalComputeEncoder?) {
        enc_nmt_embed(enc, bids, wb, bpos, bx, GkNmtEmbedArgs(d = uint(d), rows = uint(s), emb_off = uint(m.emb_off), scale = sqrt(float(d)), from_state = 0u), s * d)
        for (l in range64(m.n_enc)) {
            let o = nmt_enc_offsets(m, l)
            let bo = l * NM_ENC_MATS
            nm_mm(enc, g_nm_enc_boffs[bo], bx, bxh, bh, u.d, u.d3, mp, d, 3l * d)
            var kv = GkWdecKvArgs(rows = uint(s), sstride = uint(3l * d), soff = uint(d), nheads = uint(hh), tstride = uint(s), p0 = 0u, dst_off = 0u, boff = uint(o.bk), bias_on = 1u, mode = WDEC_KV_F16, hs = uint(hs), kscale = 1.0)
            enc_wdec_kv_h(enc, bh, bk, wb, kv, s * d)
            kv.soff = uint(2l * d)
            kv.boff = uint(o.bv)
            enc_wdec_kv_h(enc, bh, bv, wb, kv, s * d)
            enc_wdec_attn_part(enc, bh, bk, bv, bpms, bpy, wb, GkWdecAttnArgs(dim = uint(3l * d), tstride = uint(s), len0 = uint(s),
                causal = 0u, nheads = uint(hh), nrows = uint(s), koff = 0u, voff = 0u, qboff = uint(o.bq), hs = uint(hs), scale = scale), nch * hh * s)
            enc_wdec_attn_comb(enc, bpms, bpy, bat, GkWdecCombArgs(dim = uint(d), nchunks = uint(nch), nheads = uint(hh), nrows = uint(s), hs = uint(hs)), hh * s)
            nm_mm(enc, g_nm_enc_boffs[bo + 1l], bat, bxh, by, u.d, u.d, mp, d, d)
            enc_tw_add_ln(enc, bx, by, bx, wb, gk_add_ln_args(uint(d), uint(s), NMT_LN_EPS, uint(o.bo), true, uint(o.ln1_s), uint(o.ln1_b)), s)
            nm_mm(enc, g_nm_enc_boffs[bo + 2l], bx, bxh, bh, u.d, u.ff, mp, d, ff)
            tw_bias(enc, bh, wb, GkBiasActArgs(d = uint(ff), nelem = uint(s * ff), boff = uint(o.b1), act = BIAS_ACT_RELU))
            nm_mm(enc, g_nm_enc_boffs[bo + 3l], bh, bxh, by, u.ff, u.d, mp, ff, d)
            enc_tw_add_ln(enc, bx, by, bx, wb, gk_add_ln_args(uint(d), uint(s), NMT_LN_EPS, uint(o.b2), true, uint(o.ln2_s), uint(o.ln2_b)), s)
        }
        for (l in range64(m.n_dec)) {
            let o = nmt_dec_offsets(m, l)
            nm_mm(enc, g_nm_dec_boffs[l * NM_DEC_MATS + 2l], bx, bxh, bh, u.d, u.d2, mp, d, 2l * d)
            var kv = GkWdecKvArgs(rows = uint(s), sstride = uint(2l * d), soff = 0u, nheads = uint(hh), tstride = uint(s), p0 = 0u, dst_off = uint(l * d * s), boff = uint(o.bk), bias_on = 1u, mode = WDEC_KV_F16, hs = uint(hs), kscale = 1.0)
            enc_wdec_kv_h(enc, bh, bck, wb, kv, s * d)
            kv.soff = uint(d)
            kv.boff = uint(o.bv)
            enc_wdec_kv_h(enc, bh, bcv, wb, kv, s * d)
        }
    }
    var ok = encoded
    var steps = 0l
    var cbs = 1l
    let stp = unsafe(reinterpret<uint const?>(metal_buffer_contents(bst)))
    while (ok && steps < cap) {
        let n = min(steps == 0l ? int64(double(s) * g_nm_rate) + 1l : NM_TAIL_STEPS, cap - steps)
        ok = nm_run_cb(err) $(enc : MetalComputeEncoder?) {
            for (_t in range64(n)) {
                enc_nmt_embed(enc, bst, wb, bpos, bx, GkNmtEmbedArgs(d = uint(d), rows = 1u, emb_off = uint(m.emb_off), scale = sqrt(float(d)), from_state = 1u), d)
                for (l in range64(m.n_dec)) {
                    let o = nmt_dec_offsets(m, l)
                    let bo = l * NM_DEC_MATS
                    enc_gemv(enc, g_nm_mblob, g_nm_dec_boffs[bo], bx, bh, u.d, u.d2, 2l * d)
                    enc_nmt_ssru(enc, bh, wb, bcell, by, GkNmtSsruArgs(d = uint(d), bf_off = uint(o.bf), cell_off = uint(l * d)), d)
                    enc_pk_add_ln(enc, bx, by, bx, wb, gk_add_ln_args(uint(d), 1u, NMT_LN_EPS, 0u, true, uint(o.rnn_ln_s), uint(o.rnn_ln_b)), 1l)
                    enc_gemv(enc, g_nm_mblob, g_nm_dec_boffs[bo + 1l], bx, by, u.d, u.d, d)
                    enc_wdec_attn_part(enc, by, bck, bcv, bpms, bpy, wb, GkWdecAttnArgs(dim = uint(d), tstride = uint(s), len0 = uint(s),
                        causal = 0u, nheads = uint(hh), nrows = 1u, koff = uint(l * d * s), voff = uint(l * d * s), qboff = uint(o.bq), hs = uint(hs), scale = scale), nch * hh)
                    enc_wdec_attn_comb(enc, bpms, bpy, bat, GkWdecCombArgs(dim = uint(d), nchunks = uint(nch), nheads = uint(hh), nrows = 1u, hs = uint(hs)), hh)
                    enc_gemv(enc, g_nm_mblob, g_nm_dec_boffs[bo + 3l], bat, by, u.d, u.d, d)
                    enc_tw_add_ln(enc, bx, by, bx, wb, gk_add_ln_args(uint(d), 1u, NMT_LN_EPS, uint(o.bo), true, uint(o.ctx_ln_s), uint(o.ctx_ln_b)), 1l)
                    enc_gemv(enc, g_nm_mblob, g_nm_dec_boffs[bo + 4l], bx, bh, u.d, u.ff, ff)
                    tw_bias(enc, bh, wb, GkBiasActArgs(d = uint(ff), nelem = uint(ff), boff = uint(o.b1), act = BIAS_ACT_RELU))
                    enc_gemv(enc, g_nm_mblob, g_nm_dec_boffs[bo + 5l], bh, by, u.ff, u.d, d)
                    enc_tw_add_ln(enc, bx, by, bx, wb, gk_add_ln_args(uint(d), 1u, NMT_LN_EPS, uint(o.b2), true, uint(o.ffn_ln_s), uint(o.ffn_ln_b)), 1l)
                }
                enc_gemv(enc, g_nm_mblob, g_nm_cls_boff, bx, blg, u.d, u.nv, g_nm_nvpad)
                enc_nmt_pick(enc, blg, wb, bst, btoks, GkNmtPickArgs(nv = uint(m.n_vocab), boff = uint(m.out_b), eos = uint(m.eos_id), cap = uint(cap)))
            }
        }
        cbs++
        steps += n
        break if (unsafe(stp[NMT_ST_DONE]) != 0u)
    }
    if (ok) {
        let nt = int64(unsafe(stp[NMT_ST_NTOK]))
        let tp = unsafe(reinterpret<uint const?>(metal_buffer_contents(btoks)))
        out |> reserve(nt)
        for (i in range64(nt)) {
            out |> push(int64(unsafe(tp[i])))
        }
        g_nm_sentences++
        g_nm_steps += steps
        g_nm_cbs += cbs
        g_nm_tokens += nt
        g_nm_rate = 0.9lf * g_nm_rate + 0.1lf * double(nt + 1l) / double(s)
    }
    nm_give(b)
    nm_uniforms_give(u)
    return ok ? true : nm_decline(MetalNmtDecline.gpu_error)
}

[init]
def private register_metal_nmt {
    register_nmt_gpu(@@metal_nmt_translate)
}
