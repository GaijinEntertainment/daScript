options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_tts_blocks shared public

require dasllama/dasllama_lint public
require math
require daslib/archive
require dasllama/dasllama_math
require dasllama/dasllama_par
require dasllama/dasllama_plane
require dasllama/dasllama_convert
require dasllama/dasllama_tower
require dasllama/dasllama_common

//! The StyleTTS2-lineage block home: every operator the TTS families assemble, on f32
//! buffers in one of two layouts - channel-major [C][T] (the reference form, the oracle the
//! parity rail and the block test hold the other form to) or token-major [T][C] (the "rows"
//! form the generator runs: tap-GEMMs on the tiled kernel, row blocks across lanes). One home:
//! a family file re-implementing one of these is a defect.

// ===== weights and the blob =====

//! Where a weight lives in the model's blob plane: `off` floats in, `n` floats long. Beside
//! every weight array: on a served model the array is a borrowed view over that range (bound
//! at post-load, forgotten at finalize); on a staging model it is empty, the bytes sit in the
//! staging blob. An empty span (`n == 0`) is an absent weight.
struct TtsSpan {
    off : int64 = -1l
    n : int64
}

let BLOB_ALIGN = 64l   // floats: every span starts on a 256-byte boundary, a legal GPU buffer offset

//! Append `src` to a staging blob on the alignment boundary; the span it landed in.
def blob_push(var blob : array<float>; src : array<float>) : TtsSpan {
    let n = long_length(src)
    return TtsSpan() if (n == 0l)
    let off = (long_length(blob) + BLOB_ALIGN - 1l) / BLOB_ALIGN * BLOB_ALIGN
    blob |> ensure_capacity(off + n)
    blob |> resize(off + n)
    for (i in range64(n)) {
        blob[off + i] = src[i]
    }
    return TtsSpan(off = off, n = n)
}

//! Bind `a` as a borrowed view over `span` of a served blob.
def bind_span(blob : PlaneF; span : TtsSpan; var a : array<float>) {
    return if (span.n == 0l)
    if (span.off < 0l || span.n > blob.n || span.off > blob.n - span.n) {
        panic("dasLLAMA tts: weight span {span.off}+{span.n} lies outside the {blob.n}-float blob")
    }
    unsafe {
        _builtin_make_temp_array_i64(a, reinterpret<void?>(plane_at(blob, span.off)), span.n)
    }
}

//! Forget a borrowed view or delete an owned buffer - the one teardown for a weight array.
def release_weight(var a : array<float>) {
    if (lock_count(a) != 0) {
        unsafe {
            _builtin_forget_temp_array(a)
        }
    } else {
        delete a
    }
}

//! The blob walk's mode: staging moves each weight into `blob` (int8 quants into `qblob`) and
//! records its span; serving binds each weight as a view over `plane` / `qplane` at its span.
struct TtsBlobIo {
    staging : bool
    blob : array<float>?
    qblob : array<int8>?
    plane : PlaneF = PlaneF()
    qplane : PlaneI8 = PlaneI8()
}

//! One weight through the walk (see TtsBlobIo).
def weight_slot(var io : TtsBlobIo; var a : array<float>; var span : TtsSpan) {
    if (io.staging) {
        span = blob_push(*io.blob, a)
        delete a
    } else {
        bind_span(io.plane, span, a)
    }
}

//! `blob_push` for the int8 quant blob (a span counts int8 elements there).
def blob_push_q8(var qblob : array<int8>; src : array<int8>) : TtsSpan {
    let n = long_length(src)
    return TtsSpan() if (n == 0l)
    let off = (long_length(qblob) + BLOB_ALIGN - 1l) / BLOB_ALIGN * BLOB_ALIGN
    qblob |> ensure_capacity(off + n)
    qblob |> resize(off + n)
    for (i in range64(n)) {
        qblob[off + i] = src[i]
    }
    return TtsSpan(off = off, n = n)
}

//! `bind_span` over the int8 quant plane.
def bind_span_q8(qplane : PlaneI8; span : TtsSpan; var a : array<int8>) {
    return if (span.n == 0l)
    if (span.off < 0l || span.n > qplane.n || span.off > qplane.n - span.n) {
        panic("dasLLAMA tts: quant span {span.off}+{span.n} lies outside the {qplane.n}-byte quant blob")
    }
    unsafe {
        _builtin_make_temp_array_i64(a, reinterpret<void?>(plane_at(qplane, span.off)), span.n)
    }
}

//! `release_weight` for an int8 quant array.
def release_weight_q8(var a : array<int8>) {
    if (lock_count(a) != 0) {
        unsafe {
            _builtin_forget_temp_array(a)
        }
    } else {
        delete a
    }
}

//! `weight_slot` for an int8 quant array.
def weight_slot_q8(var io : TtsBlobIo; var a : array<int8>; var span : TtsSpan) {
    if (io.staging) {
        span = blob_push_q8(*io.qblob, a)
        delete a
    } else {
        bind_span_q8(io.qplane, span, a)
    }
}

//! A bare weight vector (an embedding table, a Snake alpha, the ISTFT window) with its span.
struct TtsWeight {
    a : array<float>
    span : TtsSpan = TtsSpan()
}

def serialize(var arch : Archive; var w : TtsWeight) {
    arch |> serialize(w.span)
}

def finalize(var w : TtsWeight) {
    release_weight(w.a)
}

def weights_walk(var io : TtsBlobIo; var w : TtsWeight) {
    weight_slot(io, w.a, w.span)
}

//! One 1-D convolution; `w` is the ONNX layout ([cout][cin][k] forward, [cin][cout][k] transposed)
//! and `conv1d_prepare` mints the one served layout its consumer reads (see the layouts below)
//! - `served_rows` names the consumer: the rows form or the channel-major form.
struct TtsConv1d {
    w : array<float>     // ONNX layout - the channel-major forms' weight (depthwise, im2col)
    wt : array<float>    // [cout*k][cin] - the channel-major transposed conv's GEMM form
    wk : array<float>    // [k][cin][cout_s] - the rows form's per-tap B operand, cout_s on the 16-column tile
    wtr : array<float>   // [cin][k*cout] - the rows form's transposed-conv B operand
    wq : array<int8>     // [cout][k*cin] Q8_0 quants, the taps stacked along K, repacked for the backend - the q8 rows form's W
    wqs : array<float>   // the Q8_0 scales of wq, one per 32
    b : array<float>
    w_span : TtsSpan = TtsSpan()
    wt_span : TtsSpan = TtsSpan()
    wk_span : TtsSpan = TtsSpan()
    wtr_span : TtsSpan = TtsSpan()
    wq_span : TtsSpan = TtsSpan()
    wqs_span : TtsSpan = TtsSpan()
    b_span : TtsSpan = TtsSpan()
    cout : int64
    cout_s : int64
    cin : int64
    k : int64
    stride : int64 = 1l
    pad_l : int64
    pad_r : int64
    dilation : int64 = 1l
    groups : int64 = 1l
    output_padding : int64
    transposed : bool
    served_rows : bool
    q8 : bool
    q8_n : int64         // the width of a wq row, k*cin - the K of the stacked GEMM
}

def serialize(var arch : Archive; var c : TtsConv1d) {
    arch |> serialize(c.w_span)
    arch |> serialize(c.wt_span)
    arch |> serialize(c.wk_span)
    arch |> serialize(c.wtr_span)
    arch |> serialize(c.wq_span)
    arch |> serialize(c.wqs_span)
    arch |> serialize(c.b_span)
    arch |> serialize_raw(c.cout)
    arch |> serialize_raw(c.cout_s)
    arch |> serialize_raw(c.cin)
    arch |> serialize_raw(c.k)
    arch |> serialize_raw(c.stride)
    arch |> serialize_raw(c.pad_l)
    arch |> serialize_raw(c.pad_r)
    arch |> serialize_raw(c.dilation)
    arch |> serialize_raw(c.groups)
    arch |> serialize_raw(c.output_padding)
    arch |> serialize_raw(c.transposed)
    arch |> serialize_raw(c.served_rows)
    arch |> serialize_raw(c.q8)
    arch |> serialize_raw(c.q8_n)
}

def finalize(var c : TtsConv1d) {
    release_weight(c.w)
    release_weight(c.wt)
    release_weight(c.wk)
    release_weight(c.wtr)
    release_weight_q8(c.wq)
    release_weight(c.wqs)
    release_weight(c.b)
}

def weights_walk(var io : TtsBlobIo; var c : TtsConv1d) {
    weight_slot(io, c.w, c.w_span)
    weight_slot(io, c.wt, c.wt_span)
    weight_slot(io, c.wk, c.wk_span)
    weight_slot(io, c.wtr, c.wtr_span)
    weight_slot_q8(io, c.wq, c.wq_span)
    weight_slot(io, c.wqs, c.wqs_span)
    weight_slot(io, c.b, c.b_span)
}

//! A dense layer, w [nout][nin] with an optional bias; `wt` [nin][nout] is the tiled GEMM's B
//! operand, minted by `linear_prepare` when `nout` sits on the 16-column tile.
struct TtsLinear {
    w : array<float>
    wt : array<float>
    wq : array<int8>     // [nout][nin] Q8_0 quants, repacked for the backend - the q8 rows form's W
    wqs : array<float>
    b : array<float>
    w_span : TtsSpan = TtsSpan()
    wt_span : TtsSpan = TtsSpan()
    wq_span : TtsSpan = TtsSpan()
    wqs_span : TtsSpan = TtsSpan()
    b_span : TtsSpan = TtsSpan()
    nout : int64
    nin : int64
    q8 : bool
}

def serialize(var arch : Archive; var l : TtsLinear) {
    arch |> serialize(l.w_span)
    arch |> serialize(l.wt_span)
    arch |> serialize(l.wq_span)
    arch |> serialize(l.wqs_span)
    arch |> serialize(l.b_span)
    arch |> serialize_raw(l.nout)
    arch |> serialize_raw(l.nin)
    arch |> serialize_raw(l.q8)
}

def finalize(var l : TtsLinear) {
    release_weight(l.w)
    release_weight(l.wt)
    release_weight_q8(l.wq)
    release_weight(l.wqs)
    release_weight(l.b)
}

def weights_walk(var io : TtsBlobIo; var l : TtsLinear) {
    weight_slot(io, l.w, l.w_span)
    weight_slot(io, l.wt, l.wt_span)
    weight_slot_q8(io, l.wq, l.wq_span)
    weight_slot(io, l.wqs, l.wqs_span)
    weight_slot(io, l.b, l.b_span)
}

