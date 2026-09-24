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
require dasllama/dasllama_math   // setup_dasllama_jobque_ - the CPU attention oracle runs on the que
require daslib/jobque_boost

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
            let d = ROWS_D
            let rows = ROWS_N
            let nd = rows * d
            let dc = ROWS_HEADS * LN_HS    //! the compact attention width
            let dp = ROWS_HEADS * LN_HSP   //! the padded one
            var x <- row_fill(nd, 41, 7, 0.05, 2.1)
            var y <- row_fill(nd, 17, 3, 0.04, 1.9)
            var g <- row_fill(nd, 13, 5, 0.3, 14.0)   //! wide enough to reach both tails of the GELU (past +-10)
            var q <- row_fill(rows * dc, 29, 11, 0.06, 2.8)
            var qkv <- row_fill(rows * 3 * dc, 37, 5, 0.05, 2.2)   //! a fused [q | k | v] row: the rope runs on its k slot in place
            let half = LN_HS / 2
            var cos_t <- row_fill(rows * half, 31, 2, 0.02, 0.97)
            var sin_t <- row_fill(rows * half, 19, 4, 0.03, 0.55)
            var wn <- row_fill(4 * d, 7, 1, 0.01, -0.6)   //! rows: ln w at 0, ln b at d, the bias at 2d, the next ln w at 3d (its bias reads the first ln's)
            let bx = int64(nd) * 4l
            let bw = int64(4 * d) * 4l
            let bq = int64(rows * dc) * 4l
            let bp = int64(rows * dp) * 4l
            let bph = int64(rows * dp) * 2l
            let xd = make_device_buf(bx)
            let xbd = make_device_buf(bx)
            let yd = make_device_buf(bx)
            let gd = make_device_buf(bx)
            let xb2d = make_device_buf(bx)
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
            let hostb = 4l * bx + bp + bph + bq + bqkv
            var host = make_host_buf(hostb, true, [cached = true])
            unsafe {
                upload_region_at(xd, 0l, addr<uint8 const?>(x[0]), bx)
                upload_region_at(yd, 0l, addr<uint8 const?>(y[0]), bx)
                upload_region_at(gd, 0l, addr<uint8 const?>(g[0]), bx)
                upload_region_at(wd, 0l, addr<uint8 const?>(wn[0]), bw)
                upload_region_at(qd, 0l, addr<uint8 const?>(q[0]), bq)
                upload_region_at(qkvd, 0l, addr<uint8 const?>(qkv[0]), bqkv)
                upload_region_at(cosd, 0l, addr<uint8 const?>(cos_t[0]), btab)
                upload_region_at(sind, 0l, addr<uint8 const?>(sin_t[0]), btab)
            }
            var s_rope = set_tower_rope_tab_cls(fixed_array(qkvd, cosd, sind), fixed_array(bqkv, btab, btab), fixed_array(512u, 0u, 0u))
            var pa_rope = TowerRopeTabArgs(sstride = uint(3 * dc), soff = uint(dc), nheads = uint(ROWS_HEADS), hs = uint(LN_HS), half = uint(half), rows = uint(rows))
            var s_ln = set_tower_ln_cls(fixed_array(xd, xbd, wd), fixed_array(bx, bx, bw), fixed_array(1u, 2u, 0u))
            var s_bias = set_tower_bias_act_cls(fixed_array(gd, wd), fixed_array(bx, bw), fixed_array(4u, 0u))
            var s_post = set_tower_post_add_ln_cls(fixed_array(xd, yd, xb2d, wd), fixed_array(bx, bx, bx, bw), fixed_array(1u, 8u, 16u, 0u))
            var s_pad = set_tower_hpad_cls(fixed_array(qd, qpd), fixed_array(bq, bp), fixed_array(32u, 64u))
            var s_pad16 = set_tower_hpad16_cls(fixed_array(qd, qphd), fixed_array(bq, bph), fixed_array(32u, 128u))
            var s_unpad = set_tower_hunpad_cls(fixed_array(qpd, qcd), fixed_array(bp, bq), fixed_array(64u, 256u))
            var pa_ln = TowerLnArgs(d = uint(d), rows = uint(rows), woff = 0u, boff = uint(d), eps = ROWS_EPS)
            var pa_bias = TowerBiasArgs(d = uint(d), nelem = uint(nd), boff = uint(2 * d), act = 1u)
            var pa_post = TowerLnArgs(d = uint(d), rows = uint(rows), boff = uint(2 * d), eps = ROWS_EPS, ln_on = 1u, lnoff = uint(3 * d), lnboff = uint(d))
            var pa_rs = TowerRestrideArgs(rows = uint(rows), nheads = uint(ROWS_HEADS), hs = uint(LN_HS), hsp = uint(LN_HSP), sstride = uint(dc), soff = 0u)
            let wg4 = int64((nd / 4 + 255) / 256)
            let wg_rs = int64((rows * dp + 255) / 256)
            cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_tower_ln_cls(raw, h, s_ln, pa_ln, int64(rows))
                enc_tower_bias_act_cls(raw, h, s_bias, pa_bias, wg4)
                enc_tower_hpad_cls(raw, h, s_pad, pa_rs, wg_rs)
                enc_tower_hpad16_cls(raw, h, s_pad16, pa_rs, wg_rs)
                enc_tower_hunpad_cls(raw, h, s_unpad, pa_rs, wg_rs)
                enc_tower_rope_tab_cls(raw, h, s_rope, pa_rope, int64((rows * ROWS_HEADS * half + 255) / 256))
                enc_tower_post_add_ln_cls(raw, h, s_post, pa_post, int64(rows))   //! last: it rewrites x, which the layernorm reads whole
                vhz_dep(raw, h, 1u | 2u | 4u | 16u | 64u | 128u | 256u | 512u, 0u, true)
                cmd_copy_range(raw, xbd, 0l, host.buf, 0l, bx)
                cmd_copy_range(raw, gd, 0l, host.buf, bx, bx)
                cmd_copy_range(raw, xd, 0l, host.buf, 2l * bx, bx)
                cmd_copy_range(raw, xb2d, 0l, host.buf, 3l * bx, bx)
                cmd_copy_range(raw, qpd, 0l, host.buf, 4l * bx, bp)
                cmd_copy_range(raw, qphd, 0l, host.buf, 4l * bx + bp, bph)
                cmd_copy_range(raw, qcd, 0l, host.buf, 4l * bx + bp + bph, bq)
                cmd_copy_range(raw, qkvd, 0l, host.buf, 4l * bx + bp + bph + bq, bqkv)
            }
            var qkv_dev : array<float>
            qkv_dev |> resize(rows * 3 * dc)
            var xb_dev : array<float>
            var g_dev : array<float>
            var x_dev : array<float>
            var xb2_dev : array<float>
            var qp_dev : array<float>
            var qph_dev : array<uint16>
            var qc_dev : array<float>
            xb_dev |> resize(nd)
            g_dev |> resize(nd)
            x_dev |> resize(nd)
            xb2_dev |> resize(nd)
            qp_dev |> resize(rows * dp)
            qph_dev |> resize(rows * dp)
            qc_dev |> resize(rows * dc)
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
            }
            // the oracles
            var xb_ref : array<float>
            xb_ref |> resize(nd)
            layernorm_batch(xb_ref, x, wn, 0l, int64(d), int64(d), int64(rows), ROWS_EPS)
            let bad_ln = mismatch_approx(xb_dev, xb_ref)
            t |> success(bad_ln == 0, "tower_ln: the layernorm rows match layernorm_batch ({bad_ln} of {nd} off)")
            t |> success(!approx(xb_dev[nd - 1], xb_ref[nd - 1] + 0.5), "tower_ln: the bar reds a poisoned element")
            var g_ref := g
            add_bias_rows(g_ref, wn, int64(2 * d), int64(d), int64(rows))
            gelu_tanh_lut_batch(g_ref, int64(d), int64(rows))
            let bad_bias = mismatch_approx(g_dev, g_ref)
            t |> success(bad_bias == 0, "tower_bias_act: the bias and the tanh GELU match add_bias_rows + gelu_tanh_lut_batch ({bad_bias} of {nd} off)")
            t |> success(!approx(g_dev[0], g_ref[0] + 0.5), "tower_bias_act: the bar reds a poisoned element")
            var x_ref := x
            var yb := y
            add_bias_rows(yb, wn, int64(2 * d), int64(d), int64(rows))
            add_inplace_rows(x_ref, yb, int64(nd))
            let bad_add = mismatch_approx(x_dev, x_ref)
            t |> success(bad_add == 0, "tower_post_add_ln: x += y + b ({bad_add} of {nd} off)")
            var xb2_ref : array<float>
            xb2_ref |> resize(nd)
            layernorm_batch(xb2_ref, x_ref, wn, int64(3 * d), int64(d), int64(d), int64(rows), ROWS_EPS)
            let bad_ln2 = mismatch_approx(xb2_dev, xb2_ref)
            t |> success(bad_ln2 == 0, "tower_post_add_ln: the next layernorm over the updated rows ({bad_ln2} of {nd} off)")
            t |> success(!approx(xb2_dev[nd - 1], xb2_ref[nd - 1] + 0.5), "tower_post_add_ln: the bar reds a poisoned element")
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
            t |> success(bad_pad16 == 0, "tower_hpad16: the f16 panel is that panel rounded to f16, bit for bit ({bad_pad16} off)")
            let bad_unpad = mismatch_exact_count(qc_dev, q)
            t |> success(bad_unpad == 0, "tower_hunpad: the compact rows come back off the padded panel exactly ({bad_unpad} off)")
            // the rope on the fused row's k slot: the CPU form over the slot's rows gathered compact, the q and v slots untouched
            var k_ref : array<float>
            var k_dev : array<float>
            k_ref |> resize(rows * dc)
            k_dev |> resize(rows * dc)
            var bad_slots = 0
            for (r in range(rows)) {
                for (i in range(dc)) {
                    k_ref[r * dc + i] = qkv[r * 3 * dc + dc + i]
                    k_dev[r * dc + i] = qkv_dev[r * 3 * dc + dc + i]
                    if (qkv_dev[r * 3 * dc + i] != qkv[r * 3 * dc + i] || qkv_dev[r * 3 * dc + 2 * dc + i] != qkv[r * 3 * dc + 2 * dc + i]) {
                        bad_slots++
                    }
                }
            }
            rope_neox_tab_rows(k_ref, int64(rows), int64(ROWS_HEADS), int64(LN_HS), cos_t, sin_t)
            let bad_rope = mismatch_approx(k_dev, k_ref)
            t |> success(bad_rope == 0, "tower_rope_tab: the k slot of the fused rows matches rope_neox_tab_rows ({bad_rope} of {rows * dc} off)")
            t |> success(bad_slots == 0, "tower_rope_tab: the q and v slots stay untouched ({bad_slots} moved)")
            t |> success(!approx(k_dev[0], k_ref[0] + 0.5), "tower_rope_tab: the bar reds a poisoned element")
            for (b in fixed_array(xd, xbd, yd, gd, xb2d, wd, qd, qpd, qphd, qcd, qkvd, cosd, sind)) {
                destroy_device_buf(b)
            }
            destroy_host_buf(host)
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

