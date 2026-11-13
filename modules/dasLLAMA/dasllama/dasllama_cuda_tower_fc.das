options gen2
options indenting = 4
options _dasllama_internal = true

//! The CUDA home's FastConformer seats - canary's front, blocks and projector, parakeet's and GigaAM's whole encodes, GigaAM's
//! device mel - each the shared body (`dasllama_tower_chains_fc`) on the CUDA rail off the CUDA-flavored image's device block, on
//! the lanes the Vulkan seats serve: the q8 and f16 images of every family, and GigaAM's kq lane. The block chain's residual rows
//! stay on the device for the transducer and the CTC head (`cuda_tower_fc_rows`). Installed by `dasllama_cuda_tower_audio`'s arms.
module dasllama_cuda_tower_fc shared public

require dasllama/dasllama_lint public
require math
require daslib/math_boost   // round_up: the front's column panels
require dasllama/dasllama_env   // g_env_vulkan.vk_gigaam_exact: the lane knob's env twin both homes read
require dasllama/dasllama_kqformat
require dasllama/dasllama_math   // ensure_length
require dasllama/dasllama_plane
require dasllama/dasllama_asr_types   // CanaryLayerOffs
require dasllama/dasllama_tower
require dasllama/dasllama_gpu_math
require dasllama/dasllama_gpu_kernels_common
require dasllama/dasllama_gpu_kernels_discrete
require dasllama/dasllama_cuda_common
require dasllama/dasllama_cuda_classes
require dasllama/dasllama_cuda_classes_front
require dasllama/dasllama_cuda_classes_blocks
require dasllama/dasllama_cuda_rail
require dasllama/dasllama_cuda_tower_seat
require dasllama/dasllama_canary
require dasllama/dasllama_parakeet
require dasllama/dasllama_gigaam
require dasllama/dasllama_tower_chains_fc


//! the FastConformer chains' planes the shared resident does not carry, by role: the rel quartet and canary's rel plane, the
//! f16 taps, the fronts' stage planes, slabs and rows
let private CUF_ROLES = fixed_array(TowerBuf.rel_tab, TowerBuf.rel_tabh, TowerBuf.rel_proj, TowerBuf.rel_qh, TowerBuf.rel_ph, TowerBuf.rel_r,
    TowerBuf.taps, TowerBuf.frames, TowerBuf.spec, TowerBuf.mel, TowerBuf.stage2, TowerBuf.stage3, TowerBuf.stage4, TowerBuf.stage5,
    TowerBuf.feat, TowerBuf.feat16, TowerBuf.rows_out, TowerBuf.twiddle, TowerBuf.filters)

//! a FastConformer family's residency on this home: the shared seat record, the family's own planes by role (`CUF_ROLES`), the
//! kq lane's member pairs and seats, and the front's own schedule
struct private CufResident {
    t : CutResident
    p : CutRolePlanes
    kq_q : CutBuf[TOWER_KQ_MEMBERS]
    kq_s : CutBuf[TOWER_KQ_MEMBERS]
    @scratch kq_seats : array<TowerFcSeat>
    fsched : CutBuf
}

var private g_cuf_cn = CufResident(t = CutResident(family = "canary"))
var private g_cuf_pk = CufResident(t = CutResident(family = "parakeet"))
var private g_cuf_ga = CufResident(t = CutResident(family = "gigaam"))
var private @scratch @exact_size g_cuf_offs : array<CanaryLayerOffs>   //! the encode's block offsets, the family's record mapped onto canary's
var private @scratch @exact_size g_cuf_gseats : array<TowerGemmSeat>   //! a CPU image's kq-lane record seats (the bake reads them)
var private @scratch @exact_size g_cuf_fsched : array<uint>   //! the front's schedule staging
var private g_cuf_no_rows : array<float>   //! the empty rope tables of a rel-position family
var private g_cuf_rows : TowerRowsHandoff   //! the FastConformer handoff: the served chain's residual rows still on the device
var private g_cuf_front_rows : CutBuf   //! a front's rows left on the device for the blocks behind it in the same encode (`bytes` 0: none)
var private g_cuf_front_handoff = true   //! the front hands its rows to the blocks on the device (`set_cuda_fc_front_handoff`)

//! the generation of the FastConformer rows the last chain left on the device, 0 where none: a session records it with its rows
[hot_path]
def cuda_tower_fc_gen : uint64 => rows_handoff_gen(g_cuf_rows)

//! the device plane of generation `gen`'s FastConformer rows (`bytes` long) where no seat has run since, else 0: the decoders read it device to device
def cuda_tower_fc_rows(gen : uint64; bytes : int64) : uint64 => rows_handoff_dev(g_cuf_rows, gen, bytes)

//! Test A/B seat: a parakeet / GigaAM front hands its rows to the blocks on the device (`on`), or reads them back for the blocks
//! to upload; answers the setting it replaced
def set_cuda_fc_front_handoff(on : bool) : bool {
    let was = g_cuf_front_handoff
    g_cuf_front_handoff = on
    if (on != was) {
        to_log(LOG_INFO, "dasLLAMA cuda tower: set_cuda_fc_front_handoff({on}) - the FastConformer front's rows reach the blocks {on ? "on the device" : "through the host"}\n")
    }
    return was
}

