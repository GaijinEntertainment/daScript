options gen2
options _comment_hygiene = true

module dasllama_parakeet shared public

require math
require daslib/fio
require strings
require daslib/strings_boost
require daslib/array_boost
require dasllama/dasllama_math
require dasllama/dasllama_gguf
require dasllama/dasllama_audio
require dasllama/dasllama_quant
require dasllama/dasllama_par

// Parakeet-TDT (NVIDIA FastConformer + LSTM predictor + Token-and-Duration Transducer),
// read from whisper.cpp's converted ggml bin. Oracle = whisper.cpp's in-tree parakeet-cli
// (CPU, -ng); the container/graph/decode rules are transcribed from src/parakeet.cpp —
// see parakeet_plan.md for the full spec and line references.

let PARAKEET_HOP = 160l
let private LOCAL_ATTN_THRESHOLD = 8192l   // beyond this the oracle switches to local attention

// interleaved (re, im) slots of one 512-pt FFT frame — the mel worker's stack buffer
let private FFT_FRAME = 1024

//! One decoded TDT token: vocab id, chosen duration (index + frames), the encoder frame it
//! was emitted at, and its (log-)probability under the joint's full-row log-softmax.
struct ParakeetToken {
    id : int
    dur_idx : int
    dur : int
    frame : int64
    p : float
    plog : float
}

struct private PkLayerOffs {
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
    durations : array<int>
    filters : array<float>     // [n_mel][n_fb]
    window : array<float>      // STFT window from the file
    fft : FftPlan = FftPlan()  // radix-2 plan (n_fft is 512 on every parakeet); built at load
    vocab : array<string>
    blob : array<float>
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
    lstm_ih : array<int64>     // per layer
    lstm_hh : array<int64>
    lstm_b : array<int64>
    joint_enc_w : int64
    joint_enc_b : int64
    joint_pred_w : int64
    joint_pred_b : int64
    joint_net_w : int64
    joint_net_b : int64
    // Q8 fast path (default load): Q8_0 image of the GEMM regions at their full-layout
    // offsets, repacked for the active backend; transcoded straight from the mapped file.
    q8 : bool
    qblob : array<int8>
    qscales : array<float>
}

