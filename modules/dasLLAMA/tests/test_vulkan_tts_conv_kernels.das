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
require dasllama/dasllama_convert   // the f16 image of the weight rows
require dasllama/dasllama_tts_blocks
require dasllama/dasllama_math   // setup_dasllama_jobque_
require daslib/jobque_boost

//! The TTS tower's sequence classes on Vulkan against the CPU chain (a Vulkan device, else the cells skip): the conv
//! im2col in both directions, fed through F32GemmB and held to the CPU `conv1d` end to end, and one LSTM direction
//! held to the CPU `bilstm` on both walks, and the depthwise transposed pool held to its CPU rows form, at channel and
//! hidden widths off every lattice, with the poisoned-element control on every compare.

let private POISON = 7.5   //! the pads a kernel must never read: a large finite value, so a stray read lands in the sum

def private row_fill(n : int; mul, add : int; scale, off : float) : array<float> {
    var a : array<float>
    a |> resize(n)
    for (i in range(n)) {
        a[i] = float((i * mul + add) % 97) * scale - off
    }
    return <- a
}

//! logs the widest |got - want| with the row and the column it sits at, rows `width` wide
def private log_maxdiff(tag : string; got, want : array<float>; width : int) {
    var worst = 0.0
    var at = 0
    for (i in range(length(got))) {
        let d = abs(got[i] - want[i])
        if (d > worst) {
            worst = d
            at = i
        }
    }
    to_log(LOG_INFO, "{tag}: max |gpu - cpu| {worst} at row {at / width} column {at % width}\n")
}

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

//! a raw conv - the ONNX weight ([cout][cin][k] forward, [cin][cout][k] transposed) and the bias off `row_fill`
def private make_conv(g : ConvGeom) : TtsConv1d {
    var c = TtsConv1d(cin = int64(g.cin), cout = int64(g.cout), k = int64(g.k), stride = int64(g.stride), pad_l = int64(g.pad),
        pad_r = int64(g.pad), dilation = 1l, groups = 1l, transposed = g.transposed)
    c.w <- row_fill(g.cin * g.cout * g.k, 37, 5, 0.004, 0.19)
    c.b <- row_fill(g.cout, 29, 11, 0.02, 0.9)
    return <- c
}

//! the conv's weights as `st2_slab_conv` lays them: row co [kdim], column j = tap * cin + ci, the columns past k * cin
//! poisoned - the im2col's zero columns keep them out of the sum
def private slab_rows(c : TtsConv1d; kdim : int) : array<float> {
    let cin = int(c.cin)
    let cout = int(c.cout)
    let k = int(c.k)
    var w : array<float>
    w |> resize(cout * kdim)
    for (co in range(cout)) {
        for (j in range(k * cin, kdim)) {
            w[co * kdim + j] = POISON
        }
        for (ci in range(cin)) {
            for (tap in range(k)) {
                let src = c.transposed ? (ci * cout + co) * k + tap : (co * cin + ci) * k + tap
                w[co * kdim + tap * cin + ci] = c.w[src]
            }
        }
    }
    return <- w
}

//! the CPU `conv1d` over x [cin][tin] (the conv prepared as the loader prepares it), transposed to the GEMM's [tout][cout] rows
def private conv_ref(var c : TtsConv1d; x : array<float>; tin : int) : array<float> {
    conv1d_prepare(c)
    var y_ct : array<float>
    var y : array<float>
    with_job_que() {
        setup_dasllama_jobque_()
        conv1d(c, x, int64(tin), y_ct)
    }
    transpose_ct(y_ct, c.cout, conv1d_out_len(c, int64(tin)), y)
    delete y_ct
    return <- y
}

//! x [cin][tin] at `xoff` in the device plane, the slots under it poisoned
def private at_offset(x : array<float>; xoff : int) : array<float> {
    var a : array<float>
    a |> resize(xoff + length(x))
    for (i in range(xoff)) {
        a[i] = POISON
    }
    for (i in range(length(x))) {
        a[xoff + i] = x[i]
    }
    return <- a
}

