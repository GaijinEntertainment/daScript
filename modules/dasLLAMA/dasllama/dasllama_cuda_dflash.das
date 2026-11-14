options gen2
options indenting = 4
options _dasllama_internal = true

module dasllama_cuda_dflash shared private

require dasllama/dasllama_lint public
require math
require dasllama/dasllama_common
require dasllama/dasllama_math   // reserve_resize - the landing's sizing
require dasllama/dasllama_kqformat
require dasllama/dasllama_gemm_schema   // Q8_BLOCK_ELEMS - the activation scale rows
require dasllama/dasllama_dflash
require dasllama/dasllama_layout   // q8_repack - the drafter's q8 planes off the GGUF blocks
require dasllama/dasllama_gpu_math   // ACT_SILU
require dasllama/dasllama_gpu_tier   // gpu_want_cuda, register_gpu_home
require dasllama/dasllama_gpu_kernels_common   // GkAddRmsArgs, the drafter's kernel arguments
require dasllama/dasllama_gpu_kernels_discrete   // GemvArgs, RqArgs, ActArgs
require dasllama/dasllama_gpu_resident   // moe_gpu_resident_active
require dasllama/dasllama_cuda_common
require dasllama/dasllama_cuda_classes
require dasllama/dasllama_cuda_classes_dflash
require dasllama/dasllama_cuda_decode   // the armed driver's seats: the table gather, the classifier over a feed

// The block drafter (DFlash, DFlash2) on the CUDA resident driver: the drafter's q8 planes, its activation planes and a ring a
// session on the device; the pass (`df_pass`) lands the draft rows' logits and selector projections for the host's walk, the
// commit (`df_commit`) writes the captured trunk rows' context into the ring.

let private DF_ROWS = DFLASH_DEVICE_ROWS

//! one GEMV site: its plane's format, its region record (the tensor's first block in the drafter's planes of that format) and its shape
struct private DfSite {
    fmt : int
    regs : uint64
    n : int64
    d : int64
}

//! a session's ring on the device: a layer's K and V rows a slot, [L][window][n_kv * hs] each
struct private DfRing {
    rk : uint64
    rv : uint64
}

struct private DflashCud {
    ready : bool
    fq, fs : uint64[KQ_FMT_COUNT]   //! a format's quant and scale planes in the device layout, its tensors' blocks one after another (0 = no plane of it)
    feed_src : uint64          //! the float rows the feed (xq, xs) was requantized from, and how many
    feed_elems, feed_rows : int64
    feed_kq : int              //! the feed's class: 1 = Q8_K (a superblock site reads it), 0 = Q8_0, -1 = no feed of a known source
    fblob : uint64             //! the drafter's f32 rows: the norms, the conv bases
    invf : uint64              //! the rope's inverse frequencies
    x, hn, cv, dyn, o : uint64 //! [DF_ROWS][dim] rows: the residual, the normed rows, a conv's output, the conv coefficients, a branch
    q, qn, att : uint64        //! [DF_ROWS][qd]
    kb, kn, vb : uint64        //! [DF_ROWS][kvd]
    g, u : uint64              //! [DF_ROWS][hidden]
    xq, xs : uint64            //! the feed of the widest row set, in the class `feed_kq` names
    capf : uint64              //! a commit chunk's captured rows, [DF_ROWS][ncap * dim]
    logits, hsel, ids : uint64
    pid, pval : uint64         //! the selector's slice lists, [nd][slices][topk] ids and values
    cid, cval : uint64         //! ... and each draft row's list, [nd][topk]
    sel_ok : bool              //! the device top-k serves this drafter's top-k
    walk_ok : bool             //! ... and the selector's walk runs there too: its two codebooks up in the CPU form the walk dequantizes
    cb : uint64[4]             //! the predecessor codebook's quant bytes and scale rows, then the successor's (a Q8_0 codebook's scale rows its own bytes)
    wids : uint64              //! the walk's ids: the anchor, then the picks
    walk : GkDflashWalkArgs
    walked : int64             //! the passes whose walk ran on the device
    selected : int64           //! the passes whose top-k ran on the device
    idin, capin, land : CudMapped
    xin : CudMapped             //! the block's token rows where the host embeds them (the device holds no table), [DF_ROWS][dim]
    sites : array<DfSite>
    rings : table<uint64; DfRing>
    ring_bytes : int64
    ring_said : bool           //! a session found no room for its ring, and the attach said so
    passes, commits : int64
    pass_us, commit_us : int64   //! the wall time the passes and the commits took, waits included
}

var private g_df : DflashCud

def private df_stats : DflashDeviceStats => DflashDeviceStats(passes = g_df.passes, commits = g_df.commits, pass_us = g_df.pass_us,
    commit_us = g_df.commit_us, selected = g_df.selected, walked = g_df.walked)

