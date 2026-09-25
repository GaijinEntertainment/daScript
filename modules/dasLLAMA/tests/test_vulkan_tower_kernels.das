options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module test_vulkan_tower_kernels

require dastest/testing_boost public
require math
require daslib/f16_cvt   // nolint:STYLE030,LINT019 — used only inside the vulkan static_if arms; the no-vulkan lane sees it unused
require dasllama/dasllama_plane   // nolint:STYLE030,LINT019 — the same
require dasllama/dasllama_tower   // nolint:STYLE030,LINT019 — the same
require dasllama/dasllama_common
require dasllama/dasllama_gemma4a   // gemma4a_attn_past: the Conformer cell's window
require dasllama/dasllama_qwen3a   // nolint:STYLE030,LINT019 — QWEN3A_TOK_PER_CHUNK: the front movers' qwen3a fixture, inside the vulkan arm alone
require dasllama/dasllama_audio   // nolint:STYLE030,LINT019 — build_dft_twiddles: the spectrum cell's twiddle table, inside the vulkan arm alone
require dasllama/dasllama_kqformat   // kq_elems: the Q8_0 block width that sizes the requant feed's scale plane
require ?vulkan vulkan
require ?vulkan dasllama/dasllama_vulkan_common
require ?vulkan dasllama/dasllama_vulkan_classes
require ?vulkan _vkd_oracles
require _compares
require _vk_kq_fixtures   // nolint:STYLE030,LINT019 — f16r, inside the vulkan arms alone
require dasllama/dasllama_math
require daslib/jobque_boost

//! The vision and audio towers' Vulkan kernel classes against their CPU oracles (a Vulkan device,
//! else the cells skip): the bidirectional flash tiles, the tower row classes, the Conformer and
//! FastConformer classes, the biased-block classes and their f16-feed stamps, the whisper decoder's
//! classes, the padded attention route, the window classes, the audio fronts' data movers and
//! spectral classes, and canary's rel-plane attention.

let private TOWER_NHEADS = 4
let private TOWER_ROWS = 77       //! one whole 64-row tile and a 13-row partial: the tail mask past kvlen runs on the last K step of both arms
let private TOWER_SLACK_ROWS = 64 //! shadow rows past kvlen that carry a large finite value: a leak through the tail mask reads as garbage, not as a small drift
let private TOWER_SENTINEL = 10000.0
let private TOWER_SCALE = 0.125
let private F16_SENTINEL_BITS = 0x7e007e00u   //! the f16 outputs' fill: a NaN in both halves, where the f32 word's low half would read as a legitimate zero pad
let private I8_SENTINEL_BITS = 0x80808080u    //! the Q8_0 quant outputs' fill: -128 in every byte, a value the requant never produces (|q| <= 127)
let private GUARD_GARBAGE_BITS = 0x40f00000u  //! 7.5: the slack past a guarded source, so a read the guard should stop lands a large finite value, not robust access's zero

def private fa_bidir_arm(t : T?; khr : bool; hs : int) {   // nolint:STYLE038 — one arm: the fixture, both stamps, the oracle and its controls
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let tag = "{khr ? "fa_khr" : "fa_cm2"} h{hs} bidir"
        let B_Q = 1u
        let B_K = 2u
        let B_Y = 4u
        let B_V = 8u
        let B_KH = 16u
        let B_VH = 32u
        let B_YC = 64u
        let nheads = TOWER_NHEADS
        let rows = TOWER_ROWS
        let qd = nheads * hs
        let kpad = rows + TOWER_SLACK_ROWS
        var qh : array<float>
        var kh : array<float>
        var vh : array<float>
        qh |> resize(rows * qd)
        kh |> resize(kpad * qd)
        vh |> resize(kpad * qd)
        for (i in range(rows * qd)) {
            qh[i] = float((i * 41) % 73) * 0.05 - 1.8
            kh[i] = f16r(float((i * 17) % 97) * 0.03 - 1.4)
            vh[i] = f16r(float((i * 53) % 89) * 0.04 - 1.7)
        }
        for (i in range(rows * qd, kpad * qd)) {
            kh[i] = TOWER_SENTINEL
            vh[i] = TOWER_SENTINEL
        }
        let q_bytes = int64(rows * qd) * 4l
        let m_bytes = int64(kpad * qd) * 4l
        let sh_bytes = int64(kpad * qd) * 2l
        let qdev = make_device_buf(q_bytes)
        let kdev = make_device_buf(m_bytes)
        let vdev = make_device_buf(m_bytes)
        let khdev = make_device_buf(sh_bytes)
        let vhdev = make_device_buf(sh_bytes)
        let yd = make_device_buf(q_bytes)
        let ycd = make_device_buf(q_bytes)
        var host = make_host_buf(q_bytes * 2l, true, [cached = true])
        var y : array<float>
        var yc : array<float>
        y |> resize(rows * qd)
        yc |> resize(rows * qd)
        unsafe {
            upload_region_at(qdev, 0l, addr<uint8 const?>(qh[0]), q_bytes)
            upload_region_at(kdev, 0l, addr<uint8 const?>(kh[0]), m_bytes)
            upload_region_at(vdev, 0l, addr<uint8 const?>(vh[0]), m_bytes)
        }
        fill_upload(khdev, 0l, sh_bytes, F16_SENTINEL_BITS)
        fill_upload(vhdev, 0l, sh_bytes, F16_SENTINEL_BITS)
        nan_sentinel_upload(yd, q_bytes)
        nan_sentinel_upload(ycd, q_bytes)
        var sck = set_f16cvt_cls(fixed_array(kdev, khdev), fixed_array(m_bytes, sh_bytes), fixed_array(B_K, B_KH))
        var scv = set_f16cvt_cls(fixed_array(vdev, vhdev), fixed_array(m_bytes, sh_bytes), fixed_array(B_V, B_VH))
        var sfa = fa_stamp_set(khr, fixed_array(qdev, khdev, vhdev, yd), fixed_array(q_bytes, sh_bytes, sh_bytes, q_bytes), fixed_array(B_Q, B_KH, B_VH, B_Y))
        var sfc = fa_stamp_set(khr, fixed_array(qdev, khdev, vhdev, ycd), fixed_array(q_bytes, sh_bytes, sh_bytes, q_bytes), fixed_array(B_Q, B_KH, B_VH, B_YC))
        var pcv = ActArgs(nelem = uint(kpad * qd), nblk = 0u)
        var pcf = FaArgs(rows = uint(rows), w0 = 0u, qd = uint(qd), kvd = uint(qd), kv_mul = 1u, kvbase = 0u,
            scale = TOWER_SCALE, window = 0u, sinkoff = 0u)
        let cvtwgs = int64((kpad * qd / 4 + 255) / 256)
        let wgs = fa_stamp_wgs(khr, int64(nheads), int64(rows))
        cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
            enc_f16cvt_cls(raw, h, sck, pcv, cvtwgs)
            enc_f16cvt_cls(raw, h, scv, pcv, cvtwgs)
            fa_stamp_enc(khr, int64(hs), false, false, false, false, raw, h, sfa, pcf, wgs, true)
            fa_stamp_enc(khr, int64(hs), false, false, false, false, raw, h, sfc, pcf, wgs)
            vhz_dep(raw, h, B_Y | B_YC, 0u, true)
            cmd_copy_range(raw, yd, 0l, host.buf, 0l, q_bytes)
            cmd_copy_range(raw, ycd, 0l, host.buf, q_bytes, q_bytes)
        }
        unsafe {
            memcpy(addr<void?>(y[0]), host.mapped, q_bytes)
            memcpy(addr<void?>(yc[0]), reinterpret<void?>(intptr(host.mapped) + uint64(q_bytes)), q_bytes)
        }
        var y_ref : array<float>
        y_ref |> resize(rows * qd)
        for (hh in range(nheads)) {
            for (r in range(rows)) {
                attn_row_oracle(qh, r * qd + hh * hs, kh, 0, vh, 0, qd, hh, hs, 1, rows, TOWER_SCALE, y_ref, r * qd + hh * hs)
            }
        }
        // `approx` covers the tile's f16 Q and P staging and the coopmat's reassociated sums; K and V are f16 on both sides
        let bad = mismatch_approx(y, y_ref)
        t |> success(bad == 0, "{tag}: every query row attends every key ({bad} of {rows * qd} off the CPU oracle)")
        t |> success(logits_maxabs(y) < TOWER_SENTINEL * 0.01, "{tag}: the slack rows past kvlen stayed masked (max |o| {logits_maxabs(y)})")
        t |> success(!approx(y[rows * qd - 1], y_ref[rows * qd - 1] + 1.5), "{tag}: the bar reds a poisoned oracle element")
        // the causal twin on the same planes: row 0 sees one key there and every key here, so its output moves
        var first_b : array<float>
        var first_c : array<float>
        first_b |> resize(qd)
        first_c |> resize(qd)
        for (i in range(qd)) {
            first_b[i] = y[i]
            first_c[i] = yc[i]
        }
        let moved = logits_maxdiff(first_b, first_c)
        t |> success(moved > 0.1, "{tag} control: the causal twin's first row differs (maxdiff {moved})")
        // ... while the last row sees every key on both stamps: its head rows agree to the bar
        var last_b : array<float>
        var last_c : array<float>
        last_b |> resize(qd)
        last_c |> resize(qd)
        for (i in range(qd)) {
            last_b[i] = y[(rows - 1) * qd + i]
            last_c[i] = yc[(rows - 1) * qd + i]
        }
        t |> success(mismatch_approx(last_b, last_c) == 0, "{tag} control: the last query row agrees with the causal twin (every key on both)")
        destroy_device_buf(qdev)
        destroy_device_buf(kdev)
        destroy_device_buf(vdev)
        destroy_device_buf(khdev)
        destroy_device_buf(vhdev)
        destroy_device_buf(yd)
        destroy_device_buf(ycd)
        destroy_host_buf(host)
        delete qh
        delete kh
        delete vh
        delete y
        delete yc
        delete y_ref
        delete first_b
        delete first_c
        delete last_b
        delete last_c
    }
}

[test]
def test_vkt_fa_bidir(t0 : T?) {
    t0 |> run("bidirectional flash attention (h64 and the padded h128, cm2 and KHR arms) == the CPU oracle over every key") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_f16cvt_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            var served = 0
            for (arm in fixed_array((khr = false, hs = 64), (khr = false, hs = 128), (khr = true, hs = 64), (khr = true, hs = 128))) {
                if (!fa_stamp_ensure(arm.khr, int64(arm.hs), false, false, false, false, true) || !fa_stamp_ensure(arm.khr, int64(arm.hs), false, false, false, false)) {
                    to_log(LOG_INFO, "{arm.khr ? "fa_khr" : "fa_cm2"} h{arm.hs} bidir: the device builds no such tile, arm skipped\n")
                    continue
                }
                fa_bidir_arm(t, arm.khr, arm.hs)
                served++
            }
            t |> success(served > 0, "at least one flash arm builds on this device ({served} of 4)")
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

let private ROWS_D = 256          //! the row classes' width: four 64-wide heads (the gemma4v head width), a 64-multiple
let private ROWS_N = 37           //! rows: off every tile and workgroup multiple
let private ROWS_FF = 512
let private ROWS_HEADS = 4
let private ROWS_EPS = 1.0e-6

def private row_fill(n : int; mul, add : int; scale, off : float) : array<float> {
    var a : array<float>
    a |> resize(n)
    for (i in range(n)) {
        a[i] = float((i * mul + add) % 97) * scale - off
    }
    return <- a
}

//! scales on the kernel cells' bar, quant bytes within one step: the device rounds a half-step value apart from the CPU
def private q8_rows_off(qd : array<int8>; sd : array<float>; qr : array<int8>; sr : array<float>) : int {
    var bad = 0
    for (a, b in sd, sr) {
        if (!approx(a, b)) {
            bad++
        }
    }
    for (a, b in qd, qr) {
        if (abs(int(a) - int(b)) > 1) {
            bad++
        }
    }
    return bad
}

def private gelu_quick_ref(x : float) : float => x * (1.0 / (1.0 + exp(-1.702 * x)))

//! the words of a read-back output that still carry the sentinel `word` - a whole buffer of them proves the dispatch wrote nothing there
def private sentinel_words(got : array<uint>; word : uint = NAN_SENTINEL_BITS) : int {
    var n = 0
    for (w in got) {
        if (w == word) {
            n++
        }
    }
    return n
}

[test]
def test_vkt_tower_rows(t0 : T?) {   // nolint:STYLE038 — one flat gate over the eight row classes: the fixture, one command buffer, eight oracles and their controls
    t0 |> run("the tower row classes (rms, clamp, clamp+cvt, clamp+rq, rope2d, post-add, post-add plain, geglu) == the CPU tower helpers") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tower_rms_cls() || !ensure_tower_clamp_cls() || !ensure_tower_clamp_cvt_cls()
                    || !ensure_tower_clamp_rq_cls() || !ensure_tower_rope2d_cls() || !ensure_tower_post_add_cls()
                    || !ensure_tower_post_add_plain_cls() || !ensure_tower_geglu_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let B_X = 1u
            let B_Y = 2u
            let B_XB = 4u
            let B_XB2 = 8u
            let B_Q = 16u
            let B_G = 32u
            let B_U = 64u
            let B_XH = 128u
            let B_XQ = 256u
            let B_XS = 512u
            let B_X0 = 1024u
            let B_XB0 = 2048u
            let B_XP = 4096u
            let B_XBP = 8192u
            let B_XP0 = 16384u
            let B_XBP0 = 32768u
            let B_XHALF = 65536u
            let B_XBHALF = 131072u
            let d = ROWS_D
            let rows = ROWS_N
            let ff = ROWS_FF
            let hs = d / ROWS_HEADS
            let pairs = hs / 4
            let nd = rows * d
            let nff = rows * ff
            let q8e = int(kq_elems(KqFmt.q8))
            var x <- row_fill(nd, 41, 7, 0.05, 2.1)
            var y <- row_fill(nd, 17, 3, 0.04, 1.9)
            var q <- row_fill(nd, 29, 11, 0.06, 2.8)
            var g <- row_fill(nff, 13, 5, 0.09, 4.0)
            var u <- row_fill(nff, 23, 9, 0.07, 3.3)
            var wn <- row_fill(2 * d, 7, 1, 0.01, -0.6)   // two weight rows: the pre-norm's at 0, the post-norm's at d - both near 1
            var tabs <- row_fill(rows * 4 * pairs, 31, 2, 0.02, 0.97)   // per row [cx | sx | cy | sy]: any values serve the oracle, which applies the same tables
            let lo = -1.25
            let hi = 1.5
            let lo2 = -0.75
            let hi2 = 0.9
            let bx = int64(nd) * 4l
            let bff = int64(nff) * 4l
            let bw = int64(2 * d) * 4l
            let btabs = int64(rows * 4 * pairs) * 4l
            let bh = int64(nd) * 2l
            let bq = int64(nd)
            let bs = int64(nd / q8e) * 4l
            let xd = make_device_buf(bx)
            let yd = make_device_buf(bx)
            let xbd = make_device_buf(bx)
            let xb2d = make_device_buf(bx)
            let qd = make_device_buf(bx)
            let gd = make_device_buf(bff)
            let ud = make_device_buf(bff)
            let wd = make_device_buf(bw)
            let td = make_device_buf(btabs)
            let xhd = make_device_buf(bh)
            let xqd = make_device_buf(bq)
            let xsd = make_device_buf(bs)
            let x0d = make_device_buf(bx)    // the ln_on = 0 arm's x: the same seam with no next pre-norm
            let xb0d = make_device_buf(bx)   // ... and its pre-norm output, which must stay untouched
            let x_plain_d = make_device_buf(bx)    // the plain stamp's x: the branch rows added at half weight with no post-norm
            let xb_plain_d = make_device_buf(bx)   // ... and its next pre-norm
            let x_plain0_d = make_device_buf(bx)    // the plain stamp at ln_on = 0: the seam alone
            let xb_plain0_d = make_device_buf(bx)   // ... and its pre-norm output, which must stay untouched
            let x_half_d = make_device_buf(bx)      // the post-norm stamp at ascale = 0.5, ln_on = 0
            let xb_half_d = make_device_buf(bx)
            let hostb = 11l * bx + bff + bh + bq + bs
            var host = make_host_buf(hostb, true, [cached = true])
            unsafe {
                upload_region_at(xd, 0l, addr<uint8 const?>(x[0]), bx)
                upload_region_at(x0d, 0l, addr<uint8 const?>(x[0]), bx)
                upload_region_at(x_plain_d, 0l, addr<uint8 const?>(x[0]), bx)
                upload_region_at(x_plain0_d, 0l, addr<uint8 const?>(x[0]), bx)
                upload_region_at(x_half_d, 0l, addr<uint8 const?>(x[0]), bx)
                upload_region_at(yd, 0l, addr<uint8 const?>(y[0]), bx)
                upload_region_at(qd, 0l, addr<uint8 const?>(q[0]), bx)
                upload_region_at(gd, 0l, addr<uint8 const?>(g[0]), bff)
                upload_region_at(ud, 0l, addr<uint8 const?>(u[0]), bff)
                upload_region_at(wd, 0l, addr<uint8 const?>(wn[0]), bw)
                upload_region_at(td, 0l, addr<uint8 const?>(tabs[0]), btabs)
            }
            nan_sentinel_upload(xbd, bx)
            nan_sentinel_upload(xb2d, bx)
            nan_sentinel_upload(xb0d, bx)
            nan_sentinel_upload(xb_plain_d, bx)
            nan_sentinel_upload(xb_plain0_d, bx)
            nan_sentinel_upload(xb_half_d, bx)
            nan_sentinel_upload(xsd, bs)
            fill_upload(xhd, 0l, bh, F16_SENTINEL_BITS)
            fill_upload(xqd, 0l, bq, I8_SENTINEL_BITS)
            var s_rms = set_tower_rms_cls(fixed_array(xd, xbd, wd), fixed_array(bx, bx, bw), fixed_array(B_X, B_XB, 0u))
            var s_clamp = set_tower_clamp_cls(fixed_array(yd), fixed_array(bx), fixed_array(B_Y))
            var s_cvt = set_tower_clamp_cvt_cls(fixed_array(xd, xhd), fixed_array(bx, bh), fixed_array(B_X, B_XH))
            var s_rq = set_tower_clamp_rq_cls(fixed_array(xd, xqd, xsd), fixed_array(bx, bq, bs), fixed_array(B_X, B_XQ, B_XS))
            var s_rope = set_tower_rope2d_cls(fixed_array(qd, td), fixed_array(bx, btabs), fixed_array(B_Q, 0u))
            var s_post = set_tower_post_add_cls(fixed_array(xd, yd, xb2d, wd), fixed_array(bx, bx, bx, bw), fixed_array(B_X, B_Y, B_XB2, 0u))
            var s_post0 = set_tower_post_add_cls(fixed_array(x0d, yd, xb0d, wd), fixed_array(bx, bx, bx, bw), fixed_array(B_X0, B_Y, B_XB0, 0u))
            var s_post_plain = set_tower_post_add_plain_cls(fixed_array(x_plain_d, yd, xb_plain_d, wd), fixed_array(bx, bx, bx, bw), fixed_array(B_XP, B_Y, B_XBP, 0u))
            var s_post_plain0 = set_tower_post_add_plain_cls(fixed_array(x_plain0_d, yd, xb_plain0_d, wd), fixed_array(bx, bx, bx, bw), fixed_array(B_XP0, B_Y, B_XBP0, 0u))
            var s_post_half = set_tower_post_add_cls(fixed_array(x_half_d, yd, xb_half_d, wd), fixed_array(bx, bx, bx, bw), fixed_array(B_XHALF, B_Y, B_XBHALF, 0u))
            var s_geglu = set_tower_geglu_cls(fixed_array(gd, ud), fixed_array(bff, bff), fixed_array(B_G, B_U))
            var pa_rms = TowerRowArgs(d = uint(d), rows = uint(rows), woff = 0u, eps = ROWS_EPS)
            var pa_post = TowerRowArgs(d = uint(d), rows = uint(rows), woff = uint(d), eps = ROWS_EPS, ln_on = 1u, lnoff = 0u, ascale = 1.0)
            var pa_post0 = TowerRowArgs(d = uint(d), rows = uint(rows), woff = uint(d), eps = ROWS_EPS, ln_on = 0u, lnoff = 0u, ascale = 1.0)
            var pa_post_plain = TowerRowArgs(d = uint(d), rows = uint(rows), woff = 0u, eps = ROWS_EPS, ln_on = 1u, lnoff = 0u, ascale = 0.5)
            var pa_post_plain0 = TowerRowArgs(d = uint(d), rows = uint(rows), woff = 0u, eps = ROWS_EPS, ln_on = 0u, lnoff = 0u, ascale = 0.5)
            var pa_post_half = TowerRowArgs(d = uint(d), rows = uint(rows), woff = uint(d), eps = ROWS_EPS, ln_on = 0u, lnoff = 0u, ascale = 0.5)
            var pa_clamp = TowerClampArgs(nelem = uint(nd), lo = lo, hi = hi)
            var pa_cvt = TowerClampArgs(nelem = uint(nd), lo = lo2, hi = hi2)
            var pa_rq = TowerClampRqArgs(inbase = 0u, nblk = uint(nd / q8e), lo = lo2, hi = hi2)
            var pa_rope = TowerRopeArgs(nheads = uint(ROWS_HEADS), hs = uint(hs), rows = uint(rows))
            var pa_geglu = TowerGegluArgs(nelem = uint(nff), glo = -2.0, ghi = 2.5, ulo = -1.5, uhi = 1.75, dlo = -1.0, dhi = 1.2)
            let wg4 = int64((nd / 4 + 255) / 256)
            cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_tower_rms_cls(raw, h, s_rms, pa_rms, int64(rows))
                enc_tower_clamp_cvt_cls(raw, h, s_cvt, pa_cvt, wg4)
                enc_tower_clamp_rq_cls(raw, h, s_rq, pa_rq, int64((nd / 4 + 255) / 256))
                enc_tower_rope2d_cls(raw, h, s_rope, pa_rope, int64((rows * ROWS_HEADS * pairs + 255) / 256))
                enc_tower_geglu_cls(raw, h, s_geglu, pa_geglu, int64((nff / 4 + 255) / 256))
                enc_tower_clamp_cls(raw, h, s_clamp, pa_clamp, wg4)
                enc_tower_post_add_cls(raw, h, s_post0, pa_post0, int64(rows))
                enc_tower_post_add_plain_cls(raw, h, s_post_plain, pa_post_plain, int64(rows))
                enc_tower_post_add_plain_cls(raw, h, s_post_plain0, pa_post_plain0, int64(rows))
                enc_tower_post_add_cls(raw, h, s_post_half, pa_post_half, int64(rows))
                enc_tower_post_add_cls(raw, h, s_post, pa_post, int64(rows))   // last: it rewrites x, which the rms and the feeds read whole
                vhz_dep(raw, h, B_X | B_Y | B_XB | B_XB2 | B_Q | B_G | B_XH | B_XQ | B_XS | B_X0 | B_XB0 | B_XP | B_XBP | B_XP0 | B_XBP0 | B_XHALF, 0u, true)
                cmd_copy_range(raw, xbd, 0l, host.buf, 0l, bx)
                cmd_copy_range(raw, yd, 0l, host.buf, bx, bx)
                cmd_copy_range(raw, xd, 0l, host.buf, 2l * bx, bx)
                cmd_copy_range(raw, xb2d, 0l, host.buf, 3l * bx, bx)
                cmd_copy_range(raw, gd, 0l, host.buf, 4l * bx, bff)
                cmd_copy_range(raw, xhd, 0l, host.buf, 4l * bx + bff, bh)
                cmd_copy_range(raw, xqd, 0l, host.buf, 4l * bx + bff + bh, bq)
                cmd_copy_range(raw, xsd, 0l, host.buf, 4l * bx + bff + bh + bq, bs)
                cmd_copy_range(raw, x0d, 0l, host.buf, 4l * bx + bff + bh + bq + bs, bx)
                cmd_copy_range(raw, xb0d, 0l, host.buf, 5l * bx + bff + bh + bq + bs, bx)
                cmd_copy_range(raw, x_plain_d, 0l, host.buf, 6l * bx + bff + bh + bq + bs, bx)
                cmd_copy_range(raw, xb_plain_d, 0l, host.buf, 7l * bx + bff + bh + bq + bs, bx)
                cmd_copy_range(raw, x_plain0_d, 0l, host.buf, 8l * bx + bff + bh + bq + bs, bx)
                cmd_copy_range(raw, xb_plain0_d, 0l, host.buf, 9l * bx + bff + bh + bq + bs, bx)
                cmd_copy_range(raw, x_half_d, 0l, host.buf, 10l * bx + bff + bh + bq + bs, bx)
            }
            var xp_dev : array<float>
            var xbp_dev : array<float>
            xp_dev |> resize(nd)
            xbp_dev |> resize(nd)
            var xb_dev : array<float>
            var y_dev : array<float>
            var x_dev : array<float>
            var xb2_dev : array<float>
            var g_dev : array<float>
            var xh_dev : array<uint16>
            var xq_dev : array<int8>
            var xs_dev : array<float>
            var x0_dev : array<float>
            var xb0_dev : array<uint>
            xb_dev |> resize(nd)
            y_dev |> resize(nd)
            x_dev |> resize(nd)
            xb2_dev |> resize(nd)
            g_dev |> resize(nff)
            xh_dev |> resize(nd)
            xq_dev |> resize(nd)
            xs_dev |> resize(nd / q8e)
            x0_dev |> resize(nd)
            xb0_dev |> resize(nd)
            unsafe {
                let hp = intptr(host.mapped)
                memcpy(addr<void?>(xb_dev[0]), reinterpret<void?>(hp), bx)
                memcpy(addr<void?>(y_dev[0]), reinterpret<void?>(hp + uint64(bx)), bx)
                memcpy(addr<void?>(x_dev[0]), reinterpret<void?>(hp + uint64(2l * bx)), bx)
                memcpy(addr<void?>(xb2_dev[0]), reinterpret<void?>(hp + uint64(3l * bx)), bx)
                memcpy(addr<void?>(g_dev[0]), reinterpret<void?>(hp + uint64(4l * bx)), bff)
                memcpy(addr<void?>(xh_dev[0]), reinterpret<void?>(hp + uint64(4l * bx + bff)), bh)
                memcpy(addr<void?>(xq_dev[0]), reinterpret<void?>(hp + uint64(4l * bx + bff + bh)), bq)
                memcpy(addr<void?>(xs_dev[0]), reinterpret<void?>(hp + uint64(4l * bx + bff + bh + bq)), bs)
                memcpy(addr<void?>(x0_dev[0]), reinterpret<void?>(hp + uint64(4l * bx + bff + bh + bq + bs)), bx)
                memcpy(addr<void?>(xb0_dev[0]), reinterpret<void?>(hp + uint64(5l * bx + bff + bh + bq + bs)), bx)
                memcpy(addr<void?>(xp_dev[0]), reinterpret<void?>(hp + uint64(6l * bx + bff + bh + bq + bs)), bx)
                memcpy(addr<void?>(xbp_dev[0]), reinterpret<void?>(hp + uint64(7l * bx + bff + bh + bq + bs)), bx)
            }
            let xp0_dev <- staged_f32(host.mapped, 8l * bx + bff + bh + bq + bs, nd)
            let xbp0_dev <- staged_u32(host.mapped, 9l * bx + bff + bh + bq + bs, nd)
            let xhalf_dev <- staged_f32(host.mapped, 10l * bx + bff + bh + bq + bs, nd)
            var q_dev : array<float>
            q_dev |> resize(nd)
            cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
                vhz_dep(raw, h, B_Q, 0u, true)
                cmd_copy_range(raw, qd, 0l, host.buf, 0l, bx)
            }
            unsafe {
                memcpy(addr<void?>(q_dev[0]), host.mapped, bx)
            }
            let wplane = PlaneF(p = unsafe(addr<float const?>(wn[0])), n = int64(2 * d))
            var xb_ref : array<float>
            xb_ref |> resize(nd)
            rms_rows(xb_ref, x, wplane, 0l, int64(d), int64(rows), ROWS_EPS)
            let bad_rms = mismatch_approx(xb_dev, xb_ref)
            t |> success(bad_rms == 0, "tower_rms: the weighted rms rows match rms_rows ({bad_rms} of {nd} off)")
            t |> success(!approx(xb_dev[nd - 1], xb_ref[nd - 1] + 0.5), "tower_rms: the bar reds a poisoned element")
            var y_ref : array<float>
            y_ref |> resize(nd)
            clamp_rows(y_ref, y, int64(nd), lo, hi)
            let bad_clamp = mismatch_exact(y_dev, y_ref)
            t |> success(bad_clamp == 0, "tower_clamp: the rows clamp in place exactly ({bad_clamp} of {nd} off)")
            t |> success(y_dev[nd - 1] != y_ref[nd - 1] + 0.5, "tower_clamp: the bar reds a poisoned element")
            var clamped_x : array<float>
            clamped_x |> resize(nd)
            clamp_rows(clamped_x, x, int64(nd), lo2, hi2)
            var bad_cvt = 0
            for (i in range(nd)) {
                if (uint(xh_dev[i]) != f32_to_f16(clamped_x[i])) {
                    bad_cvt++
                }
            }
            t |> success(bad_cvt == 0, "tower_clamp_cvt: the f16 feed is the clamped rows rounded to f16, bit for bit ({bad_cvt} of {nd} off)")
            t |> success(uint(xh_dev[nd - 1]) != f32_to_f16(clamped_x[nd - 1] + 0.5), "tower_clamp_cvt: the bar reds a poisoned element")
            var xq_ref : array<int8>
            var xs_ref : array<float>
            requant_rows_q8_sized(clamped_x, int64(d), int64(rows), xq_ref, xs_ref)
            let bad_rq = q8_rows_off(xq_dev, xs_dev, xq_ref, xs_ref)
            t |> success(bad_rq == 0, "tower_clamp_rq: the Q8_0 feed is the clamped rows requantized as the CPU does ({bad_rq} off)")
            var xs_poison := xs_ref
            xs_poison[0] += 0.5
            t |> success(q8_rows_off(xq_dev, xs_dev, xq_ref, xs_poison) > 0, "tower_clamp_rq: the bar reds a poisoned scale")
            var q_ref := q
            var cos_x : array<float>
            var sin_x : array<float>
            var cos_y : array<float>
            var sin_y : array<float>
            cos_x |> resize(rows * pairs)
            sin_x |> resize(rows * pairs)
            cos_y |> resize(rows * pairs)
            sin_y |> resize(rows * pairs)
            for (r in range(rows)) {
                for (j in range(pairs)) {
                    cos_x[r * pairs + j] = tabs[r * 4 * pairs + j]
                    sin_x[r * pairs + j] = tabs[r * 4 * pairs + pairs + j]
                    cos_y[r * pairs + j] = tabs[r * 4 * pairs + 2 * pairs + j]
                    sin_y[r * pairs + j] = tabs[r * 4 * pairs + 3 * pairs + j]
                }
            }
            rope_neox_2d_rows(q_ref, int64(rows), int64(ROWS_HEADS), int64(hs), cos_x, sin_x, cos_y, sin_y)
            let bad_rope = mismatch_approx(q_dev, q_ref)
            t |> success(bad_rope == 0, "tower_rope2d: the 2D neox rope matches rope_neox_2d_rows ({bad_rope} of {nd} off)")
            t |> success(!approx(q_dev[0], q_ref[0] + 0.5), "tower_rope2d: the bar reds a poisoned element")
            var x_ref := x
            var ypost : array<float>
            ypost |> resize(nd)
            rms_rows(ypost, y_ref, wplane, int64(d), int64(d), int64(rows), ROWS_EPS)   // the post-add reads y after the clamp the same command applied to it
            add_inplace_rows(x_ref, ypost, int64(nd))
            let bad_add = mismatch_approx(x_dev, x_ref)
            t |> success(bad_add == 0, "tower_post_add: x += rms(y) . wpost ({bad_add} of {nd} off)")
            t |> success(!approx(x_dev[nd - 1], x_ref[nd - 1] + 0.5), "tower_post_add: the bar reds a poisoned element of x")
            var xb2_ref : array<float>
            xb2_ref |> resize(nd)
            rms_rows(xb2_ref, x_ref, wplane, 0l, int64(d), int64(rows), ROWS_EPS)
            let bad_ln = mismatch_approx(xb2_dev, xb2_ref)
            t |> success(bad_ln == 0, "tower_post_add: the next pre-norm over the updated rows ({bad_ln} of {nd} off)")
            t |> success(!approx(xb2_dev[nd - 1], xb2_ref[nd - 1] + 0.5), "tower_post_add: the bar reds a poisoned element")
            let bad_add0 = mismatch_approx(x0_dev, x_ref)
            t |> success(bad_add0 == 0, "tower_post_add ln_on = 0: the seam alone lands the same x ({bad_add0} of {nd} off)")
            t |> equal(sentinel_words(xb0_dev), nd, "tower_post_add ln_on = 0: the pre-norm output stays untouched (every word the sentinel)")
            var xhalf_ref := x
            for (i in range(nd)) {
                xhalf_ref[i] += 0.5 * ypost[i]
            }
            let bad_half = mismatch_approx(xhalf_dev, xhalf_ref)
            t |> success(bad_half == 0, "tower_post_add ascale = 0.5: x += 0.5 . rms(y) . wpost ({bad_half} of {nd} off)")
            t |> success(!approx(xhalf_dev[nd - 1], xhalf_ref[nd - 1] + 0.5), "tower_post_add ascale = 0.5: the bar reds a poisoned element")
            var xp_ref := x
            for (i in range(nd)) {
                xp_ref[i] += 0.5 * y_ref[i]
            }
            let bad_plain = mismatch_approx(xp_dev, xp_ref)
            t |> success(bad_plain == 0, "tower_post_add_plain: x += 0.5 . y with no post-norm ({bad_plain} of {nd} off)")
            t |> success(!approx(xp_dev[0], xp_ref[0] + 0.5), "tower_post_add_plain: the bar reds a poisoned element of x")
            t |> success(mismatch_approx(xp_dev, x_ref) > nd / 4, "tower_post_add_plain: the plain stamp is not the post-norm stamp ({mismatch_approx(xp_dev, x_ref)} of {nd} apart)")
            let bad_plain0 = mismatch_approx(xp0_dev, xp_ref)
            t |> success(bad_plain0 == 0, "tower_post_add_plain ln_on = 0: the seam alone lands the same x ({bad_plain0} of {nd} off)")
            t |> success(!approx(xp0_dev[nd - 1], xp_ref[nd - 1] + 0.5), "tower_post_add_plain ln_on = 0: the bar reds a poisoned element")
            t |> equal(sentinel_words(xbp0_dev), nd, "tower_post_add_plain ln_on = 0: the pre-norm output stays untouched (every word the sentinel)")
            var xbp_ref : array<float>
            xbp_ref |> resize(nd)
            rms_rows(xbp_ref, xp_ref, wplane, 0l, int64(d), int64(rows), ROWS_EPS)
            let bad_plain_ln = mismatch_approx(xbp_dev, xbp_ref)
            t |> success(bad_plain_ln == 0, "tower_post_add_plain: the next pre-norm over the half-weight rows ({bad_plain_ln} of {nd} off)")
            t |> success(!approx(xbp_dev[nd - 1], xbp_ref[nd - 1] + 0.5), "tower_post_add_plain: the bar reds a poisoned element")
            var g_ref : array<float>
            g_ref |> resize(nff)
            for (i in range(nff)) {
                let gv = clamp(g[i], -2.0, 2.5)
                let uv = clamp(u[i], -1.5, 1.75)
                g_ref[i] = clamp(gelu_quick_ref(gv) * uv, -1.0, 1.2)
            }
            let bad_geglu = mismatch_approx(g_dev, g_ref)
            t |> success(bad_geglu == 0, "tower_geglu: the clamped GEGLU-quick matches its closed form ({bad_geglu} of {nff} off)")
            t |> success(!approx(g_dev[0], g_ref[0] + 0.5), "tower_geglu: the bar reds a poisoned element")
            for (b in fixed_array(xd, yd, xbd, xb2d, qd, gd, ud, wd, td, xhd, xqd, xsd, x0d, xb0d, x_plain_d, xb_plain_d, x_plain0_d, xb_plain0_d, x_half_d, xb_half_d)) {
                destroy_device_buf(b)
            }
            destroy_host_buf(host)
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

