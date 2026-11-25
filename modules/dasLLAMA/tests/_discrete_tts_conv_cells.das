options gen2
options indenting = 4
options _comment_hygiene = true
options _dasllama_internal = true

//! The case bodies the TTS sequence-class cells of both discrete homes run - `test_vulkan_tts_conv_kernels.das` over
//! `_vkd_oracles`' `CellPlanes`, `test_cuda_tts_kernels.das` over `_cuda_cell`'s `CuPlanes` - each written once, generic over
//! the rig (`_discrete_cells.das` carries the form): the f32 conv im2col fed through F32GemmB and held to the CPU `conv1d`,
//! one LSTM direction held to the CPU `bilstm` on both walks, and the depthwise transposed pool held to its CPU rows form.
module _discrete_tts_conv_cells shared public

require dastest/testing_boost public
require math
require dasllama/dasllama_lint public
require dasllama/dasllama_gpu_math   // f32_gemm_wgs
require dasllama/dasllama_gpu_kernels_discrete
require dasllama/dasllama_tts_blocks
require dasllama/dasllama_math   // transpose_ct, with_dasllama_jobque_
require daslib/jobque_boost
require ?vulkan dasllama/dasllama_vulkan_classes
require ?vulkan _vkd_oracles
require ?cuda dasllama/dasllama_cuda_classes
require ?cuda dasllama/dasllama_cuda_classes_tts
require ?cuda _cuda_cell
require ?das_metal dasllama/dasllama_metal_classes_tts
require ?das_metal _metal_cell
require _compares
require _discrete_cells   // seats

struct ConvGeom {
    cin : int
    cout : int
    k : int
    stride : int
    pad : int
    tin : int
    tout : int      //! the output length the cell claims, asserted against `conv1d_out_len`
    xoff : int
    coloff : int
    transposed : bool
    rows_x : bool   //! x uploaded row-major [tin][cin] (the tower's rows form) and the im2col told so
}

//! a raw conv, the ONNX weight ([cout][cin][k] forward, [cin][cout][k] transposed) and bias off `row_fill` at `wscale` and `woff`
def make_conv(g : ConvGeom; wscale, woff : float) : TtsConv1d {
    var c = TtsConv1d(cin = int64(g.cin), cout = int64(g.cout), k = int64(g.k), stride = int64(g.stride), pad_l = int64(g.pad),
        pad_r = int64(g.pad), dilation = 1l, groups = 1l, transposed = g.transposed)
    c.w <- row_fill(g.cin * g.cout * g.k, 37, 5, wscale, woff)
    c.b <- row_fill(g.cout, 29, 11, 0.02, 0.9)
    return <- c
}

//! the prepared conv's weights as `tts_slab_conv` lays them, the columns past k * cin poisoned: the im2col's zero columns keep them out
def slab_rows(c : TtsConv1d; kdim : int) : array<float> {
    var w <- [for (_i in range(int(c.cout) * kdim)); POISON]
    for (co in range64(c.cout)) {
        conv1d_weight_row(c, co, w, co * int64(kdim))
    }
    return <- w
}

//! the CPU `conv1d` over x [cin][tin] (the conv prepared as the loader prepares it), transposed to the GEMM's [tout][cout] rows
def conv_ref(var c : TtsConv1d; x : array<float>; tin : int) : array<float> {
    conv1d_prepare(c)
    var y_ct, y : array<float>
    with_dasllama_jobque_() {
        conv1d(c, x, int64(tin), y_ct)
    }
    transpose_ct(y_ct, c.cout, conv1d_out_len(c, int64(tin)), y)
    delete y_ct
    return <- y
}

