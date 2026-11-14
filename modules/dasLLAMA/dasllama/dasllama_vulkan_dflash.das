options gen2
options indenting = 4
options _dasllama_internal = true

module dasllama_vulkan_dflash shared private

require dasllama/dasllama_lint public
require math
require vulkan
require dasllama/dasllama_common
require dasllama/dasllama_math   // reserve_resize - the landing's sizing
require dasllama/dasllama_kqformat
require dasllama/dasllama_gemm_schema   // Q8_BLOCK_ELEMS - the activation scale rows
require dasllama/dasllama_dflash
require dasllama/dasllama_layout   // q8_repack - the drafter's q8 planes off the GGUF blocks
require dasllama/dasllama_gpu_math   // ACT_SILU
require dasllama/dasllama_gpu_tier   // gpu_want_cuda, register_gpu_home
require dasllama/dasllama_gpu_kernels_common   // GkAddRmsArgs, the drafter's kernel arguments
require dasllama/dasllama_gpu_kernels_discrete   // GemvArgs, ActArgs
require dasllama/dasllama_gpu_resident   // moe_gpu_resident_active
require dasllama/dasllama_vulkan_common
require dasllama/dasllama_vulkan_classes
require dasllama/dasllama_vulkan_classes_dflash
require dasllama/dasllama_vulkan_decode   // rd_chain_settle

// The block drafter (DFlash, DFlash2) on the Vulkan resident driver, the CUDA home's driver on this home's plumbing: the
// drafter's planes of every format, its activation planes and a ring a session on the device; the pass and the commit each one
// recorded command, its rows landed off mapped blocks.

let private DF_ROWS = DFLASH_DEVICE_ROWS

// the hazard classes of the drafter's own commands
let private B_X = 0x1u
let private B_HN = 0x2u
let private B_CV = 0x4u
let private B_DYN = 0x8u
let private B_O = 0x10u
let private B_Q = 0x20u
let private B_QN = 0x40u
let private B_ATT = 0x80u
let private B_KB = 0x100u
let private B_KN = 0x200u
let private B_VB = 0x400u
let private B_G = 0x800u
let private B_U = 0x1000u
let private B_XQ = 0x2000u     //! the feed, quants and scales
let private B_CAPF = 0x4000u
let private B_LOG = 0x8000u
let private B_HSEL = 0x10000u
let private B_IDS = 0x20000u
let private B_LIST = 0x40000u  //! the selector's lists: a slice's, a row's
let private B_WIDS = 0x80000u
let private B_RING = 0x100000u

//! one GEMV site: its plane's format, its region record (the tensor's first block in the drafter's planes of that format), its
//! set over the feed and the plane it writes, and its shape
struct private DfSite {
    fmt : int
    meta : HostBuf
    set_ : VkDescriptorSet
    n : int64
    d : int64
}

//! a session's ring on the device - a layer's K and V rows a slot, [L][window][n_kv * hs] each - with the sets that bind it
struct private DfRing {
    rk : uint64
    rv : uint64
    s_qkn : VkDescriptorSet    //! the commit's K rows normed and roped into the ring
    s_attn : VkDescriptorSet
}

struct private DflashVk {
    ready : bool
    fq, fs : uint64[KQ_FMT_COUNT]     //! a format's quant and scale planes in the device layout, its tensors' blocks one after another (0 = no plane of it)
    fqb, fsb : int64[KQ_FMT_COUNT]    //! ... their bytes
    fblob, invf : uint64              //! the drafter's f32 rows (the norms, the conv bases), the rope's inverse frequencies
    fblob_bytes, invf_bytes : int64
    x, hn, cv, dyn, o : uint64        //! [DF_ROWS][dim] rows: the residual, the normed rows, a conv's output, the conv coefficients, a branch
    q, qn, att : uint64               //! [DF_ROWS][qd]
    kb, kn, vb : uint64               //! [DF_ROWS][kvd]
    g, u : uint64                     //! [DF_ROWS][hidden]
    xq, xs : uint64                   //! the feed of the widest row set
    xq_bytes, xs_bytes : int64
    capf : uint64                     //! a commit chunk's captured rows, [DF_ROWS][ncap * dim]
    logits, hsel, ids : uint64
    pid, pval, cid, cval, wids : uint64
    row_bytes, dyn_bytes, q_bytes, kv_bytes, hid_bytes, capf_bytes, logits_bytes, hsel_bytes, list_bytes, row_list_bytes, wids_bytes : int64
    sel_ok : bool                     //! the device top-k serves this drafter's top-k
    walk_ok : bool                    //! ... and the selector's walk runs there too
    cb : uint64[4]                    //! the predecessor codebook's quant bytes and scale rows, then the successor's (a Q8_0 codebook's scale rows its own bytes)
    cbb : int64[4]
    walk : GkDflashWalkArgs
    idin, capin, land, xin : HostBuf  //! the ids and the captured rows up, the landing down, the block's rows where the host embeds them
    cmd : VkCommandBuffer
    sites : array<DfSite>
    rings : table<uint64; DfRing>
    ring_bytes : int64
    ring_said : bool           //! a session found no room for its ring, and the attach said so
    s_ar_xx, s_ar_xo, s_ar_xcv, s_ar_oo : VkDescriptorSet   //! the norm's sets by (rows, add partner): into `hn` every one
    s_conv_hn, s_conv_o : VkDescriptorSet
    s_qkn_q, s_qkn_k : VkDescriptorSet
    s_rq_hn, s_rq_cv, s_rq_att, s_rq_capf : VkDescriptorSet
    s_actrq, s_emb, s_cls, s_topk_s, s_topk_m, s_walk : VkDescriptorSet
    feed_set : VkDescriptorSet        //! the requant set of the rows the feed was last quantized from, their base and count; `feed_live` false = none to requant again
    feed_live : bool
    feed_base, feed_elems : int64
    feed_kq : int                     //! the feed's class: 1 = Q8_K (a superblock site reads it), 0 = Q8_0, -1 = unknown
    passes, commits, selected, walked : int64
    pass_us, commit_us : int64
}

var private g_df : DflashVk

def private df_stats : DflashDeviceStats => DflashDeviceStats(passes = g_df.passes, commits = g_df.commits, pass_us = g_df.pass_us,
    commit_us = g_df.commit_us, selected = g_df.selected, walked = g_df.walked)