let private G4A_PAST = int(gemma4a_attn_past())   //! the RPE table carries past + 1 rows
let private G4A_CAP = gemma4a_softcap()

//! the CPU form of the windowed RPE attention, spelled as `gemma4a_encode`'s loop
def private g4a_attn_ref(q, k, v, rpe, pds : array<float>; rows, d, heads, hs : int; qscale, kscale, cap : float) : array<float> {
    var ao : array<float>
    ao |> resize(rows * d)
    var sc : array<float>
    sc |> resize(G4A_PAST)
    for (hh in range(heads)) {
        let hoff = hh * hs
        for (i in range(rows)) {
            let j0 = max(0, i - (G4A_PAST - 1))
            let cnt = i - j0 + 1
            for (j in range(j0, i + 1)) {
                let rr = (G4A_PAST - (i - j)) * d
                var cs = 0.0
                var rs = 0.0
                for (n in range(hs)) {
                    let qv = q[i * d + hoff + n] * qscale * pds[n]
                    cs += qv * (k[j * d + hoff + n] * kscale)
                    rs += qv * rpe[rr + hoff + n]
                }
                sc[j - j0] = cap * tanh((cs + rs) / cap)
            }
            var m = -1.0e30
            for (c in range(cnt)) {
                m = max(m, sc[c])
            }
            var tot = 0.0
            for (c in range(cnt)) {
                sc[c] = exp(sc[c] - m)
                tot += sc[c]
            }
            for (n in range(hs)) {
                var acc = 0.0
                for (j in range(j0, i + 1)) {
                    acc += sc[j - j0] * v[j * d + hoff + n]
                }
                ao[i * d + hoff + n] = acc / tot
            }
        }
    }
    return <- ao
}

[test]
def test_vkt_tower_conformer(t0 : T?) {   // nolint:STYLE038 — one flat gate over the four classes: the fixture, one command buffer, the oracles and their controls
    t0 |> run("the Conformer row classes (silu, GLU, depthwise conv, windowed RPE attention) == the CPU forms") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tower_act_cls() || !ensure_tower_glu_sig_cls() || !ensure_tower_dw_conv5_cls()
                    || !ensure_tower_g4a_attn64_cls() || !ensure_tower_g4a_attn_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let B_S = 1u
            let B_H = 2u
            let B_G = 4u
            let B_C = 8u
            let B_AO = 16u
            let d = ROWS_D
            let rows = ROWS_N
            let hs = d / ROWS_HEADS
            let nd = rows * d
            var s <- row_fill(nd, 23, 5, 0.09, 6.0)   // silu's rows: both tails
            var glu_pw_rows <- row_fill(rows * 2 * d, 31, 7, 0.05, 2.5)
            var q <- row_fill(nd, 41, 3, 0.03, 1.1)
            var k <- row_fill(nd, 17, 11, 0.04, 1.3)
            var v <- row_fill(nd, 29, 13, 0.05, 1.7)
            var rpe <- row_fill((G4A_PAST + 1) * d, 13, 2, 0.02, 0.9)
            var wn <- row_fill(hs + d * 5, 7, 1, 0.01, 0.4)
            let bx = int64(nd) * 4l
            let bh = int64(rows * 2 * d) * 4l
            let brpe = int64((G4A_PAST + 1) * d) * 4l
            let bw = int64(hs + d * 5) * 4l
            let sd = make_device_buf(bx)
            let hd = make_device_buf(bh)
            let gd = make_device_buf(bx)
            let cd = make_device_buf(bx)
            let qd = make_device_buf(bx)
            let kd = make_device_buf(bx)
            let vd = make_device_buf(bx)
            let aod = make_device_buf(bx)
            let rd = make_device_buf(brpe)
            let wd = make_device_buf(bw)
            var host = make_host_buf(4l * bx, true, [cached = true])
            unsafe {
                upload_region_at(sd, 0l, addr<uint8 const?>(s[0]), bx)
                upload_region_at(hd, 0l, addr<uint8 const?>(glu_pw_rows[0]), bh)
                upload_region_at(qd, 0l, addr<uint8 const?>(q[0]), bx)
                upload_region_at(kd, 0l, addr<uint8 const?>(k[0]), bx)
                upload_region_at(vd, 0l, addr<uint8 const?>(v[0]), bx)
                upload_region_at(rd, 0l, addr<uint8 const?>(rpe[0]), brpe)
                upload_region_at(wd, 0l, addr<uint8 const?>(wn[0]), bw)
            }
            nan_sentinel_upload(gd, bx)
            nan_sentinel_upload(cd, bx)
            nan_sentinel_upload(aod, bx)
            var s_silu = set_tower_act_cls(fixed_array(sd), fixed_array(bx), fixed_array(B_S))
            var s_glu = set_tower_glu_sig_cls(fixed_array(hd, gd), fixed_array(bh, bx), fixed_array(B_H, B_G))
            var s_dw = set_tower_dw_conv5_cls(fixed_array(gd, cd, wd), fixed_array(bx, bx, bw), fixed_array(B_G, B_C, 0u))
            var s_attn = set_tower_g4a_attn64_cls(fixed_array(qd, kd, vd, aod, rd, wd), fixed_array(bx, bx, bx, bx, brpe, bw), fixed_array(0u, 0u, 0u, B_AO, 0u, 0u))
            var pa_silu = TowerBiasArgs(d = uint(d), nelem = uint(nd), boff = 0u, act = BIAS_ACT_SILU)
            var pa_convmod = TowerConvModArgs(d = uint(d), nelem = uint(nd), woff = uint(hs))
            let qscale = (1.0 / sqrt(float(hs))) / log(2.0)
            let kscale = log(1.0 + exp(1.0)) / log(2.0)
            var pa_attn = TowerG4aAttnArgs(d = uint(d), npos = uint(rows), pdsoff = 0u)
            cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_tower_act_cls(raw, h, s_silu, pa_silu, int64((nd / 4 + 255) / 256))
                enc_tower_glu_sig_cls(raw, h, s_glu, pa_convmod, int64((nd + 255) / 256))
                enc_tower_dw_conv5_cls(raw, h, s_dw, pa_convmod, int64((nd + 255) / 256))
                enc_tower_g4a_attn64_cls(raw, h, s_attn, pa_attn, int64((rows * ROWS_HEADS + 63) / 64))
                vhz_dep(raw, h, B_S | B_G | B_C | B_AO, 0u, true)
                cmd_copy_range(raw, sd, 0l, host.buf, 0l, bx)
                cmd_copy_range(raw, gd, 0l, host.buf, bx, bx)
                cmd_copy_range(raw, cd, 0l, host.buf, 2l * bx, bx)
                cmd_copy_range(raw, aod, 0l, host.buf, 3l * bx, bx)
            }
            var s_dev : array<float>
            var g_dev : array<float>
            var c_dev : array<float>
            var ao_dev : array<float>
            s_dev |> resize(nd)
            g_dev |> resize(nd)
            c_dev |> resize(nd)
            ao_dev |> resize(nd)
            unsafe {
                let hp = intptr(host.mapped)
                memcpy(addr<void?>(s_dev[0]), host.mapped, bx)
                memcpy(addr<void?>(g_dev[0]), reinterpret<void?>(hp + uint64(bx)), bx)
                memcpy(addr<void?>(c_dev[0]), reinterpret<void?>(hp + uint64(2l * bx)), bx)
                memcpy(addr<void?>(ao_dev[0]), reinterpret<void?>(hp + uint64(3l * bx)), bx)
            }
            var s_ref := s
            silu(s_ref, int64(nd))
            let bad_silu = mismatch_approx(s_dev, s_ref)
            t |> success(bad_silu == 0, "tower_act (the unbiased silu stamp): the rows match silu ({bad_silu} of {nd} off)")
            t |> success(!approx(s_dev[0], s_ref[0] + 0.5), "tower_act: the bar reds a poisoned element")
            var g_ref : array<float>
            g_ref |> resize(nd)
            for (r in range(rows)) {
                for (c in range(d)) {
                    let gate = glu_pw_rows[r * 2 * d + d + c]
                    g_ref[r * d + c] = glu_pw_rows[r * 2 * d + c] * (1.0 / (1.0 + exp(-gate)))
                }
            }
            let bad_glu = mismatch_approx(g_dev, g_ref)
            t |> success(bad_glu == 0, "tower_glu_sig: value . sigmoid(gate) off the pointwise rows ({bad_glu} of {nd} off)")
            t |> success(!approx(g_dev[nd - 1], g_ref[nd - 1] + 0.5), "tower_glu_sig: the bar reds a poisoned element")
            var c_ref : array<float>
            c_ref |> resize(nd)
            for (r in range(rows)) {
                for (c in range(d)) {
                    var acc = 0.0
                    for (kk in range(5)) {
                        let src = r - 4 + kk
                        if (src >= 0) {
                            acc += wn[hs + c * 5 + kk] * g_ref[src * d + c]
                        }
                    }
                    c_ref[r * d + c] = acc
                }
            }
            let bad_dw = mismatch_approx(c_dev, c_ref)
            t |> success(bad_dw == 0, "tower_dw_conv5: the causal depthwise taps match the CPU loop ({bad_dw} of {nd} off)")
            t |> success(!approx(c_dev[0], c_ref[0] + 0.5), "tower_dw_conv5: the bar reds a poisoned element")
            let ao_ref <- g4a_attn_ref(q, k, v, rpe, wn, rows, d, ROWS_HEADS, hs, qscale, kscale, G4A_CAP)
            let bad_attn = mismatch_approx(ao_dev, ao_ref)
            t |> success(bad_attn == 0, "tower_g4a_attn (64-wide stamp): the windowed RPE attention matches the CPU loop ({bad_attn} of {nd} off)")
            t |> success(!approx(ao_dev[nd - 1], ao_ref[nd - 1] + 0.5), "tower_g4a_attn: the bar reds a poisoned element")
            var k_scaled := k
            for (i in range(nd)) {
                k_scaled[i] *= 1.5
            }
            let ao_ctl <- g4a_attn_ref(q, k_scaled, v, rpe, wn, rows, d, ROWS_HEADS, hs, qscale, kscale, G4A_CAP)
            let moved = mismatch_approx(ao_dev, ao_ctl)
            t |> success(moved > nd / 4, "tower_g4a_attn control: scaled keys land the oracle elsewhere ({moved} of {nd} moved)")
            // the 128-wide stamp (the served carriers' head) over the same rows read as two heads, the queries scaled up so
            // the scores reach the softcap's knee: the oracle at the cap, and at four times the cap as the control
            let heads2 = ROWS_HEADS / 2
            let hs2 = d / heads2
            var q_at_cap_knee <- [for (qv in q); qv * 30.0]
            upload_region_at(qd, 0l, unsafe(addr<uint8 const?>(q_at_cap_knee[0])), bx)
            nan_sentinel_upload(aod, bx)
            var s_attn128 = set_tower_g4a_attn_cls(fixed_array(qd, kd, vd, aod, rd, wd), fixed_array(bx, bx, bx, bx, brpe, bw), fixed_array(0u, 0u, 0u, B_AO, 0u, 0u))
            let qscale2 = (1.0 / sqrt(float(hs2))) / log(2.0)
            var pa_attn128 = TowerG4aAttnArgs(d = uint(d), npos = uint(rows), pdsoff = 0u)
            cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_tower_g4a_attn_cls(raw, h, s_attn128, pa_attn128, int64((rows * heads2 + 63) / 64))
                vhz_dep(raw, h, B_AO, 0u, true)
                cmd_copy_range(raw, aod, 0l, host.buf, 0l, bx)
            }
            let ao_knee_dev <- staged_f32(host.mapped, 0l, nd)
            let ao_capped_ref <- g4a_attn_ref(q_at_cap_knee, k, v, rpe, wn, rows, d, heads2, hs2, qscale2, kscale, G4A_CAP)
            let bad_capped = mismatch_approx(ao_knee_dev, ao_capped_ref)
            t |> success(bad_capped == 0, "tower_g4a_attn (128-wide stamp): the windowed RPE attention over scaled queries matches the CPU loop ({bad_capped} of {nd} off)")
            t |> success(!approx(ao_knee_dev[0], ao_capped_ref[0] + 0.5), "tower_g4a_attn 128: the bar reds a poisoned element")
            let ao_cap4_ref <- g4a_attn_ref(q_at_cap_knee, k, v, rpe, wn, rows, d, heads2, hs2, qscale2, kscale, 4.0 * G4A_CAP)
            let moved_cap4 = mismatch_approx(ao_knee_dev, ao_cap4_ref)
            t |> success(moved_cap4 > nd / 4, "tower_g4a_attn 128 control: the softcap engages on the scaled queries - the oracle at four times the cap lands elsewhere ({moved_cap4} of {nd} moved)")
            for (b in fixed_array(sd, hd, gd, cd, qd, kd, vd, aod, rd, wd)) {
                destroy_device_buf(b)
            }
            destroy_host_buf(host)
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