def cuda_fc_front_handoff_on : bool => g_cuf_front_handoff

[cold_path]
def private cuf_release(var r : CufResident) {
    cut_release(r.t)
    cp_release(r.p, CUF_ROLES)
    cut_free(r.fsched)
    for (m in range(TOWER_KQ_MEMBERS)) {
        cut_free(r.kq_q[m])
        cut_free(r.kq_s[m])
    }
    let family = r.t.family
    delete r
    r <- CufResident(t = CutResident(family = family))
}

[cold_path]
def private cuf_release_all {
    cuf_release(g_cuf_cn)
    cuf_release(g_cuf_pk)
    cuf_release(g_cuf_ga)
    g_cuf_rows.bytes = 0l
}

//! a CUDA-flavored FastConformer image's resident key: its device block and f32 rows, the exact lane's halfwords, the f16 taps where read
def private cuf_key(fc : TowerFc) : uint64 {
    var k = cut_key_dev(fc.dev, fc.devplan, fc.blob, fc.n_layer)
    k = hash_combine64(hash_combine64(k, uint64(fc.w_off)), uint64(fc.w_n))
    if (fc.w_n > 0l) {
        k = hash_combine64(k, intptr(unsafe(reinterpret<void?>(fc.wblob.p))))
        k = tower_fold_words(k, unsafe(reinterpret<uint8 const?>(plane_at(fc.wblob, fc.w_off))), fc.w_n * 2l)
    }
    k = hash_combine64(hash_combine64(k, uint64(fc.taps_off)), uint64(fc.taps_n))
    if (fc.taps_n > 0l) {
        k = hash_combine64(k, intptr(unsafe(reinterpret<void?>(fc.taps.p))))
        k = tower_fold_words(k, unsafe(reinterpret<uint8 const?>(plane_at(fc.taps, fc.taps_off))), fc.taps_n * 2l)
    }
    return k
}

//! the chain's planes for the image the key names: as they are where it did not move, else uploaded (`cuf_upload`)
def private cuf_planes(var r : CufResident; fc : TowerFc) : bool {
    let key = cuf_key(fc)
    return r.t.key == key || cuf_upload(r, fc, key)
}

//! the chain's planes off a CUDA image's device block (`#tower-dev-block`) under `key`, each as stored; false with the resident released on a refused allocation
[cold_path, arch(at = "../ARCHITECTURE_IMAGE_GPU_PLANES.md#tower-dev-block")]
def private cuf_upload(var r : CufResident; fc : TowerFc; key : uint64) : bool {
    cuf_release(r)
    return false if (!cut_upload_planes(r.t, key, fc.blob, PlaneU16(), 0l))
    if (fc.f16w) {
        cut_up(r.t, r.t.wb, unsafe(reinterpret<void?>(plane_at(fc.wblob, fc.w_off))), fc.w_n * 2l)
    }
    if (tower_dev_find(fc.devplan, tower_fc_piece(CUT_HOME, "wq")).off >= 0l) {
        cut_put_piece(r.t, r.t.wq, fc, tower_fc_piece(CUT_HOME, "wq"))
        cut_put_piece(r.t, r.t.ws, fc, tower_fc_piece(CUT_HOME, "ws"))
    }
    tower_fc_dev_seats(fc, CUT_HOME, r.kq_seats)
    var m = 0
    for (fmt in range(KQ_FMT_COUNT)) {
        continue if (!fc.kq_lane || (fc.kq_fmts & (1u << uint(fmt))) == 0u || m >= TOWER_KQ_MEMBERS)
        cut_put_piece(r.t, r.kq_q[m], fc, tower_fc_kq_piece(CUT_HOME, fmt, false))
        cut_put_piece(r.t, r.kq_s[m], fc, tower_fc_kq_piece(CUT_HOME, fmt, true))
        m++
    }
    if (fc.taps_n > 0l) {
        cp_up(r.t, r.p, TowerBuf.taps, unsafe(reinterpret<void?>(plane_at(fc.taps, fc.taps_off))), fc.taps_n * 2l)
    }
    if (r.t.oom) {
        cuf_release(r)
        return false
    }
    r.t.key = key
    return true
}