//! every handle dropped, nothing destroyed: the model drop already swept the device
def private df_forget(var g : DflashVk) {
    g.sites |> clear()
    g.rings |> clear()
    for (i in range(KQ_FMT_COUNT)) {
        g.fq[i] = 0ul
        g.fs[i] = 0ul
    }
    g.fblob = 0ul; g.invf = 0ul; g.x = 0ul; g.hn = 0ul; g.cv = 0ul; g.dyn = 0ul; g.o = 0ul
    g.q = 0ul; g.qn = 0ul; g.att = 0ul; g.kb = 0ul; g.kn = 0ul; g.vb = 0ul; g.g = 0ul; g.u = 0ul; g.xq = 0ul; g.xs = 0ul
    g.capf = 0ul; g.logits = 0ul; g.hsel = 0ul; g.ids = 0ul; g.pid = 0ul; g.pval = 0ul; g.cid = 0ul; g.cval = 0ul; g.wids = 0ul
    for (i in range(4)) {
        g.cb[i] = 0ul
    }
    g.idin = HostBuf(); g.capin = HostBuf(); g.land = HostBuf(); g.xin = HostBuf()
    g.sel_ok = false
    g.walk_ok = false
    g.feed_kq = -1
    g.feed_live = false
    g.ready = false
}

def private df_host_free(var hb : HostBuf) {
    if (hb.buf != 0ul) {
        destroy_host_buf(hb)
        hb = HostBuf()
    }
}

def private df_release(var g : DflashVk) {
    for (i in range(KQ_FMT_COUNT)) {
        release_device_buf(g.fq[i])
        release_device_buf(g.fs[i])
    }
    release_device_buf(g.fblob); release_device_buf(g.invf); release_device_buf(g.x); release_device_buf(g.hn)
    release_device_buf(g.cv); release_device_buf(g.dyn); release_device_buf(g.o); release_device_buf(g.q)
    release_device_buf(g.qn); release_device_buf(g.att); release_device_buf(g.kb); release_device_buf(g.kn)
    release_device_buf(g.vb); release_device_buf(g.g); release_device_buf(g.u); release_device_buf(g.xq)
    release_device_buf(g.xs); release_device_buf(g.capf); release_device_buf(g.logits); release_device_buf(g.hsel)
    release_device_buf(g.ids); release_device_buf(g.pid); release_device_buf(g.pval); release_device_buf(g.cid)
    release_device_buf(g.cval); release_device_buf(g.wids)
    for (i in range(4)) {
        release_device_buf(g.cb[i])
    }
    for (sv in g.sites) {
        df_host_free(sv.meta)
    }
    for (r in values(g.rings)) {
        release_device_buf(r.rk)
        release_device_buf(r.rv)
    }
    df_host_free(g.idin)
    df_host_free(g.capin)
    df_host_free(g.land)
    df_host_free(g.xin)
    df_forget(g)
}

def private df_drop_forget {
    df_forget(g_df)
}

def private df_detach {
    if (g_df.ready) {
        rd_chain_settle()
        df_release(g_df)
    }
}

//! why the Vulkan arm cannot take the drafter ("" = it can)
def private df_why(dr : DflashDrafter) : string {
    return "the Vulkan resident driver holds no model" if (!moe_gpu_resident_active() || g_rd == null || !g_rd.ready)
    return "the trunk's classifier plane is not placed" if (g_rd.cls_blk < 0l)
    let shape = dflash_device_shape_why(dr, int64(GK_DFLASH_HS), int64(GK_DFLASH_ATTN_KEYS), DF_ROWS)
    return shape if (!empty(shape))
    var sites : array<DflashSitePlane>
    dflash_device_sites(dr, sites)
    var served = true
    for (sv in sites) {
        served = served && (!sv.on || (gemv_cls_ensure(int(sv.w.fmt)) && gemv_cls_ensure_n(int(sv.w.fmt))))
    }
    delete sites
    return "a weight plane's decode GEMV did not build on this device" if (!served)
    let cls_fmt = g_rd.cls_fmt_marker
    let stamps = (ensure_dflash_conv_cls() && ensure_dflash_qkn_cls() && ensure_dflash_attn_cls() && ensure_cls_ar() && ensure_cls_dn_rq()
        && ensure_cls_q8k_rq() && ensure_q8_actrq_cls() && ensure_q8k_actrq_cls() && gemv_cls_ensure(cls_fmt) && gemv_cls_ensure_n(cls_fmt)
        && (g_rd.emb_blk < 0l || emb_cls_ensure(g_rd.emb_fmt)) && (g_rd.emb_f32 == 0ul || ensure_emb_gather_f32_cls()))
    return "a kernel of the pass did not build on this device" if (!stamps)
    return ""
}

//! a selector codebook up in the form the walk dequantizes: a Q8_0 codebook's blocks as the blob holds them, a superblock
//! one's quant bytes and its own scale rows off its format's planes; false where the walk reads no such format or the rows
//! carry a per-tensor scale (the host's walk applies it)
[cold_path]
def private df_walk_book(dr : DflashDrafter; w : DflashW; vocab : int64; var oom : bool&; var g : DflashVk; at : int) : bool {
    return false if (dflash_walk_fmt_code(w.fmt) < 0 || w.scale != 0.0)
    if (w.fmt == KqFmt.q8) {
        return false if (dr.rank % Q8_BLOCK_ELEMS != 0l)
        g.cbb[at] = vocab * (dr.rank / Q8_BLOCK_ELEMS) * Q8_BLOCK_BYTES
        g.cb[at] = vk_dev_from(oom, unsafe(addr<uint8 const?>(dr.blob[w.off])), g.cbb[at])
        g.cb[at + 1] = 0ul
        return vk_range_ok(g.cbb[at])
    }
    let e = kq_elems(w.fmt)
    return false if (dr.rank % e != 0l || w.off % e != 0l)
    let sb0 = w.off / e
    let nsb = vocab * dr.rank / e
    let fi = int(w.fmt)
    g.cbb[at] = nsb * kq_qsb(w.fmt)
    g.cbb[at + 1] = nsb * kq_ssb(w.fmt)
    g.cb[at] = vk_dev_from(oom, unsafe(addr<uint8 const?>(dr.kq[fi].q[sb0 * kq_qsb(w.fmt)])), g.cbb[at])
    g.cb[at + 1] = vk_dev_from(oom, unsafe(addr<uint8 const?>(dr.kq[fi].s[sb0 * kq_ssb(w.fmt)])), g.cbb[at + 1])
    return vk_range_ok(max(g.cbb[at], g.cbb[at + 1]))
}

//! the selector's two codebooks and the walk's arguments; false leaves the walk on the host
[cold_path]
def private df_walk_books(dr : DflashDrafter; var g : DflashVk; vocab : int64; var oom : bool&) : bool {
    // the kernel stages a row of 1024 and 4096 products
    return false if (dr.rank > 1024l || dr.topk * dr.rank > 4096l || !ensure_dflash_walk_cls()
        || !df_walk_book(dr, dr.sel_prev, vocab, oom, g, 0) || !df_walk_book(dr, dr.sel_next, vocab, oom, g, 2))
    g.wids_bytes = (dr.nblock + 1l) * 4l
    g.wids = soft_device_buf(oom, g.wids_bytes)
    g.walk = GkDflashWalkArgs(rank = uint(dr.rank), k = uint(dr.topk), pfmt = uint(dflash_walk_fmt_code(dr.sel_prev.fmt)),
        nfmt = uint(dflash_walk_fmt_code(dr.sel_next.fmt)), pq0 = 0u, nq0 = 0u, tbase = 0u, rbase = 0u)
    return !oom
}