let private CN_CONV_KERNEL = 9   //! canary's depthwise taps (centered)

//! the CPU form of canary's rel-pos attention, spelled as `cn_blocks_cpu`'s loop
def private cn_attn_ref(q, k, v, posl, wn : array<float>; rows, d, heads, hs, buoff, bvoff : int) : array<float> {
    var ao : array<float>
    ao |> resize(rows * d)
    var sc : array<float>
    sc |> resize(rows)
    let scale = 1.0 / sqrt(float(hs))
    for (hh in range(heads)) {
        let hoff = hh * hs
        for (i in range(rows)) {
            let base = rows - 1 - i
            var m = -1.0e30
            for (j in range(rows)) {
                var cs = 0.0
                var rs = 0.0
                for (n in range(hs)) {
                    let qv = q[i * d + hoff + n]
                    cs += (qv + wn[buoff + hoff + n]) * k[j * d + hoff + n]
                    rs += (qv + wn[bvoff + hoff + n]) * posl[(base + j) * d + hoff + n]
                }
                sc[j] = (cs + rs) * scale
                m = max(m, sc[j])
            }
            var tot = 0.0
            for (j in range(rows)) {
                sc[j] = exp(sc[j] - m)
                tot += sc[j]
            }
            for (n in range(hs)) {
                var acc = 0.0
                for (j in range(rows)) {
                    acc += sc[j] * v[j * d + hoff + n]
                }
                ao[i * d + hoff + n] = acc / tot
            }
        }
    }
    return <- ao
}

[test]
def test_vkt_tower_fastconformer(t0 : T?) {
    t0 |> run("the FastConformer conv module tail (the centered depthwise conv + folded BN + silu) == the CPU form") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tower_cn_dw_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let B_C = 1u
            let d = ROWS_D
            let rows = ROWS_N
            let nd = rows * d
            var glu_out <- row_fill(nd, 31, 7, 0.06, 2.5)
            var wn <- row_fill(2 * d + d * CN_CONV_KERNEL + 2 * d, 7, 1, 0.01, 0.3)
            let taps_off = 2 * d
            let bnw_off = taps_off + d * CN_CONV_KERNEL
            let bnb_off = bnw_off + d
            let kpad = (CN_CONV_KERNEL - 1) / 2
            let bx = int64(nd) * 4l
            let bsrc = int64((rows + kpad) * d) * 4l   // the rows past npos carry garbage: a tap past the source's end the bound lets through reads it
            let bw = int64(2 * d + d * CN_CONV_KERNEL + 2 * d) * 4l
            let gd = make_device_buf(bsrc)
            let cd = make_device_buf(bx)
            let wd = make_device_buf(bw)
            var host = make_host_buf(bx, true, [cached = true])
            unsafe {
                upload_region_at(gd, 0l, addr<uint8 const?>(glu_out[0]), bx)
                upload_region_at(wd, 0l, addr<uint8 const?>(wn[0]), bw)
            }
            fill_upload(gd, bx, bsrc - bx, GUARD_GARBAGE_BITS)
            nan_sentinel_upload(cd, bx)
            var s_dw = set_tower_cn_dw_cls(fixed_array(gd, cd, wd), fixed_array(bsrc, bx, bw), fixed_array(0u, B_C, 0u))
            var pa_dw = TowerCnDwArgs(d = uint(d), nk = uint(CN_CONV_KERNEL), npos = uint(rows), woff = uint(taps_off), bnwoff = uint(bnw_off), bnboff = uint(bnb_off))
            cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_tower_cn_dw_cls(raw, h, s_dw, pa_dw, int64((nd + 255) / 256))
                vhz_dep(raw, h, B_C, 0u, true)
                cmd_copy_range(raw, cd, 0l, host.buf, 0l, bx)
            }
            let c_dev <- staged_f32(host.mapped, 0l, nd)
            var c_ref : array<float>
            c_ref |> resize(nd)
            for (r in range(rows)) {
                for (c in range(d)) {
                    var acc = 0.0
                    for (kk in range(CN_CONV_KERNEL)) {
                        let src = r + kk - kpad
                        if (src >= 0 && src < rows) {
                            acc += wn[taps_off + c * CN_CONV_KERNEL + kk] * glu_out[src * d + c]
                        }
                    }
                    let bv = acc * wn[bnw_off + c] + wn[bnb_off + c]
                    c_ref[r * d + c] = bv * (1.0 / (1.0 + exp(-bv)))
                }
            }
            let bad_dw = mismatch_approx(c_dev, c_ref)
            t |> success(bad_dw == 0, "tower_cn_dw: the centered taps, the folded BatchNorm and silu match the CPU loop ({bad_dw} of {nd} off)")
            t |> success(!approx(c_dev[nd - 1], c_ref[nd - 1] + 0.5), "tower_cn_dw: the bar reds a poisoned element")
            for (b in fixed_array(gd, cd, wd)) {
                destroy_device_buf(b)
            }
            destroy_host_buf(host)
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

let private LN_HS = 72   //! the SigLIP head width: off the flash tiles' 64 / 128, so the restride pads it to 128
let private LN_HSP = 128

