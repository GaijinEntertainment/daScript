options gen2
options indenting = 4
options _dasllama_internal = true

//! The CUDA home's whisper decoder seat, q8-native: the per-window cross K / V and the decode step, each the shared body
//! (`dasllama_asr_dec_chains`) on the decoder's own CUDA rail off the CUDA-flavored image's `cu.wdec` pieces; the encoder rows come
//! device to device off the CUDA tower's post-normed plane (`cuda_tower_enc_out`) where it landed them. Installed by
//! `dasllama_cuda_tower_audio`'s whisper arm with the whisper flavor.
module dasllama_cuda_asr_dec shared public

require dasllama/dasllama_lint public
require math
require dasllama/dasllama_kqformat
require dasllama/dasllama_math   // grow_resize
require dasllama/dasllama_plane
require dasllama/dasllama_tower
require dasllama/dasllama_tower_image   // tower_bake_q8: the decoder's q8 block pieces
require dasllama/dasllama_gpu_tier   // GpuDeclineCounter, TILE_READ_SLACK
require dasllama/dasllama_gpu_math
require dasllama/dasllama_gpu_kernels_common
require dasllama/dasllama_gpu_kernels_discrete
require dasllama/dasllama_cuda_common
require dasllama/dasllama_cuda_classes
require dasllama/dasllama_cuda_classes_asr
require dasllama/dasllama_cuda_decode   // register_cud_drop_hook
require dasllama/dasllama_cuda_rail
require dasllama/dasllama_cuda_tower_seat
require dasllama/dasllama_cuda_tower_whisper   // cuda_tower_enc_out, cuda_audio_tower_bake_into, cut_whisper_class_serves
require dasllama/dasllama_whisper
require dasllama/dasllama_asr_dec_chains

var private g_cud_wd_on = true
var private g_cud_wd_declines = GpuDeclineCounter(home = "cuda")
var private g_cud_wd_windows = 0l
var private g_cud_wd_handoffs = 0l   //! cross-KV windows whose encoder rows came device to device off the tower's plane
var private g_cud_wd_steps = 0l
var private g_cud_wd_tokens = 0l

//! the decoder's residency: the shared seat record (its key, its growth and its refusals) with the q8 pieces and the f32 plane
//! bound once a model in its `wq` / `ws` / `norms` / `sched`, and the decoder's own planes the step and the window bind
struct private CudWdResident {
    t : CutResident = CutResident(family = "wdec")
    g : WdecGeom
    miss_said : bool   //! the host-upload arm of the cross-KV chain said once: the tower landed no device plane for this window's rows
    kxr : CutBuf
    vxr : CutBuf
    kc : CutBuf
    vc : CutBuf
    x : CutBuf
    xq : CutBuf
    xs : CutBuf
    qkv : CutBuf
    cq : CutBuf
    yo : CutBuf
    h : CutBuf
    pms : CutBuf
    py : CutBuf
    logits : CutBuf
    xin : CutBuf
    xh : CutBuf
    xkv_q : CutBuf
    xkv_s : CutBuf
    xkv_proj : CutBuf
}

var private g_cud_wd = CudWdResident()
var private g_cud_wd_rl : CutRail   //! the decoder's own rail: its seats, picks and schedule maps kept across windows and steps
var private g_cud_wd_window_uid = 0ul     //! the live window: ONE DecoderState at a time, claimed by a served cross-KV
var private g_cud_wd_window_live = false
var private @scratch @exact_size g_cud_wd_meta : array<uint>    //! the schedule's staging
var private @scratch @exact_size g_cud_wd_rows : array<float>   //! the step's embedded rows staging
var private g_cud_wd_logits_row = 0u   //! the logits record's activation block, staged for its upload

let private CUD_WD_PIECE = "cu.wdec"   //! the whisper decoder's q8 pieces' prefix, in the encoder tower's device block

//! Engage / decline counters: `windows` served, `steps` batches, `tokens` rows, `declines` all but the knob's (policy), `handoffs` windows off the tower's device rows.
def cuda_wdec_stats : tuple<windows : int64; steps : int64; tokens : int64; declines : int64; handoffs : int64> {
    let policy = (g_cud_wd_declines.counts?["{WdecDecline.knob}"] ?? 0l)
    return (windows = g_cud_wd_windows, steps = g_cud_wd_steps, tokens = g_cud_wd_tokens, declines = g_cud_wd_declines.total - policy, handoffs = g_cud_wd_handoffs)
}

