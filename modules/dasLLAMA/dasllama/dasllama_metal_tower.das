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
require dasllama/dasllama_pocket
require dasllama/dasllama_convert
require dasllama/dasllama_kqformat
require dasllama/dasllama_gemm_schema   // Q8_BLOCK_ELEMS, Q8_QPB, Q8_SPB - the q8 blob's block shape
require dasllama/dasllama_tts_blocks
require dasllama/dasllama_tts_types
require dasllama/dasllama_layout
require dasllama/dasllama_tts_slab
require daslib/apply
require daslib/math_bits
require dasllama/dasllama_tower   // Clamp — the calibration record the gemma4a sites carry
require metal/das_metal_boost
require dasllama/dasllama_metal_common
require dasllama/dasllama_metal_kernels
require dasllama/dasllama_metal_prefill   // pf_enc_bf16_mm + the attention trio — the crowned builders

// The tower/embedder Metal driver: feed-forward encodes with no session and no mirror - one shot a
// call, the Pocket frame loop's per-voice device K/V slot the one state kept across calls. Serves the
// vision and audio block loops, the StyleTTS2 synthesis and the Pocket TTS codec and frame loop.
// Declines by its hook's refusal value (false, or a count below zero); the CPU chain is always the
// correct fallback. Engage evidence is counter deltas (metal_tower_stats), never "the model ran".

var private g_tw_failed = false
var private g_tw_pool : MetalBufferPool
var private g_tw_env_tower = true
var private g_tw_env_tower_f16 = true
var private g_tw_env_tower_flash = true
var private g_tw_tail = true              // the whisper-class projector tail rides the device behind the blocks
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

//! The towers' halfword-weight GEMM route's A/B seat (DASLLAMA_METAL_TOWER_F16's in-process twin).
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

//! The whisper-class projector tail's A/B seat: off, the tail seat serves the blocks alone and the CPU tail reads their rows.
def set_metal_tower_tail(on : bool) {
    g_tw_tail = on
}

//! The tail lever as it stands, for a cell that puts it back where it found it.
def metal_tower_tail_knob() : bool => g_tw_tail

//! Encodes whose block/merger GEMMs rode the f16-W shadow route — the engage witness.
def metal_tower_f16_encodes() : int64 => g_tower_f16_encodes

//! whisper-preprocessor mels (Qwen3-ASR's, the chat towers' chunked one) the device served
def metal_tower_mel_encodes() : int64 => g_tower_mels

//! whisper-class encodes whose projector tail ran on the device behind the blocks
def metal_tower_tail_encodes() : int64 => g_tower_tail_encodes

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
    release_handle(g_tw_ones)
    st2_slab_free(g_tw_q3a_wpad)
    st2_slab_free(g_tw_fc_twin)
    st2_slab_free(g_tw_q8)
    st2_slab_free(g_tw_conv1_pad)
    st2_slab_free(g_tw_cn_front)
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
    let mp = round_up(npos, 32l)
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
        enc_ln(enc, bx0, bblob, uint64(t.ln1_w * 4l), bblob, uint64(t.ln1_b * 4l), bx0, 0ul, u_pdim, u_eps, npos)
        // the patch GEMM: per-plane format (gemma-4's shipped mmproj is MIXED)
        if (t.patch_bf16) {
            var bwblob = plane_buffer(g_dev, unsafe(reinterpret<void?>(plane_at(t.wblob, 0l))), uint64(length(t.wblob) * 2l), mapped)
            pf_enc_bf16_mm(enc, bwblob, uint64(t.patch_w * 2l), bx0, bx1, u_pdim, u_d, mp, d, kdim = t.patch_dim)
        } else {
            enc_f32_mm(enc, bblob, uint64(t.patch_w * 4l), bx0, bx1, 0ul, u_pdim, u_d, mp, d)
        }
        enc_add_bias_rows(enc, bx1, 0ul, bblob, uint64(t.patch_b * 4l), u_d, u_tot, npos * d)
        enc_ln(enc, bx1, bblob, uint64(t.ln2_w * 4l), bblob, uint64(t.ln2_b * 4l), bx1, 0ul, u_d, u_eps, npos)
        enc_posadd2d(enc, bx1, bblob, uint64(t.pos_w * 4l), u_d, u_ncols, u_pe, u_tot, npos * d)
        enc_ln(enc, bx1, bblob, uint64(t.ln3_w * 4l), bblob, uint64(t.ln3_b * 4l), bx1, 0ul, u_d, u_eps, npos)
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

var private g_tw_q8 : St2SlabBase            //!< the served whisper-class tower's q8 GEMM planes as one device blob, resident until the weights drop
var private g_tw_q8_boffs : array<uint64>    //!< a layer's regions in that blob, TW_Q8_MATS a layer: q, k, v, o, fc1, fc2
var private g_tw_q8_conv2 = 0ul              //!< the stem's second conv in that blob
let private TW_Q8_MATS = 6l
let private TW_Q8_BLOCK_BYTES = Q8_QPB + Q8_SPB

//! one GEMM region out of the CPU backend's layout into the q8 blob's blocks at the 16-byte-aligned cursor; answers its byte offset
def private tw_q8_region(t : AudioTower; off, n, rows : int64; lay : Q8RepackLayout; var staging : array<uint8>; var cur : uint64&) : uint64 {
    cur = (cur + 15ul) & ~15ul
    let at = cur
    q8_region_to_metal_blob(plane_at(t.qblob, off), plane_at(t.qscales, off / Q8_BLOCK_ELEMS),
        n, rows, lay.repacked, lay.mr, lay.kgroup, lay.wbias, unsafe(addr(staging[int64(cur)])))
    cur += uint64((rows * n / Q8_BLOCK_ELEMS) * TW_Q8_BLOCK_BYTES)
    return at
}

//! The whisper-class tower's q8 block GEMM planes on the device, uploaded once a tower. False where the buffer cannot be made.
[cold_path, arch(at="../ARCHITECTURE_GPU_TOWER.md#tower-encode-chains")]
def private tw_q8_attach(t : AudioTower) : bool {
    // a pure read of the layout the load repacked under: selecting again here could name another
    let lay = active_q8_repack_layout(kernel_backend_needs_repack(active_kernel_backend()))
    let shape = hash_combine64(hash_combine64(uint64(lay.mr), uint64(lay.kgroup)), hash_combine64(uint64(lay.wbias), lay.repacked ? 1ul : 0ul))
    let key = hash_combine64(hash_combine64(intptr(plane_at(t.qblob, 0l)), uint64(length(t.qblob))), shape)
    return true if (st2_slab_current(g_tw_q8, key))
    st2_slab_free(g_tw_q8)
    let d = t.d_model
    let ff = t.n_ff
    let layer_elems = 4l * d * d + 2l * d * ff
    var staging : array<uint8>
    staging |> reserve_resize(((t.n_layer * layer_elems + 3l * d * d) / Q8_BLOCK_ELEMS) * TW_Q8_BLOCK_BYTES + 16l * (t.n_layer * TW_Q8_MATS + 1l))
    g_tw_q8_boffs |> resize(t.n_layer * TW_Q8_MATS)
    var cur = 0ul
    g_tw_q8_conv2 = tw_q8_region(t, t.conv2_w_off, 3l * d, d, lay, staging, cur)
    for (l in range64(t.n_layer)) {
        let lo = enc_layer_offsets(t, l)
        var mi = 0l
        for (woff in [lo.q_w, lo.k_w, lo.v_w, lo.o_w]) {
            g_tw_q8_boffs[l * TW_Q8_MATS + mi] = tw_q8_region(t, woff, d, d, lay, staging, cur)
            mi++
        }
        g_tw_q8_boffs[l * TW_Q8_MATS + 4l] = tw_q8_region(t, lo.fc1_w, d, ff, lay, staging, cur)
        g_tw_q8_boffs[l * TW_Q8_MATS + 5l] = tw_q8_region(t, lo.fc2_w, ff, d, lay, staging, cur)
    }
    var blob = metal_new_buffer(g_dev, cur)
    if (blob != null) {
        unsafe {
            memcpy(metal_buffer_contents(blob), addr(staging[0]), cur)
        }
        g_tw_q8.buf = blob
        g_tw_q8.key = key
    }
    delete staging
    return blob != null
}

//! one block GEMM of the whisper-class chain: region `site` of the device q8 blob on a q8 tower, tw_mm's forms on an f32 one
def private wt_mm(enc : MetalComputeEncoder?; q8 : bool; site : int64; f16w, bf16w : bool; bwh, bblob : MetalBuffer?; welem : int64;
                  bx, bxh, by : MetalBuffer?; bk, bn : MetalBuffer?; rows, nd, kdim : int64; cvt : bool) {
    if (q8) {
        if (bxh != null && cvt) {
            enc_cvt_half(enc, bx, bxh, rows * kdim)
        }
        pf_enc_q8_mm(enc, g_tw_q8.buf, g_tw_q8_boffs[site], bx, bxh, by, bk, bn, rows, nd, kdim)
    } else {
        tw_mm(enc, f16w, bf16w, bwh, bblob, welem, bx, bxh, by, 0ul, bk, bn, rows, nd, kdim, cvt)
    }
}

// The whisper-class block loop as ONE command buffer; the attention trio rides uend = npos (the
// non-causal uniform span). The mel's spectrum and the projector tail ride their own seats. Pad-row garbage is
// CONTAINED: QK/AV zero k/v rows >= npos, softmax zeroes [uend, np32), pad outputs go unread.
[hot_path, arch(at="../ARCHITECTURE_IMAGE.md#image-tower-twin-plane")]
def private metal_tower_blocks(t : AudioTower; var s : EncoderState; npos : int64) : bool
    => metal_tower_blocks_form(t, s, npos, TwForm.blocks)

//! the blocks with the tower's post-norm behind them in the same command buffer: the normed rows land in `s.xb`
[hot_path]
def private metal_tower_blocks_ln_post(t : AudioTower; var s : EncoderState; npos : int64) : bool
    => metal_tower_blocks_form(t, s, npos, TwForm.ln_post)

//! the blocks with the projector tail behind them in the same command buffer where the tail's shapes ride the device (the soft
//! tokens land in `s.out`), the blocks alone where they do not
[hot_path]
def private metal_tower_blocks_tail(t : AudioTower; var s : EncoderState; npos : int64) : TowerTailServed {
    if (g_tw_tail && tw_tail_shape_ok(t, npos)) {
        return metal_tower_blocks_form(t, s, npos, TwForm.tail) ? TowerTailServed.tail : TowerTailServed.declined
    }
    return metal_tower_blocks_form(t, s, npos, TwForm.blocks) ? TowerTailServed.blocks : TowerTailServed.declined
}

//! what follows the whisper-class block loop in its command buffer
enum private TwForm {
    blocks
    ln_post   // the tower's post-norm: whisper's encoder output
    tail      // the projector tail: the chat towers' soft tokens
}

//! Whether the projector tail rides the device: an f32 tower (a q8 tower's projector planes are not on the device), the tail's
//! kernels compiled, every GEMM on the f32 tile's lattice (K a multiple of 32, N of 64) and the rms rows inside the ones row.
def private tw_tail_shape_ok(t : AudioTower; npos : int64) : bool {
    return false if (t.q8 || !pf_tw_tail_ready() || npos % 2l != 0l)
    if (t.proj_kind == AudioProjKind.qwen2a) {
        return t.d_model % 32l == 0l && t.proj_dim % 64l == 0l
    }
    let m1in = t.d_model * t.stack_factor
    let mm2_in = t.proj_kind == AudioProjKind.ultravox ? t.mm1_out / 2l : t.mm1_out
    return m1in % 32l == 0l && t.mm1_out % 64l == 0l && mm2_in % 32l == 0l && t.proj_dim % 64l == 0l
}

//! the projector tail's device rows and uniforms
struct private TwTail {
    rows : int64        //!< soft tokens a chunk
    mpr : int64         //!< `rows` on the GEMM's 32-row lattice
    bin : MetalBuffer?  //!< the first GEMM's input rows, padded to `mpr`
    bhid : MetalBuffer? //!< the MLP projectors' hidden rows
    bgate : MetalBuffer?//!< ultravox: the gated half rows, the second GEMM's input
    bout : MetalBuffer? //!< the soft tokens
    bn1 : MetalBuffer?  //!< ultravox: the pre-norm's weight row
    bn2 : MetalBuffer?  //!< ultravox: the mid-norm's weight row
    bytes_in : uint64
    bytes_hid : uint64
    bytes_gate : uint64
    bytes_out : uint64
    bytes_n1 : uint64
    bytes_n2 : uint64
    u_in : MetalBuffer?
    u_hid : MetalBuffer?
    u_gate : MetalBuffer?
    u_proj : MetalBuffer?
    u_tot : MetalBuffer?
    u_eps6 : MetalBuffer?
}

def private tw_tail_make(t : AudioTower; npos : int64) : TwTail {
    let d = t.d_model
    let rows = audio_tokens_per_chunk(t)
    let mpr = round_up(rows, 32l)
    let mlp = t.proj_kind != AudioProjKind.qwen2a
    let uv = t.proj_kind == AudioProjKind.ultravox
    let m1in = mlp ? d * t.stack_factor : d
    let half = t.mm1_out / 2l
    var tl = TwTail(rows = rows, mpr = mpr,
        bytes_in = uint64(max(mpr * m1in, npos * d) * 4l), bytes_hid = uint64(mpr * t.mm1_out * 4l),
        bytes_gate = uint64(mpr * half * 4l), bytes_out = uint64(mpr * t.proj_dim * 4l),
        bytes_n1 = uint64(m1in * 4l), bytes_n2 = uint64(half * 4l))
    tl.bin = pool_acquire_pinned(g_tw_pool, g_dev, tl.bytes_in)
    tl.bout = pool_acquire_pinned(g_tw_pool, g_dev, tl.bytes_out)
    zero_rows(tl.bin, rows, mpr, m1in)   //! the tail's kernels write the live rows alone and the GEMMs read the row pad: the buffers are pooled
    tl.u_in = uniform_u32(uint(m1in))
    tl.u_proj = uniform_u32(uint(t.proj_dim))
    tl.u_tot = uniform_u32(uint(rows * (mlp ? t.mm1_out : t.proj_dim)))
    if (mlp) {
        tl.bhid = pool_acquire_pinned(g_tw_pool, g_dev, tl.bytes_hid)
        tl.u_hid = uniform_u32(uint(t.mm1_out))
    }
    if (uv) {
        tl.bgate = pool_acquire_pinned(g_tw_pool, g_dev, tl.bytes_gate)
        zero_rows(tl.bgate, rows, mpr, half)
        tl.bn1 = pool_acquire_pinned(g_tw_pool, g_dev, tl.bytes_n1)
        tl.bn2 = pool_acquire_pinned(g_tw_pool, g_dev, tl.bytes_n2)
        tl.u_gate = uniform_u32(uint(half))
        tl.u_eps6 = uniform_f32(1e-6)   // the ultravox norms' own eps, not the tower's
        unsafe {
            memcpy(metal_buffer_contents(tl.bn1), plane_at(t.fblob, t.normpre_off), tl.bytes_n1)
            memcpy(metal_buffer_contents(tl.bn2), plane_at(t.fblob, t.normmid_off), tl.bytes_n2)
        }
    }
    return tl
}

//! The projector tail behind the blocks, the CPU tail's steps at its widths: `bx` holds the blocks' rows, `bs` is a scratch
//! row plane of the blocks' size.
[arch(at="../ARCHITECTURE_GPU_TOWER.md#tower-encode-chains")]
def private tw_tail_body(enc : MetalComputeEncoder?; t : AudioTower; tl : TwTail; bblob, bx, bs : MetalBuffer?;
                         u_d, u_eps : MetalBuffer?; npos : int64) {
    let d = t.d_model
    let rows = tl.rows
    if (t.proj_kind == AudioProjKind.ultravox) {   // post-norm over every position, the stacked rows normed, gated MLP
        let half = t.mm1_out / 2l
        enc_ln(enc, bx, bblob, uint64(t.lnp_w_off * 4l), bblob, uint64(t.lnp_b_off * 4l), bs, 0ul, u_d, u_eps, npos)
        pf_enc_rms(enc, bs, 0ul, tl.bn1, tl.bin, tl.u_in, tl.u_eps6, rows)
        enc_f32_mm(enc, bblob, uint64(t.proj_w_off * 4l), tl.bin, tl.bhid, 0ul, tl.u_in, tl.u_hid, tl.mpr, t.mm1_out)
        enc_tw_swiglu_rows(enc, tl.bhid, tl.bgate, TwRowsArgs(width = uint(half), total = uint(rows * half)), rows * half)
        pf_enc_rms(enc, tl.bgate, 0ul, tl.bn2, tl.bgate, tl.u_gate, tl.u_eps6, rows)
        enc_f32_mm(enc, bblob, uint64(t.mm2_w_off * 4l), tl.bgate, tl.bout, 0ul, tl.u_gate, tl.u_proj, tl.mpr, t.proj_dim)
        return
    }
    let tp = npos / 2l   // the pair pool BEFORE the post-norm
    enc_tw_pool2(enc, bx, bs, TwRowsArgs(width = uint(d), total = uint(tp * d)), tp * d)
    enc_ln(enc, bs, bblob, uint64(t.lnp_w_off * 4l), bblob, uint64(t.lnp_b_off * 4l), tl.bin, 0ul, u_d, u_eps, tp)
    if (t.proj_kind == AudioProjKind.qwen2a) {
        enc_f32_mm(enc, bblob, uint64(t.proj_w_off * 4l), tl.bin, tl.bout, 0ul, tl.u_in, tl.u_proj, tl.mpr, t.proj_dim)
        enc_add_bias_rows(enc, tl.bout, 0ul, bblob, uint64(t.proj_b_off * 4l), tl.u_proj, tl.u_tot, rows * t.proj_dim)
    } else {   // voxtral: the stacked rows through the GELU MLP
        enc_f32_mm(enc, bblob, uint64(t.proj_w_off * 4l), tl.bin, tl.bhid, 0ul, tl.u_in, tl.u_hid, tl.mpr, t.mm1_out)
        enc_gelu_erf(enc, tl.bhid, 0ul, tl.u_tot, rows * t.mm1_out)
        enc_f32_mm(enc, bblob, uint64(t.mm2_w_off * 4l), tl.bhid, tl.bout, 0ul, tl.u_hid, tl.u_proj, tl.mpr, t.proj_dim)
    }
}

//! the tail's soft tokens into `s.out` where the command buffer ran; the tail's buffers released
def private tw_tail_land(t : AudioTower; var tl : TwTail; ran : bool; var s : EncoderState) {
    if (ran) {
        unsafe {
            memcpy(addr(s.out[0]), metal_buffer_contents(tl.bout), uint64(tl.rows * t.proj_dim * 4l))
        }
        g_tower_tail_encodes++
    }
    pool_release(g_tw_pool, tl.bin, tl.bytes_in)
    pool_release(g_tw_pool, tl.bout, tl.bytes_out)
    pool_release(g_upool, tl.u_in, 4ul)
    pool_release(g_upool, tl.u_proj, 4ul)
    pool_release(g_upool, tl.u_tot, 4ul)
    if (tl.bhid != null) {
        pool_release(g_tw_pool, tl.bhid, tl.bytes_hid)
        pool_release(g_upool, tl.u_hid, 4ul)
    }
    if (tl.bgate != null) {
        pool_release(g_tw_pool, tl.bgate, tl.bytes_gate)
        pool_release(g_tw_pool, tl.bn1, tl.bytes_n1)
        pool_release(g_tw_pool, tl.bn2, tl.bytes_n2)
        pool_release(g_upool, tl.u_gate, 4ul)
        pool_release(g_upool, tl.u_eps6, 4ul)
    }
}

[hot_path, arch(at="../ARCHITECTURE_IMAGE.md#image-tower-twin-plane")]
def private metal_tower_blocks_form(t : AudioTower; var s : EncoderState; npos : int64; form : TwForm) : bool {   // nolint:STYLE038 — the dispatch chain is the shape
    if (!g_tw_env_tower) {
        return tw_decline(MetalTowerDecline.knob)
    }
    let d = t.d_model
    let hs = d / t.n_head
    if (d % 64l != 0l || t.n_ff % 64l != 0l || hs % 64l != 0l || t.layers_off % 4l != 0l || t.layer_stride % 4l != 0l) {
        return tw_decline(MetalTowerDecline.shape)
    }
    if (!metal_tower_init() || (t.q8 && !tw_q8_attach(t))) {   // per-encode: re-arms after any shutdown seat
        return tw_decline(MetalTowerDecline.device)
    }
    residency_flush()   // pin/commit before the first submission (the post-CPU-window slack)
    let mp = round_up(npos, 32l)
    let nk64 = round_up(npos, 64l)
    let ff = t.n_ff
    let mapped = t.image_map != null
    var bblob = plane_buffer(g_dev, unsafe(reinterpret<void?>(plane_at(t.fblob, 0l))), uint64(length(t.fblob) * 4l), mapped)
    let bytes_row = uint64(mp * d * 4l)
    let bytes_h = uint64(mp * ff * 4l)
    let bytes_att = uint64(t.n_head * mp * nk64 * 2l)
    let bytes_rowk = uint64(nk64 * d * 4l)   //! K panel: the QK grid walks nk64 key rows, past mp
    let bytes_stat = uint64(t.n_head * mp * 2l * 4l)
    let twin = !t.q8 && g_tw_env_tower_f16 && length(t.wblob) > 0l
    let f16_route = twin && !t.blk_bf16 && pf_hmm_ready()
    let bf16_route = twin && t.blk_bf16
    let bytes_xh = uint64(mp * ff * 2l)
    var bxh = (f16_route || (bf16_route && pf_bf16_half_ready()) || (t.q8 && pf_q8_mm_half())) ? pool_acquire_pinned(g_tw_pool, g_dev, bytes_xh) : null
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
    var tl = form == TwForm.tail ? tw_tail_make(t, npos) : TwTail()
    var err = ""
    let ran = with_compute_encoder(g_queue, err) $(enc : MetalComputeEncoder?) {   // nolint:PERF026 — the error-text fetch runs only on the failure leg
        for (l in range64(t.n_layer)) {
            let lo = enc_layer_offsets(t, l)
            enc_ln(enc, bx, bblob, uint64(lo.ln1_w * 4l), bblob, uint64(lo.ln1_b * 4l), bxb, 0ul, u_d, u_eps, npos)
            wt_mm(enc, t.q8, l * TW_Q8_MATS, f16w, bf16w, bwh, bblob, lo.q_w, bxb, bxh, bq, u_d, u_d, mp, d, d, true)
            enc_add_bias_rows(enc, bq, 0ul, bblob, uint64(lo.q_b * 4l), u_d, u_totd, npos * d)
            wt_mm(enc, t.q8, l * TW_Q8_MATS + 1l, f16w, bf16w, bwh, bblob, lo.k_w, bxb, bxh, bk, u_d, u_d, mp, d, d, false)
            if (t.has_k_bias) {
                enc_add_bias_rows(enc, bk, 0ul, bblob, uint64(lo.k_b * 4l), u_d, u_totd, npos * d)
            }
            wt_mm(enc, t.q8, l * TW_Q8_MATS + 2l, f16w, bf16w, bwh, bblob, lo.v_w, bxb, bxh, bv, u_d, u_d, mp, d, d, false)
            enc_add_bias_rows(enc, bv, 0ul, bblob, uint64(lo.v_b * 4l), u_d, u_totd, npos * d)
            enc_qk_mm(enc, bq, bk, batt, ka, mp, nk64, t.n_head)
            enc_rowstat(enc, batt, ka, bstat, null, npos, t.n_head)
            enc_av_mm(enc, batt, bv, bxb2, ka, bstat, mp, t.n_head, hs)
            wt_mm(enc, t.q8, l * TW_Q8_MATS + 3l, f16w, bf16w, bwh, bblob, lo.o_w, bxb2, bxh, bxb, u_d, u_d, mp, d, d, true)
            enc_add_bias_rows(enc, bxb, 0ul, bblob, uint64(lo.o_b * 4l), u_d, u_totd, npos * d)
            enc_add(enc, bx, 0ul, bxb, 0ul, u_totd, npos * d)
            enc_ln(enc, bx, bblob, uint64(lo.ln2_w * 4l), bblob, uint64(lo.ln2_b * 4l), bxb, 0ul, u_d, u_eps, npos)
            wt_mm(enc, t.q8, l * TW_Q8_MATS + 4l, f16w, bf16w, bwh, bblob, lo.fc1_w, bxb, bxh, bh, u_d, u_ff, mp, ff, d, true)
            enc_add_bias_rows(enc, bh, 0ul, bblob, uint64(lo.fc1_b * 4l), u_ff, u_totff, npos * ff)
            if (t.gelu_tanh) {
                enc_gelu_lut(enc, bh, 0ul, u_totff, npos * ff)
            } else {
                enc_gelu_erf(enc, bh, 0ul, u_totff, npos * ff)
            }
            wt_mm(enc, t.q8, l * TW_Q8_MATS + 5l, f16w, bf16w, bwh, bblob, lo.fc2_w, bh, bxh, bxb2, u_ff, u_d, mp, d, ff, true)
            enc_add_bias_rows(enc, bxb2, 0ul, bblob, uint64(lo.fc2_b * 4l), u_d, u_totd, npos * d)
            enc_add(enc, bx, 0ul, bxb2, 0ul, u_totd, npos * d)
        }
        if (form == TwForm.ln_post) {
            enc_ln(enc, bx, bblob, uint64(t.lnp_w_off * 4l), bblob, uint64(t.lnp_b_off * 4l), bxb, 0ul, u_d, u_eps, npos)
        } elif (form == TwForm.tail) {
            tw_tail_body(enc, t, tl, bblob, bx, bxb, u_d, u_eps, npos)
        }
    }
    if (form == TwForm.tail) {
        tw_tail_land(t, tl, ran, s)
    }
    var served = false
    if (ran) {
        unsafe {   // the post-norm seat lands the normed rows where the CPU post-norm would: `s.xb`; the tail seat landed `s.out`
            if (form == TwForm.ln_post) {
                memcpy(addr(s.xb[0]), metal_buffer_contents(bxb), uint64(npos * d * 4l))
            } elif (form == TwForm.blocks) {
                memcpy(addr(s.x[0]), metal_buffer_contents(bx), uint64(npos * d * 4l))
            }
        }
        tw_count_encode(npos, t.n_layer, f16w || bf16w)
        served = true
    }
    pool_release(g_tw_pool, bx, bytes_row)
    pool_release(g_tw_pool, bxb, bytes_row)
    pool_release(g_tw_pool, bxb2, bytes_row)
    pool_release(g_tw_pool, bq, bytes_row)
    pool_release(g_tw_pool, bk, bytes_rowk)
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
    c.mp = round_up(npos, 32l)
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
        && gemma4a_attn_past() <= 15l && gemma4a_mid_dim(t) % 64l == 0l && t.proj_dim % 64l == 0l && gemma4a_mid_dim(t) <= int64(TW_ONES_DIM))
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
    upload_rows_padded(c.bpos, unsafe(reinterpret<void?>(plane_at(t.pos_emb, 0l))), past + 1l, 32l, d)   // the RPE table, zero-padded to the GEMM's 32-row floor
    //! the calibration records, one [imin imax omin omax] row per clamped GEMM site
    var bp = unsafe(reinterpret<float?>(metal_buffer_contents(c.bbounds)))
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

