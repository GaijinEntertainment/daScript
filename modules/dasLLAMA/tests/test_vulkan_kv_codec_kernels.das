options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module test_vulkan_kv_codec_kernels

require dastest/testing_boost public
require ?vulkan vulkan
require ?vulkan dasllama/dasllama_vulkan_common
require ?vulkan dasllama/dasllama_vulkan_classes
require ?vulkan _vkd_oracles
require ?vulkan _vk_codec_attn
require _compares
require dasllama/dasllama_common
require daslib/f16_cvt   // nolint:STYLE030,LINT019 — used only inside the vulkan static_if arms; the no-vulkan lane sees it unused
require daslib/algorithm
require math   // nolint:STYLE030,LINT019 - used only inside the vulkan static_if arms (a build without the module drops them)

//! The Vulkan K/V mirror's quant codecs against the CPU cache's own codec (a Vulkan device, else the cells skip): the stores'
//! bytes against `kv_store_row`, every byte outside the stored rows left at its fill; the decode attention's codec stamps
//! against the CPU attention over the rows `kv_load_row` reads back, on every path the pass has; the f16 shadow pass against
//! those rows rounded to half, bit for bit; tq4's un-rotation against `tq4_unrotate_batch`.

let private MIRROR_FILL = 0xA5A5A5A5u
let private MIRROR_FILL_BYTE = 0xA5u8
let private ODD_LAYERBASE = 7     //! blocks before a layer's rows: odd, so its blocks start two bytes off a word
let private SLACK_BLOCKS = 1
let private HALF_MAX = uint16(0x7BFF)   //! 65504, the largest finite half: where an f16 store clamps
let private HALF_NAN = uint16(0x7E00)
let private HALF_NAN_PAIR = 0x7E007E00u
let private PAST_HALF_RANGE = 4.0e7   //! times a fixture's values: a block scale past 65504, which the device clamps
let private PAST_HALF_VALUES = 3.0e4  //! times a fixture's values: dequantized values to 1e5 under a block scale inside the half range

def private block_bytes(kv : KVDtype) : int => int(kv_unit_bytes(kv))

//! the mirror plane the CPU codec leaves: `MIRROR_FILL`, then row `r` of `staged` (in the stored basis) at block `blocks[r]`
def private codec_want(kv : KVDtype; staged : array<float>; blocks : array<int>; mir_bytes, kvd : int) : array<uint8> {
    var want : array<uint8>
    want |> resize(mir_bytes)
    want |> fill(MIRROR_FILL_BYTE)
    for (r in range(length(blocks))) {
        kv_store_row(unsafe(addr<uint8?>(want[blocks[r] * block_bytes(kv)])), kv, int64(kvd), 0l, 0l, int64(kvd), unsafe(addr<float const?>(staged[r * kvd])))
    }
    return <- want
}

//! Every block scale of `bytes` the CPU stored as an infinite half, set to the half the device clamps it to; the count set.
def private clamp_inf_scales(var bytes : array<uint8>; kv : KVDtype) : int {
    var clamped = 0
    for (b in range(length(bytes) / block_bytes(kv))) {
        let at = b * block_bytes(kv)
        let hi = uint(bytes[at + 1])
        if ((hi & 0x7Fu) == 0x7Cu && bytes[at] == 0u8) {
            bytes[at] = 0xFFu8
            bytes[at + 1] = uint8((hi & 0x80u) | 0x7Bu)
            clamped++
        }
    }
    return clamped
}

let private KS_KVD = 96
let private KS_ROWS = 3
let private KS_POS_PER_REGION = 12
let private KS_ROW_BLOCKS = KS_KVD / 32
let private KS_REGION_BLOCKS = KS_POS_PER_REGION * KS_ROW_BLOCKS

//! a mirror plane of `blocks` blocks past the odd layer base and a slack block, to a whole word
def private mirror_bytes(kv : KVDtype; blocks : int) : int => ((ODD_LAYERBASE + blocks + SLACK_BLOCKS) * block_bytes(kv) + 3) / 4 * 4

//! the staged K and V rows: every block's largest magnitude on a different element, V's middle block of row 1 all zero, K's
//! last block of row 2 and V's first block of row 2 past the half range
def private ks_staged() : tuple<k : array<float>; v : array<float>> {
    var k <- row_fill(KS_ROWS * KS_KVD, 37, 11, 0.071, 3.3)
    var v <- row_fill(KS_ROWS * KS_KVD, 29, 5, 0.043, 2.0, 89)
    for (i in range(32)) {
        v[KS_KVD + 32 + i] = 0.0
        k[2 * KS_KVD + 64 + i] *= PAST_HALF_RANGE
        v[2 * KS_KVD + i] *= PAST_HALF_RANGE
    }
    k[5] = -9.25   // a negative block maximum: the quant reads -127
    return <- (k = k, v = v)
}

