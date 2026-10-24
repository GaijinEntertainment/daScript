options gen2
options indenting = 4
options _dasllama_internal = true

module dasllama_cuda_decode shared public
options _function_length = 0   // the token chain's encode is one walk over the layers, as the Vulkan driver's is

require dasllama/dasllama_lint public
require dasllama/dasllama_cuda_common public
require dasllama/dasllama_cuda_classes public
require dasllama/dasllama_gpu_tier
require dasllama/dasllama_env
require dasllama/dasllama_kqformat
require dasllama/dasllama_gemm_schema
require dasllama/dasllama_kv_dtype
require dasllama/dasllama_kv_codec
require dasllama/dasllama_gpu_math
require dasllama/dasllama_gpu_kernels_common
require dasllama/dasllama_gpu_kernels_discrete
require math

// The CUDA whole-model driver, the dense slice: the resident seats (`install_moe_gpu_resident`) over the CUDA home's
// kernels - a dense model's planes in one arena a format, the K/V mirror slabs, the token step as one captured graph
// replayed a token (the per-step inputs ride pinned host buffers a words-copy kernel reads), the prefill as the window
// chain over the f16-fed cm2 tiles (the Vulkan home's window, a window of the engine's prompt rows at a time) or the
// home's own int8 MMQ GEMM where every Q8_0 shape fits its tile and DASLLAMA_CUDA_MMQ asks it. What the
// slice declines, by name, before a byte uploads: a recurrent or routed layer, a NextN head, a gated or biased
// attention, a plane format outside the arena set. The mirror holds a region a stream; the token step's chain runs
// one row or the N-row command's rows, captured as a graph a row count and mode; the window chain holds one session's
// rows or several sessions' chunks as segments, each into its own region.


struct CudArena {
    cap_blocks : int64
    blocks : int64
    qsb : int64   //! quant bytes a block
    ssb : int64   //! scale bytes a block
    wq : uint64
    ws : uint64
}

var private g_cud_arenas : table<int; CudArena>

def private cud_arena_reserve(fmt : int; cap_blocks : int64) : bool {
    if (key_exists(g_cud_arenas, fmt)) {   // a reload places over the old planes
        g_cud_arenas[fmt].blocks = 0l
        return g_cud_arenas[fmt].cap_blocks >= cap_blocks
    }
    let b = dev_block_bytes(kq_fmt_of_slot(fmt))
    let wq = cuda_mem_alloc(uint64(cap_blocks * b.qsb))
    let ws = cuda_mem_alloc(uint64(max(cap_blocks * b.ssb, 64l)))
    if (wq == 0ul || ws == 0ul) {
        to_log(LOG_WARNING, "dasLLAMA cuda arena: no room for {cap_blocks} blocks of format {fmt}: {cuda_last_error()}\n")
        cuda_mem_free(wq)
        cuda_mem_free(ws)
        return false
    }
    g_cud_arenas[fmt] = CudArena(cap_blocks = cap_blocks, qsb = b.qsb, ssb = b.ssb, wq = wq, ws = ws)
    return true
}

//! the plane's block index in its format's arena, -1 where the arena refuses it
def private cud_arena_place(wq : array<uint8>; ws : array<uint8>; n, rows : int64; fmt : int) : int64 {
    if (!key_exists(g_cud_arenas, fmt)) {
        to_log(LOG_WARNING, "dasLLAMA cuda arena: a plane in format {fmt} with no arena reserved\n")
        return -1l
    }
    let need = kq_blocks_for(n, rows, fmt)
    var a & = unsafe(g_cud_arenas[fmt])
    if (a.blocks + need > a.cap_blocks) {
        to_log(LOG_WARNING, "dasLLAMA cuda arena: format {fmt} holds {a.blocks} of {a.cap_blocks} blocks, the plane needs {need} ({rows} x {n})\n")
        return -1l
    }
    if (long_length(wq) < need * a.qsb || long_length(ws) < need * a.ssb) {
        to_log(LOG_WARNING, "dasLLAMA cuda arena: format {fmt} plane bytes {long_length(wq)} / {long_length(ws)} short of {need} blocks ({a.qsb} + {a.ssb} a block)\n")
        return -1l
    }
    let blk = a.blocks
    unsafe {
        if (!cuda_memcpy_htod(a.wq + uint64(blk * a.qsb), addr<void?>(wq[0]), uint64(need * a.qsb), g_cud.stream) ||
            (a.ssb > 0l && !cuda_memcpy_htod(a.ws + uint64(blk * a.ssb), addr<void?>(ws[0]), uint64(need * a.ssb), g_cud.stream))) {
            to_log(LOG_WARNING, "dasLLAMA cuda arena: the upload of {need} blocks at block {blk} failed: {cuda_last_error()}\n")
            return -1l
        }
    }
    a.blocks += need
    return blk
}

def private cud_arena_free {
    for (a in values(g_cud_arenas)) {
        cuda_mem_free(a.wq)
        cuda_mem_free(a.ws)
    }
    g_cud_arenas |> clear()
}

//! every arena's quant and scale bytes on the device
def cud_arena_bytes : int64 {
    var total = 0l
    for (a in values(g_cud_arenas)) {
        total += a.blocks * (a.qsb + a.ssb)
    }
    return total
}

//! one window GEMM's region schedule on the device: the record [wblk0, 0, rows, 0] at word 0 and a zero map (every workgroup
//! region 0) from word 4; the record is rewritten only when the plane or the row count moves
struct CudSched {
    buf : uint64
    wblk : int64
    rows : int64
}

let SCH_Q = 0
let SCH_K = 1
let SCH_V = 2
let SCH_O = 3
let SCH_G = 4
let SCH_U = 5
let SCH_D = 6
let SCH_COUNT = 7

struct CudLayer {
    hs, qd, kvd, hid, rot, kv_mul : int64
    window, ring, mir_base, mir_slab, cbase : int64
    pre_post, gelu, vfk, kv_shared : bool
    out_scale, attn_cap : float
    bq, bk, bv, bo, b1, b3, b2 : int64
    fq, fk, fv, fo, f1, f3, f2 : int
    qkv_merged : bool
    nreg_qkv : int64
    regs_qkv : uint64   //! the merged q+k+v GEMV's region records, else the q plane's one record
    regs_k : uint64
    regs_v : uint64
    regs_o, regs_gate, regs_up, regs_down : uint64   //! the unfused sites' one-record region buffers (a fused Q8_0 site carries its block in its args)
    wo_ar, gate_up_fused, down_ar : bool   //! the Q8_0 fused sites the layer takes: wo and down with the residual epilogue, gate and up with the act
    sch : CudSched[7]   //! the window chain's GEMM schedules, one a role (SCH_*)
}

struct CudRdec {
    ready : bool
    n_layers, dim, qd, kv_dim, n_heads, hidden, vocab : int64
    neox : bool
    qk_norm : bool       //! the per-head q/k norm rides the rope pass (`CuQknRopeKv`), its weight rows in the norms plane past the final norm
    head_size : int64    //! the widest head class: the q/k norm rows' stride in the norms plane
    eps, scale : float
    kv_dt : KVDtype
    unit_bytes : int64   //! a mirror unit's bytes: the element's under a scalar codec
    mir_units : int64    //! a region's units a slab, the slack included
    regions : int64
    region : int64       //! the selected region (`cud_rdec_select_region`): the one-row command's row and the window chain's rows land in it
    nb : int64           //! the rows the N-row command steps at once, the per-row planes' height (CRD_NB)
    cos_elems : int64
    @exact_size layers : array<CudLayer>
    k_slabs, v_slabs : array<uint64>
    x, xq, xs, kv, attn, attn_part, ar_cnt, aq, as_, hq, hs_, xb2, norms, cos, logits, pick_part_max, pick_part_id, tok, dummy, supp, cls_regs : uint64
    xb : uint64                 //! the rows' normed residual ahead of a K-quant feed's requant (the N-row command's split step)
    gate, up, ffnout : uint64   //! the unfused sites' rows: the gate and up GEMVs' outputs ahead of the act, the down GEMV's ahead of the residual step
    kst, vst : uint64           //! a block-codec mirror's staged K and V row: the rope stamp writes floats here, the codec's store quantizes them into the mirror
    stage_tok : uint64          //! the staged row's token block, all zeros: position 0 in region 0, so the rope stamp lands the row at the staged plane's row 0
    ksh, vsh : uint64           //! a block-codec mirror's f16 shadow, one layer's rows at the region's context: the window chain's attention tiles read it
    tq4_signs : uint64          //! tq4's sign row (`tq4_fill_signs`, KV_TQ4_SIGNS floats): the rotation's and the un-rotation's
    attn_nsplit, attn_cnt_off : int64
    norms_bytes, ar_scal_off : int64
    in_host, out_host : void?   //! the step's mapped pinned blocks: the inputs (the TokMeta words, the token id, the rope row, the x row) the head copies read, the pick and the logits the tail's kernels write
    in_dev : uint64             //! ... as the device addresses the inputs; the out block's address is `pick`
    pick : uint64               //! the out block's first word: the argmax's own store lands the pick on the host
    emb_blk : int64             //! the token table's arena block the one-row command gathers off; -1 = the host's row rides the in block
    emb_fmt : int               //! the table's format: its arena and its gather leaf
    emb_scale : float
    cls_blk : int64
    cls_fmt : int
    cls_cap : float
    nsupp : int64
    graphs : void? [128]        //! the token step's captured graphs by row count and mode (`crd_graph_key`): bit 0 the logits down, bit 1 the rows gathered off their ids, bit 2 the attention unsplit
    unsplit_on : bool           //! the attention's unsplit form is a form of its own: the device split the keys at all
    unsplit_steps : int64       //! the steps the unsplit form served
    tokens_served : int64   //! the positions the driver served, the window chain's and the token step's
    token_steps : int64     //! the token steps alone: the first runs the chain plainly, the second captures the graph
    windows : int64         //! the window chains run
    seg_windows : int64     //! the windows that held several sessions' chunks as segments
    //! window buffers (crd_pf_setup): np rows; f16 feeds + read slack
    pf_np : int64
    pf_x, pf_xf, pf_q, pf_kv, pf_v, pf_attn, pf_af, pf_xb2, pf_gate, pf_up, pf_hf, pf_ffnout, pf_cos, pf_xq, pf_xs : uint64
    pf_aq, pf_as, pf_hq, pf_hs : uint64   //! the MMQ chain's Q8_0 feeds of the wo and down GEMMs (the x feed is pf_xq / pf_xs)
    pf_xsum, pf_asum, pf_hsum : uint64    //! the feeds' blocks' scale x quant sum beside each scale plane: the min-term leaves' feed, allocated under pf_mmq_min
    pf_kst, pf_vst : uint64   //! a block-codec mirror's staged K and V rows of the window, the codec's window store's source
    pf_mmq_min : bool        //! some window site takes an MMQ leaf with a min term (Q4_K, Q5_1): the requant kernels write the block sums beside the scales
    pf_splitk : uint64       //! the split-k partial planes (the Vulkan home's scratch bound: 256 KB an SM)
    pf_splitk_bytes : int64
    pf_sched_words : int64   //! a schedule buffer's words: the record and the widest role's map at the deepest split
}

var g_crd : CudRdec?

def private crd_free_buffers(var r : CudRdec) {
    for (b in [r.x, r.xq, r.xs, r.kv, r.attn, r.attn_part, r.ar_cnt, r.aq, r.as_, r.hq, r.hs_, r.xb2, r.norms, r.cos, r.logits, r.pick_part_max, r.pick_part_id, r.tok, r.dummy, r.supp, r.cls_regs,
                r.gate, r.up, r.ffnout, r.xb, r.kst, r.vst, r.stage_tok, r.ksh, r.vsh, r.tq4_signs]) {
        cuda_mem_free(b)
    }
    for (b in [r.pf_x, r.pf_xf, r.pf_q, r.pf_kv, r.pf_v, r.pf_attn, r.pf_af, r.pf_xb2, r.pf_gate, r.pf_up, r.pf_hf, r.pf_ffnout, r.pf_cos, r.pf_xq, r.pf_xs, r.pf_splitk,
                r.pf_aq, r.pf_as, r.pf_hq, r.pf_hs, r.pf_xsum, r.pf_asum, r.pf_hsum, r.pf_kst, r.pf_vst]) {
        cuda_mem_free(b)
    }
    for (b in r.k_slabs) {
        cuda_mem_free(b)
    }
    for (b in r.v_slabs) {
        cuda_mem_free(b)
    }
    for (L in r.layers) {
        for (b in [L.regs_qkv, L.regs_k, L.regs_v, L.regs_o, L.regs_gate, L.regs_up, L.regs_down]) {
            cuda_mem_free(b)
        }
        for (sc in L.sch) {
            cuda_mem_free(sc.buf)
        }
    }
    cuda_mem_free_host(r.in_host)
    cuda_mem_free_host(r.out_host)
    crd_graphs_drop(r)
}

