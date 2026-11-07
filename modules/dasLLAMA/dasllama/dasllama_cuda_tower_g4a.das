options gen2
options indenting = 4
options _dasllama_internal = true

//! The CUDA home's gemma4a, qwen3a-front and silero VAD seats and the device log-mel (whisper's, qwen3a's): each the shared body
//! (`dasllama_tower_chains_g4a`) on the CUDA rail off the CUDA-flavored image's device block. gemma4a's front lands its rows in the
//! blocks' residual plane on the device, so a chunk is one stream's launches; qwen3a's front and projector meet the whisper-class
//! blocks seat through the host rows the hooks hand over. Installed by `dasllama_cuda_tower_audio`'s family arms.
module dasllama_cuda_tower_g4a shared public

require dasllama/dasllama_lint public
require math
require dasllama/dasllama_kqformat
require dasllama/dasllama_math   // ensure_length
require dasllama/dasllama_plane
require dasllama/dasllama_tower
require dasllama/dasllama_gpu_tier   // TILE_READ_SLACK
require dasllama/dasllama_gpu_math
require dasllama/dasllama_gpu_kernels_common
require dasllama/dasllama_gpu_kernels_discrete
require dasllama/dasllama_cuda_common
require dasllama/dasllama_cuda_classes
require dasllama/dasllama_cuda_classes_front
require dasllama/dasllama_cuda_classes_blocks
require dasllama/dasllama_cuda_classes_asr
require dasllama/dasllama_cuda_rail
require dasllama/dasllama_cuda_tower_seat
require dasllama/dasllama_cuda_tower_whisper   // cuda_audio_tower_bake_into: qwen3a's core blocks' pieces
require dasllama/dasllama_audio
require dasllama/dasllama_gemma4a
require dasllama/dasllama_qwen3a
require dasllama/dasllama_vad
require dasllama/dasllama_tower_chains_g4a

//! the planes this module's four seats (gemma4a, qwen3a's front, the device mel, the VAD) bind beside the shared resident's, by
//! role: the rel table's planes, the fronts' spectrum, mel, later stages, f16 feed and rows, the DFT twiddles and filters, the
//! VAD's gates, state and probabilities
let private CUT_SIDE_ROLES = fixed_array(TowerBuf.rel_tab, TowerBuf.rel_tabh, TowerBuf.rel_proj, TowerBuf.spec, TowerBuf.mel, TowerBuf.stage2,
    TowerBuf.stage3, TowerBuf.feat16, TowerBuf.rows_out, TowerBuf.twiddle, TowerBuf.filters, TowerBuf.vad_gates, TowerBuf.vad_state, TowerBuf.vad_probs)

var private g_cut_g4a = CutResident(family = "gemma4a")
var private g_cut_q3a = CutResident(family = "qwen3a-front")
var private g_cut_mel = CutResident(family = "device-mel")
var private g_cut_vad = CutResident(family = "silero-vad")
var private g_cp_g4a : CutRolePlanes
var private g_cp_q3a : CutRolePlanes
var private g_cp_mel : CutRolePlanes
var private g_cp_vad : CutRolePlanes
var private @scratch @exact_size g_cut_q3a_xin : array<float>   //! the window's chunk blocks [ci][ih][iw] staged for the upload
var private @scratch @exact_size g_cut_vad_x : array<float>     //! the VAD's staging: the stream's context, the samples, the zero tail
var private g_cut_vad_calls = 0l
var private g_cut_vad_chunks = 0l
var private g_cut_vad_faults : tuple<device : bool; memory : bool>

//! The VAD calls and chunks the device served.
def cuda_tower_vad_stats : tuple<calls : int64; chunks : int64> => (calls = g_cut_vad_calls, chunks = g_cut_vad_chunks)

//! Test seat: the VAD seat fails at a class build (`device`) or an allocation (`memory`) until pinned back; answers the pins it replaced.
def set_cuda_vad_faults_(device, memory : bool) : tuple<device : bool; memory : bool> {
    let was = g_cut_vad_faults
    g_cut_vad_faults = (device = device, memory = memory)
    return was
}

