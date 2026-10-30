options gen2
options indenting = 4
options _dasllama_internal = true

module dasllama_cuda_mtp_gemma shared private

require dasllama/dasllama_lint public
require math
require dasllama/dasllama_common
require dasllama/dasllama_math   // grow_resize - the scratch sizing
require dasllama/dasllama_kqformat
require dasllama/dasllama_kv_dtype
require dasllama/dasllama_gemm_schema   // Q8_BLOCK_ELEMS - the activation scale rows
require dasllama/dasllama_mtp_gemma
require dasllama/dasllama_layout   // q8_repack - the drafter's q8 planes off the GGUF blocks
require dasllama/dasllama_gpu_math   // ACT_GELU
require dasllama/dasllama_gpu_tier   // rdec_mir_region_base, gpu_said_once
require dasllama/dasllama_gpu_kernels_common   // GkAddRmsArgs
require dasllama/dasllama_gpu_kernels_discrete   // the token step's args structs, the attention's row workgroups
require dasllama/dasllama_gpu_resident   // the tier's assistant seams: the bind, the N-row verify, the commit, the rope row
require dasllama/dasllama_cuda_common
require dasllama/dasllama_cuda_classes
require dasllama/dasllama_cuda_decode   // the armed driver's planes and seats: the mirror slabs, the table gather, the carry arm

// The gemma-4 assistant drafter on the CUDA resident driver (`dasllama_vulkan_mtp_gemma.das`'s twin): the drafter's q8 planes
// and its own activation planes on the device, a captured draft graph a (region, depth) that chains the drafts over the
// target's K/V mirror at the two capture layers, and the speculative rounds of a tick's streams verified on the N-row command.

//! one q8 GEMV site: its region record (the tensor's first block in the drafter's planes) and its shape
struct private GemmaCudGemv {
    regs : uint64
    n : int64
    d : int64
}

//! a mapped host block: the host's pointer and the device's address of the same bytes
struct private CudMapped {
    host : void?
    dev : uint64
}

//! The attached drafter on the CUDA driver: its planes, activation planes, the captured draft graphs
struct private GemmaDrafterCud {
    ready : bool
    fmt : KqFmt                 //! the weight planes' format, the sidecar loader's tag
    wq : uint64                 //! the q8 quant words, every tensor's blocks one after another
    ws : uint64                 //! ... their f16 scales
    fblob : uint64              //! the drafter's f32 rows: the norms, the q norms, the rope table
    cat : uint64
    catq : uint64
    cats : uint64
    xin : uint64
    xq : uint64
    xs : uint64
    q : uint64
    attn : uint64
    part : uint64
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
    tok : CudMapped             //! one TokMeta: the key count the attention reads, the anchor
    cos : CudMapped             //! the anchor's cos/sin plane
    inp : CudMapped             //! the seed id at word 0, the carried hidden from byte 16
    picks : CudMapped           //! the chain's ids: the seed, then each draft
    lout : CudMapped            //! the last step's logits and h_next, read back for the oracle
    gemvs : array<GemmaCudGemv>
    graphs : array<void?>       //! a (region, depth) each, captured on its first use
    hs_mx : int64
}

var private g_cud_dr : GemmaDrafter
var private g_cud_g : GemmaDrafterCud
var private g_cud_attached = false
var private @scratch g_cud_round : DiscreteRoundState   //! the tick's round: the gather, the verify's tokens, the cold streams, the stats since the attach

//! `bytes` of device memory, zeroed (0 and `oom` where the card has no room)
def private dev_soft(var oom : bool&; bytes : int64) : uint64 {
    let d = cuda_mem_alloc(uint64(max(bytes, 16l)))
    if (d == 0ul) {
        oom = true
        return 0ul
    }
    cuda_memset_d8(d, 0u8, uint64(max(bytes, 16l)), g_cud.stream)
    return d
}

//! `bytes` of `src` into a fresh device buffer (0 and `oom` where the card has no room)
def private dev_from(var oom : bool&; src : void?; bytes : int64) : uint64 {
    let d = cuda_mem_alloc(uint64(bytes))
    if (d == 0ul) {
        oom = true
        return 0ul
    }
    cuda_memcpy_htod(d, src, uint64(bytes), g_cud.stream)
    return d
}

