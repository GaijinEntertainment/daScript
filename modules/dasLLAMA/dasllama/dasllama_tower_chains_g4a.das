options gen2
options indenting = 4
options _dasllama_internal = true

//! The spectral fronts' and the gemma4a / qwen3a / silero VAD chains' dispatch sequences, written once for both discrete homes:
//! every body is generic over the rail it dispatches through (`VtRail`, `CutRail`), each stamp named by its planes' roles, each
//! GEMM lane and feed a rail primitive (`dasllama_tower_rail`). Beside the bodies, the model geometry both homes' seats and bakes
//! read - the GEMM regions, the conv stages, the conv slabs. A seat keeps the gate, the uploads, the scratch, the schedule
//! staging and the copy-out; it builds its rail and calls the body here.
module dasllama_tower_chains_g4a shared public

require dasllama/dasllama_lint public
require math
require daslib/math_boost   // round_up: the conv columns' 64 lattice
require dasllama/dasllama_math   // reserve_resize
require dasllama/dasllama_kqformat
require dasllama/dasllama_tower
require dasllama/dasllama_tower_image   // tower_bake_q8, tower_dev_put: the families' device block pieces
require dasllama/dasllama_convert   // cvt_f32_to_f16_sat_array: the conv slabs' halfwords
require dasllama/dasllama_gpu_math
require dasllama/dasllama_gpu_kernels_common
require dasllama/dasllama_gpu_kernels_discrete
require dasllama/dasllama_gemma4a
require dasllama/dasllama_qwen3a
require dasllama/dasllama_tower_rail
require dasllama/dasllama_tower_chains   // tower_clamp_out
require ?cuda dasllama/dasllama_cuda_classes_front   // the fronts' stamps on the CUDA rail
require ?cuda dasllama/dasllama_cuda_classes_blocks   // the Conformer's stamps on the CUDA rail
require ?cuda dasllama/dasllama_cuda_classes_asr   // the VAD's stamps on the CUDA rail

//! a log filterbank's law over an `n_fft`-tap spectrum into `n_mel` rows: the magnitude or the power (`mag`), the floor `lo`,
//! the ceiling `hi` (`GK_MEL_LOG_CLAMP` alone), the log's `scale`, the rows' order (`frame_major`: [frame][mel], else
//! [mel][frame]) and the floor's law (`form`, a `GK_MEL_LOG_*`)
struct TowerMelSpec {
    n_fft : int64
    n_mel : int64
    mag : bool
    lo : float
    hi : float
    scale : float
    frame_major : bool
    form : uint
}

//! the log-mel of `frames` windowed frames: `planes` against the `twiddle` DFT into `spec`, the `filters` bank into the log rows in `mel`
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#g4a-chains")]
def tower_mel(var rl : auto(R); frames : int64; m : TowerMelSpec) {
    let n_bins = m.n_fft / 2l + 1l
    var pc_d = TowerSpecDftArgs(n_fft = uint(m.n_fft), n_frames = uint(frames), mag = m.mag ? TOWER_SPEC_MAGNITUDE : TOWER_SPEC_POWER)
    enc_tower_spec_dft_cls(rl, fixed_array(TowerBuf.planes, TowerBuf.twiddle, TowerBuf.spec), pc_d, (frames * n_bins + 255l) / 256l)
    rail_pt(rl, RailProfRole.spectrum)
    var pc_m = GkMelLogArgs(n_bins = uint(n_bins), n_mel = uint(m.n_mel), frames = uint(frames), lo = m.lo, hi = m.hi, scale = m.scale,
        frame_major = m.frame_major ? 1u : 0u, form = m.form)
    enc_tower_mel_log_cls(rl, fixed_array(TowerBuf.spec, TowerBuf.filters, TowerBuf.mel), pc_m, (frames * m.n_mel + 255l) / 256l)
    rail_pt(rl, RailProfRole.mel)
}

let TOWER_WHISPER_MEL_FLOOR = 5.960464477539063e-08   //! 2^-24, the whisper-class log-mel's floor (the CPU fronts' own)
let TOWER_LOG10_E = 0.43429448190325176               //! log10(e): the whisper preprocessors' log-mel scales the natural log to log10

//! the whisper preprocessors' log-mel law (whisper, qwen3a): the power spectrum, log10 over the 2^-24 floor, [mel][frame] rows
def tower_whisper_mel_spec(n_fft, n_mel : int64) : TowerMelSpec
    => TowerMelSpec(n_fft = n_fft, n_mel = n_mel, mag = false, lo = TOWER_WHISPER_MEL_FLOOR, hi = 0.0, scale = TOWER_LOG10_E, frame_major = false,
        form = GK_MEL_LOG_MAX)

let G4A_TAIL_GEMMS = 2l   //! the projector tail's records after the blocks' (`G4A_GEMMS` a block, `gemma4a_block_regions`): the out-proj, the mm-proj

