options gen2
options indenting = 4
options _dasllama_internal = true

//! The whisper-class towers' dispatch sequences, written once for both discrete homes (`ARCHITECTURE_GPU_TOWER_RAIL.md#tower-rail`):
//! the conv stem, the pre-LN blocks with the chat projector tail behind them, generic over the rail (`VtRail`, `CutRail`), and the
//! home-free shape every seat reads beside them - the tail's record and schedule rows, the q8 regions a bake gathers, the stem's
//! f16 slab. The whisper encoder, qwen2audio, Qwen2.5-Omni's audio tower, ultravox, voxtral and qwen3a's core ride them.
module dasllama_tower_chains_whisper shared public

require dasllama/dasllama_lint public
require math
require dasllama/dasllama_kqformat
require dasllama/dasllama_math
require dasllama/dasllama_convert   // cvt_f32_to_f16_sat_array: the stem's f16 slab
require dasllama/dasllama_tower
require dasllama/dasllama_tower_image   // tower_bake_q8: the q8 lane's pieces
require dasllama/dasllama_gpu_math
require dasllama/dasllama_gpu_kernels_common
require dasllama/dasllama_gpu_kernels_discrete
require dasllama/dasllama_audio
require dasllama/dasllama_tower_rail
require dasllama/dasllama_tower_chains
require ?cuda dasllama/dasllama_cuda_classes_front   // the stem's im2col stamp on the CUDA rail
require ?cuda dasllama/dasllama_cuda_classes_blocks   // the chat tail's pool and SwiGLU stamps on the CUDA rail

//! the chat projector tail behind the blocks (`audio_projector_tail`'s steps at its widths): the rows, the GEMM widths and the
//! model offsets its stamps read; `on` where the seat's classes built for this encode
struct TowerChatTail {
    on : bool
    kind : AudioProjKind
    rows : int64        //! the soft tokens a chunk
    pool_rows : int64   //! the rows the post-norm reads: the pooled pairs (qwen2a, voxtral), every position (ultravox)
    m1in : int64        //! the first GEMM's K - the stacked row
    n1out : int64       //! its width
    m2in : int64        //! the second GEMM's K (0 = one GEMM)
    out : int64         //! the soft tokens' width
    w1 : int64          //! the GEMMs' weight offsets in the q8 plane
    w2 : int64
    lnp_w : int64       //! the post-norm rows, the ultravox norm rows and qwen2a's bias row: model offsets in the f32 blob
    lnp_b : int64
    normpre : int64
    normmid : int64
    proj_b : int64
    eps : float
}

//! a whisper-class tower has a conv stem when its layers start past the stem's planes (a conv-less core, qwen3a's, starts at 0 on every lane)
def tower_aud_has_conv(t : AudioTower) : bool => ((t.q8 || t.gemm_f16) ? t.qlayers_off : t.layers_off) > 0l

//! the tower's block offsets into `offs`, one record a block
def tower_aud_offs(t : AudioTower; @scratch @exact_size var offs : array<LayerOffs>) {
    offs |> grow_resize(t.n_layer)
    for (l in range64(t.n_layer)) {
        offs[l] = enc_layer_offsets(t, l)
    }
}

//! the tail's GEMMs on the tiles' lattice, one gate for both lanes: every K and N a 64-multiple (a 32-multiple K is q8 on a q8 tower)
def tower_ct_planes_ok(t : AudioTower) : bool {
    return false if (t.proj_dim <= 0l || t.proj_dim % 64l != 0l)
    return true if (t.proj_kind == AudioProjKind.qwen2a)
    return (t.d_model * t.stack_factor) % 64l == 0l && t.mm1_out % 64l == 0l && audio_proj_mid_width(t) % 64l == 0l
}

