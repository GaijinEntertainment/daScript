options gen2
options indenting = 4
options _dasllama_internal = true

//! The vision towers' dispatch sequences and the pre-LN block loop, written once for both discrete homes: every body is
//! generic over the rail it dispatches through (`VtRail`, `CutRail`), each stamp named by its planes' roles, each GEMM lane,
//! feed and flash tile a rail primitive (`dasllama_tower_rail`, which carries the homes' own). A seat keeps the gate, the
//! uploads, the scratch, the schedule staging and the copy-out; it builds its rail and calls the family's body here.
module dasllama_tower_chains shared public

require dasllama/dasllama_lint public
require math
require dasllama/dasllama_kqformat
require dasllama/dasllama_math   // grow_resize
require dasllama/dasllama_tower
require dasllama/dasllama_gpu_math
require dasllama/dasllama_gpu_kernels_common
require dasllama/dasllama_gpu_kernels_discrete
require dasllama/dasllama_gemma3v
require dasllama/dasllama_gemma4v
require dasllama/dasllama_gemma4uv
require dasllama/dasllama_qwen3v
require dasllama/dasllama_qwen25v
require dasllama/dasllama_tower_rail

//! gemma3v's pre-LN block offsets into `offs`, one record a block
def tower_gemma3v_offs(t : Gemma3vTower; @scratch @exact_size var offs : array<LayerOffs>) {
    offs |> grow_resize(t.n_layer)
    for (l in range64(t.n_layer)) {
        offs[l] = gemma3v_ln_offs(t, l)
    }
}

//! pre-LN block record `rid`'s schedule row: its GEMM's first weight block, the rows and the workgroups its site takes
def tower_ln_rec(rl : auto(R); offs : array<LayerOffs>; d, ff, npos, rid : int64) : TowerSchedRec {
    var recs : Q8Region[int(LN_BLOCK_GEMMS)]
    ln_block_regions(offs[rid / LN_BLOCK_GEMMS], d, ff, recs)
    let g = rid % LN_BLOCK_GEMMS
    let site = g == LN_GEMM_FC1 ? RailSite.ff : (g == LN_GEMM_FC2 ? RailSite.fc2 : RailSite.d)
    return TowerSchedRec(wblk0 = recs[g].woff / kq_elems(KqFmt.q8), rows = rail_site_rows(rl, site, npos),
        groups = rail_site_groups(rl, site, g == LN_GEMM_FC1 ? ff : d, npos))
}

//! gemma4v block record `rid`'s schedule row
def tower_gemma4v_rec(rl : auto(R); t : Gemma4vTower; npos, rid : int64) : TowerSchedRec {
    var recs : Q8Region[int(GEMMA4V_GEMMS)]
    gemma4v_block_regions(t, rid / GEMMA4V_GEMMS, recs)
    let g = rid % GEMMA4V_GEMMS
    let wide = g == GEMMA4V_GEMM_GATE || g == GEMMA4V_GEMM_UP
    let site = wide ? RailSite.ff : RailSite.d
    return TowerSchedRec(wblk0 = recs[g].woff / kq_elems(KqFmt.q8), rows = rail_site_rows(rl, site, npos),
        groups = rail_site_groups(rl, site, wide ? t.n_ff : t.n_embd, npos))
}

//! qwen3v block record `rid`'s schedule row: the fused [q | k | v] GEMM on its own site, the down GEMM on the d-wide one
def tower_qwen3v_rec(rl : auto(R); t : Qwen3vTower; npos, rid : int64) : TowerSchedRec {
    var recs : Q8Region[int(QWEN3V_GEMMS)]
    qwen3v_block_regions(t, rid / QWEN3V_GEMMS, recs)
    let g = rid % QWEN3V_GEMMS
    let site = g == QWEN3V_GEMM_QKV ? RailSite.qkv : (g == QWEN3V_GEMM_UP ? RailSite.ff : RailSite.d)
    let width = g == QWEN3V_GEMM_QKV ? 3l * t.n_embd : (g == QWEN3V_GEMM_UP ? t.ff_pad : t.n_embd)
    return TowerSchedRec(wblk0 = recs[g].woff / kq_elems(KqFmt.q8), rows = rail_site_rows(rl, site, npos), groups = rail_site_groups(rl, site, width, npos))
}

