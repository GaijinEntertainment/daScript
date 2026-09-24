options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module test_vulkan_tower_kernels

require dastest/testing_boost public
require math
require daslib/f16_cvt   // f32_to_f16 / f16_to_f32: the oracle reads the K/V rows as the f16 shadows the tile reads
require dasllama/dasllama_plane   // PlaneF: the CPU rms helper's weight plane
require dasllama/dasllama_tower   // the CPU tower helpers the row classes mirror: rms_rows, clamp_rows, rope_neox_2d_rows, add_inplace_rows
require dasllama/dasllama_common   // requant_rows_q8_sized: the Q8_0 feed's oracle
require ?vulkan vulkan   // VkCommandBuffer: the one-shot bracket's block argument
require ?vulkan dasllama/dasllama_vulkan_common
require ?vulkan dasllama/dasllama_vulkan_classes
require ?vulkan _vkd_oracles   // attn_row_oracle over every key: the bidirectional oracle
require ?vulkan _compares

//! The vision towers' Vulkan kernel classes against their CPU oracles (a Vulkan device, else the
//! cells skip): the bidirectional flash tiles, and - as the tower arc lands them - the tower-only
//! classes (rope2d, layernorm rows, clamp rows, the gated activations, the window attention).

let private TOWER_NHEADS = 4
let private TOWER_ROWS = 77       //! one whole 64-row tile and a 13-row partial: the tail mask past kvlen runs on the last K step of both arms
let private TOWER_SLACK_ROWS = 64 //! shadow rows past kvlen that carry a large finite value: a leak through the tail mask reads as garbage, not as a small drift
let private TOWER_SENTINEL = 10000.0
let private TOWER_SCALE = 0.125

def private f16r(v : float) : float => f16_to_f32(f32_to_f16(v))

//! the four stamps' ensure / set / enc by arm, so the cell reads one shape
def private fa_bidir_ensure(khr : bool; hs : int) : bool {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        if (khr) {
            return hs == 64 ? ensure_fa_khr_h64_bidir_cls() : ensure_fa_khr_h128_bidir_cls()
        }
        return hs == 64 ? ensure_fa_cm2_h64_bidir_cls() : ensure_fa_cm2_h128_bidir_cls()
    } else {
        return false
    }
}

def private fa_bidir_enc(khr : bool; hs : int; raw; var h; var s; var pc; wgs : int64) {
    if (khr) {
        if (hs == 64) {
            enc_fa_khr_h64_bidir_cls(raw, h, s, pc, wgs)
        } else {
            enc_fa_khr_h128_bidir_cls(raw, h, s, pc, wgs)
        }
    } elif (hs == 64) {
        enc_fa_cm2_h64_bidir_cls(raw, h, s, pc, wgs)
    } else {
        enc_fa_cm2_h128_bidir_cls(raw, h, s, pc, wgs)
    }
}

//! the causal twin at the same head width, the control that the mask switch is the stamps' only difference
def private fa_causal_enc(khr : bool; hs : int; raw; var h; var s; var pc; wgs : int64) {
    if (khr) {
        if (hs == 64) {
            enc_fa_khr_h64_cls(raw, h, s, pc, wgs)
        } else {
            enc_fa_khr_h128_cls(raw, h, s, pc, wgs)
        }
    } elif (hs == 64) {
        enc_fa_cm2_h64_cls(raw, h, s, pc, wgs)
    } else {
        enc_fa_cm2_h128_cls(raw, h, s, pc, wgs)
    }
}

def private fa_causal_ensure(khr : bool; hs : int) : bool {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        if (khr) {
            return hs == 64 ? ensure_fa_khr_h64_cls() : ensure_fa_khr_h128_cls()
        }
        return hs == 64 ? ensure_fa_cm2_h64_cls() : ensure_fa_cm2_h128_cls()
    } else {
        return false
    }
}

//! the workgroups a flash dispatch takes: one per (head, query tile) - 64-row tiles on the cm2 arm, 16-row on KHR
def private fa_wgs(khr : bool; nheads, rows : int) : int64 => int64(nheads) * int64((rows + (khr ? 15 : 63)) / (khr ? 16 : 64))

