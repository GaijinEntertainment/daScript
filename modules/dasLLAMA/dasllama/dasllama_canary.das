options gen2
options _comment_hygiene = true

module dasllama_canary shared public

require math
require daslib/fio
require strings
require daslib/strings_boost
require daslib/array_boost
require dasllama/dasllama_math
require dasllama/dasllama_gguf
require dasllama/dasllama_audio

// Canary-Qwen 2.5B audio front end: the FastConformer-32 encoder + the perception.proj
// linear (audio d_model 1024 -> Qwen3 hidden 2048), read from a self-describing named-tensor
// f32 bin (harness/convert_canary_to_ggml.py output). Architecture = NVIDIA FastConformer
// (parakeet's conformer neighborhood) PLUS the qkv/ffn/conv biases parakeet lacks; the TDT
// predictor/joint are dropped. fp32, correctness-first — the encode runs once per clip and is
// not the perf gate; oracle = NeMo SALM greedy (harness/canary_qwen_expected.tsv).

let CANARY_MAGIC = 0x59524e43u   // "CNRY"
let private CN_PREEMPH = 0.97
let private CN_HOP = 160l
let private CN_MEL_FLOOR = 5.960464477539063e-08lf   // 2^-24 (NeMo log_zero_guard)

//! Per-layer weight offsets inside ``blob`` (biases carried on every projection — the
//! parakeet delta). ``bn_w``/``bn_b`` become fold scale/shift at load (BN + depthwise bias).
struct private CanaryLayerOffs {
    nf1_w : int64; nf1_b : int64
    ff1a_w : int64; ff1a_b : int64; ff1b_w : int64; ff1b_b : int64
    nat_w : int64; nat_b : int64
    q_w : int64; q_b : int64; k_w : int64; k_b : int64
    v_w : int64; v_b : int64; o_w : int64; o_b : int64
    pos_w : int64; bias_u : int64; bias_v : int64
    ncv_w : int64; ncv_b : int64
    pw1_w : int64; pw1_b : int64
    dw_w : int64; dw_b : int64
    bn_w : int64; bn_b : int64; bn_mean : int64; bn_var : int64
    pw2_w : int64; pw2_b : int64
    nf2_w : int64; nf2_b : int64
    ff2a_w : int64; ff2a_b : int64; ff2b_w : int64; ff2b_b : int64
    nout_w : int64; nout_b : int64
}

//! Loaded canary encoder: hparams, mel filterbank + STFT window from the bin, the FastConformer
//! weights (all fp32 in one ``blob``), and the projector into the decoder embedding space.
struct CanaryEncoder {
    d_model : int64
    n_layer : int64
    n_head : int64
    n_ff : int64
    n_mel : int64
    n_fft : int64
    subsampling : int64
    n_sub_ch : int64
    conv_kernel : int64
    proj_dim : int64
    n_fb : int64
    eps : float = 1e-5
    filters : array<float>    // [n_mel × n_fb]
    window : array<float>
    twiddles : array<float>   // DFT twiddle matrix [n_fft × (n_fb·2)]
    blob : array<float>
    // pre-encode subsampling stack
    c0_w : int64; c0_b : int64
    c2_w : int64; c2_b : int64
    c3_w : int64; c3_b : int64
    c5_w : int64; c5_b : int64
    c6_w : int64; c6_b : int64
    pre_w : int64; pre_b : int64
    layers : array<CanaryLayerOffs>
    proj_w : int64; proj_b : int64
}

//! Every hot-path buffer of the canary encode, so a transcribe allocates nothing per call.
struct CanaryState {
    mel : array<float>        // frame-major [n_len × n_mel]
    n_len : int64
    n_valid : int64           // real mel frame count (n_in / hop) — drives the conv length mask
    // mel scratch
    pre : array<float>
    padded : array<float>
    xw : array<float>
    reim : array<float>
    power : array<float>
    // conv scratch
    col : array<float>
    y0 : array<float>
    y2 : array<float>
    y3 : array<float>
    y5 : array<float>
    y6 : array<float>
    xf : array<float>
    // encoder
    x : array<float>
    xb : array<float>
    h : array<float>
    glu : array<float>
    ao : array<float>
    qq : array<float>
    kk : array<float>
    vv : array<float>
    pos_emb : array<float>
    posl : array<float>
    qu : array<float>
    qv : array<float>
    kt : array<float>
    vh : array<float>
    posh : array<float>
    sc : array<float>
    rl : array<float>
    oh : array<float>
    out : array<float>        // projected soft tokens [tt × proj_dim]
}

