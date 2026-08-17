options gen2
options indenting = 4
options _comment_hygiene = true

module dasllama_metal_asr_dec shared public

require math
require dasllama/dasllama_env
require dasllama/dasllama_common
require dasllama/dasllama_math   // active repack params — the dq planes carry the CPU backend's layout
require dasllama/dasllama_layout   // q8_region_to_metal_blob — the 34B-blob upload transform
require dasllama/dasllama_metal_shapes
require dasllama/dasllama_whisper
require metal/das_metal_boost
require dasllama/dasllama_metal_common
require dasllama/dasllama_metal_kernels
require dasllama/dasllama_metal_prefill   // metal_prefill_pso_init — the borrowed-builder bring-up seat

// The whisper ASR-decoder Metal driver, Q8-NATIVE: cross-KV now, the decode step as it lands.
// Declines by returning false; the CPU chain is always the correct fallback. The decoder's dq
// planes upload ONCE through the 34B-blob transform (un-repacking the CPU backend's layout),
// so the tuned q8 GEMM/GEMV families serve every decoder-side matmul.

var private g_wd_failed = false
var private g_wd_pool : MetalBufferPool
var private g_wd_env = true
var private g_wd_step_env = true    // the decode-step half (DASLLAMA_METAL_WDEC_STEP); windows capture it
var private g_wd_step_min_d = 1024l   // the step's serving floor (measured: d=1280 wins 1.6x, d=384 loses)
// the attached decoder, keyed on the dq plane (base, length): ONE live whisper decoder per
// process is the serving contract — a key mismatch re-attaches; tests switching models call
// metal_wdec_shutdown between them (address reuse after an unmap could alias the key)
var private g_wd_key = 0ul
var private g_wd_len = 0l
var private g_wd_mblob : MetalBuffer?
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
var private g_wd_sxb : MetalBuffer?
var private g_wd_sxb2 : MetalBuffer?
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
    var dtot = 0l
    for (v in values(g_wdec_declines_by_reason)) {
        dtot += v
    }
    return (windows = g_wdec_windows, steps = g_wdec_steps, tokens = g_wdec_tokens, declines = dtot)
}

//! The declines-by-reason table (a copy), keyed by `MetalWdecDecline` value names.
def metal_wdec_declines : table<string; int64> {
    return <- clone_to_move(g_wdec_declines_by_reason)
}

//! The step-time split across every served decode batch: GPU-busy vs host-encode microseconds.
//! Wall minus their sum is the commit/wait/readback residue — the per-token overhead lens.
def metal_wdec_times : tuple<us_gpu : int64; us_encode : int64> {
    return (us_gpu = g_wd_us_gpu, us_encode = g_wd_us_encode)
}

//! Test A/B seat: pin the ASR-decoder driver on/off in-process (the env knob's setter twin).
def set_metal_wdec(on : bool) {
    g_wd_env = on
}

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
    note_wdec_decline("{why}")
    // knob and small = policy (the enum documents both); capability declines honor required mode
    if (why != MetalWdecDecline.knob && why != MetalWdecDecline.small) {
        require_or_panic("wdec", "{why}")
    }
    return false
}

//! Release the driver's GPU objects (the weight blob + the scratch pool) — the tests' leak-gate
//! rail and the model-switch seat; lazily re-attaches on the next served window.
def private wd_release_one(var b : MetalBuffer?&) {
    if (b != null) {
        metal_release(b)
        b = null
    }
}

def private wd_release_all {
    wd_release_one(g_wd_mblob)
    wd_release_one(g_wd_bkxr)
    wd_release_one(g_wd_bvxr)
    wd_release_one(g_wd_bkc)
    wd_release_one(g_wd_bvc)
    wd_release_one(g_wd_sx)
    wd_release_one(g_wd_sxb)
    wd_release_one(g_wd_sxb2)
    wd_release_one(g_wd_sq)
    wd_release_one(g_wd_sk)
    wd_release_one(g_wd_sv)
    wd_release_one(g_wd_sh)
    wd_release_one(g_wd_slog)
    wd_release_one(g_wd_bpms)
    wd_release_one(g_wd_bpy)
    g_wd_key = 0ul
    g_wd_len = 0l
    g_wd_window_key = 0ul
    g_wd_window_live = false
}