//! one conv's f32 im2col and F32GemmB vs the CPU conv1d at `approx` (2e-3): f32 over K <= 128 reassociated
def private tts_im2col_arm(t : T?; var cp : auto(C); tag : string; g : ConvGeom) {
    cp |> cell_clear()
    var inscope c <- make_conv(g, 0.004, 0.19)
    let tout = int(conv1d_out_len(c, int64(g.tin)))
    t |> equal(tout, g.tout, "{tag}: the output length")
    let kdim = (g.k * g.cin + 31) / 32 * 32
    var inscope x <- row_fill(g.cin * g.tin, 41, 7, 0.05, 2.1)
    var inscope x_rows : array<float>
    if (g.rows_x) {
        transpose_ct(x, int64(g.cin), int64(g.tin), x_rows)
    }
    var inscope y_ref <- conv_ref(c, x, g.tin)
    var inscope w <- slab_rows(c, kdim)
    let ny = tout * g.cout
    let xp = cp |> in_(g.rows_x ? x_rows : x, g.xoff)
    let colp = cp |> out_(tout * kdim, g.coloff)
    let wp = cp |> in_(w)
    let bp = cp |> in_(c.b)
    let yp = cp |> out_(ny)
    var pc = GkIm2col1dArgs(cin = uint(g.cin), tin = uint(g.tin), tout = uint(tout), k = uint(g.k), stride = uint(g.stride),
        pad_l = uint(g.pad), dilation = 1u, transposed = g.transposed ? 1u : 0u, kdim = uint(kdim), xoff = uint(g.xoff),
        coloff = uint(g.coloff), row_major = g.rows_x ? 1u : 0u, r0 = 0u, mp = uint(tout))
    var pm = F32GemmArgs(n = uint(kdim), d = uint(g.cout), rows = uint(tout), woff = 0u, wstride = uint(kdim), boff = 0u,
        xoff = uint(g.coloff), xstride = uint(kdim), yoff = 0u, ystride = uint(g.cout))
    cp |> cell_launch(t, tag) $(var rl) {
        enc_tts_im2col_cls(rl, seats(fixed_array(xp, colp)), pc, int64(tout * kdim))
        enc_f32_gemm_b_cls(rl, seats(fixed_array(wp, colp, yp, bp)), pm, f32_gemm_wgs(int64(tout), int64(g.cout)))
    }
    check_rows(t, "{tag} im2col + F32GemmB over {tout} x {g.cout} vs the CPU conv1d", cp |> plane_run(yp), y_ref, BAR_APPROX, g.cout)
}

//! the f32 im2col through F32GemmB vs the CPU conv1d: forward k5, transposed k4 s2, row-major x
def tts_im2col_case(t : T?; var cp : auto(C)) {
    tts_im2col_arm(t, cp, "im2col forward k5 cin22", ConvGeom(cin = 22, cout = 40, k = 5, stride = 1, pad = 2, tin = 37, tout = 37,
        xoff = 0, coloff = 96, transposed = false))
    tts_im2col_arm(t, cp, "im2col transposed k4 s2 cin16", ConvGeom(cin = 16, cout = 24, k = 4, stride = 2, pad = 1, tin = 19, tout = 38,
        xoff = 13, coloff = 0, transposed = true))
    tts_im2col_arm(t, cp, "im2col forward k5 cin22 rows x", ConvGeom(cin = 22, cout = 40, k = 5, stride = 1, pad = 2, tin = 37, tout = 37,
        xoff = 5, coloff = 0, transposed = false, rows_x = true))
}

let private FOLD_LOFF = 7     //! the lift rows' element base, sentinel slots before it
let private FOLD_OOFF = 5     //! ... and the output rows'

