options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_parakeet shared public
options _function_length = 0   // model forward/serialize walks mirror the model schema

require dasllama/dasllama_lint public
require math
require daslib/fio
require strings
require daslib/strings_boost
require daslib/array_boost
require daslib/apply
require daslib/archive
require dasllama/dasllama_env
require dasllama/dasllama_math
require dasllama/dasllama_gguf
require dasllama/dasllama_plane public   // PlaneF/PlaneI8/PlaneI32/PlaneI64 — the borrowed weight planes
require dasllama/dasllama_asr_types public   // TranscribeSegment/AsrCaps — the shared ASR floor
require dasllama/dasllama_audio
require dasllama/dasllama_tower
require dasllama/dasllama_convert
require dasllama/dasllama_par
require dasllama/dasllama_common   // apply_box_profile_runtime for the parakeet-image identity
require dasllama/dasllama_image
require dasllama/dasllama_rnnt public   // public: ParakeetToken is RnntToken

// Parakeet-TDT (NVIDIA FastConformer + LSTM predictor + Token-and-Duration Transducer),
// read from the whisper.cpp-converted ggml bin. Oracle = whisper.cpp's in-tree parakeet-cli
// (CPU, -ng); see parakeet_plan.md for the container/graph/decode spec.

let PARAKEET_HOP = 160l
let private LOCAL_ATTN_THRESHOLD = 8192l   // beyond this the oracle switches to local attention

// interleaved (re, im) slots of one 512-pt FFT frame — the mel worker's stack buffer
let private FFT_FRAME = 1024

//! One decoded TDT token: the shared transducer's record.
typedef ParakeetToken = RnntToken

//! One conformer block's blob offsets: GEMM regions index qblob on a q8 load, blob on fp32
//! (identical walks on fp32); the d-vectors, u/v biases and the depthwise taps always index blob.
struct PkLayerOffs {
    nf1_w : int64
    nf1_b : int64
    ff1a_w : int64   // 1024 -> 4096
    ff1b_w : int64   // 4096 -> 1024
    nat_w : int64
    nat_b : int64
    bias_u : int64   // [d_head × n_head]
    bias_v : int64
    q_w : int64
    k_w : int64
    v_w : int64
    o_w : int64
    pos_w : int64
    ncv_w : int64
    ncv_b : int64
    pw1_w : int64    // 1024 -> 2048
    dw_w : int64     // [9 × 1024]
    bn_w : int64
    bn_b : int64
    bn_mean : int64
    bn_var : int64
    pw2_w : int64
    nf2_w : int64
    nf2_b : int64
    ff2a_w : int64
    ff2b_w : int64
    nout_w : int64
    nout_b : int64
}

//! A loaded Parakeet-TDT model: hparams, mel filterbank + STFT window, TDT durations, SPM vocab,
//! and weights. An fp32 load keeps one blob; the default q8 load transcodes GEMM regions into
//! qblob/qscales, leaving blob with only the fp32-side regions.
struct ParakeetModel {
    n_vocab : int64
    n_state : int64
    n_head : int64
    n_layer : int64
    n_mel : int64
    n_fft : int64
    subsampling : int64
    n_sub_ch : int64
    n_conv_kernel : int64
    n_pred_dim : int64
    n_pred_layers : int64
    n_durations : int64
    n_max_tokens : int64
    eps : float = 1e-5
    blank_id : int
    durations : PlaneI32 = PlaneI32()
    filters : PlaneF = PlaneF()     // [n_mel][n_fb]
    window : PlaneF = PlaneF()      // STFT window from the file
    fft : FftPlan = FftPlan()  // radix-2 plan (n_fft is 512 on every parakeet); built at load
    vocab : array<string>
    blob : PlaneF = PlaneF()
    // pre-encode conv stack
    c0_w : int64
    c0_b : int64
    c2_w : int64
    c2_b : int64
    c3_w : int64
    c3_b : int64
    c5_w : int64
    c5_b : int64
    c6_w : int64
    c6_b : int64
    pre_w : int64
    pre_b : int64
    layers_off : int64
    layer_stride : int64
    aux_layers_off : int64     // fp32 walk of the layer span (== layers_off/layer_stride on an
    aux_layer_stride : int64   // fp32 load; on a q8 load the GEMM regions collapse out of it)
    // prediction network + joint
    embed_off : int64          // [n_vocab+1][pred_dim]
    lstm_ih : PlaneI64 = PlaneI64()     // per layer
    lstm_hh : PlaneI64 = PlaneI64()
    lstm_b : PlaneI64 = PlaneI64()
    joint_enc_w : int64
    joint_enc_b : int64
    joint_pred_w : int64
    joint_pred_b : int64
    joint_net_w : int64
    joint_net_b : int64
    // Q8 fast path (default load): Q8_0 image of the GEMM regions at their full-layout
    // offsets, repacked for the active backend; transcoded straight from the mapped file.
    q8 : bool
    qblob : PlaneI8 = PlaneI8()
    qscales : PlaneF = PlaneF()
    image_map : void?         // non-null when the planes borrow a .dlim image (dasllama_image)
    image_bytes : uint64
}

// planes borrow; vocab (meta-carried) and the fft plan (rebuilt at load) are owned
def finalize(var t : ParakeetModel) {
    apply(t) $ [unused_argument(name)] (name : string; var field) {
        static_if (typeinfo is_array(field)) {
            delete field
        } static_elif (typeinfo is_struct(field)) {
            delete field
        }
    }
    if (t.image_map != null) {
        image_backing_release(t.image_map, t.image_bytes)
        t.image_map = null
        t.image_bytes = 0ul
    }
}

// the tripwire: serialize_image_meta(ParakeetModel) covers 40 fields (vocab rides the meta
// blob — string arrays can't be raw planes); fft (rebuilt at load) + image_map + image_bytes
// are deliberate skips.
let private PARAKEET_META_FIELDS = 40 + 3

// every meta-carried ParakeetModel field, explicitly, both directions (see dasllama_image)
def private serialize_image_meta(var arch : Archive; var t : ParakeetModel) {
    verify(count_meta_fields(t) == PARAKEET_META_FIELDS)   // grew ParakeetModel? extend this list
    arch |> serialize_raw(t.n_vocab)
    arch |> serialize_raw(t.n_state)
    arch |> serialize_raw(t.n_head)
    arch |> serialize_raw(t.n_layer)
    arch |> serialize_raw(t.n_mel)
    arch |> serialize_raw(t.n_fft)
    arch |> serialize_raw(t.subsampling)
    arch |> serialize_raw(t.n_sub_ch)
    arch |> serialize_raw(t.n_conv_kernel)
    arch |> serialize_raw(t.n_pred_dim)
    arch |> serialize_raw(t.n_pred_layers)
    arch |> serialize_raw(t.n_durations)
    arch |> serialize_raw(t.n_max_tokens)
    arch |> serialize_raw(t.eps)
    arch |> serialize_raw(t.blank_id)
    serialize_strings(arch, t.vocab)
    arch |> serialize_raw(t.c0_w)
    arch |> serialize_raw(t.c0_b)
    arch |> serialize_raw(t.c2_w)
    arch |> serialize_raw(t.c2_b)
    arch |> serialize_raw(t.c3_w)
    arch |> serialize_raw(t.c3_b)
    arch |> serialize_raw(t.c5_w)
    arch |> serialize_raw(t.c5_b)
    arch |> serialize_raw(t.c6_w)
    arch |> serialize_raw(t.c6_b)
    arch |> serialize_raw(t.pre_w)
    arch |> serialize_raw(t.pre_b)
    arch |> serialize_raw(t.layers_off)
    arch |> serialize_raw(t.layer_stride)
    arch |> serialize_raw(t.aux_layers_off)
    arch |> serialize_raw(t.aux_layer_stride)
    arch |> serialize_raw(t.embed_off)
    arch |> serialize_raw(t.joint_enc_w)
    arch |> serialize_raw(t.joint_enc_b)
    arch |> serialize_raw(t.joint_pred_w)
    arch |> serialize_raw(t.joint_pred_b)
    arch |> serialize_raw(t.joint_net_w)
    arch |> serialize_raw(t.joint_net_b)
    arch |> serialize_raw(t.q8)
}

def private image_post_load(var t : ParakeetModel) {
    t.fft <- build_fft_plan(t.n_fft)   // derived tables — the parse-at-load carve-out
}

