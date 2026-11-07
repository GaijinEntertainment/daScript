options gen2
options indenting = 4
options _dasllama_internal = true

//! The CUDA home's whisper-class tower seats: the conv stem, the pre-LN blocks with the post-norm or the chat projector tail behind
//! them, each the shared body (`dasllama_tower_chains_whisper`) on the CUDA rail off the CUDA-flavored image's device block. The
//! stem's rows stay on the device for the blocks seat, which lands them on the host where it declines; the post-normed rows stay
//! on the device for the whisper decoder's cross-KV (`cuda_tower_enc_out`). Installed by `dasllama_cuda_tower_audio`'s whisper arm.
module dasllama_cuda_tower_whisper shared public

require dasllama/dasllama_lint public
require math
require dasllama/dasllama_kqformat
require dasllama/dasllama_math   // ensure_length
require dasllama/dasllama_plane
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
require dasllama/dasllama_audio
require dasllama/dasllama_tower_chains
require dasllama/dasllama_tower_chains_whisper

var private g_cut_aud = CutResident(family = "whisper-class")   //! the whisper-class towers' resident (one seat: a second tower replaces it)
var private g_cut_aud_pend_ptr : float? = null   //! the stem's rows still on the device (`x`) and the host array they are owed to; 0 bytes = none
var private g_cut_aud_pend_bytes = 0l
var private g_cut_aud_out : TowerEncOut   //! the encoder-output handoff: the served post-norm chain's `xb` rows still on the device
var private @scratch @exact_size g_cut_aud_rows : array<float>   //! the stem's f32 slab staging
let private CUT_AUD_PIECE = tower_family_piece(CUT_HOME, "aud")           //! a whisper-class tower's q8 pieces
let private CUT_AUD_FRONT16_PIECE = tower_front16_piece(CUT_HOME, "aud")  //! ... its conv stem's first conv as the f16 slab

//! the device plane of the encoder rows a caller is about to upload from `rows` (`bytes` long), else 0: the cross-KV chain copies them device to device
def cuda_tower_enc_out(rows : float const?; bytes : int64) : uint64 => enc_out_dev(g_cut_aud_out, rows, bytes, g_cut_aud.key != 0ul)

//! the stem's rows no longer pending: taken on the device, landed, or gone with their plane
def private cut_aud_drop_pending {
    g_cut_aud_pend_ptr = null
    g_cut_aud_pend_bytes = 0l
}

//! the stem's pending rows landed in the encoder state's `x` (the CPU block loop reads them); nothing pending after it
def private cut_aud_flush {
    return if (g_cut_aud_pend_bytes == 0l)
    cut_dtoh(unsafe(reinterpret<void?>(g_cut_aud_pend_ptr)), g_cut_aud.x.dev, g_cut_aud_pend_bytes)
    cut_sync(g_cut_aud.family)
    cut_aud_drop_pending()
}

//! a decline after the stem served: the stem's rows land in `s.x` first, since the CPU block loop reads them
def private cut_aud_flush_decline(why : TowerDecline) : bool {
    cut_aud_flush()
    return cut_decline(g_cut_aud, why)
}

[cold_path]
def private cut_aud_release {
    cut_release(g_cut_aud)
    cut_aud_drop_pending()
    g_cut_aud_out.bytes = 0l
}

//! the tower's planes for its key: the f32 blob whole, the lane's GEMM planes (the `cu.aud` q8 pieces or the f16 halfwords) as stored; the stem's slabs after
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#whisper-chains")]
def private cut_aud_attach(var r : CutResident; t : AudioTower) : bool {
    tower_aud_offs(t, g_cut_ln_offs)
    let key = cut_key_dev(t.dev, t.devplan, t.fblob, t.n_layer)
    return true if (r.key == key)
    cut_aud_release()
    return cut_upload_planes(r, key, t.fblob, t.gemm_f16 ? t.wblob : PlaneU16(), 0l) && (!t.q8 || cut_upload_q8(r, t, CUT_AUD_PIECE))
}