let private PAT_HEADS = 16   //! the SigLIP head count: sixteen 72-wide heads through the padded 128 route

[test]
def test_vkt_tower_padded_attn(t0 : T?) {   // nolint:STYLE038 — the padded route end to end: the fixture, the command buffer, the oracle and its controls
    t0 |> run("the padded attention route (pad, the h128 bidirectional tile, unpad) == attention_bidir at 72-wide heads") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init()) {
                t |> skip("no Vulkan device")
                return
            }
            let khr = !cm2_fa_on()
            let fa_ok = khr ? ensure_fa_khr_h128_bidir_cls() : ensure_fa_cm2_h128_bidir_cls()
            if (!fa_ok || !ensure_tower_hpad_cls() || !ensure_tower_hpad16_cls() || !ensure_tower_hunpad_cls()) {
                t |> skip("the padded route's classes declined on this device")
                return
            }
            let rows = TOWER_ROWS
            let dc = PAT_HEADS * LN_HS
            let dp = PAT_HEADS * LN_HSP
            let scale = 1.0 / sqrt(float(LN_HS))
            var q <- row_fill(rows * dc, 29, 11, 0.02, 0.9)
            var k <- row_fill(rows * dc, 17, 3, 0.03, 1.4)
            var v <- row_fill(rows * dc, 53, 7, 0.04, 1.7)
            for (i in range(rows * dc)) {   //! the shadows round K/V through f16: the oracle sees the same values
                k[i] = f16r(k[i])
                v[i] = f16r(v[i])
            }
            let bc = int64(rows * dc) * 4l
            let bp = int64(rows * dp) * 4l
            let bph = int64(rows * dp) * 2l
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
            var s_pq = set_tower_hpad_cls(fixed_array(qd, qpd), fixed_array(bc, bp), fixed_array(1u, 2u))
            var s_pk = set_tower_hpad16_cls(fixed_array(kd, kpd), fixed_array(bc, bph), fixed_array(4u, 8u))
            var s_pv = set_tower_hpad16_cls(fixed_array(vd, vpd), fixed_array(bc, bph), fixed_array(16u, 32u))
            var s_fa = fa_stamp_set(khr, fixed_array(qpd, kpd, vpd, attpd), fixed_array(bp, bph, bph, bp), fixed_array(2u, 8u, 32u, 64u))
            var s_un = set_tower_hunpad_cls(fixed_array(attpd, attd), fixed_array(bp, bc), fixed_array(64u, 128u))
            var pa_rs = TowerRestrideArgs(rows = uint(rows), nheads = uint(PAT_HEADS), hs = uint(LN_HS), hsp = uint(LN_HSP), sstride = uint(dc), soff = 0u)
            var pa_fa = FaArgs(rows = uint(rows), w0 = 0u, qd = uint(dp), kvd = uint(dp), kv_mul = 1u, kvbase = 0u, scale = scale, window = 0u, sinkoff = 0u)
            let wg_rs = int64((rows * dp + 255) / 256)
            cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_tower_hpad_cls(raw, h, s_pq, pa_rs, wg_rs)
                enc_tower_hpad16_cls(raw, h, s_pk, pa_rs, wg_rs)
                enc_tower_hpad16_cls(raw, h, s_pv, pa_rs, wg_rs)
                fa_bidir_enc(khr, LN_HSP, raw, h, s_fa, pa_fa, fa_wgs(khr, PAT_HEADS, rows))
                enc_tower_hunpad_cls(raw, h, s_un, pa_rs, wg_rs)
                vhz_dep(raw, h, 128u, 0u, true)
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

