options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_gemma4a shared public
options _function_length = 0   // model forward/serialize walks mirror the model schema

require dasllama/dasllama_lint public
require math
require daslib/fio
require strings
require daslib/strings_boost
require daslib/apply
require daslib/archive
require dasllama/dasllama_env
require dasllama/dasllama_math
require dasllama/dasllama_gguf
require dasllama/dasllama_plane public   // PlaneF — the borrowed weight planes; re-exported so requirers can delete an encoder
require dasllama/dasllama_asr_types public   // TranscribeSegment/AsrCaps — the shared ASR floor
require dasllama/dasllama_audio
require dasllama/dasllama_tower
require dasllama/dasllama_blocks   // embed_text_rows/eval_embd_ — the ASR turn drives the Gemma-4 decoder
require dasllama/dasllama_common   // apply_box_profile_runtime for the gemma4a-image identity
require dasllama/dasllama_image

// Gemma-4 E-series audio front end: the `gemma4a` Conformer encoder + the audio multimodal
// embedder, read straight from the bf16 audio mmproj GGUF (mmproj-gemma-4-E{2,4}B-it-bf16.gguf,
// no conversion). 12 blocks, audio d_model 1024, FFN 4096, 8 heads, 128 mel bins; projects into
// the text embedding (1536 E2B / 2560 E4B). Structure (tools/mtmd/models/gemma4a.cpp): subsample
// Conv2D ×2 (stride 2) + LayerNorm(channels)+ReLU + input proj -> per block { half-step macaron
// FFN (SiLU) · causal sliding-window (12) self-attn with sinusoidal RPE + softcap 50 · conv
// module (pw1 -> GLU -> causal depthwise k=5 -> RMSNorm -> SiLU -> pw2) · half-step macaron FFN }
// -> output proj -> RMSNorm -> mm input proj. fp32, correctness-first (encode runs once per clip,
// not the perf gate). 🔑 the conv/attn/ffn matmuls carry per-op activation-quant CALIBRATION
// ranges (input_max/min, output_max/min) — honored via clamp-in/clamp-out around each matmul.
// Oracle = llama-mtmd-cli greedy (--temp 0 --jinja), token-for-token.

let GEMMA4A_NORM_EPS = 1.0e-6      // hardcoded in mtmd's gemma4a graph (ignores the 1e-5 metadata)
let private G4A_N_FFT = 512l
let private G4A_WINDOW = 320l      // 20 ms frame (Hann), zero-padded to the 512 FFT
let private G4A_HOP = 160l
let private G4A_MEL_FLOOR = 0.001lf
let private G4A_CHUNK = 12l        // chunked local attention: chunk size
let private G4A_PAST = 12l         // max past horizon (window = self + 11 previous)
let private G4A_SOFTCAP = 50.0
let private G4A_RES_W = 0.5        // macaron half-step residual weight

//! Per-layer weight offsets into ``blob`` + the clamp ranges of the clamped matmuls.
struct private G4aLayer {
    ff_norm : int64
    ff_up : int64; ff_down : int64; ff_post_norm : int64
    c_ff_up : Clamp; c_ff_down : Clamp
    attn_pre_norm : int64
    q : int64; k : int64; v : int64; o : int64
    c_q : Clamp; c_k : Clamp; c_v : Clamp; c_o : Clamp
    per_dim_scale : int64
    attn_k_rel : int64
    attn_post_norm : int64
    norm_conv : int64
    conv_pw1 : int64; conv_dw : int64; conv_norm : int64; conv_pw2 : int64
    c_pw1 : Clamp; c_pw2 : Clamp
    ff_norm_1 : int64
    ff_up_1 : int64; ff_down_1 : int64; ff_post_norm_1 : int64
    c_ff_up_1 : Clamp; c_ff_down_1 : Clamp
    ln2 : int64
}

//! Loaded gemma4a encoder: hparams, HTK mel filterbank + 512 Hann + DFT twiddles, all-fp32 weights
//! in one ``blob``, and the projector into the decoder embedding space (``proj_dim``).
struct Gemma4aEncoder {
    d_model : int64
    n_layer : int64
    n_head : int64
    n_ff : int64
    n_mel : int64
    d_head : int64
    proj_dim : int64
    sub_ch0 : int64    // conv0 out channels (= n_mel, 128)
    sub_ch1 : int64    // conv1 out channels (32)
    n_bins : int64     // n_fft/2 + 1 = 257
    eps : float = GEMMA4A_NORM_EPS
    // q8: the GEMM set transcodes to Q8_0 at read and the offset fields index qblob/qscales
    // instead of blob (the canary pattern); convs, norms, biases and the RPE table stay fp32
    q8 : bool
    filters : PlaneF = PlaneF()    // [n_mel × n_bins] HTK, no area norm
    hann : PlaneF = PlaneF()       // [512]: 320 Hann at the front, zeros to 512
    twiddles : PlaneF = PlaneF()   // DFT twiddle matrix [n_fft × (n_bins·2)]
    pos_emb : PlaneF = PlaneF()    // sinusoidal RPE table [13 × d_model]
    ones : PlaneF = PlaneF()       // [proj_dim] weight for the final weightless RMSNorm
    blob : PlaneF = PlaneF()
    qblob : PlaneI8 = PlaneI8()
    qscales : PlaneF = PlaneF()
    // subsample conv frontend + input projection
    c0_w : int64; norm0_w : int64
    c1_w : int64; norm1_w : int64
    inproj_w : int64
    // output projection + audio multimodal embedder
    outproj_w : int64; outproj_b : int64
    mmproj_w : int64
    layers : G4aLayerPlane = G4aLayerPlane()
    image_map : void?         // non-null when the planes borrow a mapped .dlim (dasllama_image)
    image_bytes : uint64
}

// the per-layer offset table is a plane like any other; PlaneF covers the scalar element
// types, a POD-struct plane needs its own accessors
struct G4aLayerPlane {
    borrowed_plane : bool = true
    p : G4aLayer const? = null
    n : int64 = 0l
}

def length(pl : G4aLayerPlane) : int64 => pl.n

def operator [] (pl : G4aLayerPlane; i : int64) : G4aLayer {
    if (uint64(i) >= uint64(pl.n)) {
        panic("dasLLAMA gemma4a: layer {i} outside [0,{pl.n})")
    }
    return unsafe(pl.p[i])
}

def finalize(var pl : G4aLayerPlane) {
    pl.p = null
    pl.n = 0l
}

def plane_bind(var pl : G4aLayerPlane; base : uint8 const?; off, nbytes : int64) : bool {
    let esz = int64(typeinfo sizeof(type<G4aLayer>))
    if (nbytes % esz != 0l) {
        return false
    }
    unsafe {
        pl.p = reinterpret<G4aLayer const?>(base + off)
    }
    pl.n = nbytes / esz
    return true
}

// the encoder owns exactly one thing — its image backing. Every plane borrows.
def finalize(var t : Gemma4aEncoder) {
    if (t.image_map != null) {
        image_backing_release(t.image_map, t.image_bytes)
        t.image_map = null
        t.image_bytes = 0ul
    }
}