//! the drafter's scratch planes and mapped blocks on the device; false where the card has no room
[cold_path]
def private df_buffers(t : Model; dr : DflashDrafter; var g : DflashVk) : bool {
    let d = dr.dim
    let qd = dr.n_heads * dr.hs
    let kvd = dr.n_kv * dr.hs
    let ncap = long_length(dr.target_layers)
    let nd = dr.nblock - 1l
    let vocab = t.config.vocab_size
    var oom = false
    g.fblob_bytes = long_length(dr.fblob) * 4l
    g.invf_bytes = long_length(dr.inv_freq) * 4l
    g.fblob = vk_dev_from(oom, unsafe(addr<uint8 const?>(dr.fblob[0])), g.fblob_bytes)
    g.invf = vk_dev_from(oom, unsafe(addr<uint8 const?>(dr.inv_freq[0])), g.invf_bytes)
    g.row_bytes = DF_ROWS * d * 4l
    g.x = soft_device_buf(oom, g.row_bytes)
    g.hn = soft_device_buf(oom, g.row_bytes)
    g.cv = soft_device_buf(oom, g.row_bytes)
    g.o = soft_device_buf(oom, g.row_bytes)
    g.dyn_bytes = DF_ROWS * max(dflash_projw(dr), 4l) * 4l
    g.dyn = soft_device_buf(oom, g.dyn_bytes)
    g.q_bytes = DF_ROWS * qd * 4l
    g.q = soft_device_buf(oom, g.q_bytes)
    g.qn = soft_device_buf(oom, g.q_bytes)
    g.att = soft_device_buf(oom, g.q_bytes)
    g.kv_bytes = DF_ROWS * kvd * 4l
    g.kb = soft_device_buf(oom, g.kv_bytes)
    g.kn = soft_device_buf(oom, g.kv_bytes)
    g.vb = soft_device_buf(oom, g.kv_bytes)
    g.hid_bytes = DF_ROWS * dr.hidden * 4l
    g.g = soft_device_buf(oom, g.hid_bytes)
    g.u = soft_device_buf(oom, g.hid_bytes)
    g.xq_bytes = DF_ROWS * max(max(max(qd, dr.hidden), d), ncap * d)
    g.xs_bytes = (g.xq_bytes / 16l) * 4l   // a scale a 16 elements: past either requant's blocks
    g.xq = soft_device_buf(oom, g.xq_bytes)
    g.xs = soft_device_buf(oom, g.xs_bytes)
    g.capf_bytes = DF_ROWS * ncap * d * 4l
    g.capf = soft_device_buf(oom, g.capf_bytes)
    g.logits_bytes = nd * vocab * 4l
    g.logits = soft_device_buf(oom, g.logits_bytes)
    g.hsel_bytes = nd * max(dr.rank, 4l) * 4l
    g.hsel = soft_device_buf(oom, g.hsel_bytes)
    g.ids = soft_device_buf(oom, DF_ROWS * 4l)
    g.sel_ok = dr.topk >= 1l && dr.topk <= int64(GK_DFLASH_SEL_K) && ensure_dflash_topk_slices_cls() && ensure_dflash_topk_merge_cls()
    if (g.sel_ok) {
        g.list_bytes = nd * gk_dflash_topk_slices(vocab) * dr.topk * 4l
        g.row_list_bytes = nd * dr.topk * 4l
        g.pid = soft_device_buf(oom, g.list_bytes)
        g.pval = soft_device_buf(oom, g.list_bytes)
        g.cid = soft_device_buf(oom, g.row_list_bytes)
        g.cval = soft_device_buf(oom, g.row_list_bytes)
    }
    g.walk_ok = g.sel_ok && dr.v2 && df_walk_books(dr, g, vocab, oom)
    g.ring_bytes = dr.n_layers * dr.window * kvd * 4l
    let widest = max(max(max(g.logits_bytes, g.ring_bytes), max(g.capf_bytes, g.xq_bytes)),
        max(max(g.hid_bytes, g.row_bytes), max(max(g.q_bytes, g.kv_bytes), g.dyn_bytes)))
    return false if (oom || !vk_range_ok(widest))   //! every plane a set binds whole sits inside the device's storage range
    g.idin = make_host_buf(DF_ROWS * 4l, false)
    g.capin = make_host_buf(g.capf_bytes, false)
    g.xin = make_host_buf(g.row_bytes, false)
    g.land = make_host_buf(nd * (vocab + max(dr.rank, 4l)) * 4l, false, [cached = true])
    g.cmd = alloc_cmd()
    return true
}

