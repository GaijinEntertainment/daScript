options gen2
options indenting = 4
options _dasllama_internal = true

module dasllama_vulkan_mtp_gemma shared private

require dasllama/dasllama_lint public
require math
require vulkan
require dasllama/dasllama_common
require dasllama/dasllama_math   // grow_resize - the scratch sizing
require dasllama/dasllama_kqformat
require dasllama/dasllama_mtp_gemma
require dasllama/dasllama_gpu_math   // ACT_GELU
require dasllama/dasllama_gpu_kernels_common   // gk_rope_kv_args, GkQkRmsArgs
require dasllama/dasllama_gpu_resident   // the tier's assistant seams: the bind, the N-row verify, the commit, the rope row
require dasllama/dasllama_vulkan_common
require dasllama/dasllama_vulkan_classes
require dasllama/dasllama_vulkan_decode   // vk_rdec_carry_arm, rd_chain_settle

// The gemma-4 assistant drafter on the Vulkan resident driver (`dasllama_metal_mtp_gemma.das`'s twin): the drafter's q8 planes
// and its own activation planes on the device, a recorded draft command a (region, depth) that chains the drafts over the
// target's K/V mirror at the two capture layers, and the speculative rounds of a tick's streams verified on the N-row command.

let private B_CAT = 0x1u      //! [embed(tok) * sqrt(tdim) ; h], the pre-projection's input
let private B_CATQ = 0x2u     //! ... its Q8_0 feed
let private B_XIN = 0x4u      //! the drafter's residual row
let private B_XQ = 0x8u       //! a normed row's Q8_0 feed
let private B_Q = 0x10u       //! the layer's query row
let private B_ATT = 0x20u     //! the attention out and its partials
let private B_AQ = 0x40u      //! the attention out's Q8_0 feed
let private B_O = 0x80u       //! a branch's out row
let private B_GATE = 0x100u
let private B_UP = 0x200u
let private B_HQ = 0x400u     //! the act row's Q8_0 feed
let private B_LOG = 0x800u    //! the logits and the pick's partials
let private B_IDS = 0x1000u   //! the step's token id
let private B_HN = 0x2000u    //! the post-projection row, the next step's h
let private B_COS = 0x4000u   //! the anchor's rope row

//! the drafter's q8 GEMV sites: the three step projections, then five a layer
let private G_PRE = 0
let private G_POST = 1
let private G_CLS = 2
let private G_LAYER0 = 3
let private G_PER_LAYER = 5

def private g_layer(l : int64; site : int) : int => G_LAYER0 + int(l) * G_PER_LAYER + site

//! one q8 GEMV site: its set over the drafter's planes and its region meta (the tensor's first block)
struct private GemmaVkGemv {
    set_ : VkDescriptorSet
    meta : HostBuf
    n : int64
    d : int64
}

//! The attached drafter on the Vulkan driver: its planes, activation planes and sets, the recorded draft commands
struct private GemmaDrafterVk {
    ready : bool
    wq : uint64                 // the q8 quant words, every tensor's blocks one after another
    ws : uint64                 // ... their f16 scales
    wqb : int64
    wsb : int64
    fblob : uint64              // the drafter's f32 rows: the norms, the q norms, the rope table
    fblob_bytes : int64
    cat : uint64
    catq : uint64
    cats : uint64
    xin : uint64
    xq : uint64
    xs : uint64
    q : uint64
    attn : uint64
    part : uint64
    part_bytes : int64
    cntoff : int64
    aq : uint64
    aqs : uint64
    o : uint64
    gate : uint64
    up : uint64
    hq : uint64
    hqs : uint64
    logits : uint64
    hnext : uint64
    ids : uint64
    pmax : uint64
    pid : uint64
    tok : HostBuf               //! one TokMeta: the key count the attention reads, the anchor
    cos : HostBuf               //! the anchor's cos/sin plane
    inp : HostBuf               //! the seed id at word 0, the carried hidden from byte 16
    picks : HostBuf             //! the chain's ids: the seed, then each draft
    lout : HostBuf              //! the last step's logits and h_next, read back for the oracle
    gemvs : array<GemmaVkGemv>
    s_cat_rq : VkDescriptorSet
    s_ar : VkDescriptorSet
    s_actrq : VkDescriptorSet
    s_qkn : VkDescriptorSet
    s_rope : VkDescriptorSet
    s_emb : VkDescriptorSet
    s_pick_part : VkDescriptorSet
    s_pick_fin : VkDescriptorSet
    s_attn : array<VkDescriptorSet>   // a layer's, over its capture layer's slab
    s_tq4 : VkDescriptorSet
    s_unrot : VkDescriptorSet
    s_at_rq : VkDescriptorSet
    cmds : array<VkCommandBuffer>     // a (region, depth) each
    recorded : array<bool>
    hs_mx : int64
}

var private g_vk_dr : GemmaDrafter
var private g_vk_g : GemmaDrafterVk
var private g_vk_attached = false
var private g_vk_drafted = 0l
var private g_vk_accepted = 0l
var private g_vk_rounds = 0l
var private g_vk_joint = 0l
var private g_vk_us_draft = 0l
var private g_vk_us_verify = 0l
var private g_vk_us_cold = 0l

let private DRAFT_K_MAX = MTP_MAX_ROWS - 1l