//! A normalization's affine pair; empty arrays mean identity.
struct TtsNorm {
    w : array<float>
    b : array<float>
    w_span : TtsSpan = TtsSpan()
    b_span : TtsSpan = TtsSpan()
}

def serialize(var arch : Archive; var n : TtsNorm) {
    arch |> serialize(n.w_span)
    arch |> serialize(n.b_span)
}

def finalize(var n : TtsNorm) {
    release_weight(n.w)
    release_weight(n.b)
}

def weights_walk(var io : TtsBlobIo; var n : TtsNorm) {
    weight_slot(io, n.w, n.w_span)
    weight_slot(io, n.b, n.b_span)
}

//! One LSTM direction, gates in i,f,g,o order: w_ih [4H][nin], w_hh [4H][H], b [4H] (the two
//! bias halves summed at conversion).
struct TtsLstm {
    w_ih : array<float>
    w_hh : array<float>
    b : array<float>
    w_ih_span : TtsSpan = TtsSpan()
    w_hh_span : TtsSpan = TtsSpan()
    b_span : TtsSpan = TtsSpan()
    nin : int64
    hidden : int64
}

def serialize(var arch : Archive; var l : TtsLstm) {
    arch |> serialize(l.w_ih_span)
    arch |> serialize(l.w_hh_span)
    arch |> serialize(l.b_span)
    arch |> serialize_raw(l.nin)
    arch |> serialize_raw(l.hidden)
}

def finalize(var l : TtsLstm) {
    release_weight(l.w_ih)
    release_weight(l.w_hh)
    release_weight(l.b)
}

def weights_walk(var io : TtsBlobIo; var l : TtsLstm) {
    weight_slot(io, l.w_ih, l.w_ih_span)
    weight_slot(io, l.w_hh, l.w_hh_span)
    weight_slot(io, l.b, l.b_span)
}

struct TtsBiLstm {
    fwd : TtsLstm = TtsLstm()
    bwd : TtsLstm = TtsLstm()
}

def weights_walk(var io : TtsBlobIo; var l : TtsBiLstm) {
    weights_walk(io, l.fwd)
    weights_walk(io, l.bwd)
}

//! AdaIN1d: instance norm over time per channel, then (1 + gamma) * x + beta with gamma and
//! beta the two halves of fc(style).
struct TtsAdaIn {
    fc : TtsLinear = TtsLinear()
    norm : TtsNorm = TtsNorm()
}

def weights_walk(var io : TtsBlobIo; var a : TtsAdaIn) {
    weights_walk(io, a.fc)
    weights_walk(io, a.norm)
}

//! AdaLayerNorm: layer norm over channels per position, then the same affine from fc(style).
struct TtsAdaLayerNorm {
    fc : TtsLinear = TtsLinear()
}

def weights_walk(var io : TtsBlobIo; var a : TtsAdaLayerNorm) {
    weights_walk(io, a.fc)
}

// ===== sizing =====

//! Output length of `c` over `t_in` positions.
def conv1d_out_len(c : TtsConv1d; t_in : int64) : int64 {
    if (c.transposed) {
        return (t_in - 1l) * c.stride - c.pad_l - c.pad_r + c.dilation * (c.k - 1l) + c.output_padding + 1l
    }
    return (t_in + c.pad_l + c.pad_r - c.dilation * (c.k - 1l) - 1l) / c.stride + 1l
}

//! Pad a forward conv's input channels to `cin_s` with zero weights, the ONNX layout kept (a
//! depthwise conv pads its channel count and bias with it), so the conv reads rows padded to
//! the same width - the bake-time transform that puts a concat width on the q8 lane's 32.
def conv1d_pad_cin(var c : TtsConv1d; cin_s : int64) {
    return if (cin_s == c.cin)
    let depthwise = c.groups != 1l
    if (cin_s < c.cin || (depthwise && c.groups != c.cin) || (c.transposed && !depthwise)) {
        panic("dasLLAMA tts: conv1d_pad_cin takes a forward dense conv or a depthwise one and a wider cin; got cin={c.cin} -> {cin_s} groups={c.groups} transposed={c.transposed}")
    }
    let k = c.k
    var inscope w : array<float>
    w |> reserve_resize((depthwise ? cin_s : c.cout * cin_s) * k)
    for (v in w) {
        v = 0.0
    }
    if (depthwise) {
        for (i in range64(c.cin * k)) {
            w[i] = c.w[i]
        }
        if (!empty(c.b)) {
            c.b |> reserve_resize(cin_s)
            for (i in range64(c.cin, cin_s)) {
                c.b[i] = 0.0
            }
        }
        c.cout = cin_s
        c.groups = cin_s
    } else {
        for (co in range64(c.cout)) {
            for (ci in range64(c.cin)) {
                for (kk in range64(k)) {
                    w[(co * cin_s + ci) * k + kk] = c.w[(co * c.cin + ci) * k + kk]
                }
            }
        }
    }
    c.cin = cin_s
    c.w <- w
}

//! Pad an AdaIN from `c` to `cs` channels: the two halves of `fc` re-laid at the padded width
//! with zero gamma and beta rows for the pad, zero norm scale and shift - so a padded channel
//! normalizes to exactly zero.
def adain_pad(var a : TtsAdaIn; c, cs : int64) {
    return if (cs == c)
    let nin = a.fc.nin
    var inscope w : array<float>
    w |> reserve_resize(2l * cs * nin)
    for (v in w) {
        v = 0.0
    }
    var inscope b : array<float>
    b |> reserve_resize(2l * cs)
    for (v in b) {
        v = 0.0
    }
    for (half in range64(2l)) {
        for (ci in range64(c)) {
            for (i in range64(nin)) {
                w[(half * cs + ci) * nin + i] = a.fc.w[(half * c + ci) * nin + i]
            }
            if (!empty(a.fc.b)) {
                b[half * cs + ci] = a.fc.b[half * c + ci]
            }
        }
    }
    a.fc.w <- w
    if (!empty(a.fc.b)) {
        a.fc.b <- b
    }
    a.fc.nout = 2l * cs
    delete a.fc.wt   // a tiled twin minted at the old width would be stale; the affine reads w
    if (!empty(a.norm.w)) {
        a.norm.w |> reserve_resize(cs)
        for (i in range64(c, cs)) {
            a.norm.w[i] = 0.0
        }
    }
    if (!empty(a.norm.b)) {
        a.norm.b |> reserve_resize(cs)
        for (i in range64(c, cs)) {
            a.norm.b[i] = 0.0
        }
    }
}

//! Can this conv serve the q8 rows form: a dense forward conv whose widths quantize per 32.
def conv1d_q8_eligible(c : TtsConv1d) : bool {
    return c.groups == 1l && !c.transposed && c.stride == 1l && c.cin % 32l == 0l && c.cout % 32l == 0l
}

// The q8 rows lane: the taps stacked along K into one [cout][k*cin] matrix (tap-major within
// a row), quantized Q8_0 per 32, then repacked in place for the backend when the box's backend
// wants it (`repack`) - a conv is one GEMM over activation rows stacked the same way.
def private conv1d_prepare_q8(var c : TtsConv1d; repack : bool) {
    let kc = c.k * c.cin
    var inscope slab : array<float>
    slab |> reserve_resize(c.cout * kc)
    for (co in range64(c.cout)) {
        for (kk in range64(c.k)) {
            for (ci in range64(c.cin)) {
                slab[co * kc + kk * c.cin + ci] = c.w[(co * c.cin + ci) * c.k + kk]
            }
        }
    }
    c.wq |> reserve_resize(c.cout * kc)
    c.wqs |> reserve_resize(c.cout * kc / 32l)
    quantize_q8_0_into(slab, c.cout * kc, c.wq, c.wqs, 0l, 0l)
    if (repack) {
        repack_q8q8_weight(c.wq, c.wqs, 0l, kc, c.cout)
    }
    c.q8_n = kc
    c.q8 = true
    delete c.w
}

//! Mint the one served layout of a dense conv and drop what the consumer never reads: the rows
//! form takes `wk` (forward) or `wtr` (transposed) - or, with `q8`, the quantized stacked form -
//! and loses `w`; the channel-major form keeps `w`, a transposed one minting `wt` in its place.
def conv1d_prepare(var c : TtsConv1d; q8 : bool = false; repack : bool = false) {
    c.cout_s = c.cout
    return if (c.groups != 1l)
    if (q8 && c.served_rows && conv1d_q8_eligible(c)) {
        conv1d_prepare_q8(c, repack)
        return
    }
    if (c.transposed) {
        if (c.served_rows) {
            c.wtr |> reserve_resize(c.cin * c.k * c.cout)
        } else {
            c.wt |> reserve_resize(c.cout * c.k * c.cin)
        }
        for (ci in range64(c.cin)) {
            for (co in range64(c.cout)) {
                for (kk in range64(c.k)) {
                    let v = c.w[(ci * c.cout + co) * c.k + kk]
                    if (c.served_rows) {
                        c.wtr[(ci * c.k + kk) * c.cout + co] = v
                    } else {
                        c.wt[(co * c.k + kk) * c.cin + ci] = v
                    }
                }
            }
        }
        delete c.w
    } elif (c.served_rows) {
        c.cout_s = (c.cout + 15l) / 16l * 16l
        c.wk |> reserve_resize(c.k * c.cin * c.cout_s)
        for (i in range64(c.k * c.cin * c.cout_s)) {
            c.wk[i] = 0.0
        }
        for (co in range64(c.cout)) {
            for (ci in range64(c.cin)) {
                for (kk in range64(c.k)) {
                    c.wk[(kk * c.cin + ci) * c.cout_s + co] = c.w[(co * c.cin + ci) * c.k + kk]
                }
            }
        }
        delete c.w
    }
}

// ===== layout =====

//! [C][T] -> [T][C]
def transpose_ct(x : array<float>; c, t : int64; @scratch var y : array<float>) {
    y |> reserve_resize(c * t)
    for (ci in range64(c)) {
        let row = ci * t
        for (ti in range64(t)) {
            y[ti * c + ci] = x[row + ti]
        }
    }
}

//! [T][C] -> [C][T]
def transpose_tc(x : array<float>; t, c : int64; @scratch var y : array<float>) {
    transpose_tc_strided(x, t, c, c, y)
}

