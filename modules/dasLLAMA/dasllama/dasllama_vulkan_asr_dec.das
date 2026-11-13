options gen2
options indenting = 4
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_vulkan_asr_dec shared public

require dasllama/dasllama_lint public
require math
require daslib/enum_trait   // each: the audio families the installer walks
require dasllama/dasllama_env
require dasllama/dasllama_gpu_tier   // gpu_want_cuda, register_gpu_home: the decoder seats follow the GPU home
require dasllama/dasllama_kqformat
require dasllama/dasllama_math
require dasllama/dasllama_layout
require dasllama/dasllama_plane
require dasllama/dasllama_whisper
require dasllama/dasllama_rnnt
require dasllama/dasllama_gigaam   // the CTC head's device seat
require dasllama/dasllama_gpu_kernels_common   // GkRnntArgs and the transducer stamps' staged widths
require dasllama/dasllama_vulkan_common
require dasllama/dasllama_vulkan_classes
require dasllama/dasllama_vulkan_tower
require dasllama/dasllama_tower_chains_fc   // asr_dec_rnnt_rounds, asr_dec_ctc: the transducer's and the CTC head's shared bodies
require dasllama/dasllama_asr_dec_chains   // the whisper decoder's shared bodies
require dasllama/dasllama_tower_image   // tower_bake_q8: the decoder's q8 block pieces
require dasllama/dasllama_tower

//! The whisper ASR-decoder Vulkan driver, q8-native: the per-window cross-KV and the decode step, each the shared body
//! (`dasllama_asr_dec_chains`) on the decoder's own Vulkan rail; the Metal ASR-decoder driver fills the same hooks on an Apple build.

var private g_wd_env = true
var private g_wd_declines : VkDeclineCounter
var private g_wd_windows = 0l
var private g_wd_handoffs = 0l   //! cross-KV windows whose encoder rows came device to device off the tower's plane
var private g_wd_steps = 0l
var private g_wd_tokens = 0l

//! the hazard bits of the driver's buffers
let private WD_X = 0x1u
let private WD_XQ = 0x4u
let private WD_QKV = 0x8u
let private WD_CQ = 0x10u
let private WD_YO = 0x40u
let private WD_H = 0x80u
let private WD_PARTIALS = 0x100u
let private WD_KC = 0x200u
let private WD_VC = 0x400u
let private WD_KXR = 0x800u
let private WD_VXR = 0x1000u
let private WD_LOGITS = 0x2000u
let private WD_XIN = 0x4000u
let private WD_XFEED = 0x8000u
let private WD_TMP = 0x10000u

struct private WdResident {
    key : uint64
    g : WdecGeom
    cm2 : bool
    xkv_tile : VtTile
    wq_dev : uint64
    ws_dev : uint64
    wq_bytes : int64
    ws_bytes : int64
    norms_dev : uint64    //! the layers' f32 rows (norms, biases) then the final norm's weight and bias (`wdec_norm_rows`)
    norms_bytes : int64
    norms_base : int64    //! the element the norms plane starts at in dblob
    kxr_dev : uint64
    vxr_dev : uint64
    kc_dev : uint64
    vc_dev : uint64
    x_dev : uint64        //! the step's rows: the residual stream, its Q8_0 feed (the row passes quantize their output in place of storing it)
    xq_dev : uint64
    xs_dev : uint64
    qkv_dev : uint64
    cq_dev : uint64
    yo_dev : uint64
    h_dev : uint64
    pms_dev : uint64
    py_dev : uint64
    logits_dev : uint64      //! [nvocab]: the last row's logits
    xin_dev : uint64      //! the cross-KV chain: the encoder rows, their feed, the projection output
    xh_dev : uint64
    xkv_q_dev : uint64
    xkv_s_dev : uint64
    xkv_proj_dev : uint64
    ta_tile_rows : int64           //! the cross-KV GEMM record's rows: `ta` rounded up to the tile's token column (`vt_tile_rows`); the projection plane holds them
    meta : HostBuf        //! the schedule (`wdec_meta`): the cross records, the GEMV records' region pairs
    xhost : HostBuf       //! the step's rows in, the logits out
    oom : bool
    served_said : bool
    miss_said : bool   //! the host-upload arm of the cross-KV chain said once: the tower landed no device plane for this window's rows
}

var private g_wd : WdResident
var private g_wd_rl : VtRail   //! the decoder's own rail: its seats, picks and schedule maps kept across windows and steps
var private g_wd_window_uid = 0ul     //! the live window: ONE DecoderState at a time, claimed by a served cross-KV
var private g_wd_window_live = false

//! Engage / decline counters: `windows` = cross-KV windows served, `steps` = decode batches, `tokens` = rows across them,
//! `declines` = every decline but the knob's (a pinned-off driver is policy, not a failure), `handoffs` = the served windows
//! whose encoder rows came off the tower's device plane, no host upload.
def vulkan_wdec_stats : tuple<windows : int64; steps : int64; tokens : int64; declines : int64; handoffs : int64> {
    let policy = (g_wd_declines.counts?["{WdecDecline.knob}"] ?? 0l)
    return (windows = g_wd_windows, steps = g_wd_steps, tokens = g_wd_tokens, declines = g_wd_declines.total - policy, handoffs = g_wd_handoffs)
}

//! The declines-by-reason table (a copy), keyed by `WdecDecline` value names.
def vulkan_wdec_declines : table<string; int64> => <- clone_to_move(g_wd_declines.counts)

//! Test A/B seat: pin the ASR-decoder driver on / off in-process (the env knob's setter twin).
def set_vulkan_wdec(on : bool) {
    g_wd_env = on
}

def vulkan_wdec_on : bool => g_wd_env

[cold_path]
def private wd_decline(why : WdecDecline) : bool {
    let words = wdec_decline_words(why, "Vulkan", "the decoder knob is off (DASLLAMA_VK_WDEC=0 or set_vulkan_wdec(false))", VK_DECLINE_WORDS_DEVICE, VK_DECLINE_WORDS_MEMORY)
    return g_wd_declines |> note("the whisper decoder", "{why}", "the decoder leaves the device - a CPU image decodes on the CPU chain, a Vulkan image refuses: {words}")
}

[cold_path]
def private wd_dev_buf(bytes : int64) : uint64 => soft_device_buf(g_wd.oom, bytes)

[cold_path]
def private wd_call_buf(bytes : int64) : uint64 => soft_call_buf(g_wd.oom, bytes)

[cold_path]
def private wd_host_buf(bytes : int64; cached : bool) : HostBuf => soft_host_buf(g_wd.oom, bytes, cached)

