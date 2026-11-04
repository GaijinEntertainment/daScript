options gen2
options indenting = 4
options _dasllama_internal = true

//! The CUDA tower driver: the vision ViT chains - gemma3v, gemma4v, the gemma4uv embedder, qwen3v, qwen25v - served whole
//! on the CUDA home off the image planes, the Vulkan tower's chains dispatch for dispatch over the stamps the two discrete
//! homes share, on the `cud_` rail (no descriptor sets, no hazard rail, one stream). The q8 lane reads each family's
//! CUDA-flavored image's device block (`ARCHITECTURE_IMAGE_GPU_PLANES.md#tower-dev-block`); the exact lanes read the
//! file's planes as stored, the bf16 ones through the f32 tile's bf16 arm. Registers the vision hook seats and the vision
//! flavors under the CUDA home (`register_gpu_home`), so the Vulkan tower's vision seats stand down under the CUDA ask.
module dasllama_cuda_tower shared public

require dasllama/dasllama_lint public
require math
require dasllama/dasllama_kqformat
require dasllama/dasllama_math
require dasllama/dasllama_plane
require dasllama/dasllama_tower
require dasllama/dasllama_tower_image
require dasllama/dasllama_gpu_tier   // gpu_want_cuda, register_gpu_home, gpu_want_arms_tier, TILE_READ_SLACK, GpuDeclineCounter
require dasllama/dasllama_gpu_math   // cm2_tiles_at, batch_sched_map, f32_gemm_wgs
require dasllama/dasllama_gpu_kernels_common
require dasllama/dasllama_gpu_kernels_discrete
require dasllama/dasllama_cuda_common
require dasllama/dasllama_cuda_classes
require dasllama/dasllama_cuda_decode   // cud_cm2_pick: the window GEMM's column and k split, register_cud_drop_hook
require dasllama/dasllama_gemma3v
require dasllama/dasllama_gemma4v
require dasllama/dasllama_gemma4uv
require dasllama/dasllama_qwen3v
require dasllama/dasllama_qwen25v

var private g_cut_on = true
var private g_cut_encodes = 0l
var private g_cut_rows = 0l
var private g_cut_blocks = 0l
var private g_cut_stem_encodes = 0l   //! the stems served on the device, one an encode
var private g_cut_tail_encodes = 0l
var private g_cut_declines = GpuDeclineCounter(home = "cuda")

enum private CudaTowerDecline {
    knob
    quant_mode
    flavor
    shape
    device
    memory
}

//! Engage / decline counters; `blocks` = transformer blocks the driver ran. Model-level gates assert DELTAS on these.
def cuda_tower_stats : tuple<encodes : int64; rows : int64; blocks : int64; declines : int64; convs : int64; mels : int64> =>
    (encodes = g_cut_encodes, rows = g_cut_rows, blocks = g_cut_blocks, declines = g_cut_declines.total, convs = g_cut_stem_encodes, mels = 0l)

//! The declines-by-reason table (a copy), keyed by `CudaTowerDecline` value names.
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

let private CUT_MAX_ENCODE_ROWS = 4096l
let private CUT_SPLITK_MIN_ROWS = 512l   //! the encode rows from which the fc2 GEMM takes the split-k pick
let private CUT_NO_BIAS = -1l

struct private CutBuf {
    dev : uint64
    bytes : int64
}

//! a family's residency: the planes bound once a model (`key`), the per-encode scratch grown to the widest encode served
struct private CutResident {
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
    planes : CutBuf      //! the canvas's pixel planes
    col : CutBuf
    win_sort : CutBuf
    pos_resized : CutBuf
    win_table : CutBuf
    tail_out : CutBuf    //! the tail rows at the tail's widest width, and the projected rows the host reads back
    tail_proj : CutBuf
}

var private g_cut_g3v = CutResident(family = "gemma3v")
var private g_cut_g4v = CutResident(family = "gemma4v")
var private g_cut_uv = CutResident(family = "gemma4uv")
var private g_cut_q3v = CutResident(family = "qwen3v")
var private g_cut_q25v = CutResident(family = "qwen25v")
var private @scratch @exact_size g_cut_sched : array<uint>
var private @scratch @exact_size g_cut_map_offs : array<uint>     //! the encode's schedule map offsets, one a GEMM record
var private @scratch @exact_size g_cut_ln_offs : array<LayerOffs>
var private @scratch @exact_size g_cut_rope_tabs : array<float>    //! the packed rope tables staging (gemma4v)
var private @scratch @exact_size g_cut_win_sort : array<uint>      //! the window-sort row map staging (qwen25v)
var private @scratch @exact_size g_cut_win_table : array<uint>      //! the window table staging (qwen25v)
var private @scratch @exact_size g_cut_ones : array<float>

[cold_path]
def private cut_decline_words(why : CudaTowerDecline) : string {
    if (why == CudaTowerDecline.knob) {
        return "the tower knob is off (set_cuda_tower(false))"
    } elif (why == CudaTowerDecline.quant_mode) {
        return "the tower's planes are not the lane the driver serves"
    } elif (why == CudaTowerDecline.flavor) {
        return "the model is not the CUDA-flavored image - each backend serves its own"
    } elif (why == CudaTowerDecline.shape) {
        return "a width, head size, row count or window the stamps do not serve"
    } elif (why == CudaTowerDecline.memory) {
        return "a device allocation failed"
    }
    return "the home has no device (DASLLAMA_GPU=1 DASLLAMA_CUDA=1 arm it), or a class failed to build"
}

[cold_path]
def private cut_decline(r : CutResident; why : CudaTowerDecline) : bool
    => g_cut_declines |> note("the {r.family} tower", "{why}", "the tower stays on the CPU chain: {cut_decline_words(why)}")

def private cut_vis_gate(r : CutResident; q8 : bool; dev : TowerDev) : bool {
    return cut_decline(r, CudaTowerDecline.knob) if (!g_cut_on)
    return !q8 || dev == TowerDev.cuda || cut_decline(r, CudaTowerDecline.flavor)
}

def private cut_arm(r : CutResident) : bool {
    if (!cuda_tower_arms()) {
        return cut_decline(r, CudaTowerDecline.device)
    }
    cud_bind()
    return true
}

[cold_path]
def private cut_announce(var r : CutResident) {
    r.served_said = true
    to_log(LOG_INFO, "dasLLAMA cuda tower: {r.family} serves on the device (DASLLAMA_GPU=1 DASLLAMA_CUDA=1 arm the home)\n")
}

//! `b` holding at least `need` bytes: grown by a fresh allocation (the old bytes dropped), a refusal marked on `r.oom`
def private cut_grow(var r : CutResident; var b : CutBuf; need : int64) {
    cut_realloc(r, b, need) if (need > b.bytes && !r.oom)
}

[cold_path]
def private cut_realloc(var r : CutResident; var b : CutBuf; need : int64) {
    cuda_mem_free(b.dev)
    b.dev = cuda_mem_alloc(uint64(max(need, 64l)))
    b.bytes = b.dev == 0ul ? 0l : max(need, 64l)
    r.oom = r.oom || b.dev == 0ul
}

def private cut_free(var b : CutBuf) {
    cuda_mem_free(b.dev)
    b.dev = 0ul
    b.bytes = 0l
}

def private cut_up(var r : CutResident; var b : CutBuf; src : void?; bytes : int64) {
    cut_grow(r, b, bytes)
    return if (r.oom || bytes <= 0l)
    cut_htod(b.dev, src, bytes)
}

//! `bytes` from the host at `src` to the device at `dev` on the home's stream
def private cut_htod(dev : uint64; src : void?; bytes : int64) {
    cuda_memcpy_htod(dev, src, uint64(bytes), g_cud.stream)   // nolint:PERF026 — cu_ok's text is built on its failure arm alone
}

//! `bytes` from the device at `dev` to the host at `dst` on the home's stream
def private cut_dtoh(dst : void?; dev : uint64; bytes : int64) {
    cuda_memcpy_dtoh(dst, dev, uint64(bytes), g_cud.stream)   // nolint:PERF026 — cu_ok's text is built on its failure arm alone
}

[cold_path]
def private cut_release(var r : CutResident) {
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
    r.key = 0ul
    r.cap_rows = 0l
    r.oom = false
    r.served_said = false
    r.picks_said |> clear()
    g_cut_declines |> unsay("the {r.family} tower")
}

def private cut_each_resident(blk : block<(var r : CutResident) : void>) {
    invoke(blk, g_cut_g3v)
    invoke(blk, g_cut_g4v)
    invoke(blk, g_cut_uv)
    invoke(blk, g_cut_q3v)
    invoke(blk, g_cut_q25v)
}

//! Every resident released: the model drop's sweep and the shutdown
def cuda_tower_shutdown {
    return if (!g_cud.ready)
    cud_bind()
    cuda_stream_synchronize(g_cud.stream)
    cut_each_resident() $(var r : CutResident) {
        cut_release(r)
    }
}

//! the resident key folds every model field the resident's extent follows: the planes' addresses, sizes and sampled words, the layer count
def private cut_key(m; n_layer : int64) : uint64 {
    var k = hash_combine64(0x6b0dul, uint64(n_layer))
    k = hash_combine64(k, intptr(unsafe(reinterpret<void?>(m.blob.p))))
    k = hash_combine64(k, uint64(m.blob.n))
    k = tower_fold_words(k, unsafe(reinterpret<uint8 const?>(m.blob.p)), m.blob.n * 4l)
    k = hash_combine64(k, intptr(unsafe(reinterpret<void?>(m.wblob.p))))
    k = hash_combine64(k, uint64(m.wblob.n))
    k = tower_fold_words(k, unsafe(reinterpret<uint8 const?>(m.wblob.p)), m.wblob.n * 2l)
    static_if (typeinfo safe_has_field < dev > (m)) {
        k = hash_combine64(k, intptr(unsafe(reinterpret<void?>(m.dev.p))))
        k = hash_combine64(k, uint64(m.dev.n))
        k = tower_fold_words(k, m.dev.p, m.dev.n)
    }
    return k
}

