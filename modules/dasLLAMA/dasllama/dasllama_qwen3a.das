options gen2
options _comment_hygiene = true

module dasllama_qwen3a shared public

require math
require daslib/fio
require dasllama/dasllama_math
require dasllama/dasllama_gguf
require dasllama/dasllama_audio

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
//! projector live in `cblob`.
struct Qwen3aTower {
    core : AudioTower = AudioTower()   // d_model/heads/blocks/post_ln; has_k_bias; n_ctx = window positions
    proj_dim : int64                   // decoder embedding width (mm2 output)
    n_ch : int64                       // conv channel count (480)
    cblob : array<float>
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
}

//! Load a Qwen3-ASR mmproj GGUF (projector type `qwen3a`, bf16/f16/f32 tensors).
def load_qwen3a_tower(path : string) : Qwen3aTower {
    var t = Qwen3aTower()
    let f = fopen(path, "rb")
    if (f == null) {
        panic("dasLLAMA: cannot open mmproj gguf '{path}'")
    }
    fmap(f) $(var bytes : array<uint8>#) {
        let m <- parse_gguf_meta(bytes)
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
        c.fblob |> resize(int(o))
        for (l in range64(c.n_layer)) {
            let lo = enc_layer_offsets(c, l)
            gguf_read_tensor_f32(m, bytes, "a.blk.{l}.ln1.weight", c.fblob, lo.ln1_w, d)
            gguf_read_tensor_f32(m, bytes, "a.blk.{l}.ln1.bias", c.fblob, lo.ln1_b, d)
            gguf_read_tensor_f32(m, bytes, "a.blk.{l}.attn_q.weight", c.fblob, lo.q_w, d * d)
            gguf_read_tensor_f32(m, bytes, "a.blk.{l}.attn_q.bias", c.fblob, lo.q_b, d)
            gguf_read_tensor_f32(m, bytes, "a.blk.{l}.attn_k.weight", c.fblob, lo.k_w, d * d)
            gguf_read_tensor_f32(m, bytes, "a.blk.{l}.attn_k.bias", c.fblob, lo.k_b, d)
            gguf_read_tensor_f32(m, bytes, "a.blk.{l}.attn_v.weight", c.fblob, lo.v_w, d * d)
            gguf_read_tensor_f32(m, bytes, "a.blk.{l}.attn_v.bias", c.fblob, lo.v_b, d)
            gguf_read_tensor_f32(m, bytes, "a.blk.{l}.attn_out.weight", c.fblob, lo.o_w, d * d)
            gguf_read_tensor_f32(m, bytes, "a.blk.{l}.attn_out.bias", c.fblob, lo.o_b, d)
            gguf_read_tensor_f32(m, bytes, "a.blk.{l}.ln2.weight", c.fblob, lo.ln2_w, d)
            gguf_read_tensor_f32(m, bytes, "a.blk.{l}.ln2.bias", c.fblob, lo.ln2_b, d)
            gguf_read_tensor_f32(m, bytes, "a.blk.{l}.ffn_up.weight", c.fblob, lo.fc1_w, ff * d)
            gguf_read_tensor_f32(m, bytes, "a.blk.{l}.ffn_up.bias", c.fblob, lo.fc1_b, ff)
            gguf_read_tensor_f32(m, bytes, "a.blk.{l}.ffn_down.weight", c.fblob, lo.fc2_w, d * ff)
            gguf_read_tensor_f32(m, bytes, "a.blk.{l}.ffn_down.bias", c.fblob, lo.fc2_b, d)
        }
        gguf_read_tensor_f32(m, bytes, "a.post_ln.weight", c.fblob, c.lnp_w_off, d)
        gguf_read_tensor_f32(m, bytes, "a.post_ln.bias", c.fblob, c.lnp_b_off, d)
        t.core <- c

        // conv frontend + projector blob
        let i3 = gguf_find_tensor(m, "a.conv2d.1.weight")
        if (i3 < 0) {
            panic("dasLLAMA: a.conv2d.1.weight missing from qwen3a mmproj")
        }
        let nch = m.tensors[i3].n_elem / 9l          // [3,3,1,nch]
        t.n_ch = nch
        let feat = 16l * nch                          // conv3 output H=16 × channels
        let i2 = gguf_find_tensor(m, "mm.a.mlp.2.weight")
        if (i2 < 0) {
            panic("dasLLAMA: mm.a.mlp.2.weight missing from qwen3a mmproj")
        }
        t.proj_dim = m.tensors[i2].n_elem / d
        var co = 0l
        t.c1_w_off = co; co += nch * 9l               // cin = 1
        t.c1_b_off = co; co += nch
        t.c2_w_off = co; co += nch * nch * 9l
        t.c2_b_off = co; co += nch
        t.c3_w_off = co; co += nch * nch * 9l
        t.c3_b_off = co; co += nch
        t.cout_w_off = co; co += d * feat
        t.cout_b_off = co; co += d
        t.pos_off = co; co += QWEN3A_TOK_PER_CHUNK * d
        t.mm1_w_off = co; co += d * d
        t.mm1_b_off = co; co += d
        t.mm2_w_off = co; co += t.proj_dim * d
        t.mm2_b_off = co; co += t.proj_dim
        t.cblob |> resize(int(co))
        gguf_read_tensor_f32(m, bytes, "a.conv2d.1.weight", t.cblob, t.c1_w_off, nch * 9l)
        gguf_read_tensor_f32(m, bytes, "a.conv2d.1.bias", t.cblob, t.c1_b_off, nch)
        gguf_read_tensor_f32(m, bytes, "a.conv2d.2.weight", t.cblob, t.c2_w_off, nch * nch * 9l)
        gguf_read_tensor_f32(m, bytes, "a.conv2d.2.bias", t.cblob, t.c2_b_off, nch)
        gguf_read_tensor_f32(m, bytes, "a.conv2d.3.weight", t.cblob, t.c3_w_off, nch * nch * 9l)
        gguf_read_tensor_f32(m, bytes, "a.conv2d.3.bias", t.cblob, t.c3_b_off, nch)
        gguf_read_tensor_f32(m, bytes, "a.conv_out.weight", t.cblob, t.cout_w_off, d * feat)
        if (gguf_find_tensor(m, "a.conv_out.bias") >= 0) {   // absent in the shipped mmprojs — region stays zero
            gguf_read_tensor_f32(m, bytes, "a.conv_out.bias", t.cblob, t.cout_b_off, d)
        }
        // positions reset per chunk — only rows 0..12 of the stored table are ever used
        gguf_read_tensor_f32(m, bytes, "a.position_embd.weight", t.cblob, t.pos_off, QWEN3A_TOK_PER_CHUNK * d)
        gguf_read_tensor_f32(m, bytes, "mm.a.mlp.1.weight", t.cblob, t.mm1_w_off, d * d)
        gguf_read_tensor_f32(m, bytes, "mm.a.mlp.1.bias", t.cblob, t.mm1_b_off, d)
        gguf_read_tensor_f32(m, bytes, "mm.a.mlp.2.weight", t.cblob, t.mm2_w_off, t.proj_dim * d)
        gguf_read_tensor_f32(m, bytes, "mm.a.mlp.2.bias", t.cblob, t.mm2_b_off, t.proj_dim)
    }
    fclose(f)
    return <- t
}