[cold_path]
def private wd_release {
    var r & = g_wd
    if (r.key != 0ul) {
        for (b in fixed_array(r.wq_dev, r.ws_dev, r.norms_dev, r.kxr_dev, r.vxr_dev, r.kc_dev, r.vc_dev, r.x_dev, r.xq_dev, r.xs_dev,
                              r.qkv_dev, r.cq_dev, r.yo_dev, r.h_dev, r.pms_dev, r.py_dev, r.logits_dev, r.xin_dev, r.xh_dev, r.xkv_q_dev,
                              r.xkv_s_dev, r.xkv_proj_dev)) {
            if (b != 0ul) {
                destroy_device_buf(b)
            }
        }
        destroy_host_buf(r.meta)
        destroy_host_buf(r.xhost)
    }
    wd_forget()
}

//! the handles forgotten (the model drop's sweep destroys the objects); the next served window uploads again
[cold_path]
def private wd_forget {
    var r & = g_wd
    r.key = 0ul
    r.oom = false
    r.served_said = false
    r.miss_said = false
    g_wd_window_uid = 0ul
    g_wd_window_live = false
}

//! the decoder's rail at the resident's planes: its lane, the cross tile, both sites' binds (`wdec_binds`), every seat, the schedule (`wdec_meta`) in the mapped plane
[cold_path]
def private wd_rail(w : WhisperModel) {
    let r & = g_wd
    var rl & = g_wd_rl
    let g = r.g
    let d = g.d
    let z = wdec_sizes(g)
    vt_rail_begin(rl, RailLane.q8, r.cm2, false, false)
    rl.norms_base = r.norms_base
    rl.sites[int(RailSite.cross)] = r.xkv_tile
    wdec_binds(rl, g)
    rail_seat(rl, TowerBuf.x, r.x_dev, z.bx, WD_X)
    rail_seat(rl, TowerBuf.xq, r.xq_dev, z.bq, WD_XQ)
    rail_seat(rl, TowerBuf.xs, r.xs_dev, z.bs, WD_XQ)
    rail_seat(rl, TowerBuf.qkv, r.qkv_dev, 3l * z.bx, WD_QKV)
    rail_seat(rl, TowerBuf.cq, r.cq_dev, z.bx, WD_CQ)
    rail_seat(rl, TowerBuf.yo, r.yo_dev, z.bx, WD_YO)
    rail_seat(rl, TowerBuf.hg, r.h_dev, z.bh, WD_H)
    rail_seat(rl, TowerBuf.pms, r.pms_dev, z.bpms, WD_PARTIALS)
    rail_seat(rl, TowerBuf.py, r.py_dev, z.bpy, WD_PARTIALS)
    rail_seat(rl, TowerBuf.kc, r.kc_dev, z.bkc, WD_KC)
    rail_seat(rl, TowerBuf.vc, r.vc_dev, z.bkc, WD_VC)
    rail_seat(rl, TowerBuf.kxr, r.kxr_dev, z.bkx, WD_KXR)
    rail_seat(rl, TowerBuf.vxr, r.vxr_dev, z.bkx, WD_VXR)
    rail_seat(rl, TowerBuf.logits, r.logits_dev, g.nvocab * 4l, WD_LOGITS)
    rail_seat(rl, TowerBuf.rows_out, r.xin_dev, z.bt, WD_XIN)
    rail_seat(rl, TowerBuf.enc_h, r.xh_dev, r.cm2 ? (r.ta_tile_rows + TILE_READ_SLACK) * d * 2l : 64l, WD_XFEED)
    rail_seat(rl, TowerBuf.enc_q, r.xkv_q_dev, r.cm2 ? 64l : g.ta * d, WD_XFEED)
    rail_seat(rl, TowerBuf.enc_s, r.xkv_s_dev, r.cm2 ? 64l : g.ta * d / kq_elems(KqFmt.q8) * 4l, WD_XFEED)
    rail_seat(rl, TowerBuf.enc_proj, r.xkv_proj_dev, r.ta_tile_rows * d * 4l, WD_TMP)
    rail_seat(rl, TowerBuf.norms, r.norms_dev, r.norms_bytes, 0u)
    rail_seat(rl, TowerBuf.wq, r.wq_dev, r.wq_bytes, 0u)
    rail_seat(rl, TowerBuf.ws, r.ws_dev, r.ws_bytes, 0u)
    rail_seat(rl, TowerBuf.meta, r.meta.buf, g.meta_bytes, 0u)
    wdec_meta(rl, w, g, unsafe(reinterpret<uint?>(r.meta.mapped)))
}

//! every class the two chains dispatch built ahead of the sets that bind them, or false
[cold_path]
def private wd_ensure_classes(cm2 : bool; tile : VtTile) : bool {
    return (gemv_cls_ensure(int(KqFmt.q8)) && gemv_cls_ensure_n(int(KqFmt.q8), WDEC_ROW_CAP) && vt_tile_ensure(tile)
        && ensure_tower_ln_rq_cls() && ensure_tower_post_add_ln_rq_cls() && ensure_tower_bias_act_rq_cls() && ensure_tower_clamp_rq_cls()
        && (!cm2 || ensure_tower_clamp_cvt_cls()) && ensure_tower_wdec_kv_cls() && ensure_tower_wdec_kv16_cls() && ensure_tower_wdec_attn_part_cls()
        && ensure_tower_wdec_attn_comb_rq_cls())
}

[cold_path]
def private wd_upload_weights(w : WhisperModel) {
    var r & = g_wd
    let src = vt_dev_src(w.enc)
    r.wq_bytes = vt_piece_bytes(src, "{WD_PIECE}.wq")
    r.ws_bytes = vt_piece_bytes(src, "{WD_PIECE}.ws")
    r.wq_dev = wd_dev_buf(r.wq_bytes)
    r.ws_dev = wd_dev_buf(r.ws_bytes)
    return if (r.oom)
    vt_put_piece(src, "{WD_PIECE}.wq", r.wq_dev)
    vt_put_piece(src, "{WD_PIECE}.ws", r.ws_dev)
}

let private WD_PIECE = "vk.wdec"   //! the whisper decoder's q8 pieces' prefix, in the encoder tower's device block

//! whisper's Vulkan device block (`#tower-dev-block`): the encoder tower's pieces, then the decoder's q8 regions gathered
[cold_path, arch(at = "../ARCHITECTURE_IMAGE_GPU_PLANES.md#tower-dev-block")]
def private vulkan_whisper_bake(w : WhisperModel; var b : TowerDevBuild) : bool {
    vulkan_audio_tower_bake_into(w.enc, b)
    var inscope regions <- wdec_regions(w)
    tower_bake_q8(b, WD_PIECE, w.dqblob, w.dqscales, regions)
    return true
}

