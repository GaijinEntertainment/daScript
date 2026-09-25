options gen2
options indenting = 4
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_metal_tower shared public

require dasllama/dasllama_lint public
require math
require dasllama/dasllama_env
require dasllama/dasllama_common
require dasllama/dasllama_math   // ensure_length - the tower scratch sizing
require dasllama/dasllama_metal_shapes
require dasllama/dasllama_gemma4uv
require dasllama/dasllama_gemma4v   // Gemma4vTower/Gemma4vState — the ViT block loop
require dasllama/dasllama_gemma3v   // Gemma3vTower/Gemma3vState — the SigLIP block loop
require dasllama/dasllama_qwen3v   // Qwen3vTower/Qwen3vState — the qwen3vl ViT block loop (+ deepstack stashes)
require dasllama/dasllama_qwen25v   // Qwen25vTower/Qwen25vState — the window-attention ViT block loop
require dasllama/dasllama_audio   // AudioTower/EncoderState — the whisper-class block loop
require dasllama/dasllama_qwen3a   // Qwen3aTower — the conv2d frontend hook
require dasllama/dasllama_gemma4a   // Gemma4aEncoder/Gemma4aState — the Conformer block loop
require dasllama/dasllama_canary    // CanaryEncoder/CanaryState — the FastConformer block loop
require dasllama/dasllama_parakeet  // ParakeetModel/ParakeetState — the same chain, parakeet's blob layout
require dasllama/dasllama_styletts2
require dasllama/dasllama_tts_blocks
require dasllama/dasllama_tts_types
require dasllama/dasllama_layout
require daslib/f16_cvt
require dasllama/dasllama_tower   // Clamp — the calibration record the gemma4a sites carry
require metal/das_metal_boost
require dasllama/dasllama_metal_common
require dasllama/dasllama_metal_kernels
require dasllama/dasllama_metal_prefill   // pf_enc_bf16_mm + the attention trio — the crowned builders

// The tower/embedder Metal driver: one-shot feed-forward encodes — no session, no KV, no mirror.
// Serves the gemma4uv embedder, the gemma4v ViT block loop, and the whisper-class audio block loop.
// Declines by returning false; the CPU chain is always the correct fallback.
// Engage evidence is counter deltas (metal_tower_stats), never "the model ran".

var private g_tw_failed = false
var private g_tw_pool : MetalBufferPool
var private g_tw_env_tower = true
var private g_tw_env_tower_f16 = true
var private g_tw_env_tower_flash = true
var private g_tw_ones : MetalBuffer?      // [TW_ONES_DIM] ones — the weightless-rms weight bind
let private TW_ONES_DIM = 8192

//! Engage/decline counters (the counts live in dasllama_metal_common, the counting home);
//! `blocks` = transformer blocks run (n_layer per whisper-class encode; 0 for gemma4uv).
//! Model-level gates assert DELTAS on these.
def metal_tower_stats : tuple<encodes : int64; rows : int64; blocks : int64; convs : int64; declines : int64> {
    return (encodes = g_tower_encodes, rows = g_tower_rows, blocks = g_tower_blocks,
        convs = g_tower_convs, declines = g_tower_declines.total)
}

//! The declines-by-reason table (a copy), keyed by `MetalTowerDecline` value names.
def metal_tower_declines : table<string; int64> {
    return <- clone_to_move(g_tower_declines.by_reason)
}

//! Test A/B seat: pin the tower driver on/off in-process (the env knob's setter twin).
def set_metal_tower(on : bool) {
    g_tw_env_tower = on
}

//! The tower knob as it stands, for a cell that puts it back where it found it.
def metal_tower_knob() : bool => g_tw_env_tower

//! The qwen3v f16-W route's A/B seat (DASLLAMA_METAL_TOWER_F16's in-process twin).
def set_metal_tower_f16(on : bool) {
    g_tw_env_tower_f16 = on
}

def get_metal_tower_f16 : bool {
    return g_tw_env_tower_f16
}

def get_metal_tower_flash : bool {
    return g_tw_env_tower_flash
}

//! The tower-flash route's A/B seat (DASLLAMA_METAL_TOWER_FLASH's in-process twin).
def set_metal_tower_flash(on : bool) {
    g_tw_env_tower_flash = on
}

//! Encodes whose block/merger GEMMs rode the f16-W shadow route — the engage witness.
def metal_tower_f16_encodes() : int64 => g_tower_f16_encodes

[cold_path]   // the decline leg IS the fall-off-the-GPU path; its bookkeeping is not hot
def private tw_decline(why : MetalTowerDecline) : bool {
    note_decline(g_tower_declines, "tower", "{why}", policy = why == MetalTowerDecline.knob)
    return false
}

//! Release the tower's own GPU objects (the ones buffer + the scratch pool) — the tests'
//! leak-gate rail, beside metal_decode_shutdown. The borrowed and registry PSOs are owned by
//! their homes; lazily re-initializes on the next served encode.
def metal_tower_shutdown {
    residency_reset()   // the set references pooled buffers the drain below releases
    if (g_tw_ones != null) {
        metal_release(g_tw_ones)
        g_tw_ones = null
    }
    if (g_tw_q3a_wpad != null) {
        metal_release(g_tw_q3a_wpad)
        g_tw_q3a_wpad = null
    }
    g_tw_q3a_key = 0ul
    g_tw_q3a_len = 0l
    st2_slab_drop()
    pool_drain(g_tw_pool)
    g_tw_failed = false
}

[cold_path]   // per-encode bring-up: the registry/prefill inits are latched-flag cheap
def private metal_tower_init : bool {
    // the registry owns the tower PSOs (metal_decode_init compiles them, via the prefill seat)
    if (g_tw_failed || !metal_common_init() || !metal_prefill_pso_init() || g_pso_ln == null) {
        return false
    }
    if (g_tw_ones == null) {
        g_tw_ones = metal_new_buffer(g_dev, uint64(TW_ONES_DIM * 4))
        unsafe {
            var p = reinterpret<float?>(metal_buffer_contents(g_tw_ones))
            for (i in range(TW_ONES_DIM)) {
                p[i] = 1.0
            }
        }
    }
    return true
}

// The gemma4uv chain on the driver: rows arrive as the CPU im2col output (s.x0), pad rows of
// the GPU buffers stay zero so the GEMM pins their outputs at 0; LN/posadd grids cover the
// LOGICAL rows only. One command buffer, one wait, one readback.
[hot_path]
def private metal_gemma4uv_encode(t : Gemma4uvEmbedder; var s : Gemma4uvState; npos, n_cols : int64; var out : array<float>) : bool {
    if (!g_tw_env_tower) {
        return tw_decline(MetalTowerDecline.knob)
    }
    let d = t.n_embd
    if (d % 64l != 0l || t.proj_dim % 64l != 0l || t.patch_dim % 32l != 0l || d % 32l != 0l
            || d > int64(TW_ONES_DIM) || t.patch_w % 4l != 0l || t.proj_w % 8l != 0l || t.pos_w % 4l != 0l) {
        return tw_decline(MetalTowerDecline.shape)
    }
    if (!metal_tower_init()) {   // per-encode: re-arms after any shutdown seat
        return tw_decline(MetalTowerDecline.device)
    }
    residency_flush()   // pin/commit before the first submission (the post-CPU-window slack)
    let mp = (npos + 31l) / 32l * 32l
    let mapped = t.image_map != null
    var bblob = plane_buffer(g_dev, unsafe(reinterpret<void?>(plane_at(t.blob, 0l))), uint64(length(t.blob) * 4l), mapped)
    var bx0 = pool_acquire_pinned(g_tw_pool, g_dev, uint64(mp * t.patch_dim * 4l))
    var bx1 = pool_acquire_pinned(g_tw_pool, g_dev, uint64(mp * d * 4l))
    var bout = pool_acquire_pinned(g_tw_pool, g_dev, uint64(mp * t.proj_dim * 4l))
    upload_rows_padded(bx0, unsafe(addr(s.x0[0])), npos, mp, t.patch_dim)
    var u_pdim = uniform_u32(uint(t.patch_dim))   // also the patch GEMM's k
    var u_d = uniform_u32(uint(d))                // also the patch GEMM's n and the proj GEMM's k
    var u_eps = uniform_f32(GEMMA4UV_LN_EPS)
    var u_rms_eps = uniform_f32(t.rms_eps)
    var u_nproj = uniform_u32(uint(t.proj_dim))
    var u_ncols = uniform_u32(uint(n_cols))
    var u_pe = uniform_u32(uint(t.pos_entries))
    var u_tot = uniform_u32(uint(npos * d))
    var err = ""
    let ran = with_compute_encoder(g_queue, err) $(enc : MetalComputeEncoder?) {   // nolint:PERF026 — the error-text fetch runs only on the failure leg
        // ln1 over the im2col rows, in place
        enc_ln(enc, bx0, bblob, uint64(t.ln1_w * 4l), bblob, uint64(t.ln1_b * 4l), bx0, u_pdim, u_eps, npos)
        // the patch GEMM: per-plane format (gemma-4's shipped mmproj is MIXED)
        if (t.patch_bf16) {
            var bwblob = plane_buffer(g_dev, unsafe(reinterpret<void?>(plane_at(t.wblob, 0l))), uint64(length(t.wblob) * 2l), mapped)
            pf_enc_bf16_mm(enc, bwblob, uint64(t.patch_w * 2l), bx0, bx1, u_pdim, u_d, mp, d, kdim = t.patch_dim)
        } else {
            enc_f32_mm(enc, bblob, uint64(t.patch_w * 4l), bx0, bx1, 0ul, u_pdim, u_d, mp, d)
        }
        enc_add_bias_rows(enc, bx1, 0ul, bblob, uint64(t.patch_b * 4l), u_d, u_tot, npos * d)
        enc_ln(enc, bx1, bblob, uint64(t.ln2_w * 4l), bblob, uint64(t.ln2_b * 4l), bx1, u_d, u_eps, npos)
        enc_posadd2d(enc, bx1, bblob, uint64(t.pos_w * 4l), u_d, u_ncols, u_pe, u_tot, npos * d)
        enc_ln(enc, bx1, bblob, uint64(t.ln3_w * 4l), bblob, uint64(t.ln3_b * 4l), bx1, u_d, u_eps, npos)
        pf_enc_rms(enc, bx1, 0ul, g_tw_ones, bx1, u_d, u_rms_eps, npos)
        if (t.proj_bf16) {
            var bwblob2 = plane_buffer(g_dev, unsafe(reinterpret<void?>(plane_at(t.wblob, 0l))), uint64(length(t.wblob) * 2l), mapped)
            pf_enc_bf16_mm(enc, bwblob2, uint64(t.proj_w * 2l), bx1, bout, u_d, u_nproj, mp, t.proj_dim, kdim = d)
        } else {
            enc_f32_mm(enc, bblob, uint64(t.proj_w * 4l), bx1, bout, 0ul, u_d, u_nproj, mp, t.proj_dim)
        }
    }
    var served = false
    if (ran) {
        unsafe {
            memcpy(addr(out[0]), metal_buffer_contents(bout), uint64(npos * t.proj_dim * 4l))
        }
        g_tower_encodes++
        g_tower_rows += npos
        served = true
    }
    pool_release(g_tw_pool, bx0, uint64(mp * t.patch_dim * 4l))
    pool_release(g_tw_pool, bx1, uint64(mp * d * 4l))
    pool_release(g_tw_pool, bout, uint64(mp * t.proj_dim * 4l))
    pool_release(g_upool, u_pdim, 4ul)
    pool_release(g_upool, u_d, 4ul)
    pool_release(g_upool, u_eps, 4ul)
    pool_release(g_upool, u_rms_eps, 4ul)
    pool_release(g_upool, u_nproj, 4ul)
    pool_release(g_upool, u_ncols, 4ul)
    pool_release(g_upool, u_pe, 4ul)
    pool_release(g_upool, u_tot, 4ul)
    if (!served) {
        return tw_decline(MetalTowerDecline.gpu_error)
    }
    return true
}

// The whisper-class block loop as ONE command buffer; the attention trio rides uend = npos (the
// non-causal uniform span). Frontend and tail stay CPU; f32 towers only. Pad-row garbage is
// CONTAINED: QK/AV zero k/v rows >= npos, softmax zeroes [uend, np32), pad outputs go unread.
[hot_path, arch(at="../ARCHITECTURE_IMAGE.md#image-tower-twin-plane")]
def private metal_tower_blocks(t : AudioTower; var s : EncoderState; npos : int64) : bool {   // nolint:STYLE038 — the dispatch chain is the shape
    if (!g_tw_env_tower) {
        return tw_decline(MetalTowerDecline.knob)
    }
    if (t.q8) {
        return tw_decline(MetalTowerDecline.quant_mode)
    }
    let d = t.d_model
    let hs = d / t.n_head
    if (d % 64l != 0l || t.n_ff % 64l != 0l || hs % 64l != 0l || t.layers_off % 4l != 0l || t.layer_stride % 4l != 0l) {
        return tw_decline(MetalTowerDecline.shape)
    }
    if (!metal_tower_init()) {   // per-encode: re-arms after any shutdown seat
        return tw_decline(MetalTowerDecline.device)
    }
    residency_flush()   // pin/commit before the first submission (the post-CPU-window slack)
    let mp = (npos + 31l) / 32l * 32l
    let nk64 = (npos + 63l) / 64l * 64l
    let ff = t.n_ff
    let mapped = t.image_map != null
    var bblob = plane_buffer(g_dev, unsafe(reinterpret<void?>(plane_at(t.fblob, 0l))), uint64(length(t.fblob) * 4l), mapped)
    let bytes_row = uint64(mp * d * 4l)
    let bytes_h = uint64(mp * ff * 4l)
    let bytes_att = uint64(t.n_head * mp * nk64 * 2l)
    let bytes_rowk = uint64(nk64 * d * 4l)   //! K panel: the QK grid walks nk64 key rows, past mp
    let bytes_stat = uint64(t.n_head * mp * 2l * 4l)
    let twin = g_tw_env_tower_f16 && length(t.wblob) > 0l
    let f16_route = twin && !t.blk_bf16 && pf_hmm_ready()
    let bf16_route = twin && t.blk_bf16
    let bytes_xh = uint64(mp * ff * 2l)
    var bxh = (f16_route || (bf16_route && pf_bf16_half_ready())) ? pool_acquire_pinned(g_tw_pool, g_dev, bytes_xh) : null
    var bwh = (f16_route || bf16_route) ? plane_buffer(g_dev, unsafe(reinterpret<void?>(plane_at(t.wblob, 0l))), uint64(length(t.wblob) * 2l), mapped) : null
    let f16w = f16_route && bwh != null && bxh != null
    let bf16w = bf16_route && bwh != null
    var bx = pool_acquire_pinned(g_tw_pool, g_dev, bytes_row)
    var bxb = pool_acquire_pinned(g_tw_pool, g_dev, bytes_row)
    var bxb2 = pool_acquire_pinned(g_tw_pool, g_dev, bytes_row)
    var bq = pool_acquire_pinned(g_tw_pool, g_dev, bytes_row)
    var bk = pool_acquire_pinned(g_tw_pool, g_dev, bytes_rowk)
    var bv = pool_acquire_pinned(g_tw_pool, g_dev, bytes_row)
    var bh = pool_acquire_pinned(g_tw_pool, g_dev, bytes_h)
    var batt = pool_acquire_pinned(g_tw_pool, g_dev, bytes_att)
    var bstat = pool_acquire_pinned(g_tw_pool, g_dev, bytes_stat)
    upload_rows_padded(bx, unsafe(addr(s.x[0])), npos, mp, d)
    zero_rows(bk, npos, nk64, d)
    var u_d = uniform_u32(uint(d))
    var u_ff = uniform_u32(uint(ff))
    var u_eps = uniform_f32(t.eps)
    var u_totd = uniform_u32(uint(npos * d))
    var u_totff = uniform_u32(uint(npos * ff))
    var ka = AttnArgs(qd = uint(d), kv_dim = uint(d), head_size = uint(hs), kv_mul = 1u,
        npos = uint(npos), np32 = uint(nk64), scale = 1.0 / sqrt(float(hs)),
        qoff = 0u, qrows = uint(mp), window = 0u, softcap = 0.0, hass = 0u, uend = uint(npos))
    var err = ""
    let ran = with_compute_encoder(g_queue, err) $(enc : MetalComputeEncoder?) {   // nolint:PERF026 — the error-text fetch runs only on the failure leg
        for (l in range64(t.n_layer)) {
            let lo = enc_layer_offsets(t, l)
            enc_ln(enc, bx, bblob, uint64(lo.ln1_w * 4l), bblob, uint64(lo.ln1_b * 4l), bxb, u_d, u_eps, npos)
            tw_mm(enc, f16w, bf16w, bwh, bblob, lo.q_w, bxb, bxh, bq, 0ul, u_d, u_d, mp, d, d, true)
            enc_add_bias_rows(enc, bq, 0ul, bblob, uint64(lo.q_b * 4l), u_d, u_totd, npos * d)
            tw_mm(enc, f16w, bf16w, bwh, bblob, lo.k_w, bxb, bxh, bk, 0ul, u_d, u_d, mp, d, d, false)
            if (t.has_k_bias) {
                enc_add_bias_rows(enc, bk, 0ul, bblob, uint64(lo.k_b * 4l), u_d, u_totd, npos * d)
            }
            tw_mm(enc, f16w, bf16w, bwh, bblob, lo.v_w, bxb, bxh, bv, 0ul, u_d, u_d, mp, d, d, false)
            enc_add_bias_rows(enc, bv, 0ul, bblob, uint64(lo.v_b * 4l), u_d, u_totd, npos * d)
            enc_qk_mm(enc, bq, bk, batt, ka, mp, nk64, t.n_head)
            enc_rowstat(enc, batt, ka, bstat, null, npos, t.n_head)
            enc_av_mm(enc, batt, bv, bxb2, ka, bstat, mp, t.n_head, hs)
            tw_mm(enc, f16w, bf16w, bwh, bblob, lo.o_w, bxb2, bxh, bxb, 0ul, u_d, u_d, mp, d, d, true)
            enc_add_bias_rows(enc, bxb, 0ul, bblob, uint64(lo.o_b * 4l), u_d, u_totd, npos * d)
            enc_add(enc, bx, 0ul, bxb, 0ul, u_totd, npos * d)
            enc_ln(enc, bx, bblob, uint64(lo.ln2_w * 4l), bblob, uint64(lo.ln2_b * 4l), bxb, u_d, u_eps, npos)
            tw_mm(enc, f16w, bf16w, bwh, bblob, lo.fc1_w, bxb, bxh, bh, 0ul, u_d, u_ff, mp, ff, d, true)
            enc_add_bias_rows(enc, bh, 0ul, bblob, uint64(lo.fc1_b * 4l), u_ff, u_totff, npos * ff)
            if (t.gelu_tanh) {
                enc_gelu_lut(enc, bh, 0ul, u_totff, npos * ff)
            } else {
                enc_gelu_erf(enc, bh, 0ul, u_totff, npos * ff)
            }
            tw_mm(enc, f16w, bf16w, bwh, bblob, lo.fc2_w, bh, bxh, bxb2, 0ul, u_ff, u_d, mp, d, ff, true)
            enc_add_bias_rows(enc, bxb2, 0ul, bblob, uint64(lo.fc2_b * 4l), u_d, u_totd, npos * d)
            enc_add(enc, bx, 0ul, bxb2, 0ul, u_totd, npos * d)
        }
    }
    var served = false
    if (ran) {
        unsafe {
            memcpy(addr(s.x[0]), metal_buffer_contents(bx), uint64(npos * d * 4l))
        }
        g_tower_encodes++
        g_tower_rows += npos
        g_tower_blocks += t.n_layer
        if (f16w || bf16w) {
            g_tower_f16_encodes++
        }
        served = true
    }
    pool_release(g_tw_pool, bx, bytes_row)
    pool_release(g_tw_pool, bxb, bytes_row)
    pool_release(g_tw_pool, bxb2, bytes_row)
    pool_release(g_tw_pool, bq, bytes_row)
    pool_release(g_tw_pool, bk, bytes_row)
    pool_release(g_tw_pool, bv, bytes_row)
    pool_release(g_tw_pool, bh, bytes_h)
    pool_release(g_tw_pool, batt, bytes_att)
    pool_release(g_tw_pool, bstat, bytes_stat)
    if (bxh != null) {
        pool_release(g_tw_pool, bxh, bytes_xh)
    }
    pool_release(g_upool, u_d, 4ul)
    pool_release(g_upool, u_ff, 4ul)
    pool_release(g_upool, u_eps, 4ul)
    pool_release(g_upool, u_totd, 4ul)
    pool_release(g_upool, u_totff, 4ul)
    if (!served) {
        return tw_decline(MetalTowerDecline.gpu_error)
    }
    return true
}

//! One clamped f32 GEMM on the gemma4a chain: clamp-in to bxc, the GEMM, clamp-out in place; an inactive record skips both passes.
def private g4a_mm_site(enc : MetalComputeEncoder?; bblob, bbounds : MetalBuffer?; c : Clamp; brec : uint64;
                        bsrc, bxc, bdst : MetalBuffer?; woff : int64; u_in, u_out, u_k, u_n, u_totin, u_totout : MetalBuffer?;
                        totin, totout, mp, ndim : int64) {
    if (c.active) {
        enc_clamp(enc, bsrc, 0ul, bxc, 0ul, bbounds, brec, u_in, u_totin, totin)
        enc_f32_mm(enc, bblob, uint64(woff * 4l), bxc, bdst, 0ul, u_k, u_n, mp, ndim)
        enc_clamp(enc, bdst, 0ul, bdst, 0ul, bbounds, brec, u_out, u_totout, totout)
    } else {
        enc_f32_mm(enc, bblob, uint64(woff * 4l), bsrc, bdst, 0ul, u_k, u_n, mp, ndim)
    }
}

def private g4a_rec(var bp : float?; idx : int64; c : Clamp) {
    unsafe {
        bp[idx * 4l] = c.imin
        bp[idx * 4l + 1l] = c.imax
        bp[idx * 4l + 2l] = c.omin
        bp[idx * 4l + 3l] = c.omax
    }
}

//! The tower context every encoder-family driver shares - the blob, the row / hidden /
//! position panels, the eight uniforms and the geometry; a family derives it with its extras.
struct private TwGpuCtx {
    bblob : MetalBuffer?
    bx : MetalBuffer?
    bxb : MetalBuffer?
    bq : MetalBuffer?
    bk : MetalBuffer?
    bv : MetalBuffer?
    bao : MetalBuffer?
    bglu : MetalBuffer?
    bcvd : MetalBuffer?
    bh : MetalBuffer?
    bpos : MetalBuffer?
    u_d : MetalBuffer?
    u_2d : MetalBuffer?
    u_ff : MetalBuffer?
    u_eps : MetalBuffer?
    u_totd : MetalBuffer?
    u_tot2d : MetalBuffer?
    u_totff : MetalBuffer?
    u_half : MetalBuffer?
    mp : int64
    np64 : int64
    npos : int64
    bytes_row : uint64
    bytes_ff : uint64
    bytes_pos : uint64
}

def private tw_ctx_make(var c : TwGpuCtx; blob : void?; blob_bytes : uint64; mapped : bool;
                        npos, d, n_ff : int64; eps, half : float; bytes_pos : uint64) {
    c.mp = (npos + 31l) / 32l * 32l
    c.np64 = (npos + 63l) / 64l
    c.npos = npos
    c.bblob = plane_buffer(g_dev, blob, blob_bytes, mapped)
    c.bytes_row = uint64(c.mp * d * 4l)
    c.bytes_ff = uint64(c.mp * n_ff * 4l)
    c.bytes_pos = bytes_pos
    c.bx = pool_acquire_pinned(g_tw_pool, g_dev, c.bytes_row)
    c.bxb = pool_acquire_pinned(g_tw_pool, g_dev, c.bytes_row)
    c.bq = pool_acquire_pinned(g_tw_pool, g_dev, c.bytes_row)
    c.bk = pool_acquire_pinned(g_tw_pool, g_dev, c.bytes_row)
    c.bv = pool_acquire_pinned(g_tw_pool, g_dev, c.bytes_row)
    c.bao = pool_acquire_pinned(g_tw_pool, g_dev, c.bytes_row)
    c.bglu = pool_acquire_pinned(g_tw_pool, g_dev, c.bytes_row)
    c.bcvd = pool_acquire_pinned(g_tw_pool, g_dev, c.bytes_row)
    c.bh = pool_acquire_pinned(g_tw_pool, g_dev, c.bytes_ff)
    c.bpos = pool_acquire_pinned(g_tw_pool, g_dev, c.bytes_pos)
    c.u_d = uniform_u32(uint(d))
    c.u_2d = uniform_u32(uint(2l * d))
    c.u_ff = uniform_u32(uint(n_ff))
    c.u_eps = uniform_f32(eps)
    c.u_totd = uniform_u32(uint(npos * d))
    c.u_tot2d = uniform_u32(uint(npos * 2l * d))
    c.u_totff = uniform_u32(uint(npos * n_ff))
    c.u_half = uniform_f32(half)
}

def private tw_ctx_release(var c : TwGpuCtx) {
    pool_release(g_tw_pool, c.bx, c.bytes_row)
    pool_release(g_tw_pool, c.bxb, c.bytes_row)
    pool_release(g_tw_pool, c.bq, c.bytes_row)
    pool_release(g_tw_pool, c.bk, c.bytes_row)
    pool_release(g_tw_pool, c.bv, c.bytes_row)
    pool_release(g_tw_pool, c.bao, c.bytes_row)
    pool_release(g_tw_pool, c.bglu, c.bytes_row)
    pool_release(g_tw_pool, c.bcvd, c.bytes_row)
    pool_release(g_tw_pool, c.bh, c.bytes_ff)
    pool_release(g_tw_pool, c.bpos, c.bytes_pos)
    pool_release(g_upool, c.u_d, 4ul)
    pool_release(g_upool, c.u_2d, 4ul)
    pool_release(g_upool, c.u_ff, 4ul)
    pool_release(g_upool, c.u_eps, 4ul)
    pool_release(g_upool, c.u_totd, 4ul)
    pool_release(g_upool, c.u_tot2d, 4ul)
    pool_release(g_upool, c.u_totff, 4ul)
    pool_release(g_upool, c.u_half, 4ul)
}

struct private G4aGpuCtx : TwGpuCtx {
    bbounds : MetalBuffer?
    bxc : MetalBuffer?
    bprpe : MetalBuffer?
    u_in : MetalBuffer?
    u_out : MetalBuffer?
    ka : G4aAttnArgs
    bytes_rec : uint64
}

def private g4a_shape_ok(t : Gemma4aEncoder) : bool {
    let d = t.d_model
    return (!t.q8 && d % 64l == 0l && t.n_ff % 64l == 0l && t.n_head * t.d_head == d
        && gemma4a_attn_past() <= 15l)
}

def private g4a_ctx_make(t : Gemma4aEncoder; npos : int64) : G4aGpuCtx {
    let d = t.d_model
    let past = gemma4a_attn_past()
    var c : G4aGpuCtx
    tw_ctx_make(c, unsafe(reinterpret<void?>(plane_at(t.blob, 0l))), uint64(length(t.blob) * 4l), t.image_map != null,
        npos, d, t.n_ff, t.eps, gemma4a_res_w(), uint64(32l * d * 4l))
    c.bytes_rec = uint64(t.n_layer * 10l * 16l)
    c.bxc = pool_acquire_pinned(g_tw_pool, g_dev, c.bytes_ff)
    c.bprpe = pool_acquire_pinned(g_tw_pool, g_dev, c.bytes_pos)
    c.bbounds = pool_acquire_pinned(g_tw_pool, g_dev, c.bytes_rec)
    unsafe {
        //! the RPE table, zero-padded to the GEMM's 32-row floor
        var pp = reinterpret<float?>(metal_buffer_contents(c.bpos))
        let sp = plane_at(t.pos_emb, 0l)
        for (i in range64((past + 1l) * d)) {
            pp[i] = sp[i]
        }
        for (i in range64((past + 1l) * d, 32l * d)) {
            pp[i] = 0.0
        }
        //! the calibration records, one [imin imax omin omax] row per clamped GEMM site
        var bp = reinterpret<float?>(metal_buffer_contents(c.bbounds))
        for (l in range64(t.n_layer)) {
            let lo = t.layers[l]
            g4a_rec(bp, l * 10l, lo.c_ff_up)
            g4a_rec(bp, l * 10l + 1l, lo.c_ff_down)
            g4a_rec(bp, l * 10l + 2l, lo.c_q)
            g4a_rec(bp, l * 10l + 3l, lo.c_k)
            g4a_rec(bp, l * 10l + 4l, lo.c_v)
            g4a_rec(bp, l * 10l + 5l, lo.c_o)
            g4a_rec(bp, l * 10l + 6l, lo.c_pw1)
            g4a_rec(bp, l * 10l + 7l, lo.c_pw2)
            g4a_rec(bp, l * 10l + 8l, lo.c_ff_up_1)
            g4a_rec(bp, l * 10l + 9l, lo.c_ff_down_1)
        }
    }
    c.u_in = uniform_u32(0u)    //! the clamp record's input pair
    c.u_out = uniform_u32(1u)   //! its output pair
    c.ka = G4aAttnArgs(d = uint(d), hs = uint(t.d_head), npos = uint(npos), past = uint(past),
        qscale = (1.0 / sqrt(float(t.d_head))) / log(2.0), kscale = log(1.0 + exp(1.0)) / log(2.0),
        cap = gemma4a_softcap())
    return c
}

def private g4a_ctx_release(var c : G4aGpuCtx) {
    pool_release(g_tw_pool, c.bbounds, c.bytes_rec)
    pool_release(g_tw_pool, c.bxc, c.bytes_ff)
    pool_release(g_tw_pool, c.bprpe, c.bytes_pos)
    pool_release(g_upool, c.u_in, 4ul)
    pool_release(g_upool, c.u_out, 4ul)
    tw_ctx_release(c)
}

