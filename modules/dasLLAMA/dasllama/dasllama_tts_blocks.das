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
    wq : array<int8>     // [k][cout][cin] Q8_0 quants, repacked for the backend - the q8 rows form's per-tap W
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

//! Can this conv serve the q8 rows form: a dense forward conv whose widths quantize per 32.
def conv1d_q8_eligible(c : TtsConv1d) : bool {
    return c.groups == 1l && !c.transposed && c.stride == 1l && c.cin % 32l == 0l && c.cout % 32l == 0l
}

// The q8 rows lane: each tap [cout][cin] quantized Q8_0 along cin, then repacked in place for
// the backend when the box's backend wants it (`repack`).
def private conv1d_prepare_q8(var c : TtsConv1d; repack : bool) {
    let tap = c.cout * c.cin
    var inscope @scratch slab : array<float>
    slab |> reserve_resize(tap)
    c.wq |> reserve_resize(c.k * tap)
    c.wqs |> reserve_resize(c.k * tap / 32l)
    for (kk in range64(c.k)) {
        for (co in range64(c.cout)) {
            for (ci in range64(c.cin)) {
                slab[co * c.cin + ci] = c.w[(co * c.cin + ci) * c.k + kk]
            }
        }
        quantize_q8_0_into(slab, tap, c.wq, c.wqs, kk * tap, kk * tap / 32l)
        if (repack) {
            repack_q8q8_weight(c.wq, c.wqs, kk * tap, c.cin, c.cout)
        }
    }
    c.q8 = true
    delete c.w
}

//! Mint the one served layout of a dense conv and drop what the consumer never reads: the rows
//! form takes `wk` (forward) or `wtr` (transposed) - or, with `q8`, the quantized per-tap form -
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
def transpose_ct(x : array<float>; c, t : int64; var y : array<float>) {
    y |> reserve_resize(c * t)
    for (ci in range64(c)) {
        let row = ci * t
        for (ti in range64(t)) {
            y[ti * c + ci] = x[row + ti]
        }
    }
}

//! [T][C] -> [C][T]
def transpose_tc(x : array<float>; t, c : int64; var y : array<float>) {
    transpose_tc_strided(x, t, c, c, y)
}

//! [T][C] rows of stride `row_stride` (>= c; the served width's padding columns dropped) -> [C][T]
def transpose_tc_strided(x : array<float>; t, c, row_stride : int64; var y : array<float>) {
    y |> reserve_resize(c * t)
    for (ti in range64(t)) {
        let row = ti * row_stride
        for (ci in range64(c)) {
            y[ci * t + ti] = x[row + ci]
        }
    }
}

// ===== convolution =====

let private CONV_CHUNK = 1024l   // im2col rows per GEMM