[test]
def test_vkt_tower_ln_rows(t0 : T?) {   // nolint:STYLE038 — one flat gate over the biased-block classes: the fixture, one command buffer, the oracles and their controls
    t0 |> run("the biased-block row classes (layernorm, bias+act, post-add+ln, the head restrides) == the CPU tower helpers") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tower_ln_cls() || !ensure_tower_bias_act_cls() || !ensure_tower_post_add_ln_cls()
                    || !ensure_tower_hpad_cls() || !ensure_tower_hpad16_cls() || !ensure_tower_hunpad_cls() || !ensure_tower_rope_tab_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let B_X = 1u
            let B_XB = 2u
            let B_G = 4u
            let B_Y = 8u
            let B_XB2 = 16u
            let B_Q = 32u
            let B_QP = 64u
            let B_QPH = 128u
            let B_QC = 256u
            let B_QKV = 512u
            let B_G0 = 1024u
            let B_X0 = 2048u
            let B_XB0 = 4096u
            let B_GERF = 8192u
            let B_GSILU = 16384u
            let B_GRELU = 32768u
            let B_XHALF = 65536u
            let B_XBHALF = 131072u
            let d = ROWS_D
            let rows = ROWS_N
            let rows_pad = (rows + 63) / 64 * 64   // the padded panels' 64-aligned row cap
            let nd = rows * d
            let d_compact = ROWS_HEADS * LN_HS
            let d_padded = ROWS_HEADS * LN_HSP
            var x <- row_fill(nd, 41, 7, 0.05, 2.1)
            var y <- row_fill(nd, 17, 3, 0.04, 1.9)
            var g <- row_fill(nd, 13, 5, 0.3, 14.0)   // wide enough to reach both tails of the GELU (past +-10)
            var q <- row_fill(rows * d_compact, 29, 11, 0.06, 2.8)
            var qkv <- row_fill(rows * 3 * d_compact, 37, 5, 0.05, 2.2)   // a fused [q | k | v] row: the rope runs on its k slot in place
            let half = LN_HS / 2
            var cos_t <- row_fill(rows * half, 31, 2, 0.02, 0.97)
            var sin_t <- row_fill(rows * half, 19, 4, 0.03, 0.55)
            var wn <- row_fill(4 * d, 7, 1, 0.01, -0.6)   // rows: ln w at 0, ln b at d, the bias at 2d, the next ln w at 3d (its bias reads the first ln's)
            let bx = int64(nd) * 4l
            let bw = int64(4 * d) * 4l
            let bq = int64(rows * d_compact) * 4l
            let bp = int64(rows_pad * d_padded) * 4l
            let bph = int64(rows_pad * d_padded) * 2l
            let xd = make_device_buf(bx)
            let xbd = make_device_buf(bx)
            let yd = make_device_buf(bx)
            let gd = make_device_buf(bx)
            let g0d = make_device_buf(bx)    // the BIAS_ACT_NONE arm's rows: the bias alone
            let gerf_d = make_device_buf(bx)
            let gsilu_d = make_device_buf(bx)
            let grelu_d = make_device_buf(bx)
            let xb2d = make_device_buf(bx)
            let x0d = make_device_buf(bx)    // the ln_on = 0 arm's x: the seam with no next layernorm
            let xb0d = make_device_buf(bx)   // ... and its layernorm output, which must stay untouched
            let x_half_d = make_device_buf(bx)    // the seam at ascale = 0.5, ln_on = 0
            let xb_half_d = make_device_buf(bx)
            let wd = make_device_buf(bw)
            let qd = make_device_buf(bq)
            let qpd = make_device_buf(bp)
            let qphd = make_device_buf(bph)
            let qcd = make_device_buf(bq)
            let bqkv = 3l * bq
            let btab = int64(rows * half) * 4l
            let qkvd = make_device_buf(bqkv)
            let cosd = make_device_buf(btab)
            let sind = make_device_buf(btab)
            let hostb = 11l * bx + bp + bph + bq + bqkv
            var host = make_host_buf(hostb, true, [cached = true])
            unsafe {
                upload_region_at(xd, 0l, addr<uint8 const?>(x[0]), bx)
                upload_region_at(x0d, 0l, addr<uint8 const?>(x[0]), bx)
                upload_region_at(x_half_d, 0l, addr<uint8 const?>(x[0]), bx)
                upload_region_at(yd, 0l, addr<uint8 const?>(y[0]), bx)
                upload_region_at(gd, 0l, addr<uint8 const?>(g[0]), bx)
                upload_region_at(g0d, 0l, addr<uint8 const?>(g[0]), bx)
                upload_region_at(gerf_d, 0l, addr<uint8 const?>(g[0]), bx)
                upload_region_at(gsilu_d, 0l, addr<uint8 const?>(g[0]), bx)
                upload_region_at(grelu_d, 0l, addr<uint8 const?>(g[0]), bx)
                upload_region_at(wd, 0l, addr<uint8 const?>(wn[0]), bw)
                upload_region_at(qd, 0l, addr<uint8 const?>(q[0]), bq)
                upload_region_at(qkvd, 0l, addr<uint8 const?>(qkv[0]), bqkv)
                upload_region_at(cosd, 0l, addr<uint8 const?>(cos_t[0]), btab)
                upload_region_at(sind, 0l, addr<uint8 const?>(sin_t[0]), btab)
            }
            nan_sentinel_upload(xbd, bx)
            nan_sentinel_upload(xb2d, bx)
            nan_sentinel_upload(xb0d, bx)
            nan_sentinel_upload(xb_half_d, bx)
            nan_sentinel_upload(qpd, bp)
            nan_sentinel_upload(qcd, bq)
            fill_upload(qphd, 0l, bph, F16_SENTINEL_BITS)
            var s_rope = set_tower_rope_tab_cls(fixed_array(qkvd, cosd, sind), fixed_array(bqkv, btab, btab), fixed_array(B_QKV, 0u, 0u))
            var pa_rope = TowerRopeTabArgs(sstride = uint(3 * d_compact), soff = uint(d_compact), nheads = uint(ROWS_HEADS), hs = uint(LN_HS), rows = uint(rows))
            var s_ln = set_tower_ln_cls(fixed_array(xd, xbd, wd), fixed_array(bx, bx, bw), fixed_array(B_X, B_XB, 0u))
            var s_bias = set_tower_bias_act_cls(fixed_array(gd, wd), fixed_array(bx, bw), fixed_array(B_G, 0u))
            var s_bias0 = set_tower_bias_act_cls(fixed_array(g0d, wd), fixed_array(bx, bw), fixed_array(B_G0, 0u))
            var s_bias_erf = set_tower_bias_act_cls(fixed_array(gerf_d, wd), fixed_array(bx, bw), fixed_array(B_GERF, 0u))
            var s_bias_silu = set_tower_bias_act_cls(fixed_array(gsilu_d, wd), fixed_array(bx, bw), fixed_array(B_GSILU, 0u))
            var s_bias_relu = set_tower_bias_act_cls(fixed_array(grelu_d, wd), fixed_array(bx, bw), fixed_array(B_GRELU, 0u))
            var s_post = set_tower_post_add_ln_cls(fixed_array(xd, yd, xb2d, wd), fixed_array(bx, bx, bx, bw), fixed_array(B_X, B_Y, B_XB2, 0u))
            var s_post0 = set_tower_post_add_ln_cls(fixed_array(x0d, yd, xb0d, wd), fixed_array(bx, bx, bx, bw), fixed_array(B_X0, B_Y, B_XB0, 0u))
            var s_post_half = set_tower_post_add_ln_cls(fixed_array(x_half_d, yd, xb_half_d, wd), fixed_array(bx, bx, bx, bw), fixed_array(B_XHALF, B_Y, B_XBHALF, 0u))
            var s_pad = set_tower_hpad_cls(fixed_array(qd, qpd), fixed_array(bq, bp), fixed_array(B_Q, B_QP))
            var s_pad16 = set_tower_hpad16_cls(fixed_array(qd, qphd), fixed_array(bq, bph), fixed_array(B_Q, B_QPH))
            var s_unpad = set_tower_hunpad_cls(fixed_array(qpd, qcd), fixed_array(bp, bq), fixed_array(B_QP, B_QC))
            var pa_ln = TowerLnArgs(d = uint(d), rows = uint(rows), woff = 0u, boff = uint(d), eps = ROWS_EPS)
            var pa_bias = TowerBiasArgs(d = uint(d), nelem = uint(nd), boff = uint(2 * d), act = BIAS_ACT_GELU_TANH)
            var pa_bias0 = TowerBiasArgs(d = uint(d), nelem = uint(nd), boff = uint(2 * d), act = BIAS_ACT_NONE)
            var pa_bias_erf = TowerBiasArgs(d = uint(d), nelem = uint(nd), boff = uint(2 * d), act = BIAS_ACT_GELU_ERF)
            var pa_bias_silu = TowerBiasArgs(d = uint(d), nelem = uint(nd), boff = uint(2 * d), act = BIAS_ACT_SILU)
            var pa_bias_relu = TowerBiasArgs(d = uint(d), nelem = uint(nd), boff = uint(2 * d), act = BIAS_ACT_RELU)
            var pa_post = TowerLnArgs(d = uint(d), rows = uint(rows), boff = uint(2 * d), eps = ROWS_EPS, ln_on = 1u, lnoff = uint(3 * d), lnboff = uint(d), ascale = 1.0)
            var pa_post0 = TowerLnArgs(d = uint(d), rows = uint(rows), boff = uint(2 * d), eps = ROWS_EPS, ln_on = 0u, lnoff = uint(3 * d), lnboff = uint(d), ascale = 1.0)
            var pa_post_half = TowerLnArgs(d = uint(d), rows = uint(rows), boff = uint(2 * d), eps = ROWS_EPS, ln_on = 0u, lnoff = uint(3 * d), lnboff = uint(d), ascale = 0.5)
            var pa_rs = TowerRestrideArgs(rows = uint(rows), rows_pad = uint(rows_pad), nheads = uint(ROWS_HEADS), hs = uint(LN_HS), hsp = uint(LN_HSP), sstride = uint(d_compact), soff = 0u)
            let wg4 = int64((nd / 4 + 255) / 256)
            let wg_rs = int64((rows_pad * d_padded + 255) / 256)   // the pads cover the 64-aligned cap: the rows past `rows` come out zero
            cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_tower_ln_cls(raw, h, s_ln, pa_ln, int64(rows))
                enc_tower_bias_act_cls(raw, h, s_bias, pa_bias, wg4)
                enc_tower_bias_act_cls(raw, h, s_bias0, pa_bias0, wg4)
                enc_tower_bias_act_cls(raw, h, s_bias_erf, pa_bias_erf, wg4)
                enc_tower_bias_act_cls(raw, h, s_bias_silu, pa_bias_silu, wg4)
                enc_tower_bias_act_cls(raw, h, s_bias_relu, pa_bias_relu, wg4)
                enc_tower_hpad_cls(raw, h, s_pad, pa_rs, wg_rs)
                enc_tower_hpad16_cls(raw, h, s_pad16, pa_rs, wg_rs)
                enc_tower_hunpad_cls(raw, h, s_unpad, pa_rs, wg_rs)
                enc_tower_rope_tab_cls(raw, h, s_rope, pa_rope, int64((rows * ROWS_HEADS * half + 255) / 256))
                enc_tower_post_add_ln_cls(raw, h, s_post0, pa_post0, int64(rows))
                enc_tower_post_add_ln_cls(raw, h, s_post_half, pa_post_half, int64(rows))
                enc_tower_post_add_ln_cls(raw, h, s_post, pa_post, int64(rows))   // last: it rewrites x, which the layernorm reads whole
                vhz_dep(raw, h, B_X | B_XB | B_G | B_XB2 | B_QP | B_QPH | B_QC | B_QKV | B_G0 | B_X0 | B_XB0 | B_GERF | B_GSILU | B_GRELU | B_XHALF, 0u, true)
                cmd_copy_range(raw, xbd, 0l, host.buf, 0l, bx)
                cmd_copy_range(raw, gd, 0l, host.buf, bx, bx)
                cmd_copy_range(raw, xd, 0l, host.buf, 2l * bx, bx)
                cmd_copy_range(raw, xb2d, 0l, host.buf, 3l * bx, bx)
                cmd_copy_range(raw, qpd, 0l, host.buf, 4l * bx, bp)
                cmd_copy_range(raw, qphd, 0l, host.buf, 4l * bx + bp, bph)
                cmd_copy_range(raw, qcd, 0l, host.buf, 4l * bx + bp + bph, bq)
                cmd_copy_range(raw, qkvd, 0l, host.buf, 4l * bx + bp + bph + bq, bqkv)
                cmd_copy_range(raw, g0d, 0l, host.buf, 4l * bx + bp + bph + bq + bqkv, bx)
                cmd_copy_range(raw, x0d, 0l, host.buf, 5l * bx + bp + bph + bq + bqkv, bx)
                cmd_copy_range(raw, xb0d, 0l, host.buf, 6l * bx + bp + bph + bq + bqkv, bx)
                cmd_copy_range(raw, gerf_d, 0l, host.buf, 7l * bx + bp + bph + bq + bqkv, bx)
                cmd_copy_range(raw, gsilu_d, 0l, host.buf, 8l * bx + bp + bph + bq + bqkv, bx)
                cmd_copy_range(raw, grelu_d, 0l, host.buf, 9l * bx + bp + bph + bq + bqkv, bx)
                cmd_copy_range(raw, x_half_d, 0l, host.buf, 10l * bx + bp + bph + bq + bqkv, bx)
            }
            var qkv_dev : array<float>
            qkv_dev |> resize(rows * 3 * d_compact)
            var xb_dev : array<float>
            var g_dev : array<float>
            var x_dev : array<float>
            var xb2_dev : array<float>
            var qp_dev : array<float>
            var qph_dev : array<uint16>
            var qc_dev : array<float>
            var g0_dev : array<float>
            var x0_dev : array<float>
            var xb0_dev : array<uint>
            var gerf_dev : array<float>
            var gsilu_dev : array<float>
            gerf_dev |> resize(nd)
            gsilu_dev |> resize(nd)
            xb_dev |> resize(nd)
            g_dev |> resize(nd)
            x_dev |> resize(nd)
            xb2_dev |> resize(nd)
            qp_dev |> resize(rows_pad * d_padded)
            qph_dev |> resize(rows_pad * d_padded)
            qc_dev |> resize(rows * d_compact)
            g0_dev |> resize(nd)
            x0_dev |> resize(nd)
            xb0_dev |> resize(nd)
            unsafe {
                let hp = intptr(host.mapped)
                memcpy(addr<void?>(xb_dev[0]), host.mapped, bx)
                memcpy(addr<void?>(g_dev[0]), reinterpret<void?>(hp + uint64(bx)), bx)
                memcpy(addr<void?>(x_dev[0]), reinterpret<void?>(hp + uint64(2l * bx)), bx)
                memcpy(addr<void?>(xb2_dev[0]), reinterpret<void?>(hp + uint64(3l * bx)), bx)
                memcpy(addr<void?>(qp_dev[0]), reinterpret<void?>(hp + uint64(4l * bx)), bp)
                memcpy(addr<void?>(qph_dev[0]), reinterpret<void?>(hp + uint64(4l * bx + bp)), bph)
                memcpy(addr<void?>(qc_dev[0]), reinterpret<void?>(hp + uint64(4l * bx + bp + bph)), bq)
                memcpy(addr<void?>(qkv_dev[0]), reinterpret<void?>(hp + uint64(4l * bx + bp + bph + bq)), bqkv)
                memcpy(addr<void?>(g0_dev[0]), reinterpret<void?>(hp + uint64(4l * bx + bp + bph + bq + bqkv)), bx)
                memcpy(addr<void?>(x0_dev[0]), reinterpret<void?>(hp + uint64(5l * bx + bp + bph + bq + bqkv)), bx)
                memcpy(addr<void?>(xb0_dev[0]), reinterpret<void?>(hp + uint64(6l * bx + bp + bph + bq + bqkv)), bx)
                memcpy(addr<void?>(gerf_dev[0]), reinterpret<void?>(hp + uint64(7l * bx + bp + bph + bq + bqkv)), bx)
                memcpy(addr<void?>(gsilu_dev[0]), reinterpret<void?>(hp + uint64(8l * bx + bp + bph + bq + bqkv)), bx)
            }
            let grelu_dev <- staged_f32(host.mapped, 9l * bx + bp + bph + bq + bqkv, nd)
            let xhalf_dev <- staged_f32(host.mapped, 10l * bx + bp + bph + bq + bqkv, nd)
            var xb_ref : array<float>
            xb_ref |> resize(nd)
            layernorm_batch(xb_ref, x, wn, 0l, int64(d), int64(d), int64(rows), ROWS_EPS)
            let bad_ln = mismatch_approx(xb_dev, xb_ref)
            t |> success(bad_ln == 0, "tower_ln: the layernorm rows match layernorm_batch ({bad_ln} of {nd} off)")
            t |> success(!approx(xb_dev[nd - 1], xb_ref[nd - 1] + 0.5), "tower_ln: the bar reds a poisoned element")
            var g_ref := g
            add_bias_rows(g_ref, wn, int64(2 * d), int64(d), int64(rows))
            let bad_bias0 = mismatch_approx(g0_dev, g_ref)
            t |> success(bad_bias0 == 0, "tower_bias_act BIAS_ACT_NONE: the bias alone matches add_bias_rows ({bad_bias0} of {nd} off)")
            t |> success(!approx(g0_dev[0], g_ref[0] + 0.5), "tower_bias_act BIAS_ACT_NONE: the bar reds a poisoned element")
            var gerf_ref := g_ref
            var gsilu_ref := g_ref
            let grelu_ref <- [for (v in g_ref); max(v, 0.0)]
            let bad_relu = mismatch_approx(grelu_dev, grelu_ref)
            t |> success(bad_relu == 0, "tower_bias_act BIAS_ACT_RELU: the bias and relu match max(x + b, 0) ({bad_relu} of {nd} off)")
            t |> success(!approx(grelu_dev[nd - 1], grelu_ref[nd - 1] + 0.5), "tower_bias_act BIAS_ACT_RELU: the bar reds a poisoned element")
            gelu_tanh_lut_batch(g_ref, int64(d), int64(rows))
            let bad_bias = mismatch_approx(g_dev, g_ref)
            t |> success(bad_bias == 0, "tower_bias_act: the bias and the tanh GELU match add_bias_rows + gelu_tanh_lut_batch ({bad_bias} of {nd} off)")
            t |> success(!approx(g_dev[0], g_ref[0] + 0.5), "tower_bias_act: the bar reds a poisoned element")
            // the erf arm evaluates the CPU's rational erfc in f32 where the CPU evaluates it in double: 1e-5 relative holds the
            // f32 rounding of the polynomial and the exp (a few ulp) and is well under the erf-vs-tanh gap (4.7e-4 at x = -2.7),
            // which the approx bar (2e-3) could not tell apart
            gelu_erf_batch(gerf_ref, int64(d), int64(rows))
            let erf_bar = 1e-5
            let bad_erf = mismatch_rel(gerf_dev, gerf_ref, erf_bar)
            t |> success(bad_erf == 0, "tower_bias_act BIAS_ACT_GELU_ERF: the bias and the erf GELU match add_bias_rows + gelu_erf_batch within {erf_bar} ({bad_erf} of {nd} off)")
            var gerf_poison := gerf_ref
            gerf_poison[0] += 0.5
            t |> success(mismatch_rel(gerf_dev, gerf_poison, erf_bar) > 0, "tower_bias_act BIAS_ACT_GELU_ERF: the bar reds a poisoned element")
            let tanh_off_erf = mismatch_rel(g_dev, gerf_ref, erf_bar)
            t |> success(tanh_off_erf > nd / 4, "tower_bias_act BIAS_ACT_GELU_ERF control: the tanh arm's rows miss the erf oracle at this bar ({tanh_off_erf} of {nd} off; the tails past +-10 agree by construction) - the bar tells the forms apart")
            silu(gsilu_ref, int64(nd))
            let bad_silu_act = mismatch_approx(gsilu_dev, gsilu_ref)
            t |> success(bad_silu_act == 0, "tower_bias_act BIAS_ACT_SILU: the bias and silu match add_bias_rows + silu ({bad_silu_act} of {nd} off)")
            t |> success(!approx(gsilu_dev[nd - 1], gsilu_ref[nd - 1] + 0.5), "tower_bias_act BIAS_ACT_SILU: the bar reds a poisoned element")
            t |> success(mismatch_approx(gsilu_dev, g_ref) > nd / 4, "tower_bias_act BIAS_ACT_SILU control: the silu arm is not the tanh GELU arm ({mismatch_approx(gsilu_dev, g_ref)} of {nd} apart; the positive tail agrees by construction)")
            var x_ref := x
            var yb := y
            add_bias_rows(yb, wn, int64(2 * d), int64(d), int64(rows))
            add_inplace_rows(x_ref, yb, int64(nd))
            let bad_add = mismatch_approx(x_dev, x_ref)
            t |> success(bad_add == 0, "tower_post_add_ln: x += y + b ({bad_add} of {nd} off)")
            t |> success(!approx(x_dev[nd - 1], x_ref[nd - 1] + 0.5), "tower_post_add_ln: the bar reds a poisoned element of x")
            var xb2_ref : array<float>
            xb2_ref |> resize(nd)
            layernorm_batch(xb2_ref, x_ref, wn, int64(3 * d), int64(d), int64(d), int64(rows), ROWS_EPS)
            let bad_ln2 = mismatch_approx(xb2_dev, xb2_ref)
            t |> success(bad_ln2 == 0, "tower_post_add_ln: the next layernorm over the updated rows ({bad_ln2} of {nd} off)")
            t |> success(!approx(xb2_dev[nd - 1], xb2_ref[nd - 1] + 0.5), "tower_post_add_ln: the bar reds a poisoned element")
            let bad_add0 = mismatch_approx(x0_dev, x_ref)
            t |> success(bad_add0 == 0, "tower_post_add_ln ln_on = 0: the seam alone lands the same x ({bad_add0} of {nd} off)")
            t |> equal(sentinel_words(xb0_dev), nd, "tower_post_add_ln ln_on = 0: the layernorm output stays untouched (every word the sentinel)")
            var xhalf_ref := x
            for (i in range(nd)) {
                xhalf_ref[i] += 0.5 * yb[i]
            }
            let bad_half = mismatch_approx(xhalf_dev, xhalf_ref)
            t |> success(bad_half == 0, "tower_post_add_ln ascale = 0.5: x += 0.5 . (y + b) ({bad_half} of {nd} off)")
            t |> success(!approx(xhalf_dev[nd - 1], xhalf_ref[nd - 1] + 0.5), "tower_post_add_ln ascale = 0.5: the bar reds a poisoned element")
            var bad_pad = 0
            var bad_pad16 = 0
            for (r in range(rows)) {
                for (hh in range(ROWS_HEADS)) {
                    for (lane in range(LN_HSP)) {
                        let pi = (r * ROWS_HEADS + hh) * LN_HSP + lane
                        let want = lane < LN_HS ? q[(r * ROWS_HEADS + hh) * LN_HS + lane] : 0.0
                        if (qp_dev[pi] != want) {
                            bad_pad++
                        }
                        if (uint(qph_dev[pi]) != f32_to_f16(want)) {
                            bad_pad16++
                        }
                    }
                }
            }
            t |> success(bad_pad == 0, "tower_hpad: the padded panel is the compact rows with zero pads, exactly ({bad_pad} off)")
            t |> success(qp_dev[0] != q[0] + 0.5, "tower_hpad: the bar reds a poisoned element")
            t |> success(bad_pad16 == 0, "tower_hpad16: the f16 panel is that panel rounded to f16, bit for bit ({bad_pad16} off)")
            t |> success(uint(qph_dev[0]) != f32_to_f16(q[0] + 0.5), "tower_hpad16: the bar reds a poisoned element")
            // the rows in [rows, rows_pad): the pads write them zero, so the flash tile's last 16-row group reads no stale shadow
            let tail_lo = rows * d_padded
            let tail_hi = rows_pad * d_padded
            let bad_tail = nonzero_in(qp_dev, tail_lo, tail_hi)
            let bad_tail16 = nonzero_halves_in(qph_dev, tail_lo, tail_hi)
            t |> success(rows_pad > rows && bad_tail == 0, "tower_hpad: the {rows_pad - rows} rows past the live count are zero ({bad_tail} off)")
            t |> success(bad_tail16 == 0, "tower_hpad16: the rows past the live count are zero in f16 ({bad_tail16} off)")
            var qp_poison := qp_dev
            var qph_poison := qph_dev
            qp_poison[tail_hi - 1] += 0.5
            qph_poison[tail_hi - 1] = uint16(f32_to_f16(0.5))
            t |> success(nonzero_in(qp_poison, tail_lo, tail_hi) > 0 && nonzero_halves_in(qph_poison, tail_lo, tail_hi) > 0, "tower_hpad / hpad16: the tail check reds a poisoned element")
            let bad_unpad = mismatch_exact(qc_dev, q)
            t |> success(bad_unpad == 0, "tower_hunpad: the compact rows come back off the padded panel exactly ({bad_unpad} off)")
            t |> success(qc_dev[0] != q[0] + 0.5, "tower_hunpad: the bar reds a poisoned element")
            var k_ref : array<float>
            var k_dev : array<float>
            k_ref |> resize(rows * d_compact)
            k_dev |> resize(rows * d_compact)
            var bad_slots = 0
            for (r in range(rows)) {
                for (i in range(d_compact)) {
                    k_ref[r * d_compact + i] = qkv[r * 3 * d_compact + d_compact + i]
                    k_dev[r * d_compact + i] = qkv_dev[r * 3 * d_compact + d_compact + i]
                    if (qkv_dev[r * 3 * d_compact + i] != qkv[r * 3 * d_compact + i] || qkv_dev[r * 3 * d_compact + 2 * d_compact + i] != qkv[r * 3 * d_compact + 2 * d_compact + i]) {
                        bad_slots++
                    }
                }
            }
            rope_neox_tab_rows(k_ref, int64(rows), int64(ROWS_HEADS), int64(LN_HS), cos_t, sin_t)
            let bad_rope = mismatch_approx(k_dev, k_ref)
            t |> success(bad_rope == 0, "tower_rope_tab: the k slot of the fused rows matches rope_neox_tab_rows ({bad_rope} of {rows * d_compact} off)")
            t |> success(bad_slots == 0, "tower_rope_tab: the q and v slots stay untouched ({bad_slots} moved)")
            t |> success(!approx(k_dev[0], k_ref[0] + 0.5), "tower_rope_tab: the bar reds a poisoned element")
            for (b in fixed_array(xd, xbd, yd, gd, g0d, gerf_d, gsilu_d, grelu_d, xb2d, x0d, xb0d, x_half_d, xb_half_d, wd, qd, qpd, qphd, qcd, qkvd, cosd, sind)) {
                destroy_device_buf(b)
            }
            destroy_host_buf(host)
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkt_tower_f16_feeds(t0 : T?) {   // nolint:STYLE038 — one flat gate over the six stamps: the fixture, one command buffer, the twins and their controls
    t0 |> run("the f16-feed stamps (ln16, post_add_ln16, hpad_b, hpad16_b, hunpad16, bias_act_cvt16) == their f32 stamps rounded to half") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tower_ln_cls() || !ensure_tower_ln16_cls() || !ensure_tower_post_add_ln_cls() || !ensure_tower_post_add_ln16_cls()
                    || !ensure_tower_hpad_b_cls() || !ensure_tower_hpad16_b_cls() || !ensure_tower_hunpad16_cls() || !ensure_tower_bias_act_cls()
                    || !ensure_tower_bias_act_cvt16_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let B_X = 1u
            let B_X2 = 2u
            let B_XB = 4u
            let B_XBH = 8u
            let B_Y = 16u
            let B_G = 32u
            let B_GH = 64u
            let B_Q = 128u
            let B_QP = 256u
            let B_QPH = 512u
            let B_QCH = 1024u
            let B_XB2 = 2048u
            let B_XB2H = 4096u
            let B_GE = 8192u
            let B_GHE = 16384u
            let B_GR = 32768u
            let B_GHR = 65536u
            let B_X16Z = 131072u
            let B_XB2H0 = 262144u
            let d = ROWS_D
            let rows = ROWS_N
            let rows_pad = (rows + 63) / 64 * 64
            let nd = rows * d
            let d_compact = ROWS_HEADS * LN_HS
            let d_padded = ROWS_HEADS * LN_HSP
            var x <- row_fill(nd, 41, 7, 0.05, 2.1)
            var y <- row_fill(nd, 17, 3, 0.04, 1.9)
            var g <- row_fill(nd, 13, 5, 0.3, 14.0)
            var q <- row_fill(rows * d_compact, 29, 11, 0.06, 2.8)
            var wn <- row_fill(4 * d, 7, 1, 0.01, -0.6)   // ln w at 0, ln b at d, the bias at 2d (the q bias reads its first d_compact entries), the next ln w at 3d
            let bx = int64(nd) * 4l
            let bxh = int64(nd) * 2l
            let bw = int64(4 * d) * 4l
            let bq = int64(rows * d_compact) * 4l
            let bqh = int64(rows * d_compact) * 2l
            let bp = int64(rows_pad * d_padded) * 4l
            let bph = int64(rows_pad * d_padded) * 2l
            let xd = make_device_buf(bx)
            let x_post16_d = make_device_buf(bx)    // the f16 post-add's own x: both post-adds update x in place from the same start
            let x_post16z_d = make_device_buf(bx)   // the f16 post-add at ln_on = 0: the seam alone
            let xb2h0d = make_device_buf(bxh)       // ... and its half pre-norm output, which must stay untouched
            let gerf_d = make_device_buf(bx)        // the erf arm: the f32 stamp in place, the f16 stamp reading it first
            let gherf_d = make_device_buf(bxh)
            let grelu_d = make_device_buf(bx)       // the relu arm, the same pair
            let ghrelu_d = make_device_buf(bxh)
            let xbd = make_device_buf(bx)
            let xbhd = make_device_buf(bxh)
            let yd = make_device_buf(bx)
            let gd = make_device_buf(bx)
            let ghd = make_device_buf(bxh)
            let xb2d = make_device_buf(bx)
            let xb2hd = make_device_buf(bxh)
            let wd = make_device_buf(bw)
            let qd = make_device_buf(bq)
            let qpd = make_device_buf(bp)
            let qphd = make_device_buf(bph)
            let qchd = make_device_buf(bqh)
            let o_gerf = 2l * bx + 3l * bxh + bp + bph + bqh
            let o_gherf = o_gerf + bx
            let o_grelu = o_gherf + bxh
            let o_ghrelu = o_grelu + bx
            let o_x16z = o_ghrelu + bxh
            let o_xb2h0 = o_x16z + bx
            var host = make_host_buf(o_xb2h0 + bxh, true, [cached = true])
            unsafe {
                upload_region_at(xd, 0l, addr<uint8 const?>(x[0]), bx)
                upload_region_at(x_post16_d, 0l, addr<uint8 const?>(x[0]), bx)
                upload_region_at(x_post16z_d, 0l, addr<uint8 const?>(x[0]), bx)
                upload_region_at(yd, 0l, addr<uint8 const?>(y[0]), bx)
                upload_region_at(gd, 0l, addr<uint8 const?>(g[0]), bx)
                upload_region_at(gerf_d, 0l, addr<uint8 const?>(g[0]), bx)
                upload_region_at(grelu_d, 0l, addr<uint8 const?>(g[0]), bx)
                upload_region_at(wd, 0l, addr<uint8 const?>(wn[0]), bw)
                upload_region_at(qd, 0l, addr<uint8 const?>(q[0]), bq)
            }
            nan_sentinel_upload(xbd, bx)
            nan_sentinel_upload(xb2d, bx)
            nan_sentinel_upload(qpd, bp)
            fill_upload(xbhd, 0l, bxh, F16_SENTINEL_BITS)
            fill_upload(xb2hd, 0l, bxh, F16_SENTINEL_BITS)
            fill_upload(xb2h0d, 0l, bxh, F16_SENTINEL_BITS)
            fill_upload(ghd, 0l, bxh, F16_SENTINEL_BITS)
            fill_upload(gherf_d, 0l, bxh, F16_SENTINEL_BITS)
            fill_upload(ghrelu_d, 0l, bxh, F16_SENTINEL_BITS)
            fill_upload(qphd, 0l, bph, F16_SENTINEL_BITS)
            fill_upload(qchd, 0l, bqh, F16_SENTINEL_BITS)
            var s_ln = set_tower_ln_cls(fixed_array(xd, xbd, wd), fixed_array(bx, bx, bw), fixed_array(B_X, B_XB, 0u))
            var s_ln16 = set_tower_ln16_cls(fixed_array(xd, wd, xbhd), fixed_array(bx, bw, bxh), fixed_array(B_X, 0u, B_XBH))
            var s_bias = set_tower_bias_act_cls(fixed_array(gd, wd), fixed_array(bx, bw), fixed_array(B_G, 0u))
            var s_bias16 = set_tower_bias_act_cvt16_cls(fixed_array(gd, wd, ghd), fixed_array(bx, bw, bxh), fixed_array(B_G, 0u, B_GH))
            var s_bias_erf = set_tower_bias_act_cls(fixed_array(gerf_d, wd), fixed_array(bx, bw), fixed_array(B_GE, 0u))
            var s_bias16_erf = set_tower_bias_act_cvt16_cls(fixed_array(gerf_d, wd, gherf_d), fixed_array(bx, bw, bxh), fixed_array(B_GE, 0u, B_GHE))
            var s_bias_relu = set_tower_bias_act_cls(fixed_array(grelu_d, wd), fixed_array(bx, bw), fixed_array(B_GR, 0u))
            var s_bias16_relu = set_tower_bias_act_cvt16_cls(fixed_array(grelu_d, wd, ghrelu_d), fixed_array(bx, bw, bxh), fixed_array(B_GR, 0u, B_GHR))
            var s_post = set_tower_post_add_ln_cls(fixed_array(xd, yd, xb2d, wd), fixed_array(bx, bx, bx, bw), fixed_array(B_X, B_Y, B_XB2, 0u))
            var s_post16 = set_tower_post_add_ln16_cls(fixed_array(x_post16_d, yd, wd, xb2hd), fixed_array(bx, bx, bw, bxh), fixed_array(B_X2, B_Y, 0u, B_XB2H))
            var s_post16z = set_tower_post_add_ln16_cls(fixed_array(x_post16z_d, yd, wd, xb2h0d), fixed_array(bx, bx, bw, bxh), fixed_array(B_X16Z, B_Y, 0u, B_XB2H0))
            var s_padb = set_tower_hpad_b_cls(fixed_array(qd, qpd, wd), fixed_array(bq, bp, bw), fixed_array(B_Q, B_QP, 0u))
            var s_pad16b = set_tower_hpad16_b_cls(fixed_array(qd, qphd, wd), fixed_array(bq, bph, bw), fixed_array(B_Q, B_QPH, 0u))
            var s_unpad16 = set_tower_hunpad16_cls(fixed_array(qpd, qchd), fixed_array(bp, bqh), fixed_array(B_QP, B_QCH))
            var pa_ln = TowerLnArgs(d = uint(d), rows = uint(rows), woff = 0u, boff = uint(d), eps = ROWS_EPS)
            var pa_bias = TowerBiasArgs(d = uint(d), nelem = uint(nd), boff = uint(2 * d), act = BIAS_ACT_GELU_TANH)
            var pa_bias_erf = TowerBiasArgs(d = uint(d), nelem = uint(nd), boff = uint(2 * d), act = BIAS_ACT_GELU_ERF)
            var pa_bias_relu = TowerBiasArgs(d = uint(d), nelem = uint(nd), boff = uint(2 * d), act = BIAS_ACT_RELU)
            var pa_post = TowerLnArgs(d = uint(d), rows = uint(rows), boff = uint(2 * d), eps = ROWS_EPS, ln_on = 1u, lnoff = uint(3 * d), lnboff = uint(d), ascale = 1.0)
            var pa_post0 = TowerLnArgs(d = uint(d), rows = uint(rows), boff = uint(2 * d), eps = ROWS_EPS, ln_on = 0u, lnoff = uint(3 * d), lnboff = uint(d), ascale = 1.0)
            var pa_rs = TowerRestrideArgs(rows = uint(rows), rows_pad = uint(rows_pad), nheads = uint(ROWS_HEADS), hs = uint(LN_HS), hsp = uint(LN_HSP),
                sstride = uint(d_compact), soff = 0u, boff = uint(2 * d))
            let wg4 = int64((nd / 4 + 255) / 256)
            let wg_rs = int64((rows_pad * d_padded + 255) / 256)
            cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_tower_ln_cls(raw, h, s_ln, pa_ln, int64(rows))
                enc_tower_ln16_cls(raw, h, s_ln16, pa_ln, int64(rows))
                enc_tower_bias_act_cvt16_cls(raw, h, s_bias16, pa_bias, wg4)   // reads g before the in-place stamp rewrites it
                enc_tower_bias_act_cls(raw, h, s_bias, pa_bias, wg4)
                enc_tower_bias_act_cvt16_cls(raw, h, s_bias16_erf, pa_bias_erf, wg4)
                enc_tower_bias_act_cls(raw, h, s_bias_erf, pa_bias_erf, wg4)
                enc_tower_bias_act_cvt16_cls(raw, h, s_bias16_relu, pa_bias_relu, wg4)
                enc_tower_bias_act_cls(raw, h, s_bias_relu, pa_bias_relu, wg4)
                enc_tower_hpad_b_cls(raw, h, s_padb, pa_rs, wg_rs)
                enc_tower_hpad16_b_cls(raw, h, s_pad16b, pa_rs, wg_rs)
                enc_tower_hunpad16_cls(raw, h, s_unpad16, pa_rs, wg_rs)
                enc_tower_post_add_ln_cls(raw, h, s_post, pa_post, int64(rows))   // last on x: the layernorms read x whole
                enc_tower_post_add_ln16_cls(raw, h, s_post16, pa_post, int64(rows))
                enc_tower_post_add_ln16_cls(raw, h, s_post16z, pa_post0, int64(rows))
                vhz_dep(raw, h, B_X | B_X2 | B_XB | B_XBH | B_G | B_GH | B_QP | B_QPH | B_QCH | B_XB2 | B_XB2H | B_GE | B_GHE | B_GR | B_GHR | B_X16Z | B_XB2H0, 0u, true)
                cmd_copy_range(raw, xbd, 0l, host.buf, 0l, bx)
                cmd_copy_range(raw, xbhd, 0l, host.buf, bx, bxh)
                cmd_copy_range(raw, gd, 0l, host.buf, bx + bxh, bx)
                cmd_copy_range(raw, ghd, 0l, host.buf, 2l * bx + bxh, bxh)
                cmd_copy_range(raw, xb2hd, 0l, host.buf, 2l * bx + 2l * bxh, bxh)
                cmd_copy_range(raw, qpd, 0l, host.buf, 2l * bx + 3l * bxh, bp)
                cmd_copy_range(raw, qphd, 0l, host.buf, 2l * bx + 3l * bxh + bp, bph)
                cmd_copy_range(raw, qchd, 0l, host.buf, 2l * bx + 3l * bxh + bp + bph, bqh)
                cmd_copy_range(raw, gerf_d, 0l, host.buf, o_gerf, bx)
                cmd_copy_range(raw, gherf_d, 0l, host.buf, o_gherf, bxh)
                cmd_copy_range(raw, grelu_d, 0l, host.buf, o_grelu, bx)
                cmd_copy_range(raw, ghrelu_d, 0l, host.buf, o_ghrelu, bxh)
                cmd_copy_range(raw, x_post16z_d, 0l, host.buf, o_x16z, bx)
                cmd_copy_range(raw, xb2h0d, 0l, host.buf, o_xb2h0, bxh)
            }
            var xb_dev : array<float>
            var xbh_dev : array<uint16>
            var g_dev : array<float>
            var gh_dev : array<uint16>
            var xb2h_dev : array<uint16>
            var qp_dev : array<float>
            var qph_dev : array<uint16>
            var qch_dev : array<uint16>
            xb_dev |> resize(nd)
            xbh_dev |> resize(nd)
            g_dev |> resize(nd)
            gh_dev |> resize(nd)
            xb2h_dev |> resize(nd)
            qp_dev |> resize(rows_pad * d_padded)
            qph_dev |> resize(rows_pad * d_padded)
            qch_dev |> resize(rows * d_compact)
            unsafe {
                let hp = intptr(host.mapped)
                memcpy(addr<void?>(xb_dev[0]), host.mapped, bx)
                memcpy(addr<void?>(xbh_dev[0]), reinterpret<void?>(hp + uint64(bx)), bxh)
                memcpy(addr<void?>(g_dev[0]), reinterpret<void?>(hp + uint64(bx + bxh)), bx)
                memcpy(addr<void?>(gh_dev[0]), reinterpret<void?>(hp + uint64(2l * bx + bxh)), bxh)
                memcpy(addr<void?>(xb2h_dev[0]), reinterpret<void?>(hp + uint64(2l * bx + 2l * bxh)), bxh)
                memcpy(addr<void?>(qp_dev[0]), reinterpret<void?>(hp + uint64(2l * bx + 3l * bxh)), bp)
                memcpy(addr<void?>(qph_dev[0]), reinterpret<void?>(hp + uint64(2l * bx + 3l * bxh + bp)), bph)
                memcpy(addr<void?>(qch_dev[0]), reinterpret<void?>(hp + uint64(2l * bx + 3l * bxh + bp + bph)), bqh)
            }
            let gerf_dev <- staged_f32(host.mapped, o_gerf, nd)
            let gherf_dev <- staged_u16(host.mapped, o_gherf, nd)
            let grelu_dev <- staged_f32(host.mapped, o_grelu, nd)
            let ghrelu_dev <- staged_u16(host.mapped, o_ghrelu, nd)
            let x16z_dev <- staged_f32(host.mapped, o_x16z, nd)
            let xb2h0_dev <- staged_u32(host.mapped, o_xb2h0, nd / 2)
            var bad_ln16 = 0
            var bad_bias16 = 0
            var bad_post16 = 0
            for (i in range(nd)) {
                if (uint(xbh_dev[i]) != f32_to_f16(xb_dev[i])) {
                    bad_ln16++
                }
                if (uint(gh_dev[i]) != f32_to_f16(g_dev[i])) {
                    bad_bias16++
                }
            }
            t |> success(bad_ln16 == 0, "tower_ln16: the f16 rows are the f32 stamp's rows rounded to half, bit for bit ({bad_ln16} of {nd} off)")
            t |> success(uint(xbh_dev[0]) != f32_to_f16(xb_dev[0] + 0.5), "tower_ln16: the bar reds a poisoned element")
            var xb_ref : array<float>
            xb_ref |> resize(nd)
            layernorm_batch(xb_ref, x, wn, 0l, int64(d), int64(d), int64(rows), ROWS_EPS)
            var xbh_f : array<float>
            xbh_f |> resize(nd)
            for (i in range(nd)) {
                xbh_f[i] = f16_to_f32(uint(xbh_dev[i]))
            }
            let bad_ln_ref = mismatch_approx(xbh_f, xb_ref)
            t |> success(bad_ln_ref == 0, "tower_ln16: the half rows match layernorm_batch at the approx bar ({bad_ln_ref} of {nd} off)")
            t |> success(!approx(xbh_f[nd - 1], xb_ref[nd - 1] + 0.5), "tower_ln16 vs layernorm_batch: the bar reds a poisoned element")
            t |> success(bad_bias16 == 0, "tower_bias_act_cvt16: the f16 rows are the in-place stamp's rows rounded to half, bit for bit ({bad_bias16} of {nd} off)")
            t |> success(uint(gh_dev[0]) != f32_to_f16(g_dev[0] + 0.5), "tower_bias_act_cvt16: the bar reds a poisoned element")
            let bad_erf16 = halves_off(gherf_dev, gerf_dev)
            t |> success(bad_erf16 == 0, "tower_bias_act_cvt16 BIAS_ACT_GELU_ERF: the f16 rows are the in-place stamp's rows rounded to half, bit for bit ({bad_erf16} of {nd} off)")
            t |> success(uint(gherf_dev[0]) != f32_to_f16(gerf_dev[0] + 0.5), "tower_bias_act_cvt16 BIAS_ACT_GELU_ERF: the bar reds a poisoned element")
            let bad_relu16 = halves_off(ghrelu_dev, grelu_dev)
            t |> success(bad_relu16 == 0, "tower_bias_act_cvt16 BIAS_ACT_RELU: the f16 rows are the in-place stamp's rows rounded to half, bit for bit ({bad_relu16} of {nd} off)")
            t |> success(uint(ghrelu_dev[0]) != f32_to_f16(grelu_dev[0] + 0.5), "tower_bias_act_cvt16 BIAS_ACT_RELU: the bar reds a poisoned element")
            var x_ref := x
            var yb := y
            add_bias_rows(yb, wn, int64(2 * d), int64(d), int64(rows))
            add_inplace_rows(x_ref, yb, int64(nd))
            let bad_add16z = mismatch_approx(x16z_dev, x_ref)
            t |> success(bad_add16z == 0, "tower_post_add_ln16 ln_on = 0: the seam alone lands x += y + b ({bad_add16z} of {nd} off)")
            t |> success(!approx(x16z_dev[nd - 1], x_ref[nd - 1] + 0.5), "tower_post_add_ln16 ln_on = 0: the bar reds a poisoned element")
            t |> equal(sentinel_words(xb2h0_dev, F16_SENTINEL_BITS), nd / 2, "tower_post_add_ln16 ln_on = 0: the half pre-norm output stays untouched (every word the sentinel)")
            var xb2_ref : array<float>
            xb2_ref |> resize(nd)
            layernorm_batch(xb2_ref, x_ref, wn, int64(3 * d), int64(d), int64(d), int64(rows), ROWS_EPS)
            var xb2h_f : array<float>
            xb2h_f |> resize(nd)
            for (i in range(nd)) {
                xb2h_f[i] = f16_to_f32(uint(xb2h_dev[i]))
                if (uint(xb2h_dev[i]) != f32_to_f16(xb2_ref[i]) && !approx(xb2h_f[i], xb2_ref[i])) {
                    bad_post16++
                }
            }
            t |> success(bad_post16 == 0, "tower_post_add_ln16: the next layernorm's half rows match the CPU seam + layernorm_batch ({bad_post16} of {nd} off)")
            t |> success(!approx(xb2h_f[nd - 1], xb2_ref[nd - 1] + 0.5), "tower_post_add_ln16: the bar reds a poisoned element")
            var bad_padb = 0
            var bad_pad16b = 0
            var bad_unpad16 = 0
            for (r in range(rows)) {
                for (hh in range(ROWS_HEADS)) {
                    for (lane in range(LN_HSP)) {
                        let pi = (r * ROWS_HEADS + hh) * LN_HSP + lane
                        let ci = (r * ROWS_HEADS + hh) * LN_HS + lane
                        let want = lane < LN_HS ? q[ci] + wn[2 * d + hh * LN_HS + lane] : 0.0
                        if (qp_dev[pi] != want) {
                            bad_padb++
                        }
                        if (uint(qph_dev[pi]) != f32_to_f16(want)) {
                            bad_pad16b++
                        }
                        if (lane < LN_HS && uint(qch_dev[ci]) != f32_to_f16(qp_dev[pi])) {
                            bad_unpad16++
                        }
                    }
                }
            }
            t |> success(bad_padb == 0, "tower_hpad_b: the padded panel is the compact rows plus the bias row, exactly ({bad_padb} off)")
            t |> success(qp_dev[0] != q[0] + wn[2 * d] + 0.5, "tower_hpad_b: the bar reds a poisoned element")
            t |> success(bad_pad16b == 0, "tower_hpad16_b: the f16 panel is that panel rounded to half, bit for bit ({bad_pad16b} off)")
            t |> success(uint(qph_dev[0]) != f32_to_f16(q[0] + wn[2 * d] + 0.5), "tower_hpad16_b: the bar reds a poisoned element")
            t |> success(bad_unpad16 == 0, "tower_hunpad16: the compact rows come back off the padded panel as half, bit for bit ({bad_unpad16} off)")
            t |> success(uint(qch_dev[0]) != f32_to_f16(qp_dev[0] + 0.5), "tower_hunpad16: the bar reds a poisoned element")
            let tail_lo = rows * d_padded
            let tail_hi = rows_pad * d_padded
            let bad_tail16 = nonzero_in(qp_dev, tail_lo, tail_hi) + nonzero_halves_in(qph_dev, tail_lo, tail_hi)
            t |> success(rows_pad > rows && bad_tail16 == 0, "tower_hpad_b / hpad16_b: the {rows_pad - rows} rows past the live count are zero ({bad_tail16} off)")
            var qp_poison := qp_dev
            var qph_poison := qph_dev
            qp_poison[tail_hi - 1] += 0.5
            qph_poison[tail_hi - 1] = uint16(f32_to_f16(0.5))
            t |> success(nonzero_in(qp_poison, tail_lo, tail_hi) > 0 && nonzero_halves_in(qph_poison, tail_lo, tail_hi) > 0, "tower_hpad_b / hpad16_b: the tail check reds a poisoned element")
            for (b in fixed_array(xd, x_post16_d, x_post16z_d, xb2h0d, gerf_d, gherf_d, grelu_d, ghrelu_d, xbd, xbhd, yd, gd, ghd, xb2d, xb2hd, wd, qd, qpd, qphd, qchd)) {
                destroy_device_buf(b)
            }
            destroy_host_buf(host)
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