//! one q8_0 store arm: the token stamp (rows at their token blocks' positions, two regions) or the window stamp (rows from `pos0`)
def private q8_store_arm(t : T?; window : bool) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        var st <- ks_staged()
        let wgs = int64((KS_ROW_BLOCKS * KS_ROWS + 255) / 256)
        let pos0 = 4
        var toks : array<TokMeta>
        toks |> resize(KS_ROWS)
        toks[0].pos = 5u
        toks[1].pos = 2u
        toks[1].mirbase = uint(KS_REGION_BLOCKS)
        toks[2].pos = 9u
        var blocks <- [for (r in range(KS_ROWS)); ODD_LAYERBASE + (window ? (pos0 + r) * KS_ROW_BLOCKS : int(toks[r].mirbase) + int(toks[r].pos) * KS_ROW_BLOCKS)]
        let tag = window ? "kv_q8_store_b rows from pos0" : "kv_q8_store rows at their token positions"
        let mir_bytes = mirror_bytes(KVDtype.q8_0, 2 * KS_REGION_BLOCKS)
        let scale_bytes = 2
        with_cell_planes() $(var cp : CellPlanes) {
            let kp = cp |> in_(st.k)
            let vp = cp |> in_(st.v)
            let km = cp |> out_words(int64(mir_bytes), MIRROR_FILL)
            let vm = cp |> out_words(int64(mir_bytes), MIRROR_FILL)
            let tp = cp |> in_plain(toks)
            var s = window ? cp |> cls_set(fixed_array(kp, vp, km, vm), @@set_kv_q8_store_b_cls) : cp |> cls_set(fixed_array(kp, vp, km, vm, tp), @@set_kv_q8_store_cls)
            var pa = KvQStoreArgs(kvd = uint(KS_KVD), rows = uint(KS_ROWS), layerbase = uint(ODD_LAYERBASE), pos0 = uint(pos0))
            cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                if (window) {
                    enc_kv_q8_store_b_cls(raw, h, s, pa, wgs)
                } else {
                    enc_kv_q8_store_cls(raw, h, s, pa, wgs)
                }
            }
            var want_k <- codec_want(KVDtype.q8_0, st.k, blocks, mir_bytes, KS_KVD)
            var want_v <- codec_want(KVDtype.q8_0, st.v, blocks, mir_bytes, KS_KVD)
            t |> equal(clamp_inf_scales(want_k, KVDtype.q8_0), 1, "{tag}: the K fixture's one block past the half range reaches the scale clamp")
            t |> equal(clamp_inf_scales(want_v, KVDtype.q8_0), 1, "{tag}: the V fixture's one block past the half range reaches the scale clamp")
            var got_k <- cp |> plane_as(km, type<uint8>, mir_bytes)
            var got_v <- cp |> plane_as(vm, type<uint8>, mir_bytes)
            check_exact(t, "{tag}, K vs the CPU cache's bytes (a scale past the half range clamped)", got_k, want_k)
            check_exact(t, "{tag}, V vs the CPU cache's bytes (a scale past the half range clamped)", got_v, want_v)
            t |> equal(int(int8(got_k[blocks[0] * block_bytes(KVDtype.q8_0) + scale_bytes + 5])), -127, "{tag}: the block's negative maximum quantizes to -127")
            delete want_k
            delete want_v
            delete got_k
            delete got_v
        }
        delete toks
        delete blocks
        delete st
    }
}

[test]
def test_vkc_q8_store(t0 : T?) {
    t0 |> run("the q8_0 K/V store == the CPU cache's bytes, at token rows and at window rows; a block scale past the half range clamped") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !(ensure_kv_q8_store_cls() && ensure_kv_q8_store_b_cls())) {
                t |> skip("no Vulkan device")
                return
            }
            q8_store_arm(t, false)
            q8_store_arm(t, true)
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

let private TQ_ROWS = 2
let private TQ_POS_PER_REGION = 9