def private cn_ensure(var a : array<float>; n : int64) {
    if (int64(length(a)) < n) {
        a |> reserve(int(n))
        a |> resize(int(n))
    }
}

// ===== bin container =====

struct private CnRec {
    name : string
    file_off : int64
    n_elem : int64
}

def private cn_bytes_str(b : array<uint8> | #; o, n : int64) : string {
    if (n <= 0l) {
        return ""
    }
    var s = ""
    unsafe {
        s = string(temp_array(addr<uint8?>(b[o]), n, type<uint8>))
    }
    return s
}

def private cn_rd_i64(b : array<uint8> | #; o : int64) : int64 {
    return int64(rd_u32(b, o)) | (int64(rd_u32(b, o + 4l)) << 32l)
}

def private cn_read_rec(b : array<uint8> | #; rec : CnRec; var blob : array<float>; o : int64) {
    for (i in range64(rec.n_elem)) {
        blob[o + i] = rd_f32(b, rec.file_off + i * 4l)
    }
}

def private cn_off(offs : table<string; int64>; name : string) : int64 {
    if (!key_exists(offs, name)) {
        panic("dasLLAMA canary: missing tensor '{name}' in encoder bin")
    }
    return offs?[name] ?? 0l
}

// BN inference fold, with the depthwise conv bias absorbed: scale = γ/sqrt(σ²+eps),
// shift = β + (b_dw − μ)·scale. bn_w/bn_b slots take scale/shift; mean/var stay as read.
def private cn_fold_bn(var blob : array<float>; w, b, mean, vr, dwb, d : int64; eps : float) {
    for (j in range64(d)) {
        let s = blob[w + j] / sqrt(blob[vr + j] + eps)
        blob[w + j] = s
        blob[b + j] += (blob[dwb + j] - blob[mean + j]) * s
    }
}