//! the f32 blob with `ones_n` ones behind it and the halfword plane, as stored; false with the resident released on a refused allocation
[cold_path]
def private cut_upload_planes(var r : CutResident; key : uint64; blob : PlaneF; wblob : PlaneU16; ones_n : int64) : bool {
    r.key = key
    r.oom = false
    r.ones_off = blob.n
    cut_grow(r, r.norms, (blob.n + ones_n) * 4l)
    cut_up(r, r.norms, unsafe(reinterpret<void?>(blob.p)), blob.n * 4l)
    if (!r.oom && ones_n > 0l) {
        g_cut_ones |> reserve(ones_n)
        g_cut_ones |> resize(ones_n)
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

[cold_path, arch(at = "../ARCHITECTURE_IMAGE_GPU_PLANES.md#tower-dev-block")]
def private cut_upload_q8(var r : CutResident; m; prefix : string) : bool {
    let pq = tower_dev_find(m.devplan, "{prefix}.wq")
    let ps = tower_dev_find(m.devplan, "{prefix}.ws")
    if (pq.off < 0l || ps.off < 0l) {
        panic("dasLLAMA cuda tower: the {r.family} CUDA image carries no `{prefix}` pieces - its block was baked by another driver")
    }
    cut_up(r, r.wq, unsafe(reinterpret<void?>(plane_at(m.dev, pq.off))), pq.bytes)
    cut_up(r, r.ws, unsafe(reinterpret<void?>(plane_at(m.dev, ps.off))), ps.bytes)
    if (r.oom) {
        cut_release(r)
        return false
    }
    return true
}

//! the per-encode scratch at `npos` rows - `dp` 0: no padded attention panels (the compact rows attend), `tail_dim` 0: no tail rows; false counted `memory`
def private cut_scratch(var r : CutResident; npos, d, ff, dp, tail_dim : int64) : bool {
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
    return !r.oom || cut_decline(r, CudaTowerDecline.memory)
}

//! the vision planes at the canvas and the column plane at the scratch's rows; false counted `memory`
def private cut_vis_bufs(var r : CutResident; w, h, pd : int64) : bool {
    cut_grow(r, r.planes, w * h * 3l * 4l)
    cut_grow(r, r.col, r.cap_rows * pd * 4l)
    return !r.oom || cut_decline(r, CudaTowerDecline.memory)
}

//! one GEMM site's cm2 pick for the encode: the tile column, the k split and the workgroups its schedule map carries
struct private CutPick {
    tc : int64
    tci : int
    nsplit : int64
    ksplit : int64
    groups : int64
}

def private cut_pick(var r : CutResident; d, rows, n : int64; split_ok : bool) : CutPick {
    var pk = cud_cm2_pick(d, rows, n)
    if (!split_ok || rows < CUT_SPLITK_MIN_ROWS) {
        pk.nsplit = 1l
        pk.ksplit = 0l
    }
    if (pk.nsplit > 1l) {
        cut_grow(r, r.splitk_parts, pk.nsplit * r.cap_rows * d * 4l)
        if (r.oom) {
            r.oom = false
            pk.nsplit = 1l
            pk.ksplit = 0l
        }
    }
    let tci = pk.tc == 256l ? CUD_CM2_TC_L : (pk.tc == 128l ? CUD_CM2_TC_M : CUD_CM2_TC_S)
    return CutPick(tc = pk.tc, tci = tci, nsplit = pk.nsplit, ksplit = pk.ksplit, groups = cm2_tiles_at(d, rows, pk.tc) * pk.nsplit)
}

def private cut_pick_ensure(p : CutPick) : bool => cud_cm2_ensure(int(KqFmt.q8), p.tci) && (p.nsplit == 1l || ensure_splitk_reduce_cls())

[cold_path]
def private cut_pick_say(var r : CutResident; npos : int64; pq, pff, pfc2 : CutPick) {
    r.picks_said |> insert(npos)
    to_log(LOG_INFO, "dasLLAMA cuda tower: {r.family} at {npos} rows - {cut_pick_words("d", pq)}, {cut_pick_words("ff", pff)}, {cut_pick_words("fc2", pfc2)}\n")
}

[cold_path]
def private cut_pick_words(name : string; p : CutPick) : string {
    let split = p.nsplit > 1l ? " split {p.nsplit}" : ""
    return "{name} column {p.tc}{split}"
}

//! the encode's schedule staged: `nrec` records at its head, `map_words` of maps behind them
def private cut_sched_begin(nrec, map_words : int64) {
    g_cut_sched |> reserve(nrec * 4l + map_words)
    g_cut_sched |> resize(nrec * 4l + map_words)
    g_cut_map_offs |> reserve(nrec)
    g_cut_map_offs |> resize(nrec)
}

def private cut_sched_up(var r : CutResident) : bool {
    cut_up(r, r.sched, unsafe(addr<void?>(g_cut_sched[0])), long_length(g_cut_sched) * 4l)
    return !r.oom || cut_decline(r, CudaTowerDecline.memory)
}

//! the schedule's records in record order, each answered by `rec` as (weight block, rows, groups); the map offsets in `g_cut_map_offs`
def private cut_sched_walk(nrec : int64; rec : block<(rid : int64) : tuple<wblk0 : int64; rows : int64; groups : int64>>) {
    var cur = nrec * 4l
    var mp = unsafe(addr(g_cut_sched[0]))
    for (rid in range64(nrec)) {
        let rr = invoke(rec, rid)
        g_cut_map_offs[rid] = batch_sched_map(mp, cur, rid, rr.wblk0, rr.rows, rr.groups)
    }
}

//! the q8 cm2 tile over schedule record `rid`, the f16 feed `xh` into `y` [rows x d]; a split sums its partial planes into `y`
def private cut_mm_q8(r : CutResident; p : CutPick; rid : int64; n, d, rows : int64; y : uint64) {
    var pc = BatchArgs(n = uint(n), d = uint(d), map_off = g_cut_map_offs[rid], ksplit = uint(p.ksplit))
    let split = p.nsplit > 1l
    cud_cm2_enc(int(KqFmt.q8), p.tci, g_cud.stream, fixed_array(r.wq.dev, r.ws.dev, r.sched.dev, r.xh.dev, split ? r.splitk_parts.dev : y), pc, p.groups)
    if (split) {
        var pr = SkRedArgs(nelem = uint(rows * d), k = uint(p.nsplit), ybase = 0u)
        enc_splitk_reduce_cls(g_cud.stream, fixed_array(r.splitk_parts.dev, y), pr, splitk_reduce_wgs(rows * d))
    }
}

//! x [rows x n] into y [rows x dout] at `yoff` on the f32 tile over the norms plane's rows at `woff` (`bf16`: the halfword plane's)
def private cut_mm32(r : CutResident; bf16 : bool; x, y : uint64; n, dout, rows, woff, boff, yoff : int64) {
    var pc = f32_gemm_args(n, dout, rows, woff, max(boff, 0l), yoff)
    let wgs = f32_gemm_wgs(rows, dout)
    let w = bf16 ? r.wb.dev : r.norms.dev
    if (boff >= 0l) {
        if (bf16) {
            enc_f32_gemm_bf16_b_cls(g_cud.stream, fixed_array(w, x, y, r.norms.dev), pc, wgs)
        } else {
            enc_f32_gemm_b_cls(g_cud.stream, fixed_array(w, x, y, r.norms.dev), pc, wgs)
        }
    } elif (bf16) {
        enc_f32_gemm_bf16_cls(g_cud.stream, fixed_array(w, x, y), pc, wgs)
    } else {
        enc_f32_gemm_cls(g_cud.stream, fixed_array(w, x, y), pc, wgs)
    }
}

def private cut_f32_ensure(bf16 : bool) : bool
    => bf16 ? (ensure_f32_gemm_bf16_cls() && ensure_f32_gemm_bf16_b_cls()) : (ensure_f32_gemm_cls() && ensure_f32_gemm_b_cls())

//! the rows of `x` [nelem] as the cm2 tile's f16 feed, clamped to [lo, hi]
def private cut_cvt16(r : CutResident; x : uint64; nelem : int64; lo : float = -NO_CLAMP; hi : float = NO_CLAMP) {
    var pc = GkClampArgs(nelem = uint(nelem), lo = lo, hi = hi, scale = 1.0)
    enc_tower_clamp_cvt_cls(g_cud.stream, fixed_array(x, r.xh.dev), pc, rq_wgs(false, nelem))
}

def private cut_rows_wgs(nelem : int64) : int64 => (nelem + 255l) / 256l

def private cut_vis_im2col(r : CutResident; w, patch, pd, npos : int64; scale, shift : float) {
    var pc = GkPatchIm2colArgs(w = uint(w), p = uint(patch), row_dim = uint(pd), npos = uint(npos), total = uint(r.cap_rows * pd),
        scale = scale, shift = shift)
    enc_tower_patch_im2col_cls(g_cud.stream, fixed_array(r.planes.dev, r.col.dev), pc, cut_rows_wgs(r.cap_rows * pd))
}

def private cut_planes_up(r : CutResident; planes : array<float>; w, h : int64) {
    cut_htod(r.planes.dev, unsafe(addr<void?>(planes[0])), w * h * 3l * 4l)
}

//! the row-broadcast bias row at `boff` (and `act`) over `nelem` elements of rows `dout` wide, in place
def private cut_bias_act(r : CutResident; x : uint64; dout, nelem, boff : int64; act : uint) {
    var pc = GkBiasActArgs(d = uint(dout), nelem = uint(nelem), boff = uint(boff), act = act)
    enc_tower_bias_act_cls(g_cud.stream, fixed_array(x, r.norms.dev), pc, rq_wgs(false, nelem))
}

//! the activation alone over `nelem` elements, in place
def private cut_act(x : uint64; dout, nelem : int64; act : uint) {
    var pc = GkBiasActArgs(d = uint(dout), nelem = uint(nelem), boff = 0u, act = act)
    enc_tower_act_cls(g_cud.stream, fixed_array(x), pc, rq_wgs(false, nelem))
}

//! q / k / v restrided onto `hsp`-wide heads, the flash tile, the rows back compact: f16 into `xh` under `att16`, else `att`
def private cut_attn_padded(r : CutResident; q, k, v : uint64; sstride, npos, nheads, hs, hsp : int64; q_b, k_b, v_b : int64; q_off, k_off, v_off : int64; att16 : bool) {
    let dp = nheads * hsp
    let npos_pad = tower_cap_rows(npos)
    let wg_pad = cut_rows_wgs(npos_pad * dp)
    var pc_rs = GkRestrideArgs(rows = uint(npos), rows_pad = uint(npos_pad), nheads = uint(nheads), hs = uint(hs), hsp = uint(hsp), sstride = uint(sstride), soff = uint(q_off))
    if (q_b >= 0l) {
        pc_rs.boff = uint(q_b)
        enc_tower_hpad_b_cls(g_cud.stream, fixed_array(q, r.qp.dev, r.norms.dev), pc_rs, wg_pad)
    } else {
        enc_tower_hpad_cls(g_cud.stream, fixed_array(q, r.qp.dev), pc_rs, wg_pad)
    }
    pc_rs.soff = uint(k_off)
    if (k_b >= 0l) {
        pc_rs.boff = uint(k_b)
        enc_tower_hpad16_b_cls(g_cud.stream, fixed_array(k, r.kp16.dev, r.norms.dev), pc_rs, wg_pad)
    } else {
        enc_tower_hpad16_cls(g_cud.stream, fixed_array(k, r.kp16.dev), pc_rs, wg_pad)
    }
    pc_rs.soff = uint(v_off)
    if (v_b >= 0l) {
        pc_rs.boff = uint(v_b)
        enc_tower_hpad16_b_cls(g_cud.stream, fixed_array(v, r.vp16.dev, r.norms.dev), pc_rs, wg_pad)
    } else {
        enc_tower_hpad16_cls(g_cud.stream, fixed_array(v, r.vp16.dev), pc_rs, wg_pad)
    }
    var pc_fa = FaArgs(rows = uint(npos), w0 = 0u, qd = uint(dp), kvd = uint(dp), kv_mul = 1u, kvbase = 0u, scale = 1.0 / sqrt(float(hs)), window = 0u, sinkoff = 0u)
    cud_fa_enc(g_cud.stream, hsp, false, false, false, fixed_array(r.qp.dev, r.kp16.dev, r.vp16.dev, r.attp.dev), 0ul, pc_fa, fa_stamp_wgs(false, nheads, npos), [bidir = true])
    var pc_un = GkRestrideArgs(rows = uint(npos), rows_pad = uint(npos_pad), nheads = uint(nheads), hs = uint(hs), hsp = uint(hsp), sstride = uint(nheads * hs), soff = 0u)
    if (att16) {
        enc_tower_hunpad16_cls(g_cud.stream, fixed_array(r.attp.dev, r.xh.dev), pc_un, cut_rows_wgs(npos * dp))
    } else {
        enc_tower_hunpad_cls(g_cud.stream, fixed_array(r.attp.dev, r.att.dev), pc_un, cut_rows_wgs(npos * dp))
    }
}

def private cut_attn_ensure(hsp : int64) : bool
    => (ensure_tower_hpad_cls() && ensure_tower_hpad16_cls() && ensure_tower_hpad_b_cls() && ensure_tower_hpad16_b_cls() && ensure_tower_hunpad16_cls()
        && ensure_tower_hunpad_cls() && (hsp == 64l ? ensure_fa_cm2_h64_bidir_cls() : ensure_fa_cm2_h128_bidir_cls()))

def private cut_served(var r : CutResident; dev : uint64; var dst : array<float>; bytes, npos, n_layer : int64) : bool {
    cut_dtoh(unsafe(addr<void?>(dst[0])), dev, bytes)
    if (!cuda_stream_synchronize(g_cud.stream)) {   // nolint:PERF026 — cu_ok's text is built on its failure arm alone
        panic("dasLLAMA cuda tower: the {r.family} chain failed on the device - {cuda_last_error()}")
    }
    g_cut_encodes++
    g_cut_rows += npos
    g_cut_blocks += n_layer
    cut_announce(r) if (!r.served_said)
    return true
}

def private cut_vis_tail_ensure(vis : TowerVisTail) : bool
    => (ensure_tower_pool_2d_cls() && ensure_tower_rms_cls() && cut_f32_ensure(vis.proj_bf16) && (!vis.affine || ensure_tower_affine_rows_cls())
        && (!vis.clamp.active || ensure_tower_clamp_cls()))

def private cut_vis_tail_enc(r : CutResident; src : uint64; vis : TowerVisTail) {
    let nd = vis.nout * vis.d
    var pc_pool = GkPool2dArgs(width = uint(vis.d), gw = uint(vis.gw), k = uint(vis.merge), total = uint(nd))
    enc_tower_pool_2d_cls(g_cud.stream, fixed_array(src, r.tail_out.dev), pc_pool, cut_rows_wgs(nd))
    if (vis.affine) {
        var pc_aff = GkAffineRowsArgs(width = uint(vis.d), total = uint(nd), scale = vis.scale, boff = uint(vis.std ? vis.std_bias : 0l),
            moff = uint(vis.std ? vis.std_scale : 0l), std = vis.std ? 1u : 0u)
        enc_tower_affine_rows_cls(g_cud.stream, fixed_array(r.tail_out.dev, r.norms.dev, r.norms.dev), pc_aff, cut_rows_wgs(nd))
    }
    var pc_rms = gk_rms_args(uint(vis.d), uint(vis.nout), vis.eps, uint(vis.soft_norm_w >= 0l ? vis.soft_norm_w : r.ones_off))
    enc_tower_rms_cls(g_cud.stream, fixed_array(r.tail_out.dev, r.tail_out.dev, r.norms.dev), pc_rms, vis.nout)
    if (vis.clamp.active) {
        var pc_ci = GkClampArgs(nelem = uint(nd), lo = vis.clamp.imin, hi = vis.clamp.imax, scale = 1.0)
        enc_tower_clamp_cls(g_cud.stream, fixed_array(r.tail_out.dev), pc_ci, rq_wgs(false, nd))
    }
    cut_mm32(r, vis.proj_bf16, r.tail_out.dev, r.tail_proj.dev, vis.d, vis.proj, vis.nout, vis.proj_w, CUT_NO_BIAS, 0l)
    if (vis.clamp.active) {
        var pc_co = GkClampArgs(nelem = uint(vis.nout * vis.proj), lo = vis.clamp.omin, hi = vis.clamp.omax, scale = 1.0)
        enc_tower_clamp_cls(g_cud.stream, fixed_array(r.tail_proj.dev), pc_co, rq_wgs(false, vis.nout * vis.proj))
    }
}

let private CUT_G3V_PIECE = "cu.g3v"

def private cut_g3v_offs(t : Gemma3vTower) {
    g_cut_ln_offs |> reserve(t.n_layer)
    g_cut_ln_offs |> resize(t.n_layer)
    for (l in range64(t.n_layer)) {
        g_cut_ln_offs[l] = gemma3v_ln_offs(t, l)
    }
}

[cold_path, arch(at = "../ARCHITECTURE_IMAGE_GPU_PLANES.md#tower-dev-block")]
def private cuda_gemma3v_bake(t : Gemma3vTower; var b : TowerDevBuild) : bool {
    var inscope regions <- gemma3v_all_regions(t)
    tower_bake_q8(b, CUT_G3V_PIECE, t.qblob, t.qscales, regions)
    return true
}

//! the pre-LN block loop over `offs`; the last seam folds the post-norm and lands its f32 rows in `xb`, which the tail reads
def private cut_ln_chain(r : CutResident; offs : array<LayerOffs>; f32w : bool; d, ff, hs, hsp, nheads, npos : int64; eps : float;
                         fc1_act : uint; lnp_w, lnp_b : int64; pq, pff, pfc2 : CutPick) {
    let nl = long_length(offs)
    let nff = npos * ff
    var st = g_cud.stream
    for (l in range64(nl)) {
        let o = offs[l]
        let rb = l * LN_BLOCK_GEMMS
        if (l == 0l) {
            var pc = gk_ln_args(uint(d), uint(npos), eps, uint(o.ln1_w), uint(o.ln1_b))
            if (f32w) {
                enc_tower_ln_cls(st, fixed_array(r.x.dev, r.xb.dev, r.norms.dev), pc, npos)
            } else {
                enc_tower_ln16_cls(st, fixed_array(r.x.dev, r.norms.dev, r.xh.dev), pc, npos)
            }
        }
        if (f32w) {
            cut_mm32(r, false, r.xb.dev, r.q.dev, d, d, npos, o.q_w, CUT_NO_BIAS, 0l)
            cut_mm32(r, false, r.xb.dev, r.k.dev, d, d, npos, o.k_w, CUT_NO_BIAS, 0l)
            cut_mm32(r, false, r.xb.dev, r.v.dev, d, d, npos, o.v_w, CUT_NO_BIAS, 0l)
        } else {
            cut_mm_q8(r, pq, rb + LN_GEMM_Q, d, d, npos, r.q.dev)
            cut_mm_q8(r, pq, rb + LN_GEMM_K, d, d, npos, r.k.dev)
            cut_mm_q8(r, pq, rb + LN_GEMM_V, d, d, npos, r.v.dev)
        }
        cut_attn_padded(r, r.q.dev, r.k.dev, r.v.dev, d, npos, nheads, hs, hsp, o.q_b, o.k_b, o.v_b, 0l, 0l, 0l, !f32w)
        if (f32w) {
            cut_mm32(r, false, r.att.dev, r.yo.dev, d, d, npos, o.o_w, CUT_NO_BIAS, 0l)
        } else {
            cut_mm_q8(r, pq, rb + LN_GEMM_O, d, d, npos, r.yo.dev)
        }
        var pc_post = GkPostAddLnArgs(d = uint(d), rows = uint(npos), boff = uint(o.o_b), eps = eps, ln_on = 1u, lnoff = uint(o.ln2_w), lnboff = uint(o.ln2_b), ascale = 1.0, xboff = 0u)
        if (f32w) {
            enc_tower_post_add_ln_cls(st, fixed_array(r.x.dev, r.yo.dev, r.xb.dev, r.norms.dev), pc_post, npos)
            cut_mm32(r, false, r.xb.dev, r.hg.dev, d, ff, npos, o.fc1_w, CUT_NO_BIAS, 0l)
        } else {
            enc_tower_post_add_ln16_cls(st, fixed_array(r.x.dev, r.yo.dev, r.norms.dev, r.xh.dev), pc_post, npos)
            cut_mm_q8(r, pff, rb + LN_GEMM_FC1, d, ff, npos, r.hg.dev)
        }
        var pc_gelu = GkBiasActArgs(d = uint(ff), nelem = uint(nff), boff = uint(o.fc1_b), act = fc1_act)
        if (f32w) {
            enc_tower_bias_act_cls(st, fixed_array(r.hg.dev, r.norms.dev), pc_gelu, rq_wgs(false, nff))
            cut_mm32(r, false, r.hg.dev, r.yd.dev, ff, d, npos, o.fc2_w, CUT_NO_BIAS, 0l)
        } else {
            enc_tower_bias_act_cvt16_cls(st, fixed_array(r.hg.dev, r.norms.dev, r.xh.dev), pc_gelu, rq_wgs(false, nff))
            cut_mm_q8(r, pfc2, rb + LN_GEMM_FC2, ff, d, npos, r.yd.dev)
        }
        let last = l + 1l == nl
        pc_post.boff = uint(o.fc2_b)
        if (!last) {
            pc_post.lnoff = uint(offs[l + 1l].ln1_w)
            pc_post.lnboff = uint(offs[l + 1l].ln1_b)
        } else {
            pc_post.lnoff = uint(lnp_w)
            pc_post.lnboff = uint(lnp_b)
        }
        if (f32w || last) {
            enc_tower_post_add_ln_cls(st, fixed_array(r.x.dev, r.yd.dev, r.xb.dev, r.norms.dev), pc_post, npos)
        } else {
            enc_tower_post_add_ln16_cls(st, fixed_array(r.x.dev, r.yd.dev, r.norms.dev, r.xh.dev), pc_post, npos)
        }
    }
}

def private cut_ln_ensure(f32w : bool; hsp : int64) : bool
    => (ensure_tower_ln_cls() && ensure_tower_ln16_cls() && ensure_tower_post_add_ln_cls() && ensure_tower_post_add_ln16_cls()
        && ensure_tower_bias_act_cls() && ensure_tower_bias_act_cvt16_cls() && cut_attn_ensure(hsp) && cut_f32_ensure(false) && (f32w || ensure_tower_clamp_cvt_cls()))

[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_VISION.md#vk-vision-chains")]
def private cuda_gemma3v_chain(t : Gemma3vTower; var s : Gemma3vState; planes : array<float>; w, h, npos : int64) : bool {
    var r & = g_cut_g3v
    return false if (!cut_vis_gate(r, t.q8, t.dev_backend))
    let f32w = !t.q8
    let d = t.n_embd
    let ff = t.ff_pad
    let hs = d / t.n_head
    let hsp = hs <= 64l ? 64l : 128l
    let dp = t.n_head * hsp
    let pd = gemma3v_patch_pad(t)
    let n_side = t.image_px / t.patch
    let nout = (n_side / t.merge) * (n_side / t.merge)
    if (hs > 128l || d % 64l != 0l || ff % 64l != 0l || npos > CUT_MAX_ENCODE_ROWS || npos <= 0l || w != t.image_px || h != t.image_px
            || n_side % t.merge != 0l || npos != n_side * n_side) {
        return cut_decline(r, CudaTowerDecline.shape)
    }
    return false if (!cut_arm(r))
    let vis = TowerVisTail(nout = nout, gw = n_side, merge = t.merge, d = d, proj = t.proj_dim, soft_norm_w = t.soft_norm_w, proj_w = t.proj_w, eps = t.eps)
    if (!cut_ln_ensure(f32w, hsp) || !ensure_tower_patch_im2col_cls() || !cut_vis_tail_ensure(vis)) {
        return cut_decline(r, CudaTowerDecline.device)
    }
    let key = cut_key(t, t.n_layer)
    if (r.key != key) {
        cut_release(r)
        if (!cut_upload_planes(r, key, t.blob, t.wblob, d) || (!f32w && !cut_upload_q8(r, t, CUT_G3V_PIECE))) {
            return cut_decline(r, CudaTowerDecline.memory)
        }
    }
    if (!cut_scratch(r, npos, d, ff, dp, max(d, t.proj_dim)) || !cut_vis_bufs(r, w, h, pd)) {
        return false
    }
    cut_g3v_offs(t)
    var pq, pff, pfc2 : CutPick
    if (!f32w) {
        pq = cut_pick(r, d, npos, d, false)
        pff = cut_pick(r, ff, npos, d, false)
        pfc2 = cut_pick(r, d, npos, ff, true)
        if (!cut_pick_ensure(pq) || !cut_pick_ensure(pff) || !cut_pick_ensure(pfc2)) {
            return cut_decline(r, CudaTowerDecline.device)
        }
        let nrec = t.n_layer * LN_BLOCK_GEMMS
        cut_sched_begin(nrec, t.n_layer * (4l * pq.groups + pff.groups + pfc2.groups))
        var recs : Q8Region[int(LN_BLOCK_GEMMS)]
        cut_sched_walk(nrec) $(rid : int64) : tuple<wblk0 : int64; rows : int64; groups : int64> {
            let g = rid % LN_BLOCK_GEMMS
            if (g == 0l) {
                ln_block_regions(g_cut_ln_offs[rid / LN_BLOCK_GEMMS], d, ff, recs)
            }
            let p = g == LN_GEMM_FC1 ? pff : (g == LN_GEMM_FC2 ? pfc2 : pq)
            return (wblk0 = recs[g].woff / kq_elems(KqFmt.q8), rows = npos, groups = p.groups)
        }
        return false if (!cut_sched_up(r))
        if (!key_exists(r.picks_said, npos)) {
            cut_pick_say(r, npos, pq, pff, pfc2)
        }
    }
    cut_planes_up(r, planes, w, h)
    cut_vis_im2col(r, w, t.patch, pd, npos, 1.0, 0.0)
    cut_mm32(r, false, r.col.dev, r.x.dev, pd, d, npos, t.patch_w, t.patch_b, 0l)
    cut_bias_act(r, r.x.dev, npos * d, npos * d, t.pos_w, BIAS_ACT_NONE)
    g_cut_stem_encodes++
    cut_ln_chain(r, g_cut_ln_offs, f32w, d, ff, hs, hsp, t.n_head, npos, t.eps, BIAS_ACT_GELU_TANH, t.post_ln_w, t.post_ln_b, pq, pff, pfc2)
    cut_vis_tail_enc(r, r.xb.dev, vis)
    g_cut_tail_encodes++
    ensure_length(s.tail, nout * t.proj_dim)
    s.tail_ready = cut_served(r, r.tail_proj.dev, s.tail, nout * t.proj_dim * 4l, npos, t.n_layer)
    return s.tail_ready
}

let private CUT_G4V_PIECE = "cu.g4v"

[cold_path, arch(at = "../ARCHITECTURE_IMAGE_GPU_PLANES.md#tower-dev-block")]
def private cuda_gemma4v_bake(t : Gemma4vTower; var b : TowerDevBuild) : bool {
    var inscope regions <- gemma4v_all_regions(t)
    tower_bake_q8(b, CUT_G4V_PIECE, t.qblob, t.qscales, regions)
    return true
}

//! a clamped GEMM site on the q8 lane: the input bounds applied on the way into the f16 feed, the cm2 tile, the output bounds
def private cut_mm_clamped_q8(r : CutResident; c : Clamp; xin : uint64; nin : int64; p : CutPick; rid : int64; n, dout, rows : int64; y : uint64; out_cl : bool) {
    cut_cvt16(r, xin, nin, clamp_lo(c, true), clamp_hi(c, true))
    cut_mm_q8(r, p, rid, n, dout, rows, y)
    if (out_cl && c.active) {
        var pc_cl = GkClampArgs(nelem = uint(rows * dout), lo = c.omin, hi = c.omax, scale = 1.0)
        enc_tower_clamp_cls(g_cud.stream, fixed_array(y), pc_cl, rq_wgs(false, rows * dout))
    }
}

//! the exact lane's clamped site: the input bounds into `xc` under `copy` (the normed rows stay whole for the next site's own bounds), else in place
def private cut_mm_clamped32(r : CutResident; c : Clamp; copy, bf16 : bool; xin : uint64; nin : int64; n, dout, rows, woff : int64; y : uint64; out_cl : bool) {
    var x = xin
    if (copy || c.active) {
        var pc_in = GkClampArgs(nelem = uint(nin), lo = clamp_lo(c, true), hi = clamp_hi(c, true), scale = 1.0)
        if (copy) {
            enc_tower_clamp_copy_cls(g_cud.stream, fixed_array(xin, r.xc.dev), pc_in, rq_wgs(false, nin))
            x = r.xc.dev
        } else {
            enc_tower_clamp_cls(g_cud.stream, fixed_array(xin), pc_in, rq_wgs(false, nin))
        }
    }
    cut_mm32(r, bf16, x, y, n, dout, rows, woff, CUT_NO_BIAS, 0l)
    if (out_cl && c.active) {
        var pc_cl = GkClampArgs(nelem = uint(rows * dout), lo = c.omin, hi = c.omax, scale = 1.0)
        enc_tower_clamp_cls(g_cud.stream, fixed_array(y), pc_cl, rq_wgs(false, rows * dout))
    }
}

def private cut_g4v_ensure : bool
    => (ensure_fa_cm2_h64_bidir_cls() && ensure_tower_rms_cls() && ensure_tower_clamp_cls() && ensure_tower_rope2d_cls() && ensure_tower_post_add_cls()
        && ensure_tower_geglu_cls() && ensure_tower_clamp_cvt_cls() && ensure_tower_patch_im2col_cls() && ensure_tower_pos_add_2d_cls()
        && ensure_tower_clamp_copy_cls() && cut_f32_ensure(false) && cut_f32_ensure(true))

[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_VISION.md#vk-vision-chains")]
def private cuda_gemma4v_chain(t : Gemma4vTower; var s : Gemma4vState; planes : array<float>; w, h, npos : int64) : bool {   // nolint:STYLE038 - the dispatch chain is the shape
    var r & = g_cut_g4v
    return false if (!cut_vis_gate(r, t.q8, t.dev_backend))
    let f32w = !t.q8
    let d = t.n_embd
    let ff = t.n_ff
    let hs = d / t.n_head
    let pd = t.patch_dim
    let n_cols = w / t.patch
    let n_rows = h / t.patch
    let nout = (n_cols / t.merge) * (n_rows / t.merge)
    let vis = TowerVisTail(nout = nout, gw = n_cols, merge = t.merge, d = d, proj = t.proj_dim, soft_norm_w = -1l, proj_w = t.proj_w, proj_bf16 = t.proj_bf16, eps = t.eps,
        affine = true, scale = sqrt(float(d)), std = t.has_std, std_bias = t.std_bias, std_scale = t.std_scale, clamp = gemma4v_clamp_at(t, t.proj_clamp))
    if (hs != 64l || d % 64l != 0l || ff % 64l != 0l || npos > CUT_MAX_ENCODE_ROWS || npos <= 0l || npos != n_cols * n_rows
            || n_cols % t.merge != 0l || n_rows % t.merge != 0l) {
        return cut_decline(r, CudaTowerDecline.shape)
    }
    return false if (!cut_arm(r))
    if (!cut_g4v_ensure() || !cut_vis_tail_ensure(vis)) {
        return cut_decline(r, CudaTowerDecline.device)
    }
    let key = cut_key(t, t.n_layer)
    if (r.key != key) {
        cut_release(r)
        if (!cut_upload_planes(r, key, t.blob, t.wblob, max(hs, d)) || (!f32w && !cut_upload_q8(r, t, CUT_G4V_PIECE))) {
            return cut_decline(r, CudaTowerDecline.memory)
        }
    }
    if (!cut_scratch(r, npos, d, ff, 0l, max(d, t.proj_dim)) || !cut_vis_bufs(r, w, h, pd)) {
        return false
    }
    let c = r.cap_rows
    cut_grow(r, r.q2, c * d * 4l)
    cut_grow(r, r.k2, c * d * 4l)
    cut_grow(r, r.v2, c * d * 4l)
    cut_grow(r, r.k16, c * d * 2l)
    cut_grow(r, r.v16, c * d * 2l)
    cut_grow(r, r.tabs, c * hs * 4l)
    if (f32w) {
        cut_grow(r, r.xc, c * d * 4l)
    }
    return cut_decline(r, CudaTowerDecline.memory) if (r.oom)
    var pq, pff : CutPick
    if (!f32w) {
        pq = cut_pick(r, d, npos, d, false)
        pff = cut_pick(r, ff, npos, d, false)
        if (!cut_pick_ensure(pq) || !cut_pick_ensure(pff)) {
            return cut_decline(r, CudaTowerDecline.device)
        }
        let nrec = t.n_layer * GEMMA4V_GEMMS
        cut_sched_begin(nrec, t.n_layer * (5l * pq.groups + 2l * pff.groups))   // the down site rides pq's d-wide column
        var recs : Q8Region[int(GEMMA4V_GEMMS)]
        cut_sched_walk(nrec) $(rid : int64) : tuple<wblk0 : int64; rows : int64; groups : int64> {
            let g = rid % GEMMA4V_GEMMS
            if (g == 0l) {
                gemma4v_block_regions(t, rid / GEMMA4V_GEMMS, recs)
            }
            return (wblk0 = recs[g].woff / kq_elems(KqFmt.q8), rows = npos, groups = g == GEMMA4V_GEMM_GATE || g == GEMMA4V_GEMM_UP ? pff.groups : pq.groups)
        }
        return false if (!cut_sched_up(r))
    }
    let pairs = hs / 4l
    g_cut_rope_tabs |> reserve(npos * 4l * pairs)
    g_cut_rope_tabs |> resize(npos * 4l * pairs)
    gemma4v_pack_rope_tabs(s, npos, pairs, unsafe(addr(g_cut_rope_tabs[0])))
    cut_htod(r.tabs.dev, unsafe(addr<void?>(g_cut_rope_tabs[0])), npos * 4l * pairs * 4l)
    cut_planes_up(r, planes, w, h)
    let nd = npos * d
    let nff = npos * ff
    let wg_rope = cut_rows_wgs(npos * t.n_head * pairs)
    var pc_fa = FaArgs(rows = uint(npos), w0 = 0u, qd = uint(d), kvd = uint(d), kv_mul = 1u, kvbase = 0u, scale = 1.0, window = 0u, sinkoff = 0u)
    var pc_rope = GkRope2dArgs(nheads = uint(t.n_head), hs = uint(hs), rows = uint(npos))
    var st = g_cud.stream
    cut_vis_im2col(r, w, t.patch, pd, npos, 2.0, -1.0)
    cut_mm32(r, t.patch_bf16, r.col.dev, r.x.dev, pd, d, npos, t.patch_w, CUT_NO_BIAS, 0l)
    var pc_pos = GkPosAdd2DArgs(dim = uint(d), ncols = uint(n_cols), pos_entries = uint(t.pos_entries), total = uint(nd), tboff = uint(t.pos_w))
    enc_tower_pos_add_2d_cls(st, fixed_array(r.x.dev, r.norms.dev), pc_pos, cut_rows_wgs(nd))
    for (l in range64(t.n_layer)) {
        let o = gemma4v_block_offs(t, l)
        let cq = gemma4v_clamp_at(t, o.clamp[GEMMA4V_GEMM_Q])
        let ck = gemma4v_clamp_at(t, o.clamp[GEMMA4V_GEMM_K])
        let cv = gemma4v_clamp_at(t, o.clamp[GEMMA4V_GEMM_V])
        let co = gemma4v_clamp_at(t, o.clamp[GEMMA4V_GEMM_O])
        let cg = gemma4v_clamp_at(t, o.clamp[GEMMA4V_GEMM_GATE])
        let cu = gemma4v_clamp_at(t, o.clamp[GEMMA4V_GEMM_UP])
        let cd = gemma4v_clamp_at(t, o.clamp[GEMMA4V_GEMM_DOWN])
        let rb = l * GEMMA4V_GEMMS
        if (l == 0l) {
            var pc = gk_rms_args(uint(d), uint(npos), t.eps, uint(o.ln1))
            enc_tower_rms_cls(st, fixed_array(r.x.dev, r.xb.dev, r.norms.dev), pc, npos)
        }
        if (f32w) {
            cut_mm_clamped32(r, cq, true, t.blk_bf16, r.xb.dev, nd, d, d, npos, o.w[GEMMA4V_GEMM_Q], r.q.dev, true)
            cut_mm_clamped32(r, ck, true, t.blk_bf16, r.xb.dev, nd, d, d, npos, o.w[GEMMA4V_GEMM_K], r.k.dev, true)
            cut_mm_clamped32(r, cv, true, t.blk_bf16, r.xb.dev, nd, d, d, npos, o.w[GEMMA4V_GEMM_V], r.v.dev, true)
        } else {
            cut_mm_clamped_q8(r, cq, r.xb.dev, nd, pq, rb + GEMMA4V_GEMM_Q, d, d, npos, r.q.dev, true)
            cut_mm_clamped_q8(r, ck, r.xb.dev, nd, pq, rb + GEMMA4V_GEMM_K, d, d, npos, r.k.dev, true)
            cut_mm_clamped_q8(r, cv, r.xb.dev, nd, pq, rb + GEMMA4V_GEMM_V, d, d, npos, r.v.dev, true)
        }
        var pc_h = gk_rms_args(uint(hs), uint(npos * t.n_head), t.eps, uint(o.q_norm))
        enc_tower_rms_cls(st, fixed_array(r.q.dev, r.q2.dev, r.norms.dev), pc_h, npos * t.n_head)
        pc_h.lnoff = uint(o.k_norm)
        enc_tower_rms_cls(st, fixed_array(r.k.dev, r.k2.dev, r.norms.dev), pc_h, npos * t.n_head)
        pc_h.lnoff = uint(r.ones_off)
        enc_tower_rms_cls(st, fixed_array(r.v.dev, r.v2.dev, r.norms.dev), pc_h, npos * t.n_head)
        enc_tower_rope2d_cls(st, fixed_array(r.q2.dev, r.tabs.dev), pc_rope, wg_rope)
        enc_tower_rope2d_cls(st, fixed_array(r.k2.dev, r.tabs.dev), pc_rope, wg_rope)
        var pc_cvt = gk_f16_rows_args(uint(nd))
        enc_tower_clamp_cvt_cls(st, fixed_array(r.k2.dev, r.k16.dev), pc_cvt, rq_wgs(false, nd))
        enc_tower_clamp_cvt_cls(st, fixed_array(r.v2.dev, r.v16.dev), pc_cvt, rq_wgs(false, nd))
        cud_fa_enc(st, hs, false, false, false, fixed_array(r.q2.dev, r.k16.dev, r.v16.dev, r.att.dev), 0ul, pc_fa, fa_stamp_wgs(false, t.n_head, npos), [bidir = true])
        if (f32w) {
            cut_mm_clamped32(r, co, false, t.blk_bf16, r.att.dev, nd, d, d, npos, o.w[GEMMA4V_GEMM_O], r.yo.dev, true)
        } else {
            cut_mm_clamped_q8(r, co, r.att.dev, nd, pq, rb + GEMMA4V_GEMM_O, d, d, npos, r.yo.dev, true)
        }
        var pc_post = gk_add_post_rms_args(uint(d), uint(npos), t.eps, uint(o.attn_post), true, uint(o.ln2))
        enc_tower_post_add_cls(st, fixed_array(r.x.dev, r.yo.dev, r.xb.dev, r.norms.dev), pc_post, npos)
        if (f32w) {
            cut_mm_clamped32(r, cg, true, t.blk_bf16, r.xb.dev, nd, d, ff, npos, o.w[GEMMA4V_GEMM_GATE], r.hg.dev, false)
            cut_mm_clamped32(r, cu, true, t.blk_bf16, r.xb.dev, nd, d, ff, npos, o.w[GEMMA4V_GEMM_UP], r.hu.dev, false)
        } else {
            cut_mm_clamped_q8(r, cg, r.xb.dev, nd, pff, rb + GEMMA4V_GEMM_GATE, d, ff, npos, r.hg.dev, false)
            cut_mm_clamped_q8(r, cu, r.xb.dev, nd, pff, rb + GEMMA4V_GEMM_UP, d, ff, npos, r.hu.dev, false)
        }
        var pc_gg = GkGegluArgs(nelem = uint(nff), glo = clamp_lo(cg, false), ghi = clamp_hi(cg, false), ulo = clamp_lo(cu, false), uhi = clamp_hi(cu, false),
            dlo = clamp_lo(cd, true), dhi = clamp_hi(cd, true))
        enc_tower_geglu_cls(st, fixed_array(r.hg.dev, r.hu.dev), pc_gg, rq_wgs(false, nff))
        if (f32w) {
            cut_mm_clamped32(r, cd, false, t.blk_bf16, r.hg.dev, nff, ff, d, npos, o.w[GEMMA4V_GEMM_DOWN], r.yd.dev, true)
        } else {
            cut_mm_clamped_q8(r, cd, r.hg.dev, nff, pq, rb + GEMMA4V_GEMM_DOWN, ff, d, npos, r.yd.dev, true)
        }
        let last = l + 1l == t.n_layer
        pc_post = gk_add_post_rms_args(uint(d), uint(npos), t.eps, uint(o.ffn_post), !last, last ? 0u : uint(gemma4v_block_offs(t, l + 1l).ln1))
        enc_tower_post_add_cls(st, fixed_array(r.x.dev, r.yd.dev, r.xb.dev, r.norms.dev), pc_post, npos)
    }
    cut_vis_tail_enc(r, r.x.dev, vis)
    g_cut_stem_encodes++
    g_cut_tail_encodes++
    ensure_length(s.tail, nout * t.proj_dim)
    s.tail_ready = cut_served(r, r.tail_proj.dev, s.tail, nout * t.proj_dim * 4l, npos, t.n_layer)
    return s.tail_ready
}

def private cut_uv_ensure(patch_bf16, proj_bf16 : bool) : bool
    => (ensure_tower_patch_im2col_cls() && ensure_tower_ln_cls() && ensure_tower_pos_add_2d_cls() && ensure_tower_rms_cls()
        && cut_f32_ensure(patch_bf16) && cut_f32_ensure(proj_bf16))

[hot_path, unused_argument(s), arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_VISION.md#vk-vision-chains")]
def private cuda_gemma4uv_encode(t : Gemma4uvEmbedder; var s : Gemma4uvState; planes : array<float>; w, h, npos, n_cols : int64; var out : array<float>) : bool {
    var r & = g_cut_uv
    return cut_decline(r, CudaTowerDecline.knob) if (!g_cut_on)
    let d = t.n_embd
    let pd = t.patch_dim
    let proj = t.proj_dim
    if (d % 64l != 0l || npos > CUT_MAX_ENCODE_ROWS || npos <= 0l || npos != n_cols * (h / t.patch) || n_cols != w / t.patch) {
        return cut_decline(r, CudaTowerDecline.shape)
    }
    return false if (!cut_arm(r))
    if (!cut_uv_ensure(t.patch_bf16, t.proj_bf16)) {
        return cut_decline(r, CudaTowerDecline.device)
    }
    let key = cut_key(t, 0l)
    if (r.key != key) {
        cut_release(r)
        if (!cut_upload_planes(r, key, t.blob, t.wblob, d)) {
            return cut_decline(r, CudaTowerDecline.memory)
        }
    }
    if (!cut_scratch(r, npos, d, d, 0l, proj) || !cut_vis_bufs(r, w, h, pd)) {
        return false
    }
    cut_planes_up(r, planes, w, h)
    let nd = npos * d
    var st = g_cud.stream
    cut_vis_im2col(r, w, t.patch, pd, npos, 1.0, 0.0)
    var pc_ln1 = gk_ln_args(uint(pd), uint(npos), GEMMA4UV_LN_EPS, uint(t.ln1_w), uint(t.ln1_b))
    enc_tower_ln_cls(st, fixed_array(r.col.dev, r.col.dev, r.norms.dev), pc_ln1, npos)
    cut_mm32(r, t.patch_bf16, r.col.dev, r.x.dev, pd, d, npos, t.patch_w, t.patch_b, 0l)
    var pc_ln2 = gk_ln_args(uint(d), uint(npos), GEMMA4UV_LN_EPS, uint(t.ln2_w), uint(t.ln2_b))
    enc_tower_ln_cls(st, fixed_array(r.x.dev, r.xb.dev, r.norms.dev), pc_ln2, npos)
    var pc_pos = GkPosAdd2DArgs(dim = uint(d), ncols = uint(n_cols), pos_entries = uint(t.pos_entries), total = uint(nd), tboff = uint(t.pos_w))
    enc_tower_pos_add_2d_cls(st, fixed_array(r.xb.dev, r.norms.dev), pc_pos, cut_rows_wgs(nd))
    var pc_ln3 = gk_ln_args(uint(d), uint(npos), GEMMA4UV_LN_EPS, uint(t.ln3_w), uint(t.ln3_b))
    enc_tower_ln_cls(st, fixed_array(r.xb.dev, r.x.dev, r.norms.dev), pc_ln3, npos)
    var pc_rms = gk_rms_args(uint(d), uint(npos), t.rms_eps, uint(r.ones_off))
    enc_tower_rms_cls(st, fixed_array(r.x.dev, r.xb.dev, r.norms.dev), pc_rms, npos)
    cut_mm32(r, t.proj_bf16, r.xb.dev, r.tail_proj.dev, d, proj, npos, t.proj_w, CUT_NO_BIAS, 0l)
    g_cut_stem_encodes++
    return cut_served(r, r.tail_proj.dev, out, npos * proj * 4l, npos, 0l)
}

//! a merger MLP over `nout` rows: `xin` -> `hid` (din wide, tanh GELU) -> `dout` columns at `yoff` of `tail_proj`
def private cut_merger(r : CutResident; xin, hid : uint64; din, dout, nout, w1, b1, w2, b2, yoff : int64) {
    cut_mm32(r, false, xin, hid, din, din, nout, w1, b1, 0l)
    cut_act(hid, din, nout * din, BIAS_ACT_GELU_TANH)
    cut_mm32(r, false, hid, r.tail_proj.dev, din, dout, nout, w2, b2, yoff)
}

let private CUT_Q3V_PIECE = "cu.q3v"

[cold_path, arch(at = "../ARCHITECTURE_IMAGE_GPU_PLANES.md#tower-dev-block")]
def private cuda_qwen3v_bake(t : Qwen3vTower; var b : TowerDevBuild) : bool {
    var inscope regions <- qwen3v_all_regions(t)
    tower_bake_q8(b, CUT_Q3V_PIECE, t.qblob, t.qscales, regions)
    return true
}

def private cut_q3v_ensure(hsp : int64) : bool
    => (ensure_tower_ln_cls() && ensure_tower_ln16_cls() && ensure_tower_bias_act_cls() && ensure_tower_bias_act_cvt16_cls() && ensure_tower_rope_tab_cls()
        && ensure_tower_post_add_ln_cls() && ensure_tower_post_add_ln16_cls() && ensure_tower_patch_im2col_cls() && ensure_tower_stem_asm_cls()
        && ensure_tower_act_cls() && ensure_tower_clamp_cvt_cls() && cut_attn_ensure(hsp) && cut_f32_ensure(false))

[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_VISION.md#vk-vision-chains")]
def private cuda_qwen3v_chain(t : Qwen3vTower; var s : Qwen3vState; planes : array<float>; w, h, npos : int64) : bool {   // nolint:STYLE037,STYLE038 - the dispatch chain is the shape
    var r & = g_cut_q3v
    return false if (!cut_vis_gate(r, t.q8, t.dev_backend))
    let f32w = !t.q8
    let d = t.n_embd
    let ff = t.ff_pad
    let hs = d / t.n_head
    let hsp = hs <= 64l ? 64l : 128l
    let dp = t.n_head * hsp
    let nds = t.n_deepstack
    let pd = t.patch_dim
    let d4 = 4l * d
    let nout = npos / 4l
    let pw = w / t.patch
    let proj = t.proj_dim
    if (hs > 128l || hs % 2l != 0l || d % 64l != 0l || ff % 64l != 0l || npos > CUT_MAX_ENCODE_ROWS || npos <= 0l || npos % 4l != 0l
            || npos != pw * (h / t.patch) || pw % 2l != 0l || d > ff) {
        return cut_decline(r, CudaTowerDecline.shape)
    }
    return false if (!cut_arm(r))
    if (!cut_q3v_ensure(hsp)) {
        return cut_decline(r, CudaTowerDecline.device)
    }
    let key = cut_key(t, t.n_layer)
    if (r.key != key) {
        cut_release(r)
        if (!cut_upload_planes(r, key, t.blob, t.wblob, d) || (!f32w && !cut_upload_q8(r, t, CUT_Q3V_PIECE))) {
            return cut_decline(r, CudaTowerDecline.memory)
        }
    }
    if (!cut_scratch(r, npos, d, ff, dp, max(d4, (nds + 1l) * proj)) || !cut_vis_bufs(r, w, h, pd)) {
        return false
    }
    let c = r.cap_rows
    let half = hs / 2l
    cut_grow(r, r.qkv, 3l * c * d * 4l)
    cut_grow(r, r.cos, c * half * 4l)
    cut_grow(r, r.sin, c * half * 4l)
    let pos_rs = qwen3v_pos_resized(t, s)
    if (pos_rs) {
        cut_grow(r, r.pos_resized, c * d * 4l)
    }
    return cut_decline(r, CudaTowerDecline.memory) if (r.oom)
    var pqkv, pq, pff : CutPick
    if (!f32w) {
        pqkv = cut_pick(r, 3l * d, npos, d, false)
        pq = cut_pick(r, d, npos, d, false)
        pff = cut_pick(r, ff, npos, d, false)
        if (!cut_pick_ensure(pqkv) || !cut_pick_ensure(pq) || !cut_pick_ensure(pff)) {
            return cut_decline(r, CudaTowerDecline.device)
        }
        let nrec = t.n_layer * QWEN3V_GEMMS
        cut_sched_begin(nrec, t.n_layer * (pqkv.groups + 2l * pq.groups + pff.groups))   // the down site rides pq's d-wide column
        var recs : Q8Region[int(QWEN3V_GEMMS)]
        cut_sched_walk(nrec) $(rid : int64) : tuple<wblk0 : int64; rows : int64; groups : int64> {
            let g = rid % QWEN3V_GEMMS
            if (g == 0l) {
                qwen3v_block_regions(t, rid / QWEN3V_GEMMS, recs)
            }
            return (wblk0 = recs[g].woff / kq_elems(KqFmt.q8), rows = npos, groups = g == QWEN3V_GEMM_QKV ? pqkv.groups : (g == QWEN3V_GEMM_UP ? pff.groups : pq.groups))
        }
        return false if (!cut_sched_up(r))
    }
    let btab = npos * half * 4l
    let bslice = nout * proj * 4l
    unsafe {
        cut_planes_up(r, planes, w, h)
        if (pos_rs) {
            cut_htod(r.pos_resized.dev, addr<void?>(s.pos_rs[0]), npos * d * 4l)
        }
        cut_htod(r.cos.dev, addr<void?>(s.cos_t[0]), btab)
        cut_htod(r.sin.dev, addr<void?>(s.sin_t[0]), btab)
    }
    let nd = npos * d
    let nff = npos * ff
    let wg_rope = cut_rows_wgs(npos * t.n_head * half)
    var pc_rope = GkRopeTabArgs(rows = uint(npos), qs = uint(3l * d), col = 0u, d = uint(t.n_head * hs), dh = uint(hs), coff = 0u, soff = 0u)
    var pc_bqkv = GkBiasActArgs(d = uint(3l * d), nelem = uint(3l * nd), boff = 0u, act = BIAS_ACT_NONE)
    var pc_gelu = GkBiasActArgs(d = uint(ff), nelem = uint(nff), boff = 0u, act = BIAS_ACT_GELU_TANH)
    var st = g_cud.stream
    cut_vis_im2col(r, w, t.patch, pd, npos, 1.0, 0.0)
    cut_mm32(r, false, r.col.dev, r.yo.dev, pd, d, npos, t.patch_w, CUT_NO_BIAS, 0l)
    var pc_asm = GkStemAsmArgs(dim = uint(d), pw = uint(pw), total = uint(nd), boff = uint(t.patch_b), poff = pos_rs ? 0u : uint(t.pos_w))
    enc_tower_stem_asm_cls(st, fixed_array(r.yo.dev, r.norms.dev, pos_rs ? r.pos_resized.dev : r.norms.dev, r.x.dev), pc_asm, cut_rows_wgs(nd))
    for (l in range64(t.n_layer)) {
        let o = qwen3v_block_offs(t, l)
        let rb = l * QWEN3V_GEMMS
        if (l == 0l) {
            var pc = gk_ln_args(uint(d), uint(npos), t.eps, uint(o.ln1_w), uint(o.ln1_b))
            if (f32w) {
                enc_tower_ln_cls(st, fixed_array(r.x.dev, r.xb.dev, r.norms.dev), pc, npos)
            } else {
                enc_tower_ln16_cls(st, fixed_array(r.x.dev, r.norms.dev, r.xh.dev), pc, npos)
            }
        }
        if (f32w) {
            cut_mm32(r, false, r.xb.dev, r.qkv.dev, d, 3l * d, npos, o.w[QWEN3V_GEMM_QKV], CUT_NO_BIAS, 0l)
        } else {
            cut_mm_q8(r, pqkv, rb + QWEN3V_GEMM_QKV, d, 3l * d, npos, r.qkv.dev)
        }
        pc_bqkv.boff = uint(o.qkv_b)
        enc_tower_bias_act_cls(st, fixed_array(r.qkv.dev, r.norms.dev), pc_bqkv, rq_wgs(false, 3l * nd))
        pc_rope.col = 0u
        enc_tower_rope_tab_cls(st, fixed_array(r.qkv.dev, r.cos.dev, r.sin.dev), pc_rope, wg_rope)
        pc_rope.col = uint(d)
        enc_tower_rope_tab_cls(st, fixed_array(r.qkv.dev, r.cos.dev, r.sin.dev), pc_rope, wg_rope)
        cut_attn_padded(r, r.qkv.dev, r.qkv.dev, r.qkv.dev, 3l * d, npos, t.n_head, hs, hsp, CUT_NO_BIAS, CUT_NO_BIAS, CUT_NO_BIAS, 0l, d, 2l * d, !f32w)
        if (f32w) {
            cut_mm32(r, false, r.att.dev, r.yo.dev, d, d, npos, o.w[QWEN3V_GEMM_O], CUT_NO_BIAS, 0l)
        } else {
            cut_mm_q8(r, pq, rb + QWEN3V_GEMM_O, d, d, npos, r.yo.dev)
        }
        var pc_post = gk_add_ln_args(uint(d), uint(npos), t.eps, uint(o.out_b), true, uint(o.ln2_w), uint(o.ln2_b))
        if (f32w) {
            enc_tower_post_add_ln_cls(st, fixed_array(r.x.dev, r.yo.dev, r.xb.dev, r.norms.dev), pc_post, npos)
            cut_mm32(r, false, r.xb.dev, r.hg.dev, d, ff, npos, o.w[QWEN3V_GEMM_UP], CUT_NO_BIAS, 0l)
        } else {
            enc_tower_post_add_ln16_cls(st, fixed_array(r.x.dev, r.yo.dev, r.norms.dev, r.xh.dev), pc_post, npos)
            cut_mm_q8(r, pff, rb + QWEN3V_GEMM_UP, d, ff, npos, r.hg.dev)
        }
        pc_gelu.boff = uint(o.up_b)
        if (f32w) {
            enc_tower_bias_act_cls(st, fixed_array(r.hg.dev, r.norms.dev), pc_gelu, rq_wgs(false, nff))
            cut_mm32(r, false, r.hg.dev, r.yd.dev, ff, d, npos, o.w[QWEN3V_GEMM_DOWN], CUT_NO_BIAS, 0l)
        } else {
            enc_tower_bias_act_cvt16_cls(st, fixed_array(r.hg.dev, r.norms.dev, r.xh.dev), pc_gelu, rq_wgs(false, nff))
            cut_mm_q8(r, pq, rb + QWEN3V_GEMM_DOWN, ff, d, npos, r.yd.dev)
        }
        let last = l + 1l == t.n_layer
        let nx = last ? o : qwen3v_block_offs(t, l + 1l)
        pc_post = gk_add_ln_args(uint(d), uint(npos), t.eps, uint(o.down_b), true, uint(last ? t.post_ln_w : nx.ln1_w), uint(last ? t.post_ln_b : nx.ln1_b))
        let k = qwen3v_tap_of(t, l)
        let tap = k >= 0l && k < nds
        if (f32w || last || tap) {
            enc_tower_post_add_ln_cls(st, fixed_array(r.x.dev, r.yd.dev, r.xb.dev, r.norms.dev), pc_post, npos)
            if (tap && !last && !f32w) {   // the tap reads x; the next block's feed off the normed rows
                var pc_cvt = gk_f16_rows_args(uint(nd))
                enc_tower_clamp_cvt_cls(st, fixed_array(r.xb.dev, r.xh.dev), pc_cvt, rq_wgs(false, nd))
            }
        } else {
            enc_tower_post_add_ln16_cls(st, fixed_array(r.x.dev, r.yd.dev, r.norms.dev, r.xh.dev), pc_post, npos)
        }
        if (tap) {
            let fo = t.ds_f + k * t.ds_f_stride
            let wo = t.ds_w + k * t.ds_w_stride
            var pc_tln = gk_ln_args(uint(d4), uint(nout), t.eps, uint(fo), uint(fo + d4))
            enc_tower_ln_cls(st, fixed_array(r.x.dev, r.tail_out.dev, r.norms.dev), pc_tln, nout)
            cut_merger(r, r.tail_out.dev, r.hg.dev, d4, proj, nout, wo, fo + 8l * d, wo + 16l * d * d, fo + 12l * d, (k + 1l) * nout * proj)
        }
    }
    cut_merger(r, r.xb.dev, r.tail_out.dev, d4, proj, nout, t.mm0_w, t.mm0_b, t.mm2_w, t.mm2_b, 0l)
    g_cut_stem_encodes++
    g_cut_tail_encodes++
    if (nds > 0l) {
        cut_dtoh(unsafe(addr<void?>(s.ds_stash[0])), r.tail_proj.dev + uint64(bslice), nds * bslice)
    }
    return cut_served(r, r.tail_proj.dev, s.ds_slice, bslice, npos, t.n_layer)
}

def private cut_q25v_ensure : bool
    => (ensure_f16_gemm_cm2_w_cls() && ensure_fa_cm2_h128_bidir_cls() && ensure_tower_clamp_cvt_cls() && ensure_tower_rms_cls() && ensure_tower_bias_act_cls()
        && ensure_tower_rope_tab_cls() && ensure_tower_hpad_cls() && ensure_tower_hpad16_cls() && ensure_tower_hunpad_cls() && ensure_tower_win_attn_cls()
        && ensure_tower_post_add_rms_cls() && ensure_tower_bias_gate16_cls() && ensure_tower_patch_im2col_cls() && cut_f32_ensure(false)
        && ensure_tts_row_gather_cls() && ensure_tower_act_cls())

//! the f16 tile over the halfword plane's rows at `woff`, an element offset from the uploaded plane's base, the feed `xh` into y [rows x dout]
def private cut_mm16(r : CutResident; y : uint64; n, dout, rows, woff : int64) {
    var pc = F16GemmArgs(n = uint(n), d = uint(dout), rows = uint(rows), woff = uint(woff), ybase = 0u, ystride = uint(dout), ksplit = 0u)
    enc_f16_gemm_cm2_w_cls(g_cud.stream, fixed_array(r.wb.dev, r.xh.dev, y), pc, f16_gemm_cm2_wgs(rows, dout))
}

[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_VISION.md#vk-vision-chains")]
def private cuda_qwen25v_chain(t : Qwen25vTower; var s : Qwen25vState; planes : array<float>; w, h, npos : int64) : bool {   // nolint:STYLE038 - the dispatch chain is the shape, its two attention arms the routing
    var r & = g_cut_q25v
    return cut_decline(r, CudaTowerDecline.knob) if (!g_cut_on)
    return cut_decline(r, CudaTowerDecline.quant_mode) if (!(t.wblob.n > 0l && !t.blk_bf16))
    let d = t.n_embd
    let ff = t.ff_pad
    let hs = d / t.n_head
    let hsp = hs <= 64l ? 64l : 128l
    let dp = t.n_head * hsp
    let pd = q25v_patch_pad(t)
    let d4 = 4l * d
    let nout = npos / 4l
    let nwin = long_length(s.win_lo) - 1l
    let has_win = t.n_wa_pattern > 1l && t.n_layer > 0l
    let wmax = has_win ? qwen25v_window_widest(s) : 0l
    if (hs > 128l || hsp != 128l || hs % 2l != 0l || d % 64l != 0l || ff % 64l != 0l || npos > CUT_MAX_ENCODE_ROWS || npos <= 0l || nwin <= 0l
            || npos % 4l != 0l || npos != (w / t.patch) * (h / t.patch) || (has_win && (wmax > 64l || hs != 80l))) {
        return cut_decline(r, CudaTowerDecline.shape)
    }
    return false if (!cut_arm(r))
    if (!cut_q25v_ensure()) {
        return cut_decline(r, CudaTowerDecline.device)
    }
    let w_off = t.layer_w
    let w_n = t.n_layer * t.layer_w_stride
    let key = cut_key(t, t.n_layer)
    if (r.key != key) {
        cut_release(r)
        r.key = key
        r.oom = false
        cut_up(r, r.norms, unsafe(reinterpret<void?>(t.blob.p)), t.blob.n * 4l)
        cut_up(r, r.wb, unsafe(reinterpret<void?>(plane_at(t.wblob, w_off))), w_n * 2l)
        r.ones_off = t.blob.n
        if (r.oom) {
            cut_release(r)
            return cut_decline(r, CudaTowerDecline.memory)
        }
    }
    if (!cut_scratch(r, npos, d, ff, dp, max(d4, t.proj_dim)) || !cut_vis_bufs(r, w, h, pd)) {
        return false
    }
    let c = r.cap_rows
    let half = hs / 2l
    cut_grow(r, r.cos, c * half * 4l)
    cut_grow(r, r.sin, c * half * 4l)
    cut_grow(r, r.win_sort, c * 4l)
    cut_grow(r, r.win_table, (nwin + 1l) * 4l)
    return cut_decline(r, CudaTowerDecline.memory) if (r.oom)
    g_cut_win_table |> reserve(nwin + 1l)
    g_cut_win_table |> resize(nwin + 1l)
    qwen25v_window_table(s, unsafe(addr(g_cut_win_table[0])))
    g_cut_win_sort |> reserve(npos)
    g_cut_win_sort |> resize(npos)
    for (i in range64(npos)) {
        g_cut_win_sort[i] = uint(s.src_of[i])
    }
    let btab = npos * half * 4l
    unsafe {
        cut_planes_up(r, planes, w, h)
        cut_htod(r.win_sort.dev, addr<void?>(g_cut_win_sort[0]), npos * 4l)
        cut_htod(r.cos.dev, addr<void?>(s.cos_t[0]), btab)
        cut_htod(r.sin.dev, addr<void?>(s.sin_t[0]), btab)
        cut_htod(r.win_table.dev, addr<void?>(g_cut_win_table[0]), (nwin + 1l) * 4l)
    }
    let nd = npos * d
    let nff = npos * ff
    let wg_rope = cut_rows_wgs(npos * t.n_head * half)
    let scale = 1.0 / sqrt(float(hs))
    var pc_rope = GkRopeTabArgs(rows = uint(npos), qs = uint(d), col = 0u, d = uint(t.n_head * hs), dh = uint(hs), coff = 0u, soff = 0u)
    var pc_glu = GkBiasGateArgs(d = uint(ff), nelem = uint(nff), goff = 0u, uoff = 0u)
    var pc_win = GkWinAttnArgs(nheads = uint(t.n_head), hs = uint(hs), scale = scale)
    var st = g_cud.stream
    cut_vis_im2col(r, w, t.patch, pd, npos, 1.0, 0.0)
    cut_mm32(r, false, r.col.dev, r.yo.dev, pd, d, npos, t.patch_w, CUT_NO_BIAS, 0l)
    var pc_g = GkRowGatherArgs(c = uint(d), rows = uint(npos), aoff = 0u, boff = 0u, coff = 0u, out_off = 0u, add_b = 0u, add_c = 0u, scale = 1.0)
    enc_tts_row_gather_cls(st, fixed_array(r.yo.dev, r.win_sort.dev, r.norms.dev, r.norms.dev, r.x.dev), pc_g, nd)
    for (l in range64(t.n_layer)) {
        let o = qwen25v_block_offs(t, l)
        let full = qwen25v_layer_full(t, l)
        if (l == 0l) {
            var pc = gk_rms_args(uint(d), uint(npos), t.eps, uint(o.ln1_w))
            enc_tower_rms_cls(st, fixed_array(r.x.dev, r.xb.dev, r.norms.dev), pc, npos)
        }
        cut_cvt16(r, r.xb.dev, nd)
        cut_mm16(r, r.q.dev, d, d, npos, o.w[QWEN25V_GEMM_Q] - w_off)
        cut_bias_act(r, r.q.dev, d, nd, o.b[QWEN25V_GEMM_Q], BIAS_ACT_NONE)
        cut_mm16(r, r.k.dev, d, d, npos, o.w[QWEN25V_GEMM_K] - w_off)
        cut_bias_act(r, r.k.dev, d, nd, o.b[QWEN25V_GEMM_K], BIAS_ACT_NONE)
        cut_mm16(r, r.v.dev, d, d, npos, o.w[QWEN25V_GEMM_V] - w_off)
        cut_bias_act(r, r.v.dev, d, nd, o.b[QWEN25V_GEMM_V], BIAS_ACT_NONE)
        enc_tower_rope_tab_cls(st, fixed_array(r.q.dev, r.cos.dev, r.sin.dev), pc_rope, wg_rope)
        enc_tower_rope_tab_cls(st, fixed_array(r.k.dev, r.cos.dev, r.sin.dev), pc_rope, wg_rope)
        if (full) {
            cut_attn_padded(r, r.q.dev, r.k.dev, r.v.dev, d, npos, t.n_head, hs, hsp, CUT_NO_BIAS, CUT_NO_BIAS, CUT_NO_BIAS, 0l, 0l, 0l, false)
        } else {
            enc_tower_win_attn_cls(st, fixed_array(r.q.dev, r.k.dev, r.v.dev, r.att.dev, r.win_table.dev), pc_win, nwin * t.n_head)
        }
        cut_cvt16(r, r.att.dev, nd)
        cut_mm16(r, r.yo.dev, d, d, npos, o.w[QWEN25V_GEMM_O] - w_off)
        var pc_post = gk_add_ln_args(uint(d), uint(npos), t.eps, uint(o.b[QWEN25V_GEMM_O]), true, uint(o.ln2_w), 0u)
        enc_tower_post_add_rms_cls(st, fixed_array(r.x.dev, r.yo.dev, r.xb.dev, r.norms.dev), pc_post, npos)
        cut_cvt16(r, r.xb.dev, nd)
        cut_mm16(r, r.hg.dev, d, ff, npos, o.w[QWEN25V_GEMM_GATE] - w_off)
        cut_mm16(r, r.hu.dev, d, ff, npos, o.w[QWEN25V_GEMM_UP] - w_off)
        pc_glu.goff = uint(o.b[QWEN25V_GEMM_GATE])
        pc_glu.uoff = uint(o.b[QWEN25V_GEMM_UP])
        enc_tower_bias_gate16_cls(st, fixed_array(r.hg.dev, r.hu.dev, r.norms.dev, r.xh.dev), pc_glu, rq_wgs(false, nff))
        cut_mm16(r, r.yd.dev, ff, d, npos, o.w[QWEN25V_GEMM_DOWN] - w_off)
        let last = l + 1l == t.n_layer
        pc_post = gk_add_ln_args(uint(d), uint(npos), t.eps, uint(o.b[QWEN25V_GEMM_DOWN]), true, uint(last ? t.post_ln_w : qwen25v_block_offs(t, l + 1l).ln1_w), 0u)
        enc_tower_post_add_rms_cls(st, fixed_array(r.x.dev, r.yd.dev, r.xb.dev, r.norms.dev), pc_post, npos)
    }
    cut_merger(r, r.xb.dev, r.tail_out.dev, d4, t.proj_dim, nout, t.mm0_w, t.mm0_b, t.mm2_w, t.mm2_b, 0l)
    g_cut_stem_encodes++
    g_cut_tail_encodes++
    ensure_length(s.mout, nout * t.proj_dim)
    return cut_served(r, r.tail_proj.dev, s.mout, nout * t.proj_dim * 4l, npos, t.n_layer)
}

//! the q8 lane's pick before the family's own classes: the knob, the device, the q8 cm2 tile at every column `cut_pick` maps to
def private cut_q8_pick_arms(q8 : bool) : bool {
    return false if (!q8 || !g_cut_on || !cuda_tower_arms())
    cud_bind()
    return (cud_cm2_ensure(int(KqFmt.q8), CUD_CM2_TC_L) && cud_cm2_ensure(int(KqFmt.q8), CUD_CM2_TC_M)
        && cud_cm2_ensure(int(KqFmt.q8), CUD_CM2_TC_S))
}

//! the head width is the model's, unknown before the load: the pick builds the attention classes at both padded widths the chains serve
def private cuda_gemma3v_serves_model(q8 : bool) : bool
    => (cut_q8_pick_arms(q8) && cut_ln_ensure(false, 64l) && cut_ln_ensure(false, 128l) && ensure_splitk_reduce_cls()
        && ensure_tower_patch_im2col_cls() && cut_vis_tail_ensure(TowerVisTail()))

def private cuda_gemma4v_serves_model(q8 : bool) : bool
    => cut_q8_pick_arms(q8) && cut_g4v_ensure() && cut_vis_tail_ensure(TowerVisTail(proj_bf16 = true, affine = true, clamp = Clamp(active = true)))

def private cuda_qwen3v_serves_model(q8 : bool) : bool => cut_q8_pick_arms(q8) && cut_q3v_ensure(64l) && cut_q3v_ensure(128l)

//! the CUDA home's vision seats - the chain hooks and the q8 flavors - at [init] under the CUDA ask, or when `set_gpu_home("cuda")` moves the tier here
[arch(at = "../ARCHITECTURE_MEDIA.md#tower-weight-lane")]
def cuda_tower_install_vision {
    register_gemma3v_gpu(@@cuda_gemma3v_chain, @@cuda_tower_on, false)
    register_gemma3v_flavor(TowerDev.cuda, @@cuda_gemma3v_serves_model, @@cuda_gemma3v_bake)
    register_gemma4v_gpu(@@cuda_gemma4v_chain, @@cuda_tower_on, false)
    register_gemma4v_flavor(TowerDev.cuda, @@cuda_gemma4v_serves_model, @@cuda_gemma4v_bake)
    register_gemma4uv_gpu(@@cuda_gemma4uv_encode)
    register_qwen3v_gpu(@@cuda_qwen3v_chain, @@cuda_tower_on, false)
    register_qwen3v_flavor(TowerDev.cuda, @@cuda_qwen3v_serves_model, @@cuda_qwen3v_bake)
    register_qwen25v_gpu(@@cuda_qwen25v_chain)
}

[init]
def dasllama_cuda_tower_register {
    register_gpu_home("cuda", @@cuda_tower_install_vision)
    register_cud_drop_hook(@@cuda_tower_shutdown)
    cud_on_shutdown(@@cuda_tower_shutdown)
    return if (!gpu_want_cuda())
    cuda_tower_install_vision()
}
