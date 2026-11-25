options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_tts_blocks
require dasllama/dasllama_tts_slab   // tts_slab_build, tts_slab_linear: the tower slab's read of a K-quant linear
require dasllama/dasllama_math   // with_dasllama_jobque_
require dasllama/dasllama_tower
require dasllama/dasllama_kqformat   // KqFmt, kq_schema_id: the K-quant conv cells' formats
require dasllama/dasllama_convert    // dequant_kq_plane_sb: the weights a K-quant conv's planes read back as
require _kq_fixtures   // the salted k4 planes and the leaf per row: the Q8_K rows and the disk-order dot
require _compares   // check / check_count at the dot-envelope bar
require _kernel_oracles   // layernorm_ref, attention_bidir_ref, gelu_form_ref: the BERT cells' double forms
require daslib/jobque_boost
require daslib/f16_cvt   // f32_to_f16 / f16_to_f32: the half-float conv cell rounds its weights to halves
require daslib/defer
require daslib/rtti   // this_context().last_exception: a refusal read as text
require strings
require math

// The TTS block home's two layouts against each other: every rows-form kernel (token-major
// [T][C], tap-GEMMs and row-block dispatch) is held to its channel-major twin, the oracle,
// at the dot-envelope tolerance - |rows - ref| <= REL * (sum of |w|*|x| feeding that output)
// plus an absolute floor. The envelope comes from the reference conv run over |w| and |x|.
// Shapes are the generator's: k3/k7/k11 at dilations 1/3/5, the two transposed upsamplers,
// and the padded-width conv_post. Sizes are large enough that the row blocks dispatch.

let REL = 1.0e-4
let FLOOR = 1.0e-6
let T_ROWS = 2048l
let STYLE = 32l
let KQ_REL = 2.0e-4   //! the K-quant lane against its own leaf: the repacked tile sums the superblocks' scale products in its own order (the disk-order arm is bit-exact)

def private rand_fill(var r : TtsRng; var a : array<float>; n : int64; scale : float) {
    a |> resize(n)
    for (i in range64(n)) {
        a[i] = (rng_uniform(r) * 2.0 - 1.0) * scale
    }
}

def private abs_of(x : array<float>) : array<float> {
    var y : array<float>
    y |> resize(length(x))
    for (src, dst in x, y) {
        dst = abs(src)
    }
    return <- y
}

// A raw conv - ONNX weight and geometry, no served layout minted yet.
def private make_conv(var r : TtsRng; cin, cout, k, stride, pad, dil : int64; transposed, with_bias : bool) : TtsConv1d {
    var c = TtsConv1d()
    c.cin = cin
    c.cout = cout
    c.k = k
    c.stride = stride
    c.pad_l = pad
    c.pad_r = pad
    c.dilation = dil
    c.groups = 1l
    c.transposed = transposed
    rand_fill(r, c.w, cin * cout * k, 1.0 / sqrt(float(cin * k)))
    if (with_bias) {
        rand_fill(r, c.b, cout, 0.5)
    }
    return <- c
}

// The raw conv prepared for one consumer: the rows form or the channel-major reference.
def private prepared(c : TtsConv1d; rows : bool; q8 : bool = false) : TtsConv1d {
    var p := c
    p.served_rows = rows
    conv1d_prepare(p, q8, q8 && select_matmul_backend_for_load_())
    return <- p
}

// The same conv over |w| with no bias: its output is the dot envelope of every output element.
def private envelope_conv(c : TtsConv1d) : TtsConv1d {
    var e = TtsConv1d()
    e.cin = c.cin
    e.cout = c.cout
    e.k = c.k
    e.stride = c.stride
    e.pad_l = c.pad_l
    e.pad_r = c.pad_r
    e.dilation = c.dilation
    e.groups = c.groups
    e.transposed = c.transposed
    e.w <- abs_of(c.w)
    conv1d_prepare(e)
    return <- e
}

//! the dot-envelope bar: `rel` of the element's envelope `env`, floored at FLOOR; its control moves the last expected element
//! by twice the bar there, just past it
def private envelope_bar(rel : float; env : array<float>) : Bar {
    let at = length(env) - 1
    return Bar(kind = BarKind.envelope, rel = rel, abs_floor = FLOOR, poison = 2.0 * max(FLOOR, rel * env[at]), poison_at = at)
}

//! the envelope with the bias's largest magnitude added to every element: what the dot-envelope bar scales
def private biased(env : array<float>; bias_abs : float) : array<float> => [for (e in env); e + bias_abs]

//! a case's compare at the dot-envelope bar `rel` over `env` plus `bias_abs`, with the bar's poisoned-expectation control
def private check_envelope(t : T?; name : string; got, want, env : array<float>; bias_abs : float; rel : float = REL) {
    var inscope env_b <- biased(env, bias_abs)
    check(t, "tts blocks {name}", got, want, envelope_bar(rel, env_b), FLAT_SHAPE, env_b)
}

//! a rows conv against its channel-major twin; `poison_tap` >= 0 zeroes that tap of the rows form first, and the case then
//! asserts elements past the bar instead - the input-poison control
def private conv_case(t : T?; name : string; cin, cout, k, stride, pad, dil : int64; transposed : bool;
                      t_in : int64 = T_ROWS; poison_tap : int64 = -1l) {
    var r = TtsRng()
    rng_seed(r, 0x5eedul + uint64(k * 100l + dil))
    var inscope c0 <- make_conv(r, cin, cout, k, stride, pad, dil, transposed, true)
    var inscope e <- envelope_conv(c0)
    var inscope c <- prepared(c0, false)
    var inscope cr <- prepared(c0, true)
    if (poison_tap >= 0l) {
        for (i in range64(poison_tap * cr.cin * cr.cout_s, (poison_tap + 1l) * cr.cin * cr.cout_s)) {
            cr.wk[i] = 0.0
        }
    }
    var inscope x_ct : array<float>
    rand_fill(r, x_ct, cin * t_in, 2.0)
    var inscope x_abs <- abs_of(x_ct)
    var inscope want, env : array<float>
    conv1d(c, x_ct, t_in, want)
    conv1d(e, x_abs, t_in, env)
    var inscope x_rows : array<float>
    transpose_ct(x_ct, cin, t_in, x_rows)
    var inscope y_rows : array<float>
    conv1d_rows(cr, x_rows, t_in, y_rows)
    let t_out = conv1d_out_len(cr, t_in)
    var inscope got : array<float>
    transpose_tc_strided(y_rows, t_out, cout, cr.cout_s, got)
    if (poison_tap < 0l) {
        check_envelope(t, name, got, want, env, logits_maxabs(c0.b))
        return
    }
    var inscope env_b <- biased(env, logits_maxabs(c0.b))
    let bad = check_count(got, want, envelope_bar(REL, env_b), env_b)
    to_log(LOG_INFO, "tts blocks POISON {name}: {bad} elements past the bar\n")
    t |> success(bad > 0, "{name}: the poisoned tap must put elements past the bar, {bad} did")
}

def private snake_case(tc : T?) {
    var r = TtsRng()
    rng_seed(r, 77ul)
    let c = 64l
    let t = T_ROWS
    var inscope alpha : array<float>
    rand_fill(r, alpha, c, 1.0)
    for (i in range64(c)) {
        alpha[i] = 0.25 + abs(alpha[i])
    }
    var inscope x_ct : array<float>
    rand_fill(r, x_ct, c * t, 3.0)
    var inscope x_rows : array<float>
    transpose_ct(x_ct, c, t, x_rows)
    snake(x_ct, c, t, alpha)
    snake_rows(x_rows, t, c, alpha)
    var inscope got : array<float>
    transpose_tc(x_rows, t, c, got)
    var inscope env : array<float>
    env |> resize(c * t)
    for (i in range64(c * t)) {
        env[i] = abs(x_ct[i]) + 4.0   // |x| + 1/alpha's reach, the loose bound sin^2/alpha never exceeds
    }
    check_envelope(tc, "snake", got, x_ct, env, 0.0)
}

def private adain_case(tc : T?; c : int64) {
    var r = TtsRng()
    rng_seed(r, 91ul + uint64(c))
    let t = T_ROWS
    var a = TtsAdaIn()
    a.fc.nin = STYLE
    a.fc.nout = 2l * c
    rand_fill(r, a.fc.w, STYLE * 2l * c, 0.2)
    rand_fill(r, a.fc.b, 2l * c, 0.2)
    rand_fill(r, a.norm.w, c, 1.0)
    rand_fill(r, a.norm.b, c, 0.5)
    var inscope style : array<float>
    rand_fill(r, style, STYLE, 1.0)
    var inscope x_ct : array<float>
    rand_fill(r, x_ct, c * t, 3.0)
    for (i in range64(c * t)) {
        x_ct[i] += float(i % 7l)   // a per-channel offset so the mean is not near zero
    }
    var inscope x_rows : array<float>
    transpose_ct(x_ct, c, t, x_rows)
    adain(a, x_ct, c, t, style, 1.0e-5)
    adain_rows(a, x_rows, t, c, style, 1.0e-5)
    var inscope got : array<float>
    transpose_tc(x_rows, t, c, got)
    var inscope env : array<float>
    env |> resize(c * t)
    for (i in range64(c * t)) {
        env[i] = abs(x_ct[i]) + 8.0   // the normalized value's scale plus the affine's reach
    }
    check_envelope(tc, "adain c={c}", got, x_ct, env, 0.0)
}

def private pad_and_polar_case(tc : T?) {
    var r = TtsRng()
    rng_seed(r, 5ul)
    let c = 16l
    let t = 300l
    var inscope x_ct : array<float>
    rand_fill(r, x_ct, c * t, 1.0)
    var inscope x_rows : array<float>
    transpose_ct(x_ct, c, t, x_rows)
    var inscope want : array<float>
    var inscope y_rows := x_rows
    pad_reflect_left(x_ct, c, t, 1l, want)
    pad_reflect_left_rows_inplace(y_rows, t, c, 1l)
    var inscope got : array<float>
    transpose_tc(y_rows, t + 1l, c, got)
    var inscope env : array<float>
    env |> resize(length(want))
    for (i in range(length(want))) {
        env[i] = 0.0
    }
    check_envelope(tc, "pad_reflect_left", got, want, env, 0.0)
    // polar: rows of stride 32 carrying 2 * 11 columns
    let bins = 11l
    var inscope p_ct : array<float>
    rand_fill(r, p_ct, 2l * bins * t, 1.0)
    var inscope p_rows : array<float>
    p_rows |> resize(32l * t)
    for (i in range64(32l * t)) {
        p_rows[i] = 99.0   // padding columns carry garbage the rows form must ignore
    }
    for (f in range64(t)) {
        for (b in range64(2l * bins)) {
            p_rows[f * 32l + b] = p_ct[b * t + f]
        }
    }
    var inscope re, im, re2, im2 : array<float>
    polar_to_rect(p_ct, bins, t, re, im)
    polar_to_rect_rows(p_rows, 32l, bins, t, re2, im2)
    var inscope env2 : array<float>
    env2 |> resize(bins * t)
    for (i in range64(bins * t)) {
        env2[i] = 0.0
    }
    check_envelope(tc, "polar re", re2, re, env2, 0.0)
    check_envelope(tc, "polar im", im2, im, env2, 0.0)
}

// linear_rows on the tiled path (wt minted) against the same layer on the batched dot form.
def private linear_case(tc : T?; nin, nout : int64) {
    var r = TtsRng()
    rng_seed(r, 0xabcul + uint64(nin))
    let t = T_ROWS
    var l = TtsLinear(nin = nin, nout = nout)
    rand_fill(r, l.w, nout * nin, 1.0 / sqrt(float(nin)))
    rand_fill(r, l.b, nout, 0.5)
    var e = TtsLinear(nin = nin, nout = nout, w <- abs_of(l.w))
    var inscope x : array<float>
    rand_fill(r, x, t * nin, 2.0)
    var inscope x_abs <- abs_of(x)
    var inscope want, env : array<float>
    linear_rows(l, x, t, want)   // no wt yet: the dot form
    linear_rows(e, x_abs, t, env)
    linear_prepare(l)
    var inscope got : array<float>
    linear_rows(l, x, t, got)
    check_envelope(tc, "linear {nin}x{nout}", got, want, env, logits_maxabs(l.b))
    tc |> success(!empty(l.wt), "linear {nin}x{nout}: the tiled path engaged")
}

// the residual block's upsampling pool (depthwise transposed k3 s2) and the nearest copy of its
// shortcut, rows against channel-major
def private pool_and_nearest_case(tc : T?) {
    var r = TtsRng()
    rng_seed(r, 0x7001ul)
    let c = 64l
    let t = 300l
    var inscope pool = TtsConv1d(cin = c, cout = c, k = 3l, stride = 2l, pad_l = 1l, pad_r = 1l, dilation = 1l, groups = c, transposed = true, output_padding = 1l)
    rand_fill(r, pool.w, c * 3l, 1.0)
    rand_fill(r, pool.b, c, 0.5)
    var inscope e = TtsConv1d(cin = c, cout = c, k = 3l, stride = 2l, pad_l = 1l, pad_r = 1l, dilation = 1l, groups = c, transposed = true, output_padding = 1l)
    e.w <- abs_of(pool.w)
    conv1d_prepare(pool)
    conv1d_prepare(e)
    var inscope x_ct : array<float>
    rand_fill(r, x_ct, c * t, 2.0)
    var inscope x_abs <- abs_of(x_ct)
    var inscope want, env : array<float>
    conv1d(pool, x_ct, t, want)
    conv1d(e, x_abs, t, env)
    var inscope x_rows : array<float>
    transpose_ct(x_ct, c, t, x_rows)
    var inscope y_rows : array<float>
    conv1d_rows_transposed_depthwise(pool, x_rows, t, y_rows)
    let t_out = conv1d_out_len(pool, t)
    var inscope got : array<float>
    transpose_tc(y_rows, t_out, c, got)
    check_envelope(tc, "pool convT depthwise", got, want, env, logits_maxabs(pool.b))
    var inscope want2 : array<float>
    resize_nearest(x_ct, c, t, 2l, want2)
    var inscope y2 : array<float>
    resize_nearest_rows(x_rows, t, c, 2l, y2)
    var inscope got2 : array<float>
    transpose_tc(y2, 2l * t, c, got2)
    var inscope env2 : array<float>
    env2 |> resize(length(want2))
    for (i in range(length(want2))) {
        env2[i] = 0.0
    }
    check_envelope(tc, "nearest x2", got2, want2, env2, 0.0)
}

// a conv and an AdaIN padded from 66 to 96 input channels against the unpadded pair on the same
// rows: the padded columns stay zero and the real ones agree exactly
def private pad_case(tc : T?) {
    var r = TtsRng()
    rng_seed(r, 0x9adul)
    let cin = 66l
    let cs = 96l
    let cout = 64l
    let t = 300l
    var inscope c0 <- make_conv(r, cin, cout, 3l, 1l, 1l, 1l, false, true)
    var inscope cp := c0
    conv1d_pad_cin(cp, cs)
    var inscope c <- prepared(c0, true)
    cp.served_rows = true
    conv1d_prepare(cp)
    var inscope x : array<float>
    rand_fill(r, x, t * cin, 2.0)
    var inscope xp : array<float>
    xp |> resize(t * cs)
    for (i in range64(t * cs)) {
        let j = i % cs
        xp[i] = j < cin ? x[(i / cs) * cin + j] : 0.0
    }
    var inscope y, yp : array<float>
    conv1d_rows(c, x, t, y)
    conv1d_rows(cp, xp, t, yp)
    var inscope env : array<float>
    env |> resize(length(y))
    for (i in range(length(y))) {
        env[i] = 0.0
    }
    check_envelope(tc, "padded conv", yp, y, env, 0.0)
    var a = TtsAdaIn()
    a.fc.nin = STYLE
    a.fc.nout = 2l * cin
    rand_fill(r, a.fc.w, STYLE * 2l * cin, 0.2)
    rand_fill(r, a.fc.b, 2l * cin, 0.2)
    rand_fill(r, a.norm.w, cin, 1.0)
    rand_fill(r, a.norm.b, cin, 0.5)
    var ap := a
    adain_pad(ap, cin, cs)
    var inscope style : array<float>
    rand_fill(r, style, STYLE, 1.0)
    var inscope xa := x
    var inscope xap := xp
    adain_rows(a, xa, t, cin, style, 1.0e-5)
    adain_rows(ap, xap, t, cs, style, 1.0e-5)
    var inscope got : array<float>
    got |> resize(t * cin)
    var pad_nonzero = 0
    for (i in range64(t * cs)) {
        let j = i % cs
        if (j < cin) {
            got[(i / cs) * cin + j] = xap[i]
        } elif (xap[i] != 0.0) {
            pad_nonzero++
        }
    }
    var inscope env2 : array<float>
    env2 |> resize(t * cin)
    for (i in range64(t * cin)) {
        env2[i] = 0.0
    }
    check_envelope(tc, "padded adain", got, xa, env2, 0.0)
    tc |> equal(pad_nonzero, 0, "padded adain: nonzero padded columns")
}