//! a step replayed as a graph in any mode
def private crd_graph_captured(r : CudRdec) : bool {
    for (g in r.graphs) {
        return true if (g != null)
    }
    return false
}

def private crd_graphs_drop(var r : CudRdec) {
    for (g in r.graphs) {
        cuda_graph_destroy(g)
        g = null
    }
}

let private CRD_NB = RDEC_NROW_MAX                      //! the rows the N-row command steps at once: the per-row planes' height
let private CRD_TOK_WORDS = int(TOK_META_BYTES / 4l)   //! a TokMeta block in words
let private CRD_IN_ID_WORD = CRD_TOK_WORDS * int(CRD_NB)   //! the in block: CRD_NB TokMeta blocks, the rows' token ids from this word, the rope rows from the header's end, then the x rows
let private CRD_IN_HDR_WORDS = CRD_IN_ID_WORD + int(CRD_NB)
let private CRD_OUT_LOGITS_WORDS = int(CRD_NB)          //! the out block: a pick a row, then the rows' logits from this word

//! the driver's state dropped: its buffers, the mirror slabs, the graph; the arenas stay (the reserve seat precedes prepare)
def private crd_drop {
    return if (g_crd == null)
    cud_sync()
    crd_free_buffers(*g_crd)
    unsafe {
        delete g_crd
    }
    g_crd = null
}

//! the model's device state dropped: the driver's state and the arenas; the device stays up for the next model
def cud_drop_model {
    crd_drop()
    cud_arena_free()
}

def cud_resident_bytes : int64 => cud_arena_bytes() + (g_crd != null && g_crd.ready ? 2l * long_length(g_crd.k_slabs) * g_crd.regions * g_crd.mir_units * g_crd.unit_bytes : 0l)

def private cud_decline(why : string) : bool {
    to_log(LOG_WARNING, "dasLLAMA cuda resident: {why} - the driver declines\n")
    return false
}

//! a zero-filled device block; the clear rides g_cud.stream like every write a kernel reads - the stream-less memset runs on
//! the legacy stream, which the home's non-blocking stream never waits for, and a large clear then lands after the first
//! kernels wrote into the block
def private dev_zeroed(bytes : int64) : uint64 {
    let d = cuda_mem_alloc(uint64(bytes))
    if (d != 0ul) {
        cuda_memset_d8(d, 0u8, uint64(bytes), g_cud.stream)
    }
    return d
}

//! the plane formats the slice serves: Q8_0, the superblock K-quants and the dense per-32 formats (their GEMV leaves, cm2 tiles and
//! table gathers on the home)
def private cud_fmt_ok(fmt : int) : bool => fmt == int(KqFmt.q8) || kq_sb(fmt) || kq_dense_b32(fmt)

def cud_rdec_reserve(fmt : int; cap_blocks : int64) : bool {
    return false if (!cud_init())
    if (!cud_fmt_ok(fmt)) {
        return cud_decline("a plane in format {kq_fmt_of_slot(fmt)}: the dense slice serves Q8_0, the superblock K-quant and the dense per-32 planes")
    }
    return cud_arena_reserve(fmt, cap_blocks)
}

def cud_rdec_place(wq : array<uint8>; ws : array<uint8>; n, rows : int64; fmt : int) : int64 {
    return -1l if (!cud_init() || empty(wq) || !cud_fmt_ok(fmt) || !kq_rows_ok(n, fmt))
    return cud_arena_place(wq, ws, n, rows, fmt)
}

[unused_argument(rot)]   //! the seat's signature; the layers carry their own rotation width
def cud_rdec_prepare(n_layers, n_attn, dim, qd, kv_dim, head_size, n_heads, hidden, vocab, seq_cap : int64;
                     neox : bool; eps, scale : float; qk_norm : bool; kv_dtype : int; q_gated : bool; rot : int64; dn : RdecDnGeom;
                     mir_units, n_slabs, cos_elems, regions : int64; carry, head : bool; verify_rows : int64) : bool {
    return false if (!cud_init())
    let dt = kv_dtype_of(kv_dtype)
    if (dn.cd > 0l) return cud_decline("a recurrent layer (deltanet)")
    if (n_attn != n_layers) return cud_decline("{n_layers - n_attn} layers without attention")
    if (head) return cud_decline("a NextN draft head")
    if (carry) return cud_decline("the post-norm carry")
    if (q_gated) return cud_decline("a gated query")
    if (regions < 1l) return cud_decline("{regions} mirror regions")
    if (dim > RDEC_AR_MAX_DIM || head_size > 512l || kv_dim > RDEC_AR_MAX_DIM) return cud_decline("dim {dim}, kv_dim {kv_dim} or head_size {head_size} past the row stashes")
    if (verify_rows > 0l) return cud_decline("verify rows")
    let refused <- cud_ensure_all()   // every kernel of the home loads before the first chain: a launch precedes no ensure, and a capture loads nothing
    if (!empty(refused)) return cud_decline("{length(refused)} kernels did not compile ({refused[0]} first)")
    crd_drop()
    g_crd = new CudRdec(n_layers = n_layers, dim = dim, qd = qd, kv_dim = kv_dim, n_heads = n_heads, hidden = hidden, vocab = vocab,
        neox = neox, qk_norm = qk_norm, head_size = head_size, eps = eps, scale = scale, kv_dt = dt, regions = regions, cos_elems = cos_elems)
    var r & = unsafe(*g_crd)
    r.unit_bytes = kv_unit_bytes(dt)
    r.mir_units = mir_units + kv_row_units(dt, RDEC_MIR_SLACK)
    r.nb = CRD_NB
    let nb = r.nb
    let wide = max(max(qd, hidden), dim)
    r.x = dev_zeroed(dim * 4l * nb)
    r.xq = dev_zeroed(wide * nb)
    r.xs = dev_zeroed((wide / Q8_BLOCK_ELEMS) * 4l * nb)
    r.kv = dev_zeroed((qd + 2l * kv_dim) * 4l * nb)
    r.attn = dev_zeroed(qd * 4l * nb)
    r.attn_nsplit = da_nsplit(n_heads, int64(g_cud.sm_count))
    r.unsplit_on = r.attn_nsplit > 1l
    r.attn_cnt_off = n_heads * max(r.attn_nsplit, 1l) * (head_size + 2l) * nb   // the pieces of every row; a counter a (row, kv head, slab) group past them
    r.attn_part = dev_zeroed((r.attn_cnt_off + n_heads * nb) * 4l)
    r.ar_cnt = dev_zeroed(16l)
    r.aq = dev_zeroed(qd * nb)
    r.as_ = dev_zeroed((qd / Q8_BLOCK_ELEMS) * 4l * nb)
    r.hq = dev_zeroed(hidden * nb)
    r.hs_ = dev_zeroed((hidden / Q8_BLOCK_ELEMS) * 4l * nb)
    r.gate = dev_zeroed(hidden * 4l * nb)
    r.up = dev_zeroed(hidden * 4l * nb)
    r.ffnout = dev_zeroed(dim * 4l * nb)
    r.xb2 = dev_zeroed(dim * 4l * nb)
    r.xb = dev_zeroed(dim * 4l * nb)
    if (kv_block_codec(dt)) {
        r.kst = dev_zeroed(kv_dim * 4l * nb)
        r.vst = dev_zeroed(kv_dim * 4l * nb)
        var stage : array<TokMeta>   // row i of the staged planes: position i in region 0, so the rope stamp lands each row at its own staged row
        stage |> resize(nb)
        for (i in range64(nb)) {
            stage[i] = TokMeta(pos = uint(i))
        }
        r.stage_tok = buf_upload(stage, g_cud.stream)
        r.ksh = dev_zeroed((seq_cap * kv_dim + RDEC_MIR_SLACK) * 2l)
        r.vsh = dev_zeroed((seq_cap * kv_dim + RDEC_MIR_SLACK) * 2l)
    }
    if (dt == KVDtype.tq4) {
        var signs : array<float>
        tq4_fill_signs(signs, KV_TQ4_SIGNS)
        r.tq4_signs = buf_upload(signs, g_cud.stream)
    }
    r.cos = dev_zeroed(cos_elems * 4l * nb)
    r.logits = dev_zeroed(vocab * 4l * nb)
    r.pick_part_max = dev_zeroed(4l * int64(CLS_ARGMAX_CHUNKS) * nb)
    r.pick_part_id = dev_zeroed(4l * int64(CLS_ARGMAX_CHUNKS) * nb)
    r.tok = dev_zeroed(int64(CRD_IN_HDR_WORDS) * 4l)   // the in block's header as the kernels read it: the rows' TokMeta blocks, then the ids the gather reads
    r.dummy = dev_zeroed(256l)
    r.ar_scal_off = rdec_norms_len(n_layers, dim, qk_norm ? head_size : 0l, false)
    r.norms_bytes = (r.ar_scal_off + 2l) * 4l
    r.norms = dev_zeroed(r.norms_bytes)
    let slab_bytes = regions * r.mir_units * r.unit_bytes
    r.k_slabs |> reserve(max(n_slabs, 1l))
    r.v_slabs |> reserve(max(n_slabs, 1l))
    for (_si in range64(max(n_slabs, 1l))) {
        let k = dev_zeroed(slab_bytes)
        let v = dev_zeroed(slab_bytes)
        r.k_slabs |> push(k)
        r.v_slabs |> push(v)
        if (k == 0ul || v == 0ul) {
            return cud_decline("the K/V mirror ({(2l * slab_bytes * max(n_slabs, 1l)) >> 20l} MB in {max(n_slabs, 1l)} slab pairs at ctx {seq_cap}) found no room on the device")
        }
    }
    let inb = cuda_mem_alloc_host_mapped(uint64((int64(CRD_IN_HDR_WORDS) + (cos_elems + dim) * nb) * 4l))
    let outb = cuda_mem_alloc_host_mapped(uint64((int64(CRD_OUT_LOGITS_WORDS) + vocab * nb) * 4l))
    r.in_host = inb.host
    r.in_dev = inb.dev
    r.out_host = outb.host
    r.pick = outb.dev
    if (r.in_host == null || r.out_host == null) return cud_decline("the step's mapped host blocks did not allocate ({cuda_last_error()})")
    r.emb_blk = -1l
    r.layers |> reserve(n_layers)
    r.layers |> resize(n_layers)
    return true
}

def cud_rdec_upload_norms(norms : array<float>; obias : bool) {
    assert(g_crd != null, "cud_rdec_upload_norms before prepare")
    if (obias) {
        to_log(LOG_WARNING, "dasLLAMA cuda resident: the attention output bias rows are uploaded but the dense slice's epilogue does not add them\n")
    }
    unsafe {
        cuda_memcpy_htod(g_crd.norms, addr<void?>(norms[0]), uint64(long_length(norms) * 4l), g_cud.stream)
        var pair : float[2]
        pair[0] = g_crd.eps
        pair[1] = 1.0
        cuda_memcpy_htod(g_crd.norms + uint64(g_crd.ar_scal_off * 4l), addr<void?>(pair[0]), 8ul, g_cud.stream)
    }
}

//! a region-record buffer of `pairs` (rel_wblock, xblock) words
def private cud_regions(pairs : array<uint>) : uint64 => buf_upload(pairs, g_cud.stream)