//! [T][C] rows of stride `row_stride` (>= c; the served width's padding columns dropped) -> [C][T]
def transpose_tc_strided(x : array<float>; t, c, row_stride : int64; @scratch var y : array<float>) {
    y |> reserve_resize(c * t)
    for (ti in range64(t)) {
        let row = ti * row_stride
        for (ci in range64(c)) {
            y[ci * t + ti] = x[row + ci]
        }
    }
}

// ===== convolution =====

// ===== kernel-private scratch: module globals, one per buffer, sized to the current call and
// reused across calls - a context runs one synthesis at a time and lanes never size them, so a
// kernel's transient buffer costs one growth per context, not one per call =====
var @scratch g_conv_col : array<float>
var @scratch g_conv_ytm : array<float>
var @scratch g_convt_xtm : array<float>
var @scratch g_convt_v : array<float>
var @scratch g_rows_v : array<float>
var @scratch g_q8_xq : array<int8>
var @scratch g_q8_xs : array<float>
var @scratch g_q8_xst : array<int8>
var @scratch g_q8_xss : array<float>
var @scratch g_rows_xst : array<float>
var @scratch g_adain_h : array<float>
var @scratch g_adain_mean : array<float>
var @scratch g_adain_ssq : array<float>
var @scratch g_adain_scale : array<float>
var @scratch g_adain_shift : array<float>
var @scratch g_colsum_part : array<float>
var @scratch g_lstm_h : array<float>
var @scratch g_lstm_c : array<float>
var @scratch g_lstm_xin : array<float>
var @scratch g_lstm_gates : array<float>
var @scratch g_lstm_gates2 : array<float>
var @scratch g_snake_inv : array<float>
var @scratch @exact_size g_sine_rad : array<float>
var @scratch @exact_size g_sine_low : array<float>
var @scratch @exact_size g_sine_phase : array<float>

let private CONV_CHUNK = 1024l   // im2col rows per GEMM

def private conv1d_dense(c : TtsConv1d; x : array<float>; t_in : int64; var y : array<float>) {
    let t_out = conv1d_out_len(c, t_in)
    let kw = c.cin * c.k
    var col & = g_conv_col
    var ytm & = g_conv_ytm
    col |> reserve_resize(CONV_CHUNK * kw)
    ytm |> reserve_resize(CONV_CHUNK * c.cout)
    var t0 = 0l
    while (t0 < t_out) {
        let rows = min(CONV_CHUNK, t_out - t0)
        for (r in range64(rows)) {
            let base = (t0 + r) * c.stride - c.pad_l
            for (ci in range64(c.cin)) {
                let xrow = ci * t_in
                let crow = r * kw + ci * c.k
                for (kk in range64(c.k)) {
                    let p = base + kk * c.dilation
                    col[crow + kk] = (p >= 0l && p < t_in) ? x[xrow + p] : 0.0
                }
            }
        }
        matmul_batch(ytm, c.w, col, kw, c.cout, rows)
        for (co in range64(c.cout)) {
            let bias = empty(c.b) ? 0.0 : c.b[co]
            let yrow = co * t_out + t0
            for (r in range64(rows)) {
                y[yrow + r] = ytm[r * c.cout + co] + bias
            }
        }
        t0 += rows
    }
}

def private conv1d_depthwise(c : TtsConv1d; x : array<float>; t_in : int64; var y : array<float>) {
    let t_out = conv1d_out_len(c, t_in)
    for (ch in range64(c.cout)) {
        let bias = empty(c.b) ? 0.0 : c.b[ch]
        let xrow = ch * t_in
        let wrow = ch * c.k
        for (t in range64(t_out)) {
            var acc = bias
            let base = t * c.stride - c.pad_l
            for (kk in range64(c.k)) {
                let p = base + kk * c.dilation
                if (p >= 0l && p < t_in) {
                    acc += c.w[wrow + kk] * x[xrow + p]
                }
            }
            y[ch * t_out + t] = acc
        }
    }
}

def private conv1d_transposed_dense(c : TtsConv1d; x : array<float>; t_in : int64; var y : array<float>) {
    let t_out = conv1d_out_len(c, t_in)
    for (co in range64(c.cout)) {
        let bias = empty(c.b) ? 0.0 : c.b[co]
        for (t in range64(t_out)) {
            y[co * t_out + t] = bias
        }
    }
    let d = c.cout * c.k
    var xtm & = g_convt_xtm
    var v & = g_convt_v
    transpose_ct(x, c.cin, t_in, xtm)
    v |> reserve_resize(t_in * d)
    matmul_batch(v, c.wt, xtm, c.cin, d, t_in)
    for (t in range64(t_in)) {
        let vrow = t * d
        for (co in range64(c.cout)) {
            let yrow = co * t_out
            for (kk in range64(c.k)) {
                let p = t * c.stride + kk * c.dilation - c.pad_l
                if (p >= 0l && p < t_out) {
                    y[yrow + p] += v[vrow + co * c.k + kk]
                }
            }
        }
    }
}

def private conv1d_transposed_depthwise(c : TtsConv1d; x : array<float>; t_in : int64; var y : array<float>) {
    let t_out = conv1d_out_len(c, t_in)
    for (ch in range64(c.cout)) {
        let bias = empty(c.b) ? 0.0 : c.b[ch]
        let yrow = ch * t_out
        for (t in range64(t_out)) {
            y[yrow + t] = bias
        }
        let xrow = ch * t_in
        let wrow = ch * c.k
        for (t in range64(t_in)) {
            let xv = x[xrow + t]
            for (kk in range64(c.k)) {
                let p = t * c.stride + kk * c.dilation - c.pad_l
                if (p >= 0l && p < t_out) {
                    y[yrow + p] += c.w[wrow + kk] * xv
                }
            }
        }
    }
}

//! y [cout][t_out] = conv(x [cin][t_in]); grouped convs are the depthwise case only.
def conv1d(c : TtsConv1d; x : array<float>; t_in : int64; @scratch @exact_size var y : array<float>) {
    let t_out = conv1d_out_len(c, t_in)
    y |> reserve_resize(c.cout * t_out)
    if (c.groups == 1l) {
        if (c.transposed) {
            conv1d_transposed_dense(c, x, t_in, y)
        } else {
            conv1d_dense(c, x, t_in, y)
        }
    } elif (c.groups == c.cin && c.groups == c.cout) {
        if (c.transposed) {
            conv1d_transposed_depthwise(c, x, t_in, y)
        } else {
            conv1d_depthwise(c, x, t_in, y)
        }
    } else {
        panic("dasLLAMA tts: grouped conv with groups={c.groups} cin={c.cin} cout={c.cout} is not the depthwise shape")
    }
}

// ===== convolution, rows form =====

let private ROWS_OVERSPLIT = 2   // row-block chunks per lane: balance against the tap ranges' ragged ends

//! A row-block edge snapped up to the tile's 4-row group (the last edge stays at `t`), so no
//! lane's block ends in the scalar remainder rows; both ends of every block snap the same way.
def private tile_edge(r, t : int64) : int64 => min(t, (r + 3l) / 4l * 4l)

//! Every rows-form kernel walks channels four at a time.
def private rows_width_check(c : int64; what : string) {
    if (c % 4l != 0l) {
        panic("dasLLAMA tts: {what} needs a channel count divisible by 4, got {c}")
    }
}

// Forward dense conv, stride 1: y rows start as the bias, then each tap accumulates one GEMM
// over the rows whose shifted input row exists - no im2col, no bounds check inside the kernel.
def private conv1d_rows_dense(c : TtsConv1d; x : array<float>; t_in : int64; var y : array<float>) {
    let t_out = conv1d_out_len(c, t_in)
    let cin = c.cin
    let cout = c.cout_s
    let nbias = empty(c.b) ? 0l : c.cout
    let k = c.k
    let dil = c.dilation
    let pad_l = c.pad_l
    var yp = unsafe(addr(y[0]))
    let xp = unsafe(addr<float const?>(x[0]))
    let wp = unsafe(addr<float const?>(c.wk[0]))
    let bp = nbias > 0l ? unsafe(addr<float const?>(c.b[0])) : null
    let chunks = matmul_chunks(int(t_out), ROWS_OVERSPLIT, t_out * cin * cout * k)
    maybe_parallel_for(0, int(t_out), chunks) $(rb, re) {
        unsafe {
            let rb4 = tile_edge(int64(rb), t_out)
            let re4 = tile_edge(int64(re), t_out)
            for (r in range64(rb4, re4)) {
                var yrow = yp + r * cout
                for (co in range64(cout)) {
                    yrow[co] = co < nbias ? bp[co] : 0.0
                }
            }
            for (kk in range64(k)) {
                let shift = kk * dil - pad_l
                let r0 = max(rb4, -shift)
                let r1 = min(re4, t_in - shift)
                if (r1 > r0) {
                    gemm_f32_jo(yp + r0 * cout, xp + (r0 + shift) * cin, wp + kk * cin * cout, r1 - r0, cin, cout)
                }
            }
        }
    }
}

// Transposed dense conv: one GEMM lifts every input row to its k taps [t_in][k*cout], then
// each output row gathers the taps that land on it - disjoint rows, so both halves split
// across lanes.
def private conv1d_rows_transposed(c : TtsConv1d; x : array<float>; t_in : int64; var y : array<float>) {
    let t_out = conv1d_out_len(c, t_in)
    let cin = c.cin
    let cout = c.cout
    let k = c.k
    let dil = c.dilation
    let pad_l = c.pad_l
    let stride = c.stride
    let d = k * cout
    let has_bias = !empty(c.b)
    var v & = g_rows_v
    v |> reserve_resize(t_in * d)
    var vp = unsafe(addr(v[0]))
    let xp = unsafe(addr<float const?>(x[0]))
    let wp = unsafe(addr<float const?>(c.wtr[0]))
    let chunks_in = matmul_chunks(int(t_in), ROWS_OVERSPLIT, t_in * cin * d)
    maybe_parallel_for(0, int(t_in), chunks_in) $(rb, re) {
        unsafe {
            let rb4 = tile_edge(int64(rb), t_in)
            let re4 = tile_edge(int64(re), t_in)
            for (i in range64(rb4 * d, re4 * d)) {
                vp[i] = 0.0
            }
            if (re4 > rb4) {
                gemm_f32_jo(vp + rb4 * d, xp + rb4 * cin, wp, re4 - rb4, cin, d)
            }
        }
    }
    var yp = unsafe(addr(y[0]))
    let vcp = unsafe(addr<float const?>(v[0]))
    let b4p = has_bias ? unsafe(addr<float4 const?>(c.b[0])) : null
    let chunks_out = matmul_chunks(int(t_out), ROWS_OVERSPLIT, t_out * d)
    maybe_parallel_for(0, int(t_out), chunks_out) $(rb, re) {
        unsafe {
            let c4 = cout / 4l
            for (p in range64(int64(rb), int64(re))) {
                var y4 = reinterpret<float4?>(yp + p * cout)
                for (j in range64(c4)) {
                    y4[j] = has_bias ? b4p[j] : float4(0.0)
                }
                for (kk in range64(k)) {
                    let q = p + pad_l - kk * dil
                    if (q >= 0l && q % stride == 0l && q / stride < t_in) {
                        let v4 = reinterpret<float4 const?>(vcp + (q / stride) * d + kk * cout)
                        for (j in range64(c4)) {
                            y4[j] += v4[j]
                        }
                    }
                }
            }
        }
    }
}

