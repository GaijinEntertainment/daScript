options gen2
options _comment_hygiene = true

module dasllama_gemma4a shared public

require math
require daslib/fio
require strings
require dasllama/dasllama_math
require dasllama/dasllama_gguf
require dasllama/dasllama_audio

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

//! Per-op activation-quant calibration range (mtmd ClippableLinear). ``active`` false = a plain
//! matmul (wide default range = no-op clamp). clamp(x, imin, imax) before the matmul, clamp the
//! result to (omin, omax) after.
[safe_when_uninitialized]
struct private Clamp {
    imin : float = -3.4028235e38
    imax : float = 3.4028235e38
    omin : float = -3.4028235e38
    omax : float = 3.4028235e38
    active : bool = false
}

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
    filters : array<float>    // [n_mel × n_bins] HTK, no area norm
    hann : array<float>       // [512]: 320 Hann at the front, zeros to 512
    twiddles : array<float>   // DFT twiddle matrix [n_fft × (n_bins·2)]
    pos_emb : array<float>    // sinusoidal RPE table [13 × d_model]
    ones : array<float>       // [proj_dim] weight for the final weightless RMSNorm
    blob : array<float>
    // subsample conv frontend + input projection
    c0_w : int64; norm0_w : int64
    c1_w : int64; norm1_w : int64
    inproj_w : int64
    // output projection + audio multimodal embedder
    outproj_w : int64; outproj_b : int64
    mmproj_w : int64
    layers : array<G4aLayer>
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
}

def private g4a_ensure(var a : array<float>; n : int64) {
    if (int64(length(a)) < n) {
        a |> reserve(int(n))
        a |> resize(int(n))
    }
}

// ===== loader =====

def private g4a_read1(m : GGUFMeta; bytes : array<uint8> | #; name : string) : float {
    var tmp : array<float>
    tmp |> resize(1)
    gguf_read_tensor_f32(m, bytes, name, tmp, 0l, 1l)
    return tmp[0]
}

def private g4a_read_clamp(m : GGUFMeta; bytes : array<uint8> | #; base : string) : Clamp {
    var c = Clamp()
    if (gguf_find_tensor(m, "{base}.input_max") >= 0) {
        c.imax = g4a_read1(m, bytes, "{base}.input_max")
        c.imin = g4a_read1(m, bytes, "{base}.input_min")
        c.omax = g4a_read1(m, bytes, "{base}.output_max")
        c.omin = g4a_read1(m, bytes, "{base}.output_min")
        c.active = true
    }
    return c
}

