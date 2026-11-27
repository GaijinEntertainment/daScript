options gen2
options indenting = 4
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_tower_tts shared public

require dasllama/dasllama_lint public
require math
require daslib/enum_trait   // enum_length: a home's slot by its `TowerDev`
require dasllama/dasllama_math
require dasllama/dasllama_common
require dasllama/dasllama_tts_blocks
require dasllama/dasllama_tts_types
require dasllama/dasllama_styletts2
require dasllama/dasllama_pocket
require dasllama/dasllama_tts_slab public   // a seat's instance builds the slab records and calls the shape walk in the home's module
require dasllama/dasllama_convert
require dasllama/dasllama_kqformat   // KqFmt, the superblock strides
require dasllama/dasllama_gemm_schema   // the device layout's block strides
require dasllama/dasllama_layout   // the device-layout gathers
require dasllama/dasllama_tower   // TowerDev, the device block
require dasllama/dasllama_plane   // plane_at: a block piece's bytes
require dasllama/dasllama_image   // layout_fingerprint: the block's record shapes
require daslib/archive   // the slot records as a block piece
require daslib/fio   // release_map: a mapped piece's host pages after its upload
require dasllama/dasllama_tower_rail public
require dasllama/dasllama_gpu_math   // the row stamps' activation codes
require dasllama/dasllama_gpu_kernels_common   // the Pocket rows' stage widths
require dasllama/dasllama_gpu_kernels_discrete   // rq_wgs: a row kernel's workgroups
require ?das_metal dasllama/dasllama_metal_classes_tts public   // the TTS kernels' Metal stamps and the Metal rail
require ?das_metal dasllama/dasllama_metal_sites public   // the Metal home's own tiles at the TTS sites

//! The TTS driver, generic over the rail (`VtRail`, `CutRail`, `MtRail`): the kitten and kokoro families' synthesis
//! seats and the Pocket family's encode, voice, codec, frames and prompt seats (`ARCHITECTURE_GPU_TOWER_TTS.md`), each a
//! body a home's seat function calls with its rail. A device buffer's allocation, upload and readback and the chain's
//! one-shot are the home's rail primitives (`dasllama_vulkan_rail`, `dasllama_cuda_rail`, `dasllama_metal_rail`); the
//! gate, the arm and the counters are the home's seat file's, registered as its `TtsHome`.

let private TS_ATTN_MAX_T = int64(TTS_ATTN_TCAP)   //! the attention row stamp's key stage
let private TS_LSTM_MAX_HIDDEN = int64(TTS_LSTM_LANES)   //! the recurrence stamp's lanes

//! what a home's seat file keeps for the driver: its name in the log, its knob, its arm, a decline's words and its forget
struct TtsHome {
    name : string                   //! the home a decline's log line names; empty reads as the Vulkan home
    on : function<() : bool>        //! the home's tower knob
    arms : function<() : bool>      //! the tier's want and the device, brought up on the first ask
    words : function<(why : TowerDecline) : string>   //! what a decline of that reason tells the reader on this home
    forget : function<() : void>    //! `tts_forget` over the home's rail
    release : function<() : void>   //! `tts_release_activations` over the home's rail
    held : function<() : tuple<weights : int64; activations : int64>>   //! `tts_held` over its rail
    cm2 : function<(fmt : KqFmt; d, rows : int64) : bool>   //! the conv site's cm2 tile picked for a [rows x d] output over `fmt` weights on the home's rail, its classes built; false where the device has none
}

let private TS_HOMES = typeinfo enum_length(type<TowerDev>)

var private g_ts_homes : TtsHome[TS_HOMES]
var private g_ts_owner = TowerDev.none   //! the home whose device buffers the slabs, the scratch and the voice slots hold

//! a home's hooks, set once at its seat file's `[init]`
def tts_home_register(dev : TowerDev; home : TtsHome) {
    g_ts_homes[int(dev)] = home
    g_ts_declines[int(dev)].home = home.name
    tts_dev_release_register(@@ts_owner_release)
}

[arch(at = "../ARCHITECTURE_TTS_MEMORY.md#tts-device-release")]
def private ts_owner_release(weights : bool) {
    return if (g_ts_owner == TowerDev.none)
    let home & = unsafe(g_ts_homes[int(g_ts_owner)])
    invoke(weights ? home.forget : home.release)
}

//! the device bytes home `dev` holds for the driver: its slabs and voice slots, and its activation
//! rows - zero on a home that does not own the driver's state
[cold_path]
def tts_home_held(dev : TowerDev) : tuple<weights : int64; activations : int64> {
    return (weights = 0l, activations = 0l) if (g_ts_owner == TowerDev.none || g_ts_owner != dev)
    return invoke(g_ts_homes[int(dev)].held)
}

var private g_ts_declines : GpuDeclineCounter[TS_HOMES]   //! a home's declines, by reason
var private g_ts_encodes : int64[TS_HOMES]                //! a home's seats served, one a call
var private g_ts_widened : int64[TS_HOMES]   //! a home's dense dispatches over widened rows

//! home `dev`'s seats served, declines and dense dispatches over widened rows, process-wide
[cold_path]
def tts_home_stats(dev : TowerDev) : tuple<encodes : int64; declines : int64; widened : int64> {
    let d = int(dev)
    return (encodes = g_ts_encodes[d], declines = g_ts_declines[d].total, widened = g_ts_widened[d])
}

//! home `dev`'s declines by reason name
[cold_path]
def tts_home_declines(dev : TowerDev) : table<string; int64> => clone_to_move(g_ts_declines[int(dev)].counts)

//! the decline reasons home `dev` has said since its last forget - a cell's witness that a model swap says them anew
[cold_path]
def tts_home_said_(dev : TowerDev) : int => length(g_ts_declines[int(dev)].said)

def private ts_on(rl : auto(R)) : bool => invoke(g_ts_homes[int(_::rail_tts_dev(rl))].on)

def private ts_arms(rl : auto(R)) : bool => invoke(g_ts_homes[int(_::rail_tts_dev(rl))].arms)

//! the decline counted under the reason's name for the rail's home and said once a seat and reason; false
[cold_path]
def private ts_decline(rl : auto(R); seat : string; why : TowerDecline) : bool {
    let d = int(_::rail_tts_dev(rl))
    return g_ts_declines[d] |> note("the {seat} seat", "{why}", "the seat stays on the CPU chain: {invoke(g_ts_homes[d].words, why)}")
}

def private ts_served(rl : auto(R)) {
    g_ts_encodes[int(_::rail_tts_dev(rl))]++
}

//! a dense dispatch counted for the rail's home where it reads rows a stored leaf was widened into
[arch(at="../ARCHITECTURE_GPU_TOWER_TTS.md#tts-st2-seats")]
def private ts_count_widened(rl : auto(R); widened : bool) {
    if (widened) {
        g_ts_widened[int(_::rail_tts_dev(rl))]++
    }
}

def private ts_cm2_pick(rl : auto(R); fmt : KqFmt; d, rows : int64) : bool => invoke(g_ts_homes[int(_::rail_tts_dev(rl))].cm2, fmt, d, rows)

[arch(at="../ARCHITECTURE_GPU_TOWER_TTS.md#tts-st2-seats")]
def private ts_claim(rl : auto(R)) {
    let me = _::rail_tts_dev(rl)
    return if (g_ts_owner == me)
    if (g_ts_owner != TowerDev.none) {
        invoke(g_ts_homes[int(g_ts_owner)].forget)
    }
    g_ts_owner = me
}

let private TS_ST_Q8 = 0    //! the stored members a slab's leaves are staged in: Q8_0 blocks ...
let private TS_ST_K4 = 1    //! ... Q4_K, Q5_K and Q6_K superblocks ...
let private TS_ST_K5 = 2
let private TS_ST_K6 = 3
let private TS_ST_F16 = 4   //! ... and half floats
let private TS_ST_OWN = 5   //! the quant members again in a home's own layout, in the same order: a leaf only that home's own tile reads
let private TS_ST_MEMBERS = 9

//! the sites (a bit a `TtsStoredSite`) whose quant leaves `dev` stages in its layout: Metal's convs, under `dense` its dense and LSTM sites
def private ts_own_sites(dev : TowerDev; dense : bool) : uint
    => dev != TowerDev.metal ? 0u : (1u << uint(TtsStoredSite.conv)) | (dense ? (1u << uint(TtsStoredSite.lin)) | (1u << uint(TtsStoredSite.lstm)) : 0u)

//! the writer's mask for a slab whose leaves the exact tile reads as stored: every member above
let private TS_STORED_MASK = (1u << uint(KqFmt.q8)) | (1u << uint(KqFmt.k4)) | (1u << uint(KqFmt.k5)) | (1u << uint(KqFmt.k6)) | TTS_STORED_HALVES

//! a stored leaf as a tile reads it: its member, its first element in the member's plane pair, its rows' stride
struct private TsLeaf {
    member : int
    fmt : int       //! its format: a `KqFmt` slot, or `TTS_PK_ST_HALVES` for half floats (the record is a device-block piece, stored as its bytes: the pair leaves no padding unwritten)
    woff : int64    //! its first element in the member's planes; on the Metal layout the byte its first block starts at in the quant plane
    wstride : int64
    soff : int64    //! the Metal layout's K-quant leaf: its scale rows' byte in the scale plane ...
    doff : int64    //! ... and a split format's d tail's
}

//! a quant leaf's format (a half leaf reads Q8_0: its sites branch on the halves first)
def private ts_leaf_kq(leaf : TsLeaf) : KqFmt => leaf.fmt < 0 ? KqFmt.q8 : kq_fmt_of_slot(leaf.fmt)

//! a part's slab on the device: its f32 rows (read-only, bit 0), and the leaves the slab left out, staged as stored
struct private TsSlab {
    key : uint64
    rows : RailBuf
    wq : RailBuf[TS_ST_MEMBERS]     //! a stored member's quant plane (the halves' own plane) in the shared device layout ...
    ws : RailBuf[TS_ST_MEMBERS]     //! ... and its scale plane
    leaves : array<TsLeaf>          //! the writer's stored planes in its order: a slot's `stored - 1`
}

var private g_ts_albert : TsSlab
var private g_ts_albert_slots : TtsAlbertSlab
var private g_ts_text : TsSlab
var private g_ts_text_slots : TtsTextSlab
var private g_ts_pred : TsSlab
var private g_ts_pred_slots : TtsPredSlab

def private ts_slab_release(rl : auto(R); var sl : TsSlab) {
    ts_rows_release(rl, sl.rows)
    for (m in range(TS_ST_MEMBERS)) {
        ts_rows_release(rl, sl.wq[m])
        ts_rows_release(rl, sl.ws[m])
    }
    sl.leaves |> clear()
    sl.key = 0ul
}

//! `rail_tts_bind_ok`, the decline said under the info log naming `what`
[cold_path]
def private ts_range_ok(rl : auto(R); what : string; bytes : int64) : bool {
    return true if (_::rail_tts_bind_ok(rl, bytes))
    to_log(LOG_INFO, "{_::rail_tts_tag(rl)} tts: the {what} of {bytes / 1048576l} MB binds past the device's storage range of {_::rail_tts_bind_cap(rl) / 1048576l} MB - the seat declines\n")
    return false
}

//! the first `n` elements of `src` onto the rows at element 0
def private ts_upload(rl : auto(R); r : RailBuf; src : array<auto(TT)>; n : int64) {
    _::rail_tts_put(rl, r.dev, 0l, unsafe(addr(src[0])), n * int64(typeinfo sizeof(type<TT>)))
}

//! the rows' first `n` floats back into `out`, sized to them, through the home's readback
def private ts_readback(rl : auto(R); r : RailBuf; @scratch @exact_size var out : array<float>; n : int64) {
    unsafe(scratch_resize(out, n))
    return if (n <= 0l)
    var dst = unsafe(addr(out[0]))
    _::rail_tts_take(rl, r, 0l, dst, n * 4l)
}

def private ts_slab_current(sl : TsSlab; key : uint64) : bool => sl.rows.dev != 0ul && sl.key == key

//! a quant plane staged in its home's layout: site among `own`, shape the tile's - a conv on the gather's lattice and 64 outputs, any dense
def private ts_leaf_own(p : TtsStoredPlane; own, own_fmts : uint) : bool {
    return false if ((own & (1u << uint(p.site))) == 0u)
    return (own_fmts & (1u << uint(p.fmt))) != 0u if (p.site != TtsStoredSite.conv)
    return p.cin % (p.transposed ? 32l : 8l) == 0l && p.cout % 64l == 0l
}

//! a quant format's stored member in the shared layout: an overload a format the driver stages, no member for any other
[unused_argument(t)]
def private ts_st_of(t : KqTag_q8) : int => TS_ST_Q8

[unused_argument(t)]
def private ts_st_of(t : KqTag_k4) : int => TS_ST_K4

[unused_argument(t)]
def private ts_st_of(t : KqTag_k5) : int => TS_ST_K5

[unused_argument(t)]
def private ts_st_of(t : KqTag_k6) : int => TS_ST_K6

[unused_argument(t)]
def private ts_st_of(t) : int => -1

//! format `f`'s stored member in the shared layout, -1 where the driver stages no leaf of it
def private ts_fmt_member(f : KqFmt) : int {   // nolint:STYLE037 - the stamp's ladder, one arm per KqFmt member
    var m = -1
    kq_fmt_stamp(f) $(F) {
        m = ts_st_of(F)
    }
    return m
}

//! a stored plane's member where the home stages the sites of `own` in its own layout, a dense layer's in the formats of `own_fmts`
def private ts_leaf_member(p : TtsStoredPlane; own, own_fmts : uint) : int {
    return TS_ST_F16 if (p.kind != TtsStoredKind.quant)
    let base = ts_fmt_member(p.fmt)
    if (base < 0) {
        panic("dasLLAMA TTS: no stored member holds {p.fmt} - the slab writer's mask admits the staged members alone")
    }
    return ts_leaf_own(p, own, own_fmts) ? TS_ST_OWN + base : base
}

let TS_OWN_ANY = 0xFFFFFFFFu   //! every format a dense tile reads
let private TS_OWN_GEMV = (1u << uint(KqFmt.q8)) | (1u << uint(KqFmt.k4))   //! the formats the Metal row GEMV decodes in its own layout

def private ts_shared_member(m : int) : int => m >= TS_ST_OWN ? m - TS_ST_OWN : m

//! quant member `m`'s format, whichever layout it sits in: the format whose member it is
[cold_path]
def private ts_member_fmt(m : int) : KqFmt {
    let b = ts_shared_member(m)
    for (f in type<KqFmt>) {
        return f if (ts_fmt_member(f) == b)
    }
    panic("dasLLAMA TTS: stored member {m} is no quant format's")
}

//! member `m`'s format as a leaf carries it: its `KqFmt` slot, or the halves' code
[cold_path]
def private ts_member_slot(m : int) : int => m == TS_ST_F16 ? TTS_PK_ST_HALVES : int(ts_member_fmt(m))

//! own member `m`'s leaves in the Metal layout, each plane 16-byte placed (a K-quant one at an even superblock), pad rows zero; device-free
[cold_path]
def private ts_gather_member_metal(stored : array<TtsStoredPlane>; members : array<int>; var leaves : array<TsLeaf>; m : int; @exact_size var wq, ws : array<uint8>) {
    if (ts_shared_member(m) == TS_ST_Q8) {
        var bytes = 0l
        for (p, leaf, pm in stored, leaves, members) {
            continue if (pm != m)
            leaf = TsLeaf(member = m, fmt = int(KqFmt.q8), woff = round_up(bytes, 16l), wstride = p.k)
            bytes = leaf.woff + (p.rows + p.pad_rows) * (p.k / Q8_BLOCK_ELEMS) * (Q8_QPB + Q8_SPB)
        }
        return if (bytes == 0l)
        wq |> reserve(round_up(bytes, 16l))
        wq |> resize(round_up(bytes, 16l))
        let lay = active_q8_repack_layout(g_active_needs_repack)
        for (p, leaf, pm in stored, leaves, members) {
            continue if (pm != m)
            unsafe {
                q8_region_to_metal_blob(reinterpret<int8 const?>(p.q), reinterpret<float const?>(p.s), p.k, p.rows, g_active_needs_repack, lay.mr, lay.kgroup, lay.wbias,
                    addr(wq[leaf.woff]))
            }
        }
        return
    }
    let f = ts_member_fmt(m)
    let qsb = kq_qsb(f)
    let ssb = kq_ssb(f)
    var nsb = 0l
    for (p, pm in stored, members) {
        continue if (pm != m)
        nsb = round_up(nsb, 2l) + (p.rows + p.pad_rows) * (p.k / KQ_SB_ELEMS)
    }
    return if (nsb == 0l)
    wq |> reserve(round_up(nsb * qsb, 16l))
    wq |> resize(round_up(nsb * qsb, 16l))
    ws |> reserve(nsb * ssb)
    ws |> resize(nsb * ssb)
    var sb = 0l
    for (p, leaf, pm in stored, leaves, members) {
        continue if (pm != m)
        sb = round_up(sb, 2l)
        kq_region_to_metal_planes(f, p.q, p.s, p.rows * (p.k / KQ_SB_ELEMS), sb, wq, ws)
        let at = kq_bind_offs(f, nsb, sb * KQ_SB_ELEMS)
        leaf = TsLeaf(member = m, fmt = int(f), woff = int64(at.qoff), wstride = p.k, soff = int64(at.soff), doff = int64(at.doff))
        sb += (p.rows + p.pad_rows) * (p.k / KQ_SB_ELEMS)
    }
    metal_kq_scale_form(f, ws)
    ws |> reserve(round_up(long_length(ws), 16l))
    ws |> resize(round_up(long_length(ws), 16l))
}

//! member `m`'s leaves into its plane pair in its layout, each at its `woff` (into `leaves`), pad rows zero, planes to 16 bytes; no device
[cold_path]
def private ts_gather_member(stored : array<TtsStoredPlane>; members : array<int>; var leaves : array<TsLeaf>; m : int; @exact_size var wq, ws : array<uint8>;
                             dev : TowerDev) {
    if (m >= TS_ST_OWN) {
        if (dev == TowerDev.metal) {
            ts_gather_member_metal(stored, members, leaves, m, wq, ws)
        }
        return
    }
    let slot = ts_member_slot(m)
    var elems = 0l
    for (p, leaf, pm in stored, leaves, members) {
        continue if (pm != m)
        leaf = TsLeaf(member = m, fmt = slot, woff = elems, wstride = p.k)
        elems += (p.rows + p.pad_rows) * p.k
    }
    return if (elems == 0l)
    let kq = m != TS_ST_Q8 && m != TS_ST_F16
    let nsb = elems / KQ_SB_ELEMS
    let qsb = kq ? kq_dev_qsb(ts_member_fmt(m)) : 0l
    let ssb = kq ? kq_dev_ssb(ts_member_fmt(m)) : 0l
    let qbytes = round_up(kq ? nsb * qsb : m == TS_ST_F16 ? elems * 2l : elems, 16l)
    let sbytes = round_up(kq ? nsb * ssb : m == TS_ST_Q8 ? (elems / Q8_BLOCK_ELEMS) * Q8_SPB : 0l, 16l)
    wq |> reserve(qbytes)
    wq |> resize(qbytes)
    ws |> reserve(sbytes)
    ws |> resize(sbytes)
    let lay = active_q8_repack_layout(g_active_needs_repack)
    var inscope no_site_words : array<uint>
    for (p, leaf, pm in stored, leaves, members) {
        continue if (pm != m)
        unsafe {
            if (m == TS_ST_Q8) {
                q8_gather_rows(reinterpret<int8 const?>(p.q), null, reinterpret<float const?>(p.s), p.k, p.rows, p.rows, lay.mr, lay.kgroup, lay.wbias, wq, ws,
                    leaf.woff, leaf.woff / Q8_BLOCK_ELEMS)
            } elif (m == TS_ST_F16) {
                tts_stored_half_rows(p, addr<uint16?>(wq[leaf.woff * 2l]))
            } else {
                kq_gather_rows_dev(p.fmt, p.q, p.s, p.k, p.rows, p.rows, false, 1l, wq, ws, (leaf.woff / KQ_SB_ELEMS) * qsb, (leaf.woff / KQ_SB_ELEMS) * ssb, no_site_words)
            }
        }
    }
}

//! a part's pieces under `name` into `b` (`.rows`, `.wq<m>`/`.ws<m>` in `dev`'s layout, `.leaves`, `.slots`), read arrays into `b.spent`
[cold_path, arch(at = "../ARCHITECTURE_IMAGE_GPU_PLANES.md#tower-dev-block")]
def tts_part_bake(var b : TowerDevBuild; dev : TowerDev; name : string; var slots : auto(SS); write : block<(var w : TtsSlabWriter; var s : SS) : void>;
                  spend : bool = true; dense : bool = false; dense_fmts : uint = TS_OWN_ANY) {
    var built <- tts_slab_build_stored(slots, write, TS_STORED_MASK)
    tower_dev_put(b, "{name}.rows", built.host)
    var inscope leaves : array<TsLeaf>
    leaves |> resize(length(built.stored))
    let own = ts_own_sites(dev, dense)
    var inscope members <- [for (p in built.stored); ts_leaf_member(p, own, dense_fmts)]
    for (m in range(TS_ST_MEMBERS)) {
        var inscope @exact_size wq : array<uint8>
        var inscope @exact_size ws : array<uint8>
        ts_gather_member(built.stored, members, leaves, m, wq, ws, dev)
        tower_dev_put(b, "{name}.wq{m}", wq)
        tower_dev_put(b, "{name}.ws{m}", ws)
    }
    tower_dev_put(b, "{name}.leaves", leaves)
    var inscope rec <- mem_archive_save(slots)
    tower_dev_put(b, "{name}.slots", rec)
    if (spend) {   // a part whose weights host code still reads keeps them on the host too
        b.spent |> push_from(built.spent)
    }
    unsafe {
        delete built
    }
}

//! the piece `name` of `blk` onto fresh device rows `r`; true for a piece the block holds empty or not at all (`r` stays none)
[cold_path]
def private ts_piece_upload(rl : auto(R); var r : RailBuf; blk : TtsDevBlock; name, what : string) : bool {
    let pc = tower_dev_find(tts_block_plan(blk), name)
    r = RailBuf()
    return true if (pc.bytes <= 0l)
    return false if (!ts_rows_alloc(rl, r, pc.bytes, 0u, what))
    let src = plane_at(tts_block_dev(blk), pc.off)
    _::rail_tts_put(rl, r.dev, 0l, src, pc.bytes)
    if (blk.mapped) {   // the device holds the piece now: the mapped pages are a host copy nothing reads
        unsafe {
            g_ts_released += release_map(reinterpret<void?>(src), uint64(pc.bytes)) ? pc.bytes : 0l
        }
    }
    return true
}

var private g_ts_released = 0l   //! the mapped block bytes let go after their upload, since the process started

//! the mapped device-block bytes a home let go after uploading them, since the process started - a cell's witness
[cold_path]
def tts_block_released_bytes_ : int64 => g_ts_released

//! a Pocket model's device block as its seats attach it
def private ts_pk_block(m : PocketModel) : TtsDevBlock => tts_dev_block(m.dev, m.devplan, false, image_backing_mapped(m.image_map))

[cold_path, arch(at = "../ARCHITECTURE_IMAGE_GPU_PLANES.md#tower-dev-block"), arch(at="../ARCHITECTURE_GPU_TOWER_TTS.md#tts-st2-seats")]
def private ts_slab_attach(rl : auto(R); var sl : TsSlab; var slots : auto(SS); blk : TtsDevBlock; name, what : string) : bool {
    let rows = tower_dev_find(tts_block_plan(blk), "{name}.rows")
    return false if (rows.off < 0l || rows.bytes <= 0l)
    let key = intptr(plane_at(tts_block_dev(blk), rows.off))
    return true if (ts_slab_current(sl, key))
    let t0 = ref_time_ticks()
    ts_slab_release(rl, sl)
    var ok = ts_piece_upload(rl, sl.rows, blk, "{name}.rows", what)
    for (m in range(TS_ST_MEMBERS)) {
        ok = ok && ts_piece_upload(rl, sl.wq[m], blk, "{name}.wq{m}", "stored quant plane") && ts_piece_upload(rl, sl.ws[m], blk, "{name}.ws{m}", "stored scale plane")
    }
    let lv = tower_dev_find(tts_block_plan(blk), "{name}.leaves")
    let sp = tower_dev_find(tts_block_plan(blk), "{name}.slots")
    if (!ok || sp.bytes <= 0l) {
        ts_slab_release(rl, sl)
        return false
    }
    sl.leaves |> resize(lv.bytes / int64(typeinfo sizeof(type<TsLeaf>)))
    var inscope @exact_size rec : array<uint8>
    rec |> reserve_resize(sp.bytes)
    unsafe {
        if (lv.bytes > 0l) {
            memcpy(addr(sl.leaves[0]), plane_at(tts_block_dev(blk), lv.off), lv.bytes)
        }
        memcpy(addr(rec[0]), plane_at(tts_block_dev(blk), sp.off), sp.bytes)
    }
    var inscope fresh : SS
    mem_archive_load(rec, fresh)
    delete slots
    slots <- fresh
    sl.key = key
    ts_attach_log(rl, what, rows.bytes / 4l, 0, get_time_usec(t0))
    return true
}

let private TS_BLOCK_PLANES = 3ul   //! the pieces' byte layout: bumped when a gather, a piece name or a record's archive changes with no struct moving
let private TS_BLOCK_METAL = 3ul    //! the Metal home's stored planes: its own layout, bumped with `ts_gather_member_metal`

//! one slot record's shape into a block layout
def private ts_block_fold(var layout : uint64&; var proto) {
    layout = hash_combine64(layout, layout_fingerprint(proto))
    delete proto
}

//! the device-block layout of a StyleTTS2 model on home `dev`: each part's slot record, the stored leaf record, the planes' version
[cold_path]
def tts_st2_block_layout(dev : TowerDev) : uint64 {
    var layout = hash_combine64(TS_BLOCK_PLANES, dev == TowerDev.metal ? TS_BLOCK_METAL : 0ul)
    ts_block_fold(layout, TtsAlbertSlab())
    ts_block_fold(layout, TtsTextSlab())
    ts_block_fold(layout, TtsPredSlab())
    ts_block_fold(layout, TtsDecSlab())
    ts_block_fold(layout, TsLeaf())
    return layout
}

//! ... of a Pocket model
[cold_path]
def tts_pk_block_layout(dev : TowerDev) : uint64 {
    var layout = hash_combine64(TS_BLOCK_PLANES, dev == TowerDev.metal ? TS_BLOCK_METAL : 0ul)
    ts_block_fold(layout, TtsPkCodecSlab())
    ts_block_fold(layout, TtsPkFramesSlab())
    ts_block_fold(layout, TtsPkEncSlab())
    ts_block_fold(layout, TsLeaf())
    return layout
}