//! The 12-block dispatch chain on an already-armed context - the blocks-only and whole-chunk drivers share it.
[arch(at="../ARCHITECTURE_GPU_TOWER.md#tower-encode-chains")]
def private g4a_blocks_body(enc : MetalComputeEncoder?; t : Gemma4aEncoder; c : G4aGpuCtx) {
    let d = t.d_model
    let ff = t.n_ff
    let npos = c.npos
    let mp = c.mp
    var ka = c.ka   //! the dispatch's args seat wants a non-ref value
    for (l in range64(t.n_layer)) {
        let lo = t.layers[l]
        let rb = uint64(l * 10l * 16l)
        var bw_ffn = upload_region(unsafe(reinterpret<void?>(plane_at(t.blob, lo.ff_norm))), uint64(d * 4l))
        var bw_ffp = upload_region(unsafe(reinterpret<void?>(plane_at(t.blob, lo.ff_post_norm))), uint64(d * 4l))
        var bw_apre = upload_region(unsafe(reinterpret<void?>(plane_at(t.blob, lo.attn_pre_norm))), uint64(d * 4l))
        var bw_apost = upload_region(unsafe(reinterpret<void?>(plane_at(t.blob, lo.attn_post_norm))), uint64(d * 4l))
        var bw_ncv = upload_region(unsafe(reinterpret<void?>(plane_at(t.blob, lo.norm_conv))), uint64(d * 4l))
        var bw_cvn = upload_region(unsafe(reinterpret<void?>(plane_at(t.blob, lo.conv_norm))), uint64(d * 4l))
        var bw_ffn1 = upload_region(unsafe(reinterpret<void?>(plane_at(t.blob, lo.ff_norm_1))), uint64(d * 4l))
        var bw_ffp1 = upload_region(unsafe(reinterpret<void?>(plane_at(t.blob, lo.ff_post_norm_1))), uint64(d * 4l))
        var bw_ln2 = upload_region(unsafe(reinterpret<void?>(plane_at(t.blob, lo.ln2))), uint64(d * 4l))
        var bpds = upload_region(unsafe(reinterpret<void?>(plane_at(t.blob, lo.per_dim_scale))), uint64(t.d_head * 4l))
        pf_enc_rms(enc, c.bx, 0ul, bw_ffn, c.bxb, c.u_d, c.u_eps, npos)
        g4a_mm_site(enc, c.bblob, c.bbounds, lo.c_ff_up, rb, c.bxb, c.bxc, c.bh, lo.ff_up, c.u_in, c.u_out, c.u_d, c.u_ff, c.u_totd, c.u_totff, npos * d, npos * ff, mp, ff)
        enc_g4a_silu(enc, c.bh, 0ul, c.u_totff, npos * ff)
        g4a_mm_site(enc, c.bblob, c.bbounds, lo.c_ff_down, rb + 16ul, c.bh, c.bxc, c.bxb, lo.ff_down, c.u_in, c.u_out, c.u_ff, c.u_d, c.u_totff, c.u_totd, npos * ff, npos * d, mp, d)
        pf_enc_rms(enc, c.bxb, 0ul, bw_ffp, c.bxb, c.u_d, c.u_eps, npos)
        enc_g4a_axpy(enc, c.bx, c.bxb, c.u_totd, c.u_half, npos * d)
        pf_enc_rms(enc, c.bx, 0ul, bw_apre, c.bxb, c.u_d, c.u_eps, npos)
        g4a_mm_site(enc, c.bblob, c.bbounds, lo.c_q, rb + 32ul, c.bxb, c.bxc, c.bq, lo.q, c.u_in, c.u_out, c.u_d, c.u_d, c.u_totd, c.u_totd, npos * d, npos * d, mp, d)
        g4a_mm_site(enc, c.bblob, c.bbounds, lo.c_k, rb + 48ul, c.bxb, c.bxc, c.bk, lo.k, c.u_in, c.u_out, c.u_d, c.u_d, c.u_totd, c.u_totd, npos * d, npos * d, mp, d)
        g4a_mm_site(enc, c.bblob, c.bbounds, lo.c_v, rb + 64ul, c.bxb, c.bxc, c.bv, lo.v, c.u_in, c.u_out, c.u_d, c.u_d, c.u_totd, c.u_totd, npos * d, npos * d, mp, d)
        enc_f32_mm(enc, c.bblob, uint64(lo.attn_k_rel * 4l), c.bpos, c.bprpe, 0ul, c.u_d, c.u_d, 32l, d)
        enc_g4a_attn(enc, c.bq, c.bk, c.bv, c.bao, c.bprpe, bpds, ka, c.np64, t.n_head)
        g4a_mm_site(enc, c.bblob, c.bbounds, lo.c_o, rb + 80ul, c.bao, c.bxc, c.bxb, lo.o, c.u_in, c.u_out, c.u_d, c.u_d, c.u_totd, c.u_totd, npos * d, npos * d, mp, d)
        pf_enc_rms(enc, c.bxb, 0ul, bw_apost, c.bxb, c.u_d, c.u_eps, npos)
        enc_add(enc, c.bx, 0ul, c.bxb, 0ul, c.u_totd, npos * d)
        pf_enc_rms(enc, c.bx, 0ul, bw_ncv, c.bxb, c.u_d, c.u_eps, npos)
        g4a_mm_site(enc, c.bblob, c.bbounds, lo.c_pw1, rb + 96ul, c.bxb, c.bxc, c.bh, lo.conv_pw1, c.u_in, c.u_out, c.u_d, c.u_2d, c.u_totd, c.u_tot2d, npos * d, npos * 2l * d, mp, 2l * d)
        enc_g4a_glu(enc, c.bh, c.bglu, c.u_d, c.u_totd, npos * d)
        enc_g4a_dw(enc, c.bglu, c.bcvd, c.bblob, uint64(lo.conv_dw * 4l), c.u_d, c.u_totd, npos * d)
        pf_enc_rms(enc, c.bcvd, 0ul, bw_cvn, c.bcvd, c.u_d, c.u_eps, npos)
        enc_g4a_silu(enc, c.bcvd, 0ul, c.u_totd, npos * d)
        g4a_mm_site(enc, c.bblob, c.bbounds, lo.c_pw2, rb + 112ul, c.bcvd, c.bxc, c.bxb, lo.conv_pw2, c.u_in, c.u_out, c.u_d, c.u_d, c.u_totd, c.u_totd, npos * d, npos * d, mp, d)
        enc_add(enc, c.bx, 0ul, c.bxb, 0ul, c.u_totd, npos * d)
        pf_enc_rms(enc, c.bx, 0ul, bw_ffn1, c.bxb, c.u_d, c.u_eps, npos)
        g4a_mm_site(enc, c.bblob, c.bbounds, lo.c_ff_up_1, rb + 128ul, c.bxb, c.bxc, c.bh, lo.ff_up_1, c.u_in, c.u_out, c.u_d, c.u_ff, c.u_totd, c.u_totff, npos * d, npos * ff, mp, ff)
        enc_g4a_silu(enc, c.bh, 0ul, c.u_totff, npos * ff)
        g4a_mm_site(enc, c.bblob, c.bbounds, lo.c_ff_down_1, rb + 144ul, c.bh, c.bxc, c.bxb, lo.ff_down_1, c.u_in, c.u_out, c.u_ff, c.u_d, c.u_totff, c.u_totd, npos * ff, npos * d, mp, d)
        pf_enc_rms(enc, c.bxb, 0ul, bw_ffp1, c.bxb, c.u_d, c.u_eps, npos)
        enc_g4a_axpy(enc, c.bx, c.bxb, c.u_totd, c.u_half, npos * d)
        pf_enc_rms(enc, c.bx, 0ul, bw_ln2, c.bx, c.u_d, c.u_eps, npos)
    }
}

//! The gemma4a Conformer block loop as ONE command buffer on the f32 blob; the whole-chunk twin below also carries the mel/front.
[hot_path]
def private metal_gemma4a_blocks(t : Gemma4aEncoder; var s : Gemma4aState; npos : int64) : bool {
    if (!g_tw_env_tower) {
        return tw_decline(MetalTowerDecline.knob)
    }
    if (t.q8) {
        return tw_decline(MetalTowerDecline.quant_mode)
    }
    if (!g4a_shape_ok(t) || npos < 1l) {
        return tw_decline(MetalTowerDecline.shape)
    }
    if (!metal_tower_init() || !pf_g4a_ready()) {   // per-encode: re-arms after any shutdown seat
        return tw_decline(MetalTowerDecline.device)
    }
    residency_flush()   // pin/commit before the first submission (the post-CPU-front slack)
    let d = t.d_model
    var c <- g4a_ctx_make(t, npos)
    upload_rows_padded(c.bx, unsafe(addr(s.x[0])), npos, c.mp, d)
    var err = ""
    let ran = with_compute_encoder(g_queue, err) $(enc : MetalComputeEncoder?) {   // nolint:PERF026 — the error-text fetch runs only on the failure leg
        g4a_blocks_body(enc, t, c)
    }
    let served = tw_readback(c, ran, s.x, d, t.n_layer)
    g4a_ctx_release(c)
    if (!served) {
        return tw_decline(MetalTowerDecline.gpu_error)
    }
    return true
}

let private FC_MAX_HEADS = 16   //!< the per-head uniform seats; the shape gate holds n_head to it

//! The FastConformer driver's shared GPU context (canary and parakeet) - g4a's shape with the rel table in place of the windowed RPE, and no clamp records.
struct private FcGpuCtx : TwGpuCtx {
    bposl : MetalBuffer?
    u_nk : MetalBuffer?
    u_np : MetalBuffer?
    ka : FcAttnArgs
    wwp : int64
    ww : int64
    n_head : int64
    tap_major : bool      //!< which depthwise stamp serves: parakeet's tap-major taps or canary's channel-major
    nkp : int64
    wwpad : int64
    bqu : MetalBuffer?
    bqv : MetalBuffer?
    bkh : MetalBuffer?
    bvt : MetalBuffer?
    bph : MetalBuffer?
    bsc : MetalBuffer?
    bsr : MetalBuffer?
    bp : MetalBuffer?
    boh : MetalBuffer?
    u_heads : MetalBuffer? [FC_MAX_HEADS]
    u_hs : MetalBuffer?
    u_nkp : MetalBuffer?
    u_wwpad : MetalBuffer?
    bytes_head : uint64
    bytes_kh : uint64
    bytes_ph : uint64
    bytes_sc : uint64
    bytes_sr : uint64
    bytes_oh : uint64
}

def private fc_ctx_make(blob : PlaneF; mapped : bool; d, n_ff, n_head, nk : int64; eps : float;
                        pos_emb : array<float>; npos, ww : int64; tap_major : bool) : FcGpuCtx {
    let dh = d / n_head
    var c = FcGpuCtx(wwp = (ww + 31l) / 32l * 32l, ww = ww, n_head = n_head, tap_major = tap_major,
        nkp = (npos + 63l) / 64l * 64l, wwpad = (ww + 63l) / 64l * 64l)
    tw_ctx_make(c, unsafe(reinterpret<void?>(plane_at(blob, 0l))), uint64(length(blob) * 4l), mapped,
        npos, d, n_ff, eps, 0.5, uint64(c.wwp * d * 4l))
    c.bposl = pool_acquire_pinned(g_tw_pool, g_dev, c.bytes_pos)
    unsafe {
        //! the sinusoidal rel table, zero-padded to the GEMM's row floor
        var pp = reinterpret<float?>(metal_buffer_contents(c.bpos))
        memcpy(reinterpret<uint8?>(pp), addr(pos_emb[0]), ww * d * 4l)
        for (i in range64(ww * d, c.wwp * d)) {
            pp[i] = 0.0
        }
    }
    c.u_nk = uniform_u32(uint(nk))
    c.u_np = uniform_u32(uint(npos))
    c.ka = fc_attn_args(d, dh, npos)
    c.bytes_head = uint64(c.mp * dh * 4l)
    c.bytes_kh = uint64(c.nkp * dh * 4l)
    c.bytes_ph = uint64(c.wwpad * dh * 4l)
    c.bytes_sc = uint64(c.mp * c.nkp * 4l)
    c.bytes_sr = uint64(c.mp * c.wwpad * 4l)
    c.bytes_oh = uint64(n_head * c.mp * dh * 4l)
    c.bqu = pool_acquire_pinned(g_tw_pool, g_dev, c.bytes_head)
    c.bqv = pool_acquire_pinned(g_tw_pool, g_dev, c.bytes_head)
    c.bkh = pool_acquire_pinned(g_tw_pool, g_dev, c.bytes_kh)
    c.bvt = pool_acquire_pinned(g_tw_pool, g_dev, c.bytes_kh)
    c.bph = pool_acquire_pinned(g_tw_pool, g_dev, c.bytes_ph)
    c.bsc = pool_acquire_pinned(g_tw_pool, g_dev, c.bytes_sc)
    c.bsr = pool_acquire_pinned(g_tw_pool, g_dev, c.bytes_sr)
    c.bp = pool_acquire_pinned(g_tw_pool, g_dev, c.bytes_sc)
    c.boh = pool_acquire_pinned(g_tw_pool, g_dev, c.bytes_oh)
    for (h in range64(n_head)) {
        c.u_heads[h] = uniform_u32(uint(h))
    }
    c.u_hs = uniform_u32(uint(dh))
    c.u_nkp = uniform_u32(uint(c.nkp))
    c.u_wwpad = uniform_u32(uint(c.wwpad))
    return <- c
}

def private fc_ctx_release(var c : FcGpuCtx) {
    pool_release(g_tw_pool, c.bposl, c.bytes_pos)
    pool_release(g_upool, c.u_nk, 4ul)
    pool_release(g_upool, c.u_np, 4ul)
    pool_release(g_tw_pool, c.bqu, c.bytes_head)
    pool_release(g_tw_pool, c.bqv, c.bytes_head)
    pool_release(g_tw_pool, c.bkh, c.bytes_kh)
    pool_release(g_tw_pool, c.bvt, c.bytes_kh)
    pool_release(g_tw_pool, c.bph, c.bytes_ph)
    pool_release(g_tw_pool, c.bsc, c.bytes_sc)
    pool_release(g_tw_pool, c.bsr, c.bytes_sr)
    pool_release(g_tw_pool, c.bp, c.bytes_sc)
    pool_release(g_tw_pool, c.boh, c.bytes_oh)
    for (h in range64(c.n_head)) {
        pool_release(g_upool, c.u_heads[h], 4ul)
    }
    pool_release(g_upool, c.u_hs, 4ul)
    pool_release(g_upool, c.u_nkp, 4ul)
    pool_release(g_upool, c.u_wwpad, 4ul)
    tw_ctx_release(c)
}

//! a GEMM's bias row-add, skipped where the family has none (a negative offset - parakeet's GEMMs carry no bias)
def private fc_bias(enc : MetalComputeEncoder?; c : FcGpuCtx; buf : MetalBuffer?; boff : int64; u_w, u_tot : MetalBuffer?; n : int64) {
    if (boff >= 0l) {
        enc_add_bias_rows(enc, buf, 0ul, c.bblob, uint64(boff * 4l), u_w, u_tot, n)
    }
}

[arch(at="../ARCHITECTURE_GPU_TOWER.md#tower-encode-chains")]
def private fc_attention(enc : MetalComputeEncoder?; c : FcGpuCtx; lo : CanaryLayerOffs) {
    var ka = c.ka
    let dh = int64(ka.hs)
    let mp = c.mp
    for (h in range64(c.n_head)) {
        enc_fc_pack(enc, c.bq, c.bk, c.bv, c.bposl, c.bblob, uint64(lo.bias_u * 4l), c.bblob, uint64(lo.bias_v * 4l),
            c.bqu, c.bqv, c.bkh, c.bvt, c.bph, ka, c.u_heads[h], c.wwpad * dh)
        enc_f32_mm(enc, c.bkh, 0ul, c.bqu, c.bsc, 0ul, c.u_hs, c.u_nkp, mp, c.nkp)
        enc_f32_mm(enc, c.bph, 0ul, c.bqv, c.bsr, 0ul, c.u_hs, c.u_wwpad, mp, c.wwpad)
        enc_fc_softmax(enc, c.bsc, c.bsr, c.bp, ka, mp)
        enc_f32_mm(enc, c.bvt, 0ul, c.bp, c.boh, uint64(h) * c.bytes_head, c.u_nkp, c.u_hs, mp, dh)
    }
    enc_fc_unpack(enc, c.boh, c.bao, ka, mp * int64(ka.d))
}

[arch(at="../ARCHITECTURE_GPU_TOWER.md#tower-encode-chains")]
def private fc_block(enc : MetalComputeEncoder?; c : FcGpuCtx; lo : CanaryLayerOffs; d, ff : int64) {
    let npos = c.npos
    let mp = c.mp
    enc_ln(enc, c.bx, c.bblob, uint64(lo.nf1_w * 4l), c.bblob, uint64(lo.nf1_b * 4l), c.bxb, c.u_d, c.u_eps, npos)
    enc_f32_mm(enc, c.bblob, uint64(lo.ff1a_w * 4l), c.bxb, c.bh, 0ul, c.u_d, c.u_ff, mp, ff)
    fc_bias(enc, c, c.bh, lo.ff1a_b, c.u_ff, c.u_totff, npos * ff)
    enc_g4a_silu(enc, c.bh, 0ul, c.u_totff, npos * ff)
    enc_f32_mm(enc, c.bblob, uint64(lo.ff1b_w * 4l), c.bh, c.bxb, 0ul, c.u_ff, c.u_d, mp, d)
    fc_bias(enc, c, c.bxb, lo.ff1b_b, c.u_d, c.u_totd, npos * d)
    enc_g4a_axpy(enc, c.bx, c.bxb, c.u_totd, c.u_half, npos * d)
    enc_ln(enc, c.bx, c.bblob, uint64(lo.nat_w * 4l), c.bblob, uint64(lo.nat_b * 4l), c.bxb, c.u_d, c.u_eps, npos)
    enc_f32_mm(enc, c.bblob, uint64(lo.q_w * 4l), c.bxb, c.bq, 0ul, c.u_d, c.u_d, mp, d)
    fc_bias(enc, c, c.bq, lo.q_b, c.u_d, c.u_totd, npos * d)
    enc_f32_mm(enc, c.bblob, uint64(lo.k_w * 4l), c.bxb, c.bk, 0ul, c.u_d, c.u_d, mp, d)
    fc_bias(enc, c, c.bk, lo.k_b, c.u_d, c.u_totd, npos * d)
    enc_f32_mm(enc, c.bblob, uint64(lo.v_w * 4l), c.bxb, c.bv, 0ul, c.u_d, c.u_d, mp, d)
    fc_bias(enc, c, c.bv, lo.v_b, c.u_d, c.u_totd, npos * d)
    enc_f32_mm(enc, c.bblob, uint64(lo.pos_w * 4l), c.bpos, c.bposl, 0ul, c.u_d, c.u_d, c.wwp, d)
    fc_attention(enc, c, lo)
    enc_f32_mm(enc, c.bblob, uint64(lo.o_w * 4l), c.bao, c.bxb, 0ul, c.u_d, c.u_d, mp, d)
    fc_bias(enc, c, c.bxb, lo.o_b, c.u_d, c.u_totd, npos * d)
    enc_add(enc, c.bx, 0ul, c.bxb, 0ul, c.u_totd, npos * d)
    enc_ln(enc, c.bx, c.bblob, uint64(lo.ncv_w * 4l), c.bblob, uint64(lo.ncv_b * 4l), c.bxb, c.u_d, c.u_eps, npos)
    enc_f32_mm(enc, c.bblob, uint64(lo.pw1_w * 4l), c.bxb, c.bh, 0ul, c.u_d, c.u_2d, mp, 2l * d)
    fc_bias(enc, c, c.bh, lo.pw1_b, c.u_2d, c.u_tot2d, npos * 2l * d)
    enc_g4a_glu(enc, c.bh, c.bglu, c.u_d, c.u_totd, npos * d)
    if (c.tap_major) {
        enc_pk_dw(enc, c.bglu, c.bcvd, c.bblob, uint64(lo.dw_w * 4l), c.bblob, uint64(lo.bn_w * 4l),
            c.bblob, uint64(lo.bn_b * 4l), c.u_d, c.u_nk, c.u_np, c.u_totd, npos * d)
    } else {
        enc_cn_dw(enc, c.bglu, c.bcvd, c.bblob, uint64(lo.dw_w * 4l), c.bblob, uint64(lo.bn_w * 4l),
            c.bblob, uint64(lo.bn_b * 4l), c.u_d, c.u_nk, c.u_np, c.u_totd, npos * d)
    }
    enc_f32_mm(enc, c.bblob, uint64(lo.pw2_w * 4l), c.bcvd, c.bxb, 0ul, c.u_d, c.u_d, mp, d)
    fc_bias(enc, c, c.bxb, lo.pw2_b, c.u_d, c.u_totd, npos * d)
    enc_add(enc, c.bx, 0ul, c.bxb, 0ul, c.u_totd, npos * d)
    enc_ln(enc, c.bx, c.bblob, uint64(lo.nf2_w * 4l), c.bblob, uint64(lo.nf2_b * 4l), c.bxb, c.u_d, c.u_eps, npos)
    enc_f32_mm(enc, c.bblob, uint64(lo.ff2a_w * 4l), c.bxb, c.bh, 0ul, c.u_d, c.u_ff, mp, ff)
    fc_bias(enc, c, c.bh, lo.ff2a_b, c.u_ff, c.u_totff, npos * ff)
    enc_g4a_silu(enc, c.bh, 0ul, c.u_totff, npos * ff)
    enc_f32_mm(enc, c.bblob, uint64(lo.ff2b_w * 4l), c.bh, c.bxb, 0ul, c.u_ff, c.u_d, mp, d)
    fc_bias(enc, c, c.bxb, lo.ff2b_b, c.u_d, c.u_totd, npos * d)
    enc_g4a_axpy(enc, c.bx, c.bxb, c.u_totd, c.u_half, npos * d)
    enc_ln(enc, c.bx, c.bblob, uint64(lo.nout_w * 4l), c.bblob, uint64(lo.nout_b * 4l), c.bx, c.u_d, c.u_eps, npos)
}

def private fc_shape_ok(d, n_ff, n_head, nk, npos, ww : int64) : bool {
    let dh = n_head > 0l ? d / n_head : 0l
    return (d % 64l == 0l && n_ff % 64l == 0l && n_head * dh == d
        && dh % 64l == 0l && n_head <= int64(FC_MAX_HEADS) && nk % 2l == 1l && npos >= 1l && ww == 2l * npos - 1l)
}

//! A served encode's tail: the residual rows back to the host and the engage counters; false when the command buffer failed.
def private tw_readback(c : TwGpuCtx; ran : bool; var x : array<float>; d, n_layer : int64) : bool {
    if (!ran) {
        return false
    }
    unsafe {
        memcpy(addr(x[0]), metal_buffer_contents(c.bx), uint64(c.npos * d * 4l))
    }
    g_tower_encodes++
    g_tower_rows += c.npos
    g_tower_blocks += n_layer
    return true
}

//! The FastConformer chain from the shape gate on - context, upload, the block loop over the
//! family's layer offsets, readback - as ONE command buffer on the f32 blob; the family hook
//! keeps its knob and quant-mode declines and the front stays CPU.
def private fc_run(blob : PlaneF; mapped : bool; d, n_ff, n_head, nk, n_layer : int64; eps : float; pos_emb : array<float>;
                   var x : array<float>; npos, ww : int64; tap_major : bool; layer : block<(l : int64) : CanaryLayerOffs>) : bool {
    if (!fc_shape_ok(d, n_ff, n_head, nk, npos, ww)) {
        return tw_decline(MetalTowerDecline.shape)
    }
    if (!metal_tower_init() || !pf_fc_ready(tap_major)) {   //! per-encode: re-arms after any shutdown seat
        return tw_decline(MetalTowerDecline.device)
    }
    residency_flush()   //! pin/commit before the first submission (the post-CPU-front slack)
    var c <- fc_ctx_make(blob, mapped, d, n_ff, n_head, nk, eps, pos_emb, npos, ww, tap_major)
    upload_rows_padded(c.bx, unsafe(addr(x[0])), npos, c.mp, d)
    var err = ""
    let ran = with_compute_encoder(g_queue, err) $(enc : MetalComputeEncoder?) {   // nolint:PERF026 — the error-text fetch runs only on the failure leg
        for (l in range64(n_layer)) {
            fc_block(enc, c, invoke(layer, l), d, n_ff)
        }
    }
    let served = tw_readback(c, ran, x, d, n_layer)
    fc_ctx_release(c)
    if (!served) {
        return tw_decline(MetalTowerDecline.gpu_error)
    }
    return true
}

//! The canary FastConformer block loop as ONE command buffer on the f32 blob; the subsample front, mel and final proj stay CPU.
[hot_path]
def private metal_canary_blocks(t : CanaryEncoder; var st : CanaryState; npos, ww : int64) : bool {
    if (!g_tw_env_tower) {
        return tw_decline(MetalTowerDecline.knob)
    }
    if (t.q8) {
        return tw_decline(MetalTowerDecline.quant_mode)
    }
    canary_pos_table(t, st, npos, ww)
    return fc_run(t.blob, t.image_map != null, t.d_model, t.n_ff, t.n_head, t.conv_kernel, t.n_layer, t.eps, st.pos_emb,
        st.x, npos, ww, false) $(l) {
        return t.layers[l]
    }
}

//! Parakeet's block offsets in the shared chain's layout: the same regions, no GEMM biases (-1).
def private pk_fc_layer(lo : PkLayerOffs) : CanaryLayerOffs {
    return CanaryLayerOffs(nf1_w = lo.nf1_w, nf1_b = lo.nf1_b,
        ff1a_w = lo.ff1a_w, ff1a_b = -1l, ff1b_w = lo.ff1b_w, ff1b_b = -1l,
        nat_w = lo.nat_w, nat_b = lo.nat_b,
        q_w = lo.q_w, q_b = -1l, k_w = lo.k_w, k_b = -1l, v_w = lo.v_w, v_b = -1l, o_w = lo.o_w, o_b = -1l,
        pos_w = lo.pos_w, bias_u = lo.bias_u, bias_v = lo.bias_v,
        ncv_w = lo.ncv_w, ncv_b = lo.ncv_b,
        pw1_w = lo.pw1_w, pw1_b = -1l, dw_w = lo.dw_w, dw_b = -1l,
        bn_w = lo.bn_w, bn_b = lo.bn_b, bn_mean = lo.bn_mean, bn_var = lo.bn_var,
        pw2_w = lo.pw2_w, pw2_b = -1l,
        nf2_w = lo.nf2_w, nf2_b = lo.nf2_b,
        ff2a_w = lo.ff2a_w, ff2a_b = -1l, ff2b_w = lo.ff2b_w, ff2b_b = -1l,
        nout_w = lo.nout_w, nout_b = lo.nout_b)
}

[hot_path]
def private metal_parakeet_blocks(t : ParakeetModel; var st : ParakeetState; npos, ww : int64) : bool {
    if (!g_tw_env_tower) {
        return tw_decline(MetalTowerDecline.knob)
    }
    if (t.q8) {
        return tw_decline(MetalTowerDecline.quant_mode)
    }
    return fc_run(t.blob, t.image_map != null, t.n_state, 4l * t.n_state, t.n_head, t.n_conv_kernel, t.n_layer, t.eps, st.pos_emb,
        st.x, npos, ww, true) $(l) {
        return pk_fc_layer(pk_layer_offsets(t, l))
    }
}