//! one conv through TtsIm2col + F32GemmB against the CPU conv1d: the columns and the output under a NaN fill first
def private im2col_arm(t : T?; tag : string; g : ConvGeom) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let B_COL = 1u
        let B_Y = 2u
        var c <- make_conv(g)
        let tout = int(conv1d_out_len(c, int64(g.tin)))
        t |> equal(tout, g.tout, "{tag}: the output length")
        let kdim = (g.k * g.cin + 31) / 32 * 32
        var w <- slab_rows(c, kdim)
        var x <- row_fill(g.cin * g.tin, 41, 7, 0.05, 2.1)
        var x_rows : array<float>
        if (g.rows_x) {
            transpose_ct(x, int64(g.cin), int64(g.tin), x_rows)
        }
        var xdev <- at_offset(g.rows_x ? x_rows : x, g.xoff)
        var y_ref <- conv_ref(c, x, g.tin)
        let ny = tout * g.cout
        let bx = long_length(xdev) * 4l
        let bcol = int64(g.coloff + tout * kdim) * 4l
        let bw = long_length(w) * 4l
        let bb = int64(g.cout) * 4l
        let by = int64(ny) * 4l
        let xd = make_device_buf(bx)
        let cold = make_device_buf(bcol)
        let wd = make_device_buf(bw)
        let bd = make_device_buf(bb)
        let yd = make_device_buf(by)
        var host = make_host_buf(by, true, [cached = true])
        unsafe {
            upload_region_at(xd, 0l, addr<uint8 const?>(xdev[0]), bx)
            upload_region_at(wd, 0l, addr<uint8 const?>(w[0]), bw)
            upload_region_at(bd, 0l, addr<uint8 const?>(c.b[0]), bb)
        }
        nan_sentinel_upload(cold, bcol)
        nan_sentinel_upload(yd, by)
        var s_col = set_tts_im2col_cls(fixed_array(xd, cold), fixed_array(bx, bcol), fixed_array(0u, B_COL))
        var s_mm = set_f32_gemm_b_cls(fixed_array(wd, cold, yd, bd), fixed_array(bw, bcol, by, bb), fixed_array(0u, B_COL, B_Y, 0u))
        var pc = TtsIm2colArgs(cin = uint(g.cin), tin = uint(g.tin), tout = uint(tout), k = uint(g.k), stride = uint(g.stride),
            pad_l = uint(g.pad), dilation = 1u, transposed = g.transposed ? 1u : 0u, kdim = uint(kdim), xoff = uint(g.xoff),
            coloff = uint(g.coloff), rows = g.rows_x ? 1u : 0u)
        var pm = F32GemmArgs(n = uint(kdim), d = uint(g.cout), rows = uint(tout), woff = 0u, wstride = uint(kdim), boff = 0u,
            xoff = uint(g.coloff), xstride = uint(kdim), yoff = 0u, ystride = uint(g.cout))
        cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
            enc_tts_im2col_cls(raw, h, s_col, pc, tts_im2col_wgs(int64(tout * kdim)))
            enc_f32_gemm_b_cls(raw, h, s_mm, pm, f32_gemm_wgs(int64(tout), int64(g.cout)))
            cmd_copy_planes(raw, h, fixed_array(yd), host, fixed_array(by), B_Y)
        }
        var y_dev <- staged(host.mapped, 0l, ny, type<float>)
        // `approx`'s 2e-3: both sides f32 over K <= 128 products, the CPU's matmul_batch reassociating the k-ordered sum
        let bad = mismatch_approx(y_dev, y_ref)
        log_maxdiff(tag, y_dev, y_ref, g.cout)
        t |> success(bad == 0, "{tag}: im2col + F32GemmB == the CPU conv1d over {tout} x {g.cout} ({bad} of {ny} off)")
        t |> success(!approx(y_dev[ny - 1], y_ref[ny - 1] + 0.5), "{tag}: the bar reds a poisoned element")
        destroy_device_buf(xd)
        destroy_device_buf(cold)
        destroy_device_buf(wd)
        destroy_device_buf(bd)
        destroy_device_buf(yd)
        destroy_host_buf(host)
        delete c
        delete w
        delete x
        delete x_rows
        delete xdev
        delete y_ref
        delete y_dev
    }
}

