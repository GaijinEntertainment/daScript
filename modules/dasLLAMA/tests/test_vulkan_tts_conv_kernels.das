options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module test_vulkan_tts_conv_kernels

require dastest/testing_boost public
require math
require ?vulkan vulkan
require ?vulkan dasllama/dasllama_vulkan_common
require ?vulkan dasllama/dasllama_vulkan_classes
require ?vulkan _vkd_oracles
require _compares   // nolint:STYLE030,LINT019 — the compares, inside the vulkan arms alone
require dasllama/dasllama_convert   // nolint:STYLE030,LINT019 — cvt_f32_to_f16_sat, inside the vulkan arms alone
require dasllama/dasllama_tts_blocks
require dasllama/dasllama_math
require daslib/jobque_boost

//! The TTS tower's sequence classes on Vulkan against the CPU chain (a Vulkan device, else the cells skip; the half
//! im2col cell also skips where the device serves no f16 tile): the conv im2col in both directions, fed through
//! F32GemmB and held to the CPU `conv1d` end to end, the half im2col through the f16 tile and the bias pass, one LSTM
//! direction held to the CPU `bilstm` on both walks, and the depthwise transposed pool held to its CPU rows form, at
//! channel and hidden widths off every lattice, with the poisoned-element control on every compare.


struct private ConvGeom {
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
def private make_conv(g : ConvGeom; wscale, woff : float) : TtsConv1d {
    var c = TtsConv1d(cin = int64(g.cin), cout = int64(g.cout), k = int64(g.k), stride = int64(g.stride), pad_l = int64(g.pad),
        pad_r = int64(g.pad), dilation = 1l, groups = 1l, transposed = g.transposed)
    c.w <- row_fill(g.cin * g.cout * g.k, 37, 5, wscale, woff)
    c.b <- row_fill(g.cout, 29, 11, 0.02, 0.9)
    return <- c
}

//! the prepared conv's weights as `tts_slab_conv` lays them, the columns past k * cin poisoned: the im2col's zero columns keep them out
def private slab_rows(c : TtsConv1d; kdim : int) : array<float> {
    var w <- [for (_i in range(int(c.cout) * kdim)); POISON]
    for (co in range64(c.cout)) {
        conv1d_weight_row(c, co, w, co * int64(kdim))
    }
    return <- w
}

//! the CPU `conv1d` over x [cin][tin] (the conv prepared as the loader prepares it), transposed to the GEMM's [tout][cout] rows
def private conv_ref(var c : TtsConv1d; x : array<float>; tin : int) : array<float> {
    conv1d_prepare(c)
    var y_ct : array<float>
    var y : array<float>
    with_dasllama_jobque_() {
        conv1d(c, x, int64(tin), y_ct)
    }
    transpose_ct(y_ct, c.cout, conv1d_out_len(c, int64(tin)), y)
    delete y_ct
    return <- y
}

let private COL16_SLACK_ROWS = 64   //! the half column plane's rows past the conv's: the KHR tile loads whole 16-row blocks
let private F16_LATTICE = 1.0 / 64.0   //! the half route's operand step: sixty-fourths within F16_LATTICE_OFF of zero, every product exact on the f16 tile
let private F16_LATTICE_OFF = 0.75     //! the half route's operand offset: `row_fill`'s sixty-fourths less it span [-0.75, 0.75]

//! one conv's im2col and GEMM vs the CPU conv1d at `approx` (2e-3): `on_f16_lattice` the f16 route on exact products, else f32 over K <= 128 reassociated
def private im2col_arm(t : T?; tag : string; g : ConvGeom; on_f16_lattice : bool) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let khr = g_gpu.coopmat_mode != COOPMAT_CM2   // the pick the driver makes where an f16 tile serves at all (the half cell skipped otherwise)
        var c <- make_conv(g, on_f16_lattice ? F16_LATTICE : 0.004, on_f16_lattice ? F16_LATTICE_OFF : 0.19)
        let tout = int(conv1d_out_len(c, int64(g.tin)))
        t |> equal(tout, g.tout, "{tag}: the output length")
        let kdim = (g.k * g.cin + 31) / 32 * 32
        var x <- row_fill(g.cin * g.tin, 41, 7, on_f16_lattice ? F16_LATTICE : 0.05, on_f16_lattice ? F16_LATTICE_OFF : 2.1)
        var x_rows : array<float>
        if (g.rows_x) {
            transpose_ct(x, int64(g.cin), int64(g.tin), x_rows)
        }
        var y_ref <- conv_ref(c, x, g.tin)
        var w <- slab_rows(c, kdim)
        var w16 : array<uint16>
        if (on_f16_lattice) {
            w16 |> resize(length(w))
            unsafe {
                cvt_f32_to_f16_sat(addr(w16[0]), addr(w[0]), long_length(w))
            }
        }
        let ny = tout * g.cout
        with_cell_planes() $(var cp : CellPlanes) {
            let xp = cp |> in_(g.rows_x ? x_rows : x, g.xoff)
            let colp = on_f16_lattice ? cp |> out_((tout + COL16_SLACK_ROWS) * kdim / 2) : cp |> out_(tout * kdim, g.coloff)   // the half plane: two halfwords a float slot
            let wp = on_f16_lattice ? cp |> in_plain(w16) : cp |> in_(w)
            let bp = cp |> in_(c.b)
            let yp = cp |> out_(ny)
            var pc = TtsIm2colArgs(cin = uint(g.cin), tin = uint(g.tin), tout = uint(tout), k = uint(g.k), stride = uint(g.stride),
                pad_l = uint(g.pad), dilation = 1u, transposed = g.transposed ? 1u : 0u, kdim = uint(kdim), xoff = uint(g.xoff),
                coloff = uint(g.coloff), rows = g.rows_x ? 1u : 0u)
            if (on_f16_lattice) {
                var s_col = cp |> cls_set(fixed_array(xp, colp), @@set_tts_im2col16_cls)
                let mm = fixed_array(wp, colp, yp)
                var s_mm = f16_gemm_set(khr, gather_planes(cp.dev, mm), gather_planes(cp.bytes, mm), gather_planes(cp.hazard_bit, mm))
                var s_b = cp |> cls_set(fixed_array(yp, bp), @@set_tower_bias_act_cls)
                var pm = F16GemmArgs(n = uint(kdim), d = uint(g.cout), rows = uint(tout), woff = 0u, ybase = 0u, ystride = uint(g.cout), ksplit = 0u)
                var pb = TowerBiasArgs(d = uint(g.cout), nelem = uint(ny), boff = 0u, act = BIAS_ACT_NONE)
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_tts_im2col16_cls(raw, h, s_col, pc, int64(tout * kdim))
                    f16_gemm_enc(khr, raw, h, s_mm, pm, int64(tout), int64(g.cout))
                    enc_tower_bias_act_cls(raw, h, s_b, pb, (int64(ny) / 4l + 255l) / 256l)
                }
            } else {
                var s_col = cp |> cls_set(fixed_array(xp, colp), @@set_tts_im2col_cls)
                var s_mm = cp |> cls_set(fixed_array(wp, colp, yp, bp), @@set_f32_gemm_b_cls)
                var pm = F32GemmArgs(n = uint(kdim), d = uint(g.cout), rows = uint(tout), woff = 0u, wstride = uint(kdim), boff = 0u,
                    xoff = uint(g.coloff), xstride = uint(kdim), yoff = 0u, ystride = uint(g.cout))
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_tts_im2col_cls(raw, h, s_col, pc, int64(tout * kdim))
                    enc_f32_gemm_b_cls(raw, h, s_mm, pm, f32_gemm_wgs(int64(tout), int64(g.cout)))
                }
            }
            let route = on_f16_lattice ? "im2col16 + the {khr ? "KHR" : "cm2"} f16 tile + the bias" : "im2col + F32GemmB"
            check_rows(t, "{tag} {route} over {tout} x {g.cout} vs the CPU conv1d", cp |> plane_run(yp), y_ref, BAR_APPROX, g.cout)
        }
        delete c
        delete w
        delete w16
        delete x
        delete x_rows
        delete y_ref
    }
}

