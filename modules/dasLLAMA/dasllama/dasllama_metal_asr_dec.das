options gen2
options indenting = 4
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_metal_asr_dec shared public

require dasllama/dasllama_lint public
require math
require dasllama/dasllama_env
require dasllama/dasllama_common
require dasllama/dasllama_math   // active repack params — the dq planes carry the CPU backend's layout
require dasllama/dasllama_layout   // q8_region_to_metal_blob — the 34B-blob upload transform
require dasllama/dasllama_metal_shapes
require dasllama/dasllama_whisper
require dasllama/dasllama_rnnt
require dasllama/dasllama_tower   // TowerDev and the device block's pieces (`#tower-dev-block`)
require metal/das_metal_boost
require dasllama/dasllama_metal_common
require dasllama/dasllama_metal_lens   // release_handles
require dasllama/dasllama_gpu_math   // the row stamps' activation codes
require dasllama/dasllama_metal_kernels
require dasllama/dasllama_metal_prefill   // metal_prefill_pso_init — the borrowed-builder bring-up seat
require dasllama/dasllama_metal_tower   // metal_tower_enc_rows - the encode's device rows the transducer projection reads

// The whisper ASR-decoder Metal driver, Q8-NATIVE: cross-KV and the decode step (above the
// serving floor). Declines by returning false at window claim; mid-window the GPU owns the
// KV, so failures panic. The dq planes upload ONCE through the 34B-blob transform
// (un-repacking the CPU backend's layout) — the tuned q8 GEMM/GEMV families serve every
// decoder-side matmul.

var private g_wd_failed = false
var private g_wd_pool : MetalBufferPool
var private g_wd_env = true
var private g_wd_step_env = true    // the decode-step half (DASLLAMA_METAL_WDEC_STEP); windows capture it
//! The text width the decode step serves from: at 384 (tiny) the device step is slower than the CPU's, from 512 (base) up it is faster.
let METAL_WDEC_STEP_MIN_D = 512l
var private g_wd_step_min_d = METAL_WDEC_STEP_MIN_D
// the attached decoder, keyed on the dq plane (base, length): ONE live whisper decoder per
// process is the serving contract — a key mismatch re-attaches; tests switching models call
// metal_wdec_shutdown between them (address reuse after an unmap could alias the key)
var private g_wd_key = 0ul
var private g_wd_len = 0l
var private g_wd_mblob : MetalBuffer?
var private g_wd_mblob_borrowed = false   //! the blob wraps a Metal image's piece (the region cache owns it): a release drops the view alone
// driver-chosen DESTINATION offsets (bytes, 16B-aligned per region): the CPU element offsets
// don't stay bind-aligned for every model (an odd vocab leaves dqlayers_off 128-aligned), so
// the upload lays regions out itself. Order per layer: q,k,v,o,cq,ck,cv,co,fc1,fc2.
var private g_wd_te_boff = 0ul
var private g_wd_boffs : array<uint64>
let private WD_MATS = 10l
let private WD_ROW_CAP = 32l   // the step GEMMs run one M-pad-32 tile; a batch above it panics mid-window
// per-model persistent GPU state (attach allocates, shutdown/re-attach releases). K/V all ride
// the [hh][t][hs] layout, UNSCALED — the fused attention's `scale` carries the whole hs^-0.5.
var private g_wd_bkxr : MetalBuffer?   // cross K resident [nl][hh][ta][hs]
var private g_wd_bvxr : MetalBuffer?   // cross V resident [nl][hh][ta][hs] (also the ds.vx readback source)
var private g_wd_bkc : MetalBuffer?    // self K cache [nl][hh][tmax][hs]
var private g_wd_bvc : MetalBuffer?    // self V cache [nl][hh][tmax][hs]
var private g_wd_sx : MetalBuffer?     // [WD_ROW_CAP x d] residual stream
var private g_wd_sxb : MetalBuffer?    // [WD_ROW_CAP x d] norm/projection staging
var private g_wd_sxb2 : MetalBuffer?   // [WD_ROW_CAP x d] attention output
var private g_wd_sq : MetalBuffer?
var private g_wd_sk : MetalBuffer?
var private g_wd_sv : MetalBuffer?
var private g_wd_sh : MetalBuffer?     // [WD_ROW_CAP x 4d]
var private g_wd_slog : MetalBuffer?   // [WD_ROW_CAP x nvpad] logits (the prompt batch computes every row)
var private g_wd_bpms : MetalBuffer?   // attention chunk partials [nch][hh][n][2] (m, s)
var private g_wd_bpy : MetalBuffer?    // attention chunk partials [nch][hh][n][hs]
var private g_wd_nvpad = 0l            // n_vocab padded to 64 rows — enc_gemm_b tiles by floor(n/32), so an unpadded tail would silently drop (the GEMV ceil-divides and needs none)
// the live GPU-owned window: ONE DecoderState at a time; set by a served cross_kv, checked by
// every decode batch (a mismatch falls back cleanly — the CPU chain reads the ds.kx/vx readback)
var private g_wd_window_key = 0ul
var private g_wd_window_live = false
var private g_wd_window_step = false   // this window's captured wdec_step — the step serves only when set
// the step-time split (the g_us_* decode-rail pattern): GPU-busy vs host encode, microseconds
var private g_wd_us_gpu = 0l
var private g_wd_us_encode = 0l

//! Engage/decline counters (the counts live in dasllama_metal_common, the counting home):
//! `windows` = cross-KV windows served, `steps` = decode batches, `tokens` = rows across them.
def metal_wdec_stats : tuple<windows : int64; steps : int64; tokens : int64; declines : int64> {
    // policy notes (small, step_knob) describe SERVED windows — only refusals count
    let served_notes = (g_wdec_declines.by_reason?["{MetalWdecDecline.small}"] ?? 0l) + (g_wdec_declines.by_reason?["{MetalWdecDecline.step_knob}"] ?? 0l)
    return (windows = g_wdec_windows, steps = g_wdec_steps, tokens = g_wdec_tokens, declines = g_wdec_declines.total - served_notes)
}

//! The declines-by-reason table (a copy), keyed by `MetalWdecDecline` value names.
def metal_wdec_declines : table<string; int64> => <- clone_to_move(g_wdec_declines.by_reason)

//! The step-time split across every served decode batch: GPU-busy vs host-encode microseconds.
//! Wall minus their sum is the commit/wait/readback residue — the per-token overhead lens.
def metal_wdec_times : tuple<us_gpu : int64; us_encode : int64> => (us_gpu = g_wd_us_gpu, us_encode = g_wd_us_encode)

//! Test A/B seat: pin the ASR-decoder driver on/off in-process (the env knob's setter twin).
def set_metal_wdec(on : bool) {
    g_wd_env = on
}

//! The ASR-decoder knob as it stands, for a cell that puts it back where it found it.
def metal_wdec_knob() : bool => g_wd_env