[unused_argument(mir_idx)]
def cud_rdec_set_layer(l, mir_idx : int64; bq, bk, bv, bo, b1, b3, b2 : int64; fq, fk, fv, fo, f1, f3, f2 : int; g : RdecLayerGeom) {
    assert(g_crd != null, "cud_rdec_set_layer before prepare")
    var r & = unsafe(*g_crd)
    var L & = unsafe(r.layers[l])
    L.hs = g.hs; L.qd = g.qd; L.kvd = g.kvd; L.hid = g.hid; L.rot = g.rot
    L.kv_mul = g.kvd > 0l ? r.n_heads / (g.kvd / g.hs) : 1l
    L.pre_post = g.pre_post; L.gelu = g.gelu; L.vfk = g.vfk; L.kv_shared = g.kv_shared
    L.out_scale = g.out_scale; L.attn_cap = g.attn_cap
    L.window = g.window; L.ring = g.ring; L.mir_base = g.mir_base; L.mir_slab = g.mir_slab; L.cbase = g.cbase
    L.bq = bq; L.bk = bk; L.bv = bv; L.bo = bo; L.b1 = b1; L.b3 = b3; L.b2 = b2
    L.fq = fq; L.fk = fk; L.fv = fv; L.fo = fo; L.f1 = f1; L.f3 = f3; L.f2 = f2
    let q8 = int(KqFmt.q8)
    verify(cud_fmt_ok(fq) && cud_fmt_ok(fk) && cud_fmt_ok(fv) && cud_fmt_ok(fo) && cud_fmt_ok(f1) && cud_fmt_ok(f3) && cud_fmt_ok(f2),
        "cud decode: a plane in a format the slice does not serve reached set_layer - the place seat admits the arena formats alone")
    verify(!L.kv_shared && !L.vfk, "cud decode: a shared-KV or V-from-K layer - the dense slice projects q, k and v")
    // a fused site's epilogue requants into its consumer's feed, so it fuses only where that consumer reads Q8_0 too
    let fnext = l + 1l < r.n_layers ? r.layers[l + 1l].fq : r.cls_fmt
    L.wo_ar = fo == q8 && !kq_sb(f1)
    L.gate_up_fused = f1 == q8 && f3 == q8 && !kq_sb(f2)
    L.down_ar = f2 == q8 && !kq_sb(fnext)
    L.qkv_merged = L.qd % L.kvd == 0l && fq == fk && fk == fv   // one GEMV over the three planes needs them in one arena
    L.regs_o = cud_regions([uint(bo), 0u])
    L.regs_gate = cud_regions([uint(b1), 0u])
    L.regs_up = cud_regions([uint(b3), 0u])
    L.regs_down = cud_regions([uint(b2), 0u])
    if (L.qkv_merged) {
        // kvd-row regions (q's qd / kvd, then k, then v) fill the projection row in one dispatch; a record counts the format's blocks
        let piece_blocks = uint(kq_blocks_for(r.dim, L.kvd, fq))
        let npieces = L.qd / L.kvd
        var pairs : array<uint>
        pairs |> reserve(2l * npieces + 4l)
        for (p in range64(npieces)) {
            pairs |> push(uint(bq) + uint(p) * piece_blocks)
            pairs |> push(0u)
        }
        pairs |> push(uint(bk))
        pairs |> push(0u)
        pairs |> push(uint(bv))
        pairs |> push(0u)
        L.nreg_qkv = long_length(pairs) / 2l
        L.regs_qkv = cud_regions(pairs)
    } else {
        L.regs_qkv = cud_regions([uint(bq), 0u])
        L.regs_k = cud_regions([uint(bk), 0u])
        L.regs_v = cud_regions([uint(bv), 0u])
        L.nreg_qkv = 1l
    }
    crd_graphs_drop(r)   // the chain references the replaced layer
}

def cud_rdec_set_cls(cls_block : int64; cls_fmt : int; cap : float; supp : array<int64>) {
    assert(g_crd != null, "cud_rdec_set_cls before prepare")
    var r & = unsafe(*g_crd)
    verify(cud_fmt_ok(cls_fmt), "cud decode: a classifier in a format the slice does not serve reached set_cls")
    r.cls_blk = cls_block
    r.cls_fmt = cls_fmt
    if (r.n_layers > 0l) {   // the last layer's fused down site reads the classifier's feed format, known only now
        var LL & = unsafe(r.layers[r.n_layers - 1l])
        LL.down_ar = LL.f2 == int(KqFmt.q8) && !kq_sb(cls_fmt)
    }
    r.cls_cap = cap
    r.nsupp = long_length(supp)
    if (r.nsupp > 0l) {
        var words <- [for (s in supp); uint(s)]
        r.supp = buf_upload(words, g_cud.stream)
    }
    r.cls_regs = cud_regions([uint(cls_block), 0u])
    crd_graphs_drop(r)
    r.ready = true
}

//! the one-row command and the ids prefill gather off the table at `emb_block` through `fmt`'s gather leaf (`cud_emb_enc`)
def cud_rdec_set_emb(emb_block : int64; fmt : int; escale : float) : bool {
    assert(g_crd != null, "cud_rdec_set_emb before prepare")
    return false if (!cud_fmt_ok(fmt))
    var r & = unsafe(*g_crd)
    r.emb_blk = emb_block
    r.emb_fmt = fmt
    r.emb_scale = escale
    crd_graphs_drop(r)
    return true
}

[unused_argument(fblob, off, vocab, dim, escale)]
def cud_rdec_upload_emb_f32(fblob : array<float>; off, vocab, dim : int64; escale : float) : bool => false   //! no f32 table on the slice: the host embeds those rows

def private norm_off(l, kind : int64) : int64 => rdec_norm_off(l, kind, g_crd.dim)

def private ar_wgs(rows : int64) : int64 => ar_epilogue_wgs(rows, g_cud.rows_per_wg)

//! `words` words from `src` to `dst` on the compute queue
def private crd_words(var s : void?; src, dst : uint64; words : int64) {
    var pc = WordsArgs(n = uint(words))
    enc_words_copy(s, fixed_array(src, dst), pc, words_copy_wgs(words))
}

//! the mapped out block's host side at word `word`
def private crd_out_host(r : CudRdec; word : int64) : void? {
    unsafe {
        return reinterpret<void?>(reinterpret<uint8?>(r.out_host) + word * 4l)
    }
}

//! `words` words of device `src` landed at word `word` of the mapped out block over the copy engine (a kernel writing the mapped
//! block lands a vocabulary row in 90 us, the engine in a fifth of it)
def private crd_land(var s : void?; r : CudRdec; src : uint64; word, words : int64) {
    cuda_memcpy_dtoh(crd_out_host(r, word), src, uint64(words * 4l), s)
}

//! a decode GEMV over the plane at the region records `regs` in format `fmt`: `nreg` regions of `d` rows against the
//! `n`-wide feed (xq, xs) into y from `ybase`; `ncols` activation rows past one through the N-column leaf, their y rows
//! `ystride` apart
def private crd_gemv(var s : void?; fmt : int; regs, xq, xs, y : uint64; n, d : int64; ybase : uint; nreg : int64 = 1l; ncols : uint = 1u; ystride : uint = 0u) {
    let a = g_cud_arenas[fmt]
    var pc = GemvArgs(n = uint(n), d = uint(d), nreg = uint(nreg), ybase = ybase, ncols = ncols, ystride = ystride)
    cud_gemv_enc(fmt, s, fixed_array(a.wq, a.ws, regs, xq, xs, y), pc, nreg * d)
}

//! the residual step x += add under the next norm into the feed of a plane in format `fmt`, a row a workgroup over `rows`
//! rows: Q8_0 blocks in one dispatch, Q8_K superblocks for a K-quant consumer - fused at one row, the step's normed rows
//! then their requant past it (the fused Q8_K form quantizes one row)
def private crd_ar_rq(r : CudRdec; var s : void?; fmt : int; add : uint64; var pc : GkAddRmsArgs; rows : int64) {
    var bufs = fixed_array(r.x, add, r.norms, r.xq, r.xs)
    if (!kq_sb(fmt)) {
        enc_cls_ar_rq_b(s, bufs, pc, rows)
    } elif (rows == 1l) {
        enc_cls_ar_rqk(s, bufs, pc, rows)
    } else {
        enc_cls_ar(s, fixed_array(r.x, add, r.norms, r.xb), pc, rows)
        var pcq = RqArgs(inbase = 0u, nblk = uint(r.dim * rows / kq_unit(true)), lo = 0.0, hi = 0.0)
        enc_cls_q8k_rq(s, fixed_array(r.xb, r.xq, r.xs), pcq, rq_wgs(true, r.dim * rows))
    }
}

def private crd_attn_rqk(L : CudLayer) : uint => da_rqk_pick(g_crd.kv_dt == KVDtype.tq4, L.fo, L.hs)

//! layer `l`'s FFN over `nrows` rows; the down epilogue fuses only at one row (a last-workgroup epilogue would serialize rows)
def private crd_ffn(r : CudRdec; var s : void?; l : int64; L : CudLayer; nlfin : uint; nrows : int64) {
    let dim = r.dim
    let act = L.gelu ? ACT_GELU : ACT_SILU
    let nr = uint(nrows)
    if (L.gate_up_fused) {
        let a1 = g_cud_arenas[L.f1]
        var pcgu = GuNArgs(n = uint(dim), d = uint(L.hid), gblk = uint(L.b1), ublk = uint(L.b3), act = act, ncols = nr)
        cud_gu_n_enc(s, fixed_array(a1.wq, a1.ws, a1.wq, a1.ws, r.xq, r.xs, r.hq, r.hs_), pcgu, L.hid / 32l)
    } else {
        crd_gemv(s, L.f1, L.regs_gate, r.xq, r.xs, r.gate, dim, L.hid, 0u, 1l, nr, uint(L.hid))
        crd_gemv(s, L.f3, L.regs_up, r.xq, r.xs, r.up, dim, L.hid, 0u, 1l, nr, uint(L.hid))
        let kq2 = kq_sb(L.f2)
        var pca2 = ActArgs(nelem = uint(L.hid * nrows), act = act, nblk = uint(L.hid * nrows / kq_unit(kq2)), elem0 = 0u)
        if (kq2) {
            enc_q8k_actrq_cls(s, fixed_array(r.gate, r.up, r.hq, r.hs_), pca2, rq_wgs(true, L.hid * nrows))
        } else {
            enc_q8_actrq_cls(s, fixed_array(r.gate, r.up, r.hq, r.hs_), pca2, rq_wgs(false, L.hid * nrows))
        }
    }
    let nxt = l + 1l < r.n_layers ? uint(norm_off(l + 1l, RDEC_NORM_ATT)) : nlfin
    let fnext = l + 1l < r.n_layers ? r.layers[l + 1l].fq : r.cls_fmt
    if (L.down_ar && nrows == 1l) {
        let a2 = g_cud_arenas[L.f2]
        var pcda = GemvArArgs(n = uint(L.hid), d = uint(dim), wblk = uint(L.b2), xblk = 0u, add_on = 1u,
            woff = nxt, pwoff = uint(norm_off(l, RDEC_NORM_POST_FFN)), pre_on = L.pre_post ? 1u : 0u,
            no_norm = 0u, eps = r.eps, ascale = L.out_scale)
        enc_q8_gemv_ar_cls(s, fixed_array(a2.wq, a2.ws, r.hq, r.hs_, r.xb2, r.x, r.norms, r.xq, r.xs, r.ar_cnt), pcda, ar_wgs(dim))
    } else {
        crd_gemv(s, L.f2, L.regs_down, r.hq, r.hs_, r.ffnout, L.hid, dim, 0u, 1l, nr, uint(dim))
        var pc14 = GkAddRmsArgs(dim = uint(dim), add_on = 1u, woff = nxt, eps = r.eps, ascale = L.out_scale,
            pre_on = L.pre_post ? 1u : 0u, pwoff = uint(norm_off(l, RDEC_NORM_POST_FFN)))
        crd_ar_rq(r, s, fnext, r.ffnout, pc14, nrows)
    }
}

//! the fused q/k norm, rope and K/V store pass's args for layer `l`: the q heads then the k heads a workgroup each (`rowwg`), the
//! norm rows past the final norm at the widest head's stride, the bias rows absent (the slice declines a biased attention)
def private crd_qkn_rope_args(r : CudRdec; l : int64; L : CudLayer; layerbase, nkh, kvrow, qrow, ring : int64) : GkQknRopeArgs {
    let qkbase = rdec_norms_qk_base(r.n_layers, r.dim)   // nolint:LINT021 — the norm row offsets multiply 64-bit before the arg-field narrow
    return GkQknRopeArgs(qd = uint(L.qd), kvd = uint(L.kvd), hs = uint(L.hs), half = uint(L.rot / 2l), neox = r.neox ? 1u : 0u,
        nh = uint(r.n_heads), layerbase = uint(layerbase),
        qwoff = uint(qkbase + l * 2l * r.head_size), kwoff = uint(qkbase + (l * 2l + 1l) * r.head_size),
        eps = r.eps, qstride = uint(L.hs), boff = 0u, hasb = 0u, cbase = uint(L.cbase), vfk = L.vfk ? 1u : 0u, vnorm = 0u,
        rowwg = uint(r.n_heads + nkh), kvrow = uint(kvrow), qrow = uint(qrow), cosrow = uint(r.cos_elems), ring = uint(ring))
}