def metal_wdec_shutdown {
    wd_release_all()
    pool_drain(g_wd_pool)
    g_wd_failed = false
}

[cold_path]   // per-window bring-up: the registry/prefill inits are latched-flag cheap
def private metal_wdec_init : bool {
    return !g_wd_failed && metal_common_init() && metal_prefill_pso_init() && g_pso_cross_kx != null
}

// one region of the 34B-blob build: `off` is the SOURCE element offset in the dq planes; the
// destination is the 16B-aligned cursor, recorded and advanced
def private wd_region(w : WhisperModel; off, n, rows : int64; rep : bool; mr, kgroup, wbias : int64;
                      var staging : array<uint8>; var cur : uint64&) : uint64 {
    cur = (cur + 15ul) & ~15ul
    let at = cur
    unsafe {
        q8_region_to_metal_blob(plane_at(w.dqblob, off), plane_at(w.dqscales, off / 32l),
            n, rows, rep, mr, kgroup, wbias, addr(staging[int64(cur)]))
    }
    cur += uint64((rows * n / 32l) * 34l)
    return at
}

// Upload the decoder weights once: walk every dq region (te + 10 mats/layer, each its own
// repack unit) through the 34B transform into one GPU blob at driver-chosen aligned
// destinations. ~1.1x the dq plane, one-time.
[cold_path]
def private wdec_attach(w : WhisperModel) : bool {
    let base = intptr(plane_at(w.dqblob, 0l))
    let blen = length(w.dqblob)
    if (g_wd_mblob != null && base == g_wd_key && blen == g_wd_len) {
        return true
    }
    wd_release_all()   // a model switch drops the whole per-model state, window included
    let d = w.n_text_state
    let nl = w.n_text_layer
    g_wd_nvpad = (w.n_vocab + 63l) & ~63l
    var staging : array<uint8>
    staging |> reserve_resize((blen / 32l) * 34l + (g_wd_nvpad - w.n_vocab) * (d / 32l) * 34l + 16l * (nl * WD_MATS + 1l))
    let rep = select_matmul_backend_for_load()
    let mr = active_q8_layout_mr()
    let kgroup = active_q8_kgroup()
    let wbias = active_q8_wbias()
    var cur = 0ul
    g_wd_te_boff = wd_region(w, w.d_te_q_off, d, w.n_vocab, rep, mr, kgroup, wbias, staging, cur)
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
            g_wd_boffs[l * WD_MATS + mi] = wd_region(w, woff, d, d, rep, mr, kgroup, wbias, staging, cur)
            mi++
        }
        g_wd_boffs[l * WD_MATS + 8l] = wd_region(w, lo.fc1_w, d, 4l * d, rep, mr, kgroup, wbias, staging, cur)
        g_wd_boffs[l * WD_MATS + 9l] = wd_region(w, lo.fc2_w, 4l * d, d, rep, mr, kgroup, wbias, staging, cur)
        // the fused-QKV GEMV reads q..v as ONE 3d-row region — d%64 keeps the cursor gap-free
        assert(g_wd_boffs[l * WD_MATS + 2l] == g_wd_boffs[l * WD_MATS] + 2ul * uint64((d * d / 32l) * 34l),
            "wdec attach: q/k/v regions are not contiguous")
    }
    g_wd_mblob = metal_new_buffer(g_dev, cur)
    if (g_wd_mblob == null) {
        delete staging
        return false
    }
    unsafe {
        memcpy(metal_buffer_contents(g_wd_mblob), addr(staging[0]), cur)
    }
    delete staging
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
// into the CPU-layout ds.kx/ds.vx (the CPU decoder consumes them until the step driver lands).
[hot_path]
def private metal_whisper_cross_kv(w : WhisperModel; var ds : DecoderState; enc_out : array<float>) : bool {   // nolint:STYLE038 — the dispatch chain is the shape
    g_wd_window_live = false   // whatever happens below, the previous window is over
    if (!g_wd_env) {
        return wd_decline(MetalWdecDecline.knob)
    }
    if (!w.dq8) {
        return wd_decline(MetalWdecDecline.quant_mode)
    }
    let d = w.n_text_state
    let hh = w.n_text_head
    let hs = d / hh
    let ta = w.enc.n_ctx
    let nl = w.n_text_layer
    // mm n-tile, fused-attention hs unroll, and the tgmem score window gate the whole chain
    if (d % 64l != 0l || hs % 4l != 0l || ta > 1504l || w.n_text_ctx > 1504l) {
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
    unsafe {
        var p = reinterpret<uint8?>(metal_buffer_contents(bx))
        memcpy(p, addr(enc_out[0]), ta * d * 4l)
        if (mp > ta) {   // DEFENSIVE pad zero (the pad outputs go unread): guards stale-Inf pool reuse
            var zp = reinterpret<float?>(p + ta * d * 4l)
            for (i in range64((mp - ta) * d)) {
                zp[i] = 0.0
            }
        }
    }
    var u_d = uniform_u32(uint(d))
    var u_hs = uniform_u32(uint(hs))
    var u_ta = uniform_u32(uint(ta))
    var u_t0 = uniform_u32(0u)
    var u_h0 = uniform_u32(0u)
    var u_h1 = uniform_u32(1u)
    var u_kscale = uniform_f32(1.0 / sqrt(sqrt(float(hs))))
    var u_tot = uniform_u32(uint(d * ta))
    var err = ""
    let ran = with_compute_encoder(g_queue, err) $(enc : MetalComputeEncoder?) {   // nolint:PERF026 — the error-text fetch runs only on the failure leg
        for (l in range64(nl)) {
            let lo = dec_layer_offsets(w, l)
            enc_gemm_mm_b(enc, g_wd_mblob, g_wd_boffs[l * WD_MATS + 5l], bx, btmp, 0ul, u_d, u_d, mp, d)
            // kx: the pre-scaled CPU layout (the ds.kx readback); kxr: the unscaled resident twin
            enc_cross_kx(enc, btmp, bkx, uint64(l * d * ta * 4l), u_d, u_hs, u_ta, u_kscale, u_tot, d * ta)
            enc_cross_vx_h(enc, btmp, 0ul, g_wd_bkxr, uint64(l * d * ta * 2l), bblobf, 0ul, u_d, u_hs, u_ta, u_t0, u_ta, u_h0, u_tot, d * ta)
            enc_gemm_mm_b(enc, g_wd_mblob, g_wd_boffs[l * WD_MATS + 6l], bx, btmp, 0ul, u_d, u_d, mp, d)
            // cv bias folds into both shuffles: the f32 readback (ds.vx) and the f16 resident
            enc_cross_vx(enc, btmp, 0ul, bvx, uint64(l * d * ta * 4l), bblobf, uint64(lo.cv_b * 4l), u_d, u_hs, u_ta, u_t0, u_ta, u_h1, u_tot, d * ta)
            enc_cross_vx_h(enc, btmp, 0ul, g_wd_bvxr, uint64(l * d * ta * 2l), bblobf, uint64(lo.cv_b * 4l), u_d, u_hs, u_ta, u_t0, u_ta, u_h1, u_tot, d * ta)
        }
    }
    var served = false
    if (ran) {
        unsafe {
            memcpy(addr(ds.kx[0]), metal_buffer_contents(bkx), bytes_x)
            memcpy(addr(ds.vx[0]), metal_buffer_contents(bvx), bytes_x)
        }
        g_wdec_windows++
        g_wd_window_key = intptr(unsafe(addr(ds)))
        g_wd_window_live = true
        g_wd_window_step = g_wd_step_env && d >= g_wd_step_min_d
        if (g_wd_step_env && !g_wd_window_step) {
            wd_decline(MetalWdecDecline.small)   // policy class: the window still cross-KV-serves
        }
        served = true
    }
    pool_release(g_wd_pool, bx, bytes_rows)
    pool_release(g_wd_pool, btmp, bytes_rows)
    pool_release(g_wd_pool, bkx, bytes_x)
    pool_release(g_wd_pool, bvx, bytes_x)
    pool_release(g_upool, u_d, 4ul)
    pool_release(g_upool, u_hs, 4ul)
    pool_release(g_upool, u_ta, 4ul)
    pool_release(g_upool, u_t0, 4ul)
    pool_release(g_upool, u_h0, 4ul)
    pool_release(g_upool, u_h1, 4ul)
    pool_release(g_upool, u_kscale, 4ul)
    pool_release(g_upool, u_tot, 4ul)
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
    let cb = metal_new_command_buffer(g_queue)
    let enc = metal_new_compute_encoder_concurrent(cb)
    hz_arm(true)
    let ts = ref_time_ticks()
    invoke(blk, enc)
    hz_arm(false)
    metal_end_encoding(enc)
    encode_ms = double(get_time_usec(ts)) / 1000.0lf
    metal_commit(cb)
    metal_wait_until_completed(cb)
    err = metal_command_buffer_error(cb)
    gpu_ms = (metal_command_buffer_gpu_end_time(cb) - metal_command_buffer_gpu_start_time(cb)) * 1000.0lf
    metal_release(enc)
    metal_release(cb)
    return empty(err)
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
    if (!g_wd_window_live || !g_wd_window_step || g_wd_window_key != intptr(unsafe(addr(ds)))) {
        return false   // declined at cross_kv, or the step half is off — the CPU chain reads ds.kx/vx
    }
    if (n > WD_ROW_CAP) {   // the GPU owns this window's KV — a CPU fallback here would decode blind
        panic("dasLLAMA wdec: decode batch of {n} exceeds the {WD_ROW_CAP}-row step cap mid-window")
    }
    let d = w.n_text_state
    let hh = w.n_text_head
    let hs = d / hh
    let ta = w.enc.n_ctx
    let tmax = w.n_text_ctx
    let nl = w.n_text_layer
    let p0 = ds.n_past
    unsafe {
        var p = reinterpret<float?>(metal_buffer_contents(g_wd_sx))
        for (r in range64(n)) {   // token embedding + learned position rows, straight into the mapped buffer
            let te = w.d_te_off + int64(toks[int(r)]) * d
            let pe = w.d_pe_off + (p0 + r) * d
            for (j in range64(d)) {
                p[r * d + j] = w.dblob[te + j] + w.dblob[pe + j]
            }
        }
    }
    let mapped = w.image_map != null
    var bblobf = plane_buffer(g_dev, unsafe(reinterpret<void?>(plane_at(w.dblob, 0l))), uint64(length(w.dblob) * 4l), mapped)
    var u_d = uniform_u32(uint(d))
    var u_3d = uniform_u32(uint(3l * d))
    var u_hs = uniform_u32(uint(hs))
    var u_ff = uniform_u32(uint(4l * d))
    var u_eps = uniform_f32(w.eps)
    var u_totd = uniform_u32(uint(n * d))
    var u_totff = uniform_u32(uint(n * 4l * d))
    var u_n = uniform_u32(uint(n))
    var u_p0 = uniform_u32(uint(p0))
    var u_tmax = uniform_u32(uint(tmax))
    var u_ta = uniform_u32(uint(ta))
    var u_len_self = uniform_u32(uint(p0 + 1l))
    var u_hh = uniform_u32(uint(hh))
    var u_nch_s = uniform_u32(uint((p0 + n + 255l) / 256l))
    var u_nch_c = uniform_u32(uint((ta + 255l) / 256l))
    var u_c1 = uniform_u32(1u)
    var u_c0 = uniform_u32(0u)
    var u_scale = uniform_f32(1.0 / sqrt(float(hs)))
    var u_nv = uniform_u32(uint(g_wd_nvpad))
    var err = ""
    var gpu_ms = 0.0lf
    var encode_ms = 0.0lf
    let ran = wd_run_cb(err, gpu_ms, encode_ms) $(enc : MetalComputeEncoder?) {   // nolint:PERF026 — the error-text fetch runs only on the failure leg
        for (l in range64(nl)) {
            let lo = dec_layer_offsets(w, l)
            let bo = l * WD_MATS
            let koff_self = uint64(l * d * tmax * 2l)   // the resident K/V memory is f16
            let koff_cross = uint64(l * d * ta * 2l)
            // self-attention (v bias folds into the append shuffle, q bias into the attention load)
            enc_ln(enc, g_wd_sx, bblobf, uint64(lo.ln1_w * 4l), bblobf, uint64(lo.ln1_b * 4l), g_wd_sxb, u_d, u_eps, n)
            if (n == 1l) {   // the fused-QKV GEMV: the q,k,v regions sit contiguous in the driver's blob
                enc_gemv(enc, g_wd_mblob, g_wd_boffs[bo], g_wd_sxb, g_wd_sq, u_d, u_3d, 3l * d)
                // dim = the SOURCE row stride: the k/v column blocks sit in the fused 3d-wide rows
                enc_cross_vx_h(enc, g_wd_sq, uint64(d * 4l), g_wd_bkc, koff_self, bblobf, 0ul, u_3d, u_hs, u_n, u_p0, u_tmax, u_c0, u_totd, n * d)
                enc_cross_vx_h(enc, g_wd_sq, uint64(2l * d * 4l), g_wd_bvc, koff_self, bblobf, uint64(lo.v_b * 4l), u_3d, u_hs, u_n, u_p0, u_tmax, u_c1, u_totd, n * d)
            } else {
                wd_mm(enc, g_wd_boffs[bo], g_wd_sxb, g_wd_sq, u_d, u_d, d, d, n)
                wd_mm(enc, g_wd_boffs[bo + 1l], g_wd_sxb, g_wd_sk, u_d, u_d, d, d, n)
                wd_mm(enc, g_wd_boffs[bo + 2l], g_wd_sxb, g_wd_sv, u_d, u_d, d, d, n)
                enc_cross_vx_h(enc, g_wd_sk, 0ul, g_wd_bkc, koff_self, bblobf, 0ul, u_d, u_hs, u_n, u_p0, u_tmax, u_c0, u_totd, n * d)
                enc_cross_vx_h(enc, g_wd_sv, 0ul, g_wd_bvc, koff_self, bblobf, uint64(lo.v_b * 4l), u_d, u_hs, u_n, u_p0, u_tmax, u_c1, u_totd, n * d)
            }
            enc_wdec_attn_part(enc, g_wd_sq, g_wd_bkc, koff_self, g_wd_bvc, koff_self, g_wd_bpms, g_wd_bpy, bblobf, uint64(lo.q_b * 4l), u_d, u_hs, u_tmax, u_len_self, u_c1, u_scale, u_hh, u_n, hh, n, (p0 + n + 255l) / 256l)
            enc_wdec_attn_comb(enc, g_wd_bpms, g_wd_bpy, g_wd_sxb2, u_d, u_hs, u_nch_s, u_hh, u_n, hh, n)
            wd_mm(enc, g_wd_boffs[bo + 3l], g_wd_sxb2, g_wd_sxb, u_d, u_d, d, d, n)
            enc_bias_add_res(enc, g_wd_sx, g_wd_sxb, bblobf, uint64(lo.o_b * 4l), u_d, u_totd, n * d)
            // cross-attention against the window's resident memory
            enc_ln(enc, g_wd_sx, bblobf, uint64(lo.cln_w * 4l), bblobf, uint64(lo.cln_b * 4l), g_wd_sxb, u_d, u_eps, n)
            wd_mm(enc, g_wd_boffs[bo + 4l], g_wd_sxb, g_wd_sq, u_d, u_d, d, d, n)
            enc_wdec_attn_part(enc, g_wd_sq, g_wd_bkxr, koff_cross, g_wd_bvxr, koff_cross, g_wd_bpms, g_wd_bpy, bblobf, uint64(lo.cq_b * 4l), u_d, u_hs, u_ta, u_ta, u_c0, u_scale, u_hh, u_n, hh, n, (ta + 255l) / 256l)
            enc_wdec_attn_comb(enc, g_wd_bpms, g_wd_bpy, g_wd_sxb2, u_d, u_hs, u_nch_c, u_hh, u_n, hh, n)
            wd_mm(enc, g_wd_boffs[bo + 7l], g_wd_sxb2, g_wd_sxb, u_d, u_d, d, d, n)
            enc_bias_add_res(enc, g_wd_sx, g_wd_sxb, bblobf, uint64(lo.co_b * 4l), u_d, u_totd, n * d)
            // mlp (fc1 bias folds into the LUT-GELU pass, fc2's into the residual add)
            enc_ln(enc, g_wd_sx, bblobf, uint64(lo.ln2_w * 4l), bblobf, uint64(lo.ln2_b * 4l), g_wd_sxb, u_d, u_eps, n)
            wd_mm(enc, g_wd_boffs[bo + 8l], g_wd_sxb, g_wd_sh, u_d, u_ff, d, 4l * d, n)
            enc_bias_gelu_lut(enc, g_wd_sh, bblobf, uint64(lo.fc1_b * 4l), u_ff, u_totff, n * 4l * d)
            wd_mm(enc, g_wd_boffs[bo + 9l], g_wd_sh, g_wd_sxb, u_ff, u_d, 4l * d, d, n)
            enc_bias_add_res(enc, g_wd_sx, g_wd_sxb, bblobf, uint64(lo.fc2_b * 4l), u_d, u_totd, n * d)
        }
        // final LN, then logits: a single-row GEMV per token; the prompt batch GEMMs every row
        enc_ln(enc, g_wd_sx, bblobf, uint64(w.d_ln_w_off * 4l), bblobf, uint64(w.d_ln_b_off * 4l), g_wd_sxb, u_d, u_eps, n)
        if (n == 1l) {
            enc_gemv(enc, g_wd_mblob, g_wd_te_boff, g_wd_sxb, g_wd_slog, u_d, u_nv, g_wd_nvpad)
        } else {
            enc_gemm_b(enc, g_wd_mblob, g_wd_te_boff, g_wd_sxb, g_wd_slog, gemm_ka(d, g_wd_nvpad, g_wd_nvpad), WD_ROW_CAP, g_wd_nvpad)
        }
    }
    pool_release(g_upool, u_d, 4ul)
    pool_release(g_upool, u_3d, 4ul)
    pool_release(g_upool, u_hs, 4ul)
    pool_release(g_upool, u_ff, 4ul)
    pool_release(g_upool, u_eps, 4ul)
    pool_release(g_upool, u_totd, 4ul)
    pool_release(g_upool, u_totff, 4ul)
    pool_release(g_upool, u_n, 4ul)
    pool_release(g_upool, u_p0, 4ul)
    pool_release(g_upool, u_tmax, 4ul)
    pool_release(g_upool, u_ta, 4ul)
    pool_release(g_upool, u_len_self, 4ul)
    pool_release(g_upool, u_hh, 4ul)
    pool_release(g_upool, u_nch_s, 4ul)
    pool_release(g_upool, u_nch_c, 4ul)
    pool_release(g_upool, u_c1, 4ul)
    pool_release(g_upool, u_c0, 4ul)
    pool_release(g_upool, u_scale, 4ul)
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
    return true
}

[init]
def dasllama_metal_asr_dec_register {
    g_wd_env = g_env_metal.wdec
    g_wd_step_env = g_env_metal.wdec_step
    register_whisper_cross_kv_gpu(@@metal_whisper_cross_kv)
    register_whisper_decode_gpu(@@metal_whisper_decode)
}