//! Pin the DECODE-STEP half on/off (DASLLAMA_METAL_WDEC_STEP's twin): captured at each
//! window's cross-KV, so a mid-window flip never mixes engines on GPU-only caches.
def set_metal_wdec_step(on : bool) {
    g_wd_step_env = on
}

//! The step's serving floor on n_text_state (test seat; 0 = serve every size).
def set_metal_wdec_step_min_d(n : int64) {
    g_wd_step_min_d = n
}

[cold_path]   // the decline leg IS the fall-off-the-GPU path; its bookkeeping is not hot
def private wd_decline(why : MetalWdecDecline) : bool {
    // knob/small/step_knob = policy (the enum documents them); capability declines honor required
    let policy = why == MetalWdecDecline.knob || why == MetalWdecDecline.small || why == MetalWdecDecline.step_knob || why == MetalWdecDecline.flavor
    note_decline(g_wdec_declines, "wdec", "{why}", policy = policy)
    return false
}

var private g_wd_pend_uid = 0ul           //!< the state whose cross-KV layouts wait on the device, unread
var private g_wd_pend_bytes = 0ul         //!< their size a plane; 0 = nothing waits
var private g_wd_pend_kx : MetalBuffer?
var private g_wd_pend_vx : MetalBuffer?
var private g_wd_dropped : table<uint64>  //!< the states whose waiting layouts were lost unread; a state leaves it at its next window
var private g_wd_uid_next = 1ul

//! the driver's identity for a decoder state, minted at its first window: a state re-created at the same address inherits nothing
def private wd_uid(var ds : DecoderState) : uint64 {
    if (ds.gpu_uid == 0ul) {
        ds.gpu_uid = g_wd_uid_next++
    }
    return ds.gpu_uid
}

def private wd_pend_release {
    pool_release(g_wd_pool, g_wd_pend_kx, g_wd_pend_bytes)
    pool_release(g_wd_pool, g_wd_pend_vx, g_wd_pend_bytes)
    g_wd_pend_kx = null
    g_wd_pend_vx = null
    g_wd_pend_bytes = 0ul
}

//! the waiting cross-KV layouts of `ds`, where they are its own, landed in its `kx` / `vx`: the CPU chain is about to read them
[cold_path]
def private wd_flush_cross_kv(var ds : DecoderState) {
    if (g_wd_pend_bytes == 0ul || g_wd_pend_uid != ds.gpu_uid) {
        return
    }
    unsafe {
        memcpy(addr(ds.kx[0]), metal_buffer_contents(g_wd_pend_kx), g_wd_pend_bytes)
        memcpy(addr(ds.vx[0]), metal_buffer_contents(g_wd_pend_vx), g_wd_pend_bytes)
    }
    wd_pend_release()
}

//! the waiting layouts lost unread (another window's about to take their place, or the driver released): the owner is marked,
//! and its next decode step panics until it starts a window of its own
[cold_path]
def private wd_drop_pending {
    if (g_wd_pend_bytes > 0ul) {
        g_wd_dropped |> insert(g_wd_pend_uid)
        wd_pend_release()
    }
}

def private wd_release_all {
    wd_drop_pending()
    if (g_wd_mblob_borrowed) {
        g_wd_mblob = null
        g_wd_mblob_borrowed = false
    }
    release_handles(g_wd_mblob, g_wd_bkxr, g_wd_bvxr, g_wd_bkc, g_wd_bvc, g_wd_sx)
    release_handles(g_wd_sxb, g_wd_sxb2, g_wd_sq, g_wd_sk, g_wd_sv, g_wd_sh)
    release_handles(g_wd_slog, g_wd_bpms, g_wd_bpy)
    g_wd_key = 0ul
    g_wd_len = 0l
    g_wd_window_key = 0ul
    g_wd_window_live = false
}

//! Release the driver's GPU objects (the weight blob + the scratch pool) — the tests'
//! leak-gate rail and the model-switch seat; lazily re-attaches on the next served window.
def metal_wdec_shutdown {
    wd_release_all()
    pool_drain(g_wd_pool)
    g_wd_failed = false
}

[cold_path]   // per-window bring-up: the registry/prefill inits are latched-flag cheap
def private metal_wdec_init : bool =>
    !g_wd_failed && metal_common_init() && metal_prefill_pso_init() && g_pso_wdec_kv != null && g_pso_wdec_kv_h != null

// Upload the decoder weights once: walk every dq region (te + 10 mats/layer, each its own
// repack unit) through the 34B transform into one GPU blob at driver-chosen aligned
// destinations. ~1.1x the dq plane, one-time.
[cold_path]
def private wdec_attach(w : WhisperModel) : bool {
    let dev = w.enc.dev_backend == TowerDev.metal
    let base = dev ? intptr(plane_at(w.enc.dev, 0l)) : intptr(plane_at(w.dqblob, 0l))
    let blen = dev ? w.enc.dev.n : length(w.dqblob)
    if (g_wd_mblob != null && base == g_wd_key && blen == g_wd_len) {
        return true
    }
    wd_release_all()   // a model switch drops the whole per-model state, window included
    let d = w.n_text_state
    let nl = w.n_text_layer
    g_wd_nvpad = (w.n_vocab + 63l) & ~63l
    if (dev) {
        return false if (!wd_dev_bind(w))
    } else {
        // PURE read (the resident rail's form): re-running selection here desyncs from the load-time repack
        var inscope staging <- wd_gather(w, active_q8_repack_layout(kernel_backend_needs_repack(active_kernel_backend())))
        g_wd_mblob = metal_new_buffer(g_dev, uint64(length(staging)))
        if (g_wd_mblob == null) {
            return false
        }
        unsafe {
            memcpy(metal_buffer_contents(g_wd_mblob), addr(staging[0]), uint64(length(staging)))
        }
    }
    return wdec_attach_rest(w, base, blen)
}

let private WD_Q8_PIECE = "mt.wdec.q8"            //! the whisper decoder's 34B blob in the encoder tower's Metal device block
let private WD_Q8_OFFS_PIECE = "mt.wdec.q8.offs"  //! ... the token table's byte offset, then every layer's matrices'