//! the rows' rope on q and k, and k and v into the mirror at each row's position and region (the row's token block); under a
//! block codec the rope stages the floats (row i of the staged planes, addressed through the staged token blocks) and the
//! codec's store quantizes them into the mirror; a q/k-normed model's rows take the fused pass instead
def private crd_token_kv(r : CudRdec; var s : void?; l : int64; L : CudLayer; kmir, vmir : uint64; kvrow, nrows : int64) {
    let qd = L.qd
    let kvd = L.kvd
    let hs = L.hs
    let kvq = kv_block_codec(r.kv_dt)
    if (r.qk_norm) {
        //! the fused pass, a head a workgroup over the rows: the q heads, then the k heads (each storing its V row); the norm rows
        //! sit past the final norm at the widest head's stride, as the Vulkan home's token command reads them
        let nkh = L.kv_shared ? 0l : kvd / hs
        var pcf = crd_qkn_rope_args(r, l, L, kvq ? 0l : L.mir_base, nkh, kvrow, kvrow, kvq ? 0l : L.ring)
        var qbufs = fixed_array(r.kv, r.kv, kvq ? r.kst : kmir, kvq ? r.vst : vmir, r.cos, r.norms, kvq ? r.stage_tok : r.tok, r.dummy, r.kv)
        if (r.kv_dt == KVDtype.f16) {
            enc_qkn_rope_f16_cls(s, qbufs, pcf, (r.n_heads + nkh) * nrows)
        } else {
            enc_qkn_rope_cls(s, qbufs, pcf, (r.n_heads + nkh) * nrows)
        }
    } else {
        let qpairs = qd / 2l
        let npairs = qpairs + kvd / 2l
        var pcr = gk_rope_kv_args(qd, kvd, hs, qpairs, npairs, r.neox, false, hs, 0l, L.cbase, nrows, kvrow, kvrow, r.cos_elems, kvq ? 0l : L.mir_base, L.ring)
        let rwgs = (npairs * nrows + int64(CUD_WG_X) - 1l) / int64(CUD_WG_X)
        var rbufs = fixed_array(r.kv, r.kv, r.kv, kvq ? r.kst : kmir, kvq ? r.vst : vmir, r.dummy, r.cos, r.cos, kvq ? r.stage_tok : r.tok)
        if (r.kv_dt == KVDtype.f16) {
            enc_rope_kv_f16_cls(s, rbufs, pcr, rwgs)
        } else {
            enc_rope_kv_cls(s, rbufs, pcr, rwgs)
        }
    }
    if (r.kv_dt == KVDtype.tq4) {
        let nkh = kvd / hs
        var pct = KvTqStoreArgs(kvd = uint(kvd), hs = uint(hs), layerbase = uint(L.mir_base), pos0 = 0u, nqh = uint(r.n_heads), nkh = uint(nkh),
            qrow = uint(kvrow), qstride = uint(hs), inv_sqrt_hs = 1.0 / sqrt(float(hs)), ring = uint(L.ring))
        enc_kv_tq4_store_cls(s, fixed_array(r.kv, r.kst, r.vst, kmir, vmir, r.tq4_signs, r.tok), pct, nrows * (r.n_heads + 2l * nkh))
    } elif (kvq) {
        var pcs = KvQStoreArgs(kvd = uint(kvd), rows = uint(nrows), layerbase = uint(L.mir_base), pos0 = 0u, ring = uint(L.ring))
        enc_kv_q8_store_cls(s, fixed_array(r.kst, r.vst, kmir, vmir, r.tok), pcs, kv_store_wgs(kvd * nrows))
    }
}

//! a layer's attention over the mirror into the wo feed, `nrows` rows a row a workgroup: each row's quants, or its floats where the
//! fold cannot block them (the fold it took, `DA_RQ_*`, returned); the keys in one piece a group under `unsplit` (the Vulkan
//! home's unsplit twin), else in the device's split, the last-arriving piece combining
def private crd_token_attn(r : CudRdec; var s : void?; L : CudLayer; kmir, vmir : uint64; kvrow, nrows : int64; unsplit : bool) : uint {
    let hs = uint(L.hs)
    let mir_base = uint(L.mir_base)   // the layer's base: each row's token block adds its region
    let nsplit = unsplit ? 1l : r.attn_nsplit
    let rowwgs = da_attn_row_wgs(r.n_heads, L.kv_mul, nsplit)
    let rqk = crd_attn_rqk(L)
    var pca = DaAttnArgs(kvd = uint(L.kvd), hs = hs, kv_mul = uint(L.kv_mul), kbase = mir_base, vbase = mir_base,
        scale = r.scale, qstride = hs, gated = 0u, window = uint(L.window), cap = L.attn_cap, nsplit = uint(nsplit),
        rqk = rqk, sinkoff = 0u, rowwg = uint(rowwgs), qrow = uint(kvrow), cntoff = uint(r.attn_cnt_off), ring = uint(L.ring))
    var abufs = fixed_array(r.kv, kmir, vmir, r.attn, r.tok, r.attn_part, r.aq, r.as_)
    let g2 = da_slab_is_g2(L.kv_mul)
    let awgs = rowwgs * nrows
    if (r.kv_dt == KVDtype.f16) {
        if (g2) {
            enc_da_attn_f16_g2_cls(s, abufs, pca, awgs)
        } else {
            enc_da_attn_f16_cls(s, abufs, pca, awgs)
        }
    } elif (r.kv_dt == KVDtype.tq4) {
        if (g2) {
            enc_da_attn_tq4_g2_cls(s, abufs, pca, awgs)
        } else {
            enc_da_attn_tq4_cls(s, abufs, pca, awgs)
        }
        crd_tq4_unrot(r, s, r.attn, r.kv, L.hs, nrows, L.qd, kvrow)
    } elif (kv_block_codec(r.kv_dt)) {
        if (g2) {
            enc_da_attn_q8_g2_cls(s, abufs, pca, awgs)
        } else {
            enc_da_attn_q8_cls(s, abufs, pca, awgs)
        }
    } elif (g2) {
        enc_da_attn_g2_cls(s, abufs, pca, awgs)
    } else {
        enc_da_attn_cls(s, abufs, pca, awgs)
    }
    return rqk
}

//! tq4's un-rotation of `rows` attention rows of `orow` elements in `y` (the q plane `q` at `qrow` a row, read for a gate it has not)
def private crd_tq4_unrot(r : CudRdec; var s : void?; y, q : uint64; hs, rows, orow, qrow : int64) {
    var pcu = KvTqUnrotArgs(hs = uint(hs), nh = uint(r.n_heads), orow = uint(orow), qrow = uint(qrow), qstride = uint(hs), inv_sqrt_hs = 1.0 / sqrt(float(hs)))
    enc_kv_tq4_unrot_cls(s, fixed_array(y, q, r.tq4_signs), pcu, r.n_heads * rows)
}

//! the token step's launches over `nrows` rows on the device stream: the rows' inputs off the mapped in block (the
//! token blocks and the ids, the rope rows, the x rows - or their gather off the ids), the layers with every GEMV an N-column
//! dispatch past one row and every row kernel a row a workgroup, each row at its own position, count and region (its token
//! block), the classifier, the picks stored into the mapped out block, the rows' logits copied there when asked - captured
//! once a row count and mode into the graph the steps replay (the Vulkan home's one recorder over both commands); `unsplit`
//! the attention's one-piece form, the rows' keys under the unsplit position
[arch(at="../ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-token-command"), arch(at="../ARCHITECTURE_GPU_SHARED_ATTN.md#vk-decode-attn-split")]
def private crd_chain(var r : CudRdec; nrows : int64; logits_on, gathered, unsplit : bool) {
    var s = g_cud.stream
    let dim = r.dim
    let nr = uint(nrows)
    let one = nrows == 1l
    crd_words(s, r.in_dev, r.tok, int64(CRD_IN_HDR_WORDS))
    crd_words(s, r.in_dev + uint64(CRD_IN_HDR_WORDS * 4), r.cos, r.cos_elems * nrows)
    if (gathered) {
        let ae = g_cud_arenas[r.emb_fmt]
        cud_emb_enc(r.emb_fmt, s, ae.wq, ae.ws, r.tok + uint64(CRD_IN_ID_WORD * 4), r.x, r.x, nrows, dim, r.emb_blk, r.emb_scale)
    } else {
        crd_words(s, r.in_dev + uint64((int64(CRD_IN_HDR_WORDS) + r.cos_elems * nrows) * 4l), r.x, dim * nrows)
    }
    var pc_pro = GkAddRmsArgs(dim = uint(dim), add_on = 0u, woff = 0u, eps = r.eps, ascale = 1.0)
    crd_ar_rq(r, s, r.n_layers > 0l ? r.layers[0].fq : r.cls_fmt, r.x, pc_pro, nrows)
    let nlfin = uint(rdec_norms_final_base(r.n_layers, dim))
    for (l in range64(r.n_layers)) {
        let L & = unsafe(r.layers[l])
        let qd = L.qd
        let kvd = L.kvd
        let kvrow = qd + 2l * kvd
        if (L.qkv_merged) {
            crd_gemv(s, L.fq, L.regs_qkv, r.xq, r.xs, r.kv, dim, kvd, 0u, L.nreg_qkv, nr, uint(kvrow))
        } else {
            crd_gemv(s, L.fq, L.regs_qkv, r.xq, r.xs, r.kv, dim, qd, 0u, 1l, nr, uint(kvrow))
            crd_gemv(s, L.fk, L.regs_k, r.xq, r.xs, r.kv, dim, kvd, uint(qd), 1l, nr, uint(kvrow))
            crd_gemv(s, L.fv, L.regs_v, r.xq, r.xs, r.kv, dim, kvd, uint(qd + kvd), 1l, nr, uint(kvrow))
        }
        let kmir = r.k_slabs[L.mir_slab]
        let vmir = r.v_slabs[L.mir_slab]
        crd_token_kv(r, s, l, L, kmir, vmir, kvrow, nrows)
        let rqk = crd_token_attn(r, s, L, kmir, vmir, kvrow, nrows, unsplit)
        if (rqk == DA_RQ_NONE && kq_sb(L.fo)) {   // no fold (a head off the superblock, or tq4): the separate Q8_K requant
            var pcq = RqArgs(inbase = 0u, nblk = uint(qd * nrows / kq_unit(true)), lo = 0.0, hi = 0.0)
            enc_cls_q8k_rq(s, fixed_array(r.attn, r.aq, r.as_), pcq, rq_wgs(true, qd * nrows))
        } elif (rqk == DA_RQ_NONE) {   // no fold under tq4: the separate Q8_0 requant
            var pcq = RqArgs(inbase = 0u, nblk = uint(qd * nrows / kq_unit(false)), lo = 0.0, hi = 0.0)
            enc_cls_dn_rq(s, fixed_array(r.attn, r.aq, r.as_), pcq, rq_wgs(false, qd * nrows))
        }
        let ao = g_cud_arenas[L.fo]
        if (L.wo_ar && one) {
            var pcwa = GemvArArgs(n = uint(qd), d = uint(dim), wblk = uint(L.bo), xblk = 0u, add_on = 1u,
                woff = uint(norm_off(l, RDEC_NORM_FFN)), pwoff = uint(norm_off(l, RDEC_NORM_POST_ATT)), pre_on = L.pre_post ? 1u : 0u,
                no_norm = 0u, eps = r.eps, ascale = 1.0, abias_on = 0u, aboff = 0u)
            enc_q8_gemv_ar_cls(s, fixed_array(ao.wq, ao.ws, r.aq, r.as_, r.xb2, r.x, r.norms, r.xq, r.xs, r.ar_cnt), pcwa, ar_wgs(dim))
        } else {
            crd_gemv(s, L.fo, L.regs_o, r.aq, r.as_, r.xb2, qd, dim, 0u, 1l, nr, uint(dim))
            var pc8 = GkAddRmsArgs(dim = uint(dim), add_on = 1u, woff = uint(norm_off(l, RDEC_NORM_FFN)), eps = r.eps, ascale = 1.0,
                pre_on = L.pre_post ? 1u : 0u, pwoff = uint(norm_off(l, RDEC_NORM_POST_ATT)))
            crd_ar_rq(r, s, L.f1, r.xb2, pc8, nrows)
        }
        crd_ffn(r, s, l, L, nlfin, nrows)
    }
    crd_gemv(s, r.cls_fmt, r.cls_regs, r.xq, r.xs, r.logits, dim, r.vocab, 0u, 1l, nr, uint(r.vocab))
    if (r.cls_cap > 0.0 || r.nsupp > 0l) {
        var pce = GkClsEpiArgs(vocab = uint(r.vocab), cap = r.cls_cap, nsupp = uint(r.nsupp), rows = nr)
        enc_cls_epi(s, fixed_array(r.logits, r.nsupp > 0l ? r.supp : r.dummy), pce, cls_epi_wgs(r.vocab, nrows))
    }
    var pcp = ArgmaxArgs(vocab = uint(r.vocab))
    enc_cls_argmax_part(s, fixed_array(r.logits, r.pick_part_max, r.pick_part_id), pcp, nrows * int64(CLS_ARGMAX_CHUNKS))
    enc_cls_argmax_fin(s, fixed_array(r.pick_part_max, r.pick_part_id, r.pick), pcp, nrows)
    if (logits_on) {
        crd_land(s, r, r.logits, int64(CRD_OUT_LOGITS_WORDS), r.vocab * nrows)
    }
}

//! a captured graph's slot: the row count and the mode (bit 0 the logits down, bit 1 the rows gathered off their ids, bit 2 the
//! attention unsplit)
def private crd_graph_key(nrows : int64; logits_on, gathered, unsplit : bool) : int => int(nrows - 1l) * 8 + (logits_on ? 1 : 0) | (gathered ? 2 : 0) | (unsplit ? 4 : 0)

//! the chain as a graph, captured on the row count and mode's first replay
def private crd_ensure_graph(var r : CudRdec; nrows : int64; logits_on, gathered, unsplit : bool) : bool {
    let key = crd_graph_key(nrows, logits_on, gathered, unsplit)
    return true if (r.graphs[key] != null)
    r.graphs[key] = cuda_graph_capture(g_cud.stream) {
        crd_chain(r, nrows, logits_on, gathered, unsplit)
    }
    if (r.graphs[key] == null) {
        to_log(LOG_ERROR, "dasLLAMA cuda resident: the token step's graph did not capture ({nrows} rows): {cuda_last_error()}\n")
        return false
    }
    return true
}

