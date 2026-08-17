options gen2
options _comment_hygiene = true

module dasllama_qwen3a shared public
options _function_length = 0   // model forward/serialize walks mirror the model schema

require math
require daslib/fio
require strings
require daslib/strings_boost
require daslib/apply
require daslib/archive
require dasllama/dasllama_env
require dasllama/dasllama_plane public   // PlaneF/PlaneI8 — the borrowed weight planes; re-exported for deleters
require dasllama/dasllama_asr_types public   // TranscribeSegment/AsrCaps — the shared ASR floor
require dasllama/dasllama_math
require dasllama/dasllama_gguf
require dasllama/dasllama_audio
require dasllama/dasllama_convert   // quantize_q8_0_into — the cblob q8 image
require dasllama/dasllama_common   // apply_box_profile_runtime for the qwen3a-image identity
require dasllama/dasllama_blocks   // embed_text_rows/eval_embd_ — the ASR turn drives the qwen3 decoder
require dasllama/dasllama_image

// Qwen3-ASR audio encoder (mtmd projector type `qwen3a`): 128-mel input processed in
// 100-frame chunks — 3 stride-2 conv2d blocks per chunk (100×128×1 -> 13×16×480), features
// flattened to 7680 and projected to d_model, per-chunk positional rows 0..12, then a
// standard pre-LN transformer over an 800-frame window's positions (13 tokens per chunk)
// and a GELU MLP projector into the decoder's embedding space. The transformer reuses
// dasllama_audio's block loop (qwen3a adds a k bias, which the shared layout carries).
// Ref: tools/mtmd/models/qwen3a.cpp + mtmd_audio_preprocessor_qwen3a.

let QWEN3A_CHUNK = 100l         // mel frames per conv sub-chunk (n_window * 2)
let QWEN3A_WINDOW = 800l        // mel frames per encoder forward (n_window_infer)
let QWEN3A_TOK_PER_CHUNK = 13l  // conv output frames per chunk (100 -> 50 -> 25 -> 13)

//! The qwen3a tower: the shared AudioTower core carries the transformer blocks + post-LN
//! (`encoder_blocks` runs them); the conv2d frontend, per-chunk positions, and the MLP
//! projector live in `cblob`, with the GEMM trio (cout/mm1/mm2) served q8 from `qcblob`.
struct Qwen3aTower {
    core : AudioTower = AudioTower()   // d_model/heads/blocks/post_ln; has_k_bias; n_ctx = window positions
    proj_dim : int64                   // decoder embedding width (mm2 output)
    n_ch : int64                       // conv channel count (480)
    cq8 : bool = false                 // cblob GEMM trio (cout/mm1/mm2) quantized: qcblob/qcscales armed
    cblob : PlaneF = PlaneF()
    qcblob : PlaneI8 = PlaneI8()       // dense trio Q8_0 planes (cblob keeps just the non-GEMM rest)
    qcscales : PlaneF = PlaneF()       // per-32-block scales, indexed at woff/32
    c1_w_off : int64                   // conv2d weights, file order [oc][ic][kh][kw] (= im2col col order)
    c1_b_off : int64
    c2_w_off : int64
    c2_b_off : int64
    c3_w_off : int64
    c3_b_off : int64
    cout_w_off : int64                 // [d_model][16·n_ch]
    cout_b_off : int64
    pos_off : int64                    // 13 rows × d_model (positions reset per chunk)
    mm1_w_off : int64
    mm1_b_off : int64
    mm2_w_off : int64
    mm2_b_off : int64
    image_map : void?                  // non-null when the planes borrow a mapped .dlim (dasllama_image)
    image_bytes : uint64
}

// the tower owns exactly one thing — its image backing. Every plane borrows.
def finalize(var t : Qwen3aTower) {
    if (t.image_map != null) {
        image_backing_release(t.image_map, t.image_bytes)
        t.image_map = null
        t.image_bytes = 0ul
    }
}

// the tripwire: serialize_image_meta(Qwen3aTower) covers 17 fields (core reuses AudioTower's
// list); image_map + image_bytes are deliberate skips.
let private QWEN3A_TOWER_META_FIELDS = 17 + 2

// every meta-carried Qwen3aTower field, explicitly, both directions (see dasllama_image)
def private serialize_image_meta(var arch : Archive; var t : Qwen3aTower) {
    verify(count_meta_fields(t) == QWEN3A_TOWER_META_FIELDS)   // grew Qwen3aTower? extend this list
    serialize_image_meta(arch, t.core)
    arch |> serialize_raw(t.proj_dim)
    arch |> serialize_raw(t.n_ch)
    arch |> serialize_raw(t.cq8)
    arch |> serialize_raw(t.c1_w_off)
    arch |> serialize_raw(t.c1_b_off)
    arch |> serialize_raw(t.c2_w_off)
    arch |> serialize_raw(t.c2_b_off)
    arch |> serialize_raw(t.c3_w_off)
    arch |> serialize_raw(t.c3_b_off)
    arch |> serialize_raw(t.cout_w_off)
    arch |> serialize_raw(t.cout_b_off)
    arch |> serialize_raw(t.pos_off)
    arch |> serialize_raw(t.mm1_w_off)
    arch |> serialize_raw(t.mm1_b_off)
    arch |> serialize_raw(t.mm2_w_off)
    arch |> serialize_raw(t.mm2_b_off)
}

[unused_argument(t)]
def private image_post_load(var t : Qwen3aTower) {}   // no derived state — offsets ARE the meta

