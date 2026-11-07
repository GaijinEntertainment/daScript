options gen2
options indenting = 4
options _dasllama_internal = true

//! The CUDA home's tower seat plumbing, one text for every family's seat on this home: the knob, the counters and the declines by
//! reason, the resident record with every plane a tower chain binds, its growth, uploads and release, the cm2 picks, the schedule
//! staging, the rail's seats and the served readback (`ARCHITECTURE_GPU_TOWER_RAIL.md#tower-rail`). The vision seats
//! (`dasllama_cuda_tower`) and the audio families' seats require it; the model drop and the shutdown release every resident a
//! seat module registers here.
module dasllama_cuda_tower_seat shared public

require dasllama/dasllama_lint public
require math
require dasllama/dasllama_kqformat
require dasllama/dasllama_math   // grow_resize
require dasllama/dasllama_plane
require dasllama/dasllama_tower
require dasllama/dasllama_env   // g_env_vulkan.cuda_graphs
require dasllama/dasllama_gpu_tier  // gpu_want_arms_tier, TILE_READ_SLACK, GpuDeclineCounter
require dasllama/dasllama_gpu_math   // cm2_tiles_at
require dasllama/dasllama_gpu_kernels_discrete
require dasllama/dasllama_cuda_common
require dasllama/dasllama_cuda_classes
require dasllama/dasllama_cuda_decode   // cud_cm2_pick: the window GEMM's column and k split, register_cud_drop_hook
require dasllama/dasllama_cuda_rail

var private g_cut_on = true
var private g_cut_encodes = 0l
var private g_cut_rows = 0l
var private g_cut_blocks = 0l
var private g_cut_stem_encodes = 0l   //! the stems and fronts served on the device, one an encode
var private g_cut_tail_encodes = 0l
var private g_cut_mels = 0l   //! the log-mels served on the device
var private g_cut_declines = GpuDeclineCounter(home = "cuda")
var private g_cut_releases : array<function<() : void>>   //! every seat module's resident release, the shutdown's sweep

let CUT_HOME = "cu"   //! this home's device-block pieces' prefix (`tower_family_piece`, `tower_fc_piece`)

//! Engage / decline counters; `blocks` = transformer blocks the driver ran. Model-level gates assert DELTAS on these.
def cuda_tower_stats : tuple<encodes : int64; rows : int64; blocks : int64; declines : int64; convs : int64; mels : int64> =>
    (encodes = g_cut_encodes, rows = g_cut_rows, blocks = g_cut_blocks, declines = g_cut_declines.total, convs = g_cut_stem_encodes, mels = g_cut_mels)

//! The declines-by-reason table (a copy), keyed by `TowerDecline` value names.
def cuda_tower_declines : table<string; int64> => <- clone_to_move(g_cut_declines.counts)

//! Test A/B seat: pin the tower driver on / off in-process.
def set_cuda_tower(on : bool) {
    g_cut_on = on
}

def cuda_tower_on : bool => g_cut_on

//! the encodes whose projector tail the device ran (the soft tokens read back, the blocks' rows never)
def cuda_tower_tail_encodes : int64 => g_cut_tail_encodes

//! the tier's want (DASLLAMA_GPU) and the device: the gate every chain and every twin cell reads first
def cuda_tower_arms : bool => gpu_want_arms_tier() && cud_init()

//! a stem or front the device served for one encode
def cut_count_stem {
    g_cut_stem_encodes++
}

//! a projector tail the device served for one encode
def cut_count_tail {
    g_cut_tail_encodes++
}

//! a front the device served over `n` dispatches of it (qwen3a's chunks)
def cut_count_stems(n : int64) {
    g_cut_stem_encodes += n
}

//! a log-mel the device served
def cut_count_mel {
    g_cut_mels++
}

let CUT_MAX_ENCODE_ROWS = 4096l
let private CUT_SPLITK_MIN_ROWS = 0l   //! the encode rows from which the fc2 GEMM may take the split-k pick, by default (no floor)
var private g_cut_splitk_min_rows = CUT_SPLITK_MIN_ROWS
let private CUT_SPLITK_FILL = 4l   //! the fc2 site splits where its whole grid times this fits the card's block slots, by default
var private g_cut_splitk_fill = CUT_SPLITK_FILL

//! Test A/B seat: the fc2 site takes `cud_cm2_pick`'s k split only where its whole pick's workgroups times `fill` fit the
//! card's block slots (0: no bound, the pick's own rule); answers the value it replaced
def set_cuda_tower_splitk_fill(fill : int64) : int64 {
    let was = g_cut_splitk_fill
    g_cut_splitk_fill = max(fill, 0l)
    if (g_cut_splitk_fill != was) {
        to_log(LOG_INFO, "dasLLAMA cuda tower: set_cuda_tower_splitk_fill({g_cut_splitk_fill}) - the fc2 site's k split where its whole grid times {g_cut_splitk_fill} fits the block slots (0: no bound; the default {CUT_SPLITK_FILL})\n")
    }
    return was
}

def cuda_tower_splitk_fill : int64 => g_cut_splitk_fill

