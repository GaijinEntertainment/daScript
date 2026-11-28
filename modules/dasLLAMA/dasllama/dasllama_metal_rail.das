options gen2
options indenting = 4
options _dasllama_internal = true
options loop_unroll_cost = 0    // kernel source: the emitter gets loops and locals as written
options disable_forward

//! The Metal home's tower rail: the encoder a shared chain body records on, the planes by role, and the class state a
//! `[metal_rail_dispatch]` stamp's generated `ensure_<stamp>` / `enc_<stamp>(rail, roles, pc, ...)` runs over.
//! `dasllama_vulkan_common` and `dasllama_cuda_common` carry the same surface over `VtRail` and `CutRail`.
module dasllama_metal_rail shared public

require dasllama/dasllama_lint public
require math
require metal/msl_shader
require metal/das_metal_boost
require dasllama/dasllama_gpu_rail public
require dasllama/dasllama_tower   // TowerDev
require dasllama/dasllama_metal_common public

var private g_mtl_epoch = 1
var private g_mtl_psos : array<MetalComputePipeline?>

//! a `[metal_rail_dispatch]` family's pipelines, one a stamp by ordinal, compiled at the stamp's first ensure
struct MtlClass {
    ready : bool
    epoch : int
    psos : array<MetalComputePipeline?>
    tgmem : array<uint64>   //! ... the threadgroup bytes its dispatch binds
    failed : array<bool>    //! ... its source did not compile on this device
}

def mtl_class_init(var c : MtlClass) {
    c.ready = true
    c.epoch = g_mtl_epoch
}

def mtl_fn_made(c : MtlClass; ord : int) : bool =>
    c.ready && c.epoch == g_mtl_epoch && ord < length(c.psos) && c.psos[ord] != null

[cold_path]
def mtl_class_fn(var c : MtlClass; ord : int; source, entry : string; fastmath : bool; tgmem : uint64; name : string) : bool {
    //! the pipeline of stamp `ord` compiled from its kernel's source; false where the device refuses it, once. The census and
    //! the source dump name the stamp (`name`): a rail class's kernel method is `run` on every stamp, so its entry names none
    if (c.epoch != g_mtl_epoch) {
        c.psos |> clear()
        c.tgmem |> clear()
        c.failed |> clear()
        c.epoch = g_mtl_epoch
    }
    while (length(c.psos) <= ord) {
        c.psos |> push(null)
        c.tgmem |> push(0ul)
        c.failed |> push(false)
    }
    return false if (c.failed[ord])
    var err : string
    var pso = pso_from_source(g_dev, source, entry, fastmath, err, name)
    if (pso == null) {
        c.failed[ord] = true
        to_log(LOG_ERROR, "dasLLAMA metal rail: `{name}` compile failed - {err}\n")
        return false
    }
    kcov_register(pso, name)
    c.psos[ord] = pso
    c.tgmem[ord] = tgmem
    g_mtl_psos |> push(pso)
    return true
}

var private g_mtl_u32 : table<uint; MetalBuffer?>   //! a u32 uniform a value: the K and N words the home's own tiles bind as buffers

let private MTL_U32_CAP = 1024   //! the values kept: past it (row counts differ a call) the table is dropped at a shot's end

[cold_path]
def private mtl_rail_u32_make(v : uint) : MetalBuffer? {
    var buf = metal_new_buffer_untracked(g_dev, 4ul)
    if (buf == null) {
        panic("dasLLAMA metal rail: a uniform buffer could not be made")
    }
    *unsafe(reinterpret<uint?>(metal_buffer_contents(buf))) = v
    residency_note(buf)
    g_mtl_u32[v] = buf
    return buf
}

//! the uniform buffer holding `v`, made once a value and kept to the rail's shutdown
def mtl_rail_u32(v : uint) : MetalBuffer? {
    var have = g_mtl_u32?[v] ?? null
    return have if (have != null)
    return mtl_rail_u32_make(v)
}

let MTL_RAIL_TMPS = 2   //! a site's own rows: its input re-laid, its output landed

var private g_mtl_tmp : MetalBuffer? [MTL_RAIL_TMPS]   //! a site's own rows where the caller's are not a whole tile's
var private g_mtl_tmp_bytes : int64[MTL_RAIL_TMPS]
var private g_mtl_retired : array<MetalBuffer?>   //! temps a longer site outgrew while a command buffer still read them