//! The whole-chunk gemma4a driver: from the CPU-windowed st.xw frames, ONE command buffer runs
//! DFT -> magnitude -> mel -> both subsample convs -> inproj -> the 12 blocks; st.x readback.
//! Returns npos, or -1 declined.
[hot_path, arch(at="../ARCHITECTURE_GPU_TOWER.md#tower-encode-chains")]
def private metal_gemma4a_chunk(t : Gemma4aEncoder; var s : Gemma4aState; n_len : int64) : int64 {   // nolint:STYLE038 — the dispatch chain is the shape
    if (!g_tw_env_tower || t.q8 || !g4a_shape_ok(t) || n_len < 4l) {
        return -1l
    }
    let n_mel = t.n_mel
    let n_bins = t.n_bins
    let n_fft = (n_bins - 1l) * 2l
    let t1 = (n_len - 1l) / 2l + 1l
    let w1 = (n_mel - 1l) / 2l + 1l
    let t2 = (t1 - 1l) / 2l + 1l
    let w2 = (w1 - 1l) / 2l + 1l
    let feat = w2 * t.sub_ch1
    let npos = t2
    if (n_fft % 32l != 0l || feat % 32l != 0l || feat != t.d_model || t.sub_ch0 % 64l != 0l || t.sub_ch0 != n_mel
            || !metal_tower_init() || !pf_g4a_ready() || !pf_g4a_front_ready()) {
        return -1l
    }
    residency_flush()
    let d = t.d_model
    let mpd = (n_len + 31l) / 32l * 32l
    let dft_out_pad = (2l * n_bins + 63l) / 64l * 64l
    let mag_k_pad = (n_bins + 31l) / 32l * 32l
    let rows0 = t1 * w1
    let mp0 = (rows0 + 31l) / 32l * 32l
    let conv0_k_pad = 32l
    let rows1 = t2 * w2
    let mp1 = (rows1 + 31l) / 32l * 32l
    let kd1 = t.sub_ch0 * 9l
    let conv1_cout_pad = (t.sub_ch1 + 63l) / 64l * 64l
    var c <- g4a_ctx_make(t, npos)
    let bytes_xw = uint64(mpd * n_fft * 4l)
    let bytes_tw = uint64(dft_out_pad * n_fft * 4l)
    let bytes_reim = uint64(mpd * dft_out_pad * 4l)
    let bytes_mag = uint64(mpd * mag_k_pad * 4l)
    let bytes_fil = uint64(n_mel * mag_k_pad * 4l)
    let bytes_mel = uint64(mpd * n_mel * 4l)
    let bytes_c0 = uint64(mp0 * conv0_k_pad * 4l)
    let bytes_w0 = uint64(t.sub_ch0 * conv0_k_pad * 4l)
    let bytes_y0 = uint64(mp0 * t.sub_ch0 * 4l)
    let bytes_c1 = uint64(mp1 * kd1 * 4l)
    let bytes_w1 = uint64(conv1_cout_pad * kd1 * 4l)
    let bytes_y1p = uint64(mp1 * conv1_cout_pad * 4l)
    var bxw = pool_acquire_pinned(g_tw_pool, g_dev, bytes_xw)
    var btw = pool_acquire_pinned(g_tw_pool, g_dev, bytes_tw)
    var breim = pool_acquire_pinned(g_tw_pool, g_dev, bytes_reim)
    var bmag = pool_acquire_pinned(g_tw_pool, g_dev, bytes_mag)
    var bfil = pool_acquire_pinned(g_tw_pool, g_dev, bytes_fil)
    var bmel = pool_acquire_pinned(g_tw_pool, g_dev, bytes_mel)
    var bcol0 = pool_acquire_pinned(g_tw_pool, g_dev, bytes_c0)
    var bw0 = pool_acquire_pinned(g_tw_pool, g_dev, bytes_w0)
    var by0 = pool_acquire_pinned(g_tw_pool, g_dev, bytes_y0)
    var bcol1 = pool_acquire_pinned(g_tw_pool, g_dev, bytes_c1)
    var bw1 = pool_acquire_pinned(g_tw_pool, g_dev, bytes_w1)
    var by1p = pool_acquire_pinned(g_tw_pool, g_dev, bytes_y1p)
    unsafe {
        //! xw upload, pad rows zeroed (the DFT GEMM walks mpd rows)
        upload_rows_padded(bxw, addr(s.xw[0]), n_len, mpd, n_fft)
        //! the transposed twiddle panel [dft_out_pad x n_fft]: W rows are output bins, zero-padded
        var tp = reinterpret<float?>(metal_buffer_contents(btw))
        let sp = plane_at(t.twiddles, 0l)
        for (n in range64(2l * n_bins)) {
            for (k in range64(n_fft)) {
                tp[n * n_fft + k] = sp[k * 2l * n_bins + n]
            }
        }
        for (i in range64(2l * n_bins * n_fft, dft_out_pad * n_fft)) {
            tp[i] = 0.0
        }
        //! the mel filterbank [n_mel x mag_k_pad], k-padded
        var fp = reinterpret<float?>(metal_buffer_contents(bfil))
        let flt = plane_at(t.filters, 0l)
        for (m in range64(n_mel)) {
            for (k in range64(n_bins)) {
                fp[m * mag_k_pad + k] = flt[m * n_bins + k]
            }
            for (k in range64(n_bins, mag_k_pad)) {
                fp[m * mag_k_pad + k] = 0.0
            }
        }
        //! conv0 weight [sub_ch0 x conv0_k_pad] (kdim 9 padded), conv1 weight [conv1_cout_pad x kd1] (cout padded)
        var w0p = reinterpret<float?>(metal_buffer_contents(bw0))
        let c0 = plane_at(t.blob, t.c0_w)
        for (o in range64(t.sub_ch0)) {
            for (k in range64(9l)) {
                w0p[o * conv0_k_pad + k] = c0[o * 9l + k]
            }
            for (k in range64(9l, conv0_k_pad)) {
                w0p[o * conv0_k_pad + k] = 0.0
            }
        }
        var w1p = reinterpret<float?>(metal_buffer_contents(bw1))
        let c1w = plane_at(t.blob, t.c1_w)
        for (i in range64(t.sub_ch1 * kd1)) {
            w1p[i] = c1w[i]
        }
        for (i in range64(t.sub_ch1 * kd1, conv1_cout_pad * kd1)) {
            w1p[i] = 0.0
        }
    }
    var u_kfft = uniform_u32(uint(n_fft))
    var u_dft_out_pad = uniform_u32(uint(dft_out_pad))
    var u_nbins = uniform_u32(uint(n_bins))
    var u_mag_k_pad = uniform_u32(uint(mag_k_pad))
    var u_totmag = uniform_u32(uint(mpd * mag_k_pad))
    var u_nmel = uniform_u32(uint(n_mel))
    var u_totmel = uniform_u32(uint(n_len * n_mel))
    var u_floor = uniform_f32(gemma4a_mel_floor())
    var u_iw0 = uniform_u32(uint(n_mel))
    var u_ih0 = uniform_u32(uint(n_len))
    var u_c1 = uniform_u32(1u)
    var u_ow0 = uniform_u32(uint(w1))
    var u_conv0_k_pad = uniform_u32(uint(conv0_k_pad))
    var u_totc0 = uniform_u32(uint(rows0 * conv0_k_pad))
    var u_ch0 = uniform_u32(uint(t.sub_ch0))
    var u_rows0 = uniform_u32(uint(rows0))
    var u_iw1 = uniform_u32(uint(w1))
    var u_ih1 = uniform_u32(uint(t1))
    var u_ow1 = uniform_u32(uint(w2))
    var u_kd1 = uniform_u32(uint(kd1))
    var u_totc1 = uniform_u32(uint(rows1 * kd1))
    var u_ch1 = uniform_u32(uint(t.sub_ch1))
    var u_conv1_cout_pad = uniform_u32(uint(conv1_cout_pad))
    var u_rows1 = uniform_u32(uint(rows1))
    var u_feat = uniform_u32(uint(feat))
    var err = ""
    let ran = with_compute_encoder(g_queue, err) $(enc : MetalComputeEncoder?) {   // nolint:PERF026 — the error-text fetch runs only on the failure leg
        enc_f32_mm(enc, btw, 0ul, bxw, breim, 0ul, u_kfft, u_dft_out_pad, mpd, dft_out_pad)
        enc_g4a_mag(enc, breim, bmag, u_nbins, u_dft_out_pad, u_mag_k_pad, u_totmag, mpd * mag_k_pad)
        enc_f32_mm(enc, bfil, 0ul, bmag, bmel, 0ul, u_mag_k_pad, u_nmel, mpd, n_mel)
        enc_g4a_logmel(enc, bmel, u_totmel, u_floor, n_len * n_mel)
        //! subsample conv0 over the mel image [n_len x n_mel x 1]
        enc_im2col2d(enc, bmel, 0ul, bcol0, u_iw0, u_ih0, u_c1, u_ow0, u_conv0_k_pad, u_c1, u_totc0, rows0 * conv0_k_pad)
        enc_f32_mm(enc, bw0, 0ul, bcol0, by0, 0ul, u_conv0_k_pad, u_ch0, mp0, t.sub_ch0)
        enc_g4a_lnrelu(enc, by0, by0, c.bblob, uint64(t.norm0_w * 4l), u_ch0, u_ch0, u_ch0, u_rows0, c.u_eps, rows0)
        //! subsample conv1 over the y0 image [t1 x w1 x sub_ch0], compact 32-wide out
        enc_im2col2d(enc, by0, 0ul, bcol1, u_iw1, u_ih1, u_ch0, u_ow1, u_kd1, u_ch0, u_totc1, rows1 * kd1)
        enc_f32_mm(enc, bw1, 0ul, bcol1, by1p, 0ul, u_kd1, u_conv1_cout_pad, mp1, conv1_cout_pad)
        enc_g4a_lnrelu(enc, by1p, c.bxb, c.bblob, uint64(t.norm1_w * 4l), u_ch1, u_conv1_cout_pad, u_ch1, u_rows1, c.u_eps, rows1)
        //! input projection: [npos x feat] -> [npos x d]
        enc_f32_mm(enc, c.bblob, uint64(t.inproj_w * 4l), c.bxb, c.bx, 0ul, u_feat, c.u_d, c.mp, d)
        g4a_blocks_body(enc, t, c)
    }
    var served = false
    if (ran) {
        ensure_length(s.x, npos * d)
        unsafe {
            memcpy(addr(s.x[0]), metal_buffer_contents(c.bx), uint64(npos * d * 4l))
        }
        g_tower_encodes++
        g_tower_rows += npos
        g_tower_blocks += t.n_layer
        g_tower_convs++
        served = true
    }
    pool_release(g_tw_pool, bxw, bytes_xw)
    pool_release(g_tw_pool, btw, bytes_tw)
    pool_release(g_tw_pool, breim, bytes_reim)
    pool_release(g_tw_pool, bmag, bytes_mag)
    pool_release(g_tw_pool, bfil, bytes_fil)
    pool_release(g_tw_pool, bmel, bytes_mel)
    pool_release(g_tw_pool, bcol0, bytes_c0)
    pool_release(g_tw_pool, bw0, bytes_w0)
    pool_release(g_tw_pool, by0, bytes_y0)
    pool_release(g_tw_pool, bcol1, bytes_c1)
    pool_release(g_tw_pool, bw1, bytes_w1)
    pool_release(g_tw_pool, by1p, bytes_y1p)
    pool_release(g_upool, u_kfft, 4ul)
    pool_release(g_upool, u_dft_out_pad, 4ul)
    pool_release(g_upool, u_nbins, 4ul)
    pool_release(g_upool, u_mag_k_pad, 4ul)
    pool_release(g_upool, u_totmag, 4ul)
    pool_release(g_upool, u_nmel, 4ul)
    pool_release(g_upool, u_totmel, 4ul)
    pool_release(g_upool, u_floor, 4ul)
    pool_release(g_upool, u_iw0, 4ul)
    pool_release(g_upool, u_ih0, 4ul)
    pool_release(g_upool, u_c1, 4ul)
    pool_release(g_upool, u_ow0, 4ul)
    pool_release(g_upool, u_conv0_k_pad, 4ul)
    pool_release(g_upool, u_totc0, 4ul)
    pool_release(g_upool, u_ch0, 4ul)
    pool_release(g_upool, u_rows0, 4ul)
    pool_release(g_upool, u_iw1, 4ul)
    pool_release(g_upool, u_ih1, 4ul)
    pool_release(g_upool, u_ow1, 4ul)
    pool_release(g_upool, u_kd1, 4ul)
    pool_release(g_upool, u_totc1, 4ul)
    pool_release(g_upool, u_ch1, 4ul)
    pool_release(g_upool, u_conv1_cout_pad, 4ul)
    pool_release(g_upool, u_rows1, 4ul)
    pool_release(g_upool, u_feat, 4ul)
    g4a_ctx_release(c)
    if (!served) {
        tw_decline(MetalTowerDecline.gpu_error)
        return -1l
    }
    return npos
}

// The gemma4v ViT block loop as ONE command buffer on the file's planes (bf16 GEMMs through the
// bf16 mul_mm, f32 norms/clamps) — the CPU g4v_block chain dispatch for dispatch, the attention
// trio at uend = npos / scale 1. Stem and tail stay CPU; a q8-lane tower declines (quant_mode).
[hot_path]
def private metal_gemma4v_blocks(t : Gemma4vTower; var s : Gemma4vState; npos : int64) : bool {   // nolint:STYLE038 — the dispatch chain is the shape
    if (!g_tw_env_tower) {
        return tw_decline(MetalTowerDecline.knob)
    }
    if (t.q8) {
        return tw_decline(MetalTowerDecline.quant_mode)
    }
    let d = t.n_embd
    let hs = d / t.n_head
    let ff = t.n_ff
    // bf16 GEMMs only; hs 64 = one attention K-slab; GEMM regions bind as ushort4 views (8-aligned halfword offsets)
    if (!t.blk_bf16 || d % 64l != 0l || ff % 64l != 0l || hs != 64l || t.layer_w % 8l != 0l
            || t.layer_w_stride % 8l != 0l) {
        return tw_decline(MetalTowerDecline.shape)
    }
    if (!metal_tower_init()) {   // per-encode: re-arms after any shutdown seat
        return tw_decline(MetalTowerDecline.device)
    }
    residency_flush()   // pin/commit before the first submission (the post-CPU-window slack)
    let mp = (npos + 31l) / 32l * 32l
    let nk64 = (npos + 63l) / 64l * 64l
    let pairs = hs / 4l   // rotated pairs per rope axis = one packed table row
    let mapped = t.image_map != null
    var bblob = plane_buffer(g_dev, unsafe(reinterpret<void?>(plane_at(t.blob, 0l))), uint64(length(t.blob) * 4l), mapped)
    var bwblob = plane_buffer(g_dev, unsafe(reinterpret<void?>(plane_at(t.wblob, 0l))), uint64(length(t.wblob) * 2l), mapped)
    let bytes_row = uint64(mp * d * 4l)
    let bytes_h = uint64(mp * ff * 4l)
    let bytes_att = uint64(t.n_head * mp * nk64 * 2l)
    let bytes_rowk = uint64(nk64 * d * 4l)   //! K panel: the QK grid walks nk64 key rows, past mp
    let bytes_stat = uint64(t.n_head * mp * 2l * 4l)
    let bytes_tabs = uint64(npos * 4l * pairs * 4l)
    var bx = pool_acquire_pinned(g_tw_pool, g_dev, bytes_row)
    var bxb = pool_acquire_pinned(g_tw_pool, g_dev, bytes_row)
    var bxb2 = pool_acquire_pinned(g_tw_pool, g_dev, bytes_row)
    var bq = pool_acquire_pinned(g_tw_pool, g_dev, bytes_row)
    var bk = pool_acquire_pinned(g_tw_pool, g_dev, bytes_rowk)
    var bv = pool_acquire_pinned(g_tw_pool, g_dev, bytes_row)
    var bhg = pool_acquire_pinned(g_tw_pool, g_dev, bytes_h)
    var bhu = pool_acquire_pinned(g_tw_pool, g_dev, bytes_h)
    var batt = pool_acquire_pinned(g_tw_pool, g_dev, bytes_att)
    var bstat = pool_acquire_pinned(g_tw_pool, g_dev, bytes_stat)
    var btabs = pool_acquire_pinned(g_tw_pool, g_dev, bytes_tabs)
    unsafe {
        upload_rows_padded(bx, addr(s.x[0]), npos, mp, d)
        zero_rows(bk, npos, nk64, d)
        gemma4v_pack_rope_tabs(s, npos, pairs, reinterpret<float?>(metal_buffer_contents(btabs)))
    }
    var u_d = uniform_u32(uint(d))
    var u_ff = uniform_u32(uint(ff))
    var u_hs = uniform_u32(uint(hs))
    var u_pairs = uniform_u32(uint(pairs))
    var u_eps = uniform_f32(t.eps)
    var u_totd = uniform_u32(uint(npos * d))
    var u_totff = uniform_u32(uint(npos * ff))
    var u_totrope = uniform_u32(uint(npos * t.n_head * pairs))
    var u_in = uniform_u32(0u)    // the clamp record's input pair
    var u_out = uniform_u32(1u)   // its output pair
    var ka = AttnArgs(qd = uint(d), kv_dim = uint(d), head_size = uint(hs), kv_mul = 1u,
        npos = uint(npos), np32 = uint(nk64), scale = 1.0,
        qoff = 0u, qrows = uint(mp), window = 0u, softcap = 0.0, hass = 0u, uend = uint(npos))
    var err = ""
    let ran = with_compute_encoder(g_queue, err) $(enc : MetalComputeEncoder?) {   // nolint:PERF026 — the error-text fetch runs only on the failure leg
        for (l in range64(t.n_layer)) {
            let o = gemma4v_block_offs(t, l)
            let cb_q = uint64(o.clamp[GEMMA4V_GEMM_Q] * 4l)
            let cb_k = uint64(o.clamp[GEMMA4V_GEMM_K] * 4l)
            let cb_v = uint64(o.clamp[GEMMA4V_GEMM_V] * 4l)
            let cb_o = uint64(o.clamp[GEMMA4V_GEMM_O] * 4l)
            let cb_gate = uint64(o.clamp[GEMMA4V_GEMM_GATE] * 4l)
            let cb_up = uint64(o.clamp[GEMMA4V_GEMM_UP] * 4l)
            let cb_down = uint64(o.clamp[GEMMA4V_GEMM_DOWN] * 4l)
            var bw_ln1 = upload_region(unsafe(reinterpret<void?>(plane_at(t.blob, o.ln1))), uint64(d * 4l))
            var bw_ln2 = upload_region(unsafe(reinterpret<void?>(plane_at(t.blob, o.ln2))), uint64(d * 4l))
            var bw_ap = upload_region(unsafe(reinterpret<void?>(plane_at(t.blob, o.attn_post))), uint64(d * 4l))
            var bw_fp = upload_region(unsafe(reinterpret<void?>(plane_at(t.blob, o.ffn_post))), uint64(d * 4l))
            var bw_qn = upload_region(unsafe(reinterpret<void?>(plane_at(t.blob, o.q_norm))), uint64(hs * 4l))
            var bw_kn = upload_region(unsafe(reinterpret<void?>(plane_at(t.blob, o.k_norm))), uint64(hs * 4l))
            // each of q/k/v clamps its own copy of the normed input into bxb2 (the bounds differ per GEMM)
            pf_enc_rms(enc, bx, 0ul, bw_ln1, bxb, u_d, u_eps, npos)
            enc_clamp(enc, bxb, 0ul, bxb2, 0ul, bblob, cb_q, u_in, u_totd, npos * d)
            pf_enc_bf16_mm(enc, bwblob, uint64(o.w[GEMMA4V_GEMM_Q] * 2l), bxb2, bq, u_d, u_d, mp, d, kdim = d)
            enc_clamp(enc, bq, 0ul, bq, 0ul, bblob, cb_q, u_out, u_totd, npos * d)
            enc_clamp(enc, bxb, 0ul, bxb2, 0ul, bblob, cb_k, u_in, u_totd, npos * d)
            pf_enc_bf16_mm(enc, bwblob, uint64(o.w[GEMMA4V_GEMM_K] * 2l), bxb2, bk, u_d, u_d, mp, d, kdim = d)
            enc_clamp(enc, bk, 0ul, bk, 0ul, bblob, cb_k, u_out, u_totd, npos * d)
            enc_clamp(enc, bxb, 0ul, bxb2, 0ul, bblob, cb_v, u_in, u_totd, npos * d)
            pf_enc_bf16_mm(enc, bwblob, uint64(o.w[GEMMA4V_GEMM_V] * 2l), bxb2, bv, u_d, u_d, mp, d, kdim = d)
            enc_clamp(enc, bv, 0ul, bv, 0ul, bblob, cb_v, u_out, u_totd, npos * d)
            pf_enc_rms(enc, bq, 0ul, bw_qn, bq, u_hs, u_eps, npos * t.n_head)
            pf_enc_rms(enc, bk, 0ul, bw_kn, bk, u_hs, u_eps, npos * t.n_head)
            pf_enc_rms(enc, bv, 0ul, g_tw_ones, bv, u_hs, u_eps, npos * t.n_head)
            enc_rope2d(enc, bq, btabs, u_d, u_pairs, u_totrope, npos * t.n_head * pairs)
            enc_rope2d(enc, bk, btabs, u_d, u_pairs, u_totrope, npos * t.n_head * pairs)
            enc_qk_mm(enc, bq, bk, batt, ka, mp, nk64, t.n_head)
            enc_rowstat(enc, batt, ka, bstat, null, npos, t.n_head)
            enc_av_mm(enc, batt, bv, bxb2, ka, bstat, mp, t.n_head, hs)
            // o-proj (clamped both sides) → post-norm in place → residual
            enc_clamp(enc, bxb2, 0ul, bxb2, 0ul, bblob, cb_o, u_in, u_totd, npos * d)
            pf_enc_bf16_mm(enc, bwblob, uint64(o.w[GEMMA4V_GEMM_O] * 2l), bxb2, bxb, u_d, u_d, mp, d, kdim = d)
            enc_clamp(enc, bxb, 0ul, bxb, 0ul, bblob, cb_o, u_out, u_totd, npos * d)
            pf_enc_rms(enc, bxb, 0ul, bw_ap, bxb, u_d, u_eps, npos)
            enc_add(enc, bx, 0ul, bxb, 0ul, u_totd, npos * d)
            // ffn: rms → gate / up (each its own input clamp) → GEGLU-quick → down → post-norm → residual
            pf_enc_rms(enc, bx, 0ul, bw_ln2, bxb, u_d, u_eps, npos)
            enc_clamp(enc, bxb, 0ul, bxb2, 0ul, bblob, cb_gate, u_in, u_totd, npos * d)
            pf_enc_bf16_mm(enc, bwblob, uint64(o.w[GEMMA4V_GEMM_GATE] * 2l), bxb2, bhg, u_d, u_ff, mp, ff, kdim = d)
            enc_clamp(enc, bhg, 0ul, bhg, 0ul, bblob, cb_gate, u_out, u_totff, npos * ff)
            enc_clamp(enc, bxb, 0ul, bxb2, 0ul, bblob, cb_up, u_in, u_totd, npos * d)
            pf_enc_bf16_mm(enc, bwblob, uint64(o.w[GEMMA4V_GEMM_UP] * 2l), bxb2, bhu, u_d, u_ff, mp, ff, kdim = d)
            enc_clamp(enc, bhu, 0ul, bhu, 0ul, bblob, cb_up, u_out, u_totff, npos * ff)
            enc_geglu_quick(enc, bhg, 0ul, bhu, 0ul, u_totff, npos * ff)
            enc_clamp(enc, bhg, 0ul, bhg, 0ul, bblob, cb_down, u_in, u_totff, npos * ff)
            pf_enc_bf16_mm(enc, bwblob, uint64(o.w[GEMMA4V_GEMM_DOWN] * 2l), bhg, bxb, u_ff, u_d, mp, d, kdim = ff)
            enc_clamp(enc, bxb, 0ul, bxb, 0ul, bblob, cb_down, u_out, u_totd, npos * d)
            pf_enc_rms(enc, bxb, 0ul, bw_fp, bxb, u_d, u_eps, npos)
            enc_add(enc, bx, 0ul, bxb, 0ul, u_totd, npos * d)
        }
    }
    var served = false
    if (ran) {
        unsafe {
            memcpy(addr(s.x[0]), metal_buffer_contents(bx), uint64(npos * d * 4l))
        }
        g_tower_encodes++
        g_tower_rows += npos
        g_tower_blocks += t.n_layer
        served = true
    }
    pool_release(g_tw_pool, bx, bytes_row)
    pool_release(g_tw_pool, bxb, bytes_row)
    pool_release(g_tw_pool, bxb2, bytes_row)
    pool_release(g_tw_pool, bq, bytes_row)
    pool_release(g_tw_pool, bk, bytes_row)
    pool_release(g_tw_pool, bv, bytes_row)
    pool_release(g_tw_pool, bhg, bytes_h)
    pool_release(g_tw_pool, bhu, bytes_h)
    pool_release(g_tw_pool, batt, bytes_att)
    pool_release(g_tw_pool, bstat, bytes_stat)
    pool_release(g_tw_pool, btabs, bytes_tabs)
    pool_release(g_upool, u_in, 4ul)
    pool_release(g_upool, u_out, 4ul)
    pool_release(g_upool, u_d, 4ul)
    pool_release(g_upool, u_ff, 4ul)
    pool_release(g_upool, u_hs, 4ul)
    pool_release(g_upool, u_pairs, 4ul)
    pool_release(g_upool, u_eps, 4ul)
    pool_release(g_upool, u_totd, 4ul)
    pool_release(g_upool, u_totff, 4ul)
    pool_release(g_upool, u_totrope, 4ul)
    if (!served) {
        return tw_decline(MetalTowerDecline.gpu_error)
    }
    return true
}

def private metal_tower_serves() : bool => g_tw_env_tower

// The gemma3v SigLIP block loop as ONE command buffer on the f32 blob — LN(w,b), biased f32 GEMMs,
// scaled attention, GELU. The 72-wide heads restride to the attention tiles' 128 (zero pads exact,
// scale stays 1/sqrt(72)); ffn serves at the layout's ff_pad. Stem/tail stay CPU; q8 declines.
[hot_path, arch(at="../ARCHITECTURE_IMAGE.md#image-tower-twin-plane"), arch(at="../ARCHITECTURE_GPU_TOWER.md#tower-attn-routes")]
def private metal_gemma3v_blocks(t : Gemma3vTower; var s : Gemma3vState; npos : int64) : bool {   // nolint:STYLE038 — the dispatch chain is the shape
    if (!g_tw_env_tower) {
        return tw_decline(MetalTowerDecline.knob)
    }
    if (t.q8) {
        return tw_decline(MetalTowerDecline.quant_mode)
    }
    let d = t.n_embd
    let hs = d / t.n_head
    let ff = t.ff_pad
    let hs_pad = max(64l, (hs + 31l) / 32l * 32l)
    if (d % 64l != 0l || ff % 64l != 0l || hs > 128l) {
        return tw_decline(MetalTowerDecline.shape)
    }
    if (!metal_tower_init()) {   // per-encode: re-arms after any shutdown seat
        return tw_decline(MetalTowerDecline.device)
    }
    residency_flush()   // pin/commit before the first submission (the post-CPU-window slack)
    let mp = (npos + 31l) / 32l * 32l
    let nk64 = (npos + 63l) / 64l * 64l
    let dp = t.n_head * hs_pad   // the padded attention width
    let mapped = t.image_map != null
    var bblob = plane_buffer(g_dev, unsafe(reinterpret<void?>(plane_at(t.blob, 0l))), uint64(length(t.blob) * 4l), mapped)
    let f16_route = g_tw_env_tower_f16 && length(t.wblob) > 0l && pf_hmm_ready()
    let flash = g_tw_env_tower_flash && hs == 72l && npos % 64l == 0l && pf_tower_flash_ready()
    let bytes_row = uint64(mp * d * 4l)
    let bytes_rowp = uint64(mp * dp * 4l)
    let bytes_rowk = uint64(nk64 * dp * 4l)   //! K panel: the QK grid walks nk64 key rows, past mp
    let bytes_h = uint64(mp * ff * 4l)
    let bytes_att = flash ? 64ul : uint64(t.n_head * mp * nk64 * 2l)
    let bytes_stat = uint64(t.n_head * mp * 2l * 4l)
    let bytes_xh = uint64(mp * ff * 2l)
    let bytes_hc = uint64(t.n_head * npos * hs * 2l + 512l)   //! per-head-contiguous f16 K/V (+ the flash's V tail slack)
    var bxh = f16_route ? pool_acquire_pinned(g_tw_pool, g_dev, bytes_xh) : null
    var bwh = f16_route ? plane_buffer(g_dev, unsafe(reinterpret<void?>(plane_at(t.wblob, 0l))), uint64(length(t.wblob) * 2l), mapped) : null
    let f16w = f16_route && bwh != null && bxh != null
    var bkhc = flash ? pool_acquire_pinned(g_tw_pool, g_dev, bytes_hc) : null
    var bvhc = flash ? pool_acquire_pinned(g_tw_pool, g_dev, bytes_hc) : null
    let pd = gemma3v_patch_pad(t)
    let bytes_x0 = uint64(mp * pd * 4l)
    var bx0 = pool_acquire_pinned(g_tw_pool, g_dev, bytes_x0)   //! im2col rows - the GPU stem's input
    var bx = pool_acquire_pinned(g_tw_pool, g_dev, bytes_row)
    var bxb = pool_acquire_pinned(g_tw_pool, g_dev, bytes_row)
    var bxb2 = pool_acquire_pinned(g_tw_pool, g_dev, bytes_row)
    var braw = pool_acquire_pinned(g_tw_pool, g_dev, bytes_row)   // GEMM output ahead of the head restride
    var bq = pool_acquire_pinned(g_tw_pool, g_dev, bytes_rowp)
    var bk = pool_acquire_pinned(g_tw_pool, g_dev, bytes_rowk)
    var bv = pool_acquire_pinned(g_tw_pool, g_dev, bytes_rowp)
    var bo_p = pool_acquire_pinned(g_tw_pool, g_dev, bytes_rowp)   // padded AV output ahead of the compact
    var bh = pool_acquire_pinned(g_tw_pool, g_dev, bytes_h)
    var batt = pool_acquire_pinned(g_tw_pool, g_dev, bytes_att)
    var bstat = pool_acquire_pinned(g_tw_pool, g_dev, bytes_stat)
    upload_rows_padded(bx0, unsafe(addr(s.x0[0])), npos, mp, pd)
    zero_rows(bk, npos, nk64, dp)
    var u_d = uniform_u32(uint(d))
    var u_ff = uniform_u32(uint(ff))
    var u_hs = uniform_u32(uint(hs))
    var u_hsp = uniform_u32(uint(hs_pad))
    var u_eps = uniform_f32(t.eps)
    var u_totd = uniform_u32(uint(npos * d))
    var u_totff = uniform_u32(uint(npos * ff))
    var u_totp = uniform_u32(uint(npos * t.n_head * hs_pad))   // restride total, pad direction
    var u_totc = uniform_u32(uint(npos * t.n_head * hs))       // restride total, compact direction
    var u_np = uniform_u32(uint(npos))
    var u_pd = uniform_u32(uint(pd))
    var ka = AttnArgs(qd = uint(dp), kv_dim = uint(dp), head_size = uint(hs_pad), kv_mul = 1u,
        npos = uint(npos), np32 = uint(nk64), scale = 1.0 / sqrt(float(hs)),
        qoff = 0u, qrows = uint(mp), window = 0u, softcap = 0.0, hass = 0u, uend = uint(npos))
    var kaf = AttnArgs(qd = uint(d), kv_dim = uint(d), head_size = uint(hs), kv_mul = 1u,
        npos = uint(npos), np32 = uint(npos), scale = 1.0 / sqrt(float(hs)),
        qoff = 0u, qrows = uint(mp), window = 0u, softcap = 0.0, hass = 0u, uend = uint(npos))
    var err = ""
    let ran = with_compute_encoder(g_queue, err) $(enc : MetalComputeEncoder?) {   // nolint:PERF026 — the error-text fetch runs only on the failure leg
        //! the stem: padded patch GEMM + conv bias + the learned positions (all row-aligned)
        enc_f32_mm(enc, bblob, uint64(t.patch_w * 4l), bx0, bx, 0ul, u_pd, u_d, mp, d)
        enc_add_bias_rows(enc, bx, 0ul, bblob, uint64(t.patch_b * 4l), u_d, u_totd, npos * d)
        enc_add(enc, bx, 0ul, bblob, uint64(t.pos_w * 4l), u_totd, npos * d)
        for (l in range64(t.n_layer)) {
            let o = gemma3v_block_offs(t, l)
            enc_ln(enc, bx, bblob, uint64(o.ln1_w * 4l), bblob, uint64(o.ln1_b * 4l), bxb, u_d, u_eps, npos)
            //! the flash reads Q compact, so its Q GEMM lands in bq directly (braw is reused by K/V)
            tw_mm(enc, f16w, false, bwh, bblob, o.w[GEMMA3V_GEMM_Q], bxb, bxh, flash ? bq : braw, 0ul, u_d, u_d, mp, d, d, true)
            enc_add_bias_rows(enc, flash ? bq : braw, 0ul, bblob, uint64(o.b[GEMMA3V_GEMM_Q] * 4l), u_d, u_totd, npos * d)
            if (!flash) {
                enc_head_restride(enc, braw, 0ul, bq, 0ul, u_hs, u_hsp, u_totp, npos * t.n_head * hs_pad)
            }
            tw_mm(enc, f16w, false, bwh, bblob, o.w[GEMMA3V_GEMM_K], bxb, bxh, braw, 0ul, u_d, u_d, mp, d, d, false)
            enc_add_bias_rows(enc, braw, 0ul, bblob, uint64(o.b[GEMMA3V_GEMM_K] * 4l), u_d, u_totd, npos * d)
            if (!flash) {
                enc_head_restride(enc, braw, 0ul, bk, 0ul, u_hs, u_hsp, u_totp, npos * t.n_head * hs_pad)
            } else {
                enc_tower_kv_hc(enc, braw, bkhc, u_hs, u_d, u_np, u_totd, npos * d)
            }
            tw_mm(enc, f16w, false, bwh, bblob, o.w[GEMMA3V_GEMM_V], bxb, bxh, braw, 0ul, u_d, u_d, mp, d, d, false)
            enc_add_bias_rows(enc, braw, 0ul, bblob, uint64(o.b[GEMMA3V_GEMM_V] * 4l), u_d, u_totd, npos * d)
            if (!flash) {
                enc_head_restride(enc, braw, 0ul, bv, 0ul, u_hs, u_hsp, u_totp, npos * t.n_head * hs_pad)
                enc_qk_mm(enc, bq, bk, batt, ka, mp, nk64, t.n_head)
                enc_rowstat(enc, batt, ka, bstat, null, npos, t.n_head)
                enc_av_mm(enc, batt, bv, bo_p, ka, bstat, mp, t.n_head, hs_pad)
                enc_head_restride(enc, bo_p, 0ul, bxb2, 0ul, u_hsp, u_hs, u_totc, npos * t.n_head * hs)
            } else {
                enc_tower_kv_hc(enc, braw, bvhc, u_hs, u_d, u_np, u_totd, npos * d)
                enc_tower_flash(enc, bq, bkhc, bvhc, bxb2, kaf, mp, t.n_head)
            }
            tw_mm(enc, f16w, false, bwh, bblob, o.w[GEMMA3V_GEMM_O], bxb2, bxh, bxb, 0ul, u_d, u_d, mp, d, d, true)
            enc_add_bias_rows(enc, bxb, 0ul, bblob, uint64(o.b[GEMMA3V_GEMM_O] * 4l), u_d, u_totd, npos * d)
            enc_add(enc, bx, 0ul, bxb, 0ul, u_totd, npos * d)
            enc_ln(enc, bx, bblob, uint64(o.ln2_w * 4l), bblob, uint64(o.ln2_b * 4l), bxb, u_d, u_eps, npos)
            tw_mm(enc, f16w, false, bwh, bblob, o.w[GEMMA3V_GEMM_UP], bxb, bxh, bh, 0ul, u_d, u_ff, mp, ff, d, true)
            enc_add_bias_rows(enc, bh, 0ul, bblob, uint64(o.b[GEMMA3V_GEMM_UP] * 4l), u_ff, u_totff, npos * ff)
            enc_gelu_lut(enc, bh, 0ul, u_totff, npos * ff)
            tw_mm(enc, f16w, false, bwh, bblob, o.w[GEMMA3V_GEMM_DOWN], bh, bxh, bxb2, 0ul, u_ff, u_d, mp, d, ff, true)
            enc_add_bias_rows(enc, bxb2, 0ul, bblob, uint64(o.b[GEMMA3V_GEMM_DOWN] * 4l), u_d, u_totd, npos * d)
            enc_add(enc, bx, 0ul, bxb2, 0ul, u_totd, npos * d)
        }
    }
    var served = false
    if (ran) {
        unsafe {
            memcpy(addr(s.x[0]), metal_buffer_contents(bx), uint64(npos * d * 4l))
        }
        g_tower_encodes++
        g_tower_rows += npos
        g_tower_blocks += t.n_layer
        if (f16w) {
            g_tower_f16_encodes++
        }
        served = true
    }
    pool_release(g_tw_pool, bx, bytes_row)
    pool_release(g_tw_pool, bxb, bytes_row)
    pool_release(g_tw_pool, bxb2, bytes_row)
    pool_release(g_tw_pool, braw, bytes_row)
    pool_release(g_tw_pool, bq, bytes_rowp)
    pool_release(g_tw_pool, bk, bytes_rowp)
    pool_release(g_tw_pool, bv, bytes_rowp)
    pool_release(g_tw_pool, bo_p, bytes_rowp)
    pool_release(g_tw_pool, bh, bytes_h)
    pool_release(g_tw_pool, batt, bytes_att)
    pool_release(g_tw_pool, bstat, bytes_stat)
    if (bxh != null) {
        pool_release(g_tw_pool, bxh, bytes_xh)
    }
    if (bkhc != null) {
        pool_release(g_tw_pool, bkhc, bytes_hc)
    }
    if (bvhc != null) {
        pool_release(g_tw_pool, bvhc, bytes_hc)
    }
    pool_release(g_tw_pool, bx0, bytes_x0)
    pool_release(g_upool, u_pd, 4ul)
    pool_release(g_upool, u_np, 4ul)
    pool_release(g_upool, u_d, 4ul)
    pool_release(g_upool, u_ff, 4ul)
    pool_release(g_upool, u_hs, 4ul)
    pool_release(g_upool, u_hsp, 4ul)
    pool_release(g_upool, u_eps, 4ul)
    pool_release(g_upool, u_totd, 4ul)
    pool_release(g_upool, u_totff, 4ul)
    pool_release(g_upool, u_totp, 4ul)
    pool_release(g_upool, u_totc, 4ul)
    if (!served) {
        return tw_decline(MetalTowerDecline.gpu_error)
    }
    return true
}