//! every handle dropped, nothing destroyed: the model drop already swept the device
def private df_forget(var g : DflashCud) {
    g.sites |> clear()
    g.rings |> clear()
    for (i in range(KQ_FMT_COUNT)) {
        g.fq[i] = 0ul
        g.fs[i] = 0ul
    }
    g.feed_kq = -1
    g.fblob = 0ul; g.invf = 0ul; g.x = 0ul; g.hn = 0ul; g.cv = 0ul; g.dyn = 0ul; g.o = 0ul
    g.q = 0ul; g.qn = 0ul; g.att = 0ul; g.kb = 0ul; g.kn = 0ul; g.vb = 0ul; g.g = 0ul; g.u = 0ul; g.xq = 0ul; g.xs = 0ul
    g.capf = 0ul; g.logits = 0ul; g.hsel = 0ul; g.ids = 0ul; g.pid = 0ul; g.pval = 0ul; g.cid = 0ul; g.cval = 0ul
    g.sel_ok = false
    g.walk_ok = false
    for (b in g.cb) {
        cuda_mem_free(b)
    }
    for (i in range(4)) {
        g.cb[i] = 0ul
    }
    cuda_mem_free(g.wids)
    g.wids = 0ul
    g.idin = CudMapped(); g.capin = CudMapped(); g.land = CudMapped(); g.xin = CudMapped()
    g.ready = false
}

def private df_release(var g : DflashCud) {
    cud_sync()
    for (i in range(KQ_FMT_COUNT)) {
        cuda_mem_free(g.fq[i])
        cuda_mem_free(g.fs[i])
    }
    for (b in [g.fblob, g.invf, g.x, g.hn, g.cv, g.dyn, g.o, g.q, g.qn, g.att, g.kb, g.kn, g.vb, g.g, g.u, g.xq, g.xs,
               g.capf, g.logits, g.hsel, g.ids, g.pid, g.pval, g.cid, g.cval]) {
        cuda_mem_free(b)
    }
    for (sv in g.sites) {
        cuda_mem_free(sv.regs)
    }
    for (r in values(g.rings)) {
        cuda_mem_free(r.rk)
        cuda_mem_free(r.rv)
    }
    cud_mapped_free(g.idin)
    cud_mapped_free(g.capin)
    cud_mapped_free(g.land)
    cud_mapped_free(g.xin)
    df_forget(g)
}

//! the model drop frees nothing of the drafter's: it frees its own and stands detached
def private df_drop_forget {
    if (g_df.ready) {
        df_release(g_df)
    }
}

def private df_detach {
    if (g_df.ready) {
        df_release(g_df)
    }
}

//! why the CUDA arm cannot take the drafter ("" = it can)
def private df_why(dr : DflashDrafter) : string {
    return "the CUDA resident driver holds no model" if (!moe_gpu_resident_active() || g_crd == null || !g_crd.ready)
    let shape = dflash_device_shape_why(dr, int64(GK_DFLASH_HS), int64(GK_DFLASH_ATTN_KEYS), DF_ROWS)
    return shape if (!empty(shape))
    var sites : array<DflashSitePlane>
    dflash_device_sites(dr, sites)
    var served = true
    for (sv in sites) {
        served = served && (!sv.on || cud_gemv_ensure(int(sv.w.fmt)))
    }
    delete sites
    return "a weight plane's decode GEMV did not compile on this device" if (!served)
    let stamps = (ensure_dflash_conv_cls() && ensure_dflash_qkn_cls() && ensure_dflash_attn_cls() && ensure_cls_ar() && ensure_cls_dn_rq()
        && ensure_cls_q8k_rq() && ensure_q8_actrq_cls() && ensure_q8k_actrq_cls() && ensure_words_copy())
    return "a kernel of the pass did not compile on this device" if (!stamps)
    return ""
}

//! the drafter's planes, scratch and mapped blocks on the device; false where the card has no room
//! a selector codebook up in the form the walk dequantizes: a Q8_0 codebook's blocks as the blob holds them, a superblock
//! one's quant bytes and its own scale rows off its format's planes; false where the walk reads no such format or the rows
//! carry a per-tensor scale (the host's walk applies it)
[cold_path]
def private df_walk_book(dr : DflashDrafter; w : DflashW; vocab : int64; var oom : bool&; var q : uint64&; var sc : uint64&) : bool {
    return false if (dflash_walk_fmt_code(w.fmt) < 0 || w.scale != 0.0)
    if (w.fmt == KqFmt.q8) {
        return false if (dr.rank % Q8_BLOCK_ELEMS != 0l)
        q = cud_dev_from(oom, unsafe(addr<void?>(dr.blob[w.off])), vocab * (dr.rank / Q8_BLOCK_ELEMS) * Q8_BLOCK_BYTES)
        sc = 0ul
        return true
    }
    let e = kq_elems(w.fmt)
    return false if (dr.rank % e != 0l || w.off % e != 0l)
    let sb0 = w.off / e
    let nsb = vocab * dr.rank / e
    let fi = int(w.fmt)
    q = cud_dev_from(oom, unsafe(addr<void?>(dr.kq[fi].q[sb0 * kq_qsb(w.fmt)])), nsb * kq_qsb(w.fmt))
    sc = cud_dev_from(oom, unsafe(addr<void?>(dr.kq[fi].s[sb0 * kq_ssb(w.fmt)])), nsb * kq_ssb(w.fmt))
    return true
}