// The q8 rows lane: the input rows quantized once (Q8_0 per 32 along cin), then each tap one
// backend q8.q8 batch GEMM over the rows whose shifted input exists (the kernel splits its
// weight rows across lanes itself), added onto the bias-initialized y.
let private ROWS_STACK_BYTES = 4l * 1024l * 1024l   // a stacked chunk stays cache-resident while the GEMM reads it

// A strided forward conv on rows, f32: per chunk of output rows the k tap rows stack side by
// side (zero where a tap leaves the input) and one tiled GEMM with K = k*cin reads `wk` as
// [k*cin][cout_s]; the bias seeds the rows the GEMM accumulates onto.
def private conv1d_rows_dense_strided(c : TtsConv1d; x : array<float>; t_in : int64; var y : array<float>) {
    let t_out = conv1d_out_len(c, t_in)
    let cin = c.cin
    let cout = c.cout_s
    let nbias = empty(c.b) ? 0l : c.cout
    let k = c.k
    let kc = k * cin
    let stride = c.stride
    let dil = c.dilation
    let pad = c.pad_l
    var xst & = g_rows_xst
    let chunk = clamp(ROWS_STACK_BYTES / (kc * 4l) / 4l * 4l, 64l, t_out)
    xst |> reserve_resize(chunk * kc)
    var yp = unsafe(addr(y[0]))
    let xp = unsafe(addr<float const?>(x[0]))
    let wp = unsafe(addr<float const?>(c.wk[0]))
    let bp = nbias > 0l ? unsafe(addr<float const?>(c.b[0])) : null
    var xstp = unsafe(addr(xst[0]))
    var r0 = 0l
    while (r0 < t_out) {
        let rows = min(chunk, t_out - r0)
        maybe_parallel_for(0, int(rows), lanes_for_work(rows * kc, 0)) $(rb, re) {
            unsafe {
                for (r in range64(int64(rb), int64(re))) {
                    var drow = xstp + r * kc
                    for (kk in range64(k)) {
                        let src = (r0 + r) * stride + kk * dil - pad
                        if (src >= 0l && src < t_in) {
                            memcpy(drow + kk * cin, xp + src * cin, cin * 4l)
                        } else {
                            for (j in range64(cin)) {
                                drow[kk * cin + j] = 0.0
                            }
                        }
                    }
                }
            }
        }
        let chunks = matmul_chunks(int(rows), ROWS_OVERSPLIT, rows * kc * cout)
        maybe_parallel_for(0, int(rows), chunks) $(rb, re) {
            unsafe {
                let rb4 = tile_edge(int64(rb), rows)
                let re4 = tile_edge(int64(re), rows)
                for (r in range64(rb4, re4)) {
                    var yrow = yp + (r0 + r) * cout
                    for (co in range64(cout)) {
                        yrow[co] = co < nbias ? bp[co] : 0.0
                    }
                }
                if (re4 > rb4) {
                    gemm_f32_jo(yp + (r0 + rb4) * cout, xstp + rb4 * kc, wp, re4 - rb4, kc, cout)
                }
            }
        }
        r0 += rows
    }
}

// One GEMM per chunk of output rows over stacked rows - the k shifted input rows side by side,
// copied from the once-quantized input, zero scales where the shift leaves it - so the kernel
// sees K = k*cin and nothing accumulates across taps.
def private conv1d_rows_dense_q8(c : TtsConv1d; x : array<float>; t_in : int64; var y : array<float>) {
    let t_out = conv1d_out_len(c, t_in)
    let cin = c.cin
    let cout = c.cout
    let nb = cin / 32l
    let kc = c.q8_n
    let knb = kc / 32l
    let nbias = empty(c.b) ? 0l : cout
    var xq & = g_q8_xq
    var xs & = g_q8_xs
    var xst & = g_q8_xst
    var xss & = g_q8_xss
    xq |> reserve_resize(t_in * cin)
    xs |> reserve_resize(t_in * nb)
    quantize_q8_0_into(x, t_in * cin, xq, xs, 0l, 0l)
    let chunk = clamp(ROWS_STACK_BYTES / kc / 4l * 4l, 64l, t_out)
    xst |> reserve_resize(chunk * kc)
    xss |> reserve_resize(chunk * knb)
    var yp = unsafe(addr(y[0]))
    let bp = nbias > 0l ? unsafe(addr<float const?>(c.b[0])) : null
    let wqp = unsafe(addr<int8 const?>(c.wq[0]))
    let wsp = unsafe(addr<float const?>(c.wqs[0]))
    let xqp = unsafe(addr<int8 const?>(xq[0]))
    let xsp = unsafe(addr<float const?>(xs[0]))
    var xstp = unsafe(addr(xst[0]))
    var xssp = unsafe(addr(xss[0]))
    let k = c.k
    let dil = c.dilation
    let pad = c.pad_l
    var r0 = 0l
    while (r0 < t_out) {
        let rows = min(chunk, t_out - r0)
        maybe_parallel_for(0, int(rows), lanes_for_work(rows * kc, 0)) $(rb, re) {
            unsafe {
                for (r in range64(int64(rb), int64(re))) {
                    var drow = xstp + r * kc
                    var srow = xssp + r * knb
                    for (kk in range64(k)) {
                        let src = r0 + r + kk * dil - pad
                        if (src >= 0l && src < t_in) {
                            memcpy(drow + kk * cin, xqp + src * cin, cin)
                            memcpy(srow + kk * nb, xsp + src * nb, nb * 4l)
                        } else {
                            for (j in range64(cin)) {
                                drow[kk * cin + j] = int8(0)
                            }
                            for (j in range64(nb)) {
                                srow[kk * nb + j] = 0.0
                            }
                        }
                    }
                }
            }
        }
        unsafe {
            invoke(g_mm_q8q8_batch, yp + r0 * cout, wqp, wsp, xstp, xssp, kc, cout, rows)
        }
        if (nbias > 0l) {
            maybe_parallel_for(0, int(rows), lanes_for_work(rows * cout, 0)) $(rb, re) {
                unsafe {
                    let c4 = cout / 4l
                    let b4 = reinterpret<float4 const?>(bp)
                    for (r in range64(int64(rb), int64(re))) {
                        var y4 = reinterpret<float4?>(yp + (r0 + r) * cout)
                        for (j in range64(c4)) {
                            y4[j] += b4[j]
                        }
                    }
                }
            }
        }
        r0 += rows
    }
}

//! y [t_out][cout_s] = conv(x [t_in][cin]) in the rows form, the row stride the served width
//! `cout_s`: dense convs only - forward (stride 1 as tap-GEMMs, strided over stacked tap rows)
//! or transposed, a conv minted q8 on the q8 lane; a grouped conv runs the channel-major form.
def conv1d_rows(c : TtsConv1d; x : array<float>; t_in : int64; @scratch @exact_size var y : array<float>) {
    if (c.groups != 1l) {
        panic("dasLLAMA tts: the rows conv takes a dense conv; got groups={c.groups}")
    }
    if (!c.q8 && empty(c.transposed ? c.wtr : c.wk)) {
        panic("dasLLAMA tts: a rows conv with no rows layout - a prepared image older than this build's served layouts; delete the .dlim beside the GGUF")
    }
    let t_out = conv1d_out_len(c, t_in)
    y |> reserve_resize(t_out * c.cout_s)
    if (c.q8) {
        conv1d_rows_dense_q8(c, x, t_in, y)
    } elif (c.transposed) {
        rows_width_check(c.cout, "conv1d_rows (transposed)")
        conv1d_rows_transposed(c, x, t_in, y)
    } elif (c.stride != 1l) {
        conv1d_rows_dense_strided(c, x, t_in, y)
    } else {
        conv1d_rows_dense(c, x, t_in, y)
    }
}

//! y [t_out][c] = depthwise transposed conv of x [t_in][c] in the rows form (the residual
//! block's upsampling pool): every output row gathers the input rows its taps reach, channels
//! four a lane, so a row's sum never depends on how the rows split across workers.
def conv1d_rows_transposed_depthwise(c : TtsConv1d; x : array<float>; t_in : int64; @scratch @exact_size var y : array<float>) {
    if (c.groups != c.cin || c.cin != c.cout || !c.transposed) {
        panic("dasLLAMA tts: the rows depthwise transposed conv takes groups == cin == cout; got groups={c.groups} cin={c.cin} cout={c.cout}")
    }
    let t_out = conv1d_out_len(c, t_in)
    let ch = c.cin
    let k = c.k
    let stride = c.stride
    let dil = c.dilation
    let pad_l = c.pad_l
    let nbias = empty(c.b) ? 0l : ch
    y |> reserve_resize(t_out * ch)
    var yp = unsafe(addr(y[0]))
    let xp = unsafe(addr<float const?>(x[0]))
    let wp = unsafe(addr<float const?>(c.w[0]))
    let bp = nbias > 0l ? unsafe(addr<float const?>(c.b[0])) : null
    maybe_parallel_for(0, int(t_out), lanes_for_work(t_out * ch * k, 0)) $(rb, re) {
        unsafe {
            for (p in range64(int64(rb), int64(re))) {
                var yrow = yp + p * ch
                for (j in range64(ch)) {
                    yrow[j] = j < nbias ? bp[j] : 0.0
                }
                for (kk in range64(k)) {
                    let num = p + pad_l - kk * dil
                    continue if (num < 0l || num % stride != 0l)
                    let t = num / stride
                    continue if (t >= t_in)
                    let xrow = xp + t * ch
                    for (j in range64(ch)) {
                        yrow[j] += wp[j * k + kk] * xrow[j]
                    }
                }
            }
        }
    }
}