//! Qwen3-ASR's log-mel (WhisperFeatureExtractor, center=True, truncation=False): 200-sample
//! reflection pad both ends, NO 30s zero tail, clamp max stays in double (unlike mtmd whisper,
//! no float round-trip); n_len = min((n_padded−400)/160+1, n_in/160+1), mel-major.
def log_mel_qwen3a(samples : array<float> | #; n_mel : int64) : WhisperMel {
    let n_in = int64(length(samples))
    let head = AUDIO_N_FFT / 2l                        // 200 reflection each side
    let n_padded = n_in + 2l * head
    var padded : array<float>
    padded |> resize(int(n_padded))
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
    m.data |> resize(int(n_mel * m.n_len))

    let hann <- build_hann_window(n_fft)
    let twiddles <- build_dft_twiddles(n_fft)
    let filters <- build_mel_filterbank(n_mel, n_fft, AUDIO_SAMPLE_RATE)

    var xw : array<float>
    xw |> resize(int(m.n_len * n_fft))
    for (i in range64(m.n_len)) {
        let off = i * AUDIO_HOP
        let valid = min(n_fft, n_padded - off)
        for (j in range64(valid)) {
            xw[i * n_fft + j] = hann[j] * padded[off + j]
        }
    }
    var reim : array<float>
    reim |> resize(int(m.n_len * n_bins * 2l))
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
    c.data |> resize(int(m.n_mel * c.n_len))
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
def private conv2d_s2(var out : array<float>; var col : array<float>;
                      x : array<float>; blob : array<float>; w_off, b_off : int64;
                      iw, ih, cin, cout : int64) {
    let ow = (iw - 1l) / 2l + 1l
    let oh = (ih - 1l) / 2l + 1l
    let kdim = cin * 9l
    let rows = oh * ow
    col |> resize(int(rows * kdim))
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
    out |> resize(int(rows * cout))
    mm_blob_b(out, blob, w_off, col, kdim, cout, rows)
    add_bias_rows(out, blob, b_off, cout, rows)
    gelu_erf(out, rows * cout)
}

//! Scratch for the qwen3a conv frontend (sized on first use, reused across windows/calls).
struct Qwen3aConvState {
    xin : array<float>    // chunk input [100·128] as [ih][iw][1]
    col : array<float>
    y1 : array<float>
    y2 : array<float>
    y3 : array<float>
    feat : array<float>   // [13 × 16·n_ch]
    crow : array<float>   // conv_out rows for one chunk [13 × d_model]
}

//! Encode one padded mel window (`qwen3a_mel_window` output) to soft tokens in ``s.out``:
//! ``(n_len/100)·13`` rows × ``proj_dim``. Runs the conv2d frontend per 100-frame chunk,
//! the shared transformer blocks over the window's positions, post-LN, and the MLP projector.
def qwen3a_encode(t : Qwen3aTower; var s : EncoderState; var cs : Qwen3aConvState; win : MelChunk) {
    let d = t.core.d_model
    let nm = t.core.n_mel
    let nch = t.n_ch
    let n_chunks = win.n_len / QWEN3A_CHUNK
    let npos = n_chunks * QWEN3A_TOK_PER_CHUNK
    if (npos > t.core.n_ctx) {
        panic("qwen3a_encode: window has {npos} positions, tower sized for {t.core.n_ctx}")
    }

    // conv frontend per chunk -> rows of s.x
    cs.xin |> resize(int(QWEN3A_CHUNK * nm))
    for (ci in range64(n_chunks)) {
        // chunk input as W=100 (frames), H=128 (mel), C=1: x[ih][iw] = mel[ih][ci·100 + iw]
        for (ih in range64(nm)) {
            for (iw in range64(QWEN3A_CHUNK)) {
                cs.xin[ih * QWEN3A_CHUNK + iw] = win.data[ih * win.n_len + ci * QWEN3A_CHUNK + iw]
            }
        }
        conv2d_s2(cs.y1, cs.col, cs.xin, t.cblob, t.c1_w_off, t.c1_b_off, QWEN3A_CHUNK, nm, 1l, nch)   // 50×64
        conv2d_s2(cs.y2, cs.col, cs.y1, t.cblob, t.c2_w_off, t.c2_b_off, 50l, 64l, nch, nch)           // 25×32
        conv2d_s2(cs.y3, cs.col, cs.y2, t.cblob, t.c3_w_off, t.c3_b_off, 25l, 32l, nch, nch)           // 13×16
        // features per output frame ow: v[h + 16·c] = y3[oh=h][ow][c]
        let feat = 16l * nch
        cs.feat |> resize(int(QWEN3A_TOK_PER_CHUNK * feat))
        for (ow in range64(QWEN3A_TOK_PER_CHUNK)) {
            for (h in range64(16l)) {
                for (ch in range64(nch)) {
                    cs.feat[ow * feat + h + 16l * ch] = cs.y3[(h * QWEN3A_TOK_PER_CHUNK + ow) * nch + ch]
                }
            }
        }
        // conv_out rows for this chunk, + per-chunk positions 0..12, into the residual stream
        cs.crow |> resize(int(QWEN3A_TOK_PER_CHUNK * d))
        mm_blob_b(cs.crow, t.cblob, t.cout_w_off, cs.feat, feat, d, QWEN3A_TOK_PER_CHUNK)
        add_bias_rows(cs.crow, t.cblob, t.cout_b_off, d, QWEN3A_TOK_PER_CHUNK)
        let xbase = ci * QWEN3A_TOK_PER_CHUNK * d
        for (i in range64(QWEN3A_TOK_PER_CHUNK * d)) {
            s.x[xbase + i] = cs.crow[i] + t.cblob[t.pos_off + i]
        }
    }

    // transformer blocks + post-LN over the window's positions
    encoder_blocks(t.core, s, npos)
    layernorm_batch(s.xb, s.x, t.core.fblob, t.core.lnp_w_off, t.core.lnp_b_off, d, npos, t.core.eps)

    // MLP projector: mm1 + bias -> gelu_erf -> mm2 + bias
    s.ph |> resize(int(npos * d))
    mm_blob_b(s.ph, t.cblob, t.mm1_w_off, s.xb, d, d, npos)
    add_bias_rows(s.ph, t.cblob, t.mm1_b_off, d, npos)
    gelu_erf(s.ph, npos * d)
    s.out |> resize(int(npos * t.proj_dim))
    mm_blob_b(s.out, t.cblob, t.mm2_w_off, s.ph, d, t.proj_dim, npos)
    add_bias_rows(s.out, t.cblob, t.mm2_b_off, t.proj_dim, npos)
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
        rows |> reserve(int(int64(length(rows)) + n * t.proj_dim))
        for (i in range64(n * t.proj_dim)) {
            rows |> push(s.out[i])
        }
        total += n
        off += QWEN3A_WINDOW
    }
    return total
}