//! One block/merger GEMM: f16/bf16 read W from the baked twin plane at the f32 route's element
//! offset (bytes x2), X converting into the shared xh panel where the form takes it; f32 =
//! enc_f32_mm. `cvt` false reuses the panel a previous site converted.
[arch(at="../ARCHITECTURE_IMAGE.md#image-tower-twin-plane")]
def private tw_mm(enc : MetalComputeEncoder?; f16w, bf16w : bool; bwh, bblob : MetalBuffer?; welem : int64;
                  bx, bxh, by : MetalBuffer?; yoff : uint64; bk, bn : MetalBuffer?;
                  rows, nd, kdim : int64; cvt : bool) {
    if ((f16w || (bf16w && bxh != null)) && cvt) {
        var cka = CvtArgs(total = uint(rows * kdim))
        enc_cvt_half(enc, bx, bxh, cka, rows * kdim)
    }
    if (f16w) {
        pf_enc_hmm(enc, bwh, bxh, by, yoff, bk, bn, rows, nd, nd, kdim, uint64(welem * 2l))
    } elif (bf16w && yoff == 0ul) {   //! the bf16 builder has no y-offset seat; only the deepstack tap offsets, and no bf16 deepstack file exists
        pf_enc_bf16_mm(enc, bwh, uint64(welem * 2l), bx, by, bk, bn, rows, nd, bxh, kdim)
    } else {
        enc_f32_mm(enc, bblob, uint64(welem * 4l), bx, by, yoff, bk, bn, rows, nd)
    }
}

// The qwen3vl ViT: gemma3v's chain + the vision NEOX rope (enc_rope on compact q/k) + the
// fused qkv as three weight-offset GEMMs; tap chains encode inline over the live residual
// (hazard-ordered), the tail merger follows, ONE readback lands proj outputs for the scatter.
[hot_path, arch(at="../ARCHITECTURE_IMAGE.md#image-tower-twin-plane"), arch(at="../ARCHITECTURE_GPU_TOWER.md#tower-attn-routes")]
def private metal_qwen3v_blocks(t : Qwen3vTower; var s : Qwen3vState; npos : int64) : bool {   // nolint:STYLE038 — the dispatch chain is the shape
    if (!g_tw_env_tower) {
        return tw_decline(MetalTowerDecline.knob)
    }
    if (t.q8) {
        return tw_decline(MetalTowerDecline.quant_mode)
    }
    let d = t.n_embd
    let hs = d / t.n_head
    let ff = t.ff_pad
    let proj = t.proj_dim
    let hs_pad = max(64l, (hs + 31l) / 32l * 32l)
    //! %4 npos: the x4 merge reshape is the tap/tail GEMMs' row view; %64 patch_dim: the stem GEMM's K
    if (d % 64l != 0l || ff % 64l != 0l || hs > 128l || hs % 2l != 0l || proj % 64l != 0l || npos % 4l != 0l || t.patch_dim % 64l != 0l) {
        return tw_decline(MetalTowerDecline.shape)
    }
    if (!metal_tower_init()) {   // per-encode: re-arms after any shutdown seat
        return tw_decline(MetalTowerDecline.device)
    }
    residency_flush()   // pin/commit before the first submission (the post-CPU-window slack)
    let mp = (npos + 31l) / 32l * 32l
    let nk64 = (npos + 63l) / 64l * 64l
    let dp = t.n_head * hs_pad   // the padded attention width
    let half = hs / 2l
    let nout = npos / 4l
    let mp4 = (nout + 31l) / 32l * 32l
    let rows_ext = max(mp, mp4 * 4l)   // bx/bxb double as the [nout x 4d] merged view - pad rows beyond mp
    let mapped = t.image_map != null
    let blob_ptr = unsafe(reinterpret<void?>(plane_at(t.blob, 0l)))
    var bblob = plane_buffer(g_dev, blob_ptr, uint64(length(t.blob) * 4l), mapped)
    let bytes_row = uint64(rows_ext * d * 4l)
    let bytes_rowp = uint64(mp * dp * 4l)
    let bytes_rowk = uint64(nk64 * dp * 4l)   //! K panel: the QK grid walks nk64 key rows, past mp
    let bytes_h = uint64(mp * ff * 4l)
    let bytes_att = uint64(t.n_head * mp * nk64 * 2l)
    let bytes_stat = uint64(t.n_head * mp * 2l * 4l)
    let bytes_tab = uint64(npos * half * 4l)
    let bytes_m4 = uint64(mp4 * 4l * d * 4l)
    let bytes_pr = uint64(mp4 * proj * 4l)
    let nds = t.n_deepstack
    var bx = pool_acquire_pinned(g_tw_pool, g_dev, bytes_row)
    var bxb = pool_acquire_pinned(g_tw_pool, g_dev, bytes_row)
    var bxb2 = pool_acquire_pinned(g_tw_pool, g_dev, bytes_row)
    var braw = pool_acquire_pinned(g_tw_pool, g_dev, bytes_row)   // GEMM output ahead of rope + the head restride
    var bq = pool_acquire_pinned(g_tw_pool, g_dev, bytes_rowp)
    var bk = pool_acquire_pinned(g_tw_pool, g_dev, bytes_rowk)
    var bv = pool_acquire_pinned(g_tw_pool, g_dev, bytes_rowp)
    var bo_p = pool_acquire_pinned(g_tw_pool, g_dev, bytes_rowp)   // padded AV output ahead of the compact
    var bh = pool_acquire_pinned(g_tw_pool, g_dev, bytes_h)
    var batt = pool_acquire_pinned(g_tw_pool, g_dev, bytes_att)
    var bstat = pool_acquire_pinned(g_tw_pool, g_dev, bytes_stat)
    var bcos = pool_acquire_pinned(g_tw_pool, g_dev, bytes_tab)
    var bsin = pool_acquire_pinned(g_tw_pool, g_dev, bytes_tab)
    var bm4 = pool_acquire_pinned(g_tw_pool, g_dev, bytes_m4)     // tap/tail LN out, the [nout x 4d] merged view
    var bm4b = pool_acquire_pinned(g_tw_pool, g_dev, bytes_m4)    // fc1/mm0 out, GELU'd in place
    var btail = pool_acquire_pinned(g_tw_pool, g_dev, bytes_pr)
    var btap = nds > 0l ? pool_acquire_pinned(g_tw_pool, g_dev, uint64(nds) * bytes_pr) : null
    let twin = g_tw_env_tower_f16 && length(t.wblob) > 0l
    let f16_route = twin && !t.blk_bf16 && pf_hmm_ready()
    //! the bf16 builder has no y-offset seat, so a deepstack tap 1+ would silently split one
    //! encode across the bf16 and f32 routes - refuse the pairing until a seat exists
    let bf16_route = twin && t.blk_bf16 && nds == 0l
    let bytes_xh = uint64(max(mp * ff, mp4 * 4l * d) * 2l)
    var bxh = (f16_route || (bf16_route && pf_bf16_half_ready())) ? pool_acquire_pinned(g_tw_pool, g_dev, bytes_xh) : null
    var bwh = (f16_route || bf16_route) ? plane_buffer(g_dev, unsafe(reinterpret<void?>(plane_at(t.wblob, 0l))), uint64(length(t.wblob) * 2l), mapped) : null
    let bytes_x0 = uint64(mp * t.patch_dim * 4l)
    var bx0 = pool_acquire_pinned(g_tw_pool, g_dev, bytes_x0)   //! im2col rows - the GPU stem's input
    let pos_resized = qwen3v_pos_resized(t, s)
    var bposr = pos_resized ? pool_acquire_pinned(g_tw_pool, g_dev, uint64(npos * d * 4l)) : null
    unsafe {
        upload_rows_padded(bx0, addr(s.x0[0]), npos, mp, t.patch_dim)
        zero_rows(bk, npos, nk64, dp)
        zero_rows(bx, npos, rows_ext, d)
        if (bposr != null) {
            memcpy(metal_buffer_contents(bposr), addr(s.pos_rs[0]), npos * d * 4l)
        }
        memcpy(metal_buffer_contents(bcos), addr(s.cos_t[0]), npos * half * 4l)
        memcpy(metal_buffer_contents(bsin), addr(s.sin_t[0]), npos * half * 4l)
    }
    var u_d = uniform_u32(uint(d))
    var u_d4 = uniform_u32(uint(4l * d))
    var u_ff = uniform_u32(uint(ff))
    var u_pr = uniform_u32(uint(proj))
    var u_hs = uniform_u32(uint(hs))
    var u_hsp = uniform_u32(uint(hs_pad))
    var u_eps = uniform_f32(t.eps)
    var u_totd = uniform_u32(uint(npos * d))
    var u_totff = uniform_u32(uint(npos * ff))
    var u_totp = uniform_u32(uint(npos * t.n_head * hs_pad))   // restride total, pad direction
    var u_totc = uniform_u32(uint(npos * t.n_head * hs))       // restride total, compact direction
    var u_tot4 = uniform_u32(uint(nout * 4l * d))
    var u_totpr = uniform_u32(uint(nout * proj))
    var u_np_rope = uniform_u32(uint(npos * d / 2l))           // rope pairs over a compact q/k buffer
    var u_neox = uniform_u32(1u)
    var u_pd = uniform_u32(uint(t.patch_dim))
    var u_pw = uniform_u32(uint(s.grid_w))                     //! the stem assemble's merge-walk column count
    var ka = AttnArgs(qd = uint(dp), kv_dim = uint(dp), head_size = uint(hs_pad), kv_mul = 1u,
        npos = uint(npos), np32 = uint(nk64), scale = 1.0 / sqrt(float(hs)),
        qoff = 0u, qrows = uint(mp), window = 0u, softcap = 0.0, hass = 0u, uend = uint(npos))
    residency_flush()   // commit THIS encode's own notes before its first submission
    var err = ""
    let f16w = f16_route && bwh != null && bxh != null
    let bf16w = bf16_route && bwh != null
    let ran = with_compute_encoder(g_queue, err) $(enc : MetalComputeEncoder?) {   // nolint:PERF026 — the error-text fetch runs only on the failure leg
        enc_f32_mm(enc, bblob, uint64(t.patch_w * 4l), bx0, braw, 0ul, u_pd, u_d, mp, d)
        if (bposr != null) {
            enc_q3v_stem(enc, braw, bblob, uint64(t.patch_b * 4l), bposr, 0ul, bx, u_d, u_pw, u_totd, npos * d)
        } else {
            enc_q3v_stem(enc, braw, bblob, uint64(t.patch_b * 4l), bblob, uint64(t.pos_w * 4l), bx, u_d, u_pw, u_totd, npos * d)
        }
        for (l in range64(t.n_layer)) {
            let o = qwen3v_block_offs(t, l)
            enc_ln(enc, bx, bblob, uint64(o.ln1_w * 4l), bblob, uint64(o.ln1_b * 4l), bxb, u_d, u_eps, npos)
            let wqe = o.w[QWEN3V_GEMM_QKV]   //! the fused qkv rows: q, then k at d·d, v at 2d·d
            tw_mm(enc, f16w, bf16w, bwh, bblob, wqe, bxb, bxh, braw, 0ul, u_d, u_d, mp, d, d, true)
            enc_add_bias_rows(enc, braw, 0ul, bblob, uint64(o.qkv_b * 4l), u_d, u_totd, npos * d)
            enc_rope(enc, braw, 0ul, bcos, bsin, u_d, u_hs, u_np_rope, u_neox, null, npos * d / 2l)
            enc_head_restride(enc, braw, 0ul, bq, 0ul, u_hs, u_hsp, u_totp, npos * t.n_head * hs_pad)
            tw_mm(enc, f16w, bf16w, bwh, bblob, wqe + d * d, bxb, bxh, braw, 0ul, u_d, u_d, mp, d, d, false)
            enc_add_bias_rows(enc, braw, 0ul, bblob, uint64((o.qkv_b + d) * 4l), u_d, u_totd, npos * d)
            enc_rope(enc, braw, 0ul, bcos, bsin, u_d, u_hs, u_np_rope, u_neox, null, npos * d / 2l)
            enc_head_restride(enc, braw, 0ul, bk, 0ul, u_hs, u_hsp, u_totp, npos * t.n_head * hs_pad)
            tw_mm(enc, f16w, bf16w, bwh, bblob, wqe + 2l * d * d, bxb, bxh, braw, 0ul, u_d, u_d, mp, d, d, false)
            enc_add_bias_rows(enc, braw, 0ul, bblob, uint64((o.qkv_b + 2l * d) * 4l), u_d, u_totd, npos * d)
            enc_head_restride(enc, braw, 0ul, bv, 0ul, u_hs, u_hsp, u_totp, npos * t.n_head * hs_pad)
            enc_qk_mm(enc, bq, bk, batt, ka, mp, nk64, t.n_head)
            enc_rowstat(enc, batt, ka, bstat, null, npos, t.n_head)
            enc_av_mm(enc, batt, bv, bo_p, ka, bstat, mp, t.n_head, hs_pad)
            enc_head_restride(enc, bo_p, 0ul, bxb2, 0ul, u_hsp, u_hs, u_totc, npos * t.n_head * hs)
            tw_mm(enc, f16w, bf16w, bwh, bblob, o.w[QWEN3V_GEMM_O], bxb2, bxh, bxb, 0ul, u_d, u_d, mp, d, d, true)
            enc_add_bias_rows(enc, bxb, 0ul, bblob, uint64(o.out_b * 4l), u_d, u_totd, npos * d)
            enc_add(enc, bx, 0ul, bxb, 0ul, u_totd, npos * d)
            enc_ln(enc, bx, bblob, uint64(o.ln2_w * 4l), bblob, uint64(o.ln2_b * 4l), bxb, u_d, u_eps, npos)
            tw_mm(enc, f16w, bf16w, bwh, bblob, o.w[QWEN3V_GEMM_UP], bxb, bxh, bh, 0ul, u_d, u_ff, mp, ff, d, true)
            enc_add_bias_rows(enc, bh, 0ul, bblob, uint64(o.up_b * 4l), u_ff, u_totff, npos * ff)
            enc_gelu_lut(enc, bh, 0ul, u_totff, npos * ff)
            tw_mm(enc, f16w, bf16w, bwh, bblob, o.w[QWEN3V_GEMM_DOWN], bh, bxh, bxb2, 0ul, u_ff, u_d, mp, d, ff, true)
            enc_add_bias_rows(enc, bxb2, 0ul, bblob, uint64(o.down_b * 4l), u_d, u_totd, npos * d)
            enc_add(enc, bx, 0ul, bxb2, 0ul, u_totd, npos * d)
            let k = qwen3v_tap_of(t, l)
            if (k >= 0l) {   // the tap merger chain over the live residual's [nout x 4d] merged view
                let fo = t.ds_f + k * t.ds_f_stride
                let wo = t.ds_w + k * t.ds_w_stride
                enc_ln(enc, bx, bblob, uint64(fo * 4l), bblob, uint64((fo + 4l * d) * 4l), bm4, u_d4, u_eps, nout)
                tw_mm(enc, f16w, bf16w, bwh, bblob, wo, bm4, bxh, bm4b, 0ul, u_d4, u_d4, mp4, 4l * d, 4l * d, true)
                enc_add_bias_rows(enc, bm4b, 0ul, bblob, uint64((fo + 8l * d) * 4l), u_d4, u_tot4, nout * 4l * d)
                enc_gelu_lut(enc, bm4b, 0ul, u_tot4, nout * 4l * d)
                tw_mm(enc, f16w, bf16w, bwh, bblob, wo + 16l * d * d, bm4b, bxh, btap, uint64(k * mp4 * proj * 4l), u_d4, u_pr, mp4, proj, 4l * d, true)
                enc_add_bias_rows(enc, btap, uint64(k * mp4 * proj * 4l), bblob, uint64((fo + 12l * d) * 4l), u_pr, u_totpr, nout * proj)
            }
        }
        // the tail: post-LN over [npos x d], then the merger MLP over the [nout x 4d] view
        enc_ln(enc, bx, bblob, uint64(t.post_ln_w * 4l), bblob, uint64(t.post_ln_b * 4l), bxb, u_d, u_eps, npos)
        tw_mm(enc, f16w, bf16w, bwh, bblob, t.mm0_w, bxb, bxh, bm4b, 0ul, u_d4, u_d4, mp4, 4l * d, 4l * d, true)
        enc_add_bias_rows(enc, bm4b, 0ul, bblob, uint64(t.mm0_b * 4l), u_d4, u_tot4, nout * 4l * d)
        enc_gelu_lut(enc, bm4b, 0ul, u_tot4, nout * 4l * d)
        tw_mm(enc, f16w, bf16w, bwh, bblob, t.mm2_w, bm4b, bxh, btail, 0ul, u_d4, u_pr, mp4, proj, 4l * d, true)
        enc_add_bias_rows(enc, btail, 0ul, bblob, uint64(t.mm2_b * 4l), u_pr, u_totpr, nout * proj)
    }
    var served = false
    if (ran) {
        unsafe {
            memcpy(addr(s.ds_slice[0]), metal_buffer_contents(btail), uint64(nout * proj * 4l))
            for (k in range64(nds)) {
                memcpy(addr(s.ds_stash[k * nout * proj]),
                       reinterpret<uint8?>(metal_buffer_contents(btap)) + k * mp4 * proj * 4l, uint64(nout * proj * 4l))
            }
        }
        g_tower_encodes++
        g_tower_rows += npos
        g_tower_blocks += t.n_layer
        if (f16w || bf16w) {
            g_tower_f16_encodes++
        }
        served = true
    }
    pool_release(g_tw_pool, bx, bytes_row)
    pool_release(g_tw_pool, bxb, bytes_row)
    pool_release(g_tw_pool, bxb2, bytes_row)
    pool_release(g_tw_pool, braw, bytes_row)
    pool_release(g_tw_pool, bq, bytes_rowp)
    pool_release(g_tw_pool, bk, bytes_rowp)
    pool_release(g_tw_pool, bv, bytes_rowp)
    pool_release(g_tw_pool, bo_p, bytes_rowp)
    pool_release(g_tw_pool, bh, bytes_h)
    pool_release(g_tw_pool, batt, bytes_att)
    pool_release(g_tw_pool, bstat, bytes_stat)
    pool_release(g_tw_pool, bcos, bytes_tab)
    pool_release(g_tw_pool, bsin, bytes_tab)
    pool_release(g_tw_pool, bm4, bytes_m4)
    pool_release(g_tw_pool, bm4b, bytes_m4)
    pool_release(g_tw_pool, btail, bytes_pr)
    if (btap != null) {
        pool_release(g_tw_pool, btap, uint64(nds) * bytes_pr)
    }
    if (bxh != null) {
        pool_release(g_tw_pool, bxh, bytes_xh)
    }
    pool_release(g_tw_pool, bx0, bytes_x0)
    if (bposr != null) {
        pool_release(g_tw_pool, bposr, uint64(npos * d * 4l))
    }
    pool_release(g_upool, u_pd, 4ul)
    pool_release(g_upool, u_pw, 4ul)
    pool_release(g_upool, u_d, 4ul)
    pool_release(g_upool, u_d4, 4ul)
    pool_release(g_upool, u_ff, 4ul)
    pool_release(g_upool, u_pr, 4ul)
    pool_release(g_upool, u_hs, 4ul)
    pool_release(g_upool, u_hsp, 4ul)
    pool_release(g_upool, u_eps, 4ul)
    pool_release(g_upool, u_totd, 4ul)
    pool_release(g_upool, u_totff, 4ul)
    pool_release(g_upool, u_totp, 4ul)
    pool_release(g_upool, u_totc, 4ul)
    pool_release(g_upool, u_tot4, 4ul)
    pool_release(g_upool, u_totpr, 4ul)
    pool_release(g_upool, u_np_rope, 4ul)
    pool_release(g_upool, u_neox, 4ul)
    if (!served) {
        return tw_decline(MetalTowerDecline.gpu_error)
    }
    return true
}

//! The qwen25v window ViT as ONE command buffer: conv + window-sort gather stem, RMS-normed
//! blocks, swiglu FFN at ff_pad, the merger tail; ONE readback lands sorted proj rows in s.mout.
[hot_path, arch(at="../ARCHITECTURE_IMAGE.md#image-tower-twin-plane"), arch(at="../ARCHITECTURE_GPU_TOWER.md#tower-attn-routes")]
def private metal_qwen25v_blocks(t : Qwen25vTower; var s : Qwen25vState; npos : int64) : bool {   // nolint:STYLE038 — the dispatch chain is the shape
    if (!g_tw_env_tower) {
        return tw_decline(MetalTowerDecline.knob)
    }
    let d = t.n_embd
    let hs = d / t.n_head
    let ff = t.ff_pad
    let proj = t.proj_dim
    let hs_pad = max(64l, (hs + 31l) / 32l * 32l)
    let nwin = long_length(s.win_lo) - 1l
    let max_wr = qwen25v_window_widest(s)
    let pd = q25v_patch_pad(t)
    //! hs/max_wr cap = the window kernel's tgmem tile (64 rows x 80); %4 npos = the merged row view
    if (d % 64l != 0l || ff % 64l != 0l || hs > 80l || hs % 2l != 0l || proj % 64l != 0l
            || npos % 4l != 0l || pd % 64l != 0l || max_wr > 64l) {
        return tw_decline(MetalTowerDecline.shape)
    }
    if (!metal_tower_init() || !pf_tower_win_ready()) {   // per-encode: re-arms after any shutdown seat
        return tw_decline(MetalTowerDecline.device)
    }
    residency_flush()   // pin/commit before the first submission (the post-CPU-window slack)
    let mp = (npos + 31l) / 32l * 32l
    let nk64 = (npos + 63l) / 64l * 64l
    let dp = t.n_head * hs_pad   // the padded attention width (the full layers' slab)
    let half = hs / 2l
    let nout = npos / 4l
    let mp4 = (nout + 31l) / 32l * 32l
    let rows_ext = max(mp, mp4 * 4l)   //! bx/bxb double as the [nout x 4d] merged view - pad rows beyond mp
    let mapped = t.image_map != null
    var bblob = plane_buffer(g_dev, unsafe(reinterpret<void?>(plane_at(t.blob, 0l))), uint64(length(t.blob) * 4l), mapped)
    let twin = g_tw_env_tower_f16 && length(t.wblob) > 0l
    let f16_route = twin && !t.blk_bf16 && pf_hmm_ready()
    let bf16_route = twin && t.blk_bf16
    let bytes_row = uint64(rows_ext * d * 4l)
    let bytes_rowp = uint64(mp * dp * 4l)
    let bytes_rowk = uint64(nk64 * dp * 4l)   //! K panel: the QK grid walks nk64 key rows, past mp
    let bytes_h = uint64(mp * ff * 4l)
    let bytes_att = uint64(t.n_head * mp * nk64 * 2l)
    let bytes_stat = uint64(t.n_head * mp * 2l * 4l)
    let bytes_tab = uint64(npos * half * 4l)
    let bytes_m4 = uint64(mp4 * 4l * d * 4l)
    let bytes_pr = uint64(mp4 * proj * 4l)
    let bytes_x0 = uint64(mp * pd * 4l)
    let bytes_idx = uint64(npos * 4l)
    let bytes_wlo = uint64((nwin + 1l) * 4l)
    var bx = pool_acquire_pinned(g_tw_pool, g_dev, bytes_row)
    var bxb = pool_acquire_pinned(g_tw_pool, g_dev, bytes_row)
    var bxb2 = pool_acquire_pinned(g_tw_pool, g_dev, bytes_row)
    var braw = pool_acquire_pinned(g_tw_pool, g_dev, bytes_row)   // GEMM output ahead of rope + the head restride
    var bq = pool_acquire_pinned(g_tw_pool, g_dev, bytes_rowp)
    var bk = pool_acquire_pinned(g_tw_pool, g_dev, bytes_rowk)
    var bv = pool_acquire_pinned(g_tw_pool, g_dev, bytes_rowp)
    var bo_p = pool_acquire_pinned(g_tw_pool, g_dev, bytes_rowp)   // padded AV output ahead of the compact
    var bh = pool_acquire_pinned(g_tw_pool, g_dev, bytes_h)       // ffn gate (swiglu'd in place)
    var bhu = pool_acquire_pinned(g_tw_pool, g_dev, bytes_h)      // ffn up
    var batt = pool_acquire_pinned(g_tw_pool, g_dev, bytes_att)
    var bstat = pool_acquire_pinned(g_tw_pool, g_dev, bytes_stat)
    var bcos = pool_acquire_pinned(g_tw_pool, g_dev, bytes_tab)
    var bsin = pool_acquire_pinned(g_tw_pool, g_dev, bytes_tab)
    var bm4b = pool_acquire_pinned(g_tw_pool, g_dev, bytes_m4)    // mm0 out, GELU'd in place
    var btail = pool_acquire_pinned(g_tw_pool, g_dev, bytes_pr)
    var bx0 = pool_acquire_pinned(g_tw_pool, g_dev, bytes_x0)     //! im2col rows - the GPU stem's input
    var bsrc = pool_acquire_pinned(g_tw_pool, g_dev, bytes_idx)   //! the window-sort gather indices
    var bwlo = pool_acquire_pinned(g_tw_pool, g_dev, bytes_wlo)   //! per-window first row + total sentinel
    let bytes_xh = uint64(max(mp * ff, mp4 * 4l * d) * 2l)
    var bxh = (f16_route || (bf16_route && pf_bf16_half_ready())) ? pool_acquire_pinned(g_tw_pool, g_dev, bytes_xh) : null
    var bwh = (f16_route || bf16_route) ? plane_buffer(g_dev, unsafe(reinterpret<void?>(plane_at(t.wblob, 0l))), uint64(length(t.wblob) * 2l), mapped) : null
    unsafe {
        memcpy(metal_buffer_contents(bx0), addr(s.x0[0]), npos * pd * 4l)
        if (mp > npos) {
            var p0 = reinterpret<uint8?>(metal_buffer_contents(bx0))
            var z0 = reinterpret<float?>(p0 + npos * pd * 4l)
            for (i in range64((mp - npos) * pd)) {
                z0[i] = 0.0
            }
        }
        if (nk64 > npos) {
            var pk = reinterpret<uint8?>(metal_buffer_contents(bk))
            var zk = reinterpret<float?>(pk + npos * dp * 4l)
            for (i in range64((nk64 - npos) * dp)) {
                zk[i] = 0.0
            }
        }
        memcpy(metal_buffer_contents(bcos), addr(s.cos_t[0]), npos * half * 4l)
        memcpy(metal_buffer_contents(bsin), addr(s.sin_t[0]), npos * half * 4l)
        var sp = reinterpret<uint?>(metal_buffer_contents(bsrc))
        for (r in range64(npos)) {
            sp[r] = uint(s.src_of[r])
        }
        qwen25v_window_table(s, reinterpret<uint?>(metal_buffer_contents(bwlo)))
        zero_rows(bx, npos, rows_ext, d)
    }
    var bw_pl = upload_region(unsafe(reinterpret<void?>(plane_at(t.blob, t.post_ln_w))), uint64(d * 4l))
    var u_d = uniform_u32(uint(d))
    var u_d4 = uniform_u32(uint(4l * d))
    var u_ff = uniform_u32(uint(ff))
    var u_pr = uniform_u32(uint(proj))
    var u_hs = uniform_u32(uint(hs))
    var u_hsp = uniform_u32(uint(hs_pad))
    var u_eps = uniform_f32(t.eps)
    var u_totd = uniform_u32(uint(npos * d))
    var u_totff = uniform_u32(uint(npos * ff))
    var u_totp = uniform_u32(uint(npos * t.n_head * hs_pad))   // restride total, pad direction
    var u_totc = uniform_u32(uint(npos * t.n_head * hs))       // restride total, compact direction
    var u_tot4 = uniform_u32(uint(nout * 4l * d))
    var u_totpr = uniform_u32(uint(nout * proj))
    var u_np_rope = uniform_u32(uint(npos * d / 2l))           // rope pairs over a compact q/k buffer
    var u_neox = uniform_u32(1u)
    var u_pd = uniform_u32(uint(pd))
    var ka = AttnArgs(qd = uint(dp), kv_dim = uint(dp), head_size = uint(hs_pad), kv_mul = 1u,
        npos = uint(npos), np32 = uint(nk64), scale = 1.0 / sqrt(float(hs)),
        qoff = 0u, qrows = uint(mp), window = 0u, softcap = 0.0, hass = 0u, uend = uint(npos))
    var kaw = AttnArgs(qd = uint(d), kv_dim = uint(d), head_size = uint(hs), kv_mul = 1u,
        npos = uint(npos), np32 = uint(npos), scale = 1.0 / sqrt(float(hs)),
        qoff = 0u, qrows = uint(mp), window = 0u, softcap = 0.0, hass = 0u, uend = uint(npos))
    residency_flush()   // commit THIS encode's own notes before its first submission
    var err = ""
    let f16w = f16_route && bwh != null && bxh != null
    let bf16w = bf16_route && bwh != null
    let ran = with_compute_encoder(g_queue, err) $(enc : MetalComputeEncoder?) {   // nolint:PERF026 — the error-text fetch runs only on the failure leg
        enc_f32_mm(enc, bblob, uint64(t.patch_w * 4l), bx0, braw, 0ul, u_pd, u_d, mp, d)
        enc_tower_row_gather(enc, braw, bx, bsrc, u_d, u_totd, npos * d)
        for (l in range64(t.n_layer)) {
            let o = qwen25v_block_offs(t, l)
            let full = qwen25v_layer_full(t, l)
            var bw_ln1 = upload_region(unsafe(reinterpret<void?>(plane_at(t.blob, o.ln1_w))), uint64(d * 4l))
            var bw_ln2 = upload_region(unsafe(reinterpret<void?>(plane_at(t.blob, o.ln2_w))), uint64(d * 4l))
            pf_enc_rms(enc, bx, 0ul, bw_ln1, bxb, u_d, u_eps, npos)
            //! the window kernel reads q/k/v compact (bq/bk/bv direct); full layers restride from braw
            tw_mm(enc, f16w, bf16w, bwh, bblob, o.w[QWEN25V_GEMM_Q], bxb, bxh, full ? braw : bq, 0ul, u_d, u_d, mp, d, d, true)
            enc_add_bias_rows(enc, full ? braw : bq, 0ul, bblob, uint64(o.b[QWEN25V_GEMM_Q] * 4l), u_d, u_totd, npos * d)
            enc_rope(enc, full ? braw : bq, 0ul, bcos, bsin, u_d, u_hs, u_np_rope, u_neox, null, npos * d / 2l)
            if (full) {
                enc_head_restride(enc, braw, 0ul, bq, 0ul, u_hs, u_hsp, u_totp, npos * t.n_head * hs_pad)
            }
            tw_mm(enc, f16w, bf16w, bwh, bblob, o.w[QWEN25V_GEMM_K], bxb, bxh, full ? braw : bk, 0ul, u_d, u_d, mp, d, d, false)
            enc_add_bias_rows(enc, full ? braw : bk, 0ul, bblob, uint64(o.b[QWEN25V_GEMM_K] * 4l), u_d, u_totd, npos * d)
            enc_rope(enc, full ? braw : bk, 0ul, bcos, bsin, u_d, u_hs, u_np_rope, u_neox, null, npos * d / 2l)
            if (full) {
                enc_head_restride(enc, braw, 0ul, bk, 0ul, u_hs, u_hsp, u_totp, npos * t.n_head * hs_pad)
            }
            tw_mm(enc, f16w, bf16w, bwh, bblob, o.w[QWEN25V_GEMM_V], bxb, bxh, full ? braw : bv, 0ul, u_d, u_d, mp, d, d, false)
            enc_add_bias_rows(enc, full ? braw : bv, 0ul, bblob, uint64(o.b[QWEN25V_GEMM_V] * 4l), u_d, u_totd, npos * d)
            if (full) {
                enc_head_restride(enc, braw, 0ul, bv, 0ul, u_hs, u_hsp, u_totp, npos * t.n_head * hs_pad)
                enc_qk_mm(enc, bq, bk, batt, ka, mp, nk64, t.n_head)
                enc_rowstat(enc, batt, ka, bstat, null, npos, t.n_head)
                enc_av_mm(enc, batt, bv, bo_p, ka, bstat, mp, t.n_head, hs_pad)
                enc_head_restride(enc, bo_p, 0ul, bxb2, 0ul, u_hsp, u_hs, u_totc, npos * t.n_head * hs)
            } else {
                enc_tower_win_attn(enc, bq, bk, bv, bxb2, bwlo, kaw, nwin, t.n_head)
            }
            tw_mm(enc, f16w, bf16w, bwh, bblob, o.w[QWEN25V_GEMM_O], bxb2, bxh, bxb, 0ul, u_d, u_d, mp, d, d, true)
            enc_add_bias_rows(enc, bxb, 0ul, bblob, uint64(o.b[QWEN25V_GEMM_O] * 4l), u_d, u_totd, npos * d)
            enc_add(enc, bx, 0ul, bxb, 0ul, u_totd, npos * d)
            pf_enc_rms(enc, bx, 0ul, bw_ln2, bxb, u_d, u_eps, npos)
            tw_mm(enc, f16w, bf16w, bwh, bblob, o.w[QWEN25V_GEMM_GATE], bxb, bxh, bh, 0ul, u_d, u_ff, mp, ff, d, true)
            enc_add_bias_rows(enc, bh, 0ul, bblob, uint64(o.b[QWEN25V_GEMM_GATE] * 4l), u_ff, u_totff, npos * ff)
            tw_mm(enc, f16w, bf16w, bwh, bblob, o.w[QWEN25V_GEMM_UP], bxb, bxh, bhu, 0ul, u_d, u_ff, mp, ff, d, false)
            enc_add_bias_rows(enc, bhu, 0ul, bblob, uint64(o.b[QWEN25V_GEMM_UP] * 4l), u_ff, u_totff, npos * ff)
            enc_swiglu(enc, bh, 0ul, bhu, 0ul, u_totff, npos * ff)
            tw_mm(enc, f16w, bf16w, bwh, bblob, o.w[QWEN25V_GEMM_DOWN], bh, bxh, bxb2, 0ul, u_ff, u_d, mp, d, ff, true)
            enc_add_bias_rows(enc, bxb2, 0ul, bblob, uint64(o.b[QWEN25V_GEMM_DOWN] * 4l), u_d, u_totd, npos * d)
            enc_add(enc, bx, 0ul, bxb2, 0ul, u_totd, npos * d)
        }
        //! the tail: RMS post-norm over [npos x d], then the merger MLP over the [nout x 4d] view
        pf_enc_rms(enc, bx, 0ul, bw_pl, bxb, u_d, u_eps, npos)
        tw_mm(enc, f16w, bf16w, bwh, bblob, t.mm0_w, bxb, bxh, bm4b, 0ul, u_d4, u_d4, mp4, 4l * d, 4l * d, true)
        enc_add_bias_rows(enc, bm4b, 0ul, bblob, uint64(t.mm0_b * 4l), u_d4, u_tot4, nout * 4l * d)
        enc_gelu_lut(enc, bm4b, 0ul, u_tot4, nout * 4l * d)
        tw_mm(enc, f16w, bf16w, bwh, bblob, t.mm2_w, bm4b, bxh, btail, 0ul, u_d4, u_pr, mp4, proj, 4l * d, true)
        enc_add_bias_rows(enc, btail, 0ul, bblob, uint64(t.mm2_b * 4l), u_pr, u_totpr, nout * proj)
    }
    var served = false
    if (ran) {
        unsafe {
            memcpy(addr(s.mout[0]), metal_buffer_contents(btail), uint64(nout * proj * 4l))
        }
        g_tower_encodes++
        g_tower_rows += npos
        g_tower_blocks += t.n_layer
        if (f16w || bf16w) {
            g_tower_f16_encodes++
        }
        served = true
    }
    pool_release(g_tw_pool, bx, bytes_row)
    pool_release(g_tw_pool, bxb, bytes_row)
    pool_release(g_tw_pool, bxb2, bytes_row)
    pool_release(g_tw_pool, braw, bytes_row)
    pool_release(g_tw_pool, bq, bytes_rowp)
    pool_release(g_tw_pool, bk, bytes_rowp)
    pool_release(g_tw_pool, bv, bytes_rowp)
    pool_release(g_tw_pool, bo_p, bytes_rowp)
    pool_release(g_tw_pool, bh, bytes_h)
    pool_release(g_tw_pool, bhu, bytes_h)
    pool_release(g_tw_pool, batt, bytes_att)
    pool_release(g_tw_pool, bstat, bytes_stat)
    pool_release(g_tw_pool, bcos, bytes_tab)
    pool_release(g_tw_pool, bsin, bytes_tab)
    pool_release(g_tw_pool, bm4b, bytes_m4)
    pool_release(g_tw_pool, btail, bytes_pr)
    pool_release(g_tw_pool, bx0, bytes_x0)
    pool_release(g_tw_pool, bsrc, bytes_idx)
    pool_release(g_tw_pool, bwlo, bytes_wlo)
    if (bxh != null) {
        pool_release(g_tw_pool, bxh, bytes_xh)
    }
    pool_release(g_upool, u_pd, 4ul)
    pool_release(g_upool, u_d, 4ul)
    pool_release(g_upool, u_d4, 4ul)
    pool_release(g_upool, u_ff, 4ul)
    pool_release(g_upool, u_pr, 4ul)
    pool_release(g_upool, u_hs, 4ul)
    pool_release(g_upool, u_hsp, 4ul)
    pool_release(g_upool, u_eps, 4ul)
    pool_release(g_upool, u_totd, 4ul)
    pool_release(g_upool, u_totff, 4ul)
    pool_release(g_upool, u_totp, 4ul)
    pool_release(g_upool, u_totc, 4ul)
    pool_release(g_upool, u_tot4, 4ul)
    pool_release(g_upool, u_totpr, 4ul)
    pool_release(g_upool, u_np_rope, 4ul)
    pool_release(g_upool, u_neox, 4ul)
    if (!served) {
        return tw_decline(MetalTowerDecline.gpu_error)
    }
    return true
}