//! a resident and its side planes released together
[cold_path]
def private cut_side_release(var r : CutResident; var p : CutRolePlanes) {
    cp_release(p, CUT_SIDE_ROLES)
    cut_release(r)
}

[cold_path]
def private cut_g4a_release {
    cut_side_release(g_cut_g4a, g_cp_g4a)
    cut_side_release(g_cut_q3a, g_cp_q3a)
    cut_side_release(g_cut_mel, g_cp_mel)
    cut_side_release(g_cut_vad, g_cp_vad)
}

//! the rail at the shared resident's planes and the side planes beside them
def private cut_side_seats(var rl : CutRail; r : CutResident; p : CutRolePlanes) {
    cut_rail_seats(rl, r)
    cp_seats(rl, p, CUT_SIDE_ROLES)
}

//! a seat's planes refused: the resident and its side planes released, `memory` counted
[cold_path]
def private cut_side_oom(var r : CutResident; var p : CutRolePlanes) : bool {
    cut_side_release(r, p)
    return cut_decline(r, TowerDecline.memory)
}

//! a seat's launches waited on and its serves line said once, no encode counted (a front, a mel, the VAD)
def private cut_side_landed(var r : CutResident) {
    cut_sync(r.family)
    cut_announce(r) if (!r.served_said)
}

let private CUT_MEL_FLOOR_FRAMES = 4096l   //! the mel scratch's frames at least: a window's frames grow it once