//! the selector's two codebooks and the walk's arguments; false leaves the walk on the host
[cold_path]
def private df_walk_books(dr : DflashDrafter; var g : DflashCud; vocab : int64; var oom : bool&) : bool {
    // the kernel stages a row of 1024 and 4096 products
    return false if (dr.rank > 1024l || dr.topk * dr.rank > 4096l || !ensure_dflash_walk_cls()
        || !df_walk_book(dr, dr.sel_prev, vocab, oom, g.cb[0], g.cb[1]) || !df_walk_book(dr, dr.sel_next, vocab, oom, g.cb[2], g.cb[3]))
    g.wids = cud_dev_zero(oom, (dr.nblock + 1l) * 4l)
    g.walk = GkDflashWalkArgs(rank = uint(dr.rank), k = uint(dr.topk), pfmt = uint(dflash_walk_fmt_code(dr.sel_prev.fmt)),
        nfmt = uint(dflash_walk_fmt_code(dr.sel_next.fmt)), pq0 = 0u, nq0 = 0u, tbase = 0u, rbase = 0u)
    return !oom
}

def private df_buffers(t : Model; dr : DflashDrafter; var g : DflashCud) : bool {
    let d = dr.dim
    let qd = dr.n_heads * dr.hs
    let kvd = dr.n_kv * dr.hs
    let ncap = long_length(dr.target_layers)
    let nd = dr.nblock - 1l
    let vocab = t.config.vocab_size
    var oom = false
    g.fblob = cud_dev_from(oom, unsafe(addr<void?>(dr.fblob[0])), long_length(dr.fblob) * 4l)
    g.invf = cud_dev_from(oom, unsafe(addr<void?>(dr.inv_freq[0])), long_length(dr.inv_freq) * 4l)
    let rd = DF_ROWS * d * 4l
    g.x = cud_dev_zero(oom, rd)
    g.hn = cud_dev_zero(oom, rd)
    g.cv = cud_dev_zero(oom, rd)
    g.o = cud_dev_zero(oom, rd)
    g.dyn = cud_dev_zero(oom, DF_ROWS * max(dflash_projw(dr), 4l) * 4l)
    g.q = cud_dev_zero(oom, DF_ROWS * qd * 4l)
    g.qn = cud_dev_zero(oom, DF_ROWS * qd * 4l)
    g.att = cud_dev_zero(oom, DF_ROWS * qd * 4l)
    g.kb = cud_dev_zero(oom, DF_ROWS * kvd * 4l)
    g.kn = cud_dev_zero(oom, DF_ROWS * kvd * 4l)
    g.vb = cud_dev_zero(oom, DF_ROWS * kvd * 4l)
    g.g = cud_dev_zero(oom, DF_ROWS * dr.hidden * 4l)
    g.u = cud_dev_zero(oom, DF_ROWS * dr.hidden * 4l)
    let wide = DF_ROWS * max(max(max(qd, dr.hidden), d), ncap * d)
    g.xq = cud_dev_zero(oom, wide)
    g.xs = cud_dev_zero(oom, (wide / 16l) * 4l)   // a scale a 16 elements: past either requant's blocks
    g.capf = cud_dev_zero(oom, DF_ROWS * ncap * d * 4l)
    g.logits = cud_dev_zero(oom, nd * vocab * 4l)
    g.hsel = cud_dev_zero(oom, nd * max(dr.rank, 4l) * 4l)
    g.ids = cud_dev_zero(oom, DF_ROWS * 4l)
    g.sel_ok = dr.topk >= 1l && dr.topk <= int64(GK_DFLASH_SEL_K) && ensure_dflash_topk_slices_cls() && ensure_dflash_topk_merge_cls()
    if (g.sel_ok) {
        let lists = nd * gk_dflash_topk_slices(vocab) * dr.topk * 4l
        g.pid = cud_dev_zero(oom, lists)
        g.pval = cud_dev_zero(oom, lists)
        g.cid = cud_dev_zero(oom, nd * dr.topk * 4l)
        g.cval = cud_dev_zero(oom, nd * dr.topk * 4l)
    }
    g.walk_ok = g.sel_ok && dr.v2 && df_walk_books(dr, g, vocab, oom)
    g.idin = cud_mapped_make(oom, DF_ROWS * 4l)
    g.capin = cud_mapped_make(oom, DF_ROWS * ncap * d * 4l)
    g.land = cud_mapped_make(oom, nd * (vocab + max(dr.rank, 4l)) * 4l)
    g.xin = cud_mapped_make(oom, rd)
    g.ring_bytes = dr.n_layers * dr.window * kvd * 4l
    return !oom
}