// the tripwire: serialize_image_meta(Gemma4aEncoder) covers 20 fields; image_map + image_bytes
// are deliberate skips (the eight planes and the layer table are sections, not meta).
let private GEMMA4A_META_FIELDS = 20 + 2

// every meta-carried Gemma4aEncoder field, explicitly, both directions (see dasllama_image)
def private serialize_image_meta(var arch : Archive; var t : Gemma4aEncoder) {
    verify(count_meta_fields(t) == GEMMA4A_META_FIELDS)   // grew Gemma4aEncoder? extend this list
    arch |> serialize_raw(t.d_model)
    arch |> serialize_raw(t.n_layer)
    arch |> serialize_raw(t.n_head)
    arch |> serialize_raw(t.n_ff)
    arch |> serialize_raw(t.n_mel)
    arch |> serialize_raw(t.d_head)
    arch |> serialize_raw(t.proj_dim)
    arch |> serialize_raw(t.sub_ch0)
    arch |> serialize_raw(t.sub_ch1)
    arch |> serialize_raw(t.n_bins)
    arch |> serialize_raw(t.eps)
    arch |> serialize_raw(t.q8)
    arch |> serialize_raw(t.c0_w)
    arch |> serialize_raw(t.norm0_w)
    arch |> serialize_raw(t.c1_w)
    arch |> serialize_raw(t.norm1_w)
    arch |> serialize_raw(t.inproj_w)
    arch |> serialize_raw(t.outproj_w)
    arch |> serialize_raw(t.outproj_b)
    arch |> serialize_raw(t.mmproj_w)
}

[unused_argument(t)]
def private image_post_load(var t : Gemma4aEncoder) {}   // no derived state — offsets ARE the meta

//! The mint-side carrier: ``e`` holds every scalar the meta blob carries, the arrays beside it
//! hold the plane bytes until the writer has them. Public because reading the source gguf off-rail
//! is the only way a suite can check a served encoder against something not itself image-backed.
struct Gemma4aStaging {
    e : Gemma4aEncoder = Gemma4aEncoder()
    filters : array<float>
    hann : array<float>
    twiddles : array<float>
    pos_emb : array<float>
    ones : array<float>
    blob : array<float>
    qblob : array<int8>       // section names must match the served plane fields (bind-by-name)
    qscales : array<float>
    layers : array<G4aLayer>
}

def private serialize_image_meta(var arch : Archive; var st : Gemma4aStaging) {
    serialize_image_meta(arch, st.e)
}

// staging owns its plane bytes outright — every array here is heap, none is borrowed
def finalize(var st : Gemma4aStaging) {
    apply(st) $ [unused_argument(name)] (name : string; var field) {
        static_if (typeinfo is_array(field)) {
            delete field
        }
    }
    delete st.e
}

[unused_argument(st)]
def private image_post_load(var st : Gemma4aStaging) {}

[init]
def private register_gemma4a_image_tags {
    register_image_family_tag("gemma4a-f32")
    register_image_family_tag("gemma4a-q8")
}

//! Hot-path scratch for one clip encode — a transcribe allocates nothing per call.
struct Gemma4aState {
    // mel
    mel : array<float>        // frame-major [n_len × n_mel]
    n_len : int64
    pre : array<float>
    padded : array<float>
    xw : array<float>
    reim : array<float>
    power : array<float>
    // subsample
    col : array<float>
    y0 : array<float>
    y1 : array<float>
    // encoder
    x : array<float>          // residual stream [npos × d_model]
    xb : array<float>         // norm scratch
    xc : array<float>         // clamp-input scratch (sized to max width)
    h : array<float>          // ff hidden [npos × n_ff]
    glu : array<float>        // conv GLU output [npos × d_model]
    cvd : array<float>        // depthwise conv output [npos × d_model]
    qq : array<float>
    kk : array<float>
    vv : array<float>
    ao : array<float>         // attention output [npos × d_model]
    prpe : array<float>       // projected RPE [13 × d_model]
    sc : array<float>         // per-query score window
    out : array<float>        // projected soft tokens [npos × proj_dim]
    tmp : array<float>        // output-proj scratch [npos × proj_dim]
    xqi : array<int8>         // q8 activation image — per-site requant (clamps differ per site)
    xsi : array<float>
    posf : array<float>       // RPE table copy + its q8 image, requantized once per encode
    posq : array<int8>
    poss : array<float>
}

// ===== loader =====

//! HTK triangular mel filterbank ``[n_mel][n_bins]`` (fmin=0, fmax=sr/2, no area normalization) —
//! a direct port of mtmd's ``fill_mel_filterbank_matrix`` HTK branch for gemma4a.
def private g4a_htk_filterbank(n_mel, n_fft, sample_rate : int64) : array<float> {
    let n_bins = n_fft / 2l + 1l
    let fmax = 0.5lf * double(sample_rate)
    let m_hi = 2595.0lf * log10(1.0lf + fmax / 700.0lf)
    var hz_pts : array<double>
    hz_pts |> resize(n_mel + 2l)
    for (i in range64(n_mel + 2l)) {
        let mel = m_hi * double(i) / double(n_mel + 1l)   // m_lo = hz_to_mel(0) = 0
        hz_pts[i] = 700.0lf * (pow(10.0lf, mel / 2595.0lf) - 1.0lf)
    }
    let bin_hz_step = double(sample_rate) / double(n_fft)
    var out : array<float>
    out |> resize(n_mel * n_bins)
    for (mm in range64(n_mel)) {
        let f_left = hz_pts[mm]
        let f_center = hz_pts[mm + 1l]
        let f_right = hz_pts[mm + 2l]
        let denom_l = max(1e-30lf, f_center - f_left)
        let denom_r = max(1e-30lf, f_right - f_center)
        for (k in range64(n_bins)) {
            let f = double(k) * bin_hz_step
            var w = 0.0lf
            if (f >= f_left && f <= f_center) {
                w = (f - f_left) / denom_l
            } elif (f > f_center && f <= f_right) {
                w = (f_right - f) / denom_r
            }
            out[mm * n_bins + k] = float(w)
        }
    }
    return <- out
}