//! Test A/B seat: the encode rows from which the fc2 site may take `cud_cm2_pick`'s k split (0, the default: no floor - the fill
//! bound alone gates it); answers the floor it replaced
def set_cuda_tower_splitk_min_rows(rows : int64) : int64 {
    let was = g_cut_splitk_min_rows
    g_cut_splitk_min_rows = max(rows, 0l)
    if (g_cut_splitk_min_rows != was) {
        to_log(LOG_INFO, "dasLLAMA cuda tower: set_cuda_tower_splitk_min_rows({g_cut_splitk_min_rows}) - the fc2 site's k split from {g_cut_splitk_min_rows} encode rows (the default {CUT_SPLITK_MIN_ROWS})\n")
    }
    return was
}

def cuda_tower_splitk_min_rows : int64 => g_cut_splitk_min_rows

var private g_cut_cm2_col = 0l   //! `set_cuda_tower_cm2_col`'s column, 0 = none pinned

//! Test A/B seat: the column every tower Q8_0 cm2 site takes - 32, 128 or 256, over DASLLAMA_CM2_TILE; 0 (or any other
//! value) puts back the knob's column and the wave model's pick; answers the column it replaced
def set_cuda_tower_cm2_col(tc : int64) : int64 {
    let was = g_cut_cm2_col
    g_cut_cm2_col = tc == 32l || tc == 128l || tc == 256l ? tc : 0l
    if (g_cut_cm2_col != was) {
        to_log(LOG_INFO, "dasLLAMA cuda tower: set_cuda_tower_cm2_col({g_cut_cm2_col}) - every tower Q8_0 cm2 site on the column it names (0: each site's own)\n")
    }
    return was
}

def cuda_tower_cm2_col : int64 => g_cut_cm2_col

let CUT_DECLINE_WORDS_DEVICE = "the home has no device (DASLLAMA_GPU=1 DASLLAMA_CUDA=1 arm it), or a class failed to build"
let CUT_DECLINE_WORDS_TOWER_KNOB = "the tower knob is off (set_cuda_tower(false))"
let CUT_DECLINE_WORDS_MEMORY = "a device or pinned allocation failed"

struct CutBuf {
    dev : uint64
    bytes : int64
}

//! a family's residency: the planes bound once a model (`key`), the per-encode scratch grown to the widest encode served
struct CutResident {
    family : string
    key : uint64
    oom : bool
    served_said : bool
    picks_said : table<int64>   //! the encode row counts whose cm2 picks the log has named
    wq : CutBuf         //! the q8 lane's gathered quants and halfword scales - the image's block pieces as stored
    ws : CutBuf
    norms : CutBuf       //! the f32 blob as stored, a ones row of `d` behind it (the weightless rms)
    ones_off : int64
    wb : CutBuf          //! the halfword plane (bf16 or f16 rows) as stored, where the family has one
    sched : CutBuf       //! the encode's batch GEMM schedule: the records and their workgroup maps
    cap_rows : int64
    x : CutBuf
    xb : CutBuf
    xc : CutBuf
    xh : CutBuf          //! the cm2 tiles' f16 feed, [cap + slack x max(d, ff)]
    q : CutBuf
    k : CutBuf
    v : CutBuf
    q2 : CutBuf
    k2 : CutBuf
    v2 : CutBuf
    k16 : CutBuf
    v16 : CutBuf
    qp : CutBuf          //! the padded attention panels
    kp16 : CutBuf
    vp16 : CutBuf
    attp : CutBuf
    att : CutBuf
    yo : CutBuf
    hg : CutBuf
    hu : CutBuf
    yd : CutBuf
    tabs : CutBuf
    qkv : CutBuf
    cos : CutBuf
    sin : CutBuf
    splitk_parts : CutBuf
    planes : CutBuf      //! a stem's input: the canvas's pixel planes, a front's mel rows
    col : CutBuf
    win_sort : CutBuf
    pos_resized : CutBuf
    win_table : CutBuf
    tail_out : CutBuf    //! the tail rows at the tail's widest width, and the projected rows the host reads back
    tail_proj : CutBuf
    tail_hid : CutBuf    //! a projector tail's hidden rows and gate rows
    tail_gate : CutBuf
    stage1 : CutBuf      //! a front's first conv stage's rows
    fw16 : CutBuf        //! a front's halfword conv weight slab and its f32 slab (the conv biases, the position rows)
    fnorm : CutBuf
    front : bool         //! the front's slabs uploaded for the model the key names
}

[cold_path]
def private cut_decline_words(why : TowerDecline) : string
    => tower_decline_words(why, "CUDA", CUT_DECLINE_WORDS_TOWER_KNOB, CUT_DECLINE_WORDS_DEVICE, CUT_DECLINE_WORDS_MEMORY)

[cold_path]
def cut_decline(r : CutResident; why : TowerDecline) : bool
    => g_cut_declines |> note("the {r.family} tower", "{why}", "the tower stays on the CPU chain: {cut_decline_words(why)}")

//! the gate a seat reads first: the knob, then the lane (`served_lane`) and the CUDA-flavored image (`dev`); false with the decline counted
def cut_gate(r : CutResident; served_lane : bool; dev : TowerDev) : bool {
    return cut_decline(r, TowerDecline.knob) if (!g_cut_on)
    return cut_decline(r, TowerDecline.quant_mode) if (!served_lane)
    return dev == TowerDev.cuda || cut_decline(r, TowerDecline.flavor)
}

def cut_arm(r : CutResident) : bool {
    if (!cuda_tower_arms()) {
        return cut_decline(r, TowerDecline.device)
    }
    cud_bind()
    return true
}