//! HTK triangular mel filterbank ``[n_mel][n_bins]`` (fmin=0, fmax=sr/2, no area normalization) —
//! a direct port of mtmd's ``fill_mel_filterbank_matrix`` HTK branch for gemma4a.
def private g4a_htk_filterbank(n_mel, n_fft, sample_rate : int64) : array<float> {
    let n_bins = n_fft / 2l + 1l
    let fmax = 0.5lf * double(sample_rate)
    let m_hi = 2595.0lf * log10(1.0lf + fmax / 700.0lf)
    var hz_pts : array<double>
    hz_pts |> resize(int(n_mel + 2l))
    for (i in range64(n_mel + 2l)) {
        let mel = m_hi * double(i) / double(n_mel + 1l)   // m_lo = hz_to_mel(0) = 0
        hz_pts[i] = 700.0lf * (pow(10.0lf, mel / 2595.0lf) - 1.0lf)
    }
    let bin_hz_step = double(sample_rate) / double(n_fft)
    var out : array<float>
    out |> resize(int(n_mel * n_bins))
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

//! Load a gemma4a audio mmproj GGUF (projector type `gemma4a`, bf16 tensors). Reads only the
//! `a.*` / `mm.a.*` audio tensors (the vision tower is ignored) into one fp32 blob.
def load_gemma4a_encoder(path : string) : Gemma4aEncoder {
    var enc = Gemma4aEncoder()
    let f = fopen(path, "rb")
    if (f == null) {
        panic("dasLLAMA gemma4a: cannot open mmproj gguf '{path}'")
    }
    fmap(f) $(var bytes : array<uint8>#) {
        let m <- parse_gguf_meta(bytes)
        let proj = (gguf_has(m, "clip.audio.projector_type")
            ? gguf_str(m, bytes, "clip.audio.projector_type")
            : gguf_str(m, bytes, "clip.projector_type"))
        if (proj != "gemma4a") {
            panic("dasLLAMA gemma4a: mmproj projector type is '{proj}', expected 'gemma4a'")
        }
        enc.n_mel = gguf_int(m, bytes, "clip.audio.num_mel_bins")
        enc.d_model = gguf_int(m, bytes, "clip.audio.embedding_length")
        enc.n_head = gguf_int(m, bytes, "clip.audio.attention.head_count")
        enc.n_layer = gguf_int(m, bytes, "clip.audio.block_count")
        enc.n_ff = gguf_int(m, bytes, "clip.audio.feed_forward_length")
        enc.d_head = enc.d_model / enc.n_head
        enc.n_bins = G4A_N_FFT / 2l + 1l
        let d = enc.d_model
        let ff = enc.n_ff

        // subsample channel counts from the conv tensor sizes ([3,3,cin,cout])
        let i0 = gguf_find_tensor(m, "a.conv1d.0.weight")
        let i1 = gguf_find_tensor(m, "a.conv1d.1.weight")
        if (i0 < 0 || i1 < 0) {
            panic("dasLLAMA gemma4a: missing a.conv1d.0/1.weight")
        }
        enc.sub_ch0 = m.tensors[i0].n_elem / 9l            // cin = 1
        enc.sub_ch1 = m.tensors[i1].n_elem / (9l * enc.sub_ch0)
        // proj_dim from the audio multimodal embedder mm.a.input_projection [proj_dim × proj_dim]
        let ip = gguf_find_tensor(m, "mm.a.input_projection.weight")
        if (ip < 0) {
            panic("dasLLAMA gemma4a: missing mm.a.input_projection.weight")
        }
        let op = gguf_find_tensor(m, "a.pre_encode.out.weight")
        if (op < 0) {
            panic("dasLLAMA gemma4a: missing a.pre_encode.out.weight")
        }
        enc.proj_dim = m.tensors[op].n_elem / d            // out proj [d × proj_dim]

        // blob layout: subsample + output first, then per-layer
        var o = 0l
        enc.c0_w = o; o += enc.sub_ch0 * 9l
        enc.norm0_w = o; o += enc.sub_ch0
        enc.c1_w = o; o += enc.sub_ch1 * enc.sub_ch0 * 9l
        enc.norm1_w = o; o += enc.sub_ch1
        let feat = enc.sub_ch1 * (enc.n_mel / 4l)          // 32 × 32 = 1024
        enc.inproj_w = o; o += d * feat
        enc.outproj_w = o; o += enc.proj_dim * d
        enc.outproj_b = o; o += enc.proj_dim
        enc.mmproj_w = o; o += enc.proj_dim * enc.proj_dim
        enc.layers |> resize(int(enc.n_layer))
        for (l in range64(enc.n_layer)) {
            var lo = G4aLayer()
            lo.ff_norm = o; o += d
            lo.ff_up = o; o += ff * d
            lo.ff_down = o; o += d * ff
            lo.ff_post_norm = o; o += d
            lo.attn_pre_norm = o; o += d
            lo.q = o; o += d * d
            lo.k = o; o += d * d
            lo.v = o; o += d * d
            lo.o = o; o += d * d
            lo.per_dim_scale = o; o += enc.d_head
            lo.attn_k_rel = o; o += d * d
            lo.attn_post_norm = o; o += d
            lo.norm_conv = o; o += d
            lo.conv_pw1 = o; o += 2l * d * d
            lo.conv_dw = o; o += 5l * d
            lo.conv_norm = o; o += d
            lo.conv_pw2 = o; o += d * d
            lo.ff_norm_1 = o; o += d
            lo.ff_up_1 = o; o += ff * d
            lo.ff_down_1 = o; o += d * ff
            lo.ff_post_norm_1 = o; o += d
            lo.ln2 = o; o += d
            enc.layers[l] = lo
        }
        enc.blob |> reserve(int(o))
        enc.blob |> resize(int(o))

        // read weights
        gguf_read_tensor_f32(m, bytes, "a.conv1d.0.weight", enc.blob, enc.c0_w, enc.sub_ch0 * 9l)
        gguf_read_tensor_f32(m, bytes, "a.conv1d.0.norm.weight", enc.blob, enc.norm0_w, enc.sub_ch0)
        gguf_read_tensor_f32(m, bytes, "a.conv1d.1.weight", enc.blob, enc.c1_w, enc.sub_ch1 * enc.sub_ch0 * 9l)
        gguf_read_tensor_f32(m, bytes, "a.conv1d.1.norm.weight", enc.blob, enc.norm1_w, enc.sub_ch1)
        gguf_read_tensor_f32(m, bytes, "a.input_projection.weight", enc.blob, enc.inproj_w, d * feat)
        gguf_read_tensor_f32(m, bytes, "a.pre_encode.out.weight", enc.blob, enc.outproj_w, enc.proj_dim * d)
        gguf_read_tensor_f32(m, bytes, "a.pre_encode.out.bias", enc.blob, enc.outproj_b, enc.proj_dim)
        gguf_read_tensor_f32(m, bytes, "mm.a.input_projection.weight", enc.blob, enc.mmproj_w, enc.proj_dim * enc.proj_dim)
        for (l in range64(enc.n_layer)) {
            let lo = enc.layers[l]
            let p = "a.blk.{l}"
            gguf_read_tensor_f32(m, bytes, "{p}.ffn_norm.weight", enc.blob, lo.ff_norm, d)
            gguf_read_tensor_f32(m, bytes, "{p}.ffn_up.weight", enc.blob, lo.ff_up, ff * d)
            gguf_read_tensor_f32(m, bytes, "{p}.ffn_down.weight", enc.blob, lo.ff_down, d * ff)
            gguf_read_tensor_f32(m, bytes, "{p}.ffn_post_norm.weight", enc.blob, lo.ff_post_norm, d)
            gguf_read_tensor_f32(m, bytes, "{p}.attn_pre_norm.weight", enc.blob, lo.attn_pre_norm, d)
            gguf_read_tensor_f32(m, bytes, "{p}.attn_q.weight", enc.blob, lo.q, d * d)
            gguf_read_tensor_f32(m, bytes, "{p}.attn_k.weight", enc.blob, lo.k, d * d)
            gguf_read_tensor_f32(m, bytes, "{p}.attn_v.weight", enc.blob, lo.v, d * d)
            gguf_read_tensor_f32(m, bytes, "{p}.attn_out.weight", enc.blob, lo.o, d * d)
            gguf_read_tensor_f32(m, bytes, "{p}.per_dim_scale.weight", enc.blob, lo.per_dim_scale, enc.d_head)
            gguf_read_tensor_f32(m, bytes, "{p}.attn_k_rel.weight", enc.blob, lo.attn_k_rel, d * d)
            gguf_read_tensor_f32(m, bytes, "{p}.attn_post_norm.weight", enc.blob, lo.attn_post_norm, d)
            // 🔑 conv_norm / norm_conv are name-swapped in the GGUF (upstream tensor_mapping.py):
            // the PRE-conv RMSNorm reads the tensor named "conv_norm", the POST-depthwise one "norm_conv"
            gguf_read_tensor_f32(m, bytes, "{p}.conv_norm.weight", enc.blob, lo.norm_conv, d)
            gguf_read_tensor_f32(m, bytes, "{p}.conv_pw1.weight", enc.blob, lo.conv_pw1, 2l * d * d)
            gguf_read_tensor_f32(m, bytes, "{p}.conv_dw.weight", enc.blob, lo.conv_dw, 5l * d)
            gguf_read_tensor_f32(m, bytes, "{p}.norm_conv.weight", enc.blob, lo.conv_norm, d)
            gguf_read_tensor_f32(m, bytes, "{p}.conv_pw2.weight", enc.blob, lo.conv_pw2, d * d)
            gguf_read_tensor_f32(m, bytes, "{p}.ffn_norm_1.weight", enc.blob, lo.ff_norm_1, d)
            gguf_read_tensor_f32(m, bytes, "{p}.ffn_up_1.weight", enc.blob, lo.ff_up_1, ff * d)
            gguf_read_tensor_f32(m, bytes, "{p}.ffn_down_1.weight", enc.blob, lo.ff_down_1, d * ff)
            gguf_read_tensor_f32(m, bytes, "{p}.ffn_post_norm_1.weight", enc.blob, lo.ff_post_norm_1, d)
            gguf_read_tensor_f32(m, bytes, "{p}.ln2.weight", enc.blob, lo.ln2, d)
            // clamp ranges of the clamped matmuls
            var lc = enc.layers[l]
            lc.c_ff_up = g4a_read_clamp(m, bytes, "{p}.ffn_up")
            lc.c_ff_down = g4a_read_clamp(m, bytes, "{p}.ffn_down")
            lc.c_q = g4a_read_clamp(m, bytes, "{p}.attn_q")
            lc.c_k = g4a_read_clamp(m, bytes, "{p}.attn_k")
            lc.c_v = g4a_read_clamp(m, bytes, "{p}.attn_v")
            lc.c_o = g4a_read_clamp(m, bytes, "{p}.attn_out")
            lc.c_pw1 = g4a_read_clamp(m, bytes, "{p}.conv_pw1")
            lc.c_pw2 = g4a_read_clamp(m, bytes, "{p}.conv_pw2")
            lc.c_ff_up_1 = g4a_read_clamp(m, bytes, "{p}.ffn_up_1")
            lc.c_ff_down_1 = g4a_read_clamp(m, bytes, "{p}.ffn_down_1")
            enc.layers[l] = lc
        }
    }
    fclose(f)

    // HTK filterbank, 512 Hann (320 at the front, zeros to 512), DFT twiddles, RPE table, ones
    enc.filters <- g4a_htk_filterbank(enc.n_mel, G4A_N_FFT, AUDIO_SAMPLE_RATE)
    enc.hann |> resize(int(G4A_N_FFT))
    for (i in range64(G4A_WINDOW)) {
        enc.hann[i] = float(0.5lf - 0.5lf * double(cos(float(2.0lf * DBL_PI * double(i) / double(G4A_WINDOW)))))
    }
    enc.twiddles <- build_dft_twiddles(G4A_N_FFT)
    let num_ts = enc.d_model / 2l
    let log_ts_inc = log(10000.0) / float(max(num_ts - 1l, 1l))
    enc.pos_emb |> resize(int((G4A_PAST + 1l) * enc.d_model))
    for (pp in range64(G4A_PAST + 1l)) {
        let position = float(G4A_PAST - pp)
        for (i in range64(num_ts)) {
            let inv_ts = exp(-float(i) * log_ts_inc)
            let scaled = position * inv_ts
            enc.pos_emb[pp * enc.d_model + i] = sin(scaled)
            enc.pos_emb[pp * enc.d_model + i + num_ts] = cos(scaled)
        }
    }
    enc.ones |> resize(int(enc.proj_dim))
    for (i in range64(enc.proj_dim)) {
        enc.ones[i] = 1.0
    }
    return <- enc
}

// ===== mel (gemma4a: no preemphasis, natural-log magnitude, HTK, no per-feature norm) =====

//! gemma4a log-mel for one ≤30 s chunk: semicausal left-pad (window/2) + right-pad to the PyTorch
//! frame count, 512 Hann (320-front), magnitude spectrum, HTK mel dot, natural log(sum, floor
//! 0.001). Frame-major ``[n_len × n_mel]``; ``n_len`` trimmed to the PyTorch frame count.
def gemma4a_log_mel(enc : Gemma4aEncoder; samples : array<float> | #; off, len : int64; var st : Gemma4aState) {
    let n_mel = enc.n_mel
    let n_bins = enc.n_bins
    let pad_left = G4A_WINDOW / 2l
    let n_with_left = len + pad_left
    let pt_frames = max((n_with_left - (G4A_WINDOW + 1l)) / G4A_HOP + 1l, 1l)
    let n_padded_needed = (pt_frames - 1l) * G4A_HOP + G4A_N_FFT
    let total_pad = max(n_padded_needed - len, pad_left)
    let n_padded = total_pad + len
    g4a_ensure(st.padded, n_padded)
    for (i in range64(n_padded)) {
        st.padded[i] = 0.0
    }
    for (i in range64(len)) {
        st.padded[pad_left + i] = samples[off + i]
    }

    let n_full = (n_padded - G4A_N_FFT) / G4A_HOP + 1l
    st.n_len = min(n_full, pt_frames)
    let n_len = st.n_len
    g4a_ensure(st.mel, n_len * n_mel)

    // windowed frames -> DFT (twiddle gemm) -> magnitude -> mel -> natural log
    g4a_ensure(st.xw, n_len * G4A_N_FFT)
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
    g4a_ensure(st.reim, n_len * n_bins * 2l)
    gemm_f32(unsafe(addr(st.reim[0])), unsafe(addr(st.xw[0])), unsafe(addr(enc.twiddles[0])),
             n_len, G4A_N_FFT, n_bins * 2l)

    g4a_ensure(st.power, n_bins)
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
    g4a_ensure(st.col, rows * kdim)
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
    g4a_ensure(out, rows * cout)
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

// clamp x[0..n) in place to [imin, imax]
def private g4a_clamp(var x : array<float>; n : int64; lo, hi : float) {
    for (i in range64(n)) {
        x[i] = clamp(x[i], lo, hi)
    }
}

// clamped matmul: y = W · clamp(x); then clamp(y). x is [npos × n], y is [npos × d].
def private g4a_mm_clamped(enc : Gemma4aEncoder; var st : Gemma4aState; var y : array<float>;
                           w_off : int64; x : array<float>; n, d, npos : int64; c : Clamp) {
    if (!c.active) {
        mm_blob_b(y, enc.blob, w_off, x, n, d, npos)
        return
    }
    g4a_ensure(st.xc, n * npos)
    for (i in range64(n * npos)) {
        st.xc[i] = clamp(x[i], c.imin, c.imax)
    }
    mm_blob_b(y, enc.blob, w_off, st.xc, n, d, npos)
    g4a_clamp(y, d * npos, c.omin, c.omax)
}

// RMSNorm each d-wide row of src into dst against a blob weight (weight-multiply, gemma4a eps).
def private g4a_rms_rows(enc : Gemma4aEncoder; var dst : array<float>; src : array<float>;
                         w_off, d, npos : int64) {
    unsafe {
        var dp = addr(dst[0])
        let sp = addr<float const?>(src[0])
        let wp = addr<float const?>(enc.blob[w_off])
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
def gemma4a_encode(enc : Gemma4aEncoder; var st : Gemma4aState) : int64 {
    let d = enc.d_model
    let ff = enc.n_ff
    let n_head = enc.n_head
    let dh = enc.d_head
    let n_mel = enc.n_mel

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
    g4a_ensure(st.x, npos * d)
    mm_blob_b(st.x, enc.blob, enc.inproj_w, st.y1, feat, d, npos)

    g4a_ensure(st.xb, npos * d)
    g4a_ensure(st.xc, max(2l * d, ff) * npos)
    g4a_ensure(st.h, npos * ff)
    g4a_ensure(st.glu, npos * d)
    g4a_ensure(st.cvd, npos * d)
    g4a_ensure(st.qq, npos * d)
    g4a_ensure(st.kk, npos * d)
    g4a_ensure(st.vv, npos * d)
    g4a_ensure(st.ao, npos * d)
    g4a_ensure(st.prpe, (G4A_PAST + 1l) * d)
    g4a_ensure(st.sc, G4A_PAST + 1l)

    let q_scale = (1.0 / sqrt(float(dh))) / log(2.0)
    let k_scale = log(1.0 + exp(1.0)) / log(2.0)
    let inv_softcap = 1.0 / G4A_SOFTCAP

    for (l in range64(enc.n_layer)) {
        let lo = enc.layers[l]

        // FFN1 (macaron half)
        g4a_ffn(enc, st, lo.ff_norm, lo.ff_up, lo.ff_down, lo.ff_post_norm, lo.c_ff_up, lo.c_ff_down, d, ff, npos)

        // self-attention (causal sliding window 12, sinusoidal RPE, softcap 50)
        g4a_rms_rows(enc, st.xb, st.x, lo.attn_pre_norm, d, npos)
        g4a_mm_clamped(enc, st, st.qq, lo.q, st.xb, d, d, npos, lo.c_q)
        g4a_mm_clamped(enc, st, st.kk, lo.k, st.xb, d, d, npos, lo.c_k)
        g4a_mm_clamped(enc, st, st.vv, lo.v, st.xb, d, d, npos, lo.c_v)
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
        // project the RPE table: prpe [13 × d]
        mm_blob_b(st.prpe, enc.blob, lo.attn_k_rel, enc.pos_emb, d, d, G4A_PAST + 1l)
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
        g4a_mm_clamped(enc, st, st.xb, lo.o, st.ao, d, d, npos, lo.c_o)
        g4a_rms_rows(enc, st.xb, st.xb, lo.attn_post_norm, d, npos)
        add_inplace_rows(st.x, st.xb, npos * d)

        // conv module: RMSNorm -> pw1 (clamp) -> GLU -> causal depthwise k=5 -> RMSNorm -> SiLU -> pw2 (clamp)
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

        // FFN2 (macaron half)
        g4a_ffn(enc, st, lo.ff_norm_1, lo.ff_up_1, lo.ff_down_1, lo.ff_post_norm_1, lo.c_ff_up_1, lo.c_ff_down_1, d, ff, npos)

        // per-layer output norm
        g4a_rms_rows(enc, st.x, st.x, lo.ln2, d, npos)
    }

    // output projection -> RMSNorm (weightless) -> mm input projection
    g4a_ensure(st.tmp, npos * enc.proj_dim)
    g4a_ensure(st.out, npos * enc.proj_dim)
    mm_blob_b(st.tmp, enc.blob, enc.outproj_w, st.x, d, enc.proj_dim, npos)
    add_bias_rows(st.tmp, enc.blob, enc.outproj_b, enc.proj_dim, npos)
    unsafe {
        var tp = addr(st.tmp[0])
        let wp = addr<float const?>(enc.ones[0])
        for (r in range64(npos)) {
            rmsnorm(tp + r * enc.proj_dim, tp + r * enc.proj_dim, wp, enc.proj_dim, enc.eps)
        }
    }
    mm_blob_b(st.out, enc.blob, enc.mmproj_w, st.tmp, enc.proj_dim, enc.proj_dim, npos)
    return npos
}

//! Soft tokens for the whole clip: encode every ≤30 s chunk and append its rows (``proj_dim``
//! wide) to ``rows``. Returns the total token count.
def gemma4a_encode_all(enc : Gemma4aEncoder; var st : Gemma4aState; samples : array<float> | #;
                       var rows : array<float>) : int64 {
    let n_in = int64(length(samples))
    let chunk = 30l * AUDIO_SAMPLE_RATE
    var total = 0l
    var off = 0l
    while (off < n_in) {
        let len = min(chunk, n_in - off)
        gemma4a_log_mel(enc, samples, off, len, st)
        let n = gemma4a_encode(enc, st)
        rows |> reserve(int(int64(length(rows)) + n * enc.proj_dim))
        for (i in range64(n * enc.proj_dim)) {
            rows |> push(st.out[i])
        }
        total += n
        off += chunk
    }
    return total
}