//! Load a gemma4a audio mmproj GGUF (`a.*` / `mm.a.*` tensors; the vision tower is ignored).
//! The GEMM set transcodes to Q8_0 planes (forced — no fp32 weight lane); convs/norms/biases stay fp32.
//! Caches a box/knob-specific PREPARED IMAGE like ``load_model``; ``DASLLAMA_IMAGE=0`` disables, a ``.dlim`` path loads directly.
def load_gemma4a_encoder(path : string) : Gemma4aEncoder {
    let tag = g_g4a_q8 ? "gemma4a-q8" : "gemma4a-f32"
    if (path |> ends_with(".dlim")) {
        // named directly — no mmproj to regenerate from, so a wrong identity panics
        apply_box_profile_runtime()
        let ts_img = ref_time_ticks()
        var t = Gemma4aEncoder()
        if (!load_image(path, t, tag)) {
            panic("dasLLAMA: '{path}' is not a prepared gemma4a-encoder image for this box/knobs (identity {image_identity(tag)}) - regenerate it from the source mmproj gguf")
        }
        to_log(LOG_INFO, "dasLLAMA: prepared gemma4a-encoder image mapped in {get_time_usec(ts_img) / 1000} ms - {path}\n")
        return <- t
    }
    if (g_env_engine.image) {
        apply_box_profile_runtime()   // a backend pin must precede the identity (it names the backend)
        let img = image_path_for(path, tag)
        let ts_img = ref_time_ticks()
        var t = Gemma4aEncoder()
        if (load_image(img, t, tag)) {
            to_log(LOG_INFO, "dasLLAMA: prepared gemma4a-encoder image mapped in {get_time_usec(ts_img) / 1000} ms - {img}\n")
            return <- t
        }
        var inscope st <- stage_gemma4a_encoder(path)
        var built = Gemma4aEncoder()
        if (cache_via_image_staged(st, built, img, tag)) {
            dlim_gc_stale(path, tag)   // the fresh bake ON DISK proves its lane's siblings dead; an in-memory build reaps nothing
        }
        return <- built
    }
    // no persisted image: mint one in memory and serve THAT — a served encoder is always
    // image-backed, so there is no second, array-owning shape of this struct to maintain
    var inscope st <- stage_gemma4a_encoder(path)
    var built = Gemma4aEncoder()
    cache_via_image_staged(st, built, "", tag)
    return <- built
}

// GEMM read: a q8 load transcodes from the file straight into qblob/qscales (the fp32 blob
// never holds it); an fp32 load reads it like any plain tensor
def private g4a_read_gemm(m : GGUFMeta; b : array<uint8> | #; name : string; var st : Gemma4aStaging;
                          @scratch var wscratch : array<float>; woff, n_elem : int64) {
    if (!st.e.q8) {
        gguf_read_tensor_f32(m, b, name, st.blob, woff, n_elem)
        return
    }
    if (n_elem % 32l != 0l) {
        panic("dasLLAMA gemma4a q8: GEMM tensor '{name}' has {n_elem} elements - not 32-aligned")
    }
    if (long_length(wscratch) < n_elem) {
        wscratch |> reserve(n_elem)
        wscratch |> resize(n_elem)
    }
    gguf_read_tensor_f32(m, b, name, wscratch, 0l, n_elem)
    quantize_q8_0_into(wscratch, n_elem, st.qblob, st.qscales, woff, woff / 32l)
}

// repack one GEMM region for the active backend (scales index at woff/32)
def private g4a_repack(var st : Gemma4aStaging; woff, n, d : int64) {
    if (woff % 32l != 0l) {
        panic("dasLLAMA gemma4a q8: GEMM weight at {woff} is not 32-aligned")
    }
    if (n % 32l != 0l) {
        // n_elem%32==0 does not imply per-row alignment — straddling blocks serve wrong scales
        panic("dasLLAMA gemma4a q8: GEMM row width {n} is not 32-aligned - Q8_0 blocks would straddle rows")
    }
    repack_q8q8_weight(st.qblob, st.qscales, woff, n, d)
}

// repack every GEMM region for the active backend's q8q8 kernels; without a selected
// backend matmul_q8q8_batch reads the flat Q8_0 image as-is
def private g4a_repack_all(var st : Gemma4aStaging) {
    if (!st.e.q8 || !select_matmul_backend_for_load_()) {
        return
    }
    let d = st.e.d_model
    let ff = st.e.n_ff
    let feat = st.e.sub_ch1 * (st.e.n_mel / 4l)
    g4a_repack(st, st.e.inproj_w, feat, d)
    g4a_repack(st, st.e.outproj_w, d, st.e.proj_dim)
    g4a_repack(st, st.e.mmproj_w, st.e.proj_dim, st.e.proj_dim)
    for (l in range64(st.e.n_layer)) {
        let lo = st.layers[l]
        g4a_repack(st, lo.ff_up, d, ff)
        g4a_repack(st, lo.ff_down, ff, d)
        g4a_repack(st, lo.q, d, d)
        g4a_repack(st, lo.k, d, d)
        g4a_repack(st, lo.v, d, d)
        g4a_repack(st, lo.o, d, d)
        g4a_repack(st, lo.attn_k_rel, d, d)
        g4a_repack(st, lo.conv_pw1, d, 2l * d)
        g4a_repack(st, lo.conv_pw2, d, d)
        g4a_repack(st, lo.ff_up_1, d, ff)
        g4a_repack(st, lo.ff_down_1, ff, d)
    }
}

