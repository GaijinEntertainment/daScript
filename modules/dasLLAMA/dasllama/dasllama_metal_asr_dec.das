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

//! Engage/decline counters (the counts live in dasllama_metal_common, the counting home).
def metal_wdec_stats : tuple<windows : int64; declines : int64> {
    var dtot = 0l
    for (v in values(g_wdec_declines_by_reason)) {
        dtot += v
    }
    return (windows = g_wdec_windows, declines = dtot)
}

//! The declines-by-reason table (a copy), keyed by `MetalWdecDecline` value names.
def metal_wdec_declines : table<string; int64> {
    return <- clone_to_move(g_wdec_declines_by_reason)
}

//! Test A/B seat: pin the ASR-decoder driver on/off in-process (the env knob's setter twin).
def set_metal_wdec(on : bool) {
    g_wd_env = on
}

[cold_path]   // the decline leg IS the fall-off-the-GPU path; its bookkeeping is not hot
def private wd_decline(why : MetalWdecDecline) : bool {
    note_wdec_decline("{why}")
    if (why != MetalWdecDecline.knob) {   // knob = policy; capability declines honor required mode
        require_or_panic("wdec", "{why}")
    }
    return false
}

//! Release the driver's GPU objects (the weight blob + the scratch pool) — the tests' leak-gate
//! rail and the model-switch seat; lazily re-attaches on the next served window.
def metal_wdec_shutdown {
    if (g_wd_mblob != null) {
        metal_release(g_wd_mblob)
        g_wd_mblob = null
    }
    g_wd_key = 0ul
    g_wd_len = 0l
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
    if (g_wd_mblob != null) {
        metal_release(g_wd_mblob)
        g_wd_mblob = null
    }
    let d = w.n_text_state
    let nl = w.n_text_layer
    var staging : array<uint8>
    staging |> reserve_resize((blen / 32l) * 34l + 16l * (nl * WD_MATS + 1l))
    let rep = select_matmul_backend_for_load()
    let mr = active_q8_layout_mr()
    let kgroup = active_q8_kgroup()
    let wbias = active_q8_wbias()
    var cur = 0ul
    g_wd_te_boff = wd_region(w, w.d_te_q_off, d, w.n_vocab, rep, mr, kgroup, wbias, staging, cur)
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
    g_wd_key = base
    g_wd_len = blen
    return true
}

// The per-window cross-KV memory on the GPU: per layer the ck/cv q8 GEMMs over the encoder
// rows, then the kx transpose-prescale and vx re-tile shuffles; one command buffer, readback
// into the CPU-layout ds.kx/ds.vx (the CPU decoder consumes them until the step driver lands).
[hot_path]
def private metal_whisper_cross_kv(w : WhisperModel; var ds : DecoderState; enc_out : array<float>) : bool {
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
    if (d % 64l != 0l) {   // the mm grid's n-tile; kdim%32 is the dec-quantize precondition
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
    var u_kscale = uniform_f32(1.0 / sqrt(sqrt(float(hs))))
    var u_tot = uniform_u32(uint(d * ta))
    var err = ""
    let ran = with_compute_encoder(g_queue, err) $(enc : MetalComputeEncoder?) {   // nolint:PERF026 — the error-text fetch runs only on the failure leg
        for (l in range64(nl)) {
            let lo = dec_layer_offsets(w, l)
            enc_gemm_mm_b(enc, g_wd_mblob, g_wd_boffs[l * WD_MATS + 5l], bx, btmp, 0ul, u_d, u_d, mp, d)
            enc_cross_kx(enc, btmp, bkx, uint64(l * d * ta * 4l), u_d, u_hs, u_ta, u_kscale, u_tot, d * ta)
            enc_gemm_mm_b(enc, g_wd_mblob, g_wd_boffs[l * WD_MATS + 6l], bx, btmp, 0ul, u_d, u_d, mp, d)
            enc_add_bias_rows(enc, btmp, 0ul, bblobf, uint64(lo.cv_b * 4l), u_d, u_tot, ta * d)
            enc_cross_vx(enc, btmp, bvx, uint64(l * d * ta * 4l), u_d, u_hs, u_ta, u_tot, d * ta)
        }
    }
    var served = false
    if (ran) {
        unsafe {
            memcpy(addr(ds.kx[0]), metal_buffer_contents(bkx), bytes_x)
            memcpy(addr(ds.vx[0]), metal_buffer_contents(bvx), bytes_x)
        }
        g_wdec_windows++
        served = true
    }
    pool_release(g_wd_pool, bx, bytes_rows)
    pool_release(g_wd_pool, btmp, bytes_rows)
    pool_release(g_wd_pool, bkx, bytes_x)
    pool_release(g_wd_pool, bvx, bytes_x)
    pool_release(g_upool, u_d, 4ul)
    pool_release(g_upool, u_hs, 4ul)
    pool_release(g_upool, u_ta, 4ul)
    pool_release(g_upool, u_kscale, 4ul)
    pool_release(g_upool, u_tot, 4ul)
    if (!served) {
        return wd_decline(MetalWdecDecline.gpu_error)
    }
    return true
}

[init]
def dasllama_metal_asr_dec_register {
    g_wd_env = g_env_metal.wdec
    register_whisper_cross_kv_gpu(@@metal_whisper_cross_kv)
}