[init]
def private register_parakeet_image_tags {
    register_image_family_tag("parakeet-q8")
    register_image_family_tag("parakeet-f32", true)   // config-free: the exact lane's planes are the file's own, no box property shapes them
}

def pk_layer_offsets(m : ParakeetModel; l : int64) : PkLayerOffs {
    let d = m.n_state
    let ff = 4l * d
    let dh = d / m.n_head
    // dual walk: `o` is the full GEMM layout (qblob on a q8 load); `a` is the fp32 walk of m.blob (identical on an fp32 load)
    let g = m.q8 ? 0l : 1l
    var o = m.layers_off + l * m.layer_stride
    var a = m.aux_layers_off + l * m.aux_layer_stride
    var r = PkLayerOffs()
    r.nf1_w = a; a += d; o += d
    r.nf1_b = a; a += d; o += d
    r.ff1a_w = o; o += ff * d; a += g * ff * d
    r.ff1b_w = o; o += d * ff; a += g * d * ff
    r.nat_w = a; a += d; o += d
    r.nat_b = a; a += d; o += d
    r.bias_u = a; a += dh * m.n_head; o += dh * m.n_head
    r.bias_v = a; a += dh * m.n_head; o += dh * m.n_head
    r.q_w = o; o += d * d; a += g * d * d
    r.k_w = o; o += d * d; a += g * d * d
    r.v_w = o; o += d * d; a += g * d * d
    r.o_w = o; o += d * d; a += g * d * d
    r.pos_w = o; o += d * d; a += g * d * d
    r.ncv_w = a; a += d; o += d
    r.ncv_b = a; a += d; o += d
    r.pw1_w = o; o += 2l * d * d; a += g * 2l * d * d
    r.dw_w = a; a += m.n_conv_kernel * d; o += m.n_conv_kernel * d
    r.bn_w = a; a += d; o += d
    r.bn_b = a; a += d; o += d
    r.bn_mean = a; a += d; o += d
    r.bn_var = a; a += d; o += d
    r.pw2_w = o; o += d * d; a += g * d * d
    r.nf2_w = a; a += d; o += d
    r.nf2_b = a; a += d; o += d
    r.ff2a_w = o; o += ff * d; a += g * ff * d
    r.ff2b_w = o; o += d * ff; a += g * d * ff
    r.nout_w = a; a += d; o += d
    r.nout_b = a; a += d; o += d
    return r
}

// per-layer blob span: 8 d×d mats (q/k/v/o/pos + pw1(2) + pw2) + 4 ff×d mats + 16
// d-vectors (norms ×8, u/v, bn ×4) + the depthwise kernel
def private pk_layer_stride(d, ff, nk : int64) : int64 => 8l * d * d + 4l * ff * d + 16l * d + nk * d

// fp32 side of a q8 load: the 11 GEMM mats live in qblob only, leaving the 16 d-vectors
// and the depthwise kernel
def private pk_aux_layer_stride(d, nk : int64) : int64 => 16l * d + nk * d

// repack one GEMM region for the active backend (scales index at woff/32, so woff must be 32-aligned)
def private pk_repack(var st : ParakeetStaging; woff, n, d : int64) {
    if (woff % 32l != 0l) {
        panic("parakeet q8: GEMM weight at {woff} is not 32-aligned")
    }
    repack_q8q8_weight(st.qblob, st.qscales, woff, n, d)
}

// repack every GEMM region for the active backend's q8q8 kernels; without a selected
// backend matmul_q8q8_batch reads the flat Q8_0 image as-is
def private pk_repack_all(var st : ParakeetStaging) {
    if (!select_matmul_backend_for_load_()) {
        return
    }
    let d = st.e.n_state
    let ff = 4l * d
    let nch = st.e.n_sub_ch
    let w3 = conv_s2_out(conv_s2_out(conv_s2_out(st.e.n_mel)))
    let feat = w3 * nch                       // pre_w input dim (encode's xf row width)
    pk_repack(st, st.e.c3_w, nch, nch)
    pk_repack(st, st.e.c6_w, nch, nch)
    pk_repack(st, st.e.pre_w, feat, d)
    for (l in range64(st.e.n_layer)) {
        let lo = pk_layer_offsets(st.e, l)
        pk_repack(st, lo.ff1a_w, d, ff)
        pk_repack(st, lo.ff1b_w, ff, d)
        pk_repack(st, lo.q_w, d, d)
        pk_repack(st, lo.k_w, d, d)
        pk_repack(st, lo.v_w, d, d)
        pk_repack(st, lo.o_w, d, d)
        pk_repack(st, lo.pos_w, d, d)
        pk_repack(st, lo.pw1_w, d, 2l * d)
        pk_repack(st, lo.pw2_w, d, d)
        pk_repack(st, lo.ff2a_w, d, ff)
        pk_repack(st, lo.ff2b_w, ff, d)
    }
    pk_repack(st, st.e.joint_enc_w, d, st.e.n_pred_dim)
}

// ===== per-session scratch =====

//! Every hot-path buffer of the parakeet pipeline (mel, conv subsampling, encoder, requant,
//! TDT decode) so a transcribe allocates NOTHING per call. Arrays grow reserve-exact and never
//! shrink; a default-constructed value is ready to use.
struct ParakeetState {
    enc_gen : uint64   //!< the device rows' generation the last encode left (`tower_rows_gen`), 0 for none
    // mel
    mel : WhisperMel = WhisperMel()
    pre : array<float>
    padded : array<float>
    // conv subsampling
    y0 : array<float>
    y2 : array<float>
    y3 : array<float>
    y5 : array<float>
    y6 : array<float>
    xf : array<float>
    // encoder residual (holds the final encoder rows after the last layer) + scratch
    x : array<float>
    xb : array<float>
    h : array<float>
    qq : array<float>
    kk : array<float>
    vv : array<float>
    pos_emb : array<float>
    pos_tt : int64              //!< the frame count `pos_emb` holds the table of (0 = none), at width `pos_d`, requantized when `pos_q8`
    pos_d : int64
    pos_q8 : bool
    posq : array<int8>
    poss : array<float>
    posl : array<float>
    qu : array<float>
    qv : array<float>
    vh : array<float>
    kt : array<float>
    sc : array<float>
    rl : array<float>
    posh : array<float>
    oh : array<float>
    ao : array<float>
    // q8 activation image (q/k/v share one requant; decode reuses it)
    xqi : array<int8>
    xsi : array<float>
    xbsi : array<int>   //!< the Q8_K image's per-16 sums (a kq-lane tower's; the FC chain passes it, parakeet never fills it)
    // decode
    encp : array<float>
    @scratch toks : array<ParakeetToken>
    dec : RnntState = RnntState()
}

// conv taps arrive [ch][3][3] (dw, torch) / [cout][9] (c0, im2col order) / [ch][k]
// (conformer depthwise); the f4 kernels want tap-major [ntap][ch]
def private pk_conv_tap_major(var blob : array<float>; off, ch, ntap : int64) {
    var tmp : array<float>
    tmp |> resize(ch * ntap)
    for (c in range64(ch)) {
        for (tap in range64(ntap)) {
            tmp[tap * ch + c] = blob[off + c * ntap + tap]
        }
    }
    for (i, v in count(), tmp) {
        blob[off + int64(i)] = v
    }
    delete tmp
}

// inference BN folded to per-channel scale/shift at load: y = acc·scale + shift with
// scale = γ/sqrt(σ²), shift = β − μ·scale. bn_w/bn_b slots take scale/shift; the
// mean/var slots stay as read (dead after the fold).
def private pk_fold_bn(var blob : array<float>; w, b, mean, vr, d : int64) {
    for (j in range64(d)) {
        let s = blob[w + j] / sqrt(blob[vr + j])
        blob[w + j] = s
        blob[b + j] -= blob[mean + j] * s
    }
}

// ===== bin container =====

// whisper-bin-style tensor records: i32 n_dims, i32 name_len, i32 ftype; i32 dims (ne
// order); name bytes; raw data (f32, f16 or Q8_0 blocks), unaligned.
struct private PkTensorRec {
    ttype : int
    n_elem : int64
    off : int64
}

def private pk_bytes_str(b : array<uint8> | #; o, n : int64) : string {
    if (n <= 0l) {
        return ""
    }
    var s = ""
    unsafe {
        s = string(temp_array(addr<uint8?>(b[o]), n, type<uint8>))
    }
    return s
}