//! Load a canary encoder bin (convert_canary_to_ggml.py output; magic "CNRY", all-f32).
def load_canary_encoder(path : string) : CanaryEncoder {
    var m = CanaryEncoder()
    let n_bytes = int64(stat(path).size)
    let f = fopen(path, "rb")
    if (f == null) {
        panic("dasLLAMA canary: cannot open encoder '{path}'")
    }
    fmap(f) $(var bytes : array<uint8>#) {
        var o = 0l
        if (rd_u32(bytes, o) != CANARY_MAGIC) {
            panic("dasLLAMA canary: bad magic in '{path}'")
        }
        o += 4l
        o += 4l   // version
        m.d_model = int64(rd_i32(bytes, o)); o += 4l
        m.n_layer = int64(rd_i32(bytes, o)); o += 4l
        m.n_head = int64(rd_i32(bytes, o)); o += 4l
        m.n_ff = int64(rd_i32(bytes, o)); o += 4l
        m.n_mel = int64(rd_i32(bytes, o)); o += 4l
        m.n_fft = int64(rd_i32(bytes, o)); o += 4l
        m.subsampling = int64(rd_i32(bytes, o)); o += 4l
        m.n_sub_ch = int64(rd_i32(bytes, o)); o += 4l
        m.conv_kernel = int64(rd_i32(bytes, o)); o += 4l
        m.proj_dim = int64(rd_i32(bytes, o)); o += 4l
        m.eps = rd_f32(bytes, o); o += 4l
        m.n_fb = int64(rd_i32(bytes, o)); o += 4l
        // mel filterbank [n_mel × n_fb]
        let nfilt = m.n_mel * m.n_fb
        m.filters |> resize(int(nfilt))
        for (i in range64(nfilt)) {
            m.filters[i] = rd_f32(bytes, o + i * 4l)
        }
        o += nfilt * 4l
        // STFT window
        let n_window = int64(rd_i32(bytes, o)); o += 4l
        m.window |> resize(int(n_window))
        for (i in range64(n_window)) {
            m.window[i] = rd_f32(bytes, o + i * 4l)
        }
        o += n_window * 4l

        // named tensor records (name_len i32, name, n_elem i64, f32 data)
        var recs : array<CnRec>
        while (o + 4l <= n_bytes) {
            let name_len = int64(rd_i32(bytes, o)); o += 4l
            let name = cn_bytes_str(bytes, o, name_len); o += name_len
            let n_elem = cn_rd_i64(bytes, o); o += 8l
            recs |> emplace(CnRec(name = name, file_off = o, n_elem = n_elem))
            o += n_elem * 4l
        }

        // place every tensor into one blob, recording each offset
        var total = 0l
        for (r in recs) {
            total += r.n_elem
        }
        m.blob |> reserve(total)
        m.blob |> resize(int(total))
        var offs : table<string; int64>
        var bo = 0l
        for (r in recs) {
            cn_read_rec(bytes, r, m.blob, bo)
            offs[r.name] = bo
            bo += r.n_elem
        }
        delete recs

        // pre-encode + projector offsets
        m.c0_w = cn_off(offs, "pre_encode.conv.0.weight"); m.c0_b = cn_off(offs, "pre_encode.conv.0.bias")
        m.c2_w = cn_off(offs, "pre_encode.conv.2.weight"); m.c2_b = cn_off(offs, "pre_encode.conv.2.bias")
        m.c3_w = cn_off(offs, "pre_encode.conv.3.weight"); m.c3_b = cn_off(offs, "pre_encode.conv.3.bias")
        m.c5_w = cn_off(offs, "pre_encode.conv.5.weight"); m.c5_b = cn_off(offs, "pre_encode.conv.5.bias")
        m.c6_w = cn_off(offs, "pre_encode.conv.6.weight"); m.c6_b = cn_off(offs, "pre_encode.conv.6.bias")
        m.pre_w = cn_off(offs, "pre_encode.out.weight"); m.pre_b = cn_off(offs, "pre_encode.out.bias")
        m.proj_w = cn_off(offs, "proj.weight"); m.proj_b = cn_off(offs, "proj.bias")

        m.layers |> resize(int(m.n_layer))
        for (l in range64(m.n_layer)) {
            let lo = CanaryLayerOffs(
                nf1_w = cn_off(offs, "L{l}.norm_ff1.weight"), nf1_b = cn_off(offs, "L{l}.norm_ff1.bias"),
                ff1a_w = cn_off(offs, "L{l}.ff1.linear1.weight"), ff1a_b = cn_off(offs, "L{l}.ff1.linear1.bias"),
                ff1b_w = cn_off(offs, "L{l}.ff1.linear2.weight"), ff1b_b = cn_off(offs, "L{l}.ff1.linear2.bias"),
                nat_w = cn_off(offs, "L{l}.norm_att.weight"), nat_b = cn_off(offs, "L{l}.norm_att.bias"),
                q_w = cn_off(offs, "L{l}.attn.q.weight"), q_b = cn_off(offs, "L{l}.attn.q.bias"),
                k_w = cn_off(offs, "L{l}.attn.k.weight"), k_b = cn_off(offs, "L{l}.attn.k.bias"),
                v_w = cn_off(offs, "L{l}.attn.v.weight"), v_b = cn_off(offs, "L{l}.attn.v.bias"),
                o_w = cn_off(offs, "L{l}.attn.out.weight"), o_b = cn_off(offs, "L{l}.attn.out.bias"),
                pos_w = cn_off(offs, "L{l}.attn.pos.weight"),
                bias_u = cn_off(offs, "L{l}.attn.pos_bias_u"), bias_v = cn_off(offs, "L{l}.attn.pos_bias_v"),
                ncv_w = cn_off(offs, "L{l}.norm_conv.weight"), ncv_b = cn_off(offs, "L{l}.norm_conv.bias"),
                pw1_w = cn_off(offs, "L{l}.conv.pw1.weight"), pw1_b = cn_off(offs, "L{l}.conv.pw1.bias"),
                dw_w = cn_off(offs, "L{l}.conv.dw.weight"), dw_b = cn_off(offs, "L{l}.conv.dw.bias"),
                bn_w = cn_off(offs, "L{l}.conv.bn.weight"), bn_b = cn_off(offs, "L{l}.conv.bn.bias"),
                bn_mean = cn_off(offs, "L{l}.conv.bn.running_mean"), bn_var = cn_off(offs, "L{l}.conv.bn.running_var"),
                pw2_w = cn_off(offs, "L{l}.conv.pw2.weight"), pw2_b = cn_off(offs, "L{l}.conv.pw2.bias"),
                nf2_w = cn_off(offs, "L{l}.norm_ff2.weight"), nf2_b = cn_off(offs, "L{l}.norm_ff2.bias"),
                ff2a_w = cn_off(offs, "L{l}.ff2.linear1.weight"), ff2a_b = cn_off(offs, "L{l}.ff2.linear1.bias"),
                ff2b_w = cn_off(offs, "L{l}.ff2.linear2.weight"), ff2b_b = cn_off(offs, "L{l}.ff2.linear2.bias"),
                nout_w = cn_off(offs, "L{l}.norm_out.weight"), nout_b = cn_off(offs, "L{l}.norm_out.bias"))
            // fold BN (with the depthwise bias) into bn_w/bn_b at load
            cn_fold_bn(m.blob, lo.bn_w, lo.bn_b, lo.bn_mean, lo.bn_var, lo.dw_b, m.d_model, m.eps)
            m.layers[l] = lo
        }
    }
    fclose(f)
    m.twiddles <- build_dft_twiddles(m.n_fft)
    return <- m
}