[cold_path]
def cut_announce(var r : CutResident) {
    r.served_said = true
    to_log(LOG_INFO, "dasLLAMA cuda tower: {r.family} serves on the device (DASLLAMA_GPU=1 DASLLAMA_CUDA=1 arm the home)\n")
}

//! a chain's captured launches: the dry walk's hash it was captured under and the executable graph
struct private CutGraph {
    walk : uint64
    exec : void?
}

var private g_cut_graphs : table<uint64; CutGraph>   //! the captured chains by their rail key; a key seen once holds no graph
let private CUT_GRAPH_CAP = 32   //! the captured chains kept: past it the cache starts over

var private g_cut_graph_on = true   //! the chains replay from captured graphs (`set_cuda_tower_graphs`)
var private g_cut_graphs_off_said = false

//! every captured chain destroyed
def cut_graphs_drop {
    for (g in values(g_cut_graphs)) {
        cuda_graph_destroy(g.exec)
    }
    g_cut_graphs |> clear()
}

//! Test A/B seat: the tower chains replayed from captured graphs, or launched kernel by kernel; answers the value it replaced
def set_cuda_tower_graphs(on : bool) : bool {
    let was = g_cut_graph_on
    g_cut_graph_on = on
    cut_graphs_drop() if (!on)
    if (on != was) {
        to_log(LOG_INFO, "dasLLAMA cuda tower: set_cuda_tower_graphs({on}) - the tower chains {on ? "replay from captured graphs" : "launch kernel by kernel"} (the default replays them)\n")
    }
    return was
}

def cuda_tower_graphs_on : bool => g_cut_graph_on

//! `b` holding at least `need` bytes: grown by a fresh allocation (the old bytes dropped), a refusal marked on `r.oom`
def cut_grow(var r : CutResident; var b : CutBuf; need : int64) {
    cut_realloc(r, b, need) if (need > b.bytes && !r.oom)
}

//! the fresh allocation drops every captured graph: a graph's launches name the freed address
[cold_path]
def private cut_realloc(var r : CutResident; var b : CutBuf; need : int64) {
    cut_graphs_drop()
    cuda_mem_free(b.dev)
    b.dev = cuda_mem_alloc(uint64(max(need, 64l)))
    b.bytes = b.dev == 0ul ? 0l : max(need, 64l)
    r.oom = r.oom || b.dev == 0ul
}

def cut_free(var b : CutBuf) {
    cuda_mem_free(b.dev)
    b.dev = 0ul
    b.bytes = 0l
}

def cut_up(var r : CutResident; var b : CutBuf; src : void?; bytes : int64) {
    cut_grow(r, b, bytes)
    return if (r.oom || bytes <= 0l)
    cut_htod(b.dev, src, bytes)
}

//! `bytes` from the host at `src` to the device at `dev` on the home's stream
def cut_htod(dev : uint64; src : void?; bytes : int64) {
    cuda_memcpy_htod(dev, src, uint64(bytes), g_cud.stream)
}

//! `bytes` from the device at `dev` to the host at `dst` on the home's stream
def cut_dtoh(dst : void?; dev : uint64; bytes : int64) {
    cuda_memcpy_dtoh(dst, dev, uint64(bytes), g_cud.stream)
}

//! the stream's work waited on, a failure the seat's panic naming `who`
def cut_sync(who : string) {
    if (!cuda_stream_synchronize(g_cud.stream)) {
        panic("dasLLAMA cuda tower: the {who} chain failed on the device - {cuda_last_error()}")
    }
}

[cold_path]
def cut_release(var r : CutResident) {
    cut_free(r.wq)
    cut_free(r.ws)
    cut_free(r.norms)
    cut_free(r.wb)
    cut_free(r.sched)
    cut_free(r.x)
    cut_free(r.xb)
    cut_free(r.xc)
    cut_free(r.xh)
    cut_free(r.q)
    cut_free(r.k)
    cut_free(r.v)
    cut_free(r.q2)
    cut_free(r.k2)
    cut_free(r.v2)
    cut_free(r.k16)
    cut_free(r.v16)
    cut_free(r.qp)
    cut_free(r.kp16)
    cut_free(r.vp16)
    cut_free(r.attp)
    cut_free(r.att)
    cut_free(r.yo)
    cut_free(r.hg)
    cut_free(r.hu)
    cut_free(r.yd)
    cut_free(r.tabs)
    cut_free(r.qkv)
    cut_free(r.cos)
    cut_free(r.sin)
    cut_free(r.splitk_parts)
    cut_free(r.planes)
    cut_free(r.col)
    cut_free(r.win_sort)
    cut_free(r.pos_resized)
    cut_free(r.win_table)
    cut_free(r.tail_out)
    cut_free(r.tail_proj)
    cut_free(r.tail_hid)
    cut_free(r.tail_gate)
    cut_free(r.stage1)
    cut_free(r.fw16)
    cut_free(r.fnorm)
    cut_graphs_drop()
    r.key = 0ul
    r.cap_rows = 0l
    r.oom = false
    r.served_said = false
    r.front = false
    r.picks_said |> clear()
    g_cut_declines |> unsay("the {r.family} tower")
}