//! gemma4a's two subsample convs as the front's f16 slab, taps on the 64 lattice, the second conv's rows from element `sub_ch0 x 64`
def tower_g4a_front16(t : Gemma4aEncoder) : array<uint16> {
    let kd1 = t.sub_ch0 * 9l
    var slab : array<float>
    slab |> reserve_resize(t.sub_ch0 * 64l + t.sub_ch1 * round_up(kd1, 64l))
    tower_stage_f32_pad_row_stride(slab, t.blob, t.c0_w, 0l, t.sub_ch0, 9l, 64l)
    tower_stage_f32_pad_row_stride(slab, t.blob, t.c1_w, t.sub_ch0 * 64l, t.sub_ch1, kd1, round_up(kd1, 64l))
    var slab16 <- cvt_f32_to_f16_sat_array(slab)
    delete slab
    return <- slab16
}

//! the subsample front's widths over `n_len` frames: the two k3 s2 stages' output grids, rows and column widths, the feature
//! rows' width and the positions they land
struct TowerG4aFront {
    t1 : int64
    w1 : int64
    t2 : int64
    w2 : int64
    rows0 : int64
    rows1 : int64
    kdp1 : int64
    feat : int64
    npos : int64
}

def tower_g4a_front_geom(t : Gemma4aEncoder; n_len : int64) : TowerG4aFront {
    let t1 = conv_s2_out(n_len)
    let w1 = conv_s2_out(t.n_mel)
    let t2 = conv_s2_out(t1)
    let w2 = conv_s2_out(w1)
    return TowerG4aFront(t1 = t1, w1 = w1, t2 = t2, w2 = w2, rows0 = t1 * w1, rows1 = t2 * w2, kdp1 = round_up(t.sub_ch0 * 9l, 64l), feat = w2 * t.sub_ch1,
        npos = t2)
}

//! the gemma4a front's planes' bytes at a scratch of `cap` frames: every plane `tower_g4a_front` binds but the model's own
struct TowerG4aFrontBytes {
    frames : int64
    twiddle : int64
    spec : int64
    filters : int64
    mel : int64
    col : int64
    stage1 : int64
    stage2 : int64
    feat16 : int64
}

def tower_g4a_front_bytes(t : Gemma4aEncoder; cap : int64) : TowerG4aFrontBytes {
    let n_fft = (t.n_bins - 1l) * 2l
    let g = tower_g4a_front_geom(t, cap)
    return TowerG4aFrontBytes(frames = cap * n_fft * 4l, twiddle = n_fft * 2l * t.n_bins * 4l, spec = cap * t.n_bins * 4l, filters = t.n_mel * t.n_bins * 4l,
        mel = cap * t.n_mel * 4l, col = max(g.rows0 * 64l, g.rows1 * g.kdp1) * 2l, stage1 = g.rows0 * t.sub_ch0 * 4l, stage2 = g.rows1 * t.sub_ch1 * 4l,
        feat16 = (g.t2 + TILE_READ_SLACK) * g.feat * 2l)
}

//! the gemma4a log-mel's law: the magnitude spectrum, the natural log over the family's floor, [frame][mel] rows
def tower_g4a_mel_spec(t : Gemma4aEncoder) : TowerMelSpec
    => TowerMelSpec(n_fft = (t.n_bins - 1l) * 2l, n_mel = t.n_mel, mag = true, lo = gemma4a_mel_floor(), hi = 0.0, scale = 1.0, frame_major = true,
        form = GK_MEL_LOG_MAX)

//! a clamped gemma4a GEMM site: the input bounds into the site's feed, the GEMM on the rail's lane (`rail_lane_mm`), the output bounds under `out_cl`
def tower_g4a_mm(var rl : auto(R); c : Clamp; xin : TowerBuf; nin : int64; site : RailSite; rid, n, dout, rows, woff : int64;
                 y : TowerBuf; out_cl : bool) {
    rail_site_feed(rl, site, xin, nin, clamp_lo(c, true), clamp_hi(c, true))
    rail_lane_mm(rl, site, rid, xin, y, n, dout, rows, woff)
    tower_clamp_out(rl, c, out_cl, y, rows, dout)
}