def private mapped_make(var oom : bool&; bytes : int64) : CudMapped {
    let m = cuda_mem_alloc_host_mapped(uint64(bytes))
    if (m.host == null) {
        oom = true
    }
    return CudMapped(host = m.host, dev = m.dev)
}

def private mapped_free(var m : CudMapped) {
    if (m.host != null) {
        cuda_mem_free_host(m.host)
    }
    m = CudMapped()
}

//! every handle the drafter held dropped, nothing destroyed: the model drop already swept the device
def private cud_forget(var g : GemmaDrafterCud) {
    g.gemvs |> clear()
    g.graphs |> clear()
    g.wq = 0ul; g.ws = 0ul; g.fblob = 0ul; g.cat = 0ul; g.catq = 0ul; g.cats = 0ul; g.xin = 0ul; g.xq = 0ul; g.xs = 0ul
    g.q = 0ul; g.attn = 0ul; g.part = 0ul; g.aq = 0ul; g.aqs = 0ul; g.o = 0ul; g.gate = 0ul; g.up = 0ul; g.hq = 0ul; g.hqs = 0ul
    g.logits = 0ul; g.hnext = 0ul; g.ids = 0ul; g.pmax = 0ul; g.pid = 0ul
    g.tok = CudMapped(); g.cos = CudMapped(); g.inp = CudMapped(); g.picks = CudMapped(); g.lout = CudMapped()
    g.ready = false
}

def private cud_buffers_release(var g : GemmaDrafterCud) {
    cud_sync()
    for (gr in g.graphs) {
        if (gr != null) {
            cuda_graph_destroy(gr)
        }
    }
    for (b in [g.wq, g.ws, g.fblob, g.cat, g.catq, g.cats, g.xin, g.xq, g.xs, g.q, g.attn, g.part, g.aq, g.aqs, g.o, g.gate, g.up,
               g.hq, g.hqs, g.logits, g.hnext, g.ids, g.pmax, g.pid]) {
        cuda_mem_free(b)
    }
    for (gv in g.gemvs) {
        cuda_mem_free(gv.regs)
    }
    mapped_free(g.tok)
    mapped_free(g.cos)
    mapped_free(g.inp)
    mapped_free(g.picks)
    mapped_free(g.lout)
    cud_forget(g)
}

//! the classes the drafter's graph launches, compiled on the live device (the driver's prepare loaded every kernel; the ladder's
//! answer stands for a class the home never stamped)
def private cud_classes_ok(fmt : KqFmt; dt : KVDtype) : bool {
    var attn = false   // the decode attention's stamps of the mirror's codec, both slab groups
    if (dt == KVDtype.f16) {
        attn = ensure_da_attn_f16_cls() && ensure_da_attn_f16_g2_cls()
    } elif (dt == KVDtype.tq4) {
        attn = ensure_da_attn_tq4_cls() && ensure_da_attn_tq4_g2_cls()
    } elif (kv_block_codec(dt)) {
        attn = ensure_da_attn_q8_cls() && ensure_da_attn_q8_g2_cls()
    } else {
        attn = ensure_da_attn_cls() && ensure_da_attn_g2_cls()
    }
    let emb = g_crd.emb_blk >= 0l ? cud_emb_ensure(g_crd.emb_fmt) : ensure_emb_gather_f32_cls()
    return (attn && emb && ensure_cls_ar_rq_b() && ensure_cls_dn_rq() && ensure_q8_actrq_cls() && ensure_qkn_rope_cls() && cud_gemv_ensure(int(fmt))
        && ensure_cls_argmax_part() && ensure_cls_argmax_fin() && ensure_words_copy()
        && (dt != KVDtype.tq4 || (ensure_kv_tq4_store_cls() && ensure_kv_tq4_unrot_cls())))
}

def private cud_trunk_facts() : DrafterTrunk {
    var f : DrafterTrunk
    f.served = moe_gpu_resident_active() && g_crd != null && g_crd.ready
    return <- f if (!f.served)
    f.hc = false   // this home declines the hyper-connection chain at its seats: a trunk it serves has none
    f.emb_table = cud_rdec_emb_on()
    f.assist_rows = gpu_resident_assist_rows()
    f.own_kv <- [for (ly in g_crd.layers); !ly.recurrent && !ly.kv_shared]
    return <- f
}