//! The mmproj-gguf reader proper: scalars land in the served encoder, plane bytes in the staging
//! arrays beside it. Nothing here is servable — the image build is what produces a live encoder.
def stage_gemma4a_encoder(path : string) : Gemma4aStaging {
    var st = Gemma4aStaging()
    st.e.q8 = g_g4a_q8
    let f = fopen(path, "rb")
    if (f == null) {
        panic("dasLLAMA gemma4a: cannot open mmproj gguf '{path}'")
    }
    fmap(f) $(var bytes : array<uint8>#) {
        var inscope m <- parse_gguf_meta(bytes)
        let proj = (gguf_has(m, "clip.audio.projector_type")
            ? gguf_str(m, bytes, "clip.audio.projector_type")
            : gguf_str(m, bytes, "clip.projector_type"))
        if (proj != "gemma4a") {
            panic("dasLLAMA gemma4a: mmproj projector type is '{proj}', expected 'gemma4a'")
        }
        st.e.n_mel = gguf_int(m, bytes, "clip.audio.num_mel_bins")
        st.e.d_model = gguf_int(m, bytes, "clip.audio.embedding_length")
        st.e.n_head = gguf_int(m, bytes, "clip.audio.attention.head_count")
        st.e.n_layer = gguf_int(m, bytes, "clip.audio.block_count")
        st.e.n_ff = gguf_int(m, bytes, "clip.audio.feed_forward_length")
        st.e.d_head = st.e.d_model / st.e.n_head
        st.e.n_bins = G4A_N_FFT / 2l + 1l
        let d = st.e.d_model
        let ff = st.e.n_ff

        // subsample channel counts from the conv tensor sizes ([3,3,cin,cout])
        let i0 = gguf_find_tensor(m, "a.conv1d.0.weight")
        let i1 = gguf_find_tensor(m, "a.conv1d.1.weight")
        if (i0 < 0 || i1 < 0) {
            panic("dasLLAMA gemma4a: missing a.conv1d.0/1.weight")
        }
        st.e.sub_ch0 = m.tensors[i0].n_elem / 9l            // cin = 1
        st.e.sub_ch1 = m.tensors[i1].n_elem / (9l * st.e.sub_ch0)
        // proj_dim from the audio multimodal embedder mm.a.input_projection [proj_dim × proj_dim]
        let ip = gguf_find_tensor(m, "mm.a.input_projection.weight")
        if (ip < 0) {
            panic("dasLLAMA gemma4a: missing mm.a.input_projection.weight")
        }
        let op = gguf_find_tensor(m, "a.pre_encode.out.weight")
        if (op < 0) {
            panic("dasLLAMA gemma4a: missing a.pre_encode.out.weight")
        }
        st.e.proj_dim = m.tensors[op].n_elem / d            // out proj [d × proj_dim]

        // blob layout: subsample + output first, then per-layer. GEMM fields ride the q cursor
        // on a q8 load (the field then indexes qblob); convs, norms, biases stay on the blob
        let q8 = st.e.q8
        var o = 0l
        var qo = 0l
        st.e.c0_w = o; o += st.e.sub_ch0 * 9l                // im2col n=9 — the conv exception
        st.e.norm0_w = o; o += st.e.sub_ch0
        st.e.c1_w = o; o += st.e.sub_ch1 * st.e.sub_ch0 * 9l
        st.e.norm1_w = o; o += st.e.sub_ch1
        let feat = st.e.sub_ch1 * (st.e.n_mel / 4l)          // 32 × 32 = 1024
        st.e.inproj_w = take_cursor(q8, o, qo, d * feat)
        st.e.outproj_w = take_cursor(q8, o, qo, st.e.proj_dim * d)
        st.e.outproj_b = o; o += st.e.proj_dim
        st.e.mmproj_w = take_cursor(q8, o, qo, st.e.proj_dim * st.e.proj_dim)
        st.layers |> resize(st.e.n_layer)
        for (l in range64(st.e.n_layer)) {
            var lo = G4aLayer()
            lo.ff_norm = o; o += d
            lo.ff_up = take_cursor(q8, o, qo, ff * d)
            lo.ff_down = take_cursor(q8, o, qo, d * ff)
            lo.ff_post_norm = o; o += d
            lo.attn_pre_norm = o; o += d
            lo.q = take_cursor(q8, o, qo, d * d)
            lo.k = take_cursor(q8, o, qo, d * d)
            lo.v = take_cursor(q8, o, qo, d * d)
            lo.o = take_cursor(q8, o, qo, d * d)
            lo.per_dim_scale = o; o += st.e.d_head
            lo.attn_k_rel = take_cursor(q8, o, qo, d * d)
            lo.attn_post_norm = o; o += d
            lo.norm_conv = o; o += d
            lo.conv_pw1 = take_cursor(q8, o, qo, 2l * d * d)
            lo.conv_dw = o; o += 5l * d
            lo.conv_norm = o; o += d
            lo.conv_pw2 = take_cursor(q8, o, qo, d * d)
            lo.ff_norm_1 = o; o += d
            lo.ff_up_1 = take_cursor(q8, o, qo, ff * d)
            lo.ff_down_1 = take_cursor(q8, o, qo, d * ff)
            lo.ff_post_norm_1 = o; o += d
            lo.ln2 = o; o += d
            st.layers[l] = lo
        }
        st.blob |> reserve(o)
        st.blob |> resize(o)
        if (q8) {
            st.qblob |> reserve(qo)
            st.qblob |> resize(qo)
            st.qscales |> reserve(qo / 32l)
            st.qscales |> resize(qo / 32l)
        }

        // read weights (GEMM set via the q8 transcode; everything else straight into the blob)
        var inscope wscratch : array<float>
        gguf_read_tensor_f32(m, bytes, "a.conv1d.0.weight", st.blob, st.e.c0_w, st.e.sub_ch0 * 9l)
        gguf_read_tensor_f32(m, bytes, "a.conv1d.0.norm.weight", st.blob, st.e.norm0_w, st.e.sub_ch0)
        gguf_read_tensor_f32(m, bytes, "a.conv1d.1.weight", st.blob, st.e.c1_w, st.e.sub_ch1 * st.e.sub_ch0 * 9l)
        gguf_read_tensor_f32(m, bytes, "a.conv1d.1.norm.weight", st.blob, st.e.norm1_w, st.e.sub_ch1)
        g4a_read_gemm(m, bytes, "a.input_projection.weight", st, wscratch, st.e.inproj_w, d * feat)
        g4a_read_gemm(m, bytes, "a.pre_encode.out.weight", st, wscratch, st.e.outproj_w, st.e.proj_dim * d)
        gguf_read_tensor_f32(m, bytes, "a.pre_encode.out.bias", st.blob, st.e.outproj_b, st.e.proj_dim)
        g4a_read_gemm(m, bytes, "mm.a.input_projection.weight", st, wscratch, st.e.mmproj_w, st.e.proj_dim * st.e.proj_dim)
        for (l in range64(st.e.n_layer)) {
            let lo = st.layers[l]
            let p = "a.blk.{l}"
            gguf_read_tensor_f32(m, bytes, "{p}.ffn_norm.weight", st.blob, lo.ff_norm, d)
            g4a_read_gemm(m, bytes, "{p}.ffn_up.weight", st, wscratch, lo.ff_up, ff * d)
            g4a_read_gemm(m, bytes, "{p}.ffn_down.weight", st, wscratch, lo.ff_down, d * ff)
            gguf_read_tensor_f32(m, bytes, "{p}.ffn_post_norm.weight", st.blob, lo.ff_post_norm, d)
            gguf_read_tensor_f32(m, bytes, "{p}.attn_pre_norm.weight", st.blob, lo.attn_pre_norm, d)
            g4a_read_gemm(m, bytes, "{p}.attn_q.weight", st, wscratch, lo.q, d * d)
            g4a_read_gemm(m, bytes, "{p}.attn_k.weight", st, wscratch, lo.k, d * d)
            g4a_read_gemm(m, bytes, "{p}.attn_v.weight", st, wscratch, lo.v, d * d)
            g4a_read_gemm(m, bytes, "{p}.attn_out.weight", st, wscratch, lo.o, d * d)
            gguf_read_tensor_f32(m, bytes, "{p}.per_dim_scale.weight", st.blob, lo.per_dim_scale, st.e.d_head)
            g4a_read_gemm(m, bytes, "{p}.attn_k_rel.weight", st, wscratch, lo.attn_k_rel, d * d)
            gguf_read_tensor_f32(m, bytes, "{p}.attn_post_norm.weight", st.blob, lo.attn_post_norm, d)
            // 🔑 conv_norm / norm_conv are name-swapped in the GGUF (upstream tensor_mapping.py):
            // the PRE-conv RMSNorm reads the tensor named "conv_norm", the POST-depthwise one "norm_conv"
            gguf_read_tensor_f32(m, bytes, "{p}.conv_norm.weight", st.blob, lo.norm_conv, d)
            g4a_read_gemm(m, bytes, "{p}.conv_pw1.weight", st, wscratch, lo.conv_pw1, 2l * d * d)
            gguf_read_tensor_f32(m, bytes, "{p}.conv_dw.weight", st.blob, lo.conv_dw, 5l * d)
            gguf_read_tensor_f32(m, bytes, "{p}.norm_conv.weight", st.blob, lo.conv_norm, d)
            g4a_read_gemm(m, bytes, "{p}.conv_pw2.weight", st, wscratch, lo.conv_pw2, d * d)
            gguf_read_tensor_f32(m, bytes, "{p}.ffn_norm_1.weight", st.blob, lo.ff_norm_1, d)
            g4a_read_gemm(m, bytes, "{p}.ffn_up_1.weight", st, wscratch, lo.ff_up_1, ff * d)
            g4a_read_gemm(m, bytes, "{p}.ffn_down_1.weight", st, wscratch, lo.ff_down_1, d * ff)
            gguf_read_tensor_f32(m, bytes, "{p}.ffn_post_norm_1.weight", st.blob, lo.ff_post_norm_1, d)
            gguf_read_tensor_f32(m, bytes, "{p}.ln2.weight", st.blob, lo.ln2, d)
            // clamp ranges of the clamped matmuls
            var lc = st.layers[l]
            lc.c_ff_up = read_clamp(m, bytes, "{p}.ffn_up")
            lc.c_ff_down = read_clamp(m, bytes, "{p}.ffn_down")
            lc.c_q = read_clamp(m, bytes, "{p}.attn_q")
            lc.c_k = read_clamp(m, bytes, "{p}.attn_k")
            lc.c_v = read_clamp(m, bytes, "{p}.attn_v")
            lc.c_o = read_clamp(m, bytes, "{p}.attn_out")
            lc.c_pw1 = read_clamp(m, bytes, "{p}.conv_pw1")
            lc.c_pw2 = read_clamp(m, bytes, "{p}.conv_pw2")
            lc.c_ff_up_1 = read_clamp(m, bytes, "{p}.ffn_up_1")
            lc.c_ff_down_1 = read_clamp(m, bytes, "{p}.ffn_down_1")
            st.layers[l] = lc
        }
    }
    fclose(f)
    g4a_repack_all(st)

    // HTK filterbank, 512 Hann (320 at the front, zeros to 512), DFT twiddles, RPE table, ones
    st.filters <- g4a_htk_filterbank(st.e.n_mel, G4A_N_FFT, AUDIO_SAMPLE_RATE)
    st.hann |> resize(G4A_N_FFT)
    for (i in range64(G4A_WINDOW)) {
        st.hann[i] = float(0.5lf - 0.5lf * double(cos(float(2.0lf * DBL_PI * double(i) / double(G4A_WINDOW)))))
    }
    st.twiddles <- build_dft_twiddles(G4A_N_FFT)
    let num_ts = st.e.d_model / 2l
    let log_ts_inc = log(10000.0) / float(max(num_ts - 1l, 1l))
    st.pos_emb |> resize((G4A_PAST + 1l) * st.e.d_model)
    for (pp in range64(G4A_PAST + 1l)) {
        let position = float(G4A_PAST - pp)
        for (i in range64(num_ts)) {
            let inv_ts = exp(-float(i) * log_ts_inc)
            let scaled = position * inv_ts
            st.pos_emb[pp * st.e.d_model + i] = sin(scaled)
            st.pos_emb[pp * st.e.d_model + i + num_ts] = cos(scaled)
        }
    }
    st.ones |> resize(st.e.proj_dim)
    for (i in range64(st.e.proj_dim)) {
        st.ones[i] = 1.0
    }
    return <- st
}