//! whether this encode's tail rides the chain: the planes, the pooled rows even (the CPU tail's floor), the feeds inside the scratch
def tower_ct_shape_ok(t : AudioTower; npos : int64) : bool {
    return false if (!tower_ct_planes_ok(t))
    let pool = audio_has_avgpool(t)
    let rows = audio_tokens_per_chunk(t)
    let m1in = t.d_model * t.stack_factor
    return (npos == t.n_ctx && (!pool || npos % 2l == 0l) && rows > 0l
        && tower_cap_rows(rows) * max(m1in, audio_proj_mid_width(t)) <= tower_cap_rows(npos) * max(t.d_model, t.n_ff))
}

//! the projector planes a bake gathers beside the block records, in the repack's (K, rows) shape
def tower_ct_regions(t : AudioTower; var regions : array<Q8Region>) {
    return if (!tower_ct_planes_ok(t))
    if (t.proj_kind == AudioProjKind.qwen2a) {
        regions |> push(Q8Region(woff = t.proj_w_off, n = t.d_model, rows = t.proj_dim))
        return
    }
    regions |> push(Q8Region(woff = t.proj_w_off, n = t.d_model * t.stack_factor, rows = t.mm1_out))
    regions |> push(Q8Region(woff = t.mm2_w_off, n = audio_proj_mid_width(t), rows = t.proj_dim))
}

//! a whisper-class tower's q8 regions: the projector tail's, the stem's second conv where it has one, every block's GEMMs
def tower_aud_regions(t : AudioTower; offs : array<LayerOffs>) : array<Q8Region> {
    var regions : array<Q8Region>
    regions |> reserve(long_length(offs) * LN_BLOCK_GEMMS + 3l)
    tower_ct_regions(t, regions)
    if (tower_aud_has_conv(t)) {
        regions |> push(Q8Region(woff = t.conv2_w_off, n = 3l * t.d_model, rows = t.d_model))
    }
    var recs : Q8Region[int(LN_BLOCK_GEMMS)]
    for (o in offs) {
        ln_block_regions(o, t.d_model, t.n_ff, recs)
        regions |> push_from(recs)
    }
    return <- regions
}

//! this encode's tail record off the tower: the row counts, the widths, the model offsets; `on` is the seat's to set
def tower_chat_tail_of(t : AudioTower; npos : int64) : TowerChatTail {
    let uv = t.proj_kind == AudioProjKind.ultravox
    let one = t.proj_kind == AudioProjKind.qwen2a
    return TowerChatTail(kind = t.proj_kind, eps = t.eps, rows = audio_tokens_per_chunk(t), pool_rows = uv ? npos : npos / 2l,
        m1in = t.d_model * t.stack_factor, n1out = one ? t.proj_dim : t.mm1_out, m2in = one ? 0l : audio_proj_mid_width(t), out = t.proj_dim,
        w1 = t.proj_w_off, w2 = t.mm2_w_off, lnp_w = t.lnp_w_off, lnp_b = t.lnp_b_off, normpre = t.normpre_off, normmid = t.normmid_off,
        proj_b = t.proj_b_off)
}

//! a seat's tail standing down for an encode whose class did not build: the blocks serve, the CPU tail reads their rows; `home` names the home
[cold_path]
def tower_ct_stand_down(home : string; kind : AudioProjKind) {
    to_log(LOG_INFO, "dasLLAMA {home} tower: the {kind} projector tail stands down - a class failed to build; the blocks serve\n")
}

//! the tail's GEMMs this encode dispatches
def tower_ct_gemms(ct : TowerChatTail) : int64 => ct.on ? (ct.m2in > 0l ? 2l : 1l) : 0l

//! the tail's hidden rows' width (0: one GEMM)
def tower_ct_hid(ct : TowerChatTail) : int64 => ct.m2in > 0l ? ct.n1out : 0l

//! the gated rows' width (ultravox alone)
def tower_ct_gate(ct : TowerChatTail) : int64 => ct.kind == AudioProjKind.ultravox ? ct.m2in : 0l