//! the step over the in block's rows: the first step runs the chain plainly (every kernel loads, then the graph captures the
//! next of its row count and mode), every later one replays its graph; the stream synced, so the out block holds the rows;
//! `maxcnt` the rows' widest key count, which picks the attention's unsplit form under the unsplit position
def private crd_launch(var r : CudRdec; nrows : int64; logits_on, gathered : bool; maxcnt : int64) {
    let unsplit = r.unsplit_on && maxcnt < RD_UNSPLIT_POS
    if (r.token_steps == 0l) {
        crd_chain(r, nrows, logits_on, gathered, unsplit)
    } elif (!crd_ensure_graph(r, nrows, logits_on, gathered, unsplit) || !cuda_graph_launch(r.graphs[crd_graph_key(nrows, logits_on, gathered, unsplit)], g_cud.stream)) {
        panic("dasLLAMA cuda resident: the token step did not launch: {cuda_last_error()}")
    }
    if (!cud_sync()) {
        panic("dasLLAMA cuda resident: the token step failed on the device: {cuda_last_error()}")
    }
    r.token_steps++
    if (unsplit) {
        r.unsplit_steps++
    }
}

//! one position's step: the token block (its region the selected one), the id, the rope row and (with no id) the x row written
//! into the mapped in block, the chain, the pick and the asked logits read off the mapped out block
def private crd_step(var r : CudRdec; x : array<float>; xoff : int64; cossin : array<float>; coff : int64; pos, cnt, token : int64; logits_on : bool) {
    let gathered = token >= 0l
    unsafe {
        var w = reinterpret<uint?>(r.in_host)
        var tm = reinterpret<TokMeta?>(r.in_host)
        *tm = TokMeta(pos = uint(pos), cnt = uint(cnt), mirbase = uint(r.region * r.mir_units))
        w[CRD_IN_ID_WORD] = gathered ? uint(token) : 0u
        memcpy(addr<void?>(w[CRD_IN_HDR_WORDS]), addr<void?>(cossin[coff]), r.cos_elems * 4l)
        if (!gathered) {
            memcpy(addr<void?>(w[int64(CRD_IN_HDR_WORDS) + r.cos_elems]), addr<void?>(x[xoff]), r.dim * 4l)
        }
    }
    crd_launch(r, 1l, logits_on, gathered, cnt)
    r.tokens_served++
}

def private crd_landed_pick(r : CudRdec) : int64 {
    let pk = *unsafe(reinterpret<uint?>(r.out_host))
    if (int64(pk) >= r.vocab) {
        panic("dasLLAMA cuda resident: the device landed no pick ({pk} of a {r.vocab} vocab)")
    }
    return int64(pk)
}

//! the out block's logits at `slot` into `logits` from `at`
def private crd_logits_out(r : CudRdec; var logits : array<float>; slot : int64 = 0l; at : int64 = 0l) {
    unsafe {
        var w = reinterpret<uint?>(r.out_host)
        memcpy(addr<void?>(logits[at]), addr<void?>(w[int64(CRD_OUT_LOGITS_WORDS) + slot * r.vocab]), r.vocab * 4l)
    }
}

[unused_argument(ple_row)]
def cud_rdec_token(x : array<float>; cossin : array<float>; ple_row : array<float>; token, pos, cnt : int64; pick_only : bool; var logits : array<float>) : int64 {
    assert(g_crd != null && g_crd.ready, "cud_rdec_token before prepare/set_cls")
    var r & = unsafe(*g_crd)
    verify(token >= 0l || long_length(x) >= r.dim, "cud decode: neither a token id nor a row")
    verify(token < 0l || r.emb_blk >= 0l, "cud decode: a token id with no placed table - the engine asks the gather only past set_emb")
    crd_step(r, x, 0l, cossin, 0l, pos, cnt, token, !pick_only)
    if (pick_only) {
        return crd_landed_pick(r)
    }
    crd_logits_out(r, logits)
    return -1l
}

//! the N-row token command: `nrows` rows (2 to the per-row planes' height) through one step, each at its position, count and
//! region, its token row gathered off its id where every row brings one (else the x rows ride the in block), each row's logits
//! landed at its own pointer, or - where the pointer is null - its argmax alone into `picks` (`RdecTokenNFn`)
[unused_argument(ple, ple_raw), arch(at="../ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-token-command")]
def cud_rdec_token_n(x : array<float>; cossin : array<float>; ple : array<float>; ple_raw : bool; ids : array<int64>; poss : array<int64>; cnts : array<int64>; regs : array<int64>; nrows : int64; lrows : array<float?>; var picks : array<int64>) {
    assert(g_crd != null && g_crd.ready, "cud_rdec_token_n before prepare/set_cls")
    var r & = unsafe(*g_crd)
    verify(nrows >= 2l && nrows <= r.nb, "cud_rdec_token_n: the row count is outside the per-row planes (a one-row step is the one-row command's)")
    verify(long_length(picks) >= nrows && long_length(lrows) >= nrows, "cud_rdec_token_n: fewer pick slots or logits rows than rows")
    verify(long_length(poss) >= nrows && long_length(cnts) >= nrows && long_length(regs) >= nrows, "cud_rdec_token_n: fewer positions, counts or regions than rows")
    var gathered = r.emb_blk >= 0l && long_length(ids) >= nrows
    var all_pick = true
    var maxcnt = 0l
    for (i in range64(nrows)) {
        gathered = gathered && ids[i] >= 0l
        all_pick = all_pick && lrows[i] == null
        maxcnt = max(maxcnt, cnts[i])
        if (regs[i] < 0l || regs[i] >= r.regions) {
            panic("cud_rdec_token_n: row {i}'s region {regs[i]} is not one of the {r.regions}")
        }
    }
    verify(gathered || long_length(x) >= r.dim * nrows, "cud_rdec_token_n: the rows are short, and not every row brings a token id to gather")
    unsafe {
        var w = reinterpret<uint?>(r.in_host)
        var tm = reinterpret<TokMeta?>(r.in_host)
        for (i in range64(nrows)) {
            tm[i] = TokMeta(pos = uint(poss[i]), cnt = uint(cnts[i]), mirbase = uint(regs[i] * r.mir_units))
            w[int64(CRD_IN_ID_WORD) + i] = gathered ? uint(ids[i]) : 0u
        }
        memcpy(addr<void?>(w[CRD_IN_HDR_WORDS]), addr<void?>(cossin[0]), r.cos_elems * 4l * nrows)
        if (!gathered) {
            memcpy(addr<void?>(w[int64(CRD_IN_HDR_WORDS) + r.cos_elems * nrows]), addr<void?>(x[0]), r.dim * 4l * nrows)
        }
    }
    crd_launch(r, nrows, !all_pick, gathered, maxcnt)
    unsafe {
        let pk = reinterpret<uint?>(r.out_host)
        let lw = reinterpret<float?>(r.out_host) + int64(CRD_OUT_LOGITS_WORDS)
        for (i in range64(nrows)) {   // pick -1 beside landed logits, as the Vulkan home lands them
            if (lrows[i] != null) {
                memcpy(reinterpret<void?>(lrows[i]), reinterpret<void?>(lw + i * r.vocab), r.vocab * 4l)
                picks[i] = -1l
                continue
            }
            if (int64(pk[i]) >= r.vocab) {
                panic("dasLLAMA cuda resident: the device landed no pick for row {i} ({pk[i]} of a {r.vocab} vocab)")
            }
            picks[i] = int64(pk[i])
        }
    }
    r.tokens_served += nrows
}

//! the rows the N-row command steps at once on the armed driver: the per-row planes' height; 0 before the driver is armed
def cud_rdec_token_n_rows : int64 => g_crd != null && g_crd.ready ? g_crd.nb : 0l

//! every mirror address of the one-row command and the window chain, and the host's row copies, resolve against region `region`
def cud_rdec_select_region(region : int64) {
    assert(g_crd != null && region >= 0l && region < g_crd.regions, "cud_rdec_select_region: no such mirror region")
    g_crd.region = region
}

//! layer `L`'s mirror base in the selected region: the window chain's rows and the host's row copies land there
def private crd_region_base(r : CudRdec; L : CudLayer; region : int64) : int64 => rdec_mir_region_base(L.mir_base, region, r.mir_units)
def private crd_mir_base(r : CudRdec; L : CudLayer) : int64 => crd_region_base(r, L, r.region)

//! the s tile where the m tiles would idle three quarters of the SMs, else cm2_wave_col at two blocks an SM
def private cud_cm2_tc(d, rows : int64) : int64 => cm2_wave_col(d, rows, int64(g_cud.sm_count), 2l * int64(g_cud.sm_count))

//! split k only while the unsplit grid is short of two blocks an SM; DASLLAMA_CM2_SPLITK overrides the count
def private cud_cm2_pick(d, rows, n : int64) : tuple<tc : int64; nsplit : int64; ksplit : int64> {
    var tc = cud_cm2_tc(d, rows)
    let cores = int64(g_cud.sm_count)
    let forced = g_env_vulkan.cm2_splitk
    var want = 1l
    if (forced > 0l) {
        want = forced
    } elif (cores > 0l && rows > 128l && tc != 32l) {
        let slots = 2l * cores
        let tiles_l = cm2_tiles_at(d, rows, 256l)
        let tiles_m = cm2_tiles_at(d, rows, 128l)
        if (tiles_l * 2l <= slots) {
            tc = 256l
            want = slots / tiles_l
        } elif (tiles_m * 2l <= slots) {
            tc = 128l
            want = slots / tiles_m
        }
    }
    let sk = cm2_split_fit(n, want, rows * d * 4l, g_crd.pf_splitk_bytes)
    return sk > 1l ? (tc = tc, nsplit = sk, ksplit = cm2_chunk(n, sk)) : (tc = tc, nsplit = 1l, ksplit = 0l)
}

def private cud_sched_make(words : int64) : CudSched => CudSched(buf = dev_zeroed(words * 4l), wblk = -1l, rows = -1l)

//! the MMQ leaves by format: the Q8_0 one, the superblock ones (Q4_K, IQ4_XS, Q4_0, IQ4_NL), the per-32 ones (Q5_1, Q5_0 and
//! the off-lattice Q4_0 and IQ4_NL); a leaf with a min term (Q4_K, Q5_1) reads the activation blocks' sums
let private MMQ_LEAF_FMTS <- [int(KqFmt.q8), int(KqFmt.k4), int(KqFmt.iq4xs), int(KqFmt.q40), int(KqFmt.iq4nl), int(KqFmt.q51), int(KqFmt.q5032), int(KqFmt.q4032), int(KqFmt.iq4nl32)]
let private MMQ_LEAF_NAMES <- ["Q8_0", "Q4_K", "IQ4_XS", "Q4_0", "IQ4_NL", "Q5_1", "Q5_0", "Q4_0 (per-32)", "IQ4_NL (per-32)"]
//! the leaves whose windows did not beat the cm2 tiles on every measured card - Q5_1 lost on both, Q5_0 won on the RTX 5060 Ti and
//! lost on the RTX PRO 4500 - so the pick between the two forms is a card's to race, not a default's; `DASLLAMA_CUDA_MMQ=2` arms them
let private MMQ_RACE_FMTS <- [int(KqFmt.q51), int(KqFmt.q5032)]

def private crd_mmq_min(fmt : int) : bool => fmt == int(KqFmt.k4) || fmt == int(KqFmt.q51)

//! whether a window GEMM site - a `d` x `n` plane of format `fmt` at block `wblk` - takes the int8 MMQ tile: the knob asks it
//! (1: the leaves the measured cards favor, 2: every leaf), the window is whole tiles, the format has a leaf, the output width
//! is a multiple of 128, the reduction of 256 and a Q8_0 plane's first block of 8 (eight scales a quad); a leaf with a min
//! term folds it sixteen blocks at a time, so its reduction is a multiple of 512
def private crd_site_mmq(r : CudRdec; fmt : int; n, d, wblk : int64) : bool {
    return false if (g_env_vulkan.cuda_mmq == 0l || r.pf_np % 128l != 0l || d % 128l != 0l || n % 256l != 0l || find_index(MMQ_LEAF_FMTS, fmt) < 0
        || (g_env_vulkan.cuda_mmq == 1l && find_index(MMQ_RACE_FMTS, fmt) >= 0))
    return wblk % 8l == 0l if (fmt == int(KqFmt.q8))
    return !crd_mmq_min(fmt) || n % 512l == 0l
}