// attention_rows against a scalar per-head reference, at a token count off the 16-key pad.
def private attention_case(tc : T?) {
    var r = TtsRng()
    rng_seed(r, 0x47ul)
    let t = 37l
    let heads = 4l
    let dh = 16l
    let hidden = heads * dh
    var inscope q, k, v : array<float>
    rand_fill(r, q, t * hidden, 1.0)
    rand_fill(r, k, t * hidden, 1.0)
    rand_fill(r, v, t * hidden, 2.0)
    var inscope want : array<float>
    want |> resize(t * hidden)
    var inscope env : array<float>
    env |> resize(t * hidden)
    var inscope p : array<float>
    p |> resize(t)
    let scale = 1.0 / sqrt(float(dh))
    for (hd in range64(heads)) {
        let off = hd * dh
        for (i in range64(t)) {
            var mx = -1e30
            for (j in range64(t)) {
                var s = 0.0
                for (e in range64(dh)) {
                    s += q[i * hidden + off + e] * k[j * hidden + off + e]
                }
                p[j] = s * scale
                mx = max(mx, p[j])
            }
            var sum = 0.0
            for (j in range64(t)) {
                p[j] = exp(p[j] - mx)
                sum += p[j]
            }
            for (e in range64(dh)) {
                var acc = 0.0
                var reach = 0.0
                for (j in range64(t)) {
                    acc += p[j] * v[j * hidden + off + e]
                    reach += p[j] * abs(v[j * hidden + off + e])
                }
                want[i * hidden + off + e] = acc / sum
                env[i * hidden + off + e] = reach / sum
            }
        }
    }
    var inscope got : array<float>
    attention_rows(q, k, v, t, hidden, heads, got)
    check_envelope(tc, "attention", got, want, env, 0.0)
}

let Q8_REL = 2.0e-2   // Q8_0 weights and per-32 activations: a few thousandths of the envelope, gated at 2%
let KQ_SOURCE_REL = 0.1   // a K-quant plane's dequant against its source weights: Q4_K's sub-block error reads 2% of the envelope, gated at 10%

// a conv minted in the K-quant rows form `fmt`, repacked for the backend where `repack` asks and the backend carries kq
def private prepared_kq(c : TtsConv1d; fmt : KqFmt; repack : bool) : TtsConv1d {
    var p := c
    p.served_rows = true
    conv1d_prepare_kq(p, fmt, repack && select_matmul_backend_for_load_())
    return <- p
}

// the f32 conv whose weights are the ones `ck`'s disk-order K-quant planes dequantize to; a column past k*cin is read by
// nothing - its activation is a stacked zero - so what it dequantizes to (a sub-block's min, where it shares one) is not held
def private dequantized_conv(c0, ck : TtsConv1d; fmt : KqFmt) : TtsConv1d {
    var cd := c0
    var inscope slab : array<float>
    let rows = ck.transposed ? ck.k * ck.cout : ck.cout   // a transposed conv's matrix is its lift, row tap * cout + co over the inputs
    slab |> resize(rows * ck.kq_k)
    for (sb in range64(rows * ck.kq_k / 256l)) {
        dequant_kq_plane_sb(fmt, ck.kq, ck.ks, sb, false, slab, sb * 256l)
    }
    for (co in range64(ck.cout)) {
        for (kk in range64(ck.k)) {
            for (ci in range64(ck.cin)) {
                if (ck.transposed) {
                    cd.w[(ci * ck.cout + co) * ck.k + kk] = slab[(kk * ck.cout + co) * ck.kq_k + ci]
                } else {
                    cd.w[(co * ck.cin + ci) * ck.k + kk] = slab[co * ck.kq_k + kk * ck.cin + ci]
                }
            }
        }
    }
    return <- cd
}

// every row of a quantized conv read back through `conv1d_weight_row` - the GPU slab's read - against the same rows of the
// f32 conv `ref`, each within `bound` of its own row's largest weight; the next output channel's row is the control
def private weight_rows_case(t : T?; name : string; cq, cref : TtsConv1d; bound : float) {
    let n = cq.k * cq.cin
    var inscope got, want, other : array<float>
    got |> resize(n)
    want |> resize(n)
    other |> resize(n)
    var worst = 0.0
    var nearest_other = INFINITE_DIFF
    for (co in range64(cq.cout)) {
        conv1d_weight_row(cq, co, got, 0l)
        conv1d_weight_row(cref, co, want, 0l)
        conv1d_weight_row(cref, (co + 1l) % cq.cout, other, 0l)
        let peak = logits_maxabs(want)
        worst = max(worst, peak > 0.0 ? logits_maxdiff(got, want) / peak : INFINITE_DIFF)
        nearest_other = min(nearest_other, peak > 0.0 ? logits_maxdiff(got, other) / peak : 0.0)
    }
    to_log(LOG_INFO, "tts blocks {name}: {cq.cout} weight rows read back, worst {worst} of a row's peak (bound {bound}); the nearest next row {nearest_other}\n")
    t |> success(worst <= bound, "{name}: every weight row reads back within {bound} of its peak: worst {worst}")
    t |> success(nearest_other > bound && nearest_other > worst, "{name}: the next channel's row lands outside that bound (the read's control): {nearest_other}")
}

// The K-quant rows lane of a conv against the channel-major f32 conv over the weights its own planes dequantize to, at
// the q8 bar (the rows requantize to Q8_K); the f32 conv over the weights before quantization is the control it sits
// further from.
def private conv_case_kq(t : T?; name : string; fmt : KqFmt; cin, cout, k, stride, pad, dil : int64; repack : bool; transposed : bool = false;
                         channel_major : bool = false; x_scale : float = 2.0) {
    var r = TtsRng()
    rng_seed(r, 0x9ee4ul + uint64(k * 100l + dil))
    var inscope c0 <- make_conv(r, cin, cout, k, stride, pad, dil, transposed, true)
    var inscope ck <- prepared_kq(c0, fmt, false)
    var inscope cd <- dequantized_conv(c0, ck, fmt)
    var inscope e <- envelope_conv(cd)
    var inscope c <- prepared(cd, false)
    var inscope cf <- prepared(c0, false)
    var inscope cs <- prepared_kq(c0, fmt, repack)
    let t_in = T_ROWS
    var inscope x_ct : array<float>
    rand_fill(r, x_ct, cin * t_in, x_scale)
    var inscope x_abs <- abs_of(x_ct)
    var inscope want, env, before : array<float>
    conv1d(c, x_ct, t_in, want)
    conv1d(e, x_abs, t_in, env)
    conv1d(cf, x_ct, t_in, before)
    var inscope got : array<float>
    if (channel_major) {   // the channel-major entry over the same matrix
        conv1d(cs, x_ct, t_in, got)
    } else {
        var inscope x_rows : array<float>
        transpose_ct(x_ct, cin, t_in, x_rows)
        var inscope y_rows : array<float>
        conv1d_rows(cs, x_rows, t_in, y_rows)
        transpose_tc_strided(y_rows, conv1d_out_len(cs, t_in), cout, cs.cout_s, got)
    }
    check_envelope(t, name, got, want, env, logits_maxabs(c0.b), Q8_REL)
    t |> equal(cs.kq_fmt, kq_schema_id(fmt), "{name}: the K-quant lane minted")
    t |> equal(cs.kq_k % 256l, 0l, "{name}: the matrix's row is whole superblocks ({cs.kq_k})")
    t |> equal(cs.kq_repacked, repack && kernel_backend_has_kq(), "{name}: the planes are repacked exactly where the arm asks and the backend carries kq")
    // the planes hold the source's weights, every row of them: a row the mint left unwritten reads as zeros here
    check_envelope(t, "{name}: the planes' own conv against the source's", want, before, env, logits_maxabs(c0.b), KQ_SOURCE_REL)
    let to_planes = logits_maxdiff(got, want)
    let to_source = logits_maxdiff(got, before)
    t |> success(all_finite(got) && all_finite(before) && to_source > 2.0 * to_planes, "{name}: the rows read the quantized weights - {to_planes} from their dequant, {to_source} from the source")
    weight_rows_case(t, "{name}: the planes' rows", cs, c, 0.0)   // the planes' own dequant, bit for bit, in either layout
}

// A vector layer minted Q8_0 against its own f32 GEMV at the q8 bar; a width off 32 keeps `w` and the f32 arm.
def private vec_case_q8(t : T?; nin, nout : int64; mints : bool) {
    var r = TtsRng()
    rng_seed(r, 0x7ec8ul + uint64(nin * 7l + nout))
    var inscope l = TtsLinear()
    l.nin = nin
    l.nout = nout
    rand_fill(r, l.w, nin * nout, 1.0 / sqrt(float(nin)))
    rand_fill(r, l.b, nout, 0.5)
    var inscope x : array<float>
    rand_fill(r, x, nin, 2.0)
    var inscope want, env : array<float>
    linear_vec(l, x, want)
    env |> resize(nout)
    for (o in range64(nout)) {
        for (i in range64(nin)) {
            env[o] += abs(l.w[o * nin + i]) * abs(x[i])
        }
    }
    var inscope lq := l
    let minted = linear_vec_prepare_q8(lq, select_matmul_backend_for_load_())
    t |> equal(minted, mints, "vec q8 {nin}x{nout}: the lattice decides the mint")
    t |> equal(lq.q8, mints)
    t |> equal(empty(lq.w), mints, "vec q8 {nin}x{nout}: a minted layer drops w, a refused one keeps it")
    var inscope got : array<float>
    linear_vec(lq, x, got)
    check_envelope(t, "vec q8 {nin}x{nout}", got, want, env, logits_maxabs(l.b), mints ? Q8_REL : 0.0)
}

// The q8 rows lane of a conv against the channel-major f32 reference at the q8 bar.
def private conv_case_q8(t : T?; name : string; cin, cout, k, pad, dil : int64; transposed : bool = false; stride : int64 = 1l) {
    var r = TtsRng()
    rng_seed(r, 0x9eedul + uint64(k * 100l + dil))
    var inscope c0 <- make_conv(r, cin, cout, k, stride, pad, dil, transposed, true)
    var inscope e <- envelope_conv(c0)
    var inscope c <- prepared(c0, false)
    var inscope cq <- prepared(c0, true, true)
    let t_in = T_ROWS
    var inscope x_ct : array<float>
    rand_fill(r, x_ct, cin * t_in, 2.0)
    var inscope x_abs <- abs_of(x_ct)
    var inscope want, env : array<float>
    conv1d(c, x_ct, t_in, want)
    conv1d(e, x_abs, t_in, env)
    var inscope x_rows : array<float>
    transpose_ct(x_ct, cin, t_in, x_rows)
    var inscope y_rows : array<float>
    conv1d_rows(cq, x_rows, t_in, y_rows)
    let t_out = conv1d_out_len(cq, t_in)
    var inscope got : array<float>
    transpose_tc_strided(y_rows, t_out, cout, cq.cout_s, got)
    check_envelope(t, name, got, want, env, logits_maxabs(c0.b), Q8_REL)
    t |> success(cq.q8, "{name}: the q8 lane minted")
    weight_rows_case(t, "{name}: the quants' rows", cq, c, 1.0 / 127.0)   // a Q8_0 block's step is its peak over 127
    // a channel-major conv asked to take the quant matrix mints the same one, and its entry is the rows kernel between two transposes
    var inscope cm := c0
    cm.served_rows = false
    conv1d_prepare(cm, true, select_matmul_backend_for_load_(), true)
    t |> success(cm.q8, "{name}: the channel-major conv minted the quant matrix")
    var inscope got_cm : array<float>
    conv1d(cm, x_ct, t_in, got_cm)
    check(t, "tts blocks {name}: the channel-major entry against the rows form", got_cm, got, BAR_EXACT, FLAT_SHAPE)
    var inscope plain := c0
    plain.served_rows = false
    conv1d_prepare(plain, true, select_matmul_backend_for_load_())
    t |> success(!plain.q8 && !empty(plain.transposed ? plain.wt : plain.w), "{name}: a channel-major conv not asked keeps its f32 layout")
}

// linear_rows on the q8 lane against the dot form at the q8 bar.
def private linear_case_q8(tc : T?; nin, nout : int64) {
    var r = TtsRng()
    rng_seed(r, 0xdbcul + uint64(nin))
    let t = T_ROWS
    var l = TtsLinear(nin = nin, nout = nout)
    rand_fill(r, l.w, nout * nin, 1.0 / sqrt(float(nin)))
    rand_fill(r, l.b, nout, 0.5)
    var e = TtsLinear(nin = nin, nout = nout, w <- abs_of(l.w))
    var inscope x : array<float>
    rand_fill(r, x, t * nin, 2.0)
    var inscope x_abs <- abs_of(x)
    var inscope want, env : array<float>
    linear_rows(l, x, t, want)
    linear_rows(e, x_abs, t, env)
    var lq := l
    linear_prepare(lq, true, true, select_matmul_backend_for_load_())
    var inscope got : array<float>
    linear_rows(lq, x, t, got)
    check_envelope(tc, "linear q8 {nin}x{nout}", got, want, env, logits_maxabs(l.b), Q8_REL)
    tc |> success(lq.q8, "linear q8 {nin}x{nout}: the q8 lane minted")
}

// ===== the K-quant lane against the leaf per row =====

//! the leaf the lane applies per row: the row requantized to the Q8_K form, the k4 dot_kq over the disk-order planes, then the bias
def private kq_leaf_rows(kq, ks : array<uint8>; x : array<float>; t, nin, nout : int64; b : array<float>; var want : array<float>) {
    let nsb = nin / 256l
    var inscope a <- q8k_acts(x, t, nin)
    want |> resize(t * nout)
    for (p in range64(t)) {
        for (o in range64(nout)) {
            want[p * nout + o] = kq_row_dot(4, kq, ks, o, nsb, a.xq, a.xs, a.xbs, p * nin, p * nsb, p * (nin / 16l), nin) + b[o]
        }
    }
}

// linear_rows (the batched kq GEMM where the backend carries the tile, the GEMV per row otherwise)
// and linear_rows_decode (the GEMV) on the K-quant lane against the leaf per row; `repack` false
// holds the planes in disk order - the portable arm every tier without kq kernels runs; the
// control adds a fixed value to the middle expected element, which the bar must see
def private linear_case_kq(t : T?; nin, nout : int64; decode, repack : bool) {
    var r = TtsRng()
    rng_seed(r, 0xebcul + uint64(nin))
    let nt = decode ? 1l : T_ROWS
    var l = TtsLinear(nin = nin, nout = nout)
    rand_fill(r, l.b, nout, 0.5)
    var kq, ks : array<uint8>
    build_kq_region(4, kq, ks, nout, nin, 0x51ED270Bu)
    var inscope x : array<float>
    rand_fill(r, x, nt * nin, 2.0)
    var inscope want : array<float>
    kq_leaf_rows(kq, ks, x, nt, nin, nout, l.b, want)
    linear_take_kq(l, 4, kq, ks, repack)   // the planes move into the layer
    var inscope got : array<float>
    if (decode) {
        linear_rows_decode(l, x, got)
    } else {
        linear_rows(l, x, nt, got)
    }
    var inscope env : array<float>
    env |> resize(nt * nout)
    for (i in range64(nt * nout)) {
        env[i] = abs(want[i]) + 1.0
    }
    let name = "linear kq {nin}x{nout} {decode ? "decode" : "rows"}{repack ? "" : " disk-order"}"
    let engaged = l.kq_fmt == 4 && empty(l.w) && !(repack && kernel_backend_has_kq() != l.kq_repacked)
    //! the control's value is fixed: the outputs run to a few thousand, so a quarter would hide inside the relative bar
    let bar = Bar(kind = BarKind.envelope, rel = KQ_REL, abs_floor = FLOOR, poison = 4.0, poison_at = length(want) / 2)
    check(t, "tts blocks {name}", got, want, bar, Shape(row_width = int(nout)), env)
    t |> success(engaged, "{name}: the kq lane engaged, and the repack followed the backend")
}