//! q / k / v at their offsets in `sstride`-wide rows restrided onto `hsp` heads with their bias rows, the flash tile, the rows back compact (`xh` under `att16`, else `att`)
def tower_attn_padded(var rl : auto(R); q, k, v : TowerBuf; sstride, npos, nheads, hs, hsp, q_b, k_b, v_b, q_off, k_off, v_off : int64; att16 : bool) {
    let dq = nheads * hs
    let dp = nheads * hsp
    let npos_pad = tower_cap_rows(npos)
    let wg_pad = (npos_pad * dp + 255l) / 256l
    let mulmm = rail_mulmm(rl)
    if (mulmm) {
        rail_bias_pass(rl, q, dq, npos, q_b)
        rail_bias_pass(rl, k, dq, npos, k_b)
        rail_bias_pass(rl, v, dq, npos, v_b)
    }
    var pc_rs = GkRestrideArgs(rows = uint(npos), rows_pad = uint(npos_pad), nheads = uint(nheads), hs = uint(hs), hsp = uint(hsp), sstride = uint(sstride), soff = uint(q_off))
    if (!mulmm && q_b >= 0l) {
        pc_rs.boff = uint(q_b - rl.norms_base)
        enc_tower_hpad_b_cls(rl, fixed_array(q, TowerBuf.qp, TowerBuf.norms), pc_rs, wg_pad)
    } else {
        enc_tower_hpad_cls(rl, fixed_array(q, TowerBuf.qp), pc_rs, wg_pad)
    }
    rail_pt(rl, RailProfRole.hpad)
    pc_rs.soff = uint(k_off)
    if (!mulmm && k_b >= 0l) {
        pc_rs.boff = uint(k_b - rl.norms_base)
        enc_tower_hpad16_b_cls(rl, fixed_array(k, TowerBuf.kp16, TowerBuf.norms), pc_rs, wg_pad)
    } else {
        enc_tower_hpad16_cls(rl, fixed_array(k, TowerBuf.kp16), pc_rs, wg_pad)
    }
    rail_pt(rl, RailProfRole.hpad)
    pc_rs.soff = uint(v_off)
    if (!mulmm && v_b >= 0l) {
        pc_rs.boff = uint(v_b - rl.norms_base)
        enc_tower_hpad16_b_cls(rl, fixed_array(v, TowerBuf.vp16, TowerBuf.norms), pc_rs, wg_pad)
    } else {
        enc_tower_hpad16_cls(rl, fixed_array(v, TowerBuf.vp16), pc_rs, wg_pad)
    }
    rail_pt(rl, RailProfRole.hpad)
    var pc_fa = FaArgs(rows = uint(npos), w0 = 0u, qd = uint(dp), kvd = uint(dp), kv_mul = 1u, kvbase = 0u, scale = 1.0 / sqrt(float(hs)), window = 0u, sinkoff = 0u)
    rail_fa(rl, hsp, TowerBuf.qp, TowerBuf.kp16, TowerBuf.vp16, TowerBuf.attp, pc_fa, nheads)
    rail_pt(rl, RailProfRole.fa)
    var pc_un = GkRestrideArgs(rows = uint(npos), rows_pad = uint(npos_pad), nheads = uint(nheads), hs = uint(hs), hsp = uint(hsp), sstride = uint(dq), soff = 0u)
    let wg_unpad = (npos * dp + 255l) / 256l
    if (att16) {
        enc_tower_hunpad16_cls(rl, fixed_array(TowerBuf.attp, TowerBuf.xh), pc_un, wg_unpad)
    } else {
        enc_tower_hunpad_cls(rl, fixed_array(TowerBuf.attp, TowerBuf.att), pc_un, wg_unpad)
    }
    rail_pt(rl, RailProfRole.hunpad)
}

//! the pre-LN block loop over `offs` from `x`: each seam folds the next pre-norm, the last the post-norm into `xb` (negative `lnp_w`: bare `x`); `k_bias` the K bias
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#tower-rail")]
def tower_ln_chain(var rl : auto(R); offs : array<LayerOffs>; k_bias : bool; fc1_act : uint; eps : float; d, ff, hs, hsp, nheads, npos, lnp_w, lnp_b : int64) {
    let nb = rl.norms_base
    let ln_post = lnp_w >= 0l
    let f16 = rl.lane != RailLane.f32 && !rail_mulmm(rl)
    let nl = long_length(offs)
    let nd = npos * d
    let nff = npos * ff
    for (l in range64(nl)) {
        let o = offs[l]
        let rb = l * LN_BLOCK_GEMMS
        if (l == 0l) {
            var pc = gk_ln_args(uint(d), uint(npos), eps, uint(o.ln1_w - nb), uint(o.ln1_b - nb))
            if (f16) {
                enc_tower_ln16_cls(rl, fixed_array(TowerBuf.x, TowerBuf.norms, TowerBuf.xh), pc, npos)
            } else {
                enc_tower_ln_cls(rl, fixed_array(TowerBuf.x, TowerBuf.xb, TowerBuf.norms), pc, npos)
            }
            rail_pt(rl, RailProfRole.ln)
        }
        rail_rq_feed(rl, TowerBuf.xb, nd)
        let qkv_fork = rail_site_nsplit(rl, RailSite.d) == 1l
        rail_fork(rl) if (qkv_fork)
        rail_lane_mm(rl, RailSite.d, rb + LN_GEMM_Q, TowerBuf.xb, TowerBuf.q, d, d, npos, o.q_w)
        rail_pt(rl, RailProfRole.gemm_q)
        rail_branch(rl, 1) if (qkv_fork)
        rail_lane_mm(rl, RailSite.d, rb + LN_GEMM_K, TowerBuf.xb, TowerBuf.k, d, d, npos, o.k_w)
        rail_pt(rl, RailProfRole.gemm_k)
        rail_branch(rl, 2) if (qkv_fork)
        rail_lane_mm(rl, RailSite.d, rb + LN_GEMM_V, TowerBuf.xb, TowerBuf.v, d, d, npos, o.v_w)
        rail_pt(rl, RailProfRole.gemm_v)
        rail_join(rl) if (qkv_fork)
        tower_attn_padded(rl, TowerBuf.q, TowerBuf.k, TowerBuf.v, d, npos, nheads, hs, hsp, o.q_b, k_bias ? o.k_b : TOWER_NO_BIAS, o.v_b, 0l, 0l, 0l, f16)
        rail_rq_feed(rl, TowerBuf.att, nd)
        rail_lane_mm(rl, RailSite.d, rb + LN_GEMM_O, TowerBuf.att, TowerBuf.yo, d, d, npos, o.o_w)
        rail_pt(rl, RailProfRole.gemm_o)
        var pc_post = GkPostAddLnArgs(d = uint(d), rows = uint(npos), boff = uint(o.o_b - nb), eps = eps, ln_on = 1u, lnoff = uint(o.ln2_w - nb),
            lnboff = uint(o.ln2_b - nb), ascale = 1.0, xboff = 0u)
        if (f16) {
            enc_tower_post_add_ln16_cls(rl, fixed_array(TowerBuf.x, TowerBuf.yo, TowerBuf.norms, TowerBuf.xh), pc_post, npos)
        } else {
            enc_tower_post_add_ln_cls(rl, fixed_array(TowerBuf.x, TowerBuf.yo, TowerBuf.xb, TowerBuf.norms), pc_post, npos)
        }
        rail_pt(rl, RailProfRole.post_add)
        rail_rq_feed(rl, TowerBuf.xb, nd)
        rail_lane_mm(rl, RailSite.ff, rb + LN_GEMM_FC1, TowerBuf.xb, TowerBuf.hg, d, ff, npos, o.fc1_w)
        rail_pt(rl, RailProfRole.gemm_fc1)
        var pc_gelu = GkBiasActArgs(d = uint(ff), nelem = uint(nff), boff = uint(o.fc1_b - nb), act = fc1_act)
        if (f16) {
            enc_tower_bias_act_cvt16_cls(rl, fixed_array(TowerBuf.hg, TowerBuf.norms, TowerBuf.xh), pc_gelu, rq_wgs(false, nff))
        } else {
            enc_tower_bias_act_cls(rl, fixed_array(TowerBuf.hg, TowerBuf.norms), pc_gelu, rq_wgs(false, nff))
        }
        rail_pt(rl, RailProfRole.bias)
        rail_rq_feed(rl, TowerBuf.hg, nff)
        rail_lane_mm(rl, RailSite.fc2, rb + LN_GEMM_FC2, TowerBuf.hg, TowerBuf.yd, ff, d, npos, o.fc2_w)
        rail_pt(rl, RailProfRole.gemm_fc2)
        rail_split_sum(rl, RailSite.fc2, TowerBuf.yd, npos, d)
        let last = l + 1l == nl
        pc_post.boff = uint(o.fc2_b - nb)
        pc_post.ln_on = (last && !ln_post) ? 0u : 1u
        if (!last) {
            pc_post.lnoff = uint(offs[l + 1l].ln1_w - nb)
            pc_post.lnboff = uint(offs[l + 1l].ln1_b - nb)
        } elif (ln_post) {
            pc_post.lnoff = uint(lnp_w - nb)
            pc_post.lnboff = uint(lnp_b - nb)
        }
        if (f16 && !(last && ln_post)) {
            enc_tower_post_add_ln16_cls(rl, fixed_array(TowerBuf.x, TowerBuf.yd, TowerBuf.norms, TowerBuf.xh), pc_post, npos)
        } else {
            enc_tower_post_add_ln_cls(rl, fixed_array(TowerBuf.x, TowerBuf.yd, TowerBuf.xb, TowerBuf.norms), pc_post, npos)
        }
        rail_pt(rl, RailProfRole.post_add)
    }
}

