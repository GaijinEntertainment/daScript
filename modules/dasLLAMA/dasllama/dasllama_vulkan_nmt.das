options gen2
options indenting = 4
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_vulkan_nmt shared public

require dasllama/dasllama_lint public
require vulkan
require math
require dasllama/dasllama_kqformat
require dasllama/dasllama_math   // the active repack layout - the q8 planes carry the CPU backend's
require dasllama/dasllama_plane
require dasllama/dasllama_nmt
require dasllama/dasllama_tower   // Q8Region
require dasllama/dasllama_gpu_math   // the row stamps' activation codes
require dasllama/dasllama_gpu_kernels_common
require dasllama/dasllama_vulkan_common
require dasllama/dasllama_vulkan_classes
require dasllama/dasllama_vulkan_tower

//! The Bergamot translation Vulkan driver, the Metal driver's form on the Vulkan stamps: a sentence's encode and every decoder
//! layer's cross keys and values in one command buffer, then command buffers of greedy steps - the argmax and the state words on
//! the device (`NmtPick`) - until the pick marks the sentence done. Every block GEMM is the q8 GEMV over the row-major gather, its
//! N-column form over the encoder's rows; each row pass is followed by its Q8_0 requant, the next GEMV's feed.

let private NV_ENC_SITES = 4l          //! an encoder layer's GEMV sites: [q | k | v], o, w1, w2
let private NV_DEC_SITES = 6l          //! a decoder layer's: the cross [k | v] (the encode's), the SSRU's [W | Wf], q, o, w1, w2
let private NV_TAIL_STEPS = 4l         //! the steps a command buffer carries after the first: a sentence's tail is short

//! the hazard bits of the driver's buffers
let private NV_IDS = 0x1u
let private NV_POS = 0x2u
let private NV_X = 0x4u
let private NV_XQ = 0x8u
let private NV_H = 0x10u
let private NV_Y = 0x20u
let private NV_Q = 0x40u
let private NV_AT = 0x80u
let private NV_K = 0x100u
let private NV_V = 0x200u
let private NV_CK = 0x400u
let private NV_CV = 0x800u
let private NV_PART = 0x1000u
let private NV_CELL = 0x2000u
let private NV_ST = 0x4000u
let private NV_TOKS = 0x8000u
let private NV_LG = 0x10000u

enum private VulkanNmtDecline {
    knob         //! set_vulkan_nmt(false)
    quant_mode   //! the exact lane: the driver serves the q8 planes
    shape        //! a head width off the attention's 4 lattice, or a width off the requant's 32 blocks
    length       //! a source longer than the resident's `NMT_DEVICE_SRC_CAP` rows
    device       //! the tier has no device, or a class failed to build
    memory       //! a device or host allocation failed
}

//! The driver's device objects, laid once a model: the q8 gather (`wq`, `ws`), the f32 rows (`wn`: the embedding table, every bias
//! and norm row, the output bias), and the scratch sized to `NMT_DEVICE_SRC_CAP` rows.
struct private NvResident {
    key : uint64
    d : int64
    ff : int64
    hh : int64
    hs : int64
    nenc : int64
    ndec : int64
    nvocab : int64
    wq_nv : uint64
    ws_nv : uint64
    wn_nv : uint64
    wq_bytes : int64
    ws_bytes : int64
    wn_bytes : int64
    ids_nv : uint64
    pos_nv : uint64
    x_nv : uint64
    xq_nv : uint64
    xs_nv : uint64
    h_nv : uint64
    y_nv : uint64
    q_nv : uint64
    at_nv : uint64
    k_nv : uint64
    v_nv : uint64
    ck_nv : uint64
    cv_nv : uint64
    pms_nv : uint64
    py_nv : uint64
    cell_nv : uint64
    st_nv : uint64
    toks_nv : uint64
    lg_nv : uint64
    hin : HostBuf    //! a sentence's source ids, its starting state words, its position rows
    hout : HostBuf   //! the state words, then the token records
    @scratch @exact_size site_meta : array<HostBuf>
    @scratch @exact_size s_site : array<VkDescriptorSet>
    s_embed_src : VkDescriptorSet
    s_embed_step : VkDescriptorSet
    s_ssru : VkDescriptorSet
    s_pick : VkDescriptorSet
    s_kv_k : VkDescriptorSet
    s_kv_v : VkDescriptorSet
    s_ckv_k : VkDescriptorSet
    s_ckv_v : VkDescriptorSet
    s_part_enc : VkDescriptorSet
    s_part_dec : VkDescriptorSet
    s_comb : VkDescriptorSet
    s_seam : VkDescriptorSet
    s_seam_nb : VkDescriptorSet
    s_bias : VkDescriptorSet
    s_rq_x : VkDescriptorSet
    s_rq_at : VkDescriptorSet
    s_rq_h : VkDescriptorSet
    oom : bool
    served_said : bool
}