//! gemma4a's front over `n_len` frames in `planes`: the log-mel, both convs, the `feat16` feed, the `front1` projection over record `rid` into `y`
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#g4a-chains")]
def tower_g4a_front(var rl : auto(R); t : Gemma4aEncoder; n_len, rid : int64; y : TowerBuf) {
    let nb = rl.norms_base
    let ch0 = t.sub_ch0
    let ch1 = t.sub_ch1
    let g = tower_g4a_front_geom(t, n_len)
    tower_mel(rl, n_len, tower_g4a_mel_spec(t))
    var pc_im0 = GkIm2col2dArgs(iw = uint(t.n_mel), ih = uint(n_len), cin = 1u, sstride = 1u, kdp = 64u, rows = uint(g.rows0), soff = 0u)
    enc_tower_im2col_cls(rl, fixed_array(TowerBuf.mel, TowerBuf.col), pc_im0, (g.rows0 * 64l + 255l) / 256l)
    rail_pt(rl, RailProfRole.im2col)
    rail_mm16(rl, TowerBuf.fw16, TowerBuf.col, TowerBuf.stage1, 64l, ch0, g.rows0, 0l)
    rail_pt(rl, RailProfRole.gemm_pw)
    var pc_ln0 = GkLnReluRowsArgs(cout = uint(ch0), ldin = uint(ch0), ldout = uint(ch0), rows = uint(g.rows0), woff = uint(t.norm0_w - nb), eps = t.eps)
    enc_tower_ln_relu_cls(rl, fixed_array(TowerBuf.stage1, TowerBuf.stage1, TowerBuf.norms), pc_ln0, (g.rows0 + 255l) / 256l)
    rail_pt(rl, RailProfRole.bias)
    var pc_im1 = GkIm2col2dArgs(iw = uint(g.w1), ih = uint(g.t1), cin = uint(ch0), sstride = uint(ch0), kdp = uint(g.kdp1), rows = uint(g.rows1), soff = 0u)
    enc_tower_im2col_cls(rl, fixed_array(TowerBuf.stage1, TowerBuf.col), pc_im1, (g.rows1 * g.kdp1 + 255l) / 256l)
    rail_pt(rl, RailProfRole.im2col)
    rail_mm16(rl, TowerBuf.fw16, TowerBuf.col, TowerBuf.stage2, g.kdp1, ch1, g.rows1, ch0 * 64l)
    rail_pt(rl, RailProfRole.gemm_pw)
    var pc_ln1 = GkLnReluRowsArgs(cout = uint(ch1), ldin = uint(ch1), ldout = uint(ch1), rows = uint(g.rows1), woff = uint(t.norm1_w - nb), eps = t.eps)
    enc_tower_ln_relu_cls(rl, fixed_array(TowerBuf.stage2, TowerBuf.stage2, TowerBuf.norms), pc_ln1, (g.rows1 + 255l) / 256l)
    rail_pt(rl, RailProfRole.bias)
    let nfeat = g.npos * g.feat
    var pc_cvt = gk_f16_rows_args(uint(nfeat))
    enc_tower_clamp_cvt_cls(rl, fixed_array(TowerBuf.stage2, TowerBuf.feat16), pc_cvt, rq_wgs(false, nfeat))
    rail_pt(rl, RailProfRole.hpad)
    rail_lane_mm(rl, RailSite.front1, rid, TowerBuf.stage2, y, g.feat, t.d_model, g.npos, t.inproj_w)
    rail_pt(rl, RailProfRole.gemm_pw)
}

//! the rel-position table's rows: the window's past and the row itself
def tower_g4a_rel_rows() : int64 => gemma4a_attn_past() + 1l

//! the blocks' schedule records: every block's GEMMs, then the projector tail's two
def tower_g4a_block_recs(t : Gemma4aEncoder) : int64 => t.n_layer * G4A_GEMMS + G4A_TAIL_GEMMS

//! the GEMM site of a Conformer block's record `g`: the ff-wide ups, the 2d-wide pointwise, the rel table's projection, the d-wide rest
def tower_g4a_site(g : int64) : RailSite
    => (g == G4A_FF_UP || g == G4A_FF_UP_1) ? RailSite.ff : (g == G4A_PW1 ? RailSite.d2 : (g == G4A_REL ? RailSite.rel : RailSite.d))

//! gemma4a block record `rid`'s row over `npos`; the rel record keeps the table's raw rows, its planes sized to them (a forced l column reads past)
def tower_g4a_rec(rl : auto(R); t : Gemma4aEncoder; npos, rid : int64) : TowerSchedRec {
    let q8e = kq_elems(KqFmt.q8)
    let d = t.d_model
    let tail = tower_g4a_block_recs(t) - G4A_TAIL_GEMMS
    if (rid >= tail) {
        let out = rid == tail
        let site = out ? RailSite.tail1 : RailSite.tail2
        return TowerSchedRec(wblk0 = (out ? t.outproj_w : t.mmproj_w) / q8e, rows = rail_site_rows(rl, site, npos),
            groups = rail_site_groups(rl, site, out ? gemma4a_mid_dim(t) : t.proj_dim, npos))
    }
    var recs : Q8Region[int(G4A_GEMMS)]
    gemma4a_block_regions(t.layers[rid / G4A_GEMMS], d, t.n_ff, recs)
    let g = rid % G4A_GEMMS
    let site = tower_g4a_site(g)
    if (g == G4A_REL) {
        let rows = tower_g4a_rel_rows()
        return TowerSchedRec(wblk0 = recs[g].woff / q8e, rows = rows, groups = rail_site_groups(rl, site, d, rows))
    }
    let width = site == RailSite.ff ? t.n_ff : (site == RailSite.d2 ? 2l * d : d)
    return TowerSchedRec(wblk0 = recs[g].woff / q8e, rows = rail_site_rows(rl, site, npos), groups = rail_site_groups(rl, site, width, npos))
}