[test]
def test_vkt_tts_im2col16(t0 : T?) {
    t0 |> run("the f16 tile: forward k5 s1 p2 over 22 channels into 40, transposed k4 s2 p1 over 16 into 32, forward k3 over 32 into 64 (a 96-wide row)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || (g_gpu.coopmat_mode != COOPMAT_CM2 && !khr_kq_tile_on())) {
                t |> skip("no Vulkan device with an f16 tile (neither cm2 nor the KHR tile at subgroup 32)")
                return
            }
            if (!ensure_tts_im2col16_cls() || !ensure_tower_bias_act_cls() || !f16_gemm_ensure(g_gpu.coopmat_mode != COOPMAT_CM2)) {
                t |> skip("the f16 tile's classes did not build")
                return
            }
            im2col_arm(t, "im2col16 forward k5 cin22", ConvGeom(cin = 22, cout = 40, k = 5, stride = 1, pad = 2, tin = 37, tout = 37,
                xoff = 0, coloff = 0, transposed = false), true)
            im2col_arm(t, "im2col16 transposed k4 s2 cin16", ConvGeom(cin = 16, cout = 32, k = 4, stride = 2, pad = 1, tin = 19, tout = 38,
                xoff = 5, coloff = 0, transposed = true), true)
            im2col_arm(t, "im2col16 forward k3 cin32 rows", ConvGeom(cin = 32, cout = 64, k = 3, stride = 1, pad = 1, tin = 50, tout = 50,
                xoff = 0, coloff = 0, transposed = false, rows_x = true), true)
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkt_tts_im2col(t0 : T?) {
    t0 |> run("forward k5 s1 p2 over 22 channels: im2col + F32GemmB == the CPU conv1d") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tts_im2col_cls() || !ensure_f32_gemm_b_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            im2col_arm(t, "im2col forward k5 cin22", ConvGeom(cin = 22, cout = 40, k = 5, stride = 1, pad = 2, tin = 37, tout = 37,
                xoff = 0, coloff = 96, transposed = false), false)
        } else {
            t |> skip("dasVulkan not present")
        }
    }
    t0 |> run("transposed k4 s2 p1 over 16 channels: im2col + F32GemmB == the CPU conv1d") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tts_im2col_cls() || !ensure_f32_gemm_b_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            im2col_arm(t, "im2col transposed k4 s2 cin16", ConvGeom(cin = 16, cout = 24, k = 4, stride = 2, pad = 1, tin = 19, tout = 38,
                xoff = 13, coloff = 0, transposed = true), false)
        } else {
            t |> skip("dasVulkan not present")
        }
    }
    t0 |> run("forward k5 s1 p2 over 22 channels off row-major x: im2col + F32GemmB == the CPU conv1d") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tts_im2col_cls() || !ensure_f32_gemm_b_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            im2col_arm(t, "im2col forward k5 cin22 rows x", ConvGeom(cin = 22, cout = 40, k = 5, stride = 1, pad = 2, tin = 37, tout = 37,
                xoff = 5, coloff = 0, transposed = false, rows_x = true), false)
        } else {
            t |> skip("dasVulkan not present")
        }
    }
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