//! the fold over a transposed k4 s2 conv's lift rows, plus the bias, vs the CPU `conv1d`
def tts_fold_case(t : T?; var cp : auto(C)) {
    cp |> cell_clear()
    let tag = "tts_fold transposed k4 s2 cin16"
    let g = ConvGeom(cin = 16, cout = 24, k = 4, stride = 2, pad = 1, tin = 19, tout = 38, xoff = 0, coloff = 0, transposed = true)
    var inscope c <- make_conv(g, 0.004, 0.19)
    let tout = int(conv1d_out_len(c, int64(g.tin)))
    t |> equal(tout, g.tout, "{tag}: the output length")
    var inscope x <- row_fill(g.cin * g.tin, 41, 7, 0.05, 2.1)
    let lw = g.k * g.cout
    var inscope lift : array<float>
    lift |> resize(g.tin * lw)
    for (ti in range(g.tin)) {
        for (tap in range(g.k)) {
            for (co in range(g.cout)) {
                var acc = 0.0
                for (ci in range(g.cin)) {
                    acc += x[ci * g.tin + ti] * c.w[(ci * g.cout + co) * g.k + tap]
                }
                lift[ti * lw + tap * g.cout + co] = acc
            }
        }
    }
    var inscope bias := c.b
    var inscope y_ref <- conv_ref(c, x, g.tin)
    let ny = tout * g.cout
    let lp = cp |> in_(lift, FOLD_LOFF)
    let op = cp |> out_(ny, FOLD_OOFF)
    var pa = TtsFoldArgs(tin = uint(g.tin), tout = uint(tout), cout = uint(g.cout), k = uint(g.k), stride = uint(g.stride), pad_l = uint(g.pad),
        dilation = 1u, loff = uint(FOLD_LOFF), lstride = uint(lw), ooff = uint(FOLD_OOFF), ostride = uint(g.cout))
    cp |> cell_launch(t, tag) $(var rl) {
        enc_tts_fold_cls(rl, seats(fixed_array(lp, op)), pa, int64(ny))
    }
    var inscope got <- cp |> plane_run(op)
    for (to in range(tout)) {
        for (co in range(g.cout)) {
            got[to * g.cout + co] += bias[co]
        }
    }
    check_rows(t, "{tag}: the fold of x times the lift, plus the bias, over {tout} x {g.cout} vs the CPU conv1d", got, y_ref, BAR_APPROX, g.cout)
    var inscope whole <- cp |> plane_whole(op)
    t |> equal(nan_count(whole), FOLD_OOFF, "{tag}: the prefix before the output rows keeps its sentinel and every output element is stored")
}

let private LSTM_T = 23          //! the steps: off every power of two
let private LSTM_NIN = 48        //! the input width
let private LSTM_HH_GAP = 16     //! poisoned slots between the two directions' w_hh planes, so the backward plane sits at an offset

//! one direction's weights off `row_fill`, centred at zero and scaled by 1/sqrt(width) so the gates stay off saturation
def private make_lstm(hd, salt : int) : TtsLstm {
    var l = TtsLstm(nin = int64(LSTM_NIN), hidden = int64(hd))
    let sih = 0.5 / (48.0 * sqrt(float(LSTM_NIN)))
    let shh = 0.5 / (48.0 * sqrt(float(hd)))
    l.w_ih <- row_fill(4 * hd * LSTM_NIN, 37, 5 + salt, sih, 48.0 * sih)
    l.w_hh <- row_fill(4 * hd * hd, 53, 11 + salt, shh, 48.0 * shh)
    l.b <- row_fill(4 * hd, 29, 3 + salt, 0.006, 0.288)
    return <- l
}

//! the input gates x w_ih^T + b on the CPU `linear_rows`: [t][4H] at `gstride`, the pad columns poisoned
def private gates_ref(l : TtsLstm; x : array<float>; gstride : int) : array<float> {
    var lin = TtsLinear(nin = l.nin, nout = 4l * l.hidden)
    lin.w := l.w_ih
    lin.b := l.b
    var g : array<float>
    with_dasllama_jobque_() {
        linear_rows(lin, x, int64(LSTM_T), g)
    }
    let g4 = 4 * int(l.hidden)
    var rows : array<float>
    rows |> resize(LSTM_T * gstride)
    for (r in range(LSTM_T)) {
        for (o in range(gstride)) {
            rows[r * gstride + o] = o < g4 ? g[r * g4 + o] : POISON
        }
    }
    delete g
    delete lin
    return <- rows
}

//! the CPU `bilstm` over x [t][nin] with `fwd` and `bwd` as its two directions: [t][2H], the forward half first
def private bilstm_ref(fwd, bwd : TtsLstm; x : array<float>) : array<float> {
    var bl = TtsBiLstm()
    bl.fwd := fwd
    bl.bwd := bwd
    var y : array<float>
    with_dasllama_jobque_() {
        bilstm(bl, x, int64(LSTM_T), y)
    }
    delete bl
    return <- y
}