//! the drafter's planes of every format in the device layout, a format's tensors one after another: the Q8_0 blocks repacked
//! off the blob, a superblock format's gathered off its `kq` planes in disk order by the engine's own gather - and each site's
//! first block in its format's planes; false where the card has no room or a plane passes the device's storage range
[cold_path]
def private df_planes(dr : DflashDrafter; var g : DflashVk; sites : array<DflashSitePlane>; var bases : array<int64>) : bool {
    var oom = false
    bases |> reserve(length(sites))
    bases |> resize(length(sites))
    var tensors <- [for (sv in sites); (boff = sv.w.off, n = sv.n * sv.d); where sv.on && sv.w.fmt == KqFmt.q8]
    if (!empty(tensors)) {
        var @exact_size quants : array<uint8>
        var @exact_size scales : array<uint16>
        var qb <- q8_repack(dr.blob, tensors, quants, scales)
        var bi = 0
        for (sv, i in sites, count()) {
            if (sv.on && sv.w.fmt == KqFmt.q8) {
                bases[i] = qb[bi++]
            }
        }
        let fi = int(KqFmt.q8)
        g.fqb[fi] = long_length(quants)
        g.fsb[fi] = long_length(scales) * 2l
        g.fq[fi] = vk_dev_from(oom, unsafe(addr<uint8 const?>(quants[0])), g.fqb[fi])
        g.fs[fi] = vk_dev_from(oom, unsafe(addr<uint8 const?>(scales[0])), g.fsb[fi])
        delete quants
        delete scales
        delete qb
    }
    delete tensors
    var shell = Model()   // the gather reads a model's planes: the drafter's, cloned for the walk
    for (fi in range(KQ_FMT_COUNT)) {
        let f = kq_fmt_of_slot(fi)
        continue if (f == KqFmt.q8 || empty(dr.kq[fi].q))
        shell.kq[fi].q := dr.kq[fi].q
        shell.kq[fi].s := dr.kq[fi].s
        for (sv in sites) {   // a tensor's scale rides its plane's rows on the device: the gather reads it by the plane's offset
            if (sv.on && sv.w.fmt == f && sv.w.scale != 0.0) {
                shell.site_scale[site_scale_key(f, sv.w.off)] = sv.w.scale
            }
        }
        let b = dev_block_bytes(f)
        var @exact_size hq : array<uint8>
        var @exact_size hs : array<uint8>
        for (sv, i in sites, count()) {
            continue if (!sv.on || sv.w.fmt != f)
            var @exact_size tq : array<uint8>
            var @exact_size ts : array<uint8>
            layout_gather_dispatch(shell, f, sv.w.off, sv.n, sv.d, 0l, 1l, 1l, 0l, tq, ts, true)
            bases[i] = long_length(hq) / b.qsb
            hq |> reserve(long_length(hq) + long_length(tq))
            hq |> push_from(tq)
            hs |> reserve(long_length(hs) + long_length(ts))
            hs |> push_from(ts)
            delete tq
            delete ts
        }
        if (!empty(hq)) {
            g.fqb[fi] = long_length(hq)
            g.fsb[fi] = long_length(hs)
            g.fq[fi] = vk_dev_from(oom, unsafe(addr<uint8 const?>(hq[0])), g.fqb[fi])
            g.fs[fi] = vk_dev_from(oom, unsafe(addr<uint8 const?>(hs[0])), g.fsb[fi])
        }
        delete hq
        delete hs
    }
    delete shell
    var in_range = true
    for (fi in range(KQ_FMT_COUNT)) {
        in_range = in_range && vk_range_ok(max(g.fqb[fi], g.fsb[fi]))
    }
    return !oom && in_range
}

//! the plane a site writes, with its bytes and hazard class: the site's index within its layer names it
def private df_site_out(g : DflashVk; site : int) : tuple<y : uint64; bytes : int64; bit : uint> {
    return (y = g.o, bytes = g.row_bytes, bit = B_O) if (site == DFLASH_SITE_FC)
    return (y = g.hsel, bytes = g.hsel_bytes, bit = B_HSEL) if (site == DFLASH_SITE_SELH)
    let k = (site - DFLASH_SITE_L0) % DFLASH_SITES_LAYER
    return (y = g.dyn, bytes = g.dyn_bytes, bit = B_DYN) if (k == 0 || k == 5)
    return (y = g.q, bytes = g.q_bytes, bit = B_Q) if (k == 1)
    return (y = g.kb, bytes = g.kv_bytes, bit = B_KB) if (k == 2)
    return (y = g.vb, bytes = g.kv_bytes, bit = B_VB) if (k == 3)
    return (y = g.g, bytes = g.hid_bytes, bit = B_G) if (k == 6)
    return (y = g.u, bytes = g.hid_bytes, bit = B_U) if (k == 7)
    return (y = g.o, bytes = g.row_bytes, bit = B_O)
}

//! the sets of every kernel the commands dispatch but the ring's
[cold_path]
def private df_sets(var g : DflashVk; sites : array<DflashSitePlane>; bases : array<int64>) {
    let rb = g.row_bytes
    g.sites |> reserve(length(sites))
    for (sv, i in sites, count()) {
        if (!sv.on) {
            g.sites |> push(DfSite())
            continue
        }
        let fi = int(sv.w.fmt)
        var st = DfSite(fmt = fi, meta = make_host_buf(META_BYTES, true), n = sv.n, d = sv.d)
        unsafe {
            var mp = reinterpret<uint?>(st.meta.mapped)
            mp[0] = uint(bases[i])
            mp[1] = 0u
        }
        let out = df_site_out(g, i)
        st.set_ = gemv_planes_set(fi, (wq = g.fq[fi], ws = g.fs[fi], wqb = g.fqb[fi], wsb = g.fsb[fi]), st.meta.buf, META_BYTES, 0u,
            g.xq, g.xs, g.xq_bytes, g.xs_bytes, B_XQ, out.y, out.bytes, out.bit)
        g.sites |> push(st)
    }
    g.s_ar_xx = set_cls_ar(fixed_array(g.x, g.x, g.fblob, g.hn), fixed_array(rb, rb, g.fblob_bytes, rb), fixed_array(B_X, B_X, 0u, B_HN))
    g.s_ar_xo = set_cls_ar(fixed_array(g.x, g.o, g.fblob, g.hn), fixed_array(rb, rb, g.fblob_bytes, rb), fixed_array(B_X, B_O, 0u, B_HN))
    g.s_ar_xcv = set_cls_ar(fixed_array(g.x, g.cv, g.fblob, g.hn), fixed_array(rb, rb, g.fblob_bytes, rb), fixed_array(B_X, B_CV, 0u, B_HN))
    g.s_ar_oo = set_cls_ar(fixed_array(g.o, g.o, g.fblob, g.hn), fixed_array(rb, rb, g.fblob_bytes, rb), fixed_array(B_O, B_O, 0u, B_HN))
    g.s_conv_hn = set_dflash_conv_cls(fixed_array(g.hn, g.cv, g.dyn, g.fblob), fixed_array(rb, rb, g.dyn_bytes, g.fblob_bytes), fixed_array(B_HN, B_CV, B_DYN, 0u))
    g.s_conv_o = set_dflash_conv_cls(fixed_array(g.o, g.cv, g.dyn, g.fblob), fixed_array(rb, rb, g.dyn_bytes, g.fblob_bytes), fixed_array(B_O, B_CV, B_DYN, 0u))
    g.s_qkn_q = set_dflash_qkn_cls(fixed_array(g.q, g.fblob, g.invf, g.qn), fixed_array(g.q_bytes, g.fblob_bytes, g.invf_bytes, g.q_bytes), fixed_array(B_Q, 0u, 0u, B_QN))
    g.s_qkn_k = set_dflash_qkn_cls(fixed_array(g.kb, g.fblob, g.invf, g.kn), fixed_array(g.kv_bytes, g.fblob_bytes, g.invf_bytes, g.kv_bytes), fixed_array(B_KB, 0u, 0u, B_KN))
    g.s_rq_hn = set_rq_cls(fixed_array(g.hn, g.xq, g.xs), fixed_array(rb, g.xq_bytes, g.xs_bytes), fixed_array(B_HN, B_XQ, B_XQ))
    g.s_rq_cv = set_rq_cls(fixed_array(g.cv, g.xq, g.xs), fixed_array(rb, g.xq_bytes, g.xs_bytes), fixed_array(B_CV, B_XQ, B_XQ))
    g.s_rq_att = set_rq_cls(fixed_array(g.att, g.xq, g.xs), fixed_array(g.q_bytes, g.xq_bytes, g.xs_bytes), fixed_array(B_ATT, B_XQ, B_XQ))
    g.s_rq_capf = set_rq_cls(fixed_array(g.capf, g.xq, g.xs), fixed_array(g.capf_bytes, g.xq_bytes, g.xs_bytes), fixed_array(B_CAPF, B_XQ, B_XQ))
    g.s_actrq = set_actrq_cls(fixed_array(g.g, g.u, g.xq, g.xs), fixed_array(g.hid_bytes, g.hid_bytes, g.xq_bytes, g.xs_bytes), fixed_array(B_G, B_U, B_XQ, B_XQ))
    let idb = DF_ROWS * 4l
    if (g_rd.emb_blk >= 0l) {
        g.s_emb = emb_cls_set(g_rd.emb_fmt, arena_planes(g_rd.emb_fmt, g_rd.emb_blk), g.ids, idb, B_IDS, g.x, rb, g.x, rb, B_X)
    } elif (g_rd.emb_f32 != 0ul) {
        g.s_emb = emb_f32_set(g_rd.emb_f32, g_rd.emb_f32_bytes, g.ids, idb, B_IDS, g.x, rb, g.x, rb, B_X)
    }
    let cls_fmt = g_rd.cls_fmt_marker
    g.s_cls = gemv_planes_set(cls_fmt, arena_planes(cls_fmt, g_rd.cls_blk), g_rd.cls_regions_dev, CLS_META_BYTES, 0u,
        g.xq, g.xs, g.xq_bytes, g.xs_bytes, B_XQ, g.logits, g.logits_bytes, B_LOG)
    if (g.sel_ok) {
        g.s_topk_s = set_dflash_topk_slices_cls(fixed_array(g.logits, g.logits, g.pid, g.pval),
            fixed_array(g.logits_bytes, g.logits_bytes, g.list_bytes, g.list_bytes), fixed_array(B_LOG, B_LOG, B_LIST, B_LIST))
        g.s_topk_m = set_dflash_topk_merge_cls(fixed_array(g.pval, g.pid, g.cid, g.cval),
            fixed_array(g.list_bytes, g.list_bytes, g.row_list_bytes, g.row_list_bytes), fixed_array(B_LIST, B_LIST, B_LIST, B_LIST))
    }
    if (g.walk_ok) {
        let ps = g.cb[1] != 0ul ? g.cb[1] : g.cb[0]   // a Q8_0 codebook's scales ride its blocks
        let psb = g.cb[1] != 0ul ? g.cbb[1] : g.cbb[0]
        let ns = g.cb[3] != 0ul ? g.cb[3] : g.cb[2]
        let nsb = g.cb[3] != 0ul ? g.cbb[3] : g.cbb[2]
        g.s_walk = set_dflash_walk_cls(fixed_array(g.ids, g.hsel, g.cid, g.cval, g.cb[0], g.cb[0], ps, ps, g.cb[2], g.cb[2], ns, ns, g.wids),
            fixed_array(idb, g.hsel_bytes, g.row_list_bytes, g.row_list_bytes, g.cbb[0], g.cbb[0], psb, psb, g.cbb[2], g.cbb[2], nsb, nsb, g.wids_bytes),
            fixed_array(B_IDS, B_HSEL, B_LIST, B_LIST, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, B_WIDS))
    }
}