//! the MMQ leaves the model's window sites take: whether one carries a min term (the feeds the window produces follow), and
//! their names for the load log
def private crd_mmq_leaves(r : CudRdec) : tuple<min : bool; planes : string> {
    var on : bool[9]
    for (L in r.layers) {
        let kv_merged = L.fk == L.fv && L.bv == L.bk + kq_blocks_for(r.dim, L.kvd, L.fk)
        let kd = kv_merged ? 2l * L.kvd : L.kvd
        for (site in [(fmt = L.fq, n = r.dim, d = L.qd, b = L.bq), (fmt = L.fk, n = r.dim, d = kd, b = L.bk), (fmt = L.fv, n = r.dim, d = L.kvd, b = L.bv),
                      (fmt = L.fo, n = L.qd, d = r.dim, b = L.bo), (fmt = L.f1, n = r.dim, d = L.hid, b = L.b1), (fmt = L.f3, n = r.dim, d = L.hid, b = L.b3),
                      (fmt = L.f2, n = L.hid, d = r.dim, b = L.b2)]) {
            continue if (!crd_site_mmq(r, site.fmt, site.n, site.d, site.b))
            on[find_index(MMQ_LEAF_FMTS, site.fmt)] = true
        }
    }
    var mn = false
    var planes = ""
    for (i in range(9)) {
        continue if (!on[i])
        mn = mn || crd_mmq_min(MMQ_LEAF_FMTS[i])
        planes = empty(planes) ? MMQ_LEAF_NAMES[i] : "{planes}, {MMQ_LEAF_NAMES[i]}"
    }
    return (min = mn, planes = planes)
}

//! the x feeds a layer's q/k/v sites (`qkv`) or its gate/up sites read: the Q8_0 feed where a site takes an MMQ leaf, the f16
//! feed where one takes the cm2 tiles
def private crd_x_feeds(r : CudRdec; L : CudLayer; qkv : bool) : tuple<mmq : bool; f16 : bool> {
    var mmq = false
    var f16 = false
    if (qkv) {
        let kv_merged = L.fk == L.fv && L.bv == L.bk + kq_blocks_for(r.dim, L.kvd, L.fk)
        for (site in [(fmt = L.fq, d = L.qd, b = L.bq), (fmt = L.fk, d = kv_merged ? 2l * L.kvd : L.kvd, b = L.bk), (fmt = L.fv, d = L.kvd, b = L.bv)]) {
            let m = crd_site_mmq(r, site.fmt, r.dim, site.d, site.b)
            mmq = mmq || m
            f16 = f16 || !m
        }
    } else {
        for (site in [(fmt = L.f1, b = L.b1), (fmt = L.f3, b = L.b3)]) {
            let m = crd_site_mmq(r, site.fmt, r.dim, L.hid, site.b)
            mmq = mmq || m
            f16 = f16 || !m
        }
    }
    return (mmq = mmq, f16 = f16)
}

//! the residual step's normed rows into the feeds the next sites read: the Q8_0 blocks (their sums beside where a Q4_K leaf
//! reads them), the f16 rows, or both - the second pass norms the stream the first already updated, adding nothing
def private crd_x_norm(r : CudRdec; var s : void?; aa : uint64; var pc : GkAddRmsArgs; wlen : int64; feeds : tuple<mmq : bool; f16 : bool>) {
    if (feeds.mmq) {
        if (r.pf_mmq_min) {
            enc_cls_ar_rq_sum_b(s, fixed_array(r.pf_x, aa, r.norms, r.pf_xq, r.pf_xs, r.pf_xsum), pc, wlen)
        } else {
            enc_cls_ar_rq_b(s, fixed_array(r.pf_x, aa, r.norms, r.pf_xq, r.pf_xs), pc, wlen)
        }
        if (feeds.f16) {
            var pcn = pc
            pcn.add_on = 0u
            pcn.pre_on = 0u
            enc_cls_ar_f16_b(s, fixed_array(r.pf_x, r.pf_x, r.norms, r.pf_xf), pcn, wlen)
        }
    } else {
        enc_cls_ar_f16_b(s, fixed_array(r.pf_x, aa, r.norms, r.pf_xf), pc, wlen)
    }
}

//! one window GEMM on an MMQ leaf: `rows` Q8_0-quantized activation rows of `n` (`xq` bytes, `xs` block scales, `xsum` the
//! blocks' scale x sum the Q4_K leaf reads) against the `d` x `n` plane at `wblk` - the plane's first block in its format's unit -
//! into y[rows x d]; the rows pad to the tile - the pad rows read the feed's stale rows past the window and land past it in y,
//! where nothing reads
def private crd_mmq(fmt : int; wblk : int64; a : CudArena; xq, xs, xsum, y : uint64; n, d, rows : int64) {
    let rp = (rows + 127l) / 128l * 128l
    var pc = MmqArgs(n = uint(n), d = uint(d), rows = uint(rp), wblk = uint(wblk))
    let wgs = mmq_wgs(d, rp)
    var bufs5 = fixed_array(a.wq, a.ws, xq, xs, y)
    var bufs6 = fixed_array(a.wq, a.ws, xq, xs, y, xsum)
    if (fmt == int(KqFmt.k4)) {
        enc_k4_mmq(g_cud.stream, bufs6, pc, wgs)
    } elif (fmt == int(KqFmt.q51)) {
        enc_q51_mmq(g_cud.stream, bufs6, pc, wgs)
    } elif (fmt == int(KqFmt.iq4xs)) {
        enc_iq4xs_mmq(g_cud.stream, bufs5, pc, wgs)
    } elif (fmt == int(KqFmt.q40)) {
        enc_q40_mmq(g_cud.stream, bufs5, pc, wgs)
    } elif (fmt == int(KqFmt.iq4nl)) {
        enc_iq4nl_mmq(g_cud.stream, bufs5, pc, wgs)
    } elif (fmt == int(KqFmt.q4032)) {
        enc_q4032_mmq(g_cud.stream, bufs5, pc, wgs)
    } elif (fmt == int(KqFmt.q5032)) {
        enc_q5032_mmq(g_cud.stream, bufs5, pc, wgs)
    } elif (fmt == int(KqFmt.iq4nl32)) {
        enc_iq4nl32_mmq(g_cud.stream, bufs5, pc, wgs)
    } else {
        enc_q8_mmq(g_cud.stream, bufs5, pc, wgs)
    }
}

//! a window GEMM site on whichever feed its plane takes: the Q8_0 feed on an MMQ leaf, else the f16 feed on the plane format's cm2 tiles
def private crd_site(r : CudRdec; var sc : CudSched; fmt : int; wblk : int64; xf, xq, xs, xsum, y : uint64; n, d, rows : int64) {
    let a = g_cud_arenas[fmt]
    if (crd_site_mmq(r, fmt, n, d, wblk)) {
        crd_mmq(fmt, wblk, a, xq, xs, xsum, y, n, d, rows)
    } else {
        crd_gemm(sc, fmt, wblk, a, xf, y, n, d, rows)
    }
}

//! the schedule's record at (wblk, rows), rewritten where either moved
def private cud_sched_set(var sc : CudSched; wblk, rows : int64) {
    return if (sc.wblk == wblk && sc.rows == rows)
    var rec : uint[4]
    rec[0] = uint(wblk)
    rec[1] = 0u
    rec[2] = uint(rows)
    rec[3] = 0u
    cuda_memcpy_htod(sc.buf, unsafe(addr<void?>(rec[0])), 16ul, g_cud.stream)
    sc.wblk = wblk
    sc.rows = rows
}

//! one window GEMM: `rows` f16 activation rows of `n` against the `d` x `n` plane at `wblk` in format `fmt` into y[rows x d], on
//! the tile and the k split the shape picks - a split lands its partial planes in the scratch and the reduce sums them into y
def private crd_gemm(var sc : CudSched; fmt : int; wblk : int64; a : CudArena; xf, y : uint64; n, d, rows : int64) {
    cud_sched_set(sc, wblk, rows)
    let pk = cud_cm2_pick(d, rows, n)
    let split = pk.nsplit > 1l
    let wgs = cm2_tiles_at(d, rows, pk.tc) * pk.nsplit
    if (4l + wgs > g_crd.pf_sched_words) {
        panic("cud prefill: a window GEMM's map ({wgs} workgroups) passes the schedule buffer ({g_crd.pf_sched_words} words)")
    }
    var pc = BatchArgs(n = uint(n), d = uint(d), map_off = 4u, ksplit = split ? uint(pk.ksplit) : 0u)
    var bufs = fixed_array(a.wq, a.ws, sc.buf, xf, split ? g_crd.pf_splitk : y)
    cud_cm2_enc(fmt, pk.tc == 256l ? 0 : (pk.tc == 128l ? 1 : 2), g_cud.stream, bufs, pc, wgs)
    if (split) {
        var pr = SkRedArgs(nelem = uint(rows * d), k = uint(pk.nsplit), ybase = 0u)
        enc_splitk_reduce_cls(g_cud.stream, fixed_array(g_crd.pf_splitk, y), pr, splitk_reduce_wgs(rows * d))
    }
}

//! the window chain's planes, once a residency: np rows a plane at the model's widest layer, the f16 feeds carrying the tiles'
//! read slack, a schedule buffer a role a layer
def private crd_pf_setup(var r : CudRdec) {
    return if (r.pf_np > 0l)
    let np = pf_window_rows()
    let dim = r.dim
    var qd = r.qd
    var kvd = r.kv_dim
    var hid = r.hidden
    for (L in r.layers) {
        qd = max(qd, L.qd)
        kvd = max(kvd, L.kvd)
        hid = max(hid, L.hid)
    }
    let slack = np + TILE_READ_SLACK
    r.pf_np = np
    r.pf_x = dev_zeroed(np * dim * 4l)
    r.pf_xf = dev_zeroed(slack * dim * 2l)
    r.pf_q = dev_zeroed(np * qd * 4l)
    r.pf_kv = dev_zeroed(np * 2l * kvd * 4l)
    r.pf_v = dev_zeroed(np * kvd * 4l)
    if (kv_block_codec(r.kv_dt)) {
        r.pf_kst = dev_zeroed(np * kvd * 4l)
        r.pf_vst = dev_zeroed(np * kvd * 4l)
    }
    r.pf_attn = dev_zeroed(np * qd * 4l)
    r.pf_af = dev_zeroed(slack * qd * 2l)
    r.pf_xb2 = dev_zeroed(np * dim * 4l)
    r.pf_gate = dev_zeroed(np * hid * 4l)
    r.pf_up = dev_zeroed(np * hid * 4l)
    r.pf_hf = dev_zeroed(slack * hid * 2l)
    r.pf_ffnout = dev_zeroed(np * dim * 4l)
    r.pf_cos = dev_zeroed(np * r.cos_elems * 4l)
    r.pf_xq = dev_zeroed(np * dim)
    r.pf_xs = dev_zeroed(np * (dim / Q8_BLOCK_ELEMS) * 4l)
    let leaves = crd_mmq_leaves(r)
    r.pf_mmq_min = leaves.min
    r.pf_aq = dev_zeroed(np * qd)
    r.pf_as = dev_zeroed(np * (qd / Q8_BLOCK_ELEMS) * 4l)
    r.pf_hq = dev_zeroed(np * hid)
    r.pf_hs = dev_zeroed(np * (hid / Q8_BLOCK_ELEMS) * 4l)
    if (r.pf_mmq_min) {
        r.pf_xsum = dev_zeroed(np * (dim / Q8_BLOCK_ELEMS) * 4l)
        r.pf_asum = dev_zeroed(np * (qd / Q8_BLOCK_ELEMS) * 4l)
        r.pf_hsum = dev_zeroed(np * (hid / Q8_BLOCK_ELEMS) * 4l)
    }
    let pick = empty(leaves.planes) ? "the f16-fed cm2 tiles" : "the int8 MMQ kernel over Q8_0 feeds for the {leaves.planes} planes, the f16-fed cm2 tiles for the rest"
    to_log(LOG_INFO, "dasLLAMA cuda tier: the window chain's GEMMs on {pick} (DASLLAMA_CUDA_MMQ={g_env_vulkan.cuda_mmq})\n")
    r.pf_splitk_bytes = 262_144l * int64(g_cud.sm_count)
    r.pf_splitk = dev_zeroed(r.pf_splitk_bytes)
    let dmax = max(max(qd, 2l * kvd), max(dim, hid))
    r.pf_sched_words = 4l + max(cm2_tiles_at(dmax, np, 32l), 8l * cm2_tiles_at(dmax, np, 128l))
    for (l in range64(r.n_layers)) {
        var L & = unsafe(r.layers[l])
        for (i in range(SCH_COUNT)) {
            L.sch[i] = cud_sched_make(r.pf_sched_words)
        }
    }
}