//! the conv with its weights and bias on the f16 lattice (sixty-fourths under 0.75): every product exact on the f16 tile
def private make_conv16(g : ConvGeom) : TtsConv1d {
    var c = TtsConv1d(cin = int64(g.cin), cout = int64(g.cout), k = int64(g.k), stride = int64(g.stride), pad_l = int64(g.pad),
        pad_r = int64(g.pad), dilation = 1l, groups = 1l, transposed = g.transposed)
    c.w <- row_fill(g.cin * g.cout * g.k, 37, 5, 1.0 / 64.0, 0.75)
    c.b <- row_fill(g.cout, 29, 11, 0.02, 0.9)
    return <- c
}

let private COL16_SLACK_ROWS = 64   //! the half column plane's rows past the conv's: the KHR tile loads whole 16-row blocks

//! one conv through TtsIm2col16, the f16 tile the device serves (cm2, else the KHR twin) over the halfword weight rows, and the bias
//! pass, against the CPU conv1d on f16-lattice operands: every product exact, so the bar covers the accumulation alone
def private im2col16_arm(t : T?; tag : string; g : ConvGeom) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let B_COL = 1u
        let B_Y = 2u
        let khr = g_gpu.coopmat_mode != COOPMAT_CM2
        var c <- make_conv16(g)
        let tout = int(conv1d_out_len(c, int64(g.tin)))
        t |> equal(tout, g.tout, "{tag}: the output length")
        let kdim = (g.k * g.cin + 31) / 32 * 32
        var w <- slab_rows(c, kdim)
        var w16 : array<uint16>
        w16 |> resize(length(w))
        unsafe {
            cvt_f32_to_f16_sat(addr(w16[0]), addr(w[0]), long_length(w))
        }
        var x <- row_fill(g.cin * g.tin, 41, 7, 1.0 / 64.0, 0.75)
        var x_rows : array<float>
        if (g.rows_x) {
            transpose_ct(x, int64(g.cin), int64(g.tin), x_rows)
        }
        var xdev <- at_offset(g.rows_x ? x_rows : x, g.xoff)
        var y_ref <- conv_ref(c, x, g.tin)
        let ny = tout * g.cout
        let bx = long_length(xdev) * 4l
        let bcol = int64((tout + COL16_SLACK_ROWS) * kdim) * 2l
        let bw = long_length(w16) * 2l
        let bb = int64(g.cout) * 4l
        let by = int64(ny) * 4l
        let xd = make_device_buf(bx)
        let cold = make_device_buf(bcol)
        let wd = make_device_buf(bw)
        let bd = make_device_buf(bb)
        let yd = make_device_buf(by)
        var host = make_host_buf(by, true, [cached = true])
        unsafe {
            upload_region_at(xd, 0l, addr<uint8 const?>(xdev[0]), bx)
            upload_region_at(wd, 0l, addr<uint8 const?>(w16[0]), bw)
            upload_region_at(bd, 0l, addr<uint8 const?>(c.b[0]), bb)
        }
        nan_sentinel_upload(cold, bcol)
        nan_sentinel_upload(yd, by)
        var s_col = set_tts_im2col16_cls(fixed_array(xd, cold), fixed_array(bx, bcol), fixed_array(0u, B_COL))
        var s_mm = f16_gemm_set(khr, fixed_array(wd, cold, yd), fixed_array(bw, bcol, by), fixed_array(0u, B_COL, B_Y))
        var s_b = set_tower_bias_act_cls(fixed_array(yd, bd), fixed_array(by, bb), fixed_array(B_Y, 0u))
        var pc = TtsIm2colArgs(cin = uint(g.cin), tin = uint(g.tin), tout = uint(tout), k = uint(g.k), stride = uint(g.stride),
            pad_l = uint(g.pad), dilation = 1u, transposed = g.transposed ? 1u : 0u, kdim = uint(kdim), xoff = uint(g.xoff),
            coloff = 0u, rows = g.rows_x ? 1u : 0u)
        var pm = F16GemmArgs(n = uint(kdim), d = uint(g.cout), rows = uint(tout), woff = 0u, ybase = 0u, ystride = uint(g.cout), ksplit = 0u)
        var pb = TowerBiasArgs(d = uint(g.cout), nelem = uint(ny), boff = 0u, act = BIAS_ACT_NONE)
        cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
            enc_tts_im2col16_cls(raw, h, s_col, pc, tts_im2col16_wgs(int64(tout * kdim)))
            f16_gemm_enc(khr, raw, h, s_mm, pm, int64(tout), int64(g.cout))
            enc_tower_bias_act_cls(raw, h, s_b, pb, (int64(ny) / 4l + 255l) / 256l)
            cmd_copy_planes(raw, h, fixed_array(yd), host, fixed_array(by), B_Y)
        }
        var y_dev <- staged(host.mapped, 0l, ny, type<float>)
        let bad = mismatch_approx(y_dev, y_ref)
        log_maxdiff(tag, y_dev, y_ref, g.cout)
        t |> success(bad == 0, "{tag}: im2col16 + the {khr ? "KHR" : "cm2"} f16 tile + the bias == the CPU conv1d over {tout} x {g.cout} ({bad} of {ny} off)")
        t |> success(!approx(y_dev[ny - 1], y_ref[ny - 1] + 0.5), "{tag}: the bar reds a poisoned element")
        destroy_device_buf(xd)
        destroy_device_buf(cold)
        destroy_device_buf(wd)
        destroy_device_buf(bd)
        destroy_device_buf(yd)
        destroy_host_buf(host)
        delete c
        delete w
        delete w16
        delete x
        delete x_rows
        delete xdev
        delete y_ref
        delete y_dev
    }
}