//! `ready`: the drafter's planes on the device for `t`; false = the home declines and the CPU arm serves
[cold_path]
def private df_ready(t : Model; dr : DflashDrafter) : bool {
    df_detach()
    let why = df_why(dr)
    if (!empty(why)) {
        to_log(LOG_WARNING, "dasLLAMA: the block drafter's pass stays on the CPU - {why}\n")
        return false
    }
    var g & = unsafe(g_df)
    var sites : array<DflashSitePlane>
    dflash_device_sites(dr, sites)
    var bases : array<int64>
    let made = df_planes(dr, g, sites, bases) && df_buffers(t, dr, g) && df_ring_room(g)
    if (!made) {
        delete bases
        delete sites
        df_release(g)
        to_log(LOG_WARNING, "dasLLAMA: the block drafter's pass stays on the CPU - the card has no room for its planes and a session's ring, or a plane passes the device's storage range\n")
        return false
    }
    df_sets(g, sites, bases)
    delete bases
    delete sites
    g.feed_kq = -1
    g.feed_live = false
    g.passes = 0l
    g.commits = 0l
    g.selected = 0l
    g.walked = 0l
    g.ring_said = false
    g.ready = true
    return true
}

//! the feed requantized off the rows it was last asked for, in the class a site of `fmt` reads
def private df_requant(var g : DflashVk; raw : VkCommandBuffer; var h : VkHaz; fmt : int) {
    g.feed_kq = kq_sb(fmt) ? 1 : 0
    rq_enc(raw, h, g.feed_set, kq_sb(fmt), g.feed_base, g.feed_elems)
}

//! `elems` floats from element `base` of the plane `rq_set` reads as the feed of `site`, the first site that reads it; a later
//! site of the other feed class requants the same rows (`df_gemv`)
def private df_rq(var g : DflashVk; raw : VkCommandBuffer; var h : VkHaz; var rq_set : VkDescriptorSet; base, elems : int64; fmt : int) {
    g.feed_set = rq_set
    g.feed_base = base
    g.feed_elems = elems
    g.feed_live = true
    df_requant(g, raw, h, fmt)
}

//! one GEMV site over `rows` activation rows of the feed into its plane, its rows the site's width apart: the site's format's
//! leaf over its format's planes, the feed requantized first where it stands in the other class
def private df_gemv(var g : DflashVk; raw : VkCommandBuffer; var h : VkHaz; site : int; rows : int64) {
    let fmt = g.sites[site].fmt
    if (g.feed_kq != (kq_sb(fmt) ? 1 : 0)) {
        verify(g.feed_live, "vk dflash: a site of the other feed class with no rows to requant - engine bug")
        df_requant(g, raw, h, fmt)
    }
    var pc = GemvArgs(n = uint(g.sites[site].n), d = uint(g.sites[site].d), nreg = 1u, ybase = 0u, ncols = uint(rows), ystride = uint(g.sites[site].d))
    gemv_enc(fmt, raw, h, g.sites[site].set_, pc, g.sites[site].d)
}

//! the rows' residual step and norm through `s`: x += add (under `add_on`), then hn = rms(x) under the weight row at `woff`
def private df_norm(dr : DflashDrafter; raw : VkCommandBuffer; var h : VkHaz; var s : VkDescriptorSet; add_on : bool; woff, rows : int64) {
    var pc = GkAddRmsArgs(dim = uint(dr.dim), add_on = add_on ? 1u : 0u, woff = uint(woff), eps = dr.eps, ascale = 1.0)
    enc_cls_ar(raw, h, s, pc, rows)
}