//! the blocks' scratch at `npos` rows: the shared block planes, the rotary form's rope tables, the rel quartet and canary's rel plane
def private cuf_scratch(var r : CufResident; fc : TowerFc; npos : int64) : bool {
    let hs = tower_fc_hs(fc.d, fc.n_head)
    let dp = fc.rotary ? fc.n_head * tower_fc_hsp(hs) : 0l
    return false if (!cut_scratch(r.t, npos, fc.d, fc.ff, dp, 0l))
    let cap = r.t.cap_rows
    if (fc.rotary) {
        cut_grow(r.t, r.t.tabs, cap * hs * 4l)
    } else {
        let rel = 2l * cap
        let ww = 2l * cap - 1l
        cp_grow(r.t, r.p, TowerBuf.rel_tab, rel * fc.d * 4l)
        cp_grow(r.t, r.p, TowerBuf.rel_tabh, (rel + TILE_READ_SLACK) * fc.d * 2l)
        cp_grow(r.t, r.p, TowerBuf.rel_proj, rel * fc.d * 4l)
        cp_grow(r.t, r.p, TowerBuf.rel_qh, (cap + TILE_READ_SLACK) * 128l * 2l)
        cp_grow(r.t, r.p, TowerBuf.rel_ph, (ww + TILE_READ_SLACK) * 128l * 2l)
        cp_grow(r.t, r.p, TowerBuf.rel_r, cap * ww * 4l)
    }
    return !r.t.oom || cut_decline(r.t, TowerDecline.memory)
}

//! the family's planes beside the shared seats on the rail
def private cuf_rail_seats(var rl : CutRail; r : CufResident) {
    cut_rail_seats(rl, r.t)
    cp_seats(rl, r.p, CUF_ROLES)
    for (m in range(TOWER_KQ_MEMBERS)) {
        cut_seat(rl, TOWER_KQ_Q[m], r.kq_q[m])
        cut_seat(rl, TOWER_KQ_S[m], r.kq_s[m])
    }
}

//! every class the block chain dispatches at the padded head width `hsp`, the q8 tiles aside
def private cuf_blocks_ensure(rotary : bool; hsp : int64) : bool {
    let rel_ok = rotary || (ensure_tower_rel_sincos_cls() && ensure_tower_cn_attn_r_cls() && ensure_tower_cn_attn_r64_cls())
    let rot_ok = !rotary || (ensure_tower_rope_tab_cls() && ensure_tower_ga_dw_cls() && ensure_tower_ga_dwh_cls() && cut_attn_ensure(hsp))
    return (rel_ok && rot_ok && ensure_tower_ln_cls() && ensure_tower_clamp_cvt_cls() && ensure_tower_bias_act_cls() && ensure_tower_act_cls()
        && ensure_tower_post_add_ln_cls() && ensure_tower_post_add_ln_nb_cls() && ensure_tower_glu_sig_cls() && ensure_tower_cn_dw_cls()
        && ensure_tower_pk_dw_cls() && ensure_tower_hpad16_cls() && ensure_tower_hpad16_b_cls() && ensure_f16_gemm_cm2_w_cls())
}

//! site `site`'s pick for a [rows x width] output, every tile it decodes built (each kq member's at the same column); false where one did not
def private cuf_site(var rl : CutRail; var r : CufResident; fc : TowerFc; site : RailSite; width, rows : int64; kq : bool) : bool {
    rl.sites[int(site)] = cut_pick(r.t, width, rows, width, false)
    let p = rl.sites[int(site)]
    return false if (!cut_pick_ensure(p))
    return true if (!kq || !fc.kq_lane)
    for (fmt in range(KQ_FMT_COUNT)) {
        continue if ((fc.kq_fmts & (1u << uint(fmt))) == 0u)
        return false if (!cud_cm2_ensure(fmt, p.tci))
    }
    return true
}

