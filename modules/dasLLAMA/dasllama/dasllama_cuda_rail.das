options gen2
options indenting = 4
options _dasllama_internal = true

//! The CUDA home's rail primitives: the launches a shared tower chain body cannot spell through a generated
//! `enc_<stamp>_cls(rail, roles, pc, wgs)` overload or a shared primitive (`dasllama_tower_rail`) - the q8 / kq and f16 GEMM
//! lanes, the GEMV's grid and N-column leaf, the flash tile, the plane copy and zero, the profile mark, the site picks.
//! `dasllama_vulkan_rail` carries the same names over `VtRail`.
module dasllama_cuda_rail shared public

require dasllama/dasllama_lint public
require math
require dasllama/dasllama_kqformat
require dasllama/dasllama_gpu_math
require dasllama/dasllama_env
require dasllama/dasllama_tower   // TowerDev
require dasllama/dasllama_cuda_common public
require dasllama/dasllama_cuda_classes public

let private CUT_PROF_CAP = 2048   //! a shot's stamps; a mark past it is dropped, as the Vulkan pool's

//! the ledger of a profiled shot: a timed event a stamp, recorded on the rail's stream, and the
//! role each names
struct private CutProf {
    on : bool
    n : int
    evs : array<void?>
    roles : array<RailProfRole>
}

var private g_cut_prof : CutProf
var private g_cut_prof_said = false

[cold_path]
def private cut_prof_refused(what : string) {
    panic("dasLLAMA cuda rail: {what} - {cuda_last_error()}")
}

[cold_path]
def private cut_prof_grow {
    let ev = cuda_event_create()
    if (ev == null) {
        cut_prof_refused("a profile event could not be made")
    }
    g_cut_prof.evs |> push(ev)
    g_cut_prof.roles |> push(RailProfRole.ln)
}

[cold_path]
def private cut_prof_stamp(stream : void?; role : RailProfRole) {
    if (g_cut_prof.n == length(g_cut_prof.evs)) {
        cut_prof_grow()
    }
    g_cut_prof.roles[g_cut_prof.n] = role
    if (!cuda_event_record(g_cut_prof.evs[g_cut_prof.n], stream)) {
        cut_prof_refused("a profile stamp did not record")
    }
    g_cut_prof.n++
}

def rail_pt(rl : CutRail; role : RailProfRole) {
    //! the profile ledger's mark after a launch: a timed event on the rail's stream, the span since
    //! the last charged to `role`
    if (g_cut_prof.on && g_cut_prof.n < CUT_PROF_CAP) {
        cut_prof_stamp(rl.stream, role)
    }
}

[unused_argument(rl)]
def rail_tts_prof(rl : CutRail) : bool => rail_prof_on()

//! the ledger's events released ahead of the device
[cold_path]
def private cut_prof_drop {
    for (ev in g_cut_prof.evs) {
        cuda_event_destroy(ev)
    }
    g_cut_prof.evs |> clear()
    g_cut_prof.roles |> clear()
    g_cut_prof.n = 0
    g_cut_prof.on = false
}

[init]
def dasllama_cuda_rail_register {
    cud_on_shutdown(@@cut_prof_drop)
}

//! the stamps' spans read once the last stamp landed: each stamp's event elapsed from the one
//! before, charged to its role
[cold_path]
def private cut_prof_report(chain : string; nl, npos : int64; t0 : int64) {
    g_cut_prof.on = false
    if (!cuda_event_synchronize(g_cut_prof.evs[g_cut_prof.n - 1])) {
        cut_prof_refused("{chain}: a seat's chain failed on the device")
    }
    var s : RailProfSums
    for (i in range(1, g_cut_prof.n)) {
        let ms = cuda_event_elapsed_ms(g_cut_prof.evs[i - 1], g_cut_prof.evs[i])
        rail_prof_add(s, g_cut_prof.roles[i], double(ms) * 1000.0lf)
    }
    rail_prof_report(s, "cuda", chain, nl, npos, get_time_usec(t0))
}

//! the cut between two blocks of a tower chain: this home's stream takes the whole chain
[unused_argument(rl)]
def rail_block_cut(var rl : CutRail) {}

//! the Q8_0-feed arm runs on this rail: never, every q8 GEMM rides the cm2 feed
[unused_argument(rl)]
def rail_mulmm(rl : CutRail) : bool => false

