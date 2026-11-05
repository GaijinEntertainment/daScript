options gen2
options _dasllama_internal = true

module dasllama_dflash shared public

// dasllama_dflash.das - the DFlash2 block drafter on the CPU route: the `dflash` GGUF's loader, the per-session context
// ring the target's layer outputs feed, the one pass that drafts a block of seven from the anchor, and the selector's walk.
// The round is the common one (`mtp_spec_round_k`): this file fills its block-drafter seams.

require dasllama/dasllama_lint public
require dasllama/dasllama_common   // Model / Session, the round's block seams, embed_row, mm_b_f
require dasllama/dasllama_gguf     // the container reader (+ f16_to_f32 through its public re-export)
require dasllama/dasllama_math     // reserve_resize
require dasllama/dasllama_math_default   // matmul_kq: the CPU decode's K-quant plane GEMV over disk-order planes
require dasllama/dasllama_convert   // quantize_q8_k_into_ptr, dequant_kq_plane_sb: the K-quant planes' activation rows and codebook rows
require dasllama/dasllama_par      // maybe_parallel_for: the q8 rows thread when a jobque is up
require daslib/enum_trait   // the refusal names the KqFmt member a disk type is
require daslib/fio
require daslib/strings_boost
require daslib/random   // random_float: a sampled draft's draw off the session's rng
require strings
require math

let DFLASH_ARCH = "dflash"
let private Q8B = 34l   //! a Q8_0 block's bytes: an f16 scale and 32 quants
let private Q8E = 32l
let private DFLASH_FMTS = fixed_array(KqFmt.q8, KqFmt.k4, KqFmt.k5, KqFmt.k6)   //! the plane formats both arms of the pass serve

//! a weight plane in its own format: a Q8_0 plane at a byte offset into `blob`, a K-quant plane at an element offset into
//! its format's `kq` planes
struct DflashW {
    fmt : KqFmt
    off : int64
}

//! The drafter as the `dflash` GGUF holds it: its Q8_0 planes verbatim in `blob`, its K-quant planes in their formats'
//! planes as the trunk's loader lays them (`kq`, disk order), the f32 norms and conv bases in `fblob`
struct DflashDrafter {
    ok : bool
    why : string
    dim : int64
    hidden : int64
    vocab : int64
    n_layers : int64
    n_heads : int64
    n_kv : int64
    hs : int64
    nblock : int64       //! the pass's rows: the anchor and nblock - 1 masks
    conv_k : int64
    conv_g : int64       //! channels a dynamic coefficient
    rank : int64
    topk : int64
    window : int64
    eps : float
    theta : float
    mask_id : int64
    target_layers : array<int64>   //! trunk layers (0-based) whose OUTPUT rows the context reads, in fc's order
    blob : array<uint8>
    @safe_when_uninitialized kq : KqPlanes[KQ_FMT_COUNT]
    fblob : array<float>
    fc : DflashW
    sel_h : DflashW
    sel_prev : DflashW
    sel_next : DflashW
    hnorm_off : int64
    norm_off : int64
    wq : array<DflashW>
    wk : array<DflashW>
    wv : array<DflashW>
    wo : array<DflashW>
    w1 : array<DflashW>
    w3 : array<DflashW>
    w2 : array<DflashW>
    aconv_proj : array<DflashW>
    fconv_proj : array<DflashW>
    attn_norm_offs : array<int64>
    ffn_norm_offs : array<int64>
    qn_offs : array<int64>
    kn_offs : array<int64>
    aconv_base_offs : array<int64>   //! f32 [side][tap][dim]
    fconv_base_offs : array<int64>
}

//! A session's drafter state: the context ring (a layer's K and V rows a slot, slot = position mod window), the positions it
//! holds, and the target's captured layer outputs of the last forward's rows
struct private DflashSeq {
    ring_k : array<float>   //! [L][window][n_kv * hs]
    ring_v : array<float>
    ctx : int64             //! positions 0 .. ctx-1 committed (the ring holds the last `window`)
    valid_lo : int64        //! the lowest position whose slot is valid (a gap in the commits moves it)
    caps : array<float>     //! [rows][ncap][dim]
    caps_rows : int64
    caps_pos0 : int64
}

var private g_dflash : DflashDrafter
var private g_dflash_on = false
var private g_dflash_hc = 0l     //! the attached trunk's hyper-connection stream count (0 = a single residual stream): the capture's mean
var private g_dflash_trace = false
var private g_seqs : table<uint64; DflashSeq>

//! A device home's arm of the drafter, registered under the decode override's name: `ready` uploads the drafter's planes for `t`
//! (false = the home declines, the CPU arm serves); `commit` writes the rows' context (`caps` [rows][slot][dim]) into the session's
//! device ring at `pos0 ..`; `pass` runs the block of `rows` (the anchor and the masks) at `pos` against the ring's positions
//! `lo .. lo + nctx - 1` and lands the first `nd` draft rows' logits ([nd][vocab]) and selector projections ([nd][rank]); `drop`
//! frees a session's ring; `detach` every plane. Where `selects` (a sampled round's drafts asked with `soft`), `select_pass` runs
//! the same pass with the selector on the device too: its `nd` picks landed into drafts[1 ..], or, where the round lets it hold,
//! left on the device as the verify's rows, the pass committed unwaited - `land` then waits it and lands the picks, its GPU
//! milliseconds back (negative = failed); at `temp` above 0 the picks are drawn off `rng` as `dflash_walk_sampled` draws them,
//! each position's candidates and q landed into `cids` / `qs` [nd][topk] - by `land` where the pass held. `may_hold` = the round
//! lands no selection (the greedy hold); `may_hold_ahead` = its verify reads no drafted id on the host before the join
struct DflashDeviceSeams {
    ready : function<(t : Model; dr : DflashDrafter) : bool>
    commit : function<(dr : DflashDrafter; uid : uint64; caps : array<float>; rows, pos0 : int64) : bool>
    draft_pass : function<(t : Model; dr : DflashDrafter; uid : uint64; tok, pos, rows, nd, lo, nctx : int64; var logits : array<float>; var hsel : array<float>) : bool>
    selects : function<(soft : bool) : bool>
    select_pass : function<(t : Model; dr : DflashDrafter; uid : uint64; tok, pos, rows, nd, lo, nctx : int64; may_hold, may_hold_ahead : bool; temp : float;
                            var rng : int4&; var drafts : array<int64>; var cids : array<int64>; var qs : array<float>) : DflashSelect>
    land : function<(var drafts : array<int64>; nd : int64; var rng : int4&; var cids : array<int64>; var qs : array<float>) : double>
    drop : function<(uid : uint64) : void>
    detach : function<() : void>
}

//! how a device pass with the selector on the device ended
enum DflashSelect {
    failed
    landed   //! the picks are in the drafts
    held     //! the picks are the verify's rows on the device, `land` lands them
}
var private g_dflash_dev : table<string; DflashDeviceSeams>
var private g_dflash_dev_on = false   //! the attached drafter's ring and pass are the active home's
var private g_dflash_dev_knob = true
var private g_dflash_land_nd = 0l       //! the last draft's picks the round's land still owes the record
var private g_dflash_land_rows = 0l
var private g_dflash_held = false       //! ... left on the device as the verify's rows
var private g_dflash_held_soft = false  //! ... drawn: their candidates and q land with them
var private g_dflash_record = false
var private g_dflash_recorded : array<int64>
var private g_dflash_draft_temp = -1.0   //! a rejection-sampling round's drafter temperature; negative = the stream's sampler temperature, 0 = the greedy walk, its drafts a point-mass q
var private g_dflash_point_q = false    //! the round in flight records its greedy drafts as point masses at the land

def register_dflash_device(name : string; var seams : DflashDeviceSeams) {
    g_dflash_dev[name] <- seams
}

//! take the device arm where the active home registered one (default); false = the CPU reference arm, whatever the home
def set_dflash_device(on : bool) {
    g_dflash_dev_knob = on
}