//! One tq4 store arm at head width `hs`: `nqh` q heads rotated in place, `nkh` K and V heads rotated and quantized - at their
//! token rows (two regions), or under `window` from `pos0`. K's first head of row 1 and V's last sit past the half range.
def private tq_store_arm(t : T?; hs, nqh, nkh : int; window : bool = false) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let qd = max(nqh, 1) * hs   // a dispatch with no q head still binds a q plane, and one with no K head the staged planes
        let kvd = max(nkh, 1) * hs
        let nblk = kvd / 32
        let pos0 = 3
        let tag = "kv_tq4_store{window ? "_b" : ""} hs {hs}, {nqh} q heads, {nkh} k/v heads"
        t |> equal(KV_TQ4_SIGNS, SIGN_ROW, "the fixtures' sign row is the driver's")
        var signs <- tq4_sign_row()
        var q <- row_fill(TQ_ROWS * qd, 41, 3, 0.05, 1.8, 73)
        var kf <- row_fill(TQ_ROWS * kvd, 37, 11, 0.071, 3.3)
        var vf <- row_fill(TQ_ROWS * kvd, 29, 5, 0.043, 2.0, 89)
        for (i in range(32)) {
            vf[32 + i] = 0.0   // an all-zero block: the quantizer's zero-scale edge
        }
        for (i in range(hs)) {
            kf[kvd + i] *= PAST_HALF_RANGE
            vf[2 * kvd - hs + i] *= PAST_HALF_RANGE
        }
        var toks : array<TokMeta>
        toks |> resize(TQ_ROWS)
        toks[0].pos = 4u
        toks[1].pos = 2u
        toks[1].mirbase = uint(TQ_POS_PER_REGION * nblk)
        let mir_bytes = mirror_bytes(KVDtype.tq4, 2 * TQ_POS_PER_REGION * nblk)
        var blocks <- [for (r in range(TQ_ROWS)); ODD_LAYERBASE + (window ? (pos0 + r) * nblk : int(toks[r].mirbase) + int(toks[r].pos) * nblk)]
        var want_q <- nqh > 0 ? tq4_rotated(q, TQ_ROWS, qd, hs, signs) : clone_to_move(q)
        var kr <- tq4_rotated(kf, TQ_ROWS, kvd, hs, signs)
        var vr <- tq4_rotated(vf, TQ_ROWS, kvd, hs, signs)
        var stored <- [for (b in blocks); b; where nkh > 0]   // the blocks a K head lands at: none on a dispatch that stores no row
        var want_k <- codec_want(KVDtype.tq4, kr, stored, mir_bytes, kvd)
        var want_v <- codec_want(KVDtype.tq4, vr, stored, mir_bytes, kvd)
        t |> success(nkh == 0 || clamp_inf_scales(want_k, KVDtype.tq4) > 0, "{tag}: the K fixture's head past the half range reaches the scale clamp")
        t |> success(nkh == 0 || clamp_inf_scales(want_v, KVDtype.tq4) > 0, "{tag}: the V fixture's head past the half range reaches the scale clamp")
        delete stored
        with_cell_planes() $(var cp : CellPlanes) {
            let qp = cp |> io_(q)
            let kp = cp |> in_(kf)
            let vp = cp |> in_(vf)
            let km = cp |> out_words(int64(mir_bytes), MIRROR_FILL)
            let vm = cp |> out_words(int64(mir_bytes), MIRROR_FILL)
            let sp = cp |> in_(signs)
            let tp = cp |> in_plain(toks)
            var s = (window ? cp |> cls_set(fixed_array(qp, kp, vp, km, vm, sp), @@set_kv_tq4_store_b_cls)
                : cp |> cls_set(fixed_array(qp, kp, vp, km, vm, sp, tp), @@set_kv_tq4_store_cls))
            var pa = KvTqStoreArgs(kvd = uint(kvd), hs = uint(hs), layerbase = uint(ODD_LAYERBASE), pos0 = uint(pos0),
                nqh = uint(nqh), nkh = uint(nkh), qrow = uint(qd), qstride = uint(hs), inv_sqrt_hs = 1.0 / sqrt(float(hs)))
            cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                if (window) {
                    enc_kv_tq4_store_b_cls(raw, h, s, pa, int64(TQ_ROWS * (nqh + 2 * nkh)))
                } else {
                    enc_kv_tq4_store_cls(raw, h, s, pa, int64(TQ_ROWS * (nqh + 2 * nkh)))
                }
            }
            var got_q <- cp |> plane_run(qp)
            var got_k <- cp |> plane_as(km, type<uint8>, mir_bytes)
            var got_v <- cp |> plane_as(vm, type<uint8>, mir_bytes)
            check_rows(t, "{tag}: the q rows{nqh > 0 ? " rotated in place vs the CPU rotation" : " left alone"}", got_q, want_q, BAR_EXACT, qd, hs)
            if (nqh > 0) {
                t |> success(check_count(got_q, q, BAR_EXACT) > 0, "{tag} control: the rotation moves the q rows off their input")
            }
            check_exact(t, "{tag}: the K rows vs the CPU rotation and cache bytes (a scale past the half range clamped)", got_k, want_k)
            check_exact(t, "{tag}: the V rows vs the CPU rotation and cache bytes (a scale past the half range clamped)", got_v, want_v)
            delete got_q
            delete got_k
            delete got_v
        }
        delete signs
        delete q
        delete kf
        delete vf
        delete toks
        delete blocks
        delete want_q
        delete kr
        delete vr
        delete want_k
        delete want_v
    }
}