//! whisper's Vulkan chains serve the encoder's lane (`q8`, else exact beside the q8 decoder): the encoder's and the decoder's knobs on
def private vulkan_whisper_serves_model(q8 : bool) : bool => g_wd_env && vulkan_whisper_tower_serves(q8)

[cold_path]
def private wd_upload_norms(w : WhisperModel) {
    var r & = g_wd
    r.norms_base = w.dlayers_off
    var inscope rows <- wdec_norm_rows(w, r.g)
    r.norms_bytes = long_length(rows) * 4l
    r.norms_dev = wd_dev_buf(r.norms_bytes)
    if (!r.oom) {
        upload_region_at(r.norms_dev, 0l, unsafe(addr<uint8 const?>(rows[0])), r.norms_bytes)
    }
}

[arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-asr-decoder")]
def private wd_attach(w : WhisperModel) : bool {
    let r & = g_wd
    let src = vt_dev_src(w.enc)
    assert(src.on, "the whisper decoder's attach reads a Vulkan image's pieces - the flavor decline runs ahead of it")
    let key = vt_key_dev(src, w.dblob, w.dlayers_off, w.n_text_layer * w.dlayer_stride, w.n_text_layer)
    if (r.key != 0ul && key == r.key) {
        return true
    }
    return wd_build(w, key)
}

//! the resident built for a model the key does not know: the old one released, the planes gathered and uploaded, the
//! scratch sized to the model, the sets bound
[cold_path]
def private wd_build(w : WhisperModel; key : uint64) : bool {
    var r & = g_wd
    wd_release()
    let q8e = kq_elems(KqFmt.q8)
    let d = w.n_text_state
    let nl = w.n_text_layer
    let ta = w.enc.n_ctx
    r.cm2 = vt_cm2_feed()
    r.xkv_tile = vt_tile(r.cm2, d, ta)
    r.ta_tile_rows = vt_tile_rows(r.xkv_tile, ta)
    r.g = wdec_geom(w, vt_tile_groups(r.xkv_tile, d, ta))
    r.oom = false
    if (!wd_ensure_classes(r.cm2, r.xkv_tile)) {
        return false
    }
    let src = vt_dev_src(w.enc)
    let wq_n = vt_piece_bytes(src, "{WD_PIECE}.wq")
    if (!vk_range_ok(wq_n) || !vk_range_ok(nl * d * ta * 4l) || !vk_range_ok(nl * d * r.g.tmax * 2l) || !vk_range_ok(2l * nl * d * ta * 4l)) {
        return wd_decline(WdecDecline.shape)
    }
    r.key = key
    wd_upload_weights(w)
    wd_upload_norms(w)
    let z = wdec_sizes(r.g)
    r.kxr_dev = wd_dev_buf(z.bkx)
    r.vxr_dev = wd_dev_buf(z.bkx)
    r.kc_dev = wd_dev_buf(z.bkc)
    r.vc_dev = wd_dev_buf(z.bkc)
    r.x_dev = wd_call_buf(z.bx)
    r.xq_dev = wd_call_buf(z.bq)
    r.xs_dev = wd_dev_buf(z.bs)
    r.qkv_dev = wd_dev_buf(3l * z.bx)
    r.cq_dev = wd_dev_buf(z.bx)
    r.yo_dev = wd_dev_buf(z.bx)
    r.h_dev = wd_dev_buf(z.bh)
    r.pms_dev = wd_dev_buf(z.bpms)
    r.py_dev = wd_dev_buf(z.bpy)
    r.logits_dev = wd_dev_buf(r.g.nvocab * 4l)
    r.xin_dev = wd_call_buf(z.bt)
    if (r.cm2) {
        r.xh_dev = wd_dev_buf((r.ta_tile_rows + TILE_READ_SLACK) * d * 2l)
        r.xkv_q_dev = wd_dev_buf(64l)
        r.xkv_s_dev = wd_dev_buf(64l)
    } else {
        r.xh_dev = wd_dev_buf(64l)
        r.xkv_q_dev = wd_dev_buf(ta * d)
        r.xkv_s_dev = wd_dev_buf(ta * d / q8e * 4l)
    }
    r.xkv_proj_dev = wd_dev_buf(r.ta_tile_rows * d * 4l)
    r.meta = wd_host_buf(r.g.meta_bytes, false)
    r.xhost = wd_host_buf(max(z.bx, r.g.nvocab * 4l), true)
    if (r.oom) {
        wd_release()
        return false
    }
    wd_rail(w)
    return true
}

[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#whisper-decoder")]
def private vulkan_whisper_cross_kv(w : WhisperModel; var ds : DecoderState; enc_out : array<float>) : bool {
    let uid = wdec_uid(ds)
    g_wd_window_live = false
    ds.gpu_kv_gen = 0l
    return false if (!wdec_gate(w, g_wd_env, TowerDev.vulkan, $(why : WdecDecline) => wd_decline(why)))
    return wd_decline(WdecDecline.device) if (!vulkan_tower_arms())
    return wd_decline(WdecDecline.align) if (!vk_rail_view_aligned())
    if (!wd_attach(w)) {
        return wd_decline(g_wd.oom ? WdecDecline.memory : WdecDecline.device)
    }
    var r & = g_wd
    var rl & = g_wd_rl
    let bt = wdec_sizes(r.g).bt
    let enc_dev = vulkan_tower_enc_out(unsafe(addr<float const?>(enc_out[0])), bt)
    if (enc_dev == 0ul) {
        upload_region_at(r.xin_dev, 0l, unsafe(addr<uint8 const?>(enc_out[0])), bt)
        wdec_miss_say(r.miss_said, "vulkan") if (!r.miss_said)
    } else {
        g_wd_handoffs++
        rail_seat(rl, TowerBuf.xb, enc_dev, bt, 0u)
    }
    vt_rail_one_shot(rl, "whisper cross-kv", r.g.nl, r.g.ta) $ {
        asr_dec_whisper_cross_kv(rl, w, r.g, enc_dev != 0ul)
    }
    g_wd_windows++
    g_wd_window_uid = uid
    g_wd_window_live = true
    wdec_announce(r.served_said, "vulkan", r.g) if (!r.served_said)
    return true
}

[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#whisper-decoder")]
def private vulkan_whisper_decode(w : WhisperModel; var ds : DecoderState; toks : int const?; n : int64) : bool {
    return false if (!wdec_step_admits(ds, g_wd_window_live, g_wd_window_uid, n, "vulkan", "Vulkan"))
    var r & = g_wd
    var rl & = g_wd_rl
    let g = r.g
    let p0 = ds.n_past
    unsafe {
        whisper_embed_rows(w, toks, n, p0, reinterpret<float?>(r.xhost.mapped))
        var mp = reinterpret<uint?>(r.meta.mapped)
        mp[wdec_logits_word(g)] = wdec_logits_row(g, n)
    }
    upload_region_at(r.x_dev, 0l, unsafe(reinterpret<uint8 const?>(r.xhost.mapped)), n * g.d * 4l)
    vt_rail_one_shot(rl, "whisper decode", g.nl, n) $ {
        asr_dec_whisper_step(rl, w, g, n, p0)
        vhz_dep(rl.raw, rl.h, WD_LOGITS, 0u, true)
        cmd_copy_range(rl.raw, r.logits_dev, 0l, r.xhost.buf, 0l, g.nvocab * 4l)
        vt_pt(rl.raw, RailProfRole.copy_out)
    }
    host_buf_read(r.xhost, unsafe(addr<void?>(ds.logits[0])), g.nvocab * 4l)
    g_wd_steps++
    g_wd_tokens += n
    ds.gpu_kv_gen = 1l
    return true
}

// ===== the transducer greedy decode (parakeet, GigaAM's RNNT head) =====

//! the hazard bits of the transducer seat's buffers
let private RV_ST = 0x1u
let private RV_HB = 0x2u
let private RV_CB = 0x4u
let private RV_XIN = 0x8u
let private RV_PRED = 0x10u
let private RV_LG = 0x20u
let private RV_TOKS = 0x40u
let private RV_ENCP = 0x80u
let private RV_ROWS = 0x100u

//! The transducer seat's device objects: the weights laid once a model (`rnnt_dev_key`: the state's halves `wh` and joint columns
//! `jw`, the slab `wf` at `lay`'s offsets, the encoder projection's f32 rows `wenc`), and the per-clip planes sized to the longest
//! clip yet.
struct private RvResident {
    key : uint64
    lay : RnntDevLayout
    wh_dev : uint64
    jw_dev : uint64
    wf_dev : uint64
    wenc_dev : uint64
    wh_bytes : int64
    jw_bytes : int64
    wf_bytes : int64
    wenc_bytes : int64
    cap_frames : int64     //! the per-clip planes' frames
    cap_shape : uint64     //! the net shape the per-clip planes are sized for
    st_dev : uint64
    hb_dev : uint64
    cb_dev : uint64
    xin_dev : uint64
    pred_dev : uint64
    lg_dev : uint64
    toks_dev : uint64
    encp_dev : uint64
    rows_dev : uint64
    host : HostBuf         //! the state words, then the token records
    st_bytes : int64
    hb_bytes : int64
    cb_bytes : int64
    xin_bytes : int64
    pred_bytes : int64
    lg_bytes : int64
    toks_bytes : int64
    encp_bytes : int64
    rows_bytes : int64
    oom : bool
    served_said : bool
    miss_said : bool       //! the handoff miss said: a projecting decode uploaded its rows
}

var private g_rv : RvResident
var private @scratch g_rv_words : array<uint>   //! the token records read back
var private g_rv_env = true
var private g_rv_declines : VkDeclineCounter
var private g_rv_decodes = 0l
var private g_rv_handoffs = 0l
var private g_rv_projects = 0l
var private g_rv_rounds = 0l
var private g_rv_steps = 0l
var private g_rv_tokens = 0l
var private g_rv_cbs = 0l
var private g_rv_uploads = 0l   //! the weight uploads: one a model, again only where another net takes the seat
var private g_rv_rate = RNNT_DEV_RATE0   //! the rounds a frame the last served clip took: the first command buffer's estimate

//! Engage counters: `decodes` served on the device, `handoffs` among them off the encode's device rows, `projects` among them
//! projected on the device, `rounds` / `steps` the picks' rounds and predictor steps, `tokens`, `cbs` the command buffers,
//! `uploads` the weight uploads, `declines` every decline but the knob's.
def vulkan_rnnt_stats : tuple<decodes : int64; handoffs : int64; projects : int64; rounds : int64; steps : int64; tokens : int64; cbs : int64; uploads : int64; declines : int64> {
    return (decodes = g_rv_decodes, handoffs = g_rv_handoffs, projects = g_rv_projects, rounds = g_rv_rounds, steps = g_rv_steps,
        tokens = g_rv_tokens, cbs = g_rv_cbs, uploads = g_rv_uploads, declines = g_rv_declines.total - (g_rv_declines.counts?["{RnntDevDecline.knob}"] ?? 0l))
}

//! The transducer seat's declines-by-reason table (a copy), keyed by the decline's name, the knob's among them.
def vulkan_rnnt_declines : table<string; int64> => <- clone_to_move(g_rv_declines.counts)

//! Pin the transducer decode on the device on/off in-process (DASLLAMA_VK_RNNT's twin); answers the setting it replaced.
def set_vulkan_rnnt(on : bool) : bool {
    let was = g_rv_env
    g_rv_env = on
    return was
}

[cold_path]
def private rv_decline(why : RnntDevDecline) : bool {
    let words = rnnt_dev_decline_words(why, "Vulkan", "the transducer knob is off (DASLLAMA_VK_RNNT=0 or set_vulkan_rnnt(false))", VK_DECLINE_WORDS_DEVICE, VK_DECLINE_WORDS_MEMORY)
    return g_rv_declines |> note("the transducer decode", "{why}", "the decode stays on the CPU loop: {words}")
}

[cold_path]
def private rv_dev_buf(bytes : int64) : uint64 => soft_device_buf(g_rv.oom, bytes)

[cold_path]
def private rv_call_buf(bytes : int64) : uint64 => soft_call_buf(g_rv.oom, bytes)

[cold_path]
def private rv_release_scratch {
    var r & = g_rv
    release_device_buf(r.st_dev)
    release_device_buf(r.hb_dev)
    release_device_buf(r.cb_dev)
    release_device_buf(r.xin_dev)
    release_device_buf(r.pred_dev)
    release_device_buf(r.lg_dev)
    release_device_buf(r.toks_dev)
    release_device_buf(r.encp_dev)
    release_device_buf(r.rows_dev)
    if (r.host.buf != 0ul) {
        destroy_host_buf(r.host)
    }
    r.host = HostBuf()
    r.cap_frames = 0l
    r.cap_shape = 0ul
}

[cold_path]
def private rv_release {
    var r & = g_rv
    rv_release_scratch()
    release_device_buf(r.wh_dev)
    release_device_buf(r.jw_dev)
    release_device_buf(r.wf_dev)
    release_device_buf(r.wenc_dev)
    rv_forget()
}

//! the handles forgotten (the model drop's sweep destroys the objects); the next served decode uploads again and says its
//! declines afresh
[cold_path]
def private rv_forget {
    g_rv = RvResident()
    g_rv_rate = RNNT_DEV_RATE0
    g_rv_declines |> unsay("the transducer decode")
}

//! the weights laid once a weight plane, as the Metal seat lays them
def private rv_attach(net : RnntNet; enc : RnntEnc) : bool {
    let key = rnnt_dev_key(net, enc)
    return true if (g_rv.key == key && g_rv.wh_dev != 0ul)
    return rv_attach_build(net, enc, key)
}

//! a new net's weights uploaded under `key` - its halves and slab the Vulkan image's pieces as stored (`#tower-dev-block`) - the
//! old seat released and its said declines forgotten
[cold_path]
def private rv_attach_build(net : RnntNet; enc : RnntEnc; key : uint64) : bool {
    var r & = g_rv
    rv_release()
    r.oom = false
    r.lay = rnnt_dev_layout(net)
    r.wh_bytes = r.lay.wh_halves * 2l
    r.jw_bytes = r.lay.jw_halves * 2l
    r.wf_bytes = r.lay.wf_floats * 4l
    if (!rnnt_dev_planes_ok(net, r.lay)) {
        return rv_decline(RnntDevDecline.shape)
    }
    r.wenc_bytes = enc.q8 ? 0l : net.joint_dim * enc.d * 4l
    if (!vk_range_ok(max(max(r.wh_bytes, r.wf_bytes), max(r.jw_bytes, r.wenc_bytes)))) {
        return rv_decline(RnntDevDecline.shape)
    }
    r.wh_dev = rv_dev_buf(r.wh_bytes)
    r.jw_dev = rv_dev_buf(r.jw_bytes)
    r.wf_dev = rv_dev_buf(r.wf_bytes)
    r.wenc_dev = enc.q8 ? 0ul : rv_dev_buf(r.wenc_bytes)
    r.oom = r.oom || rnnt_dev_faulted(RnntDevFault.memory)
    if (r.oom) {
        rv_release()
        return rv_decline(RnntDevDecline.memory)
    }
    upload_region_at(r.wh_dev, 0l, unsafe(reinterpret<uint8 const?>(plane_at(net.wh, 0l))), r.wh_bytes)
    upload_region_at(r.jw_dev, 0l, unsafe(reinterpret<uint8 const?>(plane_at(net.jw, 0l))), r.jw_bytes)
    upload_region_at(r.wf_dev, 0l, unsafe(reinterpret<uint8 const?>(plane_at(net.blob, net.embed))), r.wf_bytes)
    if (!enc.q8) {
        upload_region_at(r.wenc_dev, 0l, unsafe(reinterpret<uint8 const?>(plane_at(net.blob, enc.w))), r.wenc_bytes)
    }
    r.key = key
    g_rv_uploads++
    return true
}

//! the per-clip planes for `n_frames` (a whole 64-frame step past the largest clip yet) and the round's four sets
def private rv_scratch(net : RnntNet; n_frames, n_logits, d : int64) : bool {
    let shape = hash_combine64(hash_combine64(uint64(net.pred_dim), uint64(net.joint_dim)),
        hash_combine64(uint64(net.n_layers), hash_combine64(uint64(n_logits), uint64(d))))
    return true if (g_rv.cap_frames >= n_frames && g_rv.cap_shape == shape && g_rv.st_dev != 0ul)
    return rv_scratch_build(net, n_frames, n_logits, d, shape)
}

[cold_path]
def private rv_scratch_build(net : RnntNet; n_frames, n_logits, d : int64; shape : uint64) : bool {
    var r & = g_rv
    let pd = net.pred_dim
    let jd = net.joint_dim
    rv_release_scratch()
    let cap = (n_frames + 63l) / 64l * 64l
    r.st_bytes = int64(GK_RNNT_ST_WORDS) * 4l
    r.hb_bytes = 2l * net.n_layers * pd * 4l
    r.cb_bytes = net.n_layers * pd * 4l
    r.xin_bytes = pd * 4l
    r.pred_bytes = jd * 4l
    r.lg_bytes = int64(GK_RNNT_FRAMES) * rnnt_joint_blocks(n_logits) * RNNT_JOINT_BLOCK * 4l
    r.toks_bytes = rnnt_dev_cap(net, cap) * int64(GK_RNNT_TOK_WORDS) * 4l
    r.encp_bytes = cap * jd * 4l
    r.rows_bytes = cap * d * 4l
    if (!vk_range_ok(max(max(r.toks_bytes, r.lg_bytes), max(r.encp_bytes, r.rows_bytes)))) {
        return rv_decline(RnntDevDecline.shape)
    }
    r.oom = false
    r.st_dev = rv_call_buf(r.st_bytes)
    r.hb_dev = rv_call_buf(r.hb_bytes)
    r.cb_dev = rv_call_buf(r.cb_bytes)
    r.xin_dev = rv_call_buf(r.xin_bytes)
    r.pred_dev = rv_dev_buf(r.pred_bytes)
    r.lg_dev = rv_dev_buf(r.lg_bytes)
    r.toks_dev = rv_dev_buf(r.toks_bytes)
    r.encp_dev = rv_call_buf(r.encp_bytes)
    r.rows_dev = d > 0l ? rv_call_buf(r.rows_bytes) : 0ul   //! a net with no encoder half has no rows, and a 0-byte buffer faults the driver
    r.host = soft_host_buf(r.oom, max(r.st_bytes + r.toks_bytes, r.hb_bytes + r.cb_bytes + r.xin_bytes), true)
    if (r.oom) {
        rv_release_scratch()
        return rv_decline(RnntDevDecline.memory)
    }
    r.cap_frames = cap
    r.cap_shape = shape
    return true
}

//! The clip's start uploaded: the state words (the first round's predictor step asked, the duration table), the hidden pair and
//! the cells zeroed, the first input - zeros, or the blank's embedding row.
def private rv_state_init(net : RnntNet) {
    var r & = g_rv
    rnnt_dev_state_words(net, unsafe(reinterpret<uint?>(r.host.mapped)))
    upload_region_at(r.st_dev, 0l, unsafe(reinterpret<uint8 const?>(r.host.mapped)), r.st_bytes)
    unsafe {
        var zp = reinterpret<float?>(r.host.mapped)
        for (i in range64((r.hb_bytes + r.cb_bytes) / 4l)) {
            zp[i] = 0.0
        }
        rnnt_dev_first_input(net, zp + (r.hb_bytes + r.cb_bytes) / 4l)
        let hp = reinterpret<uint8 const?>(r.host.mapped)
        upload_region_at(r.hb_dev, 0l, hp, r.hb_bytes)
        upload_region_at(r.cb_dev, 0l, hp + r.hb_bytes, r.cb_bytes)
        upload_region_at(r.xin_dev, 0l, hp + r.hb_bytes + r.cb_bytes, r.xin_bytes)
    }
}

var private g_rv_rl : VtRail   //! the transducer seat's rail: its planes seated a decode

//! the transducer seat's planes on its rail: the weights, the state and the per-clip planes, the encoder rows at `rows_dev`
def private rv_rail_seats(var rl : VtRail; r : RvResident; rows_dev : uint64; rows_bytes : int64) {
    vt_rail_begin(rl, RailLane.q8, vt_cm2_feed(), false, false)
    rail_seat(rl, TowerBuf.dec_state, r.st_dev, r.st_bytes, RV_ST)
    rail_seat(rl, TowerBuf.pred_w16, r.wh_dev, r.wh_bytes, 0u)
    rail_seat(rl, TowerBuf.joint_w16, r.jw_dev, r.jw_bytes, 0u)
    rail_seat(rl, TowerBuf.net_rows, r.wf_dev, r.wf_bytes, 0u)
    rail_seat(rl, TowerBuf.enc_proj_w, r.wenc_dev, r.wenc_bytes, 0u)
    rail_seat(rl, TowerBuf.pred_in, r.xin_dev, r.xin_bytes, RV_XIN)
    rail_seat(rl, TowerBuf.lstm_h, r.hb_dev, r.hb_bytes, RV_HB)
    rail_seat(rl, TowerBuf.lstm_c, r.cb_dev, r.cb_bytes, RV_CB)
    rail_seat(rl, TowerBuf.pred_out, r.pred_dev, r.pred_bytes, RV_PRED)
    rail_seat(rl, TowerBuf.logits, r.lg_dev, r.lg_bytes, RV_LG)
    rail_seat(rl, TowerBuf.picks, r.toks_dev, r.toks_bytes, RV_TOKS)
    rail_seat(rl, TowerBuf.enc_proj, r.encp_dev, r.encp_bytes, RV_ENCP)
    rail_seat(rl, TowerBuf.rows_out, rows_dev, rows_bytes, RV_ROWS)
}

//! Whether the Vulkan seat projects a lane's encoder half: the f32 plane's, through the exact f32 tile.
[hot_path]
def private vulkan_rnnt_projects(enc : RnntEnc) : bool => g_rv_env && !enc.q8

//! The transducer's greedy decode on the device, the Metal seat's form on the Vulkan stamps: the joint's encoder projection off
//! the encode's own device rows (the exact f32 tile) or the family's projection uploaded, then command buffers of rounds until
//! the pick marks the clip done; the state words read back a command buffer, the token records once.
[hot_path, unused_argument(s), arch(at="../ARCHITECTURE_MEDIA_ASR.md#rnnt-device-decode")]
def private vulkan_rnnt_greedy(net : RnntNet; var s : RnntState; enc : RnntEnc; x, encp : array<float>; n_frames : int64;
                               @scratch var toks : array<RnntToken>) : bool {
    if (!g_rv_env) {
        return rv_decline(RnntDevDecline.knob)
    }
    return rv_decline(RnntDevDecline.flavor) if (!net.dev_only)
    let jd = net.joint_dim
    let d = enc.d
    let n_logits = rnnt_n_logits(net)
    let projects = !enc.q8
    if (!rnnt_dev_shape_ok(net, enc, 1l)) {
        return rv_decline(RnntDevDecline.shape)
    }
    if (!vulkan_tower_arms() || !ensure_rnnt_gates_cls() || !ensure_rnnt_proj_cls() || !ensure_rnnt_joint_cls() || !ensure_rnnt_pick_cls()
            || (projects && !ensure_f32_gemm_cls()) || rnnt_dev_faulted(RnntDevFault.device)) {
        return rv_decline(RnntDevDecline.device)
    }
    if (!rv_attach(net, enc) || !rv_scratch(net, max(n_frames, 1l), n_logits, d)) {
        return false
    }
    if (n_frames <= 0l) {
        g_rv_decodes++
        return true
    }
    var r & = g_rv
    rv_state_init(net)
    var rows_dev = 0ul
    var handoff = false
    if (projects) {
        rows_dev = vulkan_tower_fc_rows(enc.gen, n_frames * d * 4l)
        handoff = rows_dev != 0ul
        if (!handoff) {
            rnnt_dev_miss_say(r.miss_said, "vulkan", enc.gen) if (!r.miss_said)
            upload_region_at(r.rows_dev, 0l, unsafe(addr<uint8 const?>(x[0])), n_frames * d * 4l)
            rows_dev = r.rows_dev
        }
    } else {
        upload_region_at(r.encp_dev, 0l, unsafe(addr<uint8 const?>(encp[0])), n_frames * jd * 4l)
    }
    var rl & = g_rv_rl
    rv_rail_seats(rl, r, rows_dev, n_frames * d * 4l)
    let ka = rnnt_dev_args(net, r.lay, n_frames, projects)
    let run = rnnt_dev_rounds(net, n_frames, unsafe(reinterpret<uint const?>(r.host.mapped)), g_rv_rate) $(rounds : int64; first : bool) : bool {
        vt_rail_one_shot(rl, "rnnt decode", net.n_layers, n_frames) $ {   // a failed submit panics in the chain's own check
            asr_dec_rnnt_rounds(rl, net, r.lay, ka, d, n_frames, rounds, first && projects)
            vhz_dep(rl.raw, rl.h, RV_ST, 0u, true)
            cmd_copy_range(rl.raw, r.st_dev, 0l, r.host.buf, 0l, r.st_bytes)
        }
        return true
    }
    g_rv_cbs += run.cbs
    if (!run.ok) {
        return rv_decline(RnntDevDecline.overrun)
    }
    let ntok = run.p.ntok
    g_rv_words |> resize(ntok * int(GK_RNNT_TOK_WORDS))
    if (ntok > 0) {
        readback_range(r.toks_dev, RV_TOKS, r.host, unsafe(addr<float?>(g_rv_words[0])), long_length(g_rv_words) * 4l)
        rnnt_dev_land_tokens(unsafe(addr<uint const?>(g_rv_words[0])), ntok, toks)
    }
    rnnt_dev_announce(r.served_said, "vulkan", net.pred_dim, jd, n_logits, projects) if (!r.served_said)
    g_rv_decodes++
    g_rv_tokens += int64(ntok)
    g_rv_rounds += run.p.rounds
    g_rv_steps += run.p.steps
    if (projects) {
        g_rv_projects++
    }
    if (handoff) {
        g_rv_handoffs++
    }
    return true
}

// ===== GigaAM's CTC head: the head GEMM over the head's own plane and the pick =====

//! the hazard bits of the CTC seat's buffers
let private CV_ROWS = 0x1u
let private CV_XH = 0x2u
let private CV_LG = 0x4u
let private CV_PK = 0x8u

//! The CTC seat's device objects: the head's planes laid once a model (`key`: the f16 head's padded halfword rows off ``wblob``,
//! or the f32 head's rows off ``blob``, and the bias row), and the per-clip planes sized to the longest clip yet.
struct private CvResident {
    key : uint64
    w_dev : uint64
    b_dev : uint64
    w_bytes : int64
    b_bytes : int64
    cap_frames : int64
    cap_shape : uint64
    rows_dev : uint64      //! the encoder rows uploaded where no device encode of their generation is held
    xh_dev : uint64        //! the f16 head's half feed, [cap + TILE_READ_SLACK][d]
    lg_dev : uint64        //! the head GEMM's rows [cap][ncp]
    pk_dev : uint64        //! a frame's record [cap][2]: the class, plog's bits
    host : HostBuf
    rows_bytes : int64
    xh_bytes : int64
    lg_bytes : int64
    pk_bytes : int64
    oom : bool
    served_said : bool
    miss_said : bool
}

var private g_cv : CvResident
var private g_cv_rl : VtRail   //! the CTC seat's rail: its planes seated a head
var private g_cv_declines : VkDeclineCounter
var private g_cv_heads = 0l      //! CTC heads served on the device
var private g_cv_handoffs = 0l   //! ... among them off the encode's own device rows
var private g_cv_uploads = 0l

//! Engage counters: `heads` served on the device, `handoffs` among them off the encode's device rows, `uploads` the head
//! uploads, `declines` every decline but the knob's.
def vulkan_ctc_stats : tuple<heads : int64; handoffs : int64; uploads : int64; declines : int64> {
    return (heads = g_cv_heads, handoffs = g_cv_handoffs, uploads = g_cv_uploads,
        declines = g_cv_declines.total - (g_cv_declines.counts?["{CtcDevDecline.knob}"] ?? 0l))
}

[cold_path]
def private cv_decline(why : CtcDevDecline) : bool {
    let words = ctc_dev_decline_words(why, "Vulkan", VK_DECLINE_WORDS_TOWER_KNOB, VK_DECLINE_WORDS_DEVICE, VK_DECLINE_WORDS_MEMORY)
    return g_cv_declines |> note("the CTC head", "{why}", "the head stays on the CPU: {words}")
}

[cold_path]
def private cv_release_scratch {
    var r & = g_cv
    release_device_buf(r.rows_dev)
    release_device_buf(r.xh_dev)
    release_device_buf(r.lg_dev)
    release_device_buf(r.pk_dev)
    if (r.host.buf != 0ul) {
        destroy_host_buf(r.host)
    }
    r.host = HostBuf()
    r.cap_frames = 0l
    r.cap_shape = 0ul
}

[cold_path]
def private cv_release {
    cv_release_scratch()
    release_device_buf(g_cv.w_dev)
    release_device_buf(g_cv.b_dev)
    cv_forget()
}

//! the handles forgotten (the model drop's sweep destroys the objects); the next served head uploads again
[cold_path]
def private cv_forget {
    g_cv = CvResident()
    g_cv_declines |> unsay("the CTC head")
}

//! the head's padded rows: the f16 head's `ncp` halfword rows (the zero pad rows the mint wrote among them), or the f32 head's
//! `nc` rows
def private cv_head_n(m : GigaamModel) : int64 => (m.ctc_f16 ? round_up(m.n_classes, GIGAAM_CTC_ROW_TILE) : m.n_classes) * m.d_model

//! the head's planes uploaded once a model, as the image holds them
def private cv_attach(m : GigaamModel) : bool {
    let nw = cv_head_n(m)
    var key = hash_combine64(m.ctc_f16 ? intptr(unsafe(reinterpret<void?>(m.wblob.p))) : intptr(unsafe(reinterpret<void?>(m.blob.p))),
        hash_combine64(uint64(m.ctc_w), uint64(m.ctc_b)))
    key = hash_combine64(key, hash_combine64(uint64(nw), uint64(m.n_classes)))
    return true if (g_cv.key == key && g_cv.w_dev != 0ul)
    return cv_attach_build(m, key, nw)
}

[cold_path]
def private cv_attach_build(m : GigaamModel; key : uint64; nw : int64) : bool {
    var r & = g_cv
    cv_release()
    r.oom = false
    r.w_bytes = nw * (m.ctc_f16 ? 2l : 4l)
    r.b_bytes = m.n_classes * 4l
    if (!vk_range_ok(r.w_bytes)) {
        return cv_decline(CtcDevDecline.shape)
    }
    r.w_dev = soft_device_buf(r.oom, r.w_bytes)
    r.b_dev = soft_device_buf(r.oom, r.b_bytes)
    if (r.oom) {
        cv_release()
        return cv_decline(CtcDevDecline.memory)
    }
    if (m.ctc_f16) {
        upload_region_at(r.w_dev, 0l, unsafe(reinterpret<uint8 const?>(plane_at(m.wblob, m.ctc_w))), r.w_bytes)
    } else {
        upload_region_at(r.w_dev, 0l, unsafe(reinterpret<uint8 const?>(plane_at(m.blob, m.ctc_w))), r.w_bytes)
    }
    upload_region_at(r.b_dev, 0l, unsafe(reinterpret<uint8 const?>(plane_at(m.blob, m.ctc_b))), r.b_bytes)
    r.key = key
    g_cv_uploads++
    return true
}

//! the per-clip planes for `n_frames` (a whole 64-frame step past the largest clip yet)
def private cv_scratch(n_frames, ncp, d : int64; f16 : bool) : bool {
    let shape = hash_combine64(hash_combine64(uint64(ncp), uint64(d)), uint64(f16 ? 1 : 0))
    return true if (g_cv.cap_frames >= n_frames && g_cv.cap_shape == shape && g_cv.lg_dev != 0ul)
    return cv_scratch_build(n_frames, ncp, d, f16, shape)
}

[cold_path]
def private cv_scratch_build(n_frames, ncp, d : int64; f16 : bool; shape : uint64) : bool {
    var r & = g_cv
    cv_release_scratch()
    let cap = (n_frames + 63l) / 64l * 64l
    r.rows_bytes = cap * d * 4l
    r.xh_bytes = f16 ? (cap + TILE_READ_SLACK) * d * 2l : 0l
    r.lg_bytes = cap * ncp * 4l
    r.pk_bytes = cap * 2l * 4l
    if (!vk_range_ok(max(r.rows_bytes, r.lg_bytes))) {
        return cv_decline(CtcDevDecline.shape)
    }
    r.oom = false
    r.rows_dev = soft_call_buf(r.oom, r.rows_bytes)
    r.xh_dev = f16 ? soft_device_buf(r.oom, r.xh_bytes) : 0ul
    r.lg_dev = soft_device_buf(r.oom, r.lg_bytes)
    r.pk_dev = soft_device_buf(r.oom, r.pk_bytes)
    r.host = soft_host_buf(r.oom, r.pk_bytes, true)
    if (r.oom) {
        cv_release_scratch()
        return cv_decline(CtcDevDecline.memory)
    }
    r.cap_frames = cap
    r.cap_shape = shape
    return true
}

//! GigaAM's CTC head as ONE command buffer on the Vulkan stamps (`#gigaam-planes`): the encode's device rows by generation (else
//! ``st.x`` uploaded), the head GEMM over its own plane, then the pick (`CtcPick`) - each frame's biased argmax and plog read
//! back into ``st.picks`` / ``st.plogs``. False declines.
[hot_path, arch(at="../ARCHITECTURE_MEDIA_ASR.md#gigaam-planes")]
def private vulkan_gigaam_ctc(m : GigaamModel; var st : GigaamState; n_frames : int64) : bool {
    if (!vulkan_tower_on()) {
        return cv_decline(CtcDevDecline.knob)
    }
    if (m.dev_backend != TowerDev.vulkan) {
        return cv_decline(CtcDevDecline.flavor)
    }
    let d = m.d_model
    let nc = m.n_classes
    let ncp = round_up(nc, GIGAAM_CTC_ROW_TILE)
    let f16 = m.ctc_f16
    if (n_frames < 1l || d % 64l != 0l || long_length(st.x) < n_frames * d) {
        return cv_decline(CtcDevDecline.shape)
    }
    let khr = f16_gemm_khr()
    if (!vulkan_tower_arms() || !ensure_ctc_pick_cls() || (f16 ? !(f16_gemm_on() && ensure_tower_clamp_cvt_cls() && f16_gemm_ensure(khr))
            : !ensure_f32_gemm_cls())) {
        return cv_decline(CtcDevDecline.device)
    }
    if (!cv_attach(m) || !cv_scratch(n_frames, ncp, d, f16)) {
        return false
    }
    var r & = g_cv
    let bx = n_frames * d * 4l
    var rows_dev = vulkan_tower_fc_rows(st.enc_gen, bx)
    let handoff = rows_dev != 0ul
    if (!handoff) {
        ctc_dev_miss_say(r.miss_said, "vulkan", st.enc_gen) if (!r.miss_said)
        upload_region_at(r.rows_dev, 0l, unsafe(addr<uint8 const?>(st.x[0])), bx)
        rows_dev = r.rows_dev
    }
    let pk_n = n_frames * 2l * 4l
    var rl & = g_cv_rl
    vt_rail_begin(rl, RailLane.q8, vt_cm2_feed(), false, khr)
    rail_seat(rl, TowerBuf.rows_out, rows_dev, bx, CV_ROWS)
    rail_seat(rl, TowerBuf.xh, r.xh_dev, r.xh_bytes, CV_XH)
    rail_seat(rl, TowerBuf.head_w, r.w_dev, r.w_bytes, 0u)
    rail_seat(rl, TowerBuf.head_b, r.b_dev, r.b_bytes, 0u)
    rail_seat(rl, TowerBuf.logits, r.lg_dev, r.lg_bytes, CV_LG)
    rail_seat(rl, TowerBuf.picks, r.pk_dev, r.pk_bytes, CV_PK)
    vt_rail_one_shot(rl, "gigaam ctc", 1l, n_frames) $ {
        asr_dec_ctc(rl, f16, d, nc, ncp, n_frames)
        vhz_dep(rl.raw, rl.h, CV_PK, 0u, true)
        cmd_copy_range(rl.raw, r.pk_dev, 0l, r.host.buf, 0l, pk_n)
    }
    unsafe {
        let pp = reinterpret<uint const?>(r.host.mapped)
        for (t in range64(n_frames)) {
            st.picks[t] = int(pp[2l * t])
            st.plogs[t] = reinterpret<float>(pp[2l * t + 1l])
        }
    }
    ctc_dev_announce(r.served_said, "vulkan", nc, d, f16) if (!r.served_said)
    g_cv_heads++
    if (handoff) {
        g_cv_handoffs++
    }
    return true
}

//! the Vulkan home's decoder seats of family `fam` (whisper's cross-KV, step and flavor, gigaam's CTC head, the transducer): run here for every family, by CUDA for a row naming this home
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#rail-audio-installs")]
def vulkan_asr_dec_install_of(fam : TowerAudioFamily) {
    static_if (!typeinfo builtin_module_exists(das_metal)) {
        if (fam == TowerAudioFamily.whisper) {
            register_whisper_cross_kv_gpu(@@vulkan_whisper_cross_kv)
            register_whisper_decode_gpu(@@vulkan_whisper_decode)
            register_whisper_flavor(TowerDev.vulkan, @@vulkan_whisper_serves_model, @@vulkan_whisper_bake)
        } elif (fam == TowerAudioFamily.gigaam) {
            register_gigaam_gpu_ctc(@@vulkan_gigaam_ctc)
        } elif (fam == TowerAudioFamily.transducer) {
            register_rnnt_gpu(@@vulkan_rnnt_greedy, @@vulkan_rnnt_projects)
        }
    }
}

//! the Vulkan home's decoder seats at [init] unless the CUDA home is asked for, or when `set_gpu_home("vulkan")` moves the tier here
def vulkan_asr_dec_install {
    for (fam in each(type<TowerAudioFamily>)) {
        vulkan_asr_dec_install_of(fam)
    }
}

[init]
def dasllama_vulkan_asr_dec_register {
    g_wd_env = g_env_vulkan.vk_wdec
    g_rv_env = g_env_vulkan.vk_rnnt
    register_vk_drop_hook(@@wd_forget)
    register_vk_drop_hook(@@rv_forget)
    register_vk_drop_hook(@@cv_forget)
    vt_on_release(@@wd_release)
    vt_on_release(@@rv_release)
    vt_on_release(@@cv_release)
    static_if (!typeinfo builtin_module_exists(das_metal)) {
        register_gpu_home("vulkan", @@vulkan_asr_dec_install)
        if (!gpu_want_cuda()) {
            vulkan_asr_dec_install()
        }
    }
}