//! the patch im2col over the canvas's planes into the column plane, filled to the scratch's rows; the pixel map as `scale` and `shift`
def tower_vis_im2col(var rl : auto(R); w, patch, pd, npos : int64; scale, shift : float) {
    let total = rl.cap_rows * pd
    var pc = GkPatchIm2colArgs(w = uint(w), p = uint(patch), row_dim = uint(pd), npos = uint(npos), total = uint(total), scale = scale, shift = shift)
    enc_tower_patch_im2col_cls(rl, fixed_array(TowerBuf.planes, TowerBuf.col), pc, (total + 255l) / 256l)
    rail_pt(rl, RailProfRole.im2col)
}

//! the soft-token tail off `src`: the grid pool, gemma4v's scale and standardize rows, the soft rms, the clamped projection into `tail_proj`
def tower_vis_tail(var rl : auto(R); src : TowerBuf; vis : TowerVisTail) {
    let nb = rl.norms_base
    let nd = vis.nout * vis.d
    var pc_pool = GkPool2dArgs(width = uint(vis.d), gw = uint(vis.gw), k = uint(vis.merge), total = uint(nd))
    enc_tower_pool_2d_cls(rl, fixed_array(src, TowerBuf.tail_out), pc_pool, (nd + 255l) / 256l)
    rail_pt(rl, RailProfRole.pool)
    if (vis.affine) {
        var pc_aff = GkAffineRowsArgs(width = uint(vis.d), total = uint(nd), scale = vis.scale, boff = uint(vis.std ? vis.std_bias - nb : 0l),
            moff = uint(vis.std ? vis.std_scale - nb : 0l), std = vis.std ? 1u : 0u)
        enc_tower_affine_rows_cls(rl, fixed_array(TowerBuf.tail_out, TowerBuf.norms, TowerBuf.norms), pc_aff, (nd + 255l) / 256l)
    }
    var pc_rms = gk_rms_args(uint(vis.d), uint(vis.nout), vis.eps, uint(vis.soft_norm_w >= 0l ? vis.soft_norm_w - nb : rl.ones_off))
    enc_tower_rms_cls(rl, fixed_array(TowerBuf.tail_out, TowerBuf.tail_out, TowerBuf.norms), pc_rms, vis.nout)
    rail_pt(rl, RailProfRole.rms)
    if (vis.clamp.active) {
        var pc_ci = GkClampArgs(nelem = uint(nd), lo = vis.clamp.imin, hi = vis.clamp.imax, scale = 1.0)
        enc_tower_clamp_cls(rl, fixed_array(TowerBuf.tail_out), pc_ci, rq_wgs(false, nd))
    }
    rail_mm32(rl, rail_vis_w(vis.proj_bf16), vis.proj_bf16, TowerBuf.tail_out, TowerBuf.tail_proj, vis.d, vis.proj, vis.nout, vis.proj_w, TOWER_NO_BIAS, 0l)
    rail_pt(rl, RailProfRole.gemm_pw)
    if (vis.clamp.active) {
        var pc_co = GkClampArgs(nelem = uint(vis.nout * vis.proj), lo = vis.clamp.omin, hi = vis.clamp.omax, scale = 1.0)
        enc_tower_clamp_cls(rl, fixed_array(TowerBuf.tail_proj), pc_co, rq_wgs(false, vis.nout * vis.proj))
    }
}