//! a seat module's residents released at the model drop's sweep and the shutdown (`cuda_tower_shutdown`)
def cut_on_release(fn : function<() : void>) {
    g_cut_releases |> push(fn)
}

//! Every resident released: the model drop's sweep and the shutdown
def cuda_tower_shutdown {
    return if (!g_cud.ready)
    cud_bind()
    cuda_stream_synchronize(g_cud.stream)
    for (fn in g_cut_releases) {
        invoke(fn)
    }
}

//! `k` folded with the halfword plane `wblob`: its address, size and sampled words
def cut_key_wblob(k : uint64; wblob : PlaneU16) : uint64 {
    let kw = hash_combine64(hash_combine64(k, intptr(unsafe(reinterpret<void?>(wblob.p)))), uint64(wblob.n))
    return tower_fold_words(kw, unsafe(reinterpret<uint8 const?>(wblob.p)), wblob.n * 2l)
}

//! the resident key folds every model field the resident's extent follows: the planes' addresses, sizes and sampled words, the layer count
def cut_key(m; n_layer : int64) : uint64 {
    var k = hash_combine64(0x6b0dul, uint64(n_layer))
    k = hash_combine64(k, intptr(unsafe(reinterpret<void?>(m.blob.p))))
    k = hash_combine64(k, uint64(m.blob.n))
    k = tower_fold_words(k, unsafe(reinterpret<uint8 const?>(m.blob.p)), m.blob.n * 4l)
    k = cut_key_wblob(k, m.wblob)
    static_if (typeinfo safe_has_field < dev > (m)) {
        k = hash_combine64(k, intptr(unsafe(reinterpret<void?>(m.dev.p))))
        k = hash_combine64(k, uint64(m.dev.n))
        k = tower_fold_words(k, m.dev.p, m.dev.n)
    }
    return k
}

//! a CUDA-flavored model's key over its device block (address, size, plan, sampled words), the f32 rows of `blob` beside it and the layer count
def cut_key_dev(dev : PlaneU8; devplan : PlaneI64; blob : PlaneF; n_layer : int64) : uint64 {
    var k = hash_combine64(hash_combine64(0x6b0dul, uint64(n_layer)), intptr(unsafe(reinterpret<void?>(dev.p))))
    k = hash_combine64(k, uint64(dev.n))
    k = tower_fold_words(k, unsafe(reinterpret<uint8 const?>(devplan.p)), devplan.n * 8l)
    k = tower_fold_words(k, dev.p, dev.n)
    k = hash_combine64(k, intptr(unsafe(reinterpret<void?>(blob.p))))
    k = hash_combine64(k, uint64(blob.n))
    return tower_fold_words(k, unsafe(reinterpret<uint8 const?>(blob.p)), blob.n * 4l)
}

var private @scratch @exact_size g_cut_ones : array<float>

//! the f32 blob with `ones_n` ones behind it and the halfword plane, as stored; false with the resident released on a refused allocation
[cold_path]
def cut_upload_planes(var r : CutResident; key : uint64; blob : PlaneF; wblob : PlaneU16; ones_n : int64) : bool {
    r.key = key
    r.oom = false
    r.ones_off = blob.n
    cut_grow(r, r.norms, (blob.n + ones_n) * 4l)
    cut_up(r, r.norms, unsafe(reinterpret<void?>(blob.p)), blob.n * 4l)
    if (!r.oom && ones_n > 0l) {
        g_cut_ones |> grow_resize(ones_n)
        for (o in g_cut_ones) {
            o = 1.0
        }
        cuda_memcpy_htod(r.norms.dev + uint64(blob.n * 4l), unsafe(addr<void?>(g_cut_ones[0])), uint64(ones_n * 4l), g_cud.stream)
    }
    if (wblob.n > 0l) {
        cut_up(r, r.wb, unsafe(reinterpret<void?>(wblob.p)), wblob.n * 2l)
    }
    if (r.oom) {
        cut_release(r)
        return false
    }
    return true
}

//! the device-block piece `name` of `m` into `b`, as stored; a missing piece panics - the block was baked by another driver
[cold_path, arch(at = "../ARCHITECTURE_IMAGE_GPU_PLANES.md#tower-dev-block")]
def cut_put_piece(var r : CutResident; var b : CutBuf; m; name : string) {
    let p = tower_dev_find(m.devplan, name)
    if (p.off < 0l) {
        panic("dasLLAMA cuda tower: the {r.family} CUDA image carries no `{name}` piece - its block was baked by another driver")
    }
    cut_up(r, b, unsafe(reinterpret<void?>(plane_at(m.dev, p.off))), p.bytes)
}

//! the q8 lane's `<prefix>.wq` / `<prefix>.ws` pieces as stored; false with the resident released on a refused allocation
[cold_path, arch(at = "../ARCHITECTURE_IMAGE_GPU_PLANES.md#tower-dev-block")]
def cut_upload_q8(var r : CutResident; m; prefix : string) : bool {
    cut_put_piece(r, r.wq, m, "{prefix}.wq")
    cut_put_piece(r, r.ws, m, "{prefix}.ws")
    if (r.oom) {
        cut_release(r)
        return false
    }
    return true
}

