options gen2
options indenting = 4
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_tts_slab shared public

require dasllama/dasllama_lint public
require math
require daslib/f16_cvt
require dasllama/dasllama_kqformat
require dasllama/dasllama_layout
require dasllama/dasllama_math
require dasllama/dasllama_convert
require dasllama/dasllama_tts_blocks
require dasllama/dasllama_styletts2

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
}

//! n rounded up to a multiple of m - the tile lattices the GEMM panels and the row pads take
def tts_round_up(n, m : int64) : int64 => (n + m - 1l) / m * m

//! `count` floats at the next 16-byte boundary (every item is a float4 read for some kernel); the element offset
def tsw_alloc(var w : TtsSlabWriter; count : int64) : int64 {
    let at = tts_round_up(w.n, 4l)
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
    fhh : int64         //! ... and its dense recurrence [4H][H]
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

//! q8 rows dequantized into the slab rows [rows][stride]: the quants gathered through the active repack, so the
//! served lane's numbers land, not the disk order's
def private tsw_put_q8(var w : TtsSlabWriter; q : array<int8>; qs : array<float>; rows, cols, w0, stride : int64) {
    return if (w.measure)
    var wq : array<uint8>
    var ws : array<uint8>
    wq |> reserve_resize(rows * cols)
    ws |> reserve_resize(rows * cols / 32l * 2l)
    let lay = active_q8_repack_layout(g_active_needs_repack)
    unsafe {
        q8_gather_rows(addr<int8 const?>(q[0]), null, addr<float const?>(qs[0]), cols, rows, rows, lay.mr, lay.kgroup, lay.wbias, wq, ws)
        let sp = addr<uint16 const?>(ws[0])
        for (r in range64(rows)) {
            for (j in range64(cols)) {
                w.host[w0 + r * stride + j] = float(int8(wq[r * cols + j])) * f16_to_f32(uint(sp[(r * cols + j) / 32l]))
            }
        }
    }
}

//! a K-quant linear dequantized into the slab rows [nout][stride], a row at a time through the active layout's group gather
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
    var s = TtsConvSlot(cout_p = tts_round_up(c.cout, 64l), kdim_p = tts_round_up(kc, 32l), gathers = c.cin % 8l == 0l)
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
    var s = TtsConvSlot(cout_p = tts_round_up(l.nout, 64l), kdim_p = tts_round_up(l.nin, 32l), gathers = false)
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

def private tts_slab_lstm_dir(var w : TtsSlabWriter; l : TtsLstm; var xs : TtsConvSlot; var hh : int64&) {
    var lin = TtsLinear(nin = l.nin, nout = 4l * l.hidden)
    lin.w := l.w_ih
    lin.b := l.b
    xs = tts_slab_linear(w, lin)
    hh = tts_slab_raw(w, l.w_hh, 4l * l.hidden * l.hidden)
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

//! the part's writer run twice - a measuring pass sizes the host copy, the second fills it and the slot record; the
//! host floats come back for the backend to upload (empty when the part carries nothing)
def tts_slab_build(var slots : auto(SS); write : block<(var w : TtsSlabWriter; var s : SS) : void>) : array<float> {
    var inscope w = TtsSlabWriter(measure = true)
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

// ----- keys: the parts' weight addresses folded with the q8 lane's repack layout -----

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

// ----- the shape checks a part's writer needs to hold before it reads the weights -----

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