// the tower slab's read of a K-quant linear (`tts_slab_linear`): every row at the slot's stride is the row the planes
// dequantize to in disk order, bit for bit, whichever layout the layer holds them in; the control reads the rows one down
def private slab_linear_cases_kq(t : T?) {
    let nin = 256
    let nout = 24
    for (repack in [true, false]) {
        var l = TtsLinear(nin = int64(nin), nout = int64(nout))
        var kq, ks : array<uint8>
        build_kq_region(4, kq, ks, int64(nout), int64(nin), 0x51ED270Bu)
        var inscope want : array<float>
        want |> resize(nout * nin)
        for (r in range(nout)) {
            kq_matrix_row(kq, ks, 4, false, int64(nin), int64(nout), int64(r), want, int64(r * nin), "the slab cell")
        }
        linear_take_kq(l, 4, kq, ks, repack)
        var slot : TtsConvSlot
        var inscope host <- tts_slab_build(slot) $(var w : TtsSlabWriter; var s : TtsConvSlot) {
            s = tts_slab_linear(w, l)
        }
        var off = 0
        var down = 0
        for (r in range(nout)) {
            let at = int(slot.woff + int64(r) * slot.kdim_p)
            off += mismatch_exact(host, want, at, r * nin, nin)
            down += mismatch_exact(host, want, at, (r + 1) % nout * nin, nin)
        }
        let name = "slab linear kq {nin}x{nout}{repack ? "" : " disk-order"}"
        t |> equal(off, 0, "{name}: the slab holds the planes' own rows")
        t |> success(slot.widened && slot.stored == 0, "{name}: the slot is marked widened")
        t |> success(all_finite(want) && down > 0, "{name}: the rows one down differ (the compare's control): {down}")
    }
}

//! the members a GPU seat stages as stored, as the slab writer's mask
let SLAB_STAGED = (1u << uint(KqFmt.q8)) | (1u << uint(KqFmt.k4)) | TTS_STORED_HALVES

def private slab_put(var w : TtsSlabWriter; c : TtsConv1d) : TtsConvSlot => tts_slab_conv(w, c)

def private slab_put(var w : TtsSlabWriter; l : TtsLinear) : TtsConvSlot => tts_slab_linear(w, l)

//! a conv's or a dense layer's slot off the slab writer under the stored mask `mask`
def private slab_slot(leaf; mask : uint) : TtsConvSlot {
    var slot : TtsConvSlot
    var built <- tts_slab_build_stored(slot, $(var w : TtsSlabWriter; var s : TtsConvSlot) {
        s = slab_put(w, leaf)
    }, mask)
    unsafe {
        delete built
    }
    return slot
}

//! `slot` is a stored plane (`stored`), else rows of the slab, widened or not (`widened`)
def private slot_is(t : T?; name : string; slot : TtsConvSlot; stored, widened : bool) {
    let held = stored ? "stays out of the slab" : "is rows of the slab"
    t |> equal(slot.stored != 0, stored, "{name}: the leaf {held}")
    t |> equal(slot.widened, widened, "{name}: the slot is {widened ? "" : "not "}marked widened")
}

def private slab_widened_marks(t : T?) {
    var r = TtsRng()
    rng_seed(r, 0x51ab3ul)
    for (cin in [64l, 66l]) {
        var inscope c0 <- make_conv(r, cin, 64l, 3l, 1l, 1l, 1l, false, true)
        var inscope cf <- prepared(c0, true)
        var inscope ch := c0
        ch.served_rows = true
        conv1d_prepare(ch, false, false, false, true)
        t |> success(conv1d_host_halves(ch) && !conv1d_host_halves(cf),
            "cin {cin}: one conv of halves, one of floats")
        let lattice = cin % 8l == 0l
        slot_is(t, "conv of floats, cin {cin}", slab_slot(cf, SLAB_STAGED), stored = false, widened = false)
        slot_is(t, "conv of halves, cin {cin}", slab_slot(ch, SLAB_STAGED), stored = lattice, widened = !lattice)
        slot_is(t, "conv of halves, cin {cin}, none staged", slab_slot(ch, 0u), stored = false, widened = true)
        var inscope ck <- prepared_kq(c0, KqFmt.k4, false)
        t |> success(ck.kq_fmt == kq_schema_id(KqFmt.k4) && !ck.kq_repacked && !ck.q8 && !conv1d_host_halves(ck),
            "cin {cin}: one Q4_K conv in disk order, no Q8_0 matrix and no halves beside its planes")
        slot_is(t, "Q4_K conv, cin {cin}", slab_slot(ck, SLAB_STAGED), stored = lattice, widened = !lattice)
        slot_is(t, "Q4_K conv, cin {cin}, none staged", slab_slot(ck, 0u), stored = false, widened = true)
        var inscope cr <- prepared_kq(c0, KqFmt.k4, true)
        let layout = cr.kq_repacked ? "repacked" : "in disk order (the backend carries no kq)"
        slot_is(t, "Q4_K conv, cin {cin}, {layout}", slab_slot(cr, SLAB_STAGED),
            stored = lattice && !cr.kq_repacked, widened = !lattice || cr.kq_repacked)
    }
    var inscope q0 <- make_conv(r, 128l, 128l, 3l, 1l, 1l, 1l, false, true)
    var inscope cq <- prepared(q0, true, true)
    t |> success(cq.q8, "the Q8_0 conv holds its quant matrix")
    slot_is(t, "Q8_0 conv", slab_slot(cq, SLAB_STAGED), stored = true, widened = false)
    slot_is(t, "Q8_0 conv, no member staged", slab_slot(cq, 0u), stored = false, widened = true)
    for (nin in [64l, 36l]) {
        var inscope lf = TtsLinear(nin = nin, nout = 24l)
        rand_fill(r, lf.w, nin * 24l, 1.0)
        var inscope lh := lf
        linear_prepare(lh, false, false, false, true, true)
        t |> success(linear_host_halves(lh) && !linear_host_halves(lf),
            "nin {nin}: one layer of halves, one of floats")
        let lattice = nin % 32l == 0l
        slot_is(t, "layer of floats, nin {nin}", slab_slot(lf, SLAB_STAGED), stored = false, widened = false)
        slot_is(t, "layer of halves, nin {nin}", slab_slot(lh, SLAB_STAGED), stored = lattice, widened = !lattice)
    }
    for (repack in [true, false]) {
        var inscope lk = TtsLinear(nin = 256l, nout = 24l)
        var kq, ks : array<uint8>
        build_kq_region(4, kq, ks, 24l, 256l, 0x51ED270Bu)
        linear_take_kq(lk, 4, kq, ks, repack)
        let name = "K-quant dense layer{lk.kq_repacked ? ", repacked" : ", disk order"}"
        slot_is(t, name, slab_slot(lk, SLAB_STAGED), stored = !lk.kq_repacked, widened = lk.kq_repacked)
    }
}

//! a width the 256-superblock does not divide is refused by name
def private kq_width_refusal(t : T?) {
    var why = "no panic"
    try {
        var l = TtsLinear(nin = 100l, nout = 8l)
        var kq, ks : array<uint8>
        linear_take_kq(l, 4, kq, ks, false)
    } recover {
        why = this_context().last_exception |> rtrim
    }
    t |> success(why |> find("needs nin on 256") >= 0, "linear kq width refusal: the layer's panic, got '{why}'")
}

// 66 input channels padded to 96 quantize per 32, so the padded conv reaches the q8 lane: the
// padded rows form against the unpadded channel-major reference at the q8 bar.
def private pad_case_q8(tc : T?) {
    var r = TtsRng()
    rng_seed(r, 0x9ad8ul)
    let cin = 66l
    let cs = 96l
    let cout = 64l
    let t = T_ROWS
    var inscope c0 <- make_conv(r, cin, cout, 3l, 1l, 1l, 1l, false, true)
    var inscope e <- envelope_conv(c0)
    var inscope c <- prepared(c0, false)
    var inscope cp := c0
    conv1d_pad_cin(cp, cs)
    cp.served_rows = true
    conv1d_prepare(cp, true, select_matmul_backend_for_load_())
    var inscope x_ct : array<float>
    rand_fill(r, x_ct, cin * t, 2.0)
    var inscope x_abs <- abs_of(x_ct)
    var inscope want, env : array<float>
    conv1d(c, x_ct, t, want)
    conv1d(e, x_abs, t, env)
    var inscope x_rows : array<float>
    transpose_ct(x_ct, cin, t, x_rows)
    var inscope xp : array<float>
    xp |> resize(t * cs)
    for (i in range64(t * cs)) {
        let j = i % cs
        xp[i] = j < cin ? x_rows[(i / cs) * cin + j] : 0.0
    }
    var inscope y_rows : array<float>
    conv1d_rows(cp, xp, t, y_rows)
    let t_out = conv1d_out_len(cp, t)
    var inscope got : array<float>
    transpose_tc_strided(y_rows, t_out, cout, cp.cout_s, got)
    check_envelope(tc, "padded conv q8", got, want, env, logits_maxabs(c0.b), Q8_REL)
    tc |> success(cp.q8, "padded conv q8: the padded width reached the q8 lane")
}

// the residual branch's arm: adain_rows_into writing a copy must land exactly where adain_rows
// lands in place
def private adain_into_case(tc : T?; c : int64) {
    var r = TtsRng()
    rng_seed(r, 0xa17ul + uint64(c))
    let t = T_ROWS
    var inscope a = TtsAdaIn()
    a.fc.nin = STYLE
    a.fc.nout = 2l * c
    rand_fill(r, a.fc.w, STYLE * 2l * c, 0.2)
    rand_fill(r, a.fc.b, 2l * c, 0.2)
    rand_fill(r, a.norm.w, c, 1.0)
    rand_fill(r, a.norm.b, c, 0.5)
    var inscope style : array<float>
    rand_fill(r, style, STYLE, 1.0)
    var inscope src : array<float>
    rand_fill(r, src, t * c, 3.0)
    var inscope inplace := src
    adain_rows(a, inplace, t, c, style, 1.0e-5)
    var inscope dst : array<float>
    adain_rows_into(a, src, dst, t, c, style, 1.0e-5)
    check_exact(tc, "tts blocks adain_rows_into c={c}", dst, inplace)
}

//! one `layernorm_rows_into` arm: `off` elements of the copy apart from `layernorm_rows` in place, and whether the source kept its bits
struct private LnIntoCase {
    off : int
    same_length : bool
    src_kept : bool
}

//! `layernorm_rows_into` of [T_ROWS][c] against `layernorm_rows` in place on a copy; `poison_at` >= 0 moves that source element first
def private layernorm_into_case(c : int64; poison_at : int64 = -1l) : LnIntoCase {
    var r = TtsRng()
    rng_seed(r, 0x1a3ul + uint64(c))
    let t = T_ROWS
    var inscope n = TtsNorm()
    rand_fill(r, n.w, c, 1.0)
    rand_fill(r, n.b, c, 0.5)
    var inscope src : array<float>
    rand_fill(r, src, t * c, 3.0)
    var inscope inplace := src
    layernorm_rows(inplace, t, c, n, 1.0e-5)
    if (poison_at >= 0l) {
        src[poison_at]++
    }
    var inscope src_before := src
    var inscope dst : array<float>
    layernorm_rows_into(src, dst, t, c, n, 1.0e-5)
    var res = LnIntoCase(same_length = length(dst) == length(inplace))
    if (res.same_length) {
        for (x, y in dst, inplace) {
            res.off += x == y ? 0 : 1
        }
    }
    res.src_kept = true
    for (x, y in src, src_before) {
        res.src_kept = res.src_kept && x == y
    }
    return res
}

// layernorm_rows against layernorm_channels, the same affine on both
def private layernorm_case(tc : T?) {
    var r = TtsRng()
    rng_seed(r, 0x1a1ul)
    let c = 64l
    let t = T_ROWS
    var inscope n = TtsNorm()
    rand_fill(r, n.w, c, 1.0)
    rand_fill(r, n.b, c, 0.5)
    var inscope x_ct : array<float>
    rand_fill(r, x_ct, c * t, 3.0)
    var inscope x_rows : array<float>
    transpose_ct(x_ct, c, t, x_rows)
    layernorm_channels(x_ct, c, t, n, 1.0e-5)
    layernorm_rows(x_rows, t, c, n, 1.0e-5)
    var inscope got : array<float>
    transpose_tc(x_rows, t, c, got)
    var inscope env : array<float>
    env |> resize(c * t)
    for (i in range64(c * t)) {
        env[i] = abs(x_ct[i]) + 4.0   // the normalized value's scale plus the affine's reach
    }
    check_envelope(tc, "layernorm_rows", got, x_ct, env, 0.0)
}

// ada_layernorm_rows against layernorm_channels plus the fc(style) affine applied per channel
def private ada_layernorm_case(tc : T?) {
    var r = TtsRng()
    rng_seed(r, 0x1a2ul)
    let c = 64l
    let t = T_ROWS
    var inscope a = TtsAdaLayerNorm()
    a.fc.nin = STYLE
    a.fc.nout = 2l * c
    rand_fill(r, a.fc.w, STYLE * 2l * c, 0.2)
    rand_fill(r, a.fc.b, 2l * c, 0.2)
    var inscope style : array<float>
    rand_fill(r, style, STYLE, 1.0)
    var inscope x_ct : array<float>
    rand_fill(r, x_ct, c * t, 3.0)
    var inscope x_rows : array<float>
    transpose_ct(x_ct, c, t, x_rows)
    var inscope h : array<float>
    linear_vec(a.fc, style, h)
    var inscope identity = TtsNorm()
    layernorm_channels(x_ct, c, t, identity, 1.0e-5)
    for (ci in range64(c)) {
        let g = 1.0 + h[ci]
        let bb = h[c + ci]
        let row = ci * t
        for (ti in range64(t)) {
            x_ct[row + ti] = x_ct[row + ti] * g + bb
        }
    }
    ada_layernorm_rows(a, x_rows, t, c, style, 1.0e-5)
    var inscope got : array<float>
    transpose_tc(x_rows, t, c, got)
    var inscope env : array<float>
    env |> resize(c * t)
    for (i in range64(c * t)) {
        env[i] = abs(x_ct[i]) + 4.0
    }
    check_envelope(tc, "ada_layernorm_rows", got, x_ct, env, 0.0)
}

// align_expand_rows against align_expand_channels: both write [C][F], so they agree exactly
def private align_expand_case(tc : T?) {
    var r = TtsRng()
    rng_seed(r, 0x1a3ul)
    let c = 64l
    let t = 300l
    var inscope x_ct : array<float>
    rand_fill(r, x_ct, c * t, 2.0)
    var inscope x_rows : array<float>
    transpose_ct(x_ct, c, t, x_rows)
    var inscope dur : array<int>
    dur |> resize(t)
    for (d in dur) {
        d = 1 + int(rng_uniform(r) * 4.0)
    }
    var inscope want, got : array<float>
    let frames = align_expand_channels(x_ct, c, t, dur, want)
    let got_frames = align_expand_rows(x_rows, t, c, dur, got)
    tc |> equal(got_frames, frames, "align_expand_rows: the rows form's frame count")
    return if (got_frames != frames)
    var inscope env : array<float>
    env |> resize(c * frames)
    for (v in env) {
        v = 0.0
    }
    check_envelope(tc, "align_expand_rows", got, want, env, 0.0)
}

// ===== the BERT encoder's embeddings and layer against a double-precision transcription =====

let BERT_T = 37l   //! off the 16-key pad and the 4-row tile
let BERT_HIDDEN = 64l
let BERT_HEADS = 4l
let BERT_FFN = 128l
let BERT_EPS = 1e-12
let BERT_REL = 1e-4   //! of the larger magnitude floored at 1: f32 GEMMs, softmax and norms against the same arithmetic in double

