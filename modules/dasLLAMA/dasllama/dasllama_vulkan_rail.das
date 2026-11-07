options gen2
options indenting = 4
options _dasllama_internal = true

//! The Vulkan home's rail primitives: the dispatches a shared tower chain body cannot spell through a generated
//! `enc_<stamp>_cls(rail, roles, pc, wgs)` overload or a shared primitive (`dasllama_tower_rail`) - the q8 / kq and f16 GEMM
//! lanes, the GEMV's grid and N-column leaf, the flash tile, the plane copy and zero, the profile mark, the site picks.
//! `dasllama_cuda_rail` carries the same names over `CutRail`.
module dasllama_vulkan_rail shared public

require dasllama/dasllama_lint public
require math
require dasllama/dasllama_env
require dasllama/dasllama_vulkan_common public
require dasllama/dasllama_vulkan_classes public

//! the profile ledger's mark after a dispatch
def rail_pt(rl : VtRail; role : RailProfRole) {
    vt_pt(rl.raw, role)
}

[unused_argument(rl), arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#rail-primitives")]
def rail_fork(var rl : VtRail) {}

[unused_argument(rl, b)]
def rail_branch(var rl : VtRail; b : int) {}

[unused_argument(rl)]
def rail_join(var rl : VtRail) {}

//! the Q8_0-feed arm runs on this rail: the mul_mm tiles off the cm2 feed
def rail_mulmm(rl : VtRail) : bool => !rl.cm2

//! the rows a schedule record of `site` carries over `rows` encode rows: the 256 column's rows rounded up to it
def rail_site_rows(rl : VtRail; site : RailSite; rows : int64) : int64 => vt_tile_rows(rl.sites[int(site)], rows)

//! the workgroups the tile at `site` takes over a [rows x width] output, every split plane's
def rail_site_groups(rl : VtRail; site : RailSite; width, rows : int64) : int64
    => vt_tile_groups(rl.sites[int(site)], width, rows) * rail_site_nsplit(rl, site)

[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#rail-sites")]
def rail_mm_kq(var rl : VtRail; site : RailSite; rid : int64; y : TowerBuf; n, dout, rows : int64) {
    let b = rail_bind(rl, site)
    var t = rl.sites[int(site)]
    t.fmt = int(b.fmt)
    let split = rail_site_nsplit(rl, site) > 1l
    var pc = BatchArgs(n = uint(n), d = uint(dout), map_off = rail_kq_map(rl, site, rid), ksplit = uint(split ? t.ksplit : 0l))
    let wq = rail_buf(rl, b.wq)
    let ws = rail_buf(rl, b.ws)
    let meta = rail_buf(rl, b.meta)
    let xh = rail_buf(rl, b.xh)
    let xq = rail_buf(rl, b.xq)
    let yb = rail_buf(rl, split ? TowerBuf.sk : y)
    var s = vt_tile_cls_set(t, wq.dev, ws.dev, wq.bytes, ws.bytes, meta.dev, meta.bytes, xh.dev, xh.bytes, xq.dev, rail_buf(rl, b.xs).dev, xq.bytes,
        t.tc > 0l ? xh.bit : xq.bit, yb.dev, yb.bytes, yb.bit)
    vt_tile_enc(rl.raw, rl.h, t, s, pc, rail_site_groups(rl, site, dout, rows))
}

//! the cm2 f16 tile's column for a [rows x d] output (`f16_cm2_col`'s wave form, `DASLLAMA_CM2_TILE` forcing s or m); the s column only where its stamp builds
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#rail-primitives")]
def vt_f16_tc(d, rows : int64) : int64 {
    let forced = g_env_vulkan.cm2_tile
    rail_cm2_tile_say(forced) if (forced > 0l && !rail_cm2_tile_said())
    let tc = forced == 32l || forced == 128l ? forced : f16_cm2_col(d, rows, int64(g_gpu.sm_count))
    return tc == 32l && ensure_f16_gemm_cm2_s_cls() ? 32l : 128l
}

[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#rail-primitives")]
def rail_mm16(var rl : VtRail; w, x, y : TowerBuf; n, dout, rows, woff : int64; ystride : int64 = 0l) {
    var pc = F16GemmArgs(n = uint(n), d = uint(dout), rows = uint(rows), woff = uint(woff), ybase = 0u, ystride = uint(ystride > 0l ? ystride : dout), ksplit = 0u)
    if (rl.gemm_khr) {
        enc_f16_gemm_khr_cls(rl, fixed_array(w, x, y), pc, f16_gemm_wgs(true, rows, dout))
    } elif (vt_f16_tc(dout, rows) == 32l) {
        enc_f16_gemm_cm2_s_cls(rl, fixed_array(w, x, y), pc, f16_gemm_cm2_wgs(rows, dout, 32l))
    } else {
        enc_f16_gemm_cm2_cls(rl, fixed_array(w, x, y), pc, f16_gemm_wgs(false, rows, dout))
    }
}

//! the q8 GEMV's dispatch grid over `rows` output rows at `lanes` lanes a row
[unused_argument(rl)]
def rail_gemv_wgs(rl : VtRail; rows : int64; lanes : uint) : int64 => gemv_wgs(rows, lanes)

//! the q8 GEMV's N-column leaf over `pc.ncols` activation rows: the stamp by the count, its row-pair twin where the lever arms it
def rail_q8_gemv_n(var rl : VtRail; roles : TowerBuf const[6]; var pc : GemvArgs; wgs : int64) {
    let pair = q8_gemv_pair_wanted(pc)
    let g2 = (wgs + 1l) / 2l
    if (pc.ncols <= 2u) {
        if (pair && ensure_q8_gemv_n2p_cls()) {
            enc_q8_gemv_n2p_cls(rl, roles, pc, g2)
        } else {
            enc_q8_gemv_n2_cls(rl, roles, pc, wgs)
        }
    } elif (pc.ncols <= 4u) {
        if (pair && ensure_q8_gemv_n4p_cls()) {
            enc_q8_gemv_n4p_cls(rl, roles, pc, g2)
        } else {
            enc_q8_gemv_n4_cls(rl, roles, pc, wgs)
        }
    } elif (pair && ensure_q8_gemv_n8p_cls()) {
        enc_q8_gemv_n8p_cls(rl, roles, pc, g2)
    } else {
        enc_q8_gemv_n8_cls(rl, roles, pc, wgs)
    }
}

//! the bidirectional flash tile of head size `hsp` over the q / k / v panels of `nheads` heads into `o`
def rail_fa(var rl : VtRail; hsp : int64; q, k, v, o : TowerBuf; var pc : FaArgs; nheads : int64) {
    let bq = rail_buf(rl, q)
    let bk = rail_buf(rl, k)
    let bv = rail_buf(rl, v)
    let bo = rail_buf(rl, o)
    var s = fa_stamp_set(rl.khr, fixed_array(bq.dev, bk.dev, bv.dev, bo.dev), fixed_array(bq.bytes, bk.bytes, bv.bytes, bo.bytes), fixed_array(bq.bit, bk.bit, bv.bit, bo.bit))
    fa_stamp_enc(rl.khr, hsp, false, false, false, false, rl.raw, rl.h, s, pc, fa_stamp_wgs(rl.khr, nheads, int64(pc.rows)), true)
}

//! `bytes` of plane `src` from byte `src_off` into plane `dst` at byte `dst_off`, behind every earlier access to either plane
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#rail-primitives")]
def rail_copy(var rl : VtRail; src : TowerBuf; src_off : int64; dst : TowerBuf; dst_off, bytes : int64) {
    let bs = rail_buf(rl, src)
    let bd = rail_buf(rl, dst)
    vhz_dep(rl.raw, rl.h, bs.bit, bd.bit, true)
    cmd_copy_range(rl.raw, bs.dev, bs.off + src_off, bd.dev, bd.off + dst_off, bytes)
}

//! `bytes` of plane `dst` from byte `off` zeroed, behind every earlier access to it
def rail_zero(var rl : VtRail; dst : TowerBuf; off, bytes : int64) {
    let bd = rail_buf(rl, dst)
    vhz_dep(rl.raw, rl.h, 0u, bd.bit, true)
    cmd_fill_zero(rl.raw, bd.dev, bd.off + off, bytes)
}