//! the drafter's planes of every format in the device layout, a format's tensors one after another: the Q8_0 blocks repacked
//! off the blob, a superblock format's gathered off its `kq` planes in disk order by the engine's own gather - and each site's
//! first block in its format's planes; false where the card has no room
[cold_path]
def private df_planes(dr : DflashDrafter; var g : DflashCud; sites : array<DflashSitePlane>; var bases : array<int64>) : bool {
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
        g.fq[int(KqFmt.q8)] = cud_dev_from(oom, unsafe(addr<void?>(quants[0])), long_length(quants))
        g.fs[int(KqFmt.q8)] = cud_dev_from(oom, unsafe(addr<void?>(scales[0])), long_length(scales) * 2l)
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
            g.fq[int(f)] = cud_dev_from(oom, unsafe(addr<void?>(hq[0])), long_length(hq))
            g.fs[int(f)] = cud_dev_from(oom, unsafe(addr<void?>(hs[0])), long_length(hs))
        }
        delete hq
        delete hs
    }
    delete shell
    return !oom
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
        to_log(LOG_WARNING, "dasLLAMA: the block drafter's pass stays on the CPU - the card has no room for its planes and a session's ring\n")
        return false
    }
    g.sites |> reserve(length(sites))
    for (sv, i in sites, count()) {
        if (sv.on) {
            g.sites |> push(DfSite(fmt = int(sv.w.fmt), regs = buf_upload([uint(bases[i]), 0u], g_cud.stream), n = sv.n, d = sv.d))
        } else {
            g.sites |> push(DfSite())
        }
    }
    delete bases
    delete sites
    g.feed_kq = -1
    verify(cud_sync(), "cud dflash: the drafter's planes did not land")
    g.passes = 0l
    g.commits = 0l
    g.selected = 0l
    g.walked = 0l
    g.ring_said = false
    g.ready = true
    return true
}

//! the feed (xq, xs) requantized off the rows it was last asked for, in the class a site of `kq` reads; where the rows are
//! token rows the requant writes the rows tensor leaf's feed beside them
def private df_requant(var g : DflashCud; var s : void?; fmt : int) {
    g.feed_kq = kq_sb(fmt) ? 1 : 0
    cud_rdec_rq_rows(s, fmt, g.feed_src, g.xq, g.xs, g.feed_elems, g.feed_rows)
}

//! `elems` floats of `src` (`rows` token rows) as the feed of `site`, the first site that reads it; a later site of the other
//! feed class requants the same rows (`df_gemv`)
def private df_rq(var g : DflashCud; var s : void?; src : uint64; elems, rows : int64; site : int) {
    g.feed_src = src
    g.feed_elems = elems
    g.feed_rows = rows
    df_requant(g, s, g.sites[site].fmt)
}

//! one GEMV site over `rows` activation rows of the feed into y, its rows the site's width apart: the site's format's leaf
//! over its format's planes, the feed requantized first where it stands in the other class
def private df_gemv(var g : DflashCud; var s : void?; site : int; y : uint64; rows : int64) {
    let sv & = unsafe(g.sites[site])
    if (g.feed_kq != (kq_sb(sv.fmt) ? 1 : 0)) {
        verify(g.feed_src != 0ul, "cud dflash: a site of the other feed class with no rows to requant - engine bug")
        df_requant(g, s, sv.fmt)
    }
    var pc = GemvArgs(n = uint(sv.n), d = uint(sv.d), nreg = 1u, ybase = 0u, ncols = uint(rows), ystride = uint(sv.d))
    cud_rows_gemv(s, sv.fmt, g.fq[sv.fmt], g.fs[sv.fmt], sv.regs, g.xq, g.xs, y, pc)
}

//! the rows' residual step and norm: x += add (under `add_on`), then `out` = rms(x) under the weight row at `woff`
def private df_norm(g : DflashCud; dr : DflashDrafter; var s : void?; x, add, out : uint64; add_on : bool; woff, rows : int64) {
    var pc = GkAddRmsArgs(dim = uint(dr.dim), add_on = add_on ? 1u : 0u, woff = uint(woff), eps = dr.eps, ascale = 1.0)
    enc_cls_ar(s, fixed_array(x, add, g.fblob, out), pc, rows)
}

//! the dynamic conv of `src` into g.cv under the coefficients in g.dyn: `side` 0 a sublayer's input, 1 its output
def private df_conv(g : DflashCud; dr : DflashDrafter; var s : void?; src : uint64; boff, side, rows : int64) {
    let d = dr.dim
    var pa = GkDflashConvArgs(dim = uint(d), g = uint(dr.conv_g), side = uint(side),
        convk = uint(dr.conv_k), rows = uint(rows), blk = 0u, boff = uint(boff))
    enc_dflash_conv_cls(s, fixed_array(src, g.cv, g.dyn, g.fblob), pa, (rows * d + 255l) / 256l)
}