def private conv1d_dense(c : TtsConv1d; x : array<float>; t_in : int64; var y : array<float>) {
    let t_out = conv1d_out_len(c, t_in)
    let kw = c.cin * c.k
    var inscope @scratch col : array<float>
    var inscope @scratch ytm : array<float>
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
    var inscope @scratch xtm : array<float>
    var inscope @scratch v : array<float>
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
def conv1d(c : TtsConv1d; x : array<float>; t_in : int64; @exact_size var y : array<float>) {
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
    var inscope @scratch v : array<float>
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
def private conv1d_rows_dense_q8(c : TtsConv1d; x : array<float>; t_in : int64; var y : array<float>) {
    let t_out = conv1d_out_len(c, t_in)
    let cin = c.cin
    let cout = c.cout
    let nb = cin / 32l
    let tap = cout * cin
    let nbias = empty(c.b) ? 0l : cout
    var inscope @scratch xq : array<int8>
    var inscope @scratch xs : array<float>
    var inscope @scratch ytap : array<float>
    xq |> reserve_resize(t_in * cin)
    xs |> reserve_resize(t_in * nb)
    ytap |> reserve_resize(t_out * cout)
    quantize_q8_0_into(x, t_in * cin, xq, xs, 0l, 0l)
    var yp = unsafe(addr(y[0]))
    var ytp = unsafe(addr(ytap[0]))
    let bp = nbias > 0l ? unsafe(addr<float const?>(c.b[0])) : null
    let wqp = unsafe(addr<int8 const?>(c.wq[0]))
    let wsp = unsafe(addr<float const?>(c.wqs[0]))
    let xqp = unsafe(addr<int8 const?>(xq[0]))
    let xsp = unsafe(addr<float const?>(xs[0]))
    maybe_parallel_for(0, int(t_out), lanes_for_work(t_out * cout, 0)) $(rb, re) {
        unsafe {
            for (r in range64(int64(rb), int64(re))) {
                var yrow = yp + r * cout
                for (co in range64(cout)) {
                    yrow[co] = co < nbias ? bp[co] : 0.0
                }
            }
        }
    }
    for (kk in range64(c.k)) {
        let shift = kk * c.dilation - c.pad_l
        let r0 = max(0l, -shift)
        let r1 = min(t_out, t_in - shift)
        continue if (r1 <= r0)
        let rows = r1 - r0
        unsafe {
            invoke(g_mm_q8q8_batch, ytp, wqp + kk * tap, wsp + kk * tap / 32l, xqp + (r0 + shift) * cin, xsp + (r0 + shift) * nb, cin, cout, rows)
        }
        maybe_parallel_for(0, int(rows), lanes_for_work(rows * cout, 0)) $(rb, re) {
            unsafe {
                let c4 = cout / 4l
                for (r in range64(int64(rb), int64(re))) {
                    var y4 = reinterpret<float4?>(yp + (r0 + r) * cout)
                    let t4 = reinterpret<float4 const?>(ytp + r * cout)
                    for (j in range64(c4)) {
                        y4[j] += t4[j]
                    }
                }
            }
        }
    }
}

//! y [t_out][cout_s] = conv(x [t_in][cin]) in the rows form; the row stride is the served
//! width `cout_s`. Dense convs only, forward at stride 1 or transposed; every other shape
//! runs the channel-major `conv1d`. A conv minted q8 runs the q8 lane.
def conv1d_rows(c : TtsConv1d; x : array<float>; t_in : int64; @exact_size var y : array<float>) {
    if (c.groups != 1l || (!c.transposed && c.stride != 1l)) {
        panic("dasLLAMA tts: the rows conv takes a dense conv (forward at stride 1, or transposed); got groups={c.groups} stride={c.stride} transposed={c.transposed}")
    }
    let t_out = conv1d_out_len(c, t_in)
    y |> reserve_resize(t_out * c.cout_s)
    if (c.q8) {
        conv1d_rows_dense_q8(c, x, t_in, y)
    } elif (c.transposed) {
        rows_width_check(c.cout, "conv1d_rows (transposed)")
        conv1d_rows_transposed(c, x, t_in, y)
    } else {
        conv1d_rows_dense(c, x, t_in, y)
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
    var inscope @scratch xq : array<int8>
    var inscope @scratch xs : array<float>
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
def linear_rows(l : TtsLinear; x : array<float>; t : int64; @exact_size var y : array<float>) {
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
def linear_vec(l : TtsLinear; x : array<float>; @exact_size var y : array<float>) {
    y |> reserve_resize(l.nout)
    matmul(y, l.w, x, l.nin, l.nout)
    return if (empty(l.b))
    for (o in range64(l.nout)) {
        y[o] += l.b[o]
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
    var inscope @scratch h : array<float>
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

// Per-channel sums over the rows of x [t][c] (of x itself, or of (x - center)^2), float4 lanes
// per row block, the blocks combined in double: `out` [c].
let private STAT_BLOCK = 256l   // rows per partial sum: fixed, so the result never depends on the lane split

def private column_sums(x : array<float>; t, c : int64; center : array<float>; squared : bool; var out : array<float>) {
    let nblk = (t + STAT_BLOCK - 1l) / STAT_BLOCK
    var inscope @scratch part : array<float>
    part |> reserve_resize(nblk * c)
    for (i in range64(nblk * c)) {
        part[i] = 0.0
    }
    let xp = unsafe(addr<float const?>(x[0]))
    var pp = unsafe(addr(part[0]))
    let m4 = unsafe(addr<float4 const?>(center[0]))
    let chunks = min(lanes_for_work(t * c, 0), int(nblk))
    maybe_parallel_for(0, int(nblk), chunks) $(bb, be) {
        unsafe {
            let c4 = c / 4l
            for (blk in range64(int64(bb), int64(be))) {
                var acc4 = reinterpret<float4?>(pp + blk * c)
                let r1 = min(t, (blk + 1l) * STAT_BLOCK)
                if (squared) {
                    for (r in range64(blk * STAT_BLOCK, r1)) {
                        let row4 = reinterpret<float4 const?>(xp + r * c)
                        for (j in range64(c4)) {
                            let dv = row4[j] - m4[j]
                            acc4[j] += dv * dv
                        }
                    }
                } else {
                    for (r in range64(blk * STAT_BLOCK, r1)) {
                        let row4 = reinterpret<float4 const?>(xp + r * c)
                        for (j in range64(c4)) {
                            acc4[j] += row4[j]
                        }
                    }
                }
            }
        }
    }
    out |> reserve_resize(c)
    for (ci in range64(c)) {
        var s = 0.0lf
        for (k in range64(nblk)) {
            s += double(part[k * c + ci])
        }
        out[ci] = float(s)
    }
}

// The per-channel affine AdaIN folds to: the instance-norm statistics of `x` over the rows,
// the norm's own scale and shift, then (1 + gamma) and beta.
def private adain_affine(a : TtsAdaIn; x : array<float>; t, c : int64; style : array<float>; eps : float;
                         var scale : array<float>; var shift : array<float>) {
    var inscope @scratch h : array<float>
    var inscope @scratch mean : array<float>
    var inscope @scratch ssq : array<float>
    linear_vec(a.fc, style, h)
    column_sums(x, t, c, h, false, mean)
    let inv_t = 1.0 / float(t)
    for (ci in range64(c)) {
        mean[ci] *= inv_t
    }
    column_sums(x, t, c, mean, true, ssq)
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
    rows_width_check(c, "adain_rows")
    var inscope @scratch scale : array<float>
    var inscope @scratch shift : array<float>
    adain_affine(a, x, t, c, style, eps, scale, shift)
    var xp = unsafe(addr(x[0]))
    let s4 = unsafe(addr<float4 const?>(scale[0]))
    let b4 = unsafe(addr<float4 const?>(shift[0]))
    let chunks = lanes_for_work(t * c, 0)
    maybe_parallel_for(0, int(t), chunks) $(rb, re) {
        unsafe {
            let c4 = c / 4l
            for (r in range64(int64(rb), int64(re))) {
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
def adain_rows_into(a : TtsAdaIn; src : array<float>; @exact_size var dst : array<float>; t, c : int64; style : array<float>; eps : float) {
    rows_width_check(c, "adain_rows_into")
    var inscope @scratch scale : array<float>
    var inscope @scratch shift : array<float>
    adain_affine(a, src, t, c, style, eps, scale, shift)
    dst |> reserve_resize(t * c)
    let xp = unsafe(addr<float const?>(src[0]))
    var dp = unsafe(addr(dst[0]))
    let s4 = unsafe(addr<float4 const?>(scale[0]))
    let b4 = unsafe(addr<float4 const?>(shift[0]))
    let chunks = lanes_for_work(t * c, 0)
    maybe_parallel_for(0, int(t), chunks) $(rb, re) {
        unsafe {
            let c4 = c / 4l
            for (r in range64(int64(rb), int64(re))) {
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
def sum_rows_into(@exact_size var y : array<float>; a, b : array<float>; n : int64) {
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
def copy_rows(@exact_size var dst : array<float>; src : array<float>; n : int64) {
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
    var inscope @scratch h : array<float>
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
    var inscope @scratch h : array<float>
    var inscope @scratch cst : array<float>
    var inscope @scratch xin : array<float>
    var inscope @scratch gates : array<float>
    var inscope @scratch gates2 : array<float>
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
def bilstm(l : TtsBiLstm; x : array<float>; t : int64; @exact_size var y : array<float>) {
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
    var inscope @scratch inv : array<float>
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
def resize_nearest(x : array<float>; c, t, factor : int64; @exact_size var y : array<float>) {
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

//! Fused multiply-add through double: a * b is exact there and the sum rounds once to double,
//! then to float - the reference kernel's fma to within a double-rounding tie.
def private fmaf(a, b, c : float) : float {
    return float(double(a) * double(b) + double(c))
}

//! Linear resample of x [C][T] by `scale`: bit for bit onnxruntime's trilinear kernel on a
//! [1][C][T] tensor (ONNX half_pixel; a depth of one halves both depth weights; the unscaled
//! channel axis weights its own row one and the next zero, the last row averaging itself).
def resize_linear(x : array<float>; c, t : int64; scale : float; @exact_size var y : array<float>) : int64 {
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
def resize_linear_torch(x : array<float>; c, t : int64; scale : float; @exact_size var y : array<float>) : int64 {
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
def align_expand_rows(x : array<float>; t, c : int64; dur : array<int>; @exact_size var y : array<float>) : int64 {
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
def align_expand_channels(x : array<float>; c, t : int64; dur : array<int>; @exact_size var y : array<float>) : int64 {
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

def private source_resize(cfg : SineSourceCfg; x : array<float>; c, t : int64; scale : float; @exact_size var y : array<float>) : int64 {
    return cfg.torch_math ? resize_linear_torch(x, c, t, scale, y) : resize_linear(x, c, t, scale, y)
}

def sine_source(cfg : SineSourceCfg; f0 : array<float>; frames : int64; noise_u, noise_n : array<float>;
                @exact_size var out : array<float>) : int64 {
    let n = frames * cfg.upsample
    let nh = cfg.n_harm
    var inscope @exact_size rad : array<float>
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
    var inscope @exact_size low : array<float>
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
    var inscope @exact_size phase : array<float>
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
    delete rad
    delete low
    delete phase
    return n
}

// ===== STFT as convolution =====

//! Magnitude and phase from the two conv halves re/im [bins][frames] -> out [2*bins][frames]
//! (magnitude rows first). A zero imaginary part reads as +0, so a negative real axis is +pi:
//! torch.angle's branch, which a conv's -0 sum would otherwise flip to -pi.
def magnitude_phase(re, im : array<float>; bins, frames : int64; eps : float; @exact_size var out : array<float>) {
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
def polar_to_rect(x : array<float>; bins, frames : int64; @exact_size var re : array<float>; @exact_size var im : array<float>) {
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
def polar_to_rect_rows(x : array<float>; stride, bins, frames : int64; @exact_size var re : array<float>; @exact_size var im : array<float>) {
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
def pad_edge(x : array<float>; n, pad : int64; @exact_size var y : array<float>) {
    y |> reserve_resize(n + 2l * pad)
    for (i in range64(n + 2l * pad)) {
        y[i] = x[clamp(i - pad, 0l, n - 1l)]
    }
}

//! Reflection-pad a mono signal by `pad` samples on both sides (torch.stft's center pad).
def pad_reflect(x : array<float>; n, pad : int64; @exact_size var y : array<float>) {
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
def pad_reflect_left(x : array<float>; c, t, left : int64; @exact_size var y : array<float>) {
    let t_out = t + left
    y |> reserve_resize(c * t_out)
    for (ci in range64(c)) {
        for (to in range64(t_out)) {
            let src = to - left
            y[ci * t_out + to] = x[ci * t + abs(src)]
        }
    }
}

//! The rows twin of `pad_reflect_left`: y [t + left][c] from x [t][c].
def pad_reflect_left_rows(x : array<float>; t, c, left : int64; @exact_size var y : array<float>) {
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