//! a merger MLP over `nout` rows: `xin` -> `hid` via the biased f32 tile at `w1` / `b1` and tanh GELU, then `w2` / `b2` to `dout` columns at `yoff` of `tail_proj`
def tower_merger(var rl : auto(R); xin, hid : TowerBuf; din, dout, nout, w1, b1, w2, b2, yoff : int64) {
    rail_mm32(rl, TowerBuf.norms, false, xin, hid, din, din, nout, w1, b1, 0l)
    var pc_act = GkBiasActArgs(d = uint(din), nelem = uint(nout * din), boff = 0u, act = BIAS_ACT_GELU_TANH)
    enc_tower_act_cls(rl, fixed_array(hid), pc_act, rq_wgs(false, nout * din))
    rail_mm32(rl, TowerBuf.norms, false, hid, TowerBuf.tail_proj, din, dout, nout, w2, b2, yoff)
    rail_pt(rl, RailProfRole.gemm_pw)
}

//! the output bounds of a clamped site over [rows x dout] of `y`, where the clamp is active and the site takes them (`out_cl`)
def tower_clamp_out(var rl : auto(R); c : Clamp; out_cl : bool; y : TowerBuf; rows, dout : int64) {
    if (out_cl && c.active) {
        var pc_cl = GkClampArgs(nelem = uint(rows * dout), lo = c.omin, hi = c.omax, scale = 1.0)
        enc_tower_clamp_cls(rl, fixed_array(y), pc_cl, rq_wgs(false, rows * dout))
    }
}

//! the exact lane's clamped site: input bounds into `xc` under `copy` (the normed rows stay whole), else in place; the f32 tile at `woff`; output bounds under `out_cl`
def tower_mm_clamped32(var rl : auto(R); c : Clamp; copy, bf16 : bool; xin : TowerBuf; nin, n, dout, rows, woff : int64; y : TowerBuf; out_cl : bool) {
    if (copy || c.active) {
        var pc_in = GkClampArgs(nelem = uint(nin), lo = clamp_lo(c, true), hi = clamp_hi(c, true), scale = 1.0)
        if (copy) {
            enc_tower_clamp_copy_cls(rl, fixed_array(xin, TowerBuf.xc), pc_in, rq_wgs(false, nin))
        } else {
            enc_tower_clamp_cls(rl, fixed_array(xin), pc_in, rq_wgs(false, nin))
        }
    }
    rail_mm32(rl, rail_vis_w(bf16), bf16, copy ? TowerBuf.xc : xin, y, n, dout, rows, woff, TOWER_NO_BIAS, 0l)
    tower_clamp_out(rl, c, out_cl, y, rows, dout)
}

//! the q8 lane's clamped site: the input bounds on the way into the feed, the q8 tile over record `rid` at `site`, the output bounds under `out_cl`
def tower_mm_clamped_q8(var rl : auto(R); c : Clamp; xin : TowerBuf; nin : int64; site : RailSite; rid, n, dout, rows : int64; y : TowerBuf; out_cl : bool) {
    rail_site_feed(rl, site, xin, nin, clamp_lo(c, true), clamp_hi(c, true))
    rail_mm_kq(rl, site, rid, y, n, dout, rows)
    tower_clamp_out(rl, c, out_cl, y, rows, dout)
}

//! a gemma4v GEMM site on the chain's lane: the exact lane's clamped f32 site (`copy`, `bf16` read there alone), else the q8 lane's
def tower_mm_clamped(var rl : auto(R); c : Clamp; copy, bf16 : bool; xin : TowerBuf; nin : int64; site : RailSite; rid, n, dout, rows, woff : int64;
                     y : TowerBuf; out_cl : bool) {
    if (rl.lane == RailLane.f32) {
        tower_mm_clamped32(rl, c, copy, bf16, xin, nin, n, dout, rows, woff, y, out_cl)
    } else {
        tower_mm_clamped_q8(rl, c, xin, nin, site, rid, n, dout, rows, y, out_cl)
    }
}

//! gemma3v's encode: the patch conv with bias and positions into `x`, the pre-LN blocks with the post-norm in the last seam, the tail off `xb`
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#tower-rail")]
def tower_gemma3v_chain(var rl : auto(R); t : Gemma3vTower; offs : array<LayerOffs>; vis : TowerVisTail; w, pd, hsp, npos : int64) {
    let d = t.n_embd
    let nd = npos * d
    tower_vis_im2col(rl, w, t.patch, pd, npos, 1.0, 0.0)
    rail_mm32(rl, TowerBuf.norms, false, TowerBuf.col, TowerBuf.x, pd, d, npos, t.patch_w, t.patch_b, 0l)
    rail_pt(rl, RailProfRole.gemm_pw)
    var pc_pos = GkBiasActArgs(d = uint(nd), nelem = uint(nd), boff = uint(t.pos_w - rl.norms_base), act = BIAS_ACT_NONE)
    enc_tower_bias_act_cls(rl, fixed_array(TowerBuf.x, TowerBuf.norms), pc_pos, rq_wgs(false, nd))
    rail_pt(rl, RailProfRole.post_add)
    tower_ln_chain(rl, offs, true, BIAS_ACT_GELU_TANH, t.eps, d, t.ff_pad, d / t.n_head, hsp, t.n_head, npos, t.post_ln_w, t.post_ln_b)
    tower_vis_tail(rl, TowerBuf.xb, vis)
}