def private pk_read(recs : table<string; PkTensorRec>; b : array<uint8> | #; name : string;
                    var dst : array<float>; dst_off, n : int64) {
    if (!key_exists(recs, name)) {
        panic("dasLLAMA parakeet: missing tensor '{name}' in model file")
    }
    let r = recs?[name] ?? default<PkTensorRec>
    if (r.n_elem != n) {
        panic("dasLLAMA parakeet: tensor '{name}' has {r.n_elem} elements, want {n}")
    }
    ggml_bin_read_f32(b, r.ttype, r.off, n, dst, dst_off)
}

// GEMM weight read: a q8 load transcodes it from the file straight into qblob/qscales at its
// full-layout offset (the fp32 blob never holds it); an fp32 load reads it into the blob.
def private pk_read_gemm(recs : table<string; PkTensorRec>; b : array<uint8> | #; name : string;
                         var st : ParakeetStaging; var scratch : array<float>; woff, n : int64) {
    if (!st.e.q8) {
        pk_read(recs, b, name, st.blob, woff, n)
        return
    }
    if (woff % 32l != 0l) {
        panic("parakeet q8: GEMM weight at {woff} is not 32-aligned")
    }
    if (long_length(scratch) < n) {
        scratch |> reserve(n)
        scratch |> resize(n)
    }
    pk_read(recs, b, name, scratch, 0l, n)
    quantize_q8_0_into(scratch, n, st.qblob, st.qscales, woff, woff / 32l)
}

//! Loads a converted Parakeet-TDT ggml bin (convert-parakeet-to-ggml.py output; f32 or f16).
//! With ``q8`` the encoder/joint GEMM weights transcode to Q8_0 straight from the mapped file (~4x CPU fast path); conv im2col, depthwise, LSTM, and joint_net stay fp32 (a few % of encode).
//! Like ``load_model``, caches a box/knob-specific PREPARED IMAGE for fast reload; ``DASLLAMA_IMAGE=0`` disables it, a ``.dlim`` path loads one directly.
def load_parakeet_model(path : string; q8 : bool = false) : ParakeetModel {
    announce_lane("parakeet", q8, "f32 planes", "set_parakeet_q8", g_pk_pin != GemmLane.unset, parakeet_gpu_serves())
    let tag = q8 ? "parakeet-q8" : "parakeet-f32"
    return <- load_staged_image(path, tag, "parakeet", "source model", type<ParakeetModel>) {
        return <- stage_parakeet_model(path, q8)
    }
}

//! The mint-side carrier: ``e`` holds every scalar the meta blob carries (vocab included), the
//! arrays beside it hold the plane bytes until the writer has them. Public because reading the
//! source bin off-rail is the only way a suite can check a served model against a non-image form.
struct ParakeetStaging {
    e : ParakeetModel = ParakeetModel()
    durations : array<int>
    filters : array<float>
    window : array<float>
    blob : array<float>
    lstm_ih : array<int64>
    lstm_hh : array<int64>
    lstm_b : array<int64>
    qblob : array<int8>
    qscales : array<float>
}

def private serialize_image_meta(var arch : Archive; var st : ParakeetStaging) {
    serialize_image_meta(arch, st.e)
}

def finalize(var st : ParakeetStaging) {
    delete_staging(st)
}

[unused_argument(st)]
def private image_post_load(var st : ParakeetStaging) {}

//! Mint a served model from a staged read WITHOUT touching disk — the in-memory image rail.
//! The off-rail control for suites: a live model that came from no `.dlim` file.
def mint_parakeet_model(var st : ParakeetStaging; var out : ParakeetModel; q8 : bool = false) {
    cache_via_image_staged(st, out, "", q8 ? "parakeet-q8" : "parakeet-f32")
}