// ===== mel (frame-major, parakeet flavor) =====

//! Canary's log-mel: preemphasis 0.97, zero center-pad n_fft/2, STFT window, power spectra, mel dot,
//! log(sum + 2⁻²⁴), then per-feature mean/std norm over valid frames (Bessel n−1, std + 1e-5);
//! frames past ``valid`` are zeroed (NeMo's seq-len mask). FRAME-major ``[n_len × n_mel]``.
def canary_log_mel(m : CanaryEncoder; samples : array<float> | #; var st : CanaryState) {
    let n_in = int64(length(samples))
    cn_ensure(st.pre, n_in)
    for (i in range(int(n_in))) {
        st.pre[i] = samples[i]
    }
    var pi = n_in - 1l
    while (pi > 0l) {   // preemph backward in place: x[i] -= 0.97·x[i−1]
        st.pre[pi] -= CN_PREEMPH * st.pre[pi - 1l]
        pi--
    }
    let pad = m.n_fft / 2l
    let n_padded = n_in + 2l * pad
    cn_ensure(st.padded, n_padded)
    for (i in range(int(pad))) {
        st.padded[i] = 0.0
    }
    for (i in range(int(n_in))) {
        st.padded[int(pad) + i] = st.pre[i]
    }
    for (i in range64(n_in + pad, n_padded)) {
        st.padded[i] = 0.0
    }

    let n_fft = m.n_fft
    let n_bins = m.n_fb
    let n_mel = m.n_mel
    st.n_len = (n_padded - n_fft) / CN_HOP + 1l
    let n_len = st.n_len
    cn_ensure(st.mel, n_len * n_mel)
    let wsz = int64(length(m.window))
    let wpad = (n_fft - wsz) / 2l

    // windowed frames -> DFT via twiddle gemm -> power
    cn_ensure(st.xw, n_len * n_fft)
    for (i in range64(n_len * n_fft)) {
        st.xw[i] = 0.0
    }
    for (i in range64(n_len)) {
        let off = i * CN_HOP
        let n_do = min(wsz, n_padded - off)
        for (j in range64(n_do)) {
            if (off + wpad + j < n_padded) {
                st.xw[i * n_fft + wpad + j] = m.window[j] * st.padded[off + wpad + j]
            }
        }
    }
    cn_ensure(st.reim, n_len * n_bins * 2l)
    gemm_f32(unsafe(addr(st.reim[0])), unsafe(addr(st.xw[0])), unsafe(addr(m.twiddles[0])),
             n_len, n_fft, n_bins * 2l)

    cn_ensure(st.power, n_bins)
    for (i in range64(n_len)) {
        for (k in range64(n_bins)) {
            let re = st.reim[i * n_bins * 2l + k * 2l]
            let im = st.reim[i * n_bins * 2l + k * 2l + 1l]
            st.power[k] = re * re + im * im
        }
        for (jm in range64(n_mel)) {
            var sum = 0.0lf
            for (k in range64(n_bins)) {
                sum += double(st.power[k] * m.filters[jm * n_bins + k])
            }
            st.mel[i * n_mel + jm] = float(log(sum + CN_MEL_FLOOR))
        }
    }

    // per-feature normalization over the valid frames, applied to all; tail masked to 0
    let valid = n_in / CN_HOP
    st.n_valid = valid
    for (j in range64(n_mel)) {
        var sum = 0.0lf
        for (i in range64(valid)) {
            sum += double(st.mel[i * n_mel + j])
        }
        let mean = sum / double(max(valid, 1l))
        var sq = 0.0lf
        for (i in range64(valid)) {
            let dv = double(st.mel[i * n_mel + j]) - mean
            sq += dv * dv
        }
        let denom = sqrt(sq / max(double(valid) - 1.0lf, 1.0lf)) + 1e-5lf
        for (i in range64(n_len)) {
            st.mel[i * n_mel + j] = float((double(st.mel[i * n_mel + j]) - mean) / denom)
        }
    }
    for (i in range64(valid, n_len)) {   // NeMo masks frames past seq_len to zero
        for (j in range64(n_mel)) {
            st.mel[i * n_mel + j] = 0.0
        }
    }
}