[cold_path, arch(at="../ARCHITECTURE_GPU_TOWER_TTS.md#tts-st2-seats")]
def tts_st2_bake(m : StyleTts2Model; var b : TowerDevBuild; dev : TowerDev) : bool {
    //! false (the model takes no flavor) where a part's shapes sit off the seats' stamps; device-free
    return false if (!tts_albert_ok(m.albert) || !tts_text_ok(m.text_encoder, TS_LSTM_MAX_HIDDEN) || !tts_pred_ok(m.predictor, TS_LSTM_MAX_HIDDEN) || !tts_dec_ok(m.decoder))
    var inscope sa : TtsAlbertSlab
    tts_part_bake(b, dev, "st2.albert", sa, $(var w : TtsSlabWriter; var s : TtsAlbertSlab) {
        tts_albert_write(w, m.albert, s)
    }, true, true)
    var inscope st : TtsTextSlab
    tts_part_bake(b, dev, "st2.text", st, $(var w : TtsSlabWriter; var s : TtsTextSlab) {
        tts_text_write(w, m.text_encoder, s)
    }, true, true)
    var inscope sp : TtsPredSlab
    tts_part_bake(b, dev, "st2.pred", sp, $(var w : TtsSlabWriter; var s : TtsPredSlab) {
        tts_pred_write(w, m.predictor, s)
    }, true, true)
    var inscope sd : TtsDecSlab
    tts_part_bake(b, dev, "st2.dec", sd, $(var w : TtsSlabWriter; var s : TtsDecSlab) {
        tts_dec_write(w, m.decoder, s)
    }, true, true)
    return true
}

//! a StyleTTS2 part's slab off the served model's block: the albert, the text encoder's and the predictor's
def private ts_albert_attach(rl : auto(R); blk : TtsDevBlock) : bool
    => ts_slab_attach(rl, g_ts_albert, g_ts_albert_slots, blk, "st2.albert", "albert slab")

def private ts_text_attach(rl : auto(R); blk : TtsDevBlock) : bool
    => ts_slab_attach(rl, g_ts_text, g_ts_text_slots, blk, "st2.text", "text slab")

def private ts_pred_attach(rl : auto(R); blk : TtsDevBlock) : bool
    => ts_slab_attach(rl, g_ts_pred, g_ts_pred_slots, blk, "st2.pred", "predictor slab")

let private TS_SCRATCH_BUFS = 30
let private TS_AUX_BIT = 1u << 30u       //! the hazard bit of a scratch's aux rows
let private TS_STATS_BIT = 1u << 31u     //! ... and of its column stats
let private TS_SCRATCH_GRAIN = 65536l    //! a slot grows in 256 KB steps, so a call a little wider reuses it

//! a seat's activation rows: one device buffer a slot sized in floats (0 = no buffer) and two more row sets - `aux`, the
//! per-call uploads (a style vector, fc(style) per norm), and `stats`, the AdaIN column stats the device writes. What a
//! seat reads back comes through the home's readback buffer, grown to the widest read (the widest slot unless the seat says less)
struct private TsScratch {
    devs : uint64[TS_SCRATCH_BUFS]
    bytes : int64[TS_SCRATCH_BUFS]
    aux : RailBuf
    stats : RailBuf
}

//! a seat's slot sizes in floats, filled by slot name
typedef TsFloats = int64[TS_SCRATCH_BUFS]

var private g_ts_albert_scratch : TsScratch
var private g_ts_text_scratch : TsScratch
var private g_ts_de_scratch : TsScratch
var private g_ts_du_scratch : TsScratch
var private g_ts_pr_scratch : TsScratch

def private ts_rows_release(rl : auto(R); var r : RailBuf) {
    _::rail_tts_free(rl, r.dev)
    r.bytes = 0l
}

def private ts_scratch_release(rl : auto(R); var s : TsScratch) {
    for (i in range(TS_SCRATCH_BUFS)) {
        _::rail_tts_free(rl, s.devs[i])
        s.bytes[i] = 0l
    }
    ts_rows_release(rl, s.aux)
    ts_rows_release(rl, s.stats)
}

//! the floats the readback buffer holds: `hb_floats` where the seat names its readback, else the widest slot
def private ts_hb_floats(floats : TsFloats; hb_floats : int64) : int64 {
    return hb_floats if (hb_floats >= 0l)
    var widest = 0l
    for (i in range(TS_SCRATCH_BUFS)) {
        widest = max(widest, floats[i])
    }
    return widest
}

def private ts_scratch_fits(rl : auto(R); s : TsScratch; floats : TsFloats; aux_floats, stats_floats, hb_floats : int64) : bool {
    return false if (s.aux.bytes < aux_floats * 4l || s.stats.bytes < stats_floats * 4l)
    for (i in range(TS_SCRATCH_BUFS)) {
        return false if (s.bytes[i] < floats[i] * 4l)
    }
    return _::rail_tts_take_bytes(rl) >= ts_hb_floats(floats, hb_floats) * 4l
}

//! `bytes` on the device under `bit`, none at zero; false when the allocation failed or the rows would bind past the storage range (said naming `what`).
//! Per-call rows, the slabs too: `ts_upload` refills rows of every kind
[cold_path]
def private ts_rows_alloc(rl : auto(R); var r : RailBuf; bytes : int64; bit : uint; what : string = "rows") : bool {
    r = RailBuf(bit = bit)
    return true if (bytes <= 0l)
    return false if (!ts_range_ok(rl, what, bytes))
    var oom = false
    r.bytes = bytes
    r.dev = _::rail_tts_alloc(rl, oom, r.bytes)
    return !oom && r.dev != 0ul
}

let private TS_SCRATCH_HEADROOM = 4l   //! a slot is sized this fraction past the call's need: a longer sentence reuses it

//! `floats` grown by the headroom to the grain; the exact round-up where the grown size would bind past the storage range
def private ts_grown(rl : auto(R); floats : int64) : int64 {
    let grown = round_up(floats + floats / TS_SCRATCH_HEADROOM, TS_SCRATCH_GRAIN)
    return _::rail_tts_bind_ok(rl, grown * 4l) ? grown : round_up(floats, TS_SCRATCH_GRAIN)
}

//! false when an allocation fails or a slot would bind past the storage range; every slot's size is settled before the old scratch goes, and a slot never shrinks
[cold_path]
def private ts_scratch_build(rl : auto(R); var s : TsScratch; floats : TsFloats; aux_floats, stats_floats, hb_floats : int64) : bool {
    var grown : TsFloats
    for (i in range(TS_SCRATCH_BUFS)) {
        grown[i] = max(floats[i] > 0l ? ts_grown(rl, floats[i]) : 0l, s.bytes[i] / 4l)   //! a slot this call leaves idle keeps its size: the seats sharing the scratch alternate
        return false if (grown[i] > 0l && !ts_range_ok(rl, "scratch slot", grown[i] * 4l))
    }
    ts_scratch_release(rl, s)
    let t0 = ref_time_ticks()
    var oom = false
    var total = 0l
    for (i in range(TS_SCRATCH_BUFS)) {
        if (grown[i] > 0l) {
            s.bytes[i] = grown[i] * 4l
            s.devs[i] = _::rail_tts_alloc(rl, oom, s.bytes[i])
            total += grown[i]
        }
    }
    let dev_us = get_time_usec(t0)
    let took = _::rail_tts_take_ready(rl, ts_grown(rl, max(ts_hb_floats(floats, hb_floats), 1l)) * 4l)
    if (oom || !took || !ts_rows_alloc(rl, s.aux, ts_grown(rl, aux_floats) * 4l, TS_AUX_BIT) || !ts_rows_alloc(rl, s.stats, ts_grown(rl, stats_floats) * 4l, TS_STATS_BIT)) {
        ts_scratch_release(rl, s)
        return false
    }
    if (_::rail_tts_prof(rl)) {
        to_log(LOG_INFO, "{_::rail_tts_tag(rl)} tts scratch: {total * 4l / 1048576l} MB over the slots in {dev_us / 1000} ms, the {_::rail_tts_take_bytes(rl) / 1048576l} MB readback buffer in {(get_time_usec(t0) - dev_us) / 1000} ms\n")
    }
    return true
}

def private ts_scratch_ready(rl : auto(R); var s : TsScratch; floats : TsFloats; aux_floats, stats_floats : int64; hb_floats : int64 = -1l) : bool
    => ts_scratch_fits(rl, s, floats, aux_floats, stats_floats, hb_floats) || ts_scratch_build(rl, s, floats, aux_floats, stats_floats, hb_floats)

//! the scratch's slot as a dispatch binds it; the hazard bit is the slot's
def private ts_rows(s : TsScratch; slot : int) : RailBuf => RailBuf(dev = s.devs[slot], bytes = s.bytes[slot], bit = 1u << uint(slot))

def private ts_pk_codec_caches_drop(rl : auto(R)) {
    for (r in g_ts_pkc_k) {
        ts_rows_release(rl, r)
    }
    for (r in g_ts_pkc_v) {
        ts_rows_release(rl, r)
    }
    g_ts_pkc_k |> clear()
    g_ts_pkc_v |> clear()
    g_ts_pkc_kv_rows = 0l
}

[cold_path, arch(at="../ARCHITECTURE_GPU_TOWER_TTS.md#tts-st2-seats")]
def tts_forget(rl : auto(R)) {
    g_ts_declines[int(_::rail_tts_dev(rl))].said |> clear()   // the next model's declines are said anew, whichever home holds the device state
    rail_prof_forget()
    return if (g_ts_owner != _::rail_tts_dev(rl))
    tts_release_activations(rl)
    ts_each_slab() $(var sl : TsSlab) {
        ts_slab_release(rl, sl)
    }
    ts_pk_voice_drop(rl)
    g_ts_owner = TowerDev.none
}

//! the activation rows the rail's home holds for the driver given back - every seat's scratch, the
//! Pocket codec caches, the readback buffer and the schedule plane - its slabs and voice slots
//! kept; the next seat grows them back
[cold_path, arch(at = "../ARCHITECTURE_TTS_MEMORY.md#tts-device-release")]
def tts_release_activations(rl : auto(R)) {
    return if (g_ts_owner != _::rail_tts_dev(rl))
    ts_each_scratch() $(var s : TsScratch) {
        ts_scratch_release(rl, s)
    }
    ts_pk_codec_caches_drop(rl)
    _::rail_tts_take_drop(rl)
    _::rail_tts_mapped_free(rl, g_ts_sched)
    g_ts_sched_words = 0l
}

//! `blk` over every seat's scratch the driver keeps
def private ts_each_scratch(blk : block<(var s : TsScratch) : void>) {
    invoke(blk, g_ts_albert_scratch)
    invoke(blk, g_ts_text_scratch)
    invoke(blk, g_ts_de_scratch)
    invoke(blk, g_ts_du_scratch)
    invoke(blk, g_ts_pr_scratch)
    invoke(blk, g_ts_dec_scratch)
    invoke(blk, g_ts_pkc_scratch)
    invoke(blk, g_ts_pkf_scratch)
    invoke(blk, g_ts_pkp_scratch)
}

//! `blk` over every slab the driver keeps
def private ts_each_slab(blk : block<(var sl : TsSlab) : void>) {
    invoke(blk, g_ts_albert)
    invoke(blk, g_ts_text)
    invoke(blk, g_ts_pred)
    invoke(blk, g_ts_dec)
    invoke(blk, g_ts_pk_codec)
    invoke(blk, g_ts_pk_frames)
    invoke(blk, g_ts_pk_enc)
}

def private ts_rows_bytes(r : RailBuf) : int64 => r.dev != 0ul ? r.bytes : 0l

//! the device bytes the rail's home holds for the driver: the slabs and voice slots, and the
//! activation rows; the readback buffer counted on a home that keeps one (a home whose reads need
//! none answers `LONG_MAX`)
[cold_path]
def tts_held(rl : auto(R)) : tuple<weights : int64; activations : int64> {
    var w = 0l
    ts_each_slab() $(var sl : TsSlab) {
        w += ts_rows_bytes(sl.rows)
        for (m in range(TS_ST_MEMBERS)) {
            w += ts_rows_bytes(sl.wq[m]) + ts_rows_bytes(sl.ws[m])
        }
    }
    for (r in g_ts_pk_voice.k) {
        w += ts_rows_bytes(r)
    }
    for (r in g_ts_pk_voice.v) {
        w += ts_rows_bytes(r)
    }
    w += ts_rows_bytes(g_ts_pk_voice.rope)
    var a = 0l
    ts_each_scratch() $(var s : TsScratch) {
        for (i in range(TS_SCRATCH_BUFS)) {
            a += s.devs[i] != 0ul ? s.bytes[i] : 0l
        }
        a += ts_rows_bytes(s.aux) + ts_rows_bytes(s.stats)
    }
    for (r in g_ts_pkc_k) {
        a += ts_rows_bytes(r)
    }
    for (r in g_ts_pkc_v) {
        a += ts_rows_bytes(r)
    }
    let take = _::rail_tts_take_bytes(rl)
    a += (take == LONG_MAX ? 0l : take) + (g_ts_sched.dev != 0ul ? g_ts_sched.bytes : 0l)
    return (weights = w, activations = a)
}

var private @scratch g_ts_aux_host : array<float>   //! the staged rows, `g_ts_aux_n` of them live; grow-only
var private g_ts_aux_n = 0l
var private @scratch g_ts_idx_host : array<int>

def private ts_aux_reset {
    g_ts_aux_n = 0l
}

//! `n` staged floats reserved at the next 16-byte boundary of the rows; the element offset
def private ts_aux_alloc(n : int64) : int64 {
    let at = round_up(g_ts_aux_n, 4l)
    g_ts_aux_host |> ensure_length(at + n)
    g_ts_aux_n = at + n
    return at
}

//! `n` floats of `src` staged; the element offset
def private ts_aux_push(src : array<float>; n : int64) : int64 {
    let at = ts_aux_alloc(n)
    for (i in range64(n)) {
        g_ts_aux_host[at + i] = src[i]
    }
    return at
}

//! a style projection queued for the device: `slot`'s fc over the style vector into 2 `c` aux floats at `at`
struct private TsStyleJob {
    slot : TtsConvSlot
    at : int64
    c : int64
}

var private @scratch g_ts_style_jobs : array<TsStyleJob>
var private g_ts_style_job_n = 0l
var private g_ts_style_at = 0l   //! the style vector's element offset in the aux rows
var private g_ts_style_n = 0l    //! ... and its length

//! the seat's style vector staged at the head of its aux rows: what the style projections read on the device
def private ts_style_begin(s : array<float>) {
    ts_aux_reset()
    g_ts_style_job_n = 0l
    g_ts_style_n = long_length(s)
    g_ts_style_at = ts_aux_push(s, g_ts_style_n)
}

//! a norm's fc(style) rows - gamma [c] then beta [c] - reserved in the aux rows and queued (`ts_style_flush`); the element offset
def private ts_style_rows(slot : TtsConvSlot; c : int64) : int64 {
    let at = ts_aux_alloc(2l * c)
    g_ts_style_jobs |> ensure_length(g_ts_style_job_n + 1l)
    g_ts_style_jobs[g_ts_style_job_n++] = TsStyleJob(slot = slot, at = at, c = c)
    return at
}

//! the queued style projections, each one row of the exact tile over the style vector into its aux rows: a seat's first dispatches
def private ts_style_flush(var rl : auto(R); slab : TsSlab; aux : RailBuf) {
    for (i in range64(g_ts_style_job_n)) {
        let j = g_ts_style_jobs[i]
        ts_lin(rl, slab, j.slot, g_ts_style_n, 2l * j.c, aux, aux, 1l, true, g_ts_style_at, j.at)
    }
}

//! the aux floats a seat's style vector takes beside its norms' rows
def private ts_style_floats(s : array<float>) : int64 => round_up(long_length(s), 4l)

//! the staged rows onto the scratch's aux buffer; false where the seat sized it short
def private ts_aux_upload(rl : auto(R); s : TsScratch) : bool {
    let n = g_ts_aux_n
    return true if (n == 0l)
    return false if (n * 4l > s.aux.bytes)
    ts_upload(rl, s.aux, g_ts_aux_host, n)
    return true
}

def private ts_gemm_args(slot : TtsConvSlot; nin, nout, rows : int64; xoff : int64 = 0l; yoff : int64 = 0l) : F32GemmArgs
    => F32GemmArgs(n = uint(nin), d = uint(nout), rows = uint(rows), woff = uint(slot.woff), wstride = uint(slot.kdim_p), boff = uint(slot.boff),
        xoff = uint(xoff), xstride = uint(nin), yoff = uint(yoff), ystride = uint(nout))

//! the stored GEMM's one dispatch: the exact tile's stamp of `fmt` (`TTS_PK_ST_HALVES`: half rows) over `wq` / `ws`, the bias off `rows`
def tts_mm_stored_enc(var rl : auto(R); fmt : int; wq, x, y, rows, ws : RailBuf; var pa : F32GemmArgs; wgs : int64) {
    if (fmt == TTS_PK_ST_HALVES) {
        _::enc_f16_f32_gemm_cls(rl, _::rail_seats(rl, fixed_array(wq, x, y, rows)), pa, wgs)
    } else {
        kq_tile_stamp(fmt, TS_PKST_SKIP, "f32_gemm") <| $() {
            _::tile_enc_cls(rl, _::rail_seats(rl, fixed_array(wq, x, y, rows, ws)), pa, wgs)
        }
    }
}

//! y = x W^T, exact tile, `slot`'s leaf as stored, `n` columns of `xstride`-wide x, bias at `boff` (`F32G_NO_BIAS`: none); `tcin` a lift's
def private ts_mm_stored(var rl : auto(R); slab : TsSlab; slot : TtsConvSlot; n, nout, xstride : int64; boff : uint; tcin : int64; x, y : RailBuf; rows : int64;
                         xoff : int64 = 0l; yoff : int64 = 0l) {
    let leaf = slab.leaves[slot.stored - 1]
    assert(leaf.member < TS_ST_OWN, "a leaf in a home's own layout is its own tile's to read")
    var pa = F32GemmArgs(n = uint(n), d = uint(nout), rows = uint(rows), woff = uint(leaf.woff), wstride = uint(leaf.wstride), boff = boff, xoff = uint(xoff),
        xstride = uint(xstride), yoff = uint(yoff), ystride = uint(nout), tcin = uint(tcin))
    let wgs = f32_gemm_wgs(rows, nout)
    tts_mm_stored_enc(rl, leaf.fmt, slab.wq[leaf.member], x, y, slab.rows, slab.ws[leaf.member], pa, wgs)
    _::rail_pt(rl, RailProfRole.tts_gemm)
}

//! the exact tile's stored members' stamps built: what `tts_mm_stored_enc` dispatches
def tts_mm_stored_ensure(rl : auto(R)) : bool {
    return (_::rail_ensure_q8_f32_gemm_cls(rl) && _::rail_ensure_kq_f32_gemm_k4_cls(rl) && _::rail_ensure_kq_f32_gemm_k5_cls(rl) && _::rail_ensure_kq_f32_gemm_k6_cls(rl)
        && _::rail_ensure_f16_f32_gemm_cls(rl))
}

//! the stored members' tile stamps and the home's site stamps built
def private ts_stored_ensure(rl : auto(R)) : bool => tts_mm_stored_ensure(rl) && _::rail_tts_sites_ensure(rl)

var private g_ts_ready_fault = false   //! a cell's seam: the seats' classes read as not built

//! a cell's seam on the flavor pick's fall to the CPU image: `tts_st2_ready`/`tts_pk_ready` fail as with unbuilt classes; old fault back
[cold_path]
def set_tts_ready_fault_(on : bool) : bool {
    let was = g_ts_ready_fault
    g_ts_ready_fault = on
    return was
}

//! every class a StyleTTS2 seat dispatches built on `rl`: what a home's flavor pick asks before it admits a lane
def tts_st2_ready(rl : auto(R)) : bool => !g_ts_ready_fault && ts_albert_ensure(rl) && ts_text_ensure(rl) && ts_dec_ensure(rl)

//! every class a Pocket seat dispatches built on `rl`, asked the same way
def tts_pk_ready(rl : auto(R)) : bool => !g_ts_ready_fault && ts_pk_codec_ensure(rl) && ts_pk_frames_ensure(rl)

//! the dense layer as a home's own tile takes it: its rows in the slab or its stored leaf's place in the home's planes
def private ts_mm_site(slab : TsSlab; slot : TtsConvSlot; nin, nout, rows : int64; biased : bool; xoff, yoff : int64) : TtsMmSite {
    var site = TtsMmSite(nin = nin, nout = nout, rows = rows, xoff = xoff, yoff = yoff, biased = biased, cout_p = slot.cout_p, kdim_p = slot.kdim_p,
        woff = slot.woff, boff = slot.boff)
    if (slot.stored != 0) {
        let leaf = slab.leaves[slot.stored - 1]
        site.stored = true
        site.halves = leaf.member == TS_ST_F16
        site.native = leaf.member >= TS_ST_OWN
        site.fmt = ts_leaf_kq(leaf)
        site.kdim_p = leaf.wstride
        site.qoff = leaf.woff
        site.soff = leaf.soff
        site.doff = leaf.doff
    }
    return site
}

//! the planes a conv's weights sit in: the slab's rows, or its stored leaf's plane pair
def private ts_conv_wq(slab : TsSlab; slot : TtsConvSlot) : RailBuf => slot.stored != 0 ? slab.wq[slab.leaves[slot.stored - 1].member] : RailBuf()

def private ts_conv_ws(slab : TsSlab; slot : TtsConvSlot) : RailBuf => slot.stored != 0 ? slab.ws[slab.leaves[slot.stored - 1].member] : RailBuf()

//! a dense linear on the f32 tile, y = x W^T + b over the slab's rows, or over the layer's stored leaf; `biased` false leaves the bias to the seam after (`ts_add_ln`)
def private ts_lin(var rl : auto(R); slab : TsSlab; slot : TtsConvSlot; nin, nout : int64; x, y : RailBuf; rows : int64; biased : bool = true;
                   xoff : int64 = 0l; yoff : int64 = 0l) {
    ts_count_widened(rl, slot.widened)
    if (_::rail_tts_mm(rl, slab.rows, ts_conv_wq(slab, slot), ts_conv_ws(slab, slot), ts_mm_site(slab, slot, nin, nout, rows, biased, xoff, yoff), x, y)) {
        _::rail_pt(rl, RailProfRole.tts_gemm)
        return   // the home's own GEMM tile served it, bias and all
    }
    if (slot.stored != 0) {
        ts_mm_stored(rl, slab, slot, nin, nout, nin, biased ? uint(slot.boff) : F32G_NO_BIAS, 0l, x, y, rows, xoff, yoff)
        return
    }
    var pa = ts_gemm_args(slot, nin, nout, rows, xoff, yoff)
    if (biased) {
        _::enc_f32_gemm_b_cls(rl, _::rail_seats(rl, fixed_array(slab.rows, x, y, slab.rows)), pa, f32_gemm_wgs(rows, nout))
    } else {
        _::enc_f32_gemm_cls(rl, _::rail_seats(rl, fixed_array(slab.rows, x, y)), pa, f32_gemm_wgs(rows, nout))
    }
    _::rail_pt(rl, RailProfRole.tts_gemm)
}

//! layernorm rows from x into y (y may be x) against a norm slot's scale and shift rows
def private ts_ln(var rl : auto(R); slab : TsSlab; ns : TtsNormSlot; x, y : RailBuf; rows, d : int64; eps : float) {
    ts_ln_at(rl, slab.rows, ns.gwoff, ns.gboff, x, y, rows, d, eps)
}

//! layernorm rows from x into y (y may be x) against scale and shift rows at `woff` / `boff` of `wn` (the slab, or the aux rows fc(style) landed in)
def private ts_ln_at(var rl : auto(R); wn : RailBuf; woff, boff : int64; x, y : RailBuf; rows, d : int64; eps : float) {
    var pa = gk_ln_args(uint(d), uint(rows), eps, uint(woff), uint(boff))
    _::enc_tower_ln_cls(rl, _::rail_seats(rl, fixed_array(x, y, wn)), pa, rows)
    _::rail_pt(rl, RailProfRole.tts_ln)
}

//! the post-LN residual seam: x += y + b (the branch linear's bias row, `bslot`), then xb = layernorm(x) on the norm slot's rows
def private ts_add_ln(var rl : auto(R); slab : TsSlab; bslot : TtsConvSlot; ns : TtsNormSlot; x, y, xb : RailBuf; rows, d : int64; eps : float) {
    var pa = GkPostAddLnArgs(d = uint(d), rows = uint(rows), woff = 0u, boff = uint(bslot.boff), eps = eps, ln_on = 1u, lnoff = uint(ns.gwoff), lnboff = uint(ns.gboff), ascale = 1.0, xboff = 0u)
    _::enc_tower_post_add_ln_cls(rl, _::rail_seats(rl, fixed_array(x, y, xb, slab.rows)), pa, rows)
    _::rail_pt(rl, RailProfRole.tts_ln)
}

//! the leaky ReLU over x's first `nelem` elements in place
def private ts_leaky(var rl : auto(R); x : RailBuf; nelem : int64; slope : float) {
    var pa = TtsElemArgs(nelem = uint(nelem), off = 0u, slope = slope)
    _::enc_tts_leaky_cls(rl, _::rail_seats(rl, fixed_array(x)), pa, nelem)
    _::rail_pt(rl, RailProfRole.tts_act)
}

//! a conv on rows, forward or transposed: x [tin][cin] gathered into col [tout][kdim_p], then the biased tile over the slot's rows into y [tout][cout]
def private ts_conv(var rl : auto(R); slab : TsSlab; slot : TtsConvSlot; cv : TtsConv1d; tin, tout : int64; x, col, y : RailBuf; half : bool = false) {
    ts_conv_padded(rl, slab, slot, cv, tin, tout, cv.pad_l, x, col, y, half)
}

//! the conv as a home's own tile takes it: its geometry under the call's left pad, its rows in the slab or its stored leaf's place in the home's planes
def private ts_conv_site(slab : TsSlab; slot : TtsConvSlot; cv : TtsConv1d; tin, tout, pad_l : int64; half : bool) : TtsConvSite {
    var site = TtsConvSite(cin = cv.cin, k = cv.k, stride = cv.stride, pad_l = pad_l, dilation = cv.dilation, transposed = cv.transposed, half = half,
        tin = tin, tout = tout, cout = cv.cout, cout_p = slot.cout_p, kdim_p = slot.kdim_p, woff = slot.woff, boff = slot.boff)
    if (slot.stored != 0) {
        let leaf = slab.leaves[slot.stored - 1]
        site.stored = true
        site.halves = leaf.member == TS_ST_F16
        site.native = leaf.member >= TS_ST_OWN
        site.fmt = ts_leaf_kq(leaf)
        site.qoff = leaf.woff
        site.soff = leaf.soff
        site.doff = leaf.doff
    }
    return site
}

//! x [tin][cin] gathered into col [tout][kdim_p] under the left pad `pad_l`, the columns as halves under `half`
def private ts_im2col(var rl : auto(R); slot : TtsConvSlot; cv : TtsConv1d; tin, tout, pad_l : int64; x, col : RailBuf; half : bool) {
    var pc = GkIm2col1dArgs(cin = uint(cv.cin), tin = uint(tin), tout = uint(tout), k = uint(cv.k), stride = uint(cv.stride), pad_l = uint(pad_l),
        dilation = uint(cv.dilation), transposed = cv.transposed ? 1u : 0u, kdim = uint(slot.kdim_p), xoff = 0u, coloff = 0u, row_major = 1u, r0 = 0u, mp = uint(tout))
    if (half) {
        _::enc_tts_im2col16_cls(rl, _::rail_seats(rl, fixed_array(x, col)), pc, tout * slot.kdim_p)
    } else {
        _::enc_tts_im2col_cls(rl, _::rail_seats(rl, fixed_array(x, col)), pc, tout * slot.kdim_p)
    }
    _::rail_pt(rl, RailProfRole.tts_im2col)
}