//! the q8 tensors of the drafter in the planes' order: (byte offset in the GGUF blob, elements) - the GEMV sites' order
def private q8_tensors(dr : GemmaDrafter) : array<tuple<boff : int64; n : int64>> {
    var out <- [(boff = dr.pre_proj_off, n = 2l * dr.tdim * dr.dim), (boff = dr.post_proj_off, n = dr.dim * dr.tdim),
        (boff = dr.tok_emb_off, n = dr.dim * dr.vocab)]
    out |> reserve(3l + 5l * dr.n_layers)
    for (l in range64(dr.n_layers)) {
        let qd = dr.n_heads * dr.hs[l]
        out |> push((boff = dr.wq_offs[l], n = dr.dim * qd))
        out |> push((boff = dr.wo_offs[l], n = qd * dr.dim))
        out |> push((boff = dr.w1_offs[l], n = dr.dim * dr.hidden))
        out |> push((boff = dr.w3_offs[l], n = dr.dim * dr.hidden))
        out |> push((boff = dr.w2_offs[l], n = dr.hidden * dr.dim))
    }
    return <- out
}

//! the GGUF's 34-byte Q8_0 blocks split into the device plane pair the q8 GEMV reads: 32 quant bytes a block, an f16 scale a
//! block; each tensor's first block index back, in `q8_tensors` order
def private q8_repack(dr : GemmaDrafter; var quants : array<uint8>; var scales : array<uint16>) : array<int64> {
    var tensors <- q8_tensors(dr)
    var nblk = 0l
    for (te in tensors) {
        nblk += te.n / 32l
    }
    quants |> reserve(nblk * 32l)
    quants |> resize(nblk * 32l)
    scales |> reserve(nblk)
    scales |> resize(nblk)
    var bases : array<int64>
    bases |> reserve(long_length(tensors))
    var cur = 0l
    for (te in tensors) {
        bases |> push(cur)
        unsafe {
            let src = addr<uint8 const?>(dr.blob[te.boff])
            for (b in range64(te.n / 32l)) {
                let bp = src + b * 34l
                scales[cur] = uint16(uint(bp[0]) | (uint(bp[1]) << 8u))
                memcpy(addr<void?>(quants[cur * 32l]), reinterpret<void?>(bp + 2l), 32l)
                cur++
            }
        }
    }
    delete tensors
    return <- bases
}

//! a device buffer of `bytes` made soft: 0 marks `oom`
def private dev_buf(var oom : bool&; bytes : int64) : uint64 => soft_device_buf(oom, bytes)

//! `bytes` of `src` into a fresh soft device buffer (0 and `oom` where the card has no room)
def private dev_from(var oom : bool&; src : uint8 const?; bytes : int64) : uint64 {
    let h = soft_device_buf(oom, bytes)
    if (h != 0ul) {
        upload_region_at(h, 0l, src, bytes)
    }
    return h
}

//! every handle the drafter held dropped, nothing destroyed: the model drop already swept them
def private vk_forget(var g : GemmaDrafterVk) {
    g.gemvs |> clear()
    g.s_attn |> clear()
    g.cmds |> clear()   //! model-owned commands: the model drop frees them
    g.recorded |> clear()
    g.wq = 0ul; g.ws = 0ul; g.fblob = 0ul; g.cat = 0ul; g.catq = 0ul; g.cats = 0ul; g.xin = 0ul; g.xq = 0ul; g.xs = 0ul
    g.q = 0ul; g.attn = 0ul; g.part = 0ul; g.aq = 0ul; g.aqs = 0ul; g.o = 0ul; g.gate = 0ul; g.up = 0ul; g.hq = 0ul; g.hqs = 0ul
    g.logits = 0ul; g.hnext = 0ul; g.ids = 0ul; g.pmax = 0ul; g.pid = 0ul
    g.tok = HostBuf(); g.cos = HostBuf(); g.inp = HostBuf(); g.picks = HostBuf(); g.lout = HostBuf()
    g.ready = false
}

def private vk_buffers_release(var g : GemmaDrafterVk) {
    release_device_buf(g.wq)
    release_device_buf(g.ws)
    release_device_buf(g.fblob)
    release_device_buf(g.cat)
    release_device_buf(g.catq)
    release_device_buf(g.cats)
    release_device_buf(g.xin)
    release_device_buf(g.xq)
    release_device_buf(g.xs)
    release_device_buf(g.q)
    release_device_buf(g.attn)
    release_device_buf(g.part)
    release_device_buf(g.aq)
    release_device_buf(g.aqs)
    release_device_buf(g.o)
    release_device_buf(g.gate)
    release_device_buf(g.up)
    release_device_buf(g.hq)
    release_device_buf(g.hqs)
    release_device_buf(g.logits)
    release_device_buf(g.hnext)
    release_device_buf(g.ids)
    release_device_buf(g.pmax)
    release_device_buf(g.pid)
    for (hb in [g.tok, g.cos, g.inp, g.picks, g.lout]) {
        if (hb.buf != 0ul) {
            destroy_host_buf(hb)
        }
    }
    for (gv in g.gemvs) {
        if (gv.meta.buf != 0ul) {
            destroy_host_buf(gv.meta)
        }
    }
    vk_forget(g)
}

//! the classes the drafter's command dispatches, compiled on the live device
def private vk_classes_ok(dt : KVDtype; slabs : array<DaSlab>) : bool {
    var ok = (ensure_cls_ar_rq_b() && ensure_cls_dn_rq() && ensure_q8_actrq_cls() && ensure_qk_rms_cls() && ensure_rope_kv_cls()
        && gemv_cls_ensure(int(KqFmt.q8)) && ensure_cls_argmax_part() && ensure_cls_argmax_fin()
        && (g_rd.emb_blk >= 0l ? emb_cls_ensure(g_rd.emb_fmt) : ensure_emb_gather_f32_cls())
        && (dt != KVDtype.tq4 || (ensure_kv_tq4_store_cls() && ensure_kv_tq4_unrot_cls())))
    for (sl in slabs) {
        ok = ok && da_attn_stamp_ensure(dt, sl)
    }
    return ok
}