// ===== conv subsampling =====

def private cn_relu_rows(var y : array<float>; n : int64) {
    for (i in range64(n)) {
        y[i] = max(y[i], 0.0)
    }
}

// NeMo's MaskedConvSequential zeroes time-rows past the valid length before each conv; here
// that lands as: zero rows [from_row, n_rows) of a [n_rows × row_stride] image.
def private cn_mask_rows(var a : array<float>; from_row, n_rows, row_stride : int64) {
    for (i in range64(from_row * row_stride, n_rows * row_stride)) {
        a[i] = 0.0
    }
}

// output valid length of a k3/s2/p1 conv stage
def private cn_conv_out_len(l : int64) : int64 => (l - 1l) / 2l + 1l

// regular conv2d k3 s2 p1, cin=1: im2col (9 taps, guarded) then mm + bias + relu
def private cn_conv0(m : CanaryEncoder; var st : CanaryState; ih, iw : int64) {
    let cout = m.n_sub_ch
    let ow = (iw - 1l) / 2l + 1l
    let oh = (ih - 1l) / 2l + 1l
    let rows = oh * ow
    cn_ensure(st.col, rows * 9l)
    for (oy in range64(oh)) {
        for (ox in range64(ow)) {
            let r = (oy * ow + ox) * 9l
            for (kh in range64(3l)) {
                let iy = oy * 2l - 1l + kh
                for (kw in range64(3l)) {
                    let ix = ox * 2l - 1l + kw
                    st.col[r + kh * 3l + kw] = (iy >= 0l && iy < ih && ix >= 0l && ix < iw
                        ? st.mel[iy * iw + ix] : 0.0)
                }
            }
        }
    }
    cn_ensure(st.y0, rows * cout)
    mm_blob_b(st.y0, m.blob, m.c0_w, st.col, 9l, cout, rows)
    add_bias_rows(st.y0, m.blob, m.c0_b, cout, rows)
    cn_relu_rows(st.y0, rows * cout)
}