//! the block chain's seat on this home (`tower_fc_blocks`): the rows read back into `x` and left on the device for the decoders under a new generation
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#fc-chains")]
def private cuf_blocks(var r : CufResident; fc : TowerFc; var x : array<float>; npos, ww : int64;
                       rope_cos : array<float> = g_cuf_no_rows; rope_sin : array<float> = g_cuf_no_rows) : bool {
    g_cuf_rows.bytes = 0l
    let front = g_cuf_front_rows
    g_cuf_front_rows = CutBuf()
    let d = fc.d
    let ff = fc.ff
    let hs = tower_fc_hs(d, fc.n_head)
    if (!tower_fc_shape_ok(fc.rotary, d, ff, fc.n_head, fc.conv_kernel, npos, ww, min(long_length(rope_cos), long_length(rope_sin)))) {
        return cut_decline(r.t, TowerDecline.shape)
    }
    return false if (!cut_arm(r.t))
    if (!cuf_blocks_ensure(fc.rotary, tower_fc_hsp(hs))) {
        return cut_decline(r.t, TowerDecline.device)
    }
    if (!cuf_planes(r, fc)) {
        return cut_decline(r.t, TowerDecline.memory)
    }
    return false if (!cuf_scratch(r, fc, npos))
    var rl & = g_cut_rl
    rail_begin(rl, rail_lane_of(fc.f16w))
    rail_bind_rel(rl)
    if (!fc.f16w) {
        if (!cuf_site(rl, r, fc, RailSite.d, d, npos, true) || !cuf_site(rl, r, fc, RailSite.ff, ff, npos, true) || !cuf_site(rl, r, fc, RailSite.d2, 2l * d, npos, true)
                || (!fc.rotary && !cuf_site(rl, r, fc, RailSite.rel, d, ww, true))) {
            return cut_decline(r.t, fc.kq_lane ? TowerDecline.kq_lane : TowerDecline.device)
        }
        let nrec = fc.n_layer * FC_GEMM_RECORDS
        var map_words = 0l
        for (rid in range64(nrec)) {
            map_words += tower_fc_rec(rl, fc, g_cuf_offs, r.kq_seats, npos, ww, rid).groups
        }
        cut_sched_begin(nrec, map_words)
        tower_sched_walk(rl.maps, unsafe(addr(g_cut_sched[0])), nrec) $(rid : int64) : TowerSchedRec {
            return tower_fc_rec(rl, fc, g_cuf_offs, r.kq_seats, npos, ww, rid)
        }
        return false if (!cut_sched_up(r.t))
    }
    let bx = npos * d * 4l
    if (front.bytes == bx) {
        cuda_memcpy_dtod(r.t.x.dev, front.dev, uint64(bx), g_cud.stream)
    } else {
        cut_htod(r.t.x.dev, unsafe(addr<void?>(x[0])), bx)
    }
    if (fc.rotary) {
        let half = hs / 2l
        cut_htod(r.t.tabs.dev, unsafe(addr<void?>(rope_cos[0])), npos * half * 4l)
        cut_htod(r.t.tabs.dev + uint64(npos * half * 4l), unsafe(addr<void?>(rope_sin[0])), npos * half * 4l)
    }
    cuf_rail_seats(rl, r)
    cut_chain(rl, "fc.blocks", (npos << 32l) | ww) {
        tower_fc_blocks(rl, fc, g_cuf_offs, r.kq_seats, npos, ww)
    }
    let served = cut_served(r.t, r.t.x.dev, x, bx, npos, fc.n_layer)
    rows_handoff_land(g_cuf_rows, r.t.x.dev, bx)
    return served
}

//! canary's projector over the rows still in `x` on the chain's lane (the `tail1` tile or the f16 GEMM), its bias, the rows read back into `out`
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#fc-chains")]
def private cuf_proj(var r : CufResident; fc : TowerFc; npos : int64; @scratch @exact_size var out : array<float>) : bool {
    let pdim = fc.proj_dim
    return cut_decline(r.t, TowerDecline.shape) if (pdim > fc.ff)
    var rl & = g_cut_rl
    rail_begin(rl, rail_lane_of(fc.f16w))
    if (!fc.f16w) {
        if (!cuf_site(rl, r, fc, RailSite.tail1, pdim, npos, false)) {
            return cut_decline(r.t, TowerDecline.device)
        }
        let groups = rail_site_groups(rl, RailSite.tail1, pdim, npos)
        cut_sched_begin(1l, groups)
        tower_sched_walk(rl.maps, unsafe(addr(g_cut_sched[0])), 1l) $ [unused_argument(rid)] (rid : int64) : TowerSchedRec {
            return TowerSchedRec(wblk0 = fc.proj_w / kq_elems(KqFmt.q8), rows = npos, groups = groups)
        }
        return false if (!cut_sched_up(r.t))
    }
    cuf_rail_seats(rl, r)
    tower_fc_proj(rl, fc, npos)
    let bytes = npos * pdim * 4l
    out |> grow_resize(npos * pdim)   // the encode's rows exactly: a shorter clip's projected rows are fewer than the last one's
    cut_dtoh(unsafe(addr<void?>(out[0])), r.t.hg.dev, bytes)
    cut_sync(r.t.family)
    return true
}

//! the canary / parakeet front's slabs for the key, where its planes carry none yet (`r.t.front`): the f16 conv0 panel, canary's twiddles, the filters
[cold_path]
def private cuf_cn_slab(var r : CufResident; fc : TowerFc) : bool {
    cut_put_piece(r.t, r.t.fw16, fc, tower_fc_piece(CUT_HOME, "front16"))
    if (fc.device_mel) {
        cp_up(r.t, r.p, TowerBuf.twiddle, unsafe(reinterpret<void?>(plane_at(fc.twiddles, 0l))), fc.n_fft * 2l * fc.n_fb * 4l)
    }
    cp_up(r.t, r.p, TowerBuf.filters, unsafe(reinterpret<void?>(plane_at(fc.filters, 0l))), fc.n_mel * fc.n_fb * 4l)
    r.t.front = !r.t.oom
    return !r.t.oom
}

//! the canary / parakeet front's scratch at `n_len` frames
def private cuf_cn_scratch(var r : CufResident; fc : TowerFc; n_len : int64) : bool {
    let b = tower_fc_front_bytes(fc, n_len)
    cut_grow(r.t, r.t.planes, b.xw)
    cp_grow(r.t, r.p, TowerBuf.spec, b.pow)
    cp_grow(r.t, r.p, TowerBuf.mel, b.mel)
    cut_grow(r.t, r.t.col, b.col)
    cut_grow(r.t, r.t.stage1, b.y0)
    cp_grow(r.t, r.p, TowerBuf.stage2, b.y2)
    cp_grow(r.t, r.p, TowerBuf.stage3, b.y2)
    cp_grow(r.t, r.p, TowerBuf.stage4, b.y3)
    cp_grow(r.t, r.p, TowerBuf.stage5, b.y3)
    cp_grow(r.t, r.p, TowerBuf.feat, b.xf)
    cp_grow(r.t, r.p, TowerBuf.feat16, b.h)
    cp_grow(r.t, r.p, TowerBuf.rows_out, b.x)
    cp_grow(r.t, r.p, TowerBuf.twiddle, 64l)
    return !r.t.oom || cut_decline(r.t, TowerDecline.memory)
}