//! The decoder's dq regions through the 34B transform at `lay` into one blob - the token table zero-padded to the logits' 64 rows,
//! then every layer's ten matrices - with `g_wd_te_boff` and `g_wd_boffs` set to their byte offsets
[cold_path]
def private wd_gather(w : WhisperModel; lay : Q8RepackLayout) : array<uint8> {
    let d = w.n_text_state
    let nl = w.n_text_layer
    let blen = length(w.dqblob)
    var staging : array<uint8>
    staging |> reserve_resize((blen / 32l) * 34l + (g_wd_nvpad - w.n_vocab) * (d / 32l) * 34l + 16l * (nl * WD_MATS + 1l))
    var cur = 0ul
    g_wd_te_boff = q8_region_append(w.dqblob, w.dqscales, w.d_te_q_off, d, w.n_vocab, lay, staging, cur)
    let te_pad = (g_wd_nvpad - w.n_vocab) * (d / 32l) * 34l
    for (i in range64(te_pad)) {   // zero rows: the prompt-batch GEMM's floor-tiling needs them, their logits go unread
        staging[int64(cur) + i] = 0u8
    }
    cur += uint64(te_pad)
    g_wd_boffs |> resize(nl * WD_MATS)
    for (l in range64(nl)) {
        let lo = dec_layer_offsets(w, l)
        var mi = 0l
        for (woff in [lo.q_w, lo.k_w, lo.v_w, lo.o_w, lo.cq_w, lo.ck_w, lo.cv_w, lo.co_w]) {
            g_wd_boffs[l * WD_MATS + mi] = q8_region_append(w.dqblob, w.dqscales, woff, d, d, lay, staging, cur)
            mi++
        }
        g_wd_boffs[l * WD_MATS + 8l] = q8_region_append(w.dqblob, w.dqscales, lo.fc1_w, d, 4l * d, lay, staging, cur)
        g_wd_boffs[l * WD_MATS + 9l] = q8_region_append(w.dqblob, w.dqscales, lo.fc2_w, 4l * d, d, lay, staging, cur)
        // the fused-QKV GEMV reads q..v as ONE 3d-row region — d%64 keeps the cursor gap-free
        assert(g_wd_boffs[l * WD_MATS + 2l] == g_wd_boffs[l * WD_MATS] + 2ul * uint64((d * d / 32l) * 34l),
            "wdec attach: q/k/v regions are not contiguous")
    }
    staging |> resize(int64(cur))
    return <- staging
}

//! the Metal-flavored model's decoder blob wrapping its piece, the offsets read back from theirs (`#tower-dev-block`)
[cold_path]
def private wd_dev_bind(w : WhisperModel) : bool {
    let p = tower_dev_find(w.enc.devplan, WD_Q8_PIECE)
    let po = tower_dev_find(w.enc.devplan, WD_Q8_OFFS_PIECE)
    return false if (p.off < 0l || po.off < 0l || po.bytes != (1l + w.n_text_layer * WD_MATS) * 8l)
    g_wd_mblob = plane_buffer(g_dev, unsafe(reinterpret<void?>(plane_at(w.enc.dev, p.off))), uint64(p.bytes), w.image_map != null)
    return false if (g_wd_mblob == null)
    g_wd_mblob_borrowed = true
    g_wd_boffs |> resize(w.n_text_layer * WD_MATS)
    unsafe {
        let wp = reinterpret<int64 const?>(plane_at(w.enc.dev, po.off))
        g_wd_te_boff = uint64(wp[0])
        for (i in range64(w.n_text_layer * WD_MATS)) {
            g_wd_boffs[i] = uint64(wp[1l + i])
        }
    }
    return true
}

//! whisper's Metal device block (`#tower-dev-block`): the encoder tower's pieces, then the decoder's 34B blob row-major and its
//! offsets
[cold_path, arch(at = "../ARCHITECTURE_IMAGE_GPU_PLANES.md#tower-dev-block")]
def private metal_whisper_bake(w : WhisperModel; var b : TowerDevBuild) : bool {
    return false if (!metal_audio_tower_bake_into(w.enc, true, b))
    g_wd_nvpad = (w.n_vocab + 63l) & ~63l
    var inscope staging <- wd_gather(w, active_q8_repack_layout(false))
    tower_dev_put(b, WD_Q8_PIECE, unsafe(addr<uint8 const?>(staging[0])), long_length(staging))
    var inscope offs <- [int64(g_wd_te_boff)]
    for (o in g_wd_boffs) {
        offs |> push(int64(o))
    }
    tower_dev_put(b, WD_Q8_OFFS_PIECE, unsafe(addr<uint8 const?>(offs[0])), long_length(offs) * 8l)
    return true
}

//! whisper's Metal chains serve an encoder lane: the tower knob on, both decoder halves on the device
[unused_argument(q8)]
def private metal_whisper_serves_model(q8 : bool) : bool => g_wd_env && g_wd_step_env && metal_whisper_tower_serves()

//! the per-model buffers past the decoder blob, the key set last: `base` and `blen` name the planes the blob came from
[cold_path]
def private wdec_attach_rest(w : WhisperModel; base : uint64; blen : int64) : bool {
    let d = w.n_text_state
    let nl = w.n_text_layer
    let ta = w.enc.n_ctx
    let tmax = w.n_text_ctx
    g_wd_bkxr = metal_new_buffer(g_dev, uint64(nl * d * ta * 2l))    // f16 resident memory
    g_wd_bvxr = metal_new_buffer(g_dev, uint64(nl * d * ta * 2l))
    g_wd_bkc = metal_new_buffer(g_dev, uint64(nl * d * tmax * 2l))
    g_wd_bvc = metal_new_buffer(g_dev, uint64(nl * d * tmax * 2l))
    g_wd_sx = metal_new_buffer(g_dev, uint64(WD_ROW_CAP * d * 4l))
    g_wd_sxb = metal_new_buffer(g_dev, uint64(WD_ROW_CAP * d * 4l))
    g_wd_sxb2 = metal_new_buffer(g_dev, uint64(WD_ROW_CAP * d * 4l))
    g_wd_sq = metal_new_buffer(g_dev, uint64(WD_ROW_CAP * d * 4l))
    g_wd_sk = metal_new_buffer(g_dev, uint64(WD_ROW_CAP * d * 4l))
    g_wd_sv = metal_new_buffer(g_dev, uint64(WD_ROW_CAP * d * 4l))
    g_wd_sh = metal_new_buffer(g_dev, uint64(WD_ROW_CAP * 4l * d * 4l))
    g_wd_slog = metal_new_buffer(g_dev, uint64(WD_ROW_CAP * g_wd_nvpad * 4l))
    let nch_cap = (max(ta, tmax) + 255l) / 256l
    let hh_a = w.n_text_head
    g_wd_bpms = metal_new_buffer(g_dev, uint64(nch_cap * hh_a * WD_ROW_CAP * 2l * 4l))
    g_wd_bpy = metal_new_buffer(g_dev, uint64(nch_cap * hh_a * WD_ROW_CAP * (d / hh_a) * 4l))
    if (g_wd_slog == null || g_wd_bpms == null || g_wd_bpy == null) {   // any miss: the key stays
        return false                                                    // unset, the next window re-attaches
    }
    g_wd_key = base
    g_wd_len = blen
    return true
}