// ===== dense =====

//! Mint the tiled GEMM's B operand `wt` [nin][nout] when the width sits on the 16-column tile;
//! a layer only `linear_rows` ever reads (`rows_only`) then drops `w` - or, with `q8` and widths
//! that quantize per 32, takes the Q8_0 form of `w` (repacked for the backend when `repack`).
def linear_prepare(var l : TtsLinear; rows_only : bool = false; q8 : bool = false; repack : bool = false) {
    if (q8 && rows_only && l.nin % 32l == 0l && l.nout % 32l == 0l) {
        l.wq |> reserve_resize(l.nout * l.nin)
        l.wqs |> reserve_resize(l.nout * l.nin / 32l)
        quantize_q8_0_into(l.w, l.nout * l.nin, l.wq, l.wqs, 0l, 0l)
        if (repack) {
            repack_q8q8_weight(l.wq, l.wqs, 0l, l.nin, l.nout)
        }
        l.q8 = true
        delete l.w
        return
    }
    return if (l.nout % 16l != 0l)
    l.wt |> reserve_resize(l.nin * l.nout)
    for (o in range64(l.nout)) {
        for (i in range64(l.nin)) {
            l.wt[i * l.nout + o] = l.w[o * l.nin + i]
        }
    }
    if (rows_only) {
        delete l.w
    }
}

// The q8 lane of linear_rows: rows quantized Q8_0 per 32 along nin, one backend q8.q8 batch
// GEMM (it splits its weight rows across lanes itself), then the bias.
def private linear_rows_q8(l : TtsLinear; x : array<float>; t : int64; var y : array<float>) {
    let nin = l.nin
    let nout = l.nout
    var xq & = g_q8_xq
    var xs & = g_q8_xs
    xq |> reserve_resize(t * nin)
    xs |> reserve_resize(t * nin / 32l)
    quantize_q8_0_into(x, t * nin, xq, xs, 0l, 0l)
    unsafe {
        invoke(g_mm_q8q8_batch, addr(y[0]), addr<int8 const?>(l.wq[0]), addr<float const?>(l.wqs[0]),
            addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), nin, nout, t)
    }
    return if (empty(l.b))
    var yp = unsafe(addr(y[0]))
    let b4 = unsafe(addr<float4 const?>(l.b[0]))
    maybe_parallel_for(0, int(t), lanes_for_work(t * nout, 0)) $(rb, re) {
        unsafe {
            let n4 = nout / 4l
            for (r in range64(int64(rb), int64(re))) {
                var y4 = reinterpret<float4?>(yp + r * nout)
                for (j in range64(n4)) {
                    y4[j] += b4[j]
                }
            }
        }
    }
}

//! y [t][nout] = x [t][nin] . w^T + b: the tiled GEMM over row blocks when `wt` was minted,
//! the batched dot form otherwise; a layer minted q8 runs the q8 lane.
def linear_rows(l : TtsLinear; x : array<float>; t : int64; @scratch @exact_size var y : array<float>) {
    y |> reserve_resize(t * l.nout)
    if (l.q8) {
        linear_rows_q8(l, x, t, y)
        return
    }
    if (empty(l.wt)) {
        matmul_batch(y, l.w, x, l.nin, l.nout, t)
        return if (empty(l.b))
        for (r in range64(t)) {
            let row = r * l.nout
            for (o in range64(l.nout)) {
                y[row + o] += l.b[o]
            }
        }
        return
    }
    let nin = l.nin
    let nout = l.nout
    let nbias = empty(l.b) ? 0l : nout
    var yp = unsafe(addr(y[0]))
    let xp = unsafe(addr<float const?>(x[0]))
    let wp = unsafe(addr<float const?>(l.wt[0]))
    let bp = nbias > 0l ? unsafe(addr<float const?>(l.b[0])) : null
    let chunks = matmul_chunks(int(t), ROWS_OVERSPLIT, t * nin * nout)
    maybe_parallel_for(0, int(t), chunks) $(rb, re) {
        unsafe {
            let rb4 = tile_edge(int64(rb), t)
            let re4 = tile_edge(int64(re), t)
            for (r in range64(rb4, re4)) {
                var yrow = yp + r * nout
                for (o in range64(nout)) {
                    yrow[o] = o < nbias ? bp[o] : 0.0
                }
            }
            if (re4 > rb4) {
                gemm_f32_jo(yp + rb4 * nout, xp + rb4 * nin, wp, re4 - rb4, nin, nout)
            }
        }
    }
}

//! y [nout] = w . x + b
def linear_vec(l : TtsLinear; x : array<float>; @scratch @exact_size var y : array<float>) {
    y |> reserve_resize(l.nout)
    matmul(y, l.w, x, l.nin, l.nout)
    return if (empty(l.b))
    for (o in range64(l.nout)) {
        y[o] += l.b[o]
    }
}

// ===== attention =====

var @scratch g_att_heads : array<float>

//! Multi-head self-attention on token-major q, k, v [t][hidden] (each row the heads' slices in
//! order) into ctx [t][hidden]: one head per lane, Q.K^T and P.V on the tiles over a 16-padded
//! key axis (pad keys score -1e30 and carry zero values), the scaled softmax per row between.
def attention_rows(q, k, v : array<float>; t, hidden, heads : int64; @scratch @exact_size var ctx : array<float>) {
    let dh = hidden / heads
    let tp = (t + 15l) / 16l * 16l
    let per_head = t * dh + dh * tp + tp * dh + t * tp + t * dh   // qh, kT, vh, s, oh
    ctx |> reserve_resize(t * hidden)
    var head_scratch & = g_att_heads
    head_scratch |> reserve_resize(heads * per_head)
    let scale = 1.0 / sqrt(float(dh))
    let qp = unsafe(addr<float const?>(q[0]))
    let kp = unsafe(addr<float const?>(k[0]))
    let vp = unsafe(addr<float const?>(v[0]))
    var cp = unsafe(addr(ctx[0]))
    var sp = unsafe(addr(head_scratch[0]))
    maybe_parallel_for(0, int(heads), int(heads)) $(hb, he) {
        unsafe {
            for (hd in range64(int64(hb), int64(he))) {
                let off = hd * dh
                var qh = sp + hd * per_head
                var kT = qh + t * dh
                var vh = kT + dh * tp
                var s = vh + tp * dh
                var oh = s + t * tp
                for (i in range64(t)) {
                    for (e in range64(dh)) {
                        qh[i * dh + e] = qp[i * hidden + off + e]
                        vh[i * dh + e] = vp[i * hidden + off + e]
                        kT[e * tp + i] = kp[i * hidden + off + e]
                    }
                }
                for (i in range64(t * dh, tp * dh)) {
                    vh[i] = 0.0
                }
                for (e in range64(dh)) {
                    for (i in range64(t, tp)) {
                        kT[e * tp + i] = 0.0
                    }
                }
                for (i in range64(t * tp)) {
                    s[i] = 0.0
                }
                gemm_f32_jo(s, qh, kT, t, dh, tp)
                for (i in range64(t)) {
                    var row = s + i * tp
                    for (j in range64(t)) {
                        row[j] *= scale
                    }
                    for (j in range64(t, tp)) {
                        row[j] = -1e30
                    }
                    softmax(row, tp)
                }
                for (i in range64(t * dh)) {
                    oh[i] = 0.0
                }
                gemm_f32_jo(oh, s, vh, t, tp, dh)
                for (i in range64(t)) {
                    for (e in range64(dh)) {
                        cp[i * hidden + off + e] = oh[i * dh + e]
                    }
                }
            }
        }
    }
}

// ===== normalization =====

//! LayerNorm over the last dim of token-major rows, in place.
def layernorm_rows(var x : array<float>; t, c : int64; n : TtsNorm; eps : float) {
    unsafe {
        let wp = addr<float const?>(n.w[0])
        let bp = addr<float const?>(n.b[0])
        for (r in range64(t)) {
            var p = addr(x[r * c])
            layernorm(p, p, wp, bp, c, eps)
        }
    }
}

//! LayerNorm over channels at every position of a [C][T] buffer, in place; identity affine
//! when `n` is empty.
def layernorm_channels(var x : array<float>; c, t : int64; n : TtsNorm; eps : float) {
    for (ti in range64(t)) {
        var mean = 0.0lf
        for (ci in range64(c)) {
            mean += double(x[ci * t + ti])
        }
        mean /= double(c)
        var variance = 0.0lf
        for (ci in range64(c)) {
            let dv = double(x[ci * t + ti]) - mean
            variance += dv * dv
        }
        variance /= double(c)
        let scale = 1.0 / sqrt(float(variance) + eps)
        let fmean = float(mean)
        for (ci in range64(c)) {
            let v = (x[ci * t + ti] - fmean) * scale
            x[ci * t + ti] = empty(n.w) ? v : v * n.w[ci] + n.b[ci]
        }
    }
}

//! InstanceNorm: per channel over time, in place; identity affine when `n` is empty.
def instance_norm_channels(var x : array<float>; c, t : int64; n : TtsNorm; eps : float) {
    for (ci in range64(c)) {
        let row = ci * t
        var mean = 0.0lf
        for (ti in range64(t)) {
            mean += double(x[row + ti])
        }
        mean /= double(t)
        var variance = 0.0lf
        for (ti in range64(t)) {
            let dv = double(x[row + ti]) - mean
            variance += dv * dv
        }
        variance /= double(t)
        let scale = 1.0 / sqrt(float(variance) + eps)
        let fmean = float(mean)
        let g = empty(n.w) ? 1.0 : n.w[ci]
        let bb = empty(n.b) ? 0.0 : n.b[ci]
        for (ti in range64(t)) {
            x[row + ti] = (x[row + ti] - fmean) * scale * g + bb
        }
    }
}

//! AdaIN1d in place on x [C][T]: (1 + gamma[c]) * instance_norm(x) + beta[c], gamma and beta
//! the halves of fc(style).
def adain(a : TtsAdaIn; var x : array<float>; c, t : int64; style : array<float>; eps : float) {
    var h & = g_adain_h
    linear_vec(a.fc, style, h)
    instance_norm_channels(x, c, t, a.norm, eps)
    for (ci in range64(c)) {
        let g = 1.0 + h[ci]
        let bb = h[c + ci]
        let row = ci * t
        for (ti in range64(t)) {
            x[row + ti] = x[row + ti] * g + bb
        }
    }
}