//! `ts_conv` under a left pad of the caller's choosing (the codec convs run the CPU's first-window pad, not the conv's own)
def private ts_conv_padded(var rl : auto(R); slab : TsSlab; slot : TtsConvSlot; cv : TtsConv1d; tin, tout, pad_l : int64; x, col, y : RailBuf;
                           half : bool = false) {
    ts_count_widened(rl, slot.widened)
    if (_::rail_tts_conv(rl, slab.rows, ts_conv_wq(slab, slot), ts_conv_ws(slab, slot), ts_conv_site(slab, slot, cv, tin, tout, pad_l, half), x, y)) {
        _::rail_pt(rl, RailProfRole.tts_gemm)
        return   // the home's own conv tile served it, bias and all
    }
    ts_im2col(rl, slot, cv, tin, tout, pad_l, x, col, false)
    if (slot.stored != 0) {
        ts_mm_stored(rl, slab, slot, cv.k * cv.cin, cv.cout, slot.kdim_p, uint(slot.boff), cv.transposed ? cv.cin : 0l, col, y, tout)
        return
    }
    var pm = F32GemmArgs(n = uint(cv.k * cv.cin), d = uint(cv.cout), rows = uint(tout), woff = uint(slot.woff), wstride = uint(slot.kdim_p), boff = uint(slot.boff),
        xoff = 0u, xstride = uint(slot.kdim_p), yoff = 0u, ystride = uint(cv.cout))
    _::enc_f32_gemm_b_cls(rl, _::rail_seats(rl, fixed_array(slab.rows, col, y, slab.rows)), pm, f32_gemm_wgs(tout, cv.cout))
    _::rail_pt(rl, RailProfRole.tts_gemm)
}

let private TS_SCHED_RECS = 512l   //! the stored convs one chain rides on the cm2 tiles: a schedule record each

var private g_ts_sched : RailMapped    //! the cm2 route's schedule: `TS_SCHED_RECS` records at its head, each dispatch's map behind them
var private g_ts_sched_used = 0l       //! the records this call wrote
var private g_ts_sched_cur = 0l        //! the next map's word
var private g_ts_sched_words = 0l      //! the plane's words

[cold_path]
def private ts_sched_build(rl : auto(R); words : int64) {
    _::rail_tts_mapped_free(rl, g_ts_sched)
    g_ts_sched = _::rail_tts_mapped_make(rl, words * 4l)
    g_ts_sched_words = g_ts_sched.dev != 0ul ? words : 0l
}

//! the schedule plane emptied for a call whose cm2 dispatches take `groups` map words; without it every conv rides the exact tile
def private ts_sched_begin(rl : auto(R); groups : int64) {
    return if (!_::rail_tts_cm2_on(rl))
    let words = TS_SCHED_RECS * 4l + groups
    if (g_ts_sched_words < words) {
        ts_sched_build(rl, round_up(words + words / TS_SCRATCH_HEADROOM, TS_SCRATCH_GRAIN))
    }
    g_ts_sched_used = 0l
    g_ts_sched_cur = TS_SCHED_RECS * 4l
}

//! x [rows x n] halves against `leaf` on its member's cm2 tile into y [rows x dout], record and map in place; false: no tile or room
def private ts_cm2_mm(var rl : auto(R); slab : TsSlab; leaf : TsLeaf; xh, y : RailBuf; n, dout, rows : int64) : bool {
    let site = RailSite.conv2
    let fmt = ts_leaf_kq(leaf)
    return false if (g_ts_sched_words == 0l || g_ts_sched_used >= TS_SCHED_RECS || !ts_cm2_pick(rl, fmt, dout, rows))
    let groups = _::rail_site_groups(rl, site, dout, rows)
    return false if (g_ts_sched_cur + groups > g_ts_sched_words)
    let wblk0 = leaf.woff / (leaf.member == TS_ST_Q8 ? Q8_BLOCK_ELEMS : KQ_SB_ELEMS)
    rl.maps |> reserve(1)
    rl.maps |> resize(1)
    unsafe {
        rl.maps[0] = batch_sched_map(reinterpret<uint?>(g_ts_sched.host), g_ts_sched_cur, g_ts_sched_used, wblk0, _::rail_site_rows(rl, site, rows), groups)
    }
    g_ts_sched_used++
    let meta = RailBuf(dev = g_ts_sched.dev, bytes = g_ts_sched.bytes)
    let roles = _::rail_seats(rl, fixed_array(slab.wq[leaf.member], slab.ws[leaf.member], meta, xh, y))
    rl.binds[int(site)] = RailSiteBind(wq = roles[0], ws = roles[1], meta = roles[2], xh = roles[3], xq = roles[3], xs = roles[3], meta_off = 0l, fmt = fmt)
    _::rail_mm_kq(rl, site, 0l, roles[4], n, dout, rows)
    return true
}

let private TS_FAST_MIN_COUT = 16l   //! the narrowest conv the fast tiles take: under it the exact tile's one pass wins

[arch(at="../ARCHITECTURE_GPU_TOWER_TTS.md#tts-st2-seats")]
def private ts_conv_fast(var rl : auto(R); slab : TsSlab; slot : TtsConvSlot; cv : TtsConv1d; tin, tout : int64; x, col, col16, y : RailBuf) : bool {
    let leaf = slab.leaves[slot.stored - 1]
    let n = leaf.wstride
    let rows = cv.transposed ? tin : tout
    let dout = cv.transposed ? cv.k * cv.cout : cv.cout
    let room = col16.bytes >= (rows + TS_COL16_SLACK_ROWS) * n * 2l && (!cv.transposed || col.bytes >= rows * dout * 4l)
    let tiled = leaf.member == TS_ST_F16 ? _::rail_tts_f16_on(rl) : _::rail_tts_cm2_on(rl) && g_ts_sched_words != 0l && g_ts_sched_used < TS_SCHED_RECS
    return false if (cv.cout % 4l != 0l || cv.cout < TS_FAST_MIN_COUT || !room || !tiled)
    var pc = GkIm2col1dArgs(cin = uint(cv.cin), tin = uint(tin), tout = uint(rows), k = uint(cv.k), stride = uint(cv.stride), pad_l = uint(cv.pad_l),
        dilation = uint(cv.dilation), transposed = 0u, kdim = uint(n), xoff = 0u, coloff = 0u, row_major = 1u, r0 = 0u, mp = uint(rows))
    if (cv.transposed) {   // the lift takes the input rows themselves: one tap, no pad
        pc.k = 1u
        pc.stride = 1u
        pc.pad_l = 0u
        pc.dilation = 1u
    }
    let yg = cv.transposed ? col : y
    _::enc_tts_im2col16_cls(rl, _::rail_seats(rl, fixed_array(x, col16)), pc, rows * n)
    _::rail_pt(rl, RailProfRole.tts_im2col)
    if (leaf.member == TS_ST_F16) {
        let seats = _::rail_seats(rl, fixed_array(slab.wq[TS_ST_F16], col16, yg))
        _::rail_mm16(rl, seats[0], seats[1], seats[2], n, dout, rows, leaf.woff)
    } elif (!ts_cm2_mm(rl, slab, leaf, col16, yg, n, dout, rows)) {
        return false   // the feed it wrote is the next conv's to overwrite
    }
    _::rail_pt(rl, RailProfRole.tts_gemm)
    if (cv.transposed) {
        var pf = TtsFoldArgs(tin = uint(tin), tout = uint(tout), cout = uint(cv.cout), k = uint(cv.k), stride = uint(cv.stride), pad_l = uint(cv.pad_l),
            dilation = uint(cv.dilation), loff = 0u, lstride = uint(dout), ooff = 0u, ostride = uint(cv.cout))
        _::enc_tts_fold_cls(rl, _::rail_seats(rl, fixed_array(col, y)), pf, tout * cv.cout)
    }
    var pb = GkBiasActArgs(d = uint(cv.cout), nelem = uint(tout * cv.cout), boff = uint(slot.boff), act = BIAS_ACT_NONE)
    _::enc_tower_bias_act_cls(rl, _::rail_seats(rl, fixed_array(y, slab.rows)), pb, rq_wgs(false, tout * cv.cout))
    _::rail_pt(rl, RailProfRole.tts_rows)
    return true
}

//! a conv on rows: under `half` a stored leaf (its quants, or a GPU image's halves) rides the fast tiles; every other the f32 tile
def private ts_conv_at(var rl : auto(R); slab : TsSlab; slot : TtsConvSlot; cv : TtsConv1d; tin, tout : int64; x, col, col16, y : RailBuf;
                       half : bool) {
    return if (half && slot.stored != 0 && col16.dev != 0ul && ts_conv_fast(rl, slab, slot, cv, tin, tout, x, col, col16, y))
    ts_conv(rl, slab, slot, cv, tin, tout, x, col, y, half)
}

//! one LSTM direction: the input gates on the biased tile into g [t][4H], then the recurrence walk into its half of y [t][2H]
def private ts_lstm_dir(var rl : auto(R); slab : TsSlab; xs : TtsConvSlot; hh : int64; hh_stored : int; l : TtsLstm; t : int64; x, g, y : RailBuf;
                        backward : bool) {
    let hd = l.hidden
    ts_lin(rl, slab, xs, l.nin, 4l * hd, x, g, t)
    var pa = TtsLstmArgs(t = uint(t), hidden = uint(hd), goff = 0u, gstride = uint(4l * hd), hhoff = uint(hh), hoff = backward ? uint(hd) : 0u,
        hstride = uint(2l * hd), backward = backward ? 1u : 0u)
    if (hh_stored != 0) {
        let leaf = slab.leaves[hh_stored - 1]
        if (leaf.member >= TS_ST_OWN) {
            if (!_::rail_tts_lstm_q8(rl, g, slab.wq[leaf.member], y, leaf.woff, t, hd, backward)) {
                panic("dasLLAMA tts: a recurrence leaf in a home's own layout its own stamp did not take")
            }
        } else {
            pa.hhoff = uint(leaf.woff)
            _::enc_tts_lstm_dir_q8_cls(rl, _::rail_seats(rl, fixed_array(g, slab.wq[TS_ST_Q8], y, slab.ws[TS_ST_Q8])), pa, 1l)
        }
    } else {
        _::enc_tts_lstm_dir_cls(rl, _::rail_seats(rl, fixed_array(g, slab.rows, y)), pa, 1l)
    }
    _::rail_pt(rl, RailProfRole.tts_lstm)
}

//! the BiLSTM over x [t][nin]: the forward walk into y's first half, the backward into its second (the CPU `bilstm`'s rows)
def private ts_bilstm(var rl : auto(R); slab : TsSlab; ls : TtsLstmSlot; l : TtsBiLstm; t : int64; x, gf, gb, y : RailBuf) {
    ts_lstm_dir(rl, slab, ls.fx, ls.fhh, ls.fh_stored, l.fwd, t, x, gf, y, false)
    ts_lstm_dir(rl, slab, ls.bx, ls.bhh, ls.bh_stored, l.bwd, t, x, gb, y, true)
}

let private TS_A_IDS = 0
let private TS_A_E = 1
let private TS_A_X = 2
let private TS_A_XB = 3
let private TS_A_XB2 = 4
let private TS_A_Q = 5
let private TS_A_K = 6
let private TS_A_V = 7
let private TS_A_FFN = 8
let private TS_A_F2 = 9

def private ts_albert_shape_ok(a : St2Albert; t : int64) : bool {
    let d = a.hidden
    let hs = a.heads > 0l ? d / a.heads : 0l
    let ff = a.ffn.nout
    return (t >= 1l && t <= a.max_pos && t <= TS_ATTN_MAX_T && d % 64l == 0l && (hs == 64l || hs == 128l) && hs * a.heads == d
        && ff % 64l == 0l && a.emb_dim % 32l == 0l && a.map_in.nin == a.emb_dim && a.map_in.nout == d && a.q.nout == d && a.k.nout == d
        && a.v.nout == d && a.dense.nout == d && a.ffn.nin == d && a.ffn_out.nin == ff && a.ffn_out.nout == d
        && tts_held(a.pos_emb.a, a.pos_emb.span) >= t * a.emb_dim && tts_held(a.type_emb.a, a.type_emb.span) >= a.emb_dim && a.n_layers >= 1l && tts_albert_ok(a))
}

def private ts_albert_ensure(rl : auto(R)) : bool {
    return (_::rail_ensure_f32_gemm_cls(rl) && _::rail_ensure_f32_gemm_b_cls(rl) && ts_stored_ensure(rl) && _::rail_ensure_tower_ln_cls(rl) && _::rail_ensure_tower_post_add_ln_cls(rl)
        && _::rail_ensure_tts_row_gather_cls(rl) && _::rail_ensure_tts_gelu_tanh_cls(rl) && _::rail_ensure_tts_attn_cls(rl))
}

[unused_argument(sc), arch(at = "../ARCHITECTURE_GPU_TOWER_TTS.md#tts-st2-seats")]
def tts_st2_albert(var rl : auto(R); a : St2Albert; ids : array<int>; @scratch @exact_size var out : array<float>; var sc : St2Scratch) : bool {
    if (!ts_on(rl)) {
        return ts_decline(rl, "albert", TowerDecline.knob)
    }
    if (sc.blk.plan_n == 0l) {
        return ts_decline(rl, "albert", TowerDecline.flavor)
    }
    let t = long_length(ids)
    if (!ts_albert_shape_ok(a, t)) {
        return ts_decline(rl, "albert", TowerDecline.shape)
    }
    for (id in ids) {
        return ts_decline(rl, "albert", TowerDecline.shape) if (id < 0 || int64(id + 1) * a.emb_dim > tts_held(a.word_emb.a, a.word_emb.span))
    }
    if (!ts_arm_ensure(rl, "albert", $ => ts_albert_ensure(rl))) {
        return ts_decline(rl, "albert", TowerDecline.device)
    }
    let d = a.hidden
    let hs = d / a.heads
    let ff = a.ffn.nout
    var fl : TsFloats
    fl[TS_A_IDS] = t
    fl[TS_A_E] = t * a.emb_dim
    for (i in range(TS_A_X, TS_A_V + 1)) {
        fl[i] = t * d
    }
    fl[TS_A_FFN] = t * ff
    fl[TS_A_F2] = t * d
    if (!ts_albert_attach(rl, sc.blk) || !ts_scratch_ready(rl, g_ts_albert_scratch, fl, 0l, 0l, t * d)) {
        return ts_decline(rl, "albert", TowerDecline.memory)
    }
    let sl & = g_ts_albert_slots
    let sc_ & = g_ts_albert_scratch
    let slab & = g_ts_albert
    let r_ids = ts_rows(sc_, TS_A_IDS)
    let r_e = ts_rows(sc_, TS_A_E)
    let r_x = ts_rows(sc_, TS_A_X)
    let r_xb = ts_rows(sc_, TS_A_XB)
    let r_xb2 = ts_rows(sc_, TS_A_XB2)
    let r_q = ts_rows(sc_, TS_A_Q)
    let r_k = ts_rows(sc_, TS_A_K)
    let r_v = ts_rows(sc_, TS_A_V)
    let r_ffn = ts_rows(sc_, TS_A_FFN)
    let r_f2 = ts_rows(sc_, TS_A_F2)
    ts_upload(rl, r_ids, ids, t)
    var pa_gather = GkRowGatherArgs(c = uint(a.emb_dim), rows = uint(t), aoff = uint(sl.word_emb), boff = uint(sl.pos_emb), coff = uint(sl.type_emb), out_off = 0u, add_b = 1u, add_c = 1u, scale = 1.0)
    var pa_attn = TtsAttnArgs(t = uint(t), d = uint(d), hs = uint(hs), heads = uint(a.heads), scale = 1.0 / sqrt(float(hs)),
        qoff = 0u, koff = 0u, voff = 0u, ooff = 0u)
    var pa_gelu = TtsElemArgs(nelem = uint(t * ff), off = 0u, slope = 0.0)
    let eps = ST2_ALBERT_EPS
    //! x the residual stream, then the full-normed rows; xb2 the attention context, then the attention-normed rows the ffn reads
    _::rail_tts_shot(rl, "albert", a.n_layers, t) {
        _::enc_tts_row_gather_cls(rl, _::rail_seats(rl, fixed_array(slab.rows, r_ids, slab.rows, slab.rows, r_e)), pa_gather, t * a.emb_dim)
        _::rail_pt(rl, RailProfRole.tts_rows)
        ts_ln(rl, slab, sl.emb_ln, r_e, r_e, t, a.emb_dim, eps)
        ts_lin(rl, slab, sl.map_in, a.emb_dim, d, r_e, r_x, t)
        for (_l in range64(a.n_layers)) {
            ts_lin(rl, slab, sl.q, d, d, r_x, r_q, t)
            ts_lin(rl, slab, sl.k, d, d, r_x, r_k, t)
            ts_lin(rl, slab, sl.v, d, d, r_x, r_v, t)
            _::enc_tts_attn_cls(rl, _::rail_seats(rl, fixed_array(r_q, r_k, r_v, r_xb2)), pa_attn, tts_attn_wgs(t, a.heads))
            _::rail_pt(rl, RailProfRole.tts_attn)
            ts_lin(rl, slab, sl.dense, d, d, r_xb2, r_xb, t, false)
            ts_add_ln(rl, slab, sl.dense, sl.attn_ln, r_x, r_xb, r_xb2, t, d, eps)
            ts_lin(rl, slab, sl.ffn, d, ff, r_xb2, r_ffn, t)
            _::enc_tts_gelu_tanh_cls(rl, _::rail_seats(rl, fixed_array(r_ffn)), pa_gelu, t * ff)
            _::rail_pt(rl, RailProfRole.tts_act)
            ts_lin(rl, slab, sl.ffn_out, ff, d, r_ffn, r_f2, t, false)
            ts_add_ln(rl, slab, sl.ffn_out, sl.full_ln, r_xb2, r_f2, r_x, t, d, eps)
        }
    }
    ts_readback(rl, r_x, out, t * d)
    ts_served(rl)
    return true
}

let private TS_T_IDS = 0
let private TS_T_A = 1
let private TS_T_B = 2
let private TS_T_COL = 3
let private TS_T_GF = 4
let private TS_T_GB = 5
let private TS_T_Y = 6

def private ts_text_shape_ok(te : St2TextEncoder; t : int64) : bool {
    let cc = te.c
    return false if (t < 1l || cc % 32l != 0l || length(te.cnn_ln) != length(te.cnn) || !tts_lstm_ok(te.lstm, cc, TS_LSTM_MAX_HIDDEN)
        || 2l * te.lstm.fwd.hidden != cc)
    for (cv in te.cnn) {
        return false if (cv.cin != cc || cv.cout != cc || cv.transposed || conv1d_out_len(cv, t) != t)
    }
    return tts_text_ok(te, TS_LSTM_MAX_HIDDEN)
}

def private ts_text_ensure(rl : auto(R)) : bool {
    return (_::rail_tts_lstm_ok(rl) && _::rail_ensure_f32_gemm_b_cls(rl) && ts_stored_ensure(rl) && _::rail_ensure_tower_ln_cls(rl) && _::rail_ensure_tts_row_gather_cls(rl)
        && _::rail_ensure_tts_leaky_cls(rl) && _::rail_ensure_tts_im2col_cls(rl) && _::rail_ensure_tts_lstm_dir_cls(rl) && _::rail_ensure_tts_lstm_dir_q8_cls(rl))
}

//! the widest im2col row the encoder's convs take
def private ts_text_kdim(te : St2TextEncoder) : int64 {
    var kdim = 32l
    for (cv in te.cnn) {
        kdim = max(kdim, round_up(cv.k * cv.cin, 32l))
    }
    return kdim
}

[arch(at = "../ARCHITECTURE_GPU_TOWER_TTS.md#tts-st2-seats")]
def tts_st2_text(var rl : auto(R); te : St2TextEncoder; ids : array<int>; @scratch @exact_size var out : array<float>; var sc : St2Scratch) : bool {
    if (!ts_on(rl)) {
        return ts_decline(rl, "text", TowerDecline.knob)
    }
    if (sc.blk.plan_n == 0l) {
        return ts_decline(rl, "text", TowerDecline.flavor)
    }
    let t = long_length(ids)
    let cc = te.c
    if (!ts_text_shape_ok(te, t)) {
        return ts_decline(rl, "text", TowerDecline.shape)
    }
    for (id in ids) {
        return ts_decline(rl, "text", TowerDecline.shape) if (id < 0 || int64(id + 1) * cc > tts_held(te.emb.a, te.emb.span))
    }
    if (!ts_arm_ensure(rl, "text", $ => ts_text_ensure(rl))) {
        return ts_decline(rl, "text", TowerDecline.device)
    }
    let hd = te.lstm.fwd.hidden
    var fl : TsFloats
    fl[TS_T_IDS] = t
    fl[TS_T_A] = t * cc
    fl[TS_T_B] = t * cc
    fl[TS_T_COL] = t * ts_text_kdim(te)
    fl[TS_T_GF] = t * 4l * hd
    fl[TS_T_GB] = t * 4l * hd
    fl[TS_T_Y] = t * 2l * hd
    if (!ts_text_attach(rl, sc.blk) || !ts_scratch_ready(rl, g_ts_text_scratch, fl, 0l, 0l, t * cc)) {
        return ts_decline(rl, "text", TowerDecline.memory)
    }
    let sl & = g_ts_text_slots
    let sc_ & = g_ts_text_scratch
    let slab & = g_ts_text
    let r_ids = ts_rows(sc_, TS_T_IDS)
    let r_a = ts_rows(sc_, TS_T_A)
    let r_b = ts_rows(sc_, TS_T_B)
    let r_col = ts_rows(sc_, TS_T_COL)
    let r_gf = ts_rows(sc_, TS_T_GF)
    let r_gb = ts_rows(sc_, TS_T_GB)
    let r_y = ts_rows(sc_, TS_T_Y)
    ts_upload(rl, r_ids, ids, t)
    var pa_gather = GkRowGatherArgs(c = uint(cc), rows = uint(t), aoff = uint(sl.emb), boff = 0u, coff = 0u, out_off = 0u, add_b = 0u, add_c = 0u, scale = 1.0)
    let ncnn = length(te.cnn)
    _::rail_tts_shot(rl, "text", int64(ncnn), t) {
        _::enc_tts_row_gather_cls(rl, _::rail_seats(rl, fixed_array(slab.rows, r_ids, slab.rows, slab.rows, r_a)), pa_gather, t * cc)
        _::rail_pt(rl, RailProfRole.tts_rows)
        for (i in range(ncnn)) {
            let src = i % 2 == 0 ? r_a : r_b
            let dst = i % 2 == 0 ? r_b : r_a
            ts_conv(rl, slab, sl.cnn[i], te.cnn[i], t, t, src, r_col, dst)
            ts_ln(rl, slab, sl.ln[i], dst, dst, t, cc, ST2_ADAIN_EPS)
            ts_leaky(rl, dst, t * cc, 0.2)
        }
        let fin = ncnn % 2 == 1 ? r_b : r_a
        ts_bilstm(rl, slab, sl.lstm, te.lstm, t, fin, r_gf, r_gb, r_y)
    }
    var rows & = unsafe(sc.te_a)
    ts_readback(rl, r_y, rows, t * cc)
    unsafe(scratch_resize(out, cc * t))
    transpose_tc(rows, t, cc, out)
    ts_served(rl)
    return true
}

let private TS_JOIN = 0.70710678118654752   //! the AdaIN residual blocks' join: (out + shortcut) / sqrt 2
let private TS_MAX_DE_LSTMS = 8              //! the duration encoder's LSTM stack the seat holds style rows for
let private TS_MAX_RES_BLOCKS = 8            //! ... and a prosody branch's residual blocks

//! o [rows][width] = cat(x [rows][c], the res rows, the f0 and n columns) zero-padded, as `pa` lays them (rs = 0 broadcasts one res row)
def private ts_concat(var rl : auto(R); x, res, f0, nd, o : RailBuf; pa : GkConcatArgs) {
    var pc = pa
    _::enc_tts_concat_cls(rl, _::rail_seats(rl, fixed_array(x, res, f0, nd, o)), pc, int64(pa.rows) * int64(pa.width))
    _::rail_pt(rl, RailProfRole.tts_rows)
}

//! raw[t] = the sigmoid sum of nb logits a row over speed
def private ts_sigsum(var rl : auto(R); logits, out : RailBuf; t, nb : int64; speed : float) {
    var pa = GkSigSumArgs(t = uint(t), nb = uint(nb), ls = uint(nb), speed = speed)
    _::enc_tts_sigsum_cls(rl, _::rail_seats(rl, fixed_array(logits, out)), pa, t)
    _::rail_pt(rl, RailProfRole.tts_rows)
}

//! the column statistics of x [t][c] into the scratch's stats rows, the block partials past the two stats rows
def private ts_colstats(var rl : auto(R); sc_ : TsScratch; x : RailBuf; t, c : int64) {
    let st = sc_.stats
    if (_::rail_tts_colstats(rl, x, st, t, c)) {
        _::rail_pt(rl, RailProfRole.tts_adain)
        return   // the home's own pass wrote the two stats rows
    }
    var pa = TtsStatsArgs(t = uint(t), c = uint(c), nblk = uint(tts_stats_blocks(t)), poff = uint(2l * c))
    _::enc_tts_colpart_sum_cls(rl, _::rail_seats(rl, fixed_array(x, st)), pa, tts_stats_blocks(t))
    _::enc_tts_colreduce_mean_cls(rl, _::rail_seats(rl, fixed_array(st, st)), pa, c)
    _::enc_tts_colpart_sq_cls(rl, _::rail_seats(rl, fixed_array(x, st, st)), pa, tts_stats_blocks(t))
    _::enc_tts_colreduce_sq_cls(rl, _::rail_seats(rl, fixed_array(st, st)), pa, c)
    _::rail_pt(rl, RailProfRole.tts_adain)
}

//! AdaIN, then leaky(0.2) or Snake on the norm slot's alpha row, from x into y (y may be x), the style rows at `hoff`
def private ts_adain(var rl : auto(R); slab : TsSlab; sc_ : TsScratch; x, y : RailBuf; t, c : int64; ns : TtsNormSlot; hoff : int64; snake : bool) {
    ts_colstats(rl, sc_, x, t, c)
    let bufs = fixed_array(x, y, sc_.stats, sc_.aux, slab.rows)
    static_assert(GK_ADAIN_EPS == ST2_ADAIN_EPS, "the AdaIN stamps' eps is the model's")
    var p2 = GkAdainArgs(c = uint(c), t = uint(t), hoff = uint(hoff), gwoff = uint(ns.gwoff), gboff = uint(ns.gboff),
        aoff = uint(ns.aux_off))
    if (snake) {
        _::enc_tts_adain_snake_cls(rl, _::rail_seats(rl, bufs), p2, t * c)
    } else {
        _::enc_tts_adain_leaky_cls(rl, _::rail_seats(rl, bufs), p2, t * c)
    }
    _::rail_pt(rl, RailProfRole.tts_adain)
}

//! the depthwise transposed pool on rows: y [t_out][c] from x [t_in][c] over the slab's w [c][k] at `woff` and b [c] at `boff`
def private ts_pool_dw(var rl : auto(R); slab : TsSlab; cv : TtsConv1d; woff, boff : int64; x, y : RailBuf; t_in, t_out : int64) {
    var pa = GkDwConvRowsArgs(c = uint(cv.cin), k = uint(cv.k), stride = uint(cv.stride), pad_l = uint(cv.pad_l), dil = uint(cv.dilation), t_in = uint(t_in),
        t_out = uint(t_out), woff = uint(woff), boff = uint(boff))
    _::enc_tts_pool_dw_cls(rl, _::rail_seats(rl, fixed_array(x, y, slab.rows)), pa, t_out * cv.cin)
    _::rail_pt(rl, RailProfRole.tts_rows)
}