// The per-window cross-KV memory on the GPU: per layer the ck/cv q8 GEMMs over the encoder
// rows, then the kx transpose-prescale and vx re-tile shuffles; one command buffer, readback
// into the CPU-layout ds.kx/ds.vx (the CPU chain reads them whenever the step declines).
[hot_path]
def private metal_whisper_cross_kv(w : WhisperModel; var ds : DecoderState; enc_out : array<float>) : bool {   // nolint:STYLE038 — the dispatch chain is the shape
    g_wd_window_live = false   // whatever happens below, the previous window is over
    wd_drop_pending()
    g_wd_dropped |> erase(wd_uid(ds))   // this state starts a window of its own, on either chain
    if (!g_wd_env) {
        return wd_decline(MetalWdecDecline.knob)
    }
    if (!w.dq8) {
        return wd_decline(MetalWdecDecline.quant_mode)
    }
    if (w.enc.dev_backend != TowerDev.metal) {
        return wd_decline(MetalWdecDecline.flavor)
    }
    let d = w.n_text_state
    let hh = w.n_text_head
    let hs = d / hh
    let ta = w.enc.n_ctx
    let nl = w.n_text_layer
    // mm n-tile + attention hs unroll gate the chain; the 256-chunk pair needs no ta/ctx cap
    if (d % 64l != 0l || hs % 4l != 0l) {
        return wd_decline(MetalWdecDecline.shape)
    }
    if (!metal_wdec_init() || !wdec_attach(w)) {
        return wd_decline(MetalWdecDecline.device)
    }
    assert(long_length(ds.kx) >= nl * d * ta && long_length(ds.vx) >= nl * d * ta)
    let mp = (ta + 31l) / 32l * 32l
    let mapped = w.image_map != null
    var bblobf = plane_buffer(g_dev, unsafe(reinterpret<void?>(plane_at(w.dblob, 0l))), uint64(length(w.dblob) * 4l), mapped)
    let bytes_rows = uint64(mp * d * 4l)
    let bytes_x = uint64(nl * d * ta * 4l)
    var bx = pool_acquire(g_wd_pool, g_dev, bytes_rows)
    var btmp = pool_acquire(g_wd_pool, g_dev, bytes_rows)
    var bkx = pool_acquire(g_wd_pool, g_dev, bytes_x)
    var bvx = pool_acquire(g_wd_pool, g_dev, bytes_x)
    var bxh = pf_q8_mm_half() ? pool_acquire(g_wd_pool, g_dev, bytes_rows / 2ul) : null
    upload_rows_padded(bx, unsafe(addr(enc_out[0])), ta, mp, d)   // the pad outputs go unread; the zero guards stale-Inf pool reuse
    var u_d = uniform_u32(uint(d))
    var err = ""
    let ran = with_compute_encoder(g_queue, err) $(enc : MetalComputeEncoder?) {   // nolint:PERF026 — the error-text fetch runs only on the failure leg
        for (l in range64(nl)) {
            let lo = dec_layer_offsets(w, l)
            if (bxh != null && l == 0l) {   // one half panel serves every layer's two GEMMs: they all read the encoder rows
                tw_half(enc, bx, bxh, mp * d)
            }
            pf_enc_q8_mm(enc, g_wd_mblob, g_wd_boffs[l * WD_MATS + 5l], bx, bxh, btmp, u_d, u_d, mp, d, d)
            // kx: the pre-scaled CPU layout (the ds.kx readback); kxr: the unscaled resident twin
            var kv_k = GkWdecKvArgs(rows = uint(ta), sstride = uint(d), soff = 0u, nheads = uint(d / hs), tstride = uint(ta), p0 = 0u, dst_off = uint(l * d * ta), boff = 0u, bias_on = 0u, mode = WDEC_KV_KX, hs = uint(hs), kscale = wdec_kscale(hs))
            enc_wdec_kv(enc, btmp, bkx, bblobf, kv_k, d * ta)
            kv_k.mode = WDEC_KV_F16
            enc_wdec_kv_h(enc, btmp, g_wd_bkxr, bblobf, kv_k, d * ta)
            pf_enc_q8_mm(enc, g_wd_mblob, g_wd_boffs[l * WD_MATS + 6l], bx, bxh, btmp, u_d, u_d, mp, d, d)
            // cv bias folds into both shuffles: the f32 readback (ds.vx) and the f16 resident
            var kv_v = GkWdecKvArgs(rows = uint(ta), sstride = uint(d), soff = 0u, nheads = uint(d / hs), tstride = uint(ta), p0 = 0u, dst_off = uint(l * d * ta), boff = uint(lo.cv_b), bias_on = 1u, mode = WDEC_KV_VX, hs = uint(hs), kscale = wdec_kscale(hs))
            enc_wdec_kv(enc, btmp, bvx, bblobf, kv_v, d * ta)
            kv_v.mode = WDEC_KV_F16
            enc_wdec_kv_h(enc, btmp, g_wd_bvxr, bblobf, kv_v, d * ta)
        }
    }
    var served = false
    if (ran) {
        g_wdec_windows++
        g_wd_window_key = intptr(unsafe(addr(ds)))
        g_wd_window_live = true
        g_wd_window_step = g_wd_step_env && (d >= g_wd_step_min_d || w.enc.dev_backend == TowerDev.metal)   //! a Metal-flavored model's step serves at any width: the CPU never runs it
        ds.gpu_kv_gen = 0l   // nothing GPU-decoded in this window yet
        if (!g_wd_step_env) {
            wd_decline(MetalWdecDecline.step_knob)   // policy: cross-KV still serves
        } elif (!g_wd_window_step) {
            wd_decline(MetalWdecDecline.small)   // policy: the window still cross-KV-serves
        }
        served = true
    }
    if (served && g_wd_window_step) {   // the device step reads the resident twins: the CPU layouts wait, read back only at a decline
        g_wd_pend_uid = ds.gpu_uid
        g_wd_pend_bytes = bytes_x
        g_wd_pend_kx = bkx
        g_wd_pend_vx = bvx
    } else {
        if (served) {
            unsafe {
                memcpy(addr(ds.kx[0]), metal_buffer_contents(bkx), bytes_x)
                memcpy(addr(ds.vx[0]), metal_buffer_contents(bvx), bytes_x)
            }
        }
        pool_release(g_wd_pool, bkx, bytes_x)
        pool_release(g_wd_pool, bvx, bytes_x)
    }
    pool_release(g_wd_pool, bx, bytes_rows)
    pool_release(g_wd_pool, btmp, bytes_rows)
    if (bxh != null) {
        pool_release(g_wd_pool, bxh, bytes_rows / 2ul)
    }
    pool_release(g_upool, u_d, 4ul)
    if (!served) {
        return wd_decline(MetalWdecDecline.gpu_error)
    }
    return true
}

// One command buffer on a CONCURRENT encoder with the hz oracle deriving barriers — every
// generated builder self-declares, so only real buffer hazards drain the pipeline (the serial
// encoder drains at each of the step's ~90 tiny dispatches).
def private wd_run_cb(var err : string&; var gpu_ms : double&; var encode_ms : double&;
                      blk : block<(enc : MetalComputeEncoder?) : void>) : bool {
    return with_compute_encoder_timed(g_queue, err, gpu_ms, encode_ms, true) $(enc) {
        hz_arm(true)
        invoke(blk, enc)
        hz_arm(false)
    }
}