//! the tail's GEMM record `i` (0: the first projector GEMM on `tail1`, 1: the second on `tail2`) behind the blocks' in the schedule
def tower_ct_rec(rl : auto(R); ct : TowerChatTail; i : int64) : TowerSchedRec {
    let second = i > 0l
    let site = second ? RailSite.tail2 : RailSite.tail1
    return TowerSchedRec(wblk0 = (second ? ct.w2 : ct.w1) / kq_elems(KqFmt.q8), rows = rail_site_rows(rl, site, ct.rows),
        groups = rail_site_groups(rl, site, second ? ct.out : ct.n1out, ct.rows))
}

//! the stem's first conv's padded column width: three taps a mel on the f16 GEMM's 64 lattice
def tower_aud_stem_kdp(t : AudioTower) : int64 => round_up(3l * t.n_mel, 64l)

//! the stem's first conv as its f16 slab: each row's three taps a mel on the f16 GEMM's 64 lattice
def tower_aud_stem16(t : AudioTower) : array<uint16> {
    let kreal = 3l * t.n_mel
    let kdp = tower_aud_stem_kdp(t)
    var slab : array<float>
    slab |> reserve_resize(t.d_model * kdp)
    tower_stage_f32_pad_row_stride(slab, t.fblob, t.conv1_w_off, 0l, t.d_model, kreal, kdp)
    var slab16 <- cvt_f32_to_f16_sat_array(slab)
    delete slab
    return <- slab16
}

//! a whisper-class tower's device block under the home's prefix (`#tower-dev-block`): the q8 regions, and the stem's f16 slab where it has a stem
[arch(at = "../ARCHITECTURE_IMAGE_GPU_PLANES.md#tower-dev-block")]
def tower_aud_bake(t : AudioTower; var b : TowerDevBuild; home : string) {
    if (t.q8) {
        var inscope offs : array<LayerOffs>
        tower_aud_offs(t, offs)
        var inscope regions <- tower_aud_regions(t, offs)
        tower_bake_q8(b, tower_family_piece(home, "aud"), t.qblob, t.qscales, regions)
    }
    return if (!tower_aud_has_conv(t))
    var inscope slab16 <- tower_aud_stem16(t)
    tower_dev_put(b, tower_front16_piece(home, "aud"), unsafe(addr<uint8 const?>(slab16[0])), long_length(slab16) * 2l)
}

//! the stem's f32 slab rows: conv1's bias, conv2's, the position rows - the stem's bias stamps read them at 0, `d` and `2 d`
def tower_aud_stem_rows(t : AudioTower) : int64 => 2l * t.d_model + t.n_ctx * t.d_model

//! the stem's second conv's schedule row, record 0 of the stem's schedule
def tower_aud_stem_rec(rl : auto(R); t : AudioTower) : TowerSchedRec
    => TowerSchedRec(wblk0 = t.conv2_w_off / kq_elems(KqFmt.q8), rows = rail_site_rows(rl, RailSite.conv2, t.n_ctx),
        groups = rail_site_groups(rl, RailSite.conv2, t.d_model, t.n_ctx))

