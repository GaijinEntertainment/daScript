options gen2
options indenting = 4
options _dasllama_internal = true

//! The CUDA home's decoders behind the FastConformer encoders: the transducer's greedy decode (parakeet, GigaAM's RNNT head) and
//! GigaAM's CTC head, each the shared body (`dasllama_tower_chains_fc`) on the CUDA rail, off the encode's device rows where the
//! chain left them (`cuda_tower_fc_rows`), else the rows uploaded. Installed by `dasllama_cuda_tower_audio`'s arms.
module dasllama_cuda_asr_dec_fc shared public

require dasllama/dasllama_lint public
require math
require daslib/math_boost   // round_up: the CTC head's padded rows, the per-clip planes' 64-frame step
require dasllama/dasllama_math   // grow_resize
require dasllama/dasllama_env   // g_env_vulkan.vk_rnnt: the transducer knob's env twin both homes read
require dasllama/dasllama_plane
require dasllama/dasllama_tower
require dasllama/dasllama_gpu_tier   // GpuDeclineCounter
require dasllama/dasllama_gpu_math
require dasllama/dasllama_gpu_kernels_common   // GkRnntArgs and the transducer stamps' staged widths
require dasllama/dasllama_gpu_kernels_discrete
require dasllama/dasllama_cuda_common
require dasllama/dasllama_cuda_classes
require dasllama/dasllama_cuda_classes_asr
require dasllama/dasllama_cuda_rail
require dasllama/dasllama_cuda_tower_seat
require dasllama/dasllama_cuda_tower_fc
require dasllama/dasllama_rnnt
require dasllama/dasllama_gigaam
require dasllama/dasllama_tower_chains_fc

//! the transducer seat's planes by role: the weights laid once a net (the prediction net's and the joint's halves, the slab, the
//! encoder projection's f32 rows), then the per-clip planes (the state words, the LSTM pair, the net's rows, the logits, the
//! token records, the projected encoder rows)
let private CUR_ROLES = fixed_array(TowerBuf.pred_w16, TowerBuf.joint_w16, TowerBuf.net_rows, TowerBuf.enc_proj_w, TowerBuf.dec_state, TowerBuf.lstm_h,
    TowerBuf.lstm_c, TowerBuf.pred_in, TowerBuf.pred_out, TowerBuf.logits, TowerBuf.picks, TowerBuf.enc_proj)

//! The transducer seat's device planes: the weights laid once a net (`rnnt_dev_key`), the per-clip planes grown to the longest clip
//! (`CUR_ROLES`), and the encoder rows uploaded where no device encode of their generation is held
struct private CurResident {
    t : CutResident
    lay : RnntDevLayout
    p : CutRolePlanes
    rows : CutBuf
    miss_said : bool
}

var private g_cur = CurResident(t = CutResident(family = "transducer"))
var private @scratch @exact_size g_cur_words : array<uint>   //! the state words, then the token records, read back
var private @scratch @exact_size g_cur_host : array<float>   //! the clip's start staged: the hidden pair and cells zeroed, the first input
var private g_cur_env = true
var private g_cur_declines = GpuDeclineCounter(home = "cuda")
var private g_cur_decodes = 0l
var private g_cur_handoffs = 0l
var private g_cur_projects = 0l
var private g_cur_uploads = 0l
var private g_cur_rate = RNNT_DEV_RATE0   //! the rounds a frame the last served clip took: the first launch's round estimate

//! Engage counters: `decodes` served, `handoffs` of them off the encode's device rows, `projects` projected on the device, `uploads` weight uploads, `declines` all but the knob's.
def cuda_rnnt_stats : tuple<decodes : int64; handoffs : int64; projects : int64; uploads : int64; declines : int64> {
    return (decodes = g_cur_decodes, handoffs = g_cur_handoffs, projects = g_cur_projects, uploads = g_cur_uploads,
        declines = g_cur_declines.total - (g_cur_declines.counts?["{RnntDevDecline.knob}"] ?? 0l))
}

//! The transducer seat's declines-by-reason table (a copy), keyed by the decline's name, the knob's among them.
def cuda_rnnt_declines : table<string; int64> => <- clone_to_move(g_cur_declines.counts)

//! Pin the transducer decode on the device on/off in-process; answers the setting it replaced.
def set_cuda_rnnt(on : bool) : bool {
    let was = g_cur_env
    g_cur_env = on
    return was
}