//! the per-encode scratch at `npos` rows - `dp` 0: no padded attention panels (the compact rows attend), `tail_dim` 0: no tail rows; false counted `memory`
def cut_scratch(var r : CutResident; npos, d, ff, dp, tail_dim : int64) : bool {
    let cap = tower_cap_rows(npos)
    r.cap_rows = max(r.cap_rows, cap)
    let c = r.cap_rows
    let bx = c * d * 4l
    let bff = c * ff * 4l
    cut_grow(r, r.x, bx)
    cut_grow(r, r.xb, bx)
    cut_grow(r, r.xh, (c + TILE_READ_SLACK) * max(d, ff) * 2l)
    cut_grow(r, r.q, bx)
    cut_grow(r, r.k, bx)
    cut_grow(r, r.v, bx)
    cut_grow(r, r.att, bx)
    cut_grow(r, r.yo, bx)
    cut_grow(r, r.hg, bff)
    cut_grow(r, r.hu, bff)
    cut_grow(r, r.yd, bx)
    if (dp > 0l) {
        cut_grow(r, r.qp, c * dp * 4l)
        cut_grow(r, r.kp16, c * dp * 2l)
        cut_grow(r, r.vp16, c * dp * 2l)
        cut_grow(r, r.attp, c * dp * 4l)
    }
    if (tail_dim > 0l) {
        cut_grow(r, r.tail_out, c * tail_dim * 4l)
        cut_grow(r, r.tail_proj, c * tail_dim * 4l)
    }
    return !r.oom || cut_decline(r, TowerDecline.memory)
}

def private cut_cm2_waves(wgs, slots : int64) : int64 => (wgs + slots - 1l) / slots

//! the column the seam or DASLLAMA_CM2_TILE forces on every tower Q8_0 cm2 site, 0 where none is forced
def private cut_cm2_forced : int64 {
    return g_cut_cm2_col if (g_cut_cm2_col != 0l)
    let forced = g_env_vulkan.cm2_tile
    rail_cm2_tile_say(forced) if (forced > 0l && !rail_cm2_tile_said())
    return forced == 32l || forced == 128l || forced == 256l ? forced : 0l
}

[arch(at="../ARCHITECTURE_GPU_ASYMMETRIES_CUDA.md#cuda-cm2-column")]
def cut_cm2_tc(d, rows : int64) : int64 {
    let forced = cut_cm2_forced()
    return forced if (forced != 0l)
    return 32l if (rows <= 64l)
    return 128l if (rows <= 128l)
    let slots = 2l * int64(g_cud.sm_count)
    return 256l if (slots == 0l)
    var tc = 128l
    var best = cut_cm2_waves(cm2_tiles_at(d, rows, 128l), slots) * 10l
    let s = cut_cm2_waves(cm2_tiles_at(d, rows, 32l), slots) * 6l
    if (s < best) {
        tc = 32l
        best = s
    }
    let l = cut_cm2_waves(cm2_tiles_at(d, rows, 256l), slots) * (rows % 256l == 0l ? 16l : 20l)
    return l < best ? 256l : tc
}

//! the pick at column `tc` with `nsplit` k planes of `ksplit` for a [rows x d] output
def private cut_pick_of(d, rows, tc, nsplit, ksplit : int64) : CutPick {
    let tci = tc == 256l ? CUD_CM2_TC_L : (tc == 128l ? CUD_CM2_TC_M : CUD_CM2_TC_S)
    return CutPick(tc = tc, tci = tci, nsplit = nsplit, ksplit = ksplit, groups = cm2_tiles_at(d, rows, tc) * nsplit)
}

//! the cm2 pick for a [rows x d] output, whole in k: the wave model's column (`cut_cm2_tc`), a seat with no split planes
def cut_pick_whole(d, rows : int64) : CutPick => cut_pick_of(d, rows, cut_cm2_tc(d, rows), 1l, 0l)

//! the whole pick's grid at column `tc` leaves enough block slots idle for a k split to pay its reduce (`set_cuda_tower_splitk_fill`); a forced split count (`DASLLAMA_CM2_SPLITK`) always splits
[arch(at="../ARCHITECTURE_GPU_ASYMMETRIES_CUDA.md#cuda-cm2-column")]
def private cut_split_pays(d, rows, tc : int64) : bool => (g_env_vulkan.cm2_splitk > 0l || g_cut_splitk_fill == 0l
    || cm2_tiles_at(d, rows, tc) * g_cut_splitk_fill <= 2l * int64(g_cud.sm_count))

//! the k split at a forced column `tc` the window's pick would widen: planes up to the block slots its grid leaves idle
def private cut_split_held(d, rows, n, tc : int64) : tuple<tc : int64; nsplit : int64; ksplit : int64> {
    let sk = cm2_split_fit(n, cud_split_want(cm2_tiles_at(d, rows, tc)))
    return (tc = tc, nsplit = sk, ksplit = sk > 1l ? cm2_chunk(n, sk) : 0l)
}

let private CUT_SPLITK_SCRATCH = 9_223_372_036_854_775_807l   //! a tower site's split planes at the pick: unbounded - `cut_pick` grows its own planes to the split, or picks whole where they do not fit