//! the projector tail's device rows and uniforms: the encoder-width rows the out projection lands, the decoder-width soft tokens
struct private G4aTail {
    btmp : MetalBuffer?
    bout : MetalBuffer?
    u_mid : MetalBuffer?
    u_proj : MetalBuffer?
    u_totmid : MetalBuffer?
    bytes_tmp : uint64
    bytes_out : uint64
}

def private g4a_tail_make(t : Gemma4aEncoder; c : G4aGpuCtx) : G4aTail {
    let mid = gemma4a_mid_dim(t)
    let bytes_tmp = uint64(c.mp * mid * 4l)
    let bytes_out = uint64(c.mp * t.proj_dim * 4l)
    return G4aTail(bytes_tmp = bytes_tmp, bytes_out = bytes_out,
        btmp = pool_acquire_pinned(g_tw_pool, g_dev, bytes_tmp), bout = pool_acquire_pinned(g_tw_pool, g_dev, bytes_out),
        u_mid = uniform_u32(uint(mid)), u_proj = uniform_u32(uint(t.proj_dim)), u_totmid = uniform_u32(uint(c.npos * mid)))
}

//! The projector tail behind the blocks, in the blocks' command buffer: the out projection and its bias, the weightless
//! norm, the audio embedder - the CPU tail's four steps at its widths.
[arch(at="../ARCHITECTURE_GPU_TOWER.md#tower-encode-chains")]
def private g4a_tail_body(enc : MetalComputeEncoder?; t : Gemma4aEncoder; c : G4aGpuCtx; tl : G4aTail) {
    let mid = gemma4a_mid_dim(t)
    enc_f32_mm(enc, c.bblob, uint64(t.outproj_w * 4l), c.bx, tl.btmp, 0ul, c.u_d, tl.u_mid, c.mp, mid)
    enc_add_bias_rows(enc, tl.btmp, 0ul, c.bblob, uint64(t.outproj_b * 4l), tl.u_mid, tl.u_totmid, c.npos * mid)
    pf_enc_rms(enc, tl.btmp, 0ul, g_tw_ones, tl.btmp, tl.u_mid, c.u_eps, c.npos)
    enc_f32_mm(enc, c.bblob, uint64(t.mmproj_w * 4l), tl.btmp, tl.bout, 0ul, tl.u_mid, tl.u_proj, c.mp, t.proj_dim)
}

//! the tail's soft tokens into `s.out` where the command buffer ran, the CPU tail told to stand down; the tail's buffers released
def private g4a_tail_land(t : Gemma4aEncoder; c : G4aGpuCtx; var tl : G4aTail; ran : bool; var s : Gemma4aState) {
    if (ran) {
        ensure_length(s.out, c.npos * t.proj_dim)
        unsafe {
            memcpy(addr(s.out[0]), metal_buffer_contents(tl.bout), uint64(c.npos * t.proj_dim * 4l))
        }
        s.out_ready = true
    }
    pool_release(g_tw_pool, tl.btmp, tl.bytes_tmp)
    pool_release(g_tw_pool, tl.bout, tl.bytes_out)
    pool_release(g_upool, tl.u_mid, 4ul)
    pool_release(g_upool, tl.u_proj, 4ul)
    pool_release(g_upool, tl.u_totmid, 4ul)
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
    var tl <- g4a_tail_make(t, c)
    upload_rows_padded(c.bx, unsafe(addr(s.x[0])), npos, c.mp, d)
    var err = ""
    let ran = with_compute_encoder(g_queue, err) $(enc : MetalComputeEncoder?) {   // nolint:PERF026 — the error-text fetch runs only on the failure leg
        g4a_blocks_body(enc, t, c)
        g4a_tail_body(enc, t, c, tl)
    }
    let served = tw_readback(c, ran, s.x, d, t.n_layer)
    g4a_tail_land(t, c, tl, ran, s)
    g4a_ctx_release(c)
    if (!served) {
        return tw_decline(MetalTowerDecline.gpu_error)
    }
    return true
}

let private TW_SET_MAX = 32

//! Pooled scratch taken one buffer at a time and released as one, each buffer's size kept beside it.
struct private TwBufSet {
    bufs : MetalBuffer? [TW_SET_MAX]
    sizes : uint64 [TW_SET_MAX]
    n : int
}

def private tw_take(var s : TwBufSet; bytes : uint64) : MetalBuffer? {
    assert(s.n < TW_SET_MAX)
    var b = pool_acquire_pinned(g_tw_pool, g_dev, bytes)
    if (b != null) {
        s.bufs[s.n] = b
        s.sizes[s.n] = bytes
        s.n++
    }
    return b
}

def private tw_release(var s : TwBufSet) {
    for (i in range(s.n)) {
        pool_release(g_tw_pool, s.bufs[i], s.sizes[i])
    }
    s.n = 0
}

//! The slab key of a device buffer built from `plane`: it stands while the plane's base address and length do.
def private tw_plane_key(plane : PlaneF) : uint64 => hash_combine64(intptr(plane_at(plane, 0l)), uint64(length(plane)))

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
    bwh : MetalBuffer?      //!< the blob's halfword twin, where the half tensor GEMM serves
    bxh : MetalBuffer?      //!< its activation panel
    f16w : bool
    dev : FcDevPlanes       //!< the all-heads attention's scratch, in the per-head scratch's place where it serves
    dev_on : bool
    scratch : TwBufSet
}

let private FC_DEV_SLAB_MAX_BYTES = 268435456ul   //!< the all-heads score slabs' ceiling (about three minutes of audio at eight heads); past it the per-head loop's reused scratch serves

var private g_tw_fc_dev = true
var private g_tw_fc_dev_encodes = 0l

//! Serve the FastConformer attention over every head at once where the device pair is crowned (`on`), or
//! keep the per-head loop; answers the setting it replaced.
def set_metal_fc_attn_dev(on : bool) : bool {
    let was = g_tw_fc_dev
    g_tw_fc_dev = on
    return was
}

//! Encodes whose FastConformer attention rode the all-heads device form - the engage witness.
def metal_tower_fc_dev_encodes() : int64 => g_tw_fc_dev_encodes

//! the all-heads attention's planes where the device pair serves and its score slabs fit; false leaves the context on the per-head scratch
def private fc_dev_attach(var c : FcGpuCtx; d : int64) : bool {
    let bytes_sch = uint64(c.n_head * c.mp * c.nkp * 2l)
    let bytes_srh = uint64(c.n_head * c.mp * c.wwpad * 2l)
    if (!g_tw_fc_dev || !pf_fc_dev_ready() || bytes_sch + bytes_srh > FC_DEV_SLAB_MAX_BYTES) {
        return false
    }
    let bytes_qh = uint64(c.mp * d * 2l)
    let bytes_kvh = uint64(c.nkp * d * 2l)
    c.dev.quh = tw_take(c.scratch, bytes_qh)
    c.dev.qvh = tw_take(c.scratch, bytes_qh)
    c.dev.kh = tw_take(c.scratch, bytes_kvh)
    c.dev.vh = tw_take(c.scratch, bytes_kvh)
    c.dev.ph = tw_take(c.scratch, uint64(c.wwpad * d * 2l))
    c.dev.sc = tw_take(c.scratch, bytes_sch)
    c.dev.sr = tw_take(c.scratch, bytes_srh)
    c.dev.stat = tw_take(c.scratch, uint64(c.n_head * c.mp * 4l))
    return true
}

var private g_tw_fc_twin : St2SlabBase   //!< the served blob's halfword twin, resident until the weights drop

//! The blob's halfword twin on the device, converted there once a blob: the halves the f16-staged
//! tile stamp rounds a tile at a time. Null where the buffer or the convert fails.
[arch(at="../ARCHITECTURE_GPU_TOWER.md#tower-encode-chains")]
def private fc_twin_attach(blob : PlaneF; bblob : MetalBuffer?) : MetalBuffer? {
    let key = tw_plane_key(blob)
    return g_tw_fc_twin.buf if (st2_slab_current(g_tw_fc_twin, key))
    st2_slab_free(g_tw_fc_twin)
    let blen = length(blob)
    var twin = metal_new_buffer(g_dev, uint64(blen) * 2ul)
    if (twin == null) {
        return null
    }
    var err = ""
    let ran = with_compute_encoder(g_queue, err) $(enc : MetalComputeEncoder?) {   // nolint:PERF026 — the error-text fetch runs only on the failure leg
        enc_cvt_half(enc, bblob, twin, blen)
    }
    if (!ran) {
        release_handle(twin)
        return null
    }
    g_tw_fc_twin.buf = twin
    g_tw_fc_twin.key = key
    return twin
}

//! one weighted GEMM of the chain: the half tensor form over the twin where it serves, the f32 tile form otherwise
def private fc_mm(enc : MetalComputeEncoder?; c : FcGpuCtx; woff : int64; bx, by, bk, bn : MetalBuffer?; rows, nd, kdim : int64) {
    tw_mm(enc, c.f16w, false, c.bwh, c.bblob, woff, bx, c.bxh, by, 0ul, bk, bn, rows, nd, kdim, true)
}

def private fc_ctx_make(blob : PlaneF; mapped : bool; d, n_ff, n_head, nk : int64; eps : float;
                        pos_emb : array<float>; npos, ww : int64; tap_major : bool) : FcGpuCtx {
    let dh = d / n_head
    var c = FcGpuCtx(wwp = round_up(ww, 32l), ww = ww, n_head = n_head, tap_major = tap_major,
        nkp = round_up(npos, 64l), wwpad = round_up(ww, 64l))
    tw_ctx_make(c, unsafe(reinterpret<void?>(plane_at(blob, 0l))), uint64(length(blob) * 4l), mapped,
        npos, d, n_ff, eps, 0.5, uint64(c.wwp * d * 4l))
    c.bposl = tw_take(c.scratch, c.bytes_pos)
    upload_rows_padded(c.bpos, unsafe(addr<void?>(pos_emb[0])), ww, c.wwp, d)   // the sinusoidal rel table, zero-padded to the GEMM's row floor
    c.u_nk = uniform_u32(uint(nk))
    c.u_np = uniform_u32(uint(npos))
    c.ka = fc_attn_args(d, dh, npos)
    c.dev_on = fc_dev_attach(c, d)
    if (!c.dev_on) {
        c.bytes_head = uint64(c.mp * dh * 4l)
        let bytes_kh = uint64(c.nkp * dh * 4l)
        let bytes_sc = uint64(c.mp * c.nkp * 4l)
        c.bqu = tw_take(c.scratch, c.bytes_head)
        c.bqv = tw_take(c.scratch, c.bytes_head)
        c.bkh = tw_take(c.scratch, bytes_kh)
        c.bvt = tw_take(c.scratch, bytes_kh)
        c.bph = tw_take(c.scratch, uint64(c.wwpad * dh * 4l))
        c.bsc = tw_take(c.scratch, bytes_sc)
        c.bsr = tw_take(c.scratch, uint64(c.mp * c.wwpad * 4l))
        c.bp = tw_take(c.scratch, bytes_sc)
        c.boh = tw_take(c.scratch, uint64(n_head * c.mp * dh * 4l))
        for (h in range64(n_head)) {
            c.u_heads[h] = uniform_u32(uint(h))
        }
        c.u_hs = uniform_u32(uint(dh))
        c.u_nkp = uniform_u32(uint(c.nkp))
        c.u_wwpad = uniform_u32(uint(c.wwpad))
    }
    if (g_tw_env_tower_f16 && pf_hmm_ready()) {
        c.bwh = fc_twin_attach(blob, c.bblob)
        c.bxh = tw_take(c.scratch, uint64(max(c.mp * max(n_ff, 2l * d), c.wwp * d) * 2l))
        c.f16w = c.bwh != null && c.bxh != null
    }
    return <- c
}

def private fc_ctx_release(var c : FcGpuCtx) {
    pool_release(g_upool, c.u_nk, 4ul)
    pool_release(g_upool, c.u_np, 4ul)
    tw_release(c.scratch)
    if (!c.dev_on) {
        for (h in range64(c.n_head)) {
            pool_release(g_upool, c.u_heads[h], 4ul)
        }
        pool_release(g_upool, c.u_hs, 4ul)
        pool_release(g_upool, c.u_nkp, 4ul)
        pool_release(g_upool, c.u_wwpad, 4ul)
    }
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
    if (c.dev_on) {
        pf_enc_fc_attn_dev(enc, c.bq, c.bk, c.bv, c.bposl, c.bblob, uint64(lo.bias_u * 4l), uint64(lo.bias_v * 4l), c.dev, c.bao, ka, c.n_head)
        return
    }
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
    enc_ln(enc, c.bx, c.bblob, uint64(lo.nf1_w * 4l), c.bblob, uint64(lo.nf1_b * 4l), c.bxb, 0ul, c.u_d, c.u_eps, npos)
    fc_mm(enc, c, lo.ff1a_w, c.bxb, c.bh, c.u_d, c.u_ff, mp, ff, d)
    fc_bias(enc, c, c.bh, lo.ff1a_b, c.u_ff, c.u_totff, npos * ff)
    enc_g4a_silu(enc, c.bh, 0ul, c.u_totff, npos * ff)
    fc_mm(enc, c, lo.ff1b_w, c.bh, c.bxb, c.u_ff, c.u_d, mp, d, ff)
    fc_bias(enc, c, c.bxb, lo.ff1b_b, c.u_d, c.u_totd, npos * d)
    enc_g4a_axpy(enc, c.bx, c.bxb, c.u_totd, c.u_half, npos * d)
    enc_ln(enc, c.bx, c.bblob, uint64(lo.nat_w * 4l), c.bblob, uint64(lo.nat_b * 4l), c.bxb, 0ul, c.u_d, c.u_eps, npos)
    fc_mm(enc, c, lo.q_w, c.bxb, c.bq, c.u_d, c.u_d, mp, d, d)
    fc_bias(enc, c, c.bq, lo.q_b, c.u_d, c.u_totd, npos * d)
    fc_mm(enc, c, lo.k_w, c.bxb, c.bk, c.u_d, c.u_d, mp, d, d)
    fc_bias(enc, c, c.bk, lo.k_b, c.u_d, c.u_totd, npos * d)
    fc_mm(enc, c, lo.v_w, c.bxb, c.bv, c.u_d, c.u_d, mp, d, d)
    fc_bias(enc, c, c.bv, lo.v_b, c.u_d, c.u_totd, npos * d)
    fc_mm(enc, c, lo.pos_w, c.bpos, c.bposl, c.u_d, c.u_d, c.wwp, d, d)
    fc_attention(enc, c, lo)
    fc_mm(enc, c, lo.o_w, c.bao, c.bxb, c.u_d, c.u_d, mp, d, d)
    fc_bias(enc, c, c.bxb, lo.o_b, c.u_d, c.u_totd, npos * d)
    enc_add(enc, c.bx, 0ul, c.bxb, 0ul, c.u_totd, npos * d)
    enc_ln(enc, c.bx, c.bblob, uint64(lo.ncv_w * 4l), c.bblob, uint64(lo.ncv_b * 4l), c.bxb, 0ul, c.u_d, c.u_eps, npos)
    fc_mm(enc, c, lo.pw1_w, c.bxb, c.bh, c.u_d, c.u_2d, mp, 2l * d, d)
    fc_bias(enc, c, c.bh, lo.pw1_b, c.u_2d, c.u_tot2d, npos * 2l * d)
    enc_g4a_glu(enc, c.bh, c.bglu, c.u_d, c.u_totd, npos * d)
    if (c.tap_major) {
        enc_pk_dw(enc, c.bglu, c.bcvd, c.bblob, uint64(lo.dw_w * 4l), c.bblob, uint64(lo.bn_w * 4l),
            c.bblob, uint64(lo.bn_b * 4l), c.u_d, c.u_nk, c.u_np, c.u_totd, npos * d)
    } else {
        enc_cn_dw(enc, c.bglu, c.bcvd, c.bblob, uint64(lo.dw_w * 4l), c.bblob, uint64(lo.bn_w * 4l),
            c.bblob, uint64(lo.bn_b * 4l), c.u_d, c.u_nk, c.u_np, c.u_totd, npos * d)
    }
    fc_mm(enc, c, lo.pw2_w, c.bcvd, c.bxb, c.u_d, c.u_d, mp, d, d)
    fc_bias(enc, c, c.bxb, lo.pw2_b, c.u_d, c.u_totd, npos * d)
    enc_add(enc, c.bx, 0ul, c.bxb, 0ul, c.u_totd, npos * d)
    enc_ln(enc, c.bx, c.bblob, uint64(lo.nf2_w * 4l), c.bblob, uint64(lo.nf2_b * 4l), c.bxb, 0ul, c.u_d, c.u_eps, npos)
    fc_mm(enc, c, lo.ff2a_w, c.bxb, c.bh, c.u_d, c.u_ff, mp, ff, d)
    fc_bias(enc, c, c.bh, lo.ff2a_b, c.u_ff, c.u_totff, npos * ff)
    enc_g4a_silu(enc, c.bh, 0ul, c.u_totff, npos * ff)
    fc_mm(enc, c, lo.ff2b_w, c.bh, c.bxb, c.u_ff, c.u_d, mp, d, ff)
    fc_bias(enc, c, c.bxb, lo.ff2b_b, c.u_d, c.u_totd, npos * d)
    enc_g4a_axpy(enc, c.bx, c.bxb, c.u_totd, c.u_half, npos * d)
    enc_ln(enc, c.bx, c.bblob, uint64(lo.nout_w * 4l), c.bblob, uint64(lo.nout_b * 4l), c.bx, 0ul, c.u_d, c.u_eps, npos)
}

def private fc_shape_ok(d, n_ff, n_head, nk, npos, ww : int64) : bool {
    let dh = n_head > 0l ? d / n_head : 0l
    return (d % 64l == 0l && n_ff % 64l == 0l && n_head * dh == d
        && dh % 64l == 0l && n_head <= int64(FC_MAX_HEADS) && nk % 2l == 1l && npos >= 1l && ww == 2l * npos - 1l)
}

def private tw_count_encode(npos, n_layer : int64; f16 : bool) {
    g_tower_encodes++
    g_tower_rows += npos
    g_tower_blocks += n_layer
    if (f16) {
        g_tower_f16_encodes++
    }
}

//! A served encode's tail: the residual rows back to the host and the engage counters; false when the command buffer failed.
def private tw_readback(c : TwGpuCtx; ran : bool; var x : array<float>; d, n_layer : int64; f16 : bool = false) : bool {
    if (!ran) {
        return false
    }
    unsafe {
        memcpy(addr(x[0]), metal_buffer_contents(c.bx), uint64(c.npos * d * 4l))
    }
    tw_count_encode(c.npos, n_layer, f16)
    return true
}

//! The FastConformer chain's one command buffer as `encode` records it, then the readback into `x` with each route's engage counted and the context released; false, declined, where the command buffer failed.
def private fc_submit(var c : FcGpuCtx; var x : array<float>; d, n_layer : int64; encode : block<(enc : MetalComputeEncoder?) : void>) : bool {
    var err = ""
    let ran = with_compute_encoder(g_queue, err) $(enc : MetalComputeEncoder?) {   // nolint:PERF026 — the error-text fetch runs only on the failure leg
        invoke(encode, enc)
    }
    if (ran) {
        ensure_length(x, c.npos * d)
    }
    let served = tw_readback(c, ran, x, d, n_layer, c.f16w)
    if (served && c.dev_on) {
        g_tw_fc_dev_encodes++
    }
    fc_ctx_release(c)
    return served || tw_decline(MetalTowerDecline.gpu_error)
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
    return fc_submit(c, x, d, n_layer) $(enc) {
        for (l in range64(n_layer)) {
            fc_block(enc, c, invoke(layer, l), d, n_ff)
        }
    }
}

//! The canary FastConformer block loop as ONE command buffer on the f32 blob; the front seat runs the mel and the subsample front ahead of it, the final proj stays CPU.
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

var private @scratch g_tw_cn_bufs : array<MetalBuffer?>   //!< the canary front's pooled buffers of one encode, taken and given back inside it
var private g_tw_cn_front : St2SlabBase   //!< the served canary encoder's three front convs with their taps tap-major: the front kernels' layout

//! Canary's first conv and its two depthwise convs on the device as [9][ch] panels, one after another - the file keeps
//! them [ch][9] - once an encoder. Null where the buffer cannot be made.
[cold_path]
def private tw_cn_front_attach(t : CanaryEncoder) : MetalBuffer? {
    let key = hash_combine64(tw_plane_key(t.blob), uint64(t.c0_w))
    return g_tw_cn_front.buf if (st2_slab_current(g_tw_cn_front, key))
    st2_slab_free(g_tw_cn_front)
    let nch = t.n_sub_ch
    var taps = metal_new_buffer(g_dev, uint64(3l * 9l * nch * 4l))
    return null if (taps == null)
    unsafe {
        var p = reinterpret<float?>(metal_buffer_contents(taps))
        for (panel, woff in range64(3l), [t.c0_w, t.c2_w, t.c5_w]) {
            for (c in range64(nch)) {
                for (tap in range64(9l)) {
                    p[(panel * 9l + tap) * nch + c] = t.blob[woff + c * 9l + tap]
                }
            }
        }
    }
    g_tw_cn_front.buf = taps
    g_tw_cn_front.key = key
    return taps
}