[cold_path]
def private mtl_rail_tmp_grow(bytes : int64; slot : int) {
    if (g_mtl_tmp[slot] != null) {
        g_mtl_retired |> push(g_mtl_tmp[slot])
    }
    g_mtl_tmp_bytes[slot] = bytes + bytes / 4l
    g_mtl_tmp[slot] = metal_new_buffer(g_dev, uint64(g_mtl_tmp_bytes[slot]))
    if (g_mtl_tmp[slot] == null) {
        panic("dasLLAMA metal rail: a site's own rows of {g_mtl_tmp_bytes[slot]} bytes could not be made")
    }
    residency_note(g_mtl_tmp[slot])
}

//! rows `slot` of at least `bytes`, a site's own: whole tiles it lands or reads ahead of a copy; an outgrown one is kept to the shot's end
def mtl_rail_tmp(bytes : int64; slot : int = 0) : RailBuf {
    if (g_mtl_tmp_bytes[slot] < bytes) {
        mtl_rail_tmp_grow(bytes, slot)
    }
    return RailBuf(dev = intptr(g_mtl_tmp[slot]), bytes = g_mtl_tmp_bytes[slot])
}

[cold_path]
def private mtl_rail_retire {
    for (b in g_mtl_retired) {
        var buf = b
        metal_release(buf)
    }
    g_mtl_retired |> clear()
}

[cold_path]
def private mtl_rail_u32_drop {
    for (b in values(g_mtl_u32)) {
        var buf = b
        metal_release(buf)
    }
    g_mtl_u32 |> clear()
}

//! every rail pipeline, uniform and landing row released: the classes recompile at their next ensure
[cold_path]
def mtl_rail_shutdown {
    for (p in g_mtl_psos) {
        metal_release(p)
    }
    g_mtl_psos |> clear()
    mtl_rail_u32_drop()
    mtl_rail_retire()
    for (slot in range(MTL_RAIL_TMPS)) {
        if (g_mtl_tmp[slot] != null) {
            metal_release(g_mtl_tmp[slot])
            g_mtl_tmp[slot] = null
            g_mtl_tmp_bytes[slot] = 0l
        }
    }
    g_mtl_epoch++
}

//! this context's rail pipelines down at its end
[finalize, arch(at="../ARCHITECTURE_GPU_METAL_LIFECYCLE.md#metal-family-lifecycle")]
def private mtl_rail_context_shutdown {
    mtl_rail_shutdown()
}

//! one GEMM site's pick for an encode
struct MtPick {
    nsplit : int64
    ksplit : int64
}

//! the Metal home's tower rail: the encoder a chain body records on, the planes by role (`TowerBuf`; a seat's `dev` the
//! `MetalBuffer?`), the lane the chain runs
struct MtRail {
    enc : MetalComputeEncoder?
    bufs : RailBuf[TOWER_BUFS]
    lane : RailLane
    norms_base : int64
    ones_off : int64
    sites : MtPick[RAIL_SITES]
    binds : RailSiteBind[RAIL_SITES]
    @scratch @exact_size maps : array<uint>
}

//! the buffer a seat holds
def rail_mtl_buf(b : RailBuf) : MetalBuffer? => unsafe(reinterpret<MetalBuffer?>(b.dev))

def mtl_rail_dispatch(c : MtlClass; ord : int; rl : MtRail; roles : auto(R); bmask, rmask, wmask : uint; var pc : auto(PT)&;
                      kslot : int; views : uint64; nviews : int; groups : uint; tg : int) {
    //! stamp `ord` of `c` over the rail's planes at `roles` - a role a binding of `bmask`, ascending - its args at `kslot`, each
    //! of `nviews` alias views (`views`: a slot above a role's index, sixteen bits a view) bound to its role's plane again:
    //! `groups` threadgroups of `tg`
    kn_pipeline(rl.enc, c.psos[ord])
    if (c.tgmem[ord] != 0ul) {
        kn_tgmem(rl.enc, c.tgmem[ord], 0)
    }
    var slot = 0
    for (i in range(typeinfo dim(roles))) {
        while ((bmask & (1u << uint(slot))) == 0u) {
            slot++
        }
        let b = rl.bufs[int(roles[i])]
        let buf = rail_mtl_buf(b)
        kn_buffer(rl.enc, buf, uint64(b.off), slot)
        let bit = 1u << uint(slot)
        if ((rmask & bit) != 0u) {
            hz_read(buf, uint64(b.off), uint64(b.bytes))
        }
        if ((wmask & bit) != 0u) {
            hz_write(buf, uint64(b.off), uint64(b.bytes))
        }
        slot++
    }
    for (v in range(nviews)) {
        let e = uint(views >> uint64(16 * v)) & 0xFFFFu
        let b = rl.bufs[int(roles[int(e & 0xFFu)])]
        kn_buffer(rl.enc, rail_mtl_buf(b), uint64(b.off), int(e >> 8u))
    }
    kn_kargs(rl.enc, pc, kslot)
    hz_gate(rl.enc)
    kn_dispatch(rl.enc, uint3(groups, 1u, 1u), uint3(uint(tg), 1u, 1u))
}