//! the GEMM records' schedule map words over `npos` rows: what a seat's schedule plane holds past the records
def tower_g4a_map_words(rl : auto(R); t : Gemma4aEncoder; npos : int64) : int64 {
    var words = 0l
    for (rid in range64(tower_g4a_block_recs(t))) {
        words += tower_g4a_rec(rl, t, npos, rid).groups
    }
    return words
}

//! gemma4a's Conformer blocks over `x`, then the projector tail into `tail_proj`; every GEMM through `tower_g4a_mm` on the image's lane
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#g4a-chains")]
def tower_g4a_blocks(var rl : auto(R); t : Gemma4aEncoder; npos : int64) {   // nolint:STYLE038 — the dispatch chain is the shape
    let nb = rl.norms_base
    let d = t.d_model
    let ff = t.n_ff
    let hs = t.d_head
    let mid = gemma4a_mid_dim(t)
    let rel_rows = tower_g4a_rel_rows()
    let nd = npos * d
    let nff = npos * ff
    let wg_attn = (npos * t.n_head + 63l) / 64l
    let tail = tower_g4a_block_recs(t) - G4A_TAIL_GEMMS
    rail_site_feed(rl, RailSite.rel, TowerBuf.rel_tab, rel_rows * d, -NO_CLAMP, NO_CLAMP)
    rail_pt(rl, RailProfRole.rq)
    var pc_glu = GkGatedRowsArgs(d = uint(d), nelem = uint(nd))
    var pc_dw = GkDwConvRowsArgs(c = uint(d), t_in = uint(npos))
    var pc_silu_ff = GkBiasActArgs(d = uint(ff), nelem = uint(nff), boff = 0u, act = BIAS_ACT_SILU)
    var pc_silu_d = GkBiasActArgs(d = uint(d), nelem = uint(nd), boff = 0u, act = BIAS_ACT_SILU)
    let sc = gemma4a_attn_scales(hs)
    var pc_attn = GkG4aAttnArgs(d = uint(d), npos = uint(npos), hs = uint(hs), pdsoff = 0u, cap = GK_G4A_ATTN_CAP, qscale = sc.qscale, kscale = sc.kscale)
    var pc_rms = gk_rms_args(uint(d), uint(npos), t.eps, 0u)
    var pc_post = GkPostAddLnArgs(d = uint(d), rows = uint(npos), woff = 0u, boff = 0u, eps = t.eps, ln_on = 1u, lnoff = 0u, lnboff = 0u, ascale = 1.0, xboff = 0u)
    for (l in range64(t.n_layer)) {
        let lo = t.layers[l]
        let rb = l * G4A_GEMMS
        pc_rms.lnoff = uint(lo.ff_norm - nb)
        enc_tower_rms_cls(rl, fixed_array(TowerBuf.x, TowerBuf.xb, TowerBuf.norms), pc_rms, npos)
        rail_pt(rl, RailProfRole.rms)
        tower_g4a_mm(rl, lo.c_ff_up, TowerBuf.xb, nd, RailSite.ff, rb + G4A_FF_UP, d, ff, npos, lo.ff_up, TowerBuf.hg, true)
        rail_pt(rl, RailProfRole.gemm_fc1)
        enc_tower_act_cls(rl, fixed_array(TowerBuf.hg), pc_silu_ff, rq_wgs(false, nff))
        rail_pt(rl, RailProfRole.silu)
        tower_g4a_mm(rl, lo.c_ff_down, TowerBuf.hg, nff, RailSite.d, rb + G4A_FF_DOWN, ff, d, npos, lo.ff_down, TowerBuf.yd, true)
        rail_pt(rl, RailProfRole.gemm_fc2)
        pc_post.woff = uint(lo.ff_post_norm - nb)
        pc_post.ascale = gemma4a_res_w()
        pc_post.lnoff = uint(lo.attn_pre_norm - nb)
        enc_tower_post_add_cls(rl, fixed_array(TowerBuf.x, TowerBuf.yd, TowerBuf.xb, TowerBuf.norms), pc_post, npos)
        rail_pt(rl, RailProfRole.post_add)
        tower_g4a_mm(rl, lo.c_q, TowerBuf.xb, nd, RailSite.d, rb + G4A_Q, d, d, npos, lo.q, TowerBuf.q, true)
        rail_pt(rl, RailProfRole.gemm_q)
        tower_g4a_mm(rl, lo.c_k, TowerBuf.xb, nd, RailSite.d, rb + G4A_K, d, d, npos, lo.k, TowerBuf.k, true)
        rail_pt(rl, RailProfRole.gemm_k)
        tower_g4a_mm(rl, lo.c_v, TowerBuf.xb, nd, RailSite.d, rb + G4A_V, d, d, npos, lo.v, TowerBuf.v, true)
        rail_pt(rl, RailProfRole.gemm_v)
        rail_lane_mm(rl, RailSite.rel, rb + G4A_REL, TowerBuf.rel_tab, TowerBuf.rel_proj, d, d, rel_rows, lo.attn_k_rel)
        rail_pt(rl, RailProfRole.gemm_rel)
        pc_attn.pdsoff = uint(lo.per_dim_scale - nb)
        let attn_roles = fixed_array(TowerBuf.q, TowerBuf.k, TowerBuf.v, TowerBuf.att, TowerBuf.rel_proj, TowerBuf.norms)
        if (hs == 64l) {
            enc_tower_g4a_attn64_cls(rl, attn_roles, pc_attn, wg_attn)
        } else {
            enc_tower_g4a_attn_cls(rl, attn_roles, pc_attn, wg_attn)
        }
        rail_pt(rl, RailProfRole.attn)
        tower_g4a_mm(rl, lo.c_o, TowerBuf.att, nd, RailSite.d, rb + G4A_O, d, d, npos, lo.o, TowerBuf.yo, true)
        rail_pt(rl, RailProfRole.gemm_o)
        pc_post.woff = uint(lo.attn_post_norm - nb)
        pc_post.ascale = 1.0
        pc_post.lnoff = uint(lo.norm_conv - nb)
        enc_tower_post_add_cls(rl, fixed_array(TowerBuf.x, TowerBuf.yo, TowerBuf.xb, TowerBuf.norms), pc_post, npos)
        rail_pt(rl, RailProfRole.post_add)
        tower_g4a_mm(rl, lo.c_pw1, TowerBuf.xb, nd, RailSite.d2, rb + G4A_PW1, d, 2l * d, npos, lo.conv_pw1, TowerBuf.hg, true)
        rail_pt(rl, RailProfRole.gemm_pw)
        enc_tower_glu_sig_cls(rl, fixed_array(TowerBuf.hg, TowerBuf.hu), pc_glu, (nd + 255l) / 256l)
        rail_pt(rl, RailProfRole.glu)
        pc_dw.woff = uint(lo.conv_dw - nb)
        enc_tower_dw_conv5_cls(rl, fixed_array(TowerBuf.hu, TowerBuf.yd, TowerBuf.norms), pc_dw, (nd + 255l) / 256l)
        rail_pt(rl, RailProfRole.dwconv)
        pc_rms.lnoff = uint(lo.conv_norm - nb)
        enc_tower_rms_cls(rl, fixed_array(TowerBuf.yd, TowerBuf.yd, TowerBuf.norms), pc_rms, npos)
        rail_pt(rl, RailProfRole.rms)
        enc_tower_act_cls(rl, fixed_array(TowerBuf.yd), pc_silu_d, rq_wgs(false, nd))
        rail_pt(rl, RailProfRole.silu)
        tower_g4a_mm(rl, lo.c_pw2, TowerBuf.yd, nd, RailSite.d, rb + G4A_PW2, d, d, npos, lo.conv_pw2, TowerBuf.yo, true)
        rail_pt(rl, RailProfRole.gemm_pw)
        pc_post.woff = 0u
        pc_post.lnoff = uint(lo.ff_norm_1 - nb)
        enc_tower_post_add_plain_cls(rl, fixed_array(TowerBuf.x, TowerBuf.yo, TowerBuf.xb, TowerBuf.norms), pc_post, npos)
        rail_pt(rl, RailProfRole.post_add)
        tower_g4a_mm(rl, lo.c_ff_up_1, TowerBuf.xb, nd, RailSite.ff, rb + G4A_FF_UP_1, d, ff, npos, lo.ff_up_1, TowerBuf.hg, true)
        rail_pt(rl, RailProfRole.gemm_fc1)
        enc_tower_act_cls(rl, fixed_array(TowerBuf.hg), pc_silu_ff, rq_wgs(false, nff))
        rail_pt(rl, RailProfRole.silu)
        tower_g4a_mm(rl, lo.c_ff_down_1, TowerBuf.hg, nff, RailSite.d, rb + G4A_FF_DOWN_1, ff, d, npos, lo.ff_down_1, TowerBuf.yd, true)
        rail_pt(rl, RailProfRole.gemm_fc2)
        pc_post.woff = uint(lo.ff_post_norm_1 - nb)
        pc_post.ascale = gemma4a_res_w()
        pc_post.ln_on = 0u
        enc_tower_post_add_cls(rl, fixed_array(TowerBuf.x, TowerBuf.yd, TowerBuf.xb, TowerBuf.norms), pc_post, npos)
        rail_pt(rl, RailProfRole.post_add)
        pc_post.ln_on = 1u
        pc_rms.lnoff = uint(lo.ln2 - nb)
        enc_tower_rms_cls(rl, fixed_array(TowerBuf.x, TowerBuf.x, TowerBuf.norms), pc_rms, npos)
        rail_pt(rl, RailProfRole.rms)
    }
    tower_g4a_mm(rl, Clamp(), TowerBuf.x, nd, RailSite.tail1, tail, d, mid, npos, t.outproj_w, TowerBuf.tail_out, false)
    rail_pt(rl, RailProfRole.gemm_o)
    rail_bias_pass(rl, TowerBuf.tail_out, mid, npos, t.outproj_b)
    var pc_rms_pt = gk_rms_args(uint(mid), uint(npos), t.eps, uint(rl.ones_off))
    enc_tower_rms_cls(rl, fixed_array(TowerBuf.tail_out, TowerBuf.tail_out, TowerBuf.norms), pc_rms_pt, npos)
    rail_pt(rl, RailProfRole.rms)
    tower_g4a_mm(rl, Clamp(), TowerBuf.tail_out, npos * mid, RailSite.tail2, tail + 1l, mid, t.proj_dim, npos, t.mmproj_w, TowerBuf.tail_proj, false)
    rail_pt(rl, RailProfRole.gemm_o)
}