def private cuf_cn_front_ensure : bool
    => (ensure_tower_im2col_cls() && ensure_tower_spec_dft_cls() && ensure_tower_mel_log_cls() && ensure_tower_mel_norm_cls() && ensure_tower_dw_conv_cls()
        && ensure_tower_pk_dw2d_cls() && ensure_tower_feat_shuffle_cls() && ensure_tower_bias_act_cls() && ensure_tower_clamp_cvt_cls() && ensure_f16_gemm_cm2_w_cls())

//! the q8 lane's front sites at `n_len` frames - each pick's tile built - and their schedule up; false with the decline counted
def private cuf_front_sched(var rl : CutRail; var r : CufResident; fc : TowerFc; n_len : int64) : bool {
    let g = tower_fc_geom(n_len, fc.n_mel)
    if (!cuf_site(rl, r, fc, RailSite.front1, fc.nch, g.t2 * g.w2, true) || !cuf_site(rl, r, fc, RailSite.front2, fc.nch, g.t3 * g.w3, true)
            || !cuf_site(rl, r, fc, RailSite.front3, fc.d, g.t3, true)) {
        return cut_decline(r.t, fc.kq_lane ? TowerDecline.kq_lane : TowerDecline.device)
    }
    var map_words = 0l
    for (rid in range64(FC_FRONT_RECORDS)) {
        map_words += tower_fc_front_rec(rl, fc, r.kq_seats, n_len, rid).groups
    }
    g_cuf_fsched |> grow_resize(FC_FRONT_RECORDS * 4l + map_words)
    tower_sched_walk(rl.maps, unsafe(addr(g_cuf_fsched[0])), FC_FRONT_RECORDS) $(rid : int64) : TowerSchedRec {
        return tower_fc_front_rec(rl, fc, r.kq_seats, n_len, rid)
    }
    cut_up(r.t, r.fsched, unsafe(addr<void?>(g_cuf_fsched[0])), long_length(g_cuf_fsched) * 4l)
    return !r.t.oom || cut_decline(r.t, TowerDecline.memory)
}

//! a front's `bytes` of rows in `rows_out` handed to the blocks behind it in the same encode: left on the device under the
//! handoff (`set_cuda_fc_front_handoff`), else read back into `x` for the blocks to upload
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#fc-chains")]
def private cuf_front_rows_stay(r : CufResident; var x : array<float>; bytes : int64; who : string) {
    if (g_cuf_front_handoff) {
        g_cuf_front_rows = CutBuf(dev = cp_dev(r.p, TowerBuf.rows_out), bytes = bytes)
    } else {
        cut_dtoh(unsafe(addr<void?>(x[0])), cp_dev(r.p, TowerBuf.rows_out), bytes)
        cut_sync(who)
    }
}

//! the canary / parakeet front's seat (`tower_fc_front`) off `src` into `x`: the frame count, or -1 counted - uncounted where the blocks would not admit
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#fc-chains")]
def private cuf_fc_front(var r : CufResident; fc : TowerFc; src : float const?; src_elems, n_len, n_valid : int64; var x : array<float>; blocks_behind : bool) : int64 {
    let nch = fc.nch
    let d = fc.d
    let g = tower_fc_geom(n_len, fc.n_mel)
    let tt = g.t3
    let feat = g.w3 * nch
    if (n_len < 1l || nch % 64l != 0l || feat % 64l != 0l || d % 64l != 0l || fc.n_fb != fc.n_fft / 2l + 1l
            || src_elems < n_len * (fc.device_mel ? fc.n_fft : fc.n_mel)) {
        cut_decline(r.t, TowerDecline.shape)
        return -1l
    }
    return -1l if (!cut_arm(r.t))
    if (!cuf_cn_front_ensure() || (blocks_behind && !cuf_blocks_ensure(fc.rotary, 64l))) {
        cut_decline(r.t, TowerDecline.device)
        return -1l
    }
    if (blocks_behind && !tower_fc_shape_ok(fc.rotary, d, fc.ff, fc.n_head, fc.conv_kernel, tt, 2l * tt - 1l, 0l)) {
        return -1l
    }
    if (!cuf_planes(r, fc) || (!r.t.front && !cuf_cn_slab(r, fc))) {
        cut_decline(r.t, TowerDecline.memory)
        return -1l
    }
    return -1l if (!cuf_cn_scratch(r, fc, n_len))
    var rl & = g_cut_rl
    rail_begin(rl, rail_lane_of(fc.f16w))
    tower_fc_front_binds(rl)
    if (!fc.f16w && !cuf_front_sched(rl, r, fc, n_len)) {
        return -1l
    }
    cut_htod(fc.device_mel ? r.t.planes.dev : cp_dev(r.p, TowerBuf.mel), unsafe(reinterpret<void?>(src)), n_len * (fc.device_mel ? fc.n_fft : fc.n_mel) * 4l)
    cuf_rail_seats(rl, r)
    cut_seat(rl, TowerBuf.meta, r.fsched)
    tower_fc_front(rl, fc, r.kq_seats, n_len, n_valid)
    let bx = tt * d * 4l
    ensure_length(x, tt * d)
    if (blocks_behind) {
        cuf_front_rows_stay(r, x, bx, fc.front_name)
    } else {
        cut_dtoh(unsafe(addr<void?>(x[0])), cp_dev(r.p, TowerBuf.rows_out), bx)
        cut_sync(fc.front_name)
    }
    return tt
}