let private WD_HEADS = 3
let private WD_TSTRIDE = 300   //! the K/V planes' positions a head: two 256-key chunks, the second partial
let private WD_P0 = 290        //! the batch's first position: its rows attend past the chunk boundary
let private WD_ROWS = 3

//! `dec_self_attn` / `dec_cross_attn` over the driver's f16 planes with the q bias folded in; causal row r attends len0 + r keys
def private wdec_attn_ref(q : array<float>; kh, vh : array<uint16>; wn : array<float>; rows, dim, ydim, heads, hs, tstride, len0, qboff : int; causal : bool; scale : float) : array<float> {
    var y : array<float>
    y |> resize(rows * ydim)
    var sc : array<float>
    sc |> resize(tstride)
    for (r in range(rows)) {
        let len = causal ? len0 + r : len0
        for (h in range(heads)) {
            var mx = -3.0e38
            for (tt in range(len)) {
                var s = 0.0
                for (j in range(hs)) {
                    s += (q[r * dim + h * hs + j] + wn[qboff + h * hs + j]) * f16_to_f32(uint(kh[(h * tstride + tt) * hs + j]))
                }
                sc[tt] = s * scale
                mx = max(mx, sc[tt])
            }
            var den = 0.0
            for (tt in range(len)) {
                sc[tt] = exp(sc[tt] - mx)
                den += sc[tt]
            }
            for (j in range(hs)) {
                var acc = 0.0
                for (tt in range(len)) {
                    acc += sc[tt] * f16_to_f32(uint(vh[(h * tstride + tt) * hs + j]))
                }
                y[r * ydim + h * hs + j] = acc / den
            }
        }
    }
    return <- y
}

//! the halves of an f16 [h][t][hs] plane at positions `from_t` and past that no longer carry the sentinel
def private wdec_tail_off(kh : array<uint16>; heads, tstride, hs, from_t : int) : int {
    var n = 0
    for (h in range(heads)) {
        for (tt in range(from_t, tstride)) {
            for (j in range(hs)) {
                if (uint(kh[(h * tstride + tt) * hs + j]) != (F16_SENTINEL_BITS & 0xFFFFu)) {
                    n++
                }
            }
        }
    }
    return n
}

[test]
def test_vkt_wdec_classes(t0 : T?) {   // nolint:STYLE038 — one flat gate over the three classes: the fixture, one command buffer, the oracles and their controls
    t0 |> run("the whisper decoder's K/V store and chunked attention pair == the CPU decoder forms") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tower_wdec_kv_cls() || !ensure_tower_wdec_attn_part_cls() || !ensure_tower_wdec_attn_comb_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let B_SRC = 1u
            let B_K = 2u
            let B_V = 4u
            let B_KX = 8u
            let B_VX = 16u
            let B_PMS = 32u
            let B_Y = 64u
            let B_YC = 128u
            let hs = int(WDEC_HS)
            let heads = WD_HEADS
            let d = heads * hs
            let dim = 3 * d
            let tstride = WD_TSTRIDE
            let rows = WD_ROWS
            let p0 = WD_P0
            let scale = 1.0 / sqrt(float(hs))
            let kscale = 1.0 / sqrt(sqrt(float(hs)))
            var src <- row_fill(rows * dim, 31, 5, 0.05, 2.3)         // the batch's fused [q | k | v] rows
            var hist <- row_fill(p0 * dim, 23, 7, 0.04, 1.7)          // the positions before the batch, the same row shape
            var wn <- row_fill(2 * d, 11, 3, 0.02, 0.9)               // the q bias row at 0, the v bias row at d
            let bsrc = int64(rows * dim) * 4l
            let bhist = int64(p0 * dim) * 4l
            let bw = int64(2 * d) * 4l
            let bkv = int64(heads * tstride * hs) * 2l
            let bkvf = int64(heads * tstride * hs) * 4l
            let nch = (tstride + int(WDEC_CHUNK) - 1) / int(WDEC_CHUNK)
            let bpms = int64(nch * heads * rows * 2) * 4l
            let bpy = int64(nch * heads * rows * hs) * 4l
            let by = int64(rows * d) * 4l
            let srcd = make_device_buf(bsrc)
            let histd = make_device_buf(bhist)
            let wd = make_device_buf(bw)
            let kd = make_device_buf(bkv)
            let vd = make_device_buf(bkv)
            let kxd = make_device_buf(bkvf)
            let vxd = make_device_buf(bkvf)
            let pmsd = make_device_buf(bpms)
            let pyd = make_device_buf(bpy)
            nan_sentinel_upload(pmsd, bpms)
            nan_sentinel_upload(pyd, bpy)
            let yd = make_device_buf(by)
            let ycd = make_device_buf(by)
            let hostb = 2l * bkv + 2l * bkvf + 2l * by
            var host = make_host_buf(hostb, true, [cached = true])
            unsafe {
                upload_region_at(srcd, 0l, addr<uint8 const?>(src[0]), bsrc)
                upload_region_at(histd, 0l, addr<uint8 const?>(hist[0]), bhist)
                upload_region_at(wd, 0l, addr<uint8 const?>(wn[0]), bw)
            }
            fill_upload(kd, 0l, bkv, F16_SENTINEL_BITS)
            fill_upload(vd, 0l, bkv, F16_SENTINEL_BITS)
            nan_sentinel_upload(kxd, bkvf)
            nan_sentinel_upload(vxd, bkvf)
            nan_sentinel_upload(yd, by)
            nan_sentinel_upload(ycd, by)
            var s_kh = set_tower_wdec_kv_cls(fixed_array(histd, kd, kxd, wd), fixed_array(bhist, bkv, bkvf, bw), fixed_array(B_SRC, B_K, B_KX, 0u))
            var s_vh = set_tower_wdec_kv_cls(fixed_array(histd, vd, vxd, wd), fixed_array(bhist, bkv, bkvf, bw), fixed_array(B_SRC, B_V, B_VX, 0u))
            var s_kb = set_tower_wdec_kv_cls(fixed_array(srcd, kd, kxd, wd), fixed_array(bsrc, bkv, bkvf, bw), fixed_array(B_SRC, B_K, B_KX, 0u))
            var s_vb = set_tower_wdec_kv_cls(fixed_array(srcd, vd, vxd, wd), fixed_array(bsrc, bkv, bkvf, bw), fixed_array(B_SRC, B_V, B_VX, 0u))
            var s_attn = set_tower_wdec_attn_part_cls(fixed_array(srcd, kd, vd, pmsd, pyd, wd), fixed_array(bsrc, bkv, bkv, bpms, bpy, bw), fixed_array(B_SRC, B_K, B_V, B_PMS, B_PMS, 0u))
            var s_comb = set_tower_wdec_attn_comb_cls(fixed_array(pmsd, pyd, yd), fixed_array(bpms, bpy, by), fixed_array(B_PMS, B_PMS, B_Y))
            var s_combc = set_tower_wdec_attn_comb_cls(fixed_array(pmsd, pyd, ycd), fixed_array(bpms, bpy, by), fixed_array(B_PMS, B_PMS, B_YC))
            var pa_kh = TowerWdecKvArgs(rows = uint(p0), sstride = uint(dim), soff = uint(d), nheads = uint(heads), tstride = uint(tstride), p0 = 0u,
                dst_off = 0u, boff = 0u, bias_on = 0u, mode = WDEC_KV_F16)
            var pa_vh = pa_kh
            pa_vh.soff = uint(2 * d)
            pa_vh.bias_on = 1u
            pa_vh.boff = uint(d)
            var pa_kb = pa_kh
            pa_kb.rows = uint(rows)
            pa_kb.p0 = uint(p0)
            var pa_vb = pa_vh
            pa_vb.rows = uint(rows)
            pa_vb.p0 = uint(p0)
            var pa_kx = pa_kb
            pa_kx.mode = WDEC_KV_KX
            var pa_vx = pa_vb
            pa_vx.mode = WDEC_KV_VX
            var pa_as = TowerWdecAttnArgs(dim = uint(dim), tstride = uint(tstride), len0 = uint(p0 + 1), causal = 1u, nheads = uint(heads),
                nrows = uint(rows), koff = 0u, voff = 0u, qboff = 0u)
            let len_cross = 150   // inside the first chunk, so the cross rows reach the combine with the second chunk's partial empty
            var pa_ac = pa_as
            pa_ac.len0 = uint(len_cross)
            pa_ac.causal = 0u
            var pa_c = TowerWdecCombArgs(dim = uint(d), nchunks = uint(nch), nheads = uint(heads), nrows = uint(rows))
            let wg_h = int64((p0 * d + 255) / 256)
            let wg_b = int64((rows * d + 255) / 256)
            let wg_attn = int64(nch * heads * rows)
            let wg_comb = int64(heads * rows)
            cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_tower_wdec_kv_cls(raw, h, s_kh, pa_kh, wg_h)
                enc_tower_wdec_kv_cls(raw, h, s_vh, pa_vh, wg_h)
                enc_tower_wdec_kv_cls(raw, h, s_kb, pa_kb, wg_b)
                enc_tower_wdec_kv_cls(raw, h, s_vb, pa_vb, wg_b)
                enc_tower_wdec_kv_cls(raw, h, s_kb, pa_kx, wg_b)
                enc_tower_wdec_kv_cls(raw, h, s_vb, pa_vx, wg_b)
                enc_tower_wdec_attn_part_cls(raw, h, s_attn, pa_as, wg_attn)
                enc_tower_wdec_attn_comb_cls(raw, h, s_comb, pa_c, wg_comb)
                enc_tower_wdec_attn_part_cls(raw, h, s_attn, pa_ac, wg_attn)
                enc_tower_wdec_attn_comb_cls(raw, h, s_combc, pa_c, wg_comb)
                vhz_dep(raw, h, B_K | B_V | B_KX | B_VX | B_Y | B_YC, 0u, true)
                cmd_copy_range(raw, kd, 0l, host.buf, 0l, bkv)
                cmd_copy_range(raw, vd, 0l, host.buf, bkv, bkv)
                cmd_copy_range(raw, kxd, 0l, host.buf, 2l * bkv, bkvf)
                cmd_copy_range(raw, vxd, 0l, host.buf, 2l * bkv + bkvf, bkvf)
                cmd_copy_range(raw, yd, 0l, host.buf, 2l * bkv + 2l * bkvf, by)
                cmd_copy_range(raw, ycd, 0l, host.buf, 2l * bkv + 2l * bkvf + by, by)
            }
            var k_dev : array<uint16>
            var v_dev : array<uint16>
            var kx_dev : array<float>
            var vx_dev : array<float>
            var y_dev : array<float>
            var yc_dev : array<float>
            k_dev |> resize(heads * tstride * hs)
            v_dev |> resize(heads * tstride * hs)
            kx_dev |> resize(heads * tstride * hs)
            vx_dev |> resize(heads * tstride * hs)
            y_dev |> resize(rows * d)
            yc_dev |> resize(rows * d)
            unsafe {
                let hp = intptr(host.mapped)
                memcpy(addr<void?>(k_dev[0]), host.mapped, bkv)
                memcpy(addr<void?>(v_dev[0]), reinterpret<void?>(hp + uint64(bkv)), bkv)
                memcpy(addr<void?>(kx_dev[0]), reinterpret<void?>(hp + uint64(2l * bkv)), bkvf)
                memcpy(addr<void?>(vx_dev[0]), reinterpret<void?>(hp + uint64(2l * bkv + bkvf)), bkvf)
                memcpy(addr<void?>(y_dev[0]), reinterpret<void?>(hp + uint64(2l * bkv + 2l * bkvf)), by)
                memcpy(addr<void?>(yc_dev[0]), reinterpret<void?>(hp + uint64(2l * bkv + 2l * bkvf + by)), by)
            }
            let len_self = p0 + rows
            var bad_k = 0
            var bad_v = 0
            var bad_kx = 0
            var bad_vx = 0
            for (h in range(heads)) {
                for (tt in range(tstride)) {
                    for (j in range(hs)) {
                        let pi = (h * tstride + tt) * hs + j
                        if (tt < len_self) {
                            let srow = tt < p0 ? hist[tt * dim + d + h * hs + j] : src[(tt - p0) * dim + d + h * hs + j]
                            let vrow = (tt < p0 ? hist[tt * dim + 2 * d + h * hs + j] : src[(tt - p0) * dim + 2 * d + h * hs + j]) + wn[d + h * hs + j]
                            if (uint(k_dev[pi]) != f32_to_f16(srow)) {
                                bad_k++
                            }
                            if (uint(v_dev[pi]) != f32_to_f16(vrow)) {
                                bad_v++
                            }
                            if (tt >= p0) {   // only the batch rows took the KX / VX stores
                                if (kx_dev[(h * hs + j) * tstride + tt] != srow * kscale) {
                                    bad_kx++
                                }
                                if (vx_dev[pi] != vrow) {
                                    bad_vx++
                                }
                            }
                        }
                    }
                }
            }
            let tail_k = wdec_tail_off(k_dev, heads, tstride, hs, len_self)
            t |> success(bad_k == 0, "tower_wdec_kv F16: the keys land [h][t][hs] at their positions, the history then the batch, bit for bit ({bad_k} off)")
            t |> success(bad_v == 0, "tower_wdec_kv F16: the values land with the bias row folded, bit for bit ({bad_v} off)")
            t |> success(uint(k_dev[0]) != f32_to_f16(hist[d] + 0.5), "tower_wdec_kv F16: the bar reds a poisoned element")
            t |> success(uint(v_dev[0]) != f32_to_f16(hist[2 * d] + wn[d] + 0.5), "tower_wdec_kv F16: the values' bar reds a poisoned element")
            t |> equal(tail_k, 0, "tower_wdec_kv F16: the positions past the batch keep the sentinel")
            var k_poison := k_dev
            k_poison[(tstride - 1) * hs] = uint16(0u)
            t |> success(wdec_tail_off(k_poison, heads, tstride, hs, len_self) > 0, "tower_wdec_kv F16: the tail check reds a poisoned position")
            t |> success(bad_kx == 0, "tower_wdec_kv KX: the CPU chain's transposed, hs^-0.25-scaled keys ({bad_kx} off)")
            t |> success(kx_dev[(0 * hs + 0) * tstride + p0] != src[d] * kscale + 0.5, "tower_wdec_kv KX: the bar reds a poisoned element")
            t |> success(bad_vx == 0, "tower_wdec_kv VX: the CPU chain's biased values ({bad_vx} off)")
            t |> success(vx_dev[p0 * hs] != src[2 * d] + wn[d] + 0.5, "tower_wdec_kv VX: the bar reds a poisoned element")
            // the reference reads the device planes, so the store's f16 rounding stays out of the attention compare
            let y_ref <- wdec_attn_ref(src, k_dev, v_dev, wn, rows, dim, d, heads, hs, tstride, p0 + 1, 0, true, scale)
            let bad_self = mismatch_approx(y_dev, y_ref)
            t |> success(bad_self == 0, "tower_wdec_attn part + comb, causal: the rows match the CPU form across the chunk boundary ({bad_self} of {rows * d} off)")
            t |> success(!approx(y_dev[0], y_ref[0] + 0.5), "tower_wdec_attn causal: the bar reds a poisoned element")
            let yc_ref <- wdec_attn_ref(src, k_dev, v_dev, wn, rows, dim, d, heads, hs, tstride, len_cross, 0, false, scale)
            let bad_cross = mismatch_approx(yc_dev, yc_ref)
            t |> success(bad_cross == 0, "tower_wdec_attn part + comb, cross: the rows match the CPU form over {len_cross} keys, the second chunk empty ({bad_cross} of {rows * d} off)")
            t |> success(!approx(yc_dev[0], yc_ref[0] + 0.5), "tower_wdec_attn cross: the bar reds a poisoned element")
            t |> success(mismatch_approx(y_dev, yc_ref) > rows * d / 4, "tower_wdec_attn control: the causal rows are not the cross rows ({mismatch_approx(y_dev, yc_ref)} of {rows * d} apart)")
            let ys_ref <- wdec_attn_ref(src, k_dev, v_dev, wn, rows, dim, d, heads, hs, tstride, p0 + 1, 0, true, scale * 2.0)
            t |> success(mismatch_approx(y_dev, ys_ref) > rows * d / 4, "tower_wdec_attn control: the keys scaled twice move the rows ({mismatch_approx(y_dev, ys_ref)} of {rows * d} apart)")
            for (b in fixed_array(srcd, histd, wd, kd, vd, kxd, vxd, pmsd, pyd, yd, ycd)) {
                destroy_device_buf(b)
            }
            destroy_host_buf(host)
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