//! The ggml-bin reader proper: scalars land in the served model, plane bytes in the staging
//! arrays beside it. Nothing here is servable — the image build is what produces a live model.
def stage_parakeet_model(path : string; q8 : bool = false) : ParakeetStaging {
    var st = ParakeetStaging()
    st.e.q8 = q8
    let n_bytes = int64(stat(path).size)
    let f = fopen(path, "rb")
    if (f == null) {
        panic("dasLLAMA parakeet: cannot open model '{path}'")
    }
    fmap(f) $(var bytes : array<uint8>#) {
        var o = 0l
        if (rd_u32(bytes, o) != 0x67676d6cu) {
            panic("dasLLAMA parakeet: bad magic in '{path}'")
        }
        o += 4l
        st.e.n_vocab = int64(rd_i32(bytes, o)); o += 4l
        o += 4l   // n_audio_ctx (unused: full-clip encode)
        st.e.n_state = int64(rd_i32(bytes, o)); o += 4l
        st.e.n_head = int64(rd_i32(bytes, o)); o += 4l
        st.e.n_layer = int64(rd_i32(bytes, o)); o += 4l
        st.e.n_mel = int64(rd_i32(bytes, o)); o += 4l
        o += 4l   // ftype (per-record types below)
        st.e.n_fft = int64(rd_i32(bytes, o)); o += 4l
        st.e.subsampling = int64(rd_i32(bytes, o)); o += 4l
        st.e.n_sub_ch = int64(rd_i32(bytes, o)); o += 4l
        st.e.n_conv_kernel = int64(rd_i32(bytes, o)); o += 4l
        st.e.n_pred_dim = int64(rd_i32(bytes, o)); o += 4l
        st.e.n_pred_layers = int64(rd_i32(bytes, o)); o += 4l
        st.e.n_durations = int64(rd_i32(bytes, o)); o += 4l
        st.e.n_max_tokens = int64(rd_i32(bytes, o)); o += 4l
        st.e.blank_id = int(st.e.n_vocab)
        if (st.e.n_fft != 512l) {   // FFT_FRAME in the mel worker is sized for 512-pt frames
            panic("dasLLAMA parakeet: n_fft {st.e.n_fft} - the mel path is built for the family's 512-pt STFT")
        }
        st.e.fft <- build_fft_plan(st.e.n_fft)

        // mel filterbank
        let fb_mel = int64(rd_i32(bytes, o)); o += 4l
        let fb_n = int64(rd_i32(bytes, o)); o += 4l
        if (fb_mel != st.e.n_mel || fb_n != st.e.n_fft / 2l + 1l) {
            panic("dasLLAMA parakeet: filterbank is {fb_mel}x{fb_n}, want {st.e.n_mel}x{st.e.n_fft / 2l + 1l}")
        }
        st.filters |> resize(fb_mel * fb_n)
        for (i in range64(fb_mel * fb_n)) {
            st.filters[i] = rd_f32(bytes, o + i * 4l)
        }
        o += fb_mel * fb_n * 4l

        // STFT window
        let n_window = int64(rd_i32(bytes, o)); o += 4l
        st.window |> resize(n_window)
        for (i in range64(n_window)) {
            st.window[i] = rd_f32(bytes, o + i * 4l)
        }
        o += n_window * 4l

        // TDT durations
        st.durations |> resize(st.e.n_durations)
        for (i in range64(st.e.n_durations)) {
            st.durations[i] = int(rd_u32(bytes, o + i * 4l))
        }
        o += st.e.n_durations * 4l

        // vocab (SPM pieces; blank = id n_vocab lives outside the table)
        let nv = int64(rd_i32(bytes, o)); o += 4l
        st.e.vocab |> resize(nv)
        for (i in range64(nv)) {
            let len = int64(rd_u32(bytes, o)); o += 4l
            st.e.vocab[i] = pk_bytes_str(bytes, o, len)
            o += len
        }

        // tensor records
        var recs : table<string; PkTensorRec>
        while (o + 12l <= n_bytes) {
            let n_dims = int64(rd_i32(bytes, o)); o += 4l
            let name_len = int64(rd_i32(bytes, o)); o += 4l
            let ttype = rd_i32(bytes, o); o += 4l
            var n_elem = 1l
            for (_d in range64(n_dims)) {
                n_elem *= int64(rd_i32(bytes, o)); o += 4l
            }
            let name = pk_bytes_str(bytes, o, name_len)
            o += name_len
            recs[name] = PkTensorRec(ttype = ttype, n_elem = n_elem, off = o)
            o += ggml_bin_tensor_bytes(ttype, n_elem)
        }

        // blob layout — dual walk: `bo` is the full layout (qblob on a q8 load, blob on an
        // fp32 load); `ao` is the fp32 walk of blob, which the GEMM regions collapse out of
        // on a q8 load (g = 0). On an fp32 load the walks are identical (ao ends == bo).
        let d = st.e.n_state
        let ff = 4l * d
        let dh = d / st.e.n_head
        let nch = st.e.n_sub_ch
        let g = q8 ? 0l : 1l
        var bo = 0l
        var ao = 0l
        st.e.c0_w = ao; ao += nch * 9l; bo += nch * 9l
        st.e.c0_b = ao; ao += nch; bo += nch
        st.e.c2_w = ao; ao += nch * 9l; bo += nch * 9l          // depthwise: per-channel 3x3
        st.e.c2_b = ao; ao += nch; bo += nch
        st.e.c3_w = bo; bo += nch * nch; ao += g * nch * nch
        st.e.c3_b = ao; ao += nch; bo += nch
        st.e.c5_w = ao; ao += nch * 9l; bo += nch * 9l
        st.e.c5_b = ao; ao += nch; bo += nch
        st.e.c6_w = bo; bo += nch * nch; ao += g * nch * nch
        st.e.c6_b = ao; ao += nch; bo += nch
        let feat = (st.e.n_mel / st.e.subsampling) * nch
        st.e.pre_w = bo; bo += d * feat; ao += g * d * feat
        st.e.pre_b = ao; ao += d; bo += d
        st.e.layers_off = bo
        st.e.layer_stride = pk_layer_stride(d, ff, st.e.n_conv_kernel)
        st.e.aux_layers_off = ao
        st.e.aux_layer_stride = q8 ? pk_aux_layer_stride(d, st.e.n_conv_kernel) : st.e.layer_stride
        bo += st.e.n_layer * st.e.layer_stride
        ao += st.e.n_layer * st.e.aux_layer_stride
        st.e.embed_off = ao; ao += (st.e.n_vocab + 1l) * st.e.n_pred_dim; bo += (st.e.n_vocab + 1l) * st.e.n_pred_dim
        let pd = st.e.n_pred_dim
        for (_l in range64(st.e.n_pred_layers)) {
            st.lstm_ih |> push(ao); ao += 4l * pd * pd; bo += 4l * pd * pd
            st.lstm_hh |> push(ao); ao += 4l * pd * pd; bo += 4l * pd * pd
            st.lstm_b |> push(ao); ao += 4l * pd; bo += 4l * pd
        }
        st.e.joint_enc_w = bo; bo += pd * d; ao += g * pd * d
        st.e.joint_enc_b = ao; ao += pd; bo += pd
        st.e.joint_pred_w = ao; ao += pd * pd; bo += pd * pd
        st.e.joint_pred_b = ao; ao += pd; bo += pd
        let n_logits = st.e.n_vocab + 1l + st.e.n_durations
        st.e.joint_net_w = ao; ao += n_logits * pd; bo += n_logits * pd
        st.e.joint_net_b = ao; ao += n_logits; bo += n_logits
        // exact allocations: reserve defeats the array allocator's pow2 rounding, which on the
        // 0.6b f32 bin would back 2.4 GiB of blob floats with a 4 GiB allocation
        st.blob |> reserve(ao)
        st.blob |> resize(ao)
        if (q8) {
            st.qblob |> reserve(bo)
            st.qblob |> resize(bo)
            st.qscales |> reserve(bo / 32l)
            st.qscales |> resize(bo / 32l)
        }

        var wscratch : array<float>   // q8 transcode staging, one GEMM tensor at a time
        pk_read(recs, bytes, "encoder.pre_encode.conv.0.weight", st.blob, st.e.c0_w, nch * 9l)
        pk_conv_tap_major(st.blob, st.e.c0_w, nch, 9l)   // c0 kernel reads tap-major too
        pk_read(recs, bytes, "encoder.pre_encode.conv.0.bias", st.blob, st.e.c0_b, nch)
        pk_read(recs, bytes, "encoder.pre_encode.conv.2.weight", st.blob, st.e.c2_w, nch * 9l)
        pk_conv_tap_major(st.blob, st.e.c2_w, nch, 9l)
        pk_read(recs, bytes, "encoder.pre_encode.conv.2.bias", st.blob, st.e.c2_b, nch)
        pk_read_gemm(recs, bytes, "encoder.pre_encode.conv.3.weight", st, wscratch, st.e.c3_w, nch * nch)
        pk_read(recs, bytes, "encoder.pre_encode.conv.3.bias", st.blob, st.e.c3_b, nch)
        pk_read(recs, bytes, "encoder.pre_encode.conv.5.weight", st.blob, st.e.c5_w, nch * 9l)
        pk_conv_tap_major(st.blob, st.e.c5_w, nch, 9l)
        pk_read(recs, bytes, "encoder.pre_encode.conv.5.bias", st.blob, st.e.c5_b, nch)
        pk_read_gemm(recs, bytes, "encoder.pre_encode.conv.6.weight", st, wscratch, st.e.c6_w, nch * nch)
        pk_read(recs, bytes, "encoder.pre_encode.conv.6.bias", st.blob, st.e.c6_b, nch)
        pk_read_gemm(recs, bytes, "encoder.pre_encode.out.weight", st, wscratch, st.e.pre_w, d * feat)
        pk_read(recs, bytes, "encoder.pre_encode.out.bias", st.blob, st.e.pre_b, d)
        for (l in range64(st.e.n_layer)) {
            let lo = pk_layer_offsets(st.e, l)
            pk_read(recs, bytes, "encoder.layers.{l}.norm_feed_forward1.weight", st.blob, lo.nf1_w, d)
            pk_read(recs, bytes, "encoder.layers.{l}.norm_feed_forward1.bias", st.blob, lo.nf1_b, d)
            pk_read_gemm(recs, bytes, "encoder.layers.{l}.feed_forward1.linear1.weight", st, wscratch, lo.ff1a_w, ff * d)
            pk_read_gemm(recs, bytes, "encoder.layers.{l}.feed_forward1.linear2.weight", st, wscratch, lo.ff1b_w, d * ff)
            pk_read(recs, bytes, "encoder.layers.{l}.norm_self_att.weight", st.blob, lo.nat_w, d)
            pk_read(recs, bytes, "encoder.layers.{l}.norm_self_att.bias", st.blob, lo.nat_b, d)
            pk_read(recs, bytes, "encoder.layers.{l}.self_attn.pos_bias_u", st.blob, lo.bias_u, dh * st.e.n_head)
            pk_read(recs, bytes, "encoder.layers.{l}.self_attn.pos_bias_v", st.blob, lo.bias_v, dh * st.e.n_head)
            pk_read_gemm(recs, bytes, "encoder.layers.{l}.self_attn.linear_q.weight", st, wscratch, lo.q_w, d * d)
            pk_read_gemm(recs, bytes, "encoder.layers.{l}.self_attn.linear_k.weight", st, wscratch, lo.k_w, d * d)
            pk_read_gemm(recs, bytes, "encoder.layers.{l}.self_attn.linear_v.weight", st, wscratch, lo.v_w, d * d)
            pk_read_gemm(recs, bytes, "encoder.layers.{l}.self_attn.linear_out.weight", st, wscratch, lo.o_w, d * d)
            pk_read_gemm(recs, bytes, "encoder.layers.{l}.self_attn.linear_pos.weight", st, wscratch, lo.pos_w, d * d)
            pk_read(recs, bytes, "encoder.layers.{l}.norm_conv.weight", st.blob, lo.ncv_w, d)
            pk_read(recs, bytes, "encoder.layers.{l}.norm_conv.bias", st.blob, lo.ncv_b, d)
            pk_read_gemm(recs, bytes, "encoder.layers.{l}.conv.pointwise_conv1.weight", st, wscratch, lo.pw1_w, 2l * d * d)
            pk_read(recs, bytes, "encoder.layers.{l}.conv.depthwise_conv.weight", st.blob, lo.dw_w, st.e.n_conv_kernel * d)
            pk_conv_tap_major(st.blob, lo.dw_w, d, st.e.n_conv_kernel)   // [d][k] -> [k][d] for the f4 row core
            pk_read(recs, bytes, "encoder.layers.{l}.conv.batch_norm.weight", st.blob, lo.bn_w, d)
            pk_read(recs, bytes, "encoder.layers.{l}.conv.batch_norm.bias", st.blob, lo.bn_b, d)
            pk_read(recs, bytes, "encoder.layers.{l}.conv.batch_norm.running_mean", st.blob, lo.bn_mean, d)
            pk_read(recs, bytes, "encoder.layers.{l}.conv.batch_norm.running_var", st.blob, lo.bn_var, d)
            pk_fold_bn(st.blob, lo.bn_w, lo.bn_b, lo.bn_mean, lo.bn_var, d)
            pk_read_gemm(recs, bytes, "encoder.layers.{l}.conv.pointwise_conv2.weight", st, wscratch, lo.pw2_w, d * d)
            pk_read(recs, bytes, "encoder.layers.{l}.norm_feed_forward2.weight", st.blob, lo.nf2_w, d)
            pk_read(recs, bytes, "encoder.layers.{l}.norm_feed_forward2.bias", st.blob, lo.nf2_b, d)
            pk_read_gemm(recs, bytes, "encoder.layers.{l}.feed_forward2.linear1.weight", st, wscratch, lo.ff2a_w, ff * d)
            pk_read_gemm(recs, bytes, "encoder.layers.{l}.feed_forward2.linear2.weight", st, wscratch, lo.ff2b_w, d * ff)
            pk_read(recs, bytes, "encoder.layers.{l}.norm_out.weight", st.blob, lo.nout_w, d)
            pk_read(recs, bytes, "encoder.layers.{l}.norm_out.bias", st.blob, lo.nout_b, d)
        }
        pk_read(recs, bytes, "decoder.prediction.embed.weight", st.blob, st.e.embed_off, (st.e.n_vocab + 1l) * st.e.n_pred_dim)
        for (l in range64(st.e.n_pred_layers)) {
            pk_read(recs, bytes, "decoder.prediction.dec_rnn.lstm.weight_ih_l{l}", st.blob, st.lstm_ih[l], 4l * st.e.n_pred_dim * st.e.n_pred_dim)
            pk_read(recs, bytes, "decoder.prediction.dec_rnn.lstm.weight_hh_l{l}", st.blob, st.lstm_hh[l], 4l * st.e.n_pred_dim * st.e.n_pred_dim)
            pk_read(recs, bytes, "decoder.prediction.dec_rnn.lstm.bias_h_l{l}", st.blob, st.lstm_b[l], 4l * st.e.n_pred_dim)
        }
        pk_read_gemm(recs, bytes, "joint.enc.weight", st, wscratch, st.e.joint_enc_w, st.e.n_pred_dim * d)
        pk_read(recs, bytes, "joint.enc.bias", st.blob, st.e.joint_enc_b, st.e.n_pred_dim)
        pk_read(recs, bytes, "joint.pred.weight", st.blob, st.e.joint_pred_w, st.e.n_pred_dim * st.e.n_pred_dim)
        pk_read(recs, bytes, "joint.pred.bias", st.blob, st.e.joint_pred_b, st.e.n_pred_dim)
        pk_read(recs, bytes, "joint.joint_net.2.weight", st.blob, st.e.joint_net_w, (st.e.n_vocab + 1l + st.e.n_durations) * st.e.n_pred_dim)
        pk_read(recs, bytes, "joint.joint_net.2.bias", st.blob, st.e.joint_net_b, st.e.n_vocab + 1l + st.e.n_durations)
        delete wscratch
    }
    fclose(f)
    if (q8) {
        pk_repack_all(st)
    }
    return <- st
}