let private WIN_HS = 80   //! the window ViT's head width, padded to 128 on the tile
let private WIN_HEADS = 4
let private WIN_ROWS = 982   //! sixteen windows: fifteen full 64-row slots and a ragged 22-row one

[test]
def test_vkt_tower_win_rows(t0 : T?) {   // nolint:STYLE038 — the window route end to end plus the two seams: the fixture, the command buffer, the oracles and their controls
    t0 |> run("the window-attention classes (slotted pads, the block-diagonal tile, the rms seam, the gated hidden) == the CPU tower helpers") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init()) {
                t |> skip("no Vulkan device")
                return
            }
            let khr = !cm2_fa_on()
            let fa_ok = khr ? ensure_fa_khr_h128_win_cls() : ensure_fa_cm2_h128_win_cls()
            if (!fa_ok || !ensure_tower_hpad_slot_cls() || !ensure_tower_hpad16_slot_cls() || !ensure_tower_hunpad_slot_cls()
                    || !ensure_tower_post_add_rms_cls() || !ensure_tower_glu_act_cls()) {
                t |> skip("the window route's classes declined on this device")
                return
            }
            let rows = WIN_ROWS
            let dc = WIN_HEADS * WIN_HS
            let dp = WIN_HEADS * LN_HSP
            let scale = 1.0 / sqrt(float(WIN_HS))
            let nslot = (rows + 63) / 64
            var win_lo : array<int64>
            win_lo |> reserve(nslot + 1)
            for (s in range(nslot + 1)) {
                win_lo |> push(min(int64(s) * 64l, int64(rows)))
            }
            let srows = nslot * 64
            var rmap : array<int>
            var wcnt : array<uint>
            rmap |> resize(srows)
            wcnt |> resize(nslot)
            for (s in range(nslot)) {
                let lo = int(win_lo[s])
                let cnt = int(win_lo[s + 1]) - lo
                wcnt[s] = uint(cnt)
                for (i in range(64)) {
                    rmap[s * 64 + i] = i < cnt ? lo + i : -1
                }
            }
            var q <- row_fill(rows * dc, 29, 11, 0.02, 0.9)
            var k <- row_fill(rows * dc, 17, 3, 0.03, 1.4)
            var v <- row_fill(rows * dc, 53, 7, 0.04, 1.7)
            for (i in range(rows * dc)) {
                k[i] = f16r(k[i])
                v[i] = f16r(v[i])
            }
            let bc = int64(rows * dc) * 4l
            let bp = int64(srows * dp) * 4l
            let bph = int64(srows * dp) * 2l
            let brm = int64(srows) * 4l
            let bwc = int64(nslot) * 4l
            let qd = make_device_buf(bc)
            let kd = make_device_buf(bc)
            let vd = make_device_buf(bc)
            let qpd = make_device_buf(bp)
            let kpd = make_device_buf(bph)
            let vpd = make_device_buf(bph)
            let attpd = make_device_buf(bp)
            let attd = make_device_buf(bc)
            let rmd = make_device_buf(brm)
            let wcd = make_device_buf(bwc)
            // the two seams' fixture: d-wide rows, a bias and rms weight rows at wn
            let d = ROWS_D
            let nrows = ROWS_N
            let nd = nrows * d
            var x <- row_fill(nd, 41, 7, 0.05, 2.1)
            var y <- row_fill(nd, 17, 3, 0.04, 1.9)
            var g <- row_fill(nd, 13, 5, 0.09, 4.0)
            var u <- row_fill(nd, 23, 9, 0.07, 3.3)
            var wn <- row_fill(4 * d, 7, 1, 0.01, -0.6)   //! rows: the o bias at 0, the rms weight at d, the gate bias at 2d, the up bias at 3d
            let bx = int64(nd) * 4l
            let bw = int64(4 * d) * 4l
            let xd = make_device_buf(bx)
            let yd = make_device_buf(bx)
            let xbd = make_device_buf(bx)
            let gd = make_device_buf(bx)
            let ud = make_device_buf(bx)
            let wd = make_device_buf(bw)
            var host = make_host_buf(bc + 3l * bx, true, [cached = true])
            unsafe {
                upload_region_at(qd, 0l, addr<uint8 const?>(q[0]), bc)
                upload_region_at(kd, 0l, addr<uint8 const?>(k[0]), bc)
                upload_region_at(vd, 0l, addr<uint8 const?>(v[0]), bc)
                upload_region_at(rmd, 0l, addr<uint8 const?>(rmap[0]), brm)
                upload_region_at(wcd, 0l, addr<uint8 const?>(wcnt[0]), bwc)
                upload_region_at(xd, 0l, addr<uint8 const?>(x[0]), bx)
                upload_region_at(yd, 0l, addr<uint8 const?>(y[0]), bx)
                upload_region_at(gd, 0l, addr<uint8 const?>(g[0]), bx)
                upload_region_at(ud, 0l, addr<uint8 const?>(u[0]), bx)
                upload_region_at(wd, 0l, addr<uint8 const?>(wn[0]), bw)
            }
            var s_pq = set_tower_hpad_slot_cls(fixed_array(qd, qpd, rmd), fixed_array(bc, bp, brm), fixed_array(1u, 2u, 0u))
            var s_pk = set_tower_hpad16_slot_cls(fixed_array(kd, kpd, rmd), fixed_array(bc, bph, brm), fixed_array(4u, 8u, 0u))
            var s_pv = set_tower_hpad16_slot_cls(fixed_array(vd, vpd, rmd), fixed_array(bc, bph, brm), fixed_array(16u, 32u, 0u))
            var s_fa : VkDescriptorSet
            if (khr) {
                s_fa = set_fa_khr_h128_win_cls(fixed_array(qpd, kpd, vpd, attpd, wcd), fixed_array(bp, bph, bph, bp, bwc), fixed_array(2u, 8u, 32u, 64u, 0u))
            } else {
                s_fa = set_fa_cm2_h128_win_cls(fixed_array(qpd, kpd, vpd, attpd, wcd), fixed_array(bp, bph, bph, bp, bwc), fixed_array(2u, 8u, 32u, 64u, 0u))
            }
            var s_un = set_tower_hunpad_slot_cls(fixed_array(attpd, attd, rmd), fixed_array(bp, bc, brm), fixed_array(64u, 128u, 0u))
            var s_post = set_tower_post_add_rms_cls(fixed_array(xd, yd, xbd, wd), fixed_array(bx, bx, bx, bw), fixed_array(256u, 512u, 1024u, 0u))
            var s_glu = set_tower_glu_act_cls(fixed_array(gd, ud, wd), fixed_array(bx, bx, bw), fixed_array(2048u, 4096u, 0u))
            var pa_rs = TowerRestrideArgs(rows = uint(srows), nheads = uint(WIN_HEADS), hs = uint(WIN_HS), hsp = uint(LN_HSP), sstride = uint(dc), soff = 0u)
            var pa_fa = FaArgs(rows = uint(srows), w0 = 0u, qd = uint(dp), kvd = uint(dp), kv_mul = 1u, kvbase = 0u, scale = scale, window = 0u, sinkoff = 0u)
            var pa_post = TowerLnArgs(d = uint(d), rows = uint(nrows), boff = 0u, eps = ROWS_EPS, ln_on = 1u, lnoff = uint(d), lnboff = 0u)
            var pa_glu = TowerGluArgs(d = uint(d), nelem = uint(nd), bgoff = uint(2 * d), buoff = uint(3 * d))
            let wg_rs = int64((srows * dp + 255) / 256)
            let wg4 = int64((nd / 4 + 255) / 256)
            cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_tower_hpad_slot_cls(raw, h, s_pq, pa_rs, wg_rs)
                enc_tower_hpad16_slot_cls(raw, h, s_pk, pa_rs, wg_rs)
                enc_tower_hpad16_slot_cls(raw, h, s_pv, pa_rs, wg_rs)
                if (khr) {
                    enc_fa_khr_h128_win_cls(raw, h, s_fa, pa_fa, fa_wgs(true, WIN_HEADS, srows))
                } else {
                    enc_fa_cm2_h128_win_cls(raw, h, s_fa, pa_fa, fa_wgs(false, WIN_HEADS, srows))
                }
                enc_tower_hunpad_slot_cls(raw, h, s_un, pa_rs, wg_rs)
                enc_tower_glu_act_cls(raw, h, s_glu, pa_glu, wg4)
                enc_tower_post_add_rms_cls(raw, h, s_post, pa_post, int64(nrows))
                vhz_dep(raw, h, 128u | 256u | 1024u | 2048u, 0u, true)
                cmd_copy_range(raw, attd, 0l, host.buf, 0l, bc)
                cmd_copy_range(raw, xd, 0l, host.buf, bc, bx)
                cmd_copy_range(raw, xbd, 0l, host.buf, bc + bx, bx)
                cmd_copy_range(raw, gd, 0l, host.buf, bc + 2l * bx, bx)
            }
            var att_dev : array<float>
            var x_dev : array<float>
            var xb_dev : array<float>
            var g_dev : array<float>
            att_dev |> resize(rows * dc)
            x_dev |> resize(nd)
            xb_dev |> resize(nd)
            g_dev |> resize(nd)
            unsafe {
                let hp = intptr(host.mapped)
                memcpy(addr<void?>(att_dev[0]), host.mapped, bc)
                memcpy(addr<void?>(x_dev[0]), reinterpret<void?>(hp + uint64(bc)), bx)
                memcpy(addr<void?>(xb_dev[0]), reinterpret<void?>(hp + uint64(bc + bx)), bx)
                memcpy(addr<void?>(g_dev[0]), reinterpret<void?>(hp + uint64(bc + 2l * bx)), bx)
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
            t |> success(bad == 0, "the window route matches attention_bidir_windows over {nslot} windows, one ragged ({bad} of {rows * dc} off)")
            t |> success(!approx(att_dev[0], att_ref[0] + 0.5), "the window route: the bar reds a poisoned element")
            //! the leak control: the same rows attended in full differ on the first row, so the slots' walls are the stamps' doing
            var full_ref : array<float>
            full_ref |> resize(rows * dc)
            with_job_que() {
                setup_dasllama_jobque_()
                attention_bidir(full_ref, q, k, v, hpp, attsc, int64(rows), int64(dc), int64(WIN_HEADS), scale)
            }
            var moved = 0.0
            for (i in range(dc)) {
                moved = max(moved, abs(full_ref[i] - att_ref[i]))
            }
            t |> success(moved > 0.01, "the window route control: full attention over the same rows lands elsewhere (maxdiff {moved})")
            var x_ref := x
            var yb := y
            add_bias_rows(yb, wn, 0l, int64(d), int64(nrows))
            add_inplace_rows(x_ref, yb, int64(nd))
            let bad_add = mismatch_approx(x_dev, x_ref)
            t |> success(bad_add == 0, "tower_post_add_rms: x += y + b ({bad_add} of {nd} off)")
            var xb_ref : array<float>
            xb_ref |> resize(nd)
            let wplane = PlaneF(p = unsafe(addr<float const?>(wn[0])), n = int64(4 * d))
            rms_rows(xb_ref, x_ref, wplane, int64(d), int64(d), int64(nrows), ROWS_EPS)
            let bad_rms = mismatch_approx(xb_dev, xb_ref)
            t |> success(bad_rms == 0, "tower_post_add_rms: the next weighted rms over the updated rows ({bad_rms} of {nd} off)")
            t |> success(!approx(xb_dev[nd - 1], xb_ref[nd - 1] + 0.5), "tower_post_add_rms: the bar reds a poisoned element")
            var g_ref := g
            var u_ref := u
            add_bias_rows(g_ref, wn, int64(2 * d), int64(d), int64(nrows))
            add_bias_rows(u_ref, wn, int64(3 * d), int64(d), int64(nrows))
            silu(g_ref, int64(nd))
            for (i in range(nd)) {
                g_ref[i] *= u_ref[i]
            }
            let bad_glu = mismatch_approx(g_dev, g_ref)
            t |> success(bad_glu == 0, "tower_glu_act: silu(g + bg) . (u + bu) matches the CPU form ({bad_glu} of {nd} off)")
            t |> success(!approx(g_dev[0], g_ref[0] + 0.5), "tower_glu_act: the bar reds a poisoned element")
            for (b in fixed_array(qd, kd, vd, qpd, kpd, vpd, attpd, attd, rmd, wcd, xd, yd, xbd, gd, ud, wd)) {
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