//! why the CUDA driver holding `f` cannot take the drafter ("" = it can): the shared ladder under this home's name
def public cud_drafter_trunk_why(f : DrafterTrunk; dr : GemmaDrafter) : string => drafter_trunk_why("CUDA", f, dr)

def private gemv_make(base, n, d : int64) : GemmaCudGemv => GemmaCudGemv(regs = buf_upload([uint(base), 0u], g_cud.stream), n = n, d = d)

[arch(at="../ARCHITECTURE_GPU_VULKAN_MTP.md#resident-assistant-drafter")]
def private cud_drafter_init(dr : GemmaDrafter; var g : GemmaDrafterCud) : bool {
    var hs_mx = 0l
    for (h in dr.hs) {
        hs_mx = max(hs_mx, h)
    }
    g.hs_mx = hs_mx
    if (!cud_classes_ok(dr.fmt, g_crd.kv_dt)) {
        return false
    }
    var @exact_size quants : array<uint8>
    var @exact_size scales : array<uint16>
    var tensors <- q8_tensors(dr)
    var bases <- q8_repack(dr.blob, tensors, quants, scales)
    delete tensors
    let made = cud_buffers_make(dr, g, quants, scales)
    delete quants
    delete scales
    if (!made) {
        delete bases
        cud_buffers_release(g)
        return false
    }
    g.gemvs <- cud_gemvs_make(dr, bases)
    delete bases
    let ngraph = g_crd.regions * (DRAFT_K_MAX + 1l)
    g.graphs |> resize(ngraph)
    verify(cud_sync(), "cud gemma drafter: the planes did not land")
    g.ready = true
    return true
}

//! the drafter's weight planes (the repacked q8 `quants` / `scales`, the f32 blob) and scratch on the device and its mapped host
//! blocks; false where the card has no room
def private cud_buffers_make(dr : GemmaDrafter; var g : GemmaDrafterCud; quants : array<uint8>; scales : array<uint16>) : bool {
    g.fmt = dr.fmt
    var oom = false
    g.wq = dev_from(oom, unsafe(addr<void?>(quants[0])), long_length(quants))
    g.ws = dev_from(oom, unsafe(addr<void?>(scales[0])), long_length(scales) * 2l)
    g.fblob = dev_from(oom, unsafe(addr<void?>(dr.fblob[0])), long_length(dr.fblob) * 4l)
    let qdm = dr.n_heads * g.hs_mx
    let cat_n = 2l * dr.tdim
    g.cat = dev_soft(oom, cat_n * 4l)
    g.catq = dev_soft(oom, cat_n)
    g.cats = dev_soft(oom, (cat_n / Q8_BLOCK_ELEMS) * 4l)
    g.xin = dev_soft(oom, dr.dim * 4l)
    g.xq = dev_soft(oom, dr.dim)
    g.xs = dev_soft(oom, (dr.dim / Q8_BLOCK_ELEMS) * 4l)
    g.q = dev_soft(oom, qdm * 4l)
    g.attn = dev_soft(oom, qdm * 4l)
    g.cntoff = dr.n_heads * (g.hs_mx + 2l)   // one key split: the counters past one piece a head
    g.part = dev_soft(oom, (g.cntoff + dr.n_heads) * 4l)
    g.aq = dev_soft(oom, qdm)
    g.aqs = dev_soft(oom, (qdm / Q8_BLOCK_ELEMS) * 4l)
    g.o = dev_soft(oom, dr.dim * 4l)
    g.gate = dev_soft(oom, dr.hidden * 4l)
    g.up = dev_soft(oom, dr.hidden * 4l)
    g.hq = dev_soft(oom, dr.hidden)
    g.hqs = dev_soft(oom, (dr.hidden / Q8_BLOCK_ELEMS) * 4l)
    g.logits = dev_soft(oom, dr.vocab * 4l)
    g.hnext = dev_soft(oom, dr.tdim * 4l)
    g.ids = dev_soft(oom, 16l)
    let pbytes = int64(CLS_ARGMAX_CHUNKS) * 4l
    g.pmax = dev_soft(oom, pbytes)
    g.pid = dev_soft(oom, pbytes)
    g.tok = mapped_make(oom, TOK_META_BYTES)
    g.cos = mapped_make(oom, g_crd.cos_elems * 4l)
    g.inp = mapped_make(oom, 16l + dr.tdim * 4l)
    g.picks = mapped_make(oom, MTP_MAX_ROWS * 4l)
    g.lout = mapped_make(oom, (dr.vocab + dr.tdim) * 4l)
    return !oom
}