// ===== mel (frame-major!) =====

//! The loaded precision (one tag — parakeet is a single carrier): the facade's
//! [[asr_exec_fmt]] arm.
def parakeet_exec_fmt(p : ParakeetModel) : string => p.q8 ? "q8" : "f32"

//! Parakeet's log-mel: preemphasis 0.97, zero center-pad n_fft/2, the file's STFT window, power
//! spectra, mel dot, NATURAL log(sum + 2⁻²⁴), then per-feature mean/std normalization over the
//! valid frames only (Bessel n−1, std + 1e-5), applied to all. FRAME-major ``[n_len][n_mel]``.
[hot_path]
def parakeet_log_mel(samples : array<float> | #; m : ParakeetModel; var st : ParakeetState) {
    let n_in = long_length(samples)
    ensure_length(st.pre, n_in)
    for (i in range(int(n_in))) {
        st.pre[i] = samples[i]
    }
    var pi = n_in - 1l
    while (pi > 0l) {   // backward in place: x[i] -= 0.97·x[i−1]
        st.pre[pi] -= 0.97 * st.pre[pi - 1l]
        pi--
    }
    let pad = m.n_fft / 2l
    let n_padded = n_in + 2l * pad
    ensure_length(st.padded, n_padded)
    for (i in range(int(pad))) {   // zero head pad (scratch is reused, not fresh)
        st.padded[i] = 0.0
    }
    for (i in range(int(n_in))) {
        st.padded[int(pad) + i] = st.pre[i]
    }
    for (i in range64(n_in + pad, n_padded)) {   // zero tail pad
        st.padded[i] = 0.0
    }

    st.mel.n_mel = m.n_mel
    st.mel.n_len = (n_padded - m.n_fft) / PARAKEET_HOP + 1l
    st.mel.n_len_org = st.mel.n_len
    ensure_length(st.mel.data, st.mel.n_len * m.n_mel)

    let n_fft = m.n_fft
    let n_bins = n_fft / 2l + 1l
    let wsz = length(m.window)
    let wpad = (n_fft - wsz) / 2l
    let mel_eps = 5.960464477539063e-08lf
    let n_len = st.mel.n_len
    let n_mel = m.n_mel

    // per-frame windowed 512-pt FFT + mel matvec, threaded over frames (frames are
    // independent — bit-exact at any lane count); scratch is a per-worker stack frame,
    // so a 9-minute clip no longer materializes the whole [frames × n_fft] image
    unsafe {
        let pdd = addr<float const?>(st.padded[0])
        let wnd = plane_at(m.window, 0l)
        let flt = plane_at(m.filters, 0l)
        let rev = addr<int const?>(m.fft.rev[0])
        let tw = addr<float const?>(m.fft.tw[0])
        var md = addr(st.mel.data[0])
        maybe_parallel_for(0, int(n_len), lanes_for_work(n_len * n_fft * 8l, 0)) $(rb, re) {
            var frame : float[FFT_FRAME]   // 512 interleaved (re, im) pairs, stack-resident
            unsafe {
                var fp = addr(frame[0])
                for (i in range64(int64(rb), int64(re))) {
                    let off = i * PARAKEET_HOP
                    for (j in range64(n_fft * 2l)) {
                        fp[j] = 0.0
                    }
                    let n_do = min(wsz, n_padded - off)   // window applied at [wpad, wpad+wsz)
                    for (j in range64(n_do)) {
                        if (off + wpad + j < n_padded) {
                            fp[(wpad + j) * 2l] = wnd[j] * pdd[off + wpad + j]
                        }
                    }
                    fft_pow2_run(n_fft, rev, tw, fp)
                    for (k in range64(n_bins)) {   // power spectrum, packed to the front
                        fp[k] = fp[2l * k] * fp[2l * k] + fp[2l * k + 1l] * fp[2l * k + 1l]
                    }
                    for (jm in range64(n_mel)) {   // tuned dot: 3.5x the 4-acc scalar form (conv_sub_bench --meldot)
                        let sum = double(dot(flt + jm * n_bins, fp, n_bins))
                        md[i * n_mel + jm] = float(log(sum + mel_eps))
                    }
                }
            }
        }
    }

    // per-feature normalization over the valid frames, applied to all
    let valid = n_in / PARAKEET_HOP
    for (j in range64(m.n_mel)) {
        var sum = 0.0lf
        for (i in range64(valid)) {
            sum += double(st.mel.data[i * m.n_mel + j])
        }
        let mean = sum / double(valid)
        var sq = 0.0lf
        for (i in range64(valid)) {
            let dv = double(st.mel.data[i * m.n_mel + j]) - mean
            sq += dv * dv
        }
        let denom = sqrt(sq / (double(valid) - 1.0lf)) + 1e-5lf
        for (i in range64(st.mel.n_len)) {
            st.mel.data[i * m.n_mel + j] = float((double(st.mel.data[i * m.n_mel + j]) - mean) / denom)
        }
    }
}

// ===== conv subsampling =====

