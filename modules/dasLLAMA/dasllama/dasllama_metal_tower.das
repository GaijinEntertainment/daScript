options gen2
options indenting = 4
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_metal_tower shared public

require dasllama/dasllama_lint public
require math
require dasllama/dasllama_env
require dasllama/dasllama_common
require dasllama/dasllama_metal_shapes
require dasllama/dasllama_gemma4uv
require dasllama/dasllama_gemma4v   // Gemma4vTower/Gemma4vState — the ViT block loop
require dasllama/dasllama_gemma3v   // Gemma3vTower/Gemma3vState — the SigLIP block loop
require dasllama/dasllama_audio   // AudioTower/EncoderState — the whisper-class block loop
require dasllama/dasllama_qwen3a   // Qwen3aTower — the conv2d frontend hook
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
var private g_tw_ones : MetalBuffer?      // [TW_ONES_DIM] ones — the weightless-rms weight bind
let private TW_ONES_DIM = 8192

//! Engage/decline counters (the counts live in dasllama_metal_common, the counting home);
//! `blocks` = transformer blocks run (n_layer per whisper-class encode; 0 for gemma4uv).
//! Model-level gates assert DELTAS on these.
def metal_tower_stats : tuple<encodes : int64; rows : int64; blocks : int64; convs : int64; declines : int64> {
    var dtot = 0l
    for (v in values(g_tower_declines_by_reason)) {
        dtot += v
    }
    return (encodes = g_tower_encodes, rows = g_tower_rows, blocks = g_tower_blocks,
        convs = g_tower_convs, declines = dtot)
}

//! The declines-by-reason table (a copy), keyed by `MetalTowerDecline` value names.
def metal_tower_declines : table<string; int64> {
    return <- clone_to_move(g_tower_declines_by_reason)
}

//! Test A/B seat: pin the tower driver on/off in-process (the env knob's setter twin).
def set_metal_tower(on : bool) {
    g_tw_env_tower = on
}

[cold_path]   // the decline leg IS the fall-off-the-GPU path; its bookkeeping is not hot
def private tw_decline(why : MetalTowerDecline) : bool {
    note_tower_decline("{why}")
    if (why != MetalTowerDecline.knob) {   // knob = policy; capability declines honor required mode
        require_or_panic("tower", "{why}")
    }
    return false
}

