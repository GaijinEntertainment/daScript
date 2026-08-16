options gen2
options indenting = 4
options _comment_hygiene = true

module dasllama_metal_tower shared public

require math
require dasllama/dasllama_env
require dasllama/dasllama_common
require dasllama/dasllama_metal_shapes
require dasllama/dasllama_gemma4uv
require dasllama/dasllama_audio   // AudioTower/EncoderState — the whisper-class block loop
require metal/das_metal_boost
require dasllama/dasllama_metal_common
require dasllama/dasllama_metal_kernels
require dasllama/dasllama_metal_prefill   // pf_enc_bf16_mm + the attention trio — the crowned builders

// The tower/embedder Metal driver (metal-media chunk 2): one-shot feed-forward encodes — no
// session, no KV, no mirror. Slice I serves the gemma4uv vision embedder; the whisper-class
// audio towers join in slice J. Contract mirrors the prefill driver: decline-and-return-false,
// the CPU chain is always the correct fallback, engage evidence is COUNTER DELTAS
// (metal_tower_stats), never "the model ran".

var private g_tw_failed = false
var private g_tw_ready = false
var private g_tw_pool : MetalBufferPool
var private g_tw_env_tower = true
var private g_tw_ones : MetalBuffer?      // [TW_ONES_DIM] ones — the weightless-rms weight bind
let private TW_ONES_DIM = 8192

var private g_tw_encodes = 0l
var private g_tw_rows = 0l
var private g_tw_blocks = 0l
var private g_tw_declines : table<string; int64>

//! Engage/decline counters: `encodes` = GPU tower encodes served, `rows` = rows produced,
//! `blocks` = transformer blocks run (n_layer per whisper-class encode; 0 for gemma4uv),
//! `declines` = total declined calls. Model-level gates assert DELTAS on these.
def metal_tower_stats : tuple<encodes : int64; rows : int64; blocks : int64; declines : int64> {
    var dtot = 0l
    for (v in values(g_tw_declines)) {
        dtot += v
    }
    return (encodes = g_tw_encodes, rows = g_tw_rows, blocks = g_tw_blocks, declines = dtot)
}

//! The declines-by-reason table (a copy), keyed by `MetalTowerDecline` value names.
def metal_tower_declines : table<string; int64> {
    return <- clone_to_move(g_tw_declines)
}

//! Test A/B seat: pin the tower driver on/off in-process (the env knob's setter twin).
def set_metal_tower(on : bool) {
    g_tw_env_tower = on
}

def private tw_decline(why : MetalTowerDecline) : bool {
    let key = "{why}"
    g_tw_declines[key] = (g_tw_declines?[key] ?? 0l) + 1l
    return false
}

