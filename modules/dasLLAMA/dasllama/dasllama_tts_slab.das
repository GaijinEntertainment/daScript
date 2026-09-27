options gen2
options indenting = 4
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_tts_slab shared public

require dasllama/dasllama_lint public
require math
require dasllama/dasllama_kqformat
require dasllama/dasllama_math
require dasllama/dasllama_par
require dasllama/dasllama_convert
require dasllama/dasllama_tts_blocks
require dasllama/dasllama_styletts2
require dasllama/dasllama_pocket
require dasllama/dasllama_rope

//! The host side of a TTS tower's weight slab, backend-neutral: the StyleTTS2 parts' weights laid out
//! as the f32 rows a tile GEMM and the row kernels read - every conv dense as [cout padded to 64 x
//! k*cin padded to 32] with column j = tap*cin + ci and its bias row beside it, every norm as a scale
//! and a shift row, an LSTM direction as its input linear and its dense recurrence - written by one
//! writer that runs twice (a measuring pass sizes the host copy, the second fills it); the slot records
//! carry element offsets a device driver turns into its own binding offsets; the keys fold the parts'
//! weight addresses and the q8 lane's repack layout, so a reload or another lane keys differently.

struct TtsSlabWriter {
    @exact_size host : array<float>   //! sized once from the measure pass; the write pass only bumps `n`
    n : int64
    measure : bool
    lstm_turned : bool = true   //! an LSTM recurrence written turned to [H][4H] (the Vulkan kernel's read), else as the model stores it (Metal's)
}

//! `count` floats at the next 16-byte boundary (every item is a float4 read for some kernel); the element offset
def tsw_alloc(var w : TtsSlabWriter; count : int64) : int64 {
    let at = round_up(w.n, 4l)
    w.n = at + count
    return at
}

def tsw_put(var w : TtsSlabWriter; i : int64; v : float) {
    if (!w.measure) {
        w.host[i] = v
    }
}

//! a conv's or a linear's dense rows: the weight rows at `woff`, the bias row at `boff` (element offsets),
//! `cout_p` rows of `kdim_p` columns; `gathers` marks a conv whose channel count sits on the 8 lattice
struct TtsConvSlot {
    woff : int64
    boff : int64
    cout_p : int64
    kdim_p : int64
    gathers : bool
}

//! a norm's scale row at `gwoff` and shift row at `gboff`; `aux_off` an alpha row beside them (the Snake blocks)
struct TtsNormSlot {
    gwoff : int64
    gboff : int64
    aux_off : int64
}

struct TtsLstmSlot {
    fx : TtsConvSlot    //! the forward direction's input linear [4H][nin] with its bias
    bx : TtsConvSlot
    fhh : int64         //! ... and its recurrence transposed [H][4H]
    bhh : int64
}

struct TtsBlockSlots {
    c1 : array<TtsConvSlot>
    c2 : array<TtsConvSlot>
    n1 : array<TtsNormSlot>
    n2 : array<TtsNormSlot>
}

struct TtsResSlots {
    n1 : TtsNormSlot
    n2 : TtsNormSlot
    c1 : TtsConvSlot
    c2 : TtsConvSlot
    c1x1 : TtsConvSlot
    pool_w : int64
    pool_b : int64
}

struct TtsAlbertSlab {
    word_emb : int64
    pos_emb : int64
    type_emb : int64
    emb_ln : TtsNormSlot
    map_in : TtsConvSlot
    q : TtsConvSlot
    k : TtsConvSlot
    v : TtsConvSlot
    dense : TtsConvSlot
    attn_ln : TtsNormSlot
    ffn : TtsConvSlot
    ffn_out : TtsConvSlot
    full_ln : TtsNormSlot
}

struct TtsTextSlab {
    emb : int64
    cnn : array<TtsConvSlot>
    ln : array<TtsNormSlot>
    lstm : TtsLstmSlot
}

struct TtsPredSlab {
    bert_enc : TtsConvSlot
    de_lstm : array<TtsLstmSlot>
    dur_lstm : TtsLstmSlot
    dur_proj : TtsConvSlot
    sh_lstm : TtsLstmSlot
    f0b : array<TtsResSlots>
    nb : array<TtsResSlots>
    f0_proj : TtsConvSlot
    n_proj : TtsConvSlot
}

struct TtsDecSlab {
    ups : array<TtsConvSlot>
    noise : array<TtsConvSlot>
    blocks : array<TtsBlockSlots>
    post : TtsConvSlot
    f0c : TtsConvSlot
    nc : TtsConvSlot
    asr_res : TtsConvSlot
    encode : TtsResSlots
    decode : array<TtsResSlots>
    src_w : int64
    src_b : int64
    stft_re : int64
    stft_im : int64
    istft_re : int64
    istft_im : int64
    window : int64
}

[arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_TTS.md#vk-tts-chain")]
def private tsw_put_q8(var w : TtsSlabWriter; q : array<int8>; qs : array<float>; rows, cols, w0, stride : int64) {
    return if (w.measure)
    let lay = active_q8_repack_layout(g_active_needs_repack)
    unsafe {
        let qp = addr<int8 const?>(q[0])
        let sp = addr<float const?>(qs[0])
        var hp = addr(w.host[w0])
        maybe_parallel_for(0, int(rows), lanes_for_work(rows * cols, 0)) $(rb, re) {
            unsafe {
                for (r in range64(int64(rb), int64(re))) {
                    q8_plane_row_f32(qp, sp, cols, rows, lay, r, hp + r * stride)
                }
            }
        }
    }
}

[arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_TTS.md#vk-tts-chain")]
def private tsw_put_kq(var w : TtsSlabWriter; l : TtsLinear; w0, stride : int64) {
    return if (w.measure)
    let f = kq_fmt_of_id(l.kq_fmt, "the tower slab")
    var inscope @exact_size row : array<float>
    row |> reserve_resize(l.nin)
    for (r in range64(l.nout)) {
        kq_plane_row_f32(f, l.kq, l.ks, l.nin, l.nout, l.kq_repacked ? active_kq_layout_mr(l.kq_fmt) : 1l, r, row, 0l)
        for (i in range64(l.nin)) {
            w.host[w0 + r * stride + i] = row[i]
        }
    }
}

//! n floats of src into the slab, zeros past its length (a window the model does not carry); the element offset
def tts_slab_raw(var w : TtsSlabWriter; src : array<float>; n : int64) : int64 {
    let at = tsw_alloc(w, n)
    for (i in range64(min(n, long_length(src)))) {
        tsw_put(w, at + i, src[i])
    }
    return at
}

def tts_slab_conv(var w : TtsSlabWriter; c : TtsConv1d) : TtsConvSlot {
    let kc = c.k * c.cin
    var s = TtsConvSlot(cout_p = round_up(c.cout, 64l), kdim_p = round_up(kc, 32l), gathers = c.cin % 8l == 0l)
    s.woff = tsw_alloc(w, s.cout_p * s.kdim_p)
    if (c.q8) {
        tsw_put_q8(w, c.wq, c.wqs, c.cout, kc, s.woff, s.kdim_p)
    } elif (!w.measure) {
        for (co in range64(c.cout)) {
            for (ci in range64(c.cin)) {
                for (tap in range64(c.k)) {
                    var v = 0.0
                    if (c.transposed) {
                        v = !empty(c.wtr) ? c.wtr[(ci * c.k + tap) * c.cout + co] : c.wt[(co * c.k + tap) * c.cin + ci]
                    } else {
                        v = !empty(c.wk) ? c.wk[(tap * c.cin + ci) * c.cout_s + co] : c.w[(co * c.cin + ci) * c.k + tap]
                    }
                    w.host[s.woff + co * s.kdim_p + tap * c.cin + ci] = v
                }
            }
        }
    }
    s.boff = tts_slab_raw(w, c.b, s.cout_p)
    return s
}

def tts_slab_linear(var w : TtsSlabWriter; l : TtsLinear) : TtsConvSlot {
    var s = TtsConvSlot(cout_p = round_up(l.nout, 64l), kdim_p = round_up(l.nin, 32l), gathers = false)
    s.woff = tsw_alloc(w, s.cout_p * s.kdim_p)
    if (l.q8) {
        tsw_put_q8(w, l.wq, l.wqs, l.nout, l.nin, s.woff, s.kdim_p)
    } elif (l.kq_fmt != 0) {
        tsw_put_kq(w, l, s.woff, s.kdim_p)
    } elif (!w.measure) {
        for (o in range64(l.nout)) {
            for (i in range64(l.nin)) {
                w.host[s.woff + o * s.kdim_p + i] = !empty(l.w) ? l.w[o * l.nin + i] : l.wt[i * l.nout + o]
            }
        }
    }
    s.boff = tts_slab_raw(w, l.b, s.cout_p)
    return s
}

[arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_TTS.md#vk-tts-chain")]
def private tts_slab_lstm_dir(var w : TtsSlabWriter; l : TtsLstm; var xs : TtsConvSlot; var hh : int64&) {
    var lin = TtsLinear(nin = l.nin, nout = 4l * l.hidden)
    lin.w := l.w_ih
    lin.b := l.b
    xs = tts_slab_linear(w, lin)
    let hd = l.hidden
    if (!w.lstm_turned) {
        hh = tts_slab_raw(w, l.w_hh, 4l * hd * hd)
        return
    }
    hh = tsw_alloc(w, 4l * hd * hd)
    return if (w.measure)
    for (o in range64(4l * hd)) {
        for (i in range64(hd)) {
            w.host[hh + i * 4l * hd + o] = l.w_hh[o * hd + i]
        }
    }
}

def tts_slab_lstm(var w : TtsSlabWriter; l : TtsBiLstm) : TtsLstmSlot {
    var s : TtsLstmSlot
    tts_slab_lstm_dir(w, l.fwd, s.fx, s.fhh)
    tts_slab_lstm_dir(w, l.bwd, s.bx, s.bhh)
    return s
}

//! a norm's scale and shift rows (ones and zeros when it carries none)
def tts_slab_norm_rows(var w : TtsSlabWriter; n : TtsNorm; c : int64) : TtsNormSlot {
    let g0 = tsw_alloc(w, 2l * c)
    for (i in range64(c)) {
        tsw_put(w, g0 + i, empty(n.w) ? 1.0 : n.w[i])
        tsw_put(w, g0 + c + i, empty(n.b) ? 0.0 : n.b[i])
    }
    return TtsNormSlot(gwoff = g0, gboff = g0 + c)
}

def tts_slab_norm(var w : TtsSlabWriter; a : TtsAdaIn; alpha : TtsWeight; c : int64) : TtsNormSlot {
    var s = tts_slab_norm_rows(w, a.norm, c)
    s.aux_off = tts_slab_raw(w, alpha.a, c)
    return s
}

def tts_slab_block(var w : TtsSlabWriter; b : St2GenResBlock) : TtsBlockSlots {
    var s : TtsBlockSlots
    let n = length(b.convs1)
    s.c1 |> reserve(n)
    s.c2 |> reserve(n)
    s.n1 |> reserve(n)
    s.n2 |> reserve(n)
    for (j in range(n)) {
        s.c1 |> push(tts_slab_conv(w, b.convs1[j]))
        s.c2 |> push(tts_slab_conv(w, b.convs2[j]))
        s.n1 |> push(tts_slab_norm(w, b.adain1[j], b.alpha1[j], b.c))
        s.n2 |> push(tts_slab_norm(w, b.adain2[j], b.alpha2[j], b.c))
    }
    return <- s
}

def tts_slab_res(var w : TtsSlabWriter; b : St2AdainResBlk) : TtsResSlots {
    var s = TtsResSlots(n1 = tts_slab_norm_rows(w, b.norm1.norm, b.dim_in_s), n2 = tts_slab_norm_rows(w, b.norm2.norm, b.dim_out),
        c1 = tts_slab_conv(w, b.conv1), c2 = tts_slab_conv(w, b.conv2))
    if (b.learned_sc) {
        s.c1x1 = tts_slab_conv(w, b.conv1x1)
    }
    if (b.upsample) {
        s.pool_w = tts_slab_raw(w, b.pool.w, b.pool.cin * b.pool.k)
        s.pool_b = tts_slab_raw(w, b.pool.b, b.pool.cin)
    }
    return s
}

def tts_albert_write(var w : TtsSlabWriter; a : St2Albert; var s : TtsAlbertSlab) {
    s.word_emb = tts_slab_raw(w, a.word_emb.a, long_length(a.word_emb.a))
    s.pos_emb = tts_slab_raw(w, a.pos_emb.a, long_length(a.pos_emb.a))
    s.type_emb = tts_slab_raw(w, a.type_emb.a, a.emb_dim)
    s.emb_ln = tts_slab_norm_rows(w, a.emb_ln, a.emb_dim)
    s.map_in = tts_slab_linear(w, a.map_in)
    s.q = tts_slab_linear(w, a.q)
    s.k = tts_slab_linear(w, a.k)
    s.v = tts_slab_linear(w, a.v)
    s.dense = tts_slab_linear(w, a.dense)
    s.attn_ln = tts_slab_norm_rows(w, a.attn_ln, a.hidden)
    s.ffn = tts_slab_linear(w, a.ffn)
    s.ffn_out = tts_slab_linear(w, a.ffn_out)
    s.full_ln = tts_slab_norm_rows(w, a.full_ln, a.hidden)
}

def tts_text_write(var w : TtsSlabWriter; te : St2TextEncoder; var s : TtsTextSlab) {
    s.emb = tts_slab_raw(w, te.emb.a, long_length(te.emb.a))
    s.cnn |> reserve(length(te.cnn))
    s.ln |> reserve(length(te.cnn_ln))
    for (c in te.cnn) {
        s.cnn |> push(tts_slab_conv(w, c))
    }
    for (n in te.cnn_ln) {
        s.ln |> push(tts_slab_norm_rows(w, n, te.c))
    }
    s.lstm = tts_slab_lstm(w, te.lstm)
}

def tts_pred_write(var w : TtsSlabWriter; p : St2Predictor; var s : TtsPredSlab) {
    s.bert_enc = tts_slab_linear(w, p.bert_encoder)
    s.de_lstm |> reserve(length(p.lstms))
    for (l in p.lstms) {
        s.de_lstm |> push(tts_slab_lstm(w, l))
    }
    s.dur_lstm = tts_slab_lstm(w, p.lstm)
    s.dur_proj = tts_slab_linear(w, p.duration_proj)
    s.sh_lstm = tts_slab_lstm(w, p.shared_lstm)
    s.f0b |> reserve(length(p.f0))
    s.nb |> reserve(length(p.n))
    for (b in p.f0) {
        s.f0b |> push(tts_slab_res(w, b))
    }
    for (b in p.n) {
        s.nb |> push(tts_slab_res(w, b))
    }
    s.f0_proj = tts_slab_conv(w, p.f0_proj)
    s.n_proj = tts_slab_conv(w, p.n_proj)
}

def tts_dec_write(var w : TtsSlabWriter; d : St2Decoder; var s : TtsDecSlab) {
    s.ups |> reserve(length(d.ups))
    s.noise |> reserve(length(d.noise_convs))
    s.blocks |> reserve(length(d.noise_res) + length(d.resblocks))
    s.decode |> reserve(length(d.decode))
    for (c in d.ups) {
        s.ups |> push(tts_slab_conv(w, c))
    }
    for (c in d.noise_convs) {
        s.noise |> push(tts_slab_conv(w, c))
    }
    for (b in d.noise_res) {
        s.blocks |> emplace(tts_slab_block(w, b))
    }
    for (b in d.resblocks) {
        s.blocks |> emplace(tts_slab_block(w, b))
    }
    s.post = tts_slab_conv(w, d.conv_post)
    s.f0c = tts_slab_conv(w, d.f0_conv)
    s.nc = tts_slab_conv(w, d.n_conv)
    s.asr_res = tts_slab_conv(w, d.asr_res)
    s.encode = tts_slab_res(w, d.encode)
    for (b in d.decode) {
        s.decode |> push(tts_slab_res(w, b))
    }
    s.src_w = tts_slab_raw(w, d.source_linear.w, d.source_linear.nin)
    s.src_b = tts_slab_raw(w, d.source_linear.b, 1l)
    s.stft_re = tts_slab_raw(w, d.stft_fwd_re.w, d.stft_fwd_re.cout * d.stft_fwd_re.k)
    s.stft_im = tts_slab_raw(w, d.stft_fwd_im.w, d.stft_fwd_im.cout * d.stft_fwd_im.k)
    s.istft_re = tts_slab_raw(w, d.stft_bwd_re.wt, d.stft_bwd_re.cin * d.stft_bwd_re.k)
    s.istft_im = tts_slab_raw(w, d.stft_bwd_im.wt, d.stft_bwd_im.cin * d.stft_bwd_im.k)
    s.window = tts_slab_raw(w, d.window.a, d.stft_bwd_re.k)
}

//! the part's writer run twice (a measuring pass, then the fill); the host floats come back to upload, empty when the part carries nothing
def tts_slab_build(var slots : auto(SS); write : block<(var w : TtsSlabWriter; var s : SS) : void>; lstm_turned : bool = true) : array<float> {
    var inscope w = TtsSlabWriter(measure = true, lstm_turned = lstm_turned)
    var probe : SS
    invoke(write, w, probe)
    unsafe {
        delete probe
    }
    w.host |> reserve_resize(w.n)
    w.n = 0l
    w.measure = false
    invoke(write, w, slots)
    return <- w.host
}

//! the q8 lane's repack layout, folded into every slab key: the same quants under another layout dequantize differently
def tts_lane_key() : uint64 {
    return 1ul if (!g_active_needs_repack)
    let lay = active_q8_repack_layout(true)
    return hash_combine64(hash_combine64(uint64(lay.mr), uint64(lay.kgroup)), uint64(lay.wbias))
}

def tts_conv_key(c : TtsConv1d) : uint64 {
    unsafe {
        if (c.q8) {
            return intptr(addr(c.wq[0]))
        }
        if (!empty(c.w)) {
            return intptr(addr(c.w[0]))
        }
        if (!empty(c.wt)) {
            return intptr(addr(c.wt[0]))
        }
        return c.transposed ? intptr(addr(c.wtr[0])) : intptr(addr(c.wk[0]))
    }
}

def private tts_key_fold(var key : uint64&; c : TtsConv1d) {
    key = hash_combine64(key, tts_conv_key(c))
}

def private tts_key_block(var key : uint64&; b : St2GenResBlock) {
    for (c in b.convs1) {
        tts_key_fold(key, c)
    }
    for (c in b.convs2) {
        tts_key_fold(key, c)
    }
}

def private tts_key_res(var key : uint64&; b : St2AdainResBlk) {
    tts_key_fold(key, b.conv1)
    tts_key_fold(key, b.conv2)
}

def tts_lin_key(l : TtsLinear) : uint64 {
    unsafe {
        if (l.q8) {
            return intptr(addr(l.wq[0]))
        }
        if (l.kq_fmt != 0) {   //! the plane's format and the active layout its rows are gathered through: a reload under another layout writes other rows
            return hash_combine64(hash_combine64(intptr(addr(l.kq[0])), uint64(l.kq_fmt)), uint64(active_kq_layout_mr(l.kq_fmt)))
        }
        return !empty(l.w) ? intptr(addr(l.w[0])) : intptr(addr(l.wt[0]))
    }
}

def tts_arr_key(a : array<float>) : uint64 {
    return 0ul if (empty(a))
    unsafe {
        return intptr(addr(a[0]))
    }
}

def tts_dec_key(d : St2Decoder) : uint64 {
    var key = 7ul + tts_lane_key()
    for (c in d.ups) {
        tts_key_fold(key, c)
    }
    for (c in d.noise_convs) {
        tts_key_fold(key, c)
    }
    for (b in d.noise_res) {
        tts_key_block(key, b)
    }
    for (b in d.resblocks) {
        tts_key_block(key, b)
    }
    tts_key_fold(key, d.conv_post)
    tts_key_fold(key, d.asr_res)
    tts_key_res(key, d.encode)
    for (b in d.decode) {
        tts_key_res(key, b)
    }
    return key
}

def tts_pred_key(p : St2Predictor) : uint64 {
    var key = hash_combine64(hash_combine64(11ul, tts_lane_key()), tts_lin_key(p.bert_encoder))
    for (l in p.lstms) {
        key = hash_combine64(key, tts_arr_key(l.fwd.w_hh))
    }
    key = hash_combine64(key, tts_arr_key(p.lstm.fwd.w_hh))
    key = hash_combine64(key, tts_arr_key(p.shared_lstm.fwd.w_hh))
    for (b in p.f0) {
        tts_key_res(key, b)
    }
    for (b in p.n) {
        tts_key_res(key, b)
    }
    return key
}

def tts_albert_key(a : St2Albert) : uint64 {
    var key = hash_combine64(hash_combine64(13ul, tts_lane_key()), tts_lin_key(a.map_in))
    key = hash_combine64(key, tts_lin_key(a.q))
    key = hash_combine64(key, tts_lin_key(a.ffn))
    return hash_combine64(key, tts_arr_key(a.word_emb.a))
}

def tts_text_key(te : St2TextEncoder) : uint64 {
    var key = hash_combine64(hash_combine64(17ul, tts_lane_key()), tts_arr_key(te.emb.a))
    for (c in te.cnn) {
        tts_key_fold(key, c)
    }
    return hash_combine64(key, tts_arr_key(te.lstm.fwd.w_hh))
}

def tts_conv_ok(c : TtsConv1d) : bool {
    return (c.groups == 1l && c.k >= 1l && c.cin >= 1l && c.cout >= 1l && c.stride >= 1l && c.dilation >= 1l
        && (c.q8 ? !empty(c.wq) : (!empty(c.w) || !empty(c.wt) || !empty(c.transposed ? c.wtr : c.wk))))
}

def tts_lin_ok(l : TtsLinear) : bool {
    return l.nin >= 1l && l.nout >= 1l && (l.q8 ? !empty(l.wq) : (l.kq_fmt != 0 ? !empty(l.kq) : (!empty(l.w) || !empty(l.wt))))
}

//! a linear the dense GEMM stamps serve: `nin` a multiple of 32, since the stamps read X's K columns to the padded width and the row buffers carry no zeroed pad
def tts_gemm_lin_ok(l : TtsLinear) : bool => tts_lin_ok(l) && l.nin % 32l == 0l

//! a BiLSTM a recurrence kernel of `max_hidden` lanes serves
def tts_lstm_ok(l : TtsBiLstm; nin, max_hidden : int64) : bool {
    let hd = l.fwd.hidden
    return (hd >= 4l && hd % 4l == 0l && hd <= max_hidden && l.bwd.hidden == hd && l.fwd.nin == nin && l.bwd.nin == nin && (2l * hd) % 32l == 0l
        && long_length(l.fwd.w_hh) >= 4l * hd * hd && long_length(l.bwd.w_hh) >= 4l * hd * hd && long_length(l.fwd.b) >= 4l * hd
        && long_length(l.fwd.w_ih) >= 4l * hd * nin && long_length(l.bwd.w_ih) >= 4l * hd * nin && long_length(l.bwd.b) >= 4l * hd)
}

def tts_res_ok(b : St2AdainResBlk) : bool {
    return (tts_conv_ok(b.conv1) && tts_conv_ok(b.conv2) && (!b.learned_sc || tts_conv_ok(b.conv1x1))
        && (!b.upsample || (b.pool.k >= 1l && long_length(b.pool.w) >= b.pool.cin * b.pool.k)))
}

//! every conv and linear a part's slab writes, checked before the writer reads them: a seat's own shape gate covers its stage, the slab covers the whole part
def tts_dec_ok(d : St2Decoder) : bool {
    for (c in d.ups) {
        return false if (!tts_conv_ok(c))
    }
    for (c in d.noise_convs) {
        return false if (!tts_conv_ok(c))
    }
    for (b in d.decode) {
        return false if (!tts_res_ok(b))
    }
    return (tts_conv_ok(d.conv_post) && tts_conv_ok(d.f0_conv) && tts_conv_ok(d.n_conv) && tts_conv_ok(d.asr_res)
        && tts_res_ok(d.encode) && tts_lin_ok(d.source_linear))
}

def tts_pred_ok(p : St2Predictor; max_hidden : int64) : bool {
    for (l in p.lstms) {
        return false if (!tts_lstm_ok(l, l.fwd.nin, max_hidden))
    }
    for (b in p.f0) {
        return false if (!tts_res_ok(b))
    }
    for (b in p.n) {
        return false if (!tts_res_ok(b))
    }
    return (tts_gemm_lin_ok(p.bert_encoder) && tts_lstm_ok(p.lstm, p.lstm.fwd.nin, max_hidden) && tts_gemm_lin_ok(p.duration_proj)
        && tts_lstm_ok(p.shared_lstm, p.shared_lstm.fwd.nin, max_hidden) && tts_conv_ok(p.f0_proj) && tts_conv_ok(p.n_proj))
}

def tts_albert_ok(a : St2Albert) : bool {
    return (tts_gemm_lin_ok(a.map_in) && tts_gemm_lin_ok(a.q) && tts_gemm_lin_ok(a.k) && tts_gemm_lin_ok(a.v) && tts_gemm_lin_ok(a.dense)
        && tts_gemm_lin_ok(a.ffn) && tts_gemm_lin_ok(a.ffn_out) && a.q.nin == a.hidden && a.k.nin == a.hidden && a.v.nin == a.hidden && a.dense.nin == a.hidden)
}

def tts_text_ok(te : St2TextEncoder; max_hidden : int64) : bool {
    for (c in te.cnn) {
        return false if (!tts_conv_ok(c))
    }
    return tts_lstm_ok(te.lstm, te.lstm.fwd.nin, max_hidden)
}

//! a transformer layer's slots: the two norms, the four linears, the layer scale rows where the layer carries them
struct TtsPkLayerSlots {
    n1 : TtsNormSlot
    in_proj : TtsConvSlot
    out_proj : TtsConvSlot
    scale1_off : int64
    has_s1 : bool
    n2 : TtsNormSlot
    ffn1 : TtsConvSlot
    ffn2 : TtsConvSlot
    scale2_off : int64
    has_s2 : bool
}

struct TtsPkCodecSlab {
    quant : TtsConvSlot
    up_w : int64          //! the depthwise upsample's taps [c][k] and bias
    up_b : int64
    layers : array<TtsPkLayerSlots>
    dec_in : TtsConvSlot
    ups : array<TtsConvSlot>
    res1 : array<TtsConvSlot>
    res2 : array<TtsConvSlot>
    dec_out : TtsConvSlot
    zero : int64          //! a zero row as wide as the transformer, the residual seam's bias
    rope_cos : int64      //! the transformer's rope tables [rope_rows][dh / 2]
    rope_sin : int64
    rope_rows : int64
}

var private g_tts_no_freqs : array<float>   //! the rope builder's absent frequency table

//! the engine's rope tables for positions 0..rows-1 at the transformer's period, cos and sin, each [rows][dh / 2]
def tts_pk_rope_tabs(tf : PocketTransformer; rows : int64; @exact_size var tcos, tsin : array<float>) {
    build_rope_tabs(tcos, tsin, tf.period, 1.0, 1.0, g_tts_no_freqs, false, 0l, rows, tf.d / tf.heads)
}

//! the same tables into the slab, cos then sin; the cos row's element offset
def tts_pk_rope_rows(var w : TtsSlabWriter; tf : PocketTransformer; rows : int64; var sin_off : int64&) : int64 {
    let half = tf.d / tf.heads / 2l
    var inscope @exact_size tcos : array<float>
    var inscope @exact_size tsin : array<float>
    tts_pk_rope_tabs(tf, rows, tcos, tsin)
    let at = tts_slab_raw(w, tcos, rows * half)
    sin_off = tts_slab_raw(w, tsin, rows * half)
    return at
}

//! the row counts a codec chain reaches for `frames`: the frames, the 200 Hz rows, each stage's rows
def tts_pk_stage_rows(mm : PocketMimi; frames : int64; @scratch var rows : array<int64>) {
    unsafe(scratch_resize(rows, 2l + long_length(mm.ratios)))
    rows[0] = frames
    rows[1] = frames * mm.frame_steps
    for (i in range64(long_length(mm.ratios))) {
        rows[2l + i] = rows[1l + i] * mm.ratios[i]
    }
}

//! a voice's residency key: the caches' addresses and shapes, and 64 sampled words of their live rows
def tts_pk_voice_key(vs : PocketVoiceState) : uint64 {
    var key = 29ul + uint64(vs.len) * 7ul
    for (kc in vs.caches) {
        key = hash_combine64(key, tts_arr_key(kc.k) + uint64(kc.cap) * 3ul)
        key = hash_combine64(hash_combine64(hash_combine64(key, tts_arr_key(kc.v)), uint64(kc.heads)), uint64(kc.dh))
        let d = kc.heads * kc.dh
        let rows = max(vs.len, 1l)
        for (i in range64(64l)) {
            let kv = kc.k[((i * 131l) % d) * kc.cap + (i * 17l) % rows]
            key = hash_combine64(key, uint64(unsafe(reinterpret<uint>(kv))))
            let vv = kc.v[(((i * 131l) % d) / kc.dh * kc.cap + (i * 17l) % rows) * kc.dh + (i * 131l) % kc.dh]
            key = hash_combine64(key, uint64(unsafe(reinterpret<uint>(vv))))
        }
    }
    return key
}

//! host rows [r0, r1) of one cache - keys [d][cap], values [heads][cap][dh] - as [pos][d] rows at kp / vp, row r0 landing at row `at`
def tts_pk_kv_rows(kc : TtsKvCache; r0, r1, at : int64; var kp, vp : float?) {
    let dh = kc.dh
    let d = kc.heads * dh
    let cap = kc.cap
    unsafe {
        for (pos in range64(r0, r1)) {
            for (h in range64(kc.heads)) {
                for (e in range64(dh)) {
                    kp[(at + pos - r0) * d + h * dh + e] = kc.k[(h * dh + e) * cap + pos]
                    vp[(at + pos - r0) * d + h * dh + e] = kc.v[(h * cap + pos) * dh + e]
                }
            }
        }
    }
}

//! the harmonic source's geometry for `f2` frames: the samples, the low-rate rows and the STFT frames; `ok` false where the decoder's source parts do not fit the stamps
struct TtsSourceShape {
    ok : bool
    samples : int64
    nlow : int64
    spec_frames : int64
}

let TTS_SRC_MAX_SAMPLES = 1l << 23   //! past it `float(to) + 0.5` rounds, and the torch law's tap coordinate parts from the CPU's double form

def tts_source_shape(d : St2Decoder; cfg : SineSourceCfg; f2 : int64) : TtsSourceShape {
    let samples = f2 * cfg.upsample
    var sh = TtsSourceShape(samples = samples, nlow = resize_len(samples, 1.0 / float(cfg.upsample)))
    return sh if (sh.samples < 1l || sh.samples >= TTS_SRC_MAX_SAMPLES || sh.nlow < 1l || cfg.n_harm < 1l || cfg.n_harm > 64l
        || resize_len(sh.nlow, float(cfg.upsample)) != sh.samples)
    let re & = unsafe(d.stft_fwd_re)
    let im & = unsafe(d.stft_fwd_im)
    return sh if (re.cin != 1l || re.cout != d.bins || re.transposed || im.k != re.k || im.cout != d.bins || im.cin != 1l || im.transposed
        || re.pad_l != 0l || re.pad_r != 0l || re.dilation != 1l || im.pad_l != 0l || im.pad_r != 0l || im.dilation != 1l || im.stride != re.stride
        || d.source_linear.nin != cfg.n_harm || d.source_linear.nout != 1l || empty(d.source_linear.b) || empty(re.w) || empty(im.w))
    sh.spec_frames = conv1d_out_len(re, sh.samples + 2l * ST2_STFT_PAD)
    sh.ok = sh.spec_frames >= 1l
    return sh
}

// The decoder's shape walks: the CPU chain's own length rules, generic over the driver's shape record `SH`. Each driver's overloads of the
// hooks - tts_note_conv, tts_note_rows, tts_note_block, tts_note_res, tts_note_post, tts_note_decoder, resolved at the driver (`_::`) - note
// what its scratch is sized by and carry that driver's own declines.

//! an AdaIN residual block over `t` rows, noted on `sh`; its output row count, or -1 on a shape the chain does not take
def tts_res_shape(var sh : auto(SH); b : St2AdainResBlk; t : int64) : int64 {
    return -1l if (b.dim_in_s % 32l != 0l || b.dim_out % 64l != 0l || b.dim_in_s < b.dim_in || !tts_conv_ok(b.conv1) || !tts_conv_ok(b.conv2)
        || b.conv1.cin != b.dim_in_s || b.conv1.cout != b.dim_out || b.conv2.cin != b.dim_out || b.conv2.cout != b.dim_out
        || (b.learned_sc && (!tts_conv_ok(b.conv1x1) || b.conv1x1.cout != b.dim_out || b.conv1x1.cin != b.dim_in_s)))
    var t1 = t
    if (b.upsample) {
        return -1l if (b.pool.groups != b.pool.cin || b.pool.cin != b.pool.cout || !b.pool.transposed || empty(b.pool.w) || b.pool.cin != b.dim_in_s)
        t1 = conv1d_out_len(b.pool, t)
        return -1l if (t1 != 2l * t)
    }
    return -1l if (conv1d_out_len(b.conv1, t1) != t1 || conv1d_out_len(b.conv2, t1) != t1 || (b.learned_sc && conv1d_out_len(b.conv1x1, t1) != t1)
        || !_::tts_note_conv(sh, b.conv1, t1, false) || !_::tts_note_conv(sh, b.conv2, t1, false)
        || (b.learned_sc && !_::tts_note_conv(sh, b.conv1x1, t1, false)) || !_::tts_note_res(sh, b, t1))
    return t1
}

//! a Snake residual block over `t` rows of `c` channels, noted on `sh`; false on a shape the chain does not take
def tts_gen_block_shape(var sh : auto(SH); b : St2GenResBlock; t, c : int64) : bool {
    let n = length(b.convs1)
    return false if (b.c != c || n == 0 || length(b.convs2) != n || length(b.adain1) != n || length(b.adain2) != n
        || length(b.alpha1) != n || length(b.alpha2) != n)
    for (j in range(n)) {
        let c1 & = unsafe(b.convs1[j])
        let c2 & = unsafe(b.convs2[j])
        return false if (!tts_conv_ok(c1) || !tts_conv_ok(c2) || c1.cin != c || c1.cout != c || c2.cin != c || c2.cout != c
            || conv1d_out_len(c1, t) != t || conv1d_out_len(c2, t) != t || long_length(b.alpha1[j].a) < c || long_length(b.alpha2[j].a) < c
            || !_::tts_note_conv(sh, c1, t, false) || !_::tts_note_conv(sh, c2, t, false))
    }
    return _::tts_note_block(sh, b, t, c)
}

//! the generator over `t` rows of `c` channels, at most `max_stages` stages: its stages noted on `sh`, conv_post's rows (`t_post`) and the last
//! stage's rows and width (`t_last`, `c_last`) set; false on a shape the chain does not take
def tts_gen_shape(d : St2Decoder; c, t, har_frames, max_stages : int64; var sh : auto(SH)) : bool {
    let nst = long_length(d.ups)
    return false if (nst == 0l || nst > max_stages || t < 1l || har_frames < 1l || d.n_kernels < 1l
        || long_length(d.noise_convs) != nst || long_length(d.noise_res) != nst || long_length(d.resblocks) != nst * d.n_kernels)
    var tt = t
    var cc = c
    _::tts_note_rows(sh, t, c)
    for (i in range64(nst)) {
        let up & = unsafe(d.ups[i])
        let nc & = unsafe(d.noise_convs[i])
        return false if (!tts_conv_ok(up) || !tts_conv_ok(nc) || up.cin != cc || nc.cin != d.noise_convs[0].cin)
        var t2 = conv1d_out_len(up, tt)
        if (i == nst - 1l) {
            t2++
        }
        let c2 = up.cout
        return false if (c2 % 64l != 0l || nc.cout != c2 || conv1d_out_len(nc, har_frames) != t2 || !tts_gen_block_shape(sh, d.noise_res[i], t2, c2))
        for (j in range64(d.n_kernels)) {
            return false if (!tts_gen_block_shape(sh, d.resblocks[i * d.n_kernels + j], t2, c2))
        }
        return false if (!_::tts_note_conv(sh, up, t2, true) || !_::tts_note_conv(sh, nc, t2, false))
        _::tts_note_rows(sh, t2, c2)
        tt = t2
        cc = c2
    }
    return false if (!tts_conv_ok(d.conv_post) || d.conv_post.cin != cc || d.conv_post.cout < 2l * d.bins)
    sh.t_post = conv1d_out_len(d.conv_post, tt)
    sh.t_last = tt
    sh.c_last = cc
    return _::tts_note_post(sh, d.conv_post, sh.t_post)
}

//! the decode chain's shape: the front convs, the AdaIN residual blocks as the concat rows ask, the source (`ss`), the generator, the inverse STFT
//! (`n_out`); false on a shape the chain does not take
def tts_decode_shape(d : St2Decoder; text_c : int64; source : SineSourceCfg; frames, f2 : int64; noise : TtsNoise; max_stages : int64; var sh : auto(SH)) : bool {
    let bwd & = unsafe(d.stft_bwd_re)
    sh.ss = tts_source_shape(d, source, f2)
    return false if (frames < 1l || f2 < 1l || text_c % 32l != 0l || !sh.ss.ok || !tts_dec_ok(d) || d.f0_conv.cin != 1l || d.n_conv.cin != 1l
        || d.f0_conv.cout != 1l || d.n_conv.cout != 1l || conv1d_out_len(d.f0_conv, f2) != frames || conv1d_out_len(d.n_conv, f2) != frames
        || d.asr_res.cin != text_c || d.asr_res.cout % 64l != 0l || conv1d_out_len(d.asr_res, frames) != frames || d.encode.dim_in != text_c + 2l
        || bwd.cin != d.bins || bwd.cout != 1l || !bwd.transposed || empty(bwd.wt) || d.stft_bwd_im.k != bwd.k
        || (d.istft_envelope && long_length(d.window.a) < bwd.k) || (noise.captured && long_length(noise.normal) < sh.ss.samples * source.n_harm)
        || long_length(noise.uniform) < source.n_harm || !_::tts_note_decoder(sh, d) || !_::tts_note_conv(sh, d.f0_conv, frames, false)
        || !_::tts_note_conv(sh, d.n_conv, frames, false) || !_::tts_note_conv(sh, d.asr_res, frames, false))
    _::tts_note_rows(sh, frames, max(text_c, d.encode.dim_in_s))
    sh.dec_rows = tts_res_shape(sh, d.encode, frames)
    return false if (sh.dec_rows < 0l)
    sh.dec_width = d.encode.dim_out
    for (b in d.decode) {
        return false if ((b.dim_in != sh.dec_width && b.dim_in != sh.dec_width + d.asr_res.cout + 2l) || (b.dim_in == sh.dec_width && b.dim_in_s != b.dim_in))
        sh.dec_rows = tts_res_shape(sh, b, sh.dec_rows)
        return false if (sh.dec_rows < 0l)
        sh.dec_width = b.dim_out
    }
    return false if (!tts_gen_shape(d, sh.dec_width, sh.dec_rows, sh.ss.spec_frames, max_stages, sh))
    sh.n_out = conv1d_out_len(bwd, sh.t_post) - 2l * ST2_STFT_PAD
    return sh.n_out >= 1l
}

//! fc(style) as a norm's rows at `dst`: gamma [c] then beta [c], gamma + 1 where the norm applies it as a layernorm weight (an AdaLN
//! site); `tmp` the fc's output scratch
def tts_style_rows(fc : TtsLinear; s : array<float>; c : int64; ln_form : bool; @scratch @exact_size var tmp : array<float>; var dst : float?) {
    linear_vec(fc, s, tmp)
    unsafe {
        memcpy(dst, addr(tmp[0]), 2l * c * 4l)
        if (ln_form) {
            for (i in range64(c)) {
                dst[i] = dst[i] + 1.0
            }
        }
    }
}

//! a row of `n` zeros - an allocation alone, since the fill pass's host starts zeroed (`tts_slab_build`); the element offset
def tts_slab_zero(var w : TtsSlabWriter; n : int64) : int64 => tsw_alloc(w, n)

//! the head's residual block: its norm rows and three linears
struct TtsPkHeadSlots {
    ln : TtsNormSlot
    adaln : TtsConvSlot
    mlp1 : TtsConvSlot
    mlp2 : TtsConvSlot
}

struct TtsPkFramesSlab {
    input_linear : TtsConvSlot
    layers : array<TtsPkLayerSlots>
    out_norm : TtsNormSlot
    out_eos : TtsConvSlot
    cond_embed : TtsConvSlot
    time_const : int64
    input_proj : TtsConvSlot
    blocks : array<TtsPkHeadSlots>
    final_ln : TtsNormSlot
    final_adaln : TtsConvSlot
    final_linear : TtsConvSlot
    std_mean : int64      //! emb_std then emb_mean, one row of 2 x latent_dim
    bos : int64
    zero : int64          //! a zero row as wide as the backbone, the residual seams' bias
}

//! a transformer layer's slots written: the two norms, the four linears, the layer scale rows where the layer carries them
def tts_pk_layer_write(var w : TtsSlabWriter; l : PocketLayer; d : int64) : TtsPkLayerSlots {
    var s = TtsPkLayerSlots(n1 = tts_slab_norm_rows(w, l.norm1, d), in_proj = tts_slab_linear(w, l.in_proj), out_proj = tts_slab_linear(w, l.out_proj),
        n2 = tts_slab_norm_rows(w, l.norm2, d), ffn1 = tts_slab_linear(w, l.ffn1), ffn2 = tts_slab_linear(w, l.ffn2))
    s.has_s1 = !empty(l.scale1.a)
    s.has_s2 = !empty(l.scale2.a)
    if (s.has_s1) {
        s.scale1_off = tts_slab_raw(w, l.scale1.a, d)
    }
    if (s.has_s2) {
        s.scale2_off = tts_slab_raw(w, l.scale2.a, d)
    }
    return s
}

//! the codec decoder's slab, the rope tables sized for `rope_rows` positions (the one-shot chain's row budget)
def tts_pk_codec_write(var w : TtsSlabWriter; mm : PocketMimi; rope_rows : int64; var s : TtsPkCodecSlab) {
    s.zero = tts_slab_zero(w, max(mm.dec_tf.d, 32l))
    s.rope_rows = rope_rows
    s.rope_cos = tts_pk_rope_rows(w, mm.dec_tf, rope_rows, s.rope_sin)
    s.quant = tts_slab_conv(w, mm.quant_proj)
    s.up_w = tts_slab_raw(w, mm.upsample.w, mm.upsample.cin * mm.upsample.k)
    s.up_b = tts_slab_raw(w, mm.upsample.b, mm.upsample.cin)
    s.layers |> reserve(length(mm.dec_tf.layers))
    for (l in mm.dec_tf.layers) {
        s.layers |> push(tts_pk_layer_write(w, l, mm.dec_tf.d))
    }
    s.dec_in = tts_slab_conv(w, mm.dec_in)
    s.ups |> reserve(length(mm.dec_up))
    s.res1 |> reserve(length(mm.dec_res))
    s.res2 |> reserve(length(mm.dec_res))
    for (c in mm.dec_up) {
        s.ups |> push(tts_slab_conv(w, c))
    }
    for (b in mm.dec_res) {
        s.res1 |> push(tts_slab_conv(w, b.conv1))
        s.res2 |> push(tts_slab_conv(w, b.conv2))
    }
    s.dec_out = tts_slab_conv(w, mm.dec_out)
}

def tts_pk_frames_write(var w : TtsSlabWriter; m : PocketModel; var s : TtsPkFramesSlab) {
    let d = m.backbone.d
    let ld = m.latent_dim
    s.zero = tts_slab_zero(w, max(d, 32l))
    s.input_linear = tts_slab_linear(w, m.input_linear)
    s.layers |> reserve(length(m.backbone.layers))
    for (l in m.backbone.layers) {
        s.layers |> push(tts_pk_layer_write(w, l, d))
    }
    s.out_norm = tts_slab_norm_rows(w, m.out_norm, d)
    s.out_eos = tts_slab_linear(w, m.out_eos)
    let h & = unsafe(m.head)
    s.cond_embed = tts_slab_linear(w, h.cond_embed)
    s.time_const = tts_slab_raw(w, h.time_const.a, h.dim)
    s.input_proj = tts_slab_linear(w, h.input_proj)
    s.blocks |> reserve(length(h.blocks))
    for (b in h.blocks) {
        s.blocks |> push(TtsPkHeadSlots(ln = tts_slab_norm_rows(w, b.ln, h.dim), adaln = tts_slab_linear(w, b.adaln), mlp1 = tts_slab_linear(w, b.mlp1),
            mlp2 = tts_slab_linear(w, b.mlp2)))
    }
    s.final_ln = tts_slab_norm_rows(w, h.final_ln, h.dim)
    s.final_adaln = tts_slab_linear(w, h.final_adaln)
    s.final_linear = tts_slab_linear(w, h.final_linear)
    let sm = tsw_alloc(w, 2l * ld)
    for (j in range64(ld)) {
        tsw_put(w, sm + j, m.emb_std.a[j])
        tsw_put(w, sm + ld + j, m.emb_mean.a[j])
    }
    s.std_mean = sm
    s.bos = tts_slab_raw(w, m.bos_emb.a, ld)
}

def private tts_pk_layer_key(var key : uint64&; l : PocketLayer) {
    key = hash_combine64(key, tts_lin_key(l.in_proj))
    key = hash_combine64(key, tts_lin_key(l.out_proj))
    key = hash_combine64(key, tts_lin_key(l.ffn1))
    key = hash_combine64(key, tts_lin_key(l.ffn2))
    key = hash_combine64(key, tts_arr_key(l.norm1.w))
    key = hash_combine64(key, tts_arr_key(l.scale1.a))
}

def tts_pk_codec_key(mm : PocketMimi) : uint64 {
    var key = hash_combine64(hash_combine64(19ul, tts_lane_key()), tts_conv_key(mm.quant_proj))
    key = hash_combine64(key, tts_arr_key(mm.upsample.w))
    for (l in mm.dec_tf.layers) {
        tts_pk_layer_key(key, l)
    }
    tts_key_fold(key, mm.dec_in)
    for (c in mm.dec_up) {
        tts_key_fold(key, c)
    }
    for (b in mm.dec_res) {
        tts_key_fold(key, b.conv1)
        tts_key_fold(key, b.conv2)
    }
    tts_key_fold(key, mm.dec_out)
    return key
}

def tts_pk_frames_key(m : PocketModel) : uint64 {
    var key = hash_combine64(hash_combine64(23ul, tts_lane_key()), tts_lin_key(m.input_linear))
    key = hash_combine64(hash_combine64(hash_combine64(key, uint64(m.backbone.d)), uint64(m.backbone.heads)), uint64(m.latent_dim))
    key = hash_combine64(hash_combine64(key, uint64(m.backbone.period)), uint64(long_length(m.backbone.layers)))
    for (l in m.backbone.layers) {
        tts_pk_layer_key(key, l)
    }
    key = hash_combine64(key, tts_arr_key(m.out_norm.w))
    key = hash_combine64(key, tts_lin_key(m.out_eos))
    let h & = unsafe(m.head)
    key = hash_combine64(key, tts_lin_key(h.cond_embed))
    key = hash_combine64(key, tts_arr_key(h.time_const.a))
    key = hash_combine64(key, tts_lin_key(h.input_proj))
    for (b in h.blocks) {
        key = hash_combine64(key, tts_arr_key(b.ln.w))
        key = hash_combine64(key, tts_lin_key(b.adaln))
        key = hash_combine64(key, tts_lin_key(b.mlp1))
        key = hash_combine64(key, tts_lin_key(b.mlp2))
    }
    key = hash_combine64(key, tts_lin_key(h.final_adaln))
    key = hash_combine64(key, tts_lin_key(h.final_linear))
    key = hash_combine64(key, tts_arr_key(m.emb_std.a))
    return hash_combine64(key, tts_arr_key(m.bos_emb.a))
}

//! a codec conv the im2col route serves: dense, not itself quantized, a transposed conv's taps past its stride (the padding the one-shot row count assumes)
def tts_pk_conv_ok(c : TtsConv1d) : bool => tts_conv_ok(c) && !c.q8 && (!c.transposed || c.k > c.stride)

def tts_pk_layer_dims_ok(l : PocketLayer; d, ffn : int64) : bool {
    return (l.in_proj.nin == d && l.in_proj.nout == 3l * d && l.out_proj.nin == d && l.out_proj.nout == d
        && l.ffn1.nin == d && l.ffn1.nout == ffn && l.ffn2.nin == ffn && l.ffn2.nout == d
        && long_length(l.norm1.w) == d && long_length(l.norm2.w) == d)
}

def tts_pk_layer_ok(l : PocketLayer; d, ffn : int64) : bool {
    return tts_pk_layer_dims_ok(l, d, ffn) && tts_gemm_lin_ok(l.in_proj) && tts_gemm_lin_ok(l.out_proj) && tts_gemm_lin_ok(l.ffn1) && tts_gemm_lin_ok(l.ffn2)
}

//! a transformer the row kernels serve: the width on the 32 lattice, heads of `hs`, the ffn on the 32 lattice
def tts_pk_tf_ok(tf : PocketTransformer; hs : int64) : bool {
    let d = tf.d
    return d >= 32l && d % 32l == 0l && tf.heads >= 1l && d % tf.heads == 0l && d / tf.heads == hs && tf.ffn % 32l == 0l
}

def tts_pk_codec_ok(mm : PocketMimi; hs, max_keys : int64) : bool {
    let tf & = unsafe(mm.dec_tf)
    let d = tf.d
    return false if (!tts_pk_tf_ok(tf, hs) || tf.context < 1l || tf.context > max_keys)
    for (l in tf.layers) {
        return false if (!tts_pk_layer_ok(l, d, tf.ffn))
    }
    let up & = unsafe(mm.upsample)
    return false if (!up.transposed || up.groups != up.cin || up.cin != up.cout || up.cin != d || mm.frame_steps != up.stride || empty(up.w)
        || !tts_pk_conv_ok(mm.quant_proj) || mm.quant_proj.cout != d || !tts_pk_conv_ok(mm.dec_in) || mm.dec_in.cin != d
        || length(mm.dec_up) != length(mm.ratios) || length(mm.dec_res) != length(mm.ratios))
    for (i, c in iter_range(mm.dec_up), mm.dec_up) {
        return false if (!tts_pk_conv_ok(c) || !c.transposed || c.stride != mm.ratios[i] || !tts_pk_conv_ok(mm.dec_res[i].conv1) || !tts_pk_conv_ok(mm.dec_res[i].conv2)
            || mm.dec_res[i].conv1.cin != c.cout || mm.dec_res[i].conv2.cout != c.cout)
    }
    return tts_pk_conv_ok(mm.dec_out) && mm.dec_out.cout >= 1l
}

//! the frame loop's shapes - the backbone's and the head's, every width within the GEMV's `max_nin` stage - with each linear one the
//! driver's GEMV serves (`lin_ok`)
def tts_pk_frames_ok(m : PocketModel; hs, max_nin : int64; lin_ok : block<(l : TtsLinear) : bool>) : bool {
    let tf & = unsafe(m.backbone)
    let d = tf.d
    let ld = m.latent_dim
    let h & = unsafe(m.head)
    let n = h.dim
    return false if (!tts_pk_tf_ok(tf, hs) || d > max_nin || n > max_nin || tf.ffn > max_nin || ld > max_nin
        || ld < 4l || ld % 4l != 0l || n < 1l || empty(h.blocks) || long_length(h.time_const.a) != n || long_length(m.emb_std.a) != ld
        || long_length(m.emb_mean.a) != ld || long_length(m.bos_emb.a) != ld || long_length(m.out_norm.w) != d
        || !invoke(lin_ok, m.input_linear) || m.input_linear.nin != ld || m.input_linear.nout != d || !invoke(lin_ok, m.out_eos) || m.out_eos.nin != d || m.out_eos.nout != 1l)
    for (l in tf.layers) {
        return false if (!tts_pk_layer_dims_ok(l, d, tf.ffn) || !invoke(lin_ok, l.in_proj) || !invoke(lin_ok, l.out_proj) || !invoke(lin_ok, l.ffn1) || !invoke(lin_ok, l.ffn2))
    }
    return false if (!invoke(lin_ok, h.cond_embed) || h.cond_embed.nin != d || h.cond_embed.nout != n || !invoke(lin_ok, h.input_proj) || h.input_proj.nin != ld || h.input_proj.nout != n)
    for (b in h.blocks) {
        return false if (!invoke(lin_ok, b.adaln) || b.adaln.nin != n || b.adaln.nout != 3l * n || !invoke(lin_ok, b.mlp1) || b.mlp1.nin != n || b.mlp1.nout != n
            || !invoke(lin_ok, b.mlp2) || b.mlp2.nin != n || b.mlp2.nout != n)
    }
    return invoke(lin_ok, h.final_adaln) && h.final_adaln.nin == n && h.final_adaln.nout == 2l * n && invoke(lin_ok, h.final_linear) && h.final_linear.nin == n && h.final_linear.nout == ld
}

//! a frames call the voice's caches hold: frames asked, a cache a backbone layer, the voice, the chunk's text rows and the frames within the
//! caches' capacity, the capacity within the attention row's `max_keys`
def tts_pk_frames_admit(m : PocketModel; vs : PocketVoiceState; n_txt, max_frames, max_keys : int64) : bool {
    return (max_frames >= 1l && n_txt >= 0l && !empty(vs.caches) && length(vs.caches) == length(m.backbone.layers)
        && vs.len + n_txt + max_frames <= vs.caches[0].cap && vs.caches[0].cap <= max_keys)
}

//! the conditioning rows a frames call reads back after `run` frames ran: every frame made and the breaking frame's row, as the CPU loop leaves them
def tts_pk_cond_rows(frames, run : int64) : int64 => min(frames + 1l, run)

//! a voice slot's residency: the key over the caches, their capacity and fill, the backbone width
struct TtsPkVoiceMeta {
    key : uint64
    cap : int64
    len : int64
    d : int64
}

//! the slot's meta for this voice at the backbone width `d`
def tts_pk_voice_meta(vs : PocketVoiceState; d : int64) : TtsPkVoiceMeta
    => TtsPkVoiceMeta(key = tts_pk_voice_key(vs), cap = vs.caches[0].cap, len = vs.len, d = d)

//! a slot holding `layers` layers of K/V rows stands for this voice: the same key, capacity and fill, a layer a cache
def tts_pk_voice_current(meta : TtsPkVoiceMeta; vs : PocketVoiceState; layers : int) : bool
    => meta.key == tts_pk_voice_key(vs) && meta.cap == vs.caches[0].cap && meta.len == vs.len && layers == length(vs.caches)