// Per-channel mean and sum of squared deviations over the rows of x [t][c] in ONE pass: each
// fixed-size row block sums (x - its own first row) and that square (the shift keeps the
// one-pass variance from cancelling), and the blocks merge in double, lane-split invariant.
let private STAT_BLOCK = 256l

def private column_stats(x : array<float>; t, c : int64; var mean : array<float>; var ssq : array<float>) {
    let nblk = (t + STAT_BLOCK - 1l) / STAT_BLOCK
    var part & = g_colsum_part
    part |> reserve_resize(2l * nblk * c)
    for (i in range64(2l * nblk * c)) {
        part[i] = 0.0
    }
    let xp = unsafe(addr<float const?>(x[0]))
    var pp = unsafe(addr(part[0]))
    let chunks = min(lanes_for_work(t * c, 0), int(nblk))
    maybe_parallel_for(0, int(nblk), chunks) $(bb, be) {
        unsafe {
            let c4 = c / 4l
            for (blk in range64(int64(bb), int64(be))) {
                let r0 = blk * STAT_BLOCK
                let r1 = min(t, r0 + STAT_BLOCK)
                let base = xp + r0 * c
                var s1 = pp + blk * 2l * c
                var s2 = s1 + c
                if (c4 * 4l != c) {   // a width off the lane: the same block sums, one channel at a time
                    for (r in range64(r0 + 1l, r1)) {
                        let row = xp + r * c
                        for (j in range64(c)) {
                            let dv = row[j] - base[j]
                            s1[j] += dv
                            s2[j] += dv * dv
                        }
                    }
                } else {
                    let b4 = reinterpret<float4 const?>(base)
                    var a1 = reinterpret<float4?>(s1)
                    var a2 = reinterpret<float4?>(s2)
                    for (r in range64(r0 + 1l, r1)) {
                        let row4 = reinterpret<float4 const?>(xp + r * c)
                        for (j in range64(c4)) {
                            let dv = row4[j] - b4[j]
                            a1[j] += dv
                            a2[j] += dv * dv
                        }
                    }
                }
            }
        }
    }
    mean |> reserve_resize(c)
    ssq |> reserve_resize(c)
    for (ci in range64(c)) {
        var n = 0.0lf
        var m = 0.0lf
        var m2 = 0.0lf
        for (blk in range64(nblk)) {
            let r0 = blk * STAT_BLOCK
            let nb = double(min(t, r0 + STAT_BLOCK) - r0)
            let sb1 = double(part[blk * 2l * c + ci])
            let sb2 = double(part[blk * 2l * c + c + ci])
            let mb = double(x[r0 * c + ci]) + sb1 / nb
            let delta = mb - m
            let nn = n + nb
            m += delta * nb / nn
            m2 += sb2 - sb1 * sb1 / nb + delta * delta * n * nb / nn
            n = nn
        }
        mean[ci] = float(m)
        ssq[ci] = float(m2)
    }
}

// The per-channel affine AdaIN folds to: the instance-norm statistics of `x` over the rows,
// the norm's own scale and shift, then (1 + gamma) and beta.
def private adain_affine(a : TtsAdaIn; x : array<float>; t, c : int64; style : array<float>; eps : float;
                         var scale : array<float>; var shift : array<float>) {
    var h & = g_adain_h
    var mean & = g_adain_mean
    var ssq & = g_adain_ssq
    linear_vec(a.fc, style, h)
    column_stats(x, t, c, mean, ssq)
    let inv_t = 1.0 / float(t)
    scale |> reserve_resize(c)
    shift |> reserve_resize(c)
    for (ci in range64(c)) {
        let rs = 1.0 / sqrt(ssq[ci] * inv_t + eps)
        let g = empty(a.norm.w) ? 1.0 : a.norm.w[ci]
        let bb = empty(a.norm.b) ? 0.0 : a.norm.b[ci]
        let gamma1 = 1.0 + h[ci]
        scale[ci] = rs * g * gamma1
        shift[ci] = (bb - mean[ci] * rs * g) * gamma1 + h[c + ci]
    }
}

//! AdaIN1d in place on token-major x [T][C].
def adain_rows(a : TtsAdaIn; var x : array<float>; t, c : int64; style : array<float>; eps : float) {
    var scale & = g_adain_scale
    var shift & = g_adain_shift
    adain_affine(a, x, t, c, style, eps, scale, shift)
    var xp = unsafe(addr(x[0]))
    let sp = unsafe(addr<float const?>(scale[0]))
    let bp = unsafe(addr<float const?>(shift[0]))
    let s4 = unsafe(addr<float4 const?>(scale[0]))
    let b4 = unsafe(addr<float4 const?>(shift[0]))
    let chunks = lanes_for_work(t * c, 0)
    maybe_parallel_for(0, int(t), chunks) $(rb, re) {
        unsafe {
            let c4 = c / 4l
            for (r in range64(int64(rb), int64(re))) {
                if (c4 * 4l != c) {   // a width off the lane
                    var row = xp + r * c
                    for (j in range64(c)) {
                        row[j] = mad(row[j], sp[j], bp[j])
                    }
                    continue
                }
                var row4 = reinterpret<float4?>(xp + r * c)
                for (j in range64(c4)) {
                    row4[j] = mad(row4[j], s4[j], b4[j])
                }
            }
        }
    }
}

//! AdaIN1d of token-major src [T][C] written into dst - the copy a residual branch would make
//! of its input, folded into the normalize pass.
def adain_rows_into(a : TtsAdaIn; src : array<float>; @scratch @exact_size var dst : array<float>; t, c : int64; style : array<float>; eps : float) {
    var scale & = g_adain_scale
    var shift & = g_adain_shift
    adain_affine(a, src, t, c, style, eps, scale, shift)
    dst |> reserve_resize(t * c)
    let xp = unsafe(addr<float const?>(src[0]))
    var dp = unsafe(addr(dst[0]))
    let sp = unsafe(addr<float const?>(scale[0]))
    let bp = unsafe(addr<float const?>(shift[0]))
    let s4 = unsafe(addr<float4 const?>(scale[0]))
    let b4 = unsafe(addr<float4 const?>(shift[0]))
    let chunks = lanes_for_work(t * c, 0)
    maybe_parallel_for(0, int(t), chunks) $(rb, re) {
        unsafe {
            let c4 = c / 4l
            for (r in range64(int64(rb), int64(re))) {
                if (c4 * 4l != c) {   // a width off the lane
                    let row = xp + r * c
                    var out = dp + r * c
                    for (j in range64(c)) {
                        out[j] = mad(row[j], sp[j], bp[j])
                    }
                    continue
                }
                let row4 = reinterpret<float4 const?>(xp + r * c)
                var out4 = reinterpret<float4?>(dp + r * c)
                for (j in range64(c4)) {
                    out4[j] = mad(row4[j], s4[j], b4[j])
                }
            }
        }
    }
}

// ===== elementwise row passes (float4 lanes, row blocks across lanes) =====

//! y += x over n floats (n % 4 == 0).
def add_rows(var y : array<float>; x : array<float>; n : int64) {
    rows_width_check(n, "add_rows")
    var yp = unsafe(addr(y[0]))
    let xp = unsafe(addr<float const?>(x[0]))
    let n4 = int(n / 4l)
    maybe_parallel_for(0, n4, lanes_for_work(n, 0)) $(rb, re) {
        unsafe {
            var y4 = reinterpret<float4?>(yp)
            let x4 = reinterpret<float4 const?>(xp)
            for (i in range64(int64(rb), int64(re))) {
                y4[i] += x4[i]
            }
        }
    }
}

//! y = a + b over n floats (n % 4 == 0); `y` sized here.
def sum_rows_into(@scratch @exact_size var y : array<float>; a, b : array<float>; n : int64) {
    rows_width_check(n, "sum_rows_into")
    y |> reserve_resize(n)
    var yp = unsafe(addr(y[0]))
    let ap = unsafe(addr<float const?>(a[0]))
    let bp = unsafe(addr<float const?>(b[0]))
    let n4 = int(n / 4l)
    maybe_parallel_for(0, n4, lanes_for_work(n, 0)) $(rb, re) {
        unsafe {
            var y4 = reinterpret<float4?>(yp)
            let a4 = reinterpret<float4 const?>(ap)
            let b4 = reinterpret<float4 const?>(bp)
            for (i in range64(int64(rb), int64(re))) {
                y4[i] = a4[i] + b4[i]
            }
        }
    }
}

//! y *= s over n floats (n % 4 == 0).
def scale_rows(var y : array<float>; s : float; n : int64) {
    rows_width_check(n, "scale_rows")
    var yp = unsafe(addr(y[0]))
    let n4 = int(n / 4l)
    let s4 = float4(s)
    maybe_parallel_for(0, n4, lanes_for_work(n, 0)) $(rb, re) {
        unsafe {
            var y4 = reinterpret<float4?>(yp)
            for (i in range64(int64(rb), int64(re))) {
                y4[i] *= s4
            }
        }
    }
}

//! dst = src over n floats (n % 4 == 0); `dst` sized here.
def copy_rows(@scratch @exact_size var dst : array<float>; src : array<float>; n : int64) {
    rows_width_check(n, "copy_rows")
    dst |> reserve_resize(n)
    var dp = unsafe(addr(dst[0]))
    let sp = unsafe(addr<float const?>(src[0]))
    let n4 = int(n / 4l)
    maybe_parallel_for(0, n4, lanes_for_work(n, 0)) $(rb, re) {
        unsafe {
            var d4 = reinterpret<float4?>(dp)
            let s4 = reinterpret<float4 const?>(sp)
            for (i in range64(int64(rb), int64(re))) {
                d4[i] = s4[i]
            }
        }
    }
}

//! AdaLayerNorm in place on token-major x [T][C]: (1 + gamma) * layernorm(row) + beta.
def ada_layernorm_rows(a : TtsAdaLayerNorm; var x : array<float>; t, c : int64; style : array<float>; eps : float) {
    var h & = g_adain_h
    linear_vec(a.fc, style, h)
    layernorm_channels_rows(x, t, c, eps)
    for (r in range64(t)) {
        let row = r * c
        for (ci in range64(c)) {
            x[row + ci] = x[row + ci] * (1.0 + h[ci]) + h[c + ci]
        }
    }
}