//! the dynamic conv through `s` (its source `hn` or `o`) into cv under the coefficients in dyn: `side` 0 a sublayer's input, 1 its output
def private df_conv(dr : DflashDrafter; raw : VkCommandBuffer; var h : VkHaz; var s : VkDescriptorSet; boff, side, rows : int64) {
    let d = dr.dim
    var pa = GkDflashConvArgs(dim = uint(d), g = uint(dr.conv_g), side = uint(side),
        convk = uint(dr.conv_k), rows = uint(rows), blk = 0u, boff = uint(boff))
    enc_dflash_conv_cls(raw, h, s, pa, (rows * d + 255l) / 256l)
}

//! `nheads` heads of every row of the set's source normed under the row at `woff` and roped at `pos0 ..` into its plane from `yoff`
def private df_qkn(dr : DflashDrafter; raw : VkCommandBuffer; var h : VkHaz; var s : VkDescriptorSet; nheads, woff, yoff, pos0, rows : int64) {
    let w = uint(nheads * dr.hs)
    var pa = GkDflashQknArgs(hs = uint(dr.hs), nheads = uint(nheads), xs = w, ys = w, pos0 = uint(pos0), xoff = 0u,
        woff = uint(woff), yoff = uint(yoff), eps = dr.eps)
    enc_dflash_qkn_cls(raw, h, s, pa, nheads * rows)
}

//! a sublayer's dynamic conv on its normed input (hn) where the drafter has one: the coefficients off the projection `site`,
//! the input's conv requantized as the feed; else the normed rows as the feed
def private df_feed(var g : DflashVk; dr : DflashDrafter; raw : VkCommandBuffer; var h : VkHaz; site : int; boff, rows : int64) {
    let first = dr.v2 ? site : site + 1   // the sublayer's first site past its projection is the next one
    df_rq(g, raw, h, g.s_rq_hn, 0l, rows * dr.dim, g.sites[first].fmt)
    if (dr.v2) {
        df_gemv(g, raw, h, site, rows)
        df_conv(dr, raw, h, g.s_conv_hn, boff, 0l, rows)
        df_rq(g, raw, h, g.s_rq_cv, 0l, rows * dr.dim, g.sites[site + 1].fmt)
    }
}

//! a sublayer's residual step: its output branch (o) through its output conv where the drafter has one, added into x, then hn
//! under the weight row at `woff`
def private df_branch_norm(var g : DflashVk; dr : DflashDrafter; raw : VkCommandBuffer; var h : VkHaz; boff, woff, rows : int64) {
    if (dr.v2) {
        df_conv(dr, raw, h, g.s_conv_o, boff, 1l, rows)
        df_norm(dr, raw, h, g.s_ar_xcv, true, woff, rows)
    } else {
        df_norm(dr, raw, h, g.s_ar_xo, true, woff, rows)
    }
}

//! layer `l` over the block's `rows` at `pos`: on entry hn holds the rows under the layer's attention norm, on exit under the
//! next layer's (the final norm past the last)
def private df_layer(var g : DflashVk; dr : DflashDrafter; raw : VkCommandBuffer; var h : VkHaz; var ring : DfRing; l, rows, pos, lo, nctx : int64) {
    let qd = dr.n_heads * dr.hs
    let kvd = uint(dr.n_kv * dr.hs)
    df_feed(g, dr, raw, h, dflash_site(l, 0), dr.v2 ? dr.aconv_base_offs[l] : 0l, rows)
    df_gemv(g, raw, h, dflash_site(l, 1), rows)
    df_gemv(g, raw, h, dflash_site(l, 2), rows)
    df_gemv(g, raw, h, dflash_site(l, 3), rows)
    df_qkn(dr, raw, h, g.s_qkn_q, dr.n_heads, dr.qn_offs[l], 0l, pos, rows)
    df_qkn(dr, raw, h, g.s_qkn_k, dr.n_kv, dr.kn_offs[l], 0l, pos, rows)
    let lbase = uint(l * dr.window) * kvd
    var pa = GkDflashAttnArgs(rows = uint(rows), pos0 = uint(pos), lo = uint(lo), nctx = uint(nctx), w = uint(dr.window), hs = uint(dr.hs),
        nheads = uint(dr.n_heads), grp = uint(dr.n_heads / dr.n_kv), scale = dr.attn_scale, rbase = 0u,
        koff = lbase, voff = lbase)
    enc_dflash_attn_cls(raw, h, ring.s_attn, pa, dr.n_heads * rows)
    df_rq(g, raw, h, g.s_rq_att, 0l, rows * qd, g.sites[dflash_site(l, 4)].fmt)
    df_gemv(g, raw, h, dflash_site(l, 4), rows)
    df_branch_norm(g, dr, raw, h, dr.v2 ? dr.aconv_base_offs[l] : 0l, dr.ffn_norm_offs[l], rows)
    df_feed(g, dr, raw, h, dflash_site(l, 5), dr.v2 ? dr.fconv_base_offs[l] : 0l, rows)
    df_gemv(g, raw, h, dflash_site(l, 6), rows)
    df_gemv(g, raw, h, dflash_site(l, 7), rows)
    let he = rows * dr.hidden
    let down_kq = kq_sb(g.sites[dflash_site(l, 8)].fmt)   // the act's requant lands in the down site's feed class
    var pc_act = ActArgs(nelem = uint(he), act = ACT_SILU, elem0 = 0u)
    actrq_enc(raw, h, g.s_actrq, down_kq, pc_act, he)
    g.feed_kq = down_kq ? 1 : 0
    g.feed_live = false   // the act's rows have no float source to requant again: the down site alone reads them
    df_gemv(g, raw, h, dflash_site(l, 8), rows)
    let nxt = l + 1l < dr.n_layers ? dr.attn_norm_offs[l + 1l] : dr.norm_off
    df_branch_norm(g, dr, raw, h, dr.v2 ? dr.fconv_base_offs[l] : 0l, nxt, rows)
}

//! a ring's K and V planes, both or neither (`k` 0 where the card has no room)
def private df_ring_planes(g : DflashVk) : tuple<k : uint64; v : uint64> {
    var oom = false
    var rk = soft_device_buf(oom, g.ring_bytes)
    var rv = soft_device_buf(oom, g.ring_bytes)
    if (oom) {
        release_device_buf(rk)
        release_device_buf(rv)
        return (k = 0ul, v = 0ul)
    }
    return (k = rk, v = rv)
}

//! a session's ring fits beside the planes: one pair made and released at the attach
def private df_ring_room(g : DflashVk) : bool {
    var pl = df_ring_planes(g)
    let made = pl.k != 0ul
    release_device_buf(pl.k)
    release_device_buf(pl.v)
    return made
}