// The whisper conv frontend for one mel chunk: im2col + conv GEMMs + bias + GELU + pos-add,
// one command buffer, s.x readback. BEST-EFFORT inside the tower knob: any decline is a
// silent false (no counter noise — the CPU conv serves and the blocks still ride the GPU).
[hot_path]
def private metal_conv_frontend(t : AudioTower; var s : EncoderState; mel : MelChunk) : bool {   // nolint:STYLE038 — the dispatch chain is the shape
    if (!g_tw_env_tower || t.q8) {
        return false
    }
    let tin = AUDIO_CHUNK_FRAMES
    let tt = t.n_ctx
    let d = t.d_model
    let nm = t.n_mel
    // tiny's 3x80 im2col width stays CPU; init re-arms after any shutdown seat
    if ((3l * nm) % 32l != 0l || d % 64l != 0l || (3l * d) % 32l != 0l || !metal_tower_init()) {
        return false
    }
    let mp1 = (tin + 31l) / 32l * 32l
    let mp2 = (tt + 31l) / 32l * 32l
    let mapped = t.image_map != null
    var bblob = plane_buffer(g_dev, unsafe(reinterpret<void?>(plane_at(t.fblob, 0l))), uint64(length(t.fblob) * 4l), mapped)
    let bytes_mel = uint64(nm * tin * 4l)
    let bytes_x1 = uint64(mp1 * 3l * nm * 4l)
    let bytes_y1 = uint64(mp1 * d * 4l)
    let bytes_x2 = uint64(mp2 * 3l * d * 4l)
    let bytes_xo = uint64(mp2 * d * 4l)
    var bmel = pool_acquire_pinned(g_tw_pool, g_dev, bytes_mel)
    var bx1 = pool_acquire_pinned(g_tw_pool, g_dev, bytes_x1)
    var by1 = pool_acquire_pinned(g_tw_pool, g_dev, bytes_y1)
    var bx2 = pool_acquire_pinned(g_tw_pool, g_dev, bytes_x2)
    var bxo = pool_acquire_pinned(g_tw_pool, g_dev, bytes_xo)
    unsafe {
        memcpy(metal_buffer_contents(bmel), addr(mel.data[0]), bytes_mel)
    }
    var u_nm = uniform_u32(uint(nm))
    var u_nm3 = uniform_u32(uint(3l * nm))
    var u_d = uniform_u32(uint(d))
    var u_3d = uniform_u32(uint(3l * d))
    var u_tin = uniform_u32(uint(tin))
    var u_s1 = uniform_u32(1u)
    var u_s2 = uniform_u32(2u)
    var u_z = uniform_u32(0u)
    var u_totx1 = uniform_u32(uint(tin * 3l * nm))
    var u_totd1 = uniform_u32(uint(tin * d))
    var u_totx2 = uniform_u32(uint(tt * 3l * d))
    var u_totd2 = uniform_u32(uint(tt * d))
    var err = ""
    let ran = with_compute_encoder(g_queue, err) $(enc : MetalComputeEncoder?) {   // nolint:PERF026 — the error-text fetch runs only on the failure leg
        enc_im2col3(enc, bmel, bx1, u_nm, u_tin, u_s1, u_s1, u_totx1, tin * 3l * nm)   // chmajor=1: the mel is [c][t]
        enc_f32_mm(enc, bblob, uint64(t.conv1_w_off * 4l), bx1, by1, 0ul, u_nm3, u_d, mp1, d)
        enc_add_bias_rows(enc, by1, 0ul, bblob, uint64(t.conv1_b_off * 4l), u_d, u_totd1, tin * d)
        if (t.gelu_tanh) {
            enc_gelu_lut(enc, by1, 0ul, u_totd1, tin * d)
        } else {
            enc_gelu_erf(enc, by1, 0ul, u_totd1, tin * d)
        }
        enc_im2col3(enc, by1, bx2, u_d, u_tin, u_s2, u_z, u_totx2, tt * 3l * d)   // chmajor=0: y1 is [t][c]
        enc_f32_mm(enc, bblob, uint64(t.conv2_w_off * 4l), bx2, bxo, 0ul, u_3d, u_d, mp2, d)
        enc_add_bias_rows(enc, bxo, 0ul, bblob, uint64(t.conv2_b_off * 4l), u_d, u_totd2, tt * d)
        if (t.gelu_tanh) {
            enc_gelu_lut(enc, bxo, 0ul, u_totd2, tt * d)
        } else {
            enc_gelu_erf(enc, bxo, 0ul, u_totd2, tt * d)
        }
        enc_add(enc, bxo, 0ul, bblob, uint64(t.pos_off * 4l), u_totd2, tt * d)
    }
    var served = false
    if (ran) {
        unsafe {
            memcpy(addr(s.x[0]), metal_buffer_contents(bxo), uint64(tt * d * 4l))
        }
        g_tower_convs++
        served = true
    }
    pool_release(g_tw_pool, bmel, bytes_mel)
    pool_release(g_tw_pool, bx1, bytes_x1)
    pool_release(g_tw_pool, by1, bytes_y1)
    pool_release(g_tw_pool, bx2, bytes_x2)
    pool_release(g_tw_pool, bxo, bytes_xo)
    pool_release(g_upool, u_nm, 4ul)
    pool_release(g_upool, u_nm3, 4ul)
    pool_release(g_upool, u_d, 4ul)
    pool_release(g_upool, u_3d, 4ul)
    pool_release(g_upool, u_tin, 4ul)
    pool_release(g_upool, u_s1, 4ul)
    pool_release(g_upool, u_s2, 4ul)
    pool_release(g_upool, u_z, 4ul)
    pool_release(g_upool, u_totx1, 4ul)
    pool_release(g_upool, u_totd1, 4ul)
    pool_release(g_upool, u_totx2, 4ul)
    pool_release(g_upool, u_totd2, 4ul)
    return served
}

// The qwen3a conv frontend needs PADDED weights for the tile GEMM (n_ch=480 fails ndim%64,
// conv1's kdim=9 fails %32): a driver-owned buffer minted once per tower — out-channels
// padded to ncp=pad64(n_ch), conv1's K padded to 32, biases padded with zeros.
var private g_tw_q3a_wpad : MetalBuffer?
var private g_tw_q3a_key = 0ul
var private g_tw_q3a_len = 0l

[cold_path]
def private q3a_wpad_attach(t : Qwen3aTower) : bool {
    let base = intptr(plane_at(t.cblob, 0l))
    let blen = length(t.cblob)
    if (g_tw_q3a_wpad != null && base == g_tw_q3a_key && blen == g_tw_q3a_len) {
        return true
    }
    if (g_tw_q3a_wpad != null) {
        metal_release(g_tw_q3a_wpad)
        g_tw_q3a_wpad = null
    }
    let nch = t.n_ch
    let ncp = (nch + 63l) & ~63l
    let k2 = 9l * nch
    let total = ncp * 32l + 2l * ncp * k2 + 3l * ncp
    g_tw_q3a_wpad = metal_new_buffer(g_dev, uint64(total * 4l))
    if (g_tw_q3a_wpad == null) {
        return false
    }
    unsafe {
        var p = reinterpret<float?>(metal_buffer_contents(g_tw_q3a_wpad))
        for (i in range64(total)) {
            p[i] = 0.0
        }
        for (oc in range64(nch)) {   // c1: [ncp][32], 9 real K columns
            for (k in range64(9l)) {
                p[oc * 32l + k] = t.cblob[t.c1_w_off + oc * 9l + k]
            }
        }
        let o2 = ncp * 32l
        let o3 = o2 + ncp * k2
        for (oc in range64(nch)) {   // c2/c3: [ncp][9*nch], dense K
            for (k in range64(k2)) {
                p[o2 + oc * k2 + k] = t.cblob[t.c2_w_off + oc * k2 + k]
                p[o3 + oc * k2 + k] = t.cblob[t.c3_w_off + oc * k2 + k]
            }
        }
        let o4 = o3 + ncp * k2
        for (oc in range64(nch)) {   // padded biases
            p[o4 + oc] = t.cblob[t.c1_b_off + oc]
            p[o4 + ncp + oc] = t.cblob[t.c2_b_off + oc]
            p[o4 + 2l * ncp + oc] = t.cblob[t.c3_b_off + oc]
        }
    }
    g_tw_q3a_key = base
    g_tw_q3a_len = blen
    return true
}

// one conv2d stage: padded im2col + the f32 tile GEMM + padded bias + erf-GELU
def private q3a_stage(enc : MetalComputeEncoder?; bsrc : MetalBuffer?; soff : uint64; bcol, bout : MetalBuffer?; yoff : uint64;
                      u_iw, u_ih, u_cin, u_ow, u_kd, u_ss, u_ti, u_kdim, u_ncp, u_tm : MetalBuffer?;
                      ti, tm, mp, ncp : int64; woff, boff : uint64) {
    enc_im2col2d(enc, bsrc, soff, bcol, u_iw, u_ih, u_cin, u_ow, u_kd, u_ss, u_ti, ti)
    enc_f32_mm(enc, g_tw_q3a_wpad, woff, bcol, bout, yoff, u_kdim, u_ncp, mp, ncp)
    enc_add_bias_rows(enc, bout, yoff, g_tw_q3a_wpad, boff, u_ncp, u_tm, tm)
    enc_gelu_erf(enc, bout, yoff, u_tm, tm)
}

//! The qwen3a front for one WINDOW: chunk convs + feature shuffle + conv_out + bias/positions
//! in one command buffer, s.x readback. The q8 cblob lane declines (the cout trio lives in
//! qcblob).
[hot_path, arch(at="../ARCHITECTURE_GPU_TOWER.md#tower-encode-chains")]
def private metal_q3a_front(t : Qwen3aTower; win : MelChunk; n_chunks : int64; var s : EncoderState) : bool {   // nolint:STYLE038 — the dispatch chain is the shape
    let nm = t.core.n_mel
    let nch = t.n_ch
    let ncp = (nch + 63l) & ~63l
    let k2 = 9l * nch
    let d = t.core.d_model
    let featw = 16l * nch
    let npos = n_chunks * QWEN3A_TOK_PER_CHUNK
    //! the shape gates run first so a forever-declining tower never mints the padded slab
    if (k2 % 32l != 0l || featw % 32l != 0l || d % 64l != 0l || t.cq8
            || !g_tw_env_tower || !metal_tower_init() || !pf_q3a_front_ready() || !q3a_wpad_attach(t)) {
        return false
    }
    let xin_n = QWEN3A_CHUNK * nm
    let y3_rows = 16l * QWEN3A_TOK_PER_CHUNK   // 208 per chunk
    let mp3 = (y3_rows + 31l) / 32l * 32l      // the stage-3 GEMM pads to 224 rows
    let mpn = (npos + 31l) / 32l * 32l
    let o2 = ncp * 32l
    let o3 = o2 + ncp * k2
    let o4 = o3 + ncp * k2
    let mapped = t.image_map != null
    var bcb = plane_buffer(g_dev, unsafe(reinterpret<void?>(plane_at(t.cblob, 0l))), uint64(length(t.cblob) * 4l), mapped)
    let bytes_xin = uint64(n_chunks * xin_n * 4l)
    let bytes_col = uint64(max(3200l * 32l, 800l * k2) * 4l)
    let bytes_y1 = uint64(3200l * ncp * 4l)
    let bytes_y2 = uint64(800l * ncp * 4l)
    let bytes_y3 = uint64(n_chunks * mp3 * ncp * 4l)
    let bytes_feat = uint64(mpn * featw * 4l)
    let bytes_x = uint64(mpn * d * 4l)
    var bxin = pool_acquire_pinned(g_tw_pool, g_dev, bytes_xin)
    var bcol = pool_acquire_pinned(g_tw_pool, g_dev, bytes_col)
    var by1 = pool_acquire_pinned(g_tw_pool, g_dev, bytes_y1)
    var by2 = pool_acquire_pinned(g_tw_pool, g_dev, bytes_y2)
    var by3 = pool_acquire_pinned(g_tw_pool, g_dev, bytes_y3)
    var bfeat = pool_acquire_pinned(g_tw_pool, g_dev, bytes_feat)
    var bx = pool_acquire_pinned(g_tw_pool, g_dev, bytes_x)
    unsafe {   // the per-chunk [ih][iw] extract from the window's mel-major plane, straight into the mapped slab
        var p = reinterpret<float?>(metal_buffer_contents(bxin))
        for (ci in range64(n_chunks)) {
            for (ih in range64(nm)) {
                for (iw in range64(QWEN3A_CHUNK)) {
                    p[ci * xin_n + ih * QWEN3A_CHUNK + iw] = win.data[ih * win.n_len + ci * QWEN3A_CHUNK + iw]
                }
            }
        }
    }
    var u_iw100 = uniform_u32(uint(QWEN3A_CHUNK))
    var u_ih128 = uniform_u32(uint(nm))
    var u_c1 = uniform_u32(1u)
    var u_ow50 = uniform_u32(50u)
    var u_iw50 = uniform_u32(50u)
    var u_ih64 = uniform_u32(64u)
    var u_ow25 = uniform_u32(25u)
    var u_iw25 = uniform_u32(25u)
    var u_ih32 = uniform_u32(32u)
    var u_ow13 = uniform_u32(uint(QWEN3A_TOK_PER_CHUNK))
    var u_nch = uniform_u32(uint(nch))
    var u_ncp = uniform_u32(uint(ncp))
    var u_kd32 = uniform_u32(32u)
    var u_k2 = uniform_u32(uint(k2))
    var u_ti1 = uniform_u32(uint(3200l * 32l))
    var u_ti2 = uniform_u32(uint(800l * k2))
    var u_ti3 = uniform_u32(uint(y3_rows * k2))
    var u_tm1 = uniform_u32(uint(3200l * ncp))
    var u_tm2 = uniform_u32(uint(800l * ncp))
    var u_tm3 = uniform_u32(uint(y3_rows * ncp))
    var u_featw = uniform_u32(uint(featw))
    var u_mp3ncp = uniform_u32(uint(mp3 * ncp))
    var u_totf = uniform_u32(uint(npos * featw))
    var u_d = uniform_u32(uint(d))
    var u_totx = uniform_u32(uint(npos * d))
    var err = ""
    let ran = with_compute_encoder(g_queue, err) $(enc : MetalComputeEncoder?) {   // nolint:PERF026 — the error-text fetch runs only on the failure leg
        for (ci in range64(n_chunks)) {
            q3a_stage(enc, bxin, uint64(ci * xin_n * 4l), bcol, by1, 0ul,
                u_iw100, u_ih128, u_c1, u_ow50, u_kd32, u_c1, u_ti1, u_kd32, u_ncp, u_tm1,
                3200l * 32l, 3200l * ncp, 3200l, ncp, 0ul, uint64(o4 * 4l))
            q3a_stage(enc, by1, 0ul, bcol, by2, 0ul,
                u_iw50, u_ih64, u_nch, u_ow25, u_k2, u_ncp, u_ti2, u_k2, u_ncp, u_tm2,
                800l * k2, 800l * ncp, 800l, ncp, uint64(o2 * 4l), uint64((o4 + ncp) * 4l))
            q3a_stage(enc, by2, 0ul, bcol, by3, uint64(ci * mp3 * ncp * 4l),
                u_iw25, u_ih32, u_nch, u_ow13, u_k2, u_ncp, u_ti3, u_k2, u_ncp, u_tm3,
                y3_rows * k2, y3_rows * ncp, mp3, ncp, uint64(o3 * 4l), uint64((o4 + 2l * ncp) * 4l))
        }
        enc_q3a_shuffle(enc, by3, bfeat, u_featw, u_nch, u_ncp, u_mp3ncp, u_totf, npos * featw)
        enc_f32_mm(enc, bcb, uint64(t.cout_w_off * 4l), bfeat, bx, 0ul, u_featw, u_d, mpn, d)
        enc_q3a_finish(enc, bx, bcb, uint64(t.cout_b_off * 4l), bcb, uint64(t.pos_off * 4l), u_d, u_totx, npos * d)
    }
    var served = false
    if (ran) {
        unsafe {
            memcpy(addr(s.x[0]), metal_buffer_contents(bx), uint64(npos * d * 4l))
        }
        g_tower_convs += n_chunks
        served = true
    }
    pool_release(g_tw_pool, bxin, bytes_xin)
    pool_release(g_tw_pool, bcol, bytes_col)
    pool_release(g_tw_pool, by1, bytes_y1)
    pool_release(g_tw_pool, by2, bytes_y2)
    pool_release(g_tw_pool, by3, bytes_y3)
    pool_release(g_tw_pool, bfeat, bytes_feat)
    pool_release(g_tw_pool, bx, bytes_x)
    pool_release(g_upool, u_iw100, 4ul)
    pool_release(g_upool, u_ih128, 4ul)
    pool_release(g_upool, u_c1, 4ul)
    pool_release(g_upool, u_ow50, 4ul)
    pool_release(g_upool, u_iw50, 4ul)
    pool_release(g_upool, u_ih64, 4ul)
    pool_release(g_upool, u_ow25, 4ul)
    pool_release(g_upool, u_iw25, 4ul)
    pool_release(g_upool, u_ih32, 4ul)
    pool_release(g_upool, u_ow13, 4ul)
    pool_release(g_upool, u_nch, 4ul)
    pool_release(g_upool, u_ncp, 4ul)
    pool_release(g_upool, u_kd32, 4ul)
    pool_release(g_upool, u_k2, 4ul)
    pool_release(g_upool, u_ti1, 4ul)
    pool_release(g_upool, u_ti2, 4ul)
    pool_release(g_upool, u_ti3, 4ul)
    pool_release(g_upool, u_tm1, 4ul)
    pool_release(g_upool, u_tm2, 4ul)
    pool_release(g_upool, u_tm3, 4ul)
    pool_release(g_upool, u_featw, 4ul)
    pool_release(g_upool, u_mp3ncp, 4ul)
    pool_release(g_upool, u_totf, 4ul)
    pool_release(g_upool, u_d, 4ul)
    pool_release(g_upool, u_totx, 4ul)
    return served
}

//! The qwen3a mel: DFT (transposed-twiddle GEMM) -> power -> mel-filter GEMM -> the mel-major
//! log10 store, one command buffer off the CPU-windowed xw frames; m.data readback. The global
//! max−8 clamp stays with the caller.
[hot_path, arch(at="../ARCHITECTURE_GPU_TOWER.md#tower-encode-chains")]
def private metal_q3a_mel(xw : array<float>; n_len, n_fft, n_bins, n_mel : int64;   // nolint:STYLE038 — the dispatch chain is the shape
                          twiddles, filters : array<float>; var m : WhisperMel) : bool {
    if (n_mel % 64l != 0l || n_len < 1l || !g_tw_env_tower || !metal_tower_init() || !pf_q3a_mel_ready()) {
        return false
    }
    let mp = (n_len + 31l) / 32l * 32l
    let kfp = (n_fft + 31l) / 32l * 32l
    let dft_out_pad = (2l * n_bins + 63l) / 64l * 64l
    let ldpow = (n_bins + 31l) / 32l * 32l
    let bytes_xw = uint64(mp * kfp * 4l)
    let bytes_tw = uint64(dft_out_pad * kfp * 4l)
    let bytes_re = uint64(mp * dft_out_pad * 4l)
    let bytes_pow = uint64(mp * ldpow * 4l)
    let bytes_fil = uint64(n_mel * ldpow * 4l)
    let bytes_raw = uint64(mp * n_mel * 4l)
    let bytes_out = uint64(n_mel * n_len * 4l)
    var bxw = pool_acquire_pinned(g_tw_pool, g_dev, bytes_xw)
    var btw = pool_acquire_pinned(g_tw_pool, g_dev, bytes_tw)
    var bre = pool_acquire_pinned(g_tw_pool, g_dev, bytes_re)
    var bpow = pool_acquire_pinned(g_tw_pool, g_dev, bytes_pow)
    var bfil = pool_acquire_pinned(g_tw_pool, g_dev, bytes_fil)
    var braw = pool_acquire_pinned(g_tw_pool, g_dev, bytes_raw)
    var bout = pool_acquire_pinned(g_tw_pool, g_dev, bytes_out)
    unsafe {
        //! xw rows at the padded k stride, zero tails; pad rows zeroed
        var xp = reinterpret<float?>(metal_buffer_contents(bxw))
        let sxp = addr<float const?>(xw[0])
        for (r in range64(n_len)) {
            memcpy(xp + r * kfp, sxp + r * n_fft, n_fft * 4l)
            for (k in range64(n_fft, kfp)) {
                xp[r * kfp + k] = 0.0
            }
        }
        for (i in range64(n_len * kfp, mp * kfp)) {
            xp[i] = 0.0
        }
        //! the transposed twiddle panel [dft_out_pad x kfp], zero-padded both ways
        var tp = reinterpret<float?>(metal_buffer_contents(btw))
        for (i in range64(dft_out_pad * kfp)) {
            tp[i] = 0.0
        }
        let stp = addr<float const?>(twiddles[0])
        for (k in range64(n_fft)) {
            for (n in range64(2l * n_bins)) {
                tp[n * kfp + k] = stp[k * 2l * n_bins + n]
            }
        }
        //! the mel filterbank [n_mel x ldpow], k-padded
        var fp = reinterpret<float?>(metal_buffer_contents(bfil))
        let sfp = addr<float const?>(filters[0])
        for (j in range64(n_mel)) {
            memcpy(fp + j * ldpow, sfp + j * n_bins, n_bins * 4l)
            for (k in range64(n_bins, ldpow)) {
                fp[j * ldpow + k] = 0.0
            }
        }
    }
    var u_kfp = uniform_u32(uint(kfp))
    var u_dft_out_pad = uniform_u32(uint(dft_out_pad))
    var u_nbins = uniform_u32(uint(n_bins))
    var u_ldpow = uniform_u32(uint(ldpow))
    var u_totpow = uniform_u32(uint(mp * ldpow))
    var u_nmel = uniform_u32(uint(n_mel))
    var u_nlen = uniform_u32(uint(n_len))
    var u_floor = uniform_f32(5.960464477539063e-08)   // the whisper preprocessor's 2^-24 floor
    var u_scale = uniform_f32(0.43429448190325176)     // log10 e — the CPU stores log10
    var u_totm = uniform_u32(uint(n_len * n_mel))
    var err = ""
    let ran = with_compute_encoder(g_queue, err) $(enc : MetalComputeEncoder?) {   // nolint:PERF026 — the error-text fetch runs only on the failure leg
        enc_f32_mm(enc, btw, 0ul, bxw, bre, 0ul, u_kfp, u_dft_out_pad, mp, dft_out_pad)
        enc_q3a_pow(enc, bre, bpow, u_nbins, u_dft_out_pad, u_ldpow, u_totpow, mp * ldpow)
        enc_f32_mm(enc, bfil, 0ul, bpow, braw, 0ul, u_ldpow, u_nmel, mp, n_mel)
        enc_q3a_logmel_t(enc, braw, bout, u_nmel, u_nlen, u_floor, u_scale, u_totm, n_len * n_mel)
    }
    var served = false
    if (ran) {
        unsafe {
            memcpy(addr(m.data[0]), metal_buffer_contents(bout), uint64(n_mel * n_len * 4l))
        }
        served = true
    }
    pool_release(g_tw_pool, bxw, bytes_xw)
    pool_release(g_tw_pool, btw, bytes_tw)
    pool_release(g_tw_pool, bre, bytes_re)
    pool_release(g_tw_pool, bpow, bytes_pow)
    pool_release(g_tw_pool, bfil, bytes_fil)
    pool_release(g_tw_pool, braw, bytes_raw)
    pool_release(g_tw_pool, bout, bytes_out)
    pool_release(g_upool, u_kfp, 4ul)
    pool_release(g_upool, u_dft_out_pad, 4ul)
    pool_release(g_upool, u_nbins, 4ul)
    pool_release(g_upool, u_ldpow, 4ul)
    pool_release(g_upool, u_totpow, 4ul)
    pool_release(g_upool, u_nmel, 4ul)
    pool_release(g_upool, u_nlen, 4ul)
    pool_release(g_upool, u_floor, 4ul)
    pool_release(g_upool, u_scale, 4ul)
    pool_release(g_upool, u_totm, 4ul)
    return served
}