//! the rows a schedule record of `site` carries over `rows` encode rows: the encode's own on every column
[unused_argument(rl, site)]
def rail_site_rows(rl : CutRail; site : RailSite; rows : int64) : int64 => rows

//! the workgroups the tile at `site` takes over a [rows x width] output, every split plane's
def rail_site_groups(rl : CutRail; site : RailSite; width, rows : int64) : int64
    => cm2_tiles_at(width, rows, rl.sites[int(site)].tc) * rail_site_nsplit(rl, site)

[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#rail-sites")]
def rail_mm_kq(var rl : CutRail; site : RailSite; rid : int64; y : TowerBuf; n, dout, rows : int64) {
    let b = rail_bind(rl, site)
    let p = rl.sites[int(site)]
    let split = rail_site_nsplit(rl, site) > 1l
    var pc = BatchArgs(n = uint(n), d = uint(dout), map_off = rail_kq_map(rl, site, rid), ksplit = uint(split ? p.ksplit : 0l))
    cud_cm2_enc(int(b.fmt), p.tci, rl.stream, fixed_array(rail_dev(rl, b.wq), rail_dev(rl, b.ws), rail_dev(rl, b.meta), rail_dev(rl, b.xh),
        rail_dev(rl, split ? TowerBuf.sk : y)), pc, rail_site_groups(rl, site, dout, rows))
}

//! the f16 tile's column for a [rows x d] output (`f16_cm2_col`'s wave form, `DASLLAMA_CM2_TILE` forcing s or m); the s column only where its stamp loads
[arch(at = "../ARCHITECTURE_GPU_ASYMMETRIES_CUDA.md#cuda-cm2-column")]
def cud_f16_tc(d, rows : int64) : int64 {
    let forced = g_env_vulkan.cm2_tile
    rail_cm2_tile_say(forced) if (forced > 0l && !rail_cm2_tile_said())
    let tc = forced == 32l || forced == 128l ? forced : f16_cm2_col(d, rows, int64(g_cud.sm_count))
    return tc == 32l && ensure_f16_gemm_cm2_ws_cls() ? 32l : 128l
}

[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#rail-primitives")]
def rail_mm16(var rl : CutRail; w, x, y : TowerBuf; n, dout, rows, woff : int64; ystride : int64 = 0l) {
    var pc = F16GemmArgs(n = uint(n), d = uint(dout), rows = uint(rows), woff = uint(woff), ybase = 0u, ystride = uint(ystride > 0l ? ystride : dout), ksplit = 0u)
    if (cud_f16_tc(dout, rows) == 32l) {
        enc_f16_gemm_cm2_ws_cls(rl, fixed_array(w, x, y), pc, f16_gemm_cm2_wgs(rows, dout, 32l))
    } else {
        enc_f16_gemm_cm2_w_cls(rl, fixed_array(w, x, y), pc, f16_gemm_cm2_wgs(rows, dout))
    }
}

//! the q8 GEMV's launch grid over `rows` output rows at `lanes` lanes a row
[unused_argument(rl)]
def rail_gemv_wgs(rl : CutRail; rows : int64; lanes : uint) : int64 => cud_gemv_wgs(rows, lanes)

//! the q8 GEMV's N-column leaf over `pc.ncols` activation rows: the stamp by the count
def rail_q8_gemv_n(var rl : CutRail; roles : TowerBuf const[6]; var pc : GemvArgs; wgs : int64) {
    if (pc.ncols <= 2u) {
        enc_q8_gemv_n2_cls(rl, roles, pc, wgs)
    } elif (pc.ncols <= 4u) {
        enc_q8_gemv_n4_cls(rl, roles, pc, wgs)
    } else {
        enc_q8_gemv_n8_cls(rl, roles, pc, cud_q8_gemv_n8_wgs(pc))
    }
}

//! the bidirectional flash tile of head size `hsp` over the q / k / v panels of `nheads` heads into `o`
def rail_fa(var rl : CutRail; hsp : int64; q, k, v, o : TowerBuf; var pc : FaArgs; nheads : int64) {
    cud_fa_enc(rl.stream, hsp, false, false, false, fixed_array(rail_dev(rl, q), rail_dev(rl, k), rail_dev(rl, v), rail_dev(rl, o)), 0ul, pc,
        fa_stamp_wgs(false, nheads, int64(pc.rows)), true)
}

//! `bytes` of plane `src` from byte `src_off` into plane `dst` at byte `dst_off`, ordered on the rail's stream
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#rail-primitives")]
def rail_copy(rl : CutRail; src : TowerBuf; src_off : int64; dst : TowerBuf; dst_off, bytes : int64) {
    let to = rail_dev(rl, dst) + uint64(dst_off)
    let from = rail_dev(rl, src) + uint64(src_off)
    return if (cud_walk_transfer(to, from, bytes))
    cuda_memcpy_dtod(to, from, uint64(bytes), rl.stream)
}

//! `bytes` of plane `dst` from byte `off` zeroed, ordered on the rail's stream
def rail_zero(rl : CutRail; dst : TowerBuf; off, bytes : int64) {
    let to = rail_dev(rl, dst) + uint64(off)
    return if (cud_walk_transfer(to, 0ul, bytes))
    cuda_memset_d8(to, 0u8, uint64(bytes), rl.stream)
}

var private g_cud_rail_fork_on = true   //! a body's forks branch onto the side streams (`set_cuda_tower_fork`)

//! Test A/B seat: a chain body's forks onto the side streams (`on`), or every launch on the one stream; answers the setting it replaced
def set_cuda_tower_fork(on : bool) : bool {
    let was = g_cud_rail_fork_on
    g_cud_rail_fork_on = on
    if (on != was) {
        to_log(LOG_INFO, "dasLLAMA cuda tower: set_cuda_tower_fork({on}) - the chains' independent launches {on ? "branch onto the side streams" : "stay on the one stream"}\n")
    }
    return was
}

def cuda_tower_fork_on : bool => g_cud_rail_fork_on

var private g_cud_tts_shot_pdl = false   //! a TTS seat's shot keeps the home's PDL mode (`set_cuda_tts_shot_pdl`)

def set_cuda_tts_shot_pdl(on : bool) : bool {
    //! Test A/B seat: a TTS seat's shot under the home's programmatic dependent launch (`on`) or in plain stream
    //! order; answers the setting it replaced
    let was = g_cud_tts_shot_pdl
    g_cud_tts_shot_pdl = on
    if (on != was) {
        let how = (on
            ? "under programmatic dependent launch where the home has it on (sm_90 and up, DASLLAMA_CUDA_PDL not 0), in plain stream order elsewhere"
            : "in plain stream order")
        to_log(LOG_INFO, "dasLLAMA cuda tts: set_cuda_tts_shot_pdl({on}) - a TTS seat's shot launches {how} (the default: plain stream order)\n")
    }
    return was
}

def cuda_tts_shot_pdl_on : bool => g_cud_tts_shot_pdl

[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#rail-primitives")]
def rail_fork(var rl : CutRail) {
    rl.main = rl.stream
    rl.branched = 0u
    return if (!g_cud_rail_fork_on || cud_walk_mark(CUD_WALK_FORK))
    cuda_event_record(g_cud.side.fork, rl.main)
}

//! the open fork's launches from here on branch `b`: 0 the stream the fork left, 1 and up a side stream (the one stream with forks off)
def rail_branch(var rl : CutRail; b : int) {
    return if (!g_cud_rail_fork_on)
    if (b == 0) {
        rl.stream = rl.main
        cud_walk_mark(CUD_WALK_BRANCH)
        return
    }
    let side = g_cud.side.streams[b - 1]
    rl.stream = side
    let bit = 1u << uint(b - 1)
    return if (cud_walk_mark(CUD_WALK_BRANCH + uint64(b)) || (rl.branched & bit) != 0u)
    rl.branched |= bit
    cuda_stream_wait_event(side, g_cud.side.fork)
}

//! the fork's join: the stream it left waits on every branch's launches, and launches on from here
def rail_join(var rl : CutRail) {
    rl.stream = rl.main
    return if (!g_cud_rail_fork_on || cud_walk_mark(CUD_WALK_JOIN))
    for (i in range(CUD_SIDE_STREAMS)) {
        continue if ((rl.branched & (1u << uint(i))) == 0u)
        cuda_event_record(g_cud.side.joins[i], g_cud.side.streams[i])
        cuda_stream_wait_event(rl.main, g_cud.side.joins[i])
    }
    rl.branched = 0u
}

//! the home a rail is of
[unused_argument(rl)]
def rail_tts_dev(rl : CutRail) : TowerDev => TowerDev.cuda

//! the home's name in the TTS driver's log lines
[unused_argument(rl)]
def rail_tts_tag(rl : CutRail) : string => "cuda"

//! the recurrence stamp's block holds its lanes on every card this home arms
[unused_argument(rl)]
def rail_tts_lstm_ok(rl : CutRail) : bool => true

//! the conv sites' schedule-fed cm2 tiles serve on this home
[unused_argument(rl)]
def rail_tts_cm2_on(rl : CutRail) : bool => true

//! the f16 GEMM serves on every card this home arms
[unused_argument(rl)]
def rail_tts_f16_on(rl : CutRail) : bool => true

[unused_argument(rl)]
def rail_tts_f16_ensure(rl : CutRail) : bool => ensure_f16_gemm_cm2_w_cls()

//! `bytes` on the device, 0 with `oom` set where the allocation failed
[unused_argument(rl)]
def rail_tts_alloc(rl : CutRail; var oom : bool&; bytes : int64) : uint64 => cud_dev_zero(oom, bytes)

[unused_argument(rl)]
def rail_tts_free(rl : CutRail; var dev : uint64&) {
    return if (dev == 0ul)
    cuda_mem_free(dev)
    dev = 0ul
}

//! `bytes` of host memory at `src` onto the buffer from byte `off`, on the home's stream
[unused_argument(rl)]
def rail_tts_put(rl : CutRail; dev : uint64; off : int64; src : void?; bytes : int64) {
    cuda_memcpy_htod(dev + uint64(off), src, uint64(bytes), g_cud.stream)
}

var private g_cud_tts_take_fault = false   //! a cell's seam: the take's copy reads as failed

def set_cuda_tts_take_fault_(on : bool) : bool {
    //! test seam: the next takes' copy reads as failed, so the take's error arm runs; answers the
    //! setting it replaced
    let was = g_cud_tts_take_fault
    g_cud_tts_take_fault = on
    return was
}

//! `bytes` of plane `b` from byte `off` into `dst`: the copy queued behind the stream's work, then
//! one sync, so `dst` holds the rows on return whatever memory it is
[unused_argument(rl)]
def rail_tts_take(rl : CutRail; b : RailBuf; off : int64; var dst : float?; bytes : int64) {
    let copied = (cuda_memcpy_dtoh(dst, b.dev + uint64(off), uint64(bytes), g_cud.stream)
        && !g_cud_tts_take_fault)
    if (!copied || !cud_sync()) {
        panic("dasLLAMA cuda tts: a seat's chain or its readback failed on the device - "
            + "{cuda_last_error()}")
    }
}

//! `bytes` the host writes in place and a launch reads; `dev` 0 where the block could not be made
[unused_argument(rl)]
def rail_tts_mapped_make(rl : CutRail; bytes : int64) : RailMapped {
    var oom = false
    let m = cud_mapped_make(oom, bytes)
    return RailMapped(host = m.host, dev = oom ? 0ul : m.dev, bytes = oom ? 0l : bytes)
}

[unused_argument(rl)]
def rail_tts_mapped_free(rl : CutRail; var m : RailMapped) {
    var cm = CudMapped(host = m.host, dev = m.dev)
    cud_mapped_free(cm)
    m = RailMapped()
}

//! `rec` on the home's stream, on the f32 lane; under the profile ledger a baseline stamp leads it
[arch(at = "../ARCHITECTURE_GPU_TOWER_TTS.md#tts-st2-seats")]
def rail_tts_shot(var rl : CutRail; chain : string; nl, npos : int64; rec : block<() : void>) {
    rail_begin(rl, RailLane.f32)
    rl.stream = g_cud.stream
    let pdl = cuda_launch_pdl()
    cuda_set_launch_pdl(pdl && g_cud_tts_shot_pdl)
    if (rail_tts_prof(rl)) {
        rail_prof_say(g_cut_prof_said, "cuda", "records a timed event after every mark on the home's stream")
        let t0 = ref_time_ticks()
        g_cut_prof.n = 0
        g_cut_prof.on = true
        cut_prof_stamp(rl.stream, RailProfRole.ln)
        invoke(rec)
        cut_prof_report(chain, nl, npos, t0)
    } else {
        invoke(rec)
    }
    cuda_set_launch_pdl(pdl)
}