var private g_nv : NvResident
var private g_nv_env = true
var private g_nv_declines : VkDeclineCounter
var private g_nv_sentences = 0l
var private g_nv_steps = 0l
var private g_nv_cbs = 0l
var private g_nv_tokens = 0l
var private g_nv_us_gpu = 0l
var private g_nv_us_encode = 0l
var private g_nv_rate = 1.0lf   //! the served sentences' steps a source token so far (the end token included), the first command buffer's estimate

//! Serve the translation family's sentences on the device (default on).
def set_vulkan_nmt(on : bool) {
    g_nv_env = on
}

def vulkan_nmt_knob() : bool => g_nv_env

//! `sentences` served whole on the device, the greedy `steps` they ran (a command buffer's tail past a sentence's end
//! included), the command buffers, the tokens they landed, `declines` every decline but the knob's.
def vulkan_nmt_stats : tuple<sentences : int64; steps : int64; cbs : int64; tokens : int64; declines : int64> {
    let policy = g_nv_declines.counts?["{VulkanNmtDecline.knob}"] ?? 0l
    return (sentences = g_nv_sentences, steps = g_nv_steps, cbs = g_nv_cbs, tokens = g_nv_tokens, declines = g_nv_declines.total - policy)
}

//! The declines-by-reason table (a copy), keyed by `VulkanNmtDecline` value names, the knob's among them.
def vulkan_nmt_declines : table<string; int64> => <- clone_to_move(g_nv_declines.counts)

//! The command buffers' submit walls and the host's work around them (the state init, the readbacks), microseconds, summed.
def vulkan_nmt_times : tuple<us_gpu : int64; us_encode : int64> => (us_gpu = g_nv_us_gpu, us_encode = g_nv_us_encode)

[cold_path]
def private nv_decline(why : VulkanNmtDecline) : bool {
    let words = (why == VulkanNmtDecline.knob ? "the translation knob is off (set_vulkan_nmt(false))"
        : why == VulkanNmtDecline.quant_mode ? "the exact lane's f32 planes, not the q8 planes the driver serves"
        : why == VulkanNmtDecline.shape ? "a geometry off the GPU homes' lattice (nmt_device_shape_ok)"
        : why == VulkanNmtDecline.length ? "a source longer than the device's {NMT_DEVICE_SRC_CAP} rows"
        : why == VulkanNmtDecline.memory ? VK_DECLINE_WORDS_MEMORY : VK_DECLINE_WORDS_DEVICE)
    return g_nv_declines |> note("the translation driver", "{why}", "the sentence stays on the CPU chain: {words}")
}

[cold_path]
def private nv_dev_buf(bytes : int64) : uint64 => soft_device_buf(g_nv.oom, bytes)

[cold_path]
def private nv_release {
    var r & = g_nv
    if (r.key != 0ul) {
        for (b in fixed_array(r.wq_nv, r.ws_nv, r.wn_nv, r.ids_nv, r.pos_nv, r.x_nv, r.xq_nv, r.xs_nv, r.h_nv, r.y_nv, r.q_nv,
                              r.at_nv, r.k_nv, r.v_nv, r.ck_nv, r.cv_nv, r.pms_nv, r.py_nv, r.cell_nv, r.st_nv, r.toks_nv, r.lg_nv)) {
            if (b != 0ul) {
                destroy_device_buf(b)
            }
        }
        for (hb in fixed_array(r.hin, r.hout)) {
            if (hb.buf != 0ul) {
                destroy_host_buf(hb)
            }
        }
        for (hb in r.site_meta) {
            if (hb.buf != 0ul) {
                destroy_host_buf(hb)
            }
        }
    }
    nv_forget()
}

//! the handles forgotten (the model drop's sweep destroys the objects); the next served sentence uploads again
[cold_path]
def private nv_forget {
    unsafe {
        delete g_nv   // the arrays' buffers alone: HostBuf's mapped pointer and the descriptor sets are handles, not das heap
    }
    g_nv <- NvResident()
    g_nv_rate = 1.0lf
    g_nv_declines |> unsay("the translation driver")
}

//! Release every device object the driver owns - the leak gate's rail and the model-switch seat; the next sentence re-attaches.
def vulkan_nmt_shutdown {
    nv_release()
}

//! Probe seam: the attached classifier's quants zeroed on the device, so every step's logits are the output bias alone - the
//! device cells' control that the device served the compare; `vulkan_nmt_shutdown` drops the poisoned planes.
def vulkan_nmt_zero_cls_probe_(m : NmtModel) {
    if (g_nv.wq_nv != 0ul) {
        upload_zeros(g_nv.wq_nv, m.cls_off, m.n_vocab * m.d)
    }
}

//! the GEMV site index of encoder layer `l`'s matrix `k`, decoder layer `l`'s, and the classifier's
def private nv_enc_site(l, k : int64) : int => int(l * NV_ENC_SITES + k)

def private nv_dec_site(l, k : int64) : int => int(g_nv.nenc * NV_ENC_SITES + l * NV_DEC_SITES + k)

def private nv_cls_site : int => int(g_nv.nenc * NV_ENC_SITES + g_nv.ndec * NV_DEC_SITES)