//! both directions into one [t][2H] row set, then the forward weights walked backward: every output under a NaN fill first
def private lstm_arm(t : T?; hd : int) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let tag = "lstm h{hd}"
        var f <- lstm_fixture(hd)
        with_cell_planes() $(var cp : CellPlanes) {
            let gp = cp |> in_(f.gates)
            let wp = cp |> in_(f.whh)
            let yp = cp |> out_(LSTM_T * 2 * hd)
            let ycp = cp |> out_(LSTM_T * hd)
            var s_y = cp |> cls_set(fixed_array(gp, wp, yp), @@set_tts_lstm_dir_cls)
            var s_yc = cp |> cls_set(fixed_array(gp, wp, ycp), @@set_tts_lstm_dir_cls)
            var pf = TtsLstmArgs(t = uint(LSTM_T), hidden = uint(hd), goff = 0u, gstride = uint(f.gstride), hhoff = 0u, hoff = 0u,
                hstride = uint(2 * hd), backward = 0u)
            var pb = TtsLstmArgs(t = uint(LSTM_T), hidden = uint(hd), goff = uint(LSTM_T * f.gstride), gstride = uint(f.gstride),
                hhoff = uint(f.bwd_hh), hoff = uint(hd), hstride = uint(2 * hd), backward = 1u)
            var pc = TtsLstmArgs(t = uint(LSTM_T), hidden = uint(hd), goff = 0u, gstride = uint(f.gstride), hhoff = 0u, hoff = 0u,
                hstride = uint(hd), backward = 1u)
            cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_tts_lstm_dir_cls(raw, h, s_y, pf, 1l)
                enc_tts_lstm_dir_cls(raw, h, s_y, pb, 1l)
                enc_tts_lstm_dir_cls(raw, h, s_yc, pc, 1l)
            }
            let y_dev <- cp |> plane_run(yp)
            let yc_dev <- cp |> plane_run(ycp)
            // `approx`'s 2e-3: both sides f32, the CPU matmul reassociating the h dot and adding the bias after it, over 23 steps
            check_rows(t, "{tag} both directions over {LSTM_T} steps vs the CPU bilstm", y_dev, f.y_ref, BAR_APPROX, 2 * hd, hd)
            check_rows(t, "{tag} the forward weights walked backward vs the CPU's backward walk", yc_dev, f.yc_ref, BAR_APPROX, hd)
            let moved = logits_maxdiff(half_rows(y_dev, hd, 0), yc_dev)
            t |> success(moved > 0.01, "{tag} control: flipping `backward` on the same gates moves the output (maxdiff {moved})")
        }
        delete f
    }
}