// ===== mel (gemma4a: no preemphasis, natural-log magnitude, HTK, no per-feature norm) =====

// q8 is the only serving format (board policy); the flag is the fp32 reference route parity
// legs compare against
var private g_g4a_q8 = true

//! Select the gemma4a tower's weight format for subsequent loads: q8 (the serving default) vs
//! the fp32 reference route parity legs compare against.
def set_gemma4a_q8(on : bool) {
    g_g4a_q8 = on
}

//! The tower's precision half of the facade's [[asr_exec_fmt]] stamp (the decoder tag comes
//! from the facade, which owns the shared LLM). The Conformer tower serves q8 GEMM planes.
def gemma4a_exec_fmt(dec_fmt : string) : string {
    return "{g_g4a_q8 ? "q8" : "f32"}/{dec_fmt}"
}

//! Gemma4a's encode-total ``asr_prof`` bucket (the facade's [[asr_encode_bucket]] arm);
//! the `g4a.mel` + `g4a.enc.*` sites split it under asr_stage_probe.
def gemma4a_encode_bucket : string {
    return "g4a.encode"
}

//! gemma4a log-mel for one ≤30 s chunk: semicausal left-pad (window/2) + right-pad to the PyTorch
//! frame count, 512 Hann (320-front), magnitude spectrum, HTK mel dot, natural log(sum, floor
//! 0.001). Frame-major ``[n_len × n_mel]``; ``n_len`` trimmed to the PyTorch frame count.
[hot_path]
def gemma4a_log_mel(enc : Gemma4aEncoder; samples : array<float> | #; off, len : int64; var st : Gemma4aState) {
    let n_mel = enc.n_mel
    let n_bins = enc.n_bins
    let pad_left = G4A_WINDOW / 2l
    let n_with_left = len + pad_left
    let pt_frames = max((n_with_left - (G4A_WINDOW + 1l)) / G4A_HOP + 1l, 1l)
    let n_padded_needed = (pt_frames - 1l) * G4A_HOP + G4A_N_FFT
    let total_pad = max(n_padded_needed - len, pad_left)
    let n_padded = total_pad + len
    ensure_length(st.padded, n_padded)
    for (i in range64(n_padded)) {
        st.padded[i] = 0.0
    }
    for (i in range64(len)) {
        st.padded[pad_left + i] = samples[off + i]
    }

    let n_full = (n_padded - G4A_N_FFT) / G4A_HOP + 1l
    st.n_len = min(n_full, pt_frames)
    let n_len = st.n_len
    ensure_length(st.mel, n_len * n_mel)

    // windowed frames -> DFT (twiddle gemm) -> magnitude -> mel -> natural log
    ensure_length(st.xw, n_len * G4A_N_FFT)
    for (i in range64(n_len * G4A_N_FFT)) {
        st.xw[i] = 0.0
    }
    for (i in range64(n_len)) {
        let base = i * G4A_HOP
        let valid = min(G4A_N_FFT, n_padded - base)
        for (j in range64(valid)) {
            st.xw[i * G4A_N_FFT + j] = enc.hann[j] * st.padded[base + j]
        }
    }
    ensure_length(st.reim, n_len * n_bins * 2l)
    gemm_f32(unsafe(addr(st.reim[0])), unsafe(addr(st.xw[0])), plane_at(enc.twiddles, 0l),
             n_len, G4A_N_FFT, n_bins * 2l)

    ensure_length(st.power, n_bins)
    for (i in range64(n_len)) {
        for (k in range64(n_bins)) {
            let re = st.reim[i * n_bins * 2l + k * 2l]
            let im = st.reim[i * n_bins * 2l + k * 2l + 1l]
            st.power[k] = sqrt(re * re + im * im)        // magnitude, not power
        }
        for (jm in range64(n_mel)) {
            var sum = 0.0lf
            for (k in range64(n_bins)) {
                sum += double(st.power[k] * enc.filters[jm * n_bins + k])
            }
            st.mel[i * n_mel + jm] = float(log(max(sum, G4A_MEL_FLOOR)))
        }
    }
}