//! one arm of the bidirectional cell: the tile over compact f32 q rows and f16 K/V shadows, against the two-pass
//! CPU oracle attending every key (the K/V rows rounded through f16 as the shadows are), the causal twin as the
//! control that the switch moves the output, the sentinel slack rows as the control that the tail mask holds
def private fa_bidir_arm(t : T?; khr : bool; hs : int) {   // nolint:STYLE038 — one arm: the fixture, both stamps, the oracle and its controls
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let tag = "{khr ? "fa_khr" : "fa_cm2"} h{hs} bidir"
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
        var sck = set_f16cvt_cls(fixed_array(kdev, khdev), fixed_array(m_bytes, sh_bytes), fixed_array(2u, 16u))
        var scv = set_f16cvt_cls(fixed_array(vdev, vhdev), fixed_array(m_bytes, sh_bytes), fixed_array(8u, 32u))
        var sfa = fa_stamp_set(khr, fixed_array(qdev, khdev, vhdev, yd), fixed_array(q_bytes, sh_bytes, sh_bytes, q_bytes), fixed_array(1u, 16u, 32u, 4u))
        var sfc = fa_stamp_set(khr, fixed_array(qdev, khdev, vhdev, ycd), fixed_array(q_bytes, sh_bytes, sh_bytes, q_bytes), fixed_array(1u, 16u, 32u, 64u))
        var pcv = ActArgs(nelem = uint(kpad * qd), nblk = 0u)
        var pcf = FaArgs(rows = uint(rows), w0 = 0u, qd = uint(qd), kvd = uint(qd), kv_mul = 1u, kvbase = 0u,
            scale = TOWER_SCALE, window = 0u, sinkoff = 0u)
        let cvtwgs = int64((kpad * qd / 4 + 255) / 256)
        cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
            enc_f16cvt_cls(raw, h, sck, pcv, cvtwgs)
            enc_f16cvt_cls(raw, h, scv, pcv, cvtwgs)
            fa_bidir_enc(khr, hs, raw, h, sfa, pcf, fa_wgs(khr, nheads, rows))
            fa_causal_enc(khr, hs, raw, h, sfc, pcf, fa_wgs(khr, nheads, rows))
            vhz_dep(raw, h, 4u | 64u, 0u, true)
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
        let bad = mismatch_approx(y, y_ref)
        t |> success(bad == 0, "{tag}: every query row attends every key ({bad} of {rows * qd} off the CPU oracle)")
        t |> success(logits_maxabs(y) < TOWER_SENTINEL * 0.01, "{tag}: the slack rows past kvlen stayed masked (max |o| {logits_maxabs(y)})")
        t |> success(!approx(y[rows * qd - 1], y_ref[rows * qd - 1] + 1.5), "{tag}: the bar reds a poisoned oracle element")
        //! the causal twin on the same planes: row 0 sees one key there and every key here, so its output moves
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
        //! ... while the last row sees every key on both stamps: its head rows agree to the bar
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
                if (!fa_bidir_ensure(arm.khr, arm.hs) || !fa_causal_ensure(arm.khr, arm.hs)) {
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

//! the Q8_0 requant compare: block scales within the kernel cells' bar, bytes within one step (the device's rounding of a
//! half-step value against the CPU's)
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

//! gelu_quick(x) = x . sigmoid(1.702 x) - the CPU LUT's closed form
def private gelu_quick_ref(x : float) : float => x * (1.0 / (1.0 + exp(-1.702 * x)))

[test]
def test_vkt_tower_rows(t0 : T?) {   // nolint:STYLE038 — one flat gate over the seven row classes: the fixture, one command buffer, seven oracles and their controls
    t0 |> run("the tower row classes (rms, clamp, clamp+cvt, clamp+rq, rope2d, post-add, geglu) == the CPU tower helpers") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tower_rms_cls() || !ensure_tower_clamp_cls() || !ensure_tower_clamp_cvt_cls()
                    || !ensure_tower_clamp_rq_cls() || !ensure_tower_rope2d_cls() || !ensure_tower_post_add_cls() || !ensure_tower_geglu_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let d = ROWS_D
            let rows = ROWS_N
            let ff = ROWS_FF
            let hs = d / ROWS_HEADS
            let pairs = hs / 4
            let nd = rows * d
            let nff = rows * ff
            var x <- row_fill(nd, 41, 7, 0.05, 2.1)
            var y <- row_fill(nd, 17, 3, 0.04, 1.9)
            var q <- row_fill(nd, 29, 11, 0.06, 2.8)
            var g <- row_fill(nff, 13, 5, 0.09, 4.0)
            var u <- row_fill(nff, 23, 9, 0.07, 3.3)
            var wn <- row_fill(2 * d, 7, 1, 0.01, -0.6)   //! two weight rows: the pre-norm's at 0, the post-norm's at d - both near 1
            var tabs <- row_fill(rows * 4 * pairs, 31, 2, 0.02, 0.97)   //! per row [cx | sx | cy | sy]: any values serve the oracle, which applies the same tables
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
            let bs = int64(nd / 32) * 4l
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
            let hostb = 4l * bx + bff + bh + bq + bs
            var host = make_host_buf(hostb, true, [cached = true])
            unsafe {
                upload_region_at(xd, 0l, addr<uint8 const?>(x[0]), bx)
                upload_region_at(yd, 0l, addr<uint8 const?>(y[0]), bx)
                upload_region_at(qd, 0l, addr<uint8 const?>(q[0]), bx)
                upload_region_at(gd, 0l, addr<uint8 const?>(g[0]), bff)
                upload_region_at(ud, 0l, addr<uint8 const?>(u[0]), bff)
                upload_region_at(wd, 0l, addr<uint8 const?>(wn[0]), bw)
                upload_region_at(td, 0l, addr<uint8 const?>(tabs[0]), btabs)
            }
            //! the hazard bits: x 1, y 2, xb 4, xb2 8, q 16, g 32, u 64, the feeds 128 / 256 / 512, the tables and weights unbit
            var s_rms = set_tower_rms_cls(fixed_array(xd, xbd, wd), fixed_array(bx, bx, bw), fixed_array(1u, 4u, 0u))
            var s_clamp = set_tower_clamp_cls(fixed_array(yd), fixed_array(bx), fixed_array(2u))
            var s_cvt = set_tower_clamp_cvt_cls(fixed_array(xd, xhd), fixed_array(bx, bh), fixed_array(1u, 128u))
            var s_rq = set_tower_clamp_rq_cls(fixed_array(xd, xqd, xsd), fixed_array(bx, bq, bs), fixed_array(1u, 256u, 512u))
            var s_rope = set_tower_rope2d_cls(fixed_array(qd, td), fixed_array(bx, btabs), fixed_array(16u, 0u))
            var s_post = set_tower_post_add_cls(fixed_array(xd, yd, xb2d, wd), fixed_array(bx, bx, bx, bw), fixed_array(1u, 2u, 8u, 0u))
            var s_geglu = set_tower_geglu_cls(fixed_array(gd, ud), fixed_array(bff, bff), fixed_array(32u, 64u))
            var pa_rms = TowerRowArgs(d = uint(d), rows = uint(rows), woff = 0u, eps = ROWS_EPS)
            var pa_post = TowerRowArgs(d = uint(d), rows = uint(rows), woff = uint(d), eps = ROWS_EPS, ln_on = 1u, lnoff = 0u)
            var pa_clamp = TowerClampArgs(nelem = uint(nd), lo = lo, hi = hi)
            var pa_cvt = TowerClampArgs(nelem = uint(nd), lo = lo2, hi = hi2)
            var pa_rq = TowerClampRqArgs(inbase = 0u, nblk = uint(nd / 32), lo = lo2, hi = hi2)
            var pa_rope = TowerRopeArgs(d = uint(d), nheads = uint(ROWS_HEADS), hs = uint(hs), pairs = uint(pairs), rows = uint(rows))
            var pa_geglu = TowerGegluArgs(nelem = uint(nff), glo = -2.0, ghi = 2.5, ulo = -1.5, uhi = 1.75, dlo = -1.0, dhi = 1.2)
            let wg4 = int64((nd / 4 + 255) / 256)
            cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_tower_rms_cls(raw, h, s_rms, pa_rms, int64(rows))
                enc_tower_clamp_cvt_cls(raw, h, s_cvt, pa_cvt, wg4)
                enc_tower_clamp_rq_cls(raw, h, s_rq, pa_rq, int64((nd / 4 + 255) / 256))
                enc_tower_rope2d_cls(raw, h, s_rope, pa_rope, int64((rows * ROWS_HEADS * pairs + 255) / 256))
                enc_tower_geglu_cls(raw, h, s_geglu, pa_geglu, int64((nff / 4 + 255) / 256))
                enc_tower_clamp_cls(raw, h, s_clamp, pa_clamp, wg4)
                enc_tower_post_add_cls(raw, h, s_post, pa_post, int64(rows))   //! last: it rewrites x, which the rms and the feeds read whole
                vhz_dep(raw, h, 1u | 2u | 4u | 8u | 16u | 32u | 128u | 256u | 512u, 0u, true)
                cmd_copy_range(raw, xbd, 0l, host.buf, 0l, bx)
                cmd_copy_range(raw, yd, 0l, host.buf, bx, bx)
                cmd_copy_range(raw, xd, 0l, host.buf, 2l * bx, bx)
                cmd_copy_range(raw, xb2d, 0l, host.buf, 3l * bx, bx)
                cmd_copy_range(raw, gd, 0l, host.buf, 4l * bx, bff)
                cmd_copy_range(raw, xhd, 0l, host.buf, 4l * bx + bff, bh)
                cmd_copy_range(raw, xqd, 0l, host.buf, 4l * bx + bff + bh, bq)
                cmd_copy_range(raw, xsd, 0l, host.buf, 4l * bx + bff + bh + bq, bs)
            }
            var xb_dev : array<float>
            var y_dev : array<float>
            var x_dev : array<float>
            var xb2_dev : array<float>
            var g_dev : array<float>
            var xh_dev : array<uint16>
            var xq_dev : array<int8>
            var xs_dev : array<float>
            xb_dev |> resize(nd)
            y_dev |> resize(nd)
            x_dev |> resize(nd)
            xb2_dev |> resize(nd)
            g_dev |> resize(nff)
            xh_dev |> resize(nd)
            xq_dev |> resize(nd)
            xs_dev |> resize(nd / 32)
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
            }
            var q_dev : array<float>
            q_dev |> resize(nd)
            cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
                vhz_dep(raw, h, 16u, 0u, true)
                cmd_copy_range(raw, qd, 0l, host.buf, 0l, bx)
            }
            unsafe {
                memcpy(addr<void?>(q_dev[0]), host.mapped, bx)
            }
            // the oracles, the CPU tower helpers over the same rows
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
            let bad_clamp = mismatch_exact_count(y_dev, y_ref)
            t |> success(bad_clamp == 0, "tower_clamp: the rows clamp in place exactly ({bad_clamp} of {nd} off)")
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
            var xq_ref : array<int8>
            var xs_ref : array<float>
            requant_rows_q8_sized(clamped_x, int64(d), int64(rows), xq_ref, xs_ref)
            let bad_rq = q8_rows_off(xq_dev, xs_dev, xq_ref, xs_ref)
            t |> success(bad_rq == 0, "tower_clamp_rq: the Q8_0 feed is the clamped rows requantized as the CPU does ({bad_rq} off)")
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
            rms_rows(ypost, y_ref, wplane, int64(d), int64(d), int64(rows), ROWS_EPS)   //! the post-add reads y after the clamp the same command applied to it
            add_inplace_rows(x_ref, ypost, int64(nd))
            let bad_add = mismatch_approx(x_dev, x_ref)
            t |> success(bad_add == 0, "tower_post_add: x += rms(y) . wpost ({bad_add} of {nd} off)")
            var xb2_ref : array<float>
            xb2_ref |> resize(nd)
            rms_rows(xb2_ref, x_ref, wplane, 0l, int64(d), int64(rows), ROWS_EPS)
            let bad_ln = mismatch_approx(xb2_dev, xb2_ref)
            t |> success(bad_ln == 0, "tower_post_add: the next pre-norm over the updated rows ({bad_ln} of {nd} off)")
            t |> success(!approx(xb2_dev[nd - 1], xb2_ref[nd - 1] + 0.5), "tower_post_add: the bar reds a poisoned element")
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
            for (b in fixed_array(xd, yd, xbd, xb2d, qd, gd, ud, wd, td, xhd, xqd, xsd)) {
                destroy_device_buf(b)
            }
            destroy_host_buf(host)
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! exact element-for-element mismatch count (the clamp writes the same floats the CPU does)
def private mismatch_exact_count(got, want : array<float>) : int {
    var bad = 0
    for (a, b in got, want) {
        if (a != b) {
            bad++
        }
    }
    return bad
}