//! out [rows][c] = src[idx[r]] - the nearest x2 upsample with idx = r / 2
def private ts_gather_rows(var rl : auto(R); src, idx, out : RailBuf; rows, c : int64) {
    var pa = GkRowGatherArgs(c = uint(c), rows = uint(rows), aoff = 0u, boff = 0u, coff = 0u, out_off = 0u, add_b = 0u, add_c = 0u, scale = 1.0)
    _::enc_tts_row_gather_cls(rl, _::rail_seats(rl, fixed_array(src, idx, src, src, out)), pa, rows * c)
    _::rail_pt(rl, RailProfRole.tts_rows)
}

//! o = (a + b) * scale over n elements (o may be a)
def private ts_add_scale(var rl : auto(R); a, b, o : RailBuf; n : int64; scale : float) {
    var pa = GkAddScaleArgs(nelem = uint(n), scale = scale)
    _::enc_tts_add_scale_cls(rl, _::rail_seats(rl, fixed_array(a, b, o)), pa, n)
    _::rail_pt(rl, RailProfRole.tts_rows)
}

def private ts_pred_ensure(rl : auto(R)) : bool {
    return (_::rail_tts_lstm_ok(rl) && _::rail_ensure_f32_gemm_b_cls(rl) && ts_stored_ensure(rl) && _::rail_ensure_tower_ln_cls(rl) && _::rail_ensure_tts_row_gather_cls(rl)
        && _::rail_ensure_tts_im2col_cls(rl) && _::rail_ensure_tts_lstm_dir_cls(rl) && _::rail_ensure_tts_lstm_dir_q8_cls(rl)
        && _::rail_ensure_tts_concat_cls(rl) && _::rail_ensure_tts_sigsum_cls(rl) && _::rail_ensure_tts_colpart_sum_cls(rl) && _::rail_ensure_tts_colpart_sq_cls(rl)
        && _::rail_ensure_tts_colreduce_mean_cls(rl) && _::rail_ensure_tts_colreduce_sq_cls(rl) && _::rail_ensure_tts_adain_leaky_cls(rl)
        && _::rail_ensure_tts_pool_dw_cls(rl) && _::rail_ensure_tts_add_scale_cls(rl))
}

let private TS_DE_BERT = 0
let private TS_DE_DEN = 1
let private TS_DE_X = 2
let private TS_DE_Y = 3
let private TS_DE_GF = 4
let private TS_DE_GB = 5

def private ts_dur_enc_shape_ok(p : St2Predictor; t, sd : int64) : bool {
    let cc = p.bert_encoder.nout
    let width = cc + sd
    return false if (t < 1l || cc % 64l != 0l || p.bert_encoder.nin % 32l != 0l || width % 32l != 0l || empty(p.lstms)
        || length(p.lns) != length(p.lstms) || length(p.lstms) > TS_MAX_DE_LSTMS)
    for (l in p.lstms) {
        return false if (!tts_lstm_ok(l, width, TS_LSTM_MAX_HIDDEN) || 2l * l.fwd.hidden != cc)
    }
    for (ln in p.lns) {
        return false if (!tts_gemm_lin_ok(ln.fc) || ln.fc.nin != sd || ln.fc.nout != 2l * cc)
    }
    return tts_pred_ok(p, TS_LSTM_MAX_HIDDEN)
}

[unused_argument(sc), arch(at = "../ARCHITECTURE_GPU_TOWER_TTS.md#tts-st2-seats")]
def tts_st2_dur_enc(var rl : auto(R); p : St2Predictor; bert : array<float>; t : int64; s : array<float>; @scratch @exact_size var out : array<float>; var sc : St2Scratch) : bool {
    if (!ts_on(rl)) {
        return ts_decline(rl, "dur_enc", TowerDecline.knob)
    }
    if (sc.blk.plan_n == 0l) {
        return ts_decline(rl, "dur_enc", TowerDecline.flavor)
    }
    let cc = p.bert_encoder.nout
    let hidden = p.bert_encoder.nin
    let sd = long_length(s)
    let width = cc + sd
    if (!ts_dur_enc_shape_ok(p, t, sd) || long_length(bert) < t * hidden) {
        return ts_decline(rl, "dur_enc", TowerDecline.shape)
    }
    if (!ts_arm_ensure(rl, "dur_enc", $ => ts_pred_ensure(rl))) {
        return ts_decline(rl, "dur_enc", TowerDecline.device)
    }
    let hd = cc / 2l
    let nl = length(p.lstms)
    var fl : TsFloats
    fl[TS_DE_BERT] = t * hidden
    fl[TS_DE_DEN] = t * cc
    fl[TS_DE_X] = t * width
    fl[TS_DE_Y] = t * cc
    fl[TS_DE_GF] = t * 4l * hd
    fl[TS_DE_GB] = t * 4l * hd
    if (!ts_pred_attach(rl, sc.blk) || !ts_scratch_ready(rl, g_ts_de_scratch, fl, 4l + sd + int64(nl) * (2l * cc + 4l), 0l, t * width)) {
        return ts_decline(rl, "dur_enc", TowerDecline.memory)
    }
    let sl & = g_ts_pred_slots
    let sc_ & = g_ts_de_scratch
    let slab & = g_ts_pred
    let r_bert = ts_rows(sc_, TS_DE_BERT)
    let r_den = ts_rows(sc_, TS_DE_DEN)
    let r_x = ts_rows(sc_, TS_DE_X)
    let r_y = ts_rows(sc_, TS_DE_Y)
    let r_gf = ts_rows(sc_, TS_DE_GF)
    let r_gb = ts_rows(sc_, TS_DE_GB)
    let aux = sc_.aux
    ts_style_begin(s)
    var hoff : int64[TS_MAX_DE_LSTMS]
    for (i in range(nl)) {
        hoff[i] = ts_style_rows(sl.ln_fc[i], cc)
    }
    if (!ts_aux_upload(rl, sc_)) {
        return ts_decline(rl, "dur_enc", TowerDecline.memory)
    }
    ts_upload(rl, r_bert, bert, t * hidden)
    let pa_cat = GkConcatArgs(c = uint(cc), rc = uint(sd), rs = 0u, f0s = 0u, ns = 0u, with_res = 1u, width = uint(width), rows = uint(t))
    _::rail_tts_shot(rl, "dur_enc", int64(nl), t) {
        ts_style_flush(rl, slab, aux)
        ts_lin(rl, slab, sl.bert_enc, hidden, cc, r_bert, r_den, t)
        ts_concat(rl, r_den, aux, aux, aux, r_x, pa_cat)
        for (i in range(nl)) {
            ts_bilstm(rl, slab, sl.de_lstm[i], p.lstms[i], t, r_x, r_gf, r_gb, r_y)
            ts_ln_at(rl, aux, hoff[i], hoff[i] + cc, r_y, r_y, t, cc, ST2_ADAIN_EPS)
            ts_concat(rl, r_y, aux, aux, aux, r_x, pa_cat)
        }
    }
    ts_readback(rl, r_x, out, t * width)
    ts_served(rl)
    return true
}

let private TS_DU_D = 0
let private TS_DU_Y = 1
let private TS_DU_GF = 2
let private TS_DU_GB = 3
let private TS_DU_LOG = 4
let private TS_DU_RAW = 5

[unused_argument(sc), arch(at = "../ARCHITECTURE_GPU_TOWER_TTS.md#tts-st2-seats")]
def tts_st2_durations(var rl : auto(R); p : St2Predictor; d : array<float>; t : int64; speed : float; @scratch @exact_size var raw : array<float>; var sc : St2Scratch) : bool {
    if (!ts_on(rl)) {
        return ts_decline(rl, "durations", TowerDecline.knob)
    }
    if (sc.blk.plan_n == 0l) {
        return ts_decline(rl, "durations", TowerDecline.flavor)
    }
    let width = p.lstm.fwd.nin
    let hd = p.lstm.fwd.hidden
    let nb = p.duration_proj.nout
    if (t < 1l || width % 32l != 0l || !tts_lstm_ok(p.lstm, width, TS_LSTM_MAX_HIDDEN) || p.duration_proj.nin != 2l * hd || nb < 1l || nb > 64l
        || speed <= 0.0 || long_length(d) < t * width || !tts_pred_ok(p, TS_LSTM_MAX_HIDDEN)) {
        return ts_decline(rl, "durations", TowerDecline.shape)
    }
    if (!ts_arm_ensure(rl, "durations", $ => ts_pred_ensure(rl))) {
        return ts_decline(rl, "durations", TowerDecline.device)
    }
    var fl : TsFloats
    fl[TS_DU_D] = t * width
    fl[TS_DU_Y] = t * 2l * hd
    fl[TS_DU_GF] = t * 4l * hd
    fl[TS_DU_GB] = t * 4l * hd
    fl[TS_DU_LOG] = t * nb
    fl[TS_DU_RAW] = t
    if (!ts_pred_attach(rl, sc.blk) || !ts_scratch_ready(rl, g_ts_du_scratch, fl, 0l, 0l, t)) {
        return ts_decline(rl, "durations", TowerDecline.memory)
    }
    let sl & = g_ts_pred_slots
    let sc_ & = g_ts_du_scratch
    let slab & = g_ts_pred
    let r_d = ts_rows(sc_, TS_DU_D)
    let r_y = ts_rows(sc_, TS_DU_Y)
    let r_gf = ts_rows(sc_, TS_DU_GF)
    let r_gb = ts_rows(sc_, TS_DU_GB)
    let r_log = ts_rows(sc_, TS_DU_LOG)
    let r_raw = ts_rows(sc_, TS_DU_RAW)
    ts_upload(rl, r_d, d, t * width)
    _::rail_tts_shot(rl, "durations", 1l, t) {
        ts_bilstm(rl, slab, sl.dur_lstm, p.lstm, t, r_d, r_gf, r_gb, r_y)
        ts_lin(rl, slab, sl.dur_proj, 2l * hd, nb, r_y, r_log, t)
        ts_sigsum(rl, r_log, r_raw, t, nb, speed)
    }
    ts_readback(rl, r_raw, raw, t)
    ts_served(rl)
    return true
}

let private TS_PR_EN = 0     //! the encoder rows [frames][c]
let private TS_PR_Y = 1      //! the shared BiLSTM's rows [frames][2H]
let private TS_PR_GF = 2
let private TS_PR_GB = 3
let private TS_PR_A = 4      //! a branch's ping-pong rows
let private TS_PR_B = 5
let private TS_PR_R = 6      //! a block's normed rows
let private TS_PR_POOL = 7   //! ... its pooled rows
let private TS_PR_C1 = 8     //! ... its first conv's rows
let private TS_PR_SC = 9     //! ... its shortcut rows (the nearest copy)
let private TS_PR_SC2 = 10   //! ... and the learned shortcut's
let private TS_PR_COL = 11   //! the im2col columns
let private TS_PR_IDX = 12   //! the nearest upsample's row map, r / 2
let private TS_PR_F0 = 13    //! the F0 branch's projection [t][1]
let private TS_PR_N = 14     //! ... and the energy branch's

//! a seat's geometry, the shape walks' record (`tts_res_shape` and its kin): the widest row count and width its blocks reach, the widest im2col
//! row, and the aux floats its norms take
struct TsShape {
    t_max : int64
    c_max : int64
    col : int64      //! the widest im2col a conv takes: its output rows x its padded row; a transposed stored conv's lift rows where wider
    kdim : int64     //! the widest padded row among them
    col16 : int64    //! the halves the fast route's widest feed takes: a stored conv's rows, the read slack past them, at its leaf's row width
    groups : int64   //! the schedule map words the stored convs' cm2 dispatches take between them
    aux : int64
    style_dim : int64
}

//! the im2col floats a conv over `rows` output rows takes
def private ts_col_of(c : TtsConv1d; rows : int64) : int64 => rows * ts_conv_kdim(c)

def private ts_conv_kdim(c : TtsConv1d) : int64 => round_up(c.k * c.cin, 32l)

//! the row width a conv's leaf is stored at - its Q8_0 or K-quant matrix's K, a half plane's rows on the 32 lattice; 0 where the conv holds floats
def private ts_conv_stored_k(c : TtsConv1d) : int64 {
    return round_up(conv1d_quant_k(c), 32l) if (conv1d_held_halves(c))
    return c.q8 ? c.q8_k : c.kq_fmt != 0 ? c.kq_k : 0l
}

//! a conv over `rows` output rows noted on the shape: the widest im2col and the widest padded row
def private ts_note_col(var sh : TsShape; c : TtsConv1d; rows : int64) {
    sh.col = max(sh.col, ts_col_of(c, rows))
    sh.kdim = max(sh.kdim, ts_conv_kdim(c))
    let n = ts_conv_stored_k(c)
    return if (n == 0l)
    let fast_gemm_rows = c.transposed ? rows / max(c.stride, 1l) + 1l : rows
    let fd = c.transposed ? c.k * c.cout : c.cout
    sh.col16 = max(sh.col16, (fast_gemm_rows + TS_COL16_SLACK_ROWS) * n)
    sh.groups += ((fd + 127l) / 128l) * ((fast_gemm_rows + 31l) / 32l)
    if (c.transposed) {
        sh.col = max(sh.col, fast_gemm_rows * fd)
    }
}

let private TS_COL16_SLACK_ROWS = 256l   //! the half column slot's rows past a conv's: a cm2 tile loads whole row tiles, the KHR tile whole 16-row blocks

//! the half column slot's floats: the widest im2col as halves and the slack rows past it, or the fast route's widest feed
def private ts_col16_floats(sh : TsShape) : int64 => max(sh.col + TS_COL16_SLACK_ROWS * sh.kdim, sh.col16) / 2l + 1l

//! the shape walks' conv hook: a conv over `rows` output rows noted (its im2col and padded row); a transposed conv serves only as an upsample
def tts_note_conv(var sh : TsShape; cv : TtsConv1d; rows : int64; upsample : bool) : bool {
    return false if (cv.transposed && !upsample)
    ts_note_col(sh, cv, rows)
    return true
}

def tts_note_rows(var sh : TsShape; t, c : int64) {
    sh.t_max = max(sh.t_max, t)
    sh.c_max = max(sh.c_max, c)
}

//! a Snake block's style rows (its norms' fc(style), 16-float aligned) and their fc widths
[unused_argument(t)]
def tts_note_block(var sh : TsShape; b : St2GenResBlock; t, c : int64) : bool {
    for (j in range(length(b.adain1))) {
        return false if (!tts_gemm_lin_ok(b.adain1[j].fc) || b.adain1[j].fc.nout != 2l * c || !tts_gemm_lin_ok(b.adain2[j].fc) || b.adain2[j].fc.nout != 2l * c
            || b.adain1[j].fc.nin != sh.style_dim || b.adain2[j].fc.nin != sh.style_dim)
    }
    sh.aux += long_length(b.convs1) * (4l * c + 16l)
    return true
}

//! an AdaIN block's pool and norms, its norms' fc widths against the style, its rows and style rows
def tts_note_res(var sh : TsShape; b : St2AdainResBlk; t1 : int64) : bool {
    return false if (!tts_res_ok(b) || !tts_gemm_lin_ok(b.norm1.fc) || b.norm1.fc.nout != 2l * b.dim_in_s || !tts_gemm_lin_ok(b.norm2.fc) || b.norm2.fc.nout != 2l * b.dim_out
        || b.norm1.fc.nin != sh.style_dim || b.norm2.fc.nin != sh.style_dim)
    tts_note_rows(sh, t1, max(b.dim_in_s, b.dim_out))
    sh.aux += 2l * (b.dim_in_s + b.dim_out) + 8l
    return true
}

def tts_note_post(var sh : TsShape; post : TtsConv1d; t_post : int64) : bool => tts_note_conv(sh, post, t_post, false)

//! the decode seat's style-row offsets hold TS_MAX_RES_BLOCKS decode blocks
[unused_argument(sh)]
def tts_note_decoder(var sh : TsShape; d : St2Decoder) : bool => length(d.decode) <= TS_MAX_RES_BLOCKS

//! a branch's blocks then its projection on the seat, or -1: the projection's row count
def private ts_branch_shape(var sh : TsShape; blocks : array<St2AdainResBlk>; proj : TtsConv1d; frames, cin : int64) : int64 {
    return -1l if (length(blocks) > TS_MAX_RES_BLOCKS || !tts_conv_ok(proj) || proj.transposed || proj.cout != 1l)
    var t = frames
    var c = cin
    for (b in blocks) {
        return -1l if (b.dim_in_s != c)
        t = tts_res_shape(sh, b, t)
        return -1l if (t < 0l)
        c = b.dim_out
    }
    return -1l if (proj.cin != c || conv1d_out_len(proj, t) != t)
    ts_note_col(sh, proj, t)
    sh.c_max = max(sh.c_max, c)
    return t
}

//! a seat's slots for the residual blocks' rows: the normed rows, the pooled rows, the first conv's rows, the two shortcuts, the im2col columns, the r / 2 map
struct private TsBlkIdx {
    r : int
    pool : int
    c1 : int
    sc : int
    sc2 : int
    col : int
    col16 : int     //! the half columns' slot, -1 where the blocks stay on the f32 tile (the prosody branches)
    idx : int
}

let private TS_PR_BLK = TsBlkIdx(r = TS_PR_R, pool = TS_PR_POOL, c1 = TS_PR_C1, sc = TS_PR_SC, sc2 = TS_PR_SC2, col = TS_PR_COL, col16 = -1, idx = TS_PR_IDX)

//! the CPU adain_res_blk, dispatch for dispatch, from bin [t][dim_in_s] into bout [t1][dim_out]; t1 back
def private ts_res_blk(var rl : auto(R); slab : TsSlab; sc_ : TsScratch; bi : TsBlkIdx; b : St2AdainResBlk; sl : TtsResSlots; hoff1, hoff2 : int64;
                       bin : RailBuf; t : int64; bout : RailBuf) : int64 {
    let r_r = ts_rows(sc_, bi.r)
    let r_pool = ts_rows(sc_, bi.pool)
    let r_c1 = ts_rows(sc_, bi.c1)
    let r_sc = ts_rows(sc_, bi.sc)
    let r_sc2 = ts_rows(sc_, bi.sc2)
    let r_col = ts_rows(sc_, bi.col)
    let half = bi.col16 >= 0
    let r_col16 = half ? ts_rows(sc_, bi.col16) : RailBuf()
    ts_adain(rl, slab, sc_, bin, r_r, t, b.dim_in_s, sl.n1, hoff1, false)
    var t1 = t
    if (b.upsample) {
        t1 = st2_blk_rows(b, t)
        ts_pool_dw(rl, slab, b.pool, sl.pool_w, sl.pool_b, r_r, r_pool, t, t1)
        ts_conv_at(rl, slab, sl.c1, b.conv1, t1, t1, r_pool, r_col, r_col16, r_c1, half)
    } else {
        ts_conv_at(rl, slab, sl.c1, b.conv1, t1, t1, r_r, r_col, r_col16, r_c1, half)
    }
    ts_adain(rl, slab, sc_, r_c1, r_c1, t1, b.dim_out, sl.n2, hoff2, false)
    ts_conv_at(rl, slab, sl.c2, b.conv2, t1, t1, r_c1, r_col, r_col16, bout, half)
    let n = t1 * b.dim_out
    if (b.upsample) {
        ts_gather_rows(rl, bin, ts_rows(sc_, bi.idx), r_sc, t1, b.dim_in_s)
        if (b.learned_sc) {
            ts_conv_at(rl, slab, sl.c1x1, b.conv1x1, t1, t1, r_sc, r_col, r_col16, r_sc2, half)
            ts_add_scale(rl, bout, r_sc2, bout, n, TS_JOIN)
        } else {
            ts_add_scale(rl, bout, r_sc, bout, n, TS_JOIN)
        }
    } elif (b.learned_sc) {
        ts_conv_at(rl, slab, sl.c1x1, b.conv1x1, t1, t1, bin, r_col, r_col16, r_sc2, half)
        ts_add_scale(rl, bout, r_sc2, bout, n, TS_JOIN)
    } else {
        ts_add_scale(rl, bout, bin, bout, n, TS_JOIN)
    }
    return t1
}

//! a prosody branch's blocks, pinging between the A and B rows, then its single-channel projection into `out` [t][1]; t back
def private ts_branch(var rl : auto(R); slab : TsSlab; sc_ : TsScratch; blocks : array<St2AdainResBlk>; slots : array<TtsResSlots>;
                      proj : TtsConv1d; pslot : TtsConvSlot; hoffs : int64[TS_MAX_RES_BLOCKS * 4]; hbase : int; x : RailBuf; frames : int64; out : RailBuf) : int64 {
    let r_a = ts_rows(sc_, TS_PR_A)
    let r_b = ts_rows(sc_, TS_PR_B)
    var t = frames
    let nb = length(blocks)
    for (i in range(nb)) {
        let src = i == 0 ? x : (i % 2 == 1 ? r_a : r_b)
        let dst = i % 2 == 0 ? r_a : r_b
        t = ts_res_blk(rl, slab, sc_, TS_PR_BLK, blocks[i], slots[i], hoffs[hbase + 2 * i], hoffs[hbase + 2 * i + 1], src, t, dst)
    }
    let fin = nb == 0 ? x : (nb % 2 == 1 ? r_a : r_b)
    ts_conv(rl, slab, pslot, proj, t, t, fin, ts_rows(sc_, TS_PR_COL), out)
    return t
}

//! the prosody seat's slots for this call's geometry
def private ts_pr_floats(sh : TsShape; cc, hd, frames : int64) : TsFloats {
    let cm = sh.c_max
    let tm = sh.t_max
    var fl : TsFloats
    fl[TS_PR_EN] = frames * cc
    fl[TS_PR_Y] = frames * 2l * hd
    fl[TS_PR_GF] = frames * 4l * hd
    fl[TS_PR_GB] = frames * 4l * hd
    for (i in range(TS_PR_A, TS_PR_SC2 + 1)) {
        fl[i] = tm * cm
    }
    fl[TS_PR_COL] = sh.col
    fl[TS_PR_IDX] = tm
    fl[TS_PR_F0] = tm
    fl[TS_PR_N] = tm
    return fl
}

//! the style rows on the device: per branch (the energy branch's from `nbase`), per block, its two norms' fc(style); false where the aux rows ran short
def private ts_pr_style_rows(var rl : auto(R); p : St2Predictor; sl : TtsPredSlab; s : array<float>; var hoffs : int64[TS_MAX_RES_BLOCKS * 4]; nbase : int; sc_ : TsScratch) : bool {
    ts_style_begin(s)
    for (i in range(length(p.f0))) {
        ts_res_style_rows(hoffs, 2 * i, p.f0[i], sl.f0b[i])
    }
    for (i in range(length(p.n))) {
        ts_res_style_rows(hoffs, nbase + 2 * i, p.n[i], sl.nb[i])
    }
    return ts_aux_upload(rl, sc_)
}

//! an AdaIN residual block's two norms' fc(style) staged, their offsets into `hoffs[at]` and `hoffs[at + 1]`
def private ts_res_style_rows(var hoffs : auto(HA); at : int; b : St2AdainResBlk; rs : TtsResSlots) {
    hoffs[at] = ts_style_rows(rs.n1.fc, b.dim_in_s)
    hoffs[at + 1] = ts_style_rows(rs.n2.fc, b.dim_out)
}

[arch(at = "../ARCHITECTURE_GPU_TOWER_TTS.md#tts-st2-seats")]
def tts_st2_prosody(var rl : auto(R); p : St2Predictor; en : array<float>; cc, frames : int64; s : array<float>;
                    @scratch @exact_size var f0 : array<float>; @scratch @exact_size var n : array<float>; var f2 : int64&; var sc : St2Scratch) : bool {
    if (!ts_on(rl)) {
        return ts_decline(rl, "prosody", TowerDecline.knob)
    }
    if (sc.blk.plan_n == 0l) {
        return ts_decline(rl, "prosody", TowerDecline.flavor)
    }
    let hd = p.shared_lstm.fwd.hidden
    var sh = TsShape(t_max = frames, c_max = max(cc, 2l * hd), col = 32l, aux = 4l, style_dim = long_length(s))
    if (frames < 1l || cc % 32l != 0l || !tts_lstm_ok(p.shared_lstm, cc, TS_LSTM_MAX_HIDDEN) || empty(p.f0) || long_length(en) < cc * frames
        || !tts_pred_ok(p, TS_LSTM_MAX_HIDDEN)) {
        return ts_decline(rl, "prosody", TowerDecline.shape)
    }
    let tf = ts_branch_shape(sh, p.f0, p.f0_proj, frames, 2l * hd)
    let tn = ts_branch_shape(sh, p.n, p.n_proj, frames, 2l * hd)
    if (tf < 0l || tn < 0l || tf != tn) {
        return ts_decline(rl, "prosody", TowerDecline.shape)
    }
    if (!ts_arm_ensure(rl, "prosody", $ => ts_pred_ensure(rl))) {
        return ts_decline(rl, "prosody", TowerDecline.device)
    }
    let fl = ts_pr_floats(sh, cc, hd, frames)
    if (!ts_pred_attach(rl, sc.blk) || !ts_scratch_ready(rl, g_ts_pr_scratch, fl, sh.aux + ts_style_floats(s), tts_stats_floats(sh.t_max, sh.c_max), max(tf, tn))) {
        return ts_decline(rl, "prosody", TowerDecline.memory)
    }
    let sl & = g_ts_pred_slots
    let sc_ & = g_ts_pr_scratch
    let slab & = g_ts_pred
    let r_en = ts_rows(sc_, TS_PR_EN)
    let r_y = ts_rows(sc_, TS_PR_Y)
    let r_gf = ts_rows(sc_, TS_PR_GF)
    let r_gb = ts_rows(sc_, TS_PR_GB)
    let r_idx = ts_rows(sc_, TS_PR_IDX)
    let r_f0 = ts_rows(sc_, TS_PR_F0)
    let r_n = ts_rows(sc_, TS_PR_N)
    var hoffs : int64[TS_MAX_RES_BLOCKS * 4]
    let nbase = TS_MAX_RES_BLOCKS * 2
    if (!ts_pr_style_rows(rl, p, sl, s, hoffs, nbase, sc_)) {
        return ts_decline(rl, "prosody", TowerDecline.memory)
    }
    ts_idx_upload(rl, r_idx, sh.t_max)
    var en_tm & = unsafe(sc.pr_en_tm)
    transpose_ct(en, cc, frames, en_tm)
    ts_upload(rl, r_en, en_tm, frames * cc)
    _::rail_tts_shot(rl, "prosody", int64(length(p.f0) + length(p.n)), frames) {
        ts_style_flush(rl, slab, sc_.aux)
        ts_bilstm(rl, slab, sl.sh_lstm, p.shared_lstm, frames, r_en, r_gf, r_gb, r_y)
        ts_branch(rl, slab, sc_, p.f0, sl.f0b, p.f0_proj, sl.f0_proj, hoffs, 0, r_y, frames, r_f0)
        ts_branch(rl, slab, sc_, p.n, sl.nb, p.n_proj, sl.n_proj, hoffs, nbase, r_y, frames, r_n)
    }
    ts_readback(rl, r_f0, f0, tf)
    ts_readback(rl, r_n, n, tn)
    f2 = tf
    ts_served(rl)
    return true
}

var private g_ts_dec : TsSlab
var private g_ts_dec_slots : TtsDecSlab
var private g_ts_dec_scratch : TsScratch
var private @scratch g_ts_hoffs : array<int64>   //! the style rows' offsets, two a norm site, per block from `g_ts_hbase`
var private @scratch g_ts_hbase : array<int64>