//! the gemma4uv embedder: the patch im2col, three layernorms around the patch linear and the 2-axis positions, the weightless rms, the projection into `tail_proj`
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#tower-rail")]
def tower_gemma4uv_chain(var rl : auto(R); t : Gemma4uvEmbedder; w, n_cols, npos : int64) {
    let nb = rl.norms_base
    let d = t.n_embd
    let pd = t.patch_dim
    let nd = npos * d
    tower_vis_im2col(rl, w, t.patch, pd, npos, 1.0, 0.0)
    var pc_ln1 = gk_ln_args(uint(pd), uint(npos), GEMMA4UV_LN_EPS, uint(t.ln1_w - nb), uint(t.ln1_b - nb))
    enc_tower_ln_cls(rl, fixed_array(TowerBuf.col, TowerBuf.col, TowerBuf.norms), pc_ln1, npos)
    rail_pt(rl, RailProfRole.ln)
    rail_mm32(rl, rail_vis_w(t.patch_bf16), t.patch_bf16, TowerBuf.col, TowerBuf.x, pd, d, npos, t.patch_w, t.patch_b, 0l)
    rail_pt(rl, RailProfRole.gemm_pw)
    var pc_ln2 = gk_ln_args(uint(d), uint(npos), GEMMA4UV_LN_EPS, uint(t.ln2_w - nb), uint(t.ln2_b - nb))
    enc_tower_ln_cls(rl, fixed_array(TowerBuf.x, TowerBuf.xb, TowerBuf.norms), pc_ln2, npos)
    var pc_pos = GkPosAdd2DArgs(dim = uint(d), ncols = uint(n_cols), pos_entries = uint(t.pos_entries), total = uint(nd), tboff = uint(t.pos_w - nb))
    enc_tower_pos_add_2d_cls(rl, fixed_array(TowerBuf.xb, TowerBuf.norms), pc_pos, (nd + 255l) / 256l)
    rail_pt(rl, RailProfRole.post_add)
    var pc_ln3 = gk_ln_args(uint(d), uint(npos), GEMMA4UV_LN_EPS, uint(t.ln3_w - nb), uint(t.ln3_b - nb))
    enc_tower_ln_cls(rl, fixed_array(TowerBuf.xb, TowerBuf.x, TowerBuf.norms), pc_ln3, npos)
    var pc_rms = gk_rms_args(uint(d), uint(npos), t.rms_eps, uint(rl.ones_off))
    enc_tower_rms_cls(rl, fixed_array(TowerBuf.x, TowerBuf.xb, TowerBuf.norms), pc_rms, npos)
    rail_pt(rl, RailProfRole.rms)
    rail_mm32(rl, rail_vis_w(t.proj_bf16), t.proj_bf16, TowerBuf.xb, TowerBuf.tail_proj, d, t.proj_dim, npos, t.proj_w, TOWER_NO_BIAS, 0l)
    rail_pt(rl, RailProfRole.gemm_pw)
}

//! gemma4v's encode: the patch conv and 2-axis positions into `x`, the post-normed clamped blocks, the soft-token tail off `x`; the rope tables packed in `tabs`
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#tower-rail")]
def tower_gemma4v_chain(var rl : auto(R); t : Gemma4vTower; vis : TowerVisTail; w, n_cols, npos : int64) {
    let nb = rl.norms_base
    let d = t.n_embd
    let ff = t.n_ff
    let nh = t.n_head
    let hs = d / nh
    let pd = t.patch_dim
    let nd = npos * d
    let nff = npos * ff
    let bf16 = t.blk_bf16
    tower_vis_im2col(rl, w, t.patch, pd, npos, 2.0, -1.0)
    rail_mm32(rl, rail_vis_w(t.patch_bf16), t.patch_bf16, TowerBuf.col, TowerBuf.x, pd, d, npos, t.patch_w, TOWER_NO_BIAS, 0l)
    rail_pt(rl, RailProfRole.gemm_pw)
    var pc_pos = GkPosAdd2DArgs(dim = uint(d), ncols = uint(n_cols), pos_entries = uint(t.pos_entries), total = uint(nd), tboff = uint(t.pos_w - nb))
    enc_tower_pos_add_2d_cls(rl, fixed_array(TowerBuf.x, TowerBuf.norms), pc_pos, (nd + 255l) / 256l)
    rail_pt(rl, RailProfRole.post_add)
    let wg_rope = (npos * nh * (hs / 4l) + 255l) / 256l
    var pc_fa = FaArgs(rows = uint(npos), w0 = 0u, qd = uint(d), kvd = uint(d), kv_mul = 1u, kvbase = 0u, scale = 1.0, window = 0u, sinkoff = 0u)
    var pc_rope = GkRope2dArgs(nheads = uint(nh), hs = uint(hs), rows = uint(npos))
    var pc_cvt = gk_f16_rows_args(uint(nd))
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
            var pc = gk_rms_args(uint(d), uint(npos), t.eps, uint(o.ln1 - nb))
            enc_tower_rms_cls(rl, fixed_array(TowerBuf.x, TowerBuf.xb, TowerBuf.norms), pc, npos)
        }
        tower_mm_clamped(rl, cq, true, bf16, TowerBuf.xb, nd, RailSite.d, rb + GEMMA4V_GEMM_Q, d, d, npos, o.w[GEMMA4V_GEMM_Q], TowerBuf.q, true)
        tower_mm_clamped(rl, ck, true, bf16, TowerBuf.xb, nd, RailSite.d, rb + GEMMA4V_GEMM_K, d, d, npos, o.w[GEMMA4V_GEMM_K], TowerBuf.k, true)
        tower_mm_clamped(rl, cv, true, bf16, TowerBuf.xb, nd, RailSite.d, rb + GEMMA4V_GEMM_V, d, d, npos, o.w[GEMMA4V_GEMM_V], TowerBuf.v, true)
        var pc_h = gk_rms_args(uint(hs), uint(npos * nh), t.eps, uint(o.q_norm - nb))
        enc_tower_rms_cls(rl, fixed_array(TowerBuf.q, TowerBuf.q2, TowerBuf.norms), pc_h, npos * nh)
        pc_h.lnoff = uint(o.k_norm - nb)
        enc_tower_rms_cls(rl, fixed_array(TowerBuf.k, TowerBuf.k2, TowerBuf.norms), pc_h, npos * nh)
        pc_h.lnoff = uint(rl.ones_off)
        enc_tower_rms_cls(rl, fixed_array(TowerBuf.v, TowerBuf.v2, TowerBuf.norms), pc_h, npos * nh)
        enc_tower_rope2d_cls(rl, fixed_array(TowerBuf.q2, TowerBuf.tabs), pc_rope, wg_rope)
        enc_tower_rope2d_cls(rl, fixed_array(TowerBuf.k2, TowerBuf.tabs), pc_rope, wg_rope)
        enc_tower_clamp_cvt_cls(rl, fixed_array(TowerBuf.k2, TowerBuf.k16), pc_cvt, rq_wgs(false, nd))
        enc_tower_clamp_cvt_cls(rl, fixed_array(TowerBuf.v2, TowerBuf.v16), pc_cvt, rq_wgs(false, nd))
        rail_fa(rl, hs, TowerBuf.q2, TowerBuf.k16, TowerBuf.v16, TowerBuf.att, pc_fa, nh)
        tower_mm_clamped(rl, co, false, bf16, TowerBuf.att, nd, RailSite.d, rb + GEMMA4V_GEMM_O, d, d, npos, o.w[GEMMA4V_GEMM_O], TowerBuf.yo, true)
        var pc_post = gk_add_post_rms_args(uint(d), uint(npos), t.eps, uint(o.attn_post - nb), true, uint(o.ln2 - nb))
        enc_tower_post_add_cls(rl, fixed_array(TowerBuf.x, TowerBuf.yo, TowerBuf.xb, TowerBuf.norms), pc_post, npos)
        tower_mm_clamped(rl, cg, true, bf16, TowerBuf.xb, nd, RailSite.ff, rb + GEMMA4V_GEMM_GATE, d, ff, npos, o.w[GEMMA4V_GEMM_GATE], TowerBuf.hg, false)
        tower_mm_clamped(rl, cu, true, bf16, TowerBuf.xb, nd, RailSite.ff, rb + GEMMA4V_GEMM_UP, d, ff, npos, o.w[GEMMA4V_GEMM_UP], TowerBuf.hu, false)
        var pc_gg = GkGegluArgs(nelem = uint(nff), glo = clamp_lo(cg, false), ghi = clamp_hi(cg, false), ulo = clamp_lo(cu, false), uhi = clamp_hi(cu, false),
            dlo = clamp_lo(cd, true), dhi = clamp_hi(cd, true))
        enc_tower_geglu_cls(rl, fixed_array(TowerBuf.hg, TowerBuf.hu), pc_gg, rq_wgs(false, nff))
        tower_mm_clamped(rl, cd, false, bf16, TowerBuf.hg, nff, RailSite.d, rb + GEMMA4V_GEMM_DOWN, ff, d, npos, o.w[GEMMA4V_GEMM_DOWN], TowerBuf.yd, true)
        let last = l + 1l == t.n_layer
        pc_post = gk_add_post_rms_args(uint(d), uint(npos), t.eps, uint(o.ffn_post - nb), !last, last ? 0u : uint(gemma4v_block_offs(t, l + 1l).ln1 - nb))
        enc_tower_post_add_cls(rl, fixed_array(TowerBuf.x, TowerBuf.yd, TowerBuf.xb, TowerBuf.norms), pc_post, npos)
    }
    tower_vis_tail(rl, TowerBuf.x, vis)
}

