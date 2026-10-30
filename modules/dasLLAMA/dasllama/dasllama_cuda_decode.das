options gen2
options indenting = 4
options _dasllama_internal = true

module dasllama_cuda_decode shared public
options _function_length = 0   // the token chain's encode is one walk over the layers, as the Vulkan driver's is

require dasllama/dasllama_lint public
require dasllama/dasllama_cuda_common public
require dasllama/dasllama_cuda_classes public
require dasllama/dasllama_cuda_stream public   // the streamed prompt: the pool planes seat, the model drop's stream state
require dasllama/dasllama_gpu_tier
require dasllama/dasllama_env
require dasllama/dasllama_kqformat
require dasllama/dasllama_gemm_schema
require dasllama/dasllama_kv_dtype
require dasllama/dasllama_kv_codec
require dasllama/dasllama_gpu_math
require dasllama/dasllama_gpu_knee   // the short window's split rule both homes serve, the CUDA knee, the race's pricing
require dasllama/dasllama_gpu_kernels_common
require dasllama/dasllama_gpu_kernels_discrete
require dasllama/dasllama_common   // get_mtp_spec: a carry live for a round's draft head keeps the greedy chain off
require dasllama/dasllama_math   // grow_resize, copy_floats: the chain's staged rope row
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
var private g_cud_dry_arenas : table<int; CudArena>   //! the mint's walk: the arenas as block arithmetic alone, no device memory behind them

var private g_cud_mint_dry_was = false   //! the home's dry flag as the mint found it
var private g_cud_dry_budget = 0l   //! the dry walk's weight budget: the lane identity's cap, not the free memory left beside whatever this process holds

//! MoeGpuMintDryFn's budget: the bytes the dry walk plans against (0 = the live free memory, the served load's)
def cud_mint_dry_budget(bytes : int64) {
    g_cud_dry_budget = bytes
}

def cud_dry_budget : int64 => g_cud.ready && g_cud.dry ? g_cud_dry_budget : 0l

//! MoeGpuMintDryFn: the home's walk dry for the mint's span - every seat runs its accept / refuse arithmetic and allocates and
//! uploads nothing, the arenas counting blocks with no memory behind them; the flag goes back as it was and the dry arenas go
[arch(at="../ARCHITECTURE_IMAGE_VULKAN.md#image-vulkan-mint")]
def cud_mint_dry(on : bool) {
    return if (!g_cud.ready)
    if (on) {
        g_cud_mint_dry_was = g_cud.dry
        g_cud.dry = true
    } else {
        g_cud.dry = g_cud_mint_dry_was
        g_cud_dry_budget = 0l
        g_cud_dry_arenas |> clear()
    }
}

def private cud_arena_reserve(fmt : int; cap_blocks : int64) : bool {
    if (g_cud.dry) {
        let b = dev_block_bytes(kq_fmt_of_slot(fmt))
        g_cud_dry_arenas[fmt] = CudArena(cap_blocks = cap_blocks, qsb = b.qsb, ssb = b.ssb)
        return true
    }
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
    if (!key_exists(g_cud.dry ? g_cud_dry_arenas : g_cud_arenas, fmt)) {
        to_log(LOG_WARNING, "dasLLAMA cuda arena: a plane in format {fmt} with no arena reserved\n")
        return -1l
    }
    let need = kq_blocks_for(n, rows, fmt)
    if (g_cud.dry) {   // the mint's walk: the block cursor alone, the refusal the live arena's
        var d & = unsafe(g_cud_dry_arenas[fmt])
        return -1l if (d.blocks + need > d.cap_blocks)
        let dblk = d.blocks
        d.blocks += need
        return dblk
    }
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

//! every arena's quant and scale bytes on the device (the dry walk's as it would place them)
def cud_arena_bytes : int64 {
    var total = 0l
    for (a in values(g_cud.dry ? g_cud_dry_arenas : g_cud_arenas)) {
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
let SCH_PG = 12       //! the per-layer-embedding gate [dim -> ple]
let SCH_PP = 13       //! ... and its projection [ple -> dim]
let SCH_COUNT = 14

//! one recurrent layer's device state slot (a slot a mirror region): the state and the conv ring pair in the layer's two
//! buffers, the session it holds and the ring image the row reads in `own`
struct CudDnSlot {
    state_dev, smalls_dev : uint64
    slot_state_bytes, ring_image_bytes, smalls_bytes, state_byte_off : int64
    hist_float_off : int64   //! the parity image `ring_image_bytes` past it
    own : DnSlotOwner
}

struct CudLayer {
    hs, qd, kvd, hid, rot, kv_mul : int64
    window, ring, mir_base, mir_slab, cbase : int64
    boff : int64          //! the layer's row base in the [q | k | v] bias plane (`has_bias`)
    pre_post, gelu, vfk, vnorm, kv_shared : bool   //! `vfk`: no v plane, V is the raw k row at its own rms (gemma-4's global layers); `vnorm`: V at its own rms, no weight
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
    egu : bool              //! the one-row step's gate + up dots, act and requant in one dispatch (`Mx4GemvGuT`): mx4 gate and up stacks over a per-32 down stack
    experts_host : bool     //! a routed layer whose experts serve on the host: the router alone runs here, the command cuts after it and resumes on the host's sum
    moe_g4 : bool           //! the gemma-4 routed form: the layer's own FFN triple as the dense branch, the routed feed x under its own pre norm,
                            //! the router over a weightless rms of x under its scale row, the picks' weights under the per-expert down scales, the
                            //! combine under the four norms (`ARCHITECTURE_GPU_VULKAN_MOE.md#vk-gemma4-moe-block`)
    n_router, n_pre2, n_post_ffn1, n_post_ffn2 : int64   //! ... its norm rows in the norms plane (floats): the router scale, the routed pre norm, the dense and the routed post norms
    n_ffn, n_post_att, n_post_ffn, n_obias, n_next : int64   //! the layer's norm rows in the norms plane (floats): the FFN norm, the sandwich norms, the output bias row, the next layer's attention norm (the final norm's on the last layer, the head norm's on the draft head)
    fnext : int             //! the feed format the next layer's head (or the classifier) reads: the residual step after the FFN quantizes into it
    storex_next : bool      //! that residual step keeps the normed row as floats in `xb` beside the quants: the next head reads it, or the carry lands it
    roll_off, snap_off : int64   //! a recurrent layer's byte offset inside a rollback row and a checkpoint row
    ple : bool              //! the layer carries the per-layer-embedding branch (gemma-4 E-series): gelu(gate . x) x the row's side slice -> proj, added under the ple post norm
    bpg, bpp : int64        //! its gate [dim x ple] and projection [ple x dim] arena blocks; -1 = the file's f32 rows (`CudRdec.ple_gf32` / `ple_pf32`)
    fpg, fpp : int
    regs_pg, regs_pp : uint64   //! the two GEMVs' one-record region buffers
    n_ple_post : int64      //! the branch's post norm row in the norms plane (floats)
    ple_fused : bool        //! the branch's act, its requant and the proj GEMV in one dispatch (`Q8GemvPleActT`): both planes Q8_0, the width at most 256
}

//! a routed layer's hot expert pool on the device (`cud_rdec_hot_layer`): `slots` experts' gate, up and down planes in their formats
//! over the hit chain's region geometry (`HotLayerGeom`)
struct CudHot : HotLayerGeom {
    wq1, ws1, wq3, ws3, wq2, ws2 : uint64
}

//! the greedy chain's state (ARCHITECTURE_GPU_VULKAN_LANDING.md#vk-greedy-chain): `ask` the engine's ask for the next step with
//! its rope row `cos_next`; `pend` a chained step in flight at `pos` of `region`, landing in `slot` with its logits where `logits`,
//! fed the running step's pick `pred` (-1 until it landed); `landed` where the last step's results sit (0 = the out block, else
//! landing slot `landed - 1`);
//! `backoff` the adaptive mode's unchained steps left and `backoff_len` its last run
struct CrdChain {
    ask, pend, logits : bool
    @exact_size cos_next : array<float>
    region, pos, pred : int64
    slot, next_slot, landed : int
    backoff, backoff_len : int
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
    n_decline_said : bool   //! the N-row command's decline, said once an armed model
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
    sel_words_host, sel_hdr_host, sel_ids_host, sel_vals_host, sel_rec_host : void?   //! the selection landing's mapped blocks: the rows' ask words the host pokes, the four planes the select kernel writes
    sel_words_dev, sel_hdr_dev, sel_ids_dev, sel_vals_dev, sel_rec_dev : uint64
    sel_ask : bool              //! the next command of `sel_ask_rows` rows lands its selection in place of its logits rows
    sel_ask_rows : int64
    sel_landed : int64          //! the rows the last command's selection covered (0 = its rows or picks alone)
    weight_bytes : int64        //! the plan's weight bytes on the device (`cud_rdec_note_weight_bytes`): the first token's paging bar
    emb_blk : int64             //! the token table's arena block the one-row command gathers off; -1 = the host's row rides the in block, or the f32 table serves
    emb_f32 : uint64            //! an F32 / F16 / BF16 table's exact f32 rows on the device (`cud_rdec_upload_emb_f32`), 0 = none
    ple : int64                 //! the per-layer-embedding width (0 = no such branch); `ple_all` = layers x ple, a row's side slices back to back
    ple_all : int64
    ple_in_host : void?         //! the commands' side rows, mapped: the host's finished rows a row (`ple_in_dev` the device's address), copied into `ple_dev` with the command
    ple_in_dev, ple_dev : uint64
    pleg, pleact : uint64       //! the gate GEMV's rows [nb x ple] and the f32 projection's act rows
    ple_gf32, ple_pf32 : uint64   //! the file's f32 gate and projection rows, where it ships them so (`ple_gf32_bytes` / `ple_pf32_bytes`)
    ple_gf32_bytes, ple_pf32_bytes : int64
    pf_ple, pf_pleg : uint64    //! the window's side rows [np x ple_all] and gate rows [np x ple]
    ple_wp : uint64             //! the pre-step's projection rows on the device (`cud_rdec_set_ple_proj`: bf16 halves under `ple_wp_bf16`, else f32); 0 = the host projects
    ple_wp_bf16 : bool
    ple_pnorm : uint64          //! the projection's norm row [ple]
    ple_w16 : uint64            //! the projection rows as the window's f16 panel (the cm2 f16 tile's weight), written once at the seat; 0 = the window takes the router GEMV
    tok_plep, pf_plep : uint64  //! the projected rows: the commands' [nb x ple_all], the window's [np x ple_all]
    ple_gpu : bool              //! the pre-step's projection and finish ride the commands and the window (`crd_ple_pre`)
    ple_tbl_blk : int64         //! the per-layer table's arena block the commands and the ids prefill gather the side rows off (`cud_rdec_set_ple_table`); -1 = the host gathers
    ple_tbl_fmt : int
    side_gathered : int64       //! the commands' rows whose side row the device gathered off the row's token id (a cell's witness)
    tok_gathered : int64        //! the one-row steps whose row the device gathered off the step's token id (a cell's witness)
    nrow_emb_gathered, nrow_side_gathered : int64   //! the N-row commands' rows whose token row / side row the device gathered off the row's id (a cell's witness)
    emb_fmt : int               //! the table's format: its arena and its gather leaf
    emb_scale : float
    cls_blk : int64
    cls_fmt : int
    cls_cap : float
    nsupp : int64
    graphs : table<int; void?>  //! the captured graphs by command, row count, region and mode (`crd_graph_key`)
    ch : CrdChain               //! the greedy chain (`crd_chain_submit`): the chained step's graph launched behind the running one
    ch_stage_host : array<void?>   //! two staging slots, mapped: the chained step's TokMeta block and rope row, the prologue copies them into the in block
    ch_stage_dev : array<uint64>
    ch_land_host : array<void?>    //! two landing slots, mapped, in the out block's layout: the running step's pick and logits copied aside ahead of the chained step
    ch_land_dev : array<uint64>
    ch_landed, ch_done : array<void?>   //! a slot's events: the running step's results aside, the chained step done
    ch_pro : table<int; void?>     //! the prologue as one graph a (region, slot, logits) key: the copies aside and into the in block, the snapshot
    ch_snap_dev : uint64           //! the region's recurrent states as the chained step found them (a miss puts them back)
    unsplit_on : bool           //! the attention's unsplit form is a form of its own: the device split the keys at all
    unsplit_steps : int64       //! the steps the unsplit form served
    tokens_served : int64   //! the positions the driver served, the window chain's and the token step's
    token_steps : int64     //! the token steps alone: the first runs the chain plainly, the second captures the graph
    windows : int64         //! the window chains run
    seg_windows : int64     //! the windows that held several sessions' chunks as segments
    //! window buffers (crd_pf_setup): np rows; f16 feeds + read slack
    pf_np : int64
    pf_wlen : int64         //! the rows of the window being run (the plan's rows between calls): the MMQ admission reads it
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
    //! the NextN draft head (`cud_rdec_set_head`): one more attention layer at index n_layers, its eh_proj plane, the cat rows' planes, its own token
    //! blocks and rope rows, the carry's landing and the same-slab verify's rollback copies (`ARCHITECTURE_GPU_VULKAN_MTP.md`'s shape)
    head : CudLayer
    head_on : bool           //! the head's planes are in the arena and its draft command runs
    head_planned : bool      //! the driver was prepared with the head: its norms block, its K/V slot and the cat planes
    carry : bool             //! the driver was prepared with the carry: the last layer's residual step keeps the post-norm rows, which land in the out block
    verify_rows : int64      //! the rows a same-slab verify steps at once (0 = none)
    head_norm_base : int64   //! the head's norm block in the norms plane (floats): `RDEC_HEAD_*` rows past it
    head_eh_blk : int64
    head_eh_fmt : int
    head_eh_regs : uint64
    head_tok : uint64        //! the head rows' token blocks (a position below the trunk rows')
    hemb : uint64            //! the head rows' unscaled embed rows [nb x dim]
    hsrc : uint64            //! the head rows' carry rows [nb x dim]: the uploaded carry, then the trunk's post-norm rows
    hn_a, hn_b : uint64      //! the two halves normed: enorm(hemb), hnorm(hsrc)
    head_cat : uint64        //! the cat rows [nb x 2 dim]: eh_proj's input
    out_hid_word : int64     //! the out block's post-norm rows (`dim` floats a row) past the logits
    roll_dev : uint64        //! the verify's rollback rows (one a verify row but the last), every recurrent layer's state and ring pair at its `roll_off`
    roll_row_bytes, roll_rows, roll_region, roll_vrows : int64
    snap_dev : uint64        //! the checkpoint rows, `RDEC_DN_SNAPS` a region, each a whole recurrent state
    snap_row_bytes : int64
    snap_par : array<int>    //! a checkpoint row's ring parity
    drafts, verifies : int64
    verify_landed_logits : bool
    v_said : bool            //! the verify's decline said once a residency
    //! the window chain's head warm (`crd_window_head_warm`): the window's embed rows kept, the shifted carry rows, the cat rows and their feed, the head's own rope rows and schedules
    pf_head_ask, pf_head_declined : bool
    pf_head_rows : int64
    pf_hemb, pf_hsrc, pf_hna, pf_hnb, pf_hcat, pf_hcos : uint64
    pf_hcatf, pf_hcatq, pf_hcats, pf_hcatsum, pf_hcat4q, pf_hcat4s, pf_hcatrs : uint64
    pf_hsch : CudSched[3]    //! the head's eh_proj, k and v schedules
    pf_cap_dev : uint64      //! the window's checkpoint capture records (`PF_CAP_WORDS`)
    //! the host-experts form (`cud_rdec_set_host_experts`): the command splits into `nseg` segments at the layers whose experts the host
    //! sums - a segment ends by landing the FFN-mixed rows and the router logits in the mapped blocks, the next opens on the host's sums
    //! (`ARCHITECTURE_GPU_VULKAN_HC.md#hc-token-command`, the plain cut) - and the hot expert pool beside it (`ARCHITECTURE_GPU_VULKAN_POOL.md#hc-hot-pool`)
    experts_host : bool
    nseg : int64
    seg_layer : array<int64>     //! the host layers in layer order, then -1 for the last segment
    hx_host, hlog_host, hacc_host : void?   //! the rows the host reads and writes: [nb x dim] mixed rows, [nb x ner] logits, [nb x dim] sums
    hx_dev, hlog_dev, hacc_dev : uint64
    ones : uint64                //! `RDEC_ROUTED_SLOTS` floats of one: the host's sum as the one routed row of weight one
    hot_slots : int64
    hot : array<CudHot>          //! a layer's pool, or off
    hot_meta1_host, hot_meta3_host, hot_meta2_host, hot_w_host : void?   //! the hit chain's region records and weights, the host writes them a step
    hot_meta1_dev, hot_meta3_dev, hot_meta2_dev, hot_w_dev : uint64
    hot_ident1 : uint64          //! the ramp 1 .. RDEC_ROUTED_SLOTS: the combine's slot window over the hit rows
    hot_hits, hot_submits : int64
    pf_hx_host, pf_hlog_host, pf_hacc_host : void?   //! the window's cut: [np x dim] mixed rows, [np x ner] logits, [np x dim] sums
    pf_hx_dev, pf_hlog_dev, pf_hacc_dev : uint64
    pf_ones : uint64             //! np floats of one
    pf_ident : uint64            //! the ramp 0 .. np: the window's one slot a position under the host form
}

var g_crd : CudRdec?
var private g_crd_served_total = 0l   //! positions the driver served in this process, across model drops (a cell's witness after its model is gone)

//! the positions a seat served: the driver's count, and the process's, which a model drop leaves standing
def private crd_served(var r : CudRdec; n : int64) {
    r.tokens_served += n
    g_crd_served_total += n
}

//! positions the CUDA driver served in this process, every model counted: a compare reads it before and after its device rows
def cud_rdec_served_total : int64 => g_crd_served_total

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
                r.pf_mxf, r.pf_mx4q, r.pf_mx4s, r.pf_mxrs, r.pf_mh4q, r.pf_mh4s, r.pf_mhrs, r.pf_mmy1, r.pf_mmy2, r.pf_mmdn,
                r.head_eh_regs, r.head_tok, r.hemb, r.hsrc, r.hn_a, r.hn_b, r.head_cat, r.roll_dev, r.snap_dev,
                r.pf_hemb, r.pf_hsrc, r.pf_hna, r.pf_hnb, r.pf_hcat, r.pf_hcos, r.pf_hcatf, r.pf_hcatq, r.pf_hcats, r.pf_hcatsum,
                r.pf_hcat4q, r.pf_hcat4s, r.pf_hcatrs, r.pf_cap_dev, r.ones, r.hot_ident1, r.pf_ones, r.pf_ident, r.emb_f32,
                r.ple_dev, r.pleg, r.pleact, r.ple_gf32, r.ple_pf32, r.pf_ple, r.pf_pleg, r.ple_wp, r.ple_pnorm, r.ple_w16, r.tok_plep, r.pf_plep]) {
        cuda_mem_free(b)
    }
    for (H in r.hot) {
        for (b in [H.wq1, H.ws1, H.wq3, H.ws3, H.wq2, H.ws2]) {
            cuda_mem_free(b)
        }
    }
    for (hb in [r.hx_host, r.hlog_host, r.hacc_host, r.hot_meta1_host, r.hot_meta3_host, r.hot_meta2_host, r.hot_w_host, r.pf_hx_host, r.pf_hlog_host, r.pf_hacc_host,
                r.sel_words_host, r.sel_hdr_host, r.sel_ids_host, r.sel_vals_host, r.sel_rec_host, r.ple_in_host]) {
        cuda_mem_free_host(hb)
    }
    for (sc in r.pf_hsch) {
        cuda_mem_free(sc.buf)
    }
    crd_layer_free(r.head)
    for (b in r.k_slabs) {
        cuda_mem_free(b)
    }
    for (b in r.v_slabs) {
        cuda_mem_free(b)
    }
    for (L in r.layers) {
        crd_layer_free(L)
    }
    cuda_mem_free_host(r.in_host)
    cuda_mem_free_host(r.out_host)
    for (hb in r.ch_stage_host) {
        cuda_mem_free_host(hb)
    }
    for (hb in r.ch_land_host) {
        cuda_mem_free_host(hb)
    }
    for (e in r.ch_landed) {
        cuda_event_destroy(e)
    }
    for (e in r.ch_done) {
        cuda_event_destroy(e)
    }
    cuda_mem_free(r.ch_snap_dev)
    crd_graphs_drop(r)
}