// depthwise conv2d k3 s2 p1: per-channel 3x3, + bias, NO relu. w [ch × 9] (kh·3+kw).
def private cn_conv_dw(m : CanaryEncoder; var out : array<float>; x : array<float>;
                       w_off, b_off : int64; iw, ih : int64) {
    let ch = m.n_sub_ch
    let ow = (iw - 1l) / 2l + 1l
    let oh = (ih - 1l) / 2l + 1l
    cn_ensure(out, oh * ow * ch)
    for (oy in range64(oh)) {
        for (ox in range64(ow)) {
            let obase = (oy * ow + ox) * ch
            for (c in range64(ch)) {
                var acc = m.blob[b_off + c]
                for (kh in range64(3l)) {
                    let iy = oy * 2l - 1l + kh
                    if (iy >= 0l && iy < ih) {
                        for (kw in range64(3l)) {
                            let ix = ox * 2l - 1l + kw
                            if (ix >= 0l && ix < iw) {
                                acc += m.blob[w_off + c * 9l + kh * 3l + kw] * x[(iy * iw + ix) * ch + c]
                            }
                        }
                    }
                }
                out[obase + c] = acc
            }
        }
    }
}

// pointwise 1x1 conv (channel mix) + bias + relu, over rows
def private cn_conv_pw(m : CanaryEncoder; var out : array<float>; x : array<float>;
                       w_off, b_off, rows : int64) {
    let ch = m.n_sub_ch
    cn_ensure(out, rows * ch)
    mm_blob_b(out, m.blob, w_off, x, ch, ch, rows)
    add_bias_rows(out, m.blob, b_off, ch, rows)
    cn_relu_rows(out, rows * ch)
}

// ===== encoder =====