[test]
def test_vkt_tts_lstm_dir(t0 : T?) {
    for (hd in fixed_array(64, 200)) {
        t0 |> run("one LSTM direction at hidden {hd}, both walks == the CPU bilstm") <| @(t : T?) {
            static_if (typeinfo builtin_module_exists(vulkan)) {
                if (!vk_moe_init() || !ensure_tts_lstm_dir_cls()) {
                    t |> skip("no Vulkan device")
                    return
                }
                lstm_arm(t, hd)
            } else {
                t |> skip("dasVulkan not present")
            }
        }
    }
}

let private PD_C = 22
let private PD_K = 3
let private PD_TIN = 19
let private PD_TOUT = 38     //! (19 - 1) * 2 - 2 + 2 + 1 + 1: the output pad's last row reaches past t_in on its first tap
let private PD_WOFF = 5      //! the slab's weight rows' element base, poisoned slots before it
let private PD_GAP = 3       //! poisoned slots between the weight rows and the bias row

//! the pool's slab plane: w [c][k] at PD_WOFF, the bias row after PD_GAP poisoned slots, four poisoned slots past it
def private pool_slab(c : TtsConv1d) : array<float> {
    var wn : array<float>
    put_run(wn, c.w, PD_WOFF)
    put_run(wn, c.b, PD_GAP)
    wn |> push_from(poison_plane(4))
    return <- wn
}

//! the depthwise transposed pool against the CPU rows form: the output under a NaN fill first
def private pool_dw_arm(t : T?) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let tag = "tts_pool_dw k3 s2 c22"
        var c = TtsConv1d(cin = int64(PD_C), cout = int64(PD_C), k = int64(PD_K), stride = 2l, pad_l = 1l, pad_r = 1l, dilation = 1l,
            groups = int64(PD_C), output_padding = 1l, transposed = true)
        c.w <- row_fill(PD_C * PD_K, 37, 5, 0.02, 0.9)
        c.b <- row_fill(PD_C, 29, 11, 0.02, 0.9)
        let tout = int(conv1d_out_len(c, int64(PD_TIN)))
        t |> equal(tout, PD_TOUT, "{tag}: the output length")
        var x <- row_fill(PD_TIN * PD_C, 41, 7, 0.05, 2.1)
        var want : array<float>
        with_dasllama_jobque_() {
            conv1d_rows_transposed_depthwise(c, x, int64(PD_TIN), want)
        }
        var wn <- pool_slab(c)
        let ny = tout * PD_C
        with_cell_planes() $(var cp : CellPlanes) {
            let xp = cp |> in_(x)
            let yp = cp |> out_(ny)
            let wp = cp |> in_(wn)
            var s = cp |> cls_set(fixed_array(xp, yp, wp), @@set_tts_pool_dw_cls)
            var pa = DwConvRowsArgs(c = uint(PD_C), k = uint(PD_K), stride = 2u, pad_l = 1u, dil = 1u, t_in = uint(PD_TIN), t_out = uint(tout),
                woff = uint(PD_WOFF), boff = uint(PD_WOFF + PD_C * PD_K + PD_GAP))
            cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_tts_pool_dw_cls(raw, h, s, pa, int64(ny))
            }
            // `approx`'s 2e-3: both sides f32 over at most two taps a channel
            check_rows(t, "{tag} over {tout} x {PD_C} vs the CPU conv1d_rows_transposed_depthwise", cp |> plane_run(yp), want, BAR_APPROX, PD_C)
        }
        delete c
        delete x
        delete want
        delete wn
    }
}

[test]
def test_vkt_tts_pool_dw(t0 : T?) {
    t0 |> run("the depthwise transposed pool k3 s2 p1 over 22 channels == the CPU conv1d_rows_transposed_depthwise") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tts_pool_dw_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            pool_dw_arm(t)
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}