//! Release the tower's own GPU objects (the ones buffer + the scratch pool) — the tests'
//! leak-gate rail, beside metal_decode_shutdown. The borrowed and registry PSOs are owned by
//! their homes; lazily re-initializes on the next served encode.
def metal_tower_shutdown {
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
def private metal_gemma4uv_encode(t : Gemma4uvEmbedder; var s : Gemma4uvState; npos, n_cols : int64; var out : array<float>) : bool {   // nolint:STYLE038 — the dispatch chain is the shape
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
    let mp = (npos + 31l) / 32l * 32l
    let mapped = t.image_map != null
    var bblob = plane_buffer(g_dev, unsafe(reinterpret<void?>(plane_at(t.blob, 0l))), uint64(length(t.blob) * 4l), mapped)
    var bx0 = pool_acquire(g_tw_pool, g_dev, uint64(mp * t.patch_dim * 4l))
    var bx1 = pool_acquire(g_tw_pool, g_dev, uint64(mp * d * 4l))
    var bout = pool_acquire(g_tw_pool, g_dev, uint64(mp * t.proj_dim * 4l))
    unsafe {
        var p = reinterpret<uint8?>(metal_buffer_contents(bx0))
        memcpy(p, addr(s.x0[0]), npos * t.patch_dim * 4l)
        if (mp > npos) {   // DEFENSIVE pad zero (controls stay green without it): guards stale-Inf pool reuse
            var zp = reinterpret<float?>(p + npos * t.patch_dim * 4l)
            for (i in range64((mp - npos) * t.patch_dim)) {
                zp[i] = 0.0
            }
        }
    }
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
            pf_enc_bf16_mm(enc, bwblob, uint64(t.patch_w * 2l), bx0, bx1, u_pdim, u_d, mp, d)
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
            pf_enc_bf16_mm(enc, bwblob2, uint64(t.proj_w * 2l), bx1, bout, u_d, u_nproj, mp, t.proj_dim)
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
[hot_path]
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
    let mp = (npos + 31l) / 32l * 32l
    let nk64 = (npos + 63l) / 64l * 64l
    let ff = t.n_ff
    let mapped = t.image_map != null
    var bblob = plane_buffer(g_dev, unsafe(reinterpret<void?>(plane_at(t.fblob, 0l))), uint64(length(t.fblob) * 4l), mapped)
    let bytes_row = uint64(mp * d * 4l)
    let bytes_h = uint64(mp * ff * 4l)
    let bytes_att = uint64(t.n_head * mp * nk64 * 4l)
    var bx = pool_acquire(g_tw_pool, g_dev, bytes_row)
    var bxb = pool_acquire(g_tw_pool, g_dev, bytes_row)
    var bxb2 = pool_acquire(g_tw_pool, g_dev, bytes_row)
    var bq = pool_acquire(g_tw_pool, g_dev, bytes_row)
    var bk = pool_acquire(g_tw_pool, g_dev, bytes_row)
    var bv = pool_acquire(g_tw_pool, g_dev, bytes_row)
    var bh = pool_acquire(g_tw_pool, g_dev, bytes_h)
    var batt = pool_acquire(g_tw_pool, g_dev, bytes_att)
    unsafe {
        var p = reinterpret<uint8?>(metal_buffer_contents(bx))
        memcpy(p, addr(s.x[0]), npos * d * 4l)
        if (mp > npos) {   // DEFENSIVE pad zero (controls stay green without it): guards stale-Inf pool reuse
            var zp = reinterpret<float?>(p + npos * d * 4l)
            for (i in range64((mp - npos) * d)) {
                zp[i] = 0.0
            }
        }
    }
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
            enc_f32_mm(enc, bblob, uint64(lo.q_w * 4l), bxb, bq, 0ul, u_d, u_d, mp, d)
            enc_add_bias_rows(enc, bq, 0ul, bblob, uint64(lo.q_b * 4l), u_d, u_totd, npos * d)
            enc_f32_mm(enc, bblob, uint64(lo.k_w * 4l), bxb, bk, 0ul, u_d, u_d, mp, d)
            if (t.has_k_bias) {
                enc_add_bias_rows(enc, bk, 0ul, bblob, uint64(lo.k_b * 4l), u_d, u_totd, npos * d)
            }
            enc_f32_mm(enc, bblob, uint64(lo.v_w * 4l), bxb, bv, 0ul, u_d, u_d, mp, d)
            enc_add_bias_rows(enc, bv, 0ul, bblob, uint64(lo.v_b * 4l), u_d, u_totd, npos * d)
            enc_qk_mm(enc, bq, bk, batt, ka, mp, nk64, t.n_head)
            enc_softmax(enc, batt, ka, null, npos, t.n_head)
            enc_av_mm(enc, batt, bv, bxb2, ka, mp, t.n_head, hs)
            enc_f32_mm(enc, bblob, uint64(lo.o_w * 4l), bxb2, bxb, 0ul, u_d, u_d, mp, d)
            enc_add_bias_rows(enc, bxb, 0ul, bblob, uint64(lo.o_b * 4l), u_d, u_totd, npos * d)
            enc_add(enc, bx, 0ul, bxb, 0ul, u_totd, npos * d)
            enc_ln(enc, bx, bblob, uint64(lo.ln2_w * 4l), bblob, uint64(lo.ln2_b * 4l), bxb, u_d, u_eps, npos)
            enc_f32_mm(enc, bblob, uint64(lo.fc1_w * 4l), bxb, bh, 0ul, u_d, u_ff, mp, ff)
            enc_add_bias_rows(enc, bh, 0ul, bblob, uint64(lo.fc1_b * 4l), u_ff, u_totff, npos * ff)
            if (t.gelu_tanh) {
                enc_gelu_lut(enc, bh, 0ul, u_totff, npos * ff)
            } else {
                enc_gelu_erf(enc, bh, 0ul, u_totff, npos * ff)
            }
            enc_f32_mm(enc, bblob, uint64(lo.fc2_w * 4l), bh, bxb2, 0ul, u_ff, u_d, mp, d)
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
    let mp = (npos + 31l) / 32l * 32l
    let nk64 = (npos + 63l) / 64l * 64l
    let pairs = hs / 4l   // rotated pairs per rope axis = one packed table row
    let mapped = t.image_map != null
    var bblob = plane_buffer(g_dev, unsafe(reinterpret<void?>(plane_at(t.blob, 0l))), uint64(length(t.blob) * 4l), mapped)
    var bwblob = plane_buffer(g_dev, unsafe(reinterpret<void?>(plane_at(t.wblob, 0l))), uint64(length(t.wblob) * 2l), mapped)
    let bytes_row = uint64(mp * d * 4l)
    let bytes_h = uint64(mp * ff * 4l)
    let bytes_att = uint64(t.n_head * mp * nk64 * 4l)
    let bytes_tabs = uint64(npos * 4l * pairs * 4l)
    var bx = pool_acquire(g_tw_pool, g_dev, bytes_row)
    var bxb = pool_acquire(g_tw_pool, g_dev, bytes_row)
    var bxb2 = pool_acquire(g_tw_pool, g_dev, bytes_row)
    var bq = pool_acquire(g_tw_pool, g_dev, bytes_row)
    var bk = pool_acquire(g_tw_pool, g_dev, bytes_row)
    var bv = pool_acquire(g_tw_pool, g_dev, bytes_row)
    var bhg = pool_acquire(g_tw_pool, g_dev, bytes_h)
    var bhu = pool_acquire(g_tw_pool, g_dev, bytes_h)
    var batt = pool_acquire(g_tw_pool, g_dev, bytes_att)
    var btabs = pool_acquire(g_tw_pool, g_dev, bytes_tabs)
    unsafe {
        var p = reinterpret<uint8?>(metal_buffer_contents(bx))
        memcpy(p, addr(s.x[0]), npos * d * 4l)
        if (mp > npos) {   // DEFENSIVE pad zero (controls stay green without it): guards stale-Inf pool reuse
            var zp = reinterpret<float?>(p + npos * d * 4l)
            for (i in range64((mp - npos) * d)) {
                zp[i] = 0.0
            }
        }
        // the rope tables, packed per row as [cx q | sx q | cy q | sy q] — the CPU's bits
        var tp = reinterpret<float?>(metal_buffer_contents(btabs))
        for (r in range64(npos)) {
            let rb = r * 4l * pairs
            for (j in range64(pairs)) {
                tp[rb + j] = s.cos_x[r * pairs + j]
                tp[rb + pairs + j] = s.sin_x[r * pairs + j]
                tp[rb + 2l * pairs + j] = s.cos_y[r * pairs + j]
                tp[rb + 3l * pairs + j] = s.sin_y[r * pairs + j]
            }
        }
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
            pf_enc_bf16_mm(enc, bwblob, uint64(o.w[GEMMA4V_GEMM_Q] * 2l), bxb2, bq, u_d, u_d, mp, d)
            enc_clamp(enc, bq, 0ul, bq, 0ul, bblob, cb_q, u_out, u_totd, npos * d)
            enc_clamp(enc, bxb, 0ul, bxb2, 0ul, bblob, cb_k, u_in, u_totd, npos * d)
            pf_enc_bf16_mm(enc, bwblob, uint64(o.w[GEMMA4V_GEMM_K] * 2l), bxb2, bk, u_d, u_d, mp, d)
            enc_clamp(enc, bk, 0ul, bk, 0ul, bblob, cb_k, u_out, u_totd, npos * d)
            enc_clamp(enc, bxb, 0ul, bxb2, 0ul, bblob, cb_v, u_in, u_totd, npos * d)
            pf_enc_bf16_mm(enc, bwblob, uint64(o.w[GEMMA4V_GEMM_V] * 2l), bxb2, bv, u_d, u_d, mp, d)
            enc_clamp(enc, bv, 0ul, bv, 0ul, bblob, cb_v, u_out, u_totd, npos * d)
            pf_enc_rms(enc, bq, 0ul, bw_qn, bq, u_hs, u_eps, npos * t.n_head)
            pf_enc_rms(enc, bk, 0ul, bw_kn, bk, u_hs, u_eps, npos * t.n_head)
            pf_enc_rms(enc, bv, 0ul, g_tw_ones, bv, u_hs, u_eps, npos * t.n_head)
            enc_rope2d(enc, bq, btabs, u_d, u_pairs, u_totrope, npos * t.n_head * pairs)
            enc_rope2d(enc, bk, btabs, u_d, u_pairs, u_totrope, npos * t.n_head * pairs)
            enc_qk_mm(enc, bq, bk, batt, ka, mp, nk64, t.n_head)
            enc_softmax(enc, batt, ka, null, npos, t.n_head)
            enc_av_mm(enc, batt, bv, bxb2, ka, mp, t.n_head, hs)
            // o-proj (clamped both sides) → post-norm in place → residual
            enc_clamp(enc, bxb2, 0ul, bxb2, 0ul, bblob, cb_o, u_in, u_totd, npos * d)
            pf_enc_bf16_mm(enc, bwblob, uint64(o.w[GEMMA4V_GEMM_O] * 2l), bxb2, bxb, u_d, u_d, mp, d)
            enc_clamp(enc, bxb, 0ul, bxb, 0ul, bblob, cb_o, u_out, u_totd, npos * d)
            pf_enc_rms(enc, bxb, 0ul, bw_ap, bxb, u_d, u_eps, npos)
            enc_add(enc, bx, 0ul, bxb, 0ul, u_totd, npos * d)
            // ffn: rms → gate / up (each its own input clamp) → GEGLU-quick → down → post-norm → residual
            pf_enc_rms(enc, bx, 0ul, bw_ln2, bxb, u_d, u_eps, npos)
            enc_clamp(enc, bxb, 0ul, bxb2, 0ul, bblob, cb_gate, u_in, u_totd, npos * d)
            pf_enc_bf16_mm(enc, bwblob, uint64(o.w[GEMMA4V_GEMM_GATE] * 2l), bxb2, bhg, u_d, u_ff, mp, ff)
            enc_clamp(enc, bhg, 0ul, bhg, 0ul, bblob, cb_gate, u_out, u_totff, npos * ff)
            enc_clamp(enc, bxb, 0ul, bxb2, 0ul, bblob, cb_up, u_in, u_totd, npos * d)
            pf_enc_bf16_mm(enc, bwblob, uint64(o.w[GEMMA4V_GEMM_UP] * 2l), bxb2, bhu, u_d, u_ff, mp, ff)
            enc_clamp(enc, bhu, 0ul, bhu, 0ul, bblob, cb_up, u_out, u_totff, npos * ff)
            enc_geglu_quick(enc, bhg, 0ul, bhu, 0ul, u_totff, npos * ff)
            enc_clamp(enc, bhg, 0ul, bhg, 0ul, bblob, cb_down, u_in, u_totff, npos * ff)
            pf_enc_bf16_mm(enc, bwblob, uint64(o.w[GEMMA4V_GEMM_DOWN] * 2l), bhg, bxb, u_ff, u_d, mp, d)
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
[hot_path]
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
    let hs_pad = hs <= 64l ? 64l : 128l
    if (d % 64l != 0l || ff % 64l != 0l || hs > 128l) {
        return tw_decline(MetalTowerDecline.shape)
    }
    if (!metal_tower_init()) {   // per-encode: re-arms after any shutdown seat
        return tw_decline(MetalTowerDecline.device)
    }
    let mp = (npos + 31l) / 32l * 32l
    let nk64 = (npos + 63l) / 64l * 64l
    let dp = t.n_head * hs_pad   // the padded attention width
    let mapped = t.image_map != null
    var bblob = plane_buffer(g_dev, unsafe(reinterpret<void?>(plane_at(t.blob, 0l))), uint64(length(t.blob) * 4l), mapped)
    let bytes_row = uint64(mp * d * 4l)
    let bytes_rowp = uint64(mp * dp * 4l)
    let bytes_h = uint64(mp * ff * 4l)
    let bytes_att = uint64(t.n_head * mp * nk64 * 4l)
    var bx = pool_acquire(g_tw_pool, g_dev, bytes_row)
    var bxb = pool_acquire(g_tw_pool, g_dev, bytes_row)
    var bxb2 = pool_acquire(g_tw_pool, g_dev, bytes_row)
    var braw = pool_acquire(g_tw_pool, g_dev, bytes_row)   // GEMM output ahead of the head restride
    var bq = pool_acquire(g_tw_pool, g_dev, bytes_rowp)
    var bk = pool_acquire(g_tw_pool, g_dev, bytes_rowp)
    var bv = pool_acquire(g_tw_pool, g_dev, bytes_rowp)
    var bo_p = pool_acquire(g_tw_pool, g_dev, bytes_rowp)   // padded AV output ahead of the compact
    var bh = pool_acquire(g_tw_pool, g_dev, bytes_h)
    var batt = pool_acquire(g_tw_pool, g_dev, bytes_att)
    unsafe {
        var p = reinterpret<uint8?>(metal_buffer_contents(bx))
        memcpy(p, addr(s.x[0]), npos * d * 4l)
        if (mp > npos) {   // DEFENSIVE pad zero (controls stay green without it): guards stale-Inf pool reuse
            var zp = reinterpret<float?>(p + npos * d * 4l)
            for (i in range64((mp - npos) * d)) {
                zp[i] = 0.0
            }
        }
    }
    var u_d = uniform_u32(uint(d))
    var u_ff = uniform_u32(uint(ff))
    var u_hs = uniform_u32(uint(hs))
    var u_hsp = uniform_u32(uint(hs_pad))
    var u_eps = uniform_f32(t.eps)
    var u_totd = uniform_u32(uint(npos * d))
    var u_totff = uniform_u32(uint(npos * ff))
    var u_totp = uniform_u32(uint(npos * t.n_head * hs_pad))   // restride total, pad direction
    var u_totc = uniform_u32(uint(npos * t.n_head * hs))       // restride total, compact direction
    var ka = AttnArgs(qd = uint(dp), kv_dim = uint(dp), head_size = uint(hs_pad), kv_mul = 1u,
        npos = uint(npos), np32 = uint(nk64), scale = 1.0 / sqrt(float(hs)),
        qoff = 0u, qrows = uint(mp), window = 0u, softcap = 0.0, hass = 0u, uend = uint(npos))
    var err = ""
    let ran = with_compute_encoder(g_queue, err) $(enc : MetalComputeEncoder?) {   // nolint:PERF026 — the error-text fetch runs only on the failure leg
        for (l in range64(t.n_layer)) {
            let o = gemma3v_block_offs(t, l)
            enc_ln(enc, bx, bblob, uint64(o.ln1_w * 4l), bblob, uint64(o.ln1_b * 4l), bxb, u_d, u_eps, npos)
            enc_f32_mm(enc, bblob, uint64(o.w[GEMMA3V_GEMM_Q] * 4l), bxb, braw, 0ul, u_d, u_d, mp, d)
            enc_add_bias_rows(enc, braw, 0ul, bblob, uint64(o.b[GEMMA3V_GEMM_Q] * 4l), u_d, u_totd, npos * d)
            enc_head_restride(enc, braw, bq, u_hs, u_hsp, u_totp, npos * t.n_head * hs_pad)
            enc_f32_mm(enc, bblob, uint64(o.w[GEMMA3V_GEMM_K] * 4l), bxb, braw, 0ul, u_d, u_d, mp, d)
            enc_add_bias_rows(enc, braw, 0ul, bblob, uint64(o.b[GEMMA3V_GEMM_K] * 4l), u_d, u_totd, npos * d)
            enc_head_restride(enc, braw, bk, u_hs, u_hsp, u_totp, npos * t.n_head * hs_pad)
            enc_f32_mm(enc, bblob, uint64(o.w[GEMMA3V_GEMM_V] * 4l), bxb, braw, 0ul, u_d, u_d, mp, d)
            enc_add_bias_rows(enc, braw, 0ul, bblob, uint64(o.b[GEMMA3V_GEMM_V] * 4l), u_d, u_totd, npos * d)
            enc_head_restride(enc, braw, bv, u_hs, u_hsp, u_totp, npos * t.n_head * hs_pad)
            enc_qk_mm(enc, bq, bk, batt, ka, mp, nk64, t.n_head)
            enc_softmax(enc, batt, ka, null, npos, t.n_head)
            enc_av_mm(enc, batt, bv, bo_p, ka, mp, t.n_head, hs_pad)
            enc_head_restride(enc, bo_p, bxb2, u_hsp, u_hs, u_totc, npos * t.n_head * hs)
            enc_f32_mm(enc, bblob, uint64(o.w[GEMMA3V_GEMM_O] * 4l), bxb2, bxb, 0ul, u_d, u_d, mp, d)
            enc_add_bias_rows(enc, bxb, 0ul, bblob, uint64(o.b[GEMMA3V_GEMM_O] * 4l), u_d, u_totd, npos * d)
            enc_add(enc, bx, 0ul, bxb, 0ul, u_totd, npos * d)
            enc_ln(enc, bx, bblob, uint64(o.ln2_w * 4l), bblob, uint64(o.ln2_b * 4l), bxb, u_d, u_eps, npos)
            enc_f32_mm(enc, bblob, uint64(o.w[GEMMA3V_GEMM_UP] * 4l), bxb, bh, 0ul, u_d, u_ff, mp, ff)
            enc_add_bias_rows(enc, bh, 0ul, bblob, uint64(o.b[GEMMA3V_GEMM_UP] * 4l), u_ff, u_totff, npos * ff)
            enc_gelu_lut(enc, bh, 0ul, u_totff, npos * ff)
            enc_f32_mm(enc, bblob, uint64(o.w[GEMMA3V_GEMM_DOWN] * 4l), bh, bxb2, 0ul, u_ff, u_d, mp, d)
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
    var bmel = pool_acquire(g_tw_pool, g_dev, bytes_mel)
    var bx1 = pool_acquire(g_tw_pool, g_dev, bytes_x1)
    var by1 = pool_acquire(g_tw_pool, g_dev, bytes_y1)
    var bx2 = pool_acquire(g_tw_pool, g_dev, bytes_x2)
    var bxo = pool_acquire(g_tw_pool, g_dev, bytes_xo)
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

// The qwen3a conv2d frontend for one WINDOW: every chunk's three conv stages in one command
// buffer (the conv planes are f32 regardless of the q8 tower, so this serves the q8 SERVING
// DEFAULT too); the shuffle/conv_out tail stays CPU. BEST-EFFORT: declines are a silent false.
[hot_path]
def private metal_q3a_convs(t : Qwen3aTower; win : MelChunk; n_chunks : int64; var y3all : array<float>) : bool {   // nolint:STYLE038 — the dispatch chain is the shape
    let nm = t.core.n_mel
    let nch = t.n_ch
    let ncp = (nch + 63l) & ~63l
    let k2 = 9l * nch
    // the k2%32 shape gate runs first so a forever-declining tower never mints the padded slab
    if (k2 % 32l != 0l || !g_tw_env_tower || !metal_tower_init() || !q3a_wpad_attach(t)) {
        return false
    }
    let xin_n = QWEN3A_CHUNK * nm
    let y3_n = 16l * QWEN3A_TOK_PER_CHUNK * nch
    let y3_rows = 16l * QWEN3A_TOK_PER_CHUNK   // 208 per chunk
    let mp3 = (y3_rows + 31l) / 32l * 32l      // the stage-3 GEMM pads to 224 rows
    y3all |> reserve_resize(n_chunks * y3_n)   // nolint:PERF026 — the caller's @scratch y3all; grows once, reused across windows
    let o2 = ncp * 32l
    let o3 = o2 + ncp * k2
    let o4 = o3 + ncp * k2
    let bytes_xin = uint64(n_chunks * xin_n * 4l)
    let bytes_col = uint64(max(3200l * 32l, 800l * k2) * 4l)
    let bytes_y1 = uint64(3200l * ncp * 4l)
    let bytes_y2 = uint64(800l * ncp * 4l)
    let bytes_y3 = uint64(n_chunks * mp3 * ncp * 4l)
    var bxin = pool_acquire(g_tw_pool, g_dev, bytes_xin)
    var bcol = pool_acquire(g_tw_pool, g_dev, bytes_col)
    var by1 = pool_acquire(g_tw_pool, g_dev, bytes_y1)
    var by2 = pool_acquire(g_tw_pool, g_dev, bytes_y2)
    var by3 = pool_acquire(g_tw_pool, g_dev, bytes_y3)
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
    }
    var served = false
    if (ran) {
        unsafe {   // strided readback: the GPU rows carry ncp floats, y3all wants the nch slice
            let src = reinterpret<float const?>(metal_buffer_contents(by3))
            var dst = addr(y3all[0])
            for (ci in range64(n_chunks)) {
                for (r in range64(y3_rows)) {
                    memcpy(dst + (ci * y3_rows + r) * nch, src + (ci * mp3 + r) * ncp, nch * 4l)
                }
            }
        }
        g_tower_convs += n_chunks
        served = true
    }
    pool_release(g_tw_pool, bxin, bytes_xin)
    pool_release(g_tw_pool, bcol, bytes_col)
    pool_release(g_tw_pool, by1, bytes_y1)
    pool_release(g_tw_pool, by2, bytes_y2)
    pool_release(g_tw_pool, by3, bytes_y3)
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
    return served
}

[init]
def dasllama_metal_tower_register {
    g_tw_env_tower = g_env_metal.tower
    register_gemma4uv_gpu(@@metal_gemma4uv_encode)
    register_gemma4v_gpu(@@metal_gemma4v_blocks, @@metal_tower_serves)
    register_gemma3v_gpu(@@metal_gemma3v_blocks, @@metal_tower_serves)
    register_tower_blocks_gpu(@@metal_tower_blocks)
    register_tower_conv_gpu(@@metal_conv_frontend)
    register_qwen3a_conv_gpu(@@metal_q3a_convs)
}
