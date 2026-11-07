options gen2
options indenting = 4
options _dasllama_internal = true

//! The rail primitives both discrete homes meet with one body, generic over the rail (`VtRail`, `CutRail`): each dispatches
//! through the home's stamp overloads and the per-home primitives (`dasllama_vulkan_rail`, `dasllama_cuda_rail`), which this
//! module re-exports, so a chain body requires this module alone.
module dasllama_tower_rail shared public

require dasllama/dasllama_lint public
require dasllama/dasllama_kqformat
require dasllama/dasllama_tower
require dasllama/dasllama_gpu_math
require dasllama/dasllama_gpu_rail public
require dasllama/dasllama_gpu_kernels_common
require dasllama/dasllama_gpu_kernels_discrete
require ?vulkan dasllama/dasllama_vulkan_rail public
require ?cuda dasllama/dasllama_cuda_rail public

let TOWER_NO_BIAS = -1l   //! a bias offset that names no row: the unbiased stamp

//! the weight plane a vision site's f32 tile reads: the halfword plane through the bf16 arm, else the norms plane
def rail_vis_w(bf16 : bool) : TowerBuf => bf16 ? TowerBuf.wb : TowerBuf.norms

//! x [rows x n] into y [rows x dout] at `yoff` on the f32 tile over plane `w` at `woff` (bf16 rows under `bf16`), biased at `boff >= 0`, rows `ystride` apart (0: `dout`)
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#rail-primitives")]
def rail_mm32(var rl : auto(R); w : TowerBuf; bf16 : bool; x, y : TowerBuf; n, dout, rows, woff, boff, yoff : int64; ystride : int64 = 0l) {
    var pc = f32_gemm_args(n, dout, rows, w == TowerBuf.norms ? woff - rl.norms_base : woff, boff >= 0l ? boff - rl.norms_base : 0l, yoff)
    if (ystride > 0l) {
        pc.ystride = uint(ystride)
    }
    let wgs = f32_gemm_wgs(rows, dout)
    if (boff >= 0l) {
        if (bf16) {
            enc_f32_gemm_bf16_b_cls(rl, fixed_array(w, x, y, TowerBuf.norms), pc, wgs)
        } else {
            enc_f32_gemm_b_cls(rl, fixed_array(w, x, y, TowerBuf.norms), pc, wgs)
        }
    } elif (bf16) {
        enc_f32_gemm_bf16_cls(rl, fixed_array(w, x, y), pc, wgs)
    } else {
        enc_f32_gemm_cls(rl, fixed_array(w, x, y), pc, wgs)
    }
}

//! a split pick's `sk` planes at `site` summed into y [rows x dout]; nothing where the pick runs whole or a lane GEMM (the f32 or f16 lane) wrote y whole
def rail_split_sum(var rl : auto(R); site : RailSite; y : TowerBuf; rows, dout : int64) {
    let nsplit = rail_site_nsplit(rl, site)
    return if (nsplit <= 1l || rl.lane != RailLane.q8)
    let nelem = rail_site_rows(rl, site, rows) * dout
    var pr = SkRedArgs(nelem = uint(nelem), k = uint(nsplit), ybase = 0u)
    enc_splitk_reduce_cls(rl, fixed_array(TowerBuf.sk, y), pr, splitk_reduce_wgs(nelem))
    rail_pt(rl, RailProfRole.sk_reduce)
}