// one projection site: the row-parallel GEMV at n=1 (occupancy = rows), the GEMM tile for the prompt
def private wd_mm(enc : MetalComputeEncoder?; boff : uint64; bx, by : MetalBuffer?;
                  u_k, u_n : MetalBuffer?; kdim, ndim, n : int64) {
    if (n == 1l) {
        enc_gemv(enc, g_wd_mblob, boff, bx, by, u_k, u_n, ndim)
    } else {
        enc_gemm_b(enc, g_wd_mblob, boff, bx, by, gemm_ka(kdim, ndim, ndim), WD_ROW_CAP, ndim)
    }
}

// One decode batch: CPU embed, then ONE command buffer through the layer stack (LN, q8
// projections, KV-append shuffles, fused self/cross attention, LUT-GELU MLP, tied-embedding
// logits) and the full-vocab logits readback — the CPU filter/sampler stays the parity anchor.
[hot_path]
def private metal_whisper_decode(w : WhisperModel; var ds : DecoderState; toks : int const?; n : int64) : bool {   // nolint:STYLE038 — the dispatch chain is the shape
    if (ds.gpu_uid != 0ul && key_exists(g_wd_dropped, ds.gpu_uid)) {
        panic("dasLLAMA wdec: this decoder window's cross-attention memory was lost before it was read back - another state's window or a driver release took its device planes; ONE live decoder window per process, and a state decodes only a window it started after the loss")
    }
    if (!g_wd_window_live || !g_wd_window_step || g_wd_window_key != intptr(unsafe(addr(ds)))) {
        wd_flush_cross_kv(ds)
        // an evicted GPU-decoded window: the CPU chain would read kcache rows the GPU owns
        if (ds.gpu_kv_gen != 0l && ds.n_past > 0l) {
            panic("dasLLAMA wdec: the GPU decoder window was evicted mid-window - ONE live decoder window per process (a second DecoderState may not interleave whisper_cross_kv/whisper_decode_batch)")
        }
        return false   // declined at cross_kv, or the step half is off — the CPU chain reads ds.kx/vx
    }
    if (n > WD_ROW_CAP) {
        if (ds.n_past == 0l) {   // first batch: nothing GPU-owned yet — release the claim, the CPU serves the window
            g_wd_window_live = false
            wd_flush_cross_kv(ds)
            return false
        }
        panic("dasLLAMA wdec: decode batch of {n} exceeds the {WD_ROW_CAP}-row step cap mid-window")
    }
    let d = w.n_text_state
    let hh = w.n_text_head
    let hs = d / hh
    let ta = w.enc.n_ctx
    let tmax = w.n_text_ctx
    let nl = w.n_text_layer
    let p0 = ds.n_past
    whisper_embed_rows(w, toks, n, p0, unsafe(reinterpret<float?>(metal_buffer_contents(g_wd_sx))))
    let mapped = w.image_map != null
    var bblobf = plane_buffer(g_dev, unsafe(reinterpret<void?>(plane_at(w.dblob, 0l))), uint64(length(w.dblob) * 4l), mapped)
    var u_d = uniform_u32(uint(d))
    var u_3d = uniform_u32(uint(3l * d))
    var u_ff = uniform_u32(uint(4l * d))
    var u_eps = uniform_f32(w.eps)
    var u_totd = uniform_u32(uint(n * d))
    var u_n = uniform_u32(uint(n))
    let nch_s = (p0 + n + 255l) / 256l
    let nch_c = (ta + 255l) / 256l
    let wd_scale = 1.0 / sqrt(float(hs))
    var u_nv = uniform_u32(uint(g_wd_nvpad))
    var err = ""
    var gpu_ms = 0.0lf
    var encode_ms = 0.0lf
    let ran = wd_run_cb(err, gpu_ms, encode_ms) $(enc : MetalComputeEncoder?) {   // nolint:PERF026 — the error-text fetch runs only on the failure leg
        for (l in range64(nl)) {
            let lo = dec_layer_offsets(w, l)
            let bo = l * WD_MATS
            // self-attention (v bias folds into the append shuffle, q bias into the attention load)
            enc_ln(enc, g_wd_sx, g_wd_sxb, bblobf, gk_ln_args(uint(d), uint(n), w.eps, uint(lo.ln1_w), uint(lo.ln1_b)), n)
            if (n == 1l) {   // the fused-QKV GEMV: the q,k,v regions sit contiguous in the driver's blob
                enc_gemv(enc, g_wd_mblob, g_wd_boffs[bo], g_wd_sxb, g_wd_sq, u_d, u_3d, 3l * d)
                // dim = the SOURCE row stride: the k/v column blocks sit in the fused 3d-wide rows
                var kv_s = GkWdecKvArgs(rows = uint(n), sstride = uint(3l * d), soff = uint(d), nheads = uint(hh), tstride = uint(tmax), p0 = uint(p0), dst_off = uint(l * d * tmax), boff = 0u, bias_on = 0u, mode = WDEC_KV_F16, hs = uint(hs), kscale = wdec_kscale(hs))
                enc_wdec_kv_h(enc, g_wd_sq, g_wd_bkc, bblobf, kv_s, n * d)
                kv_s.soff = uint(2l * d)
                kv_s.boff = uint(lo.v_b)
                kv_s.bias_on = 1u
                enc_wdec_kv_h(enc, g_wd_sq, g_wd_bvc, bblobf, kv_s, n * d)
            } else {
                wd_mm(enc, g_wd_boffs[bo], g_wd_sxb, g_wd_sq, u_d, u_d, d, d, n)
                wd_mm(enc, g_wd_boffs[bo + 1l], g_wd_sxb, g_wd_sk, u_d, u_d, d, d, n)
                wd_mm(enc, g_wd_boffs[bo + 2l], g_wd_sxb, g_wd_sv, u_d, u_d, d, d, n)
                var kv_s = GkWdecKvArgs(rows = uint(n), sstride = uint(d), soff = 0u, nheads = uint(hh), tstride = uint(tmax), p0 = uint(p0), dst_off = uint(l * d * tmax), boff = 0u, bias_on = 0u, mode = WDEC_KV_F16, hs = uint(hs), kscale = wdec_kscale(hs))
                enc_wdec_kv_h(enc, g_wd_sk, g_wd_bkc, bblobf, kv_s, n * d)
                kv_s.boff = uint(lo.v_b)
                kv_s.bias_on = 1u
                enc_wdec_kv_h(enc, g_wd_sv, g_wd_bvc, bblobf, kv_s, n * d)
            }
            enc_wdec_attn_part(enc, g_wd_sq, g_wd_bkc, g_wd_bvc, g_wd_bpms, g_wd_bpy, bblobf, GkWdecAttnArgs(dim = uint(d), tstride = uint(tmax), len0 = uint(p0 + 1l),
                causal = 1u, nheads = uint(hh), nrows = uint(n), koff = uint(l * d * tmax), voff = uint(l * d * tmax), qboff = uint(lo.q_b), hs = uint(hs), scale = wd_scale), nch_s * hh * n)
            enc_wdec_attn_comb(enc, g_wd_bpms, g_wd_bpy, g_wd_sxb2, GkWdecCombArgs(dim = uint(d), nchunks = uint(nch_s), nheads = uint(hh), nrows = uint(n), hs = uint(hs)), hh * n)
            wd_mm(enc, g_wd_boffs[bo + 3l], g_wd_sxb2, g_wd_sxb, u_d, u_d, d, d, n)
            enc_bias_add_res(enc, g_wd_sx, g_wd_sxb, bblobf, uint64(lo.o_b * 4l), u_d, u_totd, n * d)
            // cross-attention against the window's resident memory
            enc_ln(enc, g_wd_sx, g_wd_sxb, bblobf, gk_ln_args(uint(d), uint(n), w.eps, uint(lo.cln_w), uint(lo.cln_b)), n)
            wd_mm(enc, g_wd_boffs[bo + 4l], g_wd_sxb, g_wd_sq, u_d, u_d, d, d, n)
            enc_wdec_attn_part(enc, g_wd_sq, g_wd_bkxr, g_wd_bvxr, g_wd_bpms, g_wd_bpy, bblobf, GkWdecAttnArgs(dim = uint(d), tstride = uint(ta), len0 = uint(ta),
                causal = 0u, nheads = uint(hh), nrows = uint(n), koff = uint(l * d * ta), voff = uint(l * d * ta), qboff = uint(lo.cq_b), hs = uint(hs), scale = wd_scale), nch_c * hh * n)
            enc_wdec_attn_comb(enc, g_wd_bpms, g_wd_bpy, g_wd_sxb2, GkWdecCombArgs(dim = uint(d), nchunks = uint(nch_c), nheads = uint(hh), nrows = uint(n), hs = uint(hs)), hh * n)
            wd_mm(enc, g_wd_boffs[bo + 7l], g_wd_sxb2, g_wd_sxb, u_d, u_d, d, d, n)
            enc_bias_add_res(enc, g_wd_sx, g_wd_sxb, bblobf, uint64(lo.co_b * 4l), u_d, u_totd, n * d)
            // mlp (fc1 bias folds into the LUT-GELU pass, fc2's into the residual add)
            enc_ln(enc, g_wd_sx, g_wd_sxb, bblobf, gk_ln_args(uint(d), uint(n), w.eps, uint(lo.ln2_w), uint(lo.ln2_b)), n)
            wd_mm(enc, g_wd_boffs[bo + 8l], g_wd_sxb, g_wd_sh, u_d, u_ff, d, 4l * d, n)
            tw_bias(enc, g_wd_sh, bblobf, GkBiasActArgs(d = uint(4l * d), nelem = uint(n * 4l * d), boff = uint(lo.fc1_b), act = BIAS_ACT_GELU_TANH))
            wd_mm(enc, g_wd_boffs[bo + 9l], g_wd_sh, g_wd_sxb, u_ff, u_d, 4l * d, d, n)
            enc_bias_add_res(enc, g_wd_sx, g_wd_sxb, bblobf, uint64(lo.fc2_b * 4l), u_d, u_totd, n * d)
        }
        // final LN, then logits: a single-row GEMV per token; the prompt batch GEMMs every row
        enc_ln(enc, g_wd_sx, g_wd_sxb, bblobf, gk_ln_args(uint(d), uint(n), w.eps, uint(w.d_ln_w_off), uint(w.d_ln_b_off)), n)
        wd_mm(enc, g_wd_te_boff, g_wd_sxb, g_wd_slog, u_d, u_nv, d, g_wd_nvpad, n)
    }
    pool_release(g_upool, u_d, 4ul)
    pool_release(g_upool, u_3d, 4ul)
    pool_release(g_upool, u_ff, 4ul)
    pool_release(g_upool, u_eps, 4ul)
    pool_release(g_upool, u_totd, 4ul)
    pool_release(g_upool, u_n, 4ul)
    pool_release(g_upool, u_nv, 4ul)
    if (!ran) {   // the GPU owns this window's KV — a silent CPU fallback would decode blind
        panic("dasLLAMA wdec: GPU decode failed mid-window: {err}")
    }
    unsafe {
        memcpy(addr(ds.logits[0]), reinterpret<uint8?>(metal_buffer_contents(g_wd_slog)) + (n - 1l) * g_wd_nvpad * 4l, uint64(w.n_vocab * 4l))
    }
    g_wd_us_gpu += int64(gpu_ms * 1000.0lf)
    g_wd_us_encode += int64(encode_ms * 1000.0lf)
    g_wdec_steps++
    g_wdec_tokens += n
    ds.gpu_kv_gen = 1l   // the GPU now owns rows of this window's self-KV (see the eviction panic)
    return true
}

