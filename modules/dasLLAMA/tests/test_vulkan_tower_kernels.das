options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module test_vulkan_tower_kernels

require dastest/testing_boost public
require daslib/f16_cvt   // f32_to_f16 / f16_to_f32: the oracle reads the K/V rows as the f16 shadows the tile reads
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