//! the stem's slabs: conv1 as the image's f16 piece, the f32 rows its bias stamps read - conv1's bias, conv2's, the positions
[cold_path]
def private cut_aud_front_slab(var r : CutResident; t : AudioTower) : bool {
    let d = t.d_model
    cut_put_piece(r, r.fw16, t, CUT_AUD_FRONT16_PIECE)
    let n = tower_aud_stem_rows(t)
    g_cut_aud_rows |> grow_resize(n)
    for (i in range64(d)) {
        g_cut_aud_rows[i] = t.fblob[t.conv1_b_off + i]
        g_cut_aud_rows[d + i] = t.fblob[t.conv2_b_off + i]
    }
    for (i in range64(t.n_ctx * d)) {
        g_cut_aud_rows[2l * d + i] = t.fblob[t.pos_off + i]
    }
    cut_up(r, r.fnorm, unsafe(addr<void?>(g_cut_aud_rows[0])), n * 4l)
    r.front = !r.oom
    return r.front
}

[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#whisper-chains")]
def private cut_aud_scratch(var r : CutResident; t : AudioTower; npos : int64) : bool
    => cut_scratch(r, npos, t.d_model, t.n_ff, t.n_head * 64l, 0l)

//! the whisper-class shape the chains serve: heads of 64, widths on the 64 lattice, the encode's rows inside the scratch's cap
def private cut_aud_shape_ok(t : AudioTower; npos : int64) : bool
    => t.d_model / t.n_head == 64l && t.d_model % 64l == 0l && t.n_ff % 64l == 0l && npos <= CUT_MAX_ENCODE_ROWS && npos > 0l

def private cut_aud_stem_ensure : bool
    => cut_ln_ensure(false, 64l) && ensure_f16_gemm_cm2_w_cls() && ensure_tts_im2col16_cls() && ensure_tower_bias_act_cls()

[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#whisper-chains")]
def private cuda_audio_conv_front(t : AudioTower; var s : EncoderState; mel : MelChunk) : bool {
    var r & = g_cut_aud
    g_cut_aud_out.bytes = 0l
    return false if (!cut_gate(r, t.q8 || t.gemm_f16, t.dev_backend))
    let f16w = t.gemm_f16
    let tin = AUDIO_CHUNK_FRAMES
    let tt = t.n_ctx
    let d = t.d_model
    let nm = t.n_mel
    if (!cut_aud_shape_ok(t, tt) || 3l * d > max(d, t.n_ff) || mel.n_len != tin || mel.n_mel != nm || !tower_aud_has_conv(t)) {
        return cut_decline(r, TowerDecline.shape)
    }
    return false if (!cut_arm(r))
    return cut_decline(r, TowerDecline.device) if (!cut_aud_stem_ensure())
    if (!cut_aud_attach(r, t) || (!r.front && !cut_aud_front_slab(r, t))) {
        cut_aud_release()
        return cut_decline(r, TowerDecline.memory)
    }
    return false if (!cut_aud_scratch(r, t, tt))
    cut_grow(r, r.planes, nm * tin * 4l)
    cut_grow(r, r.col, tin * tower_aud_stem_kdp(t) * 2l)
    cut_grow(r, r.stage1, tin * d * 4l)
    return cut_decline(r, TowerDecline.memory) if (r.oom)
    var rl & = g_cut_rl
    rail_begin(rl, rail_lane_of(f16w))
    if (!f16w) {
        return cut_decline(r, TowerDecline.device) if (!cut_site_pick(rl, r, RailSite.conv2, d, tt, 3l * d, false))
        cut_sched_begin(1l, rail_site(rl, RailSite.conv2).groups)
        tower_sched_walk(rl.maps, unsafe(addr(g_cut_sched[0])), 1l) $(_rid : int64) : TowerSchedRec {
            return tower_aud_stem_rec(rl, t)
        }
        return false if (!cut_sched_up(r))
    }
    cut_htod(r.planes.dev, unsafe(addr<void?>(mel.data[0])), nm * tin * 4l)
    cut_rail_seats(rl, r)
    tower_aud_stem(rl, t, tin)
    g_cut_aud_pend_ptr = unsafe(addr(s.x[0]))
    g_cut_aud_pend_bytes = tt * d * 4l
    cut_count_stem()
    cut_announce(r) if (!r.served_said)
    return true
}

//! the chat tail's classes and, on the q8 lane, its two sites' picks on the rail; false where one did not build (the blocks alone serve)
def private cut_ct_ready(var rl : CutRail; var r : CutResident; ct : TowerChatTail) : bool {
    let feed_ok = ensure_tower_ln16_cls() && ensure_tower_clamp_cvt_cls()
    let rows_ok = (ensure_tower_pool_rows_cls() && ensure_tower_ln_cls() && ensure_tower_bias_act_cls() && ensure_tower_rms_cls()
        && ensure_tower_swiglu_swapped_cls() && ensure_tower_act_cls() && feed_ok)
    return true if (rows_ok && rl.lane == RailLane.f16)
    return rows_ok && cut_site_pick(rl, r, RailSite.tail1, ct.n1out, ct.rows, ct.m1in, false) && (ct.m2in == 0l || cut_site_pick(rl, r, RailSite.tail2, ct.out, ct.rows, ct.m2in, false))
}

//! the chat tail armed where this encode rides it: classes and picks (else it stands down, the blocks alone serving), its planes; false where refused
def private cut_ct_arm(var rl : CutRail; var r : CutResident; var ct : TowerChatTail) : bool {
    if (ct.on && !cut_ct_ready(rl, r, ct)) {
        ct.on = false
        tower_ct_stand_down("cuda", ct.kind)
    }
    return true if (!ct.on)
    let c = r.cap_rows
    cut_grow(r, r.tail_hid, c * max(tower_ct_hid(ct), 1l) * 4l)
    cut_grow(r, r.tail_gate, c * max(tower_ct_gate(ct), 1l) * 4l)
    cut_grow(r, r.tail_proj, c * ct.out * 4l)
    return !r.oom
}

//! the q8 lane's schedule (the blocks' records, the tail's behind where it rides) staged and uploaded; false where the upload was refused
def private cut_aud_sched(var rl : CutRail; var r : CutResident; t : AudioTower; ct : TowerChatTail; npos : int64) : bool {
    let d = t.d_model
    let ff = t.n_ff
    let nl = t.n_layer
    let nrec_blocks = nl * LN_BLOCK_GEMMS
    let g_tail = ct.on ? rail_site(rl, RailSite.tail1).groups + (ct.m2in > 0l ? rail_site(rl, RailSite.tail2).groups : 0l) : 0l
    cut_sched_begin(nrec_blocks + tower_ct_gemms(ct), nl * (4l * rail_site(rl, RailSite.d).groups + rail_site(rl, RailSite.ff).groups + rail_site(rl, RailSite.fc2).groups) + g_tail)
    tower_sched_walk(rl.maps, unsafe(addr(g_cut_sched[0])), nrec_blocks + tower_ct_gemms(ct)) $(rid : int64) : TowerSchedRec {
        return rid < nrec_blocks ? tower_ln_rec(rl, g_cut_ln_offs, d, ff, npos, rid) : tower_ct_rec(rl, ct, rid - nrec_blocks)
    }
    return cut_sched_up(r)
}

[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#whisper-chains")]
def private cut_aud_blocks(t : AudioTower; var s : EncoderState; npos : int64; ln_post : bool; var ct : TowerChatTail) : bool {
    var r & = g_cut_aud
    g_cut_aud_out.bytes = 0l
    if (!cut_gate(r, t.q8 || t.gemm_f16, t.dev_backend)) {
        cut_aud_flush()
        return false
    }
    return cut_aud_flush_decline(TowerDecline.shape) if (!cut_aud_shape_ok(t, npos))
    if (!cut_arm(r)) {
        cut_aud_flush()
        return false
    }
    let f16w = t.gemm_f16
    return cut_aud_flush_decline(TowerDecline.device) if (!cut_ln_ensure(false, 64l) || (f16w && !ensure_f16_gemm_cm2_w_cls()))
    return cut_aud_flush_decline(TowerDecline.memory) if (!cut_aud_attach(r, t))
    if (!cut_aud_scratch(r, t, npos)) {
        cut_aud_flush()
        return false
    }
    let d = t.d_model
    let ff = t.n_ff
    let nl = t.n_layer
    var rl & = g_cut_rl
    rail_begin(rl, rail_lane_of(f16w))
    if (!f16w && (!cut_site_pick(rl, r, RailSite.d, d, npos, d, false) || !cut_site_pick(rl, r, RailSite.ff, ff, npos, d, false)
            || !cut_site_pick(rl, r, RailSite.fc2, d, npos, ff, true))) {
        return cut_aud_flush_decline(TowerDecline.device)
    }
    return cut_aud_flush_decline(TowerDecline.memory) if (!cut_ct_arm(rl, r, ct))
    if (!f16w && !cut_aud_sched(rl, r, t, ct, npos)) {
        cut_aud_flush()
        return false
    }
    let bx = npos * d * 4l
    if (g_cut_aud_pend_bytes > 0l) {
        cut_aud_drop_pending()
    } else {
        cut_htod(r.x.dev, unsafe(addr<void?>(s.x[0])), bx)
    }
    cut_rail_seats(rl, r)
    cut_chain(rl, "aud.blocks", (npos << 2l) | (ln_post ? 1l : 0l) | (ct.on ? 2l : 0l)) {
        tower_aud_blocks(rl, t, g_cut_ln_offs, ct, ln_post, npos)
    }
    if (ct.on) {
        cut_count_tail()
        ensure_length(s.out, ct.rows * ct.out)
        return cut_served(r, r.tail_proj.dev, s.out, ct.rows * ct.out * 4l, npos, nl)
    }
    if (ln_post) {
        g_cut_aud_out = TowerEncOut(host = intptr(unsafe(addr(s.xb[0]))), bytes = bx, dev = r.xb.dev)
        return cut_served(r, r.xb.dev, s.xb, bx, npos, nl)
    }
    return cut_served(r, r.x.dev, s.x, bx, npos, nl)
}

//! the blocks, with the projector tail behind where `tower_ct_shape_ok` admits it; answers which served, so the CPU tail runs exactly when the device's did not
[hot_path]
def private cuda_audio_tower_blocks_tail(t : AudioTower; var s : EncoderState; npos : int64) : TowerTailServed {
    var ct = TowerChatTail()
    if (tower_ct_shape_ok(t, npos)) {
        ct = tower_chat_tail_of(t, npos)
        ct.on = true
    } else {
        cut_decline(g_cut_aud, TowerDecline.shape)   // the tail alone declines; the blocks serve and the CPU tail reads their rows
    }
    return TowerTailServed.declined if (!cut_aud_blocks(t, s, npos, false, ct))
    return ct.on ? TowerTailServed.tail : TowerTailServed.blocks
}

[hot_path]
def private cuda_audio_tower_blocks(t : AudioTower; var s : EncoderState; npos : int64) : bool {
    var ct = TowerChatTail()
    return cut_aud_blocks(t, s, npos, false, ct)
}

[hot_path]
def private cuda_audio_tower_blocks_ln_post(t : AudioTower; var s : EncoderState; npos : int64) : bool {
    var ct = TowerChatTail()
    return cut_aud_blocks(t, s, npos, true, ct)
}

//! A whisper-class tower's CUDA device block (`#tower-dev-block`): `tower_aud_bake` under this home's prefix
[cold_path, arch(at = "../ARCHITECTURE_IMAGE_GPU_PLANES.md#tower-dev-block")]
def cuda_audio_tower_bake_into(t : AudioTower; var b : TowerDevBuild) {
    tower_aud_bake(t, b, CUT_HOME)
}

[cold_path]
def private cuda_audio_tower_bake(t : AudioTower; var b : TowerDevBuild) : bool {
    cuda_audio_tower_bake_into(t, b)
    return true
}

//! a whisper-class tower's CUDA chains serve the load's lane (`q8`, else the f16 lane): the lane armed, the block and the stem classes
def cut_whisper_class_serves(family : string; q8 : bool) : bool => cut_lane_arms(family, q8) && cut_flavor_classes(family, cut_aud_stem_ensure())

//! the standalone whisper-class tower's CUDA flavor pick (`cut_whisper_class_serves`)
def cuda_whisper_tower_serves(q8 : bool) : bool => cut_whisper_class_serves("the whisper-class tower", q8)

//! the CUDA home's whisper-class tower seats: the block, post-norm, tail and stem hooks and the flavor (the device mel is `cuda_device_mel_install`'s)
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#rail-audio-installs")]
def cuda_whisper_install_tower {
    register_tower_blocks_gpu(@@cuda_audio_tower_blocks)
    register_tower_blocks_ln_post_gpu(@@cuda_audio_tower_blocks_ln_post)
    register_tower_blocks_tail_gpu(@@cuda_audio_tower_blocks_tail)
    register_tower_conv_gpu(@@cuda_audio_conv_front)
    register_audio_tower_flavor(TowerDev.cuda, @@cuda_whisper_tower_serves, @@cuda_audio_tower_bake)
}

[init]
def dasllama_cuda_tower_whisper_register {
    cut_on_release(@@cut_aud_release)
}