//! half `half` (0 forward, 1 backward) of [t][2H] rows: [t][H]
def private half_rows(y : array<float>; hd, half : int) : array<float> {
    var o : array<float>
    o |> resize(LSTM_T * hd)
    for (r in range(LSTM_T)) {
        for (j in range(hd)) {
            o[r * hd + j] = y[r * 2 * hd + half * hd + j]
        }
    }
    return <- o
}

struct private LstmFixture {
    x : array<float>
    gates : array<float>    //! the forward direction's gate rows, then the backward's at `LSTM_T * gstride`
    whh : array<float>      //! the forward w_hh transposed [H][4H], `LSTM_HH_GAP` poisoned slots, then the backward's
    y_ref : array<float>    //! the CPU bilstm: [t][2H]
    yc_ref : array<float>   //! the forward weights walked backward: [t][H], the CPU bilstm's second half over (fwd, fwd)
    gstride : int
    bwd_hh : int            //! the backward w_hh's element base
}

def private lstm_fixture(hd : int) : LstmFixture {
    var f = LstmFixture(gstride = (4 * hd + 63) / 64 * 64)
    var lf <- make_lstm(hd, 0)
    var lb <- make_lstm(hd, 19)
    f.x <- row_fill(LSTM_T * LSTM_NIN, 41, 7, 0.03, 1.44)
    f.gates <- gates_ref(lf, f.x, f.gstride)
    var gb <- gates_ref(lb, f.x, f.gstride)
    f.gates |> push_from(gb)
    var bwd_whh : array<float>
    transpose_ct(lf.w_hh, int64(4 * hd), int64(hd), f.whh)   // each direction's w_hh transposed [H][4H], as the slab lays it
    transpose_ct(lb.w_hh, int64(4 * hd), int64(hd), bwd_whh)
    f.bwd_hh = put_run(f.whh, bwd_whh, LSTM_HH_GAP)
    f.y_ref <- bilstm_ref(lf, lb, f.x)
    var yff <- bilstm_ref(lf, lf, f.x)
    f.yc_ref <- half_rows(yff, hd, 1)
    delete gb
    delete bwd_whh
    delete yff
    delete lf
    delete lb
    return <- f
}

//! the LSTM direction's bar: 23 steps of four gate dots of `hd` terms, sigmoids and tanh (read: Metal and Vulkan 8.9e-8, CUDA
//! 2.2e-6 - its fast-math exp and tanh)
let private LSTM_BARS = home_bars(Bar(rel = 1e-4, abs_floor = 1e-5, poison_scaled = true))

//! one LSTM direction at hidden `hd`: both directions into [t][2H], then forward walked backward
def tts_lstm_case(t : T?; var cp : auto(C); hd : int) {
    cp |> cell_clear()
    let tag = "lstm h{hd}"
    var inscope f <- lstm_fixture(hd)
    let gp = cp |> in_(f.gates)
    let wp = cp |> in_(f.whh)
    let yp = cp |> out_(LSTM_T * 2 * hd)
    let ycp = cp |> out_(LSTM_T * hd)
    var pf = TtsLstmArgs(t = uint(LSTM_T), hidden = uint(hd), goff = 0u, gstride = uint(f.gstride), hhoff = 0u, hoff = 0u,
        hstride = uint(2 * hd), backward = 0u)
    var pb = TtsLstmArgs(t = uint(LSTM_T), hidden = uint(hd), goff = uint(LSTM_T * f.gstride), gstride = uint(f.gstride),
        hhoff = uint(f.bwd_hh), hoff = uint(hd), hstride = uint(2 * hd), backward = 1u)
    var pc = TtsLstmArgs(t = uint(LSTM_T), hidden = uint(hd), goff = 0u, gstride = uint(f.gstride), hhoff = 0u, hoff = 0u,
        hstride = uint(hd), backward = 1u)
    cp |> cell_launch(t, tag) $(var rl) {
        enc_tts_lstm_dir_cls(rl, seats(fixed_array(gp, wp, yp)), pf, 1l)
        enc_tts_lstm_dir_cls(rl, seats(fixed_array(gp, wp, yp)), pb, 1l)
        enc_tts_lstm_dir_cls(rl, seats(fixed_array(gp, wp, ycp)), pc, 1l)
    }
    var inscope y_dev <- cp |> plane_run(yp)
    var inscope yc_dev <- cp |> plane_run(ycp)
    let bar = cp |> home_bar(LSTM_BARS)
    check_rows(t, "{tag} both directions over {LSTM_T} steps vs the CPU bilstm", y_dev, f.y_ref,
        bar, 2 * hd, hd)
    check_rows(t, "{tag} the forward weights walked backward vs the CPU's backward walk", yc_dev,
        f.yc_ref, bar, hd)
    let moved = logits_maxdiff(half_rows(y_dev, hd, 0), yc_dev)
    t |> success(moved > 0.01, "{tag} control: flipping `backward` on the same gates moves the output (maxdiff {moved})")
}