//! the cm2 pick for [rows x d] over `n`, k split where `split_ok`, the rows reach the floor (`set_cuda_tower_splitk_min_rows`)
//! and the split pays (`cut_split_pays`); a whole pick keeps the wave model's column, never the widened one, and a forced
//! column holds split or whole
def cut_pick(var r : CutResident; d, rows, n : int64; split_ok : bool) : CutPick {
    let tc = cut_cm2_tc(d, rows)
    if (!split_ok || rows < g_cut_splitk_min_rows || !cut_split_pays(d, rows, tc)) {
        return cut_pick_of(d, rows, tc, 1l, 0l)
    }
    var pk = cud_cm2_pick(d, rows, n, tc, CUT_SPLITK_SCRATCH)
    if (pk.tc != tc && cut_cm2_forced() != 0l) {
        pk = cut_split_held(d, rows, n, tc)
    }
    if (pk.nsplit > 1l) {
        cut_grow(r, r.splitk_parts, pk.nsplit * r.cap_rows * d * 4l)
        if (r.oom) {
            r.oom = false
            return cut_pick_whole(d, rows)
        }
    }
    return cut_pick_of(d, rows, pk.tc, pk.nsplit, pk.ksplit)
}

def cut_pick_ensure(p : CutPick) : bool => cud_cm2_ensure(int(KqFmt.q8), p.tci) && (p.nsplit == 1l || ensure_splitk_reduce_cls())

//! GEMM site `site`'s cm2 pick on the rail for a [rows x d] output over `n`, its classes built; false where one did not build
def cut_site_pick(var rl : CutRail; var r : CutResident; site : RailSite; d, rows, n : int64; split_ok : bool) : bool {
    rl.sites[int(site)] = cut_pick(r, d, rows, n, split_ok)
    return cut_pick_ensure(rl.sites[int(site)])
}

[cold_path]
def cut_pick_say(var r : CutResident; npos : int64; pq, pff, pfc2 : CutPick) {
    r.picks_said |> insert(npos)
    to_log(LOG_INFO, "dasLLAMA cuda tower: {r.family} at {npos} rows - {cut_pick_words("d", pq)}, {cut_pick_words("ff", pff)}, {cut_pick_words("fc2", pfc2)}\n")
}

[cold_path]
def private cut_pick_words(name : string; p : CutPick) : string {
    let split = p.nsplit > 1l ? " split {p.nsplit}" : ""
    return "{name} column {p.tc}{split}"
}

var @scratch @exact_size g_cut_sched : array<uint>   //! the encode's schedule staging
var @scratch @exact_size g_cut_ln_offs : array<LayerOffs>   //! the pre-LN chain's block offsets, one record a block
var g_cut_rl : CutRail   //! the encode's rail: every seat refills it, its schedule maps kept across encodes

//! the encode's schedule staged: `nrec` records at its head, `map_words` of maps behind them
def cut_sched_begin(nrec, map_words : int64) {
    g_cut_sched |> grow_resize(nrec * 4l + map_words)
}

def cut_sched_up(var r : CutResident) : bool {
    cut_up(r, r.sched, unsafe(addr<void?>(g_cut_sched[0])), long_length(g_cut_sched) * 4l)
    return !r.oom || cut_decline(r, TowerDecline.memory)
}

//! the rail's whole state under `tag` and `shape`: the planes' seats, the sites' picks and binds, the lane and the schedule maps
def private cut_rail_key(rl : CutRail; tag : string; shape : int64) : uint64 {
    var k = hash_combine64(hash(tag), uint64(shape))
    k = hash_combine64(hash_combine64(k, uint64(rl.lane)), uint64(rl.norms_base))
    k = hash_combine64(hash_combine64(k, uint64(rl.ones_off)), uint64(rl.cap_rows))
    for (b in rl.bufs) {
        k = hash_combine64(hash_combine64(hash_combine64(k, b.dev), uint64(b.off)), uint64(b.bytes))
    }
    for (p in rl.sites) {
        k = hash_combine64(hash_combine64(hash_combine64(k, uint64(p.tc)), uint64(p.nsplit)), uint64(p.ksplit))
    }
    for (b in rl.binds) {
        k = hash_combine64(hash_combine64(hash_combine64(k, uint64(b.wq)), uint64(b.ws)), uint64(b.meta))
        k = hash_combine64(hash_combine64(hash_combine64(k, uint64(b.xh)), uint64(b.meta_off)), uint64(b.fmt))
    }
    for (m in rl.maps) {
        k = hash_combine64(k, uint64(m))
    }
    return k
}

//! a rail key's first sight: remembered with no graph, the cache started over past its cap
[cold_path]
def private cut_graph_seen(key : uint64) {
    cut_graphs_drop() if (length(g_cut_graphs) >= CUT_GRAPH_CAP)
    g_cut_graphs |> insert(key, CutGraph())
}

//! `blk`'s launches captured into the graph at `key` under the dry walk `walk`, the old graph destroyed; null where the capture failed
[cold_path]
def private cut_graph_capture(rl : CutRail; key, walk : uint64; blk : block<void>) : void? {
    cuda_graph_destroy((g_cut_graphs?[key] ?? CutGraph()).exec)
    var exec : void?
    cud_walk(CudWalk.quiet) {
        exec = cuda_graph_capture(rl.stream, blk)
    }
    g_cut_graphs |> insert(key, CutGraph(walk = walk, exec = exec))
    return exec
}

[cold_path]
def private cut_graph_refused {
    to_log(LOG_WARNING, "dasLLAMA cuda tower: a chain's capture failed ({cuda_last_error()}); the chains launch kernel by kernel from here\n")
    g_cut_graph_on = false
    cut_graphs_drop()
}