//! every class a sentence dispatches, built
[cold_path]
def private nv_classes : bool {
    return (gemv_cls_ensure(int(KqFmt.q8)) && gemv_cls_ensure_n(int(KqFmt.q8), 8l) && ensure_nmt_embed_cls() && ensure_nmt_ssru_cls()
        && ensure_nmt_pick_cls() && ensure_nmt_attn_part_cls() && ensure_nmt_attn_comb_cls() && ensure_tower_wdec_kv16_cls()
        && ensure_tower_post_add_ln_cls() && ensure_tower_post_add_ln_nb_cls() && ensure_tower_bias_act_cls() && ensure_tower_clamp_rq_cls())
}

//! the scratch's bytes from the geometry at `NMT_DEVICE_SRC_CAP` rows: the stream rows (`bx`), the widest GEMV output (`bh`), the requant
//! feed's quants and scales (`bq`, `bs`), a layer's f16 K/V plane (`bkv`), the partials (`bpms`, `bpy`), the position rows (`bpos`)
def private nv_sizes(r : NvResident) : tuple<bx : int64; bh : int64; bq : int64; bs : int64; bkv : int64; bpms : int64; bpy : int64; bpos : int64> {
    let rows = NMT_DEVICE_SRC_CAP
    let nch = (rows + int64(WDEC_CHUNK) - 1l) / int64(WDEC_CHUNK)
    let bq = rows * max(r.d, r.ff)
    return (bx = rows * r.d * 4l, bh = rows * max(3l * r.d, r.ff) * 4l, bq = bq, bs = bq / kq_elems(KqFmt.q8) * 4l, bkv = rows * r.d * 2l,
            bpms = nch * r.hh * rows * 2l * 4l, bpy = nch * r.hh * rows * r.hs * 4l, bpos = NMT_MAX_LEN_FACTOR * rows * r.d * 4l)
}

//! the block GEMMs gathered row-major at their own element offsets - each its own repack unit, so a layer's [q | k | v] and
//! [k | v] lie back to back and one site reads them as one region - and every site's region pair
[cold_path]
def private nv_upload_weights(m : NmtModel) {
    var r & = g_nv
    let d = m.d
    let ff = m.ff
    r.wq_bytes = m.qblob.n
    r.ws_bytes = m.qscales.n * 2l
    r.wq_nv = nv_dev_buf(r.wq_bytes)
    r.ws_nv = nv_dev_buf(r.ws_bytes)
    var regions : array<Q8Region>
    regions |> reserve(m.n_enc * 6l + m.n_dec * 7l + 1l)
    r.site_meta |> reserve(m.n_enc * NV_ENC_SITES + m.n_dec * NV_DEC_SITES + 1l)
    for (l in range64(m.n_enc)) {
        let o = nmt_enc_offsets(m, l)
        for (w in fixed_array(o.wq, o.wk, o.wv, o.wo)) {
            regions |> push(Q8Region(woff = w, n = d, rows = d))
        }
        regions |> push(Q8Region(woff = o.w1, n = d, rows = ff))
        regions |> push(Q8Region(woff = o.w2, n = ff, rows = d))
        for (w in fixed_array(o.wq, o.wo, o.w1, o.w2)) {
            r.site_meta |> push(q8_site_meta(r.oom, [w]))
        }
    }
    for (l in range64(m.n_dec)) {
        let o = nmt_dec_offsets(m, l)
        regions |> push(Q8Region(woff = o.wrnn, n = d, rows = 2l * d))
        for (w in fixed_array(o.wq, o.wk, o.wv, o.wo)) {
            regions |> push(Q8Region(woff = w, n = d, rows = d))
        }
        regions |> push(Q8Region(woff = o.w1, n = d, rows = ff))
        regions |> push(Q8Region(woff = o.w2, n = ff, rows = d))
        for (w in fixed_array(o.wk, o.wrnn, o.wq, o.wo, o.w1, o.w2)) {
            r.site_meta |> push(q8_site_meta(r.oom, [w]))
        }
    }
    regions |> push(Q8Region(woff = m.cls_off, n = d, rows = m.n_vocab))
    r.site_meta |> push(q8_site_meta(r.oom, [m.cls_off]))
    if (!r.oom) {
        vt_upload_regions(m.qblob, m.qscales, regions, active_q8_repack_layout(kernel_backend_needs_repack(active_kernel_backend())), r.wq_nv,
            r.ws_nv, r.wq_bytes, r.ws_bytes)
    }
    delete regions
}