struct BertCaseLayer {
    q : TtsLinear = TtsLinear()
    k : TtsLinear = TtsLinear()
    v : TtsLinear = TtsLinear()
    dense : TtsLinear = TtsLinear()
    attn_ln : TtsNorm = TtsNorm()
    ffn : TtsLinear = TtsLinear()
    ffn_out : TtsLinear = TtsLinear()
    full_ln : TtsNorm = TtsNorm()
}

def private bert_linear(var r : TtsRng; nin, nout : int64) : TtsLinear {
    var l = TtsLinear(nin = nin, nout = nout)
    rand_fill(r, l.w, nout * nin, 1.0 / sqrt(float(nin)))
    rand_fill(r, l.b, nout, 0.2)
    linear_prepare(l)
    return <- l
}

def private bert_norm(var r : TtsRng; c : int64) : TtsNorm {
    var n = TtsNorm()
    rand_fill(r, n.w, c, 1.0)
    rand_fill(r, n.b, c, 0.3)
    for (w in n.w) {
        w++
    }
    return <- n
}

def private bert_case_layer(seed : uint64) : BertCaseLayer {
    var r = TtsRng()
    rng_seed(r, seed)
    return BertCaseLayer(q <- bert_linear(r, BERT_HIDDEN, BERT_HIDDEN), k <- bert_linear(r, BERT_HIDDEN, BERT_HIDDEN),
        v <- bert_linear(r, BERT_HIDDEN, BERT_HIDDEN), dense <- bert_linear(r, BERT_HIDDEN, BERT_HIDDEN), attn_ln <- bert_norm(r, BERT_HIDDEN),
        ffn <- bert_linear(r, BERT_HIDDEN, BERT_FFN), ffn_out <- bert_linear(r, BERT_FFN, BERT_HIDDEN), full_ln <- bert_norm(r, BERT_HIDDEN))
}

def private dlinear(l : TtsLinear; x : array<double>; t : int64) : array<double> {
    var y : array<double>
    y |> resize(t * l.nout)
    for (i in range64(t)) {
        for (o in range64(l.nout)) {
            var s = double(l.b[o])
            for (k in range64(l.nin)) {
                s += double(l.w[o * l.nin + k]) * x[i * l.nin + k]
            }
            y[i * l.nout + o] = s
        }
    }
    return <- y
}

//! `x` [t][c] under the LayerNorm `n`, in double
def private dlayernorm(x : array<double>; t, c : int64; n : TtsNorm; eps : double) : array<double> {
    var want : array<double>
    var inscope env : array<double>
    layernorm_ref(x, t, c, n.w, n.b, 0l, 0l, eps, want, env)
    return <- want
}

//! `bert_layer_rows` transcribed in double
def private dbert_layer(lw : BertCaseLayer; h : array<double>; t : int64; form : TtsGelu) : array<double> {
    var inscope q <- dlinear(lw.q, h, t)
    var inscope k <- dlinear(lw.k, h, t)
    var inscope v <- dlinear(lw.v, h, t)
    var inscope ctx <- attention_bidir_ref(q, k, v, t, BERT_HIDDEN, BERT_HEADS)
    var inscope attn_sum <- dlinear(lw.dense, ctx, t)
    for (a, x in attn_sum, h) {
        a += x
    }
    var inscope attn <- dlayernorm(attn_sum, t, BERT_HIDDEN, lw.attn_ln, double(BERT_EPS))
    var inscope f <- dlinear(lw.ffn, attn, t)
    for (x in f) {
        x = gelu_form_ref(x, form == TtsGelu.erf)
    }
    var inscope out <- dlinear(lw.ffn_out, f, t)
    for (o, a in out, attn) {
        o += a
    }
    return <- dlayernorm(out, t, BERT_HIDDEN, lw.full_ln, double(BERT_EPS))
}

def private bert_layer_case(t : T?; form : TtsGelu) {
    var inscope lw <- bert_case_layer(0xbe57ul)
    var r = TtsRng()
    rng_seed(r, 0xbe58ul)
    var inscope h : array<float>
    rand_fill(r, h, BERT_T * BERT_HIDDEN, 1.5)
    var inscope want <- [for (x in h); double(x)]
    var inscope ref <- dbert_layer(lw, want, BERT_T, form)
    let other = form == TtsGelu.erf ? TtsGelu.tanh : TtsGelu.erf
    var inscope other_ref <- dbert_layer(lw, want, BERT_T, other)
    var inscope sc = TtsBertScratch()
    bert_layer_rows(lw, BERT_HEADS, BERT_EPS, form, h, BERT_T, sc)
    check(t, "bert layer ({form})", h, ref, Bar(rel = BERT_REL), Shape(row_width = int(BERT_HIDDEN)))
    check_exceeds(t, "bert layer ({form}) against the {other} transcription", h, other_ref, Bar(rel = BERT_REL))
}

def private bert_embed_case(t : T?) {
    var r = TtsRng()
    rng_seed(r, 0xbe59ul)
    let vocab = 50l
    var inscope word, pos, typ : array<float>
    rand_fill(r, word, vocab * BERT_HIDDEN, 1.0)
    rand_fill(r, pos, BERT_T * BERT_HIDDEN, 0.5)
    rand_fill(r, typ, BERT_HIDDEN, 0.2)
    var inscope ln <- bert_norm(r, BERT_HIDDEN)
    var inscope ids <- [for (i in range64(BERT_T)); int((i * 7l + 3l) % vocab)]
    var inscope sum, untyped : array<double>
    sum |> resize(BERT_T * BERT_HIDDEN)
    untyped |> resize(BERT_T * BERT_HIDDEN)
    for (i in range64(BERT_T)) {
        for (d in range64(BERT_HIDDEN)) {
            untyped[i * BERT_HIDDEN + d] = double(word[int64(ids[i]) * BERT_HIDDEN + d]) + double(pos[i * BERT_HIDDEN + d])
            sum[i * BERT_HIDDEN + d] = untyped[i * BERT_HIDDEN + d] + double(typ[d])
        }
    }
    var inscope ref <- dlayernorm(sum, BERT_T, BERT_HIDDEN, ln, double(BERT_EPS))
    var inscope untyped_ref <- dlayernorm(untyped, BERT_T, BERT_HIDDEN, ln, double(BERT_EPS))
    var inscope got : array<float>
    bert_embed_rows(ids, word, pos, typ, ln, BERT_EPS, BERT_HIDDEN, got)
    check(t, "bert embeddings", got, ref, Bar(rel = BERT_REL), Shape(row_width = int(BERT_HIDDEN)))
    check_exceeds(t, "bert embeddings against the sum with the type row dropped", got, untyped_ref, Bar(rel = BERT_REL))
}

def private gelu_rows_case(t : T?; form : TtsGelu) {
    var r = TtsRng()
    rng_seed(r, 0xbe5aul)
    let rows = 37l
    let c = BERT_FFN
    var inscope x : array<float>
    rand_fill(r, x, rows * c, 4.0)
    var inscope whole := x
    var inscope split := x
    if (form == TtsGelu.erf) {
        gelu_erf(whole, rows * c)
    } else {
        gelu(whole, rows * c)
    }
    gelu_rows(split, rows, c, form)
    let off = mismatch_exact(split, whole)
    to_log(LOG_INFO, "gelu_rows ({form}): {off} of {length(whole)} elements apart from the whole-array kernel, the first at {first_mismatch(split, whole)}\n")
    t |> equal(off, 0, "gelu_rows ({form}): elements apart from the whole-array kernel")
    t |> success(all_finite(x) && all_finite(split), "gelu_rows ({form}): the input and the output are finite")
    let moved = mismatch_exact(split, x)
    t |> success(moved > 0, "gelu_rows ({form}): the pass moved its input ({moved} elements)")
}

[test(description = "bert_embed_rows and bert_layer_rows on both GELU forms hold a double-precision transcription of their arithmetic")]
def test_bert_blocks(t : T?) {
    with_dasllama_jobque_() {
        t |> run("gelu_rows on both forms: bit for bit the whole-array kernel over the same elements") @(t : T?) {
            gelu_rows_case(t, TtsGelu.erf)
            gelu_rows_case(t, TtsGelu.tanh)
        }
        t |> run("the BERT embeddings: word + position + type rows, then the LayerNorm") @(t : T?) {
            bert_embed_case(t)
        }
        t |> run("one post-LN BERT layer under the erf GELU and under the tanh GELU") @(t : T?) {
            bert_layer_case(t, TtsGelu.erf)
            bert_layer_case(t, TtsGelu.tanh)
        }
    }
}

// ===== split invariance: one dispatch lane against as many as the box gives =====

struct SplitCase {
    name : string
    diffs : int
}

// The two axes a rows kernel's row split moves on. The batch lane cap clamps inside
// lanes_for_work, so it reaches only the shapers that pass it - matmul_chunks. The jobque worker
// limit parks the over-limit workers, so it moves get_dispatch_lanes() itself and reaches every
// shaper: the AdaIN column stats, attention's head split, and each bare lanes_for_work(work, 0)
// site the cap alone leaves on one setting.
enum SplitAxis {
    lane_cap
    worker_limit
}

// the pin the row-split legs share: the batch lane cap decides how many chunks a rows GEMM's
// row range breaks into, and it is restored on every path out
def private at_lane_cap(cap : int; blk : block<() : void>) {
    let prev = get_batch_lane_cap()
    defer() {
        set_batch_lane_cap(prev)
    }
    set_batch_lane_cap(cap)
    invoke(blk)
}

// the second axis, the same shape. The limit lives on the que rather than in a module global,
// so the restore reads it back instead of assuming the unlimited default setup_dasllama_jobque_
// left; -1 comes back as the que's own saturated value, which restores identically.
def private at_worker_limit(limit : int; blk : block<() : void>) {
    let prev = get_jobque_worker_limit()
    defer() {
        set_jobque_worker_limit(prev)
    }
    set_jobque_worker_limit(limit)
    invoke(blk)
}

// one leg of an axis: `narrow` pins it to the fewest lanes it admits, else to the box's
def private at_split(axis : SplitAxis; narrow : bool; blk : block<() : void>) {
    if (axis == SplitAxis.worker_limit) {
        at_worker_limit(narrow ? 1 : get_total_hw_jobs(), blk)
    } else {
        at_lane_cap(narrow ? 1 : 0, blk)
    }
}

def private axis_name(axis : SplitAxis) : string =>
    axis == SplitAxis.worker_limit ? "worker limit" : "lane cap"

// the lanes a rows GEMM's shaper answers under a pinned leg - the anti-vacuous witness: the cap
// clamps inside lanes_for_work, the worker limit moves the get_dispatch_lanes() it clamps
// against, and a leg pair shaped to the same count compares nothing.
def private shaped_lanes(axis : SplitAxis; narrow : bool) : int {
    var lanes = 0
    at_split(axis, narrow) {
        lanes = lanes_for_work(1l << 40l, get_batch_lane_cap())
    }
    return lanes
}

def private split_diff(name : string; a, b : array<float>) : SplitCase {
    if (length(a) != length(b)) {
        return SplitCase(name = name, diffs = 1000000)
    }
    var res = SplitCase(name = name)
    for (x, y in a, b) {
        if (x != y) {
            res.diffs++
        }
    }
    return res
}

//! the weight lane a split case mints its conv in
enum private ConvLane {
    f32
    q8
    kq
    halves
}

//! a raw conv prepared for the rows form over half floats, its weights on the f16 lattice first: the narrow is then exact
def private prepared_halves(c : TtsConv1d) : TtsConv1d {
    var p := c
    for (v in p.w) {
        v = f16_to_f32(f32_to_f16(v))
    }
    p.served_rows = true
    conv1d_prepare(p, false, false, false, true)
    return <- p
}

//! a rows conv minted on `lane` (`fmt` the K-quant member under `ConvLane.kq`), at one lane and at many: the outputs' differing
//! elements, a million more where the lane never minted - the leg would have pinned another kernel
def private split_conv_case(name : string; lane : ConvLane; cin, cout, k, stride, pad, dil : int64; axis : SplitAxis; transposed : bool = false;
                            fmt : KqFmt = KqFmt.q8) : SplitCase {
    var r = TtsRng()
    rng_seed(r, 0x5b17ul + uint64(k * 100l + dil) + (uint64(int(lane)) << 16ul))
    var inscope c0 <- make_conv(r, cin, cout, k, stride, pad, dil, transposed, true)
    var inscope cr <- lane == ConvLane.kq ? prepared_kq(c0, fmt, true) : lane == ConvLane.halves ? prepared_halves(c0) : prepared(c0, true, lane == ConvLane.q8)
    let t_in = T_ROWS
    var inscope x_ct : array<float>
    rand_fill(r, x_ct, cin * t_in, 2.0)
    var inscope x_rows : array<float>
    transpose_ct(x_ct, cin, t_in, x_rows)
    var inscope one, many : array<float>
    at_split(axis, true) {
        conv1d_rows(cr, x_rows, t_in, one)
    }
    at_split(axis, false) {
        conv1d_rows(cr, x_rows, t_in, many)
    }
    var res = split_diff("{name} @ {axis_name(axis)}", one, many)
    let minted = (lane == ConvLane.q8 ? cr.q8 : lane == ConvLane.kq ? cr.kq_fmt != 0 : lane == ConvLane.halves ? conv1d_host_halves(cr) : true)
    res.diffs += minted ? 0 : 1000000
    return res
}

//! the dense layer at one lane and at many, the outputs' differing elements: `linear_rows` over the f32 rows, or under `halves`
//! `linear_vec` over a layer prepared over half floats
def private split_linear_case(nin, nout : int64; axis : SplitAxis; halves : bool = false) : SplitCase {
    var r = TtsRng()
    rng_seed(r, 0x5b1aul + uint64(nin) + (halves ? 0x10000ul : 0ul))
    let t = halves ? 1l : T_ROWS
    var inscope l = TtsLinear(nin = nin, nout = nout)
    rand_fill(r, l.w, nout * nin, 1.0 / sqrt(float(nin)))
    rand_fill(r, l.b, nout, 0.5)
    if (halves) {
        linear_prepare(l, true, false, false, true, true)
    } else {
        linear_prepare(l)
    }
    var inscope x : array<float>
    rand_fill(r, x, t * nin, 2.0)
    var inscope one, many : array<float>
    at_split(axis, true) {
        if (halves) {
            linear_vec(l, x, one)
        } else {
            linear_rows(l, x, t, one)
        }
    }
    at_split(axis, false) {
        if (halves) {
            linear_vec(l, x, many)
        } else {
            linear_rows(l, x, t, many)
        }
    }
    var res = split_diff("{halves ? "linear_vec halves" : "linear_rows"} {nin}x{nout} @ {axis_name(axis)}", one, many)
    res.diffs += halves && !linear_host_halves(l) ? 1000000 : 0
    return res
}

def private split_adain_case(c : int64; axis : SplitAxis) : SplitCase {
    var r = TtsRng()
    rng_seed(r, 0x5b18ul + uint64(c))
    let t = T_ROWS
    var inscope a = TtsAdaIn()
    a.fc.nin = STYLE
    a.fc.nout = 2l * c
    rand_fill(r, a.fc.w, STYLE * 2l * c, 0.2)
    rand_fill(r, a.fc.b, 2l * c, 0.2)
    rand_fill(r, a.norm.w, c, 1.0)
    rand_fill(r, a.norm.b, c, 0.5)
    var inscope style : array<float>
    rand_fill(r, style, STYLE, 1.0)
    var inscope src : array<float>
    rand_fill(r, src, t * c, 3.0)
    var inscope one := src
    var inscope many := src
    at_split(axis, true) {
        adain_rows(a, one, t, c, style, 1.0e-5)
    }
    at_split(axis, false) {
        adain_rows(a, many, t, c, style, 1.0e-5)
    }
    return split_diff("adain_rows c={c} @ {axis_name(axis)}", one, many)
}

def private split_attention_case(axis : SplitAxis) : SplitCase {
    var r = TtsRng()
    rng_seed(r, 0x5b19ul)
    let t = 401l
    let heads = 4l
    let hidden = heads * 16l
    var inscope q, k, v : array<float>
    rand_fill(r, q, t * hidden, 1.0)
    rand_fill(r, k, t * hidden, 1.0)
    rand_fill(r, v, t * hidden, 2.0)
    var inscope one, many : array<float>
    at_split(axis, true) {
        attention_rows(q, k, v, t, hidden, heads, one)
    }
    at_split(axis, false) {
        attention_rows(q, k, v, t, hidden, heads, many)
    }
    return split_diff("attention_rows @ {axis_name(axis)}", one, many)
}