//! the conv stem over one `tin`-frame mel chunk in `planes`: conv1 on the `fw16` slab into `stage1`, conv2 at the `conv2` site into `x`, positions added
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#whisper-chains")]
def tower_aud_stem(var rl : auto(R); t : AudioTower; tin : int64) {
    let tt = t.n_ctx
    let d = t.d_model
    let kdp = tower_aud_stem_kdp(t)
    let act = t.gelu_tanh ? BIAS_ACT_GELU_TANH : BIAS_ACT_GELU_ERF
    var pc_im1 = GkIm2col1dArgs(cin = uint(t.n_mel), tin = uint(tin), tout = uint(tin), k = 3u, stride = 1u, pad_l = 1u, dilation = 1u, kdim = uint(kdp),
        row_major = 0u, r0 = 0u, mp = uint(tin))
    enc_tts_im2col16_cls(rl, fixed_array(TowerBuf.planes, TowerBuf.col), pc_im1, tin * kdp)
    rail_pt(rl, RailProfRole.im2col)
    rail_mm16(rl, TowerBuf.fw16, TowerBuf.col, TowerBuf.stage1, kdp, d, tin, 0l, d)
    rail_pt(rl, RailProfRole.gemm_pw)
    var pc_b1 = GkBiasActArgs(d = uint(d), nelem = uint(tin * d), boff = 0u, act = act)
    enc_tower_bias_act_cls(rl, fixed_array(TowerBuf.stage1, TowerBuf.fnorm), pc_b1, rq_wgs(false, tin * d))
    rail_pt(rl, RailProfRole.bias)
    var pc_im2 = GkIm2col1dArgs(cin = uint(d), tin = uint(tin), tout = uint(tt), k = 3u, stride = 2u, pad_l = 1u, dilation = 1u, kdim = uint(3l * d), row_major = 1u,
        r0 = 0u, mp = uint(tt))
    enc_tts_im2col16_cls(rl, fixed_array(TowerBuf.stage1, TowerBuf.xh), pc_im2, tt * 3l * d)
    rail_pt(rl, RailProfRole.im2col)
    rail_lane_mm(rl, RailSite.conv2, 0l, TowerBuf.xh, TowerBuf.x, 3l * d, d, tt, t.conv2_w_off)
    rail_pt(rl, RailProfRole.gemm_pw)
    var pc_b2 = GkBiasActArgs(d = uint(d), nelem = uint(tt * d), boff = uint(d), act = act)
    enc_tower_bias_act_cls(rl, fixed_array(TowerBuf.x, TowerBuf.fnorm), pc_b2, rq_wgs(false, tt * d))
    rail_pt(rl, RailProfRole.bias)
    var pc_pos = GkBiasActArgs(d = uint(tt * d), nelem = uint(tt * d), boff = uint(2l * d), act = BIAS_ACT_NONE)
    enc_tower_bias_act_cls(rl, fixed_array(TowerBuf.x, TowerBuf.fnorm), pc_pos, rq_wgs(false, tt * d))
    rail_pt(rl, RailProfRole.post_add)
}

//! a tail GEMM's feed off the f32 rows in `src` at `site`, unclamped, and its mark
def private tower_ct_feed(var rl : auto(R); site : RailSite; src : TowerBuf; nelem : int64) {
    rail_site_feed(rl, site, src, nelem, -NO_CLAMP, NO_CLAMP)
    rail_pt(rl, RailProfRole.rq)
}