let private TS_MAX_STAGES = 8l   //! the generator stages the seat holds slots for

def private ts_dec_attach(rl : auto(R); blk : TtsDevBlock) : bool
    => ts_slab_attach(rl, g_ts_dec, g_ts_dec_slots, blk, "st2.dec", "decoder slab")

let private TS_D_X = 0        //! the stream rows the decode blocks and the generator ping-pong with BO
let private TS_D_ASR = 1      //! the aligned text rows [frames][text_c]
let private TS_D_RES = 2      //! ... through asr_res
let private TS_D_F0R = 3      //! F0 and energy [f2][1]
let private TS_D_NR = 4
let private TS_D_F0D = 5      //! ... through their front convs [frames][1]
let private TS_D_ND = 6
let private TS_D_CAT = 7      //! the concat rows a decode block reads
let private TS_D_NU = 8       //! the source: the initial phases [nh]
let private TS_D_NOISE = 9    //! ... the noise rows [samples][nh]
let private TS_D_LOW = 10     //! ... the phase increments and the phase per frame [nh][nlow]
let private TS_D_LOWC = 11
let private TS_D_HAR = 12     //! ... the mixed signal [samples]
let private TS_D_SPEC = 13    //! ... its spectrum rows [spec_frames][2 bins]
let private TS_D_WAVE = 14    //! the waveform out
let private TS_D_POST = 15    //! conv_post's rows [t_post][cout]
let private TS_D_BY = 16      //! the generator's rows: a stage's sum, the source rows, a block's out, two block temporaries, the ping-pong twin of X
let private TS_D_BSRC = 17
let private TS_D_BSRES = 18
let private TS_D_BXT = 19
let private TS_D_BY1 = 20
let private TS_D_BO = 21
let private TS_D_R = 22       //! the residual blocks' rows
let private TS_D_POOL = 23
let private TS_D_C1 = 24
let private TS_D_SC = 25
let private TS_D_SC2 = 26
let private TS_D_COL = 27
let private TS_D_IDX = 28
let private TS_D_COL16 = 29   //! the im2col columns as halves, the f16 tile's feed

let private TS_D_BLK = TsBlkIdx(r = TS_D_R, pool = TS_D_POOL, c1 = TS_D_C1, sc = TS_D_SC, sc2 = TS_D_SC2, col = TS_D_COL, col16 = TS_D_COL16, idx = TS_D_IDX)

//! the decode chain's geometry (`tts_decode_shape`): the blocks' widest rows, the generator's stages, the source and the inverse STFT
struct TsDecShape : TsShape {
    dec_rows : int64
    dec_width : int64
    t_last : int64      //! the generator's last stage: its rows and width
    c_last : int64
    t_post : int64
    ss : TtsSourceShape
    n_out : int64
}

//! the generator's slots for this call's geometry: the stream rows, conv_post's rows, the block rows and the columns
def private ts_gen_floats(d : St2Decoder; sh : TsDecShape) : TsFloats {
    let rows = sh.t_max * sh.c_max
    var fl : TsFloats
    fl[TS_D_X] = rows
    fl[TS_D_POST] = sh.t_post * d.conv_post.cout
    for (i in range(TS_D_BY, TS_D_SC2 + 1)) {
        fl[i] = rows
    }
    fl[TS_D_COL] = sh.col
    fl[TS_D_COL16] = ts_col16_floats(sh)
    return fl
}

//! the decode seat's slots for this call's geometry: the generator's, then the front, the concat rows, the source and the wave
def private ts_dec_floats(d : St2Decoder; sh : TsDecShape; nh, text_c, frames, f2 : int64) : TsFloats {
    var fl = ts_gen_floats(d, sh)
    fl[TS_D_ASR] = frames * text_c
    fl[TS_D_RES] = frames * d.asr_res.cout
    fl[TS_D_F0R] = f2
    fl[TS_D_NR] = f2
    fl[TS_D_F0D] = frames
    fl[TS_D_ND] = frames
    fl[TS_D_CAT] = fl[TS_D_X]
    fl[TS_D_NU] = nh
    fl[TS_D_NOISE] = sh.ss.samples * nh
    fl[TS_D_LOW] = nh * sh.ss.nlow
    fl[TS_D_LOWC] = nh * sh.ss.nlow
    fl[TS_D_HAR] = sh.ss.samples
    fl[TS_D_SPEC] = sh.ss.spec_frames * 2l * d.bins
    fl[TS_D_WAVE] = sh.n_out
    fl[TS_D_IDX] = sh.t_max
    return fl
}

//! the decode blocks' style rows (two a block into `dhoff`), then the generator's (`ts_gen_style_rows`); false where the aux rows ran short
def private ts_dec_style_rows(var rl : auto(R); d : St2Decoder; sl : TtsDecSlab; s : array<float>; var dhoff : int64[2 + 2 * TS_MAX_RES_BLOCKS]; sc_ : TsScratch) : bool {
    ts_style_begin(s)
    ts_res_style_rows(dhoff, 0, d.encode, sl.encode)
    for (i in range(length(d.decode))) {
        ts_res_style_rows(dhoff, 2 + 2 * i, d.decode[i], sl.decode[i])
    }
    ts_gen_style_rows(d, sl)
    return ts_aux_upload(rl, sc_)
}

//! the generator's blocks' style rows
def private ts_gen_style_rows(d : St2Decoder; sl : TtsDecSlab) {
    let nblk = long_length(d.noise_res) + long_length(d.resblocks)
    g_ts_hbase |> ensure_length(nblk)
    var n = 0l
    for (b in d.noise_res) {
        n += 2l * long_length(b.convs1)
    }
    for (b in d.resblocks) {
        n += 2l * long_length(b.convs1)
    }
    g_ts_hoffs |> ensure_length(n)
    var at = 0l
    var bi = 0l
    for (b in d.noise_res) {
        g_ts_hbase[bi] = at
        ts_block_style_rows(b, sl.blocks[bi], at)
        bi++
    }
    for (b in d.resblocks) {
        g_ts_hbase[bi] = at
        ts_block_style_rows(b, sl.blocks[bi], at)
        bi++
    }
}

def private ts_block_style_rows(b : St2GenResBlock; bs : TtsBlockSlots; var at : int64&) {
    for (j in range(length(b.convs1))) {
        g_ts_hoffs[at] = ts_style_rows(bs.n1[j].fc, b.c)
        g_ts_hoffs[at + 1l] = ts_style_rows(bs.n2[j].fc, b.c)
        at += 2l
    }
}

//! o += scale . y, or o = scale . y under `zero`
def private ts_axpy(var rl : auto(R); o, y : RailBuf; n : int64; scale : float; zero : bool) {
    var pa = GkAxpyArgs(nelem = uint(n), scale = scale, zero = zero ? 1u : 0u)
    _::enc_tts_axpy_cls(rl, _::rail_seats(rl, fixed_array(o, y)), pa, n)
    _::rail_pt(rl, RailProfRole.tts_rows)
}

//! a Snake residual block from bin into bout, the CPU `gen_res_block` dispatch for dispatch; `bi` the block's index in the slab's block order
def private ts_gen_res_block(var rl : auto(R); slab : TsSlab; sc_ : TsScratch; b : St2GenResBlock; sl : TtsBlockSlots; bi : int64;
                             bxt, by1 : RailBuf; bin, bout : RailBuf; t : int64) {
    let n = t * b.c
    let r_col = ts_rows(sc_, TS_D_COL)
    let r_col16 = ts_rows(sc_, TS_D_COL16)
    let hb = g_ts_hbase[bi]
    for (j in range(length(b.convs1))) {
        let src = j == 0 ? bin : bout
        ts_adain(rl, slab, sc_, src, bxt, t, b.c, sl.n1[j], g_ts_hoffs[hb + int64(2 * j)], true)
        ts_conv_at(rl, slab, sl.c1[j], b.convs1[j], t, t, bxt, r_col, r_col16, by1, true)
        ts_adain(rl, slab, sc_, by1, by1, t, b.c, sl.n2[j], g_ts_hoffs[hb + int64(2 * j + 1)], true)
        ts_conv_at(rl, slab, sl.c2[j], b.convs2[j], t, t, by1, r_col, r_col16, bxt, true)
        ts_add_scale(rl, src, bxt, bout, n, 1.0)
    }
}

//! the CPU generator_stage, dispatch for dispatch, from bx_in into bout: tt and cc carry the stream's rows and width across the stages
def private ts_stage(var rl : auto(R); slab : TsSlab; sc_ : TsScratch; d : St2Decoder; i : int64; bspec : RailBuf; bx_in, bout : RailBuf;
                     var tt, cc : int64&; har_frames : int64) {
    let sl & = g_ts_dec_slots
    let r_by = ts_rows(sc_, TS_D_BY)
    let r_bsrc = ts_rows(sc_, TS_D_BSRC)
    let r_bsres = ts_rows(sc_, TS_D_BSRES)
    let r_bxt = ts_rows(sc_, TS_D_BXT)
    let r_by1 = ts_rows(sc_, TS_D_BY1)
    let r_col = ts_rows(sc_, TS_D_COL)
    let r_col16 = ts_rows(sc_, TS_D_COL16)
    ts_leaky(rl, bx_in, tt * cc, 0.1)
    let nst = long_length(d.ups)
    let last = i == nst - 1l
    let t2 = conv1d_out_len(d.ups[i], tt) + st2_stage_pad(d, i)
    let c2 = d.ups[i].cout
    let src_t = conv1d_out_len(d.noise_convs[i], har_frames)
    ts_conv_at(rl, slab, sl.noise[i], d.noise_convs[i], har_frames, src_t, bspec, r_col, r_col16, r_bsrc, true)
    ts_gen_res_block(rl, slab, sc_, d.noise_res[i], sl.blocks[i], i, r_bxt, r_by1, r_bsrc, r_bsres, src_t)
    ts_conv_at(rl, slab, sl.ups[i], d.ups[i], tt, last ? t2 - 1l : t2, bx_in, r_col, r_col16, last ? r_bxt : r_by, true)
    let n = t2 * c2
    if (last) {
        var shift = GkPkRowsArgs(rows = uint(t2 - 1l), width = uint(c2), ss = uint(c2), sc = 0u, sr0 = 0u, ds = uint(c2), dc = 0u, dr0 = 1u)
        _::enc_tts_pk_rows_cls(rl, _::rail_seats(rl, fixed_array(r_bxt, r_by)), shift, n - c2)
        var head = GkPkRowsArgs(rows = 1u, width = uint(c2), ss = uint(c2), sc = 0u, sr0 = 1u, ds = uint(c2), dc = 0u, dr0 = 0u)
        _::enc_tts_pk_rows_cls(rl, _::rail_seats(rl, fixed_array(r_bxt, r_by)), head, c2)
        _::rail_pt(rl, RailProfRole.tts_rows)
    }
    ts_add_scale(rl, r_by, r_bsres, r_by, n, 1.0)
    let nk = d.n_kernels
    let nnoise = long_length(d.noise_res)
    for (j in range64(nk)) {
        ts_gen_res_block(rl, slab, sc_, d.resblocks[i * nk + j], sl.blocks[nnoise + i * nk + j], nnoise + i * nk + j, r_bxt, r_by1, r_by, r_bsres, t2)
        ts_axpy(rl, bout, r_bsres, n, 1.0 / float(nk), j == 0l)
    }
    tt = t2
    cc = c2
}

//! the generator from the rows in X through conv_post into POST [t_post][cout]
def private ts_generator_chain(var rl : auto(R); slab : TsSlab; sc_ : TsScratch; d : St2Decoder; cx, t, har_frames : int64) {
    let r_x = ts_rows(sc_, TS_D_X)
    let r_bo = ts_rows(sc_, TS_D_BO)
    let r_spec = ts_rows(sc_, TS_D_SPEC)
    let nst = long_length(d.ups)
    var tt = t
    var cc = cx
    for (i in range64(nst)) {
        if (i % 2l == 0l) {
            ts_stage(rl, slab, sc_, d, i, r_spec, r_x, r_bo, tt, cc, har_frames)
        } else {
            ts_stage(rl, slab, sc_, d, i, r_spec, r_bo, r_x, tt, cc, har_frames)
        }
    }
    let fin = nst % 2l == 1l ? r_bo : r_x
    ts_leaky(rl, fin, tt * cc, 0.01)
    ts_conv_at(rl, slab, g_ts_dec_slots.post, d.conv_post, tt, conv1d_out_len(d.conv_post, tt), fin, ts_rows(sc_, TS_D_COL), ts_rows(sc_, TS_D_COL16),
        ts_rows(sc_, TS_D_POST), true)
}

//! the source: the noise drawn unless captured, the phases on the law's stamps, the mixed signal, its spectrum on the pad law's stamp
def private ts_source_chain(var rl : auto(R); slab : TsSlab; sc_ : TsScratch; d : St2Decoder; cfg : SineSourceCfg; sh : TsDecShape;
                            captured : bool; seed : uint64) {
    let sl & = g_ts_dec_slots
    let nh = cfg.n_harm
    let r_f0 = ts_rows(sc_, TS_D_F0R)
    let r_nu = ts_rows(sc_, TS_D_NU)
    let r_noise = ts_rows(sc_, TS_D_NOISE)
    let r_low = ts_rows(sc_, TS_D_LOW)
    let r_lowc = ts_rows(sc_, TS_D_LOWC)
    let r_har = ts_rows(sc_, TS_D_HAR)
    let r_spec = ts_rows(sc_, TS_D_SPEC)
    var ka = GkSourceArgs(n = uint(sh.ss.samples), nlow = uint(sh.ss.nlow), nh = uint(nh), up = uint(cfg.upsample), sr = cfg.sample_rate, sine_amp = cfg.sine_amp,
        noise_std = cfg.noise_std, voiced_thr = cfg.voiced_thr, seed = uint(seed ^ (seed >> 32ul)), woff = uint(sl.src_w), boff = uint(sl.src_b))
    if (!captured) {
        _::enc_tts_src_noise_cls(rl, _::rail_seats(rl, fixed_array(r_noise)), ka, sh.ss.samples * nh)
    }
    let low_bufs = fixed_array(r_f0, r_nu, r_low)
    let cum_bufs = fixed_array(r_low, r_lowc)
    let sin_bufs = fixed_array(r_f0, r_lowc, r_noise, slab.rows, r_har)
    if (cfg.torch_math) {
        _::enc_tts_src_low_torch_cls(rl, _::rail_seats(rl, low_bufs), ka, nh * sh.ss.nlow)
        _::enc_tts_src_cumsum_torch_cls(rl, _::rail_seats(rl, cum_bufs), ka, 1l)
        _::enc_tts_src_sines_torch_cls(rl, _::rail_seats(rl, sin_bufs), ka, sh.ss.samples)
        _::rail_pt(rl, RailProfRole.tts_source)
    } else {
        _::enc_tts_src_low_onnx_cls(rl, _::rail_seats(rl, low_bufs), ka, nh * sh.ss.nlow)
        _::enc_tts_src_cumsum_onnx_cls(rl, _::rail_seats(rl, cum_bufs), ka, 1l)
        _::enc_tts_src_sines_onnx_cls(rl, _::rail_seats(rl, sin_bufs), ka, sh.ss.samples)
        _::rail_pt(rl, RailProfRole.tts_source)
    }
    let re & = unsafe(d.stft_fwd_re)
    var ks = GkStftArgs(n = uint(sh.ss.samples), frames = uint(sh.ss.spec_frames), bins = uint(d.bins), k = uint(re.k), hop = uint(re.stride), pad = uint(ST2_STFT_PAD),
        eps = d.stft_eps, reoff = uint(sl.stft_re), imoff = uint(sl.stft_im))
    let st_bufs = fixed_array(r_har, slab.rows, r_spec)
    if (d.stft_pad_reflect) {
        _::enc_tts_stft_reflect_cls(rl, _::rail_seats(rl, st_bufs), ks, sh.ss.spec_frames * d.bins)
    } else {
        _::enc_tts_stft_edge_cls(rl, _::rail_seats(rl, st_bufs), ks, sh.ss.spec_frames * d.bins)
    }
    _::rail_pt(rl, RailProfRole.tts_stft)
}

//! the encode block, then each decode block (re-concatenated where its width asks); the stream lands in X
def private ts_decoder_chain(var rl : auto(R); slab : TsSlab; sc_ : TsScratch; d : St2Decoder; dhoff : int64[2 + 2 * TS_MAX_RES_BLOCKS];
                             text_c, frames : int64) {
    let sl & = g_ts_dec_slots
    let r_x = ts_rows(sc_, TS_D_X)
    let r_bo = ts_rows(sc_, TS_D_BO)
    let r_asr = ts_rows(sc_, TS_D_ASR)
    let r_res = ts_rows(sc_, TS_D_RES)
    let r_f0d = ts_rows(sc_, TS_D_F0D)
    let r_nd = ts_rows(sc_, TS_D_ND)
    let r_cat = ts_rows(sc_, TS_D_CAT)
    let rc = uint(d.asr_res.cout)
    let kc = GkConcatArgs(c = uint(text_c), rc = rc, rs = rc, f0s = 1u, ns = 1u, with_res = 0u, width = uint(d.encode.dim_in_s), rows = uint(frames))
    ts_concat(rl, r_asr, r_res, r_f0d, r_nd, r_cat, kc)
    var tt = ts_res_blk(rl, slab, sc_, TS_D_BLK, d.encode, sl.encode, dhoff[0], dhoff[1], r_cat, frames, r_x)
    var cwr = d.encode.dim_out
    var in_x = true
    for (i in range(length(d.decode))) {
        let blk & = unsafe(d.decode[i])
        let src = in_x ? r_x : r_bo
        let dst = in_x ? r_bo : r_x
        if (blk.dim_in != cwr) {
            let kc2 = GkConcatArgs(c = uint(cwr), rc = rc, rs = rc, f0s = 1u, ns = 1u, with_res = 1u, width = uint(blk.dim_in_s), rows = uint(tt))
            ts_concat(rl, src, r_res, r_f0d, r_nd, r_cat, kc2)
            tt = ts_res_blk(rl, slab, sc_, TS_D_BLK, blk, sl.decode[i], dhoff[2 + 2 * i], dhoff[3 + 2 * i], r_cat, tt, dst)
        } else {
            tt = ts_res_blk(rl, slab, sc_, TS_D_BLK, blk, sl.decode[i], dhoff[2 + 2 * i], dhoff[3 + 2 * i], src, tt, dst)
        }
        in_x = !in_x
        cwr = blk.dim_out
    }
    if (!in_x) {   //! the generator ping-pongs X and BO from X: the stream lands in X first
        ts_add_scale(rl, r_bo, r_bo, r_x, tt * cwr, 0.5)
    }
}

def private ts_dec_ensure(rl : auto(R)) : bool {
    return (ts_pred_ensure(rl) && (!_::rail_tts_f16_on(rl) || (_::rail_ensure_tts_im2col16_cls(rl) && _::rail_tts_f16_ensure(rl))) && _::rail_ensure_tower_bias_act_cls(rl)
        && _::rail_ensure_tts_leaky_cls(rl) && _::rail_ensure_tts_adain_snake_cls(rl) && _::rail_ensure_tts_axpy_cls(rl) && _::rail_ensure_tts_pk_rows_cls(rl) && _::rail_ensure_tts_src_noise_cls(rl)
        && _::rail_ensure_tts_src_low_torch_cls(rl) && _::rail_ensure_tts_src_low_onnx_cls(rl) && _::rail_ensure_tts_src_cumsum_torch_cls(rl) && _::rail_ensure_tts_src_cumsum_onnx_cls(rl)
        && _::rail_ensure_tts_src_sines_torch_cls(rl) && _::rail_ensure_tts_src_sines_onnx_cls(rl) && _::rail_ensure_tts_stft_reflect_cls(rl) && _::rail_ensure_tts_stft_edge_cls(rl)
        && _::rail_ensure_tts_istft_cls(rl) && _::rail_ensure_tts_fold_cls(rl))
}

//! the r / 2 row map for the nearest upsamples, `rows` of it
def private ts_idx_upload(rl : auto(R); r_idx : RailBuf; rows : int64) {
    g_ts_idx_host |> ensure_length(rows)
    for (i in range64(rows)) {
        g_ts_idx_host[i] = int(i / 2l)
    }
    ts_upload(rl, r_idx, g_ts_idx_host, rows)
}

[arch(at = "../ARCHITECTURE_GPU_TOWER_TTS.md#tts-st2-seats")]
def tts_st2_decode(var rl : auto(R); d : St2Decoder; text_c : int64; source : SineSourceCfg; asr : array<float>; frames : int64; f0, nrg : array<float>; f2 : int64;
                   s_ref : array<float>; noise : TtsNoise; seed : uint64; @scratch @exact_size var wave : array<float>; var sc : St2Scratch) : bool {
    if (!ts_on(rl)) {
        return ts_decline(rl, "decode", TowerDecline.knob)
    }
    if (sc.blk.plan_n == 0l) {
        return ts_decline(rl, "decode", TowerDecline.flavor)
    }
    var sh = TsDecShape(col = 32l, aux = 8l, style_dim = long_length(s_ref))
    if (!tts_decode_shape(d, text_c, source, frames, f2, noise, TS_MAX_STAGES, sh) || long_length(asr) < text_c * frames
        || long_length(f0) < f2 || long_length(nrg) < f2) {
        return ts_decline(rl, "decode", TowerDecline.shape)
    }
    if (!ts_arm_ensure(rl, "decode", $ => ts_dec_ensure(rl))) {
        return ts_decline(rl, "decode", TowerDecline.device)
    }
    let nh = source.n_harm
    let fl = ts_dec_floats(d, sh, nh, text_c, frames, f2)
    if (!ts_dec_attach(rl, sc.blk) || !ts_scratch_ready(rl, g_ts_dec_scratch, fl, sh.aux + ts_style_floats(s_ref), tts_stats_floats(sh.t_max, sh.c_max), sh.n_out)) {
        return ts_decline(rl, "decode", TowerDecline.memory)
    }
    ts_sched_begin(rl, sh.groups)
    let sl & = g_ts_dec_slots
    let sc_ & = g_ts_dec_scratch
    let slab & = g_ts_dec
    var dhoff : int64[2 + 2 * TS_MAX_RES_BLOCKS]
    if (!ts_dec_style_rows(rl, d, sl, s_ref, dhoff, sc_)) {
        return ts_decline(rl, "decode", TowerDecline.memory)
    }
    let r_asr = ts_rows(sc_, TS_D_ASR)
    let r_f0 = ts_rows(sc_, TS_D_F0R)
    let r_nr = ts_rows(sc_, TS_D_NR)
    let r_wave = ts_rows(sc_, TS_D_WAVE)
    var asr_tm & = unsafe(sc.dec_asr_tm)
    transpose_ct(asr, text_c, frames, asr_tm)
    ts_upload(rl, r_asr, asr_tm, frames * text_c)
    ts_upload(rl, r_f0, f0, f2)
    ts_upload(rl, r_nr, nrg, f2)
    ts_upload(rl, ts_rows(sc_, TS_D_NU), noise.uniform, nh)
    if (noise.captured) {
        ts_upload(rl, ts_rows(sc_, TS_D_NOISE), noise.normal, sh.ss.samples * nh)
    }
    ts_idx_upload(rl, ts_rows(sc_, TS_D_IDX), sh.t_max)
    let bwd & = unsafe(d.stft_bwd_re)
    _::rail_tts_shot(rl, "decode", long_length(d.ups), frames) {
        ts_style_flush(rl, slab, sc_.aux)
        let r_col = ts_rows(sc_, TS_D_COL)
        let r_col16 = ts_rows(sc_, TS_D_COL16)
        ts_conv(rl, slab, sl.f0c, d.f0_conv, f2, frames, r_f0, r_col, ts_rows(sc_, TS_D_F0D))
        ts_conv(rl, slab, sl.nc, d.n_conv, f2, frames, r_nr, r_col, ts_rows(sc_, TS_D_ND))
        ts_conv_at(rl, slab, sl.asr_res, d.asr_res, frames, frames, r_asr, r_col, r_col16, ts_rows(sc_, TS_D_RES), true)
        ts_decoder_chain(rl, slab, sc_, d, dhoff, text_c, frames)
        ts_source_chain(rl, slab, sc_, d, source, sh, noise.captured, seed)
        ts_generator_chain(rl, slab, sc_, d, sh.dec_width, sh.dec_rows, sh.ss.spec_frames)
        let r_post = ts_rows(sc_, TS_D_POST)
        var ki = GkIstftArgs(tt = uint(sh.t_post), stride = uint(d.conv_post.cout), bins = uint(d.bins), k = uint(bwd.k), hop = uint(bwd.stride),
            pad = uint(ST2_STFT_PAD), envelope = d.istft_envelope ? 1u : 0u, n = uint(sh.n_out), reoff = uint(sl.istft_re), imoff = uint(sl.istft_im),
            wnoff = uint(sl.window))
        _::enc_tts_istft_cls(rl, _::rail_seats(rl, fixed_array(r_post, slab.rows, r_wave)), ki, sh.n_out)
        _::rail_pt(rl, RailProfRole.tts_stft)
    }
    ts_readback(rl, r_wave, wave, sh.n_out)
    ts_served(rl)
    return true
}

let private TS_PK_ATTN_HS = int64(GK_PK_ATTN_HS)             //! the widest head the attention row takes
let private TS_PK_ATTN_MAX_KEYS = int64(GK_PK_ATTN_MAX_KEYS) //! the scores the attention row stages
let private TS_PK_GEMV_MAX_NIN = int64(GK_PK_GEMV_MAX_NIN)   //! the x the row GEMV stages
let private TS_PK_CODEC_MAX_FRAMES = 4096l   //! the positions the codec slab's rope tables hold: a call's latent frames
let private TS_PK_KV_BIT = 1u << 28u         //! the hazard bit the voice's key rows share, and the value rows'
let private TS_PK_VV_BIT = 1u << 29u

var private g_ts_pk_codec : TsSlab
var private g_ts_pk_codec_slots : TtsPkCodecSlab
var private g_ts_pk_frames : TsSlab
var private g_ts_pk_frames_slots : TtsPkFramesSlab
var private g_ts_pkc_scratch : TsScratch
var private g_ts_pkf_scratch : TsScratch
var private @scratch g_ts_pk_rows : array<int64>   //! the codec chain's row counts: the frames, the 200 Hz rows, each stage's rows
var private @scratch g_ts_pk_kv_host : array<float>

[cold_path, arch(at="../ARCHITECTURE_GPU_TOWER_TTS.md#tts-st2-seats")]
def tts_pk_bake(m : PocketModel; var b : TowerDevBuild; dev : TowerDev) : bool {
    //! false (the model takes no flavor) where a part's shapes sit off the seats' stamps; device-free
    let frames_ok = tts_pk_frames_ok(m, TS_PK_ATTN_HS, TS_PK_GEMV_MAX_NIN) $(l : TtsLinear) : bool {
        return tts_lin_ok4(l)
    }
    return false if (!frames_ok || !tts_pk_codec_ok(m.mimi, TS_PK_ATTN_HS, TS_PK_ATTN_MAX_KEYS, TTS_PK_ATTN_HS_MIN))
    let rope_rows = TS_PK_CODEC_MAX_FRAMES * m.mimi.frame_steps
    var inscope sc : TtsPkCodecSlab
    tts_part_bake(b, dev, "pk.codec", sc, $(var w : TtsSlabWriter; var s : TtsPkCodecSlab) {
        tts_pk_codec_write(w, m.mimi, rope_rows, s)
    }, true, true)   // rows-form alone reads this part's dense leaves: the home's GEMM tile's
    var inscope sf : TtsPkFramesSlab
    tts_part_bake(b, dev, "pk.frames", sf, $(var w : TtsSlabWriter; var s : TtsPkFramesSlab) {
        tts_pk_frames_write(w, m, s)
    }, true, true, TS_OWN_GEMV)   // the frame's GEMV and the prompt's rows read these leaves: the formats the home's GEMV decodes
    if (m.cloning && tts_pk_enc_ok(m.mimi, TS_PK_ATTN_HS, TS_PK_ATTN_MAX_KEYS, TTS_PK_ATTN_HS_MIN)) {
        let enc_rope_rows = TS_PK_ENC_MAX_FRAMES * m.mimi.frame_steps
        var inscope se : TtsPkEncSlab
        tts_part_bake(b, dev, "pk.enc", se, $(var w : TtsSlabWriter; var s : TtsPkEncSlab) {
            tts_pk_enc_write(w, m.mimi, enc_rope_rows, s)
        }, true, true)   // rows-form alone reads this part's dense leaves: the home's GEMM tile's
    }
    return true
}