def private split_bert_layer_case(axis : SplitAxis) : SplitCase {
    var inscope lw <- bert_case_layer(0x5b1bul)
    var r = TtsRng()
    rng_seed(r, 0x5b1cul)
    let t = 401l
    var inscope h : array<float>
    rand_fill(r, h, t * BERT_HIDDEN, 1.5)
    var inscope one := h
    var inscope many := h
    var inscope sc = TtsBertScratch()
    at_split(axis, true) {
        bert_layer_rows(lw, BERT_HEADS, BERT_EPS, TtsGelu.erf, one, t, sc)
    }
    at_split(axis, false) {
        bert_layer_rows(lw, BERT_HEADS, BERT_EPS, TtsGelu.erf, many, t, sc)
    }
    return split_diff("bert_layer_rows @ {axis_name(axis)}", one, many)
}

def private split_gelu_rows_case(axis : SplitAxis) : SplitCase {
    var r = TtsRng()
    rng_seed(r, 0x5b1dul)
    let t = T_ROWS
    var inscope x : array<float>
    rand_fill(r, x, t * BERT_FFN, 4.0)
    var inscope one := x
    var inscope many := x
    at_split(axis, true) {
        gelu_rows(one, t, BERT_FFN, TtsGelu.erf)
    }
    at_split(axis, false) {
        gelu_rows(many, t, BERT_FFN, TtsGelu.erf)
    }
    return split_diff("gelu_rows @ {axis_name(axis)}", one, many)
}

def private report_split(t : T?; r : SplitCase) {
    t |> equal(r.diffs, 0, "{r.name}: elements differing between one dispatch lane and many")
}

// one direction's random weights at `nin` inputs and `hd` hidden lanes
def private make_lstm(var r : TtsRng; nin, hd : int64) : TtsLstm {
    var l = TtsLstm(nin = nin, hidden = hd)
    rand_fill(r, l.w_ih, 4l * hd * nin, 1.0 / sqrt(float(nin)))
    rand_fill(r, l.w_hh, 4l * hd * hd, 1.0 / sqrt(float(hd)))
    rand_fill(r, l.b, 4l * hd, 0.25)
    return <- l
}

[test(description = "A BiLSTM minted Q8_0 tracks its own f32 run over 96 steps within 0.05 of a state lane's unit range, in both directions, and is not the f32 run; a direction whose input width is off 32 keeps its f32 matrices and its run bit for bit")]
def test_lstm_q8(t : T?) {
    with_dasllama_jobque_() {
        t |> run("the q8 recurrence against the f32 one, and the width that refuses the mint") @(t : T?) {
            for (nin in [160l, 66l]) {
                let hd = 64l
                let steps = 96l
                var r = TtsRng()
                rng_seed(r, 0x157aul + uint64(nin))
                var inscope l = TtsBiLstm()
                l.fwd <- make_lstm(r, nin, hd)
                l.bwd <- make_lstm(r, nin, hd)
                var inscope x : array<float>
                rand_fill(r, x, steps * nin, 1.0)
                var inscope want : array<float>
                bilstm(l, x, steps, want)
                var inscope lq := l
                let repack = select_matmul_backend_for_load_()
                let minted = lstm_prepare_q8(lq.fwd, repack) && lstm_prepare_q8(lq.bwd, repack)
                t |> equal(minted, nin % 32l == 0l, "nin {nin}: the lattice decides the mint")
                t |> equal(empty(lq.fwd.w_hh), minted, "nin {nin}: a minted direction drops its f32 matrices, a refused one keeps them")
                var inscope got : array<float>
                bilstm(lq, x, steps, got)
                let off = logits_maxdiff(got, want)
                let peak = logits_maxabs(want)
                var inscope x_other : array<float>
                rand_fill(r, x_other, steps * nin, 1.0)
                var inscope other : array<float>
                bilstm(l, x_other, steps, other)
                let control = logits_maxdiff(other, want)
                to_log(LOG_INFO, "tts blocks lstm q8 nin {nin}, {steps} steps of seeded rows: off {off} from the f32 run, the states' peak {peak}; another input's f32 run {control} from it\n")
                if (minted) {
                    t |> success(off > 0.0 && off < 0.05, "nin {nin}: the q8 run is within 0.05 of the f32 run and is not it - off {off}, the states' peak {peak}")
                    t |> success(all_finite(other) && control > 0.05, "nin {nin}: another input's run lands outside that bound (the compare's control): {control}")
                    t |> success(peak > 0.25, "nin {nin}: the fixture's states leave the origin (peak {peak})")
                } else {
                    t |> equal(off, 0.0, "nin {nin}: the refused mint runs the f32 recurrence")
                }
            }
        }
    }
}

[test(description = "A conv minted in the K-quant rows form (Q4_K, Q5_K, Q6_K; forward at stride 1 and 2, transposed at stride 2, 6 and 10, and channel-major; a stacked row padded to 256 and one on it; disk-order and repacked planes) matches the channel-major f32 conv over the weights its own planes dequantize to within the q8 dot-envelope bar, and sits at least twice as far from the f32 conv over the weights before quantization; a vector layer minted Q8_0 matches its f32 GEMV at that bar, and one with a width off 32 keeps its f32 weight")]
def test_quant_mints(t : T?) {
    with_dasllama_jobque_() {
        t |> run("a vector layer's Q8_0 form against its f32 GEMV, and a width off 32 keeping w") @(t : T?) {
            vec_case_q8(t, 128l, 1024l, true)    // a style projection: 128 style lanes onto two halves of 512
            vec_case_q8(t, 256l, 1088l, true)    // the padded decoder width, 544 a half
            vec_case_q8(t, 128l, 132l, false)    // 66 a half, off the 32: the layer keeps w
        }
        t |> run("the K-quant conv lane against the f32 conv over its own dequantized planes") @(t : T?) {
            for (repack in [false, true]) {
                let arm = repack ? "repacked" : "disk order"
                conv_case_kq(t, "conv k4 k3 d1 pad to 512 ({arm})", KqFmt.k4, 128l, 128l, 3l, 1l, 1l, 1l, repack)
                conv_case_kq(t, "conv k4 k7 d3 pad to 1024 ({arm})", KqFmt.k4, 128l, 96l, 7l, 1l, 9l, 3l, repack)
                conv_case_kq(t, "conv k6 k1 on 256 ({arm})", KqFmt.k6, 256l, 64l, 1l, 1l, 0l, 1l, repack)
                conv_case_kq(t, "conv k5 k4 stride 2 ({arm})", KqFmt.k5, 66l, 22l, 4l, 2l, 1l, 1l, repack)
                conv_case_kq(t, "convT k4 k4 stride 2 pad to 256 ({arm})", KqFmt.k4, 128l, 64l, 4l, 2l, 1l, 1l, repack, true)
                conv_case_kq(t, "convT k6 k12 stride 6 on 256 ({arm})", KqFmt.k6, 256l, 32l, 12l, 6l, 3l, 1l, repack, true)
                conv_case_kq(t, "conv k4 k5 channel-major ({arm})", KqFmt.k4, 128l, 128l, 5l, 1l, 2l, 1l, repack, false, true)
                conv_case_kq(t, "convT k4 k20 stride 10, the kokoro upsampler's shape ({arm})", KqFmt.k4, 512l, 256l, 20l, 10l, 5l, 1l, repack, true)
            }
        }
        t |> run("a padded row stacked after a full one of the same width reads zeros in its pad columns") @(t : T?) {
            // k4 over 128 inputs fills all 512 stacked columns, with rows ten thousand times the next conv's; k4 over 66 after
            // it stacks 264 and pads to the same 512, in the same scratch. A pad column left holding the full conv's value
            // sets the scale of the Q8_K block it shares with real columns, and the real rows quantize to nothing
            conv_case_kq(t, "conv k4 k4 on 512, filling the stack", KqFmt.k4, 128l, 128l, 4l, 1l, 2l, 1l, true, false, false, 2.0e4)
            conv_case_kq(t, "conv k4 k4 over 66 inputs pad to 512, after it", KqFmt.k4, 66l, 128l, 4l, 1l, 2l, 1l, true)
            conv_case_kq(t, "convT k4 k4 on 256, filling the lift's stack", KqFmt.k4, 256l, 64l, 4l, 2l, 1l, 1l, true, true, false, 2.0e4)
            conv_case_kq(t, "convT k4 k4 over 66 inputs pad to 256, after it", KqFmt.k4, 66l, 64l, 4l, 2l, 1l, 1l, true, true)
        }
    }
}

[test(description = "Every rows-layout TTS kernel (forward, strided and transposed convs, linear, attention, snake, AdaIN, norms, pool, padding) matches its channel-major twin or reference within a dot-envelope bar or exactly, q8 and K-quant lanes at their own bars, each with a must-red control")]
def test_rows_convs(t : T?) {
    with_dasllama_jobque_() {
        t |> run("forward convs at the generator's taps") @(t : T?) {
            conv_case(t, "conv k3 d1", 64l, 64l, 3l, 1l, 1l, 1l, false)
            conv_case(t, "conv k7 d3", 64l, 64l, 7l, 1l, 9l, 3l, false)
            conv_case(t, "conv k11 d5", 64l, 64l, 11l, 1l, 25l, 5l, false)
            conv_case(t, "conv k7 wide-in", 128l, 64l, 7l, 1l, 3l, 1l, false)
        }
        t |> run("the padded served width (conv_post's 22 columns)") @(t : T?) {
            conv_case(t, "conv_post k7 cout22", 64l, 22l, 7l, 1l, 3l, 1l, false)
        }
        t |> run("the strided noise convs over stacked tap rows") @(t : T?) {
            conv_case(t, "noise conv k20 s10", 22l, 128l, 20l, 10l, 5l, 1l, false)
            conv_case(t, "noise conv k12 s6", 22l, 64l, 12l, 6l, 3l, 1l, false)
            // k*cin*4 bytes x t_out past the 4 MB stack budget: the chunk loop runs more than once
            conv_case(t, "noise conv k20 s10 wide-in", 256l, 64l, 20l, 10l, 5l, 1l, false, 4l * T_ROWS)
        }
        t |> run("transposed upsamplers") @(t : T?) {
            conv_case(t, "convT k20 s10", 64l, 32l, 20l, 10l, 5l, 1l, true)
            conv_case(t, "convT k12 s6", 64l, 32l, 12l, 6l, 3l, 1l, true)
        }
        t |> run("linear_rows on the tile") @(t : T?) {
            linear_case(t, 96l, 64l)
            linear_case(t, 768l, 3072l)
        }
        t |> run("attention_rows against the scalar heads") @(t : T?) {
            attention_case(t)
        }
        t |> run("the q8 lane against the f32 reference") @(t : T?) {
            conv_case_q8(t, "conv q8 k3 d1", 128l, 128l, 3l, 1l, 1l)
            conv_case_q8(t, "conv q8 k7 d3", 128l, 128l, 7l, 9l, 3l)
            conv_case_q8(t, "conv q8 k11 d5", 256l, 256l, 11l, 25l, 5l)
            conv_case_q8(t, "convT q8 k4 stride 2", 128l, 64l, 4l, 1l, 1l, true, 2l)
            linear_case_q8(t, 768l, 768l)
        }
        t |> run("the K-quant lane against the leaf per row - both arms, and a width off the float4 bias") @(t : T?) {
            linear_case_kq(t, 512l, 96l, false, true)
            linear_case_kq(t, 768l, 64l, true, true)
            linear_case_kq(t, 512l, 96l, false, false)
            linear_case_kq(t, 768l, 64l, true, false)
            linear_case_kq(t, 256l, 6l, true, true)   // nout off 4: the bias tail past the last float4
            kq_width_refusal(t)
            slab_linear_cases_kq(t)
        }
        t |> run("snake, adain, pad, polar") @(t : T?) {
            snake_case(t)
            adain_case(t, 64l)
            adain_case(t, 66l)   // the decoder's concat widths sit off the lane
            adain_into_case(t, 64l)
            adain_into_case(t, 66l)
            pad_and_polar_case(t)
        }
        t |> run("the norms and the duration expansion against their channel-major twins") @(t : T?) {
            layernorm_case(t)
            ada_layernorm_case(t)
            align_expand_case(t)
        }
        t |> run("layernorm_rows_into at 64 and 66 channels: bit for bit layernorm_rows in place on a copy, the source untouched") @(t : T?) {
            for (c in fixed_array(64l, 66l)) {
                let lc = layernorm_into_case(c)
                t |> success(lc.same_length, "c={c}: the copy is sized [{T_ROWS}][{c}]")
                t |> equal(lc.off, 0, "c={c}: elements of the copy apart from layernorm_rows in place")
                t |> success(lc.src_kept, "c={c}: the source keeps its bits")
            }
            let poisoned = layernorm_into_case(64l, 5l * 64l + 7l)
            t |> success(poisoned.off >= 64, "the control: one moved source element moves its row of the copy off the in-place rows ({poisoned.off} elements)")
        }
        t |> run("the residual block's pool and nearest shortcut") @(t : T?) {
            pool_and_nearest_case(t)
        }
        t |> run("an input width padded to the q8 lane") @(t : T?) {
            pad_case(t)
            pad_case_q8(t)
        }
        t |> run("the dot-envelope bar's must-red control") @(t : T?) {
            conv_case(t, "conv k3 d1 tap0 zeroed", 64l, 64l, 3l, 1l, 1l, 1l, false, T_ROWS, 0l)
            conv_case(t, "conv k7 d3 tap3 zeroed", 64l, 64l, 7l, 1l, 9l, 3l, false, T_ROWS, 3l)
        }
    }
}

// every rows kernel over one axis: the two legs, plus the witness that the axis moved the split
def private split_arms(t : T?; axis : SplitAxis) {
    let ax = axis_name(axis)
    let narrow = shaped_lanes(axis, true)
    let wide = shaped_lanes(axis, false)
    to_log(LOG_INFO, "tts blocks split axis {ax}: a rows GEMM shapes to {narrow} lane(s) against {wide}\n")
    t |> run("the {ax} axis moves the row split") @(t : T?) {
        if (get_total_hw_jobs() < 2) {
            t |> skip("a box with fewer than two hardware jobs shapes both legs the same")
            return
        }
        t |> success(narrow < wide, "{ax}: {narrow} lane(s) against {wide} - equal counts compare nothing")
    }
    t |> run("the rows convs land the same bits at one lane and at many ({ax})") @(t : T?) {
        report_split(t, split_conv_case("conv k3 pad1 d1", ConvLane.f32, 64l, 64l, 3l, 1l, 1l, 1l, axis))
        report_split(t, split_conv_case("conv k7 pad9 d3", ConvLane.f32, 64l, 64l, 7l, 1l, 9l, 3l, axis))
        report_split(t, split_conv_case("conv k4 pad4", ConvLane.f32, 64l, 64l, 4l, 1l, 4l, 1l, axis))
        report_split(t, split_conv_case("conv q8 k3 pad1 d1", ConvLane.q8, 128l, 128l, 3l, 1l, 1l, 1l, axis))
        report_split(t, split_conv_case("conv k4 k3 pad1", ConvLane.kq, 128l, 128l, 3l, 1l, 1l, 1l, axis, false, KqFmt.k4))
        report_split(t, split_conv_case("convT q8 k4 stride 2", ConvLane.q8, 128l, 64l, 4l, 2l, 1l, 1l, axis, true))
        report_split(t, split_conv_case("convT k6 k12 stride 6", ConvLane.kq, 256l, 32l, 12l, 6l, 3l, 1l, axis, true, KqFmt.k6))
        report_split(t, split_conv_case("convT k4 k4 stride 2", ConvLane.kq, 128l, 64l, 4l, 2l, 1l, 1l, axis, true, KqFmt.k4))
        report_split(t, split_conv_case("conv halves k3 pad1", ConvLane.halves, 128l, 128l, 3l, 1l, 1l, 1l, axis))
        report_split(t, split_conv_case("convT halves k4 stride 2", ConvLane.halves, 128l, 64l, 4l, 2l, 1l, 1l, axis, true))
    }
    t |> run("adain, attention and the dense layer land the same bits ({ax})") @(t : T?) {
        report_split(t, split_adain_case(64l, axis))
        report_split(t, split_adain_case(66l, axis))
        report_split(t, split_attention_case(axis))
        report_split(t, split_linear_case(768l, 3072l, axis))
        report_split(t, split_linear_case(768l, 3072l, axis, true))
    }
    t |> run("the BERT layer lands the same bits ({ax})") @(t : T?) {
        report_split(t, split_bert_layer_case(axis))
        report_split(t, split_gelu_rows_case(axis))
    }
}