//! the session's ring, made zeroed on its first use with its sets (`rk` 0 where the card has no room)
def private df_ring(var g : DflashVk; uid : uint64) : DfRing {
    if (!key_exists(g.rings, uid)) {
        let pl = df_ring_planes(g)
        if (pl.k == 0ul) {
            if (!g.ring_said) {
                g.ring_said = true
                dflash_ring_room_say("vulkan", g.ring_bytes)
            }
            return DfRing()
        }
        let rk = pl.k
        let rv = pl.v
        upload_zeros(rk, 0l, g.ring_bytes)
        upload_zeros(rv, 0l, g.ring_bytes)
        g.rings[uid] = DfRing(rk = rk, rv = rv,
            s_qkn = set_dflash_qkn_cls(fixed_array(g.kb, g.fblob, g.invf, rk), fixed_array(g.kv_bytes, g.fblob_bytes, g.invf_bytes, g.ring_bytes),
                fixed_array(B_KB, 0u, 0u, B_RING)),
            s_attn = set_dflash_attn_cls(fixed_array(g.qn, g.kn, g.vb, rk, rv, g.att),
                fixed_array(g.q_bytes, g.kv_bytes, g.kv_bytes, g.ring_bytes, g.ring_bytes, g.q_bytes), fixed_array(B_QN, B_KN, B_VB, B_RING, B_RING, B_ATT)))
    }
    return g.rings?[uid] ?? DfRing()
}

def private df_drop(uid : uint64) {
    return if (!g_df.ready || !key_exists(g_df.rings, uid))
    rd_chain_settle()
    var r = g_df.rings?[uid] ?? DfRing()
    release_device_buf(r.rk)
    release_device_buf(r.rv)
    g_df.rings |> erase(uid)
}

//! the pass's dispatches over `rows` token rows from the anchor `tok` at `pos`: the rows embedded, the layers over the session's
//! ring, the `nd` draft rows through the trunk's classifier into `logits` and, on a selector file, their selector rows into `hsel`
def private df_pass_enc(t : Model; dr : DflashDrafter; var g : DflashVk; var ring : DfRing; raw : VkCommandBuffer; var h : VkHaz; tok, pos, rows, nd, lo, nctx : int64) {
    let d = dr.dim
    unsafe {
        var w = reinterpret<uint?>(g.idin.mapped)
        w[0] = uint(tok)
        for (r in range64(1l, rows)) {
            w[r] = uint(dr.mask_id)
        }
    }
    vhz_dep(raw, h, 0u, B_IDS, true)
    cmd_copy_range(raw, g.idin.buf, 0l, g.ids, 0l, rows * 4l)
    if (g_rd.emb_blk >= 0l) {
        emb_cls_enc(g_rd.emb_fmt, raw, h, g.s_emb, rows, d, arena_local_blk(g_rd.emb_blk), g_rd.emb_scale)
    } elif (g_rd.emb_f32 != 0ul) {
        emb_f32_enc(raw, h, g.s_emb, rows, d, g_rd.emb_scale)
    } else {   // the device holds no table: the host's rows, as its token steps embed theirs
        unsafe {
            var xr = reinterpret<float?>(g.xin.mapped)
            for (r in range64(rows)) {
                embed_row(t, r == 0l ? tok : dr.mask_id, xr + r * d)
            }
            if (t.config.embed_scale != 1.0) {
                scale_inplace(xr, t.config.embed_scale, rows * d)
            }
        }
        vhz_dep(raw, h, 0u, B_X, true)
        cmd_copy_range(raw, g.xin.buf, 0l, g.x, 0l, rows * d * 4l)
    }
    df_norm(dr, raw, h, g.s_ar_xx, false, dr.attn_norm_offs[0], rows)
    for (l in range64(dr.n_layers)) {
        df_layer(g, dr, raw, h, ring, l, rows, pos, lo, nctx)
    }
    // row 0 is the anchor's: the drafts are the rows after it
    let cls_fmt = g_rd.cls_fmt_marker
    df_rq(g, raw, h, g.s_rq_hn, d, nd * d, cls_fmt)
    var pc = GemvArgs(n = uint(d), d = uint(g_rd.vocab), nreg = 1u, ybase = 0u, ncols = uint(nd), ystride = uint(g_rd.vocab))
    gemv_enc(cls_fmt, raw, h, g.s_cls, pc, g_rd.vocab)
    if (dr.v2) {
        df_gemv(g, raw, h, DFLASH_SITE_SELH, nd)
    }
    g.feed_kq = -1   // the next command opens on its own feed
    g.feed_live = false
}

def private df_pass(t : Model; dr : DflashDrafter; uid : uint64; tok, pos, rows, nd, lo, nctx : int64; var logits : array<float>; var hsel : array<float>) : bool {
    var g & = unsafe(g_df)
    return false if (!g.ready || rows > DF_ROWS || nd < 1l || nd >= rows || nd >= dr.nblock)
    let t0 = ref_time_ticks()   // clock: control - the pass's wall for the stats seat, never a decision
    rd_chain_settle()
    var ring = df_ring(g, uid)
    return false if (ring.rk == 0ul)
    let vocab = t.config.vocab_size
    var h = cmd_open(g.cmd, true)
    df_pass_enc(t, dr, g, ring, g.cmd, h, tok, pos, rows, nd, lo, nctx)
    vhz_dep(g.cmd, h, B_LOG | B_HSEL, 0u, true)
    cmd_copy_range(g.cmd, g.logits, 0l, g.land.buf, 0l, nd * vocab * 4l)
    if (dr.v2) {
        cmd_copy_range(g.cmd, g.hsel, 0l, g.land.buf, nd * vocab * 4l, nd * dr.rank * 4l)
    }
    cmd_close(g.cmd)
    submit_wait(g.cmd)
    logits |> reserve_resize(nd * vocab)
    host_buf_read(g.land, unsafe(addr<void?>(logits[0])), nd * vocab * 4l)
    if (dr.v2) {
        hsel |> reserve_resize(nd * dr.rank)
        host_buf_read(g.land, unsafe(addr<void?>(hsel[0])), nd * dr.rank * 4l, nd * vocab * 4l)
    }
    g.passes++
    g.pass_us += int64(get_time_usec(t0))
    return true
}