//! qwen3v's deepstack tap `k` off `x`: the merged rows' layernorm into `tail_out`, the tap's merger into slice `k + 1` of `tail_proj`
def tower_qwen3v_tap(var rl : auto(R); t : Qwen3vTower; k, nout : int64) {
    let d = t.n_embd
    let d4 = 4l * d
    let fo = t.ds_f + k * t.ds_f_stride
    let wo = t.ds_w + k * t.ds_w_stride
    var pc_tln = gk_ln_args(uint(d4), uint(nout), t.eps, uint(fo - rl.norms_base), uint(fo + d4 - rl.norms_base))
    enc_tower_ln_cls(rl, fixed_array(TowerBuf.x, TowerBuf.tail_out, TowerBuf.norms), pc_tln, nout)
    tower_merger(rl, TowerBuf.tail_out, TowerBuf.hg, d4, t.proj_dim, nout, wo, fo + 8l * d, wo + 16l * d * d, fo + 12l * d, (k + 1l) * nout * t.proj_dim)
}

//! qwen3v's encode: the patch conv and positions (`pos_resized` under `pos_rs`) into `x`, the fused-qkv pre-LN blocks, each deepstack tap's merger, the tail merger
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#tower-rail")]
def tower_qwen3v_chain(var rl : auto(R); t : Qwen3vTower; w, hsp, npos : int64; pos_rs : bool) {
    let nb = rl.norms_base
    let f16 = rl.lane != RailLane.f32 && !rail_mulmm(rl)
    let d = t.n_embd
    let ff = t.ff_pad
    let nh = t.n_head
    let hs = d / nh
    let nds = t.n_deepstack
    let d4 = 4l * d
    let nout = npos / 4l
    let proj = t.proj_dim
    let nd = npos * d
    let nff = npos * ff
    let wg_rope = (npos * nh * (hs / 2l) + 255l) / 256l
    tower_vis_im2col(rl, w, t.patch, t.patch_dim, npos, 1.0, 0.0)
    rail_mm32(rl, TowerBuf.norms, false, TowerBuf.col, TowerBuf.yo, t.patch_dim, d, npos, t.patch_w, TOWER_NO_BIAS, 0l)
    rail_pt(rl, RailProfRole.gemm_pw)
    var pc_asm = GkStemAsmArgs(dim = uint(d), pw = uint(w / t.patch), total = uint(nd), boff = uint(t.patch_b - nb), poff = pos_rs ? 0u : uint(t.pos_w - nb))
    enc_tower_stem_asm_cls(rl, fixed_array(TowerBuf.yo, TowerBuf.norms, pos_rs ? TowerBuf.pos_resized : TowerBuf.norms, TowerBuf.x), pc_asm, (nd + 255l) / 256l)
    rail_pt(rl, RailProfRole.shuffle)
    var pc_rope = GkRopeTabArgs(rows = uint(npos), qs = uint(3l * d), col = 0u, d = uint(nh * hs), dh = uint(hs), coff = 0u, soff = 0u)
    var pc_bqkv = GkBiasActArgs(d = uint(3l * d), nelem = uint(3l * nd), boff = 0u, act = BIAS_ACT_NONE)
    var pc_gelu = GkBiasActArgs(d = uint(ff), nelem = uint(nff), boff = 0u, act = BIAS_ACT_GELU_TANH)
    for (l in range64(t.n_layer)) {
        let o = qwen3v_block_offs(t, l)
        let rb = l * QWEN3V_GEMMS
        if (l == 0l) {
            var pc = gk_ln_args(uint(d), uint(npos), t.eps, uint(o.ln1_w - nb), uint(o.ln1_b - nb))
            if (f16) {
                enc_tower_ln16_cls(rl, fixed_array(TowerBuf.x, TowerBuf.norms, TowerBuf.xh), pc, npos)
            } else {
                enc_tower_ln_cls(rl, fixed_array(TowerBuf.x, TowerBuf.xb, TowerBuf.norms), pc, npos)
            }
        }
        rail_rq_feed(rl, TowerBuf.xb, nd)
        rail_lane_mm(rl, RailSite.qkv, rb + QWEN3V_GEMM_QKV, TowerBuf.xb, TowerBuf.qkv, d, 3l * d, npos, o.w[QWEN3V_GEMM_QKV])
        pc_bqkv.boff = uint(o.qkv_b - nb)
        enc_tower_bias_act_cls(rl, fixed_array(TowerBuf.qkv, TowerBuf.norms), pc_bqkv, rq_wgs(false, 3l * nd))
        pc_rope.col = 0u
        enc_tower_rope_tab_cls(rl, fixed_array(TowerBuf.qkv, TowerBuf.cos, TowerBuf.sin), pc_rope, wg_rope)
        pc_rope.col = uint(d)
        enc_tower_rope_tab_cls(rl, fixed_array(TowerBuf.qkv, TowerBuf.cos, TowerBuf.sin), pc_rope, wg_rope)
        tower_attn_padded(rl, TowerBuf.qkv, TowerBuf.qkv, TowerBuf.qkv, 3l * d, npos, nh, hs, hsp, TOWER_NO_BIAS, TOWER_NO_BIAS, TOWER_NO_BIAS, 0l, d, 2l * d, f16)
        rail_rq_feed(rl, TowerBuf.att, nd)
        rail_lane_mm(rl, RailSite.d, rb + QWEN3V_GEMM_O, TowerBuf.att, TowerBuf.yo, d, d, npos, o.w[QWEN3V_GEMM_O])
        var pc_post = gk_add_ln_args(uint(d), uint(npos), t.eps, uint(o.out_b - nb), true, uint(o.ln2_w - nb), uint(o.ln2_b - nb))
        if (f16) {
            enc_tower_post_add_ln16_cls(rl, fixed_array(TowerBuf.x, TowerBuf.yo, TowerBuf.norms, TowerBuf.xh), pc_post, npos)
        } else {
            enc_tower_post_add_ln_cls(rl, fixed_array(TowerBuf.x, TowerBuf.yo, TowerBuf.xb, TowerBuf.norms), pc_post, npos)
        }
        rail_rq_feed(rl, TowerBuf.xb, nd)
        rail_lane_mm(rl, RailSite.ff, rb + QWEN3V_GEMM_UP, TowerBuf.xb, TowerBuf.hg, d, ff, npos, o.w[QWEN3V_GEMM_UP])
        pc_gelu.boff = uint(o.up_b - nb)
        if (f16) {
            enc_tower_bias_act_cvt16_cls(rl, fixed_array(TowerBuf.hg, TowerBuf.norms, TowerBuf.xh), pc_gelu, rq_wgs(false, nff))
        } else {
            enc_tower_bias_act_cls(rl, fixed_array(TowerBuf.hg, TowerBuf.norms), pc_gelu, rq_wgs(false, nff))
        }
        rail_rq_feed(rl, TowerBuf.hg, nff)
        rail_lane_mm(rl, RailSite.d, rb + QWEN3V_GEMM_DOWN, TowerBuf.hg, TowerBuf.yd, ff, d, npos, o.w[QWEN3V_GEMM_DOWN])
        let last = l + 1l == t.n_layer
        let nx = last ? o : qwen3v_block_offs(t, l + 1l)
        pc_post = gk_add_ln_args(uint(d), uint(npos), t.eps, uint(o.down_b - nb), true, uint((last ? t.post_ln_w : nx.ln1_w) - nb), uint((last ? t.post_ln_b : nx.ln1_b) - nb))
        let k = qwen3v_tap_of(t, l)
        let tap = k >= 0l && k < nds
        if (f16 && !last && !tap) {
            enc_tower_post_add_ln16_cls(rl, fixed_array(TowerBuf.x, TowerBuf.yd, TowerBuf.norms, TowerBuf.xh), pc_post, npos)
        } else {
            enc_tower_post_add_ln_cls(rl, fixed_array(TowerBuf.x, TowerBuf.yd, TowerBuf.xb, TowerBuf.norms), pc_post, npos)
            if (f16 && tap && !last) {
                rail_feed(rl, TowerBuf.xb, nd, -F16_MAX, F16_MAX)
            }
        }
        if (tap) {
            tower_qwen3v_tap(rl, t, k, nout)
        }
    }
    tower_merger(rl, TowerBuf.xb, TowerBuf.tail_out, d4, proj, nout, t.mm0_w, t.mm0_b, t.mm2_w, t.mm2_b, 0l)
}