//! the shape the gemma4a stamps serve: widths on the 64 lattice, 2d inside ff, 64 or 128 heads, the stamps' window and cap, both tail widths on the lattice
def tower_g4a_shape_ok(t : Gemma4aEncoder; npos : int64) : bool {
    let d = t.d_model
    let ff = t.n_ff
    let hs = t.d_head
    let mid = gemma4a_mid_dim(t)
    return (d % 64l == 0l && ff % 64l == 0l && ff >= 2l * d && t.n_head * hs == d && (hs == 64l || hs == 128l) && gemma4a_attn_past() == int64(GK_G4A_ATTN_PAST)
        && gemma4a_softcap() == GK_G4A_ATTN_CAP && npos > 0l && mid % 64l == 0l && mid <= max(d, ff) && t.proj_dim % 64l == 0l)
}

//! the gemma4a front's shape over `n_len` frames: stage two's rows the encoder's width, conv one's channels the mel's, conv two's on the 16 lattice
def tower_g4a_front_ok(t : Gemma4aEncoder; n_len : int64) : bool {
    let g = tower_g4a_front_geom(t, n_len)
    return n_len >= 1l && g.feat == t.d_model && t.sub_ch0 == t.n_mel && t.sub_ch1 % 16l == 0l && t.d_model % 64l == 0l
}