//! `commit`: the captured rows `caps` ([rows][slot][dim]) of positions `pos0 ..` into the session's ring - c = norm(fc . the
//! layer outputs), then a layer's K (normed, roped at the position) and V rows into the position's slot; a chunk of at most
//! DF_ROWS rows that does not cross the ring's end a command
def private df_commit(dr : DflashDrafter; uid : uint64; caps : array<float>; rows, pos0 : int64) : bool {
    var g & = unsafe(g_df)
    return false if (!g.ready)
    let t0 = ref_time_ticks()   // clock: control - the commit's wall for the stats seat, never a decision
    rd_chain_settle()
    var ring = df_ring(g, uid)
    return false if (ring.rk == 0ul)
    let d = dr.dim
    let kvd = dr.n_kv * dr.hs
    let ncap = long_length(dr.target_layers)
    let w = dr.window
    var c0 = 0l
    while (c0 < rows) {
        let pos = pos0 + c0
        let slot = pos % w
        let n = min(min(DF_ROWS, rows - c0), w - slot)
        unsafe {
            memcpy(g.capin.mapped, addr<void?>(caps[c0 * ncap * d]), n * ncap * d * 4l)
        }
        var h = cmd_open(g.cmd, true)
        vhz_dep(g.cmd, h, 0u, B_CAPF, true)
        cmd_copy_range(g.cmd, g.capin.buf, 0l, g.capf, 0l, n * ncap * d * 4l)
        df_rq(g, g.cmd, h, g.s_rq_capf, 0l, n * ncap * d, g.sites[DFLASH_SITE_FC].fmt)
        df_gemv(g, g.cmd, h, DFLASH_SITE_FC, n)
        df_norm(dr, g.cmd, h, g.s_ar_oo, false, dr.hnorm_off, n)
        df_rq(g, g.cmd, h, g.s_rq_hn, 0l, n * d, g.sites[dflash_site(0l, 2)].fmt)
        for (l in range64(dr.n_layers)) {
            let at = l * w * kvd + slot * kvd
            df_gemv(g, g.cmd, h, dflash_site(l, 2), n)
            df_qkn(dr, g.cmd, h, ring.s_qkn, dr.n_kv, dr.kn_offs[l], at, pos, n)
            df_gemv(g, g.cmd, h, dflash_site(l, 3), n)
            vhz_dep(g.cmd, h, B_VB, B_RING, true)
            cmd_copy_range(g.cmd, g.vb, 0l, ring.rv, at * 4l, n * kvd * 4l)
        }
        g.feed_kq = -1
        g.feed_live = false
        cmd_close(g.cmd)
        submit_wait(g.cmd)
        c0 += n
    }
    g.commits++
    g.commit_us += int64(get_time_usec(t0))
    return true
}

def private df_selects(soft : bool) : bool => dflash_device_selects(soft, g_df.ready && g_df.sel_ok)

//! the pass with the selector's top-k on the device: each draft row's `topk` best logits (a list a slice, the slices' lists
//! merged) and its selector row - kilobytes where the rows' logits are megabytes - then the walk over them on the device
//! where the codebooks let it, the picks alone landed, else the host's own (`dflash_walk`)
[unused_argument(may_hold, may_hold_ahead, temp, rng, cids, qs)]
def private df_select_pass(t : Model; dr : DflashDrafter; uid : uint64; tok, pos, rows, nd, lo, nctx : int64; may_hold, may_hold_ahead : bool; temp : float;
                           var rng : int4&; var drafts : array<int64>; var cids : array<int64>; var qs : array<float>) : DflashSelect {
    var g & = unsafe(g_df)
    return DflashSelect.failed if (rows > DF_ROWS || nd < 1l || nd >= rows || nd >= dr.nblock)
    let t0 = ref_time_ticks()   // clock: control - the pass's wall for the stats seat, never a decision
    rd_chain_settle()
    var ring = df_ring(g, uid)
    return DflashSelect.failed if (ring.rk == 0ul)
    let k = dr.topk
    let vocab = t.config.vocab_size
    let nsl = gk_dflash_topk_slices(vocab)
    let rank = dr.v2 ? dr.rank : 0l
    let lists = nd * k * 4l
    let on_device = g.walk_ok && dflash_device_walk_on()
    var h = cmd_open(g.cmd, true)
    df_pass_enc(t, dr, g, ring, g.cmd, h, tok, pos, rows, nd, lo, nctx)
    var ka = gk_dflash_topk_args(vocab, k)
    enc_dflash_topk_slices_cls(g.cmd, h, g.s_topk_s, ka.slices, nsl * nd)
    enc_dflash_topk_merge_cls(g.cmd, h, g.s_topk_m, ka.merge, nd)
    if (on_device) {
        var wka = g.walk
        wka.nd = uint(nd)
        enc_dflash_walk_cls(g.cmd, h, g.s_walk, wka, 1l)
        vhz_dep(g.cmd, h, B_WIDS, 0u, true)
        cmd_copy_range(g.cmd, g.wids, 0l, g.land.buf, 0l, (nd + 1l) * 4l)
    } else {
        vhz_dep(g.cmd, h, B_LIST | B_HSEL, 0u, true)
        cmd_copy_range(g.cmd, g.cid, 0l, g.land.buf, 0l, lists)
        cmd_copy_range(g.cmd, g.cval, 0l, g.land.buf, lists, lists)
        if (rank > 0l) {
            cmd_copy_range(g.cmd, g.hsel, 0l, g.land.buf, 2l * lists, nd * rank * 4l)
        }
    }
    cmd_close(g.cmd)
    submit_wait(g.cmd)
    if (on_device) {
        unsafe {
            let idw = reinterpret<uint?>(g.land.mapped)
            for (r in range64(nd)) {
                drafts[r + 1l] = dflash_slot_id(idw[r + 1l])
            }
        }
        g.walked++
    } else {
        unsafe {
            let host = reinterpret<uint8?>(g.land.mapped)
            dflash_drafts_of_landing(dr, tok, nd, reinterpret<uint const?>(host), reinterpret<float const?>(host + lists),
                reinterpret<float const?>(host + 2l * lists), drafts)
        }
    }
    g.passes++
    g.selected++
    g.pass_us += int64(get_time_usec(t0))
    return DflashSelect.landed
}

var private g_df_hooked = false   //! the drop hook is registered: an install a `set_gpu_home` repeats adds no second one

[init]
def private vk_dflash_init {
    register_gpu_home("vulkan", @@vk_dflash_install)
    return if (gpu_want_cuda())   // the CUDA home's arm takes the seat where that home holds the tier
    vk_dflash_install()
}

//! the Vulkan arm's seams under the resident driver's decode override: at [init], or when `set_gpu_home("vulkan")` moves the
//! tier back here
def private vk_dflash_install {
    if (!g_df_hooked) {
        g_df_hooked = true
        register_vk_drop_hook(@@df_drop_forget)
    }
    register_dflash_device("vulkan", DflashDeviceSeams(ready = @@df_ready, commit = @@df_commit, draft_pass = @@df_pass, selects = @@df_selects,
        select_pass = @@df_select_pass, land = @@dflash_land_none, drop = @@df_drop, detach = @@df_detach, stats = @@df_stats))
}