[hot_path]
def private cuda_canary_blocks(t : CanaryEncoder; var s : CanaryState; npos, ww : int64) : bool {
    var r & = g_cuf_cn
    return false if (!cut_gate(r.t, true, t.dev_backend))
    let fc = tower_fc_of_cn(t, g_cuf_offs)
    return cuf_blocks(r, fc, s.x, npos, ww) && cuf_proj(r, fc, npos, s.out)
}

[hot_path]
def private cuda_canary_front(t : CanaryEncoder; var s : CanaryState) : int64 {
    var r & = g_cuf_cn
    return -1l if (!cut_gate(r.t, true, t.dev_backend))
    let fc = tower_fc_of_cn(t, g_cuf_offs)
    let tt = cuf_fc_front(r, fc, unsafe(addr<float const?>(s.xw[0])), long_length(s.xw), s.n_len, s.n_valid, s.x, false)
    if (tt >= 0l) {
        cut_count_stem()
    }
    return tt
}

//! the parakeet encode whole (the front off the CPU mel, then the blocks); -1 uncounted where the front cannot serve or the blocks declined behind it
[hot_path]
def private cuda_parakeet_encode(m : ParakeetModel; var st : ParakeetState) : int64 {
    var r & = g_cuf_pk
    g_cuf_rows.bytes = 0l
    return -1l if (!cut_gate(r.t, true, m.dev_backend))
    if (tower_fc_geom(st.mel.n_len, m.n_mel).t3 > parakeet_max_frames()) {
        cut_decline(r.t, TowerDecline.shape)
        return -1l
    }
    let fc = tower_fc_of_pk(m, CUT_HOME, g_cuf_offs, g_cuf_gseats)
    let tt = cuf_fc_front(r, fc, unsafe(addr<float const?>(st.mel.data[0])), long_length(st.mel.data), st.mel.n_len, st.mel.n_len, st.x, true)
    let first = !r.t.served_said   //! read past the front, whose attach re-keys the resident
    if (tt < 0l || !cuf_blocks(r, fc, st.x, tt, 2l * tt - 1l)) {
        return -1l
    }
    tower_fc_cpu_mel_say("cuda", r.t.family) if (first && !fc.device_mel)
    cut_count_stem()
    return tt
}

//! GigaAM's front slabs for the key, where its planes carry none yet (`r.t.front`): the f16 conv slab, the device mel's twiddles and window (one plane), the filters
[cold_path]
def private cuf_ga_slab(var r : CufResident; m : GigaamModel; fc : TowerFc) : bool {
    cut_put_piece(r.t, r.t.fw16, fc, tower_fc_piece(CUT_HOME, "front16"))
    cut_put_piece(r.t, r.p.b[int(TowerBuf.twiddle)], fc, tower_ga_twin_piece(CUT_HOME))
    cp_up(r.t, r.p, TowerBuf.filters, unsafe(reinterpret<void?>(plane_at(fc.filters, 0l))), m.n_mel * (m.n_fft / 2l + 1l) * 4l)
    r.t.front = !r.t.oom
    return !r.t.oom
}

//! GigaAM's front scratch at `n_len` mel frames: samples, frames, power and mel rows, one shared column panel, conv0's rows, the residual rows
def private cuf_ga_scratch(var r : CufResident; m : GigaamModel; n_len : int64) : bool {
    let b = tower_ga_front_bytes(m, n_len)
    cut_grow(r.t, r.t.planes, b.samples)
    cp_grow(r.t, r.p, TowerBuf.frames, b.frames)
    cp_grow(r.t, r.p, TowerBuf.spec, b.pow)
    cp_grow(r.t, r.p, TowerBuf.mel, b.mel)
    cut_grow(r.t, r.t.col, b.col)
    cut_grow(r.t, r.t.stage1, b.y1)
    cp_grow(r.t, r.p, TowerBuf.rows_out, b.x)
    return !r.t.oom || cut_decline(r.t, TowerDecline.memory)
}