//! FastConformer-32 encode over ``st.mel`` (frame-major) + the perception.proj projector.
//! Leaves the projected soft tokens (``proj_dim`` wide) in ``st.out``; returns the frame count.
def canary_encode(m : CanaryEncoder; var st : CanaryState) : int64 {
    let d = m.d_model
    let ff = m.n_ff
    let nch = m.n_sub_ch
    let n_head = m.n_head
    let dh = d / n_head
    let n_mel = m.n_mel

    // conv subsampling: mel [n_len × n_mel] as image [ih=n_len][iw=n_mel][c=1]
    let t1 = (st.n_len - 1l) / 2l + 1l
    let w1 = (n_mel - 1l) / 2l + 1l
    // valid time lengths propagate through the stride-2 convs (NeMo's length mask)
    let l0 = cn_conv_out_len(st.n_valid)
    let l2 = cn_conv_out_len(l0)
    cn_conv0(m, st, st.n_len, n_mel)                              // -> y0 [t1 × w1 × nch]
    cn_mask_rows(st.y0, l0, t1, w1 * nch)                         // mask before conv2 (depthwise)
    cn_conv_dw(m, st.y2, st.y0, m.c2_w, m.c2_b, w1, t1)          // -> y2 [t2 × w2 × nch]
    let t2 = (t1 - 1l) / 2l + 1l
    let w2 = (w1 - 1l) / 2l + 1l
    cn_conv_pw(m, st.y3, st.y2, m.c3_w, m.c3_b, t2 * w2)         // -> y3
    cn_mask_rows(st.y3, l2, t2, w2 * nch)                        // mask before conv5 (depthwise)
    cn_conv_dw(m, st.y5, st.y3, m.c5_w, m.c5_b, w2, t2)         // -> y5 [t3 × w3 × nch]
    let t3 = (t2 - 1l) / 2l + 1l
    let w3 = (w2 - 1l) / 2l + 1l
    cn_conv_pw(m, st.y6, st.y5, m.c6_w, m.c6_b, t3 * w3)         // -> y6

    let tt = t3
    let feat = w3 * nch
    cn_ensure(st.xf, tt * feat)                                  // xf[t][f + w3·c] = y6[t][f][c]
    for (t in range64(tt)) {
        for (fr in range64(w3)) {
            for (c in range64(nch)) {
                st.xf[t * feat + fr + w3 * c] = st.y6[(t * w3 + fr) * nch + c]
            }
        }
    }
    cn_ensure(st.x, tt * d)
    mm_blob_b(st.x, m.blob, m.pre_w, st.xf, feat, d, tt)
    add_bias_rows(st.x, m.blob, m.pre_b, d, tt)

    // rel-pos embedding table [W × d], interleaved (sin, cos); W = 2T−1
    let ww = 2l * tt - 1l
    cn_ensure(st.pos_emb, ww * d)
    for (w in range64(ww)) {
        let rel = double(tt - 1l - w)
        for (k in range64(d / 2l)) {
            let freq = exp(-(double(k * 2l) * log(10000.0lf) / double(d)))
            let theta = float(freq * rel)
            st.pos_emb[w * d + 2l * k] = sin(theta)
            st.pos_emb[w * d + 2l * k + 1l] = cos(theta)
        }
    }

    cn_ensure(st.xb, tt * d)
    cn_ensure(st.h, tt * ff)
    cn_ensure(st.glu, tt * d)
    cn_ensure(st.ao, tt * d)
    cn_ensure(st.qq, tt * d)
    cn_ensure(st.kk, tt * d)
    cn_ensure(st.vv, tt * d)
    cn_ensure(st.posl, ww * d)
    cn_ensure(st.qu, tt * dh)
    cn_ensure(st.qv, tt * dh)
    cn_ensure(st.kt, dh * tt)
    cn_ensure(st.vh, tt * dh)
    cn_ensure(st.posh, dh * ww)
    cn_ensure(st.sc, tt * tt)
    cn_ensure(st.rl, tt * ww)
    cn_ensure(st.oh, tt * dh)
    let scale = 1.0 / sqrt(float(dh))
    let nk = m.conv_kernel
    let kpad = (nk - 1l) / 2l

    for (l in range64(m.n_layer)) {
        let lo = m.layers[l]

        // FFN1 (macaron half)
        layernorm_batch(st.xb, st.x, m.blob, lo.nf1_w, lo.nf1_b, d, tt, m.eps)
        mm_blob_b(st.h, m.blob, lo.ff1a_w, st.xb, d, ff, tt)
        add_bias_rows(st.h, m.blob, lo.ff1a_b, ff, tt)
        silu4_batch(st.h, ff, tt)
        mm_blob_b(st.xb, m.blob, lo.ff1b_w, st.h, ff, d, tt)
        add_bias_rows(st.xb, m.blob, lo.ff1b_b, d, tt)
        for (i in range64(tt * d)) {
            st.x[i] += 0.5 * st.xb[i]
        }

        // rel-pos self-attention (Transformer-XL u/v biases)
        layernorm_batch(st.xb, st.x, m.blob, lo.nat_w, lo.nat_b, d, tt, m.eps)
        mm_blob_b(st.qq, m.blob, lo.q_w, st.xb, d, d, tt); add_bias_rows(st.qq, m.blob, lo.q_b, d, tt)
        mm_blob_b(st.kk, m.blob, lo.k_w, st.xb, d, d, tt); add_bias_rows(st.kk, m.blob, lo.k_b, d, tt)
        mm_blob_b(st.vv, m.blob, lo.v_w, st.xb, d, d, tt); add_bias_rows(st.vv, m.blob, lo.v_b, d, tt)
        mm_blob_b(st.posl, m.blob, lo.pos_w, st.pos_emb, d, d, ww)
        for (hi in range64(n_head)) {
            let hoff = hi * dh
            for (t in range64(tt)) {
                for (j in range64(dh)) {
                    let qv = st.qq[t * d + hoff + j]
                    st.qu[t * dh + j] = qv + m.blob[lo.bias_u + hoff + j]
                    st.qv[t * dh + j] = qv + m.blob[lo.bias_v + hoff + j]
                    st.kt[j * tt + t] = st.kk[t * d + hoff + j]
                    st.vh[t * dh + j] = st.vv[t * d + hoff + j]
                }
            }
            for (w in range64(ww)) {
                for (j in range64(dh)) {
                    st.posh[j * ww + w] = st.posl[w * d + hoff + j]
                }
            }
            for (i in range64(tt * tt)) {
                st.sc[i] = 0.0
            }
            for (i in range64(tt * ww)) {
                st.rl[i] = 0.0
            }
            gemm_f32(unsafe(addr(st.sc[0])), unsafe(addr(st.qu[0])), unsafe(addr(st.kt[0])), tt, dh, tt)
            gemm_f32(unsafe(addr(st.rl[0])), unsafe(addr(st.qv[0])), unsafe(addr(st.posh[0])), tt, dh, ww)
            for (q in range64(tt)) {
                let base = tt - 1l - q
                for (k in range64(tt)) {
                    st.sc[q * tt + k] = (st.sc[q * tt + k] + st.rl[q * ww + base + k]) * scale
                }
                softmax(unsafe(addr(st.sc[q * tt])), tt)
            }
            for (i in range64(tt * dh)) {
                st.oh[i] = 0.0
            }
            gemm_f32(unsafe(addr(st.oh[0])), unsafe(addr(st.sc[0])), unsafe(addr(st.vh[0])), tt, tt, dh)
            for (t in range64(tt)) {
                for (j in range64(dh)) {
                    st.ao[t * d + hoff + j] = st.oh[t * dh + j]
                }
            }
        }
        mm_blob_b(st.xb, m.blob, lo.o_w, st.ao, d, d, tt); add_bias_rows(st.xb, m.blob, lo.o_b, d, tt)
        add_inplace_rows(st.x, st.xb, tt * d)

        // conv module: LN -> pw1 -> GLU -> depthwise(k9) -> foldedBN -> silu -> pw2
        layernorm_batch(st.xb, st.x, m.blob, lo.ncv_w, lo.ncv_b, d, tt, m.eps)
        mm_blob_b(st.h, m.blob, lo.pw1_w, st.xb, d, 2l * d, tt)
        add_bias_rows(st.h, m.blob, lo.pw1_b, 2l * d, tt)
        for (t in range64(tt)) {   // GLU: signal · σ(gate)
            for (j in range64(d)) {
                let sg = st.h[t * 2l * d + j]
                let gt = st.h[t * 2l * d + d + j]
                st.glu[t * d + j] = sg * (1.0 / (1.0 + exp(-gt)))
            }
        }
        for (t in range64(tt)) {   // depthwise conv1d over time + folded BN + silu
            for (c in range64(d)) {
                var acc = 0.0
                for (k in range64(nk)) {
                    let src = t + k - kpad
                    if (src >= 0l && src < tt) {
                        acc += m.blob[lo.dw_w + c * nk + k] * st.glu[src * d + c]
                    }
                }
                let bv = acc * m.blob[lo.bn_w + c] + m.blob[lo.bn_b + c]
                st.ao[t * d + c] = bv * (1.0 / (1.0 + exp(-bv)))
            }
        }
        mm_blob_b(st.xb, m.blob, lo.pw2_w, st.ao, d, d, tt); add_bias_rows(st.xb, m.blob, lo.pw2_b, d, tt)
        add_inplace_rows(st.x, st.xb, tt * d)

        // FFN2 (second macaron half)
        layernorm_batch(st.xb, st.x, m.blob, lo.nf2_w, lo.nf2_b, d, tt, m.eps)
        mm_blob_b(st.h, m.blob, lo.ff2a_w, st.xb, d, ff, tt)
        add_bias_rows(st.h, m.blob, lo.ff2a_b, ff, tt)
        silu4_batch(st.h, ff, tt)
        mm_blob_b(st.xb, m.blob, lo.ff2b_w, st.h, ff, d, tt)
        add_bias_rows(st.xb, m.blob, lo.ff2b_b, d, tt)
        for (i in range64(tt * d)) {
            st.x[i] += 0.5 * st.xb[i]
        }

        // per-layer post norm
        layernorm_batch(st.x, st.x, m.blob, lo.nout_w, lo.nout_b, d, tt, m.eps)
    }

    // perception.proj: encoder rows (d) -> decoder embedding (proj_dim)
    cn_ensure(st.out, tt * m.proj_dim)
    mm_blob_b(st.out, m.blob, m.proj_w, st.x, d, m.proj_dim, tt)
    add_bias_rows(st.out, m.blob, m.proj_b, m.proj_dim, tt)
    return tt
}