[test]
def test_vkt_tts_im2col16(t0 : T?) {
    t0 |> run("the f16 tile: forward k5 s1 p2 over 22 channels into 40, transposed k4 s2 p1 over 16 into 32, forward k3 over 32 into 64 (a 96-wide row)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tts_im2col16_cls() || !ensure_tower_bias_act_cls() || !f16_gemm_ensure(g_gpu.coopmat_mode != COOPMAT_CM2)) {
                t |> skip("no Vulkan device with an f16 tile")
                return
            }
            im2col16_arm(t, "im2col16 forward k5 cin22", ConvGeom(cin = 22, cout = 40, k = 5, stride = 1, pad = 2, tin = 37, tout = 37,
                xoff = 0, coloff = 0, transposed = false))
            im2col16_arm(t, "im2col16 transposed k4 s2 cin16", ConvGeom(cin = 16, cout = 32, k = 4, stride = 2, pad = 1, tin = 19, tout = 38,
                xoff = 5, coloff = 0, transposed = true))
            im2col16_arm(t, "im2col16 forward k3 cin32 rows", ConvGeom(cin = 32, cout = 64, k = 3, stride = 1, pad = 1, tin = 50, tout = 50,
                xoff = 0, coloff = 0, transposed = false, rows_x = true))
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
                xoff = 0, coloff = 96, transposed = false))
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
                xoff = 13, coloff = 0, transposed = true))
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
                xoff = 5, coloff = 0, transposed = false, rows_x = true))
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

