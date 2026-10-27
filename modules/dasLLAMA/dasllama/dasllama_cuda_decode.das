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

// The CUDA whole-model driver: the resident seats over the CUDA home's kernels - the token step as one captured graph
// replayed a token, the prefill as the window chain, a recurrent layer's head and a routed layer's block on the discrete
// templates. The CUDA row of `ARCHITECTURE_GPU.md#gpu-backends` carries the shape; what the driver declines is listed there.


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
let SCH_DNQKV = 7
let SCH_DNZ = 8
let SCH_DNOUT = 9
let SCH_DNBETA = 10
let SCH_DNALPHA = 11
let SCH_COUNT = 12

//! one recurrent layer's device state slot (a slot a mirror region): the state and the conv ring pair in the layer's two
//! buffers, the ring image the row reads named by `parity`; `valid` where the slot holds a session's state, `dirty` where
//! the device advanced past the host copy, the owner's host addresses the flush writes back to
struct CudDnSlot {
    state_dev, smalls_dev : uint64
    slot_state_bytes, ring_image_bytes, smalls_bytes, state_byte_off : int64
    hist_float_off : int64   //! the parity image `ring_image_bytes` past it
    parity : int
    valid, dirty : bool
    owner_state_addr, owner_hist_addr : uint64
}

struct CudLayer {
    hs, qd, kvd, hid, rot, kv_mul : int64
    window, ring, mir_base, mir_slab, cbase : int64
    boff : int64          //! the layer's row base in the [q | k | v] bias plane (`has_bias`)
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
    regs_gu : uint64      //! the gate and up records as one two-region buffer: the token step's one GEMV over both planes (`gu_merged`)
    gu_merged : bool      //! gate and up share a format, so one dispatch of two regions serves both at one row - the pair of calls past it
    wo_ar, gate_up_fused, down_ar : bool   //! the Q8_0 fused sites the layer takes: wo and down with the residual epilogue, gate and up with the act
    sch : CudSched[SCH_COUNT]   //! the window chain's GEMM schedules, one a role
    recurrent : bool      //! a deltanet layer: the dn planes and slots below serve its head, the attention fields stay unused
    bqkv, bz, bout, bbeta, balpha : int64
    fqkv, fz, fout, fbeta, falpha : int
    ba_off : int64        //! the f32 arm: this layer's [beta ; alpha] row base in the dn_ba rows (elements)
    regs_dnqkv, regs_dnz, regs_dnout, regs_dnbeta, regs_dnalpha : uint64
    dnqkvz_merged : bool    //! the qkv and z planes share a format and `cd` is whole `di` pieces: the token step's one GEMV over both lands [qkv | z]
    nreg_dnqkvz : int64
    regs_dnqkvz : uint64    //! ... its region records: the qkv plane in di-row pieces, then the z plane
    dn : array<CudDnSlot>   //! a state slot a mirror region
    moe : bool              //! a routed layer: its FFN tail is the routed block over the expert stacks below and the driver's routing planes; the dense triple is its shared expert (b1 < 0: none)
    router_slot : int64     //! the layer's row in the router, router-bias, expert-bias and down-scale planes
    be1, be3, be2 : int64   //! the expert stacks' arena blocks (expert 0's; `nfe x dim / unit` blocks an expert)
    fe1, fe3, fe2 : int
    xe_own : bool           //! the experts read a feed of their own activation class, requantized off the stored normed row (the shared expert's feed is the other class)
    moe_mmq : bool          //! the window's expert pieces on the FP4 expert chain (every stack mx4, the MMQ knob on, a card at the rung): the gathered rows as the MXFP4 feed, every bucket on padded 128-row tiles
}

struct CudRdec {
    ready : bool
    n_layers, dim, qd, kv_dim, n_heads, hidden, vocab : int64
    neox : bool
    qk_norm : bool       //! the per-head q/k norm rides the rope pass (`CuQknRopeKv`), its weight rows in the norms plane past the final norm
    q_gated : bool       //! the q GEMV lands [q | gate] a head in a plane of its own (`q`), the rope and the attention read it head-strided
    head_size : int64    //! the widest head class: the q/k norm rows' stride in the norms plane
    hybrid : bool        //! some layer is recurrent: the deltanet geometry, planes and parity words below serve
    dn_cd, dn_kd, dn_di, dn_nvh, dn_nkh, dn_ds, dn_dconv : int64
    dn_ba_f32, dn_zsig : bool
    q : uint64                           //! the gated query plane: [q | gate] rows, 2 qd a row
    dn_proj, dn_oq, dn_os, dn_of : uint64   //! the recurrent head's projection row ([qkv | z]), the step's Q8_0 o row (or its f32 row under a superblock out plane)
    dn_ba : uint64                       //! the f32 arm: every recurrent layer's [beta ; alpha] rows, the file's f32
    dn_parity : array<int>               //! a region's current ring image
    pf_dn_cur : array<int>               //! ... inside a window chain, the image the region's parts read
    pf_dnqkv, pf_dnconv, pf_dnz, pf_dnbg, pf_dnws, pf_dnof, pf_dnoq, pf_dnos, pf_dnosum : uint64   //! the window's recurrent planes: the projected rows, the conv rows, the z rows, the plane arm's beta/alpha rows, the scan's o rows, the o feed (f16, or Q8_0 with its sums)
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
    x, xq, xs, proj, attn, attn_part, ar_cnt, aq, as_, hq, hs_, xb2, norms, cos, logits, pick_part_max, pick_part_id, tok, dummy, supp, cls_regs : uint64
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
    pf_fp4 : bool                         //! an nv4 site takes the FP4 tensor rung: its feeds below are allocated
    pf_knob : int64 = CUDA_MMQ_UNPINNED   //! the DASLLAMA_CUDA_MMQ answer the feeds were sized under: every later pick reads it, not the live knob
    pf_x4q, pf_x4s, pf_xrs : uint64       //! the FP4 feed of the x sites: nibble words, code words, row scales
    pf_a4q, pf_a4s, pf_ars : uint64       //! ... of the wo site
    pf_h4q, pf_h4s, pf_hrs : uint64       //! ... of the down site
    pf_dno4q, pf_dno4s, pf_dnors : uint64   //! ... of a recurrent layer's out site, over the scan's o rows
    pf_kst, pf_vst : uint64   //! a block-codec mirror's staged K and V rows of the window, the codec's window store's source
    pf_mmq_min : bool        //! some window site takes an MMQ leaf with a min term (Q4_K, Q5_1): the requant kernels write the block sums beside the scales
    pf_splitk : uint64       //! the split-k partial planes (the Vulkan home's scratch bound: 256 KB an SM)
    pf_splitk_bytes : int64
    pf_sched_words : int64   //! a schedule buffer's words: the record and the widest role's map at the deepest split
    //! the routed block (`cud_rdec_prepare_moe`): the geometry, the routing planes, the token step's slot buffers and the window's
    moe : bool
    ne, k, nfe, nsh, n_moe, ner : int64   //! experts, picks a row, expert width, the shared expert's width, routed layers, router rows a layer (the gated shared expert's gate row last)
    sh_gated, norm_topk, is_gelu, act_oai, router_bias, exps_bias : bool
    wscale : float
    router, moe_rb, moe_gb, moe_ub, moe_db, moe_ds : uint64   //! [n_moe x ner x dim] f32 rows; the router bias [n_moe x ne]; the expert gate / up [n_moe x ne x nfe] and down [n_moe x ne x dim] bias rows (a biasless model binds a zero down row); the gemma-4 down scales
    moe_ident : uint64       //! the ramp 0 .. RDEC_ROUTED_SLOTS: the token step's slot map (slot s's down row is row s)
    moe_meta1, moe_meta3, moe_meta2 : uint64   //! the picks' GEMV region records a slot: (wblock, xblock) pairs the expert GEMVs read
    moe_w, moe_eidx, moe_dlog : uint64         //! the routing weights and experts a slot; the logits rows [nb x ner]
    moe_xq, moe_xs : uint64                    //! the experts' own feed where their class differs from the shared expert's
    egate, eup, ehq, ehs, edown : uint64        //! the k slot rows: the gate and up GEMVs' outputs, the act's requant, the down GEMV's rows
    pf_xb : uint64           //! the window's f32 normed rows: the router GEMM's and the gather's input, the combine's normed output
    pf_nk : int64            //! the window's slot rows (np x k)
    pf_mlog, pf_midx, pf_mw, pf_minv, pf_bex : uint64   //! the window's logits [np x ner]; a slot row's expert, weight and bucket row; a bucket row's expert
    pf_msch1, pf_msch3, pf_msch2 : uint64      //! the expert tiles' schedules: records then maps (BATCH_META_BYTES each)
    pf_mxg, pf_mhf, pf_my1, pf_my2, pf_mdn : uint64   //! the gathered rows (f16, bucket order), the hidden rows (f16), the gate and up tiles' outputs, the down tiles' rows
    pf_mmq_rows : int64      //! the FP4 expert chain's bucket rows with their pads (`CUD_MOE_ROW_PAD` a bucket at most); 0 = its planes not made
    pf_mxf : uint64                                    //! ... the gathered rows as floats
    pf_mx4q, pf_mx4s, pf_mxrs : uint64                 //! ... their MXFP4 feed: nibble words, code words, row scales
    pf_mh4q, pf_mh4s, pf_mhrs : uint64                 //! ... the hidden rows' MXFP4 feed
    pf_mmy1, pf_mmy2, pf_mmdn : uint64                 //! ... the gate, up and down MMQ tiles' rows
    bias : uint64            //! the [q | k | v] projection bias rows, a layer's at its `boff` (`cud_rdec_upload_bias`); the rope passes add them
    has_bias, has_obias : bool   //! the q/k/v biases ride the rope passes; the attention output bias rides the norms plane's `RDEC_NORM_OBIAS` row, added at the wo residual step
    sinks : uint64           //! the per-head attention sink logits [n_layers x n_heads] (`cud_rdec_upload_sinks`); the sink attention stamps read a layer's row
    has_sinks : bool
}

var g_crd : CudRdec?