//! a k3 s2 p1 conv2d stage of a front's chunks: the input grid and channels, the output grid, the rows a chunk lands, the column
//! row's padded width, the stage's weight rows' element base in the f16 slab and its bias row's model offset
struct TowerConvStage {
    iw : int64
    ih : int64
    cin : int64
    ow : int64
    oh : int64
    rows : int64
    kdp : int64
    slab_elem_off : int64
    boff : int64
}

def tower_conv_stage(iw, ih, cin : int64) : TowerConvStage {
    let ow = conv_s2_out(iw)
    let oh = conv_s2_out(ih)
    return TowerConvStage(iw = iw, ih = ih, cin = cin, ow = ow, oh = oh, rows = oh * ow, kdp = round_up(cin * 9l, 64l))
}

//! qwen3a's three conv stages over a chunk of the mel, each stage's weight rows placed in the slab after the last's
def tower_q3a_stages(t : Qwen3aTower) : TowerConvStage[3] {
    let nch = t.n_ch
    var st : TowerConvStage[3]
    st[0] = tower_conv_stage(QWEN3A_CHUNK, t.core.n_mel, 1l)
    st[1] = tower_conv_stage(st[0].ow, st[0].oh, nch)
    st[2] = tower_conv_stage(st[1].ow, st[1].oh, nch)
    let boffs = fixed_array(t.c1_b_off, t.c2_b_off, t.c3_b_off)
    var total = 0l
    for (s in range(3)) {
        st[s].slab_elem_off = total
        st[s].boff = boffs[s]
        total += nch * st[s].kdp
    }
    return st
}

//! the `cblob` span [lo, hi) a seat's norms plane holds: the three conv biases, the conv_out bias, the position rows, the projector's two biases
def tower_q3a_norm_span(t : Qwen3aTower) : tuple<lo : int64; hi : int64> {
    let nch = t.n_ch
    let d = t.core.d_model
    let offs = fixed_array(t.c1_b_off, t.c2_b_off, t.c3_b_off, t.cout_b_off, t.pos_off, t.mm1_b_off, t.mm2_b_off)
    let ends = fixed_array(t.c1_b_off + nch, t.c2_b_off + nch, t.c3_b_off + nch, t.cout_b_off + d, t.pos_off + QWEN3A_TOK_PER_CHUNK * d,
        t.mm1_b_off + d, t.mm2_b_off + t.proj_dim)
    var lo = offs[0]
    var hi = ends[0]
    for (i in range(1, 7)) {
        lo = min(lo, offs[i])
        hi = max(hi, ends[i])
    }
    return (lo = lo, hi = hi)
}