//! The mint-side carrier: ``e`` holds every scalar the meta blob carries; ``core`` is the nested
//! tower's byte group (sections `core.fblob`). Public for the off-rail suites.
struct Qwen3aStaging {
    e : Qwen3aTower = Qwen3aTower()
    cblob : array<float>
    qcblob : array<int8>
    qcscales : array<float>
    core : AudioTowerPlanes = AudioTowerPlanes()
}

def private serialize_image_meta(var arch : Archive; var st : Qwen3aStaging) {
    serialize_image_meta(arch, st.e)
}

// staging owns its plane bytes outright — every array here is heap, none is borrowed
def finalize(var st : Qwen3aStaging) {
    delete st.cblob
    delete st.qcblob
    delete st.qcscales
    delete st.core
    delete st.e
}

[unused_argument(st)]
def private image_post_load(var st : Qwen3aStaging) {}

[init]
def private register_qwen3a_image_tags {
    register_image_family_tag("qwen3a-f32")
    register_image_family_tag("qwen3a-q8")
}

// q8 is the only serving format for the shared core AND the cblob GEMM trio (board policy);
// the flag is the fp32 reference route parity legs compare against
var private g_q3a_q8 = true

//! Select the qwen3a tower's weight format for subsequent loads: q8 (the serving default) vs
//! the fp32 reference route parity legs compare against.
def set_qwen3a_q8(on : bool) {
    g_q3a_q8 = on
}

//! Load a Qwen3-ASR mmproj GGUF (projector type `qwen3a`, bf16/f16/f32 tensors). Like
//! ``load_model``, caches a box/knob-specific PREPARED IMAGE for fast reload;
//! ``DASLLAMA_IMAGE=0`` disables it, a ``.dlim`` path loads one directly.
def load_qwen3a_tower(path : string) : Qwen3aTower {
    let tag = g_q3a_q8 ? "qwen3a-q8" : "qwen3a-f32"
    if (path |> ends_with(".dlim")) {
        // named directly — no mmproj to regenerate from, so a wrong identity panics
        apply_box_profile_runtime()
        let ts_img = ref_time_ticks()
        var t = Qwen3aTower()
        if (!load_image(path, t, tag)) {
            panic("dasLLAMA: '{path}' is not a prepared qwen3a-tower image for this box/knobs (identity {image_identity(tag)}) - regenerate it from the source mmproj gguf")
        }
        to_log(LOG_INFO, "dasLLAMA: prepared qwen3a-tower image mapped in {get_time_usec(ts_img) / 1000} ms - {path}\n")
        return <- t
    }
    if (g_env_engine.image) {
        apply_box_profile_runtime()   // a backend pin must precede the identity (it names the backend)
        let img = image_path_for(path, tag)
        let ts_img = ref_time_ticks()
        var t = Qwen3aTower()
        if (load_image(img, t, tag)) {
            to_log(LOG_INFO, "dasLLAMA: prepared qwen3a-tower image mapped in {get_time_usec(ts_img) / 1000} ms - {img}\n")
            return <- t
        }
        var inscope st <- stage_qwen3a_tower(path)
        var built = Qwen3aTower()
        if (cache_via_image_staged(st, built, img, tag)) {
            dlim_gc_stale(path, tag)   // the fresh bake ON DISK proves its lane's siblings dead; an in-memory build reaps nothing
        }
        return <- built
    }
    // no persisted image: mint one in memory and serve THAT — a served tower is always
    // image-backed, so there is no second, array-owning shape of this struct to maintain
    var inscope st <- stage_qwen3a_tower(path)
    var built = Qwen3aTower()
    cache_via_image_staged(st, built, "", tag)
    return <- built
}