//! The canary front whole, off the windowed frames `st.xw`, as ONE command buffer: the mel, the subsample stack and the
//! input projection into `st.x`, the CPU front's stages in its order. A stage's valid rows are its conv's image height:
//! a row past them reads as the zero the CPU masks it to. Answers the encoder rows, or -1 where the CPU front runs.
[hot_path, arch(at="../ARCHITECTURE_GPU_TOWER.md#tower-encode-chains")]
def private metal_canary_front(t : CanaryEncoder; var st : CanaryState) : int64 {   // nolint:STYLE038 — the dispatch chain is the shape
    if (!g_tw_env_tower || t.q8) {
        return -1l
    }
    let d = t.d_model
    let nch = t.n_sub_ch
    let n_mel = t.n_mel
    let n_fft = t.n_fft
    let n_bins = t.n_fb
    let n_len = st.n_len
    let t1 = conv_s2_out(n_len)
    let w1 = conv_s2_out(n_mel)
    let t2 = conv_s2_out(t1)
    let w2 = conv_s2_out(w1)
    let tt = conv_s2_out(t2)
    let w3 = conv_s2_out(w2)
    let l0 = conv_s2_out(st.n_valid)   //! the valid rows of each stage: the CPU front's length mask
    let l2 = conv_s2_out(l0)
    let feat = w3 * nch
    let bytes_y0 = uint64(t1 * w1 * nch * 4l)
    if (n_len < 1l || nch % 64l != 0l || feat % 32l != 0l || n_mel % 64l != 0l || n_fft % 32l != 0l || d % 64l != 0l
            || bytes_y0 > uint64(PK_FRONT_MAX_BYTES) || !metal_tower_init() || !pf_cn_front_ready()) {
        return -1l
    }
    var btaps = tw_cn_front_attach(t)
    return -1l if (btaps == null)
    residency_flush()
    let mp = round_up(n_len, 32l)
    let rows2 = t2 * w2
    let rows3 = tt * w3
    let mp2 = round_up(rows2, 32l)
    let mp3 = round_up(rows3, 32l)
    let mpt = round_up(tt, 32l)
    let dft_out_pad = round_up(2l * n_bins, 64l)
    let ldpow = round_up(n_bins, 32l)
    var bblob = plane_buffer(g_dev, unsafe(reinterpret<void?>(plane_at(t.blob, 0l))), uint64(length(t.blob) * 4l), t.image_map != null)
    let sizes = fixed_array(uint64(mp * n_fft * 4l), uint64(dft_out_pad * n_fft * 4l), uint64(mp * dft_out_pad * 4l),
        uint64(mp * ldpow * 4l), uint64(n_mel * ldpow * 4l), uint64(mp * n_mel * 4l), bytes_y0,
        uint64(mp2 * nch * 4l), uint64(mp2 * nch * 4l), uint64(mp3 * nch * 4l), uint64(mp3 * nch * 4l),
        uint64(mpt * feat * 4l), uint64(mpt * d * 4l))
    g_tw_cn_bufs |> resize(length(sizes))
    assume bufs = g_tw_cn_bufs
    for (b, n in bufs, sizes) {
        b = pool_acquire_pinned(g_tw_pool, g_dev, n)
    }
    upload_rows_strided(bufs[0], unsafe(addr<float const?>(st.xw[0])), n_len, n_fft, n_fft)
    zero_rows(bufs[0], n_len, mp, n_fft)
    tw_twiddle_panel(bufs[1], plane_at(t.twiddles, 0l), n_bins, n_fft, n_fft, dft_out_pad)
    upload_rows_strided(bufs[4], plane_at(t.filters, 0l), n_mel, n_bins, ldpow)
    unsafe {   //! the depthwise convs write their live rows alone, and the GEMM behind each reads the row pad: zero it, the buffers are pooled
        memset8(reinterpret<uint8?>(metal_buffer_contents(bufs[7])) + rows2 * nch * 4l, uint8(0), int((mp2 - rows2) * nch * 4l))
        memset8(reinterpret<uint8?>(metal_buffer_contents(bufs[9])) + rows3 * nch * 4l, uint8(0), int((mp3 - rows3) * nch * 4l))
    }
    var u_kfft = uniform_u32(uint(n_fft))
    var u_dft = uniform_u32(uint(dft_out_pad))
    var u_nbins = uniform_u32(uint(n_bins))
    var u_ldpow = uniform_u32(uint(ldpow))
    var u_totpow = uniform_u32(uint(mp * ldpow))
    var u_nmel = uniform_u32(uint(n_mel))
    var u_nch = uniform_u32(uint(nch))
    var u_feat = uniform_u32(uint(feat))
    var u_d = uniform_u32(uint(d))
    var u_tot2 = uniform_u32(uint(rows2 * nch))
    var u_tot3 = uniform_u32(uint(rows3 * nch))
    var u_totd = uniform_u32(uint(tt * d))
    var kn = CnMelNormArgs(nmel = uint(n_mel), nlen = uint(n_len), valid = uint(min(st.n_valid, n_len)), mfloor = 5.960464477539063e-08)   //! 2^-24, the CPU form's log guard
    var a0 = PkFrontArgs(iw = uint(n_mel), ih = uint(n_len), ow = uint(w1), oh = uint(t1), ch = uint(nch), total = uint(t1 * w1 * nch))
    var a2 = PkFrontArgs(iw = uint(w1), ih = uint(min(l0, t1)), ow = uint(w2), oh = uint(t2), ch = uint(nch), total = uint(rows2 * nch))
    var a5 = PkFrontArgs(iw = uint(w2), ih = uint(min(l2, t2)), ow = uint(w3), oh = uint(tt), ch = uint(nch), total = uint(rows3 * nch))
    var axf = PkFrontArgs(ow = uint(w3), oh = uint(tt), ch = uint(nch), total = uint(mpt * feat))
    let panel = uint64(9l * nch * 4l)
    var err = ""
    let ran = with_compute_encoder(g_queue, err) $(enc : MetalComputeEncoder?) {   // nolint:PERF026 — the error-text fetch runs only on the failure leg
        enc_f32_mm(enc, bufs[1], 0ul, bufs[0], bufs[2], 0ul, u_kfft, u_dft, mp, dft_out_pad)
        enc_q3a_pow(enc, bufs[2], bufs[3], u_nbins, u_dft, u_ldpow, u_totpow, mp * ldpow)
        enc_f32_mm(enc, bufs[4], 0ul, bufs[3], bufs[5], 0ul, u_ldpow, u_nmel, mp, n_mel)
        enc_cn_melnorm(enc, bufs[5], kn, n_mel)
        enc_pk_conv0(enc, bufs[5], bufs[6], btaps, 0ul, bblob, uint64(t.c0_b * 4l), a0, int64(a0.total))
        enc_pk_dw2d(enc, bufs[6], bufs[7], btaps, panel, bblob, uint64(t.c2_b * 4l), a2, int64(a2.total))
        enc_f32_mm(enc, bblob, uint64(t.c3_w * 4l), bufs[7], bufs[8], 0ul, u_nch, u_nch, mp2, nch)
        enc_bias_relu_rows(enc, bufs[8], 0ul, bblob, uint64(t.c3_b * 4l), u_nch, u_tot2, rows2 * nch)
        enc_pk_dw2d(enc, bufs[8], bufs[9], btaps, 2ul * panel, bblob, uint64(t.c5_b * 4l), a5, int64(a5.total))
        enc_f32_mm(enc, bblob, uint64(t.c6_w * 4l), bufs[9], bufs[10], 0ul, u_nch, u_nch, mp3, nch)
        enc_bias_relu_rows(enc, bufs[10], 0ul, bblob, uint64(t.c6_b * 4l), u_nch, u_tot3, rows3 * nch)
        enc_pk_xf(enc, bufs[10], bufs[11], axf, mpt * feat)
        enc_f32_mm(enc, bblob, uint64(t.pre_w * 4l), bufs[11], bufs[12], 0ul, u_feat, u_d, mpt, d)
        enc_add_bias_rows(enc, bufs[12], 0ul, bblob, uint64(t.pre_b * 4l), u_d, u_totd, tt * d)
    }
    if (ran) {
        ensure_length(st.x, tt * d)
        unsafe {
            memcpy(addr(st.x[0]), metal_buffer_contents(bufs[12]), uint64(tt * d * 4l))
        }
        g_tower_convs++
    }
    for (b, n in bufs, sizes) {
        pool_release(g_tw_pool, b, n)
    }
    for (u in fixed_array(u_kfft, u_dft, u_nbins, u_ldpow, u_totpow, u_nmel, u_nch, u_feat, u_d, u_tot2, u_tot3, u_totd)) {
        pool_release(g_upool, u, 4ul)
    }
    return ran ? tt : -1l
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

let private PK_FRONT_MAX_BYTES = 1073741824l   //!< the first conv's output a clip may ask of the device (~5 minutes of audio): a longer clip keeps the CPU front

var private g_tw_pk_front = true

//! Serve the parakeet subsample front on the device (`on`), or leave it to the CPU chain with the block seat
//! serving behind it; answers the setting it replaced.
def set_metal_parakeet_front(on : bool) : bool {
    let was = g_tw_pk_front
    g_tw_pk_front = on
    return was
}

//! The parakeet encode whole - the subsample front, the pre-projection and the block loop - as ONE command
//! buffer off the mel, the CPU chain's stages in its order. Answers the frame count, or -1 with nothing counted
//! where the front cannot serve: the CPU front runs then, and the block seat answers for itself.
[hot_path, arch(at="../ARCHITECTURE_GPU_TOWER.md#tower-encode-chains")]
def private metal_parakeet_encode(t : ParakeetModel; var st : ParakeetState) : int64 {
    if (!g_tw_env_tower || !g_tw_pk_front || t.q8) {
        return -1l
    }
    let d = t.n_state
    let nch = t.n_sub_ch
    let n_len = st.mel.n_len
    var a0 = pk_front_args(t.n_mel, n_len, nch)   //! mel -> y0 [t1 x w1 x nch]
    let t1 = int64(a0.oh)
    let w1 = int64(a0.ow)
    var a2 = pk_front_args(w1, t1, nch)           //! -> y2 [t2 x w2 x nch], the first 1x1 conv's rows
    let rows2 = int64(a2.oh) * int64(a2.ow)
    var a5 = pk_front_args(int64(a2.ow), int64(a2.oh), nch)   //! -> y5 [tt x w3 x nch], the second's
    let tt = int64(a5.oh)
    let w3 = int64(a5.ow)
    let rows3 = tt * w3
    let feat = w3 * nch
    let ww = 2l * tt - 1l
    let bytes_y0 = uint64(t1 * w1 * nch * 4l)
    if (n_len < 1l || tt > parakeet_max_frames() || nch % 64l != 0l || feat % 32l != 0l || bytes_y0 > uint64(PK_FRONT_MAX_BYTES)
            || !fc_shape_ok(d, 4l * d, t.n_head, t.n_conv_kernel, tt, ww)
            || !metal_tower_init() || !pf_fc_ready(true) || !pf_pk_front_ready()) {
        return -1l
    }
    parakeet_pos_table(t, st, tt)
    residency_flush()
    var c <- fc_ctx_make(t.blob, t.image_map != null, d, 4l * d, t.n_head, t.n_conv_kernel, t.eps, st.pos_emb, tt, ww, true)
    let mp2 = round_up(rows2, 32l)
    let mp3 = round_up(rows3, 32l)
    let bytes_mel = uint64(n_len * t.n_mel * 4l)
    let bytes_y2 = uint64(mp2 * nch * 4l)
    let bytes_y5 = uint64(mp3 * nch * 4l)
    var bmel = tw_take(c.scratch, bytes_mel)
    var by0 = tw_take(c.scratch, bytes_y0)
    var by2 = tw_take(c.scratch, bytes_y2)
    var by3 = tw_take(c.scratch, bytes_y2)
    var by5 = tw_take(c.scratch, bytes_y5)
    var by6 = tw_take(c.scratch, bytes_y5)
    var bxf = tw_take(c.scratch, uint64(c.mp * feat * 4l))
    var bfxh = c.f16w ? tw_take(c.scratch, uint64(max(mp2 * nch, c.mp * feat) * 2l)) : null
    unsafe {
        memcpy(metal_buffer_contents(bmel), addr(st.mel.data[0]), bytes_mel)
        //! the depthwise convs write their live rows alone, and the GEMM behind each reads the row pad: zero it, the buffers are pooled
        memset8(reinterpret<uint8?>(metal_buffer_contents(by2)) + rows2 * nch * 4l, uint8(0), int((mp2 - rows2) * nch * 4l))
        memset8(reinterpret<uint8?>(metal_buffer_contents(by5)) + rows3 * nch * 4l, uint8(0), int((mp3 - rows3) * nch * 4l))
    }
    var u_nch = uniform_u32(uint(nch))
    var u_feat = uniform_u32(uint(feat))
    var u_tot2 = uniform_u32(uint(rows2 * nch))
    var u_tot3 = uniform_u32(uint(rows3 * nch))
    var axf = PkFrontArgs(ow = a5.ow, oh = a5.oh, ch = a5.ch, total = uint(c.mp * feat))
    let f16w = c.f16w && bfxh != null
    let served = fc_submit(c, st.x, d, t.n_layer) $(enc) {
        enc_pk_conv0(enc, bmel, by0, c.bblob, uint64(t.c0_w * 4l), c.bblob, uint64(t.c0_b * 4l), a0, int64(a0.total))
        enc_pk_dw2d(enc, by0, by2, c.bblob, uint64(t.c2_w * 4l), c.bblob, uint64(t.c2_b * 4l), a2, int64(a2.total))
        tw_mm(enc, f16w, false, c.bwh, c.bblob, t.c3_w, by2, bfxh, by3, 0ul, u_nch, u_nch, mp2, nch, nch, true)
        enc_bias_relu_rows(enc, by3, 0ul, c.bblob, uint64(t.c3_b * 4l), u_nch, u_tot2, rows2 * nch)
        enc_pk_dw2d(enc, by3, by5, c.bblob, uint64(t.c5_w * 4l), c.bblob, uint64(t.c5_b * 4l), a5, int64(a5.total))
        tw_mm(enc, f16w, false, c.bwh, c.bblob, t.c6_w, by5, bfxh, by6, 0ul, u_nch, u_nch, mp3, nch, nch, true)
        enc_bias_relu_rows(enc, by6, 0ul, c.bblob, uint64(t.c6_b * 4l), u_nch, u_tot3, rows3 * nch)
        enc_pk_xf(enc, by6, bxf, axf, c.mp * feat)
        tw_mm(enc, f16w, false, c.bwh, c.bblob, t.pre_w, bxf, bfxh, c.bx, 0ul, u_feat, c.u_d, c.mp, d, feat, true)
        enc_add_bias_rows(enc, c.bx, 0ul, c.bblob, uint64(t.pre_b * 4l), c.u_d, c.u_totd, tt * d)
        for (l in range64(t.n_layer)) {
            fc_block(enc, c, pk_fc_layer(pk_layer_offsets(t, l)), d, 4l * d)
        }
    }
    pool_release(g_upool, u_nch, 4ul)
    pool_release(g_upool, u_feat, 4ul)
    pool_release(g_upool, u_tot2, 4ul)
    pool_release(g_upool, u_tot3, 4ul)
    if (!served) {
        return -1l
    }
    g_tower_convs++
    return tt
}

//! the DFT's transposed twiddle panel [rows_pad x kstride]: row n (a bin, real rows then imaginary) the twiddles' column n, pads zero
def private tw_twiddle_panel(var buf : MetalBuffer?; twiddles : float const?; n_bins, n_fft, kstride, rows_pad : int64) {
    unsafe {
        var tp = reinterpret<float?>(metal_buffer_contents(buf))
        for (i in range64(rows_pad * kstride)) {
            tp[i] = 0.0
        }
        for (n in range64(2l * n_bins)) {
            for (k in range64(n_fft)) {
                tp[n * kstride + k] = twiddles[k * 2l * n_bins + n]
            }
        }
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
    let t1 = conv_s2_out(n_len)
    let w1 = conv_s2_out(n_mel)
    let t2 = conv_s2_out(t1)
    let w2 = conv_s2_out(w1)
    let feat = w2 * t.sub_ch1
    let npos = t2
    if (n_fft % 32l != 0l || feat % 32l != 0l || feat != t.d_model || t.sub_ch0 % 64l != 0l || t.sub_ch0 != n_mel
            || !metal_tower_init() || !pf_g4a_ready() || !pf_g4a_front_ready()) {
        return -1l
    }
    residency_flush()
    let d = t.d_model
    let mpd = round_up(n_len, 32l)
    let dft_out_pad = round_up(2l * n_bins, 64l)
    let mag_k_pad = round_up(n_bins, 32l)
    let rows0 = t1 * w1
    let mp0 = round_up(rows0, 32l)
    let conv0_k_pad = 32l
    let rows1 = t2 * w2
    let mp1 = round_up(rows1, 32l)
    let kd1 = t.sub_ch0 * 9l
    let conv1_cout_pad = round_up(t.sub_ch1, 64l)
    var c <- g4a_ctx_make(t, npos)
    var tl <- g4a_tail_make(t, c)
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
    upload_rows_padded(bxw, unsafe(addr(s.xw[0])), n_len, mpd, n_fft)   // the DFT GEMM walks mpd rows
    tw_twiddle_panel(btw, plane_at(t.twiddles, 0l), n_bins, n_fft, n_fft, dft_out_pad)
    upload_rows_strided(bfil, plane_at(t.filters, 0l), n_mel, n_bins, mag_k_pad)   // the mel filterbank [n_mel x mag_k_pad], k-padded
    upload_rows_strided(bw0, plane_at(t.blob, t.c0_w), t.sub_ch0, 9l, conv0_k_pad)
    upload_rows_padded(bw1, unsafe(reinterpret<void?>(plane_at(t.blob, t.c1_w))), t.sub_ch1, conv1_cout_pad, kd1)
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
        g4a_tail_body(enc, t, c, tl)
    }
    g4a_tail_land(t, c, tl, ran, s)
    var served = false
    if (ran) {
        ensure_length(s.x, npos * d)
        unsafe {
            memcpy(addr(s.x[0]), metal_buffer_contents(c.bx), uint64(npos * d * 4l))
        }
        tw_count_encode(npos, t.n_layer, false)
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

//! the gemma4v chain's two ends on the device: the stem's columns and position rows ahead of the blocks, the projector
//! tail's rows behind them
struct private G4vEnds {
    tail : bool             //!< the tail rides this encode's command buffer
    n_cols : int64          //!< the patch grid's width
    nout : int64            //!< soft tokens: the pooled grid's rows
    mpo : int64             //!< `nout` on the GEMM's 32-row lattice
    bx0 : MetalBuffer?      //!< the stem's columns [mp x patch_dim]
    bpos : MetalBuffer?     //!< each row's x + y position-table rows, summed
    bpool : MetalBuffer?    //!< the pooled rows [mpo x d]
    bclamp : MetalBuffer?   //!< their clamped copy, the projection's input
    bout : MetalBuffer?     //!< the soft tokens [mpo x proj_dim]
    bstd_b : MetalBuffer?   //!< the standardize bias row (zeros where the tower has none)
    bstd_m : MetalBuffer?   //!< its scale row (ones)
    u_pd : MetalBuffer?
    u_proj : MetalBuffer?
    u_totpool : MetalBuffer?
    u_totout : MetalBuffer?
    bytes_x0 : uint64
    bytes_pos : uint64
    bytes_pool : uint64
    bytes_out : uint64
    bytes_std : uint64
}

//! The stem's operands up, and the tail's rows where the lever allows, its kernels are compiled, the grid pools whole and
//! the projection sits on its GEMM's lattice.
def private g4v_ends_make(t : Gemma4vTower; s : Gemma4vState; npos, mp : int64) : G4vEnds {
    let d = t.n_embd
    let n_cols = s.grid_w
    var e = G4vEnds(n_cols = n_cols, bytes_x0 = uint64(mp * t.patch_dim * 4l), bytes_pos = uint64(mp * d * 4l))
    e.bx0 = pool_acquire_pinned(g_tw_pool, g_dev, e.bytes_x0)
    e.bpos = pool_acquire_pinned(g_tw_pool, g_dev, e.bytes_pos)
    e.u_pd = uniform_u32(uint(t.patch_dim))
    unsafe {
        upload_rows_padded(e.bx0, addr<float const?>(s.x0[0]), npos, mp, t.patch_dim)
        var pp = reinterpret<float?>(metal_buffer_contents(e.bpos))
        let tbl = plane_at(t.blob, t.pos_w)
        for (pi in range64(npos)) {
            let ex = tbl + (pi % n_cols) * d
            let ey = tbl + (t.pos_entries + pi / n_cols) * d
            for (i in range64(d)) {
                pp[pi * d + i] = ex[i] + ey[i]
            }
        }
    }
    e.tail = (g_tw_tail && pf_tw_tail_ready() && n_cols % t.merge == 0l && s.grid_h % t.merge == 0l
        && d <= int64(TW_ONES_DIM) && t.proj_dim % 64l == 0l)
    return e if (!e.tail)
    e.nout = (n_cols / t.merge) * (s.grid_h / t.merge)
    e.mpo = round_up(e.nout, 32l)
    e.bytes_pool = uint64(e.mpo * d * 4l)
    e.bytes_out = uint64(e.mpo * t.proj_dim * 4l)
    e.bytes_std = uint64(d * 4l)
    e.bpool = pool_acquire_pinned(g_tw_pool, g_dev, e.bytes_pool)
    e.bclamp = pool_acquire_pinned(g_tw_pool, g_dev, e.bytes_pool)
    zero_rows(e.bpool, e.nout, e.mpo, d)   //! the pool and the clamp write the live rows alone and the projection reads the row pad: the buffers are pooled
    zero_rows(e.bclamp, e.nout, e.mpo, d)
    e.bout = pool_acquire_pinned(g_tw_pool, g_dev, e.bytes_out)
    e.bstd_b = pool_acquire_pinned(g_tw_pool, g_dev, e.bytes_std)
    e.bstd_m = pool_acquire_pinned(g_tw_pool, g_dev, e.bytes_std)
    e.u_proj = uniform_u32(uint(t.proj_dim))
    e.u_totpool = uniform_u32(uint(e.nout * d))
    e.u_totout = uniform_u32(uint(e.nout * t.proj_dim))
    unsafe {
        var bp = reinterpret<float?>(metal_buffer_contents(e.bstd_b))
        var mp_ = reinterpret<float?>(metal_buffer_contents(e.bstd_m))
        for (i in range64(d)) {
            bp[i] = t.has_std ? plane_at(t.blob, t.std_bias)[i] : 0.0
            mp_[i] = t.has_std ? plane_at(t.blob, t.std_scale)[i] : 1.0
        }
    }
    return e
}

//! one GEMM off the tower's weights: the halfword twin where the tensor is stored bf16, the f32 blob otherwise

//! The stem on the device: the patch conv over the columns, then each row's position rows.
[arch(at="../ARCHITECTURE_GPU_TOWER.md#tower-encode-chains")]
def private g4v_stem_body(enc : MetalComputeEncoder?; t : Gemma4vTower; e : G4vEnds; bblob, bwblob, bx : MetalBuffer?;
                          u_d, u_totd : MetalBuffer?; mp, npos : int64) {
    let d = t.n_embd
    tw_mm(enc, false, t.patch_bf16, bwblob, bblob, t.patch_w, e.bx0, null, bx, 0ul, e.u_pd, u_d, mp, d, t.patch_dim, false)
    enc_add(enc, bx, 0ul, e.bpos, 0ul, u_totd, npos * d)
}

//! The CPU tail's steps behind the blocks: the grid mean pool, the sqrt(d) scale and the standardize, the weightless
//! rms, then the projection between its two clamps.
[arch(at="../ARCHITECTURE_GPU_TOWER.md#tower-encode-chains")]
def private g4v_tail_body(enc : MetalComputeEncoder?; t : Gemma4vTower; e : G4vEnds; bblob, bwblob, bx : MetalBuffer?;
                          u_d, u_eps, u_in, u_out : MetalBuffer?) {
    return if (!e.tail)
    let d = t.n_embd
    let nout = e.nout
    let cb = uint64(t.proj_clamp * 4l)
    enc_tw_pool2d(enc, bx, e.bpool, TwPool2dArgs(width = uint(d), gw = uint(e.n_cols), k = uint(t.merge),
        ow = uint(e.n_cols / t.merge), total = uint(nout * d)), nout * d)
    enc_tw_affine_rows(enc, e.bpool, e.bstd_b, e.bstd_m, TwAffineArgs(width = uint(d), total = uint(nout * d), scale = sqrt(float(d))), nout * d)
    pf_enc_rms(enc, e.bpool, 0ul, g_tw_ones, e.bpool, u_d, u_eps, nout)
    enc_clamp(enc, e.bpool, 0ul, e.bclamp, 0ul, bblob, cb, u_in, e.u_totpool, nout * d)
    tw_mm(enc, false, t.proj_bf16, bwblob, bblob, t.proj_w, e.bclamp, null, e.bout, 0ul, u_d, e.u_proj, e.mpo, t.proj_dim, d, false)
    enc_clamp(enc, e.bout, 0ul, e.bout, 0ul, bblob, cb, u_out, e.u_totout, nout * t.proj_dim)
}

//! the tail's soft tokens into `s.tail` where the command buffer ran, the CPU tail told to stand down; the ends' buffers released
def private g4v_ends_land(t : Gemma4vTower; var e : G4vEnds; ran : bool; var s : Gemma4vState) {
    pool_release(g_tw_pool, e.bx0, e.bytes_x0)
    pool_release(g_tw_pool, e.bpos, e.bytes_pos)
    pool_release(g_upool, e.u_pd, 4ul)
    return if (!e.tail)
    if (ran) {
        ensure_length(s.tail, e.nout * t.proj_dim)
        unsafe {
            memcpy(addr(s.tail[0]), metal_buffer_contents(e.bout), uint64(e.nout * t.proj_dim * 4l))
        }
        s.tail_ready = true
        g_tower_tail_encodes++
    }
    pool_release(g_tw_pool, e.bpool, e.bytes_pool)
    pool_release(g_tw_pool, e.bclamp, e.bytes_pool)
    pool_release(g_tw_pool, e.bout, e.bytes_out)
    pool_release(g_tw_pool, e.bstd_b, e.bytes_std)
    pool_release(g_tw_pool, e.bstd_m, e.bytes_std)
    pool_release(g_upool, e.u_proj, 4ul)
    pool_release(g_upool, e.u_totpool, 4ul)
    pool_release(g_upool, e.u_totout, 4ul)
}

// The gemma4v ViT block loop as ONE command buffer on the file's planes (bf16 GEMMs through the
// bf16 mul_mm, f32 norms/clamps) — the CPU g4v_block chain dispatch for dispatch, the attention
// trio at uend = npos / scale 1. The stem and the tail ride the same command buffer (`g4v_ends_make`); a q8-lane tower declines (quant_mode).
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
            || t.layer_w_stride % 8l != 0l || t.patch_dim % 32l != 0l || s.grid_w * s.grid_h != npos) {
        return tw_decline(MetalTowerDecline.shape)
    }
    if (!metal_tower_init()) {   // per-encode: re-arms after any shutdown seat
        return tw_decline(MetalTowerDecline.device)
    }
    residency_flush()   // pin/commit before the first submission (the post-CPU-window slack)
    let mp = round_up(npos, 32l)
    let nk64 = round_up(npos, 64l)
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
    var ends = g4v_ends_make(t, s, npos, mp)
    zero_rows(bk, npos, nk64, d)
    gemma4v_pack_rope_tabs(s, npos, pairs, unsafe(reinterpret<float?>(metal_buffer_contents(btabs))))
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
        g4v_stem_body(enc, t, ends, bblob, bwblob, bx, u_d, u_totd, mp, npos)
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
        g4v_tail_body(enc, t, ends, bblob, bwblob, bx, u_d, u_eps, u_in, u_out)
    }
    var served = false
    if (ran) {
        if (!ends.tail) {   // with the tail on the device the soft tokens alone come back
            unsafe {
                memcpy(addr(s.x[0]), metal_buffer_contents(bx), uint64(npos * d * 4l))
            }
        }
        tw_count_encode(npos, t.n_layer, false)
        g_tower_convs++
        served = true
    }
    g4v_ends_land(t, ends, ran, s)
    pool_release(g_tw_pool, bx, bytes_row)
    pool_release(g_tw_pool, bxb, bytes_row)
    pool_release(g_tw_pool, bxb2, bytes_row)
    pool_release(g_tw_pool, bq, bytes_row)
    pool_release(g_tw_pool, bk, bytes_rowk)
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

//! the gemma3v projector tail's device rows: the pooled rows, the soft tokens, the soft-norm's weight row
struct private G3vTail {
    on : bool           //!< the tail rides this encode's command buffer
    n_side : int64      //!< the patch grid's side
    nout : int64        //!< soft tokens: the pooled grid's rows
    mpo : int64         //!< `nout` on the GEMM's 32-row lattice
    bpool : MetalBuffer?
    bout : MetalBuffer?
    bsoft : MetalBuffer?
    u_proj : MetalBuffer?
    bytes_pool : uint64
    bytes_out : uint64
    bytes_soft : uint64
}

//! The tail rides the chain where the lever allows, its kernels compiled, its GEMM on the f32 tile's lattice (K a multiple
//! of 32, N of 64) and the patch grid pools whole.
def private g3v_tail_make(t : Gemma3vTower; npos : int64) : G3vTail {
    let d = t.n_embd
    let n_side = t.image_px / t.patch
    var tl = G3vTail(n_side = n_side, nout = (n_side / t.merge) * (n_side / t.merge))
    tl.on = (g_tw_tail && pf_tw_tail_ready() && d % 32l == 0l && t.proj_dim % 64l == 0l
        && n_side * n_side == npos && n_side % t.merge == 0l)
    return tl if (!tl.on)
    tl.mpo = round_up(tl.nout, 32l)
    tl.bytes_pool = uint64(tl.mpo * d * 4l)
    tl.bytes_out = uint64(tl.mpo * t.proj_dim * 4l)
    tl.bytes_soft = uint64(d * 4l)
    tl.bpool = pool_acquire_pinned(g_tw_pool, g_dev, tl.bytes_pool)
    tl.bout = pool_acquire_pinned(g_tw_pool, g_dev, tl.bytes_out)
    zero_rows(tl.bpool, tl.nout, tl.mpo, d)   //! the pool writes the live rows alone and the projection reads the row pad: the buffers are pooled
    tl.bsoft = pool_acquire_pinned(g_tw_pool, g_dev, tl.bytes_soft)
    tl.u_proj = uniform_u32(uint(t.proj_dim))
    unsafe(memcpy(metal_buffer_contents(tl.bsoft), plane_at(t.blob, t.soft_norm_w), tl.bytes_soft))
    return tl
}

//! The CPU tail's four steps behind the blocks: the post-norm, the grid mean pool, the weighted rms, the projection.
[arch(at="../ARCHITECTURE_GPU_TOWER.md#tower-encode-chains")]
def private g3v_tail_body(enc : MetalComputeEncoder?; t : Gemma3vTower; tl : G3vTail; bblob, bx, bs : MetalBuffer?;
                          u_d, u_eps : MetalBuffer?; npos : int64) {
    return if (!tl.on)
    let d = t.n_embd
    enc_ln(enc, bx, bblob, uint64(t.post_ln_w * 4l), bblob, uint64(t.post_ln_b * 4l), bs, 0ul, u_d, u_eps, npos)
    enc_tw_pool2d(enc, bs, tl.bpool, TwPool2dArgs(width = uint(d), gw = uint(tl.n_side), k = uint(t.merge),
        ow = uint(tl.n_side / t.merge), total = uint(tl.nout * d)), tl.nout * d)
    pf_enc_rms(enc, tl.bpool, 0ul, tl.bsoft, tl.bpool, u_d, u_eps, tl.nout)
    enc_f32_mm(enc, bblob, uint64(t.proj_w * 4l), tl.bpool, tl.bout, 0ul, u_d, tl.u_proj, tl.mpo, t.proj_dim)
}

//! the tail's soft tokens into `s.tail` where the command buffer ran, the CPU tail told to stand down; its buffers released
def private g3v_tail_land(t : Gemma3vTower; var tl : G3vTail; ran : bool; var s : Gemma3vState) {
    return if (!tl.on)
    if (ran) {
        ensure_length(s.tail, tl.nout * t.proj_dim)
        unsafe {
            memcpy(addr(s.tail[0]), metal_buffer_contents(tl.bout), uint64(tl.nout * t.proj_dim * 4l))
        }
        s.tail_ready = true
        g_tower_tail_encodes++
    }
    pool_release(g_tw_pool, tl.bpool, tl.bytes_pool)
    pool_release(g_tw_pool, tl.bout, tl.bytes_out)
    pool_release(g_tw_pool, tl.bsoft, tl.bytes_soft)
    pool_release(g_upool, tl.u_proj, 4ul)
}

// The gemma3v SigLIP block loop as ONE command buffer on the f32 blob — LN(w,b), biased f32 GEMMs,
// scaled attention, GELU. The 72-wide heads restride to the attention tiles' 128 (zero pads exact,
// scale stays 1/sqrt(72)); ffn serves at the layout's ff_pad. The tail rides the same command buffer (`g3v_tail_make`), the patch im2col stays CPU; q8 declines.
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
    let hs_pad = max(64l, round_up(hs, 32l))
    if (d % 64l != 0l || ff % 64l != 0l || hs > 128l) {
        return tw_decline(MetalTowerDecline.shape)
    }
    if (!metal_tower_init()) {   // per-encode: re-arms after any shutdown seat
        return tw_decline(MetalTowerDecline.device)
    }
    residency_flush()   // pin/commit before the first submission (the post-CPU-window slack)
    let mp = round_up(npos, 32l)
    let nk64 = round_up(npos, 64l)
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
    var tl = g3v_tail_make(t, npos)
    var err = ""
    let ran = with_compute_encoder(g_queue, err) $(enc : MetalComputeEncoder?) {   // nolint:PERF026 — the error-text fetch runs only on the failure leg
        //! the stem: padded patch GEMM + conv bias + the learned positions (all row-aligned)
        enc_f32_mm(enc, bblob, uint64(t.patch_w * 4l), bx0, bx, 0ul, u_pd, u_d, mp, d)
        enc_add_bias_rows(enc, bx, 0ul, bblob, uint64(t.patch_b * 4l), u_d, u_totd, npos * d)
        enc_add(enc, bx, 0ul, bblob, uint64(t.pos_w * 4l), u_totd, npos * d)
        for (l in range64(t.n_layer)) {
            let o = gemma3v_block_offs(t, l)
            enc_ln(enc, bx, bblob, uint64(o.ln1_w * 4l), bblob, uint64(o.ln1_b * 4l), bxb, 0ul, u_d, u_eps, npos)
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
            enc_ln(enc, bx, bblob, uint64(o.ln2_w * 4l), bblob, uint64(o.ln2_b * 4l), bxb, 0ul, u_d, u_eps, npos)
            tw_mm(enc, f16w, false, bwh, bblob, o.w[GEMMA3V_GEMM_UP], bxb, bxh, bh, 0ul, u_d, u_ff, mp, ff, d, true)
            enc_add_bias_rows(enc, bh, 0ul, bblob, uint64(o.b[GEMMA3V_GEMM_UP] * 4l), u_ff, u_totff, npos * ff)
            enc_gelu_lut(enc, bh, 0ul, u_totff, npos * ff)
            tw_mm(enc, f16w, false, bwh, bblob, o.w[GEMMA3V_GEMM_DOWN], bh, bxh, bxb2, 0ul, u_ff, u_d, mp, d, ff, true)
            enc_add_bias_rows(enc, bxb2, 0ul, bblob, uint64(o.b[GEMMA3V_GEMM_DOWN] * 4l), u_d, u_totd, npos * d)
            enc_add(enc, bx, 0ul, bxb2, 0ul, u_totd, npos * d)
        }
        g3v_tail_body(enc, t, tl, bblob, bx, bxb, u_d, u_eps, npos)
    }
    var served = false
    if (ran) {
        if (!tl.on) {   // with the tail on the device the soft tokens alone come back: the block rows stay there
            unsafe {
                memcpy(addr(s.x[0]), metal_buffer_contents(bx), uint64(npos * d * 4l))
            }
        }
        tw_count_encode(npos, t.n_layer, f16w)
        served = true
    }
    g3v_tail_land(t, tl, ran, s)
    pool_release(g_tw_pool, bx, bytes_row)
    pool_release(g_tw_pool, bxb, bytes_row)
    pool_release(g_tw_pool, bxb2, bytes_row)
    pool_release(g_tw_pool, braw, bytes_row)
    pool_release(g_tw_pool, bq, bytes_rowp)
    pool_release(g_tw_pool, bk, bytes_rowk)
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
        enc_cvt_half(enc, bx, bxh, rows * kdim)
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
    let hs_pad = max(64l, round_up(hs, 32l))
    //! %4 npos: the x4 merge reshape is the tap/tail GEMMs' row view; %64 patch_dim: the stem GEMM's K
    if (d % 64l != 0l || ff % 64l != 0l || hs > 128l || hs % 2l != 0l || proj % 64l != 0l || npos % 4l != 0l || t.patch_dim % 64l != 0l) {
        return tw_decline(MetalTowerDecline.shape)
    }
    if (!metal_tower_init()) {   // per-encode: re-arms after any shutdown seat
        return tw_decline(MetalTowerDecline.device)
    }
    residency_flush()   // pin/commit before the first submission (the post-CPU-window slack)
    let mp = round_up(npos, 32l)
    let nk64 = round_up(npos, 64l)
    let dp = t.n_head * hs_pad   // the padded attention width
    let half = hs / 2l
    let nout = npos / 4l
    let mp4 = round_up(nout, 32l)
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
            enc_ln(enc, bx, bblob, uint64(o.ln1_w * 4l), bblob, uint64(o.ln1_b * 4l), bxb, 0ul, u_d, u_eps, npos)
            let wqe = o.w[QWEN3V_GEMM_QKV]   //! the fused qkv rows: q, then k at d·d, v at 2d·d
            tw_mm(enc, f16w, bf16w, bwh, bblob, wqe, bxb, bxh, braw, 0ul, u_d, u_d, mp, d, d, true)
            enc_add_bias_rows(enc, braw, 0ul, bblob, uint64(o.qkv_b * 4l), u_d, u_totd, npos * d)
            enc_rope(enc, braw, 0ul, bcos, bsin, u_d, u_hs, u_np_rope, u_neox, null, null, npos * d / 2l)
            enc_head_restride(enc, braw, 0ul, bq, 0ul, u_hs, u_hsp, u_totp, npos * t.n_head * hs_pad)
            tw_mm(enc, f16w, bf16w, bwh, bblob, wqe + d * d, bxb, bxh, braw, 0ul, u_d, u_d, mp, d, d, false)
            enc_add_bias_rows(enc, braw, 0ul, bblob, uint64((o.qkv_b + d) * 4l), u_d, u_totd, npos * d)
            enc_rope(enc, braw, 0ul, bcos, bsin, u_d, u_hs, u_np_rope, u_neox, null, null, npos * d / 2l)
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
            enc_ln(enc, bx, bblob, uint64(o.ln2_w * 4l), bblob, uint64(o.ln2_b * 4l), bxb, 0ul, u_d, u_eps, npos)
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
                enc_ln(enc, bx, bblob, uint64(fo * 4l), bblob, uint64((fo + 4l * d) * 4l), bm4, 0ul, u_d4, u_eps, nout)
                tw_mm(enc, f16w, bf16w, bwh, bblob, wo, bm4, bxh, bm4b, 0ul, u_d4, u_d4, mp4, 4l * d, 4l * d, true)
                enc_add_bias_rows(enc, bm4b, 0ul, bblob, uint64((fo + 8l * d) * 4l), u_d4, u_tot4, nout * 4l * d)
                enc_gelu_lut(enc, bm4b, 0ul, u_tot4, nout * 4l * d)
                tw_mm(enc, f16w, bf16w, bwh, bblob, wo + 16l * d * d, bm4b, bxh, btap, uint64(k * mp4 * proj * 4l), u_d4, u_pr, mp4, proj, 4l * d, true)
                enc_add_bias_rows(enc, btap, uint64(k * mp4 * proj * 4l), bblob, uint64((fo + 12l * d) * 4l), u_pr, u_totpr, nout * proj)
            }
        }
        // the tail: post-LN over [npos x d], then the merger MLP over the [nout x 4d] view
        enc_ln(enc, bx, bblob, uint64(t.post_ln_w * 4l), bblob, uint64(t.post_ln_b * 4l), bxb, 0ul, u_d, u_eps, npos)
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
        tw_count_encode(npos, t.n_layer, f16w || bf16w)
        served = true
    }
    pool_release(g_tw_pool, bx, bytes_row)
    pool_release(g_tw_pool, bxb, bytes_row)
    pool_release(g_tw_pool, bxb2, bytes_row)
    pool_release(g_tw_pool, braw, bytes_row)
    pool_release(g_tw_pool, bq, bytes_rowp)
    pool_release(g_tw_pool, bk, bytes_rowk)
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
    let hs_pad = max(64l, round_up(hs, 32l))
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
    let mp = round_up(npos, 32l)
    let nk64 = round_up(npos, 64l)
    let dp = t.n_head * hs_pad   // the padded attention width (the full layers' slab)
    let half = hs / 2l
    let nout = npos / 4l
    let mp4 = round_up(nout, 32l)
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
    upload_rows_padded(bx0, unsafe(addr(s.x0[0])), npos, mp, pd)
    zero_rows(bk, npos, nk64, dp)
    unsafe {
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
        var kg = RowGatherArgs(c = uint(d), add_b = 0u, add_c = 0u, total = uint(npos * d), scale = 1.0)
        enc_row_gather(enc, braw, 0ul, bsrc, g_one, 0ul, g_one, 0ul, bx, kg, npos * d)
        for (l in range64(t.n_layer)) {
            let o = qwen25v_block_offs(t, l)
            let full = qwen25v_layer_full(t, l)
            var bw_ln1 = upload_region(unsafe(reinterpret<void?>(plane_at(t.blob, o.ln1_w))), uint64(d * 4l))
            var bw_ln2 = upload_region(unsafe(reinterpret<void?>(plane_at(t.blob, o.ln2_w))), uint64(d * 4l))
            pf_enc_rms(enc, bx, 0ul, bw_ln1, bxb, u_d, u_eps, npos)
            //! the window kernel reads q/k/v compact (bq/bk/bv direct); full layers restride from braw
            tw_mm(enc, f16w, bf16w, bwh, bblob, o.w[QWEN25V_GEMM_Q], bxb, bxh, full ? braw : bq, 0ul, u_d, u_d, mp, d, d, true)
            enc_add_bias_rows(enc, full ? braw : bq, 0ul, bblob, uint64(o.b[QWEN25V_GEMM_Q] * 4l), u_d, u_totd, npos * d)
            enc_rope(enc, full ? braw : bq, 0ul, bcos, bsin, u_d, u_hs, u_np_rope, u_neox, null, null, npos * d / 2l)
            if (full) {
                enc_head_restride(enc, braw, 0ul, bq, 0ul, u_hs, u_hsp, u_totp, npos * t.n_head * hs_pad)
            }
            tw_mm(enc, f16w, bf16w, bwh, bblob, o.w[QWEN25V_GEMM_K], bxb, bxh, full ? braw : bk, 0ul, u_d, u_d, mp, d, d, false)
            enc_add_bias_rows(enc, full ? braw : bk, 0ul, bblob, uint64(o.b[QWEN25V_GEMM_K] * 4l), u_d, u_totd, npos * d)
            enc_rope(enc, full ? braw : bk, 0ul, bcos, bsin, u_d, u_hs, u_np_rope, u_neox, null, null, npos * d / 2l)
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
        tw_count_encode(npos, t.n_layer, f16w || bf16w)
        served = true
    }
    pool_release(g_tw_pool, bx, bytes_row)
    pool_release(g_tw_pool, bxb, bytes_row)
    pool_release(g_tw_pool, bxb2, bytes_row)
    pool_release(g_tw_pool, braw, bytes_row)
    pool_release(g_tw_pool, bq, bytes_rowp)
    pool_release(g_tw_pool, bk, bytes_rowk)
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

var private g_tw_conv1_pad : St2SlabBase   //!< the served whisper-class tower's first conv with its columns zero-padded to the 32 lattice

//! The first conv's weight rows at `k1` columns - the file's 3 x n_mel and zeros past them - on the device, once a tower:
//! the im2col pass pads its rows to the same lattice with zeros. Null where the buffer cannot be made.
[cold_path]
def private tw_conv1_pad_attach(t : AudioTower; k1 : int64) : MetalBuffer? {
    let key = hash_combine64(tw_plane_key(t.fblob), uint64(t.conv1_w_off))
    return g_tw_conv1_pad.buf if (st2_slab_current(g_tw_conv1_pad, key))
    st2_slab_free(g_tw_conv1_pad)
    let d = t.d_model
    let k = 3l * t.n_mel
    var pad = metal_new_buffer(g_dev, uint64(d * k1 * 4l))
    return null if (pad == null)
    unsafe {
        var p = reinterpret<float?>(metal_buffer_contents(pad))
        for (r in range64(d)) {
            for (c in range64(k1)) {
                p[r * k1 + c] = c < k ? t.fblob[t.conv1_w_off + r * k + c] : 0.0
            }
        }
    }
    g_tw_conv1_pad.buf = pad
    g_tw_conv1_pad.key = key
    return pad
}

//! the stem's second conv as a GEMM over its im2col rows: the device q8 blob's region on a q8 tower, the f32 blob otherwise
def private conv2_mm(enc : MetalComputeEncoder?; t : AudioTower; bblob, bx2, bx2h, bxo, u_3d, u_d : MetalBuffer?; mp2 : int64) {
    let d = t.d_model
    if (!t.q8) {
        enc_f32_mm(enc, bblob, uint64(t.conv2_w_off * 4l), bx2, bxo, 0ul, u_3d, u_d, mp2, d)
        return
    }
    if (bx2h != null) {
        enc_cvt_half(enc, bx2, bx2h, mp2 * 3l * d)
    }
    pf_enc_q8_mm(enc, g_tw_q8.buf, g_tw_q8_conv2, bx2, bx2h, bxo, u_3d, u_d, mp2, d, 3l * d)
}

// The whisper conv frontend for one mel chunk: im2col + conv GEMMs + bias + GELU + pos-add,
// one command buffer, s.x readback. BEST-EFFORT inside the tower knob: any decline is a
// silent false (no counter noise — the CPU conv serves and the blocks still ride the GPU).
[hot_path]
def private metal_conv_frontend(t : AudioTower; var s : EncoderState; mel : MelChunk) : bool {   // nolint:STYLE038 — the dispatch chain is the shape
    if (!g_tw_env_tower) {
        return false
    }
    let tin = AUDIO_CHUNK_FRAMES
    let tt = t.n_ctx
    let d = t.d_model
    let nm = t.n_mel
    // the first conv's 3 x n_mel columns ride the GEMM's 32 lattice: 80 mels' 240 pad to 256 through a padded weight copy
    let k1 = round_up(3l * nm, 32l)
    if (d % 64l != 0l || (3l * d) % 32l != 0l || !metal_tower_init() || (t.q8 && !tw_q8_attach(t))) {
        return false
    }
    var bw1 = k1 == 3l * nm ? null : tw_conv1_pad_attach(t, k1)
    if (k1 != 3l * nm && bw1 == null) {
        return false
    }
    let mp1 = round_up(tin, 32l)
    let mp2 = round_up(tt, 32l)
    let mapped = t.image_map != null
    var bblob = plane_buffer(g_dev, unsafe(reinterpret<void?>(plane_at(t.fblob, 0l))), uint64(length(t.fblob) * 4l), mapped)
    let bytes_mel = uint64(nm * tin * 4l)
    let bytes_x1 = uint64(mp1 * k1 * 4l)
    let bytes_y1 = uint64(mp1 * d * 4l)
    let bytes_x2 = uint64(mp2 * 3l * d * 4l)
    let bytes_xo = uint64(mp2 * d * 4l)
    var bmel = pool_acquire_pinned(g_tw_pool, g_dev, bytes_mel)
    var bx1 = pool_acquire_pinned(g_tw_pool, g_dev, bytes_x1)
    var by1 = pool_acquire_pinned(g_tw_pool, g_dev, bytes_y1)
    var bx2 = pool_acquire_pinned(g_tw_pool, g_dev, bytes_x2)
    var bxo = pool_acquire_pinned(g_tw_pool, g_dev, bytes_xo)
    var bx2h = (t.q8 && pf_q8_mm_half()) ? pool_acquire_pinned(g_tw_pool, g_dev, bytes_x2 / 2ul) : null
    unsafe {
        memcpy(metal_buffer_contents(bmel), addr(mel.data[0]), bytes_mel)
    }
    var u_nm3 = uniform_u32(uint(k1))
    var u_d = uniform_u32(uint(d))
    var u_3d = uniform_u32(uint(3l * d))
    var u_totd1 = uniform_u32(uint(tin * d))
    var u_totd2 = uniform_u32(uint(tt * d))
    var ka1 = St2ConvArgs(cin = uint(nm), k = 3u, stride = 1u, pad_l = 1u, dil = 1u, t_in = uint(tin), r0 = 0u, rows = uint(tin), xs = uint(nm))
    var k2 = St2ConvArgs(cin = uint(d), k = 3u, stride = 2u, pad_l = 1u, dil = 1u, t_in = uint(tin), r0 = 0u, rows = uint(tt), xs = uint(d))
    var err = ""
    let ran = with_compute_encoder(g_queue, err) $(enc : MetalComputeEncoder?) {   // nolint:PERF026 — the error-text fetch runs only on the failure leg
        enc_st2_im2col_cm(enc, bmel, bx1, ka1, mp1 * k1)
        if (bw1 != null) {
            enc_f32_mm(enc, bw1, 0ul, bx1, by1, 0ul, u_nm3, u_d, mp1, d)
        } else {
            enc_f32_mm(enc, bblob, uint64(t.conv1_w_off * 4l), bx1, by1, 0ul, u_nm3, u_d, mp1, d)
        }
        enc_add_bias_rows(enc, by1, 0ul, bblob, uint64(t.conv1_b_off * 4l), u_d, u_totd1, tin * d)
        if (t.gelu_tanh) {
            enc_gelu_lut(enc, by1, 0ul, u_totd1, tin * d)
        } else {
            enc_gelu_erf(enc, by1, 0ul, u_totd1, tin * d)
        }
        enc_st2_im2col(enc, by1, bx2, k2, mp2 * 3l * d)
        conv2_mm(enc, t, bblob, bx2, bx2h, bxo, u_3d, u_d, mp2)
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
    if (bx2h != null) {
        pool_release(g_tw_pool, bx2h, bytes_x2 / 2ul)
    }
    pool_release(g_upool, u_nm3, 4ul)
    pool_release(g_upool, u_d, 4ul)
    pool_release(g_upool, u_3d, 4ul)
    pool_release(g_upool, u_totd1, 4ul)
    pool_release(g_upool, u_totd2, 4ul)
    return served
}

// The qwen3a conv frontend needs PADDED weights for the tile GEMM (n_ch=480 fails ndim%64,
// conv1's kdim=9 fails %32): a driver-owned buffer minted once per tower — out-channels
// padded to ncp=pad64(n_ch), conv1's K padded to 32, biases padded with zeros.
var private g_tw_q3a_wpad : St2SlabBase

[cold_path]
def private q3a_wpad_attach(t : Qwen3aTower) : bool {
    let key = tw_plane_key(t.cblob)
    return true if (st2_slab_current(g_tw_q3a_wpad, key))
    st2_slab_free(g_tw_q3a_wpad)
    let nch = t.n_ch
    let ncp = round_up(nch, 64l)
    let k2 = 9l * nch
    let total = ncp * 32l + 2l * ncp * k2 + 3l * ncp
    var wpad = metal_new_buffer(g_dev, uint64(total * 4l))
    if (wpad == null) {
        return false
    }
    unsafe {
        var p = reinterpret<float?>(metal_buffer_contents(wpad))
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
    g_tw_q3a_wpad.buf = wpad
    g_tw_q3a_wpad.key = key
    return true
}

// one conv2d stage: padded im2col + the f32 tile GEMM + padded bias + erf-GELU
def private q3a_stage(enc : MetalComputeEncoder?; bsrc : MetalBuffer?; soff : uint64; bcol, bout : MetalBuffer?; yoff : uint64;
                      u_iw, u_ih, u_cin, u_ow, u_kd, u_ss, u_ti, u_kdim, u_ncp, u_tm : MetalBuffer?;
                      ti, tm, mp, ncp : int64; woff, boff : uint64) {
    enc_im2col2d(enc, bsrc, soff, bcol, u_iw, u_ih, u_cin, u_ow, u_kd, u_ss, u_ti, ti)
    enc_f32_mm(enc, g_tw_q3a_wpad.buf, woff, bcol, bout, yoff, u_kdim, u_ncp, mp, ncp)
    enc_add_bias_rows(enc, bout, yoff, g_tw_q3a_wpad.buf, boff, u_ncp, u_tm, tm)
    enc_gelu_erf(enc, bout, yoff, u_tm, tm)
}

//! The qwen3a front for one WINDOW: chunk convs + feature shuffle + conv_out + bias/positions
//! in one command buffer, s.x readback. The q8 cblob lane declines (the cout trio lives in
//! qcblob).
[hot_path, arch(at="../ARCHITECTURE_GPU_TOWER.md#tower-encode-chains")]
def private metal_q3a_front(t : Qwen3aTower; win : MelChunk; n_chunks : int64; var s : EncoderState) : bool {   // nolint:STYLE038 — the dispatch chain is the shape
    let nm = t.core.n_mel
    let nch = t.n_ch
    let ncp = round_up(nch, 64l)
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
    let mp3 = round_up(y3_rows, 32l)      // the stage-3 GEMM pads to 224 rows
    let mpn = round_up(npos, 32l)
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
def private metal_q3a_mel(xw : array<float>; n_len, n_fft, n_bins, n_mel : int64;
                          twiddles, filters : array<float>; var m : WhisperMel) : bool {
    if (n_mel % 64l != 0l || n_len < 1l || !g_tw_env_tower || !metal_tower_init() || !pf_q3a_mel_ready()) {
        return false
    }
    let mp = round_up(n_len, 32l)
    let kfp = round_up(n_fft, 32l)
    let dft_out_pad = round_up(2l * n_bins, 64l)
    let ldpow = round_up(n_bins, 32l)
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
    upload_rows_strided(bxw, unsafe(addr<float const?>(xw[0])), n_len, n_fft, kfp)   // xw rows at the padded k stride, zero tails; pad rows zeroed
    zero_rows(bxw, n_len, mp, kfp)
    tw_twiddle_panel(btw, unsafe(addr<float const?>(twiddles[0])), n_bins, n_fft, kfp, dft_out_pad)
    upload_rows_strided(bfil, unsafe(addr<float const?>(filters[0])), n_mel, n_bins, ldpow)   // the mel filterbank [n_mel x ldpow], k-padded
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
        // the float tiles: a quiet bin sits 80 dB under the frame's loud ones, below what a half tile resolves
        enc_f32_exact_mm(enc, btw, 0ul, bxw, bre, 0ul, u_kfp, u_dft_out_pad, mp, dft_out_pad)
        enc_q3a_pow(enc, bre, bpow, u_nbins, u_dft_out_pad, u_ldpow, u_totpow, mp * ldpow)
        enc_f32_exact_mm(enc, bfil, 0ul, bpow, braw, 0ul, u_ldpow, u_nmel, mp, n_mel)
        enc_q3a_logmel_t(enc, braw, bout, u_nmel, u_nlen, u_floor, u_scale, u_totm, n_len * n_mel)
    }
    var served = false
    if (ran) {
        unsafe {
            memcpy(addr(m.data[0]), metal_buffer_contents(bout), uint64(n_mel * n_len * 4l))
        }
        g_tower_mels++
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
    aux_off : uint64
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

def private st2_slot_uniforms(w : TtsSlabWriter; var s : St2ConvSlot) {
    if (!w.measure) {
        s.u_kdim = uniform_u32(uint(s.kdim_p))
        s.u_cout = uniform_u32(uint(s.cout_p))
    }
}

//! the shared writer's slot as the Metal slot: byte offsets, and the uniforms the GEMM binds
def private st2_slot_of(w : TtsSlabWriter; t : TtsConvSlot) : St2ConvSlot {
    var s = St2ConvSlot(woff = uint64(t.woff * 4l), boff = uint64(t.boff * 4l), cout_p = t.cout_p, kdim_p = t.kdim_p, gathers = t.gathers)
    st2_slot_uniforms(w, s)
    return s
}

def private st2_norm_of(t : TtsNormSlot) : St2NormSlot
    => St2NormSlot(gwoff = uint64(t.gwoff * 4l), gboff = uint64(t.gboff * 4l), aux_off = uint64(t.aux_off * 4l))

def private st2_byte_off(off : int64) : uint64 => uint64(off * 4l)

def private st2_slab_linear(var w : TtsSlabWriter; l : TtsLinear) : St2ConvSlot => st2_slot_of(w, tts_slab_linear(w, l))

//! n floats of src into the slab, zeros past its length; the byte offset
def private st2_slab_raw(var w : TtsSlabWriter; src : array<float>; n : int64) : uint64 => st2_byte_off(tts_slab_raw(w, src, n))

def private st2_slab_norm_rows(var w : TtsSlabWriter; n : TtsNorm; c : int64) : St2NormSlot => st2_norm_of(tts_slab_norm_rows(w, n, c))

def private st2_lstm_of(w : TtsSlabWriter; t : TtsLstmSlot) : St2LstmSlot
    => St2LstmSlot(fx = st2_slot_of(w, t.fx), bx = st2_slot_of(w, t.bx), fhh = st2_byte_off(t.fhh), bhh = st2_byte_off(t.bhh))

def private st2_block_of(w : TtsSlabWriter; t : TtsBlockSlots) : St2BlockSlots {
    return <- St2BlockSlots(c1 <- [for (c in t.c1); st2_slot_of(w, c)], c2 <- [for (c in t.c2); st2_slot_of(w, c)],
        n1 <- [for (n in t.n1); st2_norm_of(n)], n2 <- [for (n in t.n2); st2_norm_of(n)])
}

//! a residual block's slots; its 1x1 shortcut's uniforms only where the block carries one
def private st2_res_of(w : TtsSlabWriter; b : St2AdainResBlk; t : TtsResSlots) : St2ResSlots {
    var s = St2ResSlots(n1 = st2_norm_of(t.n1), n2 = st2_norm_of(t.n2), c1 = st2_slot_of(w, t.c1), c2 = st2_slot_of(w, t.c2),
        pool_w = st2_byte_off(t.pool_w), pool_b = st2_byte_off(t.pool_b))
    if (b.learned_sc) {
        s.c1x1 = st2_slot_of(w, t.c1x1)
    }
    return s
}

def private st2_slot_release(var s : St2ConvSlot) {
    st2_uniform_pair_release(s.u_kdim, s.u_cout)
}

//! A pair of pooled uniforms acquired together back to the pool, both nulled; a pair never acquired stays as it is.
def private st2_uniform_pair_release(var a, b : MetalBuffer?&) {
    if (a != null) {
        pool_release(g_upool, a, 4ul)
        pool_release(g_upool, b, 4ul)
        a = null
        b = null
    }
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
    stft_re : int64      //!< the STFT and inverse STFT bases and the window as element offsets - their kernels take the slab whole
    stft_im : int64
    istft_re : int64
    istft_im : int64
    window : int64
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

//! Untracked: the slab is GPU-read-only and written once here, as the model weights are.
[cold_path]
def private st2_slab_upload(var base : St2SlabBase; host : array<float>; key : uint64) : bool {
    let bytes = uint64(long_length(host) * 4l)
    base.buf = metal_new_buffer_untracked(g_dev, bytes)
    if (base.buf == null) {
        return false
    }
    unsafe {
        memcpy(metal_buffer_contents(base.buf), addr(host[0]), bytes)
    }
    base.key = key
    return true
}

def private st2_release_any(var s : St2ConvSlot) {
    st2_slot_release(s)
}

def private st2_release_any(var p : PkLin) {
    pk_lin_release(p)
}

def private st2_release_any(var b : MetalBuffer?) {
    release_handle(b)
}

def private st2_release_any(var a : array<auto(TT)>) {
    for (e in a) {
        st2_release_any(e)
    }
}

def private st2_release_any(var x : auto(TT)) {
    static_if (typeinfo is_struct(type<TT>)) {
        apply(x) $(name, field) {
            st2_release_any(field)
        }
    }
}

[arch(at = "../ARCHITECTURE_GPU_TOWER.md#tower-tts-chain")]
def private st2_slab_free(var base : auto(TT)) {
    st2_release_any(base)
    unsafe {
        delete base
    }
    base.key = 0ul
}

//! The slab for `key` stands: its buffer is up under that key.
def private st2_slab_current(base : St2SlabBase; key : uint64) : bool => base.buf != null && base.key == key

[cold_path, arch(at = "../ARCHITECTURE_GPU_TOWER.md#tower-tts-chain")]
def private st2_slab_rebuild(var base : auto(SS); key : uint64; write : block<(var w : TtsSlabWriter; var s : SS) : void>) : bool {
    st2_slab_free(base)
    var inscope host <- tts_slab_build(base, write, lstm_transposed = false)
    if (!st2_slab_upload(base, host, key)) {
        st2_slab_free(base)
        return false
    }
    return true
}

def private st2_dec_write(var w : TtsSlabWriter; d : St2Decoder; var s : St2DecSlab) {
    var inscope t : TtsDecSlab
    tts_dec_write(w, d, t)
    s.ups <- [for (c in t.ups); st2_slot_of(w, c)]
    s.noise <- [for (c in t.noise); st2_slot_of(w, c)]
    s.blocks |> reserve(length(t.blocks))
    for (b in t.blocks) {
        s.blocks |> emplace(st2_block_of(w, b))
    }
    s.post = st2_slot_of(w, t.post)
    s.f0c = st2_slot_of(w, t.f0c)
    s.nc = st2_slot_of(w, t.nc)
    s.asr_res = st2_slot_of(w, t.asr_res)
    s.encode = st2_res_of(w, d.encode, t.encode)
    s.decode <- [for (b, r in d.decode, t.decode); st2_res_of(w, b, r)]
    s.src_w = st2_byte_off(t.src_w)
    s.src_b = st2_byte_off(t.src_b)
    s.stft_re = t.stft_re
    s.stft_im = t.stft_im
    s.istft_re = t.istft_re
    s.istft_im = t.istft_im
    s.window = t.window
}


def private st2_dec_attach(d : St2Decoder) : bool {
    let key = tts_dec_key(d)
    return st2_slab_current(g_tw_st2_dec, key) || st2_dec_rebuild(d, key)
}

[cold_path]
def private st2_dec_rebuild(d : St2Decoder; key : uint64) : bool {
    return st2_slab_rebuild(g_tw_st2_dec, key) $(var w; var s : St2DecSlab) {
        st2_dec_write(w, d, s)
    }
}

def private st2_pred_write(var w : TtsSlabWriter; p : St2Predictor; var s : St2PredSlab) {
    var inscope t : TtsPredSlab
    tts_pred_write(w, p, t)
    s.bert_enc = st2_slot_of(w, t.bert_enc)
    s.de_lstm <- [for (l in t.de_lstm); st2_lstm_of(w, l)]
    s.dur_lstm = st2_lstm_of(w, t.dur_lstm)
    s.dur_proj = st2_slot_of(w, t.dur_proj)
    s.sh_lstm = st2_lstm_of(w, t.sh_lstm)
    s.f0b <- [for (b, r in p.f0, t.f0b); st2_res_of(w, b, r)]
    s.nb <- [for (b, r in p.n, t.nb); st2_res_of(w, b, r)]
    s.f0_proj = st2_slot_of(w, t.f0_proj)
    s.n_proj = st2_slot_of(w, t.n_proj)
}


def private st2_pred_attach(p : St2Predictor) : bool {
    let key = tts_pred_key(p)
    return st2_slab_current(g_tw_st2_pred, key) || st2_pred_rebuild(p, key)
}

[cold_path]
def private st2_pred_rebuild(p : St2Predictor; key : uint64) : bool {
    return st2_slab_rebuild(g_tw_st2_pred, key) $(var w; var s : St2PredSlab) {
        st2_pred_write(w, p, s)
    }
}

def private st2_albert_write(var w : TtsSlabWriter; a : St2Albert; var s : St2AlbertSlab) {
    var t : TtsAlbertSlab
    tts_albert_write(w, a, t)
    s.word_emb = st2_byte_off(t.word_emb)
    s.pos_emb = st2_byte_off(t.pos_emb)
    s.type_emb = st2_byte_off(t.type_emb)
    s.emb_ln = st2_norm_of(t.emb_ln)
    s.map_in = st2_slot_of(w, t.map_in)
    s.q = st2_slot_of(w, t.q)
    s.k = st2_slot_of(w, t.k)
    s.v = st2_slot_of(w, t.v)
    s.dense = st2_slot_of(w, t.dense)
    s.attn_ln = st2_norm_of(t.attn_ln)
    s.ffn = st2_slot_of(w, t.ffn)
    s.ffn_out = st2_slot_of(w, t.ffn_out)
    s.full_ln = st2_norm_of(t.full_ln)
}


def private st2_albert_attach(a : St2Albert) : bool {
    let key = tts_albert_key(a)
    return st2_slab_current(g_tw_st2_albert, key) || st2_albert_rebuild(a, key)
}

[cold_path]
def private st2_albert_rebuild(a : St2Albert; key : uint64) : bool {
    return st2_slab_rebuild(g_tw_st2_albert, key) $(var w; var s : St2AlbertSlab) {
        st2_albert_write(w, a, s)
    }
}

def private st2_text_write(var w : TtsSlabWriter; te : St2TextEncoder; var s : St2TextSlab) {
    var inscope t : TtsTextSlab
    tts_text_write(w, te, t)
    s.emb = st2_byte_off(t.emb)
    s.cnn <- [for (c in t.cnn); st2_slot_of(w, c)]
    s.ln <- [for (n in t.ln); st2_norm_of(n)]
    s.lstm = st2_lstm_of(w, t.lstm)
}


def private st2_text_attach(te : St2TextEncoder) : bool {
    let key = tts_text_key(te)
    return st2_slab_current(g_tw_st2_text, key) || st2_text_rebuild(te, key)
}

[cold_path]
def private st2_text_rebuild(te : St2TextEncoder; key : uint64) : bool {
    return st2_slab_rebuild(g_tw_st2_text, key) $(var w; var s : St2TextSlab) {
        st2_text_write(w, te, s)
    }
}

def private st2_slab_drop {
    st2_slab_free(g_tw_st2_dec)
    st2_slab_free(g_tw_st2_pred)
    st2_slab_free(g_tw_st2_albert)
    st2_slab_free(g_tw_st2_text)
    st2_slab_free(g_tw_pk_codec)
    st2_slab_free(g_tw_pk_frames)
    pk_voice_drop()
}

struct private St2GpuCtx {
    rows : TwBufSet
    us_v : uint [ST2_MAX_UNIFORMS]     //!< the value-keyed uniforms (a float's by its bits): a value binds once an encode
    us_b : MetalBuffer? [ST2_MAX_UNIFORMS]
    n_us : int
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
    let mp = round_up(rows, 32l)
    let bytes = uint64(mp * width * 4l)
    var b = tw_take(c.rows, bytes)
    zero_rows(b, rows, mp, width)
    return b
}

def private st2_ctx_release(var c : St2GpuCtx) {
    tw_release(c.rows)
    pool_release(g_tw_pool, c.bh, c.bytes_h)
    pool_release(g_tw_pool, c.bstats, c.bytes_stats)
    pool_release(g_tw_pool, c.bcols, c.bytes_cols)
    for (i in range(c.n_us)) {
        pool_release(g_upool, c.us_b[i], 4ul)
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

//! The uniform holding the float `v`: the u32 uniform of its bits, the same four bytes.
def private st2_uf(var c : St2GpuCtx; v : float) : MetalBuffer? => st2_u(c, float_bits_to_uint(v))

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
    let hoff = c.hcur
    assert(hoff + 2l * cc <= c.hcap)
    unsafe {
        var p = reinterpret<float?>(metal_buffer_contents(c.bh)) + hoff
        tts_style_rows(fc, s, cc, ln_form, g_tw_st2_style_fc, p)
    }
    c.hcur += 2l * cc
    return uint64(hoff * 4l)
}

def private st2_chunk_rows(kdim_p : int64) : int64 => clamp(ST2_IM2COL_CHUNK_MAX_BYTES / (kdim_p * 4l) / 32l * 32l, 32l, ST2_CHUNK_ROWS)

def private st2_cols_bytes(c : TtsConv1d) : uint64 {
    return 0ul if (c.cin % 8l == 0l)
    let kdim_p = round_up(c.k * c.cin, 32l)
    return uint64(st2_chunk_rows(kdim_p) * kdim_p * 4l)
}

def private st2_conv_args(cv : TtsConv1d; t_in, r0, rows : int64) : St2ConvArgs {
    return St2ConvArgs(cin = uint(cv.cin), k = uint(cv.k), stride = uint(cv.stride),
        pad_l = uint(cv.pad_l), dil = uint(cv.dilation), t_in = uint(t_in), r0 = uint(r0), rows = uint(rows), xs = uint(cv.cin))
}

//! One rows conv and its bias into rows of stride slot.cout_p; the output row count.
def private st2_conv_mm(enc : MetalComputeEncoder?; c : St2GpuCtx; slot : St2ConvSlot; transposed : bool; bx, by : MetalBuffer?; var ka : St2ConvArgs; rows : int64) {
    let mp = round_up(rows, 32l)
    if (transposed) {
        if (c.exact) {
            enc_st2_conv_tr_exact_mm(enc, c.slab, slot.woff, bx, by, 0ul, slot.u_cout, ka, mp, slot.cout_p)
        } else {
            enc_st2_conv_tr_mm(enc, c.slab, slot.woff, bx, by, 0ul, slot.u_cout, ka, mp, slot.cout_p)
        }
    } elif (c.exact) {
        enc_st2_conv_exact_mm(enc, c.slab, slot.woff, bx, by, 0ul, slot.u_cout, ka, mp, slot.cout_p)
    } else {
        enc_st2_conv_mm(enc, c.slab, slot.woff, bx, by, 0ul, slot.u_cout, ka, mp, slot.cout_p)
    }
}

def private st2_conv(enc : MetalComputeEncoder?; var c : St2GpuCtx; slot : St2ConvSlot; cv : TtsConv1d;
                     bx : MetalBuffer?; t_in : int64; by : MetalBuffer?) : int64 {
    let t_out = conv1d_out_len(cv, t_in)
    if (slot.gathers) {
        var ka = st2_conv_args(cv, t_in, 0l, t_out)
        st2_conv_mm(enc, c, slot, cv.transposed, bx, by, ka, t_out)
    }
    var r0 = slot.gathers ? t_out : 0l
    let chunk = st2_chunk_rows(slot.kdim_p)
    while (r0 < t_out) {
        let rows = min(chunk, t_out - r0)
        var ka = st2_conv_args(cv, t_in, r0, rows)
        let mp = round_up(rows, 32l)
        if (cv.transposed) {
            enc_st2_im2col_tr(enc, bx, c.bcols, ka, mp * slot.kdim_p)
        } else {
            enc_st2_im2col(enc, bx, c.bcols, ka, mp * slot.kdim_p)
        }
        st2_mm(enc, c, slot.woff, c.bcols, by, uint64(r0 * slot.cout_p * 4l), slot.u_kdim, slot.u_cout, mp, slot.cout_p)
        r0 += rows
    }
    st2_conv_bias(enc, c, slot, by, t_out)
    return t_out
}

//! The conv's bias over its `rows` output rows, and the conv counted.
def private st2_conv_bias(enc : MetalComputeEncoder?; var c : St2GpuCtx; slot : St2ConvSlot; by : MetalBuffer?; rows : int64) {
    let n = rows * slot.cout_p
    let u_n = st2_u(c, uint(n))
    enc_add_bias_rows(enc, by, 0ul, c.slab, slot.boff, slot.u_cout, u_n, n)
    g_tower_convs++
}

def private st2_mm(enc : MetalComputeEncoder?; c : St2GpuCtx; woff : uint64; bx, by : MetalBuffer?; yoff : uint64; u_kdim, u_cout : MetalBuffer?; mp, d : int64) {
    if (c.exact) {
        enc_f32_exact_mm(enc, c.slab, woff, bx, by, yoff, u_kdim, u_cout, mp, d)
    } else {
        enc_f32_mm(enc, c.slab, woff, bx, by, yoff, u_kdim, u_cout, mp, d)
    }
}

def private st2_lin(enc : MetalComputeEncoder?; var c : St2GpuCtx; slot : St2ConvSlot; bx : MetalBuffer?; rows : int64; by : MetalBuffer?) {
    st2_mm(enc, c, slot.woff, bx, by, 0ul, slot.u_kdim, slot.u_cout, round_up(rows, 32l), slot.cout_p)
    let n = rows * slot.cout_p
    let u_n = st2_u(c, uint(n))
    enc_add_bias_rows(enc, by, 0ul, c.slab, slot.boff, slot.u_cout, u_n, n)
}

def private st2_ln(enc : MetalComputeEncoder?; var c : St2GpuCtx; bx, by : MetalBuffer?; rows, d : int64; eps : float; wbuf : MetalBuffer?; woff, boff : uint64) {
    let u_d = st2_u(c, uint(d))
    let u_eps = st2_uf(c, eps)
    enc_ln(enc, bx, wbuf, woff, wbuf, boff, by, 0ul, u_d, u_eps, rows)
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
        enc_st2_adain_snake(enc, bin, bout, c.bstats, c.bh, hoff, c.slab, ns.gwoff, c.slab, ns.gboff, c.slab, ns.aux_off, ka, n)
    } else {
        enc_st2_adain_leaky(enc, bin, bout, c.bstats, c.bh, hoff, c.slab, ns.gwoff, c.slab, ns.gboff, ka, n)
    }
}

//! o = (a + b) * scale: the in-place sum (o == a) is the residual add, another o the out-of-place stamp.
def private st2_add(enc : MetalComputeEncoder?; var c : St2GpuCtx; ba, bb, bo : MetalBuffer?; n : int64; scale : float) {
    enc_st2_add(enc, ba, bb, bo, GkAddScaleArgs(nelem = uint(n), scale = scale), n)
}

def private st2_leaky(enc : MetalComputeEncoder?; var c : St2GpuCtx; bx : MetalBuffer?; n : int64; slope : float) {
    let u_slope = st2_uf(c, slope)
    let u_n = st2_u(c, uint(n))
    enc_st2_leaky(enc, bx, 0ul, u_n, u_slope, n)
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
        var kg = RowGatherArgs(c = uint(b.dim_in_s), add_b = 0u, add_c = 0u, total = uint(t1 * b.dim_in_s), scale = 1.0)
        enc_row_gather(enc, bin, 0ul, r.bidx, bin, 0ul, bin, 0ul, r.bsc, kg, t1 * b.dim_in_s)
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
    for (j in range64(nk)) {
        st2_gen_res_block(enc, c, d.resblocks[i * nk + j], sl.blocks[nnoise + i * nk + j], g.bxt, g.by1, g.by, g.bsres, t2, s)
        enc_st2_axpy(enc, bout, g.bsres, GkAxpyArgs(nelem = uint(n), scale = 1.0 / float(nk), zero = j == 0l ? 1u : 0u), n)
    }
    tt = t2
    cc = c2
}

//! A seat's shape record, filled by the shared walks (`tts_gen_shape` and its kin) through the hooks below.
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
    return false if (!tts_conv_ok(c))
    sh.cols_bytes = max(sh.cols_bytes, st2_cols_bytes(c))
    return true
}

//! a conv's gathered columns
[unused_argument(rows, upsample)]
def private tts_note_conv(var sh : St2Shape; cv : TtsConv1d; rows : int64; upsample : bool) : bool => st2_note_conv(sh, cv)

//! the rows' buffer at the 32 lattice
def private tts_note_rows(var sh : St2Shape; t, c : int64) {
    sh.c_max = max(sh.c_max, c)
    sh.row_buf_floats = max(sh.row_buf_floats, round_up(t, 32l) * c)
}

//! a Snake block's style fc rows
[unused_argument(t)]
def private tts_note_block(var sh : St2Shape; b : St2GenResBlock; t, c : int64) : bool {
    sh.style_fc_floats += long_length(b.convs1) * 2l * 2l * c
    return true
}

//! an AdaIN block's style fc rows and its rows
def private tts_note_res(var sh : St2Shape; b : St2AdainResBlk; t1 : int64) : bool {
    sh.style_fc_floats += 2l * (b.dim_in_s + b.dim_out)
    tts_note_rows(sh, t1, max(b.dim_in_s, b.dim_out))
    return true
}

//! conv_post's rows ride one ST2_POST_WIDTH row stride
def private tts_note_post(var sh : St2Shape; post : TtsConv1d; t_post : int64) : bool {
    return false if (post.cout > ST2_POST_WIDTH || !st2_note_conv(sh, post))
    sh.row_buf_floats = max(sh.row_buf_floats, round_up(t_post, 32l) * ST2_POST_WIDTH)
    return true
}

[unused_argument(sh, d)]
def private tts_note_decoder(var sh : St2Shape; d : St2Decoder) : bool => true

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
    if (!tts_gen_shape(d, cx, t, har_frames, ST2_MAX_STAGES, sh)) {
        return tw_decline(MetalTowerDecline.shape)
    }
    var tc = ref_time_ticks()
    if (!tts_dec_ok(d)) {
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

let private ST2_MAX_DE_LSTMS = 8   //!< the duration encoder's LSTM stack the seat holds buffers for

def private st2_source_chain(enc : MetalComputeEncoder?; var c : St2GpuCtx; d : St2Decoder; cfg : SineSourceCfg; ss : TtsSourceShape;
                             bf0, bnu, bnoise, blow, blowc, bhar, bspec : MetalBuffer?; captured : bool; seed : uint64) {
    let nh = cfg.n_harm
    var ka = GkSourceArgs(n = uint(ss.samples), nlow = uint(ss.nlow), nh = uint(nh), up = uint(cfg.upsample), sr = cfg.sample_rate,
        sine_amp = cfg.sine_amp, noise_std = cfg.noise_std, voiced_thr = cfg.voiced_thr, seed = uint(seed ^ (seed >> 32ul)))
    let sl & = g_tw_st2_dec
    if (!captured) {
        enc_st2_src_noise(enc, bnoise, ka, ss.samples * nh)
    }
    if (cfg.torch_math) {
        enc_st2_src_low_torch(enc, bf0, bnu, blow, ka, nh * ss.nlow)
        enc_st2_src_cumsum_torch(enc, blow, blowc, ka, nh)
        enc_st2_src_sines_torch(enc, bf0, blowc, bnoise, c.slab, sl.src_w, c.slab, sl.src_b, bhar, ka, ss.samples)
    } else {
        enc_st2_src_low_onnx(enc, bf0, bnu, blow, ka, nh * ss.nlow)
        enc_st2_src_cumsum_onnx(enc, blow, blowc, ka, nh)
        enc_st2_src_sines_onnx(enc, bf0, blowc, bnoise, c.slab, sl.src_w, c.slab, sl.src_b, bhar, ka, ss.samples)
    }
    let re & = unsafe(d.stft_fwd_re)
    var ks = GkStftArgs(n = uint(ss.samples), frames = uint(ss.spec_frames), bins = uint(d.bins), k = uint(re.k), hop = uint(re.stride),
        pad = uint(ST2_STFT_PAD), eps = d.stft_eps, reoff = uint(sl.stft_re), imoff = uint(sl.stft_im))
    if (d.stft_pad_reflect) {
        enc_st2_stft_reflect(enc, bhar, c.slab, bspec, ks, ss.spec_frames * d.bins)
    } else {
        enc_st2_stft_edge(enc, bhar, c.slab, bspec, ks, ss.spec_frames * d.bins)
    }
}

//! The decode chain's shape: the front convs and the residual as the concat rows ask, the AdaIN
//! residual blocks, the source, the generator, the inverse STFT.
struct private St2DecShape : St2Shape {
    ss : TtsSourceShape
    dec_rows : int64
    dec_width : int64
    n_out : int64
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

def private st2_dec_bufs(var c : St2GpuCtx; d : St2Decoder; sh : St2DecShape; source : SineSourceCfg; text_c, frames, f2 : int64) : St2DecBufs {
    let nh = source.n_harm
    return St2DecBufs(bx = st2_rows_buf(c, sh.row_buf_floats, 1l), basr = st2_rows_buf(c, frames, text_c), bres = st2_rows_buf(c, frames, d.asr_res.cout),
        bf0r = st2_rows_buf(c, f2, 1l), bnr = st2_rows_buf(c, f2, 1l), bf0d = st2_rows_buf(c, frames, ST2_POST_WIDTH), bnd = st2_rows_buf(c, frames, ST2_POST_WIDTH),
        bcat = st2_rows_buf(c, sh.row_buf_floats, 1l), bnu = st2_rows_buf(c, nh, 1l), bnoise = st2_rows_buf(c, sh.ss.samples * nh, 1l),
        blow = st2_rows_buf(c, nh * sh.ss.nlow, 1l), blowc = st2_rows_buf(c, nh * sh.ss.nlow, 1l), bhar = st2_rows_buf(c, sh.ss.samples, 1l),
        bspec = st2_rows_buf(c, sh.ss.spec_frames * 2l * d.bins, 1l), bwave = st2_rows_buf(c, sh.n_out, 1l))
}

//! The encode block, then each decode block (re-concatenated where its width asks); the stream lands in bx.
def private st2_decoder_chain(enc : MetalComputeEncoder?; var c : St2GpuCtx; d : St2Decoder; b : St2DecBufs; g : St2GenBufs; r : St2ResBufs;
                              text_c, frames : int64; s_ref : array<float>) {
    let sl & = g_tw_st2_dec
    let rc = uint(d.asr_res.cout)
    var kc = GkConcatArgs(c = uint(text_c), rc = rc, rs = uint(sl.asr_res.cout_p), f0s = uint(ST2_POST_WIDTH), ns = uint(ST2_POST_WIDTH),
        with_res = 0u, width = uint(d.encode.dim_in_s), rows = uint(frames))
    enc_st2_concat(enc, b.basr, b.bres, b.bf0d, b.bnd, b.bcat, kc, frames * d.encode.dim_in_s)
    var tt = st2_res_blk(enc, c, d.encode, sl.encode, r, b.bcat, frames, s_ref, b.bx)
    var cwr = d.encode.dim_out
    var in_x = true
    for (i in range(length(d.decode))) {
        let blk & = unsafe(d.decode[i])
        let src = in_x ? b.bx : g.bo
        let dst = in_x ? g.bo : b.bx
        if (blk.dim_in != cwr) {
            var kc2 = GkConcatArgs(c = uint(cwr), rc = rc, rs = uint(sl.asr_res.cout_p), f0s = uint(ST2_POST_WIDTH), ns = uint(ST2_POST_WIDTH),
                with_res = 1u, width = uint(blk.dim_in_s), rows = uint(tt))
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
    if (!tts_decode_shape(d, text_c, source, frames, f2, noise, ST2_MAX_STAGES, sh)) {
        return tw_decline(MetalTowerDecline.shape)
    }
    var tc = ref_time_ticks()
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
    var b = st2_dec_bufs(c, d, sh, source, text_c, frames, f2)
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
        var ki = GkIstftArgs(tt = uint(sh.t_post), stride = uint(ST2_POST_WIDTH), bins = uint(d.bins), k = uint(bwd.k), hop = uint(bwd.stride),
            pad = uint(ST2_STFT_PAD), envelope = d.istft_envelope ? 1u : 0u, n = uint(sh.n_out), reoff = uint(sl.istft_re), imoff = uint(sl.istft_im), wnoff = uint(sl.window))
        enc_st2_istft(enc, r.bc1, c.slab, b.bwave, ki, sh.n_out)
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
    if (frames < 1l || cc % 32l != 0l || !tts_lstm_ok(p.shared_lstm, cc, ST2_LSTM_MAX_HIDDEN) || length(p.f0) != length(p.n) || empty(p.f0)
        || p.f0[0].dim_in_s != 2l * hd || p.n[0].dim_in_s != 2l * hd || p.f0_proj.cout != 1l || p.n_proj.cout != 1l
        || !st2_note_conv(sh, p.f0_proj) || !st2_note_conv(sh, p.n_proj)) {
        return tw_decline(MetalTowerDecline.shape)
    }
    var tf = frames
    var tn = frames
    for (i in range(length(p.f0))) {
        return tw_decline(MetalTowerDecline.shape) if (i > 0 && (p.f0[i].dim_in_s != p.f0[i - 1].dim_out || p.n[i].dim_in_s != p.n[i - 1].dim_out))
        tf = tts_res_shape(sh, p.f0[i], tf)
        tn = tts_res_shape(sh, p.n[i], tn)
        return tw_decline(MetalTowerDecline.shape) if (tf < 0l || tn < 0l)
    }
    return tw_decline(MetalTowerDecline.shape) if (tf != tn || p.f0_proj.cin != p.f0[length(p.f0) - 1].dim_out || p.n_proj.cin != p.n[length(p.n) - 1].dim_out
        || conv1d_out_len(p.f0_proj, tf) != tf || conv1d_out_len(p.n_proj, tn) != tn)
    sh.row_buf_floats = max(sh.row_buf_floats, round_up(frames, 32l) * max(cc, 2l * hd))
    if (!tts_pred_ok(p, ST2_LSTM_MAX_HIDDEN)) {
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
        return tw_decline(MetalTowerDecline.shape) if (!tts_lstm_ok(l, width, ST2_LSTM_MAX_HIDDEN) || 2l * l.fwd.hidden != cc)
    }
    if (length(p.lstms) > ST2_MAX_DE_LSTMS || !tts_pred_ok(p, ST2_LSTM_MAX_HIDDEN)) {
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
        var kc = GkConcatArgs(c = uint(cc), rc = uint(sd), rs = 0u, f0s = 0u, ns = 0u, with_res = 1u, width = uint(width), rows = uint(t))
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
        || p.duration_proj.nout > 64l || speed <= 0.0 || !tts_pred_ok(p, ST2_LSTM_MAX_HIDDEN)) {
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
        enc_st2_sigsum(enc, blog, braw, GkSigSumArgs(t = uint(t), nb = uint(p.duration_proj.nout), ls = uint(sl.dur_proj.cout_p), speed = speed), t)
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
    if (!tts_albert_ok(a)) {
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
        var kg = RowGatherArgs(c = uint(a.emb_dim), add_b = 1u, add_c = 1u, total = uint(t * a.emb_dim), scale = 1.0)
        enc_row_gather(enc, c.slab, sl.word_emb, bids, c.slab, sl.pos_emb, c.slab, sl.type_emb, be, kg, t * a.emb_dim)
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
            enc_st2_gelu_tanh(enc, bffn, 0ul, u_nf, t * ff)
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
    if (t < 1l || cc % 64l != 0l || length(te.cnn_ln) != length(te.cnn) || !tts_lstm_ok(te.lstm, cc, ST2_LSTM_MAX_HIDDEN) || 2l * te.lstm.fwd.hidden != cc) {
        return tw_decline(MetalTowerDecline.shape)
    }
    for (cv in te.cnn) {
        return tw_decline(MetalTowerDecline.shape) if (!st2_note_conv(sh, cv) || cv.cin != cc || cv.cout != cc || conv1d_out_len(cv, t) != t || cv.transposed)
    }
    for (id in ids) {
        return tw_decline(MetalTowerDecline.shape) if (id < 0 || int64(id + 1) * cc > long_length(te.emb.a))
    }
    if (!tts_text_ok(te, ST2_LSTM_MAX_HIDDEN)) {
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
        var kg = RowGatherArgs(c = uint(cc), add_b = 0u, add_c = 0u, total = uint(t * cc), scale = 1.0)
        enc_row_gather(enc, c.slab, sl.emb, bids, c.slab, 0ul, c.slab, 0ul, ba, kg, t * cc)
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

struct private PkLayerSlots {
    n1 : St2NormSlot
    in_proj : St2ConvSlot
    out_proj : St2ConvSlot
    scale1_off : uint64          //!< the layer scale rows, 0 where the layer carries none
    has_s1 : bool
    n2 : St2NormSlot
    ffn1 : St2ConvSlot
    ffn2 : St2ConvSlot
    scale2_off : uint64
    has_s2 : bool
}

struct private PkCodecSlab : St2SlabBase {
    quant : St2ConvSlot
    up_w : uint64        //!< the depthwise upsample's taps [c][k] and bias
    up_b : uint64
    layers : array<PkLayerSlots>
    dec_in : St2ConvSlot
    ups : array<St2ConvSlot>
    res1 : array<St2ConvSlot>
    res2 : array<St2ConvSlot>
    dec_out : St2ConvSlot
    bcos : MetalBuffer?     //!< the transformer's rope tables over the one-shot chain's row budget
    bsin : MetalBuffer?
}

var private g_tw_pk_codec : PkCodecSlab

let private PK_CODEC_MAX_FRAMES = 512l   //!< the one-shot chain's row budget: past this the CPU's windows serve

//! the shared writer's layer slots as the Metal chain binds them: byte offsets, the linears' uniforms acquired
def private pk_layer_of(w : TtsSlabWriter; t : TtsPkLayerSlots) : PkLayerSlots {
    return PkLayerSlots(n1 = st2_norm_of(t.n1), in_proj = st2_slot_of(w, t.in_proj), out_proj = st2_slot_of(w, t.out_proj),
        scale1_off = st2_byte_off(t.scale1_off), has_s1 = t.has_s1, n2 = st2_norm_of(t.n2), ffn1 = st2_slot_of(w, t.ffn1), ffn2 = st2_slot_of(w, t.ffn2),
        scale2_off = st2_byte_off(t.scale2_off), has_s2 = t.has_s2)
}

//! the shared codec layout with no rope rows in the slab: this driver's rope tables ride their own buffers (`pk_rope_tables`)
def private pk_codec_write(var w : TtsSlabWriter; mm : PocketMimi; var s : PkCodecSlab) {
    var inscope t : TtsPkCodecSlab
    tts_pk_codec_write(w, mm, 0l, t)
    s.quant = st2_slot_of(w, t.quant)
    s.up_w = st2_byte_off(t.up_w)
    s.up_b = st2_byte_off(t.up_b)
    s.layers <- [for (l in t.layers); pk_layer_of(w, l)]
    s.dec_in = st2_slot_of(w, t.dec_in)
    s.ups <- [for (c in t.ups); st2_slot_of(w, c)]
    s.res1 <- [for (c in t.res1); st2_slot_of(w, c)]
    s.res2 <- [for (c in t.res2); st2_slot_of(w, c)]
    s.dec_out = st2_slot_of(w, t.dec_out)
}


//! the shared codec admission (dense convs, transposed taps past the stride), then the gathering stamps' 8-run channel lattice
def private pk_codec_ok(mm : PocketMimi) : bool {
    return false if (!tts_pk_codec_ok(mm, int64(PK_ATTN_HS), PK_ATTN_MAX_KEYS) || mm.quant_proj.cin % 8l != 0l || mm.dec_in.cin % 8l != 0l || mm.dec_out.cin % 8l != 0l)
    for (i, c in iter_range(mm.dec_up), mm.dec_up) {
        return false if (c.cin % 8l != 0l || mm.dec_res[i].conv1.cin % 8l != 0l || mm.dec_res[i].conv2.cin % 8l != 0l)
    }
    return true
}

def private pk_codec_attach(mm : PocketMimi) : bool {
    let key = tts_pk_codec_key(mm)
    return st2_slab_current(g_tw_pk_codec, key) || pk_codec_rebuild(mm, key)
}

//! The codec slab and its rope tables over the one-shot chain's row budget.
[cold_path]
def private pk_codec_rebuild(mm : PocketMimi; key : uint64) : bool {
    var bytes = 0l
    let ok = st2_slab_rebuild(g_tw_pk_codec, key) $(var w; var s : PkCodecSlab) {
        pk_codec_write(w, mm, s)
        bytes = long_length(w.host) * 4l
    }
    return false if (!ok)
    tts_pk_stage_rows(mm, PK_CODEC_MAX_FRAMES, g_tw_pk_sc.rows)
    if (!pk_rope_tables(mm.dec_tf, g_tw_pk_sc.rows[1], g_tw_pk_codec.bcos, g_tw_pk_codec.bsin)) {
        st2_slab_free(g_tw_pk_codec)
        return false
    }
    to_log(LOG_INFO, "dasLLAMA metal tower: pocket codec slab {bytes} bytes\n")
    return true
}

//! One codec conv over `t` input rows of stride `xs` in the CPU's first-window form; the output row count.
def private pk_conv(enc : MetalComputeEncoder?; var c : St2GpuCtx; slot : St2ConvSlot; cv : TtsConv1d;
                    bx : MetalBuffer?; t, xs : int64; by : MetalBuffer?) : int64 {
    let rows = cv.transposed ? t * cv.stride : t
    var ka = St2ConvArgs(cin = uint(cv.cin), k = uint(cv.k), stride = uint(cv.stride), pad_l = cv.transposed ? 0u : uint(cv.k - cv.stride),
        dil = uint(cv.dilation), t_in = uint(t), r0 = 0u, rows = uint(rows), xs = uint(xs))
    st2_conv_mm(enc, c, slot, cv.transposed, bx, by, ka, rows)
    st2_conv_bias(enc, c, slot, by, rows)
    return rows
}

def private pk_elu(enc : MetalComputeEncoder?; b : MetalBuffer?; rows, width : int64) {
    var ka = GkPkRowsArgs(rows = uint(rows), width = uint(width), ss = uint(width), sc = 0u, sr0 = 0u, ds = uint(width), dc = 0u, dr0 = 0u)
    enc_pk_rows_elu(enc, b, b, ka, rows * width)
}

//! The seats' host scratch, reused a chunk: the codec's stage rows, the codec transformer's per-layer K/V handles.
struct private PkGpuScratch {
    @scratch rows : array<int64>
    @scratch bk : array<MetalBuffer?>
    @scratch bv : array<MetalBuffer?>
}

var private g_tw_pk_sc : PkGpuScratch

//! The engine's rope tables for positions 0..cap-1 at the transformer's period, into two once-written buffers.
def private pk_rope_tables(tf : PocketTransformer; cap : int64; var bcos, bsin : MetalBuffer?&) : bool {
    let half = tf.d / tf.heads / 2l
    var inscope @exact_size tcos : array<float>
    var inscope @exact_size tsin : array<float>
    tts_pk_rope_tabs(tf, cap, tcos, tsin)
    bcos = metal_new_buffer_untracked(g_dev, uint64(cap * half * 4l))
    bsin = metal_new_buffer_untracked(g_dev, uint64(cap * half * 4l))
    return false if (bcos == null || bsin == null)
    st2_upload(bcos, tcos, cap * half)
    st2_upload(bsin, tsin, cap * half)
    return true
}

struct private PkTfBufs {
    bxb : MetalBuffer?
    bqkv : MetalBuffer?
    bctx : MetalBuffer?
    bh : MetalBuffer?
}

//! The transformer's buffers a chunk; the per-layer K/V handles land in the scratch's bk / bv.
def private pk_tf_bufs(var c : St2GpuCtx; tf : PocketTransformer; sl : PkCodecSlab; t : int64) : PkTfBufs {
    var b = PkTfBufs(bxb = st2_rows_buf(c, t, tf.d), bqkv = st2_rows_buf(c, t, sl.layers[0].in_proj.cout_p), bctx = st2_rows_buf(c, t, tf.d),
        bh = st2_rows_buf(c, t, sl.layers[0].ffn1.cout_p))
    g_tw_pk_sc.bk |> clear()
    g_tw_pk_sc.bv |> clear()
    g_tw_pk_sc.bk |> reserve(length(tf.layers))
    g_tw_pk_sc.bv |> reserve(length(tf.layers))
    for (_l in tf.layers) {
        g_tw_pk_sc.bk |> push(st2_rows_buf(c, t, tf.d))
        g_tw_pk_sc.bv |> push(st2_rows_buf(c, t, tf.d))
    }
    return <- b
}

//! The codec transformer over the residual rows bx [t][d] at positions 0..: the CPU's layer loop, dispatch for dispatch.
def private pk_transformer(enc : MetalComputeEncoder?; var c : St2GpuCtx; tf : PocketTransformer; sl : PkCodecSlab; b : PkTfBufs; bx : MetalBuffer?; t : int64) {
    let d = tf.d
    let dh = uint(d / tf.heads)
    let nd = t * d
    let u_d = st2_u(c, uint(d))
    let u_nd = st2_u(c, uint(nd))
    for (i in range(length(tf.layers))) {
        let s & = unsafe(sl.layers[i])
        st2_ln(enc, c, bx, b.bxb, t, d, tf.eps, c.slab, s.n1.gwoff, s.n1.gboff)
        st2_lin(enc, c, s.in_proj, b.bxb, t, b.bqkv)
        let qs = uint(s.in_proj.cout_p)
        let u_dh = st2_u(c, dh)
        let u_np = st2_u(c, uint(t * (d / 2l)))
        let u_neox = st2_u(c, 0u)
        let u_qs = st2_u(c, qs)
        enc_rope(enc, b.bqkv, 0ul, sl.bcos, sl.bsin, u_d, u_dh, u_np, u_neox, null, u_qs, t * (d / 2l))
        enc_rope(enc, b.bqkv, uint64(d * 4l), sl.bcos, sl.bsin, u_d, u_dh, u_np, u_neox, null, u_qs, t * (d / 2l))
        var kc = GkPkRowsArgs(rows = uint(t), width = uint(d), ss = qs, sc = uint(d), sr0 = 0u, ds = uint(d), dc = 0u, dr0 = 0u)
        enc_pk_rows(enc, b.bqkv, g_tw_pk_sc.bk[i], kc, nd)
        kc.sc = uint(2l * d)
        enc_pk_rows(enc, b.bqkv, g_tw_pk_sc.bv[i], kc, nd)
        var ka = PkAttnArgs(t = uint(t), qs = qs, qcol = 0u, d = uint(d), pos0 = 0u, ctx = uint(tf.context))
        enc_pk_attn(enc, b.bqkv, g_tw_pk_sc.bk[i], g_tw_pk_sc.bv[i], b.bctx, ka, tf.heads * t)
        st2_lin(enc, c, s.out_proj, b.bctx, t, b.bxb)
        if (s.has_s1) {
            enc_pk_row_scale(enc, b.bxb, 0ul, c.slab, s.scale1_off, u_d, u_nd, nd)
        }
        st2_add(enc, c, bx, b.bxb, bx, nd, 1.0)
        st2_ln(enc, c, bx, b.bxb, t, d, tf.eps, c.slab, s.n2.gwoff, s.n2.gboff)
        st2_lin(enc, c, s.ffn1, b.bxb, t, b.bh)
        let nf = t * s.ffn1.cout_p
        enc_st2_gelu_tanh(enc, b.bh, 0ul, st2_u(c, uint(nf)), nf)
        st2_lin(enc, c, s.ffn2, b.bh, t, b.bxb)
        if (s.has_s2) {
            enc_pk_row_scale(enc, b.bxb, 0ul, c.slab, s.scale2_off, u_d, u_nd, nd)
        }
        st2_add(enc, c, bx, b.bxb, bx, nd, 1.0)
    }
}

//! One ELU-conv-ELU-conv residual block over x [t][w] (a stage's rows) into y, through cc and cp.
def private pk_res_block(enc : MetalComputeEncoder?; var c : St2GpuCtx; b : PocketResConv; s1, s2 : St2ConvSlot;
                         bx, bcc, bcp, by : MetalBuffer?; t, w : int64) {
    var ka = GkPkRowsArgs(rows = uint(t), width = uint(w), ss = uint(w), sc = 0u, sr0 = 0u, ds = uint(w), dc = 0u, dr0 = 0u)
    enc_pk_rows_elu(enc, bx, bcc, ka, t * w)
    pk_conv(enc, c, s1, b.conv1, bcc, t, w, bcp)
    pk_elu(enc, bcp, t, s1.cout_p)
    pk_conv(enc, c, s2, b.conv2, bcp, t, s1.cout_p, by)
    st2_add(enc, c, by, bx, by, t * w, 1.0)
}

def private pk_codec_chain(enc : MetalComputeEncoder?; var c : St2GpuCtx; mm : PocketMimi; sl : PkCodecSlab; blat : MetalBuffer?;
                           rows : array<int64>; bwave : MetalBuffer?) {
    let d = mm.dec_tf.d
    let frames = rows[0]
    let t1 = rows[1]
    var bq = st2_rows_buf(c, frames, sl.quant.cout_p)
    pk_conv(enc, c, sl.quant, mm.quant_proj, blat, frames, mm.quant_proj.cin, bq)
    var bx = st2_rows_buf(c, t1, d)
    var kp = St2PoolArgs(c = uint(d), k = uint(mm.upsample.k), stride = uint(mm.upsample.stride), pad_l = 0u, dil = uint(mm.upsample.dilation),
        t_in = uint(frames), total = uint(t1 * d))
    enc_st2_pool_dw(enc, bq, bx, c.slab, sl.up_w, c.slab, sl.up_b, kp, t1 * d)
    var tb = pk_tf_bufs(c, mm.dec_tf, sl, t1)
    pk_transformer(enc, c, mm.dec_tf, sl, tb, bx, t1)
    var bcur = st2_rows_buf(c, t1, sl.dec_in.cout_p)
    pk_conv(enc, c, sl.dec_in, mm.dec_in, bx, t1, d, bcur)
    var t = t1
    var w = sl.dec_in.cout_p
    for (i in range64(long_length(mm.ratios))) {
        pk_elu(enc, bcur, t, w)
        let up & = unsafe(mm.dec_up[i])
        let tn = rows[2l + i]
        let wn = sl.ups[i].cout_p
        var bup = st2_rows_buf(c, tn, wn)
        pk_conv(enc, c, sl.ups[i], up, bcur, t, w, bup)
        var bcc = st2_rows_buf(c, tn, wn)
        var bcp = st2_rows_buf(c, tn, sl.res1[i].cout_p)
        var by = st2_rows_buf(c, tn, wn)
        pk_res_block(enc, c, mm.dec_res[i], sl.res1[i], sl.res2[i], bup, bcc, bcp, by, tn, wn)
        bcur = by
        t = tn
        w = wn
    }
    pk_elu(enc, bcur, t, w)
    var bo = st2_rows_buf(c, t, sl.dec_out.cout_p)
    pk_conv(enc, c, sl.dec_out, mm.dec_out, bcur, t, w, bo)
    var kw = GkPkRowsArgs(rows = uint(t), width = 1u, ss = uint(sl.dec_out.cout_p), sc = 0u, sr0 = 0u, ds = 1u, dc = 0u, dr0 = 0u)
    enc_pk_rows(enc, bo, bwave, kw, t)
}

[hot_path, unused_argument(sc), arch(at="../ARCHITECTURE_GPU_TOWER_POCKET.md#tower-pocket-codec")]   //! the carrier is the CPU chain's; the seat's rows live in the pool
def private metal_pocket_codec(m : PocketModel; latents : array<float>; frames : int64; var sc : PocketScratch; @scratch @exact_size var wave : array<float>) : bool {
    if (!g_tw_env_tower) {
        return tw_decline(MetalTowerDecline.knob)
    }
    let mm & = unsafe(m.mimi)
    if (frames < 1l || frames > PK_CODEC_MAX_FRAMES || !pk_codec_ok(mm) || m.frame_samples != mm.frame_steps * mm.hop) {
        return tw_decline(MetalTowerDecline.shape)
    }
    var tc = ref_time_ticks()
    if (!metal_tower_init() || !pk_codec_attach(mm)) {
        return tw_decline(MetalTowerDecline.device)
    }
    asr_prof_add("tts.pocket.gpu.slab", tc)
    tc = ref_time_ticks()
    residency_flush()
    var sl & = g_tw_pk_codec
    tts_pk_stage_rows(mm, frames, g_tw_pk_sc.rows)
    let samples = g_tw_pk_sc.rows[long_length(g_tw_pk_sc.rows) - 1l]
    var c = St2GpuCtx()
    st2_ctx_make(c, sl.buf, 1l, 1l, 0ul)
    c.exact = true
    var blat = st2_rows_buf(c, frames, mm.quant_proj.cin)
    st2_upload(blat, latents, frames * mm.quant_proj.cin)
    var bwave = st2_rows_buf(c, samples, 1l)
    asr_prof_add("tts.pocket.gpu.upload", tc)
    tc = ref_time_ticks()
    var err = ""
    let ran = with_compute_encoder(g_queue, err) $(enc : MetalComputeEncoder?) {   // nolint:PERF026,LINT019 — the error-text fetch runs only on the failure leg
        pk_codec_chain(enc, c, mm, sl, blat, g_tw_pk_sc.rows, bwave)
    }
    asr_prof_add("tts.pocket.gpu.chain", tc)
    if (ran) {
        unsafe(scratch_resize(wave, samples))
        st2_readback(bwave, wave, samples)
        g_tower_encodes++
        g_tower_rows += samples
    }
    st2_ctx_release(c)
    if (!ran) {
        return tw_decline(MetalTowerDecline.gpu_error)
    }
    return true
}

//! A frame-loop linear: the slab's f32 rows under the row GEMV, or a q8 region of the blob under the decode GEMV.
struct private PkLin {
    q8 : bool
    f : St2ConvSlot
    qoff : uint64        //!< the q8 route's region in the blob
    has_b : bool
    boff : uint64        //!< the q8 route's bias row in the slab, the zero row where the linear has none
    nin : int64
    nout : int64
    u_nin : MetalBuffer?
    u_nout : MetalBuffer?
}

struct private PkFrLayer {
    n1 : St2NormSlot
    in_proj : PkLin
    out_proj : PkLin
    n2 : St2NormSlot
    ffn1 : PkLin
    ffn2 : PkLin
    scale1_off : uint64
    has_s1 : bool
    scale2_off : uint64
    has_s2 : bool
}

struct private PkHeadBlock {
    ln : St2NormSlot
    adaln : PkLin
    mlp1 : PkLin
    mlp2 : PkLin
}

struct private PkFramesSlab : St2SlabBase {
    blob : MetalBuffer?      //!< the q8 route's 34B-block stream
    input_linear : PkLin
    layers : array<PkFrLayer>
    out_norm : St2NormSlot
    out_eos : PkLin
    cond_embed : PkLin
    time_const : uint64
    input_proj : PkLin
    blocks : array<PkHeadBlock>
    final_ln : St2NormSlot
    final_adaln : PkLin
    final_linear : PkLin
    std_mean : uint64        //!< emb_std then emb_mean, one row
    bos : uint64
    zero : uint64            //!< a row of zeros, the bias of a q8 linear without one
}

//! The blob's writer: the measure pass sizes it, the write pass fills it at 16-byte regions.
struct private PkBlobWriter {
    @exact_size host : array<uint8>
    n : int64
    measure : bool
}

//! One voice's device K/V rows [cap][d] per backbone layer, and the rope table for every position they can hold.
struct private PkVoiceSlot {
    meta : TtsPkVoiceMeta
    bk : array<MetalBuffer?>
    bv : array<MetalBuffer?>
    bcos : MetalBuffer?
    bsin : MetalBuffer?
}

var private g_tw_pk_frames : PkFramesSlab
var private g_tw_pk_voice : PkVoiceSlot
let private PK_ZERO_ROW = 4096l   //!< the zero bias row's width: the widest q8 linear a frame's GEMVs serve

def private pk_blob_q8(var qb : PkBlobWriter; l : TtsLinear) : uint64 {
    qb.n = round_up(qb.n, 16l)
    let at = qb.n
    qb.n += (l.nout * l.nin / Q8_BLOCK_ELEMS) * (Q8_QPB + Q8_SPB)   //! the decode GEMV reads two rows a threadgroup: the loader admits a q8 linear only 32-wide on both dims, so the second row is always a real one
    return uint64(at) if (qb.measure)
    unsafe {
        if (l.q8) {
            let lay = active_q8_repack_layout(g_active_needs_repack)
            q8_region_to_metal_blob(addr<int8 const?>(l.wq[0]), addr<float const?>(l.wqs[0]), l.nin, l.nout, g_active_needs_repack, lay.mr, lay.kgroup, lay.wbias, addr(qb.host[at]))
        } else {
            let n = l.nout * l.nin
            let f = kq_fmt_of_id(l.kq_fmt, "the tower blob")
            var inscope @exact_size rows : array<float>
            rows |> reserve_resize(n)
            for (r in range64(l.nout)) {
                kq_plane_row_f32(f, l.kq, l.ks, l.nin, l.nout, l.kq_repacked ? active_kq_layout_mr(l.kq_fmt) : 1l, r, rows, r * l.nin)
            }
            var inscope @exact_size xq : array<int8>
            var inscope @exact_size xs : array<float>
            xq |> reserve_resize(n)
            xs |> reserve_resize(n / Q8_BLOCK_ELEMS)
            quantize_q8_0_into(rows, n, xq, xs, 0l, 0l)
            q8_region_to_metal_blob(addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), l.nin, l.nout, false, 1l, 1l, 0l, addr(qb.host[at]))
        }
    }
    return uint64(at)
}

def private pk_lin_write(var w : TtsSlabWriter; var qb : PkBlobWriter; l : TtsLinear; zero : uint64) : PkLin {
    var p = PkLin(nin = l.nin, nout = l.nout, q8 = l.q8 || l.kq_fmt != 0)
    if (p.q8) {
        p.qoff = pk_blob_q8(qb, l)
        p.has_b = !empty(l.b)
        p.boff = p.has_b ? st2_slab_raw(w, l.b, l.nout) : zero
    } else {
        p.f = st2_slab_linear(w, l)
    }
    if (!w.measure) {
        p.u_nin = uniform_u32(uint(l.nin))
        p.u_nout = uniform_u32(uint(l.nout))
    }
    return p
}

def private pk_lin_release(var p : PkLin) {
    st2_slot_release(p.f)
    st2_uniform_pair_release(p.u_nin, p.u_nout)
}

def private pk_frames_write(var w : TtsSlabWriter; var qb : PkBlobWriter; m : PocketModel; var s : PkFramesSlab) {
    let d = m.backbone.d
    let ld = m.latent_dim
    s.zero = uint64(tts_slab_zero(w, PK_ZERO_ROW) * 4l)
    s.input_linear = pk_lin_write(w, qb, m.input_linear, s.zero)
    s.layers |> reserve(length(m.backbone.layers))
    for (l in m.backbone.layers) {
        var y = PkFrLayer(n1 = st2_slab_norm_rows(w, l.norm1, d), in_proj = pk_lin_write(w, qb, l.in_proj, s.zero), out_proj = pk_lin_write(w, qb, l.out_proj, s.zero),
            n2 = st2_slab_norm_rows(w, l.norm2, d), ffn1 = pk_lin_write(w, qb, l.ffn1, s.zero), ffn2 = pk_lin_write(w, qb, l.ffn2, s.zero))
        y.has_s1 = !empty(l.scale1.a)
        y.has_s2 = !empty(l.scale2.a)
        if (y.has_s1) {
            y.scale1_off = st2_slab_raw(w, l.scale1.a, d)
        }
        if (y.has_s2) {
            y.scale2_off = st2_slab_raw(w, l.scale2.a, d)
        }
        s.layers |> push(y)
    }
    s.out_norm = st2_slab_norm_rows(w, m.out_norm, d)
    s.out_eos = pk_lin_write(w, qb, m.out_eos, s.zero)
    let h & = unsafe(m.head)
    s.cond_embed = pk_lin_write(w, qb, h.cond_embed, s.zero)
    s.time_const = st2_slab_raw(w, h.time_const.a, h.dim)
    s.input_proj = pk_lin_write(w, qb, h.input_proj, s.zero)
    s.blocks |> reserve(length(h.blocks))
    for (b in h.blocks) {
        s.blocks |> push(PkHeadBlock(ln = st2_slab_norm_rows(w, b.ln, h.dim), adaln = pk_lin_write(w, qb, b.adaln, s.zero),
            mlp1 = pk_lin_write(w, qb, b.mlp1, s.zero), mlp2 = pk_lin_write(w, qb, b.mlp2, s.zero)))
    }
    s.final_ln = st2_slab_norm_rows(w, h.final_ln, h.dim)
    s.final_adaln = pk_lin_write(w, qb, h.final_adaln, s.zero)
    s.final_linear = pk_lin_write(w, qb, h.final_linear, s.zero)
    let sm = tsw_alloc(w, 2l * ld)
    for (j in range64(ld)) {
        tsw_put(w, sm + j, m.emb_std.a[j])
        tsw_put(w, sm + ld + j, m.emb_mean.a[j])
    }
    s.std_mean = uint64(sm * 4l)
    s.bos = st2_slab_raw(w, m.bos_emb.a, ld)
}

def private pk_voice_drop {
    st2_release_any(g_tw_pk_voice)
    unsafe {
        delete g_tw_pk_voice
    }
    g_tw_pk_voice.meta = TtsPkVoiceMeta()
}

//! A GEMV-served linear: q8 quants 32-wide, a K-quant plane 256-wide, f32 rows as they are.
def private pk_lin_gemv_ok(l : TtsLinear) : bool {
    return l.nin >= 1l && l.nout >= 1l && (l.q8 ? (!empty(l.wq) && l.nin % Q8_BLOCK_ELEMS == 0l) : (l.kq_fmt != 0 ? (!empty(l.kq) && l.nin % KQ_SB_ELEMS == 0l) : (!empty(l.w) || !empty(l.wt))))
}

//! the shared frames admission over the decode GEMV's lanes, every q8 route's bias within the zero row
def private pk_frames_ok(m : PocketModel) : bool {
    let d = m.backbone.d
    let n = m.head.dim
    return false if (3l * d > PK_ZERO_ROW || m.backbone.ffn > PK_ZERO_ROW || 3l * n > PK_ZERO_ROW)
    return tts_pk_frames_ok(m, int64(PK_ATTN_HS), PK_GEMV_MAX_NIN) $(l : TtsLinear) : bool {
        return pk_lin_gemv_ok(l)
    }
}

//! The frames slab and its q8 blob: the blob writer rides the slab writer's two passes, its bytes uploaded after the slab.
def private pk_frames_attach(m : PocketModel) : bool {
    let key = tts_pk_frames_key(m)
    return st2_slab_current(g_tw_pk_frames, key) || pk_frames_rebuild(m, key)
}

[cold_path]
def private pk_frames_rebuild(m : PocketModel; key : uint64) : bool {
    var inscope qb = PkBlobWriter(measure = true)
    var qbytes = 0l
    var bytes = 0l
    let ok = st2_slab_rebuild(g_tw_pk_frames, key) $(var w; var s : PkFramesSlab) {
        if (!w.measure) {
            qbytes = max(qb.n, 16l)
            qb.host |> reserve_resize(qbytes)
            qb.n = 0l
            qb.measure = false
        }
        pk_frames_write(w, qb, m, s)
        bytes = long_length(w.host) * 4l
    }
    if (!ok || qbytes == 0l) {
        return ok
    }
    g_tw_pk_frames.blob = metal_new_buffer_untracked(g_dev, uint64(qbytes))
    if (g_tw_pk_frames.blob == null) {
        st2_slab_free(g_tw_pk_frames)
        return false
    }
    unsafe {
        memcpy(metal_buffer_contents(g_tw_pk_frames.blob), addr(qb.host[0]), uint64(qbytes))
    }
    to_log(LOG_INFO, "dasLLAMA metal tower: pocket frames slab {bytes} bytes, q8 blob {qbytes} bytes\n")
    return true
}

//! Host rows [r0, r1) of one cache as the device's [pos][d] rows, written straight into the shared buffers.
[arch(at="../ARCHITECTURE_GPU_TOWER_POCKET.md#tower-pocket-frames")]
def private pk_kv_rows(kc : TtsKvCache; bk, bv : MetalBuffer?; r0, r1 : int64) {
    unsafe {
        var kp = reinterpret<float?>(metal_buffer_contents(bk))
        var vp = reinterpret<float?>(metal_buffer_contents(bv))
        kv_cache_read_rows(kc, r0, r1, r0, kp, vp)
    }
}

def private pk_voice_attach(vs : PocketVoiceState; tf : PocketTransformer) : bool
    => tts_pk_voice_current(g_tw_pk_voice.meta, vs, length(g_tw_pk_voice.bk), tf.d) || pk_voice_rebuild(vs, tf)

[cold_path]
def private pk_voice_rebuild(vs : PocketVoiceState; tf : PocketTransformer) : bool {
    var v & = g_tw_pk_voice
    pk_voice_drop()
    let cap = vs.caches[0].cap
    let d = tf.d
    v.bk |> reserve(length(vs.caches))
    v.bv |> reserve(length(vs.caches))
    for (kc in vs.caches) {
        var bk = metal_new_buffer(g_dev, uint64(cap * d * 4l))
        var bv = metal_new_buffer(g_dev, uint64(cap * d * 4l))
        if (bk == null || bv == null) {
            pk_voice_drop()
            return false
        }
        v.bk |> push(bk)
        v.bv |> push(bv)
        pk_kv_rows(kc, bk, bv, 0l, vs.len)
    }
    if (!pk_rope_tables(tf, cap, v.bcos, v.bsin)) {
        pk_voice_drop()
        return false
    }
    to_log(LOG_INFO, "dasLLAMA metal tower: pocket voice slot {long_length(vs.caches) * 2l * cap * d * 4l} bytes of K/V over {cap} rows, rope tables {cap * (d / tf.heads) * 4l} bytes\n")
    v.meta = tts_pk_voice_meta(vs, d)
    return true
}

struct private PkFrBufs {
    x : MetalBuffer?
    xb : MetalBuffer?
    qkv : MetalBuffer?
    ctx : MetalBuffer?
    h : MetalBuffer?
    hy : MetalBuffer?
    hx : MetalBuffer?
    hm : MetalBuffer?
    hmod : MetalBuffer?
    x1 : MetalBuffer?
    noise : MetalBuffer?
    forced : MetalBuffer?
    latents : MetalBuffer?
    conds : MetalBuffer?
    eos : MetalBuffer?
}

def private pk_fr_bufs(var c : St2GpuCtx; m : PocketModel; max_frames, batch, forced_n : int64) : PkFrBufs {
    let d = m.backbone.d
    let n = m.head.dim
    let ld = m.latent_dim
    return PkFrBufs(x = st2_rows_buf(c, 1l, d), xb = st2_rows_buf(c, 1l, d), qkv = st2_rows_buf(c, 1l, 3l * d), ctx = st2_rows_buf(c, 1l, d),
        h = st2_rows_buf(c, 1l, m.backbone.ffn), hy = st2_rows_buf(c, 1l, n), hx = st2_rows_buf(c, 1l, n),
        hm = st2_rows_buf(c, 1l, n), hmod = st2_rows_buf(c, 1l, 3l * n), x1 = st2_rows_buf(c, 1l, ld),
        noise = st2_rows_buf(c, batch, ld), forced = st2_rows_buf(c, max(forced_n, 1l), ld), latents = st2_rows_buf(c, max_frames, ld),
        conds = st2_rows_buf(c, max_frames, d), eos = st2_rows_buf(c, max_frames, 1l))
}

//! One GEMV site of the frame: the row in and out, and the operands its stamp's prologue and
//! epilogue read - the modulation rows, the norm's scale and shift, the slab vector, the noise
//! row and the latent row.
struct private PkGemvSite {
    bx : MetalBuffer?
    xoff : uint64
    by : MetalBuffer?
    yoff : uint64
    bmod : MetalBuffer?
    mod_off : uint64
    ln_w_off : uint64
    ln_b_off : uint64
    aux_off : uint64
    bnoise : MetalBuffer?
    noise_off : uint64
    blat : MetalBuffer?
    lat_off : uint64
}

def private pk_site(var c : St2GpuCtx; var bx : MetalBuffer?; xoff : uint64; var by : MetalBuffer?; yoff : uint64 = 0ul) : PkGemvSite {
    return PkGemvSite(bx = bx, xoff = xoff, by = by, yoff = yoff, bmod = c.slab, ln_w_off = 0ul, ln_b_off = 0ul, aux_off = 0ul, bnoise = c.slab, blat = c.slab)
}

//! The row GEMV at a site under one of the five stamp kinds.
[arch(at = "../ARCHITECTURE_GPU_TOWER_POCKET.md#tower-pocket-frames")]
def private pk_gemv(enc : MetalComputeEncoder?; var c : St2GpuCtx; p : PkLin; kind : int; s : PkGemvSite) {
    var ka = PkGemvArgs(nin = uint(p.nin), nout = uint(p.nout), stride = p.q8 ? 0u : uint(p.f.kdim_p), eps = POCKET_HEAD_LN_EPS)
    let woff = p.q8 ? 0ul : p.f.woff
    let boff = p.q8 ? p.boff : p.f.boff
    var bq = p.q8 ? g_tw_pk_frames.blob : c.slab
    let qoff = p.q8 ? p.qoff : 0ul
    if (p.q8) {
        if (kind == PK_GEMV_DOT) {
            enc_gemv(enc, bq, qoff, s.bx, s.by, p.u_nin, p.u_nout, p.nout, s.yoff, s.xoff)
            if (p.has_b) {
                enc_add_bias_rows(enc, s.by, s.yoff, c.slab, boff, p.u_nout, p.u_nout, p.nout)
            }
        } elif (kind == PK_GEMV_LN_SILU) {
            enc_pk_gemv_q8_ln_silu(enc, c.slab, boff, bq, qoff, bq, s.bx, s.xoff, s.by, s.yoff, s.bmod, s.mod_off, c.slab, s.ln_w_off, c.slab, s.ln_b_off, ka, p.nout)
        } elif (kind == PK_GEMV_GATE) {
            enc_pk_gemv_q8_gate(enc, c.slab, boff, bq, qoff, bq, s.bx, s.xoff, s.by, s.yoff, s.bmod, s.mod_off, ka, p.nout)
        } elif (kind == PK_GEMV_ADD_SILU) {
            enc_pk_gemv_q8_add_silu(enc, c.slab, boff, bq, qoff, bq, s.bx, s.xoff, s.by, s.yoff, c.slab, s.aux_off, ka, p.nout)
        } else {
            enc_pk_gemv_q8_tail(enc, c.slab, boff, bq, qoff, bq, s.bx, s.xoff, s.by, s.yoff, s.bmod, s.mod_off, c.slab, s.ln_w_off, c.slab, s.ln_b_off, c.slab, s.aux_off, s.bnoise, s.noise_off, s.blat, s.lat_off, ka, p.nout)
        }
    } else {
        if (kind == PK_GEMV_DOT) {
            enc_pk_gemv_f32(enc, c.slab, woff, c.slab, boff, s.bx, s.xoff, s.by, s.yoff, ka, p.nout)
        } elif (kind == PK_GEMV_LN_SILU) {
            enc_pk_gemv_f32_ln_silu(enc, c.slab, woff, c.slab, boff, s.bx, s.xoff, s.by, s.yoff, s.bmod, s.mod_off, c.slab, s.ln_w_off, c.slab, s.ln_b_off, ka, p.nout)
        } elif (kind == PK_GEMV_GATE) {
            enc_pk_gemv_f32_gate(enc, c.slab, woff, c.slab, boff, s.bx, s.xoff, s.by, s.yoff, s.bmod, s.mod_off, ka, p.nout)
        } elif (kind == PK_GEMV_ADD_SILU) {
            enc_pk_gemv_f32_add_silu(enc, c.slab, woff, c.slab, boff, s.bx, s.xoff, s.by, s.yoff, c.slab, s.aux_off, ka, p.nout)
        } else {
            enc_pk_gemv_f32_tail(enc, c.slab, woff, c.slab, boff, s.bx, s.xoff, s.by, s.yoff, s.bmod, s.mod_off, c.slab, s.ln_w_off, c.slab, s.ln_b_off, c.slab, s.aux_off, s.bnoise, s.noise_off, s.blat, s.lat_off, ka, p.nout)
        }
    }
}

def private pk_lin(enc : MetalComputeEncoder?; var c : St2GpuCtx; p : PkLin; var bx : MetalBuffer?; xoff : uint64; var by : MetalBuffer?; yoff : uint64) {
    pk_gemv(enc, c, p, PK_GEMV_DOT, pk_site(c, bx, xoff, by, yoff))
}

def private pk_add_ln(enc : MetalComputeEncoder?; var c : St2GpuCtx; bx, ba : MetalBuffer?; ns : St2NormSlot; by : MetalBuffer?; yoff : uint64; d : int64; eps : float) {
    let u_d = st2_u(c, uint(d))
    let u_eps = st2_uf(c, eps)
    enc_pk_add_ln(enc, bx, c.slab, ns.gwoff, c.slab, ns.gboff, by, yoff, u_d, u_eps, ba, 1l)
}

//! One backbone layer at position p: the CPU's `layer_attention` and `layer_ffn` at t = 1; the
//! residual of the layer before it (its ffn rows in xb) joins at the first norm, its own ffn rows
//! are left in xb for the next join.
def private pk_fr_layer(enc : MetalComputeEncoder?; var c : St2GpuCtx; tf : PocketTransformer; L : PkFrLayer; var b : PkFrBufs; li : int; p : int64) {
    let d = tf.d
    let u_d = st2_u(c, uint(d))
    if (li == 0) {
        st2_ln(enc, c, b.x, b.xb, 1l, d, tf.eps, c.slab, L.n1.gwoff, L.n1.gboff)
    } else {
        pk_add_ln(enc, c, b.x, b.xb, L.n1, b.xb, 0ul, d, tf.eps)
    }
    pk_lin(enc, c, L.in_proj, b.xb, 0ul, b.qkv, 0ul)
    let dh = uint(d / tf.heads)
    var kr = RopeStoreArgs(qd = uint(d), head_size = dh, npairs_q = uint(d / 2l), npairs = uint(d), neox = 0u, has_bias = 0u, rot = dh)
    enc_rpst32_c(enc, b.qkv, b.qkv, uint64(2l * d * 4l), g_tw_pk_voice.bk[li], uint64(p * d * 4l), g_tw_pk_voice.bv[li], uint64(p * d * 4l),
        g_tw_pk_voice.bcos, uint64(p * (d / tf.heads / 2l) * 4l), g_tw_pk_voice.bsin, g_one, kr, d)
    var ka = PkAttnArgs(t = 1u, qs = uint(3l * d), qcol = 0u, d = uint(d), pos0 = uint(p), ctx = uint(tf.context))
    enc_pk_attn(enc, b.qkv, g_tw_pk_voice.bk[li], g_tw_pk_voice.bv[li], b.ctx, ka, tf.heads)
    pk_lin(enc, c, L.out_proj, b.ctx, 0ul, b.xb, 0ul)
    if (L.has_s1) {
        enc_pk_row_scale(enc, b.xb, 0ul, c.slab, L.scale1_off, u_d, u_d, d)
    }
    pk_add_ln(enc, c, b.x, b.xb, L.n2, b.xb, 0ul, d, tf.eps)
    pk_lin(enc, c, L.ffn1, b.xb, 0ul, b.h, 0ul)
    enc_st2_gelu_tanh(enc, b.h, 0ul, st2_u(c, uint(tf.ffn)), tf.ffn)
    pk_lin(enc, c, L.ffn2, b.h, 0ul, b.xb, 0ul)
    if (L.has_s2) {
        enc_pk_row_scale(enc, b.xb, 0ul, c.slab, L.scale2_off, u_d, u_d, d)
    }
}

//! One frame `f` at cache position p, its noise at row `i` of the batch: the CPU's `frame_step`
//! and `head_step`, the conditioning row and the EOS logit kept, the latent denormalized.
def private pk_frame(enc : MetalComputeEncoder?; var c : St2GpuCtx; m : PocketModel; sl : PkFramesSlab; var b : PkFrBufs; p, f, i, forced_rows : int64) {
    let d = m.backbone.d
    let ld = m.latent_dim
    var bxin = c.slab
    var xoff = sl.bos
    if (f > 0l) {
        if (f <= forced_rows) {
            bxin = b.forced
            xoff = uint64((f - 1l) * ld * 4l)
        } else {
            bxin = b.x1
            xoff = 0ul
        }
    }
    pk_lin(enc, c, sl.input_linear, bxin, xoff, b.x, 0ul)
    for (li in range(length(sl.layers))) {
        pk_fr_layer(enc, c, m.backbone, sl.layers[li], b, li, p)
    }
    let coff = uint64(f * d * 4l)
    pk_add_ln(enc, c, b.x, b.xb, sl.out_norm, b.conds, coff, d, m.backbone.eps)
    pk_lin(enc, c, sl.out_eos, b.conds, coff, b.eos, uint64(f * 4l))
    let noff = uint64(i * ld * 4l)
    var s = pk_site(c, b.conds, coff, b.hy)
    s.aux_off = sl.time_const
    pk_gemv(enc, c, sl.cond_embed, PK_GEMV_ADD_SILU, s)
    pk_gemv(enc, c, sl.input_proj, PK_GEMV_DOT, pk_site(c, b.noise, noff, b.hx))
    for (blk in sl.blocks) {
        pk_gemv(enc, c, blk.adaln, PK_GEMV_DOT, pk_site(c, b.hy, 0ul, b.hmod))
        var s1 = pk_site(c, b.hx, 0ul, b.hm)
        s1.bmod = b.hmod
        s1.ln_w_off = blk.ln.gwoff
        s1.ln_b_off = blk.ln.gboff
        pk_gemv(enc, c, blk.mlp1, PK_GEMV_LN_SILU, s1)
        var s2 = pk_site(c, b.hm, 0ul, b.hx)
        s2.bmod = b.hmod
        pk_gemv(enc, c, blk.mlp2, PK_GEMV_GATE, s2)
    }
    pk_gemv(enc, c, sl.final_adaln, PK_GEMV_DOT, pk_site(c, b.hy, 0ul, b.hmod))
    var st = pk_site(c, b.hx, 0ul, b.x1)
    st.bmod = b.hmod
    st.ln_w_off = sl.final_ln.gwoff
    st.ln_b_off = sl.final_ln.gboff
    st.aux_off = sl.std_mean
    st.bnoise = b.noise
    st.noise_off = noff
    st.blat = b.latents
    st.lat_off = uint64(f * ld * 4l)
    pk_gemv(enc, c, sl.final_linear, PK_GEMV_TAIL, st)
}

//! `pocket_frames_batched` here: a command buffer a batch, noise into the shared buffer, EOS off the shared rows; frames -1 on failure
def private pk_frames_loop(m : PocketModel; var sl : PkFramesSlab; var c : St2GpuCtx; var b : PkFrBufs; base, batch, max_frames, forced_rows : int64; std : float;
                           frames_after_eos : int; captured : array<float>; var sc : PocketScratch; var f0 : int64&) : tuple<frames : int64; eos_at : int64> {
    var err = ""
    return pocket_frames_batched(m, batch, max_frames, std, frames_after_eos, captured, sc, f0, $(_k : int64) : float? {
        return unsafe(reinterpret<float?>(metal_buffer_contents(b.noise)))
    }, $(first, k : int64) : bool {
        let ran = with_compute_encoder(g_queue, err) $(enc : MetalComputeEncoder?) {   // nolint:PERF026,LINT019 — the error-text fetch runs only on the failure leg
            for (i in range64(k)) {
                pk_frame(enc, c, m, sl, b, base + first + i, first + i, i, forced_rows)
            }
        }
        if (ran) {
            g_tower_encodes++
        }
        return ran
    }, $(first, i : int64) : float {
        unsafe {
            let ep = reinterpret<float const?>(metal_buffer_contents(b.eos))
            return ep[first + i]
        }
    })
}

[hot_path, arch(at="../ARCHITECTURE_GPU_TOWER_POCKET.md#tower-pocket-frames")]
def private metal_pocket_frames(m : PocketModel; var vs : PocketVoiceState; n_txt : int64; std : float; frames_after_eos : int;
                                captured, forced : array<float>; max_frames : int64; var sc : PocketScratch) : tuple<frames : int64; eos_at : int64> {
    let none = (frames = -1l, eos_at = -1l)
    if (!g_tw_env_tower) {
        tw_decline(MetalTowerDecline.knob)
        return none
    }
    let tf & = unsafe(m.backbone)
    let d = tf.d
    let ld = m.latent_dim
    let base = vs.len + n_txt
    if (!tts_pk_frames_admit(m, vs, n_txt, max_frames, PK_ATTN_MAX_KEYS) || !pk_frames_ok(m)) {
        tw_decline(MetalTowerDecline.shape)
        return none
    }
    var tc = ref_time_ticks()
    if (!metal_tower_init() || !pk_frames_attach(m) || !pk_voice_attach(vs, tf)) {
        tw_decline(MetalTowerDecline.device)
        return none
    }
    asr_prof_add("tts.pocket.gpu.slab", tc)
    tc = ref_time_ticks()
    residency_flush()
    let dev_rows = tts_pk_prompt_dev_take(g_tw_pk_prompt_dev, vs, n_txt)
    for (li, kc in iter_range(vs.caches), vs.caches) {
        pk_kv_rows(kc, g_tw_pk_voice.bk[li], g_tw_pk_voice.bv[li], vs.len + dev_rows, base)
    }
    var sl & = g_tw_pk_frames
    let batch = min(pocket_frame_batch(), max_frames)
    let forced_rows = long_length(forced) / ld
    var c = St2GpuCtx()
    st2_ctx_make(c, sl.buf, 1l, 1l, 0ul)
    var b = pk_fr_bufs(c, m, max_frames, batch, forced_rows)
    if (forced_rows > 0l) {
        st2_upload(b.forced, forced, forced_rows * ld)
    }
    asr_prof_add("tts.pocket.gpu.upload", tc)
    tc = ref_time_ticks()
    var f0 = 0l
    let rng0 = sc.rng   //! a failed command buffer hands the CPU loop the stream as it was
    let r = pk_frames_loop(m, sl, c, b, base, batch, max_frames, forced_rows, std, frames_after_eos, captured, sc, f0)
    asr_prof_add("tts.pocket.gpu.frames", tc)
    if (r.frames >= 0l) {
        st2_readback(b.latents, sc.latents, r.frames * ld)
        st2_readback(b.conds, sc.conds, tts_pk_cond_rows(r.frames, f0) * d)
        g_tower_rows += r.frames
    }
    st2_ctx_release(c)
    if (r.frames < 0l) {
        sc.rng = rng0
        tw_decline(MetalTowerDecline.gpu_error)
        return none
    }
    return r
}

var private g_tw_pk_prompt_dev : TtsPkPromptDev

//! A frame-slab linear over `rows` rows: the f32 slot's exact GEMM, or the blob's q8 blocks under the prefill
//! ladder's q8 GEMM with the bias row after it (`bxh` the rows' half twin where that GEMM reads one).
def private pk_rows_lin(enc : MetalComputeEncoder?; var c : St2GpuCtx; p : PkLin; bx, bxh, by : MetalBuffer?; rows : int64) {
    if (!p.q8) {
        st2_lin(enc, c, p.f, bx, rows, by)
        return
    }
    let mp = round_up(rows, 32l)
    if (bxh != null) {
        enc_cvt_half(enc, bx, bxh, mp * p.nin)
    }
    pf_enc_q8_mm(enc, g_tw_pk_frames.blob, p.qoff, bx, bxh, by, p.u_nin, p.u_nout, mp, p.nout, p.nin)
    if (p.has_b) {
        let n = rows * p.nout
        let u_n = st2_u(c, uint(n))
        enc_add_bias_rows(enc, by, 0ul, c.slab, p.boff, p.u_nout, u_n, n)
    }
}

//! The backbone over the prompt's rows x [t][d] at positions pos0.. - `pk_transformer`'s loop over the frames slab's
//! layers, the keys and values into the voice slot's rows from pos0 (the slot's rope tables at that row), the last
//! layer ending at them: only the caches are read after the prompt.
def private pk_prompt_rows(enc : MetalComputeEncoder?; var c : St2GpuCtx; tf : PocketTransformer; sl : PkFramesSlab; b : PkTfBufs; bx, bxh : MetalBuffer?; t, pos0 : int64) {
    let d = tf.d
    let dh = uint(d / tf.heads)
    let tab0 = uint(pos0 * (d / tf.heads / 2l))   // the tables' row of the first prompt position
    let nd = t * d
    let u_d = st2_u(c, uint(d))
    let u_nd = st2_u(c, uint(nd))
    let v & = g_tw_pk_voice
    let last = length(tf.layers) - 1
    var kc = GkPkRowsArgs(rows = uint(t), width = uint(d), ss = uint(3l * d), sc = uint(d), sr0 = 0u, ds = uint(d), dc = 0u, dr0 = uint(pos0))
    var ra = GkRopeTabArgs(rows = uint(t), qs = uint(3l * d), col = 0u, d = uint(d), dh = dh, coff = tab0, soff = tab0)
    let pairs = t * (d / 2l)
    for (i in range(length(tf.layers))) {
        let s & = unsafe(sl.layers[i])
        let kv_only = i == last
        st2_ln(enc, c, bx, b.bxb, t, d, tf.eps, c.slab, s.n1.gwoff, s.n1.gboff)
        pk_rows_lin(enc, c, s.in_proj, b.bxb, bxh, b.bqkv, t)
        if (!kv_only) {
            ra.col = 0u
            enc_pk_rope_tab(enc, b.bqkv, v.bcos, v.bsin, ra, pairs)
        }
        ra.col = uint(d)
        enc_pk_rope_tab(enc, b.bqkv, v.bcos, v.bsin, ra, pairs)
        kc.sc = uint(d)
        enc_pk_rows(enc, b.bqkv, v.bk[i], kc, nd)
        kc.sc = uint(2l * d)
        enc_pk_rows(enc, b.bqkv, v.bv[i], kc, nd)
        continue if (kv_only)
        var ka = PkAttnArgs(t = uint(t), qs = uint(3l * d), qcol = 0u, d = uint(d), pos0 = uint(pos0), ctx = uint(tf.context))
        enc_pk_attn(enc, b.bqkv, v.bk[i], v.bv[i], b.bctx, ka, tf.heads * t)
        pk_rows_lin(enc, c, s.out_proj, b.bctx, bxh, b.bxb, t)
        if (s.has_s1) {
            enc_pk_row_scale(enc, b.bxb, 0ul, c.slab, s.scale1_off, u_d, u_nd, nd)
        }
        st2_add(enc, c, bx, b.bxb, bx, nd, 1.0)
        st2_ln(enc, c, bx, b.bxb, t, d, tf.eps, c.slab, s.n2.gwoff, s.n2.gboff)
        pk_rows_lin(enc, c, s.ffn1, b.bxb, bxh, b.bh, t)
        let nf = t * tf.ffn
        enc_st2_gelu_tanh(enc, b.bh, 0ul, st2_u(c, uint(nf)), nf)
        pk_rows_lin(enc, c, s.ffn2, b.bh, bxh, b.bxb, t)
        if (s.has_s2) {
            enc_pk_row_scale(enc, b.bxb, 0ul, c.slab, s.scale2_off, u_d, u_nd, nd)
        }
        st2_add(enc, c, bx, b.bxb, bx, nd, 1.0)
    }
}

//! The prompt's K/V rows [vs.len, vs.len + t) of every layer back from the voice slot's shared buffers into the host caches.
def private pk_prompt_readback(var vs : PocketVoiceState; t, d : int64) {
    let v & = g_tw_pk_voice
    unsafe {
        for (li, kc in iter_range(vs.caches), vs.caches) {
            let kp = reinterpret<float const?>(metal_buffer_contents(v.bk[li]))
            let vp = reinterpret<float const?>(metal_buffer_contents(v.bv[li]))
            kv_cache_append_rows(kc, kp + vs.len * d, vp + vs.len * d, t, d)
        }
    }
}

//! The text prompt's rows through the backbone after the voice's: the frames slab's layers in the rows form at
//! positions vs.len.., the keys and values into the voice slot and back to the host caches, the slot's rows left
//! for the frames call after it.
[hot_path, arch(at="../ARCHITECTURE_GPU_TOWER_POCKET.md#tower-pocket-frames")]
def private metal_pocket_prompt(m : PocketModel; var vs : PocketVoiceState; n_txt : int64; var sc : PocketScratch) : bool {
    if (!g_tw_env_tower) {
        return tw_decline(MetalTowerDecline.knob)
    }
    let tf & = unsafe(m.backbone)
    let d = tf.d
    var lin_mm_ok = tf.ffn % 64l == 0l && d % 64l == 0l
    for (l in tf.layers) {
        lin_mm_ok = lin_mm_ok && l.in_proj.nin % 32l == 0l && l.out_proj.nin % 32l == 0l && l.ffn1.nin % 32l == 0l && l.ffn2.nin % 32l == 0l
    }
    if (!tts_pk_prompt_admit(m, vs, n_txt, PK_ATTN_MAX_KEYS) || !pk_frames_ok(m) || !lin_mm_ok) {
        return tw_decline(MetalTowerDecline.shape)
    }
    var tc = ref_time_ticks()
    if (!metal_tower_init() || !pk_frames_attach(m) || !pk_voice_attach(vs, tf)) {
        return tw_decline(MetalTowerDecline.device)
    }
    asr_prof_add("tts.pocket.gpu.slab", tc)
    tc = ref_time_ticks()
    residency_flush()
    var sl & = g_tw_pk_frames
    var c = St2GpuCtx()
    st2_ctx_make(c, sl.buf, 1l, 1l, 0ul)
    c.exact = true
    var bx = st2_rows_buf(c, n_txt, d)
    st2_upload(bx, sc.rows, n_txt * d)
    var b = PkTfBufs(bxb = st2_rows_buf(c, n_txt, d), bqkv = st2_rows_buf(c, n_txt, 3l * d), bctx = st2_rows_buf(c, n_txt, d), bh = st2_rows_buf(c, n_txt, tf.ffn))
    var bxh : MetalBuffer? = null
    if (sl.blob != null && pf_q8_mm_half()) {
        bxh = st2_rows_buf(c, n_txt, (max(d, tf.ffn) + 1l) / 2l)
    }
    asr_prof_add("tts.pocket.gpu.upload", tc)
    tc = ref_time_ticks()
    var err = ""
    let ran = with_compute_encoder(g_queue, err) $(enc : MetalComputeEncoder?) {   // nolint:PERF026,LINT019 — the error-text fetch runs only on the failure leg
        pk_prompt_rows(enc, c, tf, sl, b, bx, bxh, n_txt, vs.len)
    }
    asr_prof_add("tts.pocket.gpu.prompt", tc)
    if (ran) {
        pk_prompt_readback(vs, n_txt, d)
        tts_pk_prompt_dev_set(g_tw_pk_prompt_dev, vs, n_txt)
        g_tower_encodes++
        g_tower_rows += n_txt
    }
    st2_ctx_release(c)
    if (!ran) {
        return tw_decline(MetalTowerDecline.gpu_error)
    }
    return true
}

def private metal_pocket_driver : PocketGpuDriver => PocketGpuDriver(codec = @@metal_pocket_codec, frames = @@metal_pocket_frames, prompt = @@metal_pocket_prompt)

def private metal_styletts2_driver : St2GpuDriver {
    return St2GpuDriver(albert = @@metal_styletts2_albert, text = @@metal_styletts2_text, dur_enc = @@metal_styletts2_dur_enc,
        durations = @@metal_styletts2_durations, prosody = @@metal_styletts2_prosody, decode = @@metal_styletts2_decode,
        gen = @@metal_styletts2_generator)
}

//! The q3a pad, the FastConformer twin and the StyleTTS2 slabs key on host plane addresses, which a weights-epoch drop makes lie.
def private tw_weights_drop {
    st2_slab_free(g_tw_q3a_wpad)
    st2_slab_free(g_tw_fc_twin)
    st2_slab_free(g_tw_q8)
    st2_slab_free(g_tw_conv1_pad)
    st2_slab_free(g_tw_cn_front)
    st2_slab_drop()
}

[init]
def dasllama_metal_tower_register {
    register_reload_prep(@@tw_weights_drop)
    g_tw_env_tower = g_env_metal.tower
    if (!g_tw_env_tower) {
        to_log(LOG_INFO, "dasLLAMA metal tower: DASLLAMA_METAL_TOWER=0 - every tower seat stays on the CPU chain, the StyleTTS2 synthesis and the Pocket TTS codec and frame loop included\n")
    }
    g_tw_env_tower_f16 = g_env_metal.tower_f16
    if (!g_tw_env_tower_f16) {
        to_log(LOG_INFO, "dasLLAMA metal tower: DASLLAMA_METAL_TOWER_F16=0 - the twin-W route stays off, every tower GEMM rides f32\n")
    }
    g_tw_env_tower_flash = g_env_metal.tower_flash
    if (!g_tw_env_tower_flash) {
        to_log(LOG_INFO, "dasLLAMA metal tower: DASLLAMA_METAL_TOWER_FLASH=0 - the dk72 one-pass attention stays off, the slab trio serves\n")
    }
    register_gemma4uv_gpu(@@metal_gemma4uv_encode)
    register_gemma4v_gpu(@@metal_gemma4v_blocks, @@metal_tower_serves, true)
    register_gemma3v_gpu(@@metal_gemma3v_blocks, @@metal_tower_serves)
    register_qwen3v_gpu(@@metal_qwen3v_blocks, @@metal_tower_serves)
    register_qwen25v_gpu(@@metal_qwen25v_blocks)
    register_tower_blocks_gpu(@@metal_tower_blocks, @@metal_tower_serves)
    register_tower_blocks_ln_post_gpu(@@metal_tower_blocks_ln_post)
    register_tower_blocks_tail_gpu(@@metal_tower_blocks_tail)
    register_tower_conv_gpu(@@metal_conv_frontend)
    register_qwen3a_front_gpu(@@metal_q3a_front)
    register_whisper_mel_gpu(@@metal_q3a_mel)
    register_gemma4a_gpu(@@metal_gemma4a_blocks, @@metal_tower_serves)
    register_gemma4a_chunk_gpu(@@metal_gemma4a_chunk)
    register_canary_gpu(@@metal_canary_blocks, @@metal_tower_serves)
    register_canary_front_gpu(@@metal_canary_front)
    register_parakeet_gpu(@@metal_parakeet_blocks, @@metal_tower_serves)
    register_parakeet_gpu_encode(@@metal_parakeet_encode)
    register_styletts2_gpu(metal_styletts2_driver())
    register_pocket_gpu(metal_pocket_driver())
}