//! a dense conv over rows in double, from its ONNX-layout weights by the conv's definition: y [t_out][cout], the forward
//! conv's weights [cout][cin][k], a transposed one's [cin][cout][k]
def private conv_rows_double(c : TtsConv1d; x_rows : array<float>; t_in : int64) : array<float> {
    let t_out = conv1d_out_len(c, t_in)
    var acc : array<double>
    acc |> resize(t_out * c.cout)
    for (t in range64(t_out)) {
        for (co in range64(c.cout)) {
            acc[t * c.cout + co] = empty(c.b) ? 0.0lf : double(c.b[co])
        }
    }
    for (ti in range64(c.transposed ? t_in : t_out)) {
        for (kk in range64(c.k)) {
            let p = c.transposed ? ti * c.stride + kk * c.dilation - c.pad_l : ti * c.stride - c.pad_l + kk * c.dilation
            continue if (p < 0l || p >= (c.transposed ? t_out : t_in))
            for (co in range64(c.cout)) {
                for (ci in range64(c.cin)) {
                    if (c.transposed) {
                        acc[p * c.cout + co] += double(c.w[ci * (c.cout * c.k) + co * c.k + kk]) * double(x_rows[ti * c.cin + ci])
                    } else {
                        acc[ti * c.cout + co] += double(c.w[co * (c.cin * c.k) + ci * c.k + kk]) * double(x_rows[p * c.cin + ci])
                    }
                }
            }
        }
    }
    var y <- [for (v in acc); float(v)]
    delete acc
    return <- y
}

//! a rows conv served from half floats against the same conv over those halves widened: the outputs differing in bits, and
//! the weight rows differing; `poison` zeroes one half of the plane first (the control)
def private halves_conv_off(cin, cout, k, stride, pad, dil : int64; transposed, poison : bool) : tuple<out : int; rows : int; held : bool; first : int; finite : bool; ref : float> {
    var r = TtsRng()
    rng_seed(r, 0xa11ul + uint64(k * 10l + stride))
    var inscope c0 <- make_conv(r, cin, cout, k, stride, pad, dil, transposed, true)
    for (v in c0.w) {
        v = f16_to_f32(f32_to_f16(v))   // a file's F16 tensor widened: the narrow below is then exact
    }
    var inscope cf <- prepared(c0, true)
    var inscope ch <- prepared_halves(c0)
    let held = conv1d_host_halves(ch) && empty(ch.wk) && empty(ch.wtr)
    if (poison) {
        if (transposed) {
            let at = long_length(ch.wtrh) / 2l + 1l
            ch.wtrh[at] = uint8(uint(ch.wtrh[at]) ^ 0x10u)
        } else {
            ch.wkh[1] = uint8(uint(ch.wkh[1]) ^ 0x10u)
        }
    }
    var inscope x : array<float>
    rand_fill(r, x, cin * T_ROWS, 2.0)
    var inscope yf, yh : array<float>
    conv1d_rows(cf, x, T_ROWS, yf)
    conv1d_rows(ch, x, T_ROWS, yh)
    var inscope want <- conv_rows_double(c0, x, T_ROWS)
    var inscope live <- [for (e in range64(long_length(want))); yh[(e / cout) * ch.cout_s + e % cout]]   // the rows' first cout columns: the served width pads past them
    var off = (out = 0, rows = 0, held = held, first = -1, finite = all_finite(yf) && all_finite(yh), ref = logits_maxdiff(live, want) / logits_maxabs(want))
    for (a, b, i in yf, yh, count()) {
        continue if (a == b)
        off.out++
        if (off.first < 0) {
            off.first = i
        }
    }
    var inscope rf, rh : array<float>
    rf |> resize(k * cin)
    rh |> resize(k * cin)
    for (co in range64(cout)) {
        conv1d_weight_row(cf, co, rf, 0l)
        conv1d_weight_row(ch, co, rh, 0l)
        for (a, b in rf, rh) {
            off.rows += a != b ? 1 : 0
        }
    }
    return off
}

[test(description = "The tower slab's writer marks a slot widened where it reads a leaf held as quants or halves back into the slab as float rows - a conv of halves or of Q4_K quants off the 8 lattice, a dense layer of halves off the 32 lattice, a repacked K-quant conv or layer, any such leaf with no member staged - and leaves a leaf staged as stored and a leaf of floats unmarked")]
def test_slab_widened_marks(t : T?) {
    with_dasllama_jobque_() {
        t |> run("the slab writer marks the leaves it widens, and no other") @(t : T?) {
            slab_widened_marks(t)
        }
    }
}

[test(description = "A rows conv prepared over half floats - forward at stride 1 and 2, transposed - holds the half plane alone, and its outputs and weight rows are, bit for bit, the f32 rows conv's over the same halves widened; one flipped half moves both")]
def test_rows_convs_over_halves(t : T?) {
    with_dasllama_jobque_() {
        for (cfg in fixed_array((cin = 24l, cout = 20l, k = 5l, stride = 1l, pad = 2l, dil = 1l, tr = false),
                                (cin = 16l, cout = 32l, k = 7l, stride = 2l, pad = 3l, dil = 1l, tr = false),
                                (cin = 32l, cout = 16l, k = 4l, stride = 2l, pad = 1l, dil = 1l, tr = true))) {
            let name = "{cfg.tr ? "transposed" : "forward"} k{cfg.k} stride {cfg.stride}"
            let off = halves_conv_off(cfg.cin, cfg.cout, cfg.k, cfg.stride, cfg.pad, cfg.dil, cfg.tr, false)
            t |> success(off.held, "{name}: the conv holds its half plane and no f32 rows plane")
            to_log(LOG_INFO, "{name}: {off.out} outputs off the f32 conv's, the first at {off.first}; {off.rows} weight-row elements off\n")
            t |> success(off.ref < 1.0e-5, "{name}: outputs within 1e-5 of the peak of the conv in double over the widened halves ({off.ref})")
            t |> equal(off.out, 0, "{name}: outputs off the f32 conv over the widened halves")
            t |> equal(off.rows, 0, "{name}: weight-row elements off the f32 conv's")
            let ctl = halves_conv_off(cfg.cin, cfg.cout, cfg.k, cfg.stride, cfg.pad, cfg.dil, cfg.tr, true)
            t |> success(ctl.finite && ctl.out > 0 && ctl.rows > 0 && ctl.ref > 1.0e-5, "{name} control: one flipped half moves the outputs ({ctl.out}, the first at {ctl.first}; finite; {ctl.ref} of the peak off the double conv) and a weight row ({ctl.rows})")
        }
    }
}

//! `linear_vec` over a layer's half rows against the same layer over those halves widened: the worst output difference over
//! the float outputs' peak, the weight-row elements differing, and whether the layer holds its halves alone; `poison` flips
//! a bit of the last weight first (the control)
def private halves_linear_off(nin, nout : int64; poison : bool) : tuple<out : float; rows : int; held : bool; at : int; finite : bool; ref : float> {
    var r = TtsRng()
    rng_seed(r, 0x11aul + uint64(nin))
    var inscope fl = TtsLinear(nin = nin, nout = nout)
    rand_fill(r, fl.w, nin * nout, 0.2)
    for (v in fl.w) {
        v = f16_to_f32(f32_to_f16(v))   // a file's F16 tensor widened: the narrow below is then exact
    }
    rand_fill(r, fl.b, nout, 0.1)
    var inscope hl = TtsLinear(nin = nin, nout = nout)
    hl.w := fl.w
    hl.b := fl.b
    linear_prepare(hl, true, false, false, true, true)
    let held = linear_host_halves(hl) && empty(hl.w) && long_length(hl.wh) == 2l * nin * nout
    if (poison) {
        let at = long_length(hl.wh) - 1l   // the last weight's sign and exponent byte
        hl.wh[at] = uint8(uint(hl.wh[at]) ^ 0x10u)
    }
    var inscope x : array<float>
    rand_fill(r, x, nin, 2.0)
    var inscope yf, yh : array<float>
    linear_vec(fl, x, yf)
    linear_vec(hl, x, yh)
    let worst = span_maxdiff(yh, yf, 0, 0, length(yf))
    var inscope xd <- [for (v in x); double(v)]
    var inscope wd <- dlinear(fl, xd, 1l)
    var inscope want <- [for (v in wd); float(v)]
    var off = (out = worst.diff / logits_maxabs(yf), rows = 0, held = held, at = worst.at, finite = all_finite(yf) && all_finite(yh),
        ref = logits_maxdiff(yh, want) / logits_maxabs(want))
    var inscope rf, rh : array<float>
    rf |> resize(nin)
    rh |> resize(nin)
    for (o in range64(nout)) {
        linear_weight_row(fl, o, rf, 0l)
        linear_weight_row(hl, o, rh, 0l)
        for (a, b in rf, rh) {
            off.rows += a != b ? 1 : 0
        }
    }
    return off
}

[test(description = "A vector layer prepared over half floats holds the half plane alone, `linear_vec` over it is the float layer's output over the widened halves to 1e-5 of the peak, its weight rows read back bit for bit, and one flipped bit of a half moves both")]
def test_linear_vec_over_halves(t : T?) {
    with_dasllama_jobque_() {
        for (shape in fixed_array((nin = 48l, nout = 20l), (nin = 512l, nout = 96l))) {
            let name = "{shape.nout}x{shape.nin}"
            let off = halves_linear_off(shape.nin, shape.nout, false)
            t |> success(off.held, "{name}: the layer holds two bytes a weight and no float rows")
            to_log(LOG_INFO, "{name}: the largest output difference {off.out} of the peak, at output {off.at}\n")
            t |> success(off.ref < 1.0e-5, "{name}: outputs within 1e-5 of the peak of the dot in double over the widened halves ({off.ref})")
            t |> success(off.out < 1.0e-5, "{name}: outputs within 1e-5 of the peak of the float layer's over the widened halves ({off.out}, at output {off.at})")
            t |> equal(off.rows, 0, "{name}: weight-row elements off the float layer's")
            let ctl = halves_linear_off(shape.nin, shape.nout, true)
            t |> success(ctl.finite && ctl.out > 1.0e-5 && ctl.ref > 1.0e-5 && ctl.rows == 1, "{name} control: one flipped bit moves the outputs ({ctl.out}, at output {ctl.at}; finite; {ctl.ref} of the peak off the double dot) and one weight ({ctl.rows})")
        }
    }
}

[test(description = "`conv1d_dims_of` maps GGUF conv dims to (cin, cout) for forward and transposed orientations, multiplying the groups into the grouped axis, pinned to exact values")]
def test_conv1d_dims_of(t : T?) {
    t |> run("conv1d_dims_of: the GGUF dims to (cin, cout) on both orientations, the groups folded into the grouped axis") @(t : T?) {
        let dims <- [3l, 8l, 20l]
        let fwd = conv1d_dims_of(dims, false, 2l)
        t |> equal(fwd.cin, 16l, "forward: cin = dims[1] * groups")
        t |> equal(fwd.cout, 20l, "forward: cout = dims[2]")
        let tr = conv1d_dims_of(dims, true, 2l)
        t |> equal(tr.cin, 20l, "transposed: cin = dims[2]")
        t |> equal(tr.cout, 16l, "transposed: cout = dims[1] * groups")
    }
}

//! a carrier's image fields alone: what `tts_image_origin` reads
struct private FxCarrier {
    image_map : void?
    image_bytes : uint64
}

[test(description = "`tts_image_origin` of a carrier that holds no image answers no mapping and no tag, reading no image header")]
def test_image_origin_without_an_image(t : T?) {
    t |> run("a carrier with no image has no origin to read") @(t : T?) {
        let origin = tts_image_origin(FxCarrier())
        t |> success(!origin.mapped && empty(origin.tag), "mapped {origin.mapped}, tag '{origin.tag}'")
    }
}

// ARCHITECTURE_TTS.md#tts-two-layouts: a rows kernel's result never depends on how its rows split
// across the parallel workers. Two axes move that split, and every kernel runs both. The batch
// lane cap: 1 collapses every rows GEMM to one inline chunk, 0 gives the box's lanes times the
// oversplit. The jobque worker limit: 1 leaves one live worker plus the caller lane, the box's
// job count wakes the whole pool - and unlike the cap it moves get_dispatch_lanes(), so it is
// the only axis that reaches the AdaIN column stats and the bare lanes_for_work sites.
[test(description = "The rows convs (f32 and q8), AdaIN, attention and dense layer produce bit-identical output at one dispatch lane and at many, on both the batch-lane-cap and worker-limit axes, with a witness that each axis moved the split")]
def test_rows_split_invariance(t : T?) {
    with_dasllama_jobque_() {
        split_arms(t, SplitAxis.lane_cap)
        split_arms(t, SplitAxis.worker_limit)
    }
}

// ===== the causal cached attention, ELU and layer scale (the continuous-audio family's kernels) =====

let CATT_HIDDEN = 256l
let CATT_HEADS = 4l

// a cache filled with `n` random positions, plus the q rows of the last `t` of them
def private catt_fixture(var r : TtsRng; n, t : int64; var kc : TtsKvCache; var q : array<float>) {
    kv_cache_init(kc, CATT_HEADS, CATT_HIDDEN / CATT_HEADS, n + 8l)
    var inscope k, v : array<float>
    rand_fill(r, k, n * CATT_HIDDEN, 1.0)
    rand_fill(r, v, n * CATT_HIDDEN, 1.0)
    kv_cache_append(kc, k, v, n, CATT_HIDDEN)
    rand_fill(r, q, t * CATT_HIDDEN, 1.0)
}

// the rows form against the double-precision reference over the same cache: an absolute bar,
// since the softmax bounds every output by the values' magnitude; the bar's control moves the
// reference by twice the bar on one element
def private catt_case(t : T?; name : string; n, nt, context : int64; seed : uint64) {
    var r = TtsRng()
    rng_seed(r, seed)
    var inscope kc = TtsKvCache()
    var inscope q : array<float>
    catt_fixture(r, n, nt, kc, q)
    var inscope want, got : array<float>
    attention_causal_ref(q, nt, CATT_HIDDEN, context, kc, want)
    attention_causal_rows(q, nt, CATT_HIDDEN, context, kc, got)
    var inscope env <- plane_of(length(want), 1.0)
    let bar = Bar(kind = BarKind.envelope, rel = CATT_BAR, abs_floor = FLOOR, poison = 2.0 * CATT_BAR, poison_at = 7)
    check(t, "causal attention {name}", got, want, bar, Shape(row_width = int(CATT_HIDDEN)), env)
}

let CATT_BAR = 1.0e-5

// the same fixture under a pinned split leg: the rows form must not move with the lanes
def private catt_split_case(name : string; n, t : int64; axis : SplitAxis) : SplitCase {
    var r = TtsRng()
    rng_seed(r, 0xca77ul + uint64(n))
    var inscope kc = TtsKvCache()
    var inscope q : array<float>
    catt_fixture(r, n, t, kc, q)
    var inscope one, many : array<float>
    at_split(axis, true) {
        attention_causal_rows(q, t, CATT_HIDDEN, 0l, kc, one)
    }
    at_split(axis, false) {
        attention_causal_rows(q, t, CATT_HIDDEN, 0l, kc, many)
    }
    return split_diff(name, one, many)
}