[cold_path]
def private nv_sets {
    var r & = g_nv
    let z = nv_sizes(r)
    let bck = r.ndec * z.bkv
    let bst = int64(NMT_ST_WORDS) * 4l
    let btoks = NMT_MAX_LEN_FACTOR * NMT_DEVICE_SRC_CAP * 4l
    let blg = r.nvocab * 4l
    r.s_embed_src = set_nmt_embed_cls(fixed_array(r.ids_nv, r.wn_nv, r.pos_nv, r.x_nv), fixed_array(NMT_DEVICE_SRC_CAP * 4l, r.wn_bytes, z.bpos, z.bx),
        fixed_array(NV_IDS, 0u, NV_POS, NV_X))
    r.s_embed_step = set_nmt_embed_cls(fixed_array(r.st_nv, r.wn_nv, r.pos_nv, r.x_nv), fixed_array(bst, r.wn_bytes, z.bpos, z.bx),
        fixed_array(NV_ST, 0u, NV_POS, NV_X))
    r.s_ssru = set_nmt_ssru_cls(fixed_array(r.h_nv, r.wn_nv, r.cell_nv, r.y_nv), fixed_array(z.bh, r.wn_bytes, r.ndec * r.d * 4l, z.bx),
        fixed_array(NV_H, 0u, NV_CELL, NV_Y))
    r.s_pick = set_nmt_pick_cls(fixed_array(r.lg_nv, r.wn_nv, r.st_nv, r.toks_nv), fixed_array(blg, r.wn_bytes, bst, btoks),
        fixed_array(NV_LG, 0u, NV_ST, NV_TOKS))
    r.s_kv_k = set_tower_wdec_kv16_cls(fixed_array(r.h_nv, r.k_nv, r.wn_nv), fixed_array(z.bh, z.bkv, r.wn_bytes), fixed_array(NV_H, NV_K, 0u))
    r.s_kv_v = set_tower_wdec_kv16_cls(fixed_array(r.h_nv, r.v_nv, r.wn_nv), fixed_array(z.bh, z.bkv, r.wn_bytes), fixed_array(NV_H, NV_V, 0u))
    r.s_ckv_k = set_tower_wdec_kv16_cls(fixed_array(r.h_nv, r.ck_nv, r.wn_nv), fixed_array(z.bh, bck, r.wn_bytes), fixed_array(NV_H, NV_CK, 0u))
    r.s_ckv_v = set_tower_wdec_kv16_cls(fixed_array(r.h_nv, r.cv_nv, r.wn_nv), fixed_array(z.bh, bck, r.wn_bytes), fixed_array(NV_H, NV_CV, 0u))
    r.s_part_enc = set_nmt_attn_part_cls(fixed_array(r.h_nv, r.k_nv, r.v_nv, r.pms_nv, r.py_nv, r.wn_nv),
        fixed_array(z.bh, z.bkv, z.bkv, z.bpms, z.bpy, r.wn_bytes), fixed_array(NV_H, NV_K, NV_V, NV_PART, NV_PART, 0u))
    r.s_part_dec = set_nmt_attn_part_cls(fixed_array(r.q_nv, r.ck_nv, r.cv_nv, r.pms_nv, r.py_nv, r.wn_nv),
        fixed_array(r.d * 4l, bck, bck, z.bpms, z.bpy, r.wn_bytes), fixed_array(NV_Q, NV_CK, NV_CV, NV_PART, NV_PART, 0u))
    r.s_comb = set_nmt_attn_comb_cls(fixed_array(r.pms_nv, r.py_nv, r.at_nv), fixed_array(z.bpms, z.bpy, z.bx), fixed_array(NV_PART, NV_PART, NV_AT))
    r.s_seam = set_tower_post_add_ln_cls(fixed_array(r.x_nv, r.y_nv, r.x_nv, r.wn_nv), fixed_array(z.bx, z.bx, z.bx, r.wn_bytes),
        fixed_array(NV_X, NV_Y, NV_X, 0u))
    r.s_seam_nb = set_tower_post_add_ln_nb_cls(fixed_array(r.x_nv, r.y_nv, r.x_nv, r.wn_nv), fixed_array(z.bx, z.bx, z.bx, r.wn_bytes),
        fixed_array(NV_X, NV_Y, NV_X, 0u))
    r.s_bias = set_tower_bias_act_cls(fixed_array(r.h_nv, r.wn_nv), fixed_array(z.bh, r.wn_bytes), fixed_array(NV_H, 0u))
    r.s_rq_x = set_tower_clamp_rq_cls(fixed_array(r.x_nv, r.xq_nv, r.xs_nv), fixed_array(z.bx, z.bq, z.bs), fixed_array(NV_X, NV_XQ, NV_XQ))
    r.s_rq_at = set_tower_clamp_rq_cls(fixed_array(r.at_nv, r.xq_nv, r.xs_nv), fixed_array(z.bx, z.bq, z.bs), fixed_array(NV_AT, NV_XQ, NV_XQ))
    r.s_rq_h = set_tower_clamp_rq_cls(fixed_array(r.h_nv, r.xq_nv, r.xs_nv), fixed_array(z.bh, z.bq, z.bs), fixed_array(NV_H, NV_XQ, NV_XQ))
    let nsites = nv_cls_site() + 1
    r.s_site |> reserve(nsites)
    r.s_site |> resize(nsites)
    let f = Q8GemvFeed(planes = (wq = r.wq_nv, ws = r.ws_nv, wqb = r.wq_bytes, wsb = r.ws_bytes), xq = r.xq_nv, xs = r.xs_nv, x_elems = z.bq,
        xbit = NV_XQ)
    for (l in range64(r.nenc)) {
        r.s_site[nv_enc_site(l, 0l)] = q8_gemv_set(f, r.site_meta[nv_enc_site(l, 0l)].buf, r.h_nv, z.bh, NV_H)
        r.s_site[nv_enc_site(l, 1l)] = q8_gemv_set(f, r.site_meta[nv_enc_site(l, 1l)].buf, r.y_nv, z.bx, NV_Y)
        r.s_site[nv_enc_site(l, 2l)] = q8_gemv_set(f, r.site_meta[nv_enc_site(l, 2l)].buf, r.h_nv, z.bh, NV_H)
        r.s_site[nv_enc_site(l, 3l)] = q8_gemv_set(f, r.site_meta[nv_enc_site(l, 3l)].buf, r.y_nv, z.bx, NV_Y)
    }
    for (l in range64(r.ndec)) {
        r.s_site[nv_dec_site(l, 0l)] = q8_gemv_set(f, r.site_meta[nv_dec_site(l, 0l)].buf, r.h_nv, z.bh, NV_H)
        r.s_site[nv_dec_site(l, 1l)] = q8_gemv_set(f, r.site_meta[nv_dec_site(l, 1l)].buf, r.h_nv, z.bh, NV_H)
        r.s_site[nv_dec_site(l, 2l)] = q8_gemv_set(f, r.site_meta[nv_dec_site(l, 2l)].buf, r.q_nv, r.d * 4l, NV_Q)
        r.s_site[nv_dec_site(l, 3l)] = q8_gemv_set(f, r.site_meta[nv_dec_site(l, 3l)].buf, r.y_nv, z.bx, NV_Y)
        r.s_site[nv_dec_site(l, 4l)] = q8_gemv_set(f, r.site_meta[nv_dec_site(l, 4l)].buf, r.h_nv, z.bh, NV_H)
        r.s_site[nv_dec_site(l, 5l)] = q8_gemv_set(f, r.site_meta[nv_dec_site(l, 5l)].buf, r.y_nv, z.bx, NV_Y)
    }
    r.s_site[nv_cls_site()] = q8_gemv_set(f, r.site_meta[nv_cls_site()].buf, r.lg_nv, blg, NV_LG)
}