//! `nheads` heads of every row of `src` normed under the row at `woff` and roped at `pos0 ..` into `dst` from `yoff`
def private df_qkn(g : DflashCud; dr : DflashDrafter; var s : void?; src, dst : uint64; nheads, woff, yoff, pos0, rows : int64) {
    let w = uint(nheads * dr.hs)
    var pa = GkDflashQknArgs(hs = uint(dr.hs), nheads = uint(nheads), xs = w, ys = w, pos0 = uint(pos0), xoff = 0u,
        woff = uint(woff), yoff = uint(yoff), eps = dr.eps)
    enc_dflash_qkn_cls(s, fixed_array(src, g.fblob, g.invf, dst), pa, nheads * rows)
}

//! a sublayer's dynamic conv on its normed input (g.hn) where the drafter has one: the coefficients off the projection `site`,
//! the input's conv requantized as the feed; else the normed rows as the feed
def private df_feed(var g : DflashCud; dr : DflashDrafter; var s : void?; site : int; boff, rows : int64) {
    df_rq(g, s, g.hn, rows * dr.dim, rows, dr.v2 ? site : site + 1)   // the sublayer's first site past its projection is the next one
    if (dr.v2) {
        df_gemv(g, s, site, g.dyn, rows)
        df_conv(g, dr, s, g.hn, boff, 0l, rows)
        df_rq(g, s, g.cv, rows * dr.dim, rows, site + 1)
    }
}

//! a sublayer's output branch (g.o) through its output conv where the drafter has one: the plane the residual step adds
def private df_branch(g : DflashCud; dr : DflashDrafter; var s : void?; boff, rows : int64) : uint64 {
    return g.o if (!dr.v2)
    df_conv(g, dr, s, g.o, boff, 1l, rows)
    return g.cv
}

//! layer `l` over the block's `rows` at `pos`: on entry g.hn holds the rows under the layer's attention norm, on exit under the
//! next layer's (the final norm past the last)
[arch(at="../ARCHITECTURE_GPU_ASYMMETRIES_CUDA.md#cuda-dflash-pass")]
def private df_layer(var g : DflashCud; dr : DflashDrafter; var s : void?; ring : DfRing; l, rows, pos, lo, nctx : int64) {
    let qd = dr.n_heads * dr.hs
    let kvd = uint(dr.n_kv * dr.hs)
    df_feed(g, dr, s, dflash_site(l, 0), dr.v2 ? dr.aconv_base_offs[l] : 0l, rows)
    df_gemv(g, s, dflash_site(l, 1), g.q, rows)
    df_gemv(g, s, dflash_site(l, 2), g.kb, rows)
    df_gemv(g, s, dflash_site(l, 3), g.vb, rows)
    df_qkn(g, dr, s, g.q, g.qn, dr.n_heads, dr.qn_offs[l], 0l, pos, rows)
    df_qkn(g, dr, s, g.kb, g.kn, dr.n_kv, dr.kn_offs[l], 0l, pos, rows)
    let lbase = uint(l * dr.window) * kvd
    var pa = GkDflashAttnArgs(rows = uint(rows), pos0 = uint(pos), lo = uint(lo), nctx = uint(nctx), w = uint(dr.window), hs = uint(dr.hs),
        nheads = uint(dr.n_heads), grp = uint(dr.n_heads / dr.n_kv), scale = dr.attn_scale, rbase = 0u,
        koff = lbase, voff = lbase)
    enc_dflash_attn_cls(s, fixed_array(g.qn, g.kn, g.vb, ring.rk, ring.rv, g.att), pa, dr.n_heads * rows)
    df_rq(g, s, g.att, rows * qd, rows, dflash_site(l, 4))
    df_gemv(g, s, dflash_site(l, 4), g.o, rows)
    let abranch = df_branch(g, dr, s, dr.v2 ? dr.aconv_base_offs[l] : 0l, rows)
    df_norm(g, dr, s, g.x, abranch, g.hn, true, dr.ffn_norm_offs[l], rows)
    df_feed(g, dr, s, dflash_site(l, 5), dr.v2 ? dr.fconv_base_offs[l] : 0l, rows)
    df_gemv(g, s, dflash_site(l, 6), g.g, rows)
    df_gemv(g, s, dflash_site(l, 7), g.u, rows)
    let he = rows * dr.hidden
    let down_kq = kq_sb(g.sites[dflash_site(l, 8)].fmt)   // the act's requant lands in the down site's feed class
    var pc_act = ActArgs(nelem = uint(he), act = ACT_SILU, nblk = uint(he / kq_unit(down_kq)), elem0 = 0u)
    cud_actrq_enc(s, fixed_array(g.g, g.u, g.xq, g.xs), down_kq, pc_act, he)
    g.feed_kq = down_kq ? 1 : 0
    g.feed_src = 0ul   // the act's rows have no float source to requant again: the down site alone reads them
    df_gemv(g, s, dflash_site(l, 8), g.o, rows)
    let fbranch = df_branch(g, dr, s, dr.v2 ? dr.fconv_base_offs[l] : 0l, rows)
    let nxt = l + 1l < dr.n_layers ? dr.attn_norm_offs[l + 1l] : dr.norm_off
    df_norm(g, dr, s, g.x, fbranch, g.hn, true, nxt, rows)
}