def private pk_layer_offsets(m : ParakeetModel; l : int64) : PkLayerOffs {
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
def private pk_repack(var m : ParakeetModel; woff, n, d : int64) {
    if (woff % 32l != 0l) {
        panic("parakeet q8: GEMM weight at {woff} is not 32-aligned")
    }
    repack_q8q8_weight(m.qblob, m.qscales, woff, n, d)
}

// repack every GEMM region for the active backend's q8q8 kernels; without a selected
// backend matmul_q8q8_batch reads the flat Q8_0 image as-is
def private pk_repack_all(var m : ParakeetModel) {
    if (!select_matmul_backend_for_load()) {
        return
    }
    let d = m.n_state
    let ff = 4l * d
    let nch = m.n_sub_ch
    let w1 = (m.n_mel - 1l) / 2l + 1l
    let w2 = (w1 - 1l) / 2l + 1l
    let w3 = (w2 - 1l) / 2l + 1l
    let feat = w3 * nch                       // pre_w input dim (encode's xf row width)
    pk_repack(m, m.c3_w, nch, nch)
    pk_repack(m, m.c6_w, nch, nch)
    pk_repack(m, m.pre_w, feat, d)
    for (l in range64(m.n_layer)) {
        let lo = pk_layer_offsets(m, l)
        pk_repack(m, lo.ff1a_w, d, ff)
        pk_repack(m, lo.ff1b_w, ff, d)
        pk_repack(m, lo.q_w, d, d)
        pk_repack(m, lo.k_w, d, d)
        pk_repack(m, lo.v_w, d, d)
        pk_repack(m, lo.o_w, d, d)
        pk_repack(m, lo.pos_w, d, d)
        pk_repack(m, lo.pw1_w, d, 2l * d)
        pk_repack(m, lo.pw2_w, d, d)
        pk_repack(m, lo.ff2a_w, d, ff)
        pk_repack(m, lo.ff2b_w, ff, d)
    }
    pk_repack(m, m.joint_enc_w, d, m.n_pred_dim)
}

// ===== per-session scratch =====

//! LSTM predictor state (h/c per layer) + the projected prediction vector.
struct ParakeetPredState {
    h : array<float>       // [n_layers × pred_dim]
    c : array<float>
    pred : array<float>    // joint.pred projection of the last hidden [pred_dim]
    gates : array<float>   // scratch [4 × pred_dim]: ih part
    gates2 : array<float>  // scratch [4 × pred_dim]: hh part
    xin : array<float>     // scratch [pred_dim]: layer input
    hbuf : array<float>    // scratch [pred_dim]: current layer's h (matmul takes plain arrays)
}

//! Every hot-path buffer of the parakeet pipeline (mel, conv subsampling, encoder, requant,
//! TDT decode) so a transcribe allocates NOTHING per call. Arrays grow reserve-exact and never
//! shrink; a default-constructed value is ready to use.
struct ParakeetState {
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
    posq : array<int8>
    poss : array<float>
    posl : array<float>
    quh : array<float>
    quh2 : array<float>
    vh : array<float>
    kth : array<float>
    scores : array<float>
    relm : array<float>
    post : array<float>
    oh : array<float>
    ao : array<float>
    // q8 activation image (q/k/v share one requant; decode reuses it)
    xqi : array<int8>
    xsi : array<float>
    // decode
    encp : array<float>
    joint : array<float>
    logits : array<float>
    toks : array<ParakeetToken>
    pred : ParakeetPredState = ParakeetPredState()
}

// grow-only, reserve-exact (resize alone rounds the backing alloc up to pow2 — 2x waste
// on the multi-MB encoder scratch)
def private pk_ensure(var a : array<float>; n : int64) {
    if (int64(length(a)) < n) {
        a |> reserve(int(n))
        a |> resize(int(n))
    }
}

def private pk_ensure(var a : array<int8>; n : int64) {
    if (int64(length(a)) < n) {
        a |> reserve(int(n))
        a |> resize(int(n))
    }
}

// mm_blob_b with the q8 route: activation image (xq, xs) pre-quantized by the caller
// (asr_requant_rows_q8) so q/k/v and repeat consumers share one requant.
def private pk_mm(m : ParakeetModel; var y : array<float>; woff : int64; x : array<float>;
                  xq : array<int8>; xs : array<float>; n, d, npos : int64) {
    if (m.q8) {
        matmul_q8q8_batch(y, m.qblob, m.qscales, woff, xq, xs, n, d, npos)
    } else {
        mm_blob_b(y, m.blob, woff, x, n, d, npos)
    }
}

// requant-and-multiply for single-consumer activations (scratch grows lazily)
def private pk_mm_rq(m : ParakeetModel; var y : array<float>; woff : int64; x : array<float>;
                     var xq : array<int8>; var xs : array<float>; n, d, npos : int64) {
    if (m.q8) {
        pk_requant(x, n, npos, xq, xs)
    }
    pk_mm(m, y, woff, x, xq, xs, n, d, npos)
}

def private pk_requant(x : array<float>; n, npos : int64; var xq : array<int8>; var xs : array<float>) {
    let need = npos * n
    if (int64(length(xq)) < need) {
        xq |> resize(int(need))
        xs |> resize(int(need / 32l))
    }
    asr_requant_rows_q8(x, n, npos, xq, xs)
}

// conv taps arrive [ch][3][3] (dw, torch) / [cout][9] (c0, im2col order) / [ch][k]
// (conformer depthwise); the f4 kernels want tap-major [ntap][ch]
def private pk_conv_tap_major(var blob : array<float>; off, ch, ntap : int64) {
    var tmp : array<float>
    tmp |> resize(int(ch * ntap))
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
// order); name bytes; raw data (f32/f16), unaligned.
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
    if (r.ttype == GGML_TYPE_F32) {
        for (i in range64(n)) {
            dst[dst_off + i] = rd_f32(b, r.off + i * 4l)
        }
    } else {
        for (i in range64(n)) {
            dst[dst_off + i] = f16_to_f32(rd_u16(b, r.off + i * 2l))
        }
    }
}

// GEMM weight read: a q8 load transcodes it from the file straight into qblob/qscales at its
// full-layout offset (the fp32 blob never holds it); an fp32 load reads it into the blob.
def private pk_read_gemm(recs : table<string; PkTensorRec>; b : array<uint8> | #; name : string;
                         var m : ParakeetModel; var scratch : array<float>; woff, n : int64) {
    if (!m.q8) {
        pk_read(recs, b, name, m.blob, woff, n)
        return
    }
    if (woff % 32l != 0l) {
        panic("parakeet q8: GEMM weight at {woff} is not 32-aligned")
    }
    if (int64(length(scratch)) < n) {
        scratch |> reserve(n)
        scratch |> resize(n)
    }
    pk_read(recs, b, name, scratch, 0l, n)
    quantize_q8_0_into(scratch, n, m.qblob, m.qscales, woff, woff / 32l)
}

//! Loads a converted Parakeet-TDT ggml bin (convert-parakeet-to-ggml.py output; f32 or f16).
//! With ``q8`` the encoder/joint GEMM weights transcode to Q8_0 straight from the mapped file
//! (~4x CPU fast path); conv im2col, depthwise, LSTM, and joint_net stay fp32 (a few % of encode).
def load_parakeet_model(path : string; q8 : bool = false) : ParakeetModel {
    var m = ParakeetModel()
    m.q8 = q8
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
        m.n_vocab = int64(rd_i32(bytes, o)); o += 4l
        o += 4l   // n_audio_ctx (unused: full-clip encode)
        m.n_state = int64(rd_i32(bytes, o)); o += 4l
        m.n_head = int64(rd_i32(bytes, o)); o += 4l
        m.n_layer = int64(rd_i32(bytes, o)); o += 4l
        m.n_mel = int64(rd_i32(bytes, o)); o += 4l
        o += 4l   // ftype (per-record types below)
        m.n_fft = int64(rd_i32(bytes, o)); o += 4l
        m.subsampling = int64(rd_i32(bytes, o)); o += 4l
        m.n_sub_ch = int64(rd_i32(bytes, o)); o += 4l
        m.n_conv_kernel = int64(rd_i32(bytes, o)); o += 4l
        m.n_pred_dim = int64(rd_i32(bytes, o)); o += 4l
        m.n_pred_layers = int64(rd_i32(bytes, o)); o += 4l
        m.n_durations = int64(rd_i32(bytes, o)); o += 4l
        m.n_max_tokens = int64(rd_i32(bytes, o)); o += 4l
        m.blank_id = int(m.n_vocab)
        if (m.n_fft != 512l) {   // FFT_FRAME in the mel worker is sized for 512-pt frames
            panic("dasLLAMA parakeet: n_fft {m.n_fft} — the mel path is built for the family's 512-pt STFT")
        }
        m.fft <- build_fft_plan(m.n_fft)

        // mel filterbank
        let fb_mel = int64(rd_i32(bytes, o)); o += 4l
        let fb_n = int64(rd_i32(bytes, o)); o += 4l
        if (fb_mel != m.n_mel || fb_n != m.n_fft / 2l + 1l) {
            panic("dasLLAMA parakeet: filterbank is {fb_mel}x{fb_n}, want {m.n_mel}x{m.n_fft / 2l + 1l}")
        }
        m.filters |> resize(int(fb_mel * fb_n))
        for (i in range64(fb_mel * fb_n)) {
            m.filters[i] = rd_f32(bytes, o + i * 4l)
        }
        o += fb_mel * fb_n * 4l

        // STFT window
        let n_window = int64(rd_i32(bytes, o)); o += 4l
        m.window |> resize(int(n_window))
        for (i in range64(n_window)) {
            m.window[i] = rd_f32(bytes, o + i * 4l)
        }
        o += n_window * 4l

        // TDT durations
        m.durations |> resize(int(m.n_durations))
        for (i in range64(m.n_durations)) {
            m.durations[i] = int(rd_u32(bytes, o + i * 4l))
        }
        o += m.n_durations * 4l

        // vocab (SPM pieces; blank = id n_vocab lives outside the table)
        let nv = int64(rd_i32(bytes, o)); o += 4l
        m.vocab |> resize(int(nv))
        for (i in range64(nv)) {
            let len = int64(rd_u32(bytes, o)); o += 4l
            m.vocab[i] = pk_bytes_str(bytes, o, len)
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
            o += n_elem * (ttype == GGML_TYPE_F32 ? 4l : 2l)
        }

        // blob layout — dual walk: `bo` is the full layout (qblob on a q8 load, blob on an
        // fp32 load); `ao` is the fp32 walk of blob, which the GEMM regions collapse out of
        // on a q8 load (g = 0). On an fp32 load the walks are identical (ao ends == bo).
        let d = m.n_state
        let ff = 4l * d
        let dh = d / m.n_head
        let nch = m.n_sub_ch
        let g = q8 ? 0l : 1l
        var bo = 0l
        var ao = 0l
        m.c0_w = ao; ao += nch * 9l; bo += nch * 9l
        m.c0_b = ao; ao += nch; bo += nch
        m.c2_w = ao; ao += nch * 9l; bo += nch * 9l          // depthwise: per-channel 3x3
        m.c2_b = ao; ao += nch; bo += nch
        m.c3_w = bo; bo += nch * nch; ao += g * nch * nch
        m.c3_b = ao; ao += nch; bo += nch
        m.c5_w = ao; ao += nch * 9l; bo += nch * 9l
        m.c5_b = ao; ao += nch; bo += nch
        m.c6_w = bo; bo += nch * nch; ao += g * nch * nch
        m.c6_b = ao; ao += nch; bo += nch
        let feat = (m.n_mel / m.subsampling) * nch
        m.pre_w = bo; bo += d * feat; ao += g * d * feat
        m.pre_b = ao; ao += d; bo += d
        m.layers_off = bo
        m.layer_stride = pk_layer_stride(d, ff, m.n_conv_kernel)
        m.aux_layers_off = ao
        m.aux_layer_stride = q8 ? pk_aux_layer_stride(d, m.n_conv_kernel) : m.layer_stride
        bo += m.n_layer * m.layer_stride
        ao += m.n_layer * m.aux_layer_stride
        m.embed_off = ao; ao += (m.n_vocab + 1l) * m.n_pred_dim; bo += (m.n_vocab + 1l) * m.n_pred_dim
        let pd = m.n_pred_dim
        for (_l in range64(m.n_pred_layers)) {
            m.lstm_ih |> push(ao); ao += 4l * pd * pd; bo += 4l * pd * pd
            m.lstm_hh |> push(ao); ao += 4l * pd * pd; bo += 4l * pd * pd
            m.lstm_b |> push(ao); ao += 4l * pd; bo += 4l * pd
        }
        m.joint_enc_w = bo; bo += pd * d; ao += g * pd * d
        m.joint_enc_b = ao; ao += pd; bo += pd
        m.joint_pred_w = ao; ao += pd * pd; bo += pd * pd
        m.joint_pred_b = ao; ao += pd; bo += pd
        let n_logits = m.n_vocab + 1l + m.n_durations
        m.joint_net_w = ao; ao += n_logits * pd; bo += n_logits * pd
        m.joint_net_b = ao; ao += n_logits; bo += n_logits
        // exact allocations: reserve defeats the array allocator's pow2 rounding, which on the
        // 0.6b f32 bin would back 2.4 GiB of blob floats with a 4 GiB allocation
        m.blob |> reserve(ao)
        m.blob |> resize(ao)
        if (q8) {
            m.qblob |> reserve(bo)
            m.qblob |> resize(bo)
            m.qscales |> reserve(bo / 32l)
            m.qscales |> resize(bo / 32l)
        }

        var wscratch : array<float>   // q8 transcode staging, one GEMM tensor at a time
        pk_read(recs, bytes, "encoder.pre_encode.conv.0.weight", m.blob, m.c0_w, nch * 9l)
        pk_conv_tap_major(m.blob, m.c0_w, nch, 9l)   // c0 kernel reads tap-major too
        pk_read(recs, bytes, "encoder.pre_encode.conv.0.bias", m.blob, m.c0_b, nch)
        pk_read(recs, bytes, "encoder.pre_encode.conv.2.weight", m.blob, m.c2_w, nch * 9l)
        pk_conv_tap_major(m.blob, m.c2_w, nch, 9l)
        pk_read(recs, bytes, "encoder.pre_encode.conv.2.bias", m.blob, m.c2_b, nch)
        pk_read_gemm(recs, bytes, "encoder.pre_encode.conv.3.weight", m, wscratch, m.c3_w, nch * nch)
        pk_read(recs, bytes, "encoder.pre_encode.conv.3.bias", m.blob, m.c3_b, nch)
        pk_read(recs, bytes, "encoder.pre_encode.conv.5.weight", m.blob, m.c5_w, nch * 9l)
        pk_conv_tap_major(m.blob, m.c5_w, nch, 9l)
        pk_read(recs, bytes, "encoder.pre_encode.conv.5.bias", m.blob, m.c5_b, nch)
        pk_read_gemm(recs, bytes, "encoder.pre_encode.conv.6.weight", m, wscratch, m.c6_w, nch * nch)
        pk_read(recs, bytes, "encoder.pre_encode.conv.6.bias", m.blob, m.c6_b, nch)
        pk_read_gemm(recs, bytes, "encoder.pre_encode.out.weight", m, wscratch, m.pre_w, d * feat)
        pk_read(recs, bytes, "encoder.pre_encode.out.bias", m.blob, m.pre_b, d)
        for (l in range64(m.n_layer)) {
            let lo = pk_layer_offsets(m, l)
            pk_read(recs, bytes, "encoder.layers.{l}.norm_feed_forward1.weight", m.blob, lo.nf1_w, d)
            pk_read(recs, bytes, "encoder.layers.{l}.norm_feed_forward1.bias", m.blob, lo.nf1_b, d)
            pk_read_gemm(recs, bytes, "encoder.layers.{l}.feed_forward1.linear1.weight", m, wscratch, lo.ff1a_w, ff * d)
            pk_read_gemm(recs, bytes, "encoder.layers.{l}.feed_forward1.linear2.weight", m, wscratch, lo.ff1b_w, d * ff)
            pk_read(recs, bytes, "encoder.layers.{l}.norm_self_att.weight", m.blob, lo.nat_w, d)
            pk_read(recs, bytes, "encoder.layers.{l}.norm_self_att.bias", m.blob, lo.nat_b, d)
            pk_read(recs, bytes, "encoder.layers.{l}.self_attn.pos_bias_u", m.blob, lo.bias_u, dh * m.n_head)
            pk_read(recs, bytes, "encoder.layers.{l}.self_attn.pos_bias_v", m.blob, lo.bias_v, dh * m.n_head)
            pk_read_gemm(recs, bytes, "encoder.layers.{l}.self_attn.linear_q.weight", m, wscratch, lo.q_w, d * d)
            pk_read_gemm(recs, bytes, "encoder.layers.{l}.self_attn.linear_k.weight", m, wscratch, lo.k_w, d * d)
            pk_read_gemm(recs, bytes, "encoder.layers.{l}.self_attn.linear_v.weight", m, wscratch, lo.v_w, d * d)
            pk_read_gemm(recs, bytes, "encoder.layers.{l}.self_attn.linear_out.weight", m, wscratch, lo.o_w, d * d)
            pk_read_gemm(recs, bytes, "encoder.layers.{l}.self_attn.linear_pos.weight", m, wscratch, lo.pos_w, d * d)
            pk_read(recs, bytes, "encoder.layers.{l}.norm_conv.weight", m.blob, lo.ncv_w, d)
            pk_read(recs, bytes, "encoder.layers.{l}.norm_conv.bias", m.blob, lo.ncv_b, d)
            pk_read_gemm(recs, bytes, "encoder.layers.{l}.conv.pointwise_conv1.weight", m, wscratch, lo.pw1_w, 2l * d * d)
            pk_read(recs, bytes, "encoder.layers.{l}.conv.depthwise_conv.weight", m.blob, lo.dw_w, m.n_conv_kernel * d)
            pk_conv_tap_major(m.blob, lo.dw_w, d, m.n_conv_kernel)   // [d][k] -> [k][d] for the f4 row core
            pk_read(recs, bytes, "encoder.layers.{l}.conv.batch_norm.weight", m.blob, lo.bn_w, d)
            pk_read(recs, bytes, "encoder.layers.{l}.conv.batch_norm.bias", m.blob, lo.bn_b, d)
            pk_read(recs, bytes, "encoder.layers.{l}.conv.batch_norm.running_mean", m.blob, lo.bn_mean, d)
            pk_read(recs, bytes, "encoder.layers.{l}.conv.batch_norm.running_var", m.blob, lo.bn_var, d)
            pk_fold_bn(m.blob, lo.bn_w, lo.bn_b, lo.bn_mean, lo.bn_var, d)
            pk_read_gemm(recs, bytes, "encoder.layers.{l}.conv.pointwise_conv2.weight", m, wscratch, lo.pw2_w, d * d)
            pk_read(recs, bytes, "encoder.layers.{l}.norm_feed_forward2.weight", m.blob, lo.nf2_w, d)
            pk_read(recs, bytes, "encoder.layers.{l}.norm_feed_forward2.bias", m.blob, lo.nf2_b, d)
            pk_read_gemm(recs, bytes, "encoder.layers.{l}.feed_forward2.linear1.weight", m, wscratch, lo.ff2a_w, ff * d)
            pk_read_gemm(recs, bytes, "encoder.layers.{l}.feed_forward2.linear2.weight", m, wscratch, lo.ff2b_w, d * ff)
            pk_read(recs, bytes, "encoder.layers.{l}.norm_out.weight", m.blob, lo.nout_w, d)
            pk_read(recs, bytes, "encoder.layers.{l}.norm_out.bias", m.blob, lo.nout_b, d)
        }
        pk_read(recs, bytes, "decoder.prediction.embed.weight", m.blob, m.embed_off, (m.n_vocab + 1l) * m.n_pred_dim)
        for (l in range64(m.n_pred_layers)) {
            pk_read(recs, bytes, "decoder.prediction.dec_rnn.lstm.weight_ih_l{l}", m.blob, m.lstm_ih[l], 4l * m.n_pred_dim * m.n_pred_dim)
            pk_read(recs, bytes, "decoder.prediction.dec_rnn.lstm.weight_hh_l{l}", m.blob, m.lstm_hh[l], 4l * m.n_pred_dim * m.n_pred_dim)
            pk_read(recs, bytes, "decoder.prediction.dec_rnn.lstm.bias_h_l{l}", m.blob, m.lstm_b[l], 4l * m.n_pred_dim)
        }
        pk_read_gemm(recs, bytes, "joint.enc.weight", m, wscratch, m.joint_enc_w, m.n_pred_dim * d)
        pk_read(recs, bytes, "joint.enc.bias", m.blob, m.joint_enc_b, m.n_pred_dim)
        pk_read(recs, bytes, "joint.pred.weight", m.blob, m.joint_pred_w, m.n_pred_dim * m.n_pred_dim)
        pk_read(recs, bytes, "joint.pred.bias", m.blob, m.joint_pred_b, m.n_pred_dim)
        pk_read(recs, bytes, "joint.joint_net.2.weight", m.blob, m.joint_net_w, (m.n_vocab + 1l + m.n_durations) * m.n_pred_dim)
        pk_read(recs, bytes, "joint.joint_net.2.bias", m.blob, m.joint_net_b, m.n_vocab + 1l + m.n_durations)
        delete wscratch
    }
    fclose(f)
    if (q8) {
        pk_repack_all(m)
    }
    return <- m
}