def private nv_attach(m : NmtModel) : bool {
    let lay = active_q8_repack_layout(kernel_backend_needs_repack(active_kernel_backend()))
    let key = vt_key(m.qblob, m.qscales, m.blob, 0l, m.blob.n, 0l, m.n_enc + m.n_dec, lay.mr, lay.kgroup, lay.wbias)
    return true if (g_nv.key != 0ul && key == g_nv.key)
    return nv_build(m, key)
}

//! the resident built for a model the key does not know: the old one released, the planes gathered and uploaded, the scratch
//! sized to `NMT_DEVICE_SRC_CAP` rows, the sets bound
[cold_path]
def private nv_build(m : NmtModel; key : uint64) : bool {
    var nv & = g_nv
    nv_release()
    nv.d = m.d
    nv.ff = m.ff
    nv.hh = m.n_head
    nv.hs = m.d / m.n_head
    nv.nenc = m.n_enc
    nv.ndec = m.n_dec
    nv.nvocab = m.n_vocab
    nv.oom = false
    let z = nv_sizes(nv)
    nv.wn_bytes = m.blob.n * 4l
    if (!vk_range_ok(max(m.qblob.n, nv.wn_bytes)) || !vk_range_ok(nv.ndec * z.bkv)) {
        return nv_decline(VulkanNmtDecline.memory)
    }
    nv.key = key
    nv_upload_weights(m)
    nv.wn_nv = nv_dev_buf(nv.wn_bytes)
    nv.ids_nv = nv_dev_buf(NMT_DEVICE_SRC_CAP * 4l)
    nv.pos_nv = nv_dev_buf(z.bpos)
    nv.x_nv = nv_dev_buf(z.bx)
    nv.xq_nv = nv_dev_buf(z.bq)
    nv.xs_nv = nv_dev_buf(z.bs)
    nv.h_nv = nv_dev_buf(z.bh)
    nv.y_nv = nv_dev_buf(z.bx)
    nv.q_nv = nv_dev_buf(nv.d * 4l)
    nv.at_nv = nv_dev_buf(z.bx)
    nv.k_nv = nv_dev_buf(z.bkv)
    nv.v_nv = nv_dev_buf(z.bkv)
    nv.ck_nv = nv_dev_buf(nv.ndec * z.bkv)
    nv.cv_nv = nv_dev_buf(nv.ndec * z.bkv)
    nv.pms_nv = nv_dev_buf(z.bpms)
    nv.py_nv = nv_dev_buf(z.bpy)
    nv.cell_nv = nv_dev_buf(nv.ndec * nv.d * 4l)
    nv.st_nv = nv_dev_buf(int64(NMT_ST_WORDS) * 4l)
    nv.toks_nv = nv_dev_buf(NMT_MAX_LEN_FACTOR * NMT_DEVICE_SRC_CAP * 4l)
    nv.lg_nv = nv_dev_buf(nv.nvocab * 4l)
    nv.hin = soft_host_buf(nv.oom, NMT_DEVICE_SRC_CAP * 4l + int64(NMT_ST_WORDS) * 4l + z.bpos, false)
    nv.hout = soft_host_buf(nv.oom, int64(NMT_ST_WORDS) * 4l + NMT_MAX_LEN_FACTOR * NMT_DEVICE_SRC_CAP * 4l, true)
    if (nv.oom) {
        nv_release()
        return nv_decline(VulkanNmtDecline.memory)
    }
    upload_float_ranges(nv.wn_nv, [(p = plane_at(m.blob, 0l), n = m.blob.n)])
    nv_sets()
    return true
}