//! the knob's line, once a process: the chains launch kernel by kernel
[cold_path]
def private cut_graphs_off_say {
    g_cut_graphs_off_said = true
    to_log(LOG_INFO, "dasLLAMA cuda tower: DASLLAMA_CUDA_GRAPHS=0 - the tower chains launch kernel by kernel, no graph captured (unset it, or set it to 1, to replay them from captured graphs)\n")
}

[arch(at="../ARCHITECTURE_GPU_ASYMMETRIES_CUDA.md#cuda-chain-graphs")]
def cut_chain(rl : CutRail; tag : string; shape : int64; blk : block<void>) {
    if (!g_cut_graph_on || g_env_vulkan.cuda_graphs == 0l) {
        cut_graphs_off_say() if (g_env_vulkan.cuda_graphs == 0l && !g_cut_graphs_off_said)
        invoke(blk)
        return
    }
    let key = cut_rail_key(rl, tag, shape)
    if (!key_exists(g_cut_graphs, key)) {
        cut_graph_seen(key)
        invoke(blk)
        return
    }
    let walk = cud_walk(CudWalk.dry, blk)
    let g = g_cut_graphs?[key] ?? CutGraph()
    let exec = g.exec != null && g.walk == walk ? g.exec : cut_graph_capture(rl, key, walk, blk)
    if (exec == null) {
        cut_graph_refused()
        cud_walk(CudWalk.quiet, blk)
    } elif (!cuda_graph_launch(exec, rl.stream)) {
        panic("dasLLAMA cuda tower: the {tag} chain's graph did not launch - {cuda_last_error()}")
    }
}

let private CUT_ROUND_CHUNK = 8l   //! the rounds one graph of a round chain carries

[arch(at="../ARCHITECTURE_GPU_ASYMMETRIES_CUDA.md#cuda-chain-graphs")]
def cut_chain_rounds(rl : CutRail; tag : string; shape, rounds : int64; blk : block<(n : int64) : void>) {
    for (_c in range64(rounds / CUT_ROUND_CHUNK)) {
        cut_chain(rl, tag, (shape << 8l) | CUT_ROUND_CHUNK) {
            invoke(blk, CUT_ROUND_CHUNK)
        }
    }
    for (_r in range64(rounds % CUT_ROUND_CHUNK)) {
        cut_chain(rl, tag, (shape << 8l) | 1l) {
            invoke(blk, 1l)
        }
    }
}


def cut_seat(var rl : CutRail; role : TowerBuf; b : CutBuf) {
    rail_seat(rl, role, b.dev, b.bytes, 0u)
}

//! every seat of the rail at `r`'s planes, the stream, the ones row and the scratch's rows: after every allocation of the encode
def cut_rail_seats(var rl : CutRail; r : CutResident) {
    rl.stream = g_cud.stream
    rl.ones_off = r.ones_off
    rl.cap_rows = r.cap_rows
    cut_seat(rl, TowerBuf.x, r.x)
    cut_seat(rl, TowerBuf.xb, r.xb)
    cut_seat(rl, TowerBuf.xc, r.xc)
    cut_seat(rl, TowerBuf.xh, r.xh)
    cut_seat(rl, TowerBuf.q, r.q)
    cut_seat(rl, TowerBuf.k, r.k)
    cut_seat(rl, TowerBuf.v, r.v)
    cut_seat(rl, TowerBuf.q2, r.q2)
    cut_seat(rl, TowerBuf.k2, r.k2)
    cut_seat(rl, TowerBuf.v2, r.v2)
    cut_seat(rl, TowerBuf.k16, r.k16)
    cut_seat(rl, TowerBuf.v16, r.v16)
    cut_seat(rl, TowerBuf.qp, r.qp)
    cut_seat(rl, TowerBuf.kp16, r.kp16)
    cut_seat(rl, TowerBuf.vp16, r.vp16)
    cut_seat(rl, TowerBuf.attp, r.attp)
    cut_seat(rl, TowerBuf.att, r.att)
    cut_seat(rl, TowerBuf.yo, r.yo)
    cut_seat(rl, TowerBuf.hg, r.hg)
    cut_seat(rl, TowerBuf.hu, r.hu)
    cut_seat(rl, TowerBuf.yd, r.yd)
    cut_seat(rl, TowerBuf.tabs, r.tabs)
    cut_seat(rl, TowerBuf.qkv, r.qkv)
    cut_seat(rl, TowerBuf.cos, r.cos)
    cut_seat(rl, TowerBuf.sin, r.sin)
    cut_seat(rl, TowerBuf.sk, r.splitk_parts)
    cut_seat(rl, TowerBuf.planes, r.planes)
    cut_seat(rl, TowerBuf.col, r.col)
    cut_seat(rl, TowerBuf.win_sort, r.win_sort)
    cut_seat(rl, TowerBuf.pos_resized, r.pos_resized)
    cut_seat(rl, TowerBuf.win_table, r.win_table)
    cut_seat(rl, TowerBuf.tail_out, r.tail_out)
    cut_seat(rl, TowerBuf.tail_proj, r.tail_proj)
    cut_seat(rl, TowerBuf.tail_hid, r.tail_hid)
    cut_seat(rl, TowerBuf.tail_gate, r.tail_gate)
    cut_seat(rl, TowerBuf.stage1, r.stage1)
    cut_seat(rl, TowerBuf.fw16, r.fw16)
    cut_seat(rl, TowerBuf.fnorm, r.fnorm)
    cut_seat(rl, TowerBuf.norms, r.norms)
    cut_seat(rl, TowerBuf.wb, r.wb)
    cut_seat(rl, TowerBuf.wq, r.wq)
    cut_seat(rl, TowerBuf.ws, r.ws)
    cut_seat(rl, TowerBuf.meta, r.sched)
}