//! the session's ring, made zeroed on its first use (0s where the card has no room)
//! a ring's zeroed K and V planes, both or neither (`k` 0 where the card has no room)
def private df_ring_planes(g : DflashCud) : tuple<k : uint64; v : uint64> {
    var oom = false
    let rk = cud_dev_zero(oom, g.ring_bytes)
    let rv = cud_dev_zero(oom, g.ring_bytes)
    if (oom) {
        cuda_mem_free(rk)
        cuda_mem_free(rv)
        return (k = 0ul, v = 0ul)
    }
    return (k = rk, v = rv)
}

//! a session's ring fits beside the planes: one pair made and freed at the attach
def private df_ring_room(g : DflashCud) : bool {
    let pl = df_ring_planes(g)
    cuda_mem_free(pl.k)
    cuda_mem_free(pl.v)
    return pl.k != 0ul
}

def private df_ring(var g : DflashCud; uid : uint64) : DfRing {
    if (!key_exists(g.rings, uid)) {
        let pl = df_ring_planes(g)
        if (pl.k == 0ul) {
            if (!g.ring_said) {
                g.ring_said = true
                dflash_ring_room_say("cuda", g.ring_bytes)
            }
            return DfRing()
        }
        g.rings[uid] = DfRing(rk = pl.k, rv = pl.v)
    }
    return g.rings?[uid] ?? DfRing()
}

def private df_drop(uid : uint64) {
    return if (!g_df.ready || !key_exists(g_df.rings, uid))
    cud_sync()
    let r = g_df.rings?[uid] ?? DfRing()
    cuda_mem_free(r.rk)
    cuda_mem_free(r.rv)
    g_df.rings |> erase(uid)
}

//! `draft_pass`: the block of `rows` (the anchor `tok` and the masks) at `pos` against the ring's positions `lo .. lo + nctx - 1`;
//! the first `nd` draft rows' logits ([nd][vocab]) and selector projections ([nd][rank]) landed
[arch(at="../ARCHITECTURE_GPU_ASYMMETRIES_CUDA.md#cuda-dflash-pass")]
//! the pass's launches over `rows` token rows from the anchor `tok` at `pos`: the rows embedded, the layers over the session's
//! ring, the `nd` draft rows through the trunk's classifier into `g.logits` and, on a selector file, their selector rows into
//! `g.hsel` - nothing landed, nothing waited
def private df_pass_enc(t : Model; dr : DflashDrafter; var g : DflashCud; ring : DfRing; var s : void?; tok, pos, rows, nd, lo, nctx : int64) {
    let d = dr.dim
    if (cud_rdec_emb_on()) {
        unsafe {
            var w = reinterpret<uint?>(g.idin.host)
            w[0] = uint(tok)
            for (r in range64(1l, rows)) {
                w[r] = uint(dr.mask_id)
            }
        }
        cud_words(s, g.idin.dev, g.ids, rows)
        cud_rdec_emb_gather(s, g.ids, g.x, rows, g_crd.emb_scale)
    } else {   // the device holds no table: the host's rows, as its token steps embed theirs
        unsafe {
            var xr = reinterpret<float?>(g.xin.host)
            for (r in range64(rows)) {
                embed_row(t, r == 0l ? tok : dr.mask_id, xr + r * d)
            }
            if (t.config.embed_scale != 1.0) {
                scale_inplace(xr, t.config.embed_scale, rows * d)
            }
        }
        cud_words(s, g.xin.dev, g.x, rows * d)
    }
    df_norm(g, dr, s, g.x, g.x, g.hn, false, dr.attn_norm_offs[0], rows)
    for (l in range64(dr.n_layers)) {
        df_layer(g, dr, s, ring, l, rows, pos, lo, nctx)
    }
    let draft_rows = g.hn + uint64(d * 4l)   // row 0 is the anchor's: the drafts are the rows after it
    cud_rdec_rq_rows(s, cud_rdec_cls_fmt(), draft_rows, g.xq, g.xs, nd * d, nd)
    cud_rdec_cls_rows(s, g.xq, g.xs, g.logits, nd)
    g.feed_kq = -1   // the trunk's classifier feed, not a site's
    if (dr.v2) {
        df_rq(g, s, draft_rows, nd * d, nd, DFLASH_SITE_SELH)
        df_gemv(g, s, DFLASH_SITE_SELH, g.hsel, nd)
    }
}