[cold_path]
def private nv_announce {
    g_nv.served_said = true
    to_log(LOG_INFO, "dasLLAMA vulkan nmt: the translation family serves on the device (d {g_nv.d}, {g_nv.nenc}+{g_nv.ndec} layers)\n")
}

//! `nelem` floats of the set's source as the GEMVs' Q8_0 feed
def private nv_rq(raw : VkCommandBuffer; var h : VkHaz; var s : VkDescriptorSet; nelem : int64) {
    var pc = RqArgs(inbase = 0u, nblk = uint(nelem / kq_elems(KqFmt.q8)), lo = -VT_NO_CLAMP, hi = VT_NO_CLAMP)
    enc_tower_clamp_rq_cls(raw, h, s, pc, rq_wgs(false, nelem))
}

//! the post-norm seam over `rows` rows: x = LN(x + y (+ the bias row at `boff`)), the norm's rows at `ln_s` / `ln_b`, then x's feed
def private nv_seam(raw : VkCommandBuffer; var h : VkHaz; biased : bool; rows, boff, ln_s, ln_b : int64) {
    var r & = g_nv
    var pc = gk_add_ln_args(uint(r.d), uint(rows), NMT_LN_EPS, uint(boff), true, uint(ln_s), uint(ln_b))
    if (biased) {
        enc_tower_post_add_ln_cls(raw, h, r.s_seam, pc, rows)
    } else {
        enc_tower_post_add_ln_nb_cls(raw, h, r.s_seam_nb, pc, rows)
    }
    nv_rq(raw, h, r.s_rq_x, rows * r.d)
}

//! the ReLU FFN over `rows` rows with its seam: x = LN(x + W2 relu(W1 x + b1) + b2)
def private nv_ffn(raw : VkCommandBuffer; var h : VkHaz; rows : int64; site_w1 : int; b1, b2, ln_s, ln_b : int64) {
    var r & = g_nv
    q8_gemv(raw, h, g_nv.s_site[site_w1], r.d, r.ff, 1l, rows)
    var pb = GkBiasActArgs(d = uint(r.ff), nelem = uint(rows * r.ff), boff = uint(b1), act = BIAS_ACT_RELU, xoff = 0u)
    enc_tower_bias_act_cls(raw, h, r.s_bias, pb, (rows * r.ff / 4l + 255l) / 256l)
    nv_rq(raw, h, r.s_rq_h, rows * r.ff)
    q8_gemv(raw, h, g_nv.s_site[site_w1 + 1], r.ff, r.d, 1l, rows)
    nv_seam(raw, h, true, rows, b2, ln_s, ln_b)
}

//! the attention of `nrows` query rows `dim` apart over `len` keys - the K/V planes at element `kvoff`, the q bias row at `qboff`
//! folded - into `at`, then its feed
def private nv_attn(raw : VkCommandBuffer; var h : VkHaz; var s_part : VkDescriptorSet; dim, len, nrows, kvoff, qboff : int64) {
    var r & = g_nv
    let nch = (len + int64(WDEC_CHUNK) - 1l) / int64(WDEC_CHUNK)
    var pa = GkWdecAttnArgs(dim = uint(dim), tstride = uint(len), len0 = uint(len), causal = 0u, nheads = uint(r.hh), nrows = uint(nrows),
        koff = uint(kvoff), voff = uint(kvoff), qboff = uint(qboff), hs = uint(r.hs), scale = 1.0 / sqrt(float(r.hs)))
    enc_nmt_attn_part_cls(raw, h, s_part, pa, nch * r.hh * nrows)
    var pc = GkWdecCombArgs(dim = uint(r.d), nchunks = uint(nch), nheads = uint(r.hh), nrows = uint(nrows), hs = uint(r.hs))
    enc_nmt_attn_comb_cls(raw, h, r.s_comb, pc, r.hh * nrows)
    nv_rq(raw, h, r.s_rq_at, nrows * r.d)
}