// c0: conv2d k3 s2 p1, fused bias+relu. w tap-major [9][cout] (repacked at load, L1-resident);
// bit-exact vs the old im2col+K9-GEMM+bias+relu chain and 6.5x faster even single-lane
// (conv_sub_bench); threads over oy rows (bit-exact at any lane count).
def private pk_conv2d_c1_s2_relu(var out : array<float>; x : array<float>; blob : PlaneF;
                                 w_off, b_off : int64; iw, ih, cout : int64) {
    let ow = conv_s2_out(iw)
    let oh = conv_s2_out(ih)
    ensure_length(out, oh * ow * cout)
    unsafe {
        var op = addr(out[0])
        let xp = addr<float const?>(x[0])
        let wp = plane_at(blob, w_off)
        let bp = plane_at(blob, b_off)
        let zero = float4(0.0)
        maybe_parallel_for(0, int(oh), lanes_for_work(oh * ow * cout * 9l, 0)) $(rb, re) {
            unsafe {
                for (oy in range64(int64(rb), int64(re))) {
                    let iy0 = oy * 2l - 1l
                    for (ox in range64(ow)) {
                        let ix0 = ox * 2l - 1l
                        var xs : float[9]
                        for (kh in range64(3l)) {   // 9 guarded scalar taps, hoisted out of the channel sweep
                            let iy = iy0 + kh
                            for (kw in range64(3l)) {
                                let ix = ix0 + kw
                                xs[kh * 3l + kw] = (iy >= 0l && iy < ih && ix >= 0l && ix < iw ? xp[iy * iw + ix] : 0.0)
                            }
                        }
                        let x0 = float4(xs[0], xs[0], xs[0], xs[0])
                        let x1 = float4(xs[1], xs[1], xs[1], xs[1])
                        let x2 = float4(xs[2], xs[2], xs[2], xs[2])
                        let x3 = float4(xs[3], xs[3], xs[3], xs[3])
                        let x4 = float4(xs[4], xs[4], xs[4], xs[4])
                        let x5 = float4(xs[5], xs[5], xs[5], xs[5])
                        let x6 = float4(xs[6], xs[6], xs[6], xs[6])
                        let x7 = float4(xs[7], xs[7], xs[7], xs[7])
                        let x8 = float4(xs[8], xs[8], xs[8], xs[8])
                        let o = (oy * ow + ox) * cout
                        var c = 0l
                        while (c < cout) {
                            var acc = x0 * (*reinterpret<float4 const?>(wp + c))
                            acc = mad(x1, *reinterpret<float4 const?>(wp + cout + c), acc)
                            acc = mad(x2, *reinterpret<float4 const?>(wp + 2l * cout + c), acc)
                            acc = mad(x3, *reinterpret<float4 const?>(wp + 3l * cout + c), acc)
                            acc = mad(x4, *reinterpret<float4 const?>(wp + 4l * cout + c), acc)
                            acc = mad(x5, *reinterpret<float4 const?>(wp + 5l * cout + c), acc)
                            acc = mad(x6, *reinterpret<float4 const?>(wp + 6l * cout + c), acc)
                            acc = mad(x7, *reinterpret<float4 const?>(wp + 7l * cout + c), acc)
                            acc = mad(x8, *reinterpret<float4 const?>(wp + 8l * cout + c), acc)
                            acc += *reinterpret<float4 const?>(bp + c)
                            *reinterpret<float4?>(op + o + c) = max(acc, zero)
                            c += 4l
                        }
                    }
                }
            }
        }
    }
}

// guarded single position of the depthwise 3x3 (borders where a tap falls off the image)
def private pk_dw_pos(var op : float?; xp, wp, bp : float const?; oy, ox, iw, ih, ch, ow : int64) {
    let o = (oy * ow + ox) * ch
    unsafe {
        for (c in range64(ch)) {
            var acc = 0.0
            for (kh in range64(3l)) {
                let iy = oy * 2l - 1l + kh
                if (iy >= 0l && iy < ih) {
                    for (kw in range64(3l)) {
                        let ix = ox * 2l - 1l + kw
                        if (ix >= 0l && ix < iw) {
                            acc += wp[(kh * 3l + kw) * ch + c] * xp[(iy * iw + ix) * ch + c]
                        }
                    }
                }
            }
            op[o + c] = acc + bp[c]
        }
    }
}

// depthwise conv2d k3 s2 p1, w tap-major [9][ch] (repacked at load): float4 over the
// contiguous channel axis, 9 taps unrolled, guarded borders (isolated bench: 7.2x scalar);
// interior threads over oy rows (independent — bit-exact at any lane count)
def private pk_conv2d_dw_s2(var out : array<float>; x : array<float>; blob : PlaneF;
                            w_off, b_off : int64; iw, ih, ch : int64) {
    let ow = conv_s2_out(iw)
    let oh = conv_s2_out(ih)
    let oy_end = (ih - 2l) / 2l + 1l   // last oy with iy=2*oy+1 in bounds, exclusive
    let ox_end = (iw - 2l) / 2l + 1l
    let rs = iw * ch
    ensure_length(out, oh * ow * ch)
    unsafe {
        var op = addr(out[0])
        let xp = addr<float const?>(x[0])
        let wp = plane_at(blob, w_off)
        let bp = plane_at(blob, b_off)
        maybe_parallel_for(1, int(oy_end), lanes_for_work(oh * ow * ch * 9l, 0)) $(rb, re) {
            unsafe {
                for (oy in range64(int64(rb), int64(re))) {
                    for (ox in range64(1l, ox_end)) {
                        let x0 = ((oy * 2l - 1l) * iw + ox * 2l - 1l) * ch
                        let o = (oy * ow + ox) * ch
                        var c = 0l
                        while (c < ch) {
                            var acc = (*reinterpret<float4 const?>(xp + x0 + c)) * (*reinterpret<float4 const?>(wp + c))
                            acc = mad(*reinterpret<float4 const?>(xp + x0 + ch + c), *reinterpret<float4 const?>(wp + ch + c), acc)
                            acc = mad(*reinterpret<float4 const?>(xp + x0 + 2l * ch + c), *reinterpret<float4 const?>(wp + 2l * ch + c), acc)
                            acc = mad(*reinterpret<float4 const?>(xp + x0 + rs + c), *reinterpret<float4 const?>(wp + 3l * ch + c), acc)
                            acc = mad(*reinterpret<float4 const?>(xp + x0 + rs + ch + c), *reinterpret<float4 const?>(wp + 4l * ch + c), acc)
                            acc = mad(*reinterpret<float4 const?>(xp + x0 + rs + 2l * ch + c), *reinterpret<float4 const?>(wp + 5l * ch + c), acc)
                            acc = mad(*reinterpret<float4 const?>(xp + x0 + 2l * rs + c), *reinterpret<float4 const?>(wp + 6l * ch + c), acc)
                            acc = mad(*reinterpret<float4 const?>(xp + x0 + 2l * rs + ch + c), *reinterpret<float4 const?>(wp + 7l * ch + c), acc)
                            acc = mad(*reinterpret<float4 const?>(xp + x0 + 2l * rs + 2l * ch + c), *reinterpret<float4 const?>(wp + 8l * ch + c), acc)
                            *reinterpret<float4?>(op + o + c) = acc + *reinterpret<float4 const?>(bp + c)
                            c += 4l
                        }
                    }
                }
            }
        }
        for (oy in range64(oh)) {
            let interior_y = oy >= 1l && oy < oy_end
            for (ox in range64(ow)) {
                if (!interior_y || ox == 0l || ox >= ox_end) {
                    pk_dw_pos(op, xp, wp, bp, oy, ox, iw, ih, ch, ow)
                }
            }
        }
    }
}

// feature permute xf[t][fr + w3*c] = y6[t][fr][c]: float4 over the contiguous c axis,
// 4 strided scalar stores per group (bench: 2.85x the scalar 3-loop); threads over t
def private pk_xf_permute(var xf : array<float>; y6 : array<float>; tt, w3, nch : int64) {
    let feat = w3 * nch
    unsafe {
        var fp = addr(xf[0])
        let yp = addr<float const?>(y6[0])
        maybe_parallel_for(0, int(tt), lanes_for_work(tt * feat, 0)) $(rb, re) {
            unsafe {
                for (t in range64(int64(rb), int64(re))) {
                    let base = t * feat
                    for (fr in range64(w3)) {
                        let yr = base + fr * nch
                        let fo = base + fr
                        var c = 0l
                        while (c < nch) {
                            let v = *reinterpret<float4 const?>(yp + yr + c)
                            fp[fo + w3 * c] = v.x
                            fp[fo + w3 * (c + 1l)] = v.y
                            fp[fo + w3 * (c + 2l)] = v.z
                            fp[fo + w3 * (c + 3l)] = v.w
                            c += 4l
                        }
                    }
                }
            }
        }
    }
}

