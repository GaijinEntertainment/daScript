options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_tts_blocks shared public

require dasllama/dasllama_lint public
require math
require dasllama/dasllama_math
require dasllama/dasllama_tower
require dasllama/dasllama_common

//! The StyleTTS2-lineage block home: every operator the TTS families assemble, on f32
//! channel-major [C][T] buffers (a "row" form takes token-major [T][C] where the reference
//! keeps that layout). One home: a family file re-implementing one of these is a defect.
//! Convolutions run as im2col + the batched GEMM; the depthwise forms run direct.

// ===== weights =====

//! One 1-D convolution, forward or transposed. `w` is the ONNX layout - [cout][cin][k] for a
//! forward conv, [cin][cout][k] transposed; `wt` is the transposed conv's GEMM form
//! [cout*k][cin], filled at load by `conv1d_prepare`.
struct TtsConv1d {
    w : array<float>
    wt : array<float>
    b : array<float>
    cout : int64
    cin : int64
    k : int64
    stride : int64 = 1l
    pad_l : int64
    pad_r : int64
    dilation : int64 = 1l
    groups : int64 = 1l
    output_padding : int64
    transposed : bool
}

//! A dense layer, w [nout][nin] with an optional bias.
struct TtsLinear {
    w : array<float>
    b : array<float>
    nout : int64
    nin : int64
}

//! A normalization's affine pair; empty arrays mean identity.
struct TtsNorm {
    w : array<float>
    b : array<float>
}

//! One LSTM direction, gates in i,f,g,o order: w_ih [4H][nin], w_hh [4H][H], b [4H] (the two
//! bias halves summed at conversion).
struct TtsLstm {
    w_ih : array<float>
    w_hh : array<float>
    b : array<float>
    nin : int64
    hidden : int64
}

struct TtsBiLstm {
    fwd : TtsLstm
    bwd : TtsLstm
}

//! AdaIN1d: instance norm over time per channel, then (1 + gamma) * x + beta with gamma and
//! beta the two halves of fc(style).
struct TtsAdaIn {
    fc : TtsLinear
    norm : TtsNorm
}

//! AdaLayerNorm: layer norm over channels per position, then the same affine from fc(style).
struct TtsAdaLayerNorm {
    fc : TtsLinear
}

// ===== sizing =====

//! Output length of `c` over `t_in` positions.
def conv1d_out_len(c : TtsConv1d; t_in : int64) : int64 {
    if (c.transposed) {
        return (t_in - 1l) * c.stride - c.pad_l - c.pad_r + c.dilation * (c.k - 1l) + c.output_padding + 1l
    }
    return (t_in + c.pad_l + c.pad_r - c.dilation * (c.k - 1l) - 1l) / c.stride + 1l
}

//! Build the transposed conv's GEMM weight [cout*k][cin] from the ONNX [cin][cout][k] layout.
def conv1d_prepare(var c : TtsConv1d) {
    return if (!c.transposed || c.groups != 1l)
    c.wt |> reserve_resize(c.cout * c.k * c.cin)
    for (ci in range64(c.cin)) {
        for (co in range64(c.cout)) {
            for (kk in range64(c.k)) {
                c.wt[(co * c.k + kk) * c.cin + ci] = c.w[(ci * c.cout + co) * c.k + kk]
            }
        }
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
    y |> reserve_resize(c * t)
    for (ti in range64(t)) {
        let row = ti * c
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

// ===== dense =====

//! y [t][nout] = x [t][nin] . w^T + b
def linear_rows(l : TtsLinear; x : array<float>; t : int64; @exact_size var y : array<float>) {
    y |> reserve_resize(t * l.nout)
    matmul_batch(y, l.w, x, l.nin, l.nout, t)
    return if (empty(l.b))
    for (r in range64(t)) {
        let row = r * l.nout
        for (o in range64(l.nout)) {
            y[row + o] += l.b[o]
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