[test(description = "`attention_causal_rows` matches a double-precision reference within 1e-5 across prompt, decode and windowed shapes, a grown KV cache answers bit-identically, a one-key window returns that value row, results are split-invariant, and cache overflow panics")]
def test_causal_attention(t : T?) {
    with_dasllama_jobque_() {
        catt_case(t, "prompt 40 of 40, full", 40l, 40l, 0l, 11ul)
        catt_case(t, "decode step at 41", 41l, 1l, 0l, 12ul)
        catt_case(t, "prompt 300 of 300, window 250", 300l, 300l, 250l, 13ul)
        catt_case(t, "decode at 301, window 250", 301l, 1l, 250l, 14ul)
        catt_case(t, "17 new rows over 90, window 64", 90l, 17l, 64l, 15ul)
        catt_case(t, "prompt 700 of 700, window 250: three query blocks", 700l, 700l, 250l, 17ul)
        catt_case(t, "prompt 600 of 600, full: the window is the whole prefix", 600l, 600l, 0l, 18ul)
        catt_case(t, "300 new rows over 900, window 100", 900l, 300l, 100l, 19ul)
        // a grown cache answers as the cache it grew from
        {
            var rg = TtsRng()
            rng_seed(rg, 20ul)
            var inscope kc = TtsKvCache()
            var inscope q : array<float>
            catt_fixture(rg, 30l, 5l, kc, q)
            var inscope before : array<float>
            attention_causal_rows(q, 5l, CATT_HIDDEN, 0l, kc, before)
            kv_cache_grow(kc, 200l)
            t |> equal(int(kc.cap), 200, "the cache grew to the asked capacity")
            t |> equal(int(kc.n), 30, "the fill survived the growth")
            var inscope after : array<float>
            attention_causal_rows(q, 5l, CATT_HIDDEN, 0l, kc, after)
            let g = split_diff("grown cache", before, after)
            t |> equal(g.diffs, 0, "elements that differ after the growth")
            kv_cache_grow(kc, 100l)
            t |> equal(int(kc.cap), 200, "a smaller capacity leaves the cache as it is")
            var inscope k : array<float>
            rand_fill(rg, k, 150l * CATT_HIDDEN, 1.0)
            kv_cache_append(kc, k, k, 150l, CATT_HIDDEN)
            t |> equal(int(kc.n), 180, "the grown cache takes rows the old capacity refused")
        }
        // a window that admits one key is the value row itself
        var r = TtsRng()
        rng_seed(r, 16ul)
        var inscope kc = TtsKvCache()
        var inscope q : array<float>
        catt_fixture(r, 12l, 1l, kc, q)
        var inscope got : array<float>
        attention_causal_rows(q, 1l, CATT_HIDDEN, 1l, kc, got)
        var worst = 0.0
        for (h in range64(CATT_HEADS)) {
            for (e in range64(CATT_HIDDEN / CATT_HEADS)) {
                worst = max(worst, nan_wide_diff(got[h * (CATT_HIDDEN / CATT_HEADS) + e], kc.v[(h * kc.cap + 11l) * (CATT_HIDDEN / CATT_HEADS) + e]))
            }
        }
        t |> success(worst < 1.0e-6, "a one-key window returns that key's value row (max |diff| {worst})")
        for (axis in [SplitAxis.lane_cap, SplitAxis.worker_limit]) {
            t |> success(shaped_lanes(axis, true) < shaped_lanes(axis, false), "the {axis_name(axis)} axis moves the split")
            let s = catt_split_case("prompt 200 on the {axis_name(axis)} axis", 200l, 200l, axis)
            t |> equal(s.diffs, 0, "{s.name}: elements that differ between the legs")
        }
        // the cache refuses to overflow, and a query count past the fill is refused
        var tripped = false
        try {
            var inscope k : array<float>
            rand_fill(r, k, 20l * CATT_HIDDEN, 1.0)
            kv_cache_append(kc, k, k, 20l, CATT_HIDDEN)
        } recover {
            tripped = true
        }
        t |> success(tripped, "kv_cache_append panics past the cache's capacity")
    }
}

//! `group` query heads a cache head: bit-equal to the plain form over the cache with each key-value head repeated `group` times (same sums, same order), and within the attention bar of the double reference
def private gqa_case(t : T?; name : string; n, nt, context, group : int64; seed : uint64) {
    var r = TtsRng()
    rng_seed(r, seed)
    let dh = CATT_HIDDEN / CATT_HEADS
    let hq = CATT_HEADS * group
    let hidden = hq * dh
    var inscope kc = TtsKvCache()
    var inscope q : array<float>
    kv_cache_init(kc, CATT_HEADS, dh, n + 8l)
    var inscope k, v : array<float>
    rand_fill(r, k, n * CATT_HIDDEN, 1.0)
    rand_fill(r, v, n * CATT_HIDDEN, 1.0)
    kv_cache_append(kc, k, v, n, CATT_HIDDEN)
    rand_fill(r, q, nt * hidden, 1.0)
    var inscope got, want, wide : array<float>
    attention_causal_rows(q, nt, hidden, context, kc, got)
    attention_causal_ref(q, nt, hidden, context, kc, want)
    var inscope env <- plane_of(length(want), 1.0)
    let bar = Bar(kind = BarKind.envelope, rel = CATT_BAR, abs_floor = FLOOR, poison = 2.0 * CATT_BAR, poison_at = 7)
    check(t, "grouped-query attention {name}", got, want, bar, Shape(row_width = int(hidden)), env)
    var inscope kp := k
    for (e in range64(dh)) {
        kp[e] += 3.0   //! the control's input: the first key's first head moved
    }
    var inscope kcp = TtsKvCache()
    kv_cache_init(kcp, CATT_HEADS, dh, n + 8l)
    kv_cache_append(kcp, kp, v, n, CATT_HIDDEN)
    var inscope got_p : array<float>
    attention_causal_rows(q, nt, hidden, context, kcp, got_p)
    check_exceeds(t, "grouped-query attention {name} control: a moved key", got_p, want, bar)
    var inscope kx = TtsKvCache()
    kv_cache_init(kx, hq, dh, n + 8l)
    var inscope k2, v2 : array<float>
    k2 |> resize(n * hidden)
    v2 |> resize(n * hidden)
    for (p in range64(n)) {
        for (h in range64(hq)) {
            for (e in range64(dh)) {
                k2[p * hidden + h * dh + e] = k[p * CATT_HIDDEN + (h / group) * dh + e]
                v2[p * hidden + h * dh + e] = v[p * CATT_HIDDEN + (h / group) * dh + e]
            }
        }
    }
    kv_cache_append(kx, k2, v2, n, hidden)
    attention_causal_rows(q, nt, hidden, context, kx, wide)
    let d = split_diff("{name}: grouped against the expanded cache", got, wide)
    t |> equal(d.diffs, 0, "{d.name}: elements that differ")
}

[test(description = "Grouped-query causal attention matches the double reference within 1e-5 and equals bit for bit the plain form over a cache with each KV head repeated, a moved key must exceed the bar, and widths off the head lattice are refused")]
def test_grouped_query_attention(t : T?) {
    with_dasllama_jobque_() {
        gqa_case(t, "prompt 40 of 40, two query heads a cache head", 40l, 40l, 0l, 2l, 31ul)
        gqa_case(t, "decode step at 41, four a head", 41l, 1l, 0l, 4l, 32ul)
        gqa_case(t, "17 new rows over 90, window 64, two a head", 90l, 17l, 64l, 2l, 33ul)
        gqa_case(t, "one query head a cache head is the plain form", 30l, 5l, 0l, 1l, 34ul)
        var r = TtsRng()
        rng_seed(r, 35ul)
        var inscope kc = TtsKvCache()
        var inscope q : array<float>
        catt_fixture(r, 12l, 1l, kc, q)
        var inscope ctx : array<float>
        var tripped = 0
        for (hidden in [CATT_HIDDEN + 8l, CATT_HIDDEN / 2l, CATT_HIDDEN / CATT_HEADS * 6l]) {
            try {
                attention_causal_rows(q, 1l, hidden, 0l, kc, ctx)
            } recover {
                tripped++
            }
        }
        t |> equal(tripped, 3, "a width off the head lattice, fewer query heads than cache heads, and a count the cache's heads do not divide are refused")
    }
}

//! rows of a served width packed to their channel count: every kept value at [r][c], nothing moved when the strides agree, a wider target refused
[test(description = "`compact_rows` packs 16-wide rows to 12 with every kept value exactly at its new place, leaves rows alone when strides agree, and panics on a wider target")]
def test_compact_rows(t : T?) {
    var inscope y : array<float>
    y |> resize(5 * 16)
    for (r in range(5)) {
        for (c in range(16)) {
            y[r * 16 + c] = float(r * 100 + c)
        }
    }
    compact_rows(y, 5l, 16l, 12l)
    var bad = 0
    for (r in range(5)) {
        for (c in range(12)) {
            bad += y[r * 12 + c] == float(r * 100 + c) ? 0 : 1
        }
    }
    t |> equal(bad, 0, "elements out of place after packing 16-wide rows to 12")
    var inscope same : array<float>
    same |> resize(8)
    for (i in range(8)) {
        same[i] = float(i)
    }
    compact_rows(same, 2l, 4l, 4l)
    t |> equal(same[7], 7.0, "equal strides leave the rows as they are")
    var tripped = false
    try {
        compact_rows(same, 2l, 4l, 8l)
    } recover {
        tripped = true
    }
    t |> success(tripped, "a wider target is refused")
}

//! the onset ramp: sample i of the first n scaled by i / (n - 1), the rest untouched, clamped past the wave's end, a no-op at 0 or 1 samples
[test(description = "`fade_in` scales sample i of the first n by i over n - 1 within 1e-6 and leaves the rest untouched, keeps the long ramp's step on a wave shorter than n, and is a no-op at 0 or 1 samples, with a one-sample-longer control")]
def test_fade_in(t : T?) {
    var inscope w : array<float>
    w |> resize(10)
    for (i in range(10)) {
        w[i] = 2.0
    }
    fade_in(w, 5l)
    var worst = 0.0
    for (i in range(5)) {
        worst = max(worst, nan_wide_diff(w[i], float(2.0lf * double(i) / 4.0lf)))
    }
    t |> success(worst < 1.0e-6, "the first five samples ramp 0, 0.5, 1, 1.5, 2 (max |diff| {worst})")
    var inscope w6 : array<float>
    w6 |> resize(10)
    for (i in range(10)) {
        w6[i] = 2.0
    }
    fade_in(w6, 6l)
    var off = 0.0
    for (i in range(5)) {
        off = max(off, nan_wide_diff(w6[i], float(2.0lf * double(i) / 4.0lf)))
    }
    t |> success(off > 1.0e-6, "the control: a ramp one sample longer lands outside the bar (max |diff| {off})")
    t |> equal(w[5], 2.0, "the sixth sample is untouched")
    t |> equal(w[9], 2.0, "the last sample is untouched")
    var inscope s : array<float>
    s |> resize(3)
    for (i in range(3)) {
        s[i] = 1.0
    }
    fade_in(s, 120l)
    t |> equal(s[0], 0.0, "a ramp past the end starts at 0")
    t |> success(abs(s[2] - 2.0 / 119.0) < 1.0e-6, "and keeps the ramp's step - the wave is the start of a longer one")
    var inscope u : array<float>
    u |> resize(4)
    for (i in range(4)) {
        u[i] = 0.7
    }
    fade_in(u, 1l)
    fade_in(u, 0l)
    t |> equal(u[0], 0.7, "a ramp of one or zero samples changes nothing")
}

//! the double form of `end_on_pause` at its defaults: 20 ms frame RMS in dB, the tail more than 35 dB under the loudest frame cut, a linear fade over the last 20 ms, 80 ms of zeros appended
def private end_on_pause_ref(pcm : array<float>; rate : int) : array<float> {
    let frame = int64(0.02 * float(rate))
    let n = long_length(pcm) / frame
    var inscope db : array<double>
    db |> resize(n)
    var loudest = -1.0e300lf
    for (f in range64(n)) {
        var acc = 0.0lf
        for (i in range64(frame)) {
            acc += double(pcm[f * frame + i]) * double(pcm[f * frame + i])
        }
        db[f] = 20.0lf * log10(sqrt(acc / double(frame)) + 1e-12lf)
        loudest = max(loudest, db[f])
    }
    var last = n - 1l
    while (last > 0l && db[last] <= loudest - 35.0lf) {
        last--
    }
    let end = (last + 1l) * frame
    let fade = min(int64(0.02 * float(rate)), end)
    let pause = int64(0.08 * float(rate))
    var inscope out : array<float>
    out |> resize(end + pause)
    for (i in range64(end)) {
        out[i] = pcm[i]
    }
    for (i in range64(fade)) {
        out[end - fade + i] = float(double(out[end - fade + i]) * (1.0lf - double(i) / double(fade - 1l)))
    }
    return <- out
}

[test(description = "`end_on_pause` cuts the tail 35 dB under the loudest 20 ms frame, fades the last 20 ms to zero and appends 80 ms of silence, matching a double-precision form within 1e-6, exact on the one-sample-fade and short-clip edges")]
def test_end_on_pause(t : T?) {
    let rate = 24000
    // half a second of tone, then 300 ms of near-silence 80 dB down: the silence is cut, 80 ms of zeros follow the fade
    var inscope pcm : array<float>
    pcm |> resize(rate / 2 + rate * 3 / 10)
    for (i in range(rate / 2)) {
        pcm[i] = 0.5 * sin(float(i) * 0.03)
    }
    for (i in range(rate / 2, length(pcm))) {
        pcm[i] = 1.0e-4 * sin(float(i) * 0.7)
    }
    var inscope out : array<float>
    end_on_pause(pcm, rate, out)
    t |> equal(long_length(out), int64(rate / 2 + rate * 8 / 100), "the tail is cut at the last loud frame and 80 ms of pause follow")
    var inscope want <- end_on_pause_ref(pcm, rate)
    t |> equal(length(out), length(want), "the double form agrees on the length")
    var worst = 0.0
    for (g, w in out, want) {
        worst = max(worst, nan_wide_diff(g, w))
    }
    t |> success(worst <= 1.0e-6, "end_on_pause against the double form (max |diff| {worst})")
    var inscope pcm_off := pcm
    for (i in range(rate / 2 - 480, rate / 2)) {
        pcm_off[i] = 1.0e-4 * sin(float(i) * 0.7)   //! the control's input: the tone one frame (20 ms) shorter
    }
    var inscope want_off <- end_on_pause_ref(pcm_off, rate)
    t |> success(length(want_off) < length(out), "the control: a tone one frame shorter cuts one frame earlier ({length(want_off)} against {length(out)} samples)")
    var inscope coarse : array<float>   //! at 50 Hz a frame is one sample and the 20 ms fade one sample: that sample alone goes to zero
    coarse |> resize(40)
    for (i in range(40)) {
        coarse[i] = i < 25 ? 0.5 : 1.0e-4
    }
    var inscope out50 : array<float>
    end_on_pause(coarse, 50, out50)
    t |> equal(length(out50), 25 + 4, "the one-sample fade: 25 loud samples kept and the 80 ms pause is four samples")
    t |> equal(out50[24], 0.0, "the one-sample fade zeroes the last kept sample")
    t |> equal(out50[23], 0.5, "and leaves the one before it")
    t |> equal(out[rate / 2 - 1], 0.0, "the last kept sample is faded to zero")
    t |> equal(out[length(out) - 1], 0.0, "the pause is silence")
    t |> success(abs(out[rate / 2 - 480] - pcm[rate / 2 - 480]) < 1.0e-6, "the fade starts at full level 20 ms before the cut")
    var inscope flat : array<float>
    flat |> resize(rate / 10)
    for (i in range(length(flat))) {
        flat[i] = 0.01 * sin(float(i) * 0.05)
    }
    var inscope out2 : array<float>
    end_on_pause(flat, rate, out2)
    t |> equal(length(out2), length(flat) + rate * 8 / 100, "a clip loud to its end keeps every frame and gains the pause")
    var inscope tiny : array<float>
    tiny |> resize(100)
    var inscope out3 : array<float>
    end_on_pause(tiny, rate, out3)
    t |> equal(length(out3), 100, "a clip shorter than a frame comes back as it is")
}