[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#rail-primitives")]
def rail_site_feed(var rl : auto(R); site : RailSite; src : TowerBuf; nelem : int64; lo, hi : float) {
    let b = rail_bind(rl, site)
    if (rail_mulmm(rl)) {
        var pc_rq = RqArgs(inbase = 0u, nblk = uint(nelem / kq_elems(KqFmt.q8)), lo = lo, hi = hi)
        enc_tower_clamp_rq_cls(rl, fixed_array(src, b.xq, b.xs), pc_rq, rq_wgs(false, nelem))
    } else {
        var pc = GkClampArgs(nelem = uint(nelem), lo = lo, hi = hi, scale = 1.0)
        enc_tower_clamp_cvt_cls(rl, fixed_array(src, b.xh), pc, rq_wgs(false, nelem))
    }
}

//! a block GEMM input's feed off its f32 rows clamped to [lo, hi]: `rail_site_feed` at the block sites' planes
def rail_feed(var rl : auto(R); src : TowerBuf; nelem : int64; lo, hi : float) {
    rail_site_feed(rl, RailSite.d, src, nelem, lo, hi)
}

//! a q8 GEMM input's Q8_0 feed and its mark, on the Q8_0-feed arm alone: the f16 feed lands with the stamp that wrote the rows, the exact lane reads the rows
def rail_rq_feed(var rl : auto(R); src : TowerBuf; nelem : int64) {
    if (rail_mulmm(rl) && rl.lane != RailLane.f32) {
        rail_feed(rl, src, nelem, -NO_CLAMP, NO_CLAMP)
        rail_pt(rl, RailProfRole.rq)
    }
}

[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#tower-rail")]
def rail_lane_mm(var rl : auto(R); site : RailSite; rid : int64; x, y : TowerBuf; n, dout, rows, woff : int64) {
    if (rl.lane == RailLane.f32) {
        rail_mm32(rl, TowerBuf.norms, false, x, y, n, dout, rows, woff, TOWER_NO_BIAS, 0l)
    } elif (rl.lane == RailLane.f16) {
        rail_mm16(rl, TowerBuf.wb, rail_bind(rl, site).xh, y, n, dout, rows, woff)
    } else {
        rail_mm_kq(rl, site, rid, y, n, dout, rows)
    }
}

//! the bias row at the model offset `boff` added over [rows x width] of `x` in place, and its mark; nothing where `boff` names no row
def rail_bias_pass(var rl : auto(R); x : TowerBuf; width, rows, boff : int64) {
    return if (boff < 0l)
    var pc = GkBiasActArgs(d = uint(width), nelem = uint(rows * width), boff = uint(boff - rl.norms_base), act = BIAS_ACT_NONE)
    enc_tower_bias_act_cls(rl, fixed_array(x, TowerBuf.norms), pc, rq_wgs(false, rows * width))
    rail_pt(rl, RailProfRole.bias)
}

//! the dispatches `blk` records with the seat at `role` narrowed to `bytes` from `off` past its start, the seat put back after
def rail_view(var rl : auto(R); role : TowerBuf; off, bytes : int64; blk : block<() : void>) {
    let keep = rl.bufs[int(role)]
    rl.bufs[int(role)] = RailBuf(dev = keep.dev, bytes = bytes, bit = keep.bit, off = keep.off + off)
    invoke(blk)
    rl.bufs[int(role)] = keep
}

//! the q8 GEMV at `site` over record `rid`: `ncols` rows of `n` off the site's Q8_0 feed into `nreg` regions of `dout`, y's rows `nreg * dout` apart
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#rail-primitives")]
def rail_gemv_q8(var rl : auto(R); site : RailSite; rid : int64; y : TowerBuf; n, dout, nreg, ncols : int64) {
    let b = rail_bind(rl, site)
    var pc = GemvArgs(n = uint(n), d = uint(dout), nreg = uint(nreg), ncols = uint(ncols), ystride = uint(nreg * dout),
        lanes_per_row = gemv_lanes_per_row(int(KqFmt.q8), n))
    let wgs = rail_gemv_wgs(rl, nreg * dout, pc.lanes_per_row)
    let roles = fixed_array(b.wq, b.ws, b.meta, b.xq, b.xs, y)
    rail_view(rl, b.meta, b.meta_off + int64(rl.maps[rid]) * 4l, RAIL_GEMV_META_BYTES) {
        if (ncols <= 1l) {
            enc_q8_gemv_cls(rl, roles, pc, wgs)
        } else {
            each_col_tile(pc, pc.ncols, GEMV_COL_TILE) $ {
                rail_q8_gemv_n(rl, roles, pc, wgs)
            }
        }
    }
}