def dflash_on_device() : bool => g_dflash_dev_on

var private g_dflash_home = ""   //! the home whose seams the attach found ready; the override may move after it
var private g_dflash_release_listening = false

//! the attached home's seams; valid while `g_dflash_dev_on` (the key exists: the attach checked it)
def private dev_seams() : DflashDeviceSeams? {
    unsafe {
        return addr(g_dflash_dev[g_dflash_home])
    }
}

//! print every commit's last row as a sum of magnitudes a captured layer (the two routes' captures compared by eye)
def set_dflash_trace(on : bool) {
    g_dflash_trace = on
}

def dflash_trace_on() : bool => g_dflash_trace

//! keep every round's drafts, in order, from now on (`on`), the record cleared either way
def set_dflash_record(on : bool) {
    g_dflash_record = on
    g_dflash_recorded |> clear()
}

//! the drafts every round drafted since `set_dflash_record(true)`, in order
def dflash_recorded_drafts : array<int64> => clone_to_move(g_dflash_recorded)

//! The drafter's own temperature under a rejection-sampling round (`MtpAccept.rejection` on a sampled stream): above 0 each
//! position's draft is drawn from the softmax of its top-k selector scores at it, on the device selector where it serves, else on
//! the host; 0 keeps the greedy walk; negative (default) takes the stream's own temperature
[arch(at="../ARCHITECTURE_GPU_MTP_ACCEPT.md#dflash-sampled-drafts")]
def set_dflash_draft_temp(temp : float) {
    g_dflash_draft_temp = temp
    to_log(LOG_INFO, "dasLLAMA: set_dflash_draft_temp({temp}) - a rejection-sampling round's block drafts {temp < 0.0 ? "draw each position from its candidates' softmax at the stream's own temperature (greedy under an argmax sampler)" : temp > 0.0 ? "draw each position from its candidates' softmax, on the device selector where it serves" : "walk greedy, each a point-mass q"}\n")
}

def get_dflash_draft_temp() : float => g_dflash_draft_temp

//! the load refused; the first reason stands, since the planes after a refused one are still planned
def private refuse(var dr : DflashDrafter; why : string) : bool {
    if (dr.ok) {
        dr.ok = false
        dr.why = why
    }
    return false
}

//! the index in `DFLASH_FMTS` of the format disk type `gt` serves as (-1 = none)
def private dflash_fmt_index(gt : int) : int {
    for (f, i in DFLASH_FMTS, count()) {
        return i if (kq_ggml_type(f) == gt)
    }
    return -1
}

//! why site `name` of disk type `gt` is refused: the type, the `KqFmt` member it is where one is, the formats the drafter serves
def private dflash_fmt_refusal(name : string; gt : int) : string {
    var member = ""
    for (f in type<KqFmt>) {
        if (kq_ggml_type(f) == gt) {
            member = " ({f})"
            break
        }
    }
    let served <- [for (f in DFLASH_FMTS); ggml_type_name(kq_ggml_type(f))]
    return "'{name}' is {ggml_type_name(gt)}{member}, the drafter serves its planes in {join(served, ", ")} alone"
}

//! a superblock index rounded up to even: a split scale plane's d row (two bytes a superblock) binds four-aligned there
def private even_sb(sb : int64) : int64 => (sb + 1l) & ~1l

//! the kq planes reserved for every K-quant tensor the file carries, so the planes' appends never move them
def private reserve_kq_planes(var dr : DflashDrafter; m : GGUFMeta) {
    var sbs : int64[KQ_FMT_COUNT]
    for (ti in m.tensors) {
        let fi = dflash_fmt_index(ti.ggml_type)
        continue if (fi < 0 || !kq_sb(DFLASH_FMTS[fi]))
        let f = DFLASH_FMTS[fi]
        sbs[int(f)] = even_sb(sbs[int(f)]) + ti.n_elem / kq_elems(f)
    }
    for (f in DFLASH_FMTS) {
        continue if (!kq_sb(f) || sbs[int(f)] == 0l)
        dr.kq[int(f)].q |> reserve(sbs[int(f)] * kq_qsb(f))
        dr.kq[int(f)].s |> reserve(sbs[int(f)] * kq_ssb(f))
    }
}

//! tensor `name`'s `n` elements transcoded into the format's planes at an even superblock; its element offset back
def private append_kq(var p : KqPlanes; m : GGUFMeta; bytes : array<uint8> | #; name : string; fmt : KqFmt; n : int64) : int64 {
    let sb0 = even_sb(long_length(p.q) / kq_qsb(fmt))
    let sb1 = sb0 + n / kq_elems(fmt)
    p.q |> resize(sb1 * kq_qsb(fmt))
    p.s |> resize(sb1 * kq_ssb(fmt))
    gguf_transcode_kq(m, bytes, name, fmt, p.q, p.s, sb0 * kq_elems(fmt), n)
    return sb0 * kq_elems(fmt)
}

//! tensor `name` ([rows][k]) as the plane its disk type serves: Q8_0 blocks verbatim into the blob, a K-quant into its format's
//! planes; `off` -1 = refused
def private plan_w(var dr : DflashDrafter; m : GGUFMeta; bytes : array<uint8> | #; name : string; k, rows : int64) : DflashW {
    let ti = gguf_find_tensor(m, name)
    if (ti < 0) {
        refuse(dr, "missing tensor '{name}'")
        return DflashW(off = -1l)
    }
    let gt = m.tensors[ti].ggml_type
    let fi = dflash_fmt_index(gt)
    if (fi < 0) {
        refuse(dr, dflash_fmt_refusal(name, gt))
        return DflashW(off = -1l)
    }
    let fmt = DFLASH_FMTS[fi]
    if (m.tensors[ti].n_elem != k * rows) {
        refuse(dr, "'{name}' holds {m.tensors[ti].n_elem} elements, expected {k * rows}")
        return DflashW(off = -1l)
    }
    if (!kq_rows_ok(k, fmt)) {
        refuse(dr, "'{name}' rows of {k} are off the {ggml_type_name(gt)} ({fmt}) block")
        return DflashW(off = -1l)
    }
    if (fmt != KqFmt.q8) {
        return DflashW(fmt = fmt, off = append_kq(dr.kq[int(fmt)], m, bytes, name, fmt, k * rows))
    }
    let off = long_length(dr.blob)
    var raw : array<uint8>
    gguf_read_tensor_raw(m, bytes, name, raw)
    dr.blob |> push_from(raw)
    delete raw
    return DflashW(fmt = fmt, off = off)
}

//! append tensor `name`'s f32 values to fblob; its offset back (-1 = refused)
def private plan_f32(var dr : DflashDrafter; m : GGUFMeta; bytes : array<uint8> | #; name : string; n : int64) : int64 {
    let ti = gguf_find_tensor(m, name)
    if (ti < 0 || m.tensors[ti].n_elem != n) {
        refuse(dr, ti < 0 ? "missing tensor '{name}'" : "'{name}' holds {m.tensors[ti].n_elem} elements, expected {n}")
        return -1l
    }
    let off = long_length(dr.fblob)
    dr.fblob |> reserve_resize(off + n)
    gguf_read_tensor_f32(m, bytes, name, dr.fblob, off, n)
    return off
}

def private read_meta(var dr : DflashDrafter; m : GGUFMeta; bytes : array<uint8> | #) : bool {
    if (!gguf_has(m, "general.architecture")) {
        return refuse(dr, "no general.architecture (not a GGUF model file)")
    }
    let arch = gguf_str(m, bytes, "general.architecture")
    if (arch != DFLASH_ARCH) {
        return refuse(dr, "architecture is '{arch}', expected '{DFLASH_ARCH}'")
    }
    let p = "{DFLASH_ARCH}."
    for (k in ["block_count", "embedding_length", "feed_forward_length", "attention.head_count", "attention.head_count_kv",
               "attention.key_length", "attention.layer_norm_rms_epsilon", "rope.freq_base", "block_size", "conv_kernel_size",
               "conv_group_size", "selector_rank", "selector_top_k", "target_layers", "attention.sliding_window"]) {
        if (!gguf_has(m, "{p}{k}")) {
            return refuse(dr, "missing metadata key '{p}{k}'")
        }
    }
    dr.n_layers = gguf_int(m, bytes, "{p}block_count")
    dr.dim = gguf_int(m, bytes, "{p}embedding_length")
    dr.hidden = gguf_int(m, bytes, "{p}feed_forward_length")
    dr.n_heads = gguf_int(m, bytes, "{p}attention.head_count")
    dr.n_kv = gguf_int(m, bytes, "{p}attention.head_count_kv")
    dr.hs = gguf_int(m, bytes, "{p}attention.key_length")
    dr.eps = gguf_f32(m, bytes, "{p}attention.layer_norm_rms_epsilon")
    dr.theta = gguf_f32(m, bytes, "{p}rope.freq_base")
    dr.nblock = gguf_int(m, bytes, "{p}block_size")
    dr.conv_k = gguf_int(m, bytes, "{p}conv_kernel_size")
    dr.conv_g = gguf_int(m, bytes, "{p}conv_group_size")
    dr.rank = gguf_int(m, bytes, "{p}selector_rank")
    dr.topk = gguf_int(m, bytes, "{p}selector_top_k")
    dr.window = gguf_int(m, bytes, "{p}attention.sliding_window")
    dr.mask_id = gguf_int_or(m, bytes, "tokenizer.ggml.mask_token_id", -1l)
    let tl <- gguf_int_array(m, bytes, "{p}target_layers")
    dr.target_layers |> reserve(length(tl))
    for (l in tl) {
        dr.target_layers |> push(l - 1l)   //! the file counts a layer's output from one: the trunk's 0-based layer whose output it is
    }
    if (dr.n_layers <= 0l || dr.dim <= 0l || dr.hidden <= 0l || dr.n_heads <= 0l || dr.n_kv <= 0l || dr.hs <= 0l
        || dr.n_heads % dr.n_kv != 0l || empty(dr.target_layers) || dr.conv_g <= 0l || dr.window <= 0l) {
        return refuse(dr, "degenerate geometry (layers {dr.n_layers} dim {dr.dim} heads {dr.n_heads}/{dr.n_kv} targets {length(dr.target_layers)} conv group {dr.conv_g} window {dr.window})")
    }
    if (dr.nblock < 2l || dr.nblock > MTP_MAX_ROWS || dr.conv_k != 2l || dr.dim % dr.conv_g != 0l || dr.topk < 1l || dr.rank <= 0l || dr.mask_id < 0l) {
        return refuse(dr, "unsupported block {dr.nblock}, conv kernel {dr.conv_k}, group {dr.conv_g}, top-k {dr.topk}, rank {dr.rank}, mask id {dr.mask_id}")
    }
    let sp = gguf_find_tensor(m, "selector_predecessor.weight")
    if (sp < 0 || length(m.tensors[sp].dims) < 2) {
        return refuse(dr, "missing or malformed selector_predecessor.weight")
    }
    dr.vocab = m.tensors[sp].dims[1]
    return true
}

def private plan_planes(var dr : DflashDrafter; m : GGUFMeta; bytes : array<uint8> | #) : bool {
    let d = dr.dim
    let ncap = long_length(dr.target_layers)
    reserve_kq_planes(dr, m)
    dr.fc = plan_w(dr, m, bytes, "fc.weight", ncap * d, d)
    dr.sel_h = plan_w(dr, m, bytes, "selector_hidden.weight", d, dr.rank)
    dr.sel_prev = plan_w(dr, m, bytes, "selector_predecessor.weight", dr.rank, dr.vocab)
    dr.sel_next = plan_w(dr, m, bytes, "selector_successor.weight", dr.rank, dr.vocab)
    dr.hnorm_off = plan_f32(dr, m, bytes, "enc.output_norm.weight", d)
    dr.norm_off = plan_f32(dr, m, bytes, "output_norm.weight", d)
    let qd = dr.n_heads * dr.hs
    let kvd = dr.n_kv * dr.hs
    let projw = 2l * dr.conv_k * (d / dr.conv_g)
    dr.wq |> reserve(dr.n_layers)
    dr.wk |> reserve(dr.n_layers)
    dr.wv |> reserve(dr.n_layers)
    dr.wo |> reserve(dr.n_layers)
    dr.w1 |> reserve(dr.n_layers)
    dr.w3 |> reserve(dr.n_layers)
    dr.w2 |> reserve(dr.n_layers)
    dr.aconv_proj |> reserve(dr.n_layers)
    dr.fconv_proj |> reserve(dr.n_layers)
    dr.attn_norm_offs |> reserve(dr.n_layers)
    dr.ffn_norm_offs |> reserve(dr.n_layers)
    dr.qn_offs |> reserve(dr.n_layers)
    dr.kn_offs |> reserve(dr.n_layers)
    dr.aconv_base_offs |> reserve(dr.n_layers)
    dr.fconv_base_offs |> reserve(dr.n_layers)
    for (l in range64(dr.n_layers)) {
        dr.wq |> push(plan_w(dr, m, bytes, "blk.{l}.attn_q.weight", d, qd))
        dr.wk |> push(plan_w(dr, m, bytes, "blk.{l}.attn_k.weight", d, kvd))
        dr.wv |> push(plan_w(dr, m, bytes, "blk.{l}.attn_v.weight", d, kvd))
        dr.wo |> push(plan_w(dr, m, bytes, "blk.{l}.attn_output.weight", qd, d))
        dr.w1 |> push(plan_w(dr, m, bytes, "blk.{l}.ffn_gate.weight", d, dr.hidden))
        dr.w3 |> push(plan_w(dr, m, bytes, "blk.{l}.ffn_up.weight", d, dr.hidden))
        dr.w2 |> push(plan_w(dr, m, bytes, "blk.{l}.ffn_down.weight", dr.hidden, d))
        dr.aconv_proj |> push(plan_w(dr, m, bytes, "blk.{l}.attn_conv_proj.weight", d, projw))
        dr.fconv_proj |> push(plan_w(dr, m, bytes, "blk.{l}.ffn_conv_proj.weight", d, projw))
        dr.attn_norm_offs |> push(plan_f32(dr, m, bytes, "blk.{l}.attn_norm.weight", d))
        dr.ffn_norm_offs |> push(plan_f32(dr, m, bytes, "blk.{l}.ffn_norm.weight", d))
        dr.qn_offs |> push(plan_f32(dr, m, bytes, "blk.{l}.attn_q_norm.weight", dr.hs))
        dr.kn_offs |> push(plan_f32(dr, m, bytes, "blk.{l}.attn_k_norm.weight", dr.hs))
        dr.aconv_base_offs |> push(plan_f32(dr, m, bytes, "blk.{l}.attn_conv_base", 2l * dr.conv_k * d))
        dr.fconv_base_offs |> push(plan_f32(dr, m, bytes, "blk.{l}.ffn_conv_base", 2l * dr.conv_k * d))
        return false if (!empty(dr.why))
    }
    return empty(dr.why)
}

//! The drafter at `path`, whole in memory; `ok` false names why it refused
def load_dflash_drafter(path : string) : DflashDrafter {
    var dr <- DflashDrafter(ok = true)
    let read = gguf_header_read(path) $(m; bytes) {
        if (read_meta(dr, m, bytes)) {
            plan_planes(dr, m, bytes)
        }
    }
    if (!read && dr.ok) {
        refuse(dr, "cannot read {path} as a GGUF")
    }
    return <- dr
}

//! why drafter `dr` cannot draft for `t` ("" = it can): the trunk's width, vocabulary and depth, and the classifier the
//! draft rows take (the batched classifier over the trunk's own plane)
def dflash_target_why(t : Model; dr : DflashDrafter) : string {
    let c = t.config
    return "trunk dim {c.dim} is not the drafter's {dr.dim}" if (c.dim != dr.dim)
    return "trunk vocabulary {c.vocab_size} is not the drafter's {dr.vocab}" if (c.vocab_size != dr.vocab)
    for (l in dr.target_layers) {
        return "the drafter reads layer {l}'s output of a {c.n_layers}-layer trunk" if (l < 0l || l >= c.n_layers)
    }
    let cls_fmt = c.shared_weights ? t.emb_fmt : t.wcls_fmt
    let tied_f32 = c.shared_weights && !t.cls_q8 && cls_fmt == KqFmt.q8
    return "the trunk's tied f32 classifier has no batched form for the draft rows" if (tied_f32)
    return ""
}

//! the drafter attached serves `t`: one drafter a process, and a trunk of its target's geometry is its target
def private dflash_attached(t : Model) : bool => g_dflash_on && empty(dflash_target_why(t, g_dflash))

def private dflash_release(uid : uint64) {
    if (key_exists(g_seqs, uid)) {
        g_seqs |> erase(uid)
    }
    if (g_dflash_dev_on) {
        invoke(dev_seams().drop, uid)
    }
}

//! Attach the drafter at `path` to `t`; "" = attached, else why not (logged)
def attach_dflash_drafter(t : Model; path : string) : string {
    var dr <- load_dflash_drafter(path)
    if (!dr.ok) {
        to_log(LOG_WARNING, "dasLLAMA: DFlash2 drafter {path} refused - {dr.why}\n")
        let why = dr.why
        delete dr
        return why
    }
    let why = dflash_target_why(t, dr)
    if (!empty(why)) {
        to_log(LOG_WARNING, "dasLLAMA: DFlash2 drafter {path} refused for this trunk - {why}\n")
        delete dr
        return why
    }
    detach_dflash_drafter()
    delete g_dflash
    g_dflash <- dr
    g_dflash_on = true
    g_dflash_hc = t.config.hyper_conn ? t.config.hc_count : 0l
    let home = active_decode_override()
    g_dflash_dev_on = g_dflash_dev_knob && key_exists(g_dflash_dev, home) && invoke(g_dflash_dev[home].ready, t, g_dflash)
    g_dflash_home = g_dflash_dev_on ? home : ""
    var layers := g_dflash.target_layers
    register_mtp_block_drafter(MtpBlockSeams(attached = @@dflash_attached, captured = @@dflash_capture, captured_rows = @@dflash_capture_rows,
        draft = @@dflash_block_draft, land = @@dflash_land, commit = @@dflash_commit, layers <- layers, drafts = g_dflash.nblock - 1l))
    if (!g_dflash_release_listening) {   //! the listener registry has no unregister: one registration a process
        register_session_release_listener(@@dflash_release)
        g_dflash_release_listening = true
    }
    to_log(LOG_INFO, "dasLLAMA: DFlash2 drafter {base_name(path)} attached - {g_dflash.n_layers} layers, block {g_dflash.nblock}, top-{g_dflash.topk} over {length(g_dflash.target_layers)} target layers, the pass on the {g_dflash_dev_on ? home : (g_dflash_dev_knob ? "CPU" : "CPU (set_dflash_device(false))")}\n")
    return ""
}

def detach_dflash_drafter() {
    return if (!g_dflash_on)
    g_dflash_on = false
    g_dflash_hc = 0l
    g_dflash_land_nd = 0l
    g_dflash_held = false
    g_dflash_held_soft = false
    if (g_dflash_dev_on) {
        invoke(dev_seams().detach)
        g_dflash_dev_on = false
    }
    delete g_dflash
    delete g_seqs
    unregister_mtp_block_drafter()
}

def dflash_drafter_attached(t : Model) : bool => dflash_attached(t)

//! o[ooff ..+n) = x[xoff ..] * w[woff ..] / rms(x)
def private rms_w(var o : array<float>; ooff : int64; x : array<float>; xoff : int64; w : array<float>; woff, n : int64; eps : float) {
    unsafe {
        rmsnorm(addr(o[ooff]), addr<float const?>(x[xoff]), addr<float const?>(w[woff]), n, eps)
    }
}

//! the dot of a q8 row at `bp` (`n` inputs in 34-byte blocks) with the `n` floats at `xp`
def private dot_q8p(bp : uint8 const?; n : int64; xp : float const?) : float {
    let nb = n / Q8E
    var acc = 0.0
    unsafe {
        for (b in range64(nb)) {
            let bo = b * Q8B
            let sc = f16_to_f32(uint(bp[bo]) | (uint(bp[bo + 1l]) << 8u))
            let qp = reinterpret<int8 const?>(bp + bo + 2l)
            let xo = b * Q8E
            var sub = 0.0
            for (k in range64(Q8E)) {
                sub += float(qp[k]) * xp[xo + k]
            }
            acc += sub * sc
        }
    }
    return acc
}

//! y rows = W . x rows: W `d` rows of `n` at `boff`, `rows` input rows `n` apart at xoff, output rows `d` apart at yoff
def private mm_q8(var y : array<float>; yoff : int64; blob : array<uint8>; boff : int64; x : array<float>; xoff, n, d, rows : int64) {
    let jobs = int(clamp(d / 256l, 1l, 64l))
    let rowb = (n / Q8E) * Q8B
    unsafe {
        let bp = addr<uint8 const?>(blob[boff])
        let xp = addr<float const?>(x[xoff])
        var yp = addr(y[yoff])
        maybe_parallel_for(0, int(d), jobs) $(rb, re) {
            unsafe {
                for (r in range(rb, re)) {
                    for (t in range64(rows)) {
                        yp[t * d + int64(r)] = dot_q8p(bp + int64(r) * rowb, n, xp + t * n)
                    }
                }
            }
        }
    }
}

//! the K-quant rows of `mm_w`: each input row Q8_K-quantized, then the format's plane GEMV - the CPU decode's own pair
def private mm_kq(var y : array<float>; yoff : int64; p : KqPlanes; w : DflashW; x : array<float>; xoff, n, d, rows : int64) {
    var xq : array<int8>
    xq |> reserve_resize(n)
    var xs : array<float>
    xs |> reserve_resize(n / KQ_SB_ELEMS)
    var xbs : array<int>
    xbs |> reserve_resize(n / 16l)
    for (t in range64(rows)) {
        unsafe {
            quantize_q8_k_into_ptr(addr<float const?>(x[xoff + t * n]), n, addr(xq[0]), addr(xs[0]), addr(xbs[0]), 0l, 0l, 0l)
        }
        matmul_kq(kq_schema_id(w.fmt), y, p.q, p.s, w.off, xq, xs, xbs, n, d, yoff + t * d)
    }
    delete xq
    delete xs
    delete xbs
}

//! y rows = W . x rows over plane `w` in its own format: W `d` rows of `n`, `rows` input rows `n` apart at xoff, output rows `d`
//! apart at yoff
def private mm_w(var y : array<float>; yoff : int64; dr : DflashDrafter; w : DflashW; x : array<float>; xoff, n, d, rows : int64) {
    if (w.fmt == KqFmt.q8) {
        mm_q8(y, yoff, dr.blob, w.off, x, xoff, n, d, rows)
    } else {
        mm_kq(y, yoff, dr.kq[int(w.fmt)], w, x, xoff, n, d, rows)
    }
}

//! NEOX rope of a head's `hs` values at `off` for position `pos`: the pair (i, i + hs/2)
def private rope_head(var v : array<float>; off, hs, pos : int64; theta : float) {
    let half = hs / 2l
    for (i in range64(half)) {
        let inv = pow(theta, -2.0 * float(i) / float(hs))
        let ang = float(pos) * inv
        let c = cos(ang)
        let s = sin(ang)
        let a = v[off + i]
        let b = v[off + half + i]
        v[off + i] = a * c - b * s
        v[off + half + i] = a * s + b * c
    }
}

//! the two-tap grouped dynamic convolution over the block's rows in place: row t mixes itself (tap 0) and its predecessor
//! (tap 1; row 0 has none) under base[side][tap] plus the row's own dynamic coefficient a group of channels; `dyn` holds a
//! row's projection as [side][tap][group]
def private conv_taps(dr : DflashDrafter; var x : array<float>; rows : int64; dyn : array<float>; base_off : int64; side : int64) {
    let d = dr.dim
    let g = dr.conv_g
    let ng = d / g
    let projw = 2l * dr.conv_k * ng
    var prev : array<float>
    prev |> reserve_resize(d)
    var cur : array<float>
    cur |> reserve_resize(d)
    for (t in range64(rows)) {
        for (c in range64(d)) {
            cur[c] = x[t * d + c]
        }
        for (c in range64(d)) {
            let gi = c / g
            let w0 = dr.fblob[base_off + (side * dr.conv_k + 0l) * d + c] + dyn[t * projw + (side * dr.conv_k + 0l) * ng + gi]
            var o = w0 * cur[c]
            if (t > 0l) {
                let w1 = dr.fblob[base_off + (side * dr.conv_k + 1l) * d + c] + dyn[t * projw + (side * dr.conv_k + 1l) * ng + gi]
                o += w1 * prev[c]
            }
            x[t * d + c] = o
        }
        for (c in range64(d)) {
            prev[c] = cur[c]
        }
    }
    delete prev
    delete cur
}

//! the session's drafter state (made at first use); the pointer stands for one call, no other session inserted meanwhile
def private seq_of(s : Session) : DflashSeq? {
    if (!key_exists(g_seqs, s.uid)) {
        g_seqs[s.uid] <- DflashSeq()
    }
    unsafe {
        return addr(g_seqs[s.uid])
    }
}

//! row `r` of the wide residual `res` (rows of `nh` streams of `d`) as the streams' plain mean at `dst[doff ..]`
def private stream_mean_row(var dst : array<float>; doff : int64; res : array<float>; r, d, nh : int64) {
    let scale = 1.0 / float(nh)
    for (i in range64(d)) {
        var acc = 0.0
        for (c in range64(nh)) {
            acc += res[(r * nh + c) * d + i]
        }
        dst[doff + i] = acc * scale
    }
}

//! the trunk's layer `l` output rows of the forward in flight: a decode step's row in `s.x` (rows 1), a window's `rows` in
//! `s.x_b` from position `pos0` - under hyper-connections the streams' mean of the wide rows in `s.hc_res` / `s.hc_res_b`
//! (`ARCHITECTURE_GPU_MTP_ACCEPT.md#dflash-hc-capture`) - into the session's capture image at the layer's slot
[no_env, arch(at="../ARCHITECTURE_GPU_MTP_ACCEPT.md#dflash-hc-capture")]
def private dflash_capture(var s : Session; l, rows, pos0 : int64; window : bool) {
    let d = g_dflash.dim
    let ncap = long_length(g_dflash.target_layers)
    var slot = -1l
    for (i in range64(ncap)) {
        if (g_dflash.target_layers[i] == l) {
            slot = i
        }
    }
    return if (slot < 0l)
    var q = seq_of(s)
    if (q.caps_rows != rows || q.caps_pos0 != pos0 || slot == 0l) {
        q.caps |> reserve_resize(rows * ncap * d)
        q.caps_rows = rows
        q.caps_pos0 = pos0
    }
    for (r in range64(rows)) {
        if (g_dflash_hc > 0l) {
            stream_mean_row(q.caps, (r * ncap + slot) * d, window ? s.hc_res_b : s.hc_res, r, d, g_dflash_hc)
        } elif (window) {
            copy_floats(s.x_b, r * d, q.caps, (r * ncap + slot) * d, d)
        } else {
            copy_floats(s.x, 0l, q.caps, (r * ncap + slot) * d, d)
        }
    }
}

//! a device step's whole capture image ([rows][slot][dim]) as the session's
[no_env]
def private dflash_capture_rows(var s : Session; img : array<float>; rows, pos0 : int64) {
    var q = seq_of(s)
    q.caps := img
    q.caps_rows = rows
    q.caps_pos0 = pos0
}

//! the captured rows of positions pos0 .. pos0 + rows - 1 into the ring: c = norm(fc . [the layer outputs]), a layer's K
//! (normed, roped at the position) and V rows into the position's slot; a commit past the ring's end starts the context over
[no_env]
def private dflash_commit(var s : Session; pos0, rows : int64) {
    var q = seq_of(s)
    return if (rows <= 0l || q.caps_rows < rows || q.caps_pos0 != pos0)
    let dr & = g_dflash
    let d = dr.dim
    let ncap = long_length(dr.target_layers)
    let kvd = dr.n_kv * dr.hs
    let w = dr.window
    let lstride = w * kvd
    if (long_length(q.ring_k) < dr.n_layers * lstride) {
        q.ring_k |> reserve_resize(dr.n_layers * lstride)
        q.ring_v |> reserve_resize(dr.n_layers * lstride)
    }
    if (pos0 > q.ctx) {
        q.valid_lo = pos0
    }
    if (g_dflash_trace) {
        var line = "dflash commit {pos0}+{rows}: last row slot sums"
        for (sl in range64(ncap)) {
            var sum = 0.0
            for (i in range64(d)) {
                sum += abs(q.caps[((rows - 1l) * ncap + sl) * d + i])
            }
            line += " {sum}"   // nolint:PERF001 - a handful of slots, under the trace flag alone
        }
        print("{line}\n")
    }
    if (g_dflash_dev_on) {
        if (!invoke(dev_seams().commit, dr, s.uid, q.caps, rows, pos0)) {
            to_log(LOG_ERROR, "dasLLAMA: the block drafter's device commit of {rows} rows at {pos0} failed - the context starts over past them\n")
            q.valid_lo = pos0 + rows
        }
        q.ctx = pos0 + rows
        return
    }
    var cr : array<float>
    cr |> reserve_resize(rows * d)
    var kv : array<float>
    kv |> reserve_resize(rows * kvd)
    var fcv : array<float>
    fcv |> reserve_resize(rows * d)
    mm_w(fcv, 0l, dr, dr.fc, q.caps, 0l, ncap * d, d, rows)
    for (r in range64(rows)) {
        rms_w(cr, r * d, fcv, r * d, dr.fblob, dr.hnorm_off, d, dr.eps)
    }
    for (l in range64(dr.n_layers)) {
        mm_w(kv, 0l, dr, dr.wk[l], cr, 0l, d, kvd, rows)
        for (r in range64(rows)) {
            let pos = pos0 + r
            let slot = l * lstride + (pos % w) * kvd
            for (h in range64(dr.n_kv)) {
                rms_w(q.ring_k, slot + h * dr.hs, kv, r * kvd + h * dr.hs, dr.fblob, dr.kn_offs[l], dr.hs, dr.eps)
                rope_head(q.ring_k, slot + h * dr.hs, dr.hs, pos, dr.theta)
            }
        }
        mm_w(kv, 0l, dr, dr.wv[l], cr, 0l, d, kvd, rows)
        for (r in range64(rows)) {
            copy_floats(kv, r * kvd, q.ring_v, l * lstride + ((pos0 + r) % w) * kvd, kvd)
        }
    }
    q.ctx = pos0 + rows
    delete cr
    delete kv
    delete fcv
}

//! the ring positions a block of `rows` at `pos0` attends: from the oldest position its last row still sees (or the oldest the
//! ring holds valid) up to the context's end
def private ring_span(q : DflashSeq; pos0, rows : int64) : tuple<lo : int64; nctx : int64> {
    let lo = max(q.valid_lo, max(0l, pos0 + rows - 1l - (g_dflash.window - 1l)))
    return (lo = lo, nctx = max(0l, q.ctx - lo))
}

//! attention of the block's `rows` q rows (positions pos0 + t) over the ring's positions within the window and the block's own
//! rows (both ways), a head at a time; `k`, `v` the block rows' [rows][kvd]; out [rows][n_heads * hs]
def private block_attention(dr : DflashDrafter; q : DflashSeq; l : int64; qv : array<float>; k, v : array<float>; rows, pos0 : int64;
                            var out : array<float>) {
    let hs = dr.hs
    let kvd = dr.n_kv * dr.hs
    let qd = dr.n_heads * hs
    let w = dr.window
    let lstride = w * kvd
    let grp = dr.n_heads / dr.n_kv
    let scale = 1.0 / sqrt(float(hs))
    let span = ring_span(q, pos0, rows)
    let lo = span.lo
    let nctx = span.nctx
    let nkeys = nctx + rows
    var sc : array<float>
    sc |> reserve_resize(nkeys)
    for (t in range64(rows)) {
        let qpos = pos0 + t
        for (h in range64(dr.n_heads)) {
            let kh = h / grp
            var mx = -3.0e38
            for (j in range64(nkeys)) {
                let kpos = j < nctx ? lo + j : pos0 + (j - nctx)
                if (qpos - kpos >= w || kpos - qpos >= w) {
                    sc[j] = -3.0e38
                    continue
                }
                var dot = 0.0
                if (j < nctx) {
                    let kb = l * lstride + (kpos % w) * kvd + kh * hs
                    for (i in range64(hs)) {
                        dot += qv[t * qd + h * hs + i] * q.ring_k[kb + i]
                    }
                } else {
                    let kb = (j - nctx) * kvd + kh * hs
                    for (i in range64(hs)) {
                        dot += qv[t * qd + h * hs + i] * k[kb + i]
                    }
                }
                sc[j] = dot * scale
                mx = max(mx, sc[j])
            }
            var sum = 0.0
            for (j in range64(nkeys)) {
                sc[j] = sc[j] <= -1.0e38 ? 0.0 : exp(sc[j] - mx)
                sum += sc[j]
            }
            let inv = 1.0 / sum
            for (i in range64(hs)) {
                out[t * qd + h * hs + i] = 0.0
            }
            for (j in range64(nkeys)) {
                continue if (sc[j] == 0.0)
                let p = sc[j] * inv
                if (j < nctx) {
                    let vb = l * lstride + ((lo + j) % w) * kvd + kh * hs
                    for (i in range64(hs)) {
                        out[t * qd + h * hs + i] += p * q.ring_v[vb + i]
                    }
                } else {
                    let vb = (j - nctx) * kvd + kh * hs
                    for (i in range64(hs)) {
                        out[t * qd + h * hs + i] += p * v[vb + i]
                    }
                }
            }
        }
    }
    delete sc
}

//! one drafter layer over the block's rows in place
def private block_layer(dr : DflashDrafter; q : DflashSeq; l : int64; var x : array<float>; rows, pos0 : int64) {
    let d = dr.dim
    let qd = dr.n_heads * dr.hs
    let kvd = dr.n_kv * dr.hs
    let projw = 2l * dr.conv_k * (d / dr.conv_g)
    var hn : array<float>
    hn |> reserve_resize(rows * d)
    var dyn : array<float>
    dyn |> reserve_resize(rows * projw)
    for (t in range64(rows)) {
        rms_w(hn, t * d, x, t * d, dr.fblob, dr.attn_norm_offs[l], d, dr.eps)
    }
    mm_w(dyn, 0l, dr, dr.aconv_proj[l], hn, 0l, d, projw, rows)
    conv_taps(dr, hn, rows, dyn, dr.aconv_base_offs[l], 0l)
    var qv : array<float>
    qv |> reserve_resize(rows * qd)
    var kv : array<float>
    kv |> reserve_resize(rows * kvd)
    var vv : array<float>
    vv |> reserve_resize(rows * kvd)
    mm_w(qv, 0l, dr, dr.wq[l], hn, 0l, d, qd, rows)
    mm_w(kv, 0l, dr, dr.wk[l], hn, 0l, d, kvd, rows)
    mm_w(vv, 0l, dr, dr.wv[l], hn, 0l, d, kvd, rows)
    var tmp : array<float>
    tmp |> reserve_resize(dr.hs)
    for (t in range64(rows)) {
        for (h in range64(dr.n_heads)) {
            rms_w(tmp, 0l, qv, t * qd + h * dr.hs, dr.fblob, dr.qn_offs[l], dr.hs, dr.eps)
            copy_floats(tmp, 0l, qv, t * qd + h * dr.hs, dr.hs)
            rope_head(qv, t * qd + h * dr.hs, dr.hs, pos0 + t, dr.theta)
        }
        for (h in range64(dr.n_kv)) {
            rms_w(tmp, 0l, kv, t * kvd + h * dr.hs, dr.fblob, dr.kn_offs[l], dr.hs, dr.eps)
            copy_floats(tmp, 0l, kv, t * kvd + h * dr.hs, dr.hs)
            rope_head(kv, t * kvd + h * dr.hs, dr.hs, pos0 + t, dr.theta)
        }
    }
    var att : array<float>
    att |> reserve_resize(rows * qd)
    block_attention(dr, q, l, qv, kv, vv, rows, pos0, att)
    var o : array<float>
    o |> reserve_resize(rows * d)
    mm_w(o, 0l, dr, dr.wo[l], att, 0l, qd, d, rows)
    conv_taps(dr, o, rows, dyn, dr.aconv_base_offs[l], 1l)
    for (i in range64(rows * d)) {
        x[i] += o[i]
    }
    for (t in range64(rows)) {
        rms_w(hn, t * d, x, t * d, dr.fblob, dr.ffn_norm_offs[l], d, dr.eps)
    }
    mm_w(dyn, 0l, dr, dr.fconv_proj[l], hn, 0l, d, projw, rows)
    conv_taps(dr, hn, rows, dyn, dr.fconv_base_offs[l], 0l)
    var g : array<float>
    g |> reserve_resize(rows * dr.hidden)
    var u : array<float>
    u |> reserve_resize(rows * dr.hidden)
    mm_w(g, 0l, dr, dr.w1[l], hn, 0l, d, dr.hidden, rows)
    mm_w(u, 0l, dr, dr.w3[l], hn, 0l, d, dr.hidden, rows)
    for (i in range64(rows * dr.hidden)) {
        let a = g[i]
        g[i] = a / (1.0 + exp(-a)) * u[i]
    }
    mm_w(o, 0l, dr, dr.w2[l], g, 0l, dr.hidden, d, rows)
    conv_taps(dr, o, rows, dyn, dr.fconv_base_offs[l], 1l)
    for (i in range64(rows * d)) {
        x[i] += o[i]
    }
    delete hn
    delete dyn
    delete qv
    delete kv
    delete vv
    delete tmp
    delete att
    delete o
    delete g
    delete u
}

//! the `topk` best logits of a row at `off`, best first under (logit desc, id asc), NaN never taken: ids into ids[dst ..], values
//! into vals[dst ..]; a slot past the row's ids holds -1 under -3.4e38 - the device selector's `MetalDflashTopk` bit for bit
def dflash_top_k_row(logits : array<float>; off, vocab, topk : int64; var ids : array<int64>; var vals : array<float>; dst : int64) {
    for (j in range64(topk)) {
        ids[dst + j] = -1l
        vals[dst + j] = -3.4e38
    }
    let last = dst + topk - 1l
    for (i in range64(vocab)) {
        let v = logits[off + i]
        continue if (!(v > vals[last]))
        var p = last
        while (p > dst && v > vals[p - 1l]) {
            ids[p] = ids[p - 1l]
            vals[p] = vals[p - 1l]
            p--
        }
        ids[p] = i
        vals[p] = v
    }
}

//! a row of the selector's codebook `w` (rank values a token) dequantized into out[ooff..), a K-quant row through the engine's
//! plane dequant
def private codebook_row(dr : DflashDrafter; w : DflashW; tok : int64; var out : array<float>; ooff : int64) {
    if (w.fmt != KqFmt.q8) {
        let e = kq_elems(w.fmt)
        let sb0 = (w.off + tok * dr.rank) / e
        for (b in range64(dr.rank / e)) {
            dequant_kq_plane_sb(w.fmt, dr.kq[int(w.fmt)].q, dr.kq[int(w.fmt)].s, sb0 + b, false, out, ooff + b * e)
        }
        return
    }
    let nb = dr.rank / Q8E
    unsafe {
        let bp = addr<uint8 const?>(dr.blob[w.off + tok * nb * Q8B])
        for (b in range64(nb)) {
            let bo = b * Q8B
            let sc = f16_to_f32(uint(bp[bo]) | (uint(bp[bo + 1l]) << 8u))
            let qp = reinterpret<int8 const?>(bp + bo + 2l)
            for (k in range64(Q8E)) {
                out[ooff + b * Q8E + k] = float(qp[k]) * sc
            }
        }
    }
}

//! the block's input rows: the anchor's token row, then the mask token's - off the trunk's table, through the device where the
//! image carries no CPU planes
def private block_embed(t : Model; dr : DflashDrafter; tok, rows : int64; var x : array<float>) {
    let d = dr.dim
    if (t.metal_blob) {
        var toks : array<int64>
        toks |> reserve_resize(rows)
        toks[0] = tok
        for (r in range64(1l, rows)) {
            toks[r] = dr.mask_id
        }
        verify(mtp_block_device_embed(t, toks, x), "dasLLAMA: the block drafter's device home gathers no token rows on a blob-only image")
        delete toks
        return
    }
    unsafe {
        embed_row(t, tok, addr(x[0]))
        for (r in range64(1l, rows)) {
            embed_row(t, dr.mask_id, addr(x[r * d]))
        }
    }
}

let private DFLASH_NO_SCORE = -3.4e38   //! an empty candidate slot's score: under every live score and the walk's first best

//! position r's top-k scores into scores[0 .. topk): unary + <A[pred] * H(h_r), B[cand]> summed in ascending i, `arow` holding
//! A[pred]; an empty slot scores DFLASH_NO_SCORE
def private dflash_scores(dr : DflashDrafter; r : int64; arow : array<float>; cids : array<int64>; cvals, hsel : array<float>;
                          var brow : array<float>; var scores : array<float>) {
    let topk = dr.topk
    for (j in range64(topk)) {
        let cand = cids[r * topk + j]
        if (cand < 0l) {
            scores[j] = DFLASH_NO_SCORE
            continue
        }
        codebook_row(dr, dr.sel_next, cand, brow, 0l)
        var sc = cvals[r * topk + j]
        for (i in range64(dr.rank)) {
            sc += arow[i] * hsel[r * dr.rank + i] * brow[i]
        }
        scores[j] = sc
    }
}

//! the selector's walk over `nd` positions from the anchor `tok`: each position's top-k scored, `pick(r, scores)` its slot - the
//! pick and the next position's predecessor, into picks[r + 1]
def private dflash_walk_with(dr : DflashDrafter; tok, nd : int64; cids : array<int64>; cvals, hsel : array<float>; var picks : array<int64>;
                             pick : block<(r : int64; scores : array<float>) : int64>) {
    let topk = dr.topk
    var arow, brow, scores : array<float>
    arow |> reserve_resize(dr.rank)
    brow |> reserve_resize(dr.rank)
    scores |> reserve_resize(topk)
    var pred = tok
    for (r in range64(nd)) {
        codebook_row(dr, dr.sel_prev, pred, arow, 0l)
        dflash_scores(dr, r, arow, cids, cvals, hsel, brow, scores)
        pred = cids[r * topk + invoke(pick, r, scores)]
        picks[r + 1l] = pred
    }
    delete arow
    delete brow
    delete scores
}

//! the first slot of `scores[0 .. n)` at its maximum (slot 0 where every slot is empty)
def dflash_first_best(scores : array<float>; n : int64) : int64 {
    var best = 0l
    var bests = -3.0e38
    for (j in range64(n)) {
        if (scores[j] > bests) {
            bests = scores[j]
            best = j
        }
    }
    return best
}

//! the selector's greedy walk: position r's pick the first best of its top-k scores (`dflash_scores`), the next position's
//! predecessor; the picks into picks[1 .. nd]
def dflash_walk(dr : DflashDrafter; tok, nd : int64; cids : array<int64>; cvals, hsel : array<float>; var picks : array<int64>) {
    dflash_walk_with(dr, tok, nd, cids, cvals, hsel, picks) $(_r : int64; scores : array<float>) : int64 {
        return dflash_first_best(scores, dr.topk)
    }
}

//! A sampled draft's pick off one position's `n` candidate scores: their softmax at `temp` into q[qoff .. qoff + n) (an empty
//! slot at 0) and one draw of `rng` over it, the slot back; at `temp` <= 0 a point mass on the first best and no draw
def dflash_soft_pick(scores : array<float>; n : int64; temp : float; var rng : int4&; var q : array<float>; qoff : int64) : int64 {
    for (j in range64(n)) {
        q[qoff + j] = 0.0
    }
    let best = dflash_first_best(scores, n)
    if (temp <= 0.0 || !(scores[best] > DFLASH_NO_SCORE)) {
        q[qoff + best] = 1.0
        return best
    }
    let inv = 1.0 / temp
    var sum = 0.0
    for (j in range64(n)) {
        if (scores[j] > DFLASH_NO_SCORE) {
            q[qoff + j] = exp(max((scores[j] - scores[best]) * inv, -80.0))
            sum += q[qoff + j]
        }
    }
    let inv_sum = 1.0 / sum
    for (j in range64(n)) {
        q[qoff + j] *= inv_sum
    }
    let u = random_float(rng)
    var cdf = 0.0
    var last = best
    for (j in range64(n)) {
        continue if (q[qoff + j] <= 0.0)
        last = j
        cdf += q[qoff + j]
        return j if (u < cdf)
    }
    return last
}

//! the selector's sampled walk: position r's pick drawn from the softmax of its top-k scores at `temp` (`dflash_soft_pick`, `rng`),
//! the next position's predecessor as the greedy walk's; the picks into picks[1 .. nd], each position's q into q[r * topk ..]
[arch(at="../ARCHITECTURE_GPU_MTP_ACCEPT.md#dflash-sampled-drafts")]
def dflash_walk_sampled(dr : DflashDrafter; tok, nd : int64; cids : array<int64>; cvals, hsel : array<float>; temp : float; var rng : int4&;
                        var picks : array<int64>; var q : array<float>) {
    q |> reserve_resize(nd * dr.topk)
    dflash_walk_with(dr, tok, nd, cids, cvals, hsel, picks) $(r : int64; scores : array<float>) : int64 {
        return dflash_soft_pick(scores, dr.topk, temp, rng, q, r * dr.topk)
    }
}

//! draft row `qi`'s q recorded off `n` candidates (`ids[ioff ..]` with mass `q[qoff ..]`, the positive ones) as the rejection
//! walk reads it: `s.mtp_q[qi * MTP_Q_CAP ..]`, id ascending, `s.mtp_qn[qi]` entries
def dflash_record_q(var s : Session; qi : int64; ids : array<int64>; ioff : int64; q : array<float>; qoff, n : int64) {
    let base = qi * MTP_Q_CAP
    s.mtp_q |> grow_resize(base + MTP_Q_CAP)
    s.mtp_qn |> grow_resize(qi + 1l)
    var m = 0l
    for (j in range64(min(n, MTP_Q_CAP))) {
        let id = ids[ioff + j]
        continue if (id < 0l || !(q[qoff + j] > 0.0))
        var p = base + m
        while (p > base && s.mtp_q[p - 1l].id > id) {
            s.mtp_q[p] = s.mtp_q[p - 1l]
            p--
        }
        s.mtp_q[p] = SampleCand(id = id, v = q[qoff + j])
        m++
    }
    s.mtp_qn[qi] = m
}

//! the CPU arm of the pass: the block's rows through the layers, the first `nd` draft rows normed, the trunk's classifier over
//! them (the device's on a blob-only image) into `logits` ([nd][vocab]) and the selector's projection into `hsel` ([nd][rank])
def private cpu_block_pass(t : Model; var s : Session; dr : DflashDrafter; q : DflashSeq; tok, pos, rows, nd : int64; var logits : array<float>; var hsel : array<float>) {
    let d = dr.dim
    var x : array<float>
    x |> reserve_resize(rows * d)
    block_embed(t, dr, tok, rows, x)
    for (l in range64(dr.n_layers)) {
        block_layer(dr, q, l, x, rows, pos)
    }
    var hn : array<float>
    hn |> reserve_resize(nd * d)
    for (r in range64(nd)) {
        rms_w(hn, r * d, x, (r + 1l) * d, dr.fblob, dr.norm_off, d, dr.eps)
    }
    let vocab = t.config.vocab_size
    logits |> reserve_resize(nd * vocab)
    if (t.metal_blob) {   //! no CPU planes: the device's classifier over the drafter's normed rows
        verify(mtp_block_device_cls(t, hn, nd, logits), "dasLLAMA: the block drafter's device home runs no classifier rows on a blob-only image")
    } else {
        mm_b_f(s, logits, t, cls_fmt_of(t), t.wcls_off, hn, d, vocab, nd)
    }
    hsel |> reserve_resize(nd * dr.rank)
    mm_w(hsel, 0l, dr, dr.sel_h, hn, 0l, d, dr.rank, nd)
    delete x
    delete hn
}

//! The block drafted from the anchor `tok` at `pos`: the whole block's pass (its attention is bidirectional, so a shorter
//! block would draft other ids), its first `k` drafts into `s.mtp_vbatch[1 ..]` (the selector's path through each position's
//! top-k under the pairwise scores: greedy, or drawn at the drafter's temperature in a rejection-sampling round with its q
//! recorded), their count back; 0 = the context ring does not reach `pos` (a plain step), or the device pass failed
[no_env]
def private dflash_block_draft(t : Model; var s : Session; tok, pos, k : int64) : int64 {
    let dr & = g_dflash
    let q = seq_of(s)
    g_dflash_land_nd = 0l
    g_dflash_held = false
    g_dflash_held_soft = false
    g_dflash_point_q = false
    return 0l if (q.ctx != pos || k <= 0l)
    let rows = dr.nblock
    let nd = min(k, rows - 1l)
    let vocab = t.config.vocab_size
    let t0 = ref_time_ticks()
    let records_q = mtp_drafts_sampled(s)
    let draft_temp = !records_q ? 0.0 : g_dflash_draft_temp >= 0.0 ? g_dflash_draft_temp : s.spec_params != null ? deref(s.spec_params).temp : 0.0
    g_dflash_point_q = records_q && draft_temp <= 0.0
    let soft = draft_temp > 0.0
    if (g_dflash_dev_on && invoke(dev_seams().selects, soft)) {
        return dflash_device_select(t, s, dr, *q, tok, pos, rows, nd, soft, draft_temp, t0)
    }
    var logits, hsel : array<float>
    if (g_dflash_dev_on) {
        let span = ring_span(*q, pos, rows)
        if (!invoke(dev_seams().draft_pass, t, dr, s.uid, tok, pos, rows, nd, span.lo, span.nctx, logits, hsel)) {
            to_log(LOG_ERROR, "dasLLAMA: the block drafter's device pass at {pos} failed - the round steps plain\n")
            return 0l
        }
    } else {
        cpu_block_pass(t, s, dr, *q, tok, pos, rows, nd, logits, hsel)
    }
    let t_pass = get_time_usec(t0)
    let topk = dr.topk
    var cids : array<int64>
    cids |> reserve_resize(nd * topk)
    var cvals : array<float>
    cvals |> reserve_resize(nd * topk)
    for (r in range64(nd)) {
        dflash_top_k_row(logits, r * vocab, vocab, topk, cids, cvals, r * topk)
    }
    if (draft_temp > 0.0) {
        var qs : array<float>
        dflash_walk_sampled(dr, tok, nd, cids, cvals, hsel, draft_temp, s.rng, s.mtp_vbatch, qs)
        for (r in range64(nd)) {
            dflash_record_q(s, r, cids, r * topk, qs, r * topk, topk)
        }
        delete qs
    } else {
        dflash_walk(dr, tok, nd, cids, cvals, hsel, s.mtp_vbatch)
    }
    if (g_dflash_trace) {
        print("  draft pass {rows} rows, {nd} drafts: pass {t_pass / 1000} ms, select {(get_time_usec(t0) - t_pass) / 1000} ms\n")
    }
    delete logits
    delete cids
    delete cvals
    delete hsel
    g_dflash_land_nd = nd
    g_dflash_land_rows = rows
    return nd
}

//! the device's own select - the pass and the pick one submission - its `nd` drafts (a held pass's land later) and their
//! count; 0 = the pass failed (the round steps plain)
def private dflash_device_select(t : Model; var s : Session; dr : DflashDrafter; q : DflashSeq; tok, pos, rows, nd : int64; soft : bool; draft_temp : float; t0 : int64) : int64 {
    let span = ring_span(q, pos, rows)
    var cids : array<int64>
    var qs : array<float>
    let got = invoke(dev_seams().select_pass, t, dr, s.uid, tok, pos, rows, nd, span.lo, span.nctx, !sel_wanted(s), !sel_ask_reads_drafts(s), draft_temp,
        s.rng, s.mtp_vbatch, cids, qs)
    if (got == DflashSelect.failed) {
        to_log(LOG_ERROR, "dasLLAMA: the block drafter's device pass at {pos} failed - the round steps plain\n")
        return 0l
    }
    g_dflash_held = got == DflashSelect.held
    g_dflash_held_soft = g_dflash_held && soft
    if (soft && !g_dflash_held) {
        for (r in range64(nd)) {
            dflash_record_q(s, r, cids, r * dr.topk, qs, r * dr.topk, dr.topk)
        }
    }
    delete cids
    delete qs
    if (g_dflash_trace && !g_dflash_held) {
        print("  draft pass {rows} rows, {nd} drafts: pass and {soft ? "drawn" : "greedy"} select {get_time_usec(t0) / 1000} ms on the device\n")
    }
    g_dflash_land_nd = nd
    g_dflash_land_rows = rows
    return nd
}

//! the round's land, after its verify: a held pass's picks into `s.mtp_vbatch[1 ..]` (a failed one's as -1, which the walk
//! accepts none of), then the round's drafts onto the record; once a draft, a second call lands nothing
[no_env]
def private dflash_land(var s : Session) {
    let nd = g_dflash_land_nd
    return if (nd <= 0l)
    g_dflash_land_nd = 0l
    if (g_dflash_held) {
        g_dflash_held = false
        var cids : array<int64>
        var qs : array<float>
        let ms = invoke(dev_seams().land, s.mtp_vbatch, nd, s.rng, cids, qs)
        if (ms < 0.0lf) {
            to_log(LOG_ERROR, "dasLLAMA: the block drafter's held device pass failed - its {nd} drafts stand rejected\n")
            for (r in range64(nd)) {
                s.mtp_vbatch[r + 1l] = -1l
            }
        } else {
            if (g_dflash_held_soft) {
                let topk = g_dflash.topk
                for (r in range64(nd)) {
                    dflash_record_q(s, r, cids, r * topk, qs, r * topk, topk)
                }
            }
            if (g_dflash_trace) {
                print("  draft pass {g_dflash_land_rows} rows, {nd} drafts: pass and select {ms} ms on the GPU, held for the verify\n")
            }
        }
        g_dflash_held_soft = false
        delete cids
        delete qs
    }
    if (g_dflash_point_q) {
        g_dflash_point_q = false
        let one <- [1.0]
        for (r in range64(nd)) {
            dflash_record_q(s, r, s.mtp_vbatch, r + 1l, one, 0l, 1l)
        }
    }
    if (g_dflash_record) {
        g_dflash_recorded |> reserve(long_length(g_dflash_recorded) + nd)
        for (r in range64(nd)) {
            g_dflash_recorded |> push(s.mtp_vbatch[r + 1l])
        }
    }
}