// ===== encoder =====

//! The most encoder frames an encode takes: past it the oracle switches to local attention, which no chain here runs.
def parakeet_max_frames() : int64 => LOCAL_ATTN_THRESHOLD

//! The rel-pos embedding table of `tt` frames into `st.pos_emb`: [2 tt - 1 x d], interleaved (sin, cos) pairs,
//! and on the q8 lane its requant - one serves every layer's pos projection. A session keeps the table of the
//! frame count it last built.
def parakeet_pos_table(m : ParakeetModel; var st : ParakeetState; tt : int64) {
    let d = m.n_state
    return if (st.pos_tt == tt && st.pos_d == d && (st.pos_q8 || !m.q8))
    let ww = 2l * tt - 1l
    rel_pos_table(st.pos_emb, d, tt, ww)
    if (m.q8) {
        requant_rows_q8_sized(st.pos_emb, d, ww, st.posq, st.poss)
    }
    st.pos_tt = tt
    st.pos_d = d
    st.pos_q8 = m.q8
}

//! FastConformer encoder over the whole mel: conv subsampling, then 24 conformer blocks - global attention only, panics past
//! the oracle's 8192-frame local-attention switch. Reads ``st.mel``, leaves rows in ``st.x`` and the device rows' generation
//! the encode left in ``st.enc_gen`` (`tower_rows_gen`), the transducer decode's key.
[hot_path]
def parakeet_encode(m : ParakeetModel; var st : ParakeetState) : int64 {
    let tt = pk_encode_rows(m, st)
    st.enc_gen = tower_rows_gen()
    return tt
}

def private pk_encode_rows(m : ParakeetModel; var st : ParakeetState) : int64 {
    let d = m.n_state
    let nch = m.n_sub_ch
    // the conformer block loop walks the subsampled frame count
    dispatch_phase_encode((st.mel.n_len + m.subsampling - 1l) / m.subsampling)

    if (g_pk_gpu_encode_set) {
        let served = invoke(g_pk_gpu_encode, m, st)
        return served if (served >= 0l)
    }

    // conv subsampling ([time][mel] rows are already the [ih][iw][c=1] layout)
    var tp = ref_time_ticks()
    let t1 = conv_s2_out(st.mel.n_len)
    let w1 = conv_s2_out(m.n_mel)
    pk_conv2d_c1_s2_relu(st.y0, st.mel.data, m.blob, m.c0_w, m.c0_b, m.n_mel, st.mel.n_len, nch)
    pk_conv2d_dw_s2(st.y2, st.y0, m.blob, m.c2_w, m.c2_b, w1, t1, nch)
    let w2 = conv_s2_out(w1)
    let t2 = conv_s2_out(t1)
    ensure_length(st.y3, t2 * w2 * nch)
    pk_scan("y0", st.y0, t1 * w1 * nch)
    pk_scan("y2", st.y2, t2 * w2 * nch)
    tower_mm_rq(m, st.y3, m.c3_w, st.y2, st.xqi, st.xsi, nch, nch, t2 * w2)   // 1x1 conv = per-position channel mix
    bias_relu_rows(st.y3, m.blob, m.c3_b, nch, t2 * w2)
    pk_conv2d_dw_s2(st.y5, st.y3, m.blob, m.c5_w, m.c5_b, w2, t2, nch)
    let w3 = conv_s2_out(w2)
    let t3 = conv_s2_out(t2)
    ensure_length(st.y6, t3 * w3 * nch)
    tower_mm_rq(m, st.y6, m.c6_w, st.y5, st.xqi, st.xsi, nch, nch, t3 * w3)
    bias_relu_rows(st.y6, m.blob, m.c6_b, nch, t3 * w3)

    // features per time row: v[f + w3·c] = y6[t][f][c]; then pre_out into the residual stream
    let tt = t3
    if (tt > LOCAL_ATTN_THRESHOLD) {
        panic("dasLLAMA parakeet: {tt} encoder frames crosses the local-attention switch - clip audio to under ~10.9 minutes")
    }
    let feat = w3 * nch
    ensure_length(st.xf, tt * feat)
    pk_xf_permute(st.xf, st.y6, tt, w3, nch)
    ensure_length(st.x, tt * d)
    pk_scan("y6", st.y6, t3 * w3 * nch)
    tower_mm_rq(m, st.x, m.pre_w, st.xf, st.xqi, st.xsi, feat, d, tt)
    add_bias_rows(st.x, m.blob, m.pre_b, d, tt)
    pk_scan("pre", st.x, tt * d)
    asr_prof_add("pk.conv_subsample", tp)
    tp = ref_time_ticks()

    // the oracle masks keys >= ceil(n_len_org/8): no masked tail on a full-clip encode, where ceil-per-stage == ceil(n/8)
    let n_real = (st.mel.n_len_org + m.subsampling - 1l) / m.subsampling

    let ww = 2l * tt - 1l
    parakeet_pos_table(m, st, tt)
    asr_prof_add("pk.pos_emb", tp)

    var gpu_served = false
    if (g_pk_gpu_set) {
        gpu_served = invoke(g_pk_gpu, m, st, tt, ww)
    }
    if (!gpu_served) {
        pk_blocks_cpu(m, st, tt, ww, n_real)
    }
    return tt
}

//! the GPU block hook: after the subsample front + pre-projection + pos table, the 24 conformer blocks on st.x
typedef ParakeetGpuBlocksFn = function<(m : ParakeetModel; var st : ParakeetState; tt, ww : int64) : bool>
typedef ParakeetServesFn = function<() : bool>
var private g_pk_gpu : ParakeetGpuBlocksFn
var private g_pk_serves : ParakeetServesFn
var private g_pk_gpu_set = false
var private g_pk_serves_set = false

[arch(at="../ARCHITECTURE_MEDIA.md#tower-gpu-hook")]
def register_parakeet_gpu(fn : ParakeetGpuBlocksFn; serves : ParakeetServesFn) {
    g_pk_gpu = fn
    g_pk_serves = serves
    g_pk_gpu_set = true
    g_pk_serves_set = true
}

//! the GPU encode hook: the whole encode off `st.mel` - the subsample front, the pre-projection and the blocks - into
//! `st.x`; answers the frame count, or -1 to leave the CPU front and the block hook to serve
typedef ParakeetGpuEncodeFn = function<(m : ParakeetModel; var st : ParakeetState) : int64>
var private g_pk_gpu_encode : ParakeetGpuEncodeFn
var private g_pk_gpu_encode_set = false

[arch(at="../ARCHITECTURE_MEDIA.md#tower-gpu-hook")]
def register_parakeet_gpu_encode(fn : ParakeetGpuEncodeFn) {
    g_pk_gpu_encode = fn
    g_pk_gpu_encode_set = true
}

//! Would the Metal driver serve the block loop — the lane policy asks (a serving driver wants
//! the f32 blob, so the un-pinned lane defaults to exact); false on CPU builds or knob-off.
def parakeet_gpu_serves() : bool => g_pk_serves_set && invoke(g_pk_serves)

//! Would a GPU driver serve the loaded model's encode: the exact lane, with a driver whose knob is on - the facade's device-lane arm.
def parakeet_device_gpu(m : ParakeetModel) : bool => !m.q8 && parakeet_gpu_serves()

var private g_pk_pin = GemmLane.unset

//! Pin the parakeet weight format for subsequent loads: q8 (the CPU serving lane) vs the fp32
//! lane the parity legs and the GPU driver read. ``reset_parakeet_q8`` returns to the policy
//! default (exact when the Metal driver serves).
def set_parakeet_q8(on : bool) {
    g_pk_pin = lane_pin(on)
}

def reset_parakeet_q8() {
    g_pk_pin = GemmLane.unset
}

[arch(at="../ARCHITECTURE_MEDIA.md#tower-weight-lane")]
def parakeet_serves_q8() : bool => lane_serves_q8(g_pk_pin, !parakeet_gpu_serves())