[test(description = "`elu_rows` matches scalar ELU within 1e-6, `layer_scale_rows` equals the per-channel product exactly, `pad_edge_left_rows` repeats the first row, and `rng_normal` panics on an unseeded generator instead of spinning")]
def test_elu_and_layer_scale(t : T?) {
    with_dasllama_jobque_() {
        var r = TtsRng()
        rng_seed(r, 21ul)
        var inscope x : array<float>
        rand_fill(r, x, 4096l, 6.0)
        var inscope want : array<float>
        want |> resize(length(x))
        for (src, dst in x, want) {
            dst = src > 0.0 ? src : exp(src) - 1.0
        }
        elu_rows(x, 4096l)
        var worst = 0.0
        for (g, w in x, want) {
            worst = max(worst, nan_wide_diff(g, w))
        }
        t |> success(worst <= 1.0e-6, "elu_rows against the scalar form (max |diff| {worst})")
        var inscope y : array<float>
        rand_fill(r, y, 16l * 64l, 1.0)
        var inscope s : array<float>
        rand_fill(r, s, 64l, 0.5)
        var inscope want2 : array<float>
        want2 |> resize(length(y))
        for (i in range64(16l * 64l)) {
            want2[i] = y[i] * s[i % 64l]
        }
        layer_scale_rows(y, 16l, 64l, s)
        var exact = 0
        for (g, w in y, want2) {
            if (g != w) {
                exact++
            }
        }
        t |> equal(exact, 0, "layer_scale_rows elements that differ from the per-channel product")
        var inscope padded : array<float>
        pad_edge_left_rows(y, 16l, 64l, 3l, padded)
        t |> equal(length(padded), 19 * 64, "pad_edge_left_rows grows by the pad rows")
        var edge_ok = true
        for (rr in range64(3l)) {
            for (j in range64(64l)) {
                edge_ok = edge_ok && padded[rr * 64l + j] == y[j]
            }
        }
        for (i in range64(16l * 64l)) {
            edge_ok = edge_ok && padded[3l * 64l + i] == y[i]
        }
        t |> success(edge_ok, "pad_edge_left_rows repeats the first row and keeps the rest")
        // an unseeded generator must refuse, not spin: its polar loop never leaves the unit disc
        var fresh = TtsRng()
        var tripped = false
        try {
            let v = rng_normal(fresh)
        } recover {
            tripped = true
        }
        t |> success(tripped, "rng_normal panics on a generator nobody seeded")
        rng_seed(fresh, 5ul)
        let v2 = rng_normal(fresh)
        t |> success(abs(v2) < 10.0, "a seeded generator draws")
    }
}

// the two elementwise kernels that dispatch, bit-equal on both axes that move the split
[test(description = "`elu_rows` and `layer_scale_rows` give bit-identical output at one dispatch lane and at many on both the lane-cap and worker-limit axes, each axis witnessed to move the split")]
def test_elementwise_split_invariance(t : T?) {
    with_dasllama_jobque_() {
        var r = TtsRng()
        rng_seed(r, 23ul)
        for (axis in [SplitAxis.lane_cap, SplitAxis.worker_limit]) {
            t |> success(shaped_lanes(axis, true) < shaped_lanes(axis, false), "the {axis_name(axis)} axis moves the split")
            var inscope e1, e2 : array<float>
            rand_fill(r, e1, 8192l, 6.0)
            e2 := e1
            at_split(axis, true) {
                elu_rows(e1, 8192l)
            }
            at_split(axis, false) {
                elu_rows(e2, 8192l)
            }
            let se = split_diff("elu_rows @ {axis_name(axis)}", e1, e2)
            t |> equal(se.diffs, 0, "{se.name}: elements that differ between the legs")
            var inscope l1, l2 : array<float>
            rand_fill(r, l1, 64l * 128l, 1.0)
            l2 := l1
            var inscope ls : array<float>
            rand_fill(r, ls, 128l, 0.5)
            at_split(axis, true) {
                layer_scale_rows(l1, 64l, 128l, ls)
            }
            at_split(axis, false) {
                layer_scale_rows(l2, 64l, 128l, ls)
            }
            let sl = split_diff("layer_scale_rows @ {axis_name(axis)}", l1, l2)
            t |> equal(sl.diffs, 0, "{sl.name}: elements that differ between the legs")
        }
    }
}

// rope over rows against the leaf applied per row, and the variance-scaled norm against its
// double-precision form
[test(description = "`rope_rows` equals the per-row `rope_scaled` leaf bit for bit at each row's own position and takes zero rows, and `var_scale_norm` matches its double-precision form within 1e-6, a poisoned expectation missing the bar")]
def test_rope_rows_and_var_scale_norm(t : T?) {
    var r = TtsRng()
    rng_seed(r, 24ul)
    var inscope rx : array<float>
    rand_fill(r, rx, 6l * 64l, 1.0)
    var inscope want_rope := rx
    for (row in range64(6l)) {
        rope_scaled(unsafe(addr(want_rope[row * 64l])), 100l + row, 16l, 64l, 10000.0, 1.0, false, null)
    }
    rope_rows(rx, 6l, 64l, 16l, 100l, 10000.0)
    var rope_diffs = 0
    for (g, w in rx, want_rope) {
        if (g != w) {
            rope_diffs++
        }
    }
    t |> equal(rope_diffs, 0, "rope_rows elements that differ from the leaf applied per row")
    t |> success(rx[0] != want_rope[64] || rx[1] != want_rope[65], "the rows are rotated at their own positions, not one")
    var inscope no_rows : array<float>
    rope_rows(no_rows, 0l, 64l, 16l, 100l, 10000.0)
    t |> success(empty(no_rows), "no rows: nothing roped, and the empty plane is not read")
    var inscope vx : array<float>
    rand_fill(r, vx, 256l, 0.01)
    var inscope valpha : array<float>
    rand_fill(r, valpha, 256l, 1.0)
    var mean = 0.0lf
    for (v in vx) {
        mean += double(v)
    }
    mean /= 256.0lf
    var variance = 0.0lf
    for (v in vx) {
        variance += (double(v) - mean) * (double(v) - mean)
    }
    variance /= 255.0lf
    var inscope want_norm : array<float>
    want_norm |> resize(256)
    for (j in range(256)) {
        want_norm[j] = float(double(vx[j]) * double(valpha[j]) / sqrt(variance + 1e-5lf))
    }
    var_scale_norm(vx, valpha, 256l, 1.0e-5)
    var norm_worst = 0.0
    for (g, w in vx, want_norm) {
        norm_worst = max(norm_worst, nan_wide_diff(g, w))
    }
    t |> success(norm_worst <= 1.0e-6, "var_scale_norm against the double form (max |diff| {norm_worst})")
    // the control: a poisoned double form lands outside the bar
    want_norm[3] += 1.0e-5
    var poisoned_worst = 0.0
    for (g, w in vx, want_norm) {
        poisoned_worst = max(poisoned_worst, nan_wide_diff(g, w))
    }
    t |> success(poisoned_worst > 1.0e-6, "the norm bar discriminates a poisoned expectation")
}

def private rows_conv_panics(c : TtsConv1d; x : array<float>; t_in : int64) : bool {
    var inscope y : array<float>
    var tripped = false
    try {
        conv1d_rows(c, x, t_in, y)
    } recover {
        tripped = true
    }
    return tripped
}

[test(description = "`conv1d_rows` panics on a grouped conv and on a strided forward q8 conv, and `conv1d_pad_cin` panics on a depthwise-grouped conv whose cout differs from its cin")]
def test_rows_conv_refuses_the_wrong_shape(t : T?) {
    t |> run("a grouped conv is not a rows conv") @(t : T?) {
        var r = TtsRng()
        rng_seed(r, 1ul)
        var inscope c0 <- make_conv(r, 8l, 8l, 3l, 1l, 1l, 1l, false, false)
        c0.groups = 8l
        var inscope c <- prepared(c0, false)
        t |> equal(c.cout_s, 8l)
        var inscope x : array<float>
        rand_fill(r, x, 8l * 16l, 1.0)
        t |> success(rows_conv_panics(c, x, 16l), "conv1d_rows panics on a grouped conv")
    }
    t |> run("a forward conv's q8 rows form is stride 1") @(t : T?) {
        var r = TtsRng()
        rng_seed(r, 2ul)
        var inscope c0 <- make_conv(r, 32l, 32l, 3l, 1l, 1l, 1l, false, false)
        var inscope cq <- prepared(c0, true, true)
        t |> success(cq.q8, "the fixture minted the q8 lane")
        var inscope x : array<float>
        rand_fill(r, x, 32l * 16l, 1.0)
        cq.stride = 2l
        t |> success(rows_conv_panics(cq, x, 16l), "conv1d_rows panics on a strided q8 conv")
        cq.stride = 1l
    }
    t |> run("conv1d_pad_cin takes a depthwise conv whose cout is its cin") @(t : T?) {
        var r = TtsRng()
        rng_seed(r, 3ul)
        var inscope c0 <- make_conv(r, 8l, 16l, 3l, 1l, 1l, 1l, false, false)
        c0.groups = 8l   // depthwise by its group count, but cout is not cin
        var tripped = false
        try {
            conv1d_pad_cin(c0, 32l)
        } recover {
            tripped = true
        }
        t |> success(tripped, "conv1d_pad_cin panics when a depthwise conv's cout is not its cin")
    }
}

// ===== the harmonic source's window forms: a window is where a value is computed, never what =====

// the window form of a resampler on either arithmetic - output columns [to0, to1) from the source
// columns its taps read, both held at strides off the whole row - bit-equal to the whole row
def private window_resize_arm(t : T?; torch_math : bool) {
    var r = TtsRng()
    rng_seed(r, torch_math ? 21ul : 20ul)
    let c = 3l
    let tt = 300l
    let name = torch_math ? "resize_linear_torch" : "resize_linear"
    for (scale in [1.0 / 300.0, 300.0, 0.5, 2.0]) {   // the source's two scales, and two off them
        var inscope x : array<float>
        rand_fill(r, x, c * tt, 1.0)
        var inscope whole : array<float>
        let t_out = resize_linear(torch_math, x, c, tt, scale, whole)
        let to0 = t_out / 3l
        let to1 = min(to0 + 7l, t_out)
        let span = resize_span(torch_math, tt, scale, to0, to1)
        let width = span.hi - span.lo
        let x_stride = width + 5l
        var inscope xw : array<float>
        xw |> resize(c * x_stride)
        for (ci in range64(c)) {
            for (i in range64(width)) {
                xw[ci * x_stride + i] = x[ci * tt + span.lo + i]
            }
        }
        let y_stride = (to1 - to0) + 3l
        var inscope yw : array<float>
        yw |> resize(c * y_stride)
        if (torch_math) {
            resize_linear_torch(xw, span.lo, x_stride, c, tt, scale, to0, to1, yw, y_stride)
        } else {
            resize_linear(xw, span.lo, x_stride, c, tt, scale, to0, to1, yw, y_stride)
        }
        var diffs = 0
        for (ci in range64(c)) {
            for (k in range64(to1 - to0)) {
                diffs += yw[ci * y_stride + k] == whole[ci * t_out + to0 + k] ? 0 : 1
            }
        }
        t |> equal(diffs, 0, "{name} scale={scale}: the window [{to0}, {to1}) from source columns [{span.lo}, {span.hi}) at strides {x_stride}/{y_stride} - {diffs} values off the whole row")
    }
}

def private append_range(var dst : array<float>; src : array<float>; off, n : int64) {
    let at = long_length(dst)
    dst |> resize(at + n)
    for (i in range64(n)) {
        dst[at + i] = src[off + i]
    }
}

// the source over a whole run of `frames`, `window` phase frames a step (0 = one step): the rows
// of every window in order
def private sine_run(cfg : SineSourceCfg; f0 : array<float>; frames : int64; noise_u, noise_n : array<float>;
                     window : int64; var out : array<float>) {
    let nlow = sine_source_geometry(cfg, frames).nlow
    let win = window <= 0l ? nlow : window
    var inscope cr = SineSourceCarry()
    sine_carry_reset(cr, cfg.n_harm, win)
    var inscope rows : array<float>
    var fa = 0l
    while (fa < nlow) {
        let fb = min(fa + win, nlow)
        let got = sine_source(cfg, f0, frames, noise_u, fa, fb, cr, noise_n, fa * cfg.upsample * cfg.n_harm, rows)
        append_range(out, rows, 0l, got * cfg.n_harm)
        fa = fb
    }
}

def private sine_window_arm(t : T?; torch_math : bool) {
    var cfg = SineSourceCfg()
    cfg.torch_math = torch_math
    let frames = 23l
    let n = frames * cfg.upsample
    var r = TtsRng()
    rng_seed(r, torch_math ? 31ul : 30ul)
    var inscope f0 : array<float>
    f0 |> resize(frames)
    for (i in range64(frames)) {   // voiced frames around 100-400 Hz, two unvoiced ones the threshold decides
        f0[i] = (i == 5l || i == 17l) ? 0.0 : 100.0 + 300.0 * rng_uniform(r)
    }
    var inscope noise_u : array<float>
    noise_u |> resize(cfg.n_harm)
    for (h in range64(cfg.n_harm)) {
        noise_u[h] = rng_uniform(r)
    }
    var inscope noise_n : array<float>
    noise_n |> resize(n * cfg.n_harm)
    for (i in range64(n * cfg.n_harm)) {
        noise_n[i] = rng_normal(r)
    }
    var inscope whole : array<float>
    sine_run(cfg, f0, frames, noise_u, noise_n, 0l, whole)
    t |> equal(long_length(whole), n * cfg.n_harm, "{n} samples of {cfg.n_harm} harmonics from the whole run")
    let name = torch_math ? "PyTorch's arithmetic" : "onnxruntime's arithmetic"
    for (window in [1l, 5l, 16l]) {
        var inscope windowed : array<float>
        sine_run(cfg, f0, frames, noise_u, noise_n, window, windowed)
        t |> equal(long_length(windowed), long_length(whole), "{name}, {window}-frame windows: the same sample count")
        var diffs = 0
        for (a, b in whole, windowed) {
            diffs += a == b ? 0 : 1
        }
        t |> equal(diffs, 0, "{name}, {window}-frame windows: {diffs} of {long_length(whole)} source values move with the window")
    }
    // the mix the source hands linear_rows: a window's rows through the rows GEMM equal the whole run's
    // rows, on both axes that move the row split
    var inscope l = TtsLinear(nin = cfg.n_harm, nout = 1l)
    rand_fill(r, l.w, cfg.n_harm, 1.0)
    rand_fill(r, l.b, 1l, 0.5)
    linear_prepare(l)
    var inscope har_whole : array<float>
    linear_rows(l, whole, n, har_whole)
    for (axis in [SplitAxis.lane_cap, SplitAxis.worker_limit]) {
        for (narrow in [true, false]) {
            at_split(axis, narrow) {
                var inscope har : array<float>
                let step = 5l * cfg.upsample
                var sa = 0l
                while (sa < n) {
                    let rows = min(step, n - sa)
                    var inscope xw : array<float>
                    append_range(xw, whole, sa * cfg.n_harm, rows * cfg.n_harm)
                    var inscope hw : array<float>
                    linear_rows(l, xw, rows, hw)
                    append_range(har, hw, 0l, rows)
                    sa += rows
                }
                var diffs = 0
                for (a, b in har_whole, har) {
                    diffs += a == b ? 0 : 1
                }
                t |> equal(diffs, 0, "{name}, the mix in windows at the {narrow ? "narrow" : "wide"} {axis_name(axis)}: {diffs} of {n} samples off the whole run")
            }
        }
    }
}

[test(description = "Window forms are bit-identical to the whole run: both resamplers' windows at offset strides, the harmonic sine source in 1, 5 and 16 frame windows under both arithmetics, and its linear mix under both row splits; an oversized window panics")]
def test_source_windows(t : T?) {
    t |> run("a resampler's window form equals the whole row at offsets and strides off it") @(t : T?) {
        window_resize_arm(t, false)
        window_resize_arm(t, true)
    }
    t |> run("a window past the carry's reset is refused") @(t : T?) {
        let cfg = SineSourceCfg()
        let frames = 10l
        var inscope f0 : array<float>
        f0 |> resize(frames)
        for (i in range64(frames)) {
            f0[i] = 200.0
        }
        var inscope noise_u : array<float>
        noise_u |> resize(cfg.n_harm)
        var inscope noise_n : array<float>
        noise_n |> resize(frames * cfg.upsample * cfg.n_harm)
        var inscope cr = SineSourceCarry()
        sine_carry_reset(cr, cfg.n_harm, 2l)
        var inscope out : array<float>
        var tripped = false
        try {
            sine_source(cfg, f0, frames, noise_u, 0l, 5l, cr, noise_n, 0l, out)
        } recover {
            tripped = true
        }
        t |> success(tripped, "a five-frame window on a carry reset for two panics")
    }
    t |> run("the harmonic source in frame windows equals the whole run, and so does its mix under both row splits") @(t : T?) {
        with_dasllama_jobque_() {
            sine_window_arm(t, false)
            sine_window_arm(t, true)
        }
    }
}