let private ST2_IM2COL_CHUNK_MAX_BYTES = 64l * 1024l * 1024l   //!< the im2col chunk's byte ceiling
let private ST2_CHUNK_ROWS = 4096l
let private ST2_POST_WIDTH = 64l   //!< conv_post's row stride on the driver: its cout padded to the GEMM lattice
let private ST2_MAX_STAGES = 8l    //!< bounds the distinct uniform values an encode binds (six a stage, eight more)
let private ST2_MAX_UNIFORMS = 96
let private ST2_MAX_BUFS = 32
let private ST2_JOIN = 0.70710678118654752   //!< the AdaIN residual blocks' join: (out + shortcut) / sqrt 2
let private ST2_LSTM_MAX_HIDDEN = 256l       //!< the recurrence kernel's lanes: four a unit, 1024 a threadgroup

struct private St2ConvSlot {
    woff : uint64
    boff : uint64
    cout_p : int64
    kdim_p : int64
    gathers : bool       //!< the GEMM gathers its own taps (cin % 8); else the im2col chunks feed it
    u_kdim : MetalBuffer?
    u_cout : MetalBuffer?
}

struct private St2NormSlot {
    gwoff : uint64
    gboff : uint64
    aoff : uint64
}

struct private St2BlockSlots {
    c1 : array<St2ConvSlot>
    c2 : array<St2ConvSlot>
    n1 : array<St2NormSlot>
    n2 : array<St2NormSlot>
}

struct private St2ResSlots {
    n1 : St2NormSlot
    n2 : St2NormSlot
    c1 : St2ConvSlot
    c2 : St2ConvSlot
    c1x1 : St2ConvSlot
    pool_w : uint64
    pool_b : uint64
}

struct private St2LstmSlot {
    fx : St2ConvSlot
    bx : St2ConvSlot
    fhh : uint64
    bhh : uint64
}

struct private St2SlabWriter {
    @exact_size host : array<float>   //! sized once from the measure pass; the write pass only bumps `n`
    n : int64
    measure : bool
}

//! `count` floats at the next 16-byte boundary (every item is a float4 read for some kernel).
def private sw_alloc(var w : St2SlabWriter; count : int64) : int64 {
    let at = (w.n + 3l) / 4l * 4l
    w.n = at + count
    return at
}

def private sw_put(var w : St2SlabWriter; i : int64; v : float) {
    if (!w.measure) {
        w.host[i] = v
    }
}

def private st2_ceil(n, m : int64) : int64 => (n + m - 1l) / m * m

def private st2_conv_key(c : TtsConv1d) : uint64 {
    unsafe {
        if (c.q8) {
            return intptr(addr(c.wq[0]))
        }
        if (!empty(c.w)) {
            return intptr(addr(c.w[0]))
        }
        if (!empty(c.wt)) {
            return intptr(addr(c.wt[0]))
        }
        return c.transposed ? intptr(addr(c.wtr[0])) : intptr(addr(c.wk[0]))
    }
}

def private st2_conv_ok(c : TtsConv1d) : bool {
    return (c.groups == 1l && c.k >= 1l && c.cin >= 1l && c.cout >= 1l && c.stride >= 1l && c.dilation >= 1l
        && (c.q8 ? !empty(c.wq) : (!empty(c.w) || !empty(c.wt) || !empty(c.transposed ? c.wtr : c.wk))))
}

def private sw_put_q8(var w : St2SlabWriter; q : array<int8>; qs : array<float>; rows, cols, w0, stride : int64) {
    return if (w.measure)
    var wq : array<uint8>
    var ws : array<uint8>
    wq |> reserve_resize(rows * cols)
    ws |> reserve_resize(rows * cols / 32l * 2l)
    let mr = g_active_needs_repack ? active_q8_layout_mr() : 1l
    unsafe {
        q8_gather_rows(addr<int8 const?>(q[0]), null, addr<float const?>(qs[0]), cols, rows, rows,
            mr, g_active_needs_repack ? active_q8_kgroup() : 1l, g_active_needs_repack ? active_q8_wbias() : 0l, wq, ws)
        let sp = addr<uint16 const?>(ws[0])
        for (r in range64(rows)) {
            for (j in range64(cols)) {
                w.host[w0 + r * stride + j] = float(int8(wq[r * cols + j])) * f16_to_f32(uint(sp[(r * cols + j) / 32l]))
            }
        }
    }
}

def private st2_slot_uniforms(w : St2SlabWriter; var s : St2ConvSlot) {
    if (!w.measure) {
        s.u_kdim = uniform_u32(uint(s.kdim_p))
        s.u_cout = uniform_u32(uint(s.cout_p))
    }
}

def private st2_slab_conv(var w : St2SlabWriter; c : TtsConv1d) : St2ConvSlot {
    let kc = c.k * c.cin
    var s = St2ConvSlot(cout_p = st2_ceil(c.cout, 64l), kdim_p = st2_ceil(kc, 32l), gathers = c.cin % 8l == 0l)
    let w0 = sw_alloc(w, s.cout_p * s.kdim_p)
    s.woff = uint64(w0 * 4l)
    if (c.q8) {
        sw_put_q8(w, c.wq, c.wqs, c.cout, kc, w0, s.kdim_p)
    } elif (!w.measure) {
        for (co in range64(c.cout)) {
            for (ci in range64(c.cin)) {
                for (tap in range64(c.k)) {
                    var v = 0.0
                    if (c.transposed) {
                        v = !empty(c.wtr) ? c.wtr[(ci * c.k + tap) * c.cout + co] : c.wt[(co * c.k + tap) * c.cin + ci]
                    } else {
                        v = !empty(c.wk) ? c.wk[(tap * c.cin + ci) * c.cout_s + co] : c.w[(co * c.cin + ci) * c.k + tap]
                    }
                    w.host[w0 + co * s.kdim_p + tap * c.cin + ci] = v
                }
            }
        }
    }
    s.boff = st2_slab_raw(w, c.b, s.cout_p)
    st2_slot_uniforms(w, s)
    return s
}

def private st2_slab_linear(var w : St2SlabWriter; l : TtsLinear) : St2ConvSlot {
    var s = St2ConvSlot(cout_p = st2_ceil(l.nout, 64l), kdim_p = st2_ceil(l.nin, 32l), gathers = false)
    let w0 = sw_alloc(w, s.cout_p * s.kdim_p)
    s.woff = uint64(w0 * 4l)
    if (l.q8) {
        sw_put_q8(w, l.wq, l.wqs, l.nout, l.nin, w0, s.kdim_p)
    } elif (!w.measure) {
        for (o in range64(l.nout)) {
            for (i in range64(l.nin)) {
                w.host[w0 + o * s.kdim_p + i] = !empty(l.w) ? l.w[o * l.nin + i] : l.wt[i * l.nout + o]
            }
        }
    }
    s.boff = st2_slab_raw(w, l.b, s.cout_p)
    st2_slot_uniforms(w, s)
    return s
}

//! n floats of src into the slab, zeros past its length (a window the model does not carry); the byte offset.
def private st2_slab_raw(var w : St2SlabWriter; src : array<float>; n : int64) : uint64 {
    let at = sw_alloc(w, n)
    for (i in range64(min(n, long_length(src)))) {
        sw_put(w, at + i, src[i])
    }
    return uint64(at * 4l)
}

def private st2_slab_lstm_dir(var w : St2SlabWriter; l : TtsLstm; var xs : St2ConvSlot; var hh : uint64&) {
    var lin = TtsLinear(nin = l.nin, nout = 4l * l.hidden)
    lin.w := l.w_ih
    lin.b := l.b
    xs = st2_slab_linear(w, lin)
    hh = st2_slab_raw(w, l.w_hh, 4l * l.hidden * l.hidden)
}

def private st2_slab_lstm(var w : St2SlabWriter; l : TtsBiLstm) : St2LstmSlot {
    var s : St2LstmSlot
    st2_slab_lstm_dir(w, l.fwd, s.fx, s.fhh)
    st2_slab_lstm_dir(w, l.bwd, s.bx, s.bhh)
    return s
}

//! A norm's scale and shift rows (ones and zeros when it carries none).
def private st2_slab_norm_rows(var w : St2SlabWriter; n : TtsNorm; c : int64) : St2NormSlot {
    let g0 = sw_alloc(w, 2l * c)
    for (i in range64(c)) {
        sw_put(w, g0 + i, empty(n.w) ? 1.0 : n.w[i])
        sw_put(w, g0 + c + i, empty(n.b) ? 0.0 : n.b[i])
    }
    return St2NormSlot(gwoff = uint64(g0 * 4l), gboff = uint64((g0 + c) * 4l))
}

def private st2_slab_norm(var w : St2SlabWriter; a : TtsAdaIn; alpha : TtsWeight; c : int64) : St2NormSlot {
    var s = st2_slab_norm_rows(w, a.norm, c)
    s.aoff = st2_slab_raw(w, alpha.a, c)
    return s
}

def private st2_slab_block(var w : St2SlabWriter; b : St2GenResBlock) : St2BlockSlots {
    var s : St2BlockSlots
    let n = length(b.convs1)
    s.c1 |> reserve(n)
    s.c2 |> reserve(n)
    s.n1 |> reserve(n)
    s.n2 |> reserve(n)
    for (j in range(n)) {
        s.c1 |> push(st2_slab_conv(w, b.convs1[j]))
        s.c2 |> push(st2_slab_conv(w, b.convs2[j]))
        s.n1 |> push(st2_slab_norm(w, b.adain1[j], b.alpha1[j], b.c))
        s.n2 |> push(st2_slab_norm(w, b.adain2[j], b.alpha2[j], b.c))
    }
    return <- s
}

def private st2_slab_res(var w : St2SlabWriter; b : St2AdainResBlk) : St2ResSlots {
    var s = St2ResSlots(n1 = st2_slab_norm_rows(w, b.norm1.norm, b.dim_in_s), n2 = st2_slab_norm_rows(w, b.norm2.norm, b.dim_out),
        c1 = st2_slab_conv(w, b.conv1), c2 = st2_slab_conv(w, b.conv2))
    if (b.learned_sc) {
        s.c1x1 = st2_slab_conv(w, b.conv1x1)
    }
    if (b.upsample) {
        s.pool_w = st2_slab_raw(w, b.pool.w, b.pool.cin * b.pool.k)
        s.pool_b = st2_slab_raw(w, b.pool.b, b.pool.cin)
    }
    return s
}

def private st2_slot_release(var s : St2ConvSlot) {
    if (s.u_kdim != null) {
        pool_release(g_upool, s.u_kdim, 4ul)
        pool_release(g_upool, s.u_cout, 4ul)
        s.u_kdim = null
        s.u_cout = null
    }
}

def private st2_res_release(var s : St2ResSlots) {
    st2_slot_release(s.c1)
    st2_slot_release(s.c2)
    st2_slot_release(s.c1x1)
}

def private st2_lstm_release(var s : St2LstmSlot) {
    st2_slot_release(s.fx)
    st2_slot_release(s.bx)
}

struct private St2SlabBase {
    buf : MetalBuffer?
    key : uint64
}

struct private St2DecSlab : St2SlabBase {
    ups : array<St2ConvSlot>
    noise : array<St2ConvSlot>
    blocks : array<St2BlockSlots>
    post : St2ConvSlot
    f0c : St2ConvSlot
    nc : St2ConvSlot
    asr_res : St2ConvSlot
    encode : St2ResSlots
    decode : array<St2ResSlots>
    src_w : uint64
    src_b : uint64
    stft_re : uint64
    stft_im : uint64
    istft_re : uint64
    istft_im : uint64
    window : uint64
}

struct private St2PredSlab : St2SlabBase {
    bert_enc : St2ConvSlot
    de_lstm : array<St2LstmSlot>
    dur_lstm : St2LstmSlot
    dur_proj : St2ConvSlot
    sh_lstm : St2LstmSlot
    f0b : array<St2ResSlots>
    nb : array<St2ResSlots>
    f0_proj : St2ConvSlot
    n_proj : St2ConvSlot
}

struct private St2AlbertSlab : St2SlabBase {
    word_emb : uint64
    pos_emb : uint64
    type_emb : uint64
    emb_ln : St2NormSlot
    map_in : St2ConvSlot
    q : St2ConvSlot
    k : St2ConvSlot
    v : St2ConvSlot
    dense : St2ConvSlot
    attn_ln : St2NormSlot
    ffn : St2ConvSlot
    ffn_out : St2ConvSlot
    full_ln : St2NormSlot
}

struct private St2TextSlab : St2SlabBase {
    emb : uint64
    cnn : array<St2ConvSlot>
    ln : array<St2NormSlot>
    lstm : St2LstmSlot
}

var private g_tw_st2_dec : St2DecSlab
var private g_tw_st2_pred : St2PredSlab
var private g_tw_st2_albert : St2AlbertSlab
var private g_tw_st2_text : St2TextSlab
var private @scratch @exact_size g_tw_st2_style_fc : array<float>   //!< one style fc's output

//! The q8 lane's repack layout, folded into every slab key: the same quants under another layout dequantize differently.
def private st2_lane_key() : uint64 {
    return 1ul if (!g_active_needs_repack)
    return (uint64(active_q8_layout_mr()) * 31ul + uint64(active_q8_kgroup())) * 31ul + uint64(active_q8_wbias())
}

def private st2_key_fold(var key : uint64&; c : TtsConv1d) {
    key = key * 31ul + st2_conv_key(c)
}

def private st2_key_block(var key : uint64&; b : St2GenResBlock) {
    for (c in b.convs1) {
        st2_key_fold(key, c)
    }
    for (c in b.convs2) {
        st2_key_fold(key, c)
    }
}

def private st2_key_res(var key : uint64&; b : St2AdainResBlk) {
    st2_key_fold(key, b.conv1)
    st2_key_fold(key, b.conv2)
}

def private st2_lin_key(l : TtsLinear) : uint64 {
    unsafe {
        if (l.q8) {
            return intptr(addr(l.wq[0]))
        }
        return !empty(l.w) ? intptr(addr(l.w[0])) : intptr(addr(l.wt[0]))
    }
}

def private st2_dec_key(d : St2Decoder) : uint64 {
    var key = 7ul + st2_lane_key()
    for (c in d.ups) {
        st2_key_fold(key, c)
    }
    for (c in d.noise_convs) {
        st2_key_fold(key, c)
    }
    for (b in d.noise_res) {
        st2_key_block(key, b)
    }
    for (b in d.resblocks) {
        st2_key_block(key, b)
    }
    st2_key_fold(key, d.conv_post)
    st2_key_fold(key, d.asr_res)
    st2_key_res(key, d.encode)
    for (b in d.decode) {
        st2_key_res(key, b)
    }
    return key
}

def private st2_arr_key(a : array<float>) : uint64 {
    return 0ul if (empty(a))
    unsafe {
        return intptr(addr(a[0]))
    }
}

def private st2_pred_key(p : St2Predictor) : uint64 {
    var key = 11ul + st2_lane_key() * 31ul + st2_lin_key(p.bert_encoder)
    for (l in p.lstms) {
        key = key * 31ul + st2_arr_key(l.fwd.w_hh)
    }
    key = key * 31ul + st2_arr_key(p.lstm.fwd.w_hh)
    key = key * 31ul + st2_arr_key(p.shared_lstm.fwd.w_hh)
    for (b in p.f0) {
        st2_key_res(key, b)
    }
    for (b in p.n) {
        st2_key_res(key, b)
    }
    return key
}

def private st2_albert_key(a : St2Albert) : uint64 {
    var key = 13ul + st2_lane_key() * 31ul + st2_lin_key(a.map_in)
    key = key * 31ul + st2_lin_key(a.q)
    key = key * 31ul + st2_lin_key(a.ffn)
    return key * 31ul + st2_arr_key(a.word_emb.a)
}

def private st2_text_key(te : St2TextEncoder) : uint64 {
    var key = 17ul + st2_lane_key() * 31ul + st2_arr_key(te.emb.a)
    for (c in te.cnn) {
        st2_key_fold(key, c)
    }
    return key * 31ul + st2_arr_key(te.lstm.fwd.w_hh)
}

//! Untracked: the slab is GPU-read-only and written once here, as the model weights are.
[cold_path]
def private st2_slab_upload(var base : St2SlabBase; var w : St2SlabWriter; key : uint64) : bool {
    let bytes = uint64(long_length(w.host) * 4l)
    base.buf = metal_new_buffer_untracked(g_dev, bytes)
    if (base.buf == null) {
        return false
    }
    unsafe {
        memcpy(metal_buffer_contents(base.buf), addr(w.host[0]), bytes)
    }
    base.key = key
    return true
}

def private st2_base_drop(var base : St2SlabBase) {
    if (base.buf != null) {
        metal_release(base.buf)
        base.buf = null
    }
    base.key = 0ul
}

def private st2_dec_write(var w : St2SlabWriter; d : St2Decoder; var s : St2DecSlab) {
    s.ups |> reserve(length(d.ups))
    s.noise |> reserve(length(d.noise_convs))
    s.blocks |> reserve(length(d.noise_res) + length(d.resblocks))
    s.decode |> reserve(length(d.decode))
    for (c in d.ups) {
        s.ups |> push(st2_slab_conv(w, c))
    }
    for (c in d.noise_convs) {
        s.noise |> push(st2_slab_conv(w, c))
    }
    for (b in d.noise_res) {
        s.blocks |> emplace(st2_slab_block(w, b))
    }
    for (b in d.resblocks) {
        s.blocks |> emplace(st2_slab_block(w, b))
    }
    s.post = st2_slab_conv(w, d.conv_post)
    s.f0c = st2_slab_conv(w, d.f0_conv)
    s.nc = st2_slab_conv(w, d.n_conv)
    s.asr_res = st2_slab_conv(w, d.asr_res)
    s.encode = st2_slab_res(w, d.encode)
    for (b in d.decode) {
        s.decode |> push(st2_slab_res(w, b))
    }
    s.src_w = st2_slab_raw(w, d.source_linear.w, d.source_linear.nin)
    s.src_b = st2_slab_raw(w, d.source_linear.b, 1l)
    s.stft_re = st2_slab_raw(w, d.stft_fwd_re.w, d.stft_fwd_re.cout * d.stft_fwd_re.k)
    s.stft_im = st2_slab_raw(w, d.stft_fwd_im.w, d.stft_fwd_im.cout * d.stft_fwd_im.k)
    s.istft_re = st2_slab_raw(w, d.stft_bwd_re.wt, d.stft_bwd_re.cin * d.stft_bwd_re.k)
    s.istft_im = st2_slab_raw(w, d.stft_bwd_im.wt, d.stft_bwd_im.cin * d.stft_bwd_im.k)
    s.window = st2_slab_raw(w, d.window.a, d.stft_bwd_re.k)
}

def private st2_dec_drop {
    var s & = g_tw_st2_dec
    st2_base_drop(s)
    for (c in s.ups) {
        st2_slot_release(c)
    }
    for (c in s.noise) {
        st2_slot_release(c)
    }
    for (b in s.blocks) {
        for (c in b.c1) {
            st2_slot_release(c)
        }
        for (c in b.c2) {
            st2_slot_release(c)
        }
    }
    st2_slot_release(s.post)
    st2_slot_release(s.f0c)
    st2_slot_release(s.nc)
    st2_slot_release(s.asr_res)
    st2_res_release(s.encode)
    for (b in s.decode) {
        st2_res_release(b)
    }
    unsafe {
        delete s.ups
        delete s.noise
        delete s.blocks
        delete s.decode
    }
}

def private st2_lin_ok(l : TtsLinear) : bool {
    return l.nin >= 1l && l.nout >= 1l && (l.q8 ? !empty(l.wq) : (!empty(l.w) || !empty(l.wt)))
}

//! A linear the dense GEMM stamps serve: `nin` a multiple of 32, since the stamps read X's K columns to the padded width and the row buffers carry no zeroed pad.
def private st2_gemm_lin_ok(l : TtsLinear) : bool => st2_lin_ok(l) && l.nin % 32l == 0l

def private st2_res_ok(b : St2AdainResBlk) : bool {
    return (st2_conv_ok(b.conv1) && st2_conv_ok(b.conv2) && (!b.learned_sc || st2_conv_ok(b.conv1x1))
        && (!b.upsample || (b.pool.k >= 1l && long_length(b.pool.w) >= b.pool.cin * b.pool.k)))
}

//! Every conv and linear a part's slab writes, checked before the writer reads them: a seat's own
//! shape gate covers its stage, the slab covers the whole part.
def private st2_dec_ok(d : St2Decoder) : bool {
    for (c in d.ups) {
        return false if (!st2_conv_ok(c))
    }
    for (c in d.noise_convs) {
        return false if (!st2_conv_ok(c))
    }
    for (b in d.decode) {
        return false if (!st2_res_ok(b))
    }
    return (st2_conv_ok(d.conv_post) && st2_conv_ok(d.f0_conv) && st2_conv_ok(d.n_conv) && st2_conv_ok(d.asr_res)
        && st2_res_ok(d.encode) && st2_lin_ok(d.source_linear))
}

def private st2_pred_ok(p : St2Predictor) : bool {
    for (l in p.lstms) {
        return false if (!st2_lstm_ok(l, l.fwd.nin))
    }
    for (b in p.f0) {
        return false if (!st2_res_ok(b))
    }
    for (b in p.n) {
        return false if (!st2_res_ok(b))
    }
    return (st2_gemm_lin_ok(p.bert_encoder) && st2_lstm_ok(p.lstm, p.lstm.fwd.nin) && st2_gemm_lin_ok(p.duration_proj)
        && st2_lstm_ok(p.shared_lstm, p.shared_lstm.fwd.nin) && st2_conv_ok(p.f0_proj) && st2_conv_ok(p.n_proj))
}

def private st2_albert_ok(a : St2Albert) : bool {
    return (st2_gemm_lin_ok(a.map_in) && st2_gemm_lin_ok(a.q) && st2_gemm_lin_ok(a.k) && st2_gemm_lin_ok(a.v) && st2_gemm_lin_ok(a.dense)
        && st2_gemm_lin_ok(a.ffn) && st2_gemm_lin_ok(a.ffn_out) && a.q.nin == a.hidden && a.k.nin == a.hidden && a.v.nin == a.hidden && a.dense.nin == a.hidden)
}

def private st2_text_ok(te : St2TextEncoder) : bool {
    for (c in te.cnn) {
        return false if (!st2_conv_ok(c))
    }
    return st2_lstm_ok(te.lstm, te.lstm.fwd.nin)
}

[cold_path]
def private st2_dec_attach(d : St2Decoder) : bool {
    let key = st2_dec_key(d)
    if (g_tw_st2_dec.buf != null && g_tw_st2_dec.key == key) {
        return true
    }
    st2_dec_drop()
    var w = St2SlabWriter(measure = true)
    var probe : St2DecSlab
    st2_dec_write(w, d, probe)
    w.host |> reserve_resize(w.n)
    w.n = 0l
    w.measure = false
    st2_dec_write(w, d, g_tw_st2_dec)
    if (!st2_slab_upload(g_tw_st2_dec, w, key)) {
        st2_dec_drop()
        return false
    }
    return true
}

def private st2_pred_write(var w : St2SlabWriter; p : St2Predictor; var s : St2PredSlab) {
    s.bert_enc = st2_slab_linear(w, p.bert_encoder)
    s.de_lstm |> reserve(length(p.lstms))
    for (l in p.lstms) {
        s.de_lstm |> push(st2_slab_lstm(w, l))
    }
    s.dur_lstm = st2_slab_lstm(w, p.lstm)
    s.dur_proj = st2_slab_linear(w, p.duration_proj)
    s.sh_lstm = st2_slab_lstm(w, p.shared_lstm)
    s.f0b |> reserve(length(p.f0))
    s.nb |> reserve(length(p.n))
    for (b in p.f0) {
        s.f0b |> push(st2_slab_res(w, b))
    }
    for (b in p.n) {
        s.nb |> push(st2_slab_res(w, b))
    }
    s.f0_proj = st2_slab_conv(w, p.f0_proj)
    s.n_proj = st2_slab_conv(w, p.n_proj)
}

def private st2_pred_drop {
    var s & = g_tw_st2_pred
    st2_base_drop(s)
    st2_slot_release(s.bert_enc)
    for (l in s.de_lstm) {
        st2_lstm_release(l)
    }
    st2_lstm_release(s.dur_lstm)
    st2_slot_release(s.dur_proj)
    st2_lstm_release(s.sh_lstm)
    for (b in s.f0b) {
        st2_res_release(b)
    }
    for (b in s.nb) {
        st2_res_release(b)
    }
    st2_slot_release(s.f0_proj)
    st2_slot_release(s.n_proj)
    unsafe {
        delete s.de_lstm
        delete s.f0b
        delete s.nb
    }
}

[cold_path]
def private st2_pred_attach(p : St2Predictor) : bool {
    let key = st2_pred_key(p)
    if (g_tw_st2_pred.buf != null && g_tw_st2_pred.key == key) {
        return true
    }
    st2_pred_drop()
    var w = St2SlabWriter(measure = true)
    var probe : St2PredSlab
    st2_pred_write(w, p, probe)
    w.host |> reserve_resize(w.n)
    w.n = 0l
    w.measure = false
    st2_pred_write(w, p, g_tw_st2_pred)
    if (!st2_slab_upload(g_tw_st2_pred, w, key)) {
        st2_pred_drop()
        return false
    }
    return true
}

def private st2_albert_write(var w : St2SlabWriter; a : St2Albert; var s : St2AlbertSlab) {
    s.word_emb = st2_slab_raw(w, a.word_emb.a, long_length(a.word_emb.a))
    s.pos_emb = st2_slab_raw(w, a.pos_emb.a, long_length(a.pos_emb.a))
    s.type_emb = st2_slab_raw(w, a.type_emb.a, a.emb_dim)
    s.emb_ln = st2_slab_norm_rows(w, a.emb_ln, a.emb_dim)
    s.map_in = st2_slab_linear(w, a.map_in)
    s.q = st2_slab_linear(w, a.q)
    s.k = st2_slab_linear(w, a.k)
    s.v = st2_slab_linear(w, a.v)
    s.dense = st2_slab_linear(w, a.dense)
    s.attn_ln = st2_slab_norm_rows(w, a.attn_ln, a.hidden)
    s.ffn = st2_slab_linear(w, a.ffn)
    s.ffn_out = st2_slab_linear(w, a.ffn_out)
    s.full_ln = st2_slab_norm_rows(w, a.full_ln, a.hidden)
}

def private st2_albert_drop {
    var s & = g_tw_st2_albert
    st2_base_drop(s)
    st2_slot_release(s.map_in)
    st2_slot_release(s.q)
    st2_slot_release(s.k)
    st2_slot_release(s.v)
    st2_slot_release(s.dense)
    st2_slot_release(s.ffn)
    st2_slot_release(s.ffn_out)
}

[cold_path]
def private st2_albert_attach(a : St2Albert) : bool {
    let key = st2_albert_key(a)
    if (g_tw_st2_albert.buf != null && g_tw_st2_albert.key == key) {
        return true
    }
    st2_albert_drop()
    var w = St2SlabWriter(measure = true)
    var probe : St2AlbertSlab
    st2_albert_write(w, a, probe)
    w.host |> reserve_resize(w.n)
    w.n = 0l
    w.measure = false
    st2_albert_write(w, a, g_tw_st2_albert)
    if (!st2_slab_upload(g_tw_st2_albert, w, key)) {
        st2_albert_drop()
        return false
    }
    return true
}

def private st2_text_write(var w : St2SlabWriter; te : St2TextEncoder; var s : St2TextSlab) {
    s.emb = st2_slab_raw(w, te.emb.a, long_length(te.emb.a))
    s.cnn |> reserve(length(te.cnn))
    s.ln |> reserve(length(te.cnn_ln))
    for (c in te.cnn) {
        s.cnn |> push(st2_slab_conv(w, c))
    }
    for (n in te.cnn_ln) {
        s.ln |> push(st2_slab_norm_rows(w, n, te.c))
    }
    s.lstm = st2_slab_lstm(w, te.lstm)
}

def private st2_text_drop {
    var s & = g_tw_st2_text
    st2_base_drop(s)
    for (c in s.cnn) {
        st2_slot_release(c)
    }
    st2_lstm_release(s.lstm)
    unsafe {
        delete s.cnn
        delete s.ln
    }
}

[cold_path]
def private st2_text_attach(te : St2TextEncoder) : bool {
    let key = st2_text_key(te)
    if (g_tw_st2_text.buf != null && g_tw_st2_text.key == key) {
        return true
    }
    st2_text_drop()
    var w = St2SlabWriter(measure = true)
    var probe : St2TextSlab
    st2_text_write(w, te, probe)
    w.host |> reserve_resize(w.n)
    w.n = 0l
    w.measure = false
    st2_text_write(w, te, g_tw_st2_text)
    if (!st2_slab_upload(g_tw_st2_text, w, key)) {
        st2_text_drop()
        return false
    }
    return true
}

def private st2_slab_drop {
    st2_dec_drop()
    st2_pred_drop()
    st2_albert_drop()
    st2_text_drop()
}

struct private St2GpuCtx {
    bufs : MetalBuffer? [ST2_MAX_BUFS]
    bsizes : uint64 [ST2_MAX_BUFS]
    n_bufs : int
    us_v : uint [ST2_MAX_UNIFORMS]     //!< the value-keyed uniforms: a value binds once an encode
    us_b : MetalBuffer? [ST2_MAX_UNIFORMS]
    n_us : int
    uf_v : float [ST2_MAX_UNIFORMS]
    uf_b : MetalBuffer? [ST2_MAX_UNIFORMS]
    n_uf : int
    bh : MetalBuffer?
    bytes_h : uint64
    hcur : int64
    hcap : int64
    bstats : MetalBuffer?
    exact : bool         //!< the GEMMs on the f32-exact stamps: the front end, whose durations round and whose F0 drives a phase
    bytes_stats : uint64
    bcols : MetalBuffer?
    bytes_cols : uint64
    slab : MetalBuffer?   //!< the part's slab every dispatch of the encode binds
}

def private st2_ctx_make(var c : St2GpuCtx; var slab : MetalBuffer?; h_floats, stats_c : int64; cols_bytes : uint64) {
    c.slab = slab
    c.bytes_h = uint64(max(h_floats, 1l) * 4l)
    c.bh = pool_acquire_pinned(g_tw_pool, g_dev, c.bytes_h)
    c.hcap = max(h_floats, 1l)
    c.bytes_stats = uint64(max(2l * stats_c, 2l) * 4l)
    c.bstats = pool_acquire_pinned(g_tw_pool, g_dev, c.bytes_stats)
    c.bytes_cols = max(cols_bytes, 64ul)
    c.bcols = pool_acquire_pinned(g_tw_pool, g_dev, c.bytes_cols)
}

//! `rows` rows of `width` floats, the rows up to the 32 lattice zeroed.
def private st2_rows_buf(var c : St2GpuCtx; rows, width : int64) : MetalBuffer? {
    let mp = st2_ceil(rows, 32l)
    let bytes = uint64(mp * width * 4l)
    assert(c.n_bufs < ST2_MAX_BUFS)
    var b = pool_acquire_pinned(g_tw_pool, g_dev, bytes)
    zero_rows(b, rows, mp, width)
    c.bufs[c.n_bufs] = b
    c.bsizes[c.n_bufs] = bytes
    c.n_bufs++
    return b
}

def private st2_ctx_release(var c : St2GpuCtx) {
    for (i in range(c.n_bufs)) {
        pool_release(g_tw_pool, c.bufs[i], c.bsizes[i])
    }
    pool_release(g_tw_pool, c.bh, c.bytes_h)
    pool_release(g_tw_pool, c.bstats, c.bytes_stats)
    pool_release(g_tw_pool, c.bcols, c.bytes_cols)
    for (i in range(c.n_us)) {
        pool_release(g_upool, c.us_b[i], 4ul)
    }
    for (i in range(c.n_uf)) {
        pool_release(g_upool, c.uf_b[i], 4ul)
    }
}