def private ts_pk_codec_attach(rl : auto(R); blk : TtsDevBlock) : bool
    => ts_slab_attach(rl, g_ts_pk_codec, g_ts_pk_codec_slots, blk, "pk.codec", "pocket codec slab")

//! the attach ledger under the info log: a slab's floats, its host build and its upload
[cold_path]
def private ts_attach_log(rl : auto(R); what : string; floats : int64; build_us, upload_us : int) {
    to_log(LOG_INFO, "{_::rail_tts_tag(rl)} tts {what}: {floats * 4l / 1048576l} MB, built in {build_us / 1000} ms, uploaded in {upload_us / 1000} ms\n")
}

//! a seat's pipeline build wall `us` and the driver's share since `drv0`, and `arms_us`, the device bring-up the call paid; the call site holds the ms test
[cold_path]
def private ts_ensure_log(rl : auto(R); seat : string; us, drv0, arms_us : int) {
    if (arms_us >= 1000) {
        to_log(LOG_INFO, "{_::rail_tts_tag(rl)} tts {seat}: the device up in {arms_us / 1000} ms\n")
    }
    if (us >= 1000) {
        to_log(LOG_INFO, "{_::rail_tts_tag(rl)} tts {seat} classes: built in {us / 1000} ms, the driver's {(_::rail_tts_build_us(rl) - drv0) / 1000} ms of it\n")
    }
}

//! the device armed, then the seat's classes built (`ensure`); false on either, which the caller declines as `device`; a bring-up or build wall past a ms logged
def private ts_arm_ensure(var rl : auto(R); seat : string; ensure : block<() : bool>) : bool {
    ts_claim(rl)
    let te = ref_time_ticks()
    let drv0 = _::rail_tts_build_us(rl)
    return false if (!ts_arms(rl))
    let arms_us = get_time_usec(te)
    return false if (!invoke(ensure))
    let us = get_time_usec(te) - arms_us
    if (us >= 1000 || arms_us >= 1000) {
        ts_ensure_log(rl, seat, us, drv0, arms_us)
    }
    return true
}

def private ts_pk_frames_attach(rl : auto(R); m : PocketModel) : bool
    => ts_slab_attach(rl, g_ts_pk_frames, g_ts_pk_frames_slots, ts_pk_block(m), "pk.frames", "pocket frames slab")

//! one voice's device K/V rows [cap][d] per backbone layer, and the rope tables (cos then sin) for every position they can hold
struct private TsPkVoice {
    meta : TtsPkVoiceMeta
    k : array<RailBuf>
    v : array<RailBuf>
    rope : RailBuf
    sin_off : int64
    gen : uint64      //! counted up at every rebuild: a prompt's residency record names the build it sits in
}

var private g_ts_pk_voice : TsPkVoice

def private ts_pk_voice_drop(rl : auto(R)) {
    var v & = g_ts_pk_voice
    for (r in v.k) {
        ts_rows_release(rl, r)
    }
    for (r in v.v) {
        ts_rows_release(rl, r)
    }
    v.k |> clear()
    v.v |> clear()
    ts_rows_release(rl, v.rope)
    v.meta = TtsPkVoiceMeta()
}

//! host rows [r0, r1) of one cache as the device's [pos][d] rows, staged then uploaded at row r0
[arch(at = "../ARCHITECTURE_GPU_TOWER_TTS.md#tts-pocket-seats")]
def private ts_pk_kv_upload(rl : auto(R); kc : TtsKvCache; bk, bv : RailBuf; r0, r1 : int64) {
    let d = kc.heads * kc.dh
    let rows = r1 - r0
    return if (rows <= 0l)
    g_ts_pk_kv_host |> ensure_length(2l * rows * d)
    unsafe {
        kv_cache_read_rows(kc, r0, r1, 0l, addr(g_ts_pk_kv_host[0]), addr(g_ts_pk_kv_host[rows * d]))
    }
    _::rail_tts_put(rl, bk.dev, r0 * d * 4l, unsafe(addr(g_ts_pk_kv_host[0])), rows * d * 4l)
    _::rail_tts_put(rl, bv.dev, r0 * d * 4l, unsafe(addr(g_ts_pk_kv_host[rows * d])), rows * d * 4l)
}

def private ts_pk_voice_attach(rl : auto(R); vs : PocketVoiceState; tf : PocketTransformer) : bool
    => tts_pk_voice_current(g_ts_pk_voice.meta, vs, length(g_ts_pk_voice.k), tts_pk_kv_width(tf)) || ts_pk_voice_rebuild(rl, vs, tf)

[cold_path]
def private ts_pk_voice_rebuild(rl : auto(R); vs : PocketVoiceState; tf : PocketTransformer) : bool {
    var v & = g_ts_pk_voice
    ts_pk_voice_drop(rl)
    v.gen++
    let t0 = ref_time_ticks()
    let cap = vs.caches[0].cap
    let d = tts_pk_kv_width(tf)
    let half = tf.dh / 2l
    v.k |> reserve(length(vs.caches))
    v.v |> reserve(length(vs.caches))
    for (kc in vs.caches) {
        var bk, bv : RailBuf
        if (!ts_rows_alloc(rl, bk, cap * d * 4l, TS_PK_KV_BIT) || !ts_rows_alloc(rl, bv, cap * d * 4l, TS_PK_VV_BIT)) {
            ts_rows_release(rl, bk)
            ts_pk_voice_drop(rl)
            return false
        }
        v.k |> push(bk)
        v.v |> push(bv)
        ts_pk_kv_upload(rl, kc, bk, bv, 0l, vs.len)
    }
    var inscope @exact_size tcos : array<float>
    var inscope @exact_size tsin : array<float>
    tts_pk_rope_tabs(tf, cap, tcos, tsin)
    if (!ts_rows_alloc(rl, v.rope, 2l * cap * half * 4l, 0u)) {
        ts_pk_voice_drop(rl)
        return false
    }
    ts_upload(rl, v.rope, tcos, cap * half)
    _::rail_tts_put(rl, v.rope.dev, cap * half * 4l, unsafe(addr(tsin[0])), cap * half * 4l)
    v.sin_off = cap * half
    v.meta = tts_pk_voice_meta(vs, d)
    to_log(LOG_INFO, "{_::rail_tts_tag(rl)} tts pocket voice slot: {vs.len} of {cap} rows over {length(vs.caches)} layers, up in {get_time_usec(t0) / 1000} ms\n")
    return true
}

//! a codec conv in the CPU's first-window form (forward: causal pad k - stride; transposed: t * stride rows, no pad); the output row count
def private ts_pk_conv(var rl : auto(R); slab : TsSlab; slot : TtsConvSlot; cv : TtsConv1d; t : int64; x, col, y : RailBuf) : int64 {
    let rows = cv.transposed ? t * cv.stride : t
    ts_conv_padded(rl, slab, slot, cv, t, rows, cv.transposed ? 0l : cv.k - cv.stride, x, col, y)
    return rows
}

//! a window of rows copied: `rows` x `width` from src [.][ss] at column sc, rows sr0.., into dst [.][ds] at column dc, rows dr0.., the ELU on the way where asked
def private ts_pk_rows(var rl : auto(R); src, dst : RailBuf; pa : GkPkRowsArgs; elu : bool) {
    var pc = pa
    let total = int64(pa.rows) * int64(pa.width)
    if (elu) {
        _::enc_tts_pk_rows_elu_cls(rl, _::rail_seats(rl, fixed_array(src, dst)), pc, total)
    } else {
        _::enc_tts_pk_rows_cls(rl, _::rail_seats(rl, fixed_array(src, dst)), pc, total)
    }
    _::rail_pt(rl, RailProfRole.tts_pk)
}

def private ts_pk_rows_args(rows, width : int64) : GkPkRowsArgs
    => GkPkRowsArgs(rows = uint(rows), width = uint(width), ss = uint(width), sc = 0u, sr0 = 0u, ds = uint(width), dc = 0u, dr0 = 0u)

//! the ELU over x's first rows x width elements in place
def private ts_pk_elu(var rl : auto(R); x : RailBuf; rows, width : int64) {
    ts_pk_rows(rl, x, x, ts_pk_rows_args(rows, width), true)
}

//! x [rows][c] *= the slab's scale row at `soff`
def private ts_pk_row_scale(var rl : auto(R); slab : TsSlab; x : RailBuf; rows, c, soff : int64) {
    var pa = GkBiasActArgs(d = uint(c), nelem = uint(rows * c), boff = uint(soff))
    _::enc_tts_pk_row_scale_cls(rl, _::rail_seats(rl, fixed_array(x, slab.rows)), pa, rows * c)
    _::rail_pt(rl, RailProfRole.tts_pk)
}

//! the adjacent-pair rope over the span `d` wide at column `col` of x's rows [rows][qs], positions 0.., on the tables in `tabs` (cos at coff, sin at soff)
def private ts_pk_rope(var rl : auto(R); tabs : RailBuf; coff, soff : int64; x : RailBuf; rows, qs, col, d, dh : int64) {
    var pa = GkRopeTabArgs(rows = uint(rows), qs = uint(qs), col = uint(col), d = uint(d), dh = uint(dh), coff = uint(coff), soff = uint(soff))
    _::enc_tts_pk_rope_cls(rl, _::rail_seats(rl, fixed_array(x, tabs, tabs)), pa, rows * d / 2l)
    _::rail_pt(rl, RailProfRole.tts_pk)
}

//! the causal cached attention row over `t` query rows of the fused [q | k | v] rows (stride `qs`, query i at position pos0 + i)
//! against the caches' rows: the transformer's query width, head size, key-value width and heads-per-key-head set the kernel's arguments
def private ts_pk_attn(var rl : auto(R); qkv, k, v, o : RailBuf; tf : PocketTransformer; t, qs, pos0 : int64) {
    var pa = GkPkAttnArgs(t = uint(t), qs = uint(qs), qcol = 0u, d = uint(tf.attn_dim), pos0 = uint(pos0), ctx = uint(tf.context), hs = uint(tf.dh),
        kvs = uint(tts_pk_kv_width(tf)))
    _::enc_tts_pk_attn_cls(rl, _::rail_seats(rl, fixed_array(qkv, k, v, o)), pa, tts_attn_wgs(t, tf.heads))
    _::rail_pt(rl, RailProfRole.tts_pk_attn)
}

//! x += y (no bias: the slab's zero row), then xb = layernorm(x) on the norm slot's rows - a transformer layer's residual join and the norm after it
def private ts_pk_add_ln(var rl : auto(R); slab : TsSlab; zero : int64; ns : TtsNormSlot; x, y, xb : RailBuf; rows, d : int64; eps : float) {
    let bslot = TtsConvSlot(boff = zero)
    ts_add_ln(rl, slab, bslot, ns, x, y, xb, rows, d, eps)
}

//! one GEMV site of the frame: the row in and out, and the operands a stamp's prologue and epilogue read - the modulation rows, the norm's
//! scale and shift, the slab vector, the noise row and the latent row
struct private TsPkSite {
    x : RailBuf
    xoff : int64
    y : RailBuf
    yoff : int64
    mod : RailBuf
    moff : int64
    lwoff : int64
    lboff : int64
    aoff : int64
    x0 : RailBuf
    x0off : int64
    lat : RailBuf
    loff : int64
    x0mul : float = 1.0   //! the tail's noise weight: 0 for a drifting head, whose output is the frame itself
    eps : float = POCKET_HEAD_LN_EPS   //! the input norm's epsilon: the head's unless the site sits in the backbone
}

def private ts_pk_site(x : RailBuf; xoff : int64; y : RailBuf; yoff : int64 = 0l) : TsPkSite => TsPkSite(x = x, xoff = xoff, y = y, yoff = yoff)

//! the formats the stored row GEMV has no stamp for: every member but Q8_0, Q4_K, Q5_K and Q6_K
let private TS_PKST_SKIP = "q40 q51 iq4xs k3 iq3s iq3xxs iq4nl k2 tq2 tq1 iq2s iq2xs iq2xxs iq1s iq1m mx4 iq4nl32 iq3s4 q4032 q5032 nv4 q20 q10"

//! the stored row GEMV at `s`: the stamp of its kind over the leaf's member, a member a stamp (`TtsPkGemvStoredT`)
def tts_pkst_enc(var rl : auto(R); s : TtsPkStSite; args : GkPkGemvArgs; nout : int64) {   // nolint:STYLE037,STYLE038 - kq_tile_stamp's generated arms, a format ladder a kind
    var pa = args
    let fmt = s.fmt
    if (s.kind == TTS_PK_GEMV_DOT) {
        let seats = _::rail_seats(rl, fixed_array(s.rows, s.x, s.y, s.wq, s.ws))
        if (fmt == TTS_PK_ST_HALVES) {
            _::enc_f16_tts_pkst_dot_cls(rl, seats, pa, nout)
        } else {
            kq_tile_stamp(fmt, TS_PKST_SKIP, "tts_pkst") <| $() {
                _::tile_enc_dot_cls(rl, seats, pa, nout)
            }
        }
    } elif (s.kind == TTS_PK_GEMV_LN_SILU) {
        let seats = _::rail_seats(rl, fixed_array(s.rows, s.x, s.y, s.mod, s.wq, s.ws))
        if (fmt == TTS_PK_ST_HALVES) {
            _::enc_f16_tts_pkst_ln_silu_cls(rl, seats, pa, nout)
        } else {
            kq_tile_stamp(fmt, TS_PKST_SKIP, "tts_pkst") <| $() {
                _::tile_enc_ln_silu_cls(rl, seats, pa, nout)
            }
        }
    } elif (s.kind == TTS_PK_GEMV_GATE) {
        let seats = _::rail_seats(rl, fixed_array(s.rows, s.x, s.y, s.mod, s.wq, s.ws))
        if (fmt == TTS_PK_ST_HALVES) {
            _::enc_f16_tts_pkst_gate_cls(rl, seats, pa, nout)
        } else {
            kq_tile_stamp(fmt, TS_PKST_SKIP, "tts_pkst") <| $() {
                _::tile_enc_gate_cls(rl, seats, pa, nout)
            }
        }
    } elif (s.kind == TTS_PK_GEMV_ADD_SILU) {
        let seats = _::rail_seats(rl, fixed_array(s.rows, s.x, s.y, s.wq, s.ws))
        if (fmt == TTS_PK_ST_HALVES) {
            _::enc_f16_tts_pkst_add_silu_cls(rl, seats, pa, nout)
        } else {
            kq_tile_stamp(fmt, TS_PKST_SKIP, "tts_pkst") <| $() {
                _::tile_enc_add_silu_cls(rl, seats, pa, nout)
            }
        }
    } elif (s.kind == TTS_PK_GEMV_TAIL) {
        let seats = _::rail_seats(rl, fixed_array(s.rows, s.x, s.y, s.mod, s.x0, s.lat, s.wq, s.ws))
        if (fmt == TTS_PK_ST_HALVES) {
            _::enc_f16_tts_pkst_tail_cls(rl, seats, pa, nout)
        } else {
            kq_tile_stamp(fmt, TS_PKST_SKIP, "tts_pkst") <| $() {
                _::tile_enc_tail_cls(rl, seats, pa, nout)
            }
        }
    } elif (s.kind == TTS_PK_GEMV_LN_GELU) {
        let seats = _::rail_seats(rl, fixed_array(s.rows, s.x, s.y, s.wq, s.ws))
        if (fmt == TTS_PK_ST_HALVES) {
            _::enc_f16_tts_pkst_ln_gelu_cls(rl, seats, pa, nout)
        } else {
            kq_tile_stamp(fmt, TS_PKST_SKIP, "tts_pkst") <| $() {
                _::tile_enc_ln_gelu_cls(rl, seats, pa, nout)
            }
        }
    } elif (s.kind == TTS_PK_GEMV_ADD_SCALE) {
        let seats = _::rail_seats(rl, fixed_array(s.rows, s.x, s.y, s.wq, s.ws))
        if (fmt == TTS_PK_ST_HALVES) {
            _::enc_f16_tts_pkst_add_scale_cls(rl, seats, pa, nout)
        } else {
            kq_tile_stamp(fmt, TS_PKST_SKIP, "tts_pkst") <| $() {
                _::tile_enc_add_scale_cls(rl, seats, pa, nout)
            }
        }
    } elif (s.kind == TTS_PK_GEMV_ADD) {
        let seats = _::rail_seats(rl, fixed_array(s.rows, s.x, s.y, s.wq, s.ws))
        if (fmt == TTS_PK_ST_HALVES) {
            _::enc_f16_tts_pkst_add_cls(rl, seats, pa, nout)
        } else {
            kq_tile_stamp(fmt, TS_PKST_SKIP, "tts_pkst") <| $() {
                _::tile_enc_add_cls(rl, seats, pa, nout)
            }
        }
    } elif (s.kind == TTS_PK_GEMV_LN_QKV) {
        let seats = _::rail_seats(rl, fixed_array(s.rows, s.x, s.y, s.tabs, s.kc, s.vc, s.wq, s.ws))
        if (fmt == TTS_PK_ST_HALVES) {
            _::enc_f16_tts_pkst_ln_qkv_cls(rl, seats, pa, nout)
        } else {
            kq_tile_stamp(fmt, TS_PKST_SKIP, "tts_pkst") <| $() {
                _::tile_enc_ln_qkv_cls(rl, seats, pa, nout)
            }
        }
    } else {
        panic("dasLLAMA TTS: a stored row GEMV of kind {s.kind} has no stamp")
    }
}

//! the stored-row GEMV stamps built: every kind of every member
def tts_pkst_ensure(rl : auto(R)) : bool {
    return (_::rail_ensure_q8_tts_pkst_dot_cls(rl) && _::rail_ensure_q8_tts_pkst_ln_silu_cls(rl) && _::rail_ensure_q8_tts_pkst_gate_cls(rl) && _::rail_ensure_q8_tts_pkst_add_silu_cls(rl) && _::rail_ensure_q8_tts_pkst_tail_cls(rl) && _::rail_ensure_q8_tts_pkst_ln_gelu_cls(rl) && _::rail_ensure_q8_tts_pkst_add_scale_cls(rl) && _::rail_ensure_q8_tts_pkst_add_cls(rl) && _::rail_ensure_q8_tts_pkst_ln_qkv_cls(rl)
        && _::rail_ensure_kq_tts_pkst_k4_dot_cls(rl) && _::rail_ensure_kq_tts_pkst_k4_ln_silu_cls(rl) && _::rail_ensure_kq_tts_pkst_k4_gate_cls(rl) && _::rail_ensure_kq_tts_pkst_k4_add_silu_cls(rl) && _::rail_ensure_kq_tts_pkst_k4_tail_cls(rl) && _::rail_ensure_kq_tts_pkst_k4_ln_gelu_cls(rl) && _::rail_ensure_kq_tts_pkst_k4_add_scale_cls(rl) && _::rail_ensure_kq_tts_pkst_k4_add_cls(rl) && _::rail_ensure_kq_tts_pkst_k4_ln_qkv_cls(rl)
        && _::rail_ensure_kq_tts_pkst_k5_dot_cls(rl) && _::rail_ensure_kq_tts_pkst_k5_ln_silu_cls(rl) && _::rail_ensure_kq_tts_pkst_k5_gate_cls(rl) && _::rail_ensure_kq_tts_pkst_k5_add_silu_cls(rl) && _::rail_ensure_kq_tts_pkst_k5_tail_cls(rl) && _::rail_ensure_kq_tts_pkst_k5_ln_gelu_cls(rl) && _::rail_ensure_kq_tts_pkst_k5_add_scale_cls(rl) && _::rail_ensure_kq_tts_pkst_k5_add_cls(rl) && _::rail_ensure_kq_tts_pkst_k5_ln_qkv_cls(rl)
        && _::rail_ensure_kq_tts_pkst_k6_dot_cls(rl) && _::rail_ensure_kq_tts_pkst_k6_ln_silu_cls(rl) && _::rail_ensure_kq_tts_pkst_k6_gate_cls(rl) && _::rail_ensure_kq_tts_pkst_k6_add_silu_cls(rl) && _::rail_ensure_kq_tts_pkst_k6_tail_cls(rl) && _::rail_ensure_kq_tts_pkst_k6_ln_gelu_cls(rl) && _::rail_ensure_kq_tts_pkst_k6_add_scale_cls(rl) && _::rail_ensure_kq_tts_pkst_k6_add_cls(rl) && _::rail_ensure_kq_tts_pkst_k6_ln_qkv_cls(rl)
        && _::rail_ensure_f16_tts_pkst_dot_cls(rl) && _::rail_ensure_f16_tts_pkst_ln_silu_cls(rl) && _::rail_ensure_f16_tts_pkst_gate_cls(rl) && _::rail_ensure_f16_tts_pkst_add_silu_cls(rl) && _::rail_ensure_f16_tts_pkst_tail_cls(rl) && _::rail_ensure_f16_tts_pkst_ln_gelu_cls(rl) && _::rail_ensure_f16_tts_pkst_add_scale_cls(rl) && _::rail_ensure_f16_tts_pkst_add_cls(rl) && _::rail_ensure_f16_tts_pkst_ln_qkv_cls(rl))
}

//! the stored row GEMV at `site` over `p`'s leaf, the home's stamp for a leaf in its layout; a scale-less member seats its quants twice
def private ts_pkst(var rl : auto(R); slab : TsSlab; p : TtsConvSlot; var site : TtsPkStSite; args : GkPkGemvArgs; nout : int64) {
    let leaf = slab.leaves[p.stored - 1]
    var pa = args
    pa.woff = uint(leaf.woff)
    pa.stride = uint(leaf.wstride)
    pa.wsoff = uint(leaf.soff)
    site.fmt = leaf.fmt
    site.rows = slab.rows
    site.wq = slab.wq[leaf.member]
    site.ws = slab.ws[leaf.member].dev == 0ul ? slab.wq[leaf.member] : slab.ws[leaf.member]
    if (leaf.member >= TS_ST_OWN) {
        if (!_::rail_tts_pk_own(rl, site, pa, nout)) {
            panic("dasLLAMA TTS: a leaf in a home's own layout is its own row GEMV's to read")
        }
        return
    }
    tts_pkst_enc(rl, site, pa, nout)
}

//! the row GEMV at a site under the stamp `kind` names (a `TTS_PK_GEMV_*`), over the slab's f32 rows of `p` or its stored leaf
def private ts_pk_gemv(var rl : auto(R); slab : TsSlab; p : TtsConvSlot; nin, nout : int64; kind : int; s : TsPkSite) {
    var pa = GkPkGemvArgs(nin = uint(nin), nout = uint(nout), stride = uint(p.kdim_p), eps = s.eps, woff = uint(p.woff), boff = uint(p.boff),
        xoff = uint(s.xoff), yoff = uint(s.yoff), moff = uint(s.moff), lwoff = uint(s.lwoff), lboff = uint(s.lboff), aoff = uint(s.aoff),
        x0off = uint(s.x0off), loff = uint(s.loff), x0mul = s.x0mul)
    ts_count_widened(rl, p.widened)
    if (p.stored != 0) {
        var site = TtsPkStSite(kind = kind, x = s.x, y = s.y, mod = s.mod, x0 = s.x0, lat = s.lat)
        ts_pkst(rl, slab, p, site, pa, nout)
        _::rail_pt(rl, ts_pk_gemv_role(kind))
        return
    }
    if (kind == TTS_PK_GEMV_DOT) {
        _::enc_tts_pk_gemv_dot_cls(rl, _::rail_seats(rl, fixed_array(slab.rows, s.x, s.y)), pa, nout)
    } elif (kind == TTS_PK_GEMV_LN_SILU) {
        _::enc_tts_pk_gemv_ln_silu_cls(rl, _::rail_seats(rl, fixed_array(slab.rows, s.x, s.y, s.mod)), pa, nout)
    } elif (kind == TTS_PK_GEMV_GATE) {
        _::enc_tts_pk_gemv_gate_cls(rl, _::rail_seats(rl, fixed_array(slab.rows, s.x, s.y, s.mod)), pa, nout)
    } elif (kind == TTS_PK_GEMV_ADD_SILU) {
        _::enc_tts_pk_gemv_add_silu_cls(rl, _::rail_seats(rl, fixed_array(slab.rows, s.x, s.y)), pa, nout)
    } elif (kind == TTS_PK_GEMV_LN_GELU) {
        _::enc_tts_pk_gemv_ln_gelu_cls(rl, _::rail_seats(rl, fixed_array(slab.rows, s.x, s.y)), pa, nout)
    } elif (kind == TTS_PK_GEMV_ADD_SCALE) {
        _::enc_tts_pk_gemv_add_scale_cls(rl, _::rail_seats(rl, fixed_array(slab.rows, s.x, s.y)), pa, nout)
    } elif (kind == TTS_PK_GEMV_ADD) {
        _::enc_tts_pk_gemv_add_cls(rl, _::rail_seats(rl, fixed_array(slab.rows, s.x, s.y)), pa, nout)
    } else {
        _::enc_tts_pk_gemv_tail_cls(rl, _::rail_seats(rl, fixed_array(slab.rows, s.x, s.y, s.mod, s.x0, s.lat)), pa, nout)
    }
    _::rail_pt(rl, ts_pk_gemv_role(kind))
}

//! the ledger role of a row GEMV stamp
def private ts_pk_gemv_role(kind : int) : RailProfRole {
    if (kind == TTS_PK_GEMV_DOT) return RailProfRole.tts_pk_dot
    if (kind == TTS_PK_GEMV_LN_GELU) return RailProfRole.tts_pk_ffn
    if (kind == TTS_PK_GEMV_ADD_SCALE || kind == TTS_PK_GEMV_ADD) return RailProfRole.tts_pk_res
    return RailProfRole.tts_pk_head
}

def private ts_pk_lin(var rl : auto(R); slab : TsSlab; p : TtsConvSlot; nin, nout : int64; x : RailBuf; xoff : int64; y : RailBuf; yoff : int64) {
    ts_pk_gemv(rl, slab, p, nin, nout, TTS_PK_GEMV_DOT, ts_pk_site(x, xoff, y, yoff))
}

let private TS_C_LAT = 0     //! the latent frames [frames][cin]
let private TS_C_Q = 1       //! ... projected [frames][d]
let private TS_C_X = 2       //! the transformer's residual rows [t1][d]
let private TS_C_XB = 3
let private TS_C_QKV = 4
let private TS_C_CTX = 5
let private TS_C_H = 6
let private TS_C_A = 9       //! the stages' rows: the stream pinging between A and B, the upsample, the block's two temporaries
let private TS_C_B = 10
let private TS_C_UP = 11
let private TS_C_CC = 12
let private TS_C_CP = 13
let private TS_C_O = 14      //! dec_out's rows [samples][cout]
let private TS_C_WAVE = 15
let private TS_C_COL = 16
let private TS_C_STG = 17    //! a stream conv's input: its carried rows, then the window's
let private TS_C_CARRY = 18  //! every stream conv's carried rows, each at its own rows (`g_ts_pkc_crow`)
let private TS_PK_CODEC_WINDOW = 32l   //! the latent frames a codec pass decodes: the stream's window, any length a call