//! the subgroup primitives the shared reductions ride, Metal's spelling
def gk_subgroup_add(v : float) : float => simd_sum(v)
def gk_subgroup_max(v : float) : float => simd_max(v)
def gk_subgroup_xor(v : float; m : uint) : float => simd_shuffle_xor(v, m)
def gk_subgroup_xor_u(v : uint; m : uint) : uint => simd_shuffle_xor(v, m)
def gk_subgroup_bcast0_u(v : uint) : uint => simd_shuffle(v, 0u)
def gk_subgroup_min_u(v : uint) : uint {
    var m = v
    var b = 16u
    while (b >= 1u) {
        m = min(m, simd_shuffle_xor(m, b))
        b /= 2u
    }
    return m
}

//! the home a rail is of
[unused_argument(rl)]
def rail_tts_dev(rl : MtRail) : TowerDev => TowerDev.metal

//! the home's name in the TTS driver's log lines
[unused_argument(rl)]
def rail_tts_tag(rl : MtRail) : string => "metal"

//! the ledger of a profiled shot: the command buffer open under the rail's encoder, the sums, the
//! first device error
struct private MtlProf {
    on : bool
    cb : MetalCommandBuffer?
    sums : RailProfSums
    err : string
}

var private g_mtl_prof : MtlProf
var private g_mtl_prof_said = false

//! a fresh command buffer of the home's queue and its encoder under `rl`
[cold_path]
def private mtl_prof_open(var rl : MtRail) {
    g_mtl_prof.cb = metal_new_command_buffer(g_queue)
    rl.enc = metal_new_compute_encoder(g_mtl_prof.cb)
}

//! the open command buffer ended, committed and waited: its GPU time in microseconds, its error
//! kept as the shot's first
[cold_path]
def private mtl_prof_close(var rl : MtRail) : double {
    metal_end_encoding(rl.enc)
    metal_commit(g_mtl_prof.cb)
    metal_wait_until_completed(g_mtl_prof.cb)
    let err = metal_command_buffer_error(g_mtl_prof.cb)
    if (!empty(err) && empty(g_mtl_prof.err)) {
        g_mtl_prof.err = err
    }
    let gs = metal_command_buffer_gpu_start_time(g_mtl_prof.cb)
    let us = (metal_command_buffer_gpu_end_time(g_mtl_prof.cb) - gs) * 1.0e6lf
    metal_release(rl.enc)
    metal_release(g_mtl_prof.cb)
    rl.enc = null
    g_mtl_prof.cb = null
    return us
}

//! a mark under the ledger: the dispatches since the last mark are their own command buffer, timed
//! on the device
[cold_path]
def private mtl_prof_cut(var rl : MtRail; role : RailProfRole) {
    rail_prof_add(g_mtl_prof.sums, role, mtl_prof_close(rl))
    mtl_prof_open(rl)
}

def rail_pt(var rl : MtRail; role : RailProfRole) {
    //! the profile ledger's mark after a dispatch: the open command buffer's GPU time charged to
    //! `role` (`mtl_prof_cut`)
    if (g_mtl_prof.on) {
        mtl_prof_cut(rl, role)
    }
}

[unused_argument(rl)]
def rail_tts_prof(rl : MtRail) : bool => rail_prof_on()

//! `rec`'s dispatches a command buffer a mark, the report after the last; the dispatches past the
//! last mark run uncharged, as no stamp names them
[cold_path]
def private mtl_prof_shot(var rl : MtRail; chain : string; nl, npos : int64; var err : string&;
                          rec : block<() : void>) : bool {
    rail_prof_say(g_mtl_prof_said, "metal", "runs a command buffer a mark, each committed and waited before the next")
    let t0 = ref_time_ticks()
    g_mtl_prof.sums = RailProfSums()
    g_mtl_prof.err = ""
    g_mtl_prof.on = true
    mtl_prof_open(rl)
    invoke(rec)
    g_mtl_prof.on = false
    mtl_prof_close(rl)
    err = g_mtl_prof.err
    rail_prof_report(g_mtl_prof.sums, "metal", chain, nl, npos, get_time_usec(t0))
    return empty(err)
}

//! `bytes` on the device, zeroed; 0 with `oom` set where the allocation failed
[unused_argument(rl)]
def rail_tts_alloc(rl : MtRail; var oom : bool&; bytes : int64) : uint64 {
    var buf = metal_new_buffer(g_dev, uint64(max(bytes, 16l)))
    if (buf == null) {
        oom = true
        return 0ul
    }
    residency_note(buf)
    return intptr(buf)
}