//! The mmproj-gguf reader proper: scalars land in the served tower, plane bytes in the staging
//! arrays beside it. Nothing here is servable — the image build is what produces a live tower.
def stage_qwen3a_tower(path : string) : Qwen3aStaging {
    var st = Qwen3aStaging()
    let f = fopen(path, "rb")
    if (f == null) {
        panic("dasLLAMA: cannot open mmproj gguf '{path}'")
    }
    fmap(f) $(var bytes : array<uint8>#) {
        var inscope m <- parse_gguf_meta(bytes)
        // qwen3a mmprojs spell the key clip.audio.projector_type (not clip.projector_type)
        let proj = (gguf_has(m, "clip.audio.projector_type") ? gguf_str(m, bytes, "clip.audio.projector_type")
            : gguf_str(m, bytes, "clip.projector_type"))
        if (proj != "qwen3a") {
            panic("dasLLAMA: qwen3a tower loader got projector type '{proj}'")
        }
        var c = AudioTower()
        c.n_mel = gguf_int(m, bytes, "clip.audio.num_mel_bins")
        c.d_model = gguf_int(m, bytes, "clip.audio.embedding_length")
        c.n_head = gguf_int(m, bytes, "clip.audio.attention.head_count")
        c.n_layer = gguf_int(m, bytes, "clip.audio.block_count")
        c.n_ff = gguf_int(m, bytes, "clip.audio.feed_forward_length")
        if (gguf_has(m, "clip.audio.attention.layer_norm_epsilon")) {
            c.eps = gguf_f32(m, bytes, "clip.audio.attention.layer_norm_epsilon")
        }
        c.has_k_bias = true
        c.n_ctx = (QWEN3A_WINDOW / QWEN3A_CHUNK) * QWEN3A_TOK_PER_CHUNK   // 104 positions per forward
        c.proj_dim = 0l   // the projector lives in cblob, not the core
        let d = c.d_model
        let ff = c.n_ff

        // core blob: the shared per-layer region + post-LN (no convs/pos/projector — offsets 0)
        var o = 0l
        c.layers_off = o
        c.layer_stride = enc_layer_stride(d, ff)
        o += c.n_layer * c.layer_stride
        c.lnp_w_off = o; o += d
        c.lnp_b_off = o; o += d
        st.core.fblob |> reserve_resize(o)
        for (l in range64(c.n_layer)) {
            let lo = enc_layer_offsets(c, l)
            gguf_read_tensor_f32(m, bytes, "a.blk.{l}.ln1.weight", st.core.fblob, lo.ln1_w, d)
            gguf_read_tensor_f32(m, bytes, "a.blk.{l}.ln1.bias", st.core.fblob, lo.ln1_b, d)
            gguf_read_tensor_f32(m, bytes, "a.blk.{l}.attn_q.weight", st.core.fblob, lo.q_w, d * d)
            gguf_read_tensor_f32(m, bytes, "a.blk.{l}.attn_q.bias", st.core.fblob, lo.q_b, d)
            gguf_read_tensor_f32(m, bytes, "a.blk.{l}.attn_k.weight", st.core.fblob, lo.k_w, d * d)
            gguf_read_tensor_f32(m, bytes, "a.blk.{l}.attn_k.bias", st.core.fblob, lo.k_b, d)
            gguf_read_tensor_f32(m, bytes, "a.blk.{l}.attn_v.weight", st.core.fblob, lo.v_w, d * d)
            gguf_read_tensor_f32(m, bytes, "a.blk.{l}.attn_v.bias", st.core.fblob, lo.v_b, d)
            gguf_read_tensor_f32(m, bytes, "a.blk.{l}.attn_out.weight", st.core.fblob, lo.o_w, d * d)
            gguf_read_tensor_f32(m, bytes, "a.blk.{l}.attn_out.bias", st.core.fblob, lo.o_b, d)
            gguf_read_tensor_f32(m, bytes, "a.blk.{l}.ln2.weight", st.core.fblob, lo.ln2_w, d)
            gguf_read_tensor_f32(m, bytes, "a.blk.{l}.ln2.bias", st.core.fblob, lo.ln2_b, d)
            gguf_read_tensor_f32(m, bytes, "a.blk.{l}.ffn_up.weight", st.core.fblob, lo.fc1_w, ff * d)
            gguf_read_tensor_f32(m, bytes, "a.blk.{l}.ffn_up.bias", st.core.fblob, lo.fc1_b, ff)
            gguf_read_tensor_f32(m, bytes, "a.blk.{l}.ffn_down.weight", st.core.fblob, lo.fc2_w, d * ff)
            gguf_read_tensor_f32(m, bytes, "a.blk.{l}.ffn_down.bias", st.core.fblob, lo.fc2_b, d)
        }
        gguf_read_tensor_f32(m, bytes, "a.post_ln.weight", st.core.fblob, c.lnp_w_off, d)
        gguf_read_tensor_f32(m, bytes, "a.post_ln.bias", st.core.fblob, c.lnp_b_off, d)
        st.e.core <- c
        if (g_q3a_q8) {
            tower_quantize(st.e.core, st.core)   // arm the shared core's q8 rail (encoder_blocks reads core.q8)
        }

        // conv frontend + projector blob
        let i3 = gguf_find_tensor(m, "a.conv2d.1.weight")
        if (i3 < 0) {
            panic("dasLLAMA: a.conv2d.1.weight missing from qwen3a mmproj")
        }
        let nch = m.tensors[i3].n_elem / 9l          // [3,3,1,nch]
        st.e.n_ch = nch
        let feat = 16l * nch                          // conv3 output H=16 × channels
        let i2 = gguf_find_tensor(m, "mm.a.mlp.2.weight")
        if (i2 < 0) {
            panic("dasLLAMA: mm.a.mlp.2.weight missing from qwen3a mmproj")
        }
        st.e.proj_dim = m.tensors[i2].n_elem / d
        var co = 0l
        st.e.c1_w_off = co; co += nch * 9l               // cin = 1
        st.e.c1_b_off = co; co += nch
        st.e.c2_w_off = co; co += nch * nch * 9l
        st.e.c2_b_off = co; co += nch
        st.e.c3_w_off = co; co += nch * nch * 9l
        st.e.c3_b_off = co; co += nch
        st.e.cout_w_off = co; co += d * feat
        st.e.cout_b_off = co; co += d
        st.e.pos_off = co; co += QWEN3A_TOK_PER_CHUNK * d
        st.e.mm1_w_off = co; co += d * d
        st.e.mm1_b_off = co; co += d
        st.e.mm2_w_off = co; co += st.e.proj_dim * d
        st.e.mm2_b_off = co; co += st.e.proj_dim
        st.cblob |> reserve_resize(co)
        gguf_read_tensor_f32(m, bytes, "a.conv2d.1.weight", st.cblob, st.e.c1_w_off, nch * 9l)
        gguf_read_tensor_f32(m, bytes, "a.conv2d.1.bias", st.cblob, st.e.c1_b_off, nch)
        gguf_read_tensor_f32(m, bytes, "a.conv2d.2.weight", st.cblob, st.e.c2_w_off, nch * nch * 9l)
        gguf_read_tensor_f32(m, bytes, "a.conv2d.2.bias", st.cblob, st.e.c2_b_off, nch)
        gguf_read_tensor_f32(m, bytes, "a.conv2d.3.weight", st.cblob, st.e.c3_w_off, nch * nch * 9l)
        gguf_read_tensor_f32(m, bytes, "a.conv2d.3.bias", st.cblob, st.e.c3_b_off, nch)
        gguf_read_tensor_f32(m, bytes, "a.conv_out.weight", st.cblob, st.e.cout_w_off, d * feat)
        if (gguf_find_tensor(m, "a.conv_out.bias") >= 0) {   // absent in the shipped mmprojs — region stays zero
            gguf_read_tensor_f32(m, bytes, "a.conv_out.bias", st.cblob, st.e.cout_b_off, d)
        }
        // positions reset per chunk — only rows 0..12 of the stored table are ever used
        gguf_read_tensor_f32(m, bytes, "a.position_embd.weight", st.cblob, st.e.pos_off, QWEN3A_TOK_PER_CHUNK * d)
        gguf_read_tensor_f32(m, bytes, "mm.a.mlp.1.weight", st.cblob, st.e.mm1_w_off, d * d)
        gguf_read_tensor_f32(m, bytes, "mm.a.mlp.1.bias", st.cblob, st.e.mm1_b_off, d)
        gguf_read_tensor_f32(m, bytes, "mm.a.mlp.2.weight", st.cblob, st.e.mm2_w_off, st.e.proj_dim * d)
        gguf_read_tensor_f32(m, bytes, "mm.a.mlp.2.bias", st.cblob, st.e.mm2_b_off, st.e.proj_dim)
        if (g_q3a_q8) {
            q3a_cblob_quantize(st.e, st.cblob, st.qcblob, st.qcscales)
        }
    }
    fclose(f)
    return <- st
}