def private cuf_ga_front_ensure : bool
    => (ensure_tts_im2col16_cls() && ensure_tower_bias_act_cls() && ensure_tower_ga_frames_cls() && ensure_tower_spec_dft_cls() && ensure_tower_mel_log_cls()
        && ensure_f16_gemm_cm2_w_cls())

//! GigaAM's resident armed for `n_len` frames (device, classes, planes, slabs, scratch, the rail seated); false with the decline counted
def private cuf_ga_arm(var r : CufResident; fc : TowerFc; m : GigaamModel; n_len : int64) : bool {
    return false if (!cut_arm(r.t))
    return cut_decline(r.t, TowerDecline.device) if (!cuf_ga_front_ensure())
    return cut_decline(r.t, TowerDecline.memory) if (!cuf_planes(r, fc) || (!r.t.front && !cuf_ga_slab(r, m, fc)))
    return false if (!cuf_ga_scratch(r, m, n_len))
    var rl & = g_cut_rl
    rail_begin(rl, RailLane.q8)
    cuf_rail_seats(rl, r)
    return true
}

//! The gigaam log-mel of `samples` on the device alone (`tower_ga_mel`), frame-major into `mel`: the frame count, or -1 with the decline counted
def cuda_gigaam_mel(m : GigaamModel; samples : array<float>; @exact_size var mel : array<float>) : int64 {
    var r & = g_cuf_ga
    return -1l if (!cut_gate(r.t, gigaam_gpu_lane_planes(m), m.dev_backend))
    let n_len = gigaam_mel_frames(m, long_length(samples))
    if (n_len < 1l || m.hop < 1l) {
        cut_decline(r.t, TowerDecline.shape)
        return -1l
    }
    let fc = tower_fc_of_ga(m, CUT_HOME, g_cuf_offs, g_cuf_gseats)
    return -1l if (!cuf_ga_arm(r, fc, m, n_len))
    cut_htod(r.t.planes.dev, unsafe(addr<void?>(samples[0])), ((n_len - 1l) * m.hop + m.n_fft) * 4l)
    tower_ga_mel(g_cut_rl, m, n_len)
    mel |> reserve_resize(n_len * m.n_mel)
    cut_dtoh(unsafe(addr<void?>(mel[0])), cp_dev(r.p, TowerBuf.mel), n_len * m.n_mel * 4l)
    cut_sync("gigaam-mel")
    cut_count_mel()
    return n_len
}

//! GigaAM's conv front (`tower_ga_front`) off the device or the CPU mel into `st.x`: the frame count, or -1 - uncounted where the blocks would not admit
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#fc-chains")]
def private cuf_ga_front(var r : CufResident; fc : TowerFc; m : GigaamModel; var st : GigaamState; samples : float const?; n_in : int64) : int64 {
    let k = m.subs_kernel
    let d = m.d_model
    let n_len = st.n_len
    let t1 = gigaam_conv_out(n_len, k)
    let tt = gigaam_conv_out(t1, k)
    let dev_mel = samples != null
    let n_use = (n_len - 1l) * m.hop + m.n_fft
    if (n_len < 1l || t1 < 1l || tt < 1l || k % 2l != 1l || m.n_sub_ch % 64l != 0l || d % 64l != 0l
            || (dev_mel ? (m.hop < 1l || n_use > n_in) : long_length(st.mel) < n_len * m.n_mel)) {
        cut_decline(r.t, TowerDecline.shape)
        return -1l
    }
    let rope_n = tt * (tower_fc_hs(fc.d, fc.n_head) / 2l)
    return -1l if (!tower_fc_shape_ok(true, fc.d, fc.ff, fc.n_head, fc.conv_kernel, tt, 0l, rope_n) || !cuf_ga_arm(r, fc, m, n_len))
    if (dev_mel) {
        cut_htod(r.t.planes.dev, unsafe(reinterpret<void?>(samples)), n_use * 4l)
    } else {
        cut_htod(cp_dev(r.p, TowerBuf.mel), unsafe(addr<void?>(st.mel[0])), n_len * m.n_mel * 4l)
    }
    tower_ga_front(g_cut_rl, m, n_len, dev_mel)
    ensure_length(st.x, tt * d)
    cuf_front_rows_stay(r, st.x, tt * d * 4l, fc.front_name)
    return tt
}