let private PD_C = 22
let private PD_K = 3
let private PD_TIN = 19
let private PD_TOUT = 38     //! (19 - 1) * 2 - 2 + 2 + 1 + 1: the output pad's last row reaches past t_in on its first tap
let private PD_WOFF = 5      //! the slab's weight rows' element base, poisoned slots before it
let private PD_GAP = 3       //! poisoned slots between the weight rows and the bias row

//! the pool's bar: at most two taps a channel plus the bias, at f32 (read: 1.9e-7 on every home)
let private POOL_BARS = home_bars(Bar(rel = 1e-5, abs_floor = 1e-6, poison_scaled = true))

//! the pool's slab plane: w [c][k] at PD_WOFF, the bias row after PD_GAP poisoned slots, four poisoned slots past it
def private pool_slab(c : TtsConv1d) : array<float> {
    var wn : array<float>
    put_run(wn, c.w, PD_WOFF)
    put_run(wn, c.b, PD_GAP)
    wn |> push_from(poison_plane(4))
    return <- wn
}

//! the depthwise transposed pool k3 s2 p1 with output padding against the CPU rows form, the output under a NaN fill first
def tts_pool_dw_case(t : T?; var cp : auto(C)) {
    cp |> cell_clear()
    let tag = "tts_pool_dw k3 s2 c22"
    var inscope c = TtsConv1d(cin = int64(PD_C), cout = int64(PD_C), k = int64(PD_K), stride = 2l, pad_l = 1l, pad_r = 1l, dilation = 1l,
        groups = int64(PD_C), output_padding = 1l, transposed = true)
    c.w <- row_fill(PD_C * PD_K, 37, 5, 0.02, 0.9)
    c.b <- row_fill(PD_C, 29, 11, 0.02, 0.9)
    let tout = int(conv1d_out_len(c, int64(PD_TIN)))
    t |> equal(tout, PD_TOUT, "{tag}: the output length")
    var inscope x <- row_fill(PD_TIN * PD_C, 41, 7, 0.05, 2.1)
    var inscope want : array<float>
    with_dasllama_jobque_() {
        conv1d_rows_transposed_depthwise(c, x, int64(PD_TIN), want)
    }
    var inscope wn <- pool_slab(c)
    let ny = tout * PD_C
    let xp = cp |> in_(x)
    let yp = cp |> out_(ny)
    let wp = cp |> in_(wn)
    var pa = GkDwConvRowsArgs(c = uint(PD_C), k = uint(PD_K), stride = 2u, pad_l = 1u, dil = 1u, t_in = uint(PD_TIN), t_out = uint(tout),
        woff = uint(PD_WOFF), boff = uint(PD_WOFF + PD_C * PD_K + PD_GAP))
    cp |> cell_launch(t, tag) $(var rl) {
        enc_tts_pool_dw_cls(rl, seats(fixed_array(xp, yp, wp)), pa, int64(ny))
    }
    check_rows(t, "{tag} over {tout} x {PD_C} vs the CPU conv1d_rows_transposed_depthwise",
        cp |> plane_run(yp), want,
        cp |> home_bar(POOL_BARS), PD_C)
}