def private layernorm_channels_rows(var x : array<float>; t, c : int64; eps : float) {
    for (r in range64(t)) {
        let row = r * c
        var mean = 0.0lf
        for (ci in range64(c)) {
            mean += double(x[row + ci])
        }
        mean /= double(c)
        var variance = 0.0lf
        for (ci in range64(c)) {
            let dv = double(x[row + ci]) - mean
            variance += dv * dv
        }
        variance /= double(c)
        let scale = 1.0 / sqrt(float(variance) + eps)
        let fmean = float(mean)
        for (ci in range64(c)) {
            x[row + ci] = (x[row + ci] - fmean) * scale
        }
    }
}

// ===== recurrent =====

def private sigmoid_f(x : float) : float {
    return 1.0 / (1.0 + exp(-x))
}

//! One direction over token-major x [T][nin]; writes h_t into y[t*ystride + yoff ..].
def private lstm_direction(l : TtsLstm; x : array<float>; t : int64; reverse : bool;
                           var y : array<float>; ystride, yoff : int64) {
    let hd = l.hidden
    var h & = g_lstm_h
    var cst & = g_lstm_c
    var xin & = g_lstm_xin
    var gates & = g_lstm_gates
    var gates2 & = g_lstm_gates2
    h |> reserve_resize(hd)
    cst |> reserve_resize(hd)
    xin |> reserve_resize(l.nin)
    gates |> reserve_resize(4l * hd)
    gates2 |> reserve_resize(4l * hd)
    for (j in range64(hd)) {
        h[j] = 0.0
        cst[j] = 0.0
    }
    for (step in range64(t)) {
        let ti = reverse ? t - 1l - step : step
        for (i in range64(l.nin)) {
            xin[i] = x[ti * l.nin + i]
        }
        matmul(gates, l.w_ih, xin, l.nin, 4l * hd)
        matmul(gates2, l.w_hh, h, hd, 4l * hd)
        for (j in range64(hd)) {
            let gi = sigmoid_f(gates[j] + gates2[j] + l.b[j])
            let gf = sigmoid_f(gates[hd + j] + gates2[hd + j] + l.b[hd + j])
            let gg = tanh(gates[2l * hd + j] + gates2[2l * hd + j] + l.b[2l * hd + j])
            let go = sigmoid_f(gates[3l * hd + j] + gates2[3l * hd + j] + l.b[3l * hd + j])
            let cn = gf * cst[j] + gi * gg
            cst[j] = cn
            h[j] = go * tanh(cn)
            y[ti * ystride + yoff + j] = h[j]
        }
    }
}

//! Bidirectional LSTM over token-major x [T][nin] -> y [T][2H], forward half first.
def bilstm(l : TtsBiLstm; x : array<float>; t : int64; @scratch @exact_size var y : array<float>) {
    let hd = l.fwd.hidden
    y |> reserve_resize(t * 2l * hd)
    lstm_direction(l.fwd, x, t, false, y, 2l * hd, 0l)
    lstm_direction(l.bwd, x, t, true, y, 2l * hd, hd)
}

// ===== activations =====

def leaky_relu(var x : array<float>; n : int64; slope : float) {
    for (i in range64(n)) {
        let v = x[i]
        x[i] = v > 0.0 ? v : v * slope
    }
}

//! Snake on x [C][T]: x + sin(alpha x)^2 / alpha, one alpha per channel.
def snake(var x : array<float>; c, t : int64; alpha : array<float>) {
    for (ci in range64(c)) {
        let a = alpha[ci]
        let inv = 1.0 / a
        let row = ci * t
        for (ti in range64(t)) {
            let s = sin(a * x[row + ti])
            x[row + ti] += inv * (s * s)
        }
    }
}

//! Snake on token-major x [T][C]: four channels per lane, row blocks across lanes.
def snake_rows(var x : array<float>; t, c : int64; alpha : array<float>) {
    rows_width_check(c, "snake_rows")
    var inv & = g_snake_inv
    inv |> reserve_resize(c)
    for (ci in range64(c)) {
        inv[ci] = 1.0 / alpha[ci]
    }
    var xp = unsafe(addr(x[0]))
    let a4 = unsafe(addr<float4 const?>(alpha[0]))
    let i4 = unsafe(addr<float4 const?>(inv[0]))
    let chunks = lanes_for_work(t * c * 8l, 0)
    maybe_parallel_for(0, int(t), chunks) $(rb, re) {
        unsafe {
            let c4 = c / 4l
            for (r in range64(int64(rb), int64(re))) {
                var row4 = reinterpret<float4?>(xp + r * c)
                for (j in range64(c4)) {
                    let s = sin(a4[j] * row4[j])
                    row4[j] = mad(i4[j], s * s, row4[j])
                }
            }
        }
    }
}

def sigmoid_inplace(var x : array<float>; n : int64) {
    for (i in range64(n)) {
        x[i] = sigmoid_f(x[i])
    }
}

def tanh_inplace(var x : array<float>; n : int64) {
    for (i in range64(n)) {
        x[i] = tanh(x[i])
    }
}

// ===== resampling =====

//! Nearest-neighbour upsample of x [C][T] by an integer factor (ONNX asymmetric / floor).
def resize_nearest(x : array<float>; c, t, factor : int64; @scratch @exact_size var y : array<float>) {
    let t_out = t * factor
    y |> reserve_resize(c * t_out)
    for (ci in range64(c)) {
        let xrow = ci * t
        let yrow = ci * t_out
        for (to in range64(t_out)) {
            y[yrow + to] = x[xrow + to / factor]
        }
    }
}

//! Nearest upsampling of x [t][c] along t in the rows form: each output row is a copy of its
//! source row.
def resize_nearest_rows(x : array<float>; t, c, factor : int64; @scratch @exact_size var y : array<float>) {
    let t_out = t * factor
    y |> reserve_resize(t_out * c)
    var yp = unsafe(addr(y[0]))
    let xp = unsafe(addr<float const?>(x[0]))
    unsafe {
        for (to in range64(t_out)) {
            memcpy(yp + to * c, xp + (to / factor) * c, c * 4l)
        }
    }
}

//! Fused multiply-add through double: a * b is exact there and the sum rounds once to double,
//! then to float - the reference kernel's fma to within a double-rounding tie.
def private fmaf(a, b, c : float) : float {
    return float(double(a) * double(b) + double(c))
}

//! Linear resample of x [C][T] by `scale`: bit for bit onnxruntime's trilinear kernel on a
//! [1][C][T] tensor (ONNX half_pixel; a depth of one halves both depth weights; the unscaled
//! channel axis weights its own row one and the next zero, the last row averaging itself).
def resize_linear(x : array<float>; c, t : int64; scale : float; @scratch @exact_size var y : array<float>) : int64 {
    let t_out = int64(floor(float(t) * scale))
    y |> reserve_resize(c * t_out)
    let half = 0.5
    for (to in range64(t_out)) {
        let in_x = clamp((float(to) + 0.5) / scale - 0.5, 0.0, float(t - 1l))
        let i1 = min(int64(in_x), t - 1l)
        let i2 = min(i1 + 1l, t - 1l)
        var dx1 = abs(in_x - float(i1))
        var dx2 = abs(in_x - float(i2))
        if (i1 == i2) {
            dx1 = 0.5
            dx2 = 0.5
        }
        for (ci in range64(c)) {
            let c2 = min(ci + 1l, c - 1l)
            let dy1 = c2 == ci ? 0.5 : 0.0
            let dy2 = c2 == ci ? 0.5 : 1.0
            let x11 = x[ci * t + i1]
            let x21 = x[ci * t + i2]
            let x12 = x[c2 * t + i1]
            let x22 = x[c2 * t + i2]
            var acc = fmaf((dx2 * dy2) * half, x11, ((dx1 * dy2) * half) * x21)
            acc = fmaf((dx2 * dy1) * half, x12, acc)
            acc = fmaf((dx1 * dy1) * half, x22, acc)
            acc = fmaf((dx2 * dy2) * half, x11, acc)
            acc = fmaf((dx1 * dy2) * half, x21, acc)
            acc = fmaf((dx2 * dy1) * half, x12, acc)
            acc = fmaf((dx1 * dy1) * half, x22, acc)
            y[ci * t_out + to] = acc
        }
    }
    return t_out
}

//! Linear resample of x [C][T] by `scale`, bit for bit what PyTorch's CPU upsample_linear1d
//! computes (align_corners false, scale_factor given): the source coordinate in double from the
//! float reciprocal scale, the two taps combined as fma(l0, x0, l1 * x1).
def resize_linear_torch(x : array<float>; c, t : int64; scale : float; @scratch @exact_size var y : array<float>) : int64 {
    let t_out = int64(floor(float(t) * scale))
    y |> reserve_resize(c * t_out)
    let inv = double(1.0 / scale)
    for (to in range64(t_out)) {
        let src = max(0.0, float(inv * (double(to) + 0.5lf) - 0.5lf))
        let i0 = min(int64(src), t - 1l)
        let i1 = min(i0 + 1l, t - 1l)
        let l1 = src - float(i0)
        let l0 = 1.0 - l1
        for (ci in range64(c)) {
            y[ci * t_out + to] = fmaf(l0, x[ci * t + i0], l1 * x[ci * t + i1])
        }
    }
    return t_out
}

// ===== alignment =====

//! y [C][F] repeats token t's column dur[t] times: x is token-major [T][C].
def align_expand_rows(x : array<float>; t, c : int64; dur : array<int>; @scratch @exact_size var y : array<float>) : int64 {
    var frames = 0l
    for (d in dur) {
        frames += int64(d)
    }
    y |> reserve_resize(c * frames)
    var f = 0l
    for (ti in range64(t)) {
        for (_r in range(dur[ti])) {
            for (ci in range64(c)) {
                y[ci * frames + f] = x[ti * c + ci]
            }
            f++
        }
    }
    return frames
}

//! y [C][F] repeats token t's column dur[t] times: x is channel-major [C][T].
def align_expand_channels(x : array<float>; c, t : int64; dur : array<int>; @scratch @exact_size var y : array<float>) : int64 {
    var frames = 0l
    for (d in dur) {
        frames += int64(d)
    }
    y |> reserve_resize(c * frames)
    for (ci in range64(c)) {
        var f = 0l
        for (ti in range64(t)) {
            let v = x[ci * t + ti]
            for (_r in range(dur[ti])) {
                y[ci * frames + f] = v
                f++
            }
        }
    }
    return frames
}