def private crd_free_buffers(var r : CudRdec) {
    for (b in [r.x, r.xq, r.xs, r.proj, r.attn, r.attn_part, r.ar_cnt, r.aq, r.as_, r.hq, r.hs_, r.xb2, r.norms, r.cos, r.logits, r.pick_part_max, r.pick_part_id, r.tok, r.dummy, r.supp, r.cls_regs,
                r.gate, r.up, r.ffnout, r.xb, r.kst, r.vst, r.stage_tok, r.ksh, r.vsh, r.tq4_signs]) {
        cuda_mem_free(b)
    }
    for (b in [r.pf_x, r.pf_xf, r.pf_q, r.pf_kv, r.pf_v, r.pf_attn, r.pf_af, r.pf_xb2, r.pf_gate, r.pf_up, r.pf_hf, r.pf_ffnout, r.pf_cos, r.pf_xq, r.pf_xs, r.pf_splitk,
                r.pf_aq, r.pf_as, r.pf_hq, r.pf_hs, r.pf_xsum, r.pf_asum, r.pf_hsum, r.pf_kst, r.pf_vst,
                r.pf_x4q, r.pf_x4s, r.pf_xrs, r.pf_a4q, r.pf_a4s, r.pf_ars, r.pf_h4q, r.pf_h4s, r.pf_hrs, r.pf_dno4q, r.pf_dno4s, r.pf_dnors,
                r.q, r.dn_proj, r.dn_oq, r.dn_os, r.dn_of, r.dn_ba,
                r.pf_dnqkv, r.pf_dnconv, r.pf_dnz, r.pf_dnbg, r.pf_dnws, r.pf_dnof, r.pf_dnoq, r.pf_dnos, r.pf_dnosum,
                r.router, r.moe_rb, r.moe_gb, r.moe_ub, r.moe_db, r.moe_ds, r.moe_ident, r.moe_meta1, r.moe_meta3, r.moe_meta2, r.moe_w, r.moe_eidx, r.moe_dlog,
                r.moe_xq, r.moe_xs, r.egate, r.eup, r.ehq, r.ehs, r.edown,
                r.pf_xb, r.pf_mlog, r.pf_midx, r.pf_mw, r.pf_minv, r.pf_bex, r.pf_msch1, r.pf_msch3, r.pf_msch2, r.pf_mxg, r.pf_mhf, r.pf_my1, r.pf_my2, r.pf_mdn, r.bias, r.sinks,
                r.pf_mxf, r.pf_mx4q, r.pf_mx4s, r.pf_mxrs, r.pf_mh4q, r.pf_mh4s, r.pf_mhrs, r.pf_mmy1, r.pf_mmy2, r.pf_mmdn]) {
        cuda_mem_free(b)
    }
    for (b in r.k_slabs) {
        cuda_mem_free(b)
    }
    for (b in r.v_slabs) {
        cuda_mem_free(b)
    }
    for (L in r.layers) {
        for (b in [L.regs_qkv, L.regs_k, L.regs_v, L.regs_o, L.regs_gate, L.regs_up, L.regs_gu, L.regs_down,
                   L.regs_dnqkv, L.regs_dnz, L.regs_dnqkvz, L.regs_dnout, L.regs_dnbeta, L.regs_dnalpha]) {
            cuda_mem_free(b)
        }
        for (sc in L.sch) {
            cuda_mem_free(sc.buf)
        }
        if (!empty(L.dn)) {
            cuda_mem_free(L.dn[0].state_dev)
            cuda_mem_free(L.dn[0].smalls_dev)
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

[arch(at="../ARCHITECTURE_GPU.md#gpu-backends")]
def private dev_zeroed(bytes : int64) : uint64 {
    let d = cuda_mem_alloc(uint64(bytes))
    if (d != 0ul) {
        cuda_memset_d8(d, 0u8, uint64(bytes), g_cud.stream)
    }
    return d
}

def private cud_serves_fmt(fmt : int) : bool => expert_chain_fmt_ok(kq_fmt_of_slot(fmt))

def cud_rdec_reserve(fmt : int; cap_blocks : int64) : bool {
    return false if (!cud_init())
    if (!cud_serves_fmt(fmt)) {
        return cud_decline("a plane in format {kq_fmt_of_slot(fmt)}: the driver serves Q8_0, the superblock K-quant and NVFP4 planes, the dense per-32 ones and MXFP4 expert stacks")
    }
    return cud_arena_reserve(fmt, cap_blocks)
}

def cud_rdec_place(wq : array<uint8>; ws : array<uint8>; n, rows : int64; fmt : int) : int64 {
    return -1l if (!cud_init() || empty(wq) || !cud_serves_fmt(fmt) || !kq_rows_ok(n, fmt))
    return cud_arena_place(wq, ws, n, rows, fmt)
}

[unused_argument(rot), arch(at="../ARCHITECTURE_GPU_SHARED_KERNELS.md#dn-discrete-kernels")]   //! the seat's signature; the layers carry their own rotation width
def cud_rdec_prepare(n_layers, n_attn, dim, qd, kv_dim, head_size, n_heads, hidden, vocab, seq_cap : int64;
                     neox : bool; eps, scale : float; qk_norm : bool; kv_dtype : int; q_gated : bool; rot : int64; dn : RdecDnGeom;
                     mir_units, n_slabs, cos_elems, regions : int64; carry, head : bool; verify_rows : int64) : bool {
    return false if (!cud_init())
    let dt = kv_dtype_of(kv_dtype)
    let hybrid = dn.cd > 0l
    if (hybrid && !dn_geom_fits(dn.ds, dn.nvh, dn.nkh, dn.kd, dn.di, dn.cd, dn.dconv)) {
        return cud_decline("a deltanet geometry past the kernels' capacities (ds {dn.ds}, {dn.nvh} value heads, conv dim {dn.cd}, di {dn.di}, {dn.dconv} taps)")
    }
    if (!hybrid && n_attn != n_layers) return cud_decline("{n_layers - n_attn} layers without attention on a model with no recurrent layer")
    if (head) return cud_decline("a NextN draft head")
    if (carry) return cud_decline("the post-norm carry")
    if (regions < 1l) return cud_decline("{regions} mirror regions")
    if (dim > RDEC_AR_MAX_DIM || head_size > 512l || kv_dim > RDEC_AR_MAX_DIM) return cud_decline("dim {dim}, kv_dim {kv_dim} or head_size {head_size} past the row stashes")
    if (verify_rows > 0l) return cud_decline("verify rows")
    let refused <- cud_ensure_all()   // every kernel of the home loads before the first chain: a launch precedes no ensure, and a capture loads nothing
    if (!empty(refused)) return cud_decline("{length(refused)} kernels did not compile ({refused[0]} first)")
    crd_drop()
    g_crd = new CudRdec(n_layers = n_layers, dim = dim, qd = qd, kv_dim = kv_dim, n_heads = n_heads, hidden = hidden, vocab = vocab,
        neox = neox, qk_norm = qk_norm, q_gated = q_gated, head_size = head_size, eps = eps, scale = scale, kv_dt = dt, regions = regions, cos_elems = cos_elems)
    var r & = unsafe(*g_crd)
    r.unit_bytes = kv_unit_bytes(dt)
    r.mir_units = mir_units + kv_row_units(dt, RDEC_MIR_SLACK)
    r.nb = CRD_NB
    let nb = r.nb
    let wide = max(max(qd, hidden), dim)
    r.x = dev_zeroed(dim * 4l * nb)
    r.xq = dev_zeroed(wide * nb)
    r.xs = dev_zeroed((wide / Q8_BLOCK_ELEMS) * 4l * nb)
    r.proj = dev_zeroed((qd + 2l * kv_dim) * 4l * nb)
    if (q_gated) {
        r.q = dev_zeroed(2l * qd * 4l * nb)
    }
    r.hybrid = hybrid
    if (hybrid) {
        r.dn_cd = dn.cd; r.dn_kd = dn.kd; r.dn_di = dn.di; r.dn_nvh = dn.nvh; r.dn_nkh = dn.nkh; r.dn_ds = dn.ds; r.dn_dconv = dn.dconv
        r.dn_ba_f32 = dn.ba_f32
        r.dn_zsig = dn.zsig
        r.dn_proj = dev_zeroed((dn.cd + dn.di) * 4l * nb)
        r.dn_oq = dev_zeroed(DND_OQ_BYTES * nb)
        r.dn_os = dev_zeroed(DND_OS_BYTES * nb)
        r.dn_of = dev_zeroed(DND_OQ_BYTES * 4l * nb)
        r.dn_parity |> resize(regions)
        r.pf_dn_cur |> resize(regions)
    }
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
    r.gate = dev_zeroed(hidden * 4l * max(nb, 2l))   // the merged gate-up GEMV lands both rows here at one row
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

//! the norms plane and the eps / scale pair past its rows, re-sized here: the prepare cannot know whether the output-bias rows come
def cud_rdec_upload_norms(norms : array<float>; obias : bool) {
    assert(g_crd != null, "cud_rdec_upload_norms before prepare")
    g_crd.has_obias = obias
    if (long_length(norms) != g_crd.ar_scal_off) {
        cuda_mem_free(g_crd.norms)
        g_crd.ar_scal_off = long_length(norms)
        g_crd.norms_bytes = (g_crd.ar_scal_off + 2l) * 4l
        g_crd.norms = dev_zeroed(g_crd.norms_bytes)
    }
    unsafe {
        cuda_memcpy_htod(g_crd.norms, addr<void?>(norms[0]), uint64(long_length(norms) * 4l), g_cud.stream)
        var pair : float[2]
        pair[0] = g_crd.eps
        pair[1] = 1.0
        cuda_memcpy_htod(g_crd.norms + uint64(g_crd.ar_scal_off * 4l), addr<void?>(pair[0]), 8ul, g_cud.stream)
    }
}

//! the [q | k | v] projection bias rows, a layer's at its `boff` (`install_moe_gpu_resident_bias`'s seat), which the rope passes add
def cud_rdec_upload_bias(rows : array<float>) {
    assert(g_crd != null, "cud_rdec_upload_bias before prepare")
    cuda_mem_free(g_crd.bias)
    g_crd.bias = buf_upload(rows, g_cud.stream)
    g_crd.has_bias = true
}

//! the per-head sink logits, a layer's row of `n_heads` (`install_moe_gpu_resident_sinks`'s seat); declines off head size 64, the one flash sink shape
def cud_rdec_upload_sinks(rows : array<float>) : bool {
    assert(g_crd != null, "cud_rdec_upload_sinks before prepare")
    var r & = unsafe(*g_crd)
    if (r.head_size != 64l) {
        return cud_decline("attention sinks at head size {r.head_size}: the sink stamps serve 64")
    }
    var da_ok = false
    if (r.kv_dt == KVDtype.f16) {
        da_ok = ensure_da_attn_f16_sink_cls()
    } elif (r.kv_dt == KVDtype.q8_0) {
        da_ok = ensure_da_attn_q8_sink_cls()
    } elif (r.kv_dt == KVDtype.tq4) {
        da_ok = ensure_da_attn_tq4_sink_cls()
    } else {
        da_ok = ensure_da_attn_sink_cls()
    }
    if (!(da_ok && ensure_fa_cm2_h64_sink_cls() && ensure_fa_cm2_h64_sink_f16_cls())) {
        return cud_decline("an attention-sink kernel class for the {r.kv_dt} K/V cache declined on this device")
    }
    cuda_mem_free(r.sinks)
    r.sinks = buf_upload(rows, g_cud.stream)
    r.has_sinks = true
    return true
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
    L.window = g.window; L.ring = g.ring; L.mir_base = g.mir_base; L.mir_slab = g.mir_slab; L.cbase = g.cbase; L.boff = g.boff
    L.bq = bq; L.bk = bk; L.bv = bv; L.bo = bo; L.b1 = b1; L.b3 = b3; L.b2 = b2
    L.fq = fq; L.fk = fk; L.fv = fv; L.fo = fo; L.f1 = f1; L.f3 = f3; L.f2 = f2
    verify(cud_serves_fmt(fq) && cud_serves_fmt(fk) && cud_serves_fmt(fv) && cud_serves_fmt(fo) && cud_serves_fmt(f1) && cud_serves_fmt(f3) && cud_serves_fmt(f2),
        "cud decode: a plane in a format the driver does not serve reached set_layer - the place seat admits the arena formats alone")
    verify(!L.kv_shared && !L.vfk, "cud decode: a shared-KV or V-from-K layer - the driver projects q, k and v")
    L.wo_ar = fo == int(KqFmt.q8) && !kq_sb(f1) && kq_sb(f1) == kq_sb(f3)   // the fused epilogue requants into the gate's feed alone and keeps no normed row
    L.qkv_merged = !r.q_gated && L.qd % L.kvd == 0l && fq == fk && fk == fv   // one GEMV over the three planes needs them in one arena; a gated q lands in its own plane
    L.regs_o = cud_regions([uint(bo), 0u])
    crd_ffn_regs(L)
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

//! the routed block's admission: an expert stack in any arena format (the cm2 tiles and the GEMV leaves serve them all)
def cud_rdec_moe_ok(f1, f3, f2 : int) : bool => cud_serves_fmt(f1) && cud_serves_fmt(f3) && cud_serves_fmt(f2)

def private cud_ramp(n : int64) : uint64 {
    var words <- [for (i in range64(n)); uint(i)]
    return buf_upload(words, g_cud.stream)
}

def private cud_rows_or_zero(rows : array<float>; zero_elems : int64) : uint64 => empty(rows) ? dev_zeroed(zero_elems * 4l) : buf_upload(rows, g_cud.stream)

//! the routed block's planes and token-step slot buffers, once a residency (the window's land in `crd_pf_setup`)
[arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#resident-moe-token")]
def cud_rdec_prepare_moe(ne, k, nfe, nsh, n_moe : int64; sh_gated, norm_topk, is_gelu, act_oai : bool; wscale : float; router : array<float>; dscale : array<float>;
                         rbias : array<float>; gbias : array<float>; ubias : array<float>; dbias : array<float>) : bool {
    assert(g_crd != null, "cud_rdec_prepare_moe before prepare")
    var r & = unsafe(*g_crd)
    let dim = r.dim
    let ner = ne + (nsh > 0l && sh_gated ? 1l : 0l)
    if (ne > TOPK_EXPERTS_MAX || k > RDEC_ROUTED_SLOTS || nfe % 32l != 0l || dim % 64l != 0l) {
        return cud_decline("the MoE geometry ({ne} experts, {k} routed, expert width {nfe}, dim {dim}) is outside the routing kernels' reach")
    }
    if (k * r.nb > RDEC_ROUTED_SLOTS) {
        r.nb = max(1l, RDEC_ROUTED_SLOTS / k)
        to_log(LOG_INFO, "dasLLAMA cuda resident: the N-row command steps {r.nb} rows at once - {k} picks a row over {RDEC_ROUTED_SLOTS} routed slots\n")
    }
    let nb = r.nb
    if (!empty(dscale)) return cud_decline("the gemma-4 routed form (per-expert down scales)")
    assert(long_length(router) == n_moe * ner * dim, "cud_rdec_prepare_moe: the router plane is not [n_moe x ner x dim]")
    r.moe = true
    r.ne = ne; r.k = k; r.nfe = nfe; r.nsh = nsh; r.n_moe = n_moe; r.ner = ner
    r.sh_gated = sh_gated; r.norm_topk = norm_topk; r.is_gelu = is_gelu; r.act_oai = act_oai
    r.wscale = wscale
    r.router_bias = !empty(rbias)
    r.exps_bias = !empty(gbias)
    r.router = buf_upload(router, g_cud.stream)
    r.moe_rb = cud_rows_or_zero(rbias, ne)
    r.moe_gb = cud_rows_or_zero(gbias, nfe)
    r.moe_ub = cud_rows_or_zero(ubias, nfe)
    r.moe_db = cud_rows_or_zero(dbias, dim)
    r.moe_ds = dev_zeroed(256l)
    r.moe_ident = cud_ramp(RDEC_ROUTED_SLOTS)
    r.moe_meta1 = dev_zeroed(RDEC_ROUTED_SLOTS * 8l)
    r.moe_meta3 = dev_zeroed(RDEC_ROUTED_SLOTS * 8l)
    r.moe_meta2 = dev_zeroed(RDEC_ROUTED_SLOTS * 8l)
    r.moe_w = dev_zeroed(RDEC_ROUTED_SLOTS * 4l)
    r.moe_eidx = dev_zeroed(RDEC_ROUTED_SLOTS * 4l)
    r.moe_dlog = dev_zeroed(ner * 4l * nb)
    r.moe_xq = dev_zeroed(dim * nb)
    r.moe_xs = dev_zeroed((dim / Q8_BLOCK_ELEMS) * 4l * nb)
    r.egate = dev_zeroed(nb * k * nfe * 4l)
    r.eup = dev_zeroed(nb * k * nfe * 4l)
    r.ehq = dev_zeroed(nb * k * nfe)
    r.ehs = dev_zeroed((nb * k * nfe / Q8_BLOCK_ELEMS) * 4l)
    r.edown = dev_zeroed(nb * k * dim * 4l)
    return true
}

//! layer `l`'s expert stacks on a built layer (`install_moe_gpu_resident_moe`'s experts seat): expert 0's arena block a stack, the formats, the router slot
def cud_rdec_set_moe_experts(l, router_slot, be1, be3, be2 : int64; fe1, fe3, fe2 : int) {
    assert(g_crd != null && g_crd.moe, "cud_rdec_set_moe_experts before prepare_moe")
    var r & = unsafe(*g_crd)
    var L & = unsafe(r.layers[l])
    verify(be1 >= 0l && be3 >= 0l && be2 >= 0l, "cud decode: a routed layer's experts off the device - the driver homes every expert stack it serves")
    verify(cud_serves_fmt(fe1) && cud_serves_fmt(fe3) && cud_serves_fmt(fe2), "cud decode: an expert stack in a format the driver does not serve reached set_moe_experts - the moe_ok seat admits the arena formats alone")
    L.moe = true
    L.router_slot = router_slot
    L.be1 = be1; L.be3 = be3; L.be2 = be2
    L.fe1 = fe1; L.fe3 = fe3; L.fe2 = fe2
    L.xe_own = L.b1 >= 0l && kq_sb(fe1) != kq_sb(L.f1)
    L.wo_ar = false   // the wo epilogue's fused requant keeps no normed row, which the router reads
    let knob = r.pf_knob != CUDA_MMQ_UNPINNED ? r.pf_knob : crd_mmq_knob()
    L.moe_mmq = knob != CUDA_MMQ_OFF && g_cud.sm >= FP4_RUNG_SM && fe1 == int(KqFmt.mx4) && fe3 == int(KqFmt.mx4) && fe2 == int(KqFmt.mx4) && crd_moe_mmq_fits(r.dim, r.nfe)
    if (L.moe_mmq && r.pf_mmq_rows == 0l && r.pf_nk > 0l) {   // a window already sized: the chain's planes now; else at the window's setup
        crd_pf_moe_mmq_setup(r)
    }
    crd_graphs_drop(r)
}

//! the FP4 expert leaf serves a routed layer whose two reduction widths (the gate/up's `dim`, the down's `nfe`) sit on the 64 lattice:
//! the `KTAIL` stamp stages a k64 pair a step, and the MXFP4 feed packs two block codes a k64 word; a width at 32 past a multiple of 64
//! (a legal mx4 plane) has no half pair, so the layer keeps the cm2 tiles
def crd_moe_mmq_fits(dim, nfe : int64) : bool => dim % 64l == 0l && nfe % 64l == 0l

//! an attention MoE layer (`install_moe_gpu_resident_moe`'s layer seat); the dense triple is the shared expert's, `b1` < 0 where the layer has none
def cud_rdec_set_moe_layer(l, mir_idx, router_slot, bq, bk, bv, bo, b1, b3, b2, be1, be3, be2 : int64; fq, fk, fv, fo, f1, f3, f2, fe1, fe3, fe2 : int; g : RdecLayerGeom) {
    assert(g_crd != null && g_crd.moe, "cud_rdec_set_moe_layer before prepare_moe")
    verify(!g.moe_g4, "cud decode: the gemma-4 routed form reached set_moe_layer - prepare_moe declines it by its down scales")
    cud_rdec_set_layer(l, mir_idx, bq, bk, bv, bo, b1, b3, b2, fq, fk, fv, fo, f1, f3, f2, g)
    cud_rdec_set_moe_experts(l, router_slot, be1, be3, be2, fe1, fe3, fe2)
}

//! the FFN triple's region records and its gate/up fused forms; the down epilogue's fuse waits on its consumer (`crd_layer_fuses`)
def private crd_ffn_regs(var L : CudLayer) {
    let q8 = int(KqFmt.q8)
    L.gate_up_fused = L.f1 == q8 && L.f3 == q8 && !kq_sb(L.f2)
    L.regs_gate = cud_regions([uint(L.b1), 0u])
    L.regs_up = cud_regions([uint(L.b3), 0u])
    L.gu_merged = L.f1 == L.f3 && !L.gate_up_fused
    L.regs_gu = cud_regions([uint(L.b1), 0u, uint(L.b3), 0u])
    L.regs_down = cud_regions([uint(L.b2), 0u])
}

def private crd_head_feed_fmt(L : CudLayer) : int => L.recurrent ? L.fqkv : L.fq

[arch(at="../ARCHITECTURE_GPU.md#gpu-backends")]
def private crd_storex_next(r : CudRdec; l : int64) : bool => l + 1l < r.n_layers && crd_storex_head(r.layers[l + 1l])

def private crd_storex_head(L : CudLayer) : bool => L.recurrent || crd_head_mixed(L)

[arch(at="../ARCHITECTURE_GPU.md#gpu-backends")]
def private crd_head_mixed(L : CudLayer) : bool => L.recurrent ? kq_sb(L.fz) != kq_sb(L.fqkv) : (kq_sb(L.fk) != kq_sb(L.fq) || kq_sb(L.fv) != kq_sb(L.fq))

[arch(at="../ARCHITECTURE_GPU.md#gpu-backends")]
def private crd_gu_mixed(L : CudLayer) : bool => kq_sb(L.f1) != kq_sb(L.f3)

//! the down epilogue requants into the next layer's feed, so it fuses only where that consumer reads Q8_0 and stores no normed row
def private crd_layer_fuses(var r : CudRdec) {
    for (l in range64(r.n_layers)) {
        var L & = unsafe(r.layers[l])
        let fnext = l + 1l < r.n_layers ? crd_head_feed_fmt(r.layers[l + 1l]) : r.cls_fmt
        L.down_ar = !L.moe && L.f2 == int(KqFmt.q8) && !kq_sb(fnext) && !crd_storex_next(r, l)   // a routed layer's residual step is the combine
    }
}

def cud_rdec_set_cls(cls_block : int64; cls_fmt : int; cap : float; supp : array<int64>) {
    assert(g_crd != null, "cud_rdec_set_cls before prepare")
    var r & = unsafe(*g_crd)
    verify(cud_serves_fmt(cls_fmt), "cud decode: a classifier in a format the driver does not serve reached set_cls")
    r.cls_blk = cls_block
    r.cls_fmt = cls_fmt
    crd_layer_fuses(r)   // every layer's down fuse reads its consumer's feed format, the classifier's known only now
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
    return false if (!cud_serves_fmt(fmt))
    var r & = unsafe(*g_crd)
    r.emb_blk = emb_block
    r.emb_fmt = fmt
    r.emb_scale = escale
    crd_graphs_drop(r)
    return true
}

[unused_argument(fblob, off, vocab, dim, escale)]
def cud_rdec_upload_emb_f32(fblob : array<float>; off, vocab, dim : int64; escale : float) : bool => false   //! no f32 table on the driver: the host embeds those rows

def private crd_dn_smalls_put(smalls : uint64; float_off : int64; src : float const?; n : int64) {
    cuda_memcpy_htod(smalls + uint64(float_off * 4l), unsafe(reinterpret<void?>(src)), uint64(n * 4l), g_cud.stream)
}

//! a recurrent (deltanet) layer: the beta/alpha planes, or under the f32 arm its rows at `ba_off` of `cud_rdec_upload_dn_ba`'s; a session's state arrives through `cud_rdec_dn_own`
[arch(at="../ARCHITECTURE_GPU.md#gpu-backends")]
def cud_rdec_set_dn_layer(l, bqkv, bz, bout, bbeta, balpha, b1, b3, b2, ba_off : int64; fqkv, fz, fout, fbeta, falpha, f1, f3, f2 : int;
                          convw : float const?; wnormp : float const?; ap : float const?; dtp : float const?) {
    assert(g_crd != null && g_crd.hybrid, "cud_rdec_set_dn_layer before a hybrid prepare")
    var r & = unsafe(*g_crd)
    var L & = unsafe(r.layers[l])
    L.recurrent = true
    L.hid = r.hidden   // a recurrent layer's dense triple takes the model's width
    L.out_scale = 1.0
    L.bqkv = bqkv; L.bz = bz; L.bout = bout; L.bbeta = bbeta; L.balpha = balpha
    L.fqkv = fqkv; L.fz = fz; L.fout = fout; L.fbeta = fbeta; L.falpha = falpha
    L.b1 = b1; L.b3 = b3; L.b2 = b2
    L.f1 = f1; L.f3 = f3; L.f2 = f2
    L.ba_off = ba_off
    verify(b1 >= 0l, "cud decode: a recurrent layer with no dense triple - a recurrent routed layer with no shared expert brings no dense triple to set_dn_layer")
    verify(cud_serves_fmt(fqkv) && cud_serves_fmt(fz) && cud_serves_fmt(fout) && cud_serves_fmt(f1) && cud_serves_fmt(f3) && cud_serves_fmt(f2)
        && (r.dn_ba_f32 || (cud_serves_fmt(fbeta) && cud_serves_fmt(falpha))),
        "cud decode: a plane in a format the driver does not serve reached set_dn_layer - the place seat admits the arena formats alone")
    L.regs_dnqkv = cud_regions([uint(bqkv), 0u])
    L.regs_dnz = cud_regions([uint(bz), 0u])
    L.dnqkvz_merged = fqkv == fz && r.dn_cd % r.dn_di == 0l   // one GEMV over the qkv plane in di-row pieces and the z plane lands [qkv | z] whole
    if (L.dnqkvz_merged) {
        let piece_blocks = uint(kq_blocks_for(r.dim, r.dn_di, fqkv))
        let npieces = r.dn_cd / r.dn_di
        var pairs : array<uint>
        pairs |> reserve(2l * npieces + 2l)
        for (p in range64(npieces)) {
            pairs |> push(uint(bqkv) + uint(p) * piece_blocks)
            pairs |> push(0u)
        }
        pairs |> push(uint(bz))
        pairs |> push(0u)
        L.nreg_dnqkvz = long_length(pairs) / 2l
        L.regs_dnqkvz = cud_regions(pairs)
    }
    L.regs_dnout = cud_regions([uint(bout), 0u])
    if (!r.dn_ba_f32) {
        L.regs_dnbeta = cud_regions([uint(bbeta), 0u])
        L.regs_dnalpha = cud_regions([uint(balpha), 0u])
    }
    crd_ffn_regs(L)
    let nslot = r.regions
    let state_bytes = r.dn_nvh * r.dn_ds * r.dn_ds * 4l
    let ring_elems = r.dn_cd * (r.dn_dconv - 1l)
    let smalls_bytes = (DN_SM_HIST + nslot * 2l * ring_elems + DN_SM_SLACK) * 4l
    let state_dev = dev_zeroed(state_bytes * nslot)
    let smalls_dev = dev_zeroed(smalls_bytes)
    if (state_dev == 0ul || smalls_dev == 0ul) {
        panic("cud decode: a recurrent layer's state slots found no room on the device ({cuda_last_error()})")
    }
    L.dn |> reserve(nslot)
    L.dn |> resize(nslot)
    for (s in range64(nslot)) {
        L.dn[s] = CudDnSlot(state_dev = state_dev, smalls_dev = smalls_dev, slot_state_bytes = state_bytes, ring_image_bytes = ring_elems * 4l,
                            smalls_bytes = smalls_bytes, state_byte_off = s * state_bytes, hist_float_off = DN_SM_HIST + s * 2l * ring_elems,
                            parity = 0, valid = false, dirty = false, owner_state_addr = 0ul, owner_hist_addr = 0ul)
    }
    crd_dn_smalls_put(smalls_dev, DN_SM_TAPS, convw, r.dn_cd * r.dn_dconv)
    crd_dn_smalls_put(smalls_dev, DN_SM_WNORM, wnormp, r.dn_ds)
    crd_dn_smalls_put(smalls_dev, DN_SM_A, ap, r.dn_nvh)
    crd_dn_smalls_put(smalls_dev, DN_SM_DT, dtp, r.dn_nvh)
    crd_graphs_drop(r)
}

//! every recurrent layer's f32 rows in layer order, a layer's `[beta (nvh x dim) ; alpha (nvh x dim)]` at its `ba_off`
def cud_rdec_upload_dn_ba(rows : array<float>) {
    assert(g_crd != null && g_crd.hybrid, "cud_rdec_upload_dn_ba before a hybrid prepare")
    cuda_mem_free(g_crd.dn_ba)
    g_crd.dn_ba = buf_upload(rows, g_cud.stream)
}

def private crd_dn_flush_down(st : CudDnSlot) {
    unsafe {
        cuda_memcpy_dtoh(reinterpret<void?>(st.owner_state_addr), st.state_dev + uint64(st.state_byte_off), uint64(st.slot_state_bytes), g_cud.stream)
        cuda_memcpy_dtoh(reinterpret<void?>(st.owner_hist_addr), st.smalls_dev + uint64(st.hist_float_off * 4l + int64(st.parity) * st.ring_image_bytes),
            uint64(st.ring_image_bytes), g_cud.stream)
    }
    verify(cud_sync(), "cud decode: a deltanet state flush did not land - the caller reads the host state as soon as this returns")
}

def private crd_dn_drop_slot(var st : CudDnSlot) {
    st.valid = false
    st.dirty = false
    st.owner_state_addr = 0ul
    st.owner_hist_addr = 0ul
}

def private crd_dn_send_home(var st : CudDnSlot) {
    if (st.dirty) {
        crd_dn_flush_down(st)
    }
    crd_dn_drop_slot(st)
}

def private crd_dn_state_upload(var st : CudDnSlot; stp : float const?; histp : float const?; cur_par : int) {
    unsafe {
        cuda_memcpy_htod(st.state_dev + uint64(st.state_byte_off), reinterpret<void?>(stp), uint64(st.slot_state_bytes), g_cud.stream)
        cuda_memcpy_htod(st.smalls_dev + uint64(st.hist_float_off * 4l + int64(cur_par) * st.ring_image_bytes), reinterpret<void?>(histp),
            uint64(st.ring_image_bytes), g_cud.stream)
    }
    st.parity = cur_par
    st.owner_state_addr = intptr(stp)
    st.owner_hist_addr = intptr(histp)
    st.valid = true
    st.dirty = false
}

//! bind layer `l`'s slot in the selected region to the calling session's state (`stp`) and conv history (`histp`) before a token
[arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#dn-step-owner")]
def cud_rdec_dn_own(l : int64; stp : float?; histp : float?) {
    var L & = unsafe(g_crd.layers[l])
    let slot = g_crd.region
    var st & = unsafe(L.dn[slot])
    let caller = intptr(stp)
    if (st.valid && st.owner_state_addr == caller) {
        return
    }
    if (st.valid && st.dirty) {
        crd_dn_flush_down(st)
    }
    crd_dn_state_upload(st, stp, histp, g_crd.dn_parity[slot])
}

//! After a resident prefill, hand recurrent layer `l`'s device slot to the session `stp` / `histp` name (valid, dirty, bound)
[arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#hybrid-token-command")]
def cud_rdec_prefill_dn_own(l : int64; stp : float?; histp : float?) {
    var L & = unsafe(g_crd.layers[l])
    var st & = unsafe(L.dn[g_crd.region])
    st.parity = g_crd.dn_parity[g_crd.region]
    st.owner_state_addr = intptr(stp)
    st.owner_hist_addr = intptr(histp)
    st.valid = true
    st.dirty = true
}

//! Sync layer `l`'s device state back into the calling session's buffers where the device holds the newer copy
[unused_argument(histp)]
def cud_dn_step_flush(l : int64; var stp : float?; var histp : float?) {
    return if (g_crd == null || !g_crd.hybrid || l >= long_length(g_crd.layers) || !g_crd.layers[l].recurrent)
    var L & = unsafe(g_crd.layers[l])
    for (st in L.dn) {
        if (st.valid && st.owner_state_addr == intptr(stp)) {
            crd_dn_send_home(st)
        }
    }
}

//! the slots `drop` names dropped, unwritten
def private crd_dn_drop_slots(drop : block<(st : CudDnSlot) : bool>) {
    return if (g_crd == null || !g_crd.hybrid)
    for (L in g_crd.layers) {
        continue if (!L.recurrent)
        for (st in L.dn) {
            if (invoke(drop, st)) {
                crd_dn_drop_slot(st)
            }
        }
    }
}

//! every device copy dropped: the engine rebuilt or zeroed its host copies
[cold_path, arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#dn-step-owner")]
def cud_dn_step_invalidate {
    crd_dn_drop_slots() $(_st : CudDnSlot) : bool {
        return true
    }
}

//! the copies one session's state owns dropped, unwritten: the session reset that state, or it is dying
[hot_path, arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#dn-step-owner")]
def cud_dn_step_release(state_lo : uint64; state_hi : uint64) {
    crd_dn_drop_slots() $(st : CudDnSlot) : bool {
        return st.owner_state_addr >= state_lo && st.owner_state_addr < state_hi
    }
}

def private crd_dn_advance(var r : CudRdec; region : int64) {
    r.dn_parity[region] = 1 - r.dn_parity[region]
    for (L in r.layers) {
        if (L.recurrent) {
            L.dn[region].parity = r.dn_parity[region]
            L.dn[region].dirty = true
        }
    }
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_WINDOW.md#vk-prefill-dn-block")]
def private crd_dn_begin(var r : CudRdec; segs : array<RdecPfSeg>) {
    for (sg in segs) {
        if (sg.pos0 == 0l) {
            for (L in r.layers) {
                continue if (!L.recurrent || !L.dn[sg.region].valid)
                crd_dn_send_home(L.dn[sg.region])
            }
            r.dn_parity[sg.region] = 0
        }
        r.pf_dn_cur[sg.region] = r.dn_parity[sg.region]
    }
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_WINDOW.md#vk-prefill-dn-block")]
def private crd_dn_zero_fresh(r : CudRdec; wsegs : array<RdecPfSeg>) {
    for (ws in wsegs) {
        continue if (ws.pos0 != 0l)
        for (L in r.layers) {
            continue if (!L.recurrent)
            let st & = unsafe(L.dn[ws.region])
            cuda_memset_d8(st.state_dev + uint64(st.state_byte_off), 0u8, uint64(st.slot_state_bytes), g_cud.stream)
            cuda_memset_d8(st.smalls_dev + uint64(st.hist_float_off * 4l), 0u8, uint64(st.ring_image_bytes), g_cud.stream)
        }
    }
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_WINDOW.md#vk-prefill-dn-block")]
def private crd_dn_window_done(var r : CudRdec; wsegs : array<RdecPfSeg>) {
    for (ws in wsegs) {
        r.pf_dn_cur[ws.region] = 1 - r.pf_dn_cur[ws.region]
    }
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_WINDOW.md#vk-prefill-dn-block")]
def private crd_dn_end(var r : CudRdec; segs : array<RdecPfSeg>) {
    for (sg in segs) {
        r.dn_parity[sg.region] = r.pf_dn_cur[sg.region]
    }
}

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
def private crd_ar_rq(r : CudRdec; var s : void?; fmt : int; add : uint64; var pc : GkAddRmsArgs; rows : int64; storex : bool = false) {
    var bufs = fixed_array(r.x, add, r.norms, r.xq, r.xs)
    if (storex && (!kq_sb(fmt) || rows == 1l)) {   // the normed row stored as floats beside the quants: a recurrent head's beta/alpha rows read it
        if (kq_sb(fmt)) {
            enc_cls_ar_rqkx(s, fixed_array(r.x, add, r.norms, r.xq, r.xs, r.xb), pc, rows)
        } else {
            enc_cls_ar_rqx(s, fixed_array(r.x, add, r.norms, r.xq, r.xs, r.xb), pc, rows)
        }
    } elif (!kq_sb(fmt)) {
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

//! the attention stamps' gate flag: off over a tq4 mirror, where the un-rotation gates (the Vulkan home's `rd_attn_gated`)
[arch(at = "../ARCHITECTURE_GPU_SHARED_ATTN.md#vk-kv-tq4-basis")]
def private crd_attn_gated(r : CudRdec) : bool => r.q_gated && r.kv_dt != KVDtype.tq4

//! the bias plane the rope passes bind: the rows under `has_bias`, else the placeholder the kernels never read (`hasb` 0)
def private crd_bias_plane(r : CudRdec) : uint64 => r.has_bias ? r.bias : r.dummy

def private crd_hasb(r : CudRdec) : uint => r.has_bias ? 1u : 0u

def private crd_wo_ar_args(r : CudRdec; l : int64; L : CudLayer) : GkAddRmsArgs =>
    GkAddRmsArgs(dim = uint(r.dim), add_on = 1u, woff = uint(norm_off(l, RDEC_NORM_FFN)), eps = r.eps, ascale = 1.0,
        pre_on = L.pre_post ? 1u : 0u, pwoff = uint(norm_off(l, RDEC_NORM_POST_ATT)),
        abias_on = r.has_obias ? 1u : 0u, aboff = uint(norm_off(l, RDEC_NORM_OBIAS)))

def private crd_topk_args(r : CudRdec; L : CudLayer) : GkTopKArgs {
    let dim = uint(r.dim)
    let nfe = uint(r.nfe)
    return GkTopKArgs(ne = uint(r.ne), k = uint(r.k), norm = r.norm_topk ? 1u : 0u, wscale = r.wscale, dsoff = GK_NO_DSCALE,
        rbias_on = r.router_bias ? 1u : 0u, rboff = uint(L.router_slot * r.ne), lstride = uint(r.ner),
        base1 = uint(L.be1), stride1 = nfe * dim / uint(kq_unit(L.fe1)),
        base3 = uint(L.be3), stride3 = nfe * dim / uint(kq_unit(L.fe3)),
        base2 = uint(L.be2), stride2 = nfe * dim / uint(kq_unit(L.fe2)),
        xnb2 = nfe / uint(kq_unit(L.fe2)), xnb1 = dim / uint(kq_unit(L.fe1)))
}

def private crd_moe_act(r : CudRdec; var s : void?; L : CudLayer; gate, up, outq, outs : uint64; slot_elems : int64) {
    let dkq = kq_sb(L.fe2)
    var pca = ActArgs(nelem = uint(slot_elems), act = act_code(r.act_oai, r.is_gelu), nblk = uint(slot_elems / kq_unit(dkq)),
        bias_on = r.exps_bias ? 1u : 0u, gboff = uint(L.router_slot * r.ne * r.nfe), uboff = uint(L.router_slot * r.ne * r.nfe), brow = uint(r.nfe))
    if (r.exps_bias) {   // the picks' gate and up bias rows join before the activation (`ActRqBT`), the slots' experts the top-k wrote
        if (dkq) {
            enc_q8k_actrq_b_cls(s, fixed_array(gate, up, outq, outs, r.moe_gb, r.moe_ub, r.moe_eidx), pca, rq_wgs(true, slot_elems))
        } else {
            enc_q8_actrq_b_cls(s, fixed_array(gate, up, outq, outs, r.moe_gb, r.moe_ub, r.moe_eidx), pca, rq_wgs(false, slot_elems))
        }
    } elif (dkq) {
        enc_q8k_actrq_cls(s, fixed_array(gate, up, outq, outs), pca, rq_wgs(true, slot_elems))
    } else {
        enc_q8_actrq_cls(s, fixed_array(gate, up, outq, outs), pca, rq_wgs(false, slot_elems))
    }
}

def private crd_comb_args(r : CudRdec; l : int64; L : CudLayer; nxt : uint) : GkAddRmsArgs =>
    GkAddRmsArgs(dim = uint(r.dim), add_on = L.b1 >= 0l ? 1u : 0u, woff = nxt, eps = r.eps, ascale = L.out_scale,
        slots = uint(r.k), lstride = uint(r.ner), gated = r.sh_gated && L.b1 >= 0l ? 1u : 0u,
        pre_on = L.pre_post ? 1u : 0u, pwoff = uint(norm_off(l, RDEC_NORM_POST_FFN)),
        dbias_on = r.exps_bias ? 1u : 0u, dboff = uint(L.router_slot * r.ne * r.dim))

[arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#resident-moe-token")]
def private crd_moe_tail(r : CudRdec; var s : void?; l : int64; L : CudLayer; nlfin : uint; nrows : int64) {
    let dim = r.dim
    let nfe = r.nfe
    let k = r.k
    let nr = uint(nrows)
    let nxt = l + 1l < r.n_layers ? uint(norm_off(l + 1l, RDEC_NORM_ATT)) : nlfin
    let fnext = l + 1l < r.n_layers ? crd_head_feed_fmt(r.layers[l + 1l]) : r.cls_fmt
    if (L.b1 >= 0l) {
        crd_ffn_hidden(r, s, L, nrows)
        crd_gemv(s, L.f2, L.regs_down, r.hq, r.hs_, r.ffnout, L.hid, dim, 0u, 1l, nr, uint(dim))
    }
    var xq = r.xq
    var xs = r.xs
    if (L.xe_own) {
        crd_rq_rows(s, L.fe1, r.xb, r.moe_xq, r.moe_xs, dim * nrows)
        xq = r.moe_xq
        xs = r.moe_xs
    }
    var pcr = GkRouterArgs(ne = uint(r.ner), dim = uint(dim), woff = uint(L.router_slot * r.ner * dim), obase = 0u, ncols = nr)
    each_col_tile(pcr, nr, GEMV_COL_TILE) $ {
        enc_router_gemv_cls(s, fixed_array(r.router, r.xb, r.moe_dlog, r.norms), pcr, r.ner)
    }
    var pct = crd_topk_args(r, L)
    cud_topk_n_enc(r.ne, s, fixed_array(r.moe_dlog, r.moe_meta1, r.moe_meta3, r.moe_meta2, r.moe_w, r.moe_ds, r.moe_rb, r.moe_eidx), pct, nrows)
    crd_gemv(s, L.fe1, r.moe_meta1, xq, xs, r.egate, dim, nfe, 0u, k * nrows)
    crd_gemv(s, L.fe3, r.moe_meta3, xq, xs, r.eup, dim, nfe, 0u, k * nrows)
    crd_moe_act(r, s, L, r.egate, r.eup, r.ehq, r.ehs, k * nfe * nrows)
    crd_gemv(s, L.fe2, r.moe_meta2, r.ehq, r.ehs, r.edown, nfe, dim, 0u, k * nrows)
    var pc14 = crd_comb_args(r, l, L, nxt)
    enc_cls_ar_comb(s, fixed_array(r.x, r.ffnout, r.norms, r.xb, r.moe_ident, r.moe_w, r.edown, r.moe_dlog, r.moe_db, r.moe_eidx), pc14, nrows)
    crd_rq_rows(s, fnext, r.xb, r.xq, r.xs, dim * nrows)
}

//! a layer's dense gate, up and act over `nrows` rows into the hidden feed (hq, hs): the FFN's front, and the shared expert's
def private crd_ffn_hidden(r : CudRdec; var s : void?; L : CudLayer; nrows : int64) {
    let dim = r.dim
    let act = L.gelu ? ACT_GELU : ACT_SILU
    let nr = uint(nrows)
    if (L.gate_up_fused) {
        let a1 = g_cud_arenas[L.f1]
        var pcgu = GuNArgs(n = uint(dim), d = uint(L.hid), gblk = uint(L.b1), ublk = uint(L.b3), act = act, ncols = nr)
        cud_gu_n_enc(s, fixed_array(a1.wq, a1.ws, a1.wq, a1.ws, r.xq, r.xs, r.hq, r.hs_), pcgu, L.hid / 32l)
    } else {
        let merged = L.gu_merged && nrows == 1l
        var up = r.up
        if (merged) {
            crd_gemv(s, L.f1, L.regs_gu, r.xq, r.xs, r.gate, dim, L.hid, 0u, 2l)
            up = r.gate
        } else {
            crd_gemv(s, L.f1, L.regs_gate, r.xq, r.xs, r.gate, dim, L.hid, 0u, 1l, nr, uint(L.hid))
            if (crd_gu_mixed(L)) {
                crd_rq_rows(s, L.f3, r.xb, r.xq, r.xs, dim * nrows)
            }
            crd_gemv(s, L.f3, L.regs_up, r.xq, r.xs, r.up, dim, L.hid, 0u, 1l, nr, uint(L.hid))
        }
        let kq2 = kq_sb(L.f2)
        var pca2 = ActArgs(nelem = uint(L.hid * nrows), act = act, nblk = uint(L.hid * nrows / kq_unit(kq2)), elem0 = 0u, ubase = merged ? uint(L.hid) : 0u)
        if (kq2) {
            enc_q8k_actrq_cls(s, fixed_array(r.gate, up, r.hq, r.hs_), pca2, rq_wgs(true, L.hid * nrows))
        } else {
            enc_q8_actrq_cls(s, fixed_array(r.gate, up, r.hq, r.hs_), pca2, rq_wgs(false, L.hid * nrows))
        }
    }
}

def private crd_ffn_feed_fmt(L : CudLayer) : int => L.moe && L.b1 < 0l ? L.fe1 : L.f1

[arch(at="../ARCHITECTURE_GPU.md#gpu-backends")]
def private crd_ffn_storex(L : CudLayer) : bool => L.moe || crd_gu_mixed(L)

//! the down epilogue fuses at one row alone: a last-workgroup epilogue would serialize the rows
[arch(at="../ARCHITECTURE_GPU.md#gpu-backends")]
def private crd_ffn(r : CudRdec; var s : void?; l : int64; L : CudLayer; nlfin : uint; nrows : int64) {
    let dim = r.dim
    let nr = uint(nrows)
    crd_ffn_hidden(r, s, L, nrows)
    let nxt = l + 1l < r.n_layers ? uint(norm_off(l + 1l, RDEC_NORM_ATT)) : nlfin
    let fnext = l + 1l < r.n_layers ? crd_head_feed_fmt(r.layers[l + 1l]) : r.cls_fmt
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
        crd_ar_rq(r, s, fnext, r.ffnout, pc14, nrows, crd_storex_next(r, l))
    }
}

def private crd_q_plane(r : CudRdec) : uint64 => r.q_gated ? r.q : r.proj

//! ungated, q sits at the front of the [q | k | v] projection row
def private crd_tok_q_row_stride(r : CudRdec; L : CudLayer) : int64 => r.q_gated ? 2l * L.qd : L.qd + 2l * L.kvd

//! `pf_q` is a plane of its own on either model
def private crd_pf_q_row_stride(r : CudRdec; L : CudLayer) : int64 => r.q_gated ? 2l * L.qd : L.qd

def private crd_q_head_stride(r : CudRdec; L : CudLayer) : int64 => r.q_gated ? 2l * L.hs : L.hs

def private crd_qkn_rope_args(r : CudRdec; l : int64; L : CudLayer; layerbase, nkh, kvrow, qrow, ring : int64) : GkQknRopeArgs {
    let qkbase = rdec_norms_qk_base(r.n_layers, r.dim)   // nolint:LINT021 — the norm row offsets multiply 64-bit before the arg-field narrow
    return GkQknRopeArgs(qd = uint(L.qd), kvd = uint(L.kvd), hs = uint(L.hs), half = uint(L.rot / 2l), neox = r.neox ? 1u : 0u,
        nh = uint(r.n_heads), layerbase = uint(layerbase),
        qwoff = uint(qkbase + l * 2l * r.head_size), kwoff = uint(qkbase + (l * 2l + 1l) * r.head_size),
        eps = r.eps, qstride = uint(crd_q_head_stride(r, L)), boff = uint(L.boff), hasb = crd_hasb(r), cbase = uint(L.cbase), vfk = L.vfk ? 1u : 0u, vnorm = 0u,
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
    let qplane = crd_q_plane(r)
    let qrow = crd_tok_q_row_stride(r, L)
    if (r.qk_norm) {
        let nkh = L.kv_shared ? 0l : kvd / hs
        var pcf = crd_qkn_rope_args(r, l, L, kvq ? 0l : L.mir_base, nkh, kvrow, qrow, kvq ? 0l : L.ring)
        var qbufs = fixed_array(qplane, r.proj, kvq ? r.kst : kmir, kvq ? r.vst : vmir, r.cos, r.norms, kvq ? r.stage_tok : r.tok, crd_bias_plane(r), r.proj)
        if (r.kv_dt == KVDtype.f16) {
            enc_qkn_rope_f16_cls(s, qbufs, pcf, (r.n_heads + nkh) * nrows)
        } else {
            enc_qkn_rope_cls(s, qbufs, pcf, (r.n_heads + nkh) * nrows)
        }
    } else {
        let qpairs = qd / 2l
        let npairs = qpairs + kvd / 2l
        var pcr = gk_rope_kv_args(qd, kvd, hs, qpairs, npairs, r.neox, r.has_bias, hs, L.boff, L.cbase, nrows, kvrow, kvrow, r.cos_elems, kvq ? 0l : L.mir_base, L.ring)
        let rwgs = (npairs * nrows + int64(CUD_WG_X) - 1l) / int64(CUD_WG_X)
        var rbufs = fixed_array(r.proj, r.proj, r.proj, kvq ? r.kst : kmir, kvq ? r.vst : vmir, crd_bias_plane(r), r.cos, r.cos, kvq ? r.stage_tok : r.tok)
        if (r.kv_dt == KVDtype.f16) {
            enc_rope_kv_f16_cls(s, rbufs, pcr, rwgs)
        } else {
            enc_rope_kv_cls(s, rbufs, pcr, rwgs)
        }
    }
    if (r.kv_dt == KVDtype.tq4) {
        let nkh = kvd / hs
        var pct = KvTqStoreArgs(kvd = uint(kvd), hs = uint(hs), layerbase = uint(L.mir_base), pos0 = 0u, nqh = uint(r.n_heads), nkh = uint(nkh),
            qrow = uint(qrow), qstride = uint(crd_q_head_stride(r, L)), inv_sqrt_hs = 1.0 / sqrt(float(hs)), ring = uint(L.ring))
        enc_kv_tq4_store_cls(s, fixed_array(qplane, r.kst, r.vst, kmir, vmir, r.tq4_signs, r.tok), pct, nrows * (r.n_heads + 2l * nkh))
    } elif (kvq) {
        var pcs = KvQStoreArgs(kvd = uint(kvd), rows = uint(nrows), layerbase = uint(L.mir_base), pos0 = 0u, ring = uint(L.ring))
        enc_kv_q8_store_cls(s, fixed_array(r.kst, r.vst, kmir, vmir, r.tok), pcs, kv_store_wgs(kvd * nrows))
    }
}

//! a layer's attention over the mirror into the wo feed, `nrows` rows a row a workgroup: each row's quants, or its floats where the
//! fold cannot block them (the fold it took, `DA_RQ_*`, returned); the keys in one piece a group under `unsplit` (the Vulkan
//! home's unsplit twin), else in the device's split, the last-arriving piece combining
def private crd_token_attn(r : CudRdec; var s : void?; l : int64; L : CudLayer; kmir, vmir : uint64; nrows : int64; unsplit : bool) : uint {
    let hs = uint(L.hs)
    let mir_base = uint(L.mir_base)   // the layer's base: each row's token block adds its region
    let nsplit = unsplit ? 1l : r.attn_nsplit
    let rowwgs = da_attn_row_wgs(r.n_heads, L.kv_mul, nsplit)
    let rqk = crd_attn_rqk(L)
    let qrow = crd_tok_q_row_stride(r, L)
    var pca = DaAttnArgs(kvd = uint(L.kvd), hs = hs, kv_mul = uint(L.kv_mul), kbase = mir_base, vbase = mir_base,
        scale = r.scale, qstride = uint(crd_q_head_stride(r, L)), gated = crd_attn_gated(r) ? 1u : 0u, window = uint(L.window), cap = L.attn_cap, nsplit = uint(nsplit),
        rqk = rqk, sinkoff = uint(l * r.n_heads), rowwg = uint(rowwgs), qrow = uint(qrow), cntoff = uint(r.attn_cnt_off), ring = uint(L.ring))
    var abufs = fixed_array(crd_q_plane(r), kmir, vmir, r.attn, r.tok, r.attn_part, r.aq, r.as_)
    let g2 = da_slab_is_g2(L.kv_mul)
    let awgs = rowwgs * nrows
    if (r.has_sinks) {   // the sink stamps (gpt-oss): the head's sink logit in the softmax, one stamp a mirror codec
        var sbufs = fixed_array(abufs[0], abufs[1], abufs[2], abufs[3], abufs[4], abufs[5], abufs[6], abufs[7], r.sinks)
        if (r.kv_dt == KVDtype.f16) {
            enc_da_attn_f16_sink_cls(s, sbufs, pca, awgs)
        } elif (r.kv_dt == KVDtype.tq4) {
            enc_da_attn_tq4_sink_cls(s, sbufs, pca, awgs)
            crd_tq4_unrot(r, s, r.attn, crd_q_plane(r), L.hs, nrows, L.qd, qrow, crd_q_head_stride(r, L))
        } elif (kv_block_codec(r.kv_dt)) {
            enc_da_attn_q8_sink_cls(s, sbufs, pca, awgs)
        } else {
            enc_da_attn_sink_cls(s, sbufs, pca, awgs)
        }
        return rqk
    }
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
        crd_tq4_unrot(r, s, r.attn, crd_q_plane(r), L.hs, nrows, L.qd, qrow, crd_q_head_stride(r, L))
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

//! tq4's un-rotation of `rows` rows of `orow` elements in `y`; a `qstride` past `hs` also gates each head by its gate half in `q` (`qrow` a row)
def private crd_tq4_unrot(r : CudRdec; var s : void?; y, q : uint64; hs, rows, orow, qrow, qstride : int64) {
    var pcu = KvTqUnrotArgs(hs = uint(hs), nh = uint(r.n_heads), orow = uint(orow), qrow = uint(qrow), qstride = uint(qstride), inv_sqrt_hs = 1.0 / sqrt(float(hs)))
    enc_kv_tq4_unrot_cls(s, fixed_array(y, q, r.tq4_signs), pcu, r.n_heads * rows)
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#hybrid-token-command")]
def private crd_dn_head(r : CudRdec; var s : void?; L : CudLayer; nrows : int64) {
    let dim = r.dim
    let cd = r.dn_cd
    let di = r.dn_di
    let nvh = r.dn_nvh
    let nr = uint(nrows)
    let st0 & = unsafe(L.dn[0])
    var feed_fmt = L.fqkv   // a site of the other class requants the stored row first
    if (L.dnqkvz_merged) {
        crd_gemv(s, L.fqkv, L.regs_dnqkvz, r.xq, r.xs, r.dn_proj, dim, di, 0u, L.nreg_dnqkvz, nr, uint(cd + di))
    } else {
        crd_gemv(s, L.fqkv, L.regs_dnqkv, r.xq, r.xs, r.dn_proj, dim, cd, 0u, 1l, nr, uint(cd + di))
        if (kq_sb(L.fz) != kq_sb(feed_fmt)) {
            crd_rq_rows(s, L.fz, r.xb, r.xq, r.xs, dim * nrows)
            feed_fmt = L.fz
        }
        crd_gemv(s, L.fz, L.regs_dnz, r.xq, r.xs, r.dn_proj, dim, di, uint(cd), 1l, nr, uint(cd + di))
    }
    if (r.dn_ba_f32) {
        var pcb = GkRouterArgs(ne = uint(2l * nvh), dim = uint(dim), woff = uint(L.ba_off), obase = uint(DN_SM_BETA),
            ne_split = uint(nvh), obase_split = uint(DN_SM_G), ncols = nr)
        each_col_tile(pcb, nr, GEMV_COL_TILE) $ {
            enc_router_gemv_cls(s, fixed_array(r.dn_ba, r.xb, st0.smalls_dev, r.norms), pcb, 2l * nvh)
        }
    } else {
        if (kq_sb(L.fbeta) != kq_sb(feed_fmt)) {
            crd_rq_rows(s, L.fbeta, r.xb, r.xq, r.xs, dim * nrows)
        }
        crd_gemv(s, L.fbeta, L.regs_dnbeta, r.xq, r.xs, st0.smalls_dev, dim, nvh, uint(DN_SM_BETA), 1l, nr, uint(nvh))
        if (kq_sb(L.falpha) != kq_sb(L.fbeta)) {
            crd_rq_rows(s, L.falpha, r.xb, r.xq, r.xs, dim * nrows)
        }
        crd_gemv(s, L.falpha, L.regs_dnalpha, r.xq, r.xs, st0.smalls_dev, dim, nvh, uint(DN_SM_G), 1l, nr, uint(nvh))
    }
    var pcd = DnStepArgs(cd = uint(cd), kd = uint(r.dn_kd), nkh = uint(r.dn_nkh), ds = uint(r.dn_ds), dconv = uint(r.dn_dconv),
        beta_off = uint(DN_SM_BETA), g_off = uint(DN_SM_G), taps_off = uint(DN_SM_TAPS), wnorm_off = uint(DN_SM_WNORM),
        a_off = uint(DN_SM_A), dt_off = uint(DN_SM_DT), hist_off = uint(DN_SM_HIST), eps = r.eps, qscale = 1.0 / sqrt(float(r.dn_ds)),
        out_f32 = kq_sb(L.fout) ? 1u : 0u, beta_row_stride = uint(nvh), row0 = 0u)
    var step_bufs = fixed_array(r.dn_proj, st0.state_dev, st0.smalls_dev, r.dn_os, r.dn_oq, r.tok, r.dn_of)
    if (r.dn_zsig) {
        enc_dn_step_sig_cls(s, step_bufs, pcd, nvh * nrows)
    } else {
        enc_dn_step_cls(s, step_bufs, pcd, nvh * nrows)
    }
    if (kq_sb(L.fout)) {
        crd_rq_rows(s, L.fout, r.dn_of, r.dn_oq, r.dn_os, di * nrows)
    }
    crd_gemv(s, L.fout, L.regs_dnout, r.dn_oq, r.dn_os, r.xb2, di, dim, 0u, 1l, nr, uint(dim))
}

def private crd_rq_rows(var s : void?; consumer_fmt : int; src, outq, outs : uint64; elems : int64) {
    let kq = kq_sb(consumer_fmt)
    var pcq = RqArgs(inbase = 0u, nblk = uint(elems / kq_unit(kq)), lo = 0.0, hi = 0.0)
    if (kq) {
        enc_cls_q8k_rq(s, fixed_array(src, outq, outs), pcq, rq_wgs(true, elems))
    } else {
        enc_cls_dn_rq(s, fixed_array(src, outq, outs), pcq, rq_wgs(false, elems))
    }
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
    crd_ar_rq(r, s, r.n_layers > 0l ? crd_head_feed_fmt(r.layers[0]) : r.cls_fmt, r.x, pc_pro, nrows, r.n_layers > 0l && crd_storex_head(r.layers[0]))
    let nlfin = uint(rdec_norms_final_base(r.n_layers, dim))
    for (l in range64(r.n_layers)) {
        let L & = unsafe(r.layers[l])
        if (L.recurrent) {
            crd_dn_head(r, s, L, nrows)
            var pcr = GkAddRmsArgs(dim = uint(dim), add_on = 1u, woff = uint(norm_off(l, RDEC_NORM_FFN)), eps = r.eps, ascale = 1.0)
            crd_ar_rq(r, s, crd_ffn_feed_fmt(L), r.xb2, pcr, nrows, crd_ffn_storex(L))
            if (L.moe) {
                crd_moe_tail(r, s, l, L, nlfin, nrows)
            } else {
                crd_ffn(r, s, l, L, nlfin, nrows)
            }
            continue
        }
        let qd = L.qd
        let kvd = L.kvd
        let kvrow = qd + 2l * kvd
        if (L.qkv_merged) {
            crd_gemv(s, L.fq, L.regs_qkv, r.xq, r.xs, r.proj, dim, kvd, 0u, L.nreg_qkv, nr, uint(kvrow))
        } else {
            crd_gemv(s, L.fq, L.regs_qkv, r.xq, r.xs, crd_q_plane(r), dim, r.q_gated ? 2l * qd : qd, 0u, 1l, nr, uint(crd_tok_q_row_stride(r, L)))
            if (kq_sb(L.fk) != kq_sb(L.fq)) {
                crd_rq_rows(s, L.fk, r.xb, r.xq, r.xs, dim * nrows)
            }
            crd_gemv(s, L.fk, L.regs_k, r.xq, r.xs, r.proj, dim, kvd, uint(qd), 1l, nr, uint(kvrow))
            if (kq_sb(L.fv) != kq_sb(L.fk)) {
                crd_rq_rows(s, L.fv, r.xb, r.xq, r.xs, dim * nrows)
            }
            crd_gemv(s, L.fv, L.regs_v, r.xq, r.xs, r.proj, dim, kvd, uint(qd + kvd), 1l, nr, uint(kvrow))
        }
        let kmir = r.k_slabs[L.mir_slab]
        let vmir = r.v_slabs[L.mir_slab]
        crd_token_kv(r, s, l, L, kmir, vmir, kvrow, nrows)
        let rqk = crd_token_attn(r, s, l, L, kmir, vmir, nrows, unsplit)
        if (rqk == DA_RQ_NONE) {   // no fold (a head off the superblock, or tq4): the separate requant in the wo plane's class
            crd_rq_rows(s, L.fo, r.attn, r.aq, r.as_, qd * nrows)
        }
        let ao = g_cud_arenas[L.fo]
        if (L.wo_ar && one) {
            var pcwa = GemvArArgs(n = uint(qd), d = uint(dim), wblk = uint(L.bo), xblk = 0u, add_on = 1u,
                woff = uint(norm_off(l, RDEC_NORM_FFN)), pwoff = uint(norm_off(l, RDEC_NORM_POST_ATT)), pre_on = L.pre_post ? 1u : 0u,
                no_norm = 0u, eps = r.eps, ascale = 1.0, abias_on = r.has_obias ? 1u : 0u, aboff = uint(norm_off(l, RDEC_NORM_OBIAS)))
            enc_q8_gemv_ar_cls(s, fixed_array(ao.wq, ao.ws, r.aq, r.as_, r.xb2, r.x, r.norms, r.xq, r.xs, r.ar_cnt), pcwa, ar_wgs(dim))
        } else {
            crd_gemv(s, L.fo, L.regs_o, r.aq, r.as_, r.xb2, qd, dim, 0u, 1l, nr, uint(dim))
            var pc8 = crd_wo_ar_args(r, l, L)
            crd_ar_rq(r, s, crd_ffn_feed_fmt(L), r.xb2, pc8, nrows, crd_ffn_storex(L))
        }
        if (L.moe) {
            crd_moe_tail(r, s, l, L, nlfin, nrows)
        } else {
            crd_ffn(r, s, l, L, nlfin, nrows)
        }
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
        *tm = TokMeta(pos = uint(pos), cnt = uint(cnt), mirbase = uint(r.region * r.mir_units), parity = crd_tok_parity(r, r.region), dnslot = uint(r.region))
        w[CRD_IN_ID_WORD] = gathered ? uint(token) : 0u
        memcpy(addr<void?>(w[CRD_IN_HDR_WORDS]), addr<void?>(cossin[coff]), r.cos_elems * 4l)
        if (!gathered) {
            memcpy(addr<void?>(w[int64(CRD_IN_HDR_WORDS) + r.cos_elems]), addr<void?>(x[xoff]), r.dim * 4l)
        }
    }
    crd_launch(r, 1l, logits_on, gathered, cnt)
    if (r.hybrid) {
        crd_dn_advance(r, r.region)
    }
    r.tokens_served++
}

def private crd_tok_parity(r : CudRdec; region : int64) : uint => r.hybrid ? uint(r.dn_parity[region]) : 0u

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
            tm[i] = TokMeta(pos = uint(poss[i]), cnt = uint(cnts[i]), mirbase = uint(regs[i] * r.mir_units), parity = crd_tok_parity(r, regs[i]), dnslot = uint(regs[i]))
            w[int64(CRD_IN_ID_WORD) + i] = gathered ? uint(ids[i]) : 0u
        }
        memcpy(addr<void?>(w[CRD_IN_HDR_WORDS]), addr<void?>(cossin[0]), r.cos_elems * 4l * nrows)
        if (!gathered) {
            memcpy(addr<void?>(w[int64(CRD_IN_HDR_WORDS) + r.cos_elems * nrows]), addr<void?>(x[0]), r.dim * 4l * nrows)
        }
    }
    crd_launch(r, nrows, !all_pick, gathered, maxcnt)
    if (r.hybrid) {   // every region the rows stepped moves on once: the engine puts one row a region in a command
        for (i in range64(nrows)) {
            crd_dn_advance(r, regs[i])
        }
    }
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

//! split k only while the unsplit grid, the s tile's too, is short of two blocks an SM
def private cud_cm2_pick(d, rows, n : int64) : tuple<tc : int64; nsplit : int64; ksplit : int64> {
    var tc = cud_cm2_tc(d, rows)
    let cores = int64(g_cud.sm_count)
    let forced = g_env_vulkan.cm2_splitk
    var want = 1l
    if (forced > 0l) {
        want = forced
    } elif (cores > 0l && rows > 128l) {
        let slots = 2l * cores
        if (tc == 32l) {
            let tiles_s = cm2_tiles_at(d, rows, 32l)
            if (tiles_s * 2l <= slots) {
                want = slots / tiles_s
            }
        } else {
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
    }
    let sk = cm2_split_fit(n, want, rows * d * 4l, g_crd.pf_splitk_bytes)
    return sk > 1l ? (tc = tc, nsplit = sk, ksplit = cm2_chunk(n, sk)) : (tc = tc, nsplit = 1l, ksplit = 0l)
}

def private cud_sched_make(words : int64) : CudSched => CudSched(buf = dev_zeroed(words * 4l), wblk = -1l, rows = -1l)

let private MMQ_LEAF_FMTS <- [int(KqFmt.q8), int(KqFmt.k4), int(KqFmt.iq4xs), int(KqFmt.q40), int(KqFmt.iq4nl), int(KqFmt.q51), int(KqFmt.q5032), int(KqFmt.q4032), int(KqFmt.iq4nl32), int(KqFmt.nv4)]
let private MMQ_LEAF_NAMES <- ["Q8_0", "Q4_K", "IQ4_XS", "Q4_0", "IQ4_NL", "Q5_1", "Q5_0", "Q4_0 (per-32)", "IQ4_NL (per-32)", "NVFP4 (FP4 tensor cores)"]
//! the leaves only `DASLLAMA_CUDA_MMQ=2` arms: their windows did not beat the cm2 tiles on every measured card
let private MMQ_RACE_FMTS <- [int(KqFmt.q51), int(KqFmt.q5032)]

def private crd_mmq_min(fmt : int) : bool => fmt == int(KqFmt.k4) || fmt == int(KqFmt.q51)

//! the DASLLAMA_CUDA_MMQ values: the cm2 tiles everywhere, the leaves that won on every measured card (the default), every leaf
let CUDA_MMQ_OFF = 0l
let CUDA_MMQ_WINNERS = 1l
let CUDA_MMQ_ALL = 2l
let CUDA_MMQ_UNPINNED = -1l   //! `set_cuda_mmq_pin` with it: the environment's value

var private g_cuda_mmq_pin = CUDA_MMQ_UNPINNED   //! a test's pin over DASLLAMA_CUDA_MMQ for the loads after it

//! pins the DASLLAMA_CUDA_MMQ value (`CUDA_MMQ_OFF`, `CUDA_MMQ_WINNERS` or `CUDA_MMQ_ALL`) the loads after it read; `CUDA_MMQ_UNPINNED` puts the environment's value back
def public set_cuda_mmq_pin(v : int64) {
    g_cuda_mmq_pin = v
}

//! the pin `set_cuda_mmq_pin` stored, `CUDA_MMQ_UNPINNED` when none
def public cuda_mmq_pin() : int64 => g_cuda_mmq_pin

def private crd_mmq_knob() : int64 => g_cuda_mmq_pin != CUDA_MMQ_UNPINNED ? g_cuda_mmq_pin : g_env_vulkan.cuda_mmq

//! whether a window GEMM site takes its MMQ leaf; a Q8_0 plane starts on a block of 8 (eight scales a quad), a min-term leaf's reduction is a multiple of 512 (it folds sixteen blocks at a time)
def private crd_site_mmq(r : CudRdec; fmt : int; n, d, wblk : int64) : bool {
    let knob = r.pf_knob != CUDA_MMQ_UNPINNED ? r.pf_knob : crd_mmq_knob()
    return false if (knob == CUDA_MMQ_OFF || r.pf_np % 128l != 0l || d % 128l != 0l || n % 256l != 0l || find_index(MMQ_LEAF_FMTS, fmt) < 0
        || (knob == CUDA_MMQ_WINNERS && find_index(MMQ_RACE_FMTS, fmt) >= 0)
        || (fmt == int(KqFmt.nv4) && g_cud.sm < FP4_RUNG_SM))
    return wblk % 8l == 0l if (fmt == int(KqFmt.q8))
    return !crd_mmq_min(fmt) || n % 512l == 0l
}

def private crd_kv_merged(dim : int64; L : CudLayer) : bool =>
    L.fk == L.fv && L.bv == L.bk + kq_blocks_for(dim, L.kvd, L.fk)

//! the MMQ leaves the window sites take: whether one has a min term or is the FP4 leaf (the window's feeds follow), their names for the load log
def private crd_mmq_leaves(r : CudRdec) : tuple<min : bool; fp4 : bool; planes : string> {
    var on : bool[10]
    for (L in r.layers) {
        let kd = crd_kv_merged(r.dim, L) ? 2l * L.kvd : L.kvd
        var inscope sites <- L.recurrent ? [(fmt = L.fqkv, n = r.dim, d = r.dn_cd, b = L.bqkv), (fmt = L.fz, n = r.dim, d = r.dn_di, b = L.bz), (fmt = L.fout, n = r.dn_di, d = r.dim, b = L.bout), (fmt = L.f1, n = r.dim, d = L.hid, b = L.b1), (fmt = L.f3, n = r.dim, d = L.hid, b = L.b3), (fmt = L.f2, n = L.hid, d = r.dim, b = L.b2)] : [(fmt = L.fq, n = r.dim, d = crd_pf_q_row_stride(r, L), b = L.bq), (fmt = L.fk, n = r.dim, d = kd, b = L.bk), (fmt = L.fv, n = r.dim, d = L.kvd, b = L.bv), (fmt = L.fo, n = L.qd, d = r.dim, b = L.bo), (fmt = L.f1, n = r.dim, d = L.hid, b = L.b1), (fmt = L.f3, n = r.dim, d = L.hid, b = L.b3), (fmt = L.f2, n = L.hid, d = r.dim, b = L.b2)]
        for (site in sites) {
            continue if (!crd_site_mmq(r, site.fmt, site.n, site.d, site.b))
            on[find_index(MMQ_LEAF_FMTS, site.fmt)] = true
        }
    }
    var mn = false
    var fp4 = false
    var planes = ""
    for (i in range(10)) {
        continue if (!on[i])
        mn = mn || crd_mmq_min(MMQ_LEAF_FMTS[i])
        fp4 = fp4 || MMQ_LEAF_FMTS[i] == int(KqFmt.nv4)
        planes = empty(planes) ? MMQ_LEAF_NAMES[i] : "{planes}, {MMQ_LEAF_NAMES[i]}"
    }
    return (min = mn, fp4 = fp4, planes = planes)
}

//! the x feeds a layer's head sites read; a recurrent layer under the f32 arm also reads the f16 rows, its beta/alpha tile GEMM's
def private crd_head_feeds(r : CudRdec; L : CudLayer) : tuple<mmq : bool; fp4 : bool; f16 : bool> {
    var mmq = false
    var fp4 = false
    var f16 = false
    let kv_merged = crd_kv_merged(r.dim, L)
    var inscope sites <- !L.recurrent ? [(fmt = L.fq, d = crd_pf_q_row_stride(r, L), b = L.bq), (fmt = L.fk, d = kv_merged ? 2l * L.kvd : L.kvd, b = L.bk), (fmt = L.fv, d = L.kvd, b = L.bv)] : (r.dn_ba_f32 ? [(fmt = L.fqkv, d = r.dn_cd, b = L.bqkv), (fmt = L.fz, d = r.dn_di, b = L.bz)] : [(fmt = L.fqkv, d = r.dn_cd, b = L.bqkv), (fmt = L.fz, d = r.dn_di, b = L.bz), (fmt = L.fbeta, d = r.dn_nvh, b = L.bbeta), (fmt = L.falpha, d = r.dn_nvh, b = L.balpha)])
    for (site in sites) {
        let m = crd_site_mmq(r, site.fmt, r.dim, site.d, site.b)
        mmq = mmq || (m && site.fmt != int(KqFmt.nv4))
        fp4 = fp4 || (m && site.fmt == int(KqFmt.nv4))
        f16 = f16 || !m
    }
    f16 = f16 || (L.recurrent && r.dn_ba_f32)
    return (mmq = mmq, fp4 = fp4, f16 = f16)
}

//! the x feeds a layer's gate/up sites read: Q8_0 for an int8 MMQ leaf, FP4 for the FP4 leaf, f16 for the cm2 tiles
def private crd_gu_feeds(r : CudRdec; L : CudLayer) : tuple<mmq : bool; fp4 : bool; f16 : bool> {
    var mmq = false
    var fp4 = false
    var f16 = false
    for (site in [(fmt = L.f1, b = L.b1), (fmt = L.f3, b = L.b3)]) {
        let m = crd_site_mmq(r, site.fmt, r.dim, L.hid, site.b)
        mmq = mmq || (m && site.fmt != int(KqFmt.nv4))
        fp4 = fp4 || (m && site.fmt == int(KqFmt.nv4))
        f16 = f16 || !m
    }
    return (mmq = mmq, fp4 = fp4, f16 = f16)
}

//! the residual step's normed rows into each feed the next sites read; the first pass adds the partner, every later one norms the updated stream
def private crd_x_norm(r : CudRdec; var s : void?; aa : uint64; pc : GkAddRmsArgs; wlen : int64; feeds : tuple<mmq : bool; fp4 : bool; f16 : bool>) {
    var pcx = pc
    var partner = aa
    if (feeds.fp4) {
        enc_cls_ar_fp4_b(s, fixed_array(r.pf_x, partner, r.norms, r.pf_x4q, r.pf_x4s, r.pf_xrs), pcx, wlen)
        pcx.add_on = 0u
        pcx.pre_on = 0u
        partner = r.pf_x
    }
    if (feeds.mmq) {
        if (r.pf_mmq_min) {
            enc_cls_ar_rq_sum_b(s, fixed_array(r.pf_x, partner, r.norms, r.pf_xq, r.pf_xs, r.pf_xsum), pcx, wlen)
        } else {
            enc_cls_ar_rq_b(s, fixed_array(r.pf_x, partner, r.norms, r.pf_xq, r.pf_xs), pcx, wlen)
        }
        pcx.add_on = 0u
        pcx.pre_on = 0u
        partner = r.pf_x
    }
    if (feeds.f16) {
        enc_cls_ar_f16_b(s, fixed_array(r.pf_x, partner, r.norms, r.pf_xf), pcx, wlen)
    }
}

//! one window GEMM on an MMQ leaf (`wblk` in its format's block unit); the rows pad to the tile, the pad rows reading stale feed rows into y past the window, where nothing reads
def private crd_mmq(fmt : int; wblk : int64; a : CudArena; xq, xs, xsum, y : uint64; n, d, rows : int64) {
    let rp = (rows + 127l) / 128l * 128l
    var pc = MmqArgs(n = uint(n), d = uint(d), rows = uint(rp), wblk = uint(wblk))
    let wgs = mmq_wgs(d, rp)
    var bufs = fixed_array(a.wq, a.ws, xq, xs, y, xsum)
    cud_mmq_enc(fmt, g_cud.stream, bufs, pc, wgs)
}

//! one window GEMM on the FP4 leaf over the FP4 feed; the rows pad to the tile as the int8 leaves' do
def private crd_mmq_fp4(wblk : int64; a : CudArena; x4q, x4s, xrs, y : uint64; n, d, rows : int64) {
    let rp = (rows + 127l) / 128l * 128l
    var pc = MmqFp4Args(n = uint(n), d = uint(d), rows = uint(rp), wblk = uint(wblk))
    enc_nv4_mmq(g_cud.stream, fixed_array(a.wq, a.ws, x4q, x4s, xrs, y), pc, mmq_wgs(d, rp))
}

//! a window GEMM site on its plane's feed: FP4 on the FP4 leaf, Q8_0 on an int8 MMQ leaf, else f16 on the format's cm2 tiles
def private crd_site(r : CudRdec; var sc : CudSched; fmt : int; wblk : int64; xf, xq, xs, xsum, x4q, x4s, xrs, y : uint64; n, d, rows : int64) {
    let a = g_cud_arenas[fmt]
    if (!crd_site_mmq(r, fmt, n, d, wblk)) {
        crd_gemm(sc, fmt, wblk, a, xf, y, n, d, rows)
    } elif (fmt == int(KqFmt.nv4)) {
        crd_mmq_fp4(wblk, a, x4q, x4s, xrs, y, n, d, rows)
    } else {
        crd_mmq(fmt, wblk, a, xq, xs, xsum, y, n, d, rows)
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
    cud_cm2_enc(fmt, pk.tc == 256l ? CUD_CM2_TC_L : (pk.tc == 128l ? CUD_CM2_TC_M : CUD_CM2_TC_S), g_cud.stream, bufs, pc, wgs)
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
    if (r.hybrid && np > DN_WINDOW) {
        panic("cud prefill: a window of {np} rows on a recurrent model - the smalls planes and the scan workspace hold {DN_WINDOW}")
    }
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
    r.pf_q = dev_zeroed(np * (r.q_gated ? 2l * qd : qd) * 4l)
    if (r.hybrid) {
        let cd = r.dn_cd
        let di = r.dn_di
        r.pf_dnqkv = dev_zeroed(np * cd * 4l)
        r.pf_dnconv = dev_zeroed(np * cd * 4l)
        r.pf_dnz = dev_zeroed(np * di * 4l)
        r.pf_dnbg = dev_zeroed(np * r.dn_nvh * 4l)
        r.pf_dnws = dev_zeroed(np * di * 4l)
        r.pf_dnof = dev_zeroed(slack * di * 2l)
        r.pf_dnoq = dev_zeroed(np * di)
        r.pf_dnos = dev_zeroed(np * (di / Q8_BLOCK_ELEMS) * 4l)
    }
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
    r.pf_knob = crd_mmq_knob()
    let leaves = crd_mmq_leaves(r)
    r.pf_mmq_min = leaves.min
    r.pf_fp4 = leaves.fp4
    if (r.pf_fp4) {
        r.pf_x4q = dev_zeroed(np * dim / FP4_NIBBLES_PER_BYTE)
        r.pf_x4s = dev_zeroed(np * dim / NV4_FEED_STRIP)
        r.pf_xrs = dev_zeroed(np * 4l)
        r.pf_a4q = dev_zeroed(np * qd / FP4_NIBBLES_PER_BYTE)
        r.pf_a4s = dev_zeroed(np * qd / NV4_FEED_STRIP)
        r.pf_ars = dev_zeroed(np * 4l)
        r.pf_h4q = dev_zeroed(np * hid / FP4_NIBBLES_PER_BYTE)
        r.pf_h4s = dev_zeroed(np * hid / NV4_FEED_STRIP)
        r.pf_hrs = dev_zeroed(np * 4l)
        if (r.hybrid) {
            r.pf_dno4q = dev_zeroed(np * r.dn_di / FP4_NIBBLES_PER_BYTE)
            r.pf_dno4s = dev_zeroed(np * r.dn_di / NV4_FEED_STRIP)
            r.pf_dnors = dev_zeroed(np * 4l)
        }
    }
    r.pf_aq = dev_zeroed(np * qd)
    r.pf_as = dev_zeroed(np * (qd / Q8_BLOCK_ELEMS) * 4l)
    r.pf_hq = dev_zeroed(np * hid)
    r.pf_hs = dev_zeroed(np * (hid / Q8_BLOCK_ELEMS) * 4l)
    if (r.pf_mmq_min) {
        r.pf_xsum = dev_zeroed(np * (dim / Q8_BLOCK_ELEMS) * 4l)
        r.pf_asum = dev_zeroed(np * (qd / Q8_BLOCK_ELEMS) * 4l)
        r.pf_hsum = dev_zeroed(np * (hid / Q8_BLOCK_ELEMS) * 4l)
        if (r.hybrid) {
            r.pf_dnosum = dev_zeroed(np * (r.dn_di / Q8_BLOCK_ELEMS) * 4l)
        }
    }
    let pick = empty(leaves.planes) ? "the f16-fed cm2 tiles" : "the MMQ kernels (int8 over Q8_0 feeds; NVFP4 on the FP4 tensor cores) for the {leaves.planes} planes, the f16-fed cm2 tiles for the rest"
    let knob_name = g_cuda_mmq_pin != CUDA_MMQ_UNPINNED ? "set_cuda_mmq_pin" : "DASLLAMA_CUDA_MMQ"
    to_log(LOG_INFO, "dasLLAMA cuda tier: the window chain's GEMMs on {pick} ({knob_name}={r.pf_knob})\n")
    r.pf_splitk_bytes = 262_144l * int64(g_cud.sm_count)
    r.pf_splitk = dev_zeroed(r.pf_splitk_bytes)
    let dmax = max(max(max(r.q_gated ? 2l * qd : qd, 2l * kvd), max(dim, hid)), r.hybrid ? r.dn_cd : 0l)
    r.pf_sched_words = 4l + max(cm2_tiles_at(dmax, np, 32l), 8l * cm2_tiles_at(dmax, np, 128l))
    if (r.moe) {
        crd_pf_moe_setup(r, np)
    }
    for (l in range64(r.n_layers)) {
        var L & = unsafe(r.layers[l])
        for (i in range(SCH_COUNT)) {
            L.sch[i] = cud_sched_make(r.pf_sched_words)
        }
    }
}

//! under `rot` the rows land rotated, and the un-rotation runs before the convert or the requant
def private crd_window_attn(r : CudRdec; l : int64; L : CudLayer; kmir, vmir : uint64; wsegs : array<RdecPfSeg>; wlen : int64; shadow, rot : bool) {
    var s = g_cud.stream
    let qd = L.qd
    let kvd = uint(L.kvd)
    let hs = L.hs
    let ring = shadow ? 0u : uint(L.ring)
    let kv16 = shadow || r.kv_dt == KVDtype.f16
    let o_mmq = crd_site_mmq(r, L.fo, qd, r.dim, L.bo)
    let f16out = !o_mmq && !rot   // the flash tile's f16 twin lands the cm2 feed itself
    let fa = kv16 && (hs == 64l || hs == 96l || hs == 128l || hs == 256l) && (!crd_attn_gated(r) || hs == 256l)   // the gate arm is the h256 stamps' alone: a gated model at another head takes the chunked tiles
    let convert = !o_mmq && !(fa && f16out)
    let qrow = crd_pf_q_row_stride(r, L)
    let qstride = crd_q_head_stride(r, L)
    for (sg in wsegs) {
        let mir_base = shadow ? 0u : uint(crd_region_base(r, L, sg.region))
        if (fa) {
            var pcf = FaArgs(rows = uint(sg.npos), w0 = uint(sg.pos0), qd = uint(qd), kvd = kvd, kv_mul = uint(L.kv_mul),
                kvbase = mir_base, scale = r.scale, qstride = uint(qrow), qhs = uint(qstride), window = uint(L.window), cap = L.attn_cap,
                sinkoff = uint(l * r.n_heads), row0 = uint(sg.row0), ring = ring)
            var fbufs = fixed_array(r.pf_q, kmir, vmir, f16out ? r.pf_af : r.pf_attn)
            cud_fa_enc(s, hs, crd_attn_gated(r), f16out, r.has_sinks, fbufs, r.sinks, pcf, fa_stamp_wgs(false, r.n_heads, sg.npos))
        } else {
            var pca = DaAttnBArgs(npos = uint(sg.npos), nh = uint(r.n_heads), kvd = kvd, hs = uint(hs), kv_mul = uint(L.kv_mul),
                kbase = mir_base, vbase = mir_base, w0 = uint(sg.pos0), scale = r.scale, qhs = uint(qstride), gated = crd_attn_gated(r) ? 1u : 0u,
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
        crd_tq4_unrot(r, s, r.pf_attn, r.pf_q, hs, wlen, qd, qrow, qstride)
    }
    if (convert) {
        var pcc = gk_f16_rows_args(uint(wlen * qd))
        enc_tower_clamp_cvt_cls(s, fixed_array(r.pf_attn, r.pf_af), pcc, rq_wgs(false, wlen * qd))
    }
    if (o_mmq && L.fo == int(KqFmt.nv4)) {
        var pcf = Fp4RowsArgs(k = uint(qd), inbase = 0u)
        enc_fp4_rows(s, fixed_array(r.pf_attn, r.pf_a4q, r.pf_a4s, r.pf_ars), pcf, wlen)
    } elif (o_mmq) {
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
        let nkh = L.kv_shared ? 0l : kvd / L.hs
        var qbufs = fixed_array(r.pf_q, r.pf_kv, kvq ? r.pf_kst : kmir, kvq ? r.pf_vst : vmir, r.pf_cos, r.norms, r.tok, crd_bias_plane(r), r.pf_kv)
        for (sg in wsegs) {
            var pcf = crd_qkn_rope_args(r, l, L, kvq ? 0l : crd_region_base(r, L, sg.region), nkh, kstride, crd_pf_q_row_stride(r, L), kvq ? 0l : L.ring)
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
    var rbufs = fixed_array(r.pf_q, r.pf_kv, kvq ? r.pf_kst : kmir, kvq ? r.pf_vst : vmir, r.pf_cos, crd_bias_plane(r))
    for (sg in wsegs) {
        var pcr = RopeKvBArgs(qd = uint(qd), kvd = uint(kvd), hs = hs, half = uint(L.rot / 2l), neox = r.neox ? 1u : 0u,
            kpair0 = uint(qd / 2l), ppp = uint(pairs), layerbase = kvq ? 0u : uint(crd_region_base(r, L, sg.region)), npos = uint(sg.npos),
            pos0 = uint(kvq ? sg.row0 : sg.pos0), voff = uint(voff), kstride = uint(kstride), qhs = hs, boff = uint(L.boff), hasb = crd_hasb(r),
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

//! a block codec's window after the rope staged the floats: the store quantizes them into the mirror, the shadow pass dequantizes what the tiles read
def private crd_window_kv_codec(r : CudRdec; L : CudLayer; kmir, vmir : uint64; wsegs : array<RdecPfSeg>; wlen : int64) {
    var s = g_cud.stream
    let kvd = L.kvd
    let hs = uint(L.hs)
    let pos0w = wsegs[0].pos0
    let mir_base = uint(crd_region_base(r, L, wsegs[0].region))
    let tq4 = r.kv_dt == KVDtype.tq4
    if (tq4) {
        let nkh = kvd / L.hs
        var pct = KvTqStoreArgs(kvd = uint(kvd), hs = hs, layerbase = mir_base, pos0 = uint(pos0w), nqh = uint(r.n_heads), nkh = uint(nkh),
            qrow = uint(crd_pf_q_row_stride(r, L)), qstride = uint(crd_q_head_stride(r, L)), inv_sqrt_hs = 1.0 / sqrt(float(hs)), ring = uint(L.ring))
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

let private CUD_MOE_SCHED_BYTES = 262_144l         //! an expert tile schedule's plane: the records, then the s and m dispatches' maps
def private crd_moe_bounds(r : CudRdec; nk : int64) : tuple<gu : int64; dn : int64; mgu : int64; mdn : int64> => moe_sched_bounds(r.ne, r.nfe, r.dim, nk, false)

//! the window chain's routed-block planes, once a residency
def private crd_pf_moe_setup(var r : CudRdec; np : int64) {
    let dim = r.dim
    let nfe = r.nfe
    let nk = np * r.k
    r.pf_nk = nk
    let bounds = crd_moe_bounds(r, nk)
    verify((MOE_SCHED_MAP_OFF + max(bounds.gu + bounds.mgu, bounds.dn + bounds.mdn)) * 4l <= CUD_MOE_SCHED_BYTES, "cud prefill: the routed block's schedule exceeds its plane")
    verify(max(bounds.gu, bounds.dn) <= 65535l, "cud prefill: the routed block's s dispatch outgrows the schedule's 16-bit workgroup count")
    r.pf_xb = dev_zeroed(np * dim * 4l)
    r.pf_mlog = dev_zeroed(np * r.ner * 4l)
    r.pf_midx = dev_zeroed(nk * 4l)
    r.pf_mw = dev_zeroed(nk * 4l)
    r.pf_minv = dev_zeroed(nk * 4l)
    r.pf_bex = dev_zeroed(crd_moe_pad_rows(r, nk) * 4l)   // every bucket row, the FP4 expert chain's pads among them
    r.pf_msch1 = dev_zeroed(CUD_MOE_SCHED_BYTES)
    r.pf_msch3 = dev_zeroed(CUD_MOE_SCHED_BYTES)
    r.pf_msch2 = dev_zeroed(CUD_MOE_SCHED_BYTES)
    r.pf_mxg = dev_zeroed((nk + TILE_READ_SLACK) * dim * 2l)
    r.pf_mhf = dev_zeroed((nk + TILE_READ_SLACK) * nfe * 2l)
    r.pf_my1 = dev_zeroed(nk * nfe * 4l)
    r.pf_my2 = dev_zeroed(nk * nfe * 4l)
    r.pf_mdn = dev_zeroed(nk * dim * 4l)
    var any_mmq = false
    for (L in r.layers) {
        any_mmq ||= L.moe_mmq
    }
    if (any_mmq && r.pf_mmq_rows == 0l) {
        crd_pf_moe_mmq_setup(r)
    }
}

let private CUD_MOE_ROW_PAD = 128l   //! the FP4 expert chain's bucket row pad: a bucket's rows start at a tile's multiple, so a tile past them lands in its pad

//! the bucket rows of a window of `nk` slot rows under the FP4 expert chain's pad: every expert's bucket padded to the tile at most
def private crd_moe_pad_rows(r : CudRdec; nk : int64) : int64 => nk + r.ne * (CUD_MOE_ROW_PAD - 1l)

//! the FP4 expert chain's planes, once a residency, made on the first routed layer whose stacks take the leaf (`CudLayer.moe_mmq`)
def private crd_pf_moe_mmq_setup(var r : CudRdec) {
    let dim = r.dim
    let nfe = r.nfe
    let rows = crd_moe_pad_rows(r, r.pf_nk)
    let xrows = rows + TILE_READ_SLACK
    r.pf_mmq_rows = rows
    r.pf_mxf = dev_zeroed(xrows * dim * 4l)
    r.pf_mx4q = dev_zeroed(xrows * dim / FP4_NIBBLES_PER_BYTE)
    r.pf_mx4s = dev_zeroed(xrows * (dim / MX4_FEED_WORD_ELEMS) * 4l)
    r.pf_mxrs = dev_zeroed(xrows * 4l)
    r.pf_mh4q = dev_zeroed(xrows * nfe / FP4_NIBBLES_PER_BYTE)
    r.pf_mh4s = dev_zeroed(xrows * (nfe / MX4_FEED_WORD_ELEMS) * 4l)
    r.pf_mhrs = dev_zeroed(xrows * 4l)
    r.pf_mmy1 = dev_zeroed(rows * nfe * 4l)
    r.pf_mmy2 = dev_zeroed(rows * nfe * 4l)
    r.pf_mmdn = dev_zeroed(rows * dim * 4l)
    to_log(LOG_INFO, "dasLLAMA cuda resident: the routed block's expert pieces on the FP4 expert chain under DASLLAMA_CUDA_MMQ={r.pf_knob != CUDA_MMQ_UNPINNED ? r.pf_knob : crd_mmq_knob()} (set_cuda_mmq_pin under a test's pin; 0 keeps them on the cm2 s and e tiles) - the gathered rows as the MXFP4 feed, every bucket on padded 128-row tiles ({rows} bucket rows a window)\n")
}

//! one mx4 expert plane's FP4 tiles over the schedule's m pieces (its map at `map_off`), the rows into `y` [bucket rows x d]
def private crd_moe_mmq(var s : void?; fmt : int; sched : uint64; map_off, wgs : int64; x4q, x4s, xrs, y : uint64; n, d : int64) {
    verify(fmt == int(KqFmt.mx4), "cud prefill: the FP4 expert chain serves the mx4 stacks alone")
    let a = g_cud_arenas[fmt]
    var pc = MmqFp4Args(n = uint(n), d = uint(d), rows = 0u, wblk = 0u, map_off = uint(map_off))
    enc_mx4_fp4_mmq_moe(s, fixed_array(a.wq, a.ws, x4q, x4s, xrs, y, sched), pc, wgs)
}

def private crd_x_feeds_from(r : CudRdec; var s : void?; src : uint64; wlen : int64; feeds : tuple<mmq : bool; fp4 : bool; f16 : bool>) {
    let dim = r.dim
    if (feeds.f16) {
        var pcc = gk_f16_rows_args(uint(wlen * dim))
        enc_tower_clamp_cvt_cls(s, fixed_array(src, r.pf_xf), pcc, rq_wgs(false, wlen * dim))
    }
    if (feeds.mmq) {
        var pcq = RqArgs(inbase = 0u, nblk = uint(wlen * dim / kq_unit(false)), lo = 0.0, hi = 0.0)
        if (r.pf_mmq_min) {
            enc_cls_dn_rq_sum(s, fixed_array(src, r.pf_xq, r.pf_xs, r.pf_xsum), pcq, rq_wgs(false, wlen * dim))
        } else {
            enc_cls_dn_rq(s, fixed_array(src, r.pf_xq, r.pf_xs), pcq, rq_wgs(false, wlen * dim))
        }
    }
    if (feeds.fp4) {
        var pcf = Fp4RowsArgs(k = uint(dim), inbase = 0u)
        enc_fp4_rows(s, fixed_array(src, r.pf_x4q, r.pf_x4s, r.pf_xrs), pcf, wlen)
    }
}

//! one expert stack's tiles over the window's schedule: the m pieces on the e column, then the s pieces on the s column, each
//! dispatch over [wq, ws, schedule, xg, y] at its map
def private crd_moe_tiles(var s : void?; fmt : int; sched, xg, y : uint64; n, d : int64; map_s, map_m, wgs_s, wgs_m : int64) {
    let a = g_cud_arenas[fmt]
    var pcm = BatchArgs(n = uint(n), d = uint(d), map_off = uint(map_m), ksplit = 0u)
    cud_cm2_enc(fmt, CUD_CM2_TC_E, s, fixed_array(a.wq, a.ws, sched, xg, y), pcm, wgs_m)
    var pcs = BatchArgs(n = uint(n), d = uint(d), map_off = uint(map_s), ksplit = 0u)
    cud_cm2_enc(fmt, CUD_CM2_TC_S, s, fixed_array(a.wq, a.ws, sched, xg, y), pcs, wgs_s)
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#vk-prefill-moe-block")]
def private crd_window_moe(r : CudRdec; var s : void?; l : int64; var L : CudLayer; nlfin : uint; w0, wlen : int64; segs, wsegs : array<RdecPfSeg>) {
    let dim = r.dim
    let nfe = r.nfe
    let k = r.k
    let ne = r.ne
    let nk = wlen * k
    let hid = L.hid
    var pc8 = crd_wo_ar_args(r, l, L)
    enc_cls_ar(s, fixed_array(r.pf_x, r.pf_xb2, r.norms, r.pf_xb), pc8, wlen)
    if (L.b1 >= 0l) {
        crd_x_feeds_from(r, s, r.pf_xb, wlen, crd_gu_feeds(r, L))
        crd_site(r, L.sch[SCH_G], L.f1, L.b1, r.pf_xf, r.pf_xq, r.pf_xs, r.pf_xsum, r.pf_x4q, r.pf_x4s, r.pf_xrs, r.pf_gate, dim, hid, wlen)
        crd_site(r, L.sch[SCH_U], L.f3, L.b3, r.pf_xf, r.pf_xq, r.pf_xs, r.pf_xsum, r.pf_x4q, r.pf_x4s, r.pf_xrs, r.pf_up, dim, hid, wlen)
        var pc12 = ActArgs(nelem = uint(wlen * hid), act = L.gelu ? ACT_GELU : ACT_SILU, nblk = uint(wlen * hid / kq_unit(false)), elem0 = 0u)
        if (!crd_site_mmq(r, L.f2, hid, dim, L.b2)) {
            enc_actf16_cls(s, fixed_array(r.pf_gate, r.pf_up, r.pf_hf), pc12, rq_wgs(false, wlen * hid))
        } elif (L.f2 == int(KqFmt.nv4)) {
            var pcf = Fp4RowsArgs(k = uint(hid), inbase = 0u, act = L.gelu ? ACT_GELU : ACT_SILU)
            enc_fp4_actrows(s, fixed_array(r.pf_gate, r.pf_up, r.pf_h4q, r.pf_h4s, r.pf_hrs), pcf, wlen)
        } elif (r.pf_mmq_min) {
            enc_q8_actrq_sum_cls(s, fixed_array(r.pf_gate, r.pf_up, r.pf_hq, r.pf_hs, r.pf_hsum), pc12, rq_wgs(false, wlen * hid))
        } else {
            enc_q8_actrq_cls(s, fixed_array(r.pf_gate, r.pf_up, r.pf_hq, r.pf_hs), pc12, rq_wgs(false, wlen * hid))
        }
        crd_site(r, L.sch[SCH_D], L.f2, L.b2, r.pf_hf, r.pf_hq, r.pf_hs, r.pf_hsum, r.pf_h4q, r.pf_h4s, r.pf_hrs, r.pf_ffnout, hid, dim, wlen)
    }
    var pcg = RouterGemmArgs(npos = uint(wlen), ne = uint(r.ner), dim = uint(dim), woff = uint(L.router_slot * r.ner * dim), ostride = uint(r.ner))
    enc_router_gemm_cls(s, fixed_array(r.router, r.pf_xb, r.pf_mlog), pcg, router_gemm_wgs(wlen, r.ner))
    var pct = crd_topk_args(r, L)
    cud_topk_rows_enc(ne, s, fixed_array(r.pf_mlog, r.pf_mw, r.moe_ds, r.moe_rb, r.pf_midx), pct, wlen)
    let bounds = crd_moe_bounds(r, nk)
    let map_m_gu = MOE_SCHED_MAP_OFF + bounds.gu
    let map_m_dn = MOE_SCHED_MAP_OFF + bounds.dn
    var pcs = SchedArgs(nk = uint(nk), ne = uint(ne), wt_gu = uint((nfe + 127l) / 128l), wt_dn = uint((dim + 127l) / 128l), s_only = 0u,
        map_off = uint(MOE_SCHED_MAP_OFF), map_off_mgu = uint(map_m_gu), map_off_mdn = uint(map_m_dn),
        bound_gu = uint(bounds.gu), bound_dn = uint(bounds.dn), bound_mgu = uint(bounds.mgu), bound_mdn = uint(bounds.mdn),
        base1 = uint(L.be1), stride1 = uint(nfe * dim / kq_unit(L.fe1)),
        base3 = uint(L.be3), stride3 = uint(nfe * dim / kq_unit(L.fe3)),
        base2 = uint(L.be2), stride2 = uint(nfe * dim / kq_unit(L.fe2)),
        row_pad = L.moe_mmq ? uint(CUD_MOE_ROW_PAD) : 0u, m_only = L.moe_mmq ? 1u : 0u)
    cud_moe_sched_enc(ne, s, fixed_array(r.pf_midx, r.pf_minv, r.pf_msch1, r.pf_msch3, r.pf_msch2, r.pf_bex), pcs)
    var pca = ActArgs(nelem = uint(nk * nfe), act = act_code(r.act_oai, r.is_gelu), nblk = uint(nk * nfe / kq_unit(false)), elem0 = 0u,
        bias_on = r.exps_bias ? 1u : 0u, gboff = uint(L.router_slot * ne * nfe), uboff = uint(L.router_slot * ne * nfe), brow = uint(nfe))
    var mdn = r.pf_mdn
    if (L.moe_mmq) {
        let bucket_rows = crd_moe_pad_rows(r, nk)   // (the planes hold the residency's widest)
        var pcgp = GatherArgs(n = uint(dim), k = uint(k), r0 = 0u, r1 = uint(bucket_rows))
        enc_moe_gather_f32_cls(s, fixed_array(r.pf_xb, r.pf_minv, r.pf_mxf), pcgp, nk)
        var pcx = Fp4RowsArgs(k = uint(dim), inbase = 0u)
        enc_mx4_rows(s, fixed_array(r.pf_mxf, r.pf_mx4q, r.pf_mx4s, r.pf_mxrs), pcx, bucket_rows + TILE_READ_SLACK)
        crd_moe_mmq(s, L.fe1, r.pf_msch1, map_m_gu, bounds.mgu, r.pf_mx4q, r.pf_mx4s, r.pf_mxrs, r.pf_mmy1, dim, nfe)
        crd_moe_mmq(s, L.fe3, r.pf_msch3, map_m_gu, bounds.mgu, r.pf_mx4q, r.pf_mx4s, r.pf_mxrs, r.pf_mmy2, dim, nfe)
        var pch = Fp4RowsArgs(k = uint(nfe), inbase = 0u, act = pca.act, gboff = pca.gboff, uboff = pca.uboff)
        if (r.exps_bias) {
            enc_mx4_actrows_b(s, fixed_array(r.pf_mmy1, r.pf_mmy2, r.pf_mh4q, r.pf_mh4s, r.pf_mhrs, r.moe_gb, r.moe_ub, r.pf_bex), pch, bucket_rows)
        } else {
            enc_mx4_actrows(s, fixed_array(r.pf_mmy1, r.pf_mmy2, r.pf_mh4q, r.pf_mh4s, r.pf_mhrs), pch, bucket_rows)
        }
        crd_moe_mmq(s, L.fe2, r.pf_msch2, map_m_dn, bounds.mdn, r.pf_mh4q, r.pf_mh4s, r.pf_mhrs, r.pf_mmdn, nfe, dim)
        mdn = r.pf_mmdn
    } else {
        var pcgt = GatherArgs(n = uint(dim), k = uint(k), r0 = 0u, r1 = uint(nk))
        enc_moe_gather_f16_cls(s, fixed_array(r.pf_xb, r.pf_minv, r.pf_mxg), pcgt, nk)
        crd_moe_tiles(s, L.fe1, r.pf_msch1, r.pf_mxg, r.pf_my1, dim, nfe, MOE_SCHED_MAP_OFF, map_m_gu, bounds.gu, bounds.mgu)
        crd_moe_tiles(s, L.fe3, r.pf_msch3, r.pf_mxg, r.pf_my2, dim, nfe, MOE_SCHED_MAP_OFF, map_m_gu, bounds.gu, bounds.mgu)
        if (r.exps_bias) {   // the bucket rows' experts (`pf_bex`) name the gate and up bias rows that join before the activation
            enc_actf16_b_cls(s, fixed_array(r.pf_my1, r.pf_my2, r.pf_mhf, r.moe_gb, r.moe_ub, r.pf_bex), pca, rq_wgs(false, nk * nfe))
        } else {
            enc_actf16_cls(s, fixed_array(r.pf_my1, r.pf_my2, r.pf_mhf), pca, rq_wgs(false, nk * nfe))
        }
        crd_moe_tiles(s, L.fe2, r.pf_msch2, r.pf_mhf, r.pf_mdn, nfe, dim, MOE_SCHED_MAP_OFF, map_m_dn, bounds.dn, bounds.mdn)
    }
    let last_layer = l + 1l == r.n_layers
    var pc14 = crd_comb_args(r, l, L, last_layer ? nlfin : uint(norm_off(l + 1l, RDEC_NORM_ATT)))
    enc_cls_ar_comb(s, fixed_array(r.pf_x, r.pf_ffnout, r.norms, r.pf_xb, r.pf_minv, r.pf_mw, mdn, r.pf_mlog, r.moe_db, r.pf_midx), pc14, wlen)
    if (!last_layer) {
        crd_x_feeds_from(r, s, r.pf_xb, wlen, crd_head_feeds(r, r.layers[l + 1l]))
        return
    }
    for (ws in wsegs) {
        let lr = ws.row0 + ws.npos - 1l
        continue if (rdec_seg_ending_at(segs, w0, lr) < 0l)
        crd_rq_rows(s, r.cls_fmt, r.pf_xb + uint64(lr * dim * 4l), r.pf_xq + uint64(lr * dim), r.pf_xs + uint64(lr * (dim / Q8_BLOCK_ELEMS) * 4l), dim)
    }
}

//! a segment ending in the window (`wsegs` its parts, `segs` the call's) takes the classifier at its last row
def private crd_window(var r : CudRdec; w0, wlen : int64; segs, wsegs : array<RdecPfSeg>; last : bool) {
    var s = g_cud.stream
    let dim = r.dim
    let nlfin = uint(rdec_norms_final_base(r.n_layers, dim))
    let pc_pro = GkAddRmsArgs(dim = uint(dim), add_on = 0u, woff = 0u, eps = r.eps, ascale = 1.0)
    crd_x_norm(r, s, r.pf_x, pc_pro, wlen, crd_head_feeds(r, r.layers[0]))
    for (l in range64(r.n_layers)) {
        var L & = unsafe(r.layers[l])
        let qd = L.qd
        let kvd = L.kvd
        let hid = L.hid
        if (L.recurrent) {
            crd_window_dn(r, L, wlen, wsegs)
        } else {
        let kmir = r.k_slabs[L.mir_slab]
        let vmir = r.v_slabs[L.mir_slab]
        crd_site(r, L.sch[SCH_Q], L.fq, L.bq, r.pf_xf, r.pf_xq, r.pf_xs, r.pf_xsum, r.pf_x4q, r.pf_x4s, r.pf_xrs, r.pf_q, dim, crd_pf_q_row_stride(r, L), wlen)
        let kv_merged = crd_kv_merged(dim, L)
        var voff = kvd
        var kstride = 2l * kvd
        if (kv_merged) {
            crd_site(r, L.sch[SCH_K], L.fk, L.bk, r.pf_xf, r.pf_xq, r.pf_xs, r.pf_xsum, r.pf_x4q, r.pf_x4s, r.pf_xrs, r.pf_kv, dim, 2l * kvd, wlen)
        } else {
            crd_site(r, L.sch[SCH_K], L.fk, L.bk, r.pf_xf, r.pf_xq, r.pf_xs, r.pf_xsum, r.pf_x4q, r.pf_x4s, r.pf_xrs, r.pf_kv, dim, kvd, wlen)
            crd_site(r, L.sch[SCH_V], L.fv, L.bv, r.pf_xf, r.pf_xq, r.pf_xs, r.pf_xsum, r.pf_x4q, r.pf_x4s, r.pf_xrs, r.pf_v, dim, kvd, wlen)
            cuda_memcpy_dtod(r.pf_kv + uint64(wlen * kvd * 4l), r.pf_v, uint64(wlen * kvd * 4l), s)
            voff = wlen * kvd
            kstride = kvd
        }
        let kvq = kv_block_codec(r.kv_dt)
        crd_window_kv(r, l, L, kmir, vmir, wsegs, wlen, voff, kstride)
        crd_window_attn(r, l, L, kvq ? r.ksh : kmir, kvq ? r.vsh : vmir, wsegs, wlen, kvq, r.kv_dt == KVDtype.tq4)
        crd_site(r, L.sch[SCH_O], L.fo, L.bo, r.pf_af, r.pf_aq, r.pf_as, r.pf_asum, r.pf_a4q, r.pf_a4s, r.pf_ars, r.pf_xb2, qd, dim, wlen)
        }
        if (L.moe) {
            crd_window_moe(r, s, l, L, nlfin, w0, wlen, segs, wsegs)
            continue
        }
        let pc8 = crd_wo_ar_args(r, l, L)
        crd_x_norm(r, s, r.pf_xb2, pc8, wlen, crd_gu_feeds(r, L))
        crd_site(r, L.sch[SCH_G], L.f1, L.b1, r.pf_xf, r.pf_xq, r.pf_xs, r.pf_xsum, r.pf_x4q, r.pf_x4s, r.pf_xrs, r.pf_gate, dim, hid, wlen)
        crd_site(r, L.sch[SCH_U], L.f3, L.b3, r.pf_xf, r.pf_xq, r.pf_xs, r.pf_xsum, r.pf_x4q, r.pf_x4s, r.pf_xrs, r.pf_up, dim, hid, wlen)
        var pc12 = ActArgs(nelem = uint(wlen * hid), act = L.gelu ? ACT_GELU : ACT_SILU, nblk = uint(wlen * hid / kq_unit(false)), elem0 = 0u)
        if (!crd_site_mmq(r, L.f2, hid, dim, L.b2)) {
            enc_actf16_cls(s, fixed_array(r.pf_gate, r.pf_up, r.pf_hf), pc12, rq_wgs(false, wlen * hid))
        } elif (L.f2 == int(KqFmt.nv4)) {
            var pcf = Fp4RowsArgs(k = uint(hid), inbase = 0u, act = L.gelu ? ACT_GELU : ACT_SILU)
            enc_fp4_actrows(s, fixed_array(r.pf_gate, r.pf_up, r.pf_h4q, r.pf_h4s, r.pf_hrs), pcf, wlen)
        } elif (r.pf_mmq_min) {
            enc_q8_actrq_sum_cls(s, fixed_array(r.pf_gate, r.pf_up, r.pf_hq, r.pf_hs, r.pf_hsum), pc12, rq_wgs(false, wlen * hid))
        } else {
            enc_q8_actrq_cls(s, fixed_array(r.pf_gate, r.pf_up, r.pf_hq, r.pf_hs), pc12, rq_wgs(false, wlen * hid))
        }
        crd_site(r, L.sch[SCH_D], L.f2, L.b2, r.pf_hf, r.pf_hq, r.pf_hs, r.pf_hsum, r.pf_h4q, r.pf_h4s, r.pf_hrs, r.pf_ffnout, hid, dim, wlen)
        let last_layer = l + 1l == r.n_layers
        var pc14 = GkAddRmsArgs(dim = uint(dim), add_on = 1u, woff = last_layer ? nlfin : uint(norm_off(l + 1l, RDEC_NORM_ATT)), eps = r.eps,
            ascale = L.out_scale, pre_on = L.pre_post ? 1u : 0u, pwoff = uint(norm_off(l, RDEC_NORM_POST_FFN)))
        if (!last_layer) {
            crd_x_norm(r, s, r.pf_ffnout, pc14, wlen, crd_head_feeds(r, r.layers[l + 1l]))
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

[arch(at="../ARCHITECTURE_GPU_VULKAN_WINDOW.md#vk-prefill-dn-block")]
def private crd_window_dn(r : CudRdec; var L : CudLayer; wlen : int64; wsegs : array<RdecPfSeg>) {
    var s = g_cud.stream
    let dim = r.dim
    let cd = r.dn_cd
    let di = r.dn_di
    let nvh = r.dn_nvh
    let ds = r.dn_ds
    let taps = r.dn_dconv - 1l
    let st0 & = unsafe(L.dn[0])
    let smalls = st0.smalls_dev
    crd_site(r, L.sch[SCH_DNQKV], L.fqkv, L.bqkv, r.pf_xf, r.pf_xq, r.pf_xs, r.pf_xsum, r.pf_x4q, r.pf_x4s, r.pf_xrs, r.pf_dnqkv, dim, cd, wlen)
    crd_site(r, L.sch[SCH_DNZ], L.fz, L.bz, r.pf_xf, r.pf_xq, r.pf_xs, r.pf_xsum, r.pf_x4q, r.pf_x4s, r.pf_xrs, r.pf_dnz, dim, di, wlen)
    if (r.dn_ba_f32) {
        var pcb = DnBaArgs(npos = uint(wlen), dim = uint(dim), nvh = uint(nvh), woff = uint(L.ba_off), xstride = uint(dim),
            obase_b = uint(DN_SM_BETA), obase_g = uint(DN_SM_G), ostride = uint(nvh))
        enc_dn_ba_cls(s, fixed_array(r.dn_ba, r.pf_xf, smalls), pcb, dn_ba_wgs(wlen, nvh))
    } else {
        crd_site(r, L.sch[SCH_DNBETA], L.fbeta, L.bbeta, r.pf_xf, r.pf_xq, r.pf_xs, r.pf_xsum, r.pf_x4q, r.pf_x4s, r.pf_xrs, r.pf_dnbg, dim, nvh, wlen)
        cuda_memcpy_dtod(smalls + uint64(DN_SM_BETA * 4l), r.pf_dnbg, uint64(wlen * nvh * 4l), s)
        crd_site(r, L.sch[SCH_DNALPHA], L.falpha, L.balpha, r.pf_xf, r.pf_xq, r.pf_xs, r.pf_xsum, r.pf_x4q, r.pf_x4s, r.pf_xrs, r.pf_dnbg, dim, nvh, wlen)
        cuda_memcpy_dtod(smalls + uint64(DN_SM_G * 4l), r.pf_dnbg, uint64(wlen * nvh * 4l), s)
    }
    let img = uint(st0.ring_image_bytes / 4l)
    for (ws in wsegs) {
        let st & = unsafe(L.dn[ws.region])
        let cur = uint(r.pf_dn_cur[ws.region])
        var pcc = GkDnConvArgs(npos = uint(ws.npos), cd = uint(cd), kd = uint(r.dn_kd), ds = uint(ds), dconv = uint(r.dn_dconv),
            hist_mode = 1u, taps_off = uint(DN_SM_TAPS), hist_off = uint(st.hist_float_off) + cur * img,
            eps = r.eps, qscale = 1.0 / sqrt(float(ds)), row0 = uint(ws.row0))
        enc_dn_conv_cls(s, fixed_array(r.pf_dnqkv, r.pf_dnconv, smalls), pcc, dn_conv_wgs(cd, ws.npos))
    }
    for (ws in wsegs) {
        let st & = unsafe(L.dn[ws.region])
        var pcs = DnScanArgs(npos = uint(ws.npos), cd = uint(cd), kd = uint(r.dn_kd), nvh = uint(nvh), nkh = uint(r.dn_nkh), ds = uint(ds),
            beta_off = uint(DN_SM_BETA), g_off = uint(DN_SM_G), a_off = uint(DN_SM_A), dt_off = uint(DN_SM_DT),
            o_off = uint(DN_O_OFF), di = uint(di), region_state_base = uint(st.state_byte_off / 4l), row0 = uint(ws.row0))
        enc_dn_scan_cls(s, fixed_array(r.pf_dnconv, st.state_dev, smalls, r.pf_dnws, r.dummy, r.dummy), pcs, dn_scan_wgs(nvh, ds))
    }
    var pc3 = GkDnGateArgs(nvh = uint(nvh), di = uint(di), ds = uint(ds), o_off = uint(DN_O_OFF), wnorm_off = uint(DN_SM_WNORM), eps = r.eps)
    if (r.dn_zsig) {
        enc_dn_scan_p3_sig_cls(s, fixed_array(r.pf_dnz, r.pf_dnws, smalls), pc3, wlen)
    } else {
        enc_dn_scan_p3_cls(s, fixed_array(r.pf_dnz, r.pf_dnws, smalls), pc3, wlen)
    }
    for (ws in wsegs) {
        let st & = unsafe(L.dn[ws.region])
        let cur = uint(r.pf_dn_cur[ws.region])
        var pct = DnTailArgs(cd = uint(cd), taps = uint(taps), row0 = uint(ws.row0 + max(ws.npos - taps, 0l)), hist_off = uint(st.hist_float_off) + (1u - cur) * img,
            old_rows = uint(max(taps - ws.npos, 0l)), old_off = uint(st.hist_float_off) + cur * img)
        enc_dn_tail_cls(s, fixed_array(r.pf_dnqkv, smalls), pct, (cd * taps + 255l) / 256l)
    }
    if (!crd_site_mmq(r, L.fout, di, dim, L.bout)) {   // the o rows to the cm2 tiles' f16 feed
        var pcc = gk_f16_rows_args(uint(wlen * di))
        enc_tower_clamp_cvt_cls(s, fixed_array(r.pf_dnws, r.pf_dnof), pcc, rq_wgs(false, wlen * di))
    } elif (L.fout == int(KqFmt.nv4)) {
        var pcf = Fp4RowsArgs(k = uint(di), inbase = 0u)
        enc_fp4_rows(s, fixed_array(r.pf_dnws, r.pf_dno4q, r.pf_dno4s, r.pf_dnors), pcf, wlen)
    } else {
        var pcq = RqArgs(inbase = 0u, nblk = uint(wlen * di / kq_unit(false)), lo = 0.0, hi = 0.0)
        if (r.pf_mmq_min) {
            enc_cls_dn_rq_sum(s, fixed_array(r.pf_dnws, r.pf_dnoq, r.pf_dnos, r.pf_dnosum), pcq, rq_wgs(false, wlen * di))
        } else {
            enc_cls_dn_rq(s, fixed_array(r.pf_dnws, r.pf_dnoq, r.pf_dnos), pcq, rq_wgs(false, wlen * di))
        }
    }
    crd_site(r, L.sch[SCH_DNOUT], L.fout, L.bout, r.pf_dnof, r.pf_dnoq, r.pf_dnos, r.pf_dnosum, r.pf_dno4q, r.pf_dno4s, r.pf_dnors, r.pf_xb2, di, dim, wlen)
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
    if (r.hybrid) {
        crd_dn_begin(r, segs)
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
        if (r.hybrid) {
            crd_dn_zero_fresh(r, g_crd_wsegs)
        }
        crd_window(r, w0, wlen, segs, g_crd_wsegs, last)
        if (!cud_sync()) {
            panic("dasLLAMA cuda resident: the window chain failed on the device: {cuda_last_error()}")
        }
        if (r.hybrid) {
            crd_dn_window_done(r, g_crd_wsegs)
        }
        r.windows++
        r.tokens_served += wlen
        w0 += wlen
    }
    if (r.hybrid) {
        crd_dn_end(r, segs)
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

//! a checkpoint capture rides the speculative round's snapshots, which this home installs no seats for: an empty list asks none
def cud_rdec_prefill_caps(caps : array<RdecPfCap>) {
    verify(empty(caps), "dasLLAMA cuda tier: a prefill's checkpoint captures asked of a home with no snapshot seats")
}

//! layer `l`'s mirror rows [row0, row0 + nrows) up from the host cache (`kp` / `vp` at row 0); a recurrent layer keeps no K/V
def cud_rdec_kv_sync(l, row0, nrows : int64; kp : uint8 const?; vp : uint8 const?) {
    assert(g_crd != null, "cud_rdec_kv_sync before prepare")
    return if (nrows <= 0l)
    let L & = unsafe(g_crd.layers[l])
    return if (L.recurrent)
    verify(L.ring == 0l, "cud decode: a ringed layer's rows - the driver keeps flat slabs")
    let rowb = kv_row_units(g_crd.kv_dt, L.kvd) * g_crd.unit_bytes
    let base = uint64((crd_mir_base(*g_crd, L) + row0 * kv_row_units(g_crd.kv_dt, L.kvd)) * g_crd.unit_bytes)
    unsafe {
        cuda_memcpy_htod(g_crd.k_slabs[L.mir_slab] + base, reinterpret<void?>(kp + row0 * rowb), uint64(nrows * rowb), g_cud.stream)
        cuda_memcpy_htod(g_crd.v_slabs[L.mir_slab] + base, reinterpret<void?>(vp + row0 * rowb), uint64(nrows * rowb), g_cud.stream)
    }
}

//! ... and back into the host cache
[arch(at="../ARCHITECTURE_GPU.md#gpu-backends")]
def cud_rdec_kv_read(l, row0, nrows : int64; var kp : uint8?; var vp : uint8?) {
    assert(g_crd != null, "cud_rdec_kv_read before prepare")
    return if (nrows <= 0l)
    let L & = unsafe(g_crd.layers[l])
    return if (L.recurrent)
    verify(L.ring == 0l, "cud decode: a ringed layer's rows - the driver keeps flat slabs")
    let rowb = kv_row_units(g_crd.kv_dt, L.kvd) * g_crd.unit_bytes
    let base = uint64((crd_mir_base(*g_crd, L) + row0 * kv_row_units(g_crd.kv_dt, L.kvd)) * g_crd.unit_bytes)
    unsafe {
        cuda_memcpy_dtoh(reinterpret<void?>(kp + row0 * rowb), g_crd.k_slabs[L.mir_slab] + base, uint64(nrows * rowb), g_cud.stream)
        cuda_memcpy_dtoh(reinterpret<void?>(vp + row0 * rowb), g_crd.v_slabs[L.mir_slab] + base, uint64(nrows * rowb), g_cud.stream)
    }
    verify(cud_sync(), "cud decode: the K/V readback did not land - the caller reads the host rows as soon as this returns")
}

//! the driver's counters; `unsplit_form` says the device splits the keys at all, so the unsplit form is a form of its own
def cud_rdec_stats : tuple<tokens : int64; graph : bool; windows : int64; unsplit : int64; unsplit_form : bool> =>
    (tokens = g_crd != null ? g_crd.tokens_served : 0l, graph = g_crd != null && crd_graph_captured(*g_crd), windows = g_crd != null ? g_crd.windows : 0l,
        unsplit = g_crd != null ? g_crd.unsplit_steps : 0l, unsplit_form = g_crd != null && g_crd.unsplit_on)