//! a family's planes the shared resident does not carry, by role (`TowerBuf`): a seat module's roles list names the ones it binds
struct CutRolePlanes {
    b : CutBuf[TOWER_BUFS]
}

//! the plane at `role` of `p` holding at least `need` bytes; a refusal marked on `r.oom`
def cp_grow(var r : CutResident; var p : CutRolePlanes; role : TowerBuf; need : int64) {
    cut_grow(r, p.b[int(role)], need)
}

//! `bytes` from the host at `src` into the plane at `role` of `p`, grown to them first
def cp_up(var r : CutResident; var p : CutRolePlanes; role : TowerBuf; src : void?; bytes : int64) {
    cut_up(r, p.b[int(role)], src, bytes)
}

def cp_dev(p : CutRolePlanes; role : TowerBuf) : uint64 => p.b[int(role)].dev

//! the planes of `p` at `roles` freed
[cold_path]
def cp_release(var p : CutRolePlanes; roles : auto(RL)) {
    for (role in roles) {
        cut_free(p.b[int(role)])
    }
}

//! the planes of `p` at `roles` seated on the rail
def cp_seats(var rl : CutRail; p : CutRolePlanes; roles : auto(RL)) {
    for (role in roles) {
        cut_seat(rl, role, p.b[int(role)])
    }
}

def cut_f32_ensure(bf16 : bool) : bool
    => bf16 ? (ensure_f32_gemm_bf16_cls() && ensure_f32_gemm_bf16_b_cls()) : (ensure_f32_gemm_cls() && ensure_f32_gemm_b_cls())

def cut_attn_ensure(hsp : int64) : bool
    => (ensure_tower_hpad_cls() && ensure_tower_hpad16_cls() && ensure_tower_hpad_b_cls() && ensure_tower_hpad16_b_cls() && ensure_tower_hunpad16_cls()
        && ensure_tower_hunpad_cls() && (hsp == 64l ? ensure_fa_cm2_h64_bidir_cls() : ensure_fa_cm2_h128_bidir_cls()))

//! the pre-LN block loop's classes (`tower_ln_chain`) at the padded head width `hsp`
def cut_ln_ensure(f32w : bool; hsp : int64) : bool
    => (ensure_tower_ln_cls() && ensure_tower_ln16_cls() && ensure_tower_post_add_ln_cls() && ensure_tower_post_add_ln16_cls()
        && ensure_tower_bias_act_cls() && ensure_tower_bias_act_cvt16_cls() && cut_attn_ensure(hsp) && cut_f32_ensure(false) && (f32w || ensure_tower_clamp_cvt_cls()))

//! the encode's landing for every chain: `bytes` at `dev` read back into `dst`, the counters, the announcement once
def cut_served(var r : CutResident; dev : uint64; var dst : array<float>; bytes, npos, n_layer : int64) : bool {
    cut_dtoh(unsafe(addr<void?>(dst[0])), dev, bytes)
    cut_sync(r.family)
    g_cut_encodes++
    g_cut_rows += npos
    g_cut_blocks += n_layer
    cut_announce(r) if (!r.served_said)
    return true
}

//! the q8 lane's pick before the family's own classes: the knob, the device, the q8 cm2 tile at every column `cut_pick` maps to
def cut_q8_pick_arms(q8 : bool) : bool {
    return false if (!q8 || !g_cut_on || !cuda_tower_arms())
    cud_bind()
    return (cud_cm2_ensure(int(KqFmt.q8), CUD_CM2_TC_L) && cud_cm2_ensure(int(KqFmt.q8), CUD_CM2_TC_M)
        && cud_cm2_ensure(int(KqFmt.q8), CUD_CM2_TC_S))
}

//! a family's CUDA flavor stood down at load with the failing condition said (`tower_flavor_refused`)
def cut_flavor_refused(family, why : string) : bool => tower_flavor_refused(TowerDev.cuda, family, why)

[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#rail-audio-installs")]
def cut_lane_arms(family : string; q8 : bool) : bool {
    return cut_flavor_refused(family, CUT_DECLINE_WORDS_TOWER_KNOB) if (!g_cut_on)
    return cut_flavor_refused(family, CUT_DECLINE_WORDS_DEVICE) if (!cuda_tower_arms())
    return cut_q8_pick_arms(true) || cut_flavor_refused(family, "the q8 lane's cm2 tile did not build at every column") if (q8)
    cud_bind()
    return true
}

//! a flavor pick's chain classes built (`ok`), else the stand-down said for `family`
def cut_flavor_classes(family : string; ok : bool) : bool => ok || cut_flavor_refused(family, "a class of its chains did not build")

[init]
def dasllama_cuda_tower_seat_register {
    register_cud_drop_hook(@@cuda_tower_shutdown)
    cud_on_shutdown(@@cuda_tower_shutdown)
}