//! Half-to-even rounding (torch.round / ONNX Round), which `round` is not.
def round_half_even(x : float) : float {
    let fl = floor(x)
    let diff = x - fl
    if (diff < 0.5) {
        return fl
    }
    if (diff > 0.5) {
        return fl + 1.0
    }
    return (int64(fl) % 2l == 0l) ? fl : fl + 1.0
}

// ===== source noise =====

//! PCG32: a small, platform-stable generator for the sine source's noise.
struct TtsRng {
    state : uint64
    inc : uint64
    spare : float
    has_spare : bool
}

def rng_seed(var r : TtsRng; seed : uint64) {
    r.state = 0ul
    r.inc = (seed << 1ul) | 1ul
    rng_u32(r)
    r.state += seed
    rng_u32(r)
    r.has_spare = false
}

def rng_u32(var r : TtsRng) : uint {
    let old = r.state
    r.state = old * 6364136223846793005ul + r.inc
    let xorshifted = uint(((old >> 18ul) ^ old) >> 27ul)
    let rot = uint(old >> 59ul)
    return (xorshifted >> rot) | (xorshifted << ((0u - rot) & 31u))
}

//! Uniform in [0, 1).
def rng_uniform(var r : TtsRng) : float {
    return float(rng_u32(r) >> 8u) * (1.0 / 16777216.0)
}

//! Standard normal (Marsaglia polar).
def rng_normal(var r : TtsRng) : float {
    if (r.has_spare) {
        r.has_spare = false
        return r.spare
    }
    var u = 0.0
    var v = 0.0
    var s = 0.0
    while (s >= 1.0 || s == 0.0) {
        u = rng_uniform(r) * 2.0 - 1.0
        v = rng_uniform(r) * 2.0 - 1.0
        s = u * u + v * v
    }
    let m = sqrt(-2.0 * log(s) / s)
    r.spare = v * m
    r.has_spare = true
    return u * m
}

// ===== harmonic source =====

//! The harmonic-plus-noise source: the fundamental and its harmonics as sines with a random
//! initial phase, voiced where f0 exceeds `voiced_thr`, noise everywhere. `f0` is one value per
//! frame; out is sample-major [N][n_harm], N = frames * upsample; noise_u [n_harm], noise_n [N][n_harm].
struct SineSourceCfg {
    upsample : int64 = 300l
    sample_rate : float = 24000.0
    n_harm : int64 = 9l
    sine_amp : float = 0.1
    noise_std : float = 0.003
    voiced_thr : float = 10.0
    torch_math : bool   // the reference's arithmetic: PyTorch's (double-accumulated cumsum, its resampler), else onnxruntime's
}

def private source_resize(cfg : SineSourceCfg; x : array<float>; c, t : int64; scale : float; @scratch @exact_size var y : array<float>) : int64 {
    return cfg.torch_math ? resize_linear_torch(x, c, t, scale, y) : resize_linear(x, c, t, scale, y)
}

def sine_source(cfg : SineSourceCfg; f0 : array<float>; frames : int64; noise_u, noise_n : array<float>;
                @exact_size var out : array<float>) : int64 {
    let n = frames * cfg.upsample
    let nh = cfg.n_harm
    var rad & = g_sine_rad
    rad |> reserve_resize(nh * n)
    for (h in range64(nh)) {
        let mult = float(h + 1l)
        for (i in range64(n)) {
            let cyc = f0[i / cfg.upsample] * mult / cfg.sample_rate
            rad[h * n + i] = cyc - floor(cyc)
        }
        if (h > 0l) {
            rad[h * n] += noise_u[h]
        }
    }
    var low & = g_sine_low
    let nlow = source_resize(cfg, rad, nh, n, 1.0 / float(cfg.upsample), low)
    // the reference's operation order, kept exactly: the phase reaches 1e5 radians, where one
    // float32 ulp is a hundredth of a radian, so a reassociated product is an audible change
    for (h in range64(nh)) {
        var acc = 0.0
        var acc64 = 0.0lf
        for (i in range64(nlow)) {
            if (cfg.torch_math) {
                acc64 += double(low[h * nlow + i])   // PyTorch's CPU cumsum: a double accumulator, each output rounded
                acc = float(acc64)
            } else {
                acc += low[h * nlow + i]
            }
            low[h * nlow + i] = ((acc * 2.0) * PI) * float(cfg.upsample)
        }
    }
    var phase & = g_sine_phase
    let nup = source_resize(cfg, low, nh, nlow, float(cfg.upsample), phase)
    if (nup != n) {
        panic("dasLLAMA tts: sine source resample {nlow} -> {nup} samples, want {n}")
    }
    out |> reserve_resize(n * nh)
    for (i in range64(n)) {
        let uv = f0[i / cfg.upsample] > cfg.voiced_thr ? 1.0 : 0.0
        let noise_amp = uv * cfg.noise_std + (1.0 - uv) * cfg.sine_amp / 3.0
        for (h in range64(nh)) {
            let s = sin(phase[h * n + i]) * cfg.sine_amp
            out[i * nh + h] = s * uv + noise_amp * noise_n[i * nh + h]
        }
    }
    return n
}

// ===== STFT as convolution =====

//! Magnitude and phase from the two conv halves re/im [bins][frames] -> out [2*bins][frames]
//! (magnitude rows first). A zero imaginary part reads as +0, so a negative real axis is +pi:
//! torch.angle's branch, which a conv's -0 sum would otherwise flip to -pi.
def magnitude_phase(re, im : array<float>; bins, frames : int64; eps : float; @scratch @exact_size var out : array<float>) {
    out |> reserve_resize(2l * bins * frames)
    for (i in range64(bins * frames)) {
        let r = re[i]
        let m = im[i]
        out[i] = sqrt(r * r + m * m + eps)
        out[bins * frames + i] = m == 0.0 ? (r < 0.0 ? PI : 0.0) : atan2(m, r)
    }
}

//! exp(log-magnitude) and sin(phase) into the rectangular pair the inverse conv consumes:
//! re = mag * cos(phase), im = mag * sin(phase) from x [2*bins][frames].
def polar_to_rect(x : array<float>; bins, frames : int64; @scratch @exact_size var re : array<float>; @scratch @exact_size var im : array<float>) {
    re |> reserve_resize(bins * frames)
    im |> reserve_resize(bins * frames)
    for (i in range64(bins * frames)) {
        let mag = exp(x[i])
        let ph = sin(x[bins * frames + i])
        re[i] = mag * cos(ph)
        im[i] = mag * sin(ph)
    }
}

//! `polar_to_rect` from token-major rows x [frames][stride] (magnitude columns then phase
//! columns, `stride` >= 2*bins) into the channel-major pair the inverse conv consumes.
def polar_to_rect_rows(x : array<float>; stride, bins, frames : int64; @scratch @exact_size var re : array<float>; @scratch @exact_size var im : array<float>) {
    re |> reserve_resize(bins * frames)
    im |> reserve_resize(bins * frames)
    for (f in range64(frames)) {
        let row = f * stride
        for (b in range64(bins)) {
            let mag = exp(x[row + b])
            let ph = sin(x[row + bins + b])
            re[b * frames + f] = mag * cos(ph)
            im[b * frames + f] = mag * sin(ph)
        }
    }
}

//! Edge-pad a mono signal by `pad` samples on both sides.
def pad_edge(x : array<float>; n, pad : int64; @scratch @exact_size var y : array<float>) {
    y |> reserve_resize(n + 2l * pad)
    for (i in range64(n + 2l * pad)) {
        y[i] = x[clamp(i - pad, 0l, n - 1l)]
    }
}

//! Reflection-pad a mono signal by `pad` samples on both sides (torch.stft's center pad).
def pad_reflect(x : array<float>; n, pad : int64; @scratch @exact_size var y : array<float>) {
    y |> reserve_resize(n + 2l * pad)
    for (i in range64(n + 2l * pad)) {
        var src = i - pad
        if (src < 0l) {
            src = -src
        } elif (src >= n) {
            src = 2l * (n - 1l) - src
        }
        y[i] = x[src]
    }
}

//! torch.istft's window normalization: divide the overlap-added signal by the summed squared
//! window envelope wherever it is not negligible. `wave` is the trimmed signal (`pad` samples
//! dropped on each side), `k`/`hop` the frame length and stride.
def istft_envelope_divide(var wave : array<float>; n : int64; window : array<float>; k, hop, pad : int64) {
    for (i in range64(n)) {
        let m = i + pad
        var env = 0.0
        var f = (m - k + hop) / hop
        if (f < 0l) {
            f = 0l
        }
        while (f * hop <= m) {
            let j = m - f * hop
            if (j < k) {
                env += window[j] * window[j]
            }
            f++
        }
        if (env > 1e-11) {
            wave[i] /= env
        }
    }
}

//! Reflection-pad x [C][T] by `left` on the left only (the generator's (1, 0) pad).
def pad_reflect_left(x : array<float>; c, t, left : int64; @scratch @exact_size var y : array<float>) {
    let t_out = t + left
    y |> reserve_resize(c * t_out)
    for (ci in range64(c)) {
        for (to in range64(t_out)) {
            let src = to - left
            y[ci * t_out + to] = x[ci * t + abs(src)]
        }
    }
}

//! `pad_reflect_left_rows` in place: x [t][c] grows to [t + left][c], its rows shifted down by
//! `left` and the new leading rows reflecting the ones that follow.
def pad_reflect_left_rows_inplace(@scratch @exact_size var x : array<float>; t, c, left : int64) {
    let t_out = t + left
    x |> reserve_resize(t_out * c)
    var to = t_out - 1l
    while (to >= left) {
        let src = (to - left) * c
        let dst = to * c
        for (ci in range64(c)) {
            x[dst + ci] = x[src + ci]
        }
        to--
    }
    for (p in range64(left)) {
        let src = (2l * left - p) * c
        let dst = p * c
        for (ci in range64(c)) {
            x[dst + ci] = x[src + ci]
        }
    }
}

//! The rows twin of `pad_reflect_left`: y [t + left][c] from x [t][c].
def pad_reflect_left_rows(x : array<float>; t, c, left : int64; @scratch @exact_size var y : array<float>) {
    let t_out = t + left
    y |> reserve_resize(t_out * c)
    for (to in range64(t_out)) {
        let src = abs(to - left) * c
        let dst = to * c
        for (ci in range64(c)) {
            y[dst + ci] = x[src + ci]
        }
    }
}