// ===== subsample conv (Conv2D k3 s2 p1 + LayerNorm over channels + ReLU) =====

// im2col a [ih × iw × cin] image (k3 s2 p1) into st.col, then mm + LayerNorm(cout) + ReLU into out.
// Weight flat [cout][cin][kh][kw] (= im2col column order). LayerNorm has weight only (no bias).
def private g4a_conv_stage(enc : Gemma4aEncoder; var st : Gemma4aState; var out : array<float>;
                           x : array<float>; w_off, norm_off, iw, ih, cin, cout : int64) {
    let ow = (iw - 1l) / 2l + 1l
    let oh = (ih - 1l) / 2l + 1l
    let kdim = cin * 9l
    let rows = oh * ow
    ensure_length(st.col, rows * kdim)
    for (oy in range64(oh)) {
        for (ox in range64(ow)) {
            let r = (oy * ow + ox) * kdim
            for (ic in range64(cin)) {
                for (kh in range64(3l)) {
                    let iy = oy * 2l - 1l + kh
                    for (kw in range64(3l)) {
                        let ix = ox * 2l - 1l + kw
                        st.col[r + (ic * 3l + kh) * 3l + kw] = (iy >= 0l && iy < ih && ix >= 0l && ix < iw
                            ? x[(iy * iw + ix) * cin + ic]
                            : 0.0)
                    }
                }
            }
        }
    }
    ensure_length(out, rows * cout)
    mm_blob_b(out, enc.blob, w_off, st.col, kdim, cout, rows)
    // LayerNorm over the cout channels per spatial row (mean/var, biased), weight only, then ReLU
    let eps = enc.eps
    for (r in range64(rows)) {
        var mean = 0.0lf
        for (c in range64(cout)) {
            mean += double(out[r * cout + c])
        }
        mean /= double(cout)
        var vv = 0.0lf
        for (c in range64(cout)) {
            let dv = double(out[r * cout + c]) - mean
            vv += dv * dv
        }
        vv /= double(cout)
        let inv = 1.0lf / sqrt(vv + double(eps))
        for (c in range64(cout)) {
            let nv = float((double(out[r * cout + c]) - mean) * inv) * enc.blob[norm_off + c]
            out[r * cout + c] = max(nv, 0.0)
        }
    }
}

// ===== encoder helpers =====

// mm_blob_b with the q8 route: activation image (xq, xs) pre-quantized by the caller
def private g4a_mm(enc : Gemma4aEncoder; var y : array<float>; woff : int64; x : array<float>;
                   xq : array<int8>; xs : array<float>; n, d, npos : int64) {
    if (enc.q8) {
        matmul_q8q8_batch(y, enc.qblob, enc.qscales, woff, xq, xs, n, d, npos)
    } else {
        mm_blob_b(y, enc.blob, woff, x, n, d, npos)
    }
}

// plane-activation twin (the RPE projection: q8 reads the pre-quantized posq/poss image)
def private g4a_mm(enc : Gemma4aEncoder; var y : array<float>; woff : int64; x : PlaneF;
                   xq : array<int8>; xs : array<float>; n, d, npos : int64) {
    if (enc.q8) {
        matmul_q8q8_batch(y, enc.qblob, enc.qscales, woff, xq, xs, n, d, npos)
    } else {
        mm_blob_b(y, enc.blob, woff, x, n, d, npos)
    }
}


// requant-and-multiply — per-site requant (the clamps differ per site, so images never share)
def private g4a_mm_rq(enc : Gemma4aEncoder; var st : Gemma4aState; var y : array<float>;
                      woff : int64; x : array<float>; n, d, npos : int64) {
    if (enc.q8) {
        requant_rows_q8_sized(x, n, npos, st.xqi, st.xsi)
    }
    g4a_mm(enc, y, woff, x, st.xqi, st.xsi, n, d, npos)
}

// the RPE table is layer-invariant — copy the plane and requant its q8 image once per encode
def private g4a_requant_rpe(enc : Gemma4aEncoder; var st : Gemma4aState; d : int64) {
    if (!enc.q8) {
        return
    }
    ensure_length(st.posf, (G4A_PAST + 1l) * d)
    unsafe {
        let pp = plane_at(enc.pos_emb, 0l)
        var fp = addr(st.posf[0])
        for (i in range64((G4A_PAST + 1l) * d)) {
            fp[i] = pp[i]
        }
    }
    requant_rows_q8_sized(st.posf, d, G4A_PAST + 1l, st.posq, st.poss)
}

// clamped matmul: y = W · clamp(x); then clamp(y). x is [npos × n], y is [npos × d].
def private g4a_mm_clamped(enc : Gemma4aEncoder; var st : Gemma4aState; var y : array<float>;
                           w_off : int64; x : array<float>; n, d, npos : int64; c : Clamp) {
    if (!c.active) {
        g4a_mm_rq(enc, st, y, w_off, x, n, d, npos)
        return
    }
    ensure_length(st.xc, n * npos)
    clamp_rows(st.xc, x, n * npos, c.imin, c.imax)
    g4a_mm_rq(enc, st, y, w_off, st.xc, n, d, npos)
    clamp_rows(y, y, d * npos, c.omin, c.omax)
}

// RMSNorm each d-wide row of src into dst against a blob weight (weight-multiply, gemma4a eps).
def private g4a_rms_rows(enc : Gemma4aEncoder; var dst : array<float>; src : array<float>;
                         w_off, d, npos : int64) {
    unsafe {
        var dp = addr(dst[0])
        let sp = addr<float const?>(src[0])
        let wp = plane_at(enc.blob, w_off)
        for (r in range64(npos)) {
            rmsnorm(dp + r * d, sp + r * d, wp, d, enc.eps)
        }
    }
}

// half-step macaron FFN: RMSNorm -> up (clamp) -> SiLU -> down (clamp) -> post-RMSNorm -> res += 0.5·y
def private g4a_ffn(enc : Gemma4aEncoder; var st : Gemma4aState; norm_off, up_off, down_off,
                    post_off : int64; c_up, c_down : Clamp; d, ff, npos : int64) {
    g4a_rms_rows(enc, st.xb, st.x, norm_off, d, npos)
    g4a_mm_clamped(enc, st, st.h, up_off, st.xb, d, ff, npos, c_up)
    silu(st.h, ff * npos)
    g4a_mm_clamped(enc, st, st.xb, down_off, st.h, ff, d, npos, c_down)
    g4a_rms_rows(enc, st.xb, st.xb, post_off, d, npos)
    for (i in range64(npos * d)) {
        st.x[i] += G4A_RES_W * st.xb[i]
    }
}

// ===== encode =====