def private metal_tower_init : bool {
    if (g_tw_ready && g_pso_ln != null) {   // the PSO check re-arms after metal_kernels_release
        return true
    }
    if (g_tw_failed || !metal_common_init()) {
        return false
    }
    var ok = true
    g_pso_ln = compile_pso(metal_layernorm_msl, metal_layernorm_msl_entry, metal_layernorm_msl_fastmath, ok)
    g_pso_f32mm = compile_pso(metal_f32_mulmm_msl, metal_f32_mulmm_msl_entry, metal_f32_mulmm_msl_fastmath, ok)
    g_pso_gelu_erf = compile_pso(MetalGeluErf_metal_ew1_msl, MetalGeluErf_metal_ew1_msl_entry, MetalGeluErf_metal_ew1_msl_fastmath, ok)
    g_pso_gelu_lut = compile_pso(MetalGeluLut_metal_ew1_msl, MetalGeluLut_metal_ew1_msl_entry, MetalGeluLut_metal_ew1_msl_fastmath, ok)
    g_pso_posadd = compile_pso(metal_posadd2d_msl, metal_posadd2d_msl_entry, metal_posadd2d_msl_fastmath, ok)
    if (!ok) {
        g_tw_failed = true
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
    g_tw_ready = true
    return true
}

// The gemma4uv chain on the driver: rows arrive as the CPU im2col output (s.x0), pad rows of
// the GPU buffers stay zero so the GEMM pins their outputs at 0; LN/posadd grids cover the
// LOGICAL rows only. One command buffer, one wait, one readback.
def private metal_gemma4uv_encode(t : Gemma4uvEmbedder; var s : Gemma4uvState; npos, n_cols : int64; var out : array<float>) : bool {   // nolint:STYLE038 — the dispatch chain is the shape
    if (!g_tw_env_tower) {
        return tw_decline(MetalTowerDecline.knob)
    }
    let d = t.n_embd
    if (d % 64l != 0l || t.proj_dim % 64l != 0l || t.patch_dim % 32l != 0l || d % 32l != 0l
            || d > int64(TW_ONES_DIM) || t.patch_w % 4l != 0l || t.proj_w % 8l != 0l || t.pos_w % 4l != 0l) {
        return tw_decline(MetalTowerDecline.shape)
    }
    // per-encode so the borrowed prefill PSOs re-arm after a metal_prefill_shutdown
    if (!metal_tower_init() || !metal_prefill_pso_init()) {
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
        if (mp > npos) {   // zero the pad rows: their GEMM outputs then pin at 0
            var zp = reinterpret<float?>(p + npos * t.patch_dim * 4l)
            for (i in range64((mp - npos) * t.patch_dim)) {
                zp[i] = 0.0
            }
        }
    }
    var u_pdim = uniform_u32(uint(t.patch_dim))
    var u_d = uniform_u32(uint(d))
    var u_eps = uniform_f32(GEMMA4UV_LN_EPS)
    var u_rms_eps = uniform_f32(t.rms_eps)
    var u_kpatch = uniform_u32(uint(t.patch_dim))
    var u_nd = uniform_u32(uint(d))
    var u_kproj = uniform_u32(uint(d))
    var u_nproj = uniform_u32(uint(t.proj_dim))
    var u_ncols = uniform_u32(uint(n_cols))
    var u_pe = uniform_u32(uint(t.pos_entries))
    var u_tot = uniform_u32(uint(npos * d))
    var err = ""
    let ran = with_compute_encoder(g_queue, err) $(enc : MetalComputeEncoder?) {
        // ln1 over the im2col rows, in place
        enc_ln(enc, bx0, bblob, uint64(t.ln1_w * 4l), bblob, uint64(t.ln1_b * 4l), bx0, u_pdim, u_eps, npos)
        // the patch GEMM: per-plane format (gemma-4's shipped mmproj is MIXED)
        if (t.patch_bf16) {
            var bwblob = plane_buffer(g_dev, unsafe(reinterpret<void?>(plane_at(t.wblob, 0l))), uint64(length(t.wblob) * 2l), mapped)
            pf_enc_bf16_mm(enc, bwblob, uint64(t.patch_w * 2l), bx0, bx1, u_kpatch, u_nd, mp, d)
        } else {
            enc_f32_mm(enc, bblob, uint64(t.patch_w * 4l), bx0, bx1, 0ul, u_kpatch, u_nd, mp, d)
        }
        enc_add_bias_rows(enc, bx1, 0ul, bblob, uint64(t.patch_b * 4l), u_d, u_tot, npos * d)
        enc_ln(enc, bx1, bblob, uint64(t.ln2_w * 4l), bblob, uint64(t.ln2_b * 4l), bx1, u_d, u_eps, npos)
        enc_posadd2d(enc, bx1, bblob, uint64(t.pos_w * 4l), u_d, u_ncols, u_pe, u_tot, npos * d)
        enc_ln(enc, bx1, bblob, uint64(t.ln3_w * 4l), bblob, uint64(t.ln3_b * 4l), bx1, u_d, u_eps, npos)
        pf_enc_rms(enc, bx1, 0ul, g_tw_ones, bx1, u_d, u_rms_eps, npos)
        if (t.proj_bf16) {
            var bwblob2 = plane_buffer(g_dev, unsafe(reinterpret<void?>(plane_at(t.wblob, 0l))), uint64(length(t.wblob) * 2l), mapped)
            pf_enc_bf16_mm(enc, bwblob2, uint64(t.proj_w * 2l), bx1, bout, u_kproj, u_nproj, mp, t.proj_dim)
        } else {
            enc_f32_mm(enc, bblob, uint64(t.proj_w * 4l), bx1, bout, 0ul, u_kproj, u_nproj, mp, t.proj_dim)
        }
    }
    var served = false
    if (ran) {
        unsafe {
            memcpy(addr(out[0]), metal_buffer_contents(bout), uint64(npos * t.proj_dim * 4l))
        }
        g_tw_encodes++
        g_tw_rows += npos
        served = true
    }
    pool_release(g_tw_pool, bx0, uint64(mp * t.patch_dim * 4l))
    pool_release(g_tw_pool, bx1, uint64(mp * d * 4l))
    pool_release(g_tw_pool, bout, uint64(mp * t.proj_dim * 4l))
    pool_release(g_upool, u_pdim, 4ul)
    pool_release(g_upool, u_d, 4ul)
    pool_release(g_upool, u_eps, 4ul)
    pool_release(g_upool, u_rms_eps, 4ul)
    pool_release(g_upool, u_kpatch, 4ul)
    pool_release(g_upool, u_nd, 4ul)
    pool_release(g_upool, u_kproj, 4ul)
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
// chunk-1 non-causal span). Frontend and tail stay CPU; f32 towers only. Pad-row garbage is
// CONTAINED: QK/AV zero k/v rows >= npos, softmax zeroes [uend, np32), pad outputs go unread.
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
    // per-encode so the borrowed prefill PSOs re-arm after a metal_prefill_shutdown
    if (!metal_tower_init() || !metal_prefill_pso_init()) {
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
        if (mp > npos) {   // zero the residual pads: every pad product downstream then starts finite
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
    let ran = with_compute_encoder(g_queue, err) $(enc : MetalComputeEncoder?) {
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
        g_tw_encodes++
        g_tw_rows += npos
        g_tw_blocks += t.n_layer
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

[init]
def dasllama_metal_tower_register {
    g_tw_env_tower = g_env_metal.tower
    register_gemma4uv_gpu(@@metal_gemma4uv_encode)
    register_tower_blocks_gpu(@@metal_tower_blocks)
}