[test]
def test_vkc_tq4_store(t0 : T?) {
    t0 |> run("the tq4 K/V store == the CPU rotation and the CPU cache's bytes, the q rows rotated in place, at token rows and at window rows") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !(ensure_kv_tq4_store_cls() && ensure_kv_tq4_store_b_cls())) {
                t |> skip("no Vulkan device")
                return
            }
            tq_store_arm(t, 64, 4, 2)
            tq_store_arm(t, 128, 2, 1)
            tq_store_arm(t, 512, 1, 1)   // the wide head: a thread's second element
            tq_store_arm(t, 32, 3, 3)    // one block a head
            tq_store_arm(t, 64, 4, 0)    // a shared-KV layer: q rotates, nothing stores
            tq_store_arm(t, 64, 4, 2, true)
            tq_store_arm(t, 64, 0, 2, true)   // the NextN head's prompt warm: no q head to rotate
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkc_q8_da_attn(t0 : T?) {
    t0 |> run("the decode attention's q8_0 stamps == the CPU attention over the dequantized mirror rows, on every path of the pass") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !kv_codec_cls_ensure(KVDtype.q8_0)) {
                t |> skip("no Vulkan device")
                return
            }
            codec_attn_arms(t, KVDtype.q8_0)
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkc_tq4_da_attn(t0 : T?) {
    t0 |> run("the decode attention's tq4 stamps == the CPU attention over the dequantized rotated rows, on every path of the pass") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !kv_codec_cls_ensure(KVDtype.tq4)) {
                t |> skip("no Vulkan device")
                return
            }
            codec_attn_arms(t, KVDtype.tq4)
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

let private SH_MIRROR_POS = 21
let private SH_POS0 = 5
let private SH_ROWS = 13

//! One shadow arm: `SH_MIRROR_POS` positions of `kvd` in the mirror from block `ODD_LAYERBASE`, the pass over positions
//! [SH_POS0, SH_POS0 + SH_ROWS). K's first block of the first position read dequantizes past the half range.
def private shadow_arm(t : T?; kv : KVDtype; kvd : int) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let tq4 = kv == KVDtype.tq4
        let tag = tq4 ? "kv_tq4_shadow" : "kv_q8_shadow"
        var kf <- row_fill(SH_MIRROR_POS * kvd, 37, 11, 0.071, 3.3)
        var vf <- row_fill(SH_MIRROR_POS * kvd, 29, 5, 0.043, 2.0, 89)
        for (i in range(32)) {
            kf[SH_POS0 * kvd + i] *= PAST_HALF_VALUES
        }
        var km <- codec_mirror_of_stored(kv, kf, SH_MIRROR_POS, kvd, ODD_LAYERBASE)   // the shadow reads blocks, whatever basis their rows are in
        var vm <- codec_mirror_of_stored(kv, vf, SH_MIRROR_POS, kvd, ODD_LAYERBASE)
        let halves = SH_MIRROR_POS * kvd
        with_cell_planes() $(var cp : CellPlanes) {
            let planes = fixed_array(cp |> in_plain(km.bytes), cp |> in_plain(vm.bytes),
                cp |> out_words(int64(halves) * 2l, HALF_NAN_PAIR), cp |> out_words(int64(halves) * 2l, HALF_NAN_PAIR))
            var s = tq4 ? cp |> cls_set(planes, @@set_kv_tq4_shadow_cls) : cp |> cls_set(planes, @@set_kv_q8_shadow_cls)
            var pa = KvQShadowArgs(kvd = uint(kvd), pos0 = uint(SH_POS0), rows = uint(SH_ROWS), layerbase = uint(ODD_LAYERBASE))
            let wgs = int64((kvd / 32 * SH_ROWS + 255) / 256)
            cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                if (tq4) {
                    enc_kv_tq4_shadow_cls(raw, h, s, pa, wgs)
                } else {
                    enc_kv_q8_shadow_cls(raw, h, s, pa, wgs)
                }
            }
            var got_k <- cp |> plane_as(planes[2], type<uint16>, halves)
            var got_v <- cp |> plane_as(planes[3], type<uint16>, halves)
            var want_k : array<uint16>
            var want_v : array<uint16>
            want_k |> resize(halves)
            want_v |> resize(halves)
            want_k |> fill(HALF_NAN)
            want_v |> fill(HALF_NAN)
            var clamped = 0
            for (i in range(SH_POS0 * kvd, (SH_POS0 + SH_ROWS) * kvd)) {
                want_k[i] = uint16(f32_to_f16(clamp(km.deq[i], -65504.0, 65504.0)))
                want_v[i] = uint16(f32_to_f16(clamp(vm.deq[i], -65504.0, 65504.0)))
                clamped += (uint(want_k[i]) & 0x7FFFu) == uint(HALF_MAX) ? 1 : 0
            }
            t |> success(clamped > 0, "{tag}: the fixture's block past the half range reaches the clamp ({clamped} halves at 65504)")
            check_exact(t, "{tag} K rows vs the CPU dequant as halves, clamped to the half range", got_k, want_k)
            check_exact(t, "{tag} V rows vs the CPU dequant as halves", got_v, want_v)
            delete got_k
            delete got_v
            delete want_k
            delete want_v
        }
        delete kf
        delete vf
        delete km
        delete vm
    }
}