//! Encode ``st.mel`` (frame-major, one chunk) to soft tokens in ``st.out`` (``proj_dim`` wide);
//! returns the token count = subsample(n_len). Subsample -> 12 Conformer blocks -> out proj ->
//! RMSNorm -> mm input proj.
[hot_path]
def gemma4a_encode(enc : Gemma4aEncoder; var st : Gemma4aState) : int64 {
    let d = enc.d_model
    let ff = enc.n_ff
    let n_head = enc.n_head
    let dh = enc.d_head
    let n_mel = enc.n_mel

    var tprof = ref_time_ticks()
    // subsample: mel [n_len × n_mel] as image [ih=n_len][iw=n_mel][c=1]
    g4a_conv_stage(enc, st, st.y0, st.mel, enc.c0_w, enc.norm0_w, n_mel, st.n_len, 1l, enc.sub_ch0)
    let t1 = (st.n_len - 1l) / 2l + 1l
    let w1 = (n_mel - 1l) / 2l + 1l
    g4a_conv_stage(enc, st, st.y1, st.y0, enc.c1_w, enc.norm1_w, w1, t1, enc.sub_ch0, enc.sub_ch1)
    let t2 = (t1 - 1l) / 2l + 1l
    let w2 = (w1 - 1l) / 2l + 1l
    let npos = t2
    let feat = w2 * enc.sub_ch1     // 32 × 32 = 1024; y1 rows are already [freq·32 + channel]

    // input projection: feat -> d_model
    ensure_length(st.x, npos * d)
    g4a_mm_rq(enc, st, st.x, enc.inproj_w, st.y1, feat, d, npos)
    asr_prof_add("g4a.enc.front", tprof)

    ensure_length(st.xb, npos * d)
    ensure_length(st.xc, max(2l * d, ff) * npos)
    ensure_length(st.h, npos * ff)
    ensure_length(st.glu, npos * d)
    ensure_length(st.cvd, npos * d)
    ensure_length(st.qq, npos * d)
    ensure_length(st.kk, npos * d)
    ensure_length(st.vv, npos * d)
    ensure_length(st.ao, npos * d)
    ensure_length(st.prpe, (G4A_PAST + 1l) * d)
    ensure_length(st.sc, G4A_PAST + 1l)

    let q_scale = (1.0 / sqrt(float(dh))) / log(2.0)
    let k_scale = log(1.0 + exp(1.0)) / log(2.0)
    let inv_softcap = 1.0 / G4A_SOFTCAP

    g4a_requant_rpe(enc, st, d)

    for (l in range64(enc.n_layer)) {
        let lo = enc.layers[l]

        // FFN1 (macaron half)
        tprof = ref_time_ticks()
        g4a_ffn(enc, st, lo.ff_norm, lo.ff_up, lo.ff_down, lo.ff_post_norm, lo.c_ff_up, lo.c_ff_down, d, ff, npos)
        asr_prof_add("g4a.enc.ffn", tprof)

        // self-attention (causal sliding window 12, sinusoidal RPE, softcap 50)
        tprof = ref_time_ticks()
        g4a_rms_rows(enc, st.xb, st.x, lo.attn_pre_norm, d, npos)
        g4a_mm_clamped(enc, st, st.qq, lo.q, st.xb, d, d, npos, lo.c_q)
        g4a_mm_clamped(enc, st, st.kk, lo.k, st.xb, d, d, npos, lo.c_k)
        g4a_mm_clamped(enc, st, st.vv, lo.v, st.xb, d, d, npos, lo.c_v)
        asr_prof_add("g4a.enc.attn_mm", tprof)
        tprof = ref_time_ticks()
        // scale Q (q_scale · per_dim_scale) and K (k_scale)
        for (i in range64(npos)) {
            for (hh in range64(n_head)) {
                for (j in range64(dh)) {
                    st.qq[i * d + hh * dh + j] *= q_scale * enc.blob[lo.per_dim_scale + j]
                }
            }
        }
        for (i in range64(npos * d)) {
            st.kk[i] *= k_scale
        }
        // project the RPE table: prpe [13 × d] — the q8 image was requantized before the loop
        g4a_mm(enc, st.prpe, lo.attn_k_rel, enc.pos_emb, st.posq, st.poss, d, d, G4A_PAST + 1l)
        for (hh in range64(n_head)) {
            let hoff = hh * dh
            for (i in range64(npos)) {
                let j0 = max(0l, i - (G4A_PAST - 1l))
                let cnt = i - j0 + 1l
                for (j in range64(j0, i + 1l)) {
                    let rel = i - j                        // 0..11
                    let rr = (G4A_PAST - rel) * d           // RPE row 12 - d
                    var cs = 0.0
                    var rs = 0.0
                    for (n in range64(dh)) {
                        let qv = st.qq[i * d + hoff + n]
                        cs += qv * st.kk[j * d + hoff + n]
                        rs += qv * st.prpe[rr + hoff + n]
                    }
                    var score = cs + rs
                    score = G4A_SOFTCAP * tanh(score * inv_softcap)
                    st.sc[j - j0] = score
                }
                softmax(st.sc, cnt)
                for (n in range64(dh)) {
                    var acc = 0.0
                    for (j in range64(j0, i + 1l)) {
                        acc += st.sc[j - j0] * st.vv[j * d + hoff + n]
                    }
                    st.ao[i * d + hoff + n] = acc
                }
            }
        }
        asr_prof_add("g4a.enc.attn_core", tprof)
        tprof = ref_time_ticks()
        g4a_mm_clamped(enc, st, st.xb, lo.o, st.ao, d, d, npos, lo.c_o)
        g4a_rms_rows(enc, st.xb, st.xb, lo.attn_post_norm, d, npos)
        add_inplace_rows(st.x, st.xb, npos * d)
        asr_prof_add("g4a.enc.attn_mm", tprof)

        // conv module: RMSNorm -> pw1 (clamp) -> GLU -> causal depthwise k=5 -> RMSNorm -> SiLU -> pw2 (clamp)
        tprof = ref_time_ticks()
        g4a_rms_rows(enc, st.xb, st.x, lo.norm_conv, d, npos)
        g4a_mm_clamped(enc, st, st.h, lo.conv_pw1, st.xb, d, 2l * d, npos, lo.c_pw1)
        for (t in range64(npos)) {
            for (c in range64(d)) {
                let sig = st.h[t * 2l * d + c]
                let gate = st.h[t * 2l * d + d + c]
                st.glu[t * d + c] = sig * (1.0 / (1.0 + exp(-gate)))
            }
        }
        for (t in range64(npos)) {
            for (c in range64(d)) {
                var acc = 0.0
                for (k in range64(5l)) {
                    let src = t - 4l + k
                    if (src >= 0l) {
                        acc += enc.blob[lo.conv_dw + c * 5l + k] * st.glu[src * d + c]
                    }
                }
                st.cvd[t * d + c] = acc
            }
        }
        g4a_rms_rows(enc, st.cvd, st.cvd, lo.conv_norm, d, npos)
        silu(st.cvd, npos * d)
        g4a_mm_clamped(enc, st, st.xb, lo.conv_pw2, st.cvd, d, d, npos, lo.c_pw2)
        add_inplace_rows(st.x, st.xb, npos * d)
        asr_prof_add("g4a.enc.convmod", tprof)

        // FFN2 (macaron half)
        tprof = ref_time_ticks()
        g4a_ffn(enc, st, lo.ff_norm_1, lo.ff_up_1, lo.ff_down_1, lo.ff_post_norm_1, lo.c_ff_up_1, lo.c_ff_down_1, d, ff, npos)

        // per-layer output norm
        g4a_rms_rows(enc, st.x, st.x, lo.ln2, d, npos)
        asr_prof_add("g4a.enc.ffn", tprof)
    }

    // output projection -> RMSNorm (weightless) -> mm input projection
    tprof = ref_time_ticks()
    ensure_length(st.tmp, npos * enc.proj_dim)
    ensure_length(st.out, npos * enc.proj_dim)
    g4a_mm_rq(enc, st, st.tmp, enc.outproj_w, st.x, d, enc.proj_dim, npos)
    add_bias_rows(st.tmp, enc.blob, enc.outproj_b, enc.proj_dim, npos)
    unsafe {
        var tp = addr(st.tmp[0])
        let wp = plane_at(enc.ones, 0l)
        for (r in range64(npos)) {
            rmsnorm(tp + r * enc.proj_dim, tp + r * enc.proj_dim, wp, enc.proj_dim, enc.eps)
        }
    }
    g4a_mm_rq(enc, st, st.out, enc.mmproj_w, st.tmp, enc.proj_dim, enc.proj_dim, npos)
    asr_prof_add("g4a.enc.proj", tprof)
    return npos
}