//! a parakeet block's offsets as the FastConformer chains read them: canary's record with no GEMM bias (-1 = none)
def pk_fc_layer(lo : PkLayerOffs) : CanaryLayerOffs {
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

//! parakeet's CPU block form: no GEMM bias, tap-major taps, the folded BatchNorm, its stage buckets and the NaN scan
def private pk_cpu_form(m : ParakeetModel) : FcCpuForm {
    return FcCpuForm(d = m.n_state, ff = 4l * m.n_state, n_head = m.n_head, nk = m.n_conv_kernel, biased = false,
        prof = FcCpuProf(ffn1 = FcFfnProf(ln = "pk.ffn.ln", a = "pk.ffn.a", silu = "pk.ffn.silu", b = "pk.ffn.b", res = "pk.ffn.res"),
            ffn = "pk.ffn", proj_ln = "pk.attn_proj.ln", proj_rq = "pk.attn_proj.rq", proj_q = "pk.attn_proj.q",
            proj_k = "pk.attn_proj.k", proj_v = "pk.attn_proj.v", proj_pos = "pk.attn_proj.pos", proj = "pk.attn_proj",
            heads_pack = "pk.attn_heads.pack", heads_pos = "pk.attn_heads.pos", heads_blocks = "pk.attn_heads.blocks",
            heads = "pk.attn_heads", attn_out = "pk.attn_out", conv = "pk.conv_module"),
        scan_on = g_pk_nan_scan, scan = @@pk_scan_layer)
}

def private pk_blocks_cpu(m : ParakeetModel; var st : ParakeetState; tt, ww, n_real : int64) {
    let form = pk_cpu_form(m)
    for (l in range64(m.n_layer)) {
        fc_block_cpu(m, st, form, FcRelPos(ww = ww, n_real = n_real), pk_fc_layer(pk_layer_offsets(m, l)), l, tt)
    }
}

// ===== prediction network + joint + TDT greedy =====

//! parakeet rides RnntNet's defaults: the LSTM gates packed i, f, o, g, one bias, the first step on the blank's embedding row.
def private pk_rnnt_net(m : ParakeetModel) : RnntNet {
    var net = RnntNet(blob = m.blob, n_layers = m.n_pred_layers, pred_dim = m.n_pred_dim, joint_dim = m.n_pred_dim,
        n_tokens = m.n_vocab + 1l, blank_id = m.blank_id, durations = m.durations, max_symbols = int(m.n_max_tokens),
        embed = m.embed_off, joint_pred_w = m.joint_pred_w, joint_pred_b = m.joint_pred_b,
        joint_net_w = m.joint_net_w, joint_net_b = m.joint_net_b,
        prof_step = "pk.dec.step", prof_pick = "pk.dec.pick", prof_joint = "pk.dec.joint")
    for (l in range64(m.n_pred_layers)) {
        net.lstm_ih[l] = m.lstm_ih[l]
        net.lstm_hh[l] = m.lstm_hh[l]
        net.lstm_b[l] = m.lstm_b[l]
    }
    return net
}

// NaN/Inf stage scan (debug rail, pairs with the fast_math x64 investigation): bit-pattern
// detection — a float `v != v` check folds to false under fast_math's nnan, this survives.
var g_pk_nan_scan = false

def set_pk_nan_scan(on : bool) {
    g_pk_nan_scan = on
}

[cold_path]
def private pk_scan_layer(tag : string; l : int64; x : array<float>; n : int64) {
    // layered spelling: the layer rides separately so the hot loop builds no string when off
    if (!g_pk_nan_scan) {
        return
    }
    pk_scan("L{l}.{tag}", x, n)
}

[cold_path]
def private pk_scan(tag : string; x : array<float>; n : int64) {
    if (!g_pk_nan_scan) {
        return
    }
    var nan = 0
    var inf = 0
    var first = -1l
    for (i in range64(n)) {
        let bits = unsafe(reinterpret<uint>(x[i]))
        if ((bits & 0x7F800000u) == 0x7F800000u) {
            if ((bits & 0x007FFFFFu) != 0u) {
                nan++
            } else {
                inf++
            }
            if (first < 0l) {
                first = i
            }
        }
    }
    if (nan + inf > 0) {
        print("pk_scan {tag}: nan={nan} inf={inf} first={first} of {n}\n")
    } else {
        print("pk_scan {tag}: clean ({n})\n")
    }
}

//! Greedy TDT decode over the encoder rows in ``st.x``: the joint's encoder projection of every frame,
//! then the shared transducer loop (`rnnt_greedy`). Tokens land in ``st.toks``.
[hot_path]
def parakeet_tdt_decode(m : ParakeetModel; var st : ParakeetState; n_frames : int64) {
    let d = m.n_state
    let pd = m.n_pred_dim
    let enc = RnntEnc(d = d, w = m.joint_enc_w, b = m.joint_enc_b, q8 = m.q8, gen = st.enc_gen)
    rnnt_decode(pk_rnnt_net(m), st.dec, enc, st.x, st.encp, n_frames, st.toks) $(var encp) {
        let ts_encp = ref_time_ticks()
        ensure_length(encp, n_frames * pd)
        tower_mm_rq(m, encp, m.joint_enc_w, st.x, st.xqi, st.xsi, d, pd, n_frames)
        add_bias_rows(encp, m.blob, m.joint_enc_b, pd, n_frames)
        asr_prof_add("pk.dec.encp", ts_encp)
    }
}

//! The transcript of `toks`: the shared SentencePiece rule over the model's vocab.
def parakeet_detokenize(m : ParakeetModel; toks : array<ParakeetToken> | #) : string => asr_spm_detokenize(m.vocab, toks)

// v3 (25-lang) has an 8192-piece SPM vocab; v2 (English) 1024 — the multilingual threshold
def pk_is_multilingual(m : ParakeetModel) : bool => m.n_vocab >= 8192l

//! Parakeet's capability sheet: v2 English-only, v3 the 25 EU languages; segment timestamps.
def parakeet_asr_caps(m : ParakeetModel) : AsrCaps {
    var c = AsrCaps(translate = false, timestamps = AsrTimestamps.segment,
                    streaming = false, prompt = false, default_lang = "en")
    if (pk_is_multilingual(m)) {
        c.languages <- ["bg", "hr", "cs", "da", "nl", "en", "et", "fi", "fr", "de",
                        "el", "hu", "it", "lv", "lt", "mt", "pl", "pt", "ro", "sk",
                        "sl", "es", "sv", "ru", "uk"]
    } else {
        c.languages <- ["en"]
    }
    return <- c
}

//! Session-create validation: parakeet has no language conditioning — the TDT decode
//! transcribes whatever it hears; ``lang`` is checked against the model's set, then unused.
def parakeet_check_lang(m : ParakeetModel; lang : string) {
    if (lang != "auto" && !has_value(parakeet_asr_caps(m).languages, lang)) {
        panic(pk_is_multilingual(m) ?
            "dasLLAMA asr: '{lang}' is not one of this Parakeet model's 25 languages" :
            "dasLLAMA asr: this Parakeet model is English-only")
    }
}

//! The family's encode-total ``asr_prof`` bucket: the whole FastConformer encode, on whichever home serves it.
let PARAKEET_ENCODE_BUCKET = "pk.encode_total"

//! The facade's transcribe arm: full-clip FastConformer encode + TDT greedy, one segment.
def parakeet_transcribe(m : ParakeetModel; var st : ParakeetState; samples : array<float> | #;
                        blk : block<(seg : TranscribeSegment)>; var detected_lang : string&) : int {
    var tp = ref_time_ticks()
    parakeet_log_mel(samples, m, st)
    asr_prof_add("pk.mel", tp)
    tp = ref_time_ticks()
    let tt = parakeet_encode(m, st)
    asr_prof_add(PARAKEET_ENCODE_BUCKET, tp)
    tp = ref_time_ticks()
    parakeet_tdt_decode(m, st, tt)
    asr_prof_add("pk.decode_total", tp)
    let seg <- asr_segment(0l, long_length(samples) * 100l / AUDIO_SAMPLE_RATE, parakeet_detokenize(m, st.toks), st.toks)
    if (!pk_is_multilingual(m)) {
        detected_lang = "en"   // v3 transcribes without detecting/reporting a language
    }
    blk |> invoke(seg)
    return 1
}