def private df_pass(t : Model; dr : DflashDrafter; uid : uint64; tok, pos, rows, nd, lo, nctx : int64; var logits : array<float>; var hsel : array<float>) : bool {
    var g & = unsafe(g_df)
    return false if (!g.ready || rows > DF_ROWS || nd < 1l || nd >= rows || nd >= dr.nblock)
    let t0 = ref_time_ticks()   // clock: control - the pass's wall for the stats seat, never a decision
    let ring = df_ring(g, uid)
    return false if (ring.rk == 0ul)
    cud_rdec_commit_kick()   // the trunk's recurrent commit beside the pass: the pass reads none of its slots
    var s = g_cud.stream
    let vocab = t.config.vocab_size
    df_pass_enc(t, dr, g, ring, s, tok, pos, rows, nd, lo, nctx)
    cuda_memcpy_dtoh(g.land.host, g.logits, uint64(nd * vocab * 4l), s)
    if (dr.v2) {
        unsafe {
            cuda_memcpy_dtoh(reinterpret<void?>(reinterpret<uint8?>(g.land.host) + nd * vocab * 4l), g.hsel, uint64(nd * dr.rank * 4l), s)
        }
    }
    return false if (!cud_sync())
    logits |> reserve_resize(nd * vocab)
    unsafe {
        memcpy(addr<void?>(logits[0]), g.land.host, nd * vocab * 4l)
        if (dr.v2) {
            hsel |> reserve_resize(nd * dr.rank)
            memcpy(addr<void?>(hsel[0]), reinterpret<void?>(reinterpret<uint8?>(g.land.host) + nd * vocab * 4l), nd * dr.rank * 4l)
        }
    }
    g.passes++
    g.pass_us += int64(get_time_usec(t0))
    return true
}

//! `commit`: the captured rows `caps` ([rows][slot][dim]) of positions `pos0 ..` into the session's ring - c = norm(fc . the
//! layer outputs), then a layer's K (normed, roped at the position) and V rows into the position's slot; a chunk of at most
//! DF_ROWS rows that does not cross the ring's end a time
[arch(at="../ARCHITECTURE_GPU_ASYMMETRIES_CUDA.md#cuda-dflash-pass")]
def private df_commit(dr : DflashDrafter; uid : uint64; caps : array<float>; rows, pos0 : int64) : bool {
    var g & = unsafe(g_df)
    return false if (!g.ready)
    let t0 = ref_time_ticks()   // clock: control - the commit's wall for the stats seat, never a decision
    let ring = df_ring(g, uid)
    return false if (ring.rk == 0ul)
    var s = g_cud.stream
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
            memcpy(g.capin.host, addr<void?>(caps[c0 * ncap * d]), n * ncap * d * 4l)
        }
        cud_words(s, g.capin.dev, g.capf, n * ncap * d)
        df_rq(g, s, g.capf, n * ncap * d, n, DFLASH_SITE_FC)
        df_gemv(g, s, DFLASH_SITE_FC, g.o, n)
        df_norm(g, dr, s, g.o, g.o, g.hn, false, dr.hnorm_off, n)
        df_rq(g, s, g.hn, n * d, n, dflash_site(0l, 2))
        for (l in range64(dr.n_layers)) {
            let at = l * w * kvd + slot * kvd
            df_gemv(g, s, dflash_site(l, 2), g.kb, n)
            df_qkn(g, dr, s, g.kb, ring.rk, dr.n_kv, dr.kn_offs[l], at, pos, n)
            df_gemv(g, s, dflash_site(l, 3), g.vb, n)
            cuda_memcpy_dtod(ring.rv + uint64(at * 4l), g.vb, uint64(n * kvd * 4l), s)
        }
        c0 += n
        return false if (c0 < rows && !cud_sync())   // the mapped block is the next chunk's too; the last chunk runs unwaited, the stream ordering the next pass behind it
    }
    g.commits++
    g.commit_us += int64(get_time_usec(t0))
    return true
}

def private df_selects(soft : bool) : bool => dflash_device_selects(soft, g_df.ready && g_df.sel_ok)

//! the selector's walk on the device over the pass's lists, the picks alone landed into `drafts`; false where the stream failed
def private df_walk_device(var g : DflashCud; var s : void?; tok, nd : int64; var drafts : array<int64>) : bool {
    unsafe {
        var w = reinterpret<uint?>(g.idin.host)
        w[0] = uint(tok)
    }
    var wka = g.walk
    wka.nd = uint(nd)
    let ps = g.cb[1] != 0ul ? g.cb[1] : g.cb[0]   // a Q8_0 codebook's scales ride its blocks
    let ns = g.cb[3] != 0ul ? g.cb[3] : g.cb[2]
    enc_dflash_walk_cls(s, fixed_array(g.idin.dev, g.hsel, g.cid, g.cval, g.cb[0], g.cb[0], ps, ps, g.cb[2], g.cb[2], ns, ns, g.wids), wka, 1l)
    cuda_memcpy_dtoh(g.land.host, g.wids, uint64((nd + 1l) * 4l), s)
    return false if (!cud_sync())
    unsafe {
        let idw = reinterpret<uint?>(g.land.host)
        for (r in range64(nd)) {
            drafts[r + 1l] = dflash_slot_id(idw[r + 1l])
        }
    }
    g.walked++
    return true
}