let private PAT_HEADS = 16   //! the SigLIP head count: sixteen 72-wide heads through the padded 128 route

[test, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-routes")]
def test_vkt_tower_padded_attn(t0 : T?) {   // nolint:STYLE038 — the padded route end to end: the fixture, the command buffer, the oracle and its controls
    t0 |> run("the padded attention route (pad, the h128 bidirectional tile, unpad) == attention_bidir at 72-wide heads") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init()) {
                t |> skip("no Vulkan device")
                return
            }
            let khr = !cm2_fa_on()
            let fa_ok = fa_stamp_ensure(khr, int64(LN_HSP), false, false, false, false, true)
            if (!fa_ok || !ensure_tower_hpad_cls() || !ensure_tower_hpad16_cls() || !ensure_tower_hunpad_cls()) {
                t |> skip("the padded route's classes declined on this device")
                return
            }
            let B_Q = 1u
            let B_QP = 2u
            let B_K = 4u
            let B_KP = 8u
            let B_V = 16u
            let B_VP = 32u
            let B_ATTP = 64u
            let B_ATT = 128u
            let rows = TOWER_ROWS
            let rows_pad = (rows + 63) / 64 * 64   // the padded panels' 64-aligned row cap
            let dc = PAT_HEADS * LN_HS
            let dp = PAT_HEADS * LN_HSP
            let scale = 1.0 / sqrt(float(LN_HS))
            var q <- row_fill(rows * dc, 29, 11, 0.02, 0.9)
            var k <- row_fill(rows * dc, 17, 3, 0.03, 1.4)
            var v <- row_fill(rows * dc, 53, 7, 0.04, 1.7)
            for (i in range(rows * dc)) {   // the shadows round K/V through f16: the oracle sees the same values
                k[i] = f16r(k[i])
                v[i] = f16r(v[i])
            }
            let bc = int64(rows * dc) * 4l
            let bp = int64(rows_pad * dp) * 4l
            let bph = int64(rows_pad * dp) * 2l
            let qd = make_device_buf(bc)
            let kd = make_device_buf(bc)
            let vd = make_device_buf(bc)
            let qpd = make_device_buf(bp)
            let kpd = make_device_buf(bph)
            let vpd = make_device_buf(bph)
            let attpd = make_device_buf(bp)
            let attd = make_device_buf(bc)
            var host = make_host_buf(bc, true, [cached = true])
            unsafe {
                upload_region_at(qd, 0l, addr<uint8 const?>(q[0]), bc)
                upload_region_at(kd, 0l, addr<uint8 const?>(k[0]), bc)
                upload_region_at(vd, 0l, addr<uint8 const?>(v[0]), bc)
            }
            nan_sentinel_upload(qpd, bp)
            nan_sentinel_upload(attpd, bp)
            nan_sentinel_upload(attd, bc)
            fill_upload(kpd, 0l, bph, F16_SENTINEL_BITS)
            fill_upload(vpd, 0l, bph, F16_SENTINEL_BITS)
            var s_pq = set_tower_hpad_cls(fixed_array(qd, qpd), fixed_array(bc, bp), fixed_array(B_Q, B_QP))
            var s_pk = set_tower_hpad16_cls(fixed_array(kd, kpd), fixed_array(bc, bph), fixed_array(B_K, B_KP))
            var s_pv = set_tower_hpad16_cls(fixed_array(vd, vpd), fixed_array(bc, bph), fixed_array(B_V, B_VP))
            var s_fa = fa_stamp_set(khr, fixed_array(qpd, kpd, vpd, attpd), fixed_array(bp, bph, bph, bp), fixed_array(B_QP, B_KP, B_VP, B_ATTP))
            var s_un = set_tower_hunpad_cls(fixed_array(attpd, attd), fixed_array(bp, bc), fixed_array(B_ATTP, B_ATT))
            var pa_rs = TowerRestrideArgs(rows = uint(rows), rows_pad = uint(rows_pad), nheads = uint(PAT_HEADS), hs = uint(LN_HS), hsp = uint(LN_HSP), sstride = uint(dc), soff = 0u)
            var pa_fa = FaArgs(rows = uint(rows), w0 = 0u, qd = uint(dp), kvd = uint(dp), kv_mul = 1u, kvbase = 0u, scale = scale, window = 0u, sinkoff = 0u)
            let wg_rs = int64((rows_pad * dp + 255) / 256)   // the pads cover the 64-aligned cap: the K/V shadows past `rows` are zero, not the sentinel
            cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_tower_hpad_cls(raw, h, s_pq, pa_rs, wg_rs)
                enc_tower_hpad16_cls(raw, h, s_pk, pa_rs, wg_rs)
                enc_tower_hpad16_cls(raw, h, s_pv, pa_rs, wg_rs)
                fa_stamp_enc(khr, int64(LN_HSP), false, false, false, false, raw, h, s_fa, pa_fa, fa_stamp_wgs(khr, int64(PAT_HEADS), int64(rows)), true)
                enc_tower_hunpad_cls(raw, h, s_un, pa_rs, wg_rs)
                vhz_dep(raw, h, B_ATT, 0u, true)
                cmd_copy_range(raw, attd, 0l, host.buf, 0l, bc)
            }
            var att_dev : array<float>
            att_dev |> resize(rows * dc)
            unsafe {
                memcpy(addr<void?>(att_dev[0]), host.mapped, bc)
            }
            var att_ref : array<float>
            var hp : array<float>
            var attsc : array<float>
            att_ref |> resize(rows * dc)
            with_job_que() {
                setup_dasllama_jobque_()
                attention_bidir(att_ref, q, k, v, hp, attsc, int64(rows), int64(dc), int64(PAT_HEADS), scale)
            }
            // the bar: `approx`'s 2e-3 covers the tile's f16 staging of the padded Q panel and of the softmax rows (P) that the
            // f32 oracle keeps in f32, and the coopmat's reassociated sums; K and V are rounded to f16 on both sides
            let bad = mismatch_approx(att_dev, att_ref)
            t |> success(bad == 0, "the padded route matches attention_bidir over {PAT_HEADS} heads of {LN_HS} ({bad} of {rows * dc} off)")
            t |> success(!approx(att_dev[0], att_ref[0] + 0.5), "the bar reds a poisoned element")
            for (b in fixed_array(qd, kd, vd, qpd, kpd, vpd, attpd, attd)) {
                destroy_device_buf(b)
            }
            destroy_host_buf(host)
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

let private WIN_HS = 80      //! the qwen25v head width, the one the window class is stamped at
let private WIN_HEADS = 4
let private WIN_ROWS = 982   //! sixteen windows: fifteen of 64 rows and a ragged 22-row one

[test, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-routes")]
def test_vkt_tower_win_rows(t0 : T?) {   // nolint:STYLE038 — the window route end to end plus the two seams: the fixture, the command buffer, the oracles and their controls
    t0 |> run("the window-attention classes (the f32 per-window attention, the rms seam, the gated hidden) == the CPU tower helpers") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init()) {
                t |> skip("no Vulkan device")
                return
            }
            if (!ensure_tower_win_attn_cls() || !ensure_tower_post_add_rms_cls() || !ensure_tower_glu_act_cls()) {
                t |> skip("the window route's classes declined on this device")
                return
            }
            let B_Q = 1u
            let B_K = 4u
            let B_V = 16u
            let B_ATT = 128u
            let B_X = 256u
            let B_Y = 512u
            let B_XB = 1024u
            let B_G = 2048u
            let B_U = 4096u
            let B_X0 = 8192u
            let B_XB0 = 16384u
            let rows = WIN_ROWS
            let dc = WIN_HEADS * WIN_HS
            let scale = 1.0 / sqrt(float(WIN_HS))
            let nslot = (rows + 63) / 64
            t |> success(rows % 64 != 0, "the last window is ragged ({rows} rows over {nslot} windows)")
            var win_lo : array<int64>
            win_lo |> reserve(nslot + 1)
            for (s in range(nslot + 1)) {
                win_lo |> push(min(int64(s) * 64l, int64(rows)))
            }
            var wlo : array<uint>
            wlo |> resize(nslot + 1)
            for (s in range(nslot + 1)) {
                wlo[s] = uint(win_lo[s])
            }
            var q <- row_fill(rows * dc, 29, 11, 0.02, 0.9)
            var k <- row_fill(rows * dc, 17, 3, 0.03, 1.4)
            var v <- row_fill(rows * dc, 53, 7, 0.04, 1.7)
            let bc = int64(rows * dc) * 4l
            let bwl = int64(nslot + 1) * 4l
            let qd = make_device_buf(bc)
            let kd = make_device_buf(bc)
            let vd = make_device_buf(bc)
            let attd = make_device_buf(bc)
            let wld = make_device_buf(bwl)
            let d = ROWS_D
            let nrows = ROWS_N
            let nd = nrows * d
            var x <- row_fill(nd, 41, 7, 0.05, 2.1)
            var y <- row_fill(nd, 17, 3, 0.04, 1.9)
            var g <- row_fill(nd, 13, 5, 0.09, 4.0)
            var u <- row_fill(nd, 23, 9, 0.07, 3.3)
            var wn <- row_fill(4 * d, 7, 1, 0.01, -0.6)   // rows: the o bias at 0, the rms weight at d, the gate bias at 2d, the up bias at 3d
            let bx = int64(nd) * 4l
            let bw = int64(4 * d) * 4l
            let xd = make_device_buf(bx)
            let yd = make_device_buf(bx)
            let xbd = make_device_buf(bx)
            let x0d = make_device_buf(bx)    // the ln_on = 0 arm's x: the seam with no next rms
            let xb0d = make_device_buf(bx)   // ... and its rms output, which must stay untouched
            let gd = make_device_buf(bx)
            let ud = make_device_buf(bx)
            let wd = make_device_buf(bw)
            var host = make_host_buf(bc + 5l * bx, true, [cached = true])
            unsafe {
                upload_region_at(qd, 0l, addr<uint8 const?>(q[0]), bc)
                upload_region_at(kd, 0l, addr<uint8 const?>(k[0]), bc)
                upload_region_at(vd, 0l, addr<uint8 const?>(v[0]), bc)
                upload_region_at(wld, 0l, addr<uint8 const?>(wlo[0]), bwl)
                upload_region_at(xd, 0l, addr<uint8 const?>(x[0]), bx)
                upload_region_at(x0d, 0l, addr<uint8 const?>(x[0]), bx)
                upload_region_at(yd, 0l, addr<uint8 const?>(y[0]), bx)
                upload_region_at(gd, 0l, addr<uint8 const?>(g[0]), bx)
                upload_region_at(ud, 0l, addr<uint8 const?>(u[0]), bx)
                upload_region_at(wd, 0l, addr<uint8 const?>(wn[0]), bw)
            }
            nan_sentinel_upload(attd, bc)
            nan_sentinel_upload(xbd, bx)
            nan_sentinel_upload(xb0d, bx)
            var s_win = set_tower_win_attn_cls(fixed_array(qd, kd, vd, attd, wld), fixed_array(bc, bc, bc, bc, bwl), fixed_array(B_Q, B_K, B_V, B_ATT, 0u))
            var s_post = set_tower_post_add_rms_cls(fixed_array(xd, yd, xbd, wd), fixed_array(bx, bx, bx, bw), fixed_array(B_X, B_Y, B_XB, 0u))
            var s_post0 = set_tower_post_add_rms_cls(fixed_array(x0d, yd, xb0d, wd), fixed_array(bx, bx, bx, bw), fixed_array(B_X0, B_Y, B_XB0, 0u))
            var s_glu = set_tower_glu_act_cls(fixed_array(gd, ud, wd), fixed_array(bx, bx, bw), fixed_array(B_G, B_U, 0u))
            var pa_win = TowerWinArgs(nheads = uint(WIN_HEADS), scale = scale)
            var pa_post = TowerLnArgs(d = uint(d), rows = uint(nrows), boff = 0u, eps = ROWS_EPS, ln_on = 1u, lnoff = uint(d), lnboff = 0u, ascale = 1.0)
            var pa_post0 = TowerLnArgs(d = uint(d), rows = uint(nrows), boff = 0u, eps = ROWS_EPS, ln_on = 0u, lnoff = uint(d), lnboff = 0u, ascale = 1.0)
            var pa_glu = TowerGluArgs(d = uint(d), nelem = uint(nd), bgoff = uint(2 * d), buoff = uint(3 * d))
            let wg4 = int64((nd / 4 + 255) / 256)
            cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_tower_win_attn_cls(raw, h, s_win, pa_win, int64(nslot * WIN_HEADS))
                enc_tower_glu_act_cls(raw, h, s_glu, pa_glu, wg4)
                enc_tower_post_add_rms_cls(raw, h, s_post0, pa_post0, int64(nrows))
                enc_tower_post_add_rms_cls(raw, h, s_post, pa_post, int64(nrows))
                vhz_dep(raw, h, B_ATT | B_X | B_XB | B_G | B_X0 | B_XB0, 0u, true)
                cmd_copy_range(raw, attd, 0l, host.buf, 0l, bc)
                cmd_copy_range(raw, xd, 0l, host.buf, bc, bx)
                cmd_copy_range(raw, xbd, 0l, host.buf, bc + bx, bx)
                cmd_copy_range(raw, gd, 0l, host.buf, bc + 2l * bx, bx)
                cmd_copy_range(raw, x0d, 0l, host.buf, bc + 3l * bx, bx)
                cmd_copy_range(raw, xb0d, 0l, host.buf, bc + 4l * bx, bx)
            }
            var att_dev : array<float>
            var x_dev : array<float>
            var xb_dev : array<float>
            var g_dev : array<float>
            var x0_dev : array<float>
            var xb0_dev : array<uint>
            att_dev |> resize(rows * dc)
            x_dev |> resize(nd)
            xb_dev |> resize(nd)
            g_dev |> resize(nd)
            x0_dev |> resize(nd)
            xb0_dev |> resize(nd)
            unsafe {
                let hp = intptr(host.mapped)
                memcpy(addr<void?>(att_dev[0]), host.mapped, bc)
                memcpy(addr<void?>(x_dev[0]), reinterpret<void?>(hp + uint64(bc)), bx)
                memcpy(addr<void?>(xb_dev[0]), reinterpret<void?>(hp + uint64(bc + bx)), bx)
                memcpy(addr<void?>(g_dev[0]), reinterpret<void?>(hp + uint64(bc + 2l * bx)), bx)
                memcpy(addr<void?>(x0_dev[0]), reinterpret<void?>(hp + uint64(bc + 3l * bx)), bx)
                memcpy(addr<void?>(xb0_dev[0]), reinterpret<void?>(hp + uint64(bc + 4l * bx)), bx)
            }
            var att_ref : array<float>
            var hpp : array<float>
            var attsc : array<float>
            att_ref |> resize(rows * dc)
            with_job_que() {
                setup_dasllama_jobque_()
                attention_bidir_windows(att_ref, q, k, v, hpp, attsc, win_lo, int64(dc), int64(WIN_HEADS), scale)
            }
            let bad = mismatch_approx(att_dev, att_ref)
            t |> success(bad == 0, "tower_win_attn matches attention_bidir_windows over {nslot} windows, one ragged ({bad} of {rows * dc} off)")
            t |> success(!approx(att_dev[0], att_ref[0] + 0.5), "tower_win_attn: the bar reds a poisoned element")
            // the leak control: the same rows attended in full differ on the first row, so the windows' walls are the class's doing
            var full_ref : array<float>
            full_ref |> resize(rows * dc)
            with_job_que() {
                setup_dasllama_jobque_()
                attention_bidir(full_ref, q, k, v, hpp, attsc, int64(rows), int64(dc), int64(WIN_HEADS), scale)
            }
            var moved = 0.0
            for (i in range(dc)) {
                moved = max(moved, abs(full_ref[i] - att_dev[i]))
            }
            t |> success(moved > 0.01, "tower_win_attn control: full attention over the same rows lands elsewhere (maxdiff {moved})")
            var x_ref := x
            var yb := y
            add_bias_rows(yb, wn, 0l, int64(d), int64(nrows))
            add_inplace_rows(x_ref, yb, int64(nd))
            let bad_add = mismatch_approx(x_dev, x_ref)
            t |> success(bad_add == 0, "tower_post_add_rms: x += y + b ({bad_add} of {nd} off)")
            t |> success(!approx(x_dev[nd - 1], x_ref[nd - 1] + 0.5), "tower_post_add_rms: the bar reds a poisoned element of x")
            var xb_ref : array<float>
            xb_ref |> resize(nd)
            let wplane = PlaneF(p = unsafe(addr<float const?>(wn[0])), n = int64(4 * d))
            rms_rows(xb_ref, x_ref, wplane, int64(d), int64(d), int64(nrows), ROWS_EPS)
            let bad_rms = mismatch_approx(xb_dev, xb_ref)
            t |> success(bad_rms == 0, "tower_post_add_rms: the next weighted rms over the updated rows ({bad_rms} of {nd} off)")
            t |> success(!approx(xb_dev[nd - 1], xb_ref[nd - 1] + 0.5), "tower_post_add_rms: the bar reds a poisoned element")
            let bad_add0 = mismatch_approx(x0_dev, x_ref)
            t |> success(bad_add0 == 0, "tower_post_add_rms ln_on = 0: the seam alone lands the same x ({bad_add0} of {nd} off)")
            t |> equal(sentinel_words(xb0_dev), nd, "tower_post_add_rms ln_on = 0: the rms output stays untouched (every word the sentinel)")
            var g_ref := g
            var u_ref := u
            add_bias_rows(g_ref, wn, int64(2 * d), int64(d), int64(nrows))
            add_bias_rows(u_ref, wn, int64(3 * d), int64(d), int64(nrows))
            swiglu(g_ref, u_ref, int64(nd))
            let bad_glu = mismatch_approx(g_dev, g_ref)
            t |> success(bad_glu == 0, "tower_glu_act: silu(g + bg) . (u + bu) matches the CPU form ({bad_glu} of {nd} off)")
            t |> success(!approx(g_dev[0], g_ref[0] + 0.5), "tower_glu_act: the bar reds a poisoned element")
            for (b in fixed_array(qd, kd, vd, attd, wld, xd, yd, xbd, x0d, xb0d, gd, ud, wd)) {
                destroy_device_buf(b)
            }
            destroy_host_buf(host)
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! `n` floats read back from a mapped staging, `byte_off` bytes past its start
def private staged_f32(mapped : void?; byte_off : int64; n : int) : array<float> {
    var a : array<float>
    a |> resize(n)
    unsafe {
        memcpy(addr<void?>(a[0]), reinterpret<void?>(intptr(mapped) + uint64(byte_off)), int64(n) * 4l)
    }
    return <- a
}

//! `n` raw words read back from a mapped staging, `byte_off` bytes past its start - a sentinel check's bits
def private staged_u32(mapped : void?; byte_off : int64; n : int) : array<uint> {
    var a : array<uint>
    a |> resize(n)
    unsafe {
        memcpy(addr<void?>(a[0]), reinterpret<void?>(intptr(mapped) + uint64(byte_off)), int64(n) * 4l)
    }
    return <- a
}

//! `n` halves read back from a mapped staging, `byte_off` bytes past its start
def private staged_u16(mapped : void?; byte_off : int64; n : int) : array<uint16> {
    var a : array<uint16>
    a |> resize(n)
    unsafe {
        memcpy(addr<void?>(a[0]), reinterpret<void?>(intptr(mapped) + uint64(byte_off)), int64(n) * 2l)
    }
    return <- a
}

def private halves_off(got : array<uint16>; want : array<float>) : int {
    var bad = 0
    for (g, w in got, want) {
        if (uint(g) != f32_to_f16(w)) {
            bad++
        }
    }
    return bad
}

//! the first element of `want` that is not zero - the poisoned-element control's seat, off the pad columns
def private first_live(want : array<float>) : int {
    for (i in range(length(want))) {
        if (want[i] != 0.0) {
            return i
        }
    }
    return 0
}

//! the elements of `a` in [lo, hi) that are not zero - the pads' zero-tail check
def private nonzero_in(a : array<float>; lo, hi : int) : int {
    var n = 0
    for (i in range(lo, hi)) {
        if (a[i] != 0.0) {
            n++
        }
    }
    return n
}

//! the halves of `a` in [lo, hi) that are not the zero bit pattern - the f16 pads' zero-tail check
def private nonzero_halves_in(a : array<uint16>; lo, hi : int) : int {
    var n = 0
    for (i in range(lo, hi)) {
        if (uint(a[i]) != 0u) {
            n++
        }
    }
    return n
}

let private FM_IW = 7          //! the k3 s2 p1 conv over the 7 x 5 image gives the 4 x 3 output
let private FM_IH = 5
let private FM_CIN = 3
let private FM_CHUNKS = 2
let private FM_OW = (FM_IW - 1) / 2 + 1
let private FM_OH = (FM_IH - 1) / 2 + 1
let private FM_KDP = 64        //! the column row's padded width: cin x 9 = 27 live columns, the rest zero