// dense trio planes (cout/mm1/mm2) + compact cblob — the tower recipe; convs, positions, and
// biases keep their fp32 truth, every stored offset is rewritten to its serving plane
def private q3a_cblob_quantize(var t : Qwen3aTower; var cblob : array<float>; var qblob : array<int8>; var qscales : array<float>) {
    let d = t.core.d_model
    let feat = 16l * t.n_ch
    if (d % 32l != 0l || feat % 32l != 0l || t.proj_dim % 32l != 0l) {
        panic("qwen3a cblob q8: d {d} / feat {feat} / proj {t.proj_dim} are not 32-multiples - the compact trio cannot serve this tower")
    }
    let qtotal = d * feat + d * d + t.proj_dim * d
    qblob |> reserve_resize(qtotal)
    qscales |> reserve_resize(qtotal / 32l)
    var nf : array<float>
    nf |> reserve_resize(long_length(cblob) - qtotal)
    var og = 0l
    var of = 0l
    let nch = t.n_ch
    t.c1_w_off = tq_keep(nf, cblob, of, t.c1_w_off, nch * 9l)
    t.c1_b_off = tq_keep(nf, cblob, of, t.c1_b_off, nch)
    t.c2_w_off = tq_keep(nf, cblob, of, t.c2_w_off, nch * nch * 9l)
    t.c2_b_off = tq_keep(nf, cblob, of, t.c2_b_off, nch)
    t.c3_w_off = tq_keep(nf, cblob, of, t.c3_w_off, nch * nch * 9l)
    t.c3_b_off = tq_keep(nf, cblob, of, t.c3_b_off, nch)
    t.cout_w_off = tq_quant(qblob, qscales, cblob, og, t.cout_w_off, d * feat)
    t.cout_b_off = tq_keep(nf, cblob, of, t.cout_b_off, d)
    t.pos_off = tq_keep(nf, cblob, of, t.pos_off, QWEN3A_TOK_PER_CHUNK * d)
    t.mm1_w_off = tq_quant(qblob, qscales, cblob, og, t.mm1_w_off, d * d)
    t.mm1_b_off = tq_keep(nf, cblob, of, t.mm1_b_off, d)
    t.mm2_w_off = tq_quant(qblob, qscales, cblob, og, t.mm2_w_off, t.proj_dim * d)
    t.mm2_b_off = tq_keep(nf, cblob, of, t.mm2_b_off, t.proj_dim)
    verify(og == qtotal)
    delete cblob
    cblob <- nf
    t.cq8 = true
    if (!select_matmul_backend_for_load()) {
        return   // row-major backend reads the dense Q8_0 planes as-is
    }
    tw_repack(qblob, qscales, t.cout_w_off, feat, d)
    tw_repack(qblob, qscales, t.mm1_w_off, d, d)
    tw_repack(qblob, qscales, t.mm2_w_off, d, t.proj_dim)
}