// ===== mel (frame-major!) =====

//! Parakeet's log-mel: preemphasis 0.97, zero center-pad n_fft/2, the file's STFT window, power
//! spectra, mel dot, NATURAL log(sum + 2⁻²⁴), then per-feature mean/std normalization over the
//! valid frames only (Bessel n−1, std + 1e-5), applied to all. FRAME-major ``[n_len][n_mel]``.
def parakeet_log_mel(samples : array<float> | #; m : ParakeetModel; var st : ParakeetState) {
    let n_in = int64(length(samples))
    pk_ensure(st.pre, n_in)
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
    pk_ensure(st.padded, n_padded)
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
    pk_ensure(st.mel.data, st.mel.n_len * m.n_mel)

    let n_fft = m.n_fft
    let n_bins = n_fft / 2l + 1l
    let wsz = int64(length(m.window))
    let wpad = (n_fft - wsz) / 2l
    let mel_eps = 5.960464477539063e-08lf
    let n_len = st.mel.n_len
    let n_mel = m.n_mel

    // per-frame windowed 512-pt FFT + mel matvec, threaded over frames (frames are
    // independent — bit-exact at any lane count); scratch is a per-worker stack frame,
    // so a 9-minute clip no longer materializes the whole [frames × n_fft] image
    unsafe {
        let pdd = addr<float const?>(st.padded[0])
        let wnd = addr<float const?>(m.window[0])
        let flt = addr<float const?>(m.filters[0])
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
def private pk_conv2d_c1_s2_relu(var out : array<float>; x : array<float>; blob : array<float>;
                                 w_off, b_off : int64; iw, ih, cout : int64) {
    let ow = (iw - 1l) / 2l + 1l
    let oh = (ih - 1l) / 2l + 1l
    pk_ensure(out, oh * ow * cout)
    unsafe {
        var op = addr(out[0])
        let xp = addr<float const?>(x[0])
        let wp = addr<float const?>(blob[w_off])
        let bp = addr<float const?>(blob[b_off])
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
def private pk_conv2d_dw_s2(var out : array<float>; x : array<float>; blob : array<float>;
                            w_off, b_off : int64; iw, ih, ch : int64) {
    let ow = (iw - 1l) / 2l + 1l
    let oh = (ih - 1l) / 2l + 1l
    let oy_end = (ih - 2l) / 2l + 1l   // last oy with iy=2*oy+1 in bounds, exclusive
    let ox_end = (iw - 2l) / 2l + 1l
    let rs = iw * ch
    pk_ensure(out, oh * ow * ch)
    unsafe {
        var op = addr(out[0])
        let xp = addr<float const?>(x[0])
        let wp = addr<float const?>(blob[w_off])
        let bp = addr<float const?>(blob[b_off])
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

// fused bias+relu over rows (the 1x1 conv epilogues) — one float4 pass instead of the
// add_bias_rows + relu pair re-streaming the image (bench: 11.8x); threads over rows;
// explicit npos: scratch is oversized
def private pk_bias_relu(var y : array<float>; blob : array<float>; b_off, d, npos : int64) {
    let d4 = d - d % 4l
    unsafe {
        var yp = addr(y[0])
        let bp = addr<float const?>(blob[b_off])
        let zero = float4(0.0)
        maybe_parallel_for(0, int(npos), lanes_for_work(npos * d, 0)) $(rb, re) {
            unsafe {
                for (p in range64(int64(rb), int64(re))) {
                    let row = p * d
                    var i = 0l
                    while (i < d4) {
                        let v = (*reinterpret<float4 const?>(yp + row + i)) + (*reinterpret<float4 const?>(bp + i))
                        *reinterpret<float4?>(yp + row + i) = max(v, zero)
                        i += 4l
                    }
                    while (i < d) {
                        yp[row + i] = max(yp[row + i] + bp[i], 0.0)
                        i++
                    }
                }
            }
        }
    }
}

// ===== encoder =====

//! FastConformer encoder over the whole mel: conv subsampling, then 24 conformer blocks (macaron
//! FFN, rel-pos attention, GLU+depthwise-conv+BN, post norm) — global attention only, panics past
//! the oracle's 8192-frame local-attention switch. Reads ``st.mel``, leaves rows in ``st.x``.
def parakeet_encode(m : ParakeetModel; var st : ParakeetState) : int64 {
    let d = m.n_state
    let nch = m.n_sub_ch
    let n_head = m.n_head
    let dh = d / n_head

    // conv subsampling ([time][mel] rows are already the [ih][iw][c=1] layout)
    var tp = ref_time_ticks()
    let t1 = (st.mel.n_len - 1l) / 2l + 1l
    let w1 = (m.n_mel - 1l) / 2l + 1l          // 64
    pk_conv2d_c1_s2_relu(st.y0, st.mel.data, m.blob, m.c0_w, m.c0_b, m.n_mel, st.mel.n_len, nch)
    pk_conv2d_dw_s2(st.y2, st.y0, m.blob, m.c2_w, m.c2_b, w1, t1, nch)
    let w2 = (w1 - 1l) / 2l + 1l               // 32
    let t2 = (t1 - 1l) / 2l + 1l
    pk_ensure(st.y3, t2 * w2 * nch)
    pk_scan("y0", st.y0, t1 * w1 * nch)
    pk_scan("y2", st.y2, t2 * w2 * nch)
    pk_mm_rq(m, st.y3, m.c3_w, st.y2, st.xqi, st.xsi, nch, nch, t2 * w2)   // 1x1 conv = per-position channel mix
    pk_bias_relu(st.y3, m.blob, m.c3_b, nch, t2 * w2)
    pk_conv2d_dw_s2(st.y5, st.y3, m.blob, m.c5_w, m.c5_b, w2, t2, nch)
    let w3 = (w2 - 1l) / 2l + 1l               // 16
    let t3 = (t2 - 1l) / 2l + 1l
    pk_ensure(st.y6, t3 * w3 * nch)
    pk_mm_rq(m, st.y6, m.c6_w, st.y5, st.xqi, st.xsi, nch, nch, t3 * w3)
    pk_bias_relu(st.y6, m.blob, m.c6_b, nch, t3 * w3)

    // features per time row: v[f + w3·c] = y6[t][f][c]; then pre_out into the residual stream
    let tt = t3
    if (tt > LOCAL_ATTN_THRESHOLD) {
        panic("dasLLAMA parakeet: {tt} encoder frames crosses the local-attention switch — clip audio to under ~10.9 minutes")
    }
    let feat = w3 * nch
    pk_ensure(st.xf, tt * feat)
    pk_xf_permute(st.xf, st.y6, tt, w3, nch)
    pk_ensure(st.x, tt * d)
    pk_scan("y6", st.y6, t3 * w3 * nch)
    pk_mm_rq(m, st.x, m.pre_w, st.xf, st.xqi, st.xsi, feat, d, tt)
    add_bias_rows(st.x, m.blob, m.pre_b, d, tt)
    pk_scan("pre", st.x, tt * d)
    asr_prof_add("pk.conv_subsample", tp)
    tp = ref_time_ticks()

    // masked tail (padded mel frames past the real audio): none for full-clip encode when
    // ceil-per-stage == ceil(n/8); the oracle masks keys >= ceil(n_len_org/8)
    let n_real = (st.mel.n_len_org + m.subsampling - 1l) / m.subsampling

    // rel-pos embedding table [W × d], interleaved (sin,cos) pairs; W = 2T−1
    let ww = 2l * tt - 1l
    pk_ensure(st.pos_emb, ww * d)
    for (w in range64(ww)) {
        let rel = double(tt - 1l - w)
        for (k in range64(d / 2l)) {
            let freq = exp(-(double(k * 2l) * log(10000.0lf) / double(d)))
            let theta = float(freq * rel)
            st.pos_emb[w * d + 2l * k] = sin(theta)
            st.pos_emb[w * d + 2l * k + 1l] = cos(theta)
        }
    }
    // the rel-pos image is layer-invariant: one requant serves every layer's pos projection
    if (m.q8) {
        pk_requant(st.pos_emb, d, ww, st.posq, st.poss)
    }
    asr_prof_add("pk.pos_emb", tp)

    // scratch
    pk_ensure(st.xb, tt * d)
    pk_ensure(st.h, tt * 4l * d)
    pk_ensure(st.qq, tt * d)
    pk_ensure(st.kk, tt * d)
    pk_ensure(st.vv, tt * d)
    pk_ensure(st.posl, ww * d)
    pk_ensure(st.quh, tt * dh)
    pk_ensure(st.quh2, tt * dh)
    pk_ensure(st.vh, tt * dh)
    pk_ensure(st.kth, dh * tt)
    pk_ensure(st.scores, tt * tt)
    pk_ensure(st.relm, tt * ww)
    pk_ensure(st.post, dh * ww)
    pk_ensure(st.oh, tt * dh)
    pk_ensure(st.ao, tt * d)
    let scale = 1.0 / sqrt(float(dh))

    for (l in range64(m.n_layer)) {
        let lo = pk_layer_offsets(m, l)

        // FFN1 (macaron half)
        tp = ref_time_ticks()
        var tf = tp
        layernorm_batch(st.xb, st.x, m.blob, lo.nf1_w, lo.nf1_b, d, tt, m.eps)
        asr_prof_add("pk.ffn.ln", tf)
        tf = ref_time_ticks()
        pk_mm_rq(m, st.h, lo.ff1a_w, st.xb, st.xqi, st.xsi, d, 4l * d, tt)
        asr_prof_add("pk.ffn.a", tf)
        tf = ref_time_ticks()
        silu4_batch(st.h, 4l * d, tt)
        asr_prof_add("pk.ffn.silu", tf)
        tf = ref_time_ticks()
        pk_mm_rq(m, st.xb, lo.ff1b_w, st.h, st.xqi, st.xsi, 4l * d, d, tt)
        asr_prof_add("pk.ffn.b", tf)
        tf = ref_time_ticks()
        for (i in range64(tt * d)) {
            st.x[i] += 0.5 * st.xb[i]
        }
        asr_prof_add("pk.ffn.res", tf)
        pk_scan("L{l}.ffn1", st.x, tt * d)
        asr_prof_add("pk.ffn", tp)

        // rel-pos attention
        tp = ref_time_ticks()
        var tq = tp
        layernorm_batch(st.xb, st.x, m.blob, lo.nat_w, lo.nat_b, d, tt, m.eps)
        asr_prof_add("pk.attn_proj.ln", tq)
        tq = ref_time_ticks()
        if (m.q8) {   // q/k/v share the post-LN activation: one requant, three GEMMs
            pk_requant(st.xb, d, tt, st.xqi, st.xsi)
        }
        asr_prof_add("pk.attn_proj.rq", tq)
        tq = ref_time_ticks()
        pk_mm(m, st.qq, lo.q_w, st.xb, st.xqi, st.xsi, d, d, tt)
        asr_prof_add("pk.attn_proj.q", tq)
        tq = ref_time_ticks()
        pk_mm(m, st.kk, lo.k_w, st.xb, st.xqi, st.xsi, d, d, tt)
        asr_prof_add("pk.attn_proj.k", tq)
        tq = ref_time_ticks()
        pk_mm(m, st.vv, lo.v_w, st.xb, st.xqi, st.xsi, d, d, tt)
        asr_prof_add("pk.attn_proj.v", tq)
        tq = ref_time_ticks()
        // pos projection for this layer: [W × d], against the hoisted layer-invariant image
        pk_mm(m, st.posl, lo.pos_w, st.pos_emb, st.posq, st.poss, d, d, ww)
        asr_prof_add("pk.attn_proj.pos", tq)
        asr_prof_add("pk.attn_proj", tp)
        tp = ref_time_ticks()
        for (hi in range64(n_head)) {
            let hoff = hi * dh
            unsafe {
                var qup = addr(st.quh[0])
                var qvp = addr(st.quh2[0])
                var ktp = addr(st.kth[0])
                var vhp = addr(st.vh[0])
                var pop = addr(st.post[0])
                var scp = addr(st.scores[0])
                var rlp = addr(st.relm[0])
                var ohp = addr(st.oh[0])
                var aop = addr(st.ao[0])
                let qqp = addr<float const?>(st.qq[0])
                let kkp = addr<float const?>(st.kk[0])
                let vvp = addr<float const?>(st.vv[0])
                let plp = addr<float const?>(st.posl[0])
                let bup = addr<float const?>(m.blob[lo.bias_u + hi * dh])
                let bvp = addr<float const?>(m.blob[lo.bias_v + hi * dh])
                // transposed head views ((Q+u), (Q+v), Kᵀ, V_h, pos_hᵀ) — the row GEMMs read
                // these in full, so they build before the compute dispatch
                var th = ref_time_ticks()
                maybe_parallel_for(0, int(tt), lanes_for_work(tt * dh * 4l, 0)) $(rb, re) {
                    unsafe {
                        for (t in range64(int64(rb), int64(re))) {
                            for (j in range64(dh)) {
                                let qv = qqp[t * d + hoff + j]
                                qup[t * dh + j] = qv + bup[j]
                                qvp[t * dh + j] = qv + bvp[j]
                                ktp[j * tt + t] = kkp[t * d + hoff + j]
                                vhp[t * dh + j] = vvp[t * d + hoff + j]
                            }
                        }
                    }
                }
                asr_prof_add("pk.attn_heads.pack", th)
                th = ref_time_ticks()
                maybe_parallel_for(0, int(ww), lanes_for_work(ww * dh, 0)) $(rb, re) {
                    unsafe {
                        for (w in range64(int64(rb), int64(re))) {
                            for (j in range64(dh)) {
                                pop[j * ww + w] = plp[w * d + hoff + j]
                            }
                        }
                    }
                }
                asr_prof_add("pk.attn_heads.pos", th)
                th = ref_time_ticks()
                // content scores (Q+u)·Kᵀ + rel scores (Q+v)·pos_hᵀ + softmax + P·V_h, threaded in
                // 4-ROW BLOCKS: gemm_f32's 4×16 tile is row-position-dependent, so 4-aligned chunk
                // starts keep FMA order bit-exact vs the whole-matrix call at any lane count.
                let nblk4 = (tt + 3l) / 4l
                maybe_parallel_for(0, int(nblk4), lanes_for_work(tt * dh * (2l * tt + ww), 0)) $(bb, be) {
                    unsafe {
                        let r0 = int64(bb) * 4l
                        let r1 = min(int64(be) * 4l, tt)
                        let nr = r1 - r0
                        for (i in range64(r0 * tt, r1 * tt)) {
                            scp[i] = 0.0
                        }
                        for (i in range64(r0 * ww, r1 * ww)) {
                            rlp[i] = 0.0
                        }
                        gemm_f32_jo(scp + r0 * tt, qup + r0 * dh, ktp, nr, dh, tt)
                        gemm_f32_jo(rlp + r0 * ww, qvp + r0 * dh, pop, nr, dh, ww)
                        for (q in range64(r0, r1)) {
                            let base = tt - 1l - q
                            for (k in range64(tt)) {
                                var s = (scp[q * tt + k] + rlp[q * ww + base + k]) * scale
                                if (k >= n_real) {
                                    s = -1e30
                                }
                                scp[q * tt + k] = s
                            }
                            softmax(scp + q * tt, tt)
                        }
                        for (i in range64(r0 * dh, r1 * dh)) {
                            ohp[i] = 0.0
                        }
                        gemm_f32_jo(ohp + r0 * dh, scp + r0 * tt, vhp, nr, tt, dh)
                        for (t in range64(r0, r1)) {
                            for (j in range64(dh)) {
                                aop[t * d + hoff + j] = ohp[t * dh + j]
                            }
                        }
                    }
                }
                asr_prof_add("pk.attn_heads.blocks", th)
            }
        }
        asr_prof_add("pk.attn_heads", tp)
        tp = ref_time_ticks()
        pk_mm_rq(m, st.xb, lo.o_w, st.ao, st.xqi, st.xsi, d, d, tt)
        add_inplace_rows(st.x, st.xb, tt * d)
        pk_scan("L{l}.attn", st.x, tt * d)
        asr_prof_add("pk.attn_out", tp)

        // conv module
        tp = ref_time_ticks()
        layernorm_batch(st.xb, st.x, m.blob, lo.ncv_w, lo.ncv_b, d, tt, m.eps)
        pk_mm_rq(m, st.h, lo.pw1_w, st.xb, st.xqi, st.xsi, d, 2l * d, tt)
        unsafe {
            let hp = addr<float const?>(st.h[0])
            var xbp = addr(st.xb[0])
            var aop = addr(st.ao[0])
            let dwp = addr<float const?>(m.blob[lo.dw_w])     // tap-major [k][d] since load
            let scp = addr<float const?>(m.blob[lo.bn_w])     // folded BN scale (pk_fold_bn)
            let shp = addr<float const?>(m.blob[lo.bn_b])     // folded BN shift
            let nk = m.n_conv_kernel
            let kpad = (nk - 1l) / 2l   // depthwise pad (k−1)/2 both sides
            let one = float4(1.0, 1.0, 1.0, 1.0)
            let n4 = d / 4l
            let d4ok = n4 * 4l == d
            maybe_parallel_for(0, int(tt), lanes_for_work(tt * d * 4l, 0)) $(rb, re) {
                unsafe {
                    for (t in range64(int64(rb), int64(re))) {   // GLU: signal · σ(gate), float4 exp4
                        if (d4ok) {
                            let sgp = reinterpret<float4 const?>(hp + t * 2l * d)
                            let gtp = reinterpret<float4 const?>(hp + t * 2l * d + d)
                            var o4 = reinterpret<float4?>(xbp + t * d)
                            for (k in range64(n4)) {
                                let arg = clamp(-gtp[k], EXP4_LO, EXP4_HI)
                                o4[k] = sgp[k] / (one + exp4(arg))
                            }
                        } else {   // d % 4 != 0 models only
                            for (j in range64(d)) {
                                let sg = hp[t * 2l * d + j]
                                let gt = hp[t * 2l * d + d + j]
                                xbp[t * d + j] = sg * (1.0 / (1.0 + exp(-gt)))
                            }
                        }
                    }
                }
            }
            // depthwise conv1d k9 over time (reads xb rows t±kpad — barrier from the GLU
            // dispatch above), folded BN + silu; f4 row core on the all-taps-in-range
            // interior when k==9, guarded scalar rows otherwise (borders / odd k / odd d)
            let f4ok = nk == 9l && d4ok
            maybe_parallel_for(0, int(tt), lanes_for_work(tt * d * (nk + 4l), 0)) $(rb, re) {
                unsafe {
                    for (t in range64(int64(rb), int64(re))) {
                        if (f4ok && t >= kpad && t < tt - kpad) {
                            var j = 0l
                            while (j < d) {
                                let base = (t - 4l) * d + j
                                var acc = (*reinterpret<float4 const?>(xbp + base)) * (*reinterpret<float4 const?>(dwp + j))
                                acc = mad(*reinterpret<float4 const?>(xbp + base + d), *reinterpret<float4 const?>(dwp + d + j), acc)
                                acc = mad(*reinterpret<float4 const?>(xbp + base + 2l * d), *reinterpret<float4 const?>(dwp + 2l * d + j), acc)
                                acc = mad(*reinterpret<float4 const?>(xbp + base + 3l * d), *reinterpret<float4 const?>(dwp + 3l * d + j), acc)
                                acc = mad(*reinterpret<float4 const?>(xbp + base + 4l * d), *reinterpret<float4 const?>(dwp + 4l * d + j), acc)
                                acc = mad(*reinterpret<float4 const?>(xbp + base + 5l * d), *reinterpret<float4 const?>(dwp + 5l * d + j), acc)
                                acc = mad(*reinterpret<float4 const?>(xbp + base + 6l * d), *reinterpret<float4 const?>(dwp + 6l * d + j), acc)
                                acc = mad(*reinterpret<float4 const?>(xbp + base + 7l * d), *reinterpret<float4 const?>(dwp + 7l * d + j), acc)
                                acc = mad(*reinterpret<float4 const?>(xbp + base + 8l * d), *reinterpret<float4 const?>(dwp + 8l * d + j), acc)
                                let bv4 = mad(acc, *reinterpret<float4 const?>(scp + j), *reinterpret<float4 const?>(shp + j))
                                let arg = clamp(-bv4, EXP4_LO, EXP4_HI)
                                *reinterpret<float4?>(aop + t * d + j) = bv4 / (one + exp4(arg))
                                j += 4l
                            }
                        } elif (f4ok) {   // border rows: guarded taps (uniform over j), still f4
                            var j = 0l
                            while (j < d) {
                                var acc = float4(0.0, 0.0, 0.0, 0.0)
                                for (k in range64(9l)) {
                                    let src = t + k - kpad
                                    if (src >= 0l && src < tt) {
                                        acc = mad(*reinterpret<float4 const?>(xbp + src * d + j), *reinterpret<float4 const?>(dwp + k * d + j), acc)
                                    }
                                }
                                let bv4 = mad(acc, *reinterpret<float4 const?>(scp + j), *reinterpret<float4 const?>(shp + j))
                                let arg = clamp(-bv4, EXP4_LO, EXP4_HI)
                                *reinterpret<float4?>(aop + t * d + j) = bv4 / (one + exp4(arg))
                                j += 4l
                            }
                        } else {   // generic k / d % 4 != 0 fallback
                            for (j in range64(d)) {
                                var acc = 0.0
                                for (k in range64(nk)) {
                                    let src = t + k - kpad
                                    if (src >= 0l && src < tt) {
                                        acc += dwp[k * d + j] * xbp[src * d + j]
                                    }
                                }
                                let bv = acc * scp[j] + shp[j]
                                aop[t * d + j] = bv * (1.0 / (1.0 + exp(-bv)))   // silu
                            }
                        }
                    }
                }
            }
        }
        pk_mm_rq(m, st.xb, lo.pw2_w, st.ao, st.xqi, st.xsi, d, d, tt)
        add_inplace_rows(st.x, st.xb, tt * d)
        pk_scan("L{l}.conv", st.x, tt * d)
        asr_prof_add("pk.conv_module", tp)

        // FFN2 (second macaron half)
        tp = ref_time_ticks()
        layernorm_batch(st.xb, st.x, m.blob, lo.nf2_w, lo.nf2_b, d, tt, m.eps)
        pk_mm_rq(m, st.h, lo.ff2a_w, st.xb, st.xqi, st.xsi, d, 4l * d, tt)
        silu4_batch(st.h, 4l * d, tt)
        pk_mm_rq(m, st.xb, lo.ff2b_w, st.h, st.xqi, st.xsi, 4l * d, d, tt)
        for (i in range64(tt * d)) {
            st.x[i] += 0.5 * st.xb[i]
        }

        // per-layer post norm
        layernorm_batch(st.x, st.x, m.blob, lo.nout_w, lo.nout_b, d, tt, m.eps)
        pk_scan("L{l}.out", st.x, tt * d)
        asr_prof_add("pk.ffn", tp)
    }

    // the encoder rows stay in st.x — the TDT decode reads them from there
    return tt
}

// ===== prediction network + joint + TDT greedy =====

// size the predictor state and zero the LSTM h/c (a fresh utterance starts from zeros;
// the scratch fields are written before every read)
def private pk_pred_reset(m : ParakeetModel; var s : ParakeetPredState) {
    let pd = m.n_pred_dim
    pk_ensure(s.h, m.n_pred_layers * pd)
    pk_ensure(s.c, m.n_pred_layers * pd)
    pk_ensure(s.pred, pd)
    pk_ensure(s.gates, 4l * pd)
    pk_ensure(s.gates2, 4l * pd)
    pk_ensure(s.xin, pd)
    pk_ensure(s.hbuf, pd)
    for (i in range64(m.n_pred_layers * pd)) {
        s.h[i] = 0.0
        s.c[i] = 0.0
    }
}

// advance the LSTM stack for `token` and refresh s.pred (gates packed [i,f,o,g])
def private pk_predict(m : ParakeetModel; var s : ParakeetPredState; token : int) {
    let pd = m.n_pred_dim
    for (j in range64(pd)) {
        s.xin[j] = m.blob[m.embed_off + int64(token) * pd + j]
    }
    for (l in range64(m.n_pred_layers)) {
        for (j in range64(pd)) {
            s.hbuf[j] = s.h[l * pd + j]
        }
        matmul(s.gates, m.blob, m.lstm_ih[l], s.xin, pd, 4l * pd)
        matmul(s.gates2, m.blob, m.lstm_hh[l], s.hbuf, pd, 4l * pd)
        // gates via float4 exp4: σ = 1/(1+exp4(-a)), tanh(a) = 1 − 2/(1+exp4(2a)) (geglu4 identity)
        unsafe {
            let gp = addr<float const?>(s.gates[0])
            let g2p = addr<float const?>(s.gates2[0])
            let bp = addr<float const?>(m.blob[m.lstm_b[l]])
            var cp = addr(s.c[l * pd])
            var hsp = addr(s.h[l * pd])
            var xp = addr(s.xin[0])
            let one = float4(1.0, 1.0, 1.0, 1.0)
            let two = float4(2.0, 2.0, 2.0, 2.0)
            let n4 = (pd / 4l) * 4l
            var j = 0l
            while (j < n4) {
                let ia = (*reinterpret<float4 const?>(gp + j)) + (*reinterpret<float4 const?>(g2p + j)) + (*reinterpret<float4 const?>(bp + j))
                let fa = (*reinterpret<float4 const?>(gp + pd + j)) + (*reinterpret<float4 const?>(g2p + pd + j)) + (*reinterpret<float4 const?>(bp + pd + j))
                let oa = (*reinterpret<float4 const?>(gp + 2l * pd + j)) + (*reinterpret<float4 const?>(g2p + 2l * pd + j)) + (*reinterpret<float4 const?>(bp + 2l * pd + j))
                let ga = (*reinterpret<float4 const?>(gp + 3l * pd + j)) + (*reinterpret<float4 const?>(g2p + 3l * pd + j)) + (*reinterpret<float4 const?>(bp + 3l * pd + j))
                let ig = one / (one + exp4(clamp(-ia, EXP4_LO, EXP4_HI)))
                let fg = one / (one + exp4(clamp(-fa, EXP4_LO, EXP4_HI)))
                let og = one / (one + exp4(clamp(-oa, EXP4_LO, EXP4_HI)))
                let cg = one - two / (one + exp4(clamp(two * ga, EXP4_LO, EXP4_HI)))
                let cn = fg * (*reinterpret<float4 const?>(cp + j)) + ig * cg
                *reinterpret<float4?>(cp + j) = cn
                let hn = og * (one - two / (one + exp4(clamp(two * cn, EXP4_LO, EXP4_HI))))
                *reinterpret<float4?>(hsp + j) = hn
                *reinterpret<float4?>(xp + j) = hn
                j += 4l
            }
            for (jj in range64(n4, pd)) {   // scalar tail (pd % 4 != 0 models only)
                let ig = 1.0 / (1.0 + exp(-(gp[jj] + g2p[jj] + bp[jj])))
                let fg = 1.0 / (1.0 + exp(-(gp[pd + jj] + g2p[pd + jj] + bp[pd + jj])))
                let og = 1.0 / (1.0 + exp(-(gp[2l * pd + jj] + g2p[2l * pd + jj] + bp[2l * pd + jj])))
                let cg = tanh(gp[3l * pd + jj] + g2p[3l * pd + jj] + bp[3l * pd + jj])
                let cn = fg * cp[jj] + ig * cg
                cp[jj] = cn
                let hn = og * tanh(cn)
                hsp[jj] = hn
                xp[jj] = hn
            }
        }
    }
    // joint.pred projection of the top hidden (already in s.xin after the last layer)
    matmul(s.pred, m.blob, m.joint_pred_w, s.xin, pd, pd)
    for (j in range64(pd)) {
        s.pred[j] += m.blob[m.joint_pred_b + j]
    }
}

// NaN/Inf stage scan (debug rail, pairs with the fast_math x64 investigation): bit-pattern
// detection — a float `v != v` check folds to false under fast_math's nnan, this survives.
var g_pk_nan_scan = false

def set_pk_nan_scan(on : bool) {
    g_pk_nan_scan = on
}

def private pk_scan(tag : string; x : array<float>; n : int64) {
    if (!g_pk_nan_scan) {
        return
    }
    var nan = 0
    var inf = 0
    var first = -1
    for (i in range(int(n))) {
        let bits = unsafe(reinterpret<uint>(x[i]))
        if ((bits & 0x7F800000u) == 0x7F800000u) {
            if ((bits & 0x007FFFFFu) != 0u) {
                nan++
            } else {
                inf++
            }
            if (first < 0) {
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

// decode trace (debug rail): per-step duration log-probs alongside the choice
var g_parakeet_trace = false

def set_parakeet_trace(on : bool) {
    g_parakeet_trace = on
}

//! Greedy TDT decode over the encoder rows in ``st.x`` — whisper.cpp's parakeet_decode loop:
//! joint softmax per frame, argmax token + duration, blank advances time, duration 0 caps at
//! n_max_tokens/frame. Tokens land in ``st.toks``.
def parakeet_tdt_decode(m : ParakeetModel; var st : ParakeetState; n_frames : int64) {
    st.toks |> clear()
    let d = m.n_state
    let pd = m.n_pred_dim
    let n_logits = m.n_vocab + 1l + m.n_durations
    let n_tok_logits = int(m.n_vocab + 1l)

    // precompute the joint's encoder projection for every frame [T × pred_dim]
    pk_ensure(st.encp, n_frames * pd)
    pk_mm_rq(m, st.encp, m.joint_enc_w, st.x, st.xqi, st.xsi, d, pd, n_frames)
    add_bias_rows(st.encp, m.blob, m.joint_enc_b, pd, n_frames)

    pk_pred_reset(m, st.pred)
    pk_predict(m, st.pred, m.blank_id)

    pk_ensure(st.joint, pd)
    pk_ensure(st.logits, n_logits)

    var t = 0l
    var emitted = 0
    while (t < n_frames) {
        for (j in range64(pd)) {
            let v = st.encp[t * pd + j] + st.pred.pred[j]
            st.joint[j] = max(v, 0.0)   // relu
        }
        mm_blob_b(st.logits, m.blob, m.joint_net_w, st.joint, pd, n_logits, 1l)
        for (i in range64(n_logits)) {
            st.logits[i] += m.blob[m.joint_net_b + i]
        }
        // log-softmax over the WHOLE row (the oracle's plog normalizer includes durations)
        // — explicit n_logits bounds: the scratch arrays are oversized on reuse
        var mx = st.logits[0]
        for (i in range64(1l, n_logits)) {
            mx = max(mx, st.logits[i])
        }
        var lse = 0.0lf
        for (i in range64(n_logits)) {
            lse += double(exp(st.logits[i] - mx))
        }
        let logz = float(log(lse)) + mx

        var best = 0
        for (i in range(1, n_tok_logits)) {
            if (st.logits[i] > st.logits[best]) {
                best = i
            }
        }
        // duration argmax over fp32 SOFTMAX values, not raw logits: the oracle's probs flush
        // durations' ~e⁻¹¹⁰ mass to exactly 0.0 on underflow, so ties default to index 0 —
        // an analytic logit argmax would diverge from the oracle here.
        var bdur = 0
        var bdp = exp(st.logits[n_tok_logits] - mx)
        for (i in range(1, int(m.n_durations))) {
            let dp = exp(st.logits[n_tok_logits + i] - mx)
            if (dp > bdp) {
                bdp = dp
                bdur = i
            }
        }
        var dur = m.durations[bdur]
        if (g_parakeet_trace) {
            print("  t={t} tok={best} durp=[")
            for (i in range(int(m.n_durations))) {
                print("{exp(st.logits[n_tok_logits + i] - mx)} ")
            }
            print("] -> {bdur}\n")
        }

        if (best == m.blank_id) {
            if (dur == 0) {
                dur = 1
            }
            t += int64(dur)
            emitted = 0
            continue
        }

        // token probability renormalized over the token slice (the oracle's create_token_data)
        let plog = st.logits[best] - logz
        var tok_sum = 0.0lf
        for (i in range(n_tok_logits)) {
            tok_sum += double(exp(st.logits[i] - logz))
        }
        st.toks |> push(ParakeetToken(id = best, dur_idx = bdur, dur = dur, frame = t,
            p = float(double(exp(plog)) / tok_sum), plog = plog))

        pk_predict(m, st.pred, best)
        if (dur > 0) {
            t += int64(dur)
            emitted = 0
        } else {
            emitted++
            if (emitted >= int(m.n_max_tokens)) {
                t++
                emitted = 0
            }
        }
    }
}

//! SPM detokenize: ▁ becomes a space, except while the transcript is still empty inside the
//! FIRST piece (the oracle's sentencepiece_piece_to_text rule); `<...>` control pieces
//! render as nothing.
def parakeet_detokenize(m : ParakeetModel; toks : array<ParakeetToken> | #) : string {
    var segs : array<string>
    var idx = 0
    for (t in toks) {
        let piece = m.vocab[t.id]
        if (length(piece) > 2 && starts_with(piece, "<") && ends_with(piece, ">")) {
            idx++
            continue
        }
        var s = replace(piece, "▁", " ")
        if (idx == 0) {   // first piece: strip the leading spaces the ▁ prefix produced
            while (starts_with(s, " ")) {
                s = slice(s, 1, length(s))
            }
        }
        segs |> push(s)
        idx++
    }
    return join(segs, "")
}