//! the qwen3a front's plane bytes at `cap_chunks` chunks: the chunk blocks, the shared column plane, each stage's rows
def tower_q3a_front_bytes(t : Qwen3aTower; cap_chunks : int64) : tuple<xin : int64; col : int64; y : int64[3]> {
    let st = tower_q3a_stages(t)
    var col = 0l
    var y : int64[3]
    for (s in range(3)) {
        col = max(col, cap_chunks * st[s].rows * st[s].kdp * 2l)
        y[s] = cap_chunks * st[s].rows * t.n_ch * 4l
    }
    return (xin = cap_chunks * QWEN3A_CHUNK * t.core.n_mel * 4l, col = col, y = y)
}

//! qwen3a's three convs as the front's f16 slab, each row's taps on the f16 GEMM's 64 lattice
def tower_q3a_front16(t : Qwen3aTower) : array<uint16> {
    let nch = t.n_ch
    let st = tower_q3a_stages(t)
    let woffs = fixed_array(t.c1_w_off, t.c2_w_off, t.c3_w_off)
    var slab : array<float>
    slab |> reserve_resize(st[2].slab_elem_off + nch * st[2].kdp)
    for (s in range(3)) {
        tower_stage_f32_pad_row_stride(slab, t.cblob, woffs[s], st[s].slab_elem_off, nch, st[s].cin * 9l, st[s].kdp)
    }
    var slab16 <- cvt_f32_to_f16_sat_array(slab)
    delete slab
    return <- slab16
}


//! gemma4a's device block under the home's prefix (`#tower-dev-block`): the q8 regions on the q8 lane, the conv slab on both
[arch(at = "../ARCHITECTURE_IMAGE_GPU_PLANES.md#tower-dev-block")]
def tower_g4a_bake(t : Gemma4aEncoder; var b : TowerDevBuild; home : string) {
    if (t.q8) {
        var inscope regions <- gemma4a_all_regions(t)
        tower_bake_q8(b, tower_family_piece(home, "g4a"), t.qblob, t.qscales, regions)
    }
    var inscope slab16 <- tower_g4a_front16(t)
    tower_dev_put(b, tower_front16_piece(home, "g4a"), unsafe(addr<uint8 const?>(slab16[0])), long_length(slab16) * 2l)
}

//! qwen3a's pieces under the home's prefix, behind the core's: the front's f16 slab, and on the q8 lane the conv-out and projector GEMMs
[arch(at = "../ARCHITECTURE_IMAGE_GPU_PLANES.md#tower-dev-block")]
def tower_q3a_bake(t : Qwen3aTower; var b : TowerDevBuild; home : string) {
    var inscope slab16 <- tower_q3a_front16(t)
    tower_dev_put(b, tower_front16_piece(home, "q3a"), unsafe(addr<uint8 const?>(slab16[0])), long_length(slab16) * 2l)
    return if (!t.cq8)
    var inscope regions <- tower_q3a_regions(t, true)
    tower_bake_q8(b, tower_family_piece(home, "q3a"), t.qcblob, t.qcscales, regions)
}

//! the qwen3a front's shape over `n_chunks`: stage three the shuffle's 13 x 16 grid, channels and widths on the tiles' lattices
def tower_q3a_front_ok(t : Qwen3aTower; n_chunks : int64) : bool {
    let st = tower_q3a_stages(t)
    let featw = 16l * t.n_ch
    return (st[2].ow == QWEN3A_TOK_PER_CHUNK && st[2].oh == 16l && t.n_ch % 16l == 0l && featw % 64l == 0l && t.core.d_model % 64l == 0l
        && n_chunks > 0l && QWEN3A_TOK_PER_CHUNK == int64(Q3A_TOK_PER_CHUNK))
}

//! the front's conv-out GEMM and the projector's two fed off the f16 feed in `feat16`
def tower_q3a_binds(var rl : auto(R)) {
    rail_bind_f16(rl, fixed_array(RailSite.front1, RailSite.tail1, RailSite.tail2), TowerBuf.feat16)
}