[test]
def test_vkt_tower_front_movers(t0 : T?) {   // nolint:STYLE038 — one flat gate over the ten data movers: the fixture, one command buffer, the exact oracles and their controls
    t0 |> run("the audio fronts' data movers (im2col, im2col1d, the shuffles, the head gather, clamp + halfword, zero rows, add plane, q3a finish, ln + relu) == their forms") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tower_im2col_cls() || !ensure_tower_im2col1d_cls() || !ensure_tower_q3a_shuffle_cls() || !ensure_tower_feat_shuffle_cls()
                    || !ensure_tower_head_gather_cls() || !ensure_tower_clamp_cvt_cls() || !ensure_tower_zero_rows_cls() || !ensure_tower_bias_act_cls()
                    || !ensure_tower_q3a_finish_cls() || !ensure_tower_ln_relu_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let B_COL = 1u
            let B_COL1 = 2u
            let B_COL2 = 4u
            let B_FEAT = 8u
            let B_FS = 16u
            let B_GQ = 32u
            let B_GP = 64u
            let B_CVT = 128u
            let B_Z = 256u
            let B_ADD = 512u
            let B_FIN = 1024u
            let B_LN = 2048u
            let d = ROWS_D
            let rows = ROWS_N
            let nd = rows * d
            let hs = d / ROWS_HEADS
            let second_head_base = hs
            let chunk_in = FM_IH * FM_IW * FM_CIN
            let n_src = FM_CHUNKS * chunk_in
            let n_col = FM_CHUNKS * FM_OH * FM_OW * FM_KDP
            let nch1 = 5
            let len1 = 9
            let kdp1 = 64
            let nch2 = 6
            let kdp2 = 3 * nch2
            let rows2 = (len1 + 1) / 2
            let q3_nch = 5
            let q3_tok = int(QWEN3A_TOK_PER_CHUNK)   // the kernels carry the family's positions a chunk
            let q3_feat = 16 * q3_nch
            let q3_pos = FM_CHUNKS * q3_tok
            let fs_t = 3
            let fs_w = 4
            let fs_c = 5
            let z_from = 100
            let z_to = 1000
            let add_n = 4 * d
            var src <- row_fill(n_src, 31, 5, 0.07, 3.3)
            var mel1 <- row_fill(nch1 * len1, 23, 7, 0.09, 2.0)
            var cv2 <- row_fill(len1 * nch2, 19, 3, 0.08, 2.2)
            var y3 <- row_fill(FM_CHUNKS * 16 * q3_tok * q3_nch, 11, 2, 0.06, 2.9)
            var fs_src <- row_fill(fs_t * fs_w * fs_c, 13, 9, 0.05, 1.5)
            var x <- row_fill(nd, 41, 7, 0.05, 2.1)
            var wn <- row_fill((3 + q3_tok) * d, 7, 1, 0.01, -0.6)   // the gather's bias at 0, the add plane at d, the finish's bias at 2d and its tok position rows at 3d, the ln weight at 0
            let bsrc = int64(n_src) * 4l
            let bcol = int64(n_col) * 2l
            let bmel1 = int64(nch1 * len1) * 4l
            let bcol1 = int64(len1 * kdp1) * 2l
            let bcv2 = int64(len1 * nch2) * 4l
            let bcol2 = int64(rows2 * kdp2) * 2l
            let by3 = int64(length(y3)) * 4l
            let bfeat = int64(q3_pos * q3_feat) * 2l
            let bfs = int64(fs_t * fs_w * fs_c) * 4l
            let bx = int64(nd) * 4l
            let bxh = int64(nd) * 2l
            let bw = int64((3 + q3_tok) * d) * 4l
            let bg = int64(rows * hs) * 2l
            let srcd = make_device_buf(bsrc)
            let cold = make_device_buf(bcol)
            let mel1d = make_device_buf(bmel1)
            let col1d = make_device_buf(bcol1)
            let cv2d = make_device_buf(bcv2)
            let col2d = make_device_buf(bcol2)
            let y3d = make_device_buf(by3)
            let featd = make_device_buf(bfeat)
            let fsd = make_device_buf(bfs)
            let fsod = make_device_buf(bfs)
            let xd = make_device_buf(bx)
            let wd = make_device_buf(bw)
            let gqd = make_device_buf(bg)
            let gpd = make_device_buf(bg)
            let cvtd = make_device_buf(bxh)
            let zd = make_device_buf(bx)
            let addd = make_device_buf(bx)
            let find = make_device_buf(bx)
            let lnd = make_device_buf(bx)
            let o_col = 0l
            let o_col1 = o_col + bcol
            let o_col2 = o_col1 + bcol1
            let o_feat = o_col2 + bcol2
            let o_fs = o_feat + bfeat
            let o_gq = o_fs + bfs
            let o_gp = o_gq + bg
            let o_cvt = o_gp + bg
            let o_z = o_cvt + bxh
            let o_add = o_z + bx
            let o_fin = o_add + bx
            let o_ln = o_fin + bx
            var host = make_host_buf(o_ln + bx, true, [cached = true])
            unsafe {
                upload_region_at(srcd, 0l, addr<uint8 const?>(src[0]), bsrc)
                upload_region_at(mel1d, 0l, addr<uint8 const?>(mel1[0]), bmel1)
                upload_region_at(cv2d, 0l, addr<uint8 const?>(cv2[0]), bcv2)
                upload_region_at(y3d, 0l, addr<uint8 const?>(y3[0]), by3)
                upload_region_at(fsd, 0l, addr<uint8 const?>(fs_src[0]), bfs)
                upload_region_at(xd, 0l, addr<uint8 const?>(x[0]), bx)
                upload_region_at(wd, 0l, addr<uint8 const?>(wn[0]), bw)
                for (b in fixed_array(zd, addd, find, lnd)) {
                    upload_region_at(b, 0l, addr<uint8 const?>(x[0]), bx)
                }
            }
            for (b in fixed_array(cold, col1d, col2d, featd, gqd, gpd, cvtd)) {
                fill_upload(b, 0l, b == cold ? bcol : (b == col1d ? bcol1 : (b == col2d ? bcol2 : (b == featd ? bfeat : (b == cvtd ? bxh : bg)))), F16_SENTINEL_BITS)
            }
            nan_sentinel_upload(fsod, bfs)
            var s_im = set_tower_im2col_cls(fixed_array(srcd, cold), fixed_array(bsrc, bcol), fixed_array(0u, B_COL))
            var s_im1 = set_tower_im2col1d_cls(fixed_array(mel1d, col1d), fixed_array(bmel1, bcol1), fixed_array(0u, B_COL1))
            var s_im2 = set_tower_im2col1d_cls(fixed_array(cv2d, col2d), fixed_array(bcv2, bcol2), fixed_array(0u, B_COL2))
            var s_sh = set_tower_q3a_shuffle_cls(fixed_array(y3d, featd), fixed_array(by3, bfeat), fixed_array(0u, B_FEAT))
            var s_fs = set_tower_feat_shuffle_cls(fixed_array(fsd, fsod), fixed_array(bfs, bfs), fixed_array(0u, B_FS))
            var s_gq = set_tower_head_gather_cls(fixed_array(xd, wd, gqd), fixed_array(bx, bw, bg), fixed_array(0u, 0u, B_GQ))
            var s_gp = set_tower_head_gather_cls(fixed_array(xd, wd, gpd), fixed_array(bx, bw, bg), fixed_array(0u, 0u, B_GP))
            var s_cvt = set_tower_clamp_cvt_cls(fixed_array(xd, cvtd), fixed_array(bx, bxh), fixed_array(0u, B_CVT))
            var s_z = set_tower_zero_rows_cls(fixed_array(zd), fixed_array(bx), fixed_array(B_Z))
            var s_add = set_tower_bias_act_cls(fixed_array(addd, wd), fixed_array(bx, bw), fixed_array(B_ADD, 0u))
            var s_fin = set_tower_q3a_finish_cls(fixed_array(find, wd), fixed_array(bx, bw), fixed_array(B_FIN, 0u))
            var s_ln = set_tower_ln_relu_cls(fixed_array(lnd, wd), fixed_array(bx, bw), fixed_array(B_LN, 0u))
            var pa_im = TowerIm2colArgs(iw = uint(FM_IW), ih = uint(FM_IH), cin = uint(FM_CIN), kdp = uint(FM_KDP), rows = uint(FM_CHUNKS * FM_OH * FM_OW))
            var pa_im1 = TowerIm2col1dArgs(nch = uint(nch1), srclen = uint(len1), stride = 1u, chmajor = 1u, kdp = uint(kdp1), rows = uint(len1))
            var pa_im2 = TowerIm2col1dArgs(nch = uint(nch2), srclen = uint(len1), stride = 2u, chmajor = 0u, kdp = uint(kdp2), rows = uint(rows2))
            var pa_sh = TowerQ3aShuffleArgs(nch = uint(q3_nch), npos = uint(q3_pos))
            var pa_fs = TowerFeatShuffleArgs(w = uint(fs_w), ch = uint(fs_c), t = uint(fs_t))
            var pa_gq = TowerHeadGatherArgs(d = uint(d), hs = uint(hs), hoff = uint(second_head_base), rows = uint(rows), boff = 0u, bias_on = 1u)
            var pa_gp = TowerHeadGatherArgs(d = uint(d), hs = uint(hs), hoff = uint(second_head_base), rows = uint(rows), boff = 0u, bias_on = 0u)
            var pa_cvt = TowerClampArgs(nelem = uint(nd), lo = -1.5, hi = 1.5)
            var pa_z = TowerZeroRowsArgs(from_elem = uint(z_from), to_elem = uint(z_to))
            var pa_add = TowerBiasArgs(d = uint(add_n), nelem = uint(add_n), boff = uint(d), act = BIAS_ACT_NONE)   // the stem's position plane: one bias row the width of the rows
            var pa_fin = TowerQ3aFinishArgs(d = uint(d), npos = uint(rows), boff = uint(2 * d), poff = uint(3 * d))
            var pa_ln = TowerLnReluArgs(cout = uint(d), rows = uint(rows), woff = 0u, eps = ROWS_EPS)
            cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_tower_im2col_cls(raw, h, s_im, pa_im, int64((n_col + 255) / 256))
                enc_tower_im2col1d_cls(raw, h, s_im1, pa_im1, int64((len1 * kdp1 + 255) / 256))
                enc_tower_im2col1d_cls(raw, h, s_im2, pa_im2, int64((rows2 * kdp2 + 255) / 256))
                enc_tower_q3a_shuffle_cls(raw, h, s_sh, pa_sh, int64((q3_pos * q3_feat + 255) / 256))
                enc_tower_feat_shuffle_cls(raw, h, s_fs, pa_fs, int64((fs_t * fs_w * fs_c + 255) / 256))
                enc_tower_head_gather_cls(raw, h, s_gq, pa_gq, int64((rows * hs + 255) / 256))
                enc_tower_head_gather_cls(raw, h, s_gp, pa_gp, int64((rows * hs + 255) / 256))
                enc_tower_clamp_cvt_cls(raw, h, s_cvt, pa_cvt, int64((nd / 4 + 255) / 256))
                enc_tower_zero_rows_cls(raw, h, s_z, pa_z, int64((z_to - z_from + 255) / 256))
                enc_tower_bias_act_cls(raw, h, s_add, pa_add, int64((add_n / 4 + 255) / 256))
                enc_tower_q3a_finish_cls(raw, h, s_fin, pa_fin, int64((nd + 255) / 256))
                enc_tower_ln_relu_cls(raw, h, s_ln, pa_ln, int64((rows + 255) / 256))
                vhz_dep(raw, h, B_COL | B_COL1 | B_COL2 | B_FEAT | B_FS | B_GQ | B_GP | B_CVT | B_Z | B_ADD | B_FIN | B_LN, 0u, true)
                cmd_copy_range(raw, cold, 0l, host.buf, o_col, bcol)
                cmd_copy_range(raw, col1d, 0l, host.buf, o_col1, bcol1)
                cmd_copy_range(raw, col2d, 0l, host.buf, o_col2, bcol2)
                cmd_copy_range(raw, featd, 0l, host.buf, o_feat, bfeat)
                cmd_copy_range(raw, fsod, 0l, host.buf, o_fs, bfs)
                cmd_copy_range(raw, gqd, 0l, host.buf, o_gq, bg)
                cmd_copy_range(raw, gpd, 0l, host.buf, o_gp, bg)
                cmd_copy_range(raw, cvtd, 0l, host.buf, o_cvt, bxh)
                cmd_copy_range(raw, zd, 0l, host.buf, o_z, bx)
                cmd_copy_range(raw, addd, 0l, host.buf, o_add, bx)
                cmd_copy_range(raw, find, 0l, host.buf, o_fin, bx)
                cmd_copy_range(raw, lnd, 0l, host.buf, o_ln, bx)
            }
            let col_dev <- staged_u16(host.mapped, o_col, n_col)
            let col1_dev <- staged_u16(host.mapped, o_col1, len1 * kdp1)
            let col2_dev <- staged_u16(host.mapped, o_col2, rows2 * kdp2)
            let feat_dev <- staged_u16(host.mapped, o_feat, q3_pos * q3_feat)
            let fs_dev <- staged_f32(host.mapped, o_fs, fs_t * fs_w * fs_c)
            let gq_dev <- staged_u16(host.mapped, o_gq, rows * hs)
            let gp_dev <- staged_u16(host.mapped, o_gp, rows * hs)
            let cvt_dev <- staged_u16(host.mapped, o_cvt, nd)
            let z_dev <- staged_f32(host.mapped, o_z, nd)
            let add_dev <- staged_f32(host.mapped, o_add, nd)
            let fin_dev <- staged_f32(host.mapped, o_fin, nd)
            let ln_dev <- staged_f32(host.mapped, o_ln, nd)
            var col_want : array<float>
            col_want |> resize(n_col)
            for (gid in range(n_col)) {
                let r = gid / FM_KDP
                let k = gid % FM_KDP
                var v = 0.0
                if (k < FM_CIN * 9) {
                    let ci = r / (FM_OH * FM_OW)
                    let rr = r % (FM_OH * FM_OW)
                    let iy = (rr / FM_OW) * 2 + (k % 9) / 3 - 1
                    let ix = (rr % FM_OW) * 2 + k % 3 - 1
                    if (iy >= 0 && iy < FM_IH && ix >= 0 && ix < FM_IW) {
                        v = src[ci * chunk_in + (iy * FM_IW + ix) * FM_CIN + k / 9]
                    }
                }
                col_want[gid] = v
            }
            let bad_im = halves_off(col_dev, col_want)
            t |> success(bad_im == 0, "tower_im2col: the k3 s2 p1 columns over two chunks, the pad and the zero tail, bit for bit ({bad_im} of {n_col} off)")
            let live_im = first_live(col_want)
            t |> success(uint(col_dev[live_im]) != f32_to_f16(col_want[live_im] + 0.5), "tower_im2col: the bar reds a poisoned element")
            var col1_want : array<float>
            col1_want |> resize(len1 * kdp1)
            for (gid in range(len1 * kdp1)) {
                let i = gid / kdp1
                let k = gid % kdp1
                var v = 0.0
                if (k < 3 * nch1) {
                    let sp = i + k / nch1 - 1
                    if (sp >= 0 && sp < len1) {
                        v = mel1[(k % nch1) * len1 + sp]
                    }
                }
                col1_want[gid] = v
            }
            var col2_want : array<float>
            col2_want |> resize(rows2 * kdp2)
            for (gid in range(rows2 * kdp2)) {
                let i = gid / kdp2
                let k = gid % kdp2
                let sp = i * 2 + k / nch2 - 1
                col2_want[gid] = sp >= 0 && sp < len1 ? cv2[sp * nch2 + k % nch2] : 0.0
            }
            let bad_im1 = halves_off(col1_dev, col1_want)
            let bad_im2 = halves_off(col2_dev, col2_want)
            t |> success(bad_im1 == 0, "tower_im2col1d (channel-major, stride 1): the mel's k3 columns with the zero pad and tail, bit for bit ({bad_im1} of {len1 * kdp1} off)")
            t |> success(bad_im2 == 0, "tower_im2col1d (row-major, stride 2): the conv output's k3 columns at every second position, bit for bit ({bad_im2} of {rows2 * kdp2} off)")
            let live_im1 = first_live(col1_want)
            t |> success(uint(col1_dev[live_im1]) != f32_to_f16(col1_want[live_im1] + 0.5), "tower_im2col1d: the bar reds a poisoned element")
            let live_im2 = first_live(col2_want)
            t |> success(uint(col2_dev[live_im2]) != f32_to_f16(col2_want[live_im2] + 0.5), "tower_im2col1d (row-major, stride 2): the bar reds a poisoned element")
            var feat_want : array<float>
            feat_want |> resize(q3_pos * q3_feat)
            for (gid in range(q3_pos * q3_feat)) {
                let row = gid / q3_feat
                let j = gid % q3_feat
                feat_want[gid] = y3[(row / q3_tok) * (16 * q3_tok * q3_nch) + ((j % 16) * q3_tok + row % q3_tok) * q3_nch + j / 16]
            }
            let bad_sh = halves_off(feat_dev, feat_want)
            t |> success(bad_sh == 0, "tower_q3a_shuffle: the feature rows off the third conv stage, bit for bit ({bad_sh} of {q3_pos * q3_feat} off)")
            t |> success(uint(feat_dev[first_live(feat_want)]) != f32_to_f16(feat_want[first_live(feat_want)] + 0.5), "tower_q3a_shuffle: the bar reds a poisoned element")
            var fs_want : array<float>
            fs_want |> resize(fs_t * fs_w * fs_c)
            for (gid in range(fs_t * fs_w * fs_c)) {
                let tt = gid / (fs_w * fs_c)
                let rem = gid % (fs_w * fs_c)
                fs_want[gid] = fs_src[(tt * fs_w + rem % fs_w) * fs_c + rem / fs_w]
            }
            let bad_fs = mismatch_exact(fs_dev, fs_want)
            t |> success(bad_fs == 0, "tower_feat_shuffle: the [time][ch][freq] rows off the [time][freq][ch] image, bit for bit ({bad_fs} of {fs_t * fs_w * fs_c} off)")
            t |> success(fs_dev[0] != fs_want[0] + 0.5, "tower_feat_shuffle: the bar reds a poisoned element")
            var gq_want : array<float>
            var gp_want : array<float>
            gq_want |> resize(rows * hs)
            gp_want |> resize(rows * hs)
            for (r in range(rows)) {
                for (n in range(hs)) {
                    gp_want[r * hs + n] = x[r * d + second_head_base + n]
                    gq_want[r * hs + n] = x[r * d + second_head_base + n] + wn[second_head_base + n]
                }
            }
            let bad_gq = halves_off(gq_dev, gq_want)
            let bad_gp = halves_off(gp_dev, gp_want)
            t |> success(bad_gq == 0, "tower_head_gather (biased): the head's rows plus the bias row as halves, bit for bit ({bad_gq} of {rows * hs} off)")
            t |> success(bad_gp == 0, "tower_head_gather (plain): the head's rows as halves, bit for bit ({bad_gp} of {rows * hs} off)")
            t |> success(uint(gq_dev[0]) != f32_to_f16(gq_want[0] + 0.5) && halves_off(gp_dev, gq_want) > rows * hs / 2, "tower_head_gather: the bar reds a poisoned element, and the plain arm misses the biased rows")
            var cvt_want : array<float>
            cvt_want |> resize(nd)
            var clamped = 0
            for (i in range(nd)) {
                cvt_want[i] = clamp(x[i], -1.5, 1.5)
                if (cvt_want[i] != x[i]) {
                    clamped++
                }
            }
            let bad_cvt = halves_off(cvt_dev, cvt_want)
            t |> success(bad_cvt == 0 && clamped > nd / 8, "tower_clamp_cvt: the clamped rows as halves, bit for bit ({bad_cvt} of {nd} off, {clamped} elements clamped)")
            t |> success(uint(cvt_dev[0]) != f32_to_f16(cvt_want[0] + 0.5), "tower_clamp_cvt: the bar reds a poisoned element")
            var z_want := x
            for (i in range(z_from, z_to)) {
                z_want[i] = 0.0
            }
            let bad_z = mismatch_exact(z_dev, z_want)
            t |> success(bad_z == 0 && z_dev[z_from] == 0.0 && x[z_from] != 0.0, "tower_zero_rows: the span zeroed and nothing beside it touched ({bad_z} of {nd} off; element {z_from} moved off {x[z_from]})")
            var z_poison := z_want
            z_poison[z_from] += 0.5
            t |> success(mismatch_exact(z_dev, z_poison) > 0, "tower_zero_rows: the bar reds a poisoned element")
            var add_want := x
            for (i in range(add_n)) {
                add_want[i] = x[i] + wn[d + i]
            }
            let bad_add = mismatch_exact(add_dev, add_want)
            t |> success(bad_add == 0 && add_dev[0] != x[0], "tower_bias_act as the plane add (d = nelem): the plane added element for element and nothing past it touched ({bad_add} of {nd} off)")
            var add_poison := add_want
            add_poison[add_n - 1] += 0.5
            t |> success(mismatch_exact(add_dev, add_poison) > 0, "tower_bias_act as the plane add: the bar reds a poisoned element")
            var fin_want : array<float>
            fin_want |> resize(nd)
            for (gid in range(nd)) {
                let e = gid % d
                fin_want[gid] = x[gid] + wn[2 * d + e] + wn[3 * d + ((gid / d) % q3_tok) * d + e]
            }
            let bad_fin = mismatch_approx(fin_dev, fin_want)
            t |> success(bad_fin == 0 && !approx(fin_dev[d], x[d]), "tower_q3a_finish: the conv_out bias and the per-chunk position rows added ({bad_fin} of {nd} off)")
            t |> success(!approx(fin_dev[0], fin_want[0] + 0.5), "tower_q3a_finish: the bar reds a poisoned element")
            var ln_want : array<float>
            ln_want |> resize(nd)
            for (r in range(rows)) {
                var mean = 0.0lf
                for (c in range(d)) {
                    mean += double(x[r * d + c])
                }
                mean /= double(d)
                var vv = 0.0lf
                for (c in range(d)) {
                    let dv = double(x[r * d + c]) - mean
                    vv += dv * dv
                }
                let inv = 1.0lf / sqrt(vv / double(d) + double(ROWS_EPS))
                for (c in range(d)) {
                    ln_want[r * d + c] = max(float((double(x[r * d + c]) - mean) * inv) * wn[c], 0.0)
                }
            }
            let bad_ln = mismatch_approx(ln_dev, ln_want)
            let live_ln = first_live(ln_want)
            t |> success(bad_ln == 0 && ln_dev[live_ln] != x[live_ln], "tower_ln_relu: the channel LayerNorm then ReLU match the CPU loop ({bad_ln} of {nd} off)")
            t |> success(!approx(ln_dev[live_ln], ln_want[live_ln] + 0.5), "tower_ln_relu: the bar reds a poisoned element")
            for (b in fixed_array(srcd, cold, mel1d, col1d, cv2d, col2d, y3d, featd, fsd, fsod, xd, wd, gqd, gpd, cvtd, zd, addd, find, lnd)) {
                destroy_device_buf(b)
            }
            destroy_host_buf(host)
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

let private MEL_NFFT = 64
let private MEL_NBINS = 33
let private MEL_NLEN = 5
let private MEL_NMEL = 6
let private MEL_MAX_FLOOR_G4A = 0.001                    //! gemma4a's floor under the max
let private MEL_FLOOR_24 = 5.960464477539063e-08     //! the 2^-24 floor canary adds and the whisper preprocessors max against
let private MEL_LOG10_E = 0.43429448190325176
let private NORM_NLEN = 9
let private NORM_VALID = 6
let private DW_CH = 6
let private REL_D = 64