//! Qwen3-ASR's log-mel (WhisperFeatureExtractor, center=True, truncation=False): 200-sample
//! reflection pad both ends, NO 30s zero tail, clamp max stays in double (unlike mtmd whisper,
//! no float round-trip); n_len = min((n_padded−400)/160+1, n_in/160+1), mel-major.
def log_mel_qwen3a(samples : array<float> | #; n_mel : int64) : WhisperMel {
    let n_in = long_length(samples)
    let head = AUDIO_N_FFT / 2l                        // 200 reflection each side
    let n_padded = n_in + 2l * head
    var padded : array<float>
    padded |> reserve_resize(int(n_padded))
    for (i in range(int(n_in))) {
        padded[int(head) + i] = samples[i]
    }
    for (i in range64(head)) {                         // reflect start: samples[200..1]
        let src = head - i
        padded[i] = src < n_in ? samples[src] : 0.0
    }
    for (i in range64(head)) {                         // reflect end: samples[n-2..n-201]
        let src = n_in - 2l - i
        padded[n_in + head + i] = src >= 0l ? samples[src] : 0.0
    }

    let n_fft = AUDIO_N_FFT
    let n_bins = n_fft / 2l + 1l
    var m = WhisperMel(n_mel = n_mel)
    let n_full = (n_padded - n_fft) / AUDIO_HOP + 1l
    m.n_len = min(n_full, n_in / AUDIO_HOP + 1l)       // the preprocessor's effective count
    m.n_len_org = m.n_len
    m.data |> reserve_resize(int(n_mel * m.n_len))

    let hann <- build_hann_window(n_fft)
    let twiddles <- build_dft_twiddles(n_fft)
    let filters <- build_mel_filterbank(n_mel, n_fft, AUDIO_SAMPLE_RATE)

    var xw : array<float>
    xw |> reserve_resize(int(m.n_len * n_fft))
    for (i in range64(m.n_len)) {
        let off = i * AUDIO_HOP
        let valid = min(n_fft, n_padded - off)
        for (j in range64(valid)) {
            xw[i * n_fft + j] = hann[j] * padded[off + j]
        }
    }
    var reim : array<float>
    reim |> reserve_resize(int(m.n_len * n_bins * 2l))
    gemm_f32(unsafe(addr(reim[0])), unsafe(addr(xw[0])), unsafe(addr(twiddles[0])),
             m.n_len, n_fft, n_bins * 2l)

    // power -> mel: float products in groups of 4 summed in float, groups accumulated in
    // double (the mtmd worker's exact unrolled accumulation), floor 2^-24, log10
    let mel_floor = 5.960464477539063e-08lf
    let log10_e = 0.43429448190325176lf
    var power : array<float>
    power |> resize(int(n_bins))
    for (i in range64(m.n_len)) {
        for (k in range64(n_bins)) {
            let re = reim[i * n_bins * 2l + k * 2l]
            let im = reim[i * n_bins * 2l + k * 2l + 1l]
            power[k] = re * re + im * im
        }
        for (j in range64(n_mel)) {
            var sum = 0.0lf
            var k = 0l
            while (k + 3l < n_bins) {
                sum += double(power[k] * filters[j * n_bins + k]
                    + power[k + 1l] * filters[j * n_bins + k + 1l]
                    + power[k + 2l] * filters[j * n_bins + k + 2l]
                    + power[k + 3l] * filters[j * n_bins + k + 3l])
                k += 4l
            }
            while (k < n_bins) {
                sum += double(power[k] * filters[j * n_bins + k])
                k++
            }
            m.data[j * m.n_len + i] = float(log(max(sum, mel_floor)) * log10_e)
        }
    }

    // global max−8 clamp + (x+4)/4, all in double (qwen3a's preprocessor shape)
    var mmax = -1e20lf
    for (v in m.data) {
        mmax = max(mmax, double(v))
    }
    mmax -= 8.0lf
    for (v in m.data) {
        v = float((max(double(v), mmax) + 4.0lf) / 4.0lf)
    }
    return <- m
}

//! Slice one encoder window off the full mel: frames [off, off+win_eff), zero-padded up to a
//! chunk multiple (the mtmd preprocessor's window shape — pad values are raw 0.0, NOT the
//! clamp floor). Returns a mel-major MelChunk of n_len = ceil(win_eff/100)·100.
def qwen3a_mel_window(m : WhisperMel; off : int64) : MelChunk {
    let win_eff = min(QWEN3A_WINDOW, m.n_len - off)
    let n_chunks = (win_eff + QWEN3A_CHUNK - 1l) / QWEN3A_CHUNK
    var c = MelChunk(n_mel = m.n_mel, n_len = n_chunks * QWEN3A_CHUNK)
    c.data |> resize(m.n_mel * c.n_len)
    for (j in range64(m.n_mel)) {
        for (i in range64(win_eff)) {
            c.data[j * c.n_len + i] = m.data[j * m.n_len + off + i]
        }
    }
    return <- c
}

// conv2d k3 s2 p1 over one [W × H × C] block via im2col + gemm. Input rows [ih][iw][c]
// (c fastest); output rows [oh][ow][c_out]. Weight flat [oc][ic][kh][kw] — the im2col column
// order (ic, kh, kw) matches the file layout, no permute.
def private conv2d_s2(@scratch var out : array<float>; @scratch var col : array<float>;
                      x : array<float>; blob : PlaneF; w_off, b_off : int64;
                      iw, ih, cin, cout : int64) {
    let ow = (iw - 1l) / 2l + 1l
    let oh = (ih - 1l) / 2l + 1l
    let kdim = cin * 9l
    let rows = oh * ow
    var tc = ref_time_ticks()
    col |> resize(rows * kdim)
    for (oy in range64(oh)) {
        for (ox in range64(ow)) {
            let r = (oy * ow + ox) * kdim
            for (ic in range64(cin)) {
                for (kh in range64(3l)) {
                    let iy = oy * 2l - 1l + kh
                    for (kw in range64(3l)) {
                        let ix = ox * 2l - 1l + kw
                        col[r + (ic * 3l + kh) * 3l + kw] = (iy >= 0l && iy < ih && ix >= 0l && ix < iw
                            ? x[(iy * iw + ix) * cin + ic]
                            : 0.0)
                    }
                }
            }
        }
    }
    asr_prof_add("q3a.conv.im2col", tc)
    tc = ref_time_ticks()
    out |> resize(rows * cout)
    mm_blob_b(out, blob, w_off, col, kdim, cout, rows)
    add_bias_rows(out, blob, b_off, cout, rows)
    gelu_erf(out, rows * cout)
    asr_prof_add("q3a.conv.mm", tc)
}

// The GPU conv-frontend hook (registered by dasllama_metal_tower at [init]): serves the three
// conv2d stages for EVERY chunk of one window, filling `y3all` ([n_chunks x 16*13*n_ch], the
// conv3 outputs), or declines with false — the CPU conv2d_s2 chain stays the fallback.
typedef Qwen3aConvGpuFn = function<(t : Qwen3aTower; win : MelChunk; n_chunks : int64; var y3all : array<float>) : bool>
var private g_q3a_conv_gpu : Qwen3aConvGpuFn
var private g_q3a_conv_gpu_set = false