//! the q8 GEMV sites in the graph's order: pre-proj, post-proj, classifier, then each layer's q, o, gate, up, down
def private cud_gemvs_make(dr : GemmaDrafter; bases : array<int64>) : array<GemmaCudGemv> {
    let cat_n = 2l * dr.tdim
    var out <- [gemv_make(bases[0], cat_n, dr.dim), gemv_make(bases[1], dr.dim, dr.tdim), gemv_make(bases[2], dr.dim, dr.vocab)]
    out |> reserve(3l + 5l * dr.n_layers)
    for (l in range64(dr.n_layers)) {
        let qd = dr.n_heads * dr.hs[l]
        let b0 = 3l + l * 5l
        out |> push(gemv_make(bases[b0], dr.dim, qd))
        out |> push(gemv_make(bases[b0 + 1l], qd, dr.dim))
        out |> push(gemv_make(bases[b0 + 2l], dr.dim, dr.hidden))
        out |> push(gemv_make(bases[b0 + 3l], dr.dim, dr.hidden))
        out |> push(gemv_make(bases[b0 + 4l], dr.hidden, dr.dim))
    }
    return <- out
}

//! one q8 GEMV site's launch over the drafter's planes: the feed (xq, xs) into y
def private gemv_site(g : GemmaDrafterCud; var s : void?; site : int; xq, xs, y : uint64) {
    let gv & = unsafe(g.gemvs[site])
    var pc = GemvArgs(n = uint(gv.n), d = uint(gv.d), nreg = 1u, ybase = 0u, ncols = 1u, ystride = uint(gv.d))
    cud_gemv_enc(int(g.fmt), s, fixed_array(g.wq, g.ws, gv.regs, xq, xs, y), pc, gv.d)
}

//! the residual step and the next norm's Q8_0 feed in one launch: x += rms(o) . pw (under `add`), then rms(x) . w quantized
def private ar_site(g : GemmaDrafterCud; dr : GemmaDrafter; var s : void?; add : bool; woff, pwoff : int64; ascale : float) {
    var pc = GkAddRmsArgs(dim = uint(dr.dim), add_on = add ? 1u : 0u, woff = uint(woff), eps = dr.eps, ascale = ascale, pre_on = add ? 1u : 0u, pwoff = uint(pwoff))
    enc_cls_ar_rq_b(s, fixed_array(g.xin, g.o, g.fblob, g.xq, g.xs), pc, 1l)
}

//! `elems` floats of `src` requantized to Q8_0 blocks (outq, outs)
def private rq_site(var s : void?; src, outq, outs : uint64; elems : int64) {
    var pcq = RqArgs(inbase = 0u, nblk = uint(elems / Q8_BLOCK_ELEMS), lo = 0.0, hi = 0.0)
    enc_cls_dn_rq(s, fixed_array(src, outq, outs), pcq, rq_wgs(false, elems))
}