[test]
def test_vkt_tower_front_mel(t0 : T?) {   // nolint:STYLE038 — one flat gate over the spectral classes: the fixture, one command buffer, the double-precision oracles and their controls
    t0 |> run("the audio fronts' spectral classes (spec_dft, mel_log, mel_norm, dw_conv, rel_sincos) == their CPU forms") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tower_spec_dft_cls() || !ensure_tower_mel_log_cls() || !ensure_tower_mel_norm_cls() || !ensure_tower_dw_conv_cls()
                    || !ensure_tower_rel_sincos_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let B_MAG = 1u
            let B_POW = 2u
            let B_G4A = 4u
            let B_CN = 8u
            let B_WH = 16u
            let B_NORM = 32u
            let B_DW = 64u
            let B_REL = 128u
            let n_pw = MEL_NLEN * MEL_NBINS
            let n_mel = MEL_NLEN * MEL_NMEL
            let n_norm = NORM_NLEN * MEL_NMEL
            let dw_in = FM_IH * FM_IW * DW_CH
            let dw_out = FM_OH * FM_OW * DW_CH
            let tt = ROWS_N
            let ww = 2 * tt - 1
            let n_rel = ww * REL_D
            var xw <- row_fill(MEL_NLEN * MEL_NFFT, 37, 5, 0.03, 1.4)
            let tw <- build_dft_twiddles(int64(MEL_NFFT))
            var pw <- row_fill(n_pw, 17, 3, 0.02, -0.9)
            for (v in pw) {
                v = abs(v) + 0.01
            }
            for (b in range(MEL_NBINS)) {   // frame 0 sits under gemma4a's floor: the max arm floors it, the add arm reads under it
                pw[b] = 1.0e-6
            }
            var fil <- row_fill(MEL_NMEL * MEL_NBINS, 13, 7, 0.01, -0.5)
            for (v in fil) {
                v = abs(v) * 0.1
            }
            var mel_in <- row_fill(n_norm, 29, 11, 0.07, 2.4)
            var dw_x <- row_fill(dw_in, 23, 9, 0.06, 2.7)
            var dw_w <- row_fill(DW_CH * 9 + DW_CH, 7, 2, 0.05, 0.9)   // the taps at 0, the bias row at ch x 9
            let bxw = int64(MEL_NLEN * MEL_NFFT) * 4l
            let btw = int64(length(tw)) * 4l
            let bpw = int64(n_pw) * 4l
            let bfil = int64(MEL_NMEL * MEL_NBINS) * 4l
            let bmel = int64(n_mel) * 4l
            let bnorm = int64(n_norm) * 4l
            let dw_lanes = (dw_out + 255) / 256 * 256   // the dispatch's lanes: the output past dw_out keeps the sentinel
            let bdwx = int64(dw_in) * 4l
            let bdwx_slack = int64(dw_in + FM_IW * DW_CH) * 4l   // an image row of garbage past the source: a tap below the image the row guard lets through reads it
            let bdwy = int64(dw_out) * 4l
            let bdwy_lanes = int64(dw_lanes) * 4l
            let bdww = int64(length(dw_w)) * 4l
            let brel = int64(n_rel) * 4l
            let xwd = make_device_buf(bxw)
            let twd = make_device_buf(btw)
            let magd = make_device_buf(bpw)
            let powd = make_device_buf(bpw)
            let pwd = make_device_buf(bpw)
            let fild = make_device_buf(bfil)
            let g4ad = make_device_buf(bmel)
            let cnd = make_device_buf(bmel)
            let whd = make_device_buf(bmel)
            let normd = make_device_buf(bnorm)
            let dwxd = make_device_buf(bdwx_slack)
            let dwyd = make_device_buf(bdwy_lanes)
            let dwwd = make_device_buf(bdww)
            let reld = make_device_buf(brel)
            let o_mag = 0l
            let o_pow = o_mag + bpw
            let o_g4a = o_pow + bpw
            let o_cn = o_g4a + bmel
            let o_wh = o_cn + bmel
            let o_norm = o_wh + bmel
            let o_dw = o_norm + bnorm
            let o_rel = o_dw + bdwy_lanes
            var host = make_host_buf(o_rel + brel, true, [cached = true])
            unsafe {
                upload_region_at(xwd, 0l, addr<uint8 const?>(xw[0]), bxw)
                upload_region_at(twd, 0l, addr<uint8 const?>(tw[0]), btw)
                upload_region_at(pwd, 0l, addr<uint8 const?>(pw[0]), bpw)
                upload_region_at(fild, 0l, addr<uint8 const?>(fil[0]), bfil)
                upload_region_at(normd, 0l, addr<uint8 const?>(mel_in[0]), bnorm)
                upload_region_at(dwxd, 0l, addr<uint8 const?>(dw_x[0]), bdwx)
                upload_region_at(dwwd, 0l, addr<uint8 const?>(dw_w[0]), bdww)
            }
            nan_sentinel_upload(magd, bpw)
            nan_sentinel_upload(powd, bpw)
            nan_sentinel_upload(g4ad, bmel)
            nan_sentinel_upload(cnd, bmel)
            nan_sentinel_upload(whd, bmel)
            fill_upload(dwxd, bdwx, bdwx_slack - bdwx, GUARD_GARBAGE_BITS)
            nan_sentinel_upload(dwyd, bdwy_lanes)
            nan_sentinel_upload(reld, brel)
            var s_mag = set_tower_spec_dft_cls(fixed_array(xwd, twd, magd), fixed_array(bxw, btw, bpw), fixed_array(0u, 0u, B_MAG))
            var s_pow = set_tower_spec_dft_cls(fixed_array(xwd, twd, powd), fixed_array(bxw, btw, bpw), fixed_array(0u, 0u, B_POW))
            var s_g4a = set_tower_mel_log_cls(fixed_array(pwd, fild, g4ad), fixed_array(bpw, bfil, bmel), fixed_array(0u, 0u, B_G4A))
            var s_cn = set_tower_mel_log_cls(fixed_array(pwd, fild, cnd), fixed_array(bpw, bfil, bmel), fixed_array(0u, 0u, B_CN))
            var s_wh = set_tower_mel_log_cls(fixed_array(pwd, fild, whd), fixed_array(bpw, bfil, bmel), fixed_array(0u, 0u, B_WH))
            var s_norm = set_tower_mel_norm_cls(fixed_array(normd), fixed_array(bnorm), fixed_array(B_NORM))
            var s_dw = set_tower_dw_conv_cls(fixed_array(dwxd, dwyd, dwwd), fixed_array(bdwx_slack, bdwy_lanes, bdww), fixed_array(0u, B_DW, 0u))
            var s_rel = set_tower_rel_sincos_cls(fixed_array(reld), fixed_array(brel), fixed_array(B_REL))
            var pa_mag = TowerSpecDftArgs(n_fft = uint(MEL_NFFT), n_frames = uint(MEL_NLEN), mag = 1u)
            var pa_pow = TowerSpecDftArgs(n_fft = uint(MEL_NFFT), n_frames = uint(MEL_NLEN), mag = 0u)
            var pa_g4a = TowerMelLogArgs(n_bins = uint(MEL_NBINS), n_mel = uint(MEL_NMEL), n_frames = uint(MEL_NLEN),
                floor = MEL_MAX_FLOOR_G4A, scale = 1.0, frame_major = 1u, add_floor = 0u)
            var pa_cn = TowerMelLogArgs(n_bins = uint(MEL_NBINS), n_mel = uint(MEL_NMEL), n_frames = uint(MEL_NLEN),
                floor = MEL_FLOOR_24, scale = 1.0, frame_major = 1u, add_floor = 1u)
            var pa_wh = TowerMelLogArgs(n_bins = uint(MEL_NBINS), n_mel = uint(MEL_NMEL), n_frames = uint(MEL_NLEN),
                floor = MEL_FLOOR_24, scale = MEL_LOG10_E, frame_major = 0u, add_floor = 0u)
            var pa_norm = TowerMelNormArgs(n_mel = uint(MEL_NMEL), n_frames = uint(NORM_NLEN), valid = uint(NORM_VALID))
            var pa_dw = TowerDwConvArgs(iw = uint(FM_IW), ih = uint(FM_IH), ch = uint(DW_CH), woff = 0u, boff = uint(DW_CH * 9))
            var pa_rel = TowerRelSinCosArgs(d = uint(REL_D), tt = uint(tt))
            cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_tower_spec_dft_cls(raw, h, s_mag, pa_mag, int64((n_pw + 255) / 256))
                enc_tower_spec_dft_cls(raw, h, s_pow, pa_pow, int64((n_pw + 255) / 256))
                enc_tower_mel_log_cls(raw, h, s_g4a, pa_g4a, int64((n_mel + 255) / 256))
                enc_tower_mel_log_cls(raw, h, s_cn, pa_cn, int64((n_mel + 255) / 256))
                enc_tower_mel_log_cls(raw, h, s_wh, pa_wh, int64((n_mel + 255) / 256))
                enc_tower_mel_norm_cls(raw, h, s_norm, pa_norm, int64((MEL_NMEL + 63) / 64))
                enc_tower_dw_conv_cls(raw, h, s_dw, pa_dw, int64((dw_out + 255) / 256))
                enc_tower_rel_sincos_cls(raw, h, s_rel, pa_rel, int64((n_rel + 255) / 256))
                vhz_dep(raw, h, B_MAG | B_POW | B_G4A | B_CN | B_WH | B_NORM | B_DW | B_REL, 0u, true)
                cmd_copy_range(raw, magd, 0l, host.buf, o_mag, bpw)
                cmd_copy_range(raw, powd, 0l, host.buf, o_pow, bpw)
                cmd_copy_range(raw, g4ad, 0l, host.buf, o_g4a, bmel)
                cmd_copy_range(raw, cnd, 0l, host.buf, o_cn, bmel)
                cmd_copy_range(raw, whd, 0l, host.buf, o_wh, bmel)
                cmd_copy_range(raw, normd, 0l, host.buf, o_norm, bnorm)
                cmd_copy_range(raw, dwyd, 0l, host.buf, o_dw, bdwy_lanes)
                cmd_copy_range(raw, reld, 0l, host.buf, o_rel, brel)
            }
            let mag_dev <- staged_f32(host.mapped, o_mag, n_pw)
            let pow_dev <- staged_f32(host.mapped, o_pow, n_pw)
            let g4a_dev <- staged_f32(host.mapped, o_g4a, n_mel)
            let cn_dev <- staged_f32(host.mapped, o_cn, n_mel)
            let wh_dev <- staged_f32(host.mapped, o_wh, n_mel)
            let norm_dev <- staged_f32(host.mapped, o_norm, n_norm)
            let dw_dev <- staged_f32(host.mapped, o_dw, dw_out)
            let dw_idle <- staged_u32(host.mapped, o_dw + bdwy, dw_lanes - dw_out)
            let rel_dev <- staged_f32(host.mapped, o_rel, n_rel)
            var mag_want : array<float>
            var pow_want : array<float>
            mag_want |> resize(n_pw)
            pow_want |> resize(n_pw)
            for (gid in range(n_pw)) {
                let frame = gid / MEL_NBINS
                let b = gid % MEL_NBINS
                var re = 0.0lf
                var im = 0.0lf
                for (k in range(MEL_NFFT)) {
                    re += double(xw[frame * MEL_NFFT + k]) * double(tw[k * 2 * MEL_NBINS + 2 * b])
                    im += double(xw[frame * MEL_NFFT + k]) * double(tw[k * 2 * MEL_NBINS + 2 * b + 1])
                }
                pow_want[gid] = float(re * re + im * im)
                mag_want[gid] = float(sqrt(re * re + im * im))
            }
            let bad_mag = mismatch_approx(mag_dev, mag_want)
            let bad_pow = mismatch_approx(pow_dev, pow_want)
            t |> success(bad_mag == 0, "tower_spec_dft (magnitude): the frames' spectra match the double DFT ({bad_mag} of {n_pw} off)")
            t |> success(bad_pow == 0, "tower_spec_dft (power): the same as re^2 + im^2 ({bad_pow} of {n_pw} off)")
            t |> success(!approx(mag_dev[0], mag_want[0] + 0.5) && !approx(pow_dev[1], mag_want[1]), "tower_spec_dft: the bar reds a poisoned element, and the power arm is not the magnitude")
            var g4a_want : array<float>
            var cn_want : array<float>
            var wh_want : array<float>
            g4a_want |> resize(n_mel)
            cn_want |> resize(n_mel)
            wh_want |> resize(n_mel)
            for (frame in range(MEL_NLEN)) {
                for (j in range(MEL_NMEL)) {
                    var sum = 0.0lf
                    for (k in range(MEL_NBINS)) {
                        sum += double(pw[frame * MEL_NBINS + k]) * double(fil[j * MEL_NBINS + k])
                    }
                    g4a_want[frame * MEL_NMEL + j] = float(log(max(sum, double(MEL_MAX_FLOOR_G4A))))
                    cn_want[frame * MEL_NMEL + j] = float(log(sum + double(MEL_FLOOR_24)))
                    wh_want[j * MEL_NLEN + frame] = float(log(max(sum, double(MEL_FLOOR_24))) * double(MEL_LOG10_E))
                }
            }
            let bad_g4a = mismatch_approx(g4a_dev, g4a_want)
            let bad_cn = mismatch_approx(cn_dev, cn_want)
            let bad_wh = mismatch_approx(wh_dev, wh_want)
            t |> success(bad_g4a == 0, "tower_mel_log (frame-major, max floor): the gemma4a form ({bad_g4a} of {n_mel} off)")
            t |> success(bad_cn == 0, "tower_mel_log (frame-major, added floor): the canary form ({bad_cn} of {n_mel} off)")
            t |> success(bad_wh == 0, "tower_mel_log (mel-major, log10): the whisper preprocessors' form ({bad_wh} of {n_mel} off)")
            t |> success(approx(g4a_dev[0], log(MEL_MAX_FLOOR_G4A)) && cn_dev[0] < g4a_dev[0] - 1.0, "tower_mel_log: the quiet frame floors on the max arm and reads under the floor on the add arm")
            t |> success(!approx(g4a_dev[n_mel - 1], g4a_want[n_mel - 1] + 0.5), "tower_mel_log: the bar reds a poisoned element")
            t |> success(!approx(cn_dev[n_mel - 1], cn_want[n_mel - 1] + 0.5), "tower_mel_log (added floor): the bar reds a poisoned element")
            t |> success(!approx(wh_dev[n_mel - 1], wh_want[n_mel - 1] + 0.5), "tower_mel_log (log10): the bar reds a poisoned element")
            var norm_want : array<float>
            norm_want |> resize(n_norm)
            for (j in range(MEL_NMEL)) {
                var sum = 0.0lf
                for (i in range(NORM_VALID)) {
                    sum += double(mel_in[i * MEL_NMEL + j])
                }
                let mean = sum / double(NORM_VALID)
                var sq = 0.0lf
                for (i in range(NORM_VALID)) {
                    let dv = double(mel_in[i * MEL_NMEL + j]) - mean
                    sq += dv * dv
                }
                let denom = sqrt(sq / double(NORM_VALID - 1)) + 1e-5lf
                for (i in range(NORM_NLEN)) {
                    norm_want[i * MEL_NMEL + j] = i < NORM_VALID ? float((double(mel_in[i * MEL_NMEL + j]) - mean) / denom) : 0.0
                }
            }
            let bad_norm = mismatch_approx(norm_dev, norm_want)
            var tail_off = 0
            for (i in range(NORM_VALID * MEL_NMEL, n_norm)) {
                if (norm_dev[i] != 0.0) {
                    tail_off++
                }
            }
            t |> success(bad_norm == 0 && tail_off == 0 && norm_dev[0] != mel_in[0], "tower_mel_norm: the column statistics over the valid frames and the zero tail match the CPU loop ({bad_norm} of {n_norm} off, {tail_off} tail words live)")
            t |> success(!approx(norm_dev[0], norm_want[0] + 0.5), "tower_mel_norm: the bar reds a poisoned element")
            var dw_want : array<float>
            dw_want |> resize(dw_out)
            for (gid in range(dw_out)) {
                let c = gid % DW_CH
                let o = gid / DW_CH
                var acc = double(dw_w[DW_CH * 9 + c])
                for (kh in range(3)) {
                    for (kw in range(3)) {
                        let iy = (o / FM_OW) * 2 + kh - 1
                        let ix = (o % FM_OW) * 2 + kw - 1
                        if (iy >= 0 && iy < FM_IH && ix >= 0 && ix < FM_IW) {
                            acc += double(dw_w[c * 9 + kh * 3 + kw]) * double(dw_x[(iy * FM_IW + ix) * DW_CH + c])
                        }
                    }
                }
                dw_want[gid] = float(acc)
            }
            let bad_dw = mismatch_approx(dw_dev, dw_want)
            t |> success(bad_dw == 0, "tower_dw_conv: the k3 s2 p1 depthwise taps with the bias match the CPU form ({bad_dw} of {dw_out} off)")
            t |> success(!approx(dw_dev[0], dw_want[0] + 0.5), "tower_dw_conv: the bar reds a poisoned element")
            t |> equal(sentinel_words(dw_idle), dw_lanes - dw_out, "tower_dw_conv: the idle lanes past the output write nothing (every word past it the sentinel)")
            var rel_want : array<float>
            rel_want |> resize(n_rel)
            for (w in range(ww)) {
                let rel = double(tt - 1 - w)
                for (k in range(REL_D / 2)) {
                    let freq = exp(-(double(k * 2) * log(10000.0lf) / double(REL_D)))
                    let theta = float(freq * rel)
                    rel_want[w * REL_D + 2 * k] = sin(theta)
                    rel_want[w * REL_D + 2 * k + 1] = cos(theta)
                }
            }
            let bad_rel = mismatch_approx(rel_dev, rel_want)
            var zero_row_off = 0
            for (k in range(REL_D / 2)) {
                if (rel_dev[(tt - 1) * REL_D + 2 * k] != 0.0 || rel_dev[(tt - 1) * REL_D + 2 * k + 1] != 1.0) {
                    zero_row_off++
                }
            }
            t |> success(bad_rel == 0 && zero_row_off == 0, "tower_rel_sincos: the table matches the CPU loop, the rel 0 row exactly (0, 1) ({bad_rel} of {n_rel} off, {zero_row_off} of the zero row off)")
            t |> success(!approx(rel_dev[0], rel_want[0] + 0.5), "tower_rel_sincos: the bar reds a poisoned element")
            for (b in fixed_array(xwd, twd, magd, powd, pwd, fild, g4ad, cnd, whd, normd, dwxd, dwyd, dwwd, reld)) {
                destroy_device_buf(b)
            }
            destroy_host_buf(host)
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! one head's rel plane as the f16 GEMM lands it: R[i][w] = (q_i + bias_v) . P[w] over the head's slice
def private cn_rel_plane(q, posl, wn : array<float>; rows, ww, d, hs, hoff, bvoff : int) : array<float> {
    var r : array<float>
    r |> resize(rows * ww)
    for (i in range(rows)) {
        for (w in range(ww)) {
            var acc = 0.0
            for (n in range(hs)) {
                acc += (q[i * d + hoff + n] + wn[bvoff + hoff + n]) * posl[w * d + hoff + n]
            }
            r[i * ww + w] = acc
        }
    }
    return <- r
}

[test]
def test_vkt_tower_cn_attn_rel_plane(t0 : T?) {   // nolint:STYLE038 — one flat gate over the two stamps: the fixture, one command buffer, the oracle and its controls
    t0 |> run("the rel-plane FastConformer attention (both head widths) over CPU-built planes == the CPU loop") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tower_cn_attn_r64_cls() || !ensure_tower_cn_attn_r_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let B_AO = 1u
            let B_AO2 = 2u
            let d = ROWS_D
            let rows = ROWS_N
            let hs = d / ROWS_HEADS
            let heads2 = ROWS_HEADS / 2
            let hs2 = d / heads2
            let nd = rows * d
            let ww = 2 * rows - 1
            let nr = rows * ww
            var q <- row_fill(nd, 41, 3, 0.03, 1.1)
            var k <- row_fill(nd, 17, 11, 0.04, 1.3)
            var v <- row_fill(nd, 29, 13, 0.05, 1.7)
            var posl <- row_fill(ww * d, 13, 2, 0.02, 0.9)
            var wn <- row_fill(2 * d, 7, 1, 0.01, 0.3)   // bias_u at 0, bias_v at d
            let wg = int64((rows + 63) / 64)
            let ao_rows = int(wg) * 64   // a row a lane: the idle lanes' rows past `rows` keep the sentinel
            let bx = int64(nd) * 4l
            let bao = int64(ao_rows * d) * 4l
            let bidle = bao - bx
            let br = int64(nr) * 4l
            let bw = int64(2 * d) * 4l
            let qd = make_device_buf(bx)
            let kd = make_device_buf(bx)
            let vd = make_device_buf(bx)
            let aod = make_device_buf(bao)
            let ao2d = make_device_buf(bao)
            let wd = make_device_buf(bw)
            var head_rel_planes <- [for (_i in range(ROWS_HEADS + heads2)); make_device_buf(br)]   // a plane a head: the 64-wide heads first, then the two 128-wide ones
            var host = make_host_buf(2l * bao, true, [cached = true])
            unsafe {
                upload_region_at(qd, 0l, addr<uint8 const?>(q[0]), bx)
                upload_region_at(kd, 0l, addr<uint8 const?>(k[0]), bx)
                upload_region_at(vd, 0l, addr<uint8 const?>(v[0]), bx)
                upload_region_at(wd, 0l, addr<uint8 const?>(wn[0]), bw)
                for (hh in range(ROWS_HEADS)) {
                    let plane <- cn_rel_plane(q, posl, wn, rows, ww, d, hs, hh * hs, d)
                    upload_region_at(head_rel_planes[hh], 0l, addr<uint8 const?>(plane[0]), br)
                }
                for (hh in range(heads2)) {
                    let plane <- cn_rel_plane(q, posl, wn, rows, ww, d, hs2, hh * hs2, d)
                    upload_region_at(head_rel_planes[ROWS_HEADS + hh], 0l, addr<uint8 const?>(plane[0]), br)
                }
            }
            nan_sentinel_upload(aod, bao)
            nan_sentinel_upload(ao2d, bao)
            var sets <- [for (hh in range(ROWS_HEADS + heads2));
                hh < ROWS_HEADS
                    ? set_tower_cn_attn_r64_cls(fixed_array(qd, kd, vd, aod, head_rel_planes[hh], wd), fixed_array(bx, bx, bx, bao, br, bw), fixed_array(0u, 0u, 0u, B_AO, 0u, 0u))
                    : set_tower_cn_attn_r_cls(fixed_array(qd, kd, vd, ao2d, head_rel_planes[hh], wd), fixed_array(bx, bx, bx, bao, br, bw), fixed_array(0u, 0u, 0u, B_AO2, 0u, 0u))]
            cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
                for (hh in range(ROWS_HEADS)) {
                    var pa = TowerCnAttnRArgs(d = uint(d), npos = uint(rows), buoff = 0u, hoff = uint(hh * hs))
                    enc_tower_cn_attn_r64_cls(raw, h, sets[hh], pa, wg)
                }
                for (hh in range(heads2)) {
                    var pa = TowerCnAttnRArgs(d = uint(d), npos = uint(rows), buoff = 0u, hoff = uint(hh * hs2))
                    enc_tower_cn_attn_r_cls(raw, h, sets[ROWS_HEADS + hh], pa, wg)
                }
                vhz_dep(raw, h, B_AO | B_AO2, 0u, true)
                cmd_copy_range(raw, aod, 0l, host.buf, 0l, bao)
                cmd_copy_range(raw, ao2d, 0l, host.buf, bao, bao)
            }
            let ao_dev <- staged_f32(host.mapped, 0l, nd)
            let ao2_dev <- staged_f32(host.mapped, bao, nd)
            let ao_idle <- staged_u32(host.mapped, bx, int(bidle / 4l))
            let ao2_idle <- staged_u32(host.mapped, bao + bx, int(bidle / 4l))
            t |> success(ao_rows > rows, "the dispatch carries idle lanes past the live rows ({ao_rows - rows} of {ao_rows})")
            t |> equal(sentinel_words(ao_idle) + sentinel_words(ao2_idle), 2 * (ao_rows - rows) * d, "tower_cn_attn_r: the idle lanes write nothing past the live rows on both stamps (every word the sentinel)")
            let ao_ref <- cn_attn_ref(q, k, v, posl, wn, rows, d, ROWS_HEADS, hs, 0, d)
            let ao2_ref <- cn_attn_ref(q, k, v, posl, wn, rows, d, heads2, hs2, 0, d)
            let bad64 = mismatch_approx(ao_dev, ao_ref)
            let bad128 = mismatch_approx(ao2_dev, ao2_ref)
            t |> success(bad64 == 0, "tower_cn_attn_r64: the rel-plane attention over four 64-wide heads matches the CPU loop ({bad64} of {nd} off)")
            t |> success(bad128 == 0, "tower_cn_attn_r: the same over two 128-wide heads ({bad128} of {nd} off)")
            t |> success(!approx(ao_dev[0], ao_ref[0] + 0.5) && !approx(ao2_dev[nd - 1], ao2_ref[nd - 1] + 0.5), "tower_cn_attn_r: the bar reds a poisoned element on both stamps")
            var k_scaled := k
            var posl_scaled := posl
            for (i in range(nd)) {
                k_scaled[i] *= 1.5
            }
            for (i in range(ww * d)) {
                posl_scaled[i] *= 1.5
            }
            let ctl_k <- cn_attn_ref(q, k_scaled, v, posl, wn, rows, d, ROWS_HEADS, hs, 0, d)
            let ctl_p <- cn_attn_ref(q, k, v, posl_scaled, wn, rows, d, heads2, hs2, 0, d)
            let moved_k = mismatch_approx(ao_dev, ctl_k)
            let moved_p = mismatch_approx(ao2_dev, ctl_p)
            t |> success(moved_k > nd / 4 && moved_p > nd / 4, "tower_cn_attn_r controls: scaled keys ({moved_k} moved) and a scaled rel table ({moved_p} moved) land the oracle elsewhere")
            for (b in fixed_array(qd, kd, vd, aod, ao2d, wd)) {
                destroy_device_buf(b)
            }
            for (b in head_rel_planes) {
                destroy_device_buf(b)
            }
            destroy_host_buf(host)
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}