//! qwen25v's encode over its f16 twin (blocks from `t.layer_w`): the patch conv in window order into `x`, full and windowed blocks over `nwin` windows, the tail merger
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#tower-rail")]
def tower_qwen25v_chain(var rl : auto(R); t : Qwen25vTower; w, hsp, npos, nwin : int64) {
    let nb = rl.norms_base
    let d = t.n_embd
    let ff = t.ff_pad
    let nh = t.n_head
    let hs = d / nh
    let w_off = t.layer_w
    let nd = npos * d
    let nff = npos * ff
    let wg_rope = (npos * nh * (hs / 2l) + 255l) / 256l
    tower_vis_im2col(rl, w, t.patch, q25v_patch_pad(t), npos, 1.0, 0.0)
    rail_mm32(rl, TowerBuf.norms, false, TowerBuf.col, TowerBuf.yo, q25v_patch_pad(t), d, npos, t.patch_w, TOWER_NO_BIAS, 0l)
    rail_pt(rl, RailProfRole.gemm_pw)
    var pc_g = GkRowGatherArgs(c = uint(d), rows = uint(npos), aoff = 0u, boff = 0u, coff = 0u, out_off = 0u, add_b = 0u, add_c = 0u, scale = 1.0)
    enc_tts_row_gather_cls(rl, fixed_array(TowerBuf.yo, TowerBuf.win_sort, TowerBuf.norms, TowerBuf.norms, TowerBuf.x), pc_g, nd)
    rail_pt(rl, RailProfRole.shuffle)
    var pc_rope = GkRopeTabArgs(rows = uint(npos), qs = uint(d), col = 0u, d = uint(nh * hs), dh = uint(hs), coff = 0u, soff = 0u)
    var pc_glu = GkBiasGateArgs(d = uint(ff), nelem = uint(nff), goff = 0u, uoff = 0u)
    var pc_win = GkWinAttnArgs(nheads = uint(nh), hs = uint(hs), scale = 1.0 / sqrt(float(hs)))
    for (l in range64(t.n_layer)) {
        let o = qwen25v_block_offs(t, l)
        if (l == 0l) {
            var pc = gk_rms_args(uint(d), uint(npos), t.eps, uint(o.ln1_w - nb))
            enc_tower_rms_cls(rl, fixed_array(TowerBuf.x, TowerBuf.xb, TowerBuf.norms), pc, npos)
        }
        rail_feed(rl, TowerBuf.xb, nd, -F16_MAX, F16_MAX)
        rail_mm16(rl, TowerBuf.wb, TowerBuf.xh, TowerBuf.q, d, d, npos, o.w[QWEN25V_GEMM_Q] - w_off)
        rail_bias_pass(rl, TowerBuf.q, d, npos, o.b[QWEN25V_GEMM_Q])
        rail_mm16(rl, TowerBuf.wb, TowerBuf.xh, TowerBuf.k, d, d, npos, o.w[QWEN25V_GEMM_K] - w_off)
        rail_bias_pass(rl, TowerBuf.k, d, npos, o.b[QWEN25V_GEMM_K])
        rail_mm16(rl, TowerBuf.wb, TowerBuf.xh, TowerBuf.v, d, d, npos, o.w[QWEN25V_GEMM_V] - w_off)
        rail_bias_pass(rl, TowerBuf.v, d, npos, o.b[QWEN25V_GEMM_V])
        enc_tower_rope_tab_cls(rl, fixed_array(TowerBuf.q, TowerBuf.cos, TowerBuf.sin), pc_rope, wg_rope)
        enc_tower_rope_tab_cls(rl, fixed_array(TowerBuf.k, TowerBuf.cos, TowerBuf.sin), pc_rope, wg_rope)
        if (qwen25v_layer_full(t, l)) {
            tower_attn_padded(rl, TowerBuf.q, TowerBuf.k, TowerBuf.v, d, npos, nh, hs, hsp, TOWER_NO_BIAS, TOWER_NO_BIAS, TOWER_NO_BIAS, 0l, 0l, 0l, false)
        } else {
            enc_tower_win_attn_cls(rl, fixed_array(TowerBuf.q, TowerBuf.k, TowerBuf.v, TowerBuf.att, TowerBuf.win_table), pc_win, nwin * nh)
        }
        rail_feed(rl, TowerBuf.att, nd, -F16_MAX, F16_MAX)
        rail_mm16(rl, TowerBuf.wb, TowerBuf.xh, TowerBuf.yo, d, d, npos, o.w[QWEN25V_GEMM_O] - w_off)
        var pc_post = gk_add_ln_args(uint(d), uint(npos), t.eps, uint(o.b[QWEN25V_GEMM_O] - nb), true, uint(o.ln2_w - nb), 0u)
        enc_tower_post_add_rms_cls(rl, fixed_array(TowerBuf.x, TowerBuf.yo, TowerBuf.xb, TowerBuf.norms), pc_post, npos)
        rail_feed(rl, TowerBuf.xb, nd, -F16_MAX, F16_MAX)
        rail_mm16(rl, TowerBuf.wb, TowerBuf.xh, TowerBuf.hg, d, ff, npos, o.w[QWEN25V_GEMM_GATE] - w_off)
        rail_mm16(rl, TowerBuf.wb, TowerBuf.xh, TowerBuf.hu, d, ff, npos, o.w[QWEN25V_GEMM_UP] - w_off)
        pc_glu.goff = uint(o.b[QWEN25V_GEMM_GATE] - nb)
        pc_glu.uoff = uint(o.b[QWEN25V_GEMM_UP] - nb)
        enc_tower_bias_gate16_cls(rl, fixed_array(TowerBuf.hg, TowerBuf.hu, TowerBuf.norms, TowerBuf.xh), pc_glu, rq_wgs(false, nff))
        rail_mm16(rl, TowerBuf.wb, TowerBuf.xh, TowerBuf.yd, ff, d, npos, o.w[QWEN25V_GEMM_DOWN] - w_off)
        let last = l + 1l == t.n_layer
        pc_post = gk_add_ln_args(uint(d), uint(npos), t.eps, uint(o.b[QWEN25V_GEMM_DOWN] - nb), true, uint((last ? t.post_ln_w : qwen25v_block_offs(t, l + 1l).ln1_w) - nb), 0u)
        enc_tower_post_add_rms_cls(rl, fixed_array(TowerBuf.x, TowerBuf.yd, TowerBuf.xb, TowerBuf.norms), pc_post, npos)
    }
    tower_merger(rl, TowerBuf.xb, TowerBuf.tail_out, 4l * d, t.proj_dim, npos / 4l, t.mm0_w, t.mm0_b, t.mm2_w, t.mm2_b, 0l)
}