//! the decode attention of drafter layer `l` over its capture layer's rows in region `r`: the pass the token step runs, one row
def private attn_site(g : GemmaDrafterCud; dr : GemmaDrafter; var s : void?; l, r : int64) {
    let hs = uint(dr.hs[l])
    let qd = uint(dr.n_heads) * hs
    let kv_mul = dr.n_heads / dr.kvh[l]
    let L & = unsafe(g_crd.layers[dr.kv_layer[l]])
    let dt = g_crd.kv_dt
    let kmir = g_crd.k_slabs[L.mir_slab]
    let vmir = g_crd.v_slabs[L.mir_slab]
    let rowwg = da_attn_row_wgs(dr.n_heads, kv_mul, 1l)
    let mirbase = uint(rdec_mir_region_base(L.mir_base, r, g_crd.mir_units))
    // the pass's count is the anchor and its query a row past the last key, so a sliding layer reads one key short of its window
    let win = dr.sliding[l] && L.window > 0l ? uint(L.window - 1l) : 0u
    var pca = DaAttnArgs(kvd = uint(L.kvd), hs = hs, kv_mul = uint(kv_mul), kbase = mirbase, vbase = mirbase, scale = 1.0, qstride = 0u, gated = 0u,
        window = win, cap = 0.0, nsplit = 1u, rqk = dt == KVDtype.tq4 ? DA_RQ_NONE : DA_RQ_Q8, sinkoff = 0u,
        rowwg = uint(rowwg), qrow = qd, cntoff = uint(g.cntoff), ring = uint(L.ring))
    var abufs = fixed_array(g.q, kmir, vmir, g.attn, g.tok.dev, g.part, g.aq, g.aqs)
    let g2 = da_slab_is_g2(kv_mul)
    if (dt == KVDtype.f16) {
        if (g2) {
            enc_da_attn_f16_g2_cls(s, abufs, pca, rowwg)
        } else {
            enc_da_attn_f16_cls(s, abufs, pca, rowwg)
        }
    } elif (dt == KVDtype.tq4) {
        if (g2) {
            enc_da_attn_tq4_g2_cls(s, abufs, pca, rowwg)
        } else {
            enc_da_attn_tq4_cls(s, abufs, pca, rowwg)
        }
    } elif (kv_block_codec(dt)) {
        if (g2) {
            enc_da_attn_q8_g2_cls(s, abufs, pca, rowwg)
        } else {
            enc_da_attn_q8_cls(s, abufs, pca, rowwg)
        }
    } elif (g2) {
        enc_da_attn_g2_cls(s, abufs, pca, rowwg)
    } else {
        enc_da_attn_cls(s, abufs, pca, rowwg)
    }
}

[arch(at="../ARCHITECTURE_GPU_MTP_ASSISTANT.md#assistant-drafter-step")]
def private cud_encode_layer(g : GemmaDrafterCud; dr : GemmaDrafter; var s : void?; l, r : int64) {
    let hs = dr.hs[l]
    let qd = dr.n_heads * hs
    let L & = unsafe(g_crd.layers[dr.kv_layer[l]])
    let dt = g_crd.kv_dt
    gemv_site(g, s, g_layer(l, 0), g.xq, g.xs, g.q)
    // the q heads' norm and the rope on the query alone (no k head: nothing stores); the row's class rides its capture layer's rope rows
    var pcn = GkQknRopeArgs(qd = uint(qd), kvd = uint(L.kvd), hs = uint(hs), half = uint(hs / 2l), neox = g_crd.neox ? 1u : 0u,
        nh = uint(dr.n_heads), layerbase = 0u, qwoff = uint(dr.q_norm_offs[l]), kwoff = 0u, eps = dr.eps, qstride = uint(hs),
        boff = 0u, hasb = 0u, cbase = uint(L.cbase), vfk = 0u, vnorm = 0u, rowwg = uint(dr.n_heads), kvrow = uint(L.kvd), qrow = uint(qd),
        cosrow = uint(g_crd.cos_elems), ring = 0u)
    enc_qkn_rope_cls(s, fixed_array(g.q, g.q, g.q, g.q, g.cos.dev, g.fblob, g.tok.dev, g.q, g.q), pcn, dr.n_heads)
    if (dt == KVDtype.tq4) {   // the query rotates into the stored rows' basis
        var pct = KvTqStoreArgs(kvd = uint(L.kvd), hs = uint(hs), layerbase = 0u, pos0 = 0u, nqh = uint(dr.n_heads), nkh = 0u,
            qrow = uint(qd), qstride = uint(hs), inv_sqrt_hs = 1.0 / sqrt(float(hs)), ring = 0u)
        enc_kv_tq4_store_cls(s, fixed_array(g.q, g.q, g.q, g.q, g.q, g_crd.tq4_signs, g.tok.dev), pct, dr.n_heads)
    }
    attn_site(g, dr, s, l, r)
    if (dt == KVDtype.tq4) {
        var pcu = KvTqUnrotArgs(hs = uint(hs), nh = uint(dr.n_heads), orow = uint(qd), qrow = uint(qd), qstride = uint(hs), inv_sqrt_hs = 1.0 / sqrt(float(hs)))
        enc_kv_tq4_unrot_cls(s, fixed_array(g.attn, g.q, g_crd.tq4_signs), pcu, dr.n_heads)
        rq_site(s, g.attn, g.aq, g.aqs, qd)
    }
    gemv_site(g, s, g_layer(l, 1), g.aq, g.aqs, g.o)
    ar_site(g, dr, s, true, dr.ffn_norm_offs[l], dr.post_attn_norm_offs[l], 1.0)
    gemv_site(g, s, g_layer(l, 2), g.xq, g.xs, g.gate)
    gemv_site(g, s, g_layer(l, 3), g.xq, g.xs, g.up)
    var pc_act = ActArgs(nelem = uint(dr.hidden), act = ACT_GELU, nblk = uint(dr.hidden / Q8_BLOCK_ELEMS), elem0 = 0u)
    enc_q8_actrq_cls(s, fixed_array(g.gate, g.up, g.hq, g.hqs), pc_act, rq_wgs(false, dr.hidden))
    gemv_site(g, s, g_layer(l, 4), g.hq, g.hqs, g.o)
    let nxt = l + 1l < dr.n_layers ? dr.attn_norm_offs[l + 1l] : dr.out_norm_off
    ar_site(g, dr, s, true, nxt, dr.post_ffn_norm_offs[l], dr.layer_scale[l])
}

