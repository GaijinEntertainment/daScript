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
require dasllama/dasllama_cuda_common public
require dasllama/dasllama_cuda_classes public

//! the profile ledger's mark after a launch: none on this home
[unused_argument(rl, role)]
def rail_pt(rl : CutRail; role : RailProfRole) {}

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
        enc_q8_gemv_n8_cls(rl, roles, pc, wgs)
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