// ===== the transducer greedy decode (parakeet, GigaAM's RNNT head) =====

enum private MetalRnntDecline {
    knob     //! DASLLAMA_METAL_RNNT=0, or set_metal_rnnt(false)
    shape    //! a predictor or joint past the bodies' staged widths, a third layer, a duration table past the state's
    device   //! no device, or a pipeline failed to compile, or a weight buffer failed
    gpu_error
    overrun  //! the rounds ran past the clip's budget with the clip not done
}

var private g_rd_env = true
var private g_rd_said : table<string>   //! the overrides' announces already made
var private g_rd_declines : DeclineCounter
var private g_rd_decodes = 0l     //! clips decoded on the device
var private g_rd_handoffs = 0l    //! ... whose joint projection read the encode's own device rows
var private g_rd_projects = 0l    //! ... that projected the encoder rows on the device (the rest decoded off the family's projection)
var private g_rd_rounds = 0l      //! the rounds the picks ran: a predictor step and a joint window, or a joint window alone
var private g_rd_steps = 0l       //! the predictor steps among them
var private g_rd_tokens = 0l
var private g_rd_cbs = 0l         //! the command buffers the loops took
var private g_rd_uploads = 0l     //! the weight uploads: one a model, again only where another net takes the seat
var private g_rd_us_gpu = 0l

//! Engage counters: `decodes` served on the device, `handoffs` among them off the encode's device rows, `projects` among them
//! projected on the device, `rounds` / `steps` the picks' rounds and predictor steps, `tokens`, `cbs` the command buffers,
//! `uploads` the weight uploads, `declines` every decline but the knob's.
def metal_rnnt_stats : tuple<decodes : int64; handoffs : int64; projects : int64; rounds : int64; steps : int64; tokens : int64; cbs : int64; uploads : int64; declines : int64> {
    return (decodes = g_rd_decodes, handoffs = g_rd_handoffs, projects = g_rd_projects, rounds = g_rd_rounds, steps = g_rd_steps,
        tokens = g_rd_tokens, cbs = g_rd_cbs, uploads = g_rd_uploads, declines = g_rd_declines.total - (g_rd_declines.by_reason?["{MetalRnntDecline.knob}"] ?? 0l))
}

//! The transducer seat's declines-by-reason table (a copy), keyed by the decline's name, the knob's among them.
def metal_rnnt_declines : table<string; int64> => <- clone_to_move(g_rd_declines.by_reason)