//! `bytes` of device `src` landed at byte `off` of the mapped block `m` over the copy engine (the driver's landing form)
def private land(var s : void?; m : CudMapped; off : int64; src : uint64; bytes : int64) {
    unsafe {
        cuda_memcpy_dtoh(reinterpret<void?>(reinterpret<uint8?>(m.host) + off), src, uint64(bytes), s)
    }
}

//! the draft graph of region `r` at depth `k`: the seed and the carried hidden off the mapped in block, then `k` chained steps -
//! step i gathers the token row its id names (the seed's at step 0, the last step's pick after it) beside h (the carry at step 0,
//! the last step's post-projection after it), runs the drafter, picks its argmax into the next step's id and lands it in `picks[i + 1]`
[arch(at="../ARCHITECTURE_GPU_MTP.md#mtp-round-one-join"), arch(at="../ARCHITECTURE_GPU_VULKAN_MTP.md#resident-assistant-drafter")]
def private cud_encode_chain(g : GemmaDrafterCud; dr : GemmaDrafter; r, k : int64) {
    var s = g_cud.stream
    let tb = dr.tdim * 4l
    cud_words(s, g.inp.dev, g.ids, 1l)
    cud_words(s, g.inp.dev + 16ul, g.cat + uint64(tb), dr.tdim)
    for (i in range64(k)) {
        if (i > 0l) {
            cud_words(s, g.hnext, g.cat + uint64(tb), dr.tdim)
        }
        cud_rdec_emb_gather(s, g.ids, g.cat, 1l, g_crd.emb_scale)
        rq_site(s, g.cat, g.catq, g.cats, 2l * dr.tdim)
        gemv_site(g, s, G_PRE, g.catq, g.cats, g.xin)
        ar_site(g, dr, s, false, dr.attn_norm_offs[0], 0l, 1.0)
        for (l in range64(dr.n_layers)) {
            cud_encode_layer(g, dr, s, l, r)
        }
        gemv_site(g, s, G_CLS, g.xq, g.xs, g.logits)
        var pcp = ArgmaxArgs(vocab = uint(dr.vocab))
        enc_cls_argmax_part(s, fixed_array(g.logits, g.pmax, g.pid), pcp, int64(CLS_ARGMAX_CHUNKS))
        enc_cls_argmax_fin(s, fixed_array(g.pmax, g.pid, g.ids), pcp, 1l)
        gemv_site(g, s, G_POST, g.xq, g.xs, g.hnext)
        land(s, g.picks, (i + 1l) * 4l, g.ids, 4l)
    }
    land(s, g.lout, 0l, g.logits, dr.vocab * 4l)
    land(s, g.lout, dr.vocab * 4l, g.hnext, tb)
}