def register_qwen3a_conv_gpu(fn : Qwen3aConvGpuFn) {
    g_q3a_conv_gpu = fn
    g_q3a_conv_gpu_set = true
}

//! Scratch for the qwen3a conv frontend (sized on first use, reused across windows/calls).
struct Qwen3aConvState {
    @scratch y3all : array<float>  // GPU conv path: all chunks' conv3 outputs [n_chunks x 16*13*n_ch]
    @scratch xin : array<float>    // chunk input [100·128] as [ih][iw][1]
    col : array<float>
    y1 : array<float>
    y2 : array<float>
    y3 : array<float>
    @scratch feat : array<float>   // [13 × 16·n_ch]
    @scratch crow : array<float>   // conv_out rows for one chunk [13 × d_model]
    // requant image for the cblob trio's activations (sized per encode — never the core's
    // s.xqi, which the encoder pre-sizes and writes through raw pointers)
    @scratch xq : array<int8>
    @scratch xs : array<float>
}

// q8 route for a cblob GEMM (requant-and-multiply against the trio planes; xq/xs pre-sized by
// the encode): falls back to the fp32 cblob when the region wasn't quantizable (same n%32
// condition as the repack)
def private q3a_mm_rq(t : Qwen3aTower; var y : array<float>; woff : int64; x : array<float>;
                      var xq : array<int8>; var xs : array<float>; n, d, npos : int64) {
    if (t.cq8 && n % 32l == 0l) {
        asr_requant_rows_q8(x, n, npos, xq, xs)
        matmul_q8q8_batch(y, t.qcblob, t.qcscales, woff, xq, xs, n, d, npos)
    } else {
        mm_blob_b(y, t.cblob, woff, x, n, d, npos)
    }
}

//! Encode one padded mel window (`qwen3a_mel_window` output) to soft tokens in ``s.out``:
//! ``(n_len/100)·13`` rows × ``proj_dim``. Runs the conv2d frontend per 100-frame chunk,
//! the shared transformer blocks over the window's positions, post-LN, and the MLP projector.
[hot_path]
def qwen3a_encode(t : Qwen3aTower; var s : EncoderState; var cs : Qwen3aConvState; win : MelChunk) {
    let d = t.core.d_model
    let nm = t.core.n_mel
    let nch = t.n_ch
    let n_chunks = win.n_len / QWEN3A_CHUNK
    let npos = n_chunks * QWEN3A_TOK_PER_CHUNK
    if (npos > t.core.n_ctx) {
        panic("qwen3a_encode: window has {npos} positions, tower sized for {t.core.n_ctx}")
    }
    if (t.cq8) {   // requant image for the cblob trio (cout: 13×feat rows; mm1/mm2: npos×d)
        let need = max(QWEN3A_TOK_PER_CHUNK * 16l * nch, npos * d)
        cs.xq |> grow_resize(need)
        cs.xs |> grow_resize(need / 32l)
    }

    // conv frontend per chunk -> rows of s.x; the GPU hook serves every chunk's three conv
    // stages in one pass, the shuffle/conv_out/pos tail stays CPU either way
    var tstage = ref_time_ticks()
    let gpu_convs = g_q3a_conv_gpu_set && invoke(g_q3a_conv_gpu, t, win, n_chunks, cs.y3all)
    cs.xin |> resize(QWEN3A_CHUNK * nm)
    for (ci in range64(n_chunks)) {
        if (!gpu_convs) {
            // chunk input as W=100 (frames), H=128 (mel), C=1: x[ih][iw] = mel[ih][ci·100 + iw]
            for (ih in range64(nm)) {
                for (iw in range64(QWEN3A_CHUNK)) {
                    cs.xin[ih * QWEN3A_CHUNK + iw] = win.data[ih * win.n_len + ci * QWEN3A_CHUNK + iw]
                }
            }
            conv2d_s2(cs.y1, cs.col, cs.xin, t.cblob, t.c1_w_off, t.c1_b_off, QWEN3A_CHUNK, nm, 1l, nch)   // 50×64
            conv2d_s2(cs.y2, cs.col, cs.y1, t.cblob, t.c2_w_off, t.c2_b_off, 50l, 64l, nch, nch)           // 25×32
            conv2d_s2(cs.y3, cs.col, cs.y2, t.cblob, t.c3_w_off, t.c3_b_off, 25l, 32l, nch, nch)           // 13×16
        }
        let y3base = gpu_convs ? ci * 16l * QWEN3A_TOK_PER_CHUNK * nch : 0l
        // features per output frame ow: v[h + 16·c] = y3[oh=h][ow][c]
        let feat = 16l * nch
        cs.feat |> resize(QWEN3A_TOK_PER_CHUNK * feat)
        for (ow in range64(QWEN3A_TOK_PER_CHUNK)) {
            for (h in range64(16l)) {
                for (ch in range64(nch)) {
                    let y3i = (h * QWEN3A_TOK_PER_CHUNK + ow) * nch + ch
                    cs.feat[ow * feat + h + 16l * ch] = gpu_convs ? cs.y3all[y3base + y3i] : cs.y3[y3i]
                }
            }
        }
        // conv_out rows for this chunk, + per-chunk positions 0..12, into the residual stream
        cs.crow |> resize(QWEN3A_TOK_PER_CHUNK * d)
        q3a_mm_rq(t, cs.crow, t.cout_w_off, cs.feat, cs.xq, cs.xs, feat, d, QWEN3A_TOK_PER_CHUNK)
        add_bias_rows(cs.crow, t.cblob, t.cout_b_off, d, QWEN3A_TOK_PER_CHUNK)
        let xbase = ci * QWEN3A_TOK_PER_CHUNK * d
        for (i in range64(QWEN3A_TOK_PER_CHUNK * d)) {
            s.x[xbase + i] = cs.crow[i] + t.cblob[t.pos_off + i]
        }
    }
    asr_prof_add("q3a.conv", tstage)
    tstage = ref_time_ticks()

    // transformer blocks + post-LN over the window's positions
    encoder_blocks(t.core, s, npos)
    layernorm_batch(s.xb, s.x, t.core.fblob, t.core.lnp_w_off, t.core.lnp_b_off, d, npos, t.core.eps)
    asr_prof_add("q3a.blocks", tstage)
    tstage = ref_time_ticks()

    // MLP projector: mm1 + bias -> gelu_erf -> mm2 + bias
    s.ph |> resize(npos * d)
    q3a_mm_rq(t, s.ph, t.mm1_w_off, s.xb, cs.xq, cs.xs, d, d, npos)
    add_bias_rows(s.ph, t.cblob, t.mm1_b_off, d, npos)
    gelu_erf(s.ph, npos * d)
    s.out |> resize(npos * t.proj_dim)
    q3a_mm_rq(t, s.out, t.mm2_w_off, s.ph, cs.xq, cs.xs, d, t.proj_dim, npos)
    add_bias_rows(s.out, t.cblob, t.mm2_b_off, t.proj_dim, npos)
    asr_prof_add("q3a.proj", tstage)
}