//! under `rot` the rows land rotated, and the un-rotation runs before the convert or the requant
def private crd_window_attn(r : CudRdec; L : CudLayer; kmir, vmir : uint64; wsegs : array<RdecPfSeg>; wlen : int64; shadow, rot : bool) {
    var s = g_cud.stream
    let qd = L.qd
    let kvd = uint(L.kvd)
    let hs = L.hs
    let ring = shadow ? 0u : uint(L.ring)
    let kv16 = shadow || r.kv_dt == KVDtype.f16
    let o_mmq = crd_site_mmq(r, L.fo, qd, r.dim, L.bo)
    let f16out = !o_mmq && !rot   // the flash tile's f16 twin lands the cm2 feed itself
    let fa = kv16 && (hs == 64l || hs == 96l || hs == 128l)
    let convert = !o_mmq && !(fa && f16out)
    for (sg in wsegs) {
        let mir_base = shadow ? 0u : uint(crd_region_base(r, L, sg.region))
        if (fa) {
            var pcf = FaArgs(rows = uint(sg.npos), w0 = uint(sg.pos0), qd = uint(qd), kvd = kvd, kv_mul = uint(L.kv_mul),
                kvbase = mir_base, scale = r.scale, qstride = uint(qd), qhs = uint(hs), window = uint(L.window), cap = L.attn_cap,
                sinkoff = 0u, row0 = uint(sg.row0), ring = ring)
            let fwgs = fa_stamp_wgs(false, r.n_heads, sg.npos)
            var fbufs = fixed_array(r.pf_q, kmir, vmir, f16out ? r.pf_af : r.pf_attn)
            if (hs == 64l && !f16out) {
                enc_fa_cm2_h64_cls(s, fbufs, pcf, fwgs)
            } elif (hs == 64l) {
                enc_fa_cm2_h64_f16_cls(s, fbufs, pcf, fwgs)
            } elif (hs == 96l && !f16out) {
                enc_fa_cm2_h96_cls(s, fbufs, pcf, fwgs)
            } elif (hs == 96l) {
                enc_fa_cm2_h96_f16_cls(s, fbufs, pcf, fwgs)
            } elif (!f16out) {
                enc_fa_cm2_h128_cls(s, fbufs, pcf, fwgs)
            } else {
                enc_fa_cm2_h128_f16_cls(s, fbufs, pcf, fwgs)
            }
        } else {
            var pca = DaAttnBArgs(npos = uint(sg.npos), nh = uint(r.n_heads), kvd = kvd, hs = uint(hs), kv_mul = uint(L.kv_mul),
                kbase = mir_base, vbase = mir_base, w0 = uint(sg.pos0), scale = r.scale, qhs = uint(hs), gated = 0u,
                window = uint(L.window), cap = L.attn_cap, row0 = uint(sg.row0), ring = ring)
            var abufs = fixed_array(r.pf_q, kmir, vmir, r.pf_attn)
            let awgs = at_attn_tile_wgs(r.n_heads, sg.npos)
            if (kv16) {
                enc_da_attn_b_f16_cls(s, abufs, pca, awgs)
            } else {
                enc_da_attn_b_cls(s, abufs, pca, awgs)
            }
        }
    }
    if (rot) {
        crd_tq4_unrot(r, s, r.pf_attn, r.pf_q, hs, wlen, qd, qd)
    }
    if (convert) {
        var pcc = gk_f16_rows_args(uint(wlen * qd))
        enc_tower_clamp_cvt_cls(s, fixed_array(r.pf_attn, r.pf_af), pcc, rq_wgs(false, wlen * qd))
    }
    if (o_mmq) {
        var pcq = RqArgs(inbase = 0u, nblk = uint(wlen * qd / kq_unit(false)), lo = 0.0, hi = 0.0)
        if (r.pf_mmq_min) {
            enc_cls_dn_rq_sum(s, fixed_array(r.pf_attn, r.pf_aq, r.pf_as, r.pf_asum), pcq, rq_wgs(false, wlen * qd))
        } else {
            enc_cls_dn_rq(s, fixed_array(r.pf_attn, r.pf_aq, r.pf_as), pcq, rq_wgs(false, wlen * qd))
        }
    }
}

//! a block codec holds one segment: stage from row 0, the store quantizes, the shadow pass dequantizes what the tiles read
def private crd_window_kv(r : CudRdec; l : int64; L : CudLayer; kmir, vmir : uint64; wsegs : array<RdecPfSeg>; wlen, voff, kstride : int64) {
    var s = g_cud.stream
    let qd = L.qd
    let kvd = L.kvd
    let hs = uint(L.hs)
    let kvq = kv_block_codec(r.kv_dt)
    verify(!kvq || long_length(wsegs) == 1l, "cud prefill: a block codec's window holds one segment")
    if (r.qk_norm) {
        //! the fused pass's window form over each segment: row r at position pos0 + r of its region (a block codec stages from row 0),
        //! its k row at the projection row's start and its v row `voff` past it, the q rows `qd` apart
        let nkh = L.kv_shared ? 0l : kvd / L.hs
        var qbufs = fixed_array(r.pf_q, r.pf_kv, kvq ? r.pf_kst : kmir, kvq ? r.pf_vst : vmir, r.pf_cos, r.norms, r.tok, r.dummy, r.pf_kv)
        for (sg in wsegs) {
            var pcf = crd_qkn_rope_args(r, l, L, kvq ? 0l : crd_region_base(r, L, sg.region), nkh, kstride, qd, kvq ? 0l : L.ring)
            pcf.win = 1u
            pcf.pos0 = uint(kvq ? sg.row0 : sg.pos0)
            pcf.row0 = uint(sg.row0)
            pcf.kbase = 0u
            pcf.vbase = uint(voff)
            if (r.kv_dt == KVDtype.f16) {
                enc_qkn_rope_f16_cls(s, qbufs, pcf, (r.n_heads + nkh) * sg.npos)
            } else {
                enc_qkn_rope_cls(s, qbufs, pcf, (r.n_heads + nkh) * sg.npos)
            }
        }
        return if (!kvq)
        crd_window_kv_codec(r, L, kmir, vmir, wsegs, wlen)
        return
    }
    let pairs = qd / 2l + kvd / 2l
    var rbufs = fixed_array(r.pf_q, r.pf_kv, kvq ? r.pf_kst : kmir, kvq ? r.pf_vst : vmir, r.pf_cos, r.dummy)
    for (sg in wsegs) {
        var pcr = RopeKvBArgs(qd = uint(qd), kvd = uint(kvd), hs = hs, half = uint(L.rot / 2l), neox = r.neox ? 1u : 0u,
            kpair0 = uint(qd / 2l), ppp = uint(pairs), layerbase = kvq ? 0u : uint(crd_region_base(r, L, sg.region)), npos = uint(sg.npos),
            pos0 = uint(kvq ? sg.row0 : sg.pos0), voff = uint(voff), kstride = uint(kstride), qhs = hs, boff = 0u, hasb = 0u,
            cstride = uint(r.cos_elems), cbase = uint(L.cbase), row0 = uint(sg.row0), ring = kvq ? 0u : uint(L.ring))
        let rwgs = (sg.npos * pairs + 255l) / 256l
        if (r.kv_dt == KVDtype.f16) {
            enc_rope_kv_b_f16_cls(s, rbufs, pcr, rwgs)
        } else {
            enc_rope_kv_b_cls(s, rbufs, pcr, rwgs)
        }
    }
    return if (!kvq)
    crd_window_kv_codec(r, L, kmir, vmir, wsegs, wlen)
}

//! a block codec's window after the rope staged the floats: the codec's store quantizes them into the mirror, the shadow pass
//! dequantizes the rows the window's attention tiles read
def private crd_window_kv_codec(r : CudRdec; L : CudLayer; kmir, vmir : uint64; wsegs : array<RdecPfSeg>; wlen : int64) {
    var s = g_cud.stream
    let qd = uint(L.qd)
    let kvd = L.kvd
    let hs = uint(L.hs)
    let pos0w = wsegs[0].pos0
    let mir_base = uint(crd_region_base(r, L, wsegs[0].region))
    let tq4 = r.kv_dt == KVDtype.tq4
    if (tq4) {
        let nkh = kvd / L.hs
        var pct = KvTqStoreArgs(kvd = uint(kvd), hs = hs, layerbase = mir_base, pos0 = uint(pos0w), nqh = uint(r.n_heads), nkh = uint(nkh),
            qrow = qd, qstride = hs, inv_sqrt_hs = 1.0 / sqrt(float(hs)), ring = uint(L.ring))
        enc_kv_tq4_store_b_cls(s, fixed_array(r.pf_q, r.pf_kst, r.pf_vst, kmir, vmir, r.tq4_signs), pct, wlen * (r.n_heads + 2l * nkh))
    } else {
        var pcs = KvQStoreArgs(kvd = uint(kvd), rows = uint(wlen), layerbase = mir_base, pos0 = uint(pos0w), ring = uint(L.ring))
        enc_kv_q8_store_b_cls(s, fixed_array(r.pf_kst, r.pf_vst, kmir, vmir), pcs, kv_store_wgs(wlen * kvd))
    }
    let kvlen = pos0w + wlen
    let shadow_pos0 = kv_shadow_pos0(L.window, pos0w)
    var pch = KvQShadowArgs(kvd = uint(kvd), pos0 = uint(shadow_pos0), rows = uint(kvlen - shadow_pos0), layerbase = mir_base, ring = uint(L.ring))
    let shwgs = (kv_row_units(g_crd.kv_dt, kvd) * (kvlen - shadow_pos0) + 255l) / 256l
    if (tq4) {
        enc_kv_tq4_shadow_cls(s, fixed_array(kmir, vmir, r.ksh, r.vsh), pch, shwgs)
    } else {
        enc_kv_q8_shadow_cls(s, fixed_array(kmir, vmir, r.ksh, r.vsh), pch, shwgs)
    }
}

//! a segment ending in the window (`wsegs` its parts, `segs` the call's) takes the classifier at its last row
def private crd_window(var r : CudRdec; w0, wlen : int64; segs, wsegs : array<RdecPfSeg>; last : bool) {
    var s = g_cud.stream
    let dim = r.dim
    let nlfin = uint(rdec_norms_final_base(r.n_layers, dim))
    var pc_pro = GkAddRmsArgs(dim = uint(dim), add_on = 0u, woff = 0u, eps = r.eps, ascale = 1.0)
    crd_x_norm(r, s, r.pf_x, pc_pro, wlen, crd_x_feeds(r, r.layers[0], true))
    for (l in range64(r.n_layers)) {
        var L & = unsafe(r.layers[l])
        let qd = L.qd
        let kvd = L.kvd
        let hid = L.hid
        let kmir = r.k_slabs[L.mir_slab]
        let vmir = r.v_slabs[L.mir_slab]
        crd_site(r, L.sch[SCH_Q], L.fq, L.bq, r.pf_xf, r.pf_xq, r.pf_xs, r.pf_xsum, r.pf_q, dim, qd, wlen)
        let kv_merged = L.fk == L.fv && L.bv == L.bk + kq_blocks_for(dim, kvd, L.fk)
        var voff = kvd
        var kstride = 2l * kvd
        if (kv_merged) {
            crd_site(r, L.sch[SCH_K], L.fk, L.bk, r.pf_xf, r.pf_xq, r.pf_xs, r.pf_xsum, r.pf_kv, dim, 2l * kvd, wlen)
        } else {
            crd_site(r, L.sch[SCH_K], L.fk, L.bk, r.pf_xf, r.pf_xq, r.pf_xs, r.pf_xsum, r.pf_kv, dim, kvd, wlen)
            crd_site(r, L.sch[SCH_V], L.fv, L.bv, r.pf_xf, r.pf_xq, r.pf_xs, r.pf_xsum, r.pf_v, dim, kvd, wlen)
            cuda_memcpy_dtod(r.pf_kv + uint64(wlen * kvd * 4l), r.pf_v, uint64(wlen * kvd * 4l), s)
            voff = wlen * kvd
            kstride = kvd
        }
        let kvq = kv_block_codec(r.kv_dt)
        crd_window_kv(r, l, L, kmir, vmir, wsegs, wlen, voff, kstride)
        crd_window_attn(r, L, kvq ? r.ksh : kmir, kvq ? r.vsh : vmir, wsegs, wlen, kvq, r.kv_dt == KVDtype.tq4)
        crd_site(r, L.sch[SCH_O], L.fo, L.bo, r.pf_af, r.pf_aq, r.pf_as, r.pf_asum, r.pf_xb2, qd, dim, wlen)
        var pc8 = GkAddRmsArgs(dim = uint(dim), add_on = 1u, woff = uint(norm_off(l, RDEC_NORM_FFN)), eps = r.eps, ascale = 1.0,
            pre_on = L.pre_post ? 1u : 0u, pwoff = uint(norm_off(l, RDEC_NORM_POST_ATT)))
        crd_x_norm(r, s, r.pf_xb2, pc8, wlen, crd_x_feeds(r, L, false))
        crd_site(r, L.sch[SCH_G], L.f1, L.b1, r.pf_xf, r.pf_xq, r.pf_xs, r.pf_xsum, r.pf_gate, dim, hid, wlen)
        crd_site(r, L.sch[SCH_U], L.f3, L.b3, r.pf_xf, r.pf_xq, r.pf_xs, r.pf_xsum, r.pf_up, dim, hid, wlen)
        var pc12 = ActArgs(nelem = uint(wlen * hid), act = L.gelu ? ACT_GELU : ACT_SILU, nblk = uint(wlen * hid / kq_unit(false)), elem0 = 0u)
        if (!crd_site_mmq(r, L.f2, hid, dim, L.b2)) {
            enc_actf16_cls(s, fixed_array(r.pf_gate, r.pf_up, r.pf_hf), pc12, rq_wgs(false, wlen * hid))
        } elif (r.pf_mmq_min) {
            enc_q8_actrq_sum_cls(s, fixed_array(r.pf_gate, r.pf_up, r.pf_hq, r.pf_hs, r.pf_hsum), pc12, rq_wgs(false, wlen * hid))
        } else {
            enc_q8_actrq_cls(s, fixed_array(r.pf_gate, r.pf_up, r.pf_hq, r.pf_hs), pc12, rq_wgs(false, wlen * hid))
        }
        crd_site(r, L.sch[SCH_D], L.f2, L.b2, r.pf_hf, r.pf_hq, r.pf_hs, r.pf_hsum, r.pf_ffnout, hid, dim, wlen)
        let last_layer = l + 1l == r.n_layers
        var pc14 = GkAddRmsArgs(dim = uint(dim), add_on = 1u, woff = last_layer ? nlfin : uint(norm_off(l + 1l, RDEC_NORM_ATT)), eps = r.eps,
            ascale = L.out_scale, pre_on = L.pre_post ? 1u : 0u, pwoff = uint(norm_off(l, RDEC_NORM_POST_FFN)))
        if (!last_layer) {
            crd_x_norm(r, s, r.pf_ffnout, pc14, wlen, crd_x_feeds(r, r.layers[l + 1l], true))
        } else {
            for (ws in wsegs) {
                let lr = ws.row0 + ws.npos - 1l
                continue if (rdec_seg_ending_at(segs, w0, lr) < 0l)
                var cbufs = fixed_array(r.pf_x + uint64(lr * dim * 4l), r.pf_ffnout + uint64(lr * dim * 4l), r.norms, r.pf_xq + uint64(lr * dim), r.pf_xs + uint64(lr * (dim / Q8_BLOCK_ELEMS) * 4l))
                if (kq_sb(r.cls_fmt)) {
                    enc_cls_ar_rqk(s, cbufs, pc14, 1l)
                } else {
                    enc_cls_ar_rq_b(s, cbufs, pc14, 1l)
                }
            }
        }
    }
    let ends = crd_window_cls(r, w0, segs, wsegs)
    verify(!last || ends > 0l, "cud prefill: the last window ends no segment")
}