[cold_path]
def private cur_decline(why : RnntDevDecline) : bool {
    let words = rnnt_dev_decline_words(why, "CUDA", "the transducer knob is off (DASLLAMA_VK_RNNT=0 or set_cuda_rnnt(false))", CUT_DECLINE_WORDS_DEVICE, CUT_DECLINE_WORDS_MEMORY)
    return g_cur_declines |> note("the transducer decode", "{why}", "the decode stays on the CPU loop: {words}")
}

[cold_path]
def private cur_release {
    var r & = g_cur
    cp_release(r.p, CUR_ROLES)
    cut_free(r.rows)
    cut_release(r.t)
    r <- CurResident(t = CutResident(family = "transducer"))
    g_cur_rate = RNNT_DEV_RATE0
    g_cur_declines |> unsay("the transducer decode")
}

//! the net's weights for the key: as they are where it did not move, else laid (`cur_attach_build`)
def private cur_attach(net : RnntNet; enc : RnntEnc) : bool {
    let key = rnnt_dev_key(net, enc)
    return (g_cur.t.key == key && cp_dev(g_cur.p, TowerBuf.pred_w16) != 0ul) || cur_attach_build(net, enc, key)
}

//! the net's halves and slab off its CUDA image's pieces as stored (`#tower-dev-block`), the projection's f32 rows where the seat projects; once a net
[cold_path]
def private cur_attach_build(net : RnntNet; enc : RnntEnc; key : uint64) : bool {
    cur_release()
    var r & = g_cur
    r.lay = rnnt_dev_layout(net)
    if (!rnnt_dev_planes_ok(net, r.lay)) {
        return cur_decline(RnntDevDecline.shape)
    }
    cut_up(r.t, r.p.b[int(TowerBuf.pred_w16)], unsafe(reinterpret<void?>(plane_at(net.wh, 0l))), r.lay.wh_halves * 2l)
    cut_up(r.t, r.p.b[int(TowerBuf.joint_w16)], unsafe(reinterpret<void?>(plane_at(net.jw, 0l))), r.lay.jw_halves * 2l)
    cut_up(r.t, r.p.b[int(TowerBuf.net_rows)], unsafe(reinterpret<void?>(plane_at(net.blob, net.embed))), r.lay.wf_floats * 4l)
    if (!enc.q8) {
        cp_up(r.t, r.p, TowerBuf.enc_proj_w, unsafe(reinterpret<void?>(plane_at(net.blob, enc.w))), net.joint_dim * enc.d * 4l)
    }
    r.t.oom = r.t.oom || rnnt_dev_faulted(RnntDevFault.memory)
    if (r.t.oom) {
        cur_release()
        return cur_decline(RnntDevDecline.memory)
    }
    r.t.key = key
    g_cur_uploads++
    return true
}

//! the per-clip planes for `n_frames` (a whole 64-frame step past the largest clip yet)
def private cur_scratch(net : RnntNet; n_frames, n_logits, d : int64) : bool {
    var r & = g_cur
    let cap = round_up(n_frames, 64l)
    cp_grow(r.t, r.p, TowerBuf.dec_state, int64(GK_RNNT_ST_WORDS) * 4l)
    cp_grow(r.t, r.p, TowerBuf.lstm_h, 2l * net.n_layers * net.pred_dim * 4l)
    cp_grow(r.t, r.p, TowerBuf.lstm_c, net.n_layers * net.pred_dim * 4l)
    cp_grow(r.t, r.p, TowerBuf.pred_in, net.pred_dim * 4l)
    cp_grow(r.t, r.p, TowerBuf.pred_out, net.joint_dim * 4l)
    cp_grow(r.t, r.p, TowerBuf.logits, int64(GK_RNNT_FRAMES) * rnnt_joint_blocks(n_logits) * RNNT_JOINT_BLOCK * 4l)
    cp_grow(r.t, r.p, TowerBuf.picks, rnnt_dev_cap(net, cap) * int64(GK_RNNT_TOK_WORDS) * 4l)
    cp_grow(r.t, r.p, TowerBuf.enc_proj, cap * net.joint_dim * 4l)
    if (d > 0l) {
        cut_grow(r.t, r.rows, cap * d * 4l)
    }
    if (r.t.oom) {
        r.t.oom = false
        return cur_decline(RnntDevDecline.memory)
    }
    return true
}