//! the GigaAM encode whole (the front, the rope tables, the blocks); -1 uncounted where the front cannot serve or the blocks declined behind it
[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#fc-chains")]
def private cuda_gigaam_encode(m : GigaamModel; var st : GigaamState; samples : float const?; n_in : int64) : int64 {
    var r & = g_cuf_ga
    g_cuf_rows.bytes = 0l
    return -1l if (!cut_gate(r.t, gigaam_gpu_lane_planes(m), m.dev_backend))
    let fc = tower_fc_of_ga(m, CUT_HOME, g_cuf_offs, g_cuf_gseats)
    let tt = cuf_ga_front(r, fc, m, st, samples, n_in)
    return -1l if (tt < 0l)
    gigaam_rope_tables(m, st, tt)
    return -1l if (!cuf_blocks(r, fc, st.x, tt, 0l, st.rope_cos, st.rope_sin))
    if (samples != null) {
        cut_count_mel()
    }
    cut_count_stem()
    return tt
}

var private g_cuf_ga_exact = false

//! Prefer GigaAM's exact lane for the next un-pinned loads (this home's `set_vulkan_gigaam_exact_lane`); answers the setting it replaced
def set_cuda_gigaam_exact_lane(on : bool) : bool {
    let was = g_cuf_ga_exact
    g_cuf_ga_exact = on
    register_gigaam_gpu(@@cuda_tower_on, on)
    return was
}

//! the canary / parakeet chain serves the `q8` lane's image here, else the exact pick's f16 one: the knob, the device, q8 tiles at every column, the classes
def private cuda_fc_serves_model(q8 : bool) : bool
    => cut_lane_arms("canary / parakeet", q8) && cut_flavor_classes("canary / parakeet", cuf_blocks_ensure(false, 64l) && cuf_cn_front_ensure())

//! parakeet's lanes (`e`, the lane's meta) on this chain: q8, the exact pick's f16 image, and the kq lane on the tiles of the f16 feed
def private cuda_parakeet_serves_model(e : ParakeetModel) : bool => cuda_fc_serves_model(e.q8)

//! GigaAM's chain serves a carrier on its lane: the knob, the device, the chain's and the front's classes, the block GEMM planes it reads
def private cuda_gigaam_serves_model(m : GigaamModel) : bool
    => (gigaam_gpu_lane_planes(m) && cut_lane_arms("gigaam", m.q8)
        && cut_flavor_classes("gigaam", cuf_blocks_ensure(true, 64l) && cuf_ga_front_ensure()))

[cold_path, arch(at = "../ARCHITECTURE_IMAGE_GPU_PLANES.md#tower-dev-block")]
def private cuda_canary_bake(m : CanaryEncoder; var b : TowerDevBuild) : bool {
    let fc = tower_fc_of_cn(m, g_cuf_offs)
    tower_fc_bake(fc, g_cuf_offs, g_cuf_gseats, b, CUT_HOME)
    tower_cn_front_bake(fc, b, CUT_HOME)
    return true
}

[cold_path, arch(at = "../ARCHITECTURE_IMAGE_GPU_PLANES.md#tower-dev-block")]
def private cuda_parakeet_bake(m : ParakeetModel; var b : TowerDevBuild) : bool {
    let fc = tower_fc_of_pk(m, CUT_HOME, g_cuf_offs, g_cuf_gseats)
    tower_fc_bake(fc, g_cuf_offs, g_cuf_gseats, b, CUT_HOME)
    tower_cn_front_bake(fc, b, CUT_HOME)
    return true
}

[cold_path, arch(at = "../ARCHITECTURE_IMAGE_GPU_PLANES.md#tower-dev-block")]
def private cuda_gigaam_bake(m : GigaamModel; var b : TowerDevBuild) : bool {
    tower_fc_bake(tower_fc_of_ga(m, CUT_HOME, g_cuf_offs, g_cuf_gseats), g_cuf_offs, g_cuf_gseats, b, CUT_HOME)
    return tower_ga_front_bake(m, b, CUT_HOME)
}

//! the FastConformer families' tower seats on this home - their hooks and flavors: canary's, parakeet's and GigaAM's
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#rail-audio-installs")]
def cuda_tower_install_fc(fam : TowerAudioFamily) {
    if (fam == TowerAudioFamily.canary) {
        register_canary_gpu(@@cuda_canary_blocks)
        register_canary_front_gpu(@@cuda_canary_front)
        register_canary_flavor(TowerDev.cuda, @@cuda_fc_serves_model, @@cuda_canary_bake)
    } elif (fam == TowerAudioFamily.parakeet) {
        register_parakeet_gpu_encode(@@cuda_parakeet_encode)
        register_parakeet_flavor(TowerDev.cuda, @@cuda_parakeet_serves_model, @@cuda_parakeet_bake)
    } elif (fam == TowerAudioFamily.gigaam) {
        if (g_env_vulkan.vk_gigaam_exact) {
            to_log(LOG_INFO, "dasLLAMA cuda tower: DASLLAMA_VK_GIGAAM_EXACT=1 reaches this home too - GigaAM's un-pinned loads take the exact lane on CUDA (unset it for the q8 lane)\n")
        }
        set_cuda_gigaam_exact_lane(g_env_vulkan.vk_gigaam_exact)
        register_gigaam_gpu_encode(@@cuda_gigaam_encode)
        register_gigaam_flavor(TowerDev.cuda, @@cuda_gigaam_serves_model, @@cuda_gigaam_bake)
    } elif (fam == TowerAudioFamily.transducer) {
        register_tower_rows_gen(@@cuda_tower_fc_gen)
    }
}

[init]
def dasllama_cuda_tower_fc_register {
    cut_on_release(@@cuf_release_all)
}