//! `rows` source rows' K or V slice at column `soff` of rows `sstride` wide into the f16 plane at element `dst_off`, the bias row
//! at `boff` folded
def private nv_kv(raw : VkCommandBuffer; var h : VkHaz; var s : VkDescriptorSet; rows, sstride, soff, dst_off, boff : int64) {
    var r & = g_nv
    var pc = GkWdecKvArgs(rows = uint(rows), sstride = uint(sstride), soff = uint(soff), nheads = uint(r.hh), tstride = uint(rows), p0 = 0u,
        dst_off = uint(dst_off), boff = uint(boff), bias_on = 1u, mode = WDEC_KV_F16, hs = uint(r.hs), kscale = 1.0)
    enc_tower_wdec_kv16_cls(raw, h, s, pc, (rows * r.d + 255l) / 256l)
}

//! the sentence's inputs landed off `hin` (the ids, the starting state words, the position rows), the cells zeroed, the encode, then
//! every decoder layer's cross keys and values (staged as halves, biases folded)
def private nv_encode(raw : VkCommandBuffer; var h : VkHaz; m : NmtModel; s, npos : int64) {
    var r & = g_nv
    let d = r.d
    let st_at = NMT_DEVICE_SRC_CAP * 4l
    vhz_dep(raw, h, 0u, NV_IDS | NV_POS | NV_ST | NV_CELL, true)
    cmd_copy_range(raw, r.hin.buf, 0l, r.ids_nv, 0l, s * 4l)
    cmd_copy_range(raw, r.hin.buf, st_at, r.st_nv, 0l, int64(NMT_ST_WORDS) * 4l)
    cmd_copy_range(raw, r.hin.buf, st_at + int64(NMT_ST_WORDS) * 4l, r.pos_nv, 0l, npos * d * 4l)
    cmd_fill(raw, r.cell_nv, 0l, r.ndec * d * 4l, 0u)
    var pe = GkNmtEmbedArgs(d = uint(d), rows = uint(s), emb_off = uint(m.emb_off), scale = sqrt(float(d)), from_state = 0u)
    enc_nmt_embed_cls(raw, h, r.s_embed_src, pe, (s * d + 255l) / 256l)
    nv_rq(raw, h, r.s_rq_x, s * d)
    for (l in range64(r.nenc)) {
        let o = nmt_enc_offsets(m, l)
        q8_gemv(raw, h, g_nv.s_site[nv_enc_site(l, 0l)], d, 3l * d, 1l, s)
        nv_kv(raw, h, r.s_kv_k, s, 3l * d, d, 0l, o.bk)
        nv_kv(raw, h, r.s_kv_v, s, 3l * d, 2l * d, 0l, o.bv)
        nv_attn(raw, h, r.s_part_enc, 3l * d, s, s, 0l, o.bq)
        q8_gemv(raw, h, g_nv.s_site[nv_enc_site(l, 1l)], d, d, 1l, s)
        nv_seam(raw, h, true, s, o.bo, o.ln1_s, o.ln1_b)
        nv_ffn(raw, h, s, nv_enc_site(l, 2l), o.b1, o.b2, o.ln2_s, o.ln2_b)
    }
    for (l in range64(r.ndec)) {
        let o = nmt_dec_offsets(m, l)
        q8_gemv(raw, h, g_nv.s_site[nv_dec_site(l, 0l)], d, 2l * d, 1l, s)
        nv_kv(raw, h, r.s_ckv_k, s, 2l * d, 0l, l * d * s, o.bk)
        nv_kv(raw, h, r.s_ckv_v, s, 2l * d, d, l * d * s, o.bv)
    }
}

//! one greedy step: the embed off the state words, every decoder layer, the classifier and the pick
def private nv_step(raw : VkCommandBuffer; var h : VkHaz; m : NmtModel; s, cap : int64) {
    var r & = g_nv
    let d = r.d
    var pe = GkNmtEmbedArgs(d = uint(d), rows = 1u, emb_off = uint(m.emb_off), scale = sqrt(float(d)), from_state = 1u)
    enc_nmt_embed_cls(raw, h, r.s_embed_step, pe, (d + 255l) / 256l)
    nv_rq(raw, h, r.s_rq_x, d)
    for (l in range64(r.ndec)) {
        let o = nmt_dec_offsets(m, l)
        q8_gemv(raw, h, g_nv.s_site[nv_dec_site(l, 1l)], d, 2l * d, 1l, 1l)
        var ps = GkNmtSsruArgs(d = uint(d), bf_off = uint(o.bf), cell_off = uint(l * d))
        enc_nmt_ssru_cls(raw, h, r.s_ssru, ps, (d + 255l) / 256l)
        nv_seam(raw, h, false, 1l, 0l, o.rnn_ln_s, o.rnn_ln_b)
        q8_gemv(raw, h, g_nv.s_site[nv_dec_site(l, 2l)], d, d, 1l, 1l)
        nv_attn(raw, h, r.s_part_dec, d, s, 1l, l * d * s, o.bq)
        q8_gemv(raw, h, g_nv.s_site[nv_dec_site(l, 3l)], d, d, 1l, 1l)
        nv_seam(raw, h, true, 1l, o.bo, o.ctx_ln_s, o.ctx_ln_b)
        nv_ffn(raw, h, 1l, nv_dec_site(l, 4l), o.b1, o.b2, o.ffn_ln_s, o.ffn_ln_b)
    }
    q8_gemv(raw, h, g_nv.s_site[nv_cls_site()], d, r.nvocab, 1l, 1l)
    var pp = GkNmtPickArgs(nv = uint(r.nvocab), boff = uint(m.out_b), eos = uint(m.eos_id), cap = uint(cap))
    enc_nmt_pick_cls(raw, h, r.s_pick, pp, 1l)
}