//! the clip's start uploaded: the state words (the first predictor step asked, the duration table), the hidden pair and cells zeroed, the first input
def private cur_state_init(net : RnntNet) {
    let r & = g_cur
    g_cur_words |> grow_resize(int64(GK_RNNT_ST_WORDS))
    rnnt_dev_state_words(net, unsafe(addr(g_cur_words[0])))
    cut_htod(cp_dev(r.p, TowerBuf.dec_state), unsafe(addr<void?>(g_cur_words[0])), int64(GK_RNNT_ST_WORDS) * 4l)
    let nz = 3l * net.n_layers * net.pred_dim
    g_cur_host |> grow_resize(nz + net.pred_dim)
    for (i in range64(nz)) {
        g_cur_host[i] = 0.0
    }
    rnnt_dev_first_input(net, unsafe(addr(g_cur_host[nz])))
    unsafe {
        let hp = addr(g_cur_host[0])
        cut_htod(cp_dev(r.p, TowerBuf.lstm_h), hp, 2l * net.n_layers * net.pred_dim * 4l)
        cut_htod(cp_dev(r.p, TowerBuf.lstm_c), hp + 2l * net.n_layers * net.pred_dim, net.n_layers * net.pred_dim * 4l)
        cut_htod(cp_dev(r.p, TowerBuf.pred_in), hp + nz, net.pred_dim * 4l)
    }
}

//! the transducer seat's planes on the rail, the encoder rows at `rows_dev`
def private cur_rail_seats(var rl : CutRail; r : CurResident; rows_dev : uint64; rows_bytes : int64) {
    rl.stream = g_cud.stream
    cp_seats(rl, r.p, CUR_ROLES)
    cut_seat(rl, TowerBuf.rows_out, CutBuf(dev = rows_dev, bytes = rows_bytes))
}

//! Whether this seat projects a lane's encoder half: the f32 plane's, through the f32 tile.
[hot_path]
def private cuda_rnnt_projects(enc : RnntEnc) : bool => g_cur_env && !enc.q8