//! the draft graph of region `r` at depth `k`, captured once: the chain's launches as one executable graph
[cold_path]
def private cud_draft_graph(var g : GemmaDrafterCud; dr : GemmaDrafter; r, k : int64) {
    let ci = r * (DRAFT_K_MAX + 1l) + k
    g.graphs[ci] = cuda_graph_capture(g_cud.stream) {
        cud_encode_chain(g, dr, r, k)
    }
    if (g.graphs[ci] == null) {
        panic("dasLLAMA cuda gemma drafter: the draft graph did not capture (region {r}, depth {k}): {cuda_last_error()}")
    }
}

//! `k` chained drafts of the stream in region `r` from (`seed`, `h`) at the constant anchor: the drafts land in `out[1..k]`
[arch(at="../ARCHITECTURE_GPU_VULKAN_MTP.md#resident-assistant-drafter")]
def private cud_draft_chain(t : Model; r, seed, anchor, k : int64; h : array<float>; var out : array<int64>) {
    var g & = g_cud_g
    let dr & = g_cud_dr
    let ci = r * (DRAFT_K_MAX + 1l) + k
    unsafe {
        var tm = reinterpret<TokMeta?>(g.tok.host)
        tm[0] = TokMeta(pos = uint(anchor), cnt = uint(anchor))
        var ip = reinterpret<uint8?>(g.inp.host)
        *reinterpret<uint?>(ip) = uint(seed)
        memcpy(reinterpret<void?>(ip + 16l), addr<void?>(h[0]), dr.tdim * 4l)
    }
    g_cud_cosrow |> grow_resize(g_crd.cos_elems)
    gpu_resident_rope_row(t, anchor, g_cud_cosrow, 0l)
    unsafe {
        memcpy(g.cos.host, addr<void?>(g_cud_cosrow[0]), g_crd.cos_elems * 4l)
    }
    if (g.graphs[ci] == null) {
        cud_draft_graph(g, dr, r, k)
    }
    if (!cuda_graph_launch(g.graphs[ci], g_cud.stream) || !cud_sync()) {   // nolint:PERF026 — cu_ok's text is built on its failure arm alone
        panic("dasLLAMA cuda gemma drafter: the draft graph failed on the device (region {r}, depth {k}): {cuda_last_error()}")
    }
    unsafe {
        let pp = reinterpret<uint?>(g.picks.host)
        for (i in range64(1l, k + 1l)) {
            out[i] = int64(pp[i])
        }
    }
}

var private @scratch @exact_size g_cud_cosrow : array<float>

//! One drafter step on the device against the region `s` holds (the oracle cell's GPU side): the draft token and the next h
def public cud_gemma_draft_step(t : Model; var s : Session; tok, anchor : int64; h : array<float>; var out_tok : int64&; @exact_size var h_next : array<float>) : bool {
    return false if (!g_cud_attached)
    g_cud_taken |> clear()
    let r = gpu_resident_assist_bind(t, s, anchor, 1l, g_cud_taken)
    return false if (r < 0l)
    g_cud_one |> resize(2)
    cud_draft_chain(t, r, tok, anchor, 1l, h, g_cud_one)
    out_tok = g_cud_one[1]
    h_next |> grow_resize(g_cud_dr.tdim)
    unsafe {
        memcpy(addr<void?>(h_next[0]), reinterpret<void?>(reinterpret<uint8?>(g_cud_g.lout.host) + g_cud_dr.vocab * 4l), g_cud_dr.tdim * 4l)
    }
    return true
}

//! The last device draft step's full-vocab logits (the oracle comparison reads them; the round never does)
def public cud_gemma_drafter_last_logits(@exact_size var dst : array<float>) {
    dst |> grow_resize(g_cud_dr.vocab)
    unsafe {
        memcpy(addr<void?>(dst[0]), g_cud_g.lout.host, g_cud_dr.vocab * 4l)
    }
}

var private @scratch g_cud_one : array<int64>
var private @scratch g_cud_taken : array<int64>