//! the sentence's ids, starting state words and position rows into `hin`
def private nv_stage_inputs(st : NmtState; src : array<int64>; npos : int64) {
    let r & = g_nv
    unsafe {
        var ip = reinterpret<uint?>(r.hin.mapped)
        for (i, id in count(), src) {
            ip[i] = uint(id)
        }
        var sp = ip + NMT_DEVICE_SRC_CAP
        sp[NMT_ST_T] = 0u
        sp[NMT_ST_PREV] = NMT_NO_TOKEN
        sp[NMT_ST_DONE] = 0u
        sp[NMT_ST_NTOK] = 0u
        memcpy(reinterpret<void?>(sp + int64(NMT_ST_WORDS)), addr(st.pos[0]), npos * r.d * 4l)
    }
}

//! The whole sentence on the device: the encode and every decoder layer's cross keys and values in one command buffer, then
//! command buffers of greedy steps - the argmax and the state on the device - until the pick marks the sentence done.
def private vulkan_nmt_translate(m : NmtModel; var st : NmtState; src : array<int64>; var out : array<int64>) : bool {
    if (!g_nv_env) {
        return nv_decline(VulkanNmtDecline.knob)
    }
    if (!m.q8) {
        return nv_decline(VulkanNmtDecline.quant_mode)
    }
    if (!nmt_device_shape_ok(m)) {
        return nv_decline(VulkanNmtDecline.shape)
    }
    let s = long_length(src)
    if (s < 1l || s > NMT_DEVICE_SRC_CAP) {
        return nv_decline(VulkanNmtDecline.length)
    }
    if (!vulkan_tower_arms() || !nv_classes()) {
        return nv_decline(VulkanNmtDecline.device)
    }
    if (!nv_attach(m)) {
        return false
    }
    let te0 = ref_time_ticks()
    var us_chains = 0l
    let cap = NMT_MAX_LEN_FACTOR * s
    let npos = max(s, cap)
    nmt_positions(m, st, npos)
    nv_stage_inputs(st, src, npos)
    let r & = g_nv
    var t0 = ref_time_ticks()
    vt_chain_one_shot("bergamot encode", m.n_enc, s) $(raw : VkCommandBuffer; var h : VkHaz) {
        nv_encode(raw, h, m, s, npos)
    }
    us_chains += int64(get_time_usec(t0))
    var steps = 0l
    var cbs = 1l
    let words = unsafe(reinterpret<uint const?>(r.hout.mapped))
    while (steps < cap) {
        let n = min(steps == 0l ? int64(double(s) * g_nv_rate) + 1l : NV_TAIL_STEPS, cap - steps)
        t0 = ref_time_ticks()
        vt_chain_one_shot("bergamot steps", m.n_dec, n) $(raw : VkCommandBuffer; var h : VkHaz) {
            for (_t in range64(n)) {
                nv_step(raw, h, m, s, cap)
            }
            vhz_dep(raw, h, NV_ST | NV_TOKS, 0u, true)
            cmd_copy_range(raw, r.st_nv, 0l, r.hout.buf, 0l, int64(NMT_ST_WORDS) * 4l)
            cmd_copy_range(raw, r.toks_nv, 0l, r.hout.buf, int64(NMT_ST_WORDS) * 4l, cap * 4l)
        }
        us_chains += int64(get_time_usec(t0))
        cbs++
        steps += n
        break if (unsafe(words[NMT_ST_DONE]) != 0u)
    }
    let nt = int64(unsafe(words[NMT_ST_NTOK]))
    out |> reserve(nt)
    for (i in range64(nt)) {
        out |> push(int64(unsafe(words[int64(NMT_ST_WORDS) + i])))
    }
    if (!r.served_said) {
        nv_announce()
    }
    g_nv_sentences++
    g_nv_steps += steps
    g_nv_cbs += cbs
    g_nv_tokens += nt
    g_nv_rate = 0.9lf * g_nv_rate + 0.1lf * double(nt + 1l) / double(s)
    g_nv_us_gpu += us_chains
    g_nv_us_encode += int64(get_time_usec(te0)) - us_chains
    return true
}

[init]
def private dasllama_vulkan_nmt_register {
    register_vk_drop_hook(@@nv_forget)
    static_if (!typeinfo builtin_module_exists(das_metal)) {
        register_nmt_gpu(@@vulkan_nmt_translate)
    }
}