//! The uniform holding `v`, bound once an encode (the shape gates bound the distinct values).
def private st2_u(var c : St2GpuCtx; v : uint) : MetalBuffer? {
    for (i in range(c.n_us)) {
        return c.us_b[i] if (c.us_v[i] == v)
    }
    assert(c.n_us < ST2_MAX_UNIFORMS)
    c.us_v[c.n_us] = v
    c.us_b[c.n_us] = uniform_u32(v)
    c.n_us++
    return c.us_b[c.n_us - 1]
}

def private st2_uf(var c : St2GpuCtx; v : float) : MetalBuffer? {
    for (i in range(c.n_uf)) {
        return c.uf_b[i] if (c.uf_v[i] == v)
    }
    assert(c.n_uf < ST2_MAX_UNIFORMS)
    c.uf_v[c.n_uf] = v
    c.uf_b[c.n_uf] = uniform_f32(v)
    c.n_uf++
    return c.uf_b[c.n_uf - 1]
}

def private st2_upload(b : MetalBuffer?; src : array<float>; n : int64) {
    unsafe {
        memcpy(metal_buffer_contents(b), addr(src[0]), uint64(n * 4l))
    }
}

def private st2_readback(b : MetalBuffer?; var dst : array<float>; n : int64) {
    unsafe {
        memcpy(addr(dst[0]), metal_buffer_contents(b), uint64(n * 4l))
    }
}

//! The style fc at the h cursor - (1 + gamma, beta) for an AdaLN site - and its byte offset.
def private st2_style_rows(var c : St2GpuCtx; fc : TtsLinear; s : array<float>; cc : int64; ln_form : bool) : uint64 {
    linear_vec(fc, s, g_tw_st2_style_fc)
    let hoff = c.hcur
    assert(hoff + 2l * cc <= c.hcap)
    unsafe {
        var p = reinterpret<float?>(metal_buffer_contents(c.bh)) + hoff
        memcpy(p, addr(g_tw_st2_style_fc[0]), 2l * cc * 4l)
        if (ln_form) {
            for (i in range64(cc)) {
                p[i] = p[i] + 1.0
            }
        }
    }
    c.hcur += 2l * cc
    return uint64(hoff * 4l)
}

def private st2_chunk_rows(kdim_p : int64) : int64 => clamp(ST2_IM2COL_CHUNK_MAX_BYTES / (kdim_p * 4l) / 32l * 32l, 32l, ST2_CHUNK_ROWS)

def private st2_cols_bytes(c : TtsConv1d) : uint64 {
    return 0ul if (c.cin % 8l == 0l)
    let kdim_p = st2_ceil(c.k * c.cin, 32l)
    return uint64(st2_chunk_rows(kdim_p) * kdim_p * 4l)
}

def private st2_conv_args(cv : TtsConv1d; t_in, r0, rows : int64) : St2ConvArgs {
    return St2ConvArgs(cin = uint(cv.cin), k = uint(cv.k), stride = uint(cv.stride),
        pad_l = uint(cv.pad_l), dil = uint(cv.dilation), t_in = uint(t_in), r0 = uint(r0), rows = uint(rows),
        transposed = cv.transposed ? 1u : 0u)
}

//! One rows conv and its bias into rows of stride slot.cout_p; the output row count.
def private st2_conv(enc : MetalComputeEncoder?; var c : St2GpuCtx; slot : St2ConvSlot; cv : TtsConv1d;
                     bx : MetalBuffer?; t_in : int64; by : MetalBuffer?) : int64 {
    let t_out = conv1d_out_len(cv, t_in)
    if (slot.gathers) {
        var ka = st2_conv_args(cv, t_in, 0l, t_out)
        if (c.exact) {
            enc_st2_conv_exact_mm(enc, c.slab, slot.woff, bx, by, 0ul, slot.u_cout, ka, st2_ceil(t_out, 32l), slot.cout_p)
        } else {
            enc_st2_conv_mm(enc, c.slab, slot.woff, bx, by, 0ul, slot.u_cout, ka, st2_ceil(t_out, 32l), slot.cout_p)
        }
    }
    var r0 = slot.gathers ? t_out : 0l
    let chunk = st2_chunk_rows(slot.kdim_p)
    while (r0 < t_out) {
        let rows = min(chunk, t_out - r0)
        var ka = st2_conv_args(cv, t_in, r0, rows)
        let mp = st2_ceil(rows, 32l)
        enc_st2_im2col(enc, bx, c.bcols, ka, mp * slot.kdim_p)
        st2_mm(enc, c, slot.woff, c.bcols, by, uint64(r0 * slot.cout_p * 4l), slot.u_kdim, slot.u_cout, mp, slot.cout_p)
        r0 += rows
    }
    let n = t_out * slot.cout_p
    let u_n = st2_u(c, uint(n))
    enc_add_bias_rows(enc, by, 0ul, c.slab, slot.boff, slot.u_cout, u_n, n)
    g_tower_convs++
    return t_out
}

def private st2_mm(enc : MetalComputeEncoder?; c : St2GpuCtx; woff : uint64; bx, by : MetalBuffer?; yoff : uint64; u_kdim, u_cout : MetalBuffer?; mp, d : int64) {
    if (c.exact) {
        enc_f32_exact_mm(enc, c.slab, woff, bx, by, yoff, u_kdim, u_cout, mp, d)
    } else {
        enc_f32_mm(enc, c.slab, woff, bx, by, yoff, u_kdim, u_cout, mp, d)
    }
}

def private st2_lin(enc : MetalComputeEncoder?; var c : St2GpuCtx; slot : St2ConvSlot; bx : MetalBuffer?; rows : int64; by : MetalBuffer?) {
    st2_mm(enc, c, slot.woff, bx, by, 0ul, slot.u_kdim, slot.u_cout, st2_ceil(rows, 32l), slot.cout_p)
    let n = rows * slot.cout_p
    let u_n = st2_u(c, uint(n))
    enc_add_bias_rows(enc, by, 0ul, c.slab, slot.boff, slot.u_cout, u_n, n)
}

def private st2_ln(enc : MetalComputeEncoder?; var c : St2GpuCtx; bx, by : MetalBuffer?; rows, d : int64; eps : float; wbuf : MetalBuffer?; woff, boff : uint64) {
    let u_d = st2_u(c, uint(d))
    let u_eps = st2_uf(c, eps)
    enc_ln(enc, bx, wbuf, woff, wbuf, boff, by, u_d, u_eps, rows)
}

//! AdaIN and the site's activation from bin into bout; the two may be one buffer.
def private st2_adain(enc : MetalComputeEncoder?; var c : St2GpuCtx; bin, bout : MetalBuffer?; t, cc : int64;
                      a : TtsAdaIn; ns : St2NormSlot; s : array<float>; snake : bool) {
    let hoff = st2_style_rows(c, a.fc, s, cc, false)
    let u_t = st2_u(c, uint(t))
    let u_c = st2_u(c, uint(cc))
    enc_st2_colstats(enc, bin, c.bstats, u_t, u_c, cc)
    let n = t * cc
    var ka = St2AdainArgs(c = uint(cc), t = uint(t), eps = ST2_ADAIN_EPS)
    if (snake) {
        enc_st2_adain_snake(enc, bin, bout, c.bstats, c.bh, hoff, c.slab, ns.gwoff, c.slab, ns.gboff, c.slab, ns.aoff, ka, n)
    } else {
        enc_st2_adain_leaky(enc, bin, bout, c.bstats, c.bh, hoff, c.slab, ns.gwoff, c.slab, ns.gboff, ka, n)
    }
}

def private st2_add(enc : MetalComputeEncoder?; var c : St2GpuCtx; ba, bb, bo : MetalBuffer?; n : int64; scale : float) {
    let u_scale = st2_uf(c, scale)
    let u_n = st2_u(c, uint(n))
    enc_st2_add(enc, ba, bb, bo, u_scale, u_n, n)
}

def private st2_leaky(enc : MetalComputeEncoder?; var c : St2GpuCtx; bx : MetalBuffer?; n : int64; slope : float) {
    let u_slope = st2_uf(c, slope)
    let u_n = st2_u(c, uint(n))
    enc_st2_leaky(enc, bx, u_slope, u_n, n)
}

def private st2_bilstm(enc : MetalComputeEncoder?; var c : St2GpuCtx; l : TtsBiLstm; sl : St2LstmSlot;
                       bx : MetalBuffer?; t : int64; bxg_f, bxg_b, by : MetalBuffer?) {
    let hd = l.fwd.hidden
    st2_lin(enc, c, sl.fx, bx, t, bxg_f)
    st2_lin(enc, c, sl.bx, bx, t, bxg_b)
    var kf = St2LstmArgs(t = uint(t), hidden = uint(hd), xgs = uint(sl.fx.cout_p), ystride = uint(2l * hd), yoff = 0u, reverse = 0u)
    enc_st2_lstm(enc, bxg_f, by, c.slab, sl.fhh, kf, 4l * hd)
    var kb = St2LstmArgs(t = uint(t), hidden = uint(hd), xgs = uint(sl.bx.cout_p), ystride = uint(2l * hd), yoff = uint(hd), reverse = 1u)
    enc_st2_lstm(enc, bxg_b, by, c.slab, sl.bhh, kb, 4l * hd)
}

struct private St2ResBufs {
    br : MetalBuffer?
    bpool : MetalBuffer?
    bc1 : MetalBuffer?
    bsc : MetalBuffer?
    bsc2 : MetalBuffer?
    bidx : MetalBuffer?    //!< the nearest-upsample row map [2 t]
}

def private st2_res_bufs(var c : St2GpuCtx; rows, width : int64) : St2ResBufs {
    var r = St2ResBufs(br = st2_rows_buf(c, rows, width), bpool = st2_rows_buf(c, rows, width), bc1 = st2_rows_buf(c, rows, width),
        bsc = st2_rows_buf(c, rows, width), bsc2 = st2_rows_buf(c, rows, width), bidx = st2_rows_buf(c, rows, 1l))
    unsafe {
        var p = reinterpret<uint?>(metal_buffer_contents(r.bidx))
        for (i in range64(rows)) {
            p[i] = uint(i / 2l)
        }
    }
    return r
}

//! The CPU adain_res_blk, dispatch for dispatch, from bin [t][dim_in_s] into bout [t1][dim_out]; t1 back.
def private st2_res_blk(enc : MetalComputeEncoder?; var c : St2GpuCtx; b : St2AdainResBlk; sl : St2ResSlots; r : St2ResBufs;
                        bin : MetalBuffer?; t : int64; s : array<float>; bout : MetalBuffer?) : int64 {
    st2_adain(enc, c, bin, r.br, t, b.dim_in_s, b.norm1, sl.n1, s, false)
    var t1 = t
    if (b.upsample) {
        t1 = conv1d_out_len(b.pool, t)
        var ka = St2PoolArgs(c = uint(b.pool.cin), k = uint(b.pool.k), stride = uint(b.pool.stride), pad_l = uint(b.pool.pad_l),
            dil = uint(b.pool.dilation), t_in = uint(t), total = uint(t1 * b.pool.cin))
        enc_st2_pool_dw(enc, r.br, r.bpool, c.slab, sl.pool_w, c.slab, sl.pool_b, ka, t1 * b.pool.cin)
        st2_conv(enc, c, sl.c1, b.conv1, r.bpool, t1, r.bc1)
    } else {
        st2_conv(enc, c, sl.c1, b.conv1, r.br, t1, r.bc1)
    }
    st2_adain(enc, c, r.bc1, r.bc1, t1, b.dim_out, b.norm2, sl.n2, s, false)
    st2_conv(enc, c, sl.c2, b.conv2, r.bc1, t1, bout)
    let n = t1 * b.dim_out
    if (b.upsample) {
        var kg = St2GatherArgs(c = uint(b.dim_in_s), add_b = 0u, add_c = 0u, total = uint(t1 * b.dim_in_s))
        enc_st2_gather(enc, bin, 0ul, r.bidx, bin, 0ul, bin, 0ul, r.bsc, kg, t1 * b.dim_in_s)
        if (b.learned_sc) {
            st2_conv(enc, c, sl.c1x1, b.conv1x1, r.bsc, t1, r.bsc2)
            st2_add(enc, c, bout, r.bsc2, bout, n, ST2_JOIN)
        } else {
            st2_add(enc, c, bout, r.bsc, bout, n, ST2_JOIN)
        }
    } elif (b.learned_sc) {
        st2_conv(enc, c, sl.c1x1, b.conv1x1, bin, t1, r.bsc2)
        st2_add(enc, c, bout, r.bsc2, bout, n, ST2_JOIN)
    } else {
        st2_add(enc, c, bout, bin, bout, n, ST2_JOIN)
    }
    return t1
}

//! A Snake residual block from bin into bout, the CPU `gen_res_block` dispatch for dispatch.
def private st2_gen_res_block(enc : MetalComputeEncoder?; var c : St2GpuCtx; b : St2GenResBlock; sl : St2BlockSlots;
                              bxt, by1 : MetalBuffer?; bin, bout : MetalBuffer?; t : int64; s : array<float>) {
    let n = t * b.c
    for (j in range(length(b.convs1))) {
        let src = j == 0 ? bin : bout
        st2_adain(enc, c, src, bxt, t, b.c, b.adain1[j], sl.n1[j], s, true)
        st2_conv(enc, c, sl.c1[j], b.convs1[j], bxt, t, by1)
        st2_adain(enc, c, by1, by1, t, b.c, b.adain2[j], sl.n2[j], s, true)
        st2_conv(enc, c, sl.c2[j], b.convs2[j], by1, t, bxt)
        st2_add(enc, c, src, bxt, bout, n, 1.0)
    }
}

struct private St2GenBufs {
    by : MetalBuffer?
    bsrc : MetalBuffer?
    bsres : MetalBuffer?
    bxt : MetalBuffer?
    by1 : MetalBuffer?
    bo : MetalBuffer?
}

def private st2_gen_bufs(var c : St2GpuCtx; floats_rows : int64) : St2GenBufs {
    return St2GenBufs(by = st2_rows_buf(c, floats_rows, 1l), bsrc = st2_rows_buf(c, floats_rows, 1l), bsres = st2_rows_buf(c, floats_rows, 1l),
        bxt = st2_rows_buf(c, floats_rows, 1l), by1 = st2_rows_buf(c, floats_rows, 1l), bo = st2_rows_buf(c, floats_rows, 1l))
}

//! The CPU generator_stage, dispatch for dispatch, from bx_in into bout.
def private st2_stage(enc : MetalComputeEncoder?; var c : St2GpuCtx; d : St2Decoder; i : int64; g : St2GenBufs; bspec : MetalBuffer?;
                      bx_in, bout : MetalBuffer?; var tt, cc : int64&; har_frames : int64; s : array<float>) {
    let sl & = g_tw_st2_dec
    st2_leaky(enc, c, bx_in, tt * cc, 0.1)
    let src_t = st2_conv(enc, c, sl.noise[i], d.noise_convs[i], bspec, har_frames, g.bsrc)
    st2_gen_res_block(enc, c, d.noise_res[i], sl.blocks[i], g.bxt, g.by1, g.bsrc, g.bsres, src_t, s)
    let last = i == long_length(d.ups) - 1l
    var t2 = st2_conv(enc, c, sl.ups[i], d.ups[i], bx_in, tt, last ? g.bxt : g.by)
    let c2 = d.ups[i].cout
    if (last) {
        t2++
    }
    let n = t2 * c2
    let u_n = st2_u(c, uint(n))
    if (last) {
        let u_c = st2_u(c, uint(c2))
        enc_st2_reflect1(enc, g.bxt, g.by, u_c, u_n, n)
    }
    st2_add(enc, c, g.by, g.bsres, g.by, n, 1.0)
    let nk = d.n_kernels
    let nnoise = long_length(d.noise_res)
    let u_scale = st2_uf(c, 1.0 / float(nk))
    let u_one = st2_u(c, 1u)
    let u_zero = st2_u(c, 0u)
    for (j in range64(nk)) {
        st2_gen_res_block(enc, c, d.resblocks[i * nk + j], sl.blocks[nnoise + i * nk + j], g.bxt, g.by1, g.by, g.bsres, t2, s)
        enc_st2_axpy(enc, bout, g.bsres, u_scale, j == 0l ? u_one : u_zero, u_n, n)
    }
    tt = t2
    cc = c2
}

//! The generator's shape, from the CPU chain's own length rules; false on any shape the kernels do not take.
struct private St2Shape {
    t_last : int64
    c_last : int64
    c_max : int64
    t_post : int64
    row_buf_floats : int64
    style_fc_floats : int64
    cols_bytes : uint64
}

def private st2_note_conv(var sh : St2Shape; c : TtsConv1d) : bool {
    return false if (!st2_conv_ok(c))
    sh.cols_bytes = max(sh.cols_bytes, st2_cols_bytes(c))
    return true
}

def private st2_note_block(var sh : St2Shape; b : St2GenResBlock; t, c : int64) : bool {
    let n = length(b.convs1)
    return false if (b.c != c || n == 0 || length(b.convs2) != n || length(b.adain1) != n || length(b.adain2) != n
        || length(b.alpha1) != n || length(b.alpha2) != n)
    for (j in range(n)) {
        let c1 & = unsafe(b.convs1[j])
        let c2 & = unsafe(b.convs2[j])
        return false if (!st2_note_conv(sh, c1) || !st2_note_conv(sh, c2)
            || c1.cin != c || c1.cout != c || c2.cin != c || c2.cout != c
            || conv1d_out_len(c1, t) != t || conv1d_out_len(c2, t) != t
            || long_length(b.alpha1[j].a) < c || long_length(b.alpha2[j].a) < c)
    }
    sh.style_fc_floats += int64(n) * 2l * 2l * c
    return true
}

def private st2_gen_shape(d : St2Decoder; c, t, har_frames : int64; var sh : St2Shape) : bool {
    let nst = long_length(d.ups)
    return false if (nst == 0l || nst > ST2_MAX_STAGES || t < 1l || har_frames < 1l || d.n_kernels < 1l
        || long_length(d.noise_convs) != nst || long_length(d.noise_res) != nst || long_length(d.resblocks) != nst * d.n_kernels)
    var tt = t
    var cc = c
    sh.c_max = max(sh.c_max, c)
    sh.row_buf_floats = max(sh.row_buf_floats, st2_ceil(t, 32l) * c)
    for (i in range64(nst)) {
        let up & = unsafe(d.ups[i])
        let nc & = unsafe(d.noise_convs[i])
        return false if (!st2_note_conv(sh, up) || !st2_note_conv(sh, nc) || up.cin != cc || nc.cin != d.noise_convs[0].cin)
        var t2 = conv1d_out_len(up, tt)
        if (i == nst - 1l) {
            t2++
        }
        let c2 = up.cout
        return false if (c2 % 64l != 0l || nc.cout != c2 || conv1d_out_len(nc, har_frames) != t2
            || !st2_note_block(sh, d.noise_res[i], t2, c2))
        for (j in range64(d.n_kernels)) {
            return false if (!st2_note_block(sh, d.resblocks[i * d.n_kernels + j], t2, c2))
        }
        sh.row_buf_floats = max(sh.row_buf_floats, st2_ceil(t2, 32l) * c2)
        sh.c_max = max(sh.c_max, c2)
        tt = t2
        cc = c2
    }
    return false if (!st2_note_conv(sh, d.conv_post) || d.conv_post.cin != cc || d.conv_post.cout > ST2_POST_WIDTH
        || d.conv_post.cout < 2l * d.bins)
    sh.t_post = conv1d_out_len(d.conv_post, tt)
    sh.row_buf_floats = max(sh.row_buf_floats, st2_ceil(sh.t_post, 32l) * ST2_POST_WIDTH)
    sh.t_last = tt
    sh.c_last = cc
    return true
}

//! The generator from the rows in bx through conv_post into bpost [t_post][ST2_POST_WIDTH].
def private st2_generator_chain(enc : MetalComputeEncoder?; var c : St2GpuCtx; d : St2Decoder; g : St2GenBufs; bx, bspec, bpost : MetalBuffer?;
                                cx, t, har_frames : int64; s : array<float>) {
    let nst = long_length(d.ups)
    var tt = t
    var cc = cx
    for (i in range64(nst)) {
        if (i % 2l == 0l) {
            st2_stage(enc, c, d, i, g, bspec, bx, g.bo, tt, cc, har_frames, s)
        } else {
            st2_stage(enc, c, d, i, g, bspec, g.bo, bx, tt, cc, har_frames, s)
        }
    }
    let bfin = nst % 2l == 1l ? g.bo : bx
    st2_leaky(enc, c, bfin, tt * cc, 0.01)
    st2_conv(enc, c, g_tw_st2_dec.post, d.conv_post, bfin, tt, bpost)
}

[hot_path, arch(at="../ARCHITECTURE_GPU_TOWER.md#tower-tts-chain")]
def private metal_styletts2_generator(d : St2Decoder; x_rows : array<float>; cx, t : int64; s : array<float>;
                                      har_spec : array<float>; har_frames : int64; @scratch @exact_size var wave : array<float>; var sc : St2Scratch) : bool {
    if (!g_tw_env_tower) {
        return tw_decline(MetalTowerDecline.knob)
    }
    var sh = St2Shape()
    if (!st2_gen_shape(d, cx, t, har_frames, sh)) {
        return tw_decline(MetalTowerDecline.shape)
    }
    var tc = ref_time_ticks()
    if (!st2_dec_ok(d)) {
        return tw_decline(MetalTowerDecline.shape)
    }
    if (!metal_tower_init() || !st2_dec_attach(d)) {
        return tw_decline(MetalTowerDecline.device)
    }
    asr_prof_add("tts.gen.gpu.slab", tc)
    tc = ref_time_ticks()
    residency_flush()
    var c = St2GpuCtx()
    st2_ctx_make(c, g_tw_st2_dec.buf, sh.style_fc_floats, sh.c_max, sh.cols_bytes)
    var g = st2_gen_bufs(c, sh.row_buf_floats)
    var bx = st2_rows_buf(c, sh.row_buf_floats, 1l)
    let spec_floats = har_frames * d.noise_convs[0].cin
    var bspec = st2_rows_buf(c, spec_floats, 1l)
    st2_upload(bx, x_rows, t * cx)
    st2_upload(bspec, har_spec, spec_floats)
    asr_prof_add("tts.gen.gpu.upload", tc)
    tc = ref_time_ticks()
    var err = ""
    let ran = with_compute_encoder(g_queue, err) $(enc : MetalComputeEncoder?) {   // nolint:PERF026 — the error-text fetch runs only on the failure leg
        st2_generator_chain(enc, c, d, g, bx, bspec, g.by, cx, t, har_frames, s)
    }
    asr_prof_add("tts.gen.gpu.chain", tc)
    if (ran) {
        tc = ref_time_ticks()
        let n_post = sh.t_post * ST2_POST_WIDTH
        unsafe(scratch_resize(sc.gen_post, n_post))
        var y & = unsafe(sc.gen_post)
        st2_readback(g.by, y, n_post)
        styletts2_istft(d, y, ST2_POST_WIDTH, sh.t_post, wave, sc)
        g_tower_encodes++
        g_tower_rows += sh.t_post
        asr_prof_add("tts.gen.gpu.tail", tc)
    }
    st2_ctx_release(c)
    if (!ran) {
        return tw_decline(MetalTowerDecline.gpu_error)
    }
    return true
}

//! The block's output row count, or -1 on a shape the kernels do not take.
def private st2_res_shape(var sh : St2Shape; b : St2AdainResBlk; t : int64) : int64 {
    return -1l if (b.dim_in_s % 32l != 0l || b.dim_out % 64l != 0l || b.dim_in_s < b.dim_in
        || !st2_note_conv(sh, b.conv1) || !st2_note_conv(sh, b.conv2) || b.conv1.cout != b.dim_out || b.conv2.cin != b.dim_out
        || b.conv2.cout != b.dim_out
        || (b.learned_sc && (!st2_note_conv(sh, b.conv1x1) || b.conv1x1.cout != b.dim_out || b.conv1x1.cin != b.dim_in_s)))
    var t1 = t
    if (b.upsample) {
        return -1l if (b.pool.groups != b.pool.cin || b.pool.cin != b.pool.cout || !b.pool.transposed || empty(b.pool.w) || b.pool.cin != b.dim_in_s)
        t1 = conv1d_out_len(b.pool, t)
        return -1l if (t1 != 2l * t)
    }
    return -1l if (conv1d_out_len(b.conv1, t1) != t1 || conv1d_out_len(b.conv2, t1) != t1)
    sh.style_fc_floats += 2l * (b.dim_in_s + b.dim_out)
    sh.c_max = max(sh.c_max, max(b.dim_in_s, b.dim_out))
    sh.row_buf_floats = max(sh.row_buf_floats, st2_ceil(t1, 32l) * max(b.dim_in_s, b.dim_out))
    return t1
}

//! The source's geometry for `f2` frames.
struct private St2SourceShape {
    samples : int64
    nlow : int64
    spec_frames : int64
}

let private ST2_MAX_DE_LSTMS = 8   //!< the duration encoder's LSTM stack the seat holds buffers for
let private ST2_SRC_MAX_SAMPLES = 1l << 23   //!< past it `float(to) + 0.5` rounds, and the torch law's tap coordinate parts from the CPU's double form

def private st2_source_shape(d : St2Decoder; cfg : SineSourceCfg; f2 : int64; var ss : St2SourceShape) : bool {
    ss.samples = f2 * cfg.upsample
    ss.nlow = resize_len(ss.samples, 1.0 / float(cfg.upsample))
    return false if (ss.samples < 1l || ss.samples >= ST2_SRC_MAX_SAMPLES || ss.nlow < 1l || cfg.n_harm < 1l || cfg.n_harm > 64l || resize_len(ss.nlow, float(cfg.upsample)) != ss.samples)
    let re & = unsafe(d.stft_fwd_re)
    let im & = unsafe(d.stft_fwd_im)
    return false if (re.cin != 1l || re.cout != d.bins || re.transposed || im.k != re.k || im.cout != d.bins || im.cin != 1l || im.transposed
        || re.pad_l != 0l || re.pad_r != 0l || re.dilation != 1l || im.pad_l != 0l || im.pad_r != 0l || im.dilation != 1l || im.stride != re.stride
        || d.source_linear.nin != cfg.n_harm || d.source_linear.nout != 1l || empty(d.source_linear.b) || empty(re.w) || empty(im.w))
    ss.spec_frames = conv1d_out_len(re, ss.samples + 2l * ST2_STFT_PAD)
    return ss.spec_frames >= 1l
}

def private st2_source_chain(enc : MetalComputeEncoder?; var c : St2GpuCtx; d : St2Decoder; cfg : SineSourceCfg; ss : St2SourceShape;
                             bf0, bnu, bnoise, blow, blowc, bhar, bspec : MetalBuffer?; captured : bool; seed : uint64) {
    let nh = cfg.n_harm
    var ka = St2SourceArgs(n = uint(ss.samples), nlow = uint(ss.nlow), nh = uint(nh), up = uint(cfg.upsample), sr = cfg.sample_rate,
        sine_amp = cfg.sine_amp, noise_std = cfg.noise_std, voiced_thr = cfg.voiced_thr, torch_math = cfg.torch_math ? 1u : 0u,
        captured = captured ? 1u : 0u, seed = uint(seed ^ (seed >> 32ul)))
    enc_st2_src_low(enc, bf0, bnu, blow, ka, nh * ss.nlow)
    enc_st2_src_cumsum(enc, blow, blowc, ka, nh)
    let sl & = g_tw_st2_dec
    enc_st2_src_sines(enc, bf0, blowc, bnoise, c.slab, sl.src_w, c.slab, sl.src_b, bhar, ka, ss.samples)
    let re & = unsafe(d.stft_fwd_re)
    var ks = St2StftArgs(n = uint(ss.samples), frames = uint(ss.spec_frames), bins = uint(d.bins), k = uint(re.k), hop = uint(re.stride),
        pad = uint(ST2_STFT_PAD), reflect = d.stft_pad_reflect ? 1u : 0u, eps = d.stft_eps)
    enc_st2_stft(enc, bhar, c.slab, sl.stft_re, c.slab, sl.stft_im, bspec, ks, ss.spec_frames * d.bins)
}

//! The decode chain's shape: the front convs and the residual as the concat rows ask, the AdaIN
//! residual blocks, the source, the generator, the inverse STFT.
struct private St2DecShape : St2Shape {
    ss : St2SourceShape = St2SourceShape()
    dec_rows : int64
    dec_width : int64
    n_out : int64
}

def private st2_decode_shape(d : St2Decoder; text_c : int64; source : SineSourceCfg; frames, f2 : int64; noise : TtsNoise; var sh : St2DecShape) : bool {
    let bwd & = unsafe(d.stft_bwd_re)
    return false if (frames < 1l || f2 < 1l || text_c % 32l != 0l || !st2_note_conv(sh, d.f0_conv) || !st2_note_conv(sh, d.n_conv)
        || !st2_note_conv(sh, d.asr_res) || d.f0_conv.cin != 1l || d.n_conv.cin != 1l || d.f0_conv.cout != 1l || d.n_conv.cout != 1l
        || conv1d_out_len(d.f0_conv, f2) != frames || conv1d_out_len(d.n_conv, f2) != frames || d.asr_res.cin != text_c
        || d.asr_res.cout % 64l != 0l || d.encode.dim_in != text_c + 2l || !st2_source_shape(d, source, f2, sh.ss)
        || bwd.cin != d.bins || bwd.cout != 1l || !bwd.transposed || empty(bwd.wt) || d.stft_bwd_im.k != bwd.k
        || (d.istft_envelope && long_length(d.window.a) < bwd.k) || (noise.captured && long_length(noise.normal) < sh.ss.samples * source.n_harm))
    sh.dec_rows = st2_res_shape(sh, d.encode, frames)
    return false if (sh.dec_rows < 0l)
    sh.dec_width = d.encode.dim_out
    for (b in d.decode) {
        return false if ((b.dim_in != sh.dec_width && b.dim_in != sh.dec_width + d.asr_res.cout + 2l) || (b.dim_in == sh.dec_width && b.dim_in_s != b.dim_in))
        sh.dec_rows = st2_res_shape(sh, b, sh.dec_rows)
        return false if (sh.dec_rows < 0l)
        sh.dec_width = b.dim_out
    }
    return false if (!st2_gen_shape(d, sh.dec_width, sh.dec_rows, sh.ss.spec_frames, sh))
    sh.row_buf_floats = max(sh.row_buf_floats, st2_ceil(frames, 32l) * max(d.encode.dim_in_s, text_c))
    sh.n_out = conv1d_out_len(bwd, sh.t_post) - 2l * ST2_STFT_PAD
    return sh.n_out >= 1l
}

struct private St2DecBufs {
    bx : MetalBuffer?
    basr : MetalBuffer?
    bres : MetalBuffer?
    bf0r : MetalBuffer?
    bnr : MetalBuffer?
    bf0d : MetalBuffer?
    bnd : MetalBuffer?
    bcat : MetalBuffer?
    bnu : MetalBuffer?
    bnoise : MetalBuffer?
    blow : MetalBuffer?
    blowc : MetalBuffer?
    bhar : MetalBuffer?
    bspec : MetalBuffer?
    bwave : MetalBuffer?
}