//! The declines-by-reason table (a copy), keyed by `WdecDecline` value names.
def cuda_wdec_declines : table<string; int64> => <- clone_to_move(g_cud_wd_declines.counts)

//! Test A/B seat: pin the ASR-decoder driver on / off in-process.
def set_cuda_wdec(on : bool) {
    g_cud_wd_on = on
}

def cuda_wdec_on : bool => g_cud_wd_on

[cold_path]
def private cud_wd_decline(why : WdecDecline) : bool {
    let words = wdec_decline_words(why, "CUDA", "the decoder knob is off (set_cuda_wdec(false))", CUT_DECLINE_WORDS_DEVICE, CUT_DECLINE_WORDS_MEMORY)
    return g_cud_wd_declines |> note("the whisper decoder", "{why}", "the decoder leaves the device - a CPU image decodes on the CPU chain, a CUDA image refuses: {words}")
}

[cold_path]
def private cud_wd_release {
    var r & = g_cud_wd
    cut_release(r.t)
    for (b in fixed_array(r.kxr, r.vxr, r.kc, r.vc, r.x, r.xq, r.xs, r.qkv, r.cq, r.yo, r.h, r.pms, r.py, r.logits, r.xin, r.xh, r.xkv_q, r.xkv_s,
                          r.xkv_proj)) {
        cuda_mem_free(b.dev)
    }
    delete r
    r <- CudWdResident()
    g_cud_wd_window_uid = 0ul
    g_cud_wd_window_live = false
    g_cud_wd_declines |> unsay("the whisper decoder")
}

//! every class the two chains dispatch at the cross site's pick `p`, or false
[cold_path]
def private cud_wd_ensure(p : CutPick) : bool {
    return (cut_pick_ensure(p) && ensure_q8_gemv_cls() && ensure_q8_gemv_n2_cls() && ensure_q8_gemv_n4_cls() && ensure_q8_gemv_n8_cls()
        && ensure_tower_ln_rq_cls() && ensure_tower_post_add_ln_rq_cls() && ensure_tower_bias_act_rq_cls() && ensure_tower_clamp_cvt_cls()
        && ensure_tower_wdec_kv16_cls() && ensure_tower_wdec_attn_part_cls() && ensure_tower_wdec_attn_comb_rq_cls())
}

//! the decoder's rail at the resident's planes: its lane, the cross site's pick and both sites' binds, every seat
def private cud_wd_seats(var rl : CutRail; r : CudWdResident) {
    let g = r.g
    let z = wdec_sizes(g)
    rl.stream = g_cud.stream
    rail_seat(rl, TowerBuf.x, r.x.dev, z.bx, 0u)
    rail_seat(rl, TowerBuf.xq, r.xq.dev, z.bq, 0u)
    rail_seat(rl, TowerBuf.xs, r.xs.dev, z.bs, 0u)
    rail_seat(rl, TowerBuf.qkv, r.qkv.dev, 3l * z.bx, 0u)
    rail_seat(rl, TowerBuf.cq, r.cq.dev, z.bx, 0u)
    rail_seat(rl, TowerBuf.yo, r.yo.dev, z.bx, 0u)
    rail_seat(rl, TowerBuf.hg, r.h.dev, z.bh, 0u)
    rail_seat(rl, TowerBuf.pms, r.pms.dev, z.bpms, 0u)
    rail_seat(rl, TowerBuf.py, r.py.dev, z.bpy, 0u)
    rail_seat(rl, TowerBuf.kc, r.kc.dev, z.bkc, 0u)
    rail_seat(rl, TowerBuf.vc, r.vc.dev, z.bkc, 0u)
    rail_seat(rl, TowerBuf.kxr, r.kxr.dev, z.bkx, 0u)
    rail_seat(rl, TowerBuf.vxr, r.vxr.dev, z.bkx, 0u)
    rail_seat(rl, TowerBuf.logits, r.logits.dev, g.nvocab * 4l, 0u)
    rail_seat(rl, TowerBuf.rows_out, r.xin.dev, z.bt, 0u)
    rail_seat(rl, TowerBuf.enc_h, r.xh.dev, r.xh.bytes, 0u)
    rail_seat(rl, TowerBuf.enc_q, r.xkv_q.dev, r.xkv_q.bytes, 0u)
    rail_seat(rl, TowerBuf.enc_s, r.xkv_s.dev, r.xkv_s.bytes, 0u)
    rail_seat(rl, TowerBuf.enc_proj, r.xkv_proj.dev, r.xkv_proj.bytes, 0u)
    rail_seat(rl, TowerBuf.norms, r.t.norms.dev, r.t.norms.bytes, 0u)
    rail_seat(rl, TowerBuf.wq, r.t.wq.dev, r.t.wq.bytes, 0u)
    rail_seat(rl, TowerBuf.ws, r.t.ws.dev, r.t.ws.bytes, 0u)
    rail_seat(rl, TowerBuf.meta, r.t.sched.dev, g.meta_bytes, 0u)
}