//! the pass's lists and selector rows landed and walked by the host's own `dflash_walk` into `drafts` (a drafter with no
//! selector takes each row's best); false where the stream failed
def private df_walk_host(g : DflashCud; dr : DflashDrafter; var s : void?; tok, nd : int64; var drafts : array<int64>) : bool {
    let k = dr.topk
    let lists = nd * k * 4l
    let rank = dr.v2 ? dr.rank : 0l
    unsafe {
        let host = reinterpret<uint8?>(g.land.host)
        cuda_memcpy_dtoh(reinterpret<void?>(host), g.cid, uint64(lists), s)
        cuda_memcpy_dtoh(reinterpret<void?>(host + lists), g.cval, uint64(lists), s)
        if (rank > 0l) {
            cuda_memcpy_dtoh(reinterpret<void?>(host + 2l * lists), g.hsel, uint64(nd * rank * 4l), s)
        }
    }
    return false if (!cud_sync())
    unsafe {
        let host = reinterpret<uint8?>(g.land.host)
        dflash_drafts_of_landing(dr, tok, nd, reinterpret<uint const?>(host), reinterpret<float const?>(host + lists),
            reinterpret<float const?>(host + 2l * lists), drafts)
    }
    return true
}

//! the pass with the selector's top-k on the device: each draft row's `topk` best logits (a list a slice, the slices' lists
//! merged) and its selector row - kilobytes where the rows' logits are megabytes - then the walk over them on the device
//! where the codebooks let it, the picks alone landed, else the host's own (`dflash_walk`)
[unused_argument(may_hold, may_hold_ahead, temp, rng, cids, qs), arch(at="../ARCHITECTURE_GPU_ASYMMETRIES_CUDA.md#cuda-dflash-pass")]
def private df_select_pass(t : Model; dr : DflashDrafter; uid : uint64; tok, pos, rows, nd, lo, nctx : int64; may_hold, may_hold_ahead : bool; temp : float;
                           var rng : int4&; var drafts : array<int64>; var cids : array<int64>; var qs : array<float>) : DflashSelect {
    var g & = unsafe(g_df)
    return DflashSelect.failed if (rows > DF_ROWS || nd < 1l || nd >= rows || nd >= dr.nblock)
    let t0 = ref_time_ticks()   // clock: control - the pass's wall for the stats seat, never a decision
    let ring = df_ring(g, uid)
    return DflashSelect.failed if (ring.rk == 0ul)
    cud_rdec_commit_kick()   // the trunk's recurrent commit beside the pass: the pass reads none of its slots
    var s = g_cud.stream
    df_pass_enc(t, dr, g, ring, s, tok, pos, rows, nd, lo, nctx)
    let vocab = t.config.vocab_size
    let nsl = gk_dflash_topk_slices(vocab)
    var ka = gk_dflash_topk_args(vocab, dr.topk)
    enc_dflash_topk_slices_cls(s, fixed_array(g.logits, g.logits, g.pid, g.pval), ka.slices, nsl * nd)
    enc_dflash_topk_merge_cls(s, fixed_array(g.pval, g.pid, g.cid, g.cval), ka.merge, nd)
    let landed = g.walk_ok && dflash_device_walk_on() ? df_walk_device(g, s, tok, nd, drafts) : df_walk_host(g, dr, s, tok, nd, drafts)
    return DflashSelect.failed if (!landed)
    g.passes++
    g.selected++
    g.pass_us += int64(get_time_usec(t0))
    return DflashSelect.landed
}

var private g_df_hooked = false   //! the drop hook is registered: an install a `set_gpu_home` repeats adds no second one

[init]
def private cud_dflash_init {
    register_gpu_home("cuda", @@cud_dflash_install)
    return if (!gpu_want_cuda())   // the home registers nothing where the Vulkan home holds the tier
    cud_dflash_install()
}

//! the CUDA arm's seams under the resident driver's decode override: at [init] where the home is asked for, or when
//! `set_gpu_home("cuda")` moves the tier here
def private cud_dflash_install {
    if (!g_df_hooked) {
        g_df_hooked = true
        register_cud_drop_hook(@@df_drop_forget)
    }
    register_dflash_device("vulkan", DflashDeviceSeams(ready = @@df_ready, commit = @@df_commit, draft_pass = @@df_pass, selects = @@df_selects,
        select_pass = @@df_select_pass, land = @@dflash_land_none, drop = @@df_drop, detach = @@df_detach, stats = @@df_stats))
}