var private @scratch g_ts_pkc_cn : array<int64>     //! a stream conv's carried rows now, in the chain's conv order
var private @scratch g_ts_pkc_crow : array<int64>   //! ... and its first row in the carry rows, at its own input width
var private g_ts_pkc_conv = 0                       //! the chain's next stream conv
var private g_ts_pkc_k : array<RailBuf>             //! the codec transformer's keys, a layer's rows of the call (the voice slot's hazard bits)
var private g_ts_pkc_v : array<RailBuf>             //! ... and its values
var private g_ts_pkc_kv_rows = 0l                   //! the rows each holds

//! every codec transformer layer's keys and values for a call of `rows` rows of `d`: kept where they hold as many, else rebuilt; false on an allocation failure
[cold_path]
def private ts_pk_codec_caches(rl : auto(R); layers : int; rows, d : int64) : bool {
    return true if (length(g_ts_pkc_k) == layers && g_ts_pkc_kv_rows >= rows)
    ts_pk_codec_caches_drop(rl)
    let cap = round_up(rows, 1024l)
    g_ts_pkc_k |> reserve(layers)
    g_ts_pkc_v |> reserve(layers)
    for (_li in range(layers)) {
        var bk, bv : RailBuf
        if (!ts_rows_alloc(rl, bk, cap * d * 4l, TS_PK_KV_BIT, "codec keys") || !ts_rows_alloc(rl, bv, cap * d * 4l, TS_PK_VV_BIT, "codec values")) {
            ts_rows_release(rl, bk)
            ts_pk_codec_caches_drop(rl)
            return false
        }
        g_ts_pkc_k |> push(bk)
        g_ts_pkc_v |> push(bv)
    }
    g_ts_pkc_kv_rows = cap
    return true
}

//! one more stream conv at carry element `at` (a forward conv's first carry its causal pad), widest stage input into `stg`; next element
def private ts_pk_stream_plan(cv : TtsConv1d; rows_in : int64; at : int64; var stg : int64&) : int64 {
    let need = conv1d_carry_rows(cv)
    let row = (at + cv.cin - 1l) / cv.cin
    g_ts_pkc_crow |> ensure_length(int64(g_ts_pkc_conv + 1))
    g_ts_pkc_cn |> ensure_length(int64(g_ts_pkc_conv + 1))
    g_ts_pkc_crow[g_ts_pkc_conv] = row
    g_ts_pkc_cn[g_ts_pkc_conv] = cv.transposed ? 0l : need
    g_ts_pkc_conv++
    stg = max(stg, (need + rows_in) * cv.cin)
    return (row + need) * cv.cin
}

//! the decoder's stream convs planned in the chain's order for windows of `g_ts_pk_rows`: the carry rows' and the stage rows' floats
def private ts_pk_codec_stream_plan(mm : PocketMimi) : tuple<carry : int64; stg : int64> {
    g_ts_pkc_conv = 0
    var stg = 0l
    var at = ts_pk_stream_plan(mm.upsample, g_ts_pk_rows[0], 0l, stg)
    at = ts_pk_stream_plan(mm.dec_in, g_ts_pk_rows[1], at, stg)
    for (i in range64(long_length(mm.ratios))) {
        at = ts_pk_stream_plan(mm.dec_up[i], g_ts_pk_rows[1l + i], at, stg)
        at = ts_pk_stream_plan(mm.dec_res[i].conv1, g_ts_pk_rows[2l + i], at, stg)
    }
    at = ts_pk_stream_plan(mm.dec_out, g_ts_pk_rows[long_length(g_ts_pk_rows) - 1l], at, stg)
    g_ts_pkc_conv = 0
    return (carry = at, stg = stg)
}

//! the next stream conv's input staged: its carried rows, then the window's `t` rows of x; the carried rows (`ts_pk_stream_keep` closes it)
def private ts_pk_stream_stage(var rl : auto(R); sc_ : TsScratch; cv : TtsConv1d; t : int64; x : RailBuf) : int64 {
    let w = uint(cv.cin)
    let cn = g_ts_pkc_cn[g_ts_pkc_conv]
    let stg = ts_rows(sc_, TS_C_STG)
    if (cn > 0l) {
        ts_pk_rows(rl, ts_rows(sc_, TS_C_CARRY), stg, GkPkRowsArgs(rows = uint(cn), width = w, ss = w, sc = 0u, sr0 = uint(g_ts_pkc_crow[g_ts_pkc_conv]), ds = w, dc = 0u, dr0 = 0u), false)
    }
    ts_pk_rows(rl, x, stg, GkPkRowsArgs(rows = uint(t), width = w, ss = w, sc = 0u, sr0 = 0u, ds = w, dc = 0u, dr0 = uint(cn)), false)
    return cn
}

//! the staged input's tail kept as the conv's carry for the next window, the chain moved to its next stream conv
def private ts_pk_stream_keep(var rl : auto(R); sc_ : TsScratch; cv : TtsConv1d; tin : int64) {
    let w = uint(cv.cin)
    let need = conv1d_carry_rows(cv)
    if (need > 0l) {
        ts_pk_rows(rl, ts_rows(sc_, TS_C_STG), ts_rows(sc_, TS_C_CARRY), GkPkRowsArgs(rows = uint(need), width = w, ss = w, sc = 0u, sr0 = uint(tin - need), ds = w, dc = 0u,
            dr0 = uint(g_ts_pkc_crow[g_ts_pkc_conv])), false)
        g_ts_pkc_cn[g_ts_pkc_conv] = need
    }
    g_ts_pkc_conv++
}

//! a codec conv over one window (`stream_conv`): carried rows lead the window's `t` rows, unpadded, y its own rows; the output rows
def private ts_pk_conv_stream(var rl : auto(R); slab : TsSlab; sc_ : TsScratch; slot : TtsConvSlot; cv : TtsConv1d; t : int64; x, col, y : RailBuf) : int64 {
    let cn = ts_pk_stream_stage(rl, sc_, cv, t, x)
    let tin = cn + t
    let rows = cv.transposed ? t * cv.stride : (tin - cv.k) / cv.stride + 1l
    ts_conv_padded(rl, slab, slot, cv, tin, rows, cv.transposed ? cn * cv.stride : 0l, ts_rows(sc_, TS_C_STG), col, y)
    ts_pk_stream_keep(rl, sc_, cv, tin)
    return rows
}

def private ts_pk_codec_ensure(rl : auto(R)) : bool {
    return (_::rail_ensure_f32_gemm_b_cls(rl) && ts_stored_ensure(rl) && tts_pkst_ensure(rl) && _::rail_ensure_tower_ln_cls(rl) && _::rail_ensure_tower_post_add_ln_cls(rl)
        && _::rail_ensure_tts_im2col_cls(rl) && _::rail_ensure_tts_pool_dw_cls(rl)
        && _::rail_ensure_tts_gelu_tanh_cls(rl) && _::rail_ensure_tts_add_scale_cls(rl) && _::rail_ensure_tts_pk_rows_cls(rl) && _::rail_ensure_tts_pk_rows_elu_cls(rl)
        && _::rail_ensure_tts_pk_row_scale_cls(rl) && _::rail_ensure_tts_pk_attn_cls(rl) && _::rail_ensure_tts_pk_rope_cls(rl))
}

//! the codec seat's slots: the transformer's rows at t1, the stages' at their widest rows x width, the im2col at the widest row
def private ts_pk_codec_floats(mm : PocketMimi; frames : int64) : TsFloats {
    let d = mm.dec_tf.d
    let t1 = g_ts_pk_rows[1]
    var fl : TsFloats
    let plan = ts_pk_codec_stream_plan(mm)
    fl[TS_C_STG] = plan.stg
    fl[TS_C_CARRY] = plan.carry

    fl[TS_C_LAT] = frames * mm.quant_proj.cin
    fl[TS_C_Q] = frames * d
    fl[TS_C_X] = t1 * d
    fl[TS_C_XB] = t1 * d
    fl[TS_C_QKV] = t1 * 3l * d
    fl[TS_C_CTX] = t1 * d
    fl[TS_C_H] = t1 * mm.dec_tf.ffn
    var stage = t1 * mm.dec_in.cout
    var col = max(frames * ts_conv_kdim(mm.quant_proj), t1 * ts_conv_kdim(mm.dec_in))
    for (i in range64(long_length(mm.ratios))) {
        let tn = g_ts_pk_rows[2l + i]
        let up & = unsafe(mm.dec_up[i])
        let rb & = unsafe(mm.dec_res[i])
        stage = max(stage, tn * max(up.cout, rb.conv1.cout))
        col = max(col, tn * max(ts_conv_kdim(up), max(ts_conv_kdim(rb.conv1), ts_conv_kdim(rb.conv2))))
    }
    let samples = g_ts_pk_rows[long_length(g_ts_pk_rows) - 1l]
    for (i in range(TS_C_A, TS_C_CP + 1)) {
        fl[i] = stage
    }
    fl[TS_C_O] = samples * mm.dec_out.cout
    fl[TS_C_WAVE] = samples
    fl[TS_C_COL] = max(col, samples * ts_conv_kdim(mm.dec_out))
    return fl
}

//! a transformer's rows form: every layer's keys and values written at row `kv_row0` of its cache rows, the rope tables'
//! row of position 0 at `coff` / `soff`, the queries at `pos0`; `kv_only_last` ends the last layer at its cache rows
struct private TsPkRowsForm {
    pos0 : int64
    kv_row0 : int64
    tabs : RailBuf
    coff : int64
    soff : int64
    zero : int64
    kv_only_last : bool
}

//! a transformer over the residual rows X [t][d] of the scratch's slot set: the CPU's layer loop, dispatch for dispatch
def private ts_pk_rows_tf(var rl : auto(R); slab : TsSlab; sc_ : TsScratch; tf : PocketTransformer; layers : array<TtsPkLayerSlots>;
                          t : int64; f : TsPkRowsForm; slot_x, slot_xb, slot_qkv, slot_ctx, slot_h : int; kv : block<(li : int) : tuple<k : RailBuf; v : RailBuf>>) {
    let d = tf.d
    let dh = tf.dh
    let qd = tf.attn_dim
    let kvd = tts_pk_kv_width(tf)
    let qkvw = qd + 2l * kvd
    let nd = t * d
    let half = dh / 2l
    let r_x = ts_rows(sc_, slot_x)
    let r_xb = ts_rows(sc_, slot_xb)
    let r_qkv = ts_rows(sc_, slot_qkv)
    let r_ctx = ts_rows(sc_, slot_ctx)
    let r_h = ts_rows(sc_, slot_h)
    var kc = GkPkRowsArgs(rows = uint(t), width = uint(kvd), ss = uint(qkvw), sc = uint(qd), sr0 = 0u, ds = uint(kvd), dc = 0u, dr0 = uint(f.kv_row0))
    var pa_gelu = TtsElemArgs(nelem = uint(t * tf.ffn), off = 0u, slope = 0.0)
    let coff = f.coff + f.pos0 * half
    let soff = f.soff + f.pos0 * half
    let last = length(layers) - 1
    for (i in range(length(layers))) {
        let s & = unsafe(layers[i])
        let cache = invoke(kv, i)
        let kv_only = f.kv_only_last && i == last
        ts_ln(rl, slab, s.n1, r_x, r_xb, t, d, tf.eps)
        ts_lin(rl, slab, s.in_proj, d, qkvw, r_xb, r_qkv, t)
        if (!kv_only) {
            ts_pk_rope(rl, f.tabs, coff, soff, r_qkv, t, qkvw, 0l, qd, dh)
        }
        ts_pk_rope(rl, f.tabs, coff, soff, r_qkv, t, qkvw, qd, kvd, dh)
        kc.sc = uint(qd)
        ts_pk_rows(rl, r_qkv, cache.k, kc, false)
        kc.sc = uint(qd + kvd)
        ts_pk_rows(rl, r_qkv, cache.v, kc, false)
        continue if (kv_only)
        ts_pk_attn(rl, r_qkv, cache.k, cache.v, r_ctx, tf, t, qkvw, f.pos0)
        ts_lin(rl, slab, s.out_proj, qd, d, r_ctx, r_xb, t)
        if (s.has_s1) {
            ts_pk_row_scale(rl, slab, r_xb, t, d, s.scale1_off)
        }
        ts_pk_add_ln(rl, slab, f.zero, s.n2, r_x, r_xb, r_xb, t, d, tf.eps)
        ts_lin(rl, slab, s.ffn1, d, tf.ffn, r_xb, r_h, t)
        _::enc_tts_gelu_tanh_cls(rl, _::rail_seats(rl, fixed_array(r_h)), pa_gelu, t * tf.ffn)
        _::rail_pt(rl, RailProfRole.tts_act)
        ts_lin(rl, slab, s.ffn2, tf.ffn, d, r_h, r_xb, t)
        if (s.has_s2) {
            ts_pk_row_scale(rl, slab, r_xb, t, d, s.scale2_off)
        }
        ts_add_scale(rl, r_x, r_xb, r_x, nd, 1.0)
    }
}

//! the codec transformer over a window's residual rows X [t][d] at positions pos.., each layer's keys and values appended to its own rows of the call
def private ts_pk_transformer(var rl : auto(R); slab : TsSlab; sc_ : TsScratch; tf : PocketTransformer; sl : TtsPkCodecSlab; t, pos : int64) {
    let f = TsPkRowsForm(pos0 = pos, kv_row0 = pos, tabs = slab.rows, coff = sl.rope_cos, soff = sl.rope_sin, zero = sl.zero, kv_only_last = false)
    ts_pk_rows_tf(rl, slab, sc_, tf, sl.layers, t, f, TS_C_X, TS_C_XB, TS_C_QKV, TS_C_CTX, TS_C_H) $(li : int) {
        return (k = g_ts_pkc_k[li], v = g_ts_pkc_v[li])
    }
}

//! one ELU-conv-ELU-conv residual block over x [t][w] (a stage's rows) into y, through the block's two temporaries
def private ts_pk_res_block(var rl : auto(R); slab : TsSlab; sc_ : TsScratch; b : PocketResConv; s1, s2 : TtsConvSlot; x, y : RailBuf; t, w : int64) {
    let r_cc = ts_rows(sc_, TS_C_CC)
    let r_cp = ts_rows(sc_, TS_C_CP)
    let r_col = ts_rows(sc_, TS_C_COL)
    ts_pk_rows(rl, x, r_cc, ts_pk_rows_args(t, w), true)
    ts_pk_conv_stream(rl, slab, sc_, s1, b.conv1, t, r_cc, r_col, r_cp)
    ts_pk_elu(rl, r_cp, t, b.conv1.cout)
    ts_pk_conv(rl, slab, s2, b.conv2, t, r_cp, r_col, y)
    ts_add_scale(rl, y, x, y, t * w, 1.0)
}

[arch(at="../ARCHITECTURE_GPU_TOWER_TTS.md#tts-pocket-seats")]
def private ts_pk_codec_chain(var rl : auto(R); slab : TsSlab; sc_ : TsScratch; mm : PocketMimi; sl : TtsPkCodecSlab; pos, zero_carry : int64) {
    g_ts_pkc_conv = 0
    if (zero_carry > 0l) {
        _::rail_zero(rl, _::rail_seats(rl, fixed_array(ts_rows(sc_, TS_C_CARRY)))[0], 0l, zero_carry * 4l)
    }
    let d = mm.dec_tf.d
    let frames = g_ts_pk_rows[0]
    let t1 = g_ts_pk_rows[1]
    let r_col = ts_rows(sc_, TS_C_COL)
    let r_q = ts_rows(sc_, TS_C_Q)
    let r_x = ts_rows(sc_, TS_C_X)
    ts_pk_conv(rl, slab, sl.quant, mm.quant_proj, frames, ts_rows(sc_, TS_C_LAT), r_col, r_q)
    let ucn = ts_pk_stream_stage(rl, sc_, mm.upsample, frames, r_q)
    var pp = GkDwConvRowsArgs(c = uint(d), k = uint(mm.upsample.k), stride = uint(mm.upsample.stride), pad_l = uint(ucn * mm.upsample.stride), dil = uint(mm.upsample.dilation),
        t_in = uint(ucn + frames), t_out = uint(t1), woff = uint(sl.up_w), boff = uint(sl.up_b))
    _::enc_tts_pool_dw_cls(rl, _::rail_seats(rl, fixed_array(ts_rows(sc_, TS_C_STG), r_x, slab.rows)), pp, t1 * d)
    _::rail_pt(rl, RailProfRole.tts_rows)
    ts_pk_stream_keep(rl, sc_, mm.upsample, ucn + frames)
    ts_pk_transformer(rl, slab, sc_, mm.dec_tf, sl, t1, pos)
    var cur = ts_rows(sc_, TS_C_A)
    ts_pk_conv_stream(rl, slab, sc_, sl.dec_in, mm.dec_in, t1, r_x, r_col, cur)
    var t = t1
    var w = mm.dec_in.cout
    let r_up = ts_rows(sc_, TS_C_UP)
    for (i in range64(long_length(mm.ratios))) {
        ts_pk_elu(rl, cur, t, w)
        let up & = unsafe(mm.dec_up[i])
        let tn = ts_pk_conv_stream(rl, slab, sc_, sl.ups[i], up, t, cur, r_col, r_up)
        let wn = up.cout
        let next = ts_rows(sc_, i % 2l == 0l ? TS_C_B : TS_C_A)
        ts_pk_res_block(rl, slab, sc_, mm.dec_res[i], sl.res1[i], sl.res2[i], r_up, next, tn, wn)
        cur = next
        t = tn
        w = wn
    }
    ts_pk_elu(rl, cur, t, w)
    let r_o = ts_rows(sc_, TS_C_O)
    ts_pk_conv_stream(rl, slab, sc_, sl.dec_out, mm.dec_out, t, cur, r_col, r_o)
    let kw = GkPkRowsArgs(rows = uint(t), width = 1u, ss = uint(mm.dec_out.cout), sc = 0u, sr0 = 0u, ds = 1u, dc = 0u, dr0 = 0u)
    ts_pk_rows(rl, r_o, ts_rows(sc_, TS_C_WAVE), kw, false)
}

[unused_argument(sc), arch(at = "../ARCHITECTURE_GPU_TOWER_TTS.md#tts-pocket-seats")]
def tts_pk_codec(var rl : auto(R); m : PocketModel; latents : array<float>; frames : int64; var sc : PocketScratch; @scratch @exact_size var wave : array<float>) : bool {
    if (!ts_on(rl)) {
        return ts_decline(rl, "codec", TowerDecline.knob)
    }
    if (m.devplan.n == 0l) {
        return ts_decline(rl, "codec", TowerDecline.flavor)
    }
    let mm & = unsafe(m.mimi)
    if (frames < 1l || frames > TS_PK_CODEC_MAX_FRAMES
        || !tts_pk_codec_ok(mm, TS_PK_ATTN_HS, TS_PK_ATTN_MAX_KEYS, TTS_PK_ATTN_HS_MIN) || m.frame_samples != mm.frame_steps * mm.hop
        || long_length(latents) < frames * mm.quant_proj.cin) {
        return ts_decline(rl, "codec", TowerDecline.shape)
    }
    if (!ts_arm_ensure(rl, "pocket codec", $ => ts_pk_codec_ensure(rl))) {
        return ts_decline(rl, "codec", TowerDecline.device)
    }
    if (!ts_pk_codec_attach(rl, ts_pk_block(m))) {
        return ts_decline(rl, "codec", TowerDecline.memory)
    }
    let sl & = g_ts_pk_codec_slots
    let window = min(frames, g_ts_pkc_window)
    tts_pk_stage_rows(mm, window, g_ts_pk_rows)
    let fl = ts_pk_codec_floats(mm, window)
    let kv_ok = ts_pk_codec_caches(rl, length(mm.dec_tf.layers), frames * mm.frame_steps, tts_pk_kv_width(mm.dec_tf))
    if (!kv_ok || !ts_scratch_ready(rl, g_ts_pkc_scratch, fl, 0l, 0l, g_ts_pk_rows[long_length(g_ts_pk_rows) - 1l])) {
        return ts_decline(rl, "codec", TowerDecline.memory)
    }
    let sc_ & = g_ts_pkc_scratch
    let slab & = g_ts_pk_codec
    let r_lat = ts_rows(sc_, TS_C_LAT)
    let r_wave = ts_rows(sc_, TS_C_WAVE)
    let cin = mm.quant_proj.cin
    unsafe(scratch_resize(wave, frames * m.frame_samples))
    var f0 = 0l
    while (f0 < frames) {
        let n = min(window, frames - f0)
        tts_pk_stage_rows(mm, n, g_ts_pk_rows)
        let samples = g_ts_pk_rows[long_length(g_ts_pk_rows) - 1l]
        unsafe {
            _::rail_tts_put(rl, r_lat.dev, 0l, addr(latents[f0 * cin]), n * cin * 4l)
        }
        let pos = f0 * mm.frame_steps
        _::rail_tts_shot(rl, "pocket codec", long_length(mm.dec_tf.layers), g_ts_pk_rows[1]) {
            ts_pk_codec_chain(rl, slab, sc_, mm, sl, pos, f0 == 0l ? fl[TS_C_CARRY] : 0l)
        }
        unsafe {
            _::rail_tts_take(rl, r_wave, 0l, addr(wave[f0 * m.frame_samples]), samples * 4l)
        }
        g_ts_pkc_windows++
        f0 += n
    }
    ts_served(rl)
    return true
}

let private TS_PK_ENC_WINDOW = 8l          //! the latent frames an encoder pass takes: the stream's window over a clip's samples
let private TS_PK_ENC_MAX_FRAMES = 1024l   //! the positions the encoder slab's rope tables hold: a clip's latent frames

var private g_ts_pk_enc : TsSlab
var private g_ts_pk_enc_slots : TtsPkEncSlab

def private ts_pk_enc_attach(rl : auto(R); blk : TtsDevBlock) : bool
    => ts_slab_attach(rl, g_ts_pk_enc, g_ts_pk_enc_slots, blk, "pk.enc", "pocket encoder slab")

//! the encoder's stream convs planned in the chain's order for a window of `samples`, and the scratch a window takes
def private ts_pk_enc_floats(mm : PocketMimi; frames, samples : int64) : TsFloats {
    let tf & = unsafe(mm.enc_tf)
    g_ts_pkc_conv = 0
    var stg = 0l
    var at = ts_pk_stream_plan(mm.enc_in, samples, 0l, stg)
    var t = samples
    var stage = t * mm.enc_in.cout
    var col = t * ts_conv_kdim(mm.enc_in)
    for (b, c in mm.enc_res, mm.enc_down) {
        at = ts_pk_stream_plan(b.conv1, t, at, stg)
        at = ts_pk_stream_plan(c, t, at, stg)
        col = max(col, t * max(ts_conv_kdim(b.conv1), ts_conv_kdim(b.conv2)))
        stage = max(stage, t * max(b.conv1.cin, b.conv1.cout))
        t = t / c.stride
        col = max(col, t * ts_conv_kdim(c))
        stage = max(stage, t * c.cout)
    }
    at = ts_pk_stream_plan(mm.enc_out, t, at, stg)
    at = ts_pk_stream_plan(mm.downsample, t, at, stg)
    g_ts_pkc_conv = 0
    var fl : TsFloats
    fl[TS_C_LAT] = samples
    fl[TS_C_Q] = frames * mm.downsample.cout
    fl[TS_C_X] = t * tf.d
    fl[TS_C_XB] = t * tf.d
    fl[TS_C_QKV] = t * tts_pk_qkv_width(tf)
    fl[TS_C_CTX] = t * tf.attn_dim
    fl[TS_C_H] = t * tf.ffn
    for (i in range(TS_C_A, TS_C_CP + 1)) {
        fl[i] = stage
    }
    fl[TS_C_COL] = max(col, max(t * ts_conv_kdim(mm.enc_out), frames * ts_conv_kdim(mm.downsample)))
    fl[TS_C_STG] = stg
    fl[TS_C_CARRY] = at
    return fl
}

[arch(at="../ARCHITECTURE_GPU_TOWER_TTS.md#tts-pocket-seats")]
def private ts_pk_enc_chain(var rl : auto(R); slab : TsSlab; sc_ : TsScratch; mm : PocketMimi; sl : TtsPkEncSlab; samples, pos, zero_carry : int64) {
    let tf & = unsafe(mm.enc_tf)
    g_ts_pkc_conv = 0
    if (zero_carry > 0l) {
        _::rail_zero(rl, _::rail_seats(rl, fixed_array(ts_rows(sc_, TS_C_CARRY)))[0], 0l, zero_carry * 4l)
    }
    let r_col = ts_rows(sc_, TS_C_COL)
    let r_up = ts_rows(sc_, TS_C_UP)
    let r_x = ts_rows(sc_, TS_C_X)
    var cur = ts_rows(sc_, TS_C_A)
    var t = ts_pk_conv_stream(rl, slab, sc_, sl.enc_in, mm.enc_in, samples, ts_rows(sc_, TS_C_LAT), r_col, cur)
    var w = mm.enc_in.cout
    for (i in range(length(mm.enc_down))) {
        ts_pk_res_block(rl, slab, sc_, mm.enc_res[i], sl.res1[i], sl.res2[i], cur, r_up, t, w)
        ts_pk_elu(rl, r_up, t, w)
        let next = ts_rows(sc_, i % 2 == 0 ? TS_C_B : TS_C_A)
        t = ts_pk_conv_stream(rl, slab, sc_, sl.downs[i], mm.enc_down[i], t, r_up, r_col, next)
        w = mm.enc_down[i].cout
        cur = next
    }
    ts_pk_elu(rl, cur, t, w)
    ts_pk_conv_stream(rl, slab, sc_, sl.enc_out, mm.enc_out, t, cur, r_col, r_x)
    let f = TsPkRowsForm(pos0 = pos, kv_row0 = pos, tabs = slab.rows, coff = sl.rope_cos, soff = sl.rope_sin, zero = sl.zero, kv_only_last = false)
    ts_pk_rows_tf(rl, slab, sc_, tf, sl.layers, t, f, TS_C_X, TS_C_XB, TS_C_QKV, TS_C_CTX, TS_C_H) $(li : int) {
        return (k = g_ts_pkc_k[li], v = g_ts_pkc_v[li])
    }
    if (pos == 0l) {
        let d = uint(tf.d)
        for (r in range64(conv1d_carry_rows(mm.downsample))) {
            ts_pk_rows(rl, r_x, ts_rows(sc_, TS_C_CARRY), GkPkRowsArgs(rows = 1u, width = d, ss = d, sc = 0u, sr0 = 0u, ds = d, dc = 0u,
                dr0 = uint(g_ts_pkc_crow[g_ts_pkc_conv] + r)), false)
        }
    }
    ts_pk_conv_stream(rl, slab, sc_, sl.downsample, mm.downsample, t, r_x, r_col, ts_rows(sc_, TS_C_Q))
}