[test]
def test_vkc_shadow(t0 : T?) {
    t0 |> run("a block-codec mirror's f16 shadow == the CPU dequant rounded to half and clamped, bit for bit, the rows outside the pass left alone") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !(ensure_kv_q8_shadow_cls() && ensure_kv_tq4_shadow_cls())) {
                t |> skip("no Vulkan device")
                return
            }
            shadow_arm(t, KVDtype.q8_0, 96)
            shadow_arm(t, KVDtype.tq4, 128)
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

let private UR_ROWS = 3

//! one un-rotation arm: `nh` heads `hs` wide over `UR_ROWS` rows, gated by the q plane's gate halves when `gated`
def private ur_arm(t : T?; hs, nh : int; gated : bool) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let tag = "kv_tq4_unrot hs {hs}, {nh} heads{gated ? ", gated" : ""}"
        let orow = nh * hs
        let qstride = gated ? 2 * hs : hs
        let qrow = nh * qstride
        var signs <- tq4_sign_row()
        var y <- row_fill(UR_ROWS * orow, 31, 7, 0.06, 2.4, 83)
        var q <- row_fill(UR_ROWS * qrow, 43, 5, 0.05, 2.0, 79)
        var want := y
        tq4_unrotate_batch(want, int64(UR_ROWS), int64(orow), int64(hs), unsafe(addr<float const?>(signs[0])))
        var ungated := want
        if (gated) {
            for (r in range(UR_ROWS)) {
                for (hh in range(nh)) {
                    for (i in range(hs)) {
                        want[r * orow + hh * hs + i] /= 1.0 + exp(-q[r * qrow + hh * qstride + hs + i])
                    }
                }
            }
        }
        with_cell_planes() $(var cp : CellPlanes) {
            let yp = cp |> io_(y)
            let qp = cp |> in_(q)
            let sp = cp |> in_(signs)
            var s = cp |> cls_set(fixed_array(yp, qp, sp), @@set_kv_tq4_unrot_cls)
            var pa = KvTqUnrotArgs(hs = uint(hs), nh = uint(nh), orow = uint(orow), qrow = uint(qrow), qstride = uint(qstride),
                inv_sqrt_hs = 1.0 / sqrt(float(hs)))
            cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_kv_tq4_unrot_cls(raw, h, s, pa, int64(UR_ROWS * nh))
            }
            var got <- cp |> plane_run(yp)
            check_rows(t, "{tag} vs the CPU un-rotation{gated ? " under the gate" : ""}", got, want, gated ? BAR_APPROX : BAR_EXACT, orow, hs)
            t |> success(check_count(got, y, BAR_APPROX) > 0, "{tag} control: the un-rotation moves the rows off their input")
            if (gated) {
                t |> success(check_count(got, ungated, BAR_APPROX) > 0, "{tag} control: the rows before the gate land outside the bar")
            }
            delete got
        }
        delete signs
        delete y
        delete q
        delete want
        delete ungated
    }
}

[test]
def test_vkc_tq4_unrot(t0 : T?) {
    t0 |> run("the tq4 un-rotation == the CPU un-rotation, bit for bit, and under the head gate") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_kv_tq4_unrot_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            ur_arm(t, 64, 4, false)
            ur_arm(t, 512, 1, false)
            ur_arm(t, 128, 2, true)
            ur_arm(t, 32, 3, false)
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}