//! The transducer loop's GPU-busy microseconds across every served decode.
def metal_rnnt_us_gpu : int64 => g_rd_us_gpu

//! Pin the transducer decode on the device on/off in-process (DASLLAMA_METAL_RNNT's twin); answers the setting it replaced.
def set_metal_rnnt(on : bool) : bool {
    let was = g_rd_env
    g_rd_env = on
    return was
}

[cold_path]
def private rd_decline(why : MetalRnntDecline) : bool {
    note_decline(g_rd_declines, "rnnt", "{why}", policy = why == MetalRnntDecline.knob)
    return false
}

//! The decode's weights on the device, laid once a model (`rnnt_dev_key`): the state's halves (`wh`, the joint columns `jw`), and
//! the slab `wf` at `lay`'s offsets.
struct private RdWeights {
    key : uint64
    lay : RnntDevLayout
    wh : MetalBuffer?
    jw : MetalBuffer?
    wf : MetalBuffer?
    wenc : MetalBuffer?   //! the encoder projection's f32 rows [jd][d], where the seat projects
}

var private g_rd_w : RdWeights
var private g_rd_pool : MetalBufferPool

def private rd_release_weights {
    release_handles(g_rd_w.wh, g_rd_w.jw, g_rd_w.wf, g_rd_w.wenc)
    g_rd_w.key = 0ul
}

//! Release the transducer seat's device objects - the weights and the scratch pool - the leak-gate and model-switch rail.
def metal_rnnt_shutdown {
    rd_release_weights()
    pool_drain(g_rd_pool)
}

def private rd_attach(net : RnntNet; s : RnntState; enc : RnntEnc) : bool {
    let key = rnnt_dev_key(net, enc)
    return true if (g_rd_w.key == key && g_rd_w.wh != null)
    return rd_attach_build(net, s, enc, key)
}

//! ... uploaded on a new net
[cold_path]
def private rd_attach_build(net : RnntNet; s : RnntState; enc : RnntEnc; key : uint64) : bool {
    rd_release_weights()
    g_rd_w.lay = rnnt_dev_layout(net)
    let lay = g_rd_w.lay
    if (net.devplan.n > 0l) {   //! a Metal-flavored net's halves and slab are its image's pieces, copied as stored
        g_rd_w.wh = rd_piece(net, "{MT_RNNT_PIECE}.wh", lay.wh_halves * 2l)
        g_rd_w.jw = rd_piece(net, "{MT_RNNT_PIECE}.jw", lay.jw_halves * 2l)
        g_rd_w.wf = rd_piece(net, "{MT_RNNT_PIECE}.wf", lay.wf_floats * 4l)
    } else {
        var inscope wf <- rnnt_dev_slab(net, s, enc, lay)
        unsafe {
            g_rd_w.wh = metal_buffer_from(g_dev, uint64(lay.wh_halves * 2l), addr<void?>(s.wh[0]))
            g_rd_w.jw = metal_buffer_from(g_dev, uint64(lay.jw_halves * 2l), addr<void?>(s.joint_wt[0]))
            g_rd_w.wf = metal_buffer_from(g_dev, uint64(long_length(wf) * 4l), addr<void?>(wf[0]))
        }
    }
    unsafe {
        if (!enc.q8) {
            g_rd_w.wenc = metal_buffer_from(g_dev, uint64(net.joint_dim * enc.d * 4l), reinterpret<void?>(plane_at(net.blob, enc.w)))
        }
    }
    if (g_rd_w.wh == null || g_rd_w.jw == null || g_rd_w.wf == null || (!enc.q8 && g_rd_w.wenc == null) || rnnt_dev_faulted(RnntDevFault.device)) {
        rd_release_weights()
        return false
    }
    g_rd_w.key = key
    g_rd_uploads++
    return true
}

//! the device-block piece `name` of a net's image copied into a fresh buffer; null where it is missing or not `bytes` long
def private rd_piece(net : RnntNet; name : string; bytes : int64) : MetalBuffer? {
    let p = tower_dev_find(net.devplan, name)
    return null if (p.off < 0l || p.bytes != bytes)
    unsafe {
        return metal_buffer_from(g_dev, uint64(bytes), reinterpret<void?>(plane_at(net.dev, p.off)))
    }
}

//! Whether the Metal seat projects a lane's encoder half: the f32 plane's, through the exact f32 GEMM.
def private metal_rnnt_projects(enc : RnntEnc) : bool => g_rd_env && !enc.q8

var private g_rd_rate = 0.3lf   //! the rounds a frame the last served clip took: the first command buffer's estimate

//! The decode's per-clip device state: the state words, the hidden pair and the cells, the predictor input, joint.pred's
//! output, the window's logits, the token records, the encoder projection and, where the encode left none, the rows.
struct private RdBufs {
    st : MetalBuffer?
    hb : MetalBuffer?
    cb : MetalBuffer?
    xin : MetalBuffer?
    pred : MetalBuffer?
    lg : MetalBuffer?
    toks : MetalBuffer?
    encp : MetalBuffer?
    rows : MetalBuffer?
    sizes : uint64[9]
}

def private rd_bufs_take(net : RnntNet; n_frames, n_logits, cap, d : int64; own_rows : bool) : RdBufs {
    let pd = net.pred_dim
    let jd = net.joint_dim
    let mp = round_up(n_frames, 32l)
    let sz = fixed_array(uint64(GK_RNNT_ST_WORDS) * 4ul, uint64(2l * net.n_layers * pd * 4l), uint64(net.n_layers * pd * 4l), uint64(pd * 4l),
        uint64(jd * 4l), uint64(int64(GK_RNNT_FRAMES) * rnnt_joint_blocks(n_logits) * RNNT_JOINT_BLOCK * 4l), uint64(cap * int64(GK_RNNT_TOK_WORDS) * 4l),
        uint64(mp * jd * 4l), own_rows ? uint64(mp * d * 4l) : 0ul)
    return RdBufs(sizes = sz, st = pool_acquire(g_rd_pool, g_dev, sz[0]), hb = pool_acquire(g_rd_pool, g_dev, sz[1]),
        cb = pool_acquire(g_rd_pool, g_dev, sz[2]), xin = pool_acquire(g_rd_pool, g_dev, sz[3]), pred = pool_acquire(g_rd_pool, g_dev, sz[4]),
        lg = pool_acquire(g_rd_pool, g_dev, sz[5]), toks = pool_acquire(g_rd_pool, g_dev, sz[6]), encp = pool_acquire(g_rd_pool, g_dev, sz[7]),
        rows = own_rows ? pool_acquire(g_rd_pool, g_dev, sz[8]) : null)
}