//! a clip's latent frames on the device, a window a pass as the CPU stream runs them; the frame count, or -1 where the seat declines
[cold_path, arch(at = "../ARCHITECTURE_GPU_TOWER_TTS.md#tts-pocket-seats")]
def tts_pk_encode(var rl : auto(R); m : PocketModel; pcm : array<float>; var sc : PocketScratch; @scratch @exact_size var latents : array<float>) : int64 {
    if (!ts_on(rl)) {
        ts_decline(rl, "encode", TowerDecline.knob)
        return -1l
    }
    if (m.devplan.n == 0l) {
        ts_decline(rl, "encode", TowerDecline.flavor)
        return -1l
    }
    let mm & = unsafe(m.mimi)
    let n = long_length(pcm)
    let frames = (n + m.frame_samples - 1l) / m.frame_samples
    let ld = mm.downsample.cout
    if (!m.cloning || frames < 1l || frames > TS_PK_ENC_MAX_FRAMES || m.frame_samples != mm.frame_steps * mm.hop
        || !tts_pk_enc_ok(mm, TS_PK_ATTN_HS, TS_PK_ATTN_MAX_KEYS, TTS_PK_ATTN_HS_MIN)) {
        ts_decline(rl, "encode", TowerDecline.shape)
        return -1l
    }
    if (!ts_arm_ensure(rl, "pocket encoder", $ => ts_pk_codec_ensure(rl))) {
        ts_decline(rl, "encode", TowerDecline.device)
        return -1l
    }
    let window = min(frames, TS_PK_ENC_WINDOW)
    let wsamples = window * m.frame_samples
    let fl = ts_pk_enc_floats(mm, window, wsamples)
    let ready = (ts_pk_enc_attach(rl, ts_pk_block(m))
        && ts_pk_codec_caches(rl, length(mm.enc_tf.layers), frames * mm.frame_steps, tts_pk_kv_width(mm.enc_tf))
        && ts_scratch_ready(rl, g_ts_pkc_scratch, fl, 0l, 0l, window * ld))
    if (!ready) {
        ts_decline(rl, "encode", TowerDecline.memory)
        return -1l
    }
    let sc_ & = g_ts_pkc_scratch
    let slab & = g_ts_pk_enc
    let sl & = g_ts_pk_enc_slots
    latents |> reserve_resize(frames * ld)
    sc.ca |> reserve_resize(wsamples)
    var f0 = 0l
    while (f0 < frames) {
        let nf = min(window, frames - f0)
        let ns = nf * m.frame_samples
        for (i in range64(ns)) {
            let at = f0 * m.frame_samples + i
            sc.ca[i] = at < n ? pcm[at] : 0.0
        }
        ts_upload(rl, ts_rows(sc_, TS_C_LAT), sc.ca, ns)
        let pos = f0 * mm.frame_steps
        _::rail_tts_shot(rl, "pocket encoder", long_length(mm.enc_tf.layers), nf * mm.frame_steps) {
            ts_pk_enc_chain(rl, slab, sc_, mm, sl, ns, pos, f0 == 0l ? fl[TS_C_CARRY] : 0l)
        }
        unsafe {
            _::rail_tts_take(rl, ts_rows(sc_, TS_C_Q), 0l, addr(latents[f0 * ld]), nf * ld * 4l)
        }
        f0 += nf
    }
    ts_served(rl)
    return frames
}

var private g_ts_pkc_window = TS_PK_CODEC_WINDOW     //! the codec stream's window in latent frames

//! a cell's seat: the codec stream's window in latent frames, the old one back; any window decodes the same rows, so a cell holds two equal
[cold_path]
def set_tts_codec_window_(frames : int64) : int64 {
    let was = g_ts_pkc_window
    g_ts_pkc_window = max(frames, 1l)
    return was
}

var private g_ts_pkc_windows = 0l   //! the codec stream's windows decoded since the process started

//! the codec stream's windows decoded since the process started - a cell's witness of the window a call ran at
[cold_path]
def tts_codec_windows_ : int64 => g_ts_pkc_windows

let private TS_F_X = 0        //! the backbone's residual row [d]
let private TS_F_XB = 1
let private TS_F_QKV = 2
let private TS_F_CTX = 3
let private TS_F_H = 4        //! the ffn's row
let private TS_F_HY = 5       //! the head's conditioning
let private TS_F_HX = 6       //! ... its running state
let private TS_F_HM = 7
let private TS_F_HMOD = 8     //! the modulation rows [3 n]
let private TS_F_X1 = 9       //! the frame's latent, the next frame's input
let private TS_F_NOISE = 10   //! the batch's noise rows [batch][ld]
let private TS_F_FORCED = 11  //! the forced frames [forced_n][ld]
let private TS_F_LATENTS = 12 //! every frame's latent row [max_frames][ld]
let private TS_F_CONDS = 13   //! ... and its conditioning row [max_frames][d]
let private TS_F_EOS = 14     //! ... and its EOS logit

def private ts_pk_frames_ensure(rl : auto(R)) : bool {
    return (tts_pkst_ensure(rl) && _::rail_ensure_tower_ln_cls(rl) && _::rail_ensure_tts_pk_rows_cls(rl) && _::rail_ensure_tts_pk_attn_cls(rl) && _::rail_ensure_tts_pk_gemv_dot_cls(rl) && _::rail_ensure_tts_pk_gemv_ln_silu_cls(rl)
        && _::rail_ensure_tts_pk_gemv_gate_cls(rl) && _::rail_ensure_tts_pk_gemv_add_silu_cls(rl) && _::rail_ensure_tts_pk_gemv_tail_cls(rl) && _::rail_ensure_tts_pk_gemv_ln_qkv_cls(rl)
        && _::rail_ensure_tts_pk_gemv_add_scale_cls(rl) && _::rail_ensure_tts_pk_gemv_add_cls(rl) && _::rail_ensure_tts_pk_gemv_ln_gelu_cls(rl))
}

def private ts_pk_frames_floats(m : PocketModel; max_frames, batch, forced_n : int64) : TsFloats {
    let d = m.backbone.d
    let n = m.head.dim
    let ld = m.latent_dim
    var fl : TsFloats
    fl[TS_F_X] = d
    fl[TS_F_XB] = d
    fl[TS_F_QKV] = tts_pk_qkv_width(m.backbone)
    fl[TS_F_CTX] = m.backbone.attn_dim
    fl[TS_F_H] = m.backbone.ffn
    fl[TS_F_HY] = n
    fl[TS_F_HX] = n
    fl[TS_F_HM] = n
    fl[TS_F_HMOD] = 3l * n
    fl[TS_F_X1] = ld
    fl[TS_F_NOISE] = batch * ld
    fl[TS_F_FORCED] = max(forced_n, 1l) * ld
    fl[TS_F_LATENTS] = max_frames * ld
    fl[TS_F_CONDS] = max_frames * m.cond_dim
    fl[TS_F_EOS] = max_frames
    return fl
}

//! the residual join of a frame's projection: x += W . x_in + b, times the layer scale where the layer carries one
def private ts_pk_gemv_add(var rl : auto(R); slab : TsSlab; p : TtsConvSlot; nin, nout : int64; x_in : RailBuf; x : RailBuf; scaled : bool; soff : int64) {
    var s = ts_pk_site(x_in, 0l, x)
    s.aoff = soff
    ts_pk_gemv(rl, slab, p, nin, nout, scaled ? TTS_PK_GEMV_ADD_SCALE : TTS_PK_GEMV_ADD, s)
}

//! one backbone layer at position p: the CPU's `layer_attention` and `layer_ffn` at t = 1 in five dispatches, x the residual in and out
def private ts_pk_fr_layer(var rl : auto(R); slab : TsSlab; sc_ : TsScratch; tf : PocketTransformer; L : TtsPkLayerSlots; li : int; p : int64) {
    let d = tf.d
    let dh = uint(tf.dh)
    let qd = tf.attn_dim
    let kvd = tts_pk_kv_width(tf)
    let qkvw = qd + 2l * kvd
    let r_x = ts_rows(sc_, TS_F_X)
    let r_qkv = ts_rows(sc_, TS_F_QKV)
    let r_ctx = ts_rows(sc_, TS_F_CTX)
    let r_h = ts_rows(sc_, TS_F_H)
    let v & = g_ts_pk_voice
    var pq = GkPkGemvArgs(nin = uint(d), nout = uint(qkvw), stride = uint(L.in_proj.kdim_p), eps = tf.eps, woff = uint(L.in_proj.woff),
        boff = uint(L.in_proj.boff), xoff = 0u, yoff = 0u, moff = 0u, lwoff = uint(L.n1.gwoff), lboff = uint(L.n1.gboff), aoff = 0u, x0off = 0u, loff = 0u,
        span = uint(qd), dh = dh, p = uint(p), coff = 0u, soff = uint(v.sin_off))
    if (L.in_proj.stored != 0) {
        var site = TtsPkStSite(kind = TTS_PK_GEMV_LN_QKV, x = r_x, y = r_qkv, tabs = v.rope, kc = v.k[li], vc = v.v[li])
        ts_pkst(rl, slab, L.in_proj, site, pq, qkvw)
    } else {
        ts_count_widened(rl, L.in_proj.widened)
        _::enc_tts_pk_gemv_ln_qkv_cls(rl, _::rail_seats(rl, fixed_array(slab.rows, r_x, r_qkv, v.rope, v.k[li], v.v[li])), pq, qkvw)
    }
    _::rail_pt(rl, RailProfRole.tts_pk_qkv)
    ts_pk_attn(rl, r_qkv, v.k[li], v.v[li], r_ctx, tf, 1l, qkvw, p)
    ts_pk_gemv_add(rl, slab, L.out_proj, qd, d, r_ctx, r_x, L.has_s1, L.scale1_off)
    var s1 = ts_pk_site(r_x, 0l, r_h)
    s1.lwoff = L.n2.gwoff
    s1.lboff = L.n2.gboff
    s1.eps = tf.eps
    ts_pk_gemv(rl, slab, L.ffn1, d, tf.ffn, TTS_PK_GEMV_LN_GELU, s1)
    ts_pk_gemv_add(rl, slab, L.ffn2, tf.ffn, d, r_h, r_x, L.has_s2, L.scale2_off)
}

//! frame `f` at cache position p, noise row `i` of the batch: the CPU's `frame_step` and `head_step`, the conditioning row and EOS logit kept
def private ts_pk_frame(var rl : auto(R); slab : TsSlab; sc_ : TsScratch; m : PocketModel; sl : TtsPkFramesSlab; p, f, i, forced_rows : int64) {
    let d = m.backbone.d
    let ld = m.latent_dim
    let n = m.head.dim
    let r_x = ts_rows(sc_, TS_F_X)
    let r_xb = ts_rows(sc_, TS_F_XB)
    let r_hy = ts_rows(sc_, TS_F_HY)
    let r_hx = ts_rows(sc_, TS_F_HX)
    let r_hm = ts_rows(sc_, TS_F_HM)
    let r_hmod = ts_rows(sc_, TS_F_HMOD)
    let r_x1 = ts_rows(sc_, TS_F_X1)
    let r_noise = ts_rows(sc_, TS_F_NOISE)
    let r_conds = ts_rows(sc_, TS_F_CONDS)
    let r_lat = ts_rows(sc_, TS_F_LATENTS)
    var xin = slab.rows
    var xoff = sl.bos
    if (f > 0l) {
        if (f <= forced_rows) {
            xin = ts_rows(sc_, TS_F_FORCED)
            xoff = (f - 1l) * ld
        } else {
            xin = r_x1
            xoff = 0l
        }
    }
    ts_pk_lin(rl, slab, sl.input_linear, ld, d, xin, xoff, r_x, 0l)
    for (li in range(length(sl.layers))) {
        ts_pk_fr_layer(rl, slab, sc_, m.backbone, sl.layers[li], li, p)
    }
    ts_ln(rl, slab, sl.out_norm, r_x, r_xb, 1l, d, m.backbone.eps)
    let cd = m.cond_dim
    let coff = f * cd
    if (sl.has_z) {
        ts_pk_lin(rl, slab, sl.z_proj, d, cd, r_xb, 0l, r_conds, coff)
    } else {
        let kc = GkPkRowsArgs(rows = 1u, width = uint(d), ss = uint(d), sc = 0u, sr0 = 0u, ds = uint(d), dc = 0u, dr0 = uint(f))
        ts_pk_rows(rl, r_xb, r_conds, kc, false)
    }
    ts_pk_lin(rl, slab, sl.out_eos, cd, 1l, r_conds, coff, ts_rows(sc_, TS_F_EOS), f)
    let noff = i * ld
    var s = ts_pk_site(r_conds, coff, r_hy)
    s.aoff = sl.time_const
    ts_pk_gemv(rl, slab, sl.cond_embed, cd, n, TTS_PK_GEMV_ADD_SILU, s)
    ts_pk_gemv(rl, slab, sl.input_proj, ld, n, TTS_PK_GEMV_DOT, ts_pk_site(r_noise, noff, r_hx))
    for (blk in sl.blocks) {
        ts_pk_gemv(rl, slab, blk.adaln, n, 3l * n, TTS_PK_GEMV_DOT, ts_pk_site(r_hy, 0l, r_hmod))
        var s1 = ts_pk_site(r_hx, 0l, r_hm)
        s1.mod = r_hmod
        s1.lwoff = blk.ln.gwoff
        s1.lboff = blk.ln.gboff
        ts_pk_gemv(rl, slab, blk.mlp1, n, n, TTS_PK_GEMV_LN_SILU, s1)
        var s2 = ts_pk_site(r_hm, 0l, r_hx)
        s2.mod = r_hmod
        ts_pk_gemv(rl, slab, blk.mlp2, n, n, TTS_PK_GEMV_GATE, s2)
    }
    ts_pk_gemv(rl, slab, sl.final_adaln, n, 2l * n, TTS_PK_GEMV_DOT, ts_pk_site(r_hy, 0l, r_hmod))
    var st = ts_pk_site(r_hx, 0l, r_x1)
    st.mod = r_hmod
    st.lwoff = sl.final_ln.gwoff
    st.lboff = sl.final_ln.gboff
    st.aoff = sl.std_mean
    st.x0 = r_noise
    st.x0off = noff
    st.lat = r_lat
    st.loff = f * ld
    st.x0mul = m.head.flow == PocketFlow.drifting ? 0.0 : 1.0
    ts_pk_gemv(rl, slab, sl.final_linear, n, ld, TTS_PK_GEMV_TAIL, st)
}

var private @scratch g_ts_pk_noise_host : array<float>
var private @scratch g_ts_pk_eos_host : array<float>

//! `pocket_frames_batched` here: the noise staged and uploaded ahead of each batch's submit, its EOS logits read back after
def private ts_pk_frames_loop(var rl : auto(R); m : PocketModel; sl : TtsPkFramesSlab; slab : TsSlab; sc_ : TsScratch; base, batch, max_frames, forced_rows : int64; std : float;
                              frames_after_eos : int; captured : array<float>; var sc : PocketScratch; var f0 : int64&) : tuple<frames : int64; eos_at : int64> {
    let ld = m.latent_dim
    let r_noise = ts_rows(sc_, TS_F_NOISE)
    let r_eos = ts_rows(sc_, TS_F_EOS)
    g_ts_pk_noise_host |> ensure_length(batch * ld)
    g_ts_pk_eos_host |> ensure_length(batch)
    return pocket_frames_batched(m, batch, max_frames, std, frames_after_eos, captured, sc, f0, $(_k : int64) : float? {
        return unsafe(addr(g_ts_pk_noise_host[0]))
    }, $(first, k : int64) : bool {
        ts_upload(rl, r_noise, g_ts_pk_noise_host, k * ld)
        _::rail_tts_shot(rl, "pocket frames", long_length(m.backbone.layers), k) {
            for (i in range64(k)) {
                ts_pk_frame(rl, slab, sc_, m, sl, base + first + i, first + i, i, forced_rows)
            }
        }
        ts_served(rl)
        var ep = unsafe(addr(g_ts_pk_eos_host[0]))
        _::rail_tts_take(rl, r_eos, first * 4l, ep, k * 4l)
        return true
    }, $(_first, i : int64) : float {
        return g_ts_pk_eos_host[i]
    })
}

[arch(at = "../ARCHITECTURE_GPU_TOWER_TTS.md#tts-pocket-seats")]
def tts_pk_frames(var rl : auto(R); m : PocketModel; var vs : PocketVoiceState; n_txt : int64; std : float; frames_after_eos : int;
                  captured, forced : array<float>; max_frames : int64; var sc : PocketScratch) : tuple<frames : int64; eos_at : int64> {
    let none = (frames = -1l, eos_at = -1l)
    let dev_rows = tts_pk_prompt_dev_take(g_ts_pk_prompt_dev, vs, g_ts_pk_voice.gen, sc.rows, n_txt, m.backbone.d)   //! spent on entry: a decline below must not leave it for a later chunk
    if (!ts_on(rl)) {
        ts_decline(rl, "frames", TowerDecline.knob)
        return none
    }
    if (m.devplan.n == 0l) {
        ts_decline(rl, "frames", TowerDecline.flavor)
        return none
    }
    let tf & = unsafe(m.backbone)
    let d = tf.d
    let ld = m.latent_dim
    let base = vs.len + n_txt
    let shape_ok = tts_pk_frames_ok(m, TS_PK_ATTN_HS, TS_PK_GEMV_MAX_NIN) $(l : TtsLinear) : bool {
        return tts_lin_ok4(l)   //! the row GEMV over the slab's f32 rows: the input on the 4 lattice, no GEMM tile
    }
    if (!tts_pk_frames_admit(m, vs, n_txt, max_frames, TS_PK_ATTN_MAX_KEYS) || !shape_ok) {
        ts_decline(rl, "frames", TowerDecline.shape)
        return none
    }
    if (!ts_arm_ensure(rl, "pocket frames", $ => ts_pk_frames_ensure(rl))) {
        ts_decline(rl, "frames", TowerDecline.device)
        return none
    }
    let batch = min(pocket_frame_batch(), max_frames)
    let forced_rows = long_length(forced) / ld
    let fl = ts_pk_frames_floats(m, max_frames, batch, forced_rows)
    if (!ts_pk_frames_attach(rl, m) || !ts_pk_voice_attach(rl, vs, tf) || !ts_scratch_ready(rl, g_ts_pkf_scratch, fl, 0l, 0l, max((max_frames + 1l) * d, max_frames * ld))) {
        ts_decline(rl, "frames", TowerDecline.memory)
        return none
    }
    for (li, kc in iter_range(vs.caches), vs.caches) {
        ts_pk_kv_upload(rl, kc, g_ts_pk_voice.k[li], g_ts_pk_voice.v[li], vs.len + dev_rows, base)
    }
    let sl & = g_ts_pk_frames_slots
    let sc_ & = g_ts_pkf_scratch
    let slab & = g_ts_pk_frames
    if (forced_rows > 0l) {
        ts_upload(rl, ts_rows(sc_, TS_F_FORCED), forced, forced_rows * ld)
    }
    var f0 = 0l
    let r = ts_pk_frames_loop(rl, m, sl, slab, sc_, base, batch, max_frames, forced_rows, std, frames_after_eos, captured, sc, f0)
    if (r.frames >= 0l) {
        ts_readback(rl, ts_rows(sc_, TS_F_LATENTS), sc.latents, r.frames * ld)
        ts_readback(rl, ts_rows(sc_, TS_F_CONDS), sc.conds, tts_pk_cond_rows(r.frames, f0) * m.cond_dim)
    }
    return r
}

var private g_ts_pkp_scratch : TsScratch
var private g_ts_pk_prompt_dev : TtsPkPromptDev

//! the prompt seat's slots: the rows form's five at the prompt's rows (the frames slots' indices, its own scratch)
def private ts_pk_prompt_floats(m : PocketModel; t : int64) : TsFloats {
    let d = m.backbone.d
    var fl : TsFloats
    fl[TS_F_X] = t * d
    fl[TS_F_XB] = t * d
    fl[TS_F_QKV] = t * tts_pk_qkv_width(m.backbone)
    fl[TS_F_CTX] = t * m.backbone.attn_dim
    fl[TS_F_H] = t * m.backbone.ffn
    return fl
}

//! the prompt's K/V rows [vs.len, vs.len + t) of every layer back from the voice slot into the host caches
def private ts_pk_prompt_readback(rl : auto(R); var vs : PocketVoiceState; sc_ : TsScratch; t, d, row0 : int64) {
    let v & = g_ts_pk_voice
    g_ts_pk_kv_host |> ensure_length(2l * t * d)
    unsafe {
        var kp = addr(g_ts_pk_kv_host[0])
        var vp = addr(g_ts_pk_kv_host[t * d])
        for (li, kc in iter_range(vs.caches), vs.caches) {
            _::rail_tts_take(rl, v.k[li], row0 * d * 4l, kp, t * d * 4l)
            _::rail_tts_take(rl, v.v[li], row0 * d * 4l, vp, t * d * 4l)
            kv_cache_append_rows(kc, kp, vp, t, d)
        }
    }
}

//! `rows` [n][d] through the backbone at pos0.., keys and values into the voice slot and the host caches; slab, slot and scratch attached
def private ts_pk_rows_into_voice(var rl : auto(R); m : PocketModel; var vs : PocketVoiceState; rows : array<float>; n, pos0 : int64; shot : string) {
    let tf & = unsafe(m.backbone)
    let sc_ & = g_ts_pkp_scratch
    ts_upload(rl, ts_rows(sc_, TS_F_X), rows, n * tf.d)
    _::rail_tts_shot(rl, shot, long_length(tf.layers), n) {
        ts_pk_voice_tf(rl, m, n, pos0)
    }
    ts_served(rl)
    ts_pk_prompt_readback(rl, vs, sc_, n, tts_pk_kv_width(tf), pos0)
}

[arch(at = "../ARCHITECTURE_GPU_TOWER_TTS.md#tts-pocket-seats")]
def tts_pk_prompt(var rl : auto(R); m : PocketModel; var vs : PocketVoiceState; n_txt : int64; var sc : PocketScratch) : bool {
    //! the text prompt's rows through the backbone after the voice's: the frames slab's layers in the rows form at
    //! positions vs.len.., the keys and values into the voice slot and back to the host caches, the slot's rows left for
    //! the frames call after it
    if (!ts_on(rl)) {
        return ts_decline(rl, "prompt", TowerDecline.knob)
    }
    if (m.devplan.n == 0l) {
        return ts_decline(rl, "prompt", TowerDecline.flavor)
    }
    let tf & = unsafe(m.backbone)
    let d = tf.d
    let shape_ok = tts_pk_frames_ok(m, TS_PK_ATTN_HS, TS_PK_GEMV_MAX_NIN) $(l : TtsLinear) : bool {
        return tts_lin_ok4(l)
    }
    if (!tts_pk_prompt_admit(m, vs, n_txt, TS_PK_ATTN_MAX_KEYS) || !shape_ok) {
        return ts_decline(rl, "prompt", TowerDecline.shape)
    }
    if (!ts_arm_ensure(rl, "pocket prompt", $ => ts_pk_codec_ensure(rl))) {
        return ts_decline(rl, "prompt", TowerDecline.device)
    }
    let fl = ts_pk_prompt_floats(m, n_txt)
    if (!ts_pk_frames_attach(rl, m) || !ts_pk_voice_attach(rl, vs, tf) || !ts_scratch_ready(rl, g_ts_pkp_scratch, fl, 0l, 0l, n_txt * d)) {
        return ts_decline(rl, "prompt", TowerDecline.memory)
    }
    ts_pk_rows_into_voice(rl, m, vs, sc.rows, n_txt, vs.len, "pocket prompt")
    tts_pk_prompt_dev_set(g_ts_pk_prompt_dev, vs, g_ts_pk_voice.gen, sc.rows, n_txt, d)
    return true
}

//! the backbone's layers in the rows form over the prompt scratch's `n` rows at positions pos0.., the keys and values into the voice slot
def private ts_pk_voice_tf(var rl : auto(R); m : PocketModel; n, pos0 : int64) {
    let tf & = unsafe(m.backbone)
    let sl & = g_ts_pk_frames_slots
    let v & = g_ts_pk_voice
    let f = TsPkRowsForm(pos0 = pos0, kv_row0 = pos0, tabs = v.rope, coff = 0l, soff = v.sin_off, zero = sl.zero, kv_only_last = true)
    ts_pk_rows_tf(rl, g_ts_pk_frames, g_ts_pkp_scratch, tf, sl.layers, n, f, TS_F_X, TS_F_XB, TS_F_QKV, TS_F_CTX, TS_F_H) $(li : int) {
        return (k = v.k[li], v = v.v[li])
    }
}

[cold_path, arch(at = "../ARCHITECTURE_GPU_TOWER_TTS.md#tts-pocket-seats")]
def tts_pk_voice(var rl : auto(R); m : PocketModel; lat : array<float>; frames : int64; var vs : PocketVoiceState) : bool {
    if (!ts_on(rl)) {
        return ts_decline(rl, "voice", TowerDecline.knob)
    }
    if (m.devplan.n == 0l) {
        return ts_decline(rl, "voice", TowerDecline.flavor)
    }
    let tf & = unsafe(m.backbone)
    let d = tf.d
    let ld = m.latent_dim
    let n = vs.len
    let bos = n - frames
    let shape_ok = tts_pk_frames_ok(m, TS_PK_ATTN_HS, TS_PK_GEMV_MAX_NIN) $(l : TtsLinear) : bool {
        return tts_lin_ok4(l)
    }
    if (!shape_ok || !tts_gemm_lin_ok(m.speaker_proj) || frames < 1l || bos < 0l || bos > 1l || long_length(lat) < frames * ld || empty(vs.caches)
        || length(vs.caches) != length(tf.layers) || n > vs.caches[0].cap || vs.caches[0].cap > TS_PK_ATTN_MAX_KEYS) {
        return ts_decline(rl, "voice", TowerDecline.shape)
    }
    if (!ts_arm_ensure(rl, "pocket voice", $ => ts_pk_codec_ensure(rl))) {
        return ts_decline(rl, "voice", TowerDecline.device)
    }
    let fl = ts_pk_prompt_floats(m, n)
    vs.len = 0l   // the slot is built over the empty caches: no host row to upload
    let ready = ts_pk_frames_attach(rl, m) && ts_pk_voice_rebuild(rl, vs, tf) && ts_scratch_ready(rl, g_ts_pkp_scratch, fl, 0l, 0l, n * d)
    vs.len = n
    if (!ready) {
        return ts_decline(rl, "voice", TowerDecline.memory)
    }
    let sl & = g_ts_pk_frames_slots
    let sc_ & = g_ts_pkp_scratch
    let slab & = g_ts_pk_frames
    let r_x = ts_rows(sc_, TS_F_X)
    let r_lat = ts_rows(sc_, TS_F_H)   // the ffn's rows hold the latents until the projection has read them
    ts_upload(rl, r_lat, lat, frames * ld)
    _::rail_tts_shot(rl, "pocket voice", long_length(tf.layers), n) {
        ts_lin(rl, slab, sl.spk, ld, d, r_lat, r_x, frames, true, 0l, bos * d)
        if (bos > 0l) {
            ts_pk_rows(rl, slab.rows, r_x, GkPkRowsArgs(rows = uint(d), width = 1u, ss = 1u, sc = 0u, sr0 = uint(sl.bos_voice), ds = 1u, dc = 0u, dr0 = 0u), false)
        }
        ts_pk_voice_tf(rl, m, n, 0l)
    }
    ts_served(rl)
    vs.len = 0l   // the readback appends at the caches' own length
    ts_pk_prompt_readback(rl, vs, sc_, n, tts_pk_kv_width(tf), 0l)
    vs.len = n
    g_ts_pk_voice.meta = tts_pk_voice_meta(vs, tts_pk_kv_width(tf))
    return true
}