//! the transducer's greedy decode here (`#rnnt-device-decode`): the encoder projection, then `asr_dec_rnnt_rounds` round chains until the state reads done; the records once
[hot_path, unused_argument(s), arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#fc-chains")]
def private cuda_rnnt_greedy(net : RnntNet; var s : RnntState; enc : RnntEnc; x, encp : array<float>; n_frames : int64; @scratch var toks : array<RnntToken>) : bool {
    return cur_decline(RnntDevDecline.knob) if (!g_cur_env)
    return cur_decline(RnntDevDecline.flavor) if (!net.dev_only)
    let jd = net.joint_dim
    let d = enc.d
    let n_logits = rnnt_n_logits(net)
    let projects = !enc.q8
    return cur_decline(RnntDevDecline.shape) if (!rnnt_dev_shape_ok(net, enc, 1l))
    if (!cuda_tower_arms() || !ensure_rnnt_gates_cls() || !ensure_rnnt_proj_cls() || !ensure_rnnt_joint_cls() || !ensure_rnnt_pick_cls()
            || (projects && !ensure_f32_gemm_cls()) || rnnt_dev_faulted(RnntDevFault.device)) {
        return cur_decline(RnntDevDecline.device)
    }
    cud_bind()
    return false if (!cur_attach(net, enc) || !cur_scratch(net, max(n_frames, 1l), n_logits, d))
    if (n_frames <= 0l) {
        g_cur_decodes++
        return true
    }
    var r & = g_cur
    cur_state_init(net)
    var rows_dev = 0ul
    var handoff = false
    if (projects) {
        rows_dev = cuda_tower_fc_rows(enc.gen, n_frames * d * 4l)
        handoff = rows_dev != 0ul
        if (!handoff) {
            rnnt_dev_miss_say(r.miss_said, "cuda", enc.gen) if (!r.miss_said)
            cut_htod(r.rows.dev, unsafe(addr<void?>(x[0])), n_frames * d * 4l)
            rows_dev = r.rows.dev
        }
    } else {
        cut_htod(cp_dev(r.p, TowerBuf.enc_proj), unsafe(addr<void?>(encp[0])), n_frames * jd * 4l)
    }
    var rl & = g_cut_rl
    cur_rail_seats(rl, r, rows_dev, n_frames * d * 4l)
    let ka = rnnt_dev_args(net, r.lay, n_frames, projects)
    let run = rnnt_dev_rounds(net, n_frames, unsafe(addr<uint const?>(g_cur_words[0])), g_cur_rate) $(rounds : int64; first : bool) : bool {
        if (first && projects) {
            asr_dec_rnnt_rounds(rl, net, r.lay, ka, d, n_frames, 0l, true)
        }
        cut_chain_rounds(rl, "rnnt.rounds", n_frames, rounds) $(n : int64) {
            asr_dec_rnnt_rounds(rl, net, r.lay, ka, d, n_frames, n, false)
        }
        cut_dtoh(unsafe(addr<void?>(g_cur_words[0])), cp_dev(r.p, TowerBuf.dec_state), int64(GK_RNNT_ST_WORDS) * 4l)
        cut_sync("rnnt decode")
        return true
    }
    return cur_decline(RnntDevDecline.overrun) if (!run.ok)
    let ntok = run.p.ntok
    if (ntok > 0) {
        g_cur_words |> grow_resize(int64(ntok) * int64(GK_RNNT_TOK_WORDS))
        cut_dtoh(unsafe(addr<void?>(g_cur_words[0])), cp_dev(r.p, TowerBuf.picks), long_length(g_cur_words) * 4l)
        cut_sync("rnnt decode")
        rnnt_dev_land_tokens(unsafe(addr<uint const?>(g_cur_words[0])), ntok, toks)
    }
    rnnt_dev_announce(r.t.served_said, "cuda", net.pred_dim, jd, n_logits, projects) if (!r.t.served_said)
    g_cur_decodes++
    if (projects) {
        g_cur_projects++
    }
    if (handoff) {
        g_cur_handoffs++
    }
    return true
}

//! The CTC seat's device planes: the head's weights and bias laid once a model, the per-clip planes grown to the longest clip
struct private CucResident {
    t : CutResident
    w : CutBuf
    b : CutBuf
    rows : CutBuf
    xh : CutBuf
    lg : CutBuf
    pk : CutBuf
    miss_said : bool
}

var private g_cuc = CucResident(t = CutResident(family = "ctc"))
var private @scratch @exact_size g_cuc_words : array<uint>   //! a frame's record [class, plog bits] read back
var private g_cuc_declines = GpuDeclineCounter(home = "cuda")
var private g_cuc_heads = 0l
var private g_cuc_handoffs = 0l
var private g_cuc_uploads = 0l

//! Engage counters: `heads` served on the device, `handoffs` of them off the encode's device rows, `uploads` the head uploads, `declines` all but the knob's.
def cuda_ctc_stats : tuple<heads : int64; handoffs : int64; uploads : int64; declines : int64> {
    return (heads = g_cuc_heads, handoffs = g_cuc_handoffs, uploads = g_cuc_uploads,
        declines = g_cuc_declines.total - (g_cuc_declines.counts?["{CtcDevDecline.knob}"] ?? 0l))
}

[cold_path]
def private cuc_decline(why : CtcDevDecline) : bool {
    let words = ctc_dev_decline_words(why, "CUDA", CUT_DECLINE_WORDS_TOWER_KNOB, CUT_DECLINE_WORDS_DEVICE, CUT_DECLINE_WORDS_MEMORY)
    return g_cuc_declines |> note("the CTC head", "{why}", "the head stays on the CPU: {words}")
}

[cold_path]
def private cuc_release {
    var r & = g_cuc
    for (b in fixed_array(r.w, r.b, r.rows, r.xh, r.lg, r.pk)) {
        cuda_mem_free(b.dev)
    }
    cut_release(r.t)
    r <- CucResident(t = CutResident(family = "ctc"))
    g_cuc_declines |> unsay("the CTC head")
}

//! the head's planes uploaded once a model as the image holds them: the f16 head's padded halfword rows or the f32 head's rows, and the bias row
def private cuc_attach(m : GigaamModel) : bool {
    let nw = (m.ctc_f16 ? round_up(m.n_classes, GIGAAM_CTC_ROW_TILE) : m.n_classes) * m.d_model
    var key = hash_combine64(m.ctc_f16 ? intptr(unsafe(reinterpret<void?>(m.wblob.p))) : intptr(unsafe(reinterpret<void?>(m.blob.p))),
        hash_combine64(uint64(m.ctc_w), uint64(m.ctc_b)))
    key = hash_combine64(key, hash_combine64(uint64(nw), uint64(m.n_classes)))
    return true if (g_cuc.t.key == key && g_cuc.w.dev != 0ul)
    cuc_release()
    var r & = g_cuc
    if (m.ctc_f16) {
        cut_up(r.t, r.w, unsafe(reinterpret<void?>(plane_at(m.wblob, m.ctc_w))), nw * 2l)
    } else {
        cut_up(r.t, r.w, unsafe(reinterpret<void?>(plane_at(m.blob, m.ctc_w))), nw * 4l)
    }
    cut_up(r.t, r.b, unsafe(reinterpret<void?>(plane_at(m.blob, m.ctc_b))), m.n_classes * 4l)
    if (r.t.oom) {
        cuc_release()
        return cuc_decline(CtcDevDecline.memory)
    }
    r.t.key = key
    g_cuc_uploads++
    return true
}


//! GigaAM's CTC head here (`asr_dec_ctc`, `#gigaam-planes`): device rows by generation (else `st.x` uploaded), picks and plogs into `st.picks` / `st.plogs`; false declines
[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#fc-chains")]
def private cuda_gigaam_ctc(m : GigaamModel; var st : GigaamState; n_frames : int64) : bool {
    return cuc_decline(CtcDevDecline.knob) if (!cuda_tower_on())
    return cuc_decline(CtcDevDecline.flavor) if (m.dev_backend != TowerDev.cuda)
    let d = m.d_model
    let nc = m.n_classes
    let ncp = round_up(nc, GIGAAM_CTC_ROW_TILE)
    let f16 = m.ctc_f16
    if (n_frames < 1l || d % 64l != 0l || long_length(st.x) < n_frames * d) {
        return cuc_decline(CtcDevDecline.shape)
    }
    if (!cuda_tower_arms() || !ensure_ctc_pick_cls() || (f16 ? !(ensure_tower_clamp_cvt_cls() && ensure_f16_gemm_cm2_w_cls()) : !ensure_f32_gemm_cls())) {
        return cuc_decline(CtcDevDecline.device)
    }
    cud_bind()
    return false if (!cuc_attach(m))
    var r & = g_cuc
    let cap = round_up(n_frames, 64l)
    let bx = n_frames * d * 4l
    cut_grow(r.t, r.rows, cap * d * 4l)
    if (f16) {
        cut_grow(r.t, r.xh, (cap + TILE_READ_SLACK) * d * 2l)
    }
    cut_grow(r.t, r.lg, cap * ncp * 4l)
    cut_grow(r.t, r.pk, cap * 2l * 4l)
    if (r.t.oom) {
        r.t.oom = false
        return cuc_decline(CtcDevDecline.memory)
    }
    var rows_dev = cuda_tower_fc_rows(st.enc_gen, bx)
    let handoff = rows_dev != 0ul
    if (!handoff) {
        ctc_dev_miss_say(r.miss_said, "cuda", st.enc_gen) if (!r.miss_said)
        cut_htod(r.rows.dev, unsafe(addr<void?>(st.x[0])), bx)
        rows_dev = r.rows.dev
    }
    var rl & = g_cut_rl
    rl.stream = g_cud.stream
    cut_seat(rl, TowerBuf.rows_out, CutBuf(dev = rows_dev, bytes = bx))
    cut_seat(rl, TowerBuf.xh, r.xh)
    cut_seat(rl, TowerBuf.head_w, r.w)
    cut_seat(rl, TowerBuf.head_b, r.b)
    cut_seat(rl, TowerBuf.logits, r.lg)
    cut_seat(rl, TowerBuf.picks, r.pk)
    asr_dec_ctc(rl, f16, d, nc, ncp, n_frames)
    g_cuc_words |> grow_resize(n_frames * 2l)
    cut_dtoh(unsafe(addr<void?>(g_cuc_words[0])), r.pk.dev, n_frames * 2l * 4l)
    cut_sync("gigaam ctc")
    for (t in range64(n_frames)) {
        st.picks[t] = int(g_cuc_words[2l * t])
        st.plogs[t] = unsafe(reinterpret<float>(g_cuc_words[2l * t + 1l]))
    }
    ctc_dev_announce(r.t.served_said, "cuda", nc, d, f16) if (!r.t.served_said)
    g_cuc_heads++
    if (handoff) {
        g_cuc_handoffs++
    }
    return true
}

//! the decoder seats behind the FastConformer encoders on this home: GigaAM's CTC head, the transducer decode
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#rail-audio-installs")]
def cuda_asr_dec_install_fc(fam : TowerAudioFamily) {
    if (fam == TowerAudioFamily.gigaam) {
        register_gigaam_gpu_ctc(@@cuda_gigaam_ctc)
    } elif (fam == TowerAudioFamily.transducer) {
        register_rnnt_gpu(@@cuda_rnnt_greedy, @@cuda_rnnt_projects)
    }
}

[init]
def dasllama_cuda_asr_dec_fc_register {
    g_cur_env = g_env_vulkan.vk_rnnt
    cut_on_release(@@cur_release)
    cut_on_release(@@cuc_release)
}