def private rd_bufs_give(var b : RdBufs) {
    pool_release(g_rd_pool, b.st, b.sizes[0])
    pool_release(g_rd_pool, b.hb, b.sizes[1])
    pool_release(g_rd_pool, b.cb, b.sizes[2])
    pool_release(g_rd_pool, b.xin, b.sizes[3])
    pool_release(g_rd_pool, b.pred, b.sizes[4])
    pool_release(g_rd_pool, b.lg, b.sizes[5])
    pool_release(g_rd_pool, b.toks, b.sizes[6])
    pool_release(g_rd_pool, b.encp, b.sizes[7])
    if (b.rows != null) {
        pool_release(g_rd_pool, b.rows, b.sizes[8])
    }
}

//! The clip's start: the state words, the first input, the hidden pair and the cells zeroed.
def private rd_state_init(net : RnntNet; b : RdBufs) {
    unsafe {
        rnnt_dev_state_words(net, reinterpret<uint?>(metal_buffer_contents(b.st)))
        rnnt_dev_first_input(net, reinterpret<float?>(metal_buffer_contents(b.xin)))
        memset8(reinterpret<uint8?>(metal_buffer_contents(b.hb)), uint8(0), int(b.sizes[1]))
        memset8(reinterpret<uint8?>(metal_buffer_contents(b.cb)), uint8(0), int(b.sizes[2]))
    }
}

//! A round's dispatches: each layer's gates, joint.pred's projection, the joint window, the pick.
def private rd_round(enc : MetalComputeEncoder?; net : RnntNet; b : RdBufs; ka : GkRnntArgs; n_logits : int64) {
    for (l in range64(net.n_layers)) {
        var kl = rnnt_dev_layer_args(ka, g_rd_w.lay, l)
        enc_rnnt_gates(enc, b.st, g_rd_w.wh, g_rd_w.wf, b.xin, b.hb, b.cb, kl, (net.pred_dim + 7l) / 8l)
    }
    var kr = ka
    enc_rnnt_proj(enc, b.st, g_rd_w.wh, g_rd_w.wf, b.hb, b.pred, kr, (net.joint_dim + 7l) / 8l)
    enc_rnnt_joint(enc, b.st, b.encp, g_rd_w.wf, b.pred, g_rd_w.jw, b.lg, kr, rnnt_joint_blocks(n_logits) * 4l)
    enc_rnnt_pick(enc, b.st, b.lg, g_rd_w.wf, b.xin, b.toks, kr)
}

//! The transducer's greedy decode on the device: the joint's encoder projection (the exact f32 GEMM off the encode's own
//! device rows where it left them) or the family's, then command buffers of rounds until the pick marks the clip done; the
//! token records read back once.
[hot_path, arch(at="../ARCHITECTURE_MEDIA_ASR.md#rnnt-device-decode")]
def private metal_rnnt_greedy(net : RnntNet; var s : RnntState; enc : RnntEnc; x, encp : array<float>; n_frames : int64;
                              @scratch var toks : array<RnntToken>) : bool {
    if (!g_rd_env) {
        gpu_said_once(g_rd_said, LOG_INFO, "metal rnnt: ", "DASLLAMA_METAL_RNNT=0 or set_metal_rnnt(false) - the transducer decode runs the CPU loop")
        return rd_decline(MetalRnntDecline.knob)
    }
    let jd = net.joint_dim
    let d = enc.d
    let n_logits = rnnt_n_logits(net)
    let projects = !enc.q8
    if (!rnnt_dev_shape_ok(net, enc, 64l)) {   //! the exact f32 GEMM's column tile
        return rd_decline(MetalRnntDecline.shape)
    }
    if (!metal_common_init() || !metal_prefill_pso_init() || !pf_rnnt_ready() || (projects && g_pso_f32_exact_mm == null) || !rd_attach(net, s, enc)) {
        return rd_decline(MetalRnntDecline.device)
    }
    if (n_frames <= 0l) {
        g_rd_decodes++
        return true
    }
    let mp = round_up(n_frames, 32l)
    var rows_dev = projects ? metal_tower_enc_rows(enc.gen, n_frames * d) : null
    let handoff = rows_dev != null
    var b = rd_bufs_take(net, n_frames, n_logits, rnnt_dev_cap(net, n_frames), d, projects && !handoff)
    rd_state_init(net, b)
    var u_d, u_jd : MetalBuffer?
    if (projects) {
        if (!handoff) {
            upload_rows_padded(b.rows, unsafe(addr(x[0])), n_frames, mp, d)
            rows_dev = b.rows
        }
        u_d = uniform_u32(uint(d))
        u_jd = uniform_u32(uint(jd))
    } else {
        unsafe {
            memcpy(metal_buffer_contents(b.encp), addr(encp[0]), n_frames * jd * 4l)
        }
    }
    let ka = rnnt_dev_args(net, g_rd_w.lay, n_frames, projects)
    var err = ""
    let sp = unsafe(reinterpret<uint const?>(metal_buffer_contents(b.st)))
    let run = rnnt_dev_rounds(net, n_frames, sp, g_rd_rate) $(rounds : int64; first : bool) : bool {
        var gpu_ms = 0.0lf
        let ok = with_compute_encoder_timed(g_queue, err, gpu_ms) $(enc_ : MetalComputeEncoder?) {   // nolint:PERF026 — the error-text fetch runs only on the failure leg
            if (first && projects) {
                enc_f32_exact_mm(enc_, g_rd_w.wenc, 0ul, rows_dev, b.encp, 0ul, u_d, u_jd, mp, jd)
            }
            for (_r in range64(rounds)) {
                rd_round(enc_, net, b, ka, n_logits)
            }
        }
        g_rd_us_gpu += int64(gpu_ms * 1000.0lf)
        return ok
    }
    g_rd_cbs += run.cbs
    if (run.ok) {
        rnnt_dev_land_tokens(unsafe(reinterpret<uint const?>(metal_buffer_contents(b.toks))), run.p.ntok, toks)
        g_rd_decodes++
        g_rd_tokens += int64(run.p.ntok)
        g_rd_rounds += run.p.rounds
        g_rd_steps += run.p.steps
        if (projects) {
            g_rd_projects++
        }
        if (handoff) {
            g_rd_handoffs++
        }
    }
    if (u_d != null) {
        pool_release(g_upool, u_d, 4ul)
        pool_release(g_upool, u_jd, 4ul)
    }
    rd_bufs_give(b)
    return run.ok || rd_decline(run.overrun ? MetalRnntDecline.overrun : MetalRnntDecline.gpu_error)
}

[init]
def dasllama_metal_asr_dec_register {
    g_wd_env = g_env_metal.wdec
    g_wd_step_env = g_env_metal.wdec_step
    g_rd_env = g_env_metal.rnnt
    register_reload_prep(@@rd_release_weights)
    register_whisper_cross_kv_gpu(@@metal_whisper_cross_kv)
    register_whisper_decode_gpu(@@metal_whisper_decode)
    register_whisper_flavor(TowerDev.metal, @@metal_whisper_serves_model, @@metal_whisper_bake)
    register_rnnt_gpu(@@metal_rnnt_greedy, @@metal_rnnt_projects)
}