//! why the armed driver cannot take the drafter ("" = it can): the device must hold the trunk whole on the resident driver,
//! gather its token table, run the N-row command a round's verify rides, and hold the capture layers' K/V rows itself
def private vk_target_why(dr : GemmaDrafter) : string {
    if (!moe_gpu_resident_active() || g_rd == null || !g_rd.ready) {
        return "the Vulkan resident driver does not serve the trunk whole (the load log names why)"
    }
    if (g_rd.hc) {
        return "a hyper-connection trunk"
    }
    if (g_rd.emb_blk < 0l && g_rd.emb_f32 == 0ul) {
        return "the device holds no token table to gather the seed rows from"
    }
    if (gpu_resident_assist_rows() < 2l) {
        return "the N-row token command declined the trunk (its line above), and the round's verify rides it"
    }
    for (l in range64(dr.n_layers)) {
        let kvl = dr.kv_layer[l]
        if (kvl < 0l || kvl >= long_length(g_rd.layers) || g_rd.layers[kvl].mir_idx < 0l || g_rd.layers[kvl].kv_shared) {
            return "the capture layer {kvl} keeps no K/V rows of its own on the device"
        }
    }
    return ""
}

def private gemv_set(g : GemmaDrafterVk; base, n, d : int64; xq, xs : uint64; xbit : uint; y : uint64; ybit : uint) : GemmaVkGemv {
    var gv = GemmaVkGemv(meta = make_host_buf(META_BYTES, true), n = n, d = d)
    unsafe {
        var mp = reinterpret<uint?>(gv.meta.mapped)
        mp[0] = uint(base)
        mp[1] = 0u
    }
    gv.set_ = gemv_planes_set(int(KqFmt.q8), (wq = g.wq, ws = g.ws, wqb = g.wqb, wsb = g.wsb), gv.meta.buf, META_BYTES, 0u,
        xq, xs, n, (n / 32l) * 4l, xbit, y, d * 4l, ybit)
    return gv
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_MTP.md#resident-assistant-drafter")]
def private vk_drafter_init(dr : GemmaDrafter; var g : GemmaDrafterVk) : bool {
    var hs_mx = 0l
    for (h in dr.hs) {
        hs_mx = max(hs_mx, h)
    }
    g.hs_mx = hs_mx
    let dt = g_rd.kv_dt
    var slabs <- [for (l in range64(dr.n_layers)); da_slab_of(false, dr.n_heads / dr.kvh[l])]
    if (!vk_classes_ok(dt, slabs)) {
        return false
    }
    var quants : array<uint8>
    var scales : array<uint16>
    var bases <- q8_repack(dr, quants, scales)
    let made = vk_buffers_make(dr, g, quants, scales)
    delete quants
    delete scales
    if (!made) {
        delete bases
        delete slabs
        vk_buffers_release(g)
        return false
    }
    g.gemvs <- vk_gemvs_make(dr, g, bases)
    delete bases
    vk_sets_make(dr, g, slabs)
    delete slabs
    let ncmd = g_rd.regions * (DRAFT_K_MAX + 1l)
    g.cmds |> reserve(ncmd)
    for (_i in range64(ncmd)) {
        g.cmds |> push(alloc_cmd())
    }
    g.recorded |> resize(ncmd)
    g.ready = true
    return true
}

//! the drafter's weight planes (the repacked q8 `quants` / `scales`, the f32 blob) and scratch on the device and its host
//! buffers; false where the card has no room
def private vk_buffers_make(dr : GemmaDrafter; var g : GemmaDrafterVk; quants : array<uint8>; scales : array<uint16>) : bool {
    var oom = false
    g.wqb = long_length(quants)
    g.wsb = long_length(scales) * 2l
    g.wq = dev_from(oom, unsafe(addr<uint8 const?>(quants[0])), g.wqb)
    g.ws = dev_from(oom, unsafe(addr<uint8 const?>(scales[0])), g.wsb)
    g.fblob_bytes = long_length(dr.fblob) * 4l
    g.fblob = dev_from(oom, unsafe(addr<uint8 const?>(dr.fblob[0])), g.fblob_bytes)
    let qdm = dr.n_heads * g.hs_mx
    let cat_n = 2l * dr.tdim
    g.cat = dev_buf(oom, cat_n * 4l)
    g.catq = dev_buf(oom, cat_n)
    g.cats = dev_buf(oom, (cat_n / 32l) * 4l)
    g.xin = dev_buf(oom, dr.dim * 4l)
    g.xq = dev_buf(oom, dr.dim)
    g.xs = dev_buf(oom, (dr.dim / 32l) * 4l)
    g.q = dev_buf(oom, qdm * 4l)
    g.attn = dev_buf(oom, qdm * 4l)
    g.cntoff = dr.n_heads * (g.hs_mx + 2l)   //! one key split: the counters past one piece a head
    g.part_bytes = (g.cntoff + dr.n_heads) * 4l
    g.part = dev_buf(oom, g.part_bytes)
    g.aq = dev_buf(oom, qdm)
    g.aqs = dev_buf(oom, (qdm / 32l) * 4l)
    g.o = dev_buf(oom, dr.dim * 4l)
    g.gate = dev_buf(oom, dr.hidden * 4l)
    g.up = dev_buf(oom, dr.hidden * 4l)
    g.hq = dev_buf(oom, dr.hidden)
    g.hqs = dev_buf(oom, (dr.hidden / 32l) * 4l)
    g.logits = dev_buf(oom, dr.vocab * 4l)
    g.hnext = dev_buf(oom, dr.tdim * 4l)
    g.ids = dev_buf(oom, 16l)
    let pbytes = int64(CLS_ARGMAX_CHUNKS) * 4l
    g.pmax = dev_buf(oom, pbytes)
    g.pid = dev_buf(oom, pbytes)
    return false if (oom)
    upload_zeros(g.part, 0l, g.part_bytes)
    g.tok = make_host_buf(TOK_META_BYTES, true)
    g.cos = make_host_buf(g_rd.cos_elems * 4l, true)
    g.inp = make_host_buf(16l + dr.tdim * 4l, false)
    g.picks = make_host_buf(MTP_MAX_ROWS * 4l, false, [cached = true])
    g.lout = make_host_buf((dr.vocab + dr.tdim) * 4l, false, [cached = true])
    return true
}

//! the q8 GEMV sites in the command's order: pre-proj, post-proj, classifier, then each layer's q, o, gate, up, down
def private vk_gemvs_make(dr : GemmaDrafter; g : GemmaDrafterVk; bases : array<int64>) : array<GemmaVkGemv> {
    let cat_n = 2l * dr.tdim
    var out <- [gemv_set(g, bases[0], cat_n, dr.dim, g.catq, g.cats, B_CATQ, g.xin, B_XIN),
        gemv_set(g, bases[1], dr.dim, dr.tdim, g.xq, g.xs, B_XQ, g.hnext, B_HN),
        gemv_set(g, bases[2], dr.dim, dr.vocab, g.xq, g.xs, B_XQ, g.logits, B_LOG)]
    out |> reserve(3l + 5l * dr.n_layers)
    for (l in range64(dr.n_layers)) {
        let qd = dr.n_heads * dr.hs[l]
        let b0 = 3l + l * 5l
        out |> push(gemv_set(g, bases[b0], dr.dim, qd, g.xq, g.xs, B_XQ, g.q, B_Q))
        out |> push(gemv_set(g, bases[b0 + 1l], qd, dr.dim, g.aq, g.aqs, B_AQ, g.o, B_O))
        out |> push(gemv_set(g, bases[b0 + 2l], dr.dim, dr.hidden, g.xq, g.xs, B_XQ, g.gate, B_GATE))
        out |> push(gemv_set(g, bases[b0 + 3l], dr.dim, dr.hidden, g.xq, g.xs, B_XQ, g.up, B_UP))
        out |> push(gemv_set(g, bases[b0 + 4l], dr.hidden, dr.dim, g.hq, g.hqs, B_HQ, g.o, B_O))
    }
    return <- out
}

//! the descriptor sets of every non-GEMV class the command dispatches
def private vk_sets_make(dr : GemmaDrafter; var g : GemmaDrafterVk; slabs : array<DaSlab>) {
    let dt = g_rd.kv_dt
    let qdm = dr.n_heads * g.hs_mx
    let cat_n = 2l * dr.tdim
    let pbytes = int64(CLS_ARGMAX_CHUNKS) * 4l
    g.s_cat_rq = set_rq_cls(fixed_array(g.cat, g.catq, g.cats), fixed_array(cat_n * 4l, cat_n, (cat_n / 32l) * 4l), fixed_array(B_CAT, B_CATQ, B_CATQ))
    g.s_ar = set_cls_ar_rq_b(fixed_array(g.xin, g.o, g.fblob, g.xq, g.xs), fixed_array(dr.dim * 4l, dr.dim * 4l, g.fblob_bytes, dr.dim, (dr.dim / 32l) * 4l),
        fixed_array(B_XIN, B_O, 0u, B_XQ, B_XQ))
    g.s_actrq = set_actrq_cls(fixed_array(g.gate, g.up, g.hq, g.hqs), fixed_array(dr.hidden * 4l, dr.hidden * 4l, dr.hidden, (dr.hidden / 32l) * 4l),
        fixed_array(B_GATE, B_UP, B_HQ, B_HQ))
    g.s_qkn = set_qk_rms_cls(fixed_array(g.q, g.q, g.fblob), fixed_array(qdm * 4l, qdm * 4l, g.fblob_bytes), fixed_array(B_Q, B_Q, 0u))
    let cosb = g_rd.cos_elems * 4l
    g.s_rope = set_rope_kv_cls(fixed_array(g.q, g.q, g.q, g.q, g.q, g.q, g.cos.buf, g.cos.buf, g.tok.buf),
        fixed_array(qdm * 4l, qdm * 4l, qdm * 4l, qdm * 4l, qdm * 4l, qdm * 4l, cosb, cosb, TOK_META_BYTES),
        fixed_array(B_Q, B_Q, B_Q, B_Q, B_Q, 0u, B_COS, B_COS, 0u))
    if (g_rd.emb_blk >= 0l) {
        g.s_emb = emb_cls_set(g_rd.emb_fmt, arena_planes(g_rd.emb_fmt, g_rd.emb_blk), g.ids, 16l, B_IDS, g.cat, cat_n * 4l, g.cat, cat_n * 4l, B_CAT)
    } else {
        g.s_emb = emb_f32_set(g_rd.emb_f32, g_rd.emb_f32_bytes, g.ids, 16l, B_IDS, g.cat, cat_n * 4l, g.cat, cat_n * 4l, B_CAT)
    }
    g.s_pick_part = set_cls_argmax_part(fixed_array(g.logits, g.pmax, g.pid), fixed_array(dr.vocab * 4l, pbytes, pbytes), fixed_array(B_LOG, B_LOG, B_LOG))
    g.s_pick_fin = set_cls_argmax_fin(fixed_array(g.pmax, g.pid, g.ids), fixed_array(pbytes, pbytes, 16l), fixed_array(B_LOG, B_LOG, B_IDS))
    g.s_attn |> reserve(dr.n_layers)
    for (l in range64(dr.n_layers)) {
        let L & = unsafe(g_rd.layers[dr.kv_layer[l]])
        let (kmir, vmir) = rd_slab(L)
        let mirb = rd_mir_bytes()
        g.s_attn |> push(da_attn_stamp_set(dt, slabs[l],
            fixed_array(g.q, kmir, vmir, g.attn, g.tok.buf, g.part, g.aq, g.aqs, g.q),
            fixed_array(qdm * 4l, mirb, mirb, qdm * 4l, TOK_META_BYTES, g.part_bytes, qdm, (qdm / 32l) * 4l, qdm * 4l),
            fixed_array(B_Q, 0u, 0u, B_ATT, 0u, B_ATT, B_AQ, B_AQ, 0u)))
    }
    if (dt == KVDtype.tq4) {
        let signb = KV_TQ4_SIGNS * 4l
        g.s_tq4 = set_kv_tq4_store_cls(fixed_array(g.q, g.q, g.q, g.q, g.q, g_rd.tq4_signs_dev, g.tok.buf),
            fixed_array(qdm * 4l, qdm * 4l, qdm * 4l, qdm * 4l, qdm * 4l, signb, TOK_META_BYTES), fixed_array(B_Q, 0u, 0u, 0u, 0u, 0u, 0u))
        g.s_unrot = set_kv_tq4_unrot_cls(fixed_array(g.attn, g.q, g_rd.tq4_signs_dev), fixed_array(qdm * 4l, qdm * 4l, signb), fixed_array(B_ATT, B_Q, 0u))
        g.s_at_rq = set_rq_cls(fixed_array(g.attn, g.aq, g.aqs), fixed_array(qdm * 4l, qdm, (qdm / 32l) * 4l), fixed_array(B_ATT, B_AQ, B_AQ))
    }
}

//! one q8 GEMV site's dispatch
def private gemv_site(var g : GemmaDrafterVk; raw : VkCommandBuffer; var h : VkHaz; site : int) {
    var gv & = unsafe(g.gemvs[site])
    var pc = GemvArgs(n = uint(gv.n), d = uint(gv.d), nreg = 1u, ybase = 0u, ncols = 1u, ystride = uint(gv.d))
    gemv_enc(int(KqFmt.q8), raw, h, gv.set_, pc, gv.d)
}

//! the residual step and the next norm's Q8_0 feed in one dispatch: x += rms(o) . pw (under `add`), then rms(x) . w quantized
def private ar_site(var g : GemmaDrafterVk; dr : GemmaDrafter; raw : VkCommandBuffer; var h : VkHaz; add : bool; woff, pwoff : int64; ascale : float) {
    var pc = ArArgs(dim = uint(dr.dim), add_on = add ? 1u : 0u, woff = uint(woff), eps = dr.eps, ascale = ascale, pre_on = add ? 1u : 0u, pwoff = uint(pwoff))
    enc_cls_ar_rq_b(raw, h, g.s_ar, pc, 1l)
}

[arch(at="../ARCHITECTURE_GPU_MTP_ASSISTANT.md#assistant-drafter-step")]
def private vk_encode_layer(var g : GemmaDrafterVk; dr : GemmaDrafter; raw : VkCommandBuffer; var h : VkHaz; l, r : int64) {
    let hs = dr.hs[l]
    let qd = dr.n_heads * hs
    let kv_mul = dr.n_heads / dr.kvh[l]
    let L & = unsafe(g_rd.layers[dr.kv_layer[l]])
    let dt = g_rd.kv_dt
    gemv_site(g, raw, h, g_layer(l, 0))
    var pcn = GkQkRmsArgs(hs = uint(hs), nh = uint(dr.n_heads), nkvh = 0u, qstride = uint(qd), qwoff = uint(dr.q_norm_offs[l]), eps = dr.eps, qhs = uint(hs))
    enc_qk_rms_cls(raw, h, g.s_qkn, pcn, dr.n_heads)
    //! rope on the query alone: every pair a q pair, so nothing stores; the row's class rides its capture layer's rope rows
    var pcr = gk_rope_kv_args(qd, L.kvd, hs, qd / 2l, qd / 2l, g_rd.neox, false, hs, 0l, L.cbase, 1l, 0l, qd, 0l, 0l)
    enc_rope_kv_cls(raw, h, g.s_rope, pcr, (qd / 2l + int64(WG_X) - 1l) / int64(WG_X))
    if (dt == KVDtype.tq4) {   //! the query rotates into the stored rows' basis
        var pct = KvTqStoreArgs(kvd = uint(L.kvd), hs = uint(hs), nqh = uint(dr.n_heads), nkh = 0u, qrow = uint(qd), qstride = uint(hs), inv_sqrt_hs = 1.0 / sqrt(float(hs)))
        enc_kv_tq4_store_cls(raw, h, g.s_tq4, pct, dr.n_heads)
    }
    let rowwg = da_attn_row_wgs(dr.n_heads, kv_mul, 1l)
    let mirbase = uint(rd_mir_base(L, r))
    var pca = DaAttnArgs(kvd = uint(L.kvd), hs = uint(hs), kv_mul = uint(kv_mul), kbase = mirbase, vbase = mirbase, scale = 1.0, qstride = 0u, gated = 0u,
        window = dr.sliding[l] && L.window > 0l ? uint(L.window - 1l) : 0u, cap = 0.0,   //! the pass's count is the anchor, its query a row past the last key: one key short of the window nsplit = 1u, rqk = dt == KVDtype.tq4 ? DA_RQ_NONE : DA_RQ_Q8, sinkoff = 0u,
        rowwg = uint(rowwg), qrow = uint(qd), cntoff = uint(g.cntoff))
    da_attn_stamp_enc(dt, da_slab_of(false, kv_mul), raw, h, g.s_attn[l], pca, rowwg)
    if (dt == KVDtype.tq4) {
        var pcu = KvTqUnrotArgs(hs = uint(hs), nh = uint(dr.n_heads), orow = uint(qd), qrow = uint(qd), qstride = uint(hs), inv_sqrt_hs = 1.0 / sqrt(float(hs)))
        enc_kv_tq4_unrot_cls(raw, h, g.s_unrot, pcu, dr.n_heads)
        rq_enc(raw, h, g.s_at_rq, false, 0l, qd)
    }
    gemv_site(g, raw, h, g_layer(l, 1))
    ar_site(g, dr, raw, h, true, dr.ffn_norm_offs[l], dr.post_attn_norm_offs[l], 1.0)
    gemv_site(g, raw, h, g_layer(l, 2))
    gemv_site(g, raw, h, g_layer(l, 3))
    var pc_act = ActArgs(nelem = uint(dr.hidden), act = ACT_GELU)
    actrq_enc(raw, h, g.s_actrq, false, pc_act, dr.hidden)
    gemv_site(g, raw, h, g_layer(l, 4))
    let nxt = l + 1l < dr.n_layers ? dr.attn_norm_offs[l + 1l] : dr.out_norm_off
    ar_site(g, dr, raw, h, true, nxt, dr.post_ffn_norm_offs[l], dr.layer_scale[l])
}

//! the draft command of region `r` at depth `k`: the seed and the carried hidden up, then `k` chained steps - step i gathers the
//! token row its id names (the seed's at step 0, the last step's pick after it) beside h (the carry at step 0, the last step's
//! post-projection after it), runs the drafter, picks its argmax into the next step's id and lands it in `picks[i + 1]`
[arch(at="../ARCHITECTURE_GPU_MTP.md#mtp-round-one-join"), arch(at="../ARCHITECTURE_GPU_VULKAN_MTP.md#resident-assistant-drafter")]
def private vk_record_chain(var g : GemmaDrafterVk; dr : GemmaDrafter; r, k : int64) {
    var raw = g.cmds[r * (DRAFT_K_MAX + 1l) + k]
    var h = cmd_open(raw, true)
    let tb = dr.tdim * 4l
    vhz_dep(raw, h, 0u, B_IDS | B_CAT, true)
    cmd_copy_range(raw, g.inp.buf, 0l, g.ids, 0l, 4l)
    cmd_copy_range(raw, g.inp.buf, 16l, g.cat, tb, tb)
    for (i in range64(k)) {
        if (i > 0l) {
            vhz_dep(raw, h, B_HN, B_CAT, true)
            cmd_copy_range(raw, g.hnext, 0l, g.cat, tb, tb)
        }
        if (g_rd.emb_blk >= 0l) {
            emb_cls_enc(g_rd.emb_fmt, raw, h, g.s_emb, 1l, dr.tdim, arena_local_blk(g_rd.emb_blk), g_rd.emb_scale)
        } else {
            emb_f32_enc(raw, h, g.s_emb, 1l, dr.tdim, g_rd.emb_scale)
        }
        rq_enc(raw, h, g.s_cat_rq, false, 0l, 2l * dr.tdim)
        gemv_site(g, raw, h, G_PRE)
        ar_site(g, dr, raw, h, false, dr.attn_norm_offs[0], 0l, 1.0)
        for (l in range64(dr.n_layers)) {
            vk_encode_layer(g, dr, raw, h, l, r)
        }
        gemv_site(g, raw, h, G_CLS)
        var pcp = ArgmaxArgs(vocab = uint(dr.vocab))
        enc_cls_argmax_part(raw, h, g.s_pick_part, pcp, int64(CLS_ARGMAX_CHUNKS))
        enc_cls_argmax_fin(raw, h, g.s_pick_fin, pcp, 1l)
        gemv_site(g, raw, h, G_POST)
        vhz_dep(raw, h, B_IDS, 0u, true)
        cmd_copy_range(raw, g.ids, 0l, g.picks.buf, (i + 1l) * 4l, 4l)
    }
    vhz_dep(raw, h, B_LOG | B_HN, 0u, true)
    cmd_copy_range(raw, g.logits, 0l, g.lout.buf, 0l, dr.vocab * 4l)
    cmd_copy_range(raw, g.hnext, 0l, g.lout.buf, dr.vocab * 4l, tb)
    cmd_close(raw)
    g.recorded[r * (DRAFT_K_MAX + 1l) + k] = true
}

//! `k` chained drafts of the stream in region `r` from (`seed`, `h`) at the constant anchor: the drafts land in `out[1..k]`
[arch(at="../ARCHITECTURE_GPU_VULKAN_MTP.md#resident-assistant-drafter")]
def private vk_draft_chain(t : Model; r, seed, anchor, k : int64; h : array<float>; var out : array<int64>) {
    var g & = g_vk_g
    let dr & = g_vk_dr
    rd_chain_settle()
    let ci = r * (DRAFT_K_MAX + 1l) + k
    if (!g.recorded[ci]) {
        vk_record_chain(g, dr, r, k)
    }
    unsafe {
        var tm = reinterpret<TokMeta?>(g.tok.mapped)
        tm[0] = TokMeta(pos = uint(anchor), cnt = uint(anchor))
        var ip = reinterpret<uint8?>(g.inp.mapped)
        *reinterpret<uint?>(ip) = uint(seed)
        memcpy(reinterpret<void?>(ip + 16l), addr<void?>(h[0]), dr.tdim * 4l)
    }
    g_vk_cosrow |> grow_resize(g_rd.cos_elems)
    gpu_resident_rope_row(t, anchor, g_vk_cosrow, 0l)
    unsafe {
        memcpy(g.cos.mapped, addr<void?>(g_vk_cosrow[0]), g_rd.cos_elems * 4l)
    }
    submit_wait(g.cmds[ci])
    unsafe {
        let pp = reinterpret<uint?>(g.picks.mapped)
        for (i in range64(1l, k + 1l)) {
            out[i] = int64(pp[i])
        }
    }
}

var private @scratch g_vk_cosrow : array<float>

//! One drafter step on the device against the region `s` holds (the oracle cell's GPU side): the draft token and the next h
def public vk_gemma_draft_step(t : Model; var s : Session; tok, anchor : int64; h : array<float>; var out_tok : int64&; var h_next : array<float>) : bool {
    return false if (!g_vk_attached)
    g_vk_taken |> clear()
    let r = gpu_resident_assist_bind(t, s, anchor, 1l, g_vk_taken)
    return false if (r < 0l)
    g_vk_one |> resize(2)
    vk_draft_chain(t, r, tok, anchor, 1l, h, g_vk_one)
    out_tok = g_vk_one[1]
    h_next |> grow_resize(g_vk_dr.tdim)
    host_buf_read(g_vk_g.lout, unsafe(addr<void?>(h_next[0])), g_vk_dr.tdim * 4l, g_vk_dr.vocab * 4l)
    return true
}

//! The last device draft step's full-vocab logits (the oracle comparison reads them; the round never does)
def public vk_gemma_drafter_last_logits(var dst : array<float>) {
    dst |> grow_resize(g_vk_dr.vocab)
    host_buf_read(g_vk_g.lout, unsafe(addr<void?>(dst[0])), g_vk_dr.vocab * 4l)
}

var private @scratch g_vk_one : array<int64>
var private @scratch g_vk_taken : array<int64>

//! The Vulkan attach: `""` with the drafter attached to the trunk the resident driver serves whole, else why it refused. One
//! drafter at a time; the driver then lands the post-norm hidden every step (the drafter's h input)
[arch(at="../ARCHITECTURE_GPU_VULKAN_MTP.md#resident-assistant-drafter"), arch(at="../ARCHITECTURE_GPU_MTP_ASSISTANT.md#assistant-drafter-attach")]
def private vk_attach(t : Model; path : string) : string {
    vk_detach()
    if (!moe_gpu_resident_active()) {   //! before the sidecar's read: a trunk this driver does not serve takes no drafter here
        return "the Vulkan resident driver does not serve the trunk whole (the load log names why)"
    }
    var dr <- load_gemma_drafter(path, t.config.n_layers)
    if (!dr.ok) {
        let why = clone_string(dr.why)
        delete dr
        return why
    }
    var why = gemma_drafter_target_why(t, dr)
    if (empty(why)) {
        why = vk_target_why(dr)
    }
    if (empty(why) && !vk_drafter_init(dr, g_vk_g)) {
        why = "the device has no room for the drafter's {long_length(dr.blob) >> 20l} MB of planes, or a class it dispatches did not compile"
    }
    if (empty(why) && !vk_rdec_carry_arm(true)) {
        why = "the driver took no carry landing"
    }
    if (!empty(why)) {
        vk_buffers_release(g_vk_g)
        delete dr
        return why
    }
    delete g_vk_dr
    g_vk_dr <- dr
    gpu_resident_assist_set(true)
    set_mtp_round_kind(MtpRoundKind.assistant)
    g_vk_attached = true
    g_vk_drafted = 0l
    g_vk_accepted = 0l
    g_vk_rounds = 0l
    g_vk_joint = 0l
    g_vk_us_draft = 0l
    g_vk_us_verify = 0l
    g_vk_us_cold = 0l
    to_log(LOG_INFO, "dasLLAMA gemma drafter attached on the Vulkan resident driver: {path} ({g_vk_dr.n_layers} Q-only layers over target layers {g_vk_dr.kv_layer})\n")
    return ""
}

def private vk_detach {
    return if (!g_vk_attached)
    vk_rdec_carry_arm(false)
    gpu_resident_assist_set(false)
    vk_buffers_release(g_vk_g)
    delete g_vk_dr
    set_mtp_round_kind(MtpRoundKind.nextn)
    g_vk_attached = false
}

//! the model drop sweeps every device object the drafter held: it forgets them and stands detached
def private vk_drop_forget {
    return if (!g_vk_attached)
    vk_forget(g_vk_g)
    delete g_vk_dr
    set_mtp_round_kind(MtpRoundKind.nextn)
    g_vk_attached = false
}

[unused_argument(t), hot_path]   //! one drafter a process: the attach checked it against its target
def private vk_attached_to(t : Model) : bool => g_vk_attached

//! a stream that cannot speculate this round: no carried hidden at its position, a round past the context
def private vk_round_cold(t : Model; s : Session; pos, k : int64) : bool => (pos < 1l || s.mtp_h_pos1 != pos
    || long_length(s.mtp_h) < g_vk_dr.tdim || pos + k + 1l > t.config.seq_len)

//! verify row `vr`'s truth: the device's pick where it landed one, else the argmax of the logits it landed
def private vk_row_pick(s : Session; vr, vocab : int64) : int64 {
    return s.mtp_picks[vr] if (s.mtp_picks[vr] >= 0l)
    return parallel_argmax(unsafe(addr<float const?>(s.mtp_logits_b[vr * vocab])), vocab)
}

var private @scratch g_vk_sessions : array<Session?>
var private @scratch g_vk_idx : array<int64>
var private @scratch g_vk_pos : array<int64>
var private @scratch g_vk_nr : array<int64>
var private @scratch g_vk_regs : array<int64>
var private @scratch g_vk_toks : array<int64>
var private @scratch g_vk_cold : array<int64>

//! The gemma speculative ROUNDS of a tick's streams at depth `k_req` on the Vulkan resident driver, `gemma_mtp_spec_rounds`'s
//! twin: every warm stream bound to a region of its own, its chain of drafts on the device at its constant anchor, ONE verify of
//! every stream's [tok, d_1 .. d_k] on the N-row command, each stream's accept walk and commit; a stream the round cannot take
//! steps plain after the verify. Returns the warm streams the verify carried
[hot_path, arch(at="../ARCHITECTURE_GPU_VULKAN_MTP.md#resident-assistant-drafter"), arch(at="../ARCHITECTURE_GPU_MTP_ASSISTANT.md#assistant-drafter-rounds")]
def private vk_gemma_rounds(t : Model; var sessions : array<Session?>; toks : array<int64>; k_req : int64; var acc : array<int64>) : int64 {   // nolint:STYLE038 — binds / draft chains / one verify / walks / commits is one sequential protocol on shared locals
    let c = t.config
    let n = long_length(sessions)
    let k = clamp(k_req, 1l, DRAFT_K_MAX)
    acc |> resize(n)   // nolint:PERF026 — grows to the stream count once; the callers pass reused scratch
    g_vk_sessions |> clear()
    g_vk_idx |> clear()
    g_vk_pos |> clear()
    g_vk_nr |> clear()
    g_vk_regs |> clear()
    g_vk_cold |> clear()
    g_vk_sessions |> reserve(n)
    g_vk_idx |> reserve(n)
    g_vk_pos |> reserve(n)
    g_vk_nr |> reserve(n)
    g_vk_regs |> reserve(n)
    g_vk_cold |> reserve(n)
    let td = ref_time_ticks()
    for (i in range64(n)) {
        var s & = unsafe(*sessions[i])
        acc[i] = 0l
        let pos = s.n_past
        mtp_round_begin(s)
        if (long_length(s.mtp_vbatch) < MTP_MAX_ROWS) {
            s.mtp_vbatch |> reserve_resize(MTP_MAX_ROWS)   // nolint:PERF026 — once per session
        }
        let ki = mtp_depth_for(s, k)
        let r = vk_round_cold(t, s, pos, ki) ? -1l : gpu_resident_assist_bind(t, s, pos, ki + 1l, g_vk_regs)
        if (r < 0l) {
            g_vk_cold |> push(i)
            continue
        }
        s.mtp_vbatch[0] = toks[i]
        g_vk_sessions |> push(sessions[i])
        g_vk_idx |> push(i)
        g_vk_pos |> push(pos)
        g_vk_nr |> push(ki + 1l)
        g_vk_regs |> push(r)
    }
    var ng = long_length(g_vk_sessions)
    let ts_draft = prof_ticks()
    for (gi in range64(ng)) {
        var s & = unsafe(*g_vk_sessions[gi])
        vk_draft_chain(t, g_vk_regs[gi], s.mtp_vbatch[0], g_vk_pos[gi], g_vk_nr[gi] - 1l, s.mtp_h, s.mtp_vbatch)
    }
    g_vk_us_draft += int64(get_time_usec(td))
    prof_add("mtp.draft", ts_draft)
    let tv = ref_time_ticks()
    let ts_verify = prof_ticks()
    var pick_only = ng > 0l
    g_vk_toks |> clear()
    g_vk_toks |> reserve(ng * MTP_MAX_ROWS)
    for (gi in range64(ng)) {
        var s & = unsafe(*g_vk_sessions[gi])
        pick_only = pick_only && s.pick_asked && !mtp_walk_samples(s)
        for (i in range64(g_vk_nr[gi])) {
            g_vk_toks |> push(s.mtp_vbatch[i])
        }
    }
    let vok = ng > 0l && gpu_resident_assist_verify(t, g_vk_sessions, g_vk_toks, g_vk_pos, g_vk_nr, g_vk_regs, pick_only)
    prof_add("mtp.verify", ts_verify)
    let ts_walk = prof_ticks()
    if (vok) {
        for (gi in range64(ng)) {
            var s & = unsafe(*g_vk_sessions[gi])
            let pos = g_vk_pos[gi]
            let kd = g_vk_nr[gi] - 1l
            let a = mtp_accept_walk(s, kd, c.vocab_size, $(vr) => vk_row_pick(s, vr, c.vocab_size), pick_only)
            if (pick_only) {
                land_pick(s, s.mtp_picks[a])
            }
            copy_floats(s.mtp_hrows, a * c.dim, s.mtp_h, 0l, c.dim)
            s.mtp_drafted += kd
            mtp_round_commit_host(s, pos, kd, a)
            gpu_resident_assist_commit(s)
            g_vk_drafted += kd
            g_vk_accepted += a
            g_vk_rounds++
            acc[g_vk_idx[gi]] = a
        }
        if (ng > 1l) {
            g_vk_joint++
        }
    } else {
        for (gi in range64(ng)) {
            g_vk_cold |> push(g_vk_idx[gi])
        }
        ng = 0l
    }
    g_vk_us_verify += int64(get_time_usec(tv))
    prof_add("mtp.walk", ts_walk)
    let tc = ref_time_ticks()
    for (i in g_vk_cold) {   //! after the verify: a plain step's binding takes no region the round's streams held
        var s & = unsafe(*sessions[i])
        forward(t, s, toks[i], s.n_past)
        s.n_past++
    }
    g_vk_us_cold += int64(get_time_usec(tc))
    g_vk_sessions |> clear()   // borrowed handles
    return ng
}

def private vk_seam_stats() : MtpAssistantStats
    => MtpAssistantStats(drafted = g_vk_drafted, accepted = g_vk_accepted, rounds = g_vk_rounds, joint = g_vk_joint)

//! Round time split since attach, microseconds: draft chains / verify+commit / the plain steps of streams the round could not take
def public vk_gemma_drafter_time() : tuple<draft_us : int64; verify_us : int64; cold_us : int64> => (draft_us = g_vk_us_draft, verify_us = g_vk_us_verify, cold_us = g_vk_us_cold)

[init]
def private vk_gemma_mtp_init {
    register_vk_drop_hook(@@vk_drop_forget)
    register_mtp_assistant("vulkan", MtpAssistantSeams(attach = @@vk_attach, attached = @@vk_attached_to,
        rounds = @@vk_gemma_rounds, detach = @@vk_detach, stats = @@vk_seam_stats))
}