def private crd_layer_free(L : CudLayer) {
    for (b in [L.regs_qkv, L.regs_k, L.regs_v, L.regs_o, L.regs_gate, L.regs_up, L.regs_gu, L.regs_down,
               L.regs_dnqkv, L.regs_dnz, L.regs_dnqkvz, L.regs_dnout, L.regs_dnbeta, L.regs_dnalpha, L.regs_pg, L.regs_pp]) {
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

//! a step replayed as a graph in any mode
def private crd_graph_captured(r : CudRdec) : bool => !empty(r.graphs)

def private crd_graphs_drop(var r : CudRdec) {
    for (g in values(r.graphs)) {
        cuda_graph_destroy(g)
    }
    r.graphs |> clear()
    for (g in values(r.ch_pro)) {
        cuda_graph_destroy(g)
    }
    r.ch_pro |> clear()
}

let private CRD_NB = RDEC_NROW_MAX                      //! the rows the N-row command steps at once: the per-row planes' height
let private CRD_TOK_WORDS = int(TOK_META_BYTES / 4l)   //! a TokMeta block in words
let private CRD_IN_ID_WORD = CRD_TOK_WORDS * int(CRD_NB)   //! the in block: CRD_NB TokMeta blocks, the rows' token ids from this word, the head rows' token blocks, the rope rows from the header's end, then the x rows
let private CRD_IN_HTOK_WORD = CRD_IN_ID_WORD + int(CRD_NB)   //! the head rows' TokMeta blocks (a position below the trunk rows')
let private CRD_IN_HDR_WORDS = CRD_IN_HTOK_WORD + CRD_TOK_WORDS * int(CRD_NB)
let private CRD_OUT_LOGITS_WORDS = int(CRD_NB)          //! the out block: a pick a row, then the rows' logits from this word, then the post-norm rows (`CudRdec.out_hid_word`)
let private PF_CAP_WORDS = 64l                          //! a window's checkpoint capture records: two words a capture
//! the in block's head section past the token rows: the head rope rows [nb x cos], then the carry rows [nb x dim], then the embed rows [nb x dim]
def private crd_in_hcos_word(r : CudRdec) : int64 => int64(CRD_IN_HDR_WORDS) + (r.cos_elems + r.dim) * r.nb
def private crd_in_hrow_word(r : CudRdec) : int64 => crd_in_hcos_word(r) + r.cos_elems * r.nb
def private crd_in_hemb_word(r : CudRdec) : int64 => crd_in_hrow_word(r) + r.dim * r.nb
def private crd_in_words(r : CudRdec) : int64 => crd_in_hemb_word(r) + r.dim * r.nb

//! the driver's state dropped: its buffers, the mirror slabs, the graph; the arenas stay (the reserve seat precedes prepare)
def private crd_drop {
    return if (g_crd == null)
    crd_chain_settle()
    cud_sync()
    crd_free_buffers(*g_crd)
    unsafe {
        delete g_crd
    }
    g_crd = null
}

var private g_cud_drop_hooks : array<function<() : void>>   //! the home's dependants' drop hooks (the gemma drafter): run at the model drop ahead of the driver's state

def register_cud_drop_hook(fn : function<() : void>) {
    g_cud_drop_hooks |> push(fn)
}

//! the model's device state dropped: the dependants' hooks, the stream chain's, the driver's state and the arenas; the device stays up for the next model
def cud_drop_model {
    cud_bind()
    for (fn in g_cud_drop_hooks) {
        invoke(fn)
    }
    cud_stream_drop()
    crd_drop()
    cud_arena_free()
}

//! the carry armed past prepare (the assistant drafter's attach): the last layer's residual step keeps its post-norm row and the
//! commands land it in the out block, which holds the rows from prepare; the captured graphs go, the next step re-captures with the landing
def cud_rdec_carry_arm(on : bool) : bool {
    crd_chain_settle()
    return false if (g_crd == null || !g_crd.ready)
    var r & = unsafe(*g_crd)
    return true if (r.carry == on)
    r.carry = on
    var L & = unsafe(r.layers[r.n_layers - 1l])
    L.storex_next = on
    L.down_ar = !L.moe && L.f2 == int(KqFmt.q8) && !kq_sb(L.fnext) && !on
    crd_graphs_drop(r)
    return true
}

//! The commands gather their token rows on the device (a placed table, or the f32 table): the drafter's seed rows gather there too
def cud_rdec_emb_on : bool => g_crd != null && crd_emb_on(*g_crd)

//! `npos` rows gathered off the table the driver holds at the ids in `ids` into `y` (the drafter's seed rows), on stream `s`
def cud_rdec_emb_gather(var s : void?; ids, y : uint64; npos : int64; escale : float) {
    crd_chain_settle()
    assert(g_crd != null, "cud_rdec_emb_gather before prepare")
    crd_emb_gather(*g_crd, s, ids, y, y, npos, escale)
}

//! CxfPoolPlanesFn: layer `l`'s pool planes in plane order (gate, up, down), off where the layer has no pool
def crd_xf_pool_planes(l : int64) : CxfPool {
    return CxfPool() if (g_crd == null || l >= long_length(g_crd.hot) || !g_crd.hot[l].on)
    let H & = unsafe(g_crd.hot[l])
    return CxfPool(on = true, wq = fixed_array(H.wq1, H.wq3, H.wq2), ws = fixed_array(H.ws1, H.ws3, H.ws2), blk = fixed_array(H.blk1, H.blk3, H.blk2), slots = g_crd.hot_slots)
}

//! CxfBiasPlanesFn: the routed experts' gate and up bias planes (`cud_rdec_prepare_moe` uploads them), 0 on a model with none
def crd_xf_bias_planes : tuple<gb : uint64; ub : uint64> =>
    g_crd != null && g_crd.exps_bias ? (gb = g_crd.moe_gb, ub = g_crd.moe_ub) : (gb = 0ul, ub = 0ul)

var private g_crd_hot_batch = false   //! a placement run is open: its uploads land under one wait at the close

//! RdecHotBatchFn: the pool's placement run lands under one wait (a pageable source is staged by the call itself, so the
//! caller's gather planes are free as soon as each upload returns)
def cud_rdec_hot_batch(open : bool) {
    cud_bind()
    if (!open && g_crd_hot_batch) {
        verify(cud_sync(), "cud decode: a pool placement run did not land")
    }
    g_crd_hot_batch = open
}

def cud_resident_bytes : int64 => cud_arena_bytes() + (g_crd != null && g_crd.ready ? 2l * long_length(g_crd.k_slabs) * g_crd.regions * g_crd.mir_units * g_crd.unit_bytes : 0l)

def private cud_decline(why : string) : bool {
    to_log(LOG_WARNING, "dasLLAMA cuda resident: {why} - the driver declines\n")
    return false
}

//! the formats the driver's arenas and kernels serve (`expert_chain_slot_ok`): Q8_0, the superblock K-quants and NVFP4, the dense per-32 ones and MXFP4 expert stacks
def cud_serves_fmt(fmt : int) : bool => expert_chain_slot_ok(fmt)

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
    if (regions < 1l) return cud_decline("{regions} mirror regions")
    if (dim > RDEC_AR_MAX_DIM || head_size > 512l || kv_dim > RDEC_AR_MAX_DIM) return cud_decline("dim {dim}, kv_dim {kv_dim} or head_size {head_size} past the row stashes")
    if (verify_rows > CRD_NB) return cud_decline("{verify_rows} verify rows: the per-row planes hold {CRD_NB}")
    return true if (g_cud.dry)   // the mint's walk: the shape admitted, no driver state made
    let refused <- cud_ensure_all()   // every kernel of the home loads before the first chain: a launch precedes no ensure, and a capture loads nothing
    if (!empty(refused)) return cud_decline("{length(refused)} kernels did not compile ({refused[0]} first)")
    if (cud_kq_grid() == 0ul) return cud_decline("the codebook grid did not upload: {cuda_last_error()}")   // a capture allocates nothing: the kq leaves' seventh plane lands before the first chain
    crd_drop()
    g_crd = new CudRdec(n_layers = n_layers, dim = dim, qd = qd, kv_dim = kv_dim, n_heads = n_heads, hidden = hidden, vocab = vocab,
        neox = neox, qk_norm = qk_norm, q_gated = q_gated, head_size = head_size, eps = eps, scale = scale, kv_dt = dt, regions = regions, cos_elems = cos_elems,
        head_planned = head, carry = carry, verify_rows = head ? verify_rows : 0l, roll_region = -1l)
    var r & = unsafe(*g_crd)
    r.unit_bytes = kv_unit_bytes(dt)
    r.mir_units = mir_units + kv_row_units(dt, RDEC_MIR_SLACK)
    r.nb = CRD_NB
    let nb = r.nb
    let wide = max(max(max(qd, hidden), dim), head ? 2l * dim : 0l)   // the head's eh_proj reads the [enorm ; hnorm] pair through the feed planes
    if (head) {
        crd_head_planes(r)
    }
    r.x = cud_dev_zeroed(dim * 4l * nb)
    r.xq = cud_dev_zeroed(wide * nb)
    r.xs = cud_dev_zeroed((wide / Q8_BLOCK_ELEMS) * 4l * nb)
    r.proj = cud_dev_zeroed((qd + 2l * kv_dim) * 4l * nb)
    if (q_gated) {
        r.q = cud_dev_zeroed(2l * qd * 4l * nb)
    }
    r.hybrid = hybrid
    if (hybrid) {
        r.dn_cd = dn.cd; r.dn_kd = dn.kd; r.dn_di = dn.di; r.dn_nvh = dn.nvh; r.dn_nkh = dn.nkh; r.dn_ds = dn.ds; r.dn_dconv = dn.dconv
        r.dn_ba_f32 = dn.ba_f32
        r.dn_zsig = dn.zsig
        r.dn_proj = cud_dev_zeroed((dn.cd + dn.di) * 4l * nb)
        r.dn_oq = cud_dev_zeroed(DND_OQ_BYTES * nb)
        r.dn_os = cud_dev_zeroed(DND_OS_BYTES * nb)
        r.dn_of = cud_dev_zeroed(DND_OQ_BYTES * 4l * nb)
        r.dn_parity |> resize(regions)
        r.pf_dn_cur |> resize(regions)
    }
    r.attn = cud_dev_zeroed(qd * 4l * nb)
    r.attn_nsplit = da_nsplit(n_heads, int64(g_cud.sm_count))
    r.unsplit_on = r.attn_nsplit > 1l
    r.attn_cnt_off = n_heads * max(r.attn_nsplit, 1l) * (head_size + 2l) * nb   // the pieces of every row; a counter a (row, kv head, slab) group past them
    r.attn_part = cud_dev_zeroed((r.attn_cnt_off + n_heads * nb) * 4l)
    r.ar_cnt = cud_dev_zeroed(16l)
    r.aq = cud_dev_zeroed(qd * nb)
    r.as_ = cud_dev_zeroed((qd / Q8_BLOCK_ELEMS) * 4l * nb)
    r.hq = cud_dev_zeroed(hidden * nb)
    r.hs_ = cud_dev_zeroed((hidden / Q8_BLOCK_ELEMS) * 4l * nb)
    r.gate = cud_dev_zeroed(hidden * 4l * max(nb, 2l))   // the merged gate-up GEMV lands both rows here at one row
    r.up = cud_dev_zeroed(hidden * 4l * nb)
    r.ffnout = cud_dev_zeroed(dim * 4l * nb)
    r.xb2 = cud_dev_zeroed(dim * 4l * nb)
    r.xb = cud_dev_zeroed(dim * 4l * nb)
    crd_codec_planes(r, seq_cap)
    r.cos = cud_dev_zeroed(cos_elems * 4l * nb)
    r.logits = cud_dev_zeroed(vocab * 4l * nb)
    r.pick_part_max = cud_dev_zeroed(4l * int64(CLS_ARGMAX_CHUNKS) * nb)
    r.pick_part_id = cud_dev_zeroed(4l * int64(CLS_ARGMAX_CHUNKS) * nb)
    r.tok = cud_dev_zeroed(int64(CRD_IN_HDR_WORDS) * 4l)   // the in block's header as the kernels read it: the rows' TokMeta blocks, then the ids the gather reads
    r.dummy = cud_dev_zeroed(256l)
    r.ar_scal_off = rdec_norms_len(n_layers, dim, qk_norm ? head_size : 0l, head)
    r.norms_bytes = (r.ar_scal_off + 2l) * 4l
    r.norms = cud_dev_zeroed(r.norms_bytes)
    let slab_bytes = regions * r.mir_units * r.unit_bytes
    r.k_slabs |> reserve(max(n_slabs, 1l))
    r.v_slabs |> reserve(max(n_slabs, 1l))
    for (_si in range64(max(n_slabs, 1l))) {
        let k = cud_dev_zeroed(slab_bytes)
        let v = cud_dev_zeroed(slab_bytes)
        r.k_slabs |> push(k)
        r.v_slabs |> push(v)
        if (k == 0ul || v == 0ul) {
            return cud_decline("the K/V mirror ({(2l * slab_bytes * max(n_slabs, 1l)) >> 20l} MB in {max(n_slabs, 1l)} slab pairs at ctx {seq_cap}) found no room on the device")
        }
    }
    r.out_hid_word = int64(CRD_OUT_LOGITS_WORDS) + vocab * nb
    let inb = cuda_mem_alloc_host_mapped(uint64(crd_in_words(r) * 4l))
    let outb = cuda_mem_alloc_host_mapped(uint64((r.out_hid_word + dim * nb) * 4l))   // the post-norm rows' slots sit past the logits whether or not the carry is on: an attach arms it later (`cud_rdec_carry_arm`)
    r.in_host = inb.host
    r.in_dev = inb.dev
    r.out_host = outb.host
    r.pick = outb.dev
    if (r.in_host == null || r.out_host == null) return cud_decline("the step's mapped host blocks did not allocate ({cuda_last_error()})")
    let selw = cuda_mem_alloc_host_mapped(uint64(SEL_WORDS * 4l * nb))
    let selh = cuda_mem_alloc_host_mapped(uint64(SEL_HDR * 4l * nb))
    let seli = cuda_mem_alloc_host_mapped(uint64(SEL_CAP * 4l * nb))
    let selv = cuda_mem_alloc_host_mapped(uint64(SEL_CAP * 4l * nb))
    let selr = cuda_mem_alloc_host_mapped(uint64(SEL_RECENT * 4l * nb))
    r.sel_words_host = selw.host
    r.sel_words_dev = selw.dev
    r.sel_hdr_host = selh.host
    r.sel_hdr_dev = selh.dev
    r.sel_ids_host = seli.host
    r.sel_ids_dev = seli.dev
    r.sel_vals_host = selv.host
    r.sel_vals_dev = selv.dev
    r.sel_rec_host = selr.host
    r.sel_rec_dev = selr.dev
    if (r.sel_words_host == null || r.sel_hdr_host == null || r.sel_ids_host == null || r.sel_vals_host == null || r.sel_rec_host == null) {
        return cud_decline("the selection landing's mapped blocks did not allocate ({cuda_last_error()})")
    }
    r.emb_blk = -1l
    r.ple_tbl_blk = -1l
    r.layers |> reserve(n_layers)
    r.layers |> resize(n_layers)
    return true
}

//! the norms plane and the eps / scale pair past its rows, re-sized here: the prepare cannot know whether the output-bias rows come
def cud_rdec_upload_norms(norms : array<float>; obias : bool) {
    return if (g_cud.dry)
    assert(g_crd != null, "cud_rdec_upload_norms before prepare")
    g_crd.has_obias = obias
    if (long_length(norms) != g_crd.ar_scal_off) {
        cuda_mem_free(g_crd.norms)
        g_crd.ar_scal_off = long_length(norms)
        g_crd.norms_bytes = (g_crd.ar_scal_off + 2l) * 4l
        g_crd.norms = cud_dev_zeroed(g_crd.norms_bytes)
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
    return if (g_cud.dry)
    assert(g_crd != null, "cud_rdec_upload_bias before prepare")
    cuda_mem_free(g_crd.bias)
    g_crd.bias = buf_upload(rows, g_cud.stream)
    g_crd.has_bias = true
}

//! the per-head sink logits, a layer's row of `n_heads` (`install_moe_gpu_resident_sinks`'s seat); declines off head size 64, the one flash sink shape
def cud_rdec_upload_sinks(rows : array<float>) : bool {
    return true if (g_cud.dry)
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
    return if (g_cud.dry)
    assert(g_crd != null, "cud_rdec_set_layer before prepare")
    var r & = unsafe(*g_crd)
    crd_layer_set(r, r.layers[l], bq, bk, bv, bo, b1, b3, b2, fq, fk, fv, fo, f1, f3, f2, g)
}

//! layer `l` of the chain, or the draft head at index n_layers: the K/V row seats and a routed head's experts name the head so
def private crd_layer_at(var r : CudRdec; l : int64) : CudLayer& {
    unsafe {
        if (l == r.n_layers) {
            verify(r.head_planned, "cud decode: the draft head's layer index named on a driver prepared without the head")
            return r.head
        }
        return r.layers[l]
    }
}

//! an attention layer's planes, regions and fuses: the trunk's layers and the draft head take one setter
def private crd_layer_set(var r : CudRdec; var L : CudLayer; bq, bk, bv, bo, b1, b3, b2 : int64; fq, fk, fv, fo, f1, f3, f2 : int; g : RdecLayerGeom) {
    L.hs = g.hs; L.qd = g.qd; L.kvd = g.kvd; L.hid = g.hid; L.rot = g.rot
    L.kv_mul = g.kvd > 0l ? r.n_heads / (g.kvd / g.hs) : 1l
    L.pre_post = g.pre_post; L.gelu = g.gelu; L.vfk = g.vfk; L.vnorm = g.vnorm; L.kv_shared = g.kv_shared
    L.out_scale = g.out_scale; L.attn_cap = g.attn_cap
    L.window = g.window; L.ring = g.ring; L.mir_base = g.mir_base; L.mir_slab = g.mir_slab; L.cbase = g.cbase; L.boff = g.boff
    L.bq = bq; L.bk = bk; L.bv = bv; L.bo = bo; L.b1 = b1; L.b3 = b3; L.b2 = b2
    L.fq = fq; L.fk = fk; L.fv = fv; L.fo = fo; L.f1 = f1; L.f3 = f3; L.f2 = f2
    verify(cud_serves_fmt(fq) && cud_serves_fmt(fk) && (L.vfk || cud_serves_fmt(fv)) && cud_serves_fmt(fo) && cud_serves_fmt(f1) && cud_serves_fmt(f3) && cud_serves_fmt(f2),
        "cud decode: a plane in a format the driver does not serve reached set_layer - the place seat admits the arena formats alone")
    L.wo_ar = fo == int(KqFmt.q8) && !kq_sb(f1) && kq_sb(f1) == kq_sb(f3)   // the fused epilogue requants into the gate's feed alone and keeps no normed row
    L.qkv_merged = !r.q_gated && !L.vfk && !L.kv_shared && L.qd % L.kvd == 0l && fq == fk && fk == fv   // one GEMV over the three planes needs them in one arena
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
        L.regs_k = L.kv_shared ? 0ul : cud_regions([uint(bk), 0u])   // a shared-KV layer projects q alone: its donor's rows sit in the mirror
        L.regs_v = (L.vfk || L.kv_shared) ? 0ul : cud_regions([uint(bv), 0u])
        L.nreg_qkv = 1l
    }
    crd_graphs_drop(r)   // the chain references the replaced layer
}

//! the NextN draft head's arm (`RdecSetHeadFn`): the head as one more attention layer with the trunk's shape, its eh_proj plane
//! `beh` in format `feh`, its norm rows the head block's, its K/V slot the geometry's; a hybrid's rollback rows sized with it
[arch(at="../ARCHITECTURE_GPU_VULKAN_MTP.md#resident-draft-head")]
def cud_rdec_set_head(beh : int64; feh : int; bq, bk, bv, bo, b1, b3, b2 : int64; fq, fk, fv, fo, f1, f3, f2 : int; g : RdecLayerGeom) : bool {
    return true if (g_cud.dry)
    assert(g_crd != null && g_crd.head_planned, "cud_rdec_set_head on a driver prepared without the head")
    var r & = unsafe(*g_crd)
    if (!cud_serves_fmt(feh)) {
        to_log(LOG_WARNING, "dasLLAMA cuda resident: the NextN head's eh_proj format {feh} is outside the arena formats - the CPU draft serves\n")
        return false
    }
    crd_layer_set(r, r.head, bq, bk, bv, bo, b1, b3, b2, fq, fk, fv, fo, f1, f3, f2, g)
    r.head_eh_blk = beh
    r.head_eh_fmt = feh
    cuda_mem_free(r.head_eh_regs)
    r.head_eh_regs = cud_regions([uint(beh), 0u])
    let hb = r.head_norm_base
    let dim = r.dim
    var L & = unsafe(r.head)
    L.n_ffn = hb + RDEC_HEAD_FFN * dim
    L.n_post_att = 0l
    L.n_post_ffn = 0l
    L.n_obias = 0l
    L.n_next = hb + RDEC_HEAD_OUT * dim
    L.fnext = r.cls_fmt
    L.storex_next = true   // the head norm's row is the draft's hidden: it lands as the carry
    L.down_ar = false
    L.wo_ar = L.wo_ar && !r.has_obias
    r.head_on = true
    if (r.hybrid) {
        crd_roll_alloc(r)
    }
    crd_graphs_drop(r)
    return true
}

//! a block-codec mirror's planes at the prepare: the staged K and V rows and their token blocks, the f16 shadow, tq4's sign row
def private crd_codec_planes(var r : CudRdec; seq_cap : int64) {
    let nb = r.nb
    let kv_dim = r.kv_dim
    if (kv_block_codec(r.kv_dt)) {
        r.kst = cud_dev_zeroed(kv_dim * 4l * nb)
        r.vst = cud_dev_zeroed(kv_dim * 4l * nb)
        var stage : array<TokMeta>   // row i of the staged planes: position i in region 0 (its key count the rows to it), so the rope stamp lands each row at its own staged row
        stage |> resize(nb)
        for (i in range64(nb)) {
            stage[i] = TokMeta(pos = uint(i), cnt = uint(i + 1l))
        }
        r.stage_tok = buf_upload(stage, g_cud.stream)
        r.ksh = cud_dev_zeroed((seq_cap * kv_dim + RDEC_MIR_SLACK) * 2l)
        r.vsh = cud_dev_zeroed((seq_cap * kv_dim + RDEC_MIR_SLACK) * 2l)
    }
    if (r.kv_dt == KVDtype.tq4) {
        var signs : array<float>
        tq4_fill_signs(signs, KV_TQ4_SIGNS)
        r.tq4_signs = buf_upload(signs, g_cud.stream)
    }
}

//! the head's planes at the prepare: its norm block's base, its token blocks, the embed and carry rows, the two normed halves, the cat rows
def private crd_head_planes(var r : CudRdec) {
    let dim = r.dim
    let nb = r.nb
    r.head_norm_base = rdec_head_norm_base(r.n_layers, dim, r.qk_norm ? r.head_size : 0l)
    r.head_tok = cud_dev_zeroed(int64(CRD_TOK_WORDS) * 4l * nb)
    r.hemb = cud_dev_zeroed(dim * 4l * nb)
    r.hsrc = cud_dev_zeroed(dim * 4l * nb)
    r.hn_a = cud_dev_zeroed(dim * 4l * nb)
    r.hn_b = cud_dev_zeroed(dim * 4l * nb)
    r.head_cat = cud_dev_zeroed(2l * dim * 4l * nb)
}

//! the rollback rows: `verify_rows - 1` rows, each every recurrent layer's state slot and ring pair at the layer's `roll_off`
[arch(at="../ARCHITECTURE_GPU_VULKAN_MTP.md#resident-verify-rollback")]
def private crd_roll_alloc(var r : CudRdec) {
    var off = 0l
    for (L in r.layers) {
        continue if (!L.recurrent)
        L.roll_off = off
        off += L.dn[0].slot_state_bytes + 2l * L.dn[0].ring_image_bytes
    }
    r.roll_row_bytes = off
    r.roll_rows = max(r.verify_rows - 1l, 0l)
    r.roll_region = -1l
    cuda_mem_free(r.roll_dev)
    r.roll_dev = 0ul
    if (r.roll_rows > 0l && off > 0l) {
        r.roll_dev = cud_dev_zeroed(off * r.roll_rows)
    }
}

//! the checkpoint rows, `RDEC_DN_SNAPS` a region: every recurrent layer's state slot and ring pair at the layer's `snap_off`
[arch(at="../ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#device-home-checkpoints")]
def private crd_snap_alloc(var r : CudRdec) {
    return if (r.snap_dev != 0ul)
    var off = 0l
    for (L in r.layers) {
        continue if (!L.recurrent)
        L.snap_off = off
        off += L.dn[0].slot_state_bytes + 2l * L.dn[0].ring_image_bytes
    }
    r.snap_row_bytes = off
    let rows = r.regions * int64(RDEC_DN_SNAPS)
    r.snap_par |> resize(rows)
    if (off > 0l) {
        r.snap_dev = cud_dev_zeroed(off * rows)
    }
}

//! some routed layers' experts serve on the host (`RdecSetHostExpertsFn`): set after the prepare, before the MoE prepare (which
//! then makes the cut's landings) and the layers' registration (a host layer: `be1 < 0`)
[arch(at="../ARCHITECTURE_GPU_VULKAN_HC.md#hc-token-command")]
def cud_rdec_set_host_experts(on : bool) {
    if (g_crd != null) {
        g_crd.experts_host = on
    }
}

//! the routed block's admission: an expert stack in any arena format (the cm2 tiles and the GEMV leaves serve them all)
def cud_rdec_moe_ok(f1, f3, f2 : int) : bool => cud_serves_fmt(f1) && cud_serves_fmt(f3) && cud_serves_fmt(f2)

def private cud_ramp(n : int64) : uint64 {
    var words <- [for (i in range64(n)); uint(i)]
    return buf_upload(words, g_cud.stream)
}

def private cud_rows_or_zero(rows : array<float>; zero_elems : int64) : uint64 => empty(rows) ? cud_dev_zeroed(zero_elems * 4l) : buf_upload(rows, g_cud.stream)

//! the routed block's planes and token-step slot buffers, once a residency (the window's land in `crd_pf_setup`)
[arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#resident-moe-token")]
def cud_rdec_prepare_moe(ne, k, nfe, nsh, n_moe : int64; sh_gated, norm_topk, is_gelu, act_oai : bool; wscale : float; router : array<float>; dscale : array<float>;
                         rbias : array<float>; gbias : array<float>; ubias : array<float>; dbias : array<float>) : bool {
    return true if (g_cud.dry)
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
    assert(empty(dscale) || long_length(dscale) == n_moe * ne, "cud_rdec_prepare_moe: the down-scale plane is not [n_moe x ne]")
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
    r.moe_ds = empty(dscale) ? cud_dev_zeroed(256l) : buf_upload(dscale, g_cud.stream)   // the gemma-4 per-expert down scales, [n_moe x ne]; else a zero row the top-k binds and never reads
    r.moe_ident = cud_ramp(RDEC_ROUTED_SLOTS)
    r.moe_meta1 = cud_dev_zeroed(RDEC_ROUTED_SLOTS * 8l)
    r.moe_meta3 = cud_dev_zeroed(RDEC_ROUTED_SLOTS * 8l)
    r.moe_meta2 = cud_dev_zeroed(RDEC_ROUTED_SLOTS * 8l)
    r.moe_w = cud_dev_zeroed(RDEC_ROUTED_SLOTS * 4l)
    r.moe_eidx = cud_dev_zeroed(RDEC_ROUTED_SLOTS * 4l)
    r.moe_dlog = cud_dev_zeroed(ner * 4l * nb)
    r.moe_xq = cud_dev_zeroed(dim * nb)
    r.moe_xs = cud_dev_zeroed((dim / Q8_BLOCK_ELEMS) * 4l * nb)
    r.egate = cud_dev_zeroed(nb * k * nfe * 4l)
    r.eup = cud_dev_zeroed(nb * k * nfe * 4l)
    r.ehq = cud_dev_zeroed(nb * k * nfe)
    r.ehs = cud_dev_zeroed((nb * k * nfe / Q8_BLOCK_ELEMS) * 4l)
    r.edown = cud_dev_zeroed(nb * k * dim * 4l)
    if (r.experts_host) {
        let hx = cuda_mem_alloc_host_mapped(uint64(dim * 4l * nb))
        let hl = cuda_mem_alloc_host_mapped(uint64(ner * 4l * nb))
        let ha = cuda_mem_alloc_host_mapped(uint64(dim * 4l * nb))
        r.hx_host = hx.host; r.hx_dev = hx.dev
        r.hlog_host = hl.host; r.hlog_dev = hl.dev
        r.hacc_host = ha.host; r.hacc_dev = ha.dev
        if (r.hx_host == null || r.hlog_host == null || r.hacc_host == null) return cud_decline("the host cut's mapped blocks did not allocate ({cuda_last_error()})")
        var ones <- [for (_i in range64(RDEC_ROUTED_SLOTS)); 1.0]
        r.ones = buf_upload(ones, g_cud.stream)
    }
    return true
}

//! layer `l`'s expert stacks on a built layer (`install_moe_gpu_resident_moe`'s experts seat): expert 0's arena block a stack, the formats, the router slot
def cud_rdec_set_moe_experts(l, router_slot, be1, be3, be2 : int64; fe1, fe3, fe2 : int) {
    return if (g_cud.dry)
    assert(g_crd != null && g_crd.moe, "cud_rdec_set_moe_experts before prepare_moe")
    var r & = unsafe(*g_crd)
    var L & = crd_layer_at(r, l)
    L.moe = true
    L.router_slot = router_slot
    L.wo_ar = false   // the wo epilogue's fused requant keeps no normed row, which the router reads
    if (be1 < 0l) {
        verify(r.experts_host, "cud decode: a routed layer's experts off the device on a driver with no host-experts form")
        L.experts_host = true
        L.fe1 = fe1; L.fe3 = fe3; L.fe2 = fe2
        crd_graphs_drop(r)
        return
    }
    verify(be3 >= 0l && be2 >= 0l, "cud decode: a routed layer's expert stacks placed in part")
    verify(cud_serves_fmt(fe1) && cud_serves_fmt(fe3) && cud_serves_fmt(fe2), "cud decode: an expert stack in a format the driver does not serve reached set_moe_experts - the moe_ok seat admits the arena formats alone")
    L.be1 = be1; L.be3 = be3; L.be2 = be2
    L.fe1 = fe1; L.fe3 = fe3; L.fe2 = fe2
    L.xe_own = L.b1 >= 0l && kq_sb(fe1) != kq_sb(L.f1)
    L.egu = fe1 == int(KqFmt.mx4) && fe3 == int(KqFmt.mx4) && !kq_sb(fe2) && r.nfe % Q8_BLOCK_ELEMS == 0l && ensure_mx4_gemv_gu_cls()
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
    return if (g_cud.dry)
    assert(g_crd != null && g_crd.moe, "cud_rdec_set_moe_layer before prepare_moe")
    cud_rdec_set_layer(l, mir_idx, bq, bk, bv, bo, b1, b3, b2, fq, fk, fv, fo, f1, f3, f2, g)
    cud_rdec_set_moe_experts(l, router_slot, be1, be3, be2, fe1, fe3, fe2)
    var L & = unsafe(g_crd.layers[l])
    L.moe_g4 = g.moe_g4
    if (g.moe_g4) {   // the routed feed is x under the routed pre norm, never the dense branch's normed row
        verify(b1 >= 0l, "cud decode: a gemma-4 routed layer with no dense triple")
        L.xe_own = true
    }
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
def private crd_storex_head(L : CudLayer) : bool => L.recurrent || crd_head_mixed(L)

[arch(at="../ARCHITECTURE_GPU.md#gpu-backends")]
def private crd_head_mixed(L : CudLayer) : bool => L.recurrent ? kq_sb(L.fz) != kq_sb(L.fqkv) : (kq_sb(L.fk) != kq_sb(L.fq) || (!L.vfk && kq_sb(L.fv) != kq_sb(L.fq)))

[arch(at="../ARCHITECTURE_GPU.md#gpu-backends")]
def private crd_gu_mixed(L : CudLayer) : bool => kq_sb(L.f1) != kq_sb(L.f3)

//! the down epilogue requants into the next layer's feed, so it fuses only where that consumer reads Q8_0 and stores no normed row
//! every layer's norm rows, its next feed and the fuses that read them: the last layer's step feeds the classifier and, under the carry,
//! keeps its post-norm rows as floats for the landing
def private crd_layer_fuses(var r : CudRdec) {
    let dim = r.dim
    for (l in range64(r.n_layers)) {
        var L & = unsafe(r.layers[l])
        let last = l + 1l == r.n_layers
        L.n_ffn = norm_off(l, RDEC_NORM_FFN)
        L.n_post_att = norm_off(l, RDEC_NORM_POST_ATT)
        L.n_post_ffn = norm_off(l, RDEC_NORM_POST_FFN)
        L.n_obias = norm_off(l, RDEC_NORM_OBIAS)
        L.n_router = norm_off(l, RDEC_NORM_ROUTER)
        L.n_pre2 = norm_off(l, RDEC_NORM_PRE_FFN2)
        L.n_post_ffn1 = norm_off(l, RDEC_NORM_POST_FFN1)
        L.n_post_ffn2 = norm_off(l, RDEC_NORM_POST_FFN2)
        L.n_next = last ? rdec_norms_final_base(r.n_layers, dim) : norm_off(l + 1l, RDEC_NORM_ATT)
        L.fnext = last ? r.cls_fmt : crd_head_feed_fmt(r.layers[l + 1l])
        L.storex_next = last ? r.carry : crd_storex_head(r.layers[l + 1l])
        L.down_ar = !L.moe && L.f2 == int(KqFmt.q8) && !kq_sb(L.fnext) && !L.storex_next   // a routed layer's residual step is the combine
    }
}

def cud_rdec_set_cls(cls_block : int64; cls_fmt : int; cap : float; supp : array<int64>) {
    return if (g_cud.dry)
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
    return cud_serves_fmt(fmt) if (g_cud.dry)
    assert(g_crd != null, "cud_rdec_set_emb before prepare")
    return false if (!cud_serves_fmt(fmt))
    var r & = unsafe(*g_crd)
    r.emb_blk = emb_block
    r.emb_fmt = fmt
    r.emb_scale = escale
    crd_graphs_drop(r)
    return true
}

//! RdecUploadEmbF32Fn: an F32 / F16 / BF16 table's exact f32 rows on the device under the cap, the commands gathering off it from
//! here on (the plan charged the rows); false leaves the host's rows riding the in block
def cud_rdec_upload_emb_f32(fblob : array<float>; off, vocab, dim : int64; escale : float) : bool {
    let bytes = vocab * dim * 4l
    return false if (bytes > RDEC_EMB_F32_CAP || long_length(fblob) < off + vocab * dim)
    return true if (g_cud.dry)
    assert(g_crd != null, "cud_rdec_upload_emb_f32 before prepare")
    var r & = unsafe(*g_crd)
    let dev = cud_dev_zeroed(bytes)
    return false if (dev == 0ul)
    let src = unsafe(addr<void?>(fblob[off]))
    cuda_memcpy_htod(dev, src, uint64(bytes), g_cud.stream)
    verify(cud_sync(), "cud decode: the f32 token table did not land")
    cuda_mem_free(r.emb_f32)
    r.emb_f32 = dev
    r.emb_scale = escale
    crd_graphs_drop(r)
    return true
}

//! whether the commands gather their token rows on the device: a placed table, or the f32 table
def private crd_emb_on(r : CudRdec) : bool => r.emb_blk >= 0l || r.emb_f32 != 0ul

//! RdecSetPleFn: the per-layer-embedding branch's planes at the model's width (after prepare, before the layers): the commands'
//! side rows (a mapped block the host fills, the device's copy), the gate rows, and the file's f32 gate / projection rows where it
//! ships them so; false where the width is not a 32-multiple (the gate's feed and the act's requant take 32-blocks)
[arch(at="../ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-token-command")]
def cud_rdec_set_ple(ple : int64; gate_f32 : array<float>; proj_f32 : array<float>) : bool {
    return false if (ple <= 0l || ple % 32l != 0l)
    return true if (g_cud.dry)
    assert(g_crd != null, "cud_rdec_set_ple before prepare")
    var r & = unsafe(*g_crd)
    let all = r.n_layers * ple
    let nb = r.nb
    let inb = cuda_mem_alloc_host_mapped(uint64(all * 4l * nb))
    return false if (inb.host == null)
    r.ple = ple
    r.ple_all = all
    r.ple_in_host = inb.host
    r.ple_in_dev = inb.dev
    r.ple_dev = cud_dev_zeroed(all * 4l * nb)
    r.pleg = cud_dev_zeroed(ple * 4l * nb)
    r.pleact = cud_dev_zeroed(ple * 4l * nb)
    if (!empty(gate_f32)) {
        r.ple_gf32_bytes = long_length(gate_f32) * 4l
        r.ple_gf32 = cud_dev_zeroed(r.ple_gf32_bytes)
        cuda_memcpy_htod(r.ple_gf32, unsafe(addr<void?>(gate_f32[0])), uint64(r.ple_gf32_bytes), g_cud.stream)
    }
    if (!empty(proj_f32)) {
        r.ple_pf32_bytes = long_length(proj_f32) * 4l
        r.ple_pf32 = cud_dev_zeroed(r.ple_pf32_bytes)
        cuda_memcpy_htod(r.ple_pf32, unsafe(addr<void?>(proj_f32[0])), uint64(r.ple_pf32_bytes), g_cud.stream)
    }
    verify(cud_sync(), "cud decode: the per-layer-embedding f32 rows did not land")
    crd_graphs_drop(r)
    return true
}

//! RdecSetPleLayerFn: layer `l`'s gate [dim x ple] and projection [ple x dim] planes (a block of -1 is the kind's f32 rows)
def cud_rdec_set_ple_layer(l, bgate, bproj : int64; fgate, fproj : int) {
    return if (g_cud.dry)
    assert(g_crd != null && g_crd.ple > 0l, "cud_rdec_set_ple_layer before set_ple")
    var r & = unsafe(*g_crd)
    var L & = unsafe(r.layers[l])
    verify((bgate < 0l || cud_serves_fmt(fgate)) && (bproj < 0l || cud_serves_fmt(fproj)), "cud decode: a per-layer-embedding plane in a format the driver does not serve reached set_ple_layer")
    L.ple = true
    L.bpg = bgate
    L.bpp = bproj
    L.fpg = fgate
    L.fpp = fproj
    cuda_mem_free(L.regs_pg)
    cuda_mem_free(L.regs_pp)
    L.regs_pg = bgate < 0l ? 0ul : cud_regions([uint(bgate), 0u])
    L.regs_pp = bproj < 0l ? 0ul : cud_regions([uint(bproj), 0u])
    L.n_ple_post = norm_off(l, RDEC_NORM_PLE_POST)
    L.ple_fused = (bgate >= 0l && bproj >= 0l && fgate == int(KqFmt.q8) && fproj == int(KqFmt.q8) && r.ple <= 256l && r.ple % Q8_BLOCK_ELEMS == 0l
        && ensure_q8_gemv_pleact_cls())
    crd_graphs_drop(r)
}

//! RdecSetPleProjFn: the pre-step's projection on device off the file's own rows (bf16 halves `w16`, else f32 `w32`) and its norm
//! row: the router GEMV under 1/sqrt(dim) over the commands' rows and the window's, then the finish a (row, layer) slice a
//! workgroup (`crd_ple_pre`); false where the rows are not [all x dim], the norm row not [ple], or a class declines
[arch(at="../ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-token-command")]
def cud_rdec_set_ple_proj(w16 : array<uint16>; w32 : array<float>; pnorm : array<float>; all : int64) : bool {
    return false if (g_cud.dry)   // the mint's walk leaves the pre-step with the host, as the Vulkan home's does: the mapped lane's live walk seats it
    assert(g_crd != null && g_crd.ple > 0l, "cud_rdec_set_ple_proj before set_ple")
    var r & = unsafe(*g_crd)
    let bf16 = !empty(w16)
    let n = bf16 ? long_length(w16) : long_length(w32)
    return false if (n != all * r.dim || long_length(pnorm) != r.ple)
    let hw = bf16 && !gk_bf16_pairs(r.dim, 0l)
    return false if (!(ensure_ple_finish_cls() && (bf16 ? (hw ? ensure_router_gemv_bf16_hw_cls() : ensure_router_gemv_bf16_cls()) : ensure_router_gemv_cls())))
    cuda_mem_free(r.ple_wp)
    cuda_mem_free(r.ple_pnorm)
    cuda_mem_free(r.tok_plep)
    r.ple_wp_bf16 = bf16
    r.ple_wp = bf16 ? buf_upload(w16, g_cud.stream) : buf_upload(w32, g_cud.stream)
    r.ple_pnorm = buf_upload(pnorm, g_cud.stream)
    r.tok_plep = cud_dev_zeroed(r.ple_all * 4l * r.nb)
    if (r.pf_np > 0l) {
        cuda_mem_free(r.pf_plep)
        r.pf_plep = cud_dev_zeroed(r.pf_np * r.ple_all * 4l)
    }
    cuda_mem_free(r.ple_w16)
    r.ple_w16 = 0ul
    // the window's form: the rows as an f16 panel on the cm2 f16 tile where the row width walks in the tile's k steps, else the router GEMV
    if (r.dim % int64(F16G_BK) == 0l && ensure_f16_gemm_cm2_cls() && ensure_tower_clamp_cvt_cls() && (!bf16 || ensure_ple_bf16_cvt_cls())) {
        r.ple_w16 = cud_dev_zeroed(n * 2l)
        var pcw = gk_f16_rows_args(uint(n))
        if (bf16) {
            enc_ple_bf16_cvt_cls(g_cud.stream, fixed_array(r.ple_wp, r.ple_w16), pcw, rq_wgs(false, n))
        } else {
            enc_tower_clamp_cvt_cls(g_cud.stream, fixed_array(r.ple_wp, r.ple_w16), pcw, rq_wgs(false, n))
        }
    }
    verify(cud_sync(), "cud decode: the per-layer-embedding projection rows did not land")
    r.ple_gpu = true
    crd_graphs_drop(r)
    let rows_form = bf16 ? (hw ? "bf16 halfwords" : "bf16 pairs") : "f32 rows"
    to_log(LOG_INFO, "dasLLAMA cuda resident: the per-layer-embedding pre-step's projection runs on the router GEMV over {rows_form} at the token commands, the window's on {r.ple_w16 != 0ul ? "the cm2 f16 tile over its f16 panel" : "the router GEMV (the row width is off the tile's k step)"}\n")
    return true
}

//! RdecSetPleTableFn: the per-layer table in the arena: the commands and the ids prefill gather the side rows off it (the table
//! format's gather leaf at the row width layers x ple, scaled by sqrt(ple)) once the projection rides the device too
def cud_rdec_set_ple_table(blk : int64; fmt : int) : bool {
    return cud_serves_fmt(fmt) if (g_cud.dry)
    assert(g_crd != null && g_crd.ple > 0l, "cud_rdec_set_ple_table before set_ple")
    var r & = unsafe(*g_crd)
    return false if (!cud_serves_fmt(fmt))
    r.ple_tbl_blk = blk
    r.ple_tbl_fmt = fmt
    crd_graphs_drop(r)
    return true
}

//! the commands gather the E-series side rows off the rows' ids: the per-layer table, the projection and the token table ride the device
def private crd_side_gather_on(r : CudRdec) : bool => r.ple_tbl_blk >= 0l && r.ple_gpu && crd_emb_on(r)

//! The commands gather the E-series side rows off the rows' ids (a cell's served witness)
def cud_rdec_side_gather_on : bool => g_crd != null && crd_side_gather_on(*g_crd)

//! The commands' rows whose side row the device gathered off the row's token id, in this process (a cell's served witness)
def cud_rdec_side_gathered : int64 => g_crd != null ? g_crd.side_gathered : 0l

//! The one-row steps whose row the device gathered off the step's token id, in this process (a cell's served witness)
def cud_rdec_token_gathered : int64 => g_crd != null ? g_crd.tok_gathered : 0l

//! The N-row commands' rows whose token row the device gathered off the row's token id, in this process (a cell's served witness)
def cud_rdec_nrow_embed_gathered : int64 => g_crd != null ? g_crd.nrow_emb_gathered : 0l

//! The N-row commands' rows whose side row the device gathered off the row's token id, in this process (a cell's served witness)
def cud_rdec_nrow_side_gathered : int64 => g_crd != null ? g_crd.nrow_side_gathered : 0l

//! whether the N-row command over `rows` splits every layer's keys as the one-row command at `pos` does: on this home the split
//! count is the driver's own (`attn_nsplit`) and the unsplit form keys on the position alone, so the two sum the keys in one order
[unused_argument(rows, pos)]
def cud_rdec_rows_split_alike(rows, pos : int64) : bool => true

//! the one-row command's side row as its last step left it - gathered and finished on the device under the projection - read
//! back (a cell's witness of the device pre-step against the CPU's)
[cold_path]
def cud_rdec_ple_side_row : array<float> {
    assert(g_crd != null && g_crd.ple > 0l, "cud_rdec_ple_side_row before set_ple")
    verify(cud_sync(), "cud decode: the side row's read-back waits on a failed stream")
    return <- buf_download(g_crd.ple_dev, int(g_crd.ple_all), type<float>)
}

//! window row `row`'s side row as the last window chain left it - gathered and finished on the device under the projection's
//! window form - read back (a cell's witness of the window's pre-step against the CPU's)
[cold_path]
def cud_rdec_pf_side_row(row : int64) : array<float> {
    assert(g_crd != null && g_crd.ple > 0l && g_crd.pf_np > row, "cud_rdec_pf_side_row before set_ple or a window, or past the window")
    verify(cud_sync(), "cud decode: the window side row's read-back waits on a failed stream")
    return <- buf_download(g_crd.pf_ple + uint64(row * g_crd.ple_all * 4l), int(g_crd.ple_all), type<float>)
}

//! `nrows` rows' side rows off the per-layer table at their ids in `ids` into `side` (an `EMB_ID_HOST` row takes the host's
//! staged row in `xh`), scaled by sqrt(ple)
def private crd_side_gather(r : CudRdec; var s : void?; ids, xh, side : uint64; nrows : int64) {
    let at = g_cud_arenas[r.ple_tbl_fmt]
    cud_emb_enc(r.ple_tbl_fmt, s, at.wq, at.ws, ids, xh, side, nrows, r.ple_all, r.ple_tbl_blk, sqrt(float(r.ple)))
}

//! the pre-step's device half over `nrows` rows: the projection of the embedded rows `x` (a row's stride `dim`) into `plep`
//! under 1/sqrt(dim) - the bf16 pairs, the bf16 halfwords of an odd dim, or the f32 rows - then the finish a (row, layer) slice
//! a workgroup in place on the gathered side rows `side` (`GkPleFinish`: side = (side + pnorm . rms(proj)) / sqrt(2))
[arch(at="../ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-token-command")]
def private crd_ple_pre(r : CudRdec; var s : void?; x, plep, side : uint64; nrows : int64) {
    let all = r.ple_all
    let hw = !gk_bf16_pairs(r.dim, 0l)
    var pcr = GkRouterArgs(ne = uint(all), dim = uint(r.dim), eps = r.eps, ncols = uint(nrows), yscale = 1.0 / sqrt(float(r.dim)))
    each_col_tile(pcr, uint(nrows), GEMV_COL_TILE) $ {
        if (r.ple_wp_bf16 && hw) {
            enc_router_gemv_bf16_hw_cls(s, fixed_array(r.ple_wp, x, plep, r.norms), pcr, all)
        } elif (r.ple_wp_bf16) {
            enc_router_gemv_bf16_cls(s, fixed_array(r.ple_wp, x, plep, r.norms), pcr, all)
        } else {
            enc_router_gemv_cls(s, fixed_array(r.ple_wp, x, plep, r.norms), pcr, all)
        }
    }
    var pcf = GkPleFinArgs(ple = uint(r.ple), eps = r.eps)
    enc_ple_finish_cls(s, fixed_array(plep, r.ple_pnorm, side), pcf, r.n_layers * nrows)
}

//! the pre-step's device half over the window's `wlen` rows: the embedded rows as f16 under 1/sqrt(dim) into the cm2 f16 tile
//! against the projection's f16 panel (the Vulkan window's form), the finish after it; the router GEMV where the panel is not held
def private crd_ple_pre_window(r : CudRdec; var s : void?; wlen : int64) {
    if (r.ple_w16 == 0ul) {
        crd_ple_pre(r, s, r.pf_x, r.pf_plep, r.pf_ple, wlen)
        return
    }
    let dim = r.dim
    let all = r.ple_all
    var pcc = gk_f16_rows_args(uint(wlen * dim), 1.0 / sqrt(float(dim)))
    enc_tower_clamp_cvt_cls(s, fixed_array(r.pf_x, r.pf_xf), pcc, rq_wgs(false, wlen * dim))
    var pcg = F16GemmArgs(n = uint(dim), d = uint(all), rows = uint(wlen), ystride = uint(all))
    enc_f16_gemm_cm2_cls(s, fixed_array(r.ple_w16, r.pf_xf, r.pf_plep), pcg, f16_gemm_cm2_wgs(wlen, all))
    var pcf = GkPleFinArgs(ple = uint(r.ple), eps = r.eps)
    enc_ple_finish_cls(s, fixed_array(r.pf_plep, r.ple_pnorm, r.pf_ple), pcf, r.n_layers * wlen)
}

//! the host's side rows of `nrows` rows (`ple_all` floats a row: the finished rows, or the gathered rows the device finishes)
//! into the commands' mapped block; a command whose rows the device gathers brings none, counted
def private crd_ple_stage(var r : CudRdec; rows : array<float>; nrows : int64; gathered : bool) {
    return if (r.ple <= 0l)
    if (gathered && crd_side_gather_on(r)) {
        r.side_gathered += nrows
        return
    }
    verify(long_length(rows) >= r.ple_all * nrows, "cud decode: the per-layer-embedding side rows are short of the rows")
    let src = unsafe(addr<void?>(rows[0]))
    unsafe(memcpy(r.ple_in_host, src, r.ple_all * 4l * nrows))
}

//! the per-layer-embedding branch after layer `l`'s FFN residual step, over `nrows` rows: the gate GEMV over the residual rows as
//! they are, gelu(gate) x the rows' side slices into the projection's feed, the projection, and the residual step under the branch's
//! post norm into the next head's feed (the FFN step's own next feed is recomputed here)
[arch(at="../ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-token-command")]
def private crd_ple_block(r : CudRdec; var s : void?; l : int64; L : CudLayer; nrows : int64) {
    let dim = r.dim
    let ple = r.ple
    let nr = uint(nrows)
    if (L.bpg < 0l) {   // the file's f32 gate rows read the residual rows as they are
        var pcg = gk_ple_gate_args(ple, dim, l, nr)
        enc_router_gemv_cls(s, fixed_array(r.ple_gf32, r.x, r.pleg, r.norms), pcg, ple)
    } else {
        crd_rq_rows(s, L.fpg, r.x, r.xq, r.xs, dim * nrows)
        crd_gemv(s, L.fpg, L.regs_pg, r.xq, r.xs, r.pleg, dim, ple, 0u, 1l, nr, uint(ple))
    }
    if (L.bpp < 0l) {   // the act as floats, the f32 projection rows over them
        var pcf = gk_ple_act_f32_args(ple, l, r.ple_all, nr)
        enc_ple_act_f32_cls(s, fixed_array(r.pleg, r.ple_dev, r.pleact), pcf, (ple * nrows + 255l) / 256l)
        var pcp = gk_ple_proj_args(ple, dim, l, nr)
        enc_router_gemv_cls(s, fixed_array(r.ple_pf32, r.pleact, r.ffnout, r.norms), pcp, dim)
    } elif (L.ple_fused) {   // the act, its requant and the proj GEMV in one dispatch, the columns in tiles of eight
        let a = g_cud_arenas[L.fpp]
        var pcf = gk_ple_fused_args(ple, dim, L.bpp, l, r.ple_all, nr)
        each_col_tile(pcf, nr, GEMV_COL_TILE) $ {
            enc_q8_gemv_pleact_cls(s, fixed_array(a.wq, a.ws, r.pleg, r.ple_dev, r.ffnout), pcf, (dim + 7l) / 8l)
        }
    } else {
        let kqp = kq_sb(L.fpp)
        var pca = gk_ple_act_args(ple, l, r.ple_all, nr)
        pca.nblk = uint(ple * nrows / kq_unit(kqp))
        if (kqp) {
            enc_q8k_actrq_cls(s, fixed_array(r.pleg, r.ple_dev, r.hq, r.hs_), pca, rq_wgs(true, ple * nrows))
        } else {
            enc_q8_actrq_cls(s, fixed_array(r.pleg, r.ple_dev, r.hq, r.hs_), pca, rq_wgs(false, ple * nrows))
        }
        crd_gemv(s, L.fpp, L.regs_pp, r.hq, r.hs_, r.ffnout, ple, dim, 0u, 1l, nr, uint(dim))
    }
    var pcn = GkAddRmsArgs(dim = uint(dim), add_on = 1u, woff = uint(L.n_next), eps = r.eps, ascale = L.out_scale, pre_on = 1u, pwoff = uint(L.n_ple_post))
    crd_ar_rq(r, s, L.fnext, r.ffnout, pcn, nrows, L.storex_next)
}

//! the window's per-layer-embedding branch over `wlen` rows after layer `l`'s FFN residual step: the residual rows as the gate's f16
//! feed, the gate GEMM, the act with the rows' side slices into the projection's f16 feed, the projection into the FFN out rows
def private crd_window_ple(r : CudRdec; var s : void?; l : int64; var L : CudLayer; wlen : int64) {
    let dim = r.dim
    let ple = r.ple
    var pcc = gk_f16_rows_args(uint(wlen * dim))
    enc_tower_clamp_cvt_cls(s, fixed_array(r.pf_x, r.pf_xf), pcc, rq_wgs(false, wlen * dim))
    let woff = uint(l * ple * dim)
    if (L.bpg < 0l) {
        var pcg = DnBaArgs(npos = uint(wlen), dim = uint(dim), nvh = uint(ple / 2l), woff = woff, xstride = uint(dim), obase_b = 0u, obase_g = uint(ple / 2l), ostride = uint(ple))
        enc_dn_ba_cls(s, fixed_array(r.ple_gf32, r.pf_xf, r.pf_pleg), pcg, dn_ba_wgs(wlen, ple / 2l))
    } else {
        crd_gemm(L.sch[SCH_PG], L.fpg, L.bpg, g_cud_arenas[L.fpg], r.pf_xf, r.pf_pleg, dim, ple, wlen)
    }
    var pca = ActArgs(nelem = uint(wlen * ple), act = ACT_GELU, nblk = uint(wlen * ple / kq_unit(false)), elem0 = 0u,
        ubase = uint(l * ple), ustride = uint(r.ple_all), ulen = uint(ple))
    enc_actf16_cls(s, fixed_array(r.pf_pleg, r.pf_ple, r.pf_hf), pca, rq_wgs(false, wlen * ple))
    if (L.bpp < 0l) {
        var pcp = DnBaArgs(npos = uint(wlen), dim = uint(ple), nvh = uint(dim / 2l), woff = woff, xstride = uint(ple), obase_b = 0u, obase_g = uint(dim / 2l), ostride = uint(dim))
        enc_dn_ba_cls(s, fixed_array(r.ple_pf32, r.pf_hf, r.pf_ffnout), pcp, dn_ba_wgs(wlen, dim / 2l))
    } else {
        crd_gemm(L.sch[SCH_PP], L.fpp, L.bpp, g_cud_arenas[L.fpp], r.pf_hf, r.pf_ffnout, ple, dim, wlen)
    }
}

//! The driver holds an F32 / F16 / BF16 table's exact rows and gathers off them (a cell's served witness)
def cud_rdec_emb_f32_on : bool => g_crd != null && g_crd.emb_f32 != 0ul

//! `npos` rows gathered off the table the driver holds (`ids` their ids, `xh` the host's staged rows an `EMB_ID_HOST` row takes) into `y`
def private crd_emb_gather(r : CudRdec; var s : void?; ids, xh, y : uint64; npos : int64; escale : float) {
    if (r.emb_f32 != 0ul) {
        var pce = EmbArgs(npos = uint(npos), dim = uint(r.dim), wblk0 = 0u, embed_scale = escale)
        enc_emb_gather_f32_cls(s, fixed_array(r.emb_f32, ids, y, xh), pce, (npos * r.dim / 4l + 255l) / 256l)
        return
    }
    let ae = g_cud_arenas[r.emb_fmt]
    cud_emb_enc(r.emb_fmt, s, ae.wq, ae.ws, ids, xh, y, npos, r.dim, r.emb_blk, escale)
}

def private crd_dn_smalls_put(smalls : uint64; float_off : int64; src : float const?; n : int64) {
    cuda_memcpy_htod(smalls + uint64(float_off * 4l), unsafe(reinterpret<void?>(src)), uint64(n * 4l), g_cud.stream)
}

//! a recurrent (deltanet) layer: the beta/alpha planes, or under the f32 arm its rows at `ba_off` of `cud_rdec_upload_dn_ba`'s; a session's state arrives through `cud_rdec_dn_own`
[arch(at="../ARCHITECTURE_GPU.md#gpu-backends")]
def cud_rdec_set_dn_layer(l, bqkv, bz, bout, bbeta, balpha, b1, b3, b2, ba_off : int64; fqkv, fz, fout, fbeta, falpha, f1, f3, f2 : int;
                          convw : float const?; wnormp : float const?; ap : float const?; dtp : float const?) {
    return if (g_cud.dry)
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
    let state_dev = cud_dev_zeroed(state_bytes * nslot)
    let smalls_dev = cud_dev_zeroed(smalls_bytes)
    if (state_dev == 0ul || smalls_dev == 0ul) {
        panic("cud decode: a recurrent layer's state slots found no room on the device ({cuda_last_error()})")
    }
    L.dn |> reserve(nslot)
    L.dn |> resize(nslot)
    for (s in range64(nslot)) {
        L.dn[s] = CudDnSlot(state_dev = state_dev, smalls_dev = smalls_dev, slot_state_bytes = state_bytes, ring_image_bytes = ring_elems * 4l,
                            smalls_bytes = smalls_bytes, state_byte_off = s * state_bytes, hist_float_off = DN_SM_HIST + s * 2l * ring_elems)
    }
    crd_dn_smalls_put(smalls_dev, DN_SM_TAPS, convw, r.dn_cd * r.dn_dconv)
    crd_dn_smalls_put(smalls_dev, DN_SM_WNORM, wnormp, r.dn_ds)
    crd_dn_smalls_put(smalls_dev, DN_SM_A, ap, r.dn_nvh)
    crd_dn_smalls_put(smalls_dev, DN_SM_DT, dtp, r.dn_nvh)
    crd_graphs_drop(r)
}

//! every recurrent layer's f32 rows in layer order, a layer's `[beta (nvh x dim) ; alpha (nvh x dim)]` at its `ba_off`
def cud_rdec_upload_dn_ba(rows : array<float>) {
    return if (g_cud.dry)
    assert(g_crd != null && g_crd.hybrid, "cud_rdec_upload_dn_ba before a hybrid prepare")
    cuda_mem_free(g_crd.dn_ba)
    g_crd.dn_ba = buf_upload(rows, g_cud.stream)
}

def private crd_dn_flush_down(st : CudDnSlot) {
    unsafe {
        cuda_memcpy_dtoh(reinterpret<void?>(st.own.state_addr), st.state_dev + uint64(st.state_byte_off), uint64(st.slot_state_bytes), g_cud.stream)
        cuda_memcpy_dtoh(reinterpret<void?>(st.own.hist_addr), st.smalls_dev + uint64(st.hist_float_off * 4l + int64(st.own.parity) * st.ring_image_bytes),
            uint64(st.ring_image_bytes), g_cud.stream)
    }
    verify(cud_sync(), "cud decode: a deltanet state flush did not land - the caller reads the host state as soon as this returns")
}

def private crd_dn_drop_slot(var st : CudDnSlot) {
    dn_slot_drop(st.own)
}

def private crd_dn_send_home(var st : CudDnSlot) {
    if (st.own.dirty) {
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
    dn_slot_bind(st.own, stp, histp, cur_par, false)
}

//! bind layer `l`'s slot in the selected region to the calling session's state (`stp`) and conv history (`histp`) before a token
[arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#dn-step-owner")]
def cud_rdec_dn_own(l : int64; stp : float?; histp : float?) {
    cud_bind()
    var L & = unsafe(g_crd.layers[l])
    let slot = g_crd.region
    var st & = unsafe(L.dn[slot])
    return if (dn_slot_holds(st.own, stp))
    crd_chain_settle()   // another session's state takes the slot: a chained step on the old owner's is discarded first
    if (st.own.valid && st.own.dirty) {
        crd_dn_flush_down(st)
    }
    crd_dn_state_upload(st, stp, histp, g_crd.dn_parity[slot])
}

//! After a resident prefill, hand recurrent layer `l`'s device slot to the session `stp` / `histp` name (valid, dirty, bound)
[arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#hybrid-token-command")]
def cud_rdec_prefill_dn_own(l : int64; stp : float?; histp : float?) {
    crd_chain_settle()
    var L & = unsafe(g_crd.layers[l])
    var st & = unsafe(L.dn[g_crd.region])
    dn_slot_bind(st.own, stp, histp, g_crd.dn_parity[g_crd.region], true)
}

//! Sync layer `l`'s device state back into the calling session's buffers where the device holds the newer copy
[unused_argument(histp)]
def cud_dn_step_flush(l : int64; var stp : float?; var histp : float?) {
    cud_bind()
    return if (g_crd == null || !g_crd.hybrid || l >= long_length(g_crd.layers) || !g_crd.layers[l].recurrent)
    var L & = unsafe(g_crd.layers[l])
    for (st in L.dn) {
        if (dn_slot_holds(st.own, stp)) {
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
    cud_bind()
    crd_dn_drop_slots() $(_st : CudDnSlot) : bool {
        return true
    }
}

//! the copies one session's state owns dropped, unwritten: the session reset that state, or it is dying
[hot_path, arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#dn-step-owner")]
def cud_dn_step_release(state_lo : uint64; state_hi : uint64) {
    crd_dn_drop_slots() $(st : CudDnSlot) : bool {
        return dn_slot_in(st.own, state_lo, state_hi)
    }
}

def private crd_dn_advance(var r : CudRdec; region : int64) {
    r.dn_parity[region] = 1 - r.dn_parity[region]
    for (L in r.layers) {
        if (L.recurrent) {
            dn_slot_advance(L.dn[region].own, r.dn_parity[region])
        }
    }
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_WINDOW.md#vk-prefill-dn-block")]
def private crd_dn_begin(var r : CudRdec; segs : array<RdecPfSeg>) {
    for (sg in segs) {
        if (sg.pos0 == 0l) {
            for (L in r.layers) {
                continue if (!L.recurrent || !L.dn[sg.region].own.valid)
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
    if (storex && rows == 1l) {   // the normed row stored as floats beside the quants: a recurrent head's beta/alpha rows read it, the carry lands it
        if (kq_sb(fmt)) {
            enc_cls_ar_rqkx(s, fixed_array(r.x, add, r.norms, r.xq, r.xs, r.xb), pc, rows)
        } else {
            enc_cls_ar_rqx(s, fixed_array(r.x, add, r.norms, r.xq, r.xs, r.xb), pc, rows)
        }
    } elif (storex || (kq_sb(fmt) && rows > 1l)) {   // the rows' normed floats, then their requant: the fused forms quantize one row
        enc_cls_ar(s, fixed_array(r.x, add, r.norms, r.xb), pc, rows)
        crd_rq_rows(s, fmt, r.xb, r.xq, r.xs, r.dim * rows)
    } elif (!kq_sb(fmt)) {
        enc_cls_ar_rq_b(s, bufs, pc, rows)
    } else {
        enc_cls_ar_rqk(s, bufs, pc, rows)
    }
}

def private crd_attn_rqk(L : CudLayer) : uint => da_rqk_pick(g_crd.kv_dt == KVDtype.tq4, L.fo, L.hs)

//! the attention stamps' gate flag: off over a tq4 mirror, where the un-rotation gates (the Vulkan home's `rd_attn_gated`)
[arch(at = "../ARCHITECTURE_GPU_SHARED_ATTN.md#vk-kv-tq4-basis")]
def private crd_attn_gated(r : CudRdec) : bool => r.q_gated && r.kv_dt != KVDtype.tq4

//! the bias plane the rope passes bind: the rows under `has_bias`, else the placeholder the kernels never read (`hasb` 0)
def private crd_bias_plane(r : CudRdec) : uint64 => r.has_bias ? r.bias : r.dummy

def private crd_hasb(r : CudRdec) : uint => r.has_bias ? 1u : 0u

def private crd_wo_ar_args(r : CudRdec; L : CudLayer) : GkAddRmsArgs =>
    GkAddRmsArgs(dim = uint(r.dim), add_on = 1u, woff = uint(L.n_ffn), eps = r.eps, ascale = 1.0,
        pre_on = L.pre_post ? 1u : 0u, pwoff = uint(L.n_post_att),
        abias_on = r.has_obias ? 1u : 0u, aboff = uint(L.n_obias))

def private crd_topk_args(r : CudRdec; L : CudLayer) : GkTopKArgs {
    let dim = uint(r.dim)
    let nfe = uint(r.nfe)
    var pct = gk_topk_routing(r.ne, r.k, r.norm_topk, r.wscale, r.ner, r.router_bias, uint(L.router_slot * r.ne),
        L.moe_g4 ? uint(L.router_slot * r.ne) : GK_NO_DSCALE)
    pct.base1 = uint(L.be1); pct.stride1 = nfe * dim / uint(kq_unit(L.fe1))
    pct.base3 = uint(L.be3); pct.stride3 = nfe * dim / uint(kq_unit(L.fe3))
    pct.base2 = uint(L.be2); pct.stride2 = nfe * dim / uint(kq_unit(L.fe2))
    pct.xnb2 = nfe / uint(kq_unit(L.fe2)); pct.xnb1 = dim / uint(kq_unit(L.fe1))
    return pct
}

def private crd_moe_act(r : CudRdec; var s : void?; L : CudLayer; gate, up, outq, outs : uint64; slot_elems : int64) {
    let dkq = kq_sb(L.fe2)
    var pca = ActArgs(nelem = uint(slot_elems), act = act_code(r.act_oai, r.is_gelu), nblk = uint(slot_elems / kq_unit(dkq)),
        bias_on = r.exps_bias ? 1u : 0u, gboff = uint(L.router_slot * r.ne * r.nfe), brow = uint(r.nfe))
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

def private crd_comb_args(r : CudRdec; L : CudLayer) : GkAddRmsArgs =>
    GkAddRmsArgs(dim = uint(r.dim), add_on = L.b1 >= 0l ? 1u : 0u, woff = uint(L.n_next), eps = r.eps, ascale = L.out_scale,
        slots = uint(r.k), lstride = uint(r.ner), gated = r.sh_gated && L.b1 >= 0l ? 1u : 0u,
        pre_on = L.pre_post ? 1u : 0u, pwoff = uint(L.n_post_ffn),
        dbias_on = r.exps_bias ? 1u : 0u, dboff = uint(L.router_slot * r.ne * r.dim))

//! an rms of x under the norm row at `woff`, no add, no scale: the gemma-4 routed feed's norm (the routed pre norm; the experts' own
//! requant or the window's gather reads it) and the window router's input (the router scale row, a weightless rms)
def private crd_rms_args(r : CudRdec; woff : int64) : GkAddRmsArgs =>
    GkAddRmsArgs(dim = uint(r.dim), add_on = 0u, woff = uint(woff), eps = r.eps, ascale = 1.0)

//! the gemma-4 combine's push block (`GkMoeCombG4`): the routed sum and the dense row each under its post norm, their sum under the
//! post-FFN norm, the next norm out; a host layer's sum is the one routed row of weight one
def private crd_g4_comb_args(r : CudRdec; L : CudLayer) : GkMoeCombArgs =>
    GkMoeCombArgs(n = uint(r.dim), k = L.experts_host ? 1u : uint(r.k), eps = r.eps, ascale = L.out_scale, presummed = 0u,
        rpost_off = uint(L.n_post_ffn2), dpost_off = uint(L.n_post_ffn1), spost_off = uint(L.n_post_ffn), nwoff = uint(L.n_next))

//! the router GEMV over the rows' logits: the stored normed rows, or (the gemma-4 form) x under its own rms and the router scale row
def private crd_router_rows(r : CudRdec; var s : void?; L : CudLayer; nrows : int64) {
    let nr = uint(nrows)
    var pcr = GkRouterArgs(ne = uint(r.ner), dim = uint(r.dim), woff = uint(L.router_slot * r.ner * r.dim), obase = 0u, ncols = nr,
        norm_on = L.moe_g4 ? 1u : 0u, nwoff = uint(L.n_router), eps = r.eps)
    each_col_tile(pcr, nr, GEMV_COL_TILE) $ {
        enc_router_gemv_cls(s, fixed_array(r.router, L.moe_g4 ? r.x : r.xb, r.moe_dlog, r.norms), pcr, r.ner)
    }
}

//! the gemma-4 routed feed into `xb`: x under the routed pre norm, the rows the experts' requant, the gather or the host reads
def private crd_g4_feed(r : CudRdec; var s : void?; L : CudLayer; x, ffnout, xb : uint64; nrows : int64) {
    var pcn = crd_rms_args(r, L.n_pre2)
    enc_cls_ar(s, fixed_array(x, ffnout, r.norms, xb), pcn, nrows)
}

//! a host layer's cut: the shared expert's dense tail into ffnout, the router GEMV over the stored normed rows (a gemma-4 layer's over x,
//! its routed feed into xb for the host), then the rows and the logits landed in the mapped blocks the host reads; the segment ends here
[arch(at="../ARCHITECTURE_GPU_VULKAN_HC.md#hc-token-command")]
def private crd_host_cut(r : CudRdec; var s : void?; L : CudLayer; nrows : int64) {
    let dim = r.dim
    let nr = uint(nrows)
    if (L.b1 >= 0l) {
        crd_ffn_hidden(r, s, L, nrows)
        crd_gemv(s, L.f2, L.regs_down, r.hq, r.hs_, r.ffnout, L.hid, dim, 0u, 1l, nr, uint(dim))
    }
    if (L.moe_g4) {
        crd_g4_feed(r, s, L, r.x, r.ffnout, r.xb, nrows)
    }
    crd_router_rows(r, s, L, nrows)
    cuda_memcpy_dtoh(r.hx_host, r.xb, uint64(dim * 4l * nrows), s)
    cuda_memcpy_dtoh(r.hlog_host, r.moe_dlog, uint64(r.ner * 4l * nrows), s)
}

//! the next segment's opening on the host's sums: the sums up, the pool's hit rows added under their weights where the layer has one,
//! then the layer's residual step over the sum as its one routed row of weight one beside the shared expert's row at its gate, and the
//! next head's feed
[arch(at="../ARCHITECTURE_GPU_VULKAN_POOL.md#hc-hot-pool")]
def private crd_host_resume(r : CudRdec; var s : void?; L : CudLayer; l, nrows : int64) {
    let dim = r.dim
    cuda_memcpy_htod(r.hacc_dev, r.hacc_host, uint64(dim * 4l * nrows), s)
    if (l < long_length(r.hot) && r.hot[l].on) {
        var pcc = GkMoeCombArgs(n = uint(dim), k = uint(r.k), r0 = 1u, r1 = uint(nrows * r.k) + 1u)
        enc_combine_cls(s, fixed_array(r.hacc_dev, r.hot_ident1, r.hot_w_dev, r.edown), pcc, nrows)
    }
    if (L.moe_g4) {
        var pcg = crd_g4_comb_args(r, L)
        enc_cls_ar_comb_g4(s, fixed_array(r.x, r.moe_ident, r.ones, r.hacc_dev, r.ffnout, r.norms, r.xb), pcg, nrows)
    } else {
        var pc14 = crd_comb_args(r, L)
        pc14.slots = 1u
        pc14.dbias_on = 0u   // the host summed the bias rows
        enc_cls_ar_comb(s, fixed_array(r.x, r.ffnout, r.norms, r.xb, r.moe_ident, r.ones, r.hacc_dev, r.moe_dlog, r.moe_db, r.moe_eidx), pc14, nrows)
    }
    crd_rq_rows(s, L.fnext, r.xb, r.xq, r.xs, dim * nrows)
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#resident-moe-token")]
def private crd_moe_tail(r : CudRdec; var s : void?; L : CudLayer; nrows : int64) {
    let dim = r.dim
    let nfe = r.nfe
    let k = r.k
    let nr = uint(nrows)
    if (L.b1 >= 0l) {
        crd_ffn_hidden(r, s, L, nrows)
        crd_gemv(s, L.f2, L.regs_down, r.hq, r.hs_, r.ffnout, L.hid, dim, 0u, 1l, nr, uint(dim))
    }
    if (L.moe_g4) {
        crd_g4_feed(r, s, L, r.x, r.ffnout, r.xb, nrows)
    }
    var xq = r.xq
    var xs = r.xs
    if (L.xe_own) {
        crd_rq_rows(s, L.fe1, r.xb, r.moe_xq, r.moe_xs, dim * nrows)
        xq = r.moe_xq
        xs = r.moe_xs
    }
    crd_router_rows(r, s, L, nrows)
    var pct = crd_topk_args(r, L)
    cud_topk_n_enc(r.ne, s, fixed_array(r.moe_dlog, r.moe_meta1, r.moe_meta3, r.moe_meta2, r.moe_w, r.moe_ds, r.moe_rb, r.moe_eidx), pct, nrows)
    if (nrows == 1l && L.egu) {   // the one-row step: the gate and up dots, the act and its requant in one dispatch, a workgroup an activated Q8_0 block
        let a = g_cud_arenas[L.fe1]
        var pcg = GemvArgs(n = uint(dim), d = uint(nfe), nreg = uint(k), ybase = 0u, act = act_code(r.act_oai, r.is_gelu),
            bias_on = r.exps_bias ? 1u : 0u, gboff = uint(L.router_slot * r.ne * nfe))
        enc_mx4_gemv_gu_cls(s, fixed_array(a.wq, a.ws, r.moe_meta1, xq, xs, r.egate, r.moe_meta3, r.moe_gb, r.moe_ub, r.moe_eidx, r.ehq, r.ehs), pcg, k * nfe / Q8_BLOCK_ELEMS)
    } else {
        crd_gemv(s, L.fe1, r.moe_meta1, xq, xs, r.egate, dim, nfe, 0u, k * nrows)
        crd_gemv(s, L.fe3, r.moe_meta3, xq, xs, r.eup, dim, nfe, 0u, k * nrows)
        crd_moe_act(r, s, L, r.egate, r.eup, r.ehq, r.ehs, k * nfe * nrows)
    }
    crd_gemv(s, L.fe2, r.moe_meta2, r.ehq, r.ehs, r.edown, nfe, dim, 0u, k * nrows)
    if (L.moe_g4) {
        var pcg = crd_g4_comb_args(r, L)
        enc_cls_ar_comb_g4(s, fixed_array(r.x, r.moe_ident, r.moe_w, r.edown, r.ffnout, r.norms, r.xb), pcg, nrows)
    } else {
        var pc14 = crd_comb_args(r, L)
        enc_cls_ar_comb(s, fixed_array(r.x, r.ffnout, r.norms, r.xb, r.moe_ident, r.moe_w, r.edown, r.moe_dlog, r.moe_db, r.moe_eidx), pc14, nrows)
    }
    crd_rq_rows(s, L.fnext, r.xb, r.xq, r.xs, dim * nrows)
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
def private crd_ffn(r : CudRdec; var s : void?; L : CudLayer; nrows : int64) {
    let dim = r.dim
    let nr = uint(nrows)
    crd_ffn_hidden(r, s, L, nrows)
    if (L.down_ar && nrows == 1l) {
        let a2 = g_cud_arenas[L.f2]
        var pcda = GemvArArgs(n = uint(L.hid), d = uint(dim), wblk = uint(L.b2), xblk = 0u, add_on = 1u,
            woff = uint(L.n_next), pwoff = uint(L.n_post_ffn), pre_on = L.pre_post ? 1u : 0u,
            no_norm = 0u, eps = r.eps, ascale = L.ple ? 1.0 : L.out_scale)   // a PLE layer's out scale rides the branch's step after it
        enc_q8_gemv_ar_cls(s, fixed_array(a2.wq, a2.ws, r.hq, r.hs_, r.xb2, r.x, r.norms, r.xq, r.xs, r.ar_cnt), pcda, ar_wgs(dim))
    } else {
        crd_gemv(s, L.f2, L.regs_down, r.hq, r.hs_, r.ffnout, L.hid, dim, 0u, 1l, nr, uint(dim))
        var pc14 = GkAddRmsArgs(dim = uint(dim), add_on = 1u, woff = uint(L.n_next), eps = r.eps, ascale = L.ple ? 1.0 : L.out_scale,
            pre_on = L.pre_post ? 1u : 0u, pwoff = uint(L.n_post_ffn))
        crd_ar_rq(r, s, L.fnext, r.ffnout, pc14, nrows, L.storex_next)
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
        eps = r.eps, qstride = uint(crd_q_head_stride(r, L)), boff = uint(L.boff), hasb = crd_hasb(r), cbase = uint(L.cbase), vfk = L.vfk ? 1u : 0u, vnorm = L.vnorm ? 1u : 0u,
        rowwg = uint(r.n_heads + nkh), kvrow = uint(kvrow), qrow = uint(qrow), cosrow = uint(r.cos_elems), ring = uint(ring))
}

//! the rows' rope on q and k, and k and v into the mirror at each row's position and region (the row's token block); under a
//! block codec the rope stages the floats (row i of the staged planes, addressed through the staged token blocks) and the
//! codec's store quantizes them into the mirror; a q/k-normed model's rows take the fused pass instead
//! `tok` the rows' token blocks (the trunk's, or the head's a position below); `kv_only` ropes and stores the k heads alone - the
//! head re-warm's store-only pass, whose q rows nothing reads
def private crd_token_kv(r : CudRdec; var s : void?; l : int64; L : CudLayer; kmir, vmir : uint64; kvrow, nrows : int64; tok : uint64; kv_only : bool = false) {
    let qd = L.qd
    let kvd = L.kvd
    let hs = L.hs
    let kvq = kv_block_codec(r.kv_dt)
    let qplane = crd_q_plane(r)
    let qrow = crd_tok_q_row_stride(r, L)
    let nqh = kv_only ? 0l : r.n_heads
    if (r.qk_norm) {
        let nkh = L.kv_shared ? 0l : kvd / hs
        var pcf = crd_qkn_rope_args(r, l, L, kvq ? 0l : L.mir_base, nkh, kvrow, qrow, kvq ? 0l : L.ring)
        pcf.nh = uint(nqh)
        pcf.rowwg = uint(nqh + nkh)
        var qbufs = fixed_array(qplane, r.proj, kvq ? r.kst : kmir, kvq ? r.vst : vmir, r.cos, r.norms, kvq ? r.stage_tok : tok, crd_bias_plane(r), r.proj)
        if (r.kv_dt == KVDtype.f16) {
            enc_qkn_rope_f16_cls(s, qbufs, pcf, (nqh + nkh) * nrows)
        } else {
            enc_qkn_rope_cls(s, qbufs, pcf, (nqh + nkh) * nrows)
        }
    } else {
        let qpairs = kv_only ? 0l : qd / 2l
        let npairs = qpairs + kvd / 2l
        var pcr = gk_rope_kv_args(qd, kvd, hs, qpairs, npairs, r.neox, r.has_bias, hs, L.boff, L.cbase, nrows, kvrow, kvrow, r.cos_elems, kvq ? 0l : L.mir_base, L.ring)
        let rwgs = (npairs * nrows + int64(CUD_WG_X) - 1l) / int64(CUD_WG_X)
        var rbufs = fixed_array(r.proj, r.proj, r.proj, kvq ? r.kst : kmir, kvq ? r.vst : vmir, crd_bias_plane(r), r.cos, r.cos, kvq ? r.stage_tok : tok)
        if (r.kv_dt == KVDtype.f16) {
            enc_rope_kv_f16_cls(s, rbufs, pcr, rwgs)
        } else {
            enc_rope_kv_cls(s, rbufs, pcr, rwgs)
        }
    }
    if (r.kv_dt == KVDtype.tq4) {   // a shared-KV layer rotates its query and stores nothing: its donor's rows stand in the mirror
        let nkh = L.kv_shared ? 0l : kvd / hs
        var pct = KvTqStoreArgs(kvd = uint(kvd), hs = uint(hs), layerbase = uint(L.mir_base), pos0 = 0u, nqh = uint(nqh), nkh = uint(nkh),
            qrow = uint(qrow), qstride = uint(crd_q_head_stride(r, L)), inv_sqrt_hs = 1.0 / sqrt(float(hs)), ring = uint(L.ring))
        enc_kv_tq4_store_cls(s, fixed_array(qplane, r.kst, r.vst, kmir, vmir, r.tq4_signs, tok), pct, nrows * (nqh + 2l * nkh))
    } elif (kvq && !L.kv_shared) {
        var pcs = KvQStoreArgs(kvd = uint(kvd), rows = uint(nrows), layerbase = uint(L.mir_base), pos0 = 0u, ring = uint(L.ring))
        enc_kv_q8_store_cls(s, fixed_array(r.kst, r.vst, kmir, vmir, tok), pcs, kv_store_wgs(kvd * nrows))
    }
}

//! a layer's attention over the mirror into the wo feed, `nrows` rows a row a workgroup: each row's quants, or its floats where the
//! fold cannot block them (the fold it took, `DA_RQ_*`, returned); the keys in one piece a group under `unsplit` (the Vulkan
//! home's unsplit twin), else in the device's split, the last-arriving piece combining
def private crd_token_attn(r : CudRdec; var s : void?; l : int64; L : CudLayer; kmir, vmir : uint64; nrows : int64; unsplit : bool; tok : uint64) : uint {
    let hs = uint(L.hs)
    let mir_base = uint(L.mir_base)   // the layer's base: each row's token block adds its region
    let nsplit = unsplit ? 1l : r.attn_nsplit
    let rowwgs = da_attn_row_wgs(r.n_heads, L.kv_mul, nsplit)
    let rqk = crd_attn_rqk(L)
    let qrow = crd_tok_q_row_stride(r, L)
    var pca = DaAttnArgs(kvd = uint(L.kvd), hs = hs, kv_mul = uint(L.kv_mul), kbase = mir_base, vbase = mir_base,
        scale = r.scale, qstride = uint(crd_q_head_stride(r, L)), gated = crd_attn_gated(r) ? 1u : 0u, window = uint(L.window), cap = L.attn_cap, nsplit = uint(nsplit),
        rqk = rqk, sinkoff = uint(l * r.n_heads), rowwg = uint(rowwgs), qrow = uint(qrow), cntoff = uint(r.attn_cnt_off), ring = uint(L.ring))
    var abufs = fixed_array(crd_q_plane(r), kmir, vmir, r.attn, tok, r.attn_part, r.aq, r.as_)
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

//! a recurrent layer's head over `nrows` rows; under `seq` the fused step runs a row at a time (the same-slab verify's rows share one
//! slot), and under `roll` each row's step but the last is followed by the slot's copy into the rollback row
[arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#hybrid-token-command"), arch(at="../ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-verify-command")]
def private crd_dn_head(r : CudRdec; var s : void?; L : CudLayer; nrows : int64; seq : bool = false; roll : bool = false) {
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
    if (seq) {
        let st & = unsafe(L.dn[r.region])
        for (i in range64(nrows)) {
            pcd.row0 = uint(i)
            if (r.dn_zsig) {
                enc_dn_step_sig_cls(s, step_bufs, pcd, nvh)
            } else {
                enc_dn_step_cls(s, step_bufs, pcd, nvh)
            }
            if (roll && i + 1l < nrows) {   // scratch row i holds the state the region would hold had the round stepped rows 0 .. i alone
                let dst = r.roll_dev + uint64(i * r.roll_row_bytes + L.roll_off)
                cuda_memcpy_dtod(dst, st.state_dev + uint64(st.state_byte_off), uint64(st.slot_state_bytes), s)
                cuda_memcpy_dtod(dst + uint64(st.slot_state_bytes), st.smalls_dev + uint64(st.hist_float_off * 4l), uint64(2l * st.ring_image_bytes), s)
            }
        }
    } elif (r.dn_zsig) {
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
//! one attention layer over `nrows` rows whose token blocks sit in `tok` (the trunk's rows, or the draft head's a position below): the
//! projections, the rope and K/V store, the attention, wo and its residual step into the FFN norm, then the FFN tail; `kv_only`
//! stops after the k and v heads' store - the head re-warm's pass, which attends nothing
def private crd_attn_layer(r : CudRdec; var s : void?; l : int64; L : CudLayer; nrows : int64; unsplit : bool; tok : uint64; kv_only : bool = false) {
    let dim = r.dim
    let nr = uint(nrows)
    let qd = L.qd
    let kvd = L.kvd
    let kvrow = qd + 2l * kvd
    if (L.qkv_merged) {
        crd_gemv(s, L.fq, L.regs_qkv, r.xq, r.xs, r.proj, dim, kvd, 0u, L.nreg_qkv, nr, uint(kvrow))
    } else {
        if (!kv_only) {
            crd_gemv(s, L.fq, L.regs_qkv, r.xq, r.xs, crd_q_plane(r), dim, r.q_gated ? 2l * qd : qd, 0u, 1l, nr, uint(crd_tok_q_row_stride(r, L)))
        }
        if (kq_sb(L.fk) != kq_sb(L.fq)) {
            crd_rq_rows(s, L.fk, r.xb, r.xq, r.xs, dim * nrows)
        }
        if (!L.kv_shared) {   // a shared-KV layer projects q alone: its donor's rows sit in the mirror
            crd_gemv(s, L.fk, L.regs_k, r.xq, r.xs, r.proj, dim, kvd, uint(qd), 1l, nr, uint(kvrow))
            if (!L.vfk) {   // a V-from-K layer projects no v: the rope store takes V off the raw k row
                if (kq_sb(L.fv) != kq_sb(L.fk)) {
                    crd_rq_rows(s, L.fv, r.xb, r.xq, r.xs, dim * nrows)
                }
                crd_gemv(s, L.fv, L.regs_v, r.xq, r.xs, r.proj, dim, kvd, uint(qd + kvd), 1l, nr, uint(kvrow))
            }
        }
    }
    let kmir = r.k_slabs[L.mir_slab]
    let vmir = r.v_slabs[L.mir_slab]
    crd_token_kv(r, s, l, L, kmir, vmir, kvrow, nrows, tok, kv_only)
    return if (kv_only)
    let rqk = crd_token_attn(r, s, l, L, kmir, vmir, nrows, unsplit, tok)
    if (rqk == DA_RQ_NONE) {   // no fold (a head off the superblock, or tq4): the separate requant in the wo plane's class
        crd_rq_rows(s, L.fo, r.attn, r.aq, r.as_, qd * nrows)
    }
    let ao = g_cud_arenas[L.fo]
    if (L.wo_ar && nrows == 1l) {
        var pcwa = GemvArArgs(n = uint(qd), d = uint(dim), wblk = uint(L.bo), xblk = 0u, add_on = 1u,
            woff = uint(L.n_ffn), pwoff = uint(L.n_post_att), pre_on = L.pre_post ? 1u : 0u,
            no_norm = 0u, eps = r.eps, ascale = 1.0, abias_on = r.has_obias ? 1u : 0u, aboff = uint(L.n_obias))
        enc_q8_gemv_ar_cls(s, fixed_array(ao.wq, ao.ws, r.aq, r.as_, r.xb2, r.x, r.norms, r.xq, r.xs, r.ar_cnt), pcwa, ar_wgs(dim))
    } else {
        crd_gemv(s, L.fo, L.regs_o, r.aq, r.as_, r.xb2, qd, dim, 0u, 1l, nr, uint(dim))
        var pc8 = crd_wo_ar_args(r, L)
        crd_ar_rq(r, s, crd_ffn_feed_fmt(L), r.xb2, pc8, nrows, crd_ffn_storex(L))
    }
    crd_layer_tail(r, s, L, nrows)
}

//! a layer's FFN tail: the routed block, a host layer's cut (the segment ends after it), or the dense FFN
def private crd_layer_tail(r : CudRdec; var s : void?; L : CudLayer; nrows : int64) {
    if (L.experts_host) {
        crd_host_cut(r, s, L, nrows)
    } elif (L.moe) {
        crd_moe_tail(r, s, L, nrows)
    } else {
        crd_ffn(r, s, L, nrows)
    }
}

//! the classifier over the rows' feed, its epilogue, the picks into the out block, the logits after them where asked
def private crd_cls_tail(r : CudRdec; var s : void?; nrows : int64; logits_on : bool) {
    let nr = uint(nrows)
    crd_gemv(s, r.cls_fmt, r.cls_regs, r.xq, r.xs, r.logits, r.dim, r.vocab, 0u, 1l, nr, uint(r.vocab))
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

//! the head rows' embed halves: gathered unscaled off the ids in the in block's head slots (`EMB_ID_HOST` rows take the staged
//! row), or the host's rows off the in block
def private crd_head_embeds(r : CudRdec; var s : void?; nrows : int64; gathered : bool) {
    if (gathered) {
        crd_emb_gather(r, s, r.tok + uint64(CRD_IN_ID_WORD * 4), r.hemb, r.hemb, nrows, 1.0)
    } else {
        cud_words(s, r.in_dev + uint64(crd_in_hemb_word(r) * 4l), r.hemb, r.dim * nrows)
    }
}

//! the head's prologue over `nrows` head rows, the draft's and the verify re-warm's one shape: the embed and carry halves normed,
//! cat into the [enorm ; hnorm] rows, their requant, eh_proj with the rows as columns into x, the head's attention norm into its feed
[arch(at="../ARCHITECTURE_GPU_VULKAN_MTP.md#resident-draft-head"), arch(at="../ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-verify-command")]
def private crd_head_prelude(r : CudRdec; var s : void?; nrows : int64) {
    let dim = r.dim
    let hb = uint(r.head_norm_base)
    let L & = unsafe(r.head)
    var pce = GkAddRmsArgs(dim = uint(dim), add_on = 0u, woff = hb + uint(RDEC_HEAD_ENORM * dim), eps = r.eps, ascale = 1.0)
    enc_cls_ar(s, fixed_array(r.hemb, r.hemb, r.norms, r.hn_a), pce, nrows)
    var pch = GkAddRmsArgs(dim = uint(dim), add_on = 0u, woff = hb + uint(RDEC_HEAD_HNORM * dim), eps = r.eps, ascale = 1.0)
    enc_cls_ar(s, fixed_array(r.hsrc, r.hsrc, r.norms, r.hn_b), pch, nrows)
    var pcc = GkCat2Args(d = uint(dim), rows = uint(nrows))
    enc_cat2_cls(s, fixed_array(r.head_cat, r.hn_a, r.hn_b), pcc, (nrows * dim + 255l) / 256l)
    crd_rq_rows(s, r.head_eh_fmt, r.head_cat, r.xq, r.xs, 2l * dim * nrows)
    crd_gemv(s, r.head_eh_fmt, r.head_eh_regs, r.xq, r.xs, r.x, 2l * dim, dim, 0u, 1l, uint(nrows), uint(dim))
    var pca = GkAddRmsArgs(dim = uint(dim), add_on = 0u, woff = hb + uint(RDEC_HEAD_ATT * dim), eps = r.eps, ascale = 1.0)
    crd_ar_rq(r, s, L.fq, r.x, pca, nrows, crd_storex_head(L))
}

//! the head's token blocks and rope rows off the in block's head slots
def private crd_head_inputs(r : CudRdec; var s : void?; nrows : int64) {
    cud_words(s, r.in_dev + uint64(CRD_IN_HTOK_WORD * 4), r.head_tok, int64(CRD_TOK_WORDS) * nrows)
    cud_words(s, r.in_dev + uint64(crd_in_hcos_word(r) * 4l), r.cos, r.cos_elems * nrows)
}

//! the post-norm rows of the last residual step (`xb`) into the out block: the carry's landing
def private crd_land_hid(r : CudRdec; var s : void?; nrows : int64) {
    if (r.carry) {
        crd_land(s, r, r.xb, r.out_hid_word, r.dim * nrows)
    }
}

//! segment `seg`'s launches over `nrows` rows, captured once a row count and mode into the graph the steps replay; `unsplit` the
//! attention's one-piece form; under `verify` the rows are one stream's same-slab rows - a recurrent layer steps them one at a
//! time with the rollback copies between, the head re-warms its rows after the picks, the post-norm rows land beside the logits
[arch(at="../ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-token-command"), arch(at="../ARCHITECTURE_GPU_SHARED_ATTN.md#vk-decode-attn-split"), arch(at="../ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-verify-command")]
def private crd_chain(var r : CudRdec; seg, nrows : int64; logits_on, gathered, unsplit, verify : bool) {
    var s = g_cud.stream
    let dim = r.dim
    let l0 = seg == 0l ? 0l : r.seg_layer[seg - 1l] + 1l
    let seg_host_layer = r.seg_layer[seg]
    if (seg == 0l) {
        cud_words(s, r.in_dev, r.tok, int64(CRD_IN_HDR_WORDS))
        cud_words(s, r.in_dev + uint64(CRD_IN_HDR_WORDS * 4), r.cos, r.cos_elems * nrows)
        if (gathered) {
            crd_emb_gather(r, s, r.tok + uint64(CRD_IN_ID_WORD * 4), r.x, r.x, nrows, r.emb_scale)
        } else {
            cud_words(s, r.in_dev + uint64((int64(CRD_IN_HDR_WORDS) + r.cos_elems * nrows) * 4l), r.x, dim * nrows)
        }
        if (verify) {
            crd_head_embeds(r, s, nrows, gathered)   // the head's halves before layer 0 overwrites nothing of theirs: hemb is the head's own plane
            cud_words(s, r.in_dev + uint64(crd_in_hrow_word(r) * 4l), r.hsrc, dim)   // row 0: the pre-draft carry the host uploads
        }
        if (r.ple > 0l) {
            if (gathered && !verify && crd_side_gather_on(r)) {   // each row's side row off the per-layer table at its token id
                crd_side_gather(r, s, r.tok + uint64(CRD_IN_ID_WORD * 4), r.ple_in_dev, r.ple_dev, nrows)
            } else {
                cud_words(s, r.ple_in_dev, r.ple_dev, r.ple_all * nrows)
            }
            if (r.ple_gpu) {   // the pre-step's finish: the projection of the embedded rows, normed a slice at a time and averaged into the gathered rows
                crd_ple_pre(r, s, r.x, r.tok_plep, r.ple_dev, nrows)
            }
        }
        var pc_pro = GkAddRmsArgs(dim = uint(dim), add_on = 0u, woff = 0u, eps = r.eps, ascale = 1.0)
        crd_ar_rq(r, s, r.n_layers > 0l ? crd_head_feed_fmt(r.layers[0]) : r.cls_fmt, r.x, pc_pro, nrows, r.n_layers > 0l && crd_storex_head(r.layers[0]))
    } else {
        crd_host_resume(r, s, r.layers[l0 - 1l], l0 - 1l, nrows)
    }
    let roll = verify && r.roll_dev != 0ul
    for (l in range64(l0, seg_host_layer < 0l ? r.n_layers : seg_host_layer + 1l)) {
        let L & = unsafe(r.layers[l])
        if (L.recurrent) {
            crd_dn_head(r, s, L, nrows, verify, roll)
            var pcr = GkAddRmsArgs(dim = uint(dim), add_on = 1u, woff = uint(L.n_ffn), eps = r.eps, ascale = 1.0)
            crd_ar_rq(r, s, crd_ffn_feed_fmt(L), r.xb2, pcr, nrows, crd_ffn_storex(L))
            crd_layer_tail(r, s, L, nrows)
            continue
        }
        crd_attn_layer(r, s, l, L, nrows, unsplit, r.tok)
        if (L.ple) {
            crd_ple_block(r, s, l, L, nrows)
        }
    }
    return if (seg_host_layer >= 0l)
    crd_cls_tail(r, s, nrows, logits_on)
    crd_land_hid(r, s, nrows)
    if (verify) {
        if (nrows > 1l) {   // head row i's carry half is the trunk's post-norm row i - 1
            cuda_memcpy_dtod(r.hsrc + uint64(dim * 4l), r.xb, uint64((nrows - 1l) * dim * 4l), s)
        }
        crd_head_inputs(r, s, nrows)
        crd_head_prelude(r, s, nrows)
        crd_attn_layer(r, s, r.n_layers, r.head, nrows, unsplit, r.head_tok, true)
    }
}

//! the draft command: one head row at the selected region - the embed half (gathered off the id in the in block's head id slot,
//! or the host's row) and the uploaded carry normed into the cat row, eh_proj, the head layer through the token command's
//! attention and FFN, the head norm's row kept as the hidden, the classifier and the pick; the hidden and the logits into the out block
[arch(at="../ARCHITECTURE_GPU_VULKAN_MTP.md#resident-draft-head")]
def private crd_draft_chain(var r : CudRdec; logits_on, gathered, unsplit : bool) {
    var s = g_cud.stream
    cud_words(s, r.in_dev, r.tok, int64(CRD_IN_HDR_WORDS))
    crd_head_inputs(r, s, 1l)
    crd_head_embeds(r, s, 1l, gathered)
    cud_words(s, r.in_dev + uint64(crd_in_hrow_word(r) * 4l), r.hsrc, r.dim)
    crd_head_prelude(r, s, 1l)
    crd_attn_layer(r, s, r.n_layers, r.head, 1l, unsplit, r.head_tok)
    crd_cls_tail(r, s, 1l, logits_on)
    crd_land_hid(r, s, 1l)
}

//! the commands the driver captures: the token step over the in block's rows, the same-slab verify, the draft
enum private CrdCmd {
    token
    verify
    draft
    hot      //! the hot expert pool's hit chain, a graph a (layer, region count, rows)
}

//! a captured graph's key: the command, its segment, its row count, the region (a hybrid's verify bakes the region's slot into its
//! rollback copies) and the mode (bit 0 the logits down, bit 1 the rows gathered off their ids, bit 2 the attention unsplit)
def private crd_graph_key(cmd : CrdCmd; seg, nrows, region : int64; logits_on, gathered, unsplit : bool) : int {
    panic("dasLLAMA cuda resident: a graph key field past its bits (segment {seg} of 256, region {region} of 64, rows {nrows} of 256)") if (seg >= 256l || region >= 64l || nrows >= 256l)
    return (int(cmd) << 26) | (int(seg) << 18) | (int(region) << 12) | (int(nrows) << 4) | (logits_on ? 1 : 0) | (gathered ? 2 : 0) | (unsplit ? 4 : 0)
}

def private crd_run_cmd(var r : CudRdec; cmd : CrdCmd; seg, nrows : int64; logits_on, gathered, unsplit : bool) {
    if (cmd == CrdCmd.draft) {
        crd_draft_chain(r, logits_on, gathered, unsplit)
    } else {
        crd_chain(r, seg, nrows, logits_on, gathered, unsplit, cmd == CrdCmd.verify)
    }
}

//! the command's segment as a graph, captured on its key's first replay
def private crd_ensure_graph(var r : CudRdec; cmd : CrdCmd; seg, nrows : int64; logits_on, gathered, unsplit : bool) : int {
    let region = (cmd == CrdCmd.verify && r.hybrid) ? r.region : 0l
    let key = crd_graph_key(cmd, seg, nrows, region, logits_on, gathered, unsplit)
    return key if (key_exists(r.graphs, key))
    let g = cuda_graph_capture(g_cud.stream) {
        crd_run_cmd(r, cmd, seg, nrows, logits_on, gathered, unsplit)
    }
    if (g == null) {
        panic("dasLLAMA cuda resident: the {cmd} command's graph did not capture (segment {seg}, {nrows} rows): {cuda_last_error()}")
    }
    r.graphs[key] = g
    return key
}

//! the segments a command splits into: a host layer's cut each, then the last one to the classifier
def private crd_segments_setup(var r : CudRdec) {
    r.seg_layer |> clear()
    for (l in range64(r.n_layers)) {
        if (r.layers[l].experts_host) {
            r.seg_layer |> push(l)
        }
    }
    r.seg_layer |> push(-1l)
    r.nseg = long_length(r.seg_layer)
}

//! one segment launched as its graph (or run plainly on the driver's first run, so every kernel loads before a capture) and waited -
//! or, under the greedy chain (`wait` off), left running for the chained step to queue behind
def private crd_launch_seg(var r : CudRdec; cmd : CrdCmd; seg, nrows : int64; logits_on, gathered, unsplit : bool; wait : bool = true) {
    if (r.token_steps == 0l) {
        crd_run_cmd(r, cmd, seg, nrows, logits_on, gathered, unsplit)
    } else {
        let key = crd_ensure_graph(r, cmd, seg, nrows, logits_on, gathered, unsplit)
        if (!cuda_graph_launch(r.graphs[key], g_cud.stream)) {
            panic("dasLLAMA cuda resident: the {cmd} command did not launch: {cuda_last_error()}")
        }
    }
    return if (!wait)
    if (!cud_sync()) {
        panic("dasLLAMA cuda resident: the {cmd} command failed on the device (segment {seg}): {cuda_last_error()}")
    }
}

//! the command over the in block's rows, a segment at a time with the host's routed sums between (`host(l)` after the segment that
//! cut at layer `l`); the stream synced after each, so the out block holds the rows at the end; `maxcnt` the rows' widest key
//! count, which picks the attention's unsplit form under the unsplit position
[arch(at="../ARCHITECTURE_GPU_VULKAN_HC.md#hc-token-command")]
def private crd_launch(var r : CudRdec; cmd : CrdCmd; nrows : int64; logits_on, gathered : bool; maxcnt : int64; wait : bool; host : block<(l : int64) : void>) {
    let unsplit = r.unsplit_on && maxcnt < RD_UNSPLIT_POS
    if (r.nseg == 0l) {
        crd_segments_setup(r)
    }
    let nseg = cmd == CrdCmd.draft ? 1l : r.nseg
    let selected = r.sel_ask && r.sel_ask_rows == nrows   // the selection stands in for the rows' logits: the command lands the picks alone
    r.ch.landed = 0
    for (seg in range64(nseg)) {
        crd_launch_seg(r, cmd, seg, nrows, logits_on && !selected, gathered, unsplit, wait || seg + 1l < nseg || selected)
        if (seg + 1l < nseg) {
            invoke(host, r.seg_layer[seg])
        }
    }
    r.token_steps++
    if (unsplit) {
        r.unsplit_steps++
    }
    r.sel_landed = 0l
    if (selected) {
        r.sel_ask = false
        var pcs = GkClsSelArgs(vocab = uint(r.vocab))
        enc_cls_select(g_cud.stream, fixed_array(r.logits, r.sel_words_dev, r.sel_hdr_dev, r.sel_ids_dev, r.sel_vals_dev, r.sel_rec_dev), pcs, nrows)
        verify(cud_sync(), "cud decode: the selection did not land")
        r.sel_landed = nrows
    }
}

//! RdecSelAskFn: the next command of `nrows` rows lands its selection (`words` SEL_WORDS a row) in place of its logits rows
[arch(at="../ARCHITECTURE_GPU_VULKAN_LANDING.md#logits-selection-landing")]
def cud_rdec_sel_ask(nrows : int64; words : array<uint>) : bool {
    return false if (g_crd == null || !g_crd.ready)
    var r & = unsafe(*g_crd)
    verify(nrows >= 1l && nrows <= r.nb && long_length(words) >= SEL_WORDS * nrows, "cud_rdec_sel_ask: rows past the per-row planes, or words short of them")
    let wp = unsafe(addr<void?>(words[0]))
    unsafe(memcpy(r.sel_words_host, wp, SEL_WORDS * 4l * nrows))
    r.sel_ask = true
    r.sel_ask_rows = nrows
    return true
}

//! RdecSelLandingFn: the last command's selection, pointers into the mapped landing; rows = 0 where the command landed its rows or picks
[arch(at="../ARCHITECTURE_GPU_VULKAN_LANDING.md#logits-selection-landing")]
def cud_rdec_sel_landing() : SelLanding {
    var l : SelLanding
    return l if (g_crd == null)
    var r & = unsafe(*g_crd)
    r.sel_ask = false   // an ask no command took dies here, never on a later command
    return l if (r.sel_landed == 0l)
    unsafe {
        l.rows = r.sel_landed
        l.cap = SEL_CAP
        l.recent_cap = SEL_RECENT
        l.hdr = reinterpret<uint const?>(r.sel_hdr_host)
        l.ids = reinterpret<uint const?>(r.sel_ids_host)
        l.vals = reinterpret<float const?>(r.sel_vals_host)
        l.rec = reinterpret<float const?>(r.sel_rec_host)
    }
    return l
}

//! RdecSelLandRowsFn: the rows the selection stood in for, off the device's logits plane (intact until the next command)
[arch(at="../ARCHITECTURE_GPU_VULKAN_LANDING.md#logits-selection-landing")]
def cud_rdec_sel_land_rows(row0, nrows : int64; var dst : array<float>) {
    verify(g_crd != null && g_crd.sel_landed > 0l && row0 + nrows <= g_crd.sel_landed && long_length(dst) >= g_crd.vocab * nrows, "cud_rdec_sel_land_rows: no selection landed these rows, or the destination is short")
    let r & = unsafe(*g_crd)
    let dp = unsafe(addr<void?>(dst[0]))
    cuda_memcpy_dtoh(dp, r.logits + uint64(row0 * r.vocab * 4l), uint64(nrows * r.vocab * 4l), g_cud.stream)
    verify(cud_sync(), "cud decode: the late logits landing did not land")
}

//! RdecTokenSeqFn: the short window's peel on a recurrent model - `nrows` consecutive rows of the selected region from `pos` through
//! the one-row step, each row's ring image the one the row before it wrote, the last row's logits landed
[arch(at="../ARCHITECTURE_GPU_VULKAN_WINDOW.md#vk-prefill-peel")]
def cud_rdec_token_seq(x : array<float>; cossin : array<float>; pos, nrows : int64; var logits : array<float>) {
    crd_chain_settle()
    assert(g_crd != null && g_crd.ready, "cud_rdec_token_seq before prepare/set_cls")
    var r & = unsafe(*g_crd)
    verify(nrows >= 2l && nrows <= r.nb && long_length(x) >= r.dim * nrows && long_length(cossin) >= r.cos_elems * nrows && long_length(logits) >= r.vocab,
        "cud_rdec_token_seq: the row count is outside the per-row planes, the rows short of it, or the logits row short")
    for (i in range64(nrows)) {
        crd_step(r, x, i * r.dim, cossin, i * r.cos_elems, pos + i, pos + i + 1l, -1l, i == nrows - 1l)
    }
    crd_logits_out(r, logits)
    r.pf_head_rows = 0l   // the in-order rows warm no head row
}

//! RdecKvXferFn: every span's rows between the host cache and the selected region's mirror as one run of copies on the stream, one wait at the end
[arch(at="../ARCHITECTURE_ENGINE_SERVING.md#device-home-pool")]
def cud_rdec_kv_xfer(var spans : array<RdecKvSpan>; up : bool) : bool {
    crd_chain_settle()
    return false if (g_crd == null || !g_crd.ready)
    for (sp in spans) {
        if (up) {
            cud_rdec_kv_sync(sp.l, sp.row0, sp.nrows, sp.kp, sp.vp)
        } else {
            crd_kv_read_queue(sp.l, sp.row0, sp.nrows, sp.kp, sp.vp)
        }
    }
    verify(cud_sync(), "cud decode: the pool's K/V move did not land")
    return true
}

//! the plan's device weight bytes (`rdec_note_weight_bytes`'s seat): the first token's wall is read against them
def cud_rdec_note_weight_bytes(bytes : int64) {
    if (g_crd != null) {
        g_crd.weight_bytes = bytes
    }
}

def private crd_no_host(l : int64) {
    panic("dasLLAMA cuda resident: a host layer's cut at layer {l} on a command with no host callback (the token seat serves a model with no host layers)")
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
    let chain = r.ch.ask
    r.ch.ask = false
    let chained = chain && crd_chain_ready(r, pos + 1l, cnt, logits_on, gathered)
    crd_launch(r, CrdCmd.token, 1l, logits_on, gathered, cnt, !chained) $(l : int64) {
        crd_no_host(l)
    }
    if (r.hybrid) {
        crd_dn_advance(r, r.region)
    }
    crd_served(r, 1l)
    if (gathered) {
        r.tok_gathered++
    }
    if (chained) {   // the next step queues behind this one on the stream before this one's landing is waited
        crd_chain_submit(r, pos + 1l, logits_on)
        verify(cuda_event_synchronize(r.ch_landed[r.ch.slot]), "cud decode: the running step's landing did not sync")
        r.ch.pred = crd_landed_pick(r)
    }
}

//! the out block's post-norm row `row` of the last command into `dst` (`RdecLandCarryFn`)
[arch(at="../ARCHITECTURE_GPU_VULKAN_LANDING.md#logits-transfer-queue")]   //! per token, reached through the tier's registered seat
def cud_rdec_land_carry(row : int64; var dst : array<float>) {
    crd_chain_settle()
    assert(g_crd != null && g_crd.carry, "cud_rdec_land_carry on a driver prepared without the carry")
    let r & = unsafe(*g_crd)
    assert(row >= 0l && row < r.nb && long_length(dst) >= r.dim, "cud_rdec_land_carry: the row is past the landed rows, or the destination is short of a row")
    unsafe {
        var w = reinterpret<float?>(r.out_host)
        memcpy(addr<void?>(dst[0]), addr<void?>(w[r.out_hid_word + row * r.dim]), r.dim * 4l)
    }
}

//! the head rows' token blocks and rope rows into the in block's head slots: head row i at position `pos0 + i` of the selected region
def private crd_stage_head_rows(r : CudRdec; hcos : array<float>; pos0, nrows : int64) {
    unsafe {
        var w = reinterpret<uint?>(r.in_host)
        var hm = reinterpret<TokMeta?>(addr<void?>(w[CRD_IN_HTOK_WORD]))
        for (i in range64(nrows)) {
            hm[i] = TokMeta(pos = uint(pos0 + i), cnt = uint(pos0 + i + 1l), parity = 0u, mirbase = uint(r.region * r.mir_units), dnslot = uint(r.region))
        }
        memcpy(addr<void?>(w[crd_in_hcos_word(r)]), addr<void?>(hcos[0]), r.cos_elems * 4l * nrows)
    }
}

//! one NextN draft at head row `row` of the selected region (`RdecDraftFn`): the host head rows [hyd0, row) come up, the token's id
//! (or its row) and the carry go into the in block's head slots, the draft command runs, the pick and the asked logits come off the
//! out block; the hidden lands as the carry's row 0
[unused_argument(experts), arch(at="../ARCHITECTURE_GPU_VULKAN_MTP.md#resident-draft-head")]   //! per draft, reached through the tier's registered seat
def cud_rdec_draft(x : array<float>; token : int64; h : array<float>; cossin : array<float>; row, hyd0 : int64; var kp : uint8?; var vp : uint8?; pick_only : bool; var logits : array<float>; experts : RdecHostExpertsFn) : int64 {
    crd_chain_settle()
    return -1l if (g_crd == null || !g_crd.ready || !g_crd.head_on || !g_crd.carry)
    var r & = unsafe(*g_crd)
    assert(hyd0 >= 0l && row >= 0l, "cud_rdec_draft: a negative head row")
    let gathered = token >= 0l && r.emb_blk >= 0l
    verify(gathered || long_length(x) >= r.dim, "cud_rdec_draft: neither a token id the device gathers nor a row")
    cud_rdec_kv_sync(r.n_layers, hyd0, row - hyd0, kp, vp)
    unsafe {
        var w = reinterpret<uint?>(r.in_host)
        w[CRD_IN_ID_WORD] = gathered ? uint(token) : 0u
        if (!gathered) {
            memcpy(addr<void?>(w[crd_in_hemb_word(r)]), addr<void?>(x[0]), r.dim * 4l)
        }
        memcpy(addr<void?>(w[crd_in_hrow_word(r)]), addr<void?>(h[0]), r.dim * 4l)
    }
    crd_stage_head_rows(r, cossin, row, 1l)
    crd_launch(r, CrdCmd.draft, 1l, !pick_only, gathered, row + 1l, true) $(l : int64) {
        crd_no_host(l)
    }
    r.drafts++
    if (pick_only) {
        return crd_landed_pick(r)
    }
    crd_logits_out(r, logits)
    return crd_landed_pick(r)
}

//! the drafts the driver served in this process
def cud_rdec_drafts : int64 => g_crd != null ? g_crd.drafts : 0l

//! the same-slab verifies the driver served in this process
def cud_rdec_verifies : int64 => g_crd != null ? g_crd.verifies : 0l

//! the rows a same-slab verify steps at once (`install_rdec_verify`'s `rows` seat): the depth plus one the driver was prepared for,
//! 0 where the head is off the device
[arch(at="../ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-verify-command")]
def cud_rdec_verify_rows : int64 {
    return 0l if (g_crd == null || !g_crd.ready || g_crd.verify_rows < 2l)
    var r & = unsafe(*g_crd)
    if (!r.head_on || !r.carry) {
        if (!r.v_said) {
            r.v_said = true
            rdec_v_decline("the NextN head is off the device (the load log names why)")
        }
        return 0l
    }
    return r.verify_rows
}

//! Whether the last served verify landed the rows' logits (the tier's `landed_logits` seat)
def cud_rdec_verify_landed_logits : bool => g_crd != null && g_crd.verify_landed_logits

//! the same-slab verify of `n` rows at `pos` in the selected region (`RdecVerifyFn`): the rows' token blocks at their positions, the
//! head rows' a position below, both rope sets, the rows (or their ids) and the pre-draft carry into the in block, the verify command,
//! then every row's pick, hidden and - unless `pick_only` - logits off the out block; a hybrid's region moves on `n` rows
[unused_argument(side), arch(at="../ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-verify-command"), arch(at="../ARCHITECTURE_GPU_VULKAN_HC.md#hc-verify-rows")]   //! per speculative round, reached through the tier's registered seat
def cud_rdec_verify(x, cossin, hemb, hcos, h0, side : array<float>; ids : array<int64>; pos, n : int64; pick_only : bool; var logits : array<float>; var picks : array<int64>; var hid : array<float>; experts : RdecHostExpertsRowsFn) : bool {
    crd_chain_settle()
    return false if (n < 2l || n > cud_rdec_verify_rows())
    var r & = unsafe(*g_crd)
    let dim = r.dim
    verify(pos >= 1l && long_length(logits) >= n * r.vocab && long_length(picks) >= n && long_length(hid) >= n * dim,
        "cud_rdec_verify: a verify below row 1 (the head's first row is pos - 1), or a landing short of its rows")
    let gathered = r.emb_blk >= 0l && long_length(ids) >= n
    verify(gathered || (long_length(x) >= dim * n && long_length(hemb) >= dim * n), "cud_rdec_verify: the rows are short, and the device gathers no row")
    let reg = r.region
    let par = r.hybrid ? r.dn_parity[reg] : 0
    unsafe {
        var w = reinterpret<uint?>(r.in_host)
        var tm = reinterpret<TokMeta?>(r.in_host)
        for (i in range64(n)) {
            tm[i] = TokMeta(pos = uint(pos + i), cnt = uint(pos + i + 1l), mirbase = uint(reg * r.mir_units),
                parity = r.hybrid ? uint(par ^ int(i & 1l)) : 0u, dnslot = uint(reg))
            w[int64(CRD_IN_ID_WORD) + i] = gathered ? uint(ids[i]) : 0u
        }
        memcpy(addr<void?>(w[CRD_IN_HDR_WORDS]), addr<void?>(cossin[0]), r.cos_elems * 4l * n)
        if (!gathered) {
            memcpy(addr<void?>(w[int64(CRD_IN_HDR_WORDS) + r.cos_elems * n]), addr<void?>(x[0]), dim * 4l * n)
            memcpy(addr<void?>(w[crd_in_hemb_word(r)]), addr<void?>(hemb[0]), dim * 4l * n)
        }
        memcpy(addr<void?>(w[crd_in_hrow_word(r)]), addr<void?>(h0[0]), dim * 4l)
    }
    crd_stage_head_rows(r, hcos, pos - 1l, n)
    r.verify_landed_logits = !pick_only
    crd_launch(r, CrdCmd.verify, n, !pick_only, gathered, pos + n, true) $(l : int64) {
        unsafe {
            invoke(experts, l, reinterpret<float const?>(r.hx_host), reinterpret<float const?>(r.hlog_host), reinterpret<float?>(r.hacc_host), 0l, n)
        }
    }
    unsafe {
        let pk = reinterpret<uint?>(r.out_host)
        let lw = reinterpret<float?>(r.out_host)
        for (i in range64(n)) {
            if (int64(pk[i]) >= r.vocab) {
                panic("dasLLAMA cuda resident: the device landed no pick for verify row {i} ({pk[i]} of a {r.vocab} vocab)")
            }
            picks[i] = int64(pk[i])
        }
        if (!pick_only) {
            memcpy(addr<void?>(logits[0]), addr<void?>(lw[CRD_OUT_LOGITS_WORDS]), r.vocab * 4l * n)
        }
        memcpy(addr<void?>(hid[0]), addr<void?>(lw[r.out_hid_word]), dim * 4l * n)
    }
    if (r.hybrid) {
        for (_i in range64(n)) {
            crd_dn_advance(r, reg)
        }
        r.roll_region = r.roll_dev != 0ul ? reg : -1l
        r.roll_vrows = n
    }
    crd_served(r, n)
    r.verifies++
    return true
}

//! the same-slab verify's reject (`RdecRollbackFn`): every recurrent slot and ring pair of the selected region back to the copy the
//! verify command took after its row `a`, the region's ring parity back to row `a`'s
[arch(at="../ARCHITECTURE_GPU_VULKAN_MTP.md#resident-verify-rollback")]   //! per rejected round, reached through the tier's registered seat
def cud_rdec_rollback(a : int64) : bool {
    crd_chain_settle()
    return false if (g_crd == null || !g_crd.ready || !g_crd.hybrid || g_crd.roll_dev == 0ul)
    var r & = unsafe(*g_crd)
    let reg = r.region
    return false if (reg != r.roll_region || a < 0l || a >= r.roll_rows || a + 1l >= r.roll_vrows)
    let src = r.roll_dev + uint64(a * r.roll_row_bytes)
    for (L in r.layers) {
        continue if (!L.recurrent)
        let st & = unsafe(L.dn[reg])
        cuda_memcpy_dtod(st.state_dev + uint64(st.state_byte_off), src + uint64(L.roll_off), uint64(st.slot_state_bytes), g_cud.stream)
        cuda_memcpy_dtod(st.smalls_dev + uint64(st.hist_float_off * 4l), src + uint64(L.roll_off + st.slot_state_bytes), uint64(2l * st.ring_image_bytes), g_cud.stream)
    }
    verify(cud_sync(), "cud decode: the rollback's copies did not land")
    for (_i in range64(r.roll_vrows - a - 1l)) {
        crd_dn_advance(r, reg)
    }
    r.roll_region = -1l
    return true
}

//! the selected region's recurrent state - every slot and ring pair - into its checkpoint row `slot`, the region's parity beside it
[cold_path, arch(at="../ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#device-home-checkpoints")]   //! once per checkpoint stop, reached through the tier's registered seat
def cud_rdec_dn_snap_save(slot : int64) : bool {
    crd_chain_settle()
    return false if (g_crd == null || !g_crd.ready || !g_crd.hybrid || slot < 0l || slot >= int64(RDEC_DN_SNAPS))
    var r & = unsafe(*g_crd)
    crd_snap_alloc(r)
    return false if (r.snap_dev == 0ul)
    let reg = r.region
    let row = reg * int64(RDEC_DN_SNAPS) + slot
    let dst = r.snap_dev + uint64(row * r.snap_row_bytes)
    for (L in r.layers) {
        continue if (!L.recurrent)
        let st & = unsafe(L.dn[reg])
        cuda_memcpy_dtod(dst + uint64(L.snap_off), st.state_dev + uint64(st.state_byte_off), uint64(st.slot_state_bytes), g_cud.stream)
        cuda_memcpy_dtod(dst + uint64(L.snap_off + st.slot_state_bytes), st.smalls_dev + uint64(st.hist_float_off * 4l), uint64(2l * st.ring_image_bytes), g_cud.stream)
    }
    r.snap_par[row] = r.dn_parity[reg]
    return true
}

//! checkpoint row `slot` back into the selected region's slots and ring pairs, the region's parity the row's
[cold_path, arch(at="../ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#device-home-checkpoints")]   //! once per adoption, reached through the tier's registered seat
def cud_rdec_dn_snap_load(slot : int64) : bool {
    crd_chain_settle()
    return false if (g_crd == null || !g_crd.ready || !g_crd.hybrid || g_crd.snap_dev == 0ul || slot < 0l || slot >= int64(RDEC_DN_SNAPS))
    var r & = unsafe(*g_crd)
    let reg = r.region
    let row = reg * int64(RDEC_DN_SNAPS) + slot
    let src = r.snap_dev + uint64(row * r.snap_row_bytes)
    for (L in r.layers) {
        continue if (!L.recurrent)
        let st & = unsafe(L.dn[reg])
        cuda_memcpy_dtod(st.state_dev + uint64(st.state_byte_off), src + uint64(L.snap_off), uint64(st.slot_state_bytes), g_cud.stream)
        cuda_memcpy_dtod(st.smalls_dev + uint64(st.hist_float_off * 4l), src + uint64(L.snap_off + st.slot_state_bytes), uint64(2l * st.ring_image_bytes), g_cud.stream)
    }
    if (r.dn_parity[reg] != r.snap_par[row]) {
        crd_dn_advance(r, reg)
    }
    return true
}

//! a copy of every listed recurrent layer's state and ring in the selected region into the host rows `dsts` name - the live slots
//! (`slot` -1), the slots as a parked claim left them with their owner gone (-2), or checkpoint row `slot` (`RdecDnPeekFn`); the
//! region left as it stands - the host pool's copy of a device-home stream's state
[cold_path, arch(at="../ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#device-home-checkpoints")]
def cud_rdec_dn_peek(slot : int64; dsts : array<RdecDnDst>) : bool {
    crd_chain_settle()
    return false if (g_crd == null || !g_crd.ready || !g_crd.hybrid || empty(dsts) || slot >= int64(RDEC_DN_SNAPS) || (slot >= 0l && g_crd.snap_dev == 0ul))
    let r & = unsafe(*g_crd)
    let reg = r.region
    let row = reg * int64(RDEC_DN_SNAPS) + slot
    for (d in dsts) {
        return false if (d.l < 0l || d.l >= long_length(r.layers) || !r.layers[d.l].recurrent || (slot == -1l && !r.layers[d.l].dn[reg].own.valid))
    }
    for (d in dsts) {
        let L & = unsafe(r.layers[d.l])
        let st & = unsafe(L.dn[reg])
        unsafe {
            if (slot < 0l) {
                cuda_memcpy_dtoh(reinterpret<void?>(d.stp), st.state_dev + uint64(st.state_byte_off), uint64(st.slot_state_bytes), g_cud.stream)
                cuda_memcpy_dtoh(reinterpret<void?>(d.histp), st.smalls_dev + uint64(st.hist_float_off * 4l + int64(st.own.parity) * st.ring_image_bytes), uint64(st.ring_image_bytes), g_cud.stream)
            } else {
                let src = r.snap_dev + uint64(row * r.snap_row_bytes + L.snap_off)
                cuda_memcpy_dtoh(reinterpret<void?>(d.stp), src, uint64(st.slot_state_bytes), g_cud.stream)
                cuda_memcpy_dtoh(reinterpret<void?>(d.histp), src + uint64(st.slot_state_bytes + int64(r.snap_par[row]) * st.ring_image_bytes), uint64(st.ring_image_bytes), g_cud.stream)
            }
        }
    }
    verify(cud_sync(), "cud decode: the state peek did not land - the caller reads the host rows as soon as this returns")
    return true
}

def private crd_tok_parity(r : CudRdec; region : int64) : uint => r.hybrid ? uint(r.dn_parity[region]) : 0u

var private g_cud_chain_pin = RDEC_CHAIN_ENV   //! unset: the mode is the environment's (`rdec_chain_mode`)
var private g_crd_chain_hits = 0l
var private g_crd_chain_misses = 0l

//! pins the greedy chain's mode (`DASLLAMA_CUDA_CHAIN`): 0 off, 1 every greedy step, -1 adaptive
def cud_rdec_chain_mode_set(mode : int) {
    g_cud_chain_pin = mode
    set_rdec_chain_charged(cud_rdec_chain_mode() != 0)
    cud_rdec_chain_announce()
}

//! the chain's mode: the pin, else the environment's; `cud_rdec_chain_mode_set(cud_rdec_chain_pin())` puts a pin back as found
def cud_rdec_chain_mode : int => rdec_chain_mode(g_cud_chain_pin, g_env_vulkan.cuda_chain)

def cud_rdec_chain_pin : int => g_cud_chain_pin

//! the greedy chain's announce, where its mode moved off the default off: the knob by the spelling that set it, the form, and the
//! steps the value never reaches (`cud_rdec_chain_wanted`, `crd_chain_ready`)
def cud_rdec_chain_announce {
    let mode = cud_rdec_chain_mode()
    return if (mode == 0)
    let pinned = g_cud_chain_pin != RDEC_CHAIN_ENV
    let knob = pinned ? "cud_rdec_chain_mode_set({mode})" : "DASLLAMA_CUDA_CHAIN={mode}"
    let form = mode < 0 ? "adaptively, backing off after a miss" : "on every greedy step"
    to_log(LOG_INFO, "dasLLAMA cuda tier: {knob} - the greedy chain runs {form}; it stands down whatever the value on a host-experts model, a multi-segment command, a round's draft-head carry and a per-layer-embedding model without the side gather\n")
}

//! the chained steps taken (hits) and those a wrong prediction at their own step discarded (misses) in this process; a chain
//! dropped at a switch or a settle counts as neither
def cud_rdec_chain_stats : tuple<hits : int64; misses : int64> => (hits = g_crd_chain_hits, misses = g_crd_chain_misses)

//! RdecChainWantedFn: the one-row command gathers its row and side row off the token id, no carry is live for a round's draft head
//! and no routed layer's experts serve on the host (the split command's segments stand between the steps)
[arch(at="../ARCHITECTURE_GPU_VULKAN_LANDING.md#vk-greedy-chain")]
def cud_rdec_chain_wanted : bool {
    return false if (g_crd == null || !g_crd.ready || cud_rdec_chain_mode() == 0)
    let r & = unsafe(*g_crd)
    return crd_emb_on(r) && (r.ple == 0l || (r.ple_gpu && crd_side_gather_on(r))) && !(r.carry && get_mtp_spec()) && !r.experts_host
}

//! RdecChainAskFn: the next step is a greedy one at the next position, its rope row staged for the chain
[arch(at="../ARCHITECTURE_GPU_VULKAN_LANDING.md#vk-greedy-chain")]
def cud_rdec_chain_ask(cos_next : array<float>) {
    return if (g_crd == null)
    var r & = unsafe(*g_crd)
    r.ch.ask = true
    r.ch.cos_next |> grow_resize(r.cos_elems)
    copy_floats(cos_next, 0l, r.ch.cos_next, 0l, r.cos_elems)
}

//! the chain's two staging and landing slots, their events and the recurrent states' snapshot, once a driver (the caller holds it to
//! the first chained step)
[cold_path]
def private crd_chain_alloc(var r : CudRdec) {
    let stage_bytes = (int64(CRD_TOK_WORDS) + r.cos_elems) * 4l
    let land_bytes = (int64(CRD_OUT_LOGITS_WORDS) + r.vocab) * 4l
    r.ch_stage_host |> reserve(2)
    r.ch_stage_dev |> reserve(2)
    r.ch_land_host |> reserve(2)
    r.ch_land_dev |> reserve(2)
    r.ch_landed |> reserve(2)
    r.ch_done |> reserve(2)
    for (_s in range(2)) {
        let sb = cuda_mem_alloc_host_mapped(uint64(stage_bytes))
        let lb = cuda_mem_alloc_host_mapped(uint64(land_bytes))
        if (sb.host == null || lb.host == null) {
            panic("dasLLAMA cuda resident: the greedy chain's mapped slots did not allocate ({cuda_last_error()})")
        }
        r.ch_stage_host |> push(sb.host)
        r.ch_stage_dev |> push(sb.dev)
        r.ch_land_host |> push(lb.host)
        r.ch_land_dev |> push(lb.dev)
        r.ch_landed |> push(cuda_event_create_untimed())
        r.ch_done |> push(cuda_event_create_untimed())
    }
    var snap = 0l
    for (L in r.layers) {
        snap += L.recurrent ? L.dn[0].slot_state_bytes : 0l
    }
    if (snap > 0l) {
        r.ch_snap_dev = cud_dev_zeroed(snap)
        if (r.ch_snap_dev == 0ul) {
            panic("dasLLAMA cuda resident: the greedy chain's state snapshot did not allocate ({cuda_last_error()})")
        }
    }
}

//! the chain's conditions at this step: the row gathered, the mode on and not backing off, a step past the first (its graph captured),
//! one segment, no selection asked, and both this step's and the next step's graphs captured - the chained step launches a graph alone
def private crd_chain_ready(var r : CudRdec; pos_next, cnt : int64; logits_on, gathered : bool) : bool {
    return false if (!gathered || r.token_steps < 1l || r.sel_ask || r.experts_host)
    let mode = cud_rdec_chain_mode()
    return false if (mode == 0)
    if (mode < 0 && r.ch.backoff > 0) {
        r.ch.backoff--
        return false
    }
    if (r.nseg == 0l) {
        crd_segments_setup(r)
    }
    return false if (r.nseg != 1l)
    let now_unsplit = r.unsplit_on && cnt < RD_UNSPLIT_POS
    let next_unsplit = r.unsplit_on && pos_next + 1l < RD_UNSPLIT_POS
    return (key_exists(r.graphs, crd_graph_key(CrdCmd.token, 0l, 1l, 0l, logits_on, true, now_unsplit))
        && key_exists(r.graphs, crd_graph_key(CrdCmd.token, 0l, 1l, 0l, logits_on, true, next_unsplit)))
}

//! the region's recurrent state slots in layer order with each one's offset in the chain's snapshot
def private crd_chain_snap_walk(r : CudRdec; region : int64; blk : block<(st : CudDnSlot; off : int64) : void>) {
    var off = 0l
    for (L in r.layers) {
        continue if (!L.recurrent)
        invoke(blk, L.dn[region], off)
        off += L.dn[region].slot_state_bytes
    }
}

//! the next one-row step queued behind the running one on the stream: the prologue (`crd_chain_pro_enc`; the host reads the
//! running step's results in landing slot `slot` once `ch_landed` fires), then the step's own graph at `pos_next`, `ch_done`
//! after it; the host rewrites nothing the chained step reads
[arch(at="../ARCHITECTURE_GPU_VULKAN_LANDING.md#vk-greedy-chain")]
def private crd_chain_submit(var r : CudRdec; pos_next : int64; logits_on : bool) {
    if (empty(r.ch_stage_host)) {
        crd_chain_alloc(r)
    }
    let slot = r.ch.next_slot
    r.ch.next_slot = 1 - slot
    let s = g_cud.stream
    let reg = r.region
    unsafe {
        var tm = reinterpret<TokMeta?>(r.ch_stage_host[slot])
        tm[0] = TokMeta(pos = uint(pos_next), cnt = uint(pos_next + 1l), mirbase = uint(reg * r.mir_units), parity = crd_tok_parity(r, reg), dnslot = uint(reg))
        memcpy(crd_slot_host(r.ch_stage_host[slot], int64(CRD_TOK_WORDS)), addr<void?>(r.ch.cos_next[0]), r.cos_elems * 4l)
    }
    let pkey = (int(reg) << 3) | (slot << 1) | (logits_on ? 1 : 0)
    if (!key_exists(r.ch_pro, pkey)) {   // the prologue's copies as one graph: one launch where a recurrent model's states would be a copy a layer
        let g = cuda_graph_capture(s) {
            crd_chain_pro_enc(r, slot, logits_on, s)
        }
        if (g == null) {
            panic("dasLLAMA cuda resident: the greedy chain's prologue did not capture: {cuda_last_error()}")
        }
        r.ch_pro[pkey] = g
    }
    if (!cuda_graph_launch(r.ch_pro[pkey], s)) {
        panic("dasLLAMA cuda resident: the greedy chain's prologue did not launch: {cuda_last_error()}")
    }
    verify(cuda_event_record(r.ch_landed[slot], s), "cud decode: the chain's landing event did not record")
    r.ch.landed = slot + 1
    let unsplit = r.unsplit_on && pos_next + 1l < RD_UNSPLIT_POS
    let key = crd_graph_key(CrdCmd.token, 0l, 1l, 0l, logits_on, true, unsplit)
    if (!cuda_graph_launch(r.graphs[key], s)) {
        panic("dasLLAMA cuda resident: the chained token command did not launch: {cuda_last_error()}")
    }
    verify(cuda_event_record(r.ch_done[slot], s), "cud decode: the chain's done event did not record")
    r.token_steps++
    if (unsplit) {
        r.unsplit_steps++
    }
    r.sel_landed = 0l
    r.ch.pend = true
    r.ch.region = reg
    r.ch.pos = pos_next
    r.ch.slot = slot
    r.ch.logits = logits_on
    r.ch.pred = -1l
}

//! the prologue's copies on `s`: the running step's pick off the out block and, where it landed logits, its logits plane into
//! landing slot `slot`; the pick into the in block's token id word, the staged TokMeta block and rope row into the in block; the
//! region's recurrent states into the snapshot. Captured once a (region, slot, logits) key: every address is the driver's own
def private crd_chain_pro_enc(r : CudRdec; slot : int; logits_on : bool; s : void?) {
    cuda_memcpy_dtod(r.ch_land_dev[slot], r.pick, 4ul, s)
    if (logits_on) {
        cuda_memcpy_dtoh(crd_slot_host(r.ch_land_host[slot], int64(CRD_OUT_LOGITS_WORDS)), r.logits, uint64(r.vocab * 4l), s)
    }
    cuda_memcpy_dtod(r.in_dev + uint64(CRD_IN_ID_WORD * 4), r.pick, 4ul, s)
    cuda_memcpy_dtod(r.in_dev, r.ch_stage_dev[slot], uint64(CRD_TOK_WORDS * 4), s)
    cuda_memcpy_dtod(r.in_dev + uint64(CRD_IN_HDR_WORDS * 4), r.ch_stage_dev[slot] + uint64(CRD_TOK_WORDS * 4), uint64(r.cos_elems * 4l), s)
    if (r.ch_snap_dev != 0ul) {
        crd_chain_snap_walk(r, r.region) $(st, off) {
            cuda_memcpy_dtod(r.ch_snap_dev + uint64(off), st.state_dev + uint64(st.state_byte_off), uint64(st.slot_state_bytes), s)
        }
    }
}

//! word `word` of a mapped slot
def private crd_slot_host(base : void?; word : int64) : void? {
    unsafe {
        return reinterpret<void?>(reinterpret<uint8?>(base) + word * 4l)
    }
}

//! the pending chained step dropped (the caller holds it to one in flight). `miss`: the chained step predicted another token at its
//! own region and position; a chain dropped at a switch or a settle backs off nothing. The step is waited out and the region's
//! recurrent states put back from the snapshot: the ring images alternate by parity, so the rerun reads and writes the images the
//! chained step did, and stores the same K/V row again
[cold_path, arch(at="../ARCHITECTURE_GPU_VULKAN_LANDING.md#vk-greedy-chain")]
def private crd_chain_drop(var r : CudRdec; miss : bool) {
    r.ch.pend = false
    verify(cuda_event_synchronize(r.ch_done[r.ch.slot]), "cud decode: the chained step did not sync")
    if (r.ch_snap_dev != 0ul) {
        let reg = r.ch.region
        crd_chain_snap_walk(r, reg) $(st, off) {
            cuda_memcpy_dtod(st.state_dev + uint64(st.state_byte_off), r.ch_snap_dev + uint64(off), uint64(st.slot_state_bytes), g_cud.stream)
        }
        verify(cud_sync(), "cud decode: the chain's snapshot did not restore")
    }
    r.ch.landed = 0
    return if (!miss)
    g_crd_chain_misses++
    if (cud_rdec_chain_mode() < 0) {
        rdec_chain_backoff(r.ch.backoff_len, r.ch.backoff)
    }
}

//! every other driver entry settles a chained step first: the ask dropped, the step waited out and the states put back
[arch(at="../ARCHITECTURE_GPU_VULKAN_LANDING.md#vk-greedy-chain")]
def private crd_chain_settle {
    cud_bind()   // every seat but the chained token step and the hidden rows' read opens here: the calling thread bound first
    return if (g_crd == null)
    var r & = unsafe(*g_crd)
    r.ch.ask = false
    if (r.ch.pend) {
        crd_chain_drop(r, false)
    }
}

//! the chained step taken as this step: the next chain queued behind it before its landing is waited where asked, its pick and
//! logits read off the landing slot (the out block where no further chain copied them aside)
[arch(at="../ARCHITECTURE_GPU_VULKAN_LANDING.md#vk-greedy-chain")]
def private crd_chain_take(var r : CudRdec; pos : int64; pick_only : bool; var logits : array<float>) : int64 {
    let slot = r.ch.slot
    r.ch.pend = false
    g_crd_chain_hits++
    r.ch.backoff_len = 0
    if (r.hybrid) {
        crd_dn_advance(r, r.region)
    }
    crd_served(r, 1l)
    r.tok_gathered++
    if (r.ple > 0l && crd_side_gather_on(r)) {
        r.side_gathered++
    }
    let chain = r.ch.ask
    r.ch.ask = false
    if (chain && crd_chain_ready(r, pos + 1l, pos + 1l, !pick_only, true)) {
        crd_chain_submit(r, pos + 1l, !pick_only)
        verify(cuda_event_synchronize(r.ch_landed[r.ch.slot]), "cud decode: the chained step's landing did not sync")
        let pick = crd_landed_pick(r)
        r.ch.pred = pick
        if (!pick_only) {
            crd_logits_out(r, logits)
        }
        return pick_only ? pick : -1l
    }
    verify(cuda_event_synchronize(r.ch_done[slot]), "cud decode: the chained step did not sync")
    r.ch.landed = 0
    let pick = crd_landed_pick(r)
    if (!pick_only) {
        crd_logits_out(r, logits)
    }
    return pick_only ? pick : -1l
}


//! word `word` of the last step's results: the out block, or the landing slot the chain copied them aside into
def private crd_landed_host(r : CudRdec; word : int64) : void? {
    let base = r.ch.landed == 0 ? r.out_host : r.ch_land_host[r.ch.landed - 1]
    unsafe {
        return reinterpret<void?>(reinterpret<uint8?>(base) + word * 4l)
    }
}

def private crd_landed_pick(r : CudRdec) : int64 {
    let pk = *unsafe(reinterpret<uint?>(crd_landed_host(r, 0l)))
    if (int64(pk) >= r.vocab) {
        panic("dasLLAMA cuda resident: the device landed no pick ({pk} of a {r.vocab} vocab)")
    }
    return int64(pk)
}

//! the out block's logits at `slot` into `logits` from `at`
def private crd_logits_out(r : CudRdec; var logits : array<float>; slot : int64 = 0l; at : int64 = 0l) {
    unsafe {
        var w = reinterpret<uint?>(crd_landed_host(r, 0l))
        memcpy(addr<void?>(logits[at]), addr<void?>(w[int64(CRD_OUT_LOGITS_WORDS) + slot * r.vocab]), r.vocab * 4l)
    }
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_LANDING.md#vk-greedy-chain")]
def cud_rdec_token(x : array<float>; cossin : array<float>; ple_row : array<float>; token, pos, cnt : int64; pick_only : bool; var logits : array<float>) : int64 {
    assert(g_crd != null && g_crd.ready, "cud_rdec_token before prepare/set_cls")
    cud_bind()
    var r & = unsafe(*g_crd)
    if (r.ch.pend) {
        let same_step = r.ch.region == r.region && r.ch.pos == pos && cnt == pos + 1l
        if (same_step && token >= 0l && token == r.ch.pred && (pick_only || r.ch.logits) && !r.sel_ask) {
            return crd_chain_take(r, pos, pick_only, logits)
        }
        crd_chain_drop(r, same_step)
    }
    crd_ple_stage(r, ple_row, 1l, token >= 0l)
    verify(token >= 0l || long_length(x) >= r.dim, "cud decode: neither a token id nor a row")
    verify(token < 0l || crd_emb_on(r), "cud decode: a token id with no table on the device - the engine asks the gather only past set_emb or the f32 upload")
    let first = r.token_steps == 0l
    let t0 = ref_time_ticks()   // clock: control - the first token's wall against the paging bound decides a warning, never a reported figure
    crd_step(r, x, 0l, cossin, 0l, pos, cnt, token, !pick_only)
    if (first) {
        rdec_paging_check(r.weight_bytes, int64(get_time_usec(t0)))
    }
    if (pick_only) {
        return crd_landed_pick(r)
    }
    crd_logits_out(r, logits)
    return -1l
}

//! the split token (`RdecTokenSplitFn`): the one-row step of a model with host layers - the command's segments with the host's routed
//! sum between them; `side` (the n-gram side input) is the hyper-connection chain's, which this home declines at its planes
[unused_argument(side), arch(at="../ARCHITECTURE_GPU_VULKAN_HC.md#hc-token-command")]
def cud_rdec_token_split(x : array<float>; cossin : array<float>; side : array<float>; pos, cnt : int64; pick_only : bool; var logits : array<float>; experts : RdecHostExpertsFn) : int64 {
    crd_chain_settle()
    assert(g_crd != null && g_crd.ready && g_crd.experts_host, "cud_rdec_token_split on a model with no host layers")
    var r & = unsafe(*g_crd)
    verify(long_length(x) >= r.dim, "cud decode: the split step brings no row")
    unsafe {   // the engine embeds the split step's row on the host: the row rides the in block, as the one-row command's does without the table
        var w = reinterpret<uint?>(r.in_host)
        var tm = reinterpret<TokMeta?>(r.in_host)
        *tm = TokMeta(pos = uint(pos), cnt = uint(cnt), mirbase = uint(r.region * r.mir_units), parity = crd_tok_parity(r, r.region), dnslot = uint(r.region))
        w[CRD_IN_ID_WORD] = 0u
        memcpy(addr<void?>(w[CRD_IN_HDR_WORDS]), addr<void?>(cossin[0]), r.cos_elems * 4l)
        memcpy(addr<void?>(w[int64(CRD_IN_HDR_WORDS) + r.cos_elems]), addr<void?>(x[0]), r.dim * 4l)
    }
    crd_launch(r, CrdCmd.token, 1l, !pick_only, false, cnt, true) $(l : int64) {
        unsafe {
            invoke(experts, l, reinterpret<float const?>(r.hx_host), reinterpret<float const?>(r.hlog_host), reinterpret<float?>(r.hacc_host))
        }
    }
    if (r.hybrid) {
        crd_dn_advance(r, r.region)
    }
    crd_served(r, 1l)
    if (pick_only) {
        return crd_landed_pick(r)
    }
    crd_logits_out(r, logits)
    return -1l
}

//! routed layer `l`'s hot expert pool (`RdecHotLayerFn`): `slots` slots of the gate, up and down planes in their formats as device
//! stacks of their own (a slot an expert's blocks), the hit chain's records and weights shared by every layer; `base` under 0 =
//! no pool on this layer; the offset answered is nominal - this home has no window chain over the pool, so nothing reads it
[arch(at="../ARCHITECTURE_GPU_VULKAN_POOL.md#hc-hot-pool")]
def cud_rdec_hot_layer(l, slots : int64; f1, f3, f2 : int) : tuple<base : int64; stride : int64> {
    let none = (base = -1l, stride = 0l)
    return none if (g_cud.dry)   // the mint's walk arms no pool: the mapped lane's live walk fills it
    assert(g_crd != null && g_crd.experts_host && g_crd.moe, "cud_rdec_hot_layer: after the MoE prepare of a model with host layers")
    var r & = unsafe(*g_crd)
    assert(r.hot_slots == 0l || r.hot_slots == slots, "cud_rdec_hot_layer: one slot count serves every layer")
    let dim = r.dim
    let nfe = r.nfe
    if (r.exps_bias || kq_sb(f1) != kq_sb(f3) || !kq_rows_ok(dim, f1) || !kq_rows_ok(dim, f3) || !kq_rows_ok(nfe, f2)
            || !cud_serves_fmt(f1) || !cud_serves_fmt(f3) || !cud_serves_fmt(f2)) {
        to_log(LOG_INFO, "dasLLAMA cuda resident: the hot expert pool skips layer {l} (formats {f1}/{f3}/{f2}: a biased stack, a mixed gate and up feed, a row off its block lattice, or a format outside the arena set)\n")
        return none
    }
    var H = CudHot(on = true, f1 = f1, f3 = f3, f2 = f2, blk1 = kq_blocks_for(dim, nfe, f1), blk3 = kq_blocks_for(dim, nfe, f3),
        blk2 = kq_blocks_for(nfe, dim, f2), xnb2 = nfe / kq_unit(f2), xnb1 = dim / kq_unit(f1))
    let b1 = dev_block_bytes(kq_fmt_of_slot(f1))
    let b3 = dev_block_bytes(kq_fmt_of_slot(f3))
    let b2 = dev_block_bytes(kq_fmt_of_slot(f2))
    H.wq1 = cud_dev_zeroed(slots * H.blk1 * b1.qsb)
    H.ws1 = cud_dev_zeroed(max(slots * H.blk1 * b1.ssb, 64l))
    H.wq3 = cud_dev_zeroed(slots * H.blk3 * b3.qsb)
    H.ws3 = cud_dev_zeroed(max(slots * H.blk3 * b3.ssb, 64l))
    H.wq2 = cud_dev_zeroed(slots * H.blk2 * b2.qsb)
    H.ws2 = cud_dev_zeroed(max(slots * H.blk2 * b2.ssb, 64l))
    if (H.wq1 == 0ul || H.wq3 == 0ul || H.wq2 == 0ul) {
        to_log(LOG_WARNING, "dasLLAMA cuda resident: layer {l}'s pool of {slots} slots found no room on the device - its experts stay on the host\n")
        for (b in [H.wq1, H.ws1, H.wq3, H.ws3, H.wq2, H.ws2]) {
            cuda_mem_free(b)
        }
        return none
    }
    if (r.hot_slots == 0l) {
        r.hot_slots = slots
        let m1 = cuda_mem_alloc_host_mapped(uint64(RDEC_ROUTED_SLOTS * 8l))
        let m3 = cuda_mem_alloc_host_mapped(uint64(RDEC_ROUTED_SLOTS * 8l))
        let m2 = cuda_mem_alloc_host_mapped(uint64(RDEC_ROUTED_SLOTS * 8l))
        let mw = cuda_mem_alloc_host_mapped(uint64(RDEC_ROUTED_SLOTS * 4l))
        r.hot_meta1_host = m1.host; r.hot_meta1_dev = m1.dev
        r.hot_meta3_host = m3.host; r.hot_meta3_dev = m3.dev
        r.hot_meta2_host = m2.host; r.hot_meta2_dev = m2.dev
        r.hot_w_host = mw.host; r.hot_w_dev = mw.dev
        verify(r.hot_meta1_host != null && r.hot_meta3_host != null && r.hot_meta2_host != null && r.hot_w_host != null, "cud decode: the pool's mapped blocks did not allocate")
        unsafe {
            var wp = reinterpret<float?>(r.hot_w_host)
            for (i in range64(RDEC_ROUTED_SLOTS)) {
                wp[i] = 0.0
            }
        }
        var ramp <- [for (i in range64(RDEC_ROUTED_SLOTS)); uint(i + 1l)]
        r.hot_ident1 = buf_upload(ramp, g_cud.stream)
    }
    while (long_length(r.hot) <= l) {
        r.hot |> push(CudHot())
    }
    r.hot[l] = H
    crd_graphs_drop(r)
    return (base = l * 3l, stride = 1l)
}

//! one expert's planes into slot `slot` of layer `l`'s pool (`RdecHotUploadFn`), in the gather's device layout
[cold_path, arch(at="../ARCHITECTURE_GPU_VULKAN_POOL.md#hc-hot-pool")]
def cud_rdec_hot_upload(l, slot : int64; wq1 : array<uint8>; ws1 : array<uint8>; wq3 : array<uint8>; ws3 : array<uint8>; wq2 : array<uint8>; ws2 : array<uint8>) {
    cud_bind()
    return if (g_cud.dry)
    let r & = unsafe(*g_crd)
    let H & = unsafe(r.hot[l])
    assert(H.on && slot >= 0l && slot < r.hot_slots, "cud_rdec_hot_upload: a slot outside the layer's pool")
    let b1 = dev_block_bytes(kq_fmt_of_slot(H.f1))
    let b3 = dev_block_bytes(kq_fmt_of_slot(H.f3))
    let b2 = dev_block_bytes(kq_fmt_of_slot(H.f2))
    if (long_length(wq1) != H.blk1 * b1.qsb || long_length(wq3) != H.blk3 * b3.qsb || long_length(wq2) != H.blk2 * b2.qsb
            || long_length(ws1) < H.blk1 * b1.ssb || long_length(ws3) < H.blk3 * b3.ssb || long_length(ws2) < H.blk2 * b2.ssb) {
        panic("cud_rdec_hot_upload: layer {l}'s gathered planes ({long_length(wq1)}/{long_length(wq3)}/{long_length(wq2)} quant bytes) are not one expert's in formats {H.f1}/{H.f3}/{H.f2}")
    }
    unsafe {
        cuda_memcpy_htod(H.wq1 + uint64(slot * H.blk1 * b1.qsb), addr<void?>(wq1[0]), uint64(H.blk1 * b1.qsb), g_cud.stream)
        cuda_memcpy_htod(H.wq3 + uint64(slot * H.blk3 * b3.qsb), addr<void?>(wq3[0]), uint64(H.blk3 * b3.qsb), g_cud.stream)
        cuda_memcpy_htod(H.wq2 + uint64(slot * H.blk2 * b2.qsb), addr<void?>(wq2[0]), uint64(H.blk2 * b2.qsb), g_cud.stream)
        if (b1.ssb > 0l) {
            cuda_memcpy_htod(H.ws1 + uint64(slot * H.blk1 * b1.ssb), addr<void?>(ws1[0]), uint64(H.blk1 * b1.ssb), g_cud.stream)
        }
        if (b3.ssb > 0l) {
            cuda_memcpy_htod(H.ws3 + uint64(slot * H.blk3 * b3.ssb), addr<void?>(ws3[0]), uint64(H.blk3 * b3.ssb), g_cud.stream)
        }
        if (b2.ssb > 0l) {
            cuda_memcpy_htod(H.ws2 + uint64(slot * H.blk2 * b2.ssb), addr<void?>(ws2[0]), uint64(H.blk2 * b2.ssb), g_cud.stream)
        }
    }
    verify(g_crd_hot_batch || cud_sync(), "cud decode: a pool upload did not land - the caller reuses its gather planes as soon as this returns")
}

//! the hit chain over `nreg` regions of `nrows` rows: the rows' feed requant in the gate plane's form, the gate and up GEMVs over
//! the regions, the act with the down feed's requant, the down GEMV into the routed rows; captured once a (layer, region count, rows)
[arch(at="../ARCHITECTURE_GPU_VULKAN_POOL.md#hc-hot-pool")]
def private crd_hot_chain(r : CudRdec; var s : void?; H : CudHot; nreg, nrows : int64) {
    let dim = r.dim
    let nfe = r.nfe
    crd_rq_rows(s, H.f1, r.xb, r.moe_xq, r.moe_xs, dim * nrows)
    var pcg = GemvArgs(n = uint(dim), d = uint(nfe), nreg = uint(nreg), ybase = 0u)
    cud_gemv_enc(H.f1, s, fixed_array(H.wq1, H.ws1, r.hot_meta1_dev, r.moe_xq, r.moe_xs, r.egate), pcg, nreg * nfe)
    cud_gemv_enc(H.f3, s, fixed_array(H.wq3, H.ws3, r.hot_meta3_dev, r.moe_xq, r.moe_xs, r.eup), pcg, nreg * nfe)
    let dkq = kq_sb(H.f2)
    var pca = ActArgs(nelem = uint(nreg * nfe), act = act_code(r.act_oai, r.is_gelu), nblk = uint(nreg * nfe / kq_unit(dkq)))
    if (dkq) {
        enc_q8k_actrq_cls(s, fixed_array(r.egate, r.eup, r.ehq, r.ehs), pca, rq_wgs(true, nreg * nfe))
    } else {
        enc_q8_actrq_cls(s, fixed_array(r.egate, r.eup, r.ehq, r.ehs), pca, rq_wgs(false, nreg * nfe))
    }
    var pcd = GemvArgs(n = uint(nfe), d = uint(dim), nreg = uint(nreg), ybase = 0u)
    cud_gemv_enc(H.f2, s, fixed_array(H.wq2, H.ws2, r.hot_meta2_dev, r.ehq, r.ehs, r.edown), pcd, nreg * dim)
}

//! the chain's records and weights staged in the driver's mapped blocks (`hot_stage`)
def private crd_hot_stage(r : CudRdec; H : CudHot; nreg, wlen : int64; slots : int64 const?; w : float const?) {
    unsafe {
        hot_stage(H, reinterpret<uint?>(r.hot_meta1_host), reinterpret<uint?>(r.hot_meta3_host), reinterpret<uint?>(r.hot_meta2_host),
            reinterpret<float?>(r.hot_w_host), r.k, nreg, wlen, slots, w)
    }
}

//! the hit chain launched unwaited on the device stream, so it runs while the host sums the misses; the next segment orders behind it
def private crd_hot_go(var r : CudRdec; l, nreg, nrows : int64) {
    let H & = unsafe(r.hot[l])
    let key = (int(CrdCmd.hot) << 26) | (int(l) << 12) | (int(nreg) << 4) | int(nrows)
    if (!key_exists(r.graphs, key)) {
        // the chain's classes load ahead of the capture: a module load inside a capture invalidates it, and a K-quant stack's Q8_K feed may have no launch before the pool's first chain
        verify(ensure_cls_q8k_rq() && ensure_cls_dn_rq() && ensure_q8k_actrq_cls() && ensure_q8_actrq_cls()
            && cud_gemv_ensure(H.f1) && cud_gemv_ensure(H.f3) && cud_gemv_ensure(H.f2), "cud decode: the hit chain's classes must load on a live device")
        let g = cuda_graph_capture(g_cud.stream) {
            crd_hot_chain(r, g_cud.stream, H, nreg, nrows)
        }
        if (g == null) {
            panic("dasLLAMA cuda resident: the hit chain's graph did not capture (layer {l}, {nreg} regions): {cuda_last_error()}")
        }
        r.graphs[key] = g
    }
    if (!cuda_graph_launch(r.graphs[key], g_cud.stream)) {
        panic("dasLLAMA cuda resident: the hit chain did not launch: {cuda_last_error()}")
    }
    r.hot_submits++
}

//! the hits of layer `l`'s routed step (`RdecHotSubmitFn`): their records and weights staged, the hit chain launched
[arch(at="../ARCHITECTURE_GPU_VULKAN_POOL.md#hc-hot-pool")]
def cud_rdec_hot_submit(l, nhit : int64; slots : int64 const?; w : float const?) {
    cud_bind()
    var r & = unsafe(*g_crd)
    let H & = unsafe(r.hot[l])
    assert(H.on && nhit <= r.k, "cud_rdec_hot_submit: a layer with no pool, or more hits than routed slots")
    crd_hot_stage(r, H, nhit, r.k, slots, w)
    return if (nhit == 0l)
    crd_hot_go(r, l, nhit, 1l)
    r.hot_hits += nhit
}

//! the verify rows' hits of layer `l` (`RdecHotSubmitRowsFn`): k slot and weight entries a row, a row's hits first, its misses at weight 0
[arch(at="../ARCHITECTURE_GPU_VULKAN_HC.md#hc-verify-rows")]
def cud_rdec_hot_submit_rows(l, nrows : int64; slots : int64 const?; w : float const?) {
    cud_bind()
    var r & = unsafe(*g_crd)
    let H & = unsafe(r.hot[l])
    let nreg = nrows * r.k
    if (!H.on) {
        panic("cud_rdec_hot_submit_rows: layer {l} has no pool")
    }
    if (nreg > RDEC_ROUTED_SLOTS || nrows > r.nb) {
        panic("cud_rdec_hot_submit_rows: {nrows} rows x {r.k} = {nreg} regions against {RDEC_ROUTED_SLOTS}, the planes hold {r.nb} rows")
    }
    crd_hot_stage(r, H, nreg, RDEC_ROUTED_SLOTS, slots, w)
    crd_hot_go(r, l, nreg, nrows)
    var hits = 0l
    for (j in range64(nreg)) {
        hits += unsafe(w[j]) != 0.0 ? 1l : 0l
    }
    r.hot_hits += hits
}

//! the hit chains launched and the picks they served since the driver armed
def cud_rdec_hot_stats : tuple<submits : int64; hits : int64>
    => g_crd != null ? (submits = g_crd.hot_submits, hits = g_crd.hot_hits) : (submits = 0l, hits = 0l)

//! the hyper-connection chain's seats this home declines by name: its planes, the n-gram side input, the ring and the window
[unused_argument(hc, lr, n_sites, norm_rows, inject_rows, down_blks, up_blks, head_norm, head_down, head_up, mtp_head_norm, mtp_head_down, mtp_head_up, hnorm_rows)]
def cud_rdec_set_hc(hc, lr, n_sites : int64; norm_rows : array<float>; inject_rows : array<float>; down_blks, up_blks : array<int64>; head_norm : array<float>; head_down, head_up : int64;
                    mtp_head_norm : array<float>; mtp_head_down, mtp_head_up : int64; hnorm_rows : array<float>) : bool {
    to_log(LOG_WARNING, "dasLLAMA cuda resident: the hyper-connection chain (hc {hc}, {n_sites} mixer sites) has no form on this home - the CPU rails serve\n")
    return false
}

[unused_argument(l, ind, nrows, kern, dil, bkey, bval, norm_rows, conv_taps)]
def cud_rdec_set_ngram(l, ind, nrows, kern, dil, bkey, bval : int64; norm_rows : array<float>; conv_taps : array<float>) : bool => false

[unused_argument(host, up)]
def cud_rdec_ngram_ring(var host : float?; up : bool) {
    panic("dasLLAMA cuda resident: the n-gram conv ring asked of a home with no hyper-connection chain")
}

[unused_argument(x_batch, cos_batch, side, npos, pos0, logits, experts)]
def cud_rdec_prefill_hc(x_batch : array<float>; cos_batch : array<float>; side : array<float>; npos, pos0 : int64; var logits : array<float>; experts : RdecHostExpertsRowsFn) {
    panic("dasLLAMA cuda resident: the hyper-connection window asked of a home with no hyper-connection chain")
}

//! the N-row token command: `nrows` rows (2 to the per-row planes' height) through one step, each at its position, count and
//! region, its token row gathered off its id where every row brings one (else the x rows ride the in block), each row's logits
//! landed at its own pointer, or - where the pointer is null - its argmax alone into `picks` (`RdecTokenNFn`)
[arch(at="../ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-token-command")]
def cud_rdec_token_n(x : array<float>; cossin : array<float>; ple : array<float>; ple_raw : bool; ids : array<int64>; poss : array<int64>; cnts : array<int64>; regs : array<int64>; nrows : int64; lrows : array<float?>; var picks : array<int64>) {
    crd_chain_settle()
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
    verify(r.ple <= 0l || ple_raw == r.ple_gpu, "cud_rdec_token_n: raw per-layer-embedding rows without the projection on device, or finished rows with it")
    crd_ple_stage(r, ple, nrows, gathered)
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
    crd_launch(r, CrdCmd.token, nrows, !all_pick, gathered, maxcnt, true) $(l : int64) {
        crd_no_host(l)
    }
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
    crd_served(r, nrows)
    if (gathered) {
        r.nrow_emb_gathered += nrows
        if (r.ple > 0l && crd_side_gather_on(r)) {
            r.nrow_side_gathered += nrows
        }
    }
}

//! the rows the N-row command steps at once on the armed model; 0 where a layer has no N-row form
def cud_rdec_token_n_rows : int64 {
    if (g_crd == null || !g_crd.ready) return 0l
    for (L in g_crd.layers) {
        if (L.moe && L.experts_host) return rdec_n_decline(g_crd.n_decline_said, "a routed layer whose experts serve on the host (the split command steps a row at a time)")
    }
    return g_crd.nb
}

//! every mirror address of the one-row command and the window chain, and the host's row copies, resolve against region `region`
def cud_rdec_select_region(region : int64) {
    assert(g_crd != null && region >= 0l && region < g_crd.regions, "cud_rdec_select_region: no such mirror region")
    if (region != g_crd.region) {
        crd_chain_settle()
    }
    g_crd.region = region
}

//! layer `L`'s mirror base in the selected region: the window chain's rows and the host's row copies land there
def private crd_region_base(r : CudRdec; L : CudLayer; region : int64) : int64 => rdec_mir_region_base(L.mir_base, region, r.mir_units)
def private crd_mir_base(r : CudRdec; L : CudLayer) : int64 => crd_region_base(r, L, r.region)

//! a window segment's mirror base in layer `L`: its region's, past the rows the segments packed before it hold
def private crd_seg_mir_base(r : CudRdec; L : CudLayer; sg : RdecPfSeg) : int64 {
    assert(sg.koff == 0l || L.ring == 0l, "cud prefill: a packed segment on a sliding layer's ring")
    return crd_region_base(r, L, sg.region) + sg.koff * kv_row_units(r.kv_dt, L.kvd)
}

//! The armed driver seats several segments in one region: no sliding layer's ring, no block-codec store (`RdecSegsPackedFn`)
[arch(at="../ARCHITECTURE_GPU_VULKAN_WINDOW.md#vk-prefill-scratch")]
def cud_rdec_segs_packed : bool {
    return false if (g_crd == null || !g_crd.ready || kv_block_codec(g_crd.kv_dt))
    for (L in g_crd.layers) {
        return false if (L.ring != 0l)
    }
    return true
}

//! the s tile where the m tiles would idle three quarters of the SMs, else cm2_wave_col at two blocks an SM
def private cud_cm2_tc(d, rows : int64) : int64 => cm2_wave_col(d, rows, int64(g_cud.sm_count), 2l * int64(g_cud.sm_count))

//! one window GEMM's column and split-k, picked together: (tc, chunks, 256-aligned chunk), chunks 1 and chunk 0 = whole
def cud_cm2_pick(d, rows, n : int64) : tuple<tc : int64; nsplit : int64; ksplit : int64> {
    var tc = cud_cm2_tc(d, rows)
    let cores = int64(g_cud.sm_count)
    let forced = g_env_vulkan.cm2_splitk
    let scratch = g_crd != null ? g_crd.pf_splitk_bytes : 9_223_372_036_854_775_807l
    if (forced <= 0l && rows < 32l && tc == 32l) {   // the short window: the knee's split, or whole at knee 0 - never the wave model's split, which the race never timed
        return cud_split_k_tgs() > 0l ? short_split_pick(d, rows, n, 0l, scratch, cud_split_k_tgs()) : (tc = 32l, nsplit = 1l, ksplit = 0l)
    }
    var want = 1l
    if (forced > 0l) {
        want = forced
    } elif (cores > 0l) {   // a grid under half the card's block slots splits its K, a 32-row window's included - the Vulkan wave model's rule
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
    let sk = cm2_split_fit(n, want, rows * d * 4l, scratch)
    return sk > 1l ? (tc = tc, nsplit = sk, ksplit = cm2_chunk(n, sk)) : (tc = tc, nsplit = 1l, ksplit = 0l)
}

def private cud_sched_make(words : int64) : CudSched => CudSched(buf = cud_dev_zeroed(words * 4l), wblk = -1l, rows = -1l)

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

let CUD_MMQ_S_ROWS = 32l   //! the window rows at or under which the split s column serves in the leaf's place

//! whether a window GEMM site can take its MMQ leaf at all - the knob, the plan's rows, the plane's shape and format: what the
//! window's feeds are sized for, whichever form each window takes; a Q8_0 plane starts on a block of 8 (eight scales a quad), a
//! min-term leaf's reduction is a multiple of 512 (it folds sixteen blocks at a time)
def private crd_site_mmq_able(r : CudRdec; fmt : int; n, d, wblk : int64) : bool {
    let knob = r.pf_knob != CUDA_MMQ_UNPINNED ? r.pf_knob : crd_mmq_knob()
    let f = kq_fmt_of_slot(fmt)
    return false if (knob == CUDA_MMQ_OFF || r.pf_np % 128l != 0l || d % 128l != 0l || n % 256l != 0l || !kq_mmq_leaf(f)
        || (knob == CUDA_MMQ_WINNERS && kq_mmq_raced(f))
        || (f == KqFmt.nv4 && g_cud.sm < FP4_RUNG_SM))
    return wblk % 8l == 0l if (f == KqFmt.q8)
    return !kq_mmq_min(f) || n % 512l == 0l
}

//! whether this window's GEMM site takes its MMQ leaf: a site that can, on a window past the s column's rows, outside an embedding's ask
[arch(at="../ARCHITECTURE_GPU_ASYMMETRIES_CUDA.md#cuda-asymmetries")]
def private crd_site_mmq(r : CudRdec; fmt : int; n, d, wblk : int64) : bool =>
    r.pf_wlen > CUD_MMQ_S_ROWS && !g_crd_hidden_ask && crd_site_mmq_able(r, fmt, n, d, wblk)

//! the form a window GEMM site's feed takes: the f16 rows of the cm2 tiles, the FP4 strips of the FP4 leaf, or the Q8_0 rows of the
//! int8 leaves - with their block sums where the plan carries a min-term leaf
enum private CrdFeed {
    f16
    fp4
    q8
    q8sum
}

//! the feed of a site in format slot `fmt` that takes (`mmq`) or does not take its leaf (`crd_site_mmq` on a window, `crd_site_mmq_able` for the planes)
def private crd_feed_of(mmq : bool; fmt : int; min : bool) : CrdFeed {
    return CrdFeed.f16 if (!mmq)
    return CrdFeed.fp4 if (kq_fmt_of_slot(fmt) == KqFmt.nv4)
    return min ? CrdFeed.q8sum : CrdFeed.q8
}

def private crd_kv_merged(dim : int64; L : CudLayer) : bool =>
    !L.vfk && L.fk == L.fv && L.bv == L.bk + kq_blocks_for(dim, L.kvd, L.fk)

//! the MMQ leaves the window sites take: whether one has a min term or is the FP4 leaf (the window's feeds follow), their names for the load log
def private crd_mmq_leaves(r : CudRdec) : tuple<min : bool; fp4 : bool; planes : string> {
    var inscope on : array<bool>
    on |> resize(length(KQ_MMQ_LEAF_FMTS))
    for (L in r.layers) {
        let kd = crd_kv_merged(r.dim, L) ? 2l * L.kvd : L.kvd
        var inscope sites <- L.recurrent ? [(fmt = L.fqkv, n = r.dim, d = r.dn_cd, b = L.bqkv), (fmt = L.fz, n = r.dim, d = r.dn_di, b = L.bz), (fmt = L.fout, n = r.dn_di, d = r.dim, b = L.bout), (fmt = L.f1, n = r.dim, d = L.hid, b = L.b1), (fmt = L.f3, n = r.dim, d = L.hid, b = L.b3), (fmt = L.f2, n = L.hid, d = r.dim, b = L.b2)] : [(fmt = L.fq, n = r.dim, d = crd_pf_q_row_stride(r, L), b = L.bq), (fmt = L.fk, n = r.dim, d = kd, b = L.bk), (fmt = L.fv, n = r.dim, d = L.kvd, b = L.bv), (fmt = L.fo, n = L.qd, d = r.dim, b = L.bo), (fmt = L.f1, n = r.dim, d = L.hid, b = L.b1), (fmt = L.f3, n = r.dim, d = L.hid, b = L.b3), (fmt = L.f2, n = L.hid, d = r.dim, b = L.b2)]
        for (site in sites) {
            continue if (!crd_site_mmq_able(r, site.fmt, site.n, site.d, site.b))
            on[kq_mmq_leaf_index(kq_fmt_of_slot(site.fmt))] = true
        }
    }
    var mn = false
    var fp4 = false
    var planes = ""
    for (i, f, name in count(), KQ_MMQ_LEAF_FMTS, KQ_MMQ_LEAF_NAMES) {
        continue if (!on[i])
        mn = mn || kq_mmq_min(f)
        fp4 = fp4 || f == KqFmt.nv4
        planes = empty(planes) ? name : "{planes}, {name}"
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
        continue if (site.b < 0l)   // a V-from-K layer's v site: no plane
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
    var cur = 4l
    batch_sched_map(unsafe(addr(rec[0])), cur, 0l, wblk, rows, 0l)
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
    r.pf_wlen = np
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
    r.pf_x = cud_dev_zeroed(np * dim * 4l)
    r.pf_xf = cud_dev_zeroed(slack * dim * 2l)
    r.pf_q = cud_dev_zeroed(np * (r.q_gated ? 2l * qd : qd) * 4l)
    if (r.hybrid) {
        let cd = r.dn_cd
        let di = r.dn_di
        r.pf_dnqkv = cud_dev_zeroed(np * cd * 4l)
        r.pf_dnconv = cud_dev_zeroed(np * cd * 4l)
        r.pf_dnz = cud_dev_zeroed(np * di * 4l)
        r.pf_dnbg = cud_dev_zeroed(np * r.dn_nvh * 4l)
        r.pf_dnws = cud_dev_zeroed(np * di * 4l)
        r.pf_dnof = cud_dev_zeroed(slack * di * 2l)
        r.pf_dnoq = cud_dev_zeroed(np * di)
        r.pf_dnos = cud_dev_zeroed(np * (di / Q8_BLOCK_ELEMS) * 4l)
    }
    r.pf_kv = cud_dev_zeroed(np * 2l * kvd * 4l)
    r.pf_v = cud_dev_zeroed(np * kvd * 4l)
    if (kv_block_codec(r.kv_dt)) {
        r.pf_kst = cud_dev_zeroed(np * kvd * 4l)
        r.pf_vst = cud_dev_zeroed(np * kvd * 4l)
    }
    r.pf_attn = cud_dev_zeroed(np * qd * 4l)
    r.pf_af = cud_dev_zeroed(slack * qd * 2l)
    r.pf_xb2 = cud_dev_zeroed(np * dim * 4l)
    r.pf_gate = cud_dev_zeroed(np * hid * 4l)
    r.pf_up = cud_dev_zeroed(np * hid * 4l)
    r.pf_hf = cud_dev_zeroed(slack * hid * 2l)
    r.pf_ffnout = cud_dev_zeroed(np * dim * 4l)
    if (r.ple > 0l) {
        r.pf_ple = cud_dev_zeroed(np * r.ple_all * 4l)
        r.pf_pleg = cud_dev_zeroed(np * r.ple * 4l)
        if (r.ple_gpu) {
            r.pf_plep = cud_dev_zeroed(np * r.ple_all * 4l)
        }
    }
    r.pf_cos = cud_dev_zeroed(np * r.cos_elems * 4l)
    r.pf_xq = cud_dev_zeroed(np * dim)
    r.pf_xs = cud_dev_zeroed(np * (dim / Q8_BLOCK_ELEMS) * 4l)
    r.pf_knob = crd_mmq_knob()
    let leaves = crd_mmq_leaves(r)
    r.pf_mmq_min = leaves.min
    r.pf_fp4 = leaves.fp4
    if (r.pf_fp4) {
        r.pf_x4q = cud_dev_zeroed(np * dim / FP4_NIBBLES_PER_BYTE)
        r.pf_x4s = cud_dev_zeroed(np * dim / NV4_FEED_STRIP)
        r.pf_xrs = cud_dev_zeroed(np * 4l)
        r.pf_a4q = cud_dev_zeroed(np * qd / FP4_NIBBLES_PER_BYTE)
        r.pf_a4s = cud_dev_zeroed(np * qd / NV4_FEED_STRIP)
        r.pf_ars = cud_dev_zeroed(np * 4l)
        r.pf_h4q = cud_dev_zeroed(np * hid / FP4_NIBBLES_PER_BYTE)
        r.pf_h4s = cud_dev_zeroed(np * hid / NV4_FEED_STRIP)
        r.pf_hrs = cud_dev_zeroed(np * 4l)
        if (r.hybrid) {
            r.pf_dno4q = cud_dev_zeroed(np * r.dn_di / FP4_NIBBLES_PER_BYTE)
            r.pf_dno4s = cud_dev_zeroed(np * r.dn_di / NV4_FEED_STRIP)
            r.pf_dnors = cud_dev_zeroed(np * 4l)
        }
    }
    r.pf_aq = cud_dev_zeroed(np * qd)
    r.pf_as = cud_dev_zeroed(np * (qd / Q8_BLOCK_ELEMS) * 4l)
    r.pf_hq = cud_dev_zeroed(np * hid)
    r.pf_hs = cud_dev_zeroed(np * (hid / Q8_BLOCK_ELEMS) * 4l)
    if (r.pf_mmq_min) {
        r.pf_xsum = cud_dev_zeroed(np * (dim / Q8_BLOCK_ELEMS) * 4l)
        r.pf_asum = cud_dev_zeroed(np * (qd / Q8_BLOCK_ELEMS) * 4l)
        r.pf_hsum = cud_dev_zeroed(np * (hid / Q8_BLOCK_ELEMS) * 4l)
        if (r.hybrid) {
            r.pf_dnosum = cud_dev_zeroed(np * (r.dn_di / Q8_BLOCK_ELEMS) * 4l)
        }
    }
    let pick = empty(leaves.planes) ? "the f16-fed cm2 tiles" : "the MMQ kernels (int8 over Q8_0 feeds; NVFP4 on the FP4 tensor cores) for the {leaves.planes} planes, the f16-fed cm2 tiles for the rest"
    let knob_name = g_cuda_mmq_pin != CUDA_MMQ_UNPINNED ? "set_cuda_mmq_pin" : "DASLLAMA_CUDA_MMQ"
    to_log(LOG_INFO, "dasLLAMA cuda tier: the window chain's GEMMs on {pick} ({knob_name}={r.pf_knob}; a window of {CUD_MMQ_S_ROWS} rows or fewer and an embedding's windows keep the s column whatever the value)\n")
    r.pf_splitk_bytes = 262_144l * int64(g_cud.sm_count)
    r.pf_splitk = cud_dev_zeroed(r.pf_splitk_bytes)
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
    let fa_head = hs == 64l || hs == 96l || hs == 128l || hs == 256l || hs == 512l
    // the gate arm and the softcap arm are the h256 stamps' alone, and a gated softcap pairs no stamp: those shapes take the chunked tiles
    let fa = kv16 && fa_head && (!crd_attn_gated(r) || hs == 256l) && cud_fa_cap_ok(hs, L.attn_cap) && !(crd_attn_gated(r) && L.attn_cap != 0.0)
    let convert = !o_mmq && !(fa && f16out)
    let qrow = crd_pf_q_row_stride(r, L)
    let qstride = crd_q_head_stride(r, L)
    for (sg in wsegs) {
        let mir_base = shadow ? 0u : uint(crd_seg_mir_base(r, L, sg))
        if (fa) {
            var pcf = FaArgs(rows = uint(sg.npos), w0 = uint(sg.pos0), qd = uint(qd), kvd = kvd, kv_mul = uint(L.kv_mul),
                kvbase = mir_base, scale = r.scale, qstride = uint(qrow), qhs = uint(qstride), window = uint(L.window), cap = L.attn_cap,
                sinkoff = uint(l * r.n_heads), row0 = uint(sg.row0), ring = ring)
            var fbufs = fixed_array(r.pf_q, kmir, vmir, f16out ? r.pf_af : r.pf_attn)
            cud_fa_enc(s, hs, crd_attn_gated(r), f16out, r.has_sinks, fbufs, r.sinks, pcf, fa_stamp_wgs(cud_fa_khr(hs), r.n_heads, sg.npos))
        } else {
            var pca = DaAttnBArgs(npos = uint(sg.npos), nh = uint(r.n_heads), kvd = kvd, hs = uint(hs), kv_mul = uint(L.kv_mul),
                kbase = mir_base, vbase = mir_base, w0 = uint(sg.pos0), scale = r.scale, qhs = uint(qstride), gated = crd_attn_gated(r) ? 1u : 0u,
                window = uint(L.window), cap = L.attn_cap, row0 = uint(sg.row0), ring = ring)
            var abufs = fixed_array(r.pf_q, kmir, vmir, r.pf_attn)
            if (hs > 256l) {   // the wide-head twin: a 4-row q tile (gemma-4's global layers at 512)
                let wwgs = at_attn_tile_wgs(r.n_heads, sg.npos, 4l)
                if (kv16) {
                    enc_da_attn_bw_f16_cls(s, abufs, pca, wwgs)
                } else {
                    enc_da_attn_bw_cls(s, abufs, pca, wwgs)
                }
            } else {
                let awgs = at_attn_tile_wgs(r.n_heads, sg.npos)
                if (kv16) {
                    enc_da_attn_b_f16_cls(s, abufs, pca, awgs)
                } else {
                    enc_da_attn_b_cls(s, abufs, pca, awgs)
                }
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
def private crd_window_kv(r : CudRdec; l : int64; L : CudLayer; kmir, vmir : uint64; wsegs : array<RdecPfSeg>; wlen, voff, kstride : int64; kv_only : bool = false) {
    var s = g_cud.stream
    let qd = L.qd
    let kvd = L.kvd
    let hs = uint(L.hs)
    let kvq = kv_block_codec(r.kv_dt)
    let cos = kv_only ? r.pf_hcos : r.pf_cos
    let nqh = kv_only ? 0l : r.n_heads
    verify(!kvq || long_length(wsegs) == 1l, "cud prefill: a block codec's window holds one segment")
    if (r.qk_norm) {
        crd_window_qkn_rope(r, l, L, kmir, vmir, wsegs, voff, kstride, cos, nqh)
        return if (!kvq)
        crd_window_kv_codec(r, L, kmir, vmir, wsegs, wlen)
        return
    }
    let qpairs = kv_only ? 0l : qd / 2l   // the warm's store-only pass ropes the k pairs alone: nothing reads its q rows
    let pairs = qpairs + kvd / 2l
    var rbufs = fixed_array(r.pf_q, r.pf_kv, kvq ? r.pf_kst : kmir, kvq ? r.pf_vst : vmir, cos, crd_bias_plane(r))
    for (sg in wsegs) {
        var pcr = RopeKvBArgs(qd = uint(qd), kvd = uint(kvd), hs = hs, half = uint(L.rot / 2l), neox = r.neox ? 1u : 0u,
            kpair0 = uint(qpairs), ppp = uint(pairs), layerbase = kvq ? 0u : uint(crd_seg_mir_base(r, L, sg)), npos = uint(sg.npos),
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

//! the q/k-normed model's window rows through the fused norm, rope and K/V store, `nqh` q heads a row (0 on the head warm's store-only pass)
def private crd_window_qkn_rope(r : CudRdec; l : int64; L : CudLayer; kmir, vmir : uint64; wsegs : array<RdecPfSeg>; voff, kstride : int64; cos : uint64; nqh : int64) {
    var s = g_cud.stream
    let kvq = kv_block_codec(r.kv_dt)
    let nkh = L.kv_shared ? 0l : L.kvd / L.hs
    var qbufs = fixed_array(r.pf_q, r.pf_kv, kvq ? r.pf_kst : kmir, kvq ? r.pf_vst : vmir, cos, r.norms, r.tok, crd_bias_plane(r), r.pf_kv)
    for (sg in wsegs) {
        var pcf = crd_qkn_rope_args(r, l, L, kvq ? 0l : crd_seg_mir_base(r, L, sg), nkh, kstride, crd_pf_q_row_stride(r, L), kvq ? 0l : L.ring)
        pcf.win = 1u
        pcf.pos0 = uint(kvq ? sg.row0 : sg.pos0)
        pcf.row0 = uint(sg.row0)
        pcf.kbase = 0u
        pcf.vbase = uint(voff)
        pcf.nh = uint(nqh)
        pcf.rowwg = uint(nqh + nkh)
        if (r.kv_dt == KVDtype.f16) {
            enc_qkn_rope_f16_cls(s, qbufs, pcf, (nqh + nkh) * sg.npos)
        } else {
            enc_qkn_rope_cls(s, qbufs, pcf, (nqh + nkh) * sg.npos)
        }
    }
}

//! a block codec's window after the rope staged the floats: the store quantizes them into the mirror, the shadow pass dequantizes what the tiles read
def private crd_window_kv_codec(r : CudRdec; L : CudLayer; kmir, vmir : uint64; wsegs : array<RdecPfSeg>; wlen : int64) {
    var s = g_cud.stream
    let kvd = L.kvd
    let hs = uint(L.hs)
    let pos0w = wsegs[0].pos0
    let mir_base = uint(crd_region_base(r, L, wsegs[0].region))
    let tq4 = r.kv_dt == KVDtype.tq4
    if (tq4) {   // a shared-KV layer rotates its query and stores nothing: its donor's rows stand in the mirror
        let nkh = L.kv_shared ? 0l : kvd / L.hs
        var pct = KvTqStoreArgs(kvd = uint(kvd), hs = hs, layerbase = mir_base, pos0 = uint(pos0w), nqh = uint(r.n_heads), nkh = uint(nkh),
            qrow = uint(crd_pf_q_row_stride(r, L)), qstride = uint(crd_q_head_stride(r, L)), inv_sqrt_hs = 1.0 / sqrt(float(hs)), ring = uint(L.ring))
        enc_kv_tq4_store_b_cls(s, fixed_array(r.pf_q, r.pf_kst, r.pf_vst, kmir, vmir, r.tq4_signs), pct, wlen * (r.n_heads + 2l * nkh))
    } elif (!L.kv_shared) {
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
    r.pf_xb = cud_dev_zeroed(np * dim * 4l)
    r.pf_mlog = cud_dev_zeroed(np * r.ner * 4l)
    r.pf_midx = cud_dev_zeroed(nk * 4l)
    r.pf_mw = cud_dev_zeroed(nk * 4l)
    r.pf_minv = cud_dev_zeroed(nk * 4l)
    r.pf_bex = cud_dev_zeroed(crd_moe_pad_rows(r, nk) * 4l)   // every bucket row, the FP4 expert chain's pads among them
    r.pf_msch1 = cud_dev_zeroed(CUD_MOE_SCHED_BYTES)
    r.pf_msch3 = cud_dev_zeroed(CUD_MOE_SCHED_BYTES)
    r.pf_msch2 = cud_dev_zeroed(CUD_MOE_SCHED_BYTES)
    r.pf_mxg = cud_dev_zeroed((nk + TILE_READ_SLACK) * dim * 2l)
    r.pf_mhf = cud_dev_zeroed((nk + TILE_READ_SLACK) * nfe * 2l)
    r.pf_my1 = cud_dev_zeroed(nk * nfe * 4l)
    r.pf_my2 = cud_dev_zeroed(nk * nfe * 4l)
    r.pf_mdn = cud_dev_zeroed(nk * dim * 4l)
    if (r.experts_host) {
        let hx = cuda_mem_alloc_host_mapped(uint64(np * dim * 4l))
        let hl = cuda_mem_alloc_host_mapped(uint64(np * r.ner * 4l))
        let ha = cuda_mem_alloc_host_mapped(uint64(np * dim * 4l))
        r.pf_hx_host = hx.host; r.pf_hx_dev = hx.dev
        r.pf_hlog_host = hl.host; r.pf_hlog_dev = hl.dev
        r.pf_hacc_host = ha.host; r.pf_hacc_dev = ha.dev
        if (r.pf_hx_host == null || r.pf_hlog_host == null || r.pf_hacc_host == null) {
            panic("cud prefill: the window's host cut blocks did not allocate ({cuda_last_error()})")
        }
        var ones <- [for (_i in range64(np)); 1.0]
        r.pf_ones = buf_upload(ones, g_cud.stream)
        r.pf_ident = cud_ramp(np)
    }
    var any_mmq = false
    for (L in r.layers) {
        any_mmq ||= L.moe_mmq
    }
    if (any_mmq && r.pf_mmq_rows == 0l) {
        crd_pf_moe_mmq_setup(r)
    }
}

let private CUD_MOE_ROW_PAD = int64(MMQ_SCHED_ROW_PAD)   //! the FP4 expert chain's bucket row pad, the MMQ schedule stamp's

//! the bucket rows of a window of `nk` slot rows under the FP4 expert chain's pad: every expert's bucket padded to the tile at most
def private crd_moe_pad_rows(r : CudRdec; nk : int64) : int64 => nk + r.ne * (CUD_MOE_ROW_PAD - 1l)

//! the FP4 expert chain's planes, once a residency, made on the first routed layer whose stacks take the leaf (`CudLayer.moe_mmq`)
def private crd_pf_moe_mmq_setup(var r : CudRdec) {
    let dim = r.dim
    let nfe = r.nfe
    let rows = crd_moe_pad_rows(r, r.pf_nk)
    let xrows = rows + TILE_READ_SLACK
    r.pf_mmq_rows = rows
    r.pf_mxf = cud_dev_zeroed(xrows * dim * 4l)
    r.pf_mx4q = cud_dev_zeroed(xrows * dim / FP4_NIBBLES_PER_BYTE)
    r.pf_mx4s = cud_dev_zeroed(xrows * (dim / MX4_FEED_WORD_ELEMS) * 4l)
    r.pf_mxrs = cud_dev_zeroed(xrows * 4l)
    r.pf_mh4q = cud_dev_zeroed(xrows * nfe / FP4_NIBBLES_PER_BYTE)
    r.pf_mh4s = cud_dev_zeroed(xrows * (nfe / MX4_FEED_WORD_ELEMS) * 4l)
    r.pf_mhrs = cud_dev_zeroed(xrows * 4l)
    r.pf_mmy1 = cud_dev_zeroed(rows * nfe * 4l)
    r.pf_mmy2 = cud_dev_zeroed(rows * nfe * 4l)
    r.pf_mmdn = cud_dev_zeroed(rows * dim * 4l)
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

//! the window's dense FFN over the fed normed rows: the gate and up sites, the activation into the down site's feed, the down site into ffnout
def private crd_window_ffn(r : CudRdec; var s : void?; var L : CudLayer; wlen : int64) {
    let dim = r.dim
    let hid = L.hid
    crd_site(r, L.sch[SCH_G], L.f1, L.b1, r.pf_xf, r.pf_xq, r.pf_xs, r.pf_xsum, r.pf_x4q, r.pf_x4s, r.pf_xrs, r.pf_gate, dim, hid, wlen)
    crd_site(r, L.sch[SCH_U], L.f3, L.b3, r.pf_xf, r.pf_xq, r.pf_xs, r.pf_xsum, r.pf_x4q, r.pf_x4s, r.pf_xrs, r.pf_up, dim, hid, wlen)
    var pc12 = ActArgs(nelem = uint(wlen * hid), act = L.gelu ? ACT_GELU : ACT_SILU, nblk = uint(wlen * hid / kq_unit(false)), elem0 = 0u)
    let feed = crd_feed_of(crd_site_mmq(r, L.f2, hid, dim, L.b2), L.f2, r.pf_mmq_min)
    if (feed == CrdFeed.f16) {
        enc_actf16_cls(s, fixed_array(r.pf_gate, r.pf_up, r.pf_hf), pc12, rq_wgs(false, wlen * hid))
    } elif (feed == CrdFeed.fp4) {
        var pcf = Fp4RowsArgs(k = uint(hid), inbase = 0u, act = L.gelu ? ACT_GELU : ACT_SILU)
        enc_fp4_actrows(s, fixed_array(r.pf_gate, r.pf_up, r.pf_h4q, r.pf_h4s, r.pf_hrs), pcf, wlen)
    } elif (feed == CrdFeed.q8sum) {
        enc_q8_actrq_sum_cls(s, fixed_array(r.pf_gate, r.pf_up, r.pf_hq, r.pf_hs, r.pf_hsum), pc12, rq_wgs(false, wlen * hid))
    } else {
        enc_q8_actrq_cls(s, fixed_array(r.pf_gate, r.pf_up, r.pf_hq, r.pf_hs), pc12, rq_wgs(false, wlen * hid))
    }
    crd_site(r, L.sch[SCH_D], L.f2, L.b2, r.pf_hf, r.pf_hq, r.pf_hs, r.pf_hsum, r.pf_h4q, r.pf_h4s, r.pf_hrs, r.pf_ffnout, hid, dim, wlen)
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#vk-prefill-moe-block")]
def private crd_window_moe(r : CudRdec; var s : void?; l : int64; var L : CudLayer; w0, wlen : int64; segs, wsegs : array<RdecPfSeg>; experts : RdecHostExpertsRowsFn) {
    let dim = r.dim
    let nfe = r.nfe
    let k = r.k
    let ne = r.ne
    let nk = wlen * k
    var pc8 = crd_wo_ar_args(r, L)
    enc_cls_ar(s, fixed_array(r.pf_x, r.pf_xb2, r.norms, r.pf_xb), pc8, wlen)
    if (L.experts_host) {
        crd_window_host_cut(r, s, l, L, w0, wlen, segs, wsegs, experts)
        return
    }
    if (L.b1 >= 0l) {
        crd_x_feeds_from(r, s, r.pf_xb, wlen, crd_gu_feeds(r, L))
        crd_window_ffn(r, s, L, wlen)
    }
    crd_window_router(r, s, L, wlen)
    var pct = crd_topk_args(r, L)
    cud_topk_rows_enc(ne, s, fixed_array(r.pf_mlog, r.pf_mw, r.moe_ds, r.moe_rb, r.pf_midx), pct, wlen)
    if (L.moe_g4) {   // the routed feed the gather reads
        crd_g4_feed(r, s, L, r.pf_x, r.pf_ffnout, r.pf_xb, wlen)
    }
    let bounds = crd_moe_bounds(r, nk)
    let map_m_gu = MOE_SCHED_MAP_OFF + bounds.gu
    let map_m_dn = MOE_SCHED_MAP_OFF + bounds.dn
    var pcs = gk_sched_args(nk, ne, nfe, dim, 128u, false, bounds, L.be1, L.be3, L.be2, L.fe1, L.fe3, L.fe2)
    cud_moe_sched_enc(ne, L.moe_mmq, s, fixed_array(r.pf_midx, r.pf_minv, r.pf_msch1, r.pf_msch3, r.pf_msch2, r.pf_bex), pcs)
    var pca = ActArgs(nelem = uint(nk * nfe), act = act_code(r.act_oai, r.is_gelu), nblk = uint(nk * nfe / kq_unit(false)), elem0 = 0u,
        bias_on = r.exps_bias ? 1u : 0u, gboff = uint(L.router_slot * ne * nfe), brow = uint(nfe))
    var mdn = r.pf_mdn
    if (L.moe_mmq) {
        let bucket_rows = crd_moe_pad_rows(r, nk)   // (the planes hold the residency's widest)
        var pcgp = GatherArgs(n = uint(dim), k = uint(k), r0 = 0u, r1 = uint(bucket_rows))
        enc_moe_gather_f32_cls(s, fixed_array(r.pf_xb, r.pf_minv, r.pf_mxf), pcgp, nk)
        var pcx = Fp4RowsArgs(k = uint(dim), inbase = 0u)
        enc_mx4_rows(s, fixed_array(r.pf_mxf, r.pf_mx4q, r.pf_mx4s, r.pf_mxrs), pcx, bucket_rows + TILE_READ_SLACK)
        crd_moe_mmq(s, L.fe1, r.pf_msch1, map_m_gu, bounds.mgu, r.pf_mx4q, r.pf_mx4s, r.pf_mxrs, r.pf_mmy1, dim, nfe)
        crd_moe_mmq(s, L.fe3, r.pf_msch3, map_m_gu, bounds.mgu, r.pf_mx4q, r.pf_mx4s, r.pf_mxrs, r.pf_mmy2, dim, nfe)
        var pch = Fp4RowsArgs(k = uint(nfe), inbase = 0u, act = pca.act, gboff = pca.gboff)
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
    if (L.moe_g4) {
        var pcg4 = crd_g4_comb_args(r, L)
        enc_cls_ar_comb_g4(s, fixed_array(r.pf_x, r.pf_minv, r.pf_mw, mdn, r.pf_ffnout, r.norms, r.pf_xb), pcg4, wlen)
    } else {
        var pc14 = crd_comb_args(r, L)
        enc_cls_ar_comb(s, fixed_array(r.pf_x, r.pf_ffnout, r.norms, r.pf_xb, r.pf_minv, r.pf_mw, mdn, r.pf_mlog, r.moe_db, r.pf_midx), pc14, wlen)
    }
    if (!last_layer) {
        crd_x_feeds_from(r, s, r.pf_xb, wlen, crd_head_feeds(r, r.layers[l + 1l]))
        return
    }
    crd_window_cls_feed(r, s, r.pf_xb, w0, segs, wsegs)
}

//! the window's router GEMM over the rows into pf_mlog: the stored normed rows, or (the gemma-4 form) x under a weightless rms and the
//! router scale row, normed into pf_xb first
def private crd_window_router(r : CudRdec; var s : void?; L : CudLayer; wlen : int64) {
    if (L.moe_g4) {
        var pcn = crd_rms_args(r, L.n_router)
        enc_cls_ar(s, fixed_array(r.pf_x, r.pf_ffnout, r.norms, r.pf_xb), pcn, wlen)
    }
    var pcg = RouterGemmArgs(npos = uint(wlen), ne = uint(r.ner), dim = uint(r.dim), woff = uint(L.router_slot * r.ner * r.dim))
    enc_router_gemm_cls(s, fixed_array(r.router, r.pf_xb, r.pf_mlog), pcg, router_gemm_wgs(wlen, r.ner))
}

//! a host layer's window: the shared expert's dense tail into pf_ffnout, the router GEMM over the normed rows, the rows and the logits
//! landed and the stream waited, the host's routed sums over every row (`experts`), the sums up, then the combine over them as the one
//! routed row of weight one a position beside the shared expert's row at its gate, and the next layer's feed
[arch(at="../ARCHITECTURE_GPU_VULKAN_HC.md#hc-window-chain")]
def private crd_window_host_cut(r : CudRdec; var s : void?; l : int64; var L : CudLayer; w0, wlen : int64; segs, wsegs : array<RdecPfSeg>; experts : RdecHostExpertsRowsFn) {
    let dim = r.dim
    if (L.b1 >= 0l) {
        crd_x_feeds_from(r, s, r.pf_xb, wlen, crd_gu_feeds(r, L))
        crd_window_ffn(r, s, L, wlen)
    }
    crd_window_router(r, s, L, wlen)
    if (L.moe_g4) {   // the routed feed the host reads
        crd_g4_feed(r, s, L, r.pf_x, r.pf_ffnout, r.pf_xb, wlen)
    }
    cuda_memcpy_dtoh(r.pf_hx_host, r.pf_xb, uint64(wlen * dim * 4l), s)
    cuda_memcpy_dtoh(r.pf_hlog_host, r.pf_mlog, uint64(wlen * r.ner * 4l), s)
    if (!cud_sync()) {
        panic("dasLLAMA cuda resident: the window's host cut did not land: {cuda_last_error()}")
    }
    unsafe {
        invoke(experts, l, reinterpret<float const?>(r.pf_hx_host), reinterpret<float const?>(r.pf_hlog_host), reinterpret<float?>(r.pf_hacc_host), 0l, wlen)
    }
    cuda_memcpy_htod(r.pf_hacc_dev, r.pf_hacc_host, uint64(wlen * dim * 4l), s)
    if (L.moe_g4) {
        var pcg4 = crd_g4_comb_args(r, L)
        enc_cls_ar_comb_g4(s, fixed_array(r.pf_x, r.pf_ident, r.pf_ones, r.pf_hacc_dev, r.pf_ffnout, r.norms, r.pf_xb), pcg4, wlen)
    } else {
        var pc14 = crd_comb_args(r, L)
        pc14.slots = 1u
        pc14.dbias_on = 0u   // the host summed the bias rows
        enc_cls_ar_comb(s, fixed_array(r.pf_x, r.pf_ffnout, r.norms, r.pf_xb, r.pf_ident, r.pf_ones, r.pf_hacc_dev, r.pf_mlog, r.moe_db, r.pf_midx), pc14, wlen)
    }
    if (l + 1l < r.n_layers) {
        crd_x_feeds_from(r, s, r.pf_xb, wlen, crd_head_feeds(r, r.layers[l + 1l]))
        return
    }
    crd_window_cls_feed(r, s, r.pf_xb, w0, segs, wsegs)
}

//! the classifier's feed off each ending segment's last normed row of `hrows`
def private crd_window_cls_feed(r : CudRdec; var s : void?; hrows : uint64; w0 : int64; segs, wsegs : array<RdecPfSeg>) {
    let dim = r.dim
    for (ws in wsegs) {
        let lr = ws.row0 + ws.npos - 1l
        continue if (rdec_seg_ending_at(segs, w0, lr) < 0l)
        crd_rq_rows(s, r.cls_fmt, hrows + uint64(lr * dim * 4l), r.pf_xq + uint64(lr * dim), r.pf_xs + uint64(lr * (dim / Q8_BLOCK_ELEMS) * 4l), dim)
    }
}

//! a segment ending in the window (`wsegs` its parts, `segs` the call's) takes the classifier at its last row; under `hidden` every
//! row's final residual stays in pf_x for the host and no classifier runs
def private crd_window(var r : CudRdec; w0, wlen : int64; segs, wsegs : array<RdecPfSeg>; last, hidden : bool; experts : RdecHostExpertsRowsFn) {
    var s = g_cud.stream
    let dim = r.dim
    let nlfin = uint(rdec_norms_final_base(r.n_layers, dim))
    let warm = !hidden && crd_window_warm_on(r, wsegs)
    if (warm) {   // the window's embed rows before layer 0 overwrites the residual: the head reads them unscaled
        cuda_memcpy_dtod(r.pf_hemb, r.pf_x, uint64(wlen * dim * 4l), s)
    }
    var hrows = r.pf_xb   // the window's post-norm rows, the head warm's carry halves: the combine's plane on a routed last layer
    let pc_pro = GkAddRmsArgs(dim = uint(dim), add_on = 0u, woff = 0u, eps = r.eps, ascale = 1.0)
    crd_x_norm(r, s, r.pf_x, pc_pro, wlen, crd_head_feeds(r, r.layers[0]))
    for (l in range64(r.n_layers)) {
        var L & = unsafe(r.layers[l])
        let qd = L.qd
        if (L.recurrent) {
            crd_window_dn(r, L, wlen, wsegs)
        } else {
        let kmir = r.k_slabs[L.mir_slab]
        let vmir = r.v_slabs[L.mir_slab]
        crd_window_qkv(r, l, L, wsegs, wlen, false)
        let kvq = kv_block_codec(r.kv_dt)
        crd_window_attn(r, l, L, kvq ? r.ksh : kmir, kvq ? r.vsh : vmir, wsegs, wlen, kvq, r.kv_dt == KVDtype.tq4)
        crd_site(r, L.sch[SCH_O], L.fo, L.bo, r.pf_af, r.pf_aq, r.pf_as, r.pf_asum, r.pf_a4q, r.pf_a4s, r.pf_ars, r.pf_xb2, qd, dim, wlen)
        }
        if (L.moe) {
            crd_window_moe(r, s, l, L, w0, wlen, segs, wsegs, experts)
            continue
        }
        let pc8 = crd_wo_ar_args(r, L)
        crd_x_norm(r, s, r.pf_xb2, pc8, wlen, crd_gu_feeds(r, L))
        crd_window_ffn(r, s, L, wlen)
        let last_layer = l + 1l == r.n_layers
        var pc14 = GkAddRmsArgs(dim = uint(dim), add_on = 1u, woff = last_layer ? nlfin : uint(norm_off(l + 1l, RDEC_NORM_ATT)), eps = r.eps,
            ascale = L.out_scale, pre_on = L.pre_post ? 1u : 0u, pwoff = uint(norm_off(l, RDEC_NORM_POST_FFN)))
        if (L.ple) {   // the FFN's residual step first at scale 1 (its next feed goes unread), then the branch, whose residual step under the ple post norm and the layer's out scale feeds the next head
            var pcf = pc14
            pcf.ascale = 1.0
            crd_x_norm(r, s, r.pf_ffnout, pcf, wlen, crd_head_feeds(r, last_layer ? r.layers[l] : r.layers[l + 1l]))
            crd_window_ple(r, s, l, L, wlen)
            pc14.pre_on = 1u
            pc14.pwoff = uint(L.n_ple_post)
        }
        if (!last_layer) {
            crd_x_norm(r, s, r.pf_ffnout, pc14, wlen, crd_head_feeds(r, r.layers[l + 1l]))
        } elif (hidden) {   // every row's final residual into pf_x, the normed rows unread: the host pools the residual under the final norm
            enc_cls_ar(s, fixed_array(r.pf_x, r.pf_ffnout, r.norms, r.pf_xb2), pc14, wlen)
        } elif (warm || r.carry) {   // the final norm over every row as floats (the head reads them all, the carry lands the ending rows'), the ending rows' classifier feed off the normed rows
            enc_cls_ar(s, fixed_array(r.pf_x, r.pf_ffnout, r.norms, r.pf_xb2), pc14, wlen)
            hrows = r.pf_xb2
            crd_window_cls_feed(r, s, hrows, w0, segs, wsegs)
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
    return if (hidden)
    let ends = crd_window_cls(r, w0, segs, wsegs, hrows)
    verify(!last || ends > 0l, "cud prefill: the last window ends no segment")
    if (warm) {
        crd_window_head_warm(r, w0, wlen, wsegs[0], hrows)
    }
}

//! the head's prompt warm runs over this window: asked by the engine, the head on the device, the planes made, and the window one
//! session's part (a segmented window warms no head row, said once)
[arch(at="../ARCHITECTURE_GPU_VULKAN_MTP.md#resident-head-prompt-warm")]
def private crd_window_warm_on(var r : CudRdec; wsegs : array<RdecPfSeg>) : bool {
    return false if (!r.pf_head_ask || !r.head_on || !r.carry || r.pf_head_declined)
    if (long_length(wsegs) != 1l) {
        r.pf_head_declined = true
        to_log(LOG_WARNING, "dasLLAMA cuda resident: the NextN head's prompt warm takes one session's window, and this window holds {long_length(wsegs)} parts; the head's prompt rows stay as the host holds them\n")
        return false
    }
    if (r.pf_hemb == 0ul) {
        crd_pf_head_setup(r)
    }
    return true
}

//! the head warm's planes, once a residency (the caller holds it to the first head window): the window's embed rows, the shifted
//! carry rows, the two normed halves, the cat rows and their feed in the eh_proj site's form, the head rows' rope rows, the head's
//! three schedules
[cold_path]
def private crd_pf_head_setup(var r : CudRdec) {
    let np = r.pf_np
    let dim = r.dim
    let slack = np + TILE_READ_SLACK
    r.pf_hemb = cud_dev_zeroed(np * dim * 4l)
    r.pf_hsrc = cud_dev_zeroed(np * dim * 4l)
    r.pf_hna = cud_dev_zeroed(np * dim * 4l)
    r.pf_hnb = cud_dev_zeroed(np * dim * 4l)
    r.pf_hcat = cud_dev_zeroed(np * 2l * dim * 4l)
    r.pf_hcos = cud_dev_zeroed(np * r.cos_elems * 4l)
    r.pf_hcatf = cud_dev_zeroed(slack * 2l * dim * 2l)   // the tiles' feed: a window under the leaf's rows takes it whatever window set the planes up
    let feed = crd_feed_of(crd_site_mmq_able(r, r.head_eh_fmt, 2l * dim, dim, r.head_eh_blk), r.head_eh_fmt, r.pf_mmq_min)
    if (feed == CrdFeed.fp4) {
        r.pf_hcat4q = cud_dev_zeroed(np * 2l * dim / FP4_NIBBLES_PER_BYTE)
        r.pf_hcat4s = cud_dev_zeroed(np * 2l * dim / NV4_FEED_STRIP)
        r.pf_hcatrs = cud_dev_zeroed(np * 4l)
    } elif (feed != CrdFeed.f16) {
        r.pf_hcatq = cud_dev_zeroed(np * 2l * dim)
        r.pf_hcats = cud_dev_zeroed(np * (2l * dim / Q8_BLOCK_ELEMS) * 4l)
        if (feed == CrdFeed.q8sum) {
            r.pf_hcatsum = cud_dev_zeroed(np * (2l * dim / Q8_BLOCK_ELEMS) * 4l)
        }
    }
    for (i in range(3)) {
        r.pf_hsch[i] = cud_sched_make(r.pf_sched_words)
    }
}

//! the window's head rows after the trunk: head row j at the part's position less `carry` plus j takes [enorm(embed row j + 1 - carry) ;
//! hnorm(post-norm row j - carry)] - the previous window's last post-norm row at carry row 0 from the second window on - through
//! eh_proj and the head's k and v store into its mirror slot over the head rows' own rope rows (`pf_hcos`)
[arch(at="../ARCHITECTURE_GPU_VULKAN_MTP.md#resident-head-prompt-warm")]
def private crd_window_head_warm(var r : CudRdec; w0, wlen : int64; ws : RdecPfSeg; hrows : uint64) {
    var s = g_cud.stream
    let dim = r.dim
    let rowb = uint64(dim * 4l)
    let carry = w0 > 0l ? 1l : 0l
    let nh = wlen - 1l + carry
    let hb = uint(r.head_norm_base)
    var L & = unsafe(r.head)
    if (nh > 0l) {
        if (wlen > 1l) {
            cuda_memcpy_dtod(r.pf_hsrc + rowb, hrows, rowb * uint64(wlen - 1l), s)
        }
        let src_row0 = uint(1l - carry)
        var pch = GkAddRmsArgs(dim = uint(dim), add_on = 0u, woff = hb + uint(RDEC_HEAD_HNORM * dim), eps = r.eps, ascale = 1.0, row0 = src_row0)
        enc_cls_ar(s, fixed_array(r.pf_hsrc, r.pf_hsrc, r.norms, r.pf_hnb), pch, nh)
        var pce = GkAddRmsArgs(dim = uint(dim), add_on = 0u, woff = hb + uint(RDEC_HEAD_ENORM * dim), eps = r.eps, ascale = 1.0, row0 = src_row0)
        enc_cls_ar(s, fixed_array(r.pf_hemb, r.pf_hemb, r.norms, r.pf_hna), pce, nh)
        var pcc = GkCat2Args(d = uint(dim), rows = uint(nh), a0 = src_row0, b0 = src_row0)
        enc_cat2_cls(s, fixed_array(r.pf_hcat, r.pf_hna, r.pf_hnb), pcc, (nh * dim + 255l) / 256l)
    }
    // the next window's carry row lands at row 0 after the norm above read the previous one there
    cuda_memcpy_dtod(r.pf_hsrc, hrows + rowb * uint64(wlen - 1l), rowb, s)
    return if (nh <= 0l)
    let wide = 2l * dim
    let feed = crd_feed_of(crd_site_mmq(r, r.head_eh_fmt, wide, dim, r.head_eh_blk), r.head_eh_fmt, r.pf_mmq_min)
    if (feed == CrdFeed.f16) {
        var pcv = gk_f16_rows_args(uint(nh * wide))
        enc_tower_clamp_cvt_cls(s, fixed_array(r.pf_hcat, r.pf_hcatf), pcv, rq_wgs(false, nh * wide))
    } elif (feed == CrdFeed.fp4) {
        var pcf = Fp4RowsArgs(k = uint(wide), inbase = 0u)
        enc_fp4_rows(s, fixed_array(r.pf_hcat, r.pf_hcat4q, r.pf_hcat4s, r.pf_hcatrs), pcf, nh)
    } else {
        var pcq = RqArgs(inbase = 0u, nblk = uint(nh * wide / kq_unit(false)), lo = 0.0, hi = 0.0)
        if (feed == CrdFeed.q8sum) {
            enc_cls_dn_rq_sum(s, fixed_array(r.pf_hcat, r.pf_hcatq, r.pf_hcats, r.pf_hcatsum), pcq, rq_wgs(false, nh * wide))
        } else {
            enc_cls_dn_rq(s, fixed_array(r.pf_hcat, r.pf_hcatq, r.pf_hcats), pcq, rq_wgs(false, nh * wide))
        }
    }
    crd_site(r, r.pf_hsch[0], r.head_eh_fmt, r.head_eh_blk, r.pf_hcatf, r.pf_hcatq, r.pf_hcats, r.pf_hcatsum, r.pf_hcat4q, r.pf_hcat4s, r.pf_hcatrs, r.pf_x, wide, dim, nh)
    let pca = GkAddRmsArgs(dim = uint(dim), add_on = 0u, woff = hb + uint(RDEC_HEAD_ATT * dim), eps = r.eps, ascale = 1.0)
    crd_x_norm(r, s, r.pf_x, pca, nh, crd_head_feeds(r, L))
    var hseg <- [RdecPfSeg(row0 = 0l, npos = nh, pos0 = ws.pos0 - carry, region = ws.region)]   // the part's position is the window's first row's already
    crd_window_qkv(r, r.n_layers, L, hseg, nh, true)
    r.pf_head_rows += nh
    delete hseg
}

//! a window site over the window's x feeds (the normed rows in every form the chain made)
def private crd_x_site(r : CudRdec; var sc : CudSched; fmt : int; wblk : int64; y : uint64; d, rows : int64) {
    crd_site(r, sc, fmt, wblk, r.pf_xf, r.pf_xq, r.pf_xs, r.pf_xsum, r.pf_x4q, r.pf_x4s, r.pf_xrs, y, r.dim, d, rows)
}

//! the attention layer's window projections and store: the q site (not under `kv_only`), the k and v sites, the rope and K/V store
//! of the rows into the layer's mirror slot of each part's region; the head warm's rows take `kv_only` with the head's own rope rows
def private crd_window_qkv(var r : CudRdec; l : int64; var L : CudLayer; wsegs : array<RdecPfSeg>; wlen : int64; kv_only : bool) {
    var s = g_cud.stream
    let dim = r.dim
    let kvd = L.kvd
    let kmir = r.k_slabs[L.mir_slab]
    let vmir = r.v_slabs[L.mir_slab]
    if (!kv_only) {
        crd_x_site(r, L.sch[SCH_Q], L.fq, L.bq, r.pf_q, crd_pf_q_row_stride(r, L), wlen)
    }
    let kv_merged = crd_kv_merged(dim, L)
    var voff = kvd
    var kstride = 2l * kvd
    if (L.kv_shared) {   // q alone: the rope runs over it, the store stays off (the kernel's k-head count is 0), the donor's rows serve the attention
        crd_window_kv(r, l, L, kmir, vmir, wsegs, wlen, 0l, kvd, kv_only)
        return
    }
    if (kv_merged) {
        crd_x_site(r, kv_only ? r.pf_hsch[1] : L.sch[SCH_K], L.fk, L.bk, r.pf_kv, 2l * kvd, wlen)
    } elif (L.vfk) {   // no v plane: the rope store takes V off the k rows
        crd_x_site(r, kv_only ? r.pf_hsch[1] : L.sch[SCH_K], L.fk, L.bk, r.pf_kv, kvd, wlen)
        voff = 0l
        kstride = kvd
    } else {
        crd_x_site(r, kv_only ? r.pf_hsch[1] : L.sch[SCH_K], L.fk, L.bk, r.pf_kv, kvd, wlen)
        crd_x_site(r, kv_only ? r.pf_hsch[2] : L.sch[SCH_V], L.fv, L.bv, r.pf_v, kvd, wlen)
        cuda_memcpy_dtod(r.pf_kv + uint64(wlen * kvd * 4l), r.pf_v, uint64(wlen * kvd * 4l), s)
        voff = wlen * kvd
        kstride = kvd
    }
    crd_window_kv(r, l, L, kmir, vmir, wsegs, wlen, voff, kstride, kv_only)
}

//! the window's checkpoint capture records for its parts: a capture inside a part records the row the state stands after and the
//! checkpoint row's float base; the records go up once a window, and each part's count and first record ride its scan dispatch
def private crd_window_caps(r : CudRdec; w0 : int64; segs, wsegs : array<RdecPfSeg>) {
    g_crd_wcaps |> clear()
    g_crd_cap_words |> clear()
    return if (empty(g_crd_caps))
    g_crd_wcaps |> reserve(length(wsegs))
    g_crd_cap_words |> reserve(2l * long_length(g_crd_caps))
    for (ws in wsegs) {
        let sg = segs[ws.seg]
        let part_row0_in_seg = w0 + ws.row0 - sg.row0
        var n = 0l
        let rec = long_length(g_crd_cap_words)
        for (cap in g_crd_caps) {
            continue if (cap.seg != ws.seg || cap.row <= part_row0_in_seg || cap.row >= part_row0_in_seg + ws.npos)
            g_crd_cap_words |> push(uint(cap.row - part_row0_in_seg))
            g_crd_cap_words |> push(uint(((ws.region * int64(RDEC_DN_SNAPS) + cap.slot) * r.snap_row_bytes) / 4l))
            n++
        }
        g_crd_wcaps |> push((n = n, rec = rec))
    }
    if (long_length(g_crd_cap_words) > PF_CAP_WORDS) {
        panic("cud prefill: {long_length(g_crd_cap_words) / 2l} checkpoint captures in one window, past the {PF_CAP_WORDS / 2l} the records hold")
    }
    if (!empty(g_crd_cap_words)) {
        cuda_memcpy_htod(r.pf_cap_dev, unsafe(addr<void?>(g_crd_cap_words[0])), uint64(long_length(g_crd_cap_words) * 4l), g_cud.stream)
    }
}

var private @scratch g_crd_caps : array<RdecPfCap>   //! the next prefill call's checkpoint captures (`cud_rdec_prefill_caps`)
var private @scratch g_crd_wcaps : array<tuple<n : int64; rec : int64>>   //! each window part's captures: their count and first record word
var private @scratch g_crd_cap_words : array<uint>

//! a part's captures: its count and first record word (none where the window planned none)
def private crd_part_caps(wi : int64) : tuple<n : int64; rec : int64> => wi < long_length(g_crd_wcaps) ? g_crd_wcaps[wi] : (n = 0l, rec = 0l)

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
    for (ws, wi in wsegs, count()) {
        let st & = unsafe(L.dn[ws.region])
        let wc = crd_part_caps(int64(wi))
        var pcs = DnScanArgs(npos = uint(ws.npos), cd = uint(cd), kd = uint(r.dn_kd), nvh = uint(nvh), nkh = uint(r.dn_nkh), ds = uint(ds),
            beta_off = uint(DN_SM_BETA), g_off = uint(DN_SM_G), a_off = uint(DN_SM_A), dt_off = uint(DN_SM_DT),
            o_off = uint(DN_O_OFF), di = uint(di), region_state_base = uint(st.state_byte_off / 4l), row0 = uint(ws.row0),
            cap_n = uint(wc.n), cap_rec = uint(wc.rec), cap_state_off = uint(L.snap_off / 4l))
        let caps = wc.n > 0l ? r.pf_cap_dev : r.dummy
        let capst = wc.n > 0l ? r.snap_dev : r.dummy
        enc_dn_scan_cls(s, fixed_array(r.pf_dnconv, st.state_dev, smalls, r.pf_dnws, caps, capst), pcs, dn_scan_wgs(nvh, ds))
        if (wc.n > 0l) {   // the capture's conv history: the taps rows before its row, the rows before the part off the entry image
            let cur = uint(r.pf_dn_cur[ws.region])
            var pcx = DnCapTailArgs(cd = uint(cd), taps = uint(taps), row0 = uint(ws.row0), entry_off = uint(st.hist_float_off) + cur * img,
                cap_rec = uint(wc.rec), cap_hist_off = uint((L.snap_off + st.slot_state_bytes) / 4l))
            enc_dn_cap_tail_cls(s, fixed_array(r.pf_dnqkv, smalls, r.pf_cap_dev, r.snap_dev), pcx, dn_cap_tail_wgs(cd, taps) * wc.n)
        }
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
//! out block, the carry's row beside them off `hrows` (the window's post-norm floats) under the carry; the segments ended
def private crd_window_cls(r : CudRdec; w0 : int64; segs, wsegs : array<RdecPfSeg>; hrows : uint64) : int64 {
    var s = g_cud.stream
    let dim = r.dim
    var ends = 0l
    for (ws in wsegs) {
        let lr = ws.row0 + ws.npos - 1l
        let slot = rdec_seg_ending_at(segs, w0, lr)
        continue if (slot < 0l)
        ends++
        if (r.carry) {   // the segment's carry: its last row's post-norm floats at its slot of the out block
            crd_land(s, r, hrows + uint64(lr * dim * 4l), r.out_hid_word + slot * dim, dim)
        }
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
def private crd_prefill_windows(var r : CudRdec; cos_batch : array<float>; segs : array<RdecPfSeg>; var logits : array<float>; experts : RdecHostExpertsRowsFn; feed : block<(w0 : int64; wlen : int64) : void>) {
    crd_pf_setup(r)
    let hidden = g_crd_hidden_ask   // an embedding's call: every row's final residual lands, no classifier runs, no segment needs a logits slot
    if (empty(segs) || (!hidden && long_length(segs) > CRD_NB)) {
        panic("cud prefill: {long_length(segs)} segments - the out block holds {CRD_NB} logits slots")
    }
    var npos = 0l
    for (sg in segs) {
        npos += sg.npos
    }
    if (hidden) {
        g_crd_hidden |> reserve(npos * r.dim)
        g_crd_hidden |> resize(npos * r.dim)
    }
    if (r.hybrid) {
        crd_dn_begin(r, segs)
    }
    r.pf_head_rows = 0l
    var w0 = 0l
    while (w0 < npos) {
        let wlen = min(r.pf_np, npos - w0)
        let last = w0 + wlen >= npos
        r.pf_wlen = wlen
        rdec_window_segs(segs, w0, wlen, g_crd_wsegs)
        if (long_length(g_crd_wsegs) > 1l) {
            r.seg_windows++
        }
        if (r.hybrid) {
            crd_window_caps(r, w0, segs, g_crd_wsegs)
        }
        invoke(feed, w0, wlen)
        cuda_memcpy_htod(r.pf_cos, unsafe(addr<void?>(cos_batch[w0 * r.cos_elems])), uint64(wlen * r.cos_elems * 4l), g_cud.stream)
        if (r.pf_head_ask && r.head_on && r.carry && !r.pf_head_declined) {   // the head rows' rope rows: a row behind the trunk's from the second window on
            if (r.pf_hemb == 0ul) {
                crd_pf_head_setup(r)
            }
            let carry = w0 > 0l ? 1l : 0l
            let nh = wlen - 1l + carry
            if (nh > 0l) {
                cuda_memcpy_htod(r.pf_hcos, unsafe(addr<void?>(cos_batch[(w0 - carry) * r.cos_elems])), uint64(nh * r.cos_elems * 4l), g_cud.stream)
            }
        }
        if (r.hybrid) {
            crd_dn_zero_fresh(r, g_crd_wsegs)
        }
        crd_window(r, w0, wlen, segs, g_crd_wsegs, last, hidden, experts)
        if (hidden) {   // the window's final residual rows, read off the device under the sync below
            cuda_memcpy_dtoh(unsafe(addr<void?>(g_crd_hidden[w0 * r.dim])), r.pf_x, uint64(wlen * r.dim * 4l), g_cud.stream)
        }
        if (!cud_sync()) {
            panic("dasLLAMA cuda resident: the window chain failed on the device: {cuda_last_error()}")
        }
        if (r.hybrid) {
            crd_dn_window_done(r, g_crd_wsegs)
        }
        r.windows++
        crd_served(r, wlen)
        w0 += wlen
    }
    if (r.hybrid) {
        crd_dn_end(r, segs)
        for (cap in g_crd_caps) {   // a captured row's ring parity is image 0: the capture tail wrote it there
            r.snap_par[segs[cap.seg].region * int64(RDEC_DN_SNAPS) + cap.slot] = 0
        }
        g_crd_caps |> clear()
    }
    return if (hidden)
    for (i in range64(long_length(segs))) {
        crd_logits_out(r, logits, i, i * r.vocab)
    }
}

//! the segments over the host's rows (and, on a per-layer-embedding model, the host's side rows: finished, or gathered for the
//! device to finish where the projection rides it)
def private crd_prefill_x(var r : CudRdec; x_batch, cos_batch, ple_rows : array<float>; segs : array<RdecPfSeg>; var logits : array<float>; experts : RdecHostExpertsRowsFn) {
    if (r.ple > 0l) {
        var npos = 0l
        for (sg in segs) {
            npos = max(npos, sg.row0 + sg.npos)
        }
        verify(long_length(ple_rows) >= r.ple_all * npos, "cud prefill: the per-layer-embedding side rows are short of the rows")
    }
    crd_prefill_windows(r, cos_batch, segs, logits, experts) $(w0, wlen) {
        cuda_memcpy_htod(r.pf_x, unsafe(addr<void?>(x_batch[w0 * r.dim])), uint64(wlen * r.dim * 4l), g_cud.stream)
        if (r.ple > 0l) {
            cuda_memcpy_htod(r.pf_ple, unsafe(addr<void?>(ple_rows[w0 * r.ple_all])), uint64(wlen * r.ple_all * 4l), g_cud.stream)
            if (r.ple_gpu) {
                crd_ple_pre_window(r, g_cud.stream, wlen)
            }
        }
    }
}

//! the segments over token ids: every window's rows gathered off the placed table on the device, and on a per-layer-embedding
//! model its side rows off the per-layer table, finished under the projection
def private crd_prefill_ids(var r : CudRdec; tokens : array<int64>; emb_scale : float; cos_batch : array<float>; segs : array<RdecPfSeg>; var logits : array<float>; experts : RdecHostExpertsRowsFn) {
    assert(crd_emb_on(r), "cud prefill over ids without a token table on the device")
    assert(r.ple <= 0l || crd_side_gather_on(r), "cud prefill over ids on a per-layer-embedding model without its table and projection on the device")
    var words <- [for (tk in tokens); uint(tk)]
    let ids = buf_upload(words, g_cud.stream)
    crd_prefill_windows(r, cos_batch, segs, logits, experts) $(w0, wlen) {
        crd_emb_gather(r, g_cud.stream, ids + uint64(w0 * 4l), r.pf_x, r.pf_x, wlen, emb_scale)
        if (r.ple > 0l) {
            crd_side_gather(r, g_cud.stream, ids + uint64(w0 * 4l), r.pf_ple, r.pf_ple, wlen)
            crd_ple_pre_window(r, g_cud.stream, wlen)
        }
    }
    cuda_mem_free(ids)
}

var private g_crd_no_rows : array<float>   //! the segmented prefill's empty side rows: no per-layer-embedding model reaches its x form

//! the one-session call as one segment at row 0 in the selected region
def private crd_one_seg(r : CudRdec; npos, pos0 : int64) : array<RdecPfSeg> => <- [RdecPfSeg(row0 = 0l, npos = npos, pos0 = pos0, region = r.region)]

//! the prefill over the host's rows (and, on a per-layer-embedding model, the host's finished side rows)
def cud_rdec_prefill(x_batch : array<float>; cos_batch : array<float>; ple_rows : array<float>; ple_raw : bool; npos : int64; pos0 : int64; var logits : array<float>; experts : RdecHostExpertsRowsFn) {
    crd_chain_settle()
    assert(g_crd != null && g_crd.ready, "cud_rdec_prefill before prepare/set_cls")
    var r & = unsafe(*g_crd)
    verify(r.ple <= 0l || ple_raw == r.ple_gpu, "cud prefill: raw per-layer-embedding rows without the projection on device, or finished rows with it")
    var inscope segs <- crd_one_seg(r, npos, pos0)
    crd_prefill_x(r, x_batch, cos_batch, ple_rows, segs, logits, experts)
}

//! the prefill over token ids: every window's rows gathered off the placed table on the device
def cud_rdec_prefill_ids(tokens : array<int64>; emb_scale : float; cos_batch : array<float>; npos, pos0 : int64; var logits : array<float>; experts : RdecHostExpertsRowsFn) {
    crd_chain_settle()
    assert(g_crd != null && g_crd.ready, "cud_rdec_prefill_ids before prepare/set_cls")
    var r & = unsafe(*g_crd)
    var inscope segs <- crd_one_seg(r, npos, pos0)
    crd_prefill_ids(r, tokens, emb_scale, cos_batch, segs, logits, experts)
}

//! the segmented window (the Vulkan home's form): every segment's rows or ids back to back, each segment's K/V into its own
//! region from its own position, its logits at its slot; a block-codec mirror holds one session a window, so the engine's
//! admission keeps a codec load off this form
[arch(at="../ARCHITECTURE_GPU_VULKAN_WINDOW.md#vk-prefill-segments")]
def cud_rdec_prefill_segs(x_batch : array<float>; tokens : array<int64>; use_ids : bool; emb_scale : float; cos_batch : array<float>; segs : array<RdecPfSeg>; var logits : array<float>; experts : RdecHostExpertsRowsFn) {
    crd_chain_settle()
    assert(g_crd != null && g_crd.ready && !empty(segs), "cud_rdec_prefill_segs before prepare/set_cls, or no segment")
    var r & = unsafe(*g_crd)
    verify(!kv_block_codec(r.kv_dt), "cud_rdec_prefill_segs: the window holds one session under a block-codec mirror")
    if (use_ids) {
        crd_prefill_ids(r, tokens, emb_scale, cos_batch, segs, logits, experts)
    } else {
        verify(r.ple <= 0l, "cud prefill: a segmented window over the host's rows on a per-layer-embedding model - the engine takes the ids form there")
        crd_prefill_x(r, x_batch, cos_batch, g_crd_no_rows, segs, logits, experts)
    }
}

//! the windows that held several sessions' chunks as segments in this process (a cell's witness)
def cud_rdec_prefill_segment_windows : int64 => g_crd != null ? g_crd.seg_windows : 0l

//! the next prefill call's checkpoint captures (`RdecPrefillCapsFn`): a capture's state and conv history land in its checkpoint row
//! from inside the window (the scan's capture arm, the capture tail)
[arch(at="../ARCHITECTURE_GPU_VULKAN_WINDOW.md#vk-prefill-dn-block")]
def cud_rdec_prefill_caps(caps : array<RdecPfCap>) {
    cud_bind()
    g_crd_caps := caps
    return if (empty(caps) || g_crd == null || !g_crd.hybrid)
    var r & = unsafe(*g_crd)
    crd_snap_alloc(r)
    if (r.pf_cap_dev == 0ul) {
        r.pf_cap_dev = cud_dev_zeroed(PF_CAP_WORDS * 4l)
    }
}

var private g_crd_hidden_ask = false
var private @scratch @exact_size g_crd_hidden : array<float>   //! the last chain's final residual rows, [npos x dim], where it was asked for them

//! Whether the next chains land every row's final residual - the rows an embedding pools - in place of the logits; the ask stands
//! until the next ask (`RdecHiddenAskFn`)
[hot_path, arch(at="../ARCHITECTURE_GPU_VULKAN_WINDOW.md#vk-prefill-hidden-rows")]   //! per prefill call, reached through the tier's registered seat
def cud_rdec_hidden_ask(on : bool) {
    g_crd_hidden_ask = on
}

//! `nrows` of the last chain's final residual rows from window row `src_row` into `dst` at element `dst_off` (`RdecHiddenRowsFn`)
[hot_path]   //! per prefill call, reached through the tier's registered seat
def cud_rdec_hidden_rows(var dst : array<float>; src_row, dst_off, nrows : int64) {
    cud_bind()
    let dim = g_crd.dim
    copy_floats(g_crd_hidden, src_row * dim, dst, dst_off, nrows * dim)
}

//! the engine's per-call ask for the head's prompt warm (`RdecHeadWarmAskFn`)
def cud_rdec_head_warm_ask(on : bool) {
    if (g_crd != null) {
        g_crd.pf_head_ask = on
    }
}

//! the head rows the last prefill call's window chain warmed (`RdecHeadWarmRowsFn`)
[arch(at="../ARCHITECTURE_GPU_VULKAN_MTP.md#resident-head-prompt-warm")]
def cud_rdec_head_warm_rows : int64 => g_crd != null ? g_crd.pf_head_rows : 0l

//! layer `l`'s mirror rows [row0, row0 + nrows) up from the host cache (`kp` / `vp` at row 0); a recurrent layer keeps no K/V
def cud_rdec_kv_sync(l, row0, nrows : int64; kp : uint8 const?; vp : uint8 const?) {
    crd_chain_settle()
    assert(g_crd != null, "cud_rdec_kv_sync before prepare")
    return if (nrows <= 0l)
    let L & = crd_layer_at(*g_crd, l)
    return if (L.recurrent || L.kv_shared)
    let units = kv_row_units(g_crd.kv_dt, L.kvd)
    let rowb = units * g_crd.unit_bytes
    let mir_base = crd_mir_base(*g_crd, L)
    kv_ring_runs(row0, row0 + nrows, L.ring) $(p0, n, slot : int64) {
        let base = uint64((mir_base + slot * units) * g_crd.unit_bytes)
        unsafe {
            cuda_memcpy_htod(g_crd.k_slabs[L.mir_slab] + base, reinterpret<void?>(kp + p0 * rowb), uint64(n * rowb), g_cud.stream)
            cuda_memcpy_htod(g_crd.v_slabs[L.mir_slab] + base, reinterpret<void?>(vp + p0 * rowb), uint64(n * rowb), g_cud.stream)
        }
    }
}

//! ... and back into the host cache
[arch(at="../ARCHITECTURE_GPU.md#gpu-backends")]
def cud_rdec_kv_read(l, row0, nrows : int64; var kp : uint8?; var vp : uint8?) {
    crd_chain_settle()
    crd_kv_read_queue(l, row0, nrows, kp, vp)
    verify(cud_sync(), "cud decode: the K/V readback did not land - the caller reads the host rows as soon as this returns")
}

//! the readback's copies queued on the stream, the wait the caller's (`cud_rdec_kv_read` one span, `cud_rdec_kv_xfer` a batch)
def private crd_kv_read_queue(l, row0, nrows : int64; var kp : uint8?; var vp : uint8?) {
    assert(g_crd != null, "cud_rdec_kv_read before prepare")
    return if (nrows <= 0l)
    let L & = crd_layer_at(*g_crd, l)
    return if (L.recurrent || L.kv_shared)
    let units = kv_row_units(g_crd.kv_dt, L.kvd)
    let rowb = units * g_crd.unit_bytes
    let mir_base = crd_mir_base(*g_crd, L)
    kv_ring_runs(row0, row0 + nrows, L.ring) $(p0, n, slot : int64) {
        let base = uint64((mir_base + slot * units) * g_crd.unit_bytes)
        unsafe {
            cuda_memcpy_dtoh(reinterpret<void?>(kp + p0 * rowb), g_crd.k_slabs[L.mir_slab] + base, uint64(n * rowb), g_cud.stream)
            cuda_memcpy_dtoh(reinterpret<void?>(vp + p0 * rowb), g_crd.v_slabs[L.mir_slab] + base, uint64(n * rowb), g_cud.stream)
        }
    }
}

//! the token command's attention split count (0 before the driver is armed): the pieces a row's keys split into past the unsplit position
def cud_rdec_attn_nsplit : int64 => g_crd != null ? g_crd.attn_nsplit : 0l

//! the token steps the attention's unsplit form served so far (a cell's witness)
def cud_rdec_unsplit_tokens : int64 => g_crd != null ? g_crd.unsplit_steps : 0l

//! the driver's counters; `unsplit_form` says the device splits the keys at all, so the unsplit form is a form of its own
def cud_rdec_stats : tuple<tokens : int64; graph : bool; windows : int64; unsplit : int64; unsplit_form : bool> =>
    (tokens = g_crd != null ? g_crd.tokens_served : 0l, graph = g_crd != null && crd_graph_captured(*g_crd), windows = g_crd != null ? g_crd.windows : 0l,
        unsplit = g_crd != null ? g_crd.unsplit_steps : 0l, unsplit_form = g_crd != null && g_crd.unsplit_on)