//! the input gates the driver's F32GemmB computes, x w_ih^T + b, on the CPU `linear_rows`: [t][4H] laid at `gstride`,
//! the pad columns poisoned
def private gates_ref(l : TtsLstm; x : array<float>; gstride : int) : array<float> {
    var lin = TtsLinear(nin = l.nin, nout = 4l * l.hidden)
    lin.w := l.w_ih
    lin.b := l.b
    var g : array<float>
    with_job_que() {
        setup_dasllama_jobque_()
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
    with_job_que() {
        setup_dasllama_jobque_()
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
    var f = LstmFixture(gstride = (4 * hd + 63) / 64 * 64, bwd_hh = 4 * hd * hd + LSTM_HH_GAP)
    var lf <- make_lstm(hd, 0)
    var lb <- make_lstm(hd, 19)
    f.x <- row_fill(LSTM_T * LSTM_NIN, 41, 7, 0.03, 1.44)
    f.gates <- gates_ref(lf, f.x, f.gstride)
    var gb <- gates_ref(lb, f.x, f.gstride)
    f.gates |> push_from(gb)
    let nhh = 4 * hd * hd
    f.whh |> resize(f.bwd_hh + nhh)
    for (o in range(4 * hd)) {   // each direction's w_hh transposed [H][4H], as the slab lays it
        for (i in range(hd)) {
            f.whh[i * 4 * hd + o] = lf.w_hh[o * hd + i]
            f.whh[f.bwd_hh + i * 4 * hd + o] = lb.w_hh[o * hd + i]
        }
    }
    for (i in range(nhh, f.bwd_hh)) {
        f.whh[i] = POISON
    }
    f.y_ref <- bilstm_ref(lf, lb, f.x)
    var yff <- bilstm_ref(lf, lf, f.x)
    f.yc_ref <- half_rows(yff, hd, 1)
    delete gb
    delete yff
    delete lf
    delete lb
    return <- f
}

//! both directions into one [t][2H] row set, then the forward weights walked backward: every output under a NaN fill first
def private lstm_arm(t : T?; hd : int) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let tag = "lstm h{hd}"
        let B_Y = 1u
        let B_YC = 2u
        var f <- lstm_fixture(hd)
        let ny = LSTM_T * 2 * hd
        let nyc = LSTM_T * hd
        let bg = long_length(f.gates) * 4l
        let bw = long_length(f.whh) * 4l
        let by = int64(ny) * 4l
        let byc = int64(nyc) * 4l
        let gd = make_device_buf(bg)
        let wd = make_device_buf(bw)
        let yd = make_device_buf(by)
        let ycd = make_device_buf(byc)
        let sizes = fixed_array(by, byc)
        let at <- staged_offsets(sizes)
        var host = make_host_buf(at[length(sizes)], true, [cached = true])
        unsafe {
            upload_region_at(gd, 0l, addr<uint8 const?>(f.gates[0]), bg)
            upload_region_at(wd, 0l, addr<uint8 const?>(f.whh[0]), bw)
        }
        nan_sentinel_upload(yd, by)
        nan_sentinel_upload(ycd, byc)
        var s_y = set_tts_lstm_dir_cls(fixed_array(gd, wd, yd), fixed_array(bg, bw, by), fixed_array(0u, 0u, B_Y))
        var s_yc = set_tts_lstm_dir_cls(fixed_array(gd, wd, ycd), fixed_array(bg, bw, byc), fixed_array(0u, 0u, B_YC))
        var pf = TtsLstmArgs(t = uint(LSTM_T), hidden = uint(hd), goff = 0u, gstride = uint(f.gstride), hhoff = 0u, hoff = 0u,
            hstride = uint(2 * hd), backward = 0u)
        var pb = TtsLstmArgs(t = uint(LSTM_T), hidden = uint(hd), goff = uint(LSTM_T * f.gstride), gstride = uint(f.gstride),
            hhoff = uint(f.bwd_hh), hoff = uint(hd), hstride = uint(2 * hd), backward = 1u)
        var pc = TtsLstmArgs(t = uint(LSTM_T), hidden = uint(hd), goff = 0u, gstride = uint(f.gstride), hhoff = 0u, hoff = 0u,
            hstride = uint(hd), backward = 1u)
        cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
            enc_tts_lstm_dir_cls(raw, h, s_y, pf, 1l)
            enc_tts_lstm_dir_cls(raw, h, s_y, pb, 1l)
            enc_tts_lstm_dir_cls(raw, h, s_yc, pc, 1l)
            cmd_copy_planes(raw, h, fixed_array(yd, ycd), host, sizes, B_Y | B_YC)
        }
        var y_dev <- staged(host.mapped, at[0], ny, type<float>)
        var yc_dev <- staged(host.mapped, at[1], nyc, type<float>)
        lstm_asserts(t, tag, hd, f, y_dev, yc_dev)
        destroy_device_buf(gd)
        destroy_device_buf(wd)
        destroy_device_buf(yd)
        destroy_device_buf(ycd)
        destroy_host_buf(host)
        delete f
        delete y_dev
        delete yc_dev
    }
}