//! Soft tokens for the whole clip: run every 800-frame window and append rows (proj_dim
//! wide) to ``rows``. Returns the total token count.
def qwen3a_encode_all(t : Qwen3aTower; var s : EncoderState; var cs : Qwen3aConvState;
                      mel : WhisperMel; var rows : array<float>) : int64 {
    var total = 0l
    var off = 0l
    while (off < mel.n_len) {
        let win <- qwen3a_mel_window(mel, off)
        qwen3a_encode(t, s, cs, win)
        let n = (win.n_len / QWEN3A_CHUNK) * QWEN3A_TOK_PER_CHUNK
        rows |> reserve(long_length(rows) + n * t.proj_dim)
        for (i in range64(n * t.proj_dim)) {
            rows |> push(s.out[i])
        }
        total += n
        off += QWEN3A_WINDOW
    }
    return total
}

//! One qwen3a/qwen3omni transcription stream: the decoder session + the chunked AuT tower scratch.
struct Qwen3aAsrSession {
    qctx : Session = Session()
    qenc : EncoderState = EncoderState()
    qcs : Qwen3aConvState = Qwen3aConvState()
}

//! The tower's precision half of the facade's [[asr_exec_fmt]] stamp (the decoder tag comes
//! from the facade, which owns the shared LLM). The AuT core serves q8 GEMM planes.
def qwen3a_exec_fmt(dec_fmt : string) : string {
    return "{g_q3a_q8 ? "q8" : "f32"}/{dec_fmt}"
}

//! The family's encode-total ``asr_prof`` bucket (the facade's [[asr_encode_bucket]] arm):
//! the ASR turn and the Omni chat turn time their encodes under different keys.
def qwen3a_encode_bucket(omni : bool) : string {
    return omni ? "q3o.encode" : "q3a.encode"
}

//! Qwen3-ASR self-detects the language (``languages`` empty) and takes context biasing via the
//! session's ``context``; Qwen3-Omni is a chat model driven as a transcription turn — no prompt.
def qwen3a_asr_caps(omni : bool) : AsrCaps {
    return <- AsrCaps(translate = false, timestamps = AsrTimestamps.none,
                      streaming = false, prompt = !omni)
}

//! The facade's session arm: language validation (Qwen3-ASR only accepts "auto"), a fresh
//! decoder session, and the tower's encoder scratch.
def create_qwen3a_asr_session(dec : Model; t : Qwen3aTower; lang : string; omni : bool;
                              kv : KVDtype = KVDtype.f16) : Qwen3aAsrSession {
    if (!omni && lang != "auto") {
        panic("dasLLAMA asr: Qwen3-ASR detects the language itself - create the session with lang=\"auto\"")
    }
    return <- Qwen3aAsrSession(qctx <- create_session_(dec, kv, kv), qenc <- make_encoder_state(t.core))
}