//! The CUDA attach: `""` with the drafter attached to the trunk the resident driver serves whole, else why it refused. One
//! drafter at a time; the driver then lands the post-norm hidden every step (the drafter's h input)
[arch(at="../ARCHITECTURE_GPU_VULKAN_MTP.md#resident-assistant-drafter"), arch(at="../ARCHITECTURE_GPU_MTP_ASSISTANT.md#assistant-drafter-attach")]
def private cud_attach(t : Model; path : string) : string {
    cud_detach()
    if (!moe_gpu_resident_active() || g_crd == null || !g_crd.ready) {   // before the sidecar's read: a trunk this driver does not serve takes no drafter here
        return "the CUDA resident driver does not serve the trunk whole (the load log names why)"
    }
    var dr <- load_gemma_drafter(path, t.config.n_layers)
    if (!dr.ok) {
        let why = clone_string(dr.why)
        delete dr
        return why
    }
    var why = gemma_drafter_target_why(t, dr)
    if (empty(why)) {
        var facts <- cud_trunk_facts()
        why = cud_drafter_trunk_why(facts, dr)
        delete facts
    }
    if (empty(why) && !cud_drafter_init(dr, g_cud_g)) {
        why = "the device has no room for the drafter's {long_length(dr.blob) >> 20l} MB of planes, or a class it launches did not compile"
    }
    if (empty(why) && !cud_rdec_carry_arm(true)) {
        why = "the driver took no carry landing"
    }
    if (!empty(why)) {
        cud_buffers_release(g_cud_g)
        delete dr
        return why
    }
    delete g_cud_dr
    g_cud_dr <- dr
    gpu_resident_assist_set(true)
    set_mtp_round_kind(MtpRoundKind.assistant)
    g_cud_attached = true
    discrete_round_reset(g_cud_round)
    to_log(LOG_INFO, "dasLLAMA gemma drafter attached on the CUDA resident driver: {path} ({g_cud_dr.n_layers} Q-only layers over target layers {g_cud_dr.kv_layer})\n")
    return ""
}

def private cud_detach {
    return if (!g_cud_attached)
    cud_rdec_carry_arm(false)
    gpu_resident_assist_set(false)
    cud_buffers_release(g_cud_g)
    delete g_cud_dr
    set_mtp_round_kind(MtpRoundKind.nextn)
    g_cud_attached = false
}

//! the model drop sweeps the device the drafter's objects sit on: it frees its own, forgets them and stands detached
def private cud_drop_forget {
    return if (!g_cud_attached)
    cud_buffers_release(g_cud_g)
    delete g_cud_dr
    set_mtp_round_kind(MtpRoundKind.nextn)
    g_cud_attached = false
}

[unused_argument(t), hot_path]   //! one drafter a process: the attach checked it against its target
def private cud_attached_to(t : Model) : bool => g_cud_attached

//! The gemma speculative ROUNDS of a tick's streams at depth `k_req` on the CUDA resident driver (`discrete_gemma_rounds` over
//! this home's draft graphs). Returns the warm streams the verify carried
[hot_path, arch(at="../ARCHITECTURE_GPU_VULKAN_MTP.md#resident-assistant-drafter"), arch(at="../ARCHITECTURE_GPU_MTP_ASSISTANT.md#assistant-drafter-rounds")]
def private cud_gemma_rounds(t : Model; var sessions : array<Session?>; toks : array<int64>; k_req : int64; var acc : array<int64>) : int64 {
    return discrete_gemma_rounds(t, sessions, toks, k_req, acc, g_cud_round, "gemma drafter on the CUDA resident driver - a stream steps plain this round: ", g_cud_dr.tdim) $(r, seed, anchor, k : int64; h : array<float>; var out : array<int64>) {
        cud_draft_chain(t, r, seed, anchor, k, h, out)
    }
}

def private cud_seam_stats() : MtpAssistantStats => g_cud_round.stats

[init]
def private cud_gemma_mtp_init {
    register_gpu_home("cuda", @@cud_gemma_mtp_install)
    return if (!gpu_want_cuda())   // the home registers nothing where the Vulkan home holds the tier
    cud_gemma_mtp_install()
}

//! the CUDA drafter's seams: at [init] where the home is asked for, or when `set_gpu_home("cuda")` moves the tier here
def private cud_gemma_mtp_install {
    register_cud_drop_hook(@@cud_drop_forget)
    register_mtp_assistant("cuda", MtpAssistantSeams(attach = @@cud_attach, attached = @@cud_attached_to,
        rounds = @@cud_gemma_rounds, detach = @@cud_detach, stats = @@cud_seam_stats))
}