//! the chat projector tail off the `d`-wide rows in `x` into the soft tokens in `tail_proj`, its GEMMs at records `rid1` (`tail1`) and `rid1 + 1` (`tail2`)
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#whisper-chains")]
def tower_chat_tail(var rl : auto(R); ct : TowerChatTail; d, rid1 : int64) {
    let nb = rl.norms_base
    let uv = ct.kind == AudioProjKind.ultravox
    let n1 = ct.pool_rows * d
    var pc_ln = gk_ln_args(uint(d), uint(ct.pool_rows), ct.eps, uint(ct.lnp_w - nb), uint(ct.lnp_b - nb))
    if (uv) {
        enc_tower_ln_cls(rl, fixed_array(TowerBuf.x, TowerBuf.xb, TowerBuf.norms), pc_ln, ct.pool_rows)
        rail_pt(rl, RailProfRole.ln)
        var pc_rms = gk_rms_args(uint(ct.m1in), uint(ct.rows), ULTRAVOX_NORM_EPS, uint(ct.normpre - nb))
        enc_tower_rms_cls(rl, fixed_array(TowerBuf.xb, TowerBuf.xb, TowerBuf.norms), pc_rms, ct.rows)
        rail_pt(rl, RailProfRole.rms)
        tower_ct_feed(rl, RailSite.tail1, TowerBuf.xb, n1)
    } else {
        var pc_pool = GkPoolRowsArgs(width = uint(d), total = uint(n1))
        enc_tower_pool_rows_cls(rl, fixed_array(TowerBuf.x, TowerBuf.yo), pc_pool, (n1 + 255l) / 256l)
        rail_pt(rl, RailProfRole.pool)
        if (rail_mulmm(rl)) {
            enc_tower_ln_cls(rl, fixed_array(TowerBuf.yo, TowerBuf.xb, TowerBuf.norms), pc_ln, ct.pool_rows)
            rail_pt(rl, RailProfRole.ln)
            tower_ct_feed(rl, RailSite.tail1, TowerBuf.xb, n1)
        } else {
            enc_tower_ln16_cls(rl, fixed_array(TowerBuf.yo, TowerBuf.norms, TowerBuf.xh), pc_ln, ct.pool_rows)
            rail_pt(rl, RailProfRole.ln)
        }
    }
    let one = ct.m2in == 0l
    rail_lane_mm(rl, RailSite.tail1, rid1, TowerBuf.xb, one ? TowerBuf.tail_proj : TowerBuf.tail_hid, ct.m1in, ct.n1out, ct.rows, ct.w1)
    rail_pt(rl, RailProfRole.gemm_o)
    if (one) {
        rail_bias_pass(rl, TowerBuf.tail_proj, ct.n1out, ct.rows, ct.proj_b)
        return
    }
    let nh = ct.rows * ct.n1out
    let n2 = ct.rows * ct.m2in
    if (uv) {
        var pc_gate = GkGatedRowsArgs(d = uint(ct.m2in), nelem = uint(n2))
        enc_tower_swiglu_swapped_cls(rl, fixed_array(TowerBuf.tail_hid, TowerBuf.tail_gate), pc_gate, (n2 + 255l) / 256l)
        rail_pt(rl, RailProfRole.glu)
        var pc_rms = gk_rms_args(uint(ct.m2in), uint(ct.rows), ULTRAVOX_NORM_EPS, uint(ct.normmid - nb))
        enc_tower_rms_cls(rl, fixed_array(TowerBuf.tail_gate, TowerBuf.tail_gate, TowerBuf.norms), pc_rms, ct.rows)
        rail_pt(rl, RailProfRole.rms)
        tower_ct_feed(rl, RailSite.tail2, TowerBuf.tail_gate, n2)
    } else {
        var pc_act = GkBiasActArgs(d = uint(ct.n1out), nelem = uint(nh), boff = 0u, act = BIAS_ACT_GELU_ERF)
        enc_tower_act_cls(rl, fixed_array(TowerBuf.tail_hid), pc_act, rq_wgs(false, nh))
        rail_pt(rl, RailProfRole.bias)
        tower_ct_feed(rl, RailSite.tail2, TowerBuf.tail_hid, n2)
    }
    rail_lane_mm(rl, RailSite.tail2, rid1 + 1l, uv ? TowerBuf.tail_gate : TowerBuf.tail_hid, TowerBuf.tail_proj, ct.m2in, ct.out, ct.rows, ct.w2)
    rail_pt(rl, RailProfRole.gemm_o)
}

//! the whisper-class blocks over `offs` from `x` at `npos` rows: the pre-LN loop, the post-norm into `xb` under `ln_post`, the tail behind under `ct.on`
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#whisper-chains")]
def tower_aud_blocks(var rl : auto(R); t : AudioTower; offs : array<LayerOffs>; ct : TowerChatTail; ln_post : bool; npos : int64) {
    let d = t.d_model
    let act = t.gelu_tanh ? BIAS_ACT_GELU_TANH : BIAS_ACT_GELU_ERF
    tower_ln_chain(rl, offs, t.has_k_bias, act, t.eps, d, t.n_ff, d / t.n_head, 64l, t.n_head, npos, ln_post ? t.lnp_w_off : -1l, ln_post ? t.lnp_b_off : -1l)
    if (ct.on) {
        tower_chat_tail(rl, ct, d, long_length(offs) * LN_BLOCK_GEMMS)
    }
}