//! Soft tokens for the whole clip: encode every ≤30 s chunk and append its rows (``proj_dim``
//! wide) to ``rows``. Returns the total token count.
def gemma4a_encode_all(enc : Gemma4aEncoder; var st : Gemma4aState; samples : array<float> | #;
                       var rows : array<float>) : int64 {
    let n_in = long_length(samples)
    let chunk = 30l * AUDIO_SAMPLE_RATE
    var total = 0l
    var off = 0l
    while (off < n_in) {
        let len = min(chunk, n_in - off)
        let tmel = ref_time_ticks()
        gemma4a_log_mel(enc, samples, off, len, st)
        asr_prof_add("g4a.mel", tmel)
        let n = gemma4a_encode(enc, st)
        rows |> reserve(int(long_length(rows) + n * enc.proj_dim))
        for (i in range64(n * enc.proj_dim)) {
            rows |> push(st.out[i])
        }
        total += n
        off += chunk
    }
    return total
}

//! One gemma4a transcription stream: the Gemma-4 decoder session + the Conformer encoder scratch.
struct Gemma4aAsrSession {
    qctx : Session = Session()
    g4st : Gemma4aState = Gemma4aState()
}

//! Gemma-4 E-series audio: multimodal chat model; the ASR rail drives a greedy transcription
//! turn. No timestamps; the model reasons then transcribes.
def gemma4a_asr_caps() : AsrCaps {
    var c = AsrCaps(translate = false, timestamps = AsrTimestamps.none,
                    streaming = false, prompt = false)
    c.languages <- ["en"]
    return <- c
}

//! The facade's session arm: a fresh decoder session (no language conditioning).
def create_gemma4a_asr_session(dec : Model; kv : KVDtype = KVDtype.f16) : Gemma4aAsrSession {
    return <- Gemma4aAsrSession(qctx <- create_session_(dec, kv, kv))
}

// Gemma-4 E-series audio transcription: gemma4a Conformer encode + mm embedder, soft tokens spliced
// into the gemma-4 chat template, greedy decode until `<turn|>`/`<eos>` (emitted into the ids). The
// reasoning `<|channel>...<channel|>` prefix is stripped. Token-for-token vs llama-mtmd-cli --temp 0 --jinja.
def gemma4a_transcribe(dec : Model; enc : Gemma4aEncoder; var s : Gemma4aAsrSession;
                       samples : array<float> | #; blk : block<(seg : TranscribeSegment)>;
                       var detected_lang : string&) : int {
    var tp = ref_time_ticks()
    var rows : array<float>
    let n_audio = gemma4a_encode_all(enc, s.g4st, samples, rows)
    asr_prof_add("g4a.encode", tp)

    let dim = dec.config.dim
    let nl = encode_(dec, "\n", false, false)
    // no-think turn: a bare user turn (no system/`<|think|>`) yields a deterministic transcription
    // — the reasoning channel is near-tie-brittle across float engines, so it is disabled here.
    var head : array<int64>
    head |> push(special_id(dec, "<bos>"))
    head |> push(special_id(dec, "<|turn>"))
    head |> push_from(encode_(dec, "user", false, false))
    head |> push_from(nl)
    head |> push(special_id(dec, "<|audio>"))
    var tail : array<int64>
    tail |> push(special_id(dec, "<audio|>"))
    tail |> push_from(encode_(dec, "Transcribe this audio.", false, false))
    tail |> push(special_id(dec, "<turn|>"))
    tail |> push_from(nl)
    tail |> push(special_id(dec, "<|turn>"))
    tail |> push_from(encode_(dec, "model", false, false))
    tail |> push_from(nl)

    let n_head = long_length(head)
    let npos = n_head + n_audio + long_length(tail)
    asr_ctx_guard(n_audio, npos, dec.config.seq_len)
    var embd : array<float>
    embd |> resize(int(npos * dim))
    embed_text_rows(dec, head, embd, 0l)
    for (i in range64(n_audio * dim)) {
        embd[n_head * dim + i] = rows[i]
    }
    embed_text_rows(dec, tail, embd, n_head + n_audio)

    s.qctx.n_past = 0l   // fresh sequence per transcribe
    tp = ref_time_ticks()
    eval_embd_(dec, s.qctx, embd, npos)

    let turn_end = special_id(dec, "<turn|>")
    let eos = special_id(dec, "<eos>")
    let max_new = dec.config.seq_len - npos - 1l
    var one : array<int64>
    one |> resize(1)
    var toks : array<int>
    var raw : array<string>
    toks |> reserve(512)
    raw |> reserve(512)
    for (_i in range64(max_new)) {
        var best = 0
        for (i in range(1, length(s.qctx.logits))) {
            if (s.qctx.logits[i] > s.qctx.logits[best]) {
                best = i
            }
        }
        toks |> push(best)
        if (int64(best) == turn_end || int64(best) == eos) {   // EOG: emitted into the ids, then stop
            break
        }
        raw |> push(piece_(dec, int64(best)))
        one[0] = int64(best)
        eval_(dec, s.qctx, one)
    }
    asr_prof_add("g4a.decode", tp)

    // strip the reasoning channel: keep the text after the last `<channel|>` (the final transcription)
    let full = join(raw, "")
    var text = full
    let mark = rfind(full, "<channel|>")
    if (mark >= 0) {
        text = slice(full, mark + length("<channel|>"), length(full))
    }
    var seg = TranscribeSegment(t0 = 0l, t1 = long_length(samples) * 100l / AUDIO_SAMPLE_RATE,
        text = strip(text))
    seg.tokens := toks
    detected_lang = "en"
    blk |> invoke(seg)
    return 1
}
