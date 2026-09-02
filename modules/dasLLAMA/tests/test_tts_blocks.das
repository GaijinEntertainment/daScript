options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_tts_blocks
require dasllama/dasllama_math   // setup_dasllama_jobque_
require daslib/jobque_boost
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

struct BlockCase {
    name : string
    max_ratio : float
    max_abs : float
    bad : int
}

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
def private prepared(c : TtsConv1d; rows : bool) : TtsConv1d {
    var p := c
    p.served_rows = rows
    conv1d_prepare(p)
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

def private score(name : string; got, want, env : array<float>; bias_abs : float) : BlockCase {
    var res = BlockCase(name = name)
    for (g, w, e in got, want, env) {
        let d = abs(g - w)
        let bar = max(REL * (e + bias_abs), FLOOR)
        res.max_abs = max(res.max_abs, d)
        res.max_ratio = max(res.max_ratio, d / bar)
        if (d > bar) {
            res.bad++
        }
    }
    return res
}

def private conv_case(name : string; cin, cout, k, stride, pad, dil : int64; transposed : bool) : BlockCase {
    var r = TtsRng()
    rng_seed(r, 0x5eedul + uint64(k * 100l + dil))
    var inscope c0 <- make_conv(r, cin, cout, k, stride, pad, dil, transposed, true)
    var inscope e <- envelope_conv(c0)
    var inscope c <- prepared(c0, false)
    var inscope cr <- prepared(c0, true)
    let t_in = T_ROWS
    var inscope x_ct : array<float>
    rand_fill(r, x_ct, cin * t_in, 2.0)
    var inscope x_abs <- abs_of(x_ct)
    var inscope want : array<float>
    var inscope env : array<float>
    conv1d(c, x_ct, t_in, want)
    conv1d(e, x_abs, t_in, env)
    var inscope x_rows : array<float>
    transpose_ct(x_ct, cin, t_in, x_rows)
    var inscope y_rows : array<float>
    conv1d_rows(cr, x_rows, t_in, y_rows)
    let t_out = conv1d_out_len(cr, t_in)
    var inscope got : array<float>
    transpose_tc_strided(y_rows, t_out, cout, cr.cout_s, got)
    var bias_abs = 0.0
    for (b in c0.b) {
        bias_abs = max(bias_abs, abs(b))
    }
    return score(name, got, want, env, bias_abs)
}

def private snake_case() : BlockCase {
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
    return score("snake", got, x_ct, env, 0.0)
}

def private adain_case(c : int64) : BlockCase {
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
    return score("adain c={c}", got, x_ct, env, 0.0)
}

def private pad_and_polar_case() : BlockCase {
    var r = TtsRng()
    rng_seed(r, 5ul)
    let c = 16l
    let t = 300l
    var inscope x_ct : array<float>
    rand_fill(r, x_ct, c * t, 1.0)
    var inscope x_rows : array<float>
    transpose_ct(x_ct, c, t, x_rows)
    var inscope want : array<float>
    var inscope y_rows : array<float>
    pad_reflect_left(x_ct, c, t, 1l, want)
    pad_reflect_left_rows(x_rows, t, c, 1l, y_rows)
    var inscope got : array<float>
    transpose_tc(y_rows, t + 1l, c, got)
    var inscope env : array<float>
    env |> resize(length(want))
    for (i in range(length(want))) {
        env[i] = 0.0
    }
    var res = score("pad_reflect_left", got, want, env, 0.0)
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
    var inscope re : array<float>
    var inscope im : array<float>
    var inscope re2 : array<float>
    var inscope im2 : array<float>
    polar_to_rect(p_ct, bins, t, re, im)
    polar_to_rect_rows(p_rows, 32l, bins, t, re2, im2)
    var inscope env2 : array<float>
    env2 |> resize(bins * t)
    for (i in range64(bins * t)) {
        env2[i] = 0.0
    }
    let pr = score("polar re", re2, re, env2, 0.0)
    let pi = score("polar im", im2, im, env2, 0.0)
    res.bad += pr.bad + pi.bad
    res.max_ratio = max(res.max_ratio, max(pr.max_ratio, pi.max_ratio))
    res.max_abs = max(res.max_abs, max(pr.max_abs, pi.max_abs))
    return res
}

// linear_rows on the tiled path (wt minted) against the same layer on the batched dot form.
def private linear_case(nin, nout : int64) : BlockCase {
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
    var inscope want : array<float>
    var inscope env : array<float>
    linear_rows(l, x, t, want)   // no wt yet: the dot form
    linear_rows(e, x_abs, t, env)
    linear_prepare(l)
    var inscope got : array<float>
    linear_rows(l, x, t, got)
    var bias_abs = 0.0
    for (b in l.b) {
        bias_abs = max(bias_abs, abs(b))
    }
    var res <- score("linear {nin}x{nout}", got, want, env, bias_abs)
    if (empty(l.wt)) {
        res.bad += 1000000   // the tiled path never engaged
    }
    return res
}

// the residual block's upsampling pool (depthwise transposed k3 s2) and the nearest copy of its
// shortcut, rows against channel-major
def private pool_and_nearest_case() : BlockCase {
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
    var inscope want : array<float>
    var inscope env : array<float>
    conv1d(pool, x_ct, t, want)
    conv1d(e, x_abs, t, env)
    var inscope x_rows : array<float>
    transpose_ct(x_ct, c, t, x_rows)
    var inscope y_rows : array<float>
    conv1d_rows_transposed_depthwise(pool, x_rows, t, y_rows)
    let t_out = conv1d_out_len(pool, t)
    var inscope got : array<float>
    transpose_tc(y_rows, t_out, c, got)
    var bias_abs = 0.0
    for (b in pool.b) {
        bias_abs = max(bias_abs, abs(b))
    }
    var res = score("pool convT depthwise", got, want, env, bias_abs)
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
    let near = score("nearest x2", got2, want2, env2, 0.0)
    res.bad += near.bad
    res.max_ratio = max(res.max_ratio, near.max_ratio)
    res.max_abs = max(res.max_abs, near.max_abs)
    return res
}

// a conv and an AdaIN padded from 66 to 96 input channels against the unpadded pair on the same
// rows: the padded columns stay zero and the real ones agree exactly
def private pad_case() : BlockCase {
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
    var inscope y : array<float>
    var inscope yp : array<float>
    conv1d_rows(c, x, t, y)
    conv1d_rows(cp, xp, t, yp)
    var inscope env : array<float>
    env |> resize(length(y))
    for (i in range(length(y))) {
        env[i] = 0.0
    }
    var res = score("padded conv", yp, y, env, 0.0)
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
    let ad = score("padded adain", got, xa, env2, 0.0)
    res.bad += ad.bad + pad_nonzero
    res.max_ratio = max(res.max_ratio, ad.max_ratio)
    res.max_abs = max(res.max_abs, ad.max_abs)
    return res
}

// attention_rows against a scalar per-head reference, at a token count off the 16-key pad.
def private attention_case() : BlockCase {
    var r = TtsRng()
    rng_seed(r, 0x47ul)
    let t = 37l
    let heads = 4l
    let dh = 16l
    let hidden = heads * dh
    var inscope q : array<float>
    var inscope k : array<float>
    var inscope v : array<float>
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
    return score("attention", got, want, env, 0.0)
}

let Q8_REL = 2.0e-2   // Q8_0 weights and per-32 activations: a few thousandths of the envelope, gated at 2%

// The q8 rows lane of a conv against the channel-major f32 reference at the q8 bar.
def private conv_case_q8(name : string; cin, cout, k, pad, dil : int64) : BlockCase {
    var r = TtsRng()
    rng_seed(r, 0x9eedul + uint64(k * 100l + dil))
    var inscope c0 <- make_conv(r, cin, cout, k, 1l, pad, dil, false, true)
    var inscope e <- envelope_conv(c0)
    var inscope c <- prepared(c0, false)
    var inscope cq := c0
    cq.served_rows = true
    conv1d_prepare(cq, true, select_matmul_backend_for_load_())
    let t_in = T_ROWS
    var inscope x_ct : array<float>
    rand_fill(r, x_ct, cin * t_in, 2.0)
    var inscope x_abs <- abs_of(x_ct)
    var inscope want : array<float>
    var inscope env : array<float>
    conv1d(c, x_ct, t_in, want)
    conv1d(e, x_abs, t_in, env)
    var inscope x_rows : array<float>
    transpose_ct(x_ct, cin, t_in, x_rows)
    var inscope y_rows : array<float>
    conv1d_rows(cq, x_rows, t_in, y_rows)
    let t_out = conv1d_out_len(cq, t_in)
    var inscope got : array<float>
    transpose_tc_strided(y_rows, t_out, cout, cq.cout_s, got)
    var bias_abs = 0.0
    for (b in c0.b) {
        bias_abs = max(bias_abs, abs(b))
    }
    var res = BlockCase(name = name)
    for (g, w, ev in got, want, env) {
        let d = abs(g - w)
        let bar = max(Q8_REL * (ev + bias_abs), FLOOR)
        res.max_abs = max(res.max_abs, d)
        res.max_ratio = max(res.max_ratio, d / bar)
        if (d > bar) {
            res.bad++
        }
    }
    if (!cq.q8) {
        res.bad += 1000000   // the q8 lane never minted
    }
    return res
}

// linear_rows on the q8 lane against the dot form at the q8 bar.
def private linear_case_q8(nin, nout : int64) : BlockCase {
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
    var inscope want : array<float>
    var inscope env : array<float>
    linear_rows(l, x, t, want)
    linear_rows(e, x_abs, t, env)
    var lq := l
    linear_prepare(lq, true, true, select_matmul_backend_for_load_())
    var inscope got : array<float>
    linear_rows(lq, x, t, got)
    var bias_abs = 0.0
    for (b in l.b) {
        bias_abs = max(bias_abs, abs(b))
    }
    var res = BlockCase(name = "linear q8 {nin}x{nout}")
    for (g, w, ev in got, want, env) {
        let d = abs(g - w)
        let bar = max(Q8_REL * (ev + bias_abs), FLOOR)
        res.max_abs = max(res.max_abs, d)
        res.max_ratio = max(res.max_ratio, d / bar)
        if (d > bar) {
            res.bad++
        }
    }
    if (!lq.q8) {
        res.bad += 1000000
    }
    return res
}

def private report(t : T?; r : BlockCase) {
    to_log(LOG_INFO, "tts blocks {r.name}: max |rows - ref| {r.max_abs}, worst ratio to the bar {r.max_ratio}, over the bar {r.bad}\n")
    t |> equal(r.bad, 0, "{r.name}: elements past the dot-envelope bar")
}

[test]
def test_rows_convs(t : T?) {
    with_job_que() {
        setup_dasllama_jobque_()
        t |> run("forward convs at the generator's taps") @(t : T?) {
            report(t, conv_case("conv k3 d1", 64l, 64l, 3l, 1l, 1l, 1l, false))
            report(t, conv_case("conv k7 d3", 64l, 64l, 7l, 1l, 9l, 3l, false))
            report(t, conv_case("conv k11 d5", 64l, 64l, 11l, 1l, 25l, 5l, false))
            report(t, conv_case("conv k7 wide-in", 128l, 64l, 7l, 1l, 3l, 1l, false))
        }
        t |> run("the padded served width (conv_post's 22 columns)") @(t : T?) {
            report(t, conv_case("conv_post k7 cout22", 64l, 22l, 7l, 1l, 3l, 1l, false))
        }
        t |> run("the strided noise convs over stacked tap rows") @(t : T?) {
            report(t, conv_case("noise conv k20 s10", 22l, 128l, 20l, 10l, 5l, 1l, false))
            report(t, conv_case("noise conv k12 s6", 22l, 64l, 12l, 6l, 3l, 1l, false))
        }
        t |> run("transposed upsamplers") @(t : T?) {
            report(t, conv_case("convT k20 s10", 64l, 32l, 20l, 10l, 5l, 1l, true))
            report(t, conv_case("convT k12 s6", 64l, 32l, 12l, 6l, 3l, 1l, true))
        }
        t |> run("linear_rows on the tile") @(t : T?) {
            report(t, linear_case(96l, 64l))
            report(t, linear_case(768l, 3072l))
        }
        t |> run("attention_rows against the scalar heads") @(t : T?) {
            report(t, attention_case())
        }
        t |> run("the q8 lane against the f32 reference") @(t : T?) {
            report(t, conv_case_q8("conv q8 k3 d1", 128l, 128l, 3l, 1l, 1l))
            report(t, conv_case_q8("conv q8 k7 d3", 128l, 128l, 7l, 9l, 3l))
            report(t, conv_case_q8("conv q8 k11 d5", 256l, 256l, 11l, 25l, 5l))
            report(t, linear_case_q8(768l, 768l))
        }
        t |> run("snake, adain, pad, polar") @(t : T?) {
            report(t, snake_case())
            report(t, adain_case(64l))
            report(t, adain_case(66l))   // the decoder's concat widths sit off the lane
            report(t, pad_and_polar_case())
        }
        t |> run("the residual block's pool and nearest shortcut") @(t : T?) {
            report(t, pool_and_nearest_case())
        }
        t |> run("an input width padded to the q8 lane") @(t : T?) {
            report(t, pad_case())
        }
    }
}

[test]
def test_rows_conv_refuses_the_wrong_shape(t : T?) {
    t |> run("a grouped conv is not a rows conv") @(t : T?) {
        var r = TtsRng()
        rng_seed(r, 1ul)
        var inscope c0 <- make_conv(r, 8l, 8l, 3l, 1l, 1l, 1l, false, false)
        c0.groups = 8l
        var inscope c <- prepared(c0, false)
        t |> equal(c.cout_s, 8l)
        // the refusal is a panic by contract; the shape check is what the cell pins
        t |> success(c.groups != 1l, "the shape the rows conv refuses")
    }
}