def private lstm_asserts(t : T?; tag : string; hd : int; f : LstmFixture; y_dev, yc_dev : array<float>) {
    let ny = LSTM_T * 2 * hd
    let nyc = LSTM_T * hd
    // `approx`'s 2e-3: both sides f32, the CPU matmul reassociating the h dot and adding the bias after it, over 23 steps
    let bad = mismatch_approx(y_dev, f.y_ref)
    log_maxdiff(tag, y_dev, f.y_ref, 2 * hd)
    t |> success(bad == 0, "{tag}: both directions over {LSTM_T} steps == the CPU bilstm ({bad} of {ny} off)")
    t |> success(!approx(y_dev[ny - 1], f.y_ref[ny - 1] + 0.5), "{tag}: the bar reds a poisoned element")
    let badc = mismatch_approx(yc_dev, f.yc_ref)
    log_maxdiff("{tag} reversed", yc_dev, f.yc_ref, hd)
    t |> success(badc == 0, "{tag}: the forward weights walked backward == the CPU's backward walk ({badc} of {nyc} off)")
    t |> success(!approx(yc_dev[0], f.yc_ref[0] + 0.5), "{tag} reversed: the bar reds a poisoned element")
    var fwd <- half_rows(y_dev, hd, 0)
    let moved = logits_maxdiff(fwd, yc_dev)
    t |> success(moved > 0.01, "{tag} control: flipping `backward` on the same gates moves the output (maxdiff {moved})")
    delete fwd
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
    let nw = PD_C * PD_K
    let boff = PD_WOFF + nw + PD_GAP
    var wn : array<float>
    wn |> resize(boff + PD_C + 4)
    for (v in wn) {
        v = POISON
    }
    for (i in range(nw)) {
        wn[PD_WOFF + i] = c.w[i]
    }
    for (j in range(PD_C)) {
        wn[boff + j] = c.b[j]
    }
    return <- wn
}

//! the depthwise transposed pool against the CPU rows form: the output under a NaN fill first
def private pool_dw_arm(t : T?) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let tag = "tts_pool_dw k3 s2 c22"
        let B_Y = 1u
        var c = TtsConv1d(cin = int64(PD_C), cout = int64(PD_C), k = int64(PD_K), stride = 2l, pad_l = 1l, pad_r = 1l, dilation = 1l,
            groups = int64(PD_C), output_padding = 1l, transposed = true)
        c.w <- row_fill(PD_C * PD_K, 37, 5, 0.02, 0.9)
        c.b <- row_fill(PD_C, 29, 11, 0.02, 0.9)
        let tout = int(conv1d_out_len(c, int64(PD_TIN)))
        t |> equal(tout, PD_TOUT, "{tag}: the output length")
        var x <- row_fill(PD_TIN * PD_C, 41, 7, 0.05, 2.1)
        var want : array<float>
        with_job_que() {
            setup_dasllama_jobque_()
            conv1d_rows_transposed_depthwise(c, x, int64(PD_TIN), want)
        }
        var wn <- pool_slab(c)
        let ny = tout * PD_C
        let bx = long_length(x) * 4l
        let bw = long_length(wn) * 4l
        let by = int64(ny) * 4l
        let xd = make_device_buf(bx)
        let wd = make_device_buf(bw)
        let yd = make_device_buf(by)
        var host = make_host_buf(by, true, [cached = true])
        unsafe {
            upload_region_at(xd, 0l, addr<uint8 const?>(x[0]), bx)
            upload_region_at(wd, 0l, addr<uint8 const?>(wn[0]), bw)
        }
        nan_sentinel_upload(yd, by)
        var s = set_tts_pool_dw_cls(fixed_array(xd, yd, wd), fixed_array(bx, by, bw), fixed_array(0u, B_Y, 0u))
        var pa = TtsPoolArgs(c = uint(PD_C), k = uint(PD_K), stride = 2u, pad_l = 1u, dil = 1u, t_in = uint(PD_TIN), t_out = uint(tout),
            woff = uint(PD_WOFF), boff = uint(PD_WOFF + PD_C * PD_K + PD_GAP))
        cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
            enc_tts_pool_dw_cls(raw, h, s, pa, tts_pool_dw_wgs(int64(ny)))
            cmd_copy_planes(raw, h, fixed_array(yd), host, fixed_array(by), B_Y)
        }
        var y_dev <- staged(host.mapped, 0l, ny, type<float>)
        // `approx`'s 2e-3: both sides f32 over at most two taps a channel
        let bad = mismatch_approx(y_dev, want)
        log_maxdiff(tag, y_dev, want, PD_C)
        t |> success(bad == 0, "{tag}: the pool == the CPU conv1d_rows_transposed_depthwise over {tout} x {PD_C} ({bad} of {ny} off)")
        t |> success(!approx(y_dev[ny - 1], want[ny - 1] + 0.5), "{tag}: the bar reds a poisoned element")
        destroy_device_buf(xd)
        destroy_device_buf(wd)
        destroy_device_buf(yd)
        destroy_host_buf(host)
        delete c
        delete x
        delete want
        delete wn
        delete y_dev
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