//! qwen3a's front over `n_chunks` mel chunks in `planes`: three im2col + f16 GEMM + GELU stages, the shuffle into `feat16`, the `front1` GEMM into `rows_out`
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#g4a-chains")]
def tower_q3a_front(var rl : auto(R); t : Qwen3aTower; n_chunks, rid : int64) {
    let nb = rl.norms_base
    let nch = t.n_ch
    let d = t.core.d_model
    let featw = 16l * nch
    let npos = n_chunks * QWEN3A_TOK_PER_CHUNK
    let st = tower_q3a_stages(t)
    let srcs = fixed_array(TowerBuf.planes, TowerBuf.stage1, TowerBuf.stage2)
    let ys = fixed_array(TowerBuf.stage1, TowerBuf.stage2, TowerBuf.stage3)
    for (si in range(3)) {
        let rows = n_chunks * st[si].rows
        var pc_im = GkIm2col2dArgs(iw = uint(st[si].iw), ih = uint(st[si].ih), cin = uint(st[si].cin), sstride = uint(st[si].cin), kdp = uint(st[si].kdp),
            rows = uint(rows), soff = 0u)
        enc_tower_im2col_cls(rl, fixed_array(srcs[si], TowerBuf.col), pc_im, (rows * st[si].kdp + 255l) / 256l)
        rail_pt(rl, RailProfRole.im2col)
        rail_mm16(rl, TowerBuf.fw16, TowerBuf.col, ys[si], st[si].kdp, nch, rows, st[si].slab_elem_off)
        rail_pt(rl, RailProfRole.gemm_pw)
        var pc_b = GkBiasActArgs(d = uint(nch), nelem = uint(rows * nch), boff = uint(st[si].boff - nb), act = BIAS_ACT_GELU_ERF)
        enc_tower_bias_act_cls(rl, fixed_array(ys[si], TowerBuf.norms), pc_b, rq_wgs(false, rows * nch))
        rail_pt(rl, RailProfRole.bias)
    }
    var pc_sh = GkQ3aShuffleArgs(nch = uint(nch), ncp = uint(nch), chunk_stride = uint(16l * QWEN3A_TOK_PER_CHUNK * nch), npos = uint(npos))
    enc_tower_q3a_shuffle_cls(rl, fixed_array(TowerBuf.stage3, TowerBuf.feat16), pc_sh, (npos * featw + 255l) / 256l)
    rail_pt(rl, RailProfRole.shuffle)
    rail_lane_mm(rl, RailSite.front1, rid, TowerBuf.feat16, TowerBuf.rows_out, featw, d, npos, t.cout_w_off)
    rail_pt(rl, RailProfRole.gemm_pw)
    var pc_fin = GkBiasActArgs(d = uint(d), nelem = uint(npos * d), boff = uint(t.cout_b_off - nb), act = BIAS_ACT_NONE)
    enc_tower_bias_act_cls(rl, fixed_array(TowerBuf.rows_out, TowerBuf.norms), pc_fin, rq_wgs(false, npos * d))
    pc_fin.d = uint(QWEN3A_TOK_PER_CHUNK * d)
    pc_fin.boff = uint(t.pos_off - nb)
    enc_tower_bias_act_cls(rl, fixed_array(TowerBuf.rows_out, TowerBuf.norms), pc_fin, rq_wgs(false, npos * d))
    rail_pt(rl, RailProfRole.post_add)
}

//! qwen3a's MLP projector over `rows_out` on the chain's lane: mm1 at `tail1` (record `rid`) into `tail_hid`, mm2 at `tail2` (`rid + 1`) into `tail_proj`
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#g4a-chains")]
def tower_q3a_proj(var rl : auto(R); t : Qwen3aTower; npos, rid : int64) {
    let d = t.core.d_model
    let pdim = t.proj_dim
    let nh = npos * d
    rail_site_feed(rl, RailSite.tail1, TowerBuf.rows_out, nh, -NO_CLAMP, NO_CLAMP)
    rail_lane_mm(rl, RailSite.tail1, rid, TowerBuf.rows_out, TowerBuf.tail_hid, d, d, npos, t.mm1_w_off)
    rail_pt(rl, RailProfRole.gemm_o)
    var pc_b1 = GkBiasActArgs(d = uint(d), nelem = uint(nh), boff = uint(t.mm1_b_off - rl.norms_base), act = BIAS_ACT_GELU_ERF)
    enc_tower_bias_act_cls(rl, fixed_array(TowerBuf.tail_hid, TowerBuf.norms), pc_b1, rq_wgs(false, nh))
    rail_pt(rl, RailProfRole.bias)
    rail_site_feed(rl, RailSite.tail2, TowerBuf.tail_hid, nh, -NO_CLAMP, NO_CLAMP)
    rail_lane_mm(rl, RailSite.tail2, rid + 1l, TowerBuf.tail_hid, TowerBuf.tail_proj, d, pdim, npos, t.mm2_w_off)
    rail_pt(rl, RailProfRole.gemm_o)
    rail_bias_pass(rl, TowerBuf.tail_proj, pdim, npos, t.mm2_b_off)
}

//! the VAD over `pa.n` chunks: the encoder a workgroup a chunk into `vad_gates`, then one workgroup's recurrence over `vad_state` into `vad_probs`
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#g4a-chains")]
def tower_vad_chunks(var rl : auto(R); var pa : GkVadArgs) {
    enc_tower_vad_enc_cls(rl, fixed_array(TowerBuf.planes, TowerBuf.norms, TowerBuf.vad_gates), pa, int64(pa.n))
    rail_pt(rl, RailProfRole.spectrum)
    enc_tower_vad_lstm_cls(rl, fixed_array(TowerBuf.vad_gates, TowerBuf.norms, TowerBuf.vad_state, TowerBuf.vad_probs), pa, 1l)
    rail_pt(rl, RailProfRole.tts_lstm)
}