//! the whisper preprocessors' log-mel on the device (`register_whisper_mel_gpu`): `n_len` frames into `n_mel` mel-major rows in `m.data`; false declined
[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#g4a-chains")]
def cuda_whisper_mel(xw : array<float>; n_len, n_fft, n_bins, n_mel : int64; twiddles, filters : array<float>; var m : WhisperMel) : bool {
    var r & = g_cut_mel
    var p & = g_cp_mel
    return cut_decline(r, TowerDecline.knob) if (!cuda_tower_on())
    if (n_len < 1l || n_bins != n_fft / 2l + 1l || long_length(xw) < n_len * n_fft || long_length(twiddles) < n_fft * 2l * n_bins
            || long_length(filters) < n_mel * n_bins || long_length(m.data) < n_mel * n_len) {
        return cut_decline(r, TowerDecline.shape)
    }
    return false if (!cut_arm(r))
    if (!ensure_tower_spec_dft_cls() || !ensure_tower_mel_log_cls()) {
        return cut_decline(r, TowerDecline.device)
    }
    let cap = max(n_len, CUT_MEL_FLOOR_FRAMES)
    cut_grow(r, r.planes, cap * n_fft * 4l)
    cp_grow(r, p, TowerBuf.spec, cap * n_bins * 4l)
    cp_grow(r, p, TowerBuf.mel, cap * n_mel * 4l)
    cp_up(r, p, TowerBuf.twiddle, unsafe(addr<void?>(twiddles[0])), n_fft * 2l * n_bins * 4l)
    cp_up(r, p, TowerBuf.filters, unsafe(addr<void?>(filters[0])), n_mel * n_bins * 4l)
    return cut_side_oom(r, p) if (r.oom)
    cut_htod(r.planes.dev, unsafe(addr<void?>(xw[0])), n_len * n_fft * 4l)
    var rl & = g_cut_rl
    rail_begin(rl, RailLane.q8)
    cut_side_seats(rl, r, p)
    tower_mel(rl, n_len, tower_whisper_mel_spec(n_fft, n_mel))
    cut_dtoh(unsafe(addr<void?>(m.data[0])), cp_dev(p, TowerBuf.mel), n_mel * n_len * 4l)
    cut_side_landed(r)
    cut_count_mel()
    return true
}

let private CUT_G4A_PIECE = tower_family_piece(CUT_HOME, "g4a")           //! gemma4a's q8 pieces
let private CUT_G4A_FRONT16_PIECE = tower_front16_piece(CUT_HOME, "g4a")  //! ... its subsample convs as the f16 slab
let private CUT_G4A_FRONT_FRAMES = 3008l              //! the front scratch's frames at least: a 30 s chunk's

[cold_path]
def private cuda_gemma4a_bake(t : Gemma4aEncoder; var b : TowerDevBuild) : bool {
    tower_g4a_bake(t, b, CUT_HOME)
    return true
}

//! every class the gemma4a chunk launches but the q8 tiles, at head width `hs` - the f16 GEMM of the convs and of the f16 lane among them
def private cut_g4a_ensure(hs : int64) : bool
    => (ensure_tower_spec_dft_cls() && ensure_tower_mel_log_cls() && ensure_tower_im2col_cls() && ensure_tower_ln_relu_cls() && ensure_f16_gemm_cm2_w_cls()
        && ensure_tower_clamp_cvt_cls() && ensure_tower_rms_cls() && ensure_tower_clamp_cls() && ensure_tower_post_add_cls() && ensure_tower_post_add_plain_cls()
        && ensure_tower_act_cls() && ensure_tower_glu_sig_cls() && ensure_tower_dw_conv5_cls() && ensure_tower_bias_act_cls()
        && (hs == 64l ? ensure_tower_g4a_attn64_cls() : ensure_tower_g4a_attn_cls()))

//! the gemma4a resident's key: the device block's, the f16 lane's halfword plane - its address, size and sampled words - and the rel table
def private cut_g4a_key(t : Gemma4aEncoder) : uint64 {
    let k = cut_key_wblob(cut_key_dev(t.dev, t.devplan, t.blob, t.n_layer), t.wblob)
    return tower_fold_words(k, unsafe(reinterpret<uint8 const?>(plane_at(t.pos_emb, 0l))), tower_g4a_rel_rows() * t.d_model * 4l)
}

//! the gemma4a resident's planes for the model the key names: as they are where it did not move, else uploaded (`cut_g4a_upload`)
def private cut_g4a_planes(var r : CutResident; var p : CutRolePlanes; t : Gemma4aEncoder) : bool {
    let key = cut_g4a_key(t)
    return r.key == key || cut_g4a_upload(r, p, t, key)
}

//! the gemma4a resident's planes off its CUDA image under `key`; false with both records released on a refused allocation
[cold_path]
def private cut_g4a_upload(var r : CutResident; var p : CutRolePlanes; t : Gemma4aEncoder; key : uint64) : bool {
    let rel = tower_g4a_rel_rows() * t.d_model * 4l
    cut_side_release(r, p)
    if (!cut_upload_planes(r, key, t.blob, t.wblob, gemma4a_mid_dim(t)) || (t.q8 && !cut_upload_q8(r, t, CUT_G4A_PIECE))) {
        cp_release(p, CUT_SIDE_ROLES)
        return false
    }
    let n_fft = (t.n_bins - 1l) * 2l
    cut_put_piece(r, r.fw16, t, CUT_G4A_FRONT16_PIECE)
    cp_up(r, p, TowerBuf.rel_tab, unsafe(reinterpret<void?>(plane_at(t.pos_emb, 0l))), rel)
    cp_up(r, p, TowerBuf.twiddle, unsafe(reinterpret<void?>(plane_at(t.twiddles, 0l))), n_fft * 2l * t.n_bins * 4l)
    cp_up(r, p, TowerBuf.filters, unsafe(reinterpret<void?>(plane_at(t.filters, 0l))), t.n_mel * t.n_bins * 4l)
    if (r.oom) {
        cut_side_release(r, p)
        return false
    }
    r.front = true
    return true
}

//! the chunk's scratch: the blocks' planes at `npos`, the tail at both widths, the rel feed, the front at `n_len` frames (a 30 s chunk's at least)
def private cut_g4a_scratch(var r : CutResident; var p : CutRolePlanes; t : Gemma4aEncoder; npos, n_len : int64) : bool {
    let d = t.d_model
    let rel_rows = tower_g4a_rel_rows()
    if (!cut_scratch(r, npos, d, t.n_ff, 0l, 0l)) {
        return false
    }
    let c = r.cap_rows
    cut_grow(r, r.tail_out, c * gemma4a_mid_dim(t) * 4l)
    cut_grow(r, r.tail_proj, c * t.proj_dim * 4l)
    cp_grow(r, p, TowerBuf.rel_tabh, (rel_rows + TILE_READ_SLACK) * d * 2l)
    cp_grow(r, p, TowerBuf.rel_proj, rel_rows * d * 4l)
    let b = tower_g4a_front_bytes(t, max(n_len, CUT_G4A_FRONT_FRAMES))
    cut_grow(r, r.planes, b.frames)
    cut_grow(r, r.col, b.col)
    cut_grow(r, r.stage1, b.stage1)
    cp_grow(r, p, TowerBuf.spec, b.spec)
    cp_grow(r, p, TowerBuf.mel, b.mel)
    cp_grow(r, p, TowerBuf.stage2, b.stage2)
    cp_grow(r, p, TowerBuf.feat16, b.feat16)
    return !r.oom || cut_side_oom(r, p)
}

//! the q8 lane's tiles at the sites: the blocks' five, the tail's two widths, the front's input projection; false where a tile did not build
def private cut_g4a_sites(var rl : CutRail; var r : CutResident; t : Gemma4aEncoder; npos, feat : int64) : bool {
    let d = t.d_model
    let mid = gemma4a_mid_dim(t)
    return (cut_site_pick(rl, r, RailSite.d, d, npos, d, false) && cut_site_pick(rl, r, RailSite.ff, t.n_ff, npos, d, false)
        && cut_site_pick(rl, r, RailSite.d2, 2l * d, npos, d, false) && cut_site_pick(rl, r, RailSite.rel, d, tower_g4a_rel_rows(), d, false)
        && cut_site_pick(rl, r, RailSite.tail1, mid, npos, d, false) && cut_site_pick(rl, r, RailSite.tail2, t.proj_dim, npos, mid, false)
        && cut_site_pick(rl, r, RailSite.front1, d, npos, feat, false))
}

//! the q8 lane's schedule: the blocks' records, then the front's input projection behind them as record `nb`; false counted `memory`
def private cut_g4a_sched(var rl : CutRail; var r : CutResident; t : Gemma4aEncoder; npos, nb : int64) : bool {
    let d = t.d_model
    cut_sched_begin(nb + 1l, tower_g4a_map_words(rl, t, npos) + rail_site_groups(rl, RailSite.front1, d, npos))
    tower_sched_walk(rl.maps, unsafe(addr(g_cut_sched[0])), nb + 1l) $(rid : int64) : TowerSchedRec {
        if (rid < nb) {
            return tower_g4a_rec(rl, t, npos, rid)
        }
        return TowerSchedRec(wblk0 = t.inproj_w / kq_elems(KqFmt.q8), rows = npos, groups = rail_site_groups(rl, RailSite.front1, d, npos))
    }
    return cut_sched_up(r)
}

//! gemma4a's whole chunk on the device (`register_gemma4a_chunk_gpu`) on the image's lane, soft tokens into `s.out`; the positions, or -1 declined
[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#g4a-chains")]
def private cuda_gemma4a_chunk(t : Gemma4aEncoder; var s : Gemma4aState; n_len : int64) : int64 {
    var r & = g_cut_g4a
    var p & = g_cp_g4a
    return -1l if (!cut_gate(r, true, t.dev_backend))
    let n_fft = (t.n_bins - 1l) * 2l
    let g = tower_g4a_front_geom(t, n_len)
    let npos = g.npos
    if (!tower_g4a_front_ok(t, n_len) || !tower_g4a_shape_ok(t, npos) || long_length(s.xw) < n_len * n_fft || npos > CUT_MAX_ENCODE_ROWS) {
        cut_decline(r, TowerDecline.shape)
        return -1l
    }
    return -1l if (!cut_arm(r))
    if (!cut_g4a_ensure(t.d_head)) {
        cut_decline(r, TowerDecline.device)
        return -1l
    }
    if (!cut_g4a_planes(r, p, t)) {
        cut_decline(r, TowerDecline.memory)
        return -1l
    }
    return -1l if (!cut_g4a_scratch(r, p, t, npos, n_len))
    var rl & = g_cut_rl
    rail_begin(rl, rail_lane_of(t.gemm_f16))
    rail_bind_rel(rl)
    rail_bind_f16(rl, fixed_array(RailSite.front1), TowerBuf.feat16)
    let nb = tower_g4a_block_recs(t)
    if (t.q8) {
        if (!cut_g4a_sites(rl, r, t, npos, g.feat)) {
            cut_decline(r, TowerDecline.device)
            return -1l
        }
        return -1l if (!cut_g4a_sched(rl, r, t, npos, nb))
    }
    cut_htod(r.planes.dev, unsafe(addr<void?>(s.xw[0])), n_len * n_fft * 4l)
    cut_side_seats(rl, r, p)
    cut_chain(rl, "g4a.chunk", (n_len << 32l) | npos) {
        tower_g4a_front(rl, t, n_len, nb, TowerBuf.x)
        tower_g4a_blocks(rl, t, npos)
    }
    s.out |> grow_resize(npos * t.proj_dim)   // the clip's rows exactly: a shorter clip's soft tokens are fewer than the last one's
    s.out_ready = true
    cut_served(r, r.tail_proj.dev, s.out, npos * t.proj_dim * 4l, npos, t.n_layer)
    cut_count_stem()
    return npos
}

//! the gemma4a CUDA flavor serves both lanes (q8, exact as f16): the knob on, the device armed with cm2 on the q8 lane, the classes at both head widths
def private cuda_gemma4a_serves_model(q8 : bool) : bool
    => cut_lane_arms("gemma4a", q8) && cut_flavor_classes("gemma4a", cut_g4a_ensure(64l) && cut_g4a_ensure(128l))

let private CUT_Q3A_PIECE = tower_family_piece(CUT_HOME, "q3a")           //! qwen3a's own q8 pieces: the conv-out GEMM and the projector's pair
let private CUT_Q3A_FRONT16_PIECE = tower_front16_piece(CUT_HOME, "q3a")  //! ... its three convs as the f16 slab

//! qwen3a's CUDA device block (`#tower-dev-block`): the core's whisper-class pieces, then qwen3a's own
[cold_path]
def private cuda_qwen3a_bake(t : Qwen3aTower; var b : TowerDevBuild) : bool {
    cuda_audio_tower_bake_into(t.core, b)
    tower_q3a_bake(t, b, CUT_HOME)
    return true
}

//! the qwen3a front resident's planes for the model the key names: as they are where it did not move, else uploaded (`cut_q3a_upload`)
def private cut_q3a_planes(var r : CutResident; var p : CutRolePlanes; t : Qwen3aTower) : bool {
    let key = cut_key_dev(t.core.dev, t.core.devplan, t.cblob, 1l)
    return r.key == key || cut_q3a_upload(r, p, t, key)
}

//! the qwen3a front resident's planes off its CUDA image under `key`: the conv blob, the lane's GEMM planes, the slab
[cold_path]
def private cut_q3a_upload(var r : CutResident; var p : CutRolePlanes; t : Qwen3aTower; key : uint64) : bool {
    cut_side_release(r, p)
    if (!cut_upload_planes(r, key, t.cblob, t.cf16 ? t.wcblob : PlaneU16(), 0l) || (t.cq8 && !cut_upload_q8(r, t.core, CUT_Q3A_PIECE))) {
        cp_release(p, CUT_SIDE_ROLES)
        return false
    }
    cut_put_piece(r, r.fw16, t.core, CUT_Q3A_FRONT16_PIECE)
    if (r.oom) {
        cut_side_release(r, p)
        return false
    }
    r.front = true
    return true
}

//! the conv front's classes but its tile - the f16 GEMM the f16 lane's sites ride among them
def private cut_q3a_ensure : bool
    => ensure_tower_im2col_cls() && ensure_f16_gemm_cm2_w_cls() && ensure_tower_bias_act_cls() && ensure_tower_q3a_shuffle_cls() && ensure_tower_clamp_cvt_cls()

//! qwen3a's conv front over a mel window's `n_chunks` chunks into `s.x` (`register_qwen3a_front_gpu`); false with the decline counted
[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#g4a-chains")]
def private cuda_q3a_front(t : Qwen3aTower; win : MelChunk; n_chunks : int64; var s : EncoderState) : bool {
    var r & = g_cut_q3a
    var p & = g_cp_q3a
    return false if (!cut_gate(r, t.cq8 || t.cf16, t.core.dev_backend))
    let f16w = t.cf16
    let nm = t.core.n_mel
    let d = t.core.d_model
    let featw = 16l * t.n_ch
    let npos = n_chunks * QWEN3A_TOK_PER_CHUNK
    if (!tower_q3a_front_ok(t, n_chunks) || npos > CUT_MAX_ENCODE_ROWS) {
        return cut_decline(r, TowerDecline.shape)
    }
    return false if (!cut_arm(r))
    if (!cut_q3a_ensure()) {
        return cut_decline(r, TowerDecline.device)
    }
    if (!cut_q3a_planes(r, p, t)) {
        return cut_decline(r, TowerDecline.memory)
    }
    let cap_chunks = max(n_chunks, QWEN3A_WINDOW / QWEN3A_CHUNK)
    let b = tower_q3a_front_bytes(t, cap_chunks)
    r.cap_rows = max(r.cap_rows, cap_chunks * QWEN3A_TOK_PER_CHUNK)
    cut_grow(r, r.planes, b.xin)
    cut_grow(r, r.col, b.col)
    cut_grow(r, r.stage1, b.y[0])
    cp_grow(r, p, TowerBuf.stage2, b.y[1])
    cp_grow(r, p, TowerBuf.stage3, b.y[2])
    cp_grow(r, p, TowerBuf.feat16, (r.cap_rows + TILE_READ_SLACK) * featw * 2l)
    cp_grow(r, p, TowerBuf.rows_out, r.cap_rows * d * 4l)
    return cut_side_oom(r, p) if (r.oom)
    var rl & = g_cut_rl
    rail_begin(rl, rail_lane_of(f16w))
    tower_q3a_binds(rl)
    if (!f16w) {
        if (!cut_site_pick(rl, r, RailSite.front1, d, npos, featw, false)) {
            return cut_decline(r, TowerDecline.device)
        }
        cut_sched_begin(1l, rail_site_groups(rl, RailSite.front1, d, npos))
        tower_sched_walk(rl.maps, unsafe(addr(g_cut_sched[0])), 1l) $(_rid : int64) : TowerSchedRec {
            return TowerSchedRec(wblk0 = t.cout_w_off / kq_elems(KqFmt.q8), rows = npos, groups = rail_site_groups(rl, RailSite.front1, d, npos))
        }
        return false if (!cut_sched_up(r))
    }
    let xin_n = QWEN3A_CHUNK * nm
    g_cut_q3a_xin |> grow_resize(n_chunks * xin_n)
    for (ci in range64(n_chunks)) {
        for (ih in range64(nm)) {
            for (iw in range64(QWEN3A_CHUNK)) {
                g_cut_q3a_xin[ci * xin_n + ih * QWEN3A_CHUNK + iw] = win.data[ih * win.n_len + ci * QWEN3A_CHUNK + iw]
            }
        }
    }
    cut_htod(r.planes.dev, unsafe(addr<void?>(g_cut_q3a_xin[0])), n_chunks * xin_n * 4l)
    cut_side_seats(rl, r, p)
    tower_q3a_front(rl, t, n_chunks, 0l)
    cut_dtoh(unsafe(addr<void?>(s.x[0])), cp_dev(p, TowerBuf.rows_out), npos * d * 4l)
    cut_side_landed(r)
    cut_count_stems(n_chunks)
    return true
}

//! the q8 projector's two sites picked and its schedule staged and uploaded; false with the decline counted
def private cut_q3a_proj_sched(var rl : CutRail; var r : CutResident; t : Qwen3aTower; npos : int64) : bool {
    let d = t.core.d_model
    let pdim = t.proj_dim
    if (!cut_site_pick(rl, r, RailSite.tail1, d, npos, d, false) || !cut_site_pick(rl, r, RailSite.tail2, pdim, npos, d, false)) {
        return cut_decline(r, TowerDecline.device)
    }
    let q8e = kq_elems(KqFmt.q8)
    cut_sched_begin(2l, rail_site_groups(rl, RailSite.tail1, d, npos) + rail_site_groups(rl, RailSite.tail2, pdim, npos))
    tower_sched_walk(rl.maps, unsafe(addr(g_cut_sched[0])), 2l) $(rid : int64) : TowerSchedRec {
        let site = rid == 0l ? RailSite.tail1 : RailSite.tail2
        return TowerSchedRec(wblk0 = (rid == 0l ? t.mm1_w_off : t.mm2_w_off) / q8e, rows = npos, groups = rail_site_groups(rl, site, rid == 0l ? d : pdim, npos))
    }
    return cut_sched_up(r)
}

//! qwen3a's MLP projector on the device (`register_qwen3a_proj_gpu`) over `s.xb` into `s.out`; false declined, or where the front did not run this encode
[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#g4a-chains")]
def private cuda_q3a_proj(t : Qwen3aTower; var s : EncoderState; npos : int64) : bool {
    var r & = g_cut_q3a
    var p & = g_cp_q3a
    return cut_decline(r, TowerDecline.flavor) if (t.core.dev_backend != TowerDev.cuda)
    let d = t.core.d_model
    let pdim = t.proj_dim
    let featw = 16l * t.n_ch
    if (r.cap_rows < npos || cp_dev(p, TowerBuf.rows_out) == 0ul || d % 64l != 0l || pdim % 64l != 0l || max(d, pdim) > featw || long_length(s.xb) < npos * d) {
        return cut_decline(r, TowerDecline.shape)
    }
    return false if (!cut_arm(r))
    if (!cut_q3a_ensure()) {
        return cut_decline(r, TowerDecline.device)
    }
    cut_grow(r, r.tail_hid, r.cap_rows * d * 4l)
    cut_grow(r, r.tail_proj, r.cap_rows * pdim * 4l)
    return cut_side_oom(r, p) if (r.oom)
    var rl & = g_cut_rl
    rail_begin(rl, rail_lane_of(t.cf16))
    tower_q3a_binds(rl)
    if (!t.cf16 && !cut_q3a_proj_sched(rl, r, t, npos)) {
        return false
    }
    cut_htod(cp_dev(p, TowerBuf.rows_out), unsafe(addr<void?>(s.xb[0])), npos * d * 4l)
    cut_side_seats(rl, r, p)
    tower_q3a_proj(rl, t, npos, 0l)
    s.out |> grow_resize(npos * pdim)   // the window's rows exactly: a shorter window's soft tokens are fewer than the last one's
    cut_dtoh(unsafe(addr<void?>(s.out[0])), r.tail_proj.dev, npos * pdim * 4l)
    cut_side_landed(r)
    cut_count_tail()
    return true
}

//! a qwen3a tower's CUDA chains serve the load's lane (`q8`, else the f16 lane): the whisper-class blocks', the front's classes
def private cuda_qwen3a_serves_model(q8 : bool) : bool => cut_whisper_class_serves("qwen3a", q8) && cut_flavor_classes("qwen3a", cut_q3a_ensure())

let private CUT_VAD_FLOOR_CHUNKS = 64l   //! the VAD scratch's chunks at least

//! `n` samples at `src` as ceil(n / 512) chunks behind the stream's context (`GK_VAD_CONTEXT`) on one stream (`register_vad_gpu`); probabilities, h and c read back
[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#g4a-chains")]
def private cuda_vad_chunks(m : VadModel; var st : VadState; src : float const?; n : int64; var probs : array<float>) : bool {
    var r & = g_cut_vad
    var p & = g_cp_vad
    return cut_decline(r, TowerDecline.knob) if (!cuda_tower_on())
    return cut_decline(r, TowerDecline.shape) if (!gk_vad_shape_ok(m, st, n))
    return false if (!cut_arm(r))
    if (!ensure_tower_vad_enc_cls() || !ensure_tower_vad_lstm_cls() || g_cut_vad_faults.device) {
        return cut_decline(r, TowerDecline.device)
    }
    if (r.key != m.slab_key) {
        cut_side_release(r, p)
        r.key = m.slab_key
        cut_up(r, r.norms, unsafe(addr<void?>(m.slab[0])), long_length(m.slab) * 4l)
    }
    let nch = (n + GK_VAD_CHUNK - 1l) / GK_VAD_CHUNK
    let cap = max(nch, CUT_VAD_FLOOR_CHUNKS)
    let bst = 2l * GK_VAD_HIDDEN * 4l
    cut_grow(r, r.planes, (GK_VAD_CONTEXT + cap * GK_VAD_CHUNK) * 4l)
    cp_grow(r, p, TowerBuf.vad_gates, cap * 4l * GK_VAD_HIDDEN * 4l)
    cp_grow(r, p, TowerBuf.vad_state, bst)
    cp_grow(r, p, TowerBuf.vad_probs, cap * 4l)
    r.oom = r.oom || g_cut_vad_faults.memory
    return cut_side_oom(r, p) if (r.oom)
    let nx = GK_VAD_CONTEXT + nch * GK_VAD_CHUNK
    ensure_length(g_cut_vad_x, nx)
    gk_vad_stage(unsafe(addr(g_cut_vad_x[0])), st, src, n, nch)
    let sdev = cp_dev(p, TowerBuf.vad_state)
    let hb = GK_VAD_HIDDEN * 4l
    cut_htod(r.planes.dev, unsafe(addr<void?>(g_cut_vad_x[0])), nx * 4l)
    cut_htod(sdev, unsafe(addr<void?>(st.h[0])), hb)
    cut_htod(sdev + uint64(hb), unsafe(addr<void?>(st.c[0])), hb)
    var pa = gk_vad_args(m.slab_offs, nch)
    var rl & = g_cut_rl
    rail_begin(rl, RailLane.q8)
    cut_side_seats(rl, r, p)
    tower_vad_chunks(rl, pa)
    let p0 = length(probs)
    probs |> resize(p0 + int(nch))   // nolint:PERF026 — the caller's probability row grows by the call's chunks: the seat's one output
    cut_dtoh(unsafe(addr<void?>(st.h[0])), sdev, hb)
    cut_dtoh(unsafe(addr<void?>(st.c[0])), sdev + uint64(hb), hb)
    cut_dtoh(unsafe(addr<void?>(probs[p0])), cp_dev(p, TowerBuf.vad_probs), nch * 4l)
    cut_side_landed(r)
    for (i in range64(GK_VAD_CONTEXT)) {
        st.context[i] = g_cut_vad_x[nch * GK_VAD_CHUNK + i]
    }
    g_cut_vad_calls++
    g_cut_vad_chunks += nch
    return true
}

//! gemma4a's seats on this home: the chunk hook and the flavor of both lanes
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#rail-audio-installs")]
def cuda_gemma4a_install {
    register_gemma4a_chunk_gpu(@@cuda_gemma4a_chunk)
    register_gemma4a_flavor(TowerDev.cuda, @@cuda_gemma4a_serves_model, @@cuda_gemma4a_bake)
}

//! qwen3a's own seats on this home - the front, the projector, the q8 flavor; its core blocks and its mel ride whisper's
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#rail-audio-installs")]
def cuda_qwen3a_install {
    register_qwen3a_front_gpu(@@cuda_q3a_front)
    register_qwen3a_proj_gpu(@@cuda_q3a_proj)
    register_qwen3a_flavor(TowerDev.cuda, @@cuda_qwen3a_serves_model, @@cuda_qwen3a_bake)
}

//! the device log-mel's seat on this home: the whisper preprocessors' mel, whisper's and qwen3a's (whisper's install arm)
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#rail-audio-installs")]
def cuda_device_mel_install {
    register_whisper_mel_gpu(@@cuda_whisper_mel)
}

//! the VAD's seat on this home
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#rail-audio-installs")]
def cuda_vad_install {
    register_vad_gpu(@@cuda_vad_chunks)
}

[init]
def dasllama_cuda_tower_g4a_register {
    cut_on_release(@@cut_g4a_release)
}