//! the classifier over each ending segment's last-row quants and its epilogue, the logits down into the segment's slot of the
//! out block; the segments ended
def private crd_window_cls(r : CudRdec; w0 : int64; segs, wsegs : array<RdecPfSeg>) : int64 {
    var s = g_cud.stream
    let dim = r.dim
    var ends = 0l
    for (ws in wsegs) {
        let lr = ws.row0 + ws.npos - 1l
        let slot = rdec_seg_ending_at(segs, w0, lr)
        continue if (slot < 0l)
        ends++
        let lg = r.logits + uint64(slot * r.vocab * 4l)
        crd_gemv(s, r.cls_fmt, r.cls_regs, r.pf_xq + uint64(lr * dim), r.pf_xs + uint64(lr * (dim / Q8_BLOCK_ELEMS) * 4l), lg, dim, r.vocab, 0u)
        if (r.cls_cap > 0.0 || r.nsupp > 0l) {
            var pce = GkClsEpiArgs(vocab = uint(r.vocab), cap = r.cls_cap, nsupp = uint(r.nsupp), rows = 1u)
            enc_cls_epi(s, fixed_array(lg, r.nsupp > 0l ? r.supp : r.dummy), pce, cls_epi_wgs(r.vocab, 1l))
        }
        crd_land(s, r, lg, int64(CRD_OUT_LOGITS_WORDS) + slot * r.vocab, r.vocab)
    }
    return ends
}

var private @scratch g_crd_wsegs : array<RdecPfSeg>   //! the parts of the call's segments inside the window being run

//! the call's segments as the window chain: windows of the engine's prompt-window rows over the segments' rows back to back,
//! `feed(w0, wlen)` putting a window's rows into pf_x, its rope rows up, the chain, and each segment's logits off its slot of the
//! out block
def private crd_prefill_windows(var r : CudRdec; cos_batch : array<float>; segs : array<RdecPfSeg>; var logits : array<float>; feed : block<(w0 : int64; wlen : int64) : void>) {
    crd_pf_setup(r)
    if (empty(segs) || long_length(segs) > CRD_NB) {
        panic("cud prefill: {long_length(segs)} segments - the out block holds {CRD_NB} logits slots")
    }
    var npos = 0l
    for (sg in segs) {
        npos += sg.npos
    }
    var w0 = 0l
    while (w0 < npos) {
        let wlen = min(r.pf_np, npos - w0)
        let last = w0 + wlen >= npos
        rdec_window_segs(segs, w0, wlen, g_crd_wsegs)
        if (long_length(g_crd_wsegs) > 1l) {
            r.seg_windows++
        }
        invoke(feed, w0, wlen)
        cuda_memcpy_htod(r.pf_cos, unsafe(addr<void?>(cos_batch[w0 * r.cos_elems])), uint64(wlen * r.cos_elems * 4l), g_cud.stream)
        crd_window(r, w0, wlen, segs, g_crd_wsegs, last)
        if (!cud_sync()) {
            panic("dasLLAMA cuda resident: the window chain failed on the device: {cuda_last_error()}")
        }
        r.windows++
        r.tokens_served += wlen
        w0 += wlen
    }
    for (i in range64(long_length(segs))) {
        crd_logits_out(r, logits, i, i * r.vocab)
    }
}

//! the segments over the host's rows
def private crd_prefill_x(var r : CudRdec; x_batch, cos_batch : array<float>; segs : array<RdecPfSeg>; var logits : array<float>) {
    crd_prefill_windows(r, cos_batch, segs, logits) $(w0, wlen) {
        cuda_memcpy_htod(r.pf_x, unsafe(addr<void?>(x_batch[w0 * r.dim])), uint64(wlen * r.dim * 4l), g_cud.stream)
    }
}

//! the segments over token ids: every window's rows gathered off the placed table on the device
def private crd_prefill_ids(var r : CudRdec; tokens : array<int64>; emb_scale : float; cos_batch : array<float>; segs : array<RdecPfSeg>; var logits : array<float>) {
    assert(r.emb_blk >= 0l, "cud prefill over ids without a placed token table")
    var words <- [for (tk in tokens); uint(tk)]
    let ids = buf_upload(words, g_cud.stream)
    let ae = g_cud_arenas[r.emb_fmt]
    crd_prefill_windows(r, cos_batch, segs, logits) $(w0, wlen) {
        cud_emb_enc(r.emb_fmt, g_cud.stream, ae.wq, ae.ws, ids + uint64(w0 * 4l), r.pf_x, r.pf_x, wlen, r.dim, r.emb_blk, emb_scale)
    }
    cuda_mem_free(ids)
}

//! the one-session call as one segment at row 0 in the selected region
def private crd_one_seg(r : CudRdec; npos, pos0 : int64) : array<RdecPfSeg> => <- [RdecPfSeg(row0 = 0l, npos = npos, pos0 = pos0, region = r.region)]

//! the prefill over the host's rows
[unused_argument(ple_rows, ple_raw, experts)]
def cud_rdec_prefill(x_batch : array<float>; cos_batch : array<float>; ple_rows : array<float>; ple_raw : bool; npos : int64; pos0 : int64; var logits : array<float>; experts : RdecHostExpertsRowsFn) {
    assert(g_crd != null && g_crd.ready, "cud_rdec_prefill before prepare/set_cls")
    var r & = unsafe(*g_crd)
    var inscope segs <- crd_one_seg(r, npos, pos0)
    crd_prefill_x(r, x_batch, cos_batch, segs, logits)
}

//! the prefill over token ids: every window's rows gathered off the placed table on the device
[unused_argument(experts)]
def cud_rdec_prefill_ids(tokens : array<int64>; emb_scale : float; cos_batch : array<float>; npos, pos0 : int64; var logits : array<float>; experts : RdecHostExpertsRowsFn) {
    assert(g_crd != null && g_crd.ready, "cud_rdec_prefill_ids before prepare/set_cls")
    var r & = unsafe(*g_crd)
    var inscope segs <- crd_one_seg(r, npos, pos0)
    crd_prefill_ids(r, tokens, emb_scale, cos_batch, segs, logits)
}

//! the segmented window (the Vulkan home's form): every segment's rows or ids back to back, each segment's K/V into its own
//! region from its own position, its logits at its slot; a block-codec mirror holds one session a window, so the engine's
//! admission keeps a codec load off this form
[unused_argument(experts), arch(at="../ARCHITECTURE_GPU_VULKAN_WINDOW.md#vk-prefill-segments")]
def cud_rdec_prefill_segs(x_batch : array<float>; tokens : array<int64>; use_ids : bool; emb_scale : float; cos_batch : array<float>; segs : array<RdecPfSeg>; var logits : array<float>; experts : RdecHostExpertsRowsFn) {
    assert(g_crd != null && g_crd.ready && !empty(segs), "cud_rdec_prefill_segs before prepare/set_cls, or no segment")
    var r & = unsafe(*g_crd)
    verify(!kv_block_codec(r.kv_dt), "cud_rdec_prefill_segs: the window holds one session under a block-codec mirror")
    if (use_ids) {
        crd_prefill_ids(r, tokens, emb_scale, cos_batch, segs, logits)
    } else {
        crd_prefill_x(r, x_batch, cos_batch, segs, logits)
    }
}

//! the windows that held several sessions' chunks as segments in this process (a cell's witness)
def cud_rdec_prefill_segment_windows : int64 => g_crd != null ? g_crd.seg_windows : 0l

//! a checkpoint capture is a recurrent model's stop, and the dense slice admits no recurrent model: an empty list asks none
def cud_rdec_prefill_caps(caps : array<RdecPfCap>) {
    verify(empty(caps), "dasLLAMA cuda tier: a prefill's checkpoint captures asked of the dense slice, which homes no recurrent model")
}

//! layer `l`'s mirror rows [row0, row0 + nrows) up from the host cache (from row 0); the slice keeps flat slabs, so the rows sit end to end
def cud_rdec_kv_sync(l, row0, nrows : int64; kp : uint8 const?; vp : uint8 const?) {
    assert(g_crd != null, "cud_rdec_kv_sync before prepare")
    return if (nrows <= 0l)
    let L & = unsafe(g_crd.layers[l])
    verify(L.ring == 0l, "cud decode: a ringed layer's rows - the dense slice keeps flat slabs")
    let rowb = kv_row_units(g_crd.kv_dt, L.kvd) * g_crd.unit_bytes
    let base = uint64((crd_mir_base(*g_crd, L) + row0 * kv_row_units(g_crd.kv_dt, L.kvd)) * g_crd.unit_bytes)
    unsafe {
        cuda_memcpy_htod(g_crd.k_slabs[L.mir_slab] + base, reinterpret<void?>(kp + row0 * rowb), uint64(nrows * rowb), g_cud.stream)
        cuda_memcpy_htod(g_crd.v_slabs[L.mir_slab] + base, reinterpret<void?>(vp + row0 * rowb), uint64(nrows * rowb), g_cud.stream)
    }
}

//! ... and back into the host cache
def cud_rdec_kv_read(l, row0, nrows : int64; var kp : uint8?; var vp : uint8?) {
    assert(g_crd != null, "cud_rdec_kv_read before prepare")
    return if (nrows <= 0l)
    let L & = unsafe(g_crd.layers[l])
    verify(L.ring == 0l, "cud decode: a ringed layer's rows - the dense slice keeps flat slabs")
    let rowb = kv_row_units(g_crd.kv_dt, L.kvd) * g_crd.unit_bytes
    let base = uint64((crd_mir_base(*g_crd, L) + row0 * kv_row_units(g_crd.kv_dt, L.kvd)) * g_crd.unit_bytes)
    unsafe {   //! on g_cud.stream, behind the store that wrote the rows
        cuda_memcpy_dtoh(reinterpret<void?>(kp + row0 * rowb), g_crd.k_slabs[L.mir_slab] + base, uint64(nrows * rowb), g_cud.stream)
        cuda_memcpy_dtoh(reinterpret<void?>(vp + row0 * rowb), g_crd.v_slabs[L.mir_slab] + base, uint64(nrows * rowb), g_cud.stream)
    }
}

//! the driver's counters; `unsplit_form` says the device splits the keys at all, so the unsplit form is a form of its own
def cud_rdec_stats : tuple<tokens : int64; graph : bool; windows : int64; unsplit : int64; unsplit_form : bool> =>
    (tokens = g_crd != null ? g_crd.tokens_served : 0l, graph = g_crd != null && crd_graph_captured(*g_crd), windows = g_crd != null ? g_crd.windows : 0l,
        unsplit = g_crd != null ? g_crd.unsplit_steps : 0l, unsplit_form = g_crd != null && g_crd.unsplit_on)