//! the resident built for a model the key does not know: the old one released, the q8 pieces and f32 plane up, planes sized, rail and schedule; false counted
[cold_path, arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#whisper-decoder")]
def private cud_wd_build(w : WhisperModel; key : uint64) : bool {
    cud_wd_release()
    var r & = g_cud_wd
    var rl & = g_cud_wd_rl
    let d = w.n_text_state
    let ta = w.enc.n_ctx
    let pick = cut_pick_whole(d, ta)
    return cud_wd_decline(WdecDecline.device) if (!cud_wd_ensure(pick))
    rail_begin(rl, RailLane.q8)
    rl.norms_base = w.dlayers_off
    rl.sites[int(RailSite.cross)] = pick
    r.g = wdec_geom(w, rail_site_groups(rl, RailSite.cross, d, ta))
    wdec_binds(rl, r.g)
    let z = wdec_sizes(r.g)
    let rows = rail_site_rows(rl, RailSite.cross, ta)
    var t & = r.t
    t.key = key
    cut_put_piece(t, t.wq, w.enc, "{CUD_WD_PIECE}.wq")
    cut_put_piece(t, t.ws, w.enc, "{CUD_WD_PIECE}.ws")
    var inscope norm_rows <- wdec_norm_rows(w, r.g)
    cut_grow(t, t.norms, long_length(norm_rows) * 4l)
    cut_grow(t, t.sched, r.g.meta_bytes)
    cut_grow(t, r.kxr, z.bkx)
    cut_grow(t, r.vxr, z.bkx)
    cut_grow(t, r.kc, z.bkc)
    cut_grow(t, r.vc, z.bkc)
    cut_grow(t, r.x, z.bx)
    cut_grow(t, r.xq, z.bq)
    cut_grow(t, r.xs, z.bs)
    cut_grow(t, r.qkv, 3l * z.bx)
    cut_grow(t, r.cq, z.bx)
    cut_grow(t, r.yo, z.bx)
    cut_grow(t, r.h, z.bh)
    cut_grow(t, r.pms, z.bpms)
    cut_grow(t, r.py, z.bpy)
    cut_grow(t, r.logits, r.g.nvocab * 4l)
    cut_grow(t, r.xin, z.bt)
    cut_grow(t, r.xh, (rows + TILE_READ_SLACK) * d * 2l)
    cut_grow(t, r.xkv_q, 64l)
    cut_grow(t, r.xkv_s, 64l)
    cut_grow(t, r.xkv_proj, rows * d * 4l)
    if (t.oom) {
        cud_wd_release()
        return cud_wd_decline(WdecDecline.memory)
    }
    cut_htod(t.norms.dev, unsafe(addr<void?>(norm_rows[0])), long_length(norm_rows) * 4l)
    g_cud_wd_meta |> grow_resize(r.g.meta_bytes / 4l)
    wdec_meta(rl, w, r.g, unsafe(addr(g_cud_wd_meta[0])))
    cut_htod(t.sched.dev, unsafe(addr<void?>(g_cud_wd_meta[0])), r.g.meta_bytes)
    cud_wd_seats(rl, r)
    return true
}

//! the resident for `w`: the key over its CUDA block and the decoder's f32 rows, built where the key is new
def private cud_wd_attach(w : WhisperModel) : bool {
    let key = cut_key_dev(w.enc.dev, w.enc.devplan, w.dblob, w.n_text_layer)
    return (g_cud_wd.t.key != 0ul && key == g_cud_wd.t.key) || cud_wd_build(w, key)
}

[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#whisper-decoder")]
def private cuda_whisper_cross_kv(w : WhisperModel; var ds : DecoderState; enc_out : array<float>) : bool {
    let uid = wdec_uid(ds)
    g_cud_wd_window_live = false
    ds.gpu_kv_gen = 0l
    return false if (!wdec_gate(w, g_cud_wd_on, TowerDev.cuda, $(why : WdecDecline) => cud_wd_decline(why)))
    return cud_wd_decline(WdecDecline.device) if (!cuda_tower_arms())
    cud_bind()
    return false if (!cud_wd_attach(w))
    var r & = g_cud_wd
    var rl & = g_cud_wd_rl
    let bt = wdec_sizes(r.g).bt
    let enc_dev = cuda_tower_enc_out(unsafe(addr<float const?>(enc_out[0])), bt)
    if (enc_dev == 0ul) {
        cut_htod(r.xin.dev, unsafe(addr<void?>(enc_out[0])), bt)
        wdec_miss_say(r.miss_said, "cuda") if (!r.miss_said)
    } else {
        g_cud_wd_handoffs++
        rail_seat(rl, TowerBuf.xb, enc_dev, bt, 0u)
    }
    asr_dec_whisper_cross_kv(rl, w, r.g, enc_dev != 0ul)
    cut_sync("whisper cross-kv")
    g_cud_wd_windows++
    g_cud_wd_window_uid = uid
    g_cud_wd_window_live = true
    wdec_announce(r.t.served_said, "cuda", r.g) if (!r.t.served_said)
    return true
}

[hot_path, arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#whisper-decoder")]
def private cuda_whisper_decode(w : WhisperModel; var ds : DecoderState; toks : int const?; n : int64) : bool {
    return false if (!wdec_step_admits(ds, g_cud_wd_window_live, g_cud_wd_window_uid, n, "cuda", "CUDA"))
    let r & = g_cud_wd
    var rl & = g_cud_wd_rl
    let g = r.g
    let p0 = ds.n_past
    cud_bind()
    g_cud_wd_rows |> reserve(WDEC_ROW_CAP * g.d)
    g_cud_wd_rows |> resize(n * g.d)
    whisper_embed_rows(w, toks, n, p0, unsafe(addr(g_cud_wd_rows[0])))
    g_cud_wd_logits_row = wdec_logits_row(g, n)
    cut_htod(r.x.dev, unsafe(addr<void?>(g_cud_wd_rows[0])), n * g.d * 4l)
    cut_htod(r.t.sched.dev + uint64(wdec_logits_word(g) * 4l), unsafe(addr<void?>(g_cud_wd_logits_row)), 4l)
    asr_dec_whisper_step(rl, w, g, n, p0)
    cut_dtoh(unsafe(addr<void?>(ds.logits[0])), r.logits.dev, g.nvocab * 4l)
    cut_sync("whisper decode")
    g_cud_wd_steps++
    g_cud_wd_tokens += n
    ds.gpu_kv_gen = 1l
    return true
}

//! whisper's CUDA device block (`#tower-dev-block`): the encoder tower's pieces, then the decoder's q8 regions gathered
[cold_path, arch(at = "../ARCHITECTURE_IMAGE_GPU_PLANES.md#tower-dev-block")]
def private cuda_whisper_bake(w : WhisperModel; var b : TowerDevBuild) : bool {
    cuda_audio_tower_bake_into(w.enc, b)
    var inscope regions <- wdec_regions(w)
    tower_bake_q8(b, CUD_WD_PIECE, w.dqblob, w.dqscales, regions)
    return true
}

//! whisper's CUDA chains serve the encoder's lane (`q8`, else exact beside the q8 decoder): the encoder's chains on the armed home and the decoder's knob on
def private cuda_whisper_serves_model(q8 : bool) : bool
    => (g_cud_wd_on || cut_flavor_refused("whisper", "the decoder knob is off (set_cuda_wdec(false))")) && cut_whisper_class_serves("whisper", q8)

//! the CUDA home's whisper decoder seats - the cross-KV and decode hooks - and the whisper flavor
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#rail-audio-installs")]
def cuda_whisper_install_decoder {
    register_whisper_cross_kv_gpu(@@cuda_whisper_cross_kv)
    register_whisper_decode_gpu(@@cuda_whisper_decode)
    register_whisper_flavor(TowerDev.cuda, @@cuda_whisper_serves_model, @@cuda_whisper_bake)
}

[init]
def dasllama_cuda_asr_dec_register {
    cut_on_release(@@cud_wd_release)
}