def private st2_dec_bufs(var c : St2GpuCtx; d : St2Decoder; sh : St2DecShape; source : SineSourceCfg; text_c, frames, f2 : int64; captured : bool) : St2DecBufs {
    let nh = source.n_harm
    return St2DecBufs(bx = st2_rows_buf(c, sh.row_buf_floats, 1l), basr = st2_rows_buf(c, frames, text_c), bres = st2_rows_buf(c, frames, d.asr_res.cout),
        bf0r = st2_rows_buf(c, f2, 1l), bnr = st2_rows_buf(c, f2, 1l), bf0d = st2_rows_buf(c, frames, ST2_POST_WIDTH), bnd = st2_rows_buf(c, frames, ST2_POST_WIDTH),
        bcat = st2_rows_buf(c, sh.row_buf_floats, 1l), bnu = st2_rows_buf(c, nh, 1l), bnoise = st2_rows_buf(c, captured ? sh.ss.samples * nh : 1l, 1l),
        blow = st2_rows_buf(c, nh * sh.ss.nlow, 1l), blowc = st2_rows_buf(c, nh * sh.ss.nlow, 1l), bhar = st2_rows_buf(c, sh.ss.samples, 1l),
        bspec = st2_rows_buf(c, sh.ss.spec_frames * 2l * d.bins, 1l), bwave = st2_rows_buf(c, sh.n_out, 1l))
}

//! The encode block, then each decode block (re-concatenated where its width asks); the stream lands in bx.
def private st2_decoder_chain(enc : MetalComputeEncoder?; var c : St2GpuCtx; d : St2Decoder; b : St2DecBufs; g : St2GenBufs; r : St2ResBufs;
                              text_c, frames : int64; s_ref : array<float>) {
    let sl & = g_tw_st2_dec
    let rc = uint(d.asr_res.cout)
    var kc = St2ConcatArgs(c = uint(text_c), rc = rc, rs = uint(sl.asr_res.cout_p), f0s = uint(ST2_POST_WIDTH), ns = uint(ST2_POST_WIDTH),
        with_res = 0u, width = uint(d.encode.dim_in_s), total = uint(frames * d.encode.dim_in_s))
    enc_st2_concat(enc, b.basr, b.bres, b.bf0d, b.bnd, b.bcat, kc, frames * d.encode.dim_in_s)
    var tt = st2_res_blk(enc, c, d.encode, sl.encode, r, b.bcat, frames, s_ref, b.bx)
    var cwr = d.encode.dim_out
    var in_x = true
    for (i in range(length(d.decode))) {
        let blk & = unsafe(d.decode[i])
        let src = in_x ? b.bx : g.bo
        let dst = in_x ? g.bo : b.bx
        if (blk.dim_in != cwr) {
            var kc2 = St2ConcatArgs(c = uint(cwr), rc = rc, rs = uint(sl.asr_res.cout_p), f0s = uint(ST2_POST_WIDTH), ns = uint(ST2_POST_WIDTH),
                with_res = 1u, width = uint(blk.dim_in_s), total = uint(tt * blk.dim_in_s))
            enc_st2_concat(enc, src, b.bres, b.bf0d, b.bnd, b.bcat, kc2, tt * blk.dim_in_s)
            tt = st2_res_blk(enc, c, blk, sl.decode[i], r, b.bcat, tt, s_ref, dst)
        } else {
            tt = st2_res_blk(enc, c, blk, sl.decode[i], r, src, tt, s_ref, dst)
        }
        in_x = !in_x
        cwr = blk.dim_out
    }
    if (!in_x) {   //! the generator ping-pongs bx and bo from bx: the stream lands in bx first
        st2_add(enc, c, g.bo, g.bo, b.bx, tt * cwr, 0.5)
    }
}

[hot_path, arch(at="../ARCHITECTURE_GPU_TOWER.md#tower-tts-chain")]
def private metal_styletts2_decode(d : St2Decoder; text_c : int64; source : SineSourceCfg; asr : array<float>; frames : int64;
                                   f0, nrg : array<float>; f2 : int64; s_ref : array<float>; noise : TtsNoise; seed : uint64;
                                   @scratch @exact_size var wave : array<float>; var sc : St2Scratch) : bool {
    if (!g_tw_env_tower) {
        return tw_decline(MetalTowerDecline.knob)
    }
    var sh = St2DecShape()
    if (!st2_decode_shape(d, text_c, source, frames, f2, noise, sh)) {
        return tw_decline(MetalTowerDecline.shape)
    }
    var tc = ref_time_ticks()
    if (!st2_dec_ok(d)) {
        return tw_decline(MetalTowerDecline.shape)
    }
    if (!metal_tower_init() || !st2_dec_attach(d)) {
        return tw_decline(MetalTowerDecline.device)
    }
    asr_prof_add("tts.gen.gpu.slab", tc)
    tc = ref_time_ticks()
    residency_flush()
    var c = St2GpuCtx()
    st2_ctx_make(c, g_tw_st2_dec.buf, sh.style_fc_floats, sh.c_max, sh.cols_bytes)
    var g = st2_gen_bufs(c, sh.row_buf_floats)
    var r = st2_res_bufs(c, sh.row_buf_floats, 1l)
    var b = st2_dec_bufs(c, d, sh, source, text_c, frames, f2, noise.captured)
    var asr_tm & = unsafe(sc.dec_asr_tm)
    transpose_ct(asr, text_c, frames, asr_tm)
    st2_upload(b.basr, asr_tm, frames * text_c)
    st2_upload(b.bf0r, f0, f2)
    st2_upload(b.bnr, nrg, f2)
    st2_upload(b.bnu, noise.uniform, source.n_harm)
    if (noise.captured) {
        st2_upload(b.bnoise, noise.normal, sh.ss.samples * source.n_harm)
    }
    asr_prof_add("tts.gen.gpu.upload", tc)
    tc = ref_time_ticks()
    let sl & = g_tw_st2_dec
    let bwd & = unsafe(d.stft_bwd_re)
    var err = ""
    let ran = with_compute_encoder(g_queue, err) $(enc : MetalComputeEncoder?) {   // nolint:PERF026 — the error-text fetch runs only on the failure leg
        st2_conv(enc, c, sl.f0c, d.f0_conv, b.bf0r, f2, b.bf0d)
        st2_conv(enc, c, sl.nc, d.n_conv, b.bnr, f2, b.bnd)
        st2_conv(enc, c, sl.asr_res, d.asr_res, b.basr, frames, b.bres)
        st2_decoder_chain(enc, c, d, b, g, r, text_c, frames, s_ref)
        st2_source_chain(enc, c, d, source, sh.ss, b.bf0r, b.bnu, b.bnoise, b.blow, b.blowc, b.bhar, b.bspec, noise.captured, seed)
        st2_generator_chain(enc, c, d, g, b.bx, b.bspec, r.bc1, sh.dec_width, sh.dec_rows, sh.ss.spec_frames, s_ref)
        var ki = St2IstftArgs(tt = uint(sh.t_post), stride = uint(ST2_POST_WIDTH), bins = uint(d.bins), k = uint(bwd.k), hop = uint(bwd.stride),
            pad = uint(ST2_STFT_PAD), envelope = d.istft_envelope ? 1u : 0u, n = uint(sh.n_out))
        enc_st2_istft(enc, r.bc1, c.slab, sl.istft_re, c.slab, sl.istft_im, c.slab, sl.window, b.bwave, ki, sh.n_out)
    }
    asr_prof_add("tts.gen.gpu.chain", tc)
    if (ran) {
        unsafe(scratch_resize(wave, sh.n_out))
        st2_readback(b.bwave, wave, sh.n_out)
        g_tower_encodes++
        g_tower_rows += sh.t_post
    }
    st2_ctx_release(c)
    if (!ran) {
        return tw_decline(MetalTowerDecline.gpu_error)
    }
    return true
}

def private st2_lstm_ok(l : TtsBiLstm; nin : int64) : bool {
    let hd = l.fwd.hidden
    return (hd >= 4l && hd % 4l == 0l && hd <= ST2_LSTM_MAX_HIDDEN && l.bwd.hidden == hd && l.fwd.nin == nin && l.bwd.nin == nin && (2l * hd) % 32l == 0l
        && long_length(l.fwd.w_hh) >= 4l * hd * hd && long_length(l.bwd.w_hh) >= 4l * hd * hd && long_length(l.fwd.b) >= 4l * hd
        && long_length(l.fwd.w_ih) >= 4l * hd * nin && long_length(l.bwd.w_ih) >= 4l * hd * nin && long_length(l.bwd.b) >= 4l * hd)
}

struct private St2LstmBufs {
    xf : MetalBuffer?
    xb : MetalBuffer?
    y : MetalBuffer?
}

def private st2_lstm_bufs(var c : St2GpuCtx; sl : St2LstmSlot; t, hd : int64) : St2LstmBufs {
    return St2LstmBufs(xf = st2_rows_buf(c, t, sl.fx.cout_p), xb = st2_rows_buf(c, t, sl.bx.cout_p), y = st2_rows_buf(c, t, 2l * hd))
}

//! A prosody branch's blocks, then its single-channel projection into bout [t][ST2_POST_WIDTH].
def private st2_branch(enc : MetalComputeEncoder?; var c : St2GpuCtx; blocks : array<St2AdainResBlk>; slots : array<St2ResSlots>; proj : TtsConv1d;
                       pslot : St2ConvSlot; r : St2ResBufs; bx, ba, bb, bout : MetalBuffer?; frames : int64; s : array<float>) : int64 {
    var t = frames
    let nb = length(blocks)
    for (i in range(nb)) {
        if (i == 0) {
            t = st2_res_blk(enc, c, blocks[i], slots[i], r, bx, t, s, ba)
        } elif (i % 2 == 1) {
            t = st2_res_blk(enc, c, blocks[i], slots[i], r, ba, t, s, bb)
        } else {
            t = st2_res_blk(enc, c, blocks[i], slots[i], r, bb, t, s, ba)
        }
    }
    let src = nb == 0 ? bx : (nb % 2 == 1 ? ba : bb)
    return st2_conv(enc, c, pslot, proj, src, t, bout)
}

[hot_path, arch(at="../ARCHITECTURE_GPU_TOWER.md#tower-tts-chain")]
def private metal_styletts2_prosody(p : St2Predictor; en : array<float>; cc, frames : int64; s : array<float>;
                                    @scratch @exact_size var f0 : array<float>; @scratch @exact_size var n : array<float>; var f2 : int64&; var sc : St2Scratch) : bool {
    if (!g_tw_env_tower) {
        return tw_decline(MetalTowerDecline.knob)
    }
    let hd = p.shared_lstm.fwd.hidden
    var sh = St2Shape()
    if (frames < 1l || cc % 32l != 0l || !st2_lstm_ok(p.shared_lstm, cc) || length(p.f0) != length(p.n) || empty(p.f0)
        || p.f0[0].dim_in_s != 2l * hd || p.n[0].dim_in_s != 2l * hd || p.f0_proj.cout != 1l || p.n_proj.cout != 1l
        || !st2_note_conv(sh, p.f0_proj) || !st2_note_conv(sh, p.n_proj)) {
        return tw_decline(MetalTowerDecline.shape)
    }
    var tf = frames
    var tn = frames
    for (i in range(length(p.f0))) {
        return tw_decline(MetalTowerDecline.shape) if (i > 0 && (p.f0[i].dim_in_s != p.f0[i - 1].dim_out || p.n[i].dim_in_s != p.n[i - 1].dim_out))
        tf = st2_res_shape(sh, p.f0[i], tf)
        tn = st2_res_shape(sh, p.n[i], tn)
        return tw_decline(MetalTowerDecline.shape) if (tf < 0l || tn < 0l)
    }
    return tw_decline(MetalTowerDecline.shape) if (tf != tn || p.f0_proj.cin != p.f0[length(p.f0) - 1].dim_out || p.n_proj.cin != p.n[length(p.n) - 1].dim_out
        || conv1d_out_len(p.f0_proj, tf) != tf || conv1d_out_len(p.n_proj, tn) != tn)
    sh.row_buf_floats = max(sh.row_buf_floats, st2_ceil(frames, 32l) * max(cc, 2l * hd))
    if (!st2_pred_ok(p)) {
        return tw_decline(MetalTowerDecline.shape)
    }
    if (!metal_tower_init() || !st2_pred_attach(p)) {
        return tw_decline(MetalTowerDecline.device)
    }
    residency_flush()
    var c = St2GpuCtx()
    st2_ctx_make(c, g_tw_st2_pred.buf, sh.style_fc_floats, sh.c_max, sh.cols_bytes)
    c.exact = true
    var r = st2_res_bufs(c, sh.row_buf_floats, 1l)
    var ben = st2_rows_buf(c, frames, cc)
    var lb = st2_lstm_bufs(c, g_tw_st2_pred.sh_lstm, frames, hd)
    var ba = st2_rows_buf(c, sh.row_buf_floats, 1l)
    var bb = st2_rows_buf(c, sh.row_buf_floats, 1l)
    var bf0 = st2_rows_buf(c, tf, ST2_POST_WIDTH)
    var bn = st2_rows_buf(c, tn, ST2_POST_WIDTH)
    var en_tm & = unsafe(sc.pr_en_tm)
    transpose_ct(en, cc, frames, en_tm)
    st2_upload(ben, en_tm, frames * cc)
    let sl & = g_tw_st2_pred
    var err = ""
    let ran = with_compute_encoder(g_queue, err) $(enc : MetalComputeEncoder?) {   // nolint:PERF026 — the error-text fetch runs only on the failure leg
        st2_bilstm(enc, c, p.shared_lstm, sl.sh_lstm, ben, frames, lb.xf, lb.xb, lb.y)
        st2_branch(enc, c, p.f0, sl.f0b, p.f0_proj, sl.f0_proj, r, lb.y, ba, bb, bf0, frames, s)
        st2_branch(enc, c, p.n, sl.nb, p.n_proj, sl.n_proj, r, lb.y, ba, bb, bn, frames, s)
    }
    if (ran) {
        unsafe(scratch_resize(f0, tf))
        unsafe(scratch_resize(n, tn))
        unsafe {
            let pf = reinterpret<float const?>(metal_buffer_contents(bf0))
            let pn = reinterpret<float const?>(metal_buffer_contents(bn))
            for (i in range64(tf)) {
                f0[i] = pf[i * ST2_POST_WIDTH]
                n[i] = pn[i * ST2_POST_WIDTH]
            }
        }
        f2 = tf
        g_tower_encodes++
        g_tower_rows += frames
    }
    st2_ctx_release(c)
    if (!ran) {
        return tw_decline(MetalTowerDecline.gpu_error)
    }
    return true
}

[hot_path, arch(at="../ARCHITECTURE_GPU_TOWER.md#tower-tts-chain"), unused_argument(sc)]   //! the seat's contract carries the scratch; this stage's rows cross as their own arrays
def private metal_styletts2_dur_enc(p : St2Predictor; bert : array<float>; t : int64; s : array<float>; @scratch @exact_size var out : array<float>; var sc : St2Scratch) : bool {
    if (!g_tw_env_tower) {
        return tw_decline(MetalTowerDecline.knob)
    }
    let cc = p.bert_encoder.nout
    let hidden = p.bert_encoder.nin
    let sd = long_length(s)
    let width = cc + sd
    if (t < 1l || cc % 64l != 0l || hidden % 32l != 0l || width % 32l != 0l || empty(p.lstms) || length(p.lns) != length(p.lstms)
        || long_length(bert) < t * hidden) {
        return tw_decline(MetalTowerDecline.shape)
    }
    for (l in p.lstms) {
        return tw_decline(MetalTowerDecline.shape) if (!st2_lstm_ok(l, width) || 2l * l.fwd.hidden != cc)
    }
    if (length(p.lstms) > ST2_MAX_DE_LSTMS || !st2_pred_ok(p)) {
        return tw_decline(MetalTowerDecline.shape)
    }
    if (!metal_tower_init() || !st2_pred_attach(p)) {
        return tw_decline(MetalTowerDecline.device)
    }
    residency_flush()
    var c = St2GpuCtx()
    st2_ctx_make(c, g_tw_st2_pred.buf, 2l * cc * long_length(p.lns), cc, 0ul)
    c.exact = true
    var bbert = st2_rows_buf(c, t, hidden)
    var bden = st2_rows_buf(c, t, cc)
    var bx = st2_rows_buf(c, t, width)
    var bs = st2_rows_buf(c, sd, 1l)
    var lbs : St2LstmBufs [ST2_MAX_DE_LSTMS]
    for (i in range(length(p.lstms))) {
        lbs[i] = st2_lstm_bufs(c, g_tw_st2_pred.de_lstm[i], t, p.lstms[i].fwd.hidden)
    }
    st2_upload(bbert, bert, t * hidden)
    st2_upload(bs, s, sd)
    let sl & = g_tw_st2_pred
    var err = ""
    let ran = with_compute_encoder(g_queue, err) $(enc : MetalComputeEncoder?) {   // nolint:PERF026 — the error-text fetch runs only on the failure leg
        st2_lin(enc, c, sl.bert_enc, bbert, t, bden)
        //! the style appended as a broadcast "residual" of stride 0; the width holds the f0/n columns out
        var kc = St2ConcatArgs(c = uint(cc), rc = uint(sd), rs = 0u, f0s = 0u, ns = 0u, with_res = 1u, width = uint(width), total = uint(t * width))
        enc_st2_concat(enc, bden, bs, bs, bs, bx, kc, t * width)
        for (i in range(length(p.lstms))) {
            let lb & = lbs[i]
            st2_bilstm(enc, c, p.lstms[i], sl.de_lstm[i], bx, t, lb.xf, lb.xb, lb.y)
            let hoff = st2_style_rows(c, p.lns[i].fc, s, cc, true)
            st2_ln(enc, c, lb.y, lb.y, t, cc, ST2_ADAIN_EPS, c.bh, hoff, hoff + uint64(cc * 4l))
            enc_st2_concat(enc, lb.y, bs, bs, bs, bx, kc, t * width)
        }
    }
    if (ran) {
        unsafe(scratch_resize(out, t * width))
        st2_readback(bx, out, t * width)
        g_tower_encodes++
        g_tower_rows += t
    }
    st2_ctx_release(c)
    if (!ran) {
        return tw_decline(MetalTowerDecline.gpu_error)
    }
    return true
}

[hot_path, arch(at="../ARCHITECTURE_GPU_TOWER.md#tower-tts-chain"), unused_argument(sc)]
def private metal_styletts2_durations(p : St2Predictor; d : array<float>; t : int64; speed : float; @scratch @exact_size var raw : array<float>; var sc : St2Scratch) : bool {
    if (!g_tw_env_tower) {
        return tw_decline(MetalTowerDecline.knob)
    }
    let width = p.lstm.fwd.nin
    let hd = p.lstm.fwd.hidden
    if (t < 1l || width % 32l != 0l || p.duration_proj.nin != 2l * hd || p.duration_proj.nout < 1l
        || p.duration_proj.nout > 64l || speed <= 0.0 || !st2_pred_ok(p)) {
        return tw_decline(MetalTowerDecline.shape)
    }
    if (!metal_tower_init() || !st2_pred_attach(p)) {
        return tw_decline(MetalTowerDecline.device)
    }
    residency_flush()
    var c = St2GpuCtx()
    st2_ctx_make(c, g_tw_st2_pred.buf, 0l, 1l, 0ul)
    c.exact = true
    var bd = st2_rows_buf(c, t, width)
    var lb = st2_lstm_bufs(c, g_tw_st2_pred.dur_lstm, t, hd)
    var blog = st2_rows_buf(c, t, g_tw_st2_pred.dur_proj.cout_p)
    var braw = st2_rows_buf(c, t, 1l)
    st2_upload(bd, d, t * width)
    let sl & = g_tw_st2_pred
    var err = ""
    let ran = with_compute_encoder(g_queue, err) $(enc : MetalComputeEncoder?) {   // nolint:PERF026 — the error-text fetch runs only on the failure leg
        st2_bilstm(enc, c, p.lstm, sl.dur_lstm, bd, t, lb.xf, lb.xb, lb.y)
        st2_lin(enc, c, sl.dur_proj, lb.y, t, blog)
        let u_nb = st2_u(c, uint(p.duration_proj.nout))
        let u_ls = st2_u(c, uint(sl.dur_proj.cout_p))
        let u_speed = st2_uf(c, speed)
        let u_t = st2_u(c, uint(t))
        enc_st2_sigsum(enc, blog, braw, u_nb, u_ls, u_speed, u_t, t)
    }
    if (ran) {
        unsafe(scratch_resize(raw, t))
        st2_readback(braw, raw, t)
        g_tower_encodes++
        g_tower_rows += t
    }
    st2_ctx_release(c)
    if (!ran) {
        return tw_decline(MetalTowerDecline.gpu_error)
    }
    return true
}

def private st2_ids_buf(var c : St2GpuCtx; ids : array<int>) : MetalBuffer? {
    let t = long_length(ids)
    var b = st2_rows_buf(c, t, 1l)
    unsafe {
        var p = reinterpret<uint?>(metal_buffer_contents(b))
        for (i in range64(t)) {
            p[i] = uint(ids[i])
        }
    }
    return b
}

[hot_path, arch(at="../ARCHITECTURE_GPU_TOWER.md#tower-tts-chain"), unused_argument(sc)]
def private metal_styletts2_albert(a : St2Albert; ids : array<int>; @scratch @exact_size var out : array<float>; var sc : St2Scratch) : bool {
    if (!g_tw_env_tower) {
        return tw_decline(MetalTowerDecline.knob)
    }
    let t = long_length(ids)
    let d = a.hidden
    let hs = a.heads > 0l ? d / a.heads : 0l
    let ff = a.ffn.nout
    if (t < 1l || t > a.max_pos || d % 64l != 0l || hs % 64l != 0l || hs > 128l || hs * a.heads != d || ff % 64l != 0l || a.emb_dim % 32l != 0l
        || a.map_in.nin != a.emb_dim || a.map_in.nout != d || a.q.nout != d || a.k.nout != d || a.v.nout != d || a.dense.nout != d
        || a.ffn.nin != d || a.ffn_out.nin != ff || a.ffn_out.nout != d || long_length(a.pos_emb.a) < t * a.emb_dim
        || long_length(a.type_emb.a) < a.emb_dim || a.n_layers < 1l) {
        return tw_decline(MetalTowerDecline.shape)
    }
    for (id in ids) {
        return tw_decline(MetalTowerDecline.shape) if (id < 0 || int64(id + 1) * a.emb_dim > long_length(a.word_emb.a))
    }
    if (!st2_albert_ok(a)) {
        return tw_decline(MetalTowerDecline.shape)
    }
    if (!metal_tower_init() || !st2_albert_attach(a)) {
        return tw_decline(MetalTowerDecline.device)
    }
    residency_flush()
    var c = St2GpuCtx()
    st2_ctx_make(c, g_tw_st2_albert.buf, 0l, 1l, 0ul)
    c.exact = true
    var bids = st2_ids_buf(c, ids)
    var be = st2_rows_buf(c, t, a.emb_dim)
    var bx = st2_rows_buf(c, t, d)
    var bxb = st2_rows_buf(c, t, d)
    var bxb2 = st2_rows_buf(c, t, d)
    var bq = st2_rows_buf(c, t, d)
    var bk = st2_rows_buf(c, t, d)
    var bv = st2_rows_buf(c, t, d)
    var bffn = st2_rows_buf(c, t, ff)
    var ka = St2AttnArgs(t = uint(t), d = uint(d), hs = uint(hs), heads = uint(a.heads))
    let sl & = g_tw_st2_albert
    var err = ""
    let ran = with_compute_encoder(g_queue, err) $(enc : MetalComputeEncoder?) {   // nolint:PERF026 — the error-text fetch runs only on the failure leg
        var kg = St2GatherArgs(c = uint(a.emb_dim), add_b = 1u, add_c = 1u, total = uint(t * a.emb_dim))
        enc_st2_gather(enc, c.slab, sl.word_emb, bids, c.slab, sl.pos_emb, c.slab, sl.type_emb, be, kg, t * a.emb_dim)
        st2_ln(enc, c, be, be, t, a.emb_dim, ST2_ALBERT_EPS, c.slab, sl.emb_ln.gwoff, sl.emb_ln.gboff)
        st2_lin(enc, c, sl.map_in, be, t, bx)
        let nd = t * d
        for (_l in range64(a.n_layers)) {
            st2_lin(enc, c, sl.q, bx, t, bq)
            st2_lin(enc, c, sl.k, bx, t, bk)
            st2_lin(enc, c, sl.v, bx, t, bv)
            enc_st2_attn(enc, bq, bk, bv, bxb2, ka, a.heads * t)
            st2_lin(enc, c, sl.dense, bxb2, t, bxb)
            st2_add(enc, c, bxb, bx, bxb, nd, 1.0)
            st2_ln(enc, c, bxb, bxb, t, d, ST2_ALBERT_EPS, c.slab, sl.attn_ln.gwoff, sl.attn_ln.gboff)
            st2_lin(enc, c, sl.ffn, bxb, t, bffn)
            let u_nf = st2_u(c, uint(t * ff))
            enc_st2_gelu_tanh(enc, bffn, u_nf, t * ff)
            st2_lin(enc, c, sl.ffn_out, bffn, t, bxb2)
            st2_add(enc, c, bxb2, bxb, bx, nd, 1.0)
            st2_ln(enc, c, bx, bx, t, d, ST2_ALBERT_EPS, c.slab, sl.full_ln.gwoff, sl.full_ln.gboff)
        }
    }
    if (ran) {
        unsafe(scratch_resize(out, t * d))
        st2_readback(bx, out, t * d)
        g_tower_encodes++
        g_tower_rows += t
        g_tower_blocks += a.n_layers
    }
    st2_ctx_release(c)
    if (!ran) {
        return tw_decline(MetalTowerDecline.gpu_error)
    }
    return true
}

//! The text encoder hook; answers the CPU form's channel-major [c][t] rows.
[hot_path, arch(at="../ARCHITECTURE_GPU_TOWER.md#tower-tts-chain")]
def private metal_styletts2_text(te : St2TextEncoder; ids : array<int>; @scratch @exact_size var out : array<float>; var sc : St2Scratch) : bool {
    if (!g_tw_env_tower) {
        return tw_decline(MetalTowerDecline.knob)
    }
    let t = long_length(ids)
    let cc = te.c
    var sh = St2Shape()
    if (t < 1l || cc % 64l != 0l || length(te.cnn_ln) != length(te.cnn) || !st2_lstm_ok(te.lstm, cc) || 2l * te.lstm.fwd.hidden != cc) {
        return tw_decline(MetalTowerDecline.shape)
    }
    for (cv in te.cnn) {
        return tw_decline(MetalTowerDecline.shape) if (!st2_note_conv(sh, cv) || cv.cin != cc || cv.cout != cc || conv1d_out_len(cv, t) != t || cv.transposed)
    }
    for (id in ids) {
        return tw_decline(MetalTowerDecline.shape) if (id < 0 || int64(id + 1) * cc > long_length(te.emb.a))
    }
    if (!st2_text_ok(te)) {
        return tw_decline(MetalTowerDecline.shape)
    }
    if (!metal_tower_init() || !st2_text_attach(te)) {
        return tw_decline(MetalTowerDecline.device)
    }
    residency_flush()
    var c = St2GpuCtx()
    st2_ctx_make(c, g_tw_st2_text.buf, 0l, 1l, sh.cols_bytes)
    c.exact = true
    var bids = st2_ids_buf(c, ids)
    var ba = st2_rows_buf(c, t, cc)
    var bb = st2_rows_buf(c, t, cc)
    var lb = st2_lstm_bufs(c, g_tw_st2_text.lstm, t, te.lstm.fwd.hidden)
    let sl & = g_tw_st2_text
    var err = ""
    let ran = with_compute_encoder(g_queue, err) $(enc : MetalComputeEncoder?) {   // nolint:PERF026 — the error-text fetch runs only on the failure leg
        var kg = St2GatherArgs(c = uint(cc), add_b = 0u, add_c = 0u, total = uint(t * cc))
        enc_st2_gather(enc, c.slab, sl.emb, bids, c.slab, 0ul, c.slab, 0ul, ba, kg, t * cc)
        for (i in range(length(te.cnn))) {
            let src = i % 2 == 0 ? ba : bb
            let dst = i % 2 == 0 ? bb : ba
            st2_conv(enc, c, sl.cnn[i], te.cnn[i], src, t, dst)
            st2_ln(enc, c, dst, dst, t, cc, ST2_ADAIN_EPS, c.slab, sl.ln[i].gwoff, sl.ln[i].gboff)
            st2_leaky(enc, c, dst, t * cc, 0.2)
        }
        let bfin = length(te.cnn) % 2 == 1 ? bb : ba
        st2_bilstm(enc, c, te.lstm, sl.lstm, bfin, t, lb.xf, lb.xb, lb.y)
    }
    if (ran) {
        var rows & = unsafe(sc.te_a)
        unsafe(scratch_resize(rows, t * cc))
        st2_readback(lb.y, rows, t * cc)
        unsafe(scratch_resize(out, cc * t))
        transpose_tc(rows, t, cc, out)
        g_tower_encodes++
        g_tower_rows += t
    }
    st2_ctx_release(c)
    if (!ran) {
        return tw_decline(MetalTowerDecline.gpu_error)
    }
    return true
}

def private metal_styletts2_driver : St2GpuDriver {
    return St2GpuDriver(albert = @@metal_styletts2_albert, text = @@metal_styletts2_text, dur_enc = @@metal_styletts2_dur_enc,
        durations = @@metal_styletts2_durations, prosody = @@metal_styletts2_prosody, decode = @@metal_styletts2_decode,
        gen = @@metal_styletts2_generator)
}

//! The q3a pad and the StyleTTS2 slabs key on host plane addresses, which a weights-epoch drop makes lie.
def private tw_weights_drop {
    if (g_tw_q3a_wpad != null) {
        metal_release(g_tw_q3a_wpad)
        g_tw_q3a_wpad = null
    }
    g_tw_q3a_key = 0ul
    g_tw_q3a_len = 0l
    st2_slab_drop()
}

[init]
def dasllama_metal_tower_register {
    register_reload_prep(@@tw_weights_drop)
    g_tw_env_tower = g_env_metal.tower
    g_tw_env_tower_f16 = g_env_metal.tower_f16
    if (!g_tw_env_tower_f16) {
        to_log(LOG_INFO, "dasLLAMA metal tower: DASLLAMA_METAL_TOWER_F16=0 - the twin-W route stays off, every tower GEMM rides f32\n")
    }
    g_tw_env_tower_flash = g_env_metal.tower_flash
    if (!g_tw_env_tower_flash) {
        to_log(LOG_INFO, "dasLLAMA metal tower: DASLLAMA_METAL_TOWER_FLASH=0 - the dk72 one-pass attention stays off, the slab trio serves\n")
    }
    register_gemma4uv_gpu(@@metal_gemma4uv_encode)
    register_gemma4v_gpu(@@metal_gemma4v_blocks, @@metal_tower_serves)
    register_gemma3v_gpu(@@metal_gemma3v_blocks, @@metal_tower_serves)
    register_qwen3v_gpu(@@metal_qwen3v_blocks, @@metal_tower_serves)
    register_qwen25v_gpu(@@metal_qwen25v_blocks)
    register_tower_blocks_gpu(@@metal_tower_blocks, @@metal_tower_serves)
    register_tower_conv_gpu(@@metal_conv_frontend)
    register_qwen3a_front_gpu(@@metal_q3a_front)
    register_qwen3a_mel_gpu(@@metal_q3a_mel)
    register_gemma4a_gpu(@@metal_gemma4a_blocks, @@metal_tower_serves)
    register_gemma4a_chunk_gpu(@@metal_gemma4a_chunk)
    register_canary_gpu(@@metal_canary_blocks, @@metal_tower_serves)
    register_parakeet_gpu(@@metal_parakeet_blocks, @@metal_tower_serves)
    register_styletts2_gpu(metal_styletts2_driver())
}