[unused_argument(rl)]
def rail_tts_free(rl : MtRail; var dev : uint64&) {
    return if (dev == 0ul)
    var buf = unsafe(reinterpret<MetalBuffer?>(dev))
    metal_release(buf)
    dev = 0ul
}

//! `bytes` of host memory at `src` onto the buffer from byte `off`: the buffer is the host's own memory
[unused_argument(rl)]
def rail_tts_put(rl : MtRail; dev : uint64; off : int64; src : void?; bytes : int64) {
    unsafe {
        memcpy(reinterpret<uint8?>(metal_buffer_contents(reinterpret<MetalBuffer?>(dev))) + off, src, bytes)
    }
}

//! `bytes` of plane `b` from byte `off` into `dst`: a shot has waited on its command buffer, so the rows are there
[unused_argument(rl)]
def rail_tts_take(rl : MtRail; b : RailBuf; off : int64; var dst : float?; bytes : int64) {
    unsafe {
        memcpy(dst, reinterpret<uint8?>(metal_buffer_contents(rail_mtl_buf(b))) + b.off + off, bytes)
    }
}

//! `bytes` the host writes in place and a dispatch reads; `dev` 0 where the buffer could not be made
[unused_argument(rl)]
def rail_tts_mapped_make(rl : MtRail; bytes : int64) : RailMapped {
    var buf = metal_new_buffer(g_dev, uint64(max(bytes, 16l)))
    return RailMapped() if (buf == null)
    residency_note(buf)
    return RailMapped(host = metal_buffer_contents(buf), dev = intptr(buf), bytes = bytes)
}

[unused_argument(rl)]
def rail_tts_mapped_free(rl : MtRail; var m : RailMapped) {
    if (m.dev != 0ul) {
        var buf = unsafe(reinterpret<MetalBuffer?>(m.dev))
        metal_release(buf)
    }
    m = RailMapped()
}

//! the recurrence stamp's 1024-thread threadgroup holds its lanes on every device this home arms
[unused_argument(rl)]
def rail_tts_lstm_ok(rl : MtRail) : bool => true

//! the conv sites' schedule-fed tiles: none on this home
[unused_argument(rl)]
def rail_tts_cm2_on(rl : MtRail) : bool => false

//! the f16 GEMM: none on this home
[unused_argument(rl)]
def rail_tts_f16_on(rl : MtRail) : bool => false

[unused_argument(rl)]
def rail_tts_f16_ensure(rl : MtRail) : bool => false

//! never reached: this home answers false at `rail_tts_cm2_on` and `rail_tts_f16_on`
[unused_argument(rl, site)]
def rail_site_rows(rl : MtRail; site : RailSite; rows : int64) : int64 => rows

[unused_argument(rl, site, width, rows)]
def rail_site_groups(rl : MtRail; site : RailSite; width, rows : int64) : int64 {
    panic("dasLLAMA metal rail: no schedule-fed tile on this home")
}

[unused_argument(rl, site, rid, y, n, dout, rows)]
def rail_mm_kq(var rl : MtRail; site : RailSite; rid : int64; y : TowerBuf; n, dout, rows : int64) {
    panic("dasLLAMA metal rail: no schedule-fed tile on this home")
}

[unused_argument(rl, w, x, y, n, dout, rows, woff, ystride)]
def rail_mm16(var rl : MtRail; w, x, y : TowerBuf; n, dout, rows, woff : int64; ystride : int64 = 0l) {
    panic("dasLLAMA metal rail: no f16 GEMM on this home")
}

def rail_tts_shot(var rl : MtRail; chain : string; nl, npos : int64; rec : block<() : void>) {
    //! `rec`'s dispatches through `rl` on one command buffer of the home's queue, on the f32 lane,
    //! committed and waited on; under the profile ledger a command buffer a mark (`mtl_prof_shot`)
    rail_begin(rl, RailLane.f32)
    residency_flush()
    var err : string
    var ran = false
    if (rail_tts_prof(rl)) {
        ran = mtl_prof_shot(rl, chain, nl, npos, err, rec)
    } else {
        ran = with_compute_encoder(g_queue, err) $(enc : MetalComputeEncoder?) {   // nolint:PERF026,LINT019 — the error-text fetch runs only on the failure leg; the rule fires from a seat's hot path
            rl.enc = unsafe(reinterpret<MetalComputeEncoder?>(enc))
            invoke(rec)
        }
    }
    rl.enc = null
    if (!empty(g_mtl_retired)) {
        mtl_rail_retire()
    }
    if (length(g_mtl_u32) > MTL_U32_CAP) {
        mtl_rail_u32_drop()
    }
    if (!ran) {
        panic("dasLLAMA metal tts: {chain}: a seat's chain failed on the device - {err}")
    }
}