// The qwen3a transcription turn: chatml with the audio span in <|audio_start|>/<|audio_end|>,
// the context text after it, greedy decode, and the model's `language {L}<asr_text>{text}`
// response parsed into (detected_lang, text).
def qwen3a_transcribe(dec : Model; t : Qwen3aTower; var s : Qwen3aAsrSession; context : string;
                      samples : array<float> | #; blk : block<(seg : TranscribeSegment)>;
                      var detected_lang : string&) : int {
    let tprof = ref_time_ticks()
    let mel <- log_mel_qwen3a(samples, t.core.n_mel)
    asr_prof_add("q3a.mel", tprof)
    var rows : array<float>
    let n_audio = qwen3a_encode_all(t, s.qenc, s.qcs, mel, rows)
    asr_prof_add("q3a.encode", tprof)

    let dim = dec.config.dim
    let im_start = special_id(dec, "<|im_start|>")
    let im_end = special_id(dec, "<|im_end|>")
    var head : array<int64>
    head |> push(im_start)
    head |> push_from(encode_(dec, "user\n", false, false))
    head |> push(special_id(dec, "<|audio_start|>"))
    var tail : array<int64>
    tail |> push(special_id(dec, "<|audio_end|>"))
    if (!empty(context)) {
        tail |> push_from(encode_(dec, context, false, false))
    }
    tail |> push(im_end)
    tail |> push_from(encode_(dec, "\n", false, false))
    tail |> push(im_start)
    tail |> push_from(encode_(dec, "assistant\n", false, false))

    let n_head = long_length(head)
    let n_tail = long_length(tail)
    let npos = n_head + n_audio + n_tail
    var embd : array<float>
    embd |> reserve_resize(int(npos * dim))
    embed_text_rows(dec, head, embd, 0l)
    for (i in range64(n_audio * dim)) {
        embd[n_head * dim + i] = rows[i]
    }
    embed_text_rows(dec, tail, embd, n_head + n_audio)

    s.qctx.n_past = 0l   // each transcribe is a fresh sequence; the KV rewinds to position 0
    eval_embd_(dec, s.qctx, embd, npos)

    let eot = special_id(dec, "<|endoftext|>")
    let max_new = dec.config.seq_len - npos - 1l
    var one : array<int64>
    one |> resize(1)
    var toks : array<int>
    var plog_sum = 0.0lf
    var raw : array<string>
    for (_i in range64(max_new)) {
        // greedy argmax + its logprob (logit − logsumexp) for the confidence signal
        var best = 0
        for (i in range(1, length(s.qctx.logits))) {
            if (s.qctx.logits[i] > s.qctx.logits[best]) {
                best = i
            }
        }
        let mx = s.qctx.logits[best]
        var lse = 0.0lf
        for (v in s.qctx.logits) {
            lse += double(exp(v - mx))
        }
        plog_sum += double(s.qctx.logits[best]) - (log(lse) + double(mx))
        if (int64(best) == im_end || int64(best) == eot) {
            break
        }
        toks |> push(best)
        raw |> push(piece_(dec, int64(best)))
        one[0] = int64(best)
        eval_(dec, s.qctx, one)
    }

    // parse `language {L}<asr_text>{text}` (the marker may detokenize as text or be muted)
    let full = join(raw, "")
    var text = full
    detected_lang = ""
    let mark = find(full, "<asr_text>")
    if (mark >= 0) {
        let pre = slice(full, 0, mark)
        if (starts_with(pre, "language ")) {
            detected_lang = strip(slice(pre, length("language "), length(pre)))
        }
        text = slice(full, mark + length("<asr_text>"), length(full))
    }
    var seg = TranscribeSegment(t0 = 0l, t1 = long_length(samples) * 100l / AUDIO_SAMPLE_RATE, text = text)
    seg.tokens := toks
    if (!empty(toks)) {
        seg.avg_logprob = float(plog_sum / double(length(toks) + 1))   // + the stop token's plog
    }
    blk |> invoke(seg)
    return 1
}

// Qwen3-Omni-30B transcription: the SHARED qwen3a AuT tower encodes the clip; soft tokens splice
// into a plain Qwen chatml turn, greedy-decoded until <|im_end|> (emitted into the ids — mtmd
// convention). Token-for-token vs llama-mtmd-cli --temp 0 --jinja on the same Q8 thinker + bf16 mmproj.
def qwen3omni_transcribe(dec : Model; t : Qwen3aTower; var s : Qwen3aAsrSession;
                         samples : array<float> | #; blk : block<(seg : TranscribeSegment)>) : int {
    let tp = ref_time_ticks()
    let mel <- log_mel_qwen3a(samples, t.core.n_mel)
    var arows : array<float>
    let n_audio = qwen3a_encode_all(t, s.qenc, s.qcs, mel, arows)
    asr_prof_add("q3o.encode", tp)

    let dim = dec.config.dim
    let im_start = special_id(dec, "<|im_start|>")
    let im_end = special_id(dec, "<|im_end|>")   // = the EOG (151645)
    var head : array<int64>
    head |> push(im_start)
    head |> push_from(encode_(dec, "user\n", false, false))
    head |> push(special_id(dec, "<|audio_start|>"))
    var tail : array<int64>
    tail |> push(special_id(dec, "<|audio_end|>"))
    tail |> push_from(encode_(dec, "Transcribe the audio.", false, false))
    tail |> push(im_end)
    tail |> push_from(encode_(dec, "\n", false, false))
    tail |> push(im_start)
    tail |> push_from(encode_(dec, "assistant\n", false, false))

    let n_head = long_length(head)
    let npos = n_head + n_audio + long_length(tail)
    asr_ctx_guard(n_audio, npos, dec.config.seq_len)
    var embd : array<float>
    embd |> reserve_resize(int(npos * dim))
    embed_text_rows(dec, head, embd, 0l)
    for (i in range64(n_audio * dim)) {
        embd[n_head * dim + i] = arows[i]
    }
    embed_text_rows(dec, tail, embd, n_head + n_audio)

    s.qctx.n_past = 0l   // fresh sequence per transcribe
    let td = ref_time_ticks()
    eval_embd_(dec, s.qctx, embd, npos)

    let eot = special_id(dec, "<|endoftext|>")
    let max_new = dec.config.seq_len - npos - 1l
    var one : array<int64>
    one |> resize(1)
    var toks : array<int>
    var raw : array<string>
    toks |> reserve(256)
    raw |> reserve(256)
    for (_i in range64(max_new)) {
        var best = 0
        for (i in range(1, length(s.qctx.logits))) {
            if (s.qctx.logits[i] > s.qctx.logits[best]) {
                best = i
            }
        }
        toks |> push(best)
        if (int64(best) == im_end || int64(best) == eot) {   // EOG: emitted into the ids, then stop
            break
        }
        raw |> push(piece_(dec, int64(best)))
        one[0] = int64(best)
        eval_(dec, s.qctx, one)
    }
    asr_prof_add("q3o.decode", td)

    var seg = TranscribeSegment(t0 = 0l, t1 = long_length(samples) * 100l / AUDIO_SAMPLE_RATE,
        text = strip(join(raw, "")))
    seg.tokens := toks
    blk |> invoke(seg)
    return 1
}
